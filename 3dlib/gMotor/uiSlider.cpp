/*****************************************************************************/
/*    File:    uiSlider.cpp
/*    Desc:    Slider control
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10 Nov 2004
/*****************************************************************************/
#include "stdafx.h"
#include "uiWidget.h"
#include "uiSlider.h"
#include "IFontManager.h"

/*****************************************************************************/
/*  Slider implementation
/*****************************************************************************/
IMPLEMENT_CLASS(Slider);
Slider::Slider() : m_Position( 0.0f )
{
    m_ThumbWidth    = 6;
    m_MinPos        = 0.0f;
    m_MaxPos        = 1.0f;
    m_bDragged      = false;
    m_bDrawBorder   = false;
} // Slider::Slider

void Slider::Expose( PropertyMap& pm )
{
    pm.start<Widget>( "Slider", this );
    pm.f( "MinPos", m_MinPos );
    pm.f( "MaxPos", m_MaxPos );
    pm.p( "Position", &Slider::GetPosition, &Slider::SetPosition );
    pm.f( "ThumbWidth", m_ThumbWidth );
    pm.f( "DrawBorder", m_bDrawBorder );
} // Button::Expose

void Slider::StartDrag()
{
    
} // Slider::StartDrag

void Slider::EndDrag()
{
    if (!m_bDragged) return;
} // Slider::EndDrag

void Slider::ChangePos()
{
    if (!m_bDragged) return;
} // Slider::ChangePos

void Slider::SetPosition( float pos )
{ 
    if (pos <= m_MaxPos && pos >= m_MinPos) m_Position = pos; 
} // Slider::SetPosition

void Slider::Render()
{
    Rct ext = GetScreenExtents();

    if (m_bDrawBorder)
    {
        rsThickPanel( ext, 0.0f, m_BgColor );
        rsFlush();
    }

    DrawSlideArea();
    DrawTicks();
    DrawThumb();
    
} // Slider::Render

bool Slider::OnExtents()
{
    return true;
}

void Slider::DrawThumb()
{
    Rct ext = GetScreenExtents();
    m_ThumbExt = ext;
    m_ThumbExt.Inflate( 5, 10, 5, 10 );
    m_ThumbExt.x += -8 + (m_ThumbExt.w + 8)*(m_Position - m_MinPos)/(m_MaxPos - m_MinPos);
    m_ThumbExt.w = 9;
    m_ThumbExt.y -= 3;
    
    int    GetEditorGlyphsID();
    int glID = GetEditorGlyphsID();
    Rct ruv( 40, 247, 9, 9 ); 
    ruv /= 256.0f;
    ColorValue cv( m_BgColor );
    cv.a = 1.0f;
    IWM->DrawChar( glID, Vector3D( m_ThumbExt.x, m_ThumbExt.y, 0.0f ), ruv, cv );
    IWM->FlushText( glID );
} // Slider::DrawThumb

void Slider::DrawSlideArea()
{
    Rct ext = GetScreenExtents();
    m_SlideExt = ext;
    m_SlideExt.Inflate( 5, 5, 5, 5 );
    m_SlideExt.h = 6;
   
    rsInnerPanel( m_SlideExt, 0.0f, m_BgColor );
    rsFlush();
}

void Slider::DrawTicks()
{

}

bool Slider::OnMouseMove( int mX, int mY, DWORD keys )
{
    if (m_bDragged)
    {
        float dx = mX - m_StartDragPos;
        
        m_Position += m_MinPos + dx/m_SlideExt.w*(m_MaxPos - m_MinPos);
        clamp( m_Position, m_MinPos, m_MaxPos );
        m_StartDragPos = mX;
    }
    return false;
} // Slider::OnMouseMove

bool Slider::OnMouseLBDown( int mX, int mY )
{
    Rct ext = GetScreenExtents();
    int mx = mX + ext.x;
    int my = mY + ext.y;

    Rct tExt( m_ThumbExt ); tExt.Inflate( 3 );
    Rct sExt( m_SlideExt ); sExt.Inflate( 3 );
    //  check if we are in thumb
    if (tExt.PtIn( mx, my ))
    {
        m_bDragged = true;
        m_StartDragPos = mX;
    }
    else if (sExt.PtIn( mx, my ))
    {
        float d = (mx - m_SlideExt.x)/m_SlideExt.w;
        m_Position = m_MinPos + d*(m_MaxPos - m_MinPos);
    }
    return false;
}

bool Slider::OnMouseLBUp( int mX, int mY )
{
    m_bDragged = false;
    return false;
}