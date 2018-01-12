//
//  SharedThreadAdapter.cpp
//  MessageCenter
//
//  Created by 周贺伟 on 15/12/21.
//  Copyright © 2015年 周贺伟. All rights reserved.
//

#include <stdio.h>
#include "SharedThreadAdapter.h"
#include "CThreadFactory.h"
#include "MessageCenter.h"


SharedThreadAdapter::SharedThreadAdapter(){
    m_mutex = CMutexFactory::createMutex();
}

SharedThreadAdapter::~SharedThreadAdapter(){
    if (m_messageAvailable) {
        SemaphoreManager->releaseSemaphore(m_sClientName);
    }
    SAFE_DELETE(m_mutex);
}

_VOID SharedThreadAdapter::doWork()
{
    scope_lock<ICMutex> lk(m_mutex);
    
    m_messageAvailable->waitSem();
    CMessage msg;
    _INT ret = CMessageCenter::getMessage( getClientName(), msg );
    //ret > 0 成功， =0 从消息中心取消息失败 < 0 消息中心已经停止
    if( ret >= 0 )
    {
        switch (msg.type) {
            case MESSAGE_COM_EXIT:{
                release();
            }
                break;
            default:{
                onMessage( msg.srcClientName, msg.type, msg.paramBuffer, msg.bufferSize, msg.time_stamp );
                if ( msg.syncSemaphore != _NULL ){
                    msg.syncSemaphore->postSem();
                }
                delay();
            }
                break;
        }
    }
}

_VOID  SharedThreadAdapter::cancelWork(){
    
}

_VOID SharedThreadAdapter::startRunLoop(){
    m_messageAvailable  = SemaphoreManager->createSemaphore(m_sClientName, 0);
}
