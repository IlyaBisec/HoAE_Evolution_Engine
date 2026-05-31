/*****************************************************************************/
/*    File:    uiIntEditBox.cpp
/*    Desc:    IntEditBox control
/*    Author:  Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#include "stdafx.h"
#include "uiWidget.h"
#include "uiIntEditBox.h"

/*****************************************************************************/
/*  IntEditBox implementation
/*****************************************************************************/
IMPLEMENT_CLASS(IntEditBox);
IntEditBox::IntEditBox() : m_Value(0)
{
     AddChild( m_Ratchet );
     m_Ratchet.sigChanged.Connect( this, &IntEditBox::ChangeValue );
     sigTextChanged.Connect( this, &IntEditBox::ChangeText );
} // IntEditBox::IntEditBox

void IntEditBox::OnInit()
{
    SetText( ToString( m_Value ) );
} // IntEditBox::Render

void IntEditBox::Expose( PropertyMap& pm )
{
    pm.start<EditBox>( "IntEditBox", this );
    pm.f( "Value", m_Value );
} // IntEditBox::Expose

void IntEditBox::Render()
{
    Rct ext = GetScreenExtents();
    ext.x = ext.w - m_Ratchet.GetRatchetWidth();
    ext.y = 1.0f;
    m_Ratchet.SetExtents( ext );
    EditBox::Render();
} // IntEditBox::Render

bool IntEditBox::OnKeyDown( DWORD keyCode, DWORD flags )
{
    int mult = 1;
    for (int i = '1'; i < '9'; i++)
    {
        if (GetKeyState( i ) < 0)
        {
            mult = pow( 10, i - '1' + 1 );
        }
    }
    
    if (keyCode == VK_LEFT)
    {
        int delta = -mult;
        m_Value += delta;
        SetText( ToString( m_Value ) );
        return true;
    }
    if (keyCode == VK_RIGHT)
    {
        int delta = mult;
        m_Value += delta;
        SetText( ToString( m_Value ) );
        return true;
    }
    return EditBox::OnKeyDown( keyCode, flags );
} // IntEditBox::OnKeyDown

bool IntEditBox::OnChar( DWORD charCode, DWORD flags )
{
    return false;
} // IntEditBox::OnChar


void IntEditBox::ChangeValue()
{
    int pos = m_Ratchet.GetPosition();
    int prevPos = m_Ratchet.GetPrevPosition();
    int mult = 1;
    for (int i = '1'; i < '9'; i++)
    {
        if (GetKeyState( i ) < 0)
        {
            mult = pow( 10, i - '1' + 1 );
        }
    }
    int delta = (pos - prevPos)*mult;
    m_Value += delta;
    SetText( ToString( m_Value ) );
} // IntEditBox::ChangeValue

void IntEditBox::ChangeText()
{
    int val = 0; 
    if (TypeTraits<int>::FromString( val, GetText() )) m_Value = val;
} // IntEditBox::ChangeText
