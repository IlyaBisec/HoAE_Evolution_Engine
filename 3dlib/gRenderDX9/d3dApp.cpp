//-----------------------------------------------------------------------------
// File: D3DApp.cpp
//
// Desc: Application class for the Direct3D samples framework library.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#include "gRenderPch.h"
#include "kLog.h"
#include "IRenderSystem.h"
#include "../kangaroo/resource.h"

//#define SHIPPING_VERSION

//-----------------------------------------------------------------------------
// Global access to the app (needed for the global WndProc())
//-----------------------------------------------------------------------------
static D3DApplication* g_pD3DApp = NULL;

//-----------------------------------------------------------------------------
// Name: D3DApplication()
// Desc: Constructor
//-----------------------------------------------------------------------------
D3DApplication::D3DApplication()
{
    g_pD3DApp                   = this;

    m_pD3D                      = NULL;
    m_pDevice                   = NULL;
    m_hWnd                      = NULL;
    m_hWndFocus                 = NULL;
    m_hMenu                     = NULL;
    m_bWindowed                 = true;
    m_bActive                   = false;
    m_bDeviceLost               = false;
    m_bMinimized                = false;
    m_bMaximized                = false;
    m_bIgnoreSizeChange         = false;
    m_bDeviceObjectsInited      = false;
    m_bDeviceObjectsRestored    = false;
    m_dwCreateFlags             = 0;

    m_bFrameMoving              = true;
    m_bSingleStep               = false;
    m_fTime                     = 0.0f;
    m_fElapsedTime              = 0.0f;
    m_fFPS                      = 0.0f;
    m_strDeviceStats[0]         = _T('\0');
    m_strFrameStats[0]          = _T('\0');

	m_strWindowTitle            = _T("3D-Brush");
    m_dwCreationWidth           = 640;
    m_dwCreationHeight          = 480;
    m_bShowCursorWhenFullscreen = false;
    m_bStartFullscreen          = false;
    m_bCreateMultithreadDevice  = false;
    m_bAllowDialogBoxMode       = false;

    m_bActive                   = false;

    // When m_bClipCursorWhenFullscreen is true, the cursor is limited to
    // the device window when the app goes fullscreen.  This prevents users
    // from accidentally clicking outside the app window on a multimon system.
    // This flag is turned off by default for debug builds, since it makes 
    // multimon debugging difficult.
#if defined(_DEBUG) || defined(DEBUG)
    m_bClipCursorWhenFullscreen = false;
#else
    m_bClipCursorWhenFullscreen = true;
#endif
} // D3DApplication::D3DApplication

//-----------------------------------------------------------------------------
// Name: WndProc()
// Desc: Static msg handler which passes messages to the application class.
//-----------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    g_pD3DApp->HostWndProc( hWnd, uMsg, wParam, lParam );
    return g_pD3DApp->MsgProc( hWnd, uMsg, wParam, lParam );
}

LRESULT D3DApplication::HostWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    if (m_HostWndProc) return m_HostWndProc( hWnd, uMsg, wParam, lParam );
    return 0;
} // D3DApplication::HostWndProc

//-----------------------------------------------------------------------------
// Name: ConfirmDeviceHelper()
// Desc: Static function used by D3DEnumeration
//-----------------------------------------------------------------------------
bool D3DApplication::ConfirmDeviceHelper( D3DCAPS9* pCaps, VertexProcessingType vertexProcessingType, 
                                            D3DFORMAT adapterFormat, D3DFORMAT backBufferFormat )
{
    DWORD dwBehavior;

    if (vertexProcessingType == vpSoftware) dwBehavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    else if (vertexProcessingType == vpMixed) dwBehavior = D3DCREATE_MIXED_VERTEXPROCESSING;
    else if (vertexProcessingType == vpHardware) dwBehavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    else if (vertexProcessingType == vpPureHardware)
        dwBehavior = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
    else
        dwBehavior = 0; // TODO: throw exception
    
    return SUCCEEDED( g_pD3DApp->ConfirmDevice( pCaps, dwBehavior, adapterFormat, backBufferFormat ) );
} // D3DApplication::ConfirmDeviceHelper

//-----------------------------------------------------------------------------
// Name: Create()
// Desc: Here's what this function does:
//       - Checks to make sure app is still active (if fullscreen, etc)
//       - Checks to see if it is time to draw with DXUtil_Timer, if not, it just returns S_OK
//       - Calls Render() to draw the new frame
//       - Updates some frame count statistics
//       - Calls m_pDevice->Present() to display the rendered frame.
//-----------------------------------------------------------------------------
HRESULT D3DApplication::Create( HINSTANCE hInstance, HWND hWnd )
{
    HRESULT hr;
    assert( !m_hWnd );
    m_hWnd = hWnd;

    // Create the Direct3D object
    m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
    if (m_pD3D == NULL) return DisplayErrorMsg( D3DAPPERR_NODIRECT3D, MSGERR_APPMUSTEXIT );

    //  get current desktop display mode in order to restore it at shutdown
    EnumDisplaySettings( NULL, ENUM_CURRENT_SETTINGS, &m_InitialDesktopMode );

    // Build a list of Direct3D adapters, modes and devices. The
    // ConfirmDevice() callback is used to confirm that only devices that
    // meet the app's requirements are considered.
    m_d3dEnum.SetD3D( m_pD3D );
    m_d3dEnum.ConfirmDevice              = ConfirmDeviceHelper;
    m_d3dEnum.m_AppMinFullscreenWidth    = 800;
    m_d3dEnum.m_AppMinFullscreenHeight   = 600;
    if (FAILED( hr = m_d3dEnum.Enumerate() ))
    {
        SAFE_RELEASE( m_pD3D );
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
    }

    // Unless a substitute hWnd has been specified, create a window to
    // render into
    if (m_hWnd == NULL)
    {
        // Register the windows class
		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(wc));
		wc.cbSize = sizeof(wc);
		wc.style = CS_DBLCLKS;
		wc.lpfnWndProc = WndProc;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDR_MAINFRAME));
		wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDR_MAINFRAME));
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wc.lpszClassName = "D3D Window";

/*
        WNDCLASS wndClass = { CS_DBLCLKS, WndProc, 0, 0, hInstance,
                              LoadIcon( hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON) ),
                              LoadCursor( NULL, IDC_ARROW ),
                              (HBRUSH)GetStockObject(WHITE_BRUSH),
                              NULL, _T("D3D Window") };
        RegisterClass( &wndClass );*/
		RegisterClassEx(&wc);

        // Set the window's initial style
        m_dwWindowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | 
                          WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE | WS_MAXIMIZE;
        //m_dwWindowStyle = WS_POPUP;
        
        // Set the window's initial width
        RECT rc;
        SetRect( &rc, 0, 0, m_dwCreationWidth, m_dwCreationHeight );        
        
        // Create the render window
        m_hWnd = CreateWindow( _T("D3D Window"), m_strWindowTitle, m_dwWindowStyle,
                               0, 0,
                               (rc.right-rc.left), (rc.bottom-rc.top), 0,
                               NULL, hInstance, 0 );


    }
    else
    {
        //  hook in window message procedure
        m_HostWndProc = (WNDPROC)SetWindowLong( m_hWnd, GWL_WNDPROC, (LONG)WndProc );
    }

    // The focus window can be a specified to be a different window than the
    // device window.  If not, use the device window as the focus window.
    if( m_hWndFocus == NULL ) m_hWndFocus = m_hWnd;

    // Save window properties
    m_dwWindowStyle = GetWindowLong( m_hWnd, GWL_STYLE );
    GetWindowRect( m_hWnd, &m_rcWindowBounds );
    GetClientRect( m_hWnd, &m_rcWindowClient );

    if( FAILED( hr = ChooseInitialD3DSettings() ) )
    {
        SAFE_RELEASE( m_pD3D );
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
    }

    // Initialize the application timer
    DXUtil_Timer( TIMER_START );

    // Initialize the app's custom scene stuff
    if( FAILED( hr = OneTimeSceneInit() ) )
    {
        SAFE_RELEASE( m_pD3D );
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
    }

    // Initialize the 3D environment for the app
    if( FAILED( hr = Initialize3DEnvironment() ) )
    {
        SAFE_RELEASE( m_pD3D );
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
    }

    // The app is ready to go
    m_bActive = true;

    return S_OK;
} // D3DApplication::Create

//-----------------------------------------------------------------------------
// Name: FindBestWindowedMode()
// Desc: Sets up m_d3dSettings with best available windowed mode, subject to 
//       the bRequireHAL and bRequireREF constraints.  Returns false if no such
//       mode can be found.
//-----------------------------------------------------------------------------
bool D3DApplication::FindBestWindowedMode( bool bRequireHAL, bool bRequireREF )
{
    // Get display mode of primary adapter (which is assumed to be where the window 
    // will appear)
    D3DDISPLAYMODE desktopMode;
    m_pD3D->GetAdapterDisplayMode( 0, &desktopMode );

    D3DAdapterInfo* pBestAdapterInfo = NULL;
    D3DDeviceInfo*  pBestDeviceInfo = NULL;
    D3DDeviceCombo* pBestDeviceCombo = NULL;

    bool bEndSearch = false;
    for (int i = 0; i < m_d3dEnum.m_AdapterInfoList.size(); i++ )
    {
        D3DAdapterInfo& adInfo = m_d3dEnum.m_AdapterInfoList[i];
        for( UINT j = 0; j < adInfo.m_DevInfos.size(); j++ )
        {
            D3DDeviceInfo& devInfo = adInfo.m_DevInfos[j];
            if (bRequireHAL && devInfo.m_DevType != D3DDEVTYPE_HAL) continue;
            if (bRequireREF && devInfo.m_DevType != D3DDEVTYPE_REF) continue;
            for (int k = 0; k < devInfo.m_DevCombos.size(); k++)
            {
                D3DDeviceCombo& devCombo = devInfo.m_DevCombos[k];
                bool bAdapterMatchesBB = (devCombo.m_BackBufferFormat == devCombo.m_AdapterFormat);
                if (!devCombo.m_bIsWindowed) continue;

                if (devCombo.m_BackBufferFormat != D3DFMT_R5G6B5) continue;
                if (devCombo.m_AdapterFormat != desktopMode.Format) continue;
                // If we haven't found a compatible DeviceCombo yet, or if this set
                // is better (because it's a HAL, and/or because formats match better), save it
                if (pBestDeviceCombo == NULL || 
                    pBestDeviceCombo->m_DevType != D3DDEVTYPE_HAL && 
                    devCombo.m_DevType == D3DDEVTYPE_HAL ||
                    (devCombo.m_DevType == D3DDEVTYPE_HAL && bAdapterMatchesBB))
                {
                    pBestAdapterInfo    = &adInfo;
                    pBestDeviceInfo     = &devInfo;
                    pBestDeviceCombo    = &devCombo;
                    if (devCombo.m_DevType == D3DDEVTYPE_HAL && bAdapterMatchesBB)
                    {
                        // This windowed device combo looks great -- take it
                        bEndSearch = true;
                    }
                    // Otherwise keep looking for a better windowed device combo
                }
                if (bEndSearch) break;
            }
            if (bEndSearch) break;
        }
        if (bEndSearch) break;
    }
    
    if (pBestDeviceCombo == NULL ) return false;

    D3DSettings& s = m_d3dSettings;
    s.pWindowed_AdapterInfo         = pBestAdapterInfo;
    s.pWindowed_DeviceInfo          = pBestDeviceInfo;
    s.pWindowed_DeviceCombo         = pBestDeviceCombo;
    s.m_bIsWindowed                 = true;
    s.Windowed_DisplayMode          = desktopMode;
    s.Windowed_Width                = m_rcWindowClient.right - m_rcWindowClient.left;
    s.Windowed_Height               = m_rcWindowClient.bottom - m_rcWindowClient.top;
	s.Windowed_MultisampleType = s.Windowed_WorstMultisampleType = pBestDeviceCombo->m_MSampleTypes[0];
	s.Windowed_BestMultisampleType	= pBestDeviceCombo->m_MSampleTypes[pBestDeviceCombo->m_MSampleTypes.size() - 1];

    s.Windowed_MultisampleQuality   = 0;
    s.Windowed_VertexProcessingType = pBestDeviceCombo->m_VProcessTypes[0];
    s.Windowed_PresentInterval      = pBestDeviceCombo->m_PresentIntervals[0];
    if (m_d3dEnum.m_bUseZBuffer)
    {
        s.Windowed_DepthStencilBufferFormat = pBestDeviceCombo->m_DepthStencilFormats[0];
    }
	
	return true;
} // D3DApplication::FindBestWindowedMode

//-----------------------------------------------------------------------------
// Name: FindBestFullscreenMode()
// Desc: Sets up m_d3dSettings with best available fullscreen mode, subject to 
//       the bRequireHAL and bRequireREF constraints.  Returns false if no such
//       mode can be found.
//-----------------------------------------------------------------------------
bool D3DApplication::FindBestFullscreenMode( bool bRequireHAL, bool bRequireREF )
{
    // For fullscreen, default to first HAL DeviceCombo that supports the current desktop 
    // display mode, or any display mode if HAL is not compatible with the desktop mode, or 
    // non-HAL if no HAL is available
    D3DDISPLAYMODE deskMode;
    D3DDISPLAYMODE bestDeskMode;
    D3DDISPLAYMODE bestDisplayMode;
    bestDeskMode.Width = 0;
    bestDeskMode.Height = 0;
    bestDeskMode.Format = D3DFMT_UNKNOWN;
    bestDeskMode.RefreshRate = 0;

    D3DAdapterInfo* pBestAdapterInfo = NULL;
    D3DDeviceInfo* pBestDeviceInfo = NULL;
    D3DDeviceCombo* pBestDeviceCombo = NULL;

    for( UINT i = 0; i < m_d3dEnum.m_AdapterInfoList.size(); i++ )
    {
        D3DAdapterInfo& adInfo = m_d3dEnum.m_AdapterInfoList[i];
        m_pD3D->GetAdapterDisplayMode( adInfo.m_Ordinal, &deskMode );
        for( UINT j = 0; j < adInfo.m_DevInfos.size(); j++ )
        {
            D3DDeviceInfo& devInfo = adInfo.m_DevInfos[j];
            if (bRequireHAL && devInfo.m_DevType != D3DDEVTYPE_HAL) continue;
            if (bRequireREF && devInfo.m_DevType != D3DDEVTYPE_REF) continue;
            for( UINT k = 0; k < devInfo.m_DevCombos.size(); k++ )
            {
                D3DDeviceCombo& devCombo  = devInfo.m_DevCombos[k];
                bool bAdapterMatchesBB          = (devCombo.m_BackBufferFormat == devCombo.m_AdapterFormat);
                bool bAdapterMatchesDesktop     = (devCombo.m_AdapterFormat == deskMode.Format);
                if (devCombo.m_bIsWindowed) continue;
                // If we haven't found a compatible set yet, or if this set
                // is better (because it's a HAL, and/or because formats match better),
                // save it
                if (pBestDeviceCombo == NULL ||
                    pBestDeviceCombo->m_DevType != D3DDEVTYPE_HAL && 
                    devInfo.m_DevType == D3DDEVTYPE_HAL ||
                    devCombo.m_DevType == D3DDEVTYPE_HAL && 
                    pBestDeviceCombo->m_AdapterFormat != deskMode.Format && 
                    bAdapterMatchesDesktop ||
                    devCombo.m_DevType == D3DDEVTYPE_HAL && 
                    bAdapterMatchesDesktop && 
                    bAdapterMatchesBB )
                {
                    bestDeskMode        = deskMode;
                    pBestAdapterInfo    = &adInfo;
                    pBestDeviceInfo     = &devInfo;
                    pBestDeviceCombo    = &devCombo;
                    if (devInfo.m_DevType == D3DDEVTYPE_HAL && bAdapterMatchesDesktop && bAdapterMatchesBB)
                    {
                        // This fullscreen device combo looks great -- take it
                        goto EndFullscreenDeviceComboSearch;
                    }
                    // Otherwise keep looking for a better fullscreen device combo
                }
            }
        }
    }
EndFullscreenDeviceComboSearch:
    if (pBestDeviceCombo == NULL)
        return false;

    // Need to find a display mode on the best adapter that uses pBestDeviceCombo->m_AdapterFormat
    // and is as close to bestDeskMode's res as possible
    bestDisplayMode.Width = 0;
    bestDisplayMode.Height = 0;
    bestDisplayMode.Format = D3DFMT_UNKNOWN;
    bestDisplayMode.RefreshRate = 0;
    for( UINT idm = 0; idm < pBestAdapterInfo->m_DisplayModes.size(); idm++ )
    {
        D3DDISPLAYMODE* pdm = &pBestAdapterInfo->m_DisplayModes[idm];
        if( pdm->Format != pBestDeviceCombo->m_AdapterFormat )
            continue;
        if( pdm->Width == bestDeskMode.Width &&
            pdm->Height == bestDeskMode.Height && 
            pdm->RefreshRate == bestDeskMode.RefreshRate )
        {
            // found a perfect match, so stop
            bestDisplayMode = *pdm;
            break;
        }
        else if( pdm->Width == bestDeskMode.Width &&
                 pdm->Height == bestDeskMode.Height && 
                 pdm->RefreshRate > bestDisplayMode.RefreshRate )
        {
            // refresh rate doesn't match, but width/height match, so keep this
            // and keep looking
            bestDisplayMode = *pdm;
        }
        else if( pdm->Width == bestDeskMode.Width )
        {
            // width matches, so keep this and keep looking
            bestDisplayMode = *pdm;
        }
        else if( bestDisplayMode.Width == 0 )
        {
            // we don't have anything better yet, so keep this and keep looking
            bestDisplayMode = *pdm;
        }
    }

    m_d3dSettings.pFullscreen_AdapterInfo                   = pBestAdapterInfo;
    m_d3dSettings.pFullscreen_DeviceInfo                    = pBestDeviceInfo;
    m_d3dSettings.pFullscreen_DeviceCombo                   = pBestDeviceCombo;
    m_d3dSettings.m_bIsWindowed                             = false;
    m_d3dSettings.Fullscreen_DisplayMode                    = bestDisplayMode;
    if (m_d3dEnum.m_bUseZBuffer)
        m_d3dSettings.Fullscreen_DepthStencilBufferFormat   = pBestDeviceCombo->m_DepthStencilFormats[0];
	m_d3dSettings.Fullscreen_MultisampleType = m_d3dSettings.Fullscreen_WorstMultisampleType = pBestDeviceCombo->m_MSampleTypes[0];
	m_d3dSettings.Fullscreen_BestMultisampleType			= pBestDeviceCombo->m_MSampleTypes[pBestDeviceCombo->m_MSampleTypes.size() - 1];
    m_d3dSettings.Fullscreen_MultisampleQuality             = 0;
    m_d3dSettings.Fullscreen_VertexProcessingType           = pBestDeviceCombo->m_VProcessTypes[0];
    m_d3dSettings.Fullscreen_PresentInterval                = D3DPRESENT_INTERVAL_DEFAULT;
	
	return true;
} // D3DApplication::FindBestFullscreenMode

//-----------------------------------------------------------------------------
// Name: ChooseInitialD3DSettings()
//-----------------------------------------------------------------------------
HRESULT D3DApplication::ChooseInitialD3DSettings()
{
    bool bFoundFullscreen = FindBestFullscreenMode( false, false );
    bool bFoundWindowed = FindBestWindowedMode( false, false );
    m_d3dSettings.SetDeviceClip( false );

    if( m_bStartFullscreen && bFoundFullscreen )
        m_d3dSettings.m_bIsWindowed = false;
    if( !bFoundWindowed && bFoundFullscreen )
        m_d3dSettings.m_bIsWindowed = false;

    if( !bFoundFullscreen && !bFoundWindowed )
        return D3DAPPERR_NOCOMPATIBLEDEVICES;

    return S_OK;
} // D3DApplication::ChooseInitialD3DSettings

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: Message handling function.  Here's what this function does:
//       - WM_PAINT: calls Render() and Present() is called if !m_bReady
//       - WM_EXITSIZEMOVE: window size recalc'd and calls HandlePossibleSizeChange()
//       - WM_CLOSE: calls Cleanup3dEnvironment(), DestroyMenu(), DestroyWindow(), PostQuitMessage()
//       - WM_COMMAND: IDM_CHANGEDEVICE calls UserSelectNewDevice() to select a new device
//       - WM_COMMAND: IDM_TOGGLEFULLSCREEN calls ToggleFullScreen() to toggle 
//                  between fullscreen and windowed
//       - WM_COMMAND: IDM_EXIT: shuts down the app with a WM_CLOSE 
//       - anything not handled goes to DefWindowProc()     
//-----------------------------------------------------------------------------
LRESULT D3DApplication::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
        case WM_PAINT:
            //// Handle paint messages when the app is paused
            //if( m_pDevice && !m_bActive && 
            //    m_bDeviceObjectsInited && m_bDeviceObjectsRestored )
            //{
            //    HRESULT hr;
            //    hr = m_pDevice->Present( NULL, NULL, NULL, NULL );
            //    if( D3DERR_DEVICELOST == hr )
            //        m_bDeviceLost = true;
            //}
            break;

        case WM_GETMINMAXINFO:
            ((MINMAXINFO*)lParam)->ptMinTrackSize.x = 100;
            ((MINMAXINFO*)lParam)->ptMinTrackSize.y = 100;
            break;

        case WM_ENTERSIZEMOVE:
            // Halt frame movement while the app is sizing or moving
            m_bActive = false;
            break;

        case WM_SIZE:
            // Pick up possible changes to window style due to maximize, etc.
            if( m_bWindowed && m_hWnd != NULL )
                m_dwWindowStyle = GetWindowLong( m_hWnd, GWL_STYLE );

            if( SIZE_MINIMIZED == wParam )
            {
                if( m_bClipCursorWhenFullscreen && !m_bWindowed )
                    ClipCursor( NULL );
                m_bActive = false;
                m_bMinimized = true;
                m_bMaximized = false;
            }
            else if( SIZE_MAXIMIZED == wParam )
            {
                if( m_bMinimized ) m_bActive = true;
                m_bMinimized = false;
                m_bMaximized = true;
                HandlePossibleSizeChange();
            }
            else if( SIZE_RESTORED == wParam )
            {
                if( m_bMaximized )
                {
                    m_bMaximized = false;
                    HandlePossibleSizeChange();
                }
                else if( m_bMinimized)
                {
                    m_bActive = true;
                    m_bMinimized = false;
                    HandlePossibleSizeChange();
                }
                else
                {
                    // If we're neither maximized nor minimized, the window size 
                    // is changing by the user dragging the window edges.  In this 
                    // case, we don't reset the device yet -- we wait until the 
                    // user stops dragging, and a WM_EXITSIZEMOVE message comes.
                }
            }
            break;

        case WM_EXITSIZEMOVE:
            m_bActive = true;
            HandlePossibleSizeChange();
            break;

        case WM_SETCURSOR:
            // Turn off Windows cursor in fullscreen mode
            if( m_bActive && !m_bWindowed )
            {
                SetCursor( NULL );
                if( m_bShowCursorWhenFullscreen )
                    m_pDevice->ShowCursor( true );
                return true; // prevent Windows from setting cursor to window class cursor
            }
            break;

         case WM_MOUSEMOVE:
            if( m_bActive && m_pDevice != NULL )
            {
                POINT ptCursor;
                GetCursorPos( &ptCursor );
                if( !m_bWindowed )
                    ScreenToClient( m_hWnd, &ptCursor );
                m_pDevice->SetCursorPosition( ptCursor.x, ptCursor.y, 0 );
            }
            break;

       case WM_ENTERMENULOOP:
            m_bActive = false;
            break;

        case WM_EXITMENULOOP:
            m_bActive = true;
            break;

        case WM_NCHITTEST:
            // Prevent the user from selecting the menu in fullscreen mode
            if( !m_bWindowed )
                return HTCLIENT;
            break;

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
                    if( false == m_bWindowed )
                        return 1;
                    break;
            }
            break;

        case WM_COMMAND:
            switch( LOWORD(wParam) )
            {
                case IDM_TOGGLESTART:
                    // Toggle frame movement
                    m_bFrameMoving = !m_bFrameMoving;
                    DXUtil_Timer( m_bFrameMoving ? TIMER_START : TIMER_STOP );
                    break;

                case IDM_SINGLESTEP:
                    // Single-step frame movement
                    if( false == m_bFrameMoving )
                        DXUtil_Timer( TIMER_ADVANCE );
                    else
                        DXUtil_Timer( TIMER_STOP );
                    m_bFrameMoving = false;
                    m_bSingleStep  = true;
                    break;

                case IDM_CHANGEDEVICE:
                    // Prompt the user to select a new device or mode
                    m_bActive = false;
                    UserSelectNewDevice();
                    m_bActive = true;
                    return 0;

                case IDM_TOGGLEFULLSCREEN:
                    // Toggle the fullscreen/window mode
                    m_bActive = false;
                    if( FAILED( ToggleFullscreen() ) )
                        DisplayErrorMsg( D3DAPPERR_RESETFAILED, MSGERR_APPMUSTEXIT );
                    m_bActive = true;                        
                    return 0;

                case IDM_EXIT:
                    // received key/menu command to exit app
                    SendMessage( hWnd, WM_CLOSE, 0, 0 );
                    return 0;
            }
            break;

        case WM_ACTIVATE: 
            m_bActive = ( wParam != WA_INACTIVE );
            break;
        case WM_CLOSE:
            Cleanup3DEnvironment();
            SAFE_RELEASE( m_pD3D );
            FinalCleanup();
            DestroyWindow( hWnd );
            PostQuitMessage(0);
            m_hWnd = NULL;
            return 0;
    }

    return DefWindowProc( hWnd, uMsg, wParam, lParam );
} // D3DApplication::MsgProc

//-----------------------------------------------------------------------------
// Name: HandlePossibleSizeChange()
// Desc: Reset the device if the client area size has changed.
//-----------------------------------------------------------------------------
HRESULT D3DApplication::HandlePossibleSizeChange()
{
    HRESULT hr = S_OK;
    RECT rcClientOld;
    rcClientOld = m_rcWindowClient;

    if( m_bIgnoreSizeChange )
        return S_OK;

    // Update window properties
    GetWindowRect( m_hWnd, &m_rcWindowBounds );
    GetClientRect( m_hWnd, &m_rcWindowClient );

    if( rcClientOld.right - rcClientOld.left !=
        m_rcWindowClient.right - m_rcWindowClient.left ||
        rcClientOld.bottom - rcClientOld.top !=
        m_rcWindowClient.bottom - m_rcWindowClient.top)
    {
        // A new window size will require a new backbuffer
        // size, so the 3D structures must be changed accordingly.
        m_bActive = false;

        m_d3dpp.BackBufferWidth  = m_rcWindowClient.right - m_rcWindowClient.left;
        m_d3dpp.BackBufferHeight = m_rcWindowClient.bottom - m_rcWindowClient.top;

        m_d3dSettings.Windowed_Width = m_d3dpp.BackBufferWidth;
        m_d3dSettings.Windowed_Height = m_d3dpp.BackBufferHeight;
    
        if( m_pDevice != NULL )
        {
            // Reset the 3D environment
            if( FAILED( hr = Reset3DEnvironment() ) )
            {
                if( hr == D3DERR_DEVICELOST )
                {
                    m_bDeviceLost = true;
                    hr = S_OK;
                }
                else
                {
                    if( hr != D3DERR_OUTOFVIDEOMEMORY )
                        hr = D3DAPPERR_RESETFAILED;

                    DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
                }
            }
        }
        m_bActive = true;
    }
    return hr;
} // D3DApplication::HandlePossibleSizeChange

//-----------------------------------------------------------------------------
// Name: Initialize3DEnvironment()
// Desc: Usually this function is not overridden.  Here's what this function does:
//       - Sets the windowed flag to be either windowed or fullscreen
//       - Sets parameters for z-buffer depth and back buffer
//       - Creates the D3D device
//       - Sets the window position (if windowed, that is)
//       - Makes some determinations as to the abilites of the driver (HAL, etc)
//       - Sets up some cursor stuff
//       - Calls InitDeviceObjects()
//       - Calls RestoreDeviceObjects()
//       - If all goes well, m_bActive is set to TRUE, and the function returns
//       - Otherwise, initialization is reattempted using the reference device
//-----------------------------------------------------------------------------


HRESULT D3DApplication::Initialize3DEnvironment()
{
    HRESULT hr;

    D3DAdapterInfo* pAdapterInfo = m_d3dSettings.PAdapterInfo();
    D3DDeviceInfo* pDeviceInfo = m_d3dSettings.PDeviceInfo();

    m_bWindowed = m_d3dSettings.m_bIsWindowed;

    // Prepare window for possible windowed/fullscreen change
    AdjustWindowForChange();

    // Set up the presentation parameters
    BuildPresentParamsFromSettings();

    if( pDeviceInfo->m_Caps.PrimitiveMiscCaps & D3DPMISCCAPS_NULLREFERENCE )
    {
        // Warn user about null ref device that can't render anything
        DisplayErrorMsg( D3DAPPERR_NULLREFDEVICE, 0 );
    }

    DWORD behaviorFlags;
    if (m_d3dSettings.GetVertexProcessingType() == vpSoftware)
        behaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    else if (m_d3dSettings.GetVertexProcessingType() == vpMixed)
        behaviorFlags = D3DCREATE_MIXED_VERTEXPROCESSING;
    else if (m_d3dSettings.GetVertexProcessingType() == vpHardware)
        behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    else if (m_d3dSettings.GetVertexProcessingType() == vpPureHardware)
        behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
    else
        behaviorFlags = 0; // TODO: throw exception

    // Add multithreaded flag if requested by app
    if( m_bCreateMultithreadDevice )
        behaviorFlags |= D3DCREATE_MULTITHREADED;

    // Create the device
    //
    // Set default settings
    UINT AdapterToUse=D3DADAPTER_DEFAULT; 
    D3DDEVTYPE DeviceType=D3DDEVTYPE_HAL; 
#ifdef SHIPPING_VERSION 
    // When building a shipping version, disable NVPerfHUD (opt-out) 
#else // Look for 'NVIDIA NVPerfHUD' adapter 
    // If it is present, override default settings 
    for (UINT Adapter=0;Adapter<m_pD3D->GetAdapterCount();Adapter++) { 
        D3DADAPTER_IDENTIFIER9 Identifier; 
        HRESULT Res; 
        Res = m_pD3D->GetAdapterIdentifier(Adapter,0,&Identifier); 
        if (strcmp(Identifier.Description,"NVIDIA NVPerfHUD") == 0) { 
            AdapterToUse=Adapter; 
            DeviceType=D3DDEVTYPE_REF; 
            break; 
        } 
    }
#endif 
    if (FAILED(m_pD3D->CreateDevice( AdapterToUse, DeviceType,
        m_hWndFocus, D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_d3dpp, &m_pDevice) ) ) 
    { 
        return E_FAIL; 
    }

    //hr = m_pD3D->CreateDevice( m_d3dSettings.Ordinal(), pDeviceInfo->m_DevType,
    //                           m_hWndFocus, behaviorFlags, &m_d3dpp, &m_pDevice );
    //
    //if( SUCCEEDED(hr) )
    {
        // When moving from fullscreen to windowed mode, it is important to
        // adjust the window size after recreating the device rather than
        // beforehand to ensure that you get the window size you want.  For
        // example, when switching from 640x480 fullscreen to windowed with
        // a 1000x600 window on a 1024x768 desktop, it is impossible to set
        // the window size to 1000x600 until after the display mode has
        // changed to 1024x768, because windows cannot be larger than the
        // desktop.
        if( m_bWindowed )
        {
            SetWindowPos( m_hWnd, HWND_NOTOPMOST,
                          m_rcWindowBounds.left, m_rcWindowBounds.top,
                          ( m_rcWindowBounds.right - m_rcWindowBounds.left ),
                          ( m_rcWindowBounds.bottom - m_rcWindowBounds.top ),
                          SWP_SHOWWINDOW );
        }

        // Store device Caps
        m_pDevice->GetDeviceCaps( &m_Caps );
        m_dwCreateFlags = behaviorFlags;

        // Store device description
        if( pDeviceInfo->m_DevType == D3DDEVTYPE_REF )
            lstrcpy( m_strDeviceStats, TEXT("REF") );
        else if( pDeviceInfo->m_DevType == D3DDEVTYPE_HAL )
            lstrcpy( m_strDeviceStats, TEXT("HAL") );
        else if( pDeviceInfo->m_DevType == D3DDEVTYPE_SW )
            lstrcpy( m_strDeviceStats, TEXT("SW") );

        if( behaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING &&
            behaviorFlags & D3DCREATE_PUREDEVICE )
        {
            if( pDeviceInfo->m_DevType == D3DDEVTYPE_HAL )
                lstrcat( m_strDeviceStats, TEXT(" (pure hw vp)") );
            else
                lstrcat( m_strDeviceStats, TEXT(" (simulated pure hw vp)") );
        }
        else if( behaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING )
        {
            if( pDeviceInfo->m_DevType == D3DDEVTYPE_HAL )
                lstrcat( m_strDeviceStats, TEXT(" (hw vp)") );
            else
                lstrcat( m_strDeviceStats, TEXT(" (simulated hw vp)") );
        }
        else if( behaviorFlags & D3DCREATE_MIXED_VERTEXPROCESSING )
        {
            if( pDeviceInfo->m_DevType == D3DDEVTYPE_HAL )
                lstrcat( m_strDeviceStats, TEXT(" (mixed vp)") );
            else
                lstrcat( m_strDeviceStats, TEXT(" (simulated mixed vp)") );
        }
        else if( behaviorFlags & D3DCREATE_SOFTWARE_VERTEXPROCESSING )
        {
            lstrcat( m_strDeviceStats, TEXT(" (sw vp)") );
        }

        if( pDeviceInfo->m_DevType == D3DDEVTYPE_HAL )
        {
            // Be sure not to overflow m_strDeviceStats when appending the adapter 
            // description, since it can be long.  Note that the adapter description
            // is initially CHAR and must be converted to TCHAR.
            lstrcat( m_strDeviceStats, TEXT(": ") );
            const int cchDesc = sizeof(pAdapterInfo->m_Identifier.Description);
            TCHAR szDescription[cchDesc];
            DXUtil_ConvertAnsiStringToGenericCch( szDescription, 
                pAdapterInfo->m_Identifier.Description, cchDesc );
            int maxAppend = sizeof(m_strDeviceStats) / sizeof(TCHAR) -
                lstrlen( m_strDeviceStats ) - 1;
            _tcsncat( m_strDeviceStats, szDescription, maxAppend );
        }

        // Store render target surface desc
        LPDIRECT3DSURFACE9 pBackBuffer = NULL;
        m_pDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
        pBackBuffer->GetDesc( &m_d3dsdBackBuffer );
        pBackBuffer->Release();

        // Set up the fullscreen cursor
        if( m_bShowCursorWhenFullscreen && !m_bWindowed )
        {
            HCURSOR hCursor;
#ifdef _WIN64
            hCursor = (HCURSOR)GetClassLongPtr( m_hWnd, GCLP_HCURSOR );
#else
            hCursor = (HCURSOR)ULongToHandle( GetClassLong( m_hWnd, GCL_HCURSOR ) );
#endif
            D3DUtil_SetDeviceCursor( m_pDevice, hCursor, true );
            m_pDevice->ShowCursor( true );
        }

        // Confine cursor to fullscreen window
        if( m_bClipCursorWhenFullscreen )
        {
            if (!m_bWindowed )
            {
                RECT rcWindow;
                GetWindowRect( m_hWnd, &rcWindow );
                ClipCursor( &rcWindow );
            }
            else
            {
                ClipCursor( NULL );
            }
        }

        // Initialize the app's device-dependent objects
        hr = InitDeviceObjects();
        if( FAILED(hr) )
        {
            DeleteDeviceObjects();
        }
        else
        {
            m_bDeviceObjectsInited = true;
            hr = RestoreDeviceObjects();
            if( FAILED(hr) )
            {
                InvalidateDeviceObjects();
            }
            else
            {
                m_bDeviceObjectsRestored = true;
                return S_OK;
            }
        }

        // Cleanup before we try again
        Cleanup3DEnvironment();
    }

    // If that failed, fall back to the reference rasterizer
    if (hr != D3DAPPERR_MEDIANOTFOUND && 
        hr != HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND ) && 
        pDeviceInfo->m_DevType == D3DDEVTYPE_HAL)
    {
        if (FindBestWindowedMode(false, true))
        {
            m_bWindowed = true;
            AdjustWindowForChange();
            // Make sure main window isn't topmost, so error message is visible
            SetWindowPos( m_hWnd, HWND_NOTOPMOST,
                          m_rcWindowBounds.left, m_rcWindowBounds.top,
                          ( m_rcWindowBounds.right - m_rcWindowBounds.left ),
                          ( m_rcWindowBounds.bottom - m_rcWindowBounds.top ),
                          SWP_SHOWWINDOW );

            // Let the user know we are switching from HAL to the reference rasterizer
            //DisplayErrorMsg( hr, MSGWARN_SWITCHEDTOREF );

            hr = Initialize3DEnvironment();
        }
    }
    return hr;
} // D3DApplication::Initialize3DEnvironment

//-----------------------------------------------------------------------------
// Name: BuildPresentParamsFromSettings()
// Desc:
//-----------------------------------------------------------------------------
void D3DApplication::BuildPresentParamsFromSettings()
{
    m_d3dpp.Windowed               = m_d3dSettings.m_bIsWindowed;
    m_d3dpp.BackBufferCount        = 1;
    m_d3dpp.MultiSampleType        = m_d3dSettings.MultisampleType();
    m_d3dpp.MultiSampleQuality     = m_d3dSettings.MultisampleQuality();
    m_d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
    m_d3dpp.EnableAutoDepthStencil = m_d3dEnum.m_bUseZBuffer;
    m_d3dpp.hDeviceWindow          = m_hWnd;
    if (m_d3dEnum.m_bUseZBuffer)
    {
        m_d3dpp.Flags              = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
        m_d3dpp.AutoDepthStencilFormat = m_d3dSettings.DepthStencilBufferFormat();
    }
    else
    {
        m_d3dpp.Flags              = 0;
    }

    if (m_d3dSettings.DeviceClip()) m_d3dpp.Flags |= D3DPRESENTFLAG_DEVICECLIP;
    if (m_bWindowed)
    {
        m_d3dpp.BackBufferWidth  = m_d3dSettings.Windowed_Width;
        m_d3dpp.BackBufferHeight = m_d3dSettings.Windowed_Height;
        m_d3dpp.BackBufferFormat = m_d3dSettings.BackBufferFormat();//m_d3dSettings.PDeviceCombo()->m_BackBufferFormat;
        m_d3dpp.FullScreen_RefreshRateInHz = 0;
        m_d3dpp.PresentationInterval = m_d3dSettings.PresentInterval();
    }
    else
    {
        m_d3dpp.BackBufferWidth  = m_d3dSettings.DisplayMode().Width;
        m_d3dpp.BackBufferHeight = m_d3dSettings.DisplayMode().Height;
        m_d3dpp.BackBufferFormat = m_d3dSettings.BackBufferFormat();//m_d3dSettings.PDeviceCombo()->m_BackBufferFormat;
        m_d3dpp.FullScreen_RefreshRateInHz = m_d3dSettings.Fullscreen_DisplayMode.RefreshRate;
        m_d3dpp.PresentationInterval = m_d3dSettings.PresentInterval();

        if (m_bAllowDialogBoxMode)
        {
            // Make the back buffers lockable in fullscreen mode
            // so we can show dialog boxes via SetDialogBoxMode() 
            // but since lockable back buffers incur a performance cost on 
            // some graphics hardware configurations we'll only 
            // enable lockable backbuffers where SetDialogBoxMode() would work.
            if ( (  m_d3dpp.BackBufferFormat == D3DFMT_X1R5G5B5 || 
                    m_d3dpp.BackBufferFormat == D3DFMT_R5G6B5 || 
                    m_d3dpp.BackBufferFormat == D3DFMT_X8R8G8B8 ) &&
                    ( m_d3dpp.MultiSampleType == D3DMULTISAMPLE_NONE ) &&
                    ( m_d3dpp.SwapEffect == D3DSWAPEFFECT_DISCARD ) )
            {
                m_d3dpp.Flags |= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
            }
        }
    }
} // D3DApplication::BuildPresentParamsFromSettings

//-----------------------------------------------------------------------------
// Name: Reset3DEnvironment()
// Desc: Usually this function is not overridden.  Here's what this function does:
//       - Sets the windowed flag to be either windowed or fullscreen
//       - Sets parameters for z-buffer depth and back buffer
//       - Creates the D3D device
//       - Sets the window position (if windowed, that is)
//       - Makes some determinations as to the abilites of the driver (HAL, etc)
//       - Sets up some cursor stuff
//       - Calls InitDeviceObjects()
//       - Calls RestoreDeviceObjects()
//       - If all goes well, m_bActive is set to TRUE, and the function returns
//       - Otherwise, initialization is reattempted using the reference device
//-----------------------------------------------------------------------------
HRESULT D3DApplication::Reset3DEnvironment()
{
    HRESULT hr;

    // Release all vidmem objects
    if (m_bDeviceObjectsRestored)
    {
        m_bDeviceObjectsRestored = false;
        InvalidateDeviceObjects();
    }
    // Reset the device
    if (FAILED( hr = m_pDevice->Reset( &m_d3dpp ) )) return hr;

    // Store render target surface desc
    LPDIRECT3DSURFACE9 pBackBuffer;
    m_pDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
    pBackBuffer->GetDesc( &m_d3dsdBackBuffer );
    pBackBuffer->Release();

    // Set up the fullscreen cursor
    if (m_bShowCursorWhenFullscreen && !m_bWindowed)
    {
        HCURSOR hCursor;
#ifdef _WIN64
        hCursor = (HCURSOR)GetClassLongPtr( m_hWnd, GCLP_HCURSOR );
#else
        hCursor = (HCURSOR)ULongToHandle( GetClassLong( m_hWnd, GCL_HCURSOR ) );
#endif
        D3DUtil_SetDeviceCursor( m_pDevice, hCursor, true );
        m_pDevice->ShowCursor( true );
    }

    // Confine cursor to fullscreen window
    if (m_bClipCursorWhenFullscreen)
    {
        if (!m_bWindowed)
        {
            RECT rcWindow;
            GetWindowRect( m_hWnd, &rcWindow );
            ClipCursor( &rcWindow );
        }
        else
        {
            ClipCursor( NULL );
        }
    }

    // Initialize the app's device-dependent objects
    hr = RestoreDeviceObjects();
    if (FAILED(hr))
    {
        InvalidateDeviceObjects();
        return hr;
    }
    m_bDeviceObjectsRestored = true;

    // If the app is paused, trigger the rendering of the current frame
    if (false == m_bFrameMoving)
    {
        m_bSingleStep = true;
        DXUtil_Timer( TIMER_START );
        DXUtil_Timer( TIMER_STOP );
    }

    return S_OK;
} // D3DApplication::Reset3DEnvironment

//-----------------------------------------------------------------------------
// Name: ToggleFullScreen()
// Desc: Called when user toggles between fullscreen mode and windowed mode
//-----------------------------------------------------------------------------
HRESULT D3DApplication::ToggleFullscreen()
{
    HRESULT hr;
    int AdapterOrdinalOld = m_d3dSettings.Ordinal();
    D3DDEVTYPE DevTypeOld = m_d3dSettings.DevType();

    m_bActive = false;
    m_bIgnoreSizeChange = true;

    // Toggle the windowed state
    m_bWindowed = !m_bWindowed;
    m_d3dSettings.m_bIsWindowed = m_bWindowed;

    // Prepare window for windowed/fullscreen change
    AdjustWindowForChange();

    // If m_Ordinal and m_DevType are the same, we can just do a Reset().
    // If they've changed, we need to do a complete device teardown/rebuild.
    if (m_d3dSettings.Ordinal() == AdapterOrdinalOld &&
        m_d3dSettings.DevType() == DevTypeOld)
    {
        // Reset the 3D device
        BuildPresentParamsFromSettings();
        hr = Reset3DEnvironment();
    }
    else
    {
        Cleanup3DEnvironment();
        hr = Initialize3DEnvironment();
    }

    if (FAILED( hr ))
    {
        if (hr != D3DERR_OUTOFVIDEOMEMORY)
            hr = D3DAPPERR_RESETFAILED;
        m_bIgnoreSizeChange = false;
        if (!m_bWindowed)
        {
            // Restore window type to windowed mode
            m_bWindowed = !m_bWindowed;
            m_d3dSettings.m_bIsWindowed = m_bWindowed;
            AdjustWindowForChange();
            SetWindowPos( m_hWnd, HWND_NOTOPMOST,
                        m_rcWindowBounds.left, m_rcWindowBounds.top,
                        ( m_rcWindowBounds.right - m_rcWindowBounds.left ),
                        ( m_rcWindowBounds.bottom - m_rcWindowBounds.top ),
                        SWP_SHOWWINDOW );
        }
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
    }

    m_bIgnoreSizeChange = false;

    // When moving from fullscreen to windowed mode, it is important to
    // adjust the window size after resetting the device rather than
    // beforehand to ensure that you get the window size you want.  For
    // example, when switching from 640x480 fullscreen to windowed with
    // a 1000x600 window on a 1024x768 desktop, it is impossible to set
    // the window size to 1000x600 until after the display mode has
    // changed to 1024x768, because windows cannot be larger than the
    // desktop.
    if (m_bWindowed)
    {
        SetWindowPos( m_hWnd, HWND_NOTOPMOST,
                      m_rcWindowBounds.left, m_rcWindowBounds.top,
                      ( m_rcWindowBounds.right - m_rcWindowBounds.left ),
                      ( m_rcWindowBounds.bottom - m_rcWindowBounds.top ),
                      SWP_SHOWWINDOW );
    }

    GetClientRect( m_hWnd, &m_rcWindowClient );  // Update our copy

    m_bActive = true;
    return S_OK;
} // D3DApplication::ToggleFullscreen

//-----------------------------------------------------------------------------
// Name: ForceWindowed()
// Desc: Switch to a windowed mode, even if that means picking a new device
//       and/or adapter
//-----------------------------------------------------------------------------
HRESULT D3DApplication::ForceWindowed()
{
    HRESULT hr;

    if (m_bWindowed) return S_OK;
    if (!FindBestWindowedMode( false, false ))
    {
        return E_FAIL;
    }
    m_bWindowed = true;

    // Now destroy the current 3D device objects, then reinitialize
    m_bActive = false;

    // Release all scene objects that will be re-created for the new device
    Cleanup3DEnvironment();

    // Create the new device
    if (FAILED( hr = Initialize3DEnvironment() )) return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );

    m_bActive = true;
    return S_OK;
} // D3DApplication::ForceWindowed

//-----------------------------------------------------------------------------
// Name: AdjustWindowForChange()
// Desc: Prepare the window for a possible change between windowed mode and
//       fullscreen mode.  This function is virtual and thus can be overridden
//       to provide different behavior, such as switching to an entirely
//       different window for fullscreen mode (as in the MFC sample apps).
//-----------------------------------------------------------------------------
HRESULT D3DApplication::AdjustWindowForChange()
{
    if (m_bWindowed)
    {
        // Set windowed-mode style
        SetWindowLong( m_hWnd, GWL_STYLE, m_dwWindowStyle );
        if (m_hMenu != NULL)
        {
            SetMenu( m_hWnd, m_hMenu );
            m_hMenu = NULL;
        }
    }
    else
    {
        // Set fullscreen-mode style
        SetWindowLong( m_hWnd, GWL_STYLE, WS_POPUP|WS_SYSMENU|WS_VISIBLE );
        if (m_hMenu == NULL)
        {
            m_hMenu = GetMenu( m_hWnd );
            SetMenu( m_hWnd, NULL );
        }
    }
    return S_OK;
} // D3DApplication::AdjustWindowForChange

//-----------------------------------------------------------------------------
// Name: UserSelectNewDevice()
// Desc: Displays a dialog so the user can select a new adapter, device, or
//       display mode, and then recreates the 3D environment if needed
//-----------------------------------------------------------------------------
HRESULT D3DApplication::UserSelectNewDevice()
{
    HRESULT hr;
    bool bDialogBoxMode = false;
    bool bOldWindowed = m_bWindowed;  // Preserve original windowed flag

    if (m_bWindowed == false)
    {
        // See if the current settings comply with the rules
        // for allowing SetDialogBoxMode().  
        if( (m_d3dpp.BackBufferFormat == D3DFMT_X1R5G5B5 || m_d3dpp.BackBufferFormat == D3DFMT_R5G6B5 || m_d3dpp.BackBufferFormat == D3DFMT_X8R8G8B8 ) &&
            ( m_d3dpp.MultiSampleType == D3DMULTISAMPLE_NONE ) &&
            ( m_d3dpp.SwapEffect == D3DSWAPEFFECT_DISCARD ) &&
            ( (m_d3dpp.Flags & D3DPRESENTFLAG_LOCKABLE_BACKBUFFER) == D3DPRESENTFLAG_LOCKABLE_BACKBUFFER ) &&
            ( (m_dwCreateFlags & D3DCREATE_ADAPTERGROUP_DEVICE) != D3DCREATE_ADAPTERGROUP_DEVICE ) )
        {
            if (SUCCEEDED( m_pDevice->SetDialogBoxMode( true ) )) bDialogBoxMode = true;
        }

        // If SetDialogBoxMode(true) didn't work then we can't display dialogs  
        // in fullscreen mode so we'll go back to windowed mode
        if (FALSE == bDialogBoxMode)
        {
            if (FAILED( ToggleFullscreen() ))
            {
                DisplayErrorMsg( D3DAPPERR_RESETFAILED, MSGERR_APPMUSTEXIT );
                return E_FAIL;
            }
        }
    }

    // The dialog should use the mode the sample runs in, not
    // the mode that the dialog runs in.
    D3DSettings tempSettings = m_d3dSettings;
    tempSettings.m_bIsWindowed = bOldWindowed;
    CD3DSettingsDialog settingsDialog( &m_d3dEnum, &tempSettings);
    INT_PTR nResult = settingsDialog.ShowDialog( m_hWnd );

    // Before creating the device, switch back to SetDialogBoxMode(false) 
    // mode to allow the user to pick multisampling or backbuffer formats 
    // not supported by SetDialogBoxMode(true) but typical apps wouldn't 
    // need to switch back.
    if (bDialogBoxMode) m_pDevice->SetDialogBoxMode( false );

    if (nResult != IDOK)
    {
        // If we had to switch mode to display the dialog, we
        // need to go back to the original mode the sample
        // was running in.
        if( bOldWindowed != m_bWindowed && FAILED( ToggleFullscreen() ) )
        {
            DisplayErrorMsg( D3DAPPERR_RESETFAILED, MSGERR_APPMUSTEXIT );
            return E_FAIL;
        }

        return S_OK;
    }

    settingsDialog.GetFinalSettings( &m_d3dSettings );

    m_bWindowed = m_d3dSettings.m_bIsWindowed;

    // Release all scene objects that will be re-created for the new device
    Cleanup3DEnvironment();

    // Inform the display class of the change. It will internally
    // re-create valid surfaces, a d3ddevice, etc.
    if (FAILED( hr = Initialize3DEnvironment() ))
    {
        if (hr != D3DERR_OUTOFVIDEOMEMORY)
            hr = D3DAPPERR_RESETFAILED;
        if (!m_bWindowed)
        {
            // Restore window type to windowed mode
            m_bWindowed = !m_bWindowed;
            m_d3dSettings.m_bIsWindowed = m_bWindowed;
            AdjustWindowForChange();
            SetWindowPos( m_hWnd, HWND_NOTOPMOST,
                        m_rcWindowBounds.left, m_rcWindowBounds.top,
                        ( m_rcWindowBounds.right - m_rcWindowBounds.left ),
                        ( m_rcWindowBounds.bottom - m_rcWindowBounds.top ),
                        SWP_SHOWWINDOW );
        }
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
    }

    // If the app is paused, trigger the rendering of the current frame
    if (false == m_bFrameMoving)
    {
        m_bSingleStep = true;
        DXUtil_Timer( TIMER_START );
        DXUtil_Timer( TIMER_STOP );
    }
    return S_OK;
} // D3DApplication::UserSelectNewDevice

//-----------------------------------------------------------------------------
// Name: UpdateStats()
// Desc: 
//-----------------------------------------------------------------------------
void D3DApplication::UpdateStats()
{
    // Keep track of the frame count
    static FLOAT fLastTime = 0.0f;
    static DWORD dwFrames  = 0;
    FLOAT fTime = DXUtil_Timer( TIMER_GETABSOLUTETIME );
    ++dwFrames;

    // Update the scene stats once per second
    if( fTime - fLastTime > 1.0f )
    {
        m_fFPS    = dwFrames / (fTime - fLastTime);
        fLastTime = fTime;
        dwFrames  = 0;

        TCHAR strFmt[100];
        D3DFORMAT fmtAdapter = m_d3dSettings.DisplayMode().Format;
        if( fmtAdapter == m_d3dsdBackBuffer.Format )
        {
            lstrcpyn( strFmt, D3DUtil_D3DFormatToString( fmtAdapter, false ), 100 );
        }
        else
        {
            _sntprintf( strFmt, 100, TEXT("backbuf %s, adapter %s"), 
                D3DUtil_D3DFormatToString( m_d3dsdBackBuffer.Format, false ), 
                D3DUtil_D3DFormatToString( fmtAdapter, false ) );
        }
        strFmt[99] = TEXT('\0');

        TCHAR strDepthFmt[100];
        if( m_d3dEnum.m_bUseZBuffer )
        {
            _sntprintf( strDepthFmt, 100, TEXT(" (%s)"), 
                D3DUtil_D3DFormatToString( m_d3dSettings.DepthStencilBufferFormat(), false ) );
            strDepthFmt[99] = TEXT('\0');
        }
        else
        {
            // No depth buffer
            strDepthFmt[0] = TEXT('\0');
        }

        TCHAR* pstrMultiSample;
        switch( m_d3dSettings.MultisampleType() )
        {
        case D3DMULTISAMPLE_NONMASKABLE:  pstrMultiSample = TEXT(" (Nonmaskable Multisample)"); break;
        case D3DMULTISAMPLE_2_SAMPLES:  pstrMultiSample = TEXT(" (2x Multisample)"); break;
        case D3DMULTISAMPLE_3_SAMPLES:  pstrMultiSample = TEXT(" (3x Multisample)"); break;
        case D3DMULTISAMPLE_4_SAMPLES:  pstrMultiSample = TEXT(" (4x Multisample)"); break;
        case D3DMULTISAMPLE_5_SAMPLES:  pstrMultiSample = TEXT(" (5x Multisample)"); break;
        case D3DMULTISAMPLE_6_SAMPLES:  pstrMultiSample = TEXT(" (6x Multisample)"); break;
        case D3DMULTISAMPLE_7_SAMPLES:  pstrMultiSample = TEXT(" (7x Multisample)"); break;
        case D3DMULTISAMPLE_8_SAMPLES:  pstrMultiSample = TEXT(" (8x Multisample)"); break;
        case D3DMULTISAMPLE_9_SAMPLES:  pstrMultiSample = TEXT(" (9x Multisample)"); break;
        case D3DMULTISAMPLE_10_SAMPLES: pstrMultiSample = TEXT(" (10x Multisample)"); break;
        case D3DMULTISAMPLE_11_SAMPLES: pstrMultiSample = TEXT(" (11x Multisample)"); break;
        case D3DMULTISAMPLE_12_SAMPLES: pstrMultiSample = TEXT(" (12x Multisample)"); break;
        case D3DMULTISAMPLE_13_SAMPLES: pstrMultiSample = TEXT(" (13x Multisample)"); break;
        case D3DMULTISAMPLE_14_SAMPLES: pstrMultiSample = TEXT(" (14x Multisample)"); break;
        case D3DMULTISAMPLE_15_SAMPLES: pstrMultiSample = TEXT(" (15x Multisample)"); break;
        case D3DMULTISAMPLE_16_SAMPLES: pstrMultiSample = TEXT(" (16x Multisample)"); break;
        default:                        pstrMultiSample = TEXT(""); break;
        }

        const int cchMaxFrameStats = sizeof(m_strFrameStats) / sizeof(TCHAR);
        _sntprintf( m_strFrameStats, cchMaxFrameStats, _T("%.02f fps (%dx%d), %s%s%s"), m_fFPS,
                    m_d3dsdBackBuffer.Width, m_d3dsdBackBuffer.Height,
                    strFmt, strDepthFmt, pstrMultiSample );
        m_strFrameStats[cchMaxFrameStats - 1] = TEXT('\0');
    }
} // D3DApplication::UpdateStats

//-----------------------------------------------------------------------------
// Name: Cleanup3DEnvironment()
// Desc: Cleanup scene objects
//-----------------------------------------------------------------------------
void D3DApplication::Cleanup3DEnvironment()
{
    if( m_pDevice != NULL )
    {
        if( m_bDeviceObjectsRestored )
        {
            m_bDeviceObjectsRestored = false;
            InvalidateDeviceObjects();
        }
        if( m_bDeviceObjectsInited )
        {
            m_bDeviceObjectsInited = false;
            DeleteDeviceObjects();
        }

        if (m_pDevice->Release() > 0)
        {
            DisplayErrorMsg( D3DAPPERR_NONZEROREFCOUNT, MSGERR_APPMUSTEXIT );
        }
        m_pDevice = NULL;
    }

    //  restore desktop screen mode
    int devModeRes = ChangeDisplaySettings( &m_InitialDesktopMode, 0 );
    if (devModeRes != DISP_CHANGE_SUCCESSFUL)
    {
        Log.Error( "Could not restore desktop display mode" );
    }

} // D3DApplication::Cleanup3DEnvironment

bool D3DApplication::SupportsInstancing()
{
    const D3DCAPS9& caps = GetCaps();
    if (caps.VertexShaderVersion >= D3DVS_VERSION( 3,0 )) return true;
    if (caps.VertexShaderVersion >= D3DVS_VERSION( 2,0 ))
    {
        // Because ATI supports instancing (on some drivers) without vertex shader 3.0,
        // but its also not enabled by default, so we need to not only check for support
        // but enabled it if it is supported.
        DWORD instanceSupport = MAKEFOURCC( 'I', 'N', 'S', 'T' );
        if(m_pD3D && SUCCEEDED( m_pD3D->CheckDeviceFormat(  D3DADAPTER_DEFAULT, 
                                                            D3DDEVTYPE_HAL, 
                                                            D3DFMT_X8R8G8B8,
                                                            0, D3DRTYPE_SURFACE, 
                                                            (D3DFORMAT)instanceSupport ) ))
        {
            m_pDevice->SetRenderState(D3DRS_POINTSIZE, instanceSupport);
            return true;
        }
    }
    return false;
} // D3DApplication::SupportsInstancing

//-----------------------------------------------------------------------------
// Name: DisplayErrorMsg()
// Desc: Displays error messages in a message box
//-----------------------------------------------------------------------------
HRESULT D3DApplication::DisplayErrorMsg( HRESULT hr, DWORD dwType )
{
    static bool s_bFatalErrorReported = false;
    // If a fatal error message has already been reported, the app
    // is already shutting down, so don't show more error messages.
    if( s_bFatalErrorReported ) return hr;

    switch( hr )
    {
        case D3DAPPERR_NODIRECT3D:
            Log.Error( "Could not initialize Direct3D." );
            break;

        case D3DAPPERR_NOCOMPATIBLEDEVICES:
            Log.Error( "Could not find any compatible Direct3D devices." );
            break;

        case D3DAPPERR_NOWINDOWABLEDEVICES:
            Log.Error( "Application cannot run in a desktop window with the current display settings. "
                        "Please change your desktop settings to a "
                        "16- or 32-bit display mode and re-run application." );
            break;

        case D3DAPPERR_NOHARDWAREDEVICE:
            Log.Error( "No hardware-accelerated Direct3D devices were found." );
            break;

        case D3DAPPERR_HALNOTCOMPATIBLE:
            Log.Error( "Application requires functionality that is not available on your hardware accelerator." );
            break;

        case D3DAPPERR_NOWINDOWEDHAL:
            Log.Error( "Your Direct3D hardware accelerator cannot render into a window." );
            break;

        case D3DAPPERR_NODESKTOPHAL:
            Log.Error( "Your Direct3D hardware accelerator cannot"
                       "render into a window with the current"
                       "desktop display settings." );
            break;

        case D3DAPPERR_NOHALTHISMODE:
            Log.Error( "Application requires functionality that is "
                       "not available on your Direct3D hardware "
                       "accelerator with the current desktop display "
                       "settings." );
            break;

        case D3DAPPERR_MEDIANOTFOUND:
        case 0x80070002: // HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND ):
            Log.Error( "Could not load required media." );
            break;

        case D3DAPPERR_RESETFAILED:
            Log.Error( "Could not reset the Direct3D device." );
            break;

        case D3DAPPERR_NONZEROREFCOUNT:
            Log.Error( "A D3D object has a non-zero reference count (meaning things were not properly cleaned up." );
            break;

        case D3DAPPERR_NULLREFDEVICE:
            Log.Error( "Nothing will be rendered. "
                       "The reference rendering device was selected, but your "
                       "computer only has a reduced-functionality reference device "
                       "installed.  Install the DirectX SDK to get the full "
                       "reference device." );
            break;

        case E_OUTOFMEMORY:
            Log.Error( "Not enough memory." );
            break;

        case D3DERR_OUTOFVIDEOMEMORY:
           Log.Error( "Not enough video memory." );
            break;

        case D3DERR_DRIVERINTERNALERROR:
            Log.Error( "A serious problem occured inside the display driver." );
            dwType = MSGERR_APPMUSTEXIT;
            break;

        default:
            Log.Error( "Generic application error. Enable debug output for detailed information." );
    }

    if( MSGERR_APPMUSTEXIT == dwType )
    {
        s_bFatalErrorReported = true;
        Log.Error( "Application will now exit." );
        // Close the window, which shuts down the app
        if( m_hWnd ) SendMessage( m_hWnd, WM_CLOSE, 0, 0 );
    }
    else
    {
        if( MSGWARN_SWITCHEDTOREF == dwType )
        {
            Log.Warning( "Switching to the reference rasterizer, a software device that implements"
                            "the entire Direct3D feature set, but runs very slowly." );
        }
    }

    return hr;
} // D3DApplication::DisplayErrorMsg


