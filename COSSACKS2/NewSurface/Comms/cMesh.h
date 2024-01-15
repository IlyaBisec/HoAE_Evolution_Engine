#ifndef __cMesh__
#define __cMesh__

#include "cUtils.h"
#include "cVectors.h"

namespace comms {
class cMesh
{
public:
	~cMesh();
	void Free();
	bool LoadGTS(const char *);
	void GetTri(int nTri, cVector &t0, cVector &t1, cVector &t2) const;
	void toVertsTris(cVectors &Verts, DWORDS &Tris) const;

	cVectors m_Verts;
	DWORDS m_Edges, m_Tris;
};

// ~cMesh
inline cMesh::~cMesh() {
	Free();
}

// cMesh::Free
inline void cMesh::Free() {
	m_Verts.Free();
	m_Edges.Free();
	m_Tris.Free();
}

} // comms

#endif // __cMesh__