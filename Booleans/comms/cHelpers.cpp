#include "cHelpers.h"

namespace comms {

// cHelpers::CenterWindow
BOOL cHelpers::CenterWindow(HWND hwndChild, HWND hwndParent) {
	RECT rcChild, rcParent;
	int cxChild, cyChild, cxParent, cyParent;
	int cxScreen, cyScreen, xNew, yNew;
	HDC hdc;
	
	// Child:
	GetWindowRect(hwndChild, &rcChild);
	cxChild = rcChild.right - rcChild.left;
	cyChild = rcChild.bottom - rcChild.top;
	
	if(hwndParent) {
		// Parent:
		GetWindowRect(hwndParent, &rcParent);
		cxParent = rcParent.right - rcParent.left;
		cyParent = rcParent.bottom - rcParent.top;
	} else {
		cxParent = GetSystemMetrics(SM_CXSCREEN);
		cyParent = GetSystemMetrics(SM_CYSCREEN);
		rcParent.left = 0;
		rcParent.top = 0;
		rcParent.right = cxParent;
		rcParent.bottom= cyParent;
	}
	
	// Display limits:
	hdc = GetDC(hwndChild);
	cxScreen = GetDeviceCaps(hdc, HORZRES);
	cyScreen = GetDeviceCaps(hdc, VERTRES);
	ReleaseDC(hwndChild, hdc);
	
	// xNew:
	xNew = rcParent.left + ((cxParent - cxChild) / 2);
	if (xNew < 0) {
		xNew = 0;
	} else if ((xNew + cxChild) > cxScreen) {
		xNew = cxScreen - cxChild;
	}
	
	// yNew:
	yNew = rcParent.top  + ((cyParent - cyChild) / 2);
	if (yNew < 0) {
		yNew = 0;
	} else if ((yNew + cyChild) > cyScreen) {
		yNew = cyScreen - cyChild;
	}
	
	// Posing:
	return SetWindowPos(hwndChild, NULL, xNew, yNew, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
} // cHelpers::CenterWindow

//-----------------------------------------------------------------------------
// Exec
//-----------------------------------------------------------------------------
int cHelpers::Exec(const cHelpers::ExecArgs &Args) {
	SHELLEXECUTEINFO sei;
	ZeroMemory(&sei, sizeof(sei));
	sei.cbSize = sizeof(sei);
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	sei.lpVerb = Args.pVerb;
	sei.lpFile = Args.pFileName;
	sei.lpParameters = Args.pParameters;
	sei.lpDirectory = Args.pDirectory;
	sei.nShow = Args.nShow;

	if(ShellExecuteEx(&sei)) {
		if(Args.fWait) {
			while(WaitForSingleObject(sei.hProcess, Args.msTimeOut) == WAIT_TIMEOUT);
			DWORD ExitCode;
			if(GetExitCodeProcess(sei.hProcess, &ExitCode)) return 0;
			else return -1;
		} else return 0;
	} else return -1;
}

} // comms