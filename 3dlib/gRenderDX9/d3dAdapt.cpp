/*****************************************************************************/
/*	File:	d3dAdapt.cpp
/*  Desc:	Utilities form mapping d3d constants/enumerations to/from the engine
/*	Author:	Ruslan Shestopalyuk
/*	Date:	02.11.2004
/*****************************************************************************/

#include "gRenderPch.h"
#include "d3dAdapt.h"


//-----------------------------------------------------------------------------
// CreateVDecl
//-----------------------------------------------------------------------------
// Converts engine VertexDeclaration into a Direct3D9 vertex declaration.
//
// The engine declaration can contain all vertex usages supported by
// ConvertVElemUsage(). MapsToD3DVElemUsage() currently returns true for
// every usage, so all engine vertex elements are passed to D3D9.
//
// The final D3DDECLTYPE_UNUSED element with Stream = 0xFF is required
// as the terminator of a D3D9 vertex declaration.
//-----------------------------------------------------------------------------

IDirect3DVertexDeclaration9 *CreateVDecl(
    IDirect3DDevice9 *pDevice,
    const VertexDeclaration &vdecl)
{
    if (!pDevice)
        return NULL;

    if (vdecl.m_NElements <= 0)
        return NULL;

    D3DVERTEXELEMENT9 elem[MAXD3DDECLLENGTH + 1];

    int cElem = 0;

    for (int i = 0;
        i < vdecl.m_NElements && cElem < MAXD3DDECLLENGTH;
        i++)
    {
        // Keep the original engine behavior:
        // only elements explicitly supported by the adapter
        // are copied to the D3D declaration.
        if (!MapsToD3DVElemUsage(vdecl.m_Element[i].m_Usage))
            continue;

        elem[cElem].Stream =
            vdecl.m_Element[i].m_Stream;

        elem[cElem].Offset =
            vdecl.m_Element[i].m_Offset;

        elem[cElem].Type =
            ConvertVElemType(vdecl.m_Element[i].m_Type);

        elem[cElem].Method =
            D3DDECLMETHOD_DEFAULT;

        elem[cElem].Usage =
            ConvertVElemUsage(vdecl.m_Element[i].m_Usage);

        elem[cElem].UsageIndex =
            GetVElemUsageIndex(vdecl.m_Element[i].m_Usage);

        cElem++;
    }

    // D3D9 declaration terminator.
    elem[cElem].Stream = 0xFF;
    elem[cElem].Offset = 0;
    elem[cElem].Type = D3DDECLTYPE_UNUSED;
    elem[cElem].Method = D3DDECLMETHOD_DEFAULT;
    elem[cElem].Usage = 0;
    elem[cElem].UsageIndex = 0;

    IDirect3DVertexDeclaration9 *pDecl = NULL;

    HRESULT hr = pDevice->CreateVertexDeclaration(
        elem,
        &pDecl
    );

    if (FAILED(hr))
    {
        pDecl = NULL;
    }

    return pDecl;
} // CreateVDecl


//-----------------------------------------------------------------------------
// CreateFVF
//-----------------------------------------------------------------------------
// Converts engine VertexDeclaration into a Direct3D9 FVF.
//
// FVF is much more limited than a full D3D9 vertex declaration.
//
// The following engine elements cannot be represented directly by FVF:
//   vcBinormal
//   vcTangent
//   vcColor2
//   vcColor3
//
// For those declarations this function returns 0.
//
// Blend weights and texture coordinate counts are collected first,
// because D3DFVF_XYZB1..D3DFVF_XYZB4 and D3DFVF_TEX1..D3DFVF_TEX8
// are aggregate FVF flags and should not be constructed by repeatedly
// OR-ing/clearing individual flags.
//-----------------------------------------------------------------------------

DWORD CreateFVF(const VertexDeclaration &vdecl)
{
    DWORD fvf = 0;

    // Maximum number of blend weights used by the declaration.
    int numBlendWeights = 0;

    // True if the declaration contains blend indices.
    bool hasBlendIdx = false;

    // Number of texture coordinate sets.
    int numTexCoords = 0;

    for (int i = 0; i < vdecl.m_NElements; i++)
    {
        switch (vdecl.m_Element[i].m_Usage)
        {
            //-----------------------------------------------------------------
            // Position
            //-----------------------------------------------------------------

        case vcPosition:
            fvf |= D3DFVF_XYZ;
            break;

        case vcPositionRHW:
            fvf |= D3DFVF_XYZRHW;
            break;


            //-----------------------------------------------------------------
            // Blend weights
            //-----------------------------------------------------------------

        case vcBlend0:
            if (numBlendWeights < 1)
                numBlendWeights = 1;
            break;

        case vcBlend1:
            if (numBlendWeights < 2)
                numBlendWeights = 2;
            break;

        case vcBlend2:
            if (numBlendWeights < 3)
                numBlendWeights = 3;
            break;

        case vcBlend3:
            if (numBlendWeights < 4)
                numBlendWeights = 4;
            break;


            //-----------------------------------------------------------------
            // Blend indices
            //-----------------------------------------------------------------

        case vcBlendIdx:
        case vcBlendIdx0:
        case vcBlendIdx1:
        case vcBlendIdx2:
        case vcBlendIdx3:
            hasBlendIdx = true;
            break;


            //-----------------------------------------------------------------
            // Normal
            //-----------------------------------------------------------------

        case vcNormal:
            fvf |= D3DFVF_NORMAL;
            break;


            //-----------------------------------------------------------------
            // These vertex components require a vertex declaration.
            //-----------------------------------------------------------------

        case vcBinormal:
        case vcTangent:
        case vcColor2:
        case vcColor3:
            return 0;


            //-----------------------------------------------------------------
            // Colors
            //-----------------------------------------------------------------

        case vcDiffuse:
            fvf |= D3DFVF_DIFFUSE;
            break;

        case vcSpecular:
            fvf |= D3DFVF_SPECULAR;
            break;


            //-----------------------------------------------------------------
            // Texture coordinates
            //-----------------------------------------------------------------

        case vcTexCoor0:
            if (numTexCoords < 1)
                numTexCoords = 1;
            break;

        case vcTexCoor1:
            if (numTexCoords < 2)
                numTexCoords = 2;
            break;

        case vcTexCoor2:
            if (numTexCoords < 3)
                numTexCoords = 3;
            break;

        case vcTexCoor3:
            if (numTexCoords < 4)
                numTexCoords = 4;
            break;

        case vcTexCoor4:
            if (numTexCoords < 5)
                numTexCoords = 5;
            break;

        case vcTexCoor5:
            if (numTexCoords < 6)
                numTexCoords = 6;
            break;

        case vcTexCoor6:
            if (numTexCoords < 7)
                numTexCoords = 7;
            break;

        case vcTexCoor7:
            if (numTexCoords < 8)
                numTexCoords = 8;
            break;
        }
    }


    //---------------------------------------------------------------------
    // Blend weights
    //---------------------------------------------------------------------

    switch (numBlendWeights)
    {
    case 0:
        break;

    case 1:
        fvf |= D3DFVF_XYZB1;
        break;

    case 2:
        fvf |= D3DFVF_XYZB2;
        break;

    case 3:
        fvf |= D3DFVF_XYZB3;
        break;

    case 4:
        fvf |= D3DFVF_XYZB4;
        break;

    default:
        return 0;
    }


    //---------------------------------------------------------------------
    // Blend indices
    //---------------------------------------------------------------------

    if (hasBlendIdx)
    {
        // An index without blend weights cannot form a valid FVF
        // skinning declaration.
        if (numBlendWeights == 0)
            return 0;

        fvf |= D3DFVF_LASTBETA_UBYTE4;
    }


    //---------------------------------------------------------------------
    // Texture coordinates
    //---------------------------------------------------------------------

    switch (numTexCoords)
    {
    case 0:
        break;

    case 1:
        fvf |= D3DFVF_TEX1;
        break;

    case 2:
        fvf |= D3DFVF_TEX2;
        break;

    case 3:
        fvf |= D3DFVF_TEX3;
        break;

    case 4:
        fvf |= D3DFVF_TEX4;
        break;

    case 5:
        fvf |= D3DFVF_TEX5;
        break;

    case 6:
        fvf |= D3DFVF_TEX6;
        break;

    case 7:
        fvf |= D3DFVF_TEX7;
        break;

    case 8:
        fvf |= D3DFVF_TEX8;
        break;

    default:
        return 0;
    }

    return fvf;
} // CreateFVF