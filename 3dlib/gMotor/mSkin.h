/*****************************************************************
/*  File:   mSkin.h
/*    Desc:    Skinning code
/*****************************************************************/
#ifndef __MSKIN_H__
#define __MSKIN_H__

struct VertexOut
{
    Vector3D        pos;
    Vector3D        normal;
    float            u;
    float            v;
}; // struct VertexOut

typedef void (*FuncSkin1)( const Vertex1W*, VertexOut*, int, const Matrix4D*);
typedef void (*FuncSkin2)( const Vertex2W*, VertexOut*, int, const Matrix4D*);
typedef void (*FuncSkin3)( const Vertex3W*, VertexOut*, int, const Matrix4D*);
typedef void (*FuncSkin4)( const Vertex4W*, VertexOut*, int, const Matrix4D*);

extern FuncSkin1 Skin1;
extern FuncSkin2 Skin2;
extern FuncSkin3 Skin3;
extern FuncSkin4 Skin4;

enum ProcOptimMode
{
    poNone      = 0,
    poSSE       = 1,
}; // enum ProcOptimMode

void            SetProcessorOptimizations( ProcOptimMode mode );
ProcOptimMode   GetProcessorOptimizations();

#endif // __MSKIN_H__