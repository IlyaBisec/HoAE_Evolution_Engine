/*****************************************************************************/
/*    File:    sgGN16.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    03.03.2003
/*****************************************************************************/
#include "stdafx.h"

#include "kHash.hpp"
#include "kResource.h"
#include "sgSpriteManager.h"
#include "sgGQuad.h"
#include "sgGN16.h"
#include "FPack.h"

#ifndef _INLINES
#include "sgGN16.inl"
#endif // _INLINES

/*****************************************************************************/
/*    GN16Package implementation
/*****************************************************************************/
GN16Package::GN16Package() : m_PixelDataSize(0)
{
}

Rct GN16Package::GetFrameBounds( int sprID )
{
    const GN16SpriteHdr* pSpriteHdr = GetSpriteHeader( sprID );
    if (!pSpriteHdr) return Rct::null;
    return Rct( 0.0f, 0.0f, pSpriteHdr->GetWidth(), pSpriteHdr->GetHeight() );
} // GN16Package::GetFrameBounds

void GN16Package::Init( const BYTE* data )
{
    if (!data) return;
    GN16Header* pHeader = (GN16Header*)data;
    m_NFrames            = pHeader->m_NFrames;
    m_NSegments            = pHeader->m_NSegments;
    m_WorkBufSize        = pHeader->m_MaxWorkbuf;
    m_PixelDataSize        = pHeader->m_BlockSize;
} // GN16Package::Init

const BYTE*    GN16Package::GetSegmentData( DWORD sprID, DWORD& dataSize, DWORD& segIdx, 
                                         DWORD& firstInSeg, DWORD& nFrames, 
                                         DWORD* frameOffset, DWORD color )
{
    const BYTE* pData = GetFileData();
    if (!pData) return NULL;

    const GN16SpriteHdr* pSpriteHeader = GetSpriteHeader( sprID );
    segIdx = pSpriteHeader->GetSegIdx();
    const GN16SegHdr* pSegHeader = GetSegmentHeader( segIdx );
    nFrames = pSegHeader->GetNFrames();
    firstInSeg = 0;
    const GN16SegHdr* pCountHeader = GetSegmentHeader( 0 );
    for (int i = 0; i < segIdx; i++)
    {
        firstInSeg += pCountHeader->GetNFrames();        
        pCountHeader++;
    }

    const BYTE* pSeg = pData + pSegHeader->GetOffset();
    DWORD flags    = pSegHeader->GetPackFlags();
    if (segIdx == m_NSegments - 1) 
    {
        dataSize = m_PixelDataSize - pSegHeader->GetOffset();
    }
    else
    {
        const GN16SegHdr* pNextHeader = pSegHeader + 1; 
        dataSize = pNextHeader->GetOffset() - pSegHeader->GetOffset();
    }

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
} // GN16Package::GetSegmentData

int GN16Package::GetFrameNSquares( int frameID )
{
    const BYTE* pData = GetFileData();
    if (!pData) return NULL;
    const GN16SpriteHdr* pHeader = GetSpriteHeader( frameID );
    return pHeader->GetNChunks();
} // GN16Package::GetFrameNSquares

int    GN16Package::GetFrameWidth( int sprID )
{
	if(sprID==-2)return sprID=0;
	if(sprID==-1)return 0;
    const GN16SpriteHdr* pSpriteHeader = GetSpriteHeader( sprID );
    if (!pSpriteHeader) return 0;
    return pSpriteHeader->GetWidth();
}

int GN16Package::GetFrameHeight( int sprID )
{
	if(sprID==-2)return sprID=0;
	if(sprID==-1)return 0;
    const GN16SpriteHdr* pSpriteHeader = GetSpriteHeader( sprID );
    if (!pSpriteHeader) return 0;
    return pSpriteHeader->GetHeight();
}

/*****************************************************************************/
/*    GN16Creator implementation
/*****************************************************************************/
GN16Creator::GN16Creator()
{
    SpritePackage::RegisterCreator( this );
}

SpritePackage* GN16Creator::CreatePackage( char* fileName, const BYTE* data )
{
    if (!data) return NULL;
    DWORD magic = *((DWORD*)data);
    if (magic != '61NG') return NULL;
    GN16Package* pPackage = new GN16Package();
    pPackage->Init( data );
    return pPackage;
} // GN16Creator::Load

const char*    GN16Creator::Description() const
{
    return "Non-uniform G16 Sprite Loader";
} // GN16Creator::Description



    
