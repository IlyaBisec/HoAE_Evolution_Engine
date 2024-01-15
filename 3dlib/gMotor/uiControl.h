/*****************************************************************************/
/*    File:    uiControl.h
/*    Desc:    Scene graph UI controls
/*    Author:    Ruslan Shestopalyuk
/*    Date:    08-20-2003
/*****************************************************************************/
#ifndef __UICONTROL_H__
#define __UICONTROL_H__

#include "IInput.h"



class Font;
/*****************************************************************************/
/*    Class:    Control
/*    Desc:    Base UI control class
/*****************************************************************************/
class Control : public HudNode, public IInputDispatcher
{
public:
                        Control();
    virtual                ~Control();

    void                Render();
    virtual void        RenderNonClientArea(); 

    Rct                    GetExtents        () const { return Rct( pos.x, pos.y, width, height ); }
    void                SetExtents        ( float _x, float _y, float _w, float _h );
    void                SetExtents        ( const Rct& rct );

    _inl DWORD            GetClrTop        () const { return m_ClrTop; }
    _inl DWORD            GetClrMdl        () const { return m_ClrMdl; }
    _inl DWORD            GetClrBot        () const { return m_ClrBot; }

    _inl void            SetClrTop        ( DWORD val ) { m_ClrTop = val; }
    _inl void            SetClrMdl        ( DWORD val ) { m_ClrMdl = val; }
    _inl void            SetClrBot        ( DWORD val ) { m_ClrBot = val; }

    virtual    void        Expose            ( PropertyMap& pm );
    virtual void        Serialize        ( OutStream& os ) const;
    virtual void        Unserialize        ( InStream& is );

    virtual void        OnDrop            ( int mX, int mY, DWORD ctx, DWORD obj ){}

    Rct                    ClientToScreen    ( const Rct& rct ) const;
    Rct                    ScreenToClient    ( const Rct& rct ) const;

    void                ClientToScreen    ( float& x, float& y ) const;
    void                ScreenToClient    ( float& x, float& y ) const;

    virtual _inl Rct    GetClientRect    () const { return GetExtents(); }
    void                BeginDrag        ( float mx, float my );
    void                OnDrag            ( float mx, float my );
    void                EndDrag            ( float mx, float my );
    bool                IsDragged        () const { return m_bDragged; }
    void                EnableDrag        ( bool val = true ) { m_bEnableDrag = val; } 

    _inl DWORD            GetFgColor        () const { return m_ClrFg; }
    _inl void            SetFgColor        ( DWORD val ) { m_ClrFg = val; }

    DECLARE_SCLASS(Control,HudNode,CTRL);

protected:
    DWORD                m_ClrTop;
    DWORD                m_ClrMdl;
    DWORD                m_ClrBot;
    DWORD                m_ClrFg;

    Rct                    m_ClientRct;
    
    float                m_DragX, m_DragY;
    bool                m_bDragged;
    bool                m_bEnableDrag;

private:

}; // class Control

/*****************************************************************************/
/*    Class:    Dialog
/*    Desc:    UI dialog class
/*****************************************************************************/
class Dialog : public Control
{
public:
                        Dialog();
    virtual                ~Dialog();

    virtual _inl Rct    GetClientRect            () const;
    _inl Rct            GetHeaderRect            () const;

    virtual void        RenderNonClientArea        (); 
    virtual bool         OnMouseLBDown        ( int mX, int mY );
    virtual bool         OnMouseMove                ( int mX, int mY, DWORD keys );
    virtual bool         OnMouseLBUp        ( int mX, int mY );

    virtual bool        OnMouseMBDown        ( int mX, int mY );
    virtual bool        OnMouseMBUp        ( int mX, int mY );
    void                Expose                    ( PropertyMap& pm );

    DECLARE_SCLASS(Dialog,Control,DLOG);

protected:
    int                    m_HeaderHeight;    
    DWORD                m_ClrHeader;    
    float                m_BorderWidth;
    float                m_BorderHeight;
    
    c2::string            m_FontName;
    int                    m_FontHeight;

private:
    Font*                m_pFont;

}; // class Dialog

#include "vCamera.h"
/*****************************************************************************/
/*    Class:    PushButton
/*    Desc:    
/*****************************************************************************/
class PushButton : public Control
{
public:
    enum State
    {
        bsUnknown    = 0,
        bsIdle        = 1,
        bsPressed    = 2
    };
    PushButton() : m_State( bsIdle ) {}
    void                Render();
    void                SetText( const char* txt ) { m_Text = txt; }

    virtual bool        OnMouseLBDown( int mX, int mY );
    virtual bool        OnMouseLBUp( int mX, int mY );

    DECLARE_SCLASS(PushButton,Control,BUTN);

private:
    c2::string            m_Text;
    State                m_State;

}; // class PushButton 

/*****************************************************************************/
/*    Class:    CheckBoxButton
/*****************************************************************************/
class CheckBoxButton : public Control
{
    bool                m_bChecked;

public:

    void                Render();

    DECLARE_SCLASS(CheckBoxButton,Control,CHBX);

}; // class CheckBoxButton 

/*****************************************************************************/
/*    Class:    SGEditBox
/*    Desc:    Input text control
/*****************************************************************************/
class SGEditBox : public Control
{
    c2::string            m_Text;

    float                m_TextViewPos;
    int                    m_CaretPos;
    DWORD                m_ClrText;
    DWORD                m_ClrCaret;

    DWORD                m_CaretBlinkOn;
    DWORD                m_CaretBlinkOff;

public:
    SGEditBox();

    void                Render();
    void                SetText( const char* txt ) { m_Text = txt; }
    const char*            GetText() const { return m_Text; }

    DECLARE_SCLASS(SGEditBox,Control,EDIT);

protected:
    void                RenderCaret();

}; // class SGEditBox 



#endif // __UICONTROL_H__