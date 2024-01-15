/*****************************************************************************/
/*    File:    uiCheckBox.cpp
/*    Desc:    CheckButton control
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#include "stdafx.h"
#include "uiWidget.h"
#include "uiCheckBox.h"

/*****************************************************************************/
/*  CheckButton implementation
/*****************************************************************************/
IMPLEMENT_CLASS(CheckButton);
CheckButton::CheckButton() : m_bChecked(false)
{
    m_ThumbWidth = m_ThumbHeight = 12;
    SetFocusable( false );
} // CheckButton::CheckButton

void CheckButton::Expose( PropertyMap& pm )
{
    pm.start<Widget>( "CheckButton", this );
    pm.f( "Checked",    m_bChecked      );    
    pm.f( "ThumbW",     m_ThumbWidth    );
    pm.f( "ThumbH",     m_ThumbHeight   );
    pm.f( "ShowText",   m_bShowText     );
    pm.m( "Toggle", &CheckButton::Toggle          );
    pm.m( "Check", &CheckButton::Check           );
    pm.m( "Uncheck", &CheckButton::Uncheck         );
    pm.s( "checked",    sigChecked      );
    pm.s( "unchecked",  sigUnchecked    );
} // Widget::Expose

bool CheckButton::OnMouseLBDown( int mX, int mY )
{
    if (!PtIn( mX, mY )) return false;
    Toggle();
    return true;
} // CheckButton::OnMouseLBDown

void CheckButton::Toggle() 
{ 
    m_bChecked = !m_bChecked; 
    if (m_bChecked) sigChecked(); else sigUnchecked();
} // CheckButton::Toggle

void CheckButton::Check() 
{ 
   if (!m_bChecked) 
   {
       m_bChecked = true; 
       sigChecked();
       return;
   }
   m_bChecked = true; 
} // CheckButton::Check

void CheckButton::Uncheck() 
{ 
    if (m_bChecked) 
    {
        m_bChecked = false; 
        sigUnchecked();
        return;
    }
    m_bChecked = false; 
} // CheckButton::Uncheck

void CheckButton::Render()
{
    Rct ext = GetScreenExtents();
    Rct rct;
    rct.x = ext.x;
    rct.w = m_ThumbWidth;
    rct.h = m_ThumbHeight;
    rct.y = ext.y + ext.h*0.5f - m_ThumbHeight*0.5f;

    DWORD bgColor = IsEnabled() ? 0xFFEEEEEE : 0xFFAAAAAA;
    rsInnerPanel( rct, 0.0f, m_BgColor );

    rct.Inflate( 2, 2, 2, 2 );
    if (m_bChecked)
    {
        DWORD checkColor = IsEnabled() ? 0xFF000000 : 0xFF777777;
        rsThickPanel( rct, 0.0f, checkColor );    
    }

    if (m_Text.size() > 0 && m_bShowText)
    {
        float strW = GetTextWidth( m_Text.c_str() );
        Vector3D pos( rct.GetRight() + 3, rct.y + 1, 0.0f );
        DrawText( pos.x, pos.y - 1, m_TextColor, m_Text.c_str() );
    }

    rsFlush();
    FlushText();
} // CheckButton::Render
