#include "CThread.h"
#include "CThreadWork.h"
#include "CThreadImp.h"
#include "CThreadImpPosix.h"

const uint32_t CThread::MIN_PRIORITY = 1;
const uint32_t CThread::MAX_PRIORITY = 99;
CThread::CThread(const std::string& name,
    ThreadSchedPolicy_e policy,
    uint32_t priority,
    uint32_t stackSize,
    bool bDetached)
:imp_mp(createThread(name,
    policy,
    priority,
    stackSize,
    *this,
    bDetached))
{
    cout << "CThread(" << getName().c_str()
    << ")::CThread(policy:" << policy
    << ", priority:" << priority
    << ", stackSize:" << stackSize
    << ", detached:" << bDetached << ")" << endl;
}
CThread::~CThread()
{
    cout << "CThread("getName().c_str() << ")::destructed" << endl;
    delete imp_mp;
}
void CThread::workFunc() { }
CThreadImp* CThread::createThread(
    const std::string& name,
    ThreadSchedPolicy_e policy,
    uint32_t priority,
    uint32_t stackSize,
    CThread& work,
    bool bDetached)
{
    return new CThreadImpPosix(name, policy, priority, stackSize, work, bDetached);
}
void CThread::delay(uint32_t sec, uint32_t milliSec)
{
    CThreadImpPosix::delay(sec, milliSec);
}
