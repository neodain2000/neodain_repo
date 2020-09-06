/*
** File   : neo_os_file_api.c
**
** Purpose: This file Contains all of the api calls for manipulating
**          files in a file system for posix 
*/

namespace neodain {
namesapce neo {
namespace os {

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "sys/types.h"
#include "fcntl.h"
#include "unistd.h"
#include "errno.h"
#include "pthread.h"

#include "dirent.h"
#include "sys/stat.h"
#include "signal.h"

#include "neo_os_common_types.h"
#include "neo_os_api.h"

/****************************************************************************************
                                     DEFINES
****************************************************************************************/
#define ERROR -1
#define NEO_OS_REDIRECTSTRSIZE 15


/***************************************************************************************
                                 FUNCTION PROTOTYPES
***************************************************************************************/
int32         NEO_OS_check_name_length(const char *path);
extern uint32 NEO_OS_FindCreator(void);
extern int    NEO_OS_InterruptSafeLock(pthread_mutex_t *lock, sigset_t *set, sigset_t *previous);
extern void   NEO_OS_InterruptSafeUnlock(pthread_mutex_t *lock, sigset_t *previous);

/****************************************************************************************
                                   GLOBAL DATA
****************************************************************************************/
NEO_OS_FDTableEntry NEO_OS_FDTable[NEO_OS_MAX_NUM_OPEN_FILES];
pthread_mutex_t NEO_OS_FDTableMutex;

/****************************************************************************************
                                INITIALIZATION FUNCTION
****************************************************************************************/
int32 NEO_OS_FS_Init(void)
{
    int i;
    int ret;	

    /* Initialize the file system constructs */
    for (i =0; i < NEO_OS_MAX_NUM_OPEN_FILES; i++)
    {
        NEO_OS_FDTable[i].OSfd =       -1;
        strcpy(NEO_OS_FDTable[i].Path, "\0");
        NEO_OS_FDTable[i].User =       0;
        NEO_OS_FDTable[i].IsValid =    FALSE;
    }
    
    ret = pthread_mutex_init((pthread_mutex_t *) &NEO_OS_FDTableMutex, NULL); 

    if (ret < 0)
    {
        return(NEO_OS_ERROR);
    }
    else
    {
        return(NEO_OS_SUCCESS);
    }
}
/****************************************************************************************
                                    Filesys API
****************************************************************************************/

/*
** Standard File system API
*/

/*--------------------------------------------------------------------------------------
    Name: NEO_OS_creat
    
    Purpose: creates a file specified by const char *path, with read/write 
             permissions by access. The file is also automatically opened by the
             create call.
    
    Returns: NEO_OS_FS_ERR_INVALID_POINTER if path is NULL
             NEO_OS_FS_ERR_PATH_TOO_LONG if path exceeds the maximum number of chars
             NEO_OS_FS_ERR_PATH_INVALID if path cannot be parsed
             NEO_OS_FS_ERR_NAME_TOO_LONG if the name of the file is too long
             NEO_OS_FS_ERROR if permissions are unknown or OS call fails
             NEO_OS_FS_ERR_NO_FREE_FDS if there are no free file descripors left
             NEO_OS_FS_SUCCESS if success
    
---------------------------------------------------------------------------------------*/
int32 NEO_OS_creat (const char *path, int32 access)
{
    int        status;
    char       local_path[NEO_OS_MAX_LOCAL_PATH_LEN];
    int        perm;
    mode_t     mode;
    uint32     PossibleFD;
    sigset_t   previous;
    sigset_t   mask;

    /*
    ** Check to see if the path pointer is NULL
    */
    if (path == NULL)
    {
        return NEO_OS_FS_ERR_INVALID_POINTER;
    }
   
    /*
    ** Check to see if the path is too long
    */
    if (strlen(path) >= NEO_OS_MAX_PATH_LEN)
    {
        return NEO_OS_FS_ERR_PATH_TOO_LONG;
    }

    /* 
    ** check if the name of the file is too long 
    */
    if (NEO_OS_check_name_length(path) != NEO_OS_FS_SUCCESS)
    {
        return NEO_OS_FS_ERR_NAME_TOO_LONG;
    }

    /*
    ** Check for a valid access mode
    ** For creating a file, NEO_OS_READ_ONLY does not make sense
    */
    switch(access)
    {
        case NEO_OS_WRITE_ONLY:
            perm = O_WRONLY;
            break;
        case NEO_OS_READ_WRITE:
            perm = O_RDWR;
            break;
        default:
            return NEO_OS_FS_ERROR;
    }

    /*
    ** Translate the path
    */
    if (NEO_OS_TranslatePath(path, (char *)local_path) != NEO_OS_FS_SUCCESS)
    {
        return NEO_OS_FS_ERR_PATH_INVALID;
    }

    NEO_OS_InterruptSafeLock(&NEO_OS_FDTableMutex, &mask, &previous);

    for (PossibleFD = 0; PossibleFD < NEO_OS_MAX_NUM_OPEN_FILES; PossibleFD++)
    {
        if(NEO_OS_FDTable[PossibleFD].IsValid == FALSE)
        {
            break;
        }
    }

    if (PossibleFD >= NEO_OS_MAX_NUM_OPEN_FILES)
    {
        NEO_OS_InterruptSafeUnlock(&NEO_OS_FDTableMutex, &previous);
        return NEO_OS_FS_ERR_NO_FREE_FDS;
    }


    /* Mark the table entry as valid so no other 
     * task can take that ID */
    NEO_OS_FDTable[PossibleFD].IsValid =    TRUE;

    NEO_OS_InterruptSafeUnlock(&NEO_OS_FDTableMutex, &previous);

    mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
   
    status =  open(local_path, perm | O_CREAT | O_TRUNC, mode);

    NEO_OS_InterruptSafeLock(&NEO_OS_FDTableMutex, &mask, &previous);

    if (status != ERROR)
    {
        /* fill in the table before returning */
        NEO_OS_FDTable[PossibleFD].OSfd =       status;
        strncpy(NEO_OS_FDTable[PossibleFD].Path, path, NEO_OS_MAX_PATH_LEN);
        NEO_OS_FDTable[PossibleFD].User =       NEO_OS_FindCreator();
        NEO_OS_InterruptSafeUnlock(&NEO_OS_FDTableMutex, &previous);
        return PossibleFD;
    }
    else
    {
        /* Operation failed, so reset to false */
        NEO_OS_FDTable[PossibleFD].IsValid = FALSE;
        NEO_OS_InterruptSafeUnlock(&NEO_OS_FDTableMutex, &previous);
        return NEO_OS_FS_ERROR;
    }

} /* end NEO_OS_creat */

/*--------------------------------------------------------------------------------------
    Name: NEO_OS_open
    
    Purpose: Opens a file. access parameters are NEO_OS_READ_ONLY, NEO_OS_WRITE_ONLY, or 
             NEO_OS_READ_WRITE

    Returns: NEO_OS_FS_ERR_INVALID_POINTER if path is NULL
             NEO_OS_FS_ERR_PATH_TOO_LONG if path exceeds the maximum number of chars
             NEO_OS_FS_ERR_PATH_INVALID if path cannot be parsed
             NEO_OS_FS_ERR_NAME_TOO_LONG if the name of the file is too long
             NEO_OS_FS_ERROR if permissions are unknown or OS call fails
             NEO_OS_FS_ERR_NO_FREE_FDS if there are no free file descriptors left
             a file descriptor if success
---------------------------------------------------------------------------------------*/
int32 NEO_OS_open(const char *path, int32 access, uint32 mode)
{
    int       status;
    char      local_path[NEO_OS_MAX_LOCAL_PATH_LEN];
    int       perm;
    uint32    PossibleFD;
    sigset_t  previous;
    sigset_t  mask;
    
    /*
    ** Check to see if the path pointer is NULL
    */
    if (path == NULL)
    {
        return NEO_OS_FS_ERR_INVALID_POINTER;
    }
   
    /*
    ** Check to see if the path is too long
    */
    if (strlen(path) >= NEO_OS_MAX_PATH_LEN)
    {
        return NEO_OS_FS_ERR_PATH_TOO_LONG;
    }

    /* 
    ** check if the name of the file is too long 
    */
    if (NEO_OS_check_name_length(path) != NEO_OS_FS_SUCCESS)
    {
        return NEO_OS_FS_ERR_NAME_TOO_LONG;
    }
  
    /*
    ** Check for a valid access mode
    */
    switch(access)
    {
        case NEO_OS_READ_ONLY:
            perm = O_RDONLY;
            break;
        case NEO_OS_WRITE_ONLY:
            perm = O_WRONLY | O_CREAT;
            break;
        case NEO_OS_READ_WRITE:
            perm = O_RDWR | O_CREAT;
            break;
        default:
            return NEO_OS_FS_ERROR;
    }

    /*
    ** Translate the path
    */
    if (NEO_OS_TranslatePath(path, (char *)local_path) != NEO_OS_FS_SUCCESS)
    {
        return NEO_OS_FS_ERR_PATH_INVALID;
    }
    
    NEO_OS_InterruptSafeLock(&NEO_OS_FDTableMutex, &mask, &previous);

    for (PossibleFD = 0; PossibleFD < NEO_OS_MAX_NUM_OPEN_FILES; PossibleFD++)
    {
        if(NEO_OS_FDTable[PossibleFD].IsValid == FALSE)
        {
            break;
        }
    }

    if (PossibleFD >= NEO_OS_MAX_NUM_OPEN_FILES)
    {
        NEO_OS_InterruptSafeUnlock(&NEO_OS_FDTableMutex, &previous);
        return NEO_OS_FS_ERR_NO_FREE_FDS;
    }

    /* Mark the table entry as valid so no other 
     * task can take that ID */
    NEO_OS_FDTable[PossibleFD].IsValid = TRUE;

    NEO_OS_InterruptSafeUnlock(&NEO_OS_FDTableMutex, &previous);

    /* open the file  */
    status =  open(local_path, perm, mode);

    NEO_OS_InterruptSafeLock(&NEO_OS_FDTableMutex, &mask, &previous);

    if (status != ERROR)
    {
        /* fill in the table before returning */
        NEO_OS_FDTable[PossibleFD].OSfd =       status;
        strncpy(NEO_OS_FDTable[PossibleFD].Path, path, NEO_OS_MAX_PATH_LEN);
        NEO_OS_FDTable[PossibleFD].User =       NEO_OS_FindCreator();
        NEO_OS_InterruptSafeUnlock(&NEO_OS_FDTableMutex, &previous);
        
        return PossibleFD;
    }
    else
    {
        /* Operation failed, so reset to false */
        NEO_OS_FDTable[PossibleFD].IsValid = FALSE;
        NEO_OS_InterruptSafeUnlock(&NEO_OS_FDTableMutex, &previous);
        return NEO_OS_FS_ERROR;
    }
 
} /* end NEO_OS_open */


/*--------------------------------------------------------------------------------------
    Name: NEO_OS_close
    
    Purpose: Closes a file. 

    Returns: NEO_OS_FS_ERROR if file  descriptor could not be closed
             NEO_OS_FS_ERR_INVALID_FD if the file descriptor passed in is invalid
             NEO_OS_FS_SUCCESS if success
---------------------------------------------------------------------------------------*/
int32 NEO_OS_close(int32 filedes)
{
    int       status;
    sigset_t  previous;
    sigset_t  mask;

    /* Make sure the file descriptor is legit before using it */
    if (filedes < 0 || filedes >= NEO_OS_MAX_NUM_OPEN_FILES || NEO_OS_FDTable[filedes].IsValid == FALSE)
    {
        return NEO_OS_FS_ERR_INVALID_FD;
    }
    else
    {    

        /*
        ** call close, and check for an interrupted system call 
        */
        do {
            status = close((int)NEO_OS_FDTable[filedes].OSfd);
        } while ( status == -1 && errno == EINTR );

        if (status == ERROR)
        {
            /*
            ** Remove the file from the OSAL list
            ** to free up that slot 
            */
            /* fill in the table before returning */
            NEO_OS_InterruptSafeLock(&NEO_OS_FDTableMutex, &mask, &previous);
            NEO_OS_FDTable[filedes].OSfd =       -1;
            strcpy(NEO_OS_FDTable[filedes].Path, "\0");
            NEO_OS_FDTable[filedes].User =       0;
            NEO_OS_FDTable[filedes].IsValid =    FALSE;
            NEO_OS_InterruptSafeUnlock(&NEO_OS_FDTableMutex, &previous);

            return NEO_OS_FS_ERROR;
        }
        else
        {
            /* fill in the table before returning */
            NEO_OS_InterruptSafeLock(&NEO_OS_FDTableMutex, &mask, &previous);
            NEO_OS_FDTable[filedes].OSfd =       -1;
            strcpy(NEO_OS_FDTable[filedes].Path, "\0");
            NEO_OS_FDTable[filedes].User =       0;
            NEO_OS_FDTable[filedes].IsValid =    FALSE;
            NEO_OS_InterruptSafeUnlock(&NEO_OS_FDTableMutex, &previous);
            
            return NEO_OS_FS_SUCCESS;
        }

    }
    
}/* end NEO_OS_close */

/*--------------------------------------------------------------------------------------
    Name: NEO_OS_read
    
    Purpose: reads up to nbytes from a file, and puts them into buffer. 
    
    Returns: NEO_OS_FS_ERR_INVALID_POINTER if buffer is a null pointer
             NEO_OS_FS_ERROR if OS call failed
             NEO_OS_FS_ERR_INVALID_FD if the file descriptor passed in is invalid
             number of bytes read if success
---------------------------------------------------------------------------------------*/
int32 NEO_OS_read (int32 filedes, void *buffer, uint32 nbytes)
{
    int32 status;

    if (buffer == NULL) return NEO_OS_FS_ERR_INVALID_POINTER;

    /* Make sure the file descriptor is legit before using it */
    if (filedes < 0 || filedes >= NEO_OS_MAX_NUM_OPEN_FILES || NEO_OS_FDTable[filedes].IsValid == FALSE)
    {
        return NEO_OS_FS_ERR_INVALID_FD;
    }
    else
    { 
        status = read(NEO_OS_FDTable[filedes].OSfd, buffer, nbytes);
 
        if (status == ERROR) return NEO_OS_FS_ERROR;
    }

    return status;
    
}/* end NEO_OS_read */

/*--------------------------------------------------------------------------------------
    Name: NEO_OS_write

    Purpose: writes to a file. copies up to a maximum of nbtyes of buffer to the file
             described in filedes

    Returns: NEO_OS_FS_ERR_INVALID_POINTER if buffer is NULL
             NEO_OS_FS_ERROR if OS call failed
             NEO_OS_FS_ERR_INVALID_FD if the file descriptor passed in is invalid
             number of bytes written if success
---------------------------------------------------------------------------------------*/
int32 NEO_OS_write(int32 filedes, void *buffer, uint32 nbytes)
{
    int32 status;

    if (buffer == NULL) return NEO_OS_FS_ERR_INVALID_POINTER;

    /* Make sure the file descriptor is legit before using it */
    if (filedes < 0 || filedes >= NEO_OS_MAX_NUM_OPEN_FILES || NEO_OS_FDTable[filedes].IsValid == FALSE)
    {
        return NEO_OS_FS_ERR_INVALID_FD;
    }
    else
    {
        status = write(NEO_OS_FDTable[filedes].OSfd, buffer, nbytes );
    
        if (status != ERROR) return  status;
        else return NEO_OS_FS_ERROR;
    }
    
}/* end NEO_OS_write */


/*--------------------------------------------------------------------------------------
    Name: NEO_OS_chmod

    Notes: This is not going to be implemented because there is no use for this function.
---------------------------------------------------------------------------------------*/
int32 NEO_OS_chmod (const char *path, uint32 access)
{    
    return NEO_OS_FS_UNIMPLEMENTED;
} /* end NEO_OS_chmod */

/*--------------------------------------------------------------------------------------
    Name: NEO_OS_stat
    
    Purpose: returns information about a file or directory in a os_fs_stat structure
    
    Returns: NEO_OS_FS_ERR_INVALID_POINTER if path or filestats is NULL
             NEO_OS_FS_ERR_PATH_TOO_LONG if the path is too long to be stored locally
*****        NEO_OS_FS_ERR_NAME_TOO_LONG if the name of the file is too long to be stored
             NEO_OS_FS_ERR_PATH_INVALID if path cannot be parsed
             NEO_OS_FS_ERROR id the OS call failed
             NEO_OS_FS_SUCCESS if success

    Note: The information returned is in the structure pointed to by filestats         
---------------------------------------------------------------------------------------*/
int32 NEO_OS_stat(const char *path, os_fstat_t *filestats)
{
    int ret_val;
    char local_path[NEO_OS_MAX_LOCAL_PATH_LEN];
  
    /*
    ** Check to see if the file pointers are NULL
    */
    if (path == NULL || filestats == NULL) return NEO_OS_FS_ERR_INVALID_POINTER;
        
    /*
    ** Check to see if the path is too long
    */
    if (strlen(path) >= NEO_OS_MAX_PATH_LEN) return NEO_OS_FS_ERR_PATH_TOO_LONG;

    /*
    ** Translate the path
    */
    if (NEO_OS_TranslatePath(path, (char *)local_path) != NEO_OS_FS_SUCCESS)
    {
        return NEO_OS_FS_ERR_PATH_INVALID;
    }

    ret_val = stat((char*)local_path, filestats);

    if (ret_val == ERROR) return NEO_OS_FS_ERROR;
    else return NEO_OS_FS_SUCCESS;
    
} /* end NEO_OS_stat */

/*--------------------------------------------------------------------------------------
    Name: NEO_OS_lseek

    Purpose: sets the read/write pointer to a specific offset in a specific file. 
             Whence is either NEO_OS_SEEK_SET,NEO_OS_SEEK_CUR, or NEO_OS_SEEK_END

    Returns: the new offset from the beginning of the file
             NEO_OS_FS_ERR_INVALID_FD if the file descriptor passed in is invalid
             NEO_OS_FS_ERROR if OS call failed
---------------------------------------------------------------------------------------*/
int32 NEO_OS_lseek(int32 filedes, int32 offset, uint32 whence)
{
    off_t status;
    int where;

    /* Make sure the file descriptor is legit before using it */
    if (filedes < 0 || filedes >= NEO_OS_MAX_NUM_OPEN_FILES || NEO_OS_FDTable[filedes].IsValid == FALSE)
    {
        return NEO_OS_FS_ERR_INVALID_FD;
    }
    else
    {
        switch(whence)
        {
            case NEO_OS_SEEK_SET:
                where = SEEK_SET;
                break;
            case NEO_OS_SEEK_CUR:
                where = SEEK_CUR;
                break;
            case NEO_OS_SEEK_END:
                where = SEEK_END;
                break;
            default:
                return NEO_OS_FS_ERROR;
        }
    
        status = lseek(NEO_OS_FDTable[filedes].OSfd, (off_t)offset, (int)where);

        if ((int)status != ERROR) return (int32) status;
        else return NEO_OS_FS_ERROR;
    }
 
}/* end NEO_OS_lseek */

/*--------------------------------------------------------------------------------------
    Name: NEO_OS_remove

    Purpose: removes a given filename from the drive
 
    Returns: NEO_OS_FS_SUCCESS if the driver returns OK
             NEO_OS_FS_ERROR if there is no device or the driver returns error
             NEO_OS_FS_ERR_INVALID_POINTER if path is NULL
             NEO_OS_FS_ERR_PATH_TOO_LONG if path is too long to be stored locally
             NEO_OS_FS_ERR_PATH_INVALID if path cannot be parsed
             NEO_OS_FS_ERR_NAME_TOO_LONG if the name of the file to remove is too long to be
             stored locally
---------------------------------------------------------------------------------------*/
int32 NEO_OS_remove(const char *path)
{
    int  i;
    int  status;
    char local_path[NEO_OS_MAX_LOCAL_PATH_LEN];

    /*
    ** Check to see if the path pointer is NULL
    */
    if (path == NULL)
    {
        return NEO_OS_FS_ERR_INVALID_POINTER;
    }
   
    /*
    ** Check to see if the path is too long
    */
    if (strlen(path) >= NEO_OS_MAX_PATH_LEN)
    {
        return NEO_OS_FS_ERR_PATH_TOO_LONG;
    }

    /* 
    ** check if the name of the file is too long 
    */
    if (NEO_OS_check_name_length(path) != NEO_OS_FS_SUCCESS)
    {
        return NEO_OS_FS_ERR_NAME_TOO_LONG;
    }

    /*
    ** Make sure the file is not open by the OSAL before deleting it 
    */
    for (i = 0; i < NEO_OS_MAX_NUM_OPEN_FILES; i++)
    {
        if ((NEO_OS_FDTable[i].IsValid == TRUE) && (strcmp(NEO_OS_FDTable[i].Path, path) == 0))
        {
            return NEO_OS_FS_ERROR;
        }
    }
 
    /*
    ** Translate the path
    */
    if (NEO_OS_TranslatePath(path, (char *)local_path) != NEO_OS_FS_SUCCESS)
    {
        return NEO_OS_FS_ERR_PATH_INVALID;
    }

    /*
    ** Call the system to remove the file
    */
    status = remove(local_path);
    if (status == 0)
    {
        return NEO_OS_FS_SUCCESS;
    }
    else
    {
        return NEO_OS_FS_ERROR;
    }
    
} /* end NEO_OS_remove */

/*--------------------------------------------------------------------------------------
    Name: NEO_OS_rename
    
    Purpose: renames a file

    Returns: NEO_OS_FS_SUCCESS if the rename works
             NEO_OS_FS_ERROR if the file could not be opened or renamed.
             NEO_OS_FS_ERR_INVALID_POINTER if old or new are NULL
             NEO_OS_FS_ERR_PATH_INVALID if path cannot be parsed
             NEO_OS_FS_ERR_PATH_TOO_LONG if the paths given are too long to be stored locally
             NEO_OS_FS_ERR_NAME_TOO_LONG if the new name is too long to be stored locally
---------------------------------------------------------------------------------------*/
int32 NEO_OS_rename (const char *old, const char *new)
{
    int status,i;
    char old_path[NEO_OS_MAX_LOCAL_PATH_LEN];
    char new_path[NEO_OS_MAX_LOCAL_PATH_LEN];

    /*
    ** Check to see if the path pointers are NULL
    */    
    if (old == NULL || new == NULL)
        return NEO_OS_FS_ERR_INVALID_POINTER;

    /*
    ** Check to see if the paths are too long
    */
    if (strlen(old) >= NEO_OS_MAX_PATH_LEN)
        return NEO_OS_FS_ERR_PATH_TOO_LONG;
    
    if (strlen(new) >= NEO_OS_MAX_PATH_LEN)
        return NEO_OS_FS_ERR_PATH_TOO_LONG;

    /* 
    ** check if the names of the files are too long 
    */
    if (NEO_OS_check_name_length(old) != NEO_OS_FS_SUCCESS)
        return NEO_OS_FS_ERR_NAME_TOO_LONG;

    if (NEO_OS_check_name_length(new) != NEO_OS_FS_SUCCESS)
        return NEO_OS_FS_ERR_NAME_TOO_LONG;


    /*
    ** Translate the path
    */
    if (NEO_OS_TranslatePath(old, (char *)old_path) != NEO_OS_FS_SUCCESS)
    {
        return NEO_OS_FS_ERR_PATH_INVALID;
    }

    /*
    ** Translate the path
    */
    if (NEO_OS_TranslatePath(new, (char *)new_path) != NEO_OS_FS_SUCCESS)
    {
        return NEO_OS_FS_ERR_PATH_INVALID;
    }
     
    status = rename(old_path, new_path);
    if (status == 0)
    {
        for (i =0; i < NEO_OS_MAX_NUM_OPEN_FILES; i++) 
        {
            if (strcmp(NEO_OS_FDTable[i].Path, old) == 0 &&
                NEO_OS_FDTable[i].IsValid == TRUE)
            {
                strncpy (NEO_OS_FDTable[i].Path, new, NEO_OS_MAX_PATH_LEN);  
            } 
        }
        return NEO_OS_FS_SUCCESS;
    }
    else
    {
        return NEO_OS_FS_ERROR;
    }
     
}/*end NEO_OS_rename */

/*--------------------------------------------------------------------------------------
    Name: NEO_OS_cp
    
    Purpose: Copies a single file from src to dest

    Returns: NEO_OS_FS_SUCCESS if the operation worked
             NEO_OS_FS_ERROR if the file could not be accessed
             NEO_OS_FS_ERR_INVALID_POINTER if src or dest are NULL
             NEO_OS_FS_ERR_PATH_INVALID if path cannot be parsed
             NEO_OS_FS_ERR_PATH_TOO_LONG if the paths given are too long to be stored locally
             NEO_OS_FS_ERR_NAME_TOO_LONG if the dest name is too long to be stored locally

---------------------------------------------------------------------------------------*/
int32 NEO_OS_cp(const char *src, const char *dest)
{
    int  i;
    int  status;
    char src_path[NEO_OS_MAX_LOCAL_PATH_LEN];
    char dest_path[NEO_OS_MAX_LOCAL_PATH_LEN];

    /* leave enough space for the two paths and the command */
    char command[NEO_OS_MAX_PATH_LEN * 2 + 5];
    
    /*
    ** Check to see if the path pointers are NULL
    */    
    if (src == NULL || dest == NULL)
    {
        return NEO_OS_FS_ERR_INVALID_POINTER;
    }

    /*
    ** Check to see if the paths are too long
    */
    if (strlen(src) >= NEO_OS_MAX_PATH_LEN)
    {
        return NEO_OS_FS_ERR_PATH_TOO_LONG;
    }
    
    if (strlen(dest) >= NEO_OS_MAX_PATH_LEN)
    {
        return NEO_OS_FS_ERR_PATH_TOO_LONG;
    }

    /* 
    ** check if the names of the files are too long 
    */
    if (NEO_OS_check_name_length(src) != NEO_OS_FS_SUCCESS)
    {
        return NEO_OS_FS_ERR_NAME_TOO_LONG;
    }

    if (NEO_OS_check_name_length(dest) != NEO_OS_FS_SUCCESS)
    {
        return NEO_OS_FS_ERR_NAME_TOO_LONG;
    }

    /*
    ** Make sure the destintation file is not open by the OSAL before doing the copy 
    ** This may be caught by the host OS call but it does not hurt to 
    ** be consistent 
    */
    for (i =0; i < NEO_OS_MAX_NUM_OPEN_FILES; i++)
    {
        if ((NEO_OS_FDTable[i].IsValid == TRUE) &&
           (strcmp(NEO_OS_FDTable[i].Path, dest) == 0))
        {
            return NEO_OS_FS_ERROR;
        }
    }

    /*
    ** Translate the path
    */
    if (NEO_OS_TranslatePath(src, (char *)src_path) != NEO_OS_FS_SUCCESS)
    {
        return NEO_OS_FS_ERR_PATH_INVALID;
    }

    /*
    ** Translate the path
    */
    if (NEO_OS_TranslatePath(dest, (char *)dest_path) != NEO_OS_FS_SUCCESS)
    {
        return NEO_OS_FS_ERR_PATH_INVALID;
    }

    sprintf(command,"cp %s %s", src_path, dest_path);

    status = system(command);
    if (status == 0)
    {
        return NEO_OS_FS_SUCCESS;
    }
    else
    {
        return NEO_OS_FS_ERROR;
    }
     
}/*end NEO_OS_cp */

/*--------------------------------------------------------------------------------------
    Name: NEO_OS_mv
    
    Purpose: moves a single file from src to dest

    Returns: NEO_OS_FS_SUCCESS if the rename works
             NEO_OS_FS_ERROR if the file could not be opened or renamed.
             NEO_OS_FS_ERR_INVALID_POINTER if src or dest are NULL
             NEO_OS_FS_ERR_PATH_INVALID if path cannot be parsed
             NEO_OS_FS_ERR_PATH_TOO_LONG if the paths given are too long to be stored locally
             NEO_OS_FS_ERR_NAME_TOO_LONG if the dest name is too long to be stored locally
---------------------------------------------------------------------------------------*/
int32 NEO_OS_mv(const char *src, const char *dest)
{
    int i;
    int32 status;

    /*
    ** Validate the source and destination
    ** These checks may seem redundant because NEO_OS_cp and NEO_OS_remove also do
    ** the same checks, but this call needs to abort before doing a copy
    ** in some cases.
    */

    /*
    ** Check to see if the path pointers are NULL
    */
    if (src == NULL || dest == NULL)
    {
        return NEO_OS_FS_ERR_INVALID_POINTER;
    }

    /*
    ** Check to see if the paths are too long
    */
    if (strlen(src) >= NEO_OS_MAX_PATH_LEN)
    {
        return NEO_OS_FS_ERR_PATH_TOO_LONG;
    }

    if (strlen(dest) >= NEO_OS_MAX_PATH_LEN)
    {
        return NEO_OS_FS_ERR_PATH_TOO_LONG;
    }

    /*
    ** check if the names of the files are too long
    */
    if (NEO_OS_check_name_length(src) != NEO_OS_FS_SUCCESS)
    {
        return NEO_OS_FS_ERR_NAME_TOO_LONG;
    }
    
    if (NEO_OS_check_name_length(dest) != NEO_OS_FS_SUCCESS)
    {
        return NEO_OS_FS_ERR_NAME_TOO_LONG;
    }

    /*
    ** Make sure the source file is not open by the OSAL before doing the move
    */
    for (i =0; i < NEO_OS_MAX_NUM_OPEN_FILES; i++)
    {
        if ((NEO_OS_FDTable[i].IsValid == TRUE) &&
            (strcmp(NEO_OS_FDTable[i].Path, src) == 0))
        {
            return NEO_OS_FS_ERROR;
        }
    }

    status = NEO_OS_cp (src, dest);
    if (status == NEO_OS_FS_SUCCESS)
    {
        status = NEO_OS_remove(src);
    }

    return ( status);
}/*end NEO_OS_mv */

/*
** Directory API 
*/
/*--------------------------------------------------------------------------------------
    Name: NEO_OS_mkdir

    Purpose: makes a directory specified by path.

    Returns: NEO_OS_FS_ERR_INVALID_POINTER if path is NULL
             NEO_OS_FS_ERR_PATH_TOO_LONG if the path is too long to be stored locally
             NEO_OS_FS_ERR_PATH_INVALID if path cannot be parsed
             NEO_OS_FS_ERROR if the OS call fails
             NEO_OS_FS_SUCCESS if success

    Note: The access parameter is currently unused.
---------------------------------------------------------------------------------------*/

int32 NEO_OS_mkdir(const char *path, uint32 access)
{
    int status;
    mode_t mode;
    char local_path[NEO_OS_MAX_LOCAL_PATH_LEN];

    /*
    ** Check to see if the path pointer is NULL
    */    
    if (path == NULL) return NEO_OS_FS_ERR_INVALID_POINTER;

    /*
    ** Check to see if the path is too long
    */      
    if (strlen(path) >= NEO_OS_MAX_PATH_LEN) return NEO_OS_FS_ERR_PATH_TOO_LONG;


    /*
    ** Translate the path
    */
    if (NEO_OS_TranslatePath(path, (char *)local_path) != NEO_OS_FS_SUCCESS)
    {
        return NEO_OS_FS_ERR_PATH_INVALID;
    }
    
    mode = S_IFDIR |S_IRWXU | S_IRWXG | S_IRWXO;
    status = mkdir(local_path, mode);

    if (status == 0)
    {
        return NEO_OS_FS_SUCCESS;
    }
    else
    {
        return NEO_OS_FS_ERROR;
    }
    
}/* end NEO_OS_mkdir */

/*--------------------------------------------------------------------------------------
    Name: NEO_OS_opendir

    Purpose: opens a directory for searching

    Returns: NULL if there is an error  
             a pointer to a directory if success
             
---------------------------------------------------------------------------------------*/
os_dirp_t NEO_OS_opendir(const char *path)
{

    os_dirp_t dirdescptr;
    char local_path[NEO_OS_MAX_LOCAL_PATH_LEN];

    /*
    ** Check to see if the path pointer is NULL
    */        
    if (path == NULL)
    {
        return NULL;
    }

    /*
    ** Check to see if the path is too long
    */      
    if (strlen(path) > NEO_OS_MAX_PATH_LEN)
    {
        return NULL;
    }
   
    /*
    ** Translate the path
    */
    if (NEO_OS_TranslatePath(path, (char *)local_path) != NEO_OS_FS_SUCCESS)
    {
        return NULL;
    }
   
    dirdescptr = opendir( (char*) local_path);
    
    if (dirdescptr == NULL)
    {
        return NULL;
    }
    else
    {
        return dirdescptr;
    }
    
} /* end NEO_OS_opendir */

/*--------------------------------------------------------------------------------------
    Name: NEO_OS_closedir
    
    Purpose: closes a directory

    Returns: NEO_OS_FS_SUCCESS if success
             NEO_OS_FS_ERROR if close failed
---------------------------------------------------------------------------------------*/
int32 NEO_OS_closedir(os_dirp_t directory)
{ 
    int status;

    if (directory == NULL)
    {
        return NEO_OS_FS_ERR_INVALID_POINTER;
    }

    status = closedir(directory);
    if (status != ERROR)
    {
        return NEO_OS_FS_SUCCESS;
    }
    else
    {
        return NEO_OS_FS_ERROR;
    }

} /* end NEO_OS_closedir */

/*--------------------------------------------------------------------------------------
    Name: NEO_OS_readdir

    Purpose: obtains directory entry data for the next file from an open directory

    Returns: a pointer to the next entry for success
             NULL if error or end of directory is reached
---------------------------------------------------------------------------------------*/
os_dirent_t *NEO_OS_readdir(os_dirp_t directory)
{ 
    os_dirent_t *tempptr;

    if (directory == NULL) return NULL;

    tempptr = readdir(directory);
    
    if (tempptr != NULL)
    {
        return tempptr;
    }
    else
    {
        return NULL;
    }
        
} /* end NEO_OS_readdir */

/*--------------------------------------------------------------------------------------
    Name: NEO_OS_rewinddir

    Purpose: Rewinds the directory pointer

    Returns: N/A
---------------------------------------------------------------------------------------*/
void  NEO_OS_rewinddir(os_dirp_t directory )
{
    if (directory != NULL)
    {
        rewinddir( directory);
    }
}

/*--------------------------------------------------------------------------------------
    Name: NEO_OS_rmdir
    
    Purpose: removes a directory from  the structure (must be an empty directory)

    Returns: NEO_OS_FS_ERR_INVALID_POINTER if path is NULL
             NEO_OS_FS_ERR_PATH_INVALID if path cannot be parsed    
             NEO_OS_FS_ER_PATH_TOO_LONG
             NEO_OS_FS_SUCCESS on success
             NEO_OS_FS_ERROR if the directory remove operation failed
---------------------------------------------------------------------------------------*/
int32  NEO_OS_rmdir(const char *path)
{
    int status;
    char local_path[NEO_OS_MAX_LOCAL_PATH_LEN];

    /*
    ** Check to see if the path pointer is NULL
    */            
    if (path == NULL)
    {
        return NEO_OS_FS_ERR_INVALID_POINTER;
    }

    /*
    ** Check to see if the path is too long
    */      
    if (strlen(path) >= NEO_OS_MAX_PATH_LEN)
    {
        return NEO_OS_FS_ERR_PATH_TOO_LONG;
    }

    /*
    ** Translate the path
    */
    if (NEO_OS_TranslatePath(path, (char *)local_path) != NEO_OS_FS_SUCCESS)
    {
        return NEO_OS_FS_ERR_PATH_INVALID;
    }
    
    status = rmdir(local_path);
    
    if (status == 0)
    {
        return NEO_OS_FS_SUCCESS;
    }
    else
    {
        return NEO_OS_FS_ERROR;
    }
    
}/* end NEO_OS_rmdir */

/* --------------------------------------------------------------------------------------
    Name: NEO_OS_check_name_length
    
    Purpose: Checks the length of the file name at the end of the path.
    
    Returns: NEO_OS_FS_ERROR if path is NULL, path is too long, there is no '/' in the path
             name, the name is too long
             NEO_OS_SUCCESS if success
            
    NOTE: This is only an internal function and is not intended for use by the user
 ---------------------------------------------------------------------------------------*/
int32 NEO_OS_check_name_length(const char *path)
{
    char* name_ptr;

    if (path == NULL)
    {
        return NEO_OS_FS_ERR_INVALID_POINTER;
    }

    if (strlen(path) > NEO_OS_MAX_PATH_LEN)
    {
        return NEO_OS_FS_ERROR;
    }

    /*
    ** All names passed into this function are REQUIRED to contain at
    ** least one directory separator. Find the last one.
    */
    name_ptr = strrchr(path, '/');
    if (name_ptr == NULL)
    {
        return NEO_OS_FS_ERROR;
    }

    /*
    ** Advance the pointer past the directory separator so that it
    ** indicates the final component of the path.
    */
    name_ptr ++;

    /*
     * Reject paths whose final component is too long.
     */
    if(strlen(name_ptr) > NEO_OS_MAX_FILE_NAME)
    {
        return NEO_OS_FS_ERROR;
    }

    return NEO_OS_FS_SUCCESS;

}/* end NEO_OS_check_name_length */

/* --------------------------------------------------------------------------------------
    Name: NEO_OS_ShellOutputToFile
    
    Purpose: Takes a shell command in and writes the output of that command to the specified file
    
    Returns: NEO_OS_FS_ERROR if the command was not executed properly
             NEO_OS_FS_ERR_INVALID_FD if the file descriptor passed in is invalid
             NEO_OS_SUCCESS if success
 ---------------------------------------------------------------------------------------*/
int32 NEO_OS_ShellOutputToFile(char* Cmd, int32 NEO_OS_fd)
{
    
    /* 
    ** this is a #define to avoid a 'variable length array' warning 
    ** 15 is for the size of the redirection string that is added 
    ** to the command 
    */
    char  LocalCmd[NEO_OS_MAX_CMD_LEN + NEO_OS_REDIRECTSTRSIZE]; 
    int32 ReturnCode;
    int32 Result;

    /*
    ** Check parameters
    */
    if (Cmd == NULL)
    {
        return(NEO_OS_FS_ERR_INVALID_POINTER);
    }

    /* 
    ** Make sure the file descriptor is valid before using it 
    */
    if (NEO_OS_fd < 0 || NEO_OS_fd >= NEO_OS_MAX_NUM_OPEN_FILES || NEO_OS_FDTable[NEO_OS_fd].IsValid == FALSE)
    {
        return NEO_OS_FS_ERR_INVALID_FD;
    }
    else
    {
    
        /* Make sure that we are able to access this file */
        fchmod(NEO_OS_FDTable[NEO_OS_fd].OSfd,0777);
  
        /* 
        ** add in the extra chars necessary to perform the redirection
        ** 1 for stdout and 2 for stderr. they are redirected to the 
        ** file descriptor passed in.
        **
        ** LocalCmd is defined above to be big enough to handle the largest
        ** allowed Cmd (NEO_OS_MAX_CMD_LEN characters) plus the longest possible
        ** redirect string size (NEO_OS_REDIRECTSTRSIZE). If those constants are
        */
        snprintf(LocalCmd, sizeof LocalCmd, "%s 1>&%d 2>&%d", 
            Cmd, (int)NEO_OS_FDTable[NEO_OS_fd].OSfd, (int)NEO_OS_FDTable[NEO_OS_fd].OSfd);
    
        Result = system(LocalCmd);
        if (Result == 0)
        {
            ReturnCode = NEO_OS_FS_SUCCESS;
        }
        else
        {
            ReturnCode = NEO_OS_FS_ERROR;
        }
    
        return ReturnCode;
    }
    /* should never get here */
    return NEO_OS_FS_ERROR;
}/* end NEO_OS_ShellOutputToFile */

/* --------------------------------------------------------------------------------------
Name: NEO_OS_FDGetInfo
    
Purpose: Copies the information of the given file descriptor into a structure passed in
    
Returns: NEO_OS_FS_ERR_INVALID_FD if the file descriptor passed in is invalid
         NEO_OS_FS_SUCCESS if the copying was successfull
 ---------------------------------------------------------------------------------------*/
int32 NEO_OS_FDGetInfo(int32 filedes, NEO_OS_FDTableEntry *fd_prop)
{

    if (fd_prop == NULL)
    {
        return(NEO_OS_FS_ERR_INVALID_POINTER);
    }

    /* Make sure the file descriptor is legit before using it */
    if (filedes < 0 || filedes >= NEO_OS_MAX_NUM_OPEN_FILES || NEO_OS_FDTable[filedes].IsValid == FALSE)
    {
       (*(fd_prop)).IsValid = FALSE; 
        return NEO_OS_FS_ERR_INVALID_FD;
    }
    else
    { 
        *fd_prop = NEO_OS_FDTable[filedes];
        return NEO_OS_FS_SUCCESS;
    }

}/* end NEO_OS_FDGetInfo */

/* --------------------------------------------------------------------------------------
   Name: NEO_OS_FileOpenCheck
    
   Purpose: Checks to see if a file is open 
    
   Returns: NEO_OS_FS_ERROR if the file is not open 
            NEO_OS_FS_SUCCESS if the file is open 
 ---------------------------------------------------------------------------------------*/
int32 NEO_OS_FileOpenCheck(char *Filename)
{
    uint32    i;
    sigset_t  previous;
    sigset_t  mask;

    if (Filename == NULL)
    {
        return(NEO_OS_FS_ERR_INVALID_POINTER);
    }

    NEO_OS_InterruptSafeLock(&NEO_OS_FDTableMutex, &mask, &previous);

    for (i = 0; i < NEO_OS_MAX_NUM_OPEN_FILES; i++)
    {
        if ((NEO_OS_FDTable[i].IsValid == TRUE) &&  (strcmp(NEO_OS_FDTable[i].Path, Filename) == 0))
        {
            NEO_OS_InterruptSafeUnlock(&NEO_OS_FDTableMutex, &previous);
            return(NEO_OS_FS_SUCCESS);
        }
    }/* end for */

    NEO_OS_InterruptSafeUnlock(&NEO_OS_FDTableMutex, &previous);
    return NEO_OS_FS_ERROR;

}/* end NEO_OS_FileOpenCheck */


/* --------------------------------------------------------------------------------------
   Name: NEO_OS_CloseFileByName

   Purpose: Allows a file to be closed by name.
            This will only work if the name passed in is the same name used to open 
            the file.

   Returns: NEO_OS_FS_ERR_PATH_INVALID if the file is not found 
            NEO_OS_FS_ERROR   if the file close returned an error
            NEO_OS_FS_SUCCESS if the file close suceeded  
 ---------------------------------------------------------------------------------------*/
int32 NEO_OS_CloseFileByName(char *Filename)
{
    uint32    i;
    int       status;
    sigset_t  previous;
    sigset_t  mask;

    if (Filename == NULL)
    {
        return(NEO_OS_FS_ERR_INVALID_POINTER);
    }

    NEO_OS_InterruptSafeLock(&NEO_OS_FDTableMutex, &mask, &previous);

    for (i = 0; i < NEO_OS_MAX_NUM_OPEN_FILES; i++)
    {
        if ((NEO_OS_FDTable[i].IsValid == TRUE) && (strcmp(NEO_OS_FDTable[i].Path, Filename) == 0))
        {
            /*
            ** Close the file
            */
            status = close ((int) NEO_OS_FDTable[i].OSfd);

            /*
            ** Next, remove the file from the OSAL list
            ** to free up that slot
            */
            NEO_OS_FDTable[i].OSfd =       -1;
            strcpy(NEO_OS_FDTable[i].Path, "\0");
            NEO_OS_FDTable[i].User =       0;
            NEO_OS_FDTable[i].IsValid =    FALSE;
            NEO_OS_InterruptSafeUnlock(&NEO_OS_FDTableMutex, &previous);

            if (status == 0)
            {
                return(NEO_OS_FS_SUCCESS);
            }
            else
            {
                return(NEO_OS_FS_ERROR);
            }
        }
    }/* end for */

    NEO_OS_InterruptSafeUnlock(&NEO_OS_FDTableMutex, &previous);

    return (NEO_OS_FS_ERR_PATH_INVALID);

}/* end NEO_OS_CloseFileByName */

/* --------------------------------------------------------------------------------------
   Name: NEO_OS_CloseAllFiles

   Purpose: Closes All open files that were opened through the OSAL 

   Returns: NEO_OS_FS_ERROR   if one or more file close returned an error
            NEO_OS_FS_SUCCESS if the files were all closed without error
 ---------------------------------------------------------------------------------------*/
int32 NEO_OS_CloseAllFiles(void)
{
    uint32    i;
    int32     return_status = NEO_OS_FS_SUCCESS;
    int       status;
    sigset_t  previous;
    sigset_t  mask;
    
    NEO_OS_InterruptSafeLock(&NEO_OS_FDTableMutex, &mask, &previous);

    for (i = 0; i < NEO_OS_MAX_NUM_OPEN_FILES; i++)
    {
        if (NEO_OS_FDTable[i].IsValid == TRUE )
        {
            /*
            ** Close the file
            */
            status = close ((int) NEO_OS_FDTable[i].OSfd);

            /*
            ** Next, remove the file from the OSAL list
            ** to free up that slot
            */
            NEO_OS_FDTable[i].OSfd =       -1;
            strcpy(NEO_OS_FDTable[i].Path, "\0");
            NEO_OS_FDTable[i].User =       0;
            NEO_OS_FDTable[i].IsValid =    FALSE;
            if (status == ERROR)
            {
                return_status = NEO_OS_FS_ERROR;
            }
        }
    }/* end for */

    NEO_OS_InterruptSafeUnlock(&NEO_OS_FDTableMutex, &previous);
    return (return_status);

}/* end NEO_OS_CloseAllFiles */


#ifdef __cplusplus
}
#endif

} // namespace os
} // namespace neo
} // namespace neodain


