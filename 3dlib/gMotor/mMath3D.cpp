/*****************************************************************
/*  File:   mMath3D.cpp                                          *
/*  Author: Silver, Copyright (C) GSC Game World                 *
/*  Date:   January 2002                                         *
/*****************************************************************/
#include "stdafx.h"
#include "mMath3D.h"

#ifndef _INLINES
#include "mMath3D.inl"
#endif // !_INLINES


Plane					Plane::xOz = { 0.0f, 1.0f, 0.0f, 0.0f };
Plane					Plane::yOz = { 1.0f, 0.0f, 0.0f, 0.0f };
Plane					Plane::xOy = { 0.0f, 0.0f, 1.0f, 0.0f };

WORD					Quad3D::idx[6]		= { 0, 1, 2, 2, 1, 3 };
WORD					Quad3D::idxccw[6]	= { 0, 2, 1, 1, 2, 3 };

inline float rand( int min, int max )
{
    float rndval = static_cast<float>( rand() );
    rndval /= (float)RAND_MAX;
	rndval *= max - min;
	rndval += min;
	return rndval;
}

/*****************************************************************
/*	Vector3D implementation
/*****************************************************************/
const	Vector3D Vector3D::oX( 1.0f, 0.0f, 0.0f );
const	Vector3D Vector3D::oY( 0.0f, 1.0f, 0.0f );
const	Vector3D Vector3D::oZ( 0.0f, 0.0f, 1.0f );
const	Vector3D Vector3D::null( 0.0f, 0.0f, 0.0f );

void Vector3D ::Dump()
{
	Log.Message( "x: %f. y: %f. z: %f.", x, y, z );
}

void Vector3D::txtSave( FILE* fp, const char* name )
{
	if (name)
	{
		fprintf( fp, "%s={ %-8.8f, %-8.8f, %-8.8f }\n", name, x, y, z );
	}
	else
	{
		fprintf( fp, "={ %-8.8f, %-8.8f, %-8.8f }\n", x, y, z );
	}
} // Vector3D::txtSave

bool Vector3D::orthonormalize( Vector3D& v1, Vector3D& v2, Vector3D& v3 )
{
	v3.cross( v1, v2 );
	v2.cross( v3, v1 );
	
	v1.normalize();
	v2.normalize();
	v3.normalize();
	
	return	(	fabs( v1.norm2() ) > c_SmallEpsilon &&
				fabs( v2.norm2() ) > c_SmallEpsilon &&
				fabs( v3.norm2() ) > c_SmallEpsilon);
} // Vector3D::orthonormalize

bool Vector3D::CreateBasis( Vector3D& vx, Vector3D& vy )
{
	if (fabs( x ) > c_SmallEpsilon) vx = Vector3D::oX;
	else if (fabs( y ) > c_SmallEpsilon) vx = Vector3D::oY;
	else if (fabs( z ) > c_SmallEpsilon) vx = Vector3D::oZ;
	else return false;

	vy.cross( *this, vx );
	orthonormalize( *this, vx, vy );
	return true;
} // Vector3D::CreateBasis

/*---------------------------------------------------------------*
/*  Func:	Vector3D::splineCatmullRom
/*	Desc:	Calculates point on the Catmull-Rom spline
/*	Parm:	t - interp parameter
/*			p0, p1, p2, p3 - interp points
/*---------------------------------------------------------------*/
void Vector3D::splineCatmullRom( float t,
									const Vector3D& p0,
									const Vector3D& p1,
									const Vector3D& p2,
									const Vector3D& p3 )
{
	float t2 = t * t;
	float t3 = t2 * t;

	x = 0.5f * (	2 * p1.x									+ 
					(-p0.x + p2.x)						* t		+
					(2*p0.x - 5*p1.x + 4*p2.x - p3.x)	* t2	+
					(-p0.x + 3*p1.x - 3*p2.x + p3.x)	* t3 );
	
	y = 0.5f * (	2 * p1.y									+ 
					(-p0.y + p2.y)						* t		+
					(2*p0.y - 5*p1.y + 4*p2.y - p3.y)	* t2	+
					(-p0.y + 3*p1.y - 3*p2.y + p3.y)	* t3 );
	
	z = 0.5f * (	2 * p1.z									+ 
					(-p0.z + p2.z)						* t		+
					(2*p0.z - 5*p1.z + 4*p2.z - p3.z)	* t2	+
					(-p0.z + 3*p1.z - 3*p2.z + p3.z)	* t3 );
	
}//  Vector3D::splineCatmullRom

Vector3D Vector3D::GetRandomDir()
{
	Vector3D res;
	res.x = rndValuef( -1.0f, 1.0f );
	res.y = rndValuef( -1.0f, 1.0f );
	res.z = rndValuef( -1.0f, 1.0f );
	res.normalize();

	return res;
} // Vector3D::GetRandomDir

/*****************************************************************
/*	Vector4D implementation
/*****************************************************************/
const	Vector4D Vector4D::oX( 1.0f, 0.0f, 0.0f );
const	Vector4D Vector4D::oY( 0.0f, 1.0f, 0.0f );
const	Vector4D Vector4D::oZ( 0.0f, 0.0f, 1.0f );

Vector4D::Vector4D() : x(0.0f), y(0.0f), z(0.0f), w(1.0f)
{}

Vector4D::Vector4D(  const float _x,
					 const float _y, 
					 const float _z )
{
	x = _x; y = _y; z = _z; w = 1.0f;
}

Vector4D::Vector4D(  const float _x,
					 const float _y, 
					 const float _z,
					 const float _w )
{
	x = _x; y = _y; z = _z; w = _w;
}

Vector4D::Vector4D( const Vector4D& orig )
{
    x = orig.x;
    y = orig.y;
    z = orig.z;
    w = orig.w;
}

Vector4D::Vector4D( const Vector3D& orig )
{
    x = orig.x;
    y = orig.y;
    z = orig.z;
    w = 1.0f;
}

void Vector4D::txtSave( FILE* fp )
{
	fprintf( fp, "={ %-8.8f, %-8.8f, %-8.8f, %-8.8f }\n", x, y, z, w );
}

/*****************************************************************
/*	Matrix4D implementation
/*****************************************************************/
const Matrix4D	Matrix4D::identity(  1.0f, 0.0f, 0.0f, 0.0f,
									 0.0f, 1.0f, 0.0f, 0.0f,
									 0.0f, 0.0f, 1.0f, 0.0f,
									 0.0f, 0.0f, 0.0f, 1.0f );
/*---------------------------------------------------------------*
/*  Func:	Matrix4D::TransformationFromPlaneToWorld
/*	Desc:	getting Matrix4D that transforms 3 2D-vectors (xi,yi,0)
/*			to 3 3D-vectors (Xi,Yi,Zi)
/*	Parm:	VSi - 2d vectors, VWi - 3d vectors
/*---------------------------------------------------------------*/
void Matrix4D::TransformationFromPlaneToWorld(
				const Vector2Df& VS1, const Vector2Df& VS2, const Vector2Df& VS3,
				const Vector3D&  VW1, const Vector3D&  VW2, const Vector3D&  VW3 )
{
	Matrix3D M;
	Matrix3D W;
	Matrix3D V;
	Matrix3D T1;
	Matrix3D T2;
	V.e00=VS1.x;V.e10=VS1.y;V.e20=1.0f;
	V.e01=VS2.x;V.e11=VS2.y;V.e21=1.0f;
	V.e02=VS3.x;V.e12=VS3.y;V.e22=1.0f;
	T1=V;
	V.inverse();
	W.e00=VW1.x;W.e10=VW1.y;W.e20=VW1.z;
	W.e01=VW2.x;W.e11=VW2.y;W.e21=VW2.z;
	W.e02=VW3.x;W.e12=VW3.y;W.e22=VW3.z;
	M.mul(W,V);
	T2.mul(M,T1);
	e00=M.e00;e10=M.e01;e20=0.0f;e30=M.e02;
	e01=M.e10;e11=M.e11;e21=0.0f;e31=M.e12;
	e02=M.e20;e12=M.e21;e22=0.0f;e32=M.e22;
	e03=0.0f; e13=0.0f; e23=0.0f;e33=1.0f;
}//  Matrix4D::TransformationFromPlaneToWorld

void Matrix4D::txtSave( FILE* fp, const char* name ) const
{
	if (name)
	{
		fprintf( fp,	"%s={\n{%-8.8f, %-8.8f, %-8.8f, %-8.8f}, \n"
						"{%-8.8f, %-8.8f, %-8.8f, %-8.8f}, \n"
						"{%-8.8f, %-8.8f, %-8.8f, %-8.8f}, \n"
						"{%-8.8f, %-8.8f, %-8.8f, %-8.8f} \n}\n",
						name,
						e00, e01, e02, e03, 
						e10, e11, e12, e13, 
						e20, e21, e22, e23,
						e30, e31, e32, e33 );
	}
	else
	{
		fprintf( fp,	"%s={\n{%-8.8f, %-8.8f, %-8.8f, %-8.8f}, \n"
						"{%-8.8f, %-8.8f, %-8.8f, %-8.8f}, \n"
						"{%-8.8f, %-8.8f, %-8.8f, %-8.8f}, \n"
						"{%-8.8f, %-8.8f, %-8.8f, %-8.8f} \n}\n",
						name,
						e00, e01, e02, e03, 
						e10, e11, e12, e13, 
						e20, e21, e22, e23,
						e30, e31, e32, e33 );
	}
} // Matrix4D::txtSave

void Matrix4D::Random( float minV, float maxV )
{
	float* buf = getBuf();
	for (int i = 0; i < 16; i++)
	{
		buf[i] = rndValuef( minV, maxV );
	}
}

/*---------------------------------------------------------------*
/*  Func:	Matrix4D::inverse
/*	Desc:	Finds 4x4 matrix inverse using Cramer's rule
/*	Parm:	orig - original matrix
/*	Ret:	determinant value
/*---------------------------------------------------------------*/
float Matrix4D::inverse( const Matrix4D& orig )
{
	float tmp[12]; 
	float src[16]; 
	float* mat = orig.getBuf();
	float* dst = getBuf();
	float det = 0.0f;

	for ( int i = 0; i < 4; i++) 
	{
		src[i		]	= mat[i*4		];
		src[i + 4	]	= mat[i*4 + 1	];
		src[i + 8	]	= mat[i*4 + 2	];
		src[i + 12	]	= mat[i*4 + 3	];
	}
	
	//  co-factors
	tmp[0] = src[10] * src[15];
	tmp[1] = src[11] * src[14];
	tmp[2] = src[9]	 * src[15];
	tmp[3] = src[11] * src[13];
	tmp[4] = src[9]  * src[14];
	tmp[5] = src[10] * src[13];
	tmp[6] = src[8]  * src[15];
	tmp[7] = src[11] * src[12];
	tmp[8] = src[8]  * src[14];
	tmp[9] = src[10] * src[12];
	tmp[10] = src[8] * src[13];
	tmp[11] = src[9] * src[12];
	
	dst[0] =	tmp[0] * src[5] + tmp[3] * src[6] + tmp[4]  * src[7];
	dst[0] -=	tmp[1] * src[5] + tmp[2] * src[6] + tmp[5]  * src[7];
	dst[1] =	tmp[1] * src[4] + tmp[6] * src[6] + tmp[9]  * src[7];
	dst[1] -=	tmp[0] * src[4] + tmp[7] * src[6] + tmp[8]  * src[7];
	dst[2] =	tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7];
	dst[2] -=	tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7];
	dst[3] =	tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6];
	dst[3] -=	tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6];
	dst[4] =	tmp[1] * src[1] + tmp[2] * src[2] + tmp[5]  * src[3];
	dst[4] -=	tmp[0] * src[1] + tmp[3] * src[2] + tmp[4]  * src[3];
	dst[5] =	tmp[0] * src[0] + tmp[7] * src[2] + tmp[8]  * src[3];
	dst[5] -=	tmp[1] * src[0] + tmp[6] * src[2] + tmp[9]  * src[3];
	dst[6] =	tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3];
	dst[6] -=	tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3];
	dst[7] =	tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2];
	dst[7] -=	tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2];
	
	tmp[0]	= src[2] * src[7];
	tmp[1]	= src[3] * src[6];
	tmp[2]	= src[1] * src[7];
	tmp[3]	= src[3] * src[5];
	tmp[4]	= src[1] * src[6];
	tmp[5]	= src[2] * src[5];
	tmp[6]	= src[0] * src[7];
	tmp[7]	= src[3] * src[4];
	tmp[8]	= src[0] * src[6];
	tmp[9]	= src[2] * src[4];
	tmp[10] = src[0] * src[5];
	tmp[11] = src[1] * src[4];
	
	dst[8] =	tmp[0]  * src[13] + tmp[3] * src[14] + tmp[4] * src[15];
	dst[8] -=	tmp[1]  * src[13] + tmp[2] * src[14] + tmp[5] * src[15];
	dst[9] =	tmp[1]  * src[12] + tmp[6] * src[14] + tmp[9] * src[15];
	dst[9] -=	tmp[0]  * src[12] + tmp[7] * src[14] + tmp[8] * src[15];
	dst[10] =	tmp[2]  * src[12] + tmp[7] * src[13] + tmp[10] * src[15];
	dst[10]-=	tmp[3]  * src[12] + tmp[6] * src[13] + tmp[11] * src[15];
	dst[11] =	tmp[5]  * src[12] + tmp[8] * src[13] + tmp[11] * src[14];
	dst[11]-=	tmp[4]  * src[12] + tmp[9] * src[13] + tmp[10] * src[14];
	dst[12] =	tmp[2]  * src[10] + tmp[5] * src[11] + tmp[1] * src[9];
	dst[12]-=	tmp[4]  * src[11] + tmp[0] * src[9] + tmp[3] * src[10];
	dst[13] =	tmp[8]  * src[11] + tmp[0] * src[8] + tmp[7] * src[10];
	dst[13]-=	tmp[6]  * src[10] + tmp[9] * src[11] + tmp[1] * src[8];
	dst[14] =	tmp[6]  * src[9]  + tmp[11] * src[11] + tmp[3] * src[8];
	dst[14]-=	tmp[10] * src[11] + tmp[2]  * src[8] + tmp[7] * src[9];
	dst[15] =	tmp[10] * src[10] + tmp[4]  * src[8] + tmp[9] * src[9];
	dst[15]-=	tmp[8]  * src[9]  + tmp[11] * src[10] + tmp[5] * src[8];
	
	//  determinant
	det=src[0] * dst[0]+src[1] * dst[1]+src[2] * dst[2]+src[3] * dst[3];
	//  matrix inverse 
	float idet = 1.0f;
	if (fabs( det ) >= c_SmallEpsilon) idet = 1.0f / det;
	for (int j = 0; j < 16; j++) dst[j] *= idet;

    return det;
}

/*****************************************************************************/
/*	Matrix3D implementation
/*****************************************************************************/
void Matrix3D::random( float minV, float maxV )
{
	float* buf = getBuf();
	for (int i = 0; i < 9; i++)
	{
		buf[i] = rndValuef( minV, maxV );
	}
} // Matrix3D::random

/*****************************************************************************/
/*	Line3D implementation
/*****************************************************************************/
Line3D::Line3D( const Vector3D& base, const Vector3D& direction )
{
	orig.copy( base );
	dir.copy( direction );
}

void Line3D::Random( const AABoundBox& aabb, float minR, float maxR )
{
	orig = aabb.GetRandomPoint();
	dir = Vector3D::GetRandomDir();
	float h = rndValuef( minR, maxR );
	dir *= h;
}	







