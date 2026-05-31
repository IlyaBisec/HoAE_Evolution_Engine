/*****************************************************************
/*  File:   mSkinSSE.cpp                                      
/*  Author: Manowar
/*    Desc:    SSE skinning code here            
/*****************************************************************/
#include "stdafx.h"
#include "mSkin.h"

void nulSkin1( const Vertex1W* vSrc, VertexOut* vDest, int nV, const Matrix4D* bones )
{
};

// -- offsets -------------------------------------------------------
#define M11 0
#define M12 4
#define M13 8
#define M14 12
#define M21 16
#define M22 20
#define M23 24
#define M24 28
#define M31 32
#define M32 36
#define M33 40
#define M34 44
#define M41 48
#define M42 52
#define M43 56
#define M44 60
// -- line offsets ---------------------------------------------------
#define LINE1(base) XMMWORD PTR [base+M11]
#define LINE2(base) XMMWORD PTR [base+M21]
#define LINE3(base) XMMWORD PTR [base+M31]
#define LINE4(base) XMMWORD PTR [base+M41]

#define PRD 2

void sse_Skin1( const Vertex1W* vSrc, VertexOut* vDest, int nV, const Matrix4D* bones )
{__asm{
// ------------------------------------------------------------------
    mov            ecx,nV                    ; ecx=nV
// ------------------------------------------------------------------
    mov            esi,DWORD PTR [vSrc]    ; esi=vSrc
    mov            edi,DWORD PTR [vDest]    ; edi=vDest
// ------------------------------------------------------------------
    ALIGN        16                        ; 
    new_vtx:                            ; _new cycle iteration
// ------------------------------------------------------------------
// Calculating transformation matrix address
// ------------------------------------------------------------------
    mov            eax,DWORD PTR [esi]vSrc.m        ; eax=vSrc[i].m=index
    shl            eax,6                            ; eax=index*sizeof(Matrix4D)
    add            eax,DWORD PTR [bones]            ; eax=index*sizeof(Matrix4D)+offset(bones)
// ------------------------------------------------------------------    
// Calculating the next transformation matrix address for prefetch
// ------------------------------------------------------------------    
    mov            ebx,DWORD PTR [esi+PRD*(TYPE Vertex1W)]vSrc.m    ; ebx=vSrc[i+2].m=index
    shl            ebx,6                                            ; ebx=index*sizeof(Matrix4D)
    add            ebx,DWORD PTR [bones]                            ; ebx=index*sizeof(Matrix4D)+offset(bones)
// ------------------------------------------------------------------    
// Pt (xmm1,xmm2,xmm3) & Vec (xmm4,xmm5,xmm6) interleaved
// ------------------------------------------------------------------    
    movss        xmm1,DWORD PTR [esi]vSrc.pos.x    ; xmm1 = ?.? | ?.? | ?.? | P.x
    movss        xmm2,DWORD PTR [esi]vSrc.pos.y    ; xmm2 = ?.? | ?.? | ?.? | P.y
    movss        xmm3,DWORD PTR [esi]vSrc.pos.z    ; xmm3 = ?.? | ?.? | ?.? | P.z

    shufps        xmm1,xmm1,0                        ; xmm1 = P.x | P.x | P.x | P.x
    shufps        xmm2,xmm2,0                        ; xmm2 = P.y | P.y | P.y | P.y
    shufps        xmm3,xmm3,0                        ; xmm3 = P.z | P.z | P.z | P.z

    mulps        xmm1,LINE1(eax)                    ; xmm1 = P.x*_14 | P.x*_13 | P.x*_12 | P.x*_11
    mulps        xmm2,LINE2(eax)                    ; xmm2 = P.y*_24 | P.y*_23 | P.y*_22 | P.y*_21
    mulps        xmm3,LINE3(eax)                    ; xmm3 = P.z*_34 | P.z*_33 | P.z*_32 | P.z*_31
// ------------------------------------------------------------------        
//    Actual prefetch
// ------------------------------------------------------------------        
    prefetchnta    BYTE PTR [ebx]                                    ;
// ------------------------------------------------------------------    
    movss        xmm4,DWORD PTR [esi]vSrc.normal.x    ; xmm4 = ?.? | ?.? | ?.? | N.x
    movss        xmm5,DWORD PTR [esi]vSrc.normal.y    ; xmm5 = ?.? | ?.? | ?.? | N.y
    movss        xmm6,DWORD PTR [esi]vSrc.normal.z    ; xmm6 = ?.? | ?.? | ?.? | N.z

    addps        xmm1,xmm2                        ; xmm1 = P.x*_14+P.y*_24 | P.x*_13+P.y*_23 |
                                                ;         P.x*_12+P.y*_22 | P.x*_11+P.y*_21

    shufps        xmm4,xmm4,0                        ; xmm4 = N.x | N.x | N.x | N.x
    shufps        xmm5,xmm5,0                        ; xmm5 = N.y | N.y | N.y | N.y
    shufps        xmm6,xmm6,0                        ; xmm6 = N.z | N.z | N.z | N.z

    mulps        xmm4,LINE1(eax)                    ; xmm4 = N.x*_14 | N.x*_13 | N.x*_12 | N.x*_11
    mulps        xmm5,LINE2(eax)                    ; xmm5 = N.y*_24 | N.y*_23 | N.y*_22 | N.y*_21
    mulps        xmm6,LINE3(eax)                    ; xmm6 = N.z*_34 | N.z*_33 | N.z*_32 | N.z*_31
    
    addps        xmm3,LINE4(eax)                    ; xmm3 = P.z*_34+_44 | P.z*_33+_43 | 
                                                ;         P.z*_32+_42 | P.z*_31+_41
    addps        xmm4,xmm5                        ; xmm4 = N.x*_14+N.y*_24 | N.x*_13+N.y*_23 |
                                                ;         N.x*_12+N.y*_22 | N.x*_11+N.y*_21
    addps        xmm3,xmm1                        ; xmm3 = P.x*_14+P.y*_24+P.z*_34+_44 | P.x*_13+P.y*_23+P.z*_33+_43 |
                                                ;         P.x*_12+P.y*_22+P.z*_32+_42 | P.x*_11+P.y*_21+P.z*_31+_41
    addps        xmm6,xmm4                        ; xmm6 = N.x*_14+N.y*_24+N.z*_34+_44 | N.x*_13+N.y*_23+N.z*_33+_43 |
                                                ;         N.x*_12+N.y*_22+N.z*_32+_42 | N.x*_11+N.y*_21+N.z*_31+_41
// ------------------------------------------------------------------    
// Swizling data for aligned store
// ------------------------------------------------------------------    
    movss        xmm0,xmm3                        ; xmm0 = ?.? | ?.? | ?.? | PX
    movss        xmm3,xmm6                        ; xmm3 = ?.? | PZ | PY | NX
    shufps        xmm3,xmm3,00100111b                ; xmm3 = NX | PZ | PY | ?.?
    movss        xmm3,xmm0                        ; xmm3 = NX | PZ | PY | PX

    shufps        xmm6,xmm6,11001001b                ; xmm6 = ?.? | NX | NZ | NY
    movhps        xmm6,MMWORD PTR [esi]vSrc.u        ; xmm6 = v | u | NZ | NY
// ------------------------------------------------------------------    
// Storing
// ------------------------------------------------------------------    
    movntps        XMMWORD PTR [edi],xmm3            ;
    movntps        XMMWORD PTR [edi+16],xmm6        ;
// ------------------------------------------------------------------    
    add            esi,TYPE Vertex1W        ; esi+=sizeof(Vertex1W)
    add            edi,TYPE VertexOut        ; edi+=sizeof(VertexOut)
// ------------------------------------------------------------------    
    dec            ecx                        ; ecx-=1
    jnz            new_vtx                    ; ecx==0 ? goto new_vtx : exit
// ------------------------------------------------------------------
    sfence                                ; write back all caches
// ------------------------------------------------------------------
}} // sse_Skin1

void sse_Skin2( const Vertex2W* vSrc, VertexOut* vDest, int nV, const Matrix4D* bones )
{__asm{
// ------------------------------------------------------------------
    mov            ecx,nV                    ; ecx=nV
// ------------------------------------------------------------------
    mov            esi,DWORD PTR [vSrc]    ; esi=vSrc
    mov            edi,DWORD PTR [vDest]    ; edi=vDest
// ------------------------------------------------------------------
    ALIGN        16                        ; 
    new_vtx:                            ; _new cycle iteration
// ------------------------------------------------------------------
// Calculating 1st & 2nd transformation matrix address
// ------------------------------------------------------------------
    mov            eax,DWORD PTR [esi]vSrc.m0        ; eax=vSrc[i].m0=index0
    mov            edx,DWORD PTR [esi]vSrc.m1        ; edx=vSrc[i].m1=index1
    shl            eax,6                            ; eax=index0*sizeof(Matrix4D)
    shl            edx,6                            ; edx=index1*sizeof(Matrix4D)
    add            eax,DWORD PTR [bones]            ; eax=index0*sizeof(Matrix4D)+offset(bones)
    add            edx,DWORD PTR [bones]            ; edx=index1*sizeof(Matrix4D)+offset(bones)
// ------------------------------------------------------------------    
// Blending 2 matrixes
// ------------------------------------------------------------------    
    movaps        xmm4,LINE1(eax)                    ;
    movaps        xmm5,LINE2(eax)                    ;
    movaps        xmm6,LINE3(eax)                    ;
    movaps        xmm7,LINE4(eax)                    ;

    movss        xmm0,DWORD PTR [esi]vSrc.w        ; xmm0 = ?.? | ?.? | ?.? | w
    shufps        xmm0,xmm0,0                        ; xmm0 = w | w | w | w

    subps        xmm4,LINE1(edx)                    ;
    subps        xmm5,LINE2(edx)                    ;
    subps        xmm6,LINE3(edx)                    ;
    subps        xmm7,LINE4(edx)                    ;

    mulps        xmm4,xmm0                        ;
    mulps        xmm5,xmm0                        ;
    mulps        xmm6,xmm0                        ;
    mulps        xmm7,xmm0                        ;

    addps        xmm4,LINE1(edx)                    ;
    addps        xmm5,LINE2(edx)                    ;
    addps        xmm6,LINE3(edx)                    ;
    addps        xmm7,LINE4(edx)                    ;
// ------------------------------------------------------------------    
// Calculating the next transformation matrixes address for prefetch
// ------------------------------------------------------------------    
    mov            eax,DWORD PTR [esi+PRD*(TYPE Vertex2W)]vSrc.m0    ; eax=vSrc[i+2].m0=index0
    mov            edx,DWORD PTR [esi+PRD*(TYPE Vertex2W)]vSrc.m1    ; edx=vSrc[i+2].m1=index1
    shl            eax,6                                            ; eax=index0*sizeof(Matrix4D)
    shl            edx,6                                            ; edx=index1*sizeof(Matrix4D)
    add            eax,DWORD PTR [bones]                            ; eax=index0*sizeof(Matrix4D)+offset(bones)
    add            edx,DWORD PTR [bones]                            ; edx=index1*sizeof(Matrix4D)+offset(bones)
// ------------------------------------------------------------------    
// Pt (xmm1,xmm2,xmm3) & Vec (xmm4,xmm5,xmm6) interleaved
// ------------------------------------------------------------------    
    movss        xmm1,DWORD PTR [esi]vSrc.pos.x    ; xmm1 = ?.? | ?.? | ?.? | P.x
    movss        xmm2,DWORD PTR [esi]vSrc.pos.y    ; xmm2 = ?.? | ?.? | ?.? | P.y
    movss        xmm3,DWORD PTR [esi]vSrc.pos.z    ; xmm3 = ?.? | ?.? | ?.? | P.z

    shufps        xmm1,xmm1,0                        ; xmm1 = P.x | P.x | P.x | P.x
    shufps        xmm2,xmm2,0                        ; xmm2 = P.y | P.y | P.y | P.y
    shufps        xmm3,xmm3,0                        ; xmm3 = P.z | P.z | P.z | P.z

    mulps        xmm1,xmm4                        ; xmm1 = P.x*_14 | P.x*_13 | P.x*_12 | P.x*_11
    mulps        xmm2,xmm5                        ; xmm2 = P.y*_24 | P.y*_23 | P.y*_22 | P.y*_21
    mulps        xmm3,xmm6                        ; xmm3 = P.z*_34 | P.z*_33 | P.z*_32 | P.z*_31

    prefetchnta    BYTE PTR [eax]                                    ;

    addps        xmm1,xmm2                        ; xmm1 = P.x*_14+P.y*_24 | P.x*_13+P.y*_23 |
                                                ;         P.x*_12+P.y*_22 | P.x*_11+P.y*_21
    addps        xmm3,xmm7                        ; xmm3 = P.z*_34+_44 | P.z*_33+_43 | 
                                                ;         P.z*_32+_42 | P.z*_31+_41
    addps        xmm3,xmm1                        ; xmm3 = P.x*_14+P.y*_24+P.z*_34+_44 | P.x*_13+P.y*_23+P.z*_33+_43 |
                                                ;         P.x*_12+P.y*_22+P.z*_32+_42 | P.x*_11+P.y*_21+P.z*_31+_41

    movss        xmm0,DWORD PTR [esi]vSrc.normal.x    ; xmm0 = ?.? | ?.? | ?.? | N.x
    movss        xmm2,DWORD PTR [esi]vSrc.normal.y    ; xmm2 = ?.? | ?.? | ?.? | N.y
    movss        xmm1,DWORD PTR [esi]vSrc.normal.z    ; xmm1 = ?.? | ?.? | ?.? | N.z

    shufps        xmm0,xmm0,0                        ; xmm0 = N.x | N.x | N.x | N.x
    shufps        xmm2,xmm2,0                        ; xmm2 = N.y | N.y | N.y | N.y
    shufps        xmm1,xmm1,0                        ; xmm1 = N.z | N.z | N.z | N.z

    mulps        xmm0,xmm4                        ; xmm0 = N.x*_14 | N.x*_13 | N.x*_12 | N.x*_11
    mulps        xmm2,xmm5                        ; xmm2 = N.y*_24 | N.y*_23 | N.y*_22 | N.y*_21
    mulps        xmm1,xmm6                        ; xmm1 = N.z*_34 | N.z*_33 | N.z*_32 | N.z*_31
    
    prefetchnta    BYTE PTR [edx]                                    ;

    addps        xmm0,xmm2                        ; xmm0 = N.x*_14+N.y*_24 | N.x*_13+N.y*_23 |
                                                ;         N.x*_12+N.y*_22 | N.x*_11+N.y*_21
    addps        xmm1,xmm0                        ; xmm1 = N.x*_14+N.y*_24+N.z*_34+_44 | N.x*_13+N.y*_23+N.z*_33+_43 |
                                                ;         N.x*_12+N.y*_22+N.z*_32+_42 | N.x*_11+N.y*_21+N.z*_31+_41
// ------------------------------------------------------------------    
// Swizling data for aligned store
// ------------------------------------------------------------------    
    movss        xmm7,xmm3                        ; xmm7 = ?.? | ?.? | ?.? | PX
    movss        xmm3,xmm1                        ; xmm3 = ?.? | PZ | PY | NX
    shufps        xmm3,xmm3,00100111b                ; xmm3 = NX | PZ | PY | ?.?
    movss        xmm3,xmm7                        ; xmm3 = NX | PZ | PY | PX

    shufps        xmm1,xmm1,11001001b                ; xmm1 = ?.? | NX | NZ | NY
    movhps        xmm1,MMWORD PTR [esi]vSrc.u        ; xmm1 = v | u | NZ | NY
// ------------------------------------------------------------------    
// Storing
// ------------------------------------------------------------------    
    movntps        XMMWORD PTR [edi],xmm3            ;
    movntps        XMMWORD PTR [edi+16],xmm1        ;
// ------------------------------------------------------------------    
    add            esi,TYPE Vertex2W        ; esi+=sizeof(Vertex2W)
    add            edi,TYPE VertexOut        ; edi+=sizeof(VertexOut)
// ------------------------------------------------------------------    
    dec            ecx                        ; ecx-=1
    jnz            new_vtx                    ; ecx==0 ? goto new_vtx : exit
// ------------------------------------------------------------------
    sfence                                ; write back all caches
// ------------------------------------------------------------------
}} // sse_Skin2

void sse_Skin3( const Vertex3W* vSrc, VertexOut* vDest, int nV, const Matrix4D* bones )
{__asm{
// ------------------------------------------------------------------
    mov            ecx,nV                    ; ecx=nV
// ------------------------------------------------------------------
    mov            esi,DWORD PTR [vSrc]    ; esi=vSrc
    mov            edi,DWORD PTR [vDest]    ; edi=vDest
// ------------------------------------------------------------------
    ALIGN        16                        ; 
    new_vtx:                            ; _new cycle iteration
// ------------------------------------------------------------------
// Calculating 1st, 2nd and 3rd transformation matrix address
// ------------------------------------------------------------------
    mov            eax,DWORD PTR [esi]vSrc.m0        ; eax=vSrc[i].m0=index0
    mov            edx,DWORD PTR [esi]vSrc.m1        ; edx=vSrc[i].m1=index1
    mov            ebx,DWORD PTR [esi]vSrc.m2        ; edx=vSrc[i].m2=index2
    shl            eax,6                            ; eax=index0*sizeof(Matrix4D)
    shl            edx,6                            ; edx=index1*sizeof(Matrix4D)
    shl            ebx,6                            ; ebx=index2*sizeof(Matrix4D)
    add            eax,DWORD PTR [bones]            ; eax=index0*sizeof(Matrix4D)+offset(bones)
    add            edx,DWORD PTR [bones]            ; edx=index1*sizeof(Matrix4D)+offset(bones)
    add            ebx,DWORD PTR [bones]            ; ebx=index2*sizeof(Matrix4D)+offset(bones)
// ------------------------------------------------------------------    
// Blending 3 matrixes
// ------------------------------------------------------------------    
    movss        xmm0,DWORD PTR [esi]vSrc.w0        ; xmm0 = ?.? | ?.? | ?.? | w0
    movss        xmm1,DWORD PTR [esi]vSrc.w1        ; xmm1 = ?.? | ?.? | ?.? | w1

    movaps        xmm4,LINE1(eax)                    ;

    shufps        xmm0,xmm0,0                        ; xmm0 = w0 | w0 | w0 | w0
    shufps        xmm1,xmm1,0                        ; xmm1 = w1 | w1 | w1 | w1
//- L1
    movaps        xmm2,LINE1(edx)                    ;

    subps        xmm4,LINE1(ebx);
    subps        xmm2,LINE1(ebx);

    mulps        xmm4,xmm0                        ;
    mulps        xmm2,xmm1                        ;

    addps        xmm4,LINE1(ebx);
    movaps        xmm5,LINE2(eax)                    ;
    movaps        xmm3,LINE2(edx)                    ;
    addps        xmm4,xmm2                        ;
//- L2
    subps        xmm5,LINE2(ebx);
    subps        xmm3,LINE2(ebx);

    mulps        xmm5,xmm0                        ;
    mulps        xmm3,xmm1                        ;

    addps        xmm5,LINE2(ebx);
    movaps        xmm6,LINE3(eax)                    ;
    movaps        xmm2,LINE3(edx)                    ;
    addps        xmm5,xmm3                        ;
//- L3
    subps        xmm6,LINE3(ebx)                    ;
    subps        xmm2,LINE3(ebx)                    ;

    mulps        xmm6,xmm0                        ;
    mulps        xmm2,xmm1                        ;

    addps        xmm6,LINE3(ebx)                    ;
    movaps        xmm7,LINE4(eax)                    ;
    movaps        xmm3,LINE4(edx)                    ;
    addps        xmm6,xmm2                        ;
//- L4
    subps        xmm7,LINE4(ebx)                    ;
    subps        xmm3,LINE4(ebx)                    ;

    mulps        xmm7,xmm0                        ;
    mulps        xmm3,xmm1                        ;

    addps        xmm7,LINE4(ebx)                    ;

    mov            eax,DWORD PTR [esi+PRD*(TYPE Vertex3W)]vSrc.m0    ; eax=vSrc[i+2].m0=index0
    mov            edx,DWORD PTR [esi+PRD*(TYPE Vertex3W)]vSrc.m1    ; edx=vSrc[i+2].m1=index1
    mov            ebx,DWORD PTR [esi+PRD*(TYPE Vertex3W)]vSrc.m2    ; ebx=vSrc[i+2].m2=index2
    shl            eax,6                                            ; eax=index0*sizeof(Matrix4D)
    shl            edx,6                                            ; edx=index1*sizeof(Matrix4D)
    shl            ebx,6                                            ; ebx=index2*sizeof(Matrix4D)

    addps        xmm7,xmm3                        ;
// ------------------------------------------------------------------    
// Calculating the next transformation matrixes address for prefetch
// ------------------------------------------------------------------    
    add            eax,DWORD PTR [bones]                            ; eax=index0*sizeof(Matrix4D)+offset(bones)
    add            edx,DWORD PTR [bones]                            ; edx=index1*sizeof(Matrix4D)+offset(bones)
    add            ebx,DWORD PTR [bones]                            ; ebx=index2*sizeof(Matrix4D)+offset(bones)
// ------------------------------------------------------------------    
// Pt (xmm1,xmm2,xmm3) & Vec (xmm4,xmm5,xmm6) interleaved
// ------------------------------------------------------------------    
    movss        xmm1,DWORD PTR [esi]vSrc.pos.x    ; xmm1 = ?.? | ?.? | ?.? | P.x
    movss        xmm2,DWORD PTR [esi]vSrc.pos.y    ; xmm2 = ?.? | ?.? | ?.? | P.y
    movss        xmm3,DWORD PTR [esi]vSrc.pos.z    ; xmm3 = ?.? | ?.? | ?.? | P.z

    shufps        xmm1,xmm1,0                        ; xmm1 = P.x | P.x | P.x | P.x
     shufps        xmm2,xmm2,0                        ; xmm2 = P.y | P.y | P.y | P.y
    shufps        xmm3,xmm3,0                        ; xmm3 = P.z | P.z | P.z | P.z

    prefetchnta    BYTE PTR [eax]                                    ;

    mulps        xmm1,xmm4                        ; xmm1 = P.x*_14 | P.x*_13 | P.x*_12 | P.x*_11
    mulps        xmm2,xmm5                        ; xmm2 = P.y*_24 | P.y*_23 | P.y*_22 | P.y*_21
    mulps        xmm3,xmm6                        ; xmm3 = P.z*_34 | P.z*_33 | P.z*_32 | P.z*_31

    addps        xmm1,xmm2                        ; xmm1 = P.x*_14+P.y*_24 | P.x*_13+P.y*_23 |
                                                ;         P.x*_12+P.y*_22 | P.x*_11+P.y*_21
    addps        xmm3,xmm7                        ; xmm3 = P.z*_34+_44 | P.z*_33+_43 | 
                                                ;         P.z*_32+_42 | P.z*_31+_41
    addps        xmm3,xmm1                        ; xmm3 = P.x*_14+P.y*_24+P.z*_34+_44 | P.x*_13+P.y*_23+P.z*_33+_43 |
                                                ;         P.x*_12+P.y*_22+P.z*_32+_42 | P.x*_11+P.y*_21+P.z*_31+_41

    movss        xmm0,DWORD PTR [esi]vSrc.normal.x    ; xmm0 = ?.? | ?.? | ?.? | N.x
    movss        xmm2,DWORD PTR [esi]vSrc.normal.y    ; xmm2 = ?.? | ?.? | ?.? | N.y
    movss        xmm1,DWORD PTR [esi]vSrc.normal.z    ; xmm1 = ?.? | ?.? | ?.? | N.z


    shufps        xmm0,xmm0,0                        ; xmm0 = N.x | N.x | N.x | N.x
    shufps        xmm2,xmm2,0                        ; xmm2 = N.y | N.y | N.y | N.y
    shufps        xmm1,xmm1,0                        ; xmm1 = N.z | N.z | N.z | N.z

    prefetchnta    BYTE PTR [edx]                                    ;

    mulps        xmm0,xmm4                        ; xmm0 = N.x*_14 | N.x*_13 | N.x*_12 | N.x*_11
    mulps        xmm2,xmm5                        ; xmm2 = N.y*_24 | N.y*_23 | N.y*_22 | N.y*_21
    mulps        xmm1,xmm6                        ; xmm1 = N.z*_34 | N.z*_33 | N.z*_32 | N.z*_31
    
    addps        xmm0,xmm2                        ; xmm0 = N.x*_14+N.y*_24 | N.x*_13+N.y*_23 |
                                                ;         N.x*_12+N.y*_22 | N.x*_11+N.y*_21
    addps        xmm1,xmm0                        ; xmm1 = N.x*_14+N.y*_24+N.z*_34+_44 | N.x*_13+N.y*_23+N.z*_33+_43 |
                                                ;         N.x*_12+N.y*_22+N.z*_32+_42 | N.x*_11+N.y*_21+N.z*_31+_41
// ------------------------------------------------------------------    
// Swizling data for aligned store
// ------------------------------------------------------------------    
    movss        xmm7,xmm3                        ; xmm7 = ?.? | ?.? | ?.? | PX
    movss        xmm3,xmm1                        ; xmm3 = ?.? | PZ | PY | NX
    shufps        xmm3,xmm3,00100111b                ; xmm3 = NX | PZ | PY | ?.?
    movss        xmm3,xmm7                        ; xmm3 = NX | PZ | PY | PX

    shufps        xmm1,xmm1,11001001b                ; xmm1 = ?.? | NX | NZ | NY

    prefetchnta    BYTE PTR [ebx]                                    ;

    movhps        xmm1,MMWORD PTR [esi]vSrc.u        ; xmm1 = v | u | NZ | NY
// ------------------------------------------------------------------    
// Storing
// ------------------------------------------------------------------    
    movntps        XMMWORD PTR [edi],xmm3            ;
    movntps        XMMWORD PTR [edi+16],xmm1        ;
// ------------------------------------------------------------------    
    add            esi,TYPE Vertex3W        ; esi+=sizeof(Vertex3W)
    add            edi,TYPE VertexOut        ; edi+=sizeof(VertexOut)
// ------------------------------------------------------------------    
    dec            ecx                        ; ecx-=1
    jnz            new_vtx                    ; ecx==0 ? goto new_vtx : exit
// ------------------------------------------------------------------
    sfence                                ; write back all caches
// ------------------------------------------------------------------
}} // sse_Skin3

__declspec(align(16)) const float the_one=1.0f;

void sse_Skin4( const Vertex4W* vSrc, VertexOut* vDest, int nV, const Matrix4D* bones )
{__asm{
// ------------------------------------------------------------------
    mov            ecx,nV                    ; ecx=nV
// ------------------------------------------------------------------
    mov            esi,DWORD PTR [vSrc]    ; esi=vSrc
    mov            edi,DWORD PTR [vDest]    ; edi=vDest
// ------------------------------------------------------------------
    ALIGN        16                        ; 
    new_vtx:                            ; _new cycle iteration
// ------------------------------------------------------------------
    push        ecx                        ; :(
// ------------------------------------------------------------------
// Calculating 1st, 2nd ,3rd & 4th transformation matrix address
// ------------------------------------------------------------------
    mov            eax,DWORD PTR [esi]vSrc.m0        ; eax=vSrc[i].m0=index0
    mov            edx,DWORD PTR [esi]vSrc.m1        ; edx=vSrc[i].m1=index1
    mov            ebx,DWORD PTR [esi]vSrc.m2        ; edx=vSrc[i].m2=index2
    mov            ecx,DWORD PTR [esi]vSrc.m3        ; edx=vSrc[i].m2=index3
    shl            eax,6                            ; eax=index0*sizeof(Matrix4D)
    shl            edx,6                            ; edx=index1*sizeof(Matrix4D)
    shl            ebx,6                            ; ebx=index2*sizeof(Matrix4D)
    shl            ecx,6                            ; ecx=index3*sizeof(Matrix4D)
    add            eax,DWORD PTR [bones]            ; eax=index0*sizeof(Matrix4D)+offset(bones)
    add            edx,DWORD PTR [bones]            ; edx=index1*sizeof(Matrix4D)+offset(bones)
    add            ebx,DWORD PTR [bones]            ; ebx=index2*sizeof(Matrix4D)+offset(bones)
    add            ecx,DWORD PTR [bones]            ; ecx=index3*sizeof(Matrix4D)+offset(bones)
// ------------------------------------------------------------------    
// Blending 3 matrixes ...
// ------------------------------------------------------------------    
    movss        xmm0,DWORD PTR [esi]vSrc.w0        ; xmm0 = ?.? | ?.? | ?.? | w0
    movss        xmm1,DWORD PTR [esi]vSrc.w1        ; xmm1 = ?.? | ?.? | ?.? | w1
    movss        xmm2,DWORD PTR [esi]vSrc.w2        ; xmm2 = ?.? | ?.? | ?.? | w2

    shufps        xmm0,xmm0,0                        ; xmm0 = w0 | w0 | w0 | w0
    shufps        xmm1,xmm1,0                        ; xmm1 = w1 | w1 | w1 | w1
    shufps        xmm2,xmm2,0                        ; xmm2 = w2 | w2 | w2 | w2
// ------------------------------------------------------------------    
    movaps        xmm4,LINE1(eax)                    ;
    movaps        xmm5,LINE2(eax)                    ;
    movaps        xmm6,LINE3(eax)                    ;
    movaps        xmm7,LINE4(eax)                    ;
//- L1
    subps        xmm4,LINE1(ecx)                    ;
    mulps        xmm4,xmm0                        ;
    
    movaps        xmm3,LINE1(edx)                    ;
    subps        xmm3,LINE1(ecx)                    ;
    mulps        xmm3,xmm1                        ;
    addps        xmm4,xmm3                        ;

    movaps        xmm3,LINE1(ebx)                    ;
    subps        xmm3,LINE1(ecx)                    ;
    mulps        xmm3,xmm2                        ;
    addps        xmm4,xmm3                        ;

    addps        xmm4,LINE1(ecx)                    ;
//- L2
    subps        xmm5,LINE2(ecx)                    ;
    mulps        xmm5,xmm0                        ;
    
    movaps        xmm3,LINE2(edx)                    ;
    subps        xmm3,LINE2(ecx)                    ;
    mulps        xmm3,xmm1                        ;
    addps        xmm5,xmm3                        ;

    movaps        xmm3,LINE2(ebx)                    ;
    subps        xmm3,LINE2(ecx)                    ;
    mulps        xmm3,xmm2                        ;
    addps        xmm5,xmm3                        ;

    addps        xmm5,LINE2(ecx)                    ;
//- L3
    subps        xmm6,LINE3(ecx)                    ;
    mulps        xmm6,xmm0                        ;
    
    movaps        xmm3,LINE3(edx)                    ;
    subps        xmm3,LINE3(ecx)                    ;
    mulps        xmm3,xmm1                        ;
    addps        xmm6,xmm3                        ;

    movaps        xmm3,LINE3(ebx)                    ;
    subps        xmm3,LINE3(ecx)                    ;
    mulps        xmm3,xmm2                        ;
    addps        xmm6,xmm3                        ;

    addps        xmm6,LINE3(ecx)                    ;
//- L4
    subps        xmm7,LINE4(ecx)                    ;
    mulps        xmm7,xmm0                        ;
    
    movaps        xmm3,LINE4(edx)                    ;
    subps        xmm3,LINE4(ecx)                    ;
    mulps        xmm3,xmm1                        ;
    addps        xmm7,xmm3                        ;

    movaps        xmm3,LINE4(ebx)                    ;
    subps        xmm3,LINE4(ecx)                    ;
    mulps        xmm3,xmm2                        ;
    addps        xmm7,xmm3                        ;

    addps        xmm7,LINE4(ecx)                    ;
// ------------------------------------------------------------------    
// Calculating the next transformation matrixes address for prefetch
// ------------------------------------------------------------------    
    mov            eax,DWORD PTR [esi+PRD*(TYPE Vertex4W)]vSrc.m0    ; eax=vSrc[i+2].m0=index0
    mov            edx,DWORD PTR [esi+PRD*(TYPE Vertex4W)]vSrc.m1    ; edx=vSrc[i+2].m1=index1
    mov            ebx,DWORD PTR [esi+PRD*(TYPE Vertex4W)]vSrc.m2    ; ebx=vSrc[i+2].m2=index2
    mov            ecx,DWORD PTR [esi+PRD*(TYPE Vertex4W)]vSrc.m3    ; ecx=vSrc[i+2].m3=index3
    shl            eax,6                                            ; eax=index0*sizeof(Matrix4D)
    shl            edx,6                                            ; edx=index1*sizeof(Matrix4D)
    shl            ebx,6                                            ; ebx=index2*sizeof(Matrix4D)
    shl            ecx,6                                            ; ecx=index3*sizeof(Matrix4D)
    add            eax,DWORD PTR [bones]                            ; eax=index0*sizeof(Matrix4D)+offset(bones)
    add            edx,DWORD PTR [bones]                            ; edx=index1*sizeof(Matrix4D)+offset(bones)
    add            ebx,DWORD PTR [bones]                            ; ebx=index2*sizeof(Matrix4D)+offset(bones)
    add            ecx,DWORD PTR [bones]                            ; ecx=index3*sizeof(Matrix4D)+offset(bones)
// ------------------------------------------------------------------    
// Pt (xmm1,xmm2,xmm3) & Vec (xmm4,xmm5,xmm6) interleaved
// ------------------------------------------------------------------    
    movss        xmm1,DWORD PTR [esi]vSrc.pos.x    ; xmm1 = ?.? | ?.? | ?.? | P.x
    movss        xmm2,DWORD PTR [esi]vSrc.pos.y    ; xmm2 = ?.? | ?.? | ?.? | P.y
    movss        xmm3,DWORD PTR [esi]vSrc.pos.z    ; xmm3 = ?.? | ?.? | ?.? | P.z

    shufps        xmm1,xmm1,0                        ; xmm1 = P.x | P.x | P.x | P.x
     shufps        xmm2,xmm2,0                        ; xmm2 = P.y | P.y | P.y | P.y
    shufps        xmm3,xmm3,0                        ; xmm3 = P.z | P.z | P.z | P.z

    prefetchnta    BYTE PTR [eax]                                    ;

    mulps        xmm1,xmm4                        ; xmm1 = P.x*_14 | P.x*_13 | P.x*_12 | P.x*_11
    mulps        xmm2,xmm5                        ; xmm2 = P.y*_24 | P.y*_23 | P.y*_22 | P.y*_21
    mulps        xmm3,xmm6                        ; xmm3 = P.z*_34 | P.z*_33 | P.z*_32 | P.z*_31

    addps        xmm1,xmm2                        ; xmm1 = P.x*_14+P.y*_24 | P.x*_13+P.y*_23 |
                                                ;         P.x*_12+P.y*_22 | P.x*_11+P.y*_21
    addps        xmm3,xmm7                        ; xmm3 = P.z*_34+_44 | P.z*_33+_43 | 
                                                ;         P.z*_32+_42 | P.z*_31+_41
    addps        xmm3,xmm1                        ; xmm3 = P.x*_14+P.y*_24+P.z*_34+_44 | P.x*_13+P.y*_23+P.z*_33+_43 |
                                                ;         P.x*_12+P.y*_22+P.z*_32+_42 | P.x*_11+P.y*_21+P.z*_31+_41

    movss        xmm0,DWORD PTR [esi]vSrc.normal.x    ; xmm0 = ?.? | ?.? | ?.? | N.x
    movss        xmm2,DWORD PTR [esi]vSrc.normal.y    ; xmm2 = ?.? | ?.? | ?.? | N.y
    movss        xmm1,DWORD PTR [esi]vSrc.normal.z    ; xmm1 = ?.? | ?.? | ?.? | N.z


    shufps        xmm0,xmm0,0                        ; xmm0 = N.x | N.x | N.x | N.x
    shufps        xmm2,xmm2,0                        ; xmm2 = N.y | N.y | N.y | N.y
    shufps        xmm1,xmm1,0                        ; xmm1 = N.z | N.z | N.z | N.z

    prefetchnta    BYTE PTR [edx]                                    ;

    mulps        xmm0,xmm4                        ; xmm0 = N.x*_14 | N.x*_13 | N.x*_12 | N.x*_11
    mulps        xmm2,xmm5                        ; xmm2 = N.y*_24 | N.y*_23 | N.y*_22 | N.y*_21
    mulps        xmm1,xmm6                        ; xmm1 = N.z*_34 | N.z*_33 | N.z*_32 | N.z*_31
    
    addps        xmm0,xmm2                        ; xmm0 = N.x*_14+N.y*_24 | N.x*_13+N.y*_23 |
                                                ;         N.x*_12+N.y*_22 | N.x*_11+N.y*_21
    addps        xmm1,xmm0                        ; xmm1 = N.x*_14+N.y*_24+N.z*_34+_44 | N.x*_13+N.y*_23+N.z*_33+_43 |
                                                ;         N.x*_12+N.y*_22+N.z*_32+_42 | N.x*_11+N.y*_21+N.z*_31+_41
// ------------------------------------------------------------------    
// Swizling data for aligned store
// ------------------------------------------------------------------    
    movss        xmm7,xmm3                        ; xmm7 = ?.? | ?.? | ?.? | PX
    movss        xmm3,xmm1                        ; xmm3 = ?.? | PZ | PY | NX
    shufps        xmm3,xmm3,00100111b                ; xmm3 = NX | PZ | PY | ?.?
    movss        xmm3,xmm7                        ; xmm3 = NX | PZ | PY | PX

    shufps        xmm1,xmm1,11001001b                ; xmm1 = ?.? | NX | NZ | NY

    prefetchnta    BYTE PTR [ecx]                                    ;

    movhps        xmm1,MMWORD PTR [esi]vSrc.u        ; xmm1 = v | u | NZ | NY
// ------------------------------------------------------------------    
// Storing
// ------------------------------------------------------------------    
    movntps        XMMWORD PTR [edi],xmm3            ;
    movntps        XMMWORD PTR [edi+16],xmm1        ;
// ------------------------------------------------------------------    
    add            esi,TYPE Vertex4W        ; esi+=sizeof(Vertex4W)
    add            edi,TYPE VertexOut        ; edi+=sizeof(VertexOut)

    prefetchnta    BYTE PTR [ebx]                                    ;

// ------------------------------------------------------------------    
    pop            ecx                        ; :(
    dec            ecx                        ; ecx-=1
    jnz            new_vtx                    ; ecx==0 ? goto new_vtx : exit
// ------------------------------------------------------------------
    sfence                                ; write back all caches
// ------------------------------------------------------------------
}} // sse_Skin4
