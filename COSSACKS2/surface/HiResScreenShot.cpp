#include "stdheader.h"
#include "HiResScreenShot.h"

Surface::HiResScreenShot g_HiResScreenShot;
void SetGameDisplayModeAnyway(int SizeX, int SizeY);
void GFieldShow();

namespace Surface {
//-----------------------------------------------------------------------------
// HiResScreenShot.ctor
//-----------------------------------------------------------------------------
HiResScreenShot::HiResScreenShot() {
	m_IsActive = false;
}

//-----------------------------------------------------------------------------
// HiResScreenShot::MakeScreenShot
//-----------------------------------------------------------------------------
void HiResScreenShot::MakeScreenShot() {
	IRS->GetDeviceDisplayMode(&m_ScrWidth, &m_ScrHeight, &m_ScrBpp, &m_ScrRR);

	int nModes = IRS->GetNDisplayModes();
	int MaxW = 0, MaxH = 0, W = 0, H = 0;
	for(int i = 0; i < nModes; i++) {
		IRS->GetDisplayMode(i, W, H);
		MaxW = cMath::Max(MaxW, W);
		MaxH = cMath::Max(MaxH, H);
	}

	g_SurfEditor.Update();
	m_UIWasHidden = g_SurfEditor.HideUI;
	if(!g_SurfEditor.HideUI) {
		g_SurfEditor.HideUI = true;
		g_SurfEditor.Update();
	}
	
	SetGameDisplayModeAnyway(MaxW, MaxH);
	for(int k = 0; k < 2; k++) {
		PRFUNC(GFieldShow());
		FlipPages();
	}

	m_msStart = GetTickCount();
	m_IsActive = true;
}

//-----------------------------------------------------------------------------
// HiResScreenShot::OnDrawOnMapOverAll
//-----------------------------------------------------------------------------
void HiResScreenShot::OnDrawOnMapOverAll() {
	if(!m_IsActive) return;
	
	void ScreenShot();
	ScreenShot();
	
	SetGameDisplayModeAnyway(m_ScrWidth, m_ScrHeight);

	if(!m_UIWasHidden) {
		g_SurfEditor.HideUI = false;
		g_SurfEditor.Update();
	}
	
	m_IsActive = false;
}

} // Surface