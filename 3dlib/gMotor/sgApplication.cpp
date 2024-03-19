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
#include "sgApplication.h"
#include "IWidgetManager.h"
#include "IPictureManager.h"
#include "rsRenderSystem.h"
#include <direct.h>

const DWORD		c_AppVersion	= 0;
const DWORD		c_AppSubVersion = 1;

const char* GetRootDirectory()
{
	return sg::Application::instance()->GetRootDir();
}

void ToRelativePath( char* path, int bufSize )
{
    sg::Application::instance()->ToRelativePath( path, bufSize );
}

char g_PathBuf[_MAX_PATH];
const char* ToRelativePath( const char* path )
{
    strcpy( g_PathBuf, path );
    sg::Application::instance()->ToRelativePath( g_PathBuf, _MAX_PATH );
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

BEGIN_NAMESPACE(sg)

/*****************************************************************************/
/*	Application implementation
/*****************************************************************************/
Application::Application()
{
	SetImmortal();

	getcwd( m_RootDirectory, _MAX_PATH );	
	EnsureLastSlash( m_RootDirectory );

	m_ReturnCode = 0;

	SetName( "Application" );

	Render = NULL;
	m_bActive = true;

	m_Version = c_AppVersion;
	m_SubVersion = c_AppSubVersion;
	sprintf( m_VersionString, "v%x.%x", m_Version, m_SubVersion );
    
    //  HACK
    InitMath();
}

Application::~Application()
{
}

void Application::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "Application", this );
	pm.p( "RootDirectory",	GetRootDir, SetRootDir );
	pm.p( "CommandLine",	GetCmdLine );
	pm.f( "Version",		m_Version, NULL, true );
	pm.f( "SubVersion",		m_SubVersion, NULL, true );
	pm.p( "VersionString",	GetVersionString );
} // Application::Expose

void Application::EnterMessageLoop()
{
	MSG msg;
	ZeroMemory( &msg, sizeof(msg) );
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ))
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			if (!IsActive()) continue;
			InputDispatcher::Update();
			if (Render) Render();
			if (IRS) IRS->OnFrame();
		}
	}
} // Application::StartMessageLoop

void Application::ProcessSystemMessages()
{
	MSG msg;
	ZeroMemory( &msg, sizeof(msg) );
	while (PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ))
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
} // Application::ProcessSystemMessages

void Application::ToRelativePath( char* path, int bufSize )
{
	char npath[_MAX_PATH];
	const char* pRoot = GetRootDir();
	
	int cPos = 0;
	while (tolower( pRoot[cPos] ) == tolower( path[cPos] )) cPos++;
	while (	path[cPos] == '\\' || 
			path[cPos] == '/') cPos++;
	strcpy( npath, path + cPos );
	strcpy( path, npath );
} // Application::ToRelativePath

/*****************************************************************************/
/*	AppWindow implementation
/*****************************************************************************/
AppWindow::AppWindow()
{
	SetImmortal();

	m_hWnd				= NULL;
	m_WindowClassName	= "SGAPPWINDOW_CLASS";
	m_WindowTitle		= "sgAppWindow";
	m_IconResourceID	= 0;
	m_bUseDesktopArea	= true;
	m_bFullScreen		= false;
	m_Resolution		= sr1024x768;
	m_BitDepth			= bd16;


	SetName( "AppWindow" );
	SetWindowHandle( GetActiveWindow() );
} // AppWindow::AppWindow

AppWindow::~AppWindow()
{
}

void AppWindow::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "AppWindow", this );
	pm.p( "WndClassName",	GetWindowClassName	);
	pm.p( "WindowTitle",	GetWindowTitle		);

	pm.p( "Left",			GetLeft		);
	pm.p( "Top",			GetTop		);
	pm.p( "Width",			GetWidth	);
	pm.p( "Height",			GetHeight	);
	pm.p( "UseDesktopArea", UseDesktopArea, SetUseDesktopArea	);
	pm.p( "FullScreen",		IsFullScren,	SetFullScreen		);
	pm.p( "Resolution",		GetResolution,	SetResolution		);
	pm.m( "Update",			Update );
} // AppWindow::Expose

const int c_ScreenWidth[]  = { 0, 640, 800, 1024, 1280, 1600 };
const int c_ScreenHeight[] = { 0, 480, 600,  768, 1024, 1200 };

void AppWindow::Update()
{
	ScreenProp prop;
	prop.m_Width		    = c_ScreenWidth[m_Resolution];
	prop.m_Height		    = c_ScreenHeight[m_Resolution];
	prop.m_bFullScreen		= m_bFullScreen;
	prop.m_bCoverDesktop    = m_bUseDesktopArea;
	prop.m_ColorFormat	    = (m_BitDepth == bd32) ? cfRGB888 : cfRGB565;
	IRS->SetScreenProperties( prop );
} // AppWindow::Update

bool AppWindow::CreateAppWindow()
{
	Application* app = Application::instance();

	RegisterWindowClass( app->GetHInstance() );
	InitInstance( app->GetHInstance(), SW_SHOW );
	InitRenderSystem();

    ScreenProp prop = IRS->GetScreenProperties();
    switch (prop.m_Width)
    {
        case 640:  m_Resolution = sr640x480; break;	
        case 800:  m_Resolution = sr800x600; break;		
        case 1024: m_Resolution = sr1024x768; break;		
        case 1280: m_Resolution = sr1280x1024; break;		
        case 1600: m_Resolution = sr1600x1200; break;
    }
    Update();
	return true;
} // AppWindow::CreateWindow

void AppWindow::SetWindowHandle( HWND hWnd )
{
	m_hWnd = hWnd;
}

_inl Rct AppWindow::GetExtents() const
{
	Rct		rct( m_Extents );
	RECT	wrct;

	if (!m_hWnd) 
	{
		HWND hDesk = GetDesktopWindow();
		::GetWindowRect( hDesk, &wrct );

		if (m_bUseDesktopArea)
		{
			SystemParametersInfo( SPI_GETWORKAREA, 0, &wrct, 0 );
		}
	}
	else
	{
		::GetWindowRect( m_hWnd, &wrct );
	}
	
	rct.x = wrct.left;
	rct.y = wrct.top;
	rct.w = wrct.right - wrct.left;
	rct.h = wrct.bottom - wrct.top;
	
	return rct;
} // AppWindow::GetExtents

ATOM AppWindow::RegisterWindowClass( HINSTANCE hInstance )
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= (WNDPROC) WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon( hInstance, (LPCTSTR)m_IconResourceID );
	wcex.hCursor		= NULL;
	wcex.hbrBackground	= (HBRUSH) (COLOR_WINDOW + 1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= GetWindowClassName();
	wcex.hIconSm		= LoadIcon( hInstance, (LPCTSTR)m_IconResourceID );

	return RegisterClassEx( &wcex );
} // AppWindow::RegisterWindowClass

bool AppWindow::InitInstance( HINSTANCE hInstance, int nCmdShow )
{
	Rct rct = GetExtents();
	m_hWnd = CreateWindowEx(	WS_EX_APPWINDOW,
								GetWindowClassName(), 
								GetWindowTitle(),
								WS_POPUP, 
								rct.x, 
								rct.y, 
								rct.w, 
								rct.h,
								NULL,
								NULL, 
								hInstance, 
								NULL );
	if (!m_hWnd)
	{
		return false;
	}

	ShowWindow( m_hWnd, nCmdShow );
	UpdateWindow( m_hWnd );
	SetCursor( LoadCursor( NULL, IDC_ARROW ) );
	return true;
} // AppWindow::InitInstance

DWORD __stdcall AppWindow::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{	
	switch (message) 
	{
	case WM_ACTIVATE:
		Application::instance()->SetActive( (wParam != WA_INACTIVE) );
		break;
	case WM_LBUTTONDBLCLK:
		InputDispatcher::MouseLButtonDblclk( LOWORD( lParam ), HIWORD( lParam ) );
		IWM->OnMouseLButtonDblclk( LOWORD( lParam ), HIWORD( lParam ) );
		break;

	case WM_MBUTTONDBLCLK:
		InputDispatcher::MouseMButtonDblclk( LOWORD( lParam ), HIWORD( lParam ) );
		IWM->OnMouseMButtonDblclk( LOWORD( lParam ), HIWORD( lParam ) );
		break;
	
	case WM_RBUTTONDBLCLK:
		InputDispatcher::MouseRButtonDblclk( LOWORD( lParam ), HIWORD( lParam ) );
		IWM->OnMouseRButtonDblclk( LOWORD( lParam ), HIWORD( lParam ) );
		break;

	case WM_RBUTTONDOWN:
		InputDispatcher::MouseRButtonDown( LOWORD( lParam ), HIWORD( lParam ) );
		IWM->OnMouseRButtonDown( LOWORD( lParam ), HIWORD( lParam ) );
		break;
	
	case WM_RBUTTONUP:
		InputDispatcher::MouseRButtonUp( LOWORD( lParam ), HIWORD( lParam ) );
		IWM->OnMouseRButtonUp( LOWORD( lParam ), HIWORD( lParam ) );
		break;

	case WM_LBUTTONDOWN:
		InputDispatcher::MouseLButtonDown( LOWORD( lParam ), HIWORD( lParam ) );
		IWM->OnMouseLButtonDown( LOWORD( lParam ), HIWORD( lParam ) );
		break;

	case WM_LBUTTONUP:
		InputDispatcher::MouseLButtonUp( LOWORD( lParam ), HIWORD( lParam ) );
		IWM->OnMouseLButtonUp( LOWORD( lParam ), HIWORD( lParam ) );
		break;

	case WM_MBUTTONDOWN:
		InputDispatcher::MouseMButtonDown( LOWORD( lParam ), HIWORD( lParam ) );
		IWM->OnMouseMButtonDown( LOWORD( lParam ), HIWORD( lParam ) );
		break;
	
	case WM_MBUTTONUP:
		InputDispatcher::MouseMButtonUp( LOWORD( lParam ), HIWORD( lParam ) );
		IWM->OnMouseMButtonUp( LOWORD( lParam ), HIWORD( lParam ) );
		break;

	case WM_MOUSEMOVE:
		InputDispatcher::MouseMove( LOWORD( lParam ), HIWORD( lParam ), wParam );
		IWM->OnMouseMove( LOWORD( lParam ), HIWORD( lParam ), wParam );
		break;
	
	case WM_MOUSEWHEEL:
		{
			short WheelDelta = (short) HIWORD( wParam );
			InputDispatcher::MouseWheel( WheelDelta );
			IWM->OnMouseWheel( WheelDelta );
		}break;

	case WM_KEYDOWN:
		if (Application::instance()->IsActive()) 
		{
			InputDispatcher::KeyDown( wParam, lParam );
			IWM->OnKeyDown( wParam, lParam );
		}
		break;

	case WM_CHAR:
		if (Application::instance()->IsActive()) 
		{
			InputDispatcher::Char( wParam, lParam );
			IWM->OnChar( wParam, lParam );
		}
		break;

	case WM_KEYUP:
		if (Application::instance()->IsActive()) 
		{ 
			InputDispatcher::KeyUp( wParam, lParam );
			IWM->OnKeyUp( wParam, lParam );
		}
		break;

	case WM_ERASEBKGND: return TRUE;
	
	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;

    case WM_SYSCOMMAND:
        if (wParam == SC_MONITORPOWER || wParam == SC_SCREENSAVE)
        {
            return TRUE;
        }

	default:
		return DefWindowProc( hWnd, message, wParam, lParam );
	}
	
	return FALSE;
} // AppWindow::WndProc

bool AppWindow::InitRenderSystem()
{
	if (!m_hWnd) return false;
	IRenderSystem* GetRenderSystem();
	IRS = GetRenderSystem(); 
	if (!IRS) return false;
	IRS->Init( m_hWnd );
	InitPictureManager();

	InputDispatcher::SetCoreDispatcher( &InputManager::instance() );
	InputDispatcher::Init();

	return true;
} // AppWindow::InitRenderSystem

/*****************************************************************************/
/*	CPU implementation
/*****************************************************************************/
CPU::CPU()
{
	SetImmortal();
	SetName( "CPU" );
}

bool CPU::HasMMX() const
{
	return HaveMMX();
} // CPU::HasMMX

bool CPU::Has3DNow() const
{
	return Have3DNow();
} // CPU::Has3DNow

bool CPU::HasSSE() const
{
	return HaveSSE();
} // CPU::HasSSE

bool CPU::HasSSE2() const
{
	return HaveSSE2();
} // CPU::HasSSE2

bool CPU::NoCPUID() const
{
	return !HaveCPUID();
} // CPU::NoCPUID

void CPU::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "CPU", this );
	pm.p( "MMX",		HasMMX	 );
	pm.p( "3DNow!",	    Has3DNow );
	pm.p( "SSE",		HasSSE	 );
	pm.p( "SSE2",		HasSSE2  );
}

END_NAMESPACE(sg)
