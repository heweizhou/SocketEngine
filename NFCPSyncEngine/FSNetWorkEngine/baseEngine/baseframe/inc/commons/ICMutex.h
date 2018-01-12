#ifndef _C_ICMUTEX_
#define _C_ICMUTEX_

#include "defines.h"
#include "scope_lock.hpp"

class ICMutex
{
public:
//	virtual _INT  initMutex() = 0;
	virtual _INT  lock() = 0;
    virtual _INT  tryLock() = 0;
	virtual _INT  unlock() = 0;
    virtual ~ICMutex(){}
};



#endif //_C_ICMUTEX_

