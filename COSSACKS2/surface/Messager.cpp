#include "stdheader.h"
#include "Messager.h"

Surface::Messager g_Messager;

namespace Surface {
//-----------------------------------------------------------------------------
// Messager.ctor
//-----------------------------------------------------------------------------
Messager::Messager() {
	m_fActive = false;
}

void Messager::OnDrawOnMapOverAll() {
	if(!m_fActive) {
		return;
	}
	if(!m_fFirstFrameWasShown) {
		m_tStart = GetTickCount();
		m_fFirstFrameWasShown = true;
	}
	CmdExt::MessageBoxArgs Args;
	Args.Align = CmdExt::A_CORNER;
	if(GetTickCount() - m_tStart < 1000 * Messager::tShowMsg) {
		CmdExt::MessageBox(m_Msg, Args);
	} else {
		m_fActive = false;
	}
}

//-----------------------------------------------------------------------------
// Messager::DrawEditorInterface
//-----------------------------------------------------------------------------
void Messager::DrawEditorInterface() {

}

//-----------------------------------------------------------------------------
// Messager::ShowMessage : void(const char *)
//-----------------------------------------------------------------------------
void Messager::ShowMessage(const char *pStr) {
	m_Msg.Copy(pStr);
	m_fActive = true;
	m_fFirstFrameWasShown = false;
}

} // Surface