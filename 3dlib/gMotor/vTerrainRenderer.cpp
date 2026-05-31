/*****************************************************************************/
/*    File:    vTerrainRenderer.cpp
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    11-12-2003
/*****************************************************************************/
#include "stdafx.h"

#include "ICamera.h"
#include "mNoise.h"
#include "mTriangle.h"
#include "mHeightmap.h"
#include "kUtilities.h"
#include "vMesh.h"
#include "IShadowManager.h"
#include "kSSEUtils.h"
#include "vStaticTerrain.h"
#include <limits>
#undef max

#include "vTreesRenderer.h"
#include "vTerrainRenderer.h"

DefaultTerrainCore          g_DefaultCore;
TerrainRenderer             g_Terrain;

ITerrain* ITerra = &g_Terrain;

const int c_MaxQuadTreeDepth = 16;
/*****************************************************************************/
/*    Class:    QuadIterator
/*    Desc:    Iterator for traversing quadtree
/*****************************************************************************/
template <class TQuery>
class QuadIterator : public NAryIterator<TerrainQuad, 4, c_MaxQuadTreeDepth, TQuery> 
{
    typedef NAryIterator<TerrainQuad, 4, c_MaxQuadTreeDepth, TQuery> BaseType;
public:
    QuadIterator( TerrainQuad* pRoot ) : BaseType( pRoot ){}
}; // class QuadIterator

/*****************************************************************************/
/*    Class:    RctQuery
/*    Desc:    Iterator query for locating quads overlaping with rectangle
/*****************************************************************************/
class RctQuery
{
public:
    static  Rct     rct;
    static    bool Reject( const TerrainQuad* pQuad )
    {
        return !rct.Overlap( pQuad->GetExtents() );
    }
}; // class RctQuery
Rct     RctQuery::rct;

/*****************************************************************************/
/*    Class:    RayQuery
/*    Desc:    Iterator query for raypicking leafs
/*****************************************************************************/
class RayQuery
{
public:
    static  Ray3D     ray;
    static    bool Reject( const TerrainQuad* pQuad )
    {
        return !pQuad->GetQuadAABB().Overlap( ray );
    }
}; // class RayQuery
Ray3D     RayQuery::ray;

template <class TVert>
bool CreateJaggedPSoup( Primitive& pri, const Rct& ext, int wSeg, int hSeg, int jagShift = 0 )
{
    if (wSeg <= 0 || hSeg <= 0) return false;
    int nVert    = wSeg * hSeg * 6;
    int nPri    = wSeg * hSeg;
    pri.create( nVert, 0, TVert::format(), ptTriangleList );

    TVert* v = (TVert*)pri.getVertexData();
    float wstep = ext.w / float( wSeg );
    float hstep = ext.h / float( hSeg );
    float hshift = -0.5f * ext.h / float( hSeg );

    int cV = 0;
    for (int j = 0; j < hSeg; j++)
    {
        for (int i = 0; i < wSeg; i++)
        {
            TVert& v0 = v[cV + 0]; TVert& v1 = v[cV + 1]; TVert& v2 = v[cV + 2];
            TVert& v3 = v[cV + 3]; TVert& v4 = v[cV + 4]; TVert& v5 = v[cV + 5];

            v0.x = ext.x + float( i )*wstep;
            v0.y = ext.y + float( j )*hstep;
            v0.z = 0.0f;
            v0.u = float( i )/float( wSeg );
            v0.v = float( j )/float( hSeg );

            if (((i + jagShift)&1) == 0) v0.y -= hshift;
            v0.diffuse = 0xFFFFFFFF;

            v1.x = ext.x + float( i + 1 )*wstep;
            v1.y = ext.y + float( j )*hstep;
            v1.z = 0.0f;
            v1.u = float( i + 1 )/float( wSeg );
            v1.v = float( j )/float( hSeg );

            if (((i + 1 + jagShift)&1) == 0) v1.y -= hshift;
            v1.diffuse = 0xFFFFFFFF;

            if (i&1)
            {
                v2.x = ext.x + float( i )*wstep;
                v2.y = ext.y + float( j + 1 )*hstep;
                v2.u = float( i )/float( wSeg );
                if (((i + jagShift)&1) == 0) v2.y -= hshift;
            }
            else
            {
                v2.x = ext.x + float( i + 1 )*wstep;
                v2.y = ext.y + float( j + 1 )*hstep;
                v2.u = float( i + 1 )/float( wSeg );
                if (((i + 1 + jagShift)&1) == 0) v2.y -= hshift;
            }
            v2.z = 0.0f;
            v2.v = float( j + 1 )/float( hSeg );
            v2.diffuse = 0xFFFFFFFF;

            //  second triangle
            if (i&1)
            {
                v3 = v2;
                v4 = v1;
            }
            else
            {
                v3 = v0;
                v4 = v2;
            }

            if (i&1)
            {
                v5.x = ext.x + float( i + 1 )*wstep;
                v5.y = ext.y + float( j + 1 )*hstep;
                v5.u = float( i + 1 )/float( wSeg );
                if (((i + 1 + jagShift)&1) == 0) v5.y -= hshift;
            }
            else
            {
                v5.x = ext.x + float( i )*wstep;
                v5.y = ext.y + float( j + 1 )*hstep;
                v5.u = float( i )/float( wSeg );
                if (((i + jagShift)&1) == 0) v5.y -= hshift;
            }

            v5.z = 0.0f;
            v5.v = float( j + 1 )/float( hSeg );
            v5.diffuse = 0xFFFFFFFF;

            cV += 6;
        }
    }

    pri.setNVert( cV );
    pri.setNPri    ( cV / 3 );
    return true;
} // CreateJaggedPSoup

/*****************************************************************************/
/*    DefaultTerrainCore implementation
/*****************************************************************************/
bool DefaultTerrainCore::CreateTexture( int texID, const Rct& mapExt )
{
    return true;
}

bool DefaultTerrainCore::CreateGeomery( const Rct& mapExt, int lod )
{
    BaseMesh* pMesh = ITerra->AllocateGeometry();
    if (!pMesh) return false;
    BaseMesh& mesh = *pMesh;

    Rct ext( mapExt );
    float skirt = mapExt.w / 8.0f;
    ext.x -= skirt;
    ext.y -= skirt;
    ext.w += skirt*2.0f;
    ext.h += skirt*2.0f;

    CreatePatchGrid<TerrainVertex>( mesh, ext, 10, 10 );    

    Vector3D ldir( 1, 1, 1 );
    ldir.normalize();

    VertexIterator vit;
    vit << mesh;
    while (vit)
    {
        Vector3D& v = vit.pos();
        Vector3D  n = ITerra->GetNormal( v.x, v.y );
        v.z = ITerra->GetH( v.x, v.y );
        vit.normal() = n;
        if (v.x == ext.x || v.x == ext.GetRight() ||
            v.y == ext.y || v.y == ext.GetBottom())
        {
            v.z -= skirt;
        }
        ColorValue c( 0xFFEEEEEE );
        Vector3D cv( c.r, c.g, c.b );
        float dl = ldir.dot( n );
        clamp( dl, 0.0f, 1.0f );
        cv *= dl;
        c.r = cv.x; c.g = cv.y; c.b = cv.z;
        vit.diffuse() = c;
        ++vit;
    }

    //  do texture mapping
    Matrix4D plTM;
    plTM.translation( Vector3D( mapExt.x, mapExt.y, 0.0f ) );
    Matrix4D ttm; ttm.inverse( plTM );
    int tile = Pow2( lod );
    UVMapPlanar<TerrainVertex>( mesh, ttm, mapExt.w/tile, mapExt.h/tile );  

    static int shID = IRS->GetShaderID( "terra_shadowed_hq" );
    mesh.setShader( shID );

    static int grTexID = IRS->GetTextureID( "defground.bmp" );
    mesh.setTexture( grTexID, 0 );

    return true;
}

bool DefaultTerrainCore::GetAABB( const Rct& mapExt, AABoundBox& aabb )
{
    aabb = AABoundBox( mapExt, std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());

    float ex = mapExt.GetRight();
    float ey = mapExt.GetBottom();
    float dx = mapExt.w / 8.0f;
    float dy = mapExt.h / 8.0f;

    for (float x = mapExt.x; x <= ex; x += dx)
    {
        for (float y = mapExt.y; y <= ey; y += dy)
        {
            float h = ITerra->GetH( x, y );
            if (h < aabb.minv.z) aabb.minv.z = h;
            if (h > aabb.maxv.z) aabb.maxv.z = h;
        }
    }
    return true;
} // DefaultTerrainCore::GetAABB

VertexDeclaration DefaultTerrainCore::GetVDecl() const
{
    return CreateVertexDeclaration( TerrainVertex::format() );
} // DefaultTerrainCore::GetVDecl

float DefaultTerrainCore::GetHeight( float x, float y )
{
    return m_HeightMap.GetH( x, y );
} // DefaultTerrainCore::GetHeight

void DefaultTerrainCore::SetExtents( const Rct& rct ) 
{
    m_HeightMap.SetExtents( rct );
} // DefaultTerrainCore::SetExtents

/*
//    static BaseMesh splat;
//    static Rct tRct;
//    static int grTexID = -1;
//
//    if (splat.getNVert() == 0)
//    {
//        tRct = Rct( 0.0f, -16.0f, 256.0f, 256.0f + 32.0f );
//        CreateJaggedPSoup<VertexTnL>( splat, tRct, 8, 9 );
//        int shID = IRS->GetShaderID( "terrain_splat" );
//        grTexID = IRS->GetTextureID( "kground.bmp" );
//        splat.setShader( shID );
//        splat.setTexture( grTexID );
//    }
//
//    float dx = mapExt.w/tRct.w;
//    float bx = mapExt.x;
//    float dy = mapExt.h/tRct.h;
//    float by = mapExt.y;
//
//    int nV = splat.getNVert();
//    srand( mapExt.x + mapExt.y * 15731 );
//    IRS->PushRenderTarget( texID );
//
//    int maxTexLayer = 1;
//
//    for (int layer = maxTexLayer; layer <= maxTexLayer; layer++)
//    {
//        WORD*        idx = splat.getIndices();
//        VertexTnL*    v    = (VertexTnL*)splat.getVertexData();
//        int         tx  = layer%8;
//        int         ty  = layer/8;
//        DWORD sumW = 0;
//        for (int i = 0; i < nV; i += 3)
//        {
//            VertexTnL& v1 = v[i    ];
//            VertexTnL& v2 = v[i + 1];
//            VertexTnL& v3 = v[i + 2];
//
//            DWORD c0 = GetTextureLayerColor( layer, v1.x*dx + bx, v1.y*dy + by );
//            DWORD c1 = GetTextureLayerColor( layer, v2.x*dx + bx, v2.y*dy + by );
//            DWORD c2 = GetTextureLayerColor( layer, v3.x*dx + bx, v3.y*dy + by );
//
//            sumW += Alpha(c0) + Alpha(c1) + Alpha(c2);
//
//            float l = tx*64 + rndValuef( 0.0f, 32.0f ); 
//            float t = ty*64 + rndValuef( 0.0f, 32.0f ); 
//
//            float vl = tmin( v1.x, v2.x, v3.x ); 
//            float vt = tmin( v1.y, v2.y, v3.y ); 
//
//            v1.u = (l + v1.x - vl)/512.f;
//            v1.v = (t + v1.y - vt)/512.f;
//            v1.diffuse = c0;
//
//            v2.u = (l + v2.x - vl)/512.f;
//            v2.v = (t + v2.y - vt)/512.f;
//            v2.diffuse = c1;
//
//            v3.u = (l + v3.x - vl)/512.f;
//            v3.v = (t + v3.y - vt)/512.f;
//            v3.diffuse = c2;
//        }
//        const DWORD c_TextureWeightBias = 10;
//        if (sumW > c_TextureWeightBias) DrawBM( splat );
//    }
//    IRS->PopRenderTarget();
//    return true; 
*/


/*****************************************************************************/
/*    TerrainRenderer implementation
/*****************************************************************************/
TerrainRenderer::TerrainRenderer()
{
    ITerra                = this;

    m_bShowNormals        = false;
    m_ShowNormalLen        = 10.0f;
    m_TilesPerSide        = 0;
    m_OneTileSide        = 512.0f;
    m_SegmentsPerTile    = 8;
    m_GroundHeight        = 0.0f;
    m_LODBias            = 3.0f;
    m_bDrawCulling        = false;
    m_bDrawGeomCache    = false;
    m_bDrawTexCache        = false;
    m_CurrentQuad       = -1;
    m_VBID              = -1;
    m_IBID              = -1;

    m_bDrawAABB            = false;
    m_MinLOD            = 0;
    m_LastGeometryItem    = 0;
    m_LastTextureItem    = 0;
    m_QuadTextureSide    = 256;
    m_TexturesCreated    = 0;
    m_GeomCreated        = 0;

    m_LeftBand          = 64.0f;
    m_TopBand           = 128.0f;
    m_BottomBand        = 256.0f;
    m_RightBand         = 128.0f;
    m_CullBand          = 512.0f;
    m_bNeedPrecache     = false;
    m_Quality           = 3.0f;
    m_SubstShader       = -1;

    SetCore             ( &g_DefaultCore );
    SetPerSidePow        ( pow7 );

    float ext = m_OneTileSide*m_TilesPerSide;

    Rct rct( Rct( -ext*0.5f, -ext*0.5f, ext, ext ) );
    SetExtents( rct );
    m_pCore->SetExtents( rct );
    m_VDecl             = m_pCore->GetVDecl();
} // TerrainRenderer::TerrainRenderer

void TerrainRenderer::SetCore( ITerrainCore* pCore ) 
{ 
    m_pCore = pCore; 
    m_VDecl = m_pCore->GetVDecl();
    if (IRS) m_VDecl.m_TypeID = IRS->RegisterVType( m_VDecl );
} // TerrainRenderer::SetCore

void TerrainRenderer::InvalidateAABB( TerrainQuad* pQuad )
{
    AABoundBox aabb;
    //  do speculative bounding box estimate
    //  later real bounding box will be substituted, when geometry will be available
    aabb = AABoundBox( pQuad->GetExtents(), c_MinTerraHeight, c_MaxTerraHeight );
    //GetAABB( pQuad->GetExtents(), aabb );
    pQuad->SetQuadAABB( aabb );
} // TerrainRenderer::InvalidateAABB


void TerrainRenderer::InvalidateTexture( TerrainQuad* pQuad ) 
{
    int itemID = pQuad->GetTextureID();
    if (itemID != TerrainQuad::c_BadID) m_TextureCache[itemID].m_QuadIndex = -1;
    pQuad->SetTextureID( TerrainQuad::c_BadID );
} // TerrainRenderer::InvalidateTexture


void TerrainRenderer::InvalidateGeometry( TerrainQuad* pQuad )
{
    int nGeom = pQuad->GetNGeoms();
    for (int i = 0; i < nGeom; i++)
    {
        int itemID = pQuad->GetGeometryID( i );
        if (itemID != TerrainQuad::c_BadID) m_GeometryCache[itemID].m_QuadIndex = -1;
    }
    pQuad->SetNGeoms( 0 );
    if(pQuad->m_LOD==0){
        AABoundBox AB=pQuad->GetQuadAABB();
        for(int i=0;i<m_InvalidateCallbacks.size();i++){        
            m_InvalidateCallbacks[i](Rct(AB.minv.x,AB.minv.y,AB.maxv.x-AB.minv.x,AB.maxv.y-AB.minv.y));
        }
    }
} // TerrainRenderer::InvalidateGeometry


void TerrainRenderer::DrawGeomCache()
{
    int nQuads = m_GeometryCache.size();
    if (nQuads == 0) return;

    AABoundBox aabb = RootQuad()->m_QuadAABB;
    float W = (aabb.maxv.x - aabb.minv.x);
    float H = (aabb.maxv.y - aabb.minv.y);

    Rct rct( 0, 256, 256, 256 );

    rsRect( rct, 0.0f, 0x220000FF );
    for (int i = 0; i < nQuads; i++)
    {
        TerrainGeometryItem& item = m_GeometryCache[i];
        int quadIdx = item.m_QuadIndex;
        if (quadIdx < 0) continue;
        TerrainQuad* pChunk = &m_Quads[quadIdx];
        if (!pChunk) continue;
        AABoundBox cAABB = pChunk->m_QuadAABB;
        Rct cRct;
        cRct.x = rct.x + rct.w  * (cAABB.minv.x - aabb.minv.x) / W;
        cRct.y = rct.y + rct.w  * (cAABB.minv.y - aabb.minv.y) / H;
        cRct.w = rct.w * (cAABB.maxv.x - cAABB.minv.x) / W;
        cRct.h = rct.h * (cAABB.maxv.y - cAABB.minv.y) / H;

        rsRect ( cRct, 0.0f, 0x33FFFFFF );
        rsFrame( cRct, 0.0f, 0x99FFFFFF );
    }
} // TerrainRenderer::DrawGeomCache


void TerrainRenderer::DrawTexCache()
{
    int nQuads = m_TextureCache.size();
    if (nQuads == 0) return;

    AABoundBox aabb = RootQuad()->m_QuadAABB;
    float W = (aabb.maxv.x - aabb.minv.x);
    float H = (aabb.maxv.y - aabb.minv.y);

    Rct rct( 0, 256, 256, 256 );

    rsRect( rct, 0.0f, 0x220000FF );
    for (int i = 0; i < nQuads; i++)
    {
        TerrainTextureItem& item = m_TextureCache[i];
        int quadIdx = item.m_QuadIndex;
        if (quadIdx < 0) continue;
        TerrainQuad* pChunk = &m_Quads[quadIdx];
        if (!pChunk) continue;
        AABoundBox cAABB = pChunk->m_QuadAABB;
        Rct cRct;
        cRct.x = rct.x + rct.w  * (cAABB.minv.x - aabb.minv.x) / W;
        cRct.y = rct.y + rct.w  * (cAABB.minv.y - aabb.minv.y) / H;
        cRct.w = rct.w * (cAABB.maxv.x - cAABB.minv.x) / W;
        cRct.h = rct.h * (cAABB.maxv.y - cAABB.minv.y) / H;
        cRct.Inflate( -1 );
        rsRect ( cRct, 0.0f, 0x33FFFF00 );
        rsFrame( cRct, 0.0f, 0x99FFFF00 );
    }
} // TerrainRenderer::DrawTexCache

void TerrainRenderer::DrawCulling()
{
    if (m_Quads.size() == 0) return;
    AABoundBox aabb = RootQuad()->m_QuadAABB;
    float W = (aabb.maxv.x - aabb.minv.x);
    float H = (aabb.maxv.y - aabb.minv.y);

    Rct rct( 0, 256, 256, 256 );
    Rct vp = IRS->GetViewPort();
    IRS->SetViewPort( rct );

    rsRect( rct, 0.0f, 0x220000FF );
    int nChunks = m_QDrawn.size();
    for (int i = 0; i < nChunks; i++)
    {
        TerrainQuad* pChunk = m_QDrawn[i];
        if (!pChunk) continue;
        AABoundBox cAABB = pChunk->m_QuadAABB;
        Rct cRct;
        cRct.x = rct.x + rct.w  * (cAABB.minv.x - aabb.minv.x) / W;
        cRct.y = rct.y + rct.w  * (cAABB.minv.y - aabb.minv.y) / H;
        cRct.w = rct.w * (cAABB.maxv.x - cAABB.minv.x) / W;
        cRct.h = rct.h * (cAABB.maxv.y - cAABB.minv.y) / H;
        rsRect ( cRct, 0.0f, 0x33FF5555 );
        rsFrame( cRct, 0.0f, 0x99FF5555 );
    }
    ICamera* pCam = GetCamera();
    if (pCam)
    {
        Frustum fr = pCam->GetFrustum();
        Vector3D v[12];
        int nV = fr.Intersection( Plane::xOy, v );    

        for (int i = 0; i < nV; i++)
        {
            v[i].x = rct.x + rct.w * (v[i].x - aabb.minv.x) / W;
            v[i].y = rct.y + rct.w * (v[i].y - aabb.minv.y) / H;
        }

        for (int i = 0; i < nV; i++)
        {
            int j = (i + 1)%nV; 
            rsLine( v[i].x, v[i].y, v[j].x, v[j].y, 0.0f, 0xFF00FF00 );
        }
    }
    rsFlushPoly2D();
    rsFlushLines2D();
    IRS->SetViewPort( vp );
    DrawText( 5.0f, 520.0f, 0xFFFFFF00, "Quads: %d(%d)", m_QDrawn.size(), c_TextureCacheSize );
    FlushText();
} // TerrainRenderer::DrawCulling

void TerrainRenderer::DoVisibilityCulling()
{
    if (!RootQuad()) return;
    m_QDrawn.clear();

    ICamera* pCam = GetCamera();
    if (!pCam) return;
    m_ViewerPos   = pCam->GetPosition();
    m_CullFrustum = pCam->GetFrustum();

    Vector3D v[12]; 
    int nV = m_CullFrustum.Intersection( Plane::xOy, v );
    float xMin =  FLT_MAX;
    float yMin =  FLT_MAX;
    float xMax = -FLT_MAX;
    float yMax = -FLT_MAX;
    for (int i = 0; i < nV; i++)
    {
        if (v[i].x < xMin) xMin = v[i].x;
        if (v[i].y < yMin) yMin = v[i].y;
        if (v[i].x > xMax) xMax = v[i].x;
        if (v[i].y > yMax) yMax = v[i].y;
    }
    xMin = tmax( m_Extents.x, xMin );
    yMin = tmax( m_Extents.y, yMin );
    xMax = tmin( m_Extents.GetRight(), xMax );
    yMax = tmin( m_Extents.GetBottom(), yMax );

    const QuadLevel& ql = m_QuadLevel[m_MinLOD];
    float qw = ql.qWidth;
    float qh = ql.qHeight;

    int qBegX = tmax<int>( 0, (xMin - m_Extents.x - qw)/qw );
    int qBegY = tmax<int>( 0, (yMin - m_Extents.y - qh)/qh );  
    int qEndX = tmin<int>( (xMax - m_Extents.x + qw)/qw, ql.nSideQuads - 1);
    int qEndY = tmin<int>( (yMax - m_Extents.y + qh)/qh, ql.nSideQuads - 1);  

    //  extrude testing area
    qBegX -= 4;
    qBegY -= 4;
    qEndX += 4;
    qEndY += 6;

    clamp( qBegX, 0, ql.nSideQuads );
    clamp( qBegY, 0, ql.nSideQuads );
    clamp( qEndX, 0, ql.nSideQuads );
    clamp( qEndY, 0, ql.nSideQuads );
        
    for (int j = qBegY; j <= qEndY; j++)
    {
        for (int i = qBegX; i <= qEndX; i++)
        {
            int qIdx = i + j*ql.nSideQuads;
            if (qIdx >= ql.nQuads) continue;
            TerrainQuad* pQuad = &m_Quads[ql.firstQuad + qIdx];
            ProcessQuadCulling( pQuad );
        }
    }    
    m_bNeedPrecache = true;
} // TerrainRenderer::DoVisibilityCulling

void TerrainRenderer::ProcessQuadCulling( TerrainQuad* pQuad )
{
    if (!pQuad) return;
    AABoundBox ab=pQuad->GetQuadAABB();
    ab.minv+=Vector3D(-64,-64,-64);
    ab.maxv+=Vector3D( 64, 64, 64);
    if (!m_CullFrustum.Overlap( ab )) return;
    if (pQuad->m_pChild[0] && 
        //pQuad->GetLOD() < m_MaxLOD &&
        !pQuad->SatisfiesLOD( m_ViewerPos, m_LODBias )) 
        //  divide quad
    {
        ProcessQuadCulling( pQuad->m_pChild[0] );
        ProcessQuadCulling( pQuad->m_pChild[1] );
        ProcessQuadCulling( pQuad->m_pChild[2] );
        ProcessQuadCulling( pQuad->m_pChild[3] );
        return;
    }

    pQuad->SetAlreadyDrawn( false );
    if (m_QDrawn.full()) return;
    m_QDrawn.push_back( pQuad );
} // TerrainRenderer::ProcessQuadCulling


BaseMesh* TerrainRenderer::AllocateGeometry()
{
    if (m_CurrentQuad <= 0) return NULL;
    TerrainQuad& quad = m_Quads[m_CurrentQuad];
    quad.m_LastFrame = IRS->GetCurFrame();
    int geomID = AllocateGeometryItem();
    if (geomID < 0 || geomID >= m_GeometryCache.size()) return NULL;
    TerrainGeometryItem& item = m_GeometryCache[geomID];
    item.Free();
    item.m_QuadIndex = m_CurrentQuad;
    quad.AddGeometryID( geomID );
    return &(item.m_Mesh);
} // TerrainRenderer::AllocateGeometry


int TerrainRenderer::AllocateGeometryItem()
{
    DWORD frame        = IRS->GetCurFrame();
    int cacheSize    = m_GeometryCache.size();
    int nTries        = 0;
    while (nTries < cacheSize)
    {
        m_LastGeometryItem = (m_LastGeometryItem + 1) % cacheSize;
        TerrainGeometryItem& item = m_GeometryCache[m_LastGeometryItem];
        if (item.m_QuadIndex == -1) return m_LastGeometryItem;
        TerrainQuad& quad = m_Quads[item.m_QuadIndex];

        if (quad.GetLastFrame() != frame)
        {
            int nGeom = quad.GetNGeoms();
            for (int i = 0; i < nGeom; i++)
            {
                int geomID = quad.GetGeometryID( i );
                m_GeometryCache[geomID].m_QuadIndex = -1;
            }
            quad.SetNGeoms( 0 );
            return m_LastGeometryItem;
        }
        nTries++;
    }
    return -1;
} // TerrainRenderer::AllocateGeometryItem


int TerrainRenderer::AllocateTextureItem()
{
    DWORD frame        = IRS->GetCurFrame();
    int cacheSize    = m_TextureCache.size();
    int nTries        = 0;
    while (nTries < cacheSize)
    {
        m_LastTextureItem = (m_LastTextureItem + 1) % cacheSize;
        TerrainTextureItem& item = m_TextureCache[m_LastTextureItem];
        if (item.m_QuadIndex == -1) return m_LastTextureItem;
        TerrainQuad& quad = m_Quads[item.m_QuadIndex];

        if (quad.GetLastFrame() != frame)
        {
            quad.SetTextureID( TerrainQuad::c_BadID );
            item.m_QuadIndex = -1;
            return m_LastTextureItem;
        }
        nTries++;
    }
    return -1;
} // TerrainRenderer::AllocateTextureItem

bool TerrainRenderer::PrecacheTexture( TerrainQuad* pQuad )
{
    WORD texID = AllocateTextureItem();
    bool res = CreateQuadTexture( pQuad->GetIndex(), texID );
    if (!res) 
    {
        pQuad->SetAlreadyDrawn();
        return false;
    }
    pQuad->SetTextureID( texID );
    pQuad->SetLastFrame( IRS->GetCurFrame() );
    return true;
} // TerrainRenderer::PrecacheTexture

bool TerrainRenderer::PrecacheGeometry( TerrainQuad* pQuad )
{
    m_CurrentQuad = pQuad->GetIndex();
    bool res = m_pCore->CreateGeomery( pQuad->GetExtents(), pQuad->GetLOD() );
    int nGeom = pQuad->GetNGeoms();
    if (!res || nGeom <= 0) return false;

    if (pQuad->GetNGeoms() == 0) return false;
    int geomID = pQuad->GetGeometryID( 0 );
    const TerrainGeometryItem& item = m_GeometryCache[geomID];
    const BaseMesh& pri = item.m_Mesh;
    AABoundBox aabb = pri.GetAABB();
    for (int j = 1; j < nGeom; j++)
    {
        geomID = pQuad->GetGeometryID( j );
        const TerrainGeometryItem& citem = m_GeometryCache[geomID];
        const BaseMesh& cpri = citem.m_Mesh;
        AABoundBox cAABB = cpri.GetAABB();
        if (!(cAABB == AABoundBox::null))
        {
            aabb.Union( cAABB );
        }
        else
        {
            assert( false );
        }
    }
    Rct ext = pQuad->GetExtents();
    ///aabb.Union( AABoundBox( ext, aabb.minv.z, aabb.maxv.z ) );
    aabb = AABoundBox( ext, aabb.minv.z, aabb.maxv.z );
    pQuad->SetQuadAABB( aabb );
    return true;
} // TerrainRenderer::PrecacheGeometry


void TerrainRenderer::Init()
{
    if (m_pCore->UseTextureCache()) SetTextureCacheSize( c_TextureCacheSize );
    SetGeometryCacheSize( c_GeometryCacheSize );     
    InvalidateAABB();
}

const int c_MaxPrecachedPerFrame = 3;

void TerrainRenderer::PrecacheQuad( TerrainQuad* pQ )
{
    if (pQ && pQ->InvalidGeometry()) 
    { 
        PrecacheGeometry( pQ ); 
        m_NPrecached++; 
    }
    if (m_NPrecached >= c_MaxPrecachedPerFrame) return;
    if (pQ && pQ->InvalidTexture() && 
        m_NTexVisible + 1 <= m_TextureCache.size())
    {
        PrecacheTexture( pQ ); 
        m_NPrecached++;
    }
} // TerrainRenderer::PrecacheQuad

int MaxTerrTris=100000*3;

void TerrainRenderer::Render()
{
    if (m_pCore->UseTextureCache() && m_TextureCache.size() == 0) 
    {
        SetTextureCacheSize( c_TextureCacheSize );
    }

    if (m_GeometryCache.size() == 0) 
    {
        SetGeometryCacheSize( c_GeometryCacheSize ); 
        InvalidateAABB(); 
    }

    //IRS->ResetWorldTM();

    m_TexturesCreated    = 0;
    m_GeomCreated        = 0;

    DoVisibilityCulling();

    Rct vp = IRS->GetViewPort();

    int nQuads = m_QDrawn.size();
    DWORD frame = IRS->GetCurFrame();

    //  update dirty geometry pieces
    for (int i = 0; i < nQuads; i++)
    {
        TerrainQuad* pQuad = m_QDrawn[i];
        if (pQuad->InvalidGeometry())
        {
            bool res = PrecacheGeometry( pQuad );
            if (!res) 
            {
                pQuad->SetAlreadyDrawn();
            }
            m_GeomCreated++;
        }
        pQuad->SetLastFrame( frame );
    }

    //  update dirty texture pieces
    m_NTexVisible = 0;
    if (m_pCore->UseTextureCache())
    {
        for (int i = 0; i < nQuads; i++)
        {
            TerrainQuad* pQuad = m_QDrawn[i];
            if (pQuad->InvalidTexture()) 
            {
                PrecacheTexture( pQuad );
            }
            m_NTexVisible++;
        }
    }

    IRS->SetViewPort( vp );
    //IRS->ResetWorldTM();

    int nQDrawn = 0;
    int nGDrawn = 0;

    //  setup shadow layer
    int shTexID = IShadowMgr->GetShadowMapID();
    static int whiteTex = IRS->GetTextureID( "white.tga" );
    if (shTexID != -1)
    {
        IRS->SetTextureTM( IShadowMgr->CalcShadowMapTM(), 1 );
    }else shTexID = whiteTex; 

    //  render quads
    if (m_IBID == -1) m_IBID = IRS->CreateIB( "TerrainRenderer", c_TerrainIBufferBytes, isWORD, false );
    if (m_VBID == -1) 
    {
        m_VDecl.m_TypeID = IRS->RegisterVType( m_VDecl );
        m_VBID = IRS->CreateVB( "TerrainRenderer", c_TerrainVBufferBytes, m_VDecl.m_TypeID, false );
    }
    IRS->SetIB( m_IBID );
    IRS->SetVB( m_VBID, m_VDecl.m_TypeID );

    IRS->SetShader( -1 );

    for (int i = 0; i < nQuads; i++)
    {
        TerrainQuad* pQuad = m_QDrawn[i];
        if (pQuad->IsAlreadyDrawn()) continue;

        int texID    = pQuad->GetTextureID();
        bool bHaveTexture = false;
        if (texID != TerrainQuad::c_BadID)
        {
            TerrainTextureItem& item = m_TextureCache[texID];
            texID        = item.m_TexID;
            bHaveTexture = true;
        }

        int nGeom = pQuad->GetNGeoms();
        int nV = 0;
        int nP = 0;
        for (int j = 0; j < nGeom; j++)
        {
            int geomID    = pQuad->GetGeometryID( j );
            if (geomID != TerrainQuad::c_BadID)
            {
                TerrainGeometryItem& item = m_GeometryCache[geomID];
                BaseMesh& pri = item.m_Mesh;
                int nV   = pri.getNVert();
                int nIdx = pri.getNInd();

                if (!bHaveTexture) texID = pri.getTexture( 0 );

                if (!IRS->IsIBStampValid( m_IBID, item.m_IBStamp ))
                {
                    BYTE* pOut = IRS->LockAppendIB( m_IBID, nIdx, item.m_IBPos, item.m_IBStamp );
                    if (pOut) 
                    {
                        /*SSE_*/memcpy( pOut, pri.getIndices(), nIdx*sizeof(WORD) );
                        IRS->UnlockIB( m_IBID );
                    }
                }

                if (!IRS->IsVBStampValid( m_VBID, item.m_VBStamp ))
                {   
                    BYTE* pOut  = IRS->LockAppendVB( m_VBID, nV, item.m_VBPos, item.m_VBStamp );
                    if (pOut) 
                    {
                        /*SSE_*/memcpy( pOut, pri.getVertexData(), nV*m_VDecl.m_VertexSize );
                        IRS->UnlockVB( m_VBID );
                    }
                }
                
                RenderTask& rt = IRS->AddTask();
                rt.m_bHasTM         = false;
                rt.m_ShaderID       = m_SubstShader == -1 ? pri.getShader() : m_SubstShader;
                rt.m_TexID[0]       = texID;
                rt.m_TexID[1]       = shTexID;
                rt.m_TexID[2]       = pri.getTexture(2);
                rt.m_TexID[3]       = -1;
                rt.m_TexID[4]       = -1;
                rt.m_TexID[5]       = -1;
                rt.m_TexID[6]       = -1;
                rt.m_TexID[7]       = -1;
                rt.m_bTransparent   = false;
                rt.m_VBufID         = m_VBID;
                rt.m_FirstVert      = item.m_VBPos;
                rt.m_NVert          = nV;
                rt.m_IBufID         = m_IBID;
                rt.m_FirstIdx       = item.m_IBPos;
                rt.m_NIdx           = min(nIdx,MaxTerrTris);
                rt.m_VType          = m_VDecl.m_TypeID;
                rt.m_bHasTM         = false;
                rt.m_Source         = "Terrain";
				rt.m_UseMeshConst   = pri.UseMeshConst;
				rt.m_ConstIdx		= pri.ConstIdx;
				rt.m_MeshConst		= pri.MeshConst;

				//AABoundBox AB=pQuad->GetQuadAABB();
				//IShadowMgr->AddCastingAABB(AB);
                
                nV += nV;
                nP += pri.getNPri();
               
                nGDrawn++;
            }
            else
            {
                assert( false );
            }
        }
        nQDrawn++;
    }

    IRS->Flush();
    
    ////  draw trees 
    //ITrees->Reset();

    //for (int i = 0; i < nQuads; i++)
    //{
    //    TerrainQuad* pQuad = m_QDrawn[i];
    //    ITrees->RenderBlock( pQuad->GetExtents(), pQuad->GetLOD() );
    //}  


    //IRS->Flush();
    //

    if (m_bDrawAABB)
    {
        int nChunks = m_QDrawn.size();
        for (int i = 0; i < nChunks; i++)
        {
            TerrainQuad* pChunk = m_QDrawn[i];
            if (!pChunk) continue;
            DrawAABB( pChunk->m_QuadAABB, 0x220000FF, ColorValue::Blue );
        }
        IRS->ResetWorldTM();
        rsFlushLines3D();
    }

    if (m_bDrawGeomCache)   DrawGeomCache();
    if (m_bDrawTexCache)    DrawTexCache();
    if (m_bDrawCulling)     DrawCulling();
    if (m_bShowNormals)     DrawNormals();

} // TerrainRenderer::Render


void TerrainRenderer::DrawNormals()
{
    int nQuads = m_QDrawn.size();
    for (int i = 0; i < nQuads; i++)
    {
        TerrainQuad* pQuad = m_QDrawn[i];
        int nGeom = pQuad->GetNGeoms();
        for (int j = 0; j < nGeom; j++)
        {
            int geomID    = pQuad->GetGeometryID( j );
            const TerrainGeometryItem& item = m_GeometryCache[geomID];

            const BaseMesh& pri = item.m_Mesh;
            VertexIterator it;
            it << pri;
            while (it)
            {
                Vector3D norm = it.normal();
                Vector3D pos  = it.pos();
                norm *= 100.0f;
                norm += pos;
                rsLine( pos, norm, 0xFFFF0000, 0xFFFF0000 );
                ++it;
            }
        }
    }
    rsFlushLines3D();
}  // TerrainRenderer::DrawNormals


void TerrainRenderer::SetTextureCacheSize( int nSurf )
{
    InvalidateTexture();
    m_TextureCache.clear();

    char texName[64];
    for (int i = 0; i < nSurf; i++)
    {
        sprintf( texName, "Surface%02d", i );
        int texID = IRS->CreateTexture( texName, m_QuadTextureSide, m_QuadTextureSide, 
            cfRGB565, 1, tmpDefault, true );
        TerrainTextureItem item;
        item.m_TexID     = texID;
        item.m_QuadIndex = -1;
        m_TextureCache.push_back( item );
    }

} // TerrainRenderer::SetTextureCacheSize


void TerrainRenderer::SetGeometryCacheSize( int nGeom )
{
    InvalidateGeometry();
    m_GeometryCache.clear();
    m_GeometryCache.resize( nGeom );
} // TerrainRenderer::SetGeometryCacheSize


void TerrainRenderer::SetPerSidePow( PowerOfTwo val )
{
    m_TilesPerSide = int( val );
} // TerrainRenderer::SetPerSidePow


void TerrainRenderer::Expose( PropertyMap& pm )
{
    pm.start( "TerrainRenderer", this );
    pm.f( "ShowNormals",        m_bShowNormals        );
    pm.f( "NormalLen",            m_ShowNormalLen        );
    pm.p( "TilesPerSide", &TerrainRenderer::GetPerSidePow, &TerrainRenderer::SetPerSidePow );
    pm.f( "OneTileSide",        m_OneTileSide        );
    pm.f( "SegmentsPerTile",     m_SegmentsPerTile    );
    pm.f( "GroundHeight",         m_GroundHeight         );
    pm.m( "Reset", &TerrainRenderer::Reset                );

    pm.f( "LODTreshold",        m_LODBias            );
    pm.f( "ForceLOD",             m_MinLOD            );
    pm.f( "DrawCulling",        m_bDrawCulling        );
    pm.f( "DrawGeomCache",        m_bDrawGeomCache    );
    pm.f( "DrawTexCache",        m_bDrawTexCache        );
    pm.f( "DrawAABB",            m_bDrawAABB            );
    pm.p( "QuadsDrawn", &TerrainRenderer::GetDrawQueueSize    );
    pm.f( "TexturesCreated",    m_TexturesCreated    );
    pm.f( "GeomCreated",        m_GeomCreated        );
    pm.p( "TextureCacheSize", &TerrainRenderer::GetTextureCacheSize, &TerrainRenderer::SetTextureCacheSize );
    pm.f( "LeftBand",           m_LeftBand              );
    pm.f( "TopBand",            m_TopBand               );
    pm.f( "RightBand",          m_RightBand             );
    pm.f( "BottomBand",         m_BottomBand            );
    pm.f( "CullBand",           m_CullBand              );
} // TerrainRenderer::Expose


void TerrainRenderer::Serialize( OutStream& os ) const
{
}


void TerrainRenderer::Unserialize( InStream& is )
{
}


void TerrainRenderer::Reset()
{
    InvalidateGeometry();
    InvalidateTexture();
} // TerrainRenderer::Reset


void TerrainRenderer::SetExtents( const Rct& ext )
{
    m_Extents = ext;

    int nQuads = 0;
    m_NQuadLevels = 0;
    DWORD sz = m_TilesPerSide;
    while (sz) { nQuads += sz * sz; sz >>= 1; m_NQuadLevels++; }
    m_MaxLOD = m_NQuadLevels;

    m_Quads.resize( nQuads );

    TerrainQuad& qroot          = m_Quads[0];
    qroot.m_QuadAABB.minv        = Vector3D( m_Extents.x, m_Extents.y, 0.0f );
    qroot.m_QuadAABB.maxv        = Vector3D( m_Extents.GetRight(), m_Extents.GetBottom(), 0.0f );
    qroot.m_LOD                    = GetPower( (PowerOfTwo)m_TilesPerSide );

    m_QuadLevel[0].firstQuad    = 0;
    m_QuadLevel[0].qWidth       = m_Extents.w;
    m_QuadLevel[0].qHeight      = m_Extents.h;
    m_QuadLevel[0].nQuads       = 1;
    m_QuadLevel[0].nSideQuads   = 1;

    int nLevelQuads = 4;
    int cQuad       = 1;
    int side        = 2;
    for (int lod = 1; lod < m_NQuadLevels; lod++)
    {
        nLevelQuads = side*side;
        float qw = m_Extents.w / float( side );
        float qh = m_Extents.h / float( side );
        m_QuadLevel[lod].firstQuad  = cQuad;
        m_QuadLevel[lod].qWidth     = qw;
        m_QuadLevel[lod].qHeight    = qh;
        m_QuadLevel[lod].nQuads     = nLevelQuads;
        m_QuadLevel[lod].nSideQuads = side;

        for (int j = 0; j < nLevelQuads; j++)
        {
            float qx = m_Extents.x + qw*float( j%side );
            float qy = m_Extents.y + qh*float( j/side );
            Rct qrct( qx, qy, qw, qh );

            TerrainQuad& q  = m_Quads[cQuad];
            q.m_QuadAABB    = AABoundBox( qrct, 0.0f, 0.0f );
            q.m_LOD         = m_NQuadLevels - lod - 1;
            q.m_pChild[0]   = NULL;
            q.m_pChild[1]   = NULL;
            q.m_pChild[2]   = NULL;
            q.m_pChild[3]   = NULL;
            cQuad++;
        }
        side *= 2;
    }

    for (int i = 0; i < m_NQuadLevels - 1; i++)
    {
        const QuadLevel& ql = m_QuadLevel[i];
        for (int j = 0; j < ql.nQuads; j++)
        {
            TerrainQuad& q = m_Quads[ql.firstQuad + j];
            Rct rct = q.GetExtents();
            float qx = rct.x;
            float qy = rct.y;
            float hs = rct.w * 0.25f;

            q.m_pChild[0] = GetQuad( i + 1, qx + hs,       qy + hs      );
            if (q.m_pChild[0]) q.m_pChild[0]->m_pParent = &q;

            q.m_pChild[1] = GetQuad( i + 1, qx + 3.0f*hs,  qy + hs      );
            if (q.m_pChild[1]) q.m_pChild[1]->m_pParent = &q;
            q.m_pChild[2] = GetQuad( i + 1, qx + hs,       qy + 3.0f*hs );
            if (q.m_pChild[2]) q.m_pChild[2]->m_pParent = &q;

            q.m_pChild[3] = GetQuad( i + 1, qx + 3.0f*hs,  qy + 3.0f*hs );
            if (q.m_pChild[3]) q.m_pChild[3]->m_pParent = &q;
        }
    }

    for (int i = 0; i < nQuads; i++)
    {
        m_Quads[i].SetIndex( i );
    }    
} // TerrainRenderer::SetExtents


void TerrainRenderer::SetHeightmapPow( int hpow )
{
    SetPerSidePow( (PowerOfTwo) hpow );
}


void TerrainRenderer::InvalidateAABB( const Rct* rct )
{
    if (rct == NULL)
    {
        int nQuads = m_Quads.size();
        for (int i = 0; i < nQuads; i++) 
        {
            InvalidateAABB( &m_Quads[i] );
        }
    }
    else
    {
        RctQuery::rct = *rct;
        QuadIterator<RctQuery> qit( RootQuad() );
        while (qit)
        {
            InvalidateAABB( ((TerrainQuad*)qit) );
            ++qit;
        }
    }
} // TerrainRenderer::InvalidateAABB

void TerrainRenderer::InvalidateGeometry( const Rct* rct )
{
    if (rct == NULL)
    {
        int nQuads = m_Quads.size();
        for (int i = 0; i < nQuads; i++) 
        {
            InvalidateGeometry( &m_Quads[i] );
        }
    }
    else
    {
        for (int i = 0; i < m_NQuadLevels; i++)
        {
            const QuadLevel& ql = m_QuadLevel[i];
            DWORD frame = IRS->GetCurFrame();

            int qBegX = (rct->x - m_Extents.x)/ql.qWidth - 1;
            int qBegY = (rct->y - m_Extents.y)/ql.qHeight - 1;  
            int qEndX = (rct->GetRight() - m_Extents.x)/ql.qWidth + 1;
            int qEndY = (rct->GetBottom() - m_Extents.y)/ql.qHeight + 1; 

            for (int j = qBegY; j < qEndY; j++)
            {
                for (int i = qBegX; i < qEndX; i++)
                {
                    int qIdx = i + j*ql.nSideQuads;
                    if (qIdx < 0 || qIdx >= ql.nQuads) continue;
                    TerrainQuad* pQuad = &m_Quads[ql.firstQuad + qIdx];
                    if (rct->Overlap( pQuad->GetExtents() )) InvalidateGeometry( pQuad );
                }
            }    
        }
    }
    m_QDrawn.clear();
} // TerrainRenderer::InvalidateGeometry


void TerrainRenderer::InvalidateTexture( const Rct* rct )
{
    if (rct == NULL)
    {
        int nQuads = m_Quads.size();
        for (int i = 0; i < nQuads; i++) InvalidateTexture( &m_Quads[i] );
    }
    else
    {
        for (int i = 0; i < m_NQuadLevels; i++)
        {
            const QuadLevel& ql = m_QuadLevel[i];
            DWORD frame = IRS->GetCurFrame();

            int qBegX = (rct->x - m_Extents.x)/ql.qWidth - 1;
            int qBegY = (rct->y - m_Extents.y)/ql.qHeight - 1;  
            int qEndX = (rct->GetRight() - m_Extents.x)/ql.qWidth + 1;
            int qEndY = (rct->GetBottom() - m_Extents.y)/ql.qHeight + 1; 

            for (int j = qBegY; j < qEndY; j++)
            {
                for (int i = qBegX; i < qEndX; i++)
                {
                    int qIdx = i + j*ql.nSideQuads;
                    if (qIdx < 0 || qIdx >= ql.nQuads) continue;
                    TerrainQuad* pQuad = &m_Quads[ql.firstQuad + qIdx];
                    if (rct->Overlap( pQuad->GetExtents() )) InvalidateTexture( pQuad );
                }
            }    
        }
    }
    m_QDrawn.clear();
} // TerrainRenderer::InvalidateTexture


void TerrainRenderer::OnDestroyRS()
{
    InvalidateTexture();
    InvalidateGeometry();
}


bool TerrainRenderer::Pick( int mX, int mY, Vector3D& pt )
{
    ICamera* pCam = GetCamera();
    if (!pCam) return false;
    Ray3D ray = pCam->GetPickRay( mX, mY );
    return Pick( ray.getOrig(), ray.getDir(), pt );
} // TerrainRenderer::Pick

Ray3D s_PickRay;

int TerrainRenderer::DistCmp( const void *q1, const void *q2 )
{
    TerrainQuad* pq1 = *((TerrainQuad**)q1);
    TerrainQuad* pq2 = *((TerrainQuad**)q2);

    float d1 = s_PickRay.getOrig().distance2( pq1->GetQuadAABB().GetCenter() );
    float d2 = s_PickRay.getOrig().distance2( pq2->GetQuadAABB().GetCenter() );
    if (d1 < d2) return -1;
    if (d1 > d2) return  1;
    return 0;
} // TerrainRenderer::DistCmp


bool TerrainRenderer::Pick( const Vector3D& orig, const Vector3D& dir, Vector3D& pt )
{    
    const AABoundBox& aabb = RootQuad()->GetQuadAABB();
    Vector3D dst = dir;
    dst *= aabb.GetDiagonal();
    dst += orig;

    Ray3D ray( orig, dir );
    Vector2D a = Vector2D( orig.x, orig.y );
    Vector2D b = Vector2D( dst.x, dst.y );
    Vector3D va( a.x, a.y, ray.GetZ( a.x, a.y ) );
    Vector3D vb( b.x, b.y, ray.GetZ( b.x, b.y ) );
    float H = GetH( a.x, a.y );
    float pH;

    int nSteps = va.distance( vb ) / 16.0f;
    Vector3D d; d.sub( vb, va ); d /= nSteps;
    int prevSig = H < va.z ? -1 : 1;
    int totSteps = 0;
    for (int i = 0; i < nSteps; i++)
    {
        va += d;
        pH = H; H = GetH( va.x, va.y );
        int sig = H < va.z ? -1 : 1;
        if (prevSig != sig)
            //  caught the interval with our point - now locate it
        {
            pt = va;
            pt -= d;
            while (fabs( d.z ) > c_Epsilon)
            {
                d *= 0.5f;
                Vector3D c( pt ); c += d;
                float cH = GetH( c.x, c.y );
                int cSig = cH < c.z ? -1 : 1;
                if (cSig == prevSig) pt = c;
                totSteps++;
            }
            return true;
        }
        totSteps++;
        prevSig = sig;
    }
    return false;
} // TerrainRenderer::Pick

bool TerrainRenderer::CreateQuadTexture( int quadIdx, int texID )
{
    if (texID < 0 || texID >= m_TextureCache.size()) return false;

    m_TexturesCreated++;
    TerrainQuad&        quad = m_Quads[quadIdx];
    TerrainTextureItem& item = m_TextureCache[texID];
    item.m_QuadIndex = quadIdx;    
    bool res = m_pCore->CreateTexture( item.m_TexID, quad.GetExtents() );
    return res;
} // TerrainRenderer::CreateQuadTexture


BaseMesh    g_TerraPatchBM;
int         g_CurTerraPatchTex = -1;
int         g_CurTerraPatchSha = -1;
const float c_PatchGranularity = 32.0f;
const int   c_MaxTerraPatchV   = 2048;
const int   c_MaxPatchSegments = 32;
void TerrainRenderer::DrawPatch(    float worldX, float worldY, 
                                    float width, float height, 
                                    float rotation, 
                                    const Rct& uv, float ux, float uy,
                                    int texID, DWORD color, bool bAdditive )
{
    static int shAdd = IRS->GetShaderID( "terrain_patch_add" );
    static int shMul = IRS->GetShaderID( "terrain_patch_mul" );

    if (g_TerraPatchBM.getMaxVert() == 0)
        //  create terrain patch mesh
    {
        g_TerraPatchBM.create( c_MaxTerraPatchV, c_MaxTerraPatchV*6, vfVertex2t, ptTriangleList );
    }
    int cTexID = texID;
    int cShaID = bAdditive ? shAdd : shMul;
    if (cTexID != g_TerraPatchBM.getTexture() || cShaID != g_TerraPatchBM.getShader())
    {
        FlushPatches();
        g_TerraPatchBM.setTexture( cTexID );
        g_TerraPatchBM.setShader ( cShaID );
    }

    int wSeg = tmin( tmax<int>( width/c_PatchGranularity, 1 ), c_MaxPatchSegments );
    int hSeg = tmin( tmax<int>( height/c_PatchGranularity, 1 ), c_MaxPatchSegments );
    int bV   = g_TerraPatchBM.getNVert(); 
    int bI   = g_TerraPatchBM.getNInd (); 
    int nV   = (wSeg + 1) * (hSeg + 1);
    int nI   = wSeg * hSeg * 6;
    if (bV + nV >= g_TerraPatchBM.getMaxVert() ||  
        bI + nI >= g_TerraPatchBM.getMaxInd()) 
    {
        FlushPatches();
        bI = bV = 0;
    }
    Vertex2t* v = ((Vertex2t*)g_TerraPatchBM.getVertexData()) + bV;
    WORD* idx = g_TerraPatchBM.getIndices() + bI;
    int cI = 0;
    int cV = bV;

    if (hSeg == 1 && wSeg == 1)
        //  simple quad case
    {
        idx[cI++] = cV;
        idx[cI++] = cV + 1;
        idx[cI++] = cV + 2;

        idx[cI++] = cV + 1;
        idx[cI++] = cV + 2;
        idx[cI++] = cV + 3;

        v[0].x          = -ux*width;
        v[0].y          = -uy*height;
        v[0].u          = uv.x;
        v[0].v          = uv.y;
        v[0].diffuse    = color;

        v[1].x          = ux*width;
        v[1].y          = -uy*height;
        v[1].u          = uv.x + uv.w;
        v[1].v          = uv.y;
        v[1].diffuse    = color;

        v[2].x          = -ux*width;
        v[2].y          = uy*height;
        v[2].u          = uv.x;
        v[2].v          = uv.y + uv.h;
        v[2].diffuse    = color;

        v[3].x          = ux*width;
        v[3].y          = uy*height;
        v[3].u          = uv.x + uv.w;
        v[3].v          = uv.y + uv.h;
        v[3].diffuse    = color;
    }
    else
        //  grid case
    {
        //  create indices
        for (int j = 0; j < hSeg; j++)
        {
            for (int i = 0; i < wSeg; i++)
            {
                if (i&1) 
                {
                    idx[cI++] = cV;
                    idx[cI++] = cV + 1;
                    idx[cI++] = cV + wSeg + 1; 

                    idx[cI++] = cV + wSeg + 1;
                    idx[cI++] = cV + 1;
                    idx[cI++] = cV + wSeg + 2; 
                }
                else
                {
                    idx[cI++] = cV;
                    idx[cI++] = cV + 1;
                    idx[cI++] = cV + wSeg + 2; 

                    idx[cI++] = cV;
                    idx[cI++] = cV + wSeg + 2;
                    idx[cI++] = cV + wSeg + 1; 
                }

                cV++;
            }
            cV++;
        }

        //  create vertices
        float wstep = width / float( wSeg );
        float hstep = height/float( hSeg );
        cV = 0;
        for (int j = 0; j <= hSeg; j++)
        {
            for (int i = 0; i <= wSeg; i++)
            {
                if (cV > c_MaxTerraPatchV) return;
                Vertex2t& vert = v[cV];
                vert.x = float( i )*wstep - ux*width;
                vert.y = float( j )*hstep - uy*height;
                vert.u = uv.x + uv.w*(vert.x/width + ux);
                vert.v = uv.y + uv.h*(vert.y/height + uy);
                vert.diffuse = color;
                cV++;
            }
        }
    }

    if (rotation > 0.0f)
    {
        //  create world transform matrix
        Matrix4D tm( Vector3D::one, Vector3D::oZ, DegToRad( rotation ), Vector3D( worldX, worldY, 0.0f ) );
        //  transform patch and fit to the ground
        for (int i = 0; i < nV; i++) 
        {
            v[i].z = 0.0f;
            tm.transformPt( *((Vector3D*)&v[i]) );
            v[i].z = ITerra->GetH( v[i].x, v[i].y );
            *((Vector3D*)&v[i]) = Vector3D( v[i].x, v[i].y, v[i].z );
        }
    }
    else
    {
        for (int i = 0; i < nV; i++) 
        {
            v[i].x += worldX;
            v[i].y += worldY;
            v[i].z = ITerra->GetH( v[i].x, v[i].y );
            *((Vector3D*)&v[i]) = Vector3D( v[i].x, v[i].y, v[i].z );
        }    
    }
    g_TerraPatchBM.setNVert( nV + bV );
    g_TerraPatchBM.setNInd ( nI + bI );
    g_TerraPatchBM.setNPri ( (nI + bI)/3 );
} // DrawPatch

void TerrainRenderer::FlushPatches()
{
    DrawBM( g_TerraPatchBM );
    g_TerraPatchBM.setNVert( 0 );
    g_TerraPatchBM.setNPri ( 0 );
    g_TerraPatchBM.setNInd ( 0 );
} // FlushPatches

void TerrainRenderer::SubstShader( int shaderID )
{
    m_SubstShader = shaderID;
}
void TerrainRenderer::SetInvalidateCallback( tpInvalidateQuadCallback* cb){
    m_InvalidateCallbacks.push_back( cb );
}




