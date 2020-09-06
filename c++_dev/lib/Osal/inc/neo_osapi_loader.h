/*
** File: neo_osapi_loader.h
**
** Purpose: Contains functions prototype definitions and variables declarations
**          for the OS Abstraction Layer, Object file loader API
**  
*/

#ifndef __NEO_OSAPI_LOADER_H__
#define __NEO_OSAPI_LOADER_H__

namespace neodain {
namespace neo {
namespace os {

/*
** Defines
*/


/*
** Typedefs
*/

typedef struct {
   uint32 valid;
   uint32 flags;  
   cpuaddr code_address;
   cpuaddr code_size;
   cpuaddr data_address;
   cpuaddr data_size;
   cpuaddr bss_address;
   cpuaddr bss_size;
} NEO_OS_module_address_t;

typedef struct {
   cpuaddr             entry_point;
   uint32              host_module_id;
   char                filename[NEO_OS_MAX_PATH_LEN];
   char                name[NEO_OS_MAX_API_NAME];
   NEO_OS_module_address_t addr;
} NEO_OS_module_prop_t;

/*
 * Define the former "NEO_OS_module_record_t" type as equivalent
 * to the NEO_OS_module_prop_t.  This is what the NEO_OS_ModuleInfo()
 * will output.  It used to be the same as the internal record
 * just without all the fields filled in.  This has been changed
 * to make it a separate structure, which will allow the internal
 * implementation to change without further changing the API.
 *
 * Ideally NEO_OS_module_record_t type should be removed to avoid confusion,
 * but this would break existing code that calls NEO_OS_ModuleInfo().
 */
#ifndef OSAL_OMIT_DEPRECATED
typedef NEO_OS_module_prop_t NEO_OS_module_record_t;
#endif

/*
** Loader API
*/
int32 NEO_OS_ModuleTableInit ( void );

int32 NEO_OS_SymbolLookup (cpuaddr *symbol_address, const char *symbol_name );

int32 NEO_OS_SymbolTableDump ( const char *filename, uint32 size_limit );

int32 NEO_OS_ModuleLoad ( uint32 *module_id, const char *module_name, const char *filename );

int32 NEO_OS_ModuleUnload ( uint32 module_id );

int32 NEO_OS_ModuleInfo ( uint32 module_id, NEO_OS_module_prop_t *module_info );

} // namespace os
} // namespace neo
} // namespace neodain

#endif // __NEO_OSAPI_LOADER_H__
