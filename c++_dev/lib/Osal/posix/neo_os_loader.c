/*
** File   : neo_os_loader.c
**
** Purpose: This file contains the module loader and symbol lookup functions for the OSAL.
*/

namesapce neodain {
namesapce neo {
namespace os {

#ifdef __cplusplus
extern "C" {
#endif


/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"

#include "common_types.h"
#include "osapi.h"

#include <stdio.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */
#include <pthread.h>
#include <signal.h>

#include <dlfcn.h>

/*
** If NEO_OS_INCLUDE_MODULE_LOADER is not defined, skip the whole module
*/
#ifdef NEO_OS_INCLUDE_MODULE_LOADER

/****************************************************************************************
                                     DEFINES
****************************************************************************************/

/****************************************************************************************
                                     FUNCTION PROTOTYPES 
****************************************************************************************/
extern int    NEO_OS_InterruptSafeLock(pthread_mutex_t *lock, sigset_t *set, sigset_t *previous);
extern void   NEO_OS_InterruptSafeUnlock(pthread_mutex_t *lock, sigset_t *previous);

/****************************************************************************************
                                   GLOBAL DATA
****************************************************************************************/

/*
 * The "NEO_OS_module_internal_record_t" structure is used internally
 * to the osloader module for keeping the state.  It is OS-specific
 * and should not be directly used by external entities.
 */
typedef struct
{
   int                 free;
   cpuaddr             entry_point;
   void *              host_module_id;
   char                filename[NEO_OS_MAX_PATH_LEN];
   char                name[NEO_OS_MAX_API_NAME];

} NEO_OS_module_internal_record_t;


/*
** Need to define the OS Module table here. 
** osconfig.h will have the maximum number of loadable modules defined.
*/
NEO_OS_module_internal_record_t NEO_OS_module_table[NEO_OS_MAX_MODULES];

/*
** The Mutex for protecting the above table
*/
pthread_mutex_t    NEO_OS_module_table_mut;

/****************************************************************************************
                                INITIALIZATION FUNCTION
****************************************************************************************/
int32  NEO_OS_ModuleTableInit ( void )
{
   int i;
   int return_code;
   
   /* 
   ** Initialize Module Table 
   */
   for(i = 0; i < NEO_OS_MAX_MODULES; i++)
   {
      NEO_OS_module_table[i].free        = TRUE;
      NEO_OS_module_table[i].entry_point = 0; 
      NEO_OS_module_table[i].host_module_id = 0;
      strcpy(NEO_OS_module_table[i].name,"");
      strcpy(NEO_OS_module_table[i].filename,"");
   }

   /*
   ** Create the Module Table mutex
   */
   return_code = pthread_mutex_init((pthread_mutex_t *) & NEO_OS_module_table_mut,NULL); 
   if ( return_code == 0)
   {
      return(NEO_OS_SUCCESS);
   }
   else 
   {
      return(NEO_OS_ERROR);
   }
}

/****************************************************************************************
                                    Symbol table API
****************************************************************************************/
/*--------------------------------------------------------------------------------------
    Name: NEO_OS_SymbolLookup
    
    Purpose: Find the Address of a Symbol 

    Parameters: 

    Returns: NEO_OS_ERROR if the symbol could not be found
             NEO_OS_SUCCESS if the symbol is found 
             NEO_OS_INVALID_POINTER if the pointer passed in is invalid
             
             The address of the symbol will be stored in the pointer that is passed in.
---------------------------------------------------------------------------------------*/
int32 NEO_OS_SymbolLookup( cpuaddr *SymbolAddress, const char *SymbolName )
{
   const char   *dlError;           /*  Pointer to error string   */
   void         *Function;

   /*
    * call dlerror() to clear any prior error that might have occured.
    */
   dlerror();

   /*
   ** Check parameters
   */
   if (( SymbolAddress == NULL ) || ( SymbolName == NULL ))
   {
      return(NEO_OS_INVALID_POINTER);
   }

   /*
   ** Lookup the entry point
   */
   Function = dlsym((void *)0, SymbolName);
   dlError = dlerror();
   if( dlError )
   {
      return(NEO_OS_ERROR);
   }

   *SymbolAddress = (cpuaddr)Function;
   
   return(NEO_OS_SUCCESS);
    
}/* end NEO_OS_SymbolLookup */

/*--------------------------------------------------------------------------------------
    Name: NEO_OS_SymbolTableDump
    
    Purpose: Dumps the system symbol table to a file

    Parameters: 

    Returns: NEO_OS_ERROR if the symbol table could not be read or dumped
             NEO_OS_INVALID_FILE  if the file could not be opened or written
             NEO_OS_SUCCESS if the symbol is found 
---------------------------------------------------------------------------------------*/
int32 NEO_OS_SymbolTableDump ( const char *filename, uint32 SizeLimit )
{

   return(NEO_OS_ERR_NOT_IMPLEMENTED);
   
}/* end NEO_OS_SymbolTableDump */


/****************************************************************************************
                                    Module Loader API
****************************************************************************************/

/*--------------------------------------------------------------------------------------
    Name: NEO_OS_ModuleLoad
    
    Purpose: Loads an object file into the running operating system

    Parameters: 

    Returns: NEO_OS_ERROR if the module cannot be loaded
             NEO_OS_INVALID_POINTER if one of the parameters is NULL
             NEO_OS_ERR_NO_FREE_IDS if the module table is full
             NEO_OS_ERR_NAME_TAKEN if the name is in use
             NEO_OS_SUCCESS if the module is loaded successfuly
---------------------------------------------------------------------------------------*/
int32 NEO_OS_ModuleLoad ( uint32 *module_id, const char *module_name, const char *filename )
{
   int         i;
   uint32      possible_moduleid;
   char        translated_path[NEO_OS_MAX_LOCAL_PATH_LEN];
   int32       return_code;
   void       *function_lib;     /*  Handle to shared lib file */
   const char *dl_error;    /*  Pointer to error string   */
   sigset_t    previous;
   sigset_t    mask;

   /*
   ** Check parameters
   */
   if (( filename == NULL ) || (module_id == NULL ) || (module_name == NULL))
   {
      return(NEO_OS_INVALID_POINTER);
   }
 
   NEO_OS_InterruptSafeLock(&NEO_OS_module_table_mut, &mask, &previous); 

   /*
   ** Find a free module id
   */
   for( possible_moduleid = 0; possible_moduleid < NEO_OS_MAX_MODULES; possible_moduleid++)
   {
       if (NEO_OS_module_table[possible_moduleid].free == TRUE)
       {
           break;
       }
   }

   /* 
   ** Check to see if the id is out of bounds 
   */
   if( possible_moduleid >= NEO_OS_MAX_MODULES || NEO_OS_module_table[possible_moduleid].free != TRUE)
   {
       NEO_OS_InterruptSafeUnlock(&NEO_OS_module_table_mut, &previous); 
       return NEO_OS_ERR_NO_FREE_IDS;
   }

   /* 
   ** Check to see if the module file is already loaded 
   */
   for (i = 0; i < NEO_OS_MAX_MODULES; i++)
   {
       if ((NEO_OS_module_table[i].free == FALSE) &&
          ( strcmp((char*) module_name, NEO_OS_module_table[i].name) == 0)) 
       {       
           NEO_OS_InterruptSafeUnlock(&NEO_OS_module_table_mut, &previous); 
           return NEO_OS_ERR_NAME_TAKEN;
       }
   }

   /* 
   ** Set the possible task Id to not free so that
   ** no other task can try to use it 
   */
   NEO_OS_module_table[possible_moduleid].free = FALSE ;
   NEO_OS_InterruptSafeUnlock(&NEO_OS_module_table_mut, &previous); 
 
   /*
   ** Translate the filename to the Host System
   */     
   return_code = NEO_OS_TranslatePath((const char *)filename, (char *)translated_path); 
   if ( return_code != NEO_OS_SUCCESS )
   {
      NEO_OS_module_table[possible_moduleid].free = TRUE;
      return(return_code);
   }

   /*
   ** File is ready to load
   */
   
   /* 
   ** Open the loadble bundle .. just opening it loads it into the system.
   */
   function_lib = dlopen(translated_path, RTLD_LAZY | RTLD_GLOBAL);
   dl_error = dlerror();
   if( dl_error )
   {
      NEO_OS_module_table[possible_moduleid].free = TRUE;
      return(NEO_OS_ERROR);
   }

   /*
   ** fill out the NEO_OS_module_table entry for this new module
   */
   NEO_OS_module_table[possible_moduleid].entry_point = 0; /* Only for certain targets */
   NEO_OS_module_table[possible_moduleid].host_module_id = function_lib;
   strncpy(NEO_OS_module_table[possible_moduleid].filename , filename, NEO_OS_MAX_PATH_LEN);
   strncpy(NEO_OS_module_table[possible_moduleid].name , module_name, NEO_OS_MAX_API_NAME);
 
   /*
   ** Return the OSAPI Module ID
   */
   *module_id = possible_moduleid;
     
   return(NEO_OS_SUCCESS);
   
}/* end NEO_OS_ModuleLoad */

/*--------------------------------------------------------------------------------------
    Name: NEO_OS_ModuleUnload
    
    Purpose: Unloads the module file from the running operating system

    Parameters: 

    Returns: NEO_OS_ERROR if the module is invalid or cannot be unloaded
             NEO_OS_SUCCESS if the module was unloaded successfuly 
---------------------------------------------------------------------------------------*/
int32 NEO_OS_ModuleUnload ( uint32 module_id )
{

   const char *dlError;   

   /*
   ** Check the module_id
   */
   if ( module_id >= NEO_OS_MAX_MODULES || NEO_OS_module_table[module_id].free == TRUE )
   {
      return(NEO_OS_ERR_INVALID_ID);
   }
   
   /*
   ** Attempt to close/unload the module
   */ 
   dlclose((void *)NEO_OS_module_table[module_id].host_module_id);
   dlError = dlerror();
   if( dlError )
   {
      NEO_OS_module_table[module_id].free = TRUE;  
      return(NEO_OS_ERROR);
   }
   NEO_OS_module_table[module_id].free = TRUE;
 
   return(NEO_OS_SUCCESS);
   
}/* end NEO_OS_ModuleUnload */

/*--------------------------------------------------------------------------------------
    Name: NEO_OS_ModuleInfo
    
    Purpose: Returns information about the loadable module

    Parameters: 

    Returns: NEO_OS_ERR_INVALID_ID if the module id invalid
             NEO_OS_INVALID_POINTER if the pointer to the ModuleInfo structure is invalid
             NEO_OS_SUCCESS if the module info was filled out successfuly 
---------------------------------------------------------------------------------------*/
int32 NEO_OS_ModuleInfo ( uint32 module_id, NEO_OS_module_prop_t *module_info )
{

   /*
   ** Check the parameter
   */
   if ( module_info == 0 )
   {
      return(NEO_OS_INVALID_POINTER);
   }

   /*
   ** Check the module_id
   */
   if ( module_id >= NEO_OS_MAX_MODULES || NEO_OS_module_table[module_id].free == TRUE )
   {
      return(NEO_OS_ERR_INVALID_ID);
   }

   /*
   ** Fill out the module info
   */
   module_info->entry_point = NEO_OS_module_table[module_id].entry_point;
   module_info->host_module_id = (uint32)NEO_OS_module_table[module_id].host_module_id;
   strncpy(module_info->filename, NEO_OS_module_table[module_id].filename , NEO_OS_MAX_PATH_LEN);
   strncpy(module_info->name, NEO_OS_module_table[module_id].name, NEO_OS_MAX_API_NAME);

   /*
   ** Address info is currently not valid on Linux 
   */
   module_info->addr.valid = FALSE;
   module_info->addr.code_address = 0;
   module_info->addr.code_size = 0;
   module_info->addr.data_address = 0;
   module_info->addr.data_size = 0;
   module_info->addr.bss_address = 0;
   module_info->addr.bss_size = 0;
   module_info->addr.flags = 0;

   return(NEO_OS_SUCCESS);
 
}/* end NEO_OS_ModuleInfo */

#endif


#ifdef __cplusplus
}
#endif

} // namesapce os
} // namespace neo
} // namespace neodain


