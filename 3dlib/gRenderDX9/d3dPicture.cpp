/*****************************************************************************/
/*  File:   rsPictureManagerDX.cpp
/*  Desc:   PictureInstance manager implementation
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************************/

#include "gRenderPch.h"

#include "ITexture.h"
#include "IPictureManager.h"
#include "IResourceManager.h"
#include "vMesh.h"
#include "d3dAdapt.h"
#include "kStaticArray.hpp"

IDirect3DDevice9 *GetDirect3DDevice();
IDirect3DSurface9 *GetDirect3DSurface(int texID);


/*****************************************************************/
/*  Class:  PictureInstance
/*  Desc:   Single picture file instance
/*****************************************************************/

class PictureInstance
{
public:

    struct Chunk
    {
        int     m_TexID;
        Rct     m_Extents;
        Rct     m_UV;
    };

protected:

    IDirect3DSurface9 *m_pSurface;
    std::vector<Chunk>  m_Chunks;

    int                 m_Width;
    int                 m_Height;
    int                 m_NMipLevels;

    char                m_FileName[_MAX_PATH];

    ColorFormat         m_ColorFormat;
    PictureFileFormat   m_FileFormat;

    int                 m_Stride;

    bool                m_bValid;
    bool                m_bLoaded;

    friend class PictureManager;

public:

    PictureInstance();
    ~PictureInstance();

    bool            HasFileName(const char *name)
    {
        if (!name)
            return false;

        return stricmp(m_FileName, name) == 0;
    }

    bool            IsLoaded() const
    {
        return m_bLoaded;
    }

    bool            IsValid() const
    {
        return m_bValid;
    }

    void            Load();
    void            Unload();

    int             GetNChunks() const
    {
        return (int)m_Chunks.size();
    }

    Chunk &GetChunk(int idx)
    {
        return m_Chunks[idx];
    }

    void            ClearChunks();
    bool            CreateChunks();
};


/*****************************************************************/

const int           c_ChunkSide = 256;
const ColorFormat   c_DrawFormat = cfARGB8888;
const int           c_MaxPictures = 128;


/*****************************************************************/
/*  Class:  PictureManager
/*  Desc:   Implementation of the picture manager using D3DX
/*****************************************************************/

class PictureManager : public IPictureManager, public IDeviceClient
{
    static_array<PictureInstance, c_MaxPictures> m_Pic;

    BaseMesh        m_Primitive;

    DWORD           m_Diffuse;
    bool            m_bFiltering;

    Matrix4D        m_TM;
    bool            m_bTMEnabled;

public:

    PictureManager();

    virtual int     GetImageID(
        const char *fileName);

    virtual bool    LoadImage(
        int imgID);

    virtual bool    UnloadImage(
        int imgID);

    virtual void    Purge();

    virtual BYTE *GetPixels(
        int imgID);

    virtual int     GetWidth(
        int imgID);

    virtual int     GetHeight(
        int imgID);

    virtual const char *GetFileName(
        int imgID);

    virtual bool    DrawImage(
        int imgID,
        float x,
        float y,
        float z = 0.0f);

    virtual bool    DrawImage(
        int imgID,
        const Rct &ext,
        float z = 0.0f);

    virtual bool    IsValid(
        int imgID);

    virtual bool    IsLoaded(
        int imgID);

    virtual void    SetDiffuse(
        DWORD color)
    {
        m_Diffuse = color;
    }

    virtual void    SetFiltering(
        bool bFilter = true);

    virtual PictureFileFormat GetFileFormat(
        int imgID);

    virtual ColorFormat GetColorFormat(
        int imgID);

    virtual void    OnDestroyRS();
    virtual void    OnCreateRS();

    virtual void    SetTransform(
        const Matrix4D &tm)
    {
        m_TM = tm;
    }

    virtual const Matrix4D &GetTransform() const
    {
        return m_TM;
    }

    virtual void    EnableTransform(
        bool bEnable = true)
    {
        m_bTMEnabled = bEnable;
    }

    virtual bool    TransformEnabled() const
    {
        return m_bTMEnabled;
    }
};


/*****************************************************************/
/*  Global picture manager
/*****************************************************************/

IPictureManager *GetPictureManager()
{
    static PictureManager s_PictureManager;
    return &s_PictureManager;
}

DIALOGS_API IPictureManager *IPM = NULL;


void InitPictureManager()
{
    IPM = GetPictureManager();
}


/*****************************************************************/
/*  PictureInstance implementation
/*****************************************************************/

PictureInstance::PictureInstance()
{
    m_Width = 0;
    m_Height = 0;
    m_NMipLevels = 0;

    m_FileName[0] = 0;

    m_ColorFormat = cfUnknown;
    m_FileFormat = pfUnknown;

    m_Stride = 0;

    m_bValid = false;
    m_bLoaded = false;

    m_pSurface = NULL;
}


/*****************************************************************/

PictureInstance::~PictureInstance()
{
    Unload();
}


/*****************************************************************/

void PictureInstance::Load()
{
    if (m_bLoaded)
        return;

    if (!m_bValid)
        return;

    if (m_FileName[0] == 0)
        return;

    int resID = IRM->FindResource(m_FileName);

    if (resID == -1)
    {
        Log.Error(
            "Could not find picture resource: %s",
            m_FileName
        );

        return;
    }

    /*
        Make sure there are no old resources.
    */
    Unload();

    int size = 0;

    BYTE *buf = IRM->LockData(
        resID,
        size
    );

    if (!buf || size <= 0)
    {
        if (buf)
            IRM->UnlockData(resID);

        Log.Error(
            "Could not lock picture resource: %s",
            m_FileName
        );

        return;
    }

    IDirect3DDevice9 *pDevice = GetDirect3DDevice();

    if (!pDevice)
    {
        IRM->UnlockData(resID);

        Log.Error(
            "Could not get Direct3D device while loading picture: %s",
            m_FileName
        );

        return;
    }

    /*
        Create temporary system/scratch surface.

        This surface is only used as an intermediate source for creating
        256x256 managed textures.
    */
    HRESULT hr = pDevice->CreateOffscreenPlainSurface(
        m_Width,
        m_Height,
        ConvertColorFormat(m_ColorFormat),
        D3DPOOL_SCRATCH,
        &m_pSurface,
        NULL
    );

    DX_CHK(hr);

    if (FAILED(hr) || !m_pSurface)
    {
        m_pSurface = NULL;

        IRM->UnlockData(resID);

        Log.Error(
            "Could not create scratch surface for picture: %s",
            m_FileName
        );

        return;
    }

    D3DXIMAGE_INFO info;

    ZeroMemory(
        &info,
        sizeof(info)
    );

    hr = D3DXLoadSurfaceFromFileInMemory(
        m_pSurface,
        NULL,
        NULL,
        buf,
        size,
        NULL,
        D3DX_FILTER_NONE,
        0,
        &info
    );

    IRM->UnlockData(resID);

    DX_CHK(hr);

    if (FAILED(hr))
    {
        Log.Error(
            "Could not load image from file %s",
            m_FileName
        );

        SAFE_RELEASE(m_pSurface);

        m_bLoaded = false;

        return;
    }

    /*
        Create texture chunks from the loaded surface.
    */
    if (!CreateChunks())
    {
        SAFE_RELEASE(m_pSurface);

        m_bLoaded = false;

        return;
    }

    /*
        CreateChunks releases m_pSurface after successful conversion.
    */
    m_bLoaded = true;

} // PictureInstance::Load


/*****************************************************************/

void PictureInstance::Unload()
{
    /*
        This was an important bug in the original implementation.

        m_pSurface is a COM object. It must be released, not simply
        assigned NULL.
    */
    SAFE_RELEASE(m_pSurface);

    ClearChunks();

    m_bLoaded = false;
} // PictureInstance::Unload


/*****************************************************************/

void PictureInstance::ClearChunks()
{
    for (size_t i = 0; i < m_Chunks.size(); ++i)
    {
        if (m_Chunks[i].m_TexID != -1)
        {
            IRS->DeleteTexture(
                m_Chunks[i].m_TexID
            );
        }
    }

    m_Chunks.clear();
} // PictureInstance::ClearChunks


/*****************************************************************/

const float c_TexelBias = 0.375f;


/*****************************************************************/

bool PictureInstance::CreateChunks()
{
    if (!m_pSurface)
        return false;

    if (m_Width <= 0 || m_Height <= 0)
        return false;

    ClearChunks();

    int cx = 0;
    int cy = 0;

    while (cy < m_Height)
    {
        cx = 0;

        while (cx < m_Width)
        {
            Chunk newChunk;

            newChunk.m_TexID = -1;

            /*
                Calculate actual chunk dimensions.
            */
            int right =
                tmin(
                    m_Width,
                    cx + c_ChunkSide
                );

            int bottom =
                tmin(
                    m_Height,
                    cy + c_ChunkSide
                );

            int chunkWidth = right - cx;
            int chunkHeight = bottom - cy;

            if (chunkWidth <= 0 || chunkHeight <= 0)
            {
                cx += c_ChunkSide;
                continue;
            }

            char texName[_MAX_PATH];

            sprintf(
                texName,
                "%s_%d%d",
                m_FileName,
                cx / c_ChunkSide,
                cy / c_ChunkSide
            );

            newChunk.m_TexID =
                IRS->CreateTexture(
                    texName,
                    c_ChunkSide,
                    c_ChunkSide,
                    c_DrawFormat,
                    1,
                    tmpManaged
                );

            if (newChunk.m_TexID == -1)
            {
                Log.Error(
                    "Could not create picture chunk texture: %s",
                    texName
                );

                ClearChunks();

                return false;
            }

            /*
                Picture coordinates.
            */
            newChunk.m_Extents.x =
                cx + c_TexelBias;

            newChunk.m_Extents.y =
                cy + c_TexelBias;

            newChunk.m_Extents.w =
                (float)chunkWidth;

            newChunk.m_Extents.h =
                (float)chunkHeight;

            /*
                Texture coordinates.
            */
            newChunk.m_UV.x = 0.0f;
            newChunk.m_UV.y = 0.0f;

            newChunk.m_UV.w =
                (float)chunkWidth /
                (float)c_ChunkSide;

            newChunk.m_UV.h =
                (float)chunkHeight /
                (float)c_ChunkSide;

            IDirect3DSurface9 *pChunkSurface =
                GetDirect3DSurface(
                    newChunk.m_TexID
                );

            if (!pChunkSurface)
            {
                IRS->DeleteTexture(
                    newChunk.m_TexID
                );

                ClearChunks();

                Log.Error(
                    "Could not get Direct3D surface for picture chunk: %s",
                    texName
                );

                return false;
            }

            RECT srcRect;

            srcRect.left = cx;
            srcRect.top = cy;
            srcRect.right = right;
            srcRect.bottom = bottom;

            RECT dstRect;

            dstRect.left = 0;
            dstRect.top = 0;
            dstRect.right = chunkWidth;
            dstRect.bottom = chunkHeight;

            HRESULT hr =
                D3DXLoadSurfaceFromSurface(
                    pChunkSurface,
                    NULL,
                    &dstRect,
                    m_pSurface,
                    NULL,
                    &srcRect,
                    D3DX_DEFAULT,
                    0
                );

            pChunkSurface->Release();

            DX_CHK(hr);

            if (FAILED(hr))
            {
                IRS->DeleteTexture(
                    newChunk.m_TexID
                );

                ClearChunks();

                Log.Error(
                    "Could not copy picture chunk: %s",
                    texName
                );

                return false;
            }

            m_Chunks.push_back(
                newChunk
            );

            cx += c_ChunkSide;
        }

        cy += c_ChunkSide;
    }

    /*
        Temporary source surface is no longer needed.
    */
    SAFE_RELEASE(m_pSurface);

    return !m_Chunks.empty();

} // PictureInstance::CreateChunks


/*****************************************************************/
/*  PictureManager implementation
/*****************************************************************/

PictureManager::PictureManager()
{
    m_Primitive.create(
        4,
        6,
        vfVertexTnL,
        ptTriangleList
    );

    m_Primitive.setNVert(4);
    m_Primitive.setNPri(2);
    m_Primitive.setNInd(6);

    WORD *pIdx =
        m_Primitive.getIndices();

    if (pIdx)
    {
        pIdx[0] = 0;
        pIdx[1] = 1;
        pIdx[2] = 2;

        pIdx[3] = 2;
        pIdx[4] = 1;
        pIdx[5] = 3;
    }

    m_Diffuse = 0xFFFFFFFF;
    m_bFiltering = false;

    m_TM = Matrix4D::identity;
    m_bTMEnabled = false;

} // PictureManager::PictureManager


/*****************************************************************/

int PictureManager::GetImageID(
    const char *fileName)
{
    if (!fileName || fileName[0] == 0)
        return -1;

    int resID =
        IRM->FindResource(
            fileName
        );

    if (resID == -1)
    {
        Log.Error(
            "Could not find picture resource: %s",
            fileName
        );

        return -1;
    }

    /*
        First check whether the picture is already registered.

        IMPORTANT:
        We do not need to lock the resource just to find an existing
        PictureInstance.
    */
    for (int i = 0; i < (int)m_Pic.size(); ++i)
    {
        if (m_Pic[i].HasFileName(fileName))
        {
            if (!m_Pic[i].IsLoaded())
                LoadImage(i);

            return i;
        }
    }

    /*
        Resource is new. Now inspect its image header.
    */
    int size = 0;

    BYTE *buf =
        IRM->LockData(
            resID,
            size
        );

    if (!buf || size <= 0)
    {
        if (buf)
            IRM->UnlockData(resID);

        Log.Error(
            "Could not lock picture resource: %s",
            fileName
        );

        return -1;
    }

    D3DXIMAGE_INFO info;

    ZeroMemory(
        &info,
        sizeof(info)
    );

    HRESULT hr =
        D3DXGetImageInfoFromFileInMemory(
            buf,
            size,
            &info
        );

    /*
        Always unlock the resource after the D3DX call.
    */
    IRM->UnlockData(resID);

    DX_CHK(hr);

    if (FAILED(hr))
    {
        Log.Error(
            "Could not load picture file %s.",
            fileName
        );

        return -1;
    }

    if (m_Pic.size() >= c_MaxPictures)
    {
        Log.Error(
            "PictureManager: max picture number reached."
        );

        return -1;
    }

    PictureInstance &pic =
        m_Pic.expand();

    /*
        Avoid strcpy overflow.
    */
    strncpy(
        pic.m_FileName,
        fileName,
        _MAX_PATH - 1
    );

    pic.m_FileName[_MAX_PATH - 1] = 0;

    pic.m_Width =
        (int)info.Width;

    pic.m_Height =
        (int)info.Height;

    pic.m_FileFormat =
        (PictureFileFormat)info.ImageFileFormat;

    pic.m_NMipLevels =
        (int)info.MipLevels;

    pic.m_bValid =
        true;

    pic.m_bLoaded =
        false;

    pic.m_ColorFormat =
        ConvertColorFormat(
            info.Format
        );

    return (int)m_Pic.size() - 1;

} // PictureManager::GetImageID


/*****************************************************************/

bool PictureManager::LoadImage(
    int imgID)
{
    if (imgID < 0 ||
        imgID >= (int)m_Pic.size())
    {
        return false;
    }

    m_Pic[imgID].Load();

    return m_Pic[imgID].IsLoaded();

} // PictureManager::LoadImage


/*****************************************************************/

bool PictureManager::UnloadImage(
    int imgID)
{
    if (imgID < 0 ||
        imgID >= (int)m_Pic.size())
    {
        return false;
    }

    m_Pic[imgID].Unload();

    return true;

} // PictureManager::UnloadImage


/*****************************************************************/

void PictureManager::Purge()
{
    for (int i = 0; i < (int)m_Pic.size(); ++i)
    {
        m_Pic[i].Unload();
    }

} // PictureManager::Purge


/*****************************************************************/

BYTE *PictureManager::GetPixels(
    int imgID)
{
    /*
        The current PictureInstance stores the source image only
        temporarily in a D3D scratch surface and then converts it
        into texture chunks.

        Therefore there is currently no persistent CPU-side pixel buffer
        to return here.
    */

    return NULL;

} // PictureManager::GetPixels


/*****************************************************************/

int PictureManager::GetWidth(
    int imgID)
{
    if (imgID < 0 ||
        imgID >= (int)m_Pic.size())
    {
        return 0;
    }

    return m_Pic[imgID].m_Width;

} // PictureManager::GetWidth


/*****************************************************************/

int PictureManager::GetHeight(
    int imgID)
{
    if (imgID < 0 ||
        imgID >= (int)m_Pic.size())
    {
        return 0;
    }

    return m_Pic[imgID].m_Height;

} // PictureManager::GetHeight


/*****************************************************************/

const char *PictureManager::GetFileName(
    int imgID)
{
    if (imgID < 0 ||
        imgID >= (int)m_Pic.size())
    {
        return NULL;
    }

    return m_Pic[imgID].m_FileName;

} // PictureManager::GetFileName


/*****************************************************************/

bool PictureManager::DrawImage(
    int imgID,
    float x,
    float y,
    float z)
{
    if (imgID < 0 ||
        imgID >= (int)m_Pic.size())
    {
        return false;
    }

    PictureInstance &pic =
        m_Pic[imgID];

    return DrawImage(
        imgID,
        Rct(
            x,
            y,
            (float)pic.m_Width,
            (float)pic.m_Height
        ),
        z
    );

} // PictureManager::DrawImage


/*****************************************************************/

bool PictureManager::DrawImage(
    int imgID,
    const Rct &ext,
    float z)
{
    if (imgID < 0 ||
        imgID >= (int)m_Pic.size())
    {
        return false;
    }

    PictureInstance &pic =
        m_Pic[imgID];

    if (!pic.IsLoaded())
    {
        if (!LoadImage(imgID))
            return false;
    }

    if (!pic.IsLoaded())
        return false;

    if (pic.m_Width <= 0 ||
        pic.m_Height <= 0)
    {
        return false;
    }

    float wScale =
        ext.w / (float)pic.m_Width;

    float hScale =
        ext.h / (float)pic.m_Height;


    static int shID =
        IRS->GetShaderID("hud");

    static int shID_L =
        IRS->GetShaderID("hud_L");

    if (shID == -1 || shID_L == -1)
    {
        return false;
    }

    if (m_bFiltering)
        m_Primitive.setShader(shID_L);
    else
        m_Primitive.setShader(shID);


    int nChunks =
        pic.GetNChunks();

    if (nChunks <= 0)
        return false;


    for (int i = 0; i < nChunks; ++i)
    {
        PictureInstance::Chunk &chunk =
            pic.GetChunk(i);

        if (chunk.m_TexID == -1)
            return false;

        VertexTnL *v =
            (VertexTnL *)m_Primitive.getVertexData();

        if (!v)
            return false;


        /*
            Vertex 0
        */
        v[0].x =
            chunk.m_Extents.x * wScale + ext.x;

        v[0].y =
            chunk.m_Extents.y * hScale + ext.y;

        v[0].u =
            chunk.m_UV.x;

        v[0].v =
            chunk.m_UV.y;

        v[0].z = z;
        v[0].w = 1.0f;
        v[0].diffuse = m_Diffuse;


        /*
            Vertex 1
        */
        v[1].x =
            chunk.m_Extents.GetRight() * wScale + ext.x;

        v[1].y =
            chunk.m_Extents.y * hScale + ext.y;

        v[1].u =
            chunk.m_UV.GetRight();

        v[1].v =
            chunk.m_UV.y;

        v[1].z = z;
        v[1].w = 1.0f;
        v[1].diffuse = m_Diffuse;


        /*
            Vertex 2
        */
        v[2].x =
            chunk.m_Extents.x * wScale + ext.x;

        v[2].y =
            chunk.m_Extents.GetBottom() * hScale + ext.y;

        v[2].u =
            chunk.m_UV.x;

        v[2].v =
            chunk.m_UV.GetBottom();

        v[2].z = z;
        v[2].w = 1.0f;
        v[2].diffuse = m_Diffuse;


        /*
            Vertex 3
        */
        v[3].x =
            chunk.m_Extents.GetRight() * wScale + ext.x;

        v[3].y =
            chunk.m_Extents.GetBottom() * hScale + ext.y;

        v[3].u =
            chunk.m_UV.GetRight();

        v[3].v =
            chunk.m_UV.GetBottom();

        v[3].z = z;
        v[3].w = 1.0f;
        v[3].diffuse = m_Diffuse;


        /*
            Optional transformation.
        */
        if (m_bTMEnabled)
        {
            for (int j = 0; j < 4; ++j)
            {
                Vector3D cv(
                    v[j].x,
                    v[j].y,
                    v[j].z
                );

                m_TM.transformPt(cv);

                v[j].x = cv.x;
                v[j].y = cv.y;
                v[j].z = cv.z;
            }
        }


        m_Primitive.setTexture(
            chunk.m_TexID
        );

        DrawBM(
            m_Primitive
        );
    }

    /*
        The original implementation returned false here even after
        successfully drawing the image.

        That was a real logic bug.
    */
    return true;

} // PictureManager::DrawImage


/*****************************************************************/

bool PictureManager::IsValid(
    int imgID)
{
    if (imgID < 0 ||
        imgID >= (int)m_Pic.size())
    {
        return false;
    }

    return m_Pic[imgID].IsValid();

} // PictureManager::IsValid


/*****************************************************************/

PictureFileFormat PictureManager::GetFileFormat(
    int imgID)
{
    if (imgID < 0 ||
        imgID >= (int)m_Pic.size())
    {
        return pfUnknown;
    }

    return m_Pic[imgID].m_FileFormat;

} // PictureManager::GetFileFormat


/*****************************************************************/

ColorFormat PictureManager::GetColorFormat(
    int imgID)
{
    if (imgID < 0 ||
        imgID >= (int)m_Pic.size())
    {
        return cfUnknown;
    }

    return m_Pic[imgID].m_ColorFormat;

} // PictureManager::GetColorFormat


/*****************************************************************/

bool PictureManager::IsLoaded(
    int imgID)
{
    if (imgID < 0 ||
        imgID >= (int)m_Pic.size())
    {
        return false;
    }

    return m_Pic[imgID].IsLoaded();

} // PictureManager::IsLoaded


/*****************************************************************/

void PictureManager::SetFiltering(
    bool bFilter)
{
    m_bFiltering = bFilter;

} // PictureManager::SetFiltering


/*****************************************************************/

void PictureManager::OnDestroyRS()
{
    for (int i = 0; i < (int)m_Pic.size(); ++i)
    {
        m_Pic[i].Unload();
    }

} // PictureManager::OnDestroyRS


/*****************************************************************/

void PictureManager::OnCreateRS()
{
    /*
        Picture textures use tmpManaged memory, so they will normally
        be recreated by the texture manager as needed.

        Nothing else is required here.
    */

} // PictureManager::OnCreateRS