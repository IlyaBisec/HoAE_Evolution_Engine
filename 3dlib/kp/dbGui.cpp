/*****************************************************************
/*  File:   dbGui.cpp
/*  Author: Silver, Copyright (C) GSC Game World                 
/*****************************************************************/
#include "stdafx.h"
#include "uiControl.h"
#include "dbGui.h"



/*****************************************************************/
/*	GuiDebugger implementation
/*****************************************************************/
GuiDebugger::GuiDebugger()
{
} // GuiDebugger::GuiDebugger

void GuiDebugger::Expose( PropertyMap& pm )
{
	pm.start( "GuiDebugger", this );
} // GuiDebugger::Expose

void GuiDebugger::Render()
{
} // GuiDebugger::Render


