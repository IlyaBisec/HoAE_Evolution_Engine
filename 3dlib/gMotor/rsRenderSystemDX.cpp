/*****************************************************************
/*  File:   rsRenderSystemDX.cpp                                 *
/*  Desc:   Direct3D rendering system implementation             *
/*  Author: Silver, Copyright (C) GSC Game World                 *
/*  Date:   Feb 2002                                             *
/*****************************************************************/
#include "stdafx.h"

#include "direct.h"
#include "rsDX.h"
#include "sgFog.h"
#include "kStatistics.h"
#include "kStrUtil.h"
#include "rsDeviceStates.h"

D3DCOLORVALUE DwordToD3DCOLORVALUE( DWORD col )
{
	D3DCOLORVALUE res;
	res.a = float( (col & 0xFF000000)>>24 ) / 255.0f;
	res.r = float( (col & 0x00FF0000)>>16 ) / 255.0f;
	res.g = float( (col & 0x0000FF00)>>8  ) / 255.0f;
	res.b = float( (col & 0x000000FF)     ) / 255.0f;
	return res;
}

//  O== PUBLIC INTERFACE ========================================O
D3DRenderSystem::D3DRenderSystem() 
{
	m_RenderTargetID	= 0;
	m_pBackBuffer		= NULL;
	m_pDepthStencil		= NULL;
	m_pRenderTarget		= NULL;
	m_NAdapters			= 0;
	m_RootDirectory[0]	= 0;
	m_NumActiveLights	= 0;
	m_bVSMode			= false;
	m_CurFrame			= 0;
	m_CurStateBlockID	= 0xFFFFFFFF;
    m_bInited           = false;
}

D3DRenderSystem::~D3DRenderSystem()
{
	ShutDown();
}

const char*	D3DRenderSystem::GetRootDir()
{ 
	return m_RootDirectory; 
}

void  D3DRenderSystem::SetViewTM( const Matrix4D& vmatr )
{
    m_ViewMatrix = vmatr;
	DX_CHK( m_pDevice->SetTransform( D3DTS_VIEW, (D3DMATRIX*)&vmatr ) );
}
void  D3DRenderSystem::SetProjTM( const Matrix4D& pmatr )
{
    m_ProjectionMatrix = pmatr;
	DX_CHK( m_pDevice->SetTransform( D3DTS_PROJECTION, (D3DMATRIX*)&pmatr ) );
}

void D3DRenderSystem::ResetWorldTM()
{
    m_WorldMatrix = Matrix4D::identity;
	DX_CHK( m_pDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&Matrix4D::identity ) );
}

void  D3DRenderSystem::SetWorldTM( const Matrix4D& wmatr )
{
    m_WorldMatrix = wmatr;
	DX_CHK( m_pDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&wmatr ) );
}

void  D3DRenderSystem::SetTextureTM( const Matrix4D& tmatr, int stage )
{
    m_TextureMatrix[stage] = tmatr;
	DX_CHK( m_pDevice->SetTransform( (D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0 + stage), (D3DMATRIX*)&tmatr ) );
} // D3DRenderSystem::SetTextureTM

const Matrix4D& D3DRenderSystem::GetTextureTM( int stage ) const
{
    return m_TextureMatrix[stage];
}

const Matrix3D& D3DRenderSystem::GetBumpTM( int stage ) const
{
    return m_BumpMatrix[stage];
}

bool D3DRenderSystem::SetClipPlane( DWORD idx, const Plane& plane )
{
	HRESULT hres = m_pDevice->SetClipPlane( idx, (float*)&plane );
	if (hres != S_OK)
	{
		Log.Error( "Could not set user clipping plane %d", idx );
		return false;
	}
	return true; 
} // D3DRenderSystem::SetClipPlane


void  D3DRenderSystem::ClearDevice( DWORD color, bool bColor, bool bDepth, bool bStencil )
{
	DWORD flags = 0;
	if (bColor) flags |= D3DCLEAR_TARGET;
	if (bDepth) flags |= D3DCLEAR_ZBUFFER;
	if (bStencil) flags |= D3DCLEAR_STENCIL;

	DX_CHK( m_pDevice->Clear( 0, NULL, flags, color, 1.0, 0 ) );
} // D3DRenderSystem::ClearDevice

void D3DRenderSystem::OnFrame()
{
	m_ShaderCache.OnFrame();

	m_CurStateBlockID = 0xFFFFFFFF;
    assert( m_pDevice );
    HRESULT hres = m_pDevice->TestCooperativeLevel();

    if (hres == D3DERR_DEVICELOST)
    {
        return;
    }

    if (hres == D3DERR_DEVICENOTRESET)
    {
        Log.Warning( "Device is lost. Resetting device and recreating/reloading"
            " all device-dependent resources..." );

        for (int i = 0; i < m_NotifyDestroy.size(); i++) m_NotifyDestroy[i]->OnDestroyRenderSystem();
        InvalidateDeviceObjects();
        ShutDeviceD3D();
        FORCE_RELEASE( m_pD3D );
        InitD3D();
        InitDeviceD3D();
        RestoreDeviceObjects();
        for (int i = 0; i < m_NotifyDestroy.size(); i++) m_NotifyDestroy[i]->OnCreateRenderSystem();
        return;
    }

    DX_CHK( m_pDevice->Present( NULL, NULL, NULL, NULL ) );

	DisableLights();
	m_CurFrame++;
	DX_CHK( m_pDevice->SetRenderState( D3DRS_FOGENABLE, FALSE ) );
    Stats::OnFrame();

	m_CurViewPort = m_FullViewPort;
} // D3DRenderSystem::OnFrame

//int  D3DRenderSystem::GetVBufferID()
//{
//	if (m_VBufID != -1) return m_VBufID;
//	//  create vbuffer texture
//	TextureDescr vbTD;
//	vbTD.setValues( m_ScreenProp.m_Width, 
//					m_ScreenProp.m_Width/* m_ScreenProp.height*/, 
//					m_ScreenProp.m_ColorFormat, 
//					mpVRAM, 1, tuRenderTarget );
//	m_VBufID = CreateTexture( "VBuffer", vbTD );
//	assert( m_VBufID != -1 );
//	return m_VBufID;
//} // D3DRenderSystem::GetVBufferID

void  D3DRenderSystem::DrawPrim( BaseMesh& bm )
{
	DX_CHK( m_pDevice->BeginScene() );
	m_PrimitiveCache.Draw( bm );
	DX_CHK( m_pDevice->EndScene() );
} // D3DRenderSystem::DrawPrim

void  D3DRenderSystem::Draw( BaseMesh& bm )
{
	DX_CHK( m_pDevice->BeginScene() );
	////m_TextureManager.SetTexture( bm.getTexture( 0 ), 0 );
	////m_TextureManager.SetTexture( bm.getTexture( 1 ), 1 );
	m_ShaderCache.ApplyShader( bm.getShader() ); 

	m_PrimitiveCache.Draw( bm );
	DX_CHK( m_pDevice->EndScene() );
} // D3DRenderSystem::Draw

void D3DRenderSystem::ShowCursor( bool bShow )
{
	DX_CHK( m_pDevice->ShowCursor( bShow ? TRUE : FALSE ) );
}

bool D3DRenderSystem::SetCursor( int texID, const Rct& rctOnTex, int hotspotX, int hotspotY )
{
	//int cTex = m_CursorTD.getID();
	//
	////  wrong side of the cached cursor bitmap surface
	//if (m_CursorTD.getSideX() != rctOnTex.w || 
	//	m_CursorTD.getSideY() != rctOnTex.h)
	//{
	//	//m_TextureManager.DeleteTexture( cTex );
	//	m_CursorTD.setID( c_NoID );
	//}

	//if (cTex == c_NoID)
	//{
	//	//  create cached cursor bitmap surface
	//	m_CursorTD.setSideX		( rctOnTex.w );
	//	m_CursorTD.setSideY		( rctOnTex.h );
	//	m_CursorTD.setMemPool	( mpManaged );
	//	m_CursorTD.setColFmt	( cfARGB8888 );
	//	m_CursorTD.setNMips		( 1 );
	//	m_CursorTD.setTexUsage	( tuProcedural );
	//	
	//	cTex = CreateTexture	( "HWCursor_Surface", m_CursorTD );
	//	m_CursorTD.setID		( cTex );
	//}

	////  copy pixels from the cursors texture surface to the cached cursor surface
	//POINT	pt; 
	//RECT	rct;
	//pt.x = pt.y = 0;
	//rct.left	= rctOnTex.x;
	//rct.right	= rctOnTex.GetRight() - 1;
	//rct.top		= rctOnTex.y;
	//rct.bottom	= rctOnTex.GetBottom() - 1;
	//
	//DXSurface* surfSrc   = GetTexSurface( texID );
	//DXSurface* surfDest  = GetTexSurface( cTex );

	//if (!surfDest || !surfSrc) return false;

	//DX_CHK( m_pDevice->CopyRects( surfSrc, &rct, 1, surfDest, &pt ));
	//DX_CHK( m_pDevice->SetCursorProperties( hotspotX, hotspotY, surfDest ) );
	//DX_CHK( m_pDevice->ShowCursor( TRUE ) );
	return true;
} // D3DRenderSystem::SetCursor

bool D3DRenderSystem::UpdateCursor( int x, int y, bool drawNow )
{
	assert( m_pDevice );
	DWORD flags = drawNow ? D3DCURSOR_IMMEDIATE_UPDATE : 0;
	m_pDevice->SetCursorPosition( x, y, flags );
	//DX_CHK( m_pDevice->ShowCursor( TRUE ) ); // ?
	return true;
} // D3DRenderSystem::UpdateCursor

IDirect3DSurface8* D3DRenderSystem::GetTexSurface( int texID )
{
	return NULL;////m_TextureManager.GetTextureSurface( texID, 0 );
} // D3DRenderSystem::GetTexSurface

void  D3DRenderSystem::CopyTexture( int destID, int srcID )
{
	assert( destID != 0 && srcID != 0 );
	////m_TextureManager.CopyTexture( destID, srcID );
}  // D3DRenderSystem::CopyTexture

void D3DRenderSystem::CopyTexture( int destID, const Rct& destRct, int srcID, const Rct& srcRct )
{
	RECT dRect, sRect;
	dRect.left	= destRct.x;
	dRect.top	= destRct.y;
	dRect.right = destRct.GetRight();
	dRect.bottom= destRct.GetBottom();

	sRect.left	= srcRct.x;
	sRect.top	= srcRct.y;
	sRect.right = srcRct.GetRight();
	sRect.bottom= srcRct.GetBottom();

	DXSurface* pDestSurface = GetTexSurface( destID );
	DXSurface* pSrcSurface = GetTexSurface( srcID );
	if (!pDestSurface || !pSrcSurface) return;
	HRESULT hres = D3DXLoadSurfaceFromSurface( pDestSurface, NULL, &dRect, pSrcSurface, NULL, &sRect, D3DX_DEFAULT, 0 );
	pDestSurface->Release();
	pSrcSurface->Release();
} // D3DRenderSystem::CopyTexture

void  D3DRenderSystem::CopyTexture( int destID, int srcID, 
											RECT* rectList, int nRect, POINT* ptList )
{
	if (destID != 0 && srcID != 0)
	{
		//m_TextureManager.CopyTexture( destID, srcID, rectList, nRect, ptList );
		return;
	}
	//  copy to the back buffer
	if (destID == 0)
	{
		//m_TextureManager.CopyTexture( m_pBackBuffer, srcID, rectList, nRect, ptList );
		return;
	}
} // D3DRenderSystem::CopyTexture

int  D3DRenderSystem::GetTexture( int stage )
{
	return -1;//m_TextureManager.GetCurrentTexture( stage );
}  // D3DRenderSystem::GetTexture

void  D3DRenderSystem::SetTexture( int texID, int stage )
{
	if (texID < 0) return;
	//m_TextureManager.SetTexture( texID, stage );
} // D3DRenderSystem::SetTexture

int D3DRenderSystem::CreateTexture( const char* texName, int width, int height, ColorFormat clrFormat, 
                                    int nMips, TextureMemoryPool memPool, 
                                    bool bRenderTarget, DepthStencilFormat dsFormat, bool bDynamic )
{
    return -1;
} // D3DRenderSystem::CreateTexture

void D3DRenderSystem::CreateMipLevels( int texID )
{
	//m_TextureManager.CreateMipLevels( texID );
} // D3DRenderSystem::CreateMipLevels

bool  D3DRenderSystem::DeleteTexture( int texID )
{
	return false;//m_TextureManager.DeleteTexture( texID );
} // D3DRenderSystem::DeleteTexture

void  D3DRenderSystem::SaveTexture( int texID, const char* fname )
{
	if (texID == 0)
	{
		assert( m_pBackBuffer );
		char ext[64];
		ParseExtension( fname, ext );

		D3DXIMAGE_FILEFORMAT type = D3DXIFF_DDS;
		if (!strcmp( ext, "bmp" )) type = D3DXIFF_BMP;
		if (!strcmp( ext, "tga" )) type = D3DXIFF_TGA;
		if (!strcmp( ext, "dds" )) type = D3DXIFF_DDS;

		HRESULT hres = D3DXSaveSurfaceToFile( fname, type, m_pBackBuffer, NULL, NULL );
		DX_CHK( hres );
		return;
	}
	//m_TextureManager.SaveTexture( texID, fname );
} // D3DRenderSystem::SaveTexture

int	D3DRenderSystem::GetTextureWidth( int texID ) const
{
    return 0;
} // D3DRenderSystem::GetTextureWidth

int	D3DRenderSystem::GetTextureNMips( int texID ) const
{
    return 0;
} // D3DRenderSystem::GetTextureNMips

int D3DRenderSystem::GetTextureHeight( int texID ) const
{
    return 0;
} // D3DRenderSystem::GetTextureHeight

int  D3DRenderSystem::GetTextureSize( int texID ) const
{
	return 0;//m_TextureManager.GetTextureSize( texID );
}  // D3DRenderSystem::GetTextureSize

BYTE*   D3DRenderSystem::LockTexBits( int texID, int& pitch, int level )
{
	if (texID < 0) return NULL;
	return NULL;//m_TextureManager.LockTexture( texID, pitch, level );
}  // D3DRenderSystem::LockTexBits

void  D3DRenderSystem::UnlockTexBits( int texID, int level )
{
	//m_TextureManager.UnlockTexture( texID, level );
} // D3DRenderSystem::UnlockTexBits

bool  D3DRenderSystem::ReloadTextures()
{
	//m_TextureManager.ReloadTextures();
    //m_TextureManager.LogStatus();
	return true;
}

void  D3DRenderSystem::GetClientSize( int& width, int& height )
{
	assert( false );
}  // D3DRenderSystem::GetClientSize

void  D3DRenderSystem::SetViewPort( const Rct& vp )
{
    SetViewPort( vp.x, vp.y, vp.w, vp.h, 0.0f, 1.0f );
} // D3DRenderSystem::SetViewPort

Rct  D3DRenderSystem::GetViewPort() const
{
    return Rct( m_CurViewPort.X, m_CurViewPort.Y, m_CurViewPort.Width, m_CurViewPort.Height ); 
}

void D3DRenderSystem::SetViewPort( float x, float y, float w, float h, float zn, float zf )
{
	Rct cvp( x, y, w, h );
    Rct svp( 0, 0, m_ScreenProp.m_Width, m_ScreenProp.m_Height );
    svp.Clip( cvp );

	D3DVIEWPORT8 dvp;
	dvp.X		= cvp.x;
	dvp.Y		= cvp.y;
	dvp.Width	= cvp.w;
    dvp.Height	= cvp.h;
    dvp.MinZ	= 0.0f;
    dvp.MaxZ	= 1.0f;
    m_pDevice->SetViewport( (D3DVIEWPORT8*)&dvp );	

    Rct old( m_CurViewPort.X, m_CurViewPort.Y, m_CurViewPort.Width, m_CurViewPort.Height );
    m_CurViewPort = dvp;
} // D3DRenderSystem::SetViewPort

bool  D3DRenderSystem::SetRenderTarget( int texID, int dsID )
{
	if (m_RenderTargetID == texID) return true;
	m_RenderTargetID = texID;
	
	if (m_RenderTargetID == 0)
	{
		HRESULT hRes = m_pDevice->SetRenderTarget( m_pBackBuffer, m_pDepthStencil );
        if (hRes != S_OK)
        {
            DX_CHK( S_OK );
            return false;
        }
	}
	else
	{
		m_pRenderTarget			 = NULL;//m_TextureManager.GetTextureSurface( m_RenderTargetID );
		DXSurface* pDepthStencil = NULL;//m_TextureManager.GetTextureSurface( dsID );
		HRESULT hRes = m_pDevice->SetRenderTarget( m_pRenderTarget, pDepthStencil );
        if (hRes != S_OK)
        {
            DX_CHK( S_OK );
            return false;
        }
        SAFE_DECREF( m_pRenderTarget );
	}
    return true;
} // D3DRenderSystem::SetRenderTarget

void D3DRenderSystem::AdjustWindowPos( int x, int y, int w, int h )
{
	SetWindowLong( m_hWnd, GWL_STYLE, WS_POPUP ); 
	SetWindowPos( m_hWnd, HWND_NOTOPMOST, x, y, w, h, 0 );
} // D3DRenderSystem::AdjustWindowPos

void*  D3DRenderSystem::DbgGetDevice()
{
	return (void*)m_pDevice;
}

bool D3DRenderSystem::InitD3D()
{
	m_pD3D = Direct3DCreate8( D3D_SDK_VERSION );
	massert( m_pD3D != NULL, "Failed to create DirectX device" );
	BuildDeviceList();

	//  pick default device
	m_CurDeviceInfo = m_AdapterList[0].FindDevice( D3DDEVTYPE_HAL );
	massert( m_CurDeviceInfo, "No render device available." );
	return true;
} // D3DRenderSystem::InitD3D

bool D3DRenderSystem::ShutDeviceD3D()
{
	SetTexture( 0, 0 );
	SetTexture( 0, 1 );
	//SetShader( 0 );

	//m_TextureManager.Shut();
	m_PrimitiveCache.Shut();

	SAFE_RELEASE( m_pBackBuffer     );
	SAFE_RELEASE( m_pDepthStencil );
	SAFE_RELEASE( m_pDevice );
	m_pDevice = NULL;
    m_bInited = false;
	return true;
} // D3DRenderSystem::ShutDeviceD3D

bool D3DRenderSystem::SetupDisplaySettings()
{
    DEVMODE devMode;
    ZeroMemory( &devMode, sizeof( devMode ) );
    devMode.dmSize              = sizeof( DEVMODE );
    devMode.dmBitsPerPel	    = 16;
    devMode.dmPelsWidth         = m_ScreenProp.m_Width;
    devMode.dmPelsHeight        = m_ScreenProp.m_Height;
    devMode.dmDisplayFrequency  = m_ScreenProp.m_RefreshRate;
    devMode.dmFields            = DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY | DM_BITSPERPEL;

    DWORD displayModeFlags = 0;
    if (m_ScreenProp.m_bFullScreen) displayModeFlags |= CDS_FULLSCREEN;
    int devModeRes = ChangeDisplaySettings( &devMode, displayModeFlags );
    DWORD winErr = GetLastError();
    if (devModeRes != DISP_CHANGE_SUCCESSFUL)
    {
        Log.Error( GetDispChangeErrorDesc( devModeRes ) );
        return false;
    }

    if (m_ScreenProp.m_bCoverDesktop)
    {
        RECT drct;
        HWND hDesk = GetDesktopWindow();
        ::GetWindowRect( hDesk, &drct );
        SystemParametersInfo( SPI_GETWORKAREA, 0, &drct, 0 );
        m_ScreenProp.m_Width  = drct.right  - drct.left;
        m_ScreenProp.m_Height = drct.bottom - drct.top;
    }
    
    int wX = 0;
    int wY = 0;
    int wW = m_ScreenProp.m_Width;
    int wH = m_ScreenProp.m_Height;

    SetWindowPos( m_hWnd, HWND_NOTOPMOST, wX, wY, wW, wH, SWP_SHOWWINDOW );
    ShowWindow( m_hWnd, SW_SHOW );

    RECT	wrct;
    ::GetWindowRect( m_hWnd, &wrct );
    m_FullViewPort.X		= 0.0f;
    m_FullViewPort.Y		= 0.0f;
    m_FullViewPort.Width 	= wrct.right - wrct.left;
    m_FullViewPort.Height	= wrct.bottom - wrct.top;
    m_FullViewPort.MinZ  	= 0.0f;
    m_FullViewPort.MaxZ  	= 1.0f;

    m_CurViewPort = m_FullViewPort;
    return true;
} // D3DRenderSystem::SetupDisplaySettings

bool D3DRenderSystem::InitDeviceD3D()
{
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
	
	BOOL bWindowed = (m_ScreenProp.m_bFullScreen == false);
    d3dpp.Windowed = bWindowed;

	if (bWindowed) 
	{
		d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	}
	else
	{
		d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
	}

	m_ScreenProp.m_ColorFormat = cfRGB565;

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	//d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

    d3dpp.BackBufferFormat		 = ColorFormatG2DX( m_ScreenProp.m_ColorFormat );
    d3dpp.BackBufferCount        = 1;
    d3dpp.hDeviceWindow          = m_hWnd;

	if (!m_ScreenProp.m_bCoverDesktop && !m_ScreenProp.m_bFullScreen) 
	{
		RECT rctClient;
		GetClientRect( m_hWnd, &rctClient );
		m_ScreenProp.m_Width  = rctClient.right - rctClient.left;
		m_ScreenProp.m_Height = rctClient.bottom - rctClient.top;
	}
	
	d3dpp.BackBufferWidth   = m_FullViewPort.Width;
	d3dpp.BackBufferHeight  = m_FullViewPort.Height;


	DWORD flags = D3DCREATE_MIXED_VERTEXPROCESSING;
	//DWORD flags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	//DWORD flags = D3DCREATE_HARDWARE_VERTEXPROCESSING;

	
	//if (m_Settings.hardwareTnL) flags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	//	else flags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	

	if (m_Settings.pureDevice) flags |= D3DCREATE_PUREDEVICE;
	
	D3DDEVTYPE devType = m_Settings.softwareRendering ? D3DDEVTYPE_REF : D3DDEVTYPE_HAL;

    // Create the D3DDevice
    HRESULT res = m_pD3D->CreateDevice(	m_Settings.adapterOrdinal, 
										devType, 
										m_hWnd,
										flags,
										&d3dpp, 
										&m_pDevice );

	if (res != S_OK)
	{
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		res = m_pD3D->CreateDevice(	m_Settings.adapterOrdinal, 
									devType, 
									m_hWnd,
									flags,
									&d3dpp, 
									&m_pDevice );
	
	}
	DX_CHK( res );

	if (!m_pDevice) return false;

	DX_CHK( m_pDevice->ShowCursor( FALSE ) );
	return true;
} // D3DRenderSystem::InitDeviceD3D

void  D3DRenderSystem::Init( HINSTANCE hInst, HWND hWnd )
{
	IRS = this;
    
    if (m_bInited)
    {
        Log.Error( "Trying to initialize render device twice." );
        return;
    }
    
    if (hWnd == NULL)
    {
        // Register the windows class
        WNDCLASS wndClass = { 0, MsgProc, 0, 0, hInst,
                                    NULL,LoadCursor( NULL, IDC_ARROW ),
                                    (HBRUSH)GetStockObject(WHITE_BRUSH),
                                    NULL, "Kangaroo Window" };
        RegisterClass( &wndClass );

        // Set the window's initial width
        RECT rc;
        SystemParametersInfo( SPI_GETWORKAREA, 0, &rc, 0 );
        // Create the render window
        hWnd = CreateWindow(    "Kangaroo Window", "Kangaroo", WS_POPUP,
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                rc.right - rc.left, rc.bottom - rc.top, 0,
                                NULL, hInst, 0 );
        ShowWindow( hWnd, SW_SHOW );
        UpdateWindow( hWnd );
        ::SetCursor( LoadCursor( NULL, IDC_ARROW ) );
    }
    m_hWnd = hWnd;
    
    getcwd( m_RootDirectory, _MAX_PATH );
	m_hWnd = hWnd;

	Log.Info( "Initializing D3D Render System..." );
	InitD3D();
	
    D3DDISPLAYMODE d3ddm;
    assert( m_pD3D );
	DX_CHK( m_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) );

	assert( m_CurDeviceInfo );
	D3DModeInfo* modeInfo = m_CurDeviceInfo->FindMode( d3ddm.Width, d3ddm.Height );
	if (!modeInfo)
    {
        Log.Warning( "Current display mode is not supported." );
    }
	m_ScreenProp.m_Width		= d3ddm.Width;
    m_ScreenProp.m_Height       = d3ddm.Height;
	m_ScreenProp.m_RefreshRate	= modeInfo ? modeInfo->refreshRate : 0;
    m_ScreenProp.m_ColorFormat	= ColorFormatDX2G( d3ddm.Format );

	//  get current desktop display mode in order to restore it at shutdown
	EnumDisplaySettings( NULL, ENUM_CURRENT_SETTINGS, &m_dmDesktop );

	RECT rct;
	GetClientRect( hWnd, &rct );
	int clWidth = rct.right - rct.left;
	int clHeight = rct.bottom - rct.top;
    if (!SetupDisplaySettings()) return;
	if (!InitDeviceD3D()) return;	
	InitDeviceObjects();

	Log.Info( "D3D Render System was initialized succesfully." );

	SetWorldTM( Matrix4D::identity );
    m_bInited = true;
} // D3DRenderSystem::Init

LRESULT CALLBACK D3DRenderSystem::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
    case WM_PAINT:
        // Handle paint messages when the app is paused
      /*  if( m_pDevice && !m_bActive && 
            m_bDeviceObjectsInited && m_bDeviceObjectsRestored )
        {
            HRESULT hr;
            Render();
            hr = m_pDevice->Present( NULL, NULL, NULL, NULL );
            if( D3DERR_DEVICELOST == hr )
                m_bDeviceLost = true;
        }*/
        break;

    case WM_GETMINMAXINFO:
        ((MINMAXINFO*)lParam)->ptMinTrackSize.x = 100;
        ((MINMAXINFO*)lParam)->ptMinTrackSize.y = 100;
        break;

    case WM_ENTERSIZEMOVE:
        // Halt frame movement while the app is sizing or moving
        break;

    //case WM_SIZE:
    //    // Pick up possible changes to window style due to maximize, etc.
    //    if( m_bWindowed && m_hWnd != NULL )
    //        m_dwWindowStyle = GetWindowLong( m_hWnd, GWL_STYLE );

    //    if( SIZE_MINIMIZED == wParam )
    //    {
    //        if( m_bClipCursorWhenFullscreen && !m_bWindowed )
    //            ClipCursor( NULL );
    //        Pause( true ); // Pause while we're minimized
    //        m_bMinimized = true;
    //        m_bMaximized = false;
    //    }
    //    else if( SIZE_MAXIMIZED == wParam )
    //    {
    //        if( m_bMinimized )
    //            Pause( false ); // Unpause since we're no longer minimized
    //        m_bMinimized = false;
    //        m_bMaximized = true;
    //        HandlePossibleSizeChange();
    //    }
    //    else if( SIZE_RESTORED == wParam )
    //    {
    //        if( m_bMaximized )
    //        {
    //            m_bMaximized = false;
    //            HandlePossibleSizeChange();
    //        }
    //        else if( m_bMinimized)
    //        {
    //            Pause( false ); // Unpause since we're no longer minimized
    //            m_bMinimized = false;
    //            HandlePossibleSizeChange();
    //        }
    //        else
    //        {
    //            // If we're neither maximized nor minimized, the window size 
    //            // is changing by the user dragging the window edges.  In this 
    //            // case, we don't reset the device yet -- we wait until the 
    //            // user stops dragging, and a WM_EXITSIZEMOVE message comes.
    //        }
    //    }
    //    break;

    //case WM_EXITSIZEMOVE:
    //    Pause( false );
    //    HandlePossibleSizeChange();
    //    break;

    //case WM_SETCURSOR:
    //    // Turn off Windows cursor in fullscreen mode
    //    if( m_bActive && !m_bWindowed )
    //    {
    //        SetCursor( NULL );
    //        if( m_bShowCursorWhenFullscreen )
    //            m_pDevice->ShowCursor( true );
    //        return true; // prevent Windows from setting cursor to window class cursor
    //    }
    //    break;

    //case WM_MOUSEMOVE:
    //    if( m_bActive && m_pDevice != NULL )
    //    {
    //        POINT ptCursor;
    //        GetCursorPos( &ptCursor );
    //        if( !m_bWindowed )
    //            ScreenToClient( m_hWnd, &ptCursor );
    //        m_pDevice->SetCursorPosition( ptCursor.x, ptCursor.y, 0 );
    //    }
    //    break;

    case WM_ENTERMENULOOP:
        // Pause the app when menus are displayed
        break;

    case WM_EXITMENULOOP:
        break;

    //case WM_NCHITTEST:
    //    // Prevent the user from selecting the menu in fullscreen mode
    //    if( !m_bWindowed )
    //        return HTCLIENT;
    //    break;

    case WM_POWERBROADCAST:
        switch( wParam )
        {
#ifndef PBT_APMQUERYSUSPEND
#define PBT_APMQUERYSUSPEND 0x0000
#endif
    case PBT_APMQUERYSUSPEND:
        // At this point, the app should save any data for open
        // network connections, files, etc., and prepare to go into
        // a suspended mode.
        return true;

#ifndef PBT_APMRESUMESUSPEND
#define PBT_APMRESUMESUSPEND 0x0007
#endif
    case PBT_APMRESUMESUSPEND:
        // At this point, the app should recover any data, network
        // connections, files, etc., and resume running from when
        // the app was suspended.
        return true;
        }
        break;

    case WM_SYSCOMMAND:
        // Prevent moving/sizing and power loss in fullscreen mode
        switch( wParam )
        {
        case SC_MOVE:
        case SC_SIZE:
        case SC_MAXIMIZE:
        case SC_KEYMENU:
        case SC_MONITORPOWER:
            /*if( false == m_bWindowed )
                return 1;*/
            break;
        }
        break;

    case WM_CLOSE:
        /*Cleanup3DEnvironment();
        SAFE_RELEASE( m_pD3D );
        FinalCleanup();
        HMENU hMenu;
        hMenu = GetMenu(hWnd);
        if( hMenu != NULL )
            DestroyMenu( hMenu );*/
        //DestroyWindow( m_hWnd );
        PostQuitMessage(0);
        //m_hWnd = NULL;
        return 0;
    }

    return DefWindowProc( hWnd, uMsg, wParam, lParam );
} // D3DRenderSystem::MsgProc

HRESULT D3DRenderSystem::InitDeviceObjects()
{
	DX_CHK( m_pDevice->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer ) );
	DX_CHK( m_pDevice->GetDepthStencilSurface( &m_pDepthStencil ) );

	m_PrimitiveCache.Init();
	m_ShaderCache.Init();
	//m_TextureManager.Init();

	m_VBufID = -1;
	return S_OK;
} // D3DRenderSystem::InitDeviceObjects

HRESULT D3DRenderSystem::RestoreDeviceObjects()
{
	DX_CHK( m_pDevice->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer ) );
	DX_CHK( m_pDevice->GetDepthStencilSurface	( &m_pDepthStencil ) );

	m_PrimitiveCache.RestoreDeviceObjects();
	//m_TextureManager.RestoreDeviceObjects();

	return S_OK;
} // D3DRenderSystem::RestoreDeviceObjects

void  D3DRenderSystem::SetRootDir( const char* rootDir )
{
	strcpy( m_RootDirectory, rootDir );
} // D3DRenderSystem::SetRootDir

void  D3DRenderSystem::ShutDown()
{
	ShutDeviceD3D();
	RestoreDesktopDisplayMode();
	FORCE_RELEASE( m_pD3D );
}//  D3DRenderSystem::ShutDown

void D3DRenderSystem::RestoreDesktopDisplayMode()
{
	int devModeRes = ChangeDisplaySettings( &m_dmDesktop, 0 );
	if (devModeRes != DISP_CHANGE_SUCCESSFUL)
	{
		Log.Error( "Could not restore desktop display mode: %s", 
							GetDispChangeErrorDesc( devModeRes ) );
	}
} // D3DRenderSystem::RestoreDesktopDisplayMode


void D3DRenderSystem::Dump( const char* fname )
{
}

//  shaders
void  D3DRenderSystem::SetShader( int shaderID, int passID )
{
	bool res = m_ShaderCache.ApplyShader( shaderID ); 
	//Dump();
} // D3DRenderSystem::SetShader

bool D3DRenderSystem::ApplyStateBlock( DWORD id )
{
	if (!m_pDevice) return false;
	if (m_CurStateBlockID == id) return true;
	m_CurStateBlockID = id;
	DX_CHK( m_pDevice->ApplyStateBlock( id ) );
	return true;
} // D3DRenderSystem::ApplyStateBlock

bool D3DRenderSystem::DeleteStateBlock( DWORD id )
{
	if (!m_pDevice || id == DSBlock::c_BadDevHandle) return false;
	DX_CHK( m_pDevice->DeleteStateBlock( id ) );
	return true;
} // D3DRenderSystem::DeleteStateBlock

DWORD D3DRenderSystem::CreateStateBlock( DSBlock* pBlock )
{
	if (!m_pDevice || m_pDevice->BeginStateBlock() != S_OK) return 0xFFFFFFFF;
	DWORD id;
	ApplyStateBlock( pBlock );
	if (m_pDevice->EndStateBlock( &id ) != S_OK) return 0xFFFFFFFF;
	return id; 
}

bool D3DRenderSystem::GetCurrentStateBlock( DSBlock& dsb ) const
{
	for (int i = 0; i < dsb.GetNRS(); i++)
	{
		DeviceState* pState = dsb.GetRS( i );
		DX_CHK( m_pDevice->GetRenderState(	(D3DRENDERSTATETYPE)pState->devID, 
											&pState->value ) );
	}

	for (int i = 0; i < c_MaxTextureStages; i++)
	{
		for (int j = 0; j < dsb.GetNTSS( i ); j++)
		{
			DeviceState* pState =  dsb.GetTSS( i, j );
			DX_CHK( m_pDevice->GetTextureStageState(	i, 
													(D3DTEXTURESTAGESTATETYPE)pState->devID, 
													&pState->value ) );
		}
	}
	return true;
} // D3DRenderSystem::GetCurrentStateBlock

bool D3DRenderSystem::ApplyStateBlock( DSBlock* pBlock )
{
	if (!m_pDevice) return false;
	for (int i = 0; i < pBlock->GetNRS(); i++)
	{
		const DeviceState* pState = pBlock->GetRS( i );
		DX_CHK( m_pDevice->SetRenderState( (D3DRENDERSTATETYPE)pState->devID, pState->value ) );
	}

	for (int i = 0; i < c_MaxTextureStages; i++)
	{
		for (int j = 0; j < pBlock->GetNTSS( i ); j++)
		{
			const DeviceState* pState = pBlock->GetTSS( i, j );
			DX_CHK( m_pDevice->SetTextureStageState( i,
								(D3DTEXTURESTAGESTATETYPE)pState->devID, 
								pState->value ) );
		}
	}
	return true;
} // D3DRenderSystem::ApplyStateBlock
//
//bool D3DRenderSystem::SetVSConstant( int cIdx, const Vector4D& cVec )
//{
//	DX_CHK( m_pDevice->SetVertexShaderConstant( cIdx, (void*)&cVec, 1 ) );
//	return true;
//} // D3DRenderSystem::SetVSConstant
//
//bool D3DRenderSystem::SetVSConstant( int cIdx, const Matrix4D& cMatr, bool bFull )
//{
//	DX_CHK( m_pDevice->SetVertexShaderConstant( cIdx, (void*)&cMatr, bFull ? 4 : 3 ) );
//	return true;
//} // D3DRenderSystem::SetVSConstant

const char*	 D3DRenderSystem::GetTextureName( int texID ) const 
{
	return "";//m_TextureManager.GetTextureName( texID );
} // D3DRenderSystem::GetTextureName

int   D3DRenderSystem::GetTextureID( const char* texName, BYTE* pMemFile, int memFileSize )
{
	if (!texName || texName[0] == 0) 
	{
		return -1;
	}
	
	int id = -1;//m_TextureManager.GetTextureID( texName, pMemFile, memFileSize );
	if (id >= 0) return id;

	char fname[_MAX_PATH];
	strcpy( fname, GetRootDirectory() );
	strcat( fname, "textures\\" );
	if (!LocateFile( texName, fname ))
	{
		Log.Warning( "Texture file does not exist: %s", texName );
		return -1;
	}
	strcat( fname, texName );

	id = -1;//m_TextureManager.GetTextureID( fname, pMemFile, memFileSize );
	return id;
} // D3DRenderSystem::GetTextureID

bool  D3DRenderSystem::IsShaderValid( const char* shaderName )
{
    return false;
	////  create file name
	//char fname[_MAX_PATH];
	//sprintf( fname, "%s\\shaders\\%s.sha", m_RootDirectory, shaderName );

	//ID3DXEffect* eff;
	//ID3DXBuffer* compileErr;
	//HRESULT hres = S_FALSE;
 //   //hres = D3DXCreateEffectFromFile( m_pDevice, fname, &eff, &compileErr );
	//if (hres != S_OK) 
	//{
	//	if (!compileErr)
	//	{
	//		Log.Warning( "Could not load shader file %s", fname );
	//		return false;
	//	}
	//	else
	//	{
	//		char* errbuf = (char*)compileErr->GetBufferPointer();
	//		Log.Warning( "Could not compile shader <%s>. Error message: %s", 
	//						shaderName, errbuf );
	//		SAFE_RELEASE( compileErr );
	//		return false;
	//	}
	//}
	//hres = eff->SetTechnique( "T0" );
	//bool valid = true;
	//if (hres != S_OK) valid = false;
	////  Checking if device is capable to set this shader
	//if (eff->Validate() != S_OK) valid = false;
	//SAFE_RELEASE( eff );
	//return valid;
} // D3DRenderSystem::IsShaderValid

const char* D3DRenderSystem::GetShaderName( int shID ) const
{
	return m_ShaderCache.GetShaderName( shID );
} // D3DRenderSystem::GetShaderName

int	D3DRenderSystem::GetShaderID( const char* shaderName, BYTE* shBuf, int size )
{
	return m_ShaderCache.GetShaderID( shaderName, shBuf, size );
} // D3DRenderSystem::GetShaderID

bool  D3DRenderSystem::ReloadShaders()
{	
	m_ShaderCache.ReloadShaders();
    ////m_TextureManager.LogStatus();
	return true;
} // D3DRenderSystem::ReloadShaders

void  D3DRenderSystem::SetTextureFactor( DWORD tfactor )
{
	DX_CHK( m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, tfactor ) );	
}

void D3DRenderSystem::SetZEnable( bool bEnable )
{
	DX_CHK( m_pDevice->SetRenderState( D3DRS_ZENABLE, bEnable ? TRUE : FALSE ) );	
}

void D3DRenderSystem::SetZWriteEnable( bool bEnable )
{
	DX_CHK( m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, bEnable ? TRUE : FALSE ) );	
}

void D3DRenderSystem::SetDitherEnable( bool bEnable )
{
    DX_CHK( m_pDevice->SetRenderState( D3DRS_DITHERENABLE, bEnable ? TRUE : FALSE ) );
}

void D3DRenderSystem::SetWireframe( bool bEnable )
{
	DX_CHK( m_pDevice->SetRenderState( D3DRS_FILLMODE, bEnable ? D3DFILL_WIREFRAME : D3DFILL_SOLID ) );	
}

void  D3DRenderSystem::SetAlphaRef( BYTE alphaRef )
{
	DX_CHK( m_pDevice->SetRenderState( D3DRS_ALPHAREF, alphaRef ) );	
}

void D3DRenderSystem::GetPresentParameters( D3DPRESENT_PARAMETERS& presParm )
{
	if (m_ScreenProp.m_bFullScreen)
	{
		presParm.BackBufferWidth	= m_ScreenProp.m_Width;
		presParm.BackBufferHeight	= m_ScreenProp.m_Height;
	}
	else
	{
		RECT rect;
		assert( m_hWnd );
		GetClientRect( m_hWnd, &rect );
		presParm.BackBufferWidth	= rect.right - rect.left;
		presParm.BackBufferHeight	= rect.bottom - rect.top;
	}

	presParm.BackBufferFormat		= ColorFormatG2DX( m_ScreenProp.m_ColorFormat );
	presParm.BackBufferCount		= 1;
									
	presParm.MultiSampleType		= D3DMULTISAMPLE_NONE;
									
	presParm.SwapEffect				= D3DSWAPEFFECT_COPY;
	presParm.hDeviceWindow			= m_hWnd;
	presParm.Windowed				= m_ScreenProp.m_bFullScreen ? FALSE : TRUE;
	presParm.EnableAutoDepthStencil = TRUE;
	presParm.AutoDepthStencilFormat = D3DFMT_D16;
	presParm.Flags					= 0;

    presParm.FullScreen_RefreshRateInHz			= m_ScreenProp.m_bFullScreen ? D3DPRESENT_RATE_DEFAULT : 0;
    presParm.FullScreen_PresentationInterval	= D3DPRESENT_INTERVAL_DEFAULT;
}

bool D3DRenderSystem::ResetDevice()
{
	InvalidateDeviceObjects();

	D3DPRESENT_PARAMETERS presParm;
	GetPresentParameters( presParm );
	
	//  set display fullscreen mode
	DEVMODE devMode;
	ZeroMemory( &devMode, sizeof( devMode ) );
	devMode.dmSize          = sizeof( DEVMODE );

	devMode.dmPelsWidth     = m_ScreenProp.m_Width;
	devMode.dmPelsHeight    = m_ScreenProp.m_Height;
	devMode.dmFields        = DM_PELSWIDTH | DM_PELSHEIGHT;
	
	DWORD flags = m_ScreenProp.m_bFullScreen ? CDS_FULLSCREEN : 0;
	int devModeRes = ChangeDisplaySettings( &devMode, flags );
	if (devModeRes != DISP_CHANGE_SUCCESSFUL)
	{
		assert( false );
		return false;
	}

	//  resetting device
	HRESULT hres = m_pDevice->Reset( &presParm );
	if (hres != S_OK) 
	{
		return false;
	}

	//  reinitialize all previously released resources
	RestoreDeviceObjects();

	D3DSURFACE_DESC bbDesc;
	LPDIRECT3DSURFACE8 pBackBuffer;
    m_pDevice->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
    pBackBuffer->GetDesc( &bbDesc );
    pBackBuffer->Release();


	DWORD numPasses;
	DX_CHK( m_pDevice->ValidateDevice( &numPasses ) );	
	return true;
} // D3DRenderSystem::ResetDevice

ScreenProp  D3DRenderSystem::GetScreenProp()
{
	return m_ScreenProp;
} // D3DRenderSystem::GetScreenProp

bool  D3DRenderSystem::SetScreenProp( const ScreenProp& prop )
{
    if (!m_pDevice)
    {
        Log.Error( "Trying to change screen properties with unitialized render device." );
        return false;
    }

	//  check if nothing changes
	ScreenProp rProp = prop;
	if (rProp.m_ColorFormat == cfBackBufferCompatible) rProp.m_ColorFormat = m_ScreenProp.m_ColorFormat;
	///if (rProp.equal( m_ScreenProp )) return true;
	m_ScreenProp = rProp;

	//  select maximal possible refresh rate
	assert( m_CurDeviceInfo );
	D3DModeInfo* modeInfo = m_CurDeviceInfo->FindMode( m_ScreenProp.m_Width, m_ScreenProp.m_Height );
	if (!modeInfo)
	{
		Log.Error( "Display mode %dx%d is not supported.", m_ScreenProp.m_Width, m_ScreenProp.m_Height );
		return false;
	}

	m_ScreenProp.m_RefreshRate = modeInfo->refreshRate;

	if (prop.m_ColorFormat != cfBackBufferCompatible)
	{
		m_ScreenProp.m_ColorFormat = prop.m_ColorFormat;
	}
	
	for (int i = 0; i < m_NotifyDestroy.size(); i++)
	{
		m_NotifyDestroy[i]->OnDestroyRenderSystem();
	}

	InvalidateDeviceObjects();
	ShutDeviceD3D();
	SAFE_RELEASE( m_pD3D );
    SetupDisplaySettings();
	InitD3D();
	InitDeviceD3D();
	RestoreDeviceObjects();

	for (int i = 0; i < m_NotifyDestroy.size(); i++)
	{
		m_NotifyDestroy[i]->OnCreateRenderSystem();
	}
	return true;
} // D3DRenderSystem::SetScreenProp		

void D3DRenderSystem::DisableLights()
{
	for (int i = 0; i < m_NumActiveLights; i++)
	{
		DX_CHK( m_pDevice->LightEnable( i, FALSE ) );
	}

	m_NumActiveLights = 0;
} // D3DRenderSystem::DisableLights

void D3DRenderSystem::SetMaterial( DWORD ambient, DWORD diffuse, DWORD specular, DWORD emissive, float power )
{
    D3DMATERIAL8 mtl;

    mtl.Ambient		= DwordToD3DCOLORVALUE( ambient  );
    mtl.Diffuse		= DwordToD3DCOLORVALUE( diffuse  );
    mtl.Specular	= DwordToD3DCOLORVALUE( specular );
    mtl.Emissive	= DwordToD3DCOLORVALUE( emissive );
    mtl.Power		= power;

    DX_CHK( m_pDevice->SetMaterial( &mtl ) );
} // D3DRenderSystem::SetMaterial

void D3DRenderSystem::SetDirLight( sg::DirectionalLight* pLight, int& index )
{
	if (!pLight) return;
	
	if (index == -1)
	{
		index = m_NumActiveLights;
		if (m_NumActiveLights == c_MaxLights) return;
	}

	m_NumActiveLights++;


	Vector3D dir = pLight->GetDir();

	D3DLIGHT8 light;
	light.Type		    = D3DLIGHT_DIRECTIONAL ;            
	light.Diffuse	    = DwordToD3DCOLORVALUE( pLight->GetDiffuse()  );
	light.Specular	    = DwordToD3DCOLORVALUE( pLight->GetSpecular() );        
	light.Ambient	    = DwordToD3DCOLORVALUE( pLight->GetAmbient()  );   

	light.Direction.x	= dir.x; 
	light.Direction.y	= dir.y;
	light.Direction.z	= dir.z;
	
	//  Now, folks, docs say that "for directional light position/range is ignored"
	//  That is not true, at least on some drivers. So do them happy:
	light.Range			= 100000.0f;
	light.Position.x	= 0.0f; 
	light.Position.y	= 0.0f;
	light.Position.z	= 0.0f;

	light.Attenuation0	= 1.0f;
	light.Attenuation1	= 0.0f;
	light.Attenuation2	= 0.0f;

	light.Falloff		= 1.0f;
	light.Theta			= c_PI;
	light.Phi			= c_PI;
	
	DX_CHK( m_pDevice->SetLight( index, &light ) );
	DX_CHK( m_pDevice->LightEnable( index, TRUE ) );
} // D3DRenderSystem::SetDirLight

void D3DRenderSystem::SetPointLight( sg::PointLight* pLight, int& index )
{
	if (!pLight) return;

	if (index == -1)
	{
		index = m_NumActiveLights;
		if (m_NumActiveLights == c_MaxLights) return;
	}

	m_NumActiveLights++;

	Vector3D pos = pLight->GetPos();
	Vector3D dir = pLight->GetDir();

	D3DLIGHT8 light;
	light.Type			= D3DLIGHT_POINT;            
	light.Diffuse		= DwordToD3DCOLORVALUE( pLight->GetDiffuse()  );
	light.Specular		= DwordToD3DCOLORVALUE( pLight->GetSpecular() );        
	light.Ambient		= DwordToD3DCOLORVALUE( pLight->GetAmbient()  );   

	light.Position.x	= pos.x; 
	light.Position.y	= pos.y;
	light.Position.z	= pos.z;

	light.Direction.x	= dir.x; 
	light.Direction.y	= dir.y;
	light.Direction.z	= dir.z;

	light.Range			= pLight->GetRange();   

	light.Attenuation0	= pLight->GetAttenuationA();
	light.Attenuation1	= pLight->GetAttenuationB();
	light.Attenuation2	= pLight->GetAttenuationC();

	light.Falloff		= 1.0f;
	light.Theta			= c_PI;
	light.Phi			= c_PI;

	DX_CHK( m_pDevice->SetLight( index, &light ) );
	DX_CHK( m_pDevice->LightEnable( index, TRUE ) );
} // D3DRenderSystem::SetPointLight

void D3DRenderSystem::SetSpotLight( sg::SpotLight* pLight, int& index ) 
{
	if (!pLight) return;
	
	if (index == -1)
	{
		index = m_NumActiveLights;
		if (m_NumActiveLights == c_MaxLights) return;
	}

	m_NumActiveLights++;

	Vector3D pos = pLight->GetPos();
	Vector3D dir = pLight->GetDir();

	D3DLIGHT8 light;
	light.Type		    = D3DLIGHT_DIRECTIONAL ;            
	light.Diffuse	    = DwordToD3DCOLORVALUE( pLight->GetDiffuse()  );
	light.Specular	    = DwordToD3DCOLORVALUE( pLight->GetSpecular() );        
	light.Ambient	    = DwordToD3DCOLORVALUE( pLight->GetAmbient()  );   

	light.Position.x	= pos.x; 
	light.Position.y	= pos.y;
	light.Position.z	= pos.z;

	light.Direction.x	= dir.x; 
	light.Direction.y	= dir.y;
	light.Direction.z	= dir.z;

	light.Range			= pLight->GetRange();   

	light.Attenuation0	= pLight->GetAttenuationA();
	light.Attenuation1	= pLight->GetAttenuationB();
	light.Attenuation2	= pLight->GetAttenuationC();

	light.Falloff		= pLight->GetConeFalloff();        
	light.Theta			= pLight->GetInnerCone();          
	light.Phi			= pLight->GetOuterCone();

	DX_CHK( m_pDevice->SetLight( index, &light ) );
	DX_CHK( m_pDevice->LightEnable( index, TRUE ) );
} // D3DRenderSystem::SetSpotLight

inline DWORD F2DW( float v )
{
	return *((DWORD*)&v);
}

D3DFOGMODE ConvertFogMode( sg::Fog::FogMode mode )
{
	if (mode == sg::Fog::fmLinear) return D3DFOG_LINEAR;
	if (mode == sg::Fog::fmExp) return D3DFOG_EXP;
	if (mode == sg::Fog::fmExp2) return D3DFOG_EXP2;
	return D3DFOG_NONE;
}

void D3DRenderSystem::SetFog( sg::Fog* pFog )
{
	if (pFog == NULL) 
	{
		DX_CHK( m_pDevice->SetRenderState( D3DRS_FOGENABLE, FALSE ) );
		return; 
	}

	DX_CHK( m_pDevice->SetRenderState( D3DRS_FOGENABLE,	TRUE ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_FOGCOLOR,	pFog->GetColor()		 ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_FOGSTART,	    F2DW(pFog->GetStart())	 ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_FOGEND,		F2DW(pFog->GetEnd())	 ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_FOGDENSITY,	F2DW(pFog->GetDensity()) ) );

	if (pFog->GetType() == sg::Fog::ftVertex)
	{
		DX_CHK( m_pDevice->SetRenderState( D3DRS_FOGVERTEXMODE, ConvertFogMode( pFog->GetMode() ) ) );
		DX_CHK( m_pDevice->SetRenderState( D3DRS_FOGTABLEMODE, D3DFOG_NONE ) );
		DX_CHK( m_pDevice->SetRenderState( D3DRS_RANGEFOGENABLE, F2DW(pFog->GetIsRangeBased()) ) );

	}
	else if (pFog->GetType() == sg::Fog::ftPixel)
	{
		DX_CHK( m_pDevice->SetRenderState( D3DRS_FOGTABLEMODE, ConvertFogMode( pFog->GetMode() ) ) );
		DX_CHK( m_pDevice->SetRenderState( D3DRS_FOGVERTEXMODE, D3DFOG_NONE ) );
	}

} // D3DRenderSystem::SetFog

void D3DRenderSystem::SetBumpTM( const Matrix3D& matr, int stage )
{
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_BUMPENVMAT00, F2DW( matr.e00 ) ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_BUMPENVMAT10, F2DW( matr.e10 ) ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_BUMPENVMAT01, F2DW( matr.e01 ) ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_BUMPENVMAT11, F2DW( matr.e11 ) ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_BUMPENVLSCALE,  F2DW(matr.e22) ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_BUMPENVLOFFSET, F2DW(matr.e20) ) );
    m_BumpMatrix[stage] = matr;
} // D3DRenderSystem::SetBumpTM

//  O== INTERNAL METHODS ========================================O
HRESULT D3DRenderSystem::InvalidateDeviceObjects()
{
	for (int i = 0; i < c_MaxTextureStages; i++)
	{
		SetTexture( 0, i );
	}

	SetShader( 0 );
	SetRenderTarget	( 0 );

	m_PrimitiveCache.InvalidateDeviceObjects();
	////m_TextureManager.InvalidateDeviceObjects();
	m_ShaderCache.InvalidateDeviceObjects();

	SAFE_DECREF( m_pBackBuffer );
	SAFE_DECREF( m_pDepthStencil );

	return S_OK;
} // D3DRenderSystem::InvalidateDeviceObjects

const int			c_NumDeviceTypes	= 2;
const char*			c_StrDeviceDescs[]	= {"HAL", "REF" };
const D3DDEVTYPE	c_DeviceTypes[]		= { D3DDEVTYPE_HAL, D3DDEVTYPE_REF };

static int SortModesCallback( const VOID* arg1, const VOID* arg2 )
{
    D3DDISPLAYMODE* p1 = (D3DDISPLAYMODE*)arg1;
    D3DDISPLAYMODE* p2 = (D3DDISPLAYMODE*)arg2;

    if( p1->Format > p2->Format )   return -1;
    if( p1->Format < p2->Format )   return +1;
    if( p1->Width  < p2->Width )    return -1;
    if( p1->Width  > p2->Width )    return +1;
    if( p1->Height < p2->Height )   return -1;
    if( p1->Height > p2->Height )   return +1;

    return 0;
}

int	D3DRenderSystem::GetNDisplayModes()
{
	return m_CurDeviceInfo->nModes;
} // D3DRenderSystem::GetNDisplayModes

void D3DRenderSystem::GetDisplayMode( int idx, int& width, int& height )
{
	D3DModeInfo& modeInfo = m_CurDeviceInfo->modes[idx];
	width	= modeInfo.width;
	height	= modeInfo.height;
} // D3DRenderSystem::GetDisplayMode

int D3DRenderSystem::CreateNormalMap( int texID, float amplitude )
{
//    if (!pDescr) return -1;
//    
//    char name[256];
//    sprintf( name, "%s_NormalMap", //m_TextureManager.GetTextureName( texID ) );
//    int dstID = //m_TextureManager.CreateTexture( name, *pDescr );
//    
//    DXTexture* pSrc  = //m_TextureManager.GetDXTex( texID );
//    DXTexture* pDest = //m_TextureManager.GetDXTex( dstID );
//    if (!pSrc || !pDest) return -1;
//
//    D3DXComputeNormalMap( pDest, pSrc, NULL, 0, D3DX_CHANNEL_RED, amplitude );
//    return dstID;
    return -1;
} // D3DRenderSystem::CreateNormalMap

/*---------------------------------------------------------------------------*/
/*	Func:	D3DRenderSystem::BuildDeviceList
/*	Desc:	Builds list of available devices and their display modes
/*	Remark:	Adopted from D3D samples 
/*---------------------------------------------------------------------------*/
void D3DRenderSystem::BuildDeviceList()
{
	assert( m_pD3D );
	int totalAdapters = m_pD3D->GetAdapterCount();
	m_NAdapters = 0;
    for (int i = 0; i < totalAdapters; i++)
    {
        // Fill in adapter info
        D3DAdapterInfo& curAdapter = m_AdapterList[i];
        m_pD3D->GetAdapterIdentifier( i, D3DENUM_NO_WHQL_LEVEL, &(curAdapter.adapterID) );
        m_pD3D->GetAdapterDisplayMode( i, &(curAdapter.desktopDisplayMode) );
        curAdapter.nDevices	= 0;

        // Enumerate all display modes on this adapter
        D3DDISPLAYMODE	modes[c_MaxDeviceDisplayModes];
        D3DFORMAT		formats[c_MaxDisplayFormatsInMode];
        int				numFormats  = 0;
        int				numModes	= 0;
		int				totalModes	= m_pD3D->GetAdapterModeCount( i );

        // Add the adapter's current desktop format to the list of formats
        formats[numFormats++] = curAdapter.desktopDisplayMode.Format;

        for (int j = 0; j < totalModes; j++)
        {
            //  getting next display mode
			D3DDISPLAYMODE dispMode;
            m_pD3D->EnumAdapterModes( i, j, &dispMode );
            
			if (dispMode.Width  < 640 || dispMode.Height < 400) continue;

			//  we select display mode with highest refresh rate available
            bool addMode = true;
			int k = 0;
			for (; k < numModes; k++)
            {
                if ((modes[k].Width  == dispMode.Width ) &&
                    (modes[k].Height == dispMode.Height) &&
                    (modes[k].Format == dispMode.Format))
				{
					addMode = false;
					if (modes[k].RefreshRate < dispMode.RefreshRate)
					{
						modes[k] = dispMode;
					}
				}
            }

            if (k == numModes && addMode)
            {
                modes[numModes].Width       = dispMode.Width;
                modes[numModes].Height      = dispMode.Height;
                modes[numModes].Format      = dispMode.Format;
                modes[numModes].RefreshRate = dispMode.RefreshRate;
                numModes++;

                //  check if the mode's format already exists
                int k = 0;
				for (; k < numFormats; k++)
                {
                    if (dispMode.Format == formats[k]) break;
                }

                if (k == numFormats) formats[numFormats++] = dispMode.Format;
            }
        }
		
        //  sort the list of display modes (by format, then width, then height)
        qsort( modes, numModes, sizeof(D3DDISPLAYMODE), SortModesCallback );

        //  add devices to adapter
        for (int k = 0; k < c_NumDeviceTypes; k++ )
        {
            D3DDeviceInfo& curDevice = curAdapter.devices[curAdapter.nDevices];
            curDevice.devType = c_DeviceTypes[k];
            m_pD3D->GetDeviceCaps( i, c_DeviceTypes[k], &curDevice.caps );
            curDevice.strDesc       = c_StrDeviceDescs[k];
            curDevice.nModes		= 0;
            curDevice.canDoWindowed = false;

            //  add all valid modes to device info
			for (int m = 0; m < numModes; m++)
            {
                for (int f = 0; f < numFormats; f++)
                {
                    if (modes[m].Format == formats[f])
                    {
                        //if( bFormatConfirmed[f] == TRUE )
                        {
                            //  add this mode to the device's list of valid modes
                            D3DModeInfo& curMode = curDevice.modes[curDevice.nModes];
							curMode.width				= modes[m].Width;
                            curMode.height				= modes[m].Height;
                            curMode.format				= modes[m].Format;
							curMode.refreshRate			= modes[m].RefreshRate;
                            /*
							curMode.behavior			= dwBehavior[f];
                            curMode.depthStencil		= fmtDepthStencil[f];
                            */
							curDevice.nModes++;
                        }
                    }
                }
            }

			if (curDevice.nModes > 0) curAdapter.nDevices++;

		
		}
        //  if valid devices were found, keep this adapter
        if (curAdapter.nDevices > 0) m_NAdapters++;
    }
	
} // D3DRenderSystem::BuildDeviceList

void D3DRenderSystem::DumpDeviceList( FILE* fp )
{
	fprintf( fp, "Dumping available device modes list...\n" );
	fprintf( fp, "Number of adapters: %d\n", m_NAdapters );
	for (int i = 0; i < m_NAdapters; i++)
	{
		m_AdapterList[i].Dump( fp );
	}
} // D3DRenderSystem::DumpDeviceList

DWORD D3DRenderSystem::CreateStateBlock( sg::StateBlock* pBlock )
{
	if (m_pDevice->BeginStateBlock() != S_OK) return 0xFFFFFFFF;
	DWORD id;
	pBlock->Node::Render();
	if (m_pDevice->EndStateBlock( &id ) != S_OK) return 0xFFFFFFFF;
	return id; 
} // D3DRenderSystem::CreateStateBlock

void D3DRenderSystem::SetRSBlock( sg::RenderStateBlock* pBlock	)
{
	DX_CHK( m_pDevice->SetRenderState( D3DRS_ZENABLE,				(DWORD)pBlock->m_bZEnable ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_FILLMODE,				(DWORD)pBlock->m_FillMode ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_SHADEMODE,				(DWORD)pBlock->m_ShadeMode ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE,			(DWORD)pBlock->m_bZWriteEnable ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, 		(DWORD)pBlock->m_bAlphaTestEnable ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_SRCBLEND,				(DWORD)pBlock->m_SrcBlend ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_DESTBLEND,				(DWORD)pBlock->m_DestBlend ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_CULLMODE,				(DWORD)pBlock->m_CullMode ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_ZFUNC,					(DWORD)pBlock->m_ZFunc ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_ALPHAREF,				(DWORD)pBlock->m_AlphaRef ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_ALPHAFUNC,				(DWORD)pBlock->m_AlphaFunc ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_DITHERENABLE,			(DWORD)pBlock->m_bDitherEnable ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,		(DWORD)pBlock->m_bAlphaBlendEnable ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_FOGENABLE,				(DWORD)pBlock->m_bFogEnable ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_SPECULARENABLE,		(DWORD)pBlock->m_bSpecularEnable ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_STENCILENABLE,			(DWORD)pBlock->m_bStencilEnable ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_STENCILFAIL,			(DWORD)pBlock->m_StencilFail ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_STENCILZFAIL,			(DWORD)pBlock->m_StencilZFail ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_STENCILPASS,			(DWORD)pBlock->m_StencilPass ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_STENCILFUNC,			(DWORD)pBlock->m_StencilFunc ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_STENCILREF,			(DWORD)pBlock->m_StencilRef ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_STENCILMASK,			(DWORD)pBlock->m_StencilMask ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_STENCILWRITEMASK,		(DWORD)pBlock->m_StencilWriteMask ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR,			(DWORD)pBlock->m_TextureFactor ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_LIGHTING,				(DWORD)pBlock->m_bLighting ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_AMBIENT,				(DWORD)pBlock->m_Ambient ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_COLORVERTEX,			(DWORD)pBlock->m_bColorVertex ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_LOCALVIEWER,			(DWORD)pBlock->m_bSpecularLocalViewer ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_NORMALIZENORMALS,		(DWORD)pBlock->m_bNormalizeNormals ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE,	(DWORD)pBlock->m_DiffuseMaterialSource ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_SPECULARMATERIALSOURCE,(DWORD)pBlock->m_SpecularMaterialSource ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_AMBIENTMATERIALSOURCE,	(DWORD)pBlock->m_AmbientMaterialSource ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_EMISSIVEMATERIALSOURCE,(DWORD)pBlock->m_EmissiveMaterialSource ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_VERTEXBLEND,			(DWORD)pBlock->m_VertexBlend ) );
	
	DWORD clip = 0;
	if (pBlock->m_bClipPlaneEnable[0]) clip |= 1 << 0;
	if (pBlock->m_bClipPlaneEnable[1]) clip |= 1 << 1;
	if (pBlock->m_bClipPlaneEnable[2]) clip |= 1 << 2;
	if (pBlock->m_bClipPlaneEnable[3]) clip |= 1 << 3;
	if (pBlock->m_bClipPlaneEnable[4]) clip |= 1 << 4;
	if (pBlock->m_bClipPlaneEnable[5]) clip |= 1 << 5;

	DX_CHK( m_pDevice->SetRenderState( D3DRS_CLIPPLANEENABLE,			clip ) );
	
	DX_CHK( m_pDevice->SetRenderState( D3DRS_SOFTWAREVERTEXPROCESSING,(DWORD)pBlock->m_bSoftwareVertexProcessing ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_COLORWRITEENABLE,		(DWORD)pBlock->m_ColorWriteEnable ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_TWEENFACTOR,				F2DW( pBlock->m_TweenFactor ) ) );
	DX_CHK( m_pDevice->SetRenderState( D3DRS_BLENDOP,					(DWORD)pBlock->m_Blendop ) );
} // D3DRenderSystem::SetRSBlock

void D3DRenderSystem::SetTSBlock( sg::TextureStateBlock* pBlock, int stage )
{
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_COLOROP,					(DWORD)pBlock->m_ColorOp ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_COLORARG1,				(DWORD)pBlock->m_ColorArg1 ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_COLORARG2,				(DWORD)pBlock->m_ColorArg2 ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_ALPHAOP,					(DWORD)pBlock->m_AlphaOp ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_ALPHAARG1, 				(DWORD)pBlock->m_AlphaArg1 ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_ALPHAARG2,				(DWORD)pBlock->m_AlphaArg2 ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_BUMPENVMAT00,			F2DW( pBlock->m_BumpEnvMat00 ) ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_BUMPENVMAT01,			F2DW( pBlock->m_BumpEnvMat01 ) ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_BUMPENVMAT10,			F2DW( pBlock->m_BumpEnvMat10 ) ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_BUMPENVMAT11,			F2DW( pBlock->m_BumpEnvMat11 ) ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_TEXCOORDINDEX,			(DWORD)pBlock->m_TexCoordIndex ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_ADDRESSU,				(DWORD)pBlock->m_AddressU ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_ADDRESSV,				(DWORD)pBlock->m_AddressV ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_BORDERCOLOR,				(DWORD)pBlock->m_BorderColor ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_MAGFILTER,				(DWORD)pBlock->m_MagFilter ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_MINFILTER,				(DWORD)pBlock->m_MinFilter ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_MIPFILTER,				(DWORD)pBlock->m_MipFilter ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_MIPMAPLODBIAS,			(DWORD)pBlock->m_MipmapLodBias ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_MAXMIPLEVEL,				(DWORD)pBlock->m_MaxMipLevel ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_MIPMAPLODBIAS,			F2DW( pBlock->m_MipmapLodBias ) ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_MAXANISOTROPY,			(DWORD)pBlock->m_MaxAnisotropy ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_BUMPENVLSCALE,			F2DW( pBlock->m_BumpEnvlScale ) ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_BUMPENVLOFFSET,			F2DW( pBlock->m_BumpEnvlOffset ) ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_TEXTURETRANSFORMFLAGS,	(DWORD)pBlock->m_TextureTransformFlags ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_ADDRESSW,				(DWORD)pBlock->m_AddressW ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_COLORARG0,				(DWORD)pBlock->m_ColorArg0 ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_ALPHAARG0,				(DWORD)pBlock->m_AlphaArg0 ) );
	DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_RESULTARG,				(DWORD)pBlock->m_ResultArg ) );
} // D3DRenderSystem::SetTSBlock

void D3DRenderSystem::AddClient( IDeviceClient* iNotify )
{
	m_NotifyDestroy.push_back( iNotify );
}

BYTE* D3DRenderSystem::LockVB( int vbID, int offset, int size )
{ 
    if (vbID < 0 || vbID >= m_VertexBuffers.size()) return NULL;
    const VertexBuffer* vb = m_VertexBuffers[vbID];
    if (!vb->m_pBuffer) return NULL;
    DWORD flags = D3DLOCK_NOSYSLOCK;	
    if (vb->m_bDynamic)  flags |= D3DLOCK_NOOVERWRITE;
    BYTE* pData = NULL;
    vb->m_pBuffer->Lock( offset, size, &pData, flags );
    return pData; 
} // D3DRenderSystem::LockVB

void D3DRenderSystem::UnlockVB( int vbID ) 
{
}

int D3DRenderSystem::CreateVB( int size, int vType, bool bDynamic ) 
{ 
    DWORD usage = D3DUSAGE_WRITEONLY;
    if (bDynamic) usage |= D3DUSAGE_DYNAMIC;
    IDirect3DVertexBuffer8* pBuffer = NULL;
    HRESULT hRes = m_pDevice->CreateVertexBuffer( size, usage, 0, D3DPOOL_DEFAULT, &pBuffer );
    VertexBuffer* vb = NULL;
    if (bDynamic) vb = new DynamicVB(); else vb = new StaticVB();
    vb->m_pBuffer = pBuffer;
    vb->m_MaxByte = size;
    vb->m_bDynamic = bDynamic; 
    m_VertexBuffers.push_back( vb );
    return m_VertexBuffers.size() - 1; 
} // D3DRenderSystem::CreateVB

bool D3DRenderSystem::DeleteVB( int vbID ) 
{ 
    return false; 
} // D3DRenderSystem::DeleteVB


WORD* D3DRenderSystem::LockIB( int ibID, int offset, int size )
{
    return NULL;
}

bool D3DRenderSystem::DiscardIB( int ibID )
{
    return false;
}

void D3DRenderSystem::UnlockIB( int ibID )
{

}

int D3DRenderSystem::CreateIB( int size, IndexSize idxSize, bool bDynamic )
{
    return -1;
}

bool D3DRenderSystem::DeleteIB( int ibID )
{
    return false;
} // D3DRenderSystem::DeleteIB

void D3DRenderSystem::Draw( int vbID, int firstVert, int nVert, const WORD* pIdx, int nIdx, PrimitiveType priType )
{

} // D3DRenderSystem::Draw

void D3DRenderSystem::Draw( int vType, void* pVert, int nVert, const WORD* pIdx, int nIdx, PrimitiveType priType )
{

} // D3DRenderSystem::Draw

void D3DRenderSystem::Draw( int vbID, int firstVert, int nVert, 
                            int ibID, int firstIdx, int nIdx, PrimitiveType priType )
{

} // D3DRenderSystem::Draw

void D3DRenderSystem::Draw( const Matrix4D* m_Transforms, int nInstances, 
                             int vbID, int firstVert, int nVert, 
                             int ibID, int firstIdx, int nIdx, PrimitiveType priType )
{

} // D3DRenderSystem::Draw

int	D3DRenderSystem::GetFontID( const char* name ) 
{
    return -1;
}

void D3DRenderSystem::DestroyFont( int fontID ) 
{

}

int	D3DRenderSystem::CreateFont( const char* name, int height, DWORD charset, bool bBold, bool bItalic )							
{
    return -1;
}

int	D3DRenderSystem::CreateFont( const char* texName, int charW, int charH ) 
{
    return -1;
}

int	D3DRenderSystem::GetStringWidth( int fontID, const char* str, int spacing ) 
{
    return 0;
}

int D3DRenderSystem::GetCharWidth( int fontID, BYTE ch ) 
{
    return 0;
}

int D3DRenderSystem::GetCharHeight( int fontID, BYTE ch ) 
{
    return 0; 
}

void D3DRenderSystem::SetCurrentFont( int fontID ) 
{

}

bool D3DRenderSystem::DrawString( const char* str, const Vector3D& pos, DWORD color, int spacing ) 
{
    return false;
}

bool D3DRenderSystem::DrawString3D( const char* str, const Vector3D& pos, DWORD color, int spacing ) 
{
    return false;
}

bool D3DRenderSystem::DrawChar( const Vector3D& pos, BYTE ch, DWORD color ) 
{
    return false;
}

bool D3DRenderSystem::DrawChar( const Vector3D& pos, const Rct& uv, DWORD color )
{
    return false;
}

bool D3DRenderSystem::DrawChar( const Vector3D& pos, const Rct& uv, float w, float h, DWORD color )
{
    return false;
}

void D3DRenderSystem::FlushText() 
{

} // D3DRenderSystem::FlushText

void D3DRenderSystem::DrawLine( float x1, float y1, float x2, float y2, float z, DWORD color1, DWORD color2 )
{

} // D3DRenderSystem::DrawLine

void D3DRenderSystem::DrawLine( const Vector3D& a, const Vector3D& b, DWORD color1, DWORD color2 )
{

} // D3DRenderSystem::DrawLine

void D3DRenderSystem::DrawRect( const Rct& rct, const Rct& uv, float z, DWORD ca, DWORD cb, DWORD cc, DWORD cd )
{

} // D3DRenderSystem::DrawRect

void D3DRenderSystem::DrawPoly( float ax, float ay, float bx, float by, float cx, float cy,
                               float au, float av, float bu, float bv, float cu, float cv )
{

} // D3DRenderSystem::DrawPoly

void D3DRenderSystem::DrawPoly( const Vector3D& a, const Vector3D& b, const Vector3D& c, DWORD acol, DWORD bcol, DWORD ccol,
                               float au, float av, float bu, float bv, float cu, float cv )
{

} // D3DRenderSystem::DrawPoly

void D3DRenderSystem::FlushPrim( bool bShaded )
{

} // D3DRenderSystem::FlushPrim


/*****************************************************************************/
/*	D3DModeInfo implementation
/*****************************************************************************/
void D3DModeInfo::Dump( FILE* fp )
{
	fprintf( fp, "%4d x %4d x %2d", width, height, format == D3DFMT_X8R8G8B8 ? 32 : 16 );
	fprintf( fp, " %3dHz ", refreshRate );

	switch (depthStencil)
	{
	case D3DFMT_D16:
		fprintf( fp, "(D16)\n" );
		break;
	case D3DFMT_D15S1:
	    fprintf( fp, "(D15S1)\n" );
		break;
	case D3DFMT_D24X8:
	    fprintf( fp, "(D24X8)\n" );
		break;
	case D3DFMT_D24S8:
	    fprintf( fp, "(D24S8)\n" );
	    break;
	case D3DFMT_D24X4S4:
	    fprintf( fp, "(D24X4S4)\n" );
	    break;
	case D3DFMT_D32:
	    fprintf( fp, "(D32)\n" );
	    break;
	default:
		fprintf( fp, "\n" );
	}
 
} // D3DModeInfo::Dump

/*****************************************************************************/
/*	D3DDeviceInfo implementation
/*****************************************************************************/
void D3DDeviceInfo::Dump( FILE* fp )
{
	fprintf( fp, "Device: %s NumModes: %d ", 
					strDesc, nModes );
	if (canDoWindowed) fprintf( fp, "WindowMode: Yes\n" );
		else fprintf( fp, "WindowMode: No\n" );
	for (int i = 0; i < nModes; i++)
	{
		modes[i].Dump( fp );
	}
} // D3DModeInfo::Dump

D3DModeInfo* D3DDeviceInfo::FindMode( int width, int height )
{
	for (int i = 0; i < nModes; i++)
	{
		if (modes[i].width == width && modes[i].height == height)
		{
			return &(modes[i]);
		}
	}
	return NULL;
} // D3DDeviceInfo::FindMode

/*****************************************************************************/
/*	D3DAdapterInfo implementation
/*****************************************************************************/
void D3DAdapterInfo::Dump( FILE* fp )
{
	fprintf( fp, "Adapter: %s\nDriver: %s (Ver %d)\nNum Devices: %d\n", 
					adapterID.Description, 
					adapterID.Driver, 
					adapterID.DriverVersion,
					nDevices );
	for (int i = 0; i < nDevices; i++)
	{
		devices[i].Dump( fp );
	}
} // D3DAdapterInfo::Dump

D3DDeviceInfo* D3DAdapterInfo::FindDevice( D3DDEVTYPE devType )
{
	for (int i = 0; i < nDevices; i++)
	{
		if (devices[i].devType == devType) return &(devices[i]);
	}
	return NULL;
} // D3DAdapterInfo::FindDevice

/*****************************************************************************/
/*	File:	sgApplication.h
/*	Author:	Ruslan Shestopalyuk
/*	Date:	10-29-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sgNode.h"
#include "sgNodePool.h"
#include "kInput.h"
#include "kIO.h"
#include "IWidgetManager.h"
#include "IPictureManager.h"
#include "rsRenderSystem.h"
#include <direct.h>


const char* GetRootDirectory()
{
    return "";
}

void ToRelativePath( char* path, int bufSize )
{
    char npath[_MAX_PATH];
    const char* pRoot = GetRootDirectory();

    int cPos = 0;
    while (tolower( pRoot[cPos] ) == tolower( path[cPos] )) cPos++;
    while (	path[cPos] == '\\' || 
        path[cPos] == '/') cPos++;
    strcpy( npath, path + cPos );
    strcpy( path, npath );
} // ToRelativePath

char g_PathBuf[_MAX_PATH];
const char* ToRelativePath( const char* path )
{
    strcpy( g_PathBuf, path );
    ToRelativePath( g_PathBuf, _MAX_PATH );
    return g_PathBuf;
}

const char* ParseFileName( const char* path )
{
    char dir	[_MAX_PATH];
    char drive	[_MAX_PATH];
    char file	[_MAX_PATH];
    char ext	[_MAX_PATH];
    _splitpath( path, drive, dir, file, ext );
    strcpy( g_PathBuf, file );
    return g_PathBuf;
} // ParseFileName

const char* ParseFilePathName( const char* path )
{
    char dir	[_MAX_PATH];
    char drive	[_MAX_PATH];
    char file	[_MAX_PATH];
    char ext	[_MAX_PATH];
    _splitpath( path, drive, dir, file, ext );
    strcpy( g_PathBuf, dir );
    strcat( g_PathBuf, file );
    return g_PathBuf;
} // ParseFileName

const char* ParseFileExtName( const char* path )
{
    char dir	[_MAX_PATH];
    char drive	[_MAX_PATH];
    char file	[_MAX_PATH];
    char ext	[_MAX_PATH];
    _splitpath( path, drive, dir, file, ext );
    strcpy( g_PathBuf, file );
    if (ext[0] != 0)
    {
        strcat( g_PathBuf, ext );
    }
    return g_PathBuf;
} // ParseFileExtName

/*
IRS->Init( Application::instance()->GetHInstance(), m_hWnd );
InitPictureManager();
InputDispatcher::SetCoreDispatcher( &InputManager::instance() );
InputDispatcher::Init();
getcwd( m_RootDirectory, _MAX_PATH );	
EnsureLastSlash( m_RootDirectory );
InitMath();
*/

//ATOM AppWindow::RegisterWindowClass( HINSTANCE hInstance )
//{
//	WNDCLASSEX wcex;
//
//	wcex.cbSize = sizeof(WNDCLASSEX); 
//
//	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
//	wcex.lpfnWndProc	= (WNDPROC) WndProc;
//	wcex.cbClsExtra		= 0;
//	wcex.cbWndExtra		= 0;
//	wcex.hInstance		= hInstance;
//	wcex.hIcon			= LoadIcon( hInstance, (LPCTSTR)m_IconResourceID );
//	wcex.hCursor		= NULL;
//	wcex.hbrBackground	= (HBRUSH) (COLOR_WINDOW + 1);
//	wcex.lpszMenuName	= NULL;
//	wcex.lpszClassName	= GetWindowClassName();
//	wcex.hIconSm		= LoadIcon( hInstance, (LPCTSTR)m_IconResourceID );
//
//	return RegisterClassEx( &wcex );
//} // AppWindow::RegisterWindowClass
//
//bool AppWindow::InitInstance( HINSTANCE hInstance, int nCmdShow )
//{
//	Rct rct = GetExtents();
//	m_hWnd = CreateWindowEx(	WS_EX_APPWINDOW,
//								GetWindowClassName(), 
//								GetWindowTitle(),
//								WS_POPUP, 
//								rct.x, 
//								rct.y, 
//								rct.w, 
//								rct.h,
//								NULL,
//								NULL, 
//								hInstance, 
//								NULL );
//	if (!m_hWnd)
//	{
//		return false;
//	}
//
//	ShowWindow( m_hWnd, nCmdShow );
//	UpdateWindow( m_hWnd );
//	SetCursor( LoadCursor( NULL, IDC_ARROW ) );
//	return true;
//} // AppWindow::InitInstance


