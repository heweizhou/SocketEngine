//
//  MessageCenter.m
//  MessageCenter
//
//  Created by 周贺伟 on 14/11/18.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#include "MessageCenter.h"
#include "CMutexFactory.h"
#include "CThreadFactory.h"
#include "CLinuxTime.h"
#include "SharedThreadAdapter.h"
#include "CThreadPool.h"
#include "log.h"

//message
CMessage::CMessage():type(MESSAGE_NONE),bufferSize(0),paramBuffer(_NULL),srcClientName(""),syncSemaphore(_NULL)
{
    time_stamp = _systemtime->getMSecondFrom1970();
}

CMessage::CMessage( _string& srcClientName, MessageType type, _PVOID buffer, _UINT size, _BOOL isSync )
{
    this->type          = type;
    this->srcClientName = srcClientName;
    this->bufferSize    = size;
    this->time_stamp    = _systemtime->getMicrosecondFrom1970();
    
    if( isSync )
    {
        _CHAR tmp[100] = {0};
        sprintf( tmp, "%ld%s", time_stamp, "_CB" );
        semName = tmp;  //名字太长引起创建信号量失败，省略组件名称
        syncSemaphore = SemaphoreManager->createSemaphore( this->semName, 0 );
        assert( syncSemaphore );
    }
    else
    {
        this->semName = "";
        this->syncSemaphore = _NULL;
    }
    
    if( size > 0 )
    {
        paramBuffer   = malloc( size );
        if (paramBuffer != _NULL) {
            memcpy( paramBuffer, buffer, size );
        }
    }
    else
    {
        paramBuffer = _NULL;
    }
}

CMessage::CMessage( CMessage& ref )
{
    type            = ref.type;
    srcClientName   = ref.srcClientName;
    bufferSize      = ref.bufferSize;
    time_stamp      = ref.time_stamp;
    semName         = ref.semName;
    syncSemaphore   = ref.syncSemaphore;
    
    if( ref.syncSemaphore != _NULL )
    {
        syncSemaphore   = SemaphoreManager->createSemaphore( semName, 0 );
        assert( syncSemaphore );
    }
    
    if( ref.bufferSize == 0 )
    {
        SAFE_FREE( paramBuffer );
        return;
    }
    
    _PVOID  tmp     = paramBuffer;
    tmp     = realloc( paramBuffer, ref.bufferSize );
    if( tmp == _NULL )
    {
        return;
    }
    paramBuffer = tmp;
    memcpy( paramBuffer, ref.paramBuffer, ref.bufferSize );
}

CMessage::~CMessage()
{
   SAFE_FREE( paramBuffer );
   if( syncSemaphore )
   {
       SemaphoreManager->releaseSemaphore( semName );
       syncSemaphore = _NULL;
   }
}

_BOOL CMessage::equal( CMessage& ref )
{
    return (type == ref.type
            && bufferSize == ref.bufferSize
            && paramBuffer == ref.paramBuffer);
}

_VOID CMessage::clone( CMessage& ref )
{
    type            = ref.type;
    srcClientName   = ref.srcClientName;
    bufferSize      = ref.bufferSize;
    time_stamp      = ref.time_stamp;
    semName         = ref.semName;
    syncSemaphore   = ref.syncSemaphore;
    
    if( ref.syncSemaphore != _NULL )
    {
        syncSemaphore   = SemaphoreManager->createSemaphore( semName, 0 );
        assert( syncSemaphore );
    }
    
    if( ref.bufferSize == 0 )
    {
        SAFE_FREE( paramBuffer );
        return;
    }
    
    _PVOID  tmp     = paramBuffer;
    tmp     = realloc( paramBuffer, ref.bufferSize );
    if( tmp == _NULL )
    {
        return;
    }
    paramBuffer = tmp;
    memcpy( paramBuffer, ref.paramBuffer, ref.bufferSize );
}
//
//clieninfo

class CMessageCenter::CClientInfo
{
public:
    CClientInfo( _string& semName, pfGetInstance pfn )
    {
        m_MsgClientRef = _NULL;
        m_mutex = CMutexFactory::createMutex();
        m_pfn = pfn;
        
        SemaphoreName = semName;
        messageAvailable = _NULL;
    }

    ~CClientInfo()
    {
        unInit();
        SAFE_DELETE(m_mutex);
    }
    
    _VOID init()
    {
        cleanMsgQueue();
        
        scope_lock<ICMutex> lk(m_mutex);
        if (m_MsgClientRef) {
            stopClient();
        }
        
        m_MsgClientRef = m_pfn();
        if (m_MsgClientRef) {
            m_MsgClientRef->setClientName(SemaphoreName);
            m_MsgClientRef->startRunLoop();
        }
        
        if (messageAvailable) {
            SemaphoreManager->releaseSemaphore(SemaphoreName);
        }
        
        messageAvailable = SemaphoreManager->createSemaphore( SemaphoreName, 0 );
    }
    
    _VOID unInit()
    {
        stopClient();
        cleanMsgQueue();
        SemaphoreManager->releaseSemaphore(SemaphoreName);
        messageAvailable = _NULL;
    }
    
    _VOID stopClient()
    {
        pushMessage(new CMessage(SemaphoreName, MESSAGE_COM_EXIT, _NULL, 0, _FALSE), _TRUE);
    }
    
    _VOID cleanMsgQueue()
    {
        scope_lock<ICMutex> lk(m_mutex);
        _INT size = CAST( _UINT, msgQueue.size());
        while ( size-- > 0 )
        {
            CMessage* msg = msgQueue.front();
            if (msg->syncSemaphore) {
                msg->syncSemaphore->postSem();
            }
            msgQueue.pop_front();
            SAFE_DELETE( msg );
        }
    }

    _VOID pushMessage( CMessage* msg, _BOOL isHighPri )
    {
        scope_lock<ICMutex> lk(m_mutex);
        if (msgQueue.size() > 0 )
        {
            CMessage* ptr = msgQueue.back();
            if (ptr != _NULL){
                if(msg->equal(*ptr)){
                    return;
                }
            }
            
            ptr = msgQueue.front();
            if (ptr != _NULL) {
                if (msg->equal(*ptr)) {
                    return ;
                }
            }
        }
        
        if (isHighPri) {
            msgQueue.push_front(msg);
        }else{
            msgQueue.push_back( msg );
        }
        
        if (IWork* adapter = dynamic_cast<IWork*>(m_MsgClientRef)) {
            _MY_NAME_SPACE_::CThreadPool::AddWork(adapter, _FALSE);
        }
        messageAvailable->postSem();
    }
    
    _BOOL getMessage( CMessage& refMsg )
    {
        scope_lock<ICMutex> lk(m_mutex);
        if( msgQueue.size() > 0 )
        {
            CMessage* msg = msgQueue.front();
            refMsg.clone( *msg );
            SAFE_DELETE( msg );
            msgQueue.pop_front();
            return _TRUE;
        }
        return _FALSE;
    }
    _CONST CMessage* peekMessage()
    {
        scope_lock<ICMutex> lk(m_mutex);
        if( msgQueue.size() > 0 )
        {
            CMessage* msg = msgQueue.front();
            return msg;
        }
        return _NULL;
    }
private:
    ICMutex*                m_mutex;
    pfGetInstance           m_pfn;
    MessageAdapter*         m_MsgClientRef;
public:
    _string                 SemaphoreName;
    ICSemaphoreNamed*       messageAvailable;
    std::list<CMessage*>    msgQueue;
};


//
ICMutex* CMessageCenter::m_ptrThreadMutex = CMutexFactory::createStaticMutex();
CMessageCenter::MsgMap   CMessageCenter::m_mMessageMap    = MsgMap();

_BOOL CMessageCenter::sendMessage( _string &srcClientName, _string &name, MessageType type, _PVOID param, _UINT paramBufferSize, _BOOL isHighPri, _BOOL isSync )
{
    _string semName = "";
    ICSemaphoreNamed*  semRef = _NULL;
    
    m_ptrThreadMutex->lock();
    msgIt findIt = m_mMessageMap.find( name );
    if( findIt != m_mMessageMap.end() )
    {
        CMessage* newMessage        = new CMessage( srcClientName, type, param, paramBufferSize, isSync );
        
        //向SemaphoreManager注册，增加引用计数
        semName = newMessage->semName;
        if ( semName.empty() == _FALSE )
        {
            semRef   = SemaphoreManager->createSemaphore( semName, 0 );
        }
        //
        
        findIt->second->pushMessage(newMessage, isHighPri);
        m_ptrThreadMutex->unlock();
        
        //等待消息执行方返回
        if ( semRef != _NULL )
        {
            semRef->waitSem();
            SemaphoreManager->releaseSemaphore( semName );
        }
        
        return  _TRUE;
    }
    m_ptrThreadMutex->unlock();
    return _FALSE;
}

_BOOL CMessageCenter::sendMessage( _string &srcClientName, MessageType type, _PVOID param, _UINT paramBufferSize, _BOOL isHighPri )
{
    scope_lock<ICMutex> lk( m_ptrThreadMutex );
    msgIt it = m_mMessageMap.begin();
    
    for ( ; it != m_mMessageMap.end(); ++it )
    {
        if ( srcClientName == it->first ){
            continue;
        }
        
        CMessage* newMessage = new CMessage( srcClientName, type, param, paramBufferSize, _FALSE );
        it->second->pushMessage( newMessage, isHighPri);
    }
    
    return _TRUE;
}

_INT CMessageCenter::getMessage( _string& name, CMessage& message )
{
    _INT ret = -2;
    scope_lock<ICMutex> lk( m_ptrThreadMutex );
    msgIt findIt = m_mMessageMap.find( name );

    if( findIt != m_mMessageMap.end() )
    {
        if( findIt->second->getMessage( message ) )
        {
            return 1;
        }
        ret = 0;
    }

    return ret;
}

_BOOL CMessageCenter::peekMessage( _string& name, _CONST CMessage* message )
{
    scope_lock<ICMutex> lk( m_ptrThreadMutex );
    _BOOL ret = _FALSE;
    msgIt findIt = m_mMessageMap.find( name );
    
    if( findIt != m_mMessageMap.end() )
    {
        message = findIt->second->peekMessage();
        if ( message != _NULL )
        {
            ret = _TRUE;
        }
    }

    return ret;
}

_BOOL CMessageCenter::detachAllComps()
{
    scope_lock<ICMutex> lk( m_ptrThreadMutex );
    
    for (msgIt it = m_mMessageMap.begin(); it != m_mMessageMap.end(); it++ ){
        SAFE_DELETE( it->second );
    }
    m_mMessageMap.clear();
    return _TRUE;
}

_BOOL CMessageCenter::attach(_string &name, pfGetInstance pfn)
{
    CClientInfo* cl = new CClientInfo(name, pfn);
    cl->init();
    
    scope_lock<ICMutex> lk( m_ptrThreadMutex );
    msgIt findIt = m_mMessageMap.find(name);
    if (findIt != m_mMessageMap.end()) {
        SAFE_DELETE(findIt->second);
        m_mMessageMap[name] = cl;
    }else{
        m_mMessageMap.insert(MSG_PAIR(name, cl));
    }

    return  _TRUE;
}
_BOOL CMessageCenter::detach( _string &name )
{
    scope_lock<ICMutex> lk( m_ptrThreadMutex );
    _BOOL ret       = _FALSE;
    msgIt findIt    = m_mMessageMap.find( name );
    
    if( findIt != m_mMessageMap.end() )
    {
        SAFE_DELETE(findIt->second);
        m_mMessageMap.erase( findIt );
        ret = _TRUE;
    }
    return ret;
}
