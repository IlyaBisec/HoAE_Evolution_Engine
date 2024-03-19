#pragma once

class cLog {
public:
	static const cStr EndLn;
	static void Message(const char *Format, ...);
	static void Warning(const char *Format, ...); // [Ignore] [Exit] (auto exit)
	static void Error(const char *Format, ...); // [Exit] (auto exit)
	static void ErrorNoExit(const char *Format, ...); // [Exit] (no auto exit)
	static void ShowDlg();
	static void HideDlg();
	static void SetIcon(HICON hIcon);
private:
	static cStr m_Log;
	static /*volatile */HWND m_hDlg;
	enum eDlgMode {
		DM_MESSAGE = 0,
		IS_MODAL = 0x1000,
		DM_WARNING = 1 | IS_MODAL, // Auto exit
		DM_ERROR = 2 | IS_MODAL, // Either
		DM_ASSERT = 3 | IS_MODAL // Auto exit
	};
	static /*volatile */eDlgMode m_Mode;
	static /*volatile */bool m_AutoExit;
	static BOOL IsModal() { return m_Mode & IS_MODAL; }
	static /*volatile */int m_idClicked;
	static /*volatile */bool m_ThreadIsCreated;
	static const char *m_DlgTitle;
	static void ThreadFn(PVOID);
	static void Init();
	static void GenDlgTemplate(cWin32::DlgTemplate &Dlg);
	static LRESULT CALLBACK DlgProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam);
	static void AdjustLayout();
	static void UpdateLogEditBox();
	static void EnableControls();
	static void AppendMessage(const char *Format, va_list Args);
	static int ShowModalMessage(const eDlgMode Mode, const bool AutoExit, const char *Format, va_list Args);
	static const int idDebug = 101;
	static const int idIgnore = 102;
	static const int idIgnoreAlways = 103;
	static const int idCopy = 104;
	static const int idExit = 105;
	static const int idLog = 106;
	static const int idStatusBar = 107;
	friend bool cAssertDlg(const char *Exp, const char *Msg, const char *File, int Line, bool &IgnoreAlways);
};