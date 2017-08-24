//
//  ThreadWithQueue.hpp
//  commons_mac
//
//  Created by 周贺伟 on 2016/11/23.
//  Copyright © 2016年 zhouhewei. All rights reserved.
//

#ifndef ThreadWithQueue_hpp
#define ThreadWithQueue_hpp

#include <list>
#include <map>
#include "defines.h"
#include "ICMutex.h"
#include "ICSemaphoreNamed.h"
#include "RefBase.h"
#include "IWork.h"

namespace  _MY_NAME_SPACE_
{
    class CThreadWithQueue:public RefBase
    {
    public:
        class TheadInfo;
        class ConstInfo;
        typedef ::std::list< IWork* > WorkQueue;
        typedef ::std::map<_TID, TheadInfo*> Thread_task_map;
    public:
        CThreadWithQueue();
        CThreadWithQueue(_UINT QueueSize);
        virtual ~CThreadWithQueue();
    public:
        _VOID            setDelayPerTask(_UINT ms);
        _VOID            setMaxWorkThread( _UINT maxThread );
    public:
        _VOID            start();
        _VOID            stop();
        _BOOL            getStatus();
    public:
        _VOID            AddWork(IWork* task, _BOOL isImmediately);
        _VOID            clearWorkQueue();
    public:
        static _PVOID    workThreadProc(_PVOID param);
    private:
        _UINT					 m_uiMaxThreadCount;
        _BOOL                    m_bThreadPoolStatus;
        ConstInfo*               m_const_info;
        Thread_task_map          m_mThreadTaskMap;
    };
}
#endif /* CThreadWithQueue_hpp */
