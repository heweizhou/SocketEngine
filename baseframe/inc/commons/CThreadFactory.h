//
//  CThreadFactory.h
//  commons
//
//  Created by 周贺伟 on 14/11/26.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#ifndef __commons__CThreadFactory__
#define __commons__CThreadFactory__

#include <stdio.h>
#include "defines.h"

typedef _PVOID (*ptrThreadProc)(_PVOID);

class CThreadFactory
{
public:
    static _INT createThreadWithName(_CONST _CHAR* thread_name, _TID* pid, ptrThreadProc proc, _PVOID param, thread_priority pri);
    static _INT createThread( _TID* pid, ptrThreadProc proc, _PVOID param, thread_priority pri );
    static _INT sleep( _UINT millisecond );
    static _INT waitForThread( _TID pid );
    static _TID getThreadPID();
    static _INT detachSelf();
};
#endif /* defined(__commons__CThreadFactory__) */
