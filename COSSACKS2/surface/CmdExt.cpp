#include "stdheader.h"
#include "CmdExt.h"

void GSYSDRAW();

namespace Surface {

//-----------------------------------------------------------------------------
// CmdExt::ISliderArgs::GetValue : int() const
//-----------------------------------------------------------------------------
int CmdExt::ISliderArgs::GetValue() const {
	float f = cMath::Lerp(MinValue, MaxValue, CurPos);
	float s = cMath::Sign(f);
	return int(f + s * 0.5f);
}

//-----------------------------------------------------------------------------
// CmdExt::ISliderArgs::SetValue : void(int)
//-----------------------------------------------------------------------------
void CmdExt::ISliderArgs::SetValue(int n) {
	if(n == cMath::IntMaxValue || n == cMath::IntMinValue) {
		n = DefValue;
	}
	if(n >= MinValue && n <= MaxValue) {
		CurPos = float(n - MinValue) / float(MaxValue - MinValue);
	}
}

//-----------------------------------------------------------------------------
// CmdExt::DrawSlider : void(const ISliderArgs &)
//-----------------------------------------------------------------------------
void CmdExt::DrawSlider(const ISliderArgs &Args) {
	int p = Args.x0 + int(Args.CurPos * Args.Lx);
	int c = Args.Ly / 2, R = Args.Ly / 2;
	cColor CurClr = cColor::Lerp(Args.Color0, Args.Color1, Args.CurPos);
	for(int h = 0; h < Args.Ly; h++) {
		float k = 1.0f - cMath::Square(float(h - c)) / float(R * R);
		cColor Clr = CurClr * k;
		Clr.a = 1.0f;
		GPS.DrawLine(Args.x0, Args.y0 + h, p, Args.y0 + h, Clr.ToBgra());
	}
	char Str[32];
	int hStr = GetRLCHeight(Args.pFont->RLC, 'W');
	// MinValue:
	if(!Args.fPercentage) {
		sprintf(Str, "%d", Args.MinValue);
		int wStr = GetRLCStrWidth(Str, Args.pFont);
		int yStr = Args.y0 + int(float(Args.Ly) / 2.0f - float(hStr) / 2.0f + 0.5f);
		ShowString(Args.x0 + 5, yStr, Str, Args.pFont);

		// CentralValue:
		sprintf(Str, "%d", Args.GetValue());
		wStr = GetRLCStrWidth(Str, Args.pFont);
		int cx = Args.x0 + Args.Lx / 2;
		int cy = Args.y0 + Args.Ly / 2;
		if(strlen(Str)) {
			ShowString(cx - wStr / 2, yStr, Str, Args.pFont);
		}

		// MaxValue:
		sprintf(Str, "%d", Args.MaxValue);
		wStr = GetRLCStrWidth(Str, Args.pFont);
		if(strlen(Str)) {
			ShowString(Args.x0 + Args.Lx - wStr - 5, yStr, Str, Args.pFont);
		}
	} else {
		sprintf(Str, "%d", int(Args.CurPos * 100.0f + 0.5f));
		strcat(Str, "%");
		int wStr = GetRLCStrWidth(Str, Args.pFont);
		int cx = Args.x0 + Args.Lx / 2;
		int cy = Args.y0 + Args.Ly / 2;
		ShowString(cx - wStr / 2, cy - hStr / 2, Str, Args.pFont);
	}
}

//-----------------------------------------------------------------------------
// .ctor
//-----------------------------------------------------------------------------
CmdExt::CmdExt(CallBack fn) {
	assert(fn);
	m_pCallBack = fn;
	m_fActive = false;
}

//-----------------------------------------------------------------------------
// CheckActivityOfEditor
//-----------------------------------------------------------------------------
bool CmdExt::CheckActivityOfEditor() { return m_fActive; }

//-----------------------------------------------------------------------------
// ClearActivityOfEditor
//-----------------------------------------------------------------------------
void CmdExt::ClearActivityOfEditor() { m_fActive = false; }

//-----------------------------------------------------------------------------
// ActivateEditor
//-----------------------------------------------------------------------------
void CmdExt::ActivateEditor() {
	if(m_fActive) return;
	m_fActive = true;
	m_Stage = S_INIT;
}

//-----------------------------------------------------------------------------
// MessageBox : void(const cStr &, const MessageBoxArgs &)
//-----------------------------------------------------------------------------
void CmdExt::MessageBox(cStr &Msg, const MessageBoxArgs &Args) {
	if(!Msg.Length()) return;
	bool fSlider = Args.SliderPos >= 0;
	static RLCFont *pFont = &BigWhiteFont;
	int wStr = GetRLCStrWidth(Msg.ToCharPtr(), pFont);
	int hStr = GetRLCHeight(pFont->RLC, 'W');
	int hTotal = hStr;
	if(fSlider) {
		hTotal += CmdExt::dStrSlider + CmdExt::hSlider;
	}
	int cx = 0, cy = 0;
	if(A_CENTER == Args.Align) {
		cx = RealLx / 2;
		cy = RealLy / 2;
	} else if(A_CORNER == Args.Align) {
		cx = CmdExt::dCornerOffset + wStr / 2;
		cy = RealLy - CmdExt::dCornerOffset - hTotal / 2;
	} else if(A_WORLD == Args.Align) {
		Vector3D Pr(Args.World.x, Args.World.y, Args.World.z);
		WorldToClosestScreenPoint(Pr);
		cx = (int)Pr.x;
		cy = (int)Pr.y;
	} else if(A_SCREEN == Args.Align) {
		cx = (int)Args.Screen.x;
		cy = (int)Args.Screen.y;
	} else {
		assert(0);
	}
	int x0 = cx - wStr / 2;
	int y0 = cy - hTotal / 2;
	// Drawing:
	int X0 = x0 - 2 * CmdExt::dBorder;
	int Y0 = y0 - 2 * CmdExt::dBorder;
	int W = wStr + 4 * CmdExt::dBorder;
	int H = hTotal + 4 * CmdExt::dBorder;

	// Clamping:
	if(A_WORLD == Args.Align || A_SCREEN == Args.Align) {
		const Rct &rc = ICam->GetViewport();
		int px(X0), py(Y0);
		X0 = cMath::Clamp(X0, 0, (int)rc.w - W);
		Y0 = cMath::Clamp(Y0, 0, (int)rc.h - H);
		x0 -= px - X0;
		y0 -= py - Y0;
	}

	rsRect(Rct(X0, Y0, W, H), 0.0f, cColor::White.ToBgra());
	rsRect(Rct(X0 + CmdExt::dBorder, Y0 + CmdExt::dBorder,
		W - 2 * CmdExt::dBorder, H - 2 * CmdExt::dBorder), 0.0f, Args.FaceColor);
	ShowString(x0, y0, Msg.ToCharPtr(), pFont);
	if(fSlider) {
		ISliderArgs isa;
		isa.MinValue = 0;
		isa.MaxValue = Args.SliderLen;
		isa.SetValue(Args.SliderPos);
		isa.x0 = x0;
		isa.y0 = y0 + hStr + CmdExt::dStrSlider;
		isa.Lx = wStr;
		isa.Ly = CmdExt::hSlider;
		isa.pFont = &BigWhiteFont;
		isa.fPercentage = true;
		isa.Color0 = isa.Color1 = cColor::Yellow;
		DrawSlider(isa);
	}
}

//-----------------------------------------------------------------------------
// CmdExt::DrawMessageBox : void(cStr &, const MessageBoxArgs &)
//-----------------------------------------------------------------------------
void CmdExt::DrawMessageBox(cStr &Msg, const MessageBoxArgs &Args) {
	MessageBox(Msg, Args);
	GSYSDRAW();
}

//-----------------------------------------------------------------------------
// DrawEditorInterface
//-----------------------------------------------------------------------------
void CmdExt::DrawEditorInterface() {
	if(!m_fActive) return;

	static DWORD tStart, tStatsStart;
	static cStr Stats;
	cStr Msg;
	switch(m_Stage) {
		case S_INIT:
			Msg.Free();
			if(!m_pCallBack(Msg)) {
				m_fActive = false;
				return;
			}
			tStart = GetTickCount();
			m_Stage = S_PROCESS;
			MessageBox(Msg);
			break;
		case S_PROCESS:
			Msg.Free();
			if(!m_pCallBack(Msg)) {
				tStatsStart = GetTickCount();
				float T;
				T = float(tStatsStart - tStart) / 1000.0f;
				char Buffer[128];
				sprintf(Buffer, "%.2f", T);
				Stats = Msg + " in " + Buffer + " (s).";
				ResetGroundCache();
				m_Stage = S_STATS;
			} else {
				MessageBox(Msg);
			}
			break;
		case S_STATS:
			if(GetTickCount() - tStatsStart < 1000 * CmdExt::tShowStats) {
				ShowString(10, RealLy - 40, Stats.ToCharPtr(), &BigRedFont);
			} else m_fActive = false;
			break;
		default:
			assert(0);
	}
}

} // Surface