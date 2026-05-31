/*****************************************************************************/
/*    File:    sgGU15.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    03.03.2003
/*****************************************************************************/
#include "stdafx.h"

#include "kHash.hpp"
#include "kResource.h"
#include "sgSpriteManager.h"
#include "sgGQuad.h"
#include "sgGU15.h"

#ifndef _INLINES
#include "sgGU15.inl"
#endif // _INLINES

/*****************************************************************************/
/*    GU15Package implementation
/*****************************************************************************/
GU15Package::GU15Package() 
{
}

void GU15Package::Init( const BYTE* data )
{
    if (!data) return;
    GU15Header* pHeader  = (GU15Header*)data;
    m_NFrames            = pHeader->m_NSprites;
    m_NSegments          = m_NFrames;
    m_WorkBufSize        = 0;
    m_PixelDataSize      = pHeader->m_BlockSize;
} // GU15Package::Init

const BYTE*    GU15Package::GetSegmentData( DWORD sprID, DWORD& dataSize, DWORD& segIdx, 
                                            DWORD& firstInSeg, DWORD& nFrames, 
                                            DWORD* frameOffset, DWORD color )
{
    firstInSeg    = sprID;
    nFrames       = 1;
    segIdx        = sprID;

    return NULL;
} // GU15Package::GetSegmentData

int GU15Package::GetFrameNSquares( int frameID )
{
    const GU15SpriteHdr* pHeader = GetSpriteHeader( frameID );
    return pHeader->GetNChunks();
} // GU15Package::GetFrameNSquares

/*****************************************************************************/
/*    GU15Creator implementation
/*****************************************************************************/
GU15Creator::GU15Creator()
{
    SpritePackage::RegisterCreator( this );
}

SpritePackage* GU15Creator::CreatePackage( char* fileName, const BYTE* data )
{
    if (!data) return NULL;
    DWORD magic = *((DWORD*)data);
    if (magic != '51UG') return NULL;
    GU15Package* pPackage = new GU15Package();
    pPackage->Init( data );
    return pPackage;
} // GU15Creator::Load

const char*    GU15Creator::Description() const
{
    return "Uniform G16 Sprite Loader";
} // GU15Creator::Description


