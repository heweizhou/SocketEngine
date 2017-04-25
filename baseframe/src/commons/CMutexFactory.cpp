//
//  CMutexFactory.cpp
//  commons
//
//  Created by 周贺伟 on 14/11/26.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#include "CMutexFactory.h"
#include "CLinuxMutex.h"
#include "complierOption.h"

ICMutex* CMutexFactory::createMutex()
{
#ifdef _LINUX_SYS_
    return new CLinuxMutex( CLinuxMutex::MKIND_DYNAMIC );
#endif
}

ICMutex* CMutexFactory::createStaticMutex()
{
#ifdef _LINUX_SYS_
    return new CLinuxMutex( CLinuxMutex::MKIND_STATIC );
#endif
}
