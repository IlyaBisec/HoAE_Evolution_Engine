/*****************************************************************************/
/*    File:    sgGQuad.h
/*    Desc:    Sprite package for quad-based sprite formats
/*    Author:    Ruslan Shestopalyuk
/*    Date:    27.02.2003
/*****************************************************************************/
#ifndef __SGGQUAD_H__
#define __SGGQUAD_H__



/*****************************************************************
/*    Class:    FrameChunkHeader
/*****************************************************************/
class FrameChunkHeader
{
public:
    _inl int        GetSidePow      ()                  const;
    _inl int        GetSide         ()                  const;
    _inl int        GetX            ()                  const; 
    _inl int        GetY            ()                  const;
    _inl int        GetCacheItem    ()                  const;
    _inl void       GetUV           ( WORD& u, WORD& v )const;
    _inl BYTE*      GetPixelData    ()                  const;
    _inl int        GetSizeBytes    ()                  const;

    _inl bool       IsFull          ()                  const;
    _inl bool       IsLU            ()                  const;
    _inl bool       IsLB            ()                  const;
    _inl bool       IsRU            ()                  const;
    _inl bool       IsRB            ()                  const;

    _inl void       SetCacheItem    ( int cItem );
    _inl void       SetUV           ( WORD u, WORD v );

protected:
private:
    BYTE            data[8]; //  dummy data. Don't touch it with dirty hands
}; // class FrameChunkHeader

/*****************************************************************************/
/*    Class:    QuadSpritePackage
/*****************************************************************************/
class QuadSpritePackage : public SpritePackage
{
public:
    virtual FrameInstance*      PrecacheFrame    ( int sprID, DWORD color = 0, int lod = 0 );
    virtual const BYTE*         GetSegmentData   ( DWORD sprID, DWORD& dataSize, DWORD& segIdx, 
                                                    DWORD& firstInSeg, DWORD& nFrames, 
                                                    DWORD* frameOffset, DWORD color = 0 ){ return NULL; }
    virtual DWORD               GetAlpha         ( FrameInstance* frameInst, int ptX, int ptY, bool bPrecise = false );

}; // class GU2DPackage



#ifdef _INLINES
#include "sgGQuad.inl"
#endif // _INLINES

#endif // __SGGQUAD_H__