/*****************************************************************
/*  File:   rsVertex.cpp                                          
/*  Author: Silver, Copyright (C) GSC Game World                 
/*  Date:   January 2002                                         
/*****************************************************************/
#include "stdafx.h"
#include "rsVertex.h"

void* Vertex::CreateVBuf( VertexFormat vf, int numVert )
{
    return aligned_new<BYTE>( numVert*CreateVertexDeclaration( vf ).m_VertexSize, 32 );
} // Vertex::CreateVBuf

int Vertex::GetStride( VertexFormat vf )
{
    return CreateVertexDeclaration( vf ).m_VertexSize;
} // Vertex::GetStride

VertexDeclaration CreateVertexDeclaration( VertexFormat vf )
{
    VertexDeclaration vd;
    switch (vf)
    {
    case vfVertexTnL:   vd << vcPositionRHW << vcDiffuse << vcSpecular << vcTexCoor0; break;
    case vfVertex2t:    vd << vcPosition << vcDiffuse << vcTexCoor0 << vcTexCoor1; break;    
    case vfVertexN:     vd << vcPosition << vcNormal << vcTexCoor0; break;
    case vfVertexBump:  vd << vcPosition << vcNormal << vcTexCoor0 << vcBinormal << vcTangent; break;
    case vfVertexTnL2:  vd << vcPositionRHW << vcDiffuse << vcTexCoor0 << vcTexCoor1; break;
    case vfVertexT:     vd << vcPosition << vcTexCoor0; break;
    case vfVertexMP1:   vd << vcPosition << vcBlendIdx << vcDiffuse << vcSpecular << vcTexCoor0 << vcTexCoor1; break;
    case vfVertexW1:    vd << vcPosition << vcBlendIdx << vcNormal << vcDiffuse << vcSpecular << 
                                vcTexCoor0 << vcTexCoor1; break;
    case vfVertexW2:    vd << vcPosition << vcBlend0 << vcBlendIdx << vcNormal << vcDiffuse << 
                                vcTexCoor0 << vcTexCoor1; break;
    case vfVertexW3:    vd << vcPosition << vcBlend0 << vcBlend1 << vcBlendIdx << vcNormal << 
                                vcDiffuse << vcTexCoor0 << vcTexCoor1; break;
    case vfVertexW4:    vd << vcPosition << vcBlend0 << vcBlend1 << vcBlend2 << vcBlendIdx << vcNormal << 
                                vcDiffuse << vcSpecular << vcTexCoor0 << vcTexCoor1; break;
    case vfVertexTnL2S: vd << vcPositionRHW << vcDiffuse << vcSpecular << vcTexCoor0 << vcTexCoor1; break;
    case vfVertexN2T:   vd << vcPosition << vcNormal << vcDiffuse << vcSpecular << vcTexCoor0 << vcTexCoor1; break;
    case vfVertexXYZD:  vd << vcPosition << vcDiffuse; break;    
    case vfVertexXYZW:  vd << vcPositionRHW; break;
    case vfVertexTS:    vd << vcPosition << vcBlend0 << vcDiffuse << vcSpecular << vcTexCoor0; break;
    case vfVertex1W:    vd << vcPosition << vcNormal << vcBlendIdx0 << vcTexCoor0; break;
    case vfVertex2W:    vd << vcPosition << vcNormal << vcBlendIdx0 << vcBlendIdx1 << vcBlend0 << 
                                vcTexCoor0; break;
    case vfVertex2F:    vd << vcDiffuse << vcSpecular; break;
    case vfVertex3W:    vd << vcPosition << vcNormal << vcBlendIdx0 << vcBlendIdx1 << vcBlendIdx2 << 
                                vcBlend0 << vcBlend1 << vcTexCoor0; break;
    case vfVertex4W:    vd << vcPosition << vcNormal << vcBlendIdx0 << vcBlendIdx1 << vcBlendIdx2 << 
                                vcBlendIdx3 << vcBlend0 << vcBlend1 << vcBlend2 << vcTexCoor0; break;
	case vfVertexUV3C:  vd << vcPosition << vcDiffuse << vcSpecular << vcColor2 << vcTexCoor0; break;
	case vfVertex5C:	vd << vcPosition << vcDiffuse << vcSpecular << vcColor2 << vcColor3; break;
    }
    return vd;
} // CreateVertexDeclaration
