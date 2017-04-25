//
//  CHanderManager.cpp
//  commons
//
//  Created by 周贺伟 on 14/11/26.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#include "CHanderManager.h"
#include "CMutexFactory.h"

#define INC_STEP        5000

CHanderManager*  CHanderManager::m_sInstance = _NULL;
ICMutex*         CHanderManager::m_sInstanceMutex = CMutexFactory::createStaticMutex();
CHanderManager*  CHanderManager::getInstance()
{
    scope_lock<ICMutex> lk( m_sInstanceMutex );
    
    if ( m_sInstance == _NULL )
    {
        m_sInstance = new CHanderManager();
    }
    
    return  m_sInstance;
}

CHanderManager::CHanderManager()
{
    m_handersCount = 0;
    m_mutex = CMutexFactory::createMutex();
}
CHanderManager::~CHanderManager()
{
    SAFE_DELETE( m_mutex );
}

_VOID CHanderManager::destroy()
{
    scope_lock<ICMutex> lk( m_sInstanceMutex );
    SAFE_DELETE( m_sInstance );
}

_HANDLE CHanderManager::getFreeHander()
{
    scope_lock<ICMutex> lk( m_mutex );
    
    if( m_qUnusedHanders.size() <= 0 )
    {
        _INT startIndex = m_handersCount + 1;
        
        for ( _INT i = startIndex; i < startIndex + INC_STEP; ++i )
        {
            m_qUnusedHanders.push( i );
        }
    }
    
    _HANDLE ret = m_qUnusedHanders.front();
    m_qUnusedHanders.pop();
    m_handersCount++;
    
    return  ret;
}

_VOID CHanderManager::destroyHander( _HANDLE hander )
{
    scope_lock<ICMutex> lk( m_mutex );
    m_qUnusedHanders.push( hander );
    m_handersCount--;
}