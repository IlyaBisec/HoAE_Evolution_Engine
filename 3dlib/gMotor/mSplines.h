/*****************************************************************************/
/*    File:    mSplines.h
/*    Desc:    Spline curves routines
/*    Author:    Ruslan Shestopalyuk
/*    Date:    07-15-2003
/*****************************************************************************/
#ifndef __MSPLINES_H__
#define __MSPLINES_H__

_inl float LinearInterpolate        ( float t, float ta, float a, float tb, float b );
_inl float CosineInterpolate        ( float t, float a, float b );

_inl float    CatmullRomInterpolate    ( float t, float p0, float p1, float p2, float p3 );
_inl float    HermiteInterpolate        ( float t, float p0, float q0, float p1, float q1 );
_inl float    BSplineInterpolate        ( float t, float p0, float p1, float p2, float p3 );
_inl float    BezierInterpolate        ( float t, float p0, float p1, float p2, float p3 );

_inl float    CatmullRomTangent        ( float t, float p0, float p1, float p2, float p3 );
_inl float    HermiteTangent            ( float t, float p0, float q0, float p1, float q1 );
_inl float    BSplineTangent            ( float t, float p0, float p1, float p2, float p3 );
_inl float    BezierTangent            ( float t, float p0, float p1, float p2, float p3 );

_inl Vector3D    CatmullRomInterpolate    ( float t,    const Vector3D& p0, const Vector3D& p1,
                                                    const Vector3D& p2, const Vector3D& p3 );
_inl Vector3D    HermiteInterpolate        ( float t,    const Vector3D& p0, const Vector3D& q0,
                                                    const Vector3D& p1, const Vector3D& q1 );
_inl Vector3D    BSplineInterpolate        ( float t,    const Vector3D& p0, const Vector3D& p1,
                                                    const Vector3D& p2, const Vector3D& p3 );
_inl Vector3D    BezierInterpolate        ( float t,    const Vector3D& p0, const Vector3D& p1,
                                                    const Vector3D& p2, const Vector3D& p3 );

#ifdef _INLINES
#include "mSplines.inl"
#endif // _INLINES 

#endif  // __MSPLINES_H__
