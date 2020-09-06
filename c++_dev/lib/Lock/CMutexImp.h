#ifndef __CMUTEX_IMP_H__
#define __CMUTEX_IMP_H__
#include <stdlib.h>
#include <pthread.h>
class CMutexImp
{
	friend class CCondVarImp;
  public:
	CMutexImp()
	{
		pthread_mutexattr_t mattr;
		int ret;
		if((ret = pthread_mutexattr_init(&mattr)) != 0)
		{
			throw std::bad_alloc();
		}
		if((ret = pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE)) != 0)
		{
			pthread_mutexattr_destroy(&mattr);
			throw std::bad_alloc();
		}
		if((ret = pthread_mutex_init(&mutex_m,&mattr)) != 0)
		{
			pthread_mutexattr_destroy(&mattr);
			throw std::bad_alloc();
		}
		pthread_mutexattr_destroy(&mattr);
	}
	~CMutexImp()
	{
		int ret;
		if((ret = pthread_mutex_destroy(&mutex_m)) != 0) { ; }
	}
	void lock()
	{
		int ret = pthread_mutex_lock (&mutex_m);
		if(ret != 0) { ; }
	}
	void unlock()
	{
		int ret = pthread_mutex_unlock (&mutex_m);
		if(ret != 0) { ; }
	}
  private:
	CMutexImp(const CMutexImp& inMutexImp);
	CMutexImp& operator=(const CMutexImp& inMutexImp);
	pthread_mutex_t& getMutex() { return mutex_m; }
	pthread_mutex_t mutex_m;
};
#endif
