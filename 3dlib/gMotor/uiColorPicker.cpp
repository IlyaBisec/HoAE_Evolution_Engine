/*****************************************************************************/
/*    File:    uiColorPicker.cpp
/*    Desc:    Color picking dialog
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#include "stdafx.h"
#include "uiWindow.h"
#include "uiColorPicker.h"
#include "kSystemDialogs.h"

/*****************************************************************************/
/*    ColorPicker implementation
/*****************************************************************************/    
IMPLEMENT_CLASS(ColorPicker);
void ColorPicker::Render()
{
}

/*****************************************************************************/
/*    ColorBox implementation
/*****************************************************************************/    
IMPLEMENT_CLASS(ColorBox);
ColorBox::ColorBox()
{
    SetColorValue( 0xFFFFFFFF );
    sigTextChanged.Connect( this, &ColorBox::OnChangeColor );
}

void ColorBox::OnChangeColor()
{
    TypeTraits<DWORD>::FromString( m_ColorValue, GetText() );
}

void   ColorBox::SetColorValue( DWORD c )
{
    m_ColorValue = c;
    m_Text = ToString( c );
}

bool ColorBox::OnMouseLBDown( int mX, int mY )
{
    Rct ext = GetScreenExtents();
    ext.y = 0;
    ext.x = ext.w - ext.h*2;
    if (ext.PtIn( mX, mY ))
    {
        PickColorDialog dlg( m_ColorValue );
        if (dlg.Show())
        {
            DWORD color = dlg.GetColor();
            color |= 0xFF000000;
            SetColorValue( color );
            return true;
        }
    }
    return EditBox::OnMouseLBDown( mX, mY );
} // ColorBox::OnMouseLBDown

void ColorBox::Render()
{
    EditBox::Render();
    Rct ext = GetScreenExtents();
    ext.x = ext.x + ext.w - ext.h - 3;
    rsColoredPanel( ext, 0.0f, m_ColorValue );
    rsFlush();
} // ColorBox::Render

void   ColorBox::Expose( PropertyMap& pm )
{
    pm.start<EditBox>( "ColorWditBox", this );
    pm.p( "ColorValue", &ColorBox::GetColorValue, &ColorBox::SetColorValue );
}
