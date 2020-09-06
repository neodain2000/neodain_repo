#ifndef __CRWLOCK_H__
#define __CRWLOCK_H__
#include "CLockVar.h"
#include "CRWMutexImp.h"

class CRWMutexImp;
/////////////////////////////////////////////////////////////////
/// @class CRWMutex
/// @par
/// Read/Write Lock을 처리하는 Class이다.\n
/// CRWMutex 객체를 생성하고 read lock을 위해 readLock(), write lock을 위해 writeLock()을 호출하여 사용한다.
/// readLock은 다수의 Thread가 critical section에 접근할 수 있으며 writeLock은 하나의 Thread만이 critical section 에 접근할 수 있다.
/// 따라서 read 작업이  많은 경우 상호 locking 없이 수행 가능한 반면 다수의 read로 인하여 write 가 늦추어지는 단점이 있다.
/////////////////////////////////////////////////////////////////
class CRWMutex : public CLockVar
{
  public:
	CRWMutex(){}
	~CRWMutex(){}
	void lock() { writeLock(); };
	void readLock() { imp_m.readLock(); }
	void writeLock() { imp_m.writeLock(); }
	void unlock() { imp_m.unlock(); }
  private:
	CRWMutex(const CRWMutex& inRWMutex);
	CRWMutex& operator=(const CRWMutex& inRWMutex);
	CRWMutexImp imp_m;
};	// class CRWMutex
#endif
