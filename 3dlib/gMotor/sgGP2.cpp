/*****************************************************************************/
/*    File:    sgGP2.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    03.03.2003
/*****************************************************************************/
#include "stdafx.h"

#include "kHash.hpp"
#include "kResource.h"
#include "sgSpriteManager.h"
#include "sgGP2.h"
#include "mTriangle.h"
#include "FPack.h"

/*****************************************************************************/
/*    GP2Package implementation
/*****************************************************************************/
FrameInstArray      GP2Package::s_FrameInst;
GP2SurfaceArray     GP2Package::s_Surfaces;
int                 GP2Package::s_LastSurface = -1;
int                 GP2Package::s_ScratchSurf = -1;
GP2TextureHash      GP2Package::s_SurfMapping;
FrameInstAllocator  GP2Package::s_FrameAllocator;

GP2Package::GP2Package()
{
}

void GP2Package::Init( const BYTE* data ) 
{
    if (!data) return;
    GP2Header* pHeader      = (GP2Header*)data;
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

    m_HeaderSize            = c_GP2HeaderSize;

    if (m_InfoLen > 5)
    {
        m_VertexFormat      = pHeader->m_VertexFormat;
    }
    else
    {
        m_HeaderSize--;
        m_VertexFormat      = 0;
    }
    m_VertexSize = c_GP2VertexSize;

    if (s_Surfaces.size() == 0) InitSurfaceCache();
} // GP2Package::Init

void GP2Package::InitSurfaceCache()
{
    if (s_Surfaces.size() != 0) return;
    for (int i = 0; i < c_NumGP2Surfaces; i++)
    {
        GP2Surface& surf = s_Surfaces.expand();
        surf.m_Index = i;

        char texName[256];
        sprintf( texName, "GP2Surf%02d", i );
        int texID = IRS->CreateTexture( texName, c_GP2TexSide, c_GP2TexSide, cfDXT3, 1, tmpDefault );
        surf.m_TexID    = texID;
        surf.m_GPID     = -1;
        surf.m_GPTexID  = -1;
    }
    s_ScratchSurf = IRS->CreateTexture( "GP2Scratch", c_GP2TexSide, c_GP2TexSide, cfDXT3, 1, tmpSystem );
} // GP2Package::InitSurfaceCache


GP2FrameInfo* GP2Package::GetFrameInfo( int sprID )
{
    const BYTE* pData = GetFileData();
    if (!pData) return NULL;
    return (GP2FrameInfo*)(pData + m_HeaderSize + sprID*c_GP2FrameInfoSize);
} // GP2Package::GetFrameInfo

Rct GP2Package::GetFrameBounds( int sprID )
{
    if (sprID < 0 || sprID >= m_NFrames) return Rct::null;
    if (s_FrameInst.size() == c_MaxGP2FrameInst) return Rct::null;
    //  create and init frame instance

    const BYTE* pData = GetFileData();
    if (!pData) return Rct::null;

    GP2FrameInfo* pFrameInfo = GetFrameInfo( sprID );
    if (!pFrameInfo) return Rct::null;
    return Rct( pFrameInfo->m_FrameX0, pFrameInfo->m_FrameY0, 
                pFrameInfo->m_FrameWidth, pFrameInfo->m_FrameHeight ); 
} // GP2Package::GetFrameBounds

int lzo1x_decompress_asm_fast(unsigned char* src, unsigned int src_len,
                              unsigned char* dst, unsigned int* dst_len,
                              unsigned char* wrkmem);

int GP2Package::PrecacheTexture( int texID, DWORD color, int lod, float& begU, float& begV )
{
    DWORD id = s_SurfMapping.find( GP2TextureKey( GetID(), texID ) );
    if (id == NO_ELEMENT)
    {
        id = s_SurfMapping.add( GP2TextureKey( GetID(), texID ) );
    }
    GP2TextureKey& key = s_SurfMapping.elem( id );
    if (key.m_SurfID != -1) return s_Surfaces[key.m_SurfID].m_TexID; 

    s_LastSurface = (s_LastSurface + 1)%s_Surfaces.size();
    GP2Surface& surf = s_Surfaces[s_LastSurface];

    if (surf.m_GPID != -1 && surf.m_GPTexID != -1)
    {
        ISM->Flush();
        DWORD dropID = s_SurfMapping.find( GP2TextureKey( surf.m_GPID, surf.m_GPTexID ) );
        GP2TextureKey& dropKey = s_SurfMapping.elem( dropID );
        dropKey.m_SurfID = -1;
    }
    
    surf.m_GPID     = GetID();
    surf.m_GPTexID  = texID;
    key.m_SurfID    = s_LastSurface;

    const BYTE* pData = GetFileData();
    if (!pData) return -1;
    GP2TextureInfo* pTexInfo = (GP2TextureInfo*)(pData + m_TexturesInfoOffset + texID*GP2TextureInfoSize);
    int w = pTexInfo->m_Width;
    int h = pTexInfo->m_Height;
    assert( w == h && w > 0 );
    int side = w >> lod;
    int sidePow = GetPower( (PowerOfTwo)side );

    static BYTE buf[c_GPTexSide*c_GPTexSide*2];
    const BYTE* pCh = pData + m_TexturesOffset + pTexInfo->m_TextureOffset;
    unsigned int outSize = 0;
    lzo1x_decompress_asm_fast( (unsigned char*)pCh + 9, pTexInfo->m_TextureSize,
                               (unsigned char*)buf, &outSize, NULL );
    
    int realTexID = s_Surfaces[key.m_SurfID].m_TexID;
    int pitch = 0;


    BYTE* pTex = IRS->LockTexBits( s_ScratchSurf, pitch );
    if (!pTex)
    {
        Log.Error( "Could not lock GP2 cache surface." );
        return false;
    }
    memcpy( pTex, buf + outSize - 65536, 65536 );
    IRS->UnlockTexBits( s_ScratchSurf );

    IRS->CopyTexture( realTexID, s_ScratchSurf );

    return realTexID;
} // GP2Package::PrecacheTexture

const float c_HalfTexel = 0.5f/float( c_GPTexSide );
FrameInstance* GP2Package::PrecacheFrame( int sprID, DWORD color, int lod ) 
{ 
	if (sprID < 0 || sprID >= m_NFrames) return NULL;
    if (s_FrameInst.size() == c_MaxGP2FrameInst) return NULL;
    //  create and init frame instance

    const BYTE* pData = GetFileData();
    if (!pData) return NULL;

    GP2FrameInfo* pFrameInfo = GetFrameInfo( sprID );
    if (!pFrameInfo) return NULL;
    int nSubm = pFrameInfo->m_MeshesNumber;
    
    //  calculate total number of vertices/indices needed for this frame instance
    int NI = 0, NV = 0;
    int nExtra = 0;
    const BYTE* pMeshOffset = pData + m_MeshesOffset + pFrameInfo->m_MeshesOffset*c_GP2MeshSize;
    for (int i = 0; i < nSubm; i++)
    {
        GP2Mesh* pSubm = (GP2Mesh*)pMeshOffset;    
        pMeshOffset += c_GP2MeshSize;
        NI += pSubm->m_TrianglesNumber;
        NV += pSubm->m_VerticesNumber;
        nExtra += sizeof(FrameChunk) + 
                    pSubm->m_VerticesNumber*sizeof(FrameVert) + 
                    pSubm->m_TrianglesNumber*3*sizeof(WORD);
    }
    NI *= 3;

    FrameInstance* pInst = s_FrameAllocator.NewInstance( nExtra );
    s_FrameInst.expand();
    s_FrameInst[s_FrameInst.size() - 1] = pInst;

    pInst->SetSeqID   ( m_ID  ); 
    pInst->SetFrameID ( sprID );
    pInst->SetColor   ( color );
    pInst->SetLOD     ( lod   );
    pInst->SetNChunks ( nSubm, (void*)(pInst + 1)  );
    //pInst->SetWidth   ( GetFrameWidth ( sprID )    );
    //pInst->SetHeight  ( GetFrameHeight( sprID )    );
    pInst->SetID      ( s_FrameInst.size() - 1 );
    pInst->Drop       ();
    pInst->Lock       ();

    FrameVert* v = (FrameVert*)((BYTE*)(pInst + 1) + nSubm*sizeof(FrameChunk));
    WORD* idx = (WORD*)((BYTE*)v + NV*sizeof(FrameVert));
    int cSurfID    = -1;
    int cTexID     = -1;

    //  go through all frame submeshes
    pMeshOffset = pData + m_MeshesOffset + pFrameInfo->m_MeshesOffset*c_GP2MeshSize;
    for (int i = 0; i < nSubm; i++)
    {
        GP2Mesh* pSubm = (GP2Mesh*)pMeshOffset;    
        pMeshOffset += c_GP2MeshSize;
        
        float begU = 0.0f;
        float begV = 0.0f;

        //  allocate place in the texture cache and move texture's pixel data
        int texID = pSubm->m_TextureIdx;
        if (texID != cTexID && cTexID != -1)
        {
            int g = 0;
        }
        cTexID = texID;
        WORD* pIdx = (WORD*)(pData + m_IndicesOffset + pSubm->m_TrianglesOffset*sizeof(WORD));
        int surfID = PrecacheTexture( texID, color, lod, begU, begV );
        if (surfID == -1)
        {
            Log.Error( "Could not cache frame from sprite package: %s", GetName() );
            return NULL;
        }

        cSurfID = surfID;

        //  index data
        FrameChunk& chunk = pInst->m_Chunk[i];
        chunk.m_TextureID = surfID;
        chunk.m_SurfaceID = surfID;
        int nTri  = pSubm->m_TrianglesNumber;
        int nIdx  = nTri*3;
        int nV    = pSubm->m_VerticesNumber;

        //  fill chunk vertices
        chunk.m_NVert = nV;
        chunk.m_NTri  = nTri;
        chunk.m_Vert  = v;
        chunk.m_Idx   = idx;
        memcpy( chunk.m_Idx, pIdx, nIdx*sizeof(WORD) );

        GP2Vertex* pVert = (GP2Vertex*)(pData + m_VerticesOffset + pSubm->m_VerticesOffset*m_VertexSize);
        FrameVert* fv=v;
        GP2Vertex* gv=pVert;
        float u0=begU + c_HalfTexel;
        float v0=begV + c_HalfTexel;
        for (int j = 0; j < nV; j++)
        {
            fv->x = *((short*)&gv->x);
            fv->y = *((short*)&gv->y);
            fv->z = 0.0f;
            fv->u = float( *((short*)&gv->u) )/c_GP2TexSide + u0;
            fv->v = float( *((short*)&gv->v) )/c_GP2TexSide + v0;
            fv->color = 0xFF808080;
            fv++;
            gv++;
        }
        
        v        += nV;
        idx      += nIdx;
        pSubm    += c_GP2MeshSize;
    }
    
    pInst->Unlock();
    pInst->SetSegIdx( 0 );
    pInst->SetBounds(   *((short*)&pFrameInfo->m_FrameX0), 
                        *((short*)&pFrameInfo->m_FrameY0), 
                        *((short*)&pFrameInfo->m_FrameWidth), 
                        *((short*)&pFrameInfo->m_FrameHeight) );
    return pInst;
} // GP2Package::PrecacheFrame

DWORD GP2Package::GetAlpha( FrameInstance* frameInst, int ptX, int ptY, bool bPrecise )
{
    float fX = ptX;
    float fY = ptY;

    int nChunks = frameInst->GetNChunks();
    int            inIdx = -1;
    int            inTri = -1;
    Vector3D    inBC;
    for (int i = 0; i < nChunks; i++)
    {
        FrameChunk&         chunk    = frameInst->GetChunk( i );
        const FrameVert*    v        = chunk.m_Vert;
        const WORD*         idx      = chunk.m_Idx;
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
    FrameChunk& chunk = frameInst->GetChunk( inIdx );
    const FrameVert* v = chunk.m_Vert;
    const WORD* idx = chunk.m_Idx + inTri*3;
    const FrameVert& v0 = v[idx[0]];
    const FrameVert& v1 = v[idx[1]];
    const FrameVert& v2 = v[idx[2]];

    //  FIXME: Warping issue should be treated
    float bu = (v0.u*inBC.x + v1.u*inBC.y + v2.u*inBC.z)*float( c_GPTexSide );
    float bv = (v0.v*inBC.x + v1.v*inBC.y + v2.v*inBC.z)*float( c_GPTexSide );

    int texID = g_SpriteManager.GetSurfaceTexID( chunk.m_SurfaceID );
    int pitch = 0;
    BYTE* pB = IRS->LockTexBits( texID, pitch );
    if (!pB) return 0;
    pB += int( bv )*pitch + int( bu )*2;
    DWORD clr = *((WORD*)pB);
    IRS->UnlockTexBits( texID );
    return (clr&0x0000F000)>>12;
} // GP2Package::GetAlpha

int GP2Package::GetFrameWidth( int sprID )
{
	try{
		if(sprID==-2)return m_Width;
		if(sprID==-1)return 0;
		//return m_Width;
		const FrameInstance* frInst = PrecacheFrame( sprID );
		Rct r=frInst->GetBounds();
		return r.w;
	}catch(...){
		return 1;
	}
}

int GP2Package::GetFrameHeight( int sprID )
{
	try{
		if(sprID==-2)return m_Height;
		if(sprID==-1)return 0;
		//return m_Height;
		const FrameInstance* frInst = PrecacheFrame( sprID );
		Rct r=frInst->GetBounds();
		return r.h;
	}catch(...){
		return 1;
	}
}

void GP2Package::OnFrame()
{
    s_FrameInst.clear();
    s_FrameAllocator.Purge();
} // GP2Package::OnFrame

void GP2Package::CleanCache()
{
    s_SurfMapping.reset();
} // GP2Package::CleanCache

/*****************************************************************************/
/*    GP2Creator implementation
/*****************************************************************************/
GP2Creator::GP2Creator()
{
    SpritePackage::RegisterCreator( this );
}

SpritePackage* GP2Creator::CreatePackage( char* fileName, const BYTE* data )
{
    if (!data) return NULL;
    DWORD magic = *((DWORD*)data);
    if (magic != 'D2PG') return NULL;
    GP2Package* pPackage = new GP2Package();
    pPackage->Init( data );
    return pPackage;
} // GP2Creator::Load

const char* GP2Creator::Description() const
{
    return "GP2 Sprite Loader";
} // GP2Creator::Description




    
