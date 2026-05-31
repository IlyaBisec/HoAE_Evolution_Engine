/*****************************************************************
/*  File:   dbAnimation.cpp
/*  Author: Silver, Copyright (C) GSC Game World                 
/*****************************************************************/
#include "stdafx.h"
#include "uiControl.h"
#include "dbAnimation.h"



/*****************************************************************/
/*	AnimDebugger implementation
/*****************************************************************/
AnimDebugger::AnimDebugger()
{
} // AnimDebugger::AnimDebugger

bool AnimDebugger::OnMouseLBUp( int mX, int mY )
{
    return false;
}

bool AnimDebugger::OnMouseLBDown( int mX, int mY )
{
	return false;
} // AnimDebugger::OnMouseLBDown


bool AnimDebugger::OnMouseMove( int mX, int mY, DWORD keys )
{
	return false;
} // AnimDebugger::OnMouseMove

bool AnimDebugger::OnMouseRBUp( int mX, int mY )
{
	return false;
} // AnimDebugger::OnMouseRBUp

void AnimDebugger::Expose( PropertyMap& pm )
{
	pm.start( "AnimDebugger", this );
} // AnimDebugger::Expose

void AnimDebugger::Render()
{
} // AnimDebugger::Render


