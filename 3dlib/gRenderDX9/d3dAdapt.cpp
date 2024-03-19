/*****************************************************************************/
/*	File:	d3dAdapt.cpp
/*  Desc:	Utilities form mapping d3d constants/enumerations to/from the engine
/*	Author:	Ruslan Shestopalyuk
/*	Date:	02.11.2004
/*****************************************************************************/
#include "gRenderPch.h"
#include "d3dAdapt.h"

IDirect3DVertexDeclaration9* CreateVDecl( IDirect3DDevice9* pDevice, const VertexDeclaration& vdecl )
{
    if (!pDevice) return NULL;
    IDirect3DVertexDeclaration9* pDecl = NULL;
    D3DVERTEXELEMENT9 elem[MAXD3DDECLLENGTH + 1];
    int cElem = 0;
    for (int i = 0; i < vdecl.m_NElements; i++)
    {
        elem[cElem].Stream     = vdecl.m_Element[i].m_Stream;
        elem[cElem].Offset     = vdecl.m_Element[i].m_Offset;
        elem[cElem].Type       = ConvertVElemType  ( vdecl.m_Element[i].m_Type );
        elem[cElem].Method     = D3DDECLMETHOD_DEFAULT; 
        elem[cElem].Usage      = ConvertVElemUsage ( vdecl.m_Element[i].m_Usage );
        elem[cElem].UsageIndex = GetVElemUsageIndex( vdecl.m_Element[i].m_Usage );
        if (MapsToD3DVElemUsage( vdecl.m_Element[i].m_Usage )) cElem++;
    }
    //  end element
    elem[cElem].Stream     = 0xFF; 
    elem[cElem].Offset     = 0;
    elem[cElem].Type       = D3DDECLTYPE_UNUSED;
    elem[cElem].Method     = 0;
    elem[cElem].Usage      = 0;
    elem[cElem].UsageIndex = 0;
    pDevice->CreateVertexDeclaration( elem, &pDecl );
    return pDecl;
} // CreateVDecl

DWORD CreateFVF( const VertexDeclaration& vdecl )
{
    /*    
    D3DVERTEXELEMENT9 elem[MAXD3DDECLLENGTH + 1];
    int cElem = 0;
    for (int i = 0; i < vdecl.m_NElements; i++)
    {
        elem[cElem].Stream     = vdecl.m_Element[i].m_Stream;
        elem[cElem].Offset     = vdecl.m_Element[i].m_Offset;
        elem[cElem].Type       = ConvertVElemType  ( vdecl.m_Element[i].m_Type );
        elem[cElem].Method     = D3DDECLMETHOD_DEFAULT; 
        elem[cElem].Usage      = ConvertVElemUsage ( vdecl.m_Element[i].m_Usage );
        elem[cElem].UsageIndex = GetVElemUsageIndex( vdecl.m_Element[i].m_Usage );
        if (MapsToD3DVElemUsage( vdecl.m_Element[i].m_Usage )) cElem++;
    }
    //  end element
    elem[cElem].Stream     = 0xFF; 
    elem[cElem].Offset     = 0;
    elem[cElem].Type       = D3DDECLTYPE_UNUSED;
    elem[cElem].Method     = 0;
    elem[cElem].Usage      = 0;
    elem[cElem].UsageIndex = 0;
    DWORD fvf = 0;
    HRESULT H=D3DXFVFFromDeclarator( elem, &fvf );
    //DX_CHK( H );

    return fvf;
    */    

    DWORD fvf = 0;
    for (int i = 0; i < vdecl.m_NElements; i++)
    {
        switch (vdecl.m_Element[i].m_Usage)
        {
        case vcPosition		: fvf |= D3DFVF_XYZ;    break; 
        case vcPositionRHW	: fvf |= D3DFVF_XYZRHW; break; 

        case vcBlend0		: fvf |= D3DFVF_XYZB1; break; 
        case vcBlend1		: fvf |= D3DFVF_XYZB2; fvf &= ~D3DFVF_XYZB1; break; 
        case vcBlend2		: fvf |= D3DFVF_XYZB3; fvf &= ~D3DFVF_XYZB2; break; 
        case vcBlend3		: fvf |= D3DFVF_XYZB4; fvf &= ~D3DFVF_XYZB3; break; 
        case vcBlendIdx	    : fvf |= D3DFVF_LASTBETA_UBYTE4; break; 

        case vcNormal		: fvf |= D3DFVF_NORMAL; break;
        case vcBinormal		: 
        case vcTangent		: 
		case vcColor2		:
		case vcColor3		: return 0;//FVF shoud not be used

        case vcDiffuse		: fvf |= D3DFVF_DIFFUSE; break;
        case vcSpecular		: fvf |= D3DFVF_SPECULAR; break;

        case vcTexCoor0		: fvf |= D3DFVF_TEX1; break; 
        case vcTexCoor1		: fvf |= D3DFVF_TEX2; fvf &= ~D3DFVF_TEX1; break; 
        case vcTexCoor2		: fvf |= D3DFVF_TEX3; fvf &= ~D3DFVF_TEX2; break; 
        case vcTexCoor3		: fvf |= D3DFVF_TEX4; fvf &= ~D3DFVF_TEX3; break; 
        case vcTexCoor4		: fvf |= D3DFVF_TEX5; fvf &= ~D3DFVF_TEX4; break; 
        case vcTexCoor5		: fvf |= D3DFVF_TEX6; fvf &= ~D3DFVF_TEX5; break; 
        case vcTexCoor6		: fvf |= D3DFVF_TEX7; fvf &= ~D3DFVF_TEX6; break; 
        case vcTexCoor7		: fvf |= D3DFVF_TEX8; fvf &= ~D3DFVF_TEX7; break; 
        }
    }    
    return fvf;
} // CreateVDecl