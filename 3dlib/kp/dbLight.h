/*****************************************************************
/*  File:   dbLight.h
/*  Author: Silver, Copyright (C) GSC Game World                 
/*****************************************************************/
#ifndef __DBLIGHT_H__
#define __DBLIGHT_H__


/*****************************************************************/
/*	Class:	LightDebugger
/*	Desc:	Debug light environment
/*****************************************************************/
class LightDebugger : public Dialog
{
    bool                    m_bDrawScene;
public:
							LightDebugger		();
	virtual void			Render				();
	virtual void			Expose				( PropertyMap& pm );
	virtual bool 			OnMouseLBDown	( int mX, int mY );
	virtual bool 			OnMouseMove			( int mX, int mY, DWORD keys );
	virtual bool 			OnMouseRBUp	( int mX, int mY );
    virtual bool 			OnMouseLBUp	( int mX, int mY );

	DECLARE_SCLASS( LightDebugger, Dialog, LDBG );
}; // class LightDebugger



#endif // __DBLIGHT_H__
