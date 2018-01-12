//
//  CDataHub.h
//  commons
//
//  Created by 周贺伟 on 14/12/18.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#ifndef __commons__CDataHub__
#define __commons__CDataHub__

#include <stdio.h>

#include "SingleThreadAdapter.h"
#include "SharedThreadAdapter.h"
#include <vector>
#include "DataRequest.h"

class IHubRequestBack;

//class CDataHub:public SharedThreadAdapter
class CDataHub:public SingleThreadAdapter
{
public:
    typedef     ::std::vector< IHubRequestBack* >                   CallList;
    typedef     MAP< _DataHubMessageKind,  CallList >               DispatchMap;
    typedef     CallList::iterator                                  CallListIterator;
    typedef     DispatchMap::iterator                               DispatchMapIterator;
public:
    static CDataHub*		getInstance();
public://overide
    _VOID       onMessage( _string& srcName, MessageType type, _PVOID paramBuffer, _UINT bufferSize, _TIMESTAMP time_stamp );
private:
    CDataHub();
    virtual ~CDataHub();
private:
    _VOID       registerMessage( _DataHubMessageKind MessageKind, IHubRequestBack* hr );
    _VOID       unRegisterMessage( _DataHubMessageKind MessageKind, IHubRequestBack* hr );
    _VOID       dispatchMessage_dataBack( _DataHubMessageKind MessageKind, _HANDLE memHandler, _PVOID reserve, PParamData param );
    _VOID       dispatchMessage_onEvent( _DataHubMessageKind MessageKind, _UINT code, _PVOID reserve, PParamData param );
    _VOID       clear();
public:
    DispatchMap             m_mDispatchMap;
};

//works

//for http work


#endif /* defined(__commons__CDataHub__) */
