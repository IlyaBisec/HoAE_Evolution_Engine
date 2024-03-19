/*****************************************************************
/*  File:   dbCurrent.h
/*  Author: Silver, Copyright (C) GSC Game World                 
/*****************************************************************/
#ifndef __DBCURRENT_H__
#define __DBCURRENT_H__


/*****************************************************************/
/*	Class:	CurDebugger
/*	Desc:	Different temporary things debugger
/*****************************************************************/
class CurDebugger : public Dialog
{
public:
							CurDebugger		();
	virtual void			Render				();
	virtual void			Expose				( PropertyMap& pm );
	virtual bool 			OnMouseLBDown	( int mX, int mY );
	virtual bool 			OnMouseMove			( int mX, int mY, DWORD keys );
	virtual bool 			OnMouseRBUp	( int mX, int mY );
    virtual bool 			OnMouseLBUp	( int mX, int mY );

	DECLARE_SCLASS( CurDebugger, Dialog, CDBG );
}; // class CurDebugger



#endif // __DBCURRENT_H__
