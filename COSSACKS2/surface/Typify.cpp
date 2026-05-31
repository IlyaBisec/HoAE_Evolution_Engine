#include "stdheader.h"
#include "Typify.h"
#include "SurfaceGenerator.h"
#include "CmdExt.h"

namespace Surface {

//-----------------------------------------------------------------------------------------
// Process
//-----------------------------------------------------------------------------------------
void Typify::Process(SurfRef &Ref, int idType, const SurfVertex &Center, int Radius) {
	assert(Ref.pOrig);
	eMappingType MappingType = SelExt::QueryMappingTypeKeys();
	for(int i = 0; i < Ref.idVerts.Count(); i++) {
		SurfVertex &r = Ref.pOrig->VERTS[Ref.idVerts[i]];
		if(MappingType & r.MappingType()) {
			if(SelExt::IsSelected(r, Center, Radius)) {
				r.SurfaceType = idType;
				SURF.TexturizeVertex(r);
			}
		}
	}
}

//-----------------------------------------------------------------------------
// OnLMB
//-----------------------------------------------------------------------------
DWORD Typify::OnLMB(SelExtArgs &Args) {
	if(m_fInInterface) return 0;
	SurfaceUndo::DropRefToUndo(Args.Ref);
	Process(Args.Ref, g_SurfEditor.Typify.TYPE, Args.Center, Args.Radius);
	return SelExt::IF_CACHE;
}

//-----------------------------------------------------------------------------
// OnRMB
//-----------------------------------------------------------------------------
DWORD Typify::OnRMB(SelExtArgs &Args) {
	if(m_fInInterface) return 0;
	SurfaceUndo::DropRefToUndo(Args.Ref);
	ActivateType(Args.Center.SurfaceType);
	return 0;
}

//-----------------------------------------------------------------------------
// OnDraw
//-----------------------------------------------------------------------------
void Typify::OnDraw(SelExtArgs &Args) {
	if(m_fInInterface) return;
	SurfMesh sm;
	SurfRef sr(Args.Ref);
	sr.ToSurfMesh(sm);
	sm.GetRefAll(sr, SR_VERTS);
	Process(sr, g_SurfEditor.Typify.TYPE, Args.Center, Args.Radius);
	SelExt::DrawSwatch(sm);
}

//-----------------------------------------------------------------------------
// ActivateType
//-----------------------------------------------------------------------------
void Typify::ActivateType(int idType) {
	assert(idType >= 0 && idType <= 2);
	g_SurfEditor.Typify.Type = idType;
	g_SurfEditor.Typify.Update();
}

//-----------------------------------------------------------------------------
// TypeControl
//-----------------------------------------------------------------------------
void Typify::TypeControl() {
	Enumerator *pEnum = ENUM.Get("SURFTYPES");
	int N = pEnum->GetAmount();
	for(int nType = 0; nType < N; nType++) {
		if(GetKeyState('1' + nType) < 0) {
			ActivateType(pEnum->GetVal(nType));
			break;
		}
	}
	cStr CurType(pEnum->GetStr(g_SurfEditor.Typify.TYPE));
	CmdExt::MessageBoxArgs Args;
	Args.Align = CmdExt::A_CORNER;
	CmdExt::MessageBox(CurType, Args);
}

//-----------------------------------------------------------------------------
// OnDrawInterface
//-----------------------------------------------------------------------------
void Typify::OnDrawInterface() {
	TypeControl();
	
	bool fIsPressed = GetKeyState(VK_SPACE) < 0;
	static int nCurSel = -1;
	if(fIsPressed) {
		nCurSel = m_SelCtrl.GetCurSel();
	}
	if(!fIsPressed && m_fInInterface) {
		if(nCurSel != -1) ActivateType(nCurSel);
		m_fInInterface = false;
	}
	if(fIsPressed && !m_fInInterface) {
		m_fInInterface = true;
	}
	if(fIsPressed) {
		m_SelCtrl.Draw(g_SurfEditor.Typify.TYPE, nCurSel);
	}
}

//-----------------------------------------------------------------------------
// OnActivate
//-----------------------------------------------------------------------------
void Typify::OnActivate() {
	m_fInInterface = false;
	m_SelCtrl.Init(SelCtrlCallback);
}

//-----------------------------------------------------------------------------
// SelCtrlCallback
//-----------------------------------------------------------------------------
bool Typify::SelCtrlCallback(int index, cStr &Name, DWORD &Color) {
	Enumerator *pEnum = ENUM.Get("SURFTYPES");
	assert(pEnum);
	int NTypes = pEnum->GetAmount();
	if(index >= NTypes) return false;
	Name = pEnum->GetStr(index);
	Color = cColor::Blue.ToBgra();
	return true;
}

} // Surface