#include "stdheader.h"
#include "Facturize.h"

namespace Surface {

//-----------------------------------------------------------------------------------------
// Process : void(SurfRef &, int, const SurfVertex &, int)
//-----------------------------------------------------------------------------------------
void Facturize::Process(SurfRef &Ref, int nFacture, SelExtArgs* Args) {
	assert(Ref.pOrig);
	Args->CurrFacture = nFacture;
    int SurfType = Args->Center.SurfaceType;
	eMappingType MappingType = SelExt::QueryMappingTypeKeys();
	for(int i = 0; i < Ref.idVerts.Count(); i++) {
		SurfVertex &r = Ref.pOrig->VERTS[Ref.idVerts[i]];
		if(MappingType & r.MappingType()) {
			if(SelExt::IsSelected(r, Args->Center, Args->Radius) && SurfType == r.SurfaceType) {
				r.nFacture = nFacture;
				if(!Args->PreserveUV){
					cVec2 uv=Args->GetUV(cVec3(r.nx,r.ny,r.nz),cVec3(r.x,r.y,r.z),nFacture)+Args->duv;
					r.u=uv.x;
					r.v=uv.y;					
				}
			}
		}
	}	
	Ref.UpdateNormals(false);
}

//-----------------------------------------------------------------------------
// OnLMB
//-----------------------------------------------------------------------------
DWORD Facturize::OnLMB(SelExtArgs &Args) {
	if(m_Swatch.Active()) return 0;
	static int PrevR=0;
	static cVec3 PrevV(0);
	if(PrevR==Args.Radius && PrevV==Args.Center.Pos())return 0;
	PrevR=Args.Radius;
	PrevV=Args.Center.Pos();    
	int N = GetNFactures();
	if(m_nCurFacture > N) {
		m_nCurFacture = N;
	}
	m_nCurFacture = m_Swatch.CurSel();
	SurfaceUndo::DropRefToUndo(Args.Ref);
	Process(Args.Ref, m_nCurFacture,&Args);
	return SelExt::IF_CACHE;
}

//-----------------------------------------------------------------------------
// OnRMB
//-----------------------------------------------------------------------------
DWORD Facturize::OnRMB(SelExtArgs &Args) {
	if(GetKeyState(VK_CONTROL)&0x8000){
		cVec3 n(Args.Center.nx,Args.Center.ny,Args.Center.nz);		
		n.Normalize();
        Args.AddPoint(n,Args.IntersectionPoint);
	}else{
		m_nCurFacture = Args.Center.nFacture;
		m_Swatch.CurSel(m_nCurFacture);
	}
	return 0;
}

//-----------------------------------------------------------------------------
// OnDraw
//-----------------------------------------------------------------------------
void Facturize::OnDraw(SelExtArgs &Args) {
	if(m_Swatch.Active()) return;
	SurfMesh sm;
	SurfRef sr(Args.Ref);
	sr.ToSurfMesh(sm);
	sm.GetRefAll(sr, SR_VERTS|SR_TRIS);
	m_nCurFacture = m_Swatch.CurSel();
	Process(sr, m_nCurFacture, &Args);
	SelExt::DrawSwatch(sm);
	if(LastKey==VK_BACK){
		Args.DelLast();
		LastKey=0;
	}
	Args.DrawSpline();
}

//-----------------------------------------------------------------------------
// OnActivate
//-----------------------------------------------------------------------------
void Facturize::OnActivate() {
    m_Args.LimitInSurfaceType = true;
	m_Args.ClearAllPoints();
	m_Swatch.Init(SwatchCallBack, m_nCurFacture);
}

//-----------------------------------------------------------------------------
// OnDrawInterface
//-----------------------------------------------------------------------------
void Facturize::OnDrawInterface() {
	m_Swatch.OnDrawInterface();
}

//-----------------------------------------------------------------------------
// SwatchCallBack
//-----------------------------------------------------------------------------
bool Facturize::SwatchCallBack(int index, int &idTex) {
	if(index >= GetNFactures()) return false;
	idTex = SurfVertex::GetFactureID(index);
	return true;
}

} // Surface