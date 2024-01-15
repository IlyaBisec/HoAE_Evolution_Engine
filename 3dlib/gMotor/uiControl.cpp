/*****************************************************************************/
/*    File:    uiControl.cpp
/*    Desc:    Scene graph UI controls
/*    Author:    Ruslan Shestopalyuk
/*    Date:    08-20-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include "mMath2D.h"
#include "uiControl.h"
#include "IMediaManager.h"
#include "IEffectManager.h"

IMPLEMENT_CLASS( Control                );
IMPLEMENT_CLASS( Dialog                 );
IMPLEMENT_CLASS( PushButton             );
IMPLEMENT_CLASS( CheckBoxButton         );


/*****************************************************************************/
/*    Control implementation
/*****************************************************************************/
Control::Control()
{
    m_ClrTop        = 0xFFFFFFFF;
    m_ClrMdl        = 0xFFD6D3CE;
    m_ClrBot        = 0xFF848284;
    m_ClrFg         = 0xFF000000;

    m_bDragged      = false;
    m_bEnableDrag   = true;
}

Control::~Control()
{
}

void Control::SetExtents( float _x, float _y, float _w, float _h )
{
    pos.x = _x;
    pos.y = _y;
    width = _w;
    height= _h;
}

void Control::SetExtents( const Rct& rct )
{
    pos.x = rct.x;
    pos.y = rct.y;
    width = rct.w;
    height= rct.h;
}

void Control::RenderNonClientArea()
{    
    rsPanel( GetExtents(), GetPosZ(), m_ClrTop, m_ClrMdl, m_ClrBot );
} // Dialog::RenderNonClientArea

void Control::Render()
{
    RenderNonClientArea();
    SNode::Render();
} // Control::Render

void Control::Serialize    ( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_ClrTop << m_ClrMdl << m_ClrBot << m_bEnableDrag; 
}

void Control::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
    is >> m_ClrTop >> m_ClrMdl >> m_ClrBot >> m_bEnableDrag; 
}

Rct Control::ClientToScreen( const Rct& rct ) const
{
    Rct rc = GetClientRect();
    return Rct( rct.x + rc.x, rct.y + rc.y, rct.w * scale, rct.h * scale );
}

Rct Control::ScreenToClient( const Rct& rct ) const
{
    Rct rc = GetClientRect();
    return Rct( rct.x - rc.x, rct.y - rc.y, rct.w / scale, rct.h / scale );
}

void Control::ClientToScreen( float& x, float& y ) const
{
    Rct rc = GetClientRect();
    x += rc.x; y += rc.y;
}

void Control::ScreenToClient( float& x, float& y ) const
{
    Rct rc = GetClientRect();
    x -= rc.x; y -= rc.y;
}

void Control::BeginDrag    ( float mx, float my )
{
    if (!m_bEnableDrag) return;
    m_DragX = mx;
    m_DragY = my;
    m_bDragged = true;
}

void Control::OnDrag( float mx, float my )
{
    if (!m_bDragged) return;
    pos.x += mx - m_DragX;
    pos.y += my - m_DragY;
    m_DragX = mx;
    m_DragY = my;
}

void Control::EndDrag( float mx, float my )
{
    m_DragX = mx;
    m_DragY = my;
    m_bDragged = false;
}

void Control::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "Control", this );
    pm.f( "BgTop",        m_ClrTop, "color"    );
    pm.f( "BgMdl",        m_ClrMdl, "color"    );
    pm.f( "MgBottom",    m_ClrBot, "color"    );
    pm.f( "Foreground",    m_ClrFg,  "color"    );
} // Control::Expose


/*****************************************************************************/
/*    Dialog implementation
/*****************************************************************************/
Dialog::Dialog()
{
    m_BorderWidth        = 1.0f;
    m_BorderHeight        = 1.0f;
    m_HeaderHeight        = 0.0f;
    m_ClrHeader            = 0xFF000055;
}

Dialog::~Dialog()
{
}

_inl Rct Dialog::GetClientRect() const
{
    Rct ext = GetExtents();
    ext.Inflate( m_HeaderHeight + m_BorderHeight*2.0f, m_BorderWidth, m_BorderHeight, m_BorderWidth );
    return ext;
}

void Dialog::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "Dialog", this );
    pm.f( "BorderWidth",    m_BorderWidth            );
    pm.f( "BorderHeight",    m_BorderHeight            );
    pm.f( "HeaderColor",    m_ClrHeader, "color"    );
    pm.f( "HeaderHeight",    m_HeaderHeight            );

} // Dialog::Expose

void Dialog::RenderNonClientArea()
{
    
    rsPanel( GetExtents(), GetPosZ(), m_ClrTop, m_ClrMdl, m_ClrBot );

    if (m_HeaderHeight > 0.0f)
    {
    Rct hRct = GetExtents();
    hRct.Inflate( m_BorderHeight, m_BorderWidth, 0.0f, m_BorderWidth );
    hRct.h = m_HeaderHeight;
    rsRect( hRct, GetPosZ(), m_ClrHeader );

    hRct.x += hRct.w - m_HeaderHeight;
    hRct.w = m_HeaderHeight;
    hRct.Inflate( 1.0f, 1.0f, 1.0f, 1.0f );
    rsPanel( hRct, GetPosZ(), m_ClrTop, m_ClrMdl, m_ClrBot );
    }
} // Dialog::RenderNonClientArea

bool Dialog::OnMouseLBDown( int mX, int mY )
{
    Rct hdr = GetHeaderRect(); 
    if (hdr.PtIn( mX, mY )) 
    {
        BeginDrag( mX, mY );
        return true;
    }
    
    float mx = mX, my = mY;
    //ScreenToClient( mx, my );
    Iterator it( this );
    while (it)
    {
        SNode* pNode = (SNode*)*it;
        if (pNode->HasFn( Control::Magic() ))
        {
            Control* pCtrl = (Control*) pNode;
            if (pCtrl != this && pCtrl->GetExtents().PtIn( mx, my ))
            {
                pCtrl->MouseLBDown( mX, mY );
            }
        }
        ++it;
    }

    return false;
} // Dialog::OnMouseLBDown

bool Dialog::OnMouseMBDown( int mX, int mY )
{
    Rct hdr = GetExtents(); 
    if (hdr.PtIn( mX, mY ) && 
        (GetKeyState( VK_CONTROL ) >= 0) &&
        (GetKeyState( VK_SHIFT     ) >= 0) &&
        (GetKeyState( VK_MENU     ) >= 0)
        )
    {
        BeginDrag( mX, mY );
        return true;
    }
    return false;
}

bool Dialog::OnMouseMBUp( int mX, int mY )
{
    Rct hdr = GetExtents(); 
    if (hdr.PtIn( mX, mY )) 
    {
        EndDrag( mX, mY );
        return true;
    }
    return false;
}

bool Dialog::OnMouseMove( int mX, int mY, DWORD keys )
{
    if (IsDragged()) 
    {
        float mx = mX;
        float my = mY;
        OnDrag( mx, my );
        return true;
    }

    float mx = mX, my = mY;
    ScreenToClient( mx, my );
    Iterator it( this );
    while (it)
    {
        SNode* pNode = (SNode*)*it;
        if (pNode->HasFn( Control::Magic() ))
        {
            Control* pCtrl = (Control*) pNode;
            if (pCtrl != this && pCtrl->GetExtents().PtIn( mx, my ))
            {
                pCtrl->MouseMove( mX, mY, keys );
            }
        }
        ++it;
    }

    return false;
} // Dialog::OnMouseMove

bool Dialog::OnMouseLBUp( int mX, int mY )
{
    if (IsDragged()) 
    {
        float mx = mX;
        float my = mY;
        EndDrag( mx, my );
        return true;
    }

    float mx = mX, my = mY;
    //ScreenToClient( mx, my );
    Iterator it( this );
    while (it)
    {
        SNode* pNode = (SNode*)*it;
        if (pNode->IsA<Control>())
        {
            Control* pCtrl = (Control*) pNode;
            if (pCtrl != this && pCtrl->GetExtents().PtIn( mx, my ))
            {
                pCtrl->MouseLBUp( mX, mY );
            }
        }
        ++it;
    }

    return false;
} // Dialog::OnMouseLBUp

Rct Dialog::GetHeaderRect() const
{
    Rct rct( GetExtents() );
    rct.h = m_HeaderHeight;
    return rct;
} // Dialog::GetHeaderRect

/*****************************************************************************/
/*    PushButton implementation
/*****************************************************************************/
void PushButton::Render()
{
    Rct rct = GetExtents();
    if (m_State == bsPressed) 
    {    
        rct.x += 0; rct.y += 1;
        SetExtents( rct );
    }

    Control::Render();

    float strW = GetTextWidth( m_Text.c_str() );
    Vector3D pos( rct.x + 1, rct.y, GetPosZ() );

    pos.x = tmax( pos.x, rct.GetCenterX() - strW * 0.5f );
    DrawText( pos.x, pos.y, m_ClrFg, m_Text.c_str() );

    if (m_State == bsPressed) 
    {    
        rct.x -= 0; rct.y -= 1;
        SetExtents( rct );
    }

} // PushButton::Render

bool PushButton::OnMouseLBDown( int mX, int mY )
{
    m_State = bsPressed;
    return false;
}

bool PushButton::OnMouseLBUp( int mX, int mY )
{
    m_State = bsIdle;
    return false;
}

/*****************************************************************************/
/*    CheckBoxPushButton implementation
/*****************************************************************************/
void CheckBoxButton::Render()
{
    Control::Render();
}

/*****************************************************************************/
/*    SGEditBox implementation
/*****************************************************************************/
SGEditBox::SGEditBox()
{
    m_ClrTop          = 0xFF848284;
    m_ClrMdl          = 0xFFFFFFFF;
    m_ClrBot          = 0xFFD6D3CE;
    m_ClrText         = 0xFF000008;
    m_ClrCaret        = 0xFF080000;

    m_CaretBlinkOn    = 800;
    m_CaretBlinkOff = 500;
    m_TextViewPos    = 0.0f;

    m_CaretPos        = 0;
}

void SGEditBox::Render()
{
    Control::Render();
    rsFlushPoly2D();
        
    Rct ext = GetExtents();
    Vector3D pos( ext.x - m_TextViewPos, ext.y, GetPosZ() );

    Rct vp = IRS->GetViewPort();
    IRS->SetViewPort( ext );
    
    DrawText( pos.x, pos.y, m_ClrText, m_Text );
    RenderCaret();
    rsFlushLines2D();
    
    IRS->SetViewPort( vp );
} // SGEditBox::Render

void SGEditBox::RenderCaret()
{
    DWORD tc = GetTickCount();
    Rct ext = GetExtents();
    tc %= m_CaretBlinkOn + m_CaretBlinkOff;
    if (tc < m_CaretBlinkOn)
    {
        m_CaretPos = 2;
        char c = m_Text[m_CaretPos];
        m_Text[m_CaretPos] = 0;
        float cpos = GetTextWidth( m_Text, m_CaretPos ) - m_TextViewPos;
        m_Text[m_CaretPos] = c;
        rsLine( ext.x + cpos, ext.y, ext.x + cpos, ext.GetBottom(), m_ClrCaret, m_ClrCaret );
        rsFlushLines2D();
    }
} // SGEditBox::RenderCaret
