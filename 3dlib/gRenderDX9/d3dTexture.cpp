/*****************************************************************************/
/*	File:	d3dTexture.cpp
/*  Desc:	Texture interface implementation for DirectX9 
/*	Author:	Ruslan Shestopalyuk
/*	Date:	02.11.2004
/*****************************************************************************/
#include "gRenderPch.h"
#include "d3dTexture.h"
#include "d3dAdapt.h"
#include "direct.h"
extern int OtherTime;
/*****************************************************************************/
/*  TextureDX9 implementation
/*****************************************************************************/
TextureDX9::TextureDX9( IDirect3DDevice9* pDevice )
{
    m_pDevice           = pDevice;       
    m_pTexture          = NULL;
    m_pCubeTexture      = NULL;	 
    m_pVolTexture       = NULL;   
    m_pZBuffer          = NULL;	     

    m_pBaseTexture      = NULL;	 

    m_Name              = "";
    m_Type              = tt2D;
    m_NMipMaps          = 0;
    m_MemoryPool        = tmpManaged;
    m_ColorFormat       = cfUnknown;
    m_Width             = 0;
    m_Height            = 0;

    m_bRenderTarget     = false;
    m_DSFormat          = dsfNone;
    m_bNoTexture        = false;

} // TextureDX9::TextureDX9

TextureDX9::TextureDX9()
{
    m_pDevice           = NULL;       
    m_pTexture          = NULL;
    m_pCubeTexture      = NULL;	 
    m_pVolTexture       = NULL;   
    m_pZBuffer          = NULL;	     

    m_pBaseTexture      = NULL;	 

    m_Name              = "";
    m_Type              = tt2D;
    m_NMipMaps          = 0;
    m_MemoryPool        = tmpManaged;
    m_ColorFormat       = cfUnknown;
    m_Width             = 0;
    m_Height            = 0;

    m_bRenderTarget     = false;
    m_DSFormat          = dsfNone;
    m_bNoTexture        = false;
} // TextureDX9::TextureDX9

TextureDX9::~TextureDX9()
{
} // TextureDX9::~TextureDX9

void TextureDX9::InvalidateDeviceObjects()
{
    if (m_MemoryPool != tmpManaged)
    {
        SAFE_RELEASE( m_pTexture        );
        SAFE_RELEASE( m_pCubeTexture    );
        SAFE_RELEASE( m_pVolTexture     );
        SAFE_RELEASE( m_pZBuffer        );

        m_pTexture       = NULL;
        m_pCubeTexture   = NULL;	
        m_pVolTexture    = NULL; 
        m_pZBuffer       = NULL;	

        m_pBaseTexture   = NULL;	
    }
} // TextureDX9::InvalidateDeviceObjects

void TextureDX9::RestoreDeviceObjects() 
{
    if (m_MemoryPool != tmpManaged) Create();
} // TextureDX9::RestoreDeviceObjects

IDirect3DSurface9* TextureDX9::GetSurface( int idx )
{
    if (m_pZBuffer) return m_pZBuffer;
    IDirect3DSurface9* pSurf = NULL;
    if (m_pTexture)
    {
        int nRef = GetRefCount( m_pTexture );
        DX_CHK( m_pTexture->GetSurfaceLevel( idx, &pSurf ) );
    }
    else if (m_pCubeTexture)
    {
        DX_CHK( m_pCubeTexture->GetCubeMapSurface( (D3DCUBEMAP_FACES)(idx%6), idx/6, &pSurf ) );
    }
    return pSurf;
} // TextureDX9::GetSurface

void TextureDX9::DeleteDeviceObjects()
{
    SAFE_RELEASE( m_pTexture        );
    SAFE_RELEASE( m_pCubeTexture    );
    SAFE_RELEASE( m_pVolTexture     );
    SAFE_RELEASE( m_pZBuffer        );

    m_pTexture       = NULL;
    m_pCubeTexture   = NULL;	
    m_pVolTexture    = NULL; 
    m_pZBuffer       = NULL;	

    m_pBaseTexture   = NULL;	
} // TextureDX9::ReleaseDeviceResources
int SetTextureTime=0;
void TextureDX9::Bind( int stage )
{
    if (!m_pBaseTexture)
    {
        Load();
    }
    __beginT();
    DX_CHK( m_pDevice->SetTexture( stage, m_pBaseTexture ) );
    __endT(SetTextureTime);
} // TextureDX9::Bind

bool TextureDX9::SaveToFile( const char* fName ) const
{
    if (!m_pTexture) return false;
    void ParseExtension( const char* fname, char* ext );
    char ext[64];
    ParseExtension( fName, ext );

    D3DXIMAGE_FILEFORMAT type = D3DXIFF_DDS;

    if (!strcmp( ext, "bmp" )) type = D3DXIFF_BMP;
    if (!strcmp( ext, "tga" )) type = D3DXIFF_TGA;
    if (!strcmp( ext, "dds" )) type = D3DXIFF_DDS;

    HRESULT hRes = D3DXSaveTextureToFile( fName, type, m_pBaseTexture, 0 );
    return (hRes == S_OK);
} // TextureDX9::SaveToFile

BYTE* TextureDX9::LockBits( int& pitch, int level )
{
	if (!m_pBaseTexture)
	{
		Load();
	}
    if (!m_pTexture) return NULL;
	
	// TestCooperativeLevel
//	if(!m_pDevice || FAILED(m_pDevice->TestCooperativeLevel())) {
//		return NULL;
//	}


    D3DLOCKED_RECT rct;
    DX_CHK( m_pTexture->LockRect( level, &rct, 0, 0 ) );
    pitch = rct.Pitch;
    return (BYTE*)(rct.pBits);
} // TextureDX9::LockBits

BYTE* TextureDX9::LockBits( int& pitch, const Rct& rect, int level ) const
{
    if (!m_pTexture) return NULL;
    RECT rc;
    rc.left     = rect.x;
    rc.top      = rect.y;
    rc.right    = rect.GetRight();
    rc.bottom   = rect.GetBottom();

    D3DLOCKED_RECT d3dRect;
    __beginT();
    DX_CHK( m_pTexture->LockRect( level, &d3dRect, &rc, 0 ) );
    __endT(OtherTime);
    pitch = d3dRect.Pitch;
    return (BYTE*)(d3dRect.pBits);
} // TextureDX9::LockBits

void TextureDX9::UnlockBits( int level ) const
{
    if (!m_pTexture) return;
    __beginT();
    m_pTexture->UnlockRect( level );
    __endT(OtherTime);
} // TextureDX9::UnlockBits

bool TextureDX9::Create(    int                 width, 
                            int                 height, 
                            ColorFormat         clrFormat, 
                            int                 nMips, 
                            TextureMemoryPool   memPool, 
                            bool                bRenderTarget, 
                            DepthStencilFormat  dsFormat, 
                            bool                bDynamic )
{
    m_Type          = tt2D;          
    m_NMipMaps      = nMips;      
    m_MemoryPool    = memPool;    
    m_ColorFormat   = clrFormat;   
    m_Width         = width;         
    m_Height        = height;

    m_bRenderTarget = bRenderTarget;
    m_DSFormat      = dsFormat;
    m_bDynamic      = bDynamic;
    return Create();
} // TextureDX9::Create

bool TextureDX9::Create()
{
    DeleteDeviceObjects();
    if (!m_pDevice) return false;
    HRESULT result = S_OK;
    if (m_DSFormat != dsfNone)
    {
        result = m_pDevice->CreateDepthStencilSurface(  m_Width, m_Height, 
                                ConvertDepthStencilFormat( m_DSFormat ), 
                                D3DMULTISAMPLE_NONE, 0, TRUE, &m_pZBuffer, NULL );
    }
    else
    {
        DWORD usage = 0;
        if (m_bRenderTarget) 
        {
            usage |= D3DUSAGE_RENDERTARGET;
        }
        if (m_bDynamic) usage |= D3DUSAGE_DYNAMIC;
        result = D3DXCreateTexture( m_pDevice, m_Width, m_Height, m_NMipMaps, usage,			
                                    ConvertColorFormat( m_ColorFormat ), 
                                    ConvertMemoryPool( m_MemoryPool	), 
                                    &m_pTexture );
    }

    if (result == D3DERR_OUTOFVIDEOMEMORY)
    {
        Log.Error( "Not enough video memory to create texture: %s(%dx%d)", 
                        m_Name.c_str(), m_Width, m_Height );
        return false;
    }
    if (!m_pTexture && !m_pZBuffer) return -1;

    m_pBaseTexture  = m_pTexture;

    //  declare whole texture as 'dirty'
    RECT rc;
    rc.left   = 0;
    rc.top    = 0;
    rc.right  = m_Width;
    rc.bottom = m_Height;
    if (m_pTexture) m_pTexture->AddDirtyRect( &rc );

    DX_CHK( result );
    return (result == S_OK);
} // TextureDX9::Create

bool TextureDX9::Reload() 
{
    DeleteDeviceObjects();
    return Load();
} // TextureDX9::Reload

ColorFormat TextureDX9::GetColorFormat() 
{ 
    if (m_pBaseTexture) Load(); 
    return m_ColorFormat; 
} // TextureDX9::GetColorFormat

bool TextureDX9::Load() 
{
    if (m_bNoTexture) return false;
    FilePath path;
    path.GetCWD();
    _chdir( m_SearchPath.c_str() );
    int size = 0;
    FilePath tpath( m_Name.c_str() );
    int resID = IRM->FindResource( tpath.GetFullPath() );
    if (resID == -1) 
    {
        tpath.SetExt( "dds" );
        resID = IRM->FindResource( tpath.GetFullPath() );
    }
    if (resID == -1) 
    {
        tpath.SetExt( "tga" );
        resID = IRM->FindResource( tpath.GetFullPath() );
    }
    if (resID == -1) 
    {
        tpath.SetExt( "jpg" );
        resID = IRM->FindResource( tpath.GetFullPath() );
    }
    if (resID == -1) 
    {
        tpath.SetExt( "bmp" );
        resID = IRM->FindResource( tpath.GetFullPath() );
    }
    if (resID == -1) 
    {
        Log.Warning( "Could not load texture <%s>", tpath.GetFullPath() );
        m_bNoTexture = true;
        return false; 
    }

    IRM->BindResource( resID, this );
    BYTE* pData = IRM->LockData( resID, size );
    bool res = LoadFromMemory( pData, size );
    IRM->UnlockData( resID );
    path.SetCWD();
    return res;
} // TextureDX9::Load

bool TextureDX9::LoadFromFile( const char* fName )
{
    FilePath path;
    path.GetCWD();
    _chdir( m_SearchPath.c_str() );

    HRESULT hres = S_OK;

    D3DXIMAGE_INFO info;
    hres = D3DXGetImageInfoFromFile( fName, &info );

    if (info.ResourceType == D3DRTYPE_TEXTURE)
    {
        m_Type = tt2D;
        hres = D3DXCreateTextureFromFileEx(	m_pDevice, 
                                            fName,					//  file name
                                            D3DX_DEFAULT,			//  width
                                            D3DX_DEFAULT,			//  height
                                            D3DX_DEFAULT,			//  number of mip levels
                                            0,						//  texture usage
                                            D3DFMT_UNKNOWN,			//  color format
                                            D3DPOOL_MANAGED,		//  memory pool
                                            D3DX_DEFAULT,			//  image filter 
                                            D3DX_DEFAULT,			//  mip filter
                                            0,						//  disable color key
                                            NULL,					//  don't care about image info
                                            NULL,					//  no palette
                                            &m_pTexture				
                                        );
        m_pBaseTexture  = m_pTexture;
    }
    else if (info.ResourceType == D3DRTYPE_VOLUMETEXTURE)
    {
         m_Type = tt3D;
         hres = D3DXCreateVolumeTextureFromFileEx(	m_pDevice, 
                                                    fName,					//  file name
                                                    D3DX_DEFAULT,			//  width
                                                    D3DX_DEFAULT,			//  height
                                                    D3DX_DEFAULT,			//  depth
                                                    D3DX_DEFAULT,			//  number of mip levels
                                                    0,						//  texture usage
                                                    D3DFMT_UNKNOWN,			//  color format
                                                    D3DPOOL_MANAGED,		//  memory pool
                                                    D3DX_DEFAULT,			//  image filter 
                                                    D3DX_DEFAULT,			//  mip filter
                                                    0,						//  disable color key
                                                    NULL,					//  don't care about image info
                                                    NULL,					//  no palette
                                                    &m_pVolTexture				
                                                    );
         m_pBaseTexture  = m_pVolTexture;
    }
    else if (info.ResourceType == D3DRTYPE_CUBETEXTURE)
    {
        m_Type = ttCubeMap;
        hres = D3DXCreateCubeTextureFromFileEx(	m_pDevice, 
                                                fName,					//  file name
                                                D3DX_DEFAULT,			//  width
                                                D3DX_DEFAULT,			//  number of mip levels
                                                0,						//  texture usage
                                                D3DFMT_UNKNOWN,			//  color format
                                                D3DPOOL_MANAGED,		//  memory pool
                                                D3DX_DEFAULT,			//  image filter 
                                                D3DX_DEFAULT,			//  mip filter
                                                0,						//  disable color key
                                                NULL,					//  don't care about image info
                                                NULL,					//  no palette
                                                &m_pCubeTexture				
                                                );
        m_pBaseTexture  = m_pCubeTexture;	 
    }

    path.SetCWD();

    if (hres != S_OK || !m_pTexture)
    {
        Log.Warning( "Couldn't load texture <%s>", fName );
        return false;
    }
    
    //  get info into texture description
    D3DSURFACE_DESC sdesc;
    DX_CHK( m_pTexture->GetLevelDesc( 0, &sdesc ) );
    
    m_Type          = tt2D;
    m_NMipMaps      = m_pTexture->GetLevelCount();
    m_MemoryPool    = ConvertMemoryPool( sdesc.Pool );
    m_ColorFormat   = ConvertColorFormat( sdesc.Format );
    m_Width         = sdesc.Width;
    m_Height        = sdesc.Height;
    m_DSFormat      = dsfNone;
    m_bRenderTarget = false;
    
    return true;
} // TextureDX9::LoadFromFile

bool TextureDX9::LoadFromMemory( const BYTE* pBuf, int nBytes )
{
    HRESULT hres = S_OK;
    D3DXIMAGE_INFO info;
    hres = D3DXGetImageInfoFromFileInMemory( pBuf, nBytes, &info );
	D3DXIMAGE_INFO inf;

    if (info.ResourceType == D3DRTYPE_TEXTURE)
    {
        m_Type = tt2D;
        hres = D3DXCreateTextureFromFileInMemoryEx(	m_pDevice, 
                                                    pBuf, nBytes,
                                                    D3DX_DEFAULT,			//  width
                                                    D3DX_DEFAULT,			//  height
                                                    D3DX_DEFAULT,			//  number of mip levels
                                                    0,						//  texture usage
                                                    D3DFMT_UNKNOWN,			//  color format
                                                    D3DPOOL_MANAGED,		//  memory pool
                                                    D3DX_DEFAULT,			//  image filter 
                                                    D3DX_DEFAULT,			//  mip filter
                                                    0,						//  disable color key
                                                    &inf,					//  don't care about image info
                                                    NULL,					//  no palette
                                                    &m_pTexture				
                                                    );
        m_pBaseTexture  = m_pTexture;
    }
    else if (info.ResourceType == D3DRTYPE_VOLUMETEXTURE)
    {
        m_Type = tt3D;
        hres = D3DXCreateVolumeTextureFromFileInMemoryEx(	m_pDevice, 
                                                            pBuf, nBytes,
                                                            D3DX_DEFAULT,			//  width
                                                            D3DX_DEFAULT,			//  height
                                                            D3DX_DEFAULT,			//  depth
                                                            D3DX_DEFAULT,			//  number of mip levels
                                                            0,						//  texture usage
                                                            D3DFMT_UNKNOWN,			//  color format
                                                            D3DPOOL_MANAGED,		//  memory pool
                                                            D3DX_DEFAULT,			//  image filter 
                                                            D3DX_DEFAULT,			//  mip filter
                                                            0,						//  disable color key
                                                            &inf,					//  don't care about image info
                                                            NULL,					//  no palette
                                                            &m_pVolTexture				
                                                            );
        m_pBaseTexture  = m_pVolTexture;
    }
    else if (info.ResourceType == D3DRTYPE_CUBETEXTURE)
    {
        m_Type = ttCubeMap;
        hres = D3DXCreateCubeTextureFromFileInMemoryEx(	m_pDevice, 
                                                        pBuf, nBytes,
                                                        D3DX_DEFAULT,			//  width
                                                        D3DX_DEFAULT,			//  number of mip levels
                                                        0,						//  texture usage
                                                        D3DFMT_UNKNOWN,			//  color format
                                                        D3DPOOL_MANAGED,		//  memory pool
                                                        D3DX_DEFAULT,			//  image filter 
                                                        D3DX_DEFAULT,			//  mip filter
                                                        0,						//  disable color key
                                                        &inf,					//  don't care about image info
                                                        NULL,					//  no palette
                                                        &m_pCubeTexture				
                                                        );
        m_pBaseTexture  = m_pCubeTexture;
    }


    if (hres != S_OK || !m_pBaseTexture)
    {
        Log.Warning( "Couldn't create texture from memory data: %s", m_Name.c_str() );
        return false;
    }

    //  get info into texture description
    if (m_pTexture) 
    {
        D3DSURFACE_DESC sdesc;
        DX_CHK( m_pTexture->GetLevelDesc( 0, &sdesc ) );
        m_Type          = tt2D;
        m_NMipMaps      = m_pTexture->GetLevelCount();
        m_MemoryPool    = ConvertMemoryPool( sdesc.Pool );
        m_ColorFormat   = ConvertColorFormat( sdesc.Format );
        m_Width         = sdesc.Width;//inf.Width;
        m_Height        = sdesc.Height;//inf.Height;
        bool goodX = false;
        bool goodY = false;
        for(int i=0;i<12;i++){
            int sz=1<<i;
            if(m_Width==sz)goodX = true;
            if(m_Height==sz)goodY = true;
        }
        if(!(goodX && goodY) ){
            Log.Warning("Texture <%s> has an incorrect size: %dx%d",m_Name.c_str(),m_Width,m_Height);
        }
    }
    else if (m_pCubeTexture) 
    {
        D3DSURFACE_DESC sdesc;
        DX_CHK( m_pCubeTexture->GetLevelDesc(  0, &sdesc ) );
        m_Type          = ttCubeMap;
        m_NMipMaps      = m_pCubeTexture->GetLevelCount();
        m_MemoryPool    = ConvertMemoryPool( sdesc.Pool );
        m_ColorFormat   = ConvertColorFormat( sdesc.Format );
        m_Width         = sdesc.Width;
        m_Height        = sdesc.Height;
    }
    else if (m_pVolTexture) 
    {
        D3DVOLUME_DESC vdesc;
        DX_CHK( m_pVolTexture->GetLevelDesc( 0, &vdesc ) );
        m_Type          = tt3D;
        m_NMipMaps      = m_pVolTexture->GetLevelCount();
        m_MemoryPool    = ConvertMemoryPool( vdesc.Pool );
        m_ColorFormat   = ConvertColorFormat( vdesc.Format );
        m_Width         = vdesc.Width;
        m_Height        = vdesc.Height;
    }
    else return false;

    m_DSFormat      = dsfNone;
    m_bRenderTarget = false;

    return true;
} // TextureDX9::LoadFromMemory

void TextureDX9::LoadHeader()
{
    _chdir( m_SearchPath.c_str() );
    int size = 0;
    int resID = IRM->FindResource( m_Name.c_str() );
    IRM->BindResource( resID, this );
    BYTE* pData = IRM->LockData( resID, size );

    if (resID != -1) 
    {
        FilePath path( IRM->GetFullPath( resID ) );
        path.SetFileName( "" );
        path.SetExt( "" );
        m_SearchPath = path;
    }

    HRESULT hres = S_OK;
    D3DXIMAGE_INFO info;
    hres = D3DXGetImageInfoFromFileInMemory( pData, size, &info );
    if (hres == S_OK)
    {
        m_Width     = info.Width;
        m_Height    = info.Height;
    }

    IRM->UnlockData( resID );
    _chdir( IRM->GetHomeDirectory() );
} // TextureDX9::LoadHeader

int TextureDX9::GetSize() const
{
    if (!m_pBaseTexture) return 0;
    int nPix = m_Width*m_Height;
    return ColorValue::GetBitmapSize( m_ColorFormat, nPix ); 
} // TextureDX9::GetSize