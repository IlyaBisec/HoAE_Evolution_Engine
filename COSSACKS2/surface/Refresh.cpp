#include "Refresh.h"

namespace Surface {

//-----------------------------------------------------------------------------
// .ctor
//-----------------------------------------------------------------------------
Refresh::Refresh() {
	m_fActive = false;
}

//-----------------------------------------------------------------------------
// CheckActivityOfEditor
//-----------------------------------------------------------------------------
bool Refresh::CheckActivityOfEditor() {
	return m_fActive;
}

//-----------------------------------------------------------------------------
// ClearActivityOfEditor
//-----------------------------------------------------------------------------
void Refresh::ClearActivityOfEditor() {
	m_fActive = false;
}

//-----------------------------------------------------------------------------
// ActivateEditor
//-----------------------------------------------------------------------------
void Refresh::ActivateEditor() {
	if(m_fActive) return;
	m_fActive = true;
	m_Stage = S_LABEL;
}

//-----------------------------------------------------------------------------
// Refresh::Message : void(const cStr &, eAlign)
//-----------------------------------------------------------------------------
void Refresh::Message(cStr &Msg, eAlign Align) {
	if(!Msg.Length()) return;
	static RLCFont *pFont = &BigWhiteFont;
	int w = GetRLCStrWidth(Msg, pFont);
	int h = GetRLCHeight(pFont->RLC, 'W');
	int cx = 0, cy = 0;
	if(A_CENTER == Align) {
		cx = RealLx / 2;
		cy = RealLy / 2;
	} else if(A_CORNER == Align) {
		cx = Refresh::dCornerOffset + w / 2;
		cy = RealLy - Refresh::dCornerOffset - h / 2;
	} else assert(0);
	int x0 = cx - w / 2;
	int y0 = cy - h / 2;
	rsRect(Rct(x0 - 2 * Refresh::dBorder, y0 - 2 * Refresh::dBorder,
		w + 4 * Refresh::dBorder, h + 4 * Refresh::dBorder), 0.f, cColor::White.argb());
	rsRect(Rct(x0 - Refresh::dBorder, y0 - Refresh::dBorder,
		w + 2 * Refresh::dBorder, h + 2 * Refresh::dBorder), 0.f, cColor::Blue.argb());
	ShowString(x0, y0, Msg.asChar(), pFont);
}

//-----------------------------------------------------------------------------
// Refresh::DirectPassMessage
//-----------------------------------------------------------------------------
void Refresh::DirectPassMessage() {
	cStr Msg = "Direct Illumination";
	Msg << " - " << m_CurPass << "/" << g_SurfEditor.Shader.aQtyOfLights;
	Message(Msg);
}

//-----------------------------------------------------------------------------
// Refresh::LODsPassMessage
//-----------------------------------------------------------------------------
void Refresh::LODsPassMessage() {
	cStr Msg = "LODs";
	Msg << " - " << m_CurPass + 1 << "/" << m_PassesCount;
	Message(Msg);
}

//-----------------------------------------------------------------------------
// DrawEditorInterface
//-----------------------------------------------------------------------------
void Refresh::DrawEditorInterface() {
	if(!m_fActive) return;

	static DWORD tStart;
	switch(m_Stage) {
		case S_LABEL:
			m_Stage = S_REFRESH;
			Message(cStr("Refreshing..."));
			break;
		case S_REFRESH:
			tStart = GetTickCount();
			g_UniversalMap.QUADS.Compile();
			g_UniversalMap.QUADS.Init();
			g_UniversalMap.CalcNormals();
			g_UniversalMap.FillMapOfHeights();
			m_Stage = S_TRYINITLODSPASSES;
			switch(g_SurfEditor.Refresh.Illumination) {
				case I_LOCAL:
					Message(cStr("Local Illumination"));
					m_Stage = S_LOCAL;
					break;
				case I_DIRECT:
					m_CurPass = 0;
					DirectPassMessage();
					m_Stage = S_DIRECTPASSES;
					break;
			}
			break;
		case S_TRYINITLODSPASSES:
			if(g_SurfEditor.Refresh.LODs) {
				m_PassesCount = g_UniversalMap.QUADS.BeginPassedLODsGen();
				if(m_PassesCount > 0) {
					m_Stage = S_LODSPASSES;
					m_CurPass = 0;
					LODsPassMessage();
					break;
				}
			}
			m_Stage = S_PRESTATS;
			break;
		case S_LOCAL:
			g_UniversalMap.m_Shader.Shade(g_UniversalMap, I_LOCAL);
			m_Stage = S_TRYINITLODSPASSES;
			break;
		case S_DIRECTPASSES:
			if(0 == m_CurPass) {
				g_UniversalMap.m_Shader.BeginPassedDirectIllumination(g_UniversalMap);
				m_CurPass = 1;
				DirectPassMessage();
			} else {
				if(!g_UniversalMap.m_Shader.OnePass()) {
					if(g_SurfEditor.Refresh.LODs) {
						m_Stage = S_TRYINITLODSPASSES;
					} else m_Stage = S_PRESTATS;
				}
				else {
					m_CurPass++;
					DirectPassMessage();
				}
			}
			break;
		case S_LODSPASSES:
			if(g_UniversalMap.QUADS.LODsOnePass()) {
				m_CurPass++;
				LODsPassMessage();
			} else m_Stage = S_PRESTATS;
			break;
		case S_PRESTATS:
			float T;
			T = float(GetTickCount() - tStart) / 1000.0f;
			sprintf(m_Stats, "Refreshed in %.2f (s).", T);
			ResetGroundCache();
			m_Stage = S_STATS;
			m_tStatsStart = GetTickCount();
			break;
		case S_STATS:
			if(GetTickCount() - m_tStatsStart < 1000 * Refresh::tShowStats) {
				ShowString(10, RealLy - 40, m_Stats, &BigRedFont);
			} else m_fActive = false;
			break;
		default:
			assert(0);
	}
}

} // Surface