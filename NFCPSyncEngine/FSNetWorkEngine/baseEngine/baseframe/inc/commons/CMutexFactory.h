//
//  CMutexFactory.h
//  commons
//
//  Created by 周贺伟 on 14/11/26.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#ifndef __commons__CMutexFactory__
#define __commons__CMutexFactory__

#include <stdio.h>
#include "ICMutex.h"

class CMutexFactory
{
public:
    static ICMutex* createMutex();
    static ICMutex* createStaticMutex();
};
#endif /* defined(__commons__CMutexFactory__) */
