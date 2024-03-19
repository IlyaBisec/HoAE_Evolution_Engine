/*****************************************************************************/
/*	File:	d3dVertexBuffer.h
/*  Desc:	VertexBuffer interface implementation for DirectX9 
/*	Author:	Ruslan Shestopalyuk
/*	Date:	02.11.2004
/*****************************************************************************/
#include "gRenderPch.h"
#include "d3dVertexBuffer.h"
#include "d3dAdapt.h"
#include "kSSEUtils.h"

/*****************************************************************************/
/*  VertexBufferDX9 implementation
/*****************************************************************************/
VertexBufferDX9::VertexBufferDX9( const char* name, IDirect3DDevice9* pDevice )
{
    m_Name              = name;
    m_bDynamic          = false;       
    m_pBuffer           = NULL;        
    m_SizeBytes         = 0;           
    m_NVert             = 0;           
    m_bLocked           = false;       
    m_NFilledVert       = 0;           
    m_FVF               = 0;           
    m_VStride           = 0;      
    m_VType             = -1;
    m_FirstValidStamp   = 1;
    m_CurrentStamp      = 1;
    m_bManaged          = true;
} // VertexBufferDX9::VertexBufferDX9
int SetStreamSourceTime=0;
IDirect3DDevice9* GetDirect3DDevice();
bool VertexBufferDX9::Bind( int stream, int frequency )
{
    IDirect3DDevice9* pDevice = GetDirect3DDevice();
    if (!pDevice) return false;

    __beginT();

    DX_CHK( pDevice->SetStreamSource( stream, m_pBuffer, 0, m_VStride ) );
    if (frequency > 1)
    {
        DX_CHK( pDevice->SetStreamSourceFreq( stream, frequency | D3DSTREAMSOURCE_INDEXEDDATA ) );
    }

    __endT(SetStreamSourceTime);

    return false;
} // VertexBufferDX9::Bind

bool VertexBufferDX9::Create( int nBytes, bool bDynamic, const VertexDeclaration* pVDecl )
{
    IDirect3DDevice9* pDevice = GetDirect3DDevice();
    if (!pDevice) return false;

    DeleteDeviceObjects();
    if (pVDecl)
    {
        SetVertexDecl( *pVDecl );   
        m_NVert = nBytes / m_VStride;
        m_VDecl = *pVDecl;
    }
    else
    {
        m_NVert     = 0;
        m_VStride   = 0;
        m_FVF       = 0;
    }
    
    DWORD usage = D3DUSAGE_WRITEONLY;
    if (bDynamic) 
    {
        usage |= D3DUSAGE_DYNAMIC;
        m_bManaged = false;
    }

    D3DPOOL pool = D3DPOOL_DEFAULT;
    if (m_bManaged) pool = D3DPOOL_MANAGED;

    HRESULT hr = pDevice->CreateVertexBuffer( nBytes, usage, m_FVF, pool, &m_pBuffer, NULL );
    DX_CHK( hr );
    m_bDynamic      = bDynamic;    
    m_SizeBytes     = nBytes;   
    m_bLocked       = false;     
    m_NFilledVert   = 0;
	m_Free			= false;

    return (hr == S_OK);
} // VertexBufferDX9::Create
int VLockTime=0;
BYTE* VertexBufferDX9::Lock( int firstV, int numV, DWORD& stamp, bool bDiscard )
{
    if (!m_pBuffer || firstV + numV > m_NVert || numV == 0) return NULL;

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
        m_NFilledVert = 0;
    }

    __beginT();

    DX_CHK( m_pBuffer->Lock( firstV*m_VStride, numV*m_VStride, &ptr, flags ) );

    __endT(VLockTime);

//	if(IsBadWritePtr(ptr, numV * m_VStride)) {
//		return NULL;
//	}
	
	//  assume client filled all the buffer area he has locked
	m_NFilledVert = firstV + numV;
	stamp = m_CurrentStamp++;
	m_bLocked = true;
	return (BYTE*)ptr;
}// VertexBufferDX9::Lock

void VertexBufferDX9::Purge()
{
    m_FirstValidStamp = ++m_CurrentStamp;
    m_NFilledVert = 0;
} // VertexBufferDX9::Purge

void VertexBufferDX9::Unlock()
{
    m_bLocked = false;

    __beginT();
    DX_CHK( m_pBuffer->Unlock() );
    __endT(VLockTime);

} // VertexBufferDX9::Unlock

bool VertexBufferDX9::HasAppendSpace( int numV ) 
{
    if (numV > m_NVert || numV == 0) return false;
    if (m_NFilledVert + numV > m_NVert) return false;
    return true;
}

BYTE* VertexBufferDX9::LockAppend( int numV, int& offset, DWORD& stamp )
{
    if (numV > m_NVert || numV == 0) return NULL;
    BYTE* pBuf = NULL;
    bool bDiscard = false;
    if (m_NFilledVert + numV > m_NVert) 
    {
        bDiscard = true;
        m_NFilledVert = 0;
    }
    offset = m_NFilledVert;
    pBuf = Lock( m_NFilledVert, numV, stamp, bDiscard );
    return pBuf;
} // VertexBufferDX9::LockAppend

void VertexBufferDX9::SetVertexSize( int size ) 
{ 
    if (m_VStride != size)
    {
        int nBytes = m_VStride*m_NFilledVert;
        int rem = nBytes%size;
        if (rem > 0) nBytes += size - rem;
        m_NFilledVert   = nBytes/size;
        m_NVert         = m_SizeBytes/size;
        m_VStride       = size; 
    }
} // VertexBufferDX9::SetVertexSize

void VertexBufferDX9::DeleteDeviceObjects()
{
    SAFE_RELEASE( m_pBuffer );
} // VertexBufferDX9::DeleteDeviceObjects

void VertexBufferDX9::InvalidateDeviceObjects()
{
    int nRef = GetRefCount( m_pBuffer );
    if (!m_bManaged) SAFE_RELEASE( m_pBuffer );
} // VertexBufferDX9::InvalidateDeviceObjects

void VertexBufferDX9::RestoreDeviceObjects()
{
    if (!m_pBuffer) Create( m_SizeBytes, m_bDynamic, 
                               (m_VDecl.m_NElements == 0) ? NULL : &m_VDecl );
} // VertexBufferDX9::RestoreDeviceObjects

void VertexBufferDX9::SetVertexDecl( const VertexDeclaration& vdecl )
{
    m_VDecl     = vdecl;
    m_VStride   = m_VDecl.m_VertexSize;
    m_FVF       = CreateFVF( m_VDecl );
} // VertexBufferDX9::SetVDecl

