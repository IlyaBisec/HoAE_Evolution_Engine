#include "CMainWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CMainWnd, CWnd)
BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

HTMLCMDMAP g_CmdsMap[] = {
	{ _T("close"),  ID_APP_EXIT  },
	{ NULL, 0  },
};

void CMainWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if(VK_ESCAPE == nChar) {
		m_HtmlCtrl.Navigate("cmd:close");
	}
}

BOOL CMainWnd::OnCommand(WPARAM wParam, LPARAM lParam) {
	if(ID_APP_ABOUT == wParam) {
		switch(LOWORD(lParam)) {
			case idSetX: X = HIWORD(lParam); break;
			case idSetY: Y = HIWORD(lParam); break;
			case idSetW: W = HIWORD(lParam); break;
			case idSetH: H = HIWORD(lParam); break;
			case idSetWindowPos: SetWindowPos(NULL, X, Y, W, H, 0);
				break;
			case idQuit: OnCommand(ID_APP_EXIT, 0);
				break;
			case idSetURL:
				char URL[256];
				if(GlobalGetAtomName(HIWORD(lParam), URL, 256) > 0) {
					m_HtmlCtrl.Navigate(URL);
					GlobalDeleteAtom(HIWORD(lParam));
				}
				break;
			default:
				break;
		}
		return TRUE;
	}
	if(ID_APP_EXIT == wParam) {
//		m_HtmlCtrl.Navigate("about:blank");
//		m_HtmlCtrl.Stop();
		PostQuitMessage(0);
		return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------------------------
// RegWndClass
//-----------------------------------------------------------------------------
LPCTSTR AFXAPI RegWndClass() {
//	LPTSTR lpszName = "GSCHtmlBrowser";
	HINSTANCE hInst = AfxGetInstanceHandle();

	WNDCLASS wc;
	if(::GetClassInfo(hInst, MainWndClassName, &wc)) {
		return MainWndClassName;
	}

	wc.style = 0;
	wc.lpfnWndProc = DefWindowProc;
	wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = MainWndClassName;//lpszName;
	if(!AfxRegisterClass(&wc))
		AfxThrowResourceException();
	
	return MainWndClassName;
}

//-----------------------------------------------------------------------------
// CMainWnd.ctor
//-----------------------------------------------------------------------------
CMainWnd::CMainWnd(int x0, int y0, int nWidth, int nHeight, const char *URL) {
	X = x0, Y = y0, W = nWidth, H = nHeight;
	if(X < 0) X = 0;
	if(Y < 0) Y = 0;
	if(W < 0) W = GetSystemMetrics(SM_CXSCREEN);
	if(H < 0) H = GetSystemMetrics(SM_CYSCREEN);

	if(!strlen(URL)) strcpy(m_DefURL, "about:blank");
	else strcpy(m_DefURL, URL);
	DWORD dwExStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST, dwStyle = WS_POPUP | WS_VISIBLE;
	CreateEx(dwExStyle, RegWndClass(), _T(""), dwStyle, X, Y, W, H, NULL, NULL, 0);
}

//-----------------------------------------------------------------------------
// CMainWnd::OnCreate
//-----------------------------------------------------------------------------
int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	VERIFY(CWnd::OnCreate(lpCreateStruct) == 0);
	
	VERIFY(m_HtmlCtrl.Create(CRect(), this, AFX_IDW_PANE_FIRST));
	m_HtmlCtrl.HideContextMenu(TRUE);
	m_HtmlCtrl.SetCmdMap(g_CmdsMap);
	m_HtmlCtrl.Navigate(_T(m_DefURL));
//	m_HtmlCtrl.Navigate(_T("about:blank"));
//	m_HtmlCtrl.Navigate(_T("www.dtf.ru"));
//	m_HtmlCtrl.Navigate(_T("www.gsc-game.com"));
//	m_HtmlCtrl.Navigate(_T("D:\\Work\\HTML\\1.html"));
//	m_HtmlCtrl.SetHTML(GenHTML());//FormatWindowListHTML());
	return 0;
}

//-----------------------------------------------------------------------------
// CMainWnd::OnSize
//-----------------------------------------------------------------------------
void CMainWnd::OnSize(UINT nType, int cx, int cy) {
	OnSizing(0, NULL);
}

//-----------------------------------------------------------------------------
// CMainWnd::OnSizing
//-----------------------------------------------------------------------------
void CMainWnd::OnSizing(UINT nSide, LPRECT lpRect) {
	RECT rc;
	GetClientRect(&rc);
	m_HtmlCtrl.SetWindowPos(NULL, 0, 0, rc.right, rc.bottom, SWP_NOREDRAW);
}