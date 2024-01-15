/*****************************************************************************/
/*    File:    uiLabel.cpp
/*    Desc:    Label control
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#include "stdafx.h"
#include "uiWidget.h"
#include "uiLabel.h"

/*****************************************************************************/
/*  Label implementation
/*****************************************************************************/
IMPLEMENT_CLASS(Label);
Label::Label()
{
    SetBgColor( 0 );
}

void Label::Render()
{
    Rct rct = GetScreenExtents();
    float strW = GetTextWidth( m_Text.c_str() );
    float capW = GetTextWidth( m_Caption.c_str() );
    Vector3D pos( rct.x + 1, rct.y, 0.0f );

    rsRect( GetScreenExtents(), 0.0f, GetBgColor() );
    rsFlush();
    
    DrawText( pos.x, pos.y, m_TextColor, m_Caption.c_str() );
    DrawText( pos.x + capW + 2, pos.y, m_TextColor, m_Text.c_str() );
    FlushText();
} // Label::Render

void Label::Expose( PropertyMap& pm )
{
    pm.start<Widget>( "Label", this );
    pm.p( "Caption", &Label::GetCaption, &Label::SetCaption );
} // Label::Expose
