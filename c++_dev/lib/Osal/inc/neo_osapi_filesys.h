/*
** File: neo_osapi_filesys.h
**
** Purpose: Contains functions prototype definitions and variables declarations
**          for the OS Abstraction Layer, File System module
**    
*/

#ifndef __NEO_OSAPI_FILESYS_H__
#define __NEO_OSAPI_FILESYS_H__
    
namespace neodain {
namespace neo {
namespace os {

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

#define NEO_OS_READ_ONLY        0
#define NEO_OS_WRITE_ONLY       1
#define NEO_OS_READ_WRITE       2

#define NEO_OS_SEEK_SET         0
#define NEO_OS_SEEK_CUR         1
#define NEO_OS_SEEK_END         2

#define NEO_OS_CHK_ONLY         0
#define NEO_OS_REPAIR           1

#define FS_BASED            0
#define RAM_DISK            1
#define EEPROM_DISK         2
#define ATA_DISK            3


/*
** Number of entries in the internal volume table
*/
#define NUM_TABLE_ENTRIES 14

/*
** Length of a Device and Volume name 
*/
#define NEO_OS_FS_DEV_NAME_LEN  32
#define NEO_OS_FS_PHYS_NAME_LEN 64
#define NEO_OS_FS_VOL_NAME_LEN  32


/*
** Defines for File System Calls
*/

/*
** NOTE - these values used to overlap with the other OSAPI error codes.
** They now start at -100 to avoid this overlap.
*/
#define NEO_OS_FS_ERR_PATH_TOO_LONG        (-103)
#define NEO_OS_FS_ERR_NAME_TOO_LONG        (-104)
#define NEO_OS_FS_ERR_DRIVE_NOT_CREATED    (-106)
#define NEO_OS_FS_ERR_DEVICE_NOT_FREE      (-107)
#define NEO_OS_FS_ERR_PATH_INVALID         (-108)


/* 
** Map some codes used by the file API back to the generic counterparts 
** where there is overlap between them. Do not duplicate error codes.
*/
#define NEO_OS_FS_SUCCESS                  NEO_OS_SUCCESS
#define NEO_OS_FS_ERROR                    NEO_OS_ERROR
#define NEO_OS_FS_ERR_INVALID_POINTER      NEO_OS_INVALID_POINTER
#define NEO_OS_FS_ERR_NO_FREE_FDS          NEO_OS_ERR_NO_FREE_IDS
#define NEO_OS_FS_ERR_INVALID_FD           NEO_OS_ERR_INVALID_ID
#define NEO_OS_FS_UNIMPLEMENTED            NEO_OS_ERR_NOT_IMPLEMENTED


/* This typedef is for the NEO_OS_FS_GetErrorName function, to ensure
** everyone is making an array of the same length
**
** Implementation note for developers:
**
** os_fs_err_name_t is now equivalent to the OSAL "os_err_name_t" typedef, 
** to preserve source code compatibility with anything using the NEO_OS_FS_GetErrorName api
**
** The sizes of strings in OSAL functions are built with os_fs_err_name_t's
** limits in mind. Always check the uses of os_fs_err_name_t when changing
** os_err_name_t.
*/
typedef os_err_name_t os_fs_err_name_t;


/*
** Internal structure of the OS volume table for 
** mounted file systems and path translation
*/
typedef struct {
    char   DeviceName [NEO_OS_FS_DEV_NAME_LEN];
    char   PhysDevName [NEO_OS_FS_PHYS_NAME_LEN];
    uint32 VolumeType;
    uint8  VolatileFlag;
    uint8  FreeFlag;
    uint8  IsMounted;
    char   VolumeName [NEO_OS_FS_VOL_NAME_LEN];
    char   MountPoint [NEO_OS_MAX_PATH_LEN];
    uint32 BlockSize;

} NEO_OS_VolumeInfo_t;

typedef struct {
    int32   OSfd;                   /* The underlying OS's file descriptor */
    char    Path[NEO_OS_MAX_PATH_LEN];  /* The path of the file opened */
    uint32  User;                   /* The task id of the task who opened the file*/
    uint8   IsValid;                /* Whether or not this entry is valid */
} NEO_OS_FDTableEntry;

typedef struct {
   uint32   MaxFds;                /* Total number of file descriptors */
   uint32   FreeFds;               /* Total number that are free */
   uint32   MaxVolumes;            /* Maximum number of volumes */
   uint32   FreeVolumes;           /* Total number of volumes free */
} os_fsinfo_t; 

/* modified to posix calls, since all of the 
 * applicable OSes use the posix calls */

typedef struct stat         os_fstat_t;
typedef DIR*                os_dirp_t;
typedef struct dirent       os_dirent_t;
/* still don't know what this should be*/
typedef unsigned long int   os_fshealth_t; 

/*
 * Exported Functions
*/


/******************************************************************************
** Standard File system API
******************************************************************************/
/*
 * Initializes the File System functions 
*/

int32           NEO_OS_FS_Init(void);

/*
 * Creates a file specified by path
*/
int32           NEO_OS_creat  (const char *path, int32  access);

/*
 * Opend a file for reading/writing. Returns file descriptor
*/
int32           NEO_OS_open   (const char *path,  int32 access,  uint32 mode);

/*
 * Closes an open file.
*/
int32           NEO_OS_close  (int32  filedes);

/*
 * Reads nbytes bytes from file into buffer
*/
int32           NEO_OS_read   (int32  filedes, void *buffer, uint32 nbytes);

/*
 * Write nybytes bytes of buffer into the file
*/
int32           NEO_OS_write  (int32  filedes, void *buffer, uint32 nbytes);

/*
 * Changes the permissions of a file
*/
int32           NEO_OS_chmod  (const char *path, uint32 access);

/*
 * Returns file status information in filestats
*/
int32           NEO_OS_stat   (const char *path, os_fstat_t  *filestats);

/*
 * Seeks to the specified position of an open file 
*/
int32           NEO_OS_lseek  (int32  filedes, int32 offset, uint32 whence);

/*
 * Removes a file from the file system
*/
int32           NEO_OS_remove (const char *path);

/*
 * Renames a file in the file system
*/
int32           NEO_OS_rename (const char *old_filename, const char *new_filename);

/* 
 * copies a single file from src to dest
*/
int32 NEO_OS_cp (const char *src, const char *dest);

/* 
 * moves a single file from src to dest
*/
int32 NEO_OS_mv (const char *src, const char *dest);

/*
 * Copies the info of an open file to the structure
*/
int32 NEO_OS_FDGetInfo (int32 filedes, NEO_OS_FDTableEntry *fd_prop);

/*
** Check to see if a file is open
*/
int32 NEO_OS_FileOpenCheck(char *Filename);

/*
** Close all open files
*/
int32 NEO_OS_CloseAllFiles(void);

/*
** Close a file by filename
*/
int32 NEO_OS_CloseFileByName(char *Filename);


/******************************************************************************
** Directory API 
******************************************************************************/

/*
 * Makes a new directory
*/
int32           NEO_OS_mkdir   (const char *path, uint32 access);

/*
 * Opens a directory for searching
*/
os_dirp_t       NEO_OS_opendir (const char *path);

/*
 * Closes an open directory
*/
int32           NEO_OS_closedir(os_dirp_t directory);

/*
 * Rewinds an open directory
*/
void           NEO_OS_rewinddir(os_dirp_t directory);

/*
 * Reads the next object in the directory
*/
os_dirent_t *   NEO_OS_readdir (os_dirp_t directory);

/*
 * Removes an empty directory from the file system.
*/
int32           NEO_OS_rmdir   (const char *path);

/******************************************************************************
** System Level API 
******************************************************************************/
/*
 * Makes a file system
*/
int32           NEO_OS_mkfs        (char *address,char *devname, char *volname,
                                uint32 blocksize, uint32 numblocks);
/*
 * Mounts a file system
*/
int32           NEO_OS_mount       (const char *devname, char *mountpoint);

/*
 * Initializes an existing file system
*/
int32           NEO_OS_initfs      (char *address,char *devname, char *volname,
                                uint32 blocksize, uint32 numblocks);

/*
 * removes a file system 
*/
int32           NEO_OS_rmfs        (char *devname);

/*
 * Unmounts a mounted file system
*/
int32           NEO_OS_unmount     (const char *mountpoint);

/*
 * Returns the number of free blocks in a file system
*/
int32           OS_fsBlocksFree (const char *name);

/*
** Returns the number of free bytes in a file system 
** Note the 64 bit data type to support filesystems that
** are greater than 4 Gigabytes
*/
int32 NEO_OS_fsBytesFree (const char *name, uint64 *bytes_free);

/*
 * Checks the health of a file system and repairs it if neccesary
*/
os_fshealth_t   NEO_OS_chkfs       (const char *name, boolean repair);

/*
 * Returns in the parameter the physical drive underneith the mount point 
*/
int32       NEO_OS_FS_GetPhysDriveName  (char * PhysDriveName, char * MountPoint);

/*
** Translates a OSAL Virtual file system path to a host Local path
*/
int32       NEO_OS_TranslatePath ( const char *VirtualPath, char *LocalPath);

/*             
**  Returns information about the file system in an os_fsinfo_t
*/
int32       NEO_OS_GetFsInfo(os_fsinfo_t  *filesys_info);

/******************************************************************************
** Shell API
******************************************************************************/

/* executes the shell command passed into is and writes the output of that 
 * command to the file specified by the given OSAPI file descriptor */
int32 NEO_OS_ShellOutputToFile(char* Cmd, int32 NEO_OS_fd);


} // namespace os
} // namespace neo
} // namespace neodain

#endif // __NEO_OSAPI_FILESYS_H__
