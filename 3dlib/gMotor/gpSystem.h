/*****************************************************************/
/*  File:   GPSystem.h
/*  Desc:   Sprite manager
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#ifndef __GPSYSTEM_H__
#define __GPSYSTEM_H__

#ifndef GP_USER
#define GP_API __declspec(dllexport)
#else
#define GP_API __declspec(dllimport)
#endif

#pragma warning ( disable : 4786 )
#include "gmDefines.h"

#include "kResFile.h"
#include "kCache.h"
#include "kHash.hpp"
#include "kResource.h"
#include "kPoolAllocator.h"

#include "IRenderSystem.h"

#include "gpPackage.h"
#include "gpBucket.h"

#include "gpGN16.h"
#include "gpGU16.h"

#include "gpBitmap.h"

const int c_MaxGPSeqs				= 4096;

const DWORD c_InsideChunks			= 0xFFFFFFFE;
const DWORD c_OutsideChunks			= 0x0;

const int c_GPFrameInstHashTableSize	= 104729;
const int c_GPFrameInstHashPoolSize		= 16384;
const int c_GPHashTableSize			= 7919;

class GPSeq;
class GPChunk;
class GPSystem;

typedef PointerHash<	GPFrameInst, 
						GPFrameInst::Key, 
						c_GPFrameInstHashTableSize, 
						c_GPFrameInstHashPoolSize>		GPFrameHash;
typedef PointerHash<GPSeq, c2::string, c_GPHashTableSize, c_MaxGPSeqs> GPPackageHash;
		
class ComplexBitmap;
class SoftwareVS;
class GPFont;
class Triangle2D;

const DWORD	c_NoGPFile = 0xbaadf00d;

/*****************************************************************/
/*  Class:	GPSystem
/*  Desc:	Sprite manager
/*****************************************************************/
class GPSystem 
{
public:
							GPSystem();
	virtual					~GPSystem();

	//  initializing system
	virtual void			Init( IRenderSystem* irs );

	//  generic sprite drawing function
	bool					DrawSprite(	int	gpID, int sprID,
										float x, float y, float	z,
										bool mirrored, 
										DWORD color = 0x00000000 );
	
	bool					DrawSpriteTransformed(	int gpID, int sprID, 
										const Matrix4D& transf, 
										DWORD color = 0x00000000 );

	bool					DrawSpriteAligned(	int gpID, int sprID, 
												const Plane& pl, 
												float x, float y, float	z,
												bool mirrored,
												DWORD color = 0x00000000 );


	bool					DrawWSprite(	int	gpID, int sprID,
											float x, float y, float	z,
											bool mirrored, 
											DWORD color = 0x00000000 );

	bool					DrawWSpriteTransformed(	int gpID, int sprID, 
													const Matrix4D& transf, 
													DWORD color = 0x00000000 );

	bool					DrawWChunk( int gpID, int sprID, int chunkID, 
										const Vector3D& pos, 
										BaseMesh& geom, 
										int cPoly, int nPoly, 
										int cVert, int nVert, DWORD color = 0 );

	bool					DrawWSpriteAligned(	int gpID, int sprID, 
												const Plane& pl, 
												float x, float y, float	z,
												bool mirrored,
												DWORD color = 0x00000000 );

	bool					DrawWSprite( int gpID, int sprID, const Vector3D& pos, 
											BaseMesh& geom, DWORD color = 0x0 );

	bool					DrawSpriteVS( int gpID, int sprID, DWORD color, const SoftwareVS& vs );
	

	void					DrawLine	( float x1, float y1, float x2, float y2, DWORD color );
	void					DrawLine	( const Vector3D& beg, const Vector3D& end, DWORD color );
	void					DrawFillRect( float x, float y, float w, float h, DWORD color );
	void					DrawRect( float x, float y, float w, float h, DWORD color );
	
	void					DrawFillRect( const Vector3D& lt, const Vector3D& rt, 
											const Vector3D& lb, const Vector3D& rb, 
											DWORD color );

	void					DrawString( const char* str, 
										float x, float y, float z,
										DWORD color		= 0xFFFFFFFF,	//  color of text
										float height	= -1.0f			//  height of text, in pixels
										);

	//  package loading / unloading
	int						PreLoadGPImage	( const char* gpName );

	
	bool					LoadGP			( int seqID, const char* gpPath = NULL );
	void					UnLoadGP		( int seqID );
	
	_inl void				SetGPPath		( int seqID, const char* gpPath );
	
	virtual	const char*		GetGPPath		( int seqID );
	_inl int				SetCurShader	( int shID );
	_inl void				EnableClipping  ( bool enable = true );
	_inl void				SetTexCoordBias	( float bias = 0.0f ) { GPBucket::s_TexCoordBias = bias; }


	_inl float				SetScale		( float scale );
	_inl float				GetScale		() const;	
	_inl bool				SetClipArea		( DWORD x, DWORD y, DWORD w, DWORD h );
	_inl const ViewPort&	GetClipArea		() const;
	
	//  sprite attributes getters
	_inl int				GetGPHeight		( int seqID, int frameIdx );
	_inl int				GetGPWidth		( int seqID, int frameIdx );

	_inl int				GPNFrames		( int seqID )								const;
	_inl int				GetGPNum		()											const;
	_inl const char*		GetGPName	( int seqID );

	_inl void				SetSpritePivot	(	int gpID, int sprID, 
												int centerX, int centerY )				const;

	_inl void				SetCurrentZ		( float z );
	_inl void				SetCurrentDiffuse( DWORD color );

	_inl float				GetCurrentZ		()											const;
	_inl DWORD				GetCurrentDiffuse()											const;

	_inl int				SetCurrentShader( int shID );
	_inl int				GetCurrentShader()											const;


	_inl bool				GetGPBoundFrame	( int seqID, int frameIdx, Rct& frame );
	
	_inl void				SetCurrentCamera( Camera* cam );
	_inl void				FlushPrimBucket();
	_inl void				FlushSpriteBucket( int bucketIdx );

	_inl void				Drop( GPFrameInst* frInst );

	void					OnFrame();
	void					FlushBatches();

	void					Dump();

	void					EnableZBuffer	( bool enable = true );
	DWORD					GetGPAlpha		(	int seqID, int frameIdx, 
												int ptX, int ptY, 
												bool precise = false );

	int						GetNQuads		( int seqID, int frameID );
	bool					GetQuadRect		( int seqID, int frameID, int quadID, Rct& rct );

	float					GetAvgAlpha		( int seqID, int frameID, const Triangle2D& tri );
	BYTE					GetMaxAlpha		( int seqID, int frameID, const Triangle2D& tri );

	_inl static int			GetNumTextureCacheSurfaces(); 

	void					SetRootDirectory( const char* rdir ) { strcpy( rootDirectory, rdir ); }

protected:
	void					DrawBuckets();
	void					CleanBuckets();

	_inl GPSeq*				GetSeq					( int idx ); 
	_inl GPSeq*				c_GetSeq				( int idx ) const; 
	_inl GPFrameInst*		GetFrameInstForDrawing	( int& seqID, int& sprID, DWORD color, WORD lod = 0 );
	_inl GPFrameInst*		GetFrameInst			( int seqID, int sprID, DWORD color, WORD lod = 0 );
	_inl int				UnswizzleFrameIndex		( int seqID, int sprID ) const;
	_inl GPFrame*			GetFrame				( int seqID, int sprID );
	_inl const GPFrame*		c_GetFrame				( int seqID, int sprID ) const;


private:

	void					dbgDrawBucketZs( int bucketIdx );
	GPFrameHash				frameHash;
	GPPackageHash			gpHash;

	TypedPoolAllocator<GPFrameInst>			
							alloc;

	float					gpScale;
	WORD					curLOD;

	float					curZ;
	DWORD					curDiffuse;

	
	Camera*					curCamera;
	int						curBucket;
	bool					clippingEnabled;
	bool					isInited;
	
	//  sprite drawing buckets
	c2::array<GPBucket>		sprBucket;

	BaseMesh				rectBucket2D;
	BaseMesh				lineBucket2D;
	BaseMesh				rectBucket3D;
	BaseMesh				lineBucket3D;
	
	GPFont*					debugFont;
	
	ViewPort				curViewPort;
	static int				s_GPTexNum;

	char					rootDirectory[_MAX_PATH];

public:
	static int				curCacheFactor;

	friend class			GPPixelCache;
};  // class GPSystem

/*****************************************************************************/
/*	Class:	GP 
/*	Desc:	Nifty GPSystem initializer hack class
/*****************************************************************************/
class GP 
{
public:
	static GPSystem* pGP()
	{
		if (s_pInstance) return s_pInstance;
		return GetGPLocal();
	}

	static void SetInstance( GPSystem* pGpSys )
	{
		s_pInstance = pGpSys;
	}

private:
	GP(){}
	virtual ~GP(){}
	
	static GPSystem* GetGPLocal()
	{
		static bool _gp_inited = false;
		static GPSystem gps;
		if (!_gp_inited && RC::iRS)
		{
			gps.Init( RC::iRS );
			gps.EnableClipping( false );
			gps.SetCurrentShader( RC::iRS->GetShaderID( "hud" ) );
			//gps.SetCurrentShader( 0 );
			_gp_inited = true;
		}
		return &gps;
	}
	static GPSystem* s_pInstance;

}; // class GP

#include "gpSystem.hpp"

#ifdef _INLINES
#include "gpSystem.inl"
#endif // _INLINES


#endif // __GPSYSTEM_H__