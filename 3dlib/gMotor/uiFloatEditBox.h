/*****************************************************************************/
/*    File:    uiFloatEditBox.h
/*    Desc:    FloatEditBox control
/*    Author:  Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#ifndef __UIFLOATEDITBOX_H__
#define __UIFLOATEDITBOX_H__

#include "uiEditBox.h"
#include "uiRatchet.h"

/*****************************************************************************/
/*    Class:   FloatEditBox
/*    Desc:    
/*****************************************************************************/    
class FloatEditBox : public EditBox
{
    Ratchet         m_Ratchet;
    float           m_Value;

public:
                    FloatEditBox    ();
    virtual void    OnInit          ();
    virtual void    Render          ();
    void            ChangeValue     ();
    void            ChangeText      ();
    virtual void    Expose          ( PropertyMap& pm );
    virtual bool    OnKeyDown       ( DWORD keyCode, DWORD flags );
    virtual bool    OnChar          ( DWORD charCode, DWORD flags );

    DECLARE_CLASS(FloatEditBox);
}; // class FloatEditBox

#endif // __UIFLOATEDITBOX_H__