//
//  SocketRecvManager.hpp
//  SocketEngine
//
//  Created by 周贺伟 on 16/8/18.
//  Copyright © 2016年 周贺伟. All rights reserved.
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
    static SocketRecvManager*   getInstance();
    static _VOID                destroy();
    static _PVOID               doRecv(_PVOID arg);
public:
    _VOID start(_DataHubMessageKind kind);
    _VOID stop();
    _BOOL isRuning();
public:
    _VOID SocketList_Add(_UINT fd);
    _VOID SocketList_Del(_UINT fd);
    _VOID SocketList_Clean();
public:
    _INT  FillSet(fd_set* RSet, fd_set* WSet, fd_set* ESet);
    _VOID ProcessSelectEvent(fd_set* RSet, fd_set* WSet, fd_set* ESet);
private:
    SocketRecvManager();
    ~SocketRecvManager();
    _VOID  dataBack(_UINT fd, _PVOID buffer, _ULONG bufferSize);
    _VOID  onEvent(_UINT fd, _UINT eventCode);
public:
    static SocketRecvManager*   m_sInstance;
    static ICMutex*             m_sInstanceMutex;
    
private:
    _DataHubMessageKind                     m_kind;
    _TID                                    m_pid;
    _BOOL                                   m_bRun;
    std::vector<_UINT>                      m_socketList;
    ICMutex*                                m_socketListMutex;
    
    
    ICMutex*                                m_mutex;
};

#endif /* SocketRecvManager_hpp */
