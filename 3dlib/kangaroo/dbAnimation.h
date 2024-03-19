/*****************************************************************
/*  File:   dbAnimation.h
/*  Author: Silver, Copyright (C) GSC Game World                 
/*****************************************************************/
#ifndef __DBANIMATION_H__
#define __DBANIMATION_H__


/*****************************************************************/
/*	Class:	AnimDebugger
/*	Desc:	Different temporary things debugger
/*****************************************************************/
class AnimDebugger : public Dialog
{
public:
							AnimDebugger		();
	virtual void			Render				();
	virtual void			Expose				( PropertyMap& pm );
	virtual bool 			OnMouseLBDown	( int mX, int mY );
	virtual bool 			OnMouseMove			( int mX, int mY, DWORD keys );
	virtual bool 			OnMouseRBUp	( int mX, int mY );
    virtual bool 			OnMouseLBUp	( int mX, int mY );

	DECLARE_SCLASS( AnimDebugger, Dialog, ADBG );
}; // class AnimDebugger



#endif // __DBANIMATION_H__
