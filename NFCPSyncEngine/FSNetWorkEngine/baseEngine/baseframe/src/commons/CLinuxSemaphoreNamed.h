//
//  CLinuxSemaphoreNamed.h
//  commons
//
//  Created by 周贺伟 on 14/11/26.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#ifndef __commons__CLinuxSemaphoreNamed__
#define __commons__CLinuxSemaphoreNamed__

#include <stdio.h>
#include "ICSemaphoreNamed.h"
#include "complierOption.h"
#include "RefBase.h"
#ifdef _LINUX_SYS_
#include<semaphore.h>
class CLinuxSemaphoreNamed:public ICSemaphoreNamed, public _MY_NAME_SPACE_::RefBase
{
public:
    CLinuxSemaphoreNamed();
protected:
    virtual ~CLinuxSemaphoreNamed();
public:
    _INT initSem( _string& name, _INT pshared, _INT value );
public://overide
    _INT waitSem();
    _INT postSem();
    _INT postSem(_INT value);
private:
    _string     m_name;
#ifdef ANDROID_SYSTEM
    sem_t       m_sem;
#else
    sem_t*      m_sem;
#endif
};
#endif //_LINUX_SYS_
#endif /* defined(__commons__CLinuxSemaphoreNamed__) */
