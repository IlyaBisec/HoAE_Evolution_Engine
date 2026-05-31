#include "comms.h"

#ifdef _WIN32
#pragma warning(disable: 4244)
#endif // _WIN32

namespace cWin32 {

//-----------------------------------------------------------------------------
// CopyToClipboard
//-----------------------------------------------------------------------------
bool CopyToClipboard(const char *Text, HWND hWnd) {
	if(!OpenClipboard(hWnd)) {
		return false;
	}
	EmptyClipboard();
	const size_t Len = strlen(Text);
	if(Len == 0) {
		CloseClipboard();
		return true;
	}
	HGLOBAL hBuffer = GlobalAlloc(GMEM_MOVEABLE, Len + 1);
	if(NULL == hBuffer) {
		CloseClipboard();
		return false;
	}
	char *pBuffer = (char *)GlobalLock(hBuffer);
	CopyMemory(pBuffer, Text, Len);
	pBuffer[Len] = 0;
	GlobalUnlock(hBuffer);

	SetClipboardData(CF_TEXT, hBuffer);
	CloseClipboard();
	return true;
} // CopyToClipboard

//-----------------------------------------------------------------------------
// CopyFromClipboard
//-----------------------------------------------------------------------------
void CopyFromClipboard(cStr &Text, HWND hWnd) {
	Text.Free();
	if(!IsClipboardFormatAvailable(CF_TEXT)) {
		return;
	}
	if(!OpenClipboard(hWnd)) {
		return;
	}
	HGLOBAL hBuffer = GetClipboardData(CF_TEXT);
	if(hBuffer != NULL) {
		char *pBuffer = (char *)GlobalLock(hBuffer);
		if(pBuffer != NULL) {
			Text.Copy(pBuffer);
			GlobalUnlock(hBuffer);
		}
	}
	CloseClipboard();
} // CopyFromClipboard

//----------------------------------------------------------------------------------------
// OnSizing
//----------------------------------------------------------------------------------------
void OnSizing(HWND hWnd, WPARAM wParam, LPARAM lParam, int MinClientLx, int MinClientLy) {
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
} // OnSizing

//-----------------------------------------------------------------------------
// CenterWindow
//-----------------------------------------------------------------------------
BOOL CenterWindow(HWND hwndChild, HWND hwndParent) {
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
} // CenterWindow

//-----------------------------------------------------------------------------
// Exec
//-----------------------------------------------------------------------------
int Exec(const ExecArgs &Args) {
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
			while(WaitForSingleObject(sei.hProcess, Args.TimeOutMs) == WAIT_TIMEOUT);
			DWORD ExitCode;
			if(GetExitCodeProcess(sei.hProcess, &ExitCode)) return 0;
			else return -1;
		} else return 0;
	} else return -1;
} // Exec

//-----------------------------------------------------------------------------------
// SearchFiles
//-----------------------------------------------------------------------------------
bool SearchFiles(cList<FileInfo> &Files, const cStr &Folder, const cStr &Extension) {
	HANDLE h = NULL;
	WIN32_FIND_DATA fd;
	int i = 0;
	FileInfo fi;
	
	char CurDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, CurDir);

	Files.Free();
	if(!SetCurrentDirectory(Folder)) return false;
	cStr Mask = cStr("*.") + Extension;
	if((h = FindFirstFile(Mask, &fd)) != INVALID_HANDLE_VALUE) {
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
} // SearchFiles

//*****************************************************************************
// DlgTemplate
//*****************************************************************************

//---------------------------------------------------------------------------------------------------------------------------------------------
// DlgTemplate::Create
//---------------------------------------------------------------------------------------------------------------------------------------------
void DlgTemplate::Create(LPCSTR Title, DWORD Style, DWORD ExStyle, int xPos, int yPos, int Width, int Height, LPCSTR FontName, WORD FontSize) {
	Free();
	
	// Dialog template structure:
	DLGTEMPLATE dt;
	ZeroMemory(&dt, sizeof(dt));
	dt.style = Style;
	if(FontName != NULL) {
		dt.style |= DS_SETFONT;
	}
	dt.dwExtendedStyle = ExStyle;
	dt.x = xPos;
	dt.y = yPos;
	dt.cx = Width;
	dt.cy = Height;
	AppendData(&dt, sizeof(dt));
	
	// Menu array:
	AppendData("\0", 2); // 0x0000 - no menu.

	// Class array:
	AppendData("\0", 2); // 0x0000 - predefined dialog box class.

	// Title array:
	AppendStr(Title); // Null - terminated Unicode title string.

	if(FontName != NULL) {
		// 16 - bit point size value:
		AppendData(&FontSize, sizeof(WORD));
		// Typeface array:
		AppendStr(FontName); // Null - terminated Unicode string with the name of the typeface for the font.
	}
} // DlgTemplate::Create

//----------------------------------------------------------------------------------------------------------------------------------------------
// DlgTemplate::AddControl
//----------------------------------------------------------------------------------------------------------------------------------------------
void DlgTemplate::AddControl(LPCSTR ClassName, LPCSTR Caption, DWORD Style, DWORD ExStyle, int xPos, int yPos, int Width, int Height, WORD id) {
	AlignDlgTemplateToDWORD(); // Align DLGITEMTEMPLATE on DWORD boundary.

	// Dialog item template structure:
	DLGITEMTEMPLATE dit;
	ZeroMemory(&dit, sizeof(dit));
	dit.style = Style;
	dit.dwExtendedStyle = ExStyle;
	dit.x = xPos;
	dit.y = yPos;
	dit.cx = Width;
	dit.cy = Height;
	dit.id = id;
	AppendData(&dit, sizeof(dit));
	
	// Class array:
	AppendStr(ClassName); // Null - terminated Unicode string with the name of a registered window class.

	// Title array:
	AppendStr(Caption); // Null - terminated Unicode caption string.

	// Creation data array:
	AppendData("\0", 2); // 0x0000 - no creation data.

	((DLGTEMPLATE *)m_DlgTemplate.ToPtr())->cdit++;
} // DlgTemplate::AddControl

//-----------------------------------------------------------------------------
// DlgTemplate::AppendData
//-----------------------------------------------------------------------------
void DlgTemplate::AppendData(const void *Src, int Size) {
	int index = m_DlgTemplate.Count();
	m_DlgTemplate.Add(0, Size);
	CopyMemory(&m_DlgTemplate[index], Src, Size);
} // DlgTemplate::AppendData

//-----------------------------------------------------------------------------
// DlgTemplate::AppendStr
//-----------------------------------------------------------------------------
void DlgTemplate::AppendStr(LPCSTR Str) {
	int Len = MultiByteToWideChar(CP_ACP, 0, Str, -1, NULL, 0);
	if(Len > 0) {
		WCHAR *WideStr = new WCHAR[Len];
		MultiByteToWideChar(CP_ACP, 0, Str, -1, WideStr, Len);
		AppendData(WideStr, Len * sizeof(WCHAR));
		delete WideStr;
	}
} // DlgTemplate::AppendStr

//-----------------------------------------------------------------------------
// DlgTemplate::AlignDlgTemplateToDWORD
//-----------------------------------------------------------------------------
void DlgTemplate::AlignDlgTemplateToDWORD() {
	const int PaddingSize = m_DlgTemplate.Count() % sizeof(DWORD);
	if(PaddingSize) {
		m_DlgTemplate.Add(0, PaddingSize);
	}
} // DlgTemplate::AlignDlgTemplateToDWORD

//*****************************************************************************
// FileDialog
//*****************************************************************************

//-------------------------------------------------------------------------------------------------------
// FileDialog.ctor
//-------------------------------------------------------------------------------------------------------
FileDialog::FileDialog(HWND hParentWnd, const char *InitialPath, const char *Title, const char *Filter) {
	cAssert(sizeof(m_ofn) == 88);
	ZeroMemory(&m_ofn, sizeof(m_ofn));
	m_ofn.lStructSize = sizeof(m_ofn);
	m_ofn.FlagsEx = 0x00000001; // OFN_EX_NOPLACESBAR

	SetParent(hParentWnd);

	m_ofn.lpstrFilter = Filter;
	m_ofn.nFilterIndex = 1;
	m_Buffer[0] = '\0';
	m_ofn.lpstrFile = m_Buffer;
	m_ofn.nMaxFile = MAX_PATH;

	SetInitialPath(InitialPath);
	
	m_ofn.lpstrTitle = Title;
	m_ofn.lpfnHook = HookProc;
	m_ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ENABLEHOOK | OFN_EXPLORER | OFN_ENABLESIZING;
	
	// Creating preview dialog template:
	m_PreviewDlgTemplate.Create("", DS_3DLOOK | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, WS_EX_CONTROLPARENT, 0, 0, 200, 130, "Fixedsys", 10);
	m_PreviewDlgTemplate.AddStatic("", WS_VISIBLE | WS_CHILDWINDOW | SS_CENTER, 0, 0, 4, 105, 10, idTitle);
	m_PreviewDlgTemplate.AddStatic("", SS_OWNERDRAW | WS_VISIBLE, 0, 4, 14, 96, 96, idViewPort);
	m_PreviewDlgTemplate.AddStatic("", WS_VISIBLE | WS_CHILDWINDOW, 0, 4, 112, 96, 18, idInfo);
	m_PreviewDlgTemplate.AddStatic("", WS_VISIBLE, 0, 101, 0, 99, 130, stc32);

	m_ofn.Flags |= OFN_ENABLETEMPLATEHANDLE;
	m_ofn.hInstance = (HINSTANCE)m_PreviewDlgTemplate.ToDlgTemplatePtr();

	m_hDlg = NULL;
	m_hViewPort = NULL;
	m_hTitle = NULL;
	m_hFileNameCombo = NULL;
} // FileDialog.ctor

// FileDialog::SetParent
void FileDialog::SetParent(HWND hParent) {
	m_ofn.hwndOwner = hParent;
}

//-----------------------------------------------------------------------------
// FileDialog::SetInitialPath
//-----------------------------------------------------------------------------
void FileDialog::SetInitialPath(const char *InitialPath /*= NULL*/) {
	// Creating InitialDir:
	m_InitialDir.SetLength(MAX_PATH);
	GetModuleFileName(NULL, m_InitialDir.ToCharPtr(), MAX_PATH);
	m_InitialDir.CalcLength();
	m_InitialDir.StripFileName();
	if(InitialPath && cStr::Length(InitialPath) > 0) {
		if(InitialPath[0] == '/' || InitialPath[0] == '\\') { // Path is relative to the current.
			m_InitialDir.AppendPath(InitialPath);
		} else {
			m_InitialDir = InitialPath; // Path is absolute.
			m_InitialDir.SlashesToBackSlashes();
		}
	}
	m_ofn.lpstrInitialDir = m_InitialDir.ToCharPtr();
} // FileDialog::SetInitialPath

// FileDialog::GetInitialPath
const cStr & FileDialog::GetInitialPath() const {
	return m_InitialDir;
}

//-----------------------------------------------------------------------------
// FileDialog::DoModal
//-----------------------------------------------------------------------------
BOOL FileDialog::DoModal() {
	// Storing CurDir:
	cStr CurDir(cStr::SetLengthCtor, MAX_PATH);
	GetCurrentDirectory(MAX_PATH, CurDir);
	CurDir.CapLength(cStr::Length(CurDir));
	
	// Open File Dialog:
	m_FilePathName.Free();
	HookProc(NULL, WM_SETOWNER, (WPARAM)this, (LPARAM)0);
	m_Buffer[0] = '\0';
	BOOL Load = GetOpenFileName(reinterpret_cast<OPENFILENAME *>(&m_ofn));
	OnUnloadFileForPreview(); // To free allocated data.
	HookProc(NULL, WM_SETOWNER, (WPARAM)NULL, (LPARAM)0);
	if(Load) {
		cAssert(FileExist(m_FilePathName)); // This dlg will not allow to load file that does no exist!
		SetInitialPath(m_FilePathName.GetFilePath()); // Storing last loaded from path in initial dir.
	} else {
		m_FilePathName.Free();
	}
	
	// Restoring CurDir:
	SetCurrentDirectory(CurDir);

	return Load;
} // FileDialog::DoModal

//-----------------------------------------------------------------------------
// FileDialog::FindCtrls
//-----------------------------------------------------------------------------
BOOL CALLBACK FileDialog::FindCtrls(HWND hWnd, LPARAM lParam) {
	FileDialog *pOwner = (FileDialog *)lParam;
	if(pOwner) {
		// Does this child contains ctrls?
		HWND hViewPort = GetDlgItem(hWnd, idViewPort);
		HWND hTitle = GetDlgItem(hWnd, idTitle);
		HWND hInfo = GetDlgItem(hWnd, idInfo);
		if(hViewPort) {
			pOwner->m_hViewPort = hViewPort;
		}
		if(hTitle) {
			pOwner->m_hTitle = hTitle;
		}
		if(hInfo) {
			pOwner->m_hInfo = hInfo;
		}
	}
	return TRUE;
} // FileDialog::FindCtrls

//-----------------------------------------------------------------------------
// FileDialog::DrawNoPreview
//-----------------------------------------------------------------------------
void FileDialog::DrawNoPreview(cDib24 &dib) {
	const float d = 20.0f;
	const float t = 7.0f;
	const float w = (float)dib.GetWidth();
	const float h = (float)dib.GetHeight();
	const float dx = w <= h ? d : d + (w - h) / 2.0f;
	const float dy = h <= w ? d : d + (h - w) / 2.0f;

	const cVec2 lt(dx, dy);
	const cVec2 rt(w - dx, dy);
	const cVec2 lb(dx, h - dy);
	const cVec2 rb(w - dx, h - dy);
	const cVec2 xs(t, 0);
	const cVec2 ys(0, t);

	const cVec2 vb[] = { /* First strip: */ lt, lt + ys, rb - xs, rb, rb - ys, lt + xs,
		/* Second strip: */ lb, lb + xs, rt + ys, rt, rt - xs, lb - ys };
	const int ib[] = { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 6, 7, 8, 6, 8, 9, 6, 9, 10, 6, 10, 11 };
	cDib24::Pixel p = cDib24::Pixel::FromColor(cColor::Gray25);
	for(int i = 0; i < sizeof(ib) / sizeof(ib[0]); i += 3) {
		dib.Tri(vb[ib[i]], vb[ib[i + 1]], vb[ib[i + 2]], p);
	}
} // FileDialog::DrawNoPreview

//-----------------------------------------------------------------------------
// FileDialog::UpdatePreview
//-----------------------------------------------------------------------------
void FileDialog::UpdatePreview() {
	// Retrieve file name from combo box:
	cAssert(m_hFileNameCombo);
	cStr FileName(cStr::SetLengthCtor, MAX_PATH, '\0');
	GetWindowText(m_hFileNameCombo, FileName.ToCharPtr(), MAX_PATH);
	FileName.CalcLength();
	// If file name is w/o path we should set current:
	char CurDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, CurDir);
	FileName.SetFileDefaultPath(CurDir);

	if(!cStr::EqualsNoCase(m_FilePathName, FileName)) {
		bool Exist = FileExist(FileName);
		EnableWindow(GetDlgItem(GetParent(m_hDlg), IDOK), Exist);
		cStr LoadedFileTitle, LoadedFileInfo;
		if(Exist) {
			m_FilePathName = FileName;
			OnUnloadFileForPreview();
			OnLoadFileForPreview(FileName, LoadedFileInfo);
			LoadedFileTitle = cStr(FileName).GetFileBase();
			if(LoadedFileTitle.Length() > 22) {
				LoadedFileTitle.CapLength(22);
				LoadedFileTitle.Append("...");
			}
		} else {
			m_FilePathName.Free();
			OnUnloadFileForPreview();
			LoadedFileTitle = "No Preview";
		}
		SetWindowText(m_hTitle, LoadedFileTitle);
		SetWindowText(m_hInfo, LoadedFileInfo);
		InvalidateRect(m_hViewPort, NULL, FALSE);
	}
} // FileDialog::UpdatePreview

//----------------------------------------------------------------------------------------------
// FileDialog::ParentDlgProc
//----------------------------------------------------------------------------------------------
UINT_PTR CALLBACK FileDialog::ParentDlgProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam) {
	static FileDialog *pOwner = NULL;
	static DLGPROC pPrevDlgProc = NULL;
	static bool LoadIsClicked = false;
	switch(Msg) {
		case WM_SETOWNER:
			pOwner = (FileDialog *)wParam;
			LoadIsClicked = false;
			return FALSE;
		case WM_SETPREVDLGPROC:
			pPrevDlgProc = (DLGPROC)wParam;
			return FALSE;
		case WM_COMMAND:
			if(LOWORD(wParam) == IDOK) {
				LoadIsClicked = true;
			} else if(!LoadIsClicked && LOWORD(wParam) == cmb13 && HIWORD(wParam) == CBN_EDITCHANGE) {
				pOwner->UpdatePreview();
			}
			break;
		default:
			break;
	}
	return CallWindowProc((WNDPROC)pPrevDlgProc, hDlg, Msg, wParam, lParam);
} // FileDialog::ParentDlgProc

//-----------------------------------------------------------------------------------------
// FileDialog::HookProc
//-----------------------------------------------------------------------------------------
UINT_PTR CALLBACK FileDialog::HookProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam) {
	static FileDialog *pOwner = NULL;
	const OFNOTIFY *pNotify;
	LPDRAWITEMSTRUCT lpdis;
	int Width, Height;
	static bool IsDragging;
	static POINT PrevMousePos;
	POINT CurMousePos;
	int dx, dy;
	static int xShift, yShift;
	RECT rc;
	cDib24::Pixel BackGround;
	int Len;
	cStr SelSpec;
	
	switch(Msg) {
		case WM_LBUTTONDOWN:
			cAssert(pOwner);
			GetCursorPos(&PrevMousePos);
			GetWindowRect(pOwner->m_hViewPort, &rc);
			if(pOwner->m_FilePathName.Length() && PtInRect(&rc, PrevMousePos)) { // File exist and click within viewport.
				SetCapture(hDlg);
				IsDragging = true;
			}
			break;
		case WM_MOUSEMOVE:
			if(IsDragging) {
				GetCursorPos(&CurMousePos);
				dx = CurMousePos.x - PrevMousePos.x;
				dy = CurMousePos.y - PrevMousePos.y;
				xShift += dx;
				yShift += dy;
				PrevMousePos = CurMousePos;
				if(dx || dy) {
					cAssert(pOwner);
					InvalidateRect(pOwner->m_hViewPort, NULL, FALSE);
				}
			}
			break;
		case WM_LBUTTONUP:
			if(IsDragging) {
				ReleaseCapture();
				IsDragging = false;
			}
			break;
		case WM_DRAWITEM:
			if(idViewPort == (int)wParam) {
				cAssert(pOwner);
				lpdis = (LPDRAWITEMSTRUCT)lParam;
				Width = lpdis->rcItem.right - lpdis->rcItem.left;
				Height = lpdis->rcItem.bottom - lpdis->rcItem.top;
				if(Width != pOwner->m_dib.GetWidth() || Height != pOwner->m_dib.GetHeight()) {
					pOwner->m_dib.Init(Width, Height);
				}
				BackGround = cDib24::Pixel::FromRgb(GetSysColor(COLOR_3DFACE));
				pOwner->m_dib.Clear(BackGround);
				if(!pOwner->m_FilePathName.Length()) { // No preview:
					DrawNoPreview(pOwner->m_dib);
				} else {
					pOwner->OnRenderPreview(pOwner->m_dib, xShift, yShift);
				}
				pOwner->m_dib.ToDC(lpdis->hDC);
				return TRUE;
			}
			break;
		case WM_SETOWNER:
			pOwner = (FileDialog *)wParam;
			IsDragging = false;
			xShift = yShift = 0;
			break;
		case WM_NOTIFY:
			pNotify = (const OFNOTIFY *)lParam;
			switch(pNotify->hdr.code) {
				case CDN_INITDONE:
					cWin32::CenterWindow(GetParent(hDlg), NULL);
					cAssert(pOwner);
					pOwner->m_hDlg = hDlg;
					pOwner->m_hFileNameCombo = GetDlgItem(GetParent(hDlg), cmb13);
					SendMessage(GetParent(hDlg), CDM_SETCONTROLTEXT, (WPARAM)IDOK, (LPARAM)"Load");
					EnableWindow(GetDlgItem(GetParent(hDlg), IDOK), FALSE); // "Load" is initially disabled.
					EnumChildWindows(GetParent(hDlg), FindCtrls, (LPARAM)pOwner); // Find handlers to ViewPort && Title ctrls:
					// Subclassing parent dlg:
					ParentDlgProc(NULL, WM_SETOWNER, (WPARAM)pOwner, 0);
					ParentDlgProc(NULL, WM_SETPREVDLGPROC, (WPARAM)GetWindowLongPtr(GetParent(hDlg), DWLP_DLGPROC), 0);
					SetWindowLongPtr(GetParent(hDlg), DWLP_DLGPROC, reinterpret_cast<LONG_PTR>(ParentDlgProc));
					break;
				case CDN_SELCHANGE:
					Len = SendMessage(GetParent(hDlg), CDM_GETSPEC, 0, 0);
					SelSpec.Empty();
					if(Len > 0) {
						SelSpec.SetLength(Len);
						SendMessage(GetParent(hDlg), CDM_GETSPEC, (WPARAM)Len, (LPARAM)SelSpec.ToCharPtr());
					}
					SendMessage(pOwner->m_hFileNameCombo, WM_SETTEXT, 0, (LPARAM)SelSpec.ToCharPtr());
				case CDN_FOLDERCHANGE:
					cAssert(pOwner);
					pOwner->UpdatePreview();
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
	return FALSE;
} // FileDialog::HookProc

//-----------------------------------------------------------------------------
// FileDialog::FileExist
//-----------------------------------------------------------------------------
bool FileDialog::FileExist(const char *FileName) {
	struct _stat Buffer;
	int r = _stat(FileName, &Buffer);
	return r != -1 && (Buffer.st_mode & _S_IFREG);
} // FileDialog::FileExist

//*****************************************************************************
// MeshFileDialog
//*****************************************************************************

//---------------------------------------------------------------------------------------------
// MeshFileDialog.ctor
//---------------------------------------------------------------------------------------------
MeshFileDialog::MeshFileDialog(const HWND hParentWnd, const char *InitialPath, const char *Title)
: FileDialog(hParentWnd, InitialPath, Title, "All Mesh Files\0*.OBJ\0OBJ\0*.OBJ\0") {
	m_pMesh = NULL;
} // MeshFileDialog.ctor

//--------------------------------------------------------------------------------
// MeshFileDialog::OnLoadFileForPreview
//--------------------------------------------------------------------------------
void MeshFileDialog::OnLoadFileForPreview(const char *FileName, cStr &FileInfo) {
	m_pMesh = cIO::LoadMesh(FileName);
	if(!m_pMesh) { // Error during file operations.
		return;
	}

	if(m_pMesh->GetPositions().Count() < 1 || m_pMesh->GetPolyCount() < 1) { // Mesh is empty.
		delete m_pMesh;
		m_pMesh = NULL;
		return;
	}

	if(!m_pMesh->IsValid(true)) { // Mesh is invalid (bad topology, NaNs, etc.)
		delete m_pMesh;
		m_pMesh = NULL;
		return;
	}

	m_pMesh->Triangulate();

	if(m_pMesh->GetNormals().Count() < 1) { // Normals are not filled.
		m_pMesh->CalcNormals();
	} else {
		if(!m_pMesh->GetNormals().IsNormalized()) {
			m_pMesh->GetNormals().NormalizeSafe();
		}
	}
	
	const cSphere S = cSphere::FromPoints(m_pMesh->GetPositions().ToPtr(), m_pMesh->GetPositions().Count());
	m_pMesh->GetPositions() -= S.GetCenter();
	m_pMesh->GetPositions() *= cVec3(1.0f / S.GetRadius());
	
	// FileInfo:
	FileInfo << "Verts: " << m_pMesh->GetPositions().Count() << "\tMtls: " << m_pMesh->GetMaterials().Count();
	FileInfo << "\nTris: " << m_pMesh->GetPolyCount();
} // MeshFileDialog::OnLoadFileForPreview

//-----------------------------------------------------------------------------
// MeshFileDialog::OnUnloadFileForPreview
//-----------------------------------------------------------------------------
void MeshFileDialog::OnUnloadFileForPreview() {
	if(m_pMesh) {
		delete m_pMesh;
		m_pMesh = NULL;
	}
} // MeshFileDialog::OnUnloadFileForPreview

//-----------------------------------------------------------------------------
// MeshFileDialog::OnRenderPreview
//-----------------------------------------------------------------------------
void MeshFileDialog::OnRenderPreview(cDib24 &dib, int xShift, int yShift) {
//	static float Fps = 0.0f;
//	static cTimer Timer;
//	Timer.Begin();

	const int ZBufferSize = dib.GetWidth() * dib.GetHeight();
	if(ZBufferSize != m_ZBuffer.Count()) {
		m_ZBuffer.SetCount(ZBufferSize);
	}
	m_ZBuffer.Fill(cMath::FloatMaxValue);

	if(m_pMesh != NULL) {
		const float AspectWtoH = (float)dib.GetWidth() / (float)dib.GetHeight();
		const cMat4 Proj = cMat4::PerspectiveYFovLH(50.0f, AspectWtoH, 0.2f, 1000.0f);

		const cVec2 c((float)dib.GetWidth() / 2.0f, (float)dib.GetHeight() / 2.0f);
		const float Scale = cMath::Max(c.x, c.y);
		const cMat3 M = cAngles(160 - (float)yShift, -300 - (float)xShift, 0.0f).ToMat3();
		cMat3 View;
		cMat3::Invert(M, &View);

		// Transforming positions:
		cList<cVec3> TPositions;
		for(int i = 0; i < m_pMesh->GetPositions().Count(); i++) {
			TPositions.Add(cVec3::Transform(m_pMesh->GetPositions()[i], View));
		}
		// Transforming normals:
		cList<cVec3> TNormals;
		cList<float> Dots;
		for(int i = 0; i < m_pMesh->GetNormals().Count(); i++) {
			TNormals.Add(cVec3::Transform(m_pMesh->GetNormals()[i], View));
			Dots.Add(cMath::Abs(cVec3::Dot(TNormals.GetBack(), cVec3::AxisNegZ)));
		}

		int index = 0;
		while(index < m_pMesh->GetRaw().Count()) {
			const int Deg = m_pMesh->GetRaw()[index][0];
			const int idMtl = m_pMesh->GetRaw()[index][1];
			const cVec3i &i0 = m_pMesh->GetRaw()[index + 1];
			const cVec3i &i1 = m_pMesh->GetRaw()[index + 2];
			const cVec3i &i2 = m_pMesh->GetRaw()[index + 3];

			cVec3 Tri[3] = { TPositions[i0[0]], TPositions[i1[0]], TPositions[i2[0]] };
			cVec3 Normals[3] = { TNormals[i0[2]], TNormals[i1[2]], TNormals[i2[2]] };
			const float Ds[3] = { Dots[i0[2]], Dots[i1[2]], Dots[i2[2]] };

			Tri[0].z += 2.4f;
			Tri[1].z += 2.4f;
			Tri[2].z += 2.4f;

			Tri[0].TransformCoordinate(Proj);
			Tri[1].TransformCoordinate(Proj);
			Tri[2].TransformCoordinate(Proj);
		
			for(int k = 0; k < 3; k++) {
				Tri[k] *= Scale;
				Tri[k] += cVec3(c, 0.0f);
			}
			dib.DrawPolygonGouraud(Tri, Ds, 3, m_pMesh->GetMaterials()[idMtl], m_ZBuffer.ToPtr());

			index += Deg + 1;
		}
	}

//	if(Timer.End(64)) {
//		Fps = Timer.FramesPerSec;
//	}
//	if(Fps) {
//		dib.Str(cStr::FromFloat(Fps), 0, 0);
//	}
} // MeshFileDialog::OnRenderPreview

//*****************************************************************************
// ImageFileDialog
//*****************************************************************************

//---------------------------------------------------------------------------------------------
// ImageFileDialog.ctor
//---------------------------------------------------------------------------------------------
ImageFileDialog::ImageFileDialog(const HWND hParentWnd, const char *InitialPath, const char *Title)
: FileDialog(hParentWnd, InitialPath, Title, "All Image Files\0*.BMP;*.DDS;*.TGA\0BMP\0*.BMP\0DDS\0*.DDS\0TGA\0*.TGA\0") {
} // ImageFileDialog.ctor

//--------------------------------------------------------------------------------
// ImageFileDialog::OnLoadFileForPreview
//--------------------------------------------------------------------------------
void ImageFileDialog::OnLoadFileForPreview(const char *FileName, cStr &FileInfo) {
	const cImage *pTex = cIO::LoadImage(FileName);
//	if(pTex && pTex->IsCompressed()) {
//		cImage *t = ((CompressedTexture *)pTex)->Decompress();
//		delete pTex;
//		pTex = t;
//		t = NULL;
//	}
	if(pTex) {
		m_Image.Init(pTex->GetWidth(), pTex->GetHeight());
		for(int y = 0; y < m_Image.GetHeight(); y++) {
			for(int x = 0; x < m_Image.GetWidth(); x++) {
				const cImage::PixelRgb8 p = pTex->GetPixelRgb8(x, y);
				m_Image.SetPixel(x, y, cDib24::Pixel(p.r, p.g, p.b));
			}
		}
//		int index = 0;
//		byte *Ptr = pTex->GetPixels();
//		cDib24::Pixel p;
//		for(int y = 0; y < pTex->GetHeight(); y++) {
//			for(int x = 0; x < pTex->GetWidth(); x++) {
//				m_Image.SetPixel(x, y, cDib24::Pixel(Ptr[0], Ptr[1], Ptr[2]));
//				m_Image.SetPixel(x, pTex->GetHeight() - y - 1, cDib24::Pixel(Ptr[0], Ptr[1], Ptr[2]));
//				Ptr += cImage::Format::ChannelCount(pTex->GetFormat());
//			}
//		}
	}
	// Info:
	FileInfo << "Size: " << m_Image.GetWidth() << " x " << m_Image.GetHeight();
} // ImageFileDialog::OnLoadFileForPreview

//-----------------------------------------------------------------------------
// ImageFileDialog::OnUnloadFileForPreview
//-----------------------------------------------------------------------------
void ImageFileDialog::OnUnloadFileForPreview() {
	m_Image.Free();
} // ImageFileDialog::OnUnloadFileForPreview

//-----------------------------------------------------------------------------
// ImageFileDialog::OnRenderPreview
//-----------------------------------------------------------------------------
void ImageFileDialog::OnRenderPreview(cDib24 &dib, int xShift, int yShift) {
	if(m_Image.GetWidth() == 0 || m_Image.GetHeight() == 0) {
		return;
	}
	const float wFm = (float)m_Image.GetWidth(), hFm = (float)m_Image.GetHeight();
	const float wTo = (float)dib.GetWidth(), hTo = (float)dib.GetHeight();
	const float Scale = cMath::Min(wTo / wFm, hTo / hFm);
	const int dx = (int)((wTo - Scale * wFm) / 2.0f), dy = (int)((hTo - Scale * hFm) / 2.0f);
	const int W = (int)cMath::Max(Scale * wFm, 1.0f), H = (int)cMath::Max(Scale * hFm, 1.0f);

	for(int y = dy; y < H + dy; y++) {
		for(int x = dx; x < W + dx; x++) {
			const float s = (float)(x - dx) / (float)W;
			const float t = (float)(y - dy) / (float)H;
			dib.SetPixel(x, y, m_Image.GetPixel(s, t));
		}
	}

	// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	dib.Str("0, 0", 0, 0);
	dib.Str("100, 100", 100, 100);
	dib.Str("100, 0", 100, 0);
	dib.Str("0, 100", 0, 100);
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

} // ImageFileDialog::OnRenderPreview

//*****************************************************************************
// InputManager
//*****************************************************************************

//-----------------------------------------------------------------------------
// InputManager.ctor
//-----------------------------------------------------------------------------
InputManager::InputManager() : cInput() {
	m_IsInited = false;
} // InputManager.ctor

WNDPROC InputManager::m_OldWndProc = NULL;

//---------------------------------------------------------------------------------------
// InputManager::WndProc
//---------------------------------------------------------------------------------------
LRESULT WINAPI InputManager::WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	if(Msg == WM_MOUSEWHEEL) {
		int Delta = (short)HIWORD(wParam);
		cInput::AddMouseWheelEvent(Delta / WHEEL_DELTA);
		return 0;
	} else if(Msg == WM_SYSCOMMAND && wParam == SC_KEYMENU) { // To avoid sealing on Alt key in window w/ system menu.
		return 0;
	}
	return CallWindowProc(m_OldWndProc, hWnd, Msg, wParam, lParam);
} // InputManager::WndProc

//-----------------------------------------------------------------------------
// InputManager::Free
//-----------------------------------------------------------------------------
void InputManager::Free() {
	cAssertM(m_IsInited == true, "Free() - \"Input Manager\" is not inited");
	if(!m_IsInited) {
		return;
	}
	cLog::Message("cWin32::InputManager::Free(): Releasing Standart Win32 \"Input Manager\"");
	if(m_hWnd) {
		SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_OldWndProc));
		cLog::Message("cWin32::InputManager::Free(): Window is unsubclassed");
	}
	m_IsInited = false;
} // InputManager::Free

//-----------------------------------------------------------------------------
// InputManager::Init
//-----------------------------------------------------------------------------
bool InputManager::Init(const void *hWnd) {
	cAssertM(m_IsInited == false, "Init() - \"Input Manager\" is already inited");
	if(m_IsInited) {
		return false;
	}
	
	cStr Log;
	Log << "------------------------------------------------------------" << cLog::EndLn;
	Log << "              Keyboard and mouse input devices" << cLog::EndLn;
	Log << "------------------------------------------------------------" << cLog::EndLn;
	Log << "Standart Win32 Input Manager" << cLog::EndLn << cLog::EndLn;

	int Codes[] = {
		VK_ESCAPE,		// Esc
		VK_F1,			// F1
		VK_F2,			// F2
		VK_F3,			// F3
		VK_F4,			// F4
		VK_F5,			// F5
		VK_F6,			// F6
		VK_F7,			// F7
		VK_F8,			// F8
		VK_F9,			// F9
		VK_F10,			// F10
		VK_F11,			// F11
		VK_F12,			// F12
		'0',			// Zero, "0"
		'1',			// One, "1"
		'2',			// Two, "2"
		'3',			// Three, "3"
		'4',			// Four, "4"
		'5',			// Five, "5"
		'6',			// Six, "6"
		'7',			// Seven, "7"
		'8',			// Eight, "8"
		'9',			// Nine, "9"
		VK_OEM_MINUS,	// Minus, "-"
		VK_OEM_PLUS,	// Equals, "="
		VK_BACK,		// BackSpace
		VK_TAB,			// Tab
		VK_CAPITAL,		// CapsLock
        VK_SNAPSHOT,	// PrintScreen
		VK_SCROLL,		// ScrollLock
		VK_PAUSE,		// Pause
		VK_INSERT,		// Insert
		VK_DELETE,		// Delete
		VK_HOME,		// Home
		VK_END,			// End
		VK_PRIOR,		// PageUp
		VK_NEXT,		// PageDown
		VK_UP,			// Up
		VK_DOWN,		// Down
		VK_LEFT,		// Left
		VK_RIGHT,		// Right
		VK_OEM_5,		// BackSlash, "\\"
		VK_RETURN,		// Enter
		VK_OEM_4,		// LeftBracket, "["
		VK_OEM_6,		// RightBracket, "]"
		VK_OEM_1,		// SemiColon, ";"
		VK_OEM_7,		// SingleQuote, "\'"
		VK_OEM_COMMA,	// Comma, ","
		VK_OEM_PERIOD,	// Period, "."
		VK_OEM_2,		// Slash, "/"
		VK_LSHIFT,		// LeftShift
		VK_LCONTROL,	// LeftControl
		VK_LMENU,		// LeftAlt
		VK_RSHIFT,		// RightShift
		VK_RCONTROL,	// RightControl
		VK_RMENU,		// RightAlt
		VK_SHIFT,		// Shift
		VK_CONTROL,		// Control
		VK_MENU,		// Alt
		VK_SPACE,		// Space
		VK_OEM_3,		// Tilda, "~"
		'A',			// A
		'B',			// B
		'C',			// C
		'D',			// D
		'E',			// E
		'F',			// F
		'G',			// G
		'H',			// H
		'I',			// I
		'J',			// J
		'K',			// K
		'L',			// L
		'M',			// M
		'N',			// N
		'O',			// O
		'P',			// P
		'Q',			// Q
		'R',			// R
		'S',			// S
		'T',			// T
		'U',			// U
		'V',			// V
		'W',			// W
		'X',			// X
		'Y',			// Y
		'Z'				// Z
	};
	
	ZeroMemory(m_RemapTable, sizeof(m_RemapTable));
	const int KeysCount = sizeof(Codes) / sizeof(Codes[0]);
	Log << cStr::Format("Acquiring %d keyboard keys.", KeysCount) << cLog::EndLn;
	for(int i = 0; i < KeysCount; i++) {
		m_RemapTable[Codes[i]] = i + 1;
	}
	
	const BOOL Mouse = GetSystemMetrics(SM_MOUSEPRESENT);
	const BOOL Wheel = Mouse ? GetSystemMetrics(SM_MOUSEWHEELPRESENT) : FALSE;
	Log << (Mouse ? "Mouse is installed (w/" : "No mouse.");
	if(Mouse) {
		if(!Wheel) {
			Log << "o";
		}
		Log << " wheel). Acquiring 3 buttons.";
	}
	Log << cLog::EndLn;
	
	m_hWnd = IsWindow((HWND)hWnd) ? (HWND)hWnd : NULL;
	if(m_hWnd) { // Subclassing to receive input messages.
		Log << "Window is subclassed to handle input events." << cLog::EndLn;
		m_OldWndProc = (WNDPROC)SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)WndProc);
	}

	Log << cLog::EndLn;
	
	cLog::Message(Log);
	m_IsInited = true;
	return true;
} // InputManager::Init

//-----------------------------------------------------------------------------
// InputManager::AcquireKeyboard
//-----------------------------------------------------------------------------
bool InputManager::AcquireKeyboard(KeyboardState &S) {
	cAssertM(m_IsInited, "AcquireKeyboard() - \"Input Manager\" is not inited");
	if(!m_IsInited) {
		return false;
	}

	// Input only to active window:
	if(GetForegroundWindow() != m_hWnd) {
		return false;
	}

	BYTE State[256];
	if(GetKeyboardState(State)) {
		for(int i = 0; i < 256; i++) {
			if(State[i] & 0x80) {
				S.IsDown[m_RemapTable[i]] = true;
			} else {
				S.IsDown[m_RemapTable[i]] = false;
			}
		}
		return true;
	}
	return false;
} // InputManager::AcquireKeyboard

//-----------------------------------------------------------------------------
// InputManager::AcquireMouse
//-----------------------------------------------------------------------------
bool InputManager::AcquireMouse(MouseState &S) {
	cAssertM(m_IsInited == true, "AcquireMouse() - \"Input Manager\" is not inited");
	
	if(!m_IsInited) {
		return false;
	}

	// Input only to active window:
	if(GetForegroundWindow() != m_hWnd) {
		return false;
	}

	S.Buttons[0] = GetKeyState(VK_LBUTTON) < 0;
	S.Buttons[1] = GetKeyState(VK_RBUTTON) < 0;
	S.Buttons[2] = GetKeyState(VK_MBUTTON) < 0;

	S.Wheel = 0;

	POINT Pos;
	GetCursorPos(&Pos);
	if(m_hWnd != NULL) {
		ScreenToClient(m_hWnd, &Pos);
	}
	S.PosX = Pos.x;
	S.PosY = Pos.y;
	return true;
} // InputManager::AcquireMouse

} // cWin32