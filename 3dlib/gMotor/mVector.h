/*****************************************************************
/*  File:   mVector.h                                             
/*    Desc:    Vector classes                                  
/*  Author: Silver, Copyright (C) GSC Game World                  
/*  Date:   January 2002                                          
/*****************************************************************/
#ifndef __MVECTOR_H__
#define __MVECTOR_H__

#ifndef DIALOGS_API
#ifndef DIALOGS_USER
#define DIALOGS_API __declspec(dllexport)
#else //DIALOGS_USER
#define DIALOGS_API __declspec(dllimport)
#endif //DIALOGS_USER
#endif //DIALOGS_API

class Matrix3D;
class Matrix4D;
class Vector4D;
class AABoundBox;
class Plane;

/*****************************************************************
/*  Class:  Vector3D                                              
/*****************************************************************/
class DIALOGS_API Vector3D             
{
public:
    float            x;
    float            y;
    float            z;

                            Vector3D        ( const Vector3D& orig ) : x(orig.x), y(orig.y), z(orig.z) {}
    _inl                    Vector3D        ( const Vector4D& orig );
                            Vector3D        ( float _x = 0.0f, float _y = 0.0f, float _z = 0.0f ) : x(_x), y(_y), z(_z) {}
                            Vector3D        ( float val ) { x = y = z = val; }

    void                    set                ( float _x, float _y, float _z ) { x = _x; y = _y; z = _z; }
    
    _inl void                sub                ( const Vector3D& v1, const Vector3D& v2 );
    _inl void                add                ( const Vector3D& v1, const Vector3D& v2 );
    _inl void                cross            ( const Vector3D& v1, const Vector3D& v2 );
    _inl void                sub                ( const Vector3D& v );
    _inl void                add                ( const Vector3D& v );
    _inl void                addWeighted        ( const Vector3D& v, float weight );
    _inl void                addWeighted        ( const Vector3D& v1, const Vector3D& v2, float w1, float w2 );
    _inl void               centroid        ( const Vector3D& v0, const Vector3D& v1, const Vector3D& v2 );
    _inl void                reverse            ( const Vector3D& orig );
    _inl void                round            ();
    _inl void                reverse            ();
    _inl void                random            ( float minX, float maxX, float minY, float maxY, float minZ, float maxZ );
    _inl float                dot                ( const Vector3D& v ) const;
    _inl void                FromSpherical    ( float rho, float theta, float phi );

    _inl const Vector3D&     operator +=        ( const Vector3D& vec );
    _inl const Vector3D&     operator -=        ( const Vector3D& vec );
    _inl const Vector3D&     operator *=        ( const float val );
    _inl const Vector3D&     operator *=        ( const Vector3D& vec );
    _inl const Vector3D&     operator *=        ( const Matrix3D& m );
    _inl const Vector3D&     operator *=        ( const Matrix4D& m );

    _inl const Vector3D&     mulLeft            ( const Matrix3D& m );
    _inl const Vector3D&     mulPt            ( const Vector3D& v, const Matrix4D& m );
    _inl const Vector3D&     mulPt            ( const Matrix4D& m );
    
    _inl const Vector3D&     operator /=        ( const float val );
    const Vector3D&         operator =        ( const Vector3D& orig ) { x = orig.x; y = orig.y; z = orig.z; return *this; }
    _inl const Vector3D&     operator =        ( const Vector4D& orig );

    _inl bool                isEqual            ( const Vector3D& orig, float eps = c_SpaceEpsilon ) const;
    _inl float                norm            () const;
    _inl float                norm2            () const;
    _inl float                normalize        ();
    _inl float                normalize_fast   ();
    _inl float                distance        ( const Vector3D& orig ) const;
    _inl float                distance2        ( const Vector3D& orig ) const;

    _inl void                copy            ( const Vector3D& orig );
    _inl void                copy            ( const Vector4D& orig );

    _inl float                Angle            ( const Vector3D& v );
    _inl float                Angle            ( const Vector3D& v, const Vector3D& n );

    _inl bool                isColinear        ( const Vector3D& orig, float eps = c_SpaceEpsilon ) const;
    _inl void                zero            ();
    void                    Dump            ();
    bool                    CreateBasis        ( Vector3D& vx, Vector3D& vy );
    void                    txtSave            ( FILE* fp, const char* name = 0 );

    bool                    orthonormalize  ( Vector3D& v1, Vector3D& v2 );
    static bool                orthonormalize    ( Vector3D& v1, Vector3D& v2, Vector3D& v3 );
    
    static  const            Vector3D oX;
    static  const            Vector3D oY;
    static  const            Vector3D oZ;
    static  const            Vector3D null;
    static  const            Vector3D one;

    static Vector3D         GetRandomDir();
    static Vector3D         GetRandomPos( const AABoundBox& aabb );

    friend Vector3D         operator-( const Vector3D& a, const Vector3D& b )
    {
        Vector3D res;
        res.x = a.x - b.x; res.y = a.y - b.y; res.z = a.z - b.z;
        return res; 
    }

    friend Vector3D         operator+( const Vector3D& a, const Vector3D& b )
    {
        Vector3D res;
        res.x = a.x + b.x; res.y = a.y + b.y; res.z = a.z + b.z;
        return res; 
    }

    friend Vector3D         operator*( const Vector3D& a, const Vector3D& b )
    {
        Vector3D res;
        res.x = a.x*b.x; res.y = a.y*b.y; res.z = a.z*b.z;
        return res; 
    }

    friend Vector3D         operator*( const Vector3D& a, float v )
    {
        Vector3D res;
        res.x = a.x*v;
        res.y = a.y*v;
        res.z = a.z*v;
        return res; 
    }

    friend Vector3D         operator*( float v, const Vector3D& a )
    {
        Vector3D res;
        res.x = a.x*v;
        res.y = a.y*v;
        res.z = a.z*v;
        return res; 
    }
}; // class Vector3D

/*****************************************************************
/*  Class:  Vector4D                                             *
/*  Desc:   4-dimensional vector                                 *
/*****************************************************************/
class DIALOGS_API Vector4D
{
public:
    float x, y, z, w;

    _inl                     Vector4D(){}
    _inl                    Vector4D( const Vector4D& orig );
    Vector4D( const Vector3D& v, float _w = 1.0f ) : x(v.x), y(v.y), z(v.z), w(_w) {}
    Vector4D( float _x, float _y, float _z ) : x(_x), y(_y), z(_z) {} 

    _inl                     Vector4D(    const float _x,
                                        const float _y, 
                                        const float _z,
                                        const float _w );

    _inl Vector4D&            mul( const Vector4D& v, const Matrix4D& m );
    _inl Vector4D&            mul( const Matrix4D& m, const Vector4D& v );
    _inl Vector4D&            mul( const Matrix4D& m );
    _inl Vector4D&            mulWeighted( const Matrix4D& m, float weight );
    _inl Vector4D&            operator *=( const Matrix4D& m );

    _inl Vector4D&          transform( const Matrix4D& m );

    _inl void                set( float _x, float _y, float _z );
    _inl void                set( float _x, float _y, float _z, float _w );
    _inl void                sub( const Vector4D& v1, const Vector4D& v2 );
    _inl void                add( const Vector4D& v1, const Vector4D& v2 );
    _inl void                sub( const Vector4D& v );
    _inl void                add( const Vector4D& v );

    _inl float                mul( const Vector4D& v );
    _inl float                dot( const Vector4D& v ) const;
    _inl bool                isEqual( const Vector4D& orig, float eps = c_SpaceEpsilon ) const;
    _inl void                random( float minX, float maxX, float minY, float maxY, float minZ, float maxZ );


    _inl const Vector4D&     operator +=( const Vector4D& vec );
    _inl const Vector4D&     operator -=( const Vector4D& vec );
    _inl const Vector4D&     operator *=( const float val );
    _inl const Vector4D&     operator /=( const float val );
    _inl const Vector4D&     operator =( const Vector3D& orig );
    _inl const Vector4D&     operator =( const Vector4D& orig );

    _inl const Vector4D&    copy( const Vector4D& orig );

    _inl float                norm() const;
    _inl float                norm2() const;
    _inl void                normalize();
    _inl void                normW();
    _inl void                reverse();
    _inl void                reverse( const Vector4D& orig );

    _inl void                cross( const Vector4D& v1, const Vector4D& v2 );

    _inl float                distance( const Vector4D& orig ) const;

    void                    txtSave( FILE* fp );


    static const    Vector4D oX;
    static const    Vector4D oY;
    static const    Vector4D oZ;
    static const    Vector4D null;
}; // class Vector4D


/*****************************************************************
/*  Class:  Line3D                                               *
/*  Desc:   3D line                                              *
/*****************************************************************/
class DIALOGS_API Line3D
{
    Vector3D orig;            //  line base point
    Vector3D dir;            //  line directional vector

public:
    Line3D(){}
    Line3D( const Vector3D& base, const Vector3D& direction );

    _inl float        project            ( const Vector3D& vec )            const;
    _inl float        dist2ToOrig        ( const Vector3D& vec )            const;
    _inl float        dist2ToPoint    ( const Vector3D& pt )            const;
    _inl float      dist2           ( const Line3D& line )          const;

    _inl float        length            ()                                const;

    _inl void        getPoint        ( float prLen, Vector3D& pt )    const;
    _inl Vector3D    getPoint        ( float prLen )                    const;
    _inl bool        IntersectPlane    ( const Plane& pl, Vector3D& interPt ) const;
    _inl const Vector3D& getOrig    ()                                const;
    _inl const Vector3D& getDir        ()                                const;

    _inl Vector3D    getDest            () const;
    _inl Vector3D    getCenter        () const;
    _inl void        setOrigDir        ( const Vector3D& _orig, const Vector3D& _dir );
    _inl void        setOrig            ( float x, float y, float z);
    _inl void        setOrig            ( const Vector3D& v );
    _inl void        Transform        ( const Matrix4D& tr );
    _inl bool        LineContains    ( const Vector3D& pt, float eps = c_SpaceEpsilon ) const;
    _inl bool        SegmentContains    ( const Vector3D& pt, float eps = c_SpaceEpsilon ) const;
    _inl void        Normalize        ();
    _inl float        GetZ            ( float x, float y ) const;

    Vector3D&        Orig            () { return orig; }
    Vector3D&        Dir                () { return dir; }

    void            Random            ( const AABoundBox& aabb, float minR, float maxR );

};  // class Line3D

DIALOGS_API DWORD VectorToColor( const Vector3D& v, float h );
DIALOGS_API Vector3D ToTangentSpace( const Vector3D& ldir, const Vector3D& normal, const Vector3D& right = Vector3D::oX );

#ifdef _INLINES
#include "mVector.inl"
#endif // _INLINES

#endif // __MVECTOR_H__