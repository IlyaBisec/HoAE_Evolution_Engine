/*****************************************************************************/
/*	File:	d3dIndexBuffer.h
/*  Desc:	IndexBuffer interface implementation for DirectX9 
/*	Author:	Ruslan Shestopalyuk
/*	Date:	02.11.2004
/*****************************************************************************/
#include "gRenderPch.h"
#include "d3dIndexBuffer.h"
#include "kSSEUtils.h"

/*****************************************************************************/
/*  IndexBufferDX9 implementation
/*****************************************************************************/
IndexBufferDX9::IndexBufferDX9( const char* name )
{
    m_Name              = name;
    m_pBuffer           = NULL;    

    m_bDynamic          = false;   
    m_SizeBytes         = 0;  
    m_NIndices          = 0;   
    m_bLocked           = false;    
    m_NFilledIdx        = 0;
    m_IdxStride         = 0;
    m_FirstValidStamp   = 1;
    m_CurrentStamp      = 1;
} // IndexBufferDX9::IndexBufferDX9

void IndexBufferDX9::DeleteDeviceObjects()
{
    SAFE_RELEASE( m_pBuffer );
} // IndexBufferDX9::DeleteDeviceObjects

void IndexBufferDX9::InvalidateDeviceObjects()
{
    SAFE_RELEASE( m_pBuffer );
} // IndexBufferDX9::InvalidateDeviceObjects

void IndexBufferDX9::RestoreDeviceObjects()
{
    if (!m_pBuffer) Create( m_NIndices*m_IdxStride, m_bDynamic, m_IndexSize );

    m_FirstValidStamp = m_CurrentStamp++;
    m_NFilledIdx = 0;
} // IndexBufferDX9::RestoreDeviceObjects

IDirect3DDevice9* GetDirect3DDevice();
bool IndexBufferDX9::Create( int nBytes, bool bDynamic, IndexSize idxSize )
{
    DeleteDeviceObjects();
    DWORD usage = D3DUSAGE_WRITEONLY;
    if (bDynamic) usage |= D3DUSAGE_DYNAMIC;

    D3DFORMAT format = D3DFMT_UNKNOWN;
    if (idxSize == isWORD) 
    {
        format = D3DFMT_INDEX16;
        m_IdxStride = 2;
    }
    else if (idxSize == isDWORD) 
    {
        format = D3DFMT_INDEX32;
        m_IdxStride = 4;
    }
    else return false;
    

    IDirect3DDevice9* pDevice = GetDirect3DDevice();
    if (!pDevice) return false;

    HRESULT hr = pDevice->CreateIndexBuffer( nBytes, usage, format, D3DPOOL_DEFAULT, &m_pBuffer, NULL );
    DX_CHK( hr );
    m_bDynamic      = bDynamic;    
    m_SizeBytes     = nBytes;   
    m_bLocked       = false;     
    m_NFilledIdx    = 0;
    m_NIndices      = nBytes/m_IdxStride;
    m_IndexSize     = idxSize;
	m_Free			= false;

    return (hr == S_OK);
} // IndexBufferDX9::Create

bool IndexBufferDX9::Bind()
{
    assert( m_pBuffer );
    
    IDirect3DDevice9* pDevice = GetDirect3DDevice();
    if (!pDevice) return false;

    HRESULT hRes = pDevice->SetIndices( m_pBuffer );
    DX_CHK( hRes );
    return (hRes == S_OK);
} // IndexBufferDX9::Bind

void IndexBufferDX9::Purge()
{
    m_FirstValidStamp = ++m_CurrentStamp;
    m_NFilledIdx = 0;
} // VertexBufferDX9::Purge
int ILockTime=0;
BYTE* IndexBufferDX9::Lock( int firstIdx, int numIdx, DWORD& stamp, bool bDiscard )
{
    if (!m_pBuffer || firstIdx + numIdx > m_NIndices || numIdx == 0) return NULL;

//	IDirect3DDevice9 *pDevice = GetDirect3DDevice();
//	if(!pDevice || FAILED(pDevice->TestCooperativeLevel())) {
//		return NULL;
//	}

    void* ptr;
    DWORD flags = D3DLOCK_NOSYSLOCK;
    if (m_bDynamic)
    {
        if (bDiscard) flags |= D3DLOCK_DISCARD; else flags |= D3DLOCK_NOOVERWRITE;
    }
    if (bDiscard) 
    {
        m_FirstValidStamp = m_CurrentStamp;
        m_NFilledIdx = 0;
    }

    __beginT();

    DX_CHK( m_pBuffer->Lock( firstIdx*m_IdxStride, numIdx*m_IdxStride, &ptr, flags ) );

    __endT(ILockTime);

//	if(IsBadWritePtr(ptr, numIdx * m_IdxStride)) {
//		return NULL;
//	}
	//  assume client filled all the buffer area he has locked
	m_NFilledIdx = firstIdx + numIdx;
	stamp = m_CurrentStamp++;
	m_bLocked = true;
	return (BYTE*)ptr;
} // IndexBufferDX9::Lock

bool IndexBufferDX9::HasAppendSpace( int numIdx )
{
    if (numIdx > m_NIndices || numIdx == 0) return false;
    if (m_NFilledIdx + numIdx > m_NIndices) return false;
    return true;
}

BYTE* IndexBufferDX9::LockAppend( int numIdx, int& offset, DWORD& stamp )
{
    if (numIdx > m_NIndices || numIdx == 0) return NULL;
    BYTE* pBuf = NULL;
    bool bDiscard = false;
    if (m_NFilledIdx + numIdx > m_NIndices) 
    {
        bDiscard = true; 
        m_NFilledIdx = 0;
    }
    offset = m_NFilledIdx;
    pBuf = Lock( m_NFilledIdx, numIdx, stamp, bDiscard );
    if (!pBuf) return NULL;
    return pBuf;
} // IndexBufferDX9::Append

void IndexBufferDX9::Unlock()
{
    m_bLocked = false;
    __beginT();
    DX_CHK( m_pBuffer->Unlock() );
    __endT(ILockTime);
} // IndexBufferDX9::Unlock

void IndexBufferDX9::CreateQuadBuffer()
{
    
} // IndexBufferDX9::CreateQuadBuffer

