/*****************************************************************************/
/*    File:    uiPalette.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    15.04.2003
/*****************************************************************************/
#include "stdafx.h"
#include "uiWidget.h"
#include "uiWindow.h"
#include "uiButton.h"
#include "uiPalette.h"
#include "IFontManager.h"

IMPLEMENT_CLASS(PaletteSample);
/*****************************************************************************/
/*    PaletteSample implementation
/*****************************************************************************/
PaletteSample::PaletteSample()
{
    SetDraggable( false );
    //SetDroppable( true );
}

int    GetEditorFontID();
int GetEditorGlyphsID();
void PaletteSample::Render()
{
    int         fontID  = GetEditorFontID();
    int         glID    = GetEditorGlyphsID();
    IReflected* pNode   = ObjectFactory::instance().GetClassPattern( m_SampleClass.c_str() );
    int         glyphID = 63;
    DWORD       clr     = 0xFFFFFFFF;
    
    if (pNode)
    {
        //glyphID = GetNodeGlyph( pNode );
        clr = pNode->GetColor();
    }
    
    clr &= 0x00FFFFFF;
    clr |= 0xAA000000;
    Rct uv( 256 - 83, 256 - 24, 83, 24 );
    uv /= 256;
    
    Rct rct = GetScreenExtents();
    IWM->DrawChar( glID, Vector3D( rct.x, rct.y, 0.0f ), uv, clr );
    //  draw glyph
    Rct rcGlyph( rct );
    rcGlyph.x += 3;
    rcGlyph.y += 5;
    IWM->DrawChar( glID, Vector3D( rcGlyph.x, rcGlyph.y, 0.0f ), glyphID );

    //  draw text
    DWORD textColor = m_TextColor;
    Rct txtRct( rct.x + rct.h + 8, rct.y + 7, rct.w - rct.h - 8, rct.h - 2 );
    IWM->DrawString( fontID, m_Text.c_str(), 
                        Vector3D( txtRct.x, txtRct.y, 0.0f ), textColor );
    IWM->FlushText();
} // PaletteSample::Render

void PaletteSample::Expose( PropertyMap& pm )
{
    pm.start<Widget>( "Palette", this );
    pm.p( "SampleClass", &PaletteSample::GetSampleClass, &PaletteSample::SetSampleClass );

} // Palette::Expose

/*****************************************************************************/
/*    FileBrowser implementation
/*****************************************************************************/
IMPLEMENT_CLASS(FileBrowser);
FileBrowser::FileBrowser()
{
}

void FileBrowser::Render()
{    
} // FileBrowser::Render

void FileBrowser::Expose( PropertyMap& pm )
{
    pm.start( "FileBrowser", this );
} // FileBrowser::Expose
