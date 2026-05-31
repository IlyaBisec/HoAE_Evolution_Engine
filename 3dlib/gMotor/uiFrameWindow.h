/*****************************************************************************/
/*    File:    uiFrameWindow.h
/*    Desc:    Frame windows
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#ifndef __UIFRAMEWINDOW_H__
#define __UIFRAMEWINDOW_H__

#include "uiWindow.h"

/*****************************************************************************/
/*    Class:    FrameWindow
/*    Desc:    Window, which contains another windows, managing their z-order etc.
/*****************************************************************************/    
class FrameWindow : public Window
{
public:
    FrameWindow();
    virtual void            Render();

    DECLARE_CLASS(FrameWindow);
}; // class FrameWindow

#endif // __UIFRAMEWINDOW_H__