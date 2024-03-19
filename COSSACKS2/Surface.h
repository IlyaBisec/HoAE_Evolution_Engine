#ifndef __SURFACE_H__
#define __SURFACE_H__
#define SPLITMESH_MAXNFACTURES	128
const delta=3;
const MaxLenZ=16;
const StepTri=32;

#define VERTEX VertexTS
#define vfVERTEX vfVertexTS

#include "CSun.h"

struct uv_pair{
	float u;
	float v;
};
struct VecPr
{
	int ind;	
	float ang;	
};
struct boolh
{
	int ind;
	unsigned int hash() const {return ind;}
	bool equal(const boolh& el){return ind==el.ind;}
	void copy(const boolh& el){ind=el.ind; }
};
struct Triang
{	
	char Finish; 
	char Num;	
	int Ivm[3];
	int Iv[3];	
	char L[3];	
	int Fr[3];	
	char bFr[3];
	char Nfr;
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

class BaseTriang;
class BaseMapper{
public:
	virtual DWORD GetMappingCode(Vector3D v3dPos, Vector3D v3dNormal) = NULL;	
	virtual uv_pair GetMappingUV(Vector3D v3dPos, Vector3D v3dNormal,
		DWORD dwMappingCode) = NULL;
	float Lx, Ly, su, sv; // u & v scale factors
};
class PlanarMapping:public BaseMapper{
public:
	virtual DWORD GetMappingCode(Vector3D v3dPos, Vector3D v3dNormal);
	virtual uv_pair GetMappingUV(Vector3D v3dPos, Vector3D v3dNormal,
		DWORD dwMappingCode);
};
class VerticalMapping:public BaseMapper{
public:
	//    4
	// 3     5
	//2       6
	// 1     7 
	//    0
	virtual DWORD GetMappingCode(Vector3D v3dPos, Vector3D v3dNormal);
	virtual uv_pair GetMappingUV(Vector3D v3dPos, Vector3D v3dNormal,
		DWORD dwMappingCode);
};
#define MaxMappingType 2
struct OneMapVertex{
	static BaseMapper* Mappings[MaxMappingType];
public:
	OneMapVertex() {}
	OneMapVertex(short x, short y, short z = 0) { this->x = x, this->y = y, this->z = z; }
	_inline static BaseMapper* GetMapper(int nFacture);
	_inline static int GetFactureID(int nFacture);
	_inline static int GetFactureID2(int nFacture);
	_inline static int GetShader(int nFacture);
	DWORD GetDiffuseColor(void);
	DWORD GetSpecularColor(void);
	//---------------------------------------//
	short x;
	short y;
	short z;
	DWORD Color;
	byte  nFacture;
	byte  Shadow;
	char  nx;
	char  ny;
	char  nz;
	bool equal (const OneMapVertex &rVertex)
	{
		return x == rVertex.x && y == rVertex.y && z == rVertex.z &&
			Color == rVertex.Color && Shadow == rVertex.Shadow &&
			nx == rVertex.nx && ny == rVertex.ny && nz == rVertex.nz;
	}
	unsigned int hash() const
	{
		unsigned int h = 1;
		h = 37 * h + x;
		h = 37 * h + y;
		h = 37 * h + z;
		return h;
	}
	void copy(const OneMapVertex &rVertex)
	{
		x = rVertex.x, y = rVertex.y, z = rVertex.z,
			Color = rVertex.Color, nFacture = rVertex.nFacture,
			nx = rVertex.nx, ny = rVertex.ny, rVertex.nz;
	}
};

inline operator == (const OneMapVertex &A, const OneMapVertex &B)
{ return (A.x == B.x) && (A.y == B.y) && (A.z == B.z); }

typedef DynArray<OneMapVertex> COneMapVertices;

class BaseSurfaceFunction{
public:
	//virtual float GetSurfaceHeight(float x,float y);
	virtual void ModifyVertex(OneMapVertex& V);
	virtual bool CheckIfSubDivNeed(const OneMapVertex* V1,const OneMapVertex* V2,OneMapVertex& V);
	virtual bool ShiftVertex(OneMapVertex& V);	
};
typedef void func_3D(Vector3D& V,void* param);
typedef DWORD color_func_3D(Vector3D& V,void* param);
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

class VertexBasedMeshRef{
public:
	~VertexBasedMeshRef();
	void Release(void);
	VertexBasedMesh* pOrigin;
	DynArray<DWORD> nTriangles;
	int x0, y0, w, h;
	void ExtractUniqueVertices(DynArray<OneMapVertex*> *pDAVertices);
	void SupplementMapOfHeights(void);
	void SupplementNormals(int cx, int cy, int r);
	void CalcNormals(void);
//	void SupplementLight(int cx, int cy, int r);
	void CreateVertexBasedMesh(VertexBasedMesh *pVBMesh,
		Hash<OneMapVertexHash> *ppVBMesh_VertHash);
	void toVertexBasedMesh(VertexBasedMesh &rMesh);
	void SupplementShadow(const CSun *pSun = NULL);

	void ModifySurface(int MaxSubDiv,BaseSurfaceFunction* Func);
	void Triangulation(DynArray<int>& MasIn);
	void ReturnVertrex(DynArray<VecPr>& MasOut);
	void Smooth(float Proc, int Smnum);
	void VertexList(DynArray<int>& List);
	void RecursDeleting(Hash<boolh>& WhatDel, int ii, int jj, int k);
	void DeleteFlattening();
	void ChangeIndex(Hash<boolh>& WhatDel, int j1, int i1);
	bool IfDelTri(int ii, int jj, int& k);	
};


typedef DynArray<Vector3D> CVectors3D;
typedef DynArray<float> floats;
typedef DynArray<int> ints;

typedef DynArray<DWORD> CIndices;

class MeshesCache;
class CVector3D;

class VertexBasedMesh{	
public:

	DynArray<OneMapVertex> Vert;
	DynArray<DWORD> Idxs;
	~VertexBasedMesh();
	void Release(void);

	//helpers
	virtual void GetMeshRefSphere(Vector3D v3dCenter, float Radius,
		VertexBasedMeshRef *pVBMeshRef);
	virtual void GetMeshRefBox(int x, int y, int w, int l, VertexBasedMeshRef *pVBMeshRef);
	virtual void GetMeshRefCylinder(int cx, int cy, int r, VertexBasedMeshRef &rRef);

	virtual void RemoveVertices(int idx,int count){};//not ready
	virtual void RemoveIndices (int idx,int count){};//not ready

	virtual int  AddVertex(OneMapVertex& V){
		return Vert.Add(V);
	};
	virtual int  AddIndex(int idx){
		return Idxs.Add(idx);
	};

	virtual void ReportAboutChange(Vector3D V){};//not ready
	// Generators
	void CreateHexagonalPlane(Vector3D v3dOrigin, int Sx, int Sy,
		int Dx, int Dy, DWORD dwColor, byte nFacture, byte Shadow);
	void CreateBezierHexagonalPlane(Vector3D v3dOrigin, int Sx, int Sy,
		int Dx, int Dy, DWORD dwColor, byte nFacture, byte Shadow,
		int z00,	int z01,	int z02,	int z03,
		int z10,	int z11,	int z12,	int z13,
		int z20,	int z21,	int z22,	int z23,
		int z30,	int z31,	int z32,	int z33);
	void CalcNormals(void);
	void RasterizeTriangle(int *pZBuffer, const int cZBufferSide, const Vector3D &T0, const Vector3D &T1, const Vector3D &T2);
	void CastOneShadow(const int cZBufferSide, float k, Vector3D *pTVertices, int *pZBuffer, float *pShadows, const Vector3D &rSource);
	void CastShadows(const CVectors3D &rSources, float k, int ShadowMinLevel, int ShadowMaxLevel, float maxShadow);
	void Shade(bool bCastShadows, const CSun *pSun = NULL);

	void CreateSphere(Vector3D Center,float Radius,DWORD Color,int Facture,int NSubDiv);
	void CreatePatch(Vector3D LU,Vector3D RU,Vector3D LD,Vector3D RD,int NSectX,int NSectY,DWORD Color,int Facture);//not ready
	void CreateNormals();
//	void CreateShadow(Vector3D vec, int min, int max);		
	void CreateSurface(int Nx,int Ny,float Step,BaseSurfaceFunction* Func);
	void CreateRandomPatch(int Nx,int Ny,float Step);
	void ModifySurface(int MaxSubDiv,BaseSurfaceFunction* Func);
	void Smooth(float Proc, int Smnum);
	void Smooth(DynArray<bool>& WhatSm, float Proc, int Smnum);
	//operations
	void PerformOp (Vector3D Base,float radius,func_3D* fn,void* param,bool AutoSubdiv);//not ready	
	void AddMesh   (Vector3D* Base,VertexBasedMesh* Mesh);//not ready
	void SubMesh   (Vector3D* Base,VertexBasedMesh* Mesh);//not ready
	void Simplify  (Vector3D Base,float Radius,float MaxDiff);//not ready
	void SmoothSharpEdges(Vector3D Base,float Radius,float degree);//not ready
	void SmoothSurface(Vector3D Base,float Radius);//not ready
	void AddRoundBlob(Vector3D Base,float Radius1,float Radius2,float Dz,bool AutoSubDiv);
	void SetHeightInRadius(Vector3D Base,float Radius1,float Radius2,float H,bool AutoSubDiv);
	//...
	//coloring&texturing
	//void SetFactureInRadius(Vector3D v3dBase, int Radius, int nFacture);

    void SetColorInRadius(Vector3D Base,int Radius,color_func_3D* fn,void* param,bool AutoSubDiv);//not ready
	void SetColorInRadius(Vector3D Base,int Radius1,int Radius2,DWORD Color,bool AutoSubDiv);//not ready
	bool fromModelByName(const char *strModelName, const Matrix4D &m4dWorld);
	void AddTriangle(const OneMapVertex &v0, const OneMapVertex &v1, const OneMapVertex &v2);
	int CrossSectionTriangle(const CVector3D &a, const CVector3D &b, DWORD nTri);
	bool VertexIsInside(const OneMapVertex &rVertex);
	bool CrossSectionMesh(const OneMapVertex &rVertexA, const OneMapVertex &rVertexB);
	inline void GetTriangle(DWORD nTri, OneMapVertex &v0, OneMapVertex &v1, OneMapVertex &v2) const;
	void GetTriangleCentre(DWORD nTri, OneMapVertex &c) const;
	bool CalcZ(const OneMapVertex &v, short &rZ) const;
	bool CalcBoundRect(int &x0, int &y0, int &x1, int &y1) const;
	void Sum(const COneMapVertices &rVertices, const CIndices &rIndices);
	void InvertNormals(void);
private:
	int bezierZTemplate[4][4];
	void beziersurfaceCalcVertex(float dS, float dT, int *pZ);
};

class UniversalMap;
class BaseDirectionalFunction:public BaseSurfaceFunction{
	float xc;
	float yc;
	float zc;
	Vector3D dir;
	float Radius;
	int modl[256];
public:
	void Create(Vector3D Center,Vector3D Direction, float R)
	{ 
		xc=Center.x; yc=Center.y; zc=Center.z; 
		dir=Direction; 
		dir.x*=(127/Direction.norm()); dir.y*=(127/Direction.norm()); dir.z*=(127/Direction.norm());
		Radius=R;
		for(int i=0;i<256;i++)modl[i]=rand()&255;
		for(int i=1;i<1024;i++){
			modl[i%255]=(modl[i%255]+modl[(i+1)%255]+modl[(i-1)%255])/3;
		}
	};
	float GetMaxR(float Fi){return Radius*modl[int(1000+Fi*128/c_PI)%255]/128;};
	float GetModulator(float Theta, float Fi){return GetMaxR(Fi);};
	bool InCyl(float eps, const OneMapVertex& V, float& H, float& R, float& Fi);
	bool InEnd(float eps, const OneMapVertex& V, float& Fi, float& Theta);
	bool InCyl(float eps, const OneMapVertex& V);
	bool InEnd(float eps, const OneMapVertex& V);
	void ProectOnCyl(OneMapVertex& V, float& H, float MaxR);
	bool ReturnInfo(float eps, const OneMapVertex& V){return (InCyl(eps, V) || InEnd(eps, V));};	
	bool ReturnInfo(float eps, const OneMapVertex& V, float& H, float& R, float& Fi, float& Theta)
	{
		return (InCyl(eps, V, H, R, Fi) || InEnd(eps, V, Fi, Theta));
	};	
	void Approximation(VertexBasedMeshRef& Mref, DynArray<int>& List, Hash<boolh>& VertIsIn, int rep);
	void DeleteDirectionalSurface(VertexBasedMeshRef& Mref);	
	virtual void ModifyVertex(OneMapVertex& V);
	virtual bool CheckIfSubDivNeed(const OneMapVertex* V1,const OneMapVertex* V2,OneMapVertex& V);
	void ReturnMeshRef(UniversalMap& UniMap, VertexBasedMeshRef& Mref, int& ox1, int& ox2, int& oy1, int& oy2);	
};
class BaseSphericalFunction:public BaseSurfaceFunction{
	float xc;
	float yc;
	float zc;
public:
	void Create(Vector3D Center){ xc=Center.x; yc=Center.y; zc=Center.z;};
	virtual float GetModulator(float Theta,float Fi){return 200.0;};
	void DeleteSphericalSurface(VertexBasedMesh& MESH);
	virtual void ModifyVertex(OneMapVertex& V);
	virtual bool CheckIfSubDivNeed(const OneMapVertex* V1,const OneMapVertex* V2,OneMapVertex& V);
	virtual bool ShiftVertex(OneMapVertex& V);
};
class OneUniversalMapQuad:public VertexBasedMeshRef{
public:
    bool Dirty;
};
typedef DynArray<OneUniversalMapQuad*> CUniversalQuadsPointers;

class nHashTableEntry;
typedef Hash<nHashTableEntry> nHashTable;

//class CUniversalMapClipboard;
class UniversalMap:public VertexBasedMesh{	
public:
	UniversalMap *m_pBackup;
	CSun *m_pSun;
	bool bWire;
	CUniversalQuadsPointers m_pQuads;
	int QuadX0, QuadY0, QuadLx, QuadLy, QuadsNx, QuadsNy, UMLx, UMLy;

	UniversalMap() { Release(); }
	UniversalMap(CSun *pSun, UniversalMap *pBackup = NULL)
	{
		Release();
		m_pSun = pSun;
		m_pBackup = pBackup;
		bWire = false;
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
	void ExtractQuadsInRadius(CUniversalQuadsPointers &rQuadsPointers, int cx, int cy, int r);
	bool ExtractQuadsInQuad_InQuad(int x0, int y0, int x1, int y1, int X0, int Y0, int X1, int Y1);
	void ExtractQuadsInQuad(CUniversalQuadsPointers &rQuadsPointers, int cx, int cy, int S);
	void ExtractQuads(CUniversalQuadsPointers &rQuadsPointers, const VertexBasedMesh &rVBMesh);
	void CreateQuadList();
	void RefreshQuadList();
	//void InitUniversalMapQuad(void); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	void ModifySurface(int x1, int y1, int x2, int y2, int MaxSubDiv,BaseSurfaceFunction* Func);
	void FillMapOfHeights();
	void SupplementMapOfHeightsInRect(int x0, int y0, int x1, int y1);
	void SetFactureInRadius(int cx, int cy, int r, int nFacture);
	void SetColorInRadius(int cx, int cy, int r, int d, DWORD Color);
	void SetRandomColorsInRadius(int cx, int cy, int r, int d, DWORD* Colors,int NColors);
	void SmoothColors(int cx, int cy, int r);
	void CreateHollow(int cx, int cy, int r, int depth);
	bool GetModelMinMaxXYZ(const char *strModelName,
		int &minX, int &minY, int &minZ, int &maxX, int &maxY, int &maxZ);
	void AddModelByName(const char *strModelName, const Matrix4D &m4dMap);
	void CalcMinMax(const OneMapVertex &rMVertex, OneMapVertex &rMin, OneMapVertex &rMax);
	void CombineModelByName(const char *strModelName, const Matrix4D &m4dWorld);
	void GetMeshRefCylinder(int cx, int cy, int r, VertexBasedMeshRef *pVBMRef);
	void ChangeHeightParabolic(int cx, int cy, int r, int dH);
	void ChangeHeightPlanar(int cx, int cy, int r, int dH);
	void EvenHeightInRadius(int cx, int cy, int r);
	void SmoothHeightInRadius(int cx, int cy, int r);
	static const int c_CtrlCVOffset;
	static const int c_CtrlCVrMin;
	bool CtrlCV_Loop(CIndices &rPairs, CIndices &rContour);
	inline static bool CtrlCV_PtInRect(int x0, int y0, int Width, int Depth, OneMapVertex &rVertex);
	inline static bool CtrlCV_PtInRound(int cx, int cy, int R, OneMapVertex &rVertex);
	static void CtrlCV_Update(int *oi, int *i, OneMapVertex *v, COneMapVertices &rVertices, nHashTable &rTable, int *flags);
//	void CtrlC(CUniversalMapClipboard &rClipboard, int cx, int cy, int r);
//	void CtrlV_ExtractlVertices(const CIndices &rContour, ClVertices &rlVertices,
//		bool fromStuffing, float cx, float cy, float l);
//	void CtrlV(CUniversalMapClipboard &rClipboard, int cx, int cy, int dz);
	void Compile(CIndices *pContour = NULL);
	void ArgsParser(char *str, char *tokens, float *pArgs);
	void QuadsSystem_AddRef(const OneMapVertex &v0, const OneMapVertex &v1, const OneMapVertex &v2, int nTri);
	void QuadsSystem_Init(void);
	void QuadsSystem_unInit(void);
	void Sum(VertexBasedMesh &rMesh, CIndices *pContour = NULL);
	void Sum(const OneMapVertex &v0, const OneMapVertex &v1, const OneMapVertex &v2, int *pnTri = NULL);
	void Sum(int i0, int i1, int i2);
	void Difference(const VertexBasedMesh &rMesh);
	static void AlignWithStencil(VertexBasedMesh &rMesh, VertexBasedMesh &rStencil, int cx, int cy, int R);
	static int SectionVertexClassify2D(const OneMapVertex &rSectionA, const OneMapVertex &rSectionB,
		const OneMapVertex &rVertex);
	static int RectVertexClassify2D(const OneMapVertex &rv0, const OneMapVertex &rv1, const OneMapVertex &rv2,
		const OneMapVertex &rv3, const OneMapVertex &rVertex);
	static short THMapZ(int MapCoordX, int MapCoordY);
	int Refresh(void)
	{
		if(!Vert.NValues || !Idxs.NValues) return -1;
		DWORD tStart = GetTickCount();
		Compile();
		CalcNormals();
		FillMapOfHeights();
		Shade(true, m_pSun);
		tStart = GetTickCount() - tStart;
		return tStart;
	}
	//virtual void GetMeshRef(Vector3D Base,float Radius,VertexBasedMeshRef& result);//not ready
	//virtual void RemoveVertices(int idx,int count);//not ready
	//virtual void RemoveIndices (int idx,int count);//not ready
	//virtual int  AddVertex();//not ready
	//virtual int  AddIndex();//not ready
	//virtual void ReportAboutChange(Vector3D V);//not ready
};

class OneSplitMesh{
public:
	DynArray<BaseMesh*> pBaseMeshes;
	void Release(void);
	~OneSplitMesh();
	bool SpeedySplit(VertexBasedMeshRef &rVBMRef);
	bool AdvSplitMesh(VertexBasedMeshRef *pVBMeshRef);
	void Draw(void);
};

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

class BaseTriang{	
public:
	DynArray<Triang> Tri;
	void bInit(VertexBasedMesh& MESH);
	void Init(VertexBasedMeshRef& Mref);
	int bFindFriends(VertexBasedMesh& MESH,BaseSurfaceFunction* Func);
	int gFindFriends();
	int bFindFriends(VertexBasedMesh& MESH,DynArray<int>& ChInd,BaseSurfaceFunction* Func);
	void Partition(DynArray<int>& ChInd);
	void Partition(DynArray<int>& InInd, DynArray<int>& ChInd);	
	int FindFriends(VertexBasedMeshRef& Mref,BaseSurfaceFunction* Func);
	int FindFriends(VertexBasedMeshRef& Mref,DynArray<int>& ChInd,BaseSurfaceFunction* Func);	
	void bReturnIndex(VertexBasedMesh& MESH);
	void ReturnIndex(VertexBasedMeshRef& Mref);	
};
#endif