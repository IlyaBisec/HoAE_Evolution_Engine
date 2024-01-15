#include "CMainWnd.h"
#include "assert.h"

inline bool IsToken(const char c) { return c == 'x' || c == 'y' || c == 'w' || c == 'h' || c == '\"'; }
bool ArgsParser(char *Buffer, int &x, int &y, int &w, int &h, char *URL) {
	assert(Buffer);

	strlwr(Buffer);
	int nCur = 0, k;
	bool f = false;
	while(Buffer[nCur]) {
		while(!IsToken(Buffer[nCur])) {
			nCur++;
			if(Buffer[nCur] == '\0') return f;
		}
		// Buffer[nCur] is token:
		f = true;
		int c = Buffer[nCur];
		nCur++;
		if(Buffer[nCur] == '\0') return f;
		switch(c) {
			case 'x': x = atoi(&Buffer[nCur]); break;
			case 'y': y = atoi(&Buffer[nCur]); break;
			case 'w': w = atoi(&Buffer[nCur]); break;
			case 'h': h = atoi(&Buffer[nCur]); break;
			case '\"':
				k = 0;
				while(Buffer[nCur + k] != '\"') {
					URL[k] = Buffer[nCur + k];
					if(Buffer[nCur + k] == '\0') return f;
					k++;
				}
				URL[k] = '\0';
				nCur += (k + 1);
				break;
			default: // unknown token
				assert(0);
				break;
		}
	}
	return f;
}

class CBrowserApp : public CWinApp {
public:
	virtual BOOL InitInstance() {
		int x = -1, y = -1, w = -1, h = -1;
		char URL[512];
		ZeroMemory(URL, sizeof(URL));
		if(!ArgsParser(m_lpCmdLine, x, y, w, h, URL)) {
			while(HWND hWnd = FindWindow(MainWndClassName, "")) {
				PostMessage(hWnd, WM_COMMAND, ID_APP_ABOUT, MAKELPARAM(CMainWnd::idQuit, 0));
			}
			return FALSE;
		}

//		char Info[1024]; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//		char CurDir[MAX_PATH]; // <<<<<<<<<<<<<<<<<<<<<<<<<<<
//		FILE *fp;

//		GetCurrentDirectory(MAX_PATH, CurDir); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//		sprintf(Info, "URL == \"%s\"\nCurDir == \"%s\"", URL, CurDir); // <<<<<<<<<<<<<<<<<
//		MessageBox(NULL, Info, "Info:", 0); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		// If app is already running:
		HWND hWnd = FindWindow(MainWndClassName, "");
		if(hWnd) {
			if(x >= 0) PostMessage(hWnd, WM_COMMAND, ID_APP_ABOUT, MAKELPARAM(CMainWnd::idSetX, x));
			if(y >= 0) PostMessage(hWnd, WM_COMMAND, ID_APP_ABOUT, MAKELPARAM(CMainWnd::idSetY, y));
			if(w >= 0) PostMessage(hWnd, WM_COMMAND, ID_APP_ABOUT, MAKELPARAM(CMainWnd::idSetW, w));
			if(h >= 0) PostMessage(hWnd, WM_COMMAND, ID_APP_ABOUT, MAKELPARAM(CMainWnd::idSetH, h));
			PostMessage(hWnd, WM_COMMAND, ID_APP_ABOUT, MAKELPARAM(CMainWnd::idSetWindowPos, 0));
			if(strlen(URL) > 0) {
				ATOM a = GlobalAddAtom(URL);
				PostMessage(hWnd, WM_COMMAND, ID_APP_ABOUT, MAKELPARAM(CMainWnd::idSetURL, a));
			}
			return FALSE;
		}

		m_pMainWnd = new CMainWnd(x, y, w, h, URL);

//		CCreateHTMLImage img;
//		img.Create(m_pMainWnd);
//		img.CreateImage("d:\\work\\html\\1.html", "1.bmp", CSize(600, 480), CSize());

		m_pMainWnd->ShowWindow(m_nCmdShow);
		m_pMainWnd->UpdateWindow();
		return TRUE;
	}
};

CBrowserApp g_BrowserApp;