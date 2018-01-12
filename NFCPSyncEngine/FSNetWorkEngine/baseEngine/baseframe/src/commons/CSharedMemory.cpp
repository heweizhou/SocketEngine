//
//  CSharedMemory.cpp
//  commons
//
//  Created by 周贺伟 on 14/11/26.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#include "CSharedMemory.h"
#include "CMutexFactory.h"
#include <stdio.h>
#include <stdlib.h>

CSharedMemory::CSharedMemory():m_ptr( _NULL ),m_uiLength( 0 ), m_uiCapacity(0)
{
    m_mutex = CMutexFactory::createMutex();
}

CSharedMemory::CSharedMemory( _ULONG memorySize )
{
    m_mutex = CMutexFactory::createMutex();
    
    m_uiLength      = memorySize;
    m_uiCapacity    = memorySize;
    m_ptr   = (_PVOID)malloc( memorySize );
    memset( m_ptr, 0, memorySize );
}
CSharedMemory::CSharedMemory( CSharedMemory& ref )
{
 
    m_mutex = CMutexFactory::createMutex();
    
    _PVOID refPtr = ref.obtainMemoryLock();
    m_ptr = (_PVOID)realloc( m_ptr, ref.m_uiLength );
    if( m_ptr == _NULL )
    {
        goto end;
    }
    m_uiCapacity    = ref.m_uiLength;
    m_uiLength      = ref.m_uiLength;
    memcpy( m_ptr, refPtr, ref.m_uiLength );
    
end:
    ref.releaseMemoryLock();
    return;
}

CSharedMemory::~CSharedMemory()
{
    m_mutex->lock();
    SAFE_FREE( m_ptr );
    m_mutex->unlock();
    
    SAFE_DELETE( m_mutex );
    
}

_ULONG CSharedMemory::getCapacity()
{
    return m_uiCapacity;
}

_ULONG  CSharedMemory::getlength()
{
    return m_uiLength;
}

_PVOID CSharedMemory::obtainMemoryLock()
{
    m_mutex->lock();
    return m_ptr;
}

_VOID CSharedMemory::releaseMemoryLock()
{
    m_mutex->unlock();
}

_BOOL CSharedMemory::clone( ICSharedMemory* ref )
{
    if ( ref == _NULL )
    {
        return _FALSE;
    }
    
    CSharedMemory* ptr = static_cast<CSharedMemory*>(ref);
    _PVOID refPtr = ptr->obtainMemoryLock();
 
    _BOOL  ret = _TRUE;
    if ( m_uiCapacity < ptr->m_uiLength )
    {
        _PVOID tmp = (_PVOID)realloc( m_ptr, ptr->m_uiLength );
        if( tmp == _NULL )
        {
            ret = _FALSE;
            goto end;
        }
        m_ptr           = tmp;
        m_uiCapacity    = ptr->m_uiLength;
        m_uiLength      = ptr->m_uiLength;
        
    }
    else
    {
        m_uiLength = ptr->m_uiLength;
    }
    
    memcpy( m_ptr, refPtr, ptr->m_uiLength );
    
end:
    ptr->releaseMemoryLock();
    return  ret;
}