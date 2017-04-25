//
//  CLinuxMutex.cpp
//  commons
//
//  Created by 周贺伟 on 14/11/25.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#include "CLinuxMutex.h"

CLinuxMutex::CLinuxMutex( MutexKind kind )
{
    switch ( kind )
    {
        case MKIND_DYNAMIC:
            pthread_mutex_init( &m_mutex, _NULL );
            break;
        case MKIND_STATIC:
//            pthread_mutex_init( &m_mutex, _NULL );
            m_mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
        default:
            break;
    }
    
    m_kind = kind;
}
CLinuxMutex::~CLinuxMutex()
{
    switch ( m_kind )
    {
        case MKIND_DYNAMIC:
        {
            lock();
            unlock();
            pthread_mutex_destroy( &m_mutex );
        }
            break;
        default:
            break;
    }

}

_INT  CLinuxMutex::lock()
{
    return pthread_mutex_lock( &m_mutex );
}
_INT  CLinuxMutex::unlock()
{
    return pthread_mutex_unlock( &m_mutex );
}

_INT CLinuxMutex::tryLock()
{
    return pthread_mutex_trylock( &m_mutex );
}