/*****************************************************************************/
/*    File:    uiMainFrame.h
/*    Desc:    Frame windows
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#ifndef __UIMAINFRAME_H__
#define __UIMAINFRAME_H__

#include "uiFrameWindow.h"

/*****************************************************************************/
/*    Class:    MainFrame
/*    Desc:    Root top-level frame window, entry point to the GUI system
/*****************************************************************************/    
class MainFrame : public FrameWindow
{
public:
                            MainFrame   ();
    virtual void            Render      ();
    virtual bool             OnMouseMove    ( int mX, int mY, DWORD keys );
    virtual bool             OnMouseRBUp    ( int mX, int mY );
    virtual bool             OnMouseMBUp    ( int mX, int mY );
    virtual bool             OnMouseLBUp    ( int mX, int mY );
    virtual void            OnDrop      ( int mX, int mY, IReflected* pDropped );
    Widget*                 PickWidget  ( int mX, int mY );
    
    bool                    KeyDown     ( DWORD keyCode, DWORD flags );    
    bool                    Char        ( DWORD charCode, DWORD flags );    
    bool                    KeyUp       ( DWORD keyCode, DWORD flags );    

    static MainFrame&       instance()
    {
        static MainFrame me;
        return me;
    }

    DECLARE_CLASS(MainFrame);
}; // class MainFrame

#endif // __UIMAINFRAME_H__