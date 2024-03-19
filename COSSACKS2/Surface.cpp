#include "stdheader.h"
#include "Surface.h"
#include "ITerrain.h"


#define FLOAT_EPSILON	float(0x00000001)
#define FLOAT_RAND (((rand() % 10000) - 5000) / 5000.0f)
class CVector3D
{
public:
	float x, y, z;
	static const CVector3D Null;
	CVector3D() { *this = CVector3D::Null; }
	CVector3D(float x, float y, float z) { Set(x, y, z); }
	CVector3D(const OneMapVertex &r) { x = r.x, y = r.y, z = r.z; }
	void Set(float x, float y, float z) {this->x = x, this->y = y, this->z = z; }
	CVector3D & operator = (const CVector3D &r)
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
	void Randomize()
	{
		x = FLOAT_RAND, y = FLOAT_RAND, z = FLOAT_RAND;
		Normalize();
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
	CVector3D & operator *= (float f)
	{
		x *= f, y *= f, z *= f;
		return *this;
	}
	friend CVector3D operator + (const CVector3D &, const CVector3D &);
	friend CVector3D operator - (const CVector3D &, const CVector3D &);
	friend float operator * (const CVector3D &, const CVector3D &);
	friend CVector3D operator ^ (const CVector3D &, const CVector3D &);
	friend CVector3D operator * (float, const CVector3D &);
	friend CVector3D operator / (const CVector3D &, float);
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

const CVector3D CVector3D::Null(0.0f, 0.0f, 0.0f);

// ================================================================================
// PlanarMapping::GetMappingCode
// ================================================================================	
DWORD PlanarMapping::GetMappingCode(Vector3D v3dPos, Vector3D v3dNormal)
{
	int px = v3dPos.x / 350 + v3dPos.y / 470;
	int py = v3dPos.x / 520 + v3dPos.y / 430;
	return px + (py << 16);
}

// ================================================================================
// PlanarMapping::GetMappingUV
// ================================================================================	
uv_pair PlanarMapping::GetMappingUV(Vector3D v3dPos, Vector3D v3dNormal,
									DWORD dwMappingCode)
{		
	uv_pair uv;
	float ushift = 0, vshift = 0;
	ushift = (dwMappingCode & 0xFFFF) / 10.0f,
		vshift = (dwMappingCode >> 16) / 10.0f;
	ushift -= int(ushift);
	vshift -= int(vshift);
	uv.u = v3dPos.x / 256.0f + ushift;
	uv.v = v3dPos.y / 512.0f + vshift;
	return uv;
}
// ================================================================================
// VerticalMapping::GetMappingCode
// ================================================================================	
DWORD VerticalMapping::GetMappingCode(Vector3D v3dPos, Vector3D v3dNormal)
{
	float nx = 0, ny = 0, L = 0;
	nx = v3dNormal.x, ny = v3dNormal.y;
	L = sqrt(nx * nx + ny + ny);
	nx /= L;
	ny /= L;
	if((nx >= 0) && (ny >= 0)) // I: 6, 7, 0
	{
		if(nx > 2.4 * ny) return 6;
		if(ny > 2.4 * nx) return 0;
		return 7;
	}
	else
		if((nx < 0) && (ny >=0)) // II: 0, 1, 2
		{
			nx = -nx;
			if(nx > 2.4 * ny) return 2;
			if(ny > 2.4 * nx) return 0;
			return 1;
		}
		else
			if((nx < 0) && (ny < 0)) // III: 2, 3, 4
			{
				nx = -nx;
				ny = -ny;
				if(nx > 2.4 * ny) return 2;
				if(ny > 2.4 * nx) return 4;
				return 3;
			}
			else
				if((nx >= 0) && (ny < 0))	// IV: 4, 5, 6
				{
					ny = -ny;
					if(nx > 2.4 * ny) return 6;
					if(ny > 2.4 * nx) return 4;
					return 5;
				}
				return 0;
}	

// ================================================================================
// VerticalMapping::GetMappingUV
// ================================================================================	
uv_pair VerticalMapping::GetMappingUV(Vector3D v3dPos, Vector3D v3dNormal,
									  DWORD dwMappingCode)
{
	
	uv_pair uv;
	float X = 0, Y = 0, Z = 0;
	X = v3dPos.x / (su * Lx),
		Y = v3dPos.y / (su * Lx),
		Z = v3dPos.z / (sv * Ly);
	switch(dwMappingCode){
			case 0:
				uv.u = -X;
				break;
			case 4:
				uv.u = -X;
				break;
			case 2:
				uv.u = Y;
				break;
			case 6:
				uv.u = Y;
				break;
			case 7:
				uv.u = -(X - Y) / 1.4142f;
				break;
			case 3:
				uv.u = -(X - Y) / 1.4142f;
				break;
			case 1:
				uv.u = -(X + Y) / 1.4142f;
				break;
			case 5:
				uv.u = -(X + Y) / 1.4142f;
				break;
	}
	uv.v = -Z;
	return uv;
}	

// ================================================================================
// OneMapVertex::GetMapper
// ================================================================================	
BaseMapper* OneMapVertex::GetMapper(int nFacture)
{
	int GetFactureUsage(int);
	BaseMapper *pBaseMapper = Mappings[GetFactureUsage(nFacture)];
	float du, dv, su, sv;
	void GetUVInfo(int id, float &du, float &dv, float &su, float &sv);
	GetUVInfo(nFacture, du, dv, su, sv);
	pBaseMapper->su = su, pBaseMapper->sv = sv;
	int GetFacture(int);
	pBaseMapper->Lx = IRS->GetTextureWidth(GetFacture(nFacture));
    pBaseMapper->Ly = IRS->GetTextureHeight(GetFacture(nFacture));
	return pBaseMapper;
}

// ================================================================================
// OneMapVertex::GetFactureID
// ================================================================================	
bool UsesBump(int id);
int GetFactureBump(int id,float& Contrast,float& Bright);
int GetFacture(int);
int OneMapVertex::GetFactureID(int nFacture)
{
	if(UsesBump(nFacture)){
		float x;
		return GetFactureBump(nFacture,x,x);
	}else{
		return GetFacture(nFacture);
	}
}

// ================================================================================
// OneMapVertex::GetFactureID2
// ================================================================================	
int OneMapVertex::GetFactureID2(int nFacture)
{ return GetFacture(nFacture); }

// ================================================================================
// OneMapVertex::GetShader
// ================================================================================	
int OneMapVertex::GetShader(int nFacture)
{
	static int sh  = IRS->GetShaderID("terra_shadowed");
	static int shb = IRS->GetShaderID("dot3specular");
	return UsesBump(nFacture)?shb:sh;
}
BaseMapper* OneMapVertex::Mappings[MaxMappingType] = {new PlanarMapping, new VerticalMapping};

// ================================================================================
// OneMapVertex::GetDiffuseColor
// ================================================================================
DWORD OneMapVertex::GetDiffuseColor(void)
{	
	return MulDWORD(Color,int(Shadow)*2)|0xFF000000;
//	return MulDWORD(Color, 2 * int(Shadow));
//	if(ny < 0) return MulDWORD(Color, 1);
//	else return MulDWORD(Color, 1);
	//return DiffuseColor;
}
// ================================================================================
// OneMapVertex::GetSpecularColor
// ================================================================================
int GetFactureBump(int id,float& Contrast,float& Bright);
DWORD OneMapVertex::GetSpecularColor(void)
{	
	//if(UsesBump(nFacture)){
	float Contrast;
	float Bright;
	int t=GetFactureBump(nFacture,Contrast,Bright);
	Vector3D N(nx,ny,nz);
	N.normalize();
	ILight* iLight = GetGameLight();
	Vector3D L = iLight->GetDir();
	L.reverse();
	float dotp=L.dot(N);
	float b=0.5f;
	if(dotp<0)b+=dotp;
	if(b<0)b=0;
	float a=0.5f-b*dotp;
	Vector3D VL(a*N.x+b*L.x,a*N.y+b*L.y,a*N.z+b*L.z);
	return VectorToColor(ToTangentSpace(VL,N,Vector3D(1,0,0)),0);
	//}else return 0;
}
// ================================================================================
// VertexBasedMeshRef::~VertexBasedMeshRef
// ================================================================================
VertexBasedMeshRef::~VertexBasedMeshRef()
{
	Release();
}

// ================================================================================
// VertexBasedMeshRef::Release
// ================================================================================
void VertexBasedMeshRef::Release(void)
{
	pOrigin = NULL;
	nTriangles.Clear();
}

// ================================================================================
// VertexBasedMesh::GetMeshRefSphere
// ================================================================================
void VertexBasedMesh::GetMeshRefSphere(Vector3D v3dCenter, float Radius,
									   VertexBasedMeshRef *pVBMeshRef)
{
	pVBMeshRef->Release();

	pVBMeshRef->pOrigin = this;
	// selecting triangles within sphere
	int i0 = 0, i1 = 0, i2 = 0, d = 0, R2 = Radius * Radius,
		nTriMax = Idxs.GetAmount() / 3, cx = 0, cy = 0, cz = 0;
	for(int nTri = 0; nTri < nTriMax; nTri++)
	{
		i0 = Idxs[3 * nTri + 0],
			i1 = Idxs[3 * nTri + 1],
			i2 = Idxs[3 * nTri + 2];
		cx = (Vert[i0].x + Vert[i1].x + Vert[i2].x) / 3;
		cy = (Vert[i0].y + Vert[i1].y + Vert[i2].y) / 3;
		cz = (Vert[i0].z + Vert[i1].z + Vert[i2].z) / 3;
		d = (cx - v3dCenter.x) * (cx - v3dCenter.x) +
			(cy - v3dCenter.y) * (cy - v3dCenter.y) +
			(cz - v3dCenter.z) * (cz - v3dCenter.z);
		if(d < R2) pVBMeshRef->nTriangles.Add(nTri);
	}
}

// ================================================================================
// VertexBasedMesh::GetMeshRefBox
// ================================================================================
void VertexBasedMesh::GetMeshRefBox(int x, int y, int w, int l,
									VertexBasedMeshRef *pVBMeshRef)
{
	pVBMeshRef->Release();

	pVBMeshRef->pOrigin = this;
	// selecting triangles within box
	int i0 = 0, i1 = 0, i2 = 0, nTriMax = Idxs.GetAmount() / 3,
		cx = 0, cy = 0;
	for(int nTri = 0; nTri < nTriMax; nTri++)
	{
		i0 = Idxs[3 * nTri + 0],
			i1 = Idxs[3 * nTri + 1],
			i2 = Idxs[3 * nTri + 2];
		cx = (Vert[i0].x + Vert[i1].x + Vert[i2].x) / 3,
			cy = (Vert[i0].y + Vert[i1].y + Vert[i2].y) / 3;
		if((cx < x + w) && (cx >= x) && (cy < y + l) && (cy >= y))
			pVBMeshRef->nTriangles.Add(nTri);
	}
}

bool VertexBasedMesh::CalcBoundRect(int &x0, int &y0, int &x1, int &y1) const
{
	if(!Vert.NValues) return false;
	x0 = x1 = Vert[0].x, y0 = y1 = Vert[0].y;
	for(int i = 1; i < Vert.NValues; i++)
	{
		OneMapVertex &v = Vert[i];
		if(x0 > v.x) x0 = v.x;
		if(y0 > v.x) y0 = v.y;
		if(x1 < v.x) x1 = v.x;
		if(y1 < v.y) y1 = v.y;
	}
	return true;
}

// ================================================================================
// OneSplitMesh::Release
// ================================================================================
void OneSplitMesh::Release()
{
	int N = pBaseMeshes.NValues;
	for(int i = 0; i < N; i++) delete pBaseMeshes[i];
	pBaseMeshes.Clear();
}

// ================================================================================
// OneSplitMesh::~OneSplitMesh
// ================================================================================
OneSplitMesh::~OneSplitMesh()
{
	Release();
}

class SSOneMapVertexHash
{
public:
	DWORD oi, dwMappingCode;
	bool Alpha;
	uv_pair uv;
	unsigned int hash() const
	{ return oi; }
	bool equal(const SSOneMapVertexHash &vtxh)
	{
		if((vtxh.oi == oi) && (vtxh.dwMappingCode == dwMappingCode) &&
			(vtxh.Alpha == Alpha)) return true;
		return false;
	}
	void copy(const SSOneMapVertexHash &vtxh)
	{
		oi = vtxh.oi, dwMappingCode = vtxh.dwMappingCode,
			Alpha = vtxh.Alpha, uv = vtxh.uv;
	}
};

// ================================================================================
// OneSplitMesh::
// ================================================================================
bool OneSplitMesh::SpeedySplit(VertexBasedMeshRef &rVBMRef)
{
	// Clearing...
	Release();
	// Culling:
	int N = rVBMRef.nTriangles.GetAmount();
	if(N == 0) return true;
	// Variables declaration:
	Hash<SSOneMapVertexHash> Vertices;
	DynArray<WORD> Indices;
	SSOneMapVertexHash vtxh0, vtxh1, vtxh2;
	BaseMapper *pBaseMapper = NULL;
	bool TriIsTransitive = false;
	int i = 0, nTri = 0, nFacture = 0, n0 = 0, n1 = 0, n2 = 0,
		nmin = 0, nmed = 0, nmax = 0, i0 = 0, i1 = 0, i2 = 0, NTriMax = 0;
	BaseMesh *pBaseMesh = NULL;
	VERTEX *pvtxData = NULL;
	WORD *pIndices = NULL;
	// EXTRACTING NUMBERS OF TRIANGLES
	// by facture criterion, setting transitivity property
	// storing to nTriangles
	N = rVBMRef.nTriangles.NValues;
	DynArray<DWORD> nTriangles[SPLITMESH_MAXNFACTURES];
	for(i = 0; i < N; i++)
	{
		nTri = rVBMRef.nTriangles[i];
		n0 = rVBMRef.pOrigin->Vert[rVBMRef.pOrigin->Idxs[3 * nTri + 0]].nFacture,
			n1 = rVBMRef.pOrigin->Vert[rVBMRef.pOrigin->Idxs[3 * nTri + 1]].nFacture,
			n2 = rVBMRef.pOrigin->Vert[rVBMRef.pOrigin->Idxs[3 * nTri + 2]].nFacture;
		if((n0 == n1) && (n0 == n2)) nTriangles[n0].Add(nTri + 0x80000000);
		else
		{
			nmin = SPLITMESH_MAXNFACTURES;
			if(n0 < nmin) nmin = n0;
			if(n1 < nmin) nmin = n1;
			if(n2 < nmin) nmin = n2;
			nTriangles[nmin].Add(nTri + 0x80000000);
			nmax = -1;
			if(n0 > nmax) nmax = n0;
			if(n1 > nmax) nmax = n1;
			if(n2 > nmax) nmax = n2;
			nTriangles[nmax].Add(nTri);
			nmed = nmin;
			if((n0 != nmin) && (n0 != nmax)) nmed = n0;
			if((n1 != nmin) && (n1 != nmax)) nmed = n1;
			if((n2 != nmin) && (n2 != nmax)) nmed = n2;
			if(nmed != nmin) nTriangles[nmed].Add(nTri); 
		}
	}
	// Searching max number of triangles
	for(nFacture = 0; nFacture < SPLITMESH_MAXNFACTURES; nFacture++)
		if(nTriangles[nFacture].NValues > NTriMax)
			NTriMax = nTriangles[nFacture].NValues;
	Indices.Check(3 * NTriMax);
	// Enumerating factures and processing
	for(nFacture = 0; nFacture < SPLITMESH_MAXNFACTURES; nFacture++)
	{
		N = nTriangles[nFacture].NValues;
		if((N == 0) || (N >= 0xFFFF / 3)) continue;
		// CREATING IMAGE OF MESH(ES) FOR FACTURE
		// number nFacture and storing data to Vertices and Indices
		pBaseMapper = OneMapVertex::GetMapper(nFacture);
		for(i = 0; i < N; i++)
		{
			nTri = nTriangles[nFacture][i];
			if(nTri >= 0x80000000)
			{
				vtxh0.Alpha = vtxh1.Alpha = vtxh2.Alpha = false;
				nTri -= 0x80000000;
				TriIsTransitive = false;
			}
			else TriIsTransitive = true;
			vtxh0.oi = rVBMRef.pOrigin->Idxs[3 * nTri + 0],
				vtxh1.oi = rVBMRef.pOrigin->Idxs[3 * nTri + 1],
				vtxh2.oi = rVBMRef.pOrigin->Idxs[3 * nTri + 2];
			OneMapVertex &rvtx = rVBMRef.pOrigin->Vert[vtxh0.oi];
			vtxh0.dwMappingCode = vtxh1.dwMappingCode = vtxh2.dwMappingCode =
				pBaseMapper->GetMappingCode(Vector3D(rvtx.x, rvtx.y, rvtx.z),
				Vector3D(rvtx.nx, rvtx.ny, rvtx.nz));
			if(TriIsTransitive == true)
			{
				vtxh0.Alpha = (rVBMRef.pOrigin->Vert[vtxh0.oi].nFacture != nFacture ? true:false);
				vtxh1.Alpha = (rVBMRef.pOrigin->Vert[vtxh1.oi].nFacture != nFacture ? true:false);
				vtxh2.Alpha = (rVBMRef.pOrigin->Vert[vtxh2.oi].nFacture != nFacture ? true:false);
			}
			i0 = Vertices.find(vtxh0),
				i1 = Vertices.find(vtxh1),
				i2 = Vertices.find(vtxh2);
			if(i0 == NO_ELEMENT)
			{
				OneMapVertex &rvtx = rVBMRef.pOrigin->Vert[vtxh0.oi];
				vtxh0.uv = pBaseMapper->GetMappingUV(Vector3D(rvtx.x, rvtx.y, rvtx.z),
					Vector3D(rvtx.nx, rvtx.ny, rvtx.nz), vtxh0.dwMappingCode);
				i0 = Vertices.add(vtxh0);
			}
			if(i1 == NO_ELEMENT)
			{
				OneMapVertex &rvtx = rVBMRef.pOrigin->Vert[vtxh1.oi];
				vtxh1.uv = pBaseMapper->GetMappingUV(Vector3D(rvtx.x, rvtx.y, rvtx.z),
					Vector3D(rvtx.nx, rvtx.ny, rvtx.nz), vtxh1.dwMappingCode);
				i1 = Vertices.add(vtxh1);
			}
			if(i2 == NO_ELEMENT)
			{
				OneMapVertex &rvtx = rVBMRef.pOrigin->Vert[vtxh2.oi];
				vtxh2.uv = pBaseMapper->GetMappingUV(Vector3D(rvtx.x, rvtx.y, rvtx.z),
					Vector3D(rvtx.nx, rvtx.ny, rvtx.nz), vtxh2.dwMappingCode);
				i2 = Vertices.add(vtxh2);
			}
			Indices.Add(i0);
			Indices.Add(i1);
			Indices.Add(i2);
		}
		// CREATING BASE MESH(ES)
		// based on Vertices and Indices
		pBaseMesh = new BaseMesh();
		if(pBaseMesh == NULL) return false;
		pBaseMesh->create(Vertices.numElem(), Indices.NValues, vfVertexTS);
		pvtxData = (VERTEX*)pBaseMesh->getVertexData();
		if(pvtxData == NULL) return false;
		pIndices = pBaseMesh->getIndices();
		if(pIndices == NULL) return false;
		// copying vertices...
		N = Vertices.numElem();
		for(i = 0; i < N; i++)
		{
			SSOneMapVertexHash &rvtxh = Vertices.elem(i);
			OneMapVertex &rvtx = rVBMRef.pOrigin->Vert[rvtxh.oi];
			pvtxData->x = rvtx.x, pvtxData->y = rvtx.y, pvtxData->z = rvtx.z;
			pvtxData->u = rvtxh.uv.u, pvtxData->v = rvtxh.uv.v;
			pvtxData->diffuse = rvtx.GetDiffuseColor();
			pvtxData->specular = rvtx.GetSpecularColor();
			if(rvtxh.Alpha == true) pvtxData->diffuse &= 0x00FFFFFF;
			pvtxData++;
		}
		// copying indices...
		memcpy(pIndices, Indices.Values, Indices.NValues * sizeof(WORD));
		// finalizing BaseMesh...
		pBaseMesh->setNVert(Vertices.numElem());
		pBaseMesh->setNInd(Indices.NValues);
		pBaseMesh->setNPri(Indices.NValues / 3);
		pBaseMesh->setTexture(OneMapVertex::GetFactureID(nFacture));
		pBaseMesh->setTexture(OneMapVertex::GetFactureID2(nFacture), 1);
		pBaseMesh->setShader(OneMapVertex::GetShader(nFacture));
		pBaseMeshes.Add(pBaseMesh);
		pBaseMesh = NULL;
		pvtxData = NULL;
		pIndices = NULL;
		// Clearing up...
		Vertices.reset();
		Indices.FastClear();
	}
	return true;
}

class MCAOneMapVertexHash
{
public:
	DWORD oi, i, dwMappingCode;
	bool Alpha;
	uv_pair uv;
	unsigned int hash() const
	{ return oi; }
	bool equal(const MCAOneMapVertexHash &mcavtxh)
	{
		if((mcavtxh.oi == oi) && (mcavtxh.dwMappingCode == dwMappingCode) &&
			(mcavtxh.Alpha == Alpha)) return true;
		return false;
	}
	void copy(const MCAOneMapVertexHash &mcavtxh)
	{
		oi = mcavtxh.oi, i = mcavtxh.i, dwMappingCode = mcavtxh.dwMappingCode,
			Alpha = mcavtxh.Alpha, uv = mcavtxh.uv;
	}
};

// ================================================================================
// OneSplitMesh::AdvSplitMesh
// ================================================================================
bool OneSplitMesh::AdvSplitMesh(VertexBasedMeshRef *pVBMeshRef)
{
	// uninitializing
	Release();

	// ---------------------- CACHING --------------------------
	// vertices and indices in RefVertices and RefIndices
	// from pVBMeshRef->pOrigin basing on pVBMeshRef->nTriangles
	int i = 0, nTri = 0, N = 0, i0 = 0, i1 = 0, i2 = 0, oi0 = 0, oi1 = 0, oi2 = 0;
	VertexBasedMesh *pVBMesh = NULL;
	DynArray<OneMapVertex> RefVertices;
	DynArray<DWORD> RefIndices;
	Hash<OneMapVertexHash> RefVerticesHash;
	OneMapVertexHash vtxh0, vtxh1, vtxh2;
	pVBMesh = pVBMeshRef->pOrigin;
	N = pVBMeshRef->nTriangles.GetAmount();
	for(i = 0; i < N; i++)
	{
		nTri = pVBMeshRef->nTriangles[i];
		oi0 = pVBMesh->Idxs[3 * nTri + 0],
			oi1 = pVBMesh->Idxs[3 * nTri + 1],
			oi2 = pVBMesh->Idxs[3 * nTri + 2];
		vtxh0.oi = oi0,
			vtxh1.oi = oi1,
			vtxh2.oi = oi2;
		i0 = RefVerticesHash.find(vtxh0),
			i1 = RefVerticesHash.find(vtxh1),
			i2 = RefVerticesHash.find(vtxh2);
		if(i0 == NO_ELEMENT)
		{
			i0 = RefVertices.GetAmount();
			RefVertices.Add(pVBMesh->Vert[oi0]);
			vtxh0.i = i0;
			RefVerticesHash.add(vtxh0);
		}
		else
		{
			vtxh0 = RefVerticesHash.elem(i0);
			i0 = vtxh0.i;
		}
		RefIndices.Add(i0);
		if(i1 == NO_ELEMENT)
		{
			i1 = RefVertices.GetAmount();
			RefVertices.Add(pVBMesh->Vert[oi1]);
			vtxh1.i = i1;
			RefVerticesHash.add(vtxh1);
		}
		else
		{
			vtxh1 = RefVerticesHash.elem(i1);
			i1 = vtxh1.i;
		}
		RefIndices.Add(i1);
		if(i2 == NO_ELEMENT)
		{
			i2 = RefVertices.GetAmount();
			RefVertices.Add(pVBMesh->Vert[oi2]);
			vtxh2.i = i2;
			RefVerticesHash.add(vtxh2);
		}
		else
		{
			vtxh2 = RefVerticesHash.elem(i2);
			i2 = vtxh2.i;
		}
		RefIndices.Add(i2);
	}
	RefVerticesHash.reset();

	// --------------------- CALCULATING MAPPING CODE --------------------
	// for all vertices in RefVertices
	DynArray<DWORD>MappingCodes;
	DWORD dwMappingCode = 0;
	BaseMapper *pBaseMapper = NULL;
	OneMapVertex vtx;
	Vector3D v3dPos, v3dNormal;
	N = RefVertices.GetAmount();
	for(i = 0; i < N; i++)
	{
		vtx = RefVertices[i];
		pBaseMapper = OneMapVertex::GetMapper(vtx.nFacture);
		v3dPos.x = vtx.x,
			v3dPos.y = vtx.y,
			v3dPos.z = vtx.z;
		v3dNormal.x = vtx.nx,
			v3dNormal.y = vtx.ny,
			v3dNormal.z = vtx.nz;
		dwMappingCode = pBaseMapper->GetMappingCode(v3dPos, v3dNormal);
		MappingCodes.Add(dwMappingCode);
		pBaseMapper = NULL;
	}


	// ------------------------ SPLITTING -------------------------------
	// sequence numbers of triangles in cache (RefVertices & RefIndices)
	// to nTransitiveFactureTriangles and nNotTransitiveFactureTriangles
	// by facture criterion
	DynArray<DWORD> snNotTransitiveFactureTriangles[SPLITMESH_MAXNFACTURES],
		snTransitiveFactureTriangles[SPLITMESH_MAXNFACTURES];
	int n0 = 0, n1 = 0, n2 = 0, nmin = 0, nmed = 0, nmax = 0;
	N = RefIndices.GetAmount() / 3;
	int MAX = 0;
	for(nTri = 0; nTri < N; nTri++)
	{
		i0 = RefIndices[3 * nTri + 0],
			i1 = RefIndices[3 * nTri + 1],
			i2 = RefIndices[3 * nTri + 2];
		n0 = RefVertices[i0].nFacture,
			n1 = RefVertices[i1].nFacture,
			n2 = RefVertices[i2].nFacture;
		if(n0 > MAX) MAX = n0;
		if(n1 > MAX) MAX = n1;
		if(n2 > MAX) MAX = n2;
	}
	for(nTri = 0; nTri < N; nTri++)
	{
		i0 = RefIndices[3 * nTri + 0],
			i1 = RefIndices[3 * nTri + 1],
			i2 = RefIndices[3 * nTri + 2];
		n0 = RefVertices[i0].nFacture,
			n1 = RefVertices[i1].nFacture,
			n2 = RefVertices[i2].nFacture;
		if((n0 == n1) && (n0 == n2)) snNotTransitiveFactureTriangles[n0].Add(nTri);
		else
		{
			nmin = SPLITMESH_MAXNFACTURES;
			if(n0 < nmin) nmin = n0;
			if(n1 < nmin) nmin = n1;
			if(n2 < nmin) nmin = n2;
			snNotTransitiveFactureTriangles[nmin].Add(nTri);
			nmax = -1;
			if(n0 > nmax) nmax = n0;
			if(n1 > nmax) nmax = n1;
			if(n2 > nmax) nmax = n2;
			snTransitiveFactureTriangles[nmax].Add(nTri);
			nmed = nmin;
			if((n0 != nmin) && (n0 != nmax)) nmed = n0;
			if((n1 != nmin) && (n1 != nmax)) nmed = n1;
			if((n2 != nmin) && (n2 != nmax)) nmed = n2;
			if((nmed != nmin) && (nmed != nmax))
				snTransitiveFactureTriangles[nmed].Add(nTri); 
		}
	}

	// ------------------------ FILLING -------------------------------
	// MeshVertices and MeshIndices
	DynArray<OneMapVertex> MeshVertices[SPLITMESH_MAXNFACTURES];
	DynArray<bool> AlphaMeshVertices[SPLITMESH_MAXNFACTURES];
	DynArray<uv_pair> UV[SPLITMESH_MAXNFACTURES];
	uv_pair uv = {0, 0};
	DynArray<DWORD> MeshIndices[SPLITMESH_MAXNFACTURES];

	Hash<MCAOneMapVertexHash> MeshVerticesHash;
	MCAOneMapVertexHash mcavtxh0, mcavtxh1, mcavtxh2;
	int GetNFactures(void);
	int nFacture = 0, NFactures = 0;
	NFactures = GetNFactures();
	for(nFacture = 0; nFacture < NFactures; nFacture++)
	{
		pBaseMapper = OneMapVertex::GetMapper(nFacture);
		// enumerating snNotTransitiveFactureTriangles
		N = snNotTransitiveFactureTriangles[nFacture].GetAmount();
		for(i = 0; i < N; i++)
		{
			nTri = snNotTransitiveFactureTriangles[nFacture][i];
			oi0 = RefIndices[3 * nTri + 0],
				oi1 = RefIndices[3 * nTri + 1],
				oi2 = RefIndices[3 * nTri + 2];
			dwMappingCode = MappingCodes[oi0];
			mcavtxh0.oi = oi0, mcavtxh0.dwMappingCode = dwMappingCode, mcavtxh0.Alpha = false,
				mcavtxh1.oi = oi1, mcavtxh1.dwMappingCode = dwMappingCode, mcavtxh1.Alpha = false,
				mcavtxh2.oi = oi2, mcavtxh2.dwMappingCode = dwMappingCode, mcavtxh2.Alpha = false;
			i0 = MeshVerticesHash.find(mcavtxh0),
				i1 = MeshVerticesHash.find(mcavtxh1),
				i2 = MeshVerticesHash.find(mcavtxh2);
			if(i0 == NO_ELEMENT)
			{
				i0 = MeshVertices[nFacture].GetAmount();
				vtx = RefVertices[oi0];
				v3dPos.x = vtx.x,
					v3dPos.y = vtx.y,
					v3dPos.z = vtx.z;
				v3dNormal.x = vtx.nx,
					v3dNormal.y = vtx.ny,
					v3dNormal.z = vtx.nz;
				uv = pBaseMapper->GetMappingUV(v3dPos, v3dNormal, dwMappingCode);
				UV[nFacture].Add(uv);
				MeshVertices[nFacture].Add(vtx);
				AlphaMeshVertices[nFacture].Add(false);
				mcavtxh0.i = i0;
				MeshVerticesHash.add(mcavtxh0);
			}
			else
			{
				mcavtxh0 = MeshVerticesHash.elem(i0);
				i0 = mcavtxh0.i;
			}
			if(i1 == NO_ELEMENT)
			{
				i1 = MeshVertices[nFacture].GetAmount();
				vtx = RefVertices[oi1];
				v3dPos.x = vtx.x,
					v3dPos.y = vtx.y,
					v3dPos.z = vtx.z;
				v3dNormal.x = vtx.nx,
					v3dNormal.y = vtx.ny,
					v3dNormal.z = vtx.nz;
				uv = pBaseMapper->GetMappingUV(v3dPos, v3dNormal, dwMappingCode);
				UV[nFacture].Add(uv);
				MeshVertices[nFacture].Add(vtx);
				AlphaMeshVertices[nFacture].Add(false);
				mcavtxh1.i = i1;
				MeshVerticesHash.add(mcavtxh1);
			}
			else
			{
				mcavtxh1 = MeshVerticesHash.elem(i1);
				i1 = mcavtxh1.i;
			}
			if(i2 == NO_ELEMENT)
			{
				i2 = MeshVertices[nFacture].GetAmount();
				vtx = RefVertices[oi2];
				v3dPos.x = vtx.x,
					v3dPos.y = vtx.y,
					v3dPos.z = vtx.z;
				v3dNormal.x = vtx.nx,
					v3dNormal.y = vtx.ny,
					v3dNormal.z = vtx.nz;
				uv = pBaseMapper->GetMappingUV(v3dPos, v3dNormal, dwMappingCode);
				UV[nFacture].Add(uv);
				MeshVertices[nFacture].Add(vtx);
				AlphaMeshVertices[nFacture].Add(false);
				mcavtxh2.i = i2;
				MeshVerticesHash.add(mcavtxh2);
			}
			else
			{
				mcavtxh2 = MeshVerticesHash.elem(i2);
				i2 = mcavtxh2.i;
			}
			MeshIndices[nFacture].Add(i0);
			MeshIndices[nFacture].Add(i1);
			MeshIndices[nFacture].Add(i2);
		}
		// enumerating snTransitiveFactureTringles
		OneMapVertex vtx0, vtx1, vtx2;
		int n0 = 0, n1 = 0, n2 = 0;
		N = snTransitiveFactureTriangles[nFacture].GetAmount();
		for(i = 0; i < N; i++)
		{
			nTri = snTransitiveFactureTriangles[nFacture][i];
			oi0 = RefIndices[3 * nTri + 0],
				oi1 = RefIndices[3 * nTri + 1],
				oi2 = RefIndices[3 * nTri + 2];
			vtx0 = RefVertices[oi0],
				vtx1 = RefVertices[oi1],
				vtx2 = RefVertices[oi2];
			n0 = vtx0.nFacture,
				n1 = vtx1.nFacture,
				n2 = vtx2.nFacture;
			dwMappingCode = MappingCodes[oi0];
			mcavtxh0.oi = oi0, mcavtxh0.dwMappingCode = dwMappingCode,
				mcavtxh0.Alpha = (n0 != nFacture ? true:false),
				mcavtxh1.oi = oi1, mcavtxh1.dwMappingCode = dwMappingCode,
				mcavtxh1.Alpha = (n1 != nFacture ? true:false),
				mcavtxh2.oi = oi2, mcavtxh2.dwMappingCode = dwMappingCode,
				mcavtxh2.Alpha = (n2 != nFacture ? true:false);
			i0 = MeshVerticesHash.find(mcavtxh0),
				i1 = MeshVerticesHash.find(mcavtxh1),
				i2 = MeshVerticesHash.find(mcavtxh2);
			if(i0 == NO_ELEMENT)
			{
				i0 = MeshVertices[nFacture].GetAmount();
				v3dPos.x = vtx0.x,
					v3dPos.y = vtx0.y,
					v3dPos.z = vtx0.z;
				v3dNormal.x = vtx0.nx,
					v3dNormal.y = vtx0.ny,
					v3dNormal.z = vtx0.nz;
				uv = pBaseMapper->GetMappingUV(v3dPos, v3dNormal, dwMappingCode);
				UV[nFacture].Add(uv);
				MeshVertices[nFacture].Add(vtx0);
				AlphaMeshVertices[nFacture].Add(mcavtxh0.Alpha);
				mcavtxh0.i = i0;
				MeshVerticesHash.add(mcavtxh0);
			}
			else
			{
				mcavtxh0 = MeshVerticesHash.elem(i0);
				i0 = mcavtxh0.i;
			}
			if(i1 == NO_ELEMENT)
			{
				i1 = MeshVertices[nFacture].GetAmount();
				v3dPos.x = vtx1.x,
					v3dPos.y = vtx1.y,
					v3dPos.z = vtx1.z;
				v3dNormal.x = vtx1.nx,
					v3dNormal.y = vtx1.ny,
					v3dNormal.z = vtx1.nz;
				uv = pBaseMapper->GetMappingUV(v3dPos, v3dNormal, dwMappingCode);
				UV[nFacture].Add(uv);
				MeshVertices[nFacture].Add(vtx1);
				AlphaMeshVertices[nFacture].Add(mcavtxh1.Alpha);
				mcavtxh1.i = i1;
				MeshVerticesHash.add(mcavtxh1);
			}
			else
			{
				mcavtxh1 = MeshVerticesHash.elem(i1);
				i1 = mcavtxh1.i;
			}
			if(i2 == NO_ELEMENT)
			{
				i2 = MeshVertices[nFacture].GetAmount();
				v3dPos.x = vtx2.x,
					v3dPos.y = vtx2.y,
					v3dPos.z = vtx2.z;
				v3dNormal.x = vtx2.nx,
					v3dNormal.y = vtx2.ny,
					v3dNormal.z = vtx2.nz;
				uv = pBaseMapper->GetMappingUV(v3dPos, v3dNormal, dwMappingCode);
				UV[nFacture].Add(uv);
				MeshVertices[nFacture].Add(vtx2);
				AlphaMeshVertices[nFacture].Add(mcavtxh2.Alpha);
				mcavtxh2.i = i2;
				MeshVerticesHash.add(mcavtxh2);
			}
			else
			{
				mcavtxh2 = MeshVerticesHash.elem(i2);
				i2 = mcavtxh2.i;
			}
			MeshIndices[nFacture].Add(i0);
			MeshIndices[nFacture].Add(i1);
			MeshIndices[nFacture].Add(i2);
		}
		MeshVerticesHash.reset();
		pBaseMapper = NULL;
	}

	// ------------------------- CREATING ------------------------------
	// base meshes using MeshVertices, MeshIndices, UV and AlphaVertices
	BaseMesh *pBaseMesh = NULL;
	VERTEX *pv2tData = NULL, *pv2t = NULL;
	word *pIndices = NULL;
	int nTriMax = 0, NParts = 0, nTriInCurBaseMesh = 0, nVtxInCurBaseMesh = 0;
	Hash<OneMapVertexHash> BaseMeshVerticesHash;
	for(nFacture = 0; nFacture < NFactures; nFacture++)
	{
		nTriMax = MeshIndices[nFacture].GetAmount() / 3;
		if(nTriMax == 0) continue;
		NParts = nTriMax / 0x5555;
		if(NParts * 0x5555 < nTriMax) NParts++;
		for(nTri = 0; nTri < nTriMax; nTri++)
		{
			oi0 = MeshIndices[nFacture][3 * nTri + 0],
				oi1 = MeshIndices[nFacture][3 * nTri + 1],
				oi2 = MeshIndices[nFacture][3 * nTri + 2];
			vtxh0.oi = oi0,
				vtxh1.oi = oi1,
				vtxh2.oi = oi2;
			if(nTriInCurBaseMesh == 0x5555)	// This BaseMesh is full! It's time to finish it...
			{
				pBaseMesh->setNVert(nVtxInCurBaseMesh);
				pBaseMesh->setNInd(0xFFFF);
				pBaseMesh->setNPri(0x5555);
				pBaseMesh->setTexture(OneMapVertex::GetFactureID(nFacture));
				pBaseMesh->setTexture(OneMapVertex::GetFactureID2(nFacture),1);
				pBaseMesh->setShader(OneMapVertex::GetShader(nFacture));
				pBaseMeshes.Add(pBaseMesh);
				pBaseMesh = NULL;
				pv2tData = NULL;
				pv2t = NULL;
				pIndices = NULL;
				BaseMeshVerticesHash.reset();
				nTriInCurBaseMesh = 0;
				nVtxInCurBaseMesh = 0;
			}
			if(nTriInCurBaseMesh == 0)	// And now it's time to start new BaseMesh!
			{
				pBaseMesh = new BaseMesh();
				if(pBaseMesh == NULL) return false;
				pBaseMesh->create(0xFFFF, 0xFFFF, vfVertexTS);
				pv2tData = (VERTEX*)pBaseMesh->getVertexData();
				if(pv2tData == NULL) return false;
				pIndices = pBaseMesh->getIndices();
				if(pIndices == NULL) return false;
				pv2t = pv2tData;
			}
			i0 = BaseMeshVerticesHash.find(vtxh0),
				i1 = BaseMeshVerticesHash.find(vtxh1),
				i2 = BaseMeshVerticesHash.find(vtxh2);
			if(i0 == NO_ELEMENT)
			{
				i0 = nVtxInCurBaseMesh;
				nVtxInCurBaseMesh++;
				pv2t->x = MeshVertices[nFacture][oi0].x,
					pv2t->y = MeshVertices[nFacture][oi0].y,
					pv2t->z = MeshVertices[nFacture][oi0].z;
				pv2t->diffuse = MeshVertices[nFacture][oi0].GetDiffuseColor();
				pv2t->specular = MeshVertices[nFacture][oi0].GetSpecularColor();
				if(AlphaMeshVertices[nFacture][oi0] == true) pv2t->diffuse &= 0x00FFFFFF;
				pv2t->u = UV[nFacture][oi0].u,
					pv2t->v = UV[nFacture][oi0].v;
				pv2t++;
				vtxh0.i = i0;
				BaseMeshVerticesHash.add(vtxh0);
			}
			else
			{
				vtxh0 = BaseMeshVerticesHash.elem(i0);
				i0 = vtxh0.i;
			}
			if(i1 == NO_ELEMENT)
			{
				i1 = nVtxInCurBaseMesh;
				nVtxInCurBaseMesh++;
				pv2t->x = MeshVertices[nFacture][oi1].x,
					pv2t->y = MeshVertices[nFacture][oi1].y,
					pv2t->z = MeshVertices[nFacture][oi1].z;
				pv2t->diffuse = MeshVertices[nFacture][oi1].GetDiffuseColor();
				pv2t->specular = MeshVertices[nFacture][oi1].GetSpecularColor();
				if(AlphaMeshVertices[nFacture][oi1] == true) pv2t->diffuse &= 0x00FFFFFF;
				pv2t->u = UV[nFacture][oi1].u,
					pv2t->v = UV[nFacture][oi1].v;
				pv2t++;
				vtxh1.i = i1;
				BaseMeshVerticesHash.add(vtxh1);
			}
			else
			{
				vtxh1 = BaseMeshVerticesHash.elem(i1);
				i1 = vtxh1.i;
			}
			if(i2 == NO_ELEMENT)
			{
				i2 = nVtxInCurBaseMesh;
				nVtxInCurBaseMesh++;
				pv2t->x = MeshVertices[nFacture][oi2].x,
					pv2t->y = MeshVertices[nFacture][oi2].y,
					pv2t->z = MeshVertices[nFacture][oi2].z;
				pv2t->diffuse = MeshVertices[nFacture][oi2].GetDiffuseColor();
				pv2t->specular = MeshVertices[nFacture][oi2].GetSpecularColor();
				if(AlphaMeshVertices[nFacture][oi2] == true) pv2t->diffuse &= 0x00FFFFFF;
				pv2t->u = UV[nFacture][oi2].u,
					pv2t->v = UV[nFacture][oi2].v;
				pv2t++;
				vtxh2.i = i2;
				BaseMeshVerticesHash.add(vtxh2);
			}
			else
			{
				vtxh2 = BaseMeshVerticesHash.elem(i2);
				i2 = vtxh2.i;
			}
			*pIndices = i0;
			pIndices++;
			*pIndices = i1;
			pIndices++;
			*pIndices = i2;
			pIndices++;
			nTriInCurBaseMesh++;
		}
		// if there is allocated BaseMesh we should to finalize it!
		if(pBaseMesh != NULL)
		{
			pBaseMesh->setNVert(nVtxInCurBaseMesh);
			pBaseMesh->setNInd(3 * nTriInCurBaseMesh);
			pBaseMesh->setNPri(nTriInCurBaseMesh);
			pBaseMesh->setTexture(OneMapVertex::GetFactureID(nFacture));
			pBaseMesh->setTexture(OneMapVertex::GetFactureID2(nFacture),1);
			pBaseMesh->setShader(OneMapVertex::GetShader(nFacture));
			pBaseMeshes.Add(pBaseMesh);
			pBaseMesh = NULL;
			pv2tData = NULL;
			pv2t = NULL;
			pIndices = NULL;
			BaseMeshVerticesHash.reset();
			nTriInCurBaseMesh = 0;
			nVtxInCurBaseMesh = 0;
		}
	}

	pBaseMapper = NULL;
	MappingCodes.Clear();
	for(nFacture = 0; nFacture < NFactures; nFacture++)
		UV[nFacture].Clear(),
		MeshVertices[nFacture].Clear(),
		AlphaMeshVertices[nFacture].Clear(),
		MeshIndices[nFacture].Clear(),
		snNotTransitiveFactureTriangles[nFacture].Clear(),
		snTransitiveFactureTriangles[nFacture].Clear();
	RefVertices.Clear();
	RefIndices.Clear();
	pVBMesh = NULL;

	return true;
}

// ================================================================================
// OneSplitMesh::Draw
// ================================================================================
void OneSplitMesh::Draw(void)
{
	IRS->ResetWorldTM();
	int N = pBaseMeshes.GetAmount();
	static int wID = IRS->GetTextureID( "white.bmp" );

	extern float g_ZNear, g_TerrainZBias; 
	float bias = tmin( g_TerrainZBias, g_ZNear - 10.0f );
	ICam->ShiftZ( -bias );
	ICam->Render();

	for(int i = 0; i < N; i++)
	{	
		pBaseMeshes[i]->setTexture( wID, 1 );
		DrawBM(*pBaseMeshes[i]);
	}

	ICam->ShiftZ( bias );
	ICam->Render();
}

// ================================================================================
// VertexBasedMesh::~VertexBasedMesh
// ================================================================================
VertexBasedMesh::~VertexBasedMesh()
{ Release(); }

// ================================================================================
// VertexBasedMesh::Release()
// ================================================================================
void VertexBasedMesh::Release()
{
	Vert.Clear();
	Idxs.Clear();
}

// ================================================================================
// VertexBasedMesh::CreateHexagonalPlane
// ================================================================================
void VertexBasedMesh::CreateHexagonalPlane(Vector3D v3dOrigin, int iSx, int iSy,
										   int Dx, int Dy, DWORD dwColor, byte nFacture, byte Shadow)
{
	// uninitialization
	Release();

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
}

// ================================================================================
// VertexBasedMesh::CreateBezierHexagonalPlane
// ================================================================================
void VertexBasedMesh::CreateBezierHexagonalPlane(Vector3D v3dOrigin, int Sx, int Sy,
												 int Dx, int Dy, DWORD dwColor, byte nFacture, byte Shadow,
												 int z00,	int z01,	int z02,	int z03,
												 int z10,	int z11,	int z12,	int z13,
												 int z20,	int z21,	int z22,	int z23,
												 int z30,	int z31,	int z32,	int z33)
{
	this->CreateHexagonalPlane(v3dOrigin, Sx, Sy, Dx, Dy, dwColor, nFacture, Shadow);

	// 00 - 03
	bezierZTemplate[0][0] = z00;
	bezierZTemplate[0][1] = z01;
	bezierZTemplate[0][2] = z02;
	bezierZTemplate[0][3] = z03;
	// 10 - 13
	bezierZTemplate[1][0] = z10;
	bezierZTemplate[1][1] = z11;
	bezierZTemplate[1][2] = z12;
	bezierZTemplate[1][3] = z13;
	// 20 - 23
	bezierZTemplate[2][0] = z20;
	bezierZTemplate[2][1] = z21;
	bezierZTemplate[2][2] = z22;
	bezierZTemplate[2][3] = z23;
	// 30 - 33
	bezierZTemplate[3][0] = z30;
	bezierZTemplate[3][1] = z31;
	bezierZTemplate[3][2] = z32;
	bezierZTemplate[3][3] = z33;

	float dS = 0, dT = 0,
		sx = 1.0F / (float)Sx, sy = 1.0F / (float)Sy;
	int Z = 0;
	for(int i = 0; i < Vert.GetAmount(); i++)
	{
		dS = sy * (Vert[i].y - v3dOrigin.y);
		dT = sx * (Vert[i].x - v3dOrigin.x);
		beziersurfaceCalcVertex(dS, dT, &Z);
		Vert[i].z += Z;
	}
}

// ================================================================================
// VertexBasedMesh::beziersurfaceCalcVertex
// ================================================================================
void VertexBasedMesh::beziersurfaceCalcVertex(float dS, float dT, int *pZ)
{
	int C[4] = {1, 3, 3, 1};
	float z = 0, k = 0;

	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
		{
			k = C[i] * pow(dS, i) * pow(1 - dS, 3 - i) *
				C[j] * pow(dT, j) * pow(1 - dT, 3 - j);
			z += k * bezierZTemplate[i][j];		
		}
		*pZ = (int)z;
}

// ================================================================================
// VertexBasedMesh::CalcNormals
// ================================================================================
void VertexBasedMesh::CalcNormals(void)
{
	DynArray<Vector3D> TriNormals, VtxNormals;

	for(int i = 0; i < Vert.NValues; i++) VtxNormals.Add(Vector3D::null);
	// Calculating normals in triangles:
	int NTri = Idxs.NValues / 3;
	for(int nTri = 0; nTri < NTri; nTri++)
	{
		int i0 = Idxs[3 * nTri + 0], i1 = Idxs[3 * nTri + 1], i2 = Idxs[3 * nTri + 2];
		OneMapVertex v0 = Vert[i0], v1 = Vert[i1], v2 = Vert[i2];
		v1.x -= v0.x;
		v1.y -= v0.y;
		v1.z -= v0.z;
		v2.x -= v0.x;
		v2.y -= v0.y;
		v2.z -= v0.z;
		Vector3D Normal(v2.y * v1.z - v1.y * v2.z, v1.x * v2.z - v2.x * v1.z, v2.x * v1.y - v1.x * v2.y);
		Normal.normalize();
		TriNormals.Add(Normal);
	}
	// Calculating normals in vertices:
	for(i = 0; i < Idxs.NValues; i++)
	{
		nTri = i / 3;
		Vector3D &rTriNormal = TriNormals[nTri], &rVtxNormal = VtxNormals[Idxs[i]];
		rVtxNormal.x += rTriNormal.x;
		rVtxNormal.y += rTriNormal.y;
		rVtxNormal.z += rTriNormal.z;
	}
	// Normalizing and storing normals to Vert:
	for(i = 0; i < Vert.NValues; i++)
	{
		Vector3D &rNormal = VtxNormals[i];
		rNormal.normalize();
		OneMapVertex &rVertex = Vert[i];
		rVertex.nx = 127 * rNormal.x, rVertex.ny = 127 * rNormal.y, rVertex.nz = 127 * rNormal.z;
	}
	// unInit:
	TriNormals.Clear(), VtxNormals.Clear();
}

// ================================================================================
// VertexBasedMesh::CalcShadow
// ================================================================================
/*void VertexBasedMesh::CalcShadow(Vector3D v3dSun, float k, DWORD minLevel, DWORD maxLevel)
{
	float norm = 0;
	norm = v3dSun.norm();
	v3dSun.x /= norm;
	v3dSun.y /= norm;
	v3dSun.z /= norm;
	int nvtx = 0, nvtxMax = 0;
	float nx = 0, ny = 0, nz = 0, l = 0, K = 0;
	DWORD clr = 0, R = 0, G = 0, B = 0, A = 0;
	nvtxMax = Vert.GetAmount();
	for(nvtx = 0; nvtx < nvtxMax; nvtx++)
	{
		nx = float(Vert[nvtx].nx) / 127.0f;
		ny = float(Vert[nvtx].ny) / 127.0f;
		nz = float(Vert[nvtx].nz) / 127.0f;
		l = v3dSun.x * nx + v3dSun.y * ny + v3dSun.z * nz;
		if(l < 0) l = 0;
		clr = Vert[nvtx].Color;
		R = clr & 0xFF;
		clr >>= 8;
		G = clr & 0xFF;
		clr >>= 8;
		B = clr & 0xFF;
		clr >>= 8;
		A = clr & 0xFF;
		K = k + (1 - k) * l;
		R = DWORD(K * float(R));
		G = DWORD(K * float(G));
		B = DWORD(K * float(B));
		if(R < minLevel) R = minLevel;
		else if(R > maxLevel) R = maxLevel;
		if(G < minLevel) G = minLevel;
		else if(G > maxLevel) G = maxLevel;
		if(B < minLevel) B = minLevel;
		else if(B > maxLevel) B = maxLevel;
//		Vert[nvtx].DiffuseColor = R + (G << 8) + (B << 16) + (A << 24);
	}
}*/

// ================================================================================
// UniversalMap::UniversalMap
// ================================================================================
UniversalMap::~UniversalMap(void)
{
	Release();
}

// ================================================================================
// UniversalMap::Copy
// ================================================================================
void UniversalMap::Copy(const UniversalMap *pUniversalMap)
{
	Release();
	Vert.AddValues(pUniversalMap->Vert.GetValues(), pUniversalMap->Vert.NValues);
	Idxs.AddValues(pUniversalMap->Idxs.GetValues(), pUniversalMap->Idxs.NValues);
	for(int nQ = 0; nQ < pUniversalMap->m_pQuads.NValues; nQ++)
	{
		m_pQuads.Add(new OneUniversalMapQuad);
		m_pQuads[m_pQuads.NValues - 1]->nTriangles.AddValues(pUniversalMap->m_pQuads[nQ]->nTriangles.GetValues(),
			pUniversalMap->m_pQuads[nQ]->nTriangles.NValues);
		m_pQuads[m_pQuads.NValues - 1]->pOrigin = this;
	}
	QuadX0 = pUniversalMap->QuadX0, QuadY0 = pUniversalMap->QuadY0,
		QuadLx = pUniversalMap->QuadLx, QuadLy = pUniversalMap->QuadLy,
		QuadsNx = pUniversalMap->QuadsNx, QuadsNy = pUniversalMap->QuadsNy,
		UMLx = pUniversalMap->UMLx, UMLy = pUniversalMap->UMLy;
} // UniversalMap::Copy

// ================================================================================
// UniversalMap::Release()
// ================================================================================
void UniversalMap::Release(void)
{
	Vert.Clear();
	Idxs.Clear();
	QuadsSystem_unInit();
	bWire = false;
}

// ================================================================================
// zXTri
// ================================================================================
bool zXTri(float *Z, const Vector3D &v3dTriA, const Vector3D &v3dTriB,
		   const Vector3D &v3dTriC)
{
	Vector3D v3d[3];
	v3d[0] = v3dTriA, v3d[1] = v3dTriB, v3d[2] = v3dTriC;
	int i, nhor, next, xmin, xmax, ymin, ymax;
	float x[4], z[4], y1, y2;
	xmin = xmax = v3d[0].x;
	ymin = ymax = v3d[0].y;
	for(i = 1; i < 3; i++)
	{
		if(xmin > v3d[i].x) xmin = v3d[i].x;
		if(ymin > v3d[i].y) ymin = v3d[i].y;
		if(xmax < v3d[i].x) xmax = v3d[i].x;
		if(ymax < v3d[i].y) ymax = v3d[i].y;
	}
	if(xmin == 0) v3d[0].x--, v3d[1].x--, v3d[2].x--, xmin--, xmax--;
	if(ymin == 0) v3d[0].y--, v3d[1].y--, v3d[2].y--, ymin--, ymax--;
	if(xmax == 0) v3d[0].x++, v3d[1].x++, v3d[2].x++, xmax++, xmin++;
	if(ymax == 0) v3d[0].y++, v3d[1].y++, v3d[2].y++, ymax++, ymin++;
	if((xmin > 0) || (xmax < 0) || (ymin > 0) || (ymax < 0)) return false;
	nhor = 0;
	for(i = 0; i < 3; i++)
	{
		next = i + 1;
		if(next >= 3) next = 0;
		y1 = v3d[i].y;
		y2 = v3d[next].y;
		if((0 >= y1) && (0 < y2) || (0 <= y1) && (0 > y2))
		{
			x[nhor] = v3d[i].x - (v3d[next].x - v3d[i].x) *	y1 / (y2 - y1);
			z[nhor] = v3d[i].z - (v3d[next].z - v3d[i].z) *	y1 / (y2 - y1);
			nhor++;
		}
	}
	if(nhor != 2) return false;
	if(x[1] == x[0])
	{
		if(x[1] == 0)
		{
			*Z = 0.5 * (z[0] + z[1]);
			return true;
		}
		return false;
	}
	if(((x[0] <= 0) && (x[1] >= 0)) ||
		((x[0] >= 0) && (x[1] <= 0)))
	{
		*Z = z[0] - x[0] * (z[1] - z[0]) / (x[1] - x[0]);
		return true;
	}
	return false;
}

CSun g_Sun(0.4f, 40, 160, Vector3D(10, 3, -63), Vector3D(14, 5, -20),
		   Vector3D(2, 19, -22), Vector3D(8, 16, -64), Vector3D(15, 2, -50),
		   Vector3D(3, -12, -16), Vector3D(9, 0, -60), Vector3D(21, 18, -57),
		   Vector3D(0, 19, -56), Vector3D(23, -8, -31), Vector3D(135, 51, 59),
		   Vector3D(110, 38, 64), Vector3D(127, 61, 64), Vector3D(138, 77, 64),
		   Vector3D(109, 48, 68), Vector3D(141, 61, 62), Vector3D(132, 69, 59),
		   Vector3D(111, 85, 65), Vector3D(102, 66, 59), Vector3D(134, 45, 59), Vector3D::null);
UniversalMap g_UniversalMapBackup;
UniversalMap g_UniversalMap(&g_Sun, &g_UniversalMapBackup);

//CUniversalMapClipboard g_UniversalMapClipboard;

// ================================================================================
// UniversalMap::SupplementMapOfHeightsInRect
// ================================================================================
void UniversalMap::SupplementMapOfHeightsInRect(int x0, int y0, int x1, int y1)
{
	float AllZ[4] = {0, 0, 0, 0}, Z = 0;
	const float cNotIntersected = -9999.99f;
	OneUniversalMapQuad *pQuads[4];
	for(int ny = 0; ny < MaxTH; ny++)
		for(int nx = 0; nx < VertInLine; nx++)
		{
			int x = nx << 5, y = ny << 5, T = (nx >> 1) << 1;
			if(T != nx) y -= 16;
			if(x >= x0 && x <= x1 && y >= y0 && y <= y1)
			{
				int nQx = (x - QuadX0) / QuadLx, nQy = (y - QuadY0) / QuadLy;
				ZeroMemory(pQuads, sizeof(pQuads));
				pQuads[0] = m_pQuads[nQx + QuadsNx * nQy];
				// Determining in which subquad hits (x, y):
				int X = nQx * QuadLx + QuadX0, Y = nQy * QuadLy + QuadY0;
				int dx = x - X, dy = y - Y;
				bool bRightSubQuad = false, bTopSubQuad = false;
				if(dx < (QuadLx >> 1)) bRightSubQuad = false;
				else bRightSubQuad = true;
				if(dy < (QuadLy >> 1)) bTopSubQuad = true;
				else bTopSubQuad = false;
				// 0 1
				// 2 3
				int nSubQuad = 0;
				if((bRightSubQuad == false) && (bTopSubQuad == true)) nSubQuad = 0;
				if((bRightSubQuad == false) && (bTopSubQuad == false)) nSubQuad = 2;
				if((bRightSubQuad == true) && (bTopSubQuad == true)) nSubQuad = 1;
				if((bRightSubQuad == true) && (bTopSubQuad == false)) nSubQuad = 3;
				// based on nSubQuad filling pQuads
				// (array of potential quads for intersection)
				if(bRightSubQuad == false) // to the left
				{ if(nQx >= 1) pQuads[1] = m_pQuads[(nQx - 1) + QuadsNx * nQy]; }
				else // to the right
				{ if(nQx < QuadsNx - 1) pQuads[1] = m_pQuads[(nQx + 1) + QuadsNx * nQy]; }
				if(bTopSubQuad == true) // at the top
				{ if(nQy >= 1) pQuads[2] = m_pQuads[nQx + QuadsNx * (nQy - 1)]; }
				else // at the bottom
				{ if(nQy < QuadsNy - 1) pQuads[2] = m_pQuads[nQx + QuadsNx * (nQy + 1)]; }
				if((pQuads[1] != NULL) && (pQuads[2] != NULL)) // there is a diagonal quad
				{
					switch(nSubQuad)
					{
					case 0:
						pQuads[3] = m_pQuads[(nQx - 1) + QuadsNx * (nQy - 1)];
					break;
					case 1:
						pQuads[3] = m_pQuads[(nQx + 1) + QuadsNx * (nQy - 1)];
						break;
					case 2:
						pQuads[3] = m_pQuads[(nQx - 1) + QuadsNx * (nQy + 1)];
						break;
					case 3:
						pQuads[3] = m_pQuads[(nQx + 1) + QuadsNx * (nQy + 1)];
						break;
					default: break;
					}
				}
				// Testing for intersections:
				AllZ[0] = AllZ[1] = AllZ[2] = AllZ[3] = cNotIntersected;
					for(int i = 0; i < 4; i++)
					{
						if(pQuads[i] != NULL)
						{
							for(int n = 0; n < pQuads[i]->nTriangles.NValues; n++)
							{
								int nTri = pQuads[i]->nTriangles[n];
								int i0 = Idxs[3 * nTri + 0], i1 = Idxs[3 * nTri + 1], i2 = Idxs[3 * nTri + 2];
								OneMapVertex vtx0 = Vert[i0], vtx1 = Vert[i1], vtx2 = Vert[i2];
								Vector3D v3dA, v3dB, v3dC;
								// A
								v3dA.x = vtx0.x - x,
									v3dA.y = vtx0.y - y,
									v3dA.z = vtx0.z;
								// B
								v3dB.x = vtx1.x - x,
									v3dB.y = vtx1.y - y,
									v3dB.z = vtx1.z;
								// C
								v3dC.x = vtx2.x - x,
									v3dC.y = vtx2.y - y,
									v3dC.z = vtx2.z;
								bool bX = zXTri(&Z, v3dA, v3dB, v3dC);
								if(bX == true) if(Z > AllZ[i]) AllZ[i] = Z;
							}
							if(AllZ[i] > cNotIntersected)
							{
								THMap[nx + VertInLine * ny] = (short)AllZ[i];
								break;
							}
						}
					}
			}
		}
} // UniversalMap::SupplementMapOfHeightsInRect

// ================================================================================
// UniversalMap::FillMapOfHeights
// ================================================================================
void UniversalMap::FillMapOfHeights(void)
{
	ZeroMemory(THMap, sizeof(short) * VertInLine * MaxTH);
	int i = 0, nx = 0, ny = 0, x = 0, y = 0, T = 0,
		nqx = 0, nqy = 0, UMLx = 0, UMLy = 0;
	UMLx = QuadLx * QuadsNx,
		UMLy = QuadLy * QuadsNy;
	OneUniversalMapQuad *pQuads[4] = {NULL, NULL, NULL, NULL};
	float AllZ[4] = {0, 0, 0, 0}, Z = 0;
	const float cNotIntersected = -9999.99f;
	bool bX = false;
	int nTri = 0, NTriMax = 0, i0 = 0, i1 = 0, i2 = 0, n = 0;
	Vector3D v3dA, v3dB, v3dC;
	OneMapVertex vtx0, vtx1, vtx2;
	for(ny = 0; ny < MaxTH; ny++)
		for(nx = 0; nx < VertInLine; nx++)
		{
			x = nx << 5, y = ny << 5;
			T = (nx >> 1) << 1;
			if(T != nx) y -= 16;
			if((x >= QuadX0) && (x < QuadX0 + UMLx) &&
				(y >= QuadY0) && (y < QuadY0 + UMLy))
			{
				nqx = (x - QuadX0) / QuadLx,
					nqy = (y - QuadY0) / QuadLy;
				ZeroMemory(pQuads, sizeof(pQuads));
				pQuads[0] = m_pQuads[nqx + QuadsNx * nqy];
				// determining in which subquad hits x, y
				int X = 0, Y = 0;
				X = nqx * QuadLx + QuadX0,
					Y = nqy * QuadLy + QuadY0;
				int dx = 0, dy = 0;
				dx = x - X, dy = y - Y;
				bool bRightSubQuad = false, bTopSubQuad = false;
				if(dx < (QuadLx >> 1)) bRightSubQuad = false;
				else bRightSubQuad = true;
				if(dy < (QuadLy >> 1)) bTopSubQuad = true;
				else bTopSubQuad = false;
				// 0 1
				// 2 3
				int nSubQuad = 0;
				if((bRightSubQuad == false) && (bTopSubQuad == true)) nSubQuad = 0;
				if((bRightSubQuad == false) && (bTopSubQuad == false)) nSubQuad = 2;
				if((bRightSubQuad == true) && (bTopSubQuad == true)) nSubQuad = 1;
				if((bRightSubQuad == true) && (bTopSubQuad == false)) nSubQuad = 3;
				// based on nSubQuad filling pQuads
				// (array of potential quads for intersection)
				if(bRightSubQuad == false) // to the left
				{ if(nqx >= 1) pQuads[1] = m_pQuads[(nqx - 1) + QuadsNx * nqy]; }
				else // to the right
				{ if(nqx < QuadsNx - 1) pQuads[1] = m_pQuads[(nqx + 1) + QuadsNx * nqy]; }
				if(bTopSubQuad == true) // at the top
				{ if(nqy >= 1) pQuads[2] = m_pQuads[nqx + QuadsNx * (nqy - 1)]; }
				else // at the bottom
				{ if(nqy < QuadsNy - 1) pQuads[2] = m_pQuads[nqx + QuadsNx * (nqy + 1)]; }
				if((pQuads[1] != NULL) && (pQuads[2] != NULL)) // there is a diagonal quad
				{
					switch(nSubQuad)
					{
					case 0:
						pQuads[3] = m_pQuads[(nqx - 1) + QuadsNx * (nqy - 1)];
						break;
					case 1:
						pQuads[3] = m_pQuads[(nqx + 1) + QuadsNx * (nqy - 1)];
						break;
					case 2:
						pQuads[3] = m_pQuads[(nqx - 1) + QuadsNx * (nqy + 1)];
						break;
					case 3:
						pQuads[3] = m_pQuads[(nqx + 1) + QuadsNx * (nqy + 1)];
						break;
					default: break;
					}
				}
				// Testing for intersection...
				AllZ[0] = AllZ[1] = AllZ[2] = AllZ[3] = cNotIntersected;
				for(i = 0; i < 4; i++)
				{
					if(pQuads[i] != NULL)
					{

						NTriMax = pQuads[i]->nTriangles.GetAmount();
						for(n = 0; n < NTriMax; n++)
						{
							nTri = pQuads[i]->nTriangles[n];
							i0 = Idxs[3 * nTri + 0],
								i1 = Idxs[3 * nTri + 1],
								i2 = Idxs[3 * nTri + 2];
							vtx0 = Vert[i0],
								vtx1 = Vert[i1],
								vtx2 = Vert[i2];
							// A
							v3dA.x = vtx0.x - x,
								v3dA.y = vtx0.y - y,
								v3dA.z = vtx0.z;
							// B
							v3dB.x = vtx1.x - x,
								v3dB.y = vtx1.y - y,
								v3dB.z = vtx1.z;
							// C
							v3dC.x = vtx2.x - x,
								v3dC.y = vtx2.y - y,
								v3dC.z = vtx2.z;
							bX = zXTri(&Z, v3dA, v3dB, v3dC);
							if(bX == true) if(Z > AllZ[i]) AllZ[i] = Z;
						}
						if(AllZ[i] > cNotIntersected)
						{
							THMap[nx + VertInLine * ny] = (short)AllZ[i];
							break;
						}
					}
				}
			}
		}
}


// ================================================================================
// UniversalMap::SetFactureInRadius
// ================================================================================
void UniversalMap::SetFactureInRadius(int cx, int cy, int r, int nFacture)
{
	VertexBasedMeshRef VBMRef;
	GetMeshRefCylinder(cx, cy, r, &VBMRef);
	DynArray<OneMapVertex*> pVertices;
	VBMRef.ExtractUniqueVertices(&pVertices);
	// Enumerating all vertices and setting nFacture
	int N = pVertices.GetAmount();
	if(N == 0) return;
	int r2 = 0;
	r2 = r * r;
	for(int i = 0; i < N; i++)
	{
		int GetFactureUsage(int);
		const int min_PlanarMapping_nz  = 70,
			max_VerticalMapping_nz = 110;
		int nUsage = 0, d2 = 0;
		nUsage = GetFactureUsage(nFacture);
		d2 = (cx - pVertices[i]->x) * (cx - pVertices[i]->x) +
			(cy - pVertices[i]->y) * (cy - pVertices[i]->y);
		if(d2 < r2)
		{
			if((nUsage == 0) && (pVertices[i]->nz >= min_PlanarMapping_nz))
				pVertices[i]->nFacture = nFacture;
			if((nUsage == 1) && (pVertices[i]->nz <= max_VerticalMapping_nz))
				pVertices[i]->nFacture = nFacture;
		}
	}
	pVertices.Clear();
	// Filling bound rectangle and invalidating geometry
	Rct rc(VBMRef.x0, VBMRef.y0, VBMRef.w, VBMRef.h);
	ITerra->InvalidateGeometry(&rc);
}

// ================================================================================
// UniversalMap::SetColorInRadius
// ================================================================================
void UniversalMap::SetColorInRadius(int cx, int cy, int r, int d, DWORD Color)
{
	//              |   d   |
	// ----------------------
	// |          r         |
	// |____________________|
	if((r <= 0) || (d >= r) || (d <= 0)) return;
	VertexBasedMeshRef VBMRef;
	GetMeshRefCylinder(cx, cy, r, &VBMRef);
	DynArray<OneMapVertex*> pVertices;
	VBMRef.ExtractUniqueVertices(&pVertices);
	// Enumerating all vertices and changing colors
	int N = pVertices.GetAmount();
	if(N == 0) return;
	int r_d = 0;
	r_d = r - d;
	for(int i = 0; i < N; i++)
	{
		float t = 0, fR = 0, fG = 0, fB = 0;
		int cR = 0, cG = 0, cB = 0, cr = 0, cg = 0, cb = 0, l = 0;
		const float cK = 1.0f;
		DWORD clr = 0;
		l = sqrt((cx - pVertices[i]->x) * (cx - pVertices[i]->x) +
			(cy - pVertices[i]->y) * (cy - pVertices[i]->y));
		if(l < r) // we should change color of this vertex
		{
			if(l <= r_d) pVertices[i]->Color = Color; // in this radius setting color without any gradient
			else
			{
				t = float(l - r_d) / (cK * float(d));
				if(t > cK) t = cK;
				// cR, cG, cB
				clr = pVertices[i]->Color;
				cB = clr & 0xFF;
				clr >>= 8;
				cG = clr & 0xFF;
				clr >>= 8;
				cR = clr & 0xFF;
				// cr, cg, cb
				clr = Color;
				cb = clr & 0xFF;
				clr >>= 8;
				cg = clr & 0xFF;
				clr >>= 8;
				cr = clr & 0xFF;
				fR = float(cr) + float(cR - cr) * t;
				fG = float(cg) + float(cG - cg) * t;
				fB = float(cb) + float(cB - cb) * t;
				cR = (int)fR;
				cG = (int)fG;
				cB = (int)fB;
				pVertices[i]->Color = cB + (cG << 8) + (cR << 16) + 0xFF000000;
			}
		}
	}
	pVertices.Clear();
	// Filling bound rectangle and invalidating geometry
	Rct rc(VBMRef.x0, VBMRef.y0, VBMRef.w, VBMRef.h);
	ITerra->InvalidateGeometry(&rc);
}
void UniversalMap::SetRandomColorsInRadius(int cx, int cy, int r, int d, DWORD* Colors,int NColors)
{
	if(!NColors)return;
	//              |   d   |
	// ----------------------
	// |          r         |
	// |____________________|
	if((r <= 0) || (d >= r) || (d <= 0)) return;
	VertexBasedMeshRef VBMRef;
	GetMeshRefCylinder(cx, cy, r, &VBMRef);
	DynArray<OneMapVertex*> pVertices;
	VBMRef.ExtractUniqueVertices(&pVertices);
	// Enumerating all vertices and changing colors
	int N = pVertices.GetAmount();
	if(N == 0) return;
	int r_d = 0;
	r_d = r - d;
	for(int i = 0; i < N; i++)
	{
		DWORD Color=Colors[rand()%NColors];
		float t = 0, fR = 0, fG = 0, fB = 0;
		int cR = 0, cG = 0, cB = 0, cr = 0, cg = 0, cb = 0, l = 0;
		const float cK = 1.0f;
		DWORD clr = 0;
		l = sqrt((cx - pVertices[i]->x) * (cx - pVertices[i]->x) +
			(cy - pVertices[i]->y) * (cy - pVertices[i]->y));
		if(l < r) // we should change color of this vertex
		{
			if(l <= r_d) pVertices[i]->Color = Color; // in this radius setting color without any gradient
			else
			{
				t = float(l - r_d) / (cK * float(d));
				if(t > cK) t = cK;
				// cR, cG, cB
				clr = pVertices[i]->Color;
				cB = clr & 0xFF;
				clr >>= 8;
				cG = clr & 0xFF;
				clr >>= 8;
				cR = clr & 0xFF;
				// cr, cg, cb
				clr = Color;
				cb = clr & 0xFF;
				clr >>= 8;
				cg = clr & 0xFF;
				clr >>= 8;
				cr = clr & 0xFF;
				fR = float(cr) + float(cR - cr) * t;
				fG = float(cg) + float(cG - cg) * t;
				fB = float(cb) + float(cB - cb) * t;
				cR = (int)fR;
				cG = (int)fG;
				cB = (int)fB;
				pVertices[i]->Color = cB + (cG << 8) + (cR << 16) + 0xFF000000;
			}
		}
	}
	pVertices.Clear();
	// Filling bound rectangle and invalidating geometry
	Rct rc(VBMRef.x0, VBMRef.y0, VBMRef.w, VBMRef.h);
	ITerra->InvalidateGeometry(&rc);
}

// ================================================================================
// UniversalMap::SmoothColors
// ================================================================================
void UniversalMap::SmoothColors(int cx, int cy, int r)
{
	if(r <= 0) return;
	VertexBasedMeshRef VBMRef;
	GetMeshRefCylinder(cx, cy, r, &VBMRef);
	// Smoothing
	const int K = 2, k = 1; // Parameters of smoothing
	int S = 0;
	S = K + (k << 1);
	int r2 = r * r;
	int N = VBMRef.nTriangles.GetAmount();
	if(N == 0) return;
	OneMapVertex *pvtx0 = NULL, *pvtx1 = NULL, *pvtx2 = NULL;
	int nTri = 0, i0 = 0, i1 = 0, i2 = 0, i = 0, X = 0, Y = 0, l2 = 0;
	for(i = 0; i < N; i++)
	{
		nTri = VBMRef.nTriangles[i];
		i0 = Idxs[3 * nTri + 0],
			i1 = Idxs[3 * nTri + 1],
			i2 = Idxs[3 * nTri + 2];
		pvtx0 = &Vert[i0],
			pvtx1 = &Vert[i1],
			pvtx2 = &Vert[i2];
		// Calculating XY center of nTri
		X = (pvtx0->x + pvtx1->x + pvtx2->x) / 3,
			Y = (pvtx0->y + pvtx1->y + pvtx2->y) / 3;
		// Calculating distance to center of nTri
		l2 = (cx - X) * (cx - X) + (cy - Y) * (cy - Y);
		if(l2 < r2) // Triangle is in center, so we should smooth colors of its vertices
		{
			int r0 = 0, g0 = 0, b0 = 0, r1 = 0, g1 = 0, b1 = 0, r2 = 0, g2 = 0, b2 = 0,
				R0 = 0, G0 = 0, B0 = 0, R1 = 0, G1 = 0, B1 = 0, R2 = 0, G2 = 0, B2 = 0;
			DWORD clr;
			//vtx0
			clr = pvtx0->Color;
			b0 = clr & 0xFF;
			clr >>= 8;
			g0 = clr & 0xFF;
			clr >>= 8;
			r0 = clr & 0xFF;
			// vtx1
			clr = pvtx1->Color;
			b1 = clr & 0xFF;
			clr >>= 8;
			g1 = clr & 0xFF;
			clr >>= 8;
			r1 = clr & 0xFF;
			// vtx2
			clr = pvtx2->Color;
			b2 = clr & 0xFF;
			clr >>= 8;
			g2 = clr & 0xFF;
			clr >>= 8;
			r2 = clr & 0xFF;
			// Total brightness
			int tr = 0, tg = 0, tb = 0;
			tr = r0 + r1 + r2,
				tg = g0 + g1 + g2,
				tb = b0 + b1 + b2;
			// Essentially colors smoothing
			R0 = (K * r0 + k * r1 + k * r2) / S,
				G0 = (K * g0 + k * g1 + k * g2) / S,
				B0 = (K * b0 + k * b1 + k * b2) / S;
			R1 = (K * r1 + k * r0 + k * r2) / S,
				G1 = (K * g1 + k * g0 + k * g2) / S,
				B1 = (K * b1 + k * b0 + k * b2) / S;
			R2 = (K * r2 + k * r0 + k * r1) / S,
				G2 = (K * g2 + k * g0 + k * g1) / S,
				B2 = (K * b2 + k * b0 + k * b1) / S;
			// Keeping color balance
			int TR = 0, TG = 0, TB = 0;
			TR = R0 + R1 + R2,
				TG = G0 + G1 + G2,
				TB = B0 + B1 + B2;
			int *pRColors[3] = {NULL, NULL, NULL},
				*pGColors[3] = {NULL, NULL, NULL},
				*pBColors[3] = {NULL, NULL, NULL};
			pRColors[0] = &R0,
				pRColors[1] = &R1,
				pRColors[2] = &R2;
			pGColors[0] = &G0,
				pGColors[1] = &G1,
				pGColors[2] = &G2;
			pBColors[0] = &B0,
				pBColors[1] = &B1,
				pBColors[2] = &B2;
			int TotalTimes = 0, nCur = 0;
			while(TotalTimes < 1000)
			{
				if((TR < tr) && (*pRColors[nCur] < 255))
				{
					(*pRColors[nCur])++;
					TR++;
				}
				if((TG < tg) && (*pGColors[nCur] < 255))
				{
					(*pGColors[nCur])++;
					TG++;
				}
				if((TB < tb) && (*pBColors[nCur] < 255))
				{
					(*pBColors[nCur])++;
					TB++;
				}
				nCur++;
				if(nCur >= 3) nCur = 0;
				TotalTimes++;
			}
			// Saving...
			pvtx0->Color = B0 + (G0 << 8) + (R0 << 16) + 0xFF000000;
			pvtx1->Color = B1 + (G1 << 8) + (R1 << 16) + 0xFF000000;
			pvtx2->Color = B2 + (G2 << 8) + (R2 << 16) + 0xFF000000;
		}
		pvtx0 = NULL,
			pvtx1 = NULL,
			pvtx2 = NULL;
	}
	// Filling bound rectangle and invalidating geometry
	Rct rc(VBMRef.x0, VBMRef.y0, VBMRef.w, VBMRef.h);
	ITerra->InvalidateGeometry(&rc);
}

// ================================================================================
// UniversalMap::CreateHollow
// ================================================================================
void UniversalMap::CreateHollow(int cx, int cy, int r, int depth)
{
	int QuadR = 0; // bound radius of quad
	QuadR = sqrt(((QuadLx * QuadLx) >> 2) + ((QuadLy * QuadLy) >> 2));
	int D = 0;
	if(r > depth) D = r + QuadR;
	else D = depth + QuadR;
	// extracting potential quads
	DynArray<OneUniversalMapQuad*> pQuads;
	int nqx = 0, nqy = 0, d = 0, nq = 0,
		X = 0, Y = 0; // center of quad
	for(nqy = 0; nqy < QuadsNy; nqy++)
		for(nqx = 0; nqx < QuadsNx; nqx++)
		{
			X = nqx * QuadLx + QuadX0 + (QuadLx >> 1),
				Y = nqy * QuadLy + QuadY0 + (QuadLy >> 1);
			d = sqrt((cx - X) * (cx - X) + (cy - Y) * (cy - Y));
			if(d < D)
			{
				nq = nqx + QuadsNx * nqy;
				pQuads.Add(m_pQuads[nq]);
			}
		}
		int N = 0;
		N = pQuads.GetAmount();
		if(N == 0) return;
		// merging all potential quads into one big reference
		VertexBasedMeshRef VBMeshRef;
		int i = 0;
		for(i = 0; i < N; i++)
			VBMeshRef.nTriangles.AddValues(pQuads[i]->nTriangles.GetValues(),
			pQuads[i]->nTriangles.GetAmount());
		VBMeshRef.pOrigin = pQuads[0]->pOrigin;
		// in which triangle we have hit?
		N = VBMeshRef.nTriangles.GetAmount();
		int nTri = 0, i0 = 0, i1 = 0, i2 = 0, nXTri = 0;
		VertexBasedMesh *pOrigin = NULL;
		OneMapVertex vtx0, vtx1, vtx2;
		Vector3D v3dA, v3dB, v3dC;
		bool bX = false;
		float Z = 0;
		pOrigin = VBMeshRef.pOrigin;
		for(i = 0; i < N; i++)
		{
			nTri = VBMeshRef.nTriangles[i];
			i0 = pOrigin->Idxs[3 * nTri + 0],
				i1 = pOrigin->Idxs[3 * nTri + 1],
				i2 = pOrigin->Idxs[3 * nTri + 2];
			vtx0 = pOrigin->Vert[i0],
				vtx1 = pOrigin->Vert[i1],
				vtx2 = pOrigin->Vert[i2];
			// A
			v3dA.x = vtx0.x - cx,
				v3dA.y = vtx0.y - cy,
				v3dA.z = vtx0.z;
			// B
			v3dB.x = vtx1.x - cx,
				v3dB.y = vtx1.y - cy,
				v3dB.z = vtx1.z;
			// C
			v3dC.x = vtx2.x - cx,
				v3dC.y = vtx2.y - cy,
				v3dC.z = vtx2.z;
			bX = zXTri(&Z, v3dA, v3dB, v3dC);
			if(bX == true)
			{
				nXTri = VBMeshRef.nTriangles[i];
				break;
			}
		}
		pOrigin = NULL;
		pQuads.Clear();
		if(bX == false) return;
		// calculating normal in tri
		int nx = 0, ny = 0, nz = 0;
		nx = vtx0.nx + vtx1.nx + vtx2.nx,
			ny = vtx0.ny + vtx1.ny + vtx2.ny,
			nz = vtx0.nz + vtx1.nz + vtx2.nz;
		nx /= 3,
			ny /= 3,
			nz /= 3;
		Vector3D v3dDir, v3dPos;
		v3dDir.x = -nx,
			v3dDir.y = -ny,
			v3dDir.z = -nz;
		v3dDir.normalize();
		// calculating position if eye center
		v3dPos.x = cx + depth * v3dDir.x,
			v3dPos.y = cy + depth * v3dDir.y,
			v3dPos.z = Z + depth * v3dDir.z;
		// creating hollow
		//	VertexBasedMeshRef Ref;
		BaseDirectionalFunction bdFn;
		bdFn.Create(v3dPos, v3dDir, r);
		int x0 = 0, y0 = 0, x1 = 0, y1 = 0;
		//	bdFn.ReturnMeshRef(g_UniversalMap, Ref, x0, x1, y0, y1);
		bdFn.DeleteDirectionalSurface(VBMeshRef);
		// invalidating ALL! should be fixed !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		VBMeshRef.Smooth(0.5, 2);
		CreateNormals();
		FillMapOfHeights();
//		float CreateNaturalShadow(VertexBasedMesh *, int);
//		CreateNaturalShadow(this, 2048);
		void ResetGroundCache();
		ResetGroundCache();
}

// ================================================================================
// UniversalMap::GetModelMinMaxXYZ
// ================================================================================
bool UniversalMap::GetModelMinMaxXYZ(const char *strModelName,
									 int &minX, int &minY, int &minZ, int &maxX, int &maxY, int &maxZ)
{
	// Getting pointer on geometry interface and number of geometries
	DWORD idModel = 0;
	idModel = IMM->GetModelID(strModelName);
	int NGeometries = 0;
	NGeometries = IMM->GetNumGeometries(idModel);
	if(NGeometries <= 0) return false;
	DynArray<Matrix4D> m4dLocals;
	Matrix4D m4dLocal;
	DynArray<IGeometry*> pIGeometries;
	IGeometry *pIGeometry = NULL;
	int i = 0;
	for(i = 0; i < NGeometries; i++)
	{
		pIGeometry = IMM->GetGeometry(idModel, i, m4dLocal);
		pIGeometries.Add(pIGeometry);
		m4dLocals.Add(m4dLocal);
	}
	pIGeometry = NULL;
	// ---------------
	int min_x = INT_MAX, max_x = INT_MIN, min_y = INT_MAX, max_y = INT_MIN,
		min_z = INT_MAX, max_z = INT_MIN;
	for(int nG = 0; nG < NGeometries; nG++)
	{
		// Caching geometry in cVertices
		DynArray<Vector3D> cVertices;
		int NVertices = 0;
		NVertices = pIGeometries[nG]->GetNumVertices();
		VertexIterator vtxIterator;
		pIGeometries[nG]->GetVertexIterator(vtxIterator);
		for(i = 0; i < NVertices; i++) cVertices.Add(vtxIterator.pos(i));
		// Coordinates transformation and additional caching
		DynArray<OneMapVertex> cVert;
		Matrix4D m4dT;
		m4dT = m4dLocals[nG];
		Vector3D v3d;
		OneMapVertex vtx;
		float x = 0, y = 0, z = 0;
		for(i = 0; i < NVertices; i++)
		{
			v3d = cVertices[i];
			x = v3d.x * m4dT.e00 + v3d.y * m4dT.e10 + v3d.z * m4dT.e20 + m4dT.e30;
			y = v3d.x * m4dT.e01 + v3d.y * m4dT.e11 + v3d.z * m4dT.e21 + m4dT.e31;
			z = v3d.x * m4dT.e02 + v3d.y * m4dT.e12 + v3d.z * m4dT.e22 + m4dT.e32;

			vtx.x = (short)x,
				vtx.y = (short)y,
				vtx.z = (short)z;
			cVert.Add(vtx);
		}
		// Determining min and max values
		for(i = 0; i < NVertices; i++)
		{
			vtx = cVert[i];
			// vtx
			if(vtx.x < min_x) min_x = vtx.x;
			if(vtx.y < min_y) min_y = vtx.y;
			if(vtx.z < min_z) min_z = vtx.z;
			if(vtx.x > max_x) max_x = vtx.x;
			if(vtx.y > max_y) max_y = vtx.y;
			if(vtx.z > max_z) max_z = vtx.z;
		}
		// uninit
		cVert.Clear();
		cVertices.Clear();
	}
	// uninit
	m4dLocals.Clear();
	pIGeometries.Clear();
	// -----------------------------
	minX = min_x;
	maxX = max_x;
	minY = min_y;
	maxY = max_y;
	minZ = min_z;
	maxZ = max_z;
	return true;
}

void VertexBasedMesh::AddTriangle(const OneMapVertex &v0, const OneMapVertex &v1, const OneMapVertex &v2)
{
	// Not the best implementation :-|
	int i0 = Vert.find(v0), i1 = Vert.find(v1), i2 = Vert.find(v2);
	if(i0 == -1) i0 = Vert.Add(v0);
	if(i1 == -1) i1 = Vert.Add(v1);
	if(i2 == -1) i2 = Vert.Add(v2);
	Idxs.Add(i0), Idxs.Add(i1), Idxs.Add(i2);
}

// ================================================================================
// UniversalMap::AddModelByName
// ================================================================================
//#include "SurfaceGenerator.h"
//extern SurfaceGenerator SURF;
void UniversalMap::AddModelByName(const char *strModelName, const Matrix4D &m4dMap)
{
	const DWORD dwColor = 0xFFCCCCCC;
	const int nFacture = 0;
	// Getting pointer on geometry interface and number of geometries
	DWORD idModel = 0;
	idModel = IMM->GetModelID(strModelName);
	int NGeometries = 0;
	NGeometries = IMM->GetNumGeometries(idModel);
	if(NGeometries <= 0) return;
	DynArray<Matrix4D> m4dLocals;
	Matrix4D m4dLocal;
	DynArray<IGeometry*> pIGeometries;
	IGeometry *pIGeometry = NULL;
	int i = 0;
	for(i = 0; i < NGeometries; i++)
	{
		pIGeometry = IMM->GetGeometry(idModel, i, m4dLocal);
		pIGeometries.Add(pIGeometry);
		m4dLocals.Add(m4dLocal);
	}
	pIGeometry = NULL;
	// ---------------
	int UMLx = 0, UMLy = 0,
		min_x = 0, max_x = 0, min_y = 0, max_y = 0, min_z = 0, max_z = 0;
	UMLx = QuadLx * QuadsNx,
		UMLy = QuadLy * QuadsNy;
	min_x = QuadX0 + UMLx,
		min_y = QuadY0 + UMLy,
		min_z = INT_MAX;
	VertexBasedMesh cMesh;
	for(int nG = 0; nG < NGeometries; nG++)
	{
		// Caching geometry in cVertices, cColors and cIndices
		DynArray<Vector3D> cVertices;
//		DynArray<DWORD> cIndices;
		int NIndices = 0, NVertices = 0;
		NIndices = pIGeometries[nG]->GetNumIndices();
		NVertices = pIGeometries[nG]->GetNumVertices();
		WORD *pIndices = NULL;
		pIndices = pIGeometries[nG]->GetIndices();
		if(pIndices == NULL) return;
//		for(i = 0; i < NIndices; i++) cIndices.Add(*(pIndices + i));
		for(i = 0; i < NIndices; i++) cMesh.Idxs.Add(*(pIndices + i));
		pIndices = NULL;
		VertexIterator vtxIterator;
		pIGeometries[nG]->GetVertexIterator(vtxIterator);
		for(i = 0; i < NVertices; i++) cVertices.Add(vtxIterator.pos(i));
		// Coordinates transformation and additional caching
//		DynArray<OneMapVertex> cVert;
		Matrix4D m4dT;
		m4dT = m4dLocals[nG];
		m4dT *= m4dMap;
		Vector3D v3d;
		OneMapVertex vtx;
		vtx.nFacture = nFacture;
		float x = 0, y = 0, z = 0;
		for(i = 0; i < NVertices; i++)
		{
			v3d = cVertices[i];
			x = v3d.x * m4dT.e00 + v3d.y * m4dT.e10 + v3d.z * m4dT.e20 + m4dT.e30;
			y = v3d.x * m4dT.e01 + v3d.y * m4dT.e11 + v3d.z * m4dT.e21 + m4dT.e31;
			z = v3d.x * m4dT.e02 + v3d.y * m4dT.e12 + v3d.z * m4dT.e22 + m4dT.e32;

			vtx.x = (short)x,
				vtx.y = (short)y,
				vtx.z = (short)z,
				vtx.Color = dwColor,
				vtx.Shadow = 200;
			cMesh.Vert.Add(vtx);
		}
		// Merging UniversalMap with cache
/*		int cx = 0, cy = 0, cz = 0,	// to know in which quad we should add reference
			oi0 = 0, oi1 = 0, oi2 = 0, nTri = 0, NVert = 0, NTriMax = 0, NTriMaxIdxs = 0,
			nqx = 0, nqy = 0, N = 0;
		OneMapVertex vtx0, vtx1, vtx2;
		NVert = Vert.GetAmount(); // we should add to index
		NTriMaxIdxs = Idxs.GetAmount() / 3; // we should add to nTri in quad
		Vert.AddValues(cVert.GetValues(), cVert.GetAmount());
		N = cIndices.GetAmount();
		for(i = 0; i < N; i++) Idxs.Add(cIndices[i] + NVert);
		// Adding reference to appropriate quad
		NTriMax = cIndices.GetAmount() / 3;
		for(nTri = 0; nTri < NTriMax; nTri++)
		{
			oi0 = cIndices[3 * nTri + 0],
				oi1 = cIndices[3 * nTri + 1],
				oi2 = cIndices[3 * nTri + 2];
			vtx0 = cVert[oi0], 
				vtx1 = cVert[oi1],
				vtx2 = cVert[oi2];
			cx = (vtx0.x + vtx1.x + vtx2.x) / 3,
				cy = (vtx0.y + vtx1.y + vtx2.y) / 3,
				cz = (vtx0.z + vtx1.z + vtx2.z) / 3;
			if((cx >= QuadX0) && (cx < QuadX0 + UMLx) && // nTri is within UniversalMap so we can add reference
				(cy >= QuadY0) && (cy < QuadY0 + UMLy))
			{
				// vtx0
				if(vtx0.x < min_x) min_x = vtx0.x;
				if(vtx0.y < min_y) min_y = vtx0.y;
				if(vtx0.z < min_z) min_z = vtx0.z;
				if(vtx0.x > max_x) max_x = vtx0.x;
				if(vtx0.y > max_y) max_y = vtx0.y;
				if(vtx0.z > max_z) max_z = vtx0.z;
				// vtx1
				if(vtx1.x < min_x) min_x = vtx1.x;
				if(vtx1.y < min_y) min_y = vtx1.y;
				if(vtx1.z < min_z) min_z = vtx1.z;
				if(vtx1.x > max_x) max_x = vtx1.x;
				if(vtx1.y > max_y) max_y = vtx1.y;
				if(vtx1.z > max_z) max_z = vtx1.z;
				// vtx2
				if(vtx2.x < min_x) min_x = vtx2.x;
				if(vtx2.y < min_y) min_y = vtx2.y;
				if(vtx2.z < min_z) min_z = vtx2.z;
				if(vtx2.x > max_x) max_x = vtx2.x;
				if(vtx2.y > max_y) max_y = vtx2.y;
				if(vtx2.z > max_z) max_z = vtx2.z;
				nqx = (cx - QuadX0) / QuadLx,
					nqy = (cy - QuadY0) / QuadLy;
				m_pQuads[nqx + QuadsNx * nqy]->nTriangles.Add(nTri + NTriMaxIdxs);
			}
		}*/
		cMesh.CalcNormals();
//		cMesh.CalcShadow();
		extern CSun g_Sun;
//		cMesh.Shade(g_Sun); // SHADE

//		SURF.TexturizeSurface(cMesh);
		Sum(cMesh);
		// uninit
//		cVert.Clear();
		cVertices.Clear();
//		cIndices.Clear();

		cMesh.Release();
	}
	// uninit
	m4dLocals.Clear();
	pIGeometries.Clear();
	// filling bound rectangle and invalidating geometry
//	Rct rc(min_x, min_y, max_x - min_x, max_y - min_y);
//	ITerra->InvalidateGeometry(&rc);
	void ResetGroundCache(void);
	ResetGroundCache();
}

void UniversalMap::ExtractQuadsInRadius(CUniversalQuadsPointers &rQuadsPointers, int cx, int cy, int r)
{
	if(r <= 0) return;
	int QuadR = sqrt(((QuadLx * QuadLx) >> 2) + ((QuadLy * QuadLy) >> 2)); // rough bound radius of quad
	QuadR += (QuadR >> 0); // some offset
	int D = r + QuadR; // Distance in which quads will be located
	// Extracting potential quads
	for(int nQy = 0; nQy < QuadsNy; nQy++)
		for(int nQx = 0; nQx < QuadsNx; nQx++)
		{
			int cQx = QuadX0 + nQx * QuadLx + (QuadLx >> 1),
				cQy = QuadY0 + nQy * QuadLy + (QuadLy >> 1);
			if(sqrt((cx - cQx) * (cx - cQx) + (cy - cQy) * (cy - cQy)) <= D)
			{
				OneUniversalMapQuad *pQuad = m_pQuads[nQx + QuadsNx * nQy];
				if(!pQuad) return;
				pQuad->x0 = QuadX0 + QuadLx * nQx, pQuad->y0 = QuadY0 + QuadLy * nQy;
				rQuadsPointers.Add(pQuad);
			}
		}
}

bool UniversalMap::ExtractQuadsInQuad_InQuad(int x0, int y0, int x1, int y1, int X0, int Y0, int X1, int Y1)
{
	int x[] = {x0, x0 + QuadLx, x0 + QuadLx, x0}, y[] = {y0, y0, y0 + QuadLy, y0 + QuadLy};
	for(int i = 0; i < 4; i++) if(x[i] >= X0 && x[i] <= X1 && y[i] >= Y0 && y[i] <= Y1) return true;
	return false;
}
void UniversalMap::ExtractQuadsInQuad(CUniversalQuadsPointers &rQuadsPointers, int cx, int cy, int S)
{
	if(S <= 0) return;
	int X0 = cx - S, Y0 = cy - S, X1 = cx + S, Y1 = cy + S;
	for(int nQy = 0; nQy < QuadsNy; nQy++)
		for(int nQx = 0; nQx < QuadsNx; nQx++)
		{
			OneUniversalMapQuad *pQuad = m_pQuads[nQx + QuadsNx * nQy];
			if(!pQuad) return;
			int x0 = QuadX0 + nQx * QuadLx, y0 = QuadY0 + nQy * QuadLy,
				x1 = x0 + QuadLx, y1 = y0 + QuadLy;
			if(ExtractQuadsInQuad_InQuad(x0, y0, x1, y1, X0, Y0, X1, Y1)) rQuadsPointers.Add(pQuad);
		}
}

void UniversalMap::ExtractQuads(CUniversalQuadsPointers &rQuadsPointers, const VertexBasedMesh &rVBMesh)
{
	if(!rVBMesh.Vert.NValues) return;
	int MinX = QuadX0 + QuadLx * QuadsNx, MinY = QuadY0 + QuadLy * QuadsNy, MaxX = QuadX0, MaxY = QuadY0;
	for(int i = 0; i < rVBMesh.Vert.NValues; i++)
	{
		OneMapVertex &rvtx = rVBMesh.Vert[i];
		if(rvtx.x > MaxX) MaxX = rvtx.x;
		if(rvtx.x < MinX) MinX = rvtx.x;
		if(rvtx.y > MaxY) MaxY = rvtx.y;
		if(rvtx.y < MinY) MinY = rvtx.y;
	}
	int cx = MinX + (MaxX - MinX) / 2, cy = MinY + (MaxY - MinY) / 2,
		dx = MaxX - cx, dy = MaxY - cy, r = sqrt(dx * dx + dy * dy);
	ExtractQuadsInRadius(rQuadsPointers, cx, cy, r);
}

// ================================================================================
// VertexBaseMesh::fromModelByName
// ================================================================================
bool VertexBasedMesh::fromModelByName(const char *strModelName, const Matrix4D &m4dWorld)
{
	Release();
	const DWORD dwColor = 0xFFCCCCCC;
	const int nFacture = 0;
	// Gaining geometry:
	DWORD idModel = IMM->GetModelID(strModelName);
	int NGeometries = IMM->GetNumGeometries(idModel);
	if(NGeometries <= 0) return false;
	DynArray<Matrix4D> m4dLocals;
	Matrix4D m4dLocal;
	DynArray<IGeometry*> pIGeometries;
	for(int i = 0; i < NGeometries; i++)
	{
		IGeometry *pIGeometry = IMM->GetGeometry(idModel, i, m4dLocal);
		pIGeometries.Add(pIGeometry);
		m4dLocals.Add(m4dLocal);
	}
	for(int nG = 0; nG < NGeometries; nG++)
	{
		// Caching geometry:
		DynArray<Vector3D> cVertices;
		DynArray<DWORD> cIndices;
		int NVertices = pIGeometries[nG]->GetNumVertices(),
			NIndices = pIGeometries[nG]->GetNumIndices();
		WORD *pIndices = pIGeometries[nG]->GetIndices();
		if(!pIndices) return false;
		for(i = 0; i < NIndices; i++) cIndices.Add(*pIndices), pIndices++;
		VertexIterator vtxIterator;
		pIGeometries[nG]->GetVertexIterator(vtxIterator);
		for(i = 0; i < NVertices; i++) cVertices.Add(vtxIterator.pos(i));
		// Transforming coordanates:
		DynArray<OneMapVertex> MVertices;
		Matrix4D m4dT;
		m4dT = m4dLocals[nG];
		m4dT *= m4dWorld;
		Vector3D v3d;
		OneMapVertex vtx;
		vtx.nFacture = nFacture, vtx.Color = dwColor, vtx.Shadow = 200;
		for(i = 0; i < NVertices; i++)
		{
			v3d = cVertices[i];
			float x = v3d.x * m4dT.e00 + v3d.y * m4dT.e10 + v3d.z * m4dT.e20 + m4dT.e30,
				y = v3d.x * m4dT.e01 + v3d.y * m4dT.e11 + v3d.z * m4dT.e21 + m4dT.e31,
				z = v3d.x * m4dT.e02 + v3d.y * m4dT.e12 + v3d.z * m4dT.e22 + m4dT.e32;

			vtx.x = (short)x, vtx.y = (short)y, vtx.z = (short)z;
			MVertices.Add(vtx);
		}
		// Combining geometry with this VertexBasedMesh:
		int NVert = Vert.NValues;
		Vert.AddValues(MVertices.GetValues(), MVertices.NValues);
		for(i = 0; i < cIndices.NValues; i++) Idxs.Add(cIndices[i] + NVert);

		cVertices.Clear(), cIndices.Clear(), MVertices.Clear();
	}
	m4dLocals.Clear(), pIGeometries.Clear();
	return true;
}

int VertexBasedMesh::CrossSectionTriangle(const CVector3D &a, const CVector3D &b, DWORD nTri)
{
	const float fD = 0.0f;
	int i0 = Idxs[3 * nTri + 0], i1 = Idxs[3 * nTri + 1], i2 = Idxs[3 * nTri + 2];
	CVector3D p0(Vert[i0]), p1(Vert[i1]), p2(Vert[i2]);
	CVector3D H = b - a, V = p1 - p0, W = p2 - p1, N = V ^ W;
	N.Normalize();
	if(fabs(N * H) <= fD)
		return -1; // We should retest!
	CVector3D D = p0 - a;
	Matrix3D M(H.x, H.y, H.z, -V.x, -V.y, -V.z, -W.x, -W.y, -W.z);
	M.inverse();
	float u = D.x * M.e00 + D.y * M.e10 + D.z * M.e20,
		v = D.x * M.e01 + D.y * M.e11 + D.z * M.e21,
		w = D.x * M.e02 + D.y * M.e12 + D.z * M.e22;
	CVector3D q = a + u * H;
	if(fabs(u) <= fD)
		return -1;
	if(fabs(u - 1.0f) <= fD)
		return -1;
	if(fabs(v) <= fD)
		return -1;
	if(fabs(v - 1.0f) <= fD)
		return -1;
	if(fabs(w) <= fD)
		return -1;
	if(fabs(w - 1.0f) <= fD)
		return -1;
	bool r = ((u >= 0.0f) && (u <= 1.0f) && (v >= 0.0f) && (v <= 1.0f) && (w >= 0.0f) && (w <= 1.0f) && (w <= v));
	if(r) return 1;
	return 0;
}

bool VertexBasedMesh::CrossSectionMesh(const OneMapVertex &rVertexA, const OneMapVertex &rVertexB)
{
	int NTri = Idxs.NValues / 3;
	for(int nTri = 0; nTri < NTri; nTri++)
	{
		int r = CrossSectionTriangle(rVertexA, rVertexB, nTri);
		if((r == -1) || (r == 1)) return true;
	}
	return false;
}

bool VertexBasedMesh::VertexIsInside(const OneMapVertex &rVertex)
{
	const float xyVariation = 1000.0f, zGrowth = 10000.0f;
	int NTri = Idxs.NValues / 3;
	int kAntiLoop = 0;
VertexIsInside_Label:
	if(kAntiLoop > 10) return true;
	kAntiLoop++;
	CVector3D A(rVertex);
	CVector3D B(A + CVector3D(-xyVariation / 2.0f + rand() % int(xyVariation),
		-xyVariation / 2.0f + rand() % int(xyVariation), zGrowth));
	int nIntersections = 0; 
	for(int nTri = 0; nTri < NTri; nTri++)
	{
		int r = CrossSectionTriangle(A, B, nTri);
		if(r == -1)
			goto VertexIsInside_Label;
		if(r == 1) nIntersections++;
	}
	return nIntersections & 1;
	// CONVEX TEST
/*	CVector3D V = rVertex;
	int NTri = Idxs.NValues / 3;
	for(int nTri = 0; nTri < NTri; nTri++)
	{
		OneMapVertex &vtx0 = Vert[Idxs[3 * nTri + 0]], &vtx1 = Vert[Idxs[3 * nTri + 1]],
			&vtx2 = Vert[Idxs[3 * nTri + 2]];
		CVector3D v0 = vtx0, v1 = vtx1, v2 = vtx2;
		CVector3D Normal = (v1 - v0) ^ (v2 - v1);
		float f = (V - v0) * Normal;
		if(f > 0) return false;
	}
	return true;*/
}

void UniversalMap::CalcMinMax(const OneMapVertex &rMVertex, OneMapVertex &rMin, OneMapVertex &rMax)
{
	if(rMVertex.x > rMax.x) rMax.x = rMVertex.x;
	if(rMVertex.x < rMin.x) rMin.x = rMVertex.x;
	if(rMVertex.y > rMax.y) rMax.y = rMVertex.y;
	if(rMVertex.y < rMin.y) rMin.y = rMVertex.y;
}

// ================================================================================
// UniversalMap::CombineModelByName
// ================================================================================
void UniversalMap::CombineModelByName(const char *strModelName, const Matrix4D &m4dWorld)
{
	VertexBasedMesh A, B;
	if(!A.fromModelByName(strModelName, m4dWorld)) return;
	CUniversalQuadsPointers pQuads;
	ExtractQuads(pQuads, A);
	for(int nQ = 0; nQ < pQuads.NValues; nQ++)
		for(int i = 0; i < pQuads[nQ]->nTriangles.NValues;)
		{
			int nTri = pQuads[nQ]->nTriangles[i];
			int ii0 = 3 * nTri + 0, ii1 = 3 * nTri + 1, ii2 = 3 * nTri + 2;
			int i0 = Idxs[ii0], i1 = Idxs[ii1], i2 = Idxs[ii2];
			OneMapVertex &v0 = Vert[i0], &v1 = Vert[i1], &v2 = Vert[i2];
			B.AddTriangle(v0, v1, v2);
//			bool c0 = A.CrossSectionMesh(v0, v1), c1 = A.CrossSectionMesh(v1, v2),
//				c2 = A.CrossSectionMesh(v2, v0);
			bool r0 = A.VertexIsInside(v0), r1 = A.VertexIsInside(v1), r2 = A.VertexIsInside(v2);
			//if(!r0 && !r1 && !r2 && !c0 && !c1 && !c2) { i++; continue; } // We leave this triangle...
			if(r0 || r1 || r2) pQuads[nQ]->nTriangles.Del(i, 1);
			else i++;
		}
	for(int nTri = 0; nTri < A.Idxs.NValues / 3;)
	{
		int ii0 = 3 * nTri + 0, ii1 = 3 * nTri + 1, ii2 = 3 * nTri + 2;
		int i0 = A.Idxs[ii0], i1 = A.Idxs[ii1], i2 = A.Idxs[ii2];
		OneMapVertex &v0 = A.Vert[i0], &v1 = A.Vert[i1], &v2 = A.Vert[i2];
		bool r0 = B.VertexIsInside(v0), r1 = B.VertexIsInside(v1), r2 = B.VertexIsInside(v2);
		if(r0 && r1 && r2) A.Idxs.Del(ii0, 3);
		else nTri++;
	}

	VertexBasedMesh R;
	R.Vert.AddValues(A.Vert.GetValues(), A.Vert.NValues);
	R.Idxs.AddValues(A.Idxs.GetValues(), A.Idxs.NValues);

	int UMLx = QuadLx * QuadsNx, UMLy = QuadLy * QuadsNy;
	OneMapVertex MMin, MMax;
	MMin.x = QuadX0 + UMLx, MMin.y = QuadY0 + UMLy, MMax.x = QuadX0, MMax.y = QuadY0;
	// Combining geometry:
	int NVert = Vert.NValues, NTriMaxIdxs = Idxs.NValues / 3;
	Vert.AddValues(R.Vert.GetValues(), R.Vert.NValues);
	for(int i = 0; i < R.Idxs.NValues; i++) Idxs.Add(R.Idxs[i] + NVert);
	// Adding reference to quads system:
	int NTriMax = R.Idxs.NValues / 3;
	for(int nTri = 0; nTri < NTriMax; nTri++)
	{
		int oi0 = R.Idxs[3 * nTri + 0], oi1 = R.Idxs[3 * nTri + 1], oi2 = R.Idxs[3 * nTri + 2];
		OneMapVertex vtx0 = R.Vert[oi0], vtx1 = R.Vert[oi1], vtx2 = R.Vert[oi2];
		int cx = (int(vtx0.x) + int(vtx1.x) + int(vtx2.x)) / 3,
			cy = (int(vtx0.y) + int(vtx1.y) + int(vtx2.y)) / 3,
			cz = (int(vtx0.z) + int(vtx1.z) + int(vtx2.z)) / 3;
		if((cx >= QuadX0) && (cx < QuadX0 + UMLx) && (cy >= QuadY0) && (cy < QuadY0 + UMLy))
		{
			CalcMinMax(vtx0, MMin, MMax);
			CalcMinMax(vtx1, MMin, MMax);
			CalcMinMax(vtx2, MMin, MMax);
			int nQx = (cx - QuadX0) / QuadLx, nQy = (cy - QuadY0) / QuadLy;
			m_pQuads[nQx + QuadsNx * nQy]->nTriangles.Add(nTri + NTriMaxIdxs);
		}
	}
	pQuads.Clear();
	A.Release(), R.Release();
	// Filling bound rectangle and invalidating geometry:
	Rct rc(MMin.x, MMin.y, MMax.x - MMin.x, MMax.y - MMin.y);
	ITerra->InvalidateGeometry(&rc);
}

// ================================================================================
// UniversalMap::GetMeshRefCylinder
// ================================================================================
void UniversalMap::GetMeshRefCylinder(int cx, int cy, int r, VertexBasedMeshRef *pVBMRef)
{
	if(r <= 0) return;
	int QuadR = 0; // rough bound radius of quad
	QuadR = sqrt(((QuadLx * QuadLx) >> 2) + ((QuadLy * QuadLy) >> 2));
	QuadR += (QuadR >> 1); // some offset
	int D = r + QuadR; // in which distance quad should be located
	// Extracting potential quads
	DynArray<OneUniversalMapQuad*> pQuads;
	int nqx = 0, nqy = 0, nq = 0, l = 0,
		X = 0, Y = 0; // center of quad
	for(nqy = 0; nqy < QuadsNy; nqy++)
		for(nqx = 0; nqx < QuadsNx; nqx++)
		{
			X = QuadX0 + nqx * QuadLx + (QuadLx >> 1),
				Y = QuadY0 + nqy * QuadLy + (QuadLy >> 1);
			l = sqrt((cx - X) * (cx - X) + (cy - Y) * (cy - Y));
			if(l <= D)
			{
				nq = nqx + QuadsNx * nqy;
				pQuads.Add(m_pQuads[nq]);
			}
		}
		// Merging all triangles from extracted quads in pQuads to nAllTriangles
		pVBMRef->Release();
		pVBMRef->pOrigin = this;
		OneUniversalMapQuad *pQuad = NULL;
		DynArray<DWORD> nAllTriangles;
		int N = 0, i = 0;
		N = pQuads.GetAmount();
		if(N == 0) return;
		for(i = 0; i < N; i++)
		{
			pQuad = pQuads[i];
			nAllTriangles.AddValues(pQuad->nTriangles.GetValues(),
				pQuad->nTriangles.GetAmount());
		}
		pQuad = NULL;
		pQuads.Clear();
		// Copying nAllTriangles to pVBMRef->nTriangles whose at least one vertex is in r
		// and calculating bound rect
		N = nAllTriangles.GetAmount();
		if(N == 0) return;
		int nTri = 0, min_x = 0, min_y = 0, max_x = 0, max_y = 0,
			i0 = 0, i1 = 0, i2 = 0,	r2 = 0;
		r2 = r * r;
		min_x = QuadX0 + QuadLx * QuadsNx,
			max_x = QuadX0,
			min_y = QuadY0 + QuadLy * QuadsNy,
			max_y = QuadY0;
		for(i = 0; i < N; i++)
		{
			nTri = nAllTriangles[i];
			i0 = Idxs[3 * nTri + 0],
				i1 = Idxs[3 * nTri + 1],
				i2 = Idxs[3 * nTri + 1];
			OneMapVertex &rvtx0 = Vert[i0],
				&rvtx1 = Vert[i1],
				&rvtx2 = Vert[i2];
			int vtx0_d2 = 0, vtx1_d2 = 0, vtx2_d2 = 0;
			vtx0_d2 = (cx - rvtx0.x) * (cx - rvtx0.x) + (cy - rvtx0.y) * (cy - rvtx0.y),
				vtx1_d2 = (cx - rvtx1.x) * (cx - rvtx1.x) + (cy - rvtx1.y) * (cy - rvtx1.y),
				vtx2_d2 = (cx - rvtx2.x) * (cx - rvtx2.x) + (cy - rvtx2.y) * (cy - rvtx2.y);
			if((vtx0_d2 <= r2) || (vtx1_d2 <= r2) || (vtx2_d2 <= r2))
			{
				pVBMRef->nTriangles.Add(nTri);

				if(rvtx0.x < min_x) min_x = rvtx0.x;
				if(rvtx0.x > max_x) max_x = rvtx0.x;
				if(rvtx0.y < min_y) min_y = rvtx0.y;
				if(rvtx0.y > max_y) max_y = rvtx0.y;

				if(rvtx1.x < min_x) min_x = rvtx1.x;
				if(rvtx1.x > max_x) max_x = rvtx1.x;
				if(rvtx1.y < min_y) min_y = rvtx1.y;
				if(rvtx1.y > max_y) max_y = rvtx1.y;

				if(rvtx2.x < min_x) min_x = rvtx2.x;
				if(rvtx2.x > max_x) max_x = rvtx2.x;
				if(rvtx2.y < min_y) min_y = rvtx2.y;
				if(rvtx2.y > max_y) max_y = rvtx2.y;
			}
		}
		nAllTriangles.Clear();
		pVBMRef->x0 = min_x,
			pVBMRef->y0 = min_y,
			pVBMRef->w = max_x - min_x,
			pVBMRef->h = max_y - min_y;
}

// ================================================================================
// VertexBasedMeshRef::ExtractUniqueVertices
// ================================================================================
void VertexBasedMeshRef::ExtractUniqueVertices(DynArray<OneMapVertex*> *pDAVertices)
{
	pDAVertices->Clear();
	int N = 0;
	N = nTriangles.GetAmount();
	if(N == 0) return;
	// Enumerating all triangles
	int i = 0, nTri = 0, i0 = 0, i1 = 0, i2 = 0;
	Hash<nHashTableEntry> pDAVerticesHash;
	nHashTableEntry nEntry;
	for(i = 0; i < N; i++)
	{
		nTri = nTriangles[i];
		i0 = pOrigin->Idxs[3 * nTri + 0],
			i1 = pOrigin->Idxs[3 * nTri + 1],
			i2 = pOrigin->Idxs[3 * nTri + 2];
		// i0
		nEntry.i = i0;
		if(pDAVerticesHash.find(nEntry) == NO_ELEMENT)
		{
			pDAVertices->Add(&pOrigin->Vert[i0]);
			pDAVerticesHash.add(nEntry);
		}
		// i1
		nEntry.i = i1;
		if(pDAVerticesHash.find(nEntry) == NO_ELEMENT)
		{
			pDAVertices->Add(&pOrigin->Vert[i1]);
			pDAVerticesHash.add(nEntry);
		}
		// i2
		nEntry.i = i2;
		if(pDAVerticesHash.find(nEntry) == NO_ELEMENT)
		{
			pDAVertices->Add(&pOrigin->Vert[i2]);
			pDAVerticesHash.add(nEntry);
		}
	}
	pDAVerticesHash.reset();
}


// ================================================================================
// VertexBasedMeshRef::SupplementMapOfHeights
// ================================================================================
void VertexBasedMeshRef::SupplementMapOfHeights()
{
	int N = 0;
	N = nTriangles.GetAmount();
	if(N == 0) return;
	// Enumerating all nodes in map of heights
	for(int ny = 0; ny < MaxTH; ny++)
		for(int nx = 0; nx < VertInLine; nx++)
		{
			int x = nx << 5, y = ny << 5;
			int T = (nx >> 1) << 1;
			if(T != nx) y-= 16;
			// If node of map of heights x,y is withing bound rectangle,
			// we should find for intersections
			if((x >= x0) && (x < x0 + w) && (y >= y0) && (y < y0 + h))
			{
				const float cNotIntersected = -999.99f;
				float Z = cNotIntersected, z = 0;
				for(int i = 0; i < N; i++)
				{
					int nTri = nTriangles[i];
					int i0 = pOrigin->Idxs[3 * nTri + 0],
						i1 = pOrigin->Idxs[3 * nTri + 1],
						i2 = pOrigin->Idxs[3 * nTri + 2];
					OneMapVertex &rvtx0 = pOrigin->Vert[i0],
						&rvtx1 = pOrigin->Vert[i1],
						&rvtx2 = pOrigin->Vert[i2];
					Vector3D v3dA(rvtx0.x - x, rvtx0.y - y, rvtx0.z),
						v3dB(rvtx1.x - x, rvtx1.y - y, rvtx1.z),
						v3dC(rvtx2.x - x, rvtx2.y - y, rvtx2.z);
					if(zXTri(&z, v3dA, v3dB, v3dC) == true)
						if(z > Z) Z = z;
				}
				if(Z != cNotIntersected) THMap[nx + VertInLine * ny] = (short)Z;
			}
		}
}

// ================================================================================
// VertexBasedMeshRef::SupplementShadow
// ================================================================================
void VertexBasedMeshRef::SupplementShadow(const CSun *pSun)
{
	DynArray<OneMapVertex*> pVertices;
	ExtractUniqueVertices(&pVertices);
	VertexBasedMesh Mesh;
	for(int i = 0; i < pVertices.NValues; i++)
		if(pVertices[i])
			Mesh.Vert.Add(*pVertices[i]);
	Mesh.Shade(false, pSun);
	for(i = 0; i < Mesh.Vert.NValues; i++)
		pVertices[i]->Shadow = Mesh.Vert[i].Shadow;
}

// ================================================================================
// VertexBasedMesh::Shade
// ================================================================================
void VertexBasedMesh::Shade(bool bCastShadows, const CSun *pSun)
{
	if(!Vert.NValues || (bCastShadows && !Idxs.NValues)) return;
	// Defining sources:
	CVectors3D Sources;
	int ShadowMinLevel, ShadowMaxLevel;
	float k;
	if(pSun) // from pSun:
	{
		for(int nSource = 0; nSource < pSun->m_NSources; nSource++)
			Sources.Add(pSun->m_Sources[nSource]);
		ShadowMinLevel = pSun->m_ShadowMinLevel, ShadowMaxLevel = pSun->m_ShadowMaxLevel;
		k = pSun->m_k;
	}
	else // from CSun:
	{
		for(int nSource = 0; nSource < CSun::c_defNSources; nSource++)
			Sources.Add(CSun::c_defSources[nSource]);
		ShadowMinLevel = CSun::c_defShadowMinLevel, ShadowMaxLevel = CSun::c_defShadowMaxLevel;
		k = CSun::c_defk;
	}

	// Calculating maxShadow:
	float maxShadow = 0.0f;
	for(int nSource = 0; nSource < Sources.NValues; nSource++)
	{
		float zSource = Sources[nSource].z;
		float s = k + (1.0f - k) * zSource;
		if(s < k) s = k;
		if(s > 1.0f) s = 1.0f;
		maxShadow += s;
	}

	if(bCastShadows) CastShadows(Sources, k, ShadowMinLevel, ShadowMaxLevel, maxShadow);
	else // Shading without casting:
	{
		for(int nVertex = 0; nVertex < Vert.NValues; nVertex++)
		{
			float S = 0.0f;
			OneMapVertex &v = Vert[nVertex];
			Vector3D N(v.nx, v.ny, v.nz), D;
			N /= 127.0f;
			for(int nSource = 0; nSource < Sources.NValues; nSource++)
			{
				Vector3D ZAxis = Sources[nSource];
				D = N;
				float Shadow = k + (1.0f - k) * D.dot(ZAxis);
				if(Shadow < k) Shadow = k;
				if(Shadow > 1.0f) Shadow = 1.0f;
				S += Shadow;
			}
			float s = S * 128 / maxShadow;
			if(s < ShadowMinLevel) s = ShadowMinLevel;
			if(s > ShadowMaxLevel) s = ShadowMaxLevel;
			v.Shadow = s;
		}	
	}

	// unInit:
	Sources.Clear();
}

// ================================================================================
// VertexBasedMesh::RasterizeTriangle
// ================================================================================
void VertexBasedMesh::RasterizeTriangle(int *pZBuffer, const int cZBufferSide,
								const Vector3D &T0, const Vector3D &T1, const Vector3D &T2)
{
	int x0 = (int)T0.x, y0 = (int)T0.y, z0 = (int)T0.z,
		x1 = (int)T1.x, y1 = (int)T1.y, z1 = (int)T1.z,
		x2 = (int)T2.x, y2 = (int)T2.y, z2 = (int)T2.z;

#define iSWAP(a, b) { int t = a; a = b; b = t; }

	// Sorting ascending along y axis:
	if(y0 > y1) { iSWAP(x0, x1); iSWAP(y0, y1); iSWAP(z0, z1); }
	if(y0 > y2) { iSWAP(x0, x2); iSWAP(y0, y2); iSWAP(z0, z2); }
	if(y1 > y2) { iSWAP(x1, x2); iSWAP(y1, y2); iSWAP(z1, z2); }

	if(y2 - y1 == 0) return;

	for(int y = y0; y <= y2; y++)
	{
		// Crossing with longest section ((x0, y0) --- (x2, y2)):
		int _x1 = x0 + (y - y0) * (x2 - x0) / (y2 - y0);
		int _z1 = z0 + (y - y0) * (z2 - z0) / (y2 - y0);

		// Crossing with short section ((x0, y0) --- (x1, y1) or (x1, y1) --- (x2, y2)):
		int _x2, _z2;
		if(y < y1)
		{
			_x2 = x0 + (y - y0) * (x1 - x0) / (y1 - y0);
			_z2 = z0 + (y - y0) * (z1 - z0) / (y1 - y0);
		}
		else
		{
			if(y2 == y1)
			{
				_x2 = x1;
				_z2 = z1;
			}
			else
			{
				_x2 = x1 + (y - y1) * (x2 - x1) / (y2 - y1);
				_z2 = z1 + (y - y1) * (z2 - z1) / (y2 - y1);
			}
		}

		if(_x1 > _x2)
		{
			iSWAP(_x1, _x2);
			iSWAP(_z1, _z2);
		}

		// Rasterizing with horizontal line (_x1 - _x2):
		for(int x = _x1; x < _x2; x++)
		{
			int z = _z1 + (x - _x1) * (_z2 - _z1) / (_x2 - _x1);
			if(x >= 0 && x < cZBufferSide && y >= 0 && y < cZBufferSide)
				if(pZBuffer[y * cZBufferSide + x] < z)
					pZBuffer[y * cZBufferSide + x] = z;
		}
	}
#undef iSWAP
}

// ================================================================================
// VertexBasedMesh::CastOneShadow
// ================================================================================
void VertexBasedMesh::CastOneShadow(const int cZBufferSide, float k,
		Vector3D *pTVertices, int *pZBuffer, float *pShadows, const Vector3D &rSource)
{
	// Calculating coordinate system of source:
	Vector3D XAxis, YAxis, ZAxis = rSource;
	XAxis.cross(ZAxis, Vector3D::oZ);
	YAxis.cross(ZAxis, XAxis);
	Matrix3D T(XAxis.x, YAxis.x, ZAxis.x, XAxis.y, YAxis.y, ZAxis.y, XAxis.z, YAxis.z, ZAxis.z);

	// Transforming vertices and calculating extents:
//	int xMin = INT_MAX, xMax = INT_MIN, yMin = INT_MAX, yMax = INT_MIN, zMin = INT_MAX, zMax = INT_MIN;

	for(int nVertex = 0; nVertex < Vert.NValues; nVertex++)
	{
		const OneMapVertex &v = Vert[nVertex];
		Vector3D &vT = pTVertices[nVertex];
		vT.set(v.x, v.y, v.z);
		vT *= T;
	}
	const Vector3D &vT = pTVertices[0];
	float xMin = vT.x, xMax = vT.x, yMin = vT.y, yMax = vT.y, zMin = vT.z, zMax = vT.z;
	for(nVertex = 1; nVertex < Vert.NValues; nVertex++)
	{
		float x = pTVertices[nVertex].x, y = pTVertices[nVertex].y, z = pTVertices[nVertex].z;
		if(x < xMin) xMin = x;
		if(x > xMax) xMax = x;
		if(y < yMin) yMin = y;
		if(y > yMax) yMax = y;
		if(z < zMin) zMin = z;
		if(z > zMax) zMax = z;
	}
	// Scaling transformed vertices to fit (cZBufferSide x cZBufferSide):
//	int dx = xMax - xMin, dy = yMax - yMin, dz = zMax - zMin;
	float dx = xMax - xMin, dy = yMax - yMin, dz = zMax - zMin;
	float xScale = (float)cZBufferSide / float(dx),
		yScale = (float)cZBufferSide / float(dy),
		zScale = (float)0xFFFFFF / float(dz);
	for(nVertex = 0; nVertex < Vert.NValues; nVertex++)
	{
		Vector3D &vT = pTVertices[nVertex];
		vT.x = xScale * (vT.x - float(xMin) - dx / 2) + cZBufferSide / 2;
		vT.y = yScale * (vT.y - float(yMin) - dy / 2) + cZBufferSide / 2;
		vT.z = zScale * (vT.z - float(zMin) - dz / 2);
	}

	// Rasterizing triangles to rZBuffer:
	for(int i = 0; i < cZBufferSide * cZBufferSide; i++) pZBuffer[i] = INT_MIN;
	int NTri = Idxs.NValues / 3;
	for(int nTri = 0; nTri < NTri; nTri++)
	{
		int i0 = Idxs[3 * nTri + 0], i1 = Idxs[3 * nTri + 1], i2 = Idxs[3 * nTri + 2];
		Vector3D T0 = pTVertices[i0], T1 = pTVertices[i1], T2 = pTVertices[i2];
		RasterizeTriangle(pZBuffer, cZBufferSide, T0, T1, T2);
	}

	// Calculating shadow values:
	for(int nVertex = 0; nVertex < Vert.NValues; nVertex++)
	{
		const Vector3D &TVertex = pTVertices[nVertex];
		int x = (int)TVertex.x, y = (int)TVertex.y, z = (int)TVertex.z;
		if(x < 0 || x >= cZBufferSide || y < 0 || y >= cZBufferSide || z < pZBuffer[y * cZBufferSide + x]) continue;
		const OneMapVertex &v = Vert[nVertex];
		Vector3D v3dNormal(v.nx, v.ny, v.nz);
		v3dNormal /= 127.0f;
		float Shadow = k + (1.0f - k) * v3dNormal.dot(ZAxis);
		if(Shadow < k) Shadow = k;
		if(Shadow > 1.0f) Shadow = 1.0f;
		pShadows[nVertex] += Shadow;
	}
}

// ================================================================================
// VertexBasedMesh::CastShadows
// ================================================================================
void VertexBasedMesh::CastShadows(const CVectors3D &rSources, float k,
				int ShadowMinLevel, int ShadowMaxLevel, float maxShadow)
{
	const int ZBufferSide = 2048;
	float *pShadows = NULL;
	int *pZBuffer = NULL;
	Vector3D *pTVertices = NULL;

	// Initialization:
	pShadows = new float[Vert.NValues];
	assert(pShadows != NULL);
	if(!pShadows) goto CastShadows_unInit;
	ZeroMemory(pShadows, Vert.NValues * sizeof(float));
	pZBuffer = new int[ZBufferSide * ZBufferSide];
	assert(pZBuffer != NULL);
	if(!pZBuffer) goto CastShadows_unInit;
	pTVertices = new Vector3D[Vert.NValues];
	assert(pTVertices != NULL);
	if(!pTVertices) goto CastShadows_unInit;

	// Casting:
	for(int nSource = 0; nSource < rSources.NValues; nSource++)
		CastOneShadow(ZBufferSide, k, pTVertices, pZBuffer, pShadows, rSources[nSource]);

	// Shadowing vertices:
	for(int nVertex = 0; nVertex < Vert.NValues; nVertex++)
	{
		float s = pShadows[nVertex] * 128 / maxShadow;
		if(s < ShadowMinLevel) s = ShadowMinLevel;
		if(s > ShadowMaxLevel) s = ShadowMaxLevel;
		Vert[nVertex].Shadow = s;
	}

	// unInit:
CastShadows_unInit:
	if(pShadows) delete[] pShadows;
	if(pZBuffer) delete[] pZBuffer;
	if(pTVertices) delete[] pTVertices;
}

// ================================================================================
// VertexBasedMeshRef::CreateVertexBasedMesh
// ================================================================================
void VertexBasedMeshRef::CreateVertexBasedMesh(VertexBasedMesh *pVBMesh,
											   Hash<OneMapVertexHash> *ppVBMesh_VertHash)
{
	int NTri = 0;
	NTri = nTriangles.GetAmount();
	if(NTri == 0) return;
	OneMapVertexHash vtxh;
	OneMapVertex vtx;
	int i = 0, oi0 = 0, oi1 = 0, oi2 = 0, nTri = 0, nvtx = 0,
		i0 = 0, i1 = 0, i2 = 0;
	for(i = 0; i < NTri; i++)
	{
		nTri = nTriangles[i];
		oi0 = pOrigin->Idxs[3 * nTri + 0],
			oi1 = pOrigin->Idxs[3 * nTri + 1],
			oi2 = pOrigin->Idxs[3 * nTri + 2];
		// oi0
		vtxh.oi = oi0;
		i0 = ppVBMesh_VertHash->find(vtxh);
		if(i0 == NO_ELEMENT)
		{
			vtx = pOrigin->Vert[oi0];
			pVBMesh->Vert.Add(vtx);
			i0 =  nvtx;
			vtxh.i = i0;
			ppVBMesh_VertHash->add(vtxh);
			nvtx++;
		}
		else
		{
			vtxh = ppVBMesh_VertHash->elem(i0);
			i0 = vtxh.i;
		}
		pVBMesh->Idxs.Add(i0);
		// oi1
		vtxh.oi = oi1;
		i1 = ppVBMesh_VertHash->find(vtxh);
		if(i1 == NO_ELEMENT)
		{
			vtx = pOrigin->Vert[oi1];
			pVBMesh->Vert.Add(vtx);
			i1 = nvtx;
			vtxh.i = i1;
			ppVBMesh_VertHash->add(vtxh);
			nvtx++;
		}
		else
		{
			vtxh = ppVBMesh_VertHash->elem(i1);
			i1 = vtxh.i;
		}
		pVBMesh->Idxs.Add(i1);
		// oi2
		vtxh.oi = oi2;
		i2 = ppVBMesh_VertHash->find(vtxh);
		if(i2 == NO_ELEMENT)
		{
			vtx = pOrigin->Vert[oi2];
			pVBMesh->Vert.Add(vtx);
			i2 = nvtx;
			vtxh.i = i2;
			ppVBMesh_VertHash->add(vtxh);
			nvtx++;
		}
		else
		{
			vtxh = ppVBMesh_VertHash->elem(i2);
			i2 = vtxh.i;
		}
		pVBMesh->Idxs.Add(i2);
	}
}

// CCalcNormalsHashEntry, CCalcNormalsHashTable
class CCalcNormalsHashEntry
{
public:
	DWORD oi; // oi - index in origin
	float nx, ny, nz;
	OneMapVertex *pVertex;
	CCalcNormalsHashEntry() { oi = 0, nx = ny = nz = 0, pVertex = NULL; }
	unsigned int hash() const { return oi; }
	bool equal(const CCalcNormalsHashEntry &rEntry) { return oi == rEntry.oi; }
	void copy(const CCalcNormalsHashEntry &rEntry) { *this = rEntry; }
};
typedef Hash<CCalcNormalsHashEntry> CCalcNormalsHashTable;

// ================================================================================
// VertexBasedMeshRef::CalcNormals
// ================================================================================
void VertexBasedMeshRef::CalcNormals(void)
{
	DynArray<Vector3D> TriNormals;
	// Calculating normals in triangles:
	int nTri, i0, i1, i2;
	for(int i = 0; i < nTriangles.NValues; i++)
	{
		nTri = nTriangles[i];
		i0 = pOrigin->Idxs[3 * nTri + 0], i1 = pOrigin->Idxs[3 * nTri + 1], i2 = pOrigin->Idxs[3 * nTri + 2];
		OneMapVertex v0 = pOrigin->Vert[i0], v1 = pOrigin->Vert[i1], v2 = pOrigin->Vert[i2];
		v1.x -= v0.x;
		v1.y -= v0.y;
		v1.z -= v0.z;
		v2.x -= v0.x;
		v2.y -= v0.y;
		v2.z -= v0.z;
		Vector3D Normal(v2.y * v1.z - v1.y * v2.z, -(v1.x * v2.z + v2.x * v1.z), v2.x * v1.y - v1.x * v2.y);
		Normal.normalize();
		TriNormals.Add(Normal);
	}
} // VertexBasedMeshRef::CalcNormals

// ================================================================================
// VertexBasedMeshRef::SupplementNormals
// ================================================================================
void VertexBasedMeshRef::SupplementNormals(int cx, int cy, int r)
{
	int N = nTriangles.GetAmount();
	if(N == 0) return;
	// Calculating normals in triangles
	DynArray<Vector3D> v3dTriNormals;
	int i = 0, i0 = 0, i1 = 0, i2 = 0, nTri = 0;
	float norm = 0;
	Vector3D v3dNormal;
	for(i = 0; i < N; i++)
	{
		nTri = nTriangles[i];
		i0 = pOrigin->Idxs[3 * nTri + 0],
			i1 = pOrigin->Idxs[3 * nTri + 1],
			i2 = pOrigin->Idxs[3 * nTri + 2];
		OneMapVertex vtx0 = pOrigin->Vert[i0],
			vtx1 = pOrigin->Vert[i1],
			vtx2 = pOrigin->Vert[i2];
		vtx1.x -= vtx0.x;
		vtx1.y -= vtx0.y;
		vtx1.z -= vtx0.z;
		vtx2.x -= vtx0.x;
		vtx2.y -= vtx0.y;
		vtx2.z -= vtx0.z;
		v3dNormal.x = - vtx1.y * vtx2.z + vtx2.y * vtx1.z;
		v3dNormal.y = vtx1.x * vtx2.z - vtx2.x * vtx1.z;
		v3dNormal.z = - vtx1.x * vtx2.y + vtx2.x * vtx1.y;
		norm = v3dNormal.norm();
		v3dNormal.x /= norm;
		v3dNormal.y /= norm;
		v3dNormal.z /= norm;
		v3dTriNormals.Add(v3dNormal);
	}
	// Extracting vertices, in which we should recalc normals
	int r2 = r * r, l2 = 0;
	CCalcNormalsHashTable VerticesHash;
	OneMapVertex *pvtx0 = NULL, *pvtx1 = NULL, *pvtx2 = NULL;
	CCalcNormalsHashEntry Entry;
	for(i = 0; i < N; i++)
	{
		nTri = nTriangles[i];
		i0 = pOrigin->Idxs[3 * nTri + 0],
			i1 = pOrigin->Idxs[3 * nTri + 1],
			i2 = pOrigin->Idxs[3 * nTri + 2];
		pvtx0 = &pOrigin->Vert[i0],
			pvtx1 = &pOrigin->Vert[i1],
			pvtx2 = &pOrigin->Vert[i2];
		// pvtx0
		Entry.oi = i0;
		if(VerticesHash.find(Entry) == NO_ELEMENT)
		{
			l2 = (cx - pvtx0->x) * (cx - pvtx0->x) +
				(cy - pvtx0->y) * (cy - pvtx0->y);
			if(l2 <= r2)
			{
				Entry.pVertex = pvtx0;
				VerticesHash.add(Entry);
			}
		}
		// pvtx1
		Entry.oi = i1;
		if(VerticesHash.find(Entry) == NO_ELEMENT)
		{
			l2 = (cx - pvtx1->x) * (cx - pvtx1->x) +
				(cy - pvtx1->y) * (cy - pvtx1->y);
			if(l2 <= r2)
			{
				Entry.pVertex = pvtx1;
				VerticesHash.add(Entry);
			}
		}
		// pvtx2
		Entry.oi = i2;
		if(VerticesHash.find(Entry) == NO_ELEMENT)
		{
			l2 = (cx - pvtx2->x) * (cx - pvtx2->x) +
				(cy - pvtx2->y) * (cy - pvtx2->y);
			if(l2 <= r2)
			{
				Entry.pVertex = pvtx2;
				VerticesHash.add(Entry);
			}
		}
	}
	Entry.pVertex = NULL;
	// Enumerating all triangles and calculating normals in vertices
	int n = 0;
	for(i = 0; i < N; i++)
	{
		nTri = nTriangles[i];
		v3dNormal = v3dTriNormals[i];
		i0 = pOrigin->Idxs[3 * nTri + 0],
			i1 = pOrigin->Idxs[3 * nTri + 1],
			i2 = pOrigin->Idxs[3 * nTri + 2];
		Entry.oi = i0;
		n = VerticesHash.find(Entry);
		if(n != NO_ELEMENT)
		{
			CCalcNormalsHashEntry &rEntry = VerticesHash.elem(n);
			rEntry.nx += v3dNormal.x;
			rEntry.ny += v3dNormal.y;
			rEntry.nz += v3dNormal.z;
		}
		Entry.oi = i1;
		n = VerticesHash.find(Entry);
		if(n != NO_ELEMENT)
		{
			CCalcNormalsHashEntry &rEntry = VerticesHash.elem(n);
			rEntry.nx += v3dNormal.x;
			rEntry.ny += v3dNormal.y;
			rEntry.nz += v3dNormal.z;
		}
		n = VerticesHash.find(Entry);
		if(n != NO_ELEMENT)
		{
			CCalcNormalsHashEntry &rEntry = VerticesHash.elem(n);
			rEntry.nx += v3dNormal.x;
			rEntry.ny += v3dNormal.y;
			rEntry.nz += v3dNormal.z;
		}
	}
	// Enumerating entries in VerticesHash, normilizing normals and storing
	N = VerticesHash.numElem();
	for(i = 0; i < N; i++)
	{
		Entry = VerticesHash.elem(i);
		v3dNormal.x = Entry.nx,
			v3dNormal.y = Entry.ny,
			v3dNormal.z = Entry.nz;
		norm = v3dNormal.norm();
		v3dNormal.x /= norm;
		v3dNormal.y /= norm;
		v3dNormal.z /= norm;
		v3dNormal.x *= 127,
			v3dNormal.y *= 127,
			v3dNormal.z *= 127;
		Entry.pVertex->nx = v3dNormal.x,
			Entry.pVertex->ny = v3dNormal.y,
			Entry.pVertex->nz = v3dNormal.z;
	}
	// unInit...
	VerticesHash.reset();
	v3dTriNormals.Clear();
}

// ================================================================================
// UniversalMap::ChangeHeightParabolic
// ================================================================================
void UniversalMap::ChangeHeightParabolic(int cx, int cy, int r, int dH)
{
	VertexBasedMeshRef VBMRef;
	GetMeshRefCylinder(cx, cy, r, &VBMRef);
	DynArray<OneMapVertex*> pVertices;
	VBMRef.ExtractUniqueVertices(&pVertices);
	// Enumerating all vertices and changing high parabolic
	int N = 0;
	N = pVertices.GetAmount();
	if(N == 0) return;
	OneMapVertex *pvtx = NULL;
	float l = 0, r2 = r * r;
	for(int i = 0; i < N; i++)
	{
		pvtx = pVertices[i];
		l = sqrt((cx - pvtx->x) * (cx - pvtx->x) +
			(cy - pvtx->y) * (cy - pvtx->y));
		if(l <= r) pvtx->z -= ((l * l) / r2 - 1) * dH;
	}
	pvtx = NULL;
	// Invalidating...
	VBMRef.SupplementNormals(cx, cy, r);
	SupplementMapOfHeightsInRect(VBMRef.x0, VBMRef.y0, VBMRef.x0 + VBMRef.w, VBMRef.y0 + VBMRef.h);
	VBMRef.SupplementShadow(m_pSun);
	Rct rc(VBMRef.x0, VBMRef.y0, VBMRef.w, VBMRef.h);
	ITerra->InvalidateGeometry(&rc);
}

// ================================================================================
// UniversalMap::ChangeHeightPlanar
// ================================================================================
void UniversalMap::ChangeHeightPlanar(int cx, int cy, int r, int dH)
{
	VertexBasedMeshRef VBMRef;
	GetMeshRefCylinder(cx, cy, r, &VBMRef);
	DynArray<OneMapVertex*> pVertices;
	VBMRef.ExtractUniqueVertices(&pVertices);
	// Enumerating all vertices and changing high planar
	int N = 0;
	N = pVertices.GetAmount();
	if(N == 0) return;
	OneMapVertex *pvtx = NULL;
	for(int i = 0; i < N; i++)
	{
		pvtx = pVertices[i];
		float l = sqrt((cx - pvtx->x) * (cx - pvtx->x) +
			(cy - pvtx->y) * (cy - pvtx->y));
		if(l <= r) pvtx->z += dH;
	}
	pvtx = NULL;
	// Invalidating...
	SupplementMapOfHeightsInRect(VBMRef.x0, VBMRef.y0, VBMRef.x0 + VBMRef.w, VBMRef.y0 + VBMRef.h);
	VBMRef.SupplementNormals(cx, cy, r);
	VBMRef.SupplementShadow(m_pSun);
	Rct rc(VBMRef.x0, VBMRef.y0, VBMRef.w, VBMRef.h);
	ITerra->InvalidateGeometry(&rc);
}

// ================================================================================
// UniversalMap::EvenHeightInRadius
// ================================================================================
void UniversalMap::EvenHeightInRadius(int cx, int cy, int r)
{
	VertexBasedMeshRef VBMRef;
	GetMeshRefCylinder(cx, cy, r, &VBMRef);
	// Calculating base high
	int N = 0;
	N = VBMRef.nTriangles.GetAmount();
	if(N == 0) return;
	int i = 0, i0 = 0, i1 = 0, i2 = 0, nTri = 0;
	const float cNotIntersected = -9999.99f;
	float Z = cNotIntersected, T = 0;
	for(i = 0; i < N; i++)
	{
		nTri = VBMRef.nTriangles[i];
		i0 = Idxs[3 * nTri + 0],
			i1 = Idxs[3 * nTri + 1],
			i2 = Idxs[3 * nTri + 2];
		OneMapVertex &rvtx0 = Vert[i0],
			&rvtx1 = Vert[i1],
			&rvtx2 = Vert[i2];
		Vector3D v3dA, v3dB, v3dC;
		// v3dA
		v3dA.x = rvtx0.x - cx,
			v3dA.y = rvtx0.y - cy,
			v3dA.z = rvtx0.z;
		// v3dB
		v3dB.x = rvtx1.x - cx,
			v3dB.y = rvtx1.y - cy,
			v3dB.z = rvtx1.z;
		// v3dC
		v3dC.x = rvtx2.x - cx,
			v3dC.y = rvtx2.y - cy,
			v3dC.z = rvtx2.z;
		if(zXTri(&T, v3dA, v3dB, v3dC) == true)
			if(T > Z) Z = T;
	}
	if(Z == cNotIntersected) return;
	// Extracting and then enumerating all vertices and setting z to base high Z
	DynArray<OneMapVertex*> pVertices;
	VBMRef.ExtractUniqueVertices(&pVertices);
	N = pVertices.GetAmount();
	if(N == 0) return;
	OneMapVertex *pvtx = NULL;
	for(i = 0; i < N; i++)
	{
		pvtx = pVertices[i];
		pvtx->z = Z;
	}
	pvtx = NULL;
	// Invalidating...
	int offset = 100;
	SupplementMapOfHeightsInRect(VBMRef.x0 - offset, VBMRef.y0 - offset,
		VBMRef.x0 + VBMRef.w + 2 * offset, VBMRef.y0 + VBMRef.h + 2 * offset);
	VBMRef.SupplementNormals(cx, cy, r);
	VBMRef.SupplementShadow(m_pSun);
	Rct rc(VBMRef.x0, VBMRef.y0, VBMRef.w, VBMRef.h);
	ITerra->InvalidateGeometry(&rc);
}

// ================================================================================
// UniversalMap::SmoothHeightInRadius
// ================================================================================
void UniversalMap::SmoothHeightInRadius(int cx, int cy, int r)
{
	VertexBasedMeshRef VBMRef;
	GetMeshRefCylinder(cx, cy, r, &VBMRef);
	VBMRef.Smooth(0.5, 2);
	// Invalidating...
	SupplementMapOfHeightsInRect(VBMRef.x0, VBMRef.y0, VBMRef.x0 + VBMRef.w, VBMRef.y0 + VBMRef.h);
	VBMRef.SupplementNormals(cx, cy, r);
	VBMRef.SupplementShadow(m_pSun);
	Rct rc(VBMRef.x0, VBMRef.y0, VBMRef.w, VBMRef.h);
	ITerra->InvalidateGeometry(&rc);
}

void VertexBasedMesh::InvertNormals(void)
{
	int N = Idxs.NValues / 3;
	for(int i = 0; i < N; i++) swap(Idxs[3 * i + 1], Idxs[3 * i + 2]);
}

// ================================================================================
// ITCreateGeometry
// ================================================================================
bool ITCreateGeometry(const Rct &exRect, int LOD)
{
	// Caching values from g_UniversalMap:
	int QuadX0 = g_UniversalMap.QuadX0, QuadY0 = g_UniversalMap.QuadY0,
		QuadLx = g_UniversalMap.QuadLx, QuadLy = g_UniversalMap.QuadLy,
		QuadsNx = g_UniversalMap.QuadsNx, QuadsNy = g_UniversalMap.QuadsNy;
	// Additional values:
	int x = (int)exRect.x, y = (int)exRect.y, w = (int)exRect.w, l = (int)exRect.h,
		UMLx = QuadLx * QuadsNx,
		UMLy = QuadLy * QuadsNy;
	// X, Y - center point of mini quad
	int X = x + (QuadLx >> 1), Y = y + (QuadLy >> 1);
	// is X, Y within g_UniversalMap?
	if((X < QuadX0) || (X >= QuadX0 + UMLx) ||
		(Y < QuadY0) || (Y >= QuadY0 + UMLy)) return true;
	// Calculating number of mini quad:
	int nQx = (X - QuadX0) / QuadLx, nQy = (Y - QuadY0) / QuadLy;
	int nQ = nQx + g_UniversalMap.QuadsNx * nQy;
	VertexBasedMeshRef VBMRef;
	VBMRef.pOrigin = g_UniversalMap.m_pQuads[nQ]->pOrigin;
	// Merging mini quads to VBMRef (to same size as needed):
	int NQx = w / QuadLx, NQy = l / QuadLy; // How many mini quads in needed?
	for(int j = 0; j < NQy; j++)
		for(int i = 0; i < NQx; i++)
		{
			int m_nQx = nQx + i, m_nQy = nQy + j; // numbers of mini quads
			if((m_nQx < QuadsNx) && (m_nQy < QuadsNy))
			{
				nQ = m_nQx + QuadsNx * m_nQy;
				VBMRef.nTriangles.AddValues(g_UniversalMap.m_pQuads[nQ]->nTriangles.GetValues(),
					g_UniversalMap.m_pQuads[nQ]->nTriangles.GetAmount());
			}
		}
	// Splitting VBMRef:
	OneSplitMesh OSMesh;
	OSMesh.SpeedySplit(VBMRef);

	// Copying data to base meshes, which are granted by ITerra:
	// (D - destination, S - source)
	int N = OSMesh.pBaseMeshes.GetAmount();
	for(int nMesh = 0; nMesh < N; nMesh++)
	{
		BaseMesh *pBMeshS = OSMesh.pBaseMeshes[nMesh];
		int NVertices = pBMeshS->getNVert(),
			NIndices = pBMeshS->getNInd();
		BaseMesh *pBMeshD = ITerra->AllocateGeometry();
		if(pBMeshD == NULL)
		{
			Log.Error("ITerra->AllocateGeometry() FAILED!");
			return true;
		}
		pBMeshD->create(NVertices, NIndices, vfVertexTS);
		VERTEX *pvtxDataD = (VERTEX*)pBMeshD->getVertexData(),
			*pvtxDataS = (VERTEX*)pBMeshS->getVertexData();
		word *pIndicesD = pBMeshD->getIndices(),
			*pIndicesS = pBMeshS->getIndices();
		if((pvtxDataD == NULL) || (pvtxDataS == NULL) ||
			(pIndicesD == NULL) || (pIndicesS == NULL))
		{
			Log.Error("NULL in pvtxDataD, pvtxDataS, pIndicesD or pIndicesS!");
			return true;
		}
		// Essentially copying:
		memcpy(pvtxDataD, pvtxDataS, sizeof(VERTEX) * NVertices);
		memcpy(pIndicesD, pIndicesS, sizeof(word) * NIndices);
		// Finalizing BaseMesh:
		pBMeshD->setNVert(NVertices);
		pBMeshD->setNInd(NIndices);
		pBMeshD->setNPri(NIndices / 3);
		pBMeshD->setTexture(pBMeshS->getTexture());
		pBMeshD->setTexture(pBMeshS->getTexture(1), 1);
		pBMeshD->setShader(pBMeshS->getShader());
		// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		if(g_UniversalMap.bWire)
		{
//			pBMeshD->setTexture(IRS->GetTextureID("white.tga"));
			pBMeshD->setShader(IRS->GetShaderID("terra_wire"));
		}
		// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	}
	return true;
}

short UniversalMap::THMapZ(int MapCoordX, int MapCoordY)
{
	int Lx = 32 * VertInLine, Ly = 32 * MaxTH;
	if(MapCoordX >= 0 && MapCoordX < Lx && MapCoordY >= 0 && MapCoordY < Ly)
	{
		int nx = MapCoordX / 32, ny = MapCoordY / 32;
		int z0 = THMap[nx + VertInLine * ny],
			z1 = THMap[(nx + 1) + VertInLine * ny],
			z2 = THMap[nx + VertInLine * (ny + 1)],
			z3 = THMap[(nx + 1) + VertInLine * (ny + 1)];
		return short((z0 + z1 + z2 + z3) / 4);
	}
	return 0;
}

inline void VertexBasedMesh::GetTriangle(DWORD nTri, OneMapVertex &v0, OneMapVertex &v1, OneMapVertex &v2) const
{
	int i0 = Idxs[3 * nTri + 0], i1 = Idxs[3 * nTri + 1], i2 = Idxs[3 * nTri + 2];
	v0 = Vert[i0], v1 = Vert[i1], v2 = Vert[i2];
}

// ================================================================================
// UniversalMap::SectionVertexClassify2D
// ================================================================================
#define VERTEX_ORIGIN		0
#define VERTEX_DESTINATION	1
#define VERTEX_LEFT			2
#define VERTEX_RIGHT		3
#define VERTEX_BELONGS		4
#define VERTEX_BEHIND		5
#define VERTEX_BEYOND		6
int UniversalMap::SectionVertexClassify2D(const OneMapVertex &rSectionA, const OneMapVertex &rSectionB,
										  const OneMapVertex &rVertex)
{
	double ax = rSectionB.x - rSectionA.x, ay = rSectionB.y - rSectionA.y,
		bx = rVertex.x - rSectionA.x, by = rVertex.y - rSectionA.y;
	double s = ay * bx - ax * by;
	if(s > 0.0) return VERTEX_LEFT;
	if(s < 0.0) return VERTEX_RIGHT;
	if((ax * bx < 0.0f) || (ay * by < 0.0)) return VERTEX_BEHIND;
	if((ax * ax + ay * ay) < (bx * bx + by * by)) return VERTEX_BEYOND;
	if((rVertex.x == rSectionA.x) && (rVertex.y == rSectionA.y)) return VERTEX_ORIGIN;
	if((rVertex.x == rSectionB.x) && (rVertex.y == rSectionB.y)) return VERTEX_DESTINATION;
	return VERTEX_BELONGS;
} // UniversalMap::SectionVertexClassify2D

#define VERTEX_INSIDE	7
#define VERTEX_OUTSIDE	8
int UniversalMap::RectVertexClassify2D(const OneMapVertex &rv0, const OneMapVertex &rv1,
									   const OneMapVertex &rv2, const OneMapVertex &rv3,
									   const OneMapVertex &rVertex)
{
	OneMapVertex v[4] = {rv0, rv1, rv2, rv3};
	for(int i = 0; i < 4; i++)
	{
		int r = SectionVertexClassify2D(v[i], v[(i + 1) % 4], rVertex);
		if(r == VERTEX_BELONGS || r == VERTEX_ORIGIN || r == VERTEX_DESTINATION) return VERTEX_BELONGS;
		if(r != VERTEX_LEFT) return VERTEX_OUTSIDE;
	}
	return VERTEX_INSIDE;
}

// ================================================================================
// UniversalMap::CtrlC
// ================================================================================
inline bool UniversalMap::CtrlCV_PtInRect(int x0, int y0, int Width, int Depth, OneMapVertex &rVertex)
{
	int x1 = x0 + Width, y1 = y0 + Width;
	return (rVertex.x >= x0) && (rVertex.x < x1) && (rVertex.y >= y0) && (rVertex.y < y1);
}
inline bool UniversalMap::CtrlCV_PtInRound(int cx, int cy, int r, OneMapVertex &rVertex)
{
	int r2 = r * r, dx = int(rVertex.x) - cx, dy = int(rVertex.y) - cy;
	return dx * dx + dy * dy < r2;
}
void UniversalMap::CtrlCV_Update(int *oi, int *i, OneMapVertex *v,
								COneMapVertices &rVertices, nHashTable &rTable, int *flags)
{
	nHashTableEntry hEntry;
	for(int k = 0; k < 3; k++)
		if(flags[k] == k)
		{
			hEntry.i = oi[k];
			i[k] = rTable.find(hEntry);
			if(i[k] == NO_ELEMENT)
			{
				rVertices.Add(v[k]);
				i[k] = rTable.add(hEntry);
			}
		}
}
const int UniversalMap::c_CtrlCVOffset = 512;
const int UniversalMap::c_CtrlCVrMin = 250;

void UniversalMap::QuadsSystem_AddRef(const OneMapVertex &v0, const OneMapVertex &v1, const OneMapVertex &v2, int nTri)
{
	int UMLx = QuadLx * QuadsNx, UMLy = QuadLy * QuadsNy, cx = (int(v0.x) + int(v1.x) + int(v2.x)) / 3,
		cy = (int(v0.y) + int(v1.y) + int(v2.y)) / 3;
	if((cx >= QuadX0) && (cx < QuadX0 + UMLx) && (cy >= QuadY0) && (cy < QuadY0 + UMLy))
	{
		int nQx = (cx - QuadX0) / QuadLx, nQy = (cy - QuadY0) / QuadLy;
		m_pQuads[nQx + QuadsNx * nQy]->nTriangles.Add(nTri);
	}
}

bool UniversalMap::CtrlCV_Loop(CIndices &rPairs, CIndices &rContour)
{
	if(!rPairs.NValues) return false;
	int Begin = rPairs[0], End = rPairs[1];
	rContour.Add(Begin);
	rPairs.Del(0, 2);
	int l = 0;
	while(true)
	{
		if(!rPairs.NValues) break;
		if(Begin == End) break;
		l = rPairs.find(End);
		if(l == -1) return false;
		if(l & 1) // B
		{
			rContour.Add(End);
			End = rPairs[l - 1];
			rPairs.Del(l - 1, 2);
		}
		else // A
		{
			rContour.Add(End);
			End = rPairs[l + 1];
			rPairs.Del(l, 2);
		}
	}
	return true;
}

bool VertexBasedMesh::CalcZ(const OneMapVertex &v, short &rZ) const
{
	const float cNotIntersected = -999.99f;
	float Z = cNotIntersected, z = 0;
	int NTri = Idxs.NValues / 3;
	OneMapVertex v0, v1, v2;
	for(int nTri = 0; nTri < NTri; nTri++)
	{
		GetTriangle(nTri, v0, v1, v2);
		Vector3D v3dA(v0.x - v.x, v0.y - v.y, v0.z), v3dB(v1.x - v.x, v1.y - v.y, v1.z),
			v3dC(v2.x - v.x, v2.y - v.y, v2.z);
		if((v3dA.x > 0 && v3dB.x > 0 && v3dC.x > 0) || (v3dA.x < 0 && v3dB.x < 0 && v3dC.x < 0) ||
			(v3dA.y > 0 && v3dB.y > 0 && v3dC.y > 0) || (v3dA.y < 0 && v3dB.y < 0 && v3dC.y < 0)) continue;
		if(zXTri(&z, v3dA, v3dB, v3dC) == true) if(z > Z) Z = z;
	}
	if(Z != cNotIntersected) { rZ = (short)Z; return true; }
	return false;
}

void VertexBasedMesh::GetMeshRefCylinder(int cx, int cy, int r, VertexBasedMeshRef &rRef)
{
	rRef.Release();
	rRef.pOrigin = this;
	int NTri = Idxs.NValues / 3, r2 = r * r;
	for(int nTri = 0; nTri < NTri; nTri++)
	{
		int i0 = Idxs[3 * nTri + 0], i1 = Idxs[3 * nTri + 1], i2 = Idxs[3 * nTri + 2];
		OneMapVertex &v0 = Vert[i0], &v1 = Vert[i1], &v2 = Vert[i2];
		int x = (int(v0.x) + int(v1.x) + int(v2.x)) / 3,
			y = (int(v0.y) + int(v1.y) + int(v2.y)) / 3;
		int d = (x - cx) * (x - cx) + (y - cy) * (y - cy);
		if(d < r2) rRef.nTriangles.Add(nTri);
	}
}

void VertexBasedMesh::GetTriangleCentre(DWORD nTri, OneMapVertex &c) const
{
	int i0 = Idxs[3 * nTri + 0], i1 = Idxs[3 * nTri + 1], i2 = Idxs[3 * nTri + 2];
	OneMapVertex &v0 = Vert[i0], &v1 = Vert[i1], &v2 = Vert[i2];
	int cx = (int(v0.x) + int(v1.x) + int(v2.x)) / 3,
		cy = (int(v0.y) + int(v1.y) + int(v2.y)) / 3,
		cz = (int(v0.z) + int(v1.z) + int(v2.z)) / 3;
	c.x = short(cx), c.y = short(cy), c.z = short(cz);
}

void UniversalMap::AlignWithStencil(VertexBasedMesh &rMesh, VertexBasedMesh &rStencil, int cx, int cy, int R)
{
	const short lInvariableOffset = 100, lSmoothOffset = lInvariableOffset + 100;
	int RInvariable = R - lInvariableOffset, RConst = R - lSmoothOffset;

	// Deleting unnecessary triangles in rStencil:
	DynArray<bool> inInvariableStencil;
	inInvariableStencil.Add(false, rStencil.Vert.NValues);
	for(int i = 0; i < rStencil.Vert.NValues; i++)
	{
		OneMapVertex &v = rStencil.Vert[i];
		inInvariableStencil[i] = CtrlCV_PtInRound(cx, cy, RInvariable, v);
	}
	int NTri = rStencil.Idxs.NValues / 3;
	CIndices newStencilIndices;
	for(int nTri = 0; nTri < NTri; nTri++)
	{
		int i0 = rStencil.Idxs[3 * nTri + 0], i1 = rStencil.Idxs[3 * nTri + 1], i2 = rStencil.Idxs[3 * nTri + 2];
		if(!inInvariableStencil[i0] || !inInvariableStencil[i1] || !inInvariableStencil[i2])
			newStencilIndices.Add(i0), newStencilIndices.Add(i1), newStencilIndices.Add(i2);
	}
	rStencil.Idxs.Clear();
	rStencil.Idxs.AddValues(newStencilIndices.GetValues(), newStencilIndices.NValues);
	newStencilIndices.Clear();
	inInvariableStencil.Clear();

	// Splitting vertices by radiuses and calculating Z for necessary vertices:
	DynArray<bool> inConst, inInvariable;
	inConst.Add(false, rMesh.Vert.NValues), inInvariable.Add(false, rMesh.Vert.NValues);
	DynArray<short> AllZ;
	AllZ.Add(SHRT_MIN, rMesh.Vert.NValues);
	short Z;
	for(int i = 0; i < rMesh.Vert.NValues; i++)
	{
		OneMapVertex &v = rMesh.Vert[i];
		if(CtrlCV_PtInRound(cx, cy, RConst, v)) inConst[i] = true, inInvariable[i] = true;
		else if(CtrlCV_PtInRound(cx, cy, RInvariable, v)) inInvariable[i] = true;
		if(!inInvariable[i])
			if(rStencil.CalcZ(v, Z)) AllZ[i] = Z;
	}

	// Considering vertices, for which Z cannot be calculated,
	// forming Ref for further smoothing:
	VertexBasedMeshRef Ref;
	Ref.pOrigin = &rMesh;
	NTri = rMesh.Idxs.NValues / 3;
	OneMapVertex c;
	for(int nTri = 0; nTri < NTri; nTri++)
	{
		int i0 = rMesh.Idxs[3 * nTri + 0], i1 = rMesh.Idxs[3 * nTri + 1], i2 = rMesh.Idxs[3 * nTri + 2];
		if(!inInvariable[i0] && !inInvariable[i1] && !inInvariable[i2])
		{
			if(AllZ[i0] == SHRT_MIN)
			{
				if(AllZ[i1] != SHRT_MIN) AllZ[i0] = AllZ[i1];
				if(AllZ[i2] != SHRT_MIN) AllZ[i0] = AllZ[i2];
			}
			if(AllZ[i1] == SHRT_MIN)
			{
				if(AllZ[i0] != SHRT_MIN) AllZ[i1] = AllZ[i0];
				if(AllZ[i2] != SHRT_MIN) AllZ[i1] = AllZ[i2];
			}
			if(AllZ[i2] == SHRT_MIN)
			{
				if(AllZ[i0] != SHRT_MIN) AllZ[i2] = AllZ[i0];
				if(AllZ[i1] != SHRT_MIN) AllZ[i2] = AllZ[i1];
			}
		}
		if(!inConst[i0] && !inConst[i1] && !inConst[i2]) Ref.nTriangles.Add(nTri);
	}

	for(i = 0; i < inInvariable.NValues; i++) if(!inInvariable[i]) rMesh.Vert[i].z = AllZ[i];
	// Smoothing:
	for(i = 0; i < 30; i++) Ref.Smooth(0.5, 2);

	// unInit:
	inConst.Clear(), inInvariable.Clear();
	Ref.Release();
	AllZ.Clear();
}

void UniversalMap::Sum(const OneMapVertex &v0, const OneMapVertex &v1, const OneMapVertex &v2, int *pnTri)
{
	int V = Vert.NValues, nTri = Idxs.NValues / 3;
	Vert.Add(v0), Vert.Add(v1), Vert.Add(v2);
	for(int i = 0; i < 3; i++) Idxs.Add(V + i);
	QuadsSystem_AddRef(v0, v1, v2, nTri);
	if(pnTri) *pnTri = nTri;
}

void UniversalMap::Sum(int i0, int i1, int i2)
{
	if(i0 < 0 || i1 < 0 || i2 < 0) return;
	if(i0 >= Idxs.NValues || i1 >= Idxs.NValues || i2 >= Idxs.NValues) return;
	int nTri = Idxs.NValues / 3;
	OneMapVertex &v0 = Vert[i0], &v1 = Vert[i1], &v2 = Vert[i2];
	Idxs.Add(i0), Idxs.Add(i1), Idxs.Add(i2);
	QuadsSystem_AddRef(v0, v1, v2, nTri);
}

void UniversalMap::QuadsSystem_Init(void)
{
	QuadsSystem_unInit();
	const int D = 128;
	QuadLx = D, QuadLy = D;
	OneMapVertex MMin = Vert[0], MMax = Vert[0];
	for(int i = 1; i < Vert.NValues; i++) CalcMinMax(Vert[i], MMin, MMax);
	QuadX0 = MMin.x, QuadY0 = MMin.y;
	int Dx = MMax.x - MMin.x + 1, Dy = MMax.y - MMin.y + 1;
	QuadsNx = Dx / QuadLx, QuadsNy = Dy / QuadLy;
	if(QuadsNx * QuadLx < Dx) QuadsNx++;
	if(QuadsNy * QuadLy < Dy) QuadsNy++;
	UMLx = QuadsNx * QuadLx, UMLy = QuadsNy * QuadLy;
	int NQuads = QuadsNx * QuadsNy;
	for(int nQ = 0; nQ < NQuads; nQ++)
	{
		OneUniversalMapQuad *pQ = new OneUniversalMapQuad;
		pQ->pOrigin = this;
		m_pQuads.Add(pQ);
	}
	int NTri = Idxs.NValues / 3;
	OneMapVertex v0, v1, v2;
	for(int nTri = 0; nTri < NTri; nTri++)
	{
		GetTriangle(nTri, v0, v1, v2);
		int cx = (int(v0.x) + int(v1.x) + int(v2.x)) / 3,
			cy = (int(v0.y) + int(v1.y) + int(v2.y)) / 3;
		int nQx = cx / QuadLx, nQy = cy / QuadLy;
		m_pQuads[QuadsNx * nQy + nQx]->nTriangles.Add(nTri);
	}
}

void UniversalMap::QuadsSystem_unInit(void)
{
	for(int nQ = 0; nQ < m_pQuads.NValues; nQ++)
	{
		m_pQuads[nQ]->nTriangles.Clear();
		delete m_pQuads[nQ];
	}
	m_pQuads.Clear();
	QuadLx = QuadLy = 0;
	QuadsNx = QuadsNy = 0;
	QuadX0 = QuadY0 = 0;
	UMLx = UMLy = 0;
}

void UniversalMap::ArgsParser(char *str, char *tokens, float *pArgs)
{
	int N = strlen(tokens);
	char s[10][128], Buffer[128];
	int r = sscanf(str, "%s%s%s%s%s%s%s%s%s%s%s", Buffer,
		s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7], s[8], s[9]);
	for(int i = 0; i < r; i++)
	{
		strcpy(Buffer, s[i]);
		for(int k = 0; k < N; k++) if(Buffer[0] == tokens[k]) break;
		if(k == N) continue;
		strcpy(Buffer, &Buffer[1]);
		sscanf(Buffer, "%f", &pArgs[k]);
	}
}

void VertexBasedMeshRef::toVertexBasedMesh(VertexBasedMesh &rMesh)
{
	int flags[] = {0, 1, 2}, oi[3], i[3];
	OneMapVertex v[3];
	nHashTable hTable;
	for(int iTri = 0; iTri < nTriangles.NValues; iTri++)
	{
		int nTri = nTriangles[iTri];
		for(int k = 0; k < 3; k++) oi[k] = pOrigin->Idxs[3 * nTri + k], v[k] = pOrigin->Vert[oi[k]];
		UniversalMap::CtrlCV_Update(oi, i, v, rMesh.Vert, hTable, flags);
		for(k = 0; k < 3; k++) rMesh.Idxs.Add(i[k]);
	}
	hTable.reset();
}

void UniversalMap::Compile(CIndices *pContour)
{
	COneMapVertices cVertices;
	CIndices cIndices;
	nHashTable hTable;
	int flags[] = {0, 1, 2}, oi[3], i[3];
	OneMapVertex v[3];
	for(int nQ = 0; nQ < m_pQuads.NValues; nQ++)
		for(int iTri = 0; iTri < m_pQuads[nQ]->nTriangles.NValues; iTri++)
		{
			int nTri = m_pQuads[nQ]->nTriangles[iTri];
			for(int k = 0; k < 3; k++) oi[k] = Idxs[3 * nTri + k], v[k] = Vert[oi[k]];
			CtrlCV_Update(oi, i, v, cVertices, hTable, flags);
			for(k = 0; k < 3; k++) cIndices.Add(i[k]);
		}
		Vert.Clear(), Idxs.Clear();
		Vert.AddValues(cVertices.GetValues(), cVertices.NValues);
		Idxs.AddValues(cIndices.GetValues(), cIndices.NValues);
		QuadsSystem_Init();

		// Reindexing pContour:
		nHashTableEntry hEntry;
		if(pContour)
			for(int i = 0; i < pContour->NValues; i++)
			{
				hEntry.i = pContour->Values[i];
				pContour->Values[i] = hTable.find(hEntry);
			}

			hTable.reset();
			cVertices.Clear(), cIndices.Clear();
}

void VertexBasedMesh::Sum(const COneMapVertices &rVertices, const CIndices &rIndices)
{
	int V = Vert.NValues, T = Idxs.NValues / 3;
	Vert.AddValues(rVertices.GetValues(), rVertices.NValues);
	for(int i = 0; i < rIndices.NValues; i++) Idxs.Add(V + rIndices[i]);
}

void UniversalMap::Sum(VertexBasedMesh &rMesh, CIndices *pContour)
{
	int V = Vert.NValues, T = Idxs.NValues / 3;
	Vert.AddValues(rMesh.Vert.GetValues(), rMesh.Vert.NValues);
	for(int i = 0; i < rMesh.Idxs.NValues; i++) Idxs.Add(V + rMesh.Idxs[i]);
	if(pContour) for(i = 0; i < pContour->NValues; i++) pContour->Values[i] += V;
	int NTri = rMesh.Idxs.NValues / 3;
	OneMapVertex v0, v1, v2;
	for(int nTri = 0; nTri < NTri; nTri++)
	{
		rMesh.GetTriangle(nTri, v0, v1, v2);
		QuadsSystem_AddRef(v0, v1, v2, T + nTri);
	}
}

// ================================================================================
// UniversalMap::Difference
// ================================================================================
void UniversalMap::Difference(const VertexBasedMesh &rMesh)
{
	MessageBox(NULL, "Difference", "--- is called ----", 0);
}