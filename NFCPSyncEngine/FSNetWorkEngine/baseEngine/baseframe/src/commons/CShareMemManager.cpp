//
//  CShareMemManager.cpp
//  commons
//
//  Created by 周贺伟 on 14/11/26.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#include "CShareMemManager.h"
#include "CHanderManager.h"
#include "CMutexFactory.h"
#include "CSharedMemory.h"

typedef MAP<_HANDLE, ICSharedMemory* >::iterator SMMapIt;

#define PAIR( HANDLER, SM ) std::pair<_HANDLE, ICSharedMemory*>( (HANDLER), (SM) )

CShareMemManager*    CShareMemManager::m_sInstance = _NULL;
ICMutex*             CShareMemManager::m_sInstanceMutex = CMutexFactory::createStaticMutex();
CShareMemManager* CShareMemManager::getInstance()
{
    scope_lock<ICMutex> lk(m_sInstanceMutex);
    if ( m_sInstance == _NULL )
    {
        m_sInstance = new CShareMemManager();
    }

    return m_sInstance;
}

_VOID CShareMemManager::destroy()
{
    scope_lock<ICMutex> lk(m_sInstanceMutex);
    SAFE_DELETE( m_sInstance );
}

CShareMemManager::CShareMemManager()
{
    m_mutex = CMutexFactory::createMutex();
}

CShareMemManager::~CShareMemManager()
{
    m_mutex->lock();
    SMMapIt findit = m_mShareMemoryMap.begin();
    
    for ( ; findit != m_mShareMemoryMap.end(); findit++ )
    {
        findit->second->release();
    }
    m_mutex->unlock();
    SAFE_DELETE( m_mutex );
}

ICSharedMemory* CShareMemManager::getObject( _HANDLE handler )
{
    scope_lock<ICMutex> lk( m_mutex );
    
    ICSharedMemory* ret = _NULL;
    SMMapIt findit = m_mShareMemoryMap.find( handler );
    
    if( findit != m_mShareMemoryMap.end() )
    {
        ret = findit->second;
    }

    return ret;
}

_HANDLE CShareMemManager::createSharedMemory( _ULONG dataSize )
{
    scope_lock<ICMutex> lk( m_mutex );
    _HANDLE hander = CHanderManager::getInstance()->getFreeHander();
    ICSharedMemory* sm = new CSharedMemory( dataSize );
    m_mShareMemoryMap.insert( PAIR( hander, sm ) );
    return hander;
}

_VOID CShareMemManager::incObjRef( _HANDLE handler )
{
    scope_lock<ICMutex> lk( m_mutex );
    SMMapIt findit = m_mShareMemoryMap.find( handler );
    
    if( findit != m_mShareMemoryMap.end() )
    {
        findit->second->incRef();
    }
}

_VOID CShareMemManager::destroySharedMemory( _HANDLE handler )
{
    scope_lock<ICMutex> lk( m_mutex );
    SMMapIt findit = m_mShareMemoryMap.find( handler );
    
    if( findit != m_mShareMemoryMap.end() )
    {
        if ( findit->second->getRefCount() < 2 )
        {
            SAFE_RELEASE( findit->second );
            CHanderManager::getInstance()->destroyHander( handler );
            m_mShareMemoryMap.erase( findit );
            return;
        }
        SAFE_RELEASE( findit->second );
    }
}

