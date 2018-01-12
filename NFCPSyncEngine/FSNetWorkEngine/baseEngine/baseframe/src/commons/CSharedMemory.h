//
//  CSharedMemory.h
//  commons
//
//  Created by 周贺伟 on 14/11/26.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#ifndef __commons__CSharedMemory__
#define __commons__CSharedMemory__

#include <stdio.h>
#include "defines.h"
#include "ICMutex.h"
#include "ICSharedMemory.h"

class CSharedMemory:public ICSharedMemory
{
public:
    CSharedMemory();
    CSharedMemory( _ULONG memorySize );
    CSharedMemory( CSharedMemory& ref );
    virtual ~CSharedMemory();
    
    _ULONG      getCapacity();
    _ULONG      getlength();
    _PVOID      obtainMemoryLock();
    _VOID       releaseMemoryLock();
    _BOOL       clone( ICSharedMemory* ref );
private:
    ICMutex*    m_mutex;
    _ULONG      m_uiLength;
    _ULONG      m_uiCapacity;
    _PVOID      m_ptr;
};
#endif /* defined(__commons__CSharedMemory__) */
