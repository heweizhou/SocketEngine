//
//  scope_lock.hpp
//  commons
//
//  Created by 周贺伟 on 15/7/5.
//  Copyright (c) 2015年 周贺伟. All rights reserved.
//

#ifndef commons_scope_lock_hpp
#define commons_scope_lock_hpp

template<class Mutex>
class scope_lock
{
public:
    scope_lock( Mutex* _CONST mutex )
    {
        assert( mutex );
        m_mutex = mutex;
        m_mutex->lock();
        m_locked = _TRUE;
    }
    
    scope_lock( Mutex& mutex )
    {
        m_mutex = &mutex;
        m_mutex->lock();
        m_locked = _TRUE;
    }
    
    ~scope_lock()
    {
        if ( m_locked == _TRUE )
        {
            m_mutex->unlock();
        }
        
    }
    
    _VOID unlock()
    {
        m_mutex->unlock();
        m_locked = _FALSE;
    }
    
    
private:
    _BOOL   m_locked;
    Mutex*  m_mutex;
};

#endif
