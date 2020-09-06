/*
** File: neo_osapi_timer.h
**
** Purpose: Contains functions prototype definitions and variable declarations
**          for the OS Abstraction Layer, Timer API
**  
*/

#ifndef __NEO_OSAPI_TIMER_H__
#define __NEO_OSAPI_TIMER_H__

namespace neodain {
namespace neo {
namespace os {

/*
** Typedefs
*/
typedef void (*NEO_OS_TimerCallback_t)(uint32 timer_id);
typedef int32 (*NEO_OS_TimerSync_t)(uint32 timer_id);

typedef struct {
   char                name[NEO_OS_MAX_API_NAME];
   uint32              creator;
   uint32              start_time;
   uint32              interval_time;
   uint32              accuracy;
} NEO_OS_timer_prop_t;

typedef struct {
    char                name[NEO_OS_MAX_API_NAME];
    uint32              creator;
    uint32              interval_time;
    uint32              accuracy;
} NEO_OS_timebase_prop_t;

/*
** Timer API
*/
int32  NEO_OS_TimerAPIInit          (void);

int32 NEO_OS_TimeBaseCreate         (uint32 *timer_id, const char *timebase_name, NEO_OS_TimerSync_t external_sync);
int32 NEO_OS_TimeBaseSet            (uint32 timer_id, uint32 start_time, uint32 interval_time);
int32 NEO_OS_TimeBaseDelete         (uint32 timer_id);
int32 NEO_OS_TimeBaseGetIdByName    (uint32 *timer_id, const char *timebase_name);

int32 NEO_OS_TimerCreate            (uint32 *timer_id, const char *timer_name, uint32 *clock_accuracy, NEO_OS_TimerCallback_t callback_ptr);
int32 NEO_OS_TimerAdd               (uint32 *timer_id, const char *timer_name, uint32 timebase_id, NEO_OS_ArgCallback_t  callback_ptr, void *callback_arg);
int32 NEO_OS_TimerSet               (uint32 timer_id, uint32 start_time, uint32 interval_time);
int32 NEO_OS_TimerDelete            (uint32 timer_id);

int32 NEO_OS_TimerGetIdByName       (uint32 *timer_id, const char *timer_name);
int32 NEO_OS_TimerGetInfo           (uint32  timer_id, NEO_OS_timer_prop_t *timer_prop);

} // namespace os
} // namespace neo
} // namespace neodain

#endif // __NEO_OSAPI_TIMER_H__

