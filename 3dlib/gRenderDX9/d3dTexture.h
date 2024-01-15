/*****************************************************************************/
/*	File:	d3dTexture.h
/*  Desc:	Texture interface implementation for DirectX9 
/*	Author:	Ruslan Shestopalyuk
/*	Date:	02.11.2004
/*****************************************************************************/
#ifndef __D3DTEXTURE_H__
#define __D3DTEXTURE_H__
#include "ITexture.h"
#include "IResourceManager.h"

/*****************************************************************************/
/*  Class:  TextureDX9
/*  Desc:   Direct3D9 texture class 
/*****************************************************************************/
class TextureDX9 : public ITexture, public IResource
{
    IDirect3DDevice9*           m_pDevice;          //  device interface		

    IDirect3DTexture9*          m_pTexture;	        //  1D/2D texture interface
    IDirect3DCubeTexture9*      m_pCubeTexture;	    //  the CubeMap texture interface
    IDirect3DVolumeTexture9*    m_pVolTexture;      //  volume texture interface
    IDirect3DSurface9*          m_pZBuffer;	        //  interface to Z-buffer surface, if texture used as depth buffer
    
    IDirect3DBaseTexture9*      m_pBaseTexture;	    //  general texture interface

    int                         m_ID;               //  id in the texture manager
    std::string                 m_Name;             //  texture (file) name
    std::string                 m_SearchPath;       //  suggested texture search path
    TextureType                 m_Type;             //  texture type
    int                         m_NMipMaps;         //  number of mip maps
    TextureMemoryPool           m_MemoryPool;       //  memory pool
    DepthStencilFormat          m_DSFormat;
    ColorFormat                 m_ColorFormat;      //  texture pixels color format
    int                         m_Width;            
    int                         m_Height;

    bool                        m_bRenderTarget;
    bool                        m_bDynamic;
    bool                        m_bNoTexture;

public:
                                TextureDX9      ( IDirect3DDevice9* pDevice );
                                TextureDX9      ();
                                ~TextureDX9     ();

    virtual const char*         GetName         () const { return m_Name.c_str(); }
    virtual const char*         GetSearchPath   () const { return m_SearchPath.c_str(); }
    virtual void                SetSearchPath   ( const char* path ) { m_SearchPath = path; }

    virtual TextureType         GetType         () const { return m_Type; }
    virtual int                 GetNMipMaps     () const { return m_NMipMaps; }
    virtual TextureMemoryPool   GetMemoryPool   () const { return m_MemoryPool; }
    virtual ColorFormat         GetColorFormat  ();
    virtual int                 GetWidth        () const { return m_Width; }
    virtual int                 GetHeight       () const { return m_Height; }

    virtual bool                HasAlpha        () const { return false; }
    virtual bool                IsRenderTarget  () const { return m_bRenderTarget; }
    virtual bool                IsDepthStencil  () const { return (m_DSFormat != dsfNone); }
    virtual int                 GetApproxSize   () const { return 0; }

    IDirect3DSurface9*          GetSurface      ( int idx = 0 );
    IDirect3DBaseTexture9*      GetTextureBase  () const { return m_pBaseTexture; } 
    IDirect3DTexture9*          GetTexture2D    () const { return m_pTexture; } 

    TextureMemoryPool           GetPool         () const { return m_MemoryPool; }
    int                         GetSize         () const;

    virtual void                SetID           ( int id ) { m_ID = id; }
    virtual int                 GetID           () const { return m_ID; }

    virtual BYTE*               LockBits        ( int& pitch, int level = 0 );
    virtual BYTE*               LockBits        ( int& pitch, const Rct& rect, int level = 0 ) const;
    virtual void                UnlockBits      ( int level = 0 ) const;

    virtual void                Bind            ( int stage = 0 );
    virtual bool                SaveToFile      ( const char* fName ) const;
    virtual bool                Reload          ();

    virtual bool                Create          ( int width, int height, 
                                                    ColorFormat clrFormat, 
                                                    int nMips = 0, 
                                                    TextureMemoryPool memPool = tmpManaged, 
                                                    bool bRenderTarget = false, 
                                                    DepthStencilFormat bDepthStencil = dsfNone,
                                                    bool bDynamic = false );
    virtual void                DeleteDeviceObjects     ();
    virtual void                InvalidateDeviceObjects ();
    virtual void                RestoreDeviceObjects    ();

    virtual void                SetName         ( const char* name ) { m_Name = name; }
    virtual void                LoadHeader      ();
    virtual bool                Load            (); 

private:
    bool                        Create          ();
    bool                        LoadFromFile    ( const char* fName );
    bool                        LoadFromMemory  ( const BYTE* pBuf, int nBytes );

}; // class TextureDX9

#endif // __D3DTEXTURE_H__