/*****************************************************************************/
/*    File:    uiFloatEditBox.cpp
/*    Desc:    FloatEditBox control
/*    Author:  Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#include "stdafx.h"
#include "uiWidget.h"
#include "uiFloatEditBox.h"

/*****************************************************************************/
/*  FloatEditBox implementation
/*****************************************************************************/
IMPLEMENT_CLASS(FloatEditBox);
FloatEditBox::FloatEditBox() : m_Value(0.0)
{
    AddChild( m_Ratchet );
    m_Ratchet.sigChanged.Connect( this, &FloatEditBox::ChangeValue );
    sigTextChanged.Connect( this, &FloatEditBox::ChangeText );
} // FloatEditBox::FloatEditBox

void FloatEditBox::OnInit()
{
    SetText( ToString( m_Value ) );
} // FloatEditBox::Render

void FloatEditBox::Expose( PropertyMap& pm )
{
    pm.start<EditBox>( "FloatEditBox", this );
    pm.f( "Value", m_Value );
} // FloatEditBox::Expose

void FloatEditBox::Render()
{
    Rct ext = GetScreenExtents();
    ext.x = ext.w - m_Ratchet.GetRatchetWidth();
    ext.y = 1.0f;
    m_Ratchet.SetExtents( ext );
    EditBox::Render();
} // FloatEditBox::Render

bool FloatEditBox::OnKeyDown( DWORD keyCode, DWORD flags )
{
    float mult = 1.0f;
    for (int i = '1'; i < '9'; i++)
    {
        if (GetKeyState( i ) < 0)
        {
            mult = pow( 10, i - '1' + 1 );
        }
    }
    if (GetKeyState( VK_CONTROL ) < 0) mult = 1.0/mult;

    if (keyCode == VK_LEFT)
    {
        float delta = -mult;
        m_Value += delta;
        SetText( ToString( m_Value ) );
        return true;
    }
    if (keyCode == VK_RIGHT)
    {
        float delta = mult;
        m_Value += delta;
        SetText( ToString( m_Value ) );
        return true;
    }
    
    return EditBox::OnKeyDown( keyCode, flags );
} // FloatEditBox::OnKeyDown

bool FloatEditBox::OnChar( DWORD charCode, DWORD flags )
{
    return false;
} // FloatEditBox::OnChar


void FloatEditBox::ChangeValue()
{
    float pos = m_Ratchet.GetPosition();
    float prevPos = m_Ratchet.GetPrevPosition();
    float mult = 1;
    for (int i = '1'; i < '9'; i++)
    {
        if (GetKeyState( i ) < 0)
        {
            mult = pow( 10, i - '1' + 1 );
        }
    }
    if (GetKeyState( VK_CONTROL ) < 0) mult = 1.0/mult;
    float delta = (pos - prevPos)*mult*2.0f;
    m_Value += delta;
    SetText( ToString( m_Value ) );
} // FloatEditBox::ChangeValue

void FloatEditBox::ChangeText()
{
    float val = 0; 
    if (TypeTraits<float>::FromString( val, GetText() )) m_Value = val;
} // FloatEditBox::ChangeText