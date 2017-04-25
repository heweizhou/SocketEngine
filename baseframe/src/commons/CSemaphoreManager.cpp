//
//  CSemaphoreManager.cpp
//  commons
//
//  Created by 周贺伟 on 14/12/7.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#include "CSemaphoreManager.h"
#include "CLinuxSemaphoreNamed.h"


#define SEM_PAIR(key,value)     std::pair<_string, ICSemaphoreNamed*>( (key), (value))
CSemaphoreManager*   CSemaphoreManager::m_sInstance = _NULL;
ICMutex*             CSemaphoreManager::m_sInstanceMutex = CMutexFactory::createStaticMutex();
CSemaphoreManager*   CSemaphoreManager::getInstance()
{
    scope_lock<ICMutex> lk(m_sInstanceMutex);
    if ( m_sInstance == _NULL )
    {
        m_sInstance = new CSemaphoreManager();
    }

    return m_sInstance;
}

_VOID CSemaphoreManager::destroy()
{
    scope_lock<ICMutex> lk(m_sInstanceMutex);
    SAFE_DELETE( m_sInstance );
}

CSemaphoreManager::CSemaphoreManager()
{
    m_mutex = CMutexFactory::createMutex();
    
}
CSemaphoreManager::~CSemaphoreManager()
{
    m_mutex->lock();
    SemaphoreMap::iterator it = m_mMap.begin();
    for ( ; it != m_mMap.end(); ++it )
    {
        CLinuxSemaphoreNamed* Sem = CAST( CLinuxSemaphoreNamed*, it->second );
        SAFE_RELEASE(Sem);
    }
    m_mutex->unlock();
    
    SAFE_DELETE( m_mutex );
}

ICSemaphoreNamed* CSemaphoreManager::createSemaphore( _string name, _UINT initValue )
{
    scope_lock<ICMutex> lk( m_mutex );

    ICSemaphoreNamed* ret = _NULL;
    SemaphoreMap::iterator findIt = m_mMap.find( name );
    if ( findIt != m_mMap.end() )
    {
        dynamic_cast<CLinuxSemaphoreNamed*>( findIt->second )->incRef();
        ret = findIt->second;
    }
    else
    {
        CLinuxSemaphoreNamed* newSem = new CLinuxSemaphoreNamed();
        newSem->initSem( name, 0, initValue );
        m_mMap.insert( SEM_PAIR( name, newSem) );
        ret = newSem;
    }
    
    return ret;
}

_VOID CSemaphoreManager::releaseSemaphore( _string name )
{
    scope_lock<ICMutex> lk( m_mutex );
    
    SemaphoreMap::iterator findIt = m_mMap.find( name );
    if ( findIt != m_mMap.end() )
    {
        CLinuxSemaphoreNamed* obj = dynamic_cast<CLinuxSemaphoreNamed*>( findIt->second );
        assert(obj);
        if ( obj->getRefCount() > 1 )
        {
            SAFE_RELEASE( obj );
        }
        else
        {
            CLinuxSemaphoreNamed* Sem = CAST( CLinuxSemaphoreNamed*, findIt->second );
            m_mMap.erase( findIt );
            SAFE_RELEASE( Sem );
        }
    }
}

