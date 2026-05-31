/*****************************************************************************/
/*	File:	sgHotkey.cpp
/*	Desc:	Hotkey management implementation
/*	Author:	Ruslan Shestopalyuk
/*	Date:	11-24-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include "sgNodePool.h"
#include "sgNode.h"
#include "kInput.h"
#include "kCommand.h"
#include "sgHotkey.h"

BEGIN_NAMESPACE(sg)
/*****************************************************************************/
/*	HotkeyManager implementation
/*****************************************************************************/
bool HotkeyManager::OnMouseMButtonDblclk( int mX, int mY )
{
	return false;
}

bool HotkeyManager::OnKeyDown( DWORD keyCode, DWORD flags )
{
	return false;
}

bool HotkeyManager::OnChar( DWORD charCode, DWORD flags )
{
	return false;
}

bool HotkeyManager::OnKeyUp( DWORD keyCode, DWORD flags )
{
	return false;
}

END_NAMESPACE(sg)
