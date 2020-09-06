#ifndef __CCONDVAR_IMP_H__
#define __CCONDVAR_IMP_H__
#include <time.h>
#include <pthread.h>
#include "CMutexImp.h"
#include <stdint.h>
#include <errno.h>
#include <new>
#include <string.h>
class CCondVarImp
{
  public:
    CCondVarImp()
    {
        pthread_condattr_t attr;
        int ret = 0;
        ret = pthread_condattr_init(&attr);
        if(ret != 0)
        {
            throw std::bad_alloc(); // throw exception
        }
        ret = pthread_condattr_setclock(&attr, CLOCK_MONOTONIC);
        if(ret != 0)
        {
            throw std::bad_alloc(); // throw exception
        }
        ret = pthread_cond_init(&cond_m,&attr);
        if(ret != 0)
        {
            throw std::bad_alloc(); // throw exception
        }
        pthread_condattr_destroy(&attr);
    }
    ~CCondVarImp()
    {
        int ret = pthread_cond_destroy(&cond_m);
        if(ret != 0)
        {
            //throw std::bad_alloc(); // throw exception
        }
    }
    bool broadcast()
    {
        int ret = pthread_cond_broadcast(&cond_m);
        if(ret != 0)
        {
            //throw std::bad_alloc(); // throw exception
            return false;
        }
        return true;
    }
    bool signal()
    {
        int ret = pthread_cond_signal(&cond_m);
        if(ret != 0)
        {
            //throw std::bad_alloc(); // throw exception
            return false;
        }
        return true;
    }
    bool wait(CMutexImp & mutexImp,uint32_t sec, uint32_t nsec)
    {
        struct timespec current;
        memset(&current, 0, sizeof(struct timespec));
        struct timespec timeout;
        memset(&timeout, 0, sizeof(struct timespec));
        clock_gettime(CLOCK_MONOTONIC, &current);
        timeout.tv_nsec = current.tv_nsec + nsec;
        timeout.tv_sec = current.tv_sec + sec + (timeout.tv_nsec/1000000000);
        timeout.tv_nsec %= 1000000000;
        int ret = pthread_cond_timedwait(&cond_m,&mutexImp.getMutex(),&timeout);
        if(ret == ETIMEDOUT)
        {
            //throw std::bad_alloc(); // throw exception
            return false;
        }
        else if(ret != 0)
        {
            //throw std::bad_alloc(); // throw exception
            return false;
        }
        return true;
    }
    bool wait(CMutexImp & mutexImp)
    {
        int ret = pthread_cond_wait(&cond_m,&mutexImp.getMutex());
        if(ret != 0)
        {
            //throw std::bad_alloc(); // throw exception
            return false;
        }
        return true;
    }
  private:
    CCondVarImp(const CCondVarImp& inCondVarImp);
    CCondVarImp& operator=(const CCondVarImp& inCondVarImp);
    pthread_cond_t cond_m;
}

#endif
