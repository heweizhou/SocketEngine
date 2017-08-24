//
//  SocketRecvManager.hpp
//  SocketEngine
//
//  Created by 周贺伟 on 16/8/18.
//  Copyright © 2016年 zhouhewei. All rights reserved.
//

#ifndef SocketRecvManager_hpp
#define SocketRecvManager_hpp

#include "CMutexFactory.h"
#include "RefBase.h"
#include <vector>
#include <sys/select.h>


class SocketRecvManager:public _MY_NAME_SPACE_::RefBase
{
public:
    static _PVOID               doRecv(_PVOID arg);
public:
    SocketRecvManager();
    virtual ~SocketRecvManager();
public:
    _VOID start(_DataHubMessageKind kind);
    _VOID stop();
    _BOOL isRuning();
public:
    _VOID SocketList_Add(_INT fd);
    _VOID SocketList_Del(_INT fd);
    _VOID SocketList_Clean();
public:
    _INT  FillSet(fd_set* RSet, fd_set* WSet, fd_set* ESet);
    _VOID ProcessSelectEvent(fd_set* RSet, fd_set* WSet, fd_set* ESet);
private:
    
    _VOID  dataBack(_INT fd, _PVOID buffer, _ULONG bufferSize);
    _VOID  onEvent(_INT fd, _UINT eventCode);
    _VOID  private_SocketList_Del(_INT fd);
public:
    static SocketRecvManager*   m_sInstance;
    static ICMutex*             m_sInstanceMutex;
    
private:
    _DataHubMessageKind                     m_kind;
    _TID                                    m_pid;
    _BOOL                                   m_bRun;
    std::vector<_INT>                       m_socketList;
    ICMutex*                                m_socketListMutex;
    
    
    ICMutex*                                m_mutex;
};

#endif /* SocketRecvManager_hpp */
