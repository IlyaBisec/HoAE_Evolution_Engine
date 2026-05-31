/*****************************************************************************/
/*    File:    sgGU15.h
/*    Desc:    .g15 uniform format treatment
/*    Author:    Ruslan Shestopalyuk
/*    Date:    27.02.2003
/*****************************************************************************/
#ifndef __SGGU15_H__
#define __SGGU15_H__



#pragma pack( push )
#pragma pack( 1 )
/*****************************************************************
/*    Struct:    GU15Header
/*    Desc:    GU15 file header, as it is in the file
/*****************************************************************/
struct GU15Header
{
    DWORD       m_Magic;            // "GU15"
    DWORD       m_BlockSize;        // total size of the file
    BYTE        m_NSprites;         // number of sprites in package
    WORD        m_XSize;            // width of the sprite, in pixels
    WORD        m_YSize;            // height of the sprite, in pixels
    WORD        m_InfoLen;          // size of additional info
};  // class GU15Header

/*****************************************************************
/*    Struct:    GU15SpriteHdr
/*    Desc:    Uniformly packed sprite header
/*****************************************************************/
class GU15SpriteHdr
{
    WORD        m_NChunks;        //  number of squares in sprite
    WORD        m_Reserved;
    DWORD       m_Offset;         //  frame offset        
public:
    WORD        GetNChunks() const { return m_NChunks; } 
    DWORD       GetOffset()  const { return m_Offset;  } 
}; // struct GU15SpriteHdr 
#pragma pack( pop )

/*****************************************************************************/
/*    Class:    GU15Package
/*    Desc:    Uniform G15 sprite package
/*****************************************************************************/
class GU15Package : public QuadSpritePackage
{
    int                         m_PixelDataSize;
    int                         m_FrameWidth;
    int                         m_FrameHeight;
    int                         m_InfoLen;

public:
                                GU15Package       ();
    virtual void                Init              ( const BYTE* data );
    virtual const BYTE*         GetSegmentData    ( DWORD sprID, DWORD& dataSize, DWORD& segIdx, 
                                                     DWORD& firstInSeg, DWORD& nFrames, 
                                                     DWORD* frameOffset, DWORD color = 0 );
    virtual int                 GetFrameNSquares  ( int frameID );
    virtual DWORD               GetMagic          () const { return '51UG'; }

private:
    _inl const GU15SpriteHdr*   GetSpriteHeader   ( int frameID );

}; // class GU15Package

/*****************************************************************************/
/*    Class:    GU15Creator
/*    Desc:    Loader of the uniform G16 sprite package
/*****************************************************************************/
class GU15Creator : public IPackageCreator
{
public:
    GU15Creator();
    virtual SpritePackage*    CreatePackage( char* fileName, const BYTE* data );
    virtual const char*       Description() const;

}; // class GU15Creator



#ifdef _INLINES
#include "sgGU15.inl"
#endif // _INLINES

#endif // __GPGU15_H__