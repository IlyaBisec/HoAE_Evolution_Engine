/*****************************************************************************/
/*    File:    kThread.cpp
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    28.01.2003
/*****************************************************************************/
#include "stdafx.h"
#include "gmDefines.h"
#include "kThread.h"

Thread::Thread( ThreadType thType ) : 
    handle(0), curPriority(THREAD_PRIORITY_NORMAL), type(thType)
{
    handle = CreateThread(    NULL, 
                            0, 
                            Thread::_EnterThread, 
                            (void*)this, 
                            CREATE_SUSPENDED, 
                            &id );
} // Thread::Thread

Thread::~Thread()
{
    Terminate();
}

void Thread::Run()
{
    assert( handle );
    ResumeThread( handle );
} // Thread::Run

void Thread::Pause()
{
    assert( handle );
    SuspendThread( handle );
}

void Thread::Terminate()
{
    OnTerminate();
    if (handle) TerminateThread( handle, 0 );
    handle = NULL;
}

void Thread::SetPriority( int priority )
{
    if (handle) SetThreadPriority( handle, priority );
    curPriority = priority;
}


DWORD Thread::OnStart()
{
    return 0;    
}

DWORD Thread::OnMessage( const MSG& msg )
{
    return 0;
}
    
void Thread::OnTerminate()
{
}


DWORD Thread::_StartThread()
{
    if (type == thtWorker)
    {
        return OnStart();
    }
    
    if (type == thtMessagePump)
    {
        OnStart();
        MSG msg;
        ZeroMemory( &msg, sizeof(msg) );
        while (msg.message != WM_QUIT)
        {
            if (PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ))
            {
                TranslateMessage( &msg );
                DispatchMessage( &msg );
            }
            else
            { 
                OnMessage( msg );
            }
        }
    }

    return 0;
} // Thread::_StartThread

DWORD WINAPI Thread::_EnterThread( void* param )
{
    Thread* thisThread = (Thread*)param;
    assert( thisThread );
    return thisThread->_StartThread(); 
} // Thread::_EnterThread