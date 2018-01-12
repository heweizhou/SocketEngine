//
//  FSNetworkEngine.cpp
//  NetworkEngine
//
//  Created by 周贺伟 on 15/11/16.
//  Copyright © 2015年 fxiaoke. All rights reserved.
//

#include "FSNetworkEngine.h"
#include "CThreadPool.h"
#include "MessageAdapter.h"
#include "CDataHubCtrl.h"

#include "CSocketConnectWork.hpp"
#include "CSocketWork.hpp"
#include "SocketFunctions.hpp"
#include "SocketDefines.h"

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
    _MY_NAME_SPACE_::CThreadPool::start(3);
    startDataHub();
}

_VOID stopEngineComs()
{
    stopDataHub();
    _MY_NAME_SPACE_::CThreadPool::stop();
    detachAllComponent();
}

_BOOL bundleParser(_HANDLE bundleId, _HANDLE& innerHandler, _HANDLE& outterHandler)
{
    if (bundleId == 0) {
        assert(0);
        return _FALSE;
    }
    
    CBundle* bundle = BundleManager->getObject(bundleId);
    bundle->lock();
    bundle->fillData("innerHandler", &innerHandler, sizeof(innerHandler));
    bundle->fillData("outterHandler", &outterHandler, sizeof(outterHandler));
    bundle->unlock();
    
    BundleManager->destroyBundle(bundleId);
    
    return _TRUE;
}

 _VOID FSNetworkEngine::setCallBack(IFSCallBack* cb )
{
    if (cb) {
        m_faciShareCallBack = cb;
    }
}

_HANDLE FSNetworkEngine::createSocket()
{
    _HANDLE outHandler = CSocketHandlerMapper::createOutterHandler();
    m_socket_handler_mapper->setPair(outHandler, INVALID_SOCKET_HANDLER);
    return outHandler;
}

_INT FSNetworkEngine::connectToHost( _HANDLE socketFD, _CONST _CHAR* ipAddress, _CONST _UINT port, _LONG socketMsgKind, _INT timeout_sec)
{
    _HANDLE innerHandler = -1;
    _INT result = connect_to_host(innerHandler, ipAddress, port, timeout_sec);
    
    if (result == INNER_EVENT_CONNECTED_SUCCESSED) {
        m_socket_recv_mgr->SocketList_Add((_INT)innerHandler);
        m_socket_handler_mapper->setPair(socketFD, innerHandler);
    }
    
    return result;
}


_BOOL FSNetworkEngine::async_connectToHost(_HANDLE socketFD, _CONST _CHAR* ipAddress, _CONST _UINT port, _LONG socketMsgKind, _INT timeout_sec)
{
    _string ip = ipAddress;
    SocketConnectWork* work = new SocketConnectWork(MSG_KIND_SOCKET_BACK, socketFD, ip, port, timeout_sec);
    work->add2PoolWithAutoRelease(_TRUE);
    return _TRUE;
}

_BOOL FSNetworkEngine::isConnected(_HANDLE socketFD)
{
    _HANDLE innerHandler = m_socket_handler_mapper->getInnerHandler(socketFD);
    if (innerHandler == INVALID_SOCKET_HANDLER) {
        return _FALSE;
    }
    
    return is_connected(innerHandler);
}

_LONG FSNetworkEngine::sendData(_HANDLE socketFD, _CONST _CHAR *binaryData, _LONG dataLength)
{
    if (binaryData == _NULL || dataLength == 0) {
        return 0;
    }
    
    _HANDLE innerHandler = m_socket_handler_mapper->getInnerHandler(socketFD);
    if (innerHandler == INVALID_SOCKET_HANDLER) {
        return 0;
    }
    
    return send_data(innerHandler, binaryData, dataLength);
}

_BOOL FSNetworkEngine::async_sendData(_HANDLE socketFD, _PVOID param, _ULONG param_len, _BOOL pri)
{
    _HANDLE innerHandler = m_socket_handler_mapper->getInnerHandler(socketFD);
    if (innerHandler == INVALID_SOCKET_HANDLER) {
        return _FALSE;
    }
    
    IWork* work = new SocketWork(MSG_KIND_SOCKET_BACK, innerHandler, m_faciShareCallBack, param, param_len);
    work->add2PoolWithAutoRelease(pri);
    
    return _TRUE;
}

_BOOL FSNetworkEngine::disconnectSocket(_HANDLE socketFD)
{
    _HANDLE innerHandler = m_socket_handler_mapper->getInnerHandler(socketFD);
    if (innerHandler != INVALID_SOCKET_HANDLER) {
        m_socket_recv_mgr->SocketList_Del((_INT)innerHandler);
    }
    
    m_socket_handler_mapper->removeByOutter(socketFD);
    return disconnect_socket(innerHandler);
}

_VOID FSNetworkEngine::OnDataBack(_HANDLE memHandler, _PVOID reserve, PParamData param)
{
    if (m_faciShareCallBack) {
        ICSharedMemory* shm = ShareMemManager->getObject(memHandler);
        _ULONG len = shm->getlength();
        _PVOID ptr = shm->obtainMemoryLock();
        m_faciShareCallBack->onReceiveData((int)m_socket_handler_mapper->getOutHandler((_HANDLE)reserve), (_CONST _CHAR*)ptr, len);
        shm->releaseMemoryLock();
    }
    
    ShareMemManager->destroySharedMemory(memHandler);
}
_VOID FSNetworkEngine::OnEvent(_UINT code, _PVOID reserve, PParamData param)
{
    if (m_faciShareCallBack) {
        switch (code) {
            case INNER_EVENT_SEND_SUCCESSED:
                m_faciShareCallBack->socketSendSuccessed((int)m_socket_handler_mapper->getOutHandler((_HANDLE)reserve), param);
                break;
                
            case INNER_EVENT_SEND_FAILD:
                m_faciShareCallBack->socketSendSuccessedFailed((int)m_socket_handler_mapper->getOutHandler((_HANDLE)reserve), param);
                break;
            case INNER_EVENT_SEND_PACKAGE_INVALID:
                m_faciShareCallBack->socketSendPackageInvalid((int)m_socket_handler_mapper->getOutHandler((_HANDLE)reserve), param);
                break;
            case INNER_EVENT_SOCKET_CLOSED:
            //暂时不处理select error
            //case INNER_EVENT_SOCKET_SELECT_ERROR:
                m_faciShareCallBack->socketDidClosed((int)m_socket_handler_mapper->getOutHandler((_HANDLE)reserve));
                break;
            case INNER_EVENT_CONNECTED_SUCCESSED:
            {
                _HANDLE innerHandler = -1;
                _HANDLE outterHandler = -1;

                _BOOL ret = bundleParser((_HANDLE)reserve, innerHandler, outterHandler);
                if (ret == _FALSE) {
                    return ;
                }
                
                if (!m_socket_handler_mapper->isOutHandlerExsit(outterHandler)) {
                    disconnect_socket(innerHandler);
                    return ;
                }
                m_socket_handler_mapper->setPair(outterHandler, innerHandler);
                m_socket_recv_mgr->SocketList_Add((int)innerHandler);
                m_faciShareCallBack->socketConnectSuccessed((int)outterHandler);
            }
                
                break;
                
            case INNER_EVENT_CONNECTED_FAILD:
            {
                _HANDLE innerHandler = -1;
                _HANDLE outterHandler = -1;
                
                _BOOL ret = bundleParser((_HANDLE)reserve, innerHandler, outterHandler);
                if (ret == _FALSE) {
                    return ;
                }
                m_faciShareCallBack->sockeErrorCallBack((_INT)outterHandler, SocketOtherError);
            }
                break;
                
            case INNER_EVENT_CONNECTED_TIMEOUT:
            {
                _HANDLE innerHandler = -1;
                _HANDLE outterHandler = -1;
                
                _BOOL ret = bundleParser((_HANDLE)reserve, innerHandler, outterHandler);
                if (ret == _FALSE) {
                    return ;
                }
                m_faciShareCallBack->socketConnectTimeout((_INT)outterHandler);
            }
                
                break;
            case INNER_EVENT_CONNECTED_USER_BREAK:
            {
                _HANDLE innerHandler = -1;
                _HANDLE outterHandler = -1;
                
                _BOOL ret = bundleParser((_HANDLE)reserve, innerHandler, outterHandler);
                if (ret == _FALSE) {
                    return ;
                }
                m_faciShareCallBack->socketConnectUserBreak((_INT)outterHandler);
            }
                
                break;
            case INNER_EVENT_CONNECTED_VAVAIL:
            {
                _HANDLE innerHandler = -1;
                _HANDLE outterHandler = -1;
                
                _BOOL ret = bundleParser((_HANDLE)reserve, innerHandler, outterHandler);
                if (ret == _FALSE) {
                    return ;
                }
                m_faciShareCallBack->sockeErrorCallBack((_INT)outterHandler, SocketConnectAddressTavail);
            }
                
                break;
            case INNER_EVENT_CONNECTED_REFUSED:
            {
                _HANDLE innerHandler = -1;
                _HANDLE outterHandler = -1;
                
                _BOOL ret = bundleParser((_HANDLE)reserve, innerHandler, outterHandler);
                if (ret == _FALSE) {
                    return ;
                }
                m_faciShareCallBack->sockeErrorCallBack((_INT)outterHandler, SocketConnectRefuesd);
            }
                
                break;
            default:
                break;
        }
    }
}


FSNetworkEngine::FSNetworkEngine():IHubRequestBack(MSG_KIND_SOCKET_BACK)
{
    m_faciShareCallBack = _NULL;
    m_socket_recv_mgr = new SocketRecvManager();
    m_socket_recv_mgr->start(MSG_KIND_SOCKET_BACK);
    
    m_socket_handler_mapper = new CSocketHandlerMapper();
}

FSNetworkEngine::~FSNetworkEngine()
{
    SAFE_RELEASE(m_socket_recv_mgr);
    delete m_socket_handler_mapper;
}
