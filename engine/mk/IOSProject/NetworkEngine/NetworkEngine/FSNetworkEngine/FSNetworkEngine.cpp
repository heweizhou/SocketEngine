//
//  FSNetworkEngine.cpp
//  NetworkEngine
//
//  Created by 周贺伟 on 15/11/16.
//  Copyright © 2015年 fxiaoke. All rights reserved.
//

#include <stdio.h>
#include "FSNetworkEngine.h"
#include "SocketEngineManager.h"
#include "CMutexFactory.h"
#include "scope_lock.hpp"
#include "CThreadPool.h"
#include "MessageAdapter.h"
#include "CDataHubCtrl.h"
#include "log.h"

#include "CSocketConnectWork.hpp"
#include "CSocketWork.hpp"

#define  MSG_KIND_SOCKET_BACK (DATA_HUB_MESSAGEKIND_USER + 1)

IFSNetworkEngine*       g_instance = _NULL;
ICMutex*                g_mutex  = CMutexFactory::createStaticMutex();

_VOID   startEngineComs();
_VOID   stopEngineComs();

extern "C"
{
    /**
     @brief	获取socket引擎实例
     */
    
    IFSNetworkEngine* getEngineInstance()
    {
        if (g_instance == _NULL) {
            scope_lock<ICMutex> lk(g_mutex);
            if (g_instance == _NULL) {
                startEngineComs();
                g_instance = new FSNetworkEngine();
            }
        }
        return g_instance;
    }
    
    /**
     @brief 析构引擎
     */
    _VOID destorySocketEngineInstance()
    {
        scope_lock<ICMutex> lk(g_mutex);
        stopEngineComs();
        SAFE_DELETE(g_instance);
    }
}

_VOID startEngineComs()
{
    _MY_NAME_SPACE_::CThreadPool::start();
    startDataHub();
    SocketEngineManager::start_recv_thread(MSG_KIND_SOCKET_BACK);
}

_VOID stopEngineComs()
{
    stopDataHub();
    _MY_NAME_SPACE_::CThreadPool::stop();
    detachAllComponent();
    SocketEngineManager::stop_recv_thread();
}

 _VOID FSNetworkEngine::setCallBack(IFSCallBack* cb )
{
    if (cb) {
        m_faciShareCallBack = cb;
    }
}

_HANDLE FSNetworkEngine::createSocket(_INT domain, _INT protocal)
{
    return SocketEngineManager::createSocket(domain, protocal);
}

_BOOL FSNetworkEngine::async_connectToHost(_HANDLE socketFD, _CONST _CHAR* ipAddress, _CONST _UINT port, _LONG socketMsgKind, _INT timeout_sec)
{
    _string ip = ipAddress;
    SocketConnectWork* work = new SocketConnectWork(MSG_KIND_SOCKET_BACK, socketFD, ip, port, timeout_sec);
    work->add2Pool(_TRUE);
    SAFE_RELEASE(work);
    return _TRUE;
}


_INT FSNetworkEngine::connectToHost( _HANDLE socketFD, _CONST _CHAR* ipAddress, _CONST _UINT port, _LONG socketMsgKind, _INT timeout_sec)
{
    return SocketEngineManager::connectToHost(socketFD, ipAddress, port, timeout_sec);
}

_BOOL FSNetworkEngine::isConnected(_HANDLE socketFD)
{
    return SocketEngineManager::isConnected(socketFD);
}

_LONG FSNetworkEngine::sendData(_HANDLE socketFD, _CONST _CHAR *binaryData, _LONG dataLength)
{
    if (binaryData == _NULL || dataLength == 0) {
        return 0;
    }
    return SocketEngineManager::sendData(socketFD, binaryData, dataLength);
}

//_BOOL FSNetworkEngine::async_sendData(_HANDLE socketFD, _CONST _CHAR *binaryData, _LONG dataLength, _PVOID param, _ULONG param_len, _BOOL pri)
//{
//    if (binaryData == _NULL || dataLength == 0) {
//        OnEvent(INNER_EVENT_SEND_PACKAGE_INVALID, _NULL, (PParamData)param);
//    }else{
//        
//        _HANDLE     shmHandler      = ShareMemManager->createSharedMemory(dataLength);
//        ICSharedMemory* shm         = ShareMemManager->getObject(shmHandler);
//        
//        if (!shm) {
//            return 0;
//        }
//        _PVOID ptrMem               = shm->obtainMemoryLock();
//        
//        if (!ptrMem) {
//            shm->releaseMemoryLock();
//            return 0;
//        }
//        
//        SAFE_COPY(ptrMem, dataLength, binaryData, dataLength);
//        shm->releaseMemoryLock();
//        
//        IWork* work = new SocketWork(MSG_KIND_SOCKET_BACK, socketFD, shmHandler, param, param_len);
//        work->add2Pool(pri);
//        SAFE_RELEASE(work);
//    }
//    
//    return _TRUE;
//}

_BOOL FSNetworkEngine::async_sendData(_HANDLE socketFD, _PVOID param, _ULONG param_len, _BOOL pri)
{
    IWork* work = new SocketWork(MSG_KIND_SOCKET_BACK, socketFD, m_faciShareCallBack, param, param_len);
    work->add2Pool(pri);
    SAFE_RELEASE(work);
    
    return _TRUE;
}

_BOOL FSNetworkEngine::disconnectSocket(_HANDLE socketFD)
{
    return SocketEngineManager::disconnectSocket(socketFD);
}

_VOID FSNetworkEngine::OnDataBack(_HANDLE memHandler, _PVOID reserve, PParamData param)
{
    if (m_faciShareCallBack) {
        ICSharedMemory* shm = ShareMemManager->getObject(memHandler);
        _ULONG len = shm->getlength();
        _PVOID ptr = shm->obtainMemoryLock();
        m_faciShareCallBack->onReceiveData((int)(_HANDLE)reserve, (_CONST _CHAR*)ptr, len);
        shm->releaseMemoryLock();
    }
    
    ShareMemManager->destroySharedMemory(memHandler);
}
_VOID FSNetworkEngine::OnEvent(_UINT code, _PVOID reserve, PParamData param)
{
    if (m_faciShareCallBack) {
        switch (code) {
            case INNER_EVENT_SEND_SUCCESSED:
                m_faciShareCallBack->socketSendSuccessed((int)(_HANDLE)reserve, param);
                break;
                
            case INNER_EVENT_SEND_FAILD:
                m_faciShareCallBack->socketSendSuccessedFailed((int)(_HANDLE)reserve, param);
                break;
            case INNER_EVENT_SEND_PACKAGE_INVALID:
                m_faciShareCallBack->socketSendPackageInvalid((int)(_HANDLE)reserve, param);
                break;
            case INNER_EVENT_SOCKET_CLOSED:
            //暂时不处理select error
            //case INNER_EVENT_SOCKET_SELECT_ERROR:
                m_faciShareCallBack->socketDidClosed((int)(_HANDLE)reserve);
                break;
                
            case INNER_EVENT_CONNECTED_SUCCESSED:
                m_faciShareCallBack->socketConnectSuccessed((int)(_HANDLE)reserve);
                break;
                
            case INNER_EVENT_CONNECTED_FAILD:
                m_faciShareCallBack->sockeErrorCallBack((int)(_HANDLE)reserve,SocketOtherError);
                break;
                
            case INNER_EVENT_CONNECTED_TIMEOUT:
                m_faciShareCallBack->socketConnectTimeout((int)(_HANDLE)reserve);
                break;
            case INNER_EVENT_CONNECTED_USER_BREAK:
                m_faciShareCallBack->socketConnectUserBreak((int)(_HANDLE)reserve);
                break;
            case INNER_EVENT_CONNECTED_VAVAIL:
                m_faciShareCallBack->sockeErrorCallBack((int)(_HANDLE)reserve,SocketConnectAddressTavail);
                break;
            case INNER_EVENT_CONNECTED_REFUSED:
                m_faciShareCallBack->sockeErrorCallBack((int)(_HANDLE)reserve,SocketConnectRefuesd);
                break;
            default:
                break;
        }
    }
}


FSNetworkEngine::FSNetworkEngine():IHubRequestBack(MSG_KIND_SOCKET_BACK)
{
    m_faciShareCallBack = _NULL;
}

FSNetworkEngine::~FSNetworkEngine()
{
}
