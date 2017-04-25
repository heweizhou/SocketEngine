//
//  CLinuxSemaphoreNamed.cpp
//  commons
//
//  Created by 周贺伟 on 14/11/26.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#include "CLinuxSemaphoreNamed.h"
#include "CLinuxTime.h"
#include "ToolKit.hpp"
#ifdef _LINUX_SYS_

#include <fcntl.h>

CLinuxSemaphoreNamed::CLinuxSemaphoreNamed()
{
    
}

CLinuxSemaphoreNamed::~CLinuxSemaphoreNamed()
{
    int ret = 0;
#ifdef ANDROID_SYSTEM
    ret = sem_destroy( &m_sem );
    assert(!ret);
#else
    ret = sem_close( m_sem );
    assert(!ret);
    
    ret = sem_unlink( m_name.c_str() );
    assert(!ret);
#endif
}

_INT CLinuxSemaphoreNamed::initSem( _string& name, _INT pshared, _INT value )
{
#ifdef ANDROID_SYSTEM
    return sem_init( &m_sem, pshared, value);
#else
    
    _CHAR all_name[255] = {0};
    
    _TIMESTAMP t = CLinuxTime::getInstance()->getMicrosecondFrom1970();
    sprintf(all_name, "%ld_%s", t, name.c_str());
    
    _string semName = BKDRHash(all_name);
    
    m_sem = sem_open( semName.c_str(), O_CREAT, 0644, value);
    
    if (m_sem == SEM_FAILED)
    {
        assert(0);
        printf("sem_open failed!\n");
        return -1;
    }
    m_name = semName;
    return 0;

#endif
}

_INT CLinuxSemaphoreNamed::waitSem()
{
#ifdef ANDROID_SYSTEM
    return sem_wait( &m_sem );
#else
    return sem_wait( m_sem );
#endif
}

_INT CLinuxSemaphoreNamed::postSem()
{
#ifdef ANDROID_SYSTEM
    return sem_post( &m_sem );
#else
    return sem_post( m_sem );
#endif
}

_INT CLinuxSemaphoreNamed::postSem(_INT value)
{
    for (_INT i = 0; i < value; ++i) {
#ifdef ANDROID_SYSTEM
        sem_post( &m_sem );
#else
        sem_post( m_sem );
#endif
    }
    return 0;
}

//_INT CLinuxSemaphoreNamed::closeSem()
//{
//#ifndef ANDROID_SYSTEM
//    
//#endif
//}
//_INT CLinuxSemaphoreNamed::destroy()
//{
//}

#endif //_LINUX_SYS_
