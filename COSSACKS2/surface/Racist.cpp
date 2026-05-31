#include "stdheader.h"
#include "Racist.h"
#include "SurfaceGenerator.h"
#include "CmdExt.h"

namespace Surface {

//-----------------------------------------------------------------------------
// OnLMB
//-----------------------------------------------------------------------------
DWORD Racist::OnLMB(SelExtArgs &Args) {
	if(m_fInInterface) return 0;
	bool TexturizeVerts = !(GetKeyState(VK_CONTROL) < 0);
	SurfaceUndo::DropRefToUndo(Args.Ref);
	RaceRef(g_SurfEditor.Racist.RACE, Args.Ref, Args.Center, Args.Radius, TexturizeVerts);
	SaveGenLayers();
	return SelExt::IF_CACHE;
}

//-----------------------------------------------------------------------------
// OnRMB
//-----------------------------------------------------------------------------
DWORD Racist::OnRMB(SelExtArgs &Args) {
	if(m_fInInterface) return 0;
	ActivateRace(ReadRaceIndex(Args.Center));
	return 0;
}

//-----------------------------------------------------------------------------
// OnDraw
//-----------------------------------------------------------------------------
void Racist::OnDraw(SelExtArgs &Args) {
	if(m_fInInterface) return;
	ShowPen(Args.Center.x, Args.Center.y, Args.Radius, cColor::Red.ToBgra());
}

//-----------------------------------------------------------------------------
// OnActivate
//-----------------------------------------------------------------------------
void Racist::OnActivate() {
	g_SelType = ST_INCYLINDER;
	m_fInInterface = false;
	m_SelCtrl.Init(SelCtrlCallback);
}

//-----------------------------------------------------------------------------
// ActivateRace
//-----------------------------------------------------------------------------
void Racist::ActivateRace(int idRace) {
	g_SurfEditor.Racist.Race = idRace;
	g_SurfEditor.Racist.Update();
}

//-----------------------------------------------------------------------------
// OnDrawInterface
//-----------------------------------------------------------------------------
void Racist::OnDrawInterface() {
	Enumerator *E = ENUM.Get("RACES");
	int N = E->GetAmount();
	for(int nRace = 0; nRace < N; nRace++) {
		if(GetKeyState('1' + nRace) < 0) {
			ActivateRace(E->GetVal(nRace));
			break;
		}
	}
	cStr CurRace(MLayers->MapsNames[g_SurfEditor.Racist.RACE]->pchar());
	DWORD Color(LAYERSLIST.Layers[g_SurfEditor.Racist.RACE]->Color);

	CmdExt::MessageBoxArgs mba;
	mba.Align = CmdExt::A_CORNER;
	mba.FaceColor = Color;
	CmdExt::MessageBox(CurRace, mba);

	bool fIsPressed = GetKeyState(VK_SPACE) < 0;
	static int nCurSel = -1;
	if(fIsPressed) {
		nCurSel = m_SelCtrl.GetCurSel();
	}
	if(!fIsPressed && m_fInInterface) {
		if(nCurSel != -1) {
			ActivateRace(E->GetVal(nCurSel));
		}
		m_fInInterface = false;
	}
	if(fIsPressed && !m_fInInterface) {
		m_fInInterface = true;
	}
	if(fIsPressed) {
		m_SelCtrl.Draw(g_SurfEditor.Racist.RACE - LAYERSLIST.MinRaceLandIndex, nCurSel);
	}
}

//-----------------------------------------------------------------------------
// Racist::SelCtrlCallback
//-----------------------------------------------------------------------------
bool Racist::SelCtrlCallback(int index, cStr &Name, DWORD &Color) {
	Enumerator *E = ENUM.Get("RACES");
	int NRaces = E->GetAmount();
	if(index >= NRaces) return false;
	Name = MLayers->MapsNames[E->GetVal(index)]->pchar();
	Color = LAYERSLIST.Layers[E->GetVal(index)]->Color;
	return true;
}

} // Surface