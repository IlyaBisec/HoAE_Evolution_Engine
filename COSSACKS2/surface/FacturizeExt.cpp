#include "FacturizeExt.h"

const char * GetFactureName(int);
int GetNFactures();

// FacturizeExt::FacturizeOneVertex
void FacturizeExt::FacturizeOneVertex(OneMapVertex &vtx, int nFacture, eFactType FactType) {
	if(FactType == FT_CUBIC) {
		vtx.nFacture = nFacture;
	} else {
		DWORD MappingCode = TerraMapper::CalcMappingCode(vtx);
		if(FactType == FT_VERTICAL) {
			switch(MappingCode) {
				case TerraMapper::MC_FRONT:
				case TerraMapper::MC_BACK:
				case TerraMapper::MC_LEFT:
				case TerraMapper::MC_RIGHT:
					vtx.nFacture = nFacture;
			}
		} else if(FactType == FT_HORIZONTAL) {
			switch(MappingCode) {
				case TerraMapper::MC_TOP:
				case TerraMapper::MC_BOTTOM:
					vtx.nFacture = nFacture;
			}
		}
	}
} // FacturizeExt::FacturizeOneVertex

// FacturizeExt::GetKeyFTState
eFactType FacturizeExt::GetKeyFTState() {
	bool fHorizontal = GetKeyState(VK_CONTROL) < 0;
	bool fVertical = GetKeyState(VK_SHIFT) < 0;
	if(fVertical) return FT_VERTICAL;
	else if(fHorizontal) return FT_HORIZONTAL;
	else return FT_CUBIC;
} // FacturizeExt::GetKeyFTState

int feCurFacture = 0;
bool fefInInterface = false;
// FacturizeExt::OnLMB
DWORD FacturizeExt::OnLMB(SelExtParams &Params) {
	if(!Params.VertsTo.Count()) return 0;
	if(fefInInterface) return 0;
	int N = GetNFactures();
	if(feCurFacture > N) {
		feCurFacture = N;
	}
	eFactType FactType = GetKeyFTState();
	for(int i = 0; i < Params.VertsTo.Count(); i++) {
		OneMapVertex &r = Params.RefTo.pOrigin->Vert[Params.VertsTo[i]];
		FacturizeOneVertex(r, feCurFacture, FactType);
	}
	return SelExt::IF_CACHE;
} // FacturizeExt::OnLMB

// FacturizeExt::OnRMB
DWORD FacturizeExt::OnRMB(SelExtParams &Params) {
	if(!Params.VertsTo.Count()) return 0;
	const OneMapVertex &c = Params.RefTo.pOrigin->Vert[Params.VertsTo[0]];
	ActivateFacture(c.nFacture);
	return 0;
} // FacturizeExt::OnRMB

// FacturizeExt::GenOneSample
void FacturizeExt::GenOneSample(int x, int y, int L, int nFacture, feSample &Sample) {
	static int shSwatch = IRS->GetShaderID("terra_swatch");
	static int shAux = IRS->GetShaderID("terra_aux");

	// Swatch:
	Sample.bmSample.create(4, 6, vfVertexTnL);
	
	// Topology:
	word *pInds = Sample.bmSample.getIndices();
	int Topology[6] = {0, 1, 3, 1, 2, 3};
	for(int i = 0; i < 6; i++) {
		pInds[i] = Topology[i];
	}

	// Verts:
	VertexTnL *pVerts = (VertexTnL*)Sample.bmSample.getVertexData();
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

	Sample.bmSample.setNInd(6);
	Sample.bmSample.setNVert(4);
	Sample.bmSample.setNPri(2);
	Sample.bmInfoBase = Sample.bmSample;
	Sample.bmInfoBase.setShader(shAux);
	Sample.bmSample.setTexture(OneMapVertex::GetFactureID(nFacture));
	Sample.bmSample.setShader(shSwatch);

	// InfoBase:
	pVerts = (VertexTnL*)Sample.bmInfoBase.getVertexData();
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
	int W = IRS->GetTextureWidth(GetFacture(nFacture));
	int H = IRS->GetTextureHeight(GetFacture(nFacture));
	Sample.Info.WxH = "(";
	Sample.Info.WxH += W;
	Sample.Info.WxH += "x";
	Sample.Info.WxH += H;
	Sample.Info.WxH += ")";
	// Title:
	const char *pStrPath = GetFactureName(nFacture);
	if(pStrPath) {
		char *pLastBS = strrchr(pStrPath, '\\');
		if(pLastBS) {
			pLastBS++;
			Sample.Info.Title = pLastBS;
		} else Sample.Info.Title = pStrPath;
	}

	Sample.x0 = x, Sample.y0 = y;
	Sample.x1 = x + L, Sample.y1 = y + L;
} // FacturizeExt::GenOneSample

// FacturizeExt::GenSwatch
void FacturizeExt::GenSwatch() {
	int N = GetNFactures();
	if(!N) return;
	// In line:
	double d = sqrt((double)N);
	int NumInLine = (int)d;
	if(double(NumInLine) < d) NumInLine++;
	// Dims:
	int cx = RealLx / 2, cy = RealLy / 2;
	const int Offset = 20;
	int LenInLine = RealLy - 2 * Offset;
	int LxL = LenInLine / NumInLine;
	int X0 = cx - LenInLine / 2;
	int Y0 = cy - LenInLine / 2;
	feSample S;
	for(int i = 0; i < N; i++) {
		int x = X0 + LxL * (i % NumInLine);
		int y = Y0 + LxL * (i / NumInLine);
		GenOneSample(x, y, LxL, i, S);
		m_Swatch.Samples.Add(S);
	}
	// Active Sample:
	GenOneSample(RealLx - ExtOptions::IconLxL - ExtOptions::IconRightOffset,
		ExtOptions::IconTopOffset, ExtOptions::IconLxL, feCurFacture, m_Swatch.ActiveSample);
} // FacturizeExt::GenSwatch

// FacturizeExt::DrawOneSample
void FacturizeExt::DrawOneSample(feSample &Sample) {
	// Fonts:
	static RLCFont *pFont = &SmallWhiteFont;
	static int hFont = GetRLCHeight(pFont->RLC, 'W');

	DrawBM(Sample.bmSample);
	DrawBM(Sample.bmInfoBase);
	// Info base:
	int xText = Sample.x0 + ExtOptions::dBorder;
	ShowString(xText, Sample.y0, Sample.Info.Title, pFont);
	ShowString(xText, Sample.y0 + hFont, Sample.Info.WxH, pFont);
} // FacturizeExt::DrawOneSample

// FacturizeExt::DrawSwatch
void FacturizeExt::DrawSwatch(int nCurSel) {
	for(int i = 0; i < m_Swatch.Samples.Count(); i++) {
		DrawOneSample(m_Swatch.Samples[i]);
	}
	const feSample &Cur = m_Swatch.Samples[feCurFacture];
	SelExt::DrawWindow(Cur.x0 - 1, Cur.y0 - 1, Cur.x1 + 1, Cur.y1 + 1, comms::argbWhite, ExtOptions::dBorder);
	if(nCurSel >= 0 && nCurSel < m_Swatch.Samples.Count()) {
		const feSample &Sel = m_Swatch.Samples[nCurSel];
		SelExt::DrawWindow(Sel.x0, Sel.y0, Sel.x1, Sel.y1, comms::argbRed, ExtOptions::dBorder);
	}
} // FacturizeExt::DrawSwatch

// FacturizeExt::OnDraw
void FacturizeExt::OnDraw(SelExtParams &Params) {
	if(fefInInterface) return;
	if(!Params.VertsTo.Count()) return;
	OneSplitMesh osm;
	VertexBasedMesh vbm;
	VertexBasedMeshRef Ref;
	if(!Params.RefTo.m_TriNums.NValues) {
		OneMapVertex c = Params.RefTo.pOrigin->Vert[Params.VertsTo[0]];
		c.nFacture = feCurFacture;
		vbm.GenMarker(c);
	} else {
		Params.RefTo.ConvertTo(vbm);
		eFactType FactType = GetKeyFTState();
		for(int i = 0; i < vbm.Vert.NValues; i++) {
			FacturizeOneVertex(vbm.Vert[i], feCurFacture, FactType);
		}
	}
	vbm.GetRefAll(Ref);
	osm.Split(Ref);
	osm.Draw();
} // FacturizeExt::OnDraw

// FactureExt::GetCurSel
int FacturizeExt::GetCurSel() {
	POINT pt;
	GetCursorPos(&pt);
	for(int i = 0; i < m_Swatch.Samples.Count(); i++) {
		const feSample &r = m_Swatch.Samples[i];
		if(r.IsPicked(pt.x, pt.y)) return i;
	}
	return -1;
} // FacturizeExt::GetCurSel

// FacturizeExt::OnActivate
void FacturizeExt::OnActivate() {
	GenSwatch();
} // FacturizeExt::OnActivate

// FacturizeExt::OnClear
void FacturizeExt::OnClear() {
	m_Swatch.Samples.Free();
} // FacturizeExt::OnClear

// FacturizeExt::ActivateFacture
void FacturizeExt::ActivateFacture(int nFacture) {
	feCurFacture = nFacture;
	feSample &A = m_Swatch.ActiveSample;
	const feSample &r = m_Swatch.Samples[feCurFacture];
	A.Info = r.Info;
	A.bmSample.setTexture(OneMapVertex::GetFactureID(feCurFacture));
} // FacturizeExt::ActivateFacture

// FacturizeExt::OnDrawInterface
void FacturizeExt::OnDrawInterface(SelExtParams &Params) {
	// Draw ActiveSample:
	feSample &A = m_Swatch.ActiveSample;
	DrawOneSample(A);
	SelExt::DrawWindow(A.x0 - ExtOptions::dBorder, A.y0 - ExtOptions::dBorder,
		A.x1 + ExtOptions::dBorder, A.y1 + ExtOptions::dBorder, comms::argbBlack, ExtOptions::dBorder);

	bool fIsPressed = GetKeyState(VK_SPACE) < 0;
	static int nCurSel = -1;
	if(fIsPressed) {
		nCurSel = GetCurSel();
	}
	if(!fIsPressed && fefInInterface) {
		if(nCurSel != -1) {
			ActivateFacture(nCurSel);
		}
		fefInInterface = false;
	}
	if(fIsPressed && !fefInInterface) {
		fefInInterface = true;
	}
	if(fIsPressed) {
		DrawSwatch(nCurSel);
	}
} // FacturizeExt::OnDrawInterface