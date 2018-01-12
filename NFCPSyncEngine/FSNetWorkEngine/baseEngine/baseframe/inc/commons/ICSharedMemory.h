//
//  ICSharedMemory.h
//  commons
//
//  Created by 周贺伟 on 14/12/4.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#ifndef commons_ICSharedMemory_h
#define commons_ICSharedMemory_h
#include "RefBase.h"

class ICSharedMemory:public _MY_NAME_SPACE_::RefBase
{
public:
    virtual _ULONG      getCapacity() = 0;
    virtual _ULONG      getlength() = 0;
    virtual _PVOID      obtainMemoryLock() = 0;
    virtual _VOID       releaseMemoryLock() = 0;
    virtual _BOOL       clone( ICSharedMemory* ref ) = 0;
protected:
    virtual  ~ICSharedMemory(){}
};
#endif
