/*
** File   : neo_os_file_sys.c
**
** Purpose: This file has the api's for all of the making
**          and mounting type of calls for file systems
*/

namespace neodain {
namespace neo {
namespace os {

#ifdef __cplusplus
extern "C" {
#endif


/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/statvfs.h>

#include "common_types.h"
#include "osapi.h"

#include <sys/vfs.h>

/****************************************************************************************
                                     DEFINES
****************************************************************************************/
# define ERROR (-1)
#undef NEO_OS_DEBUG_PRINTF

/****************************************************************************************
                                   GLOBAL DATA
****************************************************************************************/
/* 
** This is the volume table reference. It is defined in the BSP/startup code for the board
*/
extern NEO_OS_VolumeInfo_t NEO_OS_VolumeTable[NUM_TABLE_ENTRIES]; 

/*
** Fd Table
*/
extern NEO_OS_FDTableEntry NEO_OS_FDTable[NEO_OS_MAX_NUM_OPEN_FILES];

/****************************************************************************************
                                Filesys API
****************************************************************************************/
/*
** System Level API 
*/

/*---------------------------------------------------------------------------------------
    Name: NEO_OS_mkfs

    Purpose: Makes a file system on the target

    Returns: NEO_OS_FS_ERR_INVALID_POINTER if devname is NULL
             NEO_OS_FS_ERR_DRIVE_NOT_CREATED if the OS calls to create the the drive failed
             NEO_OS_FS_ERR_DEVICE_NOT_FREE if the volume table is full
             NEO_OS_FS_SUCCESS on creating the disk
    
---------------------------------------------------------------------------------------*/
int32 NEO_OS_mkfs(char *address, char *devname, char *volname, uint32 blocksize, uint32 numblocks)
{
    int i;
    
    /*
    ** Check parameters
    */
    if (devname == NULL || volname == NULL)
    {
        return NEO_OS_FS_ERR_INVALID_POINTER;
    }

    if(strlen(devname) >= NEO_OS_FS_DEV_NAME_LEN || strlen(volname) >= NEO_OS_FS_VOL_NAME_LEN)
    {
        return NEO_OS_FS_ERR_PATH_TOO_LONG;
    }

    /* find an open entry in the Volume Table */
    for (i = 0; i < NUM_TABLE_ENTRIES; i++)
    {
        if (NEO_OS_VolumeTable[i].FreeFlag == TRUE && NEO_OS_VolumeTable[i].IsMounted == FALSE
            && strcmp(NEO_OS_VolumeTable[i].DeviceName, devname) == 0)
            break;
    }

    if (i >= NUM_TABLE_ENTRIES)
    {
        return NEO_OS_FS_ERR_DEVICE_NOT_FREE;
    }

    /* make a disk if it is FS based */
    /*------------------------------- */
    if (NEO_OS_VolumeTable[i].VolumeType == FS_BASED)
    {
		/* now enter the info in the table */
		NEO_OS_VolumeTable[i].FreeFlag = FALSE;
		strcpy(NEO_OS_VolumeTable[i].VolumeName, volname);
		NEO_OS_VolumeTable[i].BlockSize = blocksize;
    }  
    else
    {
        return NEO_OS_FS_ERR_DRIVE_NOT_CREATED;
    }

    return NEO_OS_FS_SUCCESS; 
    
} /* end NEO_OS_mkfs */

/*---------------------------------------------------------------------------------------
    Name: NEO_OS_rmfs

    Purpose: Inititalizes a file system on the target
    
    Returns: NEO_OS_FS_ERR_INVALID_POINTER if devname is NULL
             NEO_OS_FS_ERROR is the drive specified cannot be located
             NEO_OS_FS_SUCCESS on removing  the disk
---------------------------------------------------------------------------------------*/
int32 NEO_OS_rmfs(char *devname)
{
    int i;
    int32 ReturnCode;

    if (devname ==NULL)
    {
        ReturnCode =  NEO_OS_FS_ERR_INVALID_POINTER;
    }
    else if (strlen(devname) >= NEO_OS_FS_DEV_NAME_LEN)
    {
        ReturnCode = NEO_OS_FS_ERR_PATH_TOO_LONG;
    }
    else
    {
        /* find this entry in the Volume Table */
        for (i = 0; i < NUM_TABLE_ENTRIES; i++)
        {
            if (NEO_OS_VolumeTable[i].FreeFlag == FALSE && NEO_OS_VolumeTable[i].IsMounted == FALSE
                && strcmp(NEO_OS_VolumeTable[i].DeviceName, devname) == 0)
            {
				break;
            }
        }

        /* We can't find that entry in the table */
        if (i >= NUM_TABLE_ENTRIES)
        {
            ReturnCode = NEO_OS_FS_ERROR;
        }
        else
        {
            /* Free this entry in the table */
            NEO_OS_VolumeTable[i].FreeFlag = TRUE;
            
            /* desconstruction of the filesystem to come later */

            ReturnCode = NEO_OS_FS_SUCCESS;
        }

    }
    return ReturnCode;
}/* end NEO_OS_rmfs */

/*---------------------------------------------------------------------------------------
    Name: NEO_OS_initfs

    Purpose: Inititalizes a file system on the target

    Returns: NEO_OS_FS_ERR_INVALID_POINTER if devname or volname are  NULL
             NEO_OS_FS_ERR_PATH_TOO_LONG if the name is too long
             NEO_OS_FS_ERR_DEVICE_NOT_FREE if the volume table is full
             NEO_OS_FS_ERR_DRIVE_NOT_CREATED on error
             NEO_OS_FS_SUCCESS on creating the disk
    
---------------------------------------------------------------------------------------*/
int32 NEO_OS_initfs(char *address,char *devname, char *volname, uint32 blocksize, uint32 numblocks)
{
    int i;
   
    if (devname == NULL || volname == NULL)
    {
        return NEO_OS_FS_ERR_INVALID_POINTER;
    }

    if(strlen(devname) >= NEO_OS_FS_DEV_NAME_LEN || strlen(volname) >= NEO_OS_FS_VOL_NAME_LEN)
    {
        return NEO_OS_FS_ERR_PATH_TOO_LONG;
    }

    /* find an open entry in the Volume Table */
    for (i = 0; i < NUM_TABLE_ENTRIES; i++)
    {
        if (NEO_OS_VolumeTable[i].FreeFlag == TRUE && NEO_OS_VolumeTable[i].IsMounted == FALSE
			&& strcmp(NEO_OS_VolumeTable[i].DeviceName, devname) == 0)
            break;
    }

    if (i >= NUM_TABLE_ENTRIES)
    {
        return NEO_OS_FS_ERR_DEVICE_NOT_FREE;
    }

    /* make a disk if it is FS based */
    /*------------------------------- */
    if (NEO_OS_VolumeTable[i].VolumeType == FS_BASED)
    {
       /* now enter the info in the table */
       NEO_OS_VolumeTable[i].FreeFlag = FALSE;
       strcpy(NEO_OS_VolumeTable[i].VolumeName, volname);
       NEO_OS_VolumeTable[i].BlockSize = blocksize;
    } 
    else
    {
        return NEO_OS_FS_ERR_DRIVE_NOT_CREATED;
    }

   return NEO_OS_FS_SUCCESS; 

}/* end NEO_OS_initfs */

/*--------------------------------------------------------------------------------------
    Name: NEO_OS_mount
    
    Purpose: mounts a drive.

---------------------------------------------------------------------------------------*/
int32 NEO_OS_mount (const char *devname, char* mountpoint)
{
    int i;

    /* Check parameters */
    if (devname == NULL || mountpoint == NULL)
    {
        return NEO_OS_FS_ERR_INVALID_POINTER;
    }

    if(strlen(devname) >= NEO_OS_FS_DEV_NAME_LEN || strlen(mountpoint) >= NEO_OS_MAX_PATH_LEN)
    {
        return NEO_OS_FS_ERR_PATH_TOO_LONG;
    }

    /* find the device in the table */
    for (i = 0; i < NUM_TABLE_ENTRIES; i++)
    {
        if (NEO_OS_VolumeTable[i].FreeFlag == FALSE && NEO_OS_VolumeTable[i].IsMounted == FALSE
            && strcmp(NEO_OS_VolumeTable[i].DeviceName, devname) == 0)
            break;
    }

    /* make sure we found the device */
    if (i >= NUM_TABLE_ENTRIES)
    {
        return NEO_OS_FS_ERROR;
    }

    /* attach the mountpoint */
    strcpy(NEO_OS_VolumeTable[i].MountPoint, mountpoint);
    NEO_OS_VolumeTable[i].IsMounted = TRUE;

    return NEO_OS_FS_SUCCESS;

}/* end NEO_OS_mount */

/*--------------------------------------------------------------------------------------
    Name: NEO_OS_unmount
    
    Purpose: unmounts a drive. and therefore makes all file descriptors pointing into
             the drive obsolete.

    Returns: NEO_OS_FS_ERR_INVALID_POINTER if name is NULL
             NEO_OS_FS_ERR_PATH_TOO_LONG if the absolute path given is too long
             NEO_OS_FS_ERROR if the OS calls failed
             NEO_OS_FS_SUCCESS if success
---------------------------------------------------------------------------------------*/
int32 NEO_OS_unmount (const char *mountpoint)
{
    int i;
    
    if (mountpoint == NULL)
    {
        return NEO_OS_FS_ERR_INVALID_POINTER;
    }

    if (strlen(mountpoint) >= NEO_OS_MAX_PATH_LEN)
    {
        return NEO_OS_FS_ERR_PATH_TOO_LONG;
    }

    for (i = 0; i < NUM_TABLE_ENTRIES; i++)
    {
        if (NEO_OS_VolumeTable[i].FreeFlag == FALSE && NEO_OS_VolumeTable[i].IsMounted == TRUE
			&& strcmp(NEO_OS_VolumeTable[i].MountPoint, mountpoint) == 0)
            break;
    }

    /* make sure we found the device */
    if (i >= NUM_TABLE_ENTRIES)
    {
        return NEO_OS_FS_ERROR;
    }

    /* release the informationm from the table */
    NEO_OS_VolumeTable[i].IsMounted = FALSE;
    strcpy(NEO_OS_VolumeTable[i].MountPoint, "");
    
    return NEO_OS_FS_SUCCESS;
    
}/* end NEO_OS_umount */

/*--------------------------------------------------------------------------------------
    Name: NEO_OS_fsBlocksFree

    Purpose: Returns the number of free blocks in a volume
 
    Returns: NEO_OS_FS_ERR_INVALID_POINTER if name is NULL
             NEO_OS_FS_ERR_PATH_TOO_LONG if the name is too long
             NEO_OS_FS_ERROR if the OS call failed
             The number of blocks free in a volume if success
---------------------------------------------------------------------------------------*/
int32 NEO_OS_fsBlocksFree (const char *name)
{

	int            status;
	int32          NameStatus;
	struct statvfs stat_buf;
	char           tmpFileName[NEO_OS_MAX_LOCAL_PATH_LEN +1];

	if (name == NULL)
	{
		return(NEO_OS_FS_ERR_INVALID_POINTER);
	}

	/*
	** Check the length of the volume name
	*/
	if (strlen(name) >= NEO_OS_MAX_PATH_LEN)
	{
		return(NEO_OS_FS_ERR_PATH_TOO_LONG);
	}

	/*
	** Translate the path
	*/
	NameStatus = NEO_OS_TranslatePath(name, tmpFileName);
	if (NameStatus != NEO_OS_FS_SUCCESS)
	{
		return NEO_OS_FS_ERROR;
	}

	status = statvfs(tmpFileName, &stat_buf);

	if (status == 0)
	{
		return(stat_buf.f_bfree);
	}

	return NEO_OS_FS_ERROR;

}/* end NEO_OS_fsBlocksFree */

/*--------------------------------------------------------------------------------------
    Name: NEO_OS_fsBytesFree

    Purpose: Returns the number of free bytes in a volume
 
    Returns: NEO_OS_FS_ERR_INVALID_POINTER if name is NULL
             NEO_OS_FS_ERR_PATH_TOO_LONG if the name is too long
             NEO_OS_FS_ERROR if the OS call failed
             NEO_OS_FS_SUCCESS if success
---------------------------------------------------------------------------------------*/
int32 NEO_OS_fsBytesFree(const char *name, uint64 *bytes_free)
{
	int             status;
	int32           NameStatus;
	struct statvfs  stat_buf;
	uint64          bytes_free_local;
	char            tmpFileName[NEO_OS_MAX_LOCAL_PATH_LEN +1];

	if (name == NULL || bytes_free == NULL)
	{
		return (NEO_OS_FS_ERR_INVALID_POINTER);
	}

	/*
	** Check the length of the volume name
	*/
	if (strlen(name) >= NEO_OS_MAX_PATH_LEN)
	{
		return (NEO_OS_FS_ERR_PATH_TOO_LONG);
	}

	/*
	** Translate the path
	*/
	NameStatus = NEO_OS_TranslatePath(name, tmpFileName);
	if (NameStatus != NEO_OS_FS_SUCCESS)
	{
		return NEO_OS_FS_ERROR;
	}

	status = statvfs(tmpFileName, &stat_buf);
	if (status == 0)
	{
		bytes_free_local = stat_buf.f_bfree * stat_buf.f_bsize;
		*bytes_free = bytes_free_local;
		return(NEO_OS_FS_SUCCESS);
	}

	return(NEO_OS_FS_ERROR);

}/* end NEO_OS_fsBytesFree */

/*--------------------------------------------------------------------------------------
    Name: NEO_OS_chkfs
    
    Purpose: Checks the drives for inconsisenties and either repairs it or not

    Returns: NEO_OS_FS_ERR_INVALID_POINTER if name is NULL
             NEO_OS_FS_SUCCESS if success
             NEO_OS_FS_ERROR if the OS calls fail

---------------------------------------------------------------------------------------*/
os_fshealth_t NEO_OS_chkfs (const char *name, boolean repair)
{
    return NEO_OS_FS_UNIMPLEMENTED;
}/* end NEO_OS_chkfs */

/*--------------------------------------------------------------------------------------
    Name: NEO_OS_FS_GetPhysDriveName
    
    Purpose: Returns the name of the physical volume associated with the drive,
             when given the OSAL mount point of the drive

    Returns: NEO_OS_FS_ERR_INVALID_POINTER if either  parameter is NULL
             NEO_OS_SUCCESS if success
             NEO_OS_FS_ERROR if the mountpoint could not be found
---------------------------------------------------------------------------------------*/
int32 NEO_OS_FS_GetPhysDriveName(char *PhysDriveName, char *MountPoint)
{
    int32 ReturnCode;
    int   i;
    
    if (MountPoint == NULL || PhysDriveName == NULL)
    {
        return NEO_OS_FS_ERR_INVALID_POINTER;
    }
 
    if (strlen(PhysDriveName) >= NEO_OS_FS_DEV_NAME_LEN || strlen(MountPoint) >= NEO_OS_MAX_PATH_LEN)
    {
        return NEO_OS_FS_ERR_PATH_TOO_LONG;
    }

    /*
    ** look for the CFS Mount Point in the VolumeTable 
    */
    for (i = 0; i < NUM_TABLE_ENTRIES; i++)
    {
        if (NEO_OS_VolumeTable[i].FreeFlag == FALSE &&
            strncmp(NEO_OS_VolumeTable[i].MountPoint, MountPoint, NEO_OS_MAX_PATH_LEN) == 0)
        {
            break;
        }
    }

    /* 
    ** Make sure we found a valid volume table entry 
    */
    if (i >= NUM_TABLE_ENTRIES)
    {
        ReturnCode = NEO_OS_FS_ERROR;
    }
    else
    {
		/*
		** Yes, so copy the physical drive name
		*/
		strncpy(PhysDriveName, NEO_OS_VolumeTable[i].PhysDevName, NEO_OS_FS_PHYS_NAME_LEN);
		ReturnCode = NEO_OS_SUCCESS;
    }

    return ReturnCode;
}/* end NEO_OS_FS_GetPhysDriveName */

/*-------------------------------------------------------------------------------------
 * Name: NEO_OS_TranslatePath
 * 
 * Purpose: Because of the abstraction of the filesystem across OSes, we have to change
 *          the name of the {file, directory, drive} to be what the OS can actually 
 *          accept
---------------------------------------------------------------------------------------*/
int32 NEO_OS_TranslatePath(const char *VirtualPath, char *LocalPath)
{
    char devname [NEO_OS_MAX_PATH_LEN];
    char filename[NEO_OS_MAX_PATH_LEN];
    int  NumChars;
    int  i=0;

    /*
    ** Check to see if the path pointers are NULL
    */
    if (VirtualPath == NULL)
    {
        return NEO_OS_FS_ERR_INVALID_POINTER;
    }

    if (LocalPath == NULL)
    {
        return NEO_OS_FS_ERR_INVALID_POINTER;
    }
   
    /*
    ** Check to see if the path is too long
    */
    if (strlen(VirtualPath) >= NEO_OS_MAX_PATH_LEN)
    {
        return NEO_OS_FS_ERR_PATH_TOO_LONG;
    }

    /*
    ** All valid Virtual paths must start with a '/' character
    */
    if (VirtualPath[0] != '/')
    {
       return NEO_OS_FS_ERR_PATH_INVALID;
    }

    /*
    ** Fill the file and device name to be sure they do not have garbage
    */
    memset((void *)devname,0, NEO_OS_MAX_PATH_LEN);
    memset((void *)filename,0, NEO_OS_MAX_PATH_LEN);
   
    /* 
    ** We want to find the number of chars to where the second "/" is.
    ** Since we know the first one is in spot 0, we start looking at 1, and go until
    ** we find it.
    */    
    NumChars = 1;
    while ((NumChars <= strlen(VirtualPath)) && (VirtualPath[NumChars] != '/'))
    {
        NumChars++;
    }
    
    /* 
    ** Don't let it overflow to cause a segfault when trying to get the highest level
    ** directory 
    */
    if (NumChars > strlen(VirtualPath))
    {
        NumChars = strlen(VirtualPath);
    }
  
    /*
    ** copy over only the part that is the device name 
    */
    snprintf(devname, NEO_OS_MAX_PATH_LEN, "%*s", NumChars, VirtualPath);
    
    /*
    ** Copy everything after the devname as the path/filename
    */
    snprintf(filename, NEO_OS_MAX_PATH_LEN, "%s", VirtualPath + NumChars);
    
#ifdef NEO_OS_DEBUG_PRINTF 
    printf("VirtualPath: %s, Length: %d\n",VirtualPath, (int)strlen(VirtualPath));
    printf("NumChars: %d\n",NumChars);
    printf("devname: %s\n",devname);
    printf("filename: %s\n",filename);
#endif

    /*
    ** look for the dev name we found in the VolumeTable 
    */
    for (i = 0; i < NUM_TABLE_ENTRIES; i++)
    {
        if (NEO_OS_VolumeTable[i].FreeFlag == FALSE && 
            strncmp(NEO_OS_VolumeTable[i].MountPoint, devname,NumChars) == 0)
        {
            break;
        }
    }

    /* 
    ** Make sure we found a valid drive 
    */
    if (i >= NUM_TABLE_ENTRIES)
    {
        return NEO_OS_FS_ERR_PATH_INVALID;
    }
    
    /* 
    ** copy over the physical first part of the drive 
    */
    strncpy(LocalPath, NEO_OS_VolumeTable[i].PhysDevName, NEO_OS_MAX_LOCAL_PATH_LEN);
    NumChars = strlen(LocalPath);

    /*
    ** Add the file name
    */
    strncat(LocalPath, filename, (NEO_OS_MAX_LOCAL_PATH_LEN - NumChars));

#ifdef NEO_OS_DEBUG_PRINTF
    printf("Result of TranslatePath = %s\n",LocalPath);
#endif

    return NEO_OS_FS_SUCCESS;
    
} /* end NEO_OS_TranslatePath */

/*---------------------------------------------------------------------------------------
    Name: NEO_OS_FS_GetErrorName()

    Purpose: a handy debugging tool that will copy the name of the error code to a buffer

    Returns: NEO_OS_FS_ERROR if given error number is unknown
             NEO_OS_FS_SUCCESS if given error is found and copied to the buffer
--------------------------------------------------------------------------------------- */
int32 NEO_OS_FS_GetErrorName(int32 error_num, os_fs_err_name_t *err_name)
{
    /*
    ** Implementation note for developers:
    **
    ** The size of the string literals below (including the terminating null)
    ** must fit into os_fs_err_name_t.  Always check the string length when
    ** adding or modifying strings in this function.  If changing
    ** os_fs_err_name_t then confirm these strings will fit.
    */

    os_fs_err_name_t local_name;
    int32 return_code;

    return_code = NEO_OS_FS_SUCCESS;
    
    switch (error_num)
    {
        case NEO_OS_FS_SUCCESS: 
            strcpy(local_name,"NEO_OS_FS_SUCCESS"); break;
        case NEO_OS_FS_ERROR: 
            strcpy(local_name,"NEO_OS_FS_ERROR"); break;
        case NEO_OS_FS_ERR_INVALID_POINTER: 
            strcpy(local_name,"NEO_OS_FS_ERR_INVALID_POINTER"); break;
        case NEO_OS_FS_ERR_PATH_TOO_LONG: 
            strcpy(local_name,"NEO_OS_FS_ERR_PATH_TOO_LONG"); break;
        case NEO_OS_FS_ERR_NAME_TOO_LONG: 
            strcpy(local_name,"NEO_OS_FS_ERR_NAME_TOO_LONG"); break;
        case NEO_OS_FS_UNIMPLEMENTED: 
            strcpy(local_name,"NEO_OS_FS_UNIMPLEMENTED"); break;
        case NEO_OS_FS_ERR_PATH_INVALID:
            strcpy(local_name,"NEO_OS_FS_ERR_PATH_INVALID"); break;
        case NEO_OS_FS_ERR_DRIVE_NOT_CREATED: 
            strcpy(local_name,"NEO_OS_FS_ERR_DRIVE_NOT_CREATED"); break;
    
        default: strcpy(local_name,"ERROR_UNKNOWN");
                 return_code = NEO_OS_FS_ERROR;
    }

    strcpy((char*) err_name, local_name);
    return return_code;
}

/*--------------------------------------------------------------------------------------
     Name: NEO_OS_GetFsInfo
  
     Purpose: returns information about the file system in an os_fsinfo_t
                  
     Returns: NEO_OS_FS_ERR_INVALID_POINTER if filesys_info is NULL
              NEO_OS_FS_SUCCESS if success

     Note: The information returned is in the structure pointed to by filesys_info         
 ---------------------------------------------------------------------------------------*/
int32 NEO_OS_GetFsInfo(os_fsinfo_t  *filesys_info)
{
	int i;

	/*
	** Check to see if the file pointers are NULL
	*/
	if (filesys_info == NULL)
	{
		return NEO_OS_FS_ERR_INVALID_POINTER;
	}

	filesys_info->MaxFds = NEO_OS_MAX_NUM_OPEN_FILES;
	filesys_info->MaxVolumes = NUM_TABLE_ENTRIES;

	filesys_info->FreeFds = 0;
	for (i = 0; i < NEO_OS_MAX_NUM_OPEN_FILES; i++)
	{
		if (NEO_OS_FDTable[i].IsValid == FALSE)
		{
			filesys_info->FreeFds++;
		}
	}

	filesys_info->FreeVolumes = 0;
	for (i = 0; i < NUM_TABLE_ENTRIES; i++)
	{
		if (NEO_OS_VolumeTable[i].FreeFlag == TRUE)
		{
			filesys_info->FreeVolumes++;
		}
	}

	return(NEO_OS_FS_SUCCESS);
}


#ifdef __cplusplus
}
#endif

} // namespace os
} // namespace neo
} // namespace neodain


