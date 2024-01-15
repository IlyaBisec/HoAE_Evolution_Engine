/*****************************************************************************/
/*    File:    uiTrackEdit.h
/*    Desc:    Keyframe track viewer
/*    Author:    Ruslan Shestopalyuk
/*    Date:    08-12-2003
/*****************************************************************************/
#ifndef __EDTRACKEDIT_H__
#define __EDTRACKEDIT_H__

#include "mFloatCurve.h"
#include "mQuatCurve.h"
#include "mColorCurve.h"

/*****************************************************************************/
/*    Enum:    ChartScaleMode
/*    Desc:    Types of how chart reacts on the "scale" command
/*****************************************************************************/
enum ChartScaleMode
{
    csmNone        = 0,
    csmScaleX    = 1,
    csmScaleY    = 2,
    csmScaleXY    = 3
}; // enum ChartScaleMode

/*****************************************************************************/
/*    Class:    Chart
/*    Desc:    Control for drawing function 2D plots
/*****************************************************************************/
class Chart : public Dialog
{
public:
                    Chart            ();
    void            SetCurveExtents    ( float minT, float minV, float maxT, float maxV );
    bool            ToScreenSpace    ( float& x, float& y );
    bool            FromScreenSpace    ( float& x, float& y );

    bool            ToScreenSpace    ( float& x1, float& y1, float& x2, float& y2 );
    bool            InsideCurveArea ( float x, float y ) const;
    
    virtual void    Render            ();
    virtual void    Expose            ( PropertyMap& pm );

    DECLARE_SCLASS(Chart,Dialog,CHRT);

protected:
    virtual    void    DrawLine        ( float x1, float y1, float x2, float y2, DWORD color );
    void            DrawGrid        ();

    float            GetVertGridStep    () const;
    float            GetHorzGridStep    () const;
    float            GetSamplingStep    () const;

    template <class T>
    void DrawCurve( T* pHost, float (T::*func)(float), DWORD color )
    {
        float step    = GetSamplingStep();
        float prevX = m_MinTime;
        float prevY = (pHost->*func)( prevX );
        for (float cTime = m_MinTime + step; cTime < m_MaxTime; cTime += step)
        {
            float x = cTime;
            float y = (pHost->*func)( prevX );
            DrawLine( prevX, prevY, x, y, color );
            prevX = x;
            prevY = y;
        }
    } // DrawCurve

    int                m_Border;
    float            m_MinTime, m_MaxTime;
    float            m_MinVal, m_MaxVal;
    int                m_HBorder, m_VBorder;

    int                m_VertGridTicks;
    int                m_HorzGridTicks;

    DWORD            m_GridLineColor;
    DWORD            m_AxisLineColor;
    DWORD            m_AxisLabelColor;

    int                m_NumCurveSamples;

    ChartScaleMode    m_ScaleMode;
}; // class Chart

/*****************************************************************************/
/*    Class:    KeyframeChart
/*    Desc:    Control for drawing function 2D plots
/*****************************************************************************/
class KeyframeChart : public Chart
{
public:
                                KeyframeChart();

    virtual void                Render();
    virtual void                Expose( PropertyMap& pm );

    virtual bool     OnMouseLBDown        ( int mX, int mY );
    virtual bool     OnMouseLBUp        ( int mX, int mY );
    virtual bool     OnMouseMove                ( int mX, int mY, DWORD keys );
    virtual bool     OnMouseRBDown        ( int mX, int mY );
    virtual bool     OnMouseRBUp        ( int mX, int mY );

    DECLARE_SCLASS(KeyframeChart,Chart,KFCH);

protected:
    void                        DrawTimeLabels();

    DWORD                        m_KeyframesColor;
    DWORD                        m_BeginEndColor;
    DWORD                        m_CurTimeColor;
    DWORD                        m_KeyframesSide;
    DWORD                        m_TicksColor;

    float                        m_CurX;
    float                        m_CurY;
}; // class KeyframeChart

/*****************************************************************************/
/*    Class:    FloatTrackEdit
/*    Desc:    Keyframe animation track editor
/*****************************************************************************/
class FloatTrackEdit : public KeyframeChart, public PSingleton<FloatTrackEdit>
{
    FloatCurve*                 m_pCurve;    
    float                       m_MinValueBias;
    DWORD                       m_CurveColor;

public:
                                FloatTrackEdit();
                                ~FloatTrackEdit();

    virtual    void             Render();
    void                        SetTrack( FloatCurve* pCurve ) { m_pCurve = pCurve; }
    bool                        OnMouseRBUp( int mX, int mY );
    
    DECLARE_SCLASS(FloatTrackEdit,KeyframeChart,TRED);

protected:
    float                       GetCurveValue( float val );

}; // class FloatTrackEdit

enum QuatEditMode
{
    qemEuler         = 0,
    qemQuaternion    = 1
}; // enum QuatEditMode

class Shader;
/*****************************************************************************/
/*    Class:    QuatTrackEdit
/*    Desc:    Quaternion keyframe animation track editor
/*****************************************************************************/
class QuatTrackEdit : public KeyframeChart, public PSingleton<QuatTrackEdit>
{
    const QuatCurve*            m_pCurve;

    DWORD                       m_XCurveColor;
    DWORD                       m_YCurveColor;
    DWORD                       m_ZCurveColor;
    DWORD                       m_SCurveColor;

    int                         m_NumCurveSamples;
    DWORD                       m_KeyframesColor;
    DWORD                       m_BeginEndColor;
    DWORD                       m_CurTimeColor;

    QuatEditMode                m_EditMode;
    Shader*                     m_pLineShader;

public:
    QuatTrackEdit();

    virtual    void                Render();
    void                        SetTrack( const QuatCurve* pCurve ) {     m_pCurve = pCurve; }

    DECLARE_SCLASS(QuatTrackEdit,KeyframeChart,QTED);

protected:
    float                        GetEulerX( float val );
    float                        GetEulerY( float val );
    float                        GetEulerZ( float val );

    float                        GetQuatX( float val );
    float                        GetQuatY( float val );
    float                        GetQuatZ( float val );
    float                        GetQuatS( float val );

}; // class QuatTrackEdit

/*****************************************************************************/
/*    Class:    ColorTrackEdit
/*    Desc:    Quaternion keyframe animation track editor
/*****************************************************************************/
class ColorTrackEdit : public KeyframeChart, public PSingleton<ColorTrackEdit>
{
    ColorCurve*        m_pCurve;


    DWORD                        m_ACurveColor;
    DWORD                        m_RCurveColor;
    DWORD                        m_GCurveColor;
    DWORD                        m_BCurveColor;

    int                            m_NumCurveSamples;
    DWORD                        m_KeyframesColor;
    DWORD                        m_BeginEndColor;
    DWORD                        m_CurTimeColor;

    float                        m_GradY, m_GradHeight;

public:
                                ColorTrackEdit();
    virtual    void                Render();
    void                        SetTrack( ColorCurve* pCurve ) { m_pCurve = pCurve; }

    virtual bool                 OnMouseRBUp( int mX, int mY );
    virtual bool                OnKeyDown        ( DWORD keyCode, DWORD flags );



    DECLARE_SCLASS(ColorTrackEdit,KeyframeChart,CTED);

protected:
    float                        GetAlpha( float val );
    float                        GetRed    ( float val );
    float                        GetGreen( float val );
    float                        GetBlue    ( float val );

}; // class ColorTrackEdit


#endif // __EDTRACKEDIT_H__