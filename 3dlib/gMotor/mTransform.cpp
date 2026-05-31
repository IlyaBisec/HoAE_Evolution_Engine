/*****************************************************************
/*  File:   mTransform.cpp                                          
/*  Author: Silver, Copyright (C) GSC Game World                 
/*  Date:   January 2002                                         
/*****************************************************************/
#include "stdafx.h"
#include "mTransform.h"

#ifndef _INLINES
#include "mTransform.inl"
#endif // !_INLINES

/*****************************************************************************/
/*    Matrix3D implementation
/*****************************************************************************/
const Matrix3D    Matrix3D::identity(1.0f, 0.0f, 0.0f, 
                                   0.0f, 1.0f, 0.0f, 
                                   0.0f, 0.0f, 1.0f );

const Matrix3D    Matrix3D::null( 0.0f, 0.0f, 0.0f, 
                                0.0f, 0.0f, 0.0f, 
                                0.0f, 0.0f, 0.0f );

void Matrix3D::Random( float minV, float maxV )
{
    float* buf = getBuf();
    for (int i = 0; i < 9; i++)
    {
        buf[i] = rndValuef( minV, maxV );
    }
} // Matrix3D::random

/*****************************************************************
/*    Matrix4D implementation
/*****************************************************************/
const Matrix4D    Matrix4D::identity(  1.0f, 0.0f, 0.0f, 0.0f,
                                     0.0f, 1.0f, 0.0f, 0.0f,
                                     0.0f, 0.0f, 1.0f, 0.0f,
                                     0.0f, 0.0f, 0.0f, 1.0f );
const Matrix4D    Matrix4D::null( 0.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 0.0f, 0.0f );

void Matrix4D::txtSave( FILE* fp, const char* name ) const
{
    if (name)
    {
        fprintf( fp,    "%s={\n{%-8.8f, %-8.8f, %-8.8f, %-8.8f}, \n"
                        "{%-8.8f, %-8.8f, %-8.8f, %-8.8f}, \n"
                        "{%-8.8f, %-8.8f, %-8.8f, %-8.8f}, \n"
                        "{%-8.8f, %-8.8f, %-8.8f, %-8.8f} \n}\n",
                        name,
                        e00, e01, e02, e03, 
                        e10, e11, e12, e13, 
                        e20, e21, e22, e23,
                        e30, e31, e32, e33 );
    }
    else
    {
        fprintf( fp,    "%s={\n{%-8.8f, %-8.8f, %-8.8f, %-8.8f}, \n"
                        "{%-8.8f, %-8.8f, %-8.8f, %-8.8f}, \n"
                        "{%-8.8f, %-8.8f, %-8.8f, %-8.8f}, \n"
                        "{%-8.8f, %-8.8f, %-8.8f, %-8.8f} \n}\n",
                        name,
                        e00, e01, e02, e03, 
                        e10, e11, e12, e13, 
                        e20, e21, e22, e23,
                        e30, e31, e32, e33 );
    }
} // Matrix4D::txtSave

void Matrix4D::Random( float minV, float maxV )
{
    float* buf = getBuf();
    for (int i = 0; i < 16; i++)
    {
        buf[i] = rndValuef( minV, maxV );
    }
}

/*---------------------------------------------------------------*
/*  Func:    Matrix4D::inverse
/*    Desc:    Finds 4x4 matrix inverse using Cramer's rule
/*    Parm:    orig - original matrix
/*    Ret:    determinant value
/*---------------------------------------------------------------*/
float Matrix4D::inverse( const Matrix4D& orig )
{
    float tmp[12]; 
    float src[16]; 
    float* mat = orig.getBuf();
    float* dst = getBuf();
    float det = 0.0f;

    for ( int i = 0; i < 4; i++) 
    {
        src[i        ]    = mat[i*4        ];
        src[i + 4    ]    = mat[i*4 + 1    ];
        src[i + 8    ]    = mat[i*4 + 2    ];
        src[i + 12    ]    = mat[i*4 + 3    ];
    }
    
    //  co-factors
    tmp[0] = src[10] * src[15];
    tmp[1] = src[11] * src[14];
    tmp[2] = src[9]     * src[15];
    tmp[3] = src[11] * src[13];
    tmp[4] = src[9]  * src[14];
    tmp[5] = src[10] * src[13];
    tmp[6] = src[8]  * src[15];
    tmp[7] = src[11] * src[12];
    tmp[8] = src[8]  * src[14];
    tmp[9] = src[10] * src[12];
    tmp[10] = src[8] * src[13];
    tmp[11] = src[9] * src[12];
    
    dst[0] =    tmp[0] * src[5] + tmp[3] * src[6] + tmp[4]  * src[7];
    dst[0] -=    tmp[1] * src[5] + tmp[2] * src[6] + tmp[5]  * src[7];
    dst[1] =    tmp[1] * src[4] + tmp[6] * src[6] + tmp[9]  * src[7];
    dst[1] -=    tmp[0] * src[4] + tmp[7] * src[6] + tmp[8]  * src[7];
    dst[2] =    tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7];
    dst[2] -=    tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7];
    dst[3] =    tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6];
    dst[3] -=    tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6];
    dst[4] =    tmp[1] * src[1] + tmp[2] * src[2] + tmp[5]  * src[3];
    dst[4] -=    tmp[0] * src[1] + tmp[3] * src[2] + tmp[4]  * src[3];
    dst[5] =    tmp[0] * src[0] + tmp[7] * src[2] + tmp[8]  * src[3];
    dst[5] -=    tmp[1] * src[0] + tmp[6] * src[2] + tmp[9]  * src[3];
    dst[6] =    tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3];
    dst[6] -=    tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3];
    dst[7] =    tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2];
    dst[7] -=    tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2];
    
    tmp[0]    = src[2] * src[7];
    tmp[1]    = src[3] * src[6];
    tmp[2]    = src[1] * src[7];
    tmp[3]    = src[3] * src[5];
    tmp[4]    = src[1] * src[6];
    tmp[5]    = src[2] * src[5];
    tmp[6]    = src[0] * src[7];
    tmp[7]    = src[3] * src[4];
    tmp[8]    = src[0] * src[6];
    tmp[9]    = src[2] * src[4];
    tmp[10] = src[0] * src[5];
    tmp[11] = src[1] * src[4];
    
    dst[8] =    tmp[0]  * src[13] + tmp[3] * src[14] + tmp[4] * src[15];
    dst[8] -=    tmp[1]  * src[13] + tmp[2] * src[14] + tmp[5] * src[15];
    dst[9] =    tmp[1]  * src[12] + tmp[6] * src[14] + tmp[9] * src[15];
    dst[9] -=    tmp[0]  * src[12] + tmp[7] * src[14] + tmp[8] * src[15];
    dst[10] =    tmp[2]  * src[12] + tmp[7] * src[13] + tmp[10] * src[15];
    dst[10]-=    tmp[3]  * src[12] + tmp[6] * src[13] + tmp[11] * src[15];
    dst[11] =    tmp[5]  * src[12] + tmp[8] * src[13] + tmp[11] * src[14];
    dst[11]-=    tmp[4]  * src[12] + tmp[9] * src[13] + tmp[10] * src[14];
    dst[12] =    tmp[2]  * src[10] + tmp[5] * src[11] + tmp[1] * src[9];
    dst[12]-=    tmp[4]  * src[11] + tmp[0] * src[9] + tmp[3] * src[10];
    dst[13] =    tmp[8]  * src[11] + tmp[0] * src[8] + tmp[7] * src[10];
    dst[13]-=    tmp[6]  * src[10] + tmp[9] * src[11] + tmp[1] * src[8];
    dst[14] =    tmp[6]  * src[9]  + tmp[11] * src[11] + tmp[3] * src[8];
    dst[14]-=    tmp[10] * src[11] + tmp[2]  * src[8] + tmp[7] * src[9];
    dst[15] =    tmp[10] * src[10] + tmp[4]  * src[8] + tmp[9] * src[9];
    dst[15]-=    tmp[8]  * src[9]  + tmp[11] * src[10] + tmp[5] * src[8];
    
    //  determinant
    det=src[0] * dst[0]+src[1] * dst[1]+src[2] * dst[2]+src[3] * dst[3];
    //  matrix inverse 
    float idet = 1.0f;
    if (fabs( det ) >= c_SmallEpsilon) idet = 1.0f / det;
    for (int j = 0; j < 16; j++) dst[j] *= idet;

    return det;
} // Matrix4D::inverse

float Matrix4D::inverse()
{
    Matrix4D tmp( *this );
    return inverse( tmp );
} // Matrix4D::inverse

void Matrix4D::Factorize( Vector3D& scale, Matrix3D& rot, Vector3D& trans ) const
{
    rot = *this;
    scale.x = rot.getV0().normalize();
    scale.y = rot.getV1().normalize();
    scale.z = rot.getV2().normalize();
    trans.x = e30;
    trans.y = e31;
    trans.z = e32;
} // Matrix4D::Factorize

void Matrix4D::Factorize( Vector3D& scale, Quaternion& rot, Vector3D& trans ) const
{
    Matrix3D rotTM = *this;
    scale.x = rotTM.getV0().normalize();
    scale.y = rotTM.getV1().normalize();
    scale.z = rotTM.getV2().normalize();
    rot.FromMatrix( rotTM );
    trans.x = e30;
    trans.y = e31;
    trans.z = e32;
} // Matrix4D::Factorize

void Matrix4D::ShadowTM( const Plane& p, const Vector4D& l )
{
    float d = p.a*l.x + p.b*l.y + p.c*l.z + p.d*l.w;
    e00 = p.a * l.x + d; e01 = p.a * l.y;        e02 = p.a * l.z;        e03 = p.a * l.w;  
    e10 = p.b * l.x;     e11 = p.b * l.y + d;    e12 = p.b * l.z;        e13 = p.b * l.w;  
    e20 = p.c * l.x;     e21 = p.c * l.y;        e22 = p.c * l.z + d;    e23 = p.c * l.w;  
    e30 = p.d * l.x;     e31 = p.d * l.y;        e32 = p.d * l.z;        e33 = p.d * l.w + d;
} // Matrix4D::ShadowTM

/*---------------------------------------------------------------*
/*  Func:    Plane2WorldTM
/*    Desc:    getting Matrix4D that transforms 3 2D-vectors (xi,yi,0)
/*            to 3 3D-vectors (Xi,Yi,Zi)
/*    Parm:    VSi - 2d vectors, VWi - 3d vectors
/*---------------------------------------------------------------*/
Matrix4D Plane2WorldTM(
                                        const Vector2D& VS1, const Vector2D& VS2, const Vector2D& VS3,
                                        const Vector3D&  VW1, const Vector3D&  VW2, const Vector3D&  VW3 )
{
    Matrix3D M;
    Matrix3D W;
    Matrix3D V;
    Matrix3D T1;
    Matrix3D T2;
    V.e00=VS1.x;V.e10=VS1.y;V.e20=1.0f;
    V.e01=VS2.x;V.e11=VS2.y;V.e21=1.0f;
    V.e02=VS3.x;V.e12=VS3.y;V.e22=1.0f;
    T1=V;
    V.inverse();
    W.e00=VW1.x;W.e10=VW1.y;W.e20=VW1.z;
    W.e01=VW2.x;W.e11=VW2.y;W.e21=VW2.z;
    W.e02=VW3.x;W.e12=VW3.y;W.e22=VW3.z;
    M.mul(W,V);
    T2.mul(M,T1);
    Matrix4D r;
    r.e00=M.e00;r.e10=M.e01;r.e20=0.0f;r.e30=M.e02;
    r.e01=M.e10;r.e11=M.e11;r.e21=0.0f;r.e31=M.e12;
    r.e02=M.e20;r.e12=M.e21;r.e22=0.0f;r.e32=M.e22;
    r.e03=0.0f; r.e13=0.0f; r.e23=0.0f;r.e33=1.0f;
    return r;
}//  Matrix4D::Plane2WorldTM







