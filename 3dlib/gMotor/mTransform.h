/*****************************************************************
/*  File:   mTransform.h                                             
/*    Desc:    Affine transform matrix                                  
/*  Author: Silver, Copyright (C) GSC Game World                  
/*  Date:   January 2002                                          
/*****************************************************************/
#ifndef __MTRANSFORM_H__
#define __MTRANSFORM_H__

class Matrix4D;
class Quaternion;
class Line3D;
class Vector3D;
class Vector4D;
class Plane;

/*****************************************************************
/*  Class:  Matrix3D                                         
/*  Desc:   3x3 transformation matrix                            
/*****************************************************************/
class DIALOGS_API Matrix3D
{
public:
    float        e00, e01, e02;
    float        e10, e11, e12;
    float        e20, e21, e22;

    static const Matrix3D    identity;
    static const Matrix3D    null;

    _inl                     Matrix3D(){}
    _inl                     Matrix3D( const Matrix4D& m );
    _inl                     Matrix3D( const Quaternion& q ) { rotation( q ); }
    _inl                    Matrix3D( const Vector3D& axis, float angle ) { rotation( axis, angle ); }
    _inl                     Matrix3D(    const Vector3D& v0,  
                                        const Vector3D& v1,
                                        const Vector3D& v2 );
    _inl                    Matrix3D(    float _e00, float _e01, float _e02,
                                        float _e10, float _e11, float _e12,
                                        float _e20, float _e21, float _e22 );

    _inl void                 rotation( const Vector3D& axis, float angle );
    _inl void                 rotation( float rotX, float rotY, float rotZ );
    _inl void                 rotation( const Quaternion& q );

    _inl void                 rotationXY( float cosPhi, float sinPhi );
    _inl void                 rotationYZ( float cosPhi, float sinPhi );
    _inl void                 rotationXZ( float cosPhi, float sinPhi );

    _inl void                 shearXY( float cosPhi, float sinPhi );
    _inl void                 shearYZ( float cosPhi, float sinPhi );
    _inl void                 shearXZ( float cosPhi, float sinPhi );

    _inl void                 scaling( float sx, float sy, float sz );

    _inl float*                getBuf()    const { return (float*)&e00; }
    _inl Vector3D&            getV0()        const { return (Vector3D&)e00; }
    _inl Vector3D&            getV1()        const { return (Vector3D&)e10; }
    _inl Vector3D&            getV2()        const { return (Vector3D&)e20; }

    _inl Matrix3D& operator *=( const Matrix3D& r );
    _inl Matrix3D& operator +=( const Matrix3D& r );
    _inl Matrix3D& operator *=( const float f );
    _inl Matrix3D& operator +=( const float f );

    _inl Matrix3D&            mulLeft( const Matrix3D& l );

    _inl void                mul            ( const Matrix3D& l, const Matrix3D& r );
    _inl void                setNull        ();
    _inl void                setIdentity    ();
    _inl float                inverse        ( const Matrix3D& m );
    _inl float                inverse        ();
    _inl float                det            () const;
    _inl void                txtSave        ( FILE* fp, const char* name ) const;
    _inl void                copy        ( const Matrix3D& orig );
    _inl void                copy        ( const Matrix4D& orig );
    _inl void                transpose    ();
    _inl float                trace        () const;

    _inl bool                IsOrthoNorm () const;
    _inl Vector3D            EulerXYZ    () const;
    _inl void                SetColumns    ( const Vector3D& v0, const Vector3D& v1, const Vector3D& v2 );
    _inl void                SetRows        ( const Vector3D& v0, const Vector3D& v1, const Vector3D& v2 );
    void                    Random        ( float minV = 0.0f, float maxV = 1.0f );

}; // class Matrix3D

/*****************************************************************
/*  Class:  Matrix4D                                            
/*  Desc:   Affine transformation matrix                                         
/*****************************************************************/
class DIALOGS_API Matrix4D
{
public:
    float        e00, e01, e02, e03;
    float        e10, e11, e12, e13;
    float        e20, e21, e22, e23;
    float        e30, e31, e32, e33;

    _inl Matrix4D(    float _e00, float _e01, float _e02, float _e03,
                    float _e10, float _e11, float _e12, float _e13,
                    float _e20, float _e21, float _e22, float _e23,
                    float _e30, float _e31, float _e32, float _e33 );

    _inl Matrix4D(){}
    _inl Matrix4D( const Matrix3D& m );

    _inl Matrix4D( const Vector3D& sc, const Quaternion& qrot, const Vector3D& tr );
    _inl Matrix4D( const Vector3D& sc, const Matrix3D& mrot, const Vector3D& tr );
    _inl Matrix4D( const Vector3D& sc, const Vector3D& rotAxis, float rotAngle, const Vector3D& tr );
    _inl Matrix4D( const Vector3D& vx, const Vector3D& vy, const Vector3D& vz );
    _inl Matrix4D( const Vector3D& vx, const Vector3D& vy, const Vector3D& vz, const Vector3D& tr );

    _inl Matrix4D&    mul( const Matrix4D& a, const Matrix4D& b );
    float            inverse( const Matrix4D& orig );
    float            inverse();
    _inl void        affineInverse( const Matrix4D& orig );
    
    _inl const Matrix4D&    operator = ( const Matrix3D& matr );
    _inl const Matrix4D&    operator = ( const Matrix4D& matr );
    _inl Matrix4D&            operator *=( const Matrix4D& r );
    _inl Matrix4D&            operator *=( float w );
    _inl Matrix4D&            operator +=( const Matrix4D& r );
    _inl Matrix4D&            operator -=( const Matrix4D& r );

    _inl Matrix4D&            addWeighted( const Matrix4D& r, float w );
    
    _inl void        setIdentity();
    _inl void        setNull();

    _inl float        norm2()        const;    

    _inl float*        getBuf()    const { return (float*)&e00; }
    _inl Vector4D&    getV0()        const { return (Vector4D&)e00; }
    _inl Vector4D&    getV1()        const { return (Vector4D&)e10; }
    _inl Vector4D&    getV2()        const { return (Vector4D&)e20; }
    _inl Vector4D&    getV3()        const { return (Vector4D&)e30; }

    _inl Vector3D    GetXCol        () const { return Vector3D( e00, e10, e22 ); }
    _inl Vector3D    GetXRow        () const { return Vector3D( e00, e01, e02 ); }
    _inl Vector3D    GetYCol        () const { return Vector3D( e01, e11, e21 ); } 
    _inl Vector3D    GetYRow        () const { return Vector3D( e10, e11, e12 ); } 
    _inl Vector3D    GetZCol        () const { return Vector3D( e02, e12, e22 ); }
    _inl Vector3D    GetZRow        () const { return Vector3D( e20, e21, e22 ); }

    _inl void        translation    ( float dx, float dy, float dz );
    _inl void        translation    ( const Vector3D& trans );

    _inl void        translate    ( float dx, float dy, float dz );
    _inl void        translate    ( const Vector3D& trans );

    _inl void        toBasis(    const Vector3D& bX, 
                                const Vector3D& bY, 
                                const Vector3D& bZ,
                                const Vector3D& trans );

    _inl void        fromBasis(    const Vector3D& bX, 
                                const Vector3D& bY, 
                                const Vector3D& bZ,
                                const Vector3D& trans );
    _inl void        setTranslation    ( const Vector3D& tr );
    _inl void        setRotation        ( const Matrix3D& m );
    Vector3D        getTranslation    () const { return Vector3D( e30, e31, e32 ); }
    _inl void        scaling            ( float sx, float sy, float sz );
    _inl void        scaling            ( float s );
    _inl void        st                ( float s, const Vector3D& tr );
    _inl void        st                ( const Vector3D& s, const Vector3D& tr );
    _inl void        rotation        ( const Vector3D& axis, float angle );
    _inl void        rotation        ( const Line3D& axis, float angle );
    _inl bool        equal            ( const Matrix4D& m ) const;

    _inl void        mirrorXoY        ();
    _inl void        mirrorXoZ        ();
    _inl void        mirrorYoZ        ();

    _inl void        flipXY            ();
    _inl void        flipXZ            ();
    _inl void        flipYZ            ();

    _inl void        rotation( const Quaternion& q );
    _inl void        shearing(    float sxy, float sxz, 
                                float syx, float syz,
                                float szx, float szy );

    _inl void        srt( float scale, const Vector3D& axis, float angle, const Vector3D& pos );
    
    _inl float        quadraticForm( Vector4D& v );
    _inl Matrix4D&    mulVV( const Vector4D& v );

    _inl void        transpose( const Matrix4D& orig );
    void            transpose   () { Matrix4D tmp(*this); transpose( tmp ); }

    void            txtSave        ( FILE* fp, const char* name = 0 ) const;
    void            Random        ( float minV = 0.0f, float maxV = 1.0f );
    void            Factorize   ( Vector3D& scale, Matrix3D& rot, Vector3D& trans ) const;
    void            Factorize   ( Vector3D& scale, Quaternion& rot, Vector3D& trans ) const;

    _inl int        read        ( BYTE* sbuf, int maxSize );
    _inl int        write        ( BYTE* sbuf, int maxSize ) const;
    _inl int        read        ( FILE* fp );
    _inl int        write        ( FILE* fp ) const;

    _inl Matrix4D&    mulLeft        ( const Matrix4D& m );
    _inl void        transformPt ( Vector3D& pt  ) const;
    _inl void        transformVec( Vector3D& vec ) const;

    _inl void        Blend2        ( const Matrix4D& ma, float wa, const Matrix4D& mb, float wb );
    _inl void        Blend3        (    const Matrix4D& ma, float wa, 
                                    const Matrix4D& mb, float wb,
                                    const Matrix4D& mc, float wc );
    _inl void        Blend4        (    const Matrix4D& ma, float wa, 
                                    const Matrix4D& mb, float wb,
                                    const Matrix4D& mc, float wc, 
                                    const Matrix4D& md, float wd );
    void            ShadowTM    ( const Plane& p, const Vector4D& l );
    _inl Vector3D   EulerXYZ    () const;

    static const    Matrix4D identity;
    static const    Matrix4D null;

};  // class Matrix4D

const int c_MatrixStackDepth        = 128; 
/*****************************************************************
/*  Class:  MatrixStack
/*  Desc:   4-dimencional matrix stack
/*****************************************************************/
class DIALOGS_API MatrixStack
{
    Matrix4D                m_Stack[c_MatrixStackDepth];
    int                     m_Top;
public:
    _inl                    MatrixStack();

    _inl void               Push( const Matrix4D& m, bool bOverride = false );
    _inl const Matrix4D&    Pop();
    _inl const Matrix4D&    Top() const;
    _inl void               Reset( const Matrix4D& m = Matrix4D::identity );

}; // class MatrixStack


template <class T>
bool Inverse3x3( T* m, T& det )
{
    det = m[0]*(m[4]*m[8] - m[7]*m[5]) + m[1]*(m[6]*m[5] - m[3]*m[8]) + m[2]*(m[3]*m[7] - m[6]*m[4]);
    if (fabs( det ) < c_SmallEpsilon) return false;
    T r[9];

    r[0] = (-m[5]*m[7] + m[4]*m[8]) / det;
    r[1] = ( m[2]*m[7] - m[1]*m[8]) / det;
    r[2] = (-m[2]*m[4] + m[1]*m[5]) / det;

    r[3] = ( m[5]*m[6] - m[3]*m[8]) / det;
    r[4] = (-m[2]*m[6] + m[0]*m[8]) / det;
    r[5] = ( m[2]*m[3] - m[0]*m[5]) / det;

    r[6] = (-m[4]*m[6] + m[3]*m[7]) / det;
    r[7] = ( m[1]*m[6] - m[0]*m[7]) / det;
    r[8] = (-m[1]*m[3] + m[0]*m[4]) / det;
    
    memcpy( m, r, 9*sizeof( T ) );
    return true;
}

_inl bool OrthoProjectionTM( Matrix4D& m, float viewVolW, float wToH, float zn, float zf );

#ifdef _INLINES
#include "mTransform.inl"
#endif // _INLINES

#endif // __MTRANSFORM_H__