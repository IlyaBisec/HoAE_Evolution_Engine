#include "stdheader.h"
#include "..\comms\comms.h"
#include "Html.h"

namespace HTML {

//-----------------------------------------------------------------------------
// IsActive
//-----------------------------------------------------------------------------
bool IsActive() {
	static const char MainWndClassName[] = "GSCHtmlFullScreenBrowser";
	return FindWindow(MainWndClassName, "");
}

//-----------------------------------------------------------------------------
// Close
//-----------------------------------------------------------------------------
void Close() {
	WinExec("html.exe", SW_SHOWNORMAL);
}

//-----------------------------------------------------------------------------
// ShowURL
//-----------------------------------------------------------------------------
void ShowURL(const char *URL, int x, int y, int w, int h) {
	cStr CmdLine;
	CmdLine << "\"html.exe\" x" << x << " y" << y << " w" << w << " h" << h;
	if(URL) CmdLine << " \"" << URL << "\"";
	WinExec(CmdLine.ToCharPtr(), SW_SHOWNORMAL);

	DWORD dw = GetTickCount();
	const DWORD TimeOut = 10000;
	while(!IsActive() && (GetTickCount() - dw < TimeOut)) {
		Sleep(20);
	}
}

//------------------------------------------------------------------------------
// ShowRelativePath
//------------------------------------------------------------------------------
void ShowRelativePath(const char *RelativePath, int x, int y, int w, int h) {
	assert(RelativePath);
	char Path[MAX_PATH];
	if(GetCurrentDirectory(MAX_PATH, Path)) {
		if(Path[strlen(Path) - 1] != '\\') strcat(Path, "\\");
		strcat(Path, RelativePath);
		ShowURL(Path, x, y, w, h);
	}
}

//-----------------------------------------------------------------------------
// ShowHTML
//-----------------------------------------------------------------------------
void ShowHTML(const cStr &Html, int x, int y, int w, int h) {
	const char FileName[] = "$$$Temp.html";
	//Html.toFile(FileName);
	cAssertM(0, "Fix it! Here we should write to cData and than call cIO::SaveFile!");
	char Buffer[4096];
	Buffer[0] = '\0';
	GetCurrentDirectory(sizeof(Buffer), Buffer);
	strcat(Buffer, FileName);
	ShowURL(Buffer, x, y, w, h);
}

//-----------------------------------------------------------------------------
// HtmlCtrlEnumWindowsProc
//-----------------------------------------------------------------------------
BOOL CALLBACK HtmlCtrlEnumWindowsProc(HWND hWnd, LPARAM lp) {
	DWORD style = GetWindowLong(hWnd, GWL_STYLE);
	if(style & WS_VISIBLE) {
		cStr &s = *(cStr*)lp;
		char ClassName[256];
		GetClassName(hWnd, ClassName, sizeof(ClassName));
		TCHAR Text[1024];
		GetWindowText(hWnd, Text, sizeof(Text) / sizeof(Text[0]));
		char Buffer[256];
		sprintf(Buffer, "\n<tr><td>%p&nbsp;</td><td>%s</td><td>%s</td></tr>", hWnd, ClassName, Text);
		s << Buffer;
	}
	return TRUE;
}

//-----------------------------------------------------------------------------
// Test
//-----------------------------------------------------------------------------
void Test() {
	cStr html;
	html.Free();
	html << "<html>\n<head>\n<title>Windows List</title>\n</head>\n<body bgcolor = ";
	html << cColor::Gray75.ToWebColor() << ">\n<p align = right>[<a href = \"cmd:close\">Close</a>]<hr>";
	html << "\n<table border=1 width = 100% bgcolor = " << cColor::Pink.ToWebColor() << "><tr><td><font face=arial size=+3>";
	html <<"<i>Windows List</i></font></table>";
	html << "\n<table width=100% border = 1>\n<tr><td align = center><i><b>hWnd</b></i></td><td align = center><i><b>Class</b></i></td><td align = center><i><b>Title</b></i></td></tr>";
	EnumWindows(HtmlCtrlEnumWindowsProc, (LPARAM)&html);
	html << "\n</table>";
	html << "\n</body>\n</html>";
	ShowHTML(html);
}

} // Html