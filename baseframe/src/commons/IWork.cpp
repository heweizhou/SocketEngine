//
//  IWork.cpp
//  commons
//
//  Created by 周贺伟 on 2017/3/13.
//  Copyright © 2017年 周贺伟. All rights reserved.
//

#include <stdio.h>
#include "IWork.h"
#include "CThreadPool.h"

_VOID IWork::add2Pool(_BOOL isHighPri)
{
    this->incRef();
    _MY_NAME_SPACE_::CThreadPool::AddWork(this, isHighPri);
}

_VOID IWork::add2PoolWithAutoRelease(_BOOL isHighPri)
{
    _MY_NAME_SPACE_::CThreadPool::AddWork(this, isHighPri);
}
