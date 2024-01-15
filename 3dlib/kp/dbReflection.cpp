/*****************************************************************
/*  File:   dbReflection.cpp
/*  Author: Silver, Copyright (C) GSC Game World                 
/*****************************************************************/
#include "stdafx.h"
#include "uiControl.h"
#include "dbReflection.h"



/*****************************************************************/
/*	ReflectionDebugger implementation
/*****************************************************************/
ReflectionDebugger::ReflectionDebugger()
{
    m_bDrawScene = true;
} // ReflectionDebugger::ReflectionDebugger

bool ReflectionDebugger::OnMouseLBUp( int mX, int mY )
{
    return false;
}

bool ReflectionDebugger::OnMouseLBDown( int mX, int mY )
{
	return false;
} // ReflectionDebugger::OnMouseLBDown


bool ReflectionDebugger::OnMouseMove( int mX, int mY, DWORD keys )
{
	return false;
} // ReflectionDebugger::OnMouseMove

bool ReflectionDebugger::OnMouseRBUp( int mX, int mY )
{
	return false;
} // ReflectionDebugger::OnMouseRBUp

void ReflectionDebugger::Expose( PropertyMap& pm )
{
	pm.start( "ReflectionDebugger", this );
    pm.f( "DrawScene", m_bDrawScene );
} // ReflectionDebugger::Expose

void ReflectionDebugger::Render()
{
    if (m_bDrawScene)
    {
    }
} // ReflectionDebugger::Render


