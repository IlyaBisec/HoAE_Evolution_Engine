/*****************************************************************/
/*  File:   gpMesh.h
/*  Desc:   3D visible entities classes
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#ifndef __GPMESH_H__
#define __GPMESH_H__

#include "rsVertex.h"
#include "IVertexBuffer.h"
#include "vVertexIterator.h"
#include "mMath3D.h"

class Sphere;
class Cylinder;

class AABoundBox;
//class geom::Geometry;

/*****************************************************************
/*	Class:	Primitive
/*	Desc:	Base mesh class - all polys are drawn with same 
/*				texture & shader
/*****************************************************************/
class Primitive 
{
public:
	
							Primitive();
							Primitive( const Primitive& bm );
	const Primitive&		operator=( const Primitive& bm );
	virtual					~Primitive();

    void                	copy( const Primitive& bm );

	//  getters
	_inl BYTE*				getVertexData()			const;
	_inl BYTE*				getVertexData( int vIdx)const;
    _inl BYTE*				getVNormalData( int vIdx)const;

	_inl int				getVBufSizeBytes()		const;
	_inl VertexFormat		getVertexFormat()		const;
	_inl int				getNVert()				const;
	_inl int				getMaxVert()			const;
	_inl int				getMaxInd()				const;

	_inl int				getNInd()				const;
	_inl WORD*				getIndices()			const;
	_inl int				getNPri()				const;
	_inl PrimitiveType		getPriType()			const;
	
	_inl int				getShader()				const;	
	_inl int 				getTexture( int stage = 0 )	const;

	_inl int				getIBPos()				const;
	_inl int				getVBPos()				const;
	_inl void*				getDevHandle()			const;
	_inl int				getVertexStride()		const;
	_inl void				setDevHandle( void* handle );

	_inl Vector3D			getVertexXYZ( int verIdx ) const;

    _inl float* 			vertPos( int verIdx );

	
    AABoundBox              GetAABB			()                                  const;
	void					GetBoundSphere	( Sphere& sphere )					const;
    void                	GetBoundCylinder( Vector3D& axis, Cylinder& bcyl )	const;
    void                	GetCentroid		( Vector3D& cen )					const;
	void					GetTriangle		( int triIdx,   Vector3D& v0, 
															Vector3D& v1, 
															Vector3D& v2 ) const;

	_inl bool				isTexCached()			const;
	_inl bool				isIBCached()			const;
	_inl bool				isVBCached()			const;
	_inl bool				isStatic()				const;
	_inl bool				isQuadList()			const;

	_inl bool				isIndexed()				const;

	//  setters
	_inl void				setVertexFormat( VertexFormat vf ); 
	_inl void				setPriType( PrimitiveType pt );
	_inl void				setNVert( int n ); 
	_inl void				setNPri( int n ); 
	_inl void				setNInd( int n );
	_inl void				incNVert( int n ); 
	_inl void				incNPri( int n ); 
	_inl void				incNInd( int n );
    _inl void				setMaxVert( int n );
	_inl void				setMaxInd( int n );
	_inl void				setShader( int shader );
	_inl void				setTexture( int tex, int stage = 0 );

	_inl void				setIndices( const WORD* idxBuf, int numIdx );
	_inl void				setVertices( void* vert, int numVert );
	
	_inl void				setTexCached( bool isCached );
	_inl void				setIBCached( bool isCached );
	_inl void				setVBCached( bool isCached );
	_inl void				setIsStatic( bool isStatic );
	_inl void				setIsQuadList( bool isQList );

	_inl void				setIBPos( int pos );
	_inl void				setVBPos( int pos );

	_inl void				setVertexPtr( void* ptr );

	_inl void				setIndexPtr( WORD* ptr );

    void				    replaceVertexPtr( void* ptr );

	void					rotate( const Vector3D& axis, float angle );
	void					transform( const Matrix4D& transf );
	void					transform( const Matrix4D& transf, int firstVert, int numVert );
	void					transform( const Matrix3D& transf );

	float					distance( const Vector3D& pt, const Vector3D& dir );

	int						getSizeBytes() const;

	void					setDiffuseColor( DWORD color );


	void					create( int numVert, 
									int numInd, 
									VertexFormat vformat = vfVertex2t, 
		  							PrimitiveType typePri = ptTriangleList,
									bool sharedBuffer = false );

	//  construction tools
	void					createLine( const Vector3D& orig, const Vector3D& dest );
	void					createSphere( const Vector3D& center, 
										  float R, 
										  int NSectors );
	void					createQuadArray( int nQuads );
	void					createGrid( Vector3D& lt, 
										Vector3D& rt, 
										Vector3D& lb,
										Vector3D& rb,
										int cellsX, 
										int cellsY );

	void					createGrid( float x, float y, float w, float h, 
										int cellsX, int cellsY );

	void					createQuad( Vector3D& lt, 
										 Vector3D& rt, 
										 Vector3D& lb,
										 Vector3D& rb );

	void					createPatch(		int wSeg, int hSeg,
											Vector3D& lt, 
											Vector3D& rt, 
											Vector3D& lb );

	void					createPrQuad(	float x, float y,
											float w, float h,
                        	                float z = 0.01f );

	void					createEnclosingFrame( float cx, float cy, float cz, float hw, float hh );
    void                	createFrame( float x, float y, float z, float w, float h = 0.0f );

    void					createTriangle();

	void					createUnitCube( float side );
	void					createAABB( const AABoundBox& aabb );

	bool					calcNormals();
	bool					convertVF( VertexFormat to );

	static _inl int			calcNumPri( PrimitiveType pType, int indNum );
	static _inl int			calcNumIdx( PrimitiveType pType, int priNum );


	_inl int				numPriToIdx( int priNum );
	_inl int				numIdxToPri( int indNum );


	float					PickPoly( const Line3D& ray, int& triIdx ) const;
    void					dump();

	void					OnResetDevice();

	_inl BYTE				GetFlagsByte() const { return flags; }
	_inl void				SetFlagsByte( BYTE _flags ) { flags = _flags; }
	bool					CheckSanity() const;


	Primitive&				operator +=( const Primitive& bm );

protected:
	WORD						numPri;		//  number of pimitives (triangles, lines)
	

	void*						vbuf;
	WORD						vertNum;
	WORD						maxVertNum;
	
	WORD*						ibuf;		
	int							indNum;
	int							maxIndNum;

	BYTE						flags;		//  cached flags, static/dynamic etc

	int							vbPos;		//  position in the VB
	int							ibPos;		//  position in the IB
	void*						devHandle;						

	int							texID[3];
	int							shaderID;

	unsigned char				_vertFormat;
	unsigned char				_priType;
	
}; // class Primitive

class BaseMesh : public Primitive{};

struct WPoly
{
	WORD v[3];

	template <class TVert>
	void GetPlane( Plane& plane, TVert* vert )
	{
		plane.from3Points(	*((Vector3D*)&vert[v[0]]),
							*((Vector3D*)&vert[v[1]]),
							*((Vector3D*)&vert[v[2]]) );
	}

	template <class TVert>
	void GetTriangle( Triangle& tri, TVert* vert )
	{
		tri.a = *((Vector3D*)&vert[v[0]]);
		tri.b = *((Vector3D*)&vert[v[1]]);
		tri.c = *((Vector3D*)&vert[v[2]]);
	}

	void GetTriangle( Triangle& tri, BYTE* vert, int vStride )
	{
		tri.a = *((Vector3D*)&vert[v[0]*vStride]);
		tri.b = *((Vector3D*)&vert[v[1]*vStride]);
		tri.c = *((Vector3D*)&vert[v[2]*vStride]);
	}
}; // struct WPoly

_inl void operator <<( VertexIterator& it, const Primitive& bm );

#ifdef _INLINES
#include "gpMesh.inl"
#endif // _INLINES


template <class TVert>
bool CreatePatchGrid( Primitive& pri, const Rct& ext, int wSeg, int hSeg, bool crossIndex = true )
{
	if (wSeg <= 0 || hSeg <= 0) return false;
	int nVert = (wSeg + 1) * (hSeg + 1);
	int nInd = wSeg * hSeg * 6;
	pri.create( nVert, nInd, TVert::format(), ptTriangleList );

	WORD* idx = pri.getIndices();
	int cI = 0;
	int cV = 0;
	for (int j = 0; j < hSeg; j++)
	{
		for (int i = 0; i < wSeg; i++)
		{
			if (crossIndex && ((i + j) % 2 == 1)) 
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

	TVert* v = (TVert*)pri.getVertexData();
	float wstep = ext.w / float( wSeg );
	float hstep = ext.h / float( hSeg );

	cV = 0;
	for (int j = 0; j <= hSeg; j++)
	{
		for (int i = 0; i <= wSeg; i++)
		{
			TVert& vert = v[cV];
			vert.x = ext.x + float( i )*wstep;
			vert.y = ext.y + float( j )*hstep;
			vert.z = 0.0f;
			vert.u = float( i )/float( wSeg );
			vert.v = float( j )/float( hSeg );

			vert.diffuse = 0xFF808080;
			cV++;
		}
	}

	pri.setNVert( nVert		);
	pri.setNPri	( nInd / 3	);
	pri.setNInd	( nInd		);
	return true;
} // CreatePatchGrid

template <class TVert>
bool CreateJaggedGrid( Primitive& pri, const Rct& ext, int wSeg, int hSeg, int jagShift = 0 )
{
	if (wSeg <= 0 || hSeg <= 0) return false;
	int nVert = (wSeg + 1) * (hSeg + 1);
	int nInd = wSeg * hSeg * 6;
	pri.create( nVert, nInd, TVert::format(), ptTriangleList );

	WORD* idx = pri.getIndices();
	int cI = 0;
	int cV = 0;
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

	TVert* v = (TVert*)pri.getVertexData();
	float wstep = ext.w / float( wSeg );
	float hstep = ext.h / float( hSeg );
	float hshift = -0.5f * ext.h / float( hSeg );

	cV = 0;
	for (int j = 0; j <= hSeg; j++)
	{
		for (int i = 0; i <= wSeg; i++)
		{
			TVert& vert = v[cV];
			vert.x = ext.x + float( i )*wstep;
			vert.y = ext.y + float( j )*hstep;
			vert.z = 0.0f;
			vert.u = float( i )/float( wSeg );
			vert.v = float( j )/float( hSeg );

			if (((i + jagShift)&1) == 0) vert.y -= hshift;

			//vert.diffuse = 0xFFFFFFFF;
			cV++;
		}
	}

	pri.setNVert( nVert		);
	pri.setNPri	( nInd / 3	);
	pri.setNInd	( nInd		);
	return true;
} // CreateJaggedGrid

template <class TVert>
void UVMapPlanar( Primitive& pri, const Matrix4D& planeTM, float w, float h )
{
	VertexIterator vit;
	vit << pri;
	while (vit)
	{
		Vector3D v = vit.pos();
		planeTM.transformPt( v );
		vit.uv(0).u = v.x / w;
		vit.uv(0).v = v.y / h;
		++vit;
	}
} // UVMapPlanar

template <class TVert>
void UV2MapPlanar( Primitive& pri, const Matrix4D& planeTM, float w, float h )
{
	VertexIterator vit;
	vit << pri;
	while (vit)
	{
		Vector3D v = vit;
		planeTM.transformPt( v );
		vit.u2() = v.x / w;
		vit.v2() = v.y / h;
		++vit;
	}
} // UVMapPlanar

template <class TVert> void SetW( Primitive& pri, float val )
{
	int nV = pri.getNVert();
	TVert* v = (TVert*)pri.getVertexData();
	for (int i = 0; i < nV; i++)
	{
		v[i].w = val;
	}
}

template <class VDest, class VSrc> 
void ConvertVF( BaseMesh& bm )
{
    if (VDest::format() == VSrc::format()) return;
	VSrc* sv = (VSrc*) bm.getVertexData();
	int nV = bm.getNVert();
	VDest* dv = new VDest[nV];

    VertexDeclaration vds = CreateVertexDeclaration( VSrc::format() );
    VertexDeclaration vdd = CreateVertexDeclaration( VDest::format() );

    VertexIterator its( (BYTE*)sv, nV, vds );
    VertexIterator itd( (BYTE*)dv, nV, vdd );

	while (its)
    {
        itd.pos()       = its.pos();
        itd.rhw()       = its.rhw();
        itd.normal()    = its.normal();
        itd.diffuse()   = its.diffuse();
        itd.specular()  = its.specular();
        itd.tangent()   = its.tangent();
        itd.binormal()  = its.binormal();
        
        for (int i = 0; i < c_MaxTextureCoordinates; i++) itd.uv( i ) = its.uv( i );
        for (int i = 0; i < c_MaxBlendWeights; i++)
        {
            itd.blend_idx( i ) = its.blend_idx( i );
            itd.blend_w( i )   = its.blend_w( i );     
        }
        ++its;
        ++itd;
    }
	
	bm.replaceVertexPtr( dv );
	bm.setVertexFormat( VDest::format() );
} // ConvertVF

void CreateEdgeTearMesh( const BaseMesh& pri, BaseMesh& edgeTear, DWORD clr = 0xFFFF0000 );
void DrawBM( BaseMesh& bm );
void DrawPrimBM( BaseMesh& bm );

#endif // __GPMESH_H__