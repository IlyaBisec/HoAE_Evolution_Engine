#include "..\stdheader.h"

const DWORD c_Red = 0xFFFF0000, c_Green = 0xFF00FF00, c_DarkBlue = 0xFF0000FF,
	c_White = 0xFFFFFFFF, c_Yellow = 0xFFFFFF00, c_Blue = 0xFF00FFFF,
	c_Purple = 0xFFFF00FF, c_Grey = 0xFF505050;

#define DOUBLE_MAX		double(1.7976931348623157e+308)
#define DOUBLE_MIN		double(-1.7976931348623157e+308)
#define DOUBLE_EPSILON	double(4.9406564584124654e-324)
#define DWORD_MAX		0xFFFFFFFF

enum EBooleans {Union, Intersection, ADifferenceB, BDifferenceA, NOP};
char *strBooleans[] = {"A + B", "A & B", "A - B", "B - A", "NOP"};

// ===========================================================================

#define DWORD_MAX		0xFFFFFFFF
#define FLOAT_MAX		float(0x7F7FFFFF)
#define FLOAT_MIN		float(0xFF7FFFFF)
#define FLOAT_EPSILON	float(0x00000001)

class CVertex3D
{
public:
	float x, y, z;
	static const CVertex3D Null;
	static const CVertex3D MaxValue;
	static const CVertex3D MinValue;
	static const float fEpsilon;
	CVertex3D() { *this = CVertex3D::Null; }
	CVertex3D(float x, float y, float z) { Set(x, y, z); }
	Set(float x, float y, float z)
	{ this->x = x, this->y = y, this->z = z; };
//	bool operator == (const CVertex3D &r)
//	{ return (x == r.x) && (y == r.y) && (z == r.z); }
	bool operator == (const CVertex3D &r)
	{
		float dx = r.x - x, dy = r.y - y, dz = r.z - z;
		return (sqrt(dx * dx + dy * dy + dz * dz) < fEpsilon);
	}
	bool operator != (const CVertex3D &r) { return !(*this == r); }
	friend bool operator == (const CVertex3D &rA, const CVertex3D &rB);
	friend bool operator != (const CVertex3D &rA, const CVertex3D &rB);
	float DistanceApart(const CVertex3D &r)
	{
		float dx = r.x - x, dy = r.y - y, dz = r.z - z;
		return sqrt(dx * dx + dy * dy + dz * dz);
	}
};

inline bool operator == (const CVertex3D &rA, const CVertex3D &rB)
{
	float dx = rB.x - rA.x, dy = rB.y - rA.y, dz = rB.z - rA.z;
	return (sqrt(dx * dx + dy * dy + dz * dz) < CVertex3D::fEpsilon);
}

inline bool operator != (const CVertex3D &rA, const CVertex3D &rB) { return !(rA == rB); }

class CVector3D : public CVertex3D
{
public:
	CVector3D() { (CVertex3D)(*this) = CVertex3D::Null; }
	CVector3D(float x, float y, float z) { Set(x, y, z); }
	CVector3D(const CVertex3D &r) { *this = r; }
	CVector3D & operator = (const CVertex3D &r)
	{
		x = r.x, y = r.y, z = r.z;
		return *this;
	}
	void Normalize()
	{
		float L = sqrt(x * x + y * y + z * z);
		if(L < FLOAT_EPSILON) return;
		x /= L, y /= L, z /= L;
	}
	CVector3D & operator += (const CVector3D &r)
	{
		x += r.x, y += r.y, z += r.z;
		return *this;
	}
	CVector3D & operator -= (const CVector3D &r)
	{
		x -= r.x, y -= r.y, z -= r.z;
		return *this;
	}
	friend CVector3D operator + (const CVector3D &, const CVector3D &);
	friend CVector3D operator - (const CVector3D &, const CVector3D &);
	friend float operator * (const CVector3D &, const CVector3D &);
	friend CVector3D operator ^ (const CVector3D &, const CVector3D &);
	friend CVector3D operator * (float, const CVector3D &);
	friend CVector3D operator / (const CVector3D &, float);
	unsigned int hash() const
	{
		int ix, iy, iz;
		memcpy(&ix, &x, sizeof(int)),
			memcpy(&iy, &y, sizeof(int)),
			memcpy(&iz, &z, sizeof(int));
		unsigned int h = 1;
		h = 37 * h + ix;
		h = 37 * h + iy;
		h = 37 * h + iz;
		return h;
	}
};

inline CVector3D operator + (const CVector3D &rA, const CVector3D &rB)
{ return CVector3D(rA.x + rB.x, rA.y + rB.y, rA.z + rB.z); }

inline CVector3D operator - (const CVector3D &rA, const CVector3D &rB)
{ return CVector3D(rA.x - rB.x, rA.y - rB.y, rA.z - rB.z); }

inline float operator * (const CVector3D &rA, const CVector3D &rB)
{ return rA.x * rB.x + rA.y * rB.y + rA.z * rB.z; }

inline CVector3D operator ^ (const CVector3D &rA, const CVector3D &rB)
{
	return CVector3D(rA.y * rB.z - rA.z * rB.y, rA.z * rB.x - rA.x * rB.z,
		rA.x * rB.y - rA.y * rB.x);
}

inline CVector3D operator * (float k, const CVector3D &r)
{ return CVector3D(k * r.x, k * r.y, k * r.z); }

inline CVector3D operator / (const CVector3D &r, float d)
{ return CVector3D(r.x / d, r.y / d, r.z / d); }

const CVertex3D CVertex3D::Null(0.0f, 0.0f, 0.0f);
const CVertex3D CVertex3D::MaxValue(FLOAT_MAX, FLOAT_MAX, FLOAT_MAX);
const CVertex3D CVertex3D::MinValue(FLOAT_MIN, FLOAT_MIN, FLOAT_MIN);
const float CVertex3D::fEpsilon = 1E-6f;

typedef DynArray<CVertex3D> CVertices3D;
typedef DynArray<DWORD> CIndices;

class CSection3D
{
public:
	CVertex3D A, B;
	CSection3D() {}
	CSection3D(const CVertex3D &rA, const CVertex3D &rB) { A = rA, B = rB; }
	friend bool operator == (const CSection3D &rA, const CSection3D &rB);
	friend bool operator != (const CSection3D &rA, const CSection3D &rB);
};

inline bool operator == (const CSection3D &rA, const CSection3D &rB)
{ return ((rA.A == rB.A) && (rA.B == rB.B)) || ((rA.B == rB.A) && (rA.A == rB.B)); }
inline bool operator != (const CSection3D &rA, const CSection3D &rB) { return !(rA == rB); }

typedef DynArray<CSection3D> CSections3D;

class CPolygon3D
{
public:
	CVertices3D	Vertices;
	void	Clear() { Vertices.Clear(); }
	void	AddVertices(CVertex3D vtx0, ...)
	{
		CVertex3D v = vtx0;
		va_list Vertices2Add;
		va_start(Vertices2Add, vtx0);
		while((v != CVertex3D::MaxValue) && (v != CVertex3D::MinValue))
		{
			Vertices.Add(v);
			v = va_arg(Vertices2Add, CVertex3D);
		}
		va_end(Vertices2Add);
	}
	bool	CalcNormal(CVector3D &rNormal)
	{
		if(Vertices.NValues < 3) return false;
		CVector3D p0 = Vertices[0], p1 = Vertices[1], p2 = Vertices[2],
			u = p1 - p0, v = p2 - p1;
		rNormal = u ^ v;
		rNormal.Normalize();
	}
	float	nfG(CVertex3D &rVertex)
	{
		CVector3D Normal;
		CalcNormal(Normal);
		CVector3D v0(Vertices[0]), vtx(rVertex), S = v0 - vtx;
		return S * Normal;
	}
	int		CrossSide(const CVertex3D &rA, const CVertex3D &rB, CVertex3D &rCross)
	{
		if(Vertices.NValues < 3) return -1;
		CVector3D a(rA), b(rB), H = b - a, p1(Vertices[0]),
			p2(Vertices[1]), p3(Vertices[2]), V = p2 - p1, W = p3 - p2,
			N = V ^ W, D = p1 - a;
		if(N * H == 0) return -1;
		Matrix3D M(H.x, H.y, H.z, -V.x, -V.y, -V.z, -W.x, -W.y, -W.z);
		M.inverse();
		float t = D.x * M.e00 + D.y * M.e10 + D.z * M.e20;
		CVector3D q = a + t * H;
		rCross = q;
		if((t <= 1.0f) && (t >= 0.0f)) return 1;
		return 0;
	}

};

class CMesh3D
{
public:
	CVertex3D m_BBox0, m_BBox1;
	CVertices3D	Vertices;
	CIndices	Indices;
	void		Clear() { Vertices.Clear(), Indices.Clear(); }
	void CalcBBox()
	{
		assert(Vertices.NValues > 0);
		if(Vertices.NValues <= 0) return;
		float x0, x1, y0, y1, z0, z1;
		x0 = x1 = Vertices[0].x, y0 = y1 = Vertices[0].y, z0 = z1 = Vertices[0].z;
		for(int i = 1; i < Vertices.NValues; i++)
		{
			CVertex3D &v = Vertices[i];
			if(x0 > v.x) x0 = v.x;
			if(y0 > v.y) y0 = v.y;
			if(z0 > v.z) z0 = v.z;
			if(x1 < v.x) x1 = v.x;
			if(y1 < v.y) y1 = v.y;
			if(z1 < v.z) z1 = v.z;
		}
		m_BBox0.Set(x0, y0, z0);
		m_BBox1.Set(x1, y1, z1);
	}
	CMesh3D & operator = (const CMesh3D &r)
	{
		Clear();
		Vertices.AddValues(r.Vertices.GetValues(), r.Vertices.NValues);
		Indices.AddValues(r.Indices.GetValues(), r.Indices.NValues);
		return *this;
	}
	void Booleans_ExtractInnerTriangles(CMesh3D &rR, const CMesh3D &rCutting, CIndices &ITriNumbers)
	{ Booleans_ExtractTriangles(rR, rCutting, ITriNumbers, true); }
	void Booleans_ExtractOuterTriangles(CMesh3D &rR, const CMesh3D &rCutting, CIndices &ITriNumbers)
	{ Booleans_ExtractTriangles(rR, rCutting, ITriNumbers, false); }
	void Booleans_ExtractTriangles(CMesh3D &rR, const CMesh3D &rCutting, CIndices &ITriNumbers, bool r);

	void		Booleans_Subtract(CMesh3D &, CVertices3D &, CSections3D &, CMesh3D &, CMesh3D &);
	void		AddVertices(CVertex3D vtx0, ...)
	{
		CVertex3D v = vtx0;
		va_list Vertices2Add;
		va_start(Vertices2Add, vtx0);
		while((v != CVertex3D::MaxValue) && (v != CVertex3D::MinValue))
		{
			Vertices.Add(v);
			v = va_arg(Vertices2Add, CVertex3D);
		}
		va_end(Vertices2Add);
	}
	void		AddIndices(DWORD i0, ...)
	{
		DWORD i = i0;
		va_list Indices2Add;
		va_start(Indices2Add, i0);
		while(i != DWORD_MAX)
		{
			Indices.Add(i);
			i = va_arg(Indices2Add, DWORD);
		}
		va_end(Indices2Add);
	}
	void AddTriangle(CVertex3D v0, CVertex3D v1, CVertex3D v2)
	{
		int i0 = Vertices.find(v0), i1 = Vertices.find(v1), i2 = Vertices.find(v2);
		if(i0 == -1) i0 = Vertices.Add(v0);
		if(i1 == -1) i1 = Vertices.Add(v1);
		if(i2 == -1) i2 = Vertices.Add(v2);
		AddIndices(i0, i1, i2, DWORD_MAX);
	}
	bool GetTriangle(CVertex3D &rv0, CVertex3D &rv1, CVertex3D &rv2, int nTri) const
	{
		if(nTri >= Indices.NValues / 3) return false;
		int i0 = Indices[3 * nTri + 0], i1 = Indices[3 * nTri + 1], i2 = Indices[3 * nTri + 2];
		rv0 = Vertices[i0], rv1 = Vertices[i1], rv2 = Vertices[i2];
		return true;
	}
	bool	toBaseMesh(BaseMesh &rBaseMesh, char *strShader, char *strTexture,
		DWORD Color, CVertex3D v3dOrigin);
	bool	FillNormals(BaseMesh &rNormals);
	void	generatorSphere_CalcVertex(CVertex3D &rVertex, float R, int b, int l);
	void	generatorSphere(CVertex3D c, float r, int db, int dl, CVertex3D S);
	static bool VertexIsInside(const CMesh3D &rMesh, const CVertex3D &rVertex)
	{
		CVector3D V = rVertex;
		int NTri = rMesh.Indices.NValues / 3;
		for(int nTri = 0; nTri < NTri; nTri++)
		{
			CVector3D v0 = rMesh.Vertices[rMesh.Indices[3 * nTri + 0]],
				v1 = rMesh.Vertices[rMesh.Indices[3 * nTri + 1]],
				v2 = rMesh.Vertices[rMesh.Indices[3 * nTri + 2]];
			CVector3D Normal = (v1 - v0) ^ (v2 - v1);
			float f = (V - v0) * Normal;
			if(f > 0) return false;
		}
		return true;
	}
	static int CrossSectionTriangle2(const CSection3D &rSection, const CMesh3D &rMesh, int nTri, CVertex3D &rCross);
	static bool VertexIsInside2(const CMesh3D &rMesh, const CVertex3D &rVertex)
	{
		int NTri = rMesh.Indices.NValues / 3;
VertexIsInside2_label:
		CSection3D S(rVertex, CVertex3D(rVertex.x - 300 + rand() % 600,
			rVertex.y - 300 + rand() % 600, rVertex.z + 900));
		int nIntersections = 0; 
		CVertex3D Cross;
		for(int nTri = 0; nTri < NTri; nTri++)
		{
			int r = CrossSectionTriangle2(S, rMesh, nTri, Cross);
			if(r == -1) goto VertexIsInside2_label;
			if(r == 1) nIntersections++;
		}
		return nIntersections & 1;
	}
	static bool TriangleIntersect(const CMesh3D &rMesh, int nTri, const CVertex3D &rVertexA,
		const CVertex3D &rVertexB, CVertex3D &rCross)
	{
		CVector3D p0(rMesh.Vertices[rMesh.Indices[3 * nTri + 0]]),
			p1(rMesh.Vertices[rMesh.Indices[3 * nTri + 1]]),
			p2(rMesh.Vertices[rMesh.Indices[3 * nTri + 2]]),
			a(rVertexA), b(rVertexB);
		CVector3D H = b - a, V = p1 - p0, W = p2 - p1;
		CVector3D N = V ^ W;
		N.Normalize();
		if(N * H == 0) return false; // Parallel!
		CVector3D D = p0 - a;
		Matrix3D M(H.x, H.y, H.z, -V.x, -V.y, -V.z, -W.x, -W.y, -W.z);
		M.inverse();
		float t = D.x * M.e00 + D.y * M.e10 + D.z * M.e20,
			Tau = D.x * M.e01 + D.y * M.e11 + D.z * M.e21,
			Theta = D.x * M.e02 + D.y * M.e12 + D.z * M.e22;
		CVector3D q = a + t * H;
		rCross.x = q.x, rCross.y = q.y, rCross.z = q.z;
		return ((t >= 0.0f) && (t <= 1.0f) && (Tau >= 0.0f) && (Tau <= 1.0f) &&
			(Theta >= 0.0f) && (Theta <= 1.0f) && (Theta <= Tau));
	}
	static bool TriTriIntersect(const CMesh3D &rMeshA, int nTriA,
		const CMesh3D &rMeshB, int nTriB, CSection3D &rSection, CVertices3D &rCrosses)
	{
		CVertex3D p0(rMeshA.Vertices[rMeshA.Indices[3 * nTriA + 0]]),
			p1(rMeshA.Vertices[rMeshA.Indices[3 * nTriA + 1]]),
			p2(rMeshA.Vertices[rMeshA.Indices[3 * nTriA + 2]]),
			q0(rMeshB.Vertices[rMeshB.Indices[3 * nTriB + 0]]),
			q1(rMeshB.Vertices[rMeshB.Indices[3 * nTriB + 1]]),
			q2(rMeshB.Vertices[rMeshB.Indices[3 * nTriB + 2]]);
		CVertex3D Cross, Crosses[10];
		int nCur = 0;
		if(TriangleIntersect(rMeshA, nTriA, q0, q1, Cross)) Crosses[nCur] = Cross, nCur++, rCrosses.Add(Cross);
		if(TriangleIntersect(rMeshA, nTriA, q1, q2, Cross)) Crosses[nCur] = Cross, nCur++, rCrosses.Add(Cross);
		if(TriangleIntersect(rMeshA, nTriA, q2, q0, Cross)) Crosses[nCur] = Cross, nCur++, rCrosses.Add(Cross);
		if(TriangleIntersect(rMeshB, nTriB, p0, p1, Cross)) Crosses[nCur] = Cross, nCur++, rCrosses.Add(Cross);
		if(TriangleIntersect(rMeshB, nTriB, p1, p2, Cross)) Crosses[nCur] = Cross, nCur++, rCrosses.Add(Cross);
		if(TriangleIntersect(rMeshB, nTriB, p2, p0, Cross)) Crosses[nCur] = Cross, nCur++, rCrosses.Add(Cross);
//		if(nCur > 2) MessageBox(NULL, "What the hell? nCur > 2!", "--- WARNING ---", 0); // <<<<<<<<<<<<<<<<<<<<<<<<<
		rSection.A = Crosses[0], rSection.B = Crosses[1];
		return nCur >= 2;
	}
	static bool Difference(const CMesh3D &rMeshA, const CMesh3D &rMeshB, CMesh3D &rMeshR,
		CVertices3D &rCrosses, CMesh3D &rMeshT, CSections3D &rSections);
	static bool Difference2(const CMesh3D &rMeshA, const CMesh3D &rMeshB, CMesh3D &rMeshR,
		CVertices3D &rCrosses, CMesh3D &rMeshT, CSections3D &rSections);
	static bool CrossSectionTriangle(const CSection3D &rSection, const CMesh3D &rMesh, int nTri, CVertex3D &rCross);
	static void toSections(const CMesh3D &rMesh, CSections3D &rSections, CIndices &Triangles);
	static void CrossSectionsMesh(const CSections3D &rSections, const CMesh3D &rMesh, CVertex3D **ppCrosses);
	static bool VertexIsInsideMesh_ConvTest(const CMesh3D &rMesh, const CVertex3D &rVertex);
	static void ExtractTriangles(const CMesh3D &rMesh, const CMesh3D &rClipper, const CVertex3D *pCrosses,
					  const CSections3D &rSections, const CIndices &rIndices, CMesh3D &rMeshR,
					  CIndices &rITriNumbers, bool fCriterion);
	static void toCSections(const CMesh3D &rMeshA, const CMesh3D &rMeshB, const CIndices &rITriANumbers,
		const CSections3D &rSectionsA, const CIndices &rIndicesA, CVertex3D *pCrossesA,
		int NTriB, CIndices &rCSectionsANumbers, bool fCriterion);
	static void FilletSections(const CSections3D &rSectionsA, const CSections3D &rSectionsB,
		const CIndices &rIndicesA, const CIndices &rIndicesB,
		const CIndices &rITriANumbers, const CIndices &rITriBNumbers,
		CVertex3D *pCrossesA, CVertex3D *pCrossesB, int NTriA, int NTriB, CSections3D &rFilletSections);
	static void toCVertices(CIndices &rCSectionsNumbers, const CSections3D &rSections,
		CVertices3D &rCVertices);
};

// ================================================================================
// CMesh3D::Booleans_ExtractTriangles
// ================================================================================
void CMesh3D::Booleans_ExtractTriangles(CMesh3D &rR, const CMesh3D &rCutting, CIndices &ITriNumbers, bool r)
{
	int NTriA = Indices.NValues / 3, NTriB = rCutting.Indices.NValues / 3;
	CVertices3D Crosses;
	for(int nTriA = 0; nTriA < nTriA; nTriA++)
	{
		CVertex3D v0 = Vertices[Indices[3 * nTriA + 0]],
			v1 = Vertices[Indices[3 * nTriA + 1]],
			v2 = Vertices[Indices[3 * nTriA + 2]];
//		bool r0 = CMesh3D::VertexIsInside_ConvTest(rCutting, v0),
//			r1 = CMesh3D::VertexIsInside_ConvTest(rCutting, v1),
//			r2 = CMesh3D::VertexIsInside_ConvTest(rCutting, v2);
		bool r0 = CMesh3D::VertexIsInside2(rCutting, v0),
			r1 = CMesh3D::VertexIsInside2(rCutting, v1),
			r2 = CMesh3D::VertexIsInside2(rCutting, v2);
		if((r0 != r1) || (r0 != r2) || (r1 != r2)) ITriNumbers.Add(nTriA);
		CSection3D S;
		for(int nTriB = 0; nTriB < NTriB; nTriB++)
			if(TriTriIntersect(*this, nTriA, rCutting, nTriB, S, Crosses)) break;
		if(nTriB < NTriB)
		{
			ITriNumbers.Add(nTriA);
			continue;
		}
		if((r0 == r) && (r1 == r) && (r2 == r))
		{
			int i0 = rR.Vertices.find(v0), i1 = rR.Vertices.find(v1),
				i2 = rR.Vertices.find(v2);
			if(i0 == -1) i0 = rR.Vertices.Add(v0);
			if(i1 == -1) i1 = rR.Vertices.Add(v1);
			if(i2 == -1) i2 = rR.Vertices.Add(v2);
			if(r) rR.AddIndices(i0, i2, i1, DWORD_MAX);
			else rR.AddIndices(i0, i1, i2, DWORD_MAX);
		}
	}
	Crosses.Clear();
}

int CMesh3D::CrossSectionTriangle2(const CSection3D &rSection, const CMesh3D &rMesh, int nTri, CVertex3D &rCross)
{
	const float fD = 0.01f;
	CVector3D p0(rMesh.Vertices[rMesh.Indices[3 * nTri + 0]]), p1(rMesh.Vertices[rMesh.Indices[3 * nTri + 1]]),
		p2(rMesh.Vertices[rMesh.Indices[3 * nTri + 2]]), a(rSection.A), b(rSection.B);
	CVector3D H = b - a, V = p1 - p0, W = p2 - p1, N = V ^ W;
	N.Normalize();
	if(fabs(N * H) < fD) return -1; // We should retest!
	CVector3D D = p0 - a;
	Matrix3D M(H.x, H.y, H.z, -V.x, -V.y, -V.z, -W.x, -W.y, -W.z);
	M.inverse();
	float u = D.x * M.e00 + D.y * M.e10 + D.z * M.e20,
		v = D.x * M.e01 + D.y * M.e11 + D.z * M.e21,
		w = D.x * M.e02 + D.y * M.e12 + D.z * M.e22;
	CVector3D q = a + u * H;
	rCross.x = q.x, rCross.y = q.y, rCross.z = q.z;
	if(fabs(u) < fD) return -1;
	if(fabs(u - 1.0f) < fD) return -1;
	if(fabs(v) < fD) return -1;
	if(fabs(v - 1.0f) < fD) return -1;
	if(fabs(w) < fD) return -1;
	if(fabs(w - 1.0f) < fD) return -1;
	bool r = ((u >= 0.0f) && (u <= 1.0f) && (v >= 0.0f) && (v <= 1.0f) && (w >= 0.0f) && (w <= 1.0f) && (w <= v));
	if(r) return 1;
	return 0;
}


bool CMesh3D::CrossSectionTriangle(const CSection3D &rSection, const CMesh3D &rMesh, int nTri, CVertex3D &rCross)
{
	CVector3D p0(rMesh.Vertices[rMesh.Indices[3 * nTri + 0]]), p1(rMesh.Vertices[rMesh.Indices[3 * nTri + 1]]),
		p2(rMesh.Vertices[rMesh.Indices[3 * nTri + 2]]), a(rSection.A), b(rSection.B);
	CVector3D H = b - a, V = p1 - p0, W = p2 - p1, N = V ^ W;
	N.Normalize();
	if(N * H == 0) return false; // Parallel! Consider this in the future...
	CVector3D D = p0 - a;
	Matrix3D M(H.x, H.y, H.z, -V.x, -V.y, -V.z, -W.x, -W.y, -W.z);
	M.inverse();
	float u = D.x * M.e00 + D.y * M.e10 + D.z * M.e20,
		v = D.x * M.e01 + D.y * M.e11 + D.z * M.e21,
		w = D.x * M.e02 + D.y * M.e12 + D.z * M.e22;
	CVector3D q = a + u * H;
	rCross.x = q.x, rCross.y = q.y, rCross.z = q.z;
	return ((u >= 0.0f) && (u <= 1.0f) && (v >= 0.0f) && (v <= 1.0f) && (w >= 0.0f) && (w <= 1.0f) && (w <= v));
}

void CMesh3D::toSections(const CMesh3D &rMesh, CSections3D &rSections, CIndices &rTriangles)
{
	int NTri = rMesh.Indices.NValues / 3;
	for(int nTri = 0; nTri < NTri; nTri++)
	{
		int i0 = rMesh.Indices[3 * nTri + 0], i1 = rMesh.Indices[3 * nTri + 1],
			i2 = rMesh.Indices[3 * nTri + 2];
		CVertex3D v0 = rMesh.Vertices[i0], v1 = rMesh.Vertices[i1], v2 = rMesh.Vertices[i2];
		CSection3D S0(v0, v1), S1(v1, v2), S2(v2, v0);
		i0 = rSections.find(S0), i1 = rSections.find(S1), i2 = rSections.find(S2);
		if(i0 == -1) i0 = rSections.Add(S0);
		if(i1 == -1) i1 = rSections.Add(S1);
		if(i2 == -1) i2 = rSections.Add(S2);
		rTriangles.Add(i0), rTriangles.Add(i1), rTriangles.Add(i2);
	}
}

void CMesh3D::CrossSectionsMesh(const CSections3D &rSections, const CMesh3D &rMesh, CVertex3D **ppCrosses)
{
	int NTri = rMesh.Indices.NValues / 3;
	for(int i = 0; i < rSections.NValues; i++)
	{
		CSection3D S = rSections[i];
		CVertex3D Cross;
		for(int nTri = 0; nTri < NTri; nTri++)
			if(CrossSectionTriangle(S, rMesh, nTri, Cross))
				*(*ppCrosses + rSections.NValues * nTri + i) = Cross;
	}
}

void CMesh3D::ExtractTriangles(const CMesh3D &rMesh, const CMesh3D &rClipper, const CVertex3D *pCrosses,
					  const CSections3D &rSections, const CIndices &rIndices, CMesh3D &rMeshR,
					  CIndices &rITriNumbers, bool fCriterion)
{
	CVertex3D v0, v1, v2;
	int NTri = rMesh.Indices.NValues / 3;
	for(int nTri = 0; nTri < NTri; nTri++)
	{
		rMesh.GetTriangle(v0, v1, v2, nTri);
		int nS0 = rIndices[3 * nTri + 0], nS1 = rIndices[3 * nTri + 1], nS2 = rIndices[3 * nTri + 2];
//		bool r0 = CMesh3D::VertexIsInside_ConvTest(rClipper, v0), r1 = CMesh3D::VertexIsInside_ConvTest(rClipper, v1),
//			r2 = CMesh3D::VertexIsInside_ConvTest(rClipper, v2);
		bool r0 = CMesh3D::VertexIsInside(rClipper, v0), r1 = CMesh3D::VertexIsInside(rClipper, v1),
			r2 = CMesh3D::VertexIsInside(rClipper, v2);
		bool fIntersected = false;
		int NTriClipper = rClipper.Indices.NValues / 3;
		for(int nTriClipper = 0; nTriClipper < NTriClipper; nTriClipper++)
			if((pCrosses[rSections.NValues * nTriClipper + nS0] != CVertex3D::MaxValue) ||
				(pCrosses[rSections.NValues * nTriClipper + nS1] != CVertex3D::MaxValue) ||
				(pCrosses[rSections.NValues * nTriClipper + nS2] != CVertex3D::MaxValue)) break;
		if(nTriClipper < NTriClipper) // Triangle surely intersect!
		{
			rITriNumbers.Add(nTri);
			continue;
		}
		if((r0 == fCriterion) && (r1 == fCriterion) && (r2 == fCriterion))
			if(fCriterion) rMeshR.AddTriangle(v0, v2, v1);
			else rMeshR.AddTriangle(v0, v1, v2);
	}
}

/*bool CMesh3D::VertexIsInside(const CMesh3D &rMesh, const CVertex3D &rVertex)
{
	CSection3D S(rVertex, CVertex3D(rVertex.x, rVertex.y, rVertex.z + 10000.0f));
	int NTri = rMesh.Indices.NValues / 3, nIntersections = 0;
	CVertex3D Cross;
	for(int nTri = 0; nTri = NTri; nTri++)
		if(CrossSectionTriangle(S, rMesh, nTri, Cross)) nIntersections++;
	return nIntersections & 1;
}*/

void CMesh3D::toCSections(const CMesh3D &rMeshA, const CMesh3D &rMeshB, const CIndices &rITriANumbers,
						  const CSections3D &rSectionsA, const CIndices &rIndicesA, CVertex3D *pCrossesA,
						  int NTriB, CIndices &rCSectionsANumbers, bool fCriterion)
{
	for(int i = 0; i < rITriANumbers.NValues; i++)
	{
		int nTriA = rITriANumbers[i];
		for(int j = 0; j < 3; j++)
		{
			int nS = rIndicesA[3 * nTriA + j];
			CSection3D &rS = rSectionsA[nS];
			bool r0 = VertexIsInside(rMeshB, rS.A), r1 = VertexIsInside(rMeshB, rS.B);
			if((r0 == fCriterion) && (r1 == fCriterion))
			{
				for(int nTriB = 0; nTriB < NTriB; nTriB++)
					if(pCrossesA[rSectionsA.NValues * nTriB + nS] != CVertex3D::MaxValue) break;
				if(nTriB == NTriB) rCSectionsANumbers.Add(nS);
				/*if(rCSectionsANumbers.find(nS) == -1)*/
			}
		}
	}
}

void CMesh3D::FilletSections(const CSections3D &rSectionsA, const CSections3D &rSectionsB,
							 const CIndices &rIndicesA, const CIndices &rIndicesB,
							 const CIndices &rITriANumbers, const CIndices &rITriBNumbers,
							 CVertex3D *pCrossesA, CVertex3D *pCrossesB, int NTriA, int NTriB,
							 CSections3D &rFilletSections)
{
	for(int i = 0; i < rITriANumbers.NValues; i++)
	{
		int nTriA = rITriANumbers[i];
		for(int j = 0; j < rITriBNumbers.NValues; j++)
		{
			int nTriB = rITriBNumbers[j];
			CVertex3D Crosses[32];
			int nCur = 0;
			for(int n = 0; n < 3; n++)
			{
				CVertex3D &c0 = pCrossesA[rSectionsA.NValues * nTriB + rIndicesA[3 * nTriA + n]],
					&c1 = pCrossesB[rSectionsB.NValues * nTriA + rIndicesB[3 * nTriB + n]];
				if(c0 != CVertex3D::MaxValue) Crosses[nCur] = c0, nCur++;
				if(c1 != CVertex3D::MaxValue) Crosses[nCur] = c1, nCur++;
			}
			if(nCur >= 2) rFilletSections.Add(CSection3D(Crosses[0], Crosses[1]));
		}
	}
}

void CMesh3D::toCVertices(CIndices &rCSectionsNumbers, const CSections3D &rSections,
						  CVertices3D &rCVertices)
{
	if(!rCSectionsNumbers.NValues) return;
	CSection3D &S = rSections[rCSectionsNumbers[0]];
	rCSectionsNumbers.Del(0, 1);
	CVertex3D Begin = S.A, End = S.B;
	rCVertices.Add(Begin);
	while(true)
	{
		for(int i = 0; i < rCSectionsNumbers.NValues; i++)
		{
			S = rSections[rCSectionsNumbers[i]];
			if(End == S.A)
			{
				rCVertices.Add(End);
				End = S.B;
				rCSectionsNumbers.Del(i, 1);
				break;
			}
			if(End == S.B)
			{
				rCVertices.Add(End);
				End = S.A;
				rCSectionsNumbers.Del(i, 1);
				break;
			}
		}
		if(!rCSectionsNumbers.NValues) break;
		if(End == Begin) break;
	}
}


//enum eBooleans { ebLeft, ebRight, ebBelongs };




/*
inline double dPointPoint(const cPoint &P, const cPoint &Q)
{
	double dx = Q.x - P.x;
	double dy = Q.y - P.y;
	double dz = Q.z - P.z;
	return sqrt(dx * dx + dy * dy + dz * dz);
}

double dPointSection(const cPoint &C, const cPoint &A, const cPoint &B)
{
	assert(C.GetDim() == 2 && A.GetDim() == 2 && B.GetDim() == 2);
	double l = dPointPoint(A, B);
	assert(l > -kEpsilon && l < kEpsilon);
	double s = ((A.y - C.y) * (B.x - A.x) - (A.x - C.x) * (B.y - A.y)) / (l * l);
	return s * l;
}

eBooleans ClassifyPointSection(const cPoint &C, const cPoint &A, const cPoint &B)
{
	double d = dPointSection(C, A, B);
	if(d > -kEpsilon && d < kEpsilon) return ebBelongs;
	return d > 0 ? ebRight : ebLeft;
}
*/


#define EPSILON	0.00001
#define CROSS3D(r, u, v) \
	r[0] = u[1] * v[2] - u[2] * v[1]; \
	r[1] = u[2] * v[0] - u[0] * v[2]; \
	r[2] = u[0] * v[1] - u[1] * v[0];

#define DOT2D(u, v) (u[0] * v[0] + u[1] * v[1])
#define DOT3D(u, v) (u[0] * v[0] + u[1] * v[1] + u[2] * v[2])

#define SUB2D(r, u, v) \
	r[0] = u[0] - v[0]; \
	r[1] = u[1] - v[1];
#define SUB3D(r, u, v) \
	r[0] = u[0] - v[0]; \
	r[1] = u[1] - v[1]; \
	r[2] = u[2] - v[2];

#define	ADD3D(r, u, v) \
	r[0] = u[0] + v[0]; \
	r[1] = u[1] + v[1]; \
	r[2] = u[2] + v[2];
#define MULk3D(r, k, u) \
	r[0] = k * u[0]; \
	r[1] = k * u[1]; \
	r[2] = k * u[2];

#define DIVk3D(u, k) \
	u[0] /= k, u[1] /= k, u[2] /= k;

double dVertexVertex2D(double A[2], double B[2])
{
	double D[2];
	SUB2D(D, B, A);
	return sqrt(DOT2D(D, D));
}

double dVertexVertex3D(double A[3], double B[3])
{
	double D[3];
	SUB3D(D, B, A);
	return sqrt(DOT3D(D, D));
}

#define BOOLEANS_FAILED			-1
#define BOOLEANS_LEFT			0
#define	BOOLEANS_RIGHT			1
#define	BOOLEANS_BELONGS		2
#define BOOLEANS_NOTINTERSECTS	3
#define BOOLEANS_INTERSECTS		4
#define BOOLEANS_UNKNOWN		5

class cVertex3D
{
public:
	double x, y, z;
	cVertex3D() { *this = cVertex3D::Null; }
	cVertex3D(double x, double y, double z) { Set(x, y, z); }
	void Set(double x, double y, double z)
	{ this->x = x, this->y = y, this->z; }
	static const cVertex3D Null;
};

const cVertex3D cVertex3D::Null(0., 0., 0.);

// ================================================================================
// dVertexSection2D
// ================================================================================	
int dVertexSection2D(double C[2], double A[2], double B[2], double &d)
{
	double cx = C[0], cy = C[1], ax = A[0], ay = A[1], bx = B[0], by = B[1], l, s;
	l = dVertexVertex2D(A, B);
	if(l < EPSILON) return BOOLEANS_FAILED;
	s = ((ay - cy) * (bx - ax) - (ax - cx) * (by - ay)) / (l * l);
	d = fabs(s * l);
	if(d < EPSILON) return BOOLEANS_BELONGS;
	return s > 0 ? BOOLEANS_RIGHT : BOOLEANS_LEFT;
	// BOOLEANS_FAILED, BOOLEANS_BELONGS, BOOLEANS_RIGHT, BOOLEANS_LEFT;
} // dVertexSection2D



// ================================================================================
// dSectionSection3D
// ================================================================================
int dSectionSection3D(double A[3], double AA[3], double B[3], double BB[3], double &d)
{
	double V[3], W[3], s, t, k;
	SUB3D(V, AA, A);
	SUB3D(W, BB, B);
	k = DOT3D(V, V) * DOT3D(W, W) - DOT3D(V, W) * DOT3D(V, W);
	if(k > -EPSILON && k < EPSILON) return BOOLEANS_FAILED;
	s = ((DOT3D(A, W) - DOT3D(B, W)) * DOT3D(W, V) + (DOT3D(B, V) - DOT3D(A, V)) * DOT3D(W, W)) / k;
	t = ((DOT3D(B, V) - DOT3D(A, V)) * DOT3D(W, V) + (DOT3D(A, W) - DOT3D(B, W)) * DOT3D(V, V)) / k;
	double S[3], T[3];
	MULk3D(S, s, V);
	MULk3D(T, t, W);
	double P1[3], P2[3];
	ADD3D(P1, A, S);
	ADD3D(P2, B, T);
	d = dVertexVertex3D(P1, P2);
	return d < EPSILON ? BOOLEANS_BELONGS : BOOLEANS_UNKNOWN;
	// BOOLEANS_FAILED, BOOLEANS_BELONGS, BOOLEANS_UNKNOWN
} // dSectionSection3D

// ================================================================================
// XTri
// ================================================================================
int XTri(double P[3], double Dir[3], double T0[3], double T1[3], double T2[3], double X[3])
{
	double Edge1[3], Edge2[3], PVec[3], Det, InvDet, TVec[3], QVec[3], u, v, t;
	/* find vectors for two edges sharing vert0 */
	SUB3D(Edge1, T1, T0);
	SUB3D(Edge2, T2, T0);
	/* begin calculating determinant - also used to calculate U parameter */
	CROSS3D(PVec, Dir, Edge2);
	/* if determinant is near zero, ray lies in plane of triangle */
	Det = DOT3D(Edge1, PVec);
	if(Det > -EPSILON && Det < EPSILON) return BOOLEANS_BELONGS;
	InvDet = 1.0 / Det;
	/* calculate distance from T0 to ray origin */
	SUB3D(TVec, P, T0);
	/* calculate u parameter and test bounds */
	u = DOT3D(TVec, PVec) * InvDet;
	if(u < 0.0 || u > 1.0) return BOOLEANS_NOTINTERSECTS;
	/* prepare to test v parameter */
	CROSS3D(QVec, TVec, Edge1);
	/* calculate v parameter and test bounds */
	v = DOT3D(Dir, QVec) * InvDet;
	if(v < 0 || u + v > 1.0) return BOOLEANS_NOTINTERSECTS;
	/* calculate t, ray intersects triangle */
	t = DOT3D(Edge2, QVec) * InvDet;
	if(t < 0) return BOOLEANS_NOTINTERSECTS;
	MULk3D(X, t, Dir);
	return BOOLEANS_INTERSECTS;
	// BOOLEANS_BELONGS, BOOLEANS_NOTINTERSECTS, BOOLEANS_INTERSECTS
} // XTri

int XTri(double P[3], double Dir[3], double T0[3], double T1[3], double T2[3])
{
	double Edge1[3], Edge2[3], Normal[3], w[3], nu, nw;
	// Computing normal to triangle:
	SUB3D(Edge1, T1, T0);
	SUB3D(Edge2, T2, T0);
	CROSS3D(Normal, Edge1, Edge2);
	double l = sqrt(DOT3D(Normal, Normal));
	if(l < EPSILON) return BOOLEANS_FAILED;
	DIVk3D(Normal, l);
	// Checking for parallelism:
	nu = DOT3D(Normal, Dir);
	if(nu > -EPSILON && nu < EPSILON)
	{
		SUB3D(w, P, T0);
		nw =  DOT3D(Normal, w);
		if(nw > -EPSILON && nw < EPSILON) return BOOLEANS_BELONGS;
		return BOOLEANS_NOTINTERSECTS;
	}
	double s = - nw / nu;
	if(s < 0) return BOOLEANS_NOTINTERSECTS;
//	double X[3];
//	MULk3D(s, X, Dir);
//	ADD3D(X, P, X);
//	int r0, r1, r2;
//	double d0, d1, d2;


	// BOOLEANS_FAILED, BOOLEANS_BELONGS, BOOLEANS_NOTINTERSECTS, BOOLEANS_INTERSECTS
}

#undef DIVk3D
#undef MULk3D
#undef ADD3D
#undef SUB3D
#undef SUB2D
#undef DOT3D
#undef DOT2D
#undef CROSS3D
#undef EPSILON







#define VERTEX_INSIDE	0
#define VERTEX_OUTSIDE	1
#define VERTEX_BELONGS	2
int ClassifyVertex(const CMesh3D &rMesh, const CVertex3D &rVertex, bool bUseBoundBox, int C = 0)
{
	if(C > 10) return VERTEX_BELONGS;
	const CVertex3D BBox0 = rMesh.m_BBox0, BBox1 = rMesh.m_BBox1;
	if(bUseBoundBox && (rVertex.x < BBox0.x || rVertex.x > BBox1.x || rVertex.y < BBox0.y || rVertex.y > BBox1.y ||
		rVertex.z < BBox0.z || rVertex.z > BBox1.z)) return VERTEX_OUTSIDE;
	double P[3] = {rVertex.x, rVertex.y, rVertex.z}, T0[3], T1[3], T2[3], t, u, v;

	// Computing ray direction:
	double Phi = c_DoublePI * double(rand()) / double(RAND_MAX),
		Theta = c_PI / 4. * double(rand()) / double(RAND_MAX);
	double Dir[3] = {sin(Phi) * sin(Theta), cos(Theta), cos(Phi) * sin(Theta)};

	// Testing for intersections:
	int r, NTris = rMesh.Indices.NValues / 3;
	int cIntersections = 0;
	double X[3];
	for(int nTri = 0; nTri < NTris; nTri++)
	{
		int i0 = rMesh.Indices[3 * nTri + 0], i1 = rMesh.Indices[3 * nTri + 1], i2 = rMesh.Indices[3 * nTri + 2];
		CVertex3D &v0 = rMesh.Vertices[i0], &v1 = rMesh.Vertices[i1], &v2 = rMesh.Vertices[i2];
		T0[0] = v0.x, T0[1] = v0.y, T0[2] = v0.z;
		T1[0] = v1.x, T1[1] = v1.y, T1[2] = v1.z;
		T2[0] = v2.x, T2[1] = v2.y, T2[2] = v2.z;
		r = XTri(P, Dir, T0, T1, T2, X);
		if(r == 1) cIntersections++;
		if(r == -1) return ClassifyVertex(rMesh, rVertex, bUseBoundBox, C + 1);
	}
	if(cIntersections & 1) return VERTEX_INSIDE;
	return VERTEX_OUTSIDE;
}


bool CMesh3D::Difference2(const CMesh3D &rMeshA, const CMesh3D &rMeshB, CMesh3D &rMeshR, CVertices3D &rCrosses,
						  CMesh3D &rMeshT, CSections3D &rSections)
{
	CMesh3D A = rMeshA, B = rMeshB;
	A.CalcBBox();
	B.CalcBBox();
	// Classifying vertices:
	DynArray<int> rA, rB;
	int i, r;
	int cClassifiedA = 0, cClassifiedB = 0;
	for(i = 0; i < A.Vertices.NValues; i++)
	{
		r = ClassifyVertex(B, A.Vertices[i], true);
		assert(r != VERTEX_BELONGS);
		rA.Add(r);
		if(r == VERTEX_INSIDE)
		{
			rCrosses.Add(A.Vertices[i]);
			cClassifiedA++;
		}
	}
	for(i = 0; i < B.Vertices.NValues; i++)
	{
		r = ClassifyVertex(A, B.Vertices[i], true);
		assert(r != VERTEX_BELONGS);
		rB.Add(r);
		if(r == VERTEX_OUTSIDE)
		{
			rCrosses.Add(B.Vertices[i]);
			cClassifiedB++;
		}
	}
	// Classifying sections:
	for(int i = 0; i < A.Indices.NValues - 1; i++)
	{
		int i0 = A.Indices[i], i1 = A.Indices[i + 1];
		CVertex3D &v0 = A.Vertices[i0], &v1 = A.Vertices[i1];
		int r0 = rA[i0], r1 = rA[i1];
		if(r0 == VERTEX_INSIDE && r1 == VERTEX_INSIDE)
			rSections.Add(CSection3D(v0, v1));
	}
	for(i = 0; i < B.Indices.NValues - 1; i++)
	{
		int i0 = B.Indices[i], i1 = B.Indices[i + 1];
		CVertex3D &v0 = B.Vertices[i0], &v1 = B.Vertices[i1];
		int r0 = rB[i0], r1 = rB[i1];
		if(r0 == VERTEX_OUTSIDE && r1 == VERTEX_OUTSIDE)
			rSections.Add(CSection3D(v0, v1));
	}
	// Classifying triangles:
	int NTrisA = A.Indices.NValues / 3;
	for(int nTri = 0; nTri < NTrisA; nTri++)
	{
		int i0 = A.Indices[3 * nTri + 0], i1 = A.Indices[3 * nTri + 1], i2 = A.Indices[3 * nTri + 2];
		CVertex3D &v0 = A.Vertices[i0], &v1 = A.Vertices[i1], &v2 = A.Vertices[i2];
		int r0 = rA[i0], r1 = rA[i1], r2 = rA[i2];
		if(r0 == r1 && r1 == r2 && r2 == VERTEX_INSIDE)
			rMeshR.AddTriangle(A.Vertices[i0], A.Vertices[i1], A.Vertices[i2]);
		if(r0 != r1 || r1 != r2 || r0 != r2)
			rMeshT.AddTriangle(A.Vertices[i0], A.Vertices[i1], A.Vertices[i2]);
	}
	int NTrisB = B.Indices.NValues / 3;
	for(int nTri = 0; nTri < NTrisB; nTri++)
	{
		int i0 = B.Indices[3 * nTri + 0], i1 = B.Indices[3 * nTri + 1], i2 = B.Indices[3 * nTri + 2];
		CVertex3D &v0 = B.Vertices[i0], &v1 = B.Vertices[i1], &v2 = B.Vertices[i2];
		int r0 = rB[i0], r1 = rB[i1], r2 = rB[i2];
		if(r0 == r1 && r1 == r2 && r2 == VERTEX_INSIDE)
			rMeshR.AddTriangle(B.Vertices[i0], B.Vertices[i1], B.Vertices[i2]);
		if(r0 != r1 || r1 != r2 || r0 != r2)
			rMeshT.AddTriangle(B.Vertices[i0], B.Vertices[i1], B.Vertices[i2]);
	}

	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	char Buffer[1024];
	sprintf(Buffer, "cClassifiedA = %d\r\ncClassifiedB = %d", cClassifiedA, cClassifiedB);
	MessageBox(NULL, Buffer, "--- Info ---", 0);
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	return true;
}

bool CMesh3D::Difference(const CMesh3D &rMeshA, const CMesh3D &rMeshB, CMesh3D &rMeshR, CVertices3D &rCrosses,
						 CMesh3D &rMeshT, CSections3D &rSections)
{
	bool fSuccess = true;
	int NTriA = rMeshA.Indices.NValues / 3, NTriB = rMeshB.Indices.NValues / 3;
	CSections3D SectionsA, SectionsB;

	CIndices IndicesA, IndicesB, // These are triplets of sections indices.
		CSectionsANumbers, CSectionsBNumbers;
	toSections(rMeshA, SectionsA, IndicesA);
	toSections(rMeshB, SectionsB, IndicesB);

	CVertices3D CVerticesA, CVerticesB;

	CIndices ITriANumbers, ITriBNumbers; // Numbers of surely intersected triangles.

	CVertex3D *pCrossesA = NULL, *pCrossesB = NULL;
	pCrossesA = new CVertex3D[SectionsA.NValues * NTriB];
	pCrossesB = new CVertex3D[SectionsB.NValues * NTriA];
	if(!pCrossesA || !pCrossesB) { fSuccess = false; goto Difference_unInit; }
	for(int i = 0; i < SectionsA.NValues * NTriB; i++) pCrossesA[i] = CVertex3D::MaxValue;
	for(i = 0; i < SectionsB.NValues * NTriA; i++) pCrossesB[i] = CVertex3D::MaxValue;

	CrossSectionsMesh(SectionsA, rMeshB, &pCrossesA);
	CrossSectionsMesh(SectionsB, rMeshA, &pCrossesB);

	// Extracting surely not intersected triangles to rMeshR and storing numbers of intersected:
	ExtractTriangles(rMeshA, rMeshB, pCrossesA, SectionsA, IndicesA, rMeshR, ITriANumbers, false);
	ExtractTriangles(rMeshB, rMeshA, pCrossesB, SectionsB, IndicesB, rMeshR, ITriBNumbers, true);

	toCSections(rMeshA, rMeshB, ITriANumbers, SectionsA, IndicesA, pCrossesA, NTriB, CSectionsANumbers, false);
	toCSections(rMeshB, rMeshA, ITriBNumbers, SectionsB, IndicesB, pCrossesB, NTriA, CSectionsBNumbers, true);

	for(i = 0; i < CSectionsANumbers.NValues; i++)
	{
		CSection3D &SA = SectionsA[CSectionsANumbers[i]];
		CVertex3D c((SA.A.x + SA.B.x) / 2.0f, (SA.A.y + SA.B.y) / 2.0f, (SA.A.z + SA.B.z) / 2.0f);
		float MinDist = FLOAT_MAX;
		CVertex3D vtx;
		for(int j = 0; j < CSectionsBNumbers.NValues; j++)
		{
			CSection3D &SB = SectionsB[CSectionsBNumbers[j]];
			CVertex3D v0 = SB.A, v1 = SB.B;
			float t = v0.DistanceApart(c);
			if(t < MinDist) MinDist = t, vtx = v0;
			t = v1.DistanceApart(c);
			if(t < MinDist) MinDist = t, vtx = v1;
		}
		rMeshR.AddTriangle(SA.A, SA.B, vtx);
	}
	for(i = 0; i < CSectionsBNumbers.NValues; i++)
	{
		CSection3D &SB = SectionsB[CSectionsBNumbers[i]];
		CVertex3D c((SB.A.x + SB.B.x) / 2.0f, (SB.A.y + SB.B.y) / 2.0f, (SB.A.z + SB.B.z) / 2.0f);
		float MinDist = FLOAT_MAX;
		CVertex3D vtx;
		for(int j = 0; j < CSectionsANumbers.NValues; j++)
		{
			CSection3D &SA = SectionsA[CSectionsANumbers[j]];
			CVertex3D v0 = SA.A, v1 = SA.B;
			float t = v0.DistanceApart(c);
			if(t < MinDist) MinDist = t, vtx = v0;
			t = v1.DistanceApart(c);
			if(t < MinDist) MinDist = t, vtx = v1;
		}
		rMeshR.AddTriangle(SB.A, SB.B, vtx);
	}


//	toCVertices(CSectionsANumbers, SectionsA, CVerticesA);
//	toCVertices(CSectionsBNumbers, SectionsB, CVerticesB);

	
	for(i = 0; i < CSectionsANumbers.NValues; i++)
		rSections.Add(SectionsA[CSectionsANumbers[i]]);
	for(i = 0; i < CSectionsBNumbers.NValues; i++)
		rSections.Add(SectionsB[CSectionsBNumbers[i]]);

	for(i = 0; i < SectionsA.NValues * NTriB; i++)
		if(pCrossesA[i] != CVertex3D::MaxValue) rCrosses.Add(pCrossesA[i]);
	for(i = 0; i < SectionsB.NValues * NTriA; i++)
		if(pCrossesB[i] != CVertex3D::MaxValue) rCrosses.Add(pCrossesB[i]);

Difference_unInit:
	CSectionsANumbers.Clear(), CSectionsBNumbers.Clear();
	if(pCrossesA) delete pCrossesA;
	if(pCrossesB) delete pCrossesB;
	SectionsA.Clear(), SectionsB.Clear();
	return fSuccess;

}

// ================================================================================
// CMesh3D::Booleans_Subtract
// ================================================================================
void CMesh3D::Booleans_Subtract(CMesh3D &r, CVertices3D &rCrosses, CSections3D &rSections,
								CMesh3D &rIA, CMesh3D &rIB)
{
	CMesh3D R;
	CIndices ITriANumbers, ITriBNumbers;
	Booleans_ExtractOuterTriangles(R, r, ITriANumbers);
	r.Booleans_ExtractInnerTriangles(R, *this, ITriBNumbers);

	for(int i = 0; i < ITriANumbers.NValues; i++)
	{
		int i0 = Indices[3 * ITriANumbers[i] + 0], i1 = Indices[3 * ITriANumbers[i] + 1],
			i2 = Indices[3 * ITriANumbers[i] + 2];
		CVertex3D v0 = Vertices[i0], v1 = Vertices[i1], v2 = Vertices[i2];
		i0 = rIA.Vertices.find(v0), i1 = rIA.Vertices.find(v1),
			i2 = rIA.Vertices.find(v2);
		if(i0 == -1) i0 = rIA.Vertices.Add(v0);
		if(i1 == -1) i1 = rIA.Vertices.Add(v1);
		if(i2 == -1) i2 = rIA.Vertices.Add(v2);
		rIA.AddIndices(i0, i1, i2, DWORD_MAX);
	}

	CSection3D S;
//	for(int i = 0; i < ITriANumbers.NValues; i++)
//		for(int j = 0; j < ITriBNumbers.NValues; j++)
//			if(TriTriIntersect(*this, ITriANumbers[i], r, ITriBNumbers[j], S, rCrosses))
//				rSections.Add(S);
	for(int i = 0; i < Indices.NValues / 3; i++)
		for(int j = 0; j < r.Indices.NValues / 3; j++)
			if(TriTriIntersect(*this, i, r, j, S, rCrosses)) rSections.Add(S);


	for(int i = 0; i < ITriANumbers.NValues; i++)
	{
		int nTri = ITriANumbers[i];
		int i0 = Indices[3 * nTri + 0], i1 = Indices[3 * nTri + 1],
			i2 = Indices[3 * nTri + 2];
		CVertex3D v0 = Vertices[i0], v1 = Vertices[i1], v2 = Vertices[i2];
		i0 = R.Vertices.find(v0), i1 = R.Vertices.find(v1), i2 = R.Vertices.find(v2);
		if(i0 == -1) i0 = R.Vertices.Add(v0);
		if(i1 == -1) i1 = R.Vertices.Add(v1);
		if(i2 == -1) i2 = R.Vertices.Add(v2);
		R.AddIndices(i0, i1, i2, DWORD_MAX);
	}
	for(int i = 0; i < ITriBNumbers.NValues; i++)
	{
		int nTri = ITriBNumbers[i];
		int i0 = r.Indices[3 * nTri + 0], i1 = r.Indices[3 * nTri + 1],
			i2 = r.Indices[3 * nTri + 2];
		CVertex3D v0 = r.Vertices[i0], v1 = r.Vertices[i1], v2 = r.Vertices[i2];
		i0 = R.Vertices.find(v0), i1 = R.Vertices.find(v1), i2 = R.Vertices.find(v2);
		if(i0 == -1) i0 = R.Vertices.Add(v0);
		if(i1 == -1) i1 = R.Vertices.Add(v1);
		if(i2 == -1) i2 = R.Vertices.Add(v2);
		R.AddIndices(i0, i1, i2, DWORD_MAX);
	}
	// unInit:
	ITriANumbers.Clear(), ITriBNumbers.Clear();
	Clear();
	*this = R;
	R.Clear();
}

// ================================================================================
// CMesh3D::toBaseMesh
// ================================================================================
bool CMesh3D::toBaseMesh(BaseMesh &rBaseMesh, char *strShader, char *strTexture,
						 DWORD Color, CVertex3D v3dOrigin)
{
	Vertex2t *pVertices = NULL;
	WORD *pIndices = NULL;
	rBaseMesh.create(0xFFFF, 0xFFFF);
	pVertices = (Vertex2t*)rBaseMesh.getVertexData();
	if(!pVertices) return false;
	pIndices = rBaseMesh.getIndices();
	if(!pIndices) return false;
	// Copying vertices:
	for(int i = 0; i < Vertices.NValues; i++)
		pVertices->x = Vertices[i].x + v3dOrigin.x,
			pVertices->y = Vertices[i].y + v3dOrigin.y,
			pVertices->z = Vertices[i].z + v3dOrigin.z,
			pVertices->diffuse = Color, pVertices++;
	// Copying indices:
	for(i = 0; i < Indices.NValues; i++)
		*pIndices = Indices[i], pIndices++;
	// unInitialization & Finalizing rBaseMesh:
	pVertices = NULL, pIndices = NULL;
	rBaseMesh.setNInd(Indices.NValues);
	rBaseMesh.setNVert(Vertices.NValues);
	rBaseMesh.setNPri(Indices.NValues / 3);
	rBaseMesh.setShader(IRS->GetShaderID(strShader));
	rBaseMesh.setTexture(IRS->GetTextureID(strTexture));
	return true;
} // CMesh3D::toBaseMesh

// ================================================================================
// CMesh3D::FillNormals
// ================================================================================
bool CMesh3D::FillNormals(BaseMesh &rNormals)
{
	rNormals.create(0xFFFF, 0xFFFF);
	Vertex2t *pVertices = (Vertex2t*)rNormals.getVertexData();
	if(!pVertices) return false;
	WORD *pIndices = rNormals.getIndices();
	if(!pIndices) return false;

	int NTri = Indices.NValues / 3;
	for(int nTri = 0 ; nTri < NTri; nTri++)
	{
		CVector3D v0 = Vertices[Indices[3 * nTri + 0]],
			v1 = Vertices[Indices[3 * nTri + 1]],
			v2 = Vertices[Indices[3 * nTri + 2]];
		CVector3D c = (v0 + v1 + v2) / 3.0f;
		CVector3D Normal = (v1 - v0) ^ (v2 - v1);
		Normal.Normalize();
		pVertices->x = c.x, pVertices->y = c.y, pVertices->z = c.z,
			pVertices->diffuse = c_Yellow, pVertices++;
		const float d = 30.0f;
		pVertices->x = c.x + d * Normal.x, pVertices->y = c.y + d * Normal.y,
			pVertices->z = c.z + d * Normal.z, pVertices->diffuse = c_White, pVertices++;
		*pIndices = 2 * nTri + 0, pIndices++;
		*pIndices = 2 * nTri + 1, pIndices++;
		*pIndices = 2 * nTri + 0, pIndices++;
	}
	rNormals.setNVert(2 * NTri);
	rNormals.setNInd(3 * NTri);
	rNormals.setNPri(NTri);
	rNormals.setShader(IRS->GetShaderID("WireS"));
	rNormals.setTexture(IRS->GetTextureID("white.bmp"));
}


// ================================================================================
// CMesh3D::generatorSphere
// ================================================================================
void CMesh3D::generatorSphere_CalcVertex(CVertex3D &rVertex, float R, int b, int l)
{
	float B = (float)b * c_PI / 180.0;
	float L = (float)l * c_PI / 180.0;
	rVertex.x = R * cos(B) * sin(L);
	rVertex.y = R * cos(B) * cos(L);
	rVertex.z = R * sin(B);
}
void CMesh3D::generatorSphere(CVertex3D c, float r, int db/*dLatitude*/, int dl/*dLongitude*/, CVertex3D Scale)
{
	Clear();
	CVertex3D CurVertices[4];
	for(int b = -90; b < 90; b += db)
		for(int l = 0; l < 360; l += dl)
		{
			generatorSphere_CalcVertex(CurVertices[0], r, b, l);
			generatorSphere_CalcVertex(CurVertices[1], r, b + db, l);
			generatorSphere_CalcVertex(CurVertices[2], r, b + db, l + dl);
			generatorSphere_CalcVertex(CurVertices[3], r, b, l + dl);
			int I[4];
			// Scaling && Shifting, adding unique:
			for(int n = 0; n < 4; n++)
			{
				CurVertices[n].x *= Scale.x, CurVertices[n].y *= Scale.y,
					CurVertices[n].z *= Scale.z;
				CurVertices[n].x += c.x, CurVertices[n].y += c.y,
					CurVertices[n].z += c.z;
				I[n] = Vertices.find(CurVertices[n]);
				if(I[n] == -1) I[n] = Vertices.Add(CurVertices[n]);
			}
			if(b == -90) AddIndices(I[0], I[1], I[2], DWORD_MAX);
			else
				if(b == 90 - db) AddIndices(I[0], I[1], I[3], DWORD_MAX);
				else AddIndices(I[0], I[1], I[2], I[0], I[2], I[3], DWORD_MAX);
		}
}

class xy_pair
{
public:
	double x, y;
	xy_pair() {x = 0.0f, y = 0.0f;};
	xy_pair(const xy_pair &r) { *this = r; }
	xy_pair(double v) {x = v, y = v;};
	xy_pair(double x, double y) { Set(x, y); }
	static const xy_pair null;
	static const xy_pair xy_max;
	static const xy_pair xy_min;
	static const double fToleranceDistance;
	static const double fToleranceDistance2;
	Set(double x, double y) { this->x = x, this->y = y; }
	bool operator == (const xy_pair &r) const { return (x == r.x) && (y == r.y); }
	bool operator != (const xy_pair &r) const { return !(*this == r); }
	bool operator %= (const xy_pair &r) const
	{
		double d2 = (x - r.x) * (x - r.x) + (y - r.y) * (y - r.y);
		return d2 <= xy_pair::fToleranceDistance2;
	}
	xy_pair & operator += (const xy_pair &r)
	{
		x += r.x, y += r.y;
		return *this;
	}
	xy_pair & operator -= (const xy_pair &r)
	{
		x -= r.x, y -= r.y;
		return *this;
	}
	friend xy_pair operator + (const xy_pair &, const xy_pair &);
	friend xy_pair operator - (const xy_pair &, const xy_pair &);
	friend double operator & (const xy_pair &, const xy_pair &);
	friend double operator ^ (const xy_pair &, const xy_pair &);
	friend xy_pair operator * (const double, const xy_pair &);
};

inline xy_pair operator + (const xy_pair &rA, const xy_pair &rB)
{ return xy_pair(rA.x + rB.x, rA.y + rB.y); }

inline xy_pair operator - (const xy_pair &rA, const xy_pair &rB)
{ return xy_pair(rA.x - rB.x, rA.y - rB.y); }

inline double operator & (const xy_pair &rA, const xy_pair &rB)
{ return rA.x * rB.x + rA.y * rB.y; }

inline double operator ^ (const xy_pair &rA, const xy_pair &rB)
{
	double L = sqrt(rA.x * rA.x + rA.y * rA.y) *
		sqrt(rB.x * rB.x + rB.y * rB.y);
	if(L < c_Epsilon) return 0.0f;
	double Angle = acos((rA & rB) / L);
	double z = rA.x * rB.y - rA.y * rB.x;
	if(z < 0.0f) return - Angle;
	return Angle;
}

inline xy_pair operator * (double k, const xy_pair &r)
{ return xy_pair(k * r.x, k * r.y); }

const xy_pair xy_pair::null(0.0f, 0.0f);
const xy_pair xy_pair::xy_max(DOUBLE_MAX, DOUBLE_MAX);
const xy_pair xy_pair::xy_min(DOUBLE_MIN, DOUBLE_MIN);
const double xy_pair::fToleranceDistance = 1.0f;
const double xy_pair::fToleranceDistance2 = pow(xy_pair::fToleranceDistance, 2);

class CSection2D
{
public:
	xy_pair A, B;
	DWORD Color;
	CSection2D() {A = xy_pair::null, B = xy_pair::null;}
	CSection2D(const xy_pair &A, const xy_pair &B, DWORD Color) { Set(A, B, Color); }
	CSection2D(const xy_pair &A, const xy_pair &B) { Set(A, B); }
	Set(const xy_pair &A, const xy_pair &B) { this->A = A, this->B = B; }
	Set(const xy_pair &A, const xy_pair &B, DWORD Color)
	{ this->A = A, this->B = B, this->Color = Color; }
	bool operator == (const CSection2D &r) const
	{ return (((A == r.A) && (B == r.B)) || ((B == r.A) && (A == r.B))); }
	bool operator %= (const xy_pair &rVertex) const
	{
		double L = sqrt((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y));
		if(L > c_Epsilon)
		{
			xy_pair a = B - A, b = rVertex - A;
			double s = a.y * b.x - a.x * b.y;
			double MaxX = A.x, MinX = B.x, MaxY = A.y, MinY = B.y;
			if(MinX > MaxX) MaxX = MinX, MinX = A.x;
			if(MinY > MaxY) MaxY = MinY, MinY = A.y;
			bool DimTest = ((rVertex.x >= MinX) && (rVertex.x <= MaxX) &&
				(rVertex.y >= MinY) && (rVertex.y <= MaxY));
			if(DimTest && (fabs(s) / L < xy_pair::fToleranceDistance)) return true;
		}
		return false;
	}
};

typedef DynArray<CSection2D> CSections2D;
typedef DynArray<xy_pair> CPolygon2D;
typedef DynArray<CPolygon2D> CPolygons2D;
typedef DynArray<xy_pair> CVertices2D;

class CMesh2D
{
public:
	CVertices2D Vertices;
	CIndices Indices;
	CMesh2D() {}
	CMesh2D(const CMesh2D &r)
	{
		Vertices.AddValues(r.Vertices.GetValues(), r.Vertices.NValues);
		Indices.AddValues(r.Indices.GetValues(), r.Indices.NValues);
	}
	void Release() { Vertices.Clear(), Indices.Clear(); }
	~CMesh2D() { Release(); }
	void AddVertices(xy_pair vtx0, ...)
	{
		xy_pair v = vtx0;
		va_list Vertices2Add;
		va_start(Vertices2Add, vtx0);
		while((v != xy_pair::xy_max) && (v != xy_pair::xy_min))
		{
			Vertices.Add(v);
			v = va_arg(Vertices2Add, xy_pair);
		}
		va_end(Vertices2Add);
	}
	void AddIndices(DWORD i0, ...)
	{
		DWORD i = i0;
		va_list Indices2Add;
		va_start(Indices2Add, i0);
		while(i != DWORD_MAX)
		{
			Indices.Add(i);
			i = va_arg(Indices2Add, DWORD);
		}
		va_end(Indices2Add);
	}
};

// ================================================================================
// DimTest2D
// ================================================================================	
// true - if rPolygonA is surely out of rPolygonB (or vice versa)
// false - otherwise
void DimTest2D_Dimension(const CPolygon2D &rPolygon, xy_pair &rMin, xy_pair &rMax)
{
	rMin = xy_pair::xy_max, rMax = xy_pair::xy_min;
	for(int i = 0; i < rPolygon.NValues; i++)
	{
		xy_pair c = rPolygon[i];
		if(rMin.x > c.x) rMin.x = c.x;
		if(rMax.x < c.x) rMax.x = c.x;
		if(rMin.y > c.y) rMin.y = c.y;
		if(rMax.y < c.y) rMax.y = c.y;
	}
}
bool DimTest2D(const CPolygon2D &rPolygonA, const CPolygon2D &rPolygonB)
{
	xy_pair MinA, MaxA, MinB, MaxB;
	DimTest2D_Dimension(rPolygonA, MinA, MaxA);
	DimTest2D_Dimension(rPolygonB, MinB, MaxB);
	if((MaxA.x < MinB.x) || (MinA.x > MaxB.x) ||
		(MaxA.y < MinB.y) || (MinA.y > MaxB.y)) return true;
	return false;
} // DimTest2D

bool inSection(const CSection2D &rSection, const xy_pair &rVertex)
{
	if(rSection.A.x != rSection.B.x) // Section is not vertical:
	{
		if((rVertex.x >= rSection.A.x) && (rVertex.x <= rSection.B.x))
			return true;
		if((rVertex.x >= rSection.B.x) && (rVertex.x <= rSection.A.x))
			return true;
	}
	else // Section is vertical:
	{
		if((rVertex.y >= rSection.A.y) && (rVertex.y <= rSection.B.y))
			return true;
		if((rVertex.y >= rSection.B.y) && (rVertex.y <= rSection.A.y))
			return true;
	}
	return false;
}

// ================================================================================
// SectionsCross2D 
// ================================================================================	
#define SECTIONS_DISJOINT	0
#define SECTIONS_CROSS		1
#define SECTIONS_OVERLAP	2
//#define dot(u, v)	((u).x * (v).x + (u).y * (v).y)
//#define perp(u, v)	((u).x * (v).y - (u).y * (v).x)
int SectionsCross2D(const CSection2D &rSectionA, const CSection2D &rSectionB,
				  xy_pair &rCrossA, xy_pair &rCrossB)
{
/*	xy_pair u = rSectionA.B - rSectionA.A,
		v = rSectionB.B - rSectionB.A, w = rSectionA.A - rSectionB.A;
	double d = perp(u, v);
	if(fabs(d) < c_Epsilon) // Sections are parallel:
	{
		if((perp(u, w) != 0) || (perp(v, w) != 0)) // but not collinear...
			return SECTIONS_DISJOINT;
		// Sections are collinear or degenerate:
		float du = dot(u, u), dv = dot(v, v);
		if((du == 0) && (dv == 0)) // Segments are points:
		{
			if(rSectionA.A != rSectionB.A) // but points are distinct...
				return SECTIONS_DISJOINT;
			// Segments are the same point:
			rCrossA = rSectionA.A;
			return SECTIONS_CROSS;
		}
		if(du == 0) // rSectionA is a single point:
		{
			if(!inSection(rSectionB, rSectionA.A)) // but is not in rSectionB...
				return SECTIONS_DISJOINT;
			rCrossA = rSectionA.A;
			return SECTIONS_CROSS;
		}
		if(dv == 0) // rSectionB is a single point:
		{
			if(!inSection(rSectionA, rSectionB.A)) // but is not is rSectionA...
				return SECTIONS_DISJOINT;
			rCrossA = rSectionB.A;
			return SECTIONS_CROSS;
		}
		// Section are collinear:
		float t0, t1;
		xy_pair w2 = rSectionA.B - rSectionB.A;
		if(v.x != 0) t0 = w.x / v.x, t1 = w2.x / v.x;
		else t0 = w.y / v.y, t1 = w2.y / v.y;
		if(t0 > t1) swap(t0, t1);
		if((t0 > 1) || (t1 < 0)) // no overlap...
			return SECTIONS_DISJOINT;
		t0 = t0 < 0 ? 0 : t0;
		t1 = t1 > 1 ? 1 : t1;
		if(t0 == t1) // Intersect is a point:
		{
			rCrossA = rSectionB.A + t0 * v;
			return SECTIONS_CROSS;
		}
		// They overlap in a valid subsegment:
		rCrossA = rSectionB.A + t0 * v;
		rCrossB = rSectionB.A + t1 * v;
		return SECTIONS_OVERLAP;
	}
	// The segment are skew and may intersect in a point:
	float s = perp(v, w) / d;
	if((s < 0) || (s > 1)) // no intersect with rSectionA
		return SECTIONS_DISJOINT;
	float t = perp(u, w) / d;
	if((t < 0) || (t > 1)) // no intersect with rSectionB
		return SECTIONS_DISJOINT;
	rCrossA = rSectionA.A + s * u;*/
	return SECTIONS_CROSS;
} // SectionsCross2D

// ================================================================================
// SectionVertexClassify2D
// ================================================================================
#define VERTEX_ORIGIN		0
#define VERTEX_DESTINATION	1
#define VERTEX_LEFT			2
#define VERTEX_RIGHT		3
#define VERTEX_BELONGS		4
#define VERTEX_BEHIND		5
#define VERTEX_BEYOND		6
int SectionVertexClassify2D(const CSection2D &rSection, const xy_pair &rVertex)
{
	xy_pair a = rSection.B - rSection.A, b = rVertex - rSection.A;
	double s = a.y * b.x - a.x * b.y;
	if(s > 0.0f) return VERTEX_LEFT;
	if(s < 0.0f) return VERTEX_RIGHT;
	if((a.x * b.x < 0.0f) || (a.y * b.y < 0.0f)) return VERTEX_BEHIND;
	if((a.x * a.x + a.y * a.y) < (b.x * b.x + b.y * b.y)) return VERTEX_BEYOND;
	if(rVertex == rSection.A) return VERTEX_ORIGIN;
	if(rVertex == rSection.B) return VERTEX_DESTINATION;
	return VERTEX_BELONGS;
} // SectionVertexClassify2D

// ================================================================================
// PolygonDirectionTest2D
// ================================================================================
#define DIRECTION_ANTICLOCKWISE	0
#define DIRECTION_CLOCKWISE		1
int PolygonDirectionTest2D(const CPolygon2D &rPolygon)
{
	double s = 0;
	for(int i = 0; i < rPolygon.NValues - 1; i++)
	{
		xy_pair a = rPolygon[i + 1] - rPolygon[0], b = rPolygon[i] - rPolygon[0];
		s += (a.y * b.x - a.x * b.y);
	}
	if(s < 0.0f) return DIRECTION_ANTICLOCKWISE;
	return DIRECTION_CLOCKWISE;
} // PolygonDirectionTest2D

// ================================================================================
// PolygonConvexTest2D
// ================================================================================
// true - rPolygon is convex polygon
// false - otherwise
bool PolygonConvexTest2D(const CPolygon2D &rPolygon)
{
	int NecessaryVertexPos = VERTEX_LEFT;
	if(PolygonDirectionTest2D(rPolygon) == DIRECTION_CLOCKWISE)
		NecessaryVertexPos = VERTEX_RIGHT;
	for(int i = 2; i <= rPolygon.NValues + 1; i++)
		if(SectionVertexClassify2D(CSection2D(rPolygon[i - 2],
			rPolygon[(i - 1) % rPolygon.NValues]),
			rPolygon[i % rPolygon.NValues]) != NecessaryVertexPos) return false;
	return true;
} // PolygonConvexTest2D

// ================================================================================
// PolygonVertexClassify2D
// ================================================================================
// returns VERTEX_OUTSIDE, VERTEX_INSIDE, VERTEX_BELONGS
#define VERTEX_OUTSIDE	0
#define VERTEX_INSIDE	1
int PolygonVertexClassify2D(const CPolygon2D &rPolygon, const xy_pair &rVertex)
{
	// Maybe vertex belongs?
	for(int i = 0; i < rPolygon.NValues; i++)
	{
		int r = SectionVertexClassify2D(CSection2D(rPolygon[i],
			rPolygon[(i + 1) % rPolygon.NValues]), rVertex);
		if((r == VERTEX_BELONGS) || (r == VERTEX_ORIGIN) ||
			(r == VERTEX_DESTINATION)) return VERTEX_BELONGS;
//		if(CSection2D(rPolygon[i], rPolygon[(i + 1) % rPolygon.NValues]) %= rVertex)
//			return VERTEX_BELONGS;
	}
	int NIntersections = 0;
	xy_pair A, B, xyMaxY, xyMinY;
	for(i = 0; i < rPolygon.NValues; i++)
	{
		A = rPolygon[i], B = rPolygon[(i + 1) % rPolygon.NValues];
		// Skipping section if it is:
		if(A.y == B.y) continue; // horizontal,
		if((A.y > rVertex.y) && (B.y > rVertex.y)) continue; // below,
		if((A.y < rVertex.y) && (B.y < rVertex.y)) continue; // above.
		xyMaxY = A, xyMinY = B;
		if(A.y < B.y) xyMaxY = B, xyMinY = A;
		if((xyMaxY.y == rVertex.y) && (xyMaxY.x >= rVertex.x)) NIntersections++;
		else
			if((xyMinY.y == rVertex.y) && (xyMinY.x >= rVertex.x)) continue;
			else
			{
				double t = (rVertex.y - A.y) / (B.y - A.y);
				double x = A.x + t * (B.x - A.x);
				if(x >= rVertex.x) NIntersections++;
			}
	}
	if(NIntersections & 1) return VERTEX_INSIDE;
	return VERTEX_OUTSIDE;
} // PolygonVertexClassify2D

// ================================================================================
// SectionClip2D
// ================================================================================	
typedef bool (*SectionClip2D_Criterion)(const xy_pair &, const xy_pair &);
bool SectionClip2D_xUpScendingCriterion(const xy_pair &rA, const xy_pair &rB)
{ if(rA.x > rB.x) return true; return false;}
bool SectionClip2D_xDeScendingCriterion(const xy_pair &rA, const xy_pair &rB)
{ if(rA.x < rB.x) return true; return false;}
bool SectionClip2D_yUpScendingCriterion(const xy_pair &rA, const xy_pair &rB)
{ if(rA.y > rB.y) return true; return false; }
bool SectionClip2D_yDeScendingCriterion(const xy_pair &rA, const xy_pair &rB)
{ if(rA.y < rB.y) return true; return false; }
void SectionClip2D(const CSection2D &rSection, const CPolygon2D &rPolygon,
				   CSections2D &rISections, CSections2D &rOSections,
				   bool IncludeBelongs, CVertices2D &rNodes)
{
	// Searching all cross points:
	DynArray<xy_pair> CrossPoints;
	xy_pair CrossA, CrossB;
	for(int i = 0, j = 0; i < rPolygon.NValues; i++)
	{
		int r = SectionsCross2D(rSection, CSection2D(rPolygon[i],
			rPolygon[(i + 1) % rPolygon.NValues]), CrossA, CrossB);
		if(r == SECTIONS_CROSS) CrossPoints.Add(CrossA);
		if(r == SECTIONS_OVERLAP) CrossPoints.Add(CrossA), CrossPoints.Add(CrossB);
	}
	// Adding begin and end:
	CrossPoints.Add(rSection.A), CrossPoints.Add(rSection.B);
	// Synchronizing with rNodes:
	for(i = 0; i < CrossPoints.NValues;)
	{
		double fError = DOUBLE_MAX;
		int n = 0;
		double d = 0.0f;
		for(j = 0; j < rNodes.NValues; j++)
		{
			xy_pair v0 = CrossPoints[i], v1 = rNodes[j];
			d = sqrt((v0.x - v1.x) * (v0.x - v1.x) + (v0.y - v1.y) * (v0.y - v1.y));
			if(d < fError) fError = d, n = j;
		}
		if(fError > 0.01)
			CrossPoints.Del(i, 1);
		else
		{
			CrossPoints[i] = rNodes[n];
			i++;
		}
	}
	// Deleting multiple cross points:
	DynArray<xy_pair> xyStorage;
	xy_pair xy;
	for(i = 0; i < CrossPoints.NValues; i++)
	{
		xy = CrossPoints[i];
		for(j = 0; j < xyStorage.NValues; j++) if(xy == xyStorage[j]) break; // %=
		if(j == xyStorage.NValues) xyStorage.Add(xy);
	}
	CrossPoints.Clear();
	CrossPoints.AddValues(xyStorage.GetValues(), xyStorage.NValues);
	// Selecting sort criterion:
	SectionClip2D_Criterion pfnCriterion = NULL;
	xy_pair B_A = rSection.B - rSection.A;
	if(abs(B_A.x) > abs(B_A.y))
	{
		if(rSection.A.x > rSection.B.x) pfnCriterion =
			(SectionClip2D_Criterion)SectionClip2D_xDeScendingCriterion;
		else
			pfnCriterion =
			(SectionClip2D_Criterion)SectionClip2D_xUpScendingCriterion;
	}
	else
	{
		if(rSection.A.y > rSection.B.y) pfnCriterion =
			(SectionClip2D_Criterion)SectionClip2D_yDeScendingCriterion;
		else
			pfnCriterion =
			(SectionClip2D_Criterion)SectionClip2D_yUpScendingCriterion;
	}
	// Essentially sorting:
	for(i = 0; i < CrossPoints.NValues; i++)
		for(j = 1; j < CrossPoints.NValues - i; j++)
			if(pfnCriterion(CrossPoints[j - 1], CrossPoints[j]))
			{
				xy = CrossPoints[j];
				CrossPoints[j] = CrossPoints[j - 1];
				CrossPoints[j - 1] = xy;
			}
	// Splitting sections to inside and outside:
	CSection2D S;
	for(i = 0; i < CrossPoints.NValues - 1; i++)
	{
		S.Set(CrossPoints[i], CrossPoints[i + 1]);
		xy.Set((CrossPoints[i].x + CrossPoints[i + 1].x) / 2,
			(CrossPoints[i].y + CrossPoints[i + 1].y) / 2);
		int r = PolygonVertexClassify2D(rPolygon, xy);
		if(r == VERTEX_INSIDE) rISections.Add(S);
		else
			if(r == VERTEX_OUTSIDE) rOSections.Add(S);
			else
				if(IncludeBelongs) rISections.Add(S), rOSections.Add(S); // belongs
	}
} // SectionClip2D

// ================================================================================
// PolygonOptimize2D - is using tolerance distance criterions!
// ================================================================================
void PolygonOptimize2D(CPolygon2D &rPolygon, CPolygons2D &rPolygons)
{
	if(rPolygon.NValues < 3) { rPolygon.Clear(); return; }
/*	// Deleting neighboring multiple vertices:
	for(int i = 0, j = 0; i < rPolygon.NValues; i++)
		if(rPolygon[i] %= rPolygon[(i + 1) % rPolygon.NValues])
		{
			rPolygon.Del(i, 1);
			PolygonOptimize2D(rPolygon, rPolygons);
			return;
		}
	// Deleting colinear vertices:
	for(i = 1; i <= rPolygon.NValues; i++)
		if(CSection2D(rPolygon[i - 1], rPolygon[(i + 1) % rPolygon.NValues]) %=
			rPolygon[i % rPolygon.NValues])
		{
			rPolygon.Del(i % rPolygon.NValues, 1);
			PolygonOptimize2D(rPolygon, rPolygons);
			return;
		}
	// Deleting loops:
	for(i = 1; i <= rPolygon.NValues; i++)
		if(rPolygon[i - 1] %= rPolygon[(i + 1) % rPolygon.NValues])
		{
			rPolygon.Del(i - 1, 2);
			PolygonOptimize2D(rPolygon, rPolygons);
			return;
		}
	// Detecting multiple vertices:
	for(i = 0; i < rPolygon.NValues - 1; i++)
		for(j = i + 1; j < rPolygon.NValues; j++)
			if(rPolygon[i] %= rPolygon[j])
			{
				CPolygon2D PolygonA, PolygonB;
				// PolygonA
				for(int n = i; n < j; n++) PolygonA.Add(rPolygon[n]);
				// PolygonB
				for(n = j; n < rPolygon.NValues; n++) PolygonB.Add(rPolygon[n]);
				for(n = 0; n < i; n++) PolygonB.Add(rPolygon[n]);
				PolygonOptimize2D(PolygonA, rPolygons);
				PolygonOptimize2D(PolygonB, rPolygons);
				return;
			}
	// Detecting vertex, that is too close to one of the sections:
	for(i = 0; i < rPolygon.NValues; i++)
	{
		CSection2D Section(rPolygon[i], rPolygon[(i + 1) % rPolygon.NValues]);
		for(j = 0; j < rPolygon.NValues; j++)
		{
			if((j == i) || (j == (i + 1) % rPolygon.NValues)) continue;
			if(Section %= rPolygon[j])
			{
				CPolygon2D PolygonA, PolygonB;
				// PolygonA
				for(int n = i + 1; n < rPolygon.NValues; n++) PolygonA.Add(rPolygon[n]);
				for(n = 0; n <= j; n++) PolygonA.Add(rPolygon[n]);
				// PolygonB
				for(n = j; n <= i; n++) PolygonB.Add(rPolygon[n]);
				PolygonOptimize2D(PolygonA, rPolygons);
				PolygonOptimize2D(PolygonB, rPolygons);
				return;
			}
		}
	}
	// Detecting self-intersections:
	xy_pair Cross;
	double r = 0.0f, s = 0.0f;
	for(i = 0; i < rPolygon.NValues; i++)
		for(j = 0; j < rPolygon.NValues; j++)
		{
			if(i == j) continue;
			CSection2D SectionA(rPolygon[i], rPolygon[(i + 1) % rPolygon.NValues]),
				SectionB(rPolygon[j], rPolygon[(j + 1) % rPolygon.NValues]);
			if((SectionsCross2D(SectionA, SectionB, Cross, r, s) == SECTIONS_CROSS) &&
				(r > 0.0f) && (r < 1.0f) && (s > 0.0f) && (s < 1.0f))
			{
				CPolygon2D PolygonA, PolygonB;
				// PolygonA
				PolygonA.Add(Cross);
				for(int n = j + 1; n < rPolygon.NValues; n++) PolygonA.Add(rPolygon[n]);
				for(n = 0; n <= i; n++) PolygonA.Add(rPolygon[n]);
				// PolygonB
				PolygonB.Add(Cross);
				for(n = i + 1; n <= j; n++) PolygonB.Add(rPolygon[n]);
				PolygonOptimize2D(PolygonA, rPolygons);
				PolygonOptimize2D(PolygonB, rPolygons);
				return;
			}
		}*/
	// rPolygon has valid geometry, so we can add it to rPolygons:
	CPolygon2D P;
	rPolygons.Add(P);
	if(PolygonDirectionTest2D(rPolygon) == DIRECTION_CLOCKWISE)
		for(int i = rPolygon.NValues - 1; i >= 0; i--)
			rPolygons[rPolygons.NValues - 1].Add(rPolygon[i]);
	else rPolygons[rPolygons.NValues - 1].AddValues(rPolygon.GetValues(), rPolygon.NValues);
} // PolygonOptimize2D

// ================================================================================
// Booleans2D
// ================================================================================
bool Booleans2D_Close(CPolygon2D &rPolygon, CSections2D &rSections,
					  xy_pair &rBegin, xy_pair &rEnd)
{
	// Calculating error estimates and searching minimal:
	DWORD n = 0;
	double MinError = DOUBLE_MAX;
	for(int i = 0; i < rSections.NValues; i++)
	{
		double fError = (rEnd.x - rSections[i].A.x) * (rEnd.x - rSections[i].A.x) +
			(rEnd.y - rSections[i].A.y) * (rEnd.y - rSections[i].A.y);
		if(fError < MinError) MinError = fError, n = i;
	}
	// Begin - End distance:
	double dBeginEnd = ((rEnd.x - rBegin.x) * (rEnd.x - rBegin.x) +
		(rEnd.y - rBegin.y) * (rEnd.y - rBegin.y));
/*	if((MinError > xy_pair::fToleranceDistance2) && (dBeginEnd > xy_pair::fToleranceDistance2))
	{
		rPolygon.Add(rEnd);
		rEnd = rPolygon[rPolygon.NValues - 2];
		return false;
	}*/
	// Maybe we need to finish current contour:
	if((dBeginEnd <= MinError) || (!rSections.NValues)) return true;
	// Extending contour:
	rPolygon.Add(rEnd);
	rEnd = rSections[n].B;
	rSections.Del(n, 1);
	// Recurring:
//	const int MaxI = 100;
//	for(int I = 0; I < MaxI; I++) if(Booleans2D_Close(rPolygon, rSections, rBegin, rEnd)) break;
//	if(I == MaxI) return false;
	Booleans2D_Close(rPolygon, rSections, rBegin, rEnd);
	// Storing:
//	rPolygon.Clear(), rPolygon.AddValues(rPolygon.GetValues(), rPolygon.NValues);
//	rSections.Clear(), rSections.AddValues(rSections.GetValues(), rSections.NValues);
	return true;
}
void Booleans2D(const CPolygon2D &rPolygonA, const CPolygon2D &rPolygonB,
				CPolygons2D &rPolygons, EBooleans Mode, CVertices2D &rNodes)
{
	if(Mode == NOP) return;
	// The simplest cases:
	if(DimTest2D(rPolygonA, rPolygonB)) // Polygons cannot intersect each other!
	{
		if(Mode == ADifferenceB) rPolygons.Add(rPolygonA);
		if(Mode == BDifferenceA) rPolygons.Add(rPolygonB);
		if(Mode == Union) rPolygons.Add(rPolygonA), rPolygons.Add(rPolygonB);
		return;
	}
	CSections2D ISectionsA, OSectionsA, ISectionsB, OSectionsB;
	// Inner/Outer Clipping:
	bool IncludeBelongsA = true, IncludeBelongsB = true;
	if(Mode == ADifferenceB) IncludeBelongsB = false;
	if(Mode == BDifferenceA) IncludeBelongsA = false;
	// Clipper: rPolygonB
	// Cutting: rPolygonA
	for(int i = 0, j = 0; i < rPolygonA.NValues; i++)
		SectionClip2D(CSection2D(rPolygonA[i], rPolygonA[(i + 1) % rPolygonA.NValues]),
		rPolygonB, ISectionsB, OSectionsB, IncludeBelongsB, rNodes);
	// Clipper: rPolygonA
	// Cutting: rPolygonB
	for(i = 0; i < rPolygonB.NValues; i++)
		SectionClip2D(CSection2D(rPolygonB[i], rPolygonB[(i + 1) % rPolygonB.NValues]),
		rPolygonA, ISectionsA, OSectionsA, IncludeBelongsA, rNodes);

	// Merging and combining I/OSectionsA && I/OSectionsB as needed:
	CSections2D Sections;
	switch(Mode)
	{
	case Intersection:
		Sections.AddValues(ISectionsA.GetValues(), ISectionsA.NValues);
		Sections.AddValues(ISectionsB.GetValues(), ISectionsB.NValues);
		break;
	case Union:
		Sections.AddValues(OSectionsA.GetValues(), OSectionsA.NValues);
		Sections.AddValues(OSectionsB.GetValues(), OSectionsB.NValues);
		break;
	case ADifferenceB:
		Sections.AddValues(OSectionsB.GetValues(), OSectionsB.NValues);
		for(i = 0; i < ISectionsA.NValues; i++)
			Sections.Add(CSection2D(ISectionsA[i].B, ISectionsA[i].A));
		break;
	case BDifferenceA:
		Sections.AddValues(OSectionsA.GetValues(), OSectionsA.NValues);
		for(i = 0; i < ISectionsB.NValues; i++)
			Sections.Add(CSection2D(ISectionsB[i].B, ISectionsB[i].A));
		break;
	default: break;
	}
	// Deleting multiple sections:
	CSections2D TSections;
	CSection2D S;
	for(i = 0; i < Sections.NValues; i++)
	{
		S = Sections[i];
		for(j = 0; j < TSections.NValues; j++) if(TSections[j] == S) break;
		if(j == TSections.NValues) TSections.Add(S);
	}
	Sections.Clear(), Sections.AddValues(TSections.GetValues(), TSections.NValues);
	if(TSections.NValues < 3) return;
	// Deleting sections, which may cause deadlock:
/*	for(i = 0; i < Sections.NValues;)
	{
		for(j = 0; j < Sections.NValues; j++)
		{
			if(i == j) continue;
			if(Sections[i].B %= Sections[j].A) break;
		}
		if(j == Sections.NValues)
			Sections.Del(i, 1);
		else i++;
	}*/
	// Creating resulting polygons:
	CPolygon2D CurPolygon;
	while(Sections.NValues)
	{
		S = Sections[0];
		xy_pair Begin = S.A, End = S.B;
		Sections.Del(0, 1);
		CurPolygon.Add(Begin);
		Booleans2D_Close(CurPolygon, Sections, Begin, End);
		PolygonOptimize2D(CurPolygon, rPolygons);
		CurPolygon.Clear();
	}
}

// ================================================================================
// Booleans2D_ExtractUniqueSections
// ================================================================================
void Booleans2D_ExtractUniqueSections(const CMesh2D &rMesh, CSections2D &rSections)
{
	rSections.Clear();
	int NTri = rMesh.Indices.NValues / 3;
	for(int nTri = 0; nTri < NTri; nTri++)
	{
		DWORD vi0 = rMesh.Indices[3 * nTri + 0], vi1 = rMesh.Indices[3 * nTri + 1],
			vi2 = rMesh.Indices[3 * nTri + 2];
		xy_pair A = rMesh.Vertices[vi0], B = rMesh.Vertices[vi1], C = rMesh.Vertices[vi2];
		CSection2D S0(A, B), S1(B, C), S2(C, A);
		int i0 = rSections.find(S0), i1 = rSections.find(S1), i2 = rSections.find(S2);
		if(i0 == -1) rSections.Add(S0);
		if(i1 == -1) rSections.Add(S1);
		if(i2 == -1) rSections.Add(S2);
	}
} // Booleans2D_ExtractUniqueSections

// ================================================================================
// Booleans2D_SubSections
// ================================================================================
typedef bool (*Booleans2D_SubSections_Criterion)(const xy_pair &, const xy_pair &);
bool Booleans2D_SubSections_xCriterion(const xy_pair &rA, const xy_pair &rB)
{ return rA.x > rB.x; }
bool Booleans2D_SubSections_yCriterion(const xy_pair &rA, const xy_pair &rB)
{ return rA.y > rB.y; }
void Booleans2D_SubSections_Glue(const CSection2D &rSection, CVertices2D &rCrosses,
								 CSections2D &rSubSections)
{
	// Selecting sort criterion:
	Booleans2D_SubSections_Criterion pfnCriterion = NULL;
	xy_pair B_A = rSection.B - rSection.A;
	if(fabs(B_A.x) > fabs(B_A.y)) pfnCriterion =
		(Booleans2D_SubSections_Criterion)Booleans2D_SubSections_xCriterion;
	else pfnCriterion = (Booleans2D_SubSections_Criterion)Booleans2D_SubSections_yCriterion;
	// Sorting:
	for(int n = 0; n < rCrosses.NValues; n++)
		for(int l = 1; l < rCrosses.NValues - n; l++)
			if(pfnCriterion(rCrosses[l - 1], rCrosses[l]))
				swap(rCrosses[l - 1], rCrosses[l]);
	for(n = 0; n < rCrosses.NValues - 1; n++)
		rSubSections.Add(CSection2D(rCrosses[n], rCrosses[n + 1]));
}


#define OWNER_A		0
#define OWNER_B		1
#define OWNER_AB	10

void Booleans2D_SubSections(const CSections2D &rSectionsA,
							const CSections2D &rSectionsB, CSections2D &rSubSections,
							CVertices2D &rVertices, DynArray<int> &rOwner)
{
	double r = 0.0f, s = 0.0f;
	xy_pair CrossA, CrossB;
	CVertices2D Crosses;
	CSection2D S;
	CVertices2D *pSectionsBCrosses = NULL;
	pSectionsBCrosses = new CVertices2D[rSectionsB.NValues];
	if(!pSectionsBCrosses) return;
	for(int i = 0; i < rSectionsA.NValues; i++)
	{
		S = rSectionsA[i];
		for(int j = 0; j < rSectionsB.NValues; j++)
		{
			int r = SectionsCross2D(S, rSectionsB[j], CrossA, CrossB);
			if(r == SECTIONS_CROSS) Crosses.Add(CrossA),
				pSectionsBCrosses[j].Add(CrossA);
			if(r == SECTIONS_OVERLAP) Crosses.Add(CrossB),
				pSectionsBCrosses[j].Add(CrossB);
		}

		for(int q = 0; q < Crosses.NValues; q++)
		{
			int i0 = rVertices.find(Crosses[q]);
			if(i0 == -1) rVertices.Add(Crosses[q]), rOwner.Add(OWNER_AB);
			else rOwner[i0] = OWNER_AB;
		}

		Crosses.Add(S.A), Crosses.Add(S.B);
		Booleans2D_SubSections_Glue(S, Crosses, rSubSections);
		Crosses.Clear();
	}
	for(i = 0; i < rSectionsB.NValues; i++)
	{
		S = rSectionsB[i];
		pSectionsBCrosses[i].Add(S.A), pSectionsBCrosses[i].Add(S.B);
		Booleans2D_SubSections_Glue(S, pSectionsBCrosses[i], rSubSections);
		pSectionsBCrosses[i].Clear();
	}
	// unInit...
	delete[] pSectionsBCrosses;
} // Booleans2D_SubSections

typedef struct {xy_pair Direction; int nNode;} SBooleansNodeEntry2D;
typedef DynArray<SBooleansNodeEntry2D> CBooleansNode2D;
// ================================================================================
// Booleans2D_Nodes
// ================================================================================
void Booleans2D_Nodes(const CSections2D &rSubSections,
					 CVertices2D &rNodesVertices, CBooleansNode2D **ppNodes)
{
	CSection2D S;
	for(int i = 0; i < rSubSections.NValues; i++)
	{
		S = rSubSections[i];
		if(rNodesVertices.find(S.A) == -1) rNodesVertices.Add(S.A);
		if(rNodesVertices.find(S.B) == -1) rNodesVertices.Add(S.B);
	}
	*ppNodes = new CBooleansNode2D[rNodesVertices.NValues];
	if(!*ppNodes) return;
	SBooleansNodeEntry2D Entry;
	for(i = 0; i < rSubSections.NValues; i++)
	{
		S = rSubSections[i];
		int t0 = rNodesVertices.find(S.A), t1 = rNodesVertices.find(S.B);
		xy_pair Direction = S.B - S.A;
		double L = sqrt(Direction.x * Direction.x + Direction.y * Direction.y);
		Direction.x /= L, Direction.y /= L;
		// t0
		Entry.Direction = Direction, Entry.nNode = t1;
		(*ppNodes)[t0].Add(Entry);
		// t1
		Entry.Direction.x = - Direction.x, Entry.Direction.y = - Direction.y,
			Entry.nNode = t0;
		(*ppNodes)[t1].Add(Entry);
	}
} // Booleans2D_Nodes

void Booleans2D_FixContour(CIndices &rCurContour, const CVertices2D &rVertices,
						   CIndices &rContours)
{
	if(rCurContour.NValues < 3)
	{
		rCurContour.Clear();
		return;
	}
	// Deleting neighboring multiple vertices:
	for(int i = 0, j = 0; i < rCurContour.NValues; i++)
		if(rVertices[rCurContour[i]] %=
			rVertices[rCurContour[(i + 1) % rCurContour.NValues]])
		{
			rCurContour.Del(i, 1);
			Booleans2D_FixContour(rCurContour, rVertices, rContours);
			return;
		}
	// Deleting colinear vertices:
	for(int i = 1; i <= rCurContour.NValues; i++)
		if(CSection2D(rVertices[rCurContour[i - 1]],
			rVertices[rCurContour[(i + 1) % rCurContour.NValues]]) %=
			rVertices[rCurContour[i % rCurContour.NValues]])
		{
			rCurContour.Del(i % rCurContour.NValues, 1);
			Booleans2D_FixContour(rCurContour, rVertices, rContours);
				return;
			}
	// Deleting loops:
	for(i = 1; i <= rCurContour.NValues; i++)
		if(rVertices[rCurContour[i - 1]] == rVertices[rCurContour[(i + 1) % rCurContour.NValues]])
		{
			rCurContour.Del(i - 1, 2);
			Booleans2D_FixContour(rCurContour, rVertices, rContours);
			return;
		}
	// Detecting multiple vertices:
	for(i = 0; i < rCurContour.NValues - 1; i++)
		for(j = i + 1; j < rCurContour.NValues; j++)
			if(rVertices[rCurContour[i]] %= rVertices[rCurContour[j]])
			{
				CIndices A, B;
				// A
				for(int n = i; n < j; n++) A.Add(rCurContour[n]);
				// B
				for(n = j; n < rCurContour.NValues; n++) B.Add(rCurContour[n]);
				for(n = 0; n < i; n++) B.Add(rCurContour[n]);
				Booleans2D_FixContour(A, rVertices, rContours);
				Booleans2D_FixContour(B, rVertices, rContours);
				return;
			}
	// Contour is valid, so we can add it to rContours:
	rContours.AddValues(rCurContour.GetValues(), rCurContour.NValues);
	rContours.Add(DWORD_MAX);
}

// ================================================================================
// Booleans2D_Contours
// ================================================================================
void Booleans2D_Contours_Close(int nFirstNode, int t, CBooleansNode2D *pNodes,
							   CIndices &rContours, const CVertices2D &rNodesVertices,
							   const CSections2D &rSubSections,
							   const CVertices2D &rMidVertices)
{
	CIndices CurContour;
	xy_pair InDir = pNodes[nFirstNode][t].Direction;
	int nInNode = nFirstNode;
	int nCurNode = pNodes[nFirstNode][t].nNode;
	CurContour.Add(nInNode);
	int Steps = 1;
	while(true)
	{
		CurContour.Add(nCurNode);
		double fLeftMost = DOUBLE_MAX;
		for(int i = 0; i < pNodes[nCurNode].NValues; i++)
		{
			if(nInNode == pNodes[nCurNode][i].nNode) continue;
			double fAngle = InDir ^ pNodes[nCurNode][i].Direction;
			if(fAngle < fLeftMost) fLeftMost = fAngle, t = i;
		}
		nInNode = nCurNode;
		InDir = pNodes[nCurNode][t].Direction;
		nCurNode = pNodes[nCurNode][t].nNode;
		if(nCurNode == nFirstNode) break;
		Steps++;
		if(Steps >= rNodesVertices.NValues) break;
	}
	if(Steps < rNodesVertices.NValues)
	{
		// Polygon must have anticlockwise direction:
		CPolygon2D Polygon;
		for(int i = 0; i < CurContour.NValues; i++)
			Polygon.Add(rNodesVertices[CurContour[i]]);
		if(PolygonDirectionTest2D(Polygon) == DIRECTION_ANTICLOCKWISE)
		{
			// Should be no vertex inside contour:
/*			for(i = 0; i < rMidVertices.NValues; i++)
				if(PolygonVertexClassify2D(Polygon, rMidVertices[i]) == VERTEX_INSIDE)
				{
					CurContour.Clear();
					return;
				}*/

			// Sorting indices:
			DWORD MinIndex = DWORD_MAX;
			int posMinIndex = 0;
			for(i = 0; i < CurContour.NValues; i++)
				if(CurContour[i] < MinIndex)
					MinIndex = CurContour[i], posMinIndex = i;
			for(i = 0; i < posMinIndex; i++)
			{
				DWORD dw = CurContour[0];
				CurContour.Del(0, 1);
				CurContour.Add(dw);
			}
			// Maybe this contour is already in rContours?
			bool Absent = true;
			for(int n = 0; n < rContours.NValues; n++)
			{
				bool StartComparing = false;
				if((n == 0) && (rContours[0] == CurContour[0])) StartComparing = true;
				if((n > 0) && (rContours[n - 1] == DWORD_MAX) &&
					(rContours[n] == CurContour[0])) StartComparing = true;
				if(StartComparing)
				{
					for(i = 1; i < CurContour.NValues; i++)
						if(rContours[n + i] != CurContour[i]) break;
					if(i == CurContour.NValues)
					{
						Absent = false;
						break;
					}
					n += i;
				}
			}
			if(Absent)
				Booleans2D_FixContour(CurContour, rNodesVertices, rContours);
//				rContours.AddValues(CurContour.GetValues(), CurContour.NValues),
//				rContours.Add(DWORD_MAX);
		}
	}
	CurContour.Clear();
}
void Booleans2D_Contours(const CVertices2D &rNodesVertices,
						 CBooleansNode2D *pNodes, CIndices &rContours,
						 const CSections2D &rSubSections,
						 const CVertices2D &rMidVertices)
{
	rContours.Clear();
	for(int i = 0; i < rNodesVertices.NValues; i++)
		if(pNodes[i].NValues == 2)
		{
			xy_pair Dir0 = pNodes[i][0].Direction, Dir1 = pNodes[i][1].Direction;
			xy_pair Normal = Dir0 + Dir1;
			double L = sqrt(Normal.x * Normal.x + Normal.y * Normal.y);
			if(L < c_Epsilon) continue;
			Normal.x /= L, Normal.y /= L;
			int t = ((Normal ^ Dir0) > 0.0f) ? 0 : 1;
			Booleans2D_Contours_Close(i, t, pNodes, rContours, rNodesVertices,
				rSubSections, rMidVertices);
		}
		else
			for(int t = 0; t < pNodes[i].NValues; t++)
				Booleans2D_Contours_Close(i, t, pNodes, rContours, rNodesVertices,
				rSubSections, rMidVertices);
}

// ================================================================================
// Booleans2D_Triangulate_ConvexTest
// ================================================================================
bool Booleans2D_Triangulate_ConvexTest(const CIndices &rContour,
									   const CVertices2D &rVertices)
{
	for(int i = 2; i <= rContour.NValues + 1; i++)
	{
		CSection2D Section(rVertices[rContour[i - 2]],
			rVertices[rContour[(i - 1) % rContour.NValues]]);
		xy_pair Vertex(rVertices[rContour[i % rContour.NValues]]);
		if(SectionVertexClassify2D(Section, Vertex) != VERTEX_LEFT) return false;
	}
	return true;
} // Booleans2D_Triangulate_ConvexTest

// ================================================================================
// Booleans2D_Triangulate
// ================================================================================
void Booleans2D_Triangulate_LShift(CIndices &rCurContour)
{
	if(!rCurContour.NValues) return;
	DWORD i = rCurContour[0];
	rCurContour.Del(0, 1);
	rCurContour.Add(i);
}


void Booleans2D_Triangulate(CIndices &rCurContour, CMesh2D &rMesh)
{
	if(!rCurContour.NValues) return;
	if(rCurContour.NValues == 3) // Polygon already is a triangle:
	{
		rMesh.Indices.AddValues(rCurContour.GetValues(), rCurContour.NValues);
		return;
	}
	if(Booleans2D_Triangulate_ConvexTest(rCurContour, rMesh.Vertices)) // Polygon is convex:
	{
		for(int i = 0; i < 3; i++) rMesh.Indices.Add(rCurContour[i]);
		rCurContour.Del(1, 1);
		Booleans2D_Triangulate(rCurContour, rMesh);
		return;
	}
	// Polygon is nonconvex...
	int T = 0;
	while(SectionVertexClassify2D(CSection2D(rMesh.Vertices[rCurContour[rCurContour.NValues - 1]],
		rMesh.Vertices[rCurContour[0]]), rMesh.Vertices[rCurContour[1]]) != VERTEX_RIGHT)
	{
		Booleans2D_Triangulate_LShift(rCurContour);
		T++;
		if(T > rCurContour.NValues) return;
	}
	int k = 2; // k should be in [2; NValues - 2]
	// vertex should be to the left:
	while(SectionVertexClassify2D(CSection2D(rMesh.Vertices[rCurContour[0]],
		rMesh.Vertices[rCurContour[1]]), rMesh.Vertices[rCurContour[k]]) != VERTEX_LEFT)
	{
		k++;
		if(k > rCurContour.NValues - 2) return;
	}
	// Splitting contour with diagonal Polygon[0] - Polygon[k]:
	CIndices ContourA, ContourB;
	for(int i = 0; i <= k; i++) ContourA.Add(rCurContour[i]);
	for(i = k; i <= rCurContour.NValues; i++) ContourB.Add(rCurContour[i % rCurContour.NValues]);
	Booleans2D_Triangulate(ContourA, rMesh);
	ContourA.Clear();
	Booleans2D_Triangulate(ContourB, rMesh);
	ContourB.Clear();
}

// ================================================================================
// MeshVertexClassify2D
// ================================================================================
int MeshVertexClassify2D(const CMesh2D &rMesh, const xy_pair &rVertex)
{
	int NTri = rMesh.Indices.NValues / 3;
	CPolygon2D P;
	for(int nTri = 0; nTri < NTri; nTri++)
	{
		int i0 = rMesh.Indices[3 * nTri + 0],
			i1 = rMesh.Indices[3 * nTri + 1],
			i2 = rMesh.Indices[3 * nTri + 2];
		CSection2D SA(rMesh.Vertices[i0], rMesh.Vertices[i1]),
			SB(rMesh.Vertices[i1], rMesh.Vertices[i2]),
			SC(rMesh.Vertices[i2], rMesh.Vertices[i0]);
        int rA = SectionVertexClassify2D(SA, rVertex),
			rB = SectionVertexClassify2D(SB, rVertex),
			rC = SectionVertexClassify2D(SC, rVertex);
/*		if((rA == VERTEX_BELONGS) || (rA == VERTEX_ORIGIN) ||
			(rA = VERTEX_DESTINATION)) rA = VERTEX_LEFT;
		if((rB == VERTEX_BELONGS) || (rB == VERTEX_ORIGIN) ||
			(rB == VERTEX_DESTINATION)) rB = VERTEX_LEFT;
		if((rC == VERTEX_BELONGS) || (rC == VERTEX_ORIGIN) ||
			(rC == VERTEX_DESTINATION)) rC = VERTEX_LEFT;*/
		if(((rA == VERTEX_LEFT) && (rB == VERTEX_LEFT) &&
			(rC == VERTEX_LEFT)) || (rA == VERTEX_BELONGS) ||
			(rB == VERTEX_BELONGS) || (rC == VERTEX_BELONGS)) return VERTEX_INSIDE;
	}
	return VERTEX_OUTSIDE;
}

int MeshSectionClassify2D(const CMesh2D &rMesh, const CSection2D &rSection)
{
	xy_pair c = rSection.A + rSection.B;
	c.x /= 2.0f, c.y /= 2.0f;
	if(MeshVertexClassify2D(rMesh, c) == VERTEX_INSIDE) return VERTEX_INSIDE;
	int NTri = rMesh.Indices.NValues / 3;
	xy_pair CrossA, CrossB;
	for(int nTri = 0; nTri < NTri; nTri++)
	{
		int i0 = rMesh.Indices[3 * nTri + 0], i1 = rMesh.Indices[3 * nTri + 1],
			i2 = rMesh.Indices[3 * nTri + 2];
		xy_pair A = rMesh.Vertices[i0], B = rMesh.Vertices[i1],
			C = rMesh.Vertices[i2];
		CSection2D S0(A, B), S1(B, C), S2(C, A);
		int r0 = SectionsCross2D(S0, rSection, CrossA, CrossB),
			r1 = SectionsCross2D(S1, rSection, CrossA, CrossB),
			r2 = SectionsCross2D(S2, rSection, CrossA, CrossB);
		if((r0 == SECTIONS_OVERLAP) || (r1 == SECTIONS_OVERLAP) ||
			(r2 == SECTIONS_OVERLAP)) return VERTEX_INSIDE;
	}
}

// ================================================================================
// Booleans2D
// ================================================================================
void Booleans2D(const CMesh2D &rMeshA, const CMesh2D &rMeshB,
				CMesh2D &rMeshR, EBooleans Mode, CSections2D &rSections)
{
	rMeshR.Release();
	if(Mode == NOP) return;
	// EXTRACTING UNIQUE SECTIONS
	// from rMeshA \ rMeshB to SectionsA \ SectionsB
	CSections2D SectionsA, SectionsB;
	Booleans2D_ExtractUniqueSections(rMeshA, SectionsA);
	Booleans2D_ExtractUniqueSections(rMeshB, SectionsB);
	CVertices2D Vertices;
	DynArray<int> Owner;
	for(int i = 0; i < SectionsA.NValues; i++)
	{
		CSection2D &rS = SectionsA[i];
		int i0 = Vertices.find(rS.A), i1 = Vertices.find(rS.B);
		if(i0 == -1) Vertices.Add(rS.A), Owner.Add(OWNER_A);
		if(i1 == -1) Vertices.Add(rS.B), Owner.Add(OWNER_A);
	}
	for(int i = 0; i < SectionsB.NValues; i++)
	{
		CSection2D &rS = SectionsB[i];
		int i0 = Vertices.find(rS.A), i1 = Vertices.find(rS.B);
		if(i0 == -1) Vertices.Add(rS.A), Owner.Add(OWNER_B);
		else if(Owner[i0] == OWNER_A) Owner[i0] = OWNER_AB;
		if(i1 == -1) Vertices.Add(rS.B), Owner.Add(OWNER_B);
		else if(Owner[i1] == OWNER_A) Owner[i1] = OWNER_AB;
	}
	// ADDITIONAL OWNER CHECK
	int NTri = 0;
	for(i = 0; i < Vertices.NValues; i++)
	{
		if(Owner[i] == OWNER_A)
			if(MeshVertexClassify2D(rMeshB, Vertices[i]) == VERTEX_INSIDE)
				Owner[i] = OWNER_AB;
		if(Owner[i] == OWNER_B)
			if(MeshVertexClassify2D(rMeshA, Vertices[i]) == VERTEX_INSIDE)
				Owner[i] = OWNER_AB;
	}
	// SUBDIVIDING SECTIONS
	// to surely not crossed sections
	CSections2D SubSections;
	Booleans2D_SubSections(SectionsA, SectionsB, SubSections, Vertices, Owner);
	SectionsA.Clear(), SectionsB.Clear();
	for(i = 0; i < SubSections.NValues; i++)
	{
		CSection2D S = SubSections[i];
//		int iA = Vertices.find(S.A), iB = Vertices.find(S.B);
//		int rA = Owner[iA], rB = Owner[iB];
//		if(rA == OWNER_AB) rA = OWNER_A;
//		if(rB == OWNER_AB) rB = OWNER_A;
//		if(((rA == OWNER_A) && (rB == OWNER_AB)) ||
//			((rA == OWNER_AB) && (rB == OWNER_A))) S.Color = c_Red;
//		else S.Color = c_White;
		if(MeshSectionClassify2D(rMeshA, S) == VERTEX_INSIDE) S.Color = c_Red;
		else S.Color = c_White;
		rSections.Add(S);
	}
	return;
//	CVertices2D MidVertices;
//	for(int i = 0; i < SubSections.NValues; i++)
//		MidVertices.Add(xy_pair((SubSections[i].A.x + SubSections[i].B.x) / 2,
//		(SubSections[i].A.y + SubSections[i].B.y) / 2));

	for(i = 0; i < SubSections.NValues; i++)
	{
		CSection2D S = SubSections[i];
		S.Color = c_White;
		int iA = Vertices.find(S.A), iB = Vertices.find(S.B);
		if((iA == -1) || (iB == -1)) continue;
		int oA = Owner[iA], oB = Owner[iB];
		xy_pair c = S.A + S.B;
		c.x /= 2.0f, c.y /= 2.0f;
		int rA = 0, rB = 0;

		switch(Mode)
		{
		case Intersection:
			rA = MeshVertexClassify2D(rMeshA, c),
				rB = MeshVertexClassify2D(rMeshB, c);
			if((rA == VERTEX_INSIDE) && (rB == VERTEX_INSIDE))
				rSections.Add(S);
			break;
		case ADifferenceB:
//			if(rA == VERTEX_BELONGS) rA = VERTEX_INSIDE;
//			if(rB == VERTEX_BELONGS) rB = VERTEX_OUTSIDE;
//			if((rA == VERTEX_INSIDE) && (rB == VERTEX_OUTSIDE))// || (rB == VERTEX_BELONGS)))
//				rSections.Add(S);
			break;
		case BDifferenceA:
//			r = MeshVertexClassify2D(rMeshA, c);
//			if((r == VERTEX_OUTSIDE) || (r == VERTEX_BELONGS)) rSections.Add(S);
			break;
		default:
			break;
		}
	}
	return;
/*
	// FORMING NODES
	CVertices2D NodesVertices;
	CBooleansNode2D *pNodes = NULL;
	Booleans2D_Nodes(SubSections, NodesVertices, &pNodes);
	if(!pNodes) return;
	// FORMING CLOSED CONTOURS
	// separated by DWORD_MAX
	CIndices Contours;
	Booleans2D_Contours(NodesVertices, pNodes, Contours, SubSections, MidVertices);
	SubSections.Clear();
	MidVertices.Clear();
	if(pNodes)
	{
		for(int i = 0; i < NodesVertices.NValues; i++) pNodes[i].Clear();
		delete[] pNodes;
	}
	DynArray<int> NodesVerticesBelongs;
	for(i = 0; i < NodesVertices.NValues; i++)
	{
		int r0 = MeshVertexClassify2D(rMeshA, NodesVertices[i]);
		int r1 = MeshVertexClassify2D(rMeshB, NodesVertices[i]);
		if((r0 == VERTEX_INSIDE) && (r1 == VERTEX_OUTSIDE)) NodesVerticesBelongs.Add(0);
		if((r0 == VERTEX_OUTSIDE) && (r1 == VERTEX_INSIDE)) NodesVerticesBelongs.Add(1);
		if((r0 == VERTEX_INSIDE) && (r1 == VERTEX_INSIDE)) NodesVerticesBelongs.Add(10);
		if((r0 == VERTEX_OUTSIDE) && (r1 == VERTEX_OUTSIDE)) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
			MessageBox(NULL, "r0 && r1 == VERTEX_OUTSIDE", "Booleans2D", 0);
	}
	P.Clear();
	DynArray<int> B;
	while(Contours.NValues)
	{
		while(Contours[0] != DWORD_MAX) P.Add(NodesVertices[Contours[0]]),
			B.Add(NodesVerticesBelongs[Contours[0]]), Contours.Del(0, 1);
		Contours.Del(0, 1); // DWORD_MAX
		xy_pair c;
		for(i = 0; i < P.NValues; i++) c += P[i];
		c.x /= double(P.NValues), c.y /= double(P.NValues);
		switch(Mode)
		{
		case Intersection:
			for(i = 0; i < B.NValues; i++) if(B[i] != 10) break;
			if(i == B.NValues)
				for(i = 0; i < P.NValues; i++)
					rSections.Add(CSection2D(P[i], P[(i + 1) % P.NValues], c_White));
			break;
		case ADifferenceB:
			if(MeshVertexClassify2D(rMeshA, c) == VERTEX_OUTSIDE)
				for(i = 0; i < P.NValues; i++)
					rSections.Add(CSection2D(P[i], P[(i + 1) % P.NValues], c_White));
			break;
		case BDifferenceA:
			if(MeshVertexClassify2D(rMeshB, c) == VERTEX_OUTSIDE)
				for(i = 0; i < P.NValues; i++)
					rSections.Add(CSection2D(P[i], P[(i + 1) % P.NValues, c_White]));
			break;
		default:
			break;
		}
		P.Clear();
		B.Clear();
	}*/
/*
	// TRIANGULATION
	CMesh2D MeshT;
	MeshT.Vertices.AddValues(NodesVertices.GetValues(), NodesVertices.NValues);
	NodesVertices.Clear();
	CIndices CurContour;
	while(Contours.NValues)
	{
		while(Contours[0] != DWORD_MAX) CurContour.Add(Contours[0]), Contours.Del(0, 1);
		Contours.Del(0, 1); // DWORD_MAX
		Booleans2D_Triangulate(CurContour, MeshT);
		CurContour.Clear();
	}
	Contours.Clear();
	// CREATING RESULTING MESH
	rMeshR.Vertices.AddValues(MeshT.Vertices.GetValues(), MeshT.Vertices.NValues);
	rMeshR.Indices.AddValues(MeshT.Indices.GetValues(), MeshT.Indices.NValues);
/*	int NTri = MeshT.Indices.NValues / 3;
	for(int nTri = 0; nTri < NTri; nTri++)
	{
		int i0 = MeshT.Indices[3 * nTri + 0], i1 = MeshT.Indices[3 * nTri + 1],
			i2 = MeshT.Indices[3 * nTri + 2];
		xy_pair c = MeshT.Vertices[i0] + MeshT.Vertices[i1] + MeshT.Vertices[i2];
		c.x /= 3, c.y /= 3;
		switch(Mode)
		{
		case Intersection:
			if((MeshVertexClassify2D(rMeshA, c) == VERTEX_INSIDE) &&
				(MeshVertexClassify2D(rMeshB, c) == VERTEX_INSIDE))
				rMeshR.Indices.Add(i0), rMeshR.Indices.Add(i1), rMeshR.Indices.Add(i2);
			break;
		case Union:
			rMeshR.Indices.Add(i0), rMeshR.Indices.Add(i1), rMeshR.Indices.Add(i2);
			break;
		case ADifferenceB:
			if(MeshVertexClassify2D(rMeshB, c) == VERTEX_OUTSIDE)
				rMeshR.Indices.Add(i0), rMeshR.Indices.Add(i1), rMeshR.Indices.Add(i2);
			break;
		case BDifferenceA:
			if(MeshVertexClassify2D(rMeshA, c) == VERTEX_OUTSIDE)
				rMeshR.Indices.Add(i0), rMeshR.Indices.Add(i1), rMeshR.Indices.Add(i2);
			break;
		default:
			break;
		}
	}
	MeshT.Release();*/
	// unInit...
/*
	for(int i)
	double r = 0.0f, s = 0.0f;
	xy_pair Cross;
	for(int i = 0; i < SectionsA.NValues; i++)
		for(int j = 0; j < SectionsB.NValues; j++)
			if(SectionsCross2D(SectionsA[i], SectionsB[j], Cross, r, s) ==
				SECTIONS_CROSS) Nodes.Add(Cross);
/*
	int NTriA = rMeshA.Indices.NValues / 3;
	CPolygons2D PolygonsA;
	for(int nTriA = 0; nTriA < NTriA; nTriA++)
	{
		CPolygon2D P;
		PolygonsA.Add(P);
		CPolygon2D &rP = PolygonsA.Values[PolygonsA.NValues - 1];
		for(int i = 0; i < 3; i++)
			rP.Add(rMeshA.Vertices[rMeshA.Indices[3 * nTriA + i]]);
	}
	// CONVERTING
	// rMeshB from CMesh2D to CPolygons2D PolygonsB
	int NTriB = rMeshB.Indices.NValues / 3;
	CPolygons2D PolygonsB;
	for(int nTriB = 0; nTriB < NTriB; nTriB++)
	{
		CPolygon2D P;
		PolygonsB.Add(P);
		CPolygon2D &rP = PolygonsB.Values[PolygonsB.NValues - 1];
		for(int i = 0; i < 3; i++)
			rP.Add(rMeshB.Vertices[rMeshB.Indices[3 * nTriB + i]]);
	}
	
	CPolygons2D Polygons;
	for(i = 0; i < PolygonsA.NValues; i++)
		for(int j = 0; j < PolygonsB.NValues; j++)
			Booleans2D(PolygonsA[i], PolygonsB[j], Polygons, Intersection, Nodes);
	for(i = 0; i < PolygonsA.NValues; i++)
		for(int j = 0; j < Polygons.NValues; j++)
			Booleans2D(PolygonsA[i], Polygons[j], rPolygons, ADifferenceB, Nodes);*/
/*	CPolygons2D Srorage;
	CPolygons2D *pS = &PolygonsA, *pD = &Srorage, *pClippers = &PolygonsB;
	if(Mode == BDifferenceA) pS = &PolygonsB, pClippers = &PolygonsA;
	for(int nClipper = 0; nClipper < pClippers->NValues; nClipper++)
	{
		for(int nPolygon = 0; nPolygon < pS->NValues; nPolygon++)
			Booleans2D(pS->Values[nPolygon], pClippers->Values[nClipper], *pD, Mode, Nodes);
		pS->Clear();
		swap(pS, pD);
	}

	for(int n = 0; n < pS->NValues; n++)
	{
		CPolygon2D P;
		rPolygons.Add(P);
		CPolygon2D &rP = rPolygons.Values[rPolygons.NValues - 1];
		rP.AddValues(pS->Values[n].GetValues(), pS->Values[n].NValues);
	}*/
} // Booleans2D

// ================================================================================
// CMesh2D2BaseMesh
// ================================================================================
bool CMesh2D2BaseMesh(const CMesh2D &rMesh2D, BaseMesh &rBaseMesh,
					 DWORD Color, char *strShader, char *strTexture,
					 xy_pair Origin, double z)
{
	Vertex2t *pMeshVertices = NULL;
	WORD *pMeshIndices = NULL;
	rBaseMesh.create(0xFFFF, 0xFFFF);
	pMeshVertices = (Vertex2t*)rBaseMesh.getVertexData();
	if(!pMeshVertices) return false;
	pMeshIndices = rBaseMesh.getIndices();
	if(!pMeshIndices) return false;

	for(int i = 0; i < rMesh2D.Vertices.NValues; i++)
	{
		pMeshVertices->x = rMesh2D.Vertices[i].x + Origin.x,
			pMeshVertices->y = rMesh2D.Vertices[i].y + Origin.y,
			pMeshVertices->z = z;
		pMeshVertices->diffuse = Color;
		pMeshVertices++;
	}
	for(i = 0; i < rMesh2D.Indices.NValues; i++)
	{
		*pMeshIndices = rMesh2D.Indices[i];
		pMeshIndices++;
	}
	pMeshVertices = NULL;
	pMeshIndices = NULL;
	rBaseMesh.setNInd(rMesh2D.Indices.NValues);
	rBaseMesh.setNVert(rMesh2D.Vertices.NValues);
	rBaseMesh.setNPri(rMesh2D.Indices.NValues / 3);
	rBaseMesh.setShader(IRS->GetShaderID(strShader));
	rBaseMesh.setTexture(IRS->GetTextureID(strTexture));
} // CMesh2D2BaseMesh

// ================================================================================
// Polygon2Mesh2D
// ================================================================================
void PolygonLShift2D(CPolygon2D &rPolygon)
{
	if(!rPolygon.NValues) return;
	xy_pair xy = rPolygon[0];
	rPolygon.Del(0, 1);
	rPolygon.Add(xy);
}
double Polygon2Mesh2D_sSectionVertex(const xy_pair &rSectionA,
				 const xy_pair &rSectionB, const xy_pair &rVertex)
{
	xy_pair a = rSectionB - rSectionA, b = rVertex - rSectionA;
	return a.y * b.x - a.x * b.y;
}
void Polygon2Mesh2D(const CPolygon2D &rPolygon, CMesh2D &rMesh)
{
	CPolygon2D Polygon;
	Polygon.AddValues(rPolygon.GetValues(), rPolygon.NValues);
	xy_pair A, B, C;
	int i = 0;
	if(Polygon.NValues == 3) // Polygon already is a triangle:
	{
		for(i = 0; i < 3; i++)
			rMesh.Indices.Add(rMesh.Vertices.NValues),
			rMesh.Vertices.Add(Polygon[i]);
		return;
	}
	if(PolygonConvexTest2D(Polygon)) // Polygon is convex:
	{
		for(i = 0; i < 3; i++)
			rMesh.Indices.Add(rMesh.Vertices.NValues),
			rMesh.Vertices.Add(Polygon[i]);
		Polygon.Del(1, 1);
		Polygon2Mesh2D(Polygon, rMesh);
		return;
	}
	// Polygon is nonconvex...
	while(SectionVertexClassify2D(CSection2D(Polygon[Polygon.NValues - 1],
		Polygon[0]), Polygon[1]) != VERTEX_RIGHT) PolygonLShift2D(Polygon);
	bool Ok = true;
	int k = 2; // k should be in [2; NValues - 2]
	while(true)
	{
		// vertex should be to the left:
		while(SectionVertexClassify2D(CSection2D(Polygon[0], Polygon[1]),
			Polygon[k]) != VERTEX_LEFT)
		{
			k++;
			if(k > Polygon.NValues - 2) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
				MessageBox(NULL, "Polygon is invalid!", "Polygon2Mesh2D", 0);
		}
		// secant should not cross any sections of polygon:
		xy_pair Cross = xy_pair::null;
		double r = 0, s = 0;
		Ok = true;
		for(i = 0; i < Polygon.NValues; i++)
		{
			CSection2D SectionA(Polygon[0], Polygon[k]),
				SectionB(Polygon[i], Polygon[(i + 1) % Polygon.NValues]);
/*			if(SectionsCross2D(SectionA, SectionB, Cross, r, s) == SECTIONS_CROSS)
				if((s != 0.0f) && (s != 1.0f) &&
					(SectionA.A != SectionB.A) && (SectionA.B != SectionB.B) &&
					(SectionA.A != SectionB.B) && (SectionA.B != SectionB.A))
					{
						Ok = false;
						break;
					}*/
		}
		if(Ok) break;
		k++;
		if(k > Polygon.NValues - 2) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
			MessageBox(NULL, "Polygon is invalid!", "Polygon2Mesh2D", 0);
	}
	// Splitting polygon with dagonal Polygon[0] - Polygon[k]:
	CPolygon2D PolygonA, PolygonB;
	for(i = 0; i <= k; i++) PolygonA.Add(Polygon[i]);
	for(i = k; i <= Polygon.NValues; i++) PolygonB.Add(Polygon[i % Polygon.NValues]);
	CPolygons2D Polygons;
	PolygonOptimize2D(PolygonA, Polygons);
	if(Polygons.NValues > 1) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		MessageBox(NULL, "Polygons.NValues > 1", "Polygon2Mesh2D", 0);
	Polygon2Mesh2D(Polygons[0], rMesh);
	Polygons.Clear();
	PolygonOptimize2D(PolygonB, Polygons);
	if(Polygons.NValues > 1) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		MessageBox(NULL, "Polygons.NValues > 1", "Polygon2Mesh2D", 0);
	Polygon2Mesh2D(Polygons[0], rMesh);
} // Polygon2Mesh2D

class CStar3D
{
public:
	float x, y, z;
	DWORD Color;
	void Set(float x, float y, float z, DWORD Color)
	{ this->x = x, this->y = y, this->z = z, this->Color = Color; }
	CStar3D() {}
	CStar3D(float x, float y, float z, DWORD Color) { Set(x, y, z, Color); }
};

typedef DynArray<CStar3D> CStars3D;

bool Sections2BaseMesh(const CSections3D &rSections, const CVertex3D &rOrigin, BaseMesh &rBaseMesh)
{
	Vertex2t *pVertices = NULL;
	WORD *pIndices = NULL;
	rBaseMesh.create(0xFFFF, 0xFFFF);
	pVertices = (Vertex2t*)rBaseMesh.getVertexData();
	if(!pVertices) return false;
	pIndices = rBaseMesh.getIndices();
	if(!pIndices) return false;
	DWORD Color = c_White;
	for(int i = 0; i < rSections.NValues; i++)
	{
		CSection3D &S = rSections[i];

		pVertices->x = S.A.x + rOrigin.x, pVertices->y = S.A.y + rOrigin.y, pVertices->z = S.A.z + rOrigin.z,
			pVertices->diffuse = Color, pVertices++; 
		pVertices->x = S.B.x + rOrigin.x, pVertices->y = S.B.y + rOrigin.y, pVertices->z = S.B.z + rOrigin.z,
			pVertices->diffuse = Color, pVertices++; 

		*pIndices = 2 * i + 0, pIndices++;
		*pIndices = 2 * i + 1, pIndices++;
		*pIndices = 2 * i + 0, pIndices++;
	}
	rBaseMesh.setNVert(2 * rSections.NValues);
	rBaseMesh.setNInd(3 * rSections.NValues);
	rBaseMesh.setNPri(rSections.NValues);
	rBaseMesh.setShader(IRS->GetShaderID("WireS"));
	rBaseMesh.setTexture(IRS->GetTextureID("white.bmp"));
}
bool Vertices2BaseMesh(const CVertices3D &rVertices, const CVertex3D &rOrigin, BaseMesh &rBaseMesh)
{
	Vertex2t *pVertices = NULL;
	WORD *pIndices = NULL;
	rBaseMesh.create(0xFFFF, 0xFFFF);
	pVertices = (Vertex2t*)rBaseMesh.getVertexData();
	if(!pVertices) return false;
	pIndices = rBaseMesh.getIndices();
	if(!pIndices) return false;
	const float d = 3.0f;
	DWORD Color = c_Yellow;
	for(int i = 0; i < rVertices.NValues; i++)
	{
		CVertex3D s = rVertices[i];
		s.x += rOrigin.x, s.y += rOrigin.y, s.z += rOrigin.z;

		pVertices->x = s.x - d, pVertices->y = s.y - d, pVertices->z = s.z - d,
			pVertices->diffuse = Color, pVertices++; 
		pVertices->x = s.x + d, pVertices->y = s.y + d, pVertices->z = s.z + d,
			pVertices->diffuse = Color, pVertices++;
		pVertices->x = s.x + d, pVertices->y = s.y - d, pVertices->z = s.z - d,
			pVertices->diffuse = Color, pVertices++;
		pVertices->x = s.x - d, pVertices->y = s.y + d, pVertices->z = s.z + d,
			pVertices->diffuse = Color, pVertices++;

		pVertices->x = s.x - d, pVertices->y = s.y - d, pVertices->z = s.z + d,
			pVertices->diffuse = Color, pVertices++;
		pVertices->x = s.x + d, pVertices->y = s.y + d, pVertices->z = s.z - d,
			pVertices->diffuse = Color, pVertices++;
		pVertices->x = s.x + d, pVertices->y = s.y - d, pVertices->z = s.z + d,
			pVertices->diffuse = Color, pVertices++;
		pVertices->x = s.x - d, pVertices->y = s.y + d, pVertices->z = s.z - d,
			pVertices->diffuse = Color, pVertices++;

		*pIndices = 8 * i + 0, pIndices++;
		*pIndices = 8 * i + 1, pIndices++;
		*pIndices = 8 * i + 0, pIndices++;
		*pIndices = 8 * i + 2, pIndices++;
		*pIndices = 8 * i + 3, pIndices++;
		*pIndices = 8 * i + 2, pIndices++;

		*pIndices = 8 * i + 4, pIndices++;
		*pIndices = 8 * i + 5, pIndices++;
		*pIndices = 8 * i + 4, pIndices++;
		*pIndices = 8 * i + 6, pIndices++;
		*pIndices = 8 * i + 7, pIndices++;
		*pIndices = 8 * i + 6, pIndices++;
	}
	rBaseMesh.setNVert(8 * rVertices.NValues);
	rBaseMesh.setNInd(12 * rVertices.NValues);
	rBaseMesh.setNPri(4 * rVertices.NValues);
	rBaseMesh.setShader(IRS->GetShaderID("WireS"));
	rBaseMesh.setTexture(IRS->GetTextureID("white.bmp"));
}

void _shiftvertex(CVertex3D &rVertex)
{
	// if even inc, odd - dec
	if(rand() & 1) rVertex.x += 1; else rVertex.x -= 1;
	if(rand() & 1) rVertex.x += 1; else rVertex.x -= 1;
	if(rand() & 1) rVertex.x += 1; else rVertex.x -= 1;
}
/*
eBooleans _classifyvertex(const CMesh3D &rMesh, CVertex3D &rVertex, bool bUseBBox, int N = 0)
{
	assert(N < 100);
	const CVertex3D BBox0 = rMesh.m_BBox0, BBox1 = rMesh.m_BBox1;
	if(bUseBBox && (rVertex.x < BBox0.x || rVertex.x > BBox1.x || rVertex.y < BBox0.y || rVertex.y > BBox1.y ||
		rVertex.z < BBox0.z || rVertex.z > BBox1.z)) return booleansOutSide;

	// Generating ray direction:
	double Phi = c_DoublePI * double(rand()) / double(RAND_MAX),
		Theta = c_PI / 4. * double(rand()) / double(RAND_MAX);
	CVector Dir(sin(Phi) * sin(Theta), cos(Theta), cos(Phi) * sin(Theta));

	// Testing for intersections:
	int NTris = rMesh.Indices.NValues / 3;
	int cIntersections = 0;
	for(int nTri = 0; nTri < NTris; nTri++)
	{
		int i0 = rMesh.Indices[3 * nTri + 0], i1 = rMesh.Indices[3 * nTri + 1], i2 = rMesh.Indices[3 * nTri + 2];
		const CVertex3D &v0 = rMesh.Vertices[i0], &v1 = rMesh.Vertices[i1], &v2 = rMesh.Vertices[i2];
		CVector T0(v0.x, v0.y, v0.z), T1(v1.x, v1.y, v1.z), T2(v2.x, v2.y, v2.z),
			C(rVertex.x, rVertex.y, rVertex.z);
		eBooleans r = ClassifyRayTri3D(C, Dir, T0, T1, T2, kEpsilon);
		if(r == BooleansOriginBelongs)
		{
			_shiftvertex(rVertex);
			return _classifyvertex(rMesh, rVertex, bUseBBox, N + 1);
		}
		if(r == booleansBelongs) return _classifyvertex(rMesh, rVertex, bUseBBox, N + 1);
		if(r == BooleansIntersects) cIntersections++;
	}
	return cIntersections & 1 ? booleansInSide : booleansOutSide;
	// BooleansOut, BooleansIn
}
*/
void BooleansProcess(const CMesh3D &rMeshA, const CMesh3D &rMeshB, CMesh3D &rMeshR, CVertices3D &rCrosses, CMesh3D &rMeshT, CSections3D &rSections)
{
/*	CMesh3D A = rMeshA, B = rMeshB;
	A.CalcBBox();
	B.CalcBBox();

	DynArray<eBooleans> rA, rB;
	eBooleans r;

	for(int i = 0; i < A.Vertices.NValues; i++)
	{
		r = _classifyvertex(B, A.Vertices[i], true);
		rA.Add(r);
	}
	for(i = 0; i < B.Vertices.NValues; i++)
	{
		r = _classifyvertex(A, B.Vertices[i], true);
		rB.Add(r);
	}
	CIndices ATris, BTris;
	// Classifying triangles:
	int NTrisA = A.Indices.NValues / 3;
	for(int nTri = 0; nTri < NTrisA; nTri++)
	{
		int i0 = A.Indices[3 * nTri + 0], i1 = A.Indices[3 * nTri + 1], i2 = A.Indices[3 * nTri + 2];
		CVertex3D &v0 = A.Vertices[i0], &v1 = A.Vertices[i1], &v2 = A.Vertices[i2];
		eBooleans r0 = rA[i0], r1 = rA[i1], r2 = rA[i2];
		assert(r0 != booleansBelongs && r1 != booleansBelongs && r2 != booleansBelongs);
		if(r0 == r1 && r1 == r2 && r2 == booleansInSide)
			rMeshR.AddTriangle(A.Vertices[i0], A.Vertices[i1], A.Vertices[i2]);
		if(r0 != r1 || r1 != r2 || r0 != r2) ATris.Add(i0), ATris.Add(i1), ATris.Add(i2);
	}
	int NTrisB = B.Indices.NValues / 3;
	for(int nTri = 0; nTri < NTrisB; nTri++)
	{
		int i0 = B.Indices[3 * nTri + 0], i1 = B.Indices[3 * nTri + 1], i2 = B.Indices[3 * nTri + 2];
		CVertex3D &v0 = B.Vertices[i0], &v1 = B.Vertices[i1], &v2 = B.Vertices[i2];
		int r0 = rB[i0], r1 = rB[i1], r2 = rB[i2];
		assert(r0 != booleansBelongs && r1 != booleansBelongs && r2 != booleansBelongs);
		if(r0 == r1 && r1 == r2 && r2 == booleansInSide)
			rMeshR.AddTriangle(B.Vertices[i0], B.Vertices[i1], B.Vertices[i2]);
		if(r0 != r1 || r1 != r2 || r0 != r2) BTris.Add(i0), BTris.Add(i1), BTris.Add(i2);
	}*/
}

#include "Comms\comms.h"

#include "Surface.h"
bool testBooleans3D() {
	VertexBasedMesh vbm;
	vbm.GenMarker(OneMapVertex::Origin, 10, false);
	VertexBasedMeshRef Ref;
	vbm.GetRefAll(Ref);
	Ref.m_TriNums.Del(0, 2);
	Ref.Relax();


	cBitMap b(10);
	b.SetAll();
	b = cBitMap(100);


	cBitMap bm(100);
	bm.Set(3);
	bm.Set(10);
	bm.Set(12);
	bm.Clear(10);
	assert(bm[3]);
	assert(bm[12]);
	assert(!bm[10]);



	ints l0, l1;
	for(int i = 1; i <= 10; i++) l0.Add(i);
	for(i = 100; i <= 106; i++) l1.Add(i);

	int nUsed = l0.Count();
	assert(l0.InsertAt(nUsed, -1) == nUsed);
	assert(l0.InsertAt(2, l1) == 2 + l1.Count() - 1);
	assert(l0.InsertAt(l0.Count(), l1) == l0.Count() + l1.Count() - 1);

	CMesh3D A, B;
	A.generatorSphere(CVertex3D(200, 1000, 200), 200, 10, 10, CVertex3D(2.0f, 1.0f, 1.0f));
	B.generatorSphere(CVertex3D(200, 900, 0), 200, 10, 10, CVertex3D(1.0f, 1.0f, 3.0f));

	CVertices3D Crosses;

	CMesh3D R, T;
	CSections3D Sections;
	//CMesh3D::Difference2(A, B, R, Crosses, T, Sections);
	BooleansProcess(A, B, R, Crosses, T, Sections);


	BaseMesh bmSections;
	BaseMesh bmCrosses;
	Vertices2BaseMesh(Crosses, CVertex3D::Null, bmCrosses);
//	Vertices2BaseMesh(Crosses, CVertex3D(500, 0, 0), bmCrosses);
	Crosses.Clear();

	BaseMesh bmA, bmB, bmR, bmT, bmS;
	A.toBaseMesh(bmA, "WireS", "white.bmp", c_Red - 0x80000000, CVertex3D::Null);
	B.toBaseMesh(bmB, "WireS", "white.bmp", c_Green - 0x80000000, CVertex3D::Null);
	R.toBaseMesh(bmR, "WireS", "white.bmp", c_Yellow, CVertex3D(500, 0, 0));
	T.toBaseMesh(bmT, "WireS", "white.bmp", c_White, CVertex3D(500, 0, 0));
	Sections2BaseMesh(Sections, CVertex3D::Null,/*CVertex3D(500, 0, 0),*/ bmS);
	A.Clear(), B.Clear(), R.Clear(), T.Clear(), Sections.Clear();

	while(true)
	{
		DrawBM(bmA);
		DrawBM(bmB);
		DrawBM(bmR);
		DrawBM(bmT);
		DrawBM(bmCrosses);
		DrawBM(bmS);
		DrawBM(bmSections);
		extern void SetupCamera();
		SetupCamera();
		FlipPages();
		ProcessMessages();
		IRS->ClearDevice( 0, true, true );
	}
}

bool probeBooleans(void)
{

	CMesh2D ShapeB;
	ShapeB.AddVertices(xy_pair(30, 10), xy_pair(10, 50), xy_pair(40, 50),
		xy_pair(30, 40), xy_pair(50, 30), xy_pair(70, 50), xy_pair(30, 70),
		xy_pair(20, 90), xy_pair(60, 100), xy_pair(40, 80), xy_pair(70, 80),
		xy_pair(90, 50), xy_pair(70, 20), xy_pair(90, 10), xy_pair::xy_max);
	ShapeB.AddIndices(0, 1, 3, 3, 1, 2, 0, 12, 13, 0, 3, 12, 12, 4, 11,
		4, 5, 11, 5, 6, 7, 5, 7, 9, 5, 9, 11, 11, 9, 10, 9, 7, 8, DWORD_MAX);
//	BShapeMesh.AddVertices(xy_pair(30, 10), xy_pair(10, 50), xy_pair(40, 50),
//		xy_pair(30, 40), xy_pair::xy_max);
//	BShapeMesh.AddIndices(0, 1, 3,	3, 1, 2,	DWORD_MAX);
	// CIRCLE
	CMesh2D ShapeA;
	ShapeA.AddVertices(xy_pair(0, 0), xy_pair(-43, -25), xy_pair(-50, 0),
		xy_pair(-43, 25), xy_pair(-25, 43), xy_pair(0, 50), xy_pair(25, 43),
		xy_pair(43, 25), xy_pair::xy_max);
	ShapeA.AddIndices(0, 1, 2,	0, 2, 3,	0, 3, 4,	0, 4, 5,
		0, 5, 6,	0, 6, 7,	DWORD_MAX);
/*	AShapeMesh.AddVertices(xy_pair(0, 0), xy_pair(0, -50), xy_pair(-25, -43),
		xy_pair(-43, -25), xy_pair(-50, 0), xy_pair(-43, 25), xy_pair(-25, 43),
		xy_pair(0, 50), xy_pair(25, 43), xy_pair(43, 25), xy_pair(50, 0),
		xy_pair(43, -25), xy_pair(25, -43), xy_pair::xy_max);
	AShapeMesh.AddIndices(0, 1, 2,	0, 2, 3,	0, 3, 4,	0, 4, 5,	0, 5, 6,
		0, 6, 7,	0, 7, 8,	0, 8, 9,	0, 9, 10,	0, 10, 11,
		0, 11, 12,	0, 12, 1,	DWORD_MAX);*/

//	CMesh2D ShapeB;
//	ShapeB.AddVertices(xy_pair(0, -50), xy_pair(-25, 0), xy_pair(0, 50),
//		xy_pair(25, 0), xy_pair::xy_max);
//	ShapeB.AddIndices(0, 1, 2,	0, 2, 3,	DWORD_MAX);

	// Position and scale factors:
	xy_pair PosA(419, 117), PosB(120, 180);
	double XScaleA = 5, YScaleA = 5, XScaleB = 5, YScaleB = 5;
	EBooleans Mode = Intersection;
	const double LXAxis = 500;
	xy_pair SecondOrigin(512, 0);
	do
	{
		// BOOLEANS MODE CONTROL
		for(int i = 0; i <= NOP; i++)
			if(GetKeyState('1' + i) & 0x8000)
			{
				Mode = (EBooleans)i;
				break;
			}
		ShowString(10, 10, strBooleans[Mode], &WhiteFont);
		// MESHES POSITION AND SCALE FACTORS CONTROL
		bool Left = GetKeyState(VK_LEFT) & 0x8000, Right = GetKeyState(VK_RIGHT) & 0x8000,
			Down = GetKeyState(VK_DOWN) & 0x8000, Up = GetKeyState(VK_UP) & 0x8000,
			Ctrl = GetKeyState(VK_CONTROL) & 0x8000,
			Shift = GetKeyState(VK_SHIFT) & 0x8000;
		if(!Ctrl)
		{
			const double fSpeed = 1.0f;
			if(!Shift)
			{
				if(Left) PosA.x -= fSpeed;
				if(Right) PosA.x += fSpeed;
				if(Down) PosA.y += fSpeed;
				if(Up) PosA.y -= fSpeed;
			}
			else
			{
				if(Left) PosB.x -= fSpeed;
				if(Right) PosB.x += fSpeed;
				if(Down) PosB.y += fSpeed;
				if(Up) PosB.y -= fSpeed;
			}
		}
		else
		{
			const double MinS = 0.3f, MaxS = 12.0f, ScalingSpeed = 0.1f;
			if(!Shift)
			{
				if(Left) if(XScaleA - ScalingSpeed >= MinS) XScaleA -= ScalingSpeed;
				if(Right) if(XScaleA + ScalingSpeed <= MaxS) XScaleA += ScalingSpeed;
				if(Up) if(YScaleA - ScalingSpeed >= MinS) YScaleA -= ScalingSpeed;
				if(Down) if(YScaleA + ScalingSpeed <= MaxS) YScaleA += ScalingSpeed;
			}
			else
			{
				if(Left) if(XScaleB - ScalingSpeed >= MinS) XScaleB -= ScalingSpeed;
				if(Right) if(XScaleB + ScalingSpeed <= MaxS) XScaleB += ScalingSpeed;
				if(Up) if(YScaleB - ScalingSpeed >= MinS) YScaleB -= ScalingSpeed;
				if(Down) if(YScaleB + ScalingSpeed <= MaxS) YScaleB += ScalingSpeed;
			}
		}
		// AXES
		CSections2D toRender;
		toRender.Add(CSection2D(xy_pair(0), xy_pair(LXAxis, 0), c_Red));
		toRender.Add(CSection2D(xy_pair(0), xy_pair(0, 2000), c_Green));
		toRender.Add(CSection2D(xy_pair(SecondOrigin.x, 0),
			xy_pair(SecondOrigin.x + LXAxis, 0), c_Red));
		toRender.Add(CSection2D(xy_pair(SecondOrigin.x, 0),
			xy_pair(SecondOrigin.x, 2000), c_Green));
		// CREATING MESH A
		CMesh2D MeshA;
		for(int i = 0; i < ShapeA.Vertices.NValues; i++)
			MeshA.Vertices.Add(xy_pair(XScaleA * ShapeA.Vertices[i].x + PosA.x,
			YScaleA * ShapeA.Vertices[i].y + PosA.y));
		for(i = 0; i < ShapeA.Indices.NValues; i++)
			MeshA.Indices.Add(ShapeA.Indices[i]);
		// CREATING MESH B
		CMesh2D MeshB;
		for(i = 0; i < ShapeB.Vertices.NValues; i++)
			MeshB.Vertices.Add(xy_pair(XScaleB * ShapeB.Vertices[i].x + PosB.x,
			YScaleB * ShapeB.Vertices[i].y + PosB.y));
		for(i = 0; i < ShapeB.Indices.NValues; i++)
			MeshB.Indices.Add(ShapeB.Indices[i]);
		// PROCESSING
		CMesh2D MeshR;
		CSections2D Sections;
		Booleans2D(MeshA, MeshB, MeshR, Mode, Sections);
		for(i = 0; i < Sections.NValues; i++)
		{
			CSection2D &rS = Sections[i];
			rS.A += SecondOrigin;
			rS.B += SecondOrigin;
		}
		toRender.AddValues(Sections.GetValues(), Sections.NValues);
		// TRIANGULATION
//		CMesh2D RMesh;
//		for(i = 0; i < Polygons.NValues; i++) Polygon2Mesh2D(Polygons[i], RMesh);
		// STATS
		char Buffer[4096];
		sprintf(Buffer, "NTriangles = %d", MeshR.Indices.NValues / 3);
		ShowString(512, 30, Buffer, &WhiteFont);

		// CONVERTING SECTIONS
		// from toRender to Mesh
		BaseMesh Mesh;
		Vertex2t *pMeshVertices = NULL;
		WORD *pMeshIndices = NULL;
		Mesh.create(0xFFFF, 0xFFFF);
		pMeshVertices = (Vertex2t*)Mesh.getVertexData();
		if(!pMeshVertices) return false;
		pMeshIndices = Mesh.getIndices();
		if(!pMeshIndices) return false;
		for(int n = 0; n < toRender.NValues; n++)
		{
			// Adding vertices of section to pMeshVertices
			CSection2D &rSection = toRender[n];
			pMeshVertices->x = rSection.A.x,
				pMeshVertices->y = rSection.A.y,
				pMeshVertices->z = 0;
			pMeshVertices->diffuse = rSection.Color;
			pMeshVertices++;
			pMeshVertices->x = rSection.B.x,
				pMeshVertices->y = rSection.B.y,
				pMeshVertices->z = 0;
			pMeshVertices->diffuse = rSection.Color;
			pMeshVertices++;
			// Adding indices of section' vertices to pMeshIndices
			*pMeshIndices = 2 * n;
			pMeshIndices++;
			*pMeshIndices = 2 * n + 1;
			pMeshIndices++;
			*pMeshIndices = 2 * n;
			pMeshIndices++;
		}
	
		pMeshVertices = NULL;
		pMeshVertices = NULL;
		Mesh.setNInd(3 * toRender.NValues);
		Mesh.setNVert(2 * toRender.NValues);
		Mesh.setNPri(toRender.NValues);
		Mesh.setShader(IRS->GetShaderID("WireS"));
		Mesh.setTexture(IRS->GetTextureID("white.bmp"));
		DrawBM(Mesh);

		BaseMesh BaseMeshA;
		CMesh2D2BaseMesh(MeshA, BaseMeshA, c_Red & 0xF0FFFFFF, "WireS",//"terra_shadowed",
			"Red.bmp", xy_pair::null, 0);
		MeshA.Release();
		DrawBM(BaseMeshA);
		BaseMesh BaseMeshB;
		CMesh2D2BaseMesh(MeshB, BaseMeshB, c_Green & 0xF0FFFFFF, "WireS",//"terra_shadowed",
			"Green.bmp", xy_pair::null, 1);
		MeshB.Release();
		DrawBM(BaseMeshB);
		BaseMesh BaseMeshR;
		CMesh2D2BaseMesh(MeshR, BaseMeshR, c_Blue, "WireS",//"terra_shadowed",
			"Blue.bmp", SecondOrigin, 0);
		MeshR.Release();
		DrawBM(BaseMeshR);
		// -------------------------------------------------
		FlipPages();
		ProcessMessages();
		IRS->ClearDevice( 0, true, true );
	}while(true);
	return true;
}