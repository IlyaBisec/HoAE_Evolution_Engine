#pragma once

class cRawMesh {
public:
	typedef enum {
		TT_DOTS,		// VERTS
		TT_EDGES,		// VERTS, EDGES - for wireframe only
		TT_TRIS,		// VERTS, TRIS
		TT_EDGESTRIS,	// VERTS, EDGES, TRIS
		TT_RAW,			// VERTS, RAW
		TT_EDGESRAW		// VERTS, EDGES, RAW
	} eTopologyType;

	cVectors3 VERTS;
	cVectors3 NORMALS;
	DWORDS TRIS;
	DWORDS EDGES;
    DWORDS MTLS;
    DWORDS RAW_MTLS;//Matherials
	ints RAW; // {NumInds0, index_0, ..., index_NumInds0, ...}
	eTopologyType TYPE;


	void CalcNormals();
	void InvertTris();
	void InvertNormals();

	cRawMesh(); // Default constructor.
	cRawMesh & Copy(const cRawMesh &Src);
	cRawMesh(const cRawMesh &src); // Copy - constructor.
	void Free();
	~cRawMesh(); // Default destructor.

	static bool EdgesFromRaw(DWORDS &Edges, const ints &Raw);
	bool RawToTris();
	bool ConvertToType(eTopologyType ToType);

	// VERTS, EDGES, TRIS:
	bool LoadGTS(const char *);
	// DirectX x format
	bool SaveX(const char *);
	bool LoadX(const char *);
	// Wavefront obj format
	bool LoadObj(const char *, cStr * = NULL);
	bool SaveObj(const char *);

	void AddTri(int i0, int i1, int i2);
	void GetTri(int nTri, int &i0, int &i1, int &i2) const;
	void GetTri(int nTri, cVec3 &t0, cVec3 &t1, cVec3 &t2) const;

	int EdgeCount() const;
	bool GetEdge(int nEdge, int &i0, int &i1) const;
	bool GetEdge(int nEdge, cVec3 &e0, cVec3 &e1) const;

	void WeldVertices();
//	void toPolygons();

//	// VERTS, m_Polygons:
//	void toTris();

	// Defaults: (1.0, 1.0, 10, 10, cVector::yAxis)
	// Requires: ( > 0, > 0, >= 1, >= 1, {cVector::xAxis, cVector::yAxis, cVector::zAxis})
	static cRawMesh GenPlane(float Width = 1.0f, float Height = 1.0f, int wSubDivs = 10, int hSubDivs = 10, cVec3 Axis = cVec3::yAxis);
	
	void GenHexagonalPlane(float Lx, float Ly, float Cell, bool Noisy);

//	int AddVertex(const cVector &);
	int AddPolygon(int i0, ...);

//	static cRawMesh GenGlobeSphere(int Radius, int lSubDivs, int bSubDivs);
//	static cRawMesh GenTesselationSphere(const cVector &Position, const cVector &Radius, int nSubDivisionLevel);
};

// cRawMesh
inline cRawMesh::cRawMesh()
: TYPE(TT_DOTS) {
} // cRawMesh

//-----------------------------------------------------------------------------
// cRawMesh::Copy : cRawMesh & (const cRawMesh &)
//-----------------------------------------------------------------------------
inline cRawMesh & cRawMesh::Copy(const cRawMesh &Src) {
	VERTS.Copy(Src.VERTS);
	TRIS.Copy(Src.TRIS);
	RAW.Copy(Src.RAW);
	NORMALS.Copy(Src.NORMALS);
	EDGES.Copy(Src.EDGES);
	TYPE = Src.TYPE;
	return *this;
}

// cRawMesh(const cRawMesh &)
inline cRawMesh::cRawMesh(const cRawMesh &src) {
	Copy(src);
} // cRawMesh(const cRawMesh &)

// cRawMesh::Free
inline void cRawMesh::Free() {
	VERTS.Free();
	TRIS.Free();
	NORMALS.Free();
	EDGES.Free();
	RAW.Free();
    MTLS.Free();
    RAW_MTLS.Free();
} // cRawMesh::Free

// ~cRawMesh
inline cRawMesh::~cRawMesh() {
	Free();
} // ~cRawMesh

// cRawMesh::AddVertex
/*inline int cRawMesh::AddVertex(const cVector &v) {
	return VERTS.Add(v);
} // cRawMesh::AddVertex
*/

inline void cRawMesh::AddTri(int i0, int i1, int i2) {
	assert(TT_TRIS == TYPE);
	TRIS.Add(i0);
	TRIS.Add(i1);
	TRIS.Add(i2);
}

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
	RAW.Add(nDeg);
	RAW.Append(Inds);
	return nDeg;
} // cRawMesh::AddPolygon

// cRawMesh::EdgeCount
inline int cRawMesh::EdgeCount() const {
	return EDGES.Count() / 2;
} // cRawMesh::EdgeCount