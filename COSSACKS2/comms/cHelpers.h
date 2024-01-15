#pragma once

class cHelpers {
public:
	static void OnSizing(HWND hWnd, WPARAM wParam, LPARAM lParam, int MinClientLx = 320, int MinClientLy = 200);
	static BOOL CenterWindow(HWND hwndChild, HWND hwndParent);

	struct ExecArgs {
		const char *pVerb; 	// "edit", "explore", "find", "open", "print", "properties"
		const char *pFileName;
		const char *pParameters;
		const char *pDirectory;
		int nShow;
		bool fWait;
		DWORD msTimeOut;

		ExecArgs(const char *pVerb, const char *pFileName, const char *pParameters,
			const char *pDirectory, int nShow, bool fWait, DWORD msTimeOut = 1000) {
				this->pVerb = pVerb;
				this->pFileName = pFileName;
				this->pParameters = pParameters;
				this->pDirectory = pDirectory;
				this->nShow = nShow;
				this->fWait = fWait;
				this->msTimeOut = msTimeOut;
			}
	};
	static int Exec(const ExecArgs &Args);

	typedef struct {
		cStr Path;
		cStr Name;
	} FileInfo;
	static bool SearchFiles(cList<FileInfo> &Files, const cStr &Folder, const cStr &Extension = "*");
};