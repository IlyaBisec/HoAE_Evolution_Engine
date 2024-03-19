/*****************************************************************************/
/*    File:    uiLabel.h
/*    Desc:    Label control
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#ifndef __UILABEL_H__
#define __UILABEL_H__

#include "uiWidget.h"

/*****************************************************************************/
/*    Class:    Label
/*    Desc:    Extremely simple control showing piece of text
/*****************************************************************************/    
class Label : public Widget
{
    std::string     m_Caption;
public:
                    Label       ();

    void            SetCaption  ( const char* str ) { m_Caption = str; }
    const char*     GetCaption  () const { return m_Caption.c_str(); }
    virtual void    Render        ();
    virtual void    Expose      ( PropertyMap& pm );

    DECLARE_CLASS(Label);
}; // class Label

#endif // __UILABEL_H__