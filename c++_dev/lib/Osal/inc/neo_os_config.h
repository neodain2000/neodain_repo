#ifndef __NEO_OS_CONFIG_H__
#define __NEO_OS_CONFIG_H__

namespace neodain {
namespace neo {
namespace os {

/*
** Platform Configuration Parameters for the OS API
*/

#define NEO_OS_MAX_TASKS                64
#define NEO_OS_MAX_QUEUES               64
#define NEO_OS_MAX_COUNT_SEMAPHORES     20
#define NEO_OS_MAX_BIN_SEMAPHORES       20
#define NEO_OS_MAX_MUTEXES              20

/*
** Maximum length for an absolute path name
*/
#define NEO_OS_MAX_PATH_LEN     64

/*
** Maximum length for a local or host path/filename.
**   This parameter can consist of the OSAL filename/path +
**   the host OS physical volume name or path.
*/
#define NEO_OS_MAX_LOCAL_PATH_LEN (NEO_OS_MAX_PATH_LEN + NEO_OS_FS_PHYS_NAME_LEN)

/*
** The maxium length allowed for a object (task,queue....) name
*/
#define NEO_OS_MAX_API_NAME     20

/*
** The maximum length for a file name
*/
#define NEO_OS_MAX_FILE_NAME    20
/*
** These defines are for NEO_OS_printf
*/
#define NEO_OS_BUFFER_SIZE 172
#define NEO_OS_BUFFER_MSG_DEPTH 100

/* This #define turns on a utility task that
 * will read the statements to print from
 * the NEO_OS_printf function. If you want NEO_OS_printf
 * to print the text out itself, comment this out
 *
 * NOTE: The Utility Task #defines only have meaning
 * on the VxWorks operating systems
 */

#define NEO_OS_UTILITY_TASK_ON


#ifdef NEO_OS_UTILITY_TASK_ON
    #define NEO_OS_UTILITYTASK_STACK_SIZE 2048
    /* some room is left for other lower priority tasks */
    #define NEO_OS_UTILITYTASK_PRIORITY   245
#endif


/*
** the size of a command that can be passed to the underlying OS
*/
#define NEO_OS_MAX_CMD_LEN 1000

/*
** This define will include the OS network API.
** It should be turned off for targtets that do not have a network stack or
** device ( like the basic RAD750 vxWorks BSP )
*/
#define NEO_OS_INCLUDE_NETWORK

/*
** This is the maximum number of open file descriptors allowed at a time
*/
#define NEO_OS_MAX_NUM_OPEN_FILES 50

/*
** This defines the filethe input command of NEO_OS_ShellOutputToFile
** is written to in the VxWorks6 port
*/
#define NEO_OS_SHELL_CMD_INPUT_FILE_NAME "/ram/NEO_OS_ShellCmd.in"

/*
** This define sets the queue implentation of the Linux port to use sockets
** commenting this out makes the Linux port use the POSIX message queues.
*/
/* #define OSAL_SOCKET_QUEUE */

/*
** Module loader/symbol table is optional
*/
#define NEO_OS_INCLUDE_MODULE_LOADER

#ifdef NEO_OS_INCLUDE_MODULE_LOADER
   /*
   ** This define sets the size of the OS Module Table, which keeps track of the loaded modules in
   ** the running system. This define must be set high enough to support the maximum number of
   ** loadable modules in the system. If the the table is filled up at runtime, a new module load
   ** would fail.
   */
   #define NEO_OS_MAX_MODULES 20

   /*
   ** The Static Loader define is used for switching between the Dynamic and Static loader implementations.
   */
   /* #define NEO_OS_STATIC_LOADER */

#endif


/*
** This define sets the maximum symbol name string length. It is used in implementations that
** support the symbols and symbol lookup.
*/

#define NEO_OS_MAX_SYM_LEN 64

/*
** This define sets the maximum number of time base objects
** The limit depends on the underlying OS and the resources it offers, but in general
** these are a limited resource and only a handful can be created.
**
** This is included as an example, for OSAL implementations that do not [yet] support
** separate timebase objects, this directive will be ignored.  However, the OSAL unit
** test stub code does require that this is defined.
*/
#define NEO_OS_MAX_TIMEBASES      5

/*
** This define sets the maximum number of user timers available
** The limit here depends on whether the OSAL implementation uses limited resources
** for a timer object; in the case of the newer "posix-ng" and "rtems-ng" variants,
** the "timebase" allocates the OS resources and the timer does not use any additional
** OS resources. Therefore this limit can be higher.
*/
#define NEO_OS_MAX_TIMERS         5

/*
** This define sets the maximum number of open directories
*/
#define NEO_OS_MAX_NUM_OPEN_DIRS  4

/*
** This define sets the maximum depth of an OSAL message queue.  On some implementations this may
** affect the overall OSAL memory footprint so it may be beneficial to set this limit according to
** what the application actually needs.
*/
#define NEO_OS_QUEUE_MAX_DEPTH    50

/*
 * If NEO_OS_DEBUG_PRINTF is defined, this will enable the "NEO_OS_DEBUG" statements in the code
 * This should be left disabled in a normal build as it may affect real time performance as
 * well as producing extra console output.
 */
#undef NEO_OS_DEBUG_PRINTF


/*
 * If OSAL_DEBUG_PERMISSIVE_MODE is defined, this will enable features to make the
 * OSAL library compatible with a non-root (normal user mode) environment.   In the PC-Linux/Posix
 * build, this means:
 *  - A message queue deeper than the maximum system limit will be silently truncated
 *    to the maximum system limit (no error).
 *  - If the user does not have permission to create elevated priority tasks, then the tasks will
 *    be created at the default priority (no error).  Note this behavior can also be forced by the
 *    OSAL_DEBUG_DISABLE_TASK_PRIORITIES macro below.
 *
 * Leaving this undefined will produce the default behavior, which is to return errors to the caller
 * for these conditions.
 */
#define OSAL_DEBUG_PERMISSIVE_MODE


} // namespace os
} // namesapce neo
} // namespace neodain

#endif // __NEO_OS_CONFIG_H__

