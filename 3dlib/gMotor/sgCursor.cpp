/*****************************************************************************/
/*    File:    sgCursor.cpp
/*    Desc:    Mouse cursor node
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10-07-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include "sgNodePool.h"
#include "sgNode.h"
#include "sgTransformNode.h"
#include "kIOHelpers.h"
#include "sgCursor.h"

IMPLEMENT_CLASS( Cursor                );
IMPLEMENT_CLASS( SystemCursor        );
IMPLEMENT_CLASS( TexturedCursor        );
IMPLEMENT_CLASS( HardwareCursor        );
IMPLEMENT_CLASS( FramerateCursor    );

/*****************************************************************************/
/*  Cursor implementation
/*****************************************************************************/
Cursor::Cursor()
{
    m_HotSpotX = 0;
    m_HotSpotY = 0;
}

Cursor::~Cursor()
{
}

int    Cursor::GetCursorID( const char* name ) const
{    
    return c_NoID;
}

void Cursor::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "Cursor", this );
    pm.f( "HotSpotX", m_HotSpotX );
    pm.f( "HotSpotY",    m_HotSpotY );
} // Cursor::Expose

bool Cursor::OnMouseMove( int mX, int mY, DWORD keys )
{    
    if (GetFlagState( nfInvisible )) return false;
    m_PosX = mX;
    m_PosY = mY;
    Update( mX, mY ); 
    return false;
} // Cursor::OnMouseMove

void Cursor::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_HotSpotX << m_HotSpotY;
}

void Cursor::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
    is >> m_HotSpotX >> m_HotSpotY;
}

/*****************************************************************************/
/*  SystemCursor implementation
/*****************************************************************************/
SystemCursor::SystemCursor()
{
    m_hCursor = c_BadCursorHandle;
} // SystemCursor::SystemCursor

SystemCursor::~SystemCursor()
{
    if (m_hCursor != c_BadCursorHandle)
    {
        DestroyCursor( m_hCursor );
    }
} // SystemCursor::SystemCursor

void SystemCursor::Activate()
{
    if (m_hCursor == c_BadCursorHandle) m_hCursor = LoadCursorFromFile( GetFileName() );
    if (m_hCursor == c_BadCursorHandle)
    {
        m_hCursor = LoadCursor( NULL, IDC_ARROW );
        return;
    }

    //  show cursor
    if (m_hCursor != c_BadCursorHandle) ::SetCursor( m_hCursor );
    ::ShowCursor( TRUE );
} // SystemCursor::Activate

void SystemCursor::Update( int mX, int mY )
{
} // SystemCursor::Update

void SystemCursor::SetFileName( const char* val )
{ 
    char path    [_MAX_PATH];
    char drive    [_MAX_PATH];
    char dir    [_MAX_PATH];
    char fname    [_MAX_PATH];
    char ext    [_MAX_PATH];

    strcpy( path, val );
    //ToRelativePath( path, _MAX_PATH );
    _splitpath( path, drive, dir, fname, ext );
    m_FileName = path; 
    SetName( fname );
} // SystemCursor::SetFileName

void SystemCursor::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "SystemCursor", this );
    pm.m( "Activate", &SystemCursor::Activate );
    pm.p( "FileName", &SystemCursor::GetFileName, &SystemCursor::SetFileName, "file" );
} // SystemCursor::Expose

void SystemCursor::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_FileName;
}; // SystemCursor::Serialize

void SystemCursor::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
    is >> m_FileName;
} // SystemCursor::Unserialize

/*****************************************************************************/
/*  TexturedCursor implementation
/*****************************************************************************/
TexturedCursor::TexturedCursor()
{
    m_TexID = 0;
}

void TexturedCursor::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_TexName << m_TexRct;
}; // TexturedCursor::Serialize

void TexturedCursor::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
    is >> m_TexName >> m_TexRct;
} // TexturedCursor::Unserialize

void TexturedCursor::Expose( PropertyMap& pm )
{    
    pm.start<Parent>( "TexturedCursor", this );
    pm.p ( "TexName", &TexturedCursor::GetTextureName, &TexturedCursor::SetTextureName, "#texture" );
    pm.f( "Left",        m_TexRct.x );
    pm.f( "Top",        m_TexRct.y );
    pm.f( "Width",    m_TexRct.w );
    pm.f( "Height",    m_TexRct.h );
} // TexturedCursor::Expose

/*****************************************************************************/
/*  HardwareCursor implementation
/*****************************************************************************/
void HardwareCursor::Update( int mX, int mY )
{
    IRS->UpdateCursor( mX, mY, true );
}

void HardwareCursor::Activate()
{
    if (m_TexID == 0) m_TexID = IRS->GetTextureID( m_TexName.c_str() );
    if (m_TexID == 0) return;
    
    bool res = IRS->SetCursor( m_TexID, m_TexRct, m_HotSpotX, m_HotSpotY );
    if (!res) return;

    POINT pt;
    GetCursorPos( &pt );
    m_PosX = pt.x;
    m_PosY = pt.y;

    ::ShowCursor( TRUE );
    IRS->ShowCursor( true );
    
    res = IRS->UpdateCursor( m_PosX, m_PosY, true );
} // HardwareCursor::Activate

/*****************************************************************************/
/*  FramerateCursor implementation
/*****************************************************************************/
BaseMesh FramerateCursor::s_DrawMesh;

void FramerateCursor::Update( int mX, int mY )
{
    m_PosX = mX;
    m_PosY = mY;
} // FramerateCursor::Update

void FramerateCursor::Render()
{
    SNode::Render();

    if (s_DrawMesh.getMaxVert() == 0)
    {
        s_DrawMesh.create( 4, 0, vfVertexTnL );
        s_DrawMesh.setIsQuadList( true );
        s_DrawMesh.setNPri ( 2 );
        s_DrawMesh.setNVert( 4 );
        s_DrawMesh.setShader( IRS->GetShaderID( "hud" ) );
        s_DrawMesh.setTexture( m_TexID );
    }

    static const float c_HalfPixel = 0.5f;
    VertexTnL* v = (VertexTnL*)s_DrawMesh.getVertexData();
    v[0].x = float( m_PosX - m_HotSpotX ) - c_HalfPixel;
    v[0].y = float( m_PosY - m_HotSpotY ) - c_HalfPixel;
    v[0].z = GetPosZ();
    v[0].w = 1.0f;
    v[0].diffuse = 0xFFFFFFFF;

    v[0].u = m_UV.x; 
    v[0].v = m_UV.y; 

    v[1].x = v[0].x + m_TexRct.w;
    v[1].y = v[0].y;
    v[1].z = GetPosZ();
    v[1].w = 1.0f;
    v[1].diffuse = 0xFFFFFFFF;

    v[1].u = m_UV.x + m_UV.w; 
    v[1].v = m_UV.y; 

    v[2].x = v[0].x;
    v[2].y = v[1].y + m_TexRct.h;
    v[2].z = GetPosZ();
    v[2].w = 1.0f;
    v[2].diffuse = 0xFFFFFFFF;

    v[2].u = m_UV.x; 
    v[2].v = m_UV.y + m_UV.h; 

    v[3].x = v[1].x;
    v[3].y = v[2].y;
    v[3].z = GetPosZ();
    v[3].w = 1.0f;
    v[3].diffuse = 0xFFFFFFFF;

    v[3].u = v[1].u; 
    v[3].v = v[2].v; 
    
    DrawBM( s_DrawMesh );

} // FramerateCursor::Render

void FramerateCursor::Activate()
{
    if (m_TexID == 0) m_TexID = IRS->GetTextureID( m_TexName.c_str() );
    if (m_TexID == 0) return;

    float w = IRS->GetTextureWidth( m_TexID );
    m_UV.x = m_TexRct.x / w;
    m_UV.y = m_TexRct.y / w;

    m_UV.w = m_TexRct.w / w;
    m_UV.h = m_TexRct.h / w;

    ::ShowCursor( FALSE );
    IRS->ShowCursor( false );
} // FramerateCursor::Activate



