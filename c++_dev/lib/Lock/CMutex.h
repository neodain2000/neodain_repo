#ifndef __CMUTEX_H__
#define __CMUTEX_H__
#include "CLockVar.h"
#include "CMutexImp.h"
#include <stdint.h>
class CMutex : public CLockVar
{
 public:
	CMutex() { }
	~CMutex() { }
	void lock() { imp_m.lock(); }
	void readLock() { lock(); }
	void writeLock() { lock(); }
	void unlock() { imp_m.unlock(); }
	CMutexImp& getImp() { return imp_m; }
 private:
	CMutex(const CMutex& inMutex);
	CMutex& operator=(const CMutex& inMutex);
	CMutexImp imp_m;
};

#endif
