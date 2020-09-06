#ifndef __CLOCVAR_H__
#define __CLOCVAR_H__
class CLockVar
{
  public:
	CLockVar() { }
	virtual ~CLockVar() { }
	virtual void lock()=0;
	virtual void readLock()=0;
	virtual void writeLock()=0;
	virtual void unlock()=0;
  private:
	CLockVar(const CLockVar& inLockVar);
	CLockVar& operator=(const CLockVar& inLockVar);
};
#endif
