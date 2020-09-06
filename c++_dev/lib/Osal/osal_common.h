#ifndef __OSAL_COMMON_H__
#define __OSAL_COMMON_H__

#include <cstdio>
#include <ctime>
#include <functional>
#include <string>
#include <chrono>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <climits.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <stdexcept>
#include <iostream>
#include <typeinfo>

#ifdef _WIN32
#include <windows.h>
#include <process.h> // _get_pid support
#include <io.h> // _get_oshandle and _isatty support

#ifdef __MINGW32__
#include <share.h>
#endif

#else // unix

#include <unistd.h>
#include <fcntl.h>
#include <sys/vfs.h>

#ifdef __linux__
#include <sys/syscall.h> // use gettid() syscall under linux to get thread id
#elif __FreeBSD__
#include <sys/thr.h> // use thr_self() syscall under FreeBSD to get thread id
#endif

#endif // unix

#if 0
//visual studio upto 2013 does not support noexcept nor constexpr
#if defined(_MSC_VER) && (_MSC_VER < 1900)
#define NEO_NOEXCEPT throw()
#define NEO_CONSTEXPR
#else
#define NEO_NOEXCEPT noexcept
#define NEO_CONSTEXPR constexpr
#endif
#endif

// need to study : http://jmhartsoftware.com/opensource.htm
#ifdef _WIN32 /* Windows requires definitions for the POSIX file I/O functions */
#define O_CREAT _O_CREAT
#define O_TRUNC _O_TRUNC

#ifndef S_IRUSR
#define S_IRUSR _S_IREAD
#endif

#ifndef S_IWUSR
#define S_IWUSR _S_IWRITE
#endif

#ifdef __MINGW32__
#define _SH_DENYNO 0x40
#endif



#define _INTEGRAL_MAX_BITS 64
#define read(fd,pbuffer,count) _read(fd,pbuffer,count)
#define write(fd,pbuffer,count) _write(fd,pbuffer,count)
#define open(fn,flag) _open(fn,flag)
#define open3(fn,flag,mode) _open(fn,flag,mode)
#define close(fd) _close(fd)
#define lseek64(handle,offset,origin) _lseeki64(handle,offset,origin)
#define sleep(t) Sleep(1000*(t))
#define sync() ;
#define off64_t __int64
#define size64_t __int64
#else 
#ifdef __linux__
#define open3(fn,flag,mode) open(fn,flag,mode)
#define lseek64(handle,offset,origin) lseek(handle,offset,origin)
#define FlushFileBuffers(i) 1
#define off64_t long long
#define size64_t long long
#else 
#define open3(fn,flag,mode) open(fn,flag,mode)
#define FlushFileBuffers(i) 1
#define size64_t long long
#endif
#endif

#if 0
#ifdef _WIN32
#define _WIN32_WINNT 0x500 /* Require Windows NT5 (2K, XP, 2K3) */
#include <windows.h>
#include <time.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif
#endif

#include <cerrno.h>

/* Thread management macros */
#ifdef _WIN32
#define THREAD_FUNCTION DWORD WINAPI
#define THREAD_FUNCTION_RETURN DWORD
#define THREAD_SPECIFIC_INDEX DWORD
#define pthread_t HANDLE
#define pthread_attr_t DWORD
#define pthread_create(thhandle,attr,thfunc,tharg) (int)((*thhandle=(HANDLE)_beginthreadex(NULL,0,(THREAD_FUNCTION)thfunc,tharg,0,NULL))==NULL)
#define pthread_join(thread, result) ((WaitForSingleObject((thread),INFINITE)!=WAIT_OBJECT_0) || !CloseHandle(thread))
#define pthread_detach(thread) if(thread!=NULL)CloseHandle(thread)
#define thread_sleep(nms) Sleep(nms)
#define pthread_cancel(thread) TerminateThread(thread,0)
#define ts_key_create(ts_key, destructor) {ts_key = TlsAlloc();};
#define pthread_getspecific(ts_key) TlsGetValue(ts_key)
#define pthread_setspecific(ts_key, value) TlsSetValue(ts_key, (void *)value)
#define pthread_self() GetCurrentThreadId()
#else
/* pthreads */
/* Nearly everything is already defined */
#define THREAD_FUNCTION void *
#define THREAD_FUNCTION_RETURN void *
#define THREAD_SPECIFIC_INDEX pthread_key_t
#define thread_sleep(nms) sleep((nms+500)/1000)
#define ts_key_create(ts_key, destructor) pthread_key_create (&(ts_key), destructor);
#endif

/* Syncrhronization macros: Win32 and Pthreads */
#ifdef _WIN32
#define pthread_mutex_t HANDLE
#define pthread_cond_t HANDLE
#define pthread_mutex_lock(pobject) WaitForSingleObject(*pobject,INFINITE)
#define pthread_mutex_unlock(pobject) ReleaseMutex(*pobject)
#define pthread_mutex_init(pobject,pattr) (*pobject=CreateMutex(NULL,FALSE,NULL))
#define pthread_cond_init(pobject,pattr) (*pobject=CreateEvent(NULL,FALSE,FALSE,NULL))
#define pthread_mutex_destroy(pobject) CloseHandle(*pobject)
#define pthread_cond_destroy(pobject) CloseHandle(*pobject)
#define CV_TIMEOUT INFINITE /* Tunable value */

/* USE THE FOLLOWING FOR WINDOWS 9X */
/* For addtional explanation of the condition variable emulation and the use of the
 * timeout, see the paper "Batons: A Sequential Synchronization Object" 
 * by Andrew Tucker and Johnson M Hart. (Windows Developer뭩 Journal, 
 * July, 2001, pp24 ff. www.wdj.com). */
//#define pthread_cond_wait(pcv,pmutex) {ReleaseMutex(*pmutex);WaitForSingleObject(*pcv,CV_TIMEOUT);WaitForSingleObject(*pmutex,INFINITE);};
/* You can use the following on Windows NT/2000/XP and avoid the timeout */

#define pthread_cond_wait(pcv,pmutex) {SignalObjectAndWait(*pmutex,*pcv,INFINITE,FALSE);WaitForSingleObject(*pmutex,INFINITE);};

//#define pthread_cond_broadcast(pcv) PulseEvent(*pcv)
#define pthread_cond_signal(pcv) SetEvent(*pcv)
static int OnceFlag;

//static DWORD ThId; /* This is ugly, but is required on Win9x for _beginthreadex */
#else
/* Not Windows. Assume pthreads */
#endif


namespace neodain {
namespace neo {
namespace os {
}
}
}

#endif
