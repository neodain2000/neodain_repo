#ifndef __CTHREAD_IMP_H__
#define __CTHREAD_IMP_H__
#include <string>
class CThreadImp
{
  public:
    CThreadImp() { }
    virtual ~CThreadImp() { }
    virtual bool detach() = 0;
    virtual bool join() = 0;
    virtual const std::string& getName() const = 0;
    virtual uint32_t getPriority() const = 0;
    virtual bool setPriority(uint32_t priority) = 0;
    virtual bool kill(uint32_t sig) = 0;
    virtual bool isDetached() const = 0;
    virtual bool start() = 0;
    virtual pid_t getThreadId() const = 0;
    virtual uint64_t getSelfId() const = 0;
    virtual void storeId() = 0;
  private:
    CThreadImp(const CThreadImp& inThreadImp);
    CThreadImp& operator=(const CThreadImp& inThreadImp);
};  // class CThreadImp

#endif
