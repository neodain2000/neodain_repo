
namespace neodain {
namespace neo {
namespace os {

#include <pthread.h>
#include "neo_os_task.h"


NeoTask::NeoTask() : thread_m(0), pRunnable_m(0)
{
	std::cout << "NeoTask::NeoTask() created..." << std::endl;
}

NeoTask::NeoTask(Runnable* pRunnable) : thread_m(0), pRunnable_m(pRunnable)
{
	std::cout << "NeoTask::NeoTask() created..." << std::endl;
}

void NeoTask::start()
{
	pthread_t n_thread;
	int32_t nr = pthread_create(&n_thread, NULL, &NeoTask::neo_task, this);
	if(nr != 0)
	{
		std::cout << "NeoTask::start() pthread_create fail(" << nr << ":" << strerror(errno) << ")" << std::endl; 
	}
	std::cout << "NeoTask::start() pthread_create success" << std::endl;
}

void NeoTask::run()
{
	if(pRunnable_m != 0)
	{
		pRunnable_m->Run();
	}
}

void NeoTask::wait()
{
	void* pData;
	int32_t nr = pthread_join(thread_m, &pData);
}

void NeoTask::init()
{
}

void *NeoTask::neo_task(void *pInst)
{
    NeoTask *pT = static_cast<NeoTask*>(pInst);
    pT->Run();
}



} // namespace os
} // namespace neo
} // namespace neodain

