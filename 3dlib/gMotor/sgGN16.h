/*****************************************************************************/
/*    File:    sgGN16.h
/*    Desc:    .g16 non-uniform format treatment
/*    Author:    Ruslan Shestopalyuk
/*    Date:    27.02.2003
/*****************************************************************************/
#ifndef __SGGN16_H__
#define __SGGN16_H__



#pragma pack( push )
#pragma pack( 1 )
/*****************************************************************
/*    Struct:    GN16Header
/*    Desc:    GN16 file header, as it is in the file
/*****************************************************************/
struct GN16Header
{
    DWORD                    m_Magic;        // "GN16"
    DWORD                    m_BlockSize;    // size of the pixel data block
    WORD                    m_NFrames;        // number of frames in package
    DWORD                    m_MaxWorkbuf;    // maximum workbuf size for unpack
    WORD                    m_NSegments;    // number of packing segments
};  // class GN16Header

/*****************************************************************
/*    Class:    GN16SegHdr
/*    Desc:    Non-uniformly packed segment header
/*****************************************************************/
class GN16SegHdr
{
    //unsigned int        offset        :28;    //  offset of the segment
    //PackMethods        packMethod    :4;        //  packing method
    //WORD                firstSprite;
    BYTE                    data[6];
public:
    unsigned int            GetOffset    ()    const    { return (*((DWORD*)data)) >> 4;    }
    unsigned int            GetPackFlags()    const    { return (*((DWORD*)data)) & 0xF;    }
    WORD&                    GetNFrames    ()    const    { return *((WORD*)(&data[4]));        }
}; // class GN16SegHdr

/*****************************************************************
/*    Class:    GN16SpriteHdr
/*    Desc:    Non-uniformly sprite header
/*****************************************************************/
class GN16SpriteHdr
{
    BYTE                    data[8];

public:
    WORD&                    GetNChunks    () const { return *((WORD*)(&data[0])); }
    WORD&                    GetWidth    () const { return *((WORD*)(&data[2])); }
    WORD&                    GetHeight    () const { return *((WORD*)(&data[4])); }                
    WORD&                    GetSegIdx    () const { return *((WORD*)(&data[6])); }                
}; // class GN16SpriteHdr 
#pragma pack( pop )

/*****************************************************************************/
/*    Class:    GN16Package
/*****************************************************************************/
class GN16Package : public QuadSpritePackage
{
public:
                                GN16Package         ();
    virtual void                Init                ( const BYTE* data );
    virtual const BYTE*         GetSegmentData      ( DWORD sprID, DWORD& dataSize, DWORD& segIdx, 
                                                        DWORD& firstInSeg, DWORD& nFrames, 
                                                        DWORD* frameOffset, DWORD color = 0 );

    virtual int                 GetFrameNSquares    ( int frameID );
    virtual int                 GetFrameWidth       ( int sprID );
    virtual int                 GetFrameHeight      ( int sprID );
    virtual DWORD               GetFileSize         () const { return m_PixelDataSize; }
    virtual Rct                 GetFrameBounds      ( int sprID );
    virtual DWORD               GetMagic            () const { return '61NG'; }

private:    
    _inl const GN16SpriteHdr*   GetSpriteHeader     ( int frameID );
    _inl const GN16SegHdr*      GetSegmentHeader    ( int segID );

    int                         m_PixelDataSize;
}; // class GN16Package

/*****************************************************************************/
/*    Class:    GN16Creator
/*    Desc:    Loader of the non-uniform G16 sprite package
/*****************************************************************************/
class GN16Creator : public IPackageCreator
{
public:
                            GN16Creator        ();
    virtual SpritePackage*    CreatePackage    ( char* fileName, const BYTE* data );
    virtual const char*        Description        () const;

}; // class GN16Creator



#ifdef _INLINES
#include "sgGN16.inl"
#endif // _INLINES

#endif // __SGGN16_H__