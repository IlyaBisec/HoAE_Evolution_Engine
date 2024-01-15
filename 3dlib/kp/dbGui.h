/*****************************************************************
/*  File:   dbGui.h
/*  Author: Silver, Copyright (C) GSC Game World                 
/*****************************************************************/
#ifndef __DBGUI_H__
#define __DBGUI_H__


/*****************************************************************/
/*	Class:	GuiDebugger
/*	Desc:	Debug gui 
/*****************************************************************/
class GuiDebugger : public Dialog
{
public:
							GuiDebugger		();
	virtual void			Render			();
	virtual void			Expose			( PropertyMap& pm );

	DECLARE_SCLASS( GuiDebugger, Dialog, GUBG );
}; // class GuiDebugger



#endif // __DBGUI_H__
