/*****************************************************************************/
/*    File:    sgG2D.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    03.03.2003
/*****************************************************************************/
#include "stdafx.h"

#include "kHash.hpp"
#include "kResource.h"
#include "sgSpriteManager.h"
#include "sgG2D.h"
#include "mTriangle.h"
#include "FPack.h"

/*****************************************************************************/
/*    GU2DPackage implementation
/*****************************************************************************/
GU2DPackage::GU2DPackage()
{
}

void GU2DPackage::Init( const BYTE* data ) 
{
    if (!data) return;
    GU2DHeader* pHeader     = (GU2DHeader*)data;
    m_NFrames               = pHeader->m_FramesNumber;
    m_NSegments             = pHeader->m_TexturesNumber;
    m_WorkBufSize           = 0;
    m_PixelDataSize         = pHeader->m_FileSize;

    m_VerticesOffset        = pHeader->m_VerticesOffset;    
    m_VerticesNumber        = pHeader->m_VerticesNumber;    
    m_IndicesOffset         = pHeader->m_IndicesOffset;    
    m_IndicesNumber         = pHeader->m_IndicesNumber;    
    m_MeshesOffset          = pHeader->m_MeshesOffset;            
    m_MeshesNumber          = pHeader->m_MeshesNumber;        
    m_TexturesInfoOffset    = pHeader->m_TexturesInfoOffset;
    m_TexturesOffset        = pHeader->m_TexturesOffset;    
    m_TexturesNumber        = pHeader->m_TexturesNumber;
    m_InfoLen               = pHeader->m_InfoLen;

    m_Width                 = pHeader->m_Width;        
    m_Height                = pHeader->m_Height;        
    m_NDirections           = pHeader->m_Directions;
    m_HeaderSize            = c_GU2DHeaderSize;

    if (m_InfoLen > 5)
    {
        m_VertexFormat      = pHeader->m_VertexFormat;
    }
    else
    {
        m_HeaderSize--;
        m_VertexFormat      = 0;
    }

    if (m_VertexFormat&1)      
    {
        m_VertexSize = c_GU2DVertexCSize;
    }
    else
    {
        m_VertexSize = c_GU2DVertexSize;
    }
} // GU2DPackage::Init

void GU2DPackage::Cleanup()
{
    SpritePackage::Cleanup();
    int nE = s_StampReg.numElem();
    for (int i = 0; i < nE; i++)
    {
        CacheStamp& cs = s_StampReg.elem( i );
        if (cs.m_GPID == GetID())
        {
            cs.m_DropStamp = 0; cs.m_SurfID = 0xFFFFFFFF;
        }
    }
} // GU2DPackage::Cleanup

Rct GU2DPackage::GetFrameBounds( int sprID )
{
    return Rct( 0.0f, 0.0f, m_Width, m_Height );
} // GU2DPackage::GetFrameBounds

void GU2DPackage::FreeSurface( int surfID )
{
    int nE = s_StampReg.numElem();
    for (int i = 0; i < nE; i++)
    {
        CacheStamp& cs = s_StampReg.elem( i );
        if (cs.m_SurfID == surfID)
        {
            cs.m_DropStamp  = 0; 
            cs.m_SurfID     = 0xFFFFFFFF;
        }
    }
} // GU2DPackage::FreeSurface

GU2DFrameInfo* GU2DPackage::GetFrameInfo( int sprID )
{
    const BYTE* pData = GetFileData();
    if (!pData) return NULL;
    return (GU2DFrameInfo*)(pData + m_HeaderSize + sprID*c_GU2DFrameInfoSize);
} // GU2DPackage::GetFrameInfo

Hash<CacheStamp> GU2DPackage::s_StampReg;
CacheStamp&    GU2DPackage::GetCacheStamp( int texID, DWORD color, int lod )
{
    int idx = s_StampReg.add( CacheStamp( GetID(), texID, color, lod ) );
    return s_StampReg.elem( idx );
} // GU2DPackage::GetCacheStamp

int GU2DPackage::PrecacheTexture( int texID, DWORD color, int lod, float& begU, float& begV )
{
    const BYTE* pData = GetFileData();

    if (!pData) return -1;
    
    CacheStamp& cs = GetCacheStamp( texID, color, lod );
    int surfID = cs.m_SurfID;
    if (surfID != 0xFFFFFFFF && g_SpriteManager.GetDropStamp( surfID ) == cs.m_DropStamp) 
    {
        begU = cs.ax;
        begV = cs.ay;
        return surfID;
    }

    //  should it be here?..
    //TouchData();

    GU2DTextureInfo* pTexInfo = (GU2DTextureInfo*)(pData + m_TexturesInfoOffset + texID*GU2DTextureInfoSize);
    int w = pTexInfo->m_Width;
    int h = pTexInfo->m_Height;
    assert( w == h && w > 0 );
    int side = w >> lod;
    int sidePow = GetPower( (PowerOfTwo)side );
    
    WORD ax, ay;
    surfID = g_SpriteManager.AllocateQuad( sidePow, ax, ay, NULL, -1 );
    if (surfID == -1) return -1;
    cs.m_DropStamp = g_SpriteManager.GetDropStamp( surfID );
    cs.m_SurfID    = surfID;
    
    //  unpack texture data
    static BYTE buf[c_GPTexSide*c_GPTexSide*2];
    const BYTE*    pCh    = pData + m_TexturesOffset + pTexInfo->m_TextureOffset;
    G2DUnpackTexture( (unsigned char*)pCh, (unsigned short*)buf, w, h, pTexInfo->m_Flags );
    
    //  paint texture data with national color
    if (color != 0)
    {
        void G16PaintNationColor(int R, int G, int B, int Range, unsigned short* InData);
        G16PaintNationColor((color & 0x00FF0000) >> 16, (color & 0x0000FF00) >> 8, 
            color & 0x000000FF, w, (unsigned short*)buf );
    }

    //  copy chunk pixel data to the working surface
    Rct     rect( ax, ay, side, side );
    int     pitch        = 0;
    int     sideBytes    = side << 1;
    int     srcPitch    = pTexInfo->m_Width << 1;
    BYTE*    pB            = g_SpriteManager.LockSurfRect( surfID, rect, pitch );
    if (!pB) return -1;
         if (lod == 0)    util::MemcpyRect( pB, pitch, buf, srcPitch, side, sideBytes );
    else if (lod == 1)    util::QuadShrink2xW( pB, pitch, buf, srcPitch, w );
    else if (lod == 2)    util::QuadShrink4xW( pB, pitch, buf, srcPitch, w );


    begU = s_UV[ax];
    begV = s_UV[ay];

    cs.ax        = begU;
    cs.ay        = begV;
    cs.m_LOD    = lod;
    cs.m_Color    = color;
    cs.m_GPID    = GetID();
    cs.m_TexID    = texID;

    g_SpriteManager.UnlockSurfRect();
    return surfID;
} // GU2DPackage::PrecacheTexture

const float c_HalfTexel = 0.5f/float( c_GPTexSide );
FrameInstance* GU2DPackage::PrecacheFrame( int sprID, DWORD color, int lod ) 
{ 
    //  create and init frame instance
    FrameInstance* pInst = g_SpriteManager.FindFrameInstance( m_ID, sprID, color, lod );
    if (pInst && pInst->IsCached()) return pInst;
    if (sprID < 0 || sprID >= m_NFrames) return NULL;
    
    _chdir( IRM->GetHomeDirectory() );
    const BYTE* pData = GetFileData();
    if (!pData) return NULL;

    GU2DFrameInfo* pFrameInfo = GetFrameInfo( sprID );
    if (!pFrameInfo) return NULL;
    int nSubm = pFrameInfo->m_MeshesNumber;

    //  calculate total number of vertices/indices needed for this frame instance
    int NI = 0, NV = 0;
    int nExtra = 0;
    const BYTE* pMeshOffset = pData + m_MeshesOffset + pFrameInfo->m_MeshesOffset*c_GU2DMeshSize;
    for (int i = 0; i < nSubm; i++)
    {
        GU2DMesh* pSubm = (GU2DMesh*)pMeshOffset;    
        pMeshOffset += c_GU2DMeshSize;
        NI += pSubm->m_TrianglesNumber;
        NV += pSubm->m_VerticesNumber;
        nExtra += sizeof(FrameChunk) + 
                    pSubm->m_VerticesNumber*sizeof(FrameVert) + 
                    pSubm->m_TrianglesNumber*3*sizeof(WORD);
    }
    NI *= 3;

    if (!pInst)
    {
        //  allocate frame instance from the pool
        pInst = g_SpriteManager.AllocFrameInstance( nExtra );
        if (!pInst) return false;
        pInst->SetSeqID        ( m_ID        ); 
        pInst->SetFrameID    ( sprID        );
        pInst->SetColor        ( color        );
        pInst->SetLOD        ( lod        );
        pInst->SetNChunks    ( nSubm, (void*)(pInst + 1) );
        pInst->SetWidth        ( GetFrameWidth ( sprID )    );
        pInst->SetHeight    ( GetFrameHeight( sprID )    );
        
        pInst->SetID        ( g_SpriteManager.AddFrameInstance( pInst ) );
        pInst->Drop();
    }


    pInst->Lock();
    FrameVert*    v        = (FrameVert*)((BYTE*)(pInst + 1) + nSubm*sizeof(FrameChunk));
    WORD*        idx        = (WORD*)((BYTE*)v + NV*sizeof(FrameVert));
    int            cSurfID    = -1;

    static const float c_VeryBigFloat = 1000000.0f;
    float minX =  c_VeryBigFloat;
    float minY =  c_VeryBigFloat;
    float minZ =  c_VeryBigFloat;

    float maxX = -c_VeryBigFloat;
    float maxY = -c_VeryBigFloat;
    float maxZ = -c_VeryBigFloat;

    //  go through all frame submeshes
    pMeshOffset = pData + m_MeshesOffset + pFrameInfo->m_MeshesOffset*c_GU2DMeshSize;
    for (int i = 0; i < nSubm; i++)
    {
        GU2DMesh* pSubm = (GU2DMesh*)pMeshOffset;    
        pMeshOffset += c_GU2DMeshSize;
        
        float         begU     = 0.0f;
        float         begV     = 0.0f;

        //  allocate place in the texture cache and move texture's pixel data
        int            texID    = pSubm->m_TextureIdx;
        WORD*        pIdx    = (WORD*)(pData + m_IndicesOffset + pSubm->m_TrianglesOffset*sizeof(WORD));
        int            surfID    = PrecacheTexture( texID, color, lod, begU, begV );
        if (surfID == -1)
        {
            Log.Error( "Could not cache frame from sprite package: %s", GetName() );
            return NULL;
        }

        //  add to the surface clients
        if (cSurfID != surfID) 
        {
            g_SpriteManager.AddSurfaceClient( surfID, pInst );
            if (cSurfID != -1)
            {
                int t = 0;
            }
        }
        cSurfID = surfID;

        //  index data
        FrameChunk& chunk    = pInst->m_Chunk[i];
        chunk.m_SurfaceID    = surfID;
        chunk.m_TextureID    = 0xFFFF;

        int         nTri     = pSubm->m_TrianglesNumber;
        int         nIdx     = nTri*3;
        int         nV         = pSubm->m_VerticesNumber;

        //  fill chunk vertices
        chunk.m_NVert        = nV;
        chunk.m_NTri        = nTri;
        chunk.m_Vert        = v;
        chunk.m_Idx            = idx;
        memcpy( chunk.m_Idx, pIdx, nIdx*sizeof(WORD) );

        if (m_VertexFormat&1)
        {
            GU2DVertexC* pVert    = (GU2DVertexC*)(pData + m_VerticesOffset + pSubm->m_VerticesOffset*m_VertexSize);
            for (int j = 0; j < nV; j++)
            {
                v[j].x = pVert[j].x;
                v[j].y = pVert[j].y;
                v[j].z = pVert[j].z;
                v[j].u = s_UV[pVert[j].u>>lod] + begU + c_HalfTexel;
                v[j].v = s_UV[pVert[j].v>>lod] + begV + c_HalfTexel;
                v[j].color = pVert[j].color;

                minX = tmin( minX, v[j].x );
                minY = tmin( minY, v[j].y );
                minZ = tmin( minZ, v[j].z );
                maxX = tmax( maxX, v[j].x );
                maxY = tmax( maxY, v[j].y );
                maxZ = tmax( maxZ, v[j].z );
            }
        }
        else
        {
            GU2DVertex* pVert    = (GU2DVertex*)(pData + m_VerticesOffset + pSubm->m_VerticesOffset*m_VertexSize);
            for (int j = 0; j < nV; j++)
            {
                v[j].x = pVert[j].x;
                v[j].y = pVert[j].y;
                v[j].z = pVert[j].z;
                v[j].u = s_UV[pVert[j].u>>lod] + begU + c_HalfTexel;
                v[j].v = s_UV[pVert[j].v>>lod] + begV + c_HalfTexel;
                v[j].color = 0xFF808080;

                minX = tmin( minX, v[j].x );
                minY = tmin( minY, v[j].y );
                minZ = tmin( minZ, v[j].z );
                maxX = tmax( maxX, v[j].x );
                maxY = tmax( maxY, v[j].y );
                maxZ = tmax( maxZ, v[j].z );
            }
        }
        
        v         += nV;
        idx      += nIdx;
        pSubm    += c_GU2DMeshSize;
    }
    
    pInst->Unlock();
    pInst->SetSegIdx( 0    );

    float cZ = (minZ + maxZ)*0.5f;
    maxZ -= cZ;
    minZ -= cZ;

    for (int i = 0; i < nSubm; i++)
    {
        const FrameChunk& chunk    = pInst->m_Chunk[i];
        for (int j = 0; j < chunk.m_NVert; j++) chunk.m_Vert[j].z -= cZ;
    }

    pInst->SetBounds( minX, minY, maxX - minX, maxY - minY );
    pInst->SetZBounds( minZ, maxZ );

    if (maxZ != 0.0f || minZ != 0.0f) m_bHasDepthData = true;
    if (m_VertexFormat&1) m_bHasColorData = true;
    m_AABB.Union( pInst->GetAABB() );
    return pInst;
} // GU2DPackage::PrecacheFrame

DWORD GU2DPackage::GetAlpha( FrameInstance* frameInst, int ptX, int ptY, bool bPrecise )
{
    float fX = ptX;
    float fY = ptY;

    int nChunks = frameInst->GetNChunks();
    int            inIdx = -1;
    int            inTri = -1;
    Vector3D    inBC;
    for (int i = 0; i < nChunks; i++)
    {
        FrameChunk&            chunk    = frameInst->GetChunk( i );
        const FrameVert*    v        = chunk.m_Vert;
        const WORD*            idx        = chunk.m_Idx;
        for (int j = 0; j < chunk.m_NTri; j++)
        {
            const FrameVert& v0 = v[idx[0]];
            const FrameVert& v1 = v[idx[1]];
            const FrameVert& v2 = v[idx[2]];
            Vector3D bc = BaryCoords( v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, fX, fY );
            
            const float c_TriangleBorder = 0.2f;
            if (!bPrecise &&
                bc.x >= -c_TriangleBorder && bc.x <= 1.0f + c_TriangleBorder && 
                bc.y >= -c_TriangleBorder && bc.y <= 1.0f + c_TriangleBorder &&
                bc.z >= -c_TriangleBorder && bc.z <= 1.0f + c_TriangleBorder) 
            {
                return c_InsideChunks;
            }

            if (bc.x >= 0.0f && bc.x <= 1.0f && 
                bc.y >= 0.0f && bc.y <= 1.0f &&
                bc.z >= 0.0f && bc.z <= 1.0f)
            {
                inIdx    = i;
                inTri    = j;
                inBC    = bc;
            }
            idx += 3;
        }
        if (inTri != -1) break;
    }

    if (inIdx == -1) return c_OutsideChunks;

    //  cannot do per-pixel testing
    return c_InsideChunks;

    //  well, now we do precise per-pixel testing
    FrameChunk&    chunk = frameInst->GetChunk( inIdx );
    const FrameVert*    v        = chunk.m_Vert;
    const WORD*            idx        = chunk.m_Idx + inTri*3;
    const FrameVert& v0 = v[idx[0]];
    const FrameVert& v1 = v[idx[1]];
    const FrameVert& v2 = v[idx[2]];

    //  FIXME: Warping issue should be treated
    float bu = (v0.u*inBC.x + v1.u*inBC.y + v2.u*inBC.z)*float( c_GPTexSide );
    float bv = (v0.v*inBC.x + v1.v*inBC.y + v2.v*inBC.z)*float( c_GPTexSide );

    int texID = g_SpriteManager.GetSurfaceTexID( chunk.m_SurfaceID );
    int        pitch        = 0;
    BYTE*    pB            = IRS->LockTexBits( texID, pitch );
    if (!pB) return 0;
    pB += int( bv )*pitch + int( bu )*2;
    DWORD clr = *((WORD*)pB);
    IRS->UnlockTexBits( texID );
    return (clr&0x0000F000)>>12;
} // GU2DPackage::GetAlpha

int    GU2DPackage::GetFrameWidth( int sprID )
{
    return m_Width;
}

int    GU2DPackage::GetFrameHeight( int sprID )
{
    return m_Height;
}

/*****************************************************************************/
/*    GU2DCreator implementation
/*****************************************************************************/
GU2DCreator::GU2DCreator()
{
    SpritePackage::RegisterCreator( this );
}

SpritePackage* GU2DCreator::CreatePackage( char* fileName, const BYTE* data )
{
    if (!data) return NULL;
    DWORD magic = *((DWORD*)data);
    if (magic != 'D2UG') return NULL;
    GU2DPackage* pPackage = new GU2DPackage();
    pPackage->Init( data );
    return pPackage;
} // GU2DCreator::Load

const char*    GU2DCreator::Description() const
{
    return "G2D Sprite Loader";
} // GU2DCreator::Description




    
