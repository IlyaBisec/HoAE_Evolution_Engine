/*****************************************************************
/*  File:   mPlane.h                                             
/*  Desc:   3D plane code                                      
/*  Author: Silver, Copyright (C) GSC Game World                  
/*  Date:   January 2002                                          
/*****************************************************************/
#ifndef __MPLANE_H__
#define __MPLANE_H__

#include "mTransform.h"

typedef Line3D  Ray3D;
typedef Line3D    Segment3D;
/*****************************************************************
/*  Class:  Plane                                                
/*  Desc:   Plane in the 3D space      
/*  Rmrk:    Plane equation is 'ax + by + cz + d = 0'
/*****************************************************************/
class DIALOGS_API Plane
{
public:
    float                    a;
    float                    b;
    float                    c;

    float                    d;

    static Plane            xOz;
    static Plane            yOz;
    static Plane            xOy;
                            
                            Plane        () {}
                            Plane        ( const Vector3D& pt, const Vector3D& norm ) { fromPointNormal( pt, norm ); }
                            Plane        ( float _a, float _b, float _c, float _d ) : a(_a), b(_b), c(_c), d(_d){}
                            Plane        ( const Vector3D& a, const Vector3D& b, const Vector3D& c ) { from3Points( a, b, c ); }
    _inl float                from3Points    ( const Vector3D& v1, const Vector3D& v2, const Vector3D& v3 );
    bool                    Intersect    ( const Plane& p1, const Plane& p2, Vector3D& pt ) const;
    _inl bool                intersect    ( const Line3D& ray, Vector3D& pt )  const;
    
    _inl bool                ClipSegment    ( const Segment3D& ray, Vector3D& pt ) const;
    _inl bool                Contains    ( const Vector3D& pt, float eps = c_SpaceEpsilon ) const;
    
    //  moves plane to make it contain given point
    _inl void                MoveToPoint    ( const Vector3D& pt );
    //  moves plane along its normal direction
    _inl void                Move        ( float amt ) { d -= amt; }
    void                    Transform    ( const Matrix4D& tm );

    //  mirrors ray about the plane
    _inl Ray3D                Mirror        ( const Ray3D& ray ) const;
    //  reflects geometry about the plane
    Matrix4D                ReflectionTM() const;
    //  flattens geometry onto the plane
    Matrix4D                ProjectionTM( const Vector4D& dir ) const;
    //  projects vector onto the plane
    _inl void                ProjectVec    ( Vector3D& vec ) const;
    //  projects point onto the plane
    _inl void                ProjectPt    ( Vector3D& pt ) const;
    //  decomposes vector onto normal and plane coplanar components
    _inl void                Decompose    ( const Vector3D& vec, Vector3D& normC, Vector3D& projC ) const;

    _inl float                getZ        ( float x, float y ) const;
    _inl float                getY        ( float x, float z ) const;
    _inl float                getX        ( float y, float z ) const;
    
    _inl Vector3D&            normal        () { return *(reinterpret_cast<Vector3D*>( &a )); }
    _inl const Vector3D&    normal        () const { return *(const Vector3D*)&a; }
    _inl void                normalize    ();
    _inl Vector3D            GetPoint    () const;

    _inl Vector4D&            asVector    ();
    _inl const Vector4D&    asVector    () const;
    _inl void                txtSave        ( FILE* fp );
    _inl float                dist2Pt        ( const Vector3D& v ) const;

    _inl bool                isPerpendicular    ( const Plane& p ) const;
    _inl bool                OnPositiveSide    ( const Vector3D& v ) const;
    _inl void                fromPointNormal    ( const Vector3D& pt, const Vector3D& norm );
    _inl void                fromPointNormal    ( const Vector4D& pt, const Vector4D& norm );

}; // class Plane

#ifdef _INLINES
#include "mPlane.inl"
#endif // _INLINES

#endif // __MPLANE_H__