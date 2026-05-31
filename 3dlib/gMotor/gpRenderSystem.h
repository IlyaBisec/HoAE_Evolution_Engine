/*****************************************************************************/
/*	File:	gpRenderSystem.h
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	23.01.2003
/*****************************************************************************/
#ifndef __GPRENDERSYSTEM_H__
#define __GPRENDERSYSTEM_H__
#pragma once

#include <vector>

/*****************************************************************************/
/*	Enum:	MemoryPool
/*	Desc:	Memory location of resource
/*****************************************************************************/
enum MemoryPool
{
	mpUnknown		= 0,
	mpSysMem		= 1,
	mpVRAM			= 2,
	mpManaged		= 3
};  // enum MemoryPool

/*****************************************************************************/
/*	Enum:	BufferFormat
/*	Desc:	Type of used resource buffer	
/*****************************************************************************/
enum BufferFormat
{
	bfUnknown		= 0,
	bfStatic		= 1,
	bfDynamic		= 2
};	//  enum BufferFormat

/*****************************************************************************/
/*	Enum:	TextureUsage	
/*****************************************************************************/
enum TextureUsage
{
	tuUnknown		= 0,
	tuLoadable		= 1,
	tuProcedural	= 2,
	tuRenderTarget  = 3,
	tuDynamic		= 4,
	tuDepthStencil	= 5
};	// enum TextureUsage

/*****************************************************************************/
/*	Enum:	DeviceCapability
/*****************************************************************************/
enum DeviceCapability
{
	dcUnknown						= 0,
	dcIndexedVertexBlending			= 1,
	dcBumpEnvMap					= 2,
	dcBumpEnvMapLuminance			= 3,
	dcDynamicTextures				= 4,
	dcTnL							= 5,
	dcPure							= 6,
	dcRasterization					= 7,
	dcBezier						= 8,
	dcRTPatches						= 9,
	dcNPatches						= 10,
	dcREF							= 11
};	// enum DeviceCapability

/*****************************************************************************/
/*	Enum: DepthStencilFormat
/*****************************************************************************/
enum DepthStencilFormat
{
	dsfUnknown			= 0,
	dsfD16Lockable		= 1,
	dsfD32				= 2,
	dsfD15S1			= 3,
	dsfD24S8			= 4,
	dsfD16				= 5
}; // DepthStencilFormat

class TextureDescr;
/*****************************************************************************/
/*	Class:	TextureClient
/*	Desc:	User of the texture
/*****************************************************************************/
class TextureClient
{
public:
	virtual void Invalidate( TextureDescr* texture ) = 0;
	virtual void Restore( TextureDescr* texture ) = 0;

}; // class TextureClient

/*****************************************************************************/
/*	Class:	TextureDescr
/*	Desc:	Texture description class
/*****************************************************************************/
class TextureDescr
{
public:
	TextureDescr();
	
	bool equal( const TextureDescr& td );
	bool less( const TextureDescr& td );
	void copy( const TextureDescr& orig );
	
	void			setValues( int sx, int sy, 
								ColorFormat		cf		= cfARGB4444,
								MemoryPool		mp		= mpVRAM, 
								int				nmips	= 1,
								TextureUsage	tu		= tuLoadable );

	int				getSideX()		const { return sideX;					}
	int				getSideY()		const { return sideY;					}
	ColorFormat		getColFmt()		const { return (ColorFormat)colFmt;		}
	DepthStencilFormat getDsFmt()	const { return (DepthStencilFormat)dsFormat; }

	MemoryPool		getMemPool()	const { return (MemoryPool)memPool;		}
	TextureUsage	getTexUsage()	const { return (TextureUsage)texUsage;	}
	int				getNMips()		const { return numMips;					}
	TextureClient*	getClient()		const { return client;					}
	const char*		getPoolStr()	const;
	const char*		getColFmtStr()	const;
	const char*		getUsageStr()	const;
	int				getID()			const { return id; }

	void			setSideX	( int sx			)	{ sideX		= sx;		}
	void			setSideY	( int sy			)	{ sideY		= sy;		}
	void			setColFmt	( ColorFormat cf	)	{ colFmt	= (BYTE)cf; }
	void			setDsFmt	( DepthStencilFormat dsf	)	{ dsFormat	= (BYTE)dsf; }
	void			setMemPool	( MemoryPool mp		)	{ memPool	= (BYTE)mp;	}
	void			setTexUsage	( TextureUsage tu	)	{ texUsage	= (BYTE)tu; }
	void			setNMips	( int n				)	{ numMips	= n;		}
	void			setClient	( TextureClient* cl )	{ client	= cl;		}
	void			setID		( int _id			)	{ id		= _id;		}
	
	void			invalidate	() 
	{ 
		valid = false; 
		if (client) client->Invalidate( this ); 
	}

protected:
	int				sideX;		//  width
	int				sideY;		//  height

	BYTE			colFmt;		//  color format
	BYTE			memPool;	//  memory pool
	BYTE			texUsage;	//  usage 
	BYTE			dsFormat;	//  depth stencil format if texture is depth buffer
	
	int				numMips;	//  number of mip levels

	//  runtime properties
	int				id;
	bool			valid;
	TextureClient*	client;
}; // class TextureDescr

/*****************************************************************************/
/*	Class:	ScreenProp
/*	Desc:	Describes screen properties
/*****************************************************************************/
class ScreenProp
{
public:
	ScreenProp();

	int				width;
	int				height;
	ColorFormat		colorFormat;
	bool			fullScreen;
	bool			wholeScreenWnd;
	int				refreshRate;

	bool equal( const ScreenProp& prop ) const;

}; // class ScreenProp

/*****************************************************************************/
/*	Class:	ViewPort
/*	Desc:	Rendering viewport	
/*****************************************************************************/
class ViewPort
{
public:
    DWORD       x;
	DWORD       y;
    DWORD       width;
    DWORD       height;
    
	float       MinZ;
    float       MaxZ;

	_inl bool	ClipHLine	( int& px1, int& px2, int py ) const;
	_inl bool	ClipVLine	( int px,	int& py1, int& py2 ) const;
	_inl bool	ClipRect	( int& px,	int& py, int& pw, int& ph ) const;
	_inl bool	IsPtInside	( int px,	int py ) const;
	_inl bool	IsRectInside( int px,	int py, int pw, int ph ) const;
	_inl bool	ClipY		( int py ) const;
	_inl DWORD	GetRight	() const;
	_inl DWORD	GetBottom	() const;

}; // class ViewPort

/*****************************************************************************/
/*	Class:	DeviceDescr
/*	Desc:	3D device capabilities
/*****************************************************************************/
class DeviceDescr
{
public:
	DeviceDescr();

	int				adapterOrdinal;
	c2::string		devDriver;
	c2::string		devDescr;
	
	int				texBlendStages;
	int				texInSinglePass;

	std::vector<DeviceCapability>		capBits;
	std::vector<DepthStencilFormat>	depthStencil;
	std::vector<ColorFormat>			renderTarget;	
}; // class DeviceDescr

//  primitive drawing helper utilities
class Rct;
class Vector3D;

void rsLine			( float x1, float y1, float x2, float y2, float z, DWORD color );
void rsLine			( float x1, float y1, float x2, float y2, float z, DWORD color1, DWORD color2 );
void rsFrame		( const Rct& rct, float z, DWORD color );
void rsFlushLines2D	();

void rsLine			( const Vector3D& a, const Vector3D& b, DWORD color );
void rsLine			( const Vector3D& a, const Vector3D& b, DWORD color1, DWORD color2 );
void rsFlushLines3D	();
void rsEnableZ		( bool enable = true );

void rsRect			( const Rct& rct, float z, DWORD color );
void rsRect			( const Rct& rct, float z, DWORD ca, DWORD cb, DWORD cc, DWORD cd );
void rsPanel		(  const Rct& rct, float z, 
						DWORD clrTop = 0xFFFFFFFF, 
						DWORD clrMdl = 0xFFD6D3CE, 
						DWORD clrBot = 0xFF848284 );
void rsFlushPoly2D	();

void rsPoly			( const Vector3D& a, const Vector3D& b, const Vector3D& c, 
						DWORD acol, DWORD bcol, DWORD ccol );
void rsPoly			( const Vector3D& a, const Vector3D& b, const Vector3D& c, DWORD color );

void rsQuad			( const Vector3D& a, const Vector3D& b, const Vector3D& c, const Vector3D& d, 
					 DWORD acol, DWORD bcol, DWORD ccol, DWORD dcol );
void rsQuad			( const Vector3D& a, const Vector3D& b, const Vector3D& c, const Vector3D& d, DWORD color );
void rsFlushPoly3D	();


//  Drawing different useful primitives
const int		c_DefaultSphereSegments = 8;
const float		c_DefRayLen				= 10000.0f; 
const float		c_DefHandleSide			= 5.0f;
const float		c_DefQuadSide			= 30.0f;
const float		c_DefNormalLen			= 8.0f;

class Matrix4D;

class Frustum;
class Plane;
class Line3D;
class Triangle;
class Sphere;
class Cylinder;
class Capsule;
class Cone;
class AABoundBox;

void DrawFrustum	( const Frustum& frustum, 
					 DWORD fillColor, DWORD linesColor, 
					 bool drawNormals = false );
void DrawCube		( const Vector3D& center, DWORD color, float side = c_DefHandleSide );
void DrawRay		( const Line3D& ray, DWORD linesColor, 
					 float rayLen = c_DefRayLen, 
					 float handleSide = c_DefHandleSide );

void DrawPlane		( const Plane& plane, DWORD fillColor, DWORD color,
					 const Vector3D* center = NULL,
					 float qSide = c_DefQuadSide, 
					 float nLen = c_DefNormalLen );

void DrawTriangle	( const Triangle& tri, DWORD linesColor, DWORD fillColor );
void DrawAABB		( const AABoundBox& aabb, DWORD fillColor, DWORD linesColor );
void DrawSphere		( const Sphere& sphere, 
					 DWORD fillColor, DWORD linesColor, 
					 int nSegments = c_DefaultSphereSegments );
void DrawCircle		( const Vector3D& center, const Vector3D& normal, float radius, 
					 DWORD fillColor, DWORD linesColor, int nSegments );
void DrawCircle8	( const Vector3D& center, const Vector3D& normal, float radius, 
					 DWORD fillColor, DWORD linesColor );

void DrawSpherePatch( const Sphere& sphere, 
					 DWORD fillColor, DWORD linesColor, 
					 float phiBeg, float phiEnd,
					 float thetaBeg, float thetaEnd,
					 int nSegments, const Matrix4D* pTM = NULL );
void DrawStar		( const Sphere& sphere, DWORD begColor, DWORD endColor, 
					 int nSegments = c_DefaultSphereSegments );
void DrawCylinder	( const Cylinder& cylinder, 
					 DWORD fillColor, DWORD linesColor, bool bCapped,
					 int nSegments = c_DefaultSphereSegments );
void DrawCapsule	( const Capsule& capsule, 
					 DWORD fillColor, DWORD linesColor, 
					 int nSegments = c_DefaultSphereSegments );

void DrawCone		( const Cone& cone, 
					 DWORD fillColor, DWORD linesColor, 
					 int nSegments = c_DefaultSphereSegments );

void DrawFatSegment( const Vector3D& beg, const Vector3D& end, const Vector3D& normal, 
						float width, bool bRoundEnds, DWORD color, DWORD coreColor = 0 );


#ifdef _INLINES
#include "gpRenderSystem.inl"
#endif // _INLINES

#endif // __GPRENDERSYSTEM_H__