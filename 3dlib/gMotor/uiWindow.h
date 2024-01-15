/*****************************************************************************/
/*    File:    uiWindow.h
/*    Desc:    Window control
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#ifndef __UIWINDOW_H__
#define __UIWINDOW_H__

#include "uiWidget.h"
/*****************************************************************************/
/*    Class:    Window
/*    Desc:    General window control
/*****************************************************************************/    
class Window : public Widget
{
    bool                m_bDrawBackground;
public:
                        Window();
    virtual void        Render();
    virtual void        Expose            ( PropertyMap& pm );

    virtual bool        OnMouseMBUp     ( int mX, int mY );
    virtual bool        OnMouseMBDown   ( int mX, int mY );

    DECLARE_CLASS(Window);
}; // class Window

#endif // __UIWINDOW_H__