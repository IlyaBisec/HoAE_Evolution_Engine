/*****************************************************************
/*  File:   kInput.h                                            
/*  Desc:   Input dispatching manager                           
/*  Author: Silver, Copyright (C) GSC Game World                
/*  Date:   Mar 2002                                            
/*****************************************************************/
#ifndef __KINPUT_H__
#define __KINPUT_H__

#include "IInput.h"

/*****************************************************************************/
/*	Class:	CoreInputDispatcher
/*  Desc:   
/*****************************************************************************/
class CoreInputDispatcher : public IInputDispatcher
{
public:
    virtual bool 	OnMouseWheel		( int delta );					
    virtual bool 	OnMouseMove			( int mX, int mY, DWORD keys );	
    virtual bool 	OnMouseLBDown	    ( int mX, int mY );				
    virtual bool 	OnMouseMBDown	    ( int mX, int mY );				
    virtual bool 	OnMouseRBDown	    ( int mX, int mY );				
    virtual bool 	OnMouseLBUp	        ( int mX, int mY );				
    virtual bool 	OnMouseMBUp	        ( int mX, int mY );				
    virtual bool 	OnMouseRBUp	        ( int mX, int mY );				
    virtual bool 	OnMouseLBDbl        ( int mX, int mY );				
    virtual bool 	OnMouseRBDbl        ( int mX, int mY );				
    virtual bool 	OnMouseMBDbl        ( int mX, int mY );				
    virtual bool	OnKeyDown			( DWORD keyCode, DWORD flags );	
    virtual bool	OnChar				( DWORD charCode, DWORD flags );	
    virtual bool	OnKeyUp				( DWORD keyCode, DWORD flags );	

    CoreInputDispatcher()
    {
        IInput = this;
    }
private:
}; // class CoreInputDispatcher

#endif // __KINPUT_H__
