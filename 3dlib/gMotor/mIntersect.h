/*****************************************************************************/
/*    File:    mIntersect.h
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    07-10-2003
/*****************************************************************************/
#ifndef __MINTERSECT_H__
#define __MINTERSECT_H__

DIALOGS_API bool AABBTriangleX( const AABoundBox& aabb, 
                    const Vector3D& v0,
                    const Vector3D& v1,
                    const Vector3D& v2 );

DIALOGS_API bool AABBRayX(    const AABoundBox& aabb, 
                const Line3D& ray, Vector3D& point );

DIALOGS_API bool RayTriangleX(    const Line3D& ray, 
                    const Vector3D&    v0, 
                    const Vector3D&    v1, 
                    const Vector3D&    v2, 
                    float& u, float& v, 
                    float& t );

DIALOGS_API bool TriTri2DX( const Vector2D& a0, const Vector2D& a1, const Vector2D& a2, 
               const Vector2D& b0, const Vector2D& b1, const Vector2D& b2 );

DIALOGS_API bool RayTriangleX( const Vector3D& v1, const Vector3D& v2, const Vector3D& v3, 
                  const Vector3D& org, const Vector3D& dir, 
                  Vector3D& xpt );

#endif // __MINTERSECT_H__