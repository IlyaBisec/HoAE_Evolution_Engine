/*****************************************************************
/*  File:   mQuaternion.cpp                                          
/*  Author: Silver, Copyright (C) GSC Game World                  
/*  Date:   January 2002                                          
/*****************************************************************/
#include "stdafx.h"
#include "mQuaternion.h"

#ifndef _INLINES
#include "mQuaternion.inl"
#endif // !_INLINES
const Quaternion Quaternion::identity( 0.0f, 0.0f, 0.0f, 1.0f );

const float c_QuatEpsilon = 0.001f;
/*---------------------------------------------------------------*
/*  Func:    Quaternion::Slerp
/*    Desc:    Spherical linear interpolation between two quaternions
/*                using the shortest arc
/*    Parm:    src - source quaternion
/*            dst - destination quaternion
/*            t  - interpolation parameter, 0<=t<=1    
/*---------------------------------------------------------------*/
void Quaternion::Slerp(const Quaternion& src, const Quaternion& dst, float t, bool bShortest )
{    

    Quaternion tmp( dst );
    float cosTheta = src.dot( dst );

    if (cosTheta < 0.0f && bShortest)
    {
        cosTheta = -cosTheta;
        tmp.reverse();
    }

    float t1, t2; 
    if (1.0f - cosTheta > c_Epsilon) 
    {
        float theta = acosf( cosTheta );
        float invSinTheta = 1.0f / sin( theta );
        t1 = sinf( (1.0f - t) * theta);
        t2 = sinf( t * theta );

        v.x = t1 * src.v.x    + t2 * tmp.v.x;
        v.y = t1 * src.v.y    + t2 * tmp.v.y;
        v.z = t1 * src.v.z    + t2 * tmp.v.z;
        s    = t1 * src.s    + t2 * tmp.s;

        v *= invSinTheta;
        s *= invSinTheta;
    } 
    else 
    {
        // do simple linear interpolation, because there is no reason for slerp
        t1 = 1.0f - t;
        t2 = t;

        v.x = t1 * src.v.x    + t2 * tmp.v.x;
        v.y = t1 * src.v.y    + t2 * tmp.v.y;
        v.z = t1 * src.v.z    + t2 * tmp.v.z;
        s    = t1 * src.s    + t2 * tmp.s;
    }
} // Quaternion::Slerp

void Quaternion::Nlerp(const Quaternion& src, const Quaternion& dst, float t, bool bShortest )
{    
    Quaternion& result = *this;
    float cs = src.dot( dst );
    if (cs < 0.0f && bShortest)
    {
        result = dst;
        result.reverse();
        result -= src;
        result *= t;
        result += src;
    }
    else
    {
        result = dst;
        result -= src;
        result *= t;
        result += src;
    }
    result.normalize();
} // Quaternion::Nlerp



