/*****************************************************************************/
/*    File:    uiRampEdit.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    08-12-2003
/*****************************************************************************/
#include "stdafx.h"
#include "uiWidget.h"
#include "uiWeightEdit.h"
#include "kSystemDialogs.h"
#include "kColorTraits.h"
#include "uiRampEdit.h"

IMPLEMENT_CLASS( ColorRampEdit );
IMPLEMENT_CLASS( AlphaRampEdit );

/*****************************************************************************/
/*    ColorRampEdit inplementation
/*****************************************************************************/
ColorRampEdit::ColorRampEdit() : m_SelectedKey(-1), m_bDragKey(false)
{
    SetClippedToExt( false );
    m_Extents.h = 50.0f;
    SetLockHeight();
    sigTextChanged.Connect( this, &ColorRampEdit::UpdateFromText );
}

void ColorRampEdit::UpdateFromText()
{
    TypeTraits<ColorRamp>::FromString( m_Ramp, GetText() );
}

const int c_MaxRampTextLen = 1024;
void ColorRampEdit::UpdateText()
{
    char buf[c_MaxRampTextLen];
    TypeTraits<ColorRamp>::ToString( m_Ramp, buf, 1024 );
    SetText( buf );
}

void ColorRampEdit::Render()
{
    Rct ext = GetScreenExtents();
    int nK = m_Ramp.GetNKeys();
    
    //  draw gradient bar
    for (int i = 1; i < nK; i++)
    {
        DWORD c0 = m_Ramp.GetKey( i - 1 );
        DWORD c1 = m_Ramp.GetKey( i );
        float t0 = m_Ramp.GetKeyTime( i - 1 );
        float t1 = m_Ramp.GetKeyTime( i );
        Rct rct( ext.x + ext.w*t0 + 2, ext.y, ext.w*(t1 - t0), ext.h );
        rsRect( rct, 0.0f, c0, c1, c0, c1 );
    }

    //  draw keys
    for (int i = 0; i < nK; i++)
    {
        float t = m_Ramp.GetKeyTime( i );
        Rct rct( ext.x + ext.w*t, ext.y, 4, ext.h );
        //  check whether key is selected
        if (i == m_SelectedKey)
        {
            rsPanel( rct, 0.0f, 0xFFFFFFFF, 0xFFFF0000, 0xFF848284 );
        }
        else
        {
            rsPanel( rct, 0.0f );
        }
    }
    
    rsFlushPoly2D();
    rsFlushLines2D();

} // ColorRampEdit::Render

bool ColorRampEdit::AskColor( DWORD& col )
{
    DWORD alpha = 0xFF000000&col;
    PickColorDialog dlg( col );
    if (dlg.Show())
    {
        col = dlg.GetColor();
        col &= 0x00FFFFFF;
        col |= alpha;
        return true;
    }
    return false;
} // ColorRampEdit::AskColor

float ColorRampEdit::GetTimeInPoint( int mX, int mY )
{
    Rct ext = GetExtents();
    if (ext.w < c_SmallEpsilon) return -1.0f;
    return float( mX )/ext.w;
} // ColorRampEdit::GetWeight

int    ColorRampEdit::GetKey( int mX, int mY )
{
    float t = GetTimeInPoint( mX, mY );
    
    int nK = m_Ramp.GetNKeys();
    for (int i = 1; i < nK; i++)
    {
        float t0 = m_Ramp.GetKeyTime( i - 1 );
        float t1 = m_Ramp.GetKeyTime( i );
        if (t < t0 || t > t1) continue;
        if (fabs( t - t0 ) < fabs( t - t1 )) return i - 1; else return i; 
    }
    return -1;
} // ColorRampEdit::GetKey

bool ColorRampEdit::OnMouseMove( int mX, int mY, DWORD keys )
{
    if (m_bDragKey)
    {
        if ( m_SelectedKey <= 0 || m_SelectedKey >= m_Ramp.GetNKeys() - 1) return false;
        float w = GetTimeInPoint( mX, mY );
        float wl = m_Ramp.GetKeyTime( m_SelectedKey - 1 );
        float wr = m_Ramp.GetKeyTime( m_SelectedKey + 1 );
        if (w > wl && w < wr) m_Ramp.SetKeyTime( m_SelectedKey, w );
        UpdateText();
    }
    return false;
} // ColorRampEdit::OnMouseMove

bool ColorRampEdit::OnMouseLBDown( int mX, int mY )
{
    if (!PtIn( mX, mY )) return false;
    int cKey = GetKey( mX, mY );
    if (cKey != -1) 
    {
        m_SelectedKey = cKey;
        m_bDragKey = true;
    }
    return false;
} // ColorRampEdit::OnMouseLBDown

bool ColorRampEdit::OnMouseLBDbl( int mX, int mY )
{
    if (!PtIn( mX, mY )) return false;
    int cKey = GetKey( mX, mY );
    if (cKey == -1) return false;
    m_SelectedKey = cKey;
    DWORD color = m_Ramp.GetKey( m_SelectedKey );
    if (AskColor( color ))
    {
        m_Ramp.SetKey( m_SelectedKey, color );
        UpdateText();
    }
    return false;
} // ColorRampEdit::OnMouseLBDbl

bool ColorRampEdit::OnMouseRBDown( int mX, int mY )
{
    if (!PtIn( mX, mY )) return false;
    float t = GetTimeInPoint( mX, mY );
    if (t < 0.0f || t > 1.0f) return false;
    DWORD color = 0xFFFFFFFF;
    if (AskColor( color )) 
    {
        m_Ramp.AddKey( t, color );
        UpdateText();
    }
    return false;
} // ColorRampEdit::OnMouseRBDown

bool ColorRampEdit::OnMouseLBUp( int mX, int mY )
{
    Rct ext = GetScreenExtents();
    m_bDragKey = false;
    return false;
} // ColorRampEdit::OnMouseLBUp

bool ColorRampEdit::OnKeyDown( DWORD keyCode, DWORD flags )
{
    if (keyCode == VK_BACK) 
    {
        m_Ramp.DeleteKey( m_SelectedKey );
        UpdateText();
    }
    return false;
} // ColorRampEdit::OnKeyDown

/*****************************************************************************/
/*    AlphaRampEdit inplementation
/*****************************************************************************/
AlphaRampEdit::AlphaRampEdit() : m_SelectedKey(-1), m_bDragKey(false)
{
    AddChild( m_WeightEdit );
    SetClippedToExt( false );
    m_Extents.h = 50.0f;
    SetLockHeight();
    sigTextChanged.Connect( this, &AlphaRampEdit::UpdateFromText );
}

void AlphaRampEdit::UpdateFromText()
{
    TypeTraits<AlphaRamp>::FromString( m_Ramp, GetText() );
}

void AlphaRampEdit::UpdateText()
{
    char buf[c_MaxRampTextLen];
    TypeTraits<AlphaRamp>::ToString( m_Ramp, buf, 1024 );
    SetText( buf );
}

void AlphaRampEdit::Render()
{
    Rct ext = GetScreenExtents();

    if (m_WeightEdit.IsVisible()) m_Ramp.SetKey( m_SelectedKey, m_WeightEdit.GetWeight() );

    int nK = m_Ramp.GetNKeys();

    //  draw gradient bar
    for (int i = 1; i < nK; i++)
    {
        float a0 = m_Ramp.GetKey( i - 1 );
        float a1 = m_Ramp.GetKey( i );
        ColorValue c0( 1.0f, a0, a0, a0 );
        ColorValue c1( 1.0f, a1, a1, a1 );
        float t0 = m_Ramp.GetKeyTime( i - 1 );
        float t1 = m_Ramp.GetKeyTime( i );
        Rct rct( ext.x + ext.w*t0, ext.y, ext.w*(t1 - t0), ext.h );
        rsRect( rct, 0.0f, c0, c1, c0, c1 );
        rsLine( ext.x + ext.w*t0, ext.y + ext.h*(1.0f - a0), 
                ext.x + ext.w*t1, ext.y + ext.h*(1.0f - a1), 0.0f, 0xFFFF6666 );
    }

    //  draw keys
    for (int i = 0; i < nK; i++)
    {
        float t = m_Ramp.GetKeyTime( i );
        Rct rct( ext.x + ext.w*t, ext.y, 4, ext.h );
        //  check whether key is selected
        if (i == m_SelectedKey)
        {
            rsPanel( rct, 0.0f, 0xFFFFFFFF, 0xFFFF0000, 0xFF848284 );
        }
        else
        {
            rsPanel( rct, 0.0f );
        }
    }

    rsFlushPoly2D();
    rsFlushLines2D();

    m_WeightEdit.Render();
} // AlphaRampEdit::Render

void AlphaRampEdit::AskWeight( float& w, int mX, int mY )
{
    Rct ext = GetScreenExtents();
    m_WeightEdit.SetWeight( w );
    m_WeightEdit.SetExtents( mX, mY, 13, 120 );
    m_WeightEdit.SetDrag();
    m_WeightEdit.SetVisible( true );
} // AlphaRampEdit::AskWeight

float AlphaRampEdit::GetTimeInPoint( int mX, int mY )
{
    Rct ext = GetExtents();
    if (ext.w < c_SmallEpsilon) return -1.0f;
    return float( mX )/ext.w;
} // AlphaRampEdit::GetWeight

int    AlphaRampEdit::GetKey( int mX, int mY )
{
    float t = GetTimeInPoint( mX, mY );

    int nK = m_Ramp.GetNKeys();
    for (int i = 1; i < nK; i++)
    {
        float t0 = m_Ramp.GetKeyTime( i - 1 );
        float t1 = m_Ramp.GetKeyTime( i );
        if (t < t0 || t > t1) continue;
        if (fabs( t - t0 ) < fabs( t - t1 )) return i - 1; else return i; 
    }
    return -1;
} // AlphaRampEdit::GetKey

bool AlphaRampEdit::OnMouseMove( int mX, int mY, DWORD keys )
{
    if (m_bDragKey)
    {
        if ( m_SelectedKey <= 0 || m_SelectedKey >= m_Ramp.GetNKeys() - 1) return false;
        float w = GetTimeInPoint( mX, mY );
        float wl = m_Ramp.GetKeyTime( m_SelectedKey - 1 );
        float wr = m_Ramp.GetKeyTime( m_SelectedKey + 1 );
        if (w > wl && w < wr) 
        {
            m_Ramp.SetKeyTime( m_SelectedKey, w );
            UpdateText();
        }
    }
    return false;
} // AlphaRampEdit::OnMouseMove

bool AlphaRampEdit::OnMouseLBDown( int mX, int mY )
{
    if (!PtIn( mX, mY )) return false;
    int cKey = GetKey( mX, mY );
    if (cKey != -1) 
    {
        m_SelectedKey = cKey;
        m_bDragKey = true;
        UpdateText();
    }
    return false;
} // AlphaRampEdit::OnMouseLBDown

bool AlphaRampEdit::OnMouseLBDbl( int mX, int mY )
{
    if (!PtIn( mX, mY )) return false;
    int cKey = GetKey( mX, mY );
    if (cKey == -1) return false;
    m_SelectedKey = cKey;
    float w = m_Ramp.GetKey( m_SelectedKey );
    AskWeight( w, mX, mY );
    UpdateText();
    return false;
} // AlphaRampEdit::OnMouseLBDbl

bool AlphaRampEdit::OnMouseRBDown( int mX, int mY )
{
    if (!PtIn( mX, mY )) return false;
    float t = GetTimeInPoint( mX, mY );
    if (t < 0.0f || t > 1.0f) return false;
    float w = 1.0f;
    m_SelectedKey = m_Ramp.AddKey( t, w );
    AskWeight( w, mX, mY ); 
    UpdateText();
    return false;
} // AlphaRampEdit::OnMouseRBDown

bool AlphaRampEdit::OnMouseLBUp( int mX, int mY )
{
    Rct ext = GetScreenExtents();
    m_bDragKey = false;
    UpdateText();
    return false;
} // AlphaRampEdit::OnMouseLBUp

bool AlphaRampEdit::OnKeyDown( DWORD keyCode, DWORD flags )
{
    if (keyCode == VK_BACK) 
    {
        m_Ramp.DeleteKey( m_SelectedKey );
        UpdateText();
    }
    return false;
} // AlphaRampEdit::OnKeyDown



