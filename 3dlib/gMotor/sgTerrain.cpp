/*****************************************************************************/
/*	File:	sgTerrain.cpp
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	11-12-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sgGeometry.h"
#include "mNoise.h"
#include "mTriangle.h"
#include "mHeightmap.h"
#include "sgStateBlock.h"
#include "kUtilities.h"
#include "uiControl.h"

#include "gpMesh.h"
#include "sgTerrain.h"
#include "kNAryIterator.hpp"
#include "IShadowManager.h"

#include <limits>
#undef max

#ifndef _INLINES
#include "sgTerrain.inl"
#endif // _INLINES

ITerrain* ITerra = NULL;

BEGIN_NAMESPACE(sg)

const int c_MaxQuadTreeDepth = 16;
/*****************************************************************************/
/*	Class:	QuadIterator
/*	Desc:	Iterator for traversing quadtree
/*****************************************************************************/
template <class TQuery>
class QuadIterator : public NAryIterator<TerrainQuad, 4, c_MaxQuadTreeDepth, TQuery> 
{
	typedef NAryIterator<TerrainQuad, 4, c_MaxQuadTreeDepth, TQuery> BaseType;
public:
	QuadIterator( TerrainQuad* pRoot ) : BaseType( pRoot ){}
}; // class QuadIterator

/*****************************************************************************/
/*	Class:	LeafQuadIterator
/*	Desc:	Iterator for traversing quadtree leafs
/*****************************************************************************/
template <class TQuery>
class LeafQuadIterator : public NAryLeafIterator<TerrainQuad, 4, c_MaxQuadTreeDepth, TQuery> 
{
	typedef NAryLeafIterator<TerrainQuad, 4, c_MaxQuadTreeDepth, TQuery> BaseType;
public:
	LeafQuadIterator( TerrainQuad* pRoot ) : BaseType( pRoot ){}
}; // class LeafQuadIterator

/*****************************************************************************/
/*	Class:	FrustumCullQuery
/*	Desc:	Iterator query for terrain quadtree frustum culling
/*****************************************************************************/
class FrustumCullQuery
{
public:
	static  Frustum			frustum;
	static	bool Reject( const TerrainQuad* pQuad )
	{
		AABoundBox aabb = pQuad->GetQuadAABB();
		return !frustum.Overlap( aabb );
	}
}; // class FrustumCullQuery
Frustum	FrustumCullQuery::frustum;

/*****************************************************************************/
/*	Class:	LODQuery
/*	Desc:	Iterator query for terrain quadtree LOD picking
/*****************************************************************************/
class LODQuery
{
public:
	static float			bias;
	static Vector3D			viewPos;

	static	bool Reject( const TerrainQuad* pQuad )
	{
		const AABoundBox& aabb = pQuad->GetQuadAABB();
		float dist = aabb.GetCenter().distance( viewPos ) - aabb.GetDiagonal();
		if (dist >= bias) return true;
		return false;
	}
}; // class LODQuery

float		LODQuery::bias;
Vector3D	LODQuery::viewPos;

typedef OrQuery<TerrainQuad, FrustumCullQuery, LODQuery> CullLODQuery;
/*****************************************************************************/
/*	Class:	RctQuery
/*	Desc:	Iterator query for locating quads overlaping with rectangle
/*****************************************************************************/
class RctQuery
{
public:
	static  Rct	 rct;
	static	bool Reject( const TerrainQuad* pQuad )
	{
		return !rct.Overlap( pQuad->GetExtents() );
	}
}; // class RctQuery
Rct	 RctQuery::rct;

/*****************************************************************************/
/*	Class:	RayQuery
/*	Desc:	Iterator query for raypicking leafs
/*****************************************************************************/
class RayQuery
{
public:
	static  Ray3D	 ray;
	static	bool Reject( const TerrainQuad* pQuad )
	{
		return !pQuad->GetQuadAABB().Overlap( ray );
	}
}; // class RayQuery
Ray3D	 RayQuery::ray;

template <class TVert>
bool CreateJaggedPSoup( Primitive& pri, const Rct& ext, int wSeg, int hSeg, int jagShift = 0 )
{
	if (wSeg <= 0 || hSeg <= 0) return false;
	int nVert	= wSeg * hSeg * 6;
	int nPri	= wSeg * hSeg;
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
	pri.setNPri	( cV / 3 );
	return true;
} // CreateJaggedPSoup


/*****************************************************************************/
/*	Terrain implementation
/*****************************************************************************/
Vector3D Terrain::s_PShift;

Terrain::Terrain()
{
	ITerra				= this;

	m_bShowNormals		= false;
	m_ShowNormalLen		= 10.0f;
	m_TilesPerSide		= 0;
	m_OneTileSide		= 256.0f;
	m_SegmentsPerTile	= 8;
	m_GroundHeight		= 0.0f;
	m_LODBias			= 256.0f;
	m_bDrawCulling		= false;
	m_bDrawGeomCache	= false;
	m_bDrawTexCache		= false;
    m_bDrawBorder       = false;
    m_BorderLeftEdge    = 500.0f;  
    m_BorderRightEdge   = 500.0f; 
    m_BorderTopEdge     = 500.0f;   
    m_BorderBottomEdge  = 500.0f;

    m_CurrentQuad       = -1;

	m_pTextureCache		= NULL;
	m_pGeometryCache	= NULL;

	GetHeight			= DefGetHeight;
	SetHeight			= DefSetHeight;
	CreateTexture		= DefCreateTexture;	
	CreateGeometry		= DefCreateGeometry;	
	GetAABB				= DefGetAABB;		
	GetPVS				= DefGetPVS;
    CreateChunk         = DefCreateChunk;

	m_bDrawAABB			= false;
	m_MinLOD			= 0;
	m_FreezeCamera		= NULL;

	m_LastGeometryItem	= 0;
	m_LastTextureItem	= 0;
	m_QuadTextureSide	= 256;
	m_TexturesCreated	= 0;
	m_GeomCreated		= 0;

    m_LeftBand          = 64.0f;
    m_TopBand           = 128.0f;
    m_BottomBand        = 256.0f;
    m_RightBand         = 128.0f;
    m_CullBand          = 512.0f;
    m_bNeedPrecache     = false;

    m_ShadowLayerShader = IRS->GetShaderID( "terra_shadow_layer" );

	m_pTextureCache = AddChild<Group>( "TextureCache" );
	m_pTextureCache->SetInvisible();

	m_pGeometryCache = AddChild<Group>( "GeometryCache" );
	m_pGeometryCache->SetInvisible();
    SetPerSidePow		( pow3 );

	float ext = m_OneTileSide*m_TilesPerSide;
	SetExtents			(  Rct( -ext*0.5f, -ext*0.5f, ext, ext ) );

	s_PShift.set( 530, 0, 130 );
} // Terrain::Terrain

Terrain::~Terrain()
{
}

void Terrain::DrawGeomCache()
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
} // Terrain::DrawGeomCache

void Terrain::DrawTexCache()
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
} // Terrain::DrawTexCache

void Terrain::DrawCulling()
{
	if (m_Quads.size() == 0) return;
	AABoundBox aabb = RootQuad()->m_QuadAABB;
	float W = (aabb.maxv.x - aabb.minv.x);
	float H = (aabb.maxv.y - aabb.minv.y);

	Rct rct( 0, 256, 256, 256 );
	Rct vp = IRS->SetViewPort( rct );

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
		rsRect ( cRct, 0.0f, 0x33FF0000 );
		rsFrame( cRct, 0.0f, 0x99FF0000 );
	}
	BaseCamera* pCam = m_FreezeCamera ? m_FreezeCamera : BaseCamera::GetActiveCamera();
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
	rsFlushLines2D();
	IRS->SetViewPort( vp );
} // Terrain::DrawCulling

void Terrain::FreezeCamera( bool freeze )
{
	if (freeze)
	{
		if (!BaseCamera::GetActiveCamera()) return;
		m_FreezeCamera = (BaseCamera*)BaseCamera::GetActiveCamera()->Clone();
	}
	else
	{
		NodePool::DestroyNode( m_FreezeCamera );
		m_FreezeCamera = NULL;
	}
} // Terrain::FreezeCamera

void Terrain::DoVisibilityCulling()
{
	if (!RootQuad()) return;
	m_QDrawn.clear();

	BaseCamera* pCam = m_FreezeCamera ? m_FreezeCamera : BaseCamera::GetActiveCamera();
	if (!pCam) return;

	Frustum cullFrustum = pCam->GetFrustum();
    Frustum visFrustum = cullFrustum;
    float leftBand      = m_LeftBand;
    float topBand       = m_TopBand;
    float rightBand     = m_RightBand;
    float bottomBand    = m_BottomBand;
    float cullBand      = m_CullBand;
    if (m_MinLOD > 0) 
    {
        cullBand    *= m_MinLOD + 1;
    }
    cullFrustum.Extrude( cullBand );
    visFrustum.Extrude( leftBand, topBand, rightBand, bottomBand );
    Vector3D corners[8];
    int nV = cullFrustum.Intersection( Plane::xOy, corners );	

	Vector3D v[12]; 
	cullFrustum.Intersection( Plane::xOy, v );
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

    const QuadLevel& ql = m_QuadLevel[m_NQuadLevels - m_MinLOD - 1];
    
    int qBegX = (xMin - m_Extents.x)/ql.qWidth;
    int qBegY = (yMin - m_Extents.y)/ql.qHeight;  
    int qEndX = (xMax - m_Extents.x)/ql.qWidth;
    int qEndY = (yMax - m_Extents.y)/ql.qHeight;  
    for (int j = qBegY; j < qEndY; j++)
    {
        for (int i = qBegX; i < qEndX; i++)
        {
            int qIdx = i + j*ql.nSideQuads;
            if (qIdx < 0 || qIdx >= ql.nQuads) continue;
            TerrainQuad* pQuad = &m_Quads[ql.firstQuad + qIdx];
            pQuad->SetAlreadyDrawn( true );
            if (!visFrustum.Overlap( pQuad->GetQuadAABB() )) continue; 
            pQuad->SetAlreadyDrawn( false );
            m_QDrawn.push_back( pQuad );
        }
    }	
    m_bNeedPrecache = true;
} // Terrain::DoVisibilityCulling

BaseMesh* Terrain::AllocateGeometry()
{
    if (m_CurrentQuad <= 0) return NULL;
    TerrainQuad& quad = m_Quads[m_CurrentQuad];
    quad.m_LastFrame = IRS->GetCurFrame();
    int geomID = AllocateGeometryItem();
    if (geomID < 0 || geomID >= m_GeometryCache.size()) return NULL;
    TerrainGeometryItem& item = m_GeometryCache[geomID];
    item.m_QuadIndex = m_CurrentQuad;
    quad.AddGeometryID( geomID );
    return &(item.m_pGeometry->GetPrimitive());
} // Terrain::AllocateGeometry

int	Terrain::AllocateGeometryItem()
{
	DWORD frame		= IRS->GetCurFrame();
	int cacheSize	= m_GeometryCache.size();
	int nTries		= 0;
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
} // Terrain::AllocateGeometryItem

int	Terrain::AllocateTextureItem()
{
	DWORD frame		= IRS->GetCurFrame();
	int cacheSize	= m_TextureCache.size();
	int nTries		= 0;
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
} // Terrain::AllocateTextureItem

bool Terrain::PrecacheTexture( TerrainQuad* pQuad )
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
} // Terrain::PrecacheTexture

bool Terrain::PrecacheGeometry( TerrainQuad* pQuad )
{
    m_CurrentQuad = pQuad->GetIndex();
    bool res = CreateGeometry( pQuad->GetExtents(), pQuad->GetLOD() );
    int nGeom = pQuad->GetNGeoms();
    if (!res || nGeom <= 0) return false;

    if (pQuad->GetNGeoms() == 0) return false;
	int geomID = pQuad->GetGeometryID( 0 );
	const TerrainGeometryItem& item = m_GeometryCache[geomID];
	const BaseMesh& pri = item.m_pGeometry->GetPrimitive();
	AABoundBox aabb = AABoundBox::null;
	pri.GetAABB( aabb );
	for (int j = 1; j < nGeom; j++)
	{
	    geomID = pQuad->GetGeometryID( j );
	    const TerrainGeometryItem& citem = m_GeometryCache[geomID];
	    const BaseMesh& cpri = item.m_pGeometry->GetPrimitive();
	    AABoundBox cAABB = AABoundBox::null;
	    if (citem.m_pGeometry) 
	    {
	        cpri.GetAABB( cAABB );
	        if (!(cAABB == AABoundBox::null))
	        {
	            aabb.Union( cAABB );
	        }
	        else
	        {
	            assert( false );
	        }
	    }
	}
	Rct ext = pQuad->GetExtents();
	pQuad->SetQuadAABB( AABoundBox( ext, aabb.minv.z, aabb.maxv.z ) );
    return true;
} // Terrain::PrecacheGeometry

void Terrain::Init()  
{
    if(IRS) IRS->AddOnDestroyNotified( this );

    float texMem        = IRS->GetTexMemorySize();
    float surfSz        = m_QuadTextureSide*m_QuadTextureSide*2 + 1024;
    int   nSurf         = (int)((texMem*c_TerrainCacheVMEMQuote)/surfSz);
    m_TextureCacheSize  = tmin( c_MaxTextureCacheSize, nSurf );

    int tmMB = texMem/1024.0f/1024.0f;
    Log.Info( "Creating terrain cache. TexMem: %dMb, NSurf: %d", tmMB, m_TextureCacheSize );

    SetTextureCacheSize( m_TextureCacheSize );
    SetGeometryCacheSize( c_GeometryCacheSize ); 	
    InvalidateAABB();
} // Terrain::Init

const int c_MaxPrecachedPerFrame = 3;
void Terrain::PrecacheQuad( TerrainQuad* pQ )
{
    if (CreateGeometry && pQ && pQ->InvalidGeometry()) 
    { 
        PrecacheGeometry( pQ ); 
        m_NPrecached++; 
    }
    if (m_NPrecached >= c_MaxPrecachedPerFrame) return;
    if (CreateTexture && pQ && pQ->InvalidTexture() && 
        m_NTexVisible + 1 <= m_TextureCache.size())
    {
        PrecacheTexture( pQ ); 
        m_NPrecached++;
    }
} // Terrain::PrecacheQuad

void Terrain::Render()
{
	//if (IsDisabled()) return;
    if (m_TextureCache.size() == 0 && CreateGeometry)
    { 
        SetTextureCacheSize( m_TextureCacheSize ); 
    }

    if (m_GeometryCache.size() == 0) 
    { 
        SetGeometryCacheSize( c_GeometryCacheSize ); 
        InvalidateAABB(); 
    }

	IRS->ResetWorldMatrix();
	TransformNode::ResetTMStack();
    
    BaseCamera* pCam = m_FreezeCamera ? m_FreezeCamera : BaseCamera::GetActiveCamera();
    if (!pCam) return;

    //  if camera is not moving, get culling results from previous frame
    if (m_QDrawn.size() == 0 || 
        !m_LastCamera.pos.isEqual( pCam->GetPos() ) || 
        !m_LastCamera.dir.isEqual( pCam->GetDir() ))
    {
        if (m_QDrawn.size() != 0)
        //  extrapolate camera position/direction
        {
            //TODO
        }
	    DoVisibilityCulling();
        m_LastCamera.pos = pCam->GetPos();
        m_LastCamera.dir = pCam->GetDir();
    }

	m_TexturesCreated	= 0;
	m_GeomCreated		= 0;

	Rct vp = IRS->GetViewPort();
    if (m_bDrawBorder) DrawBorder();

	int nQuads = m_QDrawn.size();
    DWORD frame = IRS->GetCurFrame();

    //  update dirty geometry pieces
	for (int i = 0; i < nQuads; i++)
	{
		TerrainQuad* pQuad = m_QDrawn[i];
		if (pQuad->IsAlreadyDrawn()) continue;
		if (pQuad->InvalidGeometry())
		{
            bool res = PrecacheGeometry( pQuad );
            if (!res) pQuad->SetAlreadyDrawn();
            m_GeomCreated++;
		}
        pQuad->SetLastFrame( frame );
	}
    
    //  update dirty texture pieces
    m_NTexVisible = 0;
    if (CreateTexture)
    {
        for (int i = 0; i < nQuads; i++)
        {
            TerrainQuad* pQuad = m_QDrawn[i];
            if (pQuad->IsAlreadyDrawn()) continue;
            if (pQuad->InvalidTexture()) 
            {
                PrecacheTexture( pQuad );
            }
            m_NTexVisible++;
        }
    }
    
    //  if nothing was created this frame, take care of precaching
    m_NPrecached = 0;
    if (m_TexturesCreated == 0 && m_GeomCreated == 0 && m_bNeedPrecache)
    {        
        for (int i = 0; i < nQuads; i++)
        {
            TerrainQuad* pQuad = m_QDrawn[i];   
            if (pQuad->IsAlreadyDrawn()) continue;
            int level = m_NQuadLevels - pQuad->GetLOD() - 1;
            const QuadLevel& ql = m_QuadLevel[level];
            Rct rct = pQuad->GetExtents();
            float hw = rct.w*0.5f;
            float hh = rct.h*0.5f;
            
            TerrainQuad* pQ = GetQuad( level, rct.x - hw, rct.y + hh );
            PrecacheQuad( pQ );
            if (m_NPrecached >= c_MaxPrecachedPerFrame) break;

            pQ = GetQuad( level, rct.x + hw, rct.y - hh );
            PrecacheQuad( pQ );
            if (m_NPrecached >= c_MaxPrecachedPerFrame) break;
            
            pQ = GetQuad( level, rct.GetRight() + hw, rct.y + hh );
            PrecacheQuad( pQ );
            if (m_NPrecached >= c_MaxPrecachedPerFrame) break;

            pQ = GetQuad( level, rct.x + hw, rct.GetBottom() + hh );
            PrecacheQuad( pQ );
            if (m_NPrecached >= c_MaxPrecachedPerFrame) break;
        }
        if (m_NPrecached == 0) m_bNeedPrecache = false;
    }
    
	IRS->SetViewPort( vp );

	TransformNode::ResetTMStack();
    int nQDrawn = 0;
    int nGDrawn = 0;

    //  setup shadow layer
    int shTexID = IShadowMgr->GetShadowMapID();
    static int whiteTex = IRS->GetTextureID( "white.tga" );
    if (shTexID != -1)
    {
        IRS->SetTextureMatrix( IShadowMgr->CalcShadowMapTM(), 1 );
    }else shTexID = whiteTex; 

    //  render quads
	for (int i = 0; i < nQuads; i++)
	{
		TerrainQuad* pQuad = m_QDrawn[i];
		if (pQuad->IsAlreadyDrawn()) continue;
        
        int texID	= pQuad->GetTextureID();
		if (texID != TerrainQuad::c_BadID)
		{
			TerrainTextureItem& item = m_TextureCache[texID];
			if (item.m_pTexture) item.m_pTexture->Render();
		}

        int nGeom = pQuad->GetNGeoms();
        int nV = 0;
        int nP = 0;
        for (int j = 0; j < nGeom; j++)
        {
            int geomID	= pQuad->GetGeometryID( j );
		    if (geomID != TerrainQuad::c_BadID)
		    {
		    	const TerrainGeometryItem& item = m_GeometryCache[geomID];
		    	if (item.m_pGeometry) 
                {
                    BaseMesh& pri = item.m_pGeometry->GetPrimitive();
                    pri.setTexture( shTexID, 1 ); 
                    IRS->Draw( pri );
                    nV += pri.getNVert();
                    nP += pri.getNPri();
                    nGDrawn++;
                }

		    }
            else
            {
                assert( false );
            }
        }
        nQDrawn++;
	}

	if (m_bDrawAABB)
	{
		int nChunks = m_QDrawn.size();
		for (int i = 0; i < nChunks; i++)
		{
			TerrainQuad* pChunk = m_QDrawn[i];
			if (!pChunk) continue;
			DrawAABB( pChunk->m_QuadAABB, 0x220000FF, ColorValue::Blue );
		}
		IRS->ResetWorldMatrix();
		rsFlushLines3D();
	}

	if (m_FreezeCamera)
	{
		Frustum fr = m_FreezeCamera->GetFrustum();
		DrawFrustum( fr, 0x44BBBB22, 0x88BBBB22, true );
	}

	if (m_bDrawGeomCache)   DrawGeomCache();
	if (m_bDrawTexCache)    DrawTexCache();
	if (m_bDrawCulling)     DrawCulling();
    if (m_bShowNormals)     DrawNormals();

} // Terrain::Render

void Terrain::DrawNormals()
{
    int nQuads = m_QDrawn.size();
    for (int i = 0; i < nQuads; i++)
    {
        TerrainQuad* pQuad = m_QDrawn[i];
        int nGeom = pQuad->GetNGeoms();
        for (int j = 0; j < nGeom; j++)
        {
            int geomID	= pQuad->GetGeometryID( j );
            const TerrainGeometryItem& item = m_GeometryCache[geomID];
            if (!item.m_pGeometry) continue;
            
            BaseMesh& pri = item.m_pGeometry->GetPrimitive();
            VertexIterator it;
		    it << pri;
		    while (it)
            {
                Vector3D norm = it.n();
                Vector3D pos  = it;
                norm *= 100.0f;
                norm += pos;
                rsLine( pos, norm, 0xFFFF0000, 0xFFFF0000 );
                ++it;
            }
        }
    }
    rsFlushLines3D();
}  // Terrain::DrawNormals

void Terrain::SetTextureCacheSize( int nSurf )
{
	if (!m_pTextureCache) return;
	m_pTextureCache->RemoveChildren();
	InvalidateTexture();
	m_TextureCache.clear();

	char texName[64];
	for (int i = 0; i < nSurf; i++)
	{
		sprintf( texName, "Surface%02d", i );
		Texture* pTexture = m_pTextureCache->AddChild<Texture>( texName );
		pTexture->SetColorFormat	( cfBackBufferCompatible );
		pTexture->SetWidth			( m_QuadTextureSide );
		pTexture->SetHeight			( m_QuadTextureSide );
		pTexture->SetUsage			( tuRenderTarget );
		pTexture->SetMemoryPool		( mpVRAM );
		pTexture->SetNMips			( 2 );
		pTexture->CreateTexture		();

		TerrainTextureItem item;
		item.m_pTexture  = pTexture;
		item.m_QuadIndex = -1;
		m_TextureCache.push_back( item );
	}

} // Terrain::SetTextureCacheSize

void Terrain::SetGeometryCacheSize( int nGeom )
{
    if (nGeom == m_GeometryCache.size()) return;

	if (!m_pGeometryCache) return;
	m_pGeometryCache->RemoveChildren();

	InvalidateGeometry();
	m_GeometryCache.clear();

	char geomName[64];
	for (int i = 0; i < nGeom; i++)
	{
		sprintf( geomName, "GeomChunk%02d", i );
		Geometry* pGeometry = m_pGeometryCache->AddChild<Geometry>( geomName );
		TerrainGeometryItem item;
		item.m_pGeometry = pGeometry;
		item.m_QuadIndex = -1;
		m_GeometryCache.push_back( item );
	}

} // Terrain::SetGeometryCacheSize

void Terrain::SetPerSidePow( PowerOfTwo val )
{
	m_TilesPerSide = int( val );
} // Terrain::SetPerSidePow

void Terrain::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "Terrain", this );
	pm.f( "ShowNormals",		m_bShowNormals		);
	pm.f( "NormalLen",			m_ShowNormalLen		);

	pm.p( "VertexLighting",		VertexLightingEnabled, EnableVertexLighting );
	pm.p( "TilesPerSide",		GetPerSidePow, SetPerSidePow );
	pm.f( "OneTileSide",		m_OneTileSide		);
	pm.f( "SegmentsPerTile", 	m_SegmentsPerTile	);
	pm.f( "GroundHeight", 		m_GroundHeight 		);
	pm.p( "TexSet",				GetTexSet, SetTexSet );
	pm.m( "Reset",				Reset				);

	pm.f( "LODTreshold",		m_LODBias			);
	pm.f( "ForceLOD", 			m_MinLOD			);
	pm.f( "DrawCulling",		m_bDrawCulling		);
	pm.f( "DrawGeomCache",		m_bDrawGeomCache	);
	pm.f( "DrawTexCache",		m_bDrawTexCache		);
	pm.f( "DrawAABB",			m_bDrawAABB			);
	pm.p( "FreezeCamera",		IsFrozenCamera, FreezeCamera );
	pm.p( "QuadsDrawn",			GetDrawQueueSize	);
	pm.f( "TexturesCreated",	m_TexturesCreated	);
	pm.f( "GeomCreated",		m_GeomCreated		);
	pm.p( "TextureCacheSize",	GetTextureCacheSize, SetTextureCacheSize );
    pm.f( "LeftBand",           m_LeftBand              );
    pm.f( "TopBand",            m_TopBand               );
    pm.f( "RightBand",          m_RightBand             );
    pm.f( "BottomBand",         m_BottomBand            );
    pm.f( "CullBand",           m_CullBand              );

	pm.p( "PShiftX",			GetPShiftX, SetPShiftX );
	pm.p( "PShiftY",			GetPShiftY, SetPShiftY );
	pm.p( "PShiftZ",			GetPShiftZ, SetPShiftZ );
} // Terrain::Expose

void Terrain::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
}

void Terrain::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
}

void Terrain::Reset()
{
	InvalidateGeometry();
	InvalidateTexture();
} // Terrain::Reset

void Terrain::SetExtents( const Rct& ext )
{
    m_Extents = ext;

    int nQuads = 0;
    m_NQuadLevels = 0;
    DWORD sz = m_TilesPerSide;
    while (sz) { nQuads += sz * sz; sz >>= 1; m_NQuadLevels++; }

    m_Quads.resize( nQuads );

    TerrainQuad& qroot          = m_Quads[0];
    qroot.m_QuadAABB.minv	    = Vector3D( m_Extents.x, m_Extents.y, 0.0f );
    qroot.m_QuadAABB.maxv	    = Vector3D( m_Extents.GetRight(), m_Extents.GetBottom(), 0.0f );
    qroot.m_LOD				    = GetPower( (PowerOfTwo)m_TilesPerSide );

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
            q.m_pChild[1] = GetQuad( i + 1, qx + 3.0f*hs,  qy + hs      );
            q.m_pChild[2] = GetQuad( i + 1, qx + hs,       qy + 3.0f*hs );
            q.m_pChild[3] = GetQuad( i + 1, qx + 3.0f*hs,  qy + 3.0f*hs );
            int t = 0;
        }
    }
    
	for (int i = 0; i < nQuads; i++)
	{
		m_Quads[i].SetIndex( i );
	}	
} // Terrain::SetExtents

void Terrain::SetHeightmapPow( int hpow )
{
	SetPerSidePow( (PowerOfTwo) hpow );
}

void Terrain::InvalidateAABB( const Rct* rct )
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
} // Terrain::InvalidateAABB

ITerrainChunk* Terrain::DefCreateChunk()
{
    return new TerrainChunk();
}

void Terrain::DefGetAABB( const Rct& mapExt, AABoundBox& aabb )
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
} // Terrain::DefGetAABB

void Terrain::InvalidateGeometry( const Rct* rct )
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
} // Terrain::InvalidateGeometry

void Terrain::InvalidateTexture( const Rct* rct )
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
} // Terrain::InvalidateTexture

void Terrain::OnDestroyRenderSystem()
{
	InvalidateTexture();
	InvalidateGeometry();
}

bool Terrain::Pick( int mX, int mY, Vector3D& pt )
{
	BaseCamera* pCam = BaseCamera::GetActiveCamera();
	if (!pCam) return false;
	Ray3D ray;
	pCam->GetPickRay( mX, mY, ray );
	return Pick( ray.getOrig(), ray.getDir(), pt );
} // Terrain::Pick

Ray3D Terrain::s_PickRay;
int Terrain::DistCmp( const void *q1, const void *q2 )
{
    TerrainQuad* pq1 = *((TerrainQuad**)q1);
    TerrainQuad* pq2 = *((TerrainQuad**)q2);

    float d1 = s_PickRay.getOrig().distance2( pq1->GetQuadAABB().GetCenter() );
    float d2 = s_PickRay.getOrig().distance2( pq2->GetQuadAABB().GetCenter() );
    if (d1 < d2) return -1;
    if (d1 > d2) return  1;
    return 0;
} // Terrain::DistCmp

bool Terrain::Pick( const Vector3D& orig, const Vector3D& dir, Vector3D& pt )
{	
    const AABoundBox& aabb = RootQuad()->GetQuadAABB();
    Vector3D dst = dir;
    dst *= aabb.GetDiagonal();
    dst += orig;
    
    Ray3D ray( orig, dir );
    Vector2Df a = Vector2Df( orig.x, orig.y );
    Vector2Df b = Vector2Df( dst.x, dst.y );
    Vector3D va( a.x, a.y, ray.GetZ( a.x, a.y ) );
    Vector3D vb( b.x, b.y, ray.GetZ( b.x, b.y ) );
    float H = GetHeight( a.x, a.y );
    float pH;

    int nSteps = va.distance( vb ) / 16.0f;
    Vector3D d; d.sub( vb, va ); d /= nSteps;
    int prevSig = H < va.z ? -1 : 1;
    int totSteps = 0;
    for (int i = 0; i < nSteps; i++)
    {
        va += d;
        pH = H; H = GetHeight( va.x, va.y );
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
                float cH = GetHeight( c.x, c.y );
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
} // Terrain::Pick

void Terrain::EnableVertexLighting( bool val )
{
	m_bVertexLighting = val;
} // Terrain::SetVertexLighting

bool Terrain::VertexLightingEnabled() const
{
	return m_bVertexLighting;
} // Terrain::GetVertexLighting

int g_TexSet = 1;
int Terrain::GetTexSet() const
{
	return g_TexSet;
}

void Terrain::SetTexSet( int val ) 
{
	g_TexSet = val;
	InvalidateTexture();
}

int Terrain::DefGetPVS( DWORD* qID, int maxq )
{
	return 0;
}

bool Terrain::DefCreateTexture( int texID, const Rct& mapExt ) 
{ 
	static BaseMesh splat;
	if (splat.getNVert() == 0)
	{
		CreateJaggedPSoup<VertexTnL>( splat, Rct( 0.0f, -16.0f, 256.0f, 256.0f + 32.0f ), 8, 9 );
		int shID = IRS->GetShaderID( "hud" );
		int grID = IRS->GetTextureID( "Textures\\kground.bmp" );
		splat.setShader( shID );
		splat.setTexture( grID );
	}
	
	WORD*		idx = splat.getIndices();
	VertexTnL*	v	= (VertexTnL*)splat.getVertexData();
	
	int nV = splat.getNVert();
	
	srand( mapExt.x + mapExt.y * 15731 );

	int tx = g_TexSet%8;
	int ty = g_TexSet/8;

	for (int i = 0; i < nV; i += 3)
	{
		VertexTnL& v1 = v[i    ];
		VertexTnL& v2 = v[i + 1];
		VertexTnL& v3 = v[i + 2];
		
		float l = tx*64 + rndValuef( 0.0f, 32.0f ); 
		float t = ty*64 + rndValuef( 0.0f, 32.0f ); 

		float vl = tmin( v1.x, v2.x, v3.x ); 
		float vt = tmin( v1.y, v2.y, v3.y ); 
		
		v1.u = (l + v1.x - vl)/512.f;
		v1.v = (t + v1.y - vt)/512.f;

		v2.u = (l + v2.x - vl)/512.f;
		v2.v = (t + v2.y - vt)/512.f;
 
		v3.u = (l + v3.x - vl)/512.f;
		v3.v = (t + v3.y - vt)/512.f;
	}

	IRS->SetRenderTarget( texID );
	IRS->Draw( splat );
	IRS->SetRenderTarget( 0 );
	return true; 
} // Terrain::DefCreateTexture

bool Terrain::CreateQuadTexture( int quadIdx, int texID )
{
	if (texID < 0 || texID >= m_TextureCache.size()) return false;
	
	m_TexturesCreated++;
	TerrainQuad&		quad = m_Quads[quadIdx];
	TerrainTextureItem& item = m_TextureCache[texID];
	item.m_QuadIndex = quadIdx;
	
	bool res = CreateTexture( item.m_pTexture->GetTexID(), quad.GetExtents() );
	///item.m_pTexture->CreateMipLevels();
	return res;
} // Terrain::CreateQuadTexture

int	Terrain::DefGetHeight( int x, int y )
{
	return PerlinNoise( ((float)x + s_PShift.x)*0.002f, 
                        ((float)y + s_PShift.y)*0.002f )*1500.0f + s_PShift.z;
} // Terrain::DefGetHeight

bool Terrain::DefCreateGeometry( const Rct& mapExt, int lod )
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

	VertexIterator vit;
	vit << mesh;
	while (vit)
	{
		Vector3D& v = vit;
		v.z = ITerra->GetH( v.x, v.y );
		vit.n() = ITerra->GetNormal( v.x, v.y );

		if (v.x == ext.x || v.x == ext.GetRight() ||
			v.y == ext.y || v.y == ext.GetBottom())
		{
			v.z -= skirt;
		}

		++vit;
	}

	//  do texture mapping
	Matrix4D plTM;
	plTM.translation( Vector3D( mapExt.x, mapExt.y, 0.0f ) );
	Matrix4D ttm; ttm.inverse( plTM );
	UVMapPlanar<TerrainVertex>( mesh, ttm, mapExt.w, mapExt.h );  

    static int shID = IRS->GetShaderID( "terra_shadowed_lit" );
    mesh.setShader( shID );
    mesh.setTexture( -1, 0 );

	return true;
} // Terrain::DefCreateGeometry

void Terrain::DrawBorder()
{
	return;
    int nQuads = m_QDrawn.size();
    TerrainQuad* pRoot = RootQuad();
    AABoundBox aabb = pRoot->GetQuadAABB();
    static int s_ShaderID = IRS->GetShaderID( "mapborder" );
    rsSetShader( s_ShaderID );
    for (int i = 0; i < nQuads; i++)
    {
        TerrainQuad* pQuad = m_QDrawn[i];
        Rct ext = pQuad->GetExtents();
        AABoundBox qaabb = pQuad->GetQuadAABB();
        Vector3D a, b, c, d;
        if (fabs( ext.y - aabb.minv.y ) < c_Epsilon)
        {
            if (fabs( ext.x - aabb.minv.x ) < c_Epsilon)
            //  left top
            {
                a = Vector3D( ext.x - m_BorderLeftEdge, ext.y - m_BorderTopEdge, m_BorderMaxZ );
                b = Vector3D( ext.x, a.y, m_BorderMaxZ );
                c = Vector3D( ext.x - m_BorderLeftEdge, ext.y, m_BorderMaxZ );
                d = Vector3D( b.x, c.y, m_BorderMinZ );
                rsSetTexture( m_BorderLT.texID );
                rsQuad( a, b, c, d, m_BorderLT.uv, m_BorderColor );
                rsFlushPoly3D();
            }
            else if (fabs( ext.GetRight() - aabb.maxv.x ) < c_Epsilon) 
            //  right top
            {
                a = Vector3D( ext.GetRight(), ext.y - m_BorderTopEdge, m_BorderMaxZ );
                b = Vector3D( a.x + m_BorderRightEdge, a.y, m_BorderMaxZ );
                c = Vector3D( a.x, ext.y, m_BorderMinZ );
                d = Vector3D( b.x, ext.y, m_BorderMaxZ );
                rsSetTexture( m_BorderRT.texID );
                rsQuad( a, b, c, d, m_BorderRT.uv, m_BorderColor );
                rsFlushPoly3D();
            }
            
            //  top
            a = Vector3D( ext.x, ext.y - m_BorderTopEdge, m_BorderMaxZ );
            b = Vector3D( ext.GetRight(), ext.y - m_BorderTopEdge, m_BorderMaxZ );
            c = Vector3D( ext.x, ext.y, m_BorderMinZ );
            d = Vector3D( ext.GetRight(), ext.y, m_BorderMinZ );
            rsSetTexture( m_BorderT.texID );
            rsQuad( a, b, c, d, m_BorderT.uv, m_BorderColor );
            rsFlushPoly3D();
        }
        else if (fabs( ext.GetBottom() - aabb.maxv.y ) < c_Epsilon)
        {
            if (fabs( ext.x - aabb.minv.x ) < c_Epsilon)
            //  left bottom
            {
                a = Vector3D( ext.x - m_BorderLeftEdge, ext.GetBottom(), m_BorderMaxZ );
                b = Vector3D( ext.x, a.y, m_BorderMinZ );
                c = Vector3D( a.x, a.y + m_BorderBottomEdge, m_BorderMaxZ );
                d = Vector3D( b.x, c.y, m_BorderMaxZ );
                rsSetTexture( m_BorderLB.texID );
                rsQuad( a, b, c, d, m_BorderLB.uv, m_BorderColor );
                rsFlushPoly3D();
            }
            else if (fabs( ext.GetRight() - aabb.maxv.x ) < c_Epsilon) 
            //  right bottom
            {
                a = Vector3D( ext.GetRight(), ext.GetBottom(), m_BorderMinZ );
                b = Vector3D( a.x + m_BorderRightEdge, a.y, m_BorderMaxZ );
                c = Vector3D( a.x, a.y + m_BorderBottomEdge, m_BorderMaxZ );
                d = Vector3D( b.x, c.y, m_BorderMaxZ );
                rsSetTexture( m_BorderRB.texID );
                rsQuad( a, b, c, d, m_BorderRB.uv, m_BorderColor );
                rsFlushPoly3D();
            }
            //  bottom
            a = Vector3D( ext.x, ext.GetBottom(), m_BorderMinZ );
            b = Vector3D( ext.GetRight(), a.y, m_BorderMinZ );
            c = Vector3D( a.x, a.y + m_BorderBottomEdge, m_BorderMaxZ );
            d = Vector3D( b.x, c.y, m_BorderMaxZ );
            rsSetTexture( m_BorderB.texID );
            rsQuad( a, b, c, d, m_BorderB.uv, m_BorderColor );
            rsFlushPoly3D();
        } 

        if (fabs( ext.x - aabb.minv.x ) < c_Epsilon)
        //  left
        {
            a = Vector3D( ext.x - m_BorderLeftEdge, ext.y, m_BorderMaxZ );
            b = Vector3D( ext.x, ext.y, m_BorderMinZ );
            c = Vector3D( ext.x - m_BorderLeftEdge, ext.GetBottom(), m_BorderMaxZ );
            d = Vector3D( ext.x, ext.GetBottom(), m_BorderMinZ );
            rsSetTexture( m_BorderL.texID );
            rsQuad( a, b, c, d, m_BorderL.uv, m_BorderColor );
            rsFlushPoly3D();
        } else if (fabs( ext.GetRight() - aabb.maxv.x ) < c_Epsilon)
        //  right
        {
            a = Vector3D( ext.GetRight(), ext.y, m_BorderMinZ );
            b = Vector3D( ext.GetRight() + m_BorderRightEdge, ext.y, m_BorderMaxZ );
            c = Vector3D( a.x, ext.GetBottom(), m_BorderMinZ );
            d = Vector3D( b.x, ext.GetBottom(), m_BorderMaxZ );
            rsSetTexture( m_BorderR.texID );
            rsQuad( a, b, c, d, m_BorderR.uv, m_BorderColor );
            rsFlushPoly3D();
        }

    }
    rsFlushPoly3D();
} // Terrain::DrawBorder

bool Terrain::InitBorderElement( TerrainBorderElement& bel, XMLNode* pNode )
{   
    if (!pNode) return false;
    const char* texName = "";
    Rct uv;
    pNode->GetValue( "Rect", uv );
    pNode->GetValue( "Texture", texName );
    int texID = IRS->GetTextureID( texName );
    if (texID != -1)
    {
        const TextureDescr* pDescr = IRS->GetTextureDescr( texID );
        float w = pDescr->getSideX();
        float h = pDescr->getSideY();
        uv.x /= w; uv.w /= w;
        uv.y /= h; uv.h /= h;
    } else return false;
    bel.texID = texID;
    bel.uv = uv;
    return true;
} // Terrain::InitBorderElement

bool Terrain::SetBorderConfigFile( const char* fname )
{
    FInStream is( fname );
    if (is.NoFile()) false;
    XMLNode root( is );

    XMLNode* pT  = root.FindChild( "Top"          );
    XMLNode* pB  = root.FindChild( "Bottom"       );
    XMLNode* pL  = root.FindChild( "Left"         );
    XMLNode* pR  = root.FindChild( "Right"        );
    XMLNode* pLT = root.FindChild( "LeftTop"      );
    XMLNode* pRT = root.FindChild( "RightTop"     );
    XMLNode* pLB = root.FindChild( "LeftBottom"   );
    XMLNode* pRB = root.FindChild( "RightBottom"  );
    
    root.GetAttr( "LeftEdge",   m_BorderLeftEdge    );
    root.GetAttr( "RightEdge",  m_BorderRightEdge   );
    root.GetAttr( "TopEdge",    m_BorderTopEdge     );
    root.GetAttr( "BottomEdge", m_BorderBottomEdge  );
    root.GetAttr( "MinZ",       m_BorderMinZ        );
    root.GetAttr( "MaxZ",       m_BorderMaxZ        );
    root.GetAttr( "Color",      m_BorderColor       );

    InitBorderElement( m_BorderT,   pT  );
    InitBorderElement( m_BorderB,   pB  );
    InitBorderElement( m_BorderL,   pL  );
    InitBorderElement( m_BorderR,   pR  );
    InitBorderElement( m_BorderLT,  pLT );
    InitBorderElement( m_BorderLB,  pLB );
    InitBorderElement( m_BorderRT,  pRT );
    InitBorderElement( m_BorderRB,  pRB );

    m_bDrawBorder = true;
    return false;
} // Terrain::SetBorderConfigFile

END_NAMESPACE(sg)