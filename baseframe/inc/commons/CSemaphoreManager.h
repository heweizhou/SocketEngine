//
//  CSemaphoreManager.h
//  commons
//
//  Created by 周贺伟 on 14/12/7.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#ifndef __commons__CSemaphoreManager__
#define __commons__CSemaphoreManager__

#include <stdio.h>
#include "defines.h"
#include "ICSemaphoreNamed.h"
#include "CMutexFactory.h"

#define SemaphoreManager    (CSemaphoreManager::getInstance())

class CSemaphoreManager
{
public:
    typedef MAP< _string, ICSemaphoreNamed*> SemaphoreMap;
public:
    static CSemaphoreManager*   getInstance();
    static _VOID                destroy();
private:
    static CSemaphoreManager*   m_sInstance;
    static ICMutex*             m_sInstanceMutex;
public:
    ICSemaphoreNamed*           createSemaphore( _string name, _UINT initValue );
    _VOID                       releaseSemaphore( _string name );
private:
    CSemaphoreManager();
    ~CSemaphoreManager();
private:
    ICMutex*                m_mutex;
    SemaphoreMap            m_mMap;
};
#endif /* defined(__commons__CSemaphoreManager__) */
