/*****************************************************************
/*  File:   dbReflection.h
/*  Author: Silver, Copyright (C) GSC Game World                 
/*****************************************************************/
#ifndef __DBREFLECTION_H__
#define __DBREFLECTION_H__


/*****************************************************************/
/*	Class:	ReflectionDebugger
/*	Desc:	Debug Reflection environment
/*****************************************************************/
class ReflectionDebugger : public Dialog
{
    bool                    m_bDrawScene;
public:
							ReflectionDebugger		();
	virtual void			Render				();
	virtual void			Expose				( PropertyMap& pm );
	virtual bool 			OnMouseLBDown	( int mX, int mY );
	virtual bool 			OnMouseMove			( int mX, int mY, DWORD keys );
	virtual bool 			OnMouseRBUp	( int mX, int mY );
    virtual bool 			OnMouseLBUp	( int mX, int mY );

	DECLARE_SCLASS( ReflectionDebugger, Dialog, RDBG );
}; // class ReflectionDebugger



#endif // __DBREFLECTION_H__
