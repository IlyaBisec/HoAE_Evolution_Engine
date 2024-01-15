#include "stdheader.h"
#include "Archway.h"
#include "..\CurrentMapOptions.h"

extern int g_SelRadius;
namespace Surface {

//-----------------------------------------------------------------------------
// Archway::OnActivate
//-----------------------------------------------------------------------------
void Archway::OnActivate() {
	POINT pt;
	GetCursorPos(&pt);
	SurfRef Ref;
	if(g_UniversalMap.Pick(Ref, pt.x, pt.y)) {
		const SurfVertex &p = g_UniversalMap.VERTS[Ref.idVerts[0]];
		m_MaxHeight = p.z + 10;
	} else m_MaxHeight = 80;
	m_Radius = g_SelRadius;
}



//-----------------------------------------------------------------------------
// Archway::OnRMB
//-----------------------------------------------------------------------------
DWORD Archway::OnRMB(SelExtArgs &Args) {
	bool fDel = GetKeyState(VK_CONTROL) < 0;
	if(fDel) {
		const SurfVertex &c = Args.Ref.pOrig->VERTS[Args.Ref.idVerts[0]];
		for(int i = 0; i < MOptions.Archways.NValues; i++) {
			if(MOptions.Archways[i]->Equals(SurfArchway(c.x, c.y, 0, 0))) {
				MOptions.Archways.DelElement(i);
				return 0;
			}
		}
		return 0;
	} else {
		const SurfVertex &p = Args.Ref.pOrig->VERTS[Args.Ref.idVerts[0]];
		m_MaxHeight = p.z;
		return SelExt::IF_CACHE;
	}
}

//-----------------------------------------------------------------------------
// Archway::OnLMB
//-----------------------------------------------------------------------------
DWORD Archway::OnLMB(SelExtArgs &Args) {
	const SurfVertex &c = Args.Ref.pOrig->VERTS[Args.Ref.idVerts[0]];
	bool f = false;
	for(int i = 0; i < MOptions.Archways.NValues; i++) {
		if(MOptions.Archways[i]->Equals(SurfArchway(c.x, c.y, 0, 0))) {
			MOptions.Archways[i]->h = m_MaxHeight;
			MOptions.Archways[i]->r = m_Radius;
			f = true;
		}
	}
	if(!f) MOptions.Archways.Add(new SurfArchway(c.x, c.y, m_MaxHeight, m_Radius));
	Args.Ref.FillMapOfHeights(m_MaxHeight);
	return SelExt::IF_CACHE;
}

//-----------------------------------------------------------------------------
// Archway::OnWheel
//-----------------------------------------------------------------------------
bool Archway::OnWheel(int Delta) {
	bool fCtrl = GetKeyState(VK_SHIFT) < 0;
	if(fCtrl) m_MaxHeight += Delta > 0 ? Archway::HeightDelta : - Archway::HeightDelta;
	else {
		m_Radius += (Delta > 0 ? SelExt::RadiusDelta : - SelExt::RadiusDelta);
		if(m_Radius < SelExt::RadiusMin) m_Radius = SelExt::RadiusMin;
	}
	g_SelRadius = m_Radius;
	return true;
}

//-----------------------------------------------------------------------------
// Archway::DrawArchway
//-----------------------------------------------------------------------------
void Archway::DrawArchway(const SurfArchway &Way, const cColor &Color) {
	SurfMesh Round;
	Round.GenRound(SurfVertex(Way.cx, Way.cy, Way.h, Color.ToBgra(), 100), Way.r, 6);
	SelExt::DrawColoredSwatch(Round);
}

//-----------------------------------------------------------------------------
// Archway::OnDraw
//-----------------------------------------------------------------------------
void Archway::OnDraw(SelExtArgs &Args) {
	SurfVertex p = Args.Ref.pOrig->VERTS[Args.Ref.idVerts[0]];
	DrawArchway(SurfArchway(p.x, p.y, m_MaxHeight, m_Radius), cColor::Cyan);
	for(int i = 0; i < MOptions.Archways.NValues; i++) {
		DrawArchway(*MOptions.Archways[i], cColor::White);
	}
}

} // Surface