#ifndef __CTHREAD_H__
#define __CTHREAD_H__
#include <string>
#include <pthread.h>
#include "CMutex.h"
// #include "CThreadWork.h"
// #include "CThreadImp.h"
// class CThreadWork;
class CThreadImp;
/////////////////////////////////////////////////////////////////
/// class CThread
/// @par
/// Thread 생성하고, 실행하기 위한 Class이다.\n
/// CThread 를 상속한 자식클래스를 정의하고 workFunc 를 오버라이딩하여야 한다.
/// 자식 클래스를 객체화 하고 start() 메쏘드를 호출하면 Thread가 생성되고
/// 생성된 Thread 에서 workFunc()가 수행된다.
/////////////////////////////////////////////////////////////////
class CThread //: public CThreadWork
{
  public:
    enum ThreadSchedPolicy_e {
        THREAD_SCHED_OTHER,
        THREAD_SCHED_FIFO,
        THREAD_SCHED_RR,
        THREAD_SCHED_MAX=THREAD_SCHED_RR
    };
    static const uint32_t MIN_PRIORITY;
    static const uint32_t MAX_PRIORITY;
    ////////////////////////////////////////////////////////////////////
    /// @brief 생성자
    /// @param name Thread 이름
    /// @param policy Thread의 scheduling policy
    /// @arg @c THREAD_SCHED_OTHER normal scheduling
    /// @arg @c THREAD_SCHED_FIFO priority based pre-emptive scheduling
    /// @arg @c THREAD_SCHED_RR priority-based realtime round-robin scheduling
    /// @param priority Thread의 scheduling priority
    /// @param stackSize Thread의 stack 크기. (OClovis) 초기값은 192K 이다.
    /// @param bDetached Detach 여부.
    /// @arg @c true  Detach 된 Thread 객체는 Thread의 workFunc(..)이 종료되면 자동 소멸된다.
    /// @arg @c false Attached 된 Thread 객체는 사용자에 의해 kill 및 join 되어 소멸되어야 한다.
    ////////////////////////////////////////////////////////////////////
    CThread(const std::string& name,
        ThreadSchedPolicy_e policy,
        uint32_t priority,
        uint32_t stackSize,
        bool bDetached);
    virtual ~CThread();
    bool detach() { return imp_mp->detach(); } // attach된 thread를 detach 한다.
    bool join() { return imp_mp->join(); } // attach된 thread를 join한다.
    const std::string & getName() const { return imp_mp->getName(); }

    uint32_t getPriority() const { return imp_mp->getPriority(); }
    bool setPriority(uint32_t priority) { return imp_mp->setPriority(priority); }
    // 단, Detached thread는 start 후에 priority를 변경할 수 없다.
    bool kill(uint32_t sig) { return imp_mp->kill(sig); } // Thread로 signal을 전달한다.
    bool isDetached() const { return imp_mp->isDetached(); }
    bool start() { return imp_mp->start(); }
    pid_t getThreadId() const { return imp_mp->getThreadId(); } // tid(LWP)
    uint64_t getSelfId() const { return imp_mp->getSelfId(); }
    void storeId() { imp_mp->storeId(); }
    virtual void workFunc();
    static void delay(uint32_t sec,uint32_t milliSec);
    static uint64_t self() { return pthread_self(); }
  private:
    CThread(const CThread& inThread);
    CThread& operator=(const CThread& inThread);
    static CThreadImp* createThread(const std::string& name,
        ThreadSchedPolicy_e policy,
        uint32_t priority,
        uint32_t stackSize,
        CThread& work,
        bool bDetached);
    CThreadImp* const imp_mp;
};
#endif
