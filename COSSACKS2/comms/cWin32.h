#pragma once

namespace cWin32 {
	bool CopyToClipboard(const char *Text, HWND hWnd = NULL);
	void CopyFromClipboard(cStr &Text, HWND hWnd = NULL);
	void OnSizing(HWND hWnd, WPARAM wParam, LPARAM lParam, int MinClientLx = 320, int MinClientLy = 200);
	BOOL CenterWindow(HWND hwndChild, HWND hwndParent);
	
	struct ExecArgs {
		const char *pVerb; 	// "edit", "explore", "find", "open", "print", "properties"
		const char *pFileName;
		const char *pParameters;
		const char *pDirectory;
		int nShow;
		bool fWait;
		DWORD TimeOutMs;
	};
	int Exec(const ExecArgs &Args);
	
	typedef struct {
		cStr Path;
		cStr Name;
	} FileInfo;
	bool SearchFiles(cList<FileInfo> &Files, const cStr &Folder, const cStr &Extension = "*");
	
	//*****************************************************************************
	// DlgTemplate
	//*****************************************************************************
	class DlgTemplate {
	public:
		DlgTemplate() {}
		DlgTemplate(LPCSTR Title, DWORD Style, DWORD ExStyle, int xPos, int yPos, int Width, int Height, LPCSTR FontName = NULL, WORD FontSize = 8) {
			Create(Title, Style, ExStyle, xPos, yPos, Width, Height, FontName, FontSize);
		}
		void Create(LPCSTR Title, DWORD Style, DWORD ExStyle, int xPos, int yPos, int Width, int Height, LPCSTR FontName = NULL, WORD FontSize = 8);
		void Free() { m_DlgTemplate.Free(); }
		void AddControl(LPCSTR ClassName, LPCSTR Caption, DWORD Style, DWORD ExStyle, int xPos, int yPos, int Width, int Height, WORD id);
		
		// Helpers:
		void AddButton(LPCSTR Caption, DWORD Style, DWORD ExStyle, int xPos, int yPos, int Width, int Height, WORD id) {
			AddControl("Button" /* 0x0080 */, Caption, Style, ExStyle, xPos, yPos, Width, Height, id);
		}
		void AddEditBox(LPCSTR Caption, DWORD Style, DWORD ExStyle, int xPos, int yPos, int Width, int Height, WORD id) {
			AddControl("Edit" /* 0x0081 */, Caption, Style, ExStyle, xPos, yPos, Width, Height, id);
		}
		void AddStatic(LPCSTR Caption, DWORD Style, DWORD ExStyle, int xPos, int yPos, int Width, int Height, WORD id) {
			AddControl("Static" /* 0x0082 */, Caption, Style, ExStyle, xPos, yPos, Width, Height, id);
		}
		void AddListBox(LPCSTR Caption, DWORD Style, DWORD ExStyle, int xPos, int yPos, int Width, int Height, WORD id) {
			AddControl("ListBox" /* 0x0083 */, Caption, Style, ExStyle, xPos, yPos, Width, Height, id);
		}
		void AddScrollBar(LPCSTR Caption, DWORD Style, DWORD ExStyle, int xPos, int yPos, int Width, int Height, WORD id) {
			AddControl("ScrollBar" /* 0x0084 */, Caption, Style, ExStyle, xPos, yPos, Width, Height, id);
		}
		void AddComboBox(LPCSTR Caption, DWORD Style, DWORD ExStyle, int xPos, int yPos, int Width, int Height, WORD id) {
			AddControl("ComboBox" /* 0x0085 */, Caption, Style, ExStyle, xPos, yPos, Width, Height, id);
		}
		
		const DLGTEMPLATE * ToDlgTemplatePtr() const { return reinterpret_cast<const DLGTEMPLATE *>(m_DlgTemplate.ToPtr()); }
	private:
		cList<char> m_DlgTemplate;
		void AppendData(const void *Src, int Size);
		void AppendStr(LPCSTR Str);
		void AlignDlgTemplateToDWORD();
	}; // DlgTemplate

	//*****************************************************************************
	// FileDialog
	//*****************************************************************************
	class FileDialog {
	public:
		FileDialog(HWND hParentWnd = NULL, const char *InitialPath = NULL, const char *Title = NULL, const char *Filter = NULL);
		void SetParent(HWND hParent);
		
		// Relative path must start from '/' or '\\'.
		// If NULL, exe - file folder will be used.
		void SetInitialPath(const char *InitialPath = NULL);
					
		const cStr & GetInitialPath() const;
		BOOL DoModal();
		const char * GetFilePathName() { return m_FilePathName; }
	protected:
		virtual void OnLoadFileForPreview(const char *FileName, cStr &FileInfo) = 0;
		virtual void OnUnloadFileForPreview() = 0;
		virtual void OnRenderPreview(cDib24 &dib, int xShift, int yShift) = 0;
	private:
		struct {
			DWORD			lStructSize;
			HWND			hwndOwner;
			HINSTANCE		hInstance;
			LPCTSTR			lpstrFilter;
			LPTSTR			lpstrCustomFilter;
			DWORD			nMaxCustFilter;
			DWORD			nFilterIndex;
			LPTSTR			lpstrFile;
			DWORD			nMaxFile;
			LPTSTR			lpstrFileTitle;
			DWORD			nMaxFileTitle;
			LPCTSTR			lpstrInitialDir;
			LPCTSTR			lpstrTitle;
			DWORD			Flags;
			WORD			nFileOffset;
			WORD			nFileExtension;
			LPCTSTR			lpstrDefExt;
			LPARAM			lCustData;
			LPOFNHOOKPROC	lpfnHook;
			LPCTSTR			lpTemplateName;
			// _WIN32_WINNT >= 0x0500
			void			*pvReserved;
			DWORD			dwReserved;
			DWORD			FlagsEx;
		} m_ofn;
		cStr m_FilePathName, m_InitialDir;
		char m_Buffer[MAX_PATH];
		cWin32::DlgTemplate m_PreviewDlgTemplate;
		HWND m_hDlg, m_hViewPort, m_hTitle, m_hFileNameCombo, m_hInfo;
		cDib24 m_dib;
		
		static const UINT WM_SETOWNER = WM_USER + 1;
		static const UINT WM_SETPREVDLGPROC = WM_USER + 2;
		static UINT_PTR CALLBACK HookProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam);
		static BOOL CALLBACK FindCtrls(HWND hWnd, LPARAM lParam);
		static bool FileExist(const char *FileName);
		static void DrawNoPreview(cDib24 &dib);
		void UpdatePreview();
		static UINT_PTR CALLBACK ParentDlgProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam);
		
		static const int idViewPort = 1010;
		static const int idTitle = 1011;
		static const int idInfo = 1012;
	};
	
	//*****************************************************************************
	// MeshFileDialog
	//*****************************************************************************
	class MeshFileDialog : public FileDialog {
	public:
		MeshFileDialog(const HWND hParentWnd = NULL, const char *InitialPath = NULL, const char *Title = "Load Mesh File");
	protected:
		virtual void OnLoadFileForPreview(const char *FileName, cStr &FileInfo);
		virtual void OnUnloadFileForPreview();
		virtual void OnRenderPreview(cDib24 &dib, int xShift, int yShift);
		cRawMesh *m_pMesh;
		cList<float> m_ZBuffer;
	};

	//*************************************************************************
	// ImageFileDialog
	//*************************************************************************
	class ImageFileDialog : public FileDialog {
	public:
		ImageFileDialog(const HWND hParentWnd = NULL, const char *InitialPath = NULL, const char *Title = "Load Image File");
	protected:
		virtual void OnLoadFileForPreview(const char *FileName, cStr &FileInfo);
		virtual void OnUnloadFileForPreview();
		virtual void OnRenderPreview(cDib24 &dib, int xShift, int yShift);
		cDib24 m_Image;
	};

	//*************************************************************************
	// InputManager
	//*************************************************************************
	class InputManager : public cInput {
	public:
		InputManager();
		
		bool Init(const void *hWnd);
		void Free();
		
		bool AcquireKeyboard(KeyboardState &State);
		bool AcquireMouse(MouseState &);
	
	private:
		bool m_IsInited;
		int m_RemapTable[256];
		HWND m_hWnd;
		static WNDPROC m_OldWndProc;
		static LRESULT WINAPI WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	};
} // cWin32