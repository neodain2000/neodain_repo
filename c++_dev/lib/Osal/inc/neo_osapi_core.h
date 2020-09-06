/*
**
**
** Purpose: Contains functions prototype definitions and variables declarations
**          for the OS Abstraction Layer, Core OS module
**
*/

#ifndef __NEO_OSAPI_CORE_H__
#define __NEO_OSAPI_CORE_H__

namespace neodain {
namespace neo {
namespace os {

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h> // for va_list

/*difines constants for NEO_OS_BinSemCreate for state of semaphore  */
#define NEO_OS_SEM_FULL     1
#define NEO_OS_SEM_EMPTY    0

/* #define for enabling floating point operations on a task*/
#define NEO_OS_FP_ENABLED 1

/* tables for the properties of objects */

/*tasks */
typedef struct {
    char name [NEO_OS_MAX_API_NAME];
    uint32 creator;
    uint32 stack_size;
    uint32 priority;
    uint32 OStask_id;
} NEO_OS_task_prop_t;

/* queues */
typedef struct {
    char name [NEO_OS_MAX_API_NAME];
    uint32 creator;
} NEO_OS_queue_prop_t;

/* Binary Semaphores */
typedef struct {
    char name [NEO_OS_MAX_API_NAME];
    uint32 creator;
    int32  value;
} NEO_OS_bin_sem_prop_t;

/* Counting Semaphores */
typedef struct {
    char name [NEO_OS_MAX_API_NAME];
    uint32 creator;
    int32 value;
} NEO_OS_count_sem_prop_t;

/* Mutexes */
typedef struct {
    char name [NEO_OS_MAX_API_NAME];
    uint32 creator;
} NEO_OS_mut_sem_prop_t;


/* struct for NEO_OS_GetLocalTime() */
typedef struct {
    uint32 seconds;
    uint32 microsecs;
} NEO_OS_time_t;

/* heap info */
typedef struct {
    uint32 free_bytes;
    uint32 free_blocks;
    uint32 largest_free_block;
} NEO_OS_heap_prop_t;


/* This typedef is for the NEO_OS_GetErrorName function, to ensure
 * everyone is making an array of the same length.
 *
 * Implementation note for developers:
 *
 * The sizes of strings in OSAL functions are built with this
 * NEO_OS_ERROR_NAME_LENGTH limit in mind.  Always check the uses of os_err_name_t
 * when changing this value.
 */

#define NEO_OS_ERROR_NAME_LENGTH    35
typedef char neo_os_err_name_t[NEO_OS_ERROR_NAME_LENGTH];

/*
** These typedefs are for the task entry point
*/
typedef void osal_task;
typedef osal_task ((*osal_task_entry)(void));

/*
** Typedef for general purpose OSAL callback functions
** This may be used by multiple APIS
*/
typedef void (*NEO_OS_ArgCallback_t)(uint32 object_id, void *arg);


/*
**  External Declarations
*/

/*
** Prototype for application startup function.
** This is implemented by the user application
*/
void NEO_OS_Application_Startup(void);


/*
** Exported Functions
*/

/*
** Initialization of API
*/
int32 NEO_OS_API_Init (void);

/*
** OS-specific background thread implementation - waits forever for events to occur.
**
** This should be called from the BSP main routine / initial thread after all other
** board / application initialization has taken place and all other tasks are running.
*/
void NEO_OS_IdleLoop (void);

/*
** NEO_OS_DeleteAllObjects() provides a means to clean up all resources allocated by this
** instance of OSAL.  It would typically be used during an orderly shutdown but may also
** be helpful for testing purposes.

*/
void NEO_OS_DeleteAllObjects    (void);

/*
** NEO_OS_ApplicationShutdown() provides a means for a user-created thread to request the orderly
** shutdown of the whole system, such as part of a user-commanded reset command.
** This is preferred over e.g. ApplicationExit() which exits immediately and does not
** provide for any means to clean up first.
*/
void NEO_OS_ApplicationShutdown (uint8 flag);


/*
** Task API
*/

int32 NEO_OS_TaskCreate             (uint32 *task_id, const char *task_name,
                                    osal_task_entry function_pointer,
                                    uint32 *stack_pointer,
                                    uint32 stack_size,
                                    uint32 priority, uint32 flags);

int32 NEO_OS_TaskDelete            (uint32 task_id);
void NEO_OS_TaskExit               (void);
int32 NEO_OS_TaskInstallDeleteHandler(osal_task_entry function_pointer);
int32 NEO_OS_TaskDelay             (uint32 millisecond);
int32 NEO_OS_TaskSetPriority       (uint32 task_id, uint32 new_priority);
int32 NEO_OS_TaskRegister          (void);
uint32 NEO_OS_TaskGetId            (void);
int32 NEO_OS_TaskGetIdByName       (uint32 *task_id, const char *task_name);
int32 NEO_OS_TaskGetInfo           (uint32 task_id, NEO_OS_task_prop_t *task_prop);

/*
** Message Queue API
*/

/*
** Queue Create now has the Queue ID returned to the caller.
*/
int32 NEO_OS_QueueCreate           (uint32 *queue_id, const char *queue_name, uint32 queue_depth, uint32 data_size, uint32 flags);
int32 NEO_OS_QueueDelete           (uint32 queue_id);
int32 NEO_OS_QueueGet              (uint32 queue_id, void *data, uint32 size, uint32 *size_copied, int32 timeout);
int32 NEO_OS_QueuePut              (uint32 queue_id, const void *data, uint32 size, uint32 flags);
int32 NEO_OS_QueueGetIdByName      (uint32 *queue_id, const char *queue_name);
int32 NEO_OS_QueueGetInfo          (uint32 queue_id, NEO_OS_queue_prop_t *queue_prop);

/*
** Semaphore API
*/

int32 NEO_OS_BinSemCreate          (uint32 *sem_id, const char *sem_name, uint32 sem_initial_value, uint32 options);
int32 NEO_OS_BinSemFlush           (uint32 sem_id);
int32 NEO_OS_BinSemGive            (uint32 sem_id);
int32 NEO_OS_BinSemTake            (uint32 sem_id);
int32 NEO_OS_BinSemTimedWait       (uint32 sem_id, uint32 msecs);
int32 NEO_OS_BinSemDelete          (uint32 sem_id);
int32 NEO_OS_BinSemGetIdByName     (uint32 *sem_id, const char *sem_name);
int32 NEO_OS_BinSemGetInfo         (uint32 sem_id, NEO_OS_bin_sem_prop_t *bin_prop);

int32 NEO_OS_CountSemCreate          (uint32 *sem_id, const char *sem_name, uint32 sem_initial_value, uint32 options);
int32 NEO_OS_CountSemGive            (uint32 sem_id);
int32 NEO_OS_CountSemTake            (uint32 sem_id);
int32 NEO_OS_CountSemTimedWait       (uint32 sem_id, uint32 msecs);
int32 NEO_OS_CountSemDelete          (uint32 sem_id);
int32 NEO_OS_CountSemGetIdByName     (uint32 *sem_id, const char *sem_name);
int32 NEO_OS_CountSemGetInfo         (uint32 sem_id, NEO_OS_count_sem_prop_t *count_prop);

/*
** Mutex API
*/

int32 NEO_OS_MutSemCreate           (uint32 *sem_id, const char *sem_name, uint32 options);
int32 NEO_OS_MutSemGive             (uint32 sem_id);
int32 NEO_OS_MutSemTake             (uint32 sem_id);
int32 NEO_OS_MutSemDelete           (uint32 sem_id);
int32 NEO_OS_MutSemGetIdByName      (uint32 *sem_id, const char *sem_name);
int32 NEO_OS_MutSemGetInfo          (uint32 sem_id, NEO_OS_mut_sem_prop_t *mut_prop);

/*
** OS Time/Tick related API
*/

int32 NEO_OS_Milli2Ticks            (uint32 milli_seconds);
int32 NEO_OS_Tick2Micros            (void);
int32 NEO_OS_GetLocalTime           (NEO_OS_time_t *time_struct);
int32 NEO_OS_SetLocalTime           (NEO_OS_time_t *time_struct);

/*
** Exception API
*/

int32 NEO_OS_ExcAttachHandler      (uint32 ExceptionNumber,
                                    void (*ExceptionHandler)(uint32, const void *,uint32),
                                    int32 parameter);
int32 NEO_OS_ExcEnable             (int32 ExceptionNumber);
int32 NEO_OS_ExcDisable            (int32 ExceptionNumber);

/*
** Floating Point Unit API
*/

int32 NEO_OS_FPUExcAttachHandler   (uint32 ExceptionNumber, void * ExceptionHandler , int32 parameter);
int32 NEO_OS_FPUExcEnable          (int32 ExceptionNumber);
int32 NEO_OS_FPUExcDisable         (int32 ExceptionNumber);
int32 NEO_OS_FPUExcSetMask         (uint32 mask);
int32 NEO_OS_FPUExcGetMask         (uint32 *mask);

/*
** Interrupt API
*/
int32 NEO_OS_IntAttachHandler  (uint32 InterruptNumber, osal_task_entry InterruptHandler, int32 parameter);
int32 NEO_OS_IntUnlock         (int32 IntLevel);
int32 NEO_OS_IntLock           (void);

int32 NEO_OS_IntEnable         (int32 Level);
int32 NEO_OS_IntDisable        (int32 Level);

int32 NEO_OS_IntSetMask        (uint32 mask);
int32 NEO_OS_IntGetMask        (uint32 *mask);
int32 NEO_OS_IntAck             (int32 InterruptNumber);

/*
** Shared memory API
*/
int32 NEO_OS_ShMemInit          (void);
int32 NEO_OS_ShMemCreate        (uint32 *Id, uint32 NBytes, const char* SegName);
int32 NEO_OS_ShMemSemTake       (uint32 Id);
int32 NEO_OS_ShMemSemGive       (uint32 Id);
int32 NEO_OS_ShMemAttach        (cpuaddr * Address, uint32 Id);
int32 NEO_OS_ShMemGetIdByName   (uint32 *ShMemId, const char *SegName );

/*
** Heap API
*/
int32 NEO_OS_HeapGetInfo       (NEO_OS_heap_prop_t *heap_prop);

/*
** API for useful debugging function
*/
int32 NEO_OS_GetErrorName      (int32 error_num, os_err_name_t* err_name);


/*
** Abstraction for printf statements
*/
void NEO_OS_printf( const char *string, ...) NEO_OS_PRINTF(1,2);
void NEO_OS_printf_disable(void);
void NEO_OS_printf_enable(void);

/*
** Call to exit the running application
** Normally embedded applications run forever, but for debugging purposes
** (unit testing for example) this is needed in order to end the test
*/
void NEO_OS_ApplicationExit(int32 Status);

#ifdef __cplusplus
}
#endif

} // namespace os
} // namespace neo
} // namespace neodain

#endif // __NEO_OSAPI_CORE_H__



