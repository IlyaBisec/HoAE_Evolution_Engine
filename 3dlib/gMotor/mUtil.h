/*****************************************************************
/*  File:   mUtil.h                                             
/*    Desc:    Different math-related helpers                          
/*  Author: Silver, Copyright (C) GSC Game World                  
/*  Date:   January 2002                                          
/*****************************************************************/
#ifndef __MUTIL_H__
#define __MUTIL_H__

#include <math.h>
#include "mConstants.h"

template <class T> T tmin( const T& a, const T& b )
{
    return (a < b) ? a : b;
}

template <class T> T tmax( const T& a, const T& b )
{
    return (a > b) ? a : b;
}

template <class T> T tmin( const T& a, const T& b, const T& c )
{
    return tmin( tmin( a, b ), c );
}

template <class T> T tmax( const T& a, const T& b, const T& c )
{
    return tmax( tmax( a, b ), c );
}

template <class T> T tmin( const T& a, const T& b, const T& c, const T& d )
{
    return tmin( tmin( a, b ), tmin( c, d ) );
}

template <class T> T tmax( const T& a, const T& b, const T& c, const T& d )
{
    return tmax( tmax( a, b ), tmax( c, d ) );
}

template <class T> T tmin( const T& a, const T& b, const T& c, const T& d, const T& e )
{
    return tmin( tmin( a, b ), tmin( c, d, e ) );
}

template <class T> T tmax( const T& a, const T& b, const T& c, const T& d, const T& e )
{
    return tmax( tmax( a, b ), tmax( c, d, e ) );
}

template <class T> T tmin( const T& a, const T& b, const T& c, 
                           const T& d, const T& e, const T& f )
{
    return tmin( tmin( a, b, c ), tmin( d, e, f ) );
}

template <class T> T tmax( const T& a, const T& b, const T& c, 
                           const T& d, const T& e, const T& f )
{
    return tmax( tmax( a, b, c ), tmax( d, e, f ) );
}

inline float sign( float val )
{
    if (val < 0.0f) return -1.0f;
    if (val > 0.0f) return 1.0f;
    return 0.0f;
}

template <class T>
int argmin_idx( const T& a, const T& b )
{
    return a < b ? 0 : 1;
}

template <class T>
int argmin_idx( const T& a, const T& b, const T& c )
{
    return a < tmin( b, c ) ? 0 : argmin_idx( b, c ) + 1;
}

template <class T>
int argmin_idx( const T& a, const T& b, const T& c, const T& d )
{
    return tmin( a, b ) < tmin( c, d ) ? argmin_idx( a, b ) : argmin_idx( c, d ) + 2;
}

//  simple averaging smoothing 2D filter
template <typename T>
void SmoothFilter( T* arr, int width, int height );

template <typename T>
_inl bool clamp( T& val, const T& minv, const T& maxv )
{
    if (val < minv) val = minv;
    else if (val > maxv) val = maxv;
    else return false;
    return true;
}

template <typename TF, typename TM>
void SetFlag( TF& flags, TM flgMask, bool val )
{
    if (val) flags |= flgMask; else flags &= ~flgMask;
}

inline float FastInvSqrt( float f )
{
    float h = f * 0.5f;
    unsigned int y = 0x5F3759DF - ((unsigned int&)( f ) >> 1);
    (float&)y  = (float&)y*(1.5f - (h*(float&)y*(float&)y));
    return (float&)y;
} // FastInvSqrt

inline float RadToDeg( float rad ) 
{ 
    return 180.0f * rad / c_PI; 
}

inline float DegToRad( float deg ) 
{ 
    return c_PI * deg / 180.0f; 
}

inline float roundf( float val )
{
    double intP;
    return (fabs( modf( val, &intP ) ) < 0.5f) ? intP : intP + 1.0f;
}

inline bool equal( float a, float b )
{
    return (2.0f*fabs( a - b )/(a + b) < 1.0e-6f);
}

void InitMath();

extern "C" long _ftol( float arg ); 

#ifdef _INLINES
#include "mUtil.inl"
#endif // _INLINES

#endif // __MUTIL_H__