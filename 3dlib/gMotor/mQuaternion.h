/*****************************************************************
/*  File:   mQuaternion.h                                            
/*    Desc:    cll math routines                                      
/*  Author: Silver, Copyright (C) GSC Game World                  
/*  Date:   January 2002                                          
/*****************************************************************/
#ifndef __MQUATERNION_H__
#define __MQUATERNION_H__

/*****************************************************************
/*  Class:  Quaternion                                            
/*  Desc:   quaternion class                                      
/*****************************************************************/
class DIALOGS_API Quaternion
{    
public:
    Vector3D    v;
    float        s;

    Quaternion() : s(0.0f) {};
    Quaternion( const Matrix3D& rotM ) { FromMatrix( rotM ); };

    Quaternion( float _x, float _y, float _z, float _s ) : 
    v(_x, _y, _z), s(_s) {}

    void                conjugate        ( const Quaternion& orig ) { v = orig.v; s = orig.s; v.reverse(); }
    void                conjugate        () { v.reverse(); }
    _inl void            setIdentity        ();

    _inl float            norm2            () const;
    _inl float            norm            () const;
    _inl void            normalize        (); 
    _inl void            reverse            ();
    _inl float            dot                ( const Quaternion& q ) const;

    _inl void            AxisToAxis        ( const Vector3D& from, const Vector3D& to );
    _inl void            FromAxisAngle    ( const Vector3D& axis, float angle );
    _inl void           ToAxisAngle     ( Vector3D& axis, float& angle ) const;
    _inl void            FromEulerAngles    ( float rotX, float rotY, float rotZ );
    _inl void            FromMatrix        ( const Matrix3D& rotM );
    _inl void            FromMatrix        ( const Matrix4D& rotM );
    void                Slerp            ( const Quaternion& src, const Quaternion& dst, float t, bool bShortest = true );
    void                Nlerp            ( const Quaternion& src, const Quaternion& dst, float t, bool bShortest = true );
    _inl Vector3D        EulerXYZ        ();

    _inl const Quaternion&    operator *=    ( const Quaternion& q );
    _inl const Quaternion&    operator *=    ( float f ) { v *= f; s *= f; return *this; }
    _inl bool                operator ==    ( const Quaternion& q ) const;
    _inl void                operator -=    ( const Quaternion& q ) { v.sub( q.v ); s -= q.s; }
    _inl void                operator +=    ( const Quaternion& q ) { v.add( q.v ); s += q.s; }

    bool                InSameHemisphere( const  Quaternion& quat ) const { return (dot( quat ) >= 0.0f); }

    static const Quaternion identity;
}; // class Quaternion

#ifdef _INLINES
#include "mQuaternion.inl"
#endif // _INLINES

#endif // __MQUATERNION_H__