/*****************************************************************/
/*  File:   gpMesh.cpp
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#include "stdafx.h"
#include "mMath3D.h"
#include "mGeom3D.h" 
#include "gpMesh.h"
#include "kHash.hpp"
#include "kSSEUtils.h"

#ifndef _INLINES
#include "gpMesh.inl"
#endif // !_INLINES

static int s_NumPrimitives = 0;
/*****************************************************************************/
/*	Primitive implementation
/*****************************************************************************/
Primitive::Primitive()
{
	texID[0]	= -1;
	texID[1]	= -1;
	texID[2]	= -1;
	shaderID	= 0;

	indNum		= 0;
	vertNum		= 0;
	numPri		= 0;
	
	maxVertNum	= 0;
	maxIndNum	= 0;

	vbuf		= 0;
	ibuf		= 0;

	setVertexFormat( vfUnknown );
	setPriType( ptUnknown );

	flags		= 0;
	devHandle	= 0;

	vbPos		= -1;
	ibPos		= -1;

	s_NumPrimitives++;
}

Primitive::Primitive( const Primitive& bm )
{
    texID[0]	= -1;
    texID[1]	= -1;
    texID[2]	= -1;
    shaderID	= 0;

    indNum		= 0;
    vertNum		= 0;
    numPri		= 0;

    maxVertNum	= 0;
    maxIndNum	= 0;

    vbuf		= 0;
    ibuf		= 0;

    setVertexFormat( vfUnknown );
    setPriType( ptUnknown );

    flags		= 0;
    devHandle	= 0;

    vbPos		= -1;
    ibPos		= -1;

	copy( bm );
	s_NumPrimitives++;
}

const Primitive&	Primitive::operator=( const Primitive& bm )
{
	copy( bm );
	return *this;
}

void Primitive::copy( const Primitive& bm )
{
	delete []ibuf;
    aligned_delete_nodestruct( vbuf );

	_priType	= bm._priType;
	_vertFormat = bm._vertFormat;
	flags		= bm.flags;
	ibPos		= bm.ibPos;
	vbPos		= bm.vbPos;
	indNum		= bm.indNum;
	maxIndNum	= bm.indNum;
	vertNum		= bm.vertNum;
	maxVertNum	= bm.vertNum;
	numPri		= bm.numPri;
	shaderID	= bm.shaderID;
	texID[0]	= bm.texID[0];
	texID[1]	= bm.texID[1];
	texID[2]	= bm.texID[2];
	ibuf		= new WORD[indNum];
	vbuf		= Vertex::CreateVBuf( getVertexFormat(), vertNum );
	memcpy( ibuf, bm.ibuf, indNum * 2 );
	memcpy( vbuf, bm.vbuf, getVBufSizeBytes() );
}

Primitive::~Primitive()
{
	delete []ibuf;
    aligned_delete_nodestruct( vbuf );
	ibuf = 0;
	vbuf = 0;
	s_NumPrimitives--;
}

void Primitive::OnResetDevice()
{
	devHandle	= 0;
	vbPos		= -1;
	ibPos		= -1;
} // Primitive::OnResetDevice

int	Primitive::getSizeBytes() const
{
	int res = sizeof( *this );
	res += maxIndNum * 2;
	res += getVBufSizeBytes();
	return res;
}

void Primitive::create( int numVert, int numInd, VertexFormat vformat, PrimitiveType typePri, bool sharedBuffer )
{
    indNum		= 0;
	vertNum		= 0;
	numPri		= 0; 

    if (numVert == maxVertNum && maxIndNum == numInd &&
        vformat == getVertexFormat() && typePri == getPriType()) return;

	maxVertNum	= numVert;
	maxIndNum	= numInd;
	setPriType( typePri );

	if (!sharedBuffer)
	{
		delete []ibuf;
		aligned_delete_nodestruct( vbuf );
		ibuf		= NULL;
		vbuf		= NULL;

		if (maxIndNum > 0) ibuf = new WORD[maxIndNum];
		vbuf = Vertex::CreateVBuf( vformat, numVert );
	}
	setVertexFormat( vformat );
} // Primitive::create

void Primitive::replaceVertexPtr( void* ptr )
{
    aligned_delete_nodestruct( vbuf );
    vbuf = ptr;
} // Primitive::replaceVertexPtr

AABoundBox Primitive::GetAABB() const
{
    AABoundBox aabb;
	int vstride = Vertex::GetStride( (VertexFormat)_vertFormat );
	float *pbuf = (float*) vbuf;
	BYTE* buf = (BYTE*) vbuf;
    if (vertNum == 0) return AABoundBox::null;
	aabb.maxv.x = pbuf[0];
	aabb.maxv.y = pbuf[1];
	aabb.maxv.z = pbuf[2];
	aabb.minv.copy( aabb.maxv );
	buf += vstride;

	for (int i = 1; i < vertNum; i++)
	{
		pbuf = (float*)buf;
		if (pbuf[0] < aabb.minv.x) aabb.minv.x = pbuf[0];
		if (pbuf[1] < aabb.minv.y) aabb.minv.y = pbuf[1];
		if (pbuf[2] < aabb.minv.z) aabb.minv.z = pbuf[2];

		if (pbuf[0] > aabb.maxv.x) aabb.maxv.x = pbuf[0];
		if (pbuf[1] > aabb.maxv.y) aabb.maxv.y = pbuf[1];
		if (pbuf[2] > aabb.maxv.z) aabb.maxv.z = pbuf[2];
		buf += vstride;
	}
    return aabb;
} // Primitive::GetAABB

bool Primitive::CheckSanity() const
{
	//  checking indices sanity
	for (int i = 0; i < indNum; i++)
	{
		if (ibuf[i] >= vertNum)
		{
			assert( false );
			return false;
		}
	}

	//  vertex/index number sanity
	if (vertNum > maxVertNum || indNum > maxIndNum)
	{
		assert( false );
		return false;
	}

	return true;
} // Primitive::CheckSanity

void Primitive::GetBoundSphere( Sphere& sphere )	const
{
	Vector3D center;
	GetCentroid( center );
	sphere.SetCenter( center );
	VertexIterator it;
	it << *this;
	float maxDist = 0.0f;
	while (it)
	{
		Vector3D pt( it.pos( 0 ) );
		pt.sub( center );
		float dist = pt.norm2();
		if (dist > maxDist) maxDist = dist;
		it++;
	}
	sphere.SetRadius( sqrt( maxDist ) );
} // Primitive::GetBoundSphere

/*---------------------------------------------------------------------------*/
/*	Func:	Primitive::GetBoundCylinder
/*	Desc:	Finds bounding cylinder of the mesh, aligned to the given axis
/*	Parm:	axis - axis of the cylinder
/*			bcyl - cylinder to place result to
/*---------------------------------------------------------------------------*/
void Primitive::GetBoundCylinder( Vector3D& axis, Cylinder& bcyl ) const
{
    Vector3D centroid;
    GetCentroid( centroid );
    Line3D line( axis, centroid );

    BYTE* pv = (BYTE*)vbuf;
    int stride = Vertex::GetStride(  getVertexFormat() );
    
	if (vertNum < 1) return;
	float cdist, maxp, minp;
	Vector3D* v = reinterpret_cast<Vector3D*>( pv );
    
    cdist = line.dist2ToOrig( *v );
	maxp  = line.project( *v );
	minp  = maxp;

	pv += stride;

	for (int i = 1; i < vertNum; i++)
	{
		v = reinterpret_cast<Vector3D*>( pv );
		float dist	= line.dist2ToOrig( *v );
		float pr	= line.project( *v );
		if (dist > cdist) cdist = dist;
		if (pr < minp) minp = pr;
		if( pr > maxp) maxp = pr;

        pv += stride;
	}

	Vector3D bottom, top;
	float r2 = cdist;
	line.getPoint( minp, bottom );
	line.getPoint( maxp, top );
	assert( false );
	//bcyl.Create( bottom, top, r2 );
} // Primitive::GetBoundCylinder

/*---------------------------------------------------------------------------*/
/*	Func:	Primitive::GetCentroid
/*	Desc:	Finds mass center of the mesh
/*---------------------------------------------------------------------------*/
void Primitive::GetCentroid( Vector3D& cen ) const
{
    cen.zero();
    BYTE* pv = (BYTE*)vbuf;
    int stride = Vertex::GetStride(  getVertexFormat() );
    for (int i = 0; i < vertNum; i++, pv += stride)
        cen.add( *((Vector3D*)pv) );
    cen /= vertNum;
} // Primitive::GetCentroid

bool Primitive::calcNormals()
{	
	VertexIterator vit;
	vit << (*this);

	//  set zero normals
    int i;
	for (i = 0; i < vertNum; i++)
	{
		vit.normal( i ).zero();
	}

	//  accumulate vertex normals from face normals
	for (i = 0; i < indNum; i += 3)
	{
		const Vector3D& v1 = vit.pos( ibuf[i    ] );
		const Vector3D& v2 = vit.pos( ibuf[i + 1] );
		const Vector3D& v3 = vit.pos( ibuf[i + 2] );
		
		Plane pl; pl.from3Points( v1, v2, v3 );
		const Vector3D& faceNormal = pl.normal();
			
		Vector3D& n1 = vit.normal( ibuf[i    ] );
		Vector3D& n2 = vit.normal( ibuf[i + 1] );
		Vector3D& n3 = vit.normal( ibuf[i + 2] );
		
		n1.add( faceNormal );
		n2.add( faceNormal );
		n3.add( faceNormal );
	}

	//  normalize normals :)
	for (i = 0; i < vertNum; i++)
	{
		vit.normal( i ).normalize();
	}
	return true;
}

bool Primitive::convertVF( VertexFormat to )
{
	VertexFormat myVF = getVertexFormat();
	if (myVF == to) return true;
	
	if (myVF == vfVertex2t && to == vfVertexN)
	{
		Vertex2t*	p2t = (Vertex2t*)	vbuf;
		VertexN*	pN	= (VertexN*)	vbuf;
		float u, v;
		DWORD diffuse;
		
		for (int i = 0; i < vertNum; i++)
		{
			u			= p2t[i].u;
			v			= p2t[i].v;
			diffuse		= p2t[i].diffuse;

			pN[i].u		= u;
			pN[i].v		= v;
			pN[i].nx	= 0.0f;
			pN[i].ny	= 0.0f;
			pN[i].nz	= 0.0f;
		}
		setVertexFormat( vfVertexN );
		calcNormals();
		return true;
	}

	if (myVF == vfVertexN && to == vfVertexN2T)
	{
		VertexN*	sv	= (VertexN*) getVertexData();
		VertexN2T*	dv	= new VertexN2T[vertNum];
		
		for (int i = 0; i < vertNum; i++)
		{
			dv[i].x			= sv[i].x;
			dv[i].y			= sv[i].y;
			dv[i].z			= sv[i].z;

			dv[i].nx		= sv[i].nx;
			dv[i].ny		= sv[i].ny;
			dv[i].nz		= sv[i].nz;

			dv[i].u			= sv[i].u;
			dv[i].v			= sv[i].v;
			
		}
		setVertexFormat( vfVertexN2T );
		delete []sv;
		setVertexPtr( dv );
		return true;
	}

	assert( false );
	/*
	int oldStride = Vertex::GetStride( myVF );
	int newStride = Vertex::GetStride( to );

	setVertexFormat( to );
	if (oldStride != newStride)
	{
		setMaxVert( (getMaxVert() * oldStride) / newStride );
	}

	setNVert( 0 );
	setNInd ( 0 );
	setNPri ( 0 );
	*/
	return true;
}

void Primitive::dump()
{
	Log.Message( "numPri %d, indNum %d, vertNum %d, texID %d",
					numPri, indNum, vertNum, texID[0] );
	for (int i = 0; i < indNum; i++) Log.Message( "%d", ibuf[i] );
}

/*---------------------------------------------------------------*
/*  Func:	Primitive::rotate
/*	Desc:	rotates object around axis on the given angle
/*	Parm:	axis - rotation axis
/*			angle - rotation angle, in radians
/*---------------------------------------------------------------*/
void Primitive::rotate( const Vector3D& axis, float angle )
{
	BYTE* bVbuf = (BYTE*) vbuf;
	int stride = Vertex::GetStride( getVertexFormat() );
	Matrix3D rot;
	rot.rotation( axis, angle );
	for (int i = 0; i < vertNum; i++)
	{
		Vector3D& v = *((Vector3D*)bVbuf);
		v *= rot; 
		bVbuf += stride;
	}
}//  Primitive::rotate

/*---------------------------------------------------------------*
/*  Func:	Primitive::transform
/*	Desc:	applies transformation to the mesh
/*	Parm:	transf - 3D transformation matrix
/*---------------------------------------------------------------*/
void Primitive::transform( const Matrix3D& transf )
{
	BYTE* bVbuf = (BYTE*) vbuf;
	int stride = Vertex::GetStride( getVertexFormat() );
	for (int i = 0; i < vertNum; i++)
	{
		Vector3D& v = *((Vector3D*)bVbuf);
		v *= transf; 
		bVbuf += stride;
	}	
}//  Primitive::transform

void Primitive::transform( const Matrix4D& transf, int firstVert, int numVert )
{
	int stride = Vertex::GetStride( getVertexFormat() );
	BYTE* bVbuf = (BYTE*) vbuf + stride * firstVert;
	assert( firstVert >= 0 && numVert >= 0 && numVert + firstVert <= vertNum );
	for (int i = 0; i < numVert; i++)
	{
		Vector3D& v = *((Vector3D*)bVbuf);
		v *= transf; 
		bVbuf += stride;
	}
}//  Primitive::transform
	
/*---------------------------------------------------------------*
/*  Func:	Primitive::transform
/*	Desc:	applies transformation to the mesh
/*	Parm:	transf - 4D transformation matrix
/*---------------------------------------------------------------*/
void Primitive::transform( const Matrix4D& transf )
{
	BYTE* bVbuf = (BYTE*) vbuf;
	int stride = Vertex::GetStride( getVertexFormat() );
	for (int i = 0; i < vertNum; i++)
	{
		transf.transformPt( *((Vector3D*)bVbuf) );
		bVbuf += stride;
	}	
}//  Primitive::transform

void Primitive::setDiffuseColor( DWORD color )
{
	VertexIterator it;
	it << *this;
	while (it)
	{
		it.diffuse() = color;
		++it;
	}
} // Primitive::setDiffuseColor

bool RayTriangleX( const Line3D& ray, 
					   const Vector3D&	v0, 
					   const Vector3D&	v1, 
					   const Vector3D&	v2, 
					   float& u, float& v, 
					   float& t );

float Primitive::PickPoly( const Line3D& ray, int& triIdx ) const
{
	if (getPriType() != ptTriangleList) return -1;
	triIdx = -1;
	int pIdx = -1;
	float cDist = -1.0f, u, v, t;
	float *pV1, *pV2, *pV3;
	BYTE* bVbuf = (BYTE*) vbuf;
	int stride = Vertex::GetStride( getVertexFormat() );

	for (int i = 0; i < indNum; i += 3)
	{
		Vector3D v1, v2, v3;
		pV1 = (float*)(bVbuf + ibuf[i  	 ] * stride);
		pV2 = (float*)(bVbuf + ibuf[i + 1] * stride);
		pV3 = (float*)(bVbuf + ibuf[i + 2] * stride);

		v1.set( pV1[0], pV1[1], pV1[2] );
		v2.set( pV2[0], pV2[1], pV2[2] );
		v3.set( pV3[0], pV3[1], pV3[2] );
		
		if (RayTriangleX( ray, v1, v2, v3, u, v, t ))
		{
			if (cDist == -1.0f || t < cDist)
			{
				triIdx	= i;
				cDist	= t;
			}
		}
		else
		//  backface
		if (RayTriangleX( ray, v3, v2, v1, u, v, t ))
		{
			if (cDist == -1.0f || t < cDist)
			{
				triIdx	= i;
				cDist	= t;
			}
		}
	}
	
	return cDist;
} // Primitive::PickPoly

void Primitive::GetTriangle( int triIdx, Vector3D& v0, Vector3D& v1, Vector3D& v2 ) const
{
	VertexIterator it;
	it << (*this);
	WORD* idx = getIndices();

	v0 = it.pos( idx[triIdx*3 + 0] );
	v1 = it.pos( idx[triIdx*3 + 1] );
	v2 = it.pos( idx[triIdx*3 + 2] );

} // Primitive::GetTriangle

void Primitive::createLine( const Vector3D& orig, const Vector3D& dest )
{
	if (getMaxVert() < 2 || getMaxInd() < 2) return;
	BYTE* pvb = (BYTE*) vbuf;
	float *fv = (float*)pvb;
	int stride = Vertex::GetStride( getVertexFormat() );

	fv[0] = orig.x; fv[1] = orig.y; fv[2] = orig.z;
	pvb += stride; fv = (float*)pvb;

	fv[0] = dest.x; fv[1] = dest.y; fv[2] = dest.z;
	WORD idx[] = { 0, 1 };
	setIndices( idx, 2 );
	setNVert( 2 );
}

void Primitive::createQuadArray( int nQuads )
{
	if (getMaxVert() < nQuads * 4 || getMaxInd() < nQuads * 6) return;
	int cV = 0;
	int cI = 0;
	for (int i = 0; i < nQuads; i++)
	{
		ibuf[cI++]	= cV;
		ibuf[cI++]	= cV + 1;
		ibuf[cI++]	= cV + 2;

		ibuf[cI++]	= cV + 2;
		ibuf[cI++]	= cV + 1;
		ibuf[cI++]	= cV + 3;

		cV += 4;
	}
}

void Primitive::createTriangle()
{
    create( 3, 3 );
    static WORD idx[] = {0, 1, 2};
    setIndices( idx, 3 );
	setNVert( 3 );
}

void Primitive::createPrQuad( float x, float y,
                             float w, float h, float z )
{
	create( 4, 6, vfVertexTnL );
	static const WORD qidx[6] = { 0, 1, 2, 2, 1, 3 };
	setIndices( qidx, 6 );
	VertexTnL* vert = (VertexTnL*) vbuf;

	vert[0].x = x;
	vert[0].y = y;
	vert[0].z = z;

	vert[1].x = x + w;
	vert[1].y = y;
	vert[1].z = z;

	vert[2].x = x;
	vert[2].y = y + h;
	vert[2].z = z;

	vert[3].x = x + w;
	vert[3].y = y + h;
	vert[3].z = z;

	vert[0].w = 1.0f;
	vert[1].w = 1.0f;
	vert[2].w = 1.0f;
	vert[3].w = 1.0f;

	vert[0].u = 0.0f;
	vert[1].u = 1.0f;
	vert[2].u = 0.0f;
	vert[3].u = 1.0f;

	vert[0].v = 0.0f;
	vert[1].v = 0.0f;
	vert[2].v = 1.0f;
	vert[3].v = 1.0f;

	setNVert( 4 );
}

void Primitive::createEnclosingFrame( float cx, float cy, float cz, float hw, float hh )
{
	if (hh == 0.0f) hw = hh;

     create( 4, 6, vfVertexTnL, ptLineStrip );
     static WORD idx[] = { 0, 1, 2, 3, 0 };
	 setIndices( idx, 5 );
	 VertexTnL* vert = (VertexTnL*) vbuf;
     vert[0].x = cx - hw;
     vert[0].y = cy - hh;
     vert[0].z = cz;

     vert[1].x = cx + hw;
     vert[1].y = cy - hh;
     vert[1].z = cz;

     vert[2].x = cx + hw;
     vert[2].y = cy + hh;
     vert[2].z = cz;

     vert[3].x = cx - hw;
     vert[3].y = cy + hh;
     vert[3].z = cz;

     setNVert( 4 );
}

void Primitive::createFrame( float x, float y, float z, float w, float h )
{
	if (h == 0.0f) w = h;

     create( 4, 5, vfVertexTnL, ptLineStrip );
     static WORD idx[] = { 0, 1, 2, 3, 0 };
	 setIndices( idx, 5 );
	 VertexTnL* vert = (VertexTnL*) vbuf;
     vert[0].x = x;
     vert[0].y = y;
     vert[0].z = z;

     vert[1].x = x + w;
     vert[1].y = y;
     vert[1].z = z;

     vert[2].x = x + w;
     vert[2].y = y + h;
     vert[2].z = z;

     vert[3].x = x;
     vert[3].y = y + h;
     vert[3].z = z;

     setNVert( 4 );
} // Primitive::createFrame

void Primitive::createPatch( int wSeg, int hSeg,
							Vector3D& lt, Vector3D& rt, 
							Vector3D& lb )
{
	int nV = (wSeg + 1)*(hSeg + 1);
	int nI = wSeg * hSeg * 6;
	create( nV, nI );

	Vector3D vX, vY, vCur;
	vX.sub( rt, lt );
	vY.sub( lt, lb );

	//  vertices
	float wx, wy, stepwx, stepwy;
	Vertex2t* vert = (Vertex2t*)getVertexData();
	int cV = 0;
	wy = 0.0f;
	stepwx = 1.0f / float( wSeg );
	stepwy = 1.0f / float( hSeg );
	for (int i = 0; i <= hSeg; i++)
	{
		wx = 0.0f;
		for (int j = 0; j <= wSeg; j++)
		{
			vCur.addWeighted( vX, vY, wx, wy );
			vert[cV].x = vCur.x;
			vert[cV].y = vCur.y;
			vert[cV].z = vCur.z;
			
			vert[cV].u = wx;
			vert[cV].v = 1.0f - wy;

			vert[cV].diffuse = 0xFFFFFFFF;

			wx += stepwx;
			cV++;
		}
		wy += stepwy;
	}

	//  indices
	WORD* qIdx = getIndices();
	int vInLine = wSeg + 1;
	int cI = 0;
	cV = 0;
	for (int i = 0; i < hSeg; i++)
	{
		for (int j = 0; j < wSeg; j++)
		{
			qIdx[cI++] = cV;
			qIdx[cI++] = cV + 1;
			qIdx[cI++] = cV + vInLine;
			qIdx[cI++] = cV + vInLine;
			qIdx[cI++] = cV + 1;
			qIdx[cI++] = cV + vInLine + 1;
			cV++;
		}
		cV++;
	}

	setNVert( nV );
	setNInd( nI );
	setNPri( calcNumPri( getPriType(), nI ) );

} // Primitive::createStrippedPatch

void Primitive::createQuad( Vector3D& lt, Vector3D& rt,
							Vector3D& lb, Vector3D& rb )
{
	create( 4, 6 );
	static const WORD qidx[6] = { 0, 1, 2, 2, 1, 3 };
	setIndices( qidx, 6 );
	Vertex2t* vert = (Vertex2t*) vbuf;
	vert[0] = lt;
	vert[0].u = 0.0f;
	vert[0].v = 0.0f;

	vert[1] = rt;
	vert[1].u = 1.0f;
	vert[1].v = 0.0f;

	vert[2] = lb;
	vert[2].u = 0.0f;
	vert[2].v = 1.0f;

	vert[3] = rb;
	vert[3].u = 1.0f;
	vert[3].v = 1.0f;

	setNVert( 4 );
}

void Primitive::createUnitCube( float side )
{
	static float cubevert[][3] = {
		{ -0.5000,	-0.5000,	-0.5000 },
		{  0.5000,	-0.5000,	-0.5000 },
		{ -0.5000,	 0.5000,	-0.5000 },
		{  0.5000,	 0.5000,	-0.5000 },
		{ -0.5000,	-0.5000,	 0.5000 },
		{  0.5000,	-0.5000,	 0.5000 },
		{ -0.5000,	 0.5000,	 0.5000 },
		{  0.5000,	 0.5000,	 0.5000 }
	};

	static WORD cubeidx[] = 
	{
		0, 2, 3,
		3, 1, 0,
		4, 5, 7,
		7, 6, 4,
		0, 1, 5,
		5, 4, 0,
		1, 3, 7,
		7, 5, 1,
		3, 2, 6,
		6, 7, 3,
		2, 0, 4,
		4, 6, 2 
	}; 

	create( 8, sizeof( cubeidx ) / 2, vfVertex2t, ptTriangleList );
	setIndices( cubeidx, sizeof( cubeidx ) / 2 );
    setNVert( 8 );
	Vertex2t* vb = (Vertex2t*) vbuf;
	for (int i = 0; i < 8; i++)
	{
		vb[i].x = cubevert[i][0] * side;
		vb[i].y = cubevert[i][1] * side;
		vb[i].z = cubevert[i][2] * side;
	}
} // Primitive::createUnitCube

Primitive& Primitive::operator +=( const Primitive& bm )
{
	assert( this != &bm );
	assert( _priType == bm._priType && _vertFormat == bm._vertFormat );

	if (maxIndNum < indNum + bm.indNum)
	//  recreate index buffer
	{
		maxIndNum = indNum + bm.indNum;
		WORD* newIB = new WORD[maxIndNum];
		memcpy( newIB, ibuf, indNum * sizeof( *ibuf ) );
		delete []ibuf;
		ibuf = newIB;
	}

	if (maxVertNum < vertNum + bm.vertNum)
	//  recreate vertex buffer
	{
		maxVertNum = vertNum + bm.vertNum;
		BYTE* newVB = (BYTE*)Vertex::CreateVBuf( getVertexFormat(), maxVertNum );
		memcpy( newVB, vbuf, vertNum * getVertexStride() );
        aligned_delete_nodestruct( vbuf );
		vbuf = newVB;
	}

	for (int i = 0; i < bm.indNum; i++)
	{
		ibuf[indNum + i] = vertNum + bm.ibuf[i];
	}
	memcpy(		((BYTE*)vbuf) + vertNum * getVertexStride(), 
				bm.vbuf, 
				bm.vertNum * bm.getVertexStride() );

	vertNum += bm.vertNum;
	indNum	+= bm.indNum;
	numPri  += bm.numPri;

	devHandle	= 0;
	vbPos		= -1;
	ibPos		= -1;
	return (*this);
} // Primitive::operator +=

void Primitive::createAABB( const AABoundBox& aabb )
{
	static WORD boxidx[] = {
		0, 1, 1, 2, 2, 3, 
		3, 0, 0, 4, 4, 7, 
		7, 6, 6, 5, 5, 4,
		1, 5, 2, 6, 3, 7
	};

	create( 8, sizeof( boxidx ) / 2, vfVertex2t, ptLineList );
	setIndices( boxidx, sizeof( boxidx ) / 2 );
    setNVert( 8 );
	
	const Vector3D& vmin = aabb.minv;
	const Vector3D& vmax = aabb.maxv;

	Vertex2t* vb = (Vertex2t*) vbuf;
	vb[0].x = vmin.x; vb[0].y = vmin.y; vb[0].z = vmin.z;
	vb[1].x = vmin.x; vb[1].y = vmin.y; vb[1].z = vmax.z;
	vb[2].x = vmax.x; vb[2].y = vmin.y; vb[2].z = vmax.z;
	vb[3].x = vmax.x; vb[3].y = vmin.y; vb[3].z = vmin.z;
	vb[4].x = vmin.x; vb[4].y = vmax.y; vb[4].z = vmin.z;
	vb[5].x = vmin.x; vb[5].y = vmax.y; vb[5].z = vmax.z;
	vb[6].x = vmax.x; vb[6].y = vmax.y; vb[6].z = vmax.z;
	vb[7].x = vmax.x; vb[7].y = vmax.y; vb[7].z = vmin.z;
} // Kit3D::CreateBox

void Primitive::createGrid( Vector3D& lt, Vector3D& rt, 
							Vector3D& lb, Vector3D& rb,
							int cellsX, int cellsY )
{
	int numV = (cellsX + cellsY) * 2;
	int numI = numV + 4;
	create( numV, numI, vfVertex2t, ptLineList ); 
	Vertex2t* vert = (Vertex2t*)vbuf;
	Vector3D v1, dv1, v2, dv2;

	v1 = lt;
	v2 = lb;
	dv1.copy( rt );
	dv1.sub( lt );
	dv1 /= cellsX;

	dv2.copy( rb );
	dv2.sub( lb );
	dv2 /= cellsX;

	int cV = 0;
    int i;
	for (i = 0; i <= cellsX; i++)
	{
		ibuf[cV] = cV;
		vert[cV].x = v1.x;
		vert[cV].y = v1.y;
		vert[cV].z = v1.z;
		cV++;

		ibuf[cV] = cV;
		vert[cV].x = v2.x;
		vert[cV].y = v2.y;
		vert[cV].z = v2.z;
		cV++;
		
		v1.add( dv1 );
		v2.add( dv2 );
	}

	v1 = lt;
	v2 = rt;
	dv1.copy( lb );
	dv1.sub( lt );
	dv1 /= cellsY;

	dv2.copy( rb );
	dv2.sub( rt );
	dv2 /= cellsY;

	v1.add( dv1 );
	v2.add( dv2 );

	for (i = 1; i < cellsY; i++)
	{
		ibuf[cV] = cV;
		vert[cV].x = v1.x;
		vert[cV].y = v1.y;
		vert[cV].z = v1.z;
		cV++;

		ibuf[cV] = cV;
		vert[cV].x = v2.x;
		vert[cV].y = v2.y;
		vert[cV].z = v2.z;
		cV++;
		
		v1.add( dv1 );
		v2.add( dv2 );
	}

	// top horz line
	ibuf[cV++] = 0;
	ibuf[cV++] = cellsX * 2;
	// bottom horz line
	ibuf[cV++] = 1;
	ibuf[cV++] = cellsX * 2 + 1;

	vertNum = numV;
	indNum	= numI;
	numPri  = numI/2;
}

void Primitive::createGrid( float x, float y, float w, float h, int cellsX, int cellsY )
{
	int numV = (cellsX + cellsY) * 2;
	int numI = numV + 4;
	create( numV, numI, vfVertexTnL, ptLineList ); 
	VertexTnL* vert = (VertexTnL*)vbuf;
	float dv1, dv2;

	dv1 = w / float( cellsX );
	dv2 = h / float( cellsY );

	int cV = 0;
	int i;
	float cx = x;
	for (i = 0; i <= cellsX; i++)
	{
		ibuf[cV]	= cV;
		vert[cV].x	= cx;
		vert[cV].y	= y;
		vert[cV].z	= 0.0f;
		vert[cV].w	= 1.0f;
		cV++;

		ibuf[cV] = cV;
		vert[cV].x = cx;
		vert[cV].y = y + w;
		vert[cV].z = 0.0f;
		vert[cV].w = 1.0f;
		cV++;

		cx += dv1;
	}

	float cy = y + dv2;

	for (i = 1; i < cellsY; i++)
	{
		ibuf[cV] = cV;
		vert[cV].x = x;
		vert[cV].y = cy;
		vert[cV].z = 0.0f;
		vert[cV].w = 1.0f;
		cV++;

		ibuf[cV] = cV;
		vert[cV].x = x + w;
		vert[cV].y = cy;
		vert[cV].z = 0.0f;
		vert[cV].w = 1.0f;
		cV++;

		cy += dv2;
	}

	// top horz line
	ibuf[cV++] = 0;
	ibuf[cV++] = cellsX * 2;

	// bottom horz line
	ibuf[cV++] = 1;
	ibuf[cV++] = cellsX * 2 + 1;

	vertNum = numV;
	indNum	= numI;
	numPri  = numI/2;
}

struct TearEdge
{
    int             a;
    int             b;
    int             count;

    TearEdge() : a(-1), b(-1), count(1){}
    TearEdge( int na, int nb ) : a(na), b(nb), count(1){}


    unsigned int hash() const
    {
        DWORD h = a*b + a + b;
        h = (h << 13) ^ h;
        h += 15731;
        return h;
    }

    bool equal( const TearEdge& el )
    {
        return  (a == el.a && b == el.b) || (a == el.b && b == el.a) ;
    }

    void copy( const TearEdge& el )
    {
        a       = el.a;
        b       = el.b;
        count   = el.count;
    }

}; // struct TearEdge
typedef Hash<TearEdge>   EdgeHash;  

void CreateEdgeTearMesh( const BaseMesh& pri, BaseMesh& edgeTear, DWORD clr )
{
    EdgeHash eHash;
    WORD* idx = pri.getIndices();  
    int nPri = pri.getNPri();
    VertexIterator vit;
    vit << pri;
    int nRep = 0;
    for (int i = 0; i < nPri; i++)
    {
        int idx0 = idx[i*3    ];
        int idx1 = idx[i*3 + 1];
        int idx2 = idx[i*3 + 2];

        TearEdge e[3] = { TearEdge( idx0, idx1 ),
                          TearEdge( idx1, idx2 ),
                          TearEdge( idx2, idx0 ) };
        
        for (int j = 0; j < 3; j++)
        {
            int n = eHash.find( e[j] );
            if (n != NO_ELEMENT) 
            {
                nRep++;
                eHash.elem( n ).count++; 
            }
            else 
            {
                eHash.add( e[j] );
            }
        }
    }

    int nEdges = 0;
    for (int i = 0; i < eHash.numElem(); i++)
    {
        TearEdge& edge = eHash.elem( i );
        if (edge.count == 1) nEdges++;
    }

    int nV = nEdges*2;
    edgeTear.create( nV, 0, vfVertex2t, ptLineList );
    Vertex2t* v = (Vertex2t*)edgeTear.getVertexData();
    int cV = 0;
    for (int i = 0; i < eHash.numElem(); i++)
    {
        TearEdge& edge = eHash.elem( i );
        if (edge.count != 1) continue;
        Vector3D va = vit.pos( edge.a );
        Vector3D vb = vit.pos( edge.b );

        v[cV].x = va.x;
        v[cV].y = va.y;
        v[cV].z = va.z;
        v[cV].diffuse = clr;
        cV++;
        
        v[cV].x = vb.x;
        v[cV].y = vb.y;
        v[cV].z = vb.z;
        v[cV].diffuse = clr;
        cV++;
    }

    edgeTear.setNVert( nV );
    edgeTear.setNInd ( 0 );
    edgeTear.setNPri ( nEdges );
} // CreateEdgeTearMesh

void DrawBM( BaseMesh& bm )
{
    IRS->SetShader( bm.getShader() );
    IRS->SetTexture( bm.getTexture( 0 ), 0, false );
    IRS->SetTexture( bm.getTexture( 1 ), 1, false );
    DrawPrimBM( bm );
} // DrawBM

void DrawPrimBM( BaseMesh& bm )
{
    PrimitiveType priType = bm.getPriType();
    if (bm.isQuadList()) priType = ptQuadList;
        
    static int vbID = IRS->GetVBufferID( "SharedDynamic" );
    static int ibID = IRS->GetIBufferID( "SharedDynamic" );

    IRS->SetVB( vbID, (int)bm.getVertexFormat() );
    IRS->SetIB( ibID );

    int     nV          = bm.getNVert();
    int     nI          = bm.getNInd();
    DWORD   vbStamp     = 0;
    DWORD   ibStamp     = 0;
    int     firstIdx    = 0;
    int     firstVert   = 0;

    if (nI > 0)
    {
        BYTE* pIdx  = IRS->LockAppendIB( ibID, nI, firstIdx, ibStamp );
        if (!pIdx) return;
        memcpy( pIdx, bm.getIndices(), nI*sizeof(WORD) );
        IRS->UnlockIB( ibID );
    }

    BYTE* pVert = IRS->LockAppendVB( vbID, nV, firstVert, vbStamp );
    if (!pVert) return;
    memcpy( pVert, bm.getVertexData(), nV*bm.getVertexStride() );
    IRS->UnlockVB( vbID );
    IRS->Draw( firstVert, nV, firstIdx, nI, priType );

} // DrawPrimBM

