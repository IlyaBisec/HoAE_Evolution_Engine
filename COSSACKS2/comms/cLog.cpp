#include "comms.h"

#pragma comment (lib, "comctl32.lib")

// TO FIX:
// Multiple call to ShowDlg starts dlg blinking.
// After ERROR when dlg stays visible Exit button is enabled
// Some asserts are called twise!!!!!

const cStr cLog::EndLn("\r\n");
cStr cLog::m_Log;
/*volatile */HWND cLog::m_hDlg = NULL;
/*volatile */cLog::eDlgMode cLog::m_Mode = cLog::DM_MESSAGE;
/*volatile */bool cLog::m_AutoExit = false;
/*volatile */int cLog::m_idClicked = 0;
/*volatile */bool cLog::m_ThreadIsCreated = false;
const char *cLog::m_DlgTitle = "Comms Runtime Library";

//-----------------------------------------------------------------------------
// cLog::Init
//-----------------------------------------------------------------------------
void cLog::Init() {
	if(!m_ThreadIsCreated) {
		m_ThreadIsCreated = true;
		_beginthread(ThreadFn, 0, NULL);
		while(!m_hDlg) {
			Sleep(10);
		}
	}
} // cLog::Init

//-----------------------------------------------------------------------------
// cLog::ThreadFn
//-----------------------------------------------------------------------------
void cLog::ThreadFn(PVOID) {
	cWin32::DlgTemplate Dlg;
	GenDlgTemplate(Dlg);
	m_hDlg = CreateDialogIndirect(GetModuleHandle(NULL), Dlg.ToDlgTemplatePtr(), NULL, (DLGPROC)cLog::DlgProc);
	MSG Msg;
	ZeroMemory(&Msg, sizeof(Msg));
	while(Msg.message != WM_QUIT) {
		if(PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)) {
			if(!IsDialogMessage(m_hDlg, &Msg)) {
				TranslateMessage(&Msg);
				DispatchMessage(&Msg);
			}
		} else {
			Sleep(10);
		}
	}
	m_ThreadIsCreated = false;
	_endthread();
} // cLog::ThreadFn

//-----------------------------------------------------------------------------
// cLog::SetIcon
//-----------------------------------------------------------------------------
void cLog::SetIcon(HICON hIcon) {
	if(!m_hDlg) {
		Init();
	}
	if(m_hDlg) {
		SendMessage(m_hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hIcon);
		SendMessage(m_hDlg, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)hIcon);
	}
} // cLog::SetIcon

//-----------------------------------------------------------------------------
// cLog::ShowDlg
//-----------------------------------------------------------------------------
void cLog::ShowDlg() {
	if(!m_hDlg) {
		Init();
	}
	if(m_hDlg) {
		EnableControls();
		AdjustLayout();
		if(!IsWindowVisible(m_hDlg)) {
			SetForegroundWindow(m_hDlg);
			ShowWindow(m_hDlg, SW_SHOW);
		}
		UpdateLogEditBox();
	}
} // cLog::ShowDlg

//-----------------------------------------------------------------------------
// cLog::HideDlg
//-----------------------------------------------------------------------------
void cLog::HideDlg() {
	if(m_hDlg && !IsModal()) {
		ShowWindow(m_hDlg, SW_HIDE);
	}
} // cLog::HideDlg

//-----------------------------------------------------------------------------
// cLog::AppendMessage
//-----------------------------------------------------------------------------
void cLog::AppendMessage(const char *Format, va_list Args) {
	cStr Buffer;
	cStr::Printfv(Buffer, Format, Args);
	m_Log.Append(Buffer);

	if(Buffer.Length() > 0 && !cStr::EqualsNoCase(m_Log.GetRight(2), "\r\n")) {
		m_Log.Append("\r\n");
	}
	UpdateLogEditBox();
} // cLog::AppendMessage

//-----------------------------------------------------------------------------
// cLog::Message
//-----------------------------------------------------------------------------
void cLog::Message(const char *Format, ...) {
	va_list Args;
	va_start(Args, Format);
	AppendMessage(Format, Args);
	va_end(Args);
} // cLog::Message

//-----------------------------------------------------------------------------
// cLog::Warning
//-----------------------------------------------------------------------------
void cLog::Warning(const char *Format, ...) {
	va_list Args;
	va_start(Args, Format);
	ShowModalMessage(DM_WARNING, true, Format, Args);
	va_end(Args);
} // cLog::Warning

//-----------------------------------------------------------------------------
// cLog::Error
//-----------------------------------------------------------------------------
void cLog::Error(const char *Format, ...) {
	va_list Args;
	va_start(Args, Format);
	ShowModalMessage(DM_ERROR, true, Format, Args);
	va_end(Args);
} // cLog::Error

//-----------------------------------------------------------------------------
// cLog::ErrorNoExit
//-----------------------------------------------------------------------------
void cLog::ErrorNoExit(const char *Format, ...) {
	va_list Args;
	va_start(Args, Format);
	ShowModalMessage(DM_ERROR, false, Format, Args);
	va_end(Args);
} // cLog::ErrorNoExit

//------------------------------------------------------------------------------------------------------------
// cLog::ShowModalMessage
//------------------------------------------------------------------------------------------------------------
int cLog::ShowModalMessage(const cLog::eDlgMode Mode, const bool AutoExit, const char *Format, va_list Args) {
	if(!m_hDlg) {
		Init();
	}
	while(IsModal()) {
		Sleep(10);
	}
	m_Mode = Mode;
	m_idClicked = 0;
	m_AutoExit = AutoExit;
	AppendMessage(Format, Args);
	const BOOL DlgWasVisible = IsWindowVisible(m_hDlg);
	// Set new title:
	cStr Title(m_DlgTitle);
	if(DM_WARNING == m_Mode) {
		Title << " Warning";
	} else if(DM_ERROR == m_Mode) {
		Title << " Error";
	} else if(DM_ASSERT == m_Mode) {
		Title << " Assert";
	}
	SetWindowText(m_hDlg, Title);

	ShowDlg();

	int id;
	while((id = m_idClicked) == 0) {
		Sleep(10);
	}
	m_idClicked = 0;

	// Restoring title:
	SetWindowText(m_hDlg, m_DlgTitle);

	m_Mode = DM_MESSAGE;
	if(!DlgWasVisible || idDebug == id) {
		HideDlg();
	}

	return id;
} // cLog::ShowModalMessage

//-----------------------------------------------------------------------------
// cLog::GenDlgTemplate
//-----------------------------------------------------------------------------
void cLog::GenDlgTemplate(cWin32::DlgTemplate &Dlg) {
	Dlg.Free();
	Dlg.Create(m_DlgTitle, DS_CENTER | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_POPUP | WS_CAPTION | WS_SYSMENU |
		WS_THICKFRAME | DS_FIXEDSYS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CONTROLPARENT | WS_EX_APPWINDOW, 0, 0, 380, 260, "Tahoma", 8);
	Dlg.AddEditBox("", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | WS_HSCROLL | WS_VSCROLL | ES_READONLY | WS_TABSTOP, 0, 0, 0, 10, 10, idLog);
	Dlg.AddControl("msctls_statusbar32", "", WS_VISIBLE, 0, 0, 0, 0, 0, idStatusBar);
	Dlg.AddButton("&Debug", WS_VISIBLE | WS_TABSTOP, 0, 0, 0, 10, 10, idDebug);
	Dlg.AddButton("&Ignore", WS_VISIBLE | WS_TABSTOP, 0, 0, 0, 10, 10, idIgnore);
	Dlg.AddButton("Ignore &Always", WS_VISIBLE | WS_TABSTOP, 0, 0, 0, 10, 10, idIgnoreAlways);
	Dlg.AddButton("&Copy", WS_VISIBLE | WS_TABSTOP, 0, 0, 0, 10, 10, idCopy);
	Dlg.AddButton("E&xit", WS_VISIBLE | WS_TABSTOP, 0, 0, 0, 10, 10, idExit);
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_BAR_CLASSES;
	InitCommonControlsEx(&icc);
} // cLog::GenDlgTemplate

//-----------------------------------------------------------------------------
// cLog::AdjustLayout
//-----------------------------------------------------------------------------
void cLog::AdjustLayout() {
	const int cxButton = 83;
	const int cyButton = 24;
	const int ButtonSpace = 4;
	const int DlgSpace = 4;
	const int StatusBarSpace = 2;
	// Dlg Size:
	RECT rc;
	GetClientRect(m_hDlg, &rc);
	const int cxDlg = rc.right - rc.left;
	const int cyDlg = rc.bottom - rc.top;
	// Status Bar Size:
	GetWindowRect(GetDlgItem(m_hDlg, idStatusBar), &rc);
	const int cyStatusBar = rc.bottom - rc.top;
	// Layout:
	MoveWindow(GetDlgItem(m_hDlg, idLog), DlgSpace, DlgSpace, cxDlg - 2 * DlgSpace,
		cyDlg - 2 * DlgSpace - cyStatusBar - cyButton - StatusBarSpace, TRUE);
	int yPos = cyDlg - cyStatusBar - cyButton - StatusBarSpace;
	int xPos = DlgSpace + 4 * cxButton + 4 * ButtonSpace;/*/cxDlg - DlgSpace - cxButton;*/
	MoveWindow(GetDlgItem(m_hDlg, idExit), xPos, yPos, cxButton, cyButton, TRUE);
	xPos -= ButtonSpace + cxButton;
	MoveWindow(GetDlgItem(m_hDlg, idCopy), xPos, yPos, cxButton, cyButton, TRUE);
	xPos -= ButtonSpace + cxButton;
	MoveWindow(GetDlgItem(m_hDlg, idIgnoreAlways), xPos, yPos, cxButton, cyButton, TRUE);
	xPos -= ButtonSpace + cxButton;
	MoveWindow(GetDlgItem(m_hDlg, idIgnore), xPos, yPos, cxButton, cyButton, TRUE);
	xPos -= ButtonSpace + cxButton;
	MoveWindow(GetDlgItem(m_hDlg, idDebug), xPos, yPos, cxButton, cyButton, TRUE);
	InvalidateRect(GetDlgItem(m_hDlg, idStatusBar), NULL, TRUE);

	InvalidateRect(GetDlgItem(m_hDlg, idDebug), NULL, TRUE);
	InvalidateRect(GetDlgItem(m_hDlg, idIgnore), NULL, TRUE);
	InvalidateRect(GetDlgItem(m_hDlg, idIgnoreAlways), NULL, TRUE);
	InvalidateRect(GetDlgItem(m_hDlg, idCopy), NULL, TRUE);
	InvalidateRect(GetDlgItem(m_hDlg, idExit), NULL, TRUE);
} // cLog::AdjustLayout

//-----------------------------------------------------------------------------
// cLog::UpdateLogEditBox
//-----------------------------------------------------------------------------
void cLog::UpdateLogEditBox() {
	if(m_hDlg) {
		SendMessage(GetDlgItem(m_hDlg, idLog), WM_SETTEXT, 0, (LPARAM)m_Log.ToCharPtr());
		SendMessage(GetDlgItem(m_hDlg, idLog), EM_LINESCROLL, 0, (LPARAM)0xffff);
		SendMessage(GetDlgItem(m_hDlg, idLog), EM_SETSEL, 0xffff, 0xffff);
	}
} // cLog::UpdateLogEditBox

//-----------------------------------------------------------------------------
// cLog::EnableControls
//-----------------------------------------------------------------------------
void cLog::EnableControls() {
	if(m_hDlg) {
		EnableWindow(GetDlgItem(m_hDlg, idDebug), DM_ASSERT == m_Mode);
		EnableWindow(GetDlgItem(m_hDlg, idIgnore), DM_ASSERT == m_Mode || DM_WARNING == m_Mode);
		EnableWindow(GetDlgItem(m_hDlg, idIgnoreAlways), DM_ASSERT == m_Mode);
		EnableWindow(GetDlgItem(m_hDlg, idCopy), IsModal());
		EnableWindow(GetDlgItem(m_hDlg, idExit), IsModal());
	}
} // cLog::EnableControls

//---------------------------------------------------------------------------------
// cLog::DlgProc
//---------------------------------------------------------------------------------
LRESULT CALLBACK cLog::DlgProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam) {
	switch(Msg) {
		case WM_INITDIALOG:
			SendMessage(GetDlgItem(hDlg, idLog), WM_SETFONT, (WPARAM)GetStockObject(SYSTEM_FIXED_FONT), 0);
			EnableMenuItem(GetSystemMenu(hDlg, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);
			m_hDlg = hDlg;
			break;
		case WM_CTLCOLORSTATIC:
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(255, 255, 255));
			return (LRESULT)GetStockObject(BLACK_BRUSH);
		case WM_SIZING:
			cWin32::OnSizing(hDlg, wParam, lParam, 478, 350);
			break;
		case WM_SIZE:
			AdjustLayout();
			SendMessage(GetDlgItem(hDlg, idStatusBar), Msg, wParam, lParam);
			return TRUE;
			break;
		case WM_COMMAND:
			WORD id;
			id = LOWORD(wParam);
			if(idCopy == id) {
				cWin32::CopyToClipboard(m_Log, hDlg);
			} else if(m_AutoExit && idExit == id) {
				DestroyWindow(hDlg);
				m_hDlg = NULL;
				exit(0);
			} else if(idDebug == id || idIgnore == id || idIgnoreAlways == id || (idExit == id && !m_AutoExit)) {
				if(0 == m_idClicked) {
					m_idClicked = id;
				}
			}
			break;
	}
	return FALSE;
} // cLog::DlgProc