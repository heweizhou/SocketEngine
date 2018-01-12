//
//  SingleThreadAdatapter.h
//  commons
//
//  Created by 周贺伟 on 15/12/21.
//  Copyright © 2015年 周贺伟. All rights reserved.
//

#ifndef SingleThreadAdapter_h
#define SingleThreadAdapter_h

#include "MessageDefines.h"
#include "MessageAdapter.h"
#include "IWork.h"

class SingleThreadAdapter:public MessageAdapter
{
public://overide
    virtual _VOID startRunLoop();
};

#endif /* SingleThreadAdatapter_h */
