//
//  CThreadFactory.cpp
//  commons
//
//  Created by 周贺伟 on 14/11/26.
//  Copyright (c) 2014年 周贺伟. All rights reserved.
//

#include "CThreadFactory.h"
#include "complierOption.h"

#ifdef _LINUX_SYS_
#include <pthread.h>
#include <unistd.h>


class ThreadWithNameParam
{
public:
    ThreadWithNameParam(_CONST _CHAR* name, ptrThreadProc proc, _PVOID param);
public:
    _PVOID invokeOriProc();
public:
    static _PVOID workThreadProcWithName(_PVOID param);
public:
    _string         m_name;
    ptrThreadProc   m_proc;
    _PVOID          m_param;
};

ThreadWithNameParam::ThreadWithNameParam(_CONST _CHAR* name, ptrThreadProc proc, _PVOID param)
{
    this->m_name = name;
    this->m_proc = proc;
    this->m_param = param;
}


_PVOID ThreadWithNameParam::invokeOriProc()
{
    return m_proc(m_param);
}

_PVOID ThreadWithNameParam::workThreadProcWithName(_PVOID param)
{
    ThreadWithNameParam* thread_with_name_param = static_cast<ThreadWithNameParam*>(param);
    pthread_setname_np(thread_with_name_param->m_name.c_str());
    _PVOID ret = thread_with_name_param->invokeOriProc();
    SAFE_DELETE(thread_with_name_param);
    
    return ret;
}

_INT CThreadFactory::createThreadWithName(_CONST _CHAR* thread_name, _TID* pid, ptrThreadProc proc, _PVOID param, thread_priority pri)
{
    
    ThreadWithNameParam* _param = new ThreadWithNameParam(thread_name, proc, param);
    return pthread_create( (pthread_t*)pid, _NULL, ThreadWithNameParam::workThreadProcWithName, _param );
}

_INT CThreadFactory::createThread( _TID* pid, ptrThreadProc proc, _PVOID param, thread_priority pri )
{
    return pthread_create( (pthread_t*)pid, _NULL, proc, param );
}

_INT CThreadFactory::sleep( _UINT millisecond )
{
    usleep( millisecond * 1000 );
    return 1;
}

_INT CThreadFactory::waitForThread( _TID pid )
{
    return pthread_join( (pthread_t)pid, _NULL );
}

_TID CThreadFactory::getThreadPID()
{
    return (_TID)pthread_self();
}

_INT CThreadFactory::detachSelf()
{
    return pthread_detach(pthread_self());
}
#endif
