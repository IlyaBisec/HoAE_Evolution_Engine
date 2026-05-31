#ifndef __ColorizeExt__
#define __ColorizeExt__

#include "SelExt.h"

class ColorizeExt : public SelExt {
public:
	DWORD OnLMB(SelExtParams &Params);
	DWORD OnRMB(SelExtParams &Params);
	void OnDraw(SelExtParams &Params);
	void OnDrawInterface(SelExtParams &Params);

	static void ColorizeOneVertex(const OneMapVertex &Center, int Radius, DWORD argbColor, OneMapVertex &vtx);
	static UINT_PTR CALLBACK CCHookProc(HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);
	static void ShowChooseColorDialog(int xCursor, int yCursor, DWORD &argbColor);
};

#endif // __ColorizeExt__