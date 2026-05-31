/*****************************************************************
/*  File:   rsRenderSystemDX.h                                   
/*  Desc:   Direct3D rendering system                             
/*  Author: Silver, Copyright (C) GSC Game World                  
/*  Date:   Feb 2002                                             
/*****************************************************************/
#ifndef __D3DRENDERSYSTEM_H__
#define __D3DRENDERSYSTEM_H__

#include "IRenderSystem.h"
#include "rsSettings.h"

const int c_MaxAdapters				= 3;
const int c_MaxAdapterDevices		= 5;
const int c_MaxDeviceDisplayModes	= 150;
const int c_MaxDisplayFormatsInMode	= 10;

const int c_MaxLights				= 8;

/*****************************************************************************/
/*	Class:	D3DModeInfo
/*	Desc:	Direct3D display mode description
/*****************************************************************************/
class D3DModeInfo
{
public:
	void			Dump( FILE* fp );

protected:
	int			width;
	int			height;
	int			refreshRate;
	D3DFORMAT	format;
	DWORD		behavior;
	D3DFORMAT	depthStencil;

private:
	friend class D3DRenderSystem;
	friend class D3DDeviceInfo;
}; // class D3DModeInfo

/*****************************************************************************/
/*	Class:	D3DDeviceInfo
/*	Desc:	Direct3D device description
/*****************************************************************************/
class D3DDeviceInfo
{
public:
	D3DDeviceInfo() : nModes(0) {}

	void			Dump( FILE* fp );
	D3DModeInfo*	FindMode( int width, int height );

protected:
	D3DDEVTYPE		devType;
    DXCaps			caps;
	const char*		strDesc;
	bool			canDoWindowed;

    int				nModes;
	D3DModeInfo		modes[c_MaxDeviceDisplayModes];

private:
	friend class D3DRenderSystem;
	friend class D3DAdapterInfo;
}; // class D3DDeviceInfo

/*****************************************************************************/
/*	Class:	D3DAdapterInfo
/*	Desc:	Direct3D adapter info
/*****************************************************************************/
class D3DAdapterInfo
{
public:
	D3DAdapterInfo() : nDevices(0) {}
	
	void			            Dump        ( FILE* fp );
	D3DDeviceInfo*	            FindDevice  ( D3DDEVTYPE devType );

protected:
    D3DADAPTER_IDENTIFIER8		adapterID;
    D3DDISPLAYMODE				desktopDisplayMode;
    int							nDevices;
    D3DDeviceInfo				devices[c_MaxAdapterDevices];

private:
	friend class D3DRenderSystem;
}; // class D3DAdapterInfo

class IniFile;
class Shader;
class Input;

/*****************************************************************
/*	Class:	D3DRenderSystem
/*	Desc:	Direct3D rendering system
/*****************************************************************/
class D3DRenderSystem : public Singleton<D3DRenderSystem>, public IRenderSystem
{
public:
						D3DRenderSystem	();
						~D3DRenderSystem();

	void 				Init			( HINSTANCE hInst, HWND hWnd );
	void 				ShutDown		();
	
    void 				SetViewTM	    ( const Matrix4D& vmatr );
	void 				SetProjTM       ( const Matrix4D& pmatr );
	void 				SetWorldTM	    ( const Matrix4D& wmatr );

    const Matrix4D&     GetViewTM       () const { return m_ViewMatrix;         }
    const Matrix4D&     GetProjTM       () const { return m_ProjectionMatrix;   }
    const Matrix4D&     GetWorldTM      () const { return m_WorldMatrix;        } 

	void				ResetWorldTM    ();
	void 				PushWorldMatrix	( const Matrix4D& wmatr );
	const Matrix4D&		PopWorldMatrix	();

	void				SetTextureTM    ( const Matrix4D& tmatr, int stage = 0 );
	void				SetBumpTM	    ( const Matrix3D& bmatr, int stage = 0 );

    const Matrix4D&     GetTextureTM    ( int stage = 0 ) const;  
    const Matrix3D&     GetBumpTM       ( int stage = 0 ) const;

    void  			    ClearDevice		( DWORD color = 0, bool bColor = true, bool bDepth = false, bool bStencil = false );
    void				AddClient       ( IDeviceClient* iNotify );

	void  				Draw			( BaseMesh& bm );
	void  				DrawPrim		( BaseMesh& bm );

	bool				SetCursor		( int texID, const Rct& rctOnTex, int hotspotX = 0, int hotspotY = 0 );
	bool				UpdateCursor	( int x, int y, bool drawNow = false );
	void				ShowCursor		( bool bShow = true );

	void 				SetTexture		( int texID, int stage = 0 );
	int 				GetTexture		( int stage = 0 );

	void 				SaveTexture		( int texID, const char* fname	);
	bool 				DeleteTexture	( int texID );
	BYTE* 				LockTexBits		( int texID, int& pitch, int level = 0	);
	void  				UnlockTexBits	( int texID, int level = 0				);
	
	void 				SetViewPort		( const Rct& vp );
	Rct					GetViewPort		() const;
	void				SetViewPort		( float x, float y, float w, float h, float zn = 0.0f, float zf = 1.0f );

	bool 				SetRenderTarget	( int texID, int dsID = 0 );

	void*				DbgGetDevice	();
	void				OnFrame			();

	void 				Dump			( const char* fname = 0 );

	void 				SetShader       ( int shaderID, int passID = 0 );
	int	 				GetShaderID		( const char* shaderName, BYTE* shBuf = NULL, int size = 0 );
	const char*			GetShaderName	( int shID ) const;

    int                 GetShaderVarID   ( int shID, const char* constantName ) { return -1; }
    int                 GetNShaderVars   ( int shID ) const { return 0; }
    bool                SetShaderTech    ( int shID, int techID ) { return false; }
    bool                SetShaderVar     ( int shID, int cID, bool val ) { return false; }
    bool                SetShaderVar     ( int shID, int cID, float val ) { return false; }
    bool                SetShaderVar     ( int shID, int cID, int val ) { return false; }
    bool                SetShaderVar     ( int shID, int cID, const Matrix4D& val ) { return false; }
    bool                SetShaderVar     ( int shID, int cID, const Vector4D& val ) { return false; }
    bool                SetShaderVar     ( int shID, int cID, const Vector3D& val ) { return false; }

	bool 				IsShaderValid	( const char* shaderName );
	virtual bool		SetClipPlane	( DWORD idx, const Plane& plane );

	int	 				GetTextureID	( const char* texName, BYTE* pMemFile = 0, int memFileSize = 0 );
    const char*		    GetTextureName	( int texID ) const;
    int			        GetTextureSize  ( int texID ) const;
    int			        GetTextureWidth ( int texID ) const;
    int			        GetTextureHeight( int texID ) const;
    int			        GetTextureNMips ( int texID ) const;

	void				SetRootDir		( const char* rootDir );
	bool				SetScreenProp	( const ScreenProp& prop );	
	ScreenProp			GetScreenProp	();	

	void				CreateMipLevels	( int texID );
	void 				BindToTexture	( BaseMesh* drawn );
	void 				CopyTexture		( int destID, int srcID );
	void 				CopyTexture		( int destID, int srcID, RECT* rectList, int nRect, POINT* ptList = 0 );
	void				CopyTexture		( int destID, const Rct& destRct, int srcID, const Rct& srcRct );

	bool 				ReloadShaders	();
	bool 				ReloadTextures	();

	void 				SetTextureFactor( DWORD tfactor );
	void				SetZEnable		( bool bEnable = true );
	void				SetZWriteEnable	( bool bEnable = true );
    void  		        SetDitherEnable	( bool bEnable = true );

	void				SetAlphaRef		( BYTE alphaRef );
	void  				SetWireframe	( bool bEnable = true );

	void 				GetClientSize	( int& width, int& height );
	void				SetDirLight	    ( sg::DirectionalLight*	pLight, int& index );
	void				SetPointLight	( sg::PointLight*		pLight, int& index );
	void				SetSpotLight	( sg::SpotLight*		pLight, int& index );
	void				DisableLights	();

	void				SetFog			( sg::Fog* pFog = NULL );

    virtual void        SetMaterial     ( DWORD ambient, DWORD diffuse, DWORD specular = 0xFFFFFFFF, 
                                            DWORD emissive = 0, float power = 0.0f );

	const char*			GetRootDir      ();
	
	void				SetRSBlock	    ( sg::RenderStateBlock* pBlock			   );
	void				SetTSBlock      ( sg::TextureStateBlock* pBlock, int stage );
	
	DWORD				CreateStateBlock( sg::StateBlock* pBlock );
	bool				ApplyStateBlock	( DWORD id );
	bool				DeleteStateBlock( DWORD id );


	bool				ApplyStateBlock	( DSBlock* pBlock );
	DWORD				CreateStateBlock( DSBlock* pBlock );
	bool				GetCurrentStateBlock( DSBlock& dsb ) const;
	DXDevice*			GetDevice		() { return m_pDevice; }
	DXSurface*			GetTexSurface	( int texID );

	bool				VSMode          () const { return m_bVSMode; }
	void				SetVSMode       ( bool val = true ) { m_bVSMode = val; }

	int					GetNDisplayModes();
	void				GetDisplayMode	( int idx, int& width, int& height );

    int                 CreateNormalMap ( int texID, float amplitude = 1.0f );

    virtual BYTE*		LockVB	        ( int vbID, int offset, int size );
    virtual bool		DiscardVB	    ( int vbID ) { return false; }
    virtual void		UnlockVB	    ( int vbID );
    virtual int         CreateVB        ( int size, int vType, bool bDynamic = false );
    virtual bool        DeleteVB        ( int vbID );

    virtual WORD*		LockIB	        ( int ibID, int offset, int size );
    virtual bool		DiscardIB	    ( int ibID );
    virtual void		UnlockIB	    ( int ibID );
    virtual int         CreateIB        ( int size, IndexSize idxSize, bool bDynamic = false );
    virtual bool        DeleteIB        ( int ibID );

    virtual int         RegisterVType   ( const VertDeclaration& vdecl ) { return -1; }
    virtual void  		Draw			( int vbID, int firstVert, int nVert, 
                                            const WORD* pIdx, int nIdx, 
                                            PrimitiveType priType = ptTriangleList );
    virtual void  		Draw			( int vType, void* pVert, int nVert,
                                            const WORD* pIdx, int nIdx, 
                                            PrimitiveType priType = ptTriangleList );
    virtual void  		Draw			( int vbID, int firstVert, int nVert, 
                                                int ibID, int firstIdx, int nIdx, 
                                                PrimitiveType priType = ptTriangleList );

    //  instanced drawing
    virtual void  		Draw			( const Matrix4D* m_Transforms, int nInstances, 
                                                int vbID, int firstVert, int nVert, 
                                                int ibID, int firstIdx, int nIdx,
                                                PrimitiveType priType = ptTriangleList );

    virtual int		    GetFontID		( const char* name );
    virtual void	    DestroyFont		( int fontID );
    virtual int		    CreateFont		( const char* name, int height, DWORD charset = DEFAULT_CHARSET, bool bBold = false, bool bItalic = false );
    virtual int		    CreateFont      ( const char* texName, int charW, int charH );
    virtual int		    GetStringWidth	( int fontID, const char* str, int spacing = 1 );
    virtual int		    GetCharWidth	( int fontID, BYTE ch );
    virtual int		    GetCharHeight	( int fontID, BYTE ch );
    virtual void        SetCurrentFont  ( int fontID );

    virtual bool	    DrawString		( const char* str, const Vector3D& pos, DWORD color = 0xFFFFFFFF, int spacing = 1 );
    virtual bool	    DrawString3D	( const char* str, const Vector3D& pos, DWORD color = 0xFFFFFFFF, int spacing = 1 );
    virtual bool	    DrawChar		( const Vector3D& pos, BYTE ch, DWORD color = 0xFFFFFFFF );
    virtual bool	    DrawChar		( const Vector3D& pos, const Rct& uv, DWORD color = 0xFFFFFFFF );
    virtual bool	    DrawChar		( const Vector3D& pos, const Rct& uv, float w, float h, DWORD color = 0xFFFFFFFF );
    virtual void	    FlushText		();	

    virtual void        DrawLine		( float x1, float y1, float x2, float y2, float z, DWORD color1, DWORD color2 );
    virtual void        DrawLine		( const Vector3D& a, const Vector3D& b, DWORD color1, DWORD color2 );
    virtual void        DrawRect		( const Rct& rct, const Rct& uv, float z, DWORD ca, DWORD cb, DWORD cc, DWORD cd );
    virtual void        DrawPoly		( float ax, float ay, float bx, float by, float cx, float cy,
                                            float au, float av, float bu, float bv, float cu, float cv );
    virtual void        DrawPoly		( const Vector3D& a, const Vector3D& b, const Vector3D& c, DWORD acol, DWORD bcol, DWORD ccol,
                                            float au, float av, float bu, float bv, float cu, float cv );
    virtual void        FlushPrim       ( bool bShaded = true );


    virtual int         CreateTexture   ( const char* texName, 
                                            int width, int height, 
                                            ColorFormat clrFormat, 
                                            int nMips = 0, 
                                            TextureMemoryPool memPool = tmpDefault, 
                                            bool bRenderTarget   = false, 
                                            DepthStencilFormat dsFormat = dsfNone,
                                            bool bDynamic        = false );

protected:
	bool				ResetDevice					();
	
	HRESULT				InitDeviceObjects			();
	HRESULT				RestoreDeviceObjects		();
	HRESULT				InvalidateDeviceObjects		();

    bool                SetupDisplaySettings        ();

	void				AdjustWindowPos				( int x, int y, int w, int h );
	bool				InitD3D						();
	bool				InitDeviceD3D				();
	bool				ShutDeviceD3D				();

	void				GetPresentParameters		( D3DPRESENT_PARAMETERS& presParm );
	void				BuildDeviceList				();
	void				DumpDeviceList				( FILE* fp );
	void				RestoreDesktopDisplayMode	();

	DWORD				GetCurFrame					() const { return m_CurFrame; }

    int					GetVertexShaderID           ( const char* shaderName );
    int					ApplyVertexShader           ( DWORD id );
    static LRESULT CALLBACK   MsgProc               ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

private:
	DXAPI*					        m_pD3D;
	DXDevice*				        m_pDevice;
	bool					        m_bVSMode;
	
	DXSurface*				        m_pRenderTarget; 
	DXSurface*				        m_pBackBuffer;
	DXSurface*				        m_pDepthStencil;

	char					        m_RootDirectory[_MAX_PATH];
	HWND					        m_hWnd;
	
	int						        m_RenderTargetID;
	D3DVIEWPORT8			        m_CurViewPort;
    D3DVIEWPORT8			        m_FullViewPort;
	int						        m_VBufID;
	DWORD					        m_CurStateBlockID;

	Settings				        m_Settings;
	ScreenProp				        m_ScreenProp;
	DEVMODE					        m_dmDesktop;

	//TextureManager			        m_TextureManager;
	ShaderCache				        m_ShaderCache;
	PrimitiveCache			        m_PrimitiveCache;

	D3DAdapterInfo			        m_AdapterList[c_MaxAdapters];
	int						        m_NAdapters;
	D3DDeviceInfo*			        m_CurDeviceInfo;

	int						        m_NumActiveLights;

	DWORD					        m_CurFrame;
    bool                            m_bInited;

    Matrix4D                        m_ViewMatrix;
    Matrix4D                        m_ProjectionMatrix;
    Matrix4D                        m_WorldMatrix;
    Matrix4D                        m_TextureMatrix[c_MaxTextureStages];
    Matrix3D                        m_BumpMatrix[c_MaxTextureStages];

	std::vector<DXVertexShader>		m_VertexShaders;
    std::vector<VertexBuffer*>	    m_VertexBuffers;
	std::vector<IDeviceClient*>	m_NotifyDestroy;
}; // class D3DRenderSystem

#endif // __D3DRENDERSYSTEM_H__