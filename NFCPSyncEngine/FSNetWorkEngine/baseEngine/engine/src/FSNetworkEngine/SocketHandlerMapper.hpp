//
//  SocketHandlerMapper.hpp
//  NetworkEngine
//
//  Created by 周贺伟 on 2017/7/10.
//  Copyright © 2017年 周贺伟. All rights reserved.
//

#ifndef SocketHandlerMapper_hpp
#define SocketHandlerMapper_hpp

#include <stdio.h>
#include "defines.h"
#include "ICMutex.h"
#include <map>

#define INVALID_SOCKET_HANDLER  -1

class CSocketHandlerMapper
{
public:
    static _HANDLE  createOutterHandler();
public:
    CSocketHandlerMapper();
    ~CSocketHandlerMapper();
public:
    _VOID   setPair(_HANDLE outterHandler, _HANDLE innerHandler);
    
    _HANDLE getOutHandler(_HANDLE innterHandler);
    _HANDLE getInnerHandler(_HANDLE outterHandler);
    _VOID   removeByInner(_HANDLE innerHandler);
    _VOID   removeByOutter(_HANDLE ouuterHandler);
    
    _BOOL   isOutHandlerExsit(_HANDLE outHandler);
private:
    ICMutex*                    m_mutex;
    std::map<_HANDLE, _HANDLE>  m_outter2Inner;
    std::map<_HANDLE, _HANDLE>  m_inner2Outter;
};

#endif /* SocketHandlerMapper_hpp */
