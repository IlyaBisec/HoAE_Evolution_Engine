/*****************************************************************************/
/*    File:    uiScrollBox.h
/*    Desc:    Widget with scrollable contents
/*    Author:    Ruslan Shestopalyuk
/*    Date:    30 Dec 2004
/*****************************************************************************/
#ifndef __UISCROLLBOX_H__
#define __UISCROLLBOX_H__

#include "uiWidget.h"
/*****************************************************************************/
/*    Class:    ScrollBox
/*    Desc:    Widget with scrollable contents
/*****************************************************************************/    
class ScrollBox : public Widget
{
    Rct             m_TotalExtents;

public:
                    ScrollBox();
    virtual void    Render();

    DECLARE_CLASS(ScrollBox);
}; // class ScrollBox

#endif // __UISCROLLBOX_H__