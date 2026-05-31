/*****************************************************************************/
/*	File:	rsPrimitiveBufferDX.cpp
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	26.12.2002
/*****************************************************************************/
#include "stdafx.h"

#include "rsVertex.h"
#include "rsDX.h"
#include "kUtilities.h"
#include "kSSEUtils.h"

const DWORD  c_FVF[] = 
{
	0, // vfUnknown		
	D3DFVF_XYZRHW	| D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1, // vfVertexTnL			
	D3DFVF_XYZ		| D3DFVF_DIFFUSE | D3DFVF_TEX2, // vfVertex2t			
	D3DFVF_XYZ		| D3DFVF_NORMAL	 | D3DFVF_TEX1, // vfVertexN				
	D3DFVF_XYZRHW	| D3DFVF_DIFFUSE | D3DFVF_TEX2, // vfVertexTnL2			
	D3DFVF_XYZ		| D3DFVF_TEX1, // vfVertexT				
	D3DFVF_XYZB1	| D3DFVF_LASTBETA_UBYTE4 | D3DFVF_TEX2, // vfVertexMP1			
	D3DFVF_XYZB1	| D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2, // vfVertexW1			
	D3DFVF_XYZRHW	| D3DFVF_DIFFUSE |D3DFVF_SPECULAR | D3DFVF_TEX2, // vfVertexTnL2S			
	D3DFVF_XYZB2	| D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2, // vfVertexW2			
	D3DFVF_XYZB3	| D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2, // vfVertexW3			
	D3DFVF_XYZB4	| D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX2, // vfVertexW4	
	D3DFVF_XYZ		| D3DFVF_NORMAL	 | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX2, // vfVertexN2T	
	D3DFVF_XYZ		| D3DFVF_DIFFUSE, // vfVertexXYZD 
	D3DFVF_XYZRHW	,// vfVertexXYZW
	D3DFVF_XYZ		| D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1,//vfVertexTS
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
}; // c_FVF

DWORD VertexFormatFVF( VertexFormat vf )
{
	assert( vf < c_NumVertexTypes );
	return c_FVF[(int)vf];
}

D3DPRIMITIVETYPE PriTypeDX( PrimitiveType priType )
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
	}
	return d3dPri;
}  // PriTypeDX

bool OptimizeForGPUCache( Primitive& pri )
{
	DWORD fvf = VertexFormatFVF( pri.getVertexFormat() );
	ID3DXMesh* pMesh = NULL;
	IDirect3DDevice8* pD3D = D3DRenderSystem::instance().GetDevice();
	HRESULT hr = D3DXCreateMeshFVF( pri.getNPri(), pri.getNVert(), D3DXMESH_MANAGED, fvf, pD3D, &pMesh );
	if (hr != S_OK) return false;
	
	DWORD* adjIn	 = new DWORD[3 * pMesh->GetNumFaces()];
	DWORD* adjOut	 = new DWORD[3 * pMesh->GetNumFaces()];
	DWORD* faceRemap = new DWORD[pMesh->GetNumFaces()];
	
	BYTE* pIdx;
	pMesh->LockIndexBuffer( 0, &pIdx );
	memcpy( pIdx, pri.getIndices(), pri.getNInd()* sizeof(WORD) );
	pMesh->UnlockIndexBuffer();
	
	BYTE* pVert;
	pMesh->LockVertexBuffer( 0, &pVert );
	memcpy( pVert, pri.getVertexData(), pri.getNVert()*pri.getVertexStride() );
	pMesh->UnlockVertexBuffer();
	
	ID3DXBuffer* pBuffer = NULL;
	D3DXCreateBuffer( pri.getVertexStride()*pri.getNVert(), &pBuffer );
	pMesh->GenerateAdjacency( 0.00001f, adjIn );
	pMesh->OptimizeInplace( D3DXMESHOPT_VERTEXCACHE, adjIn, adjOut, faceRemap, &pBuffer );

	int nV = pMesh->GetNumVertices();
	int nF = pMesh->GetNumFaces();

	pMesh->LockIndexBuffer( 0, &pIdx );
	memcpy( pri.getIndices(), pIdx, pri.getNInd()* sizeof(WORD) );
	pMesh->UnlockIndexBuffer();

	pMesh->LockVertexBuffer( 0, &pVert );
	memcpy( pri.getVertexData(), pVert, pri.getNVert()*pri.getVertexStride() );
	pMesh->UnlockVertexBuffer();
	
	delete []adjIn;
	delete []adjOut;
	delete []faceRemap;

	pMesh->Release();
	pBuffer->Release();
	return true;
} // OptimizeForGPUCache

/*****************************************************************************/
/*	PrimitiveCache implementation
/*****************************************************************************/
void PrimitiveCache::Init()
{
	//  create index buffers
	m_QuadIB.Create		( c_QuadBufferInd		);
	m_DynamicIB.Create	( c_DynamicBufferInd	);
	m_StaticIB.Create	( c_StaticBufferInd		);

	m_StaticVB.Create	( c_StaticVBufferBytes, vfUnknown );
	m_DynamicVB.Create	( c_DynamicVBufferBytes, vfUnknown );

	for (int i = 1; i < c_NumVertexTypes; i++)
	{
		m_Quad[i].AttachVB( &m_DynamicVB );
		m_Quad[i].AttachIB( &m_QuadIB );
		m_Quad[i].SetVertexFormat( (VertexFormat)i );

		m_Static[i].AttachVB( &m_StaticVB );
		m_Static[i].AttachIB( &m_StaticIB );
		m_Static[i].SetVertexFormat( (VertexFormat)i );

		m_Dynamic[i].AttachVB( &m_DynamicVB	);
		m_Dynamic[i].AttachIB( &m_DynamicIB	);
		m_Dynamic[i].SetVertexFormat( (VertexFormat)i );
	}
} // PrimitiveCache::Init

void PrimitiveCache::Shut()
{
	m_QuadIB.Release();
	m_StaticIB.Release();
	m_DynamicIB.Release();
	m_StaticVB.Release();
	m_DynamicVB.Release();
} // PrimitiveCache::Shut

bool PrimitiveCache::RestoreDeviceObjects()
{
	Init();
	return true;
} // PrimitiveCache::InvalidateDeviceObjects

bool PrimitiveCache::InvalidateDeviceObjects()
{
	Shut();
	return true;
} // PrimitiveCache::InvalidateDeviceObjects

/*****************************************************************************/
/*	PrimitiveCache implementation
/*****************************************************************************/
bool PrimitiveCache::Draw( BaseMesh& mesh )
{
	PrimitiveBuffer* pb = NULL;
	int bufIdx = (int)mesh.getVertexFormat();
	if (bufIdx == 0) return false;
	if (mesh.isQuadList())		pb = &m_Quad	[bufIdx]; 
	else if (mesh.isStatic())	pb = &m_Dynamic	[bufIdx];//pb = &m_Static	[bufIdx]; 
	else						pb = &m_Dynamic	[bufIdx];
	
	assert( pb );
	pb->Draw( mesh );
	return true;
} // PrimitiveCache::Draw

/*****************************************************************************/
/*	PrimitiveBuffer implementation
/*****************************************************************************/
DWORD PrimitiveBuffer::s_CurFVF = 0;
PrimitiveBuffer::PrimitiveBuffer()
{
	m_FVF			= 0;
	m_VertexFormat	= vfUnknown; 
	m_pVB			= NULL;
	m_pIB			= NULL;
	m_Stride		= 0;
} // PrimitiveBuffer::PrimitiveBuffer

bool PrimitiveBuffer::Draw( BaseMesh& mesh )
{
	if (mesh.getNVert() == 0) return false;
	DXDevice* pDevice = D3DRenderSystem::instance().GetDevice();

	//  vertex buffer
	int nV = mesh.getNVert();

	DX_CHK( pDevice->SetVertexShader( m_FVF ) );
	pDevice->SetStreamSource( 0, m_pVB->m_pBuffer, m_Stride );
	m_pVB->m_CurFVF		= m_FVF;
	m_pVB->m_CurStride	= m_Stride;
	int baseVertex		= m_pVB->FillVerts( mesh.getVertexData(), nV, mesh );
	int baseIndex		= m_pIB->FillIndices( mesh.getIndices(), mesh.getNInd(), mesh );
	pDevice->SetIndices( m_pIB->m_pBuffer, baseVertex );

	int numPri = mesh.getNPri();
	if (mesh.isIndexed() || mesh.isQuadList())
	{
		DX_CHK( pDevice->DrawIndexedPrimitive( PriTypeDX( mesh.getPriType() ), 0, nV, baseIndex, numPri ) );
	}
	else
	{
		DX_CHK( pDevice->DrawPrimitive(	PriTypeDX( mesh.getPriType() ), baseVertex, numPri ) );
	}
	INC_COUNTER(Polygons,numPri);
	INC_COUNTER(Dips,1);
	return true;
} // PrimitiveBuffer::Draw

void PrimitiveBuffer::SetVertexFormat( VertexFormat vertF )
{
	m_VertexFormat	= vertF;
	m_FVF			= VertexFormatFVF( vertF );
	m_Stride		= Vertex::GetStride( vertF );
} // PrimitiveBuffer::SetVertexFormat

/*****************************************************************************/
/*	VertexBuffer implementation
/*****************************************************************************/
VertexBuffer::VertexBuffer()
{
	m_CurStride	= 0;
	m_CurVert	= 0;
	m_CurByte	= 0;
	m_MaxByte	= 0;
	m_CurFVF	= 0;	
	m_pBuffer	= NULL;
    m_bDynamic  = true;
}

void VertexBuffer::Release()
{
	SAFE_RELEASE( m_pBuffer );
	m_CurStride	= 0;
	m_CurVert	= 0;
	m_CurByte	= 0;
	m_MaxByte	= 0;
	m_CurFVF	= 0;
	m_pBuffer	= NULL;
}

/*****************************************************************************/
/*	IndexBuffer implementation
/*****************************************************************************/
IndexBuffer::IndexBuffer()
{
	m_pBuffer	= NULL;

	m_MaxIdx	= 0;
	m_MaxByte	= 0;
	m_CurIdx	= 0;
	m_CurByte	= 0;
}
void IndexBuffer::Release()
{
	SAFE_RELEASE( m_pBuffer );
	m_pBuffer	= NULL;
	m_MaxIdx	= 0;
	m_MaxByte	= 0;
	m_CurIdx	= 0;
	m_CurByte	= 0;
} // IndexBuffer::Release

/*****************************************************************************/
/*	StaticIB implementation
/*****************************************************************************/
bool StaticIB::Create( int nInd )
{
    if (nInd == 0) return false;
	DXDevice* pDevice = D3DRenderSystem::instance().GetDevice();
	int nBytes = nInd * c_IBStride;
    m_pBuffer   = NULL;
    DX_CHK( pDevice->CreateIndexBuffer(	nBytes, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pBuffer ) );
    m_MaxIdx	= nInd;
	m_MaxByte	= nBytes;
	m_CurIdx	= 0;
	m_CurByte	= 0;
	return true;
} // StaticIB::Create

int StaticIB::FillIndices( WORD* idxPtr, int nInd, BaseMesh& mesh )
{
	int nLockBytes = nInd * c_IBStride;
	assert( m_pBuffer );
	if (nLockBytes > m_MaxByte || m_CurByte + nLockBytes > m_MaxByte) 
	{ 
		assert( false ); 
		return 0; 
	}
	
	DWORD flags = D3DLOCK_NOSYSLOCK;	

	BYTE* data = 0;
	DX_CHK( m_pBuffer->Lock( m_CurByte, nLockBytes, &data, flags ) );
	if (data) memcpy( data, idxPtr, nLockBytes );
	DX_CHK( m_pBuffer->Unlock() );
	
	int startIdx = m_CurIdx;
	m_CurIdx  += nInd;
	m_CurByte += nLockBytes;
	return startIdx;
} // StaticIB::FillIndices

/*****************************************************************************/
/*	DynamicIB implementation
/*****************************************************************************/
bool DynamicIB::Create( int nInd )
{	
	DXDevice* pDevice = D3DRenderSystem::instance().GetDevice();
	int nBytes = nInd*c_IBStride;
	DX_CHK( pDevice->CreateIndexBuffer(	nBytes, 
										D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 
										D3DFMT_INDEX16, 
										D3DPOOL_DEFAULT, 
										&m_pBuffer
										) );
	m_MaxIdx	= nInd;
	m_MaxByte	= nBytes;
	m_CurIdx	= 0;
	m_CurByte	= 0;
	return true;
} // DynamicIB::Create

int DynamicIB::FillIndices( WORD* idxPtr, int nInd, BaseMesh& mesh )
{
	if (nInd == 0) return 0;
	int nLockBytes = nInd * c_IBStride;
	
	assert( m_pBuffer );
	assert( nLockBytes < m_MaxByte ); 
	
	DWORD flags = D3DLOCK_NOSYSLOCK | D3DLOCK_NOOVERWRITE;	
	if (m_CurByte + nLockBytes > m_MaxByte)
	{
		m_CurByte = 0;
		m_CurIdx = 0;
		flags = D3DLOCK_NOSYSLOCK | D3DLOCK_DISCARD;
	}

	BYTE* data = 0;
	DWORD lockRes = m_pBuffer->Lock( m_CurByte, nLockBytes, &data, flags );
	if (lockRes != S_OK)
	{
		Log.Error( "Failed to lock index buffer. Pos:%d, Size:%d.", m_CurByte, nLockBytes );
		return 0;	
	}
	if (data) memcpy( data, idxPtr, nLockBytes );
	DX_CHK( m_pBuffer->Unlock() );
	
	int startIdx = m_CurIdx; 
	m_CurIdx  += nInd;
	m_CurByte += nLockBytes;
	return startIdx;
} // DynamicIB::FillIndices

/*****************************************************************************/
/*	QuadIB implementation
/*****************************************************************************/
bool QuadIB::Create( int nInd )
{
	int nQuads = nInd / 6;
	StaticIB::Create( nInd );

	assert( m_pBuffer );
	BYTE* data = 0;
	DX_CHK( m_pBuffer->Lock( 0, nInd * c_IBStride, &data, D3DLOCK_NOSYSLOCK ) );
	WORD* idxData = (WORD*) data;

	if (!idxData)
	{
		Log.Error( "Could not lock quad index buffer. NumBytes: %d", nInd*c_IBStride );
		return false;
	}
	//  fill with quad indices
	int cV = 0;
	int cI = 0;
	for (int i = 0; i < nQuads; i++)
	{
		idxData[cI++] = cV;
		idxData[cI++] = cV + 1;
		idxData[cI++] = cV + 2;
		idxData[cI++] = cV + 2;
		idxData[cI++] = cV + 1;
		idxData[cI++] = cV + 3;
		cV += 4;
	}
	
	m_MaxIdx = nInd;
	m_CurIdx = nInd;
	
	DX_CHK( m_pBuffer->Unlock() );
	return true;
} // QuadIB::Create

/*****************************************************************************/
/*	SharedIB implementation
/*****************************************************************************/
int SharedIB::FillIndices( WORD* idxPtr, int nInd, BaseMesh& mesh )
{
	//  nothing is done here, because index data is already there
	return 0;
}

/*****************************************************************************/
/*	StaticVB implementation
/*****************************************************************************/
void StaticVB::Create( int nBytes, VertexFormat vf )
{
	DXDevice* pDevice = D3DRenderSystem::instance().GetDevice();
	m_pBuffer	= NULL;
	DX_CHK( pDevice->CreateVertexBuffer(	nBytes, 
											D3DUSAGE_WRITEONLY,
											0, 
											D3DPOOL_DEFAULT, 
											&m_pBuffer ) );

	m_CurStride	= 0;
	m_CurVert	= 0;
	m_CurByte	= 0;
	m_MaxByte	= nBytes;
	m_CurFVF	= 0;
    m_bDynamic  = false;
} // StaticVB::Create

int StaticVB::FillVerts( void* vert, int nVert, BaseMesh& mesh )
{
	if (mesh.getVBPos() != -1) return 0;
	int nLockBytes = nVert * m_CurStride;
	assert( m_pBuffer );
	if (nLockBytes > m_MaxByte || m_CurByte + nLockBytes > m_MaxByte) 
	{ 
		assert( false ); 
		return 0; 
	}
	
	DWORD flags = D3DLOCK_NOSYSLOCK;	

	int remainder = m_CurByte % m_CurStride;
	if (remainder > 0) m_CurByte += m_CurStride - remainder;
	m_CurVert = m_CurByte / m_CurStride;

	BYTE* data = 0;
	DX_CHK( m_pBuffer->Lock( m_CurByte, nLockBytes, &data, flags ) );
	if (data) memcpy( data, vert, nLockBytes );

	DX_CHK( m_pBuffer->Unlock() );
	
	mesh.setVBPos( m_CurVert );
	m_CurByte += nLockBytes;
	
	int startVert = m_CurVert;
	m_CurVert += nVert;
	return startVert;
} // StaticVB::FillVerts

/*****************************************************************************/
/*	DynamicVB implementation
/*****************************************************************************/
void DynamicVB::Create( int nBytes, VertexFormat vf )
{
	DXDevice* pDevice = D3DRenderSystem::instance().GetDevice();
	m_pBuffer = NULL;
	DX_CHK( pDevice->CreateVertexBuffer(	nBytes, 
											D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
											0, 
											D3DPOOL_DEFAULT, 
											&m_pBuffer ) );
	m_CurStride	= 0;
	m_CurVert	= 0;
	m_CurByte	= 0;
	m_MaxByte	= nBytes;
	m_CurFVF	= 0;	
    m_bDynamic  = true;
} // DynamicVB::Create

int DynamicVB::FillVerts( void* vert, int nVert, BaseMesh& mesh )
{
	int nLockBytes = nVert * m_CurStride;

	assert( m_pBuffer );
	assert( nLockBytes < m_MaxByte ); 

	DWORD flags = D3DLOCK_NOSYSLOCK | D3DLOCK_NOOVERWRITE;	
	if (m_CurByte + nLockBytes > m_MaxByte) 
	{
		m_CurByte = 0;
		m_CurVert = 0;
		flags = D3DLOCK_NOSYSLOCK | D3DLOCK_DISCARD;
	}
	else
	//  align m_CurByte to the new stride
	{
		int rem = m_CurByte % m_CurStride;
		if (rem > 0) m_CurByte += m_CurStride - rem;
		m_CurVert = m_CurByte / m_CurStride;
	}

	BYTE* data = 0;
	DWORD lockRes = m_pBuffer->Lock( m_CurByte, nLockBytes, &data, flags );
	if (lockRes != S_OK)
	{
		DX_CHK( lockRes );
		Log.Error( "Failed to lock dynamic vertex buffer. Pos:%d, Size:%d.", m_CurByte, nLockBytes );
		return 0;	
	}
    assert( ((DWORD)vert)%16 == 0 );

#ifndef _MANOWAR_
	if (data) memcpy( data, vert, nLockBytes );
#else // _MANOWAR_
	if(data)SSE_memcpy(data,vert,nLockBytes);
#endif // _MANOWAR_

	DX_CHK( m_pBuffer->Unlock() );
	
	m_CurByte += nLockBytes;

	int startVert = m_CurVert;
	m_CurVert += nVert;
	return startVert;
} // DynamicVB::FillData

