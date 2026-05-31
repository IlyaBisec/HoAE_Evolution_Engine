/*****************************************************************************/
/*	File:	akScape.h
/*	Desc:	Simple terrain renderer
/*	Author:	Ruslan Shestopalyuk
/*	Date:	06-11-2003
/*****************************************************************************/
#ifndef __AKSCAPE_H__
#define __AKSCAPE_H__

const int		c_QuadHorzTris		= 16;

const float		c_WorldGridStepX		= 32.0f;
const float		c_WorldGridStepY		= 32.0f;
const float		c_HalfWorldGridStepY	= c_WorldGridStepY / 2.0f;  

const int		c_HmapGridStepX			= 32;
const int		c_HmapGridStepY			= 32;
const int		c_HalfHmapGridStepY		= c_HmapGridStepY / 2;

const float		c_HmapToWorldX			= c_WorldGridStepX / (float)c_HmapGridStepX;
const float		c_HmapToWorldY			= c_WorldGridStepY / (float)c_HmapGridStepY;

const int		c_QuadSide			= c_QuadHorzTris * c_HmapGridStepX;
const int		c_2QuadSide			= c_QuadSide * 2;

const int		c_QuadVert	= (c_QuadHorzTris + 1) * (c_QuadHorzTris + 1);
const int		c_QuadInd	= c_QuadHorzTris * c_QuadHorzTris * 6; 

const float		c_Cos30 = 0.86602540378443864676372317075294f;
const float		c_InvCos30 = 1.15470053837925152901f;
const float		c_Tan30 = 0.57735026918962576450914878050196f;

const float		c_CameraDistance = 3000.0f;
const float		c_MaxMapHeight	= 1024.0f;
const float		c_BottomFieldExtent = c_MaxMapHeight / c_Tan30;

#pragma pack( push )
#pragma pack( 8 )

extern IRenderSystem*	IRS;
extern int RealLx, RealLy, mapx, mapy;
extern short* THMap;
int GetHeight(int x,int y);

float GetGround_d2x( int x, int y );

enum ShadowMode
{
	shmNoShadow		= 0,
	shmShadowPatch	= 1,
	shmShadowVolume	= 2,
	shmSimpleShadow	= 3
}; // enum ShadowMode

/*****************************************************************************/
/*	Class:	TRect
/*	Desc:	Rectangle
/*****************************************************************************/
template <class T>
class TRect
{
public:
	T				x;
	T				y;
	T				x1;
	T				y1;

	TRect() : x(0), y(0), x1(0), y1(0) {}
	TRect( T _x, T _y, T _x1, T _y1 ) : x(_x), y(_y), x1(_x1), y1(_y1) {}
	TRect( const TRect& rct ) { x = rct.x; y = rct.y; x1 = rct.x1; y1 = rct.y1; } 

	bool			degenerate	()
	{
		return (x > x1 || y > y1);
	}

	bool			intersect	( const TRect& a, const TRect& b )
	{
		x	= max( a.x,		b.x  );
		y	= max( a.y,		b.y  );
		x1	= min( a.x1,	b.x1 );
		y1	= min( a.y1,	b.y1 );
		if (degenerate()) return false;
		return true;
	}
	T				sqrArea		() const
	{
		return (x1 - x) * (y1 - y);
	}
	void			add			( const TRect& a, const TRect& b )
	{
		x	= min( a.x,		b.x  );
		y	= min( a.y,		b.y  );
		x1	= max( a.x1,	b.x1 );
		y1	= max( a.y1,	b.y1 );
	}
	bool			inside		( const TRect& a )
	{
		return ( x >= a.x && y >= a.y && x1 <= a.x1 && y1 <= a.y1 );
	}
}; // class TRect

typedef TRect<int>	NRect;

/*****************************************************************************/
/*	Class:	ScapeQuad
/*	Desc:	Single quad of landscape mesh
/*****************************************************************************/
class ScapeQuad
{
public:
	//  internal class Attr
	class Attr
	{
	public:
		Attr() : qx(0), qy(0), qlod(0) {}
		Attr( int x, int y, int lod ) : qx(x), qy(y), qlod(lod) {}		
		unsigned int hash() const 
		{ 
			unsigned char* s = (unsigned char*) this;
			unsigned int h = 0;
			int nBytes = sizeof( *this );
			for (int i = 0; i < nBytes; i++)
			{
				h = (h * 729 + (*s) * 37) ^ (h >> 1);
				s++;
			}
			return h;
		}
		int			qx;
		int			qy;
		int			qlod;
	}; // Attr

public:
	ScapeQuad();

	bool	IsVisible();
	void	CreateInWorld( int qx, int qy, int qlod );
	void	CreateTIN( int qx, int qy, int qlod );
	void	InitMesh();

	bool hasAttr( const Attr& attr ) const 
	{ return (lod == attr.qlod) && (attr.qx == bound.x) && (attr.qy == bound.y); }

	int		getSizeBytes() const 
	{  
		return sizeof( *this ) + mesh.getSizeBytes() - sizeof( BaseMesh );
	}

	unsigned int hash() const 
	{
		return Attr( bound.x, bound.y, lod ).hash();
	}

protected:
	static void		InitIndices();	
	static  WORD	qIdx[c_QuadInd];

	BaseMesh		mesh;

private:
	friend class ScapeMesh;
	friend class ShadowPatch;
	friend class ShadowMapper;

	//  bounding box
	NRect	bound;
	int		maxH;	//  maximal height of ground surface in the quad
	int		lod;	//  level of quad's detail
}; // ScapeQuad 

const int c_QuadHashTableSize	= 541; // 100th prime number
const int c_QuadPoolSize		= 256;

/*****************************************************************************/
/*	Class:	ScapeMesh
/*	Desc:	Represents landscape mold mesh for filling z-buffer values
/*****************************************************************************/
class ScapeMesh
{
public:

	void		Init( IRenderSystem* iRS );
	void		Show( IRenderSystem* iRS );
	void		Update();
	void		Reset();
	void		SetNeedUpdate( bool need = true ) { needUpdate = need; }

protected:
	Hache<ScapeQuad, c_QuadHashTableSize, c_QuadPoolSize>	quadHache;
	NRect		cachedBounds;
	bool		needUpdate;
	Matrix4D	skewMatr;

private:
	friend class ShadowMapper;
}; // class ScapeMesh

#ifdef _INLINES
#include "akScape.inl"
#endif

#endif // __AKSCAPE_H__