//
//  CBundleManager.cpp
//  commons
//
//  Created by 周贺伟 on 14/11/27.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#include "CBundleManager.h"
#include "CHanderManager.h"
typedef MAP<_HANDLE, CBundle* >::iterator BundleMMapIt;

#define PAIR( HANDLER, SM ) std::pair<_HANDLE, CBundle*>( (HANDLER), (SM) )

CBundleManager*    CBundleManager::m_sInstance = _NULL;
ICMutex*           CBundleManager::m_sInstanceMutex = CMutexFactory::createStaticMutex();
CBundleManager* CBundleManager::getInstance()
{
    scope_lock<ICMutex> lk( m_sInstanceMutex );
    if ( m_sInstance == _NULL )
    {
        m_sInstance = new CBundleManager();
    }
    return m_sInstance;
}

_VOID CBundleManager::destroy()
{
    scope_lock<ICMutex> lk( m_sInstanceMutex );
    SAFE_DELETE( m_sInstance );
}

CBundleManager::CBundleManager()
{
    m_mutex = CMutexFactory::createMutex();
}

CBundleManager::~CBundleManager()
{
    m_mutex->lock();
    BundleMMapIt findit = m_mCBundleManagerMap.begin();
    
    for ( ; findit != m_mCBundleManagerMap.end(); findit++ )
    {
        SAFE_DELETE( findit->second );
    }
    m_mutex->unlock();
    SAFE_DELETE( m_mutex );
}

CBundle* CBundleManager::getObject( _HANDLE handler )
{
    scope_lock<ICMutex> lk( m_mutex );
    
    CBundle* ret = _NULL;
    BundleMMapIt findit = m_mCBundleManagerMap.find( handler );
    
    if( findit != m_mCBundleManagerMap.end() )
    {
        ret = findit->second;
    }
    return ret;
}

_HANDLE CBundleManager::createBundle()
{
    scope_lock<ICMutex> lk( m_mutex );
    _HANDLE hander = CHanderManager::getInstance()->getFreeHander();
    CBundle* bundle = new CBundle();
    m_mCBundleManagerMap.insert( PAIR( hander, bundle ) );
    return hander;
}

_VOID CBundleManager::destroyBundle( _HANDLE handler )
{
    scope_lock<ICMutex> lk( m_mutex );
    BundleMMapIt findit = m_mCBundleManagerMap.find( handler );
    
    if( findit != m_mCBundleManagerMap.end() )
    {
        SAFE_DELETE( findit->second );
        CHanderManager::getInstance()->destroyHander( handler );
        m_mCBundleManagerMap.erase( findit );
    }
}
