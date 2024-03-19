#include "stdheader.h"
#include "SelExt.h"
#include "..\Camera\Navigator.h"
#include "IWater.h"
#include "..\AllTools.h"

#include "..\mesh\UniHash.h"
uni_hash<DWORD> baseV;
void AddBaseV(DWORD v,DWORD v1){
	baseV.add_uniq(v,v1);
}
void ClearBaseV(){
	baseV.reset();
};


int g_SelRadius = Surface::SelExt::RadiusInitial;
bool g_fWheelIsBlocked = false;

namespace Surface {

//-----------------------------------------------------------------------------
// QueryMappingTypeKeys
//-----------------------------------------------------------------------------
eMappingType SelExt::QueryMappingTypeKeys() {
	bool fHorizontal = GetKeyState(VK_CONTROL) < 0;
	bool fVertical = GetKeyState(VK_SHIFT) < 0;
	if(fVertical) return MT_VERTICAL;
	else if(fHorizontal) return MT_HORIZONTAL;
	else return MT_CUBIC;
}

//-----------------------------------------------------------------------------
// .ctor
//-----------------------------------------------------------------------------
SelExt::SelExt() {
	m_fActive = false;
	m_pUniversalMap = &g_UniversalMap;
	m_pSelRadius = &g_SelRadius;
}

//-----------------------------------------------------------------------------
// CheckActivityOfEditor
//-----------------------------------------------------------------------------
bool SelExt::CheckActivityOfEditor() {
	return m_fActive;
}

//-----------------------------------------------------------------------------
// ClearActivityOfEditor
//-----------------------------------------------------------------------------
void SelExt::ClearActivityOfEditor() {
	if(m_fActive) {
		g_fSpaceIsBlocked = false;
		g_fWheelIsBlocked = false;
		m_fActive = false;
		OnClear();
	}
}

eSelType g_SelType;
//-----------------------------------------------------------------------------
// ActivateEditor
//-----------------------------------------------------------------------------
void SelExt::ActivateEditor() {
	g_SelType = ST_INSPHEREWITHSPREAD;
	g_fSpaceIsBlocked = true;
	g_fWheelIsBlocked = true;
	m_Args.Ref.Free();
	m_Args.Ref.pOrig = m_pUniversalMap;
	m_Args.Radius = *m_pSelRadius;
	m_Args.Center = SurfVertex::Origin;
	m_Args.IntersectionPoint.SetZero();
	m_fActive = true;
    m_Args.Angle = 0;
    m_Args.Scale = 64;
    m_Args.dU = 0;
    m_Args.dV = 0;
    m_Args.LimitInSurfaceType = false;
	m_Args.ToDefault();
	OnActivate();
}
//-----------------------------------------------------------------------------
// OnMouseWheel
//-----------------------------------------------------------------------------
bool SelExt::OnMouseWheel(int Delta) {
	if(!m_fActive) return false;
	if(!OnWheel(Delta)) {
        if(GetKeyState('A') < 0){
            //m_Args.Angle+=Delta/120;
			m_Args.ApplyRotate(Delta/120.0/64.0f);
        }else
        if(GetKeyState('S') < 0){
            //m_Args.Scale+=Delta/120;
            m_Args.ApplyScale(exp(Delta/120.0/32.0));
        }else
        if(GetKeyState('Z') < 0){
            //m_Args.dV+=Delta/120;           
			m_Args.ApplyShift(Delta/120.0/128.0f,0);
        }else
        if(GetKeyState('X') < 0){
            //m_Args.dU+=Delta/120;           
			m_Args.ApplyShift(0,Delta/120.0/128.0f);
        }else{
		    *m_pSelRadius += (Delta > 0 ? SelExt::RadiusDelta : - SelExt::RadiusDelta);
		    if(*m_pSelRadius < SelExt::RadiusMin) *m_pSelRadius = SelExt::RadiusMin;
		    m_Args.Radius = *m_pSelRadius;
        }
	}
	return true;
}

//------------------------------------------------------------------------------------------------------------------
// OnMouseHandling
//------------------------------------------------------------------------------------------------------------------
bool SelExt::OnMouseHandling(int xScreen, int yScreen, bool &LMB, bool &RMB, int xMap, int yMap, bool OverMiniMap) {
	if(!m_fActive || g_Navigator.m_Mode || OverMiniMap) return false;

	m_Args.Ref.Free();

	SurfRef sr;
	if(!m_pUniversalMap->Pick(sr, xScreen, yScreen, &m_Args.IntersectionPoint)) {
		return false;
	}
	m_Args.Center = m_pUniversalMap->VERTS[sr.idVerts[0]];
	m_Args.Ref.pOrig = sr.pOrig;

	SurfRef sp;
	switch(g_SelType) {
		case ST_INSPHEREWITHSPREAD:
			m_pUniversalMap->GetRefSphere(sp, m_Args.Center, m_Args.Radius);
			sp.SpreadTo(m_Args.Ref, sr.idVerts[0],m_Args.LimitInSurfaceType);
			break;
		case ST_INCYLINDER:
			m_pUniversalMap->GetRefCylinder(m_Args.Ref, m_Args.Center, m_Args.Radius);
			break;
		default:
			assert(0);
	}

	if(!LMB && !RMB) {
		return false;
	}

	assert(m_Args.Ref.idQuads.Count());
	assert(m_Args.Ref.idTris.Count());
	assert(m_Args.Ref.idVerts.Count());

	DWORD Flags = 0;
	if(LMB) Flags = OnLMB(m_Args);
	else if(RMB) Flags = OnRMB(m_Args);
	if(Flags & IF_CACHE) {
		if(IF_GEOMETRY == Flags) {
			m_Args.Ref.UpdateNormals(!m_Args.PreserveUV);
			//m_Args.Ref.FillMapOfHeights();
			m_pUniversalMap->m_Shader.Shade(m_Args.Ref);
			m_pUniversalMap->QUADS.Update(m_Args.Ref);
		}
		int x0, y0, x1, y1;
		m_pUniversalMap->QUADS.CalcRectOf(m_Args.Ref, x0, y0, x1, y1);
        Rct rc(x0, y0, x1-x0, y1-y0);
		ITerra->InvalidateGeometry(&rc);
        IWater->Refresh(rc);
	}
	return true;
}

//-----------------------------------------------------------------------------
// OnDrawOnMapOverAll
//-----------------------------------------------------------------------------
void SelExt::OnDrawOnMapOverAll() {
	if(!m_fActive || !m_Args.Ref.pOrig) return;
	m_Args.PreserveUV=AllTools::KeepUV;//GetKeyState(VK_SCROLL);
	if(m_Args.Ref.idVerts.Count()) OnDraw(m_Args);
}

//-----------------------------------------------------------------------------
// DrawEditorInterface
//-----------------------------------------------------------------------------
void SelExt::DrawEditorInterface() {
	if(!m_fActive) return;
	if(GetKeyState(VK_HOME) < 0) {
		g_SelRadius = Surface::SelExt::RadiusInitial;
		OnMouseWheel(0);
	}
	OnDrawInterface();
}

//------------------------------------------------------------------------------------
// DrawWindow
//------------------------------------------------------------------------------------
void SelExt::DrawWindow(int x0, int y0, int x1, int y1, DWORD argbColor, int Border) {
	rsRect(Rct(x0, y0, x1 - x0, Border), 0.f, argbColor);
	rsRect(Rct(x0, y1 - Border, x1 - x0, Border), 0.f, argbColor);
	rsRect(Rct(x0, y0 + Border, Border, y1 - y0 - 2 * Border), 0.f, argbColor);
	rsRect(Rct(x1 - Border, y0 + Border, Border, y1 - y0 - 2 * Border), 0.f, argbColor);
}

//-----------------------------------------------------------------------------
// SplitSwatch : void(SurfMesh &, SurfSplit &, SurfSplit &)
//-----------------------------------------------------------------------------
void SelExt::SplitSwatch(SurfMesh &sm, SurfSplit &ssStage1, SurfSplit &ssStage2) {
	SurfRef Ref;
	sm.GetRefAll(Ref, SR_TRIS);
	if(!Ref.idTris.Count()) return;
	ssStage1.Split(Ref, SFX_SWATCHSTAGE1);

	SurfSplit::Cache::ModifyArgs Args;
	ZeroMemory(&Args, sizeof(Args));
	Args.Flags = SurfSplit::Cache::MF_AND;
	Args.DiffuseAndFlag = 0x30ffffff;
	ssStage1.CACHE.Modify(Args);

	ssStage2.Split(Ref, SFX_SWATCHSTAGE2);
	Args.Flags = SurfSplit::Cache::MF_SUM;
	Args.DiffuseSum = 0x00080808;
	ssStage2.CACHE.Modify(Args);
}

//-----------------------------------------------------------------------------
// DrawSwatch
//-----------------------------------------------------------------------------
void SelExt::DrawSwatch(SurfMesh &Mesh) {
	SurfSplit ss1, ss2;
	SplitSwatch(Mesh, ss1, ss2);
	ss1.Draw();
	ss2.Draw();
}

//-----------------------------------------------------------------------------
// DrawColoredSwatch
//-----------------------------------------------------------------------------
void SelExt::DrawColoredSwatch(SurfMesh &Mesh) {
	SurfRef Ref;
	Mesh.GetRefAll(Ref, SR_TRIS);
	if(!Ref.idTris.Count()) return;
	static int wID = IRS->GetTextureID("white.bmp");
	SurfSplit ss;
	ss.Split(Ref, SFX_SWATCHSTAGE1);
	
	SurfSplit::Cache::ModifyArgs Args;
	ZeroMemory(&Args, sizeof(Args));
	Args.Flags = SurfSplit::Cache::MF_AND | SurfSplit::Cache::MF_TEXTURE;
	Args.DiffuseAndFlag = 0x30ffffff;
	Args.idTexture = wID;
	ss.CACHE.Modify(Args);
	ss.Draw();
	ss.Split(Ref, SFX_SWATCHSTAGE2);
	Args.DiffuseAndFlag = 0x60ffffff;
	ss.CACHE.Modify(Args);
	ss.Draw();
}
void DbgStr (Vector3D pos,DWORD Color,LPSTR sz,...)
{
	char ach[256];
	va_list va;
	va_start( va, sz );
	vsprintf ( ach, sz, va );   
	va_end( va );
	WorldToScreenSpace(pos);
	ISM->SetCurrentDiffuse(Color);
	void ShowString(int x, int y, LPCSTR lps, lpRLCFont lpf);
	ShowString(pos.x,pos.y,ach,&SmallWhiteFont);
    
};

void DrawTangents(const SurfRef &Ref){
	int n=Ref.idTris.Count();
	for(int i=0;i<n;i++){
		SurfVertex* s[3];
		int idTri=Ref.idTris[i];
		Ref.pOrig->GetTri(idTri,&s[0]);
		int ii[3];
		Ref.pOrig->GetTri(idTri,ii[0],ii[1],ii[2]);
		Vector3D v3(0,0,0);
		for(int i=0;i<3;i++){
			SurfVertex* sv=s[i];
			Vector3D vv(sv->x,sv->y,sv->z);
			Vector3D n(sv->nx,sv->ny,sv->nz);
			n.normalize();
			Vector3D t(sv->tx,sv->ty,sv->tz);
			Vector3D b(sv->bx,sv->by,sv->bz);
			t*=float(sv->tscale)/127/8;
			b*=float(sv->bscale)/127/8;
			Vector3D v3;
			t+=vv;
			b+=vv;
			rsLine(vv,t,0xFFFF0000,0xFF00FFFF);
			rsLine(vv,b,0xFF00FF00,0xFFFF00FF);
			/*
			int ns=baseV.size(ii[i]);
			if(ns==1)DbgStr(vv,0xFFFF0000,"%d(%d)",ii[i],*baseV.get(ii[i],0));
			else if(ns==2)DbgStr(vv,0xFF00FF00,"%d(%d %d)",ii[i],*baseV.get(ii[i],0),*baseV.get(ii[i],1));
			else if(ns==3)DbgStr(vv,0xFF00FF00,"%d(%d %d %d)",ii[i],*baseV.get(ii[i],0),*baseV.get(ii[i],1),*baseV.get(ii[i],2));
			else DbgStr(vv,0xFF00FF00,"%d(%d)",ii[i],ns);
			*/
			v3+=vv;
		}
		v3/=3;
		//DbgStr(v3,0xFF00FFFF,"%d",idTri);
    }
	rsFlush();
	ISM->Flush();
}
//-----------------------------------------------------------------------------
// DrawGrid
//-----------------------------------------------------------------------------
void SelExt::DrawGrid(const SurfRef &Ref, DWORD argbColor) {
	if(!Ref.idTris.Count()) return;
	SurfSplit ss;
	ss.Split(Ref, SFX_GRIDSTAGE1);

	SurfSplit::Cache::ModifyArgs Args;
	ZeroMemory(&Args, sizeof(Args));
	Args.DiffuseOrFlag = argbColor & 0x30ffffff;
	Args.Flags = SurfSplit::Cache::MF_AND | SurfSplit::Cache::MF_OR;
	ss.CACHE.Modify(Args);

	ss.Draw();
	ss.Split(Ref, SFX_GRIDSTAGE2);

	Args.DiffuseOrFlag = argbColor;
	ss.CACHE.Modify(Args);
	
	ss.Draw();
	DrawTangents(Ref);
}

//--------------------------------------------------------------------------------------
// IsSelected
//--------------------------------------------------------------------------------------
bool SelExt::IsSelected(const SurfVertex &vtx, const SurfVertex &Center, int Radius) {
	if(Radius < 1) return false;
	int Z = ST_INCYLINDER == g_SelType ? 0 : cMath::Square(Center.z - vtx.z);
	int l = sqrt(cMath::Square(Center.x - vtx.x) + cMath::Square(Center.y - vtx.y) + Z);
	return l < Radius;
}

//-----------------------------------------------------------------------------
// SurfSelCtrl::Init
//-----------------------------------------------------------------------------
void SurfSelCtrl::Init(SurfSelCtrl::CallBack fn, const int X0, const int Y0, const int W, const int H) {
	m_Samples.Free();

	int NSamples = 0;
	Sample s;
	while(fn(NSamples, s.Name, s.Color)) {
		m_Samples.Add(s);
		NSamples++;
	}
	if(!NSamples) return;

	int x0 = X0, y0 = Y0;
	if(x0 == -1) x0 = (RealLx - W) / 2;
	if(y0 == -1) y0 = (RealLy - H * NSamples) / 2;
	for(int i = 0; i < NSamples; i++) {
		Sample &s = m_Samples[i];
		s.x0 = x0;
		s.y0 = y0 + i * H;
		s.x1 = x0 + W;
		s.y1 = s.y0 + H;
	}
}

//-----------------------------------------------------------------------------
// SurfSelCtrl::Sample::Draw
//-----------------------------------------------------------------------------
void SurfSelCtrl::Sample::Draw() const {
	int w = x1 - x0, h = y1 - y0;
	rsRect(Rct(x0, y0, w, h), 0.f, Color);
	static RLCFont *pFont = &BigWhiteFont;
	assert(pFont);
	int tw = GetRLCStrWidth(cStr(Name), pFont);
	int th = GetRLCHeight(pFont->RLC, 'W');
	int tx0 = x0 + (w - tw) / 2;
	int ty0 = y0 + (h - th) / 2;
	ShowString(tx0, y0, Name.ToCharPtr(), pFont);
}

//-----------------------------------------------------------------------------
// SurfSelCtrl::Draw
//-----------------------------------------------------------------------------
void SurfSelCtrl::Draw(int nActive, int nHot) const {
   	for(int i = 0; i < m_Samples.Count(); i++) {
		m_Samples[i].Draw();
	}
	if(nActive >= 0 && nActive < m_Samples.Count()) {
		const Sample &s = m_Samples[nActive];
		SelExt::DrawWindow(s.x0 - 1, s.y0 - 1, s.x1 + 1, s.y1 + 1, cColor::White.ToBgra(), SelExt::dBorder);
	}
	if(nHot >= 0 && nHot < m_Samples.Count()) {
		const Sample &s = m_Samples[nHot];
		SelExt::DrawWindow(s.x0, s.y0, s.x1, s.y1, cColor::Red.ToBgra(), SelExt::dBorder);
	}
}

//-----------------------------------------------------------------------------
// SurfSelCtrl::GetCurSel
//-----------------------------------------------------------------------------
int SurfSelCtrl::GetCurSel() const {
	POINT pt;
	GetCursorPos(&pt);
	for(int i = 0; i < m_Samples.Count(); i++) {
		if(m_Samples[i].IsPicked(pt.x, pt.y)) return i;
	}
	return -1;
}

//*********************************************************************************************************************
// SurfListCtrl
//*********************************************************************************************************************

//-----------------------------------------------------------------------------
// SurfListCtrl::Init
//-----------------------------------------------------------------------------
void SurfListCtrl::Init(SurfListCtrl::CallBack fn, const int nSelected, const DWORD Key,
						const int X0, const int Y0, const int W, const int H) {
	m_Samples.Free();
	m_nSelected = nSelected;
	m_Key = Key;
	m_fActive = false;

	int NSamples = 0;
	Sample s;
	while(fn(NSamples, s.Name, s.Color)) {
		m_Samples.Add(s);
		NSamples++;
	}
	if(!NSamples) return;

	int x0 = X0, y0 = Y0;
	if(x0 == -1) x0 = (RealLx - W) / 2;
	if(y0 == -1) y0 = (RealLy - H * NSamples) / 2;
	for(int i = 0; i < NSamples; i++) {
		Sample &s = m_Samples[i];
		s.x0 = x0;
		s.y0 = y0 + i * H;
		s.x1 = x0 + W;
		s.y1 = s.y0 + H;
	}
}

//-----------------------------------------------------------------------------
// SurfListCtrl::Sample::Draw
//-----------------------------------------------------------------------------
void SurfListCtrl::Sample::Draw() const {
	int w = x1 - x0, h = y1 - y0;
	rsRect(Rct(x0, y0, w, h), 0.f, Color);
	static RLCFont *pFont = &BigWhiteFont;
	assert(pFont);
	int tw = GetRLCStrWidth(cStr(Name), pFont);
	int th = GetRLCHeight(pFont->RLC, 'W');
	int tx0 = x0 + (w - tw) / 2;
	int ty0 = y0 + (h - th) / 2;
	ShowString(tx0, y0, Name.ToCharPtr(), pFont);
}

//-----------------------------------------------------------------------------
// SurfListCtrl::Draw
//-----------------------------------------------------------------------------
void SurfListCtrl::Draw(int nActive, int nHot) const {
   	for(int i = 0; i < m_Samples.Count(); i++) {
		m_Samples[i].Draw();
	}
	if(nActive >= 0 && nActive < m_Samples.Count()) {
		const Sample &s = m_Samples[nActive];
		SelExt::DrawWindow(s.x0 - 1, s.y0 - 1, s.x1 + 1, s.y1 + 1, cColor::White.ToBgra(), SelExt::dBorder);
	}
	if(nHot >= 0 && nHot < m_Samples.Count()) {
		const Sample &s = m_Samples[nHot];
		SelExt::DrawWindow(s.x0, s.y0, s.x1, s.y1, cColor::Red.ToBgra(), SelExt::dBorder);
	}
}

//-----------------------------------------------------------------------------
// SurfListCtrl::GetCurSel
//-----------------------------------------------------------------------------
int SurfListCtrl::GetCurSel() const {
	POINT pt;
	GetCursorPos(&pt);
	for(int i = 0; i < m_Samples.Count(); i++) {
		if(m_Samples[i].IsPicked(pt.x, pt.y)) return i;
	}
	return -1;
}

//-----------------------------------------------------------------------------
// SurfListCtrl::OnDrawInterface : void()
//-----------------------------------------------------------------------------
void SurfListCtrl::OnDrawInterface() {
	bool fKey = GetKeyState(m_Key) < 0;
	static int nCurSel = -1;
	if(fKey) {
		nCurSel = GetCurSel();
	}
	if(!fKey && m_fActive) {
		if(nCurSel != -1) {
			m_nSelected = nCurSel;
		}
		m_fActive = false;
		return;
	}
	if(fKey && !m_fActive) {
		m_fActive = true;
	}
	if(m_fActive) {
		Draw(m_nSelected, nCurSel);
	}
}

//*********************************************************************************************************************
// SurfSwatchCtrl
//*********************************************************************************************************************

//--------------------------------------------------------------------------------------------------------------------------
// SurfSwatchCtrl::Init : void(CallBack, const int, const DWORD, const bool)
//--------------------------------------------------------------------------------------------------------------------------
void SurfSwatchCtrl::Init(SurfSwatchCtrl::CallBack fn, const int nSelected, const DWORD Key,
						  const bool fShowSelectedIcon, const bool fAllowSelectNothing) {
	m_Swatch.Samples.Free();
	m_nSelected = nSelected;
	m_Key = Key;
	m_fShowSelectedIcon = fShowSelectedIcon;
	m_fAllowSelectNothing = fAllowSelectNothing;
	m_fActive = false;

	int idTex;
	ints idsTex;
	while(fn(idsTex.Count(), idTex)) {
		idsTex.Add(idTex);
	}

	if(!idsTex.Count()) return;

	m_Swatch.Gen(idsTex);
	CurSel(m_nSelected);
}

//-----------------------------------------------------------------------------
// SurfSwatchCtrl::Sample::Draw : void()
//-----------------------------------------------------------------------------
void SurfSwatchCtrl::Sample::Draw() {
	// Fonts:
	static RLCFont *pFont = &SmallWhiteFont;
	static int hFont = GetRLCHeight(pFont->RLC, 'W');

	DrawBM(bmSample);
	DrawBM(bmInfoBase);
	// Info base:
	int xText = x0 + SelExt::dBorder;
	ShowString(xText, y0, Info.Title, pFont);
	ShowString(xText, y0 + hFont, Info.WxH, pFont);
}

//-----------------------------------------------------------------------------
// SurfSwatchCtrl::DrawSwatch : void(int, int)
//-----------------------------------------------------------------------------
void SurfSwatchCtrl::Swatch::Draw(int nSelected, int nHot) {
	for(int i = 0; i < Samples.Count(); i++) {
		Samples[i].Draw();
	}
	if(nSelected >= 0 && nSelected < Samples.Count()) {
		const Sample &Cur = Samples[nSelected];
		SelExt::DrawWindow(Cur.x0 - 1, Cur.y0 - 1, Cur.x1 + 1, Cur.y1 + 1, cColor::White.ToBgra(), SelExt::dBorder);
	}
	if(nHot >= 0 && nHot < Samples.Count()) {
		const Sample &Sel = Samples[nHot];
		SelExt::DrawWindow(Sel.x0, Sel.y0, Sel.x1, Sel.y1, cColor::Red.ToBgra(), SelExt::dBorder);
	}
}

//-----------------------------------------------------------------------------
// SurfSwatchCtrl::DrawActiveSample
//-----------------------------------------------------------------------------
void SurfSwatchCtrl::DrawActiveSample() {
	if(!m_Swatch.Samples.Count() || -1 == m_nSelected || !m_fShowSelectedIcon) return;
	Sample &A = m_Swatch.ActiveSample;
	A.Draw();
	SelExt::DrawWindow(A.x0 - SelExt::dBorder, A.y0 - SelExt::dBorder,
		A.x1 + SelExt::dBorder, A.y1 + SelExt::dBorder, cColor::Black.ToBgra(), SelExt::dBorder);
}

//-----------------------------------------------------------------------------
// SurfSwatchCtrl::Swatch::GetCurSel : int() const
//-----------------------------------------------------------------------------
int SurfSwatchCtrl::Swatch::GetCurSel() const {
	POINT pt;
	GetCursorPos(&pt);
	for(int i = 0; i < Samples.Count(); i++) {
		const Sample &r = Samples[i];
		if(r.IsPicked(pt.x, pt.y)) return i;
	}
	return -1;
}

//--------------------------------------------------------------------------------
// SurfSwatchCtrl::Sample::Gen : void(int, int, int, int)
//--------------------------------------------------------------------------------
void SurfSwatchCtrl::Sample::Gen(int x, int y, int L, int idTexture) {
	// Swatch:
	bmSample.create(4, 6, vfVertexTnL);
	
	// Topology:
	word *pInds = bmSample.getIndices();
	int Topology[6] = {0, 1, 3, 1, 2, 3};
	for(int i = 0; i < 6; i++) {
		pInds[i] = Topology[i];
	}

	// Verts:
	VertexTnL *pVerts = (VertexTnL*)bmSample.getVertexData();
	float xPos[4] = {x, x, x + L, x + L};
	float yPos[4] = {y, y + L, y + L, y};
	float u[4] = {0.f, 0.f, 1.f, 1.f};
	float v[4] = {0.f, 1.f, 1.f, 0.f};
	for(int i = 0; i < 4; i++) {
		pVerts->x = xPos[i];
		pVerts->y = yPos[i];
		pVerts->u = u[i];
		pVerts->v = v[i];
		pVerts->w = 1.f;
		pVerts->diffuse = 0xFFFFFFFF;
		pVerts++;
	}

	bmSample.setNInd(6);
	bmSample.setNVert(4);
	bmSample.setNPri(2);
	bmInfoBase = bmSample;
	bmInfoBase.setShader(SurfFx(SFX_AUX));
	bmSample.setTexture(idTexture);
	bmSample.setShader(SurfFx(SFX_SAMPLES));

	// InfoBase:
	pVerts = (VertexTnL*)bmInfoBase.getVertexData();
	const int l = 30;
	float yInfoPos[4] = {y, y + l, y + l, y};
	DWORD Diffuse[4] = {0xFF000000, 0x00000000, 0x00000000, 0xFF000000};
	for(i = 0; i < 4; i++) {
		pVerts->y = yInfoPos[i];
		pVerts->diffuse = Diffuse[i];
		pVerts++;
	}

	// Info:
	// WxH:
	int W = IRS->GetTextureWidth(idTexture);
	int H = IRS->GetTextureHeight(idTexture);
	Info.WxH = "(";
	Info.WxH += W;
	Info.WxH += "x";
	Info.WxH += H;
	Info.WxH += ")";
	// Title:
	const char *pStrPath = IRS->GetTextureName(idTexture);
	if(pStrPath) {
		char *pLastBS = strrchr(pStrPath, '\\');
		if(pLastBS) {
			pLastBS++;
			Info.Title = pLastBS;
		} else Info.Title = pStrPath;
	}

	x0 = x, y0 = y;
	x1 = x + L, y1 = y + L;
}

//-----------------------------------------------------------------------------
// SurfSwatchCtrl::Swatch::Gen : void(ints)
//-----------------------------------------------------------------------------
void SurfSwatchCtrl::Swatch::Gen(ints idsTex) {
	const int Offset = 20;

	Samples.Free();

	int N = idsTex.Count();
	if(!N) return;

	// In line:
	double d = sqrt((double)N);
	int NumInLine = (int)d;
	if(double(NumInLine) < d) NumInLine++;
	// Dims:
	int cx = RealLx / 2, cy = RealLy / 2;
	int LenInLine = RealLy - 2 * Offset;
	int LxL = LenInLine / NumInLine;
	int X0 = cx - LenInLine / 2;
	int Y0 = cy - LenInLine / 2;
	Sample S;
	for(int i = 0; i < N; i++) {
		int x = X0 + LxL * (i % NumInLine);
		int y = Y0 + LxL * (i / NumInLine);
		S.Gen(x, y, LxL, idsTex[i]);
		Samples.Add(S);
	}
	// Active Sample:
	ActiveSample.Gen(RealLx - SelExt::IconLxL - SelExt::IconRightOffset,
		SelExt::IconTopOffset, SelExt::IconLxL, idsTex[0]);
}

//-----------------------------------------------------------------------------
// SurfSwatchCtrl::CurSel : void(int)
//-----------------------------------------------------------------------------
void SurfSwatchCtrl::CurSel(int nSelected) {
	m_nSelected = nSelected;
	if(m_nSelected >= 0 && m_nSelected < m_Swatch.Samples.Count()) {
		Sample &A = m_Swatch.ActiveSample;
		const Sample &r = m_Swatch.Samples[m_nSelected];
		A.Info = r.Info;
		A.bmSample.setTexture(r.bmSample.getTexture());
	}
}

//-----------------------------------------------------------------------------
// SurfSwatchCtrl::OnDrawInterface : void()
//-----------------------------------------------------------------------------
void SurfSwatchCtrl::OnDrawInterface() {
	DrawActiveSample();

	bool fKey = GetKeyState(m_Key) < 0;
	static int nCurSel = -1;
	if(fKey) {
		nCurSel = m_Swatch.GetCurSel();
	}
	if(!fKey && m_fActive) {
		if(nCurSel != -1) {
			if(nCurSel != m_nSelected) {
				CurSel(nCurSel);
			}
		} else {
			if(m_fAllowSelectNothing) {
				CurSel(-1);
			}
		}
		m_fActive = false;
		return;
	}
	if(fKey && !m_fActive) {
		m_fActive = true;
	}
	if(m_fActive) {
		m_Swatch.Draw(m_nSelected, nCurSel);
	}
}

//--------------------------------------------------------------------------------------------------------------
// SelExt::GenArrows
//--------------------------------------------------------------------------------------------------------------
void SelExt::GenArrows(BaseMesh &bm, const cList<cVec3> &Centers, const cList<cVec3> &Dirs, const cColor &Color,
					   const float Radius, const float Length, const int idTex, const int idSh) {
	if(!Centers.Count()) {
		return;
	}
	assert(Centers.Count() == Dirs.Count());
	const int NVerts = 5 * Centers.Count();
	const int NTris = 6 * Centers.Count();
	// Pyramid topology:
	const int Topology[18] = { 0, 2, 1, 0, 3, 2, 1, 2, 4, 2, 3, 4, 3, 0, 4, 0, 1, 4 };
	const float h = 0.5f * Radius;
	const cVec3 Shifts[5] = { cVec3(-h, -h, 0.0f), cVec3(-h, h, 0.0f), cVec3(h, h, 0.0f),
		cVec3(h, -h, 0.0f), cVec3(0.0f, 0.0f, Length) };

	// Genning:
	bm.create(NVerts, 3 * NTris, vfVERTEX);
	VertexTS *pVerts = (VertexTS *)bm.getVertexData();
	word *pInds = bm.getIndices();
	for(int nArrow = 0; nArrow < Centers.Count(); nArrow++) {
		const cVec3 &c = Centers[nArrow];
		for(int nVertex = 0; nVertex < 5; nVertex++) {
			cVec3 p(Shifts[nVertex]);
			if(4 == nVertex) {
				p = Length * Dirs[nArrow];
			}
			p += Centers[nArrow];
			pVerts->x = p.x;
			pVerts->y = p.y;
			pVerts->z = p.z;
			pVerts->u = pVerts->v = 0.0f;
			pVerts->diffuse = Color.ToBgra();
			pVerts++;
		}
		const int StartIndex = 5 * nArrow;
		for(int i = 0; i < 18; i++) {
			*pInds = StartIndex + Topology[i];
			pInds++;
		}
	}
	bm.setNVert(NVerts);
	bm.setNInd(3 * NTris);
	bm.setNPri(NTris);
	bm.setShader(idSh);
	bm.setTexture(idTex);
}

//---------------------------------------------------------------------------------------------------------------------------------
// SelExt::GenCubes
//---------------------------------------------------------------------------------------------------------------------------------
void SelExt::GenCubes(BaseMesh &bm, const cList<cVec3> &Centers, const cColor &Color, const float d, const int idTex, const idSh) {
	if(!Centers.Count()) {
		return;
	}
	const int NVerts = 8 * Centers.Count();
	const int NTris = 12 * Centers.Count();
	// Cube topology:
	const int Topology[36] = { 0, 2, 1, 0, 3, 2, 2, 3, 6, 3, 7, 6, 6, 7, 4, 6, 4, 5, 0, 1, 5, 0, 5, 4,
		1, 2, 6, 1, 6, 5, 3, 0, 7, 0, 4, 7 };
	const float h = 0.5f * d;
	const cVec3 Shifts[8] = { cVec3(-h, -h, -h), cVec3(-h, h, -h), cVec3(h, h, -h), cVec3(h, -h, -h),
		cVec3(-h, -h, h), cVec3(-h, h, h), cVec3(h, h, h), cVec3(h, -h, h) };

	// Genning:
	bm.create(NVerts, 3 * NTris, vfVERTEX);
	VertexTS *pVerts = (VertexTS *)bm.getVertexData();
	word *pInds = bm.getIndices();
	for(int nCube = 0; nCube < Centers.Count(); nCube++) {
		const cVec3 &c = Centers[nCube];
		for(int nVertex = 0; nVertex < 8; nVertex++) {
			const cVec3 p = c + Shifts[nVertex];
			pVerts->x = p.x;
			pVerts->y = p.y;
			pVerts->z = p.z;
			pVerts->u = pVerts->v = 0.0f;
			pVerts->diffuse = Color.ToBgra();
			pVerts++;
		}
		const int StartIndex = 8 * nCube;
		for(int i = 0; i < 36; i++) {
			*pInds = StartIndex + Topology[i];
			pInds++;
		}
	}
	bm.setNVert(NVerts);
	bm.setNInd(3 * NTris);
	bm.setNPri(NTris);
	bm.setShader(idSh);
	bm.setTexture(idTex);
}

//--------------------------------------------------------------------------------------------------------------------------
// SelExt::GenRect
//--------------------------------------------------------------------------------------------------------------------------
void SelExt::GenRect(BaseMesh &bm, const cVec2 &lu, const cVec2 &wh, const cColor &Color, const int idTex, const int idSh) {
	bm.create(4, 6, vfVertexTnL);
	
	// Topology:
	word *pInds = bm.getIndices();
	int Topology[6] = {0, 1, 3, 1, 2, 3};
	for(int i = 0; i < 6; i++) {
		pInds[i] = Topology[i];
	}
	
	// Verts:
	VertexTnL *pVerts = (VertexTnL*)bm.getVertexData();
	float xPos[4] = { lu.x, lu.x, lu.x + wh.x, lu.x + wh.x };
	float yPos[4] = { lu.y, lu.y + wh.y, lu.y + wh.y, lu.y};
	float u[4] = { 0.f, 0.f, 1.f, 1.f };
	float v[4] = { 0.f, 1.f, 1.f, 0.f };
	for(int i = 0; i < 4; i++) {
		pVerts->x = xPos[i];
		pVerts->y = yPos[i];
		pVerts->u = u[i];
		pVerts->v = v[i];
		pVerts->w = 1.f;
		pVerts->diffuse = Color.ToBgra();
		pVerts++;
	}
	
	bm.setNInd(6);
	bm.setNVert(4);
	bm.setNPri(2);
	bm.setShader(idSh);
	bm.setTexture(idTex);
}
cVec2 SelExtArgs::GetUV(cVec3 n,cVec3 pos,int nFacture){
	cVec3 uvVec(0,0,1);
	duv=cVec2(0,0);
	if(IsSplined()){
        cVec2 p=TransformPointBySpline(pos);
		uvVec.x=p.x;
		uvVec.y=p.y;
		duv=cVec2(100,123);
	}else{
		int dx=abs(n.x);
		int dy=abs(n.y);
		int dz=abs(n.z);
		if(dz>dx && dz>dy){		
			uvVec.x=pos.x;
			uvVec.y=pos.y;
			duv=cVec2(200,-123);
		}else{
			if(dx>dy && dx>dz){
				uvVec.x=pos.y;
				uvVec.y=-pos.z;
				duv=cVec2(30,223);
			}else{
				uvVec.x=pos.x;
				uvVec.y=-pos.z;
				duv=cVec2(-127,345);
			}
		}
	}
	SurfMapper m(nFacture);
	uvVec.x*=m.uScale;
	uvVec.y*=m.vScale;
	cVec3 uv=cVec3::Transform(uvVec,uvTransform);
	return cVec2(uv.x,uv.y);
}
void SelExtArgs::ToDefault(){
	uvTransform=cMat3::Identity;
	uvTransform[0][0]=uvTransform[1][1]=1.0/256.0f;
	ClearAllPoints();
}
void SelExtArgs::ApplyScale(float s){
	cVec3 n(Center.nx,Center.ny,Center.nz);
	cVec3 p(Center.x,Center.y,Center.z);
    cVec2 uv=GetUV(n,p,CurrFacture);
	cMat3 TR1(1,0,0, 0,1,0, -uv.x,-uv.y,1);
	uvTransform*=TR1;
	cMat3 SC(s,0,0, 0,s,0, 0,0,1);
	uvTransform*=SC;
	cMat3 TR2(1,0,0, 0,1,0, uv.x,uv.y,1);
	uvTransform*=TR2;
}
void SelExtArgs::ApplyRotate(float a){
	cVec3 n(Center.nx,Center.ny,Center.nz);
	cVec3 p(Center.x,Center.y,Center.z);
	cVec2 uv=GetUV(n,p,CurrFacture);
	cMat3 TR1( 1,0,0  ,0,1,0  ,-uv.x,-uv.y,1 );
	uvTransform*=TR1;
	cMat3 ROT(cos(a),-sin(a),0,  sin(a),cos(a),0,  0,0,1);
	uvTransform*=ROT;cMat3 TR2( 1,0,0  ,0,1,0  ,uv.x,uv.y,1 );
	uvTransform*=TR2;    	
}
void SelExtArgs::ApplyShift(float du,float dv){
	cMat3 TR1( 1,0,0  ,0,1,0  ,du,dv,1 );	
	uvTransform*=TR1;
}
void SelExtArgs::CreateSpline(){
	if(PointsArray.Count()>1){
		cList<cSplineBezier3::Knot> P;
		cList<cSplineBezier3::Knot> N;
		Normals.Free();
		float L=0;
		cVec3 Prev=PointsArray[0];
		for(int i=0;i<PointsArray.Count();i++){
            cVec3 p=PointsArray[i];
			if(i>0){
				L+=(p-Prev).Length();
			}
			cSplineBezier3::Knot K;
			K.Point=p;
			K.Time=L;
			P.Add(K);
			K.Point=NormalsArray[i];
			N.Add(K);
            Prev=p;
		}
		for(int i=0;i<PointsArray.Count();i++){
			P[i].Time/=L;
			N[i].Time/=L;
			Normals.AddValue(N[i].Time,N[i].Point);
		}
		Points.SetFromKnots(P.ToPtr(),PointsArray.Count());
		SplineLength=L;
		Rpressed=false;
	}
}
void SelExtArgs::DrawSpline(){
	int n=PointsArray.Count();
	if(n){
		if(n>1){
			int np=(n-1)*20;
			cVec3 Prev;
			for(int i=0;i<=np;i++){
				float t=float(i)/np;
                cVec3 p=Points.GetValue(t);
				if(i>1){
					rsLine(*((Vector3D*)(&p)),*((Vector3D*)(&Prev)),0xFFFFFF00,0xFFFFFF00);
				}
				Prev=p;
            }			
		}
		for(int i=0;i<n;i++){
			cVec3 p=PointsArray[i];
			DrawSphere(Sphere(*((Vector3D*)(&p)),6),0xFFFF0000,0,8);
		}
		// Drawing Bezier spline segs:
		/*
		for(int nSeg = 0; nSeg < Points.GetNumSegments(); nSeg++) {
			const cSegBezier3 &Seg = Points.GetCurve(nSeg);
			for(int k = 0; k < 4; k++) {
				DrawSphere(Sphere(*((Vector3D *)(&Seg.GetPoint(k))), 8), 0xff0000ff, 0, 4);
			}
		}
		*/
		rsFlush();
	}
}
bool SelExtArgs::IsSplined(){
	return PointsArray.Count()>1;
}
cVec2 SelExtArgs::TransformPointBySpline(cVec3 p){
	cVec2 res(p.x,p.y);
	if(IsSplined()){
		static float Prec=0.0001;
		cSplineBezier3::Place place=Points.CalcClosestPointPlaceAccurate(p,Prec);
		float time=Points.GetTimeFromPlace(place);
		time=cMath::Clamp01(time);
        cVec3 nc=Normals.GetCurrentValue(time);
		cVec3 pos=Points.GetValue(time);
		cVec3 dir=Points.GetDerivativeGlobal(time);
		dir.Normalize();
		p-=pos;		
		if(time<=0){
			res.x=cVec3::Dot(dir,p);
		}else
		if(time>=1){
			res.x=SplineLength+cVec3::Dot(dir,p);
		}else res.x=time*SplineLength;
		
		cVec3 bin=cVec3::Cross(nc,dir);
		res.y=cVec3::Dot(p,bin);
	}
	return res;
}
void SelExtArgs::AddPoint(cVec3 n,cVec3 p){
	for(int i=0;i<PointsArray.Count();i++){
		float D=cVec3::Distance(PointsArray[i],p);
		if(D<40){
			PointsArray[i]=p;
			NormalsArray[i]=n;
			CreateSpline();
			return;
		}        
	}
	if(PointsArray.Count()==0 || cVec3::Distance(PointsArray[PointsArray.Count()-1],p)>5){
		PointsArray.Add(p);
		NormalsArray.Add(n);
		CreateSpline();
	}
}
void SelExtArgs::DelLast(){
	if(PointsArray.Count()){
		PointsArray.RemoveBack();
		NormalsArray.RemoveBack();
		CreateSpline();
	}
}
void SelExtArgs::ClearAllPoints(){
    PointsArray.Clear();
	NormalsArray.Clear();
}
} // Surface