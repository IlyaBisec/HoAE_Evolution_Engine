#include "..\stdheader.h"
#include "Surface.h"
#include "ITerrain.h"
#include "comms\comms.h"

extern UniversalMap g_UniversalMap;

// CreateMarkers
void CreateMarkers(VertexBasedMesh &MeshOfMarkers, const Rct &exRect) {
	int x0 = int(exRect.x), y0 = int(exRect.y), w = int(exRect.w), h = int(exRect.h);
	const int d = 10, dh = 10;
	for(int ny = 0; ny < MaxTH; ny++)
		for(int nx = 0; nx < VertInLine; nx++) {
			int x = 32 * nx, y = 32 * ny;
			if(nx % 2) y -= 16;
			if(!InRect(x0, y0, w, h, OneMapVertex(x, y))) continue;
			short h = THMap[nx + VertInLine * ny];
			MeshOfMarkers.AddMarker(OneMapVertex(x, y, h), OneMapVertex(x, y, h + dh), d, comms::argbCyan);
		}
} // CreateMarkers

const int TerraNormalLen = 30;
// CreateTriNormals
void CreateTriNormals(VertexBasedMesh &MeshOfNormals, const VertexBasedMeshRef &Ref) {
	cVectors TriNormals;
	Ref.CalcTriNormals(TriNormals);
	cVectors TriCenters;
	Ref.CalcTriCenters(TriCenters);
	for(int i = 0; i < TriNormals.Count(); i++) {
		const cVector &N = TriNormals[i];
		const cVector &C = TriCenters[i];
		cVector E = C + TerraNormalLen * N;
		MeshOfNormals.AddLine(OneMapVertex(C.x, C.y, C.z), OneMapVertex(E.x, E.y, E.z), comms::argbBlue);
	}
} // CreateTriNormals

// CreateVertNormals
void CreateVertNormals(VertexBasedMesh &MeshOfNormals, const VertexBasedMeshRef &Ref) {
	DWORDS VertNums;
	cHashIndex reIndexingTable;
	Ref.ExtractUniqueVerts(VertNums, reIndexingTable);
	for(int i = 0; i < VertNums.Count(); i++) {
		const OneMapVertex &v = Ref.pOrigin->Vert[VertNums[i]];
		cVector C(v.x, v.y, v.z);
		cVector N(v.nx, v.ny, v.nz);
		N.Normalize();
		cVector E = C + TerraNormalLen * N;
		MeshOfNormals.AddLine(v, OneMapVertex(E.x, E.y, E.z), comms::argbMagenta);
	}
} // CreateVertNormals



//#define ITGEOMETRYCALLBACK_TIMING // COMMENT THIS!

#ifdef ITGEOMETRYCALLBACK_TIMING
LARGE_INTEGER liITGeometryCallbackBeginTime, liITGeometryCallbackEndTime;
LONGLONG llITGeometryCallbackTotalTime = 0;
DWORD ITGeometryCallbackCallsCount = 0;
ints ITGeometryCallbackAllNTris, ITGeometryCallbackAllNVerts;
#endif

struct BaseMeshDescr {
	BaseMesh *pBaseMesh;
	VERTEX *pVerts;
	WORD *pInds;
	int NVerts, NInds;
};

bool fAuxiliaryCall = false;

cHashIndex reIndexingTables[SPLITMESH_MAXNFACTURES];
cList<reIndexingHashEntry> reIndexingHashEntries[SPLITMESH_MAXNFACTURES];

inline void processOneVertex(int f, const reIndexingHashEntry &E, BaseMeshDescr &BaseMeshDescr, const OneMapVertex &v,
							 /*const */TerraMapper *pMapper) {
//							 BaseMapper *pBaseMapper) {
	 int key = reIndexingTables[f].GenerateKey(E.oi);
	 uv_pair uv;
	 for(int index = reIndexingTables[f].First(key); index != -1; index = reIndexingTables[f].Next(index)) {
		 if(reIndexingHashEntries[f][index].Equals(E)) break;
	 }

	 if(index == -1) {
		 BaseMeshDescr.pVerts->x = v.x,
			 BaseMeshDescr.pVerts->y = v.y,
			 BaseMeshDescr.pVerts->z = v.z;
		 uv = pMapper->CalcMappingUV(v, E.MappingCode);

		 BaseMeshDescr.pVerts->u = uv.u,
			 BaseMeshDescr.pVerts->v = uv.v;
		 BaseMeshDescr.pVerts->diffuse = v.CalcDiffuseColor();
		 BaseMeshDescr.pVerts->specular = v.CalcSpecularColor();

		 if(v.nFacture != f) BaseMeshDescr.pVerts->diffuse &= 0x0FFFFFFF;
		 BaseMeshDescr.pVerts++;
		 index = BaseMeshDescr.NVerts;
		 BaseMeshDescr.NVerts++;
		 reIndexingTables[f].Add(key, index);
		 reIndexingHashEntries[f][index] = E;
	 }
	 *BaseMeshDescr.pInds = index;
	 BaseMeshDescr.pInds++;
	 BaseMeshDescr.NInds++;
}

// ================================================================================
// ITGeometryCallback
// ================================================================================
bool ITGeometryCallback(const Rct &exRect, int LOD) {
	static int AuxiliaryShader = IRS->GetShaderID("terra_aux");
#ifdef ITGEOMETRYCALLBACK_TIMING
	assert(QueryPerformanceCounter(&liITGeometryCallbackBeginTime));
#endif

	VertexBasedMeshRef Ref;
	// CREATING REFERENCE TO TRIS OF REQUESTED QUAD
	// by combining miniquads from g_UniversalMap
	int QuadX0 = g_UniversalMap.QuadX0, QuadY0 = g_UniversalMap.QuadY0,
		UMLx = g_UniversalMap.UMLx, UMLy = g_UniversalMap.UMLy,
		QuadLx = g_UniversalMap.QuadLx, QuadLy = g_UniversalMap.QuadLy,
		QuadsNx = g_UniversalMap.QuadsNx, QuadsNy = g_UniversalMap.QuadsNy;

	// (cx, cy) - center of mini quad:
	int cx = (int)exRect.x + (g_UniversalMap.QuadLx >> 1),
		cy = (int)exRect.y + (g_UniversalMap.QuadLy >> 1);
	// is (cx, cy) within g_UniversalMap?
	if(cx < QuadX0 || cx >= QuadX0 + UMLx || cy < QuadY0 || cy >= QuadY0 + UMLy) return true;
	// Calculating index of this mini quad:
	int iQx = (cx - QuadX0) / QuadLx, iQy = (cy - QuadY0) / QuadLy;
	int iQ = iQx + QuadsNx * iQy;
	// Combining miniquads to fit requested:
	Ref.pOrigin = &g_UniversalMap;
	int NQx = int(exRect.w) / QuadLx, NQy = int(exRect.h) / QuadLy; // How many miniquads?
	int i, j;
	for(j = 0; j < NQy; j++)
		for(i = 0; i < NQx; i++) {
			int ix = iQx + i, iy = iQy + j; // index of mini quad to combine
			if(ix < QuadsNx && iy < QuadsNy) // is within g_UniversalMap?
				//Ref.m_TriNums.Append(g_UniversalMap.m_pQuads[ix + QuadsNx * iy]->m_TriNums);
				Ref.m_TriNums.AddValues(g_UniversalMap.m_pQuads[ix + QuadsNx * iy]->m_TriNums.GetValues(),
				g_UniversalMap.m_pQuads[ix + QuadsNx * iy]->m_TriNums.NValues);
		}

	VertexBasedMesh AuxMesh;
	extern bool g_fShowTriNormals, g_fShowVertNormals, g_ShowMapOfHeights;
	if(fAuxiliaryCall) { // THIS CALL WE SHOULD CREATE NORMALS / MARKERS
		if(g_fShowTriNormals) CreateTriNormals(AuxMesh, Ref);
		if(g_fShowVertNormals) CreateVertNormals(AuxMesh, Ref);
		if(g_ShowMapOfHeights) CreateMarkers(AuxMesh, exRect);
		AuxMesh.GetRefAll(Ref);
	} else if(g_fShowTriNormals || g_fShowVertNormals || g_ShowMapOfHeights) { // ADDITIONAL CALL TO CREATE NORMALS / MARKERS
		fAuxiliaryCall = true;
		ITGeometryCallback(exRect, LOD);
		fAuxiliaryCall = false;
	}

	// CALCING NUM OF TRIS PER FACTURE
	int NTrisPerFacture[SPLITMESH_MAXNFACTURES];
	ZeroMemory(NTrisPerFacture, sizeof(NTrisPerFacture));
	int f0, f1, f2, ii0, NTris = Ref.m_TriNums.NValues;
	for(int i = 0; i < NTris; i++) {
		ii0 = 3 * Ref.m_TriNums[i];
		f0 = Ref.pOrigin->Vert[Ref.pOrigin->Idxs[ii0]].nFacture,
			f1 = Ref.pOrigin->Vert[Ref.pOrigin->Idxs[ii0 + 1]].nFacture,
			f2 = Ref.pOrigin->Vert[Ref.pOrigin->Idxs[ii0 + 2]].nFacture;
		assert(f0 < SPLITMESH_MAXNFACTURES);
		assert(f1 < SPLITMESH_MAXNFACTURES);
		assert(f2 < SPLITMESH_MAXNFACTURES);
		NTrisPerFacture[f0]++;
		if(f1 != f0) NTrisPerFacture[f1]++;
		if(f2 != f1 && f2 != f0) NTrisPerFacture[f2]++;
	}

	// GETTING BASE MAPPERS, LOCKING BASE MESHES, LOCKING REINDEXING HASH ENTRIES
//	BaseMapper *pBaseMappers[SPLITMESH_MAXNFACTURES], *pBaseMapper;
	TerraMapper Mappers[SPLITMESH_MAXNFACTURES];

	BaseMeshDescr BaseMeshes[SPLITMESH_MAXNFACTURES];
	ZeroMemory(BaseMeshes, sizeof(BaseMeshes));
	for(int f = 0; f < SPLITMESH_MAXNFACTURES; f++) {
		if(NTrisPerFacture[f]) {
			assert(3 * NTrisPerFacture[f] <= 0xFFFF);
			// BaseMappers:
			Mappers[f] = OneMapVertex::GetMapper(f);
//			pBaseMappers[f] = OneMapVertex::GetMapper(f);
			// BaseMeshes:
            BaseMesh* pMesh = ITerra->AllocateGeometry();
			if (!pMesh) return false;
            BaseMeshes[f].pBaseMesh = pMesh;
			assert(BaseMeshes[f].pBaseMesh);
			BaseMeshes[f].pBaseMesh->create(3 * NTrisPerFacture[f], 3 * NTrisPerFacture[f], vfVertexTS);
			BaseMeshes[f].pVerts = (VERTEX*)BaseMeshes[f].pBaseMesh->getVertexData();
			assert(BaseMeshes[f].pVerts);
			BaseMeshes[f].pInds = BaseMeshes[f].pBaseMesh->getIndices();
			assert(BaseMeshes[f].pInds);
			BaseMeshes[f].NVerts = 0;
			BaseMeshes[f].NInds = 0;
			// reIndexingHashEntries:
			reIndexingHashEntries[f].Lock(3 * NTrisPerFacture[f]);
		}
	}

	int oi0, oi1, oi2, i0, i1, i2;
	reIndexingHashEntry E0, E1, E2;
	// ENUMERATING TRIS AND PROCESSING
	for(i = 0; i < NTris; i++) {
		// old inds:
		ii0 = 3 * Ref.m_TriNums[i];
		oi0 = Ref.pOrigin->Idxs[ii0];
		oi1 = Ref.pOrigin->Idxs[ii0 + 1];
		oi2 = Ref.pOrigin->Idxs[ii0 + 2];
		// vert refs:
		const OneMapVertex &v0 = Ref.pOrigin->Vert[oi0],
			&v1 = Ref.pOrigin->Vert[oi1],
			&v2 = Ref.pOrigin->Vert[oi2];
		// factures:
		f0 = v0.nFacture;
		f1 = v1.nFacture;
		f2 = v2.nFacture;
		// At least one tri with facture f0:
		E0.oi = oi0;
		E1.oi = oi1;
		E2.oi = oi2;

		//E0.MappingCode = pBaseMappers[f0]->CalcMappingCode(v0);
		E0.MappingCode = Mappers[f0].CalcMappingCode(v0);
		E1.MappingCode = E0.MappingCode;
		E2.MappingCode = E0.MappingCode;

		processOneVertex(f0, E0, BaseMeshes[f0], v0, &Mappers[f0]/*pBaseMappers[f0]*/);
		processOneVertex(f0, E1, BaseMeshes[f0], v1, &Mappers[f0]/*pBaseMappers[f0]*/);
		processOneVertex(f0, E2, BaseMeshes[f0], v2, &Mappers[f0]/*pBaseMappers[f0]*/);

		if(f1 != f0) { // Tri with facture f1:
//			E0.MappingCode = pBaseMappers[f1]->CalcMappingCode(v0);
			E0.MappingCode = Mappers[f1].CalcMappingCode(v0);
			E1.MappingCode = E0.MappingCode;
			E2.MappingCode = E0.MappingCode;

			processOneVertex(f1, E0, BaseMeshes[f1], v0, &Mappers[f1]/*pBaseMappers[f1]*/);
			processOneVertex(f1, E1, BaseMeshes[f1], v1, &Mappers[f1]/*pBaseMappers[f1]*/);
			processOneVertex(f1, E2, BaseMeshes[f1], v2, &Mappers[f1]/*pBaseMappers[f1]*/);
		}

		if(f2 != f0 && f2 != f1) { // Tri with facture f2:
			//E0.MappingCode = pBaseMappers[f2]->CalcMappingCode(v0);
			E0.MappingCode = Mappers[f2].CalcMappingCode(v0);
			E1.MappingCode = E0.MappingCode;
			E2.MappingCode = E0.MappingCode;

			processOneVertex(f2, E0, BaseMeshes[f2], v0, &Mappers[f2]/*pBaseMappers[f2]*/);
			processOneVertex(f2, E1, BaseMeshes[f2], v1, &Mappers[f2]/*pBaseMappers[f2]*/);
			processOneVertex(f2, E2, BaseMeshes[f2], v2, &Mappers[f2]/*pBaseMappers[f2]*/);
		}
	}

	// FINALIZING BASE MESHES:
#ifdef ITGEOMETRYCALLBACK_TIMING
	ITGeometryCallbackAllNTris.Add(0);
	ITGeometryCallbackAllNVerts.Add(0);
#endif // ITGEOMETRYCALLBACK_TIMING

	for(f = 0; f < SPLITMESH_MAXNFACTURES; f++) {
		if(NTrisPerFacture[f]) {
			BaseMeshes[f].pBaseMesh->setNVert(BaseMeshes[f].NVerts);
			BaseMeshes[f].pBaseMesh->setNInd(BaseMeshes[f].NInds);
			BaseMeshes[f].pBaseMesh->setNPri(BaseMeshes[f].NInds / 3);
#ifdef ITGEOMETRYCALLBACK_TIMING
			ITGeometryCallbackAllNTris[ITGeometryCallbackCallsCount] += BaseMeshes[f].pBaseMesh->getNPri();
			ITGeometryCallbackAllNVerts[ITGeometryCallbackCallsCount] += BaseMeshes[f].pBaseMesh->getNVert();
#endif // ITGEOMETRYCALLBACK_TIMING
			if(fAuxiliaryCall) {
				BaseMeshes[f].pBaseMesh->setTexture(0);
				BaseMeshes[f].pBaseMesh->setTexture(0, 1);
				BaseMeshes[f].pBaseMesh->setShader(AuxiliaryShader);
			} else {
				BaseMeshes[f].pBaseMesh->setTexture(OneMapVertex::GetFactureID(f));
				BaseMeshes[f].pBaseMesh->setTexture(OneMapVertex::GetFactureID(f), 1);
				BaseMeshes[f].pBaseMesh->setTexture(OneMapVertex::GetFactureID2(f), 2);
				BaseMeshes[f].pBaseMesh->setShader(OneMapVertex::GetShader(f));
			}
		}
	}

#ifdef ITGEOMETRYCALLBACK_TIMING
	assert(QueryPerformanceCounter(&liITGeometryCallbackEndTime));
	llITGeometryCallbackTotalTime += liITGeometryCallbackEndTime.QuadPart - liITGeometryCallbackBeginTime.QuadPart;
	ITGeometryCallbackCallsCount++;
	if(GetKeyState(VK_LCONTROL) & 0x8000) {
		// in counts:
		char Str[4096], Buffer[1024];
		LONGLONG llAverageTime = llITGeometryCallbackTotalTime / ITGeometryCallbackCallsCount;
		sprintf(Str, "Average <ITGeometryCallback> timing: %d (counts)", llAverageTime);
		// in ms (accurate within .01):
		LARGE_INTEGER liFrequency;
		assert(QueryPerformanceFrequency(&liFrequency));
		llAverageTime *= 100000;
		llAverageTime /= liFrequency.QuadPart;
		double ms = double(llAverageTime) / 100.;
		sprintf(Buffer, " / %.2f (ms)", ms);
		strcat(Str, Buffer);
		// NTris:
		LONGLONG llTotalNTris = 0;
		int MaxNTris = 0;
		for(int i = 0; i < ITGeometryCallbackAllNTris.Count(); i++) {
			int NumTris = ITGeometryCallbackAllNTris[i];
			llTotalNTris += NumTris;
			if(MaxNTris < NumTris) MaxNTris = NumTris;
		}
		sprintf(Buffer, "\r\n-----------------------------\r\nNumTris: Average = %d, Max = %d",
			int(llTotalNTris / ITGeometryCallbackCallsCount), MaxNTris);
		strcat(Str, Buffer);
		// NVerts:
		LONGLONG llTotalNVerts = 0;
		int MaxNVerts = 0;
		for(i = 0; i < ITGeometryCallbackAllNVerts.Count(); i++) {
			int NumVerts = ITGeometryCallbackAllNVerts[i];
			llTotalNVerts += NumVerts;
			if(MaxNVerts < NumVerts) MaxNVerts = NumVerts;
		}
		sprintf(Buffer, "\r\n-----------------------------\r\nNumVerts: Average = %d, Max = %d",
			int(llTotalNVerts / ITGeometryCallbackCallsCount), MaxNVerts);
		strcat(Str, Buffer);
		// Memory stats:
		strcat(Str, "\r\n==============================\r\nNums of used reIndexingTables: ");
		size_t reIndexingTablesAllocated = 0;
		for(f = 0; f < SPLITMESH_MAXNFACTURES; f++ ) {
			size_t S = reIndexingTables[f].Allocated();
			if(S) {
				reIndexingTablesAllocated += S;
				sprintf(Buffer, "%d(%d) ", f, reIndexingTables[f].CalcSpread());
				strcat(Str, Buffer);
			}
		}
		float fAllocated = float(reIndexingTablesAllocated) / 1024.;
		sprintf(Buffer, "\r\nreIndexingTablesAllocated = %.2f (KB)", fAllocated);
		strcat(Str, Buffer);

		MessageBox(NULL, Str, "--- Info ---", 0);
	}
#endif // ITGEOMETRYCALLBACK_TIMING

	// unInit:
	for(f = 0; f < SPLITMESH_MAXNFACTURES; f++ ){
		if(NTrisPerFacture[f]) {
			reIndexingTables[f].Clear();
			reIndexingHashEntries[f].Free();
		}
	}
	//ZeroMemory(pBaseMappers, sizeof(pBaseMappers));
	ZeroMemory(BaseMeshes, sizeof(BaseMeshes));

	return true;
}