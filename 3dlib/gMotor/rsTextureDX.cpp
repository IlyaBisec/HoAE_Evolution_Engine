/*****************************************************************/
/*  File:   rsTextureDX.cpp
/*  Desc:   Direct3D texture management classes inplementation
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#include "stdafx.h"
#include "rsDX.h"
#include "rsRenderSystemDX.h"
#include "rsTextureDX.h"

/*****************************************************************************/
/*	CachedTexture implementation
/*****************************************************************************/
CachedTexture::CachedTexture( const char* texName )
{
	strcpy( m_Name, texName );
	_strlwr( m_Name );
	m_pTexture		= NULL;
	m_pDepthStencil = NULL;
	m_bDestroyed	= false;
} // CachedTexture::CachedTexture

CachedTexture::CachedTexture()
{
	m_Name[0]		= 0;
	m_pTexture		= NULL;
	m_pDepthStencil = NULL;
	m_bDestroyed	= false;
} // CachedTexture::CachedTexture

void CachedTexture::Reload()
{
	if (m_bDestroyed) return;
	DXDevice* pDevice = D3DRenderSystem::instance().GetDevice();
	if (m_Descr.getTexUsage() == tuLoadable)
	{
		SAFE_RELEASE( m_pTexture );
		CreateFromFile( true );
	}
} // CachedTexture::Reload

bool CachedTexture::CreateFromFile( bool forceFormat, BYTE* pMemFile, int memFileSize )
{
    _chdir( GetRootDirectory() );
	
    DXDevice* pDevice = D3DRenderSystem::instance().GetDevice();
	InvalidateDeviceObjects();

	HRESULT hres = S_OK;

	if (!forceFormat)
	{
		if (pMemFile && memFileSize)
		{
			hres = 
				D3DXCreateTextureFromFileInMemoryEx(	pDevice, 
				    pMemFile,
				    memFileSize,
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
				    &m_pTexture				//  put here texture, please
				);
		}
		else
		{
			hres = 
				D3DXCreateTextureFromFileEx(	pDevice, 
				    m_Name,					//  file name
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
				    &m_pTexture				//  put here texture, please
				);
		}

		if (hres != S_OK || !m_pTexture)
		{
			///Log.Warning( "Couldn't load texture <%s>", m_Name );
			return false;
		}
	}
	else
	{
		UINT		width  = (m_Descr.getSideX() == -1) ? D3DX_DEFAULT : m_Descr.getSideX();
		UINT		height = (m_Descr.getSideY() == -1) ? D3DX_DEFAULT : m_Descr.getSideY();

		if (pMemFile && memFileSize)
		{
			hres = 
				D3DXCreateTextureFromFileInMemoryEx(	
				    pDevice, 
				    pMemFile,
				    memFileSize,
				    width,										//  width
				    height,										//  height
				    m_Descr.getNMips(),							//  number of mip levels
				    TexUsageG2DX( m_Descr.getTexUsage() ),		//  texture usage
				    ColorFormatG2DX( m_Descr.getColFmt() ),		//  color format
				    MemoryPoolG2DX( m_Descr.getMemPool() ),		//  memory pool
				    D3DX_DEFAULT,								//  image filter 
				    D3DX_DEFAULT,								//  mip filter
				    0,											//  disable color key
				    NULL,										//  don't care about image info
				    NULL,										//  no palette
				    &m_pTexture	
                );
		}
		else
		{
			hres = 
				D3DXCreateTextureFromFileEx(	
				    pDevice, 
				    m_Name,										//  file m_Name
				    width,										//  width
				    height,										//  height
				    m_Descr.getNMips(),							//  number of mip levels
				    TexUsageG2DX( m_Descr.getTexUsage() ),		//  texture usage
				    ColorFormatG2DX( m_Descr.getColFmt() ),		//  color format
				    MemoryPoolG2DX( m_Descr.getMemPool() ),		//  memory pool
				    D3DX_DEFAULT,								//  image filter 
				    D3DX_DEFAULT,								//  mip filter
				    0,											//  disable color key
				    NULL,										//  don't care about image info
				    NULL,										//  no palette
				    &m_pTexture	
                 );
		}


		DX_CHK( hres );
		if (hres != S_OK || !m_pTexture)
		{
			//Log.Error( "Couldn't load texture <%s>", m_Name );
			return false;
		}
	}

	//  get info into texture description
	D3DSURFACE_DESC sdesc;
	DX_CHK( m_pTexture->GetLevelDesc( 0, &sdesc ) );
	m_Descr.setSideX		( sdesc.Width  );
	m_Descr.setSideY		( sdesc.Height );
	m_Descr.setColFmt		( ColorFormatDX2G	( sdesc.Format ) );
	m_Descr.setMemPool	( MemoryPoolDX2G	( sdesc.Pool   ) );
	m_Descr.setNMips		( m_pTexture->GetLevelCount() );

	TextureUsage tu = TexUsageDX2G( sdesc.Usage );
	if (tu == tuUnknown) tu = tuLoadable;
	m_Descr.setTexUsage	( tu );
	return true;
} // CachedTexture::CreateFromFile

void CachedTexture::CreateMipLevels()
{
	D3DXFilterTexture( m_pTexture, NULL, D3DX_DEFAULT, D3DX_FILTER_BOX );
} // CachedTexture::CreateMipLevels

DXSurface* CachedTexture::GetSurface( int level )
{
	if (m_pDepthStencil) { return m_pDepthStencil; }
	if (!m_pTexture) return NULL;
	DXSurface* pSurface;
	m_pTexture->GetSurfaceLevel( level, &pSurface );
	return pSurface;
} // CachedTexture::GetSurface

/*---------------------------------------------------------------------------*/
/*	Func:	CachedTexture::Create	
/*	Desc:	Creates texture resource
/*---------------------------------------------------------------------------*/
bool CachedTexture::Create( const TextureDescr* td )
{
	DXDevice* pDevice = D3DRenderSystem::instance().GetDevice();

	if (td) m_Descr = *td;

	HRESULT result = S_OK;
	if (m_Descr.getTexUsage() == tuDepthStencil)
	{
		result = pDevice->CreateDepthStencilSurface( 
                            m_Descr.getSideX(), 
                            m_Descr.getSideY(), 
							DSFormatG2DX( m_Descr.getDsFmt() ), 
                            D3DMULTISAMPLE_NONE, &m_pDepthStencil 
                            );
	}
	else
	{
		result = D3DXCreateTexture( pDevice, m_Descr.getSideX(), m_Descr.getSideY(), m_Descr.getNMips(),
			                        TexUsageG2DX	( m_Descr.getTexUsage()	),			
			                        ColorFormatG2DX	( m_Descr.getColFmt()	), 
			                        MemoryPoolG2DX	( m_Descr.getMemPool()	), 
			                        &m_pTexture );
	}

	if (result == D3DERR_OUTOFVIDEOMEMORY)
	{
		Log.Warning( "Not enough video memory when creating texture: %s(%dx%d), mp:%d", 
			m_Name, m_Descr.getSideX(), m_Descr.getSideY(), m_Descr.getMemPool() );
		return false;
	}

	DX_CHK( result );
	return (result == S_OK);
} // CachedTexture::Create

bool CachedTexture::InvalidateDeviceObjects()
{
	if (m_bDestroyed) return false;
	FORCE_RELEASE( m_pDepthStencil );
	FORCE_RELEASE( m_pTexture );
	return true;
} // CachedTexture::InvalidateDeviceObjects

bool CachedTexture::RestoreDeviceObjects()
{
	DXDevice* pDevice = D3DRenderSystem::instance().GetDevice();
	if (m_bDestroyed) return false;
	assert( m_pTexture == NULL );
	bool res = false;

	if (m_Descr.getTexUsage() == tuLoadable)
	{
		res = CreateFromFile( true );
		if (!res)
		{
			Log.Error( "Failed to reload texture <%s>", m_Name );
		}
		return true;
	}

	res = Create( &m_Descr );

	if (!res)
	{
		Log.Info( "Failed to recreate texture <%s>", m_Name );
		return false;
	}
	return true;
} // CachedTexture::RestoreDeviceObjects

int	CachedTexture::GetSizeBytes() const
{
	if (m_bDestroyed) return 0;
	int res = sizeof( *this );
	if (!m_pTexture) return res;
	D3DSURFACE_DESC sdesc;
	for (int i = 0; i < m_Descr.getNMips(); i++)
	{
		DX_CHK( m_pTexture->GetLevelDesc( i, &sdesc ) );	
		res += sdesc.Size;
	}
	return res;
} // CachedTexture::GetSizeBytes

/*****************************************************************************/
/*	 TextureManager implementation
/*****************************************************************************/
TextureManager::TextureManager()
{
	m_NTextures = 0;
} // TextureManager::TextureManager

TextureManager::~TextureManager()
{

} // TextureManager::~TextureManager

void TextureManager::Init()
{
	m_NTextures = 0;
	//  NULL texture
	CachedTexture nullTD;
	nullTD.m_Descr.setValues( 0, 0, cfUnknown, mpUnknown, tuUnknown );
	strcpy( nullTD.m_Name, "NULL" );
	m_Texture[m_NTextures++] = nullTD;
	ResetCurTextures();
} // TextureManager::Init

void TextureManager::Shut()
{
	InvalidateDeviceObjects();
} // TextureManager::Shut

const char*	TextureManager::GetTextureName( int texID ) const
{
	if (texID < 0 || texID >= m_NTextures) return "";
	return m_Texture[texID].m_Name;
}

DXSurface* TextureManager::GetTextureSurface( int texID, int level )
{
	if (texID == -1 || texID == 0) return NULL;
	return m_Texture[texID].GetSurface( level );
} // TextureManager::GetTextureSurface

void TextureManager::CopyTexture( int destID, int srcID )
{
	DXDevice* pDevice = D3DRenderSystem::instance().GetDevice();
	DX_CHK( pDevice->UpdateTexture( GetDXTex( srcID ), GetDXTex( destID ) ) );
} // TextureManager::CopyTexture

void TextureManager::CopyTexture( int destID, int srcID, RECT* rectList, int nRect, POINT* ptList )
{
	DXDevice* pDevice = D3DRenderSystem::instance().GetDevice();
	DX_CHK( pDevice->CopyRects( GetTextureSurface( srcID ),  rectList, nRect, 
		GetTextureSurface( destID ), ptList ) );
} // TextureManager::CopyTexture

void TextureManager::CopyTexture( DXSurface* destSurf, int srcID, RECT* rectList, int nRect, POINT* ptList )
{
	DXDevice* pDevice = D3DRenderSystem::instance().GetDevice();
	assert( destSurf );
	DX_CHK( pDevice->CopyRects( GetTextureSurface( srcID ), rectList, nRect, 
		destSurf, ptList ) );
} // TextureManager::CopyTexture

void TextureManager::SetTexture( int texID, int stage )
{
	DXDevice* pDevice = D3DRenderSystem::instance().GetDevice();
	if (texID == m_CurTex[stage] || texID < 0 || texID >= m_NTextures) return;
	if (texID == 0)
	{
		pDevice->SetTexture( stage, NULL );
		m_CurTex[stage] = 0;
		return;
	}
	pDevice->SetTexture( stage, m_Texture[texID].m_pTexture );
	m_CurTex[stage] = texID;
	INC_COUNTER(TexSwitches,1);
} // TextureManager::SetTexture

int TextureManager::GetTextureSize( int texID ) const
{
	assert( texID >= 0 && texID < m_NTextures );
	DXDevice* pDevice = D3DRenderSystem::instance().GetDevice();
	return m_Texture[texID].GetSizeBytes();
} // TextureManager::GetTextureSize

BYTE* TextureManager::LockTexture( int texID, int& pitch, int level )
{
	if (texID == 0) 
	{
		pitch = 0;
		return NULL;
	}
	D3DLOCKED_RECT rct;
	DXTexture* dxTex = GetDXTex( texID );
	if (dxTex == NULL) return NULL;
	DX_CHK( dxTex->LockRect( level, &rct, 0, 0 ) );
	pitch = rct.Pitch;
	return (BYTE*)(rct.pBits);
} // TextureManager::LockTexture

void TextureManager::UnlockTexture( int texID, int level )
{
	if (texID == 0) return;
	DX_CHK( GetDXTex( texID )->UnlockRect( level ) );
} // TextureManager::UnlockTexture

int	TextureManager::FindTextureByName( const char* texName )
{
	for (int i = 0; i < m_NTextures; i++)
	{
		if (!stricmp( texName, m_Texture[i].m_Name)) return i;
	}
	return -1;
} // TextureManager::FindTextureByName

int TextureManager::GetTextureID( const char* texName, BYTE* pMemFile, int memFileSize )
{
	DXDevice* pDevice = D3DRenderSystem::instance().GetDevice();
	int idx = FindTextureByName( texName );
	if (idx == -1 || m_Texture[idx].IsDestroyed())
	{
		CachedTexture htex( texName );
		if (!htex.CreateFromFile( false, pMemFile, memFileSize ))
		{
			return -1;
		}
		htex.m_Descr.setID( m_NTextures );
		idx = m_NTextures;
		m_Texture[m_NTextures++] = htex;
	}
	return idx;
} // TextureManager::GetTextureID

int TextureManager::GetTextureID( const char* texName, const TextureDescr& td, BYTE* pMemFile, int memFileSize )
{
	DXDevice* pDevice = D3DRenderSystem::instance().GetDevice();

	int idx = FindTextureByName( texName );
	if (idx == NO_ELEMENT || m_Texture[idx].IsDestroyed())
	{
		bool ok = true;		
		CachedTexture htex( texName );
		htex.m_Descr = td;

		ok = htex.CreateFromFile( true, pMemFile, memFileSize );

		if (!ok)
		{
			LogStatus();
		}
		htex.m_Descr.setID( m_NTextures );
		idx = m_NTextures;
		m_Texture[m_NTextures++] = htex;
	}
	return idx;
} // TextureManager::GetTextureID

int TextureManager::GetCurrentTexture( int stage ) const
{
	assert( stage >= 0 && stage < c_MaxTextureStages );
	return m_CurTex[stage];
} // TextureManager::GetCurrentTexture

void TextureManager::ResetCurTextures()
{
	for (int i = 0; i < c_MaxTextureStages; i++) m_CurTex[i] = 0;
} // TextureManager::ResetCurTextures

void TextureManager::CreateMipLevels( int texID )
{
	if (texID == c_NoID) return;
	m_Texture[texID].CreateMipLevels();
} // TextureManager::CreateMipLevels

bool TextureManager::DeleteTexture( int texID )
{
	if (texID == -1) return false;
	CachedTexture& htex = m_Texture[texID];
	htex.InvalidateDeviceObjects();
	htex.m_bDestroyed = true;
	return true;
} // TextureManager::DeleteTexture

void TextureManager::SaveTexture( int texID, const char* fname )
{
	if (texID < 0) return;
	void ParseExtension( const char* fname, char* ext );
	char ext[64];
	ParseExtension( fname, ext );

	D3DXIMAGE_FILEFORMAT type = D3DXIFF_DDS;

	if (!strcmp( ext, "bmp" )) type = D3DXIFF_BMP;
	if (!strcmp( ext, "tga" )) type = D3DXIFF_TGA;
	if (!strcmp( ext, "dds" )) type = D3DXIFF_DDS;
	
	D3DXSaveTextureToFile( fname, type, GetDXTex( texID ), 0 );
} // TextureManager::SaveTexture

const TextureDescr* TextureManager::GetTextureDescr( int texID ) const
{
	if (texID < 0) return NULL;
	return &(m_Texture[texID].m_Descr);
} // TextureManager::GetTextureDescr

int	TextureManager::GetUsedTextureMemory()
{
	DXDevice* pDevice = D3DRenderSystem::instance().GetDevice();
	int res = 0;
	int nTex = m_NTextures;
	for (int i = 1; i < nTex; i++)
	{
		res += m_Texture[i].GetSizeBytes();
	}
	return res;
} // TextureManager::GetUsedTextureMemory

/*---------------------------------------------------------------------------*/
/*	Func:	TextureManager::ReloadTextures	
/*	Desc:	Reloads all loadable textures
/*---------------------------------------------------------------------------*/
void TextureManager::ReloadTextures()
{
	for (int stage = 0; stage < c_MaxTextureStages; stage++)
	{
		SetTexture( 0, stage );
	}

	int nTex = m_NTextures;
	for (int i = 1; i < nTex; i++)
	{
		CachedTexture&	tex		= m_Texture[i];
		TextureDescr&	m_Descr	= tex.m_Descr; 
		if (m_Descr.getTexUsage() == tuLoadable)
		{
			tex.Reload();
		}
	}
} // TextureManager::ReloadTextures

bool TextureManager::InvalidateDeviceObjects()
{
	for (int i = 1; i < m_NTextures; i++)
	{
		m_Texture[i].InvalidateDeviceObjects();
	}
	return true;
} // TextureManager::InvalidateDeviceObjects

bool TextureManager::RestoreDeviceObjects()
{
	_chdir( GetRootDirectory() );
	for (int i = 1; i < m_NTextures; i++)
	{
		m_Texture[i].RestoreDeviceObjects();
	}

	_chdir( GetRootDirectory() );
	return true;
} // TextureManager::RestoreDeviceObjects

int TextureManager::CreateTexture( const char* texName, const TextureDescr& td )
{
	CachedTexture htex( texName );
	if (!htex.Create( &td ))
	{
		LogStatus();
		return -1;
	}
	
	int id = 0;
	int i = 1;
	for (; i < m_NTextures; i++)
	{
		if (m_Texture[i].IsDestroyed())
		{
			m_Texture[i] = htex; id = i;
			m_Texture[i].m_bDestroyed = false;
			break;
		}
	}

	if (i == m_NTextures)
	{
		id = m_NTextures;
		m_Texture[m_NTextures++] = htex;
	}

	m_Texture[id].m_Descr.setID( id );
	return id;
} // TextureManager::CreateTexture

void TextureManager::LogStatus()
{
	int nTex = m_NTextures;
	Log.Info	( "Dumping texture cache..." );
	Log.Message	( "---------------------------------------------------------------------" );
	Log.Message	( "Total textures number: %d", nTex );
	Log.Message	( "---------------------------------------------------------------------" );
	int vPool = 0;
	int mPool = 0;
	int sPool = 0;
	int uPool = 0;

	for (int i = 1; i < nTex; i++)
	{
		TextureDescr& td = m_Texture[i].m_Descr;
		int sz			= m_Texture[i].GetSizeBytes();
		int bytesRem	= sz % 1024;

        char tname[_MAX_PATH];
        char fname[_MAX_PATH];

        if (m_Texture[i].m_Descr.getMemPool() == mpVRAM)
        {
            sprintf( tname, "_%03d_%s", i, m_Texture[i].m_Name );
        }
        else
        {
            sprintf( tname, "%03d_%s", i, m_Texture[i].m_Name );
        }
        int len = strlen( tname );
        for (int j = 0; j < len; j++) 
            if (tname[j] == '\\' || tname[j] == '/'|| tname[j] == ':') tname[j] = '_';
        sprintf( fname, "c:\\dumps\\textures\\%s.png", tname );
        SaveTexture( i, fname );
		switch ( td.getMemPool() )
		{
		case mpVRAM:	vPool += sz; break;
		case mpSysMem:	sPool += sz; break;
		case mpManaged: mPool += sz; break;
		default:		uPool += sz; 
		}
	}
	Log.Message( "Dump complete. Total allocated texture resources: %db", 
					vPool + sPool + mPool + uPool );
	Log.Message( "Default pool: %db. Managed pool: %db. Sysmem pool: %db.", 
					vPool, mPool, sPool );
	if (uPool > 0)
	{
		Log.Warning( "There are %d Bytes of texture memory allocated with incorrect pool type." );
	}
	Log.Message( "---------------------------------------------------------------------" );
} // TextureManager::LogStatus
	