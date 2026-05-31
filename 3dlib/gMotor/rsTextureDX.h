/*****************************************************************/
/*  File:   rsTextureDX.h
/*  Desc:   Direct3D texture management 
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#ifndef __RSTEXTUREDX_H__
#define __RSTEXTUREDX_H__

#include "kHash.hpp"
/*****************************************************************************/
/*	Class:	CachedTexture
/*	Desc:	Texture	storage element
/*****************************************************************************/
class CachedTexture
{
public:
	bool					IsDestroyed				() const { return m_bDestroyed; }
	void					Reload					();
	bool					CreateFromFile			( bool forceFormat = false, 
													  BYTE* pMemFile = NULL, 
													  int memFileSize = 0 );

	bool					Create					( const TextureDescr* td = 0 );
	int						GetSizeBytes			() const;
	bool					InvalidateDeviceObjects	();
	bool					RestoreDeviceObjects	();
	void					CreateMipLevels			();
	DXSurface*				GetSurface				( int level );

protected:
							CachedTexture			( const char* texName );
							CachedTexture			();

	TextureDescr			m_Descr;
	DXTexture*				m_pTexture;		
	DXSurface*				m_pDepthStencil;
	bool					m_bDestroyed;
	char					m_Name[_MAX_PATH];

	friend class TextureManager;
}; // class CachedTexture

const int c_MaxTextures = 4096;
/*****************************************************************************/
/*	Class:	TextureManager
/*	Desc:	Texture storage & managament class
/*****************************************************************************/
class TextureManager
{
public:
						TextureManager		();
						~TextureManager		();
	
	void				Init				();
	void				Shut				();
	void				ReloadTextures		();
	void				ResetDeviceResources();
	void				SaveTexture			( int texID, const char* fname );
	bool				DeleteTexture		( int texID );
	void				CreateMipLevels		( int texID );
	void				LogStatus			();
	
	DXSurface*			GetTextureSurface	( int texID, int level = 0 );
	void				CopyTexture			( int destID, int srcID );
	void				CopyTexture			(	int destID, int srcID, 
													RECT* rectList, int nRect, 
													POINT* ptList );
	void				CopyTexture			( DXSurface* destSurf, int srcID, 
													RECT* rectList, int nRect, POINT* ptList = 0 );

	void				SetTexture			( int texID, int stage = 0 );
	int					GetTextureSizeBytes	( int texID );
	BYTE*				LockTexture			( int texID, int& pitch, int level = 0 );
	void				UnlockTexture		( int texID, int level = 0 );
	
	int					CreateTexture		( const char* texName, const TextureDescr& td );
	int					GetTextureID		( const char* texName, BYTE* pMemFile = NULL, int memFileSize = 0 );
	int					GetTextureID		( const char* texName, const TextureDescr& td, BYTE* pMemFile = NULL, int memFileSize = 0 );
	const char*			GetTextureName		( int texID );

	int					GetCurrentTexture	( int stage = 0 ) const;

	bool				InvalidateDeviceObjects();
	bool				RestoreDeviceObjects();

	int					FindTextureByName	( const char* texName );

	int					GetUsedTextureMemory();
	const TextureDescr* GetTextureDescr		( int texID );
    DXTexture*			GetDXTex			( int id ) { return m_Texture[id].m_pTexture; }


protected:
	int					m_CurTex[c_MaxTextureStages];

	CachedTexture		m_Texture[c_MaxTextures];
	int					m_NTextures;

private:

	void				ResetCurTextures	();
}; // class TextureManager

#define TC_CHK(A)	{ if ((A) != S_OK) LogStatus(); DX_CHK((A)); }

#endif // __RSTEXTUREDX_H__