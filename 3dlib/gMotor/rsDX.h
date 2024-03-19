/*****************************************************************************/
/*	File:	rsDX.h
/*	Author:	Ruslan Shestopalyuk
/*	Date:	23.01.2003
/*****************************************************************************/
#ifndef __RSDX_H__
#define __RSDX_H__
#pragma once

IRenderSystem* GetRenderSystemDX();

#ifdef _DX9 // DirectX9
	#include "d3d9.h"
	#include "d3dx9.h"
	#include "dxerr9.h"
	
typedef IDirect3DSurface9	DXSurface;
typedef IDirect3DTexture9	DXTexture;

typedef IDirect3DIndexBuffer9	DXIndexBuffer;
typedef IDirect3DVertexBuffer9	DXVertexBuffer;

typedef IDirect3DDevice9	DXDevice;
typedef IDirect3D9			DXAPI;
typedef D3DMATERIAL9		DXMaterial;
typedef D3DLIGHT9			DXLight;
typedef ID3DXBuffer			DXBuffer;
typedef ID3DXEffect			DXEffect;
typedef	D3DCAPS8			DXCaps;

#else // DirectX8
	#include "d3d8.h"
	#include "d3dx8.h"
	#include "dxerr8.h"

typedef IDirect3DSurface8	DXSurface;
typedef IDirect3DTexture8	DXTexture;

typedef IDirect3DIndexBuffer8	DXIndexBuffer;
typedef IDirect3DVertexBuffer8	DXVertexBuffer;

typedef IDirect3DDevice8	DXDevice;
typedef IDirect3D8			DXAPI;
typedef D3DMATERIAL8		DXMaterial;
typedef D3DLIGHT8			DXLight;
typedef ID3DXBuffer			DXBuffer;
typedef ID3DXEffect			DXEffect;
typedef	D3DCAPS8			DXCaps;
	
#endif // DirectX9/DirectX8

#include "rsVertex.h"

#include "rsDXErr.h"
#include "rsDeviceStates.h"
#include "rsShaderCache.h"
#include "rsPrimitiveBufferDX.h"
#include "rsRenderSystemDX.h"

_inl DWORD			FvfDX			( VertexFormat vertFmt	);
_inl D3DFORMAT		ColorFormatG2DX	( ColorFormat colFmt	);
_inl ColorFormat	ColorFormatDX2G	( D3DFORMAT colFmt		);
_inl D3DFORMAT		DSFormatG2DX	( DepthStencilFormat dsFmt );
_inl D3DPOOL		MemoryPoolG2DX	( MemoryPool memPool	);
_inl MemoryPool		MemoryPoolDX2G	( D3DPOOL		memPool	);
_inl DWORD			TexUsageG2DX	( TextureUsage texUsage );
_inl TextureUsage	TexUsageDX2G	( DWORD texUsage );


#ifdef _INLINES
#include "rsDX.inl"
#endif // _INLINES

#endif // __RSDX_H__