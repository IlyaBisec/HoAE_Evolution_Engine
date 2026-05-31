/*****************************************************************
/*  File:   mPlane.cpp                                           
/*  Author: Silver, Copyright (C) GSC Game World                 
/*  Date:   January 2002                                         
/*****************************************************************/
#include "stdafx.h"
#include "mPlane.h"

#ifndef _INLINES
#include "mPlane.inl"
#endif // !_INLINES

Plane                    Plane::xOz = Plane( 0.0f, 1.0f, 0.0f, 0.0f );
Plane                    Plane::yOz = Plane( 1.0f, 0.0f, 0.0f, 0.0f );
Plane                    Plane::xOy = Plane( 0.0f, 0.0f, 1.0f, 0.0f );

void Plane::Transform( const Matrix4D& tm )
{
    Vector4D pt = GetPoint();
    Vector4D norm = pt;
    norm.x += normal().x;
    norm.y += normal().y;
    norm.z += normal().z;
    norm.w += 1.0f;

    pt *= tm;
    norm *= tm;

    pt.normW();
    norm.normW();

    norm -= pt;
    norm.w = 0.0f;
    norm.normalize();
    fromPointNormal( pt, norm );
} // Plane::Transform


/*---------------------------------------------------------------------------*/
/*    Func:    Plane::intersect
/*    Desc:    finds intersection point of three planes
/*    Parm:    p1 - 2nd plane
/*            p2 - 3rd plane
/*            pt - intersection point to return 
/*    Ret:    true if such point exist
/*---------------------------------------------------------------------------*/
bool Plane::Intersect( const Plane& p1, const Plane& p2, Vector3D& pt ) const
{
    double A[9];
    A[0] = a;
    A[1] = p1.a;
    A[2] = p2.a;

    A[3] = b;
    A[4] = p1.b;
    A[5] = p2.b;

    A[6] = c;
    A[7] = p1.c;
    A[8] = p2.c;

    double det;
    Inverse3x3( A, det );
    const double c_DetEpsilon = 0.0000000001;
    if (fabs( det ) < c_DetEpsilon) return false;

    pt.x = -d*A[0] - p1.d*A[3] - p2.d*A[6];
    pt.y = -d*A[1] - p1.d*A[4] - p2.d*A[7];
    pt.z = -d*A[2] - p1.d*A[5] - p2.d*A[8];

    return true;
} // Plane::Intersect

Matrix4D Plane::ReflectionTM() const
{
    Matrix4D m;

    m.e00 = -2.0f * a * a + 1.0f; 
    m.e01 = -2.0f * b * a;
    m.e02 = -2.0f * c * a;
    m.e03 = 0.0f;

    m.e10 = -2.0f * a * b;      
    m.e11 = -2.0f * b * b + 1.0f;  
    m.e12 = -2.0f * c * b;        
    m.e13 = 0.0f;

    m.e20 = -2.0f * a * c;     
    m.e21 = -2.0f * b * c;      
    m.e22 = -2.0f * c * c + 1.0f;    
    m.e23 = 0.0f;

    m.e30 = -2.0f * a * d;      
    m.e31 = -2.0f * b * d;      
    m.e32 = -2.0f * c * d;       
    m.e33 = 1.0f;

    return m;
} // Plane::ReflectionTM

Matrix4D Plane::ProjectionTM( const Vector4D& dir ) const
{
    Matrix4D m;
    float d = normal().dot( dir );
    m.e00 = a * dir.x + d; 
    m.e01 = a * dir.y;
    m.e02 = a * dir.z;
    m.e03 = a * dir.w;

    m.e10 = b * dir.x; 
    m.e11 = b * dir.y + d;
    m.e12 = b * dir.z;
    m.e13 = b * dir.w;

    m.e20 = c * dir.x; 
    m.e21 = c * dir.y;
    m.e22 = c * dir.z + d;  
    m.e23 = c * dir.w;

    m.e30 = d * dir.x; 
    m.e31 = d * dir.y;
    m.e32 = d * dir.z;
    m.e33 = d * dir.w + d;
    return m;
} // Plane::ProjectionTM







