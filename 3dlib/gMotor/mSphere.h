/*****************************************************************************/
/*    File:    mSphere.cpp
/*  Desc:    3D sphere
/*    Author:    Ruslan Shestopalyuk
/*    Date:    09-25-2003
/*****************************************************************************/
#ifndef __MSPHERE_H__
#define __MSPHERE_H__

class VertexIterator;
/*****************************************************************
/*  Class:  Sphere                                             
/*  Desc:   3D sphere                                             
/*****************************************************************/
class DIALOGS_API Sphere
{
    Vector3D                m_Center;
    float                    m_Radius;
public:
    _inl                    Sphere        (){}
    _inl                    Sphere        ( const Vector3D& center, float radius );
    _inl                    Sphere      ( const Vector3D& a, const Vector3D& b );
    _inl                    Sphere      ( const Vector3D& a, const Vector3D& b, const Vector3D& c );
    _inl                    Sphere      ( const Vector3D& a, const Vector3D& b, 
                                          const Vector3D& c, const Vector3D& d );

    _inl Sphere&            operator += ( const Sphere& sphere );
    
    _inl XStatus            Intersect    ( const AABoundBox&    aabb   ) const;    
    _inl XStatus            Intersect    ( const Plane&        plane  ) const;
    _inl XStatus            Intersect    ( const Sphere&        sphere ) const;
    _inl bool                Overlap        ( const Sphere&        sphere ) const;

    const Vector3D&            GetCenter    () const                    { return m_Center; }
    float                    GetRadius    () const                    { return m_Radius; }
    void                    SetCenter    ( const Vector3D& center )  { m_Center = center; }
    void                    SetRadius    ( float radius )            { m_Radius = radius; }

    _inl Vector3D            RandomPoint    ();
    _inl void                Transform    ( const Matrix4D& tm );
    
    _inl void                ProjectPt    ( Vector3D& pt ) const;
    _inl void                Decompose    ( const Vector3D& pos, const Vector3D& dir, 
                                            Vector3D& normC, Vector3D& tanC ) const;
    void                    Random        ( const AABoundBox& aabb, float minR, float maxR );
    
    static const Sphere     null;

}; // class Sphere

#ifdef _INLINES
#include "mSphere.inl"
#endif // _INLINES

#endif // __MSPHERE_H__