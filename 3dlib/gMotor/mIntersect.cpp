/*****************************************************************************/
/*    File:    mIntersect.cpp
/*    Desc:    Geometrical primitives intersection 
/*    Date:    07-10-2003
/*****************************************************************************/
#include "stdafx.h"
#include <math.h>
#include <stdio.h>
#include "mMath3D.h"

namespace intersect{
int triBoxOverlap(float boxcenter[3],float boxhalfsize[3],float triverts[3][3]);
int intersect_triangle(float orig[3], float dir[3],
                   float vert0[3], float vert1[3], float vert2[3],
                   float *t, float *u, float *v);
char HitBoundingBox(    float minB[3], float maxB[3],
                        float origin[3], float dir[3],    
                        float coord[3] );    
bool TriTriIntersect2D(const Vector2D akV0[3], const Vector2D akV1[3]);
}; // namespace intersect

DIALOGS_API bool RayTriangleX(    const Line3D& ray, 
                    const Vector3D&    v0, 
                    const Vector3D&    v1,
                    const Vector3D&    v2, 
                    float& u, float& v, 
                    float& t )
{
    float orig[3], dir[3], fv0[3], fv1[3], fv2[3];
    orig[0] = ray.getOrig().x;
    orig[1] = ray.getOrig().y;
    orig[2] = ray.getOrig().z;

    dir[0] = ray.getDir().x;
    dir[1] = ray.getDir().y;
    dir[2] = ray.getDir().z;

    fv0[0] = v0.x; fv0[1] = v0.y; fv0[2] = v0.z;
    fv1[0] = v1.x; fv1[1] = v1.y; fv1[2] = v1.z;
    fv2[0] = v2.x; fv2[1] = v2.y; fv2[2] = v2.z;

    return (intersect::intersect_triangle( orig, dir, fv0, fv1, fv2, &t, &u, &v ) != 0);
} // RayTriangleX

DIALOGS_API bool AABBTriangleX( const AABoundBox& aabb, 
                    const Vector3D& v0,
                    const Vector3D& v1,
                    const Vector3D& v2 )
{
    Vector3D c;
    aabb.GetCenter( c );
    float v[3][3];
    v[0][0] = v0.x; v[0][1] = v0.y; v[0][2] = v0.z;
    v[1][0] = v1.x; v[1][1] = v1.y; v[1][2] = v1.z;
    v[2][0] = v2.x; v[2][1] = v2.y; v[2][2] = v2.z;
    
    float bhsz[3];
    bhsz[0] = (aabb.maxv.x - aabb.minv.x)*0.5f;
    bhsz[1] = (aabb.maxv.y - aabb.minv.y)*0.5f;
    bhsz[2] = (aabb.maxv.z - aabb.minv.z)*0.5f;

    return (intersect::triBoxOverlap( (float*)&c, bhsz, v ) == 1);
} // AABBTriangleX

DIALOGS_API bool AABBRayX(    const AABoundBox& aabb, 
                const Line3D& ray, Vector3D& point )
{
    Vector3D orig, dir, minb, maxb;
    minb = aabb.minv;
    maxb = aabb.maxv;
    orig = ray.getOrig();
    dir = ray.getDir();
    float pt[3];

    if (intersect::HitBoundingBox( (float*)&minb, (float*)&maxb, 
                                    (float*)&orig, (float*)&dir, pt ) != 0)
    {
        point.x = pt[0];
        point.y = pt[1];
        point.z = pt[2];
        return true;
    }
    return false;
} // AABBRayX

DIALOGS_API bool TriTri2DX( const Vector2D& a0, const Vector2D& a1, const Vector2D& a2, 
                const Vector2D& b0, const Vector2D& b1, const Vector2D& b2 )
{
    Vector2D a[3];
    Vector2D b[3];
    a[0] = a0; a[1] = a1; a[2] = a2;
    b[0] = b0; b[1] = b1; b[2] = b2;
    
    return intersect::TriTriIntersect2D( a, b );
} // TriTri2DX

namespace intersect{

/*****************************************************************************/
/*    Triangle/AABB
/*****************************************************************************/
/********************************************************/
/* AABB-triangle overlap test code                      */
/* by Tomas Akenine-Möller                              */
/* Function: int triBoxOverlap(float boxcenter[3],      */
/*          float boxhalfsize[3],float triverts[3][3]); */
/* History:                                             */
/*   2001-03-05: released the code in its first version */
/*   2001-06-18: changed the order of the tests, faster */
/*                                                      */
/* Acknowledgement: Many thanks to Pierre Terdiman for  */
/* suggestions and discussions on how to optimize code. */
/* Thanks to David Hunt for finding a ">="-bug!         */
/********************************************************/
#define X 0
#define Y 1
#define Z 2

#define CROSS(dest,v1,v2) \
    dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
    dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
    dest[2]=v1[0]*v2[1]-v1[1]*v2[0];

#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])

#define SUB(dest,v1,v2) \
    dest[0]=v1[0]-v2[0]; \
    dest[1]=v1[1]-v2[1]; \
    dest[2]=v1[2]-v2[2];

#define FINDMINMAX(x0,x1,x2,min,max) \
    min = max = x0;   \
    if(x1<min) min=x1;\
    if(x1>max) max=x1;\
    if(x2<min) min=x2;\
    if(x2>max) max=x2;

int planeBoxOverlap(float normal[3],float d, float maxbox[3])
{
    int q;
    float vmin[3],vmax[3];
    for(q=X;q<=Z;q++)
    {
        if(normal[q]>0.0f)
        {
            vmin[q]=-maxbox[q];
            vmax[q]=maxbox[q];
        }
        else
        {
            vmin[q]=maxbox[q];
            vmax[q]=-maxbox[q];
        }
    }
    if(DOT(normal,vmin)+d>0.0f) return 0;
    if(DOT(normal,vmax)+d>=0.0f) return 1;

    return 0;
}


/*======================== X-tests ========================*/
#define AXISTEST_X01(a, b, fa, fb)             \
    p0 = a*v0[Y] - b*v0[Z];                    \
    p2 = a*v2[Y] - b*v2[Z];                    \
    if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
    rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z];   \
    if(min>rad || max<-rad) return 0;

#define AXISTEST_X2(a, b, fa, fb)              \
    p0 = a*v0[Y] - b*v0[Z];                    \
    p1 = a*v1[Y] - b*v1[Z];                    \
    if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
    rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z];   \
    if(min>rad || max<-rad) return 0;

/*======================== Y-tests ========================*/
#define AXISTEST_Y02(a, b, fa, fb)             \
    p0 = -a*v0[X] + b*v0[Z];                   \
    p2 = -a*v2[X] + b*v2[Z];                       \
    if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
    rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z];   \
    if(min>rad || max<-rad) return 0;

#define AXISTEST_Y1(a, b, fa, fb)              \
    p0 = -a*v0[X] + b*v0[Z];                   \
    p1 = -a*v1[X] + b*v1[Z];                       \
    if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
    rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z];   \
    if(min>rad || max<-rad) return 0;

/*======================== Z-tests ========================*/

#define AXISTEST_Z12(a, b, fa, fb)             \
    p1 = a*v1[X] - b*v1[Y];                    \
    p2 = a*v2[X] - b*v2[Y];                    \
    if(p2<p1) {min=p2; max=p1;} else {min=p1; max=p2;} \
    rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y];   \
    if(min>rad || max<-rad) return 0;

#define AXISTEST_Z0(a, b, fa, fb)              \
    p0 = a*v0[X] - b*v0[Y];                \
    p1 = a*v1[X] - b*v1[Y];                    \
    if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
    rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y];   \
    if(min>rad || max<-rad) return 0;

int triBoxOverlap(float boxcenter[3],float boxhalfsize[3],float triverts[3][3])
{

    /*    use separating axis theorem to test overlap between triangle and box */
    /*    need to test for overlap in these directions: */
    /*    1) the {x,y,z}-directions (actually, since we use the AABB of the triangle */
    /*       we do not even need to test these) */
    /*    2) normal of the triangle */
    /*    3) crossproduct(edge from tri, {x,y,z}-directin) */
    /*       this gives 3x3=9 more tests */
    float v0[3],v1[3],v2[3];
    float min,max,d,p0,p1,p2,rad,fex,fey,fez;
    float normal[3],e0[3],e1[3],e2[3];

    /* This is the fastest branch on Sun */
    /* move everything so that the boxcenter is in (0,0,0) */
    SUB(v0,triverts[0],boxcenter);
    SUB(v1,triverts[1],boxcenter);
    SUB(v2,triverts[2],boxcenter);

    /* compute triangle edges */
    SUB(e0,v1,v0);      /* tri edge 0 */
    SUB(e1,v2,v1);      /* tri edge 1 */
    SUB(e2,v0,v2);      /* tri edge 2 */

    /* Bullet 3:  */
    /*  test the 9 tests first (this was faster) */
    fex = fabs(e0[X]);
    fey = fabs(e0[Y]);
    fez = fabs(e0[Z]);
    AXISTEST_X01(e0[Z], e0[Y], fez, fey);
    AXISTEST_Y02(e0[Z], e0[X], fez, fex);
    AXISTEST_Z12(e0[Y], e0[X], fey, fex);

    fex = fabs(e1[X]);
    fey = fabs(e1[Y]);
    fez = fabs(e1[Z]);
    AXISTEST_X01(e1[Z], e1[Y], fez, fey);
    AXISTEST_Y02(e1[Z], e1[X], fez, fex);
    AXISTEST_Z0(e1[Y], e1[X], fey, fex);

    fex = fabs(e2[X]);
    fey = fabs(e2[Y]);
    fez = fabs(e2[Z]);
    AXISTEST_X2(e2[Z], e2[Y], fez, fey);
    AXISTEST_Y1(e2[Z], e2[X], fez, fex);
    AXISTEST_Z12(e2[Y], e2[X], fey, fex);

    /* Bullet 1: */
    /*  first test overlap in the {x,y,z}-directions */
    /*  find min, max of the triangle each direction, and test for overlap in */
    /*  that direction -- this is equivalent to testing a minimal AABB around */
    /*  the triangle against the AABB */

    /* test in X-direction */
    FINDMINMAX(v0[X],v1[X],v2[X],min,max);
    if(min>boxhalfsize[X] || max<-boxhalfsize[X]) return 0;

    /* test in Y-direction */
    FINDMINMAX(v0[Y],v1[Y],v2[Y],min,max);
    if(min>boxhalfsize[Y] || max<-boxhalfsize[Y]) return 0;

    /* test in Z-direction */
    FINDMINMAX(v0[Z],v1[Z],v2[Z],min,max);
    if(min>boxhalfsize[Z] || max<-boxhalfsize[Z]) return 0;

    /* Bullet 2: */
    /*  test if the box intersects the plane of the triangle */
    /*  compute plane equation of triangle: normal*x+d=0 */
    CROSS(normal,e0,e1);
    d=-DOT(normal,v0);  /* plane eq: normal.x+d=0 */
    if(!planeBoxOverlap(normal,d,boxhalfsize)) return 0;

    return 1;   /* box and triangle overlaps */
}

/*****************************************************************************/
/*    Ray/Triangle
/*****************************************************************************/
#define EPSILON 0.000001f
#define TEST_CULL
#define CROSS(dest,v1,v2) \
    dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
    dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
    dest[2]=v1[0]*v2[1]-v1[1]*v2[0];
#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])
#define SUB(dest,v1,v2)\
dest[0]=v1[0]-v2[0]; \
dest[1]=v1[1]-v2[1]; \
dest[2]=v1[2]-v2[2]; 

int
intersect_triangle(float orig[3], float dir[3],
                   float vert0[3], float vert1[3], float vert2[3],
                   float *t, float *u, float *v)
{
    float edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
    float det,inv_det;

    /* find vectors for two edges sharing vert0 */
    SUB(edge1, vert1, vert0);
    SUB(edge2, vert2, vert0);

    /* begin calculating determinant - also used to calculate U parameter */
    CROSS(pvec, dir, edge2);

    /* if determinant is near zero, ray lies in plane of triangle */
    det = DOT(edge1, pvec);

#ifdef TEST_CULL           /* define TEST_CULL if culling is desired */
    if (det < EPSILON)
        return 0;

    /* calculate distance from vert0 to ray origin */
    SUB(tvec, orig, vert0);

    /* calculate U parameter and test bounds */
    *u = DOT(tvec, pvec);
    if (*u < 0.0f || *u > det)
        return 0;

    /* prepare to test V parameter */
    CROSS(qvec, tvec, edge1);

    /* calculate V parameter and test bounds */
    *v = DOT(dir, qvec);
    if (*v < 0.0f || *u + *v > det)
        return 0;

    /* calculate t, scale parameters, ray intersects triangle */
    *t = DOT(edge2, qvec);
    inv_det = 1.0f / det;
    *t *= inv_det;
    *u *= inv_det;
    *v *= inv_det;
#else                    /* the non-culling branch */
    if (det > -EPSILON && det < EPSILON)
        return 0;
    inv_det = 1.0f / det;

    /* calculate distance from vert0 to ray origin */
    SUB(tvec, orig, vert0);

    /* calculate U parameter and test bounds */
    *u = DOT(tvec, pvec) * inv_det;
    if (*u < 0.0f || *u > 1.0f)
        return 0;

    /* prepare to test V parameter */
    CROSS(qvec, tvec, edge1);

    /* calculate V parameter and test bounds */
    *v = DOT(dir, qvec) * inv_det;
    if (*v < 0.0f || *u + *v > 1.0f)
        return 0;

    /* calculate t, ray intersects triangle */
    *t = DOT(edge2, qvec) * inv_det;
#endif
    return 1;
}

/*****************************************************************************/
/*    Ray/AABB
/*****************************************************************************/
/* 
Fast Ray-Box Intersection
by Andrew Woo
from "Graphics Gems", Academic Press, 1990
*/

#define NUMDIM    3
#define RIGHT    0
#define LEFT    1
#define MIDDLE    2

char HitBoundingBox( float minB[NUMDIM], float maxB[NUMDIM],    /*box */
                     float origin[NUMDIM], float dir[NUMDIM],    /*ray */
                     float coord[NUMDIM] )                        /* hit point */
{
    char inside = TRUE;
    char quadrant[NUMDIM];
    register int i;
    int whichPlane;
    float maxT[NUMDIM];
    float candidatePlane[NUMDIM];

    /* Find candidate planes; this loop can be avoided if
    rays cast all from the eye(assume perpsective view) */
    for (i=0; i<NUMDIM; i++)
        if(origin[i] < minB[i]) {
            quadrant[i] = LEFT;
            candidatePlane[i] = minB[i];
            inside = FALSE;
        }else if (origin[i] > maxB[i]) {
            quadrant[i] = RIGHT;
            candidatePlane[i] = maxB[i];
            inside = FALSE;
        }else    {
            quadrant[i] = MIDDLE;
        }

        /* Ray origin inside bounding box */
        if(inside)    {
            coord = origin;
            return (TRUE);
        }


        /* Calculate T distances to candidate planes */
        for (i = 0; i < NUMDIM; i++)
            if (quadrant[i] != MIDDLE && dir[i] !=0.)
                maxT[i] = (candidatePlane[i]-origin[i]) / dir[i];
            else
                maxT[i] = -1.;

        /* Get largest of the maxT's for final choice of intersection */
        whichPlane = 0;
        for (i = 1; i < NUMDIM; i++)
            if (maxT[whichPlane] < maxT[i])
                whichPlane = i;

        /* Check final candidate actually inside box */
        if (maxT[whichPlane] < 0.) return (FALSE);
        for (i = 0; i < NUMDIM; i++)
            if (whichPlane != i) {
                coord[i] = origin[i] + maxT[whichPlane] *dir[i];
                if (coord[i] < minB[i] || coord[i] > maxB[i])
                    return (FALSE);
            } else {
                coord[i] = candidatePlane[i];
            }
            return (TRUE);                /* ray hits box */
}    

/*****************************************************************************/
/*    2Dtri/2Dtri
/*    From David's Eberly Magic library
/*****************************************************************************/
int WhichSide (const Vector2D akV[3], 
               const Vector2D& rkP,
               const Vector2D& rkD)
{
    // Vertices are projected to the form P+t*D.  Return value is +1 if all
    // t > 0, -1 if all t < 0, 0 otherwise, in which case the line splits the
    // triangle.

    int iPositive = 0, iNegative = 0, iZero = 0;

    for (int i = 0; i < 3; i++)
    {
        float fT = rkD.x * (akV[i].x - rkP.x) + rkD.y * (akV[i].y - rkP.y);
        if ( fT > 0.0f )
            iPositive++;
        else if ( fT < 0.0f )
            iNegative++;
        else
            iZero++;

        if ( iPositive > 0 && iNegative > 0 )
            return 0;
    }

    return ( iZero == 0 ? ( iPositive > 0 ? 1 : -1 ) : 0 );
} // WhichSide

bool TriTriIntersect2D(const Vector2D akV0[3], const Vector2D akV1[3])
{
    int iI0, iI1;
    Vector2D kD;

    // test edges of V0-triangle for separation
    for (iI0 = 0, iI1 = 2; iI0 < 3; iI1 = iI0, iI0++)
    {
        // test axis V0[i1] + t*perp(V0[i0]-V0[i1]), perp(x,y) = (y,-x)
        kD.x = akV0[iI0].y - akV0[iI1].y;
        kD.y = akV0[iI1].x - akV0[iI0].x;
        if ( WhichSide(akV1,akV0[iI1],kD) > 0 )
        {
            // V1-triangle is entirely on positive side of V0-triangle
            return false;
        }
    }

    // test edges of V1-triangle for separation
    for (iI0 = 0, iI1 = 2; iI0 < 3; iI1 = iI0, iI0++)
    {
        // test axis V1[i1] + t*perp(V1[i0]-V1[i1]), perp(x,y) = (y,-x)
        kD.x = akV1[iI0].y - akV1[iI1].y;
        kD.y = akV1[iI1].x - akV1[iI0].x;
        if ( WhichSide(akV0,akV1[iI1],kD) > 0 )
        {
            // V0-triangle is entirely on positive side of V1-triangle
            return false;
        }
    }

    return true;
} // TriTriIntersect2D

};  // namespace intersect


//--------------------------------------------------------------------------
//    Func:    RayTriangleX    
//    Desc:    Checks whether ray intersects triangle
//            From "Realistic ray tracing", by Peter Shirley
//    Parm:    v1, v2, v3 - triangle vertices
//            org, dir - ray origin/direction
//            @xpt - intersection point
//    Ret:    true when ray intersects triangle
//--------------------------------------------------------------------------
DIALOGS_API bool RayTriangleX( const Vector3D& v1, const Vector3D& v2, const Vector3D& v3, 
                    const Vector3D& org, const Vector3D& dir, 
                    Vector3D& xpt ) 
{
    float a = v1.x - v2.x;
    float b = v1.y - v2.y;
    float c = v1.z - v2.z;

    float d = v1.x - v3.x;
    float e = v1.y - v3.y;
    float f = v1.z - v3.z;

    float g = dir.x;
    float h = dir.y;
    float i = dir.z;

    float j = v1.x - org.x;
    float k = v1.y - org.y;
    float l = v1.z - org.z;

    float ei_hf = e*i - h*f;
    float gf_di = g*f - d*i;
    float dh_eg = d*h - e*g;
    float ak_jb = a*k - j*b;
    float jc_al = j*c - a*l;
    float bl_kc = b*l - k*c;

    float M        =    a*ei_hf + b*gf_di + c*dh_eg;
    float beta    =  (j*ei_hf + k*gf_di + l*dh_eg) / M;
    float gamma    =  (i*ak_jb + h*jc_al + g*bl_kc) / M;
    float t        = -(f*ak_jb + e*jc_al + d*bl_kc) / M;

    if ((beta + gamma    <  1.0f)&& 
       (beta            >  0.0f)&& 
       (gamma            >  0.0f)&& 
       (t                >= 0.0f)&& 
       (t                <= 1.0f))
   {
        float alpha = 1 - beta - gamma;
        xpt.x = alpha*v1.x + beta*v2.x + gamma*v3.x;
        xpt.y = alpha*v1.y + beta*v2.y + gamma*v3.y;
        xpt.z = alpha*v1.z + beta*v2.z + gamma*v3.z;
        return true;
   }
    return false;
} // RayTriangleX


