/*****************************************************************
/*  File:   dbLight.cpp
/*  Author: Silver, Copyright (C) GSC Game World                 
/*****************************************************************/
#include "stdafx.h"
#include "uiControl.h"
#include "dbLight.h"



/*****************************************************************/
/*	LightDebugger implementation
/*****************************************************************/
LightDebugger::LightDebugger()
{
    m_bDrawScene = true;
} // LightDebugger::LightDebugger

bool LightDebugger::OnMouseLBUp( int mX, int mY )
{
    return false;
}

bool LightDebugger::OnMouseLBDown( int mX, int mY )
{
	return false;
} // LightDebugger::OnMouseLBDown


bool LightDebugger::OnMouseMove( int mX, int mY, DWORD keys )
{
	return false;
} // LightDebugger::OnMouseMove

bool LightDebugger::OnMouseRBUp( int mX, int mY )
{
	return false;
} // LightDebugger::OnMouseRBUp

void LightDebugger::Expose( PropertyMap& pm )
{
	pm.start( "LightDebugger", this );
    pm.f( "DrawScene", m_bDrawScene );
} // LightDebugger::Expose

void LightDebugger::Render()
{
    if (m_bDrawScene)
    {
        static int mdlID = IMM->GetModelID( "Models\\lightScene.c2m" );
        static int shID = IRS->GetShaderID( "vertex_lit_notex" );
        IRS->SetShader( shID );
        IMM->AnimateModel( mdlID );
    }
} // LightDebugger::Render


