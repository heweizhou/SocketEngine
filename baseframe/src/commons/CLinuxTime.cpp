//
//  CLinuxTime.cpp
//  commons
//
//  Created by 周贺伟 on 15/1/17.
//  Copyright (c) 2015年 周贺伟. All rights reserved.
//

#include "CLinuxTime.h"
#include <sys/time.h>
#include <unistd.h>

CLinuxTime* CLinuxTime::m_sInstance = _NULL;

CLinuxTime* CLinuxTime::getInstance()
{
    if ( m_sInstance == _NULL )
    {
        m_sInstance = new CLinuxTime();
    }
    return  m_sInstance;
}

_VOID CLinuxTime::destroy()
{
    SAFE_DELETE( m_sInstance );
}

CLinuxTime::~CLinuxTime()
{
    
}
CLinuxTime::CLinuxTime()
{
    
}
_TIMESTAMP  CLinuxTime::getSecondFrom1970()
{
    struct timeval tv;
    gettimeofday( &tv, _NULL );
    return tv.tv_sec;
}
_TIMESTAMP  CLinuxTime::getMSecondFrom1970()
{
    struct timeval tv;
    gettimeofday( &tv, _NULL );
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

_TIMESTAMP  CLinuxTime::getMicrosecondFrom1970()
{
    struct timeval tv;
    gettimeofday( &tv, _NULL );
    return tv.tv_sec * 1000 * 1000 + tv.tv_usec;
}
