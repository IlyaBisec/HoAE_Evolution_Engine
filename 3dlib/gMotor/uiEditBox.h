/*****************************************************************************/
/*    File:    uiEditBox.h
/*    Desc:    EditBox control
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#ifndef __UIEDITBOX_H__
#define __UIEDITBOX_H__

/*****************************************************************************/
/*    Class:    EditBox
/*    Desc:     Editbox control
/*****************************************************************************/    
class EditBox : public Widget
{
    DWORD           m_FocusedColor;
    bool            m_bEditable;

    int             m_CaretPos;

    int             m_SelStart;
    int             m_SelEnd;
    DWORD           m_SelColor;

    DWORD           m_ClrCaret;

    DWORD           m_CaretBlinkOn;
    DWORD           m_CaretBlinkOff;
    static DWORD    s_BlinkStart;

    void            RenderCaret     ();
    void            ResetBlink      () { s_BlinkStart = GetTickCount(); }
public:
                    EditBox         ();
    virtual void    Render          ();
    virtual void    Expose          ( PropertyMap& pm );
    
    virtual bool    OnKeyDown       ( DWORD keyCode, DWORD flags );
    virtual bool    OnChar          ( DWORD charCode, DWORD flags );
    virtual bool    OnMouseLBDown   ( int mX, int mY );
    virtual bool    OnMouseLBDbl    ( int mX, int mY );
    virtual bool    OnMouseLBUp     ( int mX, int mY );
    virtual bool    OnMouseMove     ( int mX, int mY, DWORD keys );

    void            SetEditable     ( bool bVal ) { m_bEditable = bVal; }
    virtual bool    OnFocus         ();

    DECLARE_CLASS(EditBox);
}; // class EditBox

#endif // __UIEDITBOX_H__