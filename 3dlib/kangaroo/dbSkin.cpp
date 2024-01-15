/*****************************************************************
/*  File:   dbSkin.cpp
/*  Author: Silver, Copyright (C) GSC Game World                 
/*****************************************************************/
#include "stdafx.h"
#include "uiControl.h"
#include "dbSkin.h"


/*****************************************************************/
/*	SkinDebugger implementation
/*****************************************************************/
SkinDebugger::SkinDebugger()
{
    m_ModelName     = "Models\\tl.c2m";
    m_AnimName      = "Models\\tl_walk.c2m";

    m_ModelID       = -1;
    m_AnimID        = -1;    
    m_bDrawModel    = true;
} // SkinDebugger::SkinDebugger

bool SkinDebugger::OnMouseLBUp( int mX, int mY )
{
    return false;
}

bool SkinDebugger::OnMouseLBDown( int mX, int mY )
{
	return false;
} // SkinDebugger::OnMouseLBDown


bool SkinDebugger::OnMouseMove( int mX, int mY, DWORD keys )
{
	return false;
} // SkinDebugger::OnMouseMove

bool SkinDebugger::OnMouseRBUp( int mX, int mY )
{
	return false;
} // SkinDebugger::OnMouseRBUp

void SkinDebugger::Expose( PropertyMap& pm )
{
	pm.start( "SkinDebugger", this );
    pm.p( "ModelObject", GetModelName, SetModelName, "file|Models" );
    pm.p( "Animation", GetAnimName, SetAnimName, "file|Models" );
    pm.f( "DrawModel", m_bDrawModel );
} // SkinDebugger::Expose

void SkinDebugger::Render()
{
    if (m_ModelID == -1) m_ModelID = IModelMgr->GetModelID( m_ModelName.c_str() );
    if (m_AnimID  == -1) m_AnimID  = IModelMgr->GetAnimID( m_AnimName.c_str() );

    
    float t = GetTickCount();
    t = fmodf( t, IMM->GetAnimTime( m_AnimID ) );

} // SkinDebugger::Render


