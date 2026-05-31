/*****************************************************************************/
/*    File:    mTriangle.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    07-21-2003
/*****************************************************************************/
#include "stdafx.h"
#include "mTriangle.h"

#ifndef _INLINES
#include "mTriangle.inl"
#endif // _INLINES

/*****************************************************************************/    
/*    Triangle2D implementation
/*****************************************************************************/
Triangle2D::Triangle2D()
{}

Triangle2D::Triangle2D( float x0, float y0, 
                        float x1, float y1, 
                        float x2, float y2 )
{
    v[0].x = x0; v[0].y = y0;
    v[1].x = x1; v[1].y = y1;
    v[2].x = x2; v[2].y = y2;
}

void Triangle2D::Extrude( float amount )
{
    Vector2D c = GetCenter();
    for (int i = 0; i < 3; i++)
    {
        Vector2D d( v[i] );
        d -= c;
        d.normalize();
        d *= amount;
        v[i] += d;
    }
} // Triangle2D::Extrude

int Triangle2D::SortByXY( const void *pV1, const void *pV2 )
{
    Vector2D* v1 = (Vector2D*)pV1;
    Vector2D* v2 = (Vector2D*)pV2;

    if (v1->x < v2->x) return -1;
    if (v1->x > v2->x) return 1;
    if (v1->y < v2->y) return -1;
    if (v1->y > v2->y) return 1;
    return 0;
} // Triangle2D::SortByXY

Vector3D Triangle2D::CalcBaryCoords( const Vector2D& pt )
{
    return BaryCoords( v[0].x, v[0].y, v[1].x, v[1].y, v[2].x, v[2].y, pt.x, pt.y );
} // Triangle2D::CalcBaryCoords

DIALOGS_API Vector3D BaryCoords( float ax, float ay, float bx, float by, float cx, float cy,
                        float ptX, float ptY )
{
    float acx    = ax - cx;
    float acy    = ay - cy;
    float bcx    = bx - cx;
    float bcy    = by - cy;
    float pcx    = ptX - cx;
    float pcy    = ptY - cy;
    float m00     = acx*acx + acy*acy;
    float m01     = acx*bcx + acy*bcy;
    float m11     = bcx*bcx + bcy*bcy;
    float r0      = acx*pcx + acy*pcy;
    float r1      = bcx*pcx + bcy*pcy;
    float det     = m00 * m11 - m01 * m01;
    assert( fabs( det ) > 0.0f );
    float invDet = 1.0f / det;

    Vector3D res;
    res.x = (m11 * r0 - m01 * r1) * invDet;
    res.y = (m00 * r1 - m01 * r0) * invDet;
    res.z = 1.0f - res.x - res.y;
    return res;
} // BaryCoords

DIALOGS_API bool BaryCoords( float ax, float ay, float bx, float by, float cx, float cy,
                    float ptX, float ptY, Vector3D& res )
{
    float acx    = ax - cx;
    float acy    = ay - cy;
    float bcx    = bx - cx;
    float bcy    = by - cy;
    float pcx    = ptX - cx;
    float pcy    = ptY - cy;
    float m00     = acx*acx + acy*acy;
    float m01     = acx*bcx + acy*bcy;
    float m11     = bcx*bcx + bcy*bcy;
    float r0      = acx*pcx + acy*pcy;
    float r1      = bcx*pcx + bcy*pcy;
    float det     = m00 * m11 - m01 * m01;
    if (fabs( det ) < c_SmallEpsilon) return false;
    float invDet = 1.0f / det;

    res.x = (m11 * r0 - m01 * r1) * invDet;
    res.y = (m00 * r1 - m01 * r0) * invDet;
    res.z = 1.0f - res.x - res.y;
    return true;
} // BaryCoords

double c_DoubleEpsilon = 1.0e-16;
bool BaryCoords(    double ax, double ay, 
                    double bx, double by, 
                    double cx, double cy,
                    double ptX, double ptY, 
                    double& bcX, double& bcY, double& bcZ )
{
    double acx    = ax - cx;
    double acy    = ay - cy;
    double bcx    = bx - cx;
    double bcy    = by - cy;
    double pcx    = ptX - cx;
    double pcy    = ptY - cy;
    double m00     = acx*acx + acy*acy;
    double m01     = acx*bcx + acy*bcy;
    double m11     = bcx*bcx + bcy*bcy;
    double r0      = acx*pcx + acy*pcy;
    double r1      = bcx*pcx + bcy*pcy;
    double det     = m00 * m11 - m01 * m01;
    if (fabs( det ) < c_DoubleEpsilon) return false;
    double invDet = 1.0 / det;

    bcX = (m11 * r0 - m01 * r1) * invDet;
    bcY = (m00 * r1 - m01 * r0) * invDet;
    bcZ = 1.0 - bcX - bcY;
    return true;
} // BaryCoords



