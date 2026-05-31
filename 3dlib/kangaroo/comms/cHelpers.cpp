#include "comms.h"

//--------------------------------------------------------------------------------------------------
// OnSizing
//--------------------------------------------------------------------------------------------------
void cHelpers::OnSizing(HWND hWnd, WPARAM wParam, LPARAM lParam, int MinClientLx, int MinClientLy) {
	DWORD Style = GetWindowLong(hWnd, GWL_STYLE);
	DWORD ExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
	RECT rc;
	SetRect(&rc, 0, 0, MinClientLx, MinClientLy);
	AdjustWindowRectEx(&rc, Style, false, ExStyle);
	int MinLx = rc.right - rc.left, MinLy = rc.bottom - rc.top;

	LPRECT lprc = (LPRECT)lParam;
	if(!lprc) return;
	int Lx = lprc->right - lprc->left;
	int Ly = lprc->bottom - lprc->top;
	bool fTop = false, fBottom = false, fRight = false, fLeft = false;
	switch(wParam) {
		case WMSZ_TOP: fTop = true;
			break;
		case WMSZ_BOTTOM: fBottom = true;
			break;
		case WMSZ_RIGHT: fRight = true;
			break;
		case WMSZ_LEFT: fLeft = true;
			break;
		case WMSZ_BOTTOMLEFT: fBottom = fLeft = true;
			break;
		case WMSZ_BOTTOMRIGHT: fBottom = fRight = true;
			break;
		case WMSZ_TOPLEFT: fTop = fLeft = true;
			break;
		case WMSZ_TOPRIGHT: fTop = fRight = true;
			break;
		default:
			break;
	}
	if(Lx < MinLx) {
		if(fRight) lprc->right = lprc->left + MinLx;
		else if(fLeft) lprc->left = lprc->right - MinLx;
	}
	if(Ly < MinLy) {
		if(fTop) lprc->top = lprc->bottom - MinLy;
		else if(fBottom) lprc->bottom = lprc->top + MinLy;
	}
}

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

//---------------------------------------------------------------------------------------------
// cHelpers::SearchFiles : bool(cList<FileInfo> &, const cStr &, const cStr &Extension)
//---------------------------------------------------------------------------------------------
bool cHelpers::SearchFiles(cList<FileInfo> &Files, const cStr &Folder, const cStr &Extension) {
	HANDLE h = NULL;
	WIN32_FIND_DATA fd;
	int i = 0;
	FileInfo fi;
	
	char CurDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, CurDir);

	Files.Free();
	if(!SetCurrentDirectory(Folder.ToChar())) return false;
	cStr Mask = cStr("*.") + Extension;
	if((h = FindFirstFile(Mask.ToChar(), &fd)) != INVALID_HANDLE_VALUE) {
		if(fd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY) {
			fi.Name = fd.cFileName;
			Files.Add(fi);
		}
	} else return false;
	while(FindNextFile(h, &fd) != 0) {
		if(fd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY) {
			fi.Name = fd.cFileName;
			Files.Add(fi);
		}
	}
	FindClose(h);

	SetCurrentDirectory(CurDir);

	for(int i = 0; i < Files.Count(); i++) {
		FileInfo &r = Files[i];
		r.Path = Folder;
		r.Path.AppendBkSp();
		r.Path.Append(r.Name);
	}
	return true;
}