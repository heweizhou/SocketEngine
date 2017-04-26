#ifndef _C_THREAD_POOL_
#define _C_THREAD_POOL_

#include <list>
#include <map>
#include "defines.h"
#include "ICMutex.h"
#include "ICSemaphoreNamed.h"
#include "RefBase.h"
#include "IWork.h"

namespace  _MY_NAME_SPACE_
{
    class CThreadPool
    {
    public:
        static _VOID            start();
        static _VOID            start(_INT threadCount);
        static _VOID            stop();
        static _BOOL            getStatus();
        static _VOID            AddWork(IWork* task, _BOOL isImmediately);
        static _VOID            clearWorkQueue();
    private:
        static _PVOID           m_thread_obj;
    };
}

#endif	//_C_THREAD_POOL_
