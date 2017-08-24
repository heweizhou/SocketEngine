//
//  CSocketWork.hpp
//  NetworkEngine
//
//  Created by 周贺伟 on 2017/3/13.
//  Copyright © 2017年 zhouhewei. All rights reserved.
//

#ifndef CSocketWork_hpp
#define CSocketWork_hpp

#include <stdio.h>
#include "IWork.h"
#include "IFSCallBack.h"

class SocketWork:public IWork
{
public:
    virtual _VOID doWork();
    virtual _VOID cancelWork();
public:
    SocketWork( _DataHubMessageKind kind, _HANDLE socketHandler, IFSCallBack* cb, _PVOID param, _ULONG param_len);
    virtual ~SocketWork();
private:
    _DataHubMessageKind     m_kind;
    
    _PVOID                  m_param;
    _ULONG                  m_param_len;
    
    _HANDLE                 m_socketHandler;
    IFSCallBack*            m_cb;
    
    _BOOL                   m_bIsRunning;
};


#endif /* CSocketWork_hpp */
