//
//  CSocketConnectWork.hpp
//  NetworkEngine
//
//  Created by 周贺伟 on 2017/3/13.
//  Copyright © 2017年 周贺伟. All rights reserved.
//

#ifndef CSocketConnectWork_hpp
#define CSocketConnectWork_hpp

#include <stdio.h>
#include "IWork.h"

class SocketConnectWork:public IWork
{
public:
    virtual _VOID doWork();
    virtual _VOID cancelWork();
public:
    SocketConnectWork(_DataHubMessageKind kind, _HANDLE socketHandler, _string& ip, _UINT port, _CONST _INT timeout_sec);
    virtual ~SocketConnectWork();
private:
    _DataHubMessageKind     m_kind;
    _LONG                   m_socketMsgKind;
    
    
    _HANDLE                 m_socketHandler;
    _string                 m_ip;
    _UINT                   m_port;
    _INT                    m_timeout_sec;
    
    _BOOL                   m_bIsRunning;
};


#endif /* CSocketConnectWork_hpp */
