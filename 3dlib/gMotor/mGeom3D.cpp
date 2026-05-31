/*****************************************************************************/
/*    File:    mGeom3D.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    09-25-2003
/*****************************************************************************/
#include "stdafx.h"
#include "mGeom3D.h"

#ifndef _INLINES
#include "mGeom3D.inl"
#endif // _INLINES

/*****************************************************************
/*  Cylinder implementation                                      
/*****************************************************************/
Cylinder::Cylinder()
{
}

Cylinder::Cylinder( const Cylinder& orig )
{
    m_Radius = orig.m_Radius;
    m_Core = orig.m_Core;
}

Cylinder::Cylinder( const Capsule& orig )
{
    m_Radius= orig.GetRadius();
    m_Core = orig.GetSegment();
}

Cylinder::Cylinder( const Segment3D& core, float radius )
{
     m_Core = core;
     m_Radius = radius;
}

void Cylinder::Random( const AABoundBox& bounds, float minR, float maxR )
{
    m_Core.Random( bounds, minR, maxR );
    m_Radius = rndValuef( minR, maxR );
} // Cylinder::Random

/*****************************************************************
/*  Capsule implementation                                     
/*****************************************************************/
void Capsule::Random( const AABoundBox& bounds, float minR, float maxR )
{
    seg.Random( bounds, minR, maxR );
    radius = rndValuef( minR, maxR );
} // Capsule::Random

/*****************************************************************
/*  AABoundBox implementation                                     
/*****************************************************************/
AABoundBox    AABoundBox::null( Vector3D::null, Vector3D::null );    
Vector3D AABoundBox::GetRandomPoint() const
{
    Vector3D res;
    res.x = rndValuef( minv.x, maxv.x );
    res.y = rndValuef( minv.y, maxv.y );
    res.z = rndValuef( minv.z, maxv.z );
    return res;
} // AABoundBox::GetRandomPoint

void AABoundBox::txtSave( FILE* fp )
{
    fprintf( fp, "minv(%-2.4f %-2.4f %-2.4f); maxv(%-2.4f %-2.4f %-2.4f); ", 
        minv.x, minv.y, minv.z, maxv.x, maxv.y, maxv.z );
}

void AABoundBox::Random(    const Vector3D& minpt,
                        const Vector3D& maxpt,
                        const Vector3D& maxdim )
{
    minv.x = rndValuef( minpt.x, maxpt.x );
    minv.y = rndValuef( minpt.y, maxpt.y );
    minv.z = rndValuef( minpt.z, maxpt.z );

    maxv.copy( minv );

    maxv.x += rndValuef( 0.0f, maxdim.x );
    maxv.y += rndValuef( 0.0f, maxdim.y );
    maxv.z += rndValuef( 0.0f, maxdim.z );
} // AABoundBox::Random

void AABoundBox::Random( const AABoundBox& bnd )
{
    minv.x = rndValuef( bnd.minv.x, bnd.maxv.x );
    minv.y = rndValuef( bnd.minv.y, bnd.maxv.y );
    minv.z = rndValuef( bnd.minv.z, bnd.maxv.z );

    maxv.x = rndValuef( minv.x, bnd.maxv.x );
    maxv.y = rndValuef( minv.y, bnd.maxv.y );
    maxv.z = rndValuef( minv.z, bnd.maxv.z );
} // AABoundBox::Random

void AABoundBox::Random( const AABoundBox& bnd, float minSide, float maxSide )
{
    minv.x = rndValuef( bnd.minv.x, bnd.maxv.x );
    minv.y = rndValuef( bnd.minv.y, bnd.maxv.y );
    minv.z = rndValuef( bnd.minv.z, bnd.maxv.z );

    maxv.copy( minv );

    maxv.x += rndValuef( minSide, maxSide );
    maxv.y += rndValuef( minSide, maxSide );
    maxv.z += rndValuef( minSide, maxSide );
} // AABoundBox::Random

/*****************************************************************
/*  Triangle implementation                                     
/*****************************************************************/
void Triangle::Random( const AABoundBox& aabb, float minSide, float maxSide )
{
    a = aabb.GetRandomPoint();
    b = Vector3D::GetRandomDir();
    c = Vector3D::GetRandomDir();
    b *= rndValuef( minSide, maxSide );
    c *= rndValuef( minSide, maxSide );
    b += a;
    c += a;
} // Triangle::Random

/*****************************************************************
/*  Frustum implementation                                     
/*****************************************************************/
void Frustum::FromCameraMatrix( const Matrix4D& cm ) 
{
    plLeft.a     = cm.e03 + cm.e00;
    plLeft.b     = cm.e13 + cm.e10; 
    plLeft.c     = cm.e23 + cm.e20;
    plLeft.d     = cm.e33 + cm.e30; 

    plRight.a     = cm.e03 - cm.e00; 
    plRight.b     = cm.e13 - cm.e10; 
    plRight.c     = cm.e23 - cm.e20;
    plRight.d     = cm.e33 - cm.e30; 

    plBottom.a     = cm.e03 + cm.e01; 
    plBottom.b     = cm.e13 + cm.e11; 
    plBottom.c     = cm.e23 + cm.e21;
    plBottom.d     = cm.e33 + cm.e31; 

    plTop.a         = cm.e03 - cm.e01; 
    plTop.b         = cm.e13 - cm.e11; 
    plTop.c         = cm.e23 - cm.e21;
    plTop.d         = cm.e33 - cm.e31; 

    plNear.a     = cm.e02; 
    plNear.b     = cm.e12; 
    plNear.c     = cm.e22;
    plNear.d     = cm.e32;

    plFar.a         = cm.e03 - cm.e02; 
    plFar.b         = cm.e13 - cm.e12; 
    plFar.c         = cm.e23 - cm.e22;
    plFar.d         = cm.e33 - cm.e32;

    plLeft.        normalize();
    plRight.    normalize();
    plBottom.    normalize();
    plTop.        normalize();
    plNear.        normalize();
    plFar.        normalize();
    
    UpdateVertices();
} // Frustum::FromCameraMatrix

void Frustum::Create( const Matrix4D& tm, float topW, float baseW, 
                      float wToH, float zn, float zf )
{
    float baseH = baseW / wToH;
    float topH  = topW  / wToH;
    float alpha = atanf( 0.5f*(baseW - topW)/(zf - zn) );
    float beta  = atanf( 0.5f*(baseH - topH)/(zf - zn) );
    float cosa  = cosf( alpha );
    float cosb  = cosf( beta  );
    float sina  = sinf( alpha );
    float sinb  = sinf( beta  );

    plLeft .    fromPointNormal( Vector3D( -topW*0.5f, 0.0f, zn ), Vector3D(  cosa, 0.0f,  sina ) );
    plRight.    fromPointNormal( Vector3D(  topW*0.5f, 0.0f, zn ), Vector3D( -cosa, 0.0f,  sina ) );
    plTop.        fromPointNormal( Vector3D( 0.0f, -topH*0.5f, zn ), Vector3D( 0.0f,  cosb, sinb ) );
    plBottom.    fromPointNormal( Vector3D( 0.0f,  topH*0.5f, zn ), Vector3D( 0.0f, -cosb, sinb ) );
    plNear.        fromPointNormal( Vector3D( 0.0f, 0.0f, zn ), Vector3D::oZ );
    plFar.        fromPointNormal( Vector3D( 0.0f, 0.0f, zf ), Vector3D( 0.0f, 0.0f, -1.0f ) );

    plLeft.        Transform( tm );
    plRight.    Transform( tm );
    plBottom.    Transform( tm );
    plTop.        Transform( tm );
    plNear.        Transform( tm );
    plFar.        Transform( tm );

    UpdateVertices();
} // Frustum::Create

int Frustum::Intersection( const Plane& plane, Vector3D* v )
{
    /*int nV = 0;
    if (plane.Intersect( plLeft,    plTop,        v[nV] ) && PtIn( v[nV] ) ) nV++;
    if (plane.Intersect( plTop,        plRight,    v[nV] ) && PtIn( v[nV] ) ) nV++;
                                                                 
    if (plane.Intersect( plLeft,    plFar,        v[nV] ) && PtIn( v[nV] ) ) nV++;
    if (plane.Intersect( plFar,        plRight,    v[nV] ) && PtIn( v[nV] ) ) nV++;
                                                                 
    if (plane.Intersect( plRight,    plBottom,    v[nV] ) && PtIn( v[nV] ) ) nV++;
    if (plane.Intersect( plBottom,  plLeft,        v[nV] ) && PtIn( v[nV] ) ) nV++;
                                                                 
    if (plane.Intersect( plRight,    plNear,        v[nV] ) && PtIn( v[nV] ) ) nV++;
    if (plane.Intersect( plNear,    plLeft,        v[nV] ) && PtIn( v[nV] ) ) nV++;
        
    return nV;*/


    int nV = 0;
    if (plane.Intersect( plLeft,    plTop,        v[nV] )) nV++;
    if (plane.Intersect( plTop,        plRight,    v[nV] )) nV++;
    if (plane.Intersect( plRight,    plBottom,    v[nV] )) nV++;
    if (plane.Intersect( plBottom,  plLeft,        v[nV] )) nV++;
    Vector3D c = v[0]; c += v[1]; c += v[2]; c += v[3]; c *= 0.25f;
    if (PtIn( c )) return 4;

    nV = 0;
    if (plane.Intersect( plLeft,    plFar,        v[nV] )) nV++;
    if (plane.Intersect( plFar,        plRight,    v[nV] )) nV++;
    if (plane.Intersect( plRight,    plNear,        v[nV] )) nV++;
    if (plane.Intersect( plNear,    plLeft,        v[nV] )) nV++;
    c = v[0]; c += v[1]; c += v[2]; c += v[3]; c *= 0.25f;
    /*if (PtIn( c )) return 4;

    nV = 0;
    if (plane.Intersect( plLeft,    plFar,        v[nV] )) nV++;
    if (plane.Intersect( plFar,        plRight,    v[nV] )) nV++;
    if (plane.Intersect( plRight,    plBottom,    v[nV] )) nV++;
    if (plane.Intersect( plBottom,    plLeft,        v[nV] )) nV++;
    c = v[0]; c += v[1]; c += v[2]; c += v[3]; c *= 0.25f;
    if (PtIn( c ))*/ return 4;

    return 0;
} // Frustum::Intersection

void Frustum::UpdateVertices()
{
    plLeft. Intersect( plTop,     plNear,    vLTN );
    plRight.Intersect( plTop,     plNear,    vRTN );
    plLeft. Intersect( plBottom, plNear,    vLBN );
    plRight.Intersect( plBottom, plNear,    vRBN );
    plLeft. Intersect( plTop,     plFar,        vLTF );
    plRight.Intersect( plTop,     plFar,        vRTF );
    plLeft. Intersect( plBottom, plFar,        vLBF );
    plRight.Intersect( plBottom, plFar,        vRBF );
} // Frustum::UpdateVertices
