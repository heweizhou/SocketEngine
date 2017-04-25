//
//  CLinuxTime.h
//  commons
//
//  Created by 周贺伟 on 15/1/17.
//  Copyright (c) 2015年 周贺伟. All rights reserved.
//

#ifndef __commons__CLinuxTime__
#define __commons__CLinuxTime__

#include <stdio.h>
#include "defines.h"

#define _systemtime CLinuxTime::getInstance()
class CLinuxTime
{
public:
    static CLinuxTime*          getInstance();
    static _VOID                destroy();
private:
    ~CLinuxTime();
    CLinuxTime();
public:
    _TIMESTAMP                  getSecondFrom1970();
    _TIMESTAMP                  getMSecondFrom1970();
    _TIMESTAMP                  getMicrosecondFrom1970();
private:
    static CLinuxTime*          m_sInstance;
};
#endif /* defined(__commons__CLinuxTime__) */
