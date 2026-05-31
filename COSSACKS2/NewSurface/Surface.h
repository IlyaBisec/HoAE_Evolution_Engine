#ifndef __SURFACE_H__
#define __SURFACE_H__

//#define MAYA_BOOLEANS // COMMENT (DELETE) FOR RELEASE THIS!

#define SPLITMESH_MAXNFACTURES	128

#include "SurfaceShader.h"
#include "comms\comms.h"
#include "..\MCubes.h"

typedef enum { TFWire, TFSolid } eTerraFill;

bool VertexBasedMesh2cRawMesh(const VertexBasedMesh &, cRawMesh &);
bool cRawMesh2VertexBasedMesh(const cRawMesh &, VertexBasedMesh &);

// argb2rgb
inline DWORD argb2rgb(DWORD argb) {
	int b = argb & 0xFF;
	argb >>= 8;
	int g = argb & 0xFF;
	argb >>= 8;
	int r = argb & 0xFF;
	return RGB(r, g, b);
} // argb2rgb

// rgb2argb
inline void rgb2argb(DWORD rgb, DWORD &argb) {
	argb = (0xFF << 24) + (GetRValue(rgb) << 16) + (GetGValue(rgb) << 8) + GetBValue(rgb);
} // rgb2argb

// argb2rgb(DWORD, int &, int &, int &)
inline void argb2rgb(DWORD argb, int &r, int &g, int &b) {
	b = argb & 0xFF;
	argb >>= 8;
	g = argb & 0xFF;
	argb >>= 8;
	r = argb & 0xFF;
} // argb2rgb(DWORD, int &, int &, int &)

// rgb2argb(int, int, int, DWORD &)
inline void rgb2argb(int r, int g, int b, DWORD &argb) {
	argb = (0xFF << 24) + (r << 16) + (g << 8) + b;
} // rgb2argb(int, int, int, DWORD &)


struct uv_pair {
	float u;
	float v;
};

class nHashTableEntry
{
public:
	DWORD i; // i - unique index
	unsigned int hash() const
	{ return i; }
	bool equal(const nHashTableEntry &rn)
	{
		if(i == rn.i) return true;
		return false;
	}
	void copy(const nHashTableEntry &rn)
	{ i = rn.i; }
};

class TerraMapper;

struct OneMapVertex {
	OneMapVertex();
	OneMapVertex(short, short, short = 0);
	OneMapVertex(short, short, short, DWORD C);
	OneMapVertex(short, short, short, DWORD C, byte S);
	OneMapVertex(short, short, short, DWORD C, byte S, byte F);

	OneMapVertex & Set(short, short, short);
	OneMapVertex & Set(short, short, short, DWORD);
	OneMapVertex & Set(short, short, short, DWORD C, byte S);

	bool Equals(const OneMapVertex &) const;
	bool operator == (const OneMapVertex &) const;
	int CalcHashCode() const;

	DWORD CalcDiffuseColor() const;
	DWORD CalcSpecularColor() const;

	short x;
	short y;
	short z;
	DWORD Color;
	byte  nFacture;
	byte  Shadow;
	char  nx;
	char  ny;
	char  nz;
	byte  SurfaceType;

//	static BaseMapper *pMappers[MapperCount];
	//static BaseMapper* GetMapper(int nFacture);
	static TerraMapper GetMapper(int nFacture);
	static int GetFactureID(int nFacture);
	static int GetFactureID2(int nFacture);
	static int GetShader(int nFacture);
	static const OneMapVertex Origin;
	static const char NormalLen = 127;
};

// OneMapVertex()
inline OneMapVertex::OneMapVertex()
: x(0), y(0), z(0), nFacture(0), Color(0xFFFFFFFF) {}

// OneMapVertex(short, short, short)
inline OneMapVertex::OneMapVertex(short a, short b, short c)
: x(a), y(b), z(c), nFacture(0), Color(0xFFFFFFFF) {}

// OneMapVertex(short, short, short, DWORD)
inline OneMapVertex::OneMapVertex(short a, short b, short c, DWORD C)
: x(a), y(b), z(c), nFacture(0), Color(C) {}

// OneMapVertex(short, short, short, DWORD, byte)
inline OneMapVertex::OneMapVertex(short a, short b, short c, DWORD C, byte S)
: x(a), y(b), z(c), nFacture(0), Color(C), Shadow(S) {}

// OneMapVertex(short, short, short, DWORD, byte, byte)
inline OneMapVertex::OneMapVertex(short a, short b, short c, DWORD C, byte S, byte F)
: x(a), y(b), z(c), nFacture(F), Color(C), Shadow(S) {}

// OneMapVertex::Set(short, short, short)
inline OneMapVertex & OneMapVertex::Set(short a, short b, short c) {
	x = a, y = b, z = c; return *this;
} // OneMapVertex::Set(short, short, short)

// OneMapVertex::Set(short, short, short, DWORD)
inline OneMapVertex & OneMapVertex::Set(short a, short b, short c, DWORD C) {
	x = a, y = b, z = c;
	Color = C;
	return *this;
} // OneMapVertex::Set(short, short, short, DWORD)

// OneMapVertex::Set(short, short, short, DWORD, byte)
inline OneMapVertex & OneMapVertex::Set(short a, short b, short c, DWORD C, byte S) {
	x = a, y = b, z = c;
	Color = C, Shadow = S;
	return *this;
} // OneMapVertex::Set(short, short, short, DWORD, byte)

// OneMapVertex::Equals
inline bool OneMapVertex::Equals(const OneMapVertex &v) const {
	return v.x == x && v.y == y && v.z == z;
}

// OneMapVertex::operator ==
inline bool OneMapVertex::operator == (const OneMapVertex &v) const {
	return Equals(v);
}

// OneMapVertex::CalcHashCode
inline int OneMapVertex::CalcHashCode() const {
	int h = 1;
	h = 37 * h + x;
	h = 37 * h + y;
	h = 37 * h + z;
	return h;
}

// OneMapVerts
class OneMapVerts : public cList<OneMapVertex> {
public:
	void BBox(OneMapVertex &bbMin, OneMapVertex &bbMax) const;
}; // OneMapVerts

// InRound
inline bool InRound(int cx, int cy, int r, const OneMapVertex &v) {
	return Square(int(v.x) - cx) + Square(int(v.y) - cy) < Square(r);
}

// InRound2
inline bool InRound2(int cx, int cy, int r2, const OneMapVertex &v) {
	return Square(int(v.x) - cx) + Square(int(v.y) - cy) < r2;
}

// InSphere
inline bool InSphere(const OneMapVertex &c, int r, const OneMapVertex &v) {
	int dx = v.x - c.x, dy = v.y - c.y, dz = v.z - c.z;
	return Square(dx) + Square(dy) + Square(dz) < Square(r);
} // InSphere

// InSphere2
inline bool InSphere2(const OneMapVertex &c, int r2, const OneMapVertex &v) {
	int dx = v.x - c.x, dy = v.y - c.y, dz = v.z - c.z;
	return Square(dx) + Square(dy) + Square(dz) < r2;
} // InSphere2

// InRect
inline bool InRect(int x0, int y0, int w, int h, const OneMapVertex &v) {
	// *********     * - included
	// *       .     . - excluded
	// *       .
	// *........
	int x = int(v.x), y = int(v.y);
	return x >= x0 && x < x0 + w && y >= y0 && y < y0 + h;
}

class TerraMapper {
public:
	TerraMapper() {}
	TerraMapper(int nFacture);
	static DWORD CalcMappingCode(const OneMapVertex &);
	uv_pair CalcMappingUV(const OneMapVertex &, DWORD) const;

	float uScale, vScale;
	int fWidth, fHeight;

	static const int k = 2; 
	// | --- 1 --- |
	// *************---
	// *           * |
	// *           * |
	// *           * k
	// *           * |
	// *           * |
	// *************---

	enum MappingCodes {
		MC_BACK = 0,
		MC_LEFT = 1,
		MC_FRONT = 2,
		MC_RIGHT = 3,
		MC_TOP = 4,
		MC_BOTTOM = 5
	};
};

bool UsesBump(int id);
int GetFactureBump(int id, float &Contrast, float &Bright);
int GetFacture(int);

inline int OneMapVertex::GetFactureID(int nFacture) {	
	return GetFacture(nFacture);
}

inline int OneMapVertex::GetFactureID2(int nFacture) {
	float x;
	if(UsesBump(nFacture)) return GetFactureBump(nFacture, x, x);
	return GetFacture(nFacture);
}
extern eTerraFill g_TerraFill;
inline int OneMapVertex::GetShader(int nFacture) {
	static int Wire = IRS->GetShaderID("terra_wire");
	static int Solid = IRS->GetShaderID("terra_shadowed");
	static int shb = IRS->GetShaderID("dot3specular");
	return UsesBump(nFacture) ? shb : (g_TerraFill == TFWire ? Wire : Solid);
}

typedef DynArray<OneMapVertex> COneMapVertices;

class BaseSurfaceFunction{
public:
	virtual void ModifyVertex(OneMapVertex& V) = 0;
	virtual bool ShiftVertex(OneMapVertex& V) = 0;
};

class VertexBasedMesh;
bool zXTri(float *Z, const Vector3D &v3dTriA, const Vector3D &v3dTriB,
		   const Vector3D &v3dTriC);

class OneMapVertexHash
{
public:
	DWORD oi, i;	// oi - index in origin, i - new index
	unsigned int hash() const
	{ return oi; }
	bool equal(const OneMapVertexHash &vtxh)
	{ if(vtxh.oi == oi) return true; return false; }
	void copy(const OneMapVertexHash &vtxh)
	{ oi = vtxh.oi, i = vtxh.i; }
};

typedef DynArray<DWORD> _DWORDS;

class VertexBasedMeshRef{
public:
	~VertexBasedMeshRef();
	void Free();

	VertexBasedMesh *pOrigin;
	_DWORDS m_TriNums;

	void Copy(const VertexBasedMeshRef &Src);
	VertexBasedMeshRef & operator = (const VertexBasedMeshRef &Src);
	void AddRef(const VertexBasedMeshRef &Ref);
	void AddRef(int nTri);
	void GetRefCylinder(int cx, int cy, int r, VertexBasedMeshRef &Ref) const;
	void ConvertTo(VertexBasedMesh &Mesh) const;
	void PackTo(VertexBasedMesh &Mesh) const;
	void CalcTriCenters(cVectors &TriCenters) const;
	void CalcTriNormals(cVectors &TriNormals) const;
	void CalcNormals(int cx, int cy, int r);
	void FillMapOfHeights() const;
	void ExtractUniqueVerts(DWORDS &VertNums, cHashIndex &reIndexingTable) const;
	void reIndex(DWORDS &reInds, const DWORDS &VertNums, const cHashIndex &reIndexingTable) const;
	void CalcBoundBox(OneMapVertex &AABBMin, OneMapVertex &AABBMax) const;
	void CalcBoundSphere(OneMapVertex &Centre, int &Radius) const;
	void SmoothColors();
	int CalcMaxZ(int cx, int cy) const;
	void SupplementShadow(const SurfaceShader &Shader);

	struct RelaxArgs {
		RelaxArgs() // Defaults:
			: Amount(0.5), Iterations(2), HoldBoundaryPoints(true) {}
		RelaxArgs(double Amount /* [-1.0, 1.0] */, int Iterations /* [1, oo) */, bool HoldBoundaryPoints) {
			this->Amount = Amount;
			this->Iterations = Iterations;
			this->HoldBoundaryPoints = HoldBoundaryPoints;
		}
		double Amount;
		int Iterations;
		bool HoldBoundaryPoints;
	};
	void Relax(const RelaxArgs & Args = RelaxArgs());

	static int CalcZ(const OneMapVertex &, const OneMapVertex &, const OneMapVertex &, int, int);
	static void Tri2THMap(const OneMapVertex &, const OneMapVertex &, const OneMapVertex &);
};


typedef DynArray<DWORD> CIndices;

class MeshesCache;

class VertexBasedMesh{	
public:
	DynArray<OneMapVertex> Vert;
	DynArray<DWORD> Idxs;
	OneMapVertex m_BBox0, m_BBox1;
	VertexBasedMesh() {}
	VertexBasedMesh(const VertexBasedMesh &rMesh)
	{
		Free();
		Vert.AddValues(rMesh.Vert.GetValues(), rMesh.Vert.NValues);
		Idxs.AddValues(rMesh.Idxs.GetValues(), rMesh.Idxs.NValues);
	}
	~VertexBasedMesh();
	void Free();

	//helpers
	virtual void GetMeshRefSphere(Vector3D v3dCenter, float Radius,
		VertexBasedMeshRef *pVBMeshRef);
	virtual void GetMeshRefBox(int x, int y, int w, int l, VertexBasedMeshRef *pVBMeshRef);
	virtual void GetMeshRefCylinder(int cx, int cy, int r, VertexBasedMeshRef &rRef);
	void GetRefAll(VertexBasedMeshRef &Ref);

	virtual int  AddVertex(OneMapVertex& V){
		return Vert.Add(V);
	};
	virtual int  AddIndex(int idx){
		return Idxs.Add(idx);
	};

	// Generators
	void GenHexagonalPlane(int Lx, int Ly, int D);
	void CreateBezierHexagonalPlane(Vector3D v3dOrigin, int Sx, int Sy,
		int Dx, int Dy, DWORD dwColor, byte nFacture, byte Shadow,
		int z00,	int z01,	int z02,	int z03,
		int z10,	int z11,	int z12,	int z13,
		int z20,	int z21,	int z22,	int z23,
		int z30,	int z31,	int z32,	int z33);

	void CalcNormals(void);
	void Shade(const SurfaceShader &Shader, eSSIllumination Illumination);

	void ApplyMCubes(int CellSize,int Nx,int Ny,BaseSurfaceFunction* Func);

	bool fromModelByName(const char *strModelName, const Matrix4D &m4dWorld);
	void AddTri(DWORD i0, DWORD i1, DWORD i2);
	void AddTri(const OneMapVertex &v0, const OneMapVertex &v1, const OneMapVertex &v2);
	void AddMarker(const OneMapVertex &l0, const OneMapVertex &l1, int d, DWORD argbColor);
	void AddLine(const OneMapVertex &l0, const OneMapVertex &l1, DWORD argbColor);
	bool CrossSectionMesh(const OneMapVertex &rVertexA, const OneMapVertex &rVertexB);
	inline void GetTriangle(DWORD nTri, OneMapVertex &v0, OneMapVertex &v1, OneMapVertex &v2) const;
	void GetTriangleCentre(DWORD nTri, OneMapVertex &c) const;
	bool CalcZ(const OneMapVertex &v, short &rZ) const;
	void CalcBoundRect(int &x0, int &y0, int &x1, int &y1) const;
	void CalcBoundBox(int &x0, int &y0, int &z0, int &x1, int &y1, int &z1) const;
	void CalcBoundBox();
	void Sum(const COneMapVertices &rVertices, const CIndices &rIndices);
	void Sum(const VertexBasedMesh &);
	void InvertNormals();
	int ClassifyVertex(const OneMapVertex &rVertex, bool bUseBoundBox, int C = 0) const;
	void Offset(const OneMapVertex &rOffset);
	int NumTris() const;
	void GetTri(int nTri, int &i0, int &i1, int &i2) const;
	void GetTri(int nTri, int * oi) const;
	void GetTri(int nTri, OneMapVertex &t0, OneMapVertex &t1, OneMapVertex &t2) const;
	void GenMarker(const OneMapVertex &Center, int Dim = 15, bool fLive = true);
private:
	int bezierZTemplate[4][4];
	void beziersurfaceCalcVertex(float dS, float dT, int *pZ);
};

// VertexBasedMesh::NumTris
inline int VertexBasedMesh::NumTris() const {
	return Idxs.NValues / 3;
} // VertexBasedMesh::NumTris

// VertexBasedMesh::GetTri(int, int &, int &, int &)
inline void VertexBasedMesh::GetTri(int nTri, int &i0, int &i1, int &i2) const {
	int ii0 = 3 * nTri;
	i0 = Idxs[ii0], i1 = Idxs[ii0 + 1], i2 = Idxs[ii0 + 2];
} // VertexBasedMesh::GetTri(int, int &, int &, int &)

// VertexBasedMesh::GetTri(int, OneMapVertex &, OneMapVertex &, OneMapVertex &)
inline void VertexBasedMesh::GetTri(int nTri, OneMapVertex &t0, OneMapVertex &t1, OneMapVertex &t2) const {
	int i0, i1, i2;
	GetTri(nTri, i0, i1, i2);
	t0 = Vert[i0], t1 = Vert[i1], t2 = Vert[i2];
} // VertexBasedMesh::GetTri(int, OneMapVertex &, OneMapVertex &, OneMapVertex &)

// VertexBasedMesh::GetTri(int, int *)
inline void VertexBasedMesh::GetTri(int nTri, int * oi) const {
	int ii0 = 3 * nTri;
	for(int k = 0; k < 3; k++) {
		oi[k] = Idxs[ii0 + k];
	}
} // VertexBasedMesh::GetTri(int, int *)

class UniversalMap;

class OneUniversalMapQuad:public VertexBasedMeshRef{
public:
    bool Dirty;
	void Init();
	void Expand(const OneMapVertex &);
	// Bounding sphere:
	OneMapVertex m_Centre;
	int m_Radius;
};

inline void OneUniversalMapQuad::Init() {
	CalcBoundSphere(m_Centre, m_Radius);
}

typedef DynArray<OneUniversalMapQuad*> CUniversalQuadsPointers;

class nHashTableEntry;
typedef Hash<nHashTableEntry> nHashTable;

struct AdvToolParams {
	DWORDS TriNums;
	DWORD argbColor;
};

typedef enum { ATColorize } eAdvToolType;

struct ToolParams {
	int cx, cy, r, dH, d, nColors, nFacture, cz;
	cVector C;
	DWORD *pColors;
	VertexBasedMeshRef Ref;
};

typedef enum {TTChangeHeightParabolic, TTChangeHeightPlanar, TTEvenHeight, TTSmooth, TTExtrude,
TTFacturize, TTColorize, TTSmoothColors, TTDeformParabolic, TTDeformPlanar, TTAdvColorize} eToolType;

typedef enum { GetRef, CutRef } eRefAction;
typedef enum { UnionOp, DifferenceOp } eCombineOp;

class UniversalMap:public VertexBasedMesh{	
public:
	UniversalMap *m_pBackup;
	SurfaceShader m_Shader;
	CUniversalQuadsPointers m_pQuads;
	int QuadX0, QuadY0, QuadLx, QuadLy, QuadsNx, QuadsNy, UMLx, UMLy;

	UniversalMap() { Release(); }
	UniversalMap(const SurfaceShader &Shader, UniversalMap *pBackup = NULL)
	{
		Release();
		m_Shader = Shader;
		m_pBackup = pBackup;
	}
	void Backup_Store(void) { if(m_pBackup) m_pBackup->Copy(this); }
	void Backup_Undo(void)
	{
		if(!m_pBackup) return;
		if(!m_pBackup->Vert.NValues || !m_pBackup->Idxs.NValues ||
			!m_pBackup->m_pQuads.NValues) return;
		UniversalMap T;
		T.Copy(this);
		Copy(m_pBackup);
		m_pBackup->Copy(&T);
		T.Release();
	}
	~UniversalMap(void);
	void Release(void);
	void Copy(const UniversalMap *pUniversalMap);
	MeshesCache* LinkedTo;
	void CreateQuadList();
	void RefreshQuadList();

	bool GetModelMinMaxXYZ(const char *strModelName,
		int &minX, int &minY, int &minZ, int &maxX, int &maxY, int &maxZ);
	void AddModelByName(const char *strModelName, const Matrix4D &m4dMap);
	void CalcMinMax(const OneMapVertex &rMVertex, OneMapVertex &rMin, OneMapVertex &rMax);
	void GetMeshRefCylinder(int cx, int cy, int r, VertexBasedMeshRef *pVBMRef);
	static const int c_CtrlCVOffset;
	static const int c_CtrlCVrMin;
	bool CtrlCV_Loop(CIndices &rPairs, CIndices &rContour);
	inline static bool CtrlCV_PtInRect(int x0, int y0, int Width, int Depth, OneMapVertex &rVertex);
	inline static bool CtrlCV_PtInRound(int cx, int cy, int R, OneMapVertex &rVertex);
	static void CtrlCV_Update(int *oi, int *i, OneMapVertex *v, COneMapVertices &rVertices, nHashTable &rTable, int *flags);
	void Compile(CIndices *pContour = NULL);
	void ArgsParser(char *str, char *tokens, float *pArgs);
	void QuadsSystem_AddRef(const OneMapVertex &v0, const OneMapVertex &v1, const OneMapVertex &v2, int nTri);
	void AddRef2Quads(int nTri);
	void QuadsSystem_Init(void);
	void QuadsSystem_InitBoundingSpheres();
	void QuadsSystem_unInit(void);
	void Sum(const VertexBasedMesh &rMesh, CIndices *pContour = NULL);
	void Sum(const OneMapVertex &v0, const OneMapVertex &v1, const OneMapVertex &v2, int *pnTri = NULL);
	int Add(const OneMapVertex &v0, const OneMapVertex &v1, const OneMapVertex &v2);
	void Sum(int i0, int i1, int i2);
	void Combine(VertexBasedMesh &, eCombineOp);
	static void AlignWithStencil(VertexBasedMesh &rMesh, VertexBasedMesh &rStencil, int cx, int cy, int R);
	static int SectionVertexClassify2D(const OneMapVertex &rSectionA, const OneMapVertex &rSectionB,
		const OneMapVertex &rVertex);
	static int RectVertexClassify2D(const OneMapVertex &rv0, const OneMapVertex &rv1, const OneMapVertex &rv2,
		const OneMapVertex &rv3, const OneMapVertex &rVertex);
	static short THMapZ(int MapCoordX, int MapCoordY);
	bool MeshIsWithin(const VertexBasedMesh &rMesh) const;
	int Refresh();
	int Refresh(eSSIllumination Illumination);
	void CutRefCylinder(int cx, int cy, int r, VertexBasedMeshRef &rRef);//, DWORDS *pBoundaryVertsNums);
	void QuadsInCylinder(int cx, int cy, int r, ints &QuadNums);
	void QuadsRefCylinder(int cx, int cy, int r, VertexBasedMeshRef &Ref, eRefAction RefAction);
	void CutQuadsRefCylinder(int cx, int cy, int r, VertexBasedMeshRef &Ref);
	void GetQuadsRefCylinder(int cx, int cy, int r, VertexBasedMeshRef &Ref);
	void GetQuadsRefAll(VertexBasedMeshRef &Ref);
	void Pack();
	bool ToolOrigDir(const DWORDS &VertNums, cVector &Orig, cVector &Dir) const;
	void Tool(eToolType ToolType, const ToolParams &Params);
	void AdvTool(eAdvToolType AdvToolType, AdvToolParams &AdvParams);
	void QuadsTotalBRect(const ints &QuadNums, int &x0, int &y0, int &x1, int &y1) const;
	void Create(int Size = 16000, DWORD Color = 0xff699d2d);

	struct PickArgs {
		typedef enum { JustPick, PickWithSpread } ePickType;
		// in:
		ePickType Type;
		int xScreen, yScreen, Radius;
		// out:
		VertexBasedMeshRef RefTo;
		DWORDS VertsTo;
		// cache:
		ints QuadNums;
		VertexBasedMeshRef Ref;
		DWORDS VertNums;
		DWORDS Tris;
		cBitMap IsInval;

		void Free() {
			RefTo.Free();
			VertsTo.Free();
			//--------------
			QuadNums.Free();
			Ref.Free();
			VertNums.Free();
			Tris.Free();
		}

		PickArgs(int xScreen, int yScreen) { // JustPick
			this->xScreen = xScreen;
			this->yScreen = yScreen;
			Radius = 0;
			Type = JustPick;
		}
		PickArgs(int xScreen, int yScreen, int Radius) { // PickWithSpread
			this->xScreen = xScreen;
			this->yScreen = yScreen;
			this->Radius = Radius;
			Type = PickWithSpread;
		}
	};
	static int PickTri(const OneMapVertex &, const OneMapVertex &, const OneMapVertex &);
	bool Pick(PickArgs *pArgs);
};

#include "OneSplitMesh.h"

class MeshesCache{
	DynArray<OneSplitMesh*> Meshes;
	DynArray<OneSplitMesh*> CurrentMeshToDraw;
	DynArray<DWORD> DrawnQuadsList;
public:
	UniversalMap* LinkedTo;	
    void Draw();//checks visible/invisible UniversalMap parts, then draws //not ready
    void MakeAllDirty();//not ready
	void MakeDirtySquare(float xL,float yL,float xR,float yR);//not ready
	void MakeDirtySquad(int px,int py);//not ready
};

extern UniversalMap g_UniversalMap;

#endif