/*****************************************************************************/
/*    File:    sgSurfaceCache.h
/*    Author:    Ruslan Shestopalyuk
/*    Date:    15.12.2003
/*****************************************************************************/
#ifndef __SGSURFACECACHE_H__
#define __SGSURFACECACHE_H__

/****************************************************************************/
/*  Class:  QuadPack                                     
/*  Desc:    Describes layout of sprite chunks on the texture surface
/****************************************************************************/
class QuadPack
{
    /*****************************************************************************/
    /*    Struct:    PackLevel
    /*****************************************************************************/
    struct PackLevel
    {
        int            qx[3];
        int            qy[3];
        int            numQuads;
    };  // struct PackLevel
    
    static const int c_MaxLevel = 15;

public:
    _inl                    QuadPack        () {}

    _inl void               Free            ();
    _inl bool               AllocateQuad    ( PowerOfTwo side, int& allocx, int& allocy );
    void                    SetSide         ( PowerOfTwo side ) { m_TopLevel = GetPower( side ); }

protected:

    PackLevel               m_Level[c_MaxLevel + 1];
    int                     m_TopFreeLevel;
    int                     m_TopLevel;

}; //  class QuadPack


/*****************************************************************************/
/*    Class:    SurfaceCacheItem
/*****************************************************************************/
class SurfaceCacheItem : public Texture
{
public:
                    SurfaceCacheItem();
    virtual void    Expose( PropertyMap& pm );

    DECLARE_SCLASS(SurfaceCacheItem, SNode, SCAI);

private:
    QuadPack            m_QuadLayout;
}; // class SurfaceCacheItem

/*****************************************************************************/
/*    Class:    SurfaceCache
/*****************************************************************************/
class SurfaceCache : public SNode
{
public:
                                    SurfaceCache    ();
                                    ~SurfaceCache   ();

    virtual void                    Render          ();
    virtual void                    Expose          ( PropertyMap& pm );

    virtual void                    CreateContents  ();

    int                             GetNSurfaces    () const { return m_NSurfaces;      }
    PowerOfTwo                      GetSurfaceSide  () const { return m_SurfaceSide;    }
    ColorFormat                     GetColorFormat  () const { return m_ColorFormat;    }
    bool                            IsRT            () const { return m_bRenderTarget;  }
    MemoryPool                      GetMemoryPool   () const { return m_MemoryPool;     }

    void                            SetNSurfaces    ( int         val );            
    void                            SetSurfaceSide  ( PowerOfTwo  val );    
    void                            SetColorFormat  ( ColorFormat val );    
    void                            SetIsRT         ( bool        val );    
    void                            SetMemoryPool   ( MemoryPool  val );    

    DECLARE_SCLASS(SurfaceCache, SNode, SCAC);

protected:
    int                     m_NSurfaces;            //  number of the surfaces allocated in cache
    PowerOfTwo              m_SurfaceSide;          //  side of the surface, in texels
    ColorFormat             m_ColorFormat;          //  surfaces color fomat
    MemoryPool              m_MemoryPool;           //  memory pool type
    bool                    m_bRenderTarget;        //  whether surfaces are render targets

}; // class SurfaceCache



#ifdef _INLINES
#include "sgSurfaceCache.inl"
#endif // _INLINES

#endif // __SGSURFACECACHE_H__
