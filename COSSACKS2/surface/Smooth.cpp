#include "stdheader.h"
#include "Smooth.h"
#include "Deform.h"
#include "..\RoadsAndFactures.h"

extern ClonesArray<OneFactureInfo> FacturesList;;

namespace Surface {

//-----------------------------------------------------------------------------
// GetDMIdFromIndex
//-----------------------------------------------------------------------------
int Smooth::GetDMIdFromIndex(int index) {
	if(index < 0) return -1;
	Enumerator *E = ENUM.Get("DISPLACEMENTMAPS");
	assert(E);
	int N = E->GetAmount();
	if(index >= N) return -1;

	cStr Path = "Textures\\Displacement Maps\\";
	Path.Append(E->GetStr(index));

	return IRS->GetTextureID(Path);
}

//-----------------------------------------------------------------------------
// ToBaseMesh
//-----------------------------------------------------------------------------
bool Smooth::ToBaseMesh(const SurfMesh &sm, BaseMesh &bm) const {
	if(!sm.VERTS.Count()) return false;
	int id = GetDMIdFromIndex(m_SwatchCtrl.CurSel());
	if(-1 == id) return false;

	bm.create(sm.VERTS.Count(), sm.TRIS.Count(), vfVERTEX);
	bm.setNVert(sm.VERTS.Count());
	bm.setNInd(sm.TRIS.Count());
	bm.setNPri(sm.NTris());

	// TRIS:
	WORD *pTris = (WORD *)bm.getIndices();
	for(int i = 0; i < sm.TRIS.Count(); i++) {
		*pTris = sm.TRIS[i];
		pTris++;
	}

	// VERTS:
	VERTEX *pVerts = (VERTEX*)bm.getVertexData();
	assert(pVerts);

	uv_pair uv;
	for(int i = 0; i < sm.VERTS.Count(); i++) {
		const SurfVertex &u = sm.VERTS[i];
		pVerts->x = u.x, pVerts->y = u.y, pVerts->z = u.z;
		pVerts->diffuse = 0x30ffffff;
		CalcUV(u, uv);
		pVerts->u = uv.u, pVerts->v = uv.v;
		pVerts++;
	}
	bm.setShader(SurfFx(SFX_DEFAULT));
	bm.setTexture(id);
	return true;
}

//-----------------------------------------------------------------------------
// CalcUV
//-----------------------------------------------------------------------------
void Smooth::CalcUV(const SurfVertex &r, uv_pair &uv) const {
	uv.u = uv.v = 0.0f;
	int id = GetDMIdFromIndex(m_SwatchCtrl.CurSel());
	assert(id > -1);

	SurfMapper M;
	M.uScale = M.vScale = m_DMScale;
	M.fWidth = IRS->GetTextureWidth(id);
	M.fHeight = IRS->GetTextureHeight(id);
	int dx = - m_DMCenter.x + M.uScale * float(M.fWidth) / 2.0f;
	int dy = - m_DMCenter.y + M.vScale * float(M.fHeight) / 2.0f;
	int dz = - m_DMCenter.z + M.vScale * float(M.fHeight) / 2.0f;

	SurfVertex t = r;
	t.x += dx, t.y += dy, t.z += dz;
	uv = M.CalcMappingUV(t, SurfMapper::CalcMappingCode(t));
	if(t.MappingType() & MT_HORIZONTAL) uv.v *= 2.0f;
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	dMatrix T = dMatrix::OfRotation(dVector::zAxis, m_DMAngle);
	dVector u(uv.u - 0.5f, uv.v - 0.5f);
	u *= T;
	uv.u = u.x + 0.5f, uv.v = u.y + 0.5f;

	if(uv.u < 0) {
		uv.u = int(fabs(uv.u) + 0.5f) - fabs(uv.u);
	}
	if(uv.v < 0) {
		uv.v = int(fabs(uv.v) + 0.5f) - fabs(uv.v);
	}
	uv.u -= int(uv.u);
	uv.v -= int(uv.v);
}

bool SmoothRMBActive = false;
//-----------------------------------------------------------------------------
// OnRMB
//-----------------------------------------------------------------------------
DWORD Smooth::OnRMB(SelExtArgs &Args) {
	if(m_SwatchCtrl.Active()) return 0;

	if(!Args.Ref.idVerts.Count()) return 0;

	SurfaceUndo::DropRefToUndo(Args.Ref);

	dVector Orig, Dir;
	bool fLocalNormal = GetKeyState(VK_SHIFT) < 0;
	if(fLocalNormal && !Deform::ApproxByPlane(Args.Ref, Orig, Dir)) return 0;

	if(!SmoothRMBActive) {
		m_DMCenter = Args.Center;
		SmoothRMBActive = true;
	}

	const int xOffset[] = { -1, 1, -1, 1, -1, 1, 0, 0, cMath::IntMaxValue };
	const int yOffset[] = { -1, -1, 1, 1, 0, 0, -1, 1, cMath::IntMaxValue };

	m_nSelectedMap = m_SwatchCtrl.CurSel();
	int id = GetDMIdFromIndex(m_nSelectedMap);
	if(-1 == id) return 0;
	
	int W = IRS->GetTextureWidth(id);
	int H = IRS->GetTextureHeight(id);
	
	int p;
	BYTE *pBits = IRS->LockTexBits(id, p);
	uv_pair uv;
	if(pBits && W > 0 && H > 0 && Args.Ref.idVerts.Count()) {
		for(int i = 0; i < Args.Ref.idVerts.Count(); i++) {
			SurfVertex &u = Args.Ref.pOrig->VERTS[Args.Ref.idVerts[i]];
			
			CalcUV(u, uv);
			int x = int((float)W * uv.u);
			int y = int((float)H * uv.v);
			int n = 1;
			float l = float(pBits[x + p * y]);
			for(int k = 0; xOffset[k] != cMath::IntMaxValue && yOffset[k] != cMath::IntMaxValue; k++) {
				int tx = x + xOffset[k];
				int ty = y + yOffset[k];
				if(tx >= 0 && tx < W && ty >= 0 && ty < H) {
					l += float(pBits[tx + p * ty]);
					n++;
				}
			}
			l /= (float)n;
			dVector N(u.nx, u.ny, u.nz);
			N.Normalize();
			if(fLocalNormal) {
				float d = N.Dot(Dir);
				if(fLocalNormal) N = Dir;
				if(d < 0) N *= -1;
			}
			if(!N.IsZero()) {
				dVector Pos(u.x, u.y, u.z);

				Pos = Pos + (float(l) / 255.0f) * g_SurfEditor.Smooth.MAPAMOUNT * N;
				u.x = short(Pos.x), u.y = short(Pos.y), u.z = short(Pos.z);
			}
		}
	}
	IRS->UnlockTexBits(id);
	return IF_GEOMETRY;
}

bool SmoothLMBWasPressed = false;
//-----------------------------------------------------------------------------
// OnLMB
//-----------------------------------------------------------------------------
DWORD Smooth::OnLMB(SelExtArgs &Args) {
	if(m_SwatchCtrl.Active()) return 0;

	bool fCtrl = GetKeyState(VK_CONTROL) < 0;
	bool fShift = GetKeyState(VK_SHIFT) < 0;

	SurfaceUndo::DropRefToUndo(Args.Ref);

	if(fShift) {
		if(!SmoothLMBWasPressed) {
			SmoothLMBWasPressed = true;
			SurfRef Ref;
			g_UniversalMap.CutRefCylinder(Ref, Args.Center, Args.Radius, SR_TRIS);			
			SurfMesh Mesh;
			Ref.ToSurfMesh(Mesh);
			if(fCtrl) {
				Mesh.ApproximateGenerator();
			} else {
				SimplifySurface(Mesh);
			}
			g_UniversalMap.Sum(Mesh);
			g_UniversalMap.QUADS.Compile();
			Args.Ref.idVerts.Free(), Args.Ref.idTris.Free();
			return IF_CACHE;
		}
	} else if(fCtrl) {
		if(!SmoothLMBWasPressed) {
			SmoothLMBWasPressed = true;
			Args.Ref.Subdivide();
		}
	} else Args.Ref.Relax(g_SurfEditor.Smooth.AMOUNT);
	return IF_GEOMETRY;
}

//-----------------------------------------------------------------------------
// OnDraw
//-----------------------------------------------------------------------------
void Smooth::OnDraw(SelExtArgs &Args) {
	if(m_SwatchCtrl.Active()) return;

	if(!Lpressed) SmoothLMBWasPressed = false;
	if(!Rpressed) SmoothRMBActive = false;
	SelExt::DrawGrid(Args.Ref, cColor::Cyan.ToBgra());
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	m_nSelectedMap = m_SwatchCtrl.CurSel();
	int id = GetDMIdFromIndex(m_nSelectedMap);
	if(id > -1) {
		if(!SmoothRMBActive) {
			m_DMCenter = Args.Center;
		}

		SurfMesh sm;
		SurfRef sr(Args.Ref);
		sr.ToSurfMesh(sm);
		BaseMesh bm;
		if(ToBaseMesh(sm, bm)) {
			DrawBM(bm);
		}
	}
}

//-----------------------------------------------------------------------------
// OnActivate
//-----------------------------------------------------------------------------
void Smooth::OnActivate() {
	m_SwatchCtrl.Init(SwatchCtrlCallback, m_nSelectedMap, VK_SPACE, true, true);
}

//-----------------------------------------------------------------------------
// SwatchCtrlCallback
//-----------------------------------------------------------------------------
bool Smooth::SwatchCtrlCallback(int index, int &idTex) {
	idTex = GetDMIdFromIndex(index);
	return idTex != -1 ? true : false;
}


//-----------------------------------------------------------------------------
// OnDrawInterface
//-----------------------------------------------------------------------------
void Smooth::OnDrawInterface() {
	m_SwatchCtrl.OnDrawInterface();
}

//-----------------------------------------------------------------------------
// OnWheel
//-----------------------------------------------------------------------------
bool Smooth::OnWheel(int nDelta) {
	const float cMinScale = 1.0f;
	const float cMaxScale = 30.0f;
	const float cScaleMul = 0.1f;
	if(GetKeyState(VK_MENU) < 0 && m_SwatchCtrl.CurSel() > -1) {
		float d = m_DMScale * cScaleMul;
		m_DMScale += cMath::Sign(nDelta) * d;
		m_DMScale = cMath::Clamp(m_DMScale, cMinScale, cMaxScale);
		return true;
	}
	const float cAngleDelta = cMath::Rad(3);
	if(GetKeyState(VK_CONTROL) < 0 && m_SwatchCtrl.CurSel() > -1) {
		m_DMAngle -= cMath::Sign(nDelta) * cAngleDelta;
		return true;
	}
	return false;
}

} // Surface