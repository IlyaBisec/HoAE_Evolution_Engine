/*****************************************************************************/
/*	File:	sgHotkey.h
/*	Desc:	Hotkey commands management
/*	Author:	Ruslan Shestopalyuk
/*	Date:	11-24-2003
/*****************************************************************************/
#ifndef __SGHOTKEY_H__
#define __SGHOTKEY_H__

namespace sg{
/*****************************************************************************/
/*	Class:	HotkeyManager
/*	Desc:	Gathers input from the user and forwards it to the given set of
/*				dialogs/commands
/*****************************************************************************/
class HotkeyManager : public Node, public InputDispatcher
{
public:
	
	virtual void		RegisterCommands	(){}

	virtual bool 	OnMouseMButtonDblclk( int mX, int mY );				
	virtual bool	OnKeyDown			( DWORD keyCode, DWORD flags );	
	virtual bool	OnChar				( DWORD charCode, DWORD flags );	
	virtual bool	OnKeyUp				( DWORD keyCode, DWORD flags );	

	NODE(HotkeyManager,Node,HOTK);
protected:

}; // class HotkeyManager

}; // namespace sg

#endif // __SGHOTKEY_H__