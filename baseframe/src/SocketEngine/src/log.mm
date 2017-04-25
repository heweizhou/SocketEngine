//
//  log.cpp
//  SocketEngine
//
//  Created by 周贺伟 on 16/3/15.
//  Copyright © 2016年 fxiaoke. All rights reserved.
//

#ifdef MY_DEBUG

#include "log.h"
#include "FsDDLog.h"

void _log(const char* msg)
{
    NSString* aString = [NSString stringWithUTF8String:msg];
    [FsDDLog FsLogInfo:aString];
}

#endif
