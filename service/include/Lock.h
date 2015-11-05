/** 
*
*  @author		Seongho Baek
*  @date		2014.08.12
*/

#ifndef __LOCK_H
#define __LOCK_H

#include "Types.h"

#define LOCK(x) for (Lock __l = x;!__l;__l++)
#define TRYLOCK(x) if (Lock::tryLockMutex(x) == 0)
#define UNLOCK(x) Lock::unlockMutex(x)
class Lock
{
	public:
    	Lock(Mutex_t m): mutex(m), b(FALSE)
		{
    		pthread_mutex_lock(mutex);
		}

    	~Lock()                     { pthread_mutex_unlock(this->mutex); }
    	operator bool()             { return b; }
    	void operator++(int)		{ b = true; }			
    	void operator++()			{ b = true; }

    	static Mutex_t createMutex();
    	static Mutex_t createMutex(bool startLock);
    	static int deleteMutex(Mutex_t mutex);
    	static int unlockMutex(Mutex_t mutex);
    	static int tryLockMutex(Mutex_t mutex)
    	{
    		return pthread_mutex_trylock(mutex);
    	}

	private:
    	Mutex_t   mutex;
    	bool 	b;
};

#endif
