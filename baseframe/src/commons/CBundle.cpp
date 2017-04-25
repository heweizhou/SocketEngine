//
//  CBundle.cpp
//  commons
//
//  Created by 周贺伟 on 14/11/27.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#include "CBundle.h"

typedef MAP< _string, BundleValue* >::iterator BundleMapIt;

#define BUNDLE_PAIR( key ,value )   std::pair< _string, BundleValue* >( (key), (value) )

CBundle::CBundle()
{
    m_mutex = CMutexFactory::createMutex();
}

CBundle::~CBundle()
{
    m_mutex->lock();
    BundleMapIt it = m_mBundle.begin();
    for ( ; it != m_mBundle.end(); ++it )
    {
        SAFE_DELETE( it->second );
    }
    m_mutex->unlock();
    SAFE_DELETE( m_mutex );
}

_VOID   CBundle::setData( _string key, _PVOID buffer, _ULONG size )
{
    scope_lock<ICMutex> lk(m_mutex);
    
    BundleMapIt findIt = m_mBundle.find( key );
    if ( findIt != m_mBundle.end() )
    {
        BundleValue* bv = findIt->second;
        bv->setData( buffer, size );
        return ;
    }
    
    BundleValue* newValue = new BundleValue( buffer, size );
    m_mBundle.insert( BUNDLE_PAIR( key, newValue ) );
}

_PVOID  CBundle::getData( _string key, _ULONG& size )
{

    BundleMapIt findIt = m_mBundle.find( key );
    if ( findIt != m_mBundle.end() )
    {
        size = findIt->second->bufferSize;
        return findIt->second->buffer;
    }
    
    size = 0;
    return _NULL;
}

_VOID CBundle::lock()
{
    m_mutex->lock();
}

_VOID CBundle::unlock()
{
    m_mutex->unlock();
}

