/*****************************************************************
/*  File:   rsPictureManagerDX.cpp                                   
/*  Desc:   PictureInstance manager implementation
/*  Author: Silver, Copyright (C) GSC Game World                  
/*  Date:   Feb 2002                                             
/*****************************************************************/
#include "gRenderPch.h"

#include "ITexture.h"
#include "IPictureManager.h"
#include "IResourceManager.h"
#include "vMesh.h"
#include "d3dAdapt.h"
#include "kStaticArray.hpp"

IDirect3DDevice9* GetDirect3DDevice();
IDirect3DSurface9* GetDirect3DSurface( int texID );

/*****************************************************************/
/*	Class:	PictureInstance
/*	Desc:	Single picture file instance
/*****************************************************************/
class PictureInstance
{
public:
	struct Chunk
	{
		int				m_TexID;
		Rct				m_Extents;
		Rct				m_UV;
	}; // struct Chunk

protected:
	IDirect3DSurface9*	    m_pSurface;		
	std::vector<Chunk>	    m_Chunks;

	int					    m_Width;
	int					    m_Height;
	int					    m_NMipLevels;
	char				    m_FileName[_MAX_PATH];
	ColorFormat			    m_ColorFormat;
	PictureFileFormat	    m_FileFormat;
	int					    m_Stride;
	bool				    m_bValid;
    bool                    m_bLoaded;
	
	friend class		PictureManager;

public:
                PictureInstance	();
				~PictureInstance();

	bool		HasFileName		( const char* name ) { return (stricmp( m_FileName, name ) == 0); }
	bool		IsLoaded		() const { return m_bLoaded; }
	bool		IsValid			() const { return m_bValid; }

	void		Load			();
	void		Unload			();
	int			GetNChunks		() const { return m_Chunks.size(); }
	Chunk&		GetChunk		( int idx ) { return m_Chunks[idx]; }


	void		ClearChunks		();
	void		CreateChunks	();

}; // class PictureInstance

const int			c_ChunkSide			= 256;
const ColorFormat	c_DrawFormat		= cfARGB8888;
const int           c_MaxPictures       = 128;
/*****************************************************************/
/*	Class:	PictureManager
/*	Desc:	Implementation of the picture manager using D3DX
/*****************************************************************/
class PictureManager : public IPictureManager, public IDeviceClient
{
    static_array<PictureInstance, 
                 c_MaxPictures>         m_Pic;
	BaseMesh					        m_Primitive;
	DWORD						        m_Diffuse;
	bool						        m_bFiltering;
	Matrix4D					        m_TM;
	bool						        m_bTMEnabled;

public:
								PictureManager	();
	virtual int					GetImageID		( const char* fileName );
	virtual bool				LoadImage		( int imgID );
	virtual bool				UnloadImage 	( int imgID );
    virtual void                Purge           ();
	virtual BYTE*				GetPixels		( int imgID );
	virtual int					GetWidth		( int imgID );
	virtual int					GetHeight		( int imgID );
	virtual const char*			GetFileName		( int imgID );
	virtual bool				DrawImage		( int imgID, float x, float y, float z = 0.0f );
	virtual bool				DrawImage		( int imgID, const Rct& ext, float z = 0.0f );
	virtual bool				IsValid			( int imgID );
	virtual bool				IsLoaded		( int imgID );
	virtual void				SetDiffuse		( DWORD color ) { m_Diffuse = color; }
    virtual void				SetFiltering	( bool bFilter = true );

	virtual PictureFileFormat	GetFileFormat	( int imgID );
	virtual ColorFormat			GetColorFormat	( int imgID );

	virtual void				OnDestroyRS     ();
	virtual void				OnCreateRS      ();

	virtual void				SetTransform	( const Matrix4D& tm ) { m_TM = tm; }
	virtual const Matrix4D&		GetTransform	() const { return m_TM; }
	virtual void				EnableTransform	( bool bEnable = true ) { m_bTMEnabled = bEnable; }
	virtual bool				TransformEnabled() const { return m_bTMEnabled; }

}; // class PictureManager

IPictureManager* GetPictureManager()
{
	static PictureManager s_PictureManager;
	return &s_PictureManager;
}

DIALOGS_API IPictureManager* IPM = NULL;

void InitPictureManager()
{
	IPM = GetPictureManager();
}

/*****************************************************************/
/*	PictureInstance implementation
/*****************************************************************/
PictureInstance::PictureInstance()
{
    m_Width			= 0;
    m_Height		= 0;
    m_NMipLevels	= 0;
    m_FileName[0]	= 0;
    m_ColorFormat	= cfUnknown;
    m_FileFormat	= pfUnknown;
    m_Stride		= 0;
    m_bValid		= false;
    m_pSurface		= NULL;
    m_bLoaded       = false;
} // PictureInstance::PictureInstance

PictureInstance::~PictureInstance()
{
	Unload();
}

void PictureInstance::Load()
{
    if (IsLoaded()) return;
    int resID = IRM->FindResource( m_FileName );
    if (resID == -1) 
    {
        Log.Error( "Could not find picture resource: %s", m_FileName );
        return;
    }

    Unload();
    int size = 0;
    BYTE* buf = IRM->LockData( resID, size );
    if (!buf) return;    
    IDirect3DDevice9* pDevice = GetDirect3DDevice();
	DX_CHK( pDevice->CreateOffscreenPlainSurface( m_Width, m_Height, 
                                                    ConvertColorFormat( m_ColorFormat ), 
                                                    D3DPOOL_SCRATCH, 
                                                    &m_pSurface, NULL ) );
    D3DXIMAGE_INFO info; 
	HRESULT hRes = D3DXLoadSurfaceFromFileInMemory( m_pSurface, NULL, NULL, buf, size, NULL, D3DX_FILTER_NONE, 0, &info );
	if (hRes != S_OK)
	{
		Log.Error( "Could not load image from file %s", m_FileName );
	}

    IRM->UnlockData( resID );
   
	CreateChunks();
    m_bLoaded = true;

} // PictureInstance::Load

void PictureInstance::Unload()
{
	m_pSurface = NULL;
    m_bLoaded  = false;
	ClearChunks();
} // PictureInstance::Unload

void PictureInstance::ClearChunks()
{
	for (int i = 0; i < m_Chunks.size(); i++)
	{
		IRS->DeleteTexture( m_Chunks[i].m_TexID );
	}
	m_Chunks.clear();
} // PictureInstance::ClearChunks

const float c_TexelBias = 0.375f;
void PictureInstance::CreateChunks()
{
	if (m_Width == 0 || m_Height == 0) return;
	ClearChunks();
	
	int cx = 0; 
	int cy = 0;
	while (cy < m_Height)
	{
		cx = 0;
		while (cx < m_Width)
		{
			m_Chunks.push_back( Chunk() );
			Chunk& chunk = m_Chunks.back();
			char texName[_MAX_PATH];
			sprintf( texName, "%s_%d%d", m_FileName, cx/c_ChunkSide, cy/c_ChunkSide );
			chunk.m_TexID		= IRS->CreateTexture( texName, c_ChunkSide, c_ChunkSide, c_DrawFormat, 1, tmpManaged );
			chunk.m_Extents.x 	= cx + c_TexelBias;
			chunk.m_Extents.y 	= cy + c_TexelBias;
			
			float right  		= tmin( m_Width,  cx + c_ChunkSide );
			float bottom 		= tmin( m_Height, cy + c_ChunkSide );
			
			chunk.m_Extents.w 	= right - cx;
			chunk.m_Extents.h 	= bottom - cy;

			chunk.m_UV.x 		= 0.0f;
			chunk.m_UV.y 		= 0.0f;
			chunk.m_UV.w 		= (right  - cx)/c_ChunkSide;
			chunk.m_UV.h 		= (bottom - cy)/c_ChunkSide;

			IDirect3DSurface9* pChunkSurface = GetDirect3DSurface( chunk.m_TexID );
			RECT srcRect;
			srcRect.left		= cx;
			srcRect.top			= cy;
			srcRect.right		= right;
			srcRect.bottom  	= bottom;

			RECT dstRect;
			dstRect.left		= 0;
			dstRect.top			= 0;
			dstRect.right		= right - cx;
			dstRect.bottom  	= bottom - cy;

			DX_CHK( D3DXLoadSurfaceFromSurface( pChunkSurface, NULL, &dstRect, m_pSurface, NULL, &srcRect, D3DX_DEFAULT, 0 ) );
			pChunkSurface->Release();

			/*char path[_MAX_PATH];
			sprintf( path, "c:\\dumps\\p%d_%d.dds", cx/c_ChunkSide, cy/c_ChunkSide );
			IRS->SaveTexture( chunk.m_TexID, path );*/
			
			cx += c_ChunkSide;
		}
		cy += c_ChunkSide;
	}
    SAFE_RELEASE( m_pSurface );

} // PictureInstance::CreateChunks

/*****************************************************************/
/*	PictureManager implementation
/*****************************************************************/
PictureManager::PictureManager()
{
	m_Primitive.create	( 4, 6, vfVertexTnL, ptTriangleList );
	m_Primitive.setNVert( 4 );
	m_Primitive.setNPri	( 2 );
    m_Primitive.setNInd ( 6 );

    WORD* pIdx = m_Primitive.getIndices();
    pIdx[0] = 0; pIdx[1] = 1; pIdx[2] = 2;
    pIdx[3] = 2; pIdx[4] = 1; pIdx[5] = 3;

	m_Diffuse		= 0xFFFFFFFF;
	m_bFiltering	= false;
	m_TM			= Matrix4D::identity;
	m_bTMEnabled	= false;
} // PictureManager::PictureManager

int	PictureManager::GetImageID( const char* fileName )
{
	if (fileName[0] == 0) return -1;

    int resID = IRM->FindResource( fileName );
    if (resID == -1) 
    {
        Log.Error( "Could not find picture resource: %s", fileName );
        return -1;
    }
	
    int size  = 0;
    BYTE* buf = IRM->LockData( resID, size );
    if (!buf) return -1;

	for (int i = 0; i < m_Pic.size(); i++)
	{
		if (m_Pic[i].HasFileName( fileName )) { LoadImage( i ); return i; }
	}
	
	D3DXIMAGE_INFO info; 
	HRESULT hRes = D3DXGetImageInfoFromFileInMemory( buf, size, &info );
	if (hRes != S_OK)
	{
		Log.Error( "Could not load picture file %s.", fileName );
		return -1;
	}
    IRM->UnlockData( resID );
    
    if (m_Pic.size() == c_MaxPictures)
    {
        Log.Error( "PictureManager: max picture number reached." );
        return -1;
    }

	PictureInstance& pic = m_Pic.expand();
	strcpy( pic.m_FileName, fileName );
	pic.m_Width			= info.Width;
	pic.m_Height		= info.Height;
	pic.m_FileFormat	= (PictureFileFormat)info.ImageFileFormat;
	pic.m_NMipLevels	= info.MipLevels;
	pic.m_bValid		= true;
	pic.m_ColorFormat	= ConvertColorFormat( info.Format );
	return m_Pic.size() - 1;
} // PictureManager::GetImageID

bool PictureManager::LoadImage( int imgID )
{
	if (imgID < 0) return false;
	m_Pic[imgID].Load();
	return true;
}

bool PictureManager::UnloadImage( int imgID )
{
	if (imgID < 0) return false;
	m_Pic[imgID].Unload();
	return true;
}

void PictureManager::Purge()
{
    for (int i = 0; i < m_Pic.size(); i++)
    {
        m_Pic[i].Unload();
    }
} // PictureManager::Purge

BYTE* PictureManager::GetPixels( int imgID )
{
	return 0;
}

int PictureManager::GetWidth( int imgID )
{
	if (imgID < 0) return 0;
	return m_Pic[imgID].m_Width;
}

int	PictureManager::GetHeight( int imgID )
{
	if (imgID < 0) return 0;
	return m_Pic[imgID].m_Height;
}

const char*	PictureManager::GetFileName( int imgID )
{
	if (imgID < 0) return 0;
	return m_Pic[imgID].m_FileName;
}

bool PictureManager::DrawImage( int imgID, float x, float y, float z )
{
	if (imgID < 0) return false;
	PictureInstance& pic = m_Pic[imgID];
	return DrawImage( imgID, Rct( x, y, pic.m_Width, pic.m_Height ), z );
} // PictureManager::DrawImage

bool PictureManager::DrawImage( int imgID, const Rct& ext, float z )
{
	if (imgID < 0) return false;
	PictureInstance& pic = m_Pic[imgID];
	if (!pic.IsLoaded()) pic.Load();
	float wScale = ext.w / float( pic.m_Width  );
	float hScale = ext.h / float( pic.m_Height );
	
 
	static int shID = IRS->GetShaderID( "hud" );
	static int shID_L = IRS->GetShaderID( "hud_L" );

	if (m_bFiltering) m_Primitive.setShader( shID_L );
	else m_Primitive.setShader( shID );

	int nChunks = pic.GetNChunks();
	for (int i = 0; i < nChunks; i++)
	{
		PictureInstance::Chunk& chunk = pic.GetChunk( i );
		VertexTnL* v = (VertexTnL*)m_Primitive.getVertexData();
		v[0].x 			= chunk.m_Extents.x*wScale + ext.x;
		v[0].y 			= chunk.m_Extents.y*hScale + ext.y;
		v[0].u 			= chunk.m_UV.x;
		v[0].v 			= chunk.m_UV.y;
		v[0].z 			= z;
		v[0].w 			= 1.0f;
		v[0].diffuse	= m_Diffuse;

		v[1].x 			= chunk.m_Extents.GetRight()*wScale + ext.x;
		v[1].y 			= chunk.m_Extents.y*hScale + ext.y;
		v[1].u 			= chunk.m_UV.GetRight();
		v[1].v 			= chunk.m_UV.y;
		v[1].z 			= z;
		v[1].w 			= 1.0f;
		v[1].diffuse	= m_Diffuse;

		v[2].x 			= chunk.m_Extents.x*wScale + ext.x;
		v[2].y 			= chunk.m_Extents.GetBottom()*hScale + ext.y;
		v[2].u 			= chunk.m_UV.x;
		v[2].v 			= chunk.m_UV.GetBottom();
		v[2].z 			= z;
		v[2].w 			= 1.0f;
		v[2].diffuse	= m_Diffuse;

		v[3].x 			= chunk.m_Extents.GetRight()*wScale + ext.x;
		v[3].y 			= chunk.m_Extents.GetBottom()*hScale + ext.y;
		v[3].u 			= chunk.m_UV.GetRight();
		v[3].v 			= chunk.m_UV.GetBottom();
		v[3].z 			= z;
		v[3].w 			= 1.0f;
		v[3].diffuse	= m_Diffuse;
		
		if (m_bTMEnabled)
		{
			for (int j = 0; j < 4; j++)
			{
				Vector3D cv( v[j].x, v[j].y, v[j].z );
				m_TM.transformPt( cv );
				v[j].x = cv.x;
				v[j].y = cv.y;
				v[j].z = cv.z;
			}
		}

		m_Primitive.setTexture( chunk.m_TexID );
		DrawBM( m_Primitive );
	}
	return false;
} // PictureManager::DrawImage

bool PictureManager::IsValid( int imgID )
{
	if (imgID < 0) return 0;
	return m_Pic[imgID].IsValid();
}

PictureFileFormat PictureManager::GetFileFormat( int imgID )
{
	if (imgID < 0) return pfUnknown;
	return m_Pic[imgID].m_FileFormat;
}

ColorFormat PictureManager::GetColorFormat( int imgID )
{
	if (imgID < 0) return cfUnknown;
	return m_Pic[imgID].m_ColorFormat;
}

bool PictureManager::IsLoaded( int imgID )
{
	if (imgID < 0) return false;
	return m_Pic[imgID].IsLoaded();	
}

void PictureManager::SetFiltering( bool bFilter )
{
	m_bFiltering = bFilter;
}

void PictureManager::OnDestroyRS()
{
	for (int i = 0; i < m_Pic.size(); i++) m_Pic[i].Unload();
}

void PictureManager::OnCreateRS()
{
}



