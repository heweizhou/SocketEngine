//
//  SingleThreadAdapter.cpp
//  MessageCenter
//
//  Created by 周贺伟 on 15/12/21.
//  Copyright © 2015年 周贺伟. All rights reserved.
//

#include <stdio.h>
#include "SingleThreadAdapter.h"
#include "CThreadFactory.h"
#include "MessageCenter.h"

_PVOID thread_fun( _PVOID arg )
{
    CThreadFactory::detachSelf();
    MessageAdapter* adapter = static_cast<MessageAdapter*>(arg);
    ICSemaphoreNamed* sem   = SemaphoreManager->createSemaphore(adapter->getClientName(), 0);
    while( 1 )
    {
        sem->waitSem();
        
        CMessage msg;
        _INT ret = CMessageCenter::getMessage( adapter->getClientName(), msg );
        //ret > 0 成功， =0 从消息中心取消息失败 < 0 消息中心已经停止
        if( ret >= 0 )
        {
            switch (msg.type) {
                case MESSAGE_COM_EXIT:
                {
                    goto end;
                }
                    break;
                default:
                {
                    adapter->onMessage( msg.srcClientName, msg.type, msg.paramBuffer, msg.bufferSize, msg.time_stamp );
                    if ( msg.syncSemaphore != _NULL )
                    {
                        msg.syncSemaphore->postSem();
                    }
                    adapter->delay();
                }
                    break;
            }
        }else{
            break;
        }
    }
end:
    SemaphoreManager->releaseSemaphore(adapter->getClientName());
    adapter->release();
    return _NULL;
}


_VOID SingleThreadAdapter::startRunLoop()
{
#if DEBUG
    if (m_sClientName.length() <= 0 ) {
        assert(0);
    }
#endif
    m_messageAvailable  = SemaphoreManager->createSemaphore(m_sClientName, 0);
    CThreadFactory::createThreadWithName(m_sClientName.c_str(), &m_pid, thread_fun, this, m_priority);
}

