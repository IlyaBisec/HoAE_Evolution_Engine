/*****************************************************************
/*  File:   dbSkin.h
/*  Author: Silver, Copyright (C) GSC Game World                 
/*****************************************************************/
#ifndef __DBSKIN_H__
#define __DBSKIN_H__

/*****************************************************************/
/*	Class:	SkinDebugger
/*	Desc:	Different temporary things debugger
/*****************************************************************/
class SkinDebugger : public Dialog
{
    std::string             m_ModelName;
    std::string             m_AnimName;

    int                     m_ModelID;
    int                     m_AnimID;

    bool                    m_bDrawModel;

public:
							SkinDebugger		();
	virtual void			Render				();
	virtual void			Expose				( PropertyMap& pm );
	virtual bool 			OnMouseLBDown	( int mX, int mY );
	virtual bool 			OnMouseMove			( int mX, int mY, DWORD keys );
	virtual bool 			OnMouseRBUp	( int mX, int mY );
    virtual bool 			OnMouseLBUp	( int mX, int mY );

    const char*             GetModelName        () const { return m_ModelName.c_str(); }
    void                    SetModelName        ( const char* name ) { m_ModelName = name; }

    const char*             GetAnimName         () const { return m_ModelName.c_str(); }
    void                    SetAnimName         ( const char* name ) { m_ModelName = name; }

	DECLARE_SCLASS( SkinDebugger, Dialog, SKBG );
}; // class SkinDebugger



#endif // __DBSKIN_H__
