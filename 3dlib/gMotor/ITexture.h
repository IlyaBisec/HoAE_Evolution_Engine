/*****************************************************************
/*  File:   ITexture.h                                      
/*  Desc:   Interface to the texture
/*    Author:    Ruslan Shestopalyuk
/*  Date:   Oct 2004                                             
/*****************************************************************/
#ifndef __ITEXTURE_H__ 
#define __ITEXTURE_H__

/*****************************************************************************/
/*    Enum:    TextureMemoryPool
/*    Desc:    Memory location of the texture resource
/*****************************************************************************/
enum TextureMemoryPool
{
    tmpUnknown       = 0,
    tmpSystem        = 1,
    tmpDefault       = 2,
    tmpManaged       = 3
};  // enum TextureMemoryPool

/*****************************************************************************/
/*    Enum:    TextureUsage    
/*****************************************************************************/
enum TextureUsage
{
    tuUnknown        = 0,
    tuLoadable       = 1,
    tuProcedural     = 2,
    tuRenderTarget   = 3,
    tuDynamic        = 4,
    tuDepthStencil   = 5
};    // enum TextureUsage

/*****************************************************************/
/*    Enum:    TextureType
/*    Desc:    Interface to texture
/*****************************************************************/
enum TextureType
{
    tt1D        = 0,
    tt2D        = 1,
    tt3D        = 2,
    ttCubeMap   = 3
}; // enum TextureType

/*****************************************************************
/*  Enum:  ColorFormat
/*  Desc:  Pixel color format
/*****************************************************************/
enum ColorFormat
{
    cfUnknown       = 0,
    cfARGB4444      = 1,
    cfXRGB1555      = 2,
    cfARGB8888      = 3,
    cfRGB565        = 4,
    cfA8            = 5,
    cfRGB888        = 6,
    cfXRGB8888      = 7,
    cfV8U8          = 8,
    cfR16F          = 9,
    cfGR16F         = 10,
    cfABGR16F       = 11,
    cfR32F          = 12,
    cfGR32F         = 13,
    cfABGR32F       = 14,
    cfDXT1          = 15,
    cfDXT2          = 16,
    cfDXT3          = 17,
    cfDXT4          = 18,
    cfDXT5          = 19,
}; // enum ColorFormat

/*****************************************************************************/
/*    Enum: DepthStencilFormat
/*****************************************************************************/
enum DepthStencilFormat
{
    dsfNone         = 0,    //  not a depth-stencil surface
    dsfD16Lockable  = 1,    //  16 bit lockable surface
    dsfD32          = 2,    
    dsfD15S1        = 3,
    dsfD24S8        = 4,
    dsfD16          = 5
}; // DepthStencilFormat

class Rct;
/*****************************************************************/
/*    Class:    ITexture
/*    Desc:    Interface to texture
/*****************************************************************/
class ITexture
{
public:

    virtual const char*         GetName         () const = 0;
    virtual TextureType         GetType         () const = 0;
    virtual int                 GetNMipMaps     () const = 0;
    virtual TextureMemoryPool   GetMemoryPool   () const = 0;
    virtual ColorFormat         GetColorFormat  () = 0;
    virtual int                 GetWidth        () const = 0;
    virtual int                 GetHeight       () const = 0;
    virtual bool                HasAlpha        () const = 0;
    virtual bool                IsRenderTarget  () const = 0;
    virtual bool                IsDepthStencil  () const = 0;
    virtual int                 GetApproxSize   () const = 0;
    virtual bool                SaveToFile      ( const char* fName ) const = 0;
    
    virtual void                SetName         ( const char* name ) = 0;
    virtual void                LoadHeader      () = 0;
    virtual bool                Load            () = 0; 

    virtual void                SetID           ( int id ) = 0;
    virtual int                 GetID           () const = 0;

    virtual void                Bind            ( int stage = 0 ) = 0;

    virtual BYTE*               LockBits        ( int& pitch, int level = 0 ) = 0;
    virtual BYTE*               LockBits        ( int& pitch, const Rct& rect, int level = 0 ) const = 0;
    virtual void                UnlockBits      ( int level = 0 ) const = 0;

    virtual bool                Create          ( int width, int height, 
                                                    ColorFormat clrFormat, 
                                                    int nMips = 0, 
                                                    TextureMemoryPool memPool = tmpDefault, 
                                                    bool bRenderTarget = false, 
                                                    DepthStencilFormat dsFormat = dsfNone,
                                                    bool bDynamic = false ) = 0;

    virtual bool                Reload          () = 0;

    virtual void                DeleteDeviceObjects     () = 0;
    virtual void                RestoreDeviceObjects    () = 0;
    virtual void                InvalidateDeviceObjects () = 0;

}; // class ITexture

#endif // __ITEXTURE_H__ 