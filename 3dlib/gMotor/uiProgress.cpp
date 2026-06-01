/*****************************************************************************/
/*    File:    uiProgress.cpp
/*    Desc:    Progress control
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#include "stdafx.h"
#include "uiWidget.h"
#include "uiProgress.h"

/*****************************************************************************/
/*  Progress implementation
/*****************************************************************************/
IMPLEMENT_CLASS(Progress);
Progress::Progress()
{
    m_Progress      = 0.0f;
    m_BarColor      = 0xFF0000FF;
    m_TickWidth     = 6;
    m_TickSpacing   = 0;
    m_bDrawText     = true;
} // Progress::Progress

void Progress::Expose( PropertyMap& pm )
{
    pm.start<Widget>( "Progress", this );
    pm.f( "DrawText",       m_bDrawText                 );
    pm.p( "Progress", &Progress::GetProgress, &Progress::SetProgress    );
    pm.f( "BarColor",       m_BarColor, "color"         );
    pm.f( "TickWidth",      m_TickWidth                 );
    pm.f( "TickSpacing",    m_TickSpacing               );
} // Progress::Expose

void Progress::Render()
{
    Rct ext = GetScreenExtents();
    rsPanel( ext, 0.0f, 0xFFFFFFFF, m_BgColor );
    ext.Inflate( -1 );
    int amt = ext.w*m_Progress;
    int nTicks = amt/(m_TickWidth + m_TickSpacing);
    for (int i = 0; i <= nTicks; i++)
    {
        float tpos = i*(m_TickWidth + m_TickSpacing);
        float w = amt - tpos;
        if (m_TickSpacing < w) w = m_TickWidth;
        Rct rctTick( ext.x + tpos, ext.y, w, ext.h );
        rsPanel( rctTick, 0.0f, 0xFFFFFFFF, m_BarColor );
    }
    if (m_bDrawText)
    {
        int percents = int( m_Progress*100.0f );
        float tx = ext.x + ext.w*0.5f - GetTextWidth( "%d", percents )*0.5f;
        float ty = ext.y + ext.h*0.5f - GetTextHeight()*0.5f;
        const char c = '%';
        DrawText( tx, ty, m_TextColor, "%d%c", percents, c );
    }
    rsFlushPoly2D();
    FlushText();
} // Progress::Render
