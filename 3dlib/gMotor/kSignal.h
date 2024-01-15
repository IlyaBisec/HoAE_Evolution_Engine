/*****************************************************************************/
/*    File:    kSignal.h
/*    Desc:    Generic string routines
/*    Author:    Ruslan Shestopalyuk
/*    Date:    29.10.2004
/*****************************************************************************/
#ifndef __KSIGNAL_H__
#define __KSIGNAL_H__

#include "..\boost\boost\signal.hpp"
#include <functional>

class BaseFunctor
{
public:
    void operator()(){ Invoke(); }
    virtual void Invoke() = 0;
}; // class BaseFunctor

template <class T> class Functor : public BaseFunctor
{
    typedef void (T::*Callback)();

    Callback        m_Callback;
    T*              m_pObject;
public:
    Functor( T* pObj, Callback callback ) : m_pObject(pObj), m_Callback(callback){}
    virtual void Invoke()
    {
        if (!m_Callback || !m_pObject) return;
        (m_pObject->*m_Callback)();
    }
}; // class Functor

class Signal : public boost::signal0<void>
{
public:
    template <class T> void Connect( T* obj, void (T::*call)() )
    {
        connect( Functor<T>( obj, call ) );
    }

    template <class T> void Connect( const T& functor )
    {
        connect( functor ); 
    }

    void DisconnectAll()
    {
        disconnect_all_slots();
    }
}; // class Signal

#endif // __KSIGNAL_H__