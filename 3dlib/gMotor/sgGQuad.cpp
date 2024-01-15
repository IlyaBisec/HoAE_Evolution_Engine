/*****************************************************************************/
/*    File:    sgGQuad.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    03.03.2003
/*****************************************************************************/
#include "stdafx.h"
#include "kHash.hpp"
#include "kResource.h"
#include "sgSpriteManager.h"
#include "sgGQuad.h"

#ifndef _INLINES
#include "sgGQuad.inl"
#endif // _INLINES


/*****************************************************************************/
/*    QuadSpritePackage implementation
/*****************************************************************************/
FrameInstance* QuadSpritePackage::PrecacheFrame( int sprID, DWORD color, int lod ) 
{ 
    //  create and init frame instance
    FrameInstance* pInst = g_SpriteManager.FindFrameInstance( m_ID, sprID, color, lod );
    if (pInst && pInst->IsCached()) return pInst;
    if (sprID < 0 || sprID >= m_NFrames) return NULL;
    _chdir( IRM->GetHomeDirectory() );

    //  extract pointer to the packed segment data for this frame
    DWORD nFrames, dataSize, firstInSeg, segIdx;
    DWORD frameOffset[c_MaxFramesInPackedSegment];
    const BYTE* pData = GetSegmentData( sprID, dataSize, segIdx, firstInSeg, 
                                        nFrames, &frameOffset[0], color );
    if (!pData)
    {
        Log.Error( "Could not unpack pixel data in file %s", GetPath() );
        return NULL;
    }

    //  pecache all brother frames in the segment
    for (int i = 0; i < nFrames; i++)
    {
        int curFrame = firstInSeg + i;
        const BYTE* pFrame = pData + frameOffset[i];
        FrameInstance* cInst = g_SpriteManager.FindFrameInstance( m_ID, curFrame, color, lod );
        if (!cInst)
        {
            int nChunks    = GetFrameNSquares( curFrame );
            int nExtra    = nChunks * (sizeof( FrameChunk ) + sizeof( FrameVert )*4);

            //  allocate frame instance from the pool
            cInst = g_SpriteManager.AllocFrameInstance( nExtra );
            if (!cInst) return false;
            cInst->SetNChunks   ( nChunks, (void*)(cInst + 1) );
            cInst->SetSeqID     ( m_ID       ); 
            cInst->SetFrameID   ( curFrame   );
            cInst->SetColor     ( color      );
            cInst->SetLOD       ( lod        );
            cInst->SetWidth     ( GetFrameWidth ( curFrame ) );
            cInst->SetHeight    ( GetFrameHeight( curFrame ) );
            DWORD sprID = g_SpriteManager.AddFrameInstance( cInst );
            cInst->SetID        ( sprID      );
            cInst->Drop();
        }

        if (!cInst->IsCached()) 
        {
            const BYTE* pChunk    = pFrame;    
            int            surfID    = -1;
            FrameVert* v = (FrameVert*)((BYTE*)(cInst + 1) + cInst->m_NChunks*sizeof(FrameChunk));
            
            //  iterate on chunks
            int minX =  INT_MAX;
            int minY =  INT_MAX;
            int maxX = -INT_MAX;
            int maxY = -INT_MAX;
            
            cInst->Lock();

            int nChunks = cInst->GetNChunks();
            for (int i = 0; i < nChunks; i++)
            {
                FrameChunkHeader*    pChunkHdr = (FrameChunkHeader*) pChunk;
                FrameChunk&            chunk      = cInst->m_Chunk[i];
                int                    sidePow      = pChunkHdr->GetSidePow() - cInst->GetLOD();

                //  allocate chunk on the surface    
                WORD ax, ay;
                surfID = g_SpriteManager.AllocateQuad( sidePow, ax, ay, cInst, surfID );
                if (surfID == -1)
                {
                    Log.Error( "Could not cache frame from sprite package: %s", GetName() );
                    return NULL;
                }

                int side            = pChunkHdr->GetSide() >> cInst->GetLOD();
                int x               = pChunkHdr->GetX();
                int y               = pChunkHdr->GetY();
                int s               = pChunkHdr->GetSide();

                //  fill chunk vertices
                v[0].x                = x;
                v[0].y                = y;
                v[0].z                = 0.0f;
                v[0].u                = s_UV[ax];         
                v[0].v                = s_UV[ay];         
                v[0].color          = 0xFF808080;

                v[1].x                = x + s;
                v[1].y                = y;
                v[1].z                = 0.0f;
                v[1].u                = s_UV[ax + side];  
                v[1].v                = s_UV[ay];         
                v[1].color          = 0xFF808080;

                v[2].x                = x;
                v[2].y                = y + s;
                v[2].z                = 0.0f;
                v[2].u                = s_UV[ax];         
                v[2].v                = s_UV[ay + side];  
                v[2].color          = 0xFF808080;

                v[3].x                = x + s;
                v[3].y                = y + s;
                v[3].z                = 0.0f;
                v[3].u                = s_UV[ax + side];  
                v[3].v                = s_UV[ay + side];  
                v[3].color          = 0xFF808080;

                minX = tmin( minX, x );
                minY = tmin( minY, y );
                maxX = tmax( maxX, x + s );
                maxY = tmax( maxY, y + s );

                chunk.m_NVert        = 4;
                chunk.m_NTri         = 2;
                chunk.m_Vert         = v;
                chunk.m_Idx          = NULL;
                chunk.m_SurfaceID    = surfID;
                chunk.m_TextureID    = 0xFFFF;
                v += 4;
                
                //  copy chunk pixel data to the working surface
                Rct     rect( ax, ay, side, side );
                int        pitch        = 0;
                int        sideBytes    = side << 1;
                int        srcPitch    = pChunkHdr->GetSide() << 1;
                BYTE*    pCh            = pChunkHdr->GetPixelData();
                BYTE*    pB            = g_SpriteManager.LockSurfRect( surfID, rect, pitch );
                if (!pB) continue;

                if (cInst->GetLOD() == 0)
                {
                    util::MemcpyRect( pB, pitch, pChunkHdr->GetPixelData(), srcPitch, side, sideBytes );
                }
                else if (cInst->GetLOD() == 1)
                {
                    util::QuadShrink2xW( pB, pitch, pChunkHdr->GetPixelData(), srcPitch, pChunkHdr->GetSide() );
                }
                else if (cInst->GetLOD() == 2)
                {
                    util::QuadShrink4xW( pB, pitch, pChunkHdr->GetPixelData(), srcPitch, pChunkHdr->GetSide() );
                }

                g_SpriteManager.UnlockSurfRect();

                pChunk += pChunkHdr->GetSizeBytes();
            }            

            cInst->Unlock();

            cInst->SetSegIdx( segIdx    );
            cInst->SetBounds( minX, minY, maxX - minX, maxY - minY );
            cInst->SetZBounds( 0.0f, 0.0f );
        }
    }
    pInst = g_SpriteManager.FindFrameInstance( m_ID, sprID, color, lod );
    return pInst; 
} // QuadSpritePackage::PrecacheFrame

DWORD QuadSpritePackage::GetAlpha( FrameInstance* frameInst, int ptX, int ptY, bool bPrecise )
{
    float fX = ptX;
    float fY = ptY;

    int nChunks = frameInst->GetNChunks();
    int inIdx = -1;
    for (int i = 0; i < nChunks; i++)
    {
        FrameChunk&    chunk = frameInst->GetChunk( i );
        FrameVert* v = chunk.m_Vert;
        //  check if point is inside chunk square
        if (fX < v[0].x || fY < v[0].y || fX >= v[3].x || fY >= v[3].y) continue;
        if (!bPrecise) return c_InsideChunks;
        inIdx = i;
        break;
    }

    if (inIdx == -1) return c_OutsideChunks;

    //  cannot do per-pixel testing
    return c_InsideChunks;

    //  well, now we do precise per-pixel testing
    FrameChunk&    chunk = frameInst->GetChunk( inIdx );
    FrameVert* v = chunk.m_Vert;
    float ax = v[0].u*float( c_GPTexSide );
    float ay = v[0].v*float( c_GPTexSide );
    int texID = g_SpriteManager.GetSurfaceTexID( chunk.m_SurfaceID );
    int        pitch        = 0;
    BYTE*    pB            = IRS->LockTexBits( texID, pitch );
    if (!pB) return 0;
    pB += int( ay + fY - v[0].y )*pitch + int( ax + fX - v[0].x )*2;
    DWORD clr = *((WORD*)pB);
    IRS->UnlockTexBits( texID );
    return (clr&0x0000F000)>>12;
} // QuadSpritePackage::GetAlpha



    
