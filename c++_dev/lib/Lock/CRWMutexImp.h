#ifndef __CRWLOCK_IMP_H__
#define __CRWLOCK_IMP_H__
#include <pthread.h>
#include <errno.h>

class CRWMutexImp
{
  public:
	CRWMutexImp()
	{
		if(0 != pthread_rwlock_init(&rwLock_m, NULL))
		{
			/*
			char buff[40];
			strerror_r(errno,buff,40);
			cmfLog(CLog::CRITICAL,"CRWMutexImpPosix creation failed : %s",buff);
			*/
			throw std::bad_alloc();
		}
	}
	~CRWMutexImp() { pthread_rwlock_destroy(&rwLock_m); }
	void readLock() { pthread_rwlock_rdlock(&rwLock_m); }
	void writeLock() { pthread_rwlock_wrlock(&rwLock_m); }
	void unlock() { pthread_rwlock_unlock(&rwLock_m); }
 private:
	CRWMutexImp(const CRWMutexImp& inRWMutexImp);
	CRWMutexImp& operator=(const CRWMutexImp& inRWMutexImp);
	pthread_rwlock_t rwLock_m;
};	// class CRWMutexImp
#endif
