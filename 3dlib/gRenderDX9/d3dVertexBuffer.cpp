/*****************************************************************************/
/*	File:	d3dVertexBuffer.cpp
/*  Desc:	VertexBuffer interface implementation for DirectX9
/*	Author:	Ruslan Shestopalyuk
/*	Date:	02.11.2004
/*****************************************************************************/

#include "gRenderPch.h"
#include "d3dVertexBuffer.h"
#include "d3dAdapt.h"
#include "kSSEUtils.h"


/*****************************************************************************/
/*  External functions
/*****************************************************************************/

int SetStreamSourceTime = 0;
int VLockTime = 0;

IDirect3DDevice9 *GetDirect3DDevice();


/*****************************************************************************/
/*  VertexBufferDX9::VertexBufferDX9
/*****************************************************************************/

VertexBufferDX9::VertexBufferDX9(const char *name, IDirect3DDevice9 *pDevice)
{
    m_Name = name ? name : "";
    m_bDynamic = false;
    m_pDevice = pDevice;
    m_pBuffer = NULL;

    m_SizeBytes = 0;
    m_NVert = 0;
    m_NFilledVert = 0;

    m_bLocked = false;

    m_FVF = 0;
    m_VStride = 0;
    m_VType = -1;

    m_FirstValidStamp = 1;
    m_CurrentStamp = 1;

    m_bManaged = true;
    m_Free = false;
}


/*****************************************************************************/
/*  VertexBufferDX9::Bind
/*****************************************************************************/

bool VertexBufferDX9::Bind(int stream, int frequency)
{
    IDirect3DDevice9 *pDevice = GetDirect3DDevice();

    if (!pDevice)
        return false;

    if (!m_pBuffer)
        return false;

    if (stream < 0)
        return false;

    if (frequency <= 0)
        return false;

    if (m_VStride <= 0)
        return false;

    __beginT();

    HRESULT hr = pDevice->SetStreamSource(
        stream,
        m_pBuffer,
        0,
        m_VStride
    );

    DX_CHK(hr);

    if (FAILED(hr))
    {
        __endT(SetStreamSourceTime);
        return false;
    }

    /*
        D3D9 stream frequency state is persistent.

        If a previous draw used instancing and we don't explicitly
        reset the frequency, the next ordinary draw may still use
        the old frequency state.
    */

    if (frequency > 1)
    {
        hr = pDevice->SetStreamSourceFreq(
            stream,
            ((DWORD)frequency) | D3DSTREAMSOURCE_INDEXEDDATA
        );
    }
    else
    {
        hr = pDevice->SetStreamSourceFreq(
            stream,
            1
        );
    }

    DX_CHK(hr);

    __endT(SetStreamSourceTime);

    return SUCCEEDED(hr);
}


/*****************************************************************************/
/*  VertexBufferDX9::Create
/*****************************************************************************/

bool VertexBufferDX9::Create(
    int nBytes,
    bool bDynamic,
    const VertexDeclaration *pVDecl)
{
    IDirect3DDevice9 *pDevice = GetDirect3DDevice();

    if (!pDevice)
        return false;

    if (nBytes <= 0)
        return false;

    /*
        Remove old D3D resource first.
    */

    DeleteDeviceObjects();

    m_bLocked = false;
    m_NFilledVert = 0;
    m_SizeBytes = nBytes;

    /*
        Set vertex declaration / stride.
    */

    if (pVDecl)
    {
        SetVertexDecl(*pVDecl);

        if (m_VStride <= 0)
        {
            m_SizeBytes = 0;
            m_NVert = 0;
            return false;
        }

        m_NVert = nBytes / m_VStride;

        if (m_NVert <= 0)
        {
            m_SizeBytes = 0;
            m_NVert = 0;
            return false;
        }
    }
    else
    {
        /*
            No vertex declaration.

            Keep this buffer as a raw vertex buffer. The stride may
            subsequently be supplied through SetVertexSize().
        */

        m_NVert = 0;
        m_VStride = 0;
        m_FVF = 0;

        /*
            Do not leave an old declaration active.

            We know these fields exist from VertexDeclaration because
            they are used throughout the rendering code.
        */

        m_VDecl.m_NElements = 0;
        m_VDecl.m_VertexSize = 0;
    }

    /*
        Dynamic vertex buffers must live in DEFAULT pool in D3D9.

        Managed + D3DUSAGE_DYNAMIC is not a valid combination.
    */

    m_bDynamic = bDynamic;
    m_bManaged = !bDynamic;

    DWORD usage = D3DUSAGE_WRITEONLY;

    if (bDynamic)
        usage |= D3DUSAGE_DYNAMIC;

    D3DPOOL pool = bDynamic ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED;

    HRESULT hr = pDevice->CreateVertexBuffer(
        nBytes,
        usage,
        m_FVF,
        pool,
        &m_pBuffer,
        NULL
    );

    DX_CHK(hr);

    if (FAILED(hr))
    {
        m_pBuffer = NULL;
        m_bLocked = false;
        m_NFilledVert = 0;
        m_Free = false;

        return false;
    }

    m_bLocked = false;
    m_NFilledVert = 0;
    m_Free = false;

    return true;
}


/*****************************************************************************/
/*  VertexBufferDX9::Lock
/*****************************************************************************/

BYTE *VertexBufferDX9::Lock(
    int firstV,
    int numV,
    DWORD &stamp,
    bool bDiscard)
{
    stamp = 0;

    if (!m_pBuffer)
        return NULL;

    if (m_bLocked)
        return NULL;

    if (m_VStride <= 0)
        return NULL;

    if (firstV < 0)
        return NULL;

    if (numV <= 0)
        return NULL;

    /*
        Avoid integer overflow in:

            firstV + numV > m_NVert
    */

    if (firstV > m_NVert)
        return NULL;

    if (numV > m_NVert - firstV)
        return NULL;

    DWORD flags = D3DLOCK_NOSYSLOCK;

    /*
        DISCARD / NOOVERWRITE are only valid/useful for dynamic
        buffers.
    */

    if (m_bDynamic)
    {
        if (bDiscard)
            flags |= D3DLOCK_DISCARD;
        else
            flags |= D3DLOCK_NOOVERWRITE;
    }
    else
    {
        /*
            DISCARD has no meaning for a static/managed buffer.
        */

        bDiscard = false;
    }

    void *ptr = NULL;

    __beginT();

    HRESULT hr = m_pBuffer->Lock(
        firstV * m_VStride,
        numV * m_VStride,
        &ptr,
        flags
    );

    DX_CHK(hr);

    __endT(VLockTime);

    if (FAILED(hr) || !ptr)
        return NULL;

    /*
        Only update the internal state after Lock succeeded.
    */

    if (bDiscard)
    {
        m_FirstValidStamp = m_CurrentStamp;
        m_NFilledVert = 0;
    }

    int endV = firstV + numV;

    if (endV > m_NFilledVert)
        m_NFilledVert = endV;

    stamp = (DWORD)m_CurrentStamp++;

    m_bLocked = true;

    return (BYTE *)ptr;
}


/*****************************************************************************/
/*  VertexBufferDX9::Unlock
/*****************************************************************************/

void VertexBufferDX9::Unlock()
{
    if (!m_pBuffer)
        return;

    if (!m_bLocked)
        return;

    __beginT();

    HRESULT hr = m_pBuffer->Unlock();

    DX_CHK(hr);

    __endT(VLockTime);

    /*
        Keep the locked state if Unlock failed.
        This prevents the object from pretending that the resource
        is unlocked when D3D9 reported an error.
    */

    if (SUCCEEDED(hr))
        m_bLocked = false;
}


/*****************************************************************************/
/*  VertexBufferDX9::HasAppendSpace
/*****************************************************************************/

bool VertexBufferDX9::HasAppendSpace(int numV)
{
    if (!m_pBuffer)
        return false;

    if (m_VStride <= 0)
        return false;

    if (numV <= 0)
        return false;

    if (numV > m_NVert)
        return false;

    if (m_NFilledVert > m_NVert)
        return false;

    if (numV > m_NVert - m_NFilledVert)
        return false;

    return true;
}


/*****************************************************************************/
/*  VertexBufferDX9::LockAppend
/*****************************************************************************/

BYTE *VertexBufferDX9::LockAppend(
    int numV,
    int &offset,
    DWORD &stamp)
{
    offset = 0;
    stamp = 0;

    if (!m_pBuffer)
        return NULL;

    if (m_bLocked)
        return NULL;

    if (m_VStride <= 0)
        return NULL;

    if (numV <= 0)
        return NULL;

    if (numV > m_NVert)
        return NULL;

    /*
        If there isn't enough room at the end of the buffer,
        discard the old contents and start again from vertex 0.
    */

    if (numV > m_NVert - m_NFilledVert)
    {
        offset = 0;

        return Lock(
            0,
            numV,
            stamp,
            true
        );
    }

    /*
        Append after the currently filled vertices.
    */

    offset = m_NFilledVert;

    return Lock(
        m_NFilledVert,
        numV,
        stamp,
        false
    );
}


/*****************************************************************************/
/*  VertexBufferDX9::Purge
/*****************************************************************************/

void VertexBufferDX9::Purge()
{
    /*
        Purging a locked D3D resource is unsafe.
    */

    if (m_bLocked)
        return;

    m_FirstValidStamp = ++m_CurrentStamp;
    m_NFilledVert = 0;
}


/*****************************************************************************/
/*  VertexBufferDX9::SetVertexSize
/*****************************************************************************/

void VertexBufferDX9::SetVertexSize(int size)
{
    if (size <= 0)
        return;

    if (m_bLocked)
        return;

    if (m_VStride == size)
    {
        if (m_SizeBytes > 0)
            m_NVert = m_SizeBytes / size;

        return;
    }

    /*
        Preserve the amount of already occupied data as bytes.

        This is important when the stride changes after the buffer
        has already been created.
    */

    int filledBytes = 0;

    if (m_VStride > 0 && m_NFilledVert > 0)
        filledBytes = m_VStride * m_NFilledVert;

    m_VStride = size;

    if (m_SizeBytes > 0)
        m_NVert = m_SizeBytes / m_VStride;
    else
        m_NVert = 0;

    if (filledBytes > 0)
    {
        int newFilledVert =
            (filledBytes + m_VStride - 1) / m_VStride;

        if (newFilledVert > m_NVert)
            newFilledVert = m_NVert;

        m_NFilledVert = newFilledVert;
    }
    else
    {
        m_NFilledVert = 0;
    }
}


/*****************************************************************************/
/*  VertexBufferDX9::DeleteDeviceObjects
/*****************************************************************************/

void VertexBufferDX9::DeleteDeviceObjects()
{
    /*
        A resource should normally not be deleted while locked.

        Try to leave the object in a consistent state even if this
        function is called during shutdown.
    */

    if (m_pBuffer && m_bLocked)
    {
        HRESULT hr = m_pBuffer->Unlock();

        DX_CHK(hr);

        m_bLocked = false;
    }

    SAFE_RELEASE(m_pBuffer);

    m_bLocked = false;
}


/*****************************************************************************/
/*  VertexBufferDX9::InvalidateDeviceObjects
/*****************************************************************************/

void VertexBufferDX9::InvalidateDeviceObjects()
{
    if (m_bLocked && m_pBuffer)
    {
        HRESULT hr = m_pBuffer->Unlock();

        DX_CHK(hr);

        m_bLocked = false;
    }

    /*
        Managed resources survive device loss.

        Default-pool resources, including dynamic vertex buffers,
        must be released and recreated after Reset().
    */

    if (!m_bManaged)
    {
        SAFE_RELEASE(m_pBuffer);

        /*
            Contents of a DEFAULT-pool vertex buffer are lost.
        */

        m_NFilledVert = 0;
    }
}


/*****************************************************************************/
/*  VertexBufferDX9::RestoreDeviceObjects
/*****************************************************************************/

void VertexBufferDX9::RestoreDeviceObjects()
{
    if (m_pBuffer)
        return;

    if (m_SizeBytes <= 0)
        return;

    /*
        Save information which Create(NULL) would otherwise clear.

        This is especially important for buffers which don't have a
        VertexDeclaration and receive their stride through
        SetVertexSize().
    */

    int savedStride = m_VStride;
    DWORD savedFVF = m_FVF;

    bool hasDeclaration =
        (m_VDecl.m_NElements > 0);

    VertexDeclaration *pDecl = NULL;

    if (hasDeclaration)
        pDecl = &m_VDecl;

    bool result = Create(
        m_SizeBytes,
        m_bDynamic,
        pDecl
    );

    if (!result)
    {
        /*
            Create() keeps m_SizeBytes so a later restore attempt
            can still be made.
        */

        return;
    }

    /*
        Restore manually specified stride/FVF for raw buffers.
    */

    if (!hasDeclaration)
    {
        m_VStride = savedStride;
        m_FVF = savedFVF;

        if (m_VStride > 0)
            m_NVert = m_SizeBytes / m_VStride;
        else
            m_NVert = 0;
    }

    /*
        DEFAULT-pool contents are lost after device reset.
    */

    m_NFilledVert = 0;
    m_bLocked = false;
}


/*****************************************************************************/
/*  VertexBufferDX9::SetVertexDecl
/*****************************************************************************/

void VertexBufferDX9::SetVertexDecl(
    const VertexDeclaration &vdecl)
{
    m_VDecl = vdecl;

    m_VStride = m_VDecl.m_VertexSize;
    m_FVF = CreateFVF(m_VDecl);
}