#include "ColorizeExt.h"

const DWORD ceInitColor = comms::argbYellow;

// ColorizeExt::ColorizeOneVertex
void ColorizeExt::ColorizeOneVertex(const OneMapVertex &Center, int Radius, DWORD argbColor, OneMapVertex &vtx) {
	//           | rFadeOff |
	// ----------------------
	// |        Radius      |
	// |____________________|
	if(Radius < 1) return;
	int rFadeOff = Radius / ExtOptions::rFadeOffPortion;

	int r_d = Radius - rFadeOff;

	float t, fR, fG, fB;
	int cR, cG, cB, cr, cg, cb;
	const float cK = 1.0f;
	DWORD clr;
	int l = sqrt(Square(Center.x - vtx.x) + Square(Center.y - vtx.y) + Square(Center.z - vtx.z));
	if(l < Radius) {
		if(l <= r_d) {
			vtx.Color = argbColor; // w/o gradient
		} else {
			t = float(l - r_d) / (cK * float(rFadeOff));
			if(t > cK) t = cK;
			argb2rgb(vtx.Color, cR, cG, cB);
			argb2rgb(argbColor, cr, cg, cb);
			fR = float(cr) + float(cR - cr) * t;
			fG = float(cg) + float(cG - cg) * t;
			fB = float(cb) + float(cB - cb) * t;
			rgb2argb((int)fR, (int)fG, (int)fB, vtx.Color);
		}
	}
} // ColorizeExt::ColorizeOneVertex

DWORD ceCurColor = ceInitColor;
// ColorizeExt::OnLMB
DWORD ColorizeExt::OnLMB(SelExtParams &Params) {
	if(!Params.VertsTo.Count()) return 0;
	OneMapVertex &c = Params.RefTo.pOrigin->Vert[Params.VertsTo[0]];
	c.Color = ceCurColor;
	for(int i = 1; i < Params.VertsTo.Count(); i++) {
		OneMapVertex &r = Params.RefTo.pOrigin->Vert[Params.VertsTo[i]];
		ColorizeOneVertex(c, Params.Radius, ceCurColor, r);
	}
	return SelExt::IF_CACHE;
} // ColorizeExt::OnLMB

int ceCursorX = 0, ceCursorY = 0;
// ColorizeExt::CCHookProc
UINT_PTR CALLBACK ColorizeExt::CCHookProc(HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam) {
	const int idTimer = 1;
	RECT rc;
	extern HWND g_hWnd;
	int Lx, Ly, dx, dy, x, y;
	if(uiMsg == WM_INITDIALOG) {
		// g_hWnd:
		GetWindowRect(g_hWnd, &rc);
		Lx = rc.right - rc.left;
		Ly = rc.bottom - rc.top;
		// hDlg:
		GetWindowRect(hDlg, &rc);
		dx = rc.right - rc.left, dy = rc.bottom - rc.top;
		x = ceCursorX - dx / 2, y = ceCursorY - dy / 2;
		// Posing:
		comms::Clamp(x, 0, Lx - dx);
		comms::Clamp(y, 0, Ly - dy);
		SetWindowPos(hDlg, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		SetTimer(hDlg, idTimer, 10, NULL);
		return TRUE;
	}
	if(uiMsg == WM_TIMER) {
		if(wParam == idTimer) {
			bool fPressed = GetKeyState(VK_SPACE) < 0;
			if(!fPressed) {
				KillTimer(hDlg, idTimer);
				PostMessage(hDlg, WM_COMMAND, MAKEWPARAM(IDOK, 0), 0);
			}
		}
	}
	if(uiMsg == WM_KEYDOWN) {
  		if(wParam == VK_SPACE) {
			return TRUE;
		}
	}
	if(uiMsg == WM_KEYUP) {
		if(wParam == VK_SPACE) {
			return TRUE;
		}
	}
	if(uiMsg == WM_CHAR) {
		if(wParam == ' ') {
			return TRUE;
		}
	}
	return FALSE;
} // ColorizeExt::CCHookProc

// ColorizeExt::ShowChooseColorDialog
void ColorizeExt::ShowChooseColorDialog(int xCursor, int yCursor, DWORD &argbColor) {
	CHOOSECOLOR cc;
	static COLORREF acrCustClr[16];
	static DWORD rgbCur = argb2rgb(ceInitColor);

	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	extern HWND g_hWnd;
	cc.hwndOwner = g_hWnd;
	cc.lpCustColors = (LPDWORD)acrCustClr;
	cc.rgbResult = rgbCur;
	cc.lpfnHook = CCHookProc;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT | CC_ENABLEHOOK;
	ceCursorX = xCursor, ceCursorY = yCursor;
	if(ChooseColor(&cc)) {
		rgbCur = cc.rgbResult;
		rgb2argb(rgbCur, argbColor);
	}
} // ColorizeExt::ShowChooseColorDialog

// ColorizeExt::OnRMB
DWORD ColorizeExt::OnRMB(SelExtParams &Params) {
	if(!Params.RefTo.m_TriNums.NValues) return 0;
	Params.RefTo.SmoothColors();
	return SelExt::IF_CACHE;
} // ColorizeExt::OnRMB

// ColorizeExt::OnDraw
void ColorizeExt::OnDraw(SelExtParams &Params) {
	// Choose Color:
	static bool fWasPressed = false;
	bool fIsPressed = GetKeyState(VK_SPACE) < 0;
	if(!fIsPressed) fWasPressed = false;
	if(fIsPressed && !fWasPressed) {
		POINT pt;
		GetCursorPos(&pt);
		ShowChooseColorDialog(pt.x, pt.y, ceCurColor);
		fWasPressed = true;
	}
	// OnDraw:
	if(!Params.VertsTo.Count()) return;
	OneSplitMesh Splitted;
	if(!Params.RefTo.m_TriNums.NValues) {
		VertexBasedMesh Marker;
		OneMapVertex c = Params.RefTo.pOrigin->Vert[Params.VertsTo[0]];
		c.Color = ceCurColor;
		Marker.GenMarker(c);
		VertexBasedMeshRef Ref;
		Marker.GetRefAll(Ref);
		Splitted.Split(Ref, 3);
		Splitted.Draw();
	} else {
		DrawGrid(Params.RefTo, ceCurColor);
	}
} // ColorizeExt::OnDraw

// ColorizeExt::OnDrawInterface
void ColorizeExt::OnDrawInterface(SelExtParams &Params) {
	int x0 = RealLx - ExtOptions::IconLxL - ExtOptions::IconRightOffset;
	rsRect(Rct(x0, ExtOptions::IconTopOffset, ExtOptions::IconLxL, ExtOptions::IconLxL), 0.f, ceCurColor);
	SelExt::DrawWindow(x0 - ExtOptions::dBorder, ExtOptions::IconTopOffset - ExtOptions::dBorder,
		x0 + ExtOptions::IconLxL + ExtOptions::dBorder, ExtOptions::IconTopOffset + ExtOptions::IconLxL + ExtOptions::dBorder,
		comms::argbBlack, ExtOptions::dBorder);
} // ColorizeExt::OnDrawInterface