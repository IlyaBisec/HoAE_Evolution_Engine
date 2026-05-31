/*****************************************************************************/
/*	File:	kInput.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	22.05.2003
/*****************************************************************************/
#include "stdafx.h"
#include "kInput.h"

CoreInputDispatcher g_CoreInputDispatcher;
IInputDispatcher*   IInput = &g_CoreInputDispatcher;

bool CoreInputDispatcher::OnMouseWheel( int delta )					
{ 
    return false; 
}

bool CoreInputDispatcher::OnMouseMove( int mX, int mY, DWORD keys )	
{ 
    return false; 
}

bool CoreInputDispatcher::OnMouseLBDown( int mX, int mY )
{ 
    return false; 
}

bool CoreInputDispatcher::OnMouseMBDown( int mX, int mY )				
{ 
    return false; 
}

bool CoreInputDispatcher::OnMouseRBDown( int mX, int mY )				
{ 
    return false; 
}

bool CoreInputDispatcher::OnMouseLBUp( int mX, int mY )				
{ 
    return false; 
}

bool CoreInputDispatcher::OnMouseMBUp( int mX, int mY )				
{ 
    return false; 
}

bool CoreInputDispatcher::OnMouseRBUp( int mX, int mY )				
{ 
    return false; 
}

bool CoreInputDispatcher::OnMouseLBDbl( int mX, int mY )				
{ 
    return false; 
}

bool CoreInputDispatcher::OnMouseRBDbl( int mX, int mY )				
{ 
    return false; 
}

bool CoreInputDispatcher::OnMouseMBDbl( int mX, int mY )				
{ 
    return false; 
}

bool CoreInputDispatcher::OnKeyDown( DWORD keyCode, DWORD flags )	
{ 
    return false; 
}

bool CoreInputDispatcher::OnChar( DWORD charCode, DWORD flags )	
{ 
    return false; 
}

bool CoreInputDispatcher::OnKeyUp( DWORD keyCode, DWORD flags )	
{ 
    return false; 
}
