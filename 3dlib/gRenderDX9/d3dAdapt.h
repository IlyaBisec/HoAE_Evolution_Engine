/*****************************************************************************/
/*	File:	d3dAdapt.h
/*  Desc:	Utilities form mapping d3d constants/enumerations to/from the engine
/*	Author:	Ruslan Shestopalyuk
/*	Date:	02.11.2004
/*****************************************************************************/
#ifndef __D3DADAPT_H__
#define __D3DADAPT_H__
#include "IRenderSystem.h"
#include "ITexture.h"
#include "IVertexBuffer.h"

inline D3DFORMAT ConvertDepthStencilFormat( DepthStencilFormat dsFormat )
{
    switch (dsFormat)
    {
        case dsfD16Lockable	: return D3DFMT_D16_LOCKABLE;
        case dsfD32			: return D3DFMT_D32;
        case dsfD15S1		: return D3DFMT_D15S1;
        case dsfD24S8		: return D3DFMT_D24S8;
        case dsfD16			: return D3DFMT_D16;
    }
    return D3DFMT_D16;
} // ConvertDepthStencilFormat

inline DepthStencilFormat ConvertDepthStencilFormat( D3DFORMAT dsFormat )
{
    switch (dsFormat)
    {
        case D3DFMT_D16_LOCKABLE	: return dsfD16Lockable;
        case D3DFMT_D32			    : return dsfD32;
        case D3DFMT_D15S1		    : return dsfD15S1;
        case D3DFMT_D24S8		    : return dsfD24S8;
        case D3DFMT_D16			    : return dsfD16;
    }
    return dsfNone;
} // ConvertDepthStencilFormat

inline D3DFORMAT ConvertColorFormat( ColorFormat clrFormat )
{
    D3DFORMAT d3dcf = D3DFMT_A4R4G4B4;
    switch (clrFormat)
    {
        case cfARGB4444: d3dcf = D3DFMT_A4R4G4B4;	    break;
        case cfXRGB1555: d3dcf = D3DFMT_X1R5G5B5;	    break;
        case cfARGB8888: d3dcf = D3DFMT_A8R8G8B8;	    break;
        case cfXRGB8888: d3dcf = D3DFMT_X8R8G8B8;	    break;
        case cfRGB888:   d3dcf = D3DFMT_R8G8B8;		    break;
        case cfA8:		 d3dcf = D3DFMT_A8;		        break;
        case cfRGB565:   d3dcf = D3DFMT_R5G6B5;	        break;
        case cfV8U8:     d3dcf = D3DFMT_V8U8;	        break;
        case cfR16F:     d3dcf = D3DFMT_R16F;	        break;     
        case cfGR16F:    d3dcf = D3DFMT_G16R16F;	    break;     
        case cfABGR16F:  d3dcf = D3DFMT_A16B16G16R16F;	break;     
        case cfR32F:     d3dcf = D3DFMT_R32F;		    break;     
        case cfGR32F:    d3dcf = D3DFMT_G32R32F;	    break;     
        case cfABGR32F:  d3dcf = D3DFMT_A32B32G32R32F;	break;  
        case cfDXT1:     d3dcf = D3DFMT_DXT1;	        break;  
        case cfDXT2:     d3dcf = D3DFMT_DXT2;	        break;
        case cfDXT3:     d3dcf = D3DFMT_DXT3;	        break;
        case cfDXT4:     d3dcf = D3DFMT_DXT4;	        break;
        case cfDXT5:     d3dcf = D3DFMT_DXT5;	        break;
        case cfUnknown:	 d3dcf = D3DFMT_UNKNOWN;	    break;
    }
    return d3dcf;
} // ConvertColorFormat

inline ColorFormat ConvertColorFormat( D3DFORMAT colFmt )
{
    ColorFormat res = cfARGB4444;
    switch (colFmt)
    {
    case D3DFMT_A4R4G4B4:       res = cfARGB4444;	break;
    case D3DFMT_X1R5G5B5:       res = cfXRGB1555;	break;
    case D3DFMT_A8R8G8B8:       res = cfARGB8888;	break;
    case D3DFMT_X8R8G8B8:       res = cfXRGB8888;	break;
    case D3DFMT_R5G6B5:         res = cfRGB565;	    break;
    case D3DFMT_V8U8:           res = cfV8U8;	    break;
    case D3DFMT_R8G8B8:	        res = cfRGB888;	    break;
    case D3DFMT_A8:		        res = cfA8;		    break;
    case D3DFMT_R16F:	        res = cfR16F;       break;     
    case D3DFMT_G16R16F:	    res = cfGR16F;      break;     
    case D3DFMT_A16B16G16R16F:  res = cfABGR16F;	break;     
    case D3DFMT_R32F:		    res = cfR32F;       break;     
    case D3DFMT_G32R32F:	    res = cfGR32F;      break;     
    case D3DFMT_A32B32G32R32F:  res = cfABGR32F;	break;
    case D3DFMT_DXT1:           res = cfDXT1;	    break;
    case D3DFMT_DXT2:           res = cfDXT2;	    break;
    case D3DFMT_DXT3:           res = cfDXT3;	    break;
    case D3DFMT_DXT4:           res = cfDXT4;	    break;
    case D3DFMT_DXT5:           res = cfDXT5;	    break;
    case D3DFMT_UNKNOWN:        res = cfUnknown;	break;
    };
    return res;
} // ConvertColorFormat

inline D3DPOOL ConvertMemoryPool( TextureMemoryPool memPool	)
{
    D3DPOOL pool = D3DPOOL_MANAGED;
    switch (memPool)
    {
    case tmpDefault:    pool = D3DPOOL_DEFAULT;	    break;
    case tmpSystem:     pool = D3DPOOL_SYSTEMMEM;	break;
    case tmpManaged:    pool = D3DPOOL_MANAGED;	    break;
    };
    return pool;
} // ConvertMemoryPool

inline TextureMemoryPool ConvertMemoryPool( D3DPOOL memPool	)
{
    TextureMemoryPool pool = tmpManaged;
    switch (memPool)
    {
        case D3DPOOL_DEFAULT:    pool = tmpDefault;  break;
        case D3DPOOL_SYSTEMMEM:	 pool = tmpSystem;   break;
        case D3DPOOL_MANAGED:    pool = tmpManaged;  break;
    };
    return pool;
} // ConvertMemoryPool

inline D3DDECLTYPE ConvertVElemType( VertCompType type )
{
    D3DDECLTYPE res = D3DDECLTYPE_UNUSED;
    switch (type)
    {
        case ctFloat1: res = D3DDECLTYPE_FLOAT1;    break;
        case ctFloat2: res = D3DDECLTYPE_FLOAT2;    break;
        case ctFloat3: res = D3DDECLTYPE_FLOAT3;    break;
        case ctFloat4: res = D3DDECLTYPE_FLOAT4;    break;
        case ctColor:  res = D3DDECLTYPE_D3DCOLOR;  break;
        case ctShort2: res = D3DDECLTYPE_SHORT2;    break;
        case ctShort4: res = D3DDECLTYPE_SHORT4;    break;
        case ctUByte4: res = D3DDECLTYPE_UBYTE4;    break;
    };
    return res;
} // ConvertVElemType

inline D3DDECLUSAGE ConvertVElemUsage( VertCompUsage usage )
{
    D3DDECLUSAGE res = D3DDECLUSAGE_TEXCOORD;
    switch (usage)
    {
        case vcPosition: 
        case vcPositionRHW: 
            res = D3DDECLUSAGE_POSITION;  break;
        case vcNormal:      res = D3DDECLUSAGE_NORMAL;    break;
        case vcDiffuse:     res = D3DDECLUSAGE_COLOR;     break;
        case vcSpecular:    res = D3DDECLUSAGE_COLOR;     break;
		case vcColor2:		res = D3DDECLUSAGE_COLOR;     break;
		case vcColor3:		res = D3DDECLUSAGE_COLOR;     break;
        case vcBinormal:    res = D3DDECLUSAGE_BINORMAL;  break;
        case vcTangent:     res = D3DDECLUSAGE_TANGENT;   break;
        case vcBlend0:
        case vcBlend1:
        case vcBlend2:
        case vcBlend3:
            res = D3DDECLUSAGE_BLENDWEIGHT; break;
        case vcBlendIdx:
        case vcBlendIdx0:
        case vcBlendIdx1:
        case vcBlendIdx2:
        case vcBlendIdx3:
            res = D3DDECLUSAGE_BLENDINDICES; break;
        case vcTexCoor0:
        case vcTexCoor1:
        case vcTexCoor2:
        case vcTexCoor3:
        case vcTexCoor4:
        case vcTexCoor5:
        case vcTexCoor6:
        case vcTexCoor7:
            res = D3DDECLUSAGE_TEXCOORD; break;
    };
    return res;
} // ConvertVElemUsage

inline int GetVElemUsageIndex( VertCompUsage usage )
{
    int res = 0;
    switch (usage)
    {
        case vcSpecular:    res = 1; break;
		case vcColor2:		res = 2; break;
		case vcColor3:		res = 3; break;
        case vcTexCoor0:    res = 0; break;
        case vcTexCoor1:    res = 1; break;
        case vcTexCoor2:    res = 2; break;
        case vcTexCoor3:    res = 3; break;
        case vcTexCoor4:    res = 4; break;
        case vcTexCoor5:    res = 5; break;
        case vcTexCoor6:    res = 6; break;
        case vcTexCoor7:    res = 7; break;
        case vcBlend0:      res = 0; break;
        case vcBlend1:      res = 1; break;
        case vcBlend2:      res = 2; break;
        case vcBlend3:      res = 3; break;
        case vcBlendIdx0:   res = 0; break;
        case vcBlendIdx1:   res = 1; break;
        case vcBlendIdx2:   res = 2; break;
        case vcBlendIdx3:   res = 3; break;		
    };
    return res;
} // GetVElemUsageIndex

inline bool MapsToD3DVElemUsage( VertCompUsage usage )
{
    return true;
} // MapsToD3DVElemUsage

inline D3DPRIMITIVETYPE ConvertPrimitiveType( PrimitiveType priType )
{
    D3DPRIMITIVETYPE d3dPri = D3DPT_POINTLIST;
    switch (priType)
    {
    case ptTriangleList:	d3dPri = D3DPT_TRIANGLELIST;	break;
    case ptTriangleStrip:	d3dPri = D3DPT_TRIANGLESTRIP;	break;
    case ptTriangleFan:     d3dPri = D3DPT_TRIANGLEFAN;		break;
    case ptLineStrip:		d3dPri = D3DPT_LINESTRIP;		break;
    case ptLineList:		d3dPri = D3DPT_LINELIST;		break;
    case ptPointList:		d3dPri = D3DPT_POINTLIST;		break;
    case ptQuadList:        d3dPri = D3DPT_TRIANGLELIST;	break;
    }
    return d3dPri;
}  // ConvertPrimitiveType

IDirect3DVertexDeclaration9* CreateVDecl( IDirect3DDevice9* pDevice, const VertexDeclaration& vdecl );
DWORD CreateFVF( const VertexDeclaration& vdecl );

#endif // __D3DADAPT_H__