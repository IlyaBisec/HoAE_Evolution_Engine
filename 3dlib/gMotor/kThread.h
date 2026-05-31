/*****************************************************************************/
/*    File:    kThread.h
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    28.01.2003
/*****************************************************************************/
#ifndef __KTHREAD_H__
#define __KTHREAD_H__

enum ThreadType
{
    thtUnknown        = 0,
    thtWorker        = 1,
    thtMessagePump    = 2
}; // enum ThreadType

/*****************************************************************************/
/*    Class:    Thread
/*    Desc:    Windows thread wrapper class
/*****************************************************************************/
class Thread
{
    HANDLE            handle;
    DWORD            id;
    int                curPriority;
    ThreadType        type;

public:
                    Thread( ThreadType thType );
    virtual            ~Thread();

    void            Run();
    void            Pause();
    void            Terminate();
    void            SetPriority( int priority );

protected:
    virtual    DWORD    OnStart();
    virtual DWORD    OnMessage( const MSG& msg );
    virtual    void    OnTerminate();

public:
    DWORD                    _StartThread();
    static    DWORD WINAPI    _EnterThread( void* param );        

}; // class Thread

#endif // __KTHREAD_H__