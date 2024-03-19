#include "Refresh.h"
#include "Generate.h"

namespace Surface {

//-----------------------------------------------------------------------------
// .ctor
//-----------------------------------------------------------------------------
Generate::Generate() {
	m_fActive = false;
}

//-----------------------------------------------------------------------------
// CheckActivityOfEditor
//-----------------------------------------------------------------------------
bool Generate::CheckActivityOfEditor() {
	return m_fActive;
}

//-----------------------------------------------------------------------------
// ClearActivityOfEditor
//-----------------------------------------------------------------------------
void Generate::ClearActivityOfEditor() {
	m_fActive = false;
}

//-----------------------------------------------------------------------------
// ActivateEditor
//-----------------------------------------------------------------------------
void Generate::ActivateEditor() {
	if(m_fActive) return;
	m_fActive = true;
	m_Stage = S_INIT;
}

bool GeneratorOnePass(cStr &);
//-----------------------------------------------------------------------------
// DrawEditorInterface
//-----------------------------------------------------------------------------
void Generate::DrawEditorInterface() {
	if(!m_fActive) return;
	cStr Label;
	
	static DWORD tStart;
	bool f;
	switch(m_Stage) {
		case S_INIT:
			tStart = GetTickCount();
			m_Stage = S_PASSES;
			assert(GeneratorOnePass(Label));
			Refresh::Message(Label);
			break;
		case S_PASSES:
			f = GeneratorOnePass(Label);
			if(f) Refresh::Message(Label);
			else {
				m_Stage = S_PRESTATS;
			}
			break;
		case S_PRESTATS:
			float T;
			T = float(GetTickCount() - tStart) / 1000.0f;
			sprintf(m_Stats, "Generated in %.2f (s).", T);
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