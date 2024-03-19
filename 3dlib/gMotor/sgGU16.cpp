/*****************************************************************************/
/*    File:    sgGU16.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    03.03.2003
/*****************************************************************************/
#include "stdafx.h"

#include "kHash.hpp"
#include "kResource.h"
#include "sgSpriteManager.h"
#include "sgGQuad.h"
#include "sgGU16.h"
#include "FPack.h"

#ifndef _INLINES
#include "sgGU16.inl"
#endif // _INLINES

/*****************************************************************************/
/*    GU16Package implementation
/*****************************************************************************/
GU16Package::GU16Package() : m_FramesPerSegment(0)
{
}

Rct GU16Package::GetFrameBounds( int sprID )
{
    return Rct( 0.0f, 0.0f, m_FrameWidth, m_FrameHeight );
} // QuadSpritePackage::GetFrameBounds

void GU16Package::Init( const BYTE* data )
{
    if (!data) return;
    GU16Header* pHeader  = (GU16Header*)data;
    m_NFrames            = pHeader->m_NSprites;
    m_NSegments          = pHeader->m_NPackSegments;
    m_FramesPerSegment   = pHeader->m_NFramesPerSegment;
    m_WorkBufSize        = pHeader->m_MaxWorkbuf;
    m_PixelDataSize      = pHeader->m_BlockSize;
    m_FrameWidth         = pHeader->m_XSize;
    m_FrameHeight        = pHeader->m_YSize;
} // GU16Package::Init

const BYTE*    GU16Package::GetSegmentData( DWORD sprID, DWORD& dataSize, DWORD& segIdx, 
                                            DWORD& firstInSeg, DWORD& nFrames, 
                                            DWORD* frameOffset, DWORD color )
{
    const BYTE* pData = GetFileData();
    if (!pData) return NULL;

    segIdx = sprID / m_FramesPerSegment;
    nFrames = m_FramesPerSegment;
    const GU16SegHdr* pHeader = (GU16SegHdr*)(pData + sizeof(GU16Header) + segIdx*sizeof(GU16SegHdr)); 

    if (segIdx == m_NSegments - 1) 
    {
        nFrames = m_NFrames - (m_NSegments - 1)*m_FramesPerSegment;
        dataSize = m_PixelDataSize - pHeader->GetOffset();
    }
    else
    {
        const GU16SegHdr* pNextHeader = pHeader + 1; 
        dataSize = pNextHeader->GetOffset() - pHeader->GetOffset();
    }

    firstInSeg = segIdx * m_FramesPerSegment;

    const BYTE* pSeg = pData + pHeader->GetOffset();
    DWORD flags;
    flags = pHeader->GetPackFlags();
    
    const BYTE* pPalette0 = GetPalette( 0 );
    const BYTE* pPalette1 = GetPalette( 1 );
    if (pPalette0 && pPalette1) G16SetPalette( (BYTE*)pPalette0, (BYTE*)pPalette1 );
    G16SetNationalColor( (color & 0x00FF0000) >> 16, (color & 0x0000FF00) >> 8, color & 0x000000FF );

    AdjustWorkBuffer( GetWorkBufSize() );
    DWORD unpackedSize = *((DWORD*)pSeg);
    AdjustUnpackBuffer( unpackedSize );

    bool res = G16UnpackSegment( const_cast<BYTE*>( pSeg ), 
                                 dataSize - 4, s_UnpackBuffer, s_WorkBuffer, 
                                 (unsigned int*)frameOffset, nFrames, 
                                 flags );
    if (!res) return NULL;
    return s_UnpackBuffer;
} // GU16Package::GetSegmentData

int GU16Package::GetFrameNSquares( int frameID )
{
    const GU16SpriteHdr* pHeader = GetSpriteHeader( frameID );
    return pHeader->GetNChunks();
} // GU16Package::GetFrameNSquares

/*****************************************************************************/
/*    GU16Creator implementation
/*****************************************************************************/
GU16Creator::GU16Creator()
{
    SpritePackage::RegisterCreator( this );
}

SpritePackage* GU16Creator::CreatePackage( char* fileName, const BYTE* data )
{
    if (!data) return NULL;
    DWORD magic = *((DWORD*)data);
    if (magic != '61UG') return NULL;
    GU16Package* pPackage = new GU16Package();
    pPackage->Init( data );
    return pPackage;
} // GU16Creator::Load

const char*    GU16Creator::Description() const
{
    return "Uniform G16 Sprite Loader";
} // GU16Creator::Description



