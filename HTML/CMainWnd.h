#include "CHtmlCtrl.h"

static const char MainWndClassName[] = "GSCHtmlFullScreenBrowser";

class CMainWnd : public CWnd {
public:
	CMainWnd() {}
	CMainWnd(int x0, int y0, int nWidth, int nHeight, const char *URL);

	static const int idSetX = 0;
	static const int idSetY = 1;
	static const int idSetW = 2;
	static const int idSetH = 3;
	static const int idSetWindowPos = 4;
	static const int idQuit = 5;
	static const int idSetURL = 6;
protected:
	CHtmlCtrl m_HtmlCtrl;
	char m_DefURL[4096];

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT nSide, LPRECT lpRect);
	afx_msg BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	DECLARE_DYNCREATE(CMainWnd)
	DECLARE_MESSAGE_MAP()

	int X, Y, W, H;
};