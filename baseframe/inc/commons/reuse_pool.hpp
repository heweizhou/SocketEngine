//
//  reuse_pool.hpp
//  commons
//
//  Created by 周贺伟 on 15/7/5.
//  Copyright (c) 2015年 周贺伟. All rights reserved.
//

#ifndef commons_reuse_pool_hpp
#define commons_reuse_pool_hpp

#include <queue>
#include "defines.h"

template<class T>
class reuse_pool
{
public:
    typedef  _VOID(*clearFunctionPtr)(T*);
public:
    T*  getUnuse()
    {
        if( m_qReuseQueue.size() == 0 )
        {
            return 0;
        }
        else
        {
            T* ret = m_qReuseQueue.front();
            m_qReuseQueue.pop();
            return ret;
        }
    }
    
    _VOID   enPool( T* ele )
    {
        m_qReuseQueue.push( ele );
    }
    
    _VOID  clear( clearFunctionPtr fptr )
    {
        assert( fptr );
        while ( m_qReuseQueue.size() > 0 )
        {
            fptr( m_qReuseQueue.front() );
            m_qReuseQueue.pop();
        }
    }
private:
    std::queue< T* >   m_qReuseQueue;
};

#endif
