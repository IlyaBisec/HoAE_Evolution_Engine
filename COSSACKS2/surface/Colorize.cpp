#include "stdheader.h"
#include "Colorize.h"

namespace Surface {

DWORD ceCurColor = cColor::Gray50.ToBgra();

//------------------------------------------------------------------------------------------------------------
// ColorizeOneVertex
//------------------------------------------------------------------------------------------------------------
void Colorize::ColorizeOneVertex(const SurfVertex &Center, int Radius, DWORD argbColor, SurfVertex &vtx) {
	//           | rFadeOff |
	// ----------------------
	// |        Radius      |
	// |____________________|
	if(Radius < 1) return;
	int rFadeOff = Radius / Colorize::rFadeOffPortion;

	int r_d = Radius - rFadeOff;

	float t, fR, fG, fB;
	int cR, cG, cB, cr, cg, cb;
	const float cK = 1.0f;
	DWORD clr;
	int Z = ST_INCYLINDER == g_SelType ? 0 : cMath::Square(Center.z - vtx.z);
	int l = sqrt(cMath::Square(Center.x - vtx.x) + cMath::Square(Center.y - vtx.y) + Z);
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
}

//-----------------------------------------------------------------------------------------
// Process : void(SurfRef &Ref, DWORD, const SurfVertex &, int)
//-----------------------------------------------------------------------------------------
void Colorize::Process(SurfRef &Ref, DWORD Color, const SurfVertex &Centre, int Radius) {
	assert(Ref.pOrig);
	eMappingType MappingType = SelExt::QueryMappingTypeKeys();
	bool fOnlyAlongFacture = GetKeyState(VK_CONTROL) < 0 && GetKeyState(VK_SHIFT) < 0;
	if(fOnlyAlongFacture) {
		MappingType = (eMappingType)0;
	}
	for(int i = 0; i < Ref.idVerts.Count(); i++) {
		SurfVertex &r = Ref.pOrig->VERTS[Ref.idVerts[i]];
		if(Centre.SurfaceType == r.SurfaceType && ((MappingType & r.MappingType()) || (fOnlyAlongFacture && r.nFacture == Centre.nFacture))) {
			ColorizeOneVertex(Centre, Radius, Color, r);
		}
	}
}

//-----------------------------------------------------------------------------
// OnLMB
//-----------------------------------------------------------------------------
DWORD Colorize::OnLMB(SelExtArgs &Args) {
	SurfaceUndo::DropRefToUndo(Args.Ref);
	Process(Args.Ref, ceCurColor, Args.Center, Args.Radius);
	return SelExt::IF_CACHE;
}
void Colorize::OnActivate(){
    m_Args.LimitInSurfaceType = true;
}
int ceCursorX = 0, ceCursorY = 0;
//-------------------------------------------------------------------------------------------
// CCHookProc
//-------------------------------------------------------------------------------------------
UINT_PTR CALLBACK Colorize::CCHookProc(HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam) {
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
		cMath::Clamp(x, 0, Lx - dx);
		cMath::Clamp(y, 0, Ly - dy);
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
}

//--------------------------------------------------------------------------------
// ShowChooseColorDialog
//--------------------------------------------------------------------------------
void Colorize::ShowChooseColorDialog(int xCursor, int yCursor, DWORD &argbColor) {
	CHOOSECOLOR cc;
	static COLORREF acrCustClr[16];
	DWORD rgbCur = argb2rgb(argbColor);

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
}

//-----------------------------------------------------------------------------
// OnRMB
//-----------------------------------------------------------------------------
DWORD Colorize::OnRMB(SelExtArgs &Args) {
	bool fGetColor = GetKeyState(VK_CONTROL) < 0;
	if(fGetColor) {
		ceCurColor = Args.Center.Color;
		return 0;
	}
	SurfaceUndo::DropRefToUndo(Args.Ref);
	Args.Ref.SmoothColors();
	return SelExt::IF_CACHE;
}

//-----------------------------------------------------------------------------
// OnDraw
//-----------------------------------------------------------------------------
void Colorize::OnDraw(SelExtArgs &Args) {
	SurfRef sr(Args.Ref);
	SurfMesh sm;
	sr.ToSurfMesh(sm);
	sm.GetRefAll(sr, SR_VERTS);
	extern bool Rpressed;
	if(!Rpressed) Process(sr, ceCurColor, Args.Center, Args.Radius);
	SelExt::DrawSwatch(sm);
}

//-----------------------------------------------------------------------------
// OnDrawInterface
//-----------------------------------------------------------------------------
void Colorize::OnDrawInterface() {
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
	// Color Sample:
	int x0 = RealLx - SelExt::IconLxL - SelExt::IconRightOffset;
	rsRect(Rct(x0, SelExt::IconTopOffset, SelExt::IconLxL, SelExt::IconLxL), 0.f, ceCurColor);
	SelExt::DrawWindow(x0 - SelExt::dBorder, SelExt::IconTopOffset - SelExt::dBorder,
		x0 + SelExt::IconLxL + SelExt::dBorder, SelExt::IconTopOffset + SelExt::IconLxL + SelExt::dBorder,
		cColor::Black.ToBgra(), SelExt::dBorder);
}

} // Surface