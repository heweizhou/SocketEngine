//
//  CDataHub.cpp
//  commons
//
//  Created by 周贺伟 on 14/12/18.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#include "CDataHub.h"
#include "CThreadPool.h"
#include "CShareMemManager.h"
#include "CBundleManager.h"
#include "CShareMemManager.h"
#include "MessageAdapter.h"
#include "DataRequestProtocol.h"
#include "log.h"
#include <algorithm>

#define THREAD_NUMBER 1
#define  MAP_PAIR(key,value)  std::pair<_DataHubMessageKind, CallList>( (key), (value) )

CDataHub* CDataHub::getInstance()
{
    return  new CDataHub();
}

CDataHub::CDataHub()
{
}

CDataHub::~CDataHub()
{
}

_VOID CDataHub::clear()
{
    DispatchMapIterator it = m_mDispatchMap.begin();
    
    for( ;it != m_mDispatchMap.end(); ++it)
    {
        CallListIterator hrIt =  it->second.begin();
        CallListIterator hrItEnd = it->second.end();
        for (; hrIt != hrItEnd; ++hrIt) {
            (*hrIt)->release();
        }
    }
    
    m_mDispatchMap.clear();
}

_VOID CDataHub::registerMessage( _DataHubMessageKind MessageKind, IHubRequestBack* hr )
{
    DispatchMapIterator findIt = m_mDispatchMap.find( MessageKind );
    if ( findIt == m_mDispatchMap.end() )
    {
        CallList    newList;
        hr->incRef();
        newList.push_back( hr );
        m_mDispatchMap.insert( MAP_PAIR( MessageKind, newList ) );
    }
    else
    {
        findIt->second.push_back( hr );
    }
    
}


_VOID CDataHub::unRegisterMessage( _DataHubMessageKind MessageKind, IHubRequestBack* hr )
{
    DispatchMapIterator findIt = m_mDispatchMap.find( MessageKind );
    if ( findIt != m_mDispatchMap.end() )
    {
        CallListIterator it =  std::find( findIt->second.begin(), findIt->second.end(), hr );
        if ( it != findIt->second.end() )
        {
            SAFE_RELEASE((*it));
            findIt->second.erase( it );
        }
    }
}

_VOID CDataHub::dispatchMessage_dataBack( _DataHubMessageKind MessageKind, _HANDLE memHandler, _PVOID reserve, PParamData param)
{
    
    DispatchMapIterator findIt = m_mDispatchMap.find( MessageKind );
    if ( findIt != m_mDispatchMap.end() )
    {
        CallListIterator it = findIt->second.begin();
        while (it != findIt->second.end()) {
            if ((*it)->getRefCount() > 1) {
                (*it)->OnDataBack( memHandler, reserve, param );
                it++;
            }else if((*it)->getRefCount() == 1){
                (*it)->release();
                findIt->second.erase(it++);
            }else{
                findIt->second.erase(it++);
            }
            
        }
    }
    
    ShareMemManager->destroySharedMemory( memHandler );
}

_VOID CDataHub::dispatchMessage_onEvent( _DataHubMessageKind MessageKind, _UINT code, _PVOID reserve, PParamData param )
{    
    DispatchMapIterator findIt = m_mDispatchMap.find( MessageKind );
    if ( findIt != m_mDispatchMap.end() )
    {
        CallListIterator it = findIt->second.begin();
        while (it != findIt->second.end()) {
            if ((*it)->getRefCount() > 1) {
                (*it)->OnEvent(code, reserve, param);
                it++;
            }else if((*it)->getRefCount() == 1){
                (*it)->release();
                it = findIt->second.erase(it);
            }else{
                it = findIt->second.erase(it);
            }
            
        }
    }
}

_VOID CDataHub::onMessage( _string& srcName, MessageType type, _PVOID paramBuffer, _UINT bufferSize, _TIMESTAMP time_stamp )
{
    switch ( type)
    {
        case MESSAGE_DATAHUB_LOAD_COMPLETE:
        {
            _HANDLE     bundleHandler = *static_cast<_HANDLE*>(paramBuffer);
            CBundle*    bundle        = BundleManager->getObject( bundleHandler );
            
            assert( bundle );
            
            if (!bundle) {
                break;
            }
            
            _DataHubMessageKind msgKind = 0;
            ParamData  param;
            _PVOID  reserve     = _NULL;
            _HANDLE memHandler  = 0;
            _ULONG  size        = 0;
            
            _PVOID buffer;
            
            bundle->lock();
            buffer = bundle->getData( "MsgKind", size );
            memcpy( &msgKind, buffer, size );
            
            buffer = bundle->getData( "memHandler", size );
            memcpy( &memHandler, buffer, size );
            
            buffer = bundle->getData("reserve", size);
            memcpy(&reserve, buffer, size);
            
            buffer = bundle->getData( "param", size );
            dispatchMessage_dataBack(msgKind, memHandler, reserve, (PParamData)buffer);
            
            bundle->unlock();
            BundleManager->destroyBundle( bundleHandler );
            break;
        }
        
        case MESSAGE_DATAHUB_ON_EVENT:
        {
            _HANDLE  bundleHandler = *static_cast<_HANDLE*>(paramBuffer);
            if (bundleHandler != 0) {
                
                CBundle* bundle = BundleManager->getObject( bundleHandler );
                assert( bundle );
                
                if (!bundle) {
                    break;
                }
                
                _DataHubMessageKind msgKind = 0;
                _UINT   code        = 0;
                _PVOID  reserve     = _NULL;
                _ULONG  size        = 0;
                
                _PVOID buffer = _NULL;
                
                bundle->lock();
                buffer = bundle->getData("MsgKind", size);
                memcpy( &msgKind, buffer, size );
                
                buffer = bundle->getData("event_code", size);
                memcpy(&code, buffer, size);
                
                buffer = bundle->getData("reserve", size);
                memcpy(&reserve, buffer, size);
                
                buffer = bundle->getData("param", size);
                
                dispatchMessage_onEvent(msgKind, code, reserve, (PParamData)buffer);
                
                bundle->unlock();
                BundleManager->destroyBundle(bundleHandler);
            }
        }
            break;
            
        case MESSAGE_DATAHUB_REG_MSG_KIND:
        {
            _HANDLE     bundleHandler = *static_cast<_HANDLE*>(paramBuffer);
            CBundle*    bundle        = BundleManager->getObject( bundleHandler );
            
            assert( bundle );
            
            if (!bundle) {
                break;
            }
            
            _DataHubMessageKind msgKind = 0;
            _PVOID  thisPtr             = _NULL;
            
            _ULONG  size                = 0;
            _PVOID  buffer              = _NULL;
            
            bundle->lock();
            buffer = bundle->getData( "MsgKind", size );
            memcpy( &msgKind, buffer, size );
            
            buffer = bundle->getData( "ptr", size );
            memcpy( &thisPtr, buffer, size );
            bundle->unlock();
            
            this->registerMessage(msgKind, (IHubRequestBack*)thisPtr);
            BundleManager->destroyBundle( bundleHandler );
        }
            break;
        case MESSAGE_DATAHUB_UNREG_MSG_KIND:
        {
            _HANDLE     bundleHandler = *static_cast<_HANDLE*>(paramBuffer);
            CBundle*    bundle        = BundleManager->getObject( bundleHandler );
            
            assert( bundle );
            
            if (!bundle) {
                break;
            }
            
            _DataHubMessageKind msgKind = 0;
            _PVOID  thisPtr             = _NULL;
            
            _ULONG  size                = 0;
            _PVOID  buffer              = _NULL;
            
            bundle->lock();
            buffer = bundle->getData( "MsgKind", size );
            memcpy( &msgKind, buffer, size );
            
            buffer = bundle->getData( "ptr", size );
            memcpy( &thisPtr, buffer, size );
            bundle->unlock();
            
            this->unRegisterMessage(msgKind, (IHubRequestBack*)thisPtr);
            BundleManager->destroyBundle( bundleHandler );
        }
            break;
            
        case MESSAGE_DATAHUB_CLEAR_TASKS:
        {
            _MY_NAME_SPACE_::CThreadPool::clearWorkQueue();
        }
            break;
        case MESSAGE_DATAHUB_CLEAR:
        {
            clear();
        }
            break;
            
        default:
//            _MY_NAME_SPACE_::loge("fx log", "from %s Unkonw messageType %d!\n", srcName.c_str(), type);
            break;
    }
}

_HANDLE createRegisterInfoBundle(_DataHubMessageKind MessageKind, IHubRequestBack* hr)
{
    _HANDLE     bundleHandler   = BundleManager->createBundle();
    CBundle*    bundle          = BundleManager->getObject( bundleHandler );
    
    if (bundle) {
        bundle->setData( "MsgKind", &MessageKind, sizeof( _DataHubMessageKind ) );
        bundle->setData( "ptr", &hr, sizeof(hr));
    }
    return  bundleHandler;
}

//IHubRequestBack

IHubRequestBack::IHubRequestBack( _DataHubMessageKind kind ):m_kind(kind)
{
    _HANDLE bundleHandler = createRegisterInfoBundle(kind, this);
    sendMessage( _SERVICE_DATAHUB, MESSAGE_DATAHUB_REG_MSG_KIND, &bundleHandler, sizeof(bundleHandler));
}

IHubRequestBack::~IHubRequestBack()
{
}

//c functions

extern "C"
{

    _VOID clearUndoRequest()
    {
        postMessage( _SERVICE_DATAHUB, MESSAGE_DATAHUB_CLEAR_TASKS, _NULL, 0 );
    }
    
    _VOID startDataHub()
    {
#ifdef DEBUG
        assert(_MY_NAME_SPACE_::CThreadPool::getStatus());
#endif
        /*启动DataHub组件之前请先启动ThreadPool
         启动线程池支持
         ThreadPool->setMaxWorkThread(THREAD_NUMBER);
         ThreadPool->start();
         */
        attach2MessageCenter(_SERVICE_DATAHUB, (pfGetInstance)CDataHub::getInstance);
    }
    
    _VOID stopDataHub()
    {
        detachFromMessageCenter(_SERVICE_DATAHUB);
    }
}


