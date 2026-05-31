/*****************************************************************************/
/*    File:    rsRenderSystem.cpp
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    23.01.2003
/*****************************************************************************/
#include "stdafx.h" 
#include "mMath2D.h"
#include "vMesh.h"
#include "rsRenderSystem.h"
#include "ICamera.h"
/*****************************************************************************/
/*    Primitive drawing utilities implementation
/*****************************************************************************/
static BaseMesh     g_bmLines2D, g_bmLines3D, g_bmPoly2D, g_bmPoly3D;
const int           c_PrimPoolSize      = 4096;
static int          g_LineShader        = 0;
static int          g_LastShader        = 0;
static bool         g_bLinesZEnable     = false;

const RenderTask  RenderTask::c_Invalid;

DIALOGS_API void rsRestoreShader()
{
    g_LineShader = g_LastShader;
    g_bmLines2D.setShader( g_LineShader ); 
    g_bmLines3D.setShader( g_LineShader );
    g_bmPoly2D.setShader ( g_LineShader );
    g_bmPoly3D.setShader ( g_LineShader );
}

DIALOGS_API void rsSetTexture( int texID, int stage )
{
    g_bmPoly2D.setTexture( texID, stage );
    g_bmPoly3D.setTexture( texID, stage );
}

DIALOGS_API void rsSetShader( int shader )
{
    g_LastShader = g_LineShader;
    g_bmLines2D.setShader( shader ); 
    g_bmLines3D.setShader( shader );
    g_bmPoly2D.setShader ( shader );
    g_bmPoly3D.setShader ( shader );
} // rsSetShader

void rsEnableZ( bool enable )
{
    if (g_bLinesZEnable == enable) return;

    static int lines3D_blend     = IRS->GetShaderID( "linesZ.fx" );
    static int lines3D_blend_noZ = IRS->GetShaderID( "lines.fx" );
    if (enable) 
    {
        g_LineShader = lines3D_blend;
    }
    else
    {
        g_LineShader = lines3D_blend_noZ;
    }
    g_bLinesZEnable = enable;
    g_bmLines2D.setShader( g_LineShader ); 
    g_bmLines3D.setShader( g_LineShader );
    g_bmPoly2D.setShader( g_LineShader );
    g_bmPoly3D.setShader( g_LineShader );
} // rsEnableZ

DIALOGS_API void rsLine( float x1, float y1, float x2, float y2, float z, DWORD color )
{
    rsLine( x1, y1, x2, y2, z, color, color );
}

DIALOGS_API void rsLine( float x1, float y1, float x2, float y2, float z, DWORD color1, DWORD color2 )
{
    if (g_bmLines2D.getMaxVert() == 0)
    {
        g_bmLines2D.create( c_PrimPoolSize, 0, vfVertexTnL, ptLineList );    
        rsEnableZ();
    }

    int nV = g_bmLines2D.getNVert();
    if (nV + 2 >= c_PrimPoolSize) 
    {
        rsFlushLines2D();
        nV = 0;
    }

    VertexTnL* v = (VertexTnL*)g_bmLines2D.getVertexData();
    VertexTnL& v1 = v[nV];
    VertexTnL& v2 = v[nV + 1];
    
    v1.x = x1; v1.y = y1; v1.z = z; v1.w = 1.0f;
    v2.x = x2; v2.y = y2; v2.z = z;    v2.w = 1.0f;

    v1.diffuse = color1;        
    v2.diffuse = color2;    
    
    g_bmLines2D.setNVert( nV + 2 );
    g_bmLines2D.setNPri( (nV>>1) + 1 );
} // rsLine

DIALOGS_API void rsLine( float x1, float y1, float z1, float x2, float y2, float z2, DWORD color1, DWORD color2 )
{
    if (g_bmLines2D.getMaxVert() == 0)
    {
        g_bmLines2D.create( c_PrimPoolSize, 0, vfVertexTnL, ptLineList );    
        rsEnableZ();
    }

    int nV = g_bmLines2D.getNVert();
    if (nV + 2 >= c_PrimPoolSize) 
    {
        rsFlushLines2D();
        nV = 0;
    }

    VertexTnL* v = (VertexTnL*)g_bmLines2D.getVertexData();
    VertexTnL& v1 = v[nV];
    VertexTnL& v2 = v[nV + 1];

    v1.x = x1; v1.y = y1; v1.z = z1; v1.w = 1.0f;
    v2.x = x2; v2.y = y2; v2.z = z2; v2.w = 1.0f;

    v1.diffuse = color1;        
    v2.diffuse = color2;    

    g_bmLines2D.setNVert( nV + 2 );
    g_bmLines2D.setNPri( (nV>>1) + 1 );
} // rsLine

void rsFrame( const Rct& rct, float z, DWORD color )
{
    rsLine( rct.x, rct.y, rct.x + rct.w - 1, rct.y, z, color );
    rsLine( rct.x + rct.w - 1, rct.y, rct.x + rct.w - 1, rct.y + rct.h - 1, z, color );
    rsLine( rct.x + rct.w - 1, rct.y + rct.h - 1, rct.x, rct.y + rct.h - 1, z, color );
    rsLine( rct.x, rct.y + rct.h - 1, rct.x, rct.y, z, color );
}

DIALOGS_API void rsLine( const Vector3D& a, const Vector3D& b, DWORD color )
{
    rsLine( a, b, color, color );
}

DIALOGS_API void rsLine( const Vector3D& a, const Vector3D& b, DWORD color1, DWORD color2 )
{
    if (g_bmLines3D.getMaxVert() == 0)
    {
        g_bmLines3D.create( c_PrimPoolSize, 0, vfVertex2t, ptLineList );
        rsEnableZ();
    }

    int nV = g_bmLines3D.getNVert();
    if (nV + 2 >= c_PrimPoolSize) 
    {
        rsFlushLines3D();
        nV = 0;
    }
    
    Vertex2t* v = (Vertex2t*)g_bmLines3D.getVertexData();
    Vertex2t& v1 = v[nV];
    Vertex2t& v2 = v[nV + 1];
    
    v1.x = a.x; v1.y = a.y; v1.z = a.z; 
    v2.x = b.x; v2.y = b.y; v2.z = b.z;    

    v1.diffuse = color1;        
    v2.diffuse = color2;    
    
    g_bmLines3D.setNVert( nV + 2 );
    g_bmLines3D.setNPri( (nV>>1) + 1 );
} // rsLine

DIALOGS_API void rsRect(const float cx, const float cy, const float Side, DWORD Clr) {
	float x = cx - Side * 0.5f;
	float y = cy - Side * 0.5f;
	rsRect(Rct(ceilf(x), ceilf(y), Side, Side), 0, Clr);
}

DIALOGS_API void rsRect( const Rct& rct, float z, DWORD color )
{
    rsRect( rct, z, color, color, color, color );
}

DIALOGS_API void rsRect( const Rct& rct, const Rct& uv, float z, DWORD color )
{
    rsRect( rct, uv, z, color, color, color, color );
}

DIALOGS_API void rsRect( const Rct& rct, const Rct& uv, float z, DWORD ca, DWORD cb, DWORD cc, DWORD cd )
{
    if (g_bmPoly2D.getMaxVert() == 0)
    {
        g_bmPoly2D.create( c_PrimPoolSize, 0, vfVertexTnL, ptTriangleList );
        g_bmPoly2D.setIsQuadList( true );
        rsEnableZ();
    }

    int nV = g_bmPoly2D.getNVert();
    if (nV + 4 >= c_PrimPoolSize) 
    {
        rsFlushPoly2D();
        nV = 0;
    }

    VertexTnL* v = (VertexTnL*)g_bmPoly2D.getVertexData();
    VertexTnL& v1 = v[nV + 0];
    VertexTnL& v2 = v[nV + 1];
    VertexTnL& v3 = v[nV + 2];
    VertexTnL& v4 = v[nV + 3];

    v1.x = rct.x;                v1.y = rct.y;                v1.z = z;  v1.w = 1.0f;
    v2.x = rct.x + rct.w;        v2.y = rct.y;                v2.z = z;  v2.w = 1.0f;
    v3.x = rct.x;                v3.y = rct.y + rct.h;        v3.z = z;  v3.w = 1.0f;
    v4.x = v2.x;                v4.y = v3.y;                v4.z = z;  v4.w = 1.0f;

    v1.diffuse = ca;
    v2.diffuse = cb;
    v3.diffuse = cc;
    v4.diffuse = cd;

    v1.u = uv.x;
    v1.v = uv.y;
    v2.u = uv.GetRight();
    v2.v = uv.y;
    v3.u = uv.x;
    v3.v = uv.GetBottom();
    v4.u = uv.GetRight();
    v4.v = uv.GetBottom();

    g_bmPoly2D.setNVert( nV + 4 );
    g_bmPoly2D.setNPri( g_bmPoly2D.getNPri() + 2 );
} // rsRect

DIALOGS_API void rsRect( const Rct& rct, float z, DWORD ca, DWORD cb, DWORD cc, DWORD cd )
{
    if (g_bmPoly2D.getMaxVert() == 0)
    {
        g_bmPoly2D.create( c_PrimPoolSize, 0, vfVertexTnL, ptTriangleList );
        g_bmPoly2D.setIsQuadList( true );
        rsEnableZ();
    }

    int nV = g_bmPoly2D.getNVert();
    if (nV + 4 >= c_PrimPoolSize) 
    {
        rsFlushPoly2D();
        nV = 0;
    }

    VertexTnL* v = (VertexTnL*)g_bmPoly2D.getVertexData();
    VertexTnL& v1 = v[nV + 0];
    VertexTnL& v2 = v[nV + 1];
    VertexTnL& v3 = v[nV + 2];
    VertexTnL& v4 = v[nV + 3];

    v1.x = rct.x; v1.y = rct.y; v1.z = z;  v1.w = 1.0f;
    v2.x = rct.x + rct.w; v2.y = rct.y; v2.z = z; v2.w = 1.0f;
    v3.x = rct.x; v3.y = rct.y + rct.h; v3.z = z;  v3.w = 1.0f;
    v4.x = v2.x; v4.y = v3.y; v4.z = z; v4.w = 1.0f;

    v1.diffuse = ca;
    v2.diffuse = cb;
    v3.diffuse = cc;
    v4.diffuse = cd;

    g_bmPoly2D.setNVert( nV + 4 );
    g_bmPoly2D.setNPri( g_bmPoly2D.getNPri() + 2 );
} // rsRect

DIALOGS_API void rsColoredPanel(  const Rct& rct, float z, DWORD clr )
{
     ColorValue clrTop( 0xFFEEEEEE );
     ColorValue clrMdl( clr );
     ColorValue clrBot( 0xFF848284 );
     /*clrTop *= clrMdl;
     clrBot *= clrMdl;*/    
     clrTop.a *= clrMdl.a;
     clrBot.a *= clrMdl.a;
     rsPanel( rct, z, clrTop, clrMdl, clrBot );
}

void rsDoubleFrame( const Rct& rct, float z, DWORD clrTop, DWORD clrBot, bool bUp )
{
    if (bUp)
    {
        rsLine( rct.x + 1, rct.y, rct.r() - 2, rct.y, z, clrTop );
        rsLine( rct.x, rct.y, rct.x, rct.b() - 2, z, clrTop );
        rsLine( rct.x, rct.b() - 1, rct.r() - 1, rct.b() - 1, z, clrBot );
        rsLine( rct.r() - 1, rct.y, rct.r() - 1, rct.b() - 2, z, clrBot );
    }
    else
    {
        rsLine( rct.x + 1, rct.y, rct.r() - 1, rct.y, z, clrTop );
        rsLine( rct.x, rct.y, rct.x, rct.b() - 1, z, clrTop );
        rsLine( rct.x + 1, rct.b() - 1, rct.r() - 1, rct.b() - 1, z, clrBot );
        rsLine( rct.r() - 1, rct.y + 1, rct.r() - 1, rct.b() - 2, z, clrBot );
    }
} // rsDoubleFrame

DIALOGS_API void rsPanel( const Rct& rct, float z, DWORD clrTop, DWORD clrMdl, DWORD clrBot )
{
    Rct rc( rct );
    rc.Inflate( -1 );
    rsRect( rc, z, clrMdl );
    rc.Inflate( 1 );
    rsDoubleFrame( rc, z, clrTop, clrBot, true );
} // rsPanel

DIALOGS_API void rsThickPanel( const Rct& rct, float z, DWORD clr )
{
    ColorValue clrTop0( 0xFFD6D3CE );
    ColorValue clrTop( 0xFFFFFFFF );
    ColorValue clrMdl( clr );
    ColorValue clrBot( 0xFF848284 );
    ColorValue clrBot0( 0xFF424142 );

    clrTop0.a *= clrMdl.a;
    clrTop.a  *= clrMdl.a;
    clrBot.a  *= clrMdl.a;
    clrBot0.a *= clrMdl.a;

    Rct rc( rct );
    rc.Inflate( -2 );
    rsRect( rc, z, clrMdl );
    rc.Inflate( 1 );
    rsDoubleFrame( rc, z, clrTop, clrBot, true );
    rc.Inflate( 1 );
    rsDoubleFrame( rc, z, clrTop0, clrBot0, true );
} // rsThickPanel

DIALOGS_API void rsInnerPanel( const Rct& rct, float z, DWORD clr )
{
    ColorValue clrTop( 0xFF848284 );
    ColorValue clrMdl( clr );
    ColorValue clrIn ( 0xFF000000 );
    ColorValue clrBot( 0xFFFFFFFF );
    
    Rct rc( rct );
    rc.Inflate( -2 );
    rsRect( rc, z, clrMdl );
    rc.Inflate( 1 );
    rsFrame( rc, z, clrIn );
    rc.Inflate( 1 );
    rsDoubleFrame( rc, z, clrTop, clrBot, true );
} // rsInnerPanel

DIALOGS_API void rsPoly( const Vector3D& a, const Vector3D& b, const Vector3D& c, DWORD acol, DWORD bcol, DWORD ccol )
{
    if (g_bmPoly3D.getMaxVert() == 0)
    {
        g_bmPoly3D.create( c_PrimPoolSize, 0, vfVertex2t, ptTriangleList );
        rsEnableZ();
    }

    int nV = g_bmPoly3D.getNVert();
    if (nV + 3 >= c_PrimPoolSize) 
    {
        rsFlushPoly3D();
        nV = 0;
    }
    Vertex2t* v = (Vertex2t*)g_bmPoly3D.getVertexData();
    Vertex2t& v1 = v[nV];
    Vertex2t& v2 = v[nV + 1];
    Vertex2t& v3 = v[nV + 2];
    v1 = a; v2 = b; v3 = c;

    v1.diffuse = acol;
    v2.diffuse = bcol;
    v3.diffuse = ccol;

    g_bmPoly3D.setNVert( nV + 3 );
    g_bmPoly3D.setNPri( g_bmPoly3D.getNPri() + 1 );
} // rsPoly

DIALOGS_API void rsPoly2D( const Vector3D& a, const Vector3D& b, const Vector3D& c, 
               DWORD acol, DWORD bcol, DWORD ccol )
{
    if (g_bmPoly2D.getMaxVert() == 0)
    {
        g_bmPoly2D.create( c_PrimPoolSize, 0, vfVertexTnL, ptTriangleList );
        rsEnableZ();
    }

    int nV = g_bmPoly2D.getNVert();
    if (nV + 4 >= c_PrimPoolSize) 
    {
        rsFlushPoly2D();
        nV = 0;
    }
    VertexTnL* v = (VertexTnL*)g_bmPoly2D.getVertexData();
    VertexTnL& v1 = v[nV];
    VertexTnL& v2 = v[nV + 1];
    VertexTnL& v3 = v[nV + 2];
    VertexTnL& v4 = v[nV + 3];
    v1 = a; v2 = b; v3 = c; 

    v1.diffuse = acol;
    v2.diffuse = bcol;
    v3.diffuse = ccol;

    v1.w = 1.0f;
    v2.w = 1.0f;
    v3.w = 1.0f;

    v4 = v3;

    g_bmPoly2D.setNVert( nV + 4 );
    g_bmPoly2D.setNPri( g_bmPoly2D.getNPri() + 2 );
} // rsPoly

DIALOGS_API void rsPoly( const Vector3D& a, const Vector3D& b, const Vector3D& c, 
            float au, float av, float bu, float bv, float cu, float cv,
            float au2, float av2, float bu2, float bv2, float cu2, float cv2,
            DWORD acol, DWORD bcol, DWORD ccol )
{
    if (g_bmPoly3D.getMaxVert() == 0)
    {
        g_bmPoly3D.create( c_PrimPoolSize, 0, vfVertex2t, ptTriangleList );
        rsEnableZ();
    }

    int nV = g_bmPoly3D.getNVert();
    if (nV + 3 >= c_PrimPoolSize) 
    {
        rsFlushPoly3D();
        nV = 0;
    }
    Vertex2t* v = (Vertex2t*)g_bmPoly3D.getVertexData();
    Vertex2t& v1 = v[nV];
    Vertex2t& v2 = v[nV + 1];
    Vertex2t& v3 = v[nV + 2];
    v1 = a; v2 = b; v3 = c;

    v1.u = au;
    v1.v = av;

    v2.u = bu;
    v2.v = bv;

    v3.u = cu;
    v3.v = cv;

    v1.u2 = au2;
    v1.v2 = av2;

    v2.u2 = bu2;
    v2.v2 = bv2;

    v3.u2 = cu2;
    v3.v2 = cv2;

    v1.diffuse = acol;
    v2.diffuse = bcol;
    v3.diffuse = ccol;

    g_bmPoly3D.setNVert( nV + 3 );
    g_bmPoly3D.setNPri( g_bmPoly3D.getNPri() + 1 );
} // rsPoly

DIALOGS_API void rsPoly( const Vector3D& a, const Vector3D& b, const Vector3D& c, 
             float au, float av, float bu, float bv, float cu, float cv,
             DWORD acol, DWORD bcol, DWORD ccol )
{
    if (g_bmPoly3D.getMaxVert() == 0)
    {
        g_bmPoly3D.create( c_PrimPoolSize, 0, vfVertex2t, ptTriangleList );
        rsEnableZ();
    }

    int nV = g_bmPoly3D.getNVert();
    if (nV + 3 >= c_PrimPoolSize) 
    {
        rsFlushPoly3D();
        nV = 0;
    }
    Vertex2t* v = (Vertex2t*)g_bmPoly3D.getVertexData();
    Vertex2t& v1 = v[nV];
    Vertex2t& v2 = v[nV + 1];
    Vertex2t& v3 = v[nV + 2];
    v1 = a; v2 = b; v3 = c;
    
    v1.u = au;
    v1.v = av;

    v2.u = bu;
    v2.v = bv;
    
    v3.u = cu;
    v3.v = cv;

    v1.diffuse = acol;
    v2.diffuse = bcol;
    v3.diffuse = ccol;

    g_bmPoly3D.setNVert( nV + 3 );
    g_bmPoly3D.setNPri( g_bmPoly3D.getNPri() + 1 );
} // rsPoly


DIALOGS_API void rsPoly( const Vector3D& a, const Vector3D& b, const Vector3D& c, DWORD color )
{
    rsPoly( a, b, c, color, color, color );
} // rsPoly

DIALOGS_API void rsQuad( const Vector3D& a, const Vector3D& b, const Vector3D& c, const Vector3D& d, 
             DWORD acol, DWORD bcol, DWORD ccol, DWORD dcol )
{
    rsPoly( a, b, c, acol, bcol, ccol );
    rsPoly( c, b, d, ccol, bcol, dcol );

} // rsQuad

DIALOGS_API void rsQuad( const Vector3D& a, const Vector3D& b, const Vector3D& c, const Vector3D& d, 
                    const Rct& uv, DWORD acol, DWORD bcol, DWORD ccol, DWORD dcol )
{
    rsPoly( a, b, c, 
            uv.x, uv.y, 
            uv.GetRight(), uv.y, 
            uv.x, uv.GetBottom(), 
            acol, bcol, ccol );
    rsPoly( c, b, d, 
            uv.x, uv.GetBottom(), 
            uv.GetRight(), uv.y, 
            uv.GetRight(), uv.GetBottom(), 
            ccol, bcol, dcol );
} // rsQuad

DIALOGS_API void rsQuad( const Vector3D& a, const Vector3D& b, const Vector3D& c, const Vector3D& d, 
            const Rct& uv, const Rct& uv2, DWORD col )
{
    rsPoly( a, b, c, 
        uv.x, uv.y, 
        uv.GetRight(), uv.y, 
        uv.x, uv.GetBottom(), 
        uv2.x, uv2.y, 
        uv2.GetRight(), uv2.y, 
        uv2.x, uv2.GetBottom(),
        col, col, col );

    rsPoly( c, b, d, 
        uv.x, uv.GetBottom(), 
        uv.GetRight(), uv.y, 
        uv.GetRight(), uv.GetBottom(), 
        uv2.x, uv2.GetBottom(), 
        uv2.GetRight(), uv2.y, 
        uv2.GetRight(), uv2.GetBottom(), 
        col, col, col );
} // rsQuad

DIALOGS_API void rsQuad( const Vector3D& a, const Vector3D& b, const Vector3D& c, const Vector3D& d, const Rct& uv, DWORD col )
{
    rsQuad( a, b, c, d, uv, col, col, col, col );
}


DIALOGS_API void rsQuad( const Vector3D& a, const Vector3D& b, const Vector3D& c, const Vector3D& d, DWORD color )
{
    rsQuad( a, b, c, d, color, color, color, color );
} // rsQuad

DIALOGS_API void rsFlushPoly3D( bool bShaded )
{
    if (g_bmPoly3D.getNVert() == 0) return;
    if (bShaded) DrawBM( g_bmPoly3D ); else DrawPrimBM( g_bmPoly3D );
    g_bmPoly3D.setNVert( 0 );
    g_bmPoly3D.setNPri( 0 );
} // rsFlushPoly3D

DIALOGS_API void rsFlushLines3D( bool bShaded )
{
    if (g_bmLines3D.getNVert() == 0) return;

    if (bShaded) DrawBM( g_bmLines3D ); else DrawPrimBM( g_bmLines3D );
    g_bmLines3D.setNVert( 0 );
    g_bmLines3D.setNPri( 0 );
}

DIALOGS_API void rsFlushLines2D( bool bShaded )
{
    if (g_bmLines2D.getNVert() == 0) return;

    if (bShaded) DrawBM( g_bmLines2D ); else DrawPrimBM( g_bmLines2D );
    g_bmLines2D.setNVert( 0 );
    g_bmLines2D.setNPri( 0 );
}

DIALOGS_API void rsFlushPoly2D( bool bShaded )
{
    if (g_bmPoly2D.getNVert() == 0) return;
    if (bShaded) DrawBM( g_bmPoly2D ); else DrawPrimBM( g_bmPoly2D );
    g_bmPoly2D.setNVert( 0 );
    g_bmPoly2D.setNPri( 0 );
}

DIALOGS_API void rsFlush( bool bShaded )
{
    rsFlushLines2D( bShaded );
    rsFlushLines3D( bShaded );
    rsFlushPoly2D ( bShaded );
    rsFlushPoly3D ( bShaded );
} // rsFlush

DIALOGS_API void DrawRay( const Ray3D& ray, DWORD linesColor, float rayLen, float handleSide )
{
    Vector3D dest( ray.getDir() );
    dest *= rayLen;
    dest += ray.getOrig();
    rsLine( ray.getOrig(), dest, linesColor );
    AABoundBox aabb( ray.getOrig(), handleSide );
    DrawAABB( aabb, 0, linesColor );
} // DrawRay

DIALOGS_API void DrawPlane( const Plane& plane, DWORD fillColor, DWORD color, const Vector3D* center, float qSide, float nLen )
{
    Vector3D c = center ? *center : plane.GetPoint();
    Vector3D n = plane.normal();
    Vector3D vx, vy;
    n.CreateBasis( vx, vy );

    Vector3D lt, lb, rt, rb;
    DrawRay( Ray3D( c, plane.normal() ), color, nLen, 1.0f );
    lt.addWeighted( vx, vy, -qSide, -qSide ); lt += c;
    rt.addWeighted( vx, vy,  qSide, -qSide ); rt += c;
    lb.addWeighted( vx, vy, -qSide,  qSide ); lb += c;
    rb.addWeighted( vx, vy,  qSide,  qSide ); rb += c;

    rsQuad( lt, rt, lb, rb, fillColor );

    rsLine( lt, rt, color, color );
    rsLine( rt, rb, color, color );
    rsLine( rb, lb, color, color );
    rsLine( lb, lt, color, color );

} // DrawPlane

DIALOGS_API void DrawPoint( const Vector3D& pt, DWORD clr, float rad )
{

    DrawAnchor( pt, rad, clr, clr, clr, true );
}

DIALOGS_API void DrawTriangle( const Triangle& tri, DWORD linesColor, DWORD fillColor )
{
    if (fillColor) rsPoly( tri.a, tri.b, tri.c, fillColor );
    if (linesColor) 
    {
        rsLine( tri.a, tri.b, linesColor );
        rsLine( tri.b, tri.c, linesColor );
        rsLine( tri.a, tri.c, linesColor );
    }
} // DrawTriangle

DIALOGS_API void DrawFrustum( const Frustum& frustum, DWORD fillColor, DWORD linesColor, bool drawNormals )
{
    Vector3D ltn = frustum.ltn();
    Vector3D rtn = frustum.rtn();
    Vector3D lbn = frustum.lbn();
    Vector3D rbn = frustum.rbn();
    Vector3D ltf = frustum.ltf();
    Vector3D rtf = frustum.rtf();
    Vector3D lbf = frustum.lbf();
    Vector3D rbf = frustum.rbf();

    if (linesColor != 0)
    {
        rsLine( ltn, rtn, linesColor, linesColor );
        rsLine( lbn, rbn, linesColor, linesColor );
        rsLine( ltn, lbn, linesColor, linesColor );
        rsLine( rtn, rbn, linesColor, linesColor );

        rsLine( ltf, rtf, linesColor, linesColor );
        rsLine( lbf, rbf, linesColor, linesColor );
        rsLine( ltf, lbf, linesColor, linesColor );
        rsLine( rtf, rbf, linesColor, linesColor );

        rsLine( ltn, ltf, linesColor, linesColor );
        rsLine( lbn, lbf, linesColor, linesColor );
        rsLine( rtn, rtf, linesColor, linesColor );
        rsLine( rbn, rbf, linesColor, linesColor );
    }

    if (fillColor != 0)
    {
        rsQuad( ltn, rtn, lbn, rbn, fillColor );
        rsQuad( rtf, ltf, rbf, lbf, fillColor );
        rsQuad( ltf, rtf, ltn, rtn, fillColor );
        rsQuad( rbf, lbf, rbn, lbn, fillColor );
        rsQuad( ltf, ltn, lbf, lbn, fillColor );
        rsQuad( rtn, rtf, rbn, rbf, fillColor );
    }

    if (drawNormals)
    {
        Vector3D vn( lbn );
        vn += rbn; vn += rtn; vn += ltn; vn *= 0.25f;

        Vector3D vf( lbf );
        vf += rbf; vf += rtf; vf += ltf; vf *= 0.25f;

        Vector3D vt( ltn );
        vt += rtn; vt += rtf; vt += ltf; vt *= 0.25f;

        Vector3D vb( lbn );
        vb += rbn; vb += rbf; vb += lbf; vb *= 0.25f;

        Vector3D vl( lbf );
        vl += lbn; vl += ltn; vl += ltf; vl *= 0.25f;

        Vector3D vr( rbn );
        vr += rbf; vr += rtn; vr += rtf; vr *= 0.25f;

        Vector3D ndir( frustum.plNear.normal()      );
        Vector3D fdir( frustum.plFar.normal()       );
        Vector3D ldir( frustum.plLeft.normal()      );
        Vector3D rdir( frustum.plRight.normal()     );
        Vector3D tdir( frustum.plTop.normal()       );
        Vector3D bdir( frustum.plBottom.normal()    );

        ndir.normalize();
        fdir.normalize();
        ldir.normalize();
        rdir.normalize();
        tdir.normalize();
        bdir.normalize();

        static const float c_NormalLen = 100.0f;

        ndir *= c_NormalLen;
        fdir *= c_NormalLen;
        ldir *= c_NormalLen;
        rdir *= c_NormalLen;
        tdir *= c_NormalLen;
        bdir *= c_NormalLen;

        ndir += vn;
        fdir += vf;
        ldir += vl;
        rdir += vr;
        tdir += vt;
        bdir += vb;

        rsLine( vn,    ndir, linesColor, linesColor );
        rsLine( vf,    fdir, linesColor, linesColor );
        rsLine( vl,    ldir, linesColor, linesColor );
        rsLine( vr,    rdir, linesColor, linesColor );
        rsLine( vt,    tdir, linesColor, linesColor );
        rsLine( vb,    bdir, linesColor, linesColor );
    }

}// DrawFrustum

DIALOGS_API void DrawCube( const Vector3D& center, DWORD color, float side )
{
    AABoundBox aabb( center, side * 0.5f );
    DrawAABB( aabb, 0, color );
} // DrawCube

DIALOGS_API void DrawAABB( const AABoundBox& aabb, DWORD fillColor, DWORD linesColor )
{
    Vector3D ltn = Vector3D( aabb.minv.x, aabb.minv.y, aabb.maxv.z );
    Vector3D rtn = Vector3D( aabb.minv.x, aabb.maxv.y, aabb.maxv.z );
    Vector3D lbn = Vector3D( aabb.minv.x, aabb.minv.y, aabb.minv.z );
    Vector3D rbn = Vector3D( aabb.minv.x, aabb.maxv.y, aabb.minv.z );
    Vector3D ltf = Vector3D( aabb.maxv.x, aabb.minv.y, aabb.maxv.z );
    Vector3D rtf = Vector3D( aabb.maxv.x, aabb.maxv.y, aabb.maxv.z );
    Vector3D lbf = Vector3D( aabb.maxv.x, aabb.minv.y, aabb.minv.z );
    Vector3D rbf = Vector3D( aabb.maxv.x, aabb.maxv.y, aabb.minv.z );

    if (linesColor != 0)
    {
        rsLine( ltn, rtn, linesColor, linesColor );
        rsLine( lbn, rbn, linesColor, linesColor );
        rsLine( ltn, lbn, linesColor, linesColor );
        rsLine( rtn, rbn, linesColor, linesColor );

        rsLine( ltf, rtf, linesColor, linesColor );
        rsLine( lbf, rbf, linesColor, linesColor );
        rsLine( ltf, lbf, linesColor, linesColor );
        rsLine( rtf, rbf, linesColor, linesColor );

        rsLine( ltn, ltf, linesColor, linesColor );
        rsLine( lbn, lbf, linesColor, linesColor );
        rsLine( rtn, rtf, linesColor, linesColor );
        rsLine( rbn, rbf, linesColor, linesColor );
    }

    if (fillColor != 0)
    {
        rsQuad( ltn, rtn, lbn, rbn, fillColor );
        rsQuad( rtf, ltf, rbf, lbf, fillColor );
        rsQuad( ltf, rtf, ltn, rtn, fillColor );
        rsQuad( rbf, lbf, rbn, lbn, fillColor );
        rsQuad( ltf, ltn, lbf, lbn, fillColor );
        rsQuad( rtn, rtf, rbn, rbf, fillColor );
    }
} // DrawAABB

const float c_Circle8C[9] = 
{
    1.0f, 0.70710677f, 0.0f, -0.70710677f, -1.0f, -0.70710677f,    0.0f, 0.70710677f, 1.0f

};

const float c_Circle8S[9] = 
{
    0.0f, 0.70710677f, 1.0f, 0.70710677f, 0.0f, -0.70710677f, -1.0f, -0.70710677f, 0.0f
};

DIALOGS_API void DrawCircle8( const Vector3D& center, const Vector3D& normal, float radius, 
                 DWORD fillColor, DWORD linesColor )
{
    Matrix4D cTM;
    Vector3D z( normal );
    Vector3D x, y;
    z.CreateBasis( x, y );
    cTM.fromBasis( x, y, z, center );
    Vector3D a, b;

    for (int i = 0; i < 8; i++)
    {
        a.set( radius * c_Circle8C[i],        radius * c_Circle8S[i],     0.0f );
        b.set( radius * c_Circle8C[i + 1],    radius * c_Circle8S[i + 1], 0.0f );

        a *= cTM;
        b *= cTM;

        if (linesColor != 0)
        {
            rsLine( a, b, linesColor, linesColor );
        }

        if (fillColor != 0)
        {
            rsPoly( a, b, center, fillColor );
        }    
    }

} // DrawCircle8

DIALOGS_API void DrawCircle( const Vector3D& center, const Vector3D& normal, float radius, DWORD fillColor, DWORD linesColor, int nSegments )
{
    Matrix4D cTM;
    Vector3D z( normal );
    Vector3D x, y;
    z.CreateBasis( x, y );
    cTM.fromBasis( x, y, z, center );

    float phiStep = c_DoublePI / float( nSegments );
    Vector3D a, b;

    for (float phi = 0.0f; phi < c_DoublePI; phi += phiStep)
    {
        a.set( radius * cos( phi ), radius * sin( phi ), 0.0f );
        b.set( radius * cos( phi + phiStep), radius * sin( phi + phiStep ), 0.0f );

        a *= cTM;
        b *= cTM;

        if (linesColor != 0)
        {
            rsLine( a, b, linesColor, linesColor );
        }

        if (fillColor != 0)
        {
            rsPoly( a, b, center, fillColor );
        }    
    }

} // DrawCircle

DIALOGS_API void DrawCircle( float x, float y, float radius, DWORD fillColor, DWORD linesColor, int nSegments )
{
    float phiStep = c_DoublePI/float( nSegments );
    Vector3D a, b;

    for (float phi = 0.0f; phi < c_DoublePI; phi += phiStep)
    {
        a.set( x + radius*cos( phi ), y + radius*sin( phi ), 0.0f );
        b.set( x + radius*cos( phi + phiStep), y + radius*sin( phi + phiStep ), 0.0f );

        if (linesColor != 0)
        {
            rsLine( a.x, a.y, b.x, b.y, 0.0f, linesColor, linesColor );
        }

        if (fillColor != 0)
        {
            rsPoly2D(   Vector3D( a.x, a.y, 0.0f ), 
                        Vector3D( b.x, b.y, 0.0f ),  
                        Vector3D( x, y, 0.0f ), 
                        fillColor, fillColor, fillColor );
        }    
    }
} // DrawCircle

DIALOGS_API void DrawSphere( const Sphere& sphere, DWORD fillColor, DWORD linesColor, int nSegments )
{
    float phiStep = c_DoublePI / float( nSegments );
    float thetaStep = phiStep;

    Vector3D a, b, c, d;

    for (float phi = 0.0f; phi <= c_PI; phi += phiStep)
    {
        for (float theta = 0.0f; theta <= c_DoublePI; theta += thetaStep)
        {
            a.FromSpherical( sphere.GetRadius(), theta, phi );
            a += sphere.GetCenter();
    
            b.FromSpherical( sphere.GetRadius(), theta + thetaStep, phi );
            b += sphere.GetCenter();

            c.FromSpherical( sphere.GetRadius(), theta, phi + phiStep );
            c += sphere.GetCenter();

            d.FromSpherical( sphere.GetRadius(), theta + thetaStep, phi + phiStep );
            d += sphere.GetCenter();

            if (linesColor != 0)
            {
                rsLine( b, d, linesColor, linesColor );
                rsLine( c, d, linesColor, linesColor );
            }

            if (fillColor != 0)
            {
                if (theta < c_PI) rsQuad( b, a, d, c, fillColor );
                else rsQuad( a, b, c, d, fillColor );
            }
        }
    }
} // DrawSphere

DIALOGS_API void DrawAnchor( const Vector3D& pos, float side, DWORD color, bool bBothSides )
{
    DrawAnchor( pos, side, color, color, color, bBothSides );
}

DIALOGS_API void DrawAnchor( const Vector3D& pos, float side, DWORD cx, DWORD cy, DWORD cz, bool bBothSides )
{
    Vector3D bX = pos;
    Vector3D bY = pos;
    Vector3D bZ = pos;
    Vector3D eX = pos;
    Vector3D eY = pos;
    Vector3D eZ = pos;

    eX.x += side;
    eY.y += side;
    eZ.z += side;

    if (bBothSides)
    {
        bX.x -= side; 
        bY.y -= side; 
        bZ.z -= side; 
    }
    rsLine( bX, eX, cx, cx );
    rsLine( bY, eY, cy, cy );
    rsLine( bZ, eZ, cz, cz );
} // void DrawAnchor

DIALOGS_API void DrawSpherePatch( const Sphere& sphere, 
                     DWORD fillColor, DWORD linesColor, 
                     float phiBeg, float phiEnd,
                     float thetaBeg, float thetaEnd,
                     int nSegments, const Matrix4D* pTM )
{
    float phiStep = 2.0f * c_PI / float( nSegments );
    float thetaStep = phiStep;

    Vector3D a, b, c, d;

    for (float phi = phiBeg; phi < phiEnd; phi += phiStep)
    {
        for (float theta = thetaBeg; theta < thetaEnd; theta += thetaStep)
        {
            a.FromSpherical( sphere.GetRadius(), theta, phi );
            a += sphere.GetCenter();

            b.FromSpherical( sphere.GetRadius(), theta + thetaStep, phi );
            b += sphere.GetCenter();

            c.FromSpherical( sphere.GetRadius(), theta, phi + phiStep );
            c += sphere.GetCenter();

            d.FromSpherical( sphere.GetRadius(), theta + thetaStep, phi + phiStep );
            d += sphere.GetCenter();

            if (pTM)
            {
                a *= *pTM;
                b *= *pTM;
                c *= *pTM;
                d *= *pTM;
            }

            if (linesColor != 0)
            {
                rsLine( b, d, linesColor, linesColor );
                rsLine( c, d, linesColor, linesColor );
            }

            if (fillColor != 0)
            {
                if (theta < c_PI) rsQuad( b, a, d, c, fillColor );
                else rsQuad( a, b, c, d, fillColor );
            }
        }
    }
} // DrawSpherePatch

DIALOGS_API void DrawStar( const Sphere& sphere, DWORD begColor, DWORD endColor, int nSegments )
{
    float phiStep = 2.0f * c_PI / float( nSegments );
    float thetaStep = phiStep;

    for (float phi = phiStep; phi < c_PI; phi += phiStep)
    {
        for (float theta = 0.0f; theta < c_DoublePI; theta += thetaStep)
        {
            Vector3D pt;
            pt.FromSpherical( sphere.GetRadius(), theta, phi );
            pt += sphere.GetCenter();
            rsLine( sphere.GetCenter(), pt, begColor, endColor );
        }
    }
} // DrawStar

DIALOGS_API void DrawCylinder( const Cylinder& cylinder, 
                  DWORD fillColor, DWORD linesColor, bool bCapped,
                  int nSegments )
{
    float phiStep = c_DoublePI / float( nSegments );
    Vector3D a, b, c, d;

    float H = cylinder.GetHeight();
    float r = cylinder.GetRadius();
    Matrix4D cylTM( cylinder.GetWorldTM() );

    Vector3D top = cylinder.GetTop();
    Vector3D bottom = cylinder.GetBase();

    for (float phi = 0.0f; phi < c_DoublePI; phi += phiStep)
    {
        a.set( r * cos( phi ), r * sin( phi ), 0.0f );
        b.set( a.x, a.y, H );
        c.set( r * cos( phi + phiStep), r * sin( phi + phiStep ), 0.0f );
        d.set( c.x, c.y, H );

        a *= cylTM;
        b *= cylTM;
        c *= cylTM;
        d *= cylTM;

        if (linesColor != 0)
        {
            rsLine( a, b, linesColor, linesColor );
            rsLine( a, c, linesColor, linesColor );
            rsLine( b, d, linesColor, linesColor );
        }

        if (fillColor != 0)
        {
            rsQuad( a, c, b, d, fillColor );
            if (bCapped)
            {
                rsPoly( c, a, bottom, fillColor );
                rsPoly( b, d, top, fillColor );
            }
        }    
    }

} // DrawCylinder

DIALOGS_API void DrawCone( const Cone& cone, DWORD fillColor, DWORD linesColor, int nSegments )
{
    float phiStep = c_DoublePI / float( nSegments );
    Vector3D a, b;

    Matrix4D coneTM( cone.GetWorldTM() );

    Vector3D top = cone.GetTop();
    float r = cone.GetBaseRadius();

    for (float phi = 0.0f; phi < c_DoublePI; phi += phiStep)
    {
        a.set( r * cos( phi ), r * sin( phi ), 0.0f );
        b.set( r * cos( phi + phiStep), r * sin( phi + phiStep ), 0.0f );

        a *= coneTM;
        b *= coneTM;

        if (linesColor != 0)
        {
            rsLine( a, b,   linesColor, linesColor );
            rsLine( b, top, linesColor, linesColor );
            rsLine( top, b, linesColor, linesColor );
        }

        if (fillColor != 0)
        {
            rsPoly( a, b, top, fillColor );
        }    
    }
} // DrawCone

DIALOGS_API void DrawCapsule( const Capsule& capsule, 
                 DWORD fillColor, DWORD linesColor, 
                 int nSegments )
{
    Cylinder cylinder( capsule );
    DrawCylinder( cylinder, fillColor, linesColor, false, nSegments );
    Sphere sphere( Vector3D( 0.0f, 0.0f, 0.0f ), capsule.GetRadius() );

    Matrix4D tm = capsule.GetWorldTM();

    tm.setTranslation( capsule.GetTop() );
    DrawSpherePatch( sphere, fillColor, linesColor, 
        0.0f, c_DoublePI, 0.0f, c_HalfPI, 
        nSegments, &tm );

    tm.setTranslation( capsule.GetBase() );
    DrawSpherePatch( sphere, fillColor, linesColor, 
        0.0f, c_DoublePI, c_HalfPI, c_PI, 
        nSegments, &tm );
} // DrawCapsule

DIALOGS_API void DrawFatSegment( const Vector3D& beg, const Vector3D& end, const Vector3D& normal, 
                        float width, bool bRoundEnds, DWORD color, DWORD coreColor )
{
    rsLine( beg, end, coreColor );
    Vector3D side, dir;
    dir.sub( end, beg );
    dir.normalize();
    side.cross( normal, dir );
    side.normalize();
    
    Vector3D a( beg ), b( beg ), c( end ), d( end );
    float hw = width * 0.5f;
    a.addWeighted( side,  hw );
    b.addWeighted( side, -hw );
    c.addWeighted( side,  hw );
    d.addWeighted( side, -hw );
    
    rsQuad( a, b, c, d, color );

    if (bRoundEnds)
    {
    
    }
} // DrawFatSegment

DIALOGS_API void rsQuad2D( const Vector3D& a, const Vector3D& b, const Vector3D& c, const Vector3D& d, 
               DWORD acol, DWORD bcol, DWORD ccol, DWORD dcol, const Rct& uv )
{
    if (g_bmPoly2D.getMaxVert() == 0)
    {
        g_bmPoly2D.create( c_PrimPoolSize, 0, vfVertexTnL, ptTriangleList );
        g_bmPoly2D.setIsQuadList( true );
        rsEnableZ();
    }

    VertexIterator vit;
    vit << g_bmPoly2D;
    int nV = g_bmPoly2D.getNVert();
    if (nV + 4 >= c_PrimPoolSize) 
    {
        rsFlushPoly2D();
        nV = 0;
    }

    VertexTnL* v = (VertexTnL*)g_bmPoly2D.getVertexData();
    VertexTnL& v1 = v[nV + 0];
    VertexTnL& v2 = v[nV + 1];
    VertexTnL& v3 = v[nV + 2];
    VertexTnL& v4 = v[nV + 3];

    v1 = a;  v1.w = 1.0f;
    v2 = b;  v2.w = 1.0f;
    v3 = c;  v3.w = 1.0f;
    v4 = d;  v4.w = 1.0f;

    v1.diffuse = acol;
    v2.diffuse = bcol;
    v3.diffuse = ccol;
    v4.diffuse = dcol;

    v1.u = uv.x;
    v1.v = uv.y;
    v2.u = uv.GetRight();
    v2.v = uv.y;
    v3.u = uv.x;
    v3.v = uv.GetBottom();
    v4.u = uv.GetRight();
    v4.v = uv.GetBottom();

    g_bmPoly2D.setNVert( nV + 4 );
    g_bmPoly2D.setNPri( g_bmPoly2D.getNPri() + 2 );
} // rsQuad2D

const float c_MinBendAngle = DegToRad( 10.0f );
void DrawCurve( float width, int nPoints, const Vector3D* points, 
                const DWORD* clrTop, const DWORD* clrBottom )
{
    float hw        = width*0.5f;
    float rrad      = hw;
    float begShift  = 0.0f;
    float cu        = 0.0f;           
    Vector3D up( Vector3D::oZ );

    for (int i = 1; i < nPoints; i++)
    {
        const Vector3D& p1 = points[i - 1];
        const Vector3D& p2 = points[i];
        const Vector3D& pn = (i == nPoints - 1) ? points[i] : points[i + 1];

        DWORD c1T = clrTop[i - 1];
        DWORD c1B = clrBottom[i - 1];
        DWORD c2T = clrTop[i];
        DWORD c2B = clrBottom[i];
        
        DWORD cnT = (i == nPoints - 1) ? clrTop[i] : clrTop[i + 1];
        DWORD cnB = (i == nPoints - 1) ? clrBottom[i] : clrBottom[i + 1];

        Vector3D a( p2 ); a -= p1;
        Vector3D b( pn ); b -= p2;

        float da    = a.normalize();
        float db    = b.normalize();
        float dot   = a.dot( b );
        clamp( dot, -1.0f, 1.0f );
        float phi   = acosf( dot );
        float theta = (c_PI - phi)*0.5f;

        Vector3D right( -a.y, a.x, 0.0f );

        Vector3D pa( p1 ), pb( p1 );
        pa.addWeighted( right, hw );
        pb.addWeighted( right, -hw );

        pa.addWeighted( a, begShift );
        pb.addWeighted( a, begShift );

        float segLen = da - begShift;
        begShift = rrad/tanf( theta );
        segLen -= begShift;

        Vector3D pc( pa ), pd( pb );
        pc.addWeighted( a, segLen );
        pd.addWeighted( a, segLen );

        float du = segLen/width;
        rsQuad2D( pa, pc, pb, pd, c1T, c2T, c1B, c2B, Rct( cu, 0.0f, du, 1.0f ) );
        cu += du;

        if (i < nPoints - 1)
        {
            //  center of the rounding circle
            Vector3D cc( b ); cc -= a;
            cc.normalize();
            cc *= rrad/sinf( theta );
            cc += p2;

            float   alpha     = phi;

            Vector3D norm;
            norm.cross( a, b );
            if (norm.dot( up ) < 0.0f) alpha = -alpha;

            int     nSteps    = fabs( alpha/c_MinBendAngle );
            float   dAlpha    = alpha/float( nSteps );
            float   cA        = cosf( dAlpha );
            float   sA        = sinf( dAlpha );
            float   dx        = -cA*cc.x + sA*cc.y + cc.x;
            float   dy        = -sA*cc.x - cA*cc.y + cc.y;

            float grad  = 0.0f;
            float dgrad = grad / float(nSteps);

            for (int j = 0; j < nSteps; j++)
            {
                pa = pc; pb = pd;

                float pcx = pc.x*cA - pc.y*sA + dx;
                float pcy = pc.x*sA + pc.y*cA + dy;
                pc.x = pcx; pc.y = pcy;

                float pdx = pd.x*cA - pd.y*sA + dx;
                float pdy = pd.x*sA + pd.y*cA + dy;
                pd.x = pdx; pd.y = pdy;

                DWORD caT = ColorValue::Gradient( c2T, cnT, grad );
                DWORD caB = ColorValue::Gradient( c2B, cnB, grad );
                grad += dgrad;
                DWORD cbT = ColorValue::Gradient( c2T, cnT, grad );
                DWORD cbB = ColorValue::Gradient( c2B, cnB, grad );

                float du = dAlpha*rrad/width;
                rsQuad2D( pa, pc, pb, pd, caT, cbT, caB, cbB, Rct( cu, 0.0f, du, 1.0f ) );
                cu += du;
            }
        }
    }
    rsFlushLines2D();
} // DrawCurve

void DrawCurve( float width, int nPoints, const Vector3D* points, const DWORD* colors )
{
    DrawCurve( width, nPoints, points, colors, colors );
} // DrawCurve

DIALOGS_API void DrawArrow( const Vector3D& start, const Vector3D& dir, DWORD color, float len, float head, float headR )
{
    Vector3D wdir( dir ); wdir *= len;
    Vector3D end( start ); end += wdir;
    Vector3D headStart( start );
    headStart.addWeighted( wdir, 1.0f - head );
    rsLine( start, end, color, color );
    DrawCone( Cone( end, headStart, atan2f( headR, head ) ), color, 0, 10 );
} // DrawArrow

void DrawFaces( BaseMesh& bm, bool bDrawNumbers )
{
    ICamera* cam = GetCamera();
    if (!cam) return;
    int nF = bm.getNPri();
    VertexIterator vit;
    vit << bm;
    WORD* idx = bm.getIndices();
    for (int i = 0; i < nF; i++)
    {
        const Vector3D& v0 = vit.pos( idx[i*3    ] );
        const Vector3D& v1 = vit.pos( idx[i*3 + 1] );
        const Vector3D& v2 = vit.pos( idx[i*3 + 2] );
        Vector3D c; c.centroid( v0, v1, v2 );
        Vector4D cx( c );
        cam->ToSpace( sWorld, sScreen, cx );
        rsRect( Rct( cx.x, cx.y, 2.0f ), 0.0f, 0xFFFFFF00 );
        if (bDrawNumbers)
        {
            DrawText( cx.x + 3, cx.y + 3, 0xFFFFFFFF, "%d", i );
        }
    }
} // DrawFaces

void DrawVertices( BaseMesh& bm, bool bDrawNumbers, bool bDrawHandles )
{
    ICamera* cam = GetCamera();
    if (!cam) return;
    int nV = bm.getNVert();
    VertexIterator vit;
    vit << bm;
    for (int i = 0; i < nV; i++)
    {
        Vector4D v = vit.pos( i );
        cam->ToSpace( sWorld, sScreen,  v );
        if (bDrawHandles) rsRect( Rct( v.x, v.y, 2.0f ), 0.0f, 0xFFCC33CC );
        if (bDrawNumbers) DrawText( v.x + 3, v.y + 3, 0xFFFF99FF, "%d", i );
    }
} // DrawVertices

void DrawGrid( float side, int nCells, DWORD clr, DWORD subClr )
{
    ICamera* cam = GetCamera();
    if (!cam) return;

    cam->ShiftZ( 0.1f );

    rsEnableZ();
    Rct grid( -side*0.5f, -side*0.5f, side, side );

    float dx = grid.w / nCells;
    float x = grid.x;
    for (int i = 0; i <= nCells; i++)
    {
        rsLine( Vector3D( x, grid.y, 0.0f ), Vector3D( x, grid.GetBottom(), 0.0f ), subClr );
        x += dx;
    }

    float dy = grid.h / nCells;
    float y = grid.y;
    for (int i = 0; i <= nCells; i++)
    {
        rsLine( Vector3D( grid.x, y, 0.0f ), 
            Vector3D( grid.GetRight(), y, 0.0f ), subClr );
        y += dy;
    }

    rsLine( Vector3D( 0.0f, grid.y, 0.0f ), Vector3D( 0.0f, grid.GetBottom(), 0.0f ), clr );
    rsLine( Vector3D( grid.x, 0.0f, 0.0f ), Vector3D( grid.GetRight(), 0.0f, 0.0f ), clr );
    rsFlush();

    cam->ShiftZ( -0.1f );

} // DrawGrid


void DrawPercentGauge( float percent, const Rct& ext, DWORD clr )
{
    clamp( percent, 0.0f, 100.0f );
    percent *= 8.0f/100.0f;
    float cx = ext.GetCenterX();
    float cy = ext.GetCenterY();
    Vector3D c( cx, cy, 0.0f );
    Vector3D lt( ext.x, ext.y, 0.0f );
    float c_px[] = { 0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.5f };
    float c_py[] = { 0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f, 0.5f, 0.0f, 0.0f };

    //rsFlush();
	ISM->Flush();
    int cPt = percent;
    for (int i = 0; i <= cPt; i++)
    {
        int idx = 8 - i;
        Vector3D a( c_px[idx]*ext.w,     c_py[idx]*ext.h,     0.0f );
        Vector3D b( c_px[idx - 1]*ext.w, c_py[idx - 1]*ext.h, 0.0f );
        a += lt; b += lt;
        if (i < cPt) rsPoly2D( c, b, a, clr, clr, clr );
        if (i == cPt)
        {
            Vector3D d;
            d.sub( b, a );
            d *= percent - cPt;
            d += a;
            rsPoly2D( c, d, a, clr, clr, clr );
        }
    }
    rsFlush();
} // DrawPercentGauge

