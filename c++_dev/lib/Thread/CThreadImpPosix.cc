#include "CThreadImpPosix.h"
#include "CThread.h"
#include <errno.h>
#include <signal.h>
#include <sys/time.h>
#include <asm/unistd.h>

inline uint32_t CThreadImpPosix::getPosixPolicy(CThread::ThreadSchedPolicy_e policy)
{
    switch(policy)
    {
        case CThread::THREAD_SCHED_OTHER: return SCHED_OTHER;
        case CThread::THREAD_SCHED_FIFO: return SCHED_FIFO;
        case CThread::THREAD_SCHED_RR: return SCHED_RR;
        default: return SCHED_OTHER;
    }
}
inline uint32_t CThreadImpPosix::getPosixPriority(CThread::ThreadSchedPolicy_e policy, uint32_t cmf_priority)
{
    if(policy == CThread::THREAD_SCHED_OTHER) return 0;
#ifdef _POSIX_PRIORITY_SCHEDULING
    int posix_max = sched_get_priority_max(getPosixPolicy(policy));
    int posix_min = sched_get_priority_min(getPosixPolicy(policy));
    int posix_range = posix_max - posix_min;
    int cmf_range = (int)(CThread::MAX_PRIORITY - CThread::MIN_PRIORITY);
    return ((cmf_priority - CThread::MIN_PRIORITY) * posix_range / cmf_range) + posix_min;
#else
    return (1 + (cmf_priority/2));
#endif
}

CThreadImpPosix::CThreadImpPosix(
    const std::string& name,
    CThread::ThreadSchedPolicy_e policy,
    uint32_t priority,
    uint32_t stackSize,
    CThread& work,
    bool bDetached)
:CThreadImp(), bDetached_m(bDetached), name_m(name), policy_m(policy), priority_m(priority), stackSize_m(stackSize), bStarted_m(false), work_mp(&work)
{
    cout << "CThreadImpPosix(" << name_m.c_str() << ")::construction" << endl;
}

CThreadImpPosix::~CThreadImpPosix()
{
    cout << "CThreadImpPosix(" << name_m.c_str() << ")::try to destory" << endl;
    if(bStarted_m)
    {
        /*
        if(!bDetached_m)
        {
            pthread_detach(threadId_m);
        }
        */
        if(threadId_m == pthread_self())
        {
            pthread_exit((void*)NULL);
        }
        else
        {
            pthread_cancel(threadId_m);
        }
    }
    // do not delete work_mp; work_mp is already deleted.
}

bool CThreadImpPosix::start()
{
    if(!bStarted_m)
    {
        pthread_attr_t threadAttribute;
        uint32_t ret = pthread_attr_init(&threadAttribute);
        if(0 != ret)
        {
            cout << "CThreadImpPosix(" << name_m.c_str() << ")::start - thread attribute init failed(ret : " << ret << ")" << endl;
            return false;
        }
        ret = pthread_attr_setstacksize(&threadAttribute,stackSize_m);
        if(0 != ret)
        {
            cout << "CThreadImpPosix(" << name_m.c_str() << ")::start - thread attribute set stacksize failed(ret : " << ret << ")" << endl;
            return false;
        }
        if(bDetached_m)
        {
            ret = pthread_attr_setdetachstate(&threadAttribute,PTHREAD_CREATE_DETACHED);
            if(0 != ret)
            {
                cout << "CThreadImpPosix(" << name_m.c_str() << ")::start - thread attribute set detach faile(ret : " << ret << ")" << endl;
                return false;
            }
        }
        uint32_t retry = 0;
        do {
            if(ret != 0)
            {
                delay(0,500);
            }
            ret = pthread_create(&threadId_m,&threadAttribute,threadFunc,static_cast<void*>(this));
        } while(ret && (ret == EAGAIN || ret == ENOMEM) && ++retry < 3 );
        if(ret != 0)
        {
            cout << "CThreadImpPosix(" << name_m.c_str() << ")::start - thread creation failed(ret : " << ret << ")" << endl;
            return false;
        }

        if(false == bDetached_m)
        {
            struct sched_param schedParam = {0};
            schedParam.sched_priority = getPosixPriority(policy_m,priority_m);
            if(policy_m != CThread::THREAD_SCHED_OTHER)
            {
                ret = pthread_setschedparam(threadId_m,getPosixPolicy(policy_m),(const struct sched_param*)&schedParam);
                if(ret != 0)
                {
                    cout << "CThreadImpPosix(" << name_m.c_str() << ")::start - set schedparam failed(ret : " << ret << ")" << endl;
                    //do not return with false;
                }
            }
        }
        bStarted_m = true;
        cout << "CThreadImpPosix(" << name_m.c_str() << ")::started" << endl;
    }
    return true;
}

bool CThreadImpPosix::detach()
{
    if(!bStarted_m)
    {
        // thread is not started
        cout << "CThreadImpPosix::detached" << endl;
        bDetached_m = true;
        return true;
    }
    else if(!bDetached_m)
    {
        //if(threadId_m != pthread_self())  ???
        {
            int32_t ret = pthread_detach(threadId_m);
            if(ret != 0)
            {
                cout << "CThreadImpPosix::detaching failed : " << ret << endl;
                return false;
            }
            return true;
        }
        return false;
    }
    return false;
}

bool CThreadImpPosix::join()
{
    if(bStarted_m && (!bDetached_m))
    {
        cout << "CThreadImpPosix::join" << endl;

        if(threadId_m != pthread_self())
        {
            int32_t ret = pthread_join(threadId_m,0);
            if(ret != 0)
            {
                cout << "CThreadImpPosix::join failed(" << ret << ")" << endl;
                return false;
            }
            return true;
        }
        else
        {
            cout << "CThreadImpPosix::join cannot join itself" << endl;
            return false;
        }
    }
    else
    {
        cout << "CThreadImpPosix::join not started or not detached yet" << endl;
        return false;
    }
}

const std::string & CThreadImpPosix::getName() const
{
    return name_m;
}

uint32_t CThreadImpPosix::getPriority() const
{
#if 0
    ClUint32T  taskPriority;
    if(!bDetached_m)
    {
        if(clOsalTaskPriorityGet(taskId_m, &taskPriority) == true)
        {
            return getCThreadPriority(taskPriority);
        }
    }
#endif
    return priority_m;
}

bool CThreadImpPosix::setPriority(uint32_t cmf_priority)
{
    if(policy_m == CThread::THREAD_SCHED_OTHER) return false;
    if(bStarted_m)
    {
        struct sched_param schedParam = {0};
        int32_t policy;
        int32_t ret = pthread_getschedparam(threadId_m,&policy,&schedParam);
                                                                                                                                                    
        // thread is started and attached
        if(ret != 0)
        {
            cout << "CThreadImpPosix::setPriority - cannot get current param" << endl;
            return false;
        }
        schedParam.sched_priority = getPosixPriority(policy_m,cmf_priority);
        ret = pthread_setschedparam(threadId_m,getPosixPolicy(policy_m),&schedParam);
        if(0 != ret)
        {
            cout << "CThreadImpPosix::setPriority - cannot set param" << endl;
            return false;
        }
        priority_m = cmf_priority;
        return true;
    }
    else
    {
        priority_m = cmf_priority;
    }
    return false;
}

bool CThreadImpPosix::kill(uint32_t sig)
{
    if(bStarted_m)
    {
        cout << "CThreadImpPosix::kill(" << sig << ")" << endl;
        // thread is started and attached
        int32_t ret = pthread_kill(threadId_m,sig);
        if(0 != ret)
        {
            cout << "CThreadImpPosix::kill(" << sig << ") failed(ret:" << ret << ")" << endl;
            return false;
        }
        return true;
    }
    return false;
}

bool CThreadImpPosix::isDetached() const
{
    return bDetached_m;
}

void* CThreadImpPosix::threadFunc(void* arg_p)
{
    CThreadImpPosix* impPosix_p = static_cast<CThreadImpPosix*>(arg_p);
                                                                                                  

    // impPosix_p->storeId();
    cout << "CThreadImpPosix::threadFunc running" << endl;
    if(impPosix_p->policy_m != CThread::THREAD_SCHED_OTHER)
    {
        struct sched_param schedParam = {0};
        schedParam.sched_priority = getPosixPriority(impPosix_p->policy_m,impPosix_p->priority_m);
        int32_t ret = pthread_setschedparam(impPosix_p->threadId_m,getPosixPolicy(impPosix_p->policy_m),(const struct sched_param*)&schedParam);
        if(0 != ret)
        {
            cout << "CThreadImpPosix::threadFunc - cannot set sched param : " << impPosix_p->name_m.c_str() << endl;
        }
    }
    impPosix_p->work_mp->workFunc();
    cout << "CThreadImpPosix::threadFunc exit : " << impPosix_p->getName().c_str() << endl;
    if(impPosix_p->isDetached())
    {
        cout << "CThreadImpPosix::try to destry CThead" << endl;
        delete impPosix_p;
    }
    return 0;
}

void CThreadImpPosix::delay(uint32_t sec,uint32_t milliSec)
{
    int32_t ret = 0;
    struct timespec timeOut; memset(&timeOut, 0, sizeof(timespec));
    timeOut.tv_sec = sec;
    timeOut.tv_nsec = (milliSec%1000)*1000*1000;
    do {
        ret = nanosleep(&timeOut,&timeOut);
        if(0 != ret && EINTR != errno)
        {
            cout << "CThreadImpPosix::delay failed : " << ret << endl;
            return;
        }
    } while(0 != ret && EINTR == errno);
}
uint64_t CThreadImpPosix::self()
{
    return (uint64_t)pthread_self();
}
pid_t gettid(void)
{
   return syscall(__NR_gettid);
}
 

void CThreadImpPosix::storeId()
{
    threadId_m = gettid();
    selfId_m = self();
}
