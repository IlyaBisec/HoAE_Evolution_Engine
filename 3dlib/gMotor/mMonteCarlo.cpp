/*****************************************************************************/
/*    File:    mMonteCarlo.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    07-28-2003
/*****************************************************************************/
#include "stdafx.h"
#include "mMonteCarlo.h"

//  returns random vector in the hemisphere with axis cDir
//  (slow lame version)
Vector3D SampleHemisphereDirection( const Vector3D& cDir )
{
    Vector3D v;

    float r1 = rndValuef();
    float r2 = rndValuef();
    float k = sqrtf( 1.0f - r2*r2 );

    v.x = cos( 2.0f * c_PI * r1 ) * k;
    v.y = sin( 2.0f * c_PI * r1 ) * k;
    v.z = r2;
    
    return v;
} // SampleHemisphereDirection

Vector3D SampleSphereDirection()
{
    Vector3D v;
    float r1 = rndValuef();
    float r2 = rndValuef();
    float k  = sqrtf( r2 * (1.0f - r2) );
    v.x = 2.0f * cos( 2.0f * c_PI * r1 ) * k;
    v.y = 2.0f * sin( 2.0f * c_PI * r1 ) * k;
    v.z = 1.0f - 2.0f * r2;
    return v;
}


