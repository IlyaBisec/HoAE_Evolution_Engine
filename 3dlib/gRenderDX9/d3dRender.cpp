/*****************************************************************************/
/*	File:	d3dRender.cpp
/*  Desc:	Render system interface implementation for DirectX9 
/*	Author:	Ruslan Shestopalyuk
/*	Date:	02.11.2004
/*****************************************************************************/
#include "gRenderPch.h"
#include <DxErr.h>
#include "resource.h"

#include "IMediaManager.h"
#include "IResourceManager.h"
#include "rsVertex.h"

#include "d3dTexture.h"
#include "d3dShaderFX.h"
#include "d3dIndexBuffer.h"
#include "d3dVertexBuffer.h"
#include "d3dFont.h"
#include "d3dRender.h"
#include "d3dAdapt.h"
#include <algorithm>


#include "stdafx.h"
#include "sgTransformNode.h"
#include "sgLight.h"

int OtherTime=0;
int NDX_calls=0;


/*
#include "kResFile.h"
#include "kIO.h"
#include "kIOHelpers.h"
#include "vMesh.h"
#include "sg.h"
#include "sgNode.h"
#include "sgGeometry.h"
#include "sgLight.h"
*/

//  Direct3D libraries
#pragma comment (lib, "d3d9.lib" )
#pragma comment (lib, "d3dx9.lib" )
#pragma comment (lib, "dxguid.lib" )
#pragma comment (lib, "dxerr9.lib" )

IRenderSystem* GetRenderSystem()
{
    static RenderSystemDX9 s_RSDX9;
    return &s_RSDX9;
}

DIALOGS_API IRenderSystem* IRS = GetRenderSystem();

IDirect3DDevice9* GetDirect3DDevice()
{
    return ((RenderSystemDX9*)GetRenderSystem())->GetDevice();
}

IDirect3DSurface9* GetDirect3DSurface( int texID )
{
    return ((RenderSystemDX9*)GetRenderSystem())->GetSurface( texID );
}

//INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
//{
//    InitCommonControls();
//    IRS->Init( hInst, NULL );
//    return g_RSDX9.Run();
//} // WinMain

/*****************************************************************************/
/*  RenderSystemDX9 implementation  
/*****************************************************************************/
RenderSystemDX9::RenderSystemDX9()
{
    RECT drct;
    SystemParametersInfo( SPI_GETWORKAREA, 0, &drct, 0 );

    m_strWindowTitle        = _T("3D-brush");
    m_d3dEnum.m_bUseZBuffer = TRUE;
    
    m_dwCreationWidth       = drct.right  - drct.left;
    m_dwCreationHeight      = drct.bottom - drct.top;
    m_CurrentFrame          = 0;
    IRS                     = this;
    m_bInited               = false;
    m_CurShader             = -1;
    m_CurPass               = 0;
    m_ViewPortZNear         = 0.0f;
    m_ViewPortZFar          = 1.0f;
    m_FPS                   = 0.0f;

    m_pBackBufferSurface    = NULL;
    m_pDepthStencilSurface  = NULL;

    m_FogDensity            = 0.00002f;
    m_TFactor               = 0xFFFFFFFF;
    m_ViewTM                = Matrix4D::identity;
    m_ProjTM                = Matrix4D::identity;
    m_WorldTM               = Matrix4D::identity;
    m_ShadersQuality        = 0;
	m_EnableBump			= true;

    for (int i = 0; i < c_MaxTextureStages; i++) m_CurTex[i] = -1;

	m_TimeIsOverridden		  = false;
	m_RefreshRateIsOverridden = false;
	m_EnablePostEffects		  = false;
	m_EnableBloom			  = false;
	m_MotionBlurAlpha		  = 0.6;
	m_BackbufferTextureID	  = 0xFFFFFFFF;
	m_BackbufferZID			  = 0xFFFFFFFF;
} // RenderSystemDX9::RenderSystemDX9

RenderSystemDX9::~RenderSystemDX9()
{
    for (int i = 0; i < m_Textures.size(); i++) delete m_Textures[i];
    for (int i = 0; i < m_Shaders.size(); i++)  delete m_Shaders[i];
    for (int i = 0; i < m_IBuffers.size(); i++) delete m_IBuffers[i];
    for (int i = 0; i < m_VBuffers.size(); i++) delete m_VBuffers[i];
    for (int i = 0; i < m_Fonts.size(); i++)    delete m_Fonts[i];

} // RenderSystemDX9::~RenderSystemDX9

//-----------------------------------------------------------------------------
// Name: OneTimeSceneInit()
// Desc: Paired with FinalCleanup().
//       The window has been created and the IDirect3D9 interface has been
//       created, but the device has not been created yet.  Here you can
//       perform application-related initialization and cleanup that does
//       not depend on a device.
//-----------------------------------------------------------------------------
HRESULT RenderSystemDX9::OneTimeSceneInit()
{
    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: InitDeviceObjects()
// Desc: Paired with DeleteDeviceObjects()
//       The device has been created.  Resources that are not lost on
//       Reset() can be created here -- resources in D3DPOOL_MANAGED,
//       D3DPOOL_SCRATCH, or D3DPOOL_SYSTEMMEM.  Image surfaces created via
//       CreateImageSurface are never lost and can be created here.  Vertex
//       shaders and pixel shaders can also be created here as they are not
//       lost on Reset().
//-----------------------------------------------------------------------------
HRESULT RenderSystemDX9::InitDeviceObjects()
{
    CreateVTypeTable();
    CreateIB( "SharedDynamic",  c_DynIBufferBytes,      isWORD, true    );
    CreateIB( "SharedStatic",   c_StaticIBufferBytes,   isWORD, false   );
    CreateIB( "Quads",          c_QuadIBufferBytes,     isWORD, false   );
    CreateVB( "SharedDynamic",  c_DynVBufferBytes,      -1,     true    );
    CreateVB( "SharedStatic",   c_StaticVBufferBytes,   -1,     false   );

    m_ViewPort.x = 0.0f;
    m_ViewPort.y = 0.0f;
    m_ViewPort.w = m_d3dpp.BackBufferWidth;
    m_ViewPort.h = m_d3dpp.BackBufferHeight;

    SAFE_RELEASE(m_pBackBufferSurface);
    SAFE_RELEASE(m_pDepthStencilSurface);
    DX_CHK( m_pDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBufferSurface ) );
    DX_CHK( m_pDevice->GetDepthStencilSurface( &m_pDepthStencilSurface ) );
    return S_OK;
} // RenderSystemDX9::InitDeviceObjects

//-----------------------------------------------------------------------------
// Name: RestoreDeviceObjects()
// Desc: Paired with InvalidateDeviceObjects()
//       The device exists, but may have just been Reset().  Resources in
//       D3DPOOL_DEFAULT and any other device state that persists during
//       rendering should be set here.  Render states, matrices, textures,
//       etc., that don't change during rendering can be set once here to
//       avoid redundant state setting during Render()
//-----------------------------------------------------------------------------
HRESULT RenderSystemDX9::RestoreDeviceObjects()
{
    HRESULT hr = S_OK;
    for (int i = 0; i < m_Textures.size();  i++) m_Textures[i]->RestoreDeviceObjects();
    for (int i = 0; i < m_Shaders.size();   i++) m_Shaders[i]->RestoreDeviceObjects();
    for (int i = 0; i < m_VBuffers.size();  i++) m_VBuffers[i]->RestoreDeviceObjects();
    for (int i = 0; i < m_IBuffers.size();  i++) m_IBuffers[i]->RestoreDeviceObjects();
    for (int i = 0; i < m_Fonts.size();     i++) m_Fonts[i]->RestoreDeviceObjects();

    m_ViewPort.x = 0.0f;
    m_ViewPort.y = 0.0f;
    m_ViewPort.w = m_d3dpp.BackBufferWidth;
    m_ViewPort.h = m_d3dpp.BackBufferHeight;

    SAFE_RELEASE(m_pBackBufferSurface);
    SAFE_RELEASE(m_pDepthStencilSurface);
    DX_CHK( m_pDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBufferSurface ) );
    DX_CHK( m_pDevice->GetDepthStencilSurface( &m_pDepthStencilSurface ) );

    FillQuadIndexBuffer();

    for (int i = 0; i < m_DeviceClients.size(); i++)
    {
        m_DeviceClients[i]->OnDestroyRS();
    }

    return hr;
} // RenderSystemDX9::RestoreDeviceObjects

//-----------------------------------------------------------------------------
// Name: InvalidateDeviceObjects()
// Desc: Invalidates device objects.  Paired with RestoreDeviceObjects()
//-----------------------------------------------------------------------------
HRESULT RenderSystemDX9::InvalidateDeviceObjects()
{
    for (int i = 0; i < m_Textures.size();  i++) m_Textures[i]->InvalidateDeviceObjects();
    for (int i = 0; i < m_Shaders.size();   i++) m_Shaders[i]->InvalidateDeviceObjects();
    for (int i = 0; i < m_VBuffers.size();  i++) m_VBuffers[i]->InvalidateDeviceObjects();
    for (int i = 0; i < m_IBuffers.size();  i++) m_IBuffers[i]->InvalidateDeviceObjects();
    for (int i = 0; i < m_Fonts.size();     i++) m_Fonts[i]->InvalidateDeviceObjects();

    SAFE_RELEASE( m_pBackBufferSurface );
    SAFE_RELEASE( m_pDepthStencilSurface );

    return S_OK;
} // RenderSystemDX9::InvalidateDeviceObjects

//-----------------------------------------------------------------------------
// Name: DeleteDeviceObjects()
// Desc: Paired with InitDeviceObjects()
//       Called when the app is exiting, or the device is being changed,
//       this function deletes any device dependent objects.  
//-----------------------------------------------------------------------------
HRESULT RenderSystemDX9::DeleteDeviceObjects()
{
    for (int i = 0; i < m_Textures.size();  i++) m_Textures[i]->DeleteDeviceObjects();
    for (int i = 0; i < m_Shaders.size();   i++) m_Shaders[i]->DeleteDeviceObjects();
    for (int i = 0; i < m_VBuffers.size();  i++) m_VBuffers[i]->DeleteDeviceObjects();
    for (int i = 0; i < m_IBuffers.size();  i++) m_IBuffers[i]->DeleteDeviceObjects();
    for (int i = 0; i < m_Fonts.size();     i++) m_Fonts[i]->DeleteDeviceObjects();

    SAFE_RELEASE( m_pBackBufferSurface );
    SAFE_RELEASE( m_pDepthStencilSurface );

    ClearVTypeTable();
    return S_OK;
} // RenderSystemDX9::DeleteDeviceObjects

//-----------------------------------------------------------------------------
// Name: FinalCleanup()
// Desc: Paired with OneTimeSceneInit()
//       Called before the app exits, this function gives the app the chance
//       to cleanup after itself.
//-----------------------------------------------------------------------------
HRESULT RenderSystemDX9::FinalCleanup()
{
    return S_OK;
} // RenderSystemDX9::FinalCleanup

//-----------------------------------------------------------------------------
// Name: ConfirmDevice()
// Desc: Called during device initialization, this code checks the device
//       for some minimum set of capabilities
//-----------------------------------------------------------------------------
HRESULT RenderSystemDX9::ConfirmDevice( D3DCAPS9* pCaps, DWORD dwBehavior,
                                          D3DFORMAT adapterFormat, 
                                          D3DFORMAT backBufferFormat )
{
    // Debugging vertex shaders requires either REF or software vertex processing 
    // and debugging pixel shaders requires REF.  
    #ifdef DEBUG_VS
        if( pCaps->DeviceType != D3DDEVTYPE_REF && 
            (dwBehavior & D3DCREATE_SOFTWARE_VERTEXPROCESSING) == 0 )
            return E_FAIL;
    #endif
    #ifdef DEBUG_PS
        if( pCaps->DeviceType != D3DDEVTYPE_REF )
            return E_FAIL;
    #endif

    if (dwBehavior & D3DCREATE_PUREDEVICE) return E_FAIL;
    // No fallback, so need ps1.1
    if (pCaps->PixelShaderVersion < D3DPS_VERSION(1,1))
    {
        return E_FAIL;
    }

    // If device doesn't support 1.1 vertex shaders in HW, switch to SWVP.
    if (pCaps->VertexShaderVersion < D3DVS_VERSION(1,1))
    {
        if ((dwBehavior & D3DCREATE_SOFTWARE_VERTEXPROCESSING ) == 0)
        {
            return E_FAIL;
        }
    }

    return S_OK;
} // RenderSystemDX9::ConfirmDevice

void InitMath();
void RenderSystemDX9::Init( HINSTANCE hInst, HWND hWnd )
{
    if (m_bInited)
    {
        Log.Warning( "RenderSystemDX9: trying to initialize twice." );
        return;
    }

    HRESULT hr = Create( hInst, hWnd );
    bool sInst = SupportsInstancing();
    m_bInited = (hr == S_OK);
    
    InitMath();

    IRM->MountDataSource( "Shaders",        "Shaders"   );
    IRM->MountDataSource( "Models",         "Models"    );
    IRM->MountDataSource( "Textures",       "Textures"  );
    IRM->MountDataSource( "Scripts",        "Scripts"   );
} // RenderSystemDX9::Init

void RenderSystemDX9::ShutDown()
{
    Cleanup3DEnvironment();
} // RenderSystemDX9::ShutDown

bool RenderSystemDX9::SetScreenProp( const ScreenProp& prop ) 
{ 
    if (prop.m_Width == 0 || prop.m_Height == 0) return false;
    if (prop.m_bFullScreen)
    {
        m_d3dSettings.m_bIsWindowed                 = true;
        m_d3dSettings.Fullscreen_DisplayMode.Width  = prop.m_Width;
        m_d3dSettings.Fullscreen_DisplayMode.Height = prop.m_Height;
        FindBestFullscreenMode( true, false );
    }
    else
    {
        m_d3dSettings.m_bIsWindowed     = true;
        m_d3dSettings.Windowed_Width    = prop.m_Width;
        m_d3dSettings.Windowed_Height   = prop.m_Height;

        SetMonitorResolution();
        FindBestWindowedMode( true, false );
    }

    return (HandlePossibleSizeChange() != S_OK);
} // RenderSystemDX9::SetScreenProp

//-----------------------------------------------------------------------------------
// RenderSystemDX9::AddScreenResolution
//-----------------------------------------------------------------------------------
void RenderSystemDX9::AddScreenResolution(const int XRes, const int YRes, const int RR) {
	D3DAdapterInfo *pAdapter = m_d3dSettings.PAdapterInfo();
	D3DDISPLAYMODE dm;
	dm.Width = XRes;
	dm.Height = YRes;
	dm.RefreshRate = RR;
	dm.Format = D3DFMT_R8G8B8;
	pAdapter->m_DisplayModes.push_back(dm);
} // RenderSystemDX9::AddScreenResolution

const char* GetDispChangeErrorDesc( LONG result );
bool RenderSystemDX9::SetMonitorResolution()
{
    DEVMODE devMode;
    ZeroMemory( &devMode, sizeof( devMode ) );
    devMode.dmSize              = sizeof( DEVMODE );

    int w = INT_MAX;
    int h = INT_MAX;
    int hz = 0;

    int dw = m_d3dSettings.Windowed_Width;
    int dh = m_d3dSettings.Windowed_Height;

    m_BestDisplayModes.clear();

    D3DAdapterInfo* pAd = m_d3dSettings.PAdapterInfo();
    for (int i = 0; i < pAd->m_DisplayModes.size(); i++)
    {
        D3DDISPLAYMODE& dmode = pAd->m_DisplayModes[i];
        if ((dmode.Width >= dw && dmode.Height >= dh) && 
            (dmode.Width <= w && dmode.Height <= h && dmode.RefreshRate >= hz))
        {
            w  = dmode.Width;
            h  = dmode.Height; 
            hz = dmode.RefreshRate;
        }
    }

    if (w == INT_MAX || h == INT_MAX || hz == INT_MAX)
    {
        return false;
    }
    
    D3DFORMAT bbFormat = m_d3dSettings.BackBufferFormat();

    devMode.dmPelsWidth         = w;
    devMode.dmPelsHeight        = h;
    devMode.dmBitsPerPel        = (bbFormat == D3DFMT_R5G6B5) ? 16 : 32;
	devMode.dmDisplayFrequency  = m_RefreshRateIsOverridden ? m_HzOverriddenRefreshRate : hz;
    devMode.dmFields            =   DM_PELSWIDTH | 
                                    DM_PELSHEIGHT | 
                                    DM_DISPLAYFREQUENCY |
                                    DM_BITSPERPEL;

    DWORD displayModeFlags = 0;
    if (!m_d3dSettings.m_bIsWindowed) displayModeFlags |= CDS_FULLSCREEN;
    int devModeRes = ChangeDisplaySettings( &devMode, displayModeFlags );
    DWORD winErr = GetLastError();
    if (devModeRes != DISP_CHANGE_SUCCESSFUL)
    {
        Log.Error( GetDispChangeErrorDesc( devModeRes ) );
        return false;
    }

    /*if (m_ScreenProp.m_bCoverDesktop)
    {
        RECT drct;
        HWND hDesk = GetDesktopWindow();
        ::GetWindowRect( hDesk, &drct );
        SystemParametersInfo( SPI_GETWORKAREA, 0, &drct, 0 );
        m_ScreenProp.m_Width  = drct.right  - drct.left;
        m_ScreenProp.m_Height = drct.bottom - drct.top;
    }*/

    int wX = 0;
    int wY = 0;
    int wW = m_d3dSettings.Windowed_Width;
    int wH = m_d3dSettings.Windowed_Height;

    SetWindowPos( m_hWnd, HWND_NOTOPMOST, wX, wY, wW, wH, SWP_SHOWWINDOW );
    ShowWindow( m_hWnd, SW_SHOW );

    RECT	wrct;
    ::GetWindowRect( m_hWnd, &wrct );
    return true;
} // RenderSystemDX9::SetMonitorResolution

ScreenProp RenderSystemDX9::GetScreenProp() 
{ 
	D3DDISPLAYMODE dp;
	m_pDevice->GetDisplayMode(0,&dp);
	ScreenProp sp;
	sp.m_Width=dp.Width;
	sp.m_Height=dp.Height;
	sp.m_RefreshRate=dp.RefreshRate;
	sp.m_ColorFormat=cfRGB888;
    return sp; 
}

void RenderSystemDX9::SetViewTM( const Matrix4D& vmatr )
{
    m_ViewTM = vmatr;
    __beginT()
    DX_CHK( m_pDevice->SetTransform( D3DTS_VIEW, (D3DMATRIX*)&vmatr ) );
    __endT(OtherTime);
}

void RenderSystemDX9::SetProjTM( const Matrix4D& pmatr )
{
    m_ProjTM = pmatr;
    __beginT();
    DX_CHK( m_pDevice->SetTransform( D3DTS_PROJECTION, (D3DMATRIX*)&pmatr ) );
    __endT(OtherTime);
} // RenderSystemDX9::SetProjTM
int TimeInSWTM=0;
void RenderSystemDX9::SetWorldTM( const Matrix4D& wmatr )
{
    m_WorldTM = wmatr;

    __beginT();

    DX_CHK( m_pDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&wmatr ) );

    __endT(TimeInSWTM);

    m_WTM_is1 = false;
} // RenderSystemDX9::SetWorldTM

void RenderSystemDX9::SetWorldViewProjTM( const Matrix4D& wmatr )
{
	m_WorldViewProjTM = wmatr;	
} // RenderSystemDX9::SetWorldViewProjTM

void RenderSystemDX9::ResetWorldTM()
{
    if(m_WTM_is1)return;
    m_WorldTM = Matrix4D::identity;

    __beginT();

    DX_CHK( m_pDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&Matrix4D::identity ) );

    __endT(TimeInSWTM);

    m_WTM_is1 = true;
} // RenderSystemDX9::ResetWorldTM

void RenderSystemDX9::SetTextureTM( const Matrix4D& tmatr, int stage ) 
{
    m_TextureTM[stage] = tmatr;
    __beginT();
    DX_CHK( m_pDevice->SetTransform( (D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0 + stage), (D3DMATRIX*)&tmatr ) );
    __endT(OtherTime);
} // RenderSystemDX9::SetTextureTM

void RenderSystemDX9::SetBumpTM( const Matrix3D& bmatr, int stage ) 
{
    __beginT();
    DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_BUMPENVMAT00, F2DW( bmatr.e00 ) ) );
    DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_BUMPENVMAT10, F2DW( bmatr.e10 ) ) );
    DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_BUMPENVMAT01, F2DW( bmatr.e01 ) ) );
    DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_BUMPENVMAT11, F2DW( bmatr.e11 ) ) );
    DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_BUMPENVLSCALE,  F2DW(bmatr.e22) ) );
    DX_CHK( m_pDevice->SetTextureStageState( stage, D3DTSS_BUMPENVLOFFSET, F2DW(bmatr.e20) ) );
    __endT(OtherTime);
    m_BumpTM[stage] = bmatr;
} // RenderSystemDX9::SetBumpTM(

void RenderSystemDX9::GetClientSize( int& width, int& height ) 
{

}

void RenderSystemDX9::AddClient( IDeviceClient* iNotify ) 
{
    m_DeviceClients.push_back( iNotify );
} // RenderSystemDX9::AddClient 

void RenderSystemDX9::ClearDevice( DWORD color, bool bColor, bool bDepth, bool bStencil ) 
{
    DWORD flags = 0;
    if (bColor) flags |= D3DCLEAR_TARGET;
    if (bDepth) flags |= D3DCLEAR_ZBUFFER;
    if (bStencil) flags |= D3DCLEAR_STENCIL;

    __beginT();
    DX_CHK( m_pDevice->Clear( 0, NULL, flags, color, 1.0, 0 ) );
    __endT(OtherTime);
} // RenderSystemDX9::ClearDevice

bool RenderSystemDX9::SetCursor( int texID, const Rct& rctOnTex, int hotspotX, int hotspotY ) 
{
    return false; 
}

bool RenderSystemDX9::UpdateCursor( int x, int y, bool drawNow ) 
{ 
    assert( m_pDevice );
    DWORD flags = drawNow ? D3DCURSOR_IMMEDIATE_UPDATE : 0;
    m_pDevice->SetCursorPosition( x, y, flags );
    return true;
} // RenderSystemDX9::UpdateCursor

void RenderSystemDX9::ShowCursor( bool bShow ) 
{
    __beginT();
    DX_CHK( m_pDevice->ShowCursor( bShow ? TRUE : FALSE ) );
    __endT(OtherTime);
} // RenderSystemDX9::ShowCursor

bool RenderSystemDX9::StartFrame() 
{
    if (!m_bActive) return false;
    HRESULT hr;
    if (m_bDeviceLost)
    {
        // Test the cooperative level to see if it's okay to render
        if (FAILED( hr = m_pDevice->TestCooperativeLevel() ))
        {
            // If the device was lost, do not render until we get it back
            if (D3DERR_DEVICELOST == hr) return false;

            // Check if the device needs to be reset.
            if (D3DERR_DEVICENOTRESET == hr)
            {
                // If we are windowed, read the desktop mode and use the same format for
                // the back buffer
                if (m_bWindowed)
                {
                    D3DAdapterInfo* pAdapterInfo = m_d3dSettings.PAdapterInfo();
                    m_pD3D->GetAdapterDisplayMode( pAdapterInfo->m_Ordinal, &m_d3dSettings.Windowed_DisplayMode );
                    m_d3dpp.BackBufferFormat = m_d3dSettings.Windowed_DisplayMode.Format;
                }
                if (FAILED( hr = Reset3DEnvironment() )) return false;
            }
            return false;
        }
        m_bDeviceLost = false;
    }


    // Get the app's time, in seconds. Skip rendering if no time elapsed
    FLOAT fAppTime        = DXUtil_Timer( TIMER_GETAPPTIME );
    FLOAT fElapsedAppTime = DXUtil_Timer( TIMER_GETELAPSEDTIME );
    if ((0.0f == fElapsedAppTime) && m_bFrameMoving) return true;
    m_fTime        = fAppTime;
    m_fElapsedTime = fElapsedAppTime;

    m_ViewPort.x = 0.0f;
    m_ViewPort.y = 0.0f;
    m_ViewPort.w = m_d3dpp.BackBufferWidth;
    m_ViewPort.h = m_d3dpp.BackBufferHeight;
    
    // DX_CHK( m_pDevice->BeginScene() );
    //static int s_shID = IRS->GetShaderID( "lines.fx" );
    //SetShader( s_shID );

    SetTextureFactor( 0xFFFFFFFF );
    SetTextureFactor( 0x00FFFFFF );
    SetTextureFactor( 0xFFFFFFFF );	

    return true;
} // RenderSystemDX9::StartFrame

bool g_bKangaroo = false;
void RenderSystemDX9::EndFrame() 
{
    void RenderUI();
	SetShader( -1 );

    if (g_bKangaroo) RenderUI();

    m_CurrentFrame++;
    Flush();

    SetShader( -1 );

    static Timer s_FPSTimer;
    static float s_Time = s_FPSTimer.seconds();
    float dt = s_FPSTimer.seconds() - s_Time;
    m_FPS = 1.0f/dt;
    s_Time = s_FPSTimer.seconds();

    //DX_CHK( m_pDevice->EndScene() );
	
    // Show the frame on the primary surface.
    HRESULT hr = m_pDevice->Present( NULL, NULL, NULL, NULL );
    if (D3DERR_DEVICELOST == hr) m_bDeviceLost = true;
} // RenderSystemDX9::EndFrame

bool RenderSystemDX9::ReloadShaders() 
{ 
    int nS = m_Shaders.size();
    for (int i = 0; i < nS; i++)
    {
        m_Shaders[i]->Reload();
    }
    return true; 
} // RenderSystemDX9::ReloadShaders

bool RenderSystemDX9::ReloadTextures() 
{ 
    int nT = m_Textures.size();
    for (int i = 0; i < nT; i++)
    {
        m_Textures[i]->Reload();
    }
    return true; 
} // RenderSystemDX9::ReloadTextures

int RenderSystemDX9::CreateTexture( const char* texName, int width, int height, 
                                    ColorFormat clrFormat, int nMips, TextureMemoryPool memPool, 
                                    bool bRenderTarget, DepthStencilFormat dsFormat,
                                    bool bDynamic )
{
    int nTex = m_Textures.size();
    TextureDX9* pTex = NULL;
    bool bFound = false;
    for (int i = 0; i < nTex; i++)
    {
        pTex = m_Textures[i];
        if (!strcmp( pTex->GetName(), texName ))
        {
            pTex->DeleteDeviceObjects();
            bFound = true;
            break;
        }
    }
    if (!bFound) 
    {
        pTex = new TextureDX9( m_pDevice );
        pTex->SetID( m_Textures.size() );
        m_Textures.push_back( pTex );
        pTex->SetName( texName );
    }
    pTex->Create( width, height, clrFormat, nMips, memPool, bRenderTarget, dsFormat, bDynamic );
    return pTex->GetID(); 
} // RenderSystemDX9::CreateTexture

void RenderSystemDX9::SetTexture( int texID, int stage, bool bCache ) 
{
	//if(texID==m_CurTex[stage])return;
    if (texID < 0 || texID >= m_Textures.size() /*|| texID == m_CurTex[stage]*/) return;	
    m_CurTex[stage] = texID;
    //  FIXME:
    /*if (!bCache) */m_Textures[texID]->Bind( stage );
} // RenderSystemDX9::SetTexture

IDirect3DSurface9* RenderSystemDX9::GetSurface( int texID )
{
     if (texID < 0 || texID >= m_Textures.size()) return NULL;
     return m_Textures[texID]->GetSurface( 0 );
} // RenderSystemDX9::GetSurface

int	RenderSystemDX9::GetTexture( int stage ) 
{ 
    if (stage < 0 || stage >= c_MaxTextureStages) return -1;
    return m_CurTex[stage]; 
} // RenderSystemDX9::GetTexture

bool RenderSystemDX9::SaveTexture( int texID, const char* fname ) 
{
    if (texID < 0 || texID >= m_Textures.size()) return false;
    return m_Textures[texID]->SaveToFile( fname );
} // RenderSystemDX9::SaveTexture

void RenderSystemDX9::CopyTexture( int destID, int srcID, const Rct* rct, int nRect ) 
{ 
    if (destID < 0 || destID >= m_Textures.size()) return;
    if (srcID < 0 || srcID >= m_Textures.size()) return;

    Rct rect;
    if (!rct)
    {
        rect.x = 0;
        rect.y = 0;
        rect.w = m_Textures[srcID]->GetWidth();
        rect.h = m_Textures[srcID]->GetHeight();
        nRect  = 1;
        rct    = &rect;
    }

    IDirect3DTexture9* pSrc = m_Textures[srcID]->GetTexture2D();
    IDirect3DTexture9* pDst = m_Textures[destID]->GetTexture2D();
    if (!pSrc || !pDst) return;

    for (int i = 0; i < nRect; i++)
    {
        RECT wrct;
        wrct.left    = rct[i].x;
        wrct.top     = rct[i].y;
        wrct.right   = rct[i].GetRight();
        wrct.bottom  = rct[i].GetBottom();
        pDst->AddDirtyRect( &wrct );
    }

    __beginT();
    DX_CHK( m_pDevice->UpdateTexture( pSrc, pDst ) );
    __endT(OtherTime);
} //RenderSystemDX9::CopyTexture

void RenderSystemDX9::CreateMipLevels( int texID ) 
{
    if (texID <= 0 || texID >= m_Textures.size()) return;
    TextureDX9& tex = *(m_Textures[texID]);
    DX_CHK( D3DXFilterTexture( tex.GetTexture2D(), NULL, 
                                0, 
                                D3DX_FILTER_TRIANGLE | 
                                D3DX_FILTER_DITHER   | 
                                D3DX_FILTER_MIRROR_U |
                                D3DX_FILTER_MIRROR_V ) );

} // RenderSystemDX9::CreateMipLevels

int RenderSystemDX9::CreateNormalMap( int texID, float amplitude ) 
{ 
    if (texID <= 0 || texID >= m_Textures.size()) return -1;
    char name[256];
    TextureDX9& tex = *(m_Textures[texID]);
    sprintf( name, "%s_normalmap", tex.GetName() );
    int dstID = CreateTexture( name, tex.GetWidth(), tex.GetHeight(), cfARGB8888, 1, tmpDefault );
    if (dstID == -1) return -1;
	
	//int src_pitch;
	//BYTE* src = IRS->LockTexBits(texID,src_pitch);
	//int dst_pitch;
	//BYTE* dst = IRS->LockTexBits(dstID,dst_pitch);

	TextureDX9& dtex = *(m_Textures[dstID]);
    
    IDirect3DTexture9* pSrc  = tex.GetTexture2D();
    IDirect3DTexture9* pDest = dtex.GetTexture2D();
    if (!pSrc || !pDest) return -1;

    DX_CHK( D3DXComputeNormalMap( pDest, pSrc, NULL, 0, D3DX_CHANNEL_RED, amplitude ) );
	SaveTexture(dstID,"temp.dds");
    return dstID; 
} // RenderSystemDX9::CreateNormalMap

// RenderSystemDX9::RegCallbackOnGetTextureID
void RenderSystemDX9::RegCallbackOnGetTextureID(IRenderSystem::OnGetTextureID *Fn) {
    int c;
    for(c = 0; c < m_OnGetTextureIDCallbacks.size(); c++) {
		if(m_OnGetTextureIDCallbacks[c] == Fn) {
			break;
		}
	}
	if(c == m_OnGetTextureIDCallbacks.size()) {
		m_OnGetTextureIDCallbacks.push_back(Fn);
	}
} // RenderSystemDX9::RegCallbackOnGetTextureID

// RenderSystemDX9::RegCallbackOnLoadTexture
void RenderSystemDX9::RegCallbackOnLoadTexture(IRenderSystem::OnLoadTexture *Fn) {
    int c;
    for(c = 0; c < m_OnLoadTextureCallbacks.size(); c++) {
		if(m_OnLoadTextureCallbacks[c] == Fn) {
			break;
		}
	}
	if(c == m_OnLoadTextureCallbacks.size()) {
		m_OnLoadTextureCallbacks.push_back(Fn);
	}
} // RenderSystemDX9::RegCallbackOnLoadTexture

int	RenderSystemDX9::GetTextureID( const char* texName ) 
{ 
    if (!texName || texName[0] == 0) return -1;
    int nTex = m_Textures.size();
    for (int i = 0; i < nTex; i++)
    {
        const ITexture* pTex = m_Textures[i];
		if (!stricmp( pTex->GetName(), texName )) {
			// Tex is precached. So, is there any OnGetTextureID callbacks we should call?
			for(int c = 0; c < m_OnGetTextureIDCallbacks.size(); c++) {
				m_OnGetTextureIDCallbacks[c](texName);
			}
			return i;
		}
    }
    //  load texture 
    TextureDX9* pTex = new TextureDX9( m_pDevice );
    pTex->SetID( m_Textures.size() );
    pTex->SetName( texName );
    FilePath path; path.GetCWD();
    pTex->SetSearchPath( path.GetFullPath() );
    pTex->LoadHeader();
    
    m_Textures.push_back( pTex );
    path.SetCWD();
	// Tex is loaded. Is there any OnLoadTexture callbacks?
	for(int c = 0; c < m_OnLoadTextureCallbacks.size(); c++) {
		m_OnLoadTextureCallbacks[c](texName);
	}
    return pTex->GetID(); 
} // RenderSystemDX9::GetTextureID

bool RenderSystemDX9::DeleteTexture( int texID ) 
{ 
    if (texID < 0 || texID >= m_Textures.size()) return false;
    m_Textures[texID]->DeleteDeviceObjects();
    return true;
} // RenderSystemDX9::DeleteTexture

const char*	RenderSystemDX9::GetTextureName( int texID )
{ 
    if (texID < 0 || texID >= m_Textures.size()) return "";
    return m_Textures[texID]->GetName();
} // RenderSystemDX9::GetTextureName

const char* RenderSystemDX9::GetTexturePath( int texID ) const
{
    if (texID < 0 || texID >= m_Textures.size()) return "";
    return m_Textures[texID]->GetSearchPath();
} // RenderSystemDX9::GetTexturePath

BYTE* RenderSystemDX9::LockTexBits( int texID, int& pitch, int level ) 
{ 
    if (texID < 0 || texID >= m_Textures.size() || !m_Textures[texID]) return NULL;
    return m_Textures[texID]->LockBits( pitch, level );
} // RenderSystemDX9::LockTexBits

BYTE* RenderSystemDX9::LockTexBits( int texID, const Rct& rect, int& pitch, int level ) 
{ 
    if (texID < 0 || texID >= m_Textures.size()) return NULL;
    return m_Textures[texID]->LockBits( pitch, rect, level );
} // RenderSystemDX9::LockTexBits

void RenderSystemDX9::UnlockTexBits( int texID, int level ) 
{
    if (texID < 0 || texID >= m_Textures.size()) return;
    m_Textures[texID]->UnlockBits( level );
} // RenderSystemDX9::UnlockTexBits

int	RenderSystemDX9::GetShaderID( const char* shaderName ) 
{ 
    if (!m_bInited) return -1;
    int lqShaderID = -1;
	int shShaderID = -1;
	static int defShadowShader=-1;
	if(defShadowShader==-1){
		defShadowShader=-2;
		defShadowShader=GetShaderID("projected_shadow_caster");
	}
	shShaderID=defShadowShader>=0?defShadowShader:-1;

    _chdir(IRM->GetHomeDirectory());
    if(!strstr(shaderName,"low\\")){
        char sh[MAX_PATH];
        sprintf(sh,"Shaders\\low\\%s.fx",shaderName);        
        if(FileExists(sh)){
            sprintf(sh,"low\\%s",shaderName);
            lqShaderID = GetShaderID(sh);
        }
    }
	if(!strstr(shaderName,"shadows\\")){
        char sh[MAX_PATH];
        sprintf(sh,"Shaders\\shadows\\%s.fx",shaderName);        
        if(FileExists(sh)){
            sprintf(sh,"shadows\\%s",shaderName);
            shShaderID = GetShaderID(sh);
        }		
	}else shShaderID=-1;
    int nSha = m_Shaders.size();
    for (int i = 0; i < nSha; i++)
    {
        const IShader* pSha = m_Shaders[i];
        if (!stricmp( pSha->GetName(), shaderName )) return i;
    }
    char fname[MAX_PATH];
    sprintf(fname,"Shaders\\%s",shaderName);
    if(!strstr(fname,".fx"))strcat(fname,".fx");
    ShaderFX* pSha = NULL;        
    pSha = new ShaderFX( m_pDevice );    
    if (!pSha->Load( fname ))
    {
        if(strstr(shaderName,"low\\")){
            {
                Log.Error( "Could not load shader: %s", shaderName+4 );
            }            
        }
        delete(pSha);
        return lqShaderID;
    }
    pSha->SetName( shaderName );
    pSha->SetID( nSha );
    m_Shaders.push_back( pSha );
    m_LowShaderID.push_back( lqShaderID );
	m_ShadowShaders.push_back( shShaderID );
    return nSha; 
} // RenderSystemDX9::GetShaderID

int RenderSystemDX9::GetNShaderVars( int shID ) const
{
    if (shID < 0 || shID >= m_Shaders.size() || m_Shaders[shID] == NULL) return 0;
    return m_Shaders[shID]->GetNShaderVars();
} // RenderSystemDX9::GetNShaderVars

int RenderSystemDX9::GetNShaderPasses( int shID ) const
{
    if (shID < 0 || shID >= m_Shaders.size() || m_Shaders[shID] == NULL) return 0;
    return m_Shaders[shID]->GetNPasses();
} // RenderSystemDX9::GetNShaderPasses     

bool RenderSystemDX9::SetShaderTech( int shID, int techID )
{
    if (shID < 0 || shID >= m_Shaders.size() || m_Shaders[shID] == NULL) return 0;
    return m_Shaders[shID]->SetActiveTech( techID );
} // RenderSystemDX9::SetShaderTech

IShader* RenderSystemDX9::GetShader( int shID ) const 
{ 
    if (shID < 0 || shID >= m_Shaders.size()) return NULL;
    return m_Shaders[shID]; 
} // RenderSystemDX9::GetShaderName


const char*	RenderSystemDX9::GetShaderName( int shID ) const 
{ 
    if (shID < 0 || shID >= m_Shaders.size() || m_Shaders[shID] == NULL) return "";
    return m_Shaders[shID]->GetName(); 
} // RenderSystemDX9::GetShaderName

bool RenderSystemDX9::SetClipPlane( DWORD idx, const Plane& plane ) 
{ 
    HRESULT hres = m_pDevice->SetClipPlane( idx, (float*)&plane );
    if (hres != S_OK)
    {
        Log.Error( "Could not set user clipping plane %d", idx );
        return false;
    }
    return true; 
} // RenderSystemDX9::SetClipPlane
int SetRenderTargetTime=0;
bool RenderSystemDX9::PushRenderTarget( int texID, int dsID ) 
{ 
    if (texID >= m_Textures.size()) return false;

    IDirect3DSurface9* pPrevColor = NULL;
    IDirect3DSurface9* pNewColor  = NULL;
    IDirect3DSurface9* pPrevDS    = NULL;
    IDirect3DSurface9* pNewDS     = NULL;

    if (m_RTStack.size() > 0)
    {
        const RenderTargetPair& rt = m_RTStack.top();
        if (rt.m_Color >= 0) 
        {
            pPrevColor = m_Textures[rt.m_Color]->GetSurface(); 
        } else pPrevColor = NULL; 
        if (rt.m_DepthStencil >= 0)  
        {
            pPrevDS = m_Textures[rt.m_DepthStencil]->GetSurface(); 
        } else pPrevDS = NULL;  
    }
    else
    {
        pPrevColor = m_pBackBufferSurface;
        pPrevDS    = m_pDepthStencilSurface;
    }

    if (texID >= 0) pNewColor = m_Textures[texID]->GetSurface(); else pNewColor = NULL;
    if (dsID > 0) pNewDS = m_Textures[dsID]->GetSurface(); else pNewDS = NULL;

    if (dsID == 0) pNewDS = m_pDepthStencilSurface;

    m_RTStack.push( RenderTargetPair( texID, dsID ) );
    
    HRESULT hRes = S_OK;
    if (pPrevColor != pNewColor)
    {
        __beginT();
        hRes &= m_pDevice->SetRenderTarget( 0, pNewColor );
        __endT(SetRenderTargetTime);
    }
    
    if (pPrevColor != m_pBackBufferSurface) 
    {
        SAFE_DECREF( pPrevColor );
        SAFE_DECREF( pPrevColor );
    }
    
    if (pPrevDS != pNewDS)
    {
        hRes &= m_pDevice->SetDepthStencilSurface( pNewDS );
    }

    if (pPrevDS != m_pDepthStencilSurface) 
    {
        //SAFE_DECREF( pPrevDS );
        //SAFE_DECREF( pPrevDS );
    }

    return (hRes == S_OK);
} // RenderSystemDX9::PushRenderTarget

bool RenderSystemDX9::PopRenderTarget() 
{
    if (m_RTStack.size() == 0) return false;

    IDirect3DSurface9* pPrevColor   = NULL;
    IDirect3DSurface9* pNewColor    = NULL;
    IDirect3DSurface9* pPrevDS      = NULL;
    IDirect3DSurface9* pNewDS       = NULL;

    const RenderTargetPair& rt = m_RTStack.top();
    if (rt.m_Color >= 0) pPrevColor = m_Textures[rt.m_Color]->GetSurface();  
    if (rt.m_DepthStencil >= 0) pPrevDS = m_Textures[rt.m_DepthStencil]->GetSurface();  
    m_RTStack.pop();
    
    if (m_RTStack.size() > 0)
    {
        const RenderTargetPair& nrt = m_RTStack.top();
        if (nrt.m_Color >= 0) pNewColor = m_Textures[nrt.m_Color]->GetSurface(); 
        if (nrt.m_DepthStencil >= 0) pNewDS = m_Textures[nrt.m_DepthStencil]->GetSurface(); 
    }
    else
    {
        pNewColor = m_pBackBufferSurface;
        pNewDS    = m_pDepthStencilSurface;    
    }

    HRESULT hRes = S_OK;
    if (pPrevColor != pNewColor)
    {
        __beginT();
        hRes &= m_pDevice->SetRenderTarget( 0, pNewColor );
        __endT(SetRenderTargetTime);
    }

    if (pPrevColor != m_pBackBufferSurface) 
    {
        SAFE_DECREF( pPrevColor );
        SAFE_DECREF( pPrevColor );
    }

    if (pPrevDS != pNewDS)
    {
        hRes &= m_pDevice->SetDepthStencilSurface( pNewDS );
    }

    if (pPrevDS != m_pDepthStencilSurface) 
    {
        //SAFE_DECREF( pPrevDS );
        //SAFE_DECREF( pPrevDS );
    }

    return (hRes == S_OK);
} // RenderSystemDX9::PopRenderTarget
int TimeInSVP=0;
void RenderSystemDX9::SetViewPort( const Rct& vp, float zn, float zf, bool bClip ) 
{ 
    m_ViewPort      = vp; 
    m_ViewPortZNear = zn;
    m_ViewPortZFar  = zf;

    if (bClip)
    {
        Rct svp( 0, 0, GetBackBufferW(), GetBackBufferH() );
        svp.Clip( m_ViewPort );
    }

    D3DVIEWPORT9 dvp;
    dvp.X		= m_ViewPort.x;
    dvp.Y		= m_ViewPort.y;
    dvp.Width	= m_ViewPort.w;
    dvp.Height	= m_ViewPort.h;
    dvp.MinZ	= m_ViewPortZNear;
    dvp.MaxZ	= m_ViewPortZFar;

    __beginT();

    DX_CHK( m_pDevice->SetViewport( (D3DVIEWPORT9*)&dvp ) );	

    __endT(TimeInSVP);

} // RenderSystemDX9::SetViewPort

int PrevSetAV=-1;
void RenderSystemDX9::SetShaderAutoVars()
{
    if (m_CurShader == -1) return;	
	//if(PrevSetAV == m_CurShader)return;
	PrevSetAV = m_CurShader;
    m_Shaders[m_CurShader]->SetAutoVars();
} // RenderSystemDX9::SetShaderAutoVars
void RenderSystemDX9::SetShaderConst(int ConstIndex,float Value){
	if(ConstIndex>=0 && ConstIndex<4){
		m_UserShaderConsts[ConstIndex]=Value;
	}else Log.Warning("Shader const out of rang: %d",ConstIndex);
}
float RenderSystemDX9::GetShaderConst(int ConstIndex){
	if(ConstIndex>=0 && ConstIndex<4){
		return m_UserShaderConsts[ConstIndex];
	}else Log.Warning("Shader const out of rang: %d",ConstIndex);
	return 0;
}

int BEffTime=0;
void RenderSystemDX9::SetShader( int shaderID, int passID ) 
{
    if(m_ShadersQuality && shaderID>0)
    {
		if(m_ShadersQuality==255){
            int shL = m_ShadowShaders[shaderID];
			if(shL>=0)shaderID = shL;
		}else{
			int shL = m_LowShaderID[shaderID];
			if(shL>0)shaderID = shL;
		}
    }
    if (m_CurPass == passID && m_CurShader == shaderID) return;
	PrevSetAV=-1;
    if (m_CurShader >= 0) 
    {
        m_Shaders[m_CurShader]->EndPass();
        if (shaderID != m_CurShader)
        {
            m_Shaders[m_CurShader]->End();
        }
    }
    if (shaderID < 0 || shaderID >= m_Shaders.size())
    {
        m_CurShader = -1;
        return;
    }
    m_CurShader = shaderID;
    if (m_CurShader >= 0) 
    {
        __beginT();
        m_Shaders[m_CurShader]->Begin();
        m_Shaders[m_CurShader]->BeginPass( passID );
		ApplyFogStateBlock();
        __endT(BEffTime);
    }
    m_CurPass = passID;
} // RenderSystemDX9::SetShader
void RenderSystemDX9::SetShadersQuality( int Level ){
    m_ShadersQuality = Level;
}
int RenderSystemDX9::GetShadersQuality(){
    return m_ShadersQuality;
}
void RenderSystemDX9::SetBumpEnable	( bool State ){
	m_EnableBump = State;
}
bool RenderSystemDX9::GetBumpEnable	(){
    return m_EnableBump;
}
bool RenderSystemDX9::IsShaderValid( int shID, int techID )
{ 
    assert( false );
    return false; 
} // RenderSystemDX9::IsShaderValid

RenderTask& RenderSystemDX9::AddTask() 
{ 
    if (m_NTasks == c_MaxRenderTasks) Flush();
    RenderTask& task = m_Tasks[m_NTasks];
    m_NTasks++;
    task.m_TFactor = m_TFactor;
    return task; 
}  // RenderSystemDX9::AddTask
int SetTextureFactorTime=0;
void RenderSystemDX9::SetTextureFactor( DWORD tfactor )
{
    if (m_TFactor == tfactor) return;
    __beginT();
    DX_CHK( m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, tfactor ) );	
    __endT(SetTextureFactorTime);
    m_TFactor = tfactor;
} // RenderSystemDX9::SetTextureFactor

void RenderSystemDX9::SetZEnable( bool bEnable )
{
    __beginT();
    DX_CHK( m_pDevice->SetRenderState( D3DRS_ZENABLE, bEnable ? TRUE : FALSE ) );	
    __endT(OtherTime);
}

void RenderSystemDX9::SetZWriteEnable( bool bEnable )
{
    __beginT();
    DX_CHK( m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, bEnable ? TRUE : FALSE ) );	
    __endT(OtherTime);
}

void RenderSystemDX9::SetDitherEnable( bool bEnable )
{
    __beginT();
    DX_CHK( m_pDevice->SetRenderState( D3DRS_DITHERENABLE, bEnable ? TRUE : FALSE ) );
    __endT(OtherTime);
}

void RenderSystemDX9::SetTexFilterEnable( bool bEnable ) 
{
    __beginT();
    if (bEnable)
    {
        DX_CHK( m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR  ) );  
        DX_CHK( m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR  ) );  
    }
    else
    {
        DX_CHK( m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT  ) );  
        DX_CHK( m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT  ) );  
    }
    __endT(OtherTime);
} // RenderSystemDX9::SetTexFilterEnable     
void RenderSystemDX9::SetColorConst   ( DWORD Color )
{
    m_ColorConst=Color;
}
DWORD RenderSystemDX9::GetColorConst()
{
    return m_ColorConst;
}
void RenderSystemDX9::SetWireframe( bool bEnable )
{
    __beginT();
    DX_CHK( m_pDevice->SetRenderState( D3DRS_FILLMODE, bEnable ? D3DFILL_WIREFRAME : D3DFILL_SOLID ) );	
    __endT(OtherTime);
}

void  RenderSystemDX9::SetAlphaRef( BYTE alphaRef )
{
    __beginT();
    DX_CHK( m_pDevice->SetRenderState( D3DRS_ALPHAREF, alphaRef ) );	
    __endT(OtherTime);
}

void RenderSystemDX9::Dump( const char* fname )
{
}

bool RenderSystemDX9::ApplyStateBlock( DWORD id ) 
{ 
    return false; 
}

bool RenderSystemDX9::DeleteStateBlock( DWORD id ) 
{ 
    return false; 
}

int RenderSystemDX9::GetTexMemorySize() const 
{
    if (!m_pDevice) return 0;
	DWORD sz=m_pDevice->GetAvailableTextureMem();
	if(sz>512000000)sz=512000000;
    return sz;
} // RenderSystemDX9::GetTexMemorySize

int	RenderSystemDX9::GetNDisplayModes() 
{ 
    if (m_BestDisplayModes.size() == 0)
    {
        D3DAdapterInfo* pAd = m_d3dSettings.PAdapterInfo();
        if (!pAd) return 0;
        for (int i = 0; i < pAd->m_DisplayModes.size(); i++)
        {
            D3DDISPLAYMODE& dmode = pAd->m_DisplayModes[i];
            int nB = m_BestDisplayModes.size();
            bool bPresent = false;
            for (int j = 0; j < nB; j++)
            {
                D3DDISPLAYMODE& m = m_BestDisplayModes[j];
                if (m.Width == dmode.Width && 
                    m.Height == dmode.Height &&
                    m.RefreshRate < dmode.RefreshRate)
                {
                    m = dmode;
                    bPresent = true;
                }
            }
            if (!bPresent) m_BestDisplayModes.push_back( dmode );
        }
    }
    return m_BestDisplayModes.size(); 
} // RenderSystemDX9::GetNDisplayModes

void RenderSystemDX9::GetDisplayMode( int idx, int& width, int& height ) 
{
    if (idx < 0 || idx >= m_BestDisplayModes.size()) return;
    D3DDISPLAYMODE& mode = m_BestDisplayModes[idx];
    width = mode.Width;
    height = mode.Height;
} // RenderSystemDX9::GetDisplayMode

D3DCOLORVALUE DwordToD3DCOLORVALUE( DWORD col )
{
    D3DCOLORVALUE res;
    res.a = float( (col & 0xFF000000)>>24 ) / 255.0f;
    res.r = float( (col & 0x00FF0000)>>16 ) / 255.0f;
    res.g = float( (col & 0x0000FF00)>>8  ) / 255.0f;
    res.b = float( (col & 0x000000FF)     ) / 255.0f;
    return res;
}

void RenderSystemDX9::SetDirLight( DirectionalLight* pLight, int& index ) 
{
    if (!pLight) return;
    if (index == -1){
        index=0;
    }
    //if (index == -1)
    //{
    //    index = m_NumActiveLights;
    //    if (m_NumActiveLights == c_MaxLights) return;
    //}

    //m_NumActiveLights++;


    Vector3D dir = pLight->GetDir();

    D3DLIGHT9 light;
    light.Type		= D3DLIGHT_DIRECTIONAL ;            
    light.Diffuse	= DwordToD3DCOLORVALUE( pLight->GetDiffuse()  );
    light.Specular	= DwordToD3DCOLORVALUE( pLight->GetSpecular() );        
    light.Ambient	= DwordToD3DCOLORVALUE( pLight->GetAmbient()  );   

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

    __beginT()
    DX_CHK( m_pDevice->SetLight( index, &light ) );
    DX_CHK( m_pDevice->LightEnable( index, TRUE ) );
    __endT(OtherTime);
}
void RenderSystemDX9::SetPointLight( PointLight* pLight, int& index ) 
{
}

void RenderSystemDX9::SetSpotLight( SpotLight* pLight, int& index ) 
{
}	

void RenderSystemDX9::SetRSBlock( RenderStateBlock* pBlock ) 
{
}

void RenderSystemDX9::SetTSBlock( TextureStateBlock* pBlock, int stage ) 
{
}

DWORD RenderSystemDX9::CreateStateBlock( StateBlock* pBlock	) 
{
    return 0; 
}

void RenderSystemDX9::SetMaterial( DWORD ambient, DWORD diffuse, DWORD specular, DWORD emissive, float power )
{
    D3DMATERIAL9 mtl;

    mtl.Ambient		= ToD3DCOLORVALUE( ambient  );
    mtl.Diffuse		= ToD3DCOLORVALUE( diffuse  );
    mtl.Specular	= ToD3DCOLORVALUE( specular );
    mtl.Emissive	= ToD3DCOLORVALUE( emissive );
    mtl.Power		= power;

    __beginT();
    DX_CHK( m_pDevice->SetMaterial( &mtl ) );
    __endT(OtherTime);
} // D3DRenderSystem::SetMaterial

void RenderSystemDX9::SetFog( DWORD FogColor,float FogStart,float FogEnd,float FogDensity,int FogMode )			 
{
	m_FogColor=FogColor;
	m_FogStart=FogStart; 
	m_FogEnd=FogEnd; 
	m_FogDensity=FogDensity; 
	m_FogMode=FogMode;
	if(FogColor){
		DX_CHK( m_pDevice->SetRenderState(D3DRS_FOGCOLOR,m_FogColor)); 
		DX_CHK( m_pDevice->SetRenderState(D3DRS_FOGSTART,*(DWORD *)(&m_FogStart))); 
		DX_CHK( m_pDevice->SetRenderState(D3DRS_FOGEND,*(DWORD *)(&m_FogEnd))); 
		DX_CHK( m_pDevice->SetRenderState(D3DRS_FOGDENSITY,*(DWORD *)(&m_FogDensity))); 
		DX_CHK( m_pDevice->SetRenderState(D3DRS_FOGTABLEMODE,m_FogMode)); 	
		DX_CHK( m_pDevice->SetRenderState(D3DRS_FOGVERTEXMODE,m_FogMode));
		DX_CHK( m_pDevice->SetRenderState(D3DRS_FOGENABLE,0)); 
	}

} // RenderSystemDX9::SetFog

void RenderSystemDX9::ApplyFogStateBlock(){
	if(!m_FogColor)DX_CHK( m_pDevice->SetRenderState(D3DRS_FOGENABLE,0)); 
	/*
    __beginT();	
	if(m_FogColor){        
		DX_CHK( m_pDevice->SetRenderState(D3DRS_FOGCOLOR,m_FogColor)); 
		DX_CHK( m_pDevice->SetRenderState(D3DRS_FOGSTART,*(DWORD *)(&m_FogStart))); 
		DX_CHK( m_pDevice->SetRenderState(D3DRS_FOGEND,*(DWORD *)(&m_FogEnd))); 
		DX_CHK( m_pDevice->SetRenderState(D3DRS_FOGDENSITY,*(DWORD *)(&m_FogDensity))); 
		DX_CHK( m_pDevice->SetRenderState(D3DRS_FOGTABLEMODE,m_FogMode)); 	
		DX_CHK( m_pDevice->SetRenderState(D3DRS_FOGVERTEXMODE,m_FogMode));
	}else{
		DX_CHK( m_pDevice->SetRenderState(D3DRS_FOGENABLE,0)); 
	}
    __endT(OtherTime);
	*/	
}

void RenderSystemDX9::DisableLights() 
{
} // RenderSystemDX9::DisableLights

BYTE* RenderSystemDX9::LockAppendVB( int vbID, int size, int& offset, DWORD& stamp )
{
    if (vbID < 0 || vbID >= m_VBuffers.size()) return NULL;
    if (!m_VBuffers[vbID]->HasAppendSpace( size )) 
    {
        Flush();
    }
    return m_VBuffers[vbID]->LockAppend( size, offset, stamp ); 
} // RenderSystemDX9::LockAppendVB

BYTE* RenderSystemDX9::LockAppendIB( int ibID, int size, int& offset, DWORD& stamp )
{
    if (ibID < 0 || ibID >= m_IBuffers.size()) return NULL;
    if (!m_IBuffers[ibID]->HasAppendSpace( size )) 
    {
        Flush();
    }
    return m_IBuffers[ibID]->LockAppend( size, offset, stamp );
} // RenderSystemDX9::LockAppendIB

BYTE* RenderSystemDX9::LockVB( int vbID, int firstV, int numV, DWORD& stamp ) 
{ 
    if (vbID < 0 || vbID >= m_VBuffers.size()) return NULL;
    return m_VBuffers[vbID]->Lock( firstV, numV, stamp ); 
} // RenderSystemDX9::LockVB

bool RenderSystemDX9::IsVBStampValid( int vbID, DWORD stamp )
{
    if (vbID < 0 || vbID >= m_VBuffers.size()) return true;
    return m_VBuffers[vbID]->IsStampValid( stamp );
} // RenderSystemDX9::IsVBStampValid

int RenderSystemDX9::GetVBufferID( const char* vbName )
{
    for (int i = 0; i < m_VBuffers.size(); i++)
    {
        if (!stricmp( vbName, m_VBuffers[i]->GetName() )) return i;
    }
    return -1;
} // RenderSystemDX9::GetVBufferID

bool RenderSystemDX9::SetVB( int vbID, int vType, int stream, int frequency )
{
    if (vbID < 0 || vbID >= m_VBuffers.size()) return false;
    IVertexBuffer* ivb = m_VBuffers[vbID];
    if (vType < 0) vType = ivb->GetVType();
    const VertexTypeEntry& vt = m_VertexTypes[vType];
    int vSize = vt.m_VDecl.m_VertexSize;
    if (vSize == 0) return false;
    ivb->SetVType( vType );  
    ivb->SetVertexSize( vSize );
    __beginT();
    if(vt.m_FVF){
        DX_CHK( m_pDevice->SetFVF( vt.m_FVF ) );
    }else{
        DX_CHK( m_pDevice->SetVertexDeclaration( vt.m_pVDeclD3D ) );
    }    
    __endT(OtherTime);
    ivb->Bind( stream, frequency );
    return true;
} // RenderSystemDX9::SetVB

bool RenderSystemDX9::DiscardVB( int vbID ) 
{ 
    return false; 
} // RenderSystemDX9::DiscardVB

void RenderSystemDX9::UnlockVB( int vbID ) 
{ 
    if (vbID < 0 || vbID >= m_VBuffers.size()) return;
    m_VBuffers[vbID]->Unlock();
} // RenderSystemDX9::UnlockVB

int RenderSystemDX9::CreateVB( const char* name, int size, int vType, bool bDynamic ) 
{ 
    int vbID = GetVBufferID( name );
    if (vbID != -1) return vbID;

	int n=m_VBuffers.size();
	VertexBufferDX9* bestVB=NULL;
	int bestID=-1;
	int maxdif=size/4;
	for(int i=0;i<n;i++){
		VertexBufferDX9* VB=m_VBuffers[i];
		if(VB->IsFree() && VB->GetVType()==vType && VB->IsDynamic()==bDynamic){
            int dsize=VB->GetSizeBytes()-size;
			if(dsize>=0 && dsize<=maxdif){
                maxdif=dsize;
				bestVB=VB;
				bestID=i;
			}
		}
	}
	if(bestVB){
		bestVB->SetFree(false);
		return bestID;
	}
    VertexBufferDX9* vbuf = new VertexBufferDX9( name, m_pDevice );
    const VertexDeclaration* pVDecl = NULL;
    if (vType >= 0)
    {
        const VertexTypeEntry& vt = m_VertexTypes[vType];
        pVDecl = &vt.m_VDecl;
    }
    bool res = vbuf->Create( size, bDynamic, pVDecl );
    if (!res)
    {
        if (bDynamic) Log.Error( "Could not create dynamic vertex buffer. Name: %s, Size: %d", name, size );
        else Log.Error( "Could not create static vertex buffer. Name: %s, Size: %d", name, size );
        delete vbuf;
        return -1;
    }
    vbID = m_VBuffers.size();
    m_VBuffers.push_back( vbuf );
    return vbID; 
} // RenderSystemDX9::CreateVB

bool RenderSystemDX9::DeleteVB( int vbID ) 
{ 
	VertexBufferDX9* VB=m_VBuffers[vbID];
	if(VB){
		VB->SetFree(true);
	}
    return false; 
} // RenderSystemDX9::DeleteVB


int RenderSystemDX9::GetShaderVarID( int shID, const char* constantName ) 
{ 
    if (shID < 0 || shID >= m_Shaders.size()) return -1;
    return m_Shaders[shID]->GetShaderVarID( constantName );
} // RenderSystemDX9::GetShaderVarID

bool RenderSystemDX9::SetShaderVar( int shID, int cID, bool val ) 
{
    if (shID < 0 || shID >= m_Shaders.size()) return -1;
    return m_Shaders[shID]->SetShaderVar( cID, val );
} // RenderSystemDX9::SetShaderVar

bool RenderSystemDX9::SetShaderVar( int shID, int cID, float val ) 
{ 
    if (shID < 0 || shID >= m_Shaders.size()) return -1;
    return m_Shaders[shID]->SetShaderVar( cID, val );
} // RenderSystemDX9::SetShaderVar

bool RenderSystemDX9::SetShaderVar( int shID, int cID, int val ) 
{ 
    if (shID < 0 || shID >= m_Shaders.size()) return -1;
    return m_Shaders[shID]->SetShaderVar( cID, val ); 
} // RenderSystemDX9::SetShaderVar

bool RenderSystemDX9::SetShaderVar( int shID, int cID, const Matrix4D& val ) 
{ 
    if (shID < 0 || shID >= m_Shaders.size()) return -1;
    return m_Shaders[shID]->SetShaderVar( cID, val ); 
} // RenderSystemDX9::SetShaderVar

bool RenderSystemDX9::SetShaderVar( int shID, int cID, const Vector4D& val ) 
{ 
	if(m_ShadersQuality && shID>0)
	{
		if(m_ShadersQuality==255){
			int shL = m_ShadowShaders[shID];
			if(shL>=0)shID = shL;
		}else{
			int shL = m_LowShaderID[shID];
			if(shL>0)shID = shL;
		}
	}
    if (shID < 0 || shID >= m_Shaders.size()) return -1;
    return m_Shaders[shID]->SetShaderVar( cID, val ); 
} // RenderSystemDX9::SetShaderVar

bool RenderSystemDX9::SetShaderVar( int shID, int cID, const Vector4D* val, int count ) 
{ 
	if (shID < 0 || shID >= m_Shaders.size()) return -1;
	return m_Shaders[shID]->SetShaderVar( cID, val, count ); 
} // RenderSystemDX9::SetShaderVar

bool RenderSystemDX9::SetShaderVar( int shID, int cID, const Vector3D& val ) 
{ 
    if (shID < 0 || shID >= m_Shaders.size()) return -1;
    return m_Shaders[shID]->SetShaderVar( cID, val ); 
} // RenderSystemDX9::SetShaderVar

int	RenderSystemDX9::GetTextureWidth( int texID ) const
{
    if (texID < 0 || texID >= m_Textures.size()) return 0;
    return m_Textures[texID]->GetWidth();
} // RenderSystemDX9::GetTextureWidth

int RenderSystemDX9::GetNTextures() const
{
    return m_Textures.size();
} // RenderSystemDX9::GetNTextures

int RenderSystemDX9::GetTextureSize( int texID ) const
{
    if (texID < 0 || texID >= m_Textures.size()) return 0;
    return m_Textures[texID]->GetSize();
} // RenderSystemDX9::GetTextureSize

TextureMemoryPool RenderSystemDX9::GetTexturePool( int texID ) const
{
    if (texID < 0 || texID >= m_Textures.size()) return tmpUnknown;
    return m_Textures[texID]->GetPool();
} // RenderSystemDX9::GetTexturePool

ColorFormat RenderSystemDX9::GetTextureFormat( int texID )
{
    if (texID < 0 || texID >= m_Textures.size()) return cfUnknown;
    return m_Textures[texID]->GetColorFormat();
} // RenderSystemDX9::GetTextureFormat

int	RenderSystemDX9::GetTextureHeight( int texID ) const
{
    if (texID < 0 || texID >= m_Textures.size()) return 0;
    return m_Textures[texID]->GetHeight();
} // RenderSystemDX9::GetTextureHeight

int	RenderSystemDX9::GetTextureNMips( int texID ) const
{
    if (texID < 0 || texID >= m_Textures.size()) return 0;
    return m_Textures[texID]->GetNMipMaps();
} // RenderSystemDX9::GetTextureNMips


BYTE* RenderSystemDX9::LockIB( int ibID, int firstIdx, int numIdx, DWORD& stamp )
{
    if (ibID < 0 || ibID >= m_IBuffers.size()) return NULL;
    return m_IBuffers[ibID]->Lock( firstIdx, numIdx, stamp );
} // RenderSystemDX9::LockIB


bool RenderSystemDX9::DiscardIB( int ibID )
{
    if (ibID < 0 || ibID >= m_IBuffers.size()) return false;
    return false;//m_IBuffers[ibID]->Discard();
} // RenderSystemDX9::LockIB

void RenderSystemDX9::UnlockIB( int ibID )
{
    if (ibID < 0 || ibID >= m_IBuffers.size()) return;
    return m_IBuffers[ibID]->Unlock();
} // RenderSystemDX9::UnlockIB

int RenderSystemDX9::CreateIB( const char* name, int size, IndexSize idxSize, bool bDynamic )
{
    int ibID = GetIBufferID( name );
    if (ibID != -1) return ibID;

	int n=m_IBuffers.size();
	IndexBufferDX9* bestIB=NULL;
	int bestID=-1;
	int maxdif=size/4;
	for(int i=0;i<n;i++){
		IndexBufferDX9* IB=m_IBuffers[i];
		if(IB->IsFree() && IB->IsDynamic()==bDynamic){
			int ds=IB->GetSizeBytes()-size;
			if(ds>=0 && ds<=maxdif){
				maxdif=ds;
				bestIB=IB;
				bestID=i;
			}
		}
	}
	if(bestIB){
		bestIB->SetFree(false);
		return bestID;
	}

    IndexBufferDX9* ibuf = new IndexBufferDX9( name );
    const VertexDeclaration* pVDecl = NULL;
    bool res = ibuf->Create( size, bDynamic, idxSize );
    if (!res)
    {
        if (bDynamic) Log.Error( "Could not create dynamic index buffer. Name: %s, Size: %d", name, size );
        else Log.Error( "Could not create static index buffer. Name: %s, Size: %d", name, size );
        delete ibuf;
        return -1;
    }
    ibID = m_IBuffers.size();
    m_IBuffers.push_back( ibuf );
    return ibID; 
} // RenderSystemDX9::CreateIB

bool RenderSystemDX9::IsIBStampValid( int ibID, DWORD stamp )
{
    if (ibID < 0 || ibID >= m_IBuffers.size()) return true;
    return m_IBuffers[ibID]->IsStampValid( stamp );
} // RenderSystemDX9::IsIBStampValid

int RenderSystemDX9::GetIBufferID( const char* ibName )
{
    for (int i = 0; i < m_IBuffers.size(); i++)
    {
        if (!stricmp( ibName, m_IBuffers[i]->GetName() )) return i;
    }
    return -1;
} // RenderSystemDX9::GetIBufferID

bool RenderSystemDX9::SetIB( int ibID )
{
    if (ibID < 0 || ibID >= m_IBuffers.size()) return false;
    return m_IBuffers[ibID]->Bind();
} // RenderSystemDX9::SetIB

bool RenderSystemDX9::DeleteIB( int ibID )
{
	IndexBufferDX9* IB=m_IBuffers[ibID];
	if(IB){
		IB->SetFree(true);
	}
    return false;
} // RenderSystemDX9::DeleteIB

int RenderSystemDX9::RegisterVType( const VertexDeclaration& vdecl )
{
    int nVT = m_VertexTypes.size();
    int i;
    for (i = 0; i < nVT; i++)
    {
        if (m_VertexTypes[i].m_VDecl == vdecl) return i;
    }
    // search whether already registered 
    VertexTypeEntry ve;
    VertexFormat vf = (VertexFormat)i;
    ve.m_VFormat    = vf;
    ve.m_VDecl      = vdecl;
    ve.m_pVDeclD3D  = CreateVDecl( m_pDevice, vdecl );
    ve.m_FVF        = CreateFVF( vdecl );
    m_VertexTypes.push_back( ve );
    return m_VertexTypes.size() - 1;
} // RenderSystemDX9::RegisterVType

int TimeInDrawPrim=0;
int Diplimit=1000000;
int NumDips=0;
void RenderSystemDX9::Draw( int firstVert, int nVert, int firstIdx, int nIdx, PrimitiveType priType )
{
    if (nVert == 0 || Diplimit == 0) return;
    Diplimit--;
    NumDips++;
    //  draw primitives
    bool bIndexed   = true; 
    int numPri      = 0;
    if (nIdx == 0)
    {
        numPri = GetNumPrimitives( priType, nVert );
        bIndexed = false;
        if (priType == ptQuadList)
        {
            numPri      = nVert/2;
            bIndexed    = true;
            firstIdx    = 0;
            static int s_QuadIB = GetIBufferID( "Quads" );
            SetIB( s_QuadIB );
        }
    }
    else
    {
        numPri = GetNumPrimitives( priType, nIdx );
    }

    //SetFogOverride();
    for(int i=0;i<8;i++){
        if(m_TextureOverride[i]>0){
            IRS->SetTexture( m_TextureOverride[i],i );
        }
    }

    __beginT();

    DX_CHK( m_pDevice->BeginScene() );
    if (bIndexed)
    {
        DX_CHK( m_pDevice->DrawIndexedPrimitive( ConvertPrimitiveType( priType ), 
                                                    firstVert, 
                                                    0, 
                                                    nVert, 
                                                    firstIdx, 
                                                    numPri ) );
    }
    else
    {
        DX_CHK( m_pDevice->DrawPrimitive( ConvertPrimitiveType( priType ), firstVert, numPri ) );
    }
    DX_CHK( m_pDevice->EndScene() );

    __endT(TimeInDrawPrim);


} // RenderSystemDX9::Draw


int	RenderSystemDX9::GetFontID( const char* name ) 
{
    int nF = m_Fonts.size();
    for (int i = 0; i < nF; i++)
    {
        const IFont* pTex = m_Fonts[i];
        if (!strcmp( pTex->GetName(), name )) return i;
    }
    //  load texture 
    IFont* pFont = new FontDX9( m_pDevice );
    
    //  create font here...

    m_Fonts.push_back( pFont );
    pFont->SetID( nF );
    return nF; 
} // RenderSystemDX9::GetFontID 

void RenderSystemDX9::DestroyFont( int fontID ) 
{

} // RenderSystemDX9::DestroyFont

int	RenderSystemDX9::CreateFont( const char* name, int height, DWORD charset, bool bBold, bool bItalic )							
{
    return -1;
}

int	RenderSystemDX9::CreateFont( const char* texName, int charW, int charH ) 
{
    return -1;
}

int	RenderSystemDX9::GetStringWidth( int fontID, const char* str, int spacing ) 
{
    return 0;
}

int RenderSystemDX9::GetCharWidth( int fontID, BYTE ch ) 
{
    return 0;
}

int RenderSystemDX9::GetCharHeight( int fontID, BYTE ch ) 
{
    return 0; 
}

void RenderSystemDX9::SetCurrentFont( int fontID ) 
{

}

bool RenderSystemDX9::DrawString( const char* str, const Vector3D& pos, DWORD color, int spacing ) 
{
    return false;
}

bool RenderSystemDX9::DrawString3D( const char* str, const Vector3D& pos, DWORD color, int spacing ) 
{
    return false;
}

bool RenderSystemDX9::DrawChar( const Vector3D& pos, BYTE ch, DWORD color ) 
{
    return false;
}

bool RenderSystemDX9::DrawChar( const Vector3D& pos, const Rct& uv, DWORD color )
{
    return false;
}

bool RenderSystemDX9::DrawChar( const Vector3D& pos, const Rct& uv, float w, float h, DWORD color )
{
    return false;
}

void RenderSystemDX9::FlushText() 
{

}

void RenderSystemDX9::DrawLine( float x1, float y1, float x2, float y2, float z, DWORD color1, DWORD color2 )
{

} // RenderSystemDX9::DrawLine

void RenderSystemDX9::DrawLine( const Vector3D& a, const Vector3D& b, DWORD color1, DWORD color2 )
{

} // RenderSystemDX9::DrawLine

void RenderSystemDX9::DrawRect( const Rct& rct, const Rct& uv, float z, DWORD ca, DWORD cb, DWORD cc, DWORD cd )
{

} // RenderSystemDX9::DrawRect

void RenderSystemDX9::DrawPoly( float ax, float ay, float bx, float by, float cx, float cy,
                                 float au, float av, float bu, float bv, float cu, float cv )
{

} // RenderSystemDX9::DrawPoly

void RenderSystemDX9::DrawPoly( const Vector3D& a, const Vector3D& b, const Vector3D& c, DWORD acol, DWORD bcol, DWORD ccol,
                                 float au, float av, float bu, float bv, float cu, float cv )
{

} // RenderSystemDX9::DrawPoly

void RenderSystemDX9::FlushPrim( bool bShaded )
{

} // RenderSystemDX9::FlushPrim

LRESULT RenderSystemDX9::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    if (!IInput) return D3DApplication::MsgProc( hWnd, uMsg, wParam, lParam );
    
    switch (uMsg) 
    {
    case WM_LBUTTONDBLCLK:
        IInput->MouseLBDbl( LOWORD( lParam ), HIWORD( lParam ) );
        break;

    case WM_MBUTTONDBLCLK:
        IInput->MouseMBDbl( LOWORD( lParam ), HIWORD( lParam ) );
        break;

    case WM_RBUTTONDBLCLK:
        IInput->MouseRBDbl( LOWORD( lParam ), HIWORD( lParam ) );
        break;

    case WM_RBUTTONDOWN:
        IInput->MouseRBDown( LOWORD( lParam ), HIWORD( lParam ) );
        break;

    case WM_RBUTTONUP:
        IInput->MouseRBUp( LOWORD( lParam ), HIWORD( lParam ) );
        break;

    case WM_LBUTTONDOWN:
        IInput->MouseLBDown( LOWORD( lParam ), HIWORD( lParam ) );
        break;

    case WM_LBUTTONUP:
        IInput->MouseLBUp( LOWORD( lParam ), HIWORD( lParam ) );
        break;

    case WM_MBUTTONDOWN:
        IInput->MouseMBDown( LOWORD( lParam ), HIWORD( lParam ) );
        break;

    case WM_MBUTTONUP:
        IInput->MouseMBUp( LOWORD( lParam ), HIWORD( lParam ) );
        break;

    case WM_MOUSEMOVE:
        IInput->MouseMove( LOWORD( lParam ), HIWORD( lParam ), wParam );
        break;

    case WM_MOUSEWHEEL:
        {
            short WheelDelta = (short) HIWORD( wParam );
            IInput->MouseWheel( WheelDelta );
        }break;

    case WM_KEYDOWN:
            IInput->KeyDown( wParam, lParam );
        break;

    case WM_CHAR:
            IInput->Char( wParam, lParam );
        break;

    case WM_KEYUP:
            IInput->KeyUp( wParam, lParam );
        break;

    case WM_ERASEBKGND: return TRUE;
    }

    return D3DApplication::MsgProc( hWnd, uMsg, wParam, lParam );
} // RenderSystemDX9::MsgProc

void RenderSystemDX9::CreateVTypeTable()
{
    int nVTypes = (int)vfLAST;
    for (int i = 0; i < nVTypes; i++)
    {
        VertexFormat vf = (VertexFormat)i;
        VertexDeclaration vdecl = CreateVertexDeclaration( vf );
        int vtype = RegisterVType( vdecl );
        assert( i == vtype );
    }
} // RenderSystemDX9::CreateVTypeTable

void RenderSystemDX9::ClearVTypeTable()
{
    for (int i = 0; i < m_VertexTypes.size(); i++)
    {
        if (m_VertexTypes[i].m_pVDeclD3D) 
        {
            SAFE_RELEASE( m_VertexTypes[i].m_pVDeclD3D );
        }
    }
    m_VertexTypes.clear();
} // RenderSystemDX9::ClearVTypeTable

void RenderSystemDX9::FillQuadIndexBuffer()
{
    int ibID = GetIBufferID( "Quads" );
    if (ibID == -1) return;
    IIndexBuffer* ibuf = m_IBuffers[ibID];
    int nIdx = c_QuadIBufferBytes/2;
    int nQuads = nIdx/6;
    DWORD stamp;
    WORD* pIdx = (WORD*)ibuf->Lock( 0, nQuads*6, stamp, true );
    if (!pIdx) 
    {
        Log.Error( "Could not lock quad index buffer." );
        return;
    }
    for (int i = 0; i < nQuads; i++)
    {
        int cV = i*4;
        pIdx[i*6 + 0] = cV + 0;
        pIdx[i*6 + 1] = cV + 1;
        pIdx[i*6 + 2] = cV + 2;
        pIdx[i*6 + 3] = cV + 2;
        pIdx[i*6 + 4] = cV + 1;
        pIdx[i*6 + 5] = cV + 3;
    }
    ibuf->Unlock();
} // RenderSystemDX9::FillQuadIndexBuffer

void RenderSystemDX9::PurgeStaticBuffers()
{
    for (int i = 0; i < m_VBuffers.size(); i++)
    {
        if (!m_VBuffers[i]->IsDynamic()) m_VBuffers[i]->Purge();
    }

    for (int i = 0; i < m_IBuffers.size(); i++)
    {
        if (!m_IBuffers[i]->IsDynamic() && 
            stricmp( m_IBuffers[i]->GetName(), "Quads" )) m_IBuffers[i]->Purge();
    }
} // RenderSystemDX9::PurgeStaticBuffers


struct TaskCompare
{
    bool operator ()( const RenderTask* l, const RenderTask* r ) { return (*l < *r); }
}; // struct TaskCompare

int rt_compare( const void * elem1, const void * elem2 ){
	RenderTask* rt1=*((RenderTask**)elem1);
	RenderTask* rt2=*((RenderTask**)elem2);
	if(rt1->m_ShaderID==rt2->m_ShaderID){	
		if(rt1->m_TexID[0]==rt2->m_TexID[0]){			
			return 0;
		}else return rt1->m_TexID[0]<rt2->m_TexID[0] ? -1 : 1;
	}else return rt1->m_ShaderID<rt2->m_ShaderID ? -1 : 1;
}

void RenderSystemDX9::SortTasks()
{
    if (m_bSorted) return;
    m_SortedTasks.resize( m_NTasks );
    int nT = m_NTasks;
    for (int i = 0; i < nT; i++) { m_SortedTasks[i] = &m_Tasks[i]; }
	if(nT>1){
		qsort(&m_SortedTasks[0],nT,4,rt_compare);
	}
    //std::sort( m_SortedTasks.begin(), m_SortedTasks.end(), TaskCompare() );
    m_bSorted = true;
} // RenderSystemDX9::SortTasks

void RenderSystemDX9::Flush()
{
    SortTasks();
    int nT = m_SortedTasks.size();
    for (int i = 0; i < nT; i++)
    {
        const RenderTask& rt = *m_SortedTasks[i];
        const RenderTask& pt = (i == 0) ? RenderTask::c_Invalid : *m_SortedTasks[i - 1];

        if (rt.m_ShaderID != pt.m_ShaderID) 
        {
            SetShader( rt.m_ShaderID, rt.m_Pass );
        }
        
        if (rt.m_bHasTM) SetWorldTM( rt.m_TM );// else ResetWorldTM();
        if (rt.m_ShaderID != pt.m_ShaderID) 
        {
            SetShaderAutoVars();
        }

        if (rt.m_VBufID != pt.m_VBufID || rt.m_VType != pt.m_VType) SetVB( rt.m_VBufID, rt.m_VType );
        if (rt.m_IBufID != pt.m_IBufID) 
        {
            SetIB( rt.m_IBufID );
        }

        for (int j = 0; j < c_MaxTextureStages; j++) 
        {
            const int texID = rt.m_TexID[j];
            if ( ( texID != pt.m_TexID[j] || rt.m_ShaderID != pt.m_ShaderID) && texID >= 0) 
            {
                m_Textures[texID]->Bind( j );
            }
        }

        SetTextureFactor( rt.m_TFactor );
		if(rt.m_UseMeshConst){
			IRS->SetShaderVar(rt.m_ShaderID,rt.m_ConstIdx,rt.m_MeshConst);
		}
        int nP = IRS->GetNShaderPasses( rt.m_ShaderID );
        if (nP == 1)
        //  single-pass shader
        {
            Draw( rt.m_FirstVert, rt.m_NVert, rt.m_FirstIdx, rt.m_NIdx, rt.m_PriType );
        }
        else
        //  multipass shader
        {
            for (int i = 0; i < nP; i++)
            {
                IRS->SetShader( rt.m_ShaderID, i );
                SetShaderAutoVars();
                Draw( rt.m_FirstVert, rt.m_NVert, rt.m_FirstIdx, rt.m_NIdx, rt.m_PriType );
            }
        }
    }

    m_NTasks  = 0; 
    m_bSorted = false; 
} // RenderSystemDX9::RenderScene

//-------------------------------------------------------------------------------------
// RenderSystemDX9::ScreenShot : void(const char *, _D3DXIMAGE_FILEFORMAT)
//-------------------------------------------------------------------------------------
void RenderSystemDX9::ScreenShot(const char *pFileName, _D3DXIMAGE_FILEFORMAT Format) {
	LPDIRECT3DSURFACE9 Buffer;
	int W = GetSystemMetrics(SM_CXSCREEN);
	int H = GetSystemMetrics(SM_CYSCREEN);
	
	HRESULT hr = m_pDevice->CreateOffscreenPlainSurface(W, H, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &Buffer, NULL);
	if(FAILED(hr)) return;
	hr = m_pDevice->GetFrontBufferData(NULL, Buffer);
	if(SUCCEEDED(hr)) {
		D3DXSaveSurfaceToFile(pFileName, Format, Buffer, NULL, NULL);
	}
	Buffer->Release();
} // RenderSystemDX9::ScreenShot

//-----------------------------------------------------------------------------
// RenderSystemDX9::ScreenShotBMP : void(const char *)
//-----------------------------------------------------------------------------
void RenderSystemDX9::ScreenShotBMP(const char *pBMPFileName) {
	ScreenShot(pBMPFileName, D3DXIFF_BMP);
} // RenderSystemDX9::ScreenShotBMP

//-----------------------------------------------------------------------------
// RenderSystemDX9::ScreenShotJPG : void(const char *)
//-----------------------------------------------------------------------------
void RenderSystemDX9::ScreenShotJPG(const char *pJPGFileName) {
	ScreenShot(pJPGFileName, D3DXIFF_JPG);
} // RenderSystemDX9::ScreenShotJPG

//-----------------------------------------------------------------------------
// RenderSystemDX9::TimeOverrideEnable
//-----------------------------------------------------------------------------
void RenderSystemDX9::TimeOverrideEnable(const float secNewTime) {
	m_TimeIsOverridden = true;
	m_secOverriddenTime = secNewTime;
} // RenderSystemDX9::TimeOverrideEnable

//-----------------------------------------------------------------------------
// RenderSystemDX9::TimeOverrideDisable
//-----------------------------------------------------------------------------
void RenderSystemDX9::TimeOverrideDisable() {
	m_TimeIsOverridden = false;
} // RenderSystemDX9::TimeOverrideDisable

//-----------------------------------------------------------------------------
// RenderSystemDX9::TimeOverrideIsEnabled
//-----------------------------------------------------------------------------
bool RenderSystemDX9::TimeOverrideIsEnabled(float *pSecTime) {
	if(!m_TimeIsOverridden) {
		return false;
	}
	if(pSecTime) {
		*pSecTime = m_secOverriddenTime;
	}
	return true;
} // RenderSystemDX9::TimeOverrideIsEnabled

//-----------------------------------------------------------------------------
// RenderSystemDX9::AntialiasingIsEnabled
//-----------------------------------------------------------------------------
bool RenderSystemDX9::AntialiasingIsEnabled() {
	return m_d3dpp.MultiSampleType == m_d3dSettings.MultisampleTypeBest();
} // RenderSystemDX9::AntialiasingIsEnabled

//-----------------------------------------------------------------------------
// RenderSystemDX9::AntialiasingEnable
//-----------------------------------------------------------------------------
void RenderSystemDX9::AntialiasingEnable() {
	m_d3dSettings.SetMultisampleTypeToBest();
	m_d3dpp.MultiSampleType = m_d3dSettings.MultisampleType();
	Reset3DEnvironment();
}// RenderSystemDX9::AntialiasingEnable

//-----------------------------------------------------------------------------
// RenderSystemDX9::AntialiasingDisable
//-----------------------------------------------------------------------------
void RenderSystemDX9::AntialiasingDisable() {
	m_d3dSettings.SetMultisampleTypeToWorst();
	m_d3dpp.MultiSampleType = m_d3dSettings.MultisampleType();
	Reset3DEnvironment();
} // RenderSystemDX9::AntialiasingDisable

//-----------------------------------------------------------------------------
// RenderSystemDX9::TrueColorEnable
//-----------------------------------------------------------------------------
void RenderSystemDX9::TrueColorEnable() {
	m_d3dSettings.BackBufferForceTrueColorEnable();
	m_d3dpp.BackBufferFormat = m_d3dSettings.BackBufferFormat();
	Reset3DEnvironment();
} // RenderSystemDX9::TrueColorEnable

//-----------------------------------------------------------------------------
// RenderSystemDX9::TrueColorDisable
//-----------------------------------------------------------------------------
void RenderSystemDX9::TrueColorDisable() {
	m_d3dSettings.BackBufferForceTrueColorDisable();
	m_d3dpp.BackBufferFormat = m_d3dSettings.BackBufferFormat();
	Reset3DEnvironment();
} // RenderSystemDX9::TrueColorDisable

//-----------------------------------------------------------------------------
// RenderSystemDX9::TrueColorIsEnabled
//-----------------------------------------------------------------------------
bool RenderSystemDX9::TrueColorIsEnabled() {
	return m_d3dpp.BackBufferFormat == D3DFMT_X8R8G8B8;
	return false;
} // RenderSystemDX9::TrueColorIsEnabled

//-----------------------------------------------------------------------------------------------
// RenderSystemDX9::GetDeviceDisplayMode
//-----------------------------------------------------------------------------------------------
bool RenderSystemDX9::GetDeviceDisplayMode(int *pWidth, int *pHeight, int *pBpp, int *pRefreshRate) {
	if(!m_pDevice) return false;
	D3DDISPLAYMODE dm;
	m_pDevice->GetDisplayMode(0, &dm);
	if(pWidth) {
		*pWidth = dm.Width;
	}
	if(pHeight) {
		*pHeight = dm.Height;
	}
	if(pBpp) {
		switch(dm.Format) {
			case D3DFMT_A2R10G10B10:
			case D3DFMT_X8R8G8B8:
				*pBpp = 32;
				break;
			case D3DFMT_X1R5G5B5:
			case D3DFMT_R5G6B5:
				*pBpp = 16;
				break;
			default:
				assert(0);
		}
	}
	if(pRefreshRate) {
		*pRefreshRate = dm.RefreshRate;
	}
	return false;
} // RenderSystemDX9::GetDeviceDisplayMode
void RenderSystemDX9::SetTextureOverride( int TextureID,int Stage ){
    assert(Stage>=0 && Stage<8);
    m_TextureOverride[Stage] = TextureID;
	IRS->SetTexture( TextureID,Stage );
}
int RenderSystemDX9::GetTextureOverride( int Stage ){
    assert(Stage>=0 && Stage<8);
    return m_TextureOverride[Stage];
}
void RenderSystemDX9::SetTransparentTexOverride( int Stage ){
    static int texid=-1;
    if(texid==-1){
        texid = CreateTexture("transtex",2,2,cfARGB4444);
        int Pitch;
        BYTE* ptr = IRS->LockTexBits(texid,Pitch);
        if( ptr ){
            memset(ptr,0,Pitch*2);
            IRS->UnlockTexBits( texid );
        }
    }
	SetTextureOverride( texid,Stage );
	IRS->SetTexture(texid,3); 
}

//-----------------------------------------------------------------------------
// RenderSystemDX9::RefreshRateOverrideEnable
//-----------------------------------------------------------------------------
void RenderSystemDX9::RefreshRateOverrideEnable(const int Hz) {
	m_RefreshRateIsOverridden = true;
	m_HzOverriddenRefreshRate = Hz;
//	m_HzDefaultRefreshRate = m_d3dSettings.Fullscreen_DisplayMode.RefreshRate;
//	m_d3dSettings.Fullscreen_DisplayMode.RefreshRate = Hz;
//	m_d3dSettings.Windowed_DisplayMode.RefreshRate = Hz;
	Reset3DEnvironment();
} // RenderSystemDX9::RefreshRateOverrideEnable

//-----------------------------------------------------------------------------
// RenderSystemDX9::RefreshRateOverrideDisable
//-----------------------------------------------------------------------------
void RenderSystemDX9::RefreshRateOverrideDisable() {
	if(!m_RefreshRateIsOverridden) return;
//	m_d3dSettings.Fullscreen_DisplayMode.RefreshRate = m_HzDefaultRefreshRate;
//	m_d3dSettings.Windowed_DisplayMode.RefreshRate = m_HzDefaultRefreshRate;
	m_RefreshRateIsOverridden = false;
	Reset3DEnvironment();
} // RenderSystemDX9::RefreshRateOverrideDisable

//-----------------------------------------------------------------------------
// RenderSystemDX9::RefreshRateOverrideIsEnabled
//-----------------------------------------------------------------------------
bool RenderSystemDX9::RefreshRateOverrideIsEnabled(int *pHz) {
	if(!m_RefreshRateIsOverridden) {
		return false;
	}
	if(pHz) {
		*pHz = m_HzOverriddenRefreshRate;
	}
	return true;
} // RenderSystemDX9::RefreshRateOverrideIsEnabled
void RenderSystemDX9::EnablePostEffects	(bool Enable,float AutoBrightDegree,float AutoBrightSpeed,float ,float MotionBlurAlpha){
	if(Enable){
		if(m_BackbufferTextureID==0xFFFFFFFF){
			ScreenProp sp=GetScreenProp();			
            m_BackbufferTextureID=IRS->CreateTexture("PostEffBackbuffer",sp.m_Width,sp.m_Height,cfARGB8888,1,tmpDefault,true);						
			m_BackbufferZID=IRS->CreateTexture("PostEffectZBuffer",sp.m_Width,sp.m_Height,cfUnknown,1,tmpDefault,false,dsfD16);		
		}
		m_EnablePostEffects=true;
        m_AutoBrightSpeed=AutoBrightSpeed;
		m_AutoBrightDegree=AutoBrightDegree;
		m_MotionBlurAlpha=MotionBlurAlpha;
	}else{
		m_EnablePostEffects=false;
		if(m_BackbufferTextureID!=0xFFFFFFFF){
			IRS->DeleteTexture(m_BackbufferTextureID);
			m_BackbufferTextureID=0xFFFFFFFF;
		}
		if(m_BackbufferZID!=0xFFFFFFFF){
			IRS->DeleteTexture(m_BackbufferZID);
			m_BackbufferZID=0xFFFFFFFF;
		}
	}
}
void RenderSystemDX9::PreparePosteffects(){
	if(m_EnablePostEffects && m_BackbufferTextureID!=0xFFFFFFFF){
        PushRenderTarget(m_BackbufferTextureID,m_BackbufferZID);
	}
}
bool RenderSystemDX9::CopyRenderTarget(int RenderTargetID,int SysMemTexID){
	IDirect3DSurface9* RTS=GetDirect3DSurface(RenderTargetID);
	IDirect3DSurface9* SMS=GetDirect3DSurface(SysMemTexID);

	if(RTS && SMS){
		DX_CHK(m_pDevice->GetRenderTargetData(RTS,SMS));
		SAFE_DECREF(m_Textures[RenderTargetID]->GetTexture2D());
		SAFE_DECREF(m_Textures[SysMemTexID]->GetTexture2D());
		return true;
	}	
	return false;
}
void RenderSystemDX9::RenderPosteffects(){    
	if( m_EnablePostEffects && m_BackbufferTextureID!=0xFFFFFFFF ){        
		PopRenderTarget();
		static int Tm=GetTickCount();
        int Tc=GetTickCount();
		float dt=Tc-Tm;
		Tm=Tc;
		ScreenProp sp=GetScreenProp();
		if( sp.m_Width!=IRS->GetTextureWidth(m_BackbufferTextureID) 
				|| sp.m_Height!=IRS->GetTextureHeight(m_BackbufferTextureID) ){
			IRS->DeleteTexture(m_BackbufferTextureID);
			IRS->DeleteTexture(m_BackbufferZID);
			m_BackbufferTextureID=IRS->CreateTexture("PostEffBackbuffer",sp.m_Width,sp.m_Height,cfARGB8888,1,tmpDefault,true);
			m_BackbufferZID=IRS->CreateTexture("PostEffectZBuffer",sp.m_Width,sp.m_Height,cfUnknown,1,tmpDefault,false,dsfD16);
			if(m_BackbufferTextureID==0xFFFFFFFF){
				EnablePostEffects(false,m_AutoBrightDegree,m_AutoBrightSpeed,0,m_MotionBlurAlpha);
				return;
			}
		}

		
		static int Tex1=CreateTexture("BloomTex1",256,256,cfRGB888,1,tmpDefault,true);
		static int Tex2=CreateTexture("BloomTex2",128,128,cfRGB888,1,tmpDefault,true);
		static int Tex3=CreateTexture("BloomTex3", 64, 64,cfRGB888,1,tmpDefault,true);
		static int Tex4=CreateTexture("BloomTex4", 32, 32,cfARGB8888,1,tmpDefault,true);		

		static int Tex4s=CreateTexture("BloomTex4s", 32, 32,cfARGB8888,1,tmpSystem,false);


		int texs[3]={Tex2,Tex3,Tex4};		

		static int bd=IRS->GetShaderID("BloomDowngrade");
		static int bs=IRS->GetShaderID("BloomSampler");
		static int sh=IRS->GetShaderID("PostEffects");
		static int dg=IRS->GetShaderID("Downgrade");
		

		PushRenderTarget(Tex1,-1);
		static DWORD MColor=0xFFFFFFFF;
		rsSetTexture(m_BackbufferTextureID);
		rsSetTexture(-1,1);
		rsSetShader( bd );
        rsRect(Rct(0,0,256,256),Rct(0,0,1,1),0.99f,MColor,MColor,MColor,MColor);
		rsFlush();
		PopRenderTarget();

		PushRenderTarget(Tex2,-1);
		IRS->ClearDevice();
		rsSetTexture(Tex1);
		rsSetTexture(-1,1);
		rsSetShader( bs );
		rsRect(Rct(0,0,IRS->GetTextureWidth(Tex2),IRS->GetTextureHeight(Tex2)),Rct(0,0,1,1),0.99f,0xFF101010,0xFF101010,0xFF101010,0xFF101010);
		rsFlush();
		PopRenderTarget();

		static int dL=0;
		static float duL=0.5;
		static float duR=-0.5;

		for(int i=1;i<3;i++){
			PushRenderTarget(texs[i],-1);
			rsSetTexture(texs[i-1]);
			rsSetShader( dg );
			DWORD c=0xFFFFFFFF;			
			int pLx=IRS->GetTextureWidth(texs[i-1]);
			int Lx=IRS->GetTextureWidth(texs[i]);
			float dL=duL/Lx;
			float dR=duR/Lx;
			rsRect(Rct(0,0,Lx-dL,Lx-dL),Rct(dL,dL,1-dL-dR,1-dL-dR),0.99f,c,c,c,c);
			rsFlush();
			PopRenderTarget();
		}	
		CopyRenderTarget(Tex4,Tex4s);
		int pitch;
		BYTE* ptr=IRS->LockTexBits(Tex4s,Rct(0,0,8,8),pitch,0);
		if(ptr){
            Vector3D c(0,0,0);
			float w=0;
			for(int i=0;i<1024;i++){
				DWORD cL=((DWORD*)ptr)[i];
				float B=cL&255;
				float G=(cL>>8 )&255;
				float R=(cL>>16)&255;
				float W=pow(G,m_AutoBrightDegree);
				c.z+=B*W;
				c.y+=G*W;
				c.x+=R*W;
				w+=W;
			}
			if(w>0)c/=w;
			static Vector3D prev_c(0,0,0);			
			float d=1.0f/m_AutoBrightSpeed;
			d=1-pow(1-d,dt/30.0f);
			prev_c=prev_c*(1-d)+c*d;
			IRS->SetTextureFactor(DWORD(prev_c.z)+(DWORD(prev_c.y)<<8)+(DWORD(prev_c.x)<<16)+0xFF000000);
			IRS->UnlockTexBits(Tex4s);
		}   
		
		sp=IRS->GetScreenProp();
		rsSetTexture(m_BackbufferTextureID);		
		rsSetTexture(Tex2,1);
		rsSetShader( sh );

		float mba=1-pow(1-m_MotionBlurAlpha,dt/30.0f);
        DWORD MBlurColor=0xFFFFFF+(DWORD(mba*255)<<24);
		float dx=0.5f/sp.m_Width;
		float dy=0.5f/sp.m_Height;
		rsRect(Rct(0,0,sp.m_Width,sp.m_Height),Rct(dx,dy,1-dx,1-dy),0.99f,MBlurColor,MBlurColor,MBlurColor,MBlurColor);		
		rsFlush();
		rsRestoreShader();
	}
}