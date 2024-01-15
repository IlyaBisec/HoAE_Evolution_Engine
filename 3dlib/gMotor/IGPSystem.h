/*****************************************************************/
/*  File:   IGPSystem.h
/*  Desc:   Sprite manager abstract interface
/*  Desc:   3D rendering device abstract interface               
/*  Date:   Jan 2004											 
/*****************************************************************/
#ifndef __IGPSYSTEM_H__
#define __IGPSYSTEM_H__

class Matrix4D;
class Plane;
class BaseMesh;
class Vector3D;
class Rct;
class Triangle2D;

const DWORD c_InsideChunks				= 0xFFFFFFFE;
const DWORD c_OutsideChunks				= 0x0;

/*****************************************************************/
/*  Class:	IGPSystem
/*  Desc:	Sprite manager abstract interface
/*****************************************************************/
class IGPSystem 
{
public:
	//  initializing system
	virtual void Init() = 0;
	//  sprite drawing functions
	virtual	bool DrawSprite				( int gpID, int sprID, float x, float y, float z, bool mirrored, 
											DWORD color = 0x00000000 ) = 0;
	virtual	bool DrawSpriteTransformed	( int gpID, int sprID, const Matrix4D& transf, 
											DWORD color = 0x00000000 ) = 0;
	virtual	bool DrawSpriteAligned		( int gpID, int sprID, const Plane& pl, float x, float y, float z, 
											bool mirrored, DWORD color = 0x00000000 ) = 0;
	virtual	bool DrawWSprite			( int gpID, int sprID, float x, float y, float z, bool mirrored, 
											DWORD color = 0x00000000 ) = 0;
	virtual	bool DrawWSpriteTransformed	( int gpID, int sprID, const Matrix4D& transf, 
											DWORD color = 0x00000000 ) = 0;
	virtual	bool DrawWChunk				( int gpID, int sprID, int chunkID, const Vector3D& pos, BaseMesh& geom, 
											int cPoly, int nPoly, int cVert, int nVert, DWORD color = 0 ) = 0;
	virtual	bool DrawWSpriteAligned		( int gpID, int sprID, const Plane& pl, float x, float y, float z,
											bool mirrored, DWORD color = 0x00000000 ) = 0;
	virtual	bool DrawWSprite			( int gpID, int sprID, const Vector3D& pos, BaseMesh& geom, DWORD color = 0x0 ) = 0;
	virtual	void DrawLine				( float x1, float y1, float x2, float y2, DWORD color ) = 0;
	virtual	void DrawLine				( const Vector3D& beg, const Vector3D& end, DWORD color ) = 0;
	virtual	void DrawFillRect			( float x, float y, float w, float h, DWORD color ) = 0;
	virtual	void DrawRect				( float x, float y, float w, float h, DWORD color ) = 0;
	virtual	void DrawFillRect			( const Vector3D& lt, const Vector3D& rt, const Vector3D& lb, const Vector3D& rb, 
											DWORD color ) = 0;
	virtual	void DrawFillRect			( const Vector3D& lt, const Vector3D& rt, const Vector3D& lb, const Vector3D& rb, 
											DWORD clt, DWORD crt, DWORD clb, DWORD crb ) = 0;

	virtual	void DrawString				( const char* str, float x, float y, float z, 
											DWORD color = 0xFFFFFFFF, float height = -1.0f ) = 0;
	//  storage manipulation
	virtual	int		PreLoadGPImage		( const char* gpName ) = 0;
	virtual	bool	LoadGP				( int seqID, const char* gpPath = NULL ) = 0;
	virtual	void	UnLoadGP			( int seqID ) = 0;
	virtual bool	IsGPLoaded			( int seqID ) const = 0;

	virtual	void	SetGPPath			( int seqID, const char* gpPath ) = 0;
		
	//  drawing manipulation
	virtual	void	EnableClipping		( bool enable = true ) = 0;
	virtual	void	SetTexCoordBias		( float bias = 0.0f ) = 0;
	virtual	float	SetScale			( float scale ) = 0;
	virtual	float	GetScale			() const = 0;	
	virtual	bool	SetClipArea			( DWORD x, DWORD y, DWORD w, DWORD h ) = 0;
	virtual	const ::ViewPort&	GetClipArea	() const = 0;
	
	virtual	int		GetFrameHeight		( int seqID, int frameIdx ) = 0;
	virtual	int		GetFrameWidth		( int seqID, int frameIdx ) = 0;

	virtual	int		GPNFrames			( int seqID ) const = 0;
	virtual	int		GetGPNum			() const = 0;
	virtual	const char*	GetGPName		( int seqID ) = 0;
	virtual	const char*	GetGPPath		( int seqID ) = 0;
	virtual	void	SetCurrentZ			( float z ) = 0;
	virtual	void	SetCurrentDiffuse	( DWORD color ) = 0;
	virtual	float	GetCurrentZ			() const = 0;
	virtual	DWORD	GetCurrentDiffuse	() const = 0;
	virtual	int		SetCurrentShader	( int shID ) = 0;
	virtual	int		GetCurrentShader	() const = 0;
	virtual	bool 	GetGPBoundFrame		( int seqID, int frameIdx, Rct& frame ) = 0;
	virtual	void 	FlushPrimBucket		() = 0;
	virtual	void 	FlushSpriteBucket	( int bucketIdx ) = 0;
	virtual	void 	OnFrame				() = 0;
	virtual	void 	FlushBatches		() = 0;
	virtual	void 	Dump				() = 0;
	virtual	void 	EnableZBuffer		( bool enable = true ) = 0;
	virtual	DWORD	GetGPAlpha			(	int seqID, int frameIdx, int ptX, int ptY, bool precise = false ) = 0;
	virtual	int		GetNQuads			( int seqID, int frameID ) = 0;
	virtual	bool	GetQuadRect			( int seqID, int frameID, int quadID, Rct& rct ) = 0;
	virtual	float	GetAvgAlpha			( int seqID, int frameID, const Triangle2D& tri ) = 0;
	virtual	BYTE	GetMaxAlpha			( int seqID, int frameID, const Triangle2D& tri ) = 0;
	virtual	void	SetRootDirectory	( const char* rdir ) = 0;
}; // IGPSystem

IGPSystem* GetGPSystemInterface();

#endif // __IGPSYSTEM_H__