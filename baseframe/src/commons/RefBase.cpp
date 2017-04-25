//
//  RefBase.cpp
//  commons
//
//  Created by 周贺伟 on 14/12/3.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#include <stdio.h>
#include "RefBase.h"
#include "CMutexFactory.h"

namespace _MY_NAME_SPACE_ {
    RefBase::RefBase():m__count( 1 ),m__isReleaseing(_FALSE)
    {
        m__mutex = CMutexFactory::createMutex();
    }
    
    RefBase::RefBase( _CONST RefBase& ref )
    {
        m__mutex = CMutexFactory::createMutex();
        m__count = 1;
    }
    
    RefBase::~RefBase(){
    }
    
    _BOOL RefBase::incRef()
    {
        scope_lock<ICMutex> lk( m__mutex );
        if (m__isReleaseing) {
            assert(0);
            return _FALSE;
        }
        m__count++;
        
        return _TRUE;
    }
    
    _UINT RefBase::getRefCount()
    {
        scope_lock<ICMutex> lk( m__mutex );
        if (m__isReleaseing) {
            assert(0);
            return 0;
        }
        return m__count;
    }
    
    _VOID RefBase::release()
    {
        m__mutex->lock();
        
        if (m__isReleaseing) {
            return ;
        }
        
        m__count--;
        
        if( m__count == 0 ){
            m__isReleaseing = _TRUE;
        }
        
        m__mutex->unlock();
        
        if (m__isReleaseing) {
            SAFE_DELETE( m__mutex );
            delete  this;
        }
    }
}
