#ifndef __CLOCK_H__
#define __CLOCK_H__
#include "CLockVar.h"
#include "CMutex.h"
#include "CRWMutex.h"

class CMutex;
class CRWMutex;
/////////////////////////////////////////////////////////////////
/// @class CLock
/// @par
/// Mutex들에 대한 RAII(Resource acquisition is initialization)를 구현한 Class 이다.
/// RAII는 생성자에서 자원을 할당하고 소멸자에서 자원을 해제함으로써 자원의 미반환을 방지하기 위한 것이다.
/// 따라서 CLock을 사용하면 Mutex를 깜박 잊고 풀지 않는 경우를 방지할 수 있다.
/// CLock의 생성자 파라메터로 잠그고자 하는 Mutex의 포인터를 넘겨준다.
/////////////////////////////////////////////////////////////////
class CLock {
  public:
	enum LockType_e { READLOCK, WRITELOCK };
	CLock(CLockVar* mutex_p) : lockVar_mp(mutex_p), bLocked_m(false)
	{
		lockVar_mp->lock();
		bLocked_m = true;
	}
	CLock(CRWMutex* rwMutex_p,LockType_e type) : lockVar_mp(rwMutex_p), bLocked_m(false)
	{
		if(type == CLock::READLOCK)
		{
			lockVar_mp->readLock();
		}
		else
		{
			lockVar_mp->writeLock();
		}
		bLocked_m = true;
	}
	////////////////////////////////////////////////////////////////////
	/// @brief 소멸자\n
	/// 생성자에서 lock() 호출되며, 소멸자에서 unlock() 호출된다.
	////////////////////////////////////////////////////////////////////
	~CLock()
	{
		if(true == bLocked_m)
		{
			lockVar_mp->unlock();
		}
	}
	void lock()
	{
		lockVar_mp->lock();
		bLocked_m = true;
	}
	void readLock()
	{
		lockVar_mp->readLock();
		bLocked_m = true;
	}
	void writeLock()
	{
		lockVar_mp->writeLock();
		bLocked_m = true;
	}
	void unlock()
	{
		bLocked_m = false;
		lockVar_mp->unlock();
	}
  private:
	CLock(const CLock& inLock);				// to prohibit copy
	CLock& operator=(const CLock& inLock);	// to prohibit copy
	CLockVar* const lockVar_mp;
	bool bLocked_m;
};	// class CLock
#endif
