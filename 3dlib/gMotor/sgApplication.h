/*****************************************************************************/
/*	File:	sgApplication.h
/*	Author:	Ruslan Shestopalyuk
/*	Date:	10-29-2003
/*****************************************************************************/
#ifndef __SGAPPWINDOW_H__
#define __SGAPPWINDOW_H__

namespace sg{

typedef void (*RenderCallback)();
const int c_MaxVersionString = 256;
/*****************************************************************************/
/*	Class:	Application, singleton 
/*	Desc:	Application settings node
*****************************************************************************/
class Application : public Node, public PSingleton<Application>
{
public:
							Application			();
	virtual					~Application		();

	virtual void			Expose				( PropertyMap& pm );

	_inl const char*		GetRootDir			() const { return m_RootDirectory; }
	_inl const char*		GetCmdLine			() const { return m_CommandLine.c_str(); }
	_inl void				SetRootDir			( const char* str ) { strcpy( m_RootDirectory, str ); }
	_inl void				SetCmdLine			( const char* str ) { m_CommandLine = str; }

	_inl HINSTANCE			GetHInstance		() const { return m_hInstance; }
	_inl void				SetHInstance		( HINSTANCE val ) { m_hInstance = val; }

	void					EnterMessageLoop	();
	void					ProcessSystemMessages();

	int						GetReturnCode		() const { return m_ReturnCode; }
	void					ToRelativePath		( char* path, int bufSize );
	void					SetRenderCallback	( RenderCallback callb ) { Render = callb; }

	void					SetActive			( bool bActive = true ) { m_bActive = bActive; }
	bool					IsActive			() const { return m_bActive; }

	const char*				GetVersionString	() const { return m_VersionString; }
	DWORD					GetVersion			() const { return m_Version; }
	DWORD					GetSubVersion		() const { return m_SubVersion; }

	NODE(Application, Node, APPL);

protected:
	RenderCallback			Render;

	HINSTANCE				m_hInstance;
	char					m_RootDirectory[_MAX_PATH];
	std::string				m_CommandLine;
	int						m_ReturnCode;
	bool					m_bActive;

	DWORD					m_Version;
	DWORD					m_SubVersion;
	char					m_VersionString[c_MaxVersionString];
}; // class Application

/*****************************************************************************/
/*	Class:	AppWindow, singleton 
/*	Desc:	Main application system window
*****************************************************************************/
class AppWindow : public Node, public PSingleton<AppWindow>
{
public:
							AppWindow			();
	virtual					~AppWindow			();

	_inl Rct				GetExtents			() const;
	
	_inl const char*		GetWindowTitle		() const { return m_WindowTitle.c_str(); }
	_inl const char*		GetWindowClassName	() const { return m_WindowClassName.c_str(); }
	_inl HWND				GetWndHandle		() const { return m_hWnd; }
	
	_inl void				SetWindowTitle		( const char* val ){ m_WindowTitle = val; }
	_inl void				SetWindowClassName	( const char* val ) { m_WindowClassName = val; }

	_inl int				GetLeft				() const { return GetExtents().x; }
	_inl int				GetTop				() const { return GetExtents().y; }
	_inl int				GetWidth			() const { return GetExtents().w; }
	_inl int				GetHeight			() const { return GetExtents().h; }

	void					SetLeft				( int val );
	void					SetTop				( int val );
	void					SetWidth			( int val );
	void					SetHeight			( int val );

	bool					UseDesktopArea		() const { return m_bUseDesktopArea; }
	bool					IsFullScren			() const { return m_bFullScreen; }
	ScreenResolution		GetResolution		() const { return m_Resolution; }
	ScreenBitDepth			GetBitDepth			() const { return m_BitDepth; }

	void					SetUseDesktopArea	( bool val ) { m_bUseDesktopArea = val; }
	void					SetFullScreen		( bool val ) { m_bFullScreen = val; }
	void					SetResolution		( ScreenResolution val ) { m_Resolution = val; }
	void					SetBitDepth			( ScreenBitDepth val ) { m_BitDepth = val; }
	virtual void			Expose				( PropertyMap& pm );

	bool					CreateAppWindow		();
	void					SetIconResourceID	( DWORD id ) { m_IconResourceID = id; }
	
	void					SetWindowHandle		( HWND hWnd );
	HWND					GetWindowHandle		() const { return m_hWnd; }

	typedef UINT_PTR 		(*PWndProc)			( HWND, UINT, WPARAM, LPARAM );
	PWndProc				GetWndProc			() { return (PWndProc)WndProc; }

	void					Update				();
	

	NODE(AppWindow, Node, APPW);

protected:
	HWND					m_hWnd;
	Rct						m_Extents;
	DWORD					m_IconResourceID;

	std::string				m_WindowClassName;
	std::string				m_WindowTitle;
	ScreenResolution		m_Resolution;
	ScreenBitDepth			m_BitDepth;
	bool					m_bFullScreen;

	bool					m_bUseDesktopArea;

	ATOM					RegisterWindowClass	( HINSTANCE hInstance );
	bool					InitInstance		( HINSTANCE hInstance, int nCmdShow );
	static DWORD __stdcall	WndProc				( HWND hWnd, UINT message, 
													WPARAM wParam, LPARAM lParam );

	bool					InitRenderSystem();	
}; // class AppWindow

/*****************************************************************************/
/*	Class:	CPU, singleton 
/*	Desc:	CPU properties node
*****************************************************************************/
class CPU : public Node, public PSingleton<CPU>
{

public:
						CPU		();
	bool				HasMMX	() const;
	bool				Has3DNow() const;
	bool				HasSSE	() const;
	bool				HasSSE2	() const;
	bool				NoCPUID () const;


	virtual void		Expose( PropertyMap& pm );

	NODE(CPU,Node,CPUN);
}; // class CPU

/*****************************************************************************/
/*	Class:	DiskFolder
/*	Desc:	Folder on the disk
*****************************************************************************/
class DiskFolder : public Node
{
public:
	
	NODE(DiskFolder,Node,FOLD);
}; // class DiskFolder


}; // namespace sg

#endif // __SGAPPWINDOW_H__