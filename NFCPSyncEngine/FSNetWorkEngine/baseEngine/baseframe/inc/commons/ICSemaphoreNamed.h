//
//  ISemaphoreNamed.h
//  commons
//
//  Created by 周贺伟 on 14/11/26.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#ifndef commons_ICSemaphoreNamed_h
#define commons_ICSemaphoreNamed_h

#include "defines.h" 

class ICSemaphoreNamed
{
public:
    virtual _INT waitSem() = 0;
    virtual _INT postSem() = 0;
    virtual _INT postSem(_INT value) = 0;
    virtual ~ICSemaphoreNamed(){}
};

#endif
