/*****************************************************************************/
/*    File:    uiWeightEdit.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    08-12-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include "uiControl.h"
#include "uiWeightEdit.h"
#include "kSystemDialogs.h"
#include "IFontManager.h"

int    GetEditorFontID();

IMPLEMENT_CLASS( WeightEdit );

WeightEdit::WeightEdit()
{
    m_bDrag             = false;
    m_Weight            = 0.5f;

    m_MarksColor        = 0xFF000000;
    m_SeparatorColor    = 0xFF000000;
    m_SliderColor       = 0xFF0000FF;
    m_DigitColor        = 0xFF0000FF;

    m_bAutoHide         = true;
}

void WeightEdit::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "WeightEdit", this );
    pm.f( "Weight",         m_Weight );
    pm.f( "MarksColor",     m_MarksColor,       "color" );
    pm.f( "SeparatorColor", m_SeparatorColor,   "color" );
    pm.f( "SliderColor",    m_SliderColor,      "color" );
    pm.f( "DigitColor",     m_DigitColor,       "color" );
} // PEmitter::Expose

void WeightEdit::Render()
{
    if (!IsVisible()) return;
    Rct ext = GetScreenExtents();
    bool bVertical = false;
    if (ext.h > ext.w) bVertical = true;
    
    int fID = GetEditorFontID();
    //  draw background
    if (bVertical)
    {
        float txtH = IWM->GetCharHeight( fID, '0' );
        ext.y += txtH + 2;
        ext.h -= txtH + 2;
        rsPanel( ext, 0.0f );
        rsLine( ext.x + ext.w*0.5f, ext.y, ext.x + ext.w*0.5f, ext.GetBottom(), 0.0f, m_SeparatorColor );
        float step = (ext.h - 1) / 10.0f;
        //  draw marks
        float dY = ext.y;
        for (int i = 0; i <= 10; i++)
        {
            float d = 0.0f;
            if (i == 0 || i == 10) d = 1.0f;
            else if (i == 5) d = 1.0f;
            else d = 2.0f;
            rsLine( ext.x + d, dY, ext.GetRight() - d, dY, 0.0f, m_MarksColor );
            dY += step;
        }
        rsFlushPoly2D();
        rsFlushLines2D();

        //  draw slider
        float pos = ext.y + ext.h*(1.0f - m_Weight);
        rsPanel( Rct( ext.x, pos - 2, ext.w, 4 ), 0.0f, 0xFFFFFFFF, m_SliderColor, 0xFF848284 );
        
        //  draw weight text
        char buf[64];
        sprintf( buf, "%.2f", m_Weight );
        float txtW = IWM->GetStringWidth( fID, "0.00" );
        Rct rcTxt( ext.x + (ext.w - txtW)*0.5f, ext.y - txtH - 2, txtW + 1, txtH );
        IWM->DrawString( fID, buf, Vector3D( rcTxt.x + 2, rcTxt.y, 0.0f ), m_DigitColor );
        rcTxt.Inflate( 1.0f );
        rsPanel( rcTxt, 0.0f );
        
        rsFlushPoly2D();
        IWM->FlushText( fID );
        m_SliderExt = ext;
    }
    else
    {
        //float step = ext.w / 10.0f;
        ////  draw marks
        //float dX = ext.x;
        //for (int i = 0; i <= 10; i++)
        //{
        //    float d = 1.0f;
        //    if (i == 0 || i == 10) d -= 1.0f;
        //    else if (i == 5) d -= 2.0f;
        //    else d -= 4.0f;
        //    rsLine( dX, ext.y, dX, ext.GetBottom(), 0.0f, 0xFF000000 );
        //    dX += step;
        //}
        ////  draw slider
        //float pos = ext.x + ext.w*m_Weight;
        //rsPanel( Rct( pos, ext.y, 4, ext.h ), 0.0f );
    }
} // WeightEdit::Render

bool WeightEdit::OnMouseMove( int mX, int mY, DWORD keys )
{
    if (m_bDrag)
    {
        Rct scrExt = GetScreenExtents(); 
        Rct ext = m_SliderExt;
        bool bVertical = false;
        if (ext.h > ext.w) bVertical = true;
        if (bVertical)
        {
            m_Weight = 1.0f - (mY - ext.y + scrExt.y)/ext.h;
            clamp( m_Weight, 0.0f, 1.0f );
        }
        else
        {
            m_Weight = (mX - ext.x + scrExt.x)/ext.w;
            clamp( m_Weight, 0.0f, 1.0f );
        }
    }
    return false;
} // WeightEdit::OnMouseMove

bool WeightEdit::OnMouseLBDown( int mX, int mY )
{
    Rct ext = m_SliderExt;
    if (!ext.PtIn( mX, mY )) return false; 
    m_bDrag = true;
    OnMouseMove( mX, mY, 0 );
    return false;
}

bool WeightEdit::OnMouseLBUp( int mX, int mY )
{
    m_bDrag = false;
    if (m_bAutoHide) SetVisible( false );
    return false;
}



