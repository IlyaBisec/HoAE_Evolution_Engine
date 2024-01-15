/*****************************************************************
/*  File:   mSkin.cpp                                      
/*    Desc:    Skinning code here                      
/*****************************************************************/
#include "stdafx.h"
#include "mSkin.h"

void fpu_Skin1( const Vertex1W* vSrc, VertexOut* vDest, int nV, const Matrix4D* bones );                                 
void fpu_Skin2( const Vertex2W* vSrc, VertexOut* vDest, int nV, const Matrix4D* bones );                                 
void fpu_Skin3( const Vertex3W* vSrc, VertexOut* vDest, int nV, const Matrix4D* bones );                                 
void fpu_Skin4( const Vertex4W* vSrc, VertexOut* vDest, int nV, const Matrix4D* bones );   

void sse_Skin1( const Vertex1W* vSrc, VertexOut* vDest, int nV, const Matrix4D* bones );                                 
void sse_Skin2( const Vertex2W* vSrc, VertexOut* vDest, int nV, const Matrix4D* bones );                                 
void sse_Skin3( const Vertex3W* vSrc, VertexOut* vDest, int nV, const Matrix4D* bones );                                 
void sse_Skin4( const Vertex4W* vSrc, VertexOut* vDest, int nV, const Matrix4D* bones ); 

FuncSkin1 Skin1 = fpu_Skin1;
FuncSkin2 Skin2 = fpu_Skin2;
FuncSkin3 Skin3 = fpu_Skin3;
FuncSkin4 Skin4 = fpu_Skin4;

ProcOptimMode   g_ProcOptimMode = poNone;
void SetProcessorOptimizations( ProcOptimMode mode )
{
    g_ProcOptimMode = mode;
    switch (mode)
    {
    case poNone: 
        Skin1 = fpu_Skin1;
        Skin2 = fpu_Skin2;
        Skin3 = fpu_Skin3;
        Skin4 = fpu_Skin4;
    break;
    case poSSE:
        Skin1 = sse_Skin1;
        Skin2 = sse_Skin2;
        Skin3 = sse_Skin3;
        Skin4 = sse_Skin4;
    break;
    }
} // SetProcessorOptimizations

void InitMath()
{
    if (HaveSSE())
    {
        SetProcessorOptimizations( poSSE );
    }
    else
    {
        SetProcessorOptimizations( poNone );
    }
} // InitMath

ProcOptimMode GetProcessorOptimizations()
{
    return g_ProcOptimMode;
} // SetProcessorOptimizations

void fpu_Skin1( const Vertex1W* vSrc, VertexOut* vDest, int nV, const Matrix4D* bones )
{
    for (int i = 0; i < nV; i++)
    {
        const Vertex1W& v = vSrc[i];
        Vector3D pos    = v.pos;
        Vector3D normal = v.normal;
        bones[v.m].transformPt    ( pos );
        bones[v.m].transformVec    ( normal );
        vDest[i].pos    = pos;
        vDest[i].normal = normal;
        vDest[i].u      = v.u;
        vDest[i].v      = v.v;
    }
} // fpu_Skin1

void fpu_Skin2( const Vertex2W* vSrc, VertexOut* vDest, int nV, const Matrix4D* bones )
{
    Matrix4D boneM; 
    for (int i = 0; i < nV; i++)
    {
        const Vertex2W& v = vSrc[i];
        Vector3D pos    = v.pos;
        Vector3D normal = v.normal;
        //  calculate blending matrix
        boneM.Blend2(   bones[v.m0], v.w, 
                        bones[v.m1], 1.0f - v.w );
        boneM.transformPt    ( pos );
        boneM.transformVec    ( normal );
        vDest[i].pos    = pos;
        vDest[i].normal = normal;
        vDest[i].u      = v.u;
        vDest[i].v      = v.v;
    }
} // fpu_Skin2

void fpu_Skin3( const Vertex3W* vSrc, VertexOut* vDest, int nV, const Matrix4D* bones )
{
    Matrix4D boneM;
    for (int i = 0; i < nV; i++)
    {
        const Vertex3W& v = vSrc[i];
        Vector3D pos    = v.pos;
        Vector3D normal = v.normal;
        //  calculate blending matrix
        boneM.Blend3(   bones[v.m0], v.w0, 
                        bones[v.m1], v.w1, 
                        bones[v.m2], 1.0f - v.w0 - v.w1 );
        boneM.transformPt    ( pos );
        boneM.transformVec    ( normal );
        vDest[i].pos    = pos;
        vDest[i].normal = normal;
        vDest[i].u      = v.u;
        vDest[i].v      = v.v;
    }
} // fpu_Skin3

void fpu_Skin4( const Vertex4W* vSrc, VertexOut* vDest, int nV, const Matrix4D* bones )
{
    Matrix4D boneM;
    for (int i = 0; i < nV; i++)
    {
        const Vertex4W& v = vSrc[i];
        Vector3D pos    = v.pos;
        Vector3D normal = v.normal;
        //  calculate blending matrix
        boneM.Blend4(   bones[v.m0], v.w0, 
                        bones[v.m1], v.w1, 
                        bones[v.m2], v.w2,
                        bones[v.m3], 1.0f - v.w0 - v.w1 - v.w2 );
        boneM.transformPt    ( pos );
        boneM.transformVec    ( normal );
        vDest[i].pos    = pos;
        vDest[i].normal = normal;
        vDest[i].u      = v.u;
        vDest[i].v      = v.v;
    }
} // fpu_Skin4