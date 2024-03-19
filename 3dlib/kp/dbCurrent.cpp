/*****************************************************************
/*  File:   dbCurrent.cpp
/*  Author: Silver, Copyright (C) GSC Game World                 
/*****************************************************************/
#include "stdafx.h"
#include "uiControl.h"
#include "dbCurrent.h"
#include "kDB.h"



/*****************************************************************/
/*	CurDebugger implementation
/*****************************************************************/
CurDebugger::CurDebugger()
{
} // CurDebugger::CurDebugger

bool CurDebugger::OnMouseLBUp( int mX, int mY )
{
    return false;
}

bool CurDebugger::OnMouseLBDown( int mX, int mY )
{
	return false;
} // CurDebugger::OnMouseLBDown


bool CurDebugger::OnMouseMove( int mX, int mY, DWORD keys )
{
	return false;
} // CurDebugger::OnMouseMove

bool CurDebugger::OnMouseRBUp( int mX, int mY )
{
	return false;
} // CurDebugger::OnMouseRBUp

void CurDebugger::Expose( PropertyMap& pm )
{
	pm.start( "CurDebugger", this );
} // CurDebugger::Expose

void CurDebugger::Render()
{
} // CurDebugger::Render


