//
//  CLinuxMutex.h
//  commons
//
//  Created by 周贺伟 on 14/11/25.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#ifndef __commons__CLinuxMutex__
#define __commons__CLinuxMutex__

#include "ICMutex.h"
#include "complierOption.h"

#ifdef _LINUX_SYS_
#include <pthread.h>

class CLinuxMutex:public ICMutex
{
public:
    enum MutexKind
    {
        MKIND_STATIC,
        MKIND_DYNAMIC
    };
public:
    CLinuxMutex( MutexKind kind );
    virtual ~CLinuxMutex();
public://overide
//    virtual _INT  initMutex();
    virtual _INT  lock();
    virtual _INT  tryLock();
    virtual _INT  unlock();
private:
    MutexKind           m_kind;
    pthread_mutex_t     m_mutex;
};
#endif //_LINUX_SYS_

#endif /* defined(__commons__CLinuxMutex__) */
