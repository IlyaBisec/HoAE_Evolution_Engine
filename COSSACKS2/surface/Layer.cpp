#include "stdheader.h"
#include "Layer.h"
#include "SurfaceGenerator.h"
#include "CmdExt.h"

namespace Surface {

//-----------------------------------------------------------------------------
// ClearLayers
//-----------------------------------------------------------------------------
void Layer::ClearLayers(const SurfVertex &Center, int Radius) {
	Enumerator *E = ENUM.Get("SURFLAYERS");
	for(int i = 0; i < E->GetAmount(); i++) {
		int id = FindLayerByName(E->Get(i));
		SurfLayerClearRound(id, Center, Radius);
	}
}

//-----------------------------------------------------------------------------
// OnLMB
//-----------------------------------------------------------------------------
DWORD Layer::OnLMB(SelExtArgs &Args) {
	if(m_fInInterface) return 0;
	SurfaceUndo::DropLayers(Args.Ref);
	OneLayerColor *pLayer = LAYERSLIST.Layers[g_SurfEditor.Layer.SelLayerIndex()];
	if(pLayer->WillEraseOthers) {
		ClearLayers(Args.Center, Args.Radius);
	} else {
		SurfLayerDrawRound(g_SurfEditor.Layer.SelLayerIndex(), Args.Center, Args.Radius);
	}
	SaveGenLayers();
	return SelExt::IF_CACHE;
}

//-----------------------------------------------------------------------------
// OnRMB
//-----------------------------------------------------------------------------
DWORD Layer::OnRMB(SelExtArgs &Args) {
	if(m_fInInterface) return 0;
	SurfaceUndo::DropLayers(Args.Ref);
	OneLayerColor *pLayer = LAYERSLIST.Layers[g_SurfEditor.Layer.SelLayerIndex()];
	if(pLayer->WillEraseOthers) {
		ClearLayers(Args.Center, Args.Radius);
	} else {
		SurfLayerClearRound(g_SurfEditor.Layer.SelLayerIndex(), Args.Center, Args.Radius);
	}
	SaveGenLayers();
	return SelExt::IF_CACHE;
}

//-----------------------------------------------------------------------------
// Layer::FindLayerByName : int(const char *, DWORD *)
//-----------------------------------------------------------------------------
int Layer::FindLayerByName(const char *LayerName, DWORD *pColor) {
	for(int i = 0; i < LAYERSLIST.Layers.GetAmount(); i++) {
		OneLayerColor *pLayer = LAYERSLIST.Layers[i];
		if(cStr::EqualsNoCase(LayerName, pLayer->Name.pchar())) {
			if(pColor) {
				*pColor = pLayer->Color;
			}
			return i;
		}
	}
	return -1;
}

//-----------------------------------------------------------------------------
// OnDraw
//-----------------------------------------------------------------------------
void Layer::OnDraw(SelExtArgs &Args) {
	if(m_fInInterface) return;
	ShowPen(Args.Center.x, Args.Center.y, Args.Radius, g_SurfEditor.Layer.SelLayerColor());
}

//-----------------------------------------------------------------------------
// OnActivate
//-----------------------------------------------------------------------------
void Layer::OnActivate() {
	g_SelType = ST_INCYLINDER;
	g_LockArrows = true;
	ResetGroundCache();
	m_fInInterface = false;
	m_SelCtrl.Init(SelCtrlCallback);
}

//-----------------------------------------------------------------------------
// OnClear
//-----------------------------------------------------------------------------
void Layer::OnClear() {
	g_LockArrows = false;
	DistField::ClearMaps();
	ResetGroundCache();
}

//-----------------------------------------------------------------------------
// ActivateLayer
//-----------------------------------------------------------------------------
void Layer::ActivateLayer(int idLayer) {
	g_SurfEditor.Layer.Layer = idLayer;
	g_SurfEditor.Layer.Update();
	ResetGroundCache();
}

//-----------------------------------------------------------------------------
// ScanXXOnDigits
//-----------------------------------------------------------------------------
/*int Layer::ScanXXOnDigits() {
	static DWORD PrevPressedTime = 0;
	static int PrevDigit = 0;
	static bool WasPressed = false;
	const char Keys[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

	int N = -1;
	for(int d = 0; d < 10; d++) {
		if(GetKeyState(Keys[d]) < 0) {
			if(WasPressed) return -1;
			if(GetTickCount() - PrevPressedTime < ScanDelay) {
				N = 10 * PrevDigit + d;
			} else N = d;
			PrevDigit = d;
			PrevPressedTime = GetTickCount();
			WasPressed = true;
			break;
		}
	}
	if(-1 == N) WasPressed = false;
	return N;
}*/

//-----------------------------------------------------------------------------
// ScanXOnArrows
//-----------------------------------------------------------------------------
int Layer::ScanXOnArrows() {
	static DWORD PrevPressedTime = 0;
	static bool WasPressed = false;
	if(GetTickCount() - PrevPressedTime > ScanDelay) WasPressed = false;
	const DWORD Keys[] = { VK_LEFT, VK_DOWN, VK_RIGHT, VK_UP };
	const int Deltas[] = { -1, -1, 1, 1 };

	int Delta = 0;
	for(int i = 0; i < 4; i++) {
		if(GetKeyState(Keys[i]) < 0) {
			if(WasPressed) return 0;
			Delta = Deltas[i];
			WasPressed = true;
			PrevPressedTime = GetTickCount();
			break;
		}
	}

	if(!Delta) WasPressed = false;
	return Delta;
}

//-----------------------------------------------------------------------------
// OnDrawInterface
//-----------------------------------------------------------------------------
void Layer::OnDrawInterface() {
	Enumerator *E = ENUM.Get("SURFLAYERS");

//	int N = ScanXXOnDigits();
//	if(N > -1) { ActivateLayer(E->GetVal(cMath::Clamp(N, 0, E->GetAmount() - 1))); }

	int D = ScanXOnArrows();
	if(D) { ActivateLayer(E->GetVal(cMath::Clamp(g_SurfEditor.Layer.LAYER + D, 0, E->GetAmount() - 1))); }

	CmdExt::MessageBoxArgs mba;
	mba.Align = CmdExt::A_CORNER;
	mba.FaceColor = g_SurfEditor.Layer.SelLayerColor();
	CmdExt::MessageBox(cStr(g_SurfEditor.Layer.SelLayerName()), mba);

	bool fIsPressed = GetKeyState(VK_SPACE) < 0;
	static int nCurSel = -1;
	if(fIsPressed) {
		nCurSel = m_SelCtrl.GetCurSel();
	}
	if(!fIsPressed && m_fInInterface) {
		if(nCurSel != -1) {
			ActivateLayer(nCurSel);
		}
		m_fInInterface = false;
	}
	if(fIsPressed && !m_fInInterface) {
		m_fInInterface = true;
	}
	if(fIsPressed) {
		m_SelCtrl.Draw(g_SurfEditor.Layer.LAYER, nCurSel);
	}
}

//-----------------------------------------------------------------------------
// Layer::SetCtrlCallback
//-----------------------------------------------------------------------------
bool Layer::SelCtrlCallback(int index, cStr &Name, DWORD &Color) {
	Enumerator *E = ENUM.Get("SURFLAYERS");
	int NLayers = E->GetAmount();
	if(index >= NLayers) return false;
	const char *LayerName = E->Get(index);
	DWORD Clr = 0;
	int id = FindLayerByName(LayerName, &Clr);
	assert(id >= 0 && "Error in layers registration!");
	Name = LayerName;
	Color = Clr;
	return true;
}

} // Surface