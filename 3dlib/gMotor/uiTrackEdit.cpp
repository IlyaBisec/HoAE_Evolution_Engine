/*****************************************************************************/
/*    File:    uiTrackEdit.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    08-12-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include "uiControl.h"
#include "uiTrackEdit.h"
#include "kSystemDialogs.h"
#include "IFontManager.h" 
#include "sgAnimation.h"

int GetEditorFontID();

IMPLEMENT_CLASS( Chart              );
IMPLEMENT_CLASS( KeyframeChart      );
IMPLEMENT_CLASS( FloatTrackEdit        );
IMPLEMENT_CLASS( QuatTrackEdit        );
IMPLEMENT_CLASS( ColorTrackEdit        );

/*****************************************************************************/
/*    Chart inplementation
/*****************************************************************************/
Chart::Chart()
{
    m_HBorder            = 30;     
    m_VBorder            = 20; 

    m_VertGridTicks        = 10;
    m_HorzGridTicks        = 20;
    m_NumCurveSamples    = 1000;

    m_GridLineColor        = 0xFF444455;
    m_AxisLineColor        = 0xFF666677;
    m_AxisLabelColor    = 0xFF777799;
    m_ScaleMode            = csmScaleXY;

    SetClrMdl( 0x22000000 );
}

bool Chart::ToScreenSpace( float& x, float& y )
{
    Rct ext = GetExtents();
    ext.Inflate( m_VBorder, m_HBorder, m_VBorder, m_HBorder );
    x = ext.x + ext.w*(x - m_MinTime)/(m_MaxTime - m_MinTime);
    y = ext.y + ext.h*(m_MaxVal - y)/(m_MaxVal - m_MinVal);
    return (x >= ext.x) && (x <= ext.GetRight()) && 
        (y >= ext.y) && (y <= ext.GetBottom()) ;
} // Chart::ToScreenSpace    

bool Chart::FromScreenSpace( float& x, float& y )
{
    Rct ext = GetExtents();
    ext.Inflate( m_VBorder, m_HBorder, m_VBorder, m_HBorder );
    x = (x - ext.x)*(m_MaxTime - m_MinTime)/ext.w + m_MinTime;
    y = m_MaxVal - (y - ext.y)*(m_MaxVal  - m_MinVal )/ext.h;
    return    (x >= m_MinTime) && (x <= m_MaxTime) && 
            (y >= m_MinVal ) && (y <= m_MaxVal );
} // Chart::FromScreenSpace

float Chart::GetVertGridStep() const
{
    return (m_MaxVal - m_MinVal) / float( m_VertGridTicks );
} // Chart::GetVertGridStep

float Chart::GetHorzGridStep() const
{
    return (m_MaxTime - m_MinTime) / float( m_HorzGridTicks );
} // Chart::GetHorzGridStep

float Chart::GetSamplingStep() const
{
    return (m_MaxTime - m_MinTime) / float( m_NumCurveSamples );
}

bool Chart::ToScreenSpace( float& x1, float& y1, float& x2, float& y2 )
{
    return ToScreenSpace( x1, y1 ) && ToScreenSpace( x2, y2 );
} // Chart::ToScreenSpace

void Chart::SetCurveExtents( float minT, float minV, float maxT, float maxV )
{
    m_MinTime    = minT;
    m_MaxTime    = maxT;
    m_MinVal    = minV;
    m_MaxVal    = maxV;
} // TrackChartEdit::SetCurveExtents

void Chart::Render()
{
    Parent::Render();
    
    DrawGrid();
} // Chart::Render

void Chart::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "Chart", this );
    pm.f( "MinTime",            m_MinTime            );
    pm.f( "MaxTime",            m_MaxTime            );
    pm.f( "MinValue",            m_MinVal            );
    pm.f( "MaxValue",            m_MaxVal            );
    pm.f( "NumCurveSamples",    m_NumCurveSamples    );
    pm.f( "HorzBorder",            m_HBorder            );
    pm.f( "VertBorder",            m_VBorder            );
    pm.f( "VertGridTicks",        m_VertGridTicks        );
    pm.f( "HorzGridTicks",        m_HorzGridTicks        );
    pm.f( "GridLineColor",        m_GridLineColor        );
    pm.f( "AxisLineColor",        m_AxisLineColor        );
    pm.f( "AxisLabelColor",        m_AxisLabelColor    );

} // Chart::Expose

void Chart::DrawLine( float x1, float y1, float x2, float y2, DWORD color )
{
    if (!ToScreenSpace( x1, y1, x2, y2 )) return;
    rsLine( x1, y1, x2, y2, GetPosZ(), color, color );
} // Chart::DrawLine

void Chart::DrawGrid()
{
    int fontID = GetEditorFontID();

    float hStep = GetHorzGridStep();
    float vStep = GetVertGridStep();
    
    float hTickShift = 0.0f;
    float vTickShift = 0.0f;

    float t1 = 0.0f; float v1 = 0.0f;
    float t2 = 1.0f; float v2 = 0.0f;

    FromScreenSpace( t1, v1 );
    FromScreenSpace( t2, v2 );

    float p = fabs( t2 - t1 );
    float q = fabs( v2 - v1 );

    DrawLine( 0.0f, m_MinVal, 0.0f, m_MaxVal, m_AxisLineColor );
    DrawLine( p,    m_MinVal, p,    m_MaxVal, m_AxisLineColor );

    DrawLine( m_MinTime, 0.0f,    m_MaxTime, 0.0f, m_AxisLineColor );
    DrawLine( m_MinTime, q,        m_MaxTime, q,     m_AxisLineColor );
    
    for (float cx = m_MinTime + hTickShift; cx <= m_MaxTime; cx += hStep)
    {
        DrawLine( cx, m_MinVal, cx, m_MaxVal, m_GridLineColor );
        char str[256];
        sprintf( str, "%0.2f", cx );
        Vector3D pos( cx, m_MinVal, GetPosZ() );
        ToScreenSpace( pos.x, pos.y );
        pos.x -= IWM->GetStringWidth( fontID, str ) * 0.5f;
        pos.y += 3.0f;
        IWM->DrawString( fontID, str, pos, m_AxisLabelColor );
    }

    for (float cy = m_MinVal + vTickShift; cy <= m_MaxVal; cy += vStep)
    {
        DrawLine( m_MinTime, cy, m_MaxTime, cy, m_GridLineColor );
        if (cy > m_MinVal + vTickShift) 
        {
            char str[256];
            sprintf( str, "%0.2f", cy );
            Vector3D pos( m_MinVal, cy, GetPosZ() );
            ToScreenSpace( pos.x, pos.y );
            pos.x -= IWM->GetStringWidth( fontID, str ) + 3.0f;
            pos.y -= IWM->GetCharHeight( fontID, 'D' ) * 0.5f;
            IWM->DrawString( fontID, str, pos, m_AxisLabelColor );
        }
    }

    rsFlushLines3D();
    IWM->FlushText( fontID );
} // Chart::DrawGrid

bool Chart::InsideCurveArea( float x, float y ) const
{
    return (m_MinTime <= x) && (m_MaxTime >= x) && (m_MinVal <= y) && (m_MaxVal >= y);
}

/*****************************************************************************/
/*    KeyframeChart inplementation
/*****************************************************************************/
KeyframeChart::KeyframeChart()
{
    m_KeyframesColor    = 0xFFFFFF00;
    m_KeyframesSide        = 4.0f;
    m_BeginEndColor        = 0xFF000000;
    m_CurTimeColor        = 0xFFFFFFFF;
    m_TicksColor        = 0xFFFFFF00;

    m_CurX = m_CurY = 0;
} // KeyframeChart::KeyframeChart

void KeyframeChart::Render()
{
    Parent::Render();
    DrawTimeLabels();
    
    int fontID = GetEditorFontID();

    if (InsideCurveArea( m_CurX, m_CurY )) 
    {
        char str[128];
        Rct ext = GetExtents();
        Vector3D pos = Vector3D( ext.x + 3, ext.y + 3, 0.0f );
        sprintf( str, "Time: %0.4f", m_CurX );
        IWM->DrawString( fontID, str, pos, m_TicksColor );

        pos = Vector3D( ext.x + 100.0f, ext.y + 3, 0.0f );
        sprintf( str, "Value: %0.4f", m_CurY );
        IWM->DrawString( fontID, str, pos, m_TicksColor );
    }
    IWM->FlushText( fontID );
} // KeyframeChart::Render

void KeyframeChart::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "KeyframeChart", this );
    pm.f( "KeyframesColor",    m_KeyframesColor    );
    pm.f( "BeginEndColor",    m_BeginEndColor        );
    pm.f( "CurTimeColor",        m_CurTimeColor        );
} // KeyframeChart::Expose

void KeyframeChart::DrawTimeLabels()
{
    float t1 = 0.0f; float v1 = 0.0f;
    float t2 = 1.0f; float v2 = 0.0f;

    FromScreenSpace( t1, v1 );
    FromScreenSpace( t2, v2 );

    float p = fabs( t2 - t1 );

    DrawLine( m_MinTime,     m_MinVal, m_MinTime,        m_MaxVal, m_BeginEndColor );
    DrawLine( m_MinTime + p, m_MinVal, m_MinTime + p,    m_MaxVal, m_BeginEndColor );

    DrawLine( m_MaxTime,     m_MinVal, m_MaxTime,        m_MaxVal, m_BeginEndColor );
    DrawLine( m_MaxTime - p, m_MinVal, m_MaxTime - p,    m_MaxVal, m_BeginEndColor );

    float curTime = Animation::CurTime();    
    DrawLine( curTime,         m_MinVal, curTime,            m_MaxVal, m_CurTimeColor );

    rsFlushLines2D();    
} // KeyframeChart::DrawTimeLabels

bool KeyframeChart::OnMouseLBDown( int mX, int mY )
{
    if (IsInvisible()) return false;
    /*m_CurX = mX;
    m_CurY = mY;
    FromScreenSpace( m_CurX, m_CurY );
    */
    return false;
} // KeyframeChart::OnMouseLBDown

bool KeyframeChart::OnMouseLBUp( int mX, int mY )
{
    return false;
} // KeyframeChart::OnMouseLBUp

bool KeyframeChart::OnMouseMove( int mX, int mY, DWORD keys )
{
    m_CurX = mX;
    m_CurY = mY;
    FromScreenSpace( m_CurX, m_CurY );

    return false;
} // KeyframeChart::OnMouseMove

bool KeyframeChart::OnMouseRBDown( int mX, int mY )
{
    return false;
} // KeyframeChart::OnMouseRBDown

bool KeyframeChart::OnMouseRBUp( int mX, int mY )
{
    return false;
} // KeyframeChart::OnMouseRBUp

/*****************************************************************************/
/*    FloatTrackEdit inplementation
/*****************************************************************************/
FloatTrackEdit::FloatTrackEdit()
{
    SetName( "FloatTrackEdit" );
    m_pCurve            = NULL;
    m_MinValueBias        = 2.0f;
    m_CurveColor        = 0xFFFF0000;
}

FloatTrackEdit::~FloatTrackEdit()
{
}

void FloatTrackEdit::Render()
{
    float minV = m_pCurve->GetMinVal();
    float maxV = m_pCurve->GetMaxVal();

    if (maxV - minV < m_MinValueBias) 
    {
        float mean = (minV + maxV) * 0.5f;
        minV = mean - 0.5f * m_MinValueBias;
        maxV = mean + 0.5f * m_MinValueBias;
    }

    float minT = m_pCurve->GetMinTime();
    float maxT = m_pCurve->GetMaxTime();

    if (maxT - minT == 0.0f) maxT = minT + 1000.0f;

    SetCurveExtents( minT, minV, maxT, maxV );

    Parent::Render();
    if (!m_pCurve) return;
    
    DrawGrid();
    DrawTimeLabels();

    DrawCurve( this, &FloatTrackEdit::GetCurveValue, m_CurveColor );

    
    int nKeys = m_pCurve->GetNKeys();
    for (int i = 0; i < nKeys; i++)
    {
        float x = m_pCurve->GetKeyTime ( i );
        float y = m_pCurve->GetKeyValue( i );
        ToScreenSpace( x, y );
        Rct rct( x - 1, y - 1, 4, 4 );
        rsFrame( rct, GetPosZ(), m_KeyframesColor );
    }

    rsFlushLines2D();
} // FloatTrackEdit::Render

float FloatTrackEdit::GetCurveValue( float val )
{
    if (!m_pCurve) return 0.0f;
    return m_pCurve->GetValue( val );
} // FloatTrackEdit::GetCurveValue

bool FloatTrackEdit::OnMouseRBUp( int mX, int mY )
{
    if (IsInvisible() || GetKeyState( VK_MENU ) < 0) return false;
    if (!m_pCurve || !InsideCurveArea( m_CurX, m_CurY )) return false;

    if (GetKeyState( VK_CONTROL ) < 0)
    {
        float* pVal = m_pCurve->KeyValue( m_CurX ); 
        if (pVal) *pVal = m_CurY;
        return false;
    }

    m_pCurve->AddKey( m_CurX, m_CurY );
    return false;
} // ColorTrackEdit::OnMouseLBUp

/*****************************************************************************/
/*    QuatTrackEdit inplementation
/*****************************************************************************/
QuatTrackEdit::QuatTrackEdit()
{
    SetName( "QuatTrackEdit" );
    m_pCurve            = NULL;
    m_NumCurveSamples    = 500;

    m_XCurveColor        = 0xAAFF0000;
    m_YCurveColor        = 0xAA00FF00;
    m_ZCurveColor        = 0xAA0000FF;
    m_SCurveColor        = 0xAAEEEEEE;

    m_KeyframesColor    = 0xFFFFFF00;
    m_BeginEndColor        = 0xFF000000;
    m_CurTimeColor        = 0xFFFFFFFF;
    m_EditMode            = qemQuaternion;
    m_pLineShader        = AddChild<Shader>( "lines3D_blend" );
}

void QuatTrackEdit::Render()
{
    if (!m_pCurve) return;

    float minV = -c_PI;
    float maxV = c_PI;

    if (m_EditMode == qemQuaternion)
    {
        minV = -1.0f;
        maxV = 1.0f;
    }
    
    if (m_pLineShader) m_pLineShader->Render();


    float minT = m_pCurve->GetMinTime();
    float maxT = m_pCurve->GetMaxTime();

    if (maxT - minT == 0.0f) maxT = minT + 1000.0f;

    SetCurveExtents( minT, minV, maxT, maxV );

    DrawGrid();
    DrawTimeLabels();

    Parent::Render();


    int nKeys = m_pCurve->GetNKeys();

    if (m_EditMode == qemEuler) 
    {

        DrawCurve( this, &QuatTrackEdit::GetEulerX, m_XCurveColor );
        DrawCurve( this, &QuatTrackEdit::GetEulerY, m_YCurveColor );
        DrawCurve( this, &QuatTrackEdit::GetEulerZ, m_ZCurveColor );

        if (nKeys > 0)
        {
            Matrix3D    rot; 
            for (int i = 0; i < nKeys; i++)
            {
                float        cTime = m_pCurve->GetKeyTime( i );
                Quaternion    q = m_pCurve->GetKeyValue( i );
                Vector3D    t( cTime, cTime, cTime );
                rot.rotation( q );
                Vector3D    v = rot.EulerXYZ();

                ToScreenSpace( t.x, v.x );
                ToScreenSpace( t.y, v.y );
                ToScreenSpace( t.z, v.z );

                Rct rct( t.x - 1, v.x - 1, 4, 4 );
                rsFrame( rct, GetPosZ(), m_KeyframesColor );
                rct.Set( t.y - 1, v.y - 1, 4, 4 );
                rsFrame( rct, GetPosZ(), m_KeyframesColor );
                rct.Set( t.z - 1, v.z - 1, 4, 4 );
                rsFrame( rct, GetPosZ(), m_KeyframesColor );

            }
        }
    }
    else if (m_EditMode == qemQuaternion) 
    {
        DrawCurve( this, &QuatTrackEdit::GetQuatX, m_XCurveColor );
        DrawCurve( this, &QuatTrackEdit::GetQuatY, m_YCurveColor );
        DrawCurve( this, &QuatTrackEdit::GetQuatZ, m_ZCurveColor );
        DrawCurve( this, &QuatTrackEdit::GetQuatS, m_SCurveColor );

        if (nKeys > 0)
        {
            for (int i = 0; i < nKeys; i++)
            {
                float        cTime = m_pCurve->GetKeyTime( i );
                Quaternion    q = m_pCurve->GetValue( cTime );
                Vector4D    t( cTime, cTime, cTime, cTime );

                ToScreenSpace( t.x, q.v.x );
                ToScreenSpace( t.y, q.v.y );
                ToScreenSpace( t.z, q.v.z );
                ToScreenSpace( t.w, q.s   );


                Rct rct( t.x - 1, q.v.x - 1, 4, 4 );
                rsFrame( rct, GetPosZ(), m_KeyframesColor );
                rct.Set( t.y - 1, q.v.y - 1, 4, 4 );
                rsFrame( rct, GetPosZ(), m_KeyframesColor );
                rct.Set( t.z - 1, q.v.z - 1, 4, 4 );
                rsFrame( rct, GetPosZ(), m_KeyframesColor );
                rct.Set( t.w - 1, q.s - 1, 4, 4 );
                rsFrame( rct, GetPosZ(), m_KeyframesColor );
            }
        }
    }

    rsFlushLines2D();
} // QuatTrackEdit::Render

float QuatTrackEdit::GetEulerX( float val )
{
    if (!m_pCurve) return 0.0f;
    Quaternion    q = m_pCurve->GetValue( val );
    Matrix3D rot;
    rot.rotation( q );
    Vector3D v = rot.EulerXYZ();
    return v.x;
} // QuatTrackEdit::GetEulerX

float QuatTrackEdit::GetEulerY( float val )
{
    if (!m_pCurve) return 0.0f;
    Quaternion    q = m_pCurve->GetValue( val );
    Matrix3D rot;
    rot.rotation( q );
    Vector3D v = rot.EulerXYZ();
    return v.y;
} // QuatTrackEdit::GetEulerY

float QuatTrackEdit::GetEulerZ( float val )
{
    if (!m_pCurve) return 0.0f;
    Quaternion    q = m_pCurve->GetValue( val );
    Matrix3D rot;
    rot.rotation( q );
    Vector3D v = rot.EulerXYZ();
    return v.z;
} // QuatTrackEdit::GetEulerZ

float QuatTrackEdit::GetQuatX( float val )
{
    if (!m_pCurve) return 0.0f;
    Quaternion    q = m_pCurve->GetValue( val );
    return q.v.x;
} // QuatTrackEdit::GetQuatX

float QuatTrackEdit::GetQuatY( float val )
{
    if (!m_pCurve) return 0.0f;
    Quaternion    q = m_pCurve->GetValue( val );
    return q.v.y;
} // QuatTrackEdit::GetQuatY

float QuatTrackEdit::GetQuatZ( float val )
{
    if (!m_pCurve) return 0.0f;
    Quaternion    q = m_pCurve->GetValue( val );
    return q.v.z;
} // QuatTrackEdit::GetQuatZ

float QuatTrackEdit::GetQuatS( float val )
{
    if (!m_pCurve) return 0.0f;
    Quaternion    q = m_pCurve->GetValue( val );
    return q.s;
} // QuatTrackEdit::GetQuatS

/*****************************************************************************/
/*    ColorTrackEdit inplementation
/*****************************************************************************/
ColorTrackEdit::ColorTrackEdit()
{
    SetName( "ColorTrackEdit" );
    m_pCurve            = NULL;
    m_NumCurveSamples    = 500;

    m_RCurveColor        = 0xAAFF0000;
    m_GCurveColor        = 0xAA00FF00;
    m_BCurveColor        = 0xAA0000FF;
    m_ACurveColor        = 0xAAEEEEEE;

    m_KeyframesColor    = 0xFFFFFF00;
    m_BeginEndColor        = 0xFF000000;
    m_CurTimeColor        = 0xFFFFFFFF;

    m_GradHeight        = 10.0f;

    m_HBorder            = 50;     
    m_VBorder            = 30; 

    m_NumCurveSamples    = 300;
}

float ColorTrackEdit::GetAlpha( float val )
{
    if (!m_pCurve) return 0.0f;
    return m_pCurve->GetValue( val ).a;
}

float ColorTrackEdit::GetRed( float val )
{
    if (!m_pCurve) return 0.0f;
    return m_pCurve->GetValue( val ).r;
}

float ColorTrackEdit::GetGreen( float val )
{
    if (!m_pCurve) return 0.0f;
    return m_pCurve->GetValue( val ).g;
}

float ColorTrackEdit::GetBlue( float val )
{
    if (!m_pCurve) return 0.0f;
    return m_pCurve->GetValue( val ).b;
}

void ColorTrackEdit::Render()
{
    if (!m_pCurve) return;

    float minV = -c_PI;
    float maxV = c_PI;

    minV = 0.0f;
    maxV = 1.0f;

    float minT = m_pCurve->GetMinTime();
    float maxT = m_pCurve->GetMaxTime();

    if (maxT - minT == 0.0f) maxT = minT + 1000.0f;

    SetCurveExtents( minT, minV, maxT, maxV );
    
    m_GradY = GetExtents().y + m_VBorder - m_GradHeight - 3.0f;

    Parent::Render();

    int nKeys = m_pCurve->GetNKeys();

    DrawCurve( this, &ColorTrackEdit::GetAlpha,    m_ACurveColor );
    DrawCurve( this, &ColorTrackEdit::GetRed,    m_RCurveColor );
    DrawCurve( this, &ColorTrackEdit::GetGreen,    m_GCurveColor );
    DrawCurve( this, &ColorTrackEdit::GetBlue,    m_BCurveColor );

    DWORD prevClr    = 0;
    float prevX        = 0.0f;
    if (nKeys > 0)
    {
        for (int i = 0; i < nKeys; i++)
        {
            float cTime    = m_pCurve->GetKeyTime( i );
            ColorValue v = m_pCurve->GetKeyValue( i );
            Vector4D t( cTime, cTime, cTime, cTime );

            ToScreenSpace( t.x, v.a );
            ToScreenSpace( t.y, v.r );
            ToScreenSpace( t.z, v.g );
            ToScreenSpace( t.w, v.b );

            Rct rct( t.x - 1, v.a - 1, 4, 4 ); rsFrame( rct, GetPosZ(), m_KeyframesColor );
            rct.Set( t.y - 1, v.r - 1, 4, 4 ); rsFrame( rct, GetPosZ(), m_KeyframesColor );
            rct.Set( t.z - 1, v.g - 1, 4, 4 ); rsFrame( rct, GetPosZ(), m_KeyframesColor );
            rct.Set( t.w - 1, v.b - 1, 4, 4 ); rsFrame( rct, GetPosZ(), m_KeyframesColor );

            DWORD clr = m_pCurve->GetKeyValue( i );
            if (i > 0)
            {
                Rct gradRct( prevX, m_GradY, t.x - prevX, m_GradHeight );
                rsRect( gradRct, GetPosZ(), prevClr, clr, prevClr, clr );
            }
            prevX = t.x;
            prevClr = clr;
        }
    }

    ToScreenSpace( minT, minV, maxT, maxV );
    Rct gradRct( minT, m_GradY, maxT - minT, m_GradHeight );
    rsFrame( gradRct, GetPosZ(), 0x77AAAACC );

    rsFlushLines2D();
    rsFlushPoly2D();

} // ColorTrackEdit::Render

bool ColorTrackEdit::OnMouseRBUp( int mX, int mY )
{
    if (IsInvisible() || GetKeyState( VK_MENU ) < 0) return false;
    if (!m_pCurve || !InsideCurveArea( m_CurX, m_CurY )) return false;

    float curX = mX;
    float curY = mY;
    FromScreenSpace( curX, curY );

    if (!m_pCurve || !InsideCurveArea( m_CurX, m_CurY )) return false;

    if (GetKeyState( '1' ) < 0)
    {
        ColorValue* pVal = m_pCurve->KeyValue( m_CurX ); 
        if (pVal) pVal->a = m_CurY;
        return false;
    }
    else if (GetKeyState( '2' ) < 0)
    {
        ColorValue* pVal = m_pCurve->KeyValue( m_CurX ); 
        if (pVal) pVal->r = m_CurY;
        return false;
    }
    else if (GetKeyState( '3' ) < 0)
    {
        ColorValue* pVal = m_pCurve->KeyValue( m_CurX ); 
        if (pVal) pVal->g = m_CurY;
        return false;
    }
    else if (GetKeyState( '4' ) < 0)
    {
        ColorValue* pVal = m_pCurve->KeyValue( m_CurX ); 
        if (pVal) pVal->b = m_CurY;
        return false;
    }

    ColorValue cval( curY, curY, curY, curY );
    DWORD color = cval;

    PickColorDialog dlg( color );
    if (dlg.Show())
    {
        color = dlg.GetColor();
        cval = color;
        m_pCurve->AddKey( curX, cval );
    }

    return false;
} // ColorTrackEdit::OnMouseLBUp

bool ColorTrackEdit::OnKeyDown( DWORD keyCode, DWORD flags )
{
    if (keyCode == VK_DELETE)
    {
        m_pCurve->DeleteKey( 0 );
    }
    return false;
} // ColorTrackEdit::OnKeyDown




