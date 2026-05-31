/*****************************************************************/
/*  File:   gpText.cpp
/*  Desc:   2D polygonal bitmap text
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#include "stdafx.h"
#include "gmDefines.h"
#include "gpText.h"

#include "IRenderSystem.h"
#include "mMath3D.h"

BEGIN_NAMESPACE(sg)
/*****************************************************************/
/*  GPFont implementation
/*****************************************************************/
GPFont::GPFont()
{
	posX			= 0.0f;
	posY			= 0.0f;
	depth			= 0.0000001f;
	nMaxTextSymbols = c_MaxTextSymbols;
	texID			= 0;
}

GPFont::~GPFont()
{
}

void GPFont::Init( const char* fontName, int _fontHeight )
{
	fontHeight = _fontHeight;
	CreateSymbolTable( fontName, fontHeight );
	
	nMaxTextSymbols = c_MaxTextSymbols;
	int numVert = nMaxTextSymbols * 4;
	mesh.create(	numVert, 
					0,
					vfTnL,
					ptTriangleList );
	mesh.setIsQuadList( true );

	mesh.setTexture( texID );
	static shaderID = RC::iRS->GetShaderID( "text3d" );
	mesh.setShader( shaderID );
}

void GPFont::CreateSymbolTable( const char* fontName, int _fontHeight )
{
	fontHeight = _fontHeight;
    float scale  = 1.0f;
	int texSide = 256;
	TextureDescr textTD;
	if (texID == 0) 
	{
		textTD.setValues( texSide, texSide, cfARGB4444, mpManaged, 1, tuProcedural );
		texID = RC::iRS->CreateTexture( fontName, textTD );
		assert( texID != 0 );
	}

	if (texID == 0)
	{
		Log.Warning( "Couldn't create font texture." );
		return;
	}

    DWORD*     pBitmapBits;
    BITMAPINFO bmi;
    ZeroMemory( &bmi.bmiHeader,  sizeof(BITMAPINFOHEADER) );
    bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth       =  texSide;
    bmi.bmiHeader.biHeight      = -texSide;
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biBitCount    = 32;

    //   DC and a bitmap for the font
    HDC     hDC       = CreateCompatibleDC( NULL );
    HBITMAP hBitmap	  = CreateDIBSection( hDC, &bmi, DIB_RGB_COLORS,
                                          (VOID**)&pBitmapBits, NULL, 0 );
    SetMapMode( hDC, MM_TEXT );

    //  create a font
    INT nHeight    = -MulDiv( fontHeight, 
        (INT)(GetDeviceCaps( hDC, LOGPIXELSY ) * scale), 72 );

    DWORD dwBold   = FALSE;
    DWORD dwItalic = FALSE;
    HFONT hFont    = CreateFont( nHeight, 0, 0, 0, dwBold, dwItalic,
                          FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                          CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
                          VARIABLE_PITCH, fontName );
    if (hFont == 0)
	{
		Log.Warning( "Couldn't create font %s.", fontName );
		return;
	}

    HGDIOBJ oldBmp = SelectObject( hDC, hBitmap );
    HGDIOBJ oldFnt = SelectObject( hDC, hFont );

    // Set text properties
    SetTextColor( hDC, RGB(255,255,255) );
    SetBkColor(   hDC, 0x00000000 );
    SetTextAlign( hDC, TA_TOP );

    // Loop through all printable character and output them to the bitmap..
    // Meanwhile, keep track of the corresponding tex coords for each character.
    DWORD x = 0;
    DWORD y = 0;
    char str[2] = "x";
    SIZE size;

    for (char c = 32; c < 127; c++)
    {
        str[0] = c;
        GetTextExtentPoint32( hDC, str, 1, &size );

        if ((int)(x + size.cx + 1) > texSide)
        {
            x  = 0;
            y += size.cy + 1;
        }

        ExtTextOut( hDC, x+0, y+0, ETO_OPAQUE, NULL, str, 1, NULL );

        texCoord[c-32][0] = ((float)(x))			/ texSide;
        texCoord[c-32][1] = ((float)(y))			/ texSide;
        texCoord[c-32][2] = ((float)(x + size.cx))	/ texSide;
        texCoord[c-32][3] = ((float)(y + size.cy))	/ texSide;

        x += size.cx+1;
    }

    // Lock the surface and write the alpha values for the set pixels
    int pitch = 0;
	BYTE* pDstRow = RC::iRS->LockTexBits( texID, pitch );
    WORD* pDst16;
    BYTE bAlpha; // 4-bit measure of pixel intensity

    for (y = 0; y < texSide; y++)
    {
        pDst16 = (WORD*)pDstRow;
        for (x = 0; x < texSide; x++)
        {
            bAlpha = (BYTE)((pBitmapBits[texSide * y + x] & 0xff) >> 4);
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

    // Done updating texture, so clean up used objects
    RC::iRS->UnlockTexBits( texID );
	
	SelectObject( hDC, oldBmp );
	SelectObject( hDC, oldFnt );

    DeleteDC( hDC );
    DeleteObject( hFont );
	DeleteObject( hBitmap );

	//char fname[512];
	//sprintf( fname, "c:\\dumps\\font.dds" );
	//RC::iRS->SaveTexture( texID, fname );
	
}

void GPFont::DrawString( const char* str, 
						float x, float y, float z,
						DWORD color, float height, GPTextAlignment align )
{
	if (height == -1.0f) height = fontHeight;
	int nSym = strlen( str );
	if (nSym > nMaxTextSymbols)
	{
		nMaxTextSymbols += nMaxTextSymbols / 2;
		int numVert = nMaxTextSymbols * 4;
		mesh.create(	numVert, 
						0,
						vfTnL,
						ptTriangleList );
	}

	VertexTnL* vbuf = (VertexTnL*)mesh.getVertexData();

	Vector4D pos( x, y, z, 1.0f );

	float curX = pos.x;
	float curY = pos.y;
	int cV = 0;
	float symW, symH, symU, symV;

	for (int i = 0; i < nSym; i++)
	{
		char c = str[i];

		symU = texCoord[c - 32][0];
		symV = texCoord[c - 32][1];
		symW = (texCoord[c - 32][2] - texCoord[c - 32][0]) * 256.0f;
		symH = (texCoord[c - 32][3] - texCoord[c - 32][1]) * 256.0f;
		symH *= height / fontHeight;

		vbuf[cV].setXYZW( curX, curY, depth, 1.0f );
		vbuf[cV].u = symU;
		vbuf[cV].v = symV;
		vbuf[cV].diffuse = color;
		cV++;

		vbuf[cV].setXYZW( curX + symW, curY, depth, 1.0f );
		vbuf[cV].u = texCoord[c - 32][2];
		vbuf[cV].v = symV;
		vbuf[cV].diffuse = color;
		cV++;

		vbuf[cV].setXYZW( curX, curY + symH, depth, 1.0f );
		vbuf[cV].u = symU;
		vbuf[cV].v = texCoord[c - 32][3];
		vbuf[cV].diffuse = color;
		cV++;

		vbuf[cV].setXYZW( curX + symW, curY + symH, depth, 1.0f );
		vbuf[cV].u = texCoord[c - 32][2];
		vbuf[cV].v = texCoord[c - 32][3];
		vbuf[cV].diffuse = color;
		cV++;
		
		curX += symW + 1;
	}

	mesh.setNVert	( nSym * 4 );
	mesh.setNPri	( nSym * 2 );

	bool st = mesh.isStatic();
	bool ql = mesh.isQuadList();
	RC::iRS->Draw( mesh );
}

END_NAMESPACE(sg)

