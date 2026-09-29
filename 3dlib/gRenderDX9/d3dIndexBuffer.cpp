/*****************************************************************************/
/*	File:	d3dIndexBuffer.cpp
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

IndexBufferDX9::IndexBufferDX9(const char *name)
{
    m_Name = name ? name : "";
    m_bDynamic = false;
    m_SizeBytes = 0;
    m_NIndices = 0;
    m_bLocked = false;
    m_NFilledIdx = 0;
    m_IdxStride = 0;
    m_IndexSize = isWORD;

    m_pBuffer = NULL;

    m_FirstValidStamp = 1;
    m_CurrentStamp = 1;

    m_Free = false;
} // IndexBufferDX9::IndexBufferDX9


/*****************************************************************************/
/*  DeleteDeviceObjects
/*****************************************************************************/

void IndexBufferDX9::DeleteDeviceObjects()
{
    /*
        Make sure the buffer is not considered locked after the D3D object
        has been released.
    */
    m_bLocked = false;

    SAFE_RELEASE(m_pBuffer);
} // IndexBufferDX9::DeleteDeviceObjects


/*****************************************************************************/
/*  InvalidateDeviceObjects
/*****************************************************************************/

void IndexBufferDX9::InvalidateDeviceObjects()
{
    /*
        Index buffers are created in D3DPOOL_DEFAULT, therefore they must
        be released before a device reset.
    */
    m_bLocked = false;

    SAFE_RELEASE(m_pBuffer);
} // IndexBufferDX9::InvalidateDeviceObjects


/*****************************************************************************/
/*  RestoreDeviceObjects
/*****************************************************************************/

void IndexBufferDX9::RestoreDeviceObjects()
{
    if (m_pBuffer)
        return;

    if (m_SizeBytes <= 0)
        return;

    if (m_IdxStride <= 0)
        return;

    /*
        Recreate the buffer using the parameters saved by Create().
    */
    if (!Create(m_SizeBytes, m_bDynamic, m_IndexSize))
        return;

    m_FirstValidStamp = m_CurrentStamp++;
    m_NFilledIdx = 0;
    m_bLocked = false;
} // IndexBufferDX9::RestoreDeviceObjects


/*****************************************************************************/
/*  External Direct3D device
/*****************************************************************************/

IDirect3DDevice9 *GetDirect3DDevice();


/*****************************************************************************/
/*  Create
/*****************************************************************************/

bool IndexBufferDX9::Create(int nBytes, bool bDynamic, IndexSize idxSize)
{
    if (nBytes <= 0)
        return false;

    /*
        Determine index format and stride.
    */
    D3DFORMAT format = D3DFMT_UNKNOWN;
    int idxStride = 0;

    if (idxSize == isWORD)
    {
        format = D3DFMT_INDEX16;
        idxStride = 2;
    }
    else if (idxSize == isDWORD)
    {
        format = D3DFMT_INDEX32;
        idxStride = 4;
    }
    else
    {
        return false;
    }

    /*
        Index buffer size must be aligned to the index size.
    */
    if ((nBytes % idxStride) != 0)
        return false;

    IDirect3DDevice9 *pDevice = GetDirect3DDevice();

    if (!pDevice)
        return false;

    /*
        If the buffer is currently locked, do not release it silently.
        This should normally never happen.
    */
    if (m_bLocked)
    {
        m_bLocked = false;
    }

    SAFE_RELEASE(m_pBuffer);

    DWORD usage = D3DUSAGE_WRITEONLY;

    if (bDynamic)
        usage |= D3DUSAGE_DYNAMIC;

    HRESULT hr = pDevice->CreateIndexBuffer(
        nBytes,
        usage,
        format,
        D3DPOOL_DEFAULT,
        &m_pBuffer,
        NULL
    );

    DX_CHK(hr);

    if (FAILED(hr))
    {
        m_pBuffer = NULL;
        return false;
    }

    m_bDynamic = bDynamic;
    m_SizeBytes = nBytes;
    m_NIndices = nBytes / idxStride;
    m_IdxStride = idxStride;
    m_IndexSize = idxSize;

    m_bLocked = false;
    m_NFilledIdx = 0;

    m_FirstValidStamp = m_CurrentStamp++;

    m_Free = false;

    return true;
} // IndexBufferDX9::Create


/*****************************************************************************/
/*  Bind
/*****************************************************************************/

bool IndexBufferDX9::Bind()
{
    if (!m_pBuffer)
        return false;

    IDirect3DDevice9 *pDevice = GetDirect3DDevice();

    if (!pDevice)
        return false;

    HRESULT hr = pDevice->SetIndices(m_pBuffer);

    DX_CHK(hr);

    return SUCCEEDED(hr);
} // IndexBufferDX9::Bind


/*****************************************************************************/
/*  Purge
/*****************************************************************************/

void IndexBufferDX9::Purge()
{
    m_FirstValidStamp = ++m_CurrentStamp;
    m_NFilledIdx = 0;
} // IndexBufferDX9::Purge


/*****************************************************************************/
/*  Lock timing
/*****************************************************************************/

int ILockTime = 0;


/*****************************************************************************/
/*  Lock
/*****************************************************************************/

BYTE *IndexBufferDX9::Lock(
    int firstIdx,
    int numIdx,
    DWORD &stamp,
    bool bDiscard)
{
    stamp = 0;

    if (!m_pBuffer)
        return NULL;

    if (m_IdxStride <= 0)
        return NULL;

    if (firstIdx < 0 || numIdx <= 0)
        return NULL;

    if (firstIdx >= m_NIndices)
        return NULL;

    if (numIdx > m_NIndices - firstIdx)
        return NULL;

    /*
        Do not lock the same buffer twice.
    */
    if (m_bLocked)
        return NULL;

    /*
        Dynamic buffers use DISCARD/NOOVERWRITE.
        Static buffers simply use NOSYSLOCK.
    */
    DWORD flags = D3DLOCK_NOSYSLOCK;

    if (m_bDynamic)
    {
        if (bDiscard)
            flags |= D3DLOCK_DISCARD;
        else
            flags |= D3DLOCK_NOOVERWRITE;
    }

    if (bDiscard)
    {
        m_FirstValidStamp = m_CurrentStamp;
        m_NFilledIdx = 0;
    }

    void *ptr = NULL;

    __beginT();

    HRESULT hr = m_pBuffer->Lock(
        firstIdx * m_IdxStride,
        numIdx * m_IdxStride,
        &ptr,
        flags
    );

    __endT(ILockTime);

    DX_CHK(hr);

    /*
        This is critical.

        Never use ptr if IDirect3DIndexBuffer9::Lock failed.
    */
    if (FAILED(hr) || ptr == NULL)
    {
        m_bLocked = false;
        return NULL;
    }

    /*
        Assume the caller fills the entire locked region.
    */
    m_NFilledIdx = firstIdx + numIdx;

    stamp = m_CurrentStamp++;

    m_bLocked = true;

    return (BYTE *)ptr;
} // IndexBufferDX9::Lock


/*****************************************************************************/
/*  HasAppendSpace
/*****************************************************************************/

bool IndexBufferDX9::HasAppendSpace(int numIdx)
{
    if (!m_pBuffer)
        return false;

    if (numIdx <= 0)
        return false;

    if (numIdx > m_NIndices)
        return false;

    if (m_NFilledIdx < 0)
        return false;

    if (m_NFilledIdx + numIdx > m_NIndices)
        return false;

    return true;
} // IndexBufferDX9::HasAppendSpace


/*****************************************************************************/
/*  LockAppend
/*****************************************************************************/

BYTE *IndexBufferDX9::LockAppend(
    int numIdx,
    int &offset,
    DWORD &stamp)
{
    offset = 0;
    stamp = 0;

    if (!m_pBuffer)
        return NULL;

    if (numIdx <= 0 || numIdx > m_NIndices)
        return NULL;

    /*
        If there isn't enough room, discard the old contents and start
        writing from the beginning of the dynamic buffer.
    */
    bool bDiscard = false;

    if (m_NFilledIdx + numIdx > m_NIndices)
    {
        if (!m_bDynamic)
            return NULL;

        bDiscard = true;
        m_NFilledIdx = 0;
    }

    offset = m_NFilledIdx;

    BYTE *pBuf = Lock(
        m_NFilledIdx,
        numIdx,
        stamp,
        bDiscard
    );

    if (!pBuf)
    {
        offset = 0;
        stamp = 0;
        return NULL;
    }

    return pBuf;
} // IndexBufferDX9::LockAppend


/*****************************************************************************/
/*  Unlock
/*****************************************************************************/

void IndexBufferDX9::Unlock()
{
    if (!m_pBuffer)
    {
        m_bLocked = false;
        return;
    }

    if (!m_bLocked)
        return;

    __beginT();

    HRESULT hr = m_pBuffer->Unlock();

    __endT(ILockTime);

    DX_CHK(hr);

    m_bLocked = false;
} // IndexBufferDX9::Unlock


/*****************************************************************************/
/*  CreateQuadBuffer
/*****************************************************************************/

void IndexBufferDX9::CreateQuadBuffer()
{
    /*
        Intentionally left empty.

        The original engine may have used a special shared quad index
        buffer. Do not create an implementation here until its users and
        expected size are known.
    */
} // IndexBufferDX9::CreateQuadBuffer