/*****************************************************************************/
/*    File:    vFontManager.cpp
/*    Desc:    Implementation of the ui manager
/*    Author:    Ruslan Shestopalyuk
/*    Date:    08-20-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include "vFontManager.h"
#include "uiControl.h"

//  global instance of the widget manager
FontManager        g_WMgr;

//  global interface pointer
IFontManager* IWM = &g_WMgr;

IMPLEMENT_CLASS( FontManager );

/*****************************************************************************/
/*    BitmapFont implementation
/*****************************************************************************/
BitmapFont::BitmapFont() : m_StartCode(0), m_TexID(-1), m_NChars(0)
{ 
    m_Name[0] = 0; 
    m_FreeSpace.Set( 0.0f, 0.0f, 0.0f, 0.0f );
    m_FreeLine.Zero(); 
    m_NCharInst = 0;
    m_Type = ftUnknown;
}

int    BitmapFont::AllocateChar( BYTE code, float width, float height )
{
    if (width == 0 || height == 0) return -1;
    if (m_NChars == 256) return -1;
    Char& ch = m_Char[m_NChars++];

    if (m_FreeLine.w <= width + 8 || m_FreeLine.h < height)
    {
        if (m_FreeSpace.h < height || m_FreeSpace.w < width) return NULL;
        ch.m_Ext.x         = 0.0f;
        ch.m_Ext.y         = m_FreeSpace.y;
        ch.m_Ext.w         = width;
        ch.m_Ext.h         = height;

        m_FreeLine.w    =  m_FreeSpace.w;
        m_FreeLine.h    =  height;
        m_FreeLine.x    =  width;
        m_FreeLine.y    =  m_FreeSpace.y;

        m_FreeSpace.y     += height;
        m_FreeSpace.h     -= height;
    }
    else
    {
        ch.m_Ext.x         = m_FreeLine.x;
        ch.m_Ext.y         = m_FreeLine.y;
        ch.m_Ext.w         = width;
        ch.m_Ext.h         = height;

        m_FreeLine.w     -= width;
        m_FreeLine.x     += width;
    }

    ch.m_UV.x = ch.m_Ext.x / float( m_TexWidth );
    ch.m_UV.w = ch.m_Ext.w / float( m_TexWidth );
    ch.m_UV.y = ch.m_Ext.y / float( m_TexHeight );
    ch.m_UV.h = ch.m_Ext.h / float( m_TexHeight );
    
    ch.m_Code = code;

    return m_NChars - 1;
} // BitmapFont::AllocateChar


const float c_HalfPixel = 0.5f;
void BitmapFont::Flush()
{
    if (m_NCharInst == 0) return;
    static BaseMesh        s_BM;
    static int shID    = IRS->GetShaderID( "text" );
    if (s_BM.getNVert() == 0)
    {
        s_BM.create( c_MaxCharInst*4, c_MaxCharInst*6, vfVertexTnL );
        WORD* pIdx = s_BM.getIndices();
        int cV = 0;
        for (int i = 0; i < c_MaxCharInst; i++)
        {
            pIdx[i*6 + 0] = cV;
            pIdx[i*6 + 1] = cV + 1;
            pIdx[i*6 + 2] = cV + 2;
            pIdx[i*6 + 3] = cV + 2;
            pIdx[i*6 + 4] = cV + 1;
            pIdx[i*6 + 5] = cV + 3;
            cV += 4;
        }
    }
    
    VertexTnL* v = (VertexTnL*) s_BM.getVertexData();
    for (int i = 0; i < m_NCharInst; i++)
    {
        CharInst& ch = m_CharInst[i];
        VertexTnL& v0 = v[i*4 + 0];
        VertexTnL& v1 = v[i*4 + 1];
        VertexTnL& v2 = v[i*4 + 2];
        VertexTnL& v3 = v[i*4 + 3];
        
        float z = ch.m_Pos.z;

        v0.x = ch.m_Pos.x - c_HalfPixel; 
        v0.y = ch.m_Pos.y - c_HalfPixel; 
        v0.z = z; 

        v0.u = ch.m_UV.x; 
        v0.v = ch.m_UV.y;

        v1.x = v0.x + ch.w; 
        v1.y = v0.y; 
        v1.z = z;

        v1.u = ch.m_UV.GetRight(); 
        v1.v = ch.m_UV.y;

        v2.x = v0.x; 
        v2.y = v0.y + ch.h; 
        v2.z = z;

        v2.u = ch.m_UV.x; 
        v2.v = ch.m_UV.GetBottom();

        v3.x = v1.x; 
        v3.y = v2.y; 
        v3.z = z;

        v3.u = v1.u; 
        v3.v = v2.v;

        v0.diffuse = ch.m_Color;
        v1.diffuse = ch.m_Color;
        v2.diffuse = ch.m_Color;
        v3.diffuse = ch.m_Color;

        v0.w = 1.0f;
        v1.w = 1.0f;
        v2.w = 1.0f;
        v3.w = 1.0f;
    }
    
    s_BM.setNPri    ( m_NCharInst*2 );
    s_BM.setNInd    ( m_NCharInst*6 );
    s_BM.setNVert    ( m_NCharInst*4 );
    s_BM.setTexture    ( m_TexID );
    s_BM.setShader    ( shID );
    DrawBM( s_BM );
    m_NCharInst = 0;
} // BitmapFont::Flush

/*****************************************************************************/
/*    FontManager implementation
/*****************************************************************************/
FontManager::FontManager()
{
    m_NFonts = 0;
}

FontManager::~FontManager()
{
    for (int i = 0; i < m_NFonts; i++)
    {
        delete m_Fonts[i];
    }
}

void FontManager::OnDrop( int mX, int mY, DWORD ctx, DWORD obj )
{
    //  FIXME
    Iterator it( Root::instance(), Control::FnFilter );
    while (it)
    {
        Control* pCtl = (Control*)*it;
        pCtl->OnDrop( mX, mY, ctx, obj );
        ++it;
    }
} // FontManager::OnDrop

int    FontManager::GetFontID( const char* name )
{
    int nF = m_NFonts;
    for (int i = 0; i < nF; i++)
    {
        if (!stricmp( name, m_Fonts[i]->m_Name )) return i;
    }
    return -1;
} // FontManager::GetFontID

void FontManager::DestroyFont( int fontID )
{
}

void FontManager::CreateFullFontName( char* fullName, const char* fontName, int charW, int charH )
{
    sprintf( fullName, "%s%d_%d", fontName, charW, charH );
    _strlwr( fullName );
} // FontManager::CreateFullFontName

void FontManager::CreateFullFontName( char* fullName, const char* fontName, int height, DWORD charset, bool bBold, bool bItalic )
{
    sprintf( fullName, "%s%d", fontName, height );
    if (charset == RUSSIAN_CHARSET) strcat( fullName, "ru" );
    if (bBold) strcat( fullName, "b" );
    if (bItalic) strcat( fullName, "i" );
    _strlwr( fullName );
} // FontManager::CreateFullFontName

int    FontManager::CreateFont( const char* name )
{
    if (m_NFonts == c_MaxFonts)
    {
        Log.Error( "Maximal number of fonts reached." );
        return -1;
    }
    BitmapFont* font = new BitmapFont();
    m_Fonts[m_NFonts] = font;
    strcpy( font->m_Name, name );
    _strlwr( font->m_Name );
    m_NFonts++;
    return m_NFonts - 1;
} // FontManager::CreateFont

int    FontManager::CreateFont( const char* name, int height, DWORD charset, bool bBold, bool bItalic )
{
    char fullName[256];
    CreateFullFontName( fullName, name, height, charset, bBold, bItalic );
    int id = GetFontID( fullName );
    if (id != -1) return id;
    id = CreateFont( fullName );
    BitmapFont* pFont = m_Fonts[id];

    pFont->m_Type = ftGDIGenerated;

    //   create dc for the font
    HDC     hDC       = CreateCompatibleDC( NULL );

    //  create a font
    INT nHeight    = -MulDiv( height, (INT)(GetDeviceCaps( hDC, LOGPIXELSY ) * 1.0f), 72 );

    DWORD dwBold   = bBold   ? TRUE : FALSE;
    DWORD dwItalic = bItalic ? TRUE : FALSE;
    HFONT hFont    = ::CreateFont( -nHeight, 0, 0, 0, dwBold, dwItalic, FALSE, FALSE, charset, OUT_DEFAULT_PRECIS,
                                    CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, name );
    if (hFont == 0)
    {
        Log.Warning( "Couldn't create font %s.", fullName );
        return false;
    }

    // Set text properties
    HGDIOBJ hOldFont = SelectObject( hDC, hFont    );
    SetTextColor( hDC, RGB(255,255,255) );
    SetBkColor    ( hDC, 0x00000000        );
    SetTextAlign( hDC, TA_TOP            );

    //  first find how much place our font will occupy on the texture
    int texW = 256;
    int texH = 128;
    unsigned char str[2] = "x";
    SIZE size;
    for (int i = 32; i < 256; i++)
    {
        str[0] = i;
        GetTextExtentPoint32( hDC, (LPCSTR)str, 1, &size );
    }
    
    //  create texture for the font
    int texID = IRS->CreateTexture( fullName, texW, texH, cfARGB4444, 1, tmpManaged );
    pFont->m_TexID        = texID;
    pFont->m_TexWidth    = texW;
    pFont->m_TexHeight    = texH;
    pFont->m_FreeSpace.Set( 0.0f, 0.0f, texW, texH );
    if (pFont->m_TexID == -1) 
    {
        Log.Error( "Could not create texture for font %s", fullName );
    }

    DWORD*     pBitmapBits;
    BITMAPINFO bmi;
    ZeroMemory( &bmi.bmiHeader,  sizeof(BITMAPINFOHEADER) );
    bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth       = texW;
    bmi.bmiHeader.biHeight      = -texH;
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biBitCount    = 32;

    //  create bitmap
    HBITMAP hBitmap      = CreateDIBSection( hDC, &bmi, DIB_RGB_COLORS, (VOID**)&pBitmapBits, NULL, 0 );
    SetMapMode( hDC, MM_TEXT );

    HGDIOBJ hOldBmp = SelectObject( hDC, hBitmap );
    
    pFont->m_StartCode = 32;
    // Loop through all printable Glyphs and output them to the bitmap.
    // Meanwhile, keep track of the corresponding tex coords for each Glyph.
    DWORD x = 0;
    DWORD y = 0;
    for (int i = 32; i < 256; i++)
    {
        str[0] = i;
        DWORD code = i;
        GetTextExtentPoint32( hDC, (LPCSTR)str, 1, &size );
        
        int chID = pFont->AllocateChar( code, size.cx, size.cy );
        if (chID < 0)
        {
            Log.Error( "Could not allocate all font chars." );
            return -1;
        }
        BitmapFont::Char& ch = pFont->m_Char[chID];
        ExtTextOut( hDC, ch.m_Ext.x, ch.m_Ext.y, ETO_OPAQUE, NULL, (LPCSTR)str, 1, NULL );
    }

    // Lock the surface and write the alpha values for the set pixels
    int pitch = 0;
    BYTE* pDstRow = IRS->LockTexBits( texID, pitch );

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
        IRS->UnlockTexBits( texID );
    }

    // Clean up used GDI objects
    SelectObject( hDC, hOldBmp );
    SelectObject( hDC, hOldFont );

    DeleteObject( hBitmap );
    DeleteObject( hFont );

    DeleteDC( hDC );

    ///IRS->SaveTexture( texID, "c:\\dumps\\__font__.dds" );

    return id;
} // FontManager::CreateFont

int    FontManager::CreateUniformFont( const char* texName, int charW, int charH )
{
    char fullName[256];
    CreateFullFontName( fullName, texName, charW, charH );
    int id = GetFontID( fullName );
    if (id != -1) return id;
    id = CreateFont( fullName );
    BitmapFont* pFont = m_Fonts[id];
    pFont->m_Type = ftUniform;
    
    int texID = IRS->GetTextureID( texName );
    if (texID == -1) return -1;
    pFont->m_TexID = texID;
    int texW = IRS->GetTextureWidth( texID );
    int texH = IRS->GetTextureHeight( texID );

    int x = 0; 
    int y = 0;
    int i = 0;
    for (i = 0; i < 256; i ++)
    {
        BitmapFont::Char& ch = pFont->m_Char[i];
        ch.m_Code = i;
        ch.m_Ext = Rct( x, y, charW, charH );

        ch.m_UV.x = ch.m_Ext.x / float( texW );
        ch.m_UV.w = ch.m_Ext.w / float( texW );
        ch.m_UV.y = ch.m_Ext.y / float( texH );
        ch.m_UV.h = ch.m_Ext.h / float( texH );

        x += charW;
        if (x >= texW)
        {
            x = 0;
            y += charH;
            if (y >= texH) break;
        }
    }    
    pFont->m_NChars = i;
    pFont->m_TexWidth  = texW;
    pFont->m_TexHeight = texH;
    return id;
} // FontManager::CreateUniformFont

int    FontManager::GetStringWidth( int fontID, const char* str, int spacing )
{
    if (fontID < 0 || fontID >= m_NFonts) return 0;
    BitmapFont* pFont = m_Fonts[fontID];
    
    int w = 0;
    const char* ch = str;
    while (*ch)
    {
        w += pFont->GetCharW( *ch ) + spacing;
        ch++;
    }
    return w; 
} // FontManager::GetStringWidth

bool FontManager::DrawStringW( int fontID, const char* str, const Vector3D& pos, DWORD color, int spacing )
{
    Vector4D wpos( pos );
    ICamera* pCam = GetCamera();
    if (!pCam) return false;
    pCam->ToSpace( sWorld, sScreen,  wpos );
    return DrawString( fontID, str, wpos, color, spacing );
} // FontManager::DrawStringW

bool FontManager::DrawString( int fontID, const char* str, const Vector3D& pos, DWORD color, int spacing )
{
    if (fontID < 0 || fontID >= m_NFonts) return false;
    BitmapFont* pFont = m_Fonts[fontID];
    Vector3D p( pos );
    int ch = 0;
    while (str[ch])
    {
        pFont->AddCharInst( str[ch], p, color );
        p.x += pFont->GetCharW( str[ch] ) + spacing;
        ch++;
    }
    return true; 
} // FontManager::DrawString

bool FontManager::DrawChar( int fontID, const Vector3D& pos, BYTE ch, DWORD color )
{
    if (fontID < 0 || fontID >= m_NFonts) return false;
    BitmapFont* pFont = m_Fonts[fontID];
    pFont->AddCharInst( ch, pos, color );
    return true; 
} // FontManager::DrawChar 

bool FontManager::DrawChar( int fontID, const Vector3D& pos, const Rct& uv, DWORD color )
{
    if (fontID < 0 || fontID >= m_NFonts) return false;
    BitmapFont* pFont = m_Fonts[fontID];
    pFont->AddCharInst( uv, pos, color );
    return true; 
} // FontManager::DrawChar

bool FontManager::DrawChar( int fontID, const Vector3D& pos, const Rct& uv, float w, float h, DWORD color )
{
    if (fontID < 0 || fontID >= m_NFonts) return false;
    BitmapFont* pFont = m_Fonts[fontID];
    pFont->AddCharInst( uv, pos, w, h, color );
    return true; 
} // FontManager::DrawChar

int    FontManager::GetCharWidth( int fontID, BYTE ch )
{
    if (fontID < 0 || fontID >= m_NFonts) return 0;
    BitmapFont* pFont = m_Fonts[fontID];
    return pFont->GetCharW( ch );
} // FontManager::GetCharWidth

int    FontManager::GetCharHeight( int fontID, BYTE ch )
{
    if (fontID < 0 || fontID >= m_NFonts) return 0;
    BitmapFont* pFont = m_Fonts[fontID];
    return pFont->GetCharH( ch );
} // FontManager::GetCharHeight

void FontManager::FlushText( int fontID )
{
    if (fontID < 0 || fontID >= m_NFonts) 
    {
        for (int i = 0; i < m_NFonts; i++) m_Fonts[i]->Flush();
        return;
    }
    BitmapFont* pFont = m_Fonts[fontID];
    pFont->Flush();
} // FontManager::FlushText

void FontManager::OnDestroyRS()
{
    for (int i = 0; i < m_NFonts; i++)
    {
        BitmapFont* pFont = m_Fonts[i];
        if (pFont->m_Type == ftUniform)
        {
        
        }
        else if (pFont->m_Type == ftGDIGenerated)
        {
        
        }
    }
} // FontManager::OnDestroyRS


