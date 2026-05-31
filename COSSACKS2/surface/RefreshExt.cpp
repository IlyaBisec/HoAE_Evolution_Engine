#include "RefreshExt.h"
#include "ExtOptions.h"

//-----------------------------------------------------------------------------
// .ctor : void()
//-----------------------------------------------------------------------------
RefreshExt::RefreshExt() {
	m_fActive = false;
	m_fIlluminate = false;
	m_pUniversalMap = &g_UniversalMap;
}

//-----------------------------------------------------------------------------
// .ctor : void(eSSIllumination)
//-----------------------------------------------------------------------------
RefreshExt::RefreshExt(eSSIllumination Illumination) {
	m_fActive = false;
	m_Illumination = Illumination;
	m_fIlluminate = true;
	m_pUniversalMap = &g_UniversalMap;
}

//-----------------------------------------------------------------------------
// RefreshExt::CheckActivityOfEditor : bool()
//-----------------------------------------------------------------------------
bool RefreshExt::CheckActivityOfEditor() {
	return m_fActive;
}

//-----------------------------------------------------------------------------
// RefreshExt::ClearActivityOfEditor : void()
//-----------------------------------------------------------------------------
void RefreshExt::ClearActivityOfEditor() {
	m_fActive = false;
}

//-----------------------------------------------------------------------------
// RefreshExt::ActivateEditor : void()
//-----------------------------------------------------------------------------
void RefreshExt::ActivateEditor() {
	if(m_fActive) return;
	m_fActive = true;
	m_Stage = SLabel;
}

//-----------------------------------------------------------------------------
// RefreshExt::DrawEditorInterface : void()
//-----------------------------------------------------------------------------
void RefreshExt::DrawEditorInterface() {
	char Label[] = "Refreshing...";
	static RLCFont *pFont = &BigWhiteFont;

	if(!m_fActive) return;
	switch(m_Stage) {
		case SLabel:
			int w, h, cx, cy, x0, y0;
			w = GetRLCStrWidth(Label, pFont);
			h = GetRLCHeight(pFont->RLC, 'W');
			cx = RealLx / 2;
			cy = RealLy / 2;
			x0 = cx - w / 2;
			y0 = cy - h / 2;
			rsRect(Rct(x0 - 2 * ExtOptions::dBorder, y0 - 2 * ExtOptions::dBorder,
				w + 4 * ExtOptions::dBorder, h + 4 * ExtOptions::dBorder), 0.f, comms::argbWhite);
			rsRect(Rct(x0 - ExtOptions::dBorder, y0 - ExtOptions::dBorder,
				w + 2 * ExtOptions::dBorder, h + 2 * ExtOptions::dBorder), 0.f, comms::argbBlue);
			ShowString(x0, y0, Label, pFont);
			m_Stage = SRefresh;
			break;
		case SRefresh:
			float T;
			if(m_fIlluminate) {
				T = m_pUniversalMap->Refresh(m_Illumination) / 1000.0f;
			} else {
				T = m_pUniversalMap->Refresh() / 1000.f;
			}
			sprintf(m_Stats, "Refreshed in %.2f (s).", T);
			void ResetGroundCache();
			ResetGroundCache();
			m_Stage = SStats;
			m_tStatsStart = GetTickCount();
			break;
		case SStats:
			if(GetTickCount() - m_tStatsStart < 1000 * ExtOptions::tShowStats) {
				ShowString(10, RealLy - 40, m_Stats, &BigRedFont);
			} else m_fActive = false;
			break;
	}
}