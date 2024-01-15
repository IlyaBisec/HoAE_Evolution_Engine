#ifndef __cRawMesh__
#define __cRawMesh__

#include "cUtils.h"
#include "cVectors.h"

namespace comms {

typedef enum {RMTTDots /* m_Verts */, RMTTEdges /* m_Verts, m_Edges - for wireframe only */,
RMTTTris /* m_Verts, m_Tris */, RMTTEdgesTris /* m_Verts, m_Edges, m_Tris */, RMTTRaw /* m_Verts, m_Raw */,
RMTTEdgesRaw /* m_Edges, m_Raw */} eRawMeshTopologyType;

class cRawMesh {
public:
	cVectors m_Verts;
	DWORDS m_Edges, m_Tris;
	ints m_Raw; // {NumInds0, index_0, ..., index_NumInds0, ...}
	eRawMeshTopologyType m_Type;

	cRawMesh(); // Default constructor.
	cRawMesh & operator = (const cRawMesh &src); // Assignment operator.
	cRawMesh(const cRawMesh &src); // Copy - constructor.
	void Free();
	~cRawMesh(); // Default destructor.

	static bool EdgesFromRaw(DWORDS &Edges, const ints &Raw);
	bool Type(eRawMeshTopologyType TopologyType);


	// m_Verts, m_Edges, m_Tris:
	bool LoadGTS(const char *);
	void GetTri(int nTri, int &i0, int &i1, int &i2) const;
	void GetTri(int nTri, cVector &t0, cVector &t1, cVector &t2) const;
	int EdgeCount() const;
	bool GetEdge(int nEdge, int &i0, int &i1) const;
	bool GetEdge(int nEdge, cVector &e0, cVector &e1) const;
//	void toPolygons();

//	// m_Verts, m_Polygons:
//	void toTris();

	// Defaults: (1.0, 1.0, 10, 10, cVector::yAxis)
	// Requires: ( > 0, > 0, >= 1, >= 1, {cVector::xAxis, cVector::yAxis, cVector::zAxis})
	static cRawMesh GenPlane(double Width = 1.0, double Height = 1.0, int wSubDivs = 10, int hSubDivs = 10, cVector Axis = cVector::yAxis);

	// Defaults: (1.0, 1.0, 10, 10, cVector::yAxis)
	// Requires: ( > 0,  > 0, >= 1, >= 1, {cVector::xAxis, cVector::yAxis, cVector::zAxis})
	static cRawMesh GenHexagonalPlane(double Width = 1.0, double Height = 1.0, int wSubDivs = 10, int hSubDivs = 10, cVector Axis = cVector::yAxis);

//	int AddVertex(const cVector &);
	int AddPolygon(int i0, ...);

//	static cRawMesh GenGlobeSphere(int Radius, int lSubDivs, int bSubDivs);
//	static cRawMesh GenTesselationSphere(const cVector &Position, const cVector &Radius, int nSubDivisionLevel);
};

// cRawMesh
inline cRawMesh::cRawMesh()
: m_Type(RMTTDots) {
} // cRawMesh

// cRawMesh::operator =
inline cRawMesh & cRawMesh::operator = (const cRawMesh &src) {
	m_Verts = src.m_Verts;
	m_Edges = src.m_Edges;
	m_Tris = src.m_Tris;
	m_Raw = src.m_Raw;
	m_Type = src.m_Type;
	return *this;
} // cRawMesh::operator =

// cRawMesh(const cRawMesh &)
inline cRawMesh::cRawMesh(const cRawMesh &src) {
	*this = src;
} // cRawMesh(const cRawMesh &)

// cRawMesh::Free
inline void cRawMesh::Free() {
	m_Verts.Free();
	m_Edges.Free();
	m_Tris.Free();
	m_Raw.Free();
} // cRawMesh::Free

// ~cRawMesh
inline cRawMesh::~cRawMesh() {
	Free();
} // ~cRawMesh

// cRawMesh::AddVertex
/*inline int cRawMesh::AddVertex(const cVector &v) {
	return m_Verts.Add(v);
} // cRawMesh::AddVertex
*/
// cRawMesh::AddPolygon
inline int cRawMesh::AddPolygon(int i0, ...) {
	va_list arg_ptr;
	va_start(arg_ptr, i0);
	int i = i0;
	ints Inds;
	while(i >= 0) {
		Inds.Add(i);
		i = va_arg(arg_ptr, int);
	}
	va_end(arg_ptr);
	// -> m_Topology:
	int nDeg = Inds.Count();
	assert(nDeg >= 1);
	m_Raw.Add(nDeg);
	m_Raw.Append(Inds);
	return nDeg;
} // cRawMesh::AddPolygon

// cRawMesh::EdgeCount
inline int cRawMesh::EdgeCount() const {
	return m_Edges.Count() / 2;
} // cRawMesh::EdgeCount

} // comms

#endif // __cRawMesh__