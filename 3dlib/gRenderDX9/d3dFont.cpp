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

FontDX9::FontDX9(IDirect3DDevice9 *pDevice)
{
    m_pDevice = pDevice;
    m_pFont = NULL;
    m_ID = -1;
    m_Name = "";
} // FontDX9::FontDX9


FontDX9::~FontDX9()
{
    DeleteDeviceObjects();

    m_pDevice = NULL;
} // FontDX9::~FontDX9


/*****************************************************************************/
/*  Create
/*****************************************************************************/

bool FontDX9::Create(const char *name, int height, DWORD charset,
    bool bBold, bool bItalic)
{
    if (m_pDevice == NULL)
        return false;

    if (name == NULL || name[0] == 0)
        return false;

    if (height <= 0)
        return false;

    /*
        If this FontDX9 object already owns a font, release it before
        creating a new one.
    */
    DeleteDeviceObjects();

    HDC hDC = GetDC(NULL);

    if (hDC == NULL)
        return false;

    int logPixelsY = GetDeviceCaps(hDC, LOGPIXELSY);

    ReleaseDC(NULL, hDC);

    if (logPixelsY <= 0)
        logPixelsY = 96;

    /*
        D3DXCreateFont expects the font height in logical units.
        Negative height means character height rather than cell height.
    */
    int nHeight = -MulDiv(height, logPixelsY, 72);

    if (nHeight == 0)
        nHeight = -height;

    HRESULT hr = D3DXCreateFont(
        m_pDevice,
        nHeight,
        0,
        bBold ? FW_BOLD : FW_NORMAL,
        0,
        bItalic ? TRUE : FALSE,
        charset,
        OUT_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        name,
        &m_pFont
    );

    if (FAILED(hr))
    {
        m_pFont = NULL;
        return false;
    }

    m_Name = name;

    return true;
} // FontDX9::Create


/*****************************************************************************/
/*  Create bitmap/texture font
/*
/*  Not implemented yet.
/*****************************************************************************/

bool FontDX9::Create(const char *texName, int charW, int charH)
{
    /*
        This is a different font system based on a texture atlas.
        The old engine interface supports it, but DX9 ID3DXFont does not
        directly provide the same functionality.

        Keep this disabled until the original texture-font implementation
        is located/restored.
    */

    return false;
} // FontDX9::Create


/*****************************************************************************/
/*  GetStringWidth
/*****************************************************************************/

int FontDX9::GetStringWidth(const char *str, int spacing)
{
    if (m_pFont == NULL || str == NULL || str[0] == 0)
        return 0;

    RECT rc;
    SetRect(&rc, 0, 0, 0, 0);

    /*
        DT_CALCRECT calculates the required rectangle without rendering.
    */
    m_pFont->DrawTextA(
        NULL,
        str,
        -1,
        &rc,
        DT_LEFT | DT_TOP | DT_CALCRECT | DT_SINGLELINE,
        0
    );

    int width = rc.right - rc.left;

    /*
        ID3DXFont calculates normal glyph spacing itself.
        Additional spacing is applied between characters.
    */
    if (spacing != 1)
    {
        int length = lstrlenA(str);

        if (length > 1)
            width += (length - 1) * (spacing - 1);
    }

    return width;
} // FontDX9::GetStringWidth


/*****************************************************************************/
/*  GetCharWidth
/*****************************************************************************/

int FontDX9::GetCharWidth(BYTE ch)
{
    if (m_pFont == NULL)
        return 0;

    char str[2];
    str[0] = (char)ch;
    str[1] = 0;

    RECT rc;
    SetRect(&rc, 0, 0, 0, 0);

    m_pFont->DrawTextA(
        NULL,
        str,
        1,
        &rc,
        DT_LEFT | DT_TOP | DT_CALCRECT | DT_SINGLELINE,
        0
    );

    return rc.right - rc.left;
} // FontDX9::GetCharWidth


/*****************************************************************************/
/*  GetCharHeight
/*****************************************************************************/

int FontDX9::GetCharHeight(BYTE ch)
{
    if (m_pFont == NULL)
        return 0;

    char str[2];
    str[0] = (char)ch;
    str[1] = 0;

    RECT rc;
    SetRect(&rc, 0, 0, 0, 0);

    m_pFont->DrawTextA(
        NULL,
        str,
        1,
        &rc,
        DT_LEFT | DT_TOP | DT_CALCRECT | DT_SINGLELINE,
        0
    );

    return rc.bottom - rc.top;
} // FontDX9::GetCharHeight


/*****************************************************************************/
/*  DrawString
/*****************************************************************************/

bool FontDX9::DrawString(const char *str,
    const Vector3D &pos,
    DWORD color,
    int spacing)
{
    if (m_pFont == NULL || str == NULL || str[0] == 0)
        return false;

    /*
        ID3DXFont uses screen coordinates for DrawText.
        Vector3D::x / y are therefore interpreted as screen coordinates.

        DT_NOCLIP prevents D3DX from clipping the string against the
        supplied rectangle.
    */
    RECT rc;

    int x = (int)pos.x;
    int y = (int)pos.y;

    SetRect(
        &rc,
        x,
        y,
        x + 32767,
        y + 32767
    );

    DWORD flags = DT_LEFT | DT_TOP | DT_NOCLIP | DT_SINGLELINE;

    if (spacing == 1)
    {
        HRESULT hr = m_pFont->DrawTextA(
            NULL,
            str,
            -1,
            &rc,
            flags,
            color
        );

        return SUCCEEDED(hr);
    }

    /*
        ID3DXFont has no character-spacing parameter.
        Draw characters individually when custom spacing is requested.
    */
    int currentX = x;

    const char *p = str;

    while (*p)
    {
        char ch[2];
        ch[0] = *p;
        ch[1] = 0;

        RECT charRect;

        SetRect(
            &charRect,
            currentX,
            y,
            currentX + 32767,
            y + 32767
        );

        HRESULT hr = m_pFont->DrawTextA(
            NULL,
            ch,
            1,
            &charRect,
            flags,
            color
        );

        if (FAILED(hr))
            return false;

        int charWidth = GetCharWidth((BYTE)*p);

        currentX += charWidth + (spacing - 1);

        ++p;
    }

    return true;
} // FontDX9::DrawString


/*****************************************************************************/
/*  DrawString3D
/*****************************************************************************/

bool FontDX9::DrawString3D(const char *str,
    const Vector3D &pos,
    DWORD color,
    int spacing)
{
    /*
        ID3DXFont::DrawText renders in screen space.
        A real 3D implementation would require projecting the 3D position
        through the current camera/view/projection matrices.

        Until the original engine's 3D text path is restored, use the
        screen-space implementation.
    */

    return DrawString(str, pos, color, spacing);
} // FontDX9::DrawString3D


/*****************************************************************************/
/*  DrawChar
/*****************************************************************************/

bool FontDX9::DrawChar(const Vector3D &pos,
    BYTE ch,
    DWORD color)
{
    if (m_pFont == NULL)
        return false;

    char str[2];
    str[0] = (char)ch;
    str[1] = 0;

    RECT rc;

    SetRect(
        &rc,
        (int)pos.x,
        (int)pos.y,
        (int)pos.x + 32767,
        (int)pos.y + 32767
    );

    HRESULT hr = m_pFont->DrawTextA(
        NULL,
        str,
        1,
        &rc,
        DT_LEFT | DT_TOP | DT_NOCLIP | DT_SINGLELINE,
        color
    );

    return SUCCEEDED(hr);
} // FontDX9::DrawChar


/*****************************************************************************/
/*  DrawChar
/*
/*  Texture UV version.
/*
/*  ID3DXFont does not expose glyph UV coordinates, therefore this version
/*  cannot be directly implemented using ID3DXFont.
/*****************************************************************************/

bool FontDX9::DrawChar(const Vector3D &pos,
    const Rct &uv,
    DWORD color)
{
    return false;
} // FontDX9::DrawChar


/*****************************************************************************/
/*  DrawChar
/*
/*  Texture UV + size version.
/*****************************************************************************/

bool FontDX9::DrawChar(const Vector3D &pos,
    const Rct &uv,
    float w,
    float h,
    DWORD color)
{
    return false;
} // FontDX9::DrawChar


/*****************************************************************************/
/*  Flush
/*****************************************************************************/

void FontDX9::Flush()
{
    /*
        ID3DXFont internally batches draw calls.
        There is no explicit flush operation exposed by ID3DXFont.
    */
} // FontDX9::Flush


/*****************************************************************************/
/*  Device objects
/*****************************************************************************/

void FontDX9::DeleteDeviceObjects()
{
    SAFE_RELEASE(m_pFont);
} // FontDX9::DeleteDeviceObjects


void FontDX9::InvalidateDeviceObjects()
{
    if (m_pFont)
        m_pFont->OnLostDevice();
} // FontDX9::InvalidateDeviceObjects


void FontDX9::RestoreDeviceObjects()
{
    if (m_pFont)
        m_pFont->OnResetDevice();
} // FontDX9::RestoreDeviceObjects