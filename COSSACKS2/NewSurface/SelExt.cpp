#include "SelExt.h"
#include "NavExt.h"

int g_SelRadius = 4 * ExtOptions::dRadius;

// SelExt
SelExt::SelExt() {
	m_fActive = false;
	extern UniversalMap g_UniversalMap;
	m_pUniversalMap = &g_UniversalMap;
	m_pSelRadius = &g_SelRadius;
} // SelExt

// SelExt::CheckActivityOfEditor
bool SelExt::CheckActivityOfEditor() {
	return m_fActive;
} // SelExt::CheckActivityOfEditor

// SelExt::ClearActivityOfEditor
void SelExt::ClearActivityOfEditor() {
	g_fSpaceIsBlocked = false;
	m_fActive = false;
	OnClear();
} // SelExt::ClearActivityOfEditor

// SelExt::ActivateEditor
void SelExt::ActivateEditor() {
	g_fSpaceIsBlocked = true;
	m_Params.RefTo.Free();
	m_Params.VertsTo.Free();
	m_fActive = true;
	OnActivate();
} // SelExt::ActivateEditor

// SelExt::OnMouseWheel
bool SelExt::OnMouseWheel(int Delta) {
	if(!m_fActive) return false;
	*m_pSelRadius += (Delta > 0 ? ExtOptions::dRadius : - ExtOptions::dRadius);
	if(*m_pSelRadius < 0) *m_pSelRadius = 0;
	m_Params.Radius = *m_pSelRadius;
	return true;
} // SelExt::OnMouseWheel

// SelExt::OnMouseHandling
bool SelExt::OnMouseHandling(int sx, int sy, bool &LMB, bool &RMB, int, int, bool OverMiniMap) {
	if(!m_fActive || g_NavExt.Args.fNavMode || OverMiniMap) return false;

	UniversalMap::PickArgs Args(sx, sy, *m_pSelRadius);
	m_pUniversalMap->Pick(&Args);


	m_Params.Radius = *m_pSelRadius;
	m_Params.RefTo = Args.RefTo;
	m_Params.VertsTo = Args.VertsTo;
	if(LMB || RMB) {
		DWORD Flags = 0;
		if(LMB) Flags = OnLMB(m_Params);
		else if(RMB) Flags = OnRMB(m_Params);
		if(Flags & IF_CACHE) {
			if(Flags & IF_GEOMETRY && Args.VertNums.Count()) {
				// reCalcing Normals && Shading (Local Illumination):
				cVectors TriNormals;
				Args.Ref.CalcTriNormals(TriNormals);
				cVectors VertNormals;
				VertNormals.Lock(Args.VertNums.Count());
				int i;
				for(int k = 0; k < Args.Tris.Count(); k++) {
					i = Args.Tris[k];
					if(Args.IsInval[i]) {
						VertNormals[i] += TriNormals[k / 3];
					}
				}
				for(k = 0; k < Args.VertNums.Count(); k++) {
					if(Args.IsInval[k]) {
						OneMapVertex &vtx = m_pUniversalMap->Vert[Args.VertNums[k]];
						cVector &N = VertNormals[k];
						N.Normalize();
						N *= 127;
						vtx.nx = N.x, vtx.ny = N.y, vtx.nz = N.z;
						m_pUniversalMap->m_Shader.ShadeOneVertex(&vtx);
					}
				}
				Args.Ref.FillMapOfHeights();
				for(i = 0; i < Args.QuadNums.Count(); i++) {
					m_pUniversalMap->m_pQuads[Args.QuadNums[i]]->Init();
				}
			}
			// Invalidating cache (just extracted quads):
			int x0, y0, x1, y1;
			m_pUniversalMap->QuadsTotalBRect(Args.QuadNums, x0, y0, x1, y1);
			ITerra->InvalidateGeometry(&Rct(x0, y0, x1, y1));
		}
		return true;
	}
	return false;
} // SelExt::OnMouseHandling

// SelExt::OnDrawOnMapOverAll
void SelExt::OnDrawOnMapOverAll() {
	if(!m_fActive) return;
	OnDraw(m_Params);
} // SelExt::OnDrawOnMapOverAll

// SelExt::DrawEditorInterface
void SelExt::DrawEditorInterface() {
	if(!m_fActive) return;
	OnDrawInterface(m_Params);
} // SelExt::DrawEditorInterface

// SelExt::DrawWindow
void SelExt::DrawWindow(int x0, int y0, int x1, int y1, DWORD argbColor, int Border) {
	rsRect(Rct(x0, y0, x1 - x0, Border), 0.f, argbColor);
	rsRect(Rct(x0, y1 - Border, x1 - x0, Border), 0.f, argbColor);
	rsRect(Rct(x0, y0 + Border, Border, y1 - y0 - 2 * Border), 0.f, argbColor);
	rsRect(Rct(x1 - Border, y0 + Border, Border, y1 - y0 - 2 * Border), 0.f, argbColor);
} // SelExt::DrawWindow

// SelExt::DrawGrid
void SelExt::DrawGrid(const VertexBasedMeshRef &Ref, DWORD argbColor) {
	if(!Ref.m_TriNums.NValues) return;
	OneSplitMesh Splitted;
	Splitted.Split(Ref, 1);
	Splitted.Modify(OneMapVertex::Origin, 0, argbColor & 0x30FFFFFF, 0, OneSplitMesh::MF_AND | OneSplitMesh::MF_OR);
    Splitted.Draw();
	Splitted.Split(Ref, 2);
	Splitted.Modify(OneMapVertex::Origin, 0, argbColor, 0, OneSplitMesh::MF_AND | OneSplitMesh::MF_OR);
	Splitted.Draw();
} // SelExt::DrawGrid