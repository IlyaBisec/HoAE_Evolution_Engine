#include "cRawMesh.h"
#include "cIO.h"

namespace comms {

bool cRawMesh::LoadGTS(const char *FileName) {
	return cIO::Load(FileName, this, cIO::GTS);
}

bool cRawMesh::LoadX(const char *FileName) {
	return cIO::Load(FileName, this, cIO::X);
}

bool cRawMesh::SaveX(const char *FileName) {
	return cIO::Save(FileName, *this, cIO::X);
}

bool cRawMesh::LoadObj(const char *FileName, cStr *pLog) {
	return cIO::Load(FileName, this, cIO::Obj, pLog);
}

bool cRawMesh::SaveObj(const char *FileName) {
	return cIO::Save(FileName, *this, cIO::Obj);
}

//const int g_nBufferSize = 4096;
//char g_Buffer[g_nBufferSize];

//-----------------------------------------------------------------------------
// LoadObj
//-----------------------------------------------------------------------------
/*int cRawMesh::LoadObj_FetchLine(FILE *pFile) {
	char *Str = fgets(g_Buffer, g_nBufferSize, pFile);
	if(NULL == Str) return -1; // EOF
//	g_Buffer
}*/
//bool cRawMesh::LoadObj(const char *FileName) {
/*
	const int nBufferSize = 4096;
	char Buffer[nBufferSize];

	while(true) {*/
/*		int r = LoadObj_FetchLine(pFile);
		if(-1 == r || 0 == r) break;

		char *Str = fgets(Buffer, nBufferSize, pFile);
		if(NULL == Str) break; // EOF
		Buffer[nBufferSize - 2] = ' ';
		Buffer[nBufferSize - 1] = '\0';

		char *index;
		for(p = Str; *p != '\0'; p++) {
		}

  for (ptr = str; *ptr != '\0'; ptr++) {
    if (*ptr == '\t') {
      *ptr = ' ';
    }
    else if (*ptr == '\n') {
      *ptr = ' ';
      break;
    }
  }*/



//	}

//	return true;
//}

// cRawMesh::GetEdge(int, int &, int &)
bool cRawMesh::GetEdge(int nEdge, int &i0, int &i1) const {
	int ii0 = 2 * nEdge;
	assert(RangeCheck(ii0, 0, m_Edges.Count() - 2));
	if(!RangeCheck(ii0, 0, m_Edges.Count() - 2)) return false;
	i0 = m_Edges[ii0];
	i1 = m_Edges[ii0 + 1];
	return true;
} // cRawMesh::GetEdge(int, int &, int &)

// cRawMesh::GetEdge(int, cVector &, cVector &)
bool cRawMesh::GetEdge(int nEdge, cVector &e0, cVector &e1) const {
	int i0, i1;
	bool f = GetEdge(nEdge, i0, i1);
	if(!f) return false;
	e0 = m_Verts[i0];
	e1 = m_Verts[i1];
	return true;
} // cRawMesh::GetEdge(int, cVector &, cVector &)

// cRawMesh::GetTri(int, int &, int &, int &)
void cRawMesh::GetTri(int nTri, int &i0, int &i1, int &i2) const {
	ints VertNums;
	switch(m_Type) {
		case RMTTTris:
			assert(nTri < m_Tris.Count() / 3);
			int ii0;
			ii0 = 3 * nTri;
			i0 = m_Tris[ii0];
			i1 = m_Tris[ii0 + 1];
			i2 = m_Tris[ii0 + 2];
			break;
		case RMTTEdgesTris:
			int k, e, l, i;
			for(k = 0; k < 3; k++) {
				e = m_Tris[3 * nTri + k];
				for(l = 0; l < 2; l++) {
					i = m_Edges[2 * e + l];
					if(VertNums.IndexOf(i) == -1) VertNums.Add(i);
				}
			}
			assert(VertNums.Count() == 3);
			i0 = VertNums[0];
			i1 = VertNums[1];
			i2 = VertNums[2];
			break;
		default:
			assert(m_Type == RMTTTris || m_Type == RMTTEdgesTris);
	}
} // cRawMesh::GetTri(int, int &, int &, int &)

// cRawMesh::GetTri(int, cVector &, cVector &, cVector &)
void cRawMesh::GetTri(int nTri, cVector &t0, cVector &t1, cVector &t2) const {
	int i0, i1, i2;
	GetTri(nTri, i0, i1, i2);
	t0 = m_Verts[i0];
	t1 = m_Verts[i1];
	t2 = m_Verts[i2];
} // cRawMesh::GetTri(int, cVector &, cVector &, cVector &)

// cRawMesh::toPolygons
/*void cRawMesh::toPolygons() {
	m_Polygons.Free();
	int nTris = m_Tris.Count() / 3;
	for(int nTri = 0; nTri < nTris; nTri++) {
		ints VertNums;
		for(int k = 0; k < 3; k++) {
			int e = m_Tris[3 * nTri + k];
			for(int l = 0; l < 2; l++) {
				int i = m_Edges[2 * e + l];
				if(VertNums.IndexOf(i) == -1) VertNums.Add(i);
			}
		}
		assert(VertNums.Count() == 3);
		m_Polygons.Add(3);
		m_Polygons.Append(VertNums);
	}
}*/ // cRawMesh::toPolygons


// cRawMesh::EdgesFromRaw
bool cRawMesh::EdgesFromRaw(DWORDS &Edges, const ints &Raw) {
	Edges.Free();
	if(!Raw.Count()) return true; // Nothing is done but ok!
	// Stats about Raw:
	ints iis; // Inds of inds.
	int i = 0, MaxIndex = -1, nDeg, n;
	while(i < Raw.Count()) {
		nDeg = Raw[i];
		assert(nDeg >= 3);
		if(nDeg < 3) return false;
		assert(i + nDeg < Raw.Count());
		if(i + nDeg >= Raw.Count()) return false;
		iis.Add(i);
		for(n = 1; n <= nDeg; n++) {
			MaxIndex = comms::Max(MaxIndex, Raw[i + n]);
		}
		i += nDeg + 1;
	}
	// Extracting edges:
	bools Map;
	int N = MaxIndex + 1;
   	Map.Lock(Square(N), false);
	for(int ii = 0; ii < iis.Count(); ii++) {
		i = iis[ii];
		nDeg = Raw[i];
		for(n = 0; n < nDeg; n++) {
			int e0 = Raw[i + 1 + n];
			int e1 = Raw[i + 1 + (n + 1) % nDeg];
			if(!Map[e0 + N * e1]) {
				Edges.Add(e0);
				Edges.Add(e1);
				Map[e0 + N * e1] = true;
				Map[e1 + N * e0] = true;
			}
		}
	}
	return true;
} // cRawMesh::EdgesFromRaw

bool cRawMesh::RawToTris() {
	int i = 0;
	while(i < m_Raw.Count()) {
		int nDeg = m_Raw[i];
		int i0 = m_Raw[++i];
		nDeg -= 2;
		while(nDeg--) {
			i++;
			m_Tris.Add(i0);
			m_Tris.Add(m_Raw[i]);
			m_Tris.Add(m_Raw[i + 1]);
		}
		i+= 2;
	}
	return true;
}

// cRawMesh::Type
bool cRawMesh::Type(eRawMeshTopologyType TopologyType) {
	if(m_Type == TopologyType) return true;
	bool f;
	switch(TopologyType) {
		case RMTTTris:
			if(RMTTRaw == m_Type) {
				RawToTris();
				m_Raw.Free();
				m_Type = RMTTTris;
				return true;
			}
			break;
		case RMTTEdges:
			if(m_Type == RMTTRaw) {
				f = EdgesFromRaw(m_Edges, m_Raw);
				if(f) {
					m_Raw.Free();
					m_Type = RMTTEdges;
				}
				return f;
			}
	}
} // cRawMesh::Type

// cRawMesh::GenPlane
cRawMesh cRawMesh::GenPlane(double Width, double Height, int wSubDivs, int hSubDivs, cVector Axis) {
	// Params:
	// Width > 0
	int r = cUtils::CompareZero(Width);
	assert(r == 1);
	if(r < 1) Width = 1.0;
	// Height > 0
	r = cUtils::CompareZero(Height);
	assert(r == 1);
	if(r < 1) Height = 1.0;
	// wSubDivs >= 1
	assert(wSubDivs >= 1);
	if(wSubDivs < 1) wSubDivs = 1;
	// hSubDivs >= 1
	assert(hSubDivs >= 1);
	if(hSubDivs < 1) hSubDivs = 1;
	// Axis in {xAxis, yAxis, zAxis}
	bool f = Axis == cVector::xAxis || Axis == cVector::yAxis || Axis == cVector::zAxis;
	assert(f);
	if(!f) Axis = cVector::yAxis;
	// Generating:
	cRawMesh P;
	// Verts:
	double w0 = - Width / 2.0;
	double w = w0, h = - Height / 2.0;
	double dw = Width / double(wSubDivs), dh = Height / double(hSubDivs);
	int ih, iw;

	const double fx = Axis == cVector::xAxis ? 0.0 : 1.0;
	const double fy = Axis == cVector::yAxis ? 0.0 : 1.0;

	for(ih = 0; ih <= hSubDivs; ih++) {
		for(iw = 0; iw <= wSubDivs; iw++) {
			P.m_Verts.Add(cVector(fx * w, fy * h, (1.0 - fx) * w + (1.0 - fy) * h));
			w += dw;
		}
		h += dh;
		w = w0;
	}
	// Raw:
	for(ih = 0; ih < hSubDivs; ih++)
		for(iw = 0; iw < wSubDivs; iw++) {
			int i0 = (wSubDivs + 1) * ih + iw, i1 = (wSubDivs + 1) * (ih + 1) + iw;
			int nDeg = P.AddPolygon(i0, i1, i1 + 1, i0 + 1, -1);
			assert(nDeg == 4);
		}
	P.m_Type = RMTTRaw;
	return P;
} // cRawMesh::GenPlane

// cRawMesh::GenHexagonalPlane
cRawMesh cRawMesh::GenHexagonalPlane(double Width, double Height, int wSubDivs, int hSubDivs, cVector Axis) {
	// Params:
	// Width > 0
	int r = cUtils::CompareZero(Width);
	assert(r == 1);
	if(r < 1) Width = 1.0;
	// Height > 0
	r = cUtils::CompareZero(Height);
	assert(r == 1);
	if(r < 1) Height = 1.0;
	// wSubDivs >= 1
	assert(wSubDivs >= 1);
	if(wSubDivs < 1) wSubDivs = 1;
	// hSubDivs >= 1
	assert(hSubDivs >= 1);
	if(hSubDivs < 1) hSubDivs = 1;
	// Axis in {xAxis, yAxis, zAxis}
	bool f = Axis == cVector::xAxis || Axis == cVector::yAxis || Axis == cVector::zAxis;
	assert(f);
	if(!f) Axis = cVector::yAxis;

	// Generating:
	cRawMesh P;
	return P;
} // cRawMesh::GenHexagonalPlane

void cRawMesh::ScaleTo(double dMax) {
	cVector bbMin, bbMax;
	m_Verts.BBox(bbMin, bbMax);
	cVector d = bbMax - bbMin;
	double Scale = dMax / Max(d.x, d.y, d.z);
	m_Verts *= Scale;
}

void cRawMesh::InvertNormals() {
	for(int i = 0; i < m_Normals.Count(); i++) {
		m_Normals[i].Invert();
	}
}

void cRawMesh::CalcNormals() {
	assert(RMTTTris == m_Type);
	m_Normals.Lock(m_Verts.Count(), cVector::Origin);
	cVector t0, t1, t2;
	int i0, i1, i2;
	int NTris = m_Tris.Count() / 3;
	for(int nTri = 0; nTri < NTris; nTri++) {
		GetTri(nTri, t0, t1, t2);
		cVector N = (t1 - t0).Cross(t2 - t0).Normal();
		GetTri(nTri, i0, i1, i2);
		m_Normals[i0] += N;
		m_Normals[i1] += N;
		m_Normals[i2] += N;
	}
	for(int i = 0; i < m_Normals.Count(); i++) {
		m_Normals[i].Normalize();
	}
}

/*void VertexBasedMesh::CreateHexagonalPlane(Vector3D v3dOrigin, int iSx, int iSy,
										   int Dx, int Dy, DWORD dwColor, byte nFacture, byte Shadow)
{
	// uninitialization
	Free();

	int Nx = iSx / Dx, Ny = iSy / Dy, nx = Nx - 1, ny = Ny - 1, r = 0, c = 0;
	OneMapVertex vtx;
	// calculating vertices
	for(int r = 0; r <= Ny; r++)
		for(int c = 0; c <= Nx; c++)
		{
			vtx.x = v3dOrigin.x + c * Dx, vtx.y = v3dOrigin.y + r * Dy, 
				vtx.z = v3dOrigin.z, vtx.Shadow = Shadow,
				vtx.Color = dwColor, vtx.nFacture = nFacture;
			AddVertex(vtx);
		}
		for(int r = 0; r <= ny; r++)
			for(int c = 0; c <= nx; c++)
			{
				vtx.x = v3dOrigin.x + c * Dx + Dx / 2,
					vtx.y = v3dOrigin.y + r * Dy + Dy / 2,
					vtx.z = v3dOrigin.z, vtx.Shadow = Shadow,
					vtx.Color = dwColor, vtx.nFacture = nFacture;
				AddVertex(vtx);
			}
			// triangulation
			int nStart = (Nx + 1) * (Ny + 1);
			for(int r = 0; r <= ny; r++)
				for(int c = 0; c <= nx; c++)
				{
					// triangle to the left
					AddIndex(c + r * (Nx + 1));
					AddIndex(c + r * (nx + 1) + nStart);
					AddIndex(c + r * (Nx + 1) + (Nx + 1));
					// triangle to the right
					AddIndex(c + r * (nx + 1) + nStart);
					AddIndex(c + r * (Nx + 1) + 1);
					AddIndex(c + r * (Nx + 1) + 1 + (Nx + 1));
					if(r != 0)
					{
						// triangle to the upper left
						AddIndex(c + r * (nx + 1) + nStart);
						AddIndex(c + r * (Nx + 1));
						AddIndex(c + (r - 1) * (nx + 1) + nStart);
						// triangle to the upper right
						AddIndex(c + r * (nx + 1) + nStart);
						AddIndex(c + (r - 1) * (nx + 1) + nStart);
						AddIndex(c + r * (Nx + 1) + 1);
					}
					if(r == 0)
					{
						// triangle to upper
						AddIndex(c + r * (nx + 1) + nStart);
						AddIndex(c + r * (Nx + 1));
						AddIndex(c + r * (Nx + 1) + 1);
					}
					if(r == ny)
					{
						// triangle to lower
						AddIndex(c + r * (nx + 1) + nStart);
						AddIndex(c + (r + 1) * (Nx + 1));
						AddIndex(c + (r + 1) * (Nx + 1) + 1);
					}
				}
}*/

/*
// CalcSphereVert
inline void CalcSphereVert(cVector &v, int Radius, double l, double b) {
	double lSin = sin(l), lCos = cos(l), bSin = sin(b), bCos = cos(b);
	v.x = Radius * bCos * lSin;
	v.y = Radius * bCos * lCos;
	v.z = Radius * bSin;
} // CalcSphereVert

// cRawMesh::GenGlobeSphere
cRawMesh cRawMesh::GenGlobeSphere(int Radius, int lSubDivs, int bSubDivs) {
	assert(lSubDivs >= 3);
	assert(bSubDivs >= 2);

	cRawMesh S;
	// Angle deltas:
	double dl = comms::c2Pi / double(lSubDivs),
		db = comms::cPi / double(bSubDivs);
	// Bottom cover:
	S.m_Verts.Add(cVector(0, 0, - Radius)); // bottom vert.
	double l = 0, b = - comms::cPi2 + db;
	int s = 1;
	cVector v;
	for(int i = 0; i < lSubDivs; i++) {
		CalcSphereVert(v, Radius, l, b);
		S.AddVertex(v);
		l += dl;
		// Tri:
		int cur = i + s, next = (i + 1) % lSubDivs + 1;
		assert(S.AddPolygon(0, cur, next, -1) == 3);
	}
	// Body:
	for(int j = 2; j < bSubDivs; j++) {
		b += db;
		s = (j - 1) * lSubDivs + 1;
		l = 0;
		for(i = 0; i < lSubDivs; i++) {
			CalcSphereVert(v, Radius, l, b);
			S.AddVertex(v);
			l += dl;
			// Tri:
			int cur = i + s, next = (i + 1) % lSubDivs + s;
			assert(S.AddPolygon(cur - lSubDivs, cur, next, -1) == 3);
			assert(S.AddPolygon(cur - lSubDivs, next, next - lSubDivs, -1) == 3);
		}

	}
	// Top cover:
	int iTopVert = S.AddVertex(cVector(0, 0, Radius)); // top vert.
	assert(iTopVert == 1 + (bSubDivs - 1) * lSubDivs);
	s = (bSubDivs - 2) * lSubDivs + 1;
	for(i = 0; i < lSubDivs; i++) {
		// Tri:
		int cur = i + s, next = (i + 1) % lSubDivs + s;
		assert(S.AddPolygon(cur, iTopVert, next, -1) == 3);
	}
	return S;
} // cRawMesh::GenGlobeSphere

// cRawMesh::GenTesselationSphere
cRawMesh cRawMesh::GenTesselationSphere(const cVector &Position, const cVector &Radius, int nSubDivisionLevel) {
	assert(nSubDivisionLevel >= 1 && nSubDivisionLevel <= 10);

	cRawMesh S;

	return S;
}*/


// (SubDivisionLevel, VerticesNum, EdgesNum, PolygonsNum)
// (1, 12, 30, 20)
// (2, 42, 120, 80)
// (3, 92, 270, 180)
// (4, 162, 480, 320)
// (5, 252, 750, 500)
// (6, 362, 1080, 720)
// (7, 492, 1470, 980)
// (8, 642, 1920, 1280)
// (9, 812, 2430, 1620)
// (10, 1002, 3000, 2000)
//void GenTesselationSphere(const cVector &Position, const cVector &Radius, int nSubDivisionLevel) {
//	assert(nSubDivisionLevel >= 1 && nSubDivisionLevel <= 10);
//}

// (nSides, nSegments, VerticesNum, EdgesNum, PolygonsNum) (Verts, Edges, Faces, Tris)
// (3, 2, 10, 18, 12)
// (4, 2, 11, 21, 14)
// (3, 3, 13, 24, 15) (8, 15, 9, 12)
// (24, 12, 271, 561, 294) (266, 552, 288, 528)
// (100, 90, 8907, 17909, 9006) (8902, 17900, 9000, 17800)
//void GenGlobeSphere(const cVector &Position, const cVector &Radius, int nSides = 24, int nSegments = 12, const cVector &Axis = cVector::xAxis) {
//}
/*void GenGlobeSphere(VertexBasedMesh &M, int Radius, int lSubDivs, int bSubDivs) {
	M.Free();

	assert(lSubDivs >= 3);
	assert(bSubDivs >= 2);

	// Angle deltas:
	double dl = comms::c2Pi / double(lSubDivs),
		db = comms::cPi / double(bSubDivs);
	// Bottom cover:
	OneMapVertex v;
	M.Vert.Add(OneMapVertex(0, 0, - Radius)); // bottom vert.
	double l = 0, b = - comms::cPi2 + db;
	int s = 1;
	for(int i = 0; i < lSubDivs; i++) {
		GetSphere2Vert(v, Radius, l, b);
		M.Vert.Add(v);
		l += dl;
		// Tri:
		int cur = i + s, next = (i + 1) % lSubDivs + 1;
		M.Idxs.Add(0);
		M.Idxs.Add(cur);
		M.Idxs.Add(next);
	}
	// Body:
	for(int j = 2; j < bSubDivs; j++) {
		b += db;
		s = (j - 1) * lSubDivs + 1;
		l = 0;
		for(i = 0; i < lSubDivs; i++) {
			GetSphere2Vert(v, Radius, l, b);
			M.Vert.Add(v);
			l += dl;
			// Tri:
			int cur = i + s, next = (i + 1) % lSubDivs + s;
			M.Idxs.Add(cur - lSubDivs), M.Idxs.Add(cur), M.Idxs.Add(next);
			M.Idxs.Add(cur - lSubDivs), M.Idxs.Add(next), M.Idxs.Add(next - lSubDivs);
		}

	}
	// Top cover:
	int iTopVert = M.Vert.Add(OneMapVertex(0, 0, Radius)); // top vert.
	assert(iTopVert == 1 + (bSubDivs - 1) * lSubDivs);
	s = (bSubDivs - 2) * lSubDivs + 1;
	for(i = 0; i < lSubDivs; i++) {
		// Tri:
		int cur = i + s, next = (i + 1) % lSubDivs + s;
		M.Idxs.Add(cur), M.Idxs.Add(iTopVert), M.Idxs.Add(next);
	}
}*/
} // comms