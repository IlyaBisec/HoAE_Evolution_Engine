/*****************************************************************************/
/*    File:    mSphere.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    09-25-2003
/*****************************************************************************/
#include "stdafx.h"
#include "mSphere.h"

#ifndef _INLINES
#include "mSphere.inl"
#endif // _INLINES

/*****************************************************************
/*  Sphere implementation                                     
/*****************************************************************/
const Sphere Sphere::null = Sphere( Vector3D::null, 0.0f );
void Sphere::Random( const AABoundBox& aabb, float minR, float maxR )
{
    m_Center.x = rndValuef( aabb.minv.x, aabb.maxv.x );
    m_Center.y = rndValuef( aabb.minv.y, aabb.maxv.y );
    m_Center.z = rndValuef( aabb.minv.z, aabb.maxv.z );
    m_Radius   = rndValuef( minR, maxR );
} // Sphere::Random
