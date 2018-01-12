//
//  CShareMemManager.h
//  commons
//
//  Created by 周贺伟 on 14/11/26.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#ifndef __commons__CShareMemManager__
#define __commons__CShareMemManager__

#include <stdio.h>
#include "defines.h"
#include "ICSharedMemory.h"
#include "ICMutex.h"

#define ShareMemManager (CShareMemManager::getInstance())
typedef MAP<_HANDLE, ICSharedMemory*> SMMap;

class CShareMemManager
{
public:
    static CShareMemManager*    getInstance();
    static _VOID                destroy();
public:
    _HANDLE                     createSharedMemory( _ULONG dataSize );
    _VOID                       destroySharedMemory( _HANDLE hander );
    _VOID                       incObjRef( _HANDLE handler );
    ICSharedMemory*             getObject( _HANDLE handler );
private:
    CShareMemManager();
    ~CShareMemManager();
private:
    static CShareMemManager*    m_sInstance;
    static ICMutex*             m_sInstanceMutex;
private:
    ICMutex*                    m_mutex;
    SMMap                       m_mShareMemoryMap;
};
#endif /* defined(__commons__CShareMemManager__) */
