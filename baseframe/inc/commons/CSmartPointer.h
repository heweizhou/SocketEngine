//
//  SmartPointer.h
//  OpenGLES_Class_One
//
//  Created by zhouhewei on 13-12-27.
//
//

#ifndef __OpenGLES_Class_One__SmartPointer__
#define __OpenGLES_Class_One__SmartPointer__

#include <stdio.h>

template<class T>
class SmartPointer{
public:
        
    SmartPointer( SmartPointer<T> const &rhs )
    {
        pt = rhs.pt;
        if( pt != _NULL )
            pt->incRef();
    }
    
    SmartPointer(T *t)
    {
        pt = t;
        if( pt != _NULL )
            pt->incRef();
    }
    
    ~SmartPointer()
    {
        if( pt != _NULL )
            pt->release();
    }
    
    SmartPointer<T>& operator = (SmartPointer<T> rhs )
    {
        if (rhs == *this )
        {
            return *this;
        }
        
        if( pt != _NULL )
            pt->release();
        
        if ( rhs.pt != _NULL )
        {
            rhs.pt->incRef();
            pt = rhs.pt;
        }
        
        return *this;
    }
    
    _BOOL operator== (const SmartPointer<T>& ar) const
    {
        return pt == ar.pt;
    }
    
    _BOOL isNull()
    {
        return  (pt == _NULL);
    }
    T& operator *()
    {
        return *pt;
    }
    T* operator ->()
    {
        return pt;
    }
    T* c_ptr()
    {
        return pt;
    }
    
private:
    T*          pt;
};

#endif /* defined(__OpenGLES_Class_One__SmartPointer__) */
