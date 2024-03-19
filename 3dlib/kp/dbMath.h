/*****************************************************************
/*  File:   dbMath.h
/*  Author: Silver, Copyright (C) GSC Game World                 
/*****************************************************************/
#ifndef __DBMATH_H__
#define __DBMATH_H__


/*****************************************************************/
/*	Class:	MathDebugger
/*	Desc:	Different temporary things debugger
/*****************************************************************/
class MathDebugger : public Dialog
{
public:
							MathDebugger		();
	virtual void			Render				();
	virtual void			Expose				( PropertyMap& pm );
	virtual bool 			OnMouseLBDown	    ( int mX, int mY );
	virtual bool 			OnMouseMove			( int mX, int mY, DWORD keys );
	virtual bool 			OnMouseRBUp	        ( int mX, int mY );
    virtual bool 			OnMouseLBUp	        ( int mX, int mY );

	DECLARE_SCLASS( MathDebugger, Dialog, MDBG );
}; // class MathDebugger



#endif // __DBMATH_H__
