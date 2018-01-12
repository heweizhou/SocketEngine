//
//  MessageAdapter.h
//  MessageCenter
//
//  Created by 周贺伟 on 14/11/23.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#ifndef __MessageCenter__MessageAdapter__
#define __MessageCenter__MessageAdapter__

#include "RefBase.h"
#include "MessageDefines.h"
#include "CSemaphoreManager.h"

class MessageAdapter:virtual public _MY_NAME_SPACE_::RefBase
{
public:
    _BOOL           broadcastMessage( MessageType type, _PVOID paramBuffer, _UINT bufferSize );
    _BOOL           sendMessage( _string destName, MessageType type, _PVOID paramBuffer, _UINT bufferSize );
    _BOOL           postMessage( _string destName, MessageType type, _PVOID paramBuffer, _UINT bufferSize );
    _BOOL           getMessage( _string& srcName, MessageType& type, _PVOID& paramBuffer, _UINT& bufferSize, _TIMESTAMP& time_stamp );
    _BOOL           peekMessage( _string& srcName, MessageType& type, _PVOID& paramBuffer, _UINT& bufferSize, _TIMESTAMP& time_stamp );
    
public://overide
    virtual _VOID   startRunLoop() = 0;
    virtual _VOID   onMessage( _string& srcName, MessageType type, _PVOID paramBuffer, _UINT bufferSize, _TIMESTAMP time_stamp ) = 0;
public:
    MessageAdapter(_ULONG interval_millisecond = 0, thread_priority pri = NORMAL );//名字太长引起信号量创建失败，最好在20个字符以下
    virtual ~MessageAdapter();
public:
    _VOID       setClientName(_string &name);
    _string&    getClientName();
    _VOID       waitForMessageAvailable();
    _VOID       delay();
protected:
    ICSemaphoreNamed*       m_messageAvailable;
    _string                 m_sClientName;
    _TID                    m_pid;
    _ULONG                  m_interval;
    thread_priority         m_priority;
};

typedef  MessageAdapter* (*pfGetInstance)();

extern "C"
{
    _BOOL attach2MessageCenter(_string name, pfGetInstance staticFunction);
    _BOOL detachFromMessageCenter(_string name);
    _BOOL detachAllComponent();
    _BOOL sendMessage( _string destName, MessageType type, _PVOID paramBuffer, _UINT bufferSize);
    _BOOL postMessage( _string destName, MessageType type, _PVOID paramBuffer, _UINT bufferSize);
    _BOOL broadcastMessage( MessageType type, _PVOID paramBuffer, _UINT bufferSize );
}
#endif /* defined(__MessageCenter__MessageAdapter__) */
