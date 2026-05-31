/*****************************************************************************/
/*    File:    sgGU16.h
/*    Desc:    .g16 uniform format treatment
/*    Author:    Ruslan Shestopalyuk
/*    Date:    27.02.2003
/*****************************************************************************/
#ifndef __SGGU16_H__
#define __SGGU16_H__



#pragma pack( push )
#pragma pack( 1 )
/*****************************************************************
/*    Struct:    GU16Header
/*    Desc:    GU16 file header, as it is in the file
/*****************************************************************/
struct GU16Header
{
    DWORD       m_Magic;            // "GU16"
    DWORD       m_BlockSize;        // size of the pixel data block
    BYTE        m_NFramesPerSegment;// number of sprites in segment
    WORD        m_NSprites;         // number of sprites in package
    WORD        m_XSize;            // width of the sprite, in pixels
    WORD        m_YSize;            // height of the sprite, in pixels
    DWORD       m_MaxWorkbuf;       // maximum workbuf size for unpack
    WORD        m_NPackSegments;    // number of packing segments
};  // class GU16Header

/*****************************************************************
/*    Class:    GU16SegHdr
/*    Desc:    Uniformly packed segment
/*****************************************************************/
class GU16SegHdr
{
    //unsigned int        offset        :28        //  offset of the segment
    //PackMethods        packMethod    :4;        //  packing method
    DWORD                data;
public:
    _inl unsigned int GetOffset()        const { return data >> 4;    }
    _inl unsigned int GetPackFlags()    const { return data & 0xF;    }
};  // class GU16SegHdr

/*****************************************************************
/*    Struct:    GU16SpriteHdr
/*    Desc:    Uniformly packed sprite header
/*****************************************************************/
class GU16SpriteHdr
{
    WORD            data;            
public:
    WORD            GetNChunks() const { return data; } 
}; // struct GU16SpriteHdr 
#pragma pack( pop )

/*****************************************************************************/
/*    Class:    GU16Package
/*    Desc:    Uniform G16 sprite package
/*****************************************************************************/
class GU16Package : public QuadSpritePackage
{
    int                        m_FramesPerSegment;
    int                        m_PixelDataSize;
    int                        m_FrameWidth;
    int                        m_FrameHeight;

public:
                               GU16Package         ();
    virtual void               Init                ( const BYTE* data );
    virtual const BYTE*        GetSegmentData      ( DWORD sprID, DWORD& dataSize, DWORD& segIdx, 
                                                       DWORD& firstInSeg, DWORD& nFrames, 
                                                       DWORD* frameOffset, DWORD color = 0 );
    virtual int                GetFrameNSquares    ( int frameID );
	virtual int                GetFrameWidth       ( int sprID ) { return sprID==-1?0:m_FrameWidth;  }
    virtual int                GetFrameHeight      ( int sprID ) { return sprID==-1?0:m_FrameHeight; }
    virtual DWORD              GetFileSize         () const { return m_PixelDataSize; }
    virtual Rct                GetFrameBounds      ( int sprID );
    virtual DWORD              GetMagic            () const { return '61UG'; }


private:
    _inl const GU16SpriteHdr*  GetSpriteHeader     ( int frameID );

}; // class GU16Package

/*****************************************************************************/
/*    Class:    GU16Creator
/*    Desc:    Loader of the uniform G16 sprite package
/*****************************************************************************/
class GU16Creator : public IPackageCreator
{
public:
    GU16Creator();
    virtual SpritePackage*    CreatePackage( char* fileName, const BYTE* data );
    virtual const char*       Description() const;

}; // class GU16Creator



#ifdef _INLINES
#include "sgGU16.inl"
#endif // _INLINES

#endif // __GPGU16_H__