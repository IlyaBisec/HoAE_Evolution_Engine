/*****************************************************************
/*  File:   edDebuggers.cpp
/*  Author: Silver, Copyright (C) GSC Game World                 
/*****************************************************************/
#include "stdafx.h"
#include "uiControl.h"
#include "dbDebugger.h"
#include "dbCurrent.h"
#include "dbLodder.h"
#include "dbAnimation.h"
#include "dbMath.h"
#include "dbReflection.h"
#include "dbSkin.h"
#include "dbGui.h"



IMPLEMENT_CLASS( VIPMDebugger       );
IMPLEMENT_CLASS( LODDebugger        );
IMPLEMENT_CLASS( ReflectionDebugger );
IMPLEMENT_CLASS( AnimDebugger       );
IMPLEMENT_CLASS( CurDebugger        );
IMPLEMENT_CLASS( GuiDebugger        );

/*****************************************************************/
/*	Debugger implementation
/*****************************************************************/
Debugger::Debugger()
{
    m_CurDebugger = dbGui;

    AddChild<CurDebugger        > ( "Current"             );
    AddChild<LODDebugger        > ( "LOD"                 );
    AddChild<AnimDebugger       > ( "Animation"           );
    AddChild<MathDebugger       > ( "Math"                );
    AddChild<ReflectionDebugger > ( "ReflectionDebugger"  );
    AddChild<VIPMDebugger       > ( "Lodder"              );
    AddChild<GuiDebugger        > ( "Gui"                 );

} // Debugger::Debugger

bool Debugger::OnMouseLBUp( int mX, int mY )
{
    Dialog* pCurDbg = (Dialog*)GetChild( (int)m_CurDebugger );
    if (pCurDbg) return pCurDbg->OnMouseLBUp( mX, mY );
    return false;
} // Debugger::OnMouseLBUp

bool Debugger::OnMouseLBDown( int mX, int mY )
{
    Dialog* pCurDbg = (Dialog*)GetChild( (int)m_CurDebugger );
    if (pCurDbg) return pCurDbg->OnMouseLBDown( mX, mY );
	return false;
} // Debugger::OnMouseLBDown

bool Debugger::OnMouseMove( int mX, int mY, DWORD keys )
{
    Dialog* pCurDbg = (Dialog*)GetChild( (int)m_CurDebugger );
    if (pCurDbg) return pCurDbg->OnMouseMove( mX, mY, keys );
	return false;
} // Debugger::OnMouseMove

bool Debugger::OnMouseRBUp( int mX, int mY )
{
    Dialog* pCurDbg = (Dialog*)GetChild( (int)m_CurDebugger );
    if (pCurDbg) return pCurDbg->MouseRBUp( mX, mY );
	return false;
} // Debugger::OnMouseRBUp

void Debugger::Expose( PropertyMap& pm )
{
	pm.start( "Debugger", this );
    pm.f( "CurDebugger", m_CurDebugger );
 } // Debugger::Expose

void Debugger::Render()
{
    Dialog* pCurDbg = (Dialog*)GetChild( (int)m_CurDebugger );
    if (pCurDbg) pCurDbg->Render();	
} // Debugger::Render


