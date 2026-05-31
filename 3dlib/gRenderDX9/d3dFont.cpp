/*****************************************************************************/
/*	File:	d3dFont.cpp
/*  Desc:	Font interface implementation for DirectX9 
/*	Author:	Ruslan Shestopalyuk
/*	Date:	02.11.2004
/*****************************************************************************/
#include "gRenderPch.h"
#include "d3dFont.h"

/*****************************************************************************/
/*  FontDX9 implementation
/*****************************************************************************/
FontDX9::FontDX9( IDirect3DDevice9* pDevice )
{
    m_pDevice = pDevice;
    m_pFont = NULL;
} // FontDX9::FontDX9

FontDX9::~FontDX9()
{
}

bool FontDX9::Create( const char* name, int height, DWORD charset, bool bBold, bool bItalic )
{
    HDC hDC = GetDC( NULL );
    int nHeight = -MulDiv( height, GetDeviceCaps( hDC, LOGPIXELSY ), 72 );
    ReleaseDC( NULL, hDC );
    HRESULT hr = D3DXCreateFont( m_pDevice, nHeight, 0, (DWORD)bBold, 0, (DWORD)bItalic, 
                                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
                                    DEFAULT_PITCH | FF_DONTCARE, name, &m_pFont );
    return (hr == S_OK);
} // FontDX9::Create

bool FontDX9::Create( const char* texName, int charW, int charH )
{
    return false;
} // FontDX9::Create

int	FontDX9::GetStringWidth( const char* str, int spacing )
{
    return 0;
} // FontDX9::GetStringWidth

int	FontDX9::GetCharWidth( BYTE ch )
{
    return 0;
} // FontDX9::GetCharWidth

int	FontDX9::GetCharHeight( BYTE ch )
{
    return 0;
} // FontDX9::GetCharHeight

bool FontDX9::DrawString( const char* str, const Vector3D& pos, DWORD color, int spacing )
{
    return false;
} // FontDX9::DrawString

bool FontDX9::DrawString3D( const char* str, const Vector3D& pos, DWORD color , int spacing )
{
    return false;
} // FontDX9::DrawString3D

bool FontDX9::DrawChar( const Vector3D& pos, BYTE ch, DWORD color )
{
    return false;
} // FontDX9::DrawChar

bool FontDX9::DrawChar( const Vector3D& pos, const Rct& uv, DWORD color )
{
    return false;
} // FontDX9::DrawChar

bool FontDX9::DrawChar( const Vector3D& pos, const Rct& uv, float w, float h, DWORD color )
{
    return false;
} // FontDX9::DrawChar

void FontDX9::Flush()
{

} // FontDX9::Flush

void FontDX9::DeleteDeviceObjects()
{
    SAFE_RELEASE( m_pFont );
} // FontDX9::DeleteDeviceObjects

void FontDX9::InvalidateDeviceObjects()
{
    if (m_pFont) m_pFont->OnLostDevice();
} // FontDX9::InvalidateDeviceObjects

void FontDX9::RestoreDeviceObjects()
{
    if (m_pFont) m_pFont->OnResetDevice();
} // FontDX9::RestoreDeviceObjects