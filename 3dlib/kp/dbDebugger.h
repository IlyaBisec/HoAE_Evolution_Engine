/*****************************************************************
/*  File:   dbDebugger.h
/*  Author: Silver, Copyright (C) GSC Game World                 
/*****************************************************************/
#ifndef __DBDEBUGGER_H__
#define __DBDEBUGGER_H__

#include "mTriangle.h"
#include "mHeightmap.h"

enum DebuggerType
{
    dbCurrent       = 0,
    dbLodder        = 1,
    dbImpostor      = 2,
    dbAnimation     = 3,
    dbMath          = 4,
    dbLight         = 5,
    dbReflection    = 6,
    dbVIPM          = 7,
    dbSkin          = 8,
    dbSkinCache     = 9,
    dbGui           = 10,
}; // enum DebuggerType

ENUM(DebuggerType, "DebuggerType", 
                    en_val( dbCurrent,      "Current"      ) <<
                    en_val( dbLodder,       "Lodder"       ) << 
                    en_val( dbImpostor,     "Impostor"     ) <<
                    en_val( dbAnimation,    "Animation"    ) <<
                    en_val( dbMath,         "Math"         ) <<
                    en_val( dbLight,        "LightSource"        ) <<
                    en_val( dbReflection,   "Reflection"   ) <<
                    en_val( dbVIPM,         "VIPM"         ) << 
                    en_val( dbSkin,         "Skin"         ) <<
                    en_val( dbSkinCache,    "SkinCache"    ) <<
                    en_val( dbGui,          "Gui"          )
                    );


/*****************************************************************/
/*	Class:	Debugger
/*	Desc:	Different temporary things debugger
/*****************************************************************/
class Debugger : public Dialog
{
    DebuggerType            m_CurDebugger;

protected:
public:
							Debugger		    ();
	virtual void			Render				();
	virtual void			Expose				( PropertyMap& pm );
	virtual bool 			OnMouseLBDown	( int mX, int mY );
	virtual bool 			OnMouseMove			( int mX, int mY, DWORD keys );
	virtual bool 			OnMouseRBUp	( int mX, int mY );
    virtual bool 			OnMouseLBUp	( int mX, int mY );

	DECLARE_SCLASS( Debugger, Dialog, TDBG );
}; // class Debugger



#endif // __DBDEBUGGER_H__
