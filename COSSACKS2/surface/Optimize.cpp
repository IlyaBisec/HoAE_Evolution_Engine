#include "Refresh.h"
#include "Optimize.h"

namespace Surface {

//-----------------------------------------------------------------------------
// .ctor
//-----------------------------------------------------------------------------
Optimize::Optimize() {
	m_fActive = false;
}

//-----------------------------------------------------------------------------
// CheckActivityOfEditor
//-----------------------------------------------------------------------------
bool Optimize::CheckActivityOfEditor() {
	return m_fActive;
}

//-----------------------------------------------------------------------------
// ClearActivityOfEditor
//-----------------------------------------------------------------------------
void Optimize::ClearActivityOfEditor() {
	m_fActive = false;
}

//-----------------------------------------------------------------------------
// ActivateEditor
//-----------------------------------------------------------------------------
void Optimize::ActivateEditor() {
	if(m_fActive) return;
	m_fActive = true;
	m_Stage = S_INIT;
}

bool GeneratorOnePass(cStr &);
//-----------------------------------------------------------------------------
// DrawEditorInterface
//-----------------------------------------------------------------------------
void Optimize::DrawEditorInterface() {
	if(!m_fActive) return;
	cStr Label;
	
	static DWORD tStart;
	bool f;
	switch(m_Stage) {
		case S_INIT:
			tStart = GetTickCount();
			m_Stage = S_OPTIMIZE;
			Refresh::Message(cStr("Optimizing..."));
			break;
		case S_OPTIMIZE:
			void OptimizeSurfMap();
			OptimizeSurfMap();
			g_UniversalMap.QUADS.Init();
			g_UniversalMap.QUADS.FillMapOfHeights();
			m_Stage = S_PRESTATS;
			break;
		case S_PRESTATS:
			float T;
			T = float(GetTickCount() - tStart) / 1000.0f;
			sprintf(m_Stats, "Optimized in %.2f (s).", T);
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