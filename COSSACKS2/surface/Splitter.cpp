#include "..\stdheader.h"
#include "Surface.h"

namespace Surface {

//-----------------------------------------------------------------------------
// Free
//-----------------------------------------------------------------------------
void Splitter::Free() {
	for(int i = 0; i < m_Cache.Count(); i++)
		delete m_Cache[i];
	m_Cache.Free();
}

//-------------------------------------------------------------------------------------------------------------------------------------
// Modify
//-------------------------------------------------------------------------------------------------------------------------------------
void Splitter::Modify(BaseMesh &bm, const SurfVertex &Offset, DWORD DiffuseAndFlag, DWORD DiffuseOrFlag, int idTexture, DWORD Flags) {
	if(Flags & ModifyFlags::MF_TEXTURE) {
		//bm.setTexture(SurfVertex::GetFactureID(nFacture));
		bm.setTexture(idTexture);
	}
	VERTEX *pVerts = (VERTEX*)bm.getVertexData();
	assert(pVerts);
	int NVerts = bm.getNVert();
	for(int j = 0; j < NVerts; j++) {
		if(Flags & ModifyFlags::MF_OFFSET) {
			pVerts->x += Offset.x;
			pVerts->y += Offset.y;
			pVerts->z += Offset.z;
		}
		if(Flags & ModifyFlags::MF_SEA) {
			if(pVerts->z < 1) pVerts->z = 1;
		}
		if(Flags & ModifyFlags::MF_AND) {
			pVerts->diffuse &= DiffuseAndFlag;
		}
		if(Flags & ModifyFlags::MF_OR) {
			pVerts->diffuse |= DiffuseOrFlag;
		}
		pVerts++;
	}
}

//-----------------------------------------------------------------------------------------------------------------------
// Modify
//-----------------------------------------------------------------------------------------------------------------------
void Splitter::Modify(const SurfVertex &Offset, DWORD DiffuseAndFlag, DWORD DiffuseOrFlag, int idTexture, DWORD Flags) {
	for(int i = 0; i < m_Cache.Count(); i++) {
		BaseMesh *pBaseMesh = m_Cache[i];
		if(!pBaseMesh) continue;
		Modify(*pBaseMesh, Offset, DiffuseAndFlag, DiffuseOrFlag, idTexture, Flags);
	}
}

//-----------------------------------------------------------------------------
// SurfSplit.dtor
//-----------------------------------------------------------------------------
Splitter::~Splitter() {
	Free();
}

//-----------------------------------------------------------------------------
// Draw
//-----------------------------------------------------------------------------
void Splitter::Draw() {
	IRS->ResetWorldTM();

	extern float g_TerrainZBias, g_ZNear;
//	float zBias = tmin( g_TerrainZBias, g_ZNear - 10.0f );// - 1;
//	ICam->ShiftZ(- zBias);
//	ICam->Render();

	const float zBias = 1;
	ICam->ShiftZ(zBias);
	ICam->Render();

	for(int i = 0; i < m_Cache.Count(); i++) {
		DrawBM(*m_Cache[i]);
	}

	ICam->ShiftZ(- zBias);
	ICam->Render();
}

//#define SPLIT_TIMING // COMMENT THIS!

#ifdef SPLIT_TIMING
LARGE_INTEGER liSplitBeginTime, liSplitEndTime;
LONGLONG llSplitTotalTime = 0;
DWORD SplitCallsCount = 0;
#endif // SPLIT_TIMING

struct SplitCache {
	//BaseMapper *pBaseMappers[SPLITMESH_MAXNFACTURES];
	SurfMapper TerraMappers[SPLITMESH_MAXNFACTURES];
	cList<VERTEX> BigMeshVerts[SPLITMESH_MAXNFACTURES];
	DWORDS BigMeshInds[SPLITMESH_MAXNFACTURES];
	cList<reIndexingHashEntry> reIndexingHashEntries[SPLITMESH_MAXNFACTURES];
	cHashIndex reIndexingTables[SPLITMESH_MAXNFACTURES];
};

// SplitOneVertex
inline void SplitOneVertex(int f, const reIndexingHashEntry &E, const SurfVertex &v, SplitCache &C) {
	int key = C.reIndexingTables[f].GenerateKey(E.oi);
	for(int index = C.reIndexingTables[f].First(key); index != -1; index = C.reIndexingTables[f].Next(index)) {
		if(C.reIndexingHashEntries[f][index].Equals(E)) break;
	}

	if(index == -1) {
		VERTEX b;
		b.x = v.x,
			b.y = v.y,
			b.z = v.z;
		//uv_pair uv = C.pBaseMappers[f]->CalcMappingUV(v, E.MappingCode);
		uv_pair uv = C.TerraMappers[f].CalcMappingUV(v, E.MappingCode);

		b.u = uv.u,
			b.v = uv.v;
		b.diffuse = v.CalcDiffuseColor();
		// ----------------------------------------------------------
//		if(E.MappingCode < 8) { // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//			b.diffuse = comms::argbColorPalette[E.MappingCode]; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//		} // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		// ----------------------------------------------------------

		if(v.nFacture != f) b.diffuse &= 0x0FFFFFFF;
		index = C.BigMeshVerts[f].Add(b);
		C.reIndexingTables[f].Add(key, index);
		C.reIndexingHashEntries[f][index] = E;
	}
	C.BigMeshInds[f].Add(index);
}

// FinalizeBaseMesh
inline void FinalizeBaseMesh(BaseMesh *pBaseMesh, int NVerts, int NInds, int f, eSurfFx SFX) {
	static int wID = IRS->GetTextureID("white.bmp");
	pBaseMesh->setNVert(NVerts);
	pBaseMesh->setNInd(NInds);
	assert(0 == NInds % 3);
	pBaseMesh->setNPri(NInds / 3);
	pBaseMesh->setShader(SurfFx(SFX));
	pBaseMesh->setTexture(SurfVertex::GetFactureID(f));
	pBaseMesh->setTexture(wID, 1);
/*	if(nStage == 0) {
		pBaseMesh->setShader(SurfVertex::SurfFx(f));
		pBaseMesh->setTexture(SurfVertex::GetFactureID(f));
		pBaseMesh->setTexture(wID, 1);
	} else if(nStage == 1) {
		pBaseMesh->setShader(Stage1Shader);
	} else if(nStage == 2) {
		pBaseMesh->setShader(Stage2Shader);
	} else if(nStage == 3) {
		pBaseMesh->setShader(AuxShader);
	}*/
}

//-----------------------------------------------------------------------------
// Splitter::Split : void(const SurfRef &, eSurfFx)
//-----------------------------------------------------------------------------
void Splitter::Split(const SurfRef &Ref, eSurfFx SFX) {
#ifdef SPLIT_TIMING
	assert(QueryPerformanceCounter(&liSplitBeginTime));
#endif // SPLIT_TIMING

	Free();

	// CALCING NUM OF TRIS PER FACTURE
	int NTrisPerFacture[SPLITMESH_MAXNFACTURES];
	ZeroMemory(NTrisPerFacture, sizeof(NTrisPerFacture));
	int nTri, f0, f1, f2, NTris = Ref.idTris.Count();
	SurfVertex t[3];
	for(int iTri = 0; iTri < NTris; iTri++) {
		nTri = Ref.idTris[iTri];
		Ref.pOrig->GetTri(nTri, t);
		f0 = t[0].nFacture;
		f1 = t[1].nFacture,
		f2 = t[2].nFacture;
		assert(f0 < SPLITMESH_MAXNFACTURES);
		assert(f1 < SPLITMESH_MAXNFACTURES);
		assert(f2 < SPLITMESH_MAXNFACTURES);
		NTrisPerFacture[f0]++;
		if(f1 != f0) NTrisPerFacture[f1]++;
		if(f2 != f1 && f2 != f0) NTrisPerFacture[f2]++;
	}
	// GETTING BASE MAPPERS, ALLOCATING BIG MESH VERTS && INDS
	// LOCKING REINDEXING HASH ENTRIES
	SplitCache C;
	for(int f = 0; f < SPLITMESH_MAXNFACTURES; f++) {
		if(NTrisPerFacture[f]) {
			// pBaseMappers:
			//C.pBaseMappers[f] = SurfVertex::GetMapper(f);
			C.TerraMappers[f] = SurfVertex::GetMapper(f);

			// BigMeshVerts && BigMeshInds:
			int NInds = 3 * NTrisPerFacture[f];
			C.BigMeshVerts[f].Check(NInds); // NVerts should be <= NInds
			C.BigMeshInds[f].Check(NInds);
			// reIndexingHashEntries:
			C.reIndexingHashEntries[f].Lock(NInds);
		}
	}

	int oi0, oi1, oi2, i0, i1, i2;
	reIndexingHashEntry E0, E1, E2;
	// ENUMERATING TRIS AND PROCESSING
	for(iTri = 0; iTri < NTris; iTri++) {
		// old inds:
		nTri = Ref.idTris[iTri];
		Ref.pOrig->GetTri(nTri, oi0, oi1, oi2);
		// vert refs:
		const SurfVertex &v0 = Ref.pOrig->VERTS[oi0],
			&v1 = Ref.pOrig->VERTS[oi1],
			&v2 = Ref.pOrig->VERTS[oi2];
		// factures:
		f0 = v0.nFacture;
		f1 = v1.nFacture;
		f2 = v2.nFacture;
		// At least one tri with facture f0:
		E0.oi = oi0;
		E1.oi = oi1;
		E2.oi = oi2;

		//E0.MappingCode = C.pBaseMappers[f0]->CalcMappingCode(v0);
		E0.MappingCode = C.TerraMappers[f0].CalcMappingCode(v0);
		E1.MappingCode = E0.MappingCode;
		E2.MappingCode = E0.MappingCode;

		SplitOneVertex(f0, E0, v0, C);
		SplitOneVertex(f0, E1, v1, C);
		SplitOneVertex(f0, E2, v2, C);

		if(f1 != f0) { // Tri with facture f1:
			//E0.MappingCode = C.pBaseMappers[f1]->CalcMappingCode(v0);
			E0.MappingCode = C.TerraMappers[f1].CalcMappingCode(v0);
			E1.MappingCode = E0.MappingCode;
			E2.MappingCode = E0.MappingCode;

			SplitOneVertex(f1, E0, v0, C);
			SplitOneVertex(f1, E1, v1, C);
			SplitOneVertex(f1, E2, v2, C);
		}

		if(f2 != f0 && f2 != f1) { // Tri with facture f2:
			//E0.MappingCode = C.pBaseMappers[f2]->CalcMappingCode(v0);
			E0.MappingCode = C.TerraMappers[f2].CalcMappingCode(v0);
			E1.MappingCode = E0.MappingCode;
			E2.MappingCode = E0.MappingCode;

			SplitOneVertex(f2, E0, v0, C);
			SplitOneVertex(f2, E1, v1, C);
			SplitOneVertex(f2, E2, v2, C);
		}
	}

	// CONVERTING BIG MESH TO BASE MESHES
	BaseMesh *pBaseMesh = NULL;
	VERTEX *pVerts = NULL;
	WORD *pInds = NULL;
	ints oiEntries;
	cHashIndex oiHash;
	for(f = 0; f < SPLITMESH_MAXNFACTURES; f++) {
		int NInds = C.BigMeshInds[f].Count();
		if(!NInds) continue;
		// how many parts we have?
		int NParts = NInds / 0xFFFF;
		if(NParts * 0xFFFF < NInds) NParts++;
		int NIndsInCurBaseMesh = 0, NVertsInCurBaseMesh = 0;
		for(int ii = 0; ii < NInds; ii++) { // index of index
			if(NIndsInCurBaseMesh == 0xFFFF) { // this base mesh is full so finalize it!
				FinalizeBaseMesh(pBaseMesh, NVertsInCurBaseMesh, 0xFFFF, f, SFX);
				m_Cache.Add(pBaseMesh);
				pBaseMesh = NULL, pVerts = NULL, pInds = NULL;
				NIndsInCurBaseMesh = 0, NVertsInCurBaseMesh = 0;
				oiHash.Clear();
			}
			if(!NIndsInCurBaseMesh) { // allocating new base mesh
				pBaseMesh = new BaseMesh();
				assert(pBaseMesh);
				int N = NInds - ii;
				if(N > 0xFFFF) N = 0xFFFF;
				pBaseMesh->create(N, N, vfVertexTS);
				pVerts = (VERTEX*)pBaseMesh->getVertexData();
				assert(pVerts);
				pInds = pBaseMesh->getIndices();
				assert(pInds);
				oiEntries.Lock(0xFFFF, -1);
			}
			// processing cur index:
			int oi = C.BigMeshInds[f][ii]; // old index
			int key = oiHash.GenerateKey(oi);
			for(int index = oiHash.First(key); index != -1; index = oiHash.Next(index)) {
				if(oiEntries[index] == oi) break;
			}
			if(index == -1) { // there is no requested vertex in this base mesh, so add it!
				index = NVertsInCurBaseMesh;
				*pVerts = C.BigMeshVerts[f][oi];
				pVerts++;
				oiHash.Add(key, index);
				oiEntries[index] = oi;
				NVertsInCurBaseMesh++;
			}
			*pInds = index;
			pInds++;
			NIndsInCurBaseMesh++;
		}
		if(pBaseMesh) { // there is unfinished base besh
			FinalizeBaseMesh(pBaseMesh, NVertsInCurBaseMesh, NIndsInCurBaseMesh, f, SFX);
			assert(pBaseMesh->getNPri() > 0);
			m_Cache.Add(pBaseMesh);
			pBaseMesh = NULL, pVerts = NULL, pInds = NULL;
			NIndsInCurBaseMesh = 0, NVertsInCurBaseMesh = 0;
			oiHash.Clear();
		}
	}

#ifdef SPLIT_TIMING
	assert(QueryPerformanceCounter(&liSplitEndTime));
	llSplitTotalTime += liSplitEndTime.QuadPart - liSplitBeginTime.QuadPart;
	SplitCallsCount++;
	if(GetKeyState(VK_LCONTROL) & 0x8000) {
		// in counts:
		char Str[4096], Buffer[1024];
		LONGLONG llAverageTime = llSplitTotalTime / SplitCallsCount;
		sprintf(Str, "Average <Split> timing: %d (counts)", llAverageTime);
		// in ms (accurate within .01):
		LARGE_INTEGER liFrequency;
		assert(QueryPerformanceFrequency(&liFrequency));
		llAverageTime *= 100000;
		llAverageTime /= liFrequency.QuadPart;
		double ms = double(llAverageTime) / 100.;
		sprintf(Buffer, " / %.2f (ms)", ms);
		strcat(Str, Buffer);
		// NTris && NVerts per facture:
		strcat(Str, "\r\n------------------------------\r\nf(NTris, NVerts): ");
		for(f = 0; f < SPLITMESH_MAXNFACTURES; f++) {
			if(C.BigMeshInds[f].Count() || C.BigMeshVerts[f].Count()) {
				sprintf(Buffer, "%d(%d, %d) ", f, C.BigMeshInds[f].Count() / 3, C.BigMeshVerts[f].Count());
				strcat(Str, Buffer);
			}
		}
		// Base meshes stats:
		strcat(Str, "\r\n------------------------------\r\nBaseMeshes:\r\nf(NTris, NVerts): ");
		for(i = 0; i < m_Cache.Count(); i++) {
			assert(m_Cache[i]->getNPri() == m_Cache[i]->getNInd() / 3);
			sprintf(Buffer, "%d(%d, %d) ", i, m_Cache[i]->getNPri(), m_Cache[i]->getNVert());
			strcat(Str, Buffer);
		}

		// Memory stats:
		strcat(Str, "\r\n==============================\r\nSplitCache memory stats:\r\n");
		size_t A = 0;
		for(f = 0; f < SPLITMESH_MAXNFACTURES; f++) {
			A += C.BigMeshInds[f].Allocated();
			A += C.BigMeshVerts[f].Allocated();
			A += C.reIndexingHashEntries[f].Allocated();
			A += C.reIndexingTables[f].Allocated();
		}
		float fAllocated = float(A) / 1024.;
		sprintf(Buffer, "Allocated = %.2f (KB)", fAllocated);
		strcat(Str, Buffer);

		MessageBox(NULL, Str, "--- Info ---", 0);
	}
#endif // SPLIT_TIMING

	// unInit:
	oiHash.Free();
	oiEntries.Free();
	for(f = 0; f < SPLITMESH_MAXNFACTURES; f++) {
		if(NTrisPerFacture[f]) {
			C.BigMeshVerts[f].Free();
			C.BigMeshInds[f].Free();
			C.reIndexingHashEntries[f].Free();
			C.reIndexingTables[f].Free();
		}
	}
	//ZeroMemory(C.pBaseMappers, sizeof(C.pBaseMappers));
}

} // Surface