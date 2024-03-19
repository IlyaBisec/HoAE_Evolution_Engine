#include "stdheader.h"
#include "mLodder.h"
#include "Surface.h"
#include "ITerrain.h"
#include "SurfSplit.h"

namespace Surface {

//-------------------------------------------------------------------------------------
// AddCube
//-------------------------------------------------------------------------------------
void AddCube(SurfMesh &M, const cVec3 &Center, const cVec3 &Size, const cColor &Color) {
	const int StartIndex = M.VERTS.Count();
	SurfVertex u;
	u.Color = Color.ToBgra();
	u.Luminance = 255;
	
	const int Topology[36] = {
		0, 2, 1,
		0, 3, 2,
		2, 3, 6,
		3, 7, 6,
		6, 7, 4,
		6, 4, 5,
		0, 1, 5,
		0, 5, 4,
		1, 2, 6,
		1, 6, 5,
		3, 0, 7,
		0, 4, 7
	};

	const cVec3 h = 0.5f * Size;
	const cVec3 Shifts[8] = {
		cVec3(-h.x, -h.y, -h.z),
		cVec3(-h.x, h.y, -h.z),
		cVec3(h.x, h.y, -h.z),
		cVec3(h.x, -h.y, -h.z),
		cVec3(-h.x, -h.y, h.z),
		cVec3(-h.x, h.y, h.z),
		cVec3(h.x, h.y, h.z),
		cVec3(h.x, -h.y, h.z)
	};
	
	// Appending:
	for(int i = 0; i < 36; i++) {
		const int i0 = Topology[i];
		const cVec3 p = Center + Shifts[i0];
		u.x = p.x, u.y = p.y, u.z = p.z;
		M.VERTS.Add(u);
		M.TRIS.Add(StartIndex + i);
	}
} // AddCube

// AddMarker
void AddMarker(SurfMesh &M, const SurfVertex &l0, const SurfVertex &l1, int d, DWORD argbColor) {
	int V = M.VERTS.Count();
	M.VERTS.Add(SurfVertex(l0.x - d / 2, l0.y, l0.z, argbColor, 255));
	M.VERTS.Add(SurfVertex(l1.x - d / 2, l1.y, l1.z, argbColor, 255));
	M.VERTS.Add(SurfVertex(l1.x + d / 2, l1.y, l1.z, argbColor, 255));

	M.VERTS.Add(SurfVertex(l0.x - d / 2, l0.y, l0.z, argbColor, 255));
	M.VERTS.Add(SurfVertex(l1.x + d / 2, l1.y, l1.z, argbColor, 255));
	M.VERTS.Add(SurfVertex(l0.x + d / 2, l0.y, l0.z, argbColor, 255));

	M.VERTS.Add(SurfVertex(l0.x, l0.y - d / 2, l0.z, argbColor, 255));
	M.VERTS.Add(SurfVertex(l1.x, l1.y - d / 2, l1.z, argbColor, 255));
	M.VERTS.Add(SurfVertex(l1.x, l1.y + d / 2, l1.z, argbColor, 255));

	M.VERTS.Add(SurfVertex(l0.x, l0.y - d / 2, l0.z, argbColor, 255));
	M.VERTS.Add(SurfVertex(l1.x, l1.y + d / 2, l1.z, argbColor, 255));
	M.VERTS.Add(SurfVertex(l0.x, l0.y + d / 2, l0.z, argbColor, 255));
	for(int iTri = 0; iTri < 4; iTri++) {
		int i0 = V + 3 * iTri;
		M.AddTri(i0, i0 + 1, i0 + 2);
	}
}

// GenMarkers
void GenMarkers(SurfMesh &MeshOfMarkers, const Rct &exRect) {
	int x0 = int(exRect.x), y0 = int(exRect.y), w = int(exRect.w), h = int(exRect.h);
	const int d = 10, dh = 10;
	for(int ny = 0; ny < MaxTH; ny++)
		for(int nx = 0; nx < VertInLine; nx++) {
			int x = 32 * nx, y = 32 * ny;
			if(nx % 2) y -= 16;
			if(x < x0 || x >= x0 + w || y < y0 || y >= y0 + h) continue;
			short h = THMap[nx + VertInLine * ny];
			AddMarker(MeshOfMarkers, SurfVertex(x, y, h), SurfVertex(x, y, h + dh), d, cColor::Cyan.ToBgra());
		}
}

// AddLine
void AddLine(SurfMesh &M, const SurfVertex &l0, const SurfVertex &l1, DWORD argbColor, int d = 1) {
	int V = M.VERTS.Count();
	M.VERTS.Add(SurfVertex(l0.x, l0.y, l0.z, argbColor, 255));
	M.VERTS.Add(SurfVertex(l1.x, l1.y, l1.z, argbColor, 255));
	M.VERTS.Add(SurfVertex(l1.x + d, l1.y, l1.z, argbColor, 255));

	M.VERTS.Add(SurfVertex(l0.x, l0.y, l0.z, argbColor, 255));
	M.VERTS.Add(SurfVertex(l1.x + d, l1.y, l1.z, argbColor, 255));
	M.VERTS.Add(SurfVertex(l0.x + d, l0.y, l0.z, argbColor, 255));

	M.VERTS.Add(SurfVertex(l0.x, l0.y, l0.z, argbColor, 255));
	M.VERTS.Add(SurfVertex(l1.x, l1.y, l1.z, argbColor, 255));
	M.VERTS.Add(SurfVertex(l1.x, l1.y + d, l1.z, argbColor, 255));

	M.VERTS.Add(SurfVertex(l0.x, l0.y, l0.z, argbColor, 255));
	M.VERTS.Add(SurfVertex(l1.x, l1.y + d, l1.z, argbColor, 255));
	M.VERTS.Add(SurfVertex(l0.x, l0.y + d, l0.z, argbColor, 255));
	for(int iTri = 0; iTri < 4; iTri++) {
		int i0 = V + 3 * iTri;
		M.AddTri(i0, i0 + 1, i0 + 2);
	}
}

const int SurfNormalsLen = 30;
// GenTriNormals
void GenTriNormals(SurfMesh &MeshOfNormals,  SurfRef &Ref) {
	SurfVertex c;
	dVector N, C, E;
	for(int iTri = 0; iTri < Ref.idTris.Count(); iTri++) {
		int nTri = Ref.idTris[iTri];
		Ref.pOrig->GetTri(nTri, c);
		C.Set(c.x, c.y, c.z);
		Ref.pOrig->GetTri(nTri, N);
		E = C + SurfNormalsLen * N;
		AddLine(MeshOfNormals, SurfVertex(C.x, C.y, C.z), SurfVertex(E.x, E.y, E.z), cColor::Blue.ToBgra());
	}
}

// GenVertNormals
void GenVertNormals(SurfMesh &MeshOfNormals, SurfRef &Ref) {
	Ref.Complete(SR_VERTS, SR_TRIS);
	for(int i = 0; i < Ref.idVerts.Count(); i++) {
		const SurfVertex &v = Ref.pOrig->VERTS[Ref.idVerts[i]];
		dVector C(v.x, v.y, v.z);
		dVector N(v.nx, v.ny, v.nz);
		N.Normalize();
		dVector E = C + SurfNormalsLen * N;
		AddLine(MeshOfNormals, v, SurfVertex(E.x, E.y, E.z), cColor::Magenta.ToBgra());
	}
}

} // Surface

using namespace Surface;
using namespace comms;

void Degrade(SurfMesh &Dst, const SurfMesh &Src, int LOD) {
	VIPMLodder Lodder;
	// Adding verts:
	Vector3D n;
	for(int nVert = 0; nVert < Src.VERTS.Count(); nVert++) {
		const SurfVertex &r = Src.VERTS[nVert];
		n = Vector3D(r.nx, r.ny, r.nz);
		n.normalize();
		Lodder.AddVertex(Vector3D(r.x, r.y, r.z), n);
	}
	// Adding tris:
	int i0, i1, i2;
	for(int nTri = 0; nTri < Src.NTris(); nTri++) {
		Src.GetTri(nTri, i0, i1, i2);
		Lodder.AddFace(i0, i1, i2);
	}
	Lodder.Process();
	int NCollapses = cMath::Min(int(Src.VERTS.Count() - Src.VERTS.Count() / pow(2, LOD)), Lodder.GetMaxCollapses());
	Lodder.SetNCollapses(NCollapses);

	Dst.Free();
	int NVerts = Lodder.GetNVerts();
	for(nVert = 0; nVert < NVerts; nVert++) {
		int index = Lodder.GetOldVIndex(nVert);
		Dst.VERTS.Add(Src.VERTS[index]);
	}

	int NTris = Lodder.GetNFaces();
	for(nTri = 0; nTri < NTris; nTri++) {
		Lodder.GetFace(nTri, i0, i1, i2);
		Dst.TRIS.Add(i0);
		Dst.TRIS.Add(i1);
		Dst.TRIS.Add(i2);
	}
	Lodder.Reset();
}
bool uv_based_GeometryCallback( SurfRef &Ref, bool fAuxiliaryCall );

bool GeometryCallback(SurfRef &Ref, bool fAuxiliaryCall) {
	SurfSplit ss;
	ss.Split(Ref, fAuxiliaryCall ? Surface::SFX_AUX : (g_SurfEditor.Wireframe ? Surface::SFX_WIRE : Surface::SFX_DEFAULT));
	//uv_based_GeometryCallback(Ref,fAuxiliaryCall);

	if(IsActiveSurfTool("Layer") || IsActiveSurfTool("LocalGenerator") || IsActiveSurfTool("LocalExporter")) {
		SurfSplit::Cache::ModifyArgs Args;
		ZeroMemory(&Args, sizeof(Args));
		Args.Flags = SurfSplit::Cache::MF_LAYER;
		ss.CACHE.Modify(Args);
	}

	for(int i = 0; i < ss.CACHE.Count(); i++) {
		BaseMesh *pBaseMesh = ITerra->AllocateGeometry();
        if(!pBaseMesh){ 
            return false;
        }
		pBaseMesh->Swap(ss.CACHE[i]);
	}
}

bool fAuxiliaryCall = false;
void CalculateBoundary(SurfRef &);
void MakeDirtyGrassQuad(Rct rct);
bool ITGeometryCallback(const Rct &exRect, int) {
	if(!g_SurfEditor.ShowSurface) return false;

	//MakeDirtyGrassQuad(exRect);

	Surface::SurfRef Ref;
	g_UniversalMap.QUADS.GetRefRect(Ref, (int)exRect.x, (int)exRect.y,
		int(exRect.x + exRect.w - 1), int(exRect.y + exRect.h - 1));
	if(!Ref.idQuads.Count()) return false;
	Ref.Complete(SR_TRIS, SR_QUADS);
	if(!Ref.idTris.Count()) return false;

	SurfMesh AuxMesh;
	if(fAuxiliaryCall) { // THIS CALL WE SHOULD CREATE NORMALS / MARKERS
		if(g_SurfEditor.ShowNormalInTris) GenTriNormals(AuxMesh, Ref);
		if(g_SurfEditor.ShowNormalInVerts) GenVertNormals(AuxMesh, Ref);
		if(g_SurfEditor.ShowMapOfHeights) GenMarkers(AuxMesh, exRect);
		AuxMesh.GetRefAll(Ref, SR_TRIS);
		if(!Ref.idTris.Count()) return false;
	} else if(g_SurfEditor.ShowNormalInTris || g_SurfEditor.ShowNormalInVerts || g_SurfEditor.ShowMapOfHeights) { // ADDITIONAL CALL TO CREATE NORMALS / MARKERS
		fAuxiliaryCall = true;
		ITGeometryCallback(exRect, 0/*Lod*/);
		fAuxiliaryCall = false;
	}

//	static cTimer T; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//	T.Begin(); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	int NVerts = g_UniversalMap.VERTS.Count(), NTris = g_UniversalMap.TRIS.Count();

	if(!fAuxiliaryCall && g_SurfEditor.SubdivideByRace && Ref.pOrig == &g_UniversalMap) {
		Ref.SubdivideByRaceWithDupVerts();
//		Ref.SubdivideByRace();
	}

	assert(Ref.idTris.Count());
	
	bool r = GeometryCallback(Ref, fAuxiliaryCall);

	if(NVerts < g_UniversalMap.VERTS.Count()) {
		g_UniversalMap.VERTS.RemoveAt(NVerts, g_UniversalMap.VERTS.Count() - NVerts);
	}
	if(NTris < g_UniversalMap.TRIS.Count()) {
		g_UniversalMap.TRIS.RemoveAt(NTris, g_UniversalMap.TRIS.Count() - NTris);
	}

//	if(T.End(256)) { // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//		cStr Info("ITGeometryCallback: "); // <<<<<<<<<<<<<<<<<<<<<<<
//		Info += T.m_msMeanTime; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//		Log.Message(Info.asChar()); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//	} // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	return r;
}