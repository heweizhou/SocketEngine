//
//  MessageAdapter.cpp
//  MessageCenter
//
//  Created by 周贺伟 on 14/11/23.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#include "MessageAdapter.h"
#include "MessageCenter.h"
#include "CThreadFactory.h"

MessageAdapter::MessageAdapter(_ULONG interval_millisecond, thread_priority pri)
{
    m_sClientName       = "";
    m_interval          = interval_millisecond;
    m_priority          = pri;
    m_messageAvailable  = _NULL;
}

MessageAdapter::~MessageAdapter()
{
    SemaphoreManager->releaseSemaphore( m_sClientName );
}

_VOID MessageAdapter::waitForMessageAvailable()
{
    m_messageAvailable->waitSem();
}

_VOID MessageAdapter::delay()
{
    if (m_interval != 0) {
        CThreadFactory::sleep((_UINT)m_interval);
    }
}

_VOID MessageAdapter::setClientName(_string &name)
{
    m_sClientName = name;
    
}
_string& MessageAdapter::getClientName()
{
    return m_sClientName;
}

_BOOL MessageAdapter::sendMessage( _string destName, MessageType type, _PVOID paramBuffer, _UINT bufferSize )
{
    return CMessageCenter::sendMessage( m_sClientName, destName, type , paramBuffer, bufferSize, _FALSE, _TRUE );
}

_BOOL MessageAdapter::postMessage( _string destName, MessageType type, _PVOID paramBuffer, _UINT bufferSize )
{
    return CMessageCenter::sendMessage( m_sClientName, destName, type , paramBuffer, bufferSize, _FALSE, _FALSE );
}

_BOOL MessageAdapter::peekMessage( _string& srcName, MessageType& type, _PVOID& paramBuffer, _UINT& bufferSize, _TIMESTAMP& time_stamp )
{
    CMessage msg;
    if(CMessageCenter::peekMessage( m_sClientName, &msg ) )
    {
        srcName     = msg.srcClientName;
        type        = msg.type;
        paramBuffer = msg.paramBuffer;
        bufferSize  = msg.bufferSize;
        time_stamp  = msg.time_stamp;
        
        return _TRUE;
    }
    return _FALSE;
}

_BOOL MessageAdapter::getMessage( _string& srcName, MessageType& type, _PVOID& paramBuffer, _UINT& bufferSize, _TIMESTAMP& time_stamp )
{
    CMessage msg;
    if(CMessageCenter::getMessage( m_sClientName, msg ) )
    {
        srcName     = msg.srcClientName;
        type        = msg.type;
        paramBuffer = msg.paramBuffer;
        bufferSize  = msg.bufferSize;
        time_stamp  = msg.time_stamp;
        
        return _TRUE;
    }
    return _FALSE;
}

_BOOL MessageAdapter::broadcastMessage( MessageType type, _PVOID paramBuffer, _UINT bufferSize )
{
    return CMessageCenter::sendMessage( m_sClientName, type , paramBuffer, bufferSize, _FALSE );
}

//global
_BOOL attach2MessageCenter(_string name, pfGetInstance staticFunction)
{
    return CMessageCenter::attach(name, staticFunction);
}

_BOOL detachFromMessageCenter(_string name)
{
    return CMessageCenter::detach(name);
}

_BOOL sendMessage( _string destName, MessageType type, _PVOID paramBuffer, _UINT bufferSize )
{
    _string anymous = "";
    return CMessageCenter::sendMessage( anymous, destName, type , paramBuffer, bufferSize, _FALSE, _TRUE );
}

_BOOL postMessage( _string destName, MessageType type, _PVOID paramBuffer, _UINT bufferSize)
{
    _string anymous = "";
    return CMessageCenter::sendMessage( anymous, destName, type , paramBuffer, bufferSize, _FALSE, _FALSE );
}

_BOOL broadcastMessage( MessageType type, _PVOID paramBuffer, _UINT bufferSize )
{
    _string anymous = "";
    return CMessageCenter::sendMessage( anymous, type , paramBuffer, bufferSize, _FALSE );
}


_BOOL detachAllComponent()
{
    return CMessageCenter::detachAllComps();
}
