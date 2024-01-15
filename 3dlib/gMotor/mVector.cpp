/*****************************************************************
/*  File:   mVector.cpp 
/*  Author: Silver, Copyright (C) GSC Game World                
/*  Date:   January 2002                                          
/*****************************************************************/
#include "stdafx.h"
#include "mVector.h"

#ifndef _INLINES
#include "mVector.inl"
#endif // !_INLINES

/*****************************************************************
/*    Vector3D implementation
/*****************************************************************/
const    Vector3D Vector3D::oX    ( 1.0f, 0.0f, 0.0f );
const    Vector3D Vector3D::oY    ( 0.0f, 1.0f, 0.0f );
const    Vector3D Vector3D::oZ    ( 0.0f, 0.0f, 1.0f );
const    Vector3D Vector3D::null    ( 0.0f, 0.0f, 0.0f );
const    Vector3D Vector3D::one    ( 1.0f, 1.0f, 1.0f );

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
    
    return    (    fabs( v1.norm2() ) > c_SmallEpsilon &&
                fabs( v2.norm2() ) > c_SmallEpsilon &&
                fabs( v3.norm2() ) > c_SmallEpsilon);
} // Vector3D::orthonormalize

bool Vector3D::orthonormalize( Vector3D& v1, Vector3D& v2 )
{
    v2.cross( v1, *this );
    v1.cross( *this, v2 );

    normalize();
    v1.normalize();
    v2.normalize();

    return    (    fabs( norm2() ) > c_SmallEpsilon &&
                fabs( v1.norm2() ) > c_SmallEpsilon &&
                fabs( v2.norm2() ) > c_SmallEpsilon);
} // Vector3D::orthonormalize

bool Vector3D::CreateBasis( Vector3D& vx, Vector3D& vy )
{
    Vector3D vz( *this );
    if (fabs( x ) < c_SmallEpsilon) vx = Vector3D::oX;
    else if (fabs( y ) < c_SmallEpsilon) vx = Vector3D::oY;
    else if (fabs( z ) < c_SmallEpsilon) vx = Vector3D::oZ;
    else vx = Vector3D::oX;

    vy.cross( *this, vx );
    orthonormalize( *this, vx, vy );
    return true;
} // Vector3D::CreateBasis

Vector3D Vector3D::GetRandomDir()
{
    float r1 = rndValuef();
    float r2 = rndValuef();
    float m  = sqrtf( r2 * (1.0f - r2) );
    Vector3D res( 2.0f * cosf( c_DoublePI * r1 )*m, 
        2.0f * sinf( c_DoublePI * r1 )*m, 
        1.0f - 2.0f*r2 );
    return res;
} // Vector3D::GetRandomDir

Vector3D Vector3D::GetRandomPos( const AABoundBox& aabb )
{
    Vector3D res;
    res.random( aabb.minv.x, aabb.maxv.x, 
                aabb.minv.y, aabb.maxv.y,
                aabb.minv.z, aabb.maxv.z );
    return res;
} // Vector3D::GetRandomPos

/*****************************************************************
/*    Vector4D implementation
/*****************************************************************/
const    Vector4D Vector4D::oX( 1.0f, 0.0f, 0.0f );
const    Vector4D Vector4D::oY( 0.0f, 1.0f, 0.0f );
const    Vector4D Vector4D::oZ( 0.0f, 0.0f, 1.0f );
const    Vector4D Vector4D::null( 0.0f, 0.0f, 0.0f, 0.0f );

void Vector4D::txtSave( FILE* fp )
{
    fprintf( fp, "={ %-8.8f, %-8.8f, %-8.8f, %-8.8f }\n", x, y, z, w );
}

/*****************************************************************************/
/*    Line3D implementation
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

/*****************************************************************************/
/*    Globals implementation
/*****************************************************************************/
DIALOGS_API DWORD VectorToColor( const Vector3D& v, float h )
{
    int r = (int)( 127.0f * v.x + 128.0f );
    int g = (int)( 127.0f * v.y + 128.0f );
    int b = (int)( 127.0f * v.z + 128.0f );
    int a = (int)( 255.0f * h );
    clamp( r, 0, (int)255 );
    clamp( g, 0, (int)255 );
    clamp( b, 0, (int)255 );
    clamp( a, 0, (int)255 );

    return( (a<<24L) + (r<<16L) + (g<<8L) + (b<<0L) );
} // VectorToColor

DIALOGS_API Vector3D ToTangentSpace( const Vector3D& ldir, const Vector3D& normal, const Vector3D& right )
{
    Vector3D n = normal;
    Vector3D t = right;
    Vector3D b;
    b.cross( n, t );
    b.normalize();
    t.cross( b, n );
    t.normalize();
    n.normalize();

    Matrix3D toTS;
    toTS.e00 = t.x; toTS.e10 = t.y; toTS.e20 = t.z;
    toTS.e01 = b.x; toTS.e11 = b.y; toTS.e21 = b.z;
    toTS.e02 = n.x; toTS.e12 = n.y; toTS.e22 = n.z;
    Vector3D res( ldir );
    res *= toTS;
    return res;
} // GetTangentSpaceLight






