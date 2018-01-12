//
//  IWork.h
//  commons
//
//  Created by 周贺伟 on 14/12/18.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#ifndef commons_IWork_h
#define commons_IWork_h

#include "RefBase.h"

class IWork:virtual public ::_MY_NAME_SPACE_::RefBase
{
protected:
    virtual ~IWork(){};
public:
    virtual _VOID doWork() = 0;
    virtual _VOID cancelWork() = 0;
public:
    _VOID add2Pool(_BOOL isHighPri);
    _VOID add2PoolWithAutoRelease(_BOOL isHighPri);
};

#endif
