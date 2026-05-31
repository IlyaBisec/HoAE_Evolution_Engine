/*****************************************************************************/
/*	File:	d3dRender.h
/*  Desc:	Render system interface implementation for DirectX9 
/*	Author:	Ruslan Shestopalyuk
/*	Date:	02.11.2004
/*****************************************************************************/
#ifndef __D3DRENDER_H__
#define __D3DRENDER_H__

#include <stack>
#include "kStaticArray.hpp"

/*****************************************************************************/
/*  Struct: VertexTypeEntry
/*  Desc:   Helper structure for caching vertex description
/*****************************************************************************/
struct VertexTypeEntry
{
    VertexFormat                    m_VFormat;      //  vertex format
    VertexDeclaration               m_VDecl;        //  vertex declaration
    DWORD                           m_FVF;          //  FVF code
    IDirect3DVertexDeclaration9*    m_pVDeclD3D;    //  D3D declaration object

    VertexTypeEntry() : m_VFormat(vfUnknown), m_FVF(0), m_pVDeclD3D(NULL) {}
}; // struct VertexTypeEntry

const int c_DynVBufferBytes     = 65535 * 64;
const int c_StaticVBufferBytes  = 1024*1024*16;
const int c_DynIBufferBytes     = 65535 * 8;
const int c_StaticIBufferBytes  = 1024*1024*8;
const int c_QuadIBufferBytes    = 65535 * 8;
const int c_MaxRenderTasks       = 16384;

typedef static_array<RenderTask*, c_MaxRenderTasks> SortedTasks;
/*****************************************************************
/*  Class:  RenderSystemDX9
/*  Desc:   Implementation of the render system for DirectX9 
/*****************************************************************/
class RenderSystemDX9 : public D3DApplication, public IRenderSystem
{
    bool                            m_bInited;
    Matrix4D                        m_ViewTM; 
    Matrix4D                        m_ProjTM; 
    Matrix4D                        m_WorldTM; 
	Matrix4D						m_WorldViewProjTM;
    Matrix4D                        m_TextureTM [c_MaxTextureStages];
    Matrix3D                        m_BumpTM    [c_MaxTextureStages]; 
    DWORD                           m_CurrentFrame;
    DWORD                           m_TextureOverride[8];
    DWORD                           m_ColorConst;
    int                             m_ShadersQuality;
	bool							m_EnableBump;
    
    Rct                             m_ViewPort;         //  current viewport extents
    float                           m_ViewPortZNear;    //  current viewport near clip plane
    float                           m_ViewPortZFar;     //  current viewport far clip plane
    bool                            m_WTM_is1;          //  WorldTM is 1
    
	//fog state
    float                           m_FogDensity; 
	DWORD							m_FogColor;
	float							m_FogStart;
	float							m_FogEnd;
	float							m_FogMode;


    std::vector<IShader*>           m_Shaders;          //  shader registry
    std::vector<int>                m_LowShaderID;      //  low quality shader index -1 means no lq shader
	std::vector<int>				m_ShadowShaders;	//  shaders for shadow - they will be taken from shadows\ folder
    std::vector<IFont*>             m_Fonts;            //  fonts registry

    std::vector<TextureDX9*>        m_Textures;     //  texture registry
    std::vector<VertexBufferDX9*>   m_VBuffers;     //  vertex buffer registry
    std::vector<IndexBufferDX9*>    m_IBuffers;     //  index buffer registry
    std::vector<IDeviceClient*>     m_DeviceClients;//  registry of device-dependent entities

    std::vector<VertexTypeEntry>    m_VertexTypes;  //  cached stock vertex type descriptions

    std::vector<D3DDISPLAYMODE>     m_BestDisplayModes;

    int                             m_CurShader;        //  index of the currently active shader
    int                             m_CurPass;          //  current shader pass
    int                             m_CurTex[c_MaxTextureStages];
    float                           m_FPS;
	float							m_UserShaderConsts[4];

	DWORD							m_EnablePostEffects;
	DWORD							m_BackbufferTextureID;
	DWORD							m_BackbufferZID;
	float							m_MotionBlurAlpha;
	bool							m_EnableBloom;
	float							m_AutoBrightDegree;
	float							m_AutoBrightSpeed;

    struct RenderTargetPair
    {
        int     m_Color;
        int     m_DepthStencil;

        RenderTargetPair( int clr, int depth ) : m_Color(clr), m_DepthStencil(depth) {}
    };

    std::stack<RenderTargetPair>    m_RTStack;

    IDirect3DSurface9*              m_pBackBufferSurface;
    IDirect3DSurface9*              m_pDepthStencilSurface;

public:
                            RenderSystemDX9 ();
                            ~RenderSystemDX9();
    virtual void			Init			( HINSTANCE hInst, HWND hWnd = NULL );
    virtual void			ShutDown		();
    virtual bool  			SetScreenProp	( const ScreenProp& prop );
    virtual ScreenProp		GetScreenProp	();
    virtual int			    GetNDisplayModes();
    virtual void		    GetDisplayMode	( int idx, int& width, int& height );
    virtual void			GetClientSize	( int& width, int& height );
    virtual void			AddClient       ( IDeviceClient* iNotify );
    virtual void  			ClearDevice		( DWORD color, bool bColor = true, bool bDepth = true, bool bStencil = true ) ;
    virtual DWORD 		    GetCurFrame		() const { return m_CurrentFrame; }
    virtual float           GetFPS          () const { return m_FPS; } 
	virtual void			EnablePostEffects	(bool Enable,float AutoBrightDegree,float AutoBrightSpeed,float ,float MotionBlurAlpha);
	virtual void			PreparePosteffects	();
	virtual void			RenderPosteffects	();

    //  transforms
    virtual void			SetViewTM	    ( const Matrix4D& vmatr );
    virtual void			SetProjTM       ( const Matrix4D& pmatr );
    virtual void			SetWorldTM	    ( const Matrix4D& wmatr );
	virtual void            SetWorldViewProjTM  ( const Matrix4D& wmatr );
    virtual void			ResetWorldTM    ();
    virtual void			SetTextureTM    ( const Matrix4D& tmatr, int stage = 0 );
    virtual void			SetBumpTM	    ( const Matrix3D& bmatr, int stage = 0 );
    virtual const Matrix4D& GetViewTM       () const { return m_ViewTM; }
    virtual const Matrix4D& GetProjTM       () const { return m_ProjTM; }
    virtual const Matrix4D& GetWorldTM      () const { return m_WorldTM; }
	virtual const Matrix4D& GetWorldViewProjTM() const { return m_WorldViewProjTM; }
    virtual const Matrix4D& GetTextureTM    ( int stage = 0 ) const { return m_TextureTM[stage]; }  
    virtual const Matrix3D& GetBumpTM       ( int stage = 0 ) const { return m_BumpTM[stage]; }

    //  cursor
    virtual bool  			SetCursor		( int texID, const Rct& rctOnTex, int hotspotX, int hotspotY );
    virtual bool  			UpdateCursor	( int x, int y, bool drawNow = false );
    virtual void  			ShowCursor		( bool bShow = true );

    //  vertex buffers
    virtual int             RegisterVType   ( const VertexDeclaration& vdecl );
    virtual BYTE*		    LockVB	        ( int vbID, int firstV, int numV, DWORD& stamp );
    virtual BYTE*		    LockAppendVB	( int vbID, int size, int& offset, DWORD& stamp );
    virtual void		    UnlockVB	    ( int vbID );
    virtual int             GetVBufferID    ( const char* vbName );
    virtual bool		    DiscardVB	    ( int vbID );
    virtual int             CreateVB        ( const char* name, int size, int vType, bool bDynamic = false );
    virtual bool            DeleteVB        ( int vbID );
    virtual bool            SetVB           ( int vbID,  int vType = -1, int stream = 0, int frequency = 1 );
    virtual bool            IsVBStampValid  ( int vbID, DWORD stamp );

    //  index buffers
    virtual BYTE*		    LockIB	        ( int ibID, int firstIdx, int numIdx, DWORD& stamp );
    virtual BYTE*		    LockAppendIB	( int ibID, int size, int& offset, DWORD& stamp );
    virtual void		    UnlockIB	    ( int ibID );
    virtual bool            IsIBStampValid  ( int ibID, DWORD stamp ); 
    virtual int             GetIBufferID    ( const char* ibName );
    virtual bool		    DiscardIB	    ( int ibID );
    virtual int             CreateIB        ( const char* name, int size, IndexSize idxSize, bool bDynamic = false );
    virtual bool            DeleteIB        ( int ibID );
    virtual bool            SetIB           ( int ibID );

    //  drawing
    virtual bool  			StartFrame		();
    virtual void  			EndFrame		();
    virtual void  			Draw			( int firstVert, int nVert, int firstIdx, int nIdx, 
                                                PrimitiveType priType = ptTriangleList );

    virtual float           GetFogDensity   () const { return m_FogDensity; }
    virtual void            SetFogDensity   ( float fog ) { m_FogDensity = fog; }

    //  textures
    virtual bool  			ReloadTextures  ();
    virtual void            SetTextureOverride  ( int TextureID,int Stage );
    virtual int             GetTextureOverride  ( int Stage );
    virtual void            SetTransparentTexOverride( int Stage );
    virtual void			SetTexture		( int texID, int stage = 0, bool bCache = false );
    virtual int				GetTexture		( int stage = 0 );
    virtual int				GetShader		() { return m_CurShader; }
    virtual bool			SaveTexture		( int texID, const char* fname );
    virtual void            CopyTexture     ( int destID, int srcID, const Rct* rct = NULL, int nRect = 1 ); 
    virtual void			CreateMipLevels	( int texID );
    virtual int				GetTextureID	( const char* texName );
	virtual bool			CopyRenderTarget(int RenderTargetID,int SysMemTexID);

	virtual void			RegCallbackOnGetTextureID(IRenderSystem::OnGetTextureID *Fn);
	virtual void			RegCallbackOnLoadTexture(IRenderSystem::OnLoadTexture *Fn);

    virtual int             CreateNormalMap ( int texID, float amplitude = 10.0f );
    virtual bool			DeleteTexture	( int texID );
    virtual BYTE*		    LockTexBits		( int texID, int& pitch, int level = 0 );
    BYTE* 				    LockTexBits		( int texID, const Rct& rect, int& pitch, int level = 0	);
    virtual void		    UnlockTexBits	( int texID, int level = 0 );
    virtual int   		    GetTexMemorySize() const;
    
    virtual const char*		GetTextureName	( int texID );
    virtual ColorFormat     GetTextureFormat( int texID );
    virtual int             GetNTextures    () const;
    virtual int             GetTextureSize  ( int texID ) const;
    virtual TextureMemoryPool GetTexturePool( int texID ) const;
    
    virtual int			    GetTextureWidth ( int texID ) const;
    virtual int			    GetTextureHeight( int texID ) const;
    virtual int			    GetTextureNMips ( int texID ) const;

    virtual int             CreateTexture   ( const char* texName, 
                                                int width, int height, 
                                                ColorFormat clrFormat, 
                                                int nMips = 0, 
                                                TextureMemoryPool memPool = tmpManaged, 
                                                bool bRenderTarget = false, 
                                                DepthStencilFormat dsFormat = dsfNone,
                                                bool bDynamic = false );

    //  shaders
    virtual int				GetShaderID		( const char* shaderName );
    virtual const char*		GetShaderName	( int shID ) const;
    virtual IShader*        GetShader       ( int shID ) const;
    virtual int             GetNShaderVars  ( int shID ) const;
    virtual int             GetNShaderPasses( int shID ) const;
    virtual int             GetShaderVarID  ( int shID, const char* constantName );
    virtual bool            SetShaderTech   ( int shID, int techID );
    virtual bool            SetShaderVar    ( int shID, int cID, bool val );
    virtual bool            SetShaderVar    ( int shID, int cID, float val );
    virtual bool            SetShaderVar    ( int shID, int cID, int val );
    virtual bool            SetShaderVar    ( int shID, int cID, const Matrix4D& val );
    virtual bool            SetShaderVar    ( int shID, int cID, const Vector4D& val );
	virtual bool            SetShaderVar    ( int shID, int cID, const Vector4D* val, int count );
    virtual bool            SetShaderVar    ( int shID, int cID, const Vector3D& val );
	virtual void			SetShaderConst	(int ConstIndex,float Value);
	virtual float			GetShaderConst  (int ConstIndex );
    virtual bool  			ReloadShaders   ();
    virtual void  		    SetShader       ( int shaderID, int passID = 0 );
    virtual void            SetShaderAutoVars();
    virtual bool  		    IsShaderValid	( int shID, int techID = 0 );
    virtual void            SetShadersQuality( int Level );
    virtual int             GetShadersQuality();
	virtual void			SetBumpEnable	( bool State );
	virtual	bool			GetBumpEnable	();

    virtual bool		    SetClipPlane	( DWORD idx, const Plane& plane );
    virtual bool		    PushRenderTarget( int texID, int dsID = -1 );
    virtual bool            PopRenderTarget ();
    virtual void		    SetViewPort		( const Rct& vp, float zn = 0.0f, float zf = 1.0f, bool bClip = true );
    virtual Rct			    GetViewPort		() const { return m_ViewPort; }
    
    virtual DWORD           GetTextureFactor() const { return m_TFactor; }
    virtual const char*     GetTexturePath  ( int texID ) const;
    //  direct render states changing 
    virtual void  		    SetTextureFactor( DWORD tfactor		  );
    virtual void  		    SetAlphaRef		( BYTE alphaRef		  );
    virtual void  		    SetZEnable		( bool bEnable = true );
    virtual void  		    SetZWriteEnable	( bool bEnable = true );
    virtual void  		    SetDitherEnable	( bool bEnable = true );
    virtual void  		    SetWireframe	( bool bEnable = true );
    virtual void            SetTexFilterEnable( bool bEnable = true ); 
    virtual void            SetColorConst   ( DWORD Color );
    virtual DWORD           GetColorConst   ();

    virtual void		    Dump			( const char* fname = 0 );
    virtual bool  		    ApplyStateBlock	( DWORD id );
    virtual bool  		    DeleteStateBlock( DWORD id );   
    virtual void  		    SetRSBlock	    ( RenderStateBlock*	pBlock			   );
    virtual void  		    SetTSBlock      ( TextureStateBlock* pBlock, int stage );
    virtual DWORD 		    CreateStateBlock( StateBlock*		 pBlock			   );

    //  lighting
    virtual void  		    SetDirLight	    ( DirectionalLight*	pLight, int& index );
    virtual void  		    SetPointLight	( PointLight*		pLight, int& index );
    virtual void  		    SetSpotLight	( SpotLight*		pLight, int& index );
	virtual void			SetFog( DWORD FogColor,float FogStart,float FogEnd,float FogDensity,int FogMode );
	virtual void			ApplyFogStateBlock  ();
    virtual void  		    DisableLights	();
    virtual void            SetMaterial     ( DWORD ambient, DWORD diffuse, DWORD specular, DWORD emissive, float power );

    //  font
    virtual int		        GetFontID		( const char* name );
    virtual void	        DestroyFont		( int fontID );
    virtual int		        CreateFont		( const char* name, int height, DWORD charset = DEFAULT_CHARSET, bool bBold = false, bool bItalic = false );
    virtual int		        CreateFont      ( const char* texName, int charW, int charH );
    virtual int		        GetStringWidth	( int fontID, const char* str, int spacing = 1 );
    virtual int		        GetCharWidth	( int fontID, BYTE ch );
    virtual int		        GetCharHeight	( int fontID, BYTE ch );
    virtual void            SetCurrentFont  ( int fontID );

    virtual bool	        DrawString		( const char* str, const Vector3D& pos, DWORD colorxFFFFFFFF, int spacing = 1 );
    virtual bool	        DrawString3D	( const char* str, const Vector3D& pos, DWORD colorxFFFFFFFF, int spacing = 1 );
    virtual bool	        DrawChar		( const Vector3D& pos, BYTE ch, DWORD colorxFFFFFFFF );
    virtual bool	        DrawChar		( const Vector3D& pos, const Rct& uv, DWORD colorxFFFFFFFF );
    virtual bool	        DrawChar		( const Vector3D& pos, const Rct& uv, float w, float h, DWORD colorxFFFFFFFF );
    virtual void	        FlushText		();	

    //  primitives
    virtual void            DrawLine		( float x1, float y1, float x2, float y2, float z, DWORD color1, DWORD color2 );
    virtual void            DrawLine		( const Vector3D& a, const Vector3D& b, DWORD color1, DWORD color2 );
    virtual void            DrawRect		( const Rct& rct, const Rct& uv, float z, DWORD ca, DWORD cb, DWORD cc, DWORD cd );
    virtual void            DrawPoly		( float ax, float ay, float bx, float by, float cx, float cy,
                                                float au, float av, float bu, float bv, float cu, float cv );
    virtual void            DrawPoly		( const Vector3D& a, const Vector3D& b, const Vector3D& c, DWORD acol, DWORD bcol, DWORD ccol,
                                                float au, float av, float bu, float bv, float cu, float cv );
    virtual void            FlushPrim       ( bool bShaded = true );
    virtual void            PurgeStaticBuffers ();

    virtual RenderTask&     AddTask         ();
    virtual void            Flush           ();

    //  internal methods for Direct3D-aware clients
    IDirect3DDevice9*       GetDevice       () { return m_pDevice; }
    IDirect3DSurface9*      GetSurface      ( int texID );

	virtual void			ScreenShotBMP		(const char *pBMPFileName);
	virtual void			ScreenShotJPG		(const char *pJPGFileName);

	virtual void			TimeOverrideEnable		(const float secNewTime);
	virtual void			TimeOverrideDisable		();
	virtual bool			TimeOverrideIsEnabled	(float *pSecTime = NULL);

	virtual void			AntialiasingEnable		();
	virtual void			AntialiasingDisable		();
	virtual bool			AntialiasingIsEnabled	();

	virtual void			TrueColorEnable			();
	virtual void			TrueColorDisable		();
	virtual bool			TrueColorIsEnabled		();

	virtual bool			GetDeviceDisplayMode	(int *pWidth, int *pHeight, int *pBpp, int *pRefrehsRate);

	virtual void			RefreshRateOverrideEnable(const int Hz);
	virtual void			RefreshRateOverrideDisable();
	virtual bool			RefreshRateOverrideIsEnabled(int *pHz = NULL);
	virtual HWND			GetHWND	() { return m_hWnd; }

	virtual void			AddScreenResolution		(const int XRes, const int YRes, const int RR);

protected:
	void					ScreenShot(const char *pFileName, _D3DXIMAGE_FILEFORMAT Format);


    bool                SetMonitorResolution    ();
    void                CreateVTypeTable        ();
    void                ClearVTypeTable         ();
    HRESULT             OneTimeSceneInit        ();
    HRESULT             InitDeviceObjects       ();
    HRESULT             RestoreDeviceObjects    ();
    HRESULT             InvalidateDeviceObjects ();
    HRESULT             DeleteDeviceObjects     ();
    HRESULT             FinalCleanup            ();
    HRESULT             ConfirmDevice           ( D3DCAPS9* pCaps, DWORD dwBehavior, 
                                                    D3DFORMAT adapterFormat, D3DFORMAT backBufferFormat );

    void                FillQuadIndexBuffer     ();
    virtual LRESULT     MsgProc                 ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );    

    RenderTask           m_Tasks[c_MaxRenderTasks];
    SortedTasks         m_SortedTasks;
    int                 m_NTasks;
    bool                m_bSorted;

    DWORD               m_TFactor;
    void                SortTasks();

	bool m_TimeIsOverridden;
	float m_secOverriddenTime;

	bool m_RefreshRateIsOverridden;
	int m_HzOverriddenRefreshRate;
	int m_HzDefaultRefreshRate;

	std::vector<IRenderSystem::OnGetTextureID *> m_OnGetTextureIDCallbacks;
	std::vector<IRenderSystem::OnLoadTexture *> m_OnLoadTextureCallbacks;

}; // class RenderSystemDX9

IDirect3DDevice9* GetDirect3DDevice();
IDirect3DSurface9* GetDirect3DSurface( int texID );



#endif // __D3DRENDER_H__


