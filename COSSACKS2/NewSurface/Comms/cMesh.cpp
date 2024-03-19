#include <stdio.h>
#include "cMesh.h"

namespace comms {

// cMesh::LoadGTS
bool cMesh::LoadGTS(const char *szFileName) {
	Free();

	FILE *fp;
	if((fp = fopen(szFileName, "r")) == NULL) return false;
	int nVerts, nEdges, nTris;
	fscanf(fp, "%d%d%d", &nVerts, &nEdges, &nTris);
	bool fSuccess = true;
	// -> Verts:
	float x, y, z;
	for(int i = 0; i < nVerts; i++) {
		if(fscanf(fp, "%f%f%f", &x, &y, &z) == EOF) {
			fSuccess = false;
			goto LoadGTSEnd;
		}
		m_Verts.Add(cVector(x, y, z));
	}
	// -> Edges:
	int i0, i1;
	for(i = 0; i < nEdges; i++) {
		if(fscanf(fp, "%d%d", &i0, &i1) == EOF) {
			fSuccess = false;
			goto LoadGTSEnd;
		}
		m_Edges.Add(i0 - 1);
		m_Edges.Add(i1 - 1);
	}
	// -> Tris:
	int i2;
	for(i = 0; i < nTris; i++) {
		if(fscanf(fp, "%d%d%d", &i0, &i1, &i2) == EOF) {
			fSuccess = false;
			goto LoadGTSEnd;
		}
		m_Tris.Add(i0 - 1);
		m_Tris.Add(i1 - 1);
		m_Tris.Add(i2 - 1);
	}
LoadGTSEnd:
	assert(!fclose(fp));
	return fSuccess;
} // cMesh::LoadGTS

// cMesh::GetTri
void cMesh::GetTri(int nTri, cVector &t0, cVector &t1, cVector &t2) const {
	ints VertNums;
	for(int k = 0; k < 3; k++) {
		int e = m_Tris[3 * nTri + k];
		for(int l = 0; l < 2; l++) {
			int i = m_Edges[2 * e + l];
			if(VertNums.IndexOf(i) == -1) VertNums.Add(i);
		}
	}
	assert(VertNums.Count() == 3);
	t0 = m_Verts[VertNums[0]];
	t1 = m_Verts[VertNums[1]];
	t2 = m_Verts[VertNums[2]];
} // cMesh::GetTri

// cMesh::toVertsTris
void cMesh::toVertsTris(cVectors &Verts, DWORDS &Tris) const {
	Verts = m_Verts;

	Tris.Free();
	int nTris = m_Tris.Count() / 3;
	for(int nTri = 0; nTri < nTris; nTri++) {
		DWORDS VertNums;
		for(int k = 0; k < 3; k++) {
			int e = m_Tris[3 * nTri + k];
			for(int l = 0; l < 2; l++) {
				int i = m_Edges[2 * e + l];
				if(VertNums.IndexOf(i) == -1) VertNums.Add(i);
			}
		}
		assert(VertNums.Count() == 3);
		Tris.Append(VertNums);
	}
}

} // comms