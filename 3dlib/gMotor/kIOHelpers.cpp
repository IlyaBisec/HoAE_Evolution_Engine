/*****************************************************************************/
/*    File:    kIOHelpers.cpp
/*    Desc:    Auxiliary io routines
/*    Author:    Ruslan Shestopalyuk
/*    Date:    20.02.2003
/*****************************************************************************/
#include "stdafx.h"
#include "kIOHelpers.h"

#ifndef _INLINES
#include "kIOHelpers.inl"
#endif // _INLINES

OutStream&    operator<<( OutStream& os, const BaseMesh& bm )
{
    VertexFormat    vf = bm.getVertexFormat();
    PrimitiveType    pt = bm.getPriType(); 

    os << bm.getNVert() << bm.getNInd() << bm.getNPri() << bm.GetFlagsByte();
    os.Write( &vf, sizeof( vf ) );
    os.Write( &pt, sizeof( pt ) );
    if (bm.getNVert() > 0) os.Write( bm.getVertexData(), bm.getNVert() * Vertex::GetStride( vf ) );
    if (bm.getNInd() > 0)  os.Write( bm.getIndices(), bm.getNInd() * sizeof( WORD ) );
    
#ifndef _INLINES
    bm.CheckSanity();
#endif // _INLINES

    return os;
} // operator<< 

InStream&    operator>>( InStream& is, BaseMesh& bm )
{
    int        nV;
    int        nI;
    int        nPri;
    BYTE    flags;

    VertexFormat    vf;
    PrimitiveType    pt;

    is >> nV >> nI >> nPri >> flags;
    is.Read( &vf, sizeof( vf ) );
    is.Read( &pt, sizeof( pt ) );

    bm.create( nV + 2, nI, vf, pt );
    if (nV > 0) is.Read( bm.getVertexData(), nV * Vertex::GetStride( vf ) );
    if (nI > 0) is.Read( bm.getIndices(), nI * 2 );
    
    bm.setNVert        ( nV    );
    bm.setNInd        ( nI    );
    bm.setNPri        ( nPri    );
    bm.SetFlagsByte    ( flags );

#ifndef _INLINES
    bm.CheckSanity();
#endif // _INLINES

    return is;
} // operator>>

