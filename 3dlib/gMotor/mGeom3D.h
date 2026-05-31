/*****************************************************************************/
/*    File:    mGeom3D.cpp
/*  Desc:    Platonic 3D geometry primitives
/*    Author:    Ruslan Shestopalyuk
/*    Date:    09-25-2003
/*****************************************************************************/
#ifndef __MGEOM3D_H__
#define __MGEOM3D_H__

class   AABoundBox;
class   Cylinder;
class   Capsule;
class   Frustum;
class   Sphere;
class   Triangle;

struct WSegment 
{
    Vector3D    beg;    
    Vector3D    end;
    float        width;
    Vector3D    normal;
}; // struct WSegment 

/*****************************************************************
/*  Class:  Cone                                             
/*  Desc:   3D cone                                             
/*****************************************************************/
class Cone
{
    Vector3D                m_Top;        //  top of the cone
    Vector3D                m_Base;        //  bottom of the cone
    float                    m_Angle;    //  angle of the cone

public:
    _inl                    Cone        (){}
    _inl                    Cone        ( const Vector3D& top, const Vector3D& base, float angle )
                                            : m_Top( top ), m_Base( base ), m_Angle( angle ) {}

    _inl const Vector3D&    GetTop            () const { return m_Top;   }
    _inl const Vector3D&    GetBase            () const { return m_Base;  }
    _inl float                GetAngle        () const { return m_Angle; }
    
    _inl float                GetBaseRadius    () const;
    _inl float                GetHeight        () const;
    _inl Matrix4D            GetWorldTM        () const;
    _inl Vector3D            GetDir            () const;
    _inl Vector3D            RandomDir        () const;

}; // class Cone

/*****************************************************************
/*  Class:  Cylinder                                          
/*  Desc:   Describes 3D cylinder                                 
/*****************************************************************/
class DIALOGS_API Cylinder
{
public:
                    Cylinder    ();
                    Cylinder    ( const Cylinder& orig );
                    Cylinder    ( const Segment3D& core, float radius );
                    Cylinder    ( const Capsule& orig );
    
    _inl bool        IsInfinite    () const;
    _inl void        SetInfinite    ();

    _inl Vector3D    GetBase        () const;
    _inl Vector3D    GetDir        () const;
    _inl Vector3D    GetTop        () const;

    _inl Vector3D    GetCenter    () const;
    _inl float        GetRadius    () const;
    _inl XStatus    Intersect    ( const Plane& plane  ) const;

    _inl float        GetHeight    () const;
    _inl Matrix4D    GetWorldTM  () const;

    _inl const Segment3D& GetCore() const { return m_Core; }


    void            Random        ( const AABoundBox& bounds, float minR, float maxR );

protected:
    Segment3D        m_Core;            //  cylinder core segment
    float            m_Radius;        //  cylinder radius
};  // class Cylinder

/*****************************************************************
/*  Class:  Capsule                                           
/*  Desc:   Cylinder, capped with hemispheres
/*****************************************************************/
class DIALOGS_API Capsule
{
public:
                    Capsule        () {} 
                    Capsule        ( const Vector3D& orig, const Vector3D& dir, float r ) :
                                    seg( orig, dir ), radius( r ) {}
    _inl Vector3D    GetBase        () const;
    _inl Vector3D    GetDir        () const;
    _inl Vector3D    GetTop        () const;

    _inl Vector3D    GetCenter    () const;
    _inl float        GetRadius    () const;
    _inl float        GetHeight    () const;
    _inl Matrix4D    GetWorldTM  () const;
    
    _inl const Segment3D& GetSegment() const { return seg; }

    _inl Sphere        GetTopSphere() const;
    _inl Sphere        GetBottomSphere() const;

    _inl XStatus    Intersect    ( const Plane& plane  ) const;
    void            Random        ( const AABoundBox& bounds, float minR, float maxR );

protected:
    Segment3D        seg;
    float            radius;
}; // class Capsule

/*****************************************************************
/*  Class:  Frustum                                              *
/*  Desc:   Describes visibility frustum                         *
/*****************************************************************/
class DIALOGS_API Frustum
{
public:
    Frustum() {}

    _inl const Vector3D&    ltn() const { return vLTN; }
    _inl const Vector3D&    rtn() const { return vRTN; }
    _inl const Vector3D&    lbn() const { return vLBN; }
    _inl const Vector3D&    rbn() const { return vRBN; }

    _inl const Vector3D&    ltf() const { return vLTF; }
    _inl const Vector3D&    rtf() const { return vRTF; }
    _inl const Vector3D&    lbf() const { return vLBF; }
    _inl const Vector3D&    rbf() const { return vRBF; }

    _inl bool                Overlap            ( const AABoundBox& aabb     ) const;
    _inl bool                Overlap            ( const Sphere&        sphere   ) const;
    _inl bool                Overlap            ( const Cylinder&    cylinder ) const;
    _inl bool                Overlap            ( const Capsule&    capsule  ) const;
    _inl XStatus            Intersect        ( const Vector3D& a, const Vector3D& b ) const;

    _inl void                Extrude            ( float val );
    _inl void               Extrude         ( float left, float top, float right, float bottom );

    _inl XStatus            Intersect        ( const AABoundBox& aabb ) const;
    int                        Intersection    ( const Plane& plane, Vector3D* v );
    void                    FromCameraMatrix( const Matrix4D& cm );     

    void                    Create            ( const Matrix4D& tm, 
                                                float topW, float baseW, 
                                                float wToH, 
                                                float zn, float zf );
    _inl bool                PtIn            ( const Vector3D& pt ) const;
    const Plane&            GetPlane        ( int idx ) const { return (&plNear)[idx]; }
    const Vector3D&         GetCorner       ( int idx ) const { return (&vLTN)[idx]; }

public:
    Plane                    plNear;        
    Plane                    plFar;        
    Plane                    plLeft;        
    Plane                    plRight;        
    Plane                    plTop;
    Plane                    plBottom;

private:
    void                    UpdateVertices();

    //  cached frustum vertices
    Vector3D                vLTN, vRTN, vLBN, vRBN, vLTF, vRTF, vLBF, vRBF;

};  // class Frustum

/*****************************************************************
/*  Class:  AABoundBox                                           
/*  Desc:   Axis-aligned bounding box 
/*****************************************************************/
class DIALOGS_API AABoundBox
{
public:
    _inl AABoundBox();
    _inl AABoundBox( const Vector3D& _minv, const Vector3D& _maxv );
    _inl AABoundBox( float minx, float miny, float minz, float maxx, float maxy, float maxz );
    _inl AABoundBox( const Vector3D& c ) : minv( c ), maxv( c ) {}
    _inl AABoundBox( const Vector3D& center, float halfSide );
    _inl AABoundBox( const Vector3D& center, float cx, float cy, float cz );
    _inl AABoundBox( const Rct& base, float minZ, float maxZ );

    _inl void        GetCenter        ( Vector3D& dest )    const;
    _inl Vector3D    GetCenter        ()    const;

    _inl float        GetDiagonal        ()                    const;
    _inl void        Union            ( const AABoundBox& orig );
    _inl void        operator +=        ( const AABoundBox& orig ) { Union( orig ); }    
    _inl bool       operator ==     ( const AABoundBox& orig ) const;

    _inl void        Copy            ( const AABoundBox& orig );

    _inl float        Distance        ( const Vector3D& p ) const;
    _inl float        Distance2        ( const Vector3D& p ) const;
    
    //  intersection tests
    _inl XStatus    Intersect         ( const Plane&        plane    ) const;
    _inl XStatus    Intersect         ( const AABoundBox& aabb    ) const;
    _inl XStatus    Intersect        ( const Vector3D& a, const Vector3D& b ) const;
    
    _inl Sphere        GetBoundSphere    () const;
    
    _inl bool        IsOutside           ( const AABoundBox& aabb    ) const;
    _inl bool        Overlap            ( const AABoundBox& aabb    ) const;
    _inl bool        Overlap            ( const Ray3D&        ray        ) const;
    _inl bool        OverlapSegment    ( const Segment3D&    seg        ) const;
    _inl bool        Overlap            ( const Sphere&        sphere    ) const;
    _inl void        Set                ( float minx, float miny, float minz, 
                                        float maxx, float maxy, float maxz );

    _inl void        Transform        ( const Matrix4D& matr );
    _inl void        GetCorners        ( Vector3D (&c)[8] ) const;
    
    void            Random            ( const Vector3D& minpt,
                                      const Vector3D& maxpt,
                                      const Vector3D& maxdim );
    void            Random            ( const AABoundBox& bnd );
    void            Random            ( const AABoundBox& bnd, float minSide, float maxSide );
    
    Vector3D        GetRandomPoint    () const;

    _inl void        Extend            ( const Vector3D& pt );
    _inl void        Extend            ( const Triangle& pt );
    _inl void        Extend            ( const AABoundBox& aabb );

    _inl bool        PtIn            ( const Vector3D& pt ) const;

    _inl float        GetDX            () const { return maxv.x - minv.x; }    
    _inl float        GetDY            () const { return maxv.y - minv.y; }    
    _inl float        GetDZ            () const { return maxv.z - minv.z; }    
    _inl bool        XIn                ( float x ) const { return x >= minv.x && x <= maxv.x; }
    _inl bool        YIn                ( float y ) const { return y >= minv.y && y <= maxv.y; }
    _inl bool        ZIn                ( float z ) const { return z >= minv.z && z <= maxv.z; }
    
    _inl Vector3D    xyz                () const { return minv; }
    _inl Vector3D    xyZ                () const { return Vector3D( minv.x, minv.y, maxv.z ); }
    _inl Vector3D    xYz                () const { return Vector3D( minv.x, maxv.y, minv.z ); }
    _inl Vector3D    xYZ                () const { return Vector3D( minv.x, maxv.y, maxv.z ); }
    _inl Vector3D    Xyz                () const { return Vector3D( maxv.x, minv.y, minv.z ); }
    _inl Vector3D    XyZ                () const { return Vector3D( maxv.x, minv.y, maxv.z ); }
    _inl Vector3D    XYz                () const { return Vector3D( maxv.x, maxv.y, minv.z ); }
    _inl Vector3D    XYZ                () const { return maxv; }

    _inl Plane        GetPlane        ( int idx ) const;

    void            txtSave            ( FILE* fp );

    Vector3D            minv;        //  "min" vertex of the bbox
    Vector3D            maxv;        //  "max" vertex of the bbox

    static AABoundBox    null;    
};  // class AABoundBox

/*****************************************************************
/*  Class:  Triangle                                           
/*  Desc:   3D triangle
/*****************************************************************/
class DIALOGS_API Triangle
{
public:
    Vector3D                a, b, c;
        
    _inl                    Triangle    (){}
    _inl                    Triangle    (    const Vector3D& _a,
                                            const Vector3D& _b,
                                            const Vector3D& _c ) : a(_a), b(_b), c(_c) {}
    _inl Vector3D&          GetV        ( int idx ) { return ((Vector3D*)this)[idx]; }
    _inl XStatus            Intersect     ( const Plane& plane ) const;
    _inl float              Distance    ( const Ray3D& ray );
    void                    Random        ( const AABoundBox& aabb, float minSide, float maxSide );
    _inl int                Clip        ( const Plane& pl, Vector3D (&pt)[2] );
    _inl Plane              GetPlane    () const;
    _inl AABoundBox         GetAABB        () const;

    _inl const Vector3D&    ArgminX        () const;
    _inl const Vector3D&    ArgmaxX        () const;
    _inl const Vector3D&    ArgminY        () const;
    _inl const Vector3D&    ArgmaxY        () const;
    _inl const Vector3D&    ArgminZ        () const;
    _inl const Vector3D&    ArgmaxZ        () const;    
}; // class Triangle

#ifdef _INLINES
#include "mGeom3D.inl"
#endif // _INLINES

#endif // __MGEOM3D_H__