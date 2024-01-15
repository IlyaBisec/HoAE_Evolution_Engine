/*****************************************************************************/
/*	File:	sgFont.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	09-18-2003
/*****************************************************************************/
#include "stdafx.h"
#include "kIOHelpers.h"
#include "mAlgo.h"
#include "sgFont.h"
#include "sgNodeResMgr.hpp"
#include "kTemplates.hpp"

#ifndef _INLINES
#include "sgFont.inl"
#endif // !_INLINES

BEGIN_NAMESPACE(sg)
/*****************************************************************************/
/*	Font implementation
/*****************************************************************************/
Font::Font()
{
	m_HorzSpacing	= 1;
	m_Charset		= RUSSIAN_CHARSET;
	m_bBold			= false;
	m_bItalic		= false;
}

Font::~Font()
{
}

void Font::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "Font", this );
	pm.p	( "FontName",	GetFontName, SetFontName );
	pm.f	( "Bold",		m_bBold			);
	pm.f	( "Italic",		m_bItalic		);
	pm.f	( "Height",		m_Height		);
	pm.f	( "HorzSpacing",m_HorzSpacing	);
	pm.m0( "Generate",	Generate		);
} // Font::Expose

void Font::AddString( const Vector3D& pos, const char* str, DWORD color, Geometry* pBucket )
{
	if (!str) return;
	const char* pStr = str;
	Vector3D cPos( pos );
	int nPages = GetNPages();
	while (*pStr) 
	{
		DWORD code = DWORD( *pStr );
		int cPage = 0;
		for (int i = 0; i < nPages; i++)
		{
			GlyphPage* pPage = GetPage( i );
			int charIdx = pPage->FindGlyph( code );
			if (charIdx == -1) continue;
			const Rct* pCharExt = pPage->AddGlyph( charIdx, cPos, color, m_TM );
			
			if (!pCharExt) 
			{
				Render();
				pCharExt = pPage->AddGlyph( charIdx, cPos, color, m_TM );
			}

			if (pCharExt)
			{
				cPos.x += m_HorzSpacing + pCharExt->w;
				break;
			}
		}
		pStr++;
	}
} // Font::DrawString

float Font::GetStringWidth( const char* str, int nChar )
{
	float curW = 0.0f;
	const char* pStr = str;
	int nPages = GetNPages();

	if (nChar < 0) nChar = strlen( str );
	for (int c = 0; c < nChar; c++) 
	{
		DWORD code = DWORD( *pStr );
		int cPage = 0;
		for (int i = 0; i < nPages; i++)
		{
			GlyphPage* pPage = GetPage( i );
			int charIdx = pPage->FindGlyph( code );
			if (charIdx == -1) continue;
			curW += pPage->GetGlyphWidth( charIdx ) + m_HorzSpacing;
		}
		pStr++;
	}
	return curW;
} // Font::GetStringWidth

void Font::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_FontName << m_bBold << m_bItalic << m_Height << m_HorzSpacing;
} // Font::Serialize

void Font::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
	is >> m_FontName >> m_bBold >> m_bItalic >> m_Height >> m_HorzSpacing;
} // Font::Unserialize

//-------------------------------------------------------------------------------
//  Func:	Font::Generate  
//  Desc:	Creates font texture and Glyph table, using Windows GDI functions
//  Ret:    true when everything is cool
//-------------------------------------------------------------------------------
bool Font::Generate()
{
	AddChild<DeviceStateSet>( "text3d" );

	GlyphPage* pPage		= CreatePage();
	Texture*  pTexture  = pPage->GetTexture();
	int texW = pTexture->GetWidth();
	int texH = pTexture->GetHeight();

    DWORD*     pBitmapBits;
    BITMAPINFO bmi;
    ZeroMemory( &bmi.bmiHeader,  sizeof(BITMAPINFOHEADER) );
    bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth       =  pTexture->GetWidth();
    bmi.bmiHeader.biHeight      = -pTexture->GetHeight();
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biBitCount    = 32;

    //   DC and a bitmap for the font
    HDC     hDC       = CreateCompatibleDC( NULL );
    HBITMAP hBitmap	  = CreateDIBSection( hDC, &bmi, DIB_RGB_COLORS,
                                          (VOID**)&pBitmapBits, NULL, 0 );
    SetMapMode( hDC, MM_TEXT );

    //  create a font
    //INT nHeight    = -MulDiv( m_Height, (INT)(GetDeviceCaps( hDC, LOGPIXELSY ) * 1.0f), 72 );
	INT nHeight = m_Height;
    
	DWORD dwBold   = m_bBold   ? TRUE : FALSE;
	DWORD dwItalic = m_bItalic ? TRUE : FALSE;
    HFONT hFont    = CreateFont( -nHeight, 0, 0, 0, dwBold, dwItalic,
                          FALSE, FALSE, m_Charset, OUT_DEFAULT_PRECIS,
                          CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
                          VARIABLE_PITCH, m_FontName.c_str() );
    if (hFont == 0)
	{
		Log.Warning( "Couldn't create font %s.", m_FontName );
		return false;
	}

	// Set text properties
    HGDIOBJ hOldFont = SelectObject( hDC, hFont			);
	SetTextColor( hDC, RGB(255,255,255) );
	SetBkColor(   hDC, 0x00000000		);
	SetTextAlign( hDC, TA_TOP			);

	HGDIOBJ hOldBmp = SelectObject( hDC, hBitmap );

    // Loop through all printable Glyphs and output them to the bitmap.
    // Meanwhile, keep track of the corresponding tex coords for each Glyph.
    DWORD x = 0;
    DWORD y = 0;
    char str[2] = "x";
    SIZE size;

	for (char c = 32; c < 127; c++)
	{
		str[0] = c;
		DWORD code = c;

		GetTextExtentPoint32( hDC, str, 1, &size );
		GlyphPage::Glyph* pChar = pPage->AllocateGlyph( code, size.cx, size.cy );
		if ( pChar ) 
		{
			Rct* pRct = &pChar->ext;
			ExtTextOut( hDC, pRct->x, pRct->y, ETO_OPAQUE, NULL, str, 1, NULL );
		}
	}
	
	// Lock the surface and write the alpha values for the set pixels
    int pitch = 0;
	BYTE* pDstRow = pTexture->LockBits( pitch );

	if (pDstRow)
	{
    	WORD* pDst16;
    	BYTE bAlpha; // 4-bit measure of pixel intensity
		
    	for (y = 0; y < texH; y++)
    	{
    	    pDst16 = (WORD*)pDstRow;
    	    for (x = 0; x < texW; x++)
    	    {
    	        bAlpha = (BYTE)((pBitmapBits[texW * y + x] & 0xff) >> 4);
    	        if (bAlpha > 0)
    	        {
    	            *pDst16++ = (bAlpha << 12) | 0x0fff;
    	        }
    	        else
    	        {
    	            *pDst16++ = 0x0000;
    	        }
    	    }
    	    pDstRow += pitch;
    	}
		pTexture->UnlockBits();
	}

	// Clean up used GDI objects
	SelectObject( hDC, hOldBmp );
	SelectObject( hDC, hOldFont );

    DeleteObject( hBitmap );
	DeleteObject( hFont );

	DeleteDC( hDC );
    
	//pTexture->Save( "c:\\dumps\\_font.dds" );

	return true;
} // Font::Generate

/*****************************************************************************/
/*	FontMgr implementation
/*****************************************************************************/
class FontMgr : public NodeResMgr<Font>, public PSingleton<FontMgr>
{
public:

	NODE(FontMgr, Node, FMGR);
}; // class FontMgr

END_NAMESPACE(sg)
