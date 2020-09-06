#ifndef __NEO_OS_TASK_H__
#define __NEO_OS_TASK_H__

namespace neodain {
namespace neo {
namespace os {

#include <iostream>


/*
struct Runnalbe {
    virtual void Run() = 0;    
};
*/

// equal to Runnable struct
class Runnalbe {
  public:
    virtual void Run() = 0;    
};

class NeoTask : public Runnable
{
  private:
    pthread_t thread_m;
    Runnable* pRunnable_m;
    static void* neo_task(void* pInst);

  public:
    NeoTask();
    NeoTask(Runnable* pRunnable);
    void start();
    void wait();
    virtual void run();

};



} // namespace os
} // namespace neo
} // namespace neodain

#endif // __NEO_OS_TASK_H__
