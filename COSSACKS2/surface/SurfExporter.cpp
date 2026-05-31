#include "stdheader.h"
#include "SurfExporter.h"

namespace Surface {

UINT_PTR CALLBACK OFNHookProc(HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);

//-----------------------------------------------------------------------------
// SurfExporter::Export : void(const SurfMesh &)
//-----------------------------------------------------------------------------
void SurfExporter::Export(const SurfMesh &Mesh) {
	// Cur Dir:
	char szCurDir[4096];
	szCurDir[0] = '\0';
	GetCurrentDirectory(4096, szCurDir);
	// ------------------------------------------------

	// Initial Dir:
	char strInitialDir[MAX_PATH];
	GetModuleFileName(NULL, strInitialDir, MAX_PATH);
	int i = 0, nLastBackSlash = 0;
	while(strInitialDir[i]) {
		if(strInitialDir[i] == '\\') {
			nLastBackSlash = i;
		}
		i++;
	}
	strInitialDir[nLastBackSlash + 1] = '\0';
	strcat(strInitialDir, "Meshes");

	// Open File Dialog:
	OPENFILENAME ofn;
	char szFile[260];

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	extern HWND g_hWnd;
	ofn.hwndOwner = g_hWnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Wavefront OBJ Files (*.obj)\0*.obj\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = strInitialDir;
	ofn.lpstrTitle = "Export Surface:";
	ofn.lpfnHook = OFNHookProc;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ENABLEHOOK | OFN_EXPLORER;

	bool f = GetSaveFileName(&ofn);
	// Restoring Dir:
	SetCurrentDirectory(szCurDir);
	CheckExt3(ofn.lpstrFile, "obj");
	if(f) {
		// Exporting:
		cRawMesh rm;
		Mesh.ToRawMesh(rm);
		cStr fn(ofn.lpstrFile);
		fn.SetFileDefaultExtension("obj");
		cIO::SaveMesh(rm, fn);
	}
}

//-----------------------------------------------------------------------------
// SurfExporter::ActivateEditor
//-----------------------------------------------------------------------------
void SurfExporter::ActivateEditor() {
	Export(g_UniversalMap);
}

//-----------------------------------------------------------------------------
// SurfExporter::CheckExt3
//-----------------------------------------------------------------------------
void SurfExporter::CheckExt3(char *pStr, const char *pExt) {
	strupr(pStr);
	int l = strlen(pStr);
	assert(strlen(pExt) == 3);
	if(strlen(pExt) > 3) return;
	char Ext[4];
	strcpy(Ext, pExt);
	strupr(Ext);
	if(l < 4) {
		strcat(pStr, ".");
		strcat(pStr, Ext);
	} else {
		if(pStr[l - 1] != Ext[2] || pStr[l - 2] != Ext[1] ||
			pStr[l - 3] != Ext[0] || pStr[l - 4] != '.') {
				strcat(pStr, ".");
				strcat(pStr, Ext);
			}
	}
}

} // Surface