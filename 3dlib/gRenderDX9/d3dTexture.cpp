/*****************************************************************************/
/*	File:	d3dTexture.cpp
/*  Desc:	Texture interface implementation for DirectX9
/*	Author:	Ruslan Shestopalyuk
/*	Date:	02.11.2004
/*****************************************************************************/

#include "gRenderPch.h"
#include "d3dTexture.h"
#include "d3dAdapt.h"
#include "direct.h"

extern int OtherTime;

/*****************************************************************************/
/*  TextureDX9 implementation
/*****************************************************************************/

TextureDX9::TextureDX9(IDirect3DDevice9 *pDevice)
{
    m_pDevice = pDevice;

    m_pTexture = NULL;
    m_pCubeTexture = NULL;
    m_pVolTexture = NULL;
    m_pZBuffer = NULL;
    m_pBaseTexture = NULL;

    m_ID = -1;
    m_Name = "";
    m_SearchPath = "";
    m_Type = tt2D;
    m_NMipMaps = 0;
    m_MemoryPool = tmpManaged;
    m_DSFormat = dsfNone;
    m_ColorFormat = cfUnknown;

    m_Width = 0;
    m_Height = 0;

    m_bRenderTarget = false;
    m_bDynamic = false;
    m_bNoTexture = false;
}

TextureDX9::TextureDX9()
{
    m_pDevice = NULL;

    m_pTexture = NULL;
    m_pCubeTexture = NULL;
    m_pVolTexture = NULL;
    m_pZBuffer = NULL;
    m_pBaseTexture = NULL;

    m_ID = -1;
    m_Name = "";
    m_SearchPath = "";
    m_Type = tt2D;
    m_NMipMaps = 0;
    m_MemoryPool = tmpManaged;
    m_DSFormat = dsfNone;
    m_ColorFormat = cfUnknown;

    m_Width = 0;
    m_Height = 0;

    m_bRenderTarget = false;
    m_bDynamic = false;
    m_bNoTexture = false;
}

TextureDX9::~TextureDX9()
{
    DeleteDeviceObjects();
}


/*****************************************************************************/
/* Device objects
/*****************************************************************************/

void TextureDX9::InvalidateDeviceObjects()
{
    if (m_MemoryPool != tmpManaged)
    {
        SAFE_RELEASE(m_pTexture);
        SAFE_RELEASE(m_pCubeTexture);
        SAFE_RELEASE(m_pVolTexture);
        SAFE_RELEASE(m_pZBuffer);

        m_pTexture = NULL;
        m_pCubeTexture = NULL;
        m_pVolTexture = NULL;
        m_pZBuffer = NULL;

        m_pBaseTexture = NULL;
    }
}

void TextureDX9::RestoreDeviceObjects()
{
    if (m_MemoryPool != tmpManaged)
    {
        Create();
    }
}

void TextureDX9::DeleteDeviceObjects()
{
    SAFE_RELEASE(m_pTexture);
    SAFE_RELEASE(m_pCubeTexture);
    SAFE_RELEASE(m_pVolTexture);
    SAFE_RELEASE(m_pZBuffer);

    m_pTexture = NULL;
    m_pCubeTexture = NULL;
    m_pVolTexture = NULL;
    m_pZBuffer = NULL;

    m_pBaseTexture = NULL;
}


/*****************************************************************************/
/* GetSurface
/*****************************************************************************/

IDirect3DSurface9 *TextureDX9::GetSurface(int idx)
{
    if (m_pZBuffer)
    {
        m_pZBuffer->AddRef();
        return m_pZBuffer;
    }

    IDirect3DSurface9 *pSurf = NULL;

    if (m_pTexture)
    {
        HRESULT hr = m_pTexture->GetSurfaceLevel(idx, &pSurf);

        if (FAILED(hr))
        {
            return NULL;
        }
    }
    else if (m_pCubeTexture)
    {
        HRESULT hr = m_pCubeTexture->GetCubeMapSurface(
            (D3DCUBEMAP_FACES)(idx % 6),
            idx / 6,
            &pSurf
        );

        if (FAILED(hr))
        {
            return NULL;
        }
    }

    return pSurf;
}


/*****************************************************************************/
/* Bind
/*****************************************************************************/

int SetTextureTime = 0;

void TextureDX9::Bind(int stage)
{
    if (!m_pBaseTexture)
    {
        if (!Load())
        {
            return;
        }
    }

    if (!m_pDevice)
    {
        return;
    }

    __beginT();

    DX_CHK(
        m_pDevice->SetTexture(
            stage,
            m_pBaseTexture
        )
    );

    __endT(SetTextureTime);
}


/*****************************************************************************/
/* SaveToFile
/*****************************************************************************/

bool TextureDX9::SaveToFile(const char *fName) const
{
    if (!fName || !fName[0])
        return false;

    if (!m_pBaseTexture)
        return false;

    void ParseExtension(const char *fname, char *ext);

    char ext[64];
    ext[0] = 0;

    ParseExtension(fName, ext);

    D3DXIMAGE_FILEFORMAT type = D3DXIFF_DDS;

    if (!strcmp(ext, "bmp"))
        type = D3DXIFF_BMP;
    else if (!strcmp(ext, "jpg") || !strcmp(ext, "jpeg"))
        type = D3DXIFF_JPG;
    else if (!strcmp(ext, "tga"))
        type = D3DXIFF_TGA;
    else if (!strcmp(ext, "png"))
        type = D3DXIFF_PNG;
    else if (!strcmp(ext, "dds"))
        type = D3DXIFF_DDS;

    HRESULT hRes = D3DXSaveTextureToFile(
        fName,
        type,
        m_pBaseTexture,
        NULL
    );

    return SUCCEEDED(hRes);
}


/*****************************************************************************/
/* LockBits
/*****************************************************************************/

BYTE *TextureDX9::LockBits(int &pitch, int level)
{
    pitch = 0;

    if (!m_pBaseTexture)
    {
        if (!Load())
        {
            Log.Error(
                "LockBits: could not load texture <%s>",
                m_Name.c_str()
            );

            return NULL;
        }
    }

    if (!m_pTexture)
    {
        Log.Error(
            "LockBits: m_pTexture == NULL, texture=%s",
            m_Name.c_str()
        );

        return NULL;
    }

    if (level < 0 || level >= m_pTexture->GetLevelCount())
    {
        Log.Error(
            "LockBits: invalid mip level %d, texture=%s",
            level,
            m_Name.c_str()
        );

        return NULL;
    }

    D3DLOCKED_RECT rct;
    ZeroMemory(&rct, sizeof(rct));

    HRESULT hr = m_pTexture->LockRect(
        level,
        &rct,
        NULL,
        0
    );

    if (FAILED(hr))
    {
        Log.Error(
            "LockBits FAILED: texture=%s id=%d size=%dx%d level=%d HRESULT=0x%08X",
            m_Name.c_str(),
            m_ID,
            m_Width,
            m_Height,
            level,
            (unsigned)hr
        );

        return NULL;
    }

    if (!rct.pBits)
    {
        Log.Error(
            "LockBits FAILED: pBits == NULL, texture=%s",
            m_Name.c_str()
        );

        m_pTexture->UnlockRect(level);
        return NULL;
    }

    pitch = rct.Pitch;

    return (BYTE *)rct.pBits;
}


/*****************************************************************************/
/* LockBits with rectangle
/*****************************************************************************/

BYTE *TextureDX9::LockBits(
    int &pitch,
    const Rct &rect,
    int level
) const
{
    pitch = 0;

    if (!m_pTexture)
        return NULL;

    if (level < 0 || level >= m_pTexture->GetLevelCount())
        return NULL;

    RECT rc;

    rc.left = rect.x;
    rc.top = rect.y;
    rc.right = rect.GetRight();
    rc.bottom = rect.GetBottom();

    D3DLOCKED_RECT d3dRect;
    ZeroMemory(&d3dRect, sizeof(d3dRect));

    __beginT();

    HRESULT hr = m_pTexture->LockRect(
        level,
        &d3dRect,
        &rc,
        0
    );

    __endT(OtherTime);

    if (FAILED(hr))
    {
        Log.Error(
            "LockBits(rect) FAILED: texture=%s level=%d HRESULT=0x%08X",
            m_Name.c_str(),
            level,
            (unsigned)hr
        );

        return NULL;
    }

    if (!d3dRect.pBits)
    {
        m_pTexture->UnlockRect(level);
        return NULL;
    }

    pitch = d3dRect.Pitch;

    return (BYTE *)d3dRect.pBits;
}


/*****************************************************************************/
/* UnlockBits
/*****************************************************************************/

void TextureDX9::UnlockBits(int level) const
{
    if (!m_pTexture)
        return;

    if (level < 0 || level >= m_pTexture->GetLevelCount())
        return;

    m_pTexture->UnlockRect(level);
}


/*****************************************************************************/
/* Create
/*****************************************************************************/

bool TextureDX9::Create(
    int width,
    int height,
    ColorFormat clrFormat,
    int nMips,
    TextureMemoryPool memPool,
    bool bRenderTarget,
    DepthStencilFormat dsFormat,
    bool bDynamic
)
{
    if (width <= 0 || height <= 0)
        return false;

    m_Type = tt2D;
    m_NMipMaps = nMips;
    m_MemoryPool = memPool;
    m_ColorFormat = clrFormat;
    m_Width = width;
    m_Height = height;

    m_bRenderTarget = bRenderTarget;
    m_DSFormat = dsFormat;
    m_bDynamic = bDynamic;
    m_bNoTexture = false;

    return Create();
}


/*****************************************************************************/
/* Internal Create
/*****************************************************************************/

bool TextureDX9::Create()
{
    DeleteDeviceObjects();

    if (!m_pDevice)
    {
        Log.Error(
            "TextureDX9::Create: m_pDevice == NULL, texture=%s",
            m_Name.c_str()
        );

        return false;
    }

    if (m_Width <= 0 || m_Height <= 0)
    {
        Log.Error(
            "TextureDX9::Create: invalid size %dx%d, texture=%s",
            m_Width,
            m_Height,
            m_Name.c_str()
        );

        return false;
    }

    HRESULT result = S_OK;

    if (m_DSFormat != dsfNone)
    {
        result = m_pDevice->CreateDepthStencilSurface(
            m_Width,
            m_Height,
            ConvertDepthStencilFormat(m_DSFormat),
            D3DMULTISAMPLE_NONE,
            0,
            TRUE,
            &m_pZBuffer,
            NULL
        );
    }
    else
    {
        DWORD usage = 0;

        if (m_bRenderTarget)
            usage |= D3DUSAGE_RENDERTARGET;

        if (m_bDynamic)
            usage |= D3DUSAGE_DYNAMIC;

        result = D3DXCreateTexture(
            m_pDevice,
            m_Width,
            m_Height,
            m_NMipMaps,
            usage,
            ConvertColorFormat(m_ColorFormat),
            ConvertMemoryPool(m_MemoryPool),
            &m_pTexture
        );
    }

    if (FAILED(result))
    {
        Log.Error(
            "TextureDX9::Create FAILED: %s %dx%d HRESULT=0x%08X",
            m_Name.c_str(),
            m_Width,
            m_Height,
            (unsigned int)result
        );

        DeleteDeviceObjects();
        return false;
    }

    if (!m_pTexture && !m_pZBuffer)
    {
        Log.Error(
            "TextureDX9::Create: no texture and no ZBuffer: %s",
            m_Name.c_str()
        );

        return false;
    }

    if (m_pTexture)
    {
        m_pBaseTexture = m_pTexture;

        RECT rc;
        rc.left = 0;
        rc.top = 0;
        rc.right = m_Width;
        rc.bottom = m_Height;

        m_pTexture->AddDirtyRect(&rc);
    }
    else
    {
        m_pBaseTexture = NULL;
    }

    return true;
}


/*****************************************************************************/
/* Reload
/*****************************************************************************/

bool TextureDX9::Reload()
{
    m_bNoTexture = false;

    DeleteDeviceObjects();

    return Load();
}


/*****************************************************************************/
/* GetColorFormat
/*****************************************************************************/

ColorFormat TextureDX9::GetColorFormat()
{
    if (!m_pBaseTexture && !m_pZBuffer)
    {
        Load();
    }

    return m_ColorFormat;
}


/*****************************************************************************/
/* Load
/*****************************************************************************/

bool TextureDX9::Load()
{
    if (m_bNoTexture)
        return false;

    if (m_pBaseTexture)
        return true;

    if (!IRM)
    {
        Log.Error(
            "TextureDX9::Load: IRM == NULL, texture=%s",
            m_Name.c_str()
        );

        return false;
    }

    FilePath path;
    path.GetCWD();

    if (!m_SearchPath.empty())
        _chdir(m_SearchPath.c_str());

    int size = 0;
    int resID = -1;

    FilePath tpath(m_Name.c_str());

    resID = IRM->FindResource(tpath.GetFullPath());

    if (resID == -1)
    {
        tpath.SetExt("dds");
        resID = IRM->FindResource(tpath.GetFullPath());
    }

    if (resID == -1)
    {
        tpath.SetExt("tga");
        resID = IRM->FindResource(tpath.GetFullPath());
    }

    if (resID == -1)
    {
        tpath.SetExt("jpg");
        resID = IRM->FindResource(tpath.GetFullPath());
    }

    if (resID == -1)
    {
        tpath.SetExt("bmp");
        resID = IRM->FindResource(tpath.GetFullPath());
    }

    if (resID == -1)
    {
        Log.Warning(
            "Could not load texture <%s>",
            tpath.GetFullPath()
        );

        path.SetCWD();

        m_bNoTexture = true;
        return false;
    }

    IRM->BindResource(resID, this);

    BYTE *pData = IRM->LockData(resID, size);

    if (!pData || size <= 0)
    {
        Log.Warning(
            "Could not lock texture resource <%s>",
            tpath.GetFullPath()
        );

        IRM->UnlockData(resID);
        path.SetCWD();

        return false;
    }

    bool res = LoadFromMemory(pData, size);

    IRM->UnlockData(resID);

    path.SetCWD();

    if (!res)
    {
        Log.Warning(
            "Could not create texture <%s>",
            m_Name.c_str()
        );
    }

    return res;
}


/*****************************************************************************/
/* LoadFromFile
/*****************************************************************************/

bool TextureDX9::LoadFromFile(const char *fName)
{
    if (!fName || !fName[0])
        return false;

    if (!m_pDevice)
        return false;

    FilePath path;
    path.GetCWD();

    if (!m_SearchPath.empty())
        _chdir(m_SearchPath.c_str());

    D3DXIMAGE_INFO info;
    ZeroMemory(&info, sizeof(info));

    HRESULT hres = D3DXGetImageInfoFromFile(
        fName,
        &info
    );

    if (FAILED(hres))
    {
        Log.Warning(
            "Couldn't get image information <%s>, HRESULT=0x%08X",
            fName,
            (unsigned)hres
        );

        path.SetCWD();
        return false;
    }

    DeleteDeviceObjects();

    if (info.ResourceType == D3DRTYPE_TEXTURE)
    {
        m_Type = tt2D;

        hres = D3DXCreateTextureFromFileEx(
            m_pDevice,
            fName,
            D3DX_DEFAULT,
            D3DX_DEFAULT,
            D3DX_DEFAULT,
            0,
            D3DFMT_UNKNOWN,
            D3DPOOL_MANAGED,
            D3DX_DEFAULT,
            D3DX_DEFAULT,
            0,
            NULL,
            NULL,
            &m_pTexture
        );

        m_pBaseTexture = m_pTexture;
    }
    else if (info.ResourceType == D3DRTYPE_VOLUMETEXTURE)
    {
        m_Type = tt3D;

        hres = D3DXCreateVolumeTextureFromFileEx(
            m_pDevice,
            fName,
            D3DX_DEFAULT,
            D3DX_DEFAULT,
            D3DX_DEFAULT,
            D3DX_DEFAULT,
            0,
            D3DFMT_UNKNOWN,
            D3DPOOL_MANAGED,
            D3DX_DEFAULT,
            D3DX_DEFAULT,
            0,
            NULL,
            NULL,
            &m_pVolTexture
        );

        m_pBaseTexture = m_pVolTexture;
    }
    else if (info.ResourceType == D3DRTYPE_CUBETEXTURE)
    {
        m_Type = ttCubeMap;

        hres = D3DXCreateCubeTextureFromFileEx(
            m_pDevice,
            fName,
            D3DX_DEFAULT,
            D3DX_DEFAULT,
            0,
            D3DFMT_UNKNOWN,
            D3DPOOL_MANAGED,
            D3DX_DEFAULT,
            D3DX_DEFAULT,
            0,
            NULL,
            NULL,
            &m_pCubeTexture
        );

        m_pBaseTexture = m_pCubeTexture;
    }
    else
    {
        Log.Warning(
            "Unsupported texture resource type <%s>",
            fName
        );

        path.SetCWD();
        return false;
    }

    path.SetCWD();

    if (FAILED(hres) || !m_pBaseTexture)
    {
        Log.Warning(
            "Couldn't load texture <%s>, HRESULT=0x%08X",
            fName,
            (unsigned)hres
        );

        DeleteDeviceObjects();
        return false;
    }

    if (m_pTexture)
    {
        D3DSURFACE_DESC sdesc;

        hres = m_pTexture->GetLevelDesc(
            0,
            &sdesc
        );

        if (FAILED(hres))
        {
            DeleteDeviceObjects();
            return false;
        }

        m_Type = tt2D;
        m_NMipMaps = m_pTexture->GetLevelCount();
        m_MemoryPool = ConvertMemoryPool(sdesc.Pool);
        m_ColorFormat = ConvertColorFormat(sdesc.Format);
        m_Width = sdesc.Width;
        m_Height = sdesc.Height;
        m_DSFormat = dsfNone;
        m_bRenderTarget = false;
    }
    else if (m_pCubeTexture)
    {
        D3DSURFACE_DESC sdesc;

        hres = m_pCubeTexture->GetLevelDesc(
            0,
            &sdesc
        );

        if (FAILED(hres))
        {
            DeleteDeviceObjects();
            return false;
        }

        m_Type = ttCubeMap;
        m_NMipMaps = m_pCubeTexture->GetLevelCount();
        m_MemoryPool = ConvertMemoryPool(sdesc.Pool);
        m_ColorFormat = ConvertColorFormat(sdesc.Format);
        m_Width = sdesc.Width;
        m_Height = sdesc.Height;
        m_DSFormat = dsfNone;
        m_bRenderTarget = false;
    }
    else if (m_pVolTexture)
    {
        D3DVOLUME_DESC vdesc;

        hres = m_pVolTexture->GetLevelDesc(
            0,
            &vdesc
        );

        if (FAILED(hres))
        {
            DeleteDeviceObjects();
            return false;
        }

        m_Type = tt3D;
        m_NMipMaps = m_pVolTexture->GetLevelCount();
        m_MemoryPool = ConvertMemoryPool(vdesc.Pool);
        m_ColorFormat = ConvertColorFormat(vdesc.Format);
        m_Width = vdesc.Width;
        m_Height = vdesc.Height;
        m_DSFormat = dsfNone;
        m_bRenderTarget = false;
    }

    m_bNoTexture = false;

    return true;
}


/*****************************************************************************/
/* LoadFromMemory
/*****************************************************************************/

bool TextureDX9::LoadFromMemory(
    const BYTE *pBuf,
    int nBytes
)
{
    if (!m_pDevice)
        return false;

    if (!pBuf || nBytes <= 0)
    {
        Log.Warning(
            "Couldn't create texture from empty memory data: %s",
            m_Name.c_str()
        );

        return false;
    }

    D3DXIMAGE_INFO info;
    ZeroMemory(&info, sizeof(info));

    HRESULT hres = D3DXGetImageInfoFromFileInMemory(
        pBuf,
        nBytes,
        &info
    );

    if (FAILED(hres))
    {
        Log.Warning(
            "Couldn't read texture information from memory: %s HRESULT=0x%08X",
            m_Name.c_str(),
            (unsigned)hres
        );

        return false;
    }

    DeleteDeviceObjects();

    if (info.ResourceType == D3DRTYPE_TEXTURE)
    {
        m_Type = tt2D;

        hres = D3DXCreateTextureFromFileInMemoryEx(
            m_pDevice,
            pBuf,
            nBytes,
            D3DX_DEFAULT,
            D3DX_DEFAULT,
            D3DX_DEFAULT,
            0,
            D3DFMT_UNKNOWN,
            D3DPOOL_MANAGED,
            D3DX_DEFAULT,
            D3DX_DEFAULT,
            0,
            NULL,
            NULL,
            &m_pTexture
        );

        m_pBaseTexture = m_pTexture;
    }
    else if (info.ResourceType == D3DRTYPE_VOLUMETEXTURE)
    {
        m_Type = tt3D;

        hres = D3DXCreateVolumeTextureFromFileInMemoryEx(
            m_pDevice,
            pBuf,
            nBytes,
            D3DX_DEFAULT,
            D3DX_DEFAULT,
            D3DX_DEFAULT,
            D3DX_DEFAULT,
            0,
            D3DFMT_UNKNOWN,
            D3DPOOL_MANAGED,
            D3DX_DEFAULT,
            D3DX_DEFAULT,
            0,
            NULL,
            NULL,
            &m_pVolTexture
        );

        m_pBaseTexture = m_pVolTexture;
    }
    else if (info.ResourceType == D3DRTYPE_CUBETEXTURE)
    {
        m_Type = ttCubeMap;

        hres = D3DXCreateCubeTextureFromFileInMemoryEx(
            m_pDevice,
            pBuf,
            nBytes,
            D3DX_DEFAULT,
            D3DX_DEFAULT,
            0,
            D3DFMT_UNKNOWN,
            D3DPOOL_MANAGED,
            D3DX_DEFAULT,
            D3DX_DEFAULT,
            0,
            NULL,
            NULL,
            &m_pCubeTexture
        );

        m_pBaseTexture = m_pCubeTexture;
    }
    else
    {
        Log.Warning(
            "Unsupported texture resource type in memory: %s",
            m_Name.c_str()
        );

        return false;
    }

    if (FAILED(hres) || !m_pBaseTexture)
    {
        Log.Warning(
            "Couldn't create texture from memory data: %s HRESULT=0x%08X",
            m_Name.c_str(),
            (unsigned)hres
        );

        DeleteDeviceObjects();
        return false;
    }

    if (m_pTexture)
    {
        D3DSURFACE_DESC sdesc;

        hres = m_pTexture->GetLevelDesc(
            0,
            &sdesc
        );

        if (FAILED(hres))
        {
            DeleteDeviceObjects();
            return false;
        }

        m_Type = tt2D;
        m_NMipMaps = m_pTexture->GetLevelCount();
        m_MemoryPool = ConvertMemoryPool(sdesc.Pool);
        m_ColorFormat = ConvertColorFormat(sdesc.Format);
        m_Width = sdesc.Width;
        m_Height = sdesc.Height;

        bool goodX = false;
        bool goodY = false;

        for (int i = 0; i < 12; i++)
        {
            int sz = 1 << i;

            if (m_Width == sz)
                goodX = true;

            if (m_Height == sz)
                goodY = true;
        }

        if (!goodX || !goodY)
        {
            Log.Warning(
                "Texture <%s> has an incorrect size: %dx%d",
                m_Name.c_str(),
                m_Width,
                m_Height
            );
        }
    }
    else if (m_pCubeTexture)
    {
        D3DSURFACE_DESC sdesc;

        hres = m_pCubeTexture->GetLevelDesc(
            0,
            &sdesc
        );

        if (FAILED(hres))
        {
            DeleteDeviceObjects();
            return false;
        }

        m_Type = ttCubeMap;
        m_NMipMaps = m_pCubeTexture->GetLevelCount();
        m_MemoryPool = ConvertMemoryPool(sdesc.Pool);
        m_ColorFormat = ConvertColorFormat(sdesc.Format);
        m_Width = sdesc.Width;
        m_Height = sdesc.Height;
    }
    else if (m_pVolTexture)
    {
        D3DVOLUME_DESC vdesc;

        hres = m_pVolTexture->GetLevelDesc(
            0,
            &vdesc
        );

        if (FAILED(hres))
        {
            DeleteDeviceObjects();
            return false;
        }

        m_Type = tt3D;
        m_NMipMaps = m_pVolTexture->GetLevelCount();
        m_MemoryPool = ConvertMemoryPool(vdesc.Pool);
        m_ColorFormat = ConvertColorFormat(vdesc.Format);
        m_Width = vdesc.Width;
        m_Height = vdesc.Height;
    }
    else
    {
        DeleteDeviceObjects();
        return false;
    }

    m_DSFormat = dsfNone;
    m_bRenderTarget = false;
    m_bNoTexture = false;

    return true;
}


/*****************************************************************************/
/* LoadHeader
/*****************************************************************************/

void TextureDX9::LoadHeader()
{
    if (!IRM)
        return;

    FilePath oldPath;
    oldPath.GetCWD();

    if (!m_SearchPath.empty())
        _chdir(m_SearchPath.c_str());

    int size = 0;

    int resID = IRM->FindResource(
        m_Name.c_str()
    );

    if (resID == -1)
    {
        oldPath.SetCWD();
        return;
    }

    FilePath path(
        IRM->GetFullPath(resID)
    );

    path.SetFileName("");
    path.SetExt("");

    m_SearchPath = path;

    IRM->BindResource(resID, this);

    BYTE *pData = IRM->LockData(
        resID,
        size
    );

    if (!pData || size <= 0)
    {
        IRM->UnlockData(resID);
        oldPath.SetCWD();
        return;
    }

    D3DXIMAGE_INFO info;
    ZeroMemory(&info, sizeof(info));

    HRESULT hres = D3DXGetImageInfoFromFileInMemory(
        pData,
        size,
        &info
    );

    if (SUCCEEDED(hres))
    {
        m_Width = info.Width;
        m_Height = info.Height;
    }

    IRM->UnlockData(resID);

    oldPath.SetCWD();
}


/*****************************************************************************/
/* GetSize
/*****************************************************************************/

int TextureDX9::GetSize() const
{
    if (!m_pBaseTexture)
        return 0;

    int nPix = m_Width * m_Height;

    return ColorValue::GetBitmapSize(
        m_ColorFormat,
        nPix
    );
}