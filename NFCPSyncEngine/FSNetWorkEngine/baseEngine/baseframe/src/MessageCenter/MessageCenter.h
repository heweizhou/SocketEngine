//
//  MessageCenter.h
//  MessageCenter
//
//  Created by 周贺伟 on 14/11/18.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#ifndef _MESSAGE_CENTER_
#define _MESSAGE_CENTER_

#include <stdlib.h>
#include <stdio.h>
#include <queue>
#include "MessageAdapter.h"
#include "MessageDefines.h"
#include "CSemaphoreManager.h"
#include "ICMutex.h"

#define MAX_MESSAGE_COUNT       100            //待解决数量过小造成死锁的问题，优化结构
#define MSG_ISFULL(semName)     semName+"_FUL"

class CMessage
{
public:
    CMessage();
    CMessage( _string& srcClientName, MessageType type, _PVOID buffer, _UINT size, _BOOL isSync );
    CMessage( CMessage& ref );
    ~CMessage();
    _BOOL equal( CMessage& ref );
    _VOID clone( CMessage& ref );
public:
    MessageType         type;
    _string             semName;
    _string             srcClientName;
    _UINT               bufferSize;
    _PVOID              paramBuffer;
    ICSemaphoreNamed*   syncSemaphore;
    _TIMESTAMP          time_stamp;
};

#define MSG_PAIR( key, value )  std::pair< _string, CClientInfo* >( (key), (value) )

class CMessageCenter
{
private:
    class CClientInfo;
public:
    typedef MAP<_string, CClientInfo*> MsgMap;
    typedef MsgMap::iterator    msgIt;
public:
    static _BOOL attach(_string &name, pfGetInstance pfn);
    static _BOOL detach(_string &name);
    static _BOOL detachAllComps();
    static _BOOL sendMessage( _string &srcClientName, _string &name, MessageType type, _PVOID param, _UINT paramBufferSize, _BOOL isHighPri, _BOOL isSync );
    static _BOOL sendMessage( _string &srcClientName, MessageType type, _PVOID param, _UINT paramBufferSize, _BOOL isHighPri );
    static _INT  getMessage( _string& name, CMessage& message );
    static _BOOL peekMessage( _string& name, _CONST CMessage* message );
private:
    static ICMutex* m_ptrThreadMutex;
    static MsgMap   m_mMessageMap;
};

#endif
