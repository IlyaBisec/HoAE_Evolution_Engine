/*****************************************************************************/
/*    File:    uiIntEditBox.h
/*    Desc:    IntEditBox control
/*    Author:  Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#ifndef __UIINTEDITBOX_H__
#define __UIINTEDITBOX_H__

#include "uiEditBox.h"
#include "uiRatchet.h"

/*****************************************************************************/
/*    Class:   IntEditBox
/*    Desc:    
/*****************************************************************************/    
class IntEditBox : public EditBox
{
    Ratchet         m_Ratchet;
    int             m_Value;

public:
                    IntEditBox      ();
    virtual void    OnInit          ();
    virtual void    Render          ();
    void            ChangeValue     ();
    void            ChangeText      ();
    bool            OnKeyDown       ( DWORD keyCode, DWORD flags );
    virtual bool    OnChar          ( DWORD charCode, DWORD flags );
    virtual void    Expose          ( PropertyMap& pm );

    DECLARE_CLASS(IntEditBox);
}; // class IntEditBox

#endif // __UIINTEDITBOX_H__