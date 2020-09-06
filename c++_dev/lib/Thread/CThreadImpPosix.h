#ifndef __CTHREADIMPPOSIX_H__
#define __CTHREADIMPPOSIX_H__
#include <string>
#include "CThread.h"
#include "CThreadImp.h"
#include "pthread.h"

class CThread;
class CThreadImpPosix : public CThreadImp
{
  public:
    CThreadImpPosix(
        const std::string& name,
        CThread::ThreadSchedPolicy_e policy,
        uint32_t priority,
        uint32_t stackSize,
        CThread& work,
        bool bDetached
    );
    virtual ~CThreadImpPosix();
    virtual bool detach();
    virtual bool join();
    virtual const std::string & getName() const;
    virtual uint32_t getPriority() const;
    virtual bool setPriority(uint32_t priority);
    virtual bool kill(uint32_t sig);
    virtual bool isDetached() const;
    virtual bool start();
    virtual pid_t getThreadId() const { return threadId_m; }
    virtual uint64_t getSelfId() const { return selfId_m; }
    virtual void storeId();
    static void delay(uint32_t sec,uint32_t milliSec);
    uint64_t self();
  private:
    CThreadImpPosix(const CThreadImpPosix& inThreadImpPosix);
    CThreadImpPosix& operator=(const CThreadImpPosix& inThreadImpPosix);
    static void* threadFunc(void*);
    inline static uint32_t getPosixPolicy(CThread::ThreadSchedPolicy_e policy);
    inline static uint32_t getPosixPriority(CThread::ThreadSchedPolicy_e policy,uint32_t cmf_priority);
    bool bDetached_m;
    const std::string name_m;
    CThread::ThreadSchedPolicy_e policy_m;
    uint32_t priority_m;
    uint32_t stackSize_m;
    bool bStarted_m;
    CThread* const work_mp;
    pthread_t threadId_m;
    uint64_t selfId_m;
};  // class CThreadImpPosix
#endif 
