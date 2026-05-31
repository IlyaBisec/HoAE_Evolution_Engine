/*****************************************************************/
/*  File:   sgGPSystem.cpp
/*  Desc:   Sprite manager implementation
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#include "stdafx.h"
#include "fexPack.h"
#include "rsVertex.h"
#include "kCache.h"

#include "mMath2D.h"
#include "mTriangle.h"

#include "kHash.hpp"
#include "kResource.h"
#include "gpText.h"
#include "gpPackage.h"

#include "gpGN16.h"
#include "gpGU16.h"

#include "gpBitmap.h"

#include "sg.h"
#include "sgGPSystem.h"
#include "kUtilities.h" 
#include "kStrUtil.h"
#include "kBmptool.h"
#include "gpSeries.h"

IGPSystem* GetGPSystemInterface()
{
	return sg::GPSystem::instance();
}

BEGIN_NAMESPACE( sg )

/*****************************************************************/
/*  GPSystem implementation
/*****************************************************************/
int				GPSystem::curCacheFactor;
GPSystem*		GPSystem::s_pInstance = NULL;

GPSystem::GPSystem() 
{
	curCamera			= 0;
	gpScale				= 1.0f;
	curLOD				= 0;
	curDiffuse			= 0xFFFFFFFF;
	curZ				= 0.0f;
	debugFont			= NULL;
	m_RootDirectory[0]	= 0;
	isInited			= false;
	clippingEnabled		= true;

	m_NumSurfaces		= 512;
	m_UVBias			= 0.0f;

	const int c_PackedGPMemBudget	= 10485760;
	const int c_UnpackedGPMemBudget	= 10485760 * 3;
	GPSeq::s_PackSegMgr.SetMemoryBudget( c_PackedGPMemBudget );
	GPSeq::s_PixDataMgr.SetMemoryBudget( c_UnpackedGPMemBudget );

	SetName( "GPSystem" );

	m_pSurfaces = AddChild<Group>( "Surfaces" );
	m_pPackages = AddChild<Group>( "Packages" );
	
	s_pInstance = this;

	m_NumWSpritesDrawn = 0;
} // GPSystem::GPSystem

GPSystem::~GPSystem()
{
	delete debugFont;
}

void GPSystem::SetNumSurfaces( int val )
{

}

void GPSystem::Expose( PropertyMap& pmap )
{
	pmap.start<Parent>( "GPSystem", this );
	pmap.prop( "NumSurfaces", GetNumSurfaces, SetNumSurfaces );
	pmap.field( "WorldSprites", m_NumWSpritesDrawn, NULL, true );
} // GPSystem::Expose

void GPSystem::SetTexCoordBias( float bias ) 
{
	m_UVBias = bias;
	GPBucket::s_TexCoordBias = bias;
} // GPSystem::SetTexCoordBias

void GPSystem::Init()
{
	_getcwd( m_RootDirectory, _MAX_PATH );
	GPSeq::s_PixelCache.Init( RC::iRS, this );
	sprBucket.resize( m_NumSurfaces );
	for (int i = 0; i < m_NumSurfaces; i++)
	{
		sprBucket[i].Init();
		int texID =  GPSeq::s_PixelCache.GetItemTextureSurfID( i );
		sprBucket[i].setTexture( texID );
		//char texName[256];
		//sprintf( texName, "GPSurf%02d", i );
		//Texture* pTexture = m_pSurfaces->AddChild<Texture>( texName );
		//pTexture->SetTexID( texID );
	}

	curBucket		= -1;
	curCacheFactor  = GetTickCount();

	static int shPrim	= RC::iRS->GetShaderID( "hudZDisable" );

	Texture* pWhiteTex = AddChild<Texture>( "white.bmp" );
	pWhiteTex->Render();

	GPBucket::s_CurShader = shPrim;

	//  primitive buckets
	const int c_NumAuxVert	= 256;
	lineBucket2D.create			( c_NumAuxVert, 0, vfTnL, ptLineList );
	lineBucket2D.setShader		( shPrim );
	lineBucket2D.setTexture		( pWhiteTex->GetTexID() );

	lineBucket3D.create			( c_NumAuxVert, 0, vf2Tex, ptLineList );
	lineBucket3D.setShader		( shPrim );
	lineBucket3D.setTexture		( pWhiteTex->GetTexID() );

	rectBucket2D.create			( c_NumAuxVert, 0, vfTnL, ptTriangleList );
	rectBucket2D.setShader		( shPrim );
	rectBucket2D.setTexture		( pWhiteTex->GetTexID() );
	rectBucket2D.setIsQuadList	( true );

	rectBucket3D.create			( c_NumAuxVert, 0, vf2Tex, ptTriangleList );
	rectBucket3D.setShader		( shPrim );
	rectBucket3D.setTexture		( pWhiteTex->GetTexID() );
	rectBucket3D.setIsQuadList	( true ); 

	EnableZBuffer( false );
	EnableClipping();

	debugFont = new GPFont();
	debugFont->Init( "Arial", 8 );

	static const int c_DefaultGPInstAllocatorPage = 4096 * 16;
	alloc.SetPageSize( c_DefaultGPInstAllocatorPage );

	isInited = true;
}  // GPSystem::Init

void GPSystem::SetRootDirectory( const char* rdir )
{
	strcpy( m_RootDirectory, rdir );
}

int GPSystem::SetCurrentShader( int shID ) 
{ 
	int res = GPBucket::s_CurShader; 
	GPBucket::s_CurShader = shID; 
	return res;
} // GPSystem::SetCurShader

int GPSystem::GetCurrentShader() const
{ 
	return GPBucket::s_CurShader;
} // GPSystem::SetCurShader

void GPSystem::SetCurrentZ( float z )
{
	curZ = z;	
}

float	GPSystem::GetCurrentZ()	const
{
	return curZ;
}

DWORD	GPSystem::GetCurrentDiffuse() const
{
	return curDiffuse;
}

void GPSystem::SetCurrentDiffuse( DWORD color )
{
	curDiffuse = color;
}

const char*	GPSystem::GetGPPath( int seqID )
{
	GPSeq *pGps = gpHash.elem( seqID );
	if (!pGps)
	//  default path
	{
		static char path[_MAX_PATH];		
		//  default path and extension: 
		//  .g16 file in the game "root" folder
		strcpy( path, gpHash.entry( seqID ).key );
		strcat( path, ".g16" );
		return path;
	}
	return pGps->GetPath();
} // GPSystem::GetGPPath

void GPSystem::DrawFillRect( float x, float y, float w, float h, DWORD color )
{
	if (rectBucket2D.getNVert() >= rectBucket2D.getMaxVert() - 4)
	{
		RC::iRS->Draw( rectBucket2D );
		rectBucket2D.setNInd ( 0 );
		rectBucket2D.setNVert( 0 );
		rectBucket2D.setNPri ( 0 );
	}

	VertexTnL* vert = (VertexTnL*)rectBucket2D.getVertexData() + rectBucket2D.getNVert();
	vert[0].setXYZW( x,		y,		0.0f, 1.0f );
	vert[1].setXYZW( x + w,	y,		0.0f, 1.0f );
	vert[2].setXYZW( x,		y + h,	0.0f, 1.0f );
	vert[3].setXYZW( x + w,	y + h,	0.0f, 1.0f );

	vert[0].setDiffuse( color );
	vert[1].setDiffuse( color );
	vert[2].setDiffuse( color );
	vert[3].setDiffuse( color );
	
	rectBucket2D.incNVert		( 4 );
	rectBucket2D.incNPri		( 2 );
} // GPSystem::DrawFillRect

void GPSystem::DrawFillRect(	const Vector3D& lt, const Vector3D& rt, 
								const Vector3D& lb, const Vector3D& rb, 
								DWORD clt, DWORD crt, DWORD clb, DWORD crb )
{
	if (rectBucket3D.getNVert() >= rectBucket3D.getMaxVert() - 4)
	{
		RC::iRS->Draw( rectBucket3D );
		rectBucket3D.setNInd ( 0 );
		rectBucket3D.setNVert( 0 );
		rectBucket3D.setNPri ( 0 );
	}

	Vertex2t* vert = (Vertex2t*)rectBucket3D.getVertexData() + rectBucket3D.getNVert();
	vert[0].setXYZ( lt.x, lt.y,	lt.z );
	vert[1].setXYZ( rt.x, rt.y,	rt.z );
	vert[2].setXYZ( lb.x, lb.y,	lb.z );
	vert[3].setXYZ( rb.x, rb.y,	rb.z );

	vert[0].setDiffuse( clt );
	vert[1].setDiffuse( crt );
	vert[2].setDiffuse( clb );
	vert[3].setDiffuse( crb );

	rectBucket2D.incNVert		( 4 );
	rectBucket2D.incNPri		( 2 );
} // GPSystem::DrawFillRect

void GPSystem::DrawLine( float x1, float y1, float x2, float y2, DWORD color )
{	
	if (lineBucket2D.getNVert() >= lineBucket2D.getMaxVert() - 2)
	{
		RC::iRS->Draw( lineBucket2D );
		lineBucket2D.setNInd ( 0 );
		lineBucket2D.setNVert( 0 );
		lineBucket2D.setNPri ( 0 );
	}

	VertexTnL* vert = (VertexTnL*)lineBucket2D.getVertexData() + lineBucket2D.getNVert();
	vert[0].setXYZW( x1, y1, 0.0f, 1.0f );
	vert[0].setDiffuse( color );
	vert[1].setXYZW( x2, y2, 0.0f, 1.0f );
	vert[1].setDiffuse( color );
	lineBucket2D.incNVert		( 2 );
	lineBucket2D.incNPri		( 1 );
} // GPSystem::DrawLine

void GPSystem::DrawRect( float x, float y, float w, float h, DWORD color )
{
	DrawLine( x, y, x + w, y, color );
	DrawLine( x + w, y, x + w, y + h, color );
	DrawLine( x + w, y + h, x, y + h, color );
	DrawLine( x, y + h, x, y, color );
} // GPSystem::DrawRect

void GPSystem::DrawLine( const Vector3D& beg, const Vector3D& end, DWORD color )
{
	if (lineBucket3D.getNVert() >= lineBucket3D.getMaxVert() - 2)
	{
		RC::iRS->Draw( lineBucket3D );
		lineBucket3D.setNInd ( 0 );
		lineBucket3D.setNVert( 0 );
		lineBucket3D.setNPri ( 0 );
	}
	Vertex2t* vert = (Vertex2t*)lineBucket3D.getVertexData() + lineBucket3D.getNVert();
	vert[0].setXYZ( beg.x, beg.y, beg.z );
	vert[0].setDiffuse( color );
	vert[1].setXYZ( end.x, end.y, end.z );
	vert[1].setDiffuse( color );
	lineBucket3D.incNVert		( 2 );
	lineBucket3D.incNPri		( 1 );
} // GPSystem::DrawLine

void GPSystem::DrawString( const char* str, 
							float x, float y, float z,
							DWORD color, float height )
{
	if (debugFont)
	{
		debugFont->DrawString( str, x, y, z, color, height, taLeftTop );
	}
} // GPSystem::DrawString

void GPSystem::DrawFillRect( const Vector3D& lt, const Vector3D& rt, 
								const Vector3D& lb, const Vector3D& rb, 
								DWORD color )
{
	if (rectBucket3D.getNVert() >= rectBucket3D.getMaxVert() - 4)
	{
		RC::iRS->Draw( rectBucket3D );
		rectBucket3D.setNInd ( 0 );
		rectBucket3D.setNVert( 0 );
		rectBucket3D.setNPri ( 0 );
	}

	Vertex2t* vert = (Vertex2t*)rectBucket3D.getVertexData() + rectBucket3D.getNVert();
	vert[0].setXYZ( lt.x, lt.y,	lt.z );
	vert[1].setXYZ( rt.x, rt.y,	rt.z );
	vert[2].setXYZ( lb.x, lb.y,	lb.z );
	vert[3].setXYZ( rb.x, rb.y,	rb.z );

	vert[0].setDiffuse( color );
	vert[1].setDiffuse( color );
	vert[2].setDiffuse( color );
	vert[3].setDiffuse( color );
	
	rectBucket3D.incNVert		( 4 );
	rectBucket3D.incNPri		( 2 );
} // GPSystem::DrawFillRect

void GPSystem::EnableZBuffer( bool enable )
{
	static bool zEnabled = false;
	static int shSprite2D	= RC::iRS->GetShaderID( "hudZDisable" );
	static int shSprite2DZ	= RC::iRS->GetShaderID( "hud" );

	if (enable == zEnabled) return;
	zEnabled = enable;

	FlushBatches();
	if (enable) GPBucket::s_CurShader = shSprite2DZ;
		else GPBucket::s_CurShader = shSprite2D;
	
} // GPSystem::EnableZBuffer

/*---------------------------------------------------------------------------*/
/*	Func:	GPSystem::GetGPAlpha	
/*	Desc:	returns alpha value of the given pixel inside sprite
/*	Parm:	ptX, ptY - points coordinates in local sprite space
/*			precise - when true, alpha is returned even if there is no pixel 
/*				data in system memory
/*	Ret:	Alpha value or, if pixel data is unavailable:
/*				c_InsideChunks - pixel is inside chunks
/*				c_OutsideChunks - pixel is outside chunks
/*---------------------------------------------------------------------------*/
DWORD GPSystem::GetGPAlpha( int seqID, int frameIdx, 
							int ptX, int ptY, 
							bool precise )
{
	if (seqID < 0 || seqID > GetGPNum()) return c_OutsideChunks;

	BYTE* pChunk = 0;
	GPSeq* pSeq = GetSeq( seqID );
	if (!pSeq) return c_OutsideChunks;
	//assert( pSeq );
	GPFrame* pFrame = pSeq->GetFrame( frameIdx );
	if (!pFrame) return c_OutsideChunks;
	
	if (ptX > pFrame->width || ptY > pFrame->height) 
	{
		return c_OutsideChunks;
	}

	GPFrameInst* pFrameInst = pFrame->lastFrameInst;
	if (!pFrameInst)
	{
		pFrameInst = GetFrameInst( seqID, frameIdx, 0 );
	}

	BYTE* pixData = pFrameInst->GetPixelData();
	
	for (int i = 0; i < pFrame->nChunks; i++)
	{
		GPChunkHeader*	chunk = (GPChunkHeader*) pixData;
		int side = chunk->GetSide();
		
		//  check if point is inside chunk square
		if (ptX < chunk->GetX() || 
			ptY < chunk->GetY() ||
			ptX > chunk->GetX() + side || 
			ptY > chunk->GetY() + side) 
		{
			pixData += chunk->GetSizeBytes(); 
			continue;
		}

		if (!precise) return c_InsideChunks;
		//  precise per-pixel testing
		WORD* pPix = (WORD*) chunk->GetPixelData();
		int idx = ptX - chunk->GetX() + (ptY - chunk->GetY()) * side;
		return (pPix[idx] & 0xF000) >> 13;
	}
	return c_OutsideChunks;
} // GPSystem::GetGPAlpha

/*---------------------------------------------------------------*
/*  Func:	GPSystem::PreLoadGPImage
/*	Desc:	Preloads gp information, actual data is not loaded yet
/*	Parm:	gpName - file name of sprite package, without extension
/*	Ret:	identifier of the sprite package
/*---------------------------------------------------------------*/
int	GPSystem::PreLoadGPImage( const char* gpName )
{
	int seqID = gpHash.find( gpName );
	if (seqID == NO_ELEMENT)
	{
		//  right now we push NULL pointer to the GPSeq,
		//  later, in LoadGP it will be created
		seqID = gpHash.add( gpName, NULL ); 
		m_pPackages->AddChild<GPPackage>( gpName );
	}

	assert( seqID != NO_ELEMENT );
	return seqID;
}//  PreLoadGPImage

bool GPSystem::LoadGP( int seqID, const char* gpPath )
{
	GPSeq *pGps = gpHash.elem( seqID );
	if (!pGps)
	{
		if (gpPath == NULL) gpPath = GetGPPath( seqID );
		if (m_RootDirectory[0])
		{
			char fullPath[_MAX_PATH];
			strcpy( fullPath, m_RootDirectory );
			if (fullPath[strlen( fullPath ) - 1] != '\\') strcat( fullPath, "\\" );
			strcat( fullPath, gpPath );
			pGps = GPSeq::CreateHeaderFromFile( fullPath );
		}
		else
		{
			pGps = GPSeq::CreateHeaderFromFile( gpPath );	
		}

		if (!pGps) 
		{
			gpHash.entry( seqID ).el = (GPSeq*)c_NoGPFile;
			return false;
		}
		pGps->SetID( seqID );
		gpHash.entry( seqID ).el = pGps;
	}
	return true;
} // GPSystem::LoadGP

void GPSystem::UnLoadGP( int seqID )
{
	todo( "GPSystem::UnLoadGP" );
}

void GPSystem::CleanBuckets()
{
	for (int i = 0; i < m_NumSurfaces; i++)
	{
		if (sprBucket[i].getNVert() == 0) continue;
		
		sprBucket[i].setNVert( 0 );
	}
} // GPSystem::CleanBuckets

void GPSystem::DrawBuckets()
{
	for (int i = 0; i < m_NumSurfaces; i++)
	{
		sprBucket[i].Flush();
	}
	FlushPrimBucket();
} // GPSystem::drawBuckets

void GPSystem::OnFrame()
{
	GPSeq::s_PixelCache.OnFrame();
	FlushBatches();
	m_NumWSpritesDrawn = 0;
} // GPSystem::OnFrame

void GPSystem::FlushBatches()
{
	DrawBuckets();
	FlushPrimBucket();
	curCacheFactor = GetTickCount();
	GPFrame::curCacheFactor = curCacheFactor;
	BaseResource::curCacheFactor = curCacheFactor;
} // GPSystem::FlushBatches


struct CountAlpha
{
	static GPSystem*	pGP;
	static DWORD		alpha;
	static DWORD		nPix;
	static int			seqID;
	static int			frameID;

	static bool SumAlpha( int x, int y )
	{
		nPix++;
		alpha += pGP->GetGPAlpha( seqID, frameID, x, y, true );
		return true;
	}

	static bool MaxAlpha( int x, int y )
	{
		nPix++;
		DWORD cAlpha = pGP->GetGPAlpha( seqID, frameID, x, y, true );
		if (cAlpha > alpha) alpha = cAlpha;
		return true;
	}
}; // struct CountAlpha

GPSystem*	CountAlpha::pGP		= NULL;
DWORD		CountAlpha::alpha	= 0;
DWORD		CountAlpha::nPix	= 0;
int			CountAlpha::seqID	= 0;
int			CountAlpha::frameID	= 0;

BYTE GPSystem::GetMaxAlpha( int seqID, int frameID, const Triangle2D& tri )
{
	CountAlpha::pGP		= this;
	CountAlpha::alpha	= 0;
	CountAlpha::nPix	= 0;
	CountAlpha::seqID	= seqID;
	CountAlpha::frameID = frameID;

	tri.Rasterize( 1.0f, CountAlpha::MaxAlpha );
	return CountAlpha::alpha;
} // GPSystem::GetMaxAlpha 

float GPSystem::GetAvgAlpha( int seqID, int frameID, const Triangle2D& tri )
{	
	CountAlpha::pGP		= this;
	CountAlpha::alpha	= 0;
	CountAlpha::nPix	= 0;
	CountAlpha::seqID	= seqID;
	CountAlpha::frameID = frameID;

	tri.Rasterize( 1.0f, CountAlpha::SumAlpha );
	
	if (CountAlpha::nPix == 0) return 0;
	return float( CountAlpha::alpha ) / float( CountAlpha::nPix );
} // GPSystem::GetAvgAlpha

/*---------------------------------------------------------------------------*/
/*	Func:	GPSystem::DrawSprite
/*	Desc:	Generic sprite drawing function
/*	Parm:	gpID		- package ID
/*			sprID		- sprite ID
/*			x, y, z		- position in screen space
/*---------------------------------------------------------------------------*/
bool GPSystem::DrawSprite(	int	gpID, int sprID, 
							float x, float y, float z,
							bool mirrored, DWORD color )
{
	if (gpID < 0 || !isInited) return false;

	GPFrameInst* frameInst = GetFrameInstForDrawing( gpID, sprID, color, curLOD );
	if (frameInst == NULL) return false;

	if (clippingEnabled)
	{
		Rct rct( x, y, frameInst->pFrame->width, frameInst->pFrame->height );
		Rct vp( curViewPort.x, curViewPort.y, curViewPort.width, curViewPort.height );
		if (!vp.Overlap( rct ))
		{
			return false;
		}
	}

	//  move chunks to the batch 
	int	nChunks	= frameInst->GetNChunks(); 
	int	numV	= nChunks * 4;

	if (gpScale == 1.0f)
	{
		for (int i = 0; i < nChunks; i++)
		{
			const GPChunk&	chunk	= frameInst->GetChunk( i );
			GPBucket&	bucket	= sprBucket[chunk.GetSurfID()];
			bucket.SetCurrentVertexFormat( vfTnL );
			bucket.SetIsQuadList( true );
			if (mirrored)
			{
				bucket.AddQuadMirrored( chunk, x, y, z, curDiffuse );
			}
			else
			{
				bucket.AddQuad( chunk, x, y, z, curDiffuse );
			}
		}
	}
	else
	{
		for (int i = 0; i < nChunks; i++)
		{
			const GPChunk&	chunk	= frameInst->GetChunk( i );
			GPBucket&	bucket	= sprBucket[chunk.GetSurfID()];
			bucket.SetCurrentVertexFormat( vfTnL );
			bucket.SetIsQuadList( true );
			if (mirrored)
			{
				bucket.AddQuadMirroredScaled( chunk, x, y, z, gpScale, curDiffuse );
			}
			else
			{
				bucket.AddQuadScaled( chunk, x, y, z, gpScale, curDiffuse );
			}
		}
	}
	return true;
} // GPSystem::DrawSprite

void GPSystem::Dump()
{
	GPSeq::s_PixelCache.Dump();
}

/*---------------------------------------------------------------------------*/
/*	Func:	GPSystem::DrawSpriteTransformed
/*	Desc:	Draws post-transformed in screen space sprite
/*	Parm:	gpID		- package ID
/*			sprID		- sprite ID
/*			transf		- screen-space transform matrix 
/*---------------------------------------------------------------------------*/
bool GPSystem::DrawSpriteTransformed( int gpID, int sprID, const Matrix4D& transf, DWORD color )
{
	if (gpID < 0 || !isInited) return false;

	GPFrameInst* frameInst = GetFrameInstForDrawing( gpID, sprID, color, curLOD );
	if (frameInst == NULL) return false;
	
	//  move chunks to the batch 
	int	nChunks	= frameInst->GetNChunks(); 
	int	numV	= nChunks * 4;

	for (int i = 0; i < nChunks; i++)
	{
		GPChunk&	chunk	= frameInst->GetChunk( i );
		GPBucket&	bucket	= sprBucket[chunk.GetSurfID()];
		bucket.SetCurrentVertexFormat( vfTnL );
		bucket.SetIsQuadList( true );
		bucket.AddQuad( chunk, transf, curDiffuse );
	}
	return true;
} // GPSystem::DrawSpriteTransformed

bool GPSystem::DrawSpriteAligned(	int gpID, int sprID, 
									const Plane& pl, 
									float x, float y, float	z,
									bool mirrored,
									DWORD color )
{
	if (gpID < 0 || !isInited) return false;

	GPFrameInst* frameInst = GetFrameInstForDrawing( gpID, sprID, color, curLOD );
	if (frameInst == NULL) return false;

	//  move chunks to the batch 
	int	nChunks	= frameInst->GetNChunks(); 
	int	numV	= nChunks * 4;
	
	for (int i = 0; i < nChunks; i++)
	{
		const GPChunk&	chunk	= frameInst->GetChunk( i );
		GPBucket&	bucket	= sprBucket[chunk.GetSurfID()];
		bucket.SetCurrentVertexFormat( vfTnL );
		bucket.SetIsQuadList( true );
		if (mirrored)
		{
			bucket.AddQuadMirroredAligned( chunk, pl, x, y, z, gpScale, curDiffuse );
		}
		else
		{
			bucket.AddQuadAligned( chunk, pl, x, y, z, gpScale, curDiffuse );
		}
	}
	return true;
} // GPSystem::DrawSpriteAligned

/*---------------------------------------------------------------------------*/
/*	Func:	GPSystem::DrawSprite
/*	Desc:	Generic sprite drawing function
/*	Parm:	gpID		- package ID
/*			sprID		- sprite ID
/*			x, y, z		- position in screen space
/*---------------------------------------------------------------------------*/
bool GPSystem::DrawWSprite(	int	gpID, int sprID, 
							float x, float y, float z,
							bool mirrored, DWORD color )
{
	if (gpID < 0 || !isInited) return false;

	GPFrameInst* frameInst = GetFrameInstForDrawing( gpID, sprID, color, curLOD );
	if (frameInst == NULL) return false;

	//  move chunks to the batch 
	int	nChunks	= frameInst->GetNChunks(); 
	int	numV	= nChunks * 4;

	if (gpScale == 1.0f)
	{
		for (int i = 0; i < nChunks; i++)
		{
			const GPChunk&	chunk	= frameInst->GetChunk( i );
			GPBucket&	bucket	= sprBucket[chunk.GetSurfID()];
			bucket.SetCurrentVertexFormat( vf2Tex );
			bucket.SetIsQuadList( true );
			if (mirrored)
			{
				bucket.AddQuadMirrored( chunk, x, y, z, curDiffuse );
			}
			else
			{
				bucket.AddQuad( chunk, x, y, z, curDiffuse );
			}
		}
	}
	else
	{
		for (int i = 0; i < nChunks; i++)
		{
			const GPChunk&	chunk	= frameInst->GetChunk( i );
			GPBucket&	bucket	= sprBucket[chunk.GetSurfID()];
			bucket.SetCurrentVertexFormat( vf2Tex );
			bucket.SetIsQuadList( true );
			if (mirrored)
			{
				bucket.AddQuadMirroredScaled( chunk, x, y, z, gpScale, curDiffuse );
			}
			else
			{
				bucket.AddQuadScaled( chunk, x, y, z, gpScale, curDiffuse );
			}
		}
	}
	return true;
} // GPSystem::DrawWSprite

/*---------------------------------------------------------------------------*/
/*	Func:	GPSystem::DrawWChunk
/*	Desc:	Adds to GP queue single chunk from big complex sprite, 
/*				which is pulled onto its own geometry
/*	Parm:	gpID, sprID, chunkID - package, frame, chunk index
/*			geom - mesh with sprite geometry
/*			cPoly, nPoly - start polygon and number of polygons to draw
/*			cVert, nVert - start vertices and number of vertices to draw
/*	Ret:	true, when things go clear...well, nevermind
/*---------------------------------------------------------------------------*/
bool GPSystem::DrawWChunk( int gpID, int sprID, int chunkID, const Vector3D& pos, 
							BaseMesh& geom, 
							int cPoly, int nPoly, 
							int cVert, int nVert, 
							DWORD color )
{
	if (gpID < 0 || !isInited) return false;
	GPFrameInst* frameInst = GetFrameInstForDrawing( gpID, sprID, color, curLOD );
	if (frameInst == NULL) return false;
	assert( chunkID >= 0 && chunkID < frameInst->GetNChunks() );

	const GPChunk&	chunk	= frameInst->GetChunk( chunkID );
	GPBucket&	bucket	= sprBucket[chunk.GetSurfID()];
	bucket.SetCurrentVertexFormat( geom.getVertexFormat() );
	bucket.SetIsQuadList( false );	
	bucket.AddQuad( chunk, pos, GetCurrentDiffuse(), geom, 
					 cVert, nVert, cPoly, nPoly );
	return true;	
} // GPSystem::DrawWChunk

bool GPSystem::DrawWSprite( int gpID, int sprID, const Vector3D& pos, 
							BaseMesh& geom, DWORD color )
{
	if (gpID < 0 || !isInited) return false;
	GPFrameInst* frameInst = GetFrameInstForDrawing( gpID, sprID, color, curLOD );
	if (frameInst == NULL) return false;

	int		nChunks	= frameInst->GetNChunks(); 
	BYTE*	pVert	= geom.getVertexData();
	int		stride	= geom.getVertexStride();

	for (int i = 0; i < nChunks; i++)
	{
		const GPChunk&	chunk	= frameInst->GetChunk( i );
		GPBucket&	bucket	= sprBucket[chunk.GetSurfID()];
		bucket.SetCurrentVertexFormat( vf2Tex );
		bucket.SetIsQuadList( true );
		bucket.AddQuad( chunk, pos, pVert, curDiffuse );
		pVert += stride * 4;
	}
	return true;	
} // GPSystem::DrawWChunk

/*---------------------------------------------------------------------------*/
/*	Func:	GPSystem::DrawWSpriteTransformed
/*	Desc:	Draws post-transformed in screen space sprite
/*	Parm:	gpID		- package ID
/*			sprID		- sprite ID
/*			transf		- screen-space transform matrix 
/*---------------------------------------------------------------------------*/
bool GPSystem::DrawWSpriteTransformed( int gpID, int sprID, const Matrix4D& transf, DWORD color )
{
	if (gpID < 0 || !isInited) return false;

	GPFrameInst* frameInst = GetFrameInstForDrawing( gpID, sprID, color, curLOD );
	if (frameInst == NULL) return false;

	//  move chunks to the batch 
	int	nChunks	= frameInst->GetNChunks(); 
	int	numV	= nChunks * 4;

	for (int i = 0; i < nChunks; i++)
	{
		const GPChunk&	chunk	= frameInst->GetChunk( i );
		GPBucket&	bucket	= sprBucket[chunk.GetSurfID()];
		bucket.SetCurrentVertexFormat( vf2Tex );
		bucket.SetIsQuadList( true );
		bucket.AddQuad( chunk, transf, curDiffuse );
	}
	m_NumWSpritesDrawn++;
	return true;
} // GPSystem::DrawWSpriteTransformed

bool GPSystem::DrawWSpriteAligned(	int gpID, int sprID, 
									const Plane& pl, 
									float x, float y, float	z,
									bool mirrored,
									DWORD color )
{
	if (gpID < 0 || !isInited) return false;

	GPFrameInst* frameInst = GetFrameInstForDrawing( gpID, sprID, color, curLOD );
	if (frameInst == NULL) return false;

	//  move chunks to the batch 
	int	nChunks	= frameInst->GetNChunks(); 
	int	numV	= nChunks * 4;

	
	for (int i = 0; i < nChunks; i++)
	{
		const GPChunk&	chunk	= frameInst->GetChunk( i );
		GPBucket&	bucket	= sprBucket[chunk.GetSurfID()];
		bucket.SetCurrentVertexFormat( vf2Tex );
		bucket.SetIsQuadList( true );
		bucket.AddQuadAligned( chunk, pl, x, y, z, gpScale, curDiffuse );
	}
	return true;
} // GPSystem::DrawWSpriteAligned

int	GPSystem::GetNQuads( int seqID, int frameID )
{
	if (seqID		<  0					|| 
		seqID		>= gpHash.numElem()		||
		frameID		>= GPNFrames( seqID )	|| 
		frameID		<  0) 
	{
		return 0;
	}

	GPFrameInst* frameInst = GetFrameInst( seqID, frameID, 0x00000000 );
	if (!frameInst) return 0;
	return frameInst->GetNChunks();
} // GPSystem::GetNQuads

bool GPSystem::GetQuadRect( int seqID, int frameID, int quadID, Rct& rct )
{
	GPFrameInst* frameInst = GetFrameInst( seqID, frameID, 0x00000000 );
	if (!frameInst) return false;
	const GPChunk& chunk = frameInst->GetChunk( quadID );
	rct.x = chunk.x;
	rct.y = chunk.y;
	rct.w = chunk.side;
	rct.h = chunk.side;
	return true;
} // GPSystem::GetQuadRect


const int c_BigPositiveInt = INT_MAX;

/*****************************************************************
/*  GPSystem implementation										 *
/*****************************************************************/
void GPSystem::Drop( GPFrameInst* frInst )
{
	int id = frameHash.find( GPFrameInst::Key( frInst->gpID, frInst->sprID, 
		frInst->color, frInst->lod ) );
	if (id != NO_ELEMENT)
	{
		frameHash.entry( id ).el = NULL;
	}
}

int GPSystem::GPNFrames( int seqID ) const
{
	if (seqID < 0 || seqID >= gpHash.numElem()) return 0x7FFFFFFF;
	const GPSeq* seq = c_GetSeq( seqID );
	if (!seq || (DWORD)seq == c_NoGPFile) return 0x7FFFFFFF;
	return seq->GetNFrames();
} // GPSystem::GPNFrames

bool GPSystem::IsGPLoaded( int seqID ) const
{
	if (seqID < 0 || seqID >= gpHash.numElem()) return false;
	const GPSeq* seq = c_GetSeq( seqID );
	if (!seq || (DWORD)seq == c_NoGPFile) return false;
	return true;
} // GPSystem::IsGPLoaded

const char* GPSystem::GetGPName( int seqID )
{
	static char empty[] = "";
	if (seqID < 0 || seqID >= gpHash.numElem()) return empty;
	return gpHash.entry( seqID ).key;
} // GPSystem::GetGPName

int GPSystem::GetFrameHeight( int seqID, int frameIdx )
{
	if (seqID		<  0					|| 
		seqID		>= gpHash.numElem()		||
		frameIdx	>= GPNFrames( seqID )	|| 
		frameIdx	<  0) 
	{
		return c_BigPositiveInt;
	}
	const GPFrame* frame = c_GetFrame( seqID, frameIdx );
	if (!frame) 
	{
		LoadGP( seqID );
		frame = c_GetFrame( seqID, frameIdx );
		if (!frame) return c_BigPositiveInt;
	}	
	return frame->height;
} // GPSystem::GetFrameHeight

bool GPSystem::GetGPBoundFrame( int seqID, int frameIdx, Rct& frame )
{
	if (seqID < 0 || seqID >= gpHash.numElem() ||
		frameIdx >= GPNFrames( seqID ) || frameIdx < 0) return false;
	const GPFrameInst* frInst = GetFrameInst( seqID, frameIdx, 0 );
	if (!frInst) return false;
	massert( frInst, "GPSystem::GetGPBoundFrame - no frame instance." );
	int left, top;
	if (!frInst->GetLeftTop( left, top )) return false;
	frame.x = left;
	frame.y = top;
	frame.w = GetFrameWidth( seqID, frameIdx ) - frame.x;
	frame.h = GetFrameHeight( seqID, frameIdx ) - frame.y;
	return true;
} // GPSystem::GetGPBoundFrame


GPFrame* GPSystem::GetFrame( int seqID, int sprID )
{
	return c_GetSeq( seqID )->GetFrame( UnswizzleFrameIndex( seqID, sprID ) );
} // GPSystem::GetFrame

const GPFrame* GPSystem::c_GetFrame( int seqID, int sprID ) const
{
	//  lots of crap here...
	GPSeq* seq = (GPSeq*)c_GetSeq( seqID );
	if (!seq || (DWORD)seq == c_NoGPFile) return NULL;
	return seq->GetFrame( UnswizzleFrameIndex( seqID, sprID ) );
} // GPSystem::GetFrame

int GPSystem::GetFrameWidth( int seqID, int frameIdx )
{
	if (seqID		<  0					|| 
		seqID		>= gpHash.numElem()		||
		frameIdx	>= GPNFrames( seqID )	|| 
		frameIdx	<  0) 
	{
		return c_BigPositiveInt;
	}
	const GPFrame* frame = c_GetFrame( seqID, frameIdx );
	if (!frame) 
	{
		LoadGP( seqID );
		frame = c_GetFrame( seqID, frameIdx );
		if (!frame) return c_BigPositiveInt;
	}
	return frame->width;
} // GPSystem::GetFrameWidth

void GPSystem::SetGPPath( int seqID, const char* gpPath )
{
	if (seqID < 0 || seqID >= gpHash.numElem()) 
	{
		Log.Warning( "Incorrect GPID in GPSystem::SetGPPath, seqID:%d, gpPath:<%s>",
			seqID, gpPath );
		return;
	}
	GetSeq( seqID )->SetPath( gpPath );
} // GPSystem::SetGPPath

int GPSystem::GetGPNum() const
{
	return gpHash.numElem();
}

void GPSystem::SetCurrentCamera( Camera* cam )
{
	curCamera = cam;
} // GPSystem::setCurrentCamera

float GPSystem::GetScale() const
{
	return gpScale;
} // GPSystem::GetScale

float GPSystem::SetScale( float scale )
{
	float old = gpScale;
	gpScale = scale;

	if (gpScale > 0.5f)
	{
		curLOD = 0;
	}
	else if (gpScale > 0.25f)
	{
		curLOD = 1;
	}
	else if (gpScale > 0.125f)
	{
		curLOD = 2;
	}
	else
	{
		curLOD = 3;
	}

	return scale;
} // GPSystem::SetScale

void GPSystem::EnableClipping( bool enable )
{
	clippingEnabled = enable;
}

void GPSystem::FlushPrimBucket()
{
	if (lineBucket2D.getNVert() != 0)
	{
		RC::iRS->Draw( lineBucket2D );
		lineBucket2D.setNInd ( 0 );
		lineBucket2D.setNVert( 0 );
		lineBucket2D.setNPri ( 0 );
	}

	if (lineBucket3D.getNVert() != 0)
	{
		RC::iRS->Draw( lineBucket3D );
		lineBucket3D.setNInd ( 0 );
		lineBucket3D.setNVert( 0 );
		lineBucket3D.setNPri ( 0 );
	}

	if (rectBucket2D.getNVert() != 0)
	{
		RC::iRS->Draw( rectBucket2D );
		rectBucket2D.setNInd ( 0 );
		rectBucket2D.setNVert( 0 );
		rectBucket2D.setNPri ( 0 );
	}

	if (rectBucket3D.getNVert() != 0)
	{
		RC::iRS->Draw( rectBucket3D );
		rectBucket3D.setNInd ( 0 );
		rectBucket3D.setNVert( 0 );
		rectBucket3D.setNPri ( 0 );
	}
} // GPSystem::flushBucket

void GPSystem::FlushSpriteBucket( int bucketIdx )
{
	assert( bucketIdx >= 0 && bucketIdx < m_NumSurfaces );
	sprBucket[bucketIdx].Flush();
}

bool GPSystem::SetClipArea( DWORD x, DWORD y, 
						   DWORD w, DWORD h )
{
	if (curViewPort.x == x && curViewPort.y == y &&
		curViewPort.width == w && curViewPort.height == h)
	{
		return false;
	}
	curViewPort.x		= x;
	curViewPort.y		= y;
	curViewPort.width	= w;
	curViewPort.height	= h;
	curViewPort.MinZ	= 0.0f;
	curViewPort.MaxZ	= 1.0f;

	//  viewport is changed, so we need to flush all batched stuff here
	if (RC::iRS) 
	{
		FlushBatches();
		RC::iRS->SetViewPort( curViewPort );
	}
	return true;
} // GPSystem::SetClipArea

const ::ViewPort& GPSystem::GetClipArea() const
{
	return curViewPort;
} // GPSystem::GetClipArea

GPSeq*	GPSystem::GetSeq( int idx ) 
{
	return gpHash.elem( idx );
} // GPSystem::GetSeq

GPSeq*	GPSystem::c_GetSeq( int idx ) const
{
	return gpHash.c_elem( idx );
}

GPFrameInst* GPSystem::GetFrameInstForDrawing( int& gpID, int& sprID, DWORD color, WORD lod )
{
	GPFrameInst* frameInst = NULL; 
	try
	{
		GPSeq* seq = gpHash.elem( gpID );
		if (!seq) LoadGP( gpID );
		seq = gpHash.elem( gpID );
		if (reinterpret_cast<DWORD>( seq ) == c_NoGPFile) return NULL;
		if (!seq) return NULL;
		if (sprID < 0 || sprID >= seq->GetNFrames())
		{
			Log.Warning( "Frame index is outside boundary: gp:%s fr:%d", seq->GetPath(), sprID );
			return NULL;
		}

		sprID = UnswizzleFrameIndex( gpID, sprID );
		frameInst = GetFrameInst( gpID, sprID, color, lod );

		// ensure that pixel data is already in texture memory
		if (!frameInst || !frameInst->Touch()) return NULL;
		frameInst->Hit( curCacheFactor );
	}
	catch (...) 
	{
		const char* gpName = gpHash.entry( gpID ).key.c_str();
		Log.Error( "Error in GPFile: %s, Frame: %d", gpName, sprID );
		return NULL;
	}

	return frameInst;
} // GPSystem::GetFrameInstForDrawing

/*---------------------------------------------------------------------------*/
/*	Func:	GPSystem::UnswizzleFrameIndex
/*	Desc:	Unswizzle frame index in case of "directional indexing"
/*	Parm:	Converts frame indexing from representation where frames are
/*			ordedered like
/*			<dir0:anmFrame0, dir1:anmFrame0, ...> 
/*			<dir0:anmFrame1, dir1:anmFrame1, ...>...
/*			to the order
/*			<dir0:anmFrame0, dir0:anmFrame1, ...> 
/*			<dir1:anmFrame0, dir1:anmFrame1, ...>...
/*			First case is convenient for cossacks engine, second for compression
/*	Ret:	Converted frame index
/*---------------------------------------------------------------------------*/
int GPSystem::UnswizzleFrameIndex( int gpID, int sprID ) const
{
	GPSeq* seq = c_GetSeq( gpID );
	if (!seq) 
	{
		((GPSystem*)this)->LoadGP( gpID );
		seq = c_GetSeq( gpID );
		if (!seq) return sprID;
	}
	assert( seq );
	int nDir = seq->GetNDirections();
	if (nDir > 1)
	{
		int nSpr = seq->GetNFrames();
		int seqLen = nSpr / nDir;
		sprID = sprID / nDir + seqLen * (sprID % nDir);
	}
	return sprID;
} // GPSystem::UnswizzleFrameIndex

GPFrameInst* GPSystem::GetFrameInst( int gpID, int sprID, DWORD color, WORD lod )
{
	//  find given frame reference in hash
	int frameID = frameHash.find( GPFrameInst::Key( gpID, sprID, color, lod ) );
	if (frameID == NO_ELEMENT || frameHash.elem( frameID ) == NULL)
		//  create frame instance
	{
		GPSeq* seq = gpHash.elem( gpID );
		if (!seq) return NULL;
		if (reinterpret_cast<DWORD>( seq ) == c_NoGPFile) return NULL;

		int nChunks	= seq->GetFrame( sprID )->nChunks;
		int nExtra	= nChunks * sizeof( GPChunk );
		GPFrameInst* pInst = alloc.NewInstance( nExtra );
		assert( pInst );

		pInst->SetNChunks( nChunks, (void*)(pInst + 1) );

		if (!pInst) return NULL;

		pInst->gpID		= seq->GetID(); 
		pInst->pFrame	= seq->GetFrame( sprID );
		pInst->color	= color;
		pInst->lod		= lod;
		pInst->pSeq		= seq;

		frameID = frameHash.add( GPFrameInst::Key( gpID, sprID, color, lod ), pInst );
		pInst->InitPrefix();
	}

	return frameHash.elem( frameID );
} // GPSystem::GetFrameInst

void GPSystem::Render()
{
	OnFrame();
}

/*****************************************************************
/*	TexItemLayout implementation
/*****************************************************************/
TexItemLayout::TexItemLayout()
{
	maxLevel = c_MaxQuadLevels;
	texID = 0;
}

void TexItemLayout::Init( int texSide )
{
	maxPixelCapacity = texSide * texSide;
	maxLevel = 0;
	while (texSide > 1) 
	{
		texSide >>= 1;
		maxLevel++;				
	}
	Free();
}

/*****************************************************************
/*	QuadLayoutLevel implementation
/*****************************************************************/
void QuadLayoutLevel::Dump()
{
	Log.Message( "Free chunks: %d. ", numQuads );
	for (int i = 0; i < numQuads; i++)
	{
		Log.Message( "%d %d", quad[i].x, quad[i].y );
	}
}

END_NAMESPACE( sg )
