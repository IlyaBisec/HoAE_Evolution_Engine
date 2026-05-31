#include "comms.h"

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

// cRawMesh::GetEdge(int, int &, int &)
bool cRawMesh::GetEdge(int nEdge, int &i0, int &i1) const {
	int ii0 = 2 * nEdge;
	assert(cMath::IsInRange(ii0, 0, EDGES.Count() - 2));
	if(!cMath::IsInRange(ii0, 0, EDGES.Count() - 2)) return false;
	i0 = EDGES[ii0];
	i1 = EDGES[ii0 + 1];
	return true;
} // cRawMesh::GetEdge(int, int &, int &)

// cRawMesh::GetEdge(int, cVec3 &, cVec3 &)
bool cRawMesh::GetEdge(int nEdge, cVec3 &e0, cVec3 &e1) const {
	int i0, i1;
	bool f = GetEdge(nEdge, i0, i1);
	if(!f) return false;
	e0 = VERTS[i0];
	e1 = VERTS[i1];
	return true;
} // cRawMesh::GetEdge(int, cVector &, cVector &)

// cRawMesh::GetTri(int, int &, int &, int &)
void cRawMesh::GetTri(int nTri, int &i0, int &i1, int &i2) const {
	ints VertNums;
	switch(TYPE) {
		case TT_TRIS:
			assert(nTri < TRIS.Count() / 3);
			int ii0;
			ii0 = 3 * nTri;
			i0 = TRIS[ii0];
			i1 = TRIS[ii0 + 1];
			i2 = TRIS[ii0 + 2];
			break;
		case TT_EDGESTRIS:
			int k, e, l, i;
			for(k = 0; k < 3; k++) {
				e = TRIS[3 * nTri + k];
				for(l = 0; l < 2; l++) {
					i = EDGES[2 * e + l];
					if(VertNums.IndexOf(i) == -1) VertNums.Add(i);
				}
			}
			assert(VertNums.Count() == 3);
			i0 = VertNums[0];
			i1 = VertNums[1];
			i2 = VertNums[2];
			break;
		default:
			assert(TYPE == TT_TRIS || TYPE == TT_EDGESTRIS);
	}
} // cRawMesh::GetTri(int, int &, int &, int &)

// cRawMesh::GetTri(int, cVec3 &, cVec3 &, cVec3 &)
void cRawMesh::GetTri(int nTri, cVec3 &t0, cVec3 &t1, cVec3 &t2) const {
	int i0, i1, i2;
	GetTri(nTri, i0, i1, i2);
	t0 = VERTS[i0];
	t1 = VERTS[i1];
	t2 = VERTS[i2];
} // cRawMesh::GetTri(int, cVector &, cVector &, cVector &)

// cRawMesh::toPolygons
/*void cRawMesh::toPolygons() {
	m_Polygons.Free();
	int nTris = TRIS.Count() / 3;
	for(int nTri = 0; nTri < nTris; nTri++) {
		ints VertNums;
		for(int k = 0; k < 3; k++) {
			int e = TRIS[3 * nTri + k];
			for(int l = 0; l < 2; l++) {
				int i = EDGES[2 * e + l];
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
			MaxIndex = cMath::Max(MaxIndex, Raw[i + n]);
		}
		i += nDeg + 1;
	}
	// Extracting edges:
	bools Map;
	int N = MaxIndex + 1;
	Map.Lock(cMath::Square(N), false);
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
	while(i < RAW.Count()) {
		int nDeg = RAW[i];        
		int i0 = RAW[++i];
        int M0 = RAW_MTLS[i];
		nDeg -= 2;
		while(nDeg--) {
			i++;
			TRIS.Add(i0);
			TRIS.Add(RAW[i]);
			TRIS.Add(RAW[i + 1]);
            MTLS.Add(M0);
            MTLS.Add(RAW_MTLS[i]);
            MTLS.Add(RAW_MTLS[i+1]);
		}
		i+= 2;
	}
	return true;
}

// cRawMesh::Type
bool cRawMesh::ConvertToType(eTopologyType ToType) {
	if(TYPE == ToType) return true;
	bool f;
	switch(ToType) {
		case TT_TRIS:
			if(TT_RAW == TYPE) {
				RawToTris();
				RAW.Free();
				TYPE = TT_TRIS;
				return true;
			}
			break;
		case TT_EDGES:
			if(TYPE == TT_RAW) {
				f = EdgesFromRaw(EDGES, RAW);
				if(f) {
					RAW.Free();
					TYPE = TT_EDGES;
				}
				return f;
			}
		default:
			assert(0);
			return false;
	}
} // cRawMesh::Type

// cRawMesh::GenPlane
/*cRawMesh cRawMesh::GenPlane(double Width, double Height, int wSubDivs, int hSubDivs, cVector Axis) {
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
			P.VERTS.Add(cVector(fx * w, fy * h, (1.0 - fx) * w + (1.0 - fy) * h));
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
	P.m_Type = RMTT_RAW;
	return P;
}*/ // cRawMesh::GenPlane

void cRawMesh::GenHexagonalPlane(float Lx, float Ly, float Cell, bool Noisy) {
	Free();
	assert(Cell > 0);

	TYPE = TT_TRIS;

	int xCells = int(Lx / Cell), yCells = int(Ly / Cell), ix, iy;
	for(iy = 0; iy <= yCells; iy++)
		for(ix = 0; ix <= xCells; ix++) {
			VERTS.Add(cVec3(float(ix) * Cell, float(iy) * Cell, 0.0f));
		}
	for(iy = 0; iy < yCells; iy++)
		for(ix = 0; ix < xCells; ix++) {
			VERTS.Add(cVec3(float(ix) * Cell + Cell / 2.0f, float(iy)  * Cell + Cell / 2.0f, 0.0f));
		}

	int Shift = (xCells + 1) * (yCells + 1);
	for(int yt = 0; yt < yCells; yt++)
		for(int xt = 0; xt < xCells; xt++) {
			// Left:
			int i0 = yt * xCells + xt + Shift;
			int i1 = yt * (xCells + 1) + xt;
			int i2 = i1 + (xCells + 1);
			AddTri(i0, i2, i1);
			// Right:
			AddTri(i0, i1 + 1, i2 + 1);
			if(0 == yt) { // Top cover:
				AddTri(i0, i1, i1 + 1);
			} else {
				int i4 = i0 - xCells;
				// Left - top:
				AddTri(i0, i1, i4);
				// Right - top:
				AddTri(i0, i4, i1 + 1);
				if(yCells - 1 == yt) { // Bottom cover:
					AddTri(i0, i2 + 1, i2);
				}
			}
	}
	if(Noisy) {
		int A = int(Cell) / 3;
		for(int i = 0; i < VERTS.Count(); i++) {
			cVec3 &u = VERTS[i];
			u.x += float(rand() % A - A / 2);
			u.y += float(rand() % A - A / 2);
		}
	}
}

void cRawMesh::InvertTris() {
	for(int i = 0; i < TRIS.Count(); i += 3) {
		cMath::Swap(TRIS[i + 1], TRIS[i + 2]);
	}
}

void cRawMesh::InvertNormals() {
	for(int i = 0; i < NORMALS.Count(); i++) {
		NORMALS[i] *= -1.0f;
	}
}

void cRawMesh::CalcNormals() {
	assert(TT_TRIS == TYPE);
	NORMALS.Lock(VERTS.Count(), cVec3::Zero);
	cVec3 t0, t1, t2;
	int i0, i1, i2;
	int NTris = TRIS.Count() / 3;
	for(int nTri = 0; nTri < NTris; nTri++) {
		GetTri(nTri, t0, t1, t2);
		cPlane P(t0, t1, t2);
		GetTri(nTri, i0, i1, i2);
		NORMALS[i0] += P.GetNormal();
		NORMALS[i1] += P.GetNormal();
		NORMALS[i2] += P.GetNormal();
	}
	for(int i = 0; i < NORMALS.Count(); i++) {
		NORMALS[i].Normalize();
	}
}

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
	S.VERTS.Add(cVector(0, 0, - Radius)); // bottom vert.
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
/*void GenGlobeSphere(SurfMesh &M, int Radius, int lSubDivs, int bSubDivs) {
	M.Free();

	assert(lSubDivs >= 3);
	assert(bSubDivs >= 2);

	// Angle deltas:
	double dl = comms::c2Pi / double(lSubDivs),
		db = comms::cPi / double(bSubDivs);
	// Bottom cover:
	SurfVertex v;
	M.Vert.Add(SurfVertex(0, 0, - Radius)); // bottom vert.
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
	int iTopVert = M.Vert.Add(SurfVertex(0, 0, Radius)); // top vert.
	assert(iTopVert == 1 + (bSubDivs - 1) * lSubDivs);
	s = (bSubDivs - 2) * lSubDivs + 1;
	for(i = 0; i < lSubDivs; i++) {
		// Tri:
		int cur = i + s, next = (i + 1) % lSubDivs + s;
		M.Idxs.Add(cur), M.Idxs.Add(iTopVert), M.Idxs.Add(next);
	}
}*/

void cRawMesh::WeldVertices() {
	DWORDS Remap;
	VERTS.RemoveDups(&Remap);
	DWORDS Tris;
	switch(TYPE) {
		case TT_TRIS:
			int i0, i1, i2, NTris;
			NTris = TRIS.Count() / 3;
			for(int nTri = 0; nTri < NTris; nTri++) {
				GetTri(nTri, i0, i1, i2);
				i0 = Remap[i0];
				i1 = Remap[i1];
				i2 = Remap[i2];
				if(i0 != i1 && i0 != i2 && i1 != i2) {
					Tris.Add(i0);
					Tris.Add(i1);
					Tris.Add(i2);
				}
			}
			TRIS.Copy(Tris);
			break;
		default:
			assert(TT_TRIS == TYPE);
	}
}