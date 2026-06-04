/*****************************************************************************/
/*    File:    kSignal.h
/*    Desc:    Generic string routines
/*    Author:    Ruslan Shestopalyuk
/*    Date:    29.10.2004
/*****************************************************************************/
#ifndef __KSIGNAL_H__
#define __KSIGNAL_H__

#include <vector>
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

// Replace boost::signal with std::function based implementation
// ilya_bisec 04.06.2026
class Signal
{
public:
    // typedef std::function<void()> Func; 2006 version
    using Func = std::function<void()>;

    // Connect object member function
    template <class T> void Connect( T* obj, void (T::*call)() )
    {
        m_slots.push_back([=]() {
            (obj->*call)();
            });
    }

    // Connect std::function
    void Connect(Func f)
    {
        m_slots.push_back(f);
    }

    // Connect generic functor
    template <class T> void Connect( const T& functor )
    {
        m_slots.push_back(functor);
    }

    // Emit signal
    void operator()()
    {
        for (auto &f : m_slots)
            f();
    }

    // Remove all connections
    void DisconnectAll()
    {
        m_slots.clear();
    }

private:
    // Registered callbacks
    std::vector<Func> m_slots;
}; // class Signal

#endif // __KSIGNAL_H__