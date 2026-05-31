/*****************************************************************/
/*  File:   sgGPSystem.h
/*  Desc:   Sprite manager
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#ifndef __SGGPSYSTEM_H__
#define __SGGPSYSTEM_H__

#include "gmDefines.h"
#include "kResFile.h"
#include "kCache.h"
#include "kHash.hpp"
#include "kResource.h"
#include "kPoolAllocator.h"
#include "gpPackage.h"
#include "gpBucket.h"
#include "gpGN16.h"
#include "gpGU16.h"
#include "gpBitmap.h"
#include "IRenderSystem.h"
#include "IGPSystem.h"

namespace sg{

const int	c_MaxGPSeqs					= 4096;
const int	c_GPFrameInstHashTableSize	= 104729;
const int	c_GPFrameInstHashPoolSize	= 16384;
const int	c_GPHashTableSize			= 7919;

class Group;

class GPSeq;
class GPFont;
class GPChunk;
class GPSystem;

typedef PointerHash<	GPFrameInst, 
GPFrameInst::Key, 
c_GPFrameInstHashTableSize, 
c_GPFrameInstHashPoolSize>		GPFrameHash;
typedef PointerHash<GPSeq, c2::string, c_GPHashTableSize, c_MaxGPSeqs> GPPackageHash;
const DWORD	c_NoGPFile = 0xbaadf00d;

/*****************************************************************/
/*  Class:	GPPackage
/*  Desc:	Sprite package file
/*****************************************************************/
class GPPackage : public Node
{
public:
	
	MAGIC(GPPackage, Node, GPAK);
}; // class GPPackage

/*****************************************************************/
/*  Class:	GPSystem
/*  Desc:	Sprite manager
/*****************************************************************/
class GPSystem : public Node, public IGPSystem
{
public:
							GPSystem	();
	virtual					~GPSystem	();
	
	virtual void			Render		(); 

	virtual void Init();
	//  sprite drawing functions
	virtual	bool DrawSprite				( int gpID, int sprID, float x, float y, float z, bool mirrored, 
		DWORD color = 0x00000000 );
	virtual	bool DrawSpriteTransformed	( int gpID, int sprID, const Matrix4D& transf, 
		DWORD color = 0x00000000 );
	virtual	bool DrawSpriteAligned		( int gpID, int sprID, const Plane& pl, float x, float y, float z, 
		bool mirrored, DWORD color = 0x00000000 );
	virtual	bool DrawWSprite			( int gpID, int sprID, float x, float y, float z, bool mirrored, 
		DWORD color = 0x00000000 );
	virtual	bool DrawWSpriteTransformed	( int gpID, int sprID, const Matrix4D& transf, 
		DWORD color = 0x00000000 );
	virtual	bool DrawWChunk				( int gpID, int sprID, int chunkID, const Vector3D& pos, BaseMesh& geom, 
		int cPoly, int nPoly, int cVert, int nVert, DWORD color = 0 );
	virtual	bool DrawWSpriteAligned		( int gpID, int sprID, const Plane& pl, float x, float y, float z,
		bool mirrored, DWORD color = 0x00000000 );
	virtual	bool DrawWSprite			( int gpID, int sprID, const Vector3D& pos, BaseMesh& geom, DWORD color = 0x0 );
	virtual	void DrawLine				( float x1, float y1, float x2, float y2, DWORD color );
	virtual	void DrawLine				( const Vector3D& beg, const Vector3D& end, DWORD color );
	virtual	void DrawFillRect			( float x, float y, float w, float h, DWORD color );
	virtual	void DrawRect				( float x, float y, float w, float h, DWORD color );
	virtual	void DrawFillRect			( const Vector3D& lt, const Vector3D& rt, const Vector3D& lb, const Vector3D& rb, 
		DWORD color );
	
	virtual	void DrawFillRect			( const Vector3D& lt, const Vector3D& rt, const Vector3D& lb, const Vector3D& rb, 
											DWORD clt, DWORD crt, DWORD clb, DWORD crb );

	virtual	void DrawString				( const char* str, float x, float y, float z, 
		DWORD color = 0xFFFFFFFF, float height = -1.0f );

	//  storage manipulation
	virtual	int  PreLoadGPImage			( const char* gpName );
	virtual	bool LoadGP					( int seqID, const char* gpPath = NULL );
	virtual	void UnLoadGP				( int seqID );
	virtual bool IsGPLoaded				( int seqID ) const;

	virtual	void SetGPPath				( int seqID, const char* gpPath );

	//  drawing manipulation
	virtual	void	EnableClipping		( bool enable = true );
	virtual	void	SetTexCoordBias		( float bias = 0.0f );
	virtual	float	SetScale			( float scale );
	virtual	float	GetScale			() const;	
	virtual	bool	SetClipArea			( DWORD x, DWORD y, DWORD w, DWORD h );
	virtual	const ::ViewPort& GetClipArea() const;
	virtual	int		GetFrameHeight		( int seqID, int frameIdx );
	virtual	int		GetFrameWidth		( int seqID, int frameIdx );
	virtual	int		GPNFrames			( int seqID ) const;
	virtual	int		GetGPNum			() const;
	virtual	const char*	GetGPName		( int seqID );
	virtual	const char*	GetGPPath		( int seqID );
	virtual	void	SetCurrentZ			( float z );
	virtual	void	SetCurrentDiffuse	( DWORD color );
	virtual	float	GetCurrentZ			() const;
	virtual	DWORD	GetCurrentDiffuse	() const;
	virtual	int		SetCurrentShader	( int shID );
	virtual	int		GetCurrentShader	() const;
	virtual	bool 	GetGPBoundFrame		( int seqID, int frameIdx, Rct& frame );
	virtual	void 	SetCurrentCamera	( Camera* cam );
	virtual	void 	FlushPrimBucket		();
	virtual	void 	FlushSpriteBucket	( int bucketIdx );
	virtual	void 	OnFrame				();
	virtual	void 	FlushBatches		();
	virtual	void 	Dump				();
	virtual	void 	EnableZBuffer		( bool enable = true );
	virtual	DWORD	GetGPAlpha			(	int seqID, int frameIdx, int ptX, int ptY, bool precise = false );
	virtual	int		GetNQuads			( int seqID, int frameID );
	virtual	bool	GetQuadRect			( int seqID, int frameID, int quadID, Rct& rct );
	virtual	float	GetAvgAlpha			( int seqID, int frameID, const Triangle2D& tri );
	virtual	BYTE	GetMaxAlpha			( int seqID, int frameID, const Triangle2D& tri );
	virtual	void	SetRootDirectory	( const char* rdir );
	
	static GPSystem*		instance() 
	{
		if (s_pInstance) return s_pInstance;
		s_pInstance = new GPSystem();
		s_pInstance->Init();
		return s_pInstance;
	}

	void			SetNumSurfaces		( int val );
	virtual void	Expose				( PropertyMap& pmap );

	MAGIC(GPSystem,Node,GPSY);

protected:
	void					DrawBuckets();
	void					CleanBuckets();

	void 					Drop					( GPFrameInst* frInst );
	_inl GPSeq*				GetSeq					( int idx ); 
	_inl GPSeq*				c_GetSeq				( int idx ) const; 
	_inl GPFrameInst*		GetFrameInstForDrawing	( int& seqID, int& sprID, DWORD color, WORD lod = 0 );
	_inl GPFrameInst*		GetFrameInst			( int seqID, int sprID, DWORD color, WORD lod = 0 );
	_inl int				UnswizzleFrameIndex		( int seqID, int sprID ) const;
	_inl GPFrame*			GetFrame				( int seqID, int sprID );
	_inl const GPFrame*		c_GetFrame				( int seqID, int sprID ) const;

	int						GetNumSurfaces			() const { return m_NumSurfaces; }
	
private:
	
	static GPSystem*		s_pInstance;

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

	int						m_NumSurfaces;
	float					m_UVBias;
	int						m_NumWSpritesDrawn;
	
	//  sprite drawing buckets
	std::vector<GPBucket>	sprBucket;

	BaseMesh				rectBucket2D;
	BaseMesh				lineBucket2D;
	BaseMesh				rectBucket3D;
	BaseMesh				lineBucket3D;
	
	GPFont*					debugFont;
	
	::ViewPort				curViewPort;
	char					m_RootDirectory[_MAX_PATH];

	Group*					m_pSurfaces;
	Group*					m_pPackages;
	Group*					m_pShaders;

public:
	static int				curCacheFactor;

	friend class			GPPixelCache;
};  // class GPSystem

}; // namespace sg

#endif // __SGGPSYSTEM_H__