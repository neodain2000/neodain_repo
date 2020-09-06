/*
**
** PC-Linux define
**
**
**
*/

#ifndef __NEO_OS_API__
#define __NEO_OS_API__

#include "neo_os_common_types.h"

namespace neodain {
namespace neo {
namespace os {

#ifdef __cplusplus
extern "C" {
#endif

#define NEO_OS_SUCCESS                     (0)
#define NEO_OS_ERROR                       (-1)
#define NEO_OS_INVALID_POINTER             (-2)
#define NEO_OS_ERROR_ADDRESS_MISALIGNED    (-3)
#define NEO_OS_ERROR_TIMEOUT               (-4)
#define NEO_OS_INVALID_INT_NUM             (-5)
#define NEO_OS_SEM_FAILURE                 (-6)
#define NEO_OS_SEM_TIMEOUT                 (-7)
#define NEO_OS_QUEUE_EMPTY                 (-8)
#define NEO_OS_QUEUE_FULL                  (-9)
#define NEO_OS_QUEUE_TIMEOUT               (-10)
#define NEO_OS_QUEUE_INVALID_SIZE          (-11)
#define NEO_OS_QUEUE_ID_ERROR              (-12)
#define NEO_OS_ERR_NAME_TOO_LONG           (-13)
#define NEO_OS_ERR_NO_FREE_IDS             (-14)
#define NEO_OS_ERR_NAME_TAKEN              (-15)
#define NEO_OS_ERR_INVALID_ID              (-16)
#define NEO_OS_ERR_NAME_NOT_FOUND          (-17)
#define NEO_OS_ERR_SEM_NOT_FULL            (-18)
#define NEO_OS_ERR_INVALID_PRIORITY        (-19)
#define NEO_OS_INVALID_SEM_VALUE           (-20)
#define NEO_OS_ERR_FILE                    (-27)
#define NEO_OS_ERR_NOT_IMPLEMENTED         (-28)
#define NEO_OS_TIMER_ERR_INVALID_ARGS      (-29)
#define NEO_OS_TIMER_ERR_TIMER_ID          (-30)
#define NEO_OS_TIMER_ERR_UNAVAILABLE       (-31)
#define NEO_OS_TIMER_ERR_INTERNAL          (-32)

/*
** Defines for Queue Timeout parameters
*/
#define NEO_OS_PEND   (-1)
#define NEO_OS_CHECK  (0)

#include "neo_os_version.h"

/*
** Include the configuration file
*/
#include "neo_os_config.h"

/*
** Include the OS API modules
*/
#include "neo_osapi_core.h"
#include "neo_osapi_filesys.h"
#include "neo_osapi_net.h"
#include "neo_osapi_loader.h"
#include "neo_osapi_timer.h"


#ifdef __cplusplus
}
#endif

} // namespace os
} // namespace neo
} // namespace neodain

#endif


