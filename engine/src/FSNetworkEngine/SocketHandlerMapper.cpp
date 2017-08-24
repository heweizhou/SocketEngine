//
//  SocketHandlerMapper.cpp
//  NetworkEngine
//
//  Created by 周贺伟 on 2017/7/10.
//  Copyright © 2017年 zhouhewei. All rights reserved.
//

#include "SocketHandlerMapper.hpp"
#include "scope_lock.hpp"
#include "CMutexFactory.h"

_HANDLE  CSocketHandlerMapper::createOutterHandler()
{
    static ICMutex* mutex = CMutexFactory::createStaticMutex();
    static _ULONG handler = 0;
    
    scope_lock<ICMutex> lk(mutex);
    return ++handler;
}

CSocketHandlerMapper::CSocketHandlerMapper()
{
    m_mutex = CMutexFactory::createMutex();
}

CSocketHandlerMapper::~CSocketHandlerMapper()
{
    delete m_mutex;
}

_VOID CSocketHandlerMapper::setPair(_HANDLE outterHandler, _HANDLE innerHandler)
{
    scope_lock<ICMutex> lk(m_mutex);
    if (innerHandler != INVALID_SOCKET_HANDLER) {
        auto findIt = m_inner2Outter.find(innerHandler);
        if (findIt != m_inner2Outter.end()) {
            findIt->second = outterHandler;
        } else {
            m_inner2Outter.insert(std::pair<_HANDLE, _HANDLE>(innerHandler, outterHandler));
        }
    }
    
    if (outterHandler != INVALID_SOCKET_HANDLER) {
        auto findIt = m_outter2Inner.find(outterHandler);
        if (findIt != m_outter2Inner.end()) {
            findIt->second = innerHandler;
        } else {
            m_outter2Inner.insert(std::pair<_HANDLE, _HANDLE>(outterHandler, innerHandler));
        }
    }
}

_HANDLE CSocketHandlerMapper::getOutHandler(_HANDLE innterHandler)
{
    scope_lock<ICMutex> lk(m_mutex);
    
    auto findIt = m_inner2Outter.find(innterHandler);
    if (findIt != m_inner2Outter.end()) {
        return findIt->second;
    }
    
    return INVALID_SOCKET_HANDLER;
}

_HANDLE CSocketHandlerMapper::getInnerHandler(_HANDLE outterHandler)
{
    scope_lock<ICMutex> lk(m_mutex);
    
    auto findIt = m_outter2Inner.find(outterHandler);
    if (findIt != m_outter2Inner.end()) {
        return findIt->second;
    }
    
    return INVALID_SOCKET_HANDLER;
}

_BOOL CSocketHandlerMapper::isOutHandlerExsit(_HANDLE outHandler)
{
    scope_lock<ICMutex> lk(m_mutex);
    auto findIt = m_outter2Inner.find(outHandler);
    if (findIt != m_outter2Inner.end()) {
        return _TRUE;
    }
    
    return _FALSE;
}

_VOID CSocketHandlerMapper::removeByInner(_HANDLE innerHandler)
{
    scope_lock<ICMutex> lk(m_mutex);
    auto outFindit = m_inner2Outter.find(innerHandler);
    if (outFindit != m_inner2Outter.end()) {
        auto innerFindIt = m_outter2Inner.find(outFindit->second);
        if (innerFindIt != m_outter2Inner.end()) {
            m_outter2Inner.erase(innerFindIt);
        }
        m_inner2Outter.erase(outFindit);
    }
    
}

_VOID CSocketHandlerMapper::removeByOutter(_HANDLE outerHandler)
{
    scope_lock<ICMutex> lk(m_mutex);
    
    auto innerFindIt = m_outter2Inner.find(outerHandler);
    if (innerFindIt != m_outter2Inner.end()) {
        auto outFindit = m_inner2Outter.find(innerFindIt->second);
        if (outFindit != m_inner2Outter.end()) {
            m_inner2Outter.erase(outFindit);
        }
        m_outter2Inner.erase(innerFindIt);
    }
   
}
