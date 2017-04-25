//
//  SharedThreadAdapter.h
//  MessageCenter
//
//  Created by 周贺伟 on 15/12/21.
//  Copyright © 2015年 周贺伟. All rights reserved.
//

#ifndef SharedThreadAdapter_h
#define SharedThreadAdapter_h

#include "MessageDefines.h"
#include "MessageAdapter.h"
#include "IWork.h"

class SharedThreadAdapter:public MessageAdapter, public IWork
{
public:
    SharedThreadAdapter();
    virtual ~SharedThreadAdapter();
public://overide MessageAdapter
    virtual _VOID startRunLoop();
public://overide IWork
    virtual _VOID  doWork();
    virtual _VOID  cancelWork();
private:
    ICMutex*  m_mutex;
};

#endif /* SharedThreadAdapter_h */
