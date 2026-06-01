/*****************************************************************************/
/*    File:    uiButton.cpp
/*    Desc:    Button control
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#include "stdafx.h"
#include "uiWidget.h"
#include "uiButton.h"
#include "IScriptManager.h"

/*****************************************************************************/
/*  Button implementation
/*****************************************************************************/
IMPLEMENT_CLASS(Button);
Button::Button() : m_bDown( false )
{
    SetFocusable( false );
    m_BgColor       = 0xFFD6D3CE;
    m_SpritePack    = "";
    m_SpritePackID  = -1;
    m_SpriteID      = -1;
    m_bDrawBorder   = true;

} // Button::Button

void Button::Expose( PropertyMap& pm )
{
    pm.start<Widget>( "Button", this );
    pm.f( "Down", m_bDown );
    pm.p( "SpritePack", &Button::GetSpritePack, &Button::SetSpritePack );
    pm.f( "SpriteID",   m_SpriteID );
    pm.f( "DrawBorder", m_bDrawBorder );
    pm.s( "pressed",    sigPressed );
} // Button::Expose

bool Button::OnMouseLBDown( int mX, int mY )    
{ 
    if (!PtIn( mX, mY )) return false;
    m_bDown = true; 
    return true;
} // Button::OnMouseLBDown

bool Button::OnMouseLBUp( int mX, int mY )    
{ 
    if (!PtIn( mX, mY )) return false;
    if (m_bDown) 
    {
        sigPressed(); 
        m_bDown = false;
        return true;
    }
    return false;
} // Button::OnMouseLBUp

void Button::Render()
{
    Rct rct = GetScreenExtents();
    if (m_bDown) 
    {    
        rct.x += 0; rct.y += 1;
    }

    if (m_SpritePackID == -1 && m_SpritePack.size() > 0)
    {
        m_SpritePackID = ISM->GetPackageID( m_SpritePack.c_str() );
    }

    DWORD bgColor = m_BgColor;
    DWORD fgColor = m_TextColor;
    if (!IsEnabled())
    {
        fgColor = m_DisTextColor;
        bgColor = 0x99E6E3EE;
    }

    if (m_bDrawBorder)
    {
        rsColoredPanel( rct, 0.0f, bgColor );
        rsFlush();
    }

    if (m_SpritePackID >= 0)
    {
        int gW = ISM->GetFrameWidth( m_SpritePackID, m_SpriteID );
        int gH = ISM->GetFrameHeight( m_SpritePackID, m_SpriteID );
        int cx = rct.x + (rct.w - gW)/2;
        int cy = rct.y + (rct.h - gH)/2;
        ISM->SetCurrentDiffuse( bgColor );
        ISM->DrawSprite( m_SpritePackID, m_SpriteID, Vector3D( cx, cy, 0.0f), false, 0 );
        ISM->Flush();
    }
    
    //  button text
    float strW = GetTextWidth( m_Text.c_str() );
    float strH = GetTextHeight();
    Vector3D pos( rct.x + 1, rct.y, 0.0f );
    pos.x = tmax( pos.x, rct.GetCenterX() - strW * 0.5f );
    pos.y = tmax( pos.y, rct.GetCenterY() - strH * 0.5f );
    DrawText( pos.x, pos.y, fgColor, m_Text.c_str() );
    FlushText();

} // Button::Render

/*****************************************************************************/
/*  RadioButton implementation
/*****************************************************************************/
IMPLEMENT_CLASS(RadioButton);
RadioButton::RadioButton() 
{
} // RadioButton::RadioButton

void RadioButton::Expose( PropertyMap& pm )
{
    pm.start<Button>( "RadioButton", this );
} // RadioButton::Expose

bool RadioButton::OnMouseLBDown( int mX, int mY )    
{ 
    if (!PtIn( mX, mY )) return false;
    m_bDown = !m_bDown; 
    return true;
} // RadioButton::OnMouseLBDown

bool RadioButton::OnMouseLBUp( int mX, int mY )    
{ 
    if (!PtIn( mX, mY )) return false;
    sigPressed();
    return true;
} // RadioButton::OnMouseLBUp

void RadioButton::Render()
{
    Button::Render();
} // RadioButton::Render

/*****************************************************************************/
/*  RadioGroup implementation
/*****************************************************************************/
IMPLEMENT_CLASS(RadioGroup);
RadioGroup::RadioGroup()
{
    AlignToParent();
    m_bAllowNonePresed = true;
}

void RadioGroup::Expose( PropertyMap& pm )
{
    pm.start<Widget>( "RadioGroup", this );
    pm.f( "AllowNonePressed", m_bAllowNonePresed );
}

void RadioGroup::Render()
{
}

void RadioGroup::OnButtonPressed( RadioButton* pButton )
{
    for (int i = 0; i < NumChildren(); i++)
    {
        RadioButton* pChild = dynamic_cast<RadioButton*>( GetChild( i ) );
        if (!pChild) continue;
        if (pChild != pButton && pChild->IsDown())
        {
            pChild->Unpress();
        }
    }

    if (!pButton->IsDown() && !m_bAllowNonePresed) pButton->Press();
} // RadioGroup::OnButtonPressed

struct RadioFunctor
{
    RadioButton*    m_pButton;
    RadioGroup*     m_pGroup;

    RadioFunctor( RadioGroup* pGroup, RadioButton* pButton ) : 
    m_pGroup( pGroup ), m_pButton( pButton ) {}
    void operator()() const
    {
        m_pGroup->OnButtonPressed( m_pButton );
    }
}; // struct RadioFunctor

bool RadioGroup::AddChild( IReflected* pChild )
{
    Widget::AddChild( pChild );
    RadioButton* pButton = dynamic_cast<RadioButton*>( pChild );
    if (!pButton) return false;
    RadioFunctor func( this, pButton );
    pButton->sigPressed.Connect( func ); 
    return true;
} // RadioGroup::AddChild
