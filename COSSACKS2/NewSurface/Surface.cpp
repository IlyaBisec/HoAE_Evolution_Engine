#include "stdheader.h"
#include "Surface.h"
#include "ITerrain.h"

// OneMapVerts::BBox
void OneMapVerts::BBox(OneMapVertex &bbMin, OneMapVertex &bbMax) const {
	bbMin = bbMax = OneMapVertex::Origin;
	if(Count()) {
		bbMin = bbMax = GetAt(0);
		for(int i = 1; i < Count(); i++) {
			const OneMapVertex &r = GetAt(i);
			bbMin.x = Min(bbMin.x, r.x);
			bbMax.x = Max(bbMax.x, r.x);
			bbMin.y = Min(bbMin.y, r.y);
			bbMax.y = Max(bbMax.y, r.y);
			bbMin.z = Min(bbMin.z, r.z);
			bbMax.z = Max(bbMax.z, r.z);
		}
	}
} // OneMapVerts::BBox

// PlanarMapper::CalcMappingCode
/*DWORD PlanarMapper::CalcMappingCode(const OneMapVertex &v) {
	int px = float(v.x) / 350 + float(v.y) / 470;
	int py = float(v.x) / 520 + float(v.y) / 430;
	return px + (py << 16);
}*/

// PlanarMapper::CalcMappingUV
/*uv_pair PlanarMapper::CalcMappingUV(const OneMapVertex &v, DWORD MappingCode, const TerraMapperParams &) {
	uv_pair uv;
	float ushift = (MappingCode & 0xFFFF) / 10.0f,
		vshift = (MappingCode >> 16) / 10.0f;
	ushift -= int(ushift);
	vshift -= int(vshift);
	uv.u = float(v.x) / 256.0f + ushift;
	uv.v = float(v.y) / 512.0f + vshift;
	return uv;
}*/

// VerticalMapper::CalcMappingCode
/*DWORD VerticalMapper::CalcMappingCode(const OneMapVertex &v) {
//	float nx = v.nx, ny = v.ny;
//	float L = sqrt(nx * nx + ny * ny);
//	nx /= L;
//	ny /= L;
	int nx = v.nx, ny = v.ny, nz = v.nz;
	// -----------------------------------------------------------
	int lx = abs(nx), ly = abs(ny), lz = abs(nz);
	const int k = 2;
	if(lz >= k * lx && lz >= k * ly) { // Top(4) or Bottom(5)
		if(nz >= 0) return 4;
		return 5;
	}
	if(lx >= ly) { // Left(1) or Right(3)
		if(nx >= 0) return 3;
		return 1;
	} else if(lx < ly) { // Back(0) or Front(2)
		if(ny >= 0) return 0;
		return 2;
	}

	/*	if(nx >= 0 && ny >= 0) { // I: 6, 7, 0
		if(nx > 2.4 * ny) return 6;
		if(ny > 2.4 * nx) return 0;
		return 7;
	}
	else
		if(nx < 0 && ny >=0) { // II: 0, 1, 2
			nx = -nx;
			if(nx > 2.4 * ny) return 2;
			if(ny > 2.4 * nx) return 0;
			return 1;
		}
		else
			if(nx < 0 && ny < 0) { // III: 2, 3, 4
				nx = -nx;
				ny = -ny;
				if(nx > 2.4 * ny) return 2;
				if(ny > 2.4 * nx) return 4;
				return 3;
			}
			else
				if(nx >= 0 && ny < 0) {	// IV: 4, 5, 6
					ny = -ny;
					if(nx > 2.4 * ny) return 6;
					if(ny > 2.4 * nx) return 4;
					return 5;
				}
				return 0;*/
//}

// VerticalMapper::CalcMappingUV
/*uv_pair VerticalMapper::CalcMappingUV(const OneMapVertex &v, DWORD MappingCode, const TerraMapperParams &Params) {
	uv_pair uv;
	float X = float(v.x) / (Params.uScale * Params.fWidth),
		Y = float(v.y) / (Params.uScale * Params.fWidth),
		Z = float(v.z) / (Params.vScale * Params.fHeight);
	if(MappingCode <= 3) { // Back, Front, Left or Right
		switch(MappingCode) {
			case 0: // Back
				uv.u = X;
				break;
			case 2: // Front
				uv.u = - X;
				break;
			case 1: // Left
				uv.u = Y;
				break;
			case 3: // Right
				uv.u = - Y;
				break;
		}
		uv.v = - Z;
	} else { // Top or Bottom
		uv.u = float(v.x) / 256.f;
		uv.v = float(v.y) / 512.f;
	}*/
/*	switch(MappingCode) {
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
	uv.u=-uv.u;*/
	//return uv;
//}

void GetUVInfo(int id, float &du, float &dv, float &su, float &sv);
int GetFacture(int);

// TerraMapper(int)
TerraMapper::TerraMapper(int nFacture) {
	float du, dv;
	GetUVInfo(nFacture, du, dv, uScale, vScale);
	fWidth = IRS->GetTextureWidth(GetFacture(nFacture));
	fHeight = IRS->GetTextureHeight(GetFacture(nFacture));
} // TerraMapper(int)

// TerraMapper::CalcMappingCode
DWORD TerraMapper::CalcMappingCode(const OneMapVertex &vtx) {
	int lx = abs(vtx.nx), ly = abs(vtx.ny), lz = abs(vtx.nz);
	if(lz >= k * lx && lz >= k * ly) { // Top or Bottom
		if(vtx.nz >= 0) return MC_TOP;
		return MC_BOTTOM;
	}
	if(lx >= ly) { // Left or Right
		if(vtx.nx >= 0) return MC_RIGHT;
		return MC_LEFT;
	} else if(lx < ly) { // Back or Front
		if(vtx.ny >= 0) return MC_BACK;
		return MC_FRONT;
	}
} // TerraMapper::CalcMappingCode

// TerraMapper::CalcMappingUV
uv_pair TerraMapper::CalcMappingUV(const OneMapVertex &vtx, DWORD MappingCode) const {
	float i = 1.f / (uScale * fWidth);
	float X = float(vtx.x) * i;
	float Y = float(vtx.y) * i;
	float Z = float(vtx.z) / (vScale * fHeight);

	uv_pair uv;
	switch(MappingCode) {
		case MC_TOP:
		case MC_BOTTOM:
			uv.u = X;
			uv.v = Y / 2.f;
			return uv;
		case MC_BACK:
			uv.u = X;
			break;
		case MC_FRONT:
			uv.u = - X;
			break;
		case MC_LEFT:
			uv.u = Y;
			break;
		case MC_RIGHT:
			uv.u = - Y;
			break;
	}
	uv.v = - Z;
	return uv;
} // TerraMapper::CalcMappingUV


// OneMapVertex::GetMapper
TerraMapper OneMapVertex::GetMapper(int nFacture) {
	return TerraMapper(nFacture);
} // OneMapVertex::GetMapper

// OneMapVertex::CalcDiffuseColor
DWORD OneMapVertex::CalcDiffuseColor() const
{	
	//return MulDWORD(Color, int(Shadow) * 2) | 0xFF000000;
	DWORD C = Color;
	int S = 2 * int(Shadow);
	DWORD R = 0;
	for(int i = 0; i < 4; i++) {
		int LowByte = C & 0xFF;
		C >>= 8;
		int CurByte = (S * LowByte) >> 8;
		if(CurByte > 0xFF) CurByte = 0xFF;
		R |= (CurByte << (i << 3));
	}
	return R | 0xFF000000;
}

// OneMapVertex::CalcSpecularColor
//DWORD OneMapVertex::CalcSpecularColor() const
//{	
//	float Contrast, Bright;
//	//int GetFactureBump(int id, float &Contrast, float &Bright);
//	//if(GetFactureBump(nFacture, Contrast, Bright) < 0) return 0xFF808080;
//	Vector3D N(nx, ny, nz);
//	N.normalize();
//	ILight* iLight = GetGameLight();
//	Vector3D L = iLight->GetDir();
//	L.reverse();
//	float dotp = L.dot(N);
//	float b = 0.5f;
//	if(dotp < 0) b += dotp;
//	if(b < 0) b = 0;	
//	float a = 0.5f - b * dotp;a*=a;b*=b;
//	Vector3D VL(a * N.x + b * L.x, a * N.y + b * L.y, a * N.z + b * L.z);
//	VL.normalize();
//	return VectorToColor(ToTangentSpace(VL, N, Vector3D(1, 0, 0)), 0);
//}
DWORD OneMapVertex::CalcSpecularColor() const
{	
	Vector3D N(nx, ny, nz);
	N.normalize();
	ILight* iLight = GetGameLight();
	Vector3D L = iLight->GetDir();
	L.reverse();
	return VectorToColor(ToTangentSpace(L, N, Vector3D(1, 0, 0)), 0);	
}

const OneMapVertex OneMapVertex::Origin(0, 0, 0);

// ~VertexBasedMeshRef
VertexBasedMeshRef::~VertexBasedMeshRef() {
	Free();
}

// VertexBasedMeshRef::Free
void VertexBasedMeshRef::Free() {
	pOrigin = NULL;
	m_TriNums.Clear();
}

//-----------------------------------------------------------------------------
// VertexBasedMeshRef::Copy : void(const VertexBasedMeshRef &)
//-----------------------------------------------------------------------------
void VertexBasedMeshRef::Copy(const VertexBasedMeshRef &Src) {
	Free();
	pOrigin = Src.pOrigin;
	m_TriNums += Src.m_TriNums;
}

//-----------------------------------------------------------------------------------
// VertexBasedMeshRef::operator = : VertexBasedMeshRef & (const VertexBasedMeshRef &)
//-----------------------------------------------------------------------------------
VertexBasedMeshRef & VertexBasedMeshRef::operator = (const VertexBasedMeshRef &Src) {
	Copy(Src);
	return *this;
}

// ================================================================================
// VertexBasedMesh::GetMeshRefSphere
// ================================================================================
void VertexBasedMesh::GetMeshRefSphere(Vector3D v3dCenter, float Radius,
									   VertexBasedMeshRef *pVBMeshRef)
{
	pVBMeshRef->Free();

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
		if(d < R2) pVBMeshRef->m_TriNums.Add(nTri);
	}
}

// ================================================================================
// VertexBasedMesh::GetMeshRefBox
// ================================================================================
void VertexBasedMesh::GetMeshRefBox(int x, int y, int w, int l,
									VertexBasedMeshRef *pVBMeshRef)
{
	pVBMeshRef->Free();

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
			pVBMeshRef->m_TriNums.Add(nTri);
	}
}

void VertexBasedMesh::CalcBoundRect(int &x0, int &y0, int &x1, int &y1) const
{
	assert(Vert.NValues > 0);
	if(Vert.NValues <= 0) return;
	x0 = x1 = Vert[0].x, y0 = y1 = Vert[0].y;
	for(int i = 1; i < Vert.NValues; i++)
	{
		OneMapVertex &v = Vert[i];
		if(x0 > v.x) x0 = v.x;
		if(y0 > v.x) y0 = v.y;
		if(x1 < v.x) x1 = v.x;
		if(y1 < v.y) y1 = v.y;
	}
}

void VertexBasedMesh::CalcBoundBox(int &x0, int &y0, int &z0, int &x1, int &y1, int &z1) const
{
	assert(Vert.NValues > 0);
	if(Vert.NValues <= 0) return;
	x0 = x1 = Vert[0].x, y0 = y1 = Vert[0].y, z0 = z1 = Vert[0].z;
	for(int i = 1; i < Vert.NValues; i++)
	{
		OneMapVertex &v = Vert[i];
		if(x0 > v.x) x0 = v.x;
		if(y0 > v.y) y0 = v.y;
		if(z0 > v.z) z0 = v.z;
		if(x1 < v.x) x1 = v.x;
		if(y1 < v.y) y1 = v.y;
		if(z1 < v.z) z1 = v.z;
	}
}

void VertexBasedMesh::CalcBoundBox()
{
	int x0, y0, z0, x1, y1, z1;
	CalcBoundBox(x0, y0, z0, x1, y1, z1);
	m_BBox0.Set(short(x0), short(y0), short(z0));
	m_BBox1.Set(short(x1), short(y1), short(z1));
}






// ~VertexBasedMesh
VertexBasedMesh::~VertexBasedMesh() {
	Free();
}

// VertexBasedMesh::Free
void VertexBasedMesh::Free() {
	Vert.Clear();
	Idxs.Clear();
}
//-----------------------------------------------------------------------------
// VertexBasedMesh::GenHexagonalPlane()
//-----------------------------------------------------------------------------
void VertexBasedMesh::GenHexagonalPlane(int Lx, int Ly, int D) {
	Free();
	assert(D > 0);

	int Nx = Lx / D, Ny = Ly / D, nx = Nx - 1, ny = Ny - 1, r = 0, c = 0;
	OneMapVertex vtx;
	int A = D / 3, rx, ry;

	for(int r = 0; r <= Ny; r++)
		for(int c = 0; c <= Nx; c++) {
			rx = rand() % A - A / 2;
			ry = rand() % A - A / 2;
			AddVertex(OneMapVertex(c * D + rx, r * D + ry));
		}
	for(int r = 0; r <= ny; r++)
		for(int c = 0; c <= nx; c++) {
			rx = rand() % A - A / 2;
			ry = rand() % A - A / 2;
			AddVertex(OneMapVertex(c * D + D / 2 + rx, r * D + D / 2 + ry));
		}
	// triangulation
	int nStart = (Nx + 1) * (Ny + 1), i0, i1, i2;
	for(int r = 0; r <= ny; r++)
		for(int c = 0; c <= nx; c++)
		{
			// triangle to the left
			i0 = c + r * (Nx + 1);
			i1 = c + r * (nx + 1) + nStart;
			i2 = c + r * (Nx + 1) + (Nx + 1);
			AddTri(i0, i1, i2);
			// triangle to the right
			i0 = c + r * (nx + 1) + nStart;
			i1 = c + r * (Nx + 1) + 1;
			i2 = c + r * (Nx + 1) + 1 + (Nx + 1);
			AddTri(i0, i1, i2);
			if(r != 0) {
				// triangle to the upper left
				i0 = c + r * (nx + 1) + nStart;
				i1 = c + r * (Nx + 1);
				i2 = c + (r - 1) * (nx + 1) + nStart;
				AddTri(i0, i1, i2);
				// triangle to the upper right
				i0 = c + r * (nx + 1) + nStart;
				i1 = c + (r - 1) * (nx + 1) + nStart;
				i2 = c + r * (Nx + 1) + 1;
				AddTri(i0, i1, i2);
			}
			if(r == 0) {
				// triangle to upper
				i0 = c + r * (nx + 1) + nStart;
				i1 = c + r * (Nx + 1);
				i2 = c + r * (Nx + 1) + 1;
				AddTri(i0, i1, i2);
			}
			if(r == ny) {
				// triangle to lower
				i0 = c + r * (nx + 1) + nStart;
				i1 = c + (r + 1) * (Nx + 1);
				i2 = c + (r + 1) * (Nx + 1) + 1;
				AddTri(i0, i1, i2);
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
	GenHexagonalPlane(Sx, Sy, Dx);

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

//-----------------------------------------------------------------------------
// VertexBasedMeshRef::Relax : void(const RelaxArgs &)
//-----------------------------------------------------------------------------
struct RelaxEdge {
	int e0, e1, nUsed;
	RelaxEdge() : e0(-1), e1(-1), nUsed(1) {}
	bool Equals(const RelaxEdge &Edge) const {
		return Edge.e0 == e0 && Edge.e1 == e1;
	}
};
struct RelaxNode {
	int oi;
	ints SurrVerts;
	bool fBoundary;
	RelaxNode() : oi(-1), fBoundary(false) {}
};
void VertexBasedMeshRef::Relax(const RelaxArgs & Args) {
	assert(pOrigin);
	if(!m_TriNums.NValues) return;
	// Extracting edges and calcing their usage stats:
	int nTri, oi[3], k, e0, e1, key, index;
	cHashIndex eHash;
	RelaxEdge Edge;
	cList<RelaxEdge> Edges;
	int MaxEdgeCount = 3 * m_TriNums.NValues;
	Edges.Lock(MaxEdgeCount);
	int nEdgesUsed = 0;
	for(int iTri = 0; iTri < m_TriNums.NValues; iTri++) {
		nTri = m_TriNums[iTri];
		pOrigin->GetTri(nTri, oi);
		for(k = 0; k < 3; k++) {
			e0 = oi[k], e1 = oi[(k + 1) % 3];
			Edge.e0 = Min(e0, e1);
			Edge.e1 = Max(e0, e1);
			key = eHash.GenerateKey(Edge.e0, Edge.e1);
			for(index = eHash.First(key); index != -1; index = eHash.Next(index)) {
				if(Edges[index].Equals(Edge)) break;
			}
			if(index == -1) {
				index = nEdgesUsed;
				nEdgesUsed++;
				Edges[index] = Edge;
				eHash.Add(key, index);
			} else {
				Edges[index].nUsed++;
			}
		}
	}
	Edges.RemoveAt(nEdgesUsed, MaxEdgeCount - nEdgesUsed);
	// Extracting verts:
	DWORDS VertNums;
	cHashIndex reIndexingTable;
	ExtractUniqueVerts(VertNums, reIndexingTable);
	// Filling nodes:
	// oi:
	cList<RelaxNode> Nodes;
	Nodes.Lock(VertNums.Count());
	for(k = 0; k < VertNums.Count(); k++) {
		Nodes[k].oi = VertNums[k];
	}
	// SurrVerts:
	int e[2];
	for(int nEdge = 0; nEdge < Edges.Count(); nEdge++) {
		e[0] = Edges[nEdge].e0;
		e[1] = Edges[nEdge].e1;
		for(k = 0; k < 2; k++) {
			key = reIndexingTable.GenerateKey(e[k]);
			for(index = reIndexingTable.First(key); index != -1; index = reIndexingTable.Next(index)) {
				if(Nodes[index].oi == e[k]) break;
			}
			assert(index != -1);
			e[k] = index;
		}
		Nodes[e[0]].SurrVerts.Add(Nodes[e[1]].oi);
		Nodes[e[1]].SurrVerts.Add(Nodes[e[0]].oi);
		if(Edges[nEdge].nUsed <= 1) {
			Nodes[e[0]].fBoundary = true;
			Nodes[e[1]].fBoundary = true;
		}
	}
	// Essentially relaxation:
	double l = comms::Clamp(Args.Amount, -1., 1.);
	for(int N = 0; N < Args.Iterations; N++) {
		for(int nNode = 0; nNode < Nodes.Count(); nNode++) {
			const RelaxNode &Node = Nodes[nNode];
			if(Args.HoldBoundaryPoints && Node.fBoundary) continue;
			cVector AvePos;
			for(k = 0; k < Node.SurrVerts.Count(); k++) {
				const OneMapVertex &vtx = pOrigin->Vert[Node.SurrVerts[k]];
				AvePos += cVector(vtx.x, vtx.y, vtx.z);
			}
			AvePos /= Node.SurrVerts.Count();
			OneMapVertex &r = pOrigin->Vert[Node.oi];
			cVector CurPos(r.x, r.y, r.z);
			cVector Pos = CurPos + l * (AvePos - CurPos);
			r.x = (short)Pos.x;
			r.y = (short)Pos.y;
			r.z = (short)Pos.z;
		}
	}
}

// VertexBasedMeshRef::CalcBoundBox
void VertexBasedMeshRef::CalcBoundBox(OneMapVertex &AABBMin, OneMapVertex &AABBMax) const {
	assert(pOrigin);
	AABBMin = OneMapVertex::Origin;
	AABBMax = OneMapVertex::Origin;
	if(!m_TriNums.NValues) return;

	// Init AABBMin && AABBMax:
	int ii0 = 3 * m_TriNums[0];
	int oi = pOrigin->Idxs[ii0];
	AABBMax = AABBMin = pOrigin->Vert[oi];
	// Enuming tris:
	for(int iTri = 0; iTri < m_TriNums.NValues; iTri++) {
		ii0 = 3 * m_TriNums[iTri];
		for(int k = 0; k < 3; k++) {
			oi = pOrigin->Idxs[ii0 + k];
			const OneMapVertex &v = pOrigin->Vert[oi];
			AABBMin.x = Min(AABBMin.x, v.x);
			AABBMin.y = Min(AABBMin.y, v.y);
			AABBMin.z = Min(AABBMin.z, v.z);
			AABBMax.x = Max(AABBMax.x, v.x);
			AABBMax.y = Max(AABBMax.y, v.y);
			AABBMax.z = Max(AABBMax.z, v.z);
		}
	}
} // VertexBasedMeshRef::CalcBoundBox

// VertexBasedMeshRef::CalcBoundSphere
void VertexBasedMeshRef::CalcBoundSphere(OneMapVertex &Centre, int &Radius) const {
	assert(pOrigin);
	Centre = OneMapVertex::Origin;
	Radius = 0;
	if(!m_TriNums.NValues) return;
	// Calcing AABB:
	OneMapVertex AABBMin, AABBMax;
	CalcBoundBox(AABBMin, AABBMax);
	// -> Centre:
	Centre.Set((AABBMin.x + AABBMax.x) / 2, (AABBMin.y + AABBMax.y) / 2, (AABBMin.z + AABBMax.z) / 2);
	// -> Radius:
	int dx = AABBMax.x - Centre.x, dy = AABBMax.y - Centre.y, dz = AABBMax.z - Centre.z;
	Radius = sqrt(Square(dx) + Square(dy) + Square(dz));
} // VertexBasedMeshRef::CalcBoundSphere

// VertexBasedMeshRef::CalcTriCenters
void VertexBasedMeshRef::CalcTriCenters(cVectors &TriCenters) const {
	assert(pOrigin);
	TriCenters.Free();
	if(!m_TriNums.NValues) return;
	TriCenters.Lock(m_TriNums.NValues);
	// Enuming tris & calcing centers:
	int ii0, i0, i1, i2;
	cVector c;
	for(int iTri = 0; iTri < m_TriNums.NValues; iTri++) {
		ii0 = 3 * m_TriNums[iTri];
		i0 = pOrigin->Idxs[ii0], i1 = pOrigin->Idxs[ii0 + 1], i2 = pOrigin->Idxs[ii0 + 2];
		const OneMapVertex &v0 = pOrigin->Vert[i0], &v1 = pOrigin->Vert[i1], &v2 = pOrigin->Vert[i2];
		c.x = double(v0.x + v1.x + v2.x) / 3.;
		c.y = double(v0.y + v1.y + v2.y) / 3.;
		c.z = double(v0.z + v1.z + v2.z) / 3.;
		TriCenters[iTri] = c;
	}
} // VertexBasedMeshRef::CalcTriCenters

// VertexBasedMeshRef::CalcTriNormals
void VertexBasedMeshRef::CalcTriNormals(cVectors &TriNormals) const {
	assert(pOrigin);
	TriNormals.Free();
	if(!m_TriNums.NValues) return;
	TriNormals.Lock(m_TriNums.NValues);
	// Enuming tris & calcing normals:
	int ii0, i0, i1, i2;
	cVector u, v;
	for(int iTri = 0; iTri < m_TriNums.NValues; iTri++) {
		ii0 = 3 * m_TriNums[iTri];
		i0 = pOrigin->Idxs[ii0], i1 = pOrigin->Idxs[ii0 + 1], i2 = pOrigin->Idxs[ii0 + 2];
		const OneMapVertex &v0 = pOrigin->Vert[i0], &v1 = pOrigin->Vert[i1], &v2 = pOrigin->Vert[i2];
		u.Set(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
		v.Set(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);
		cVector &n = TriNormals[iTri];
		n = u.Cross(v);
		n.Normalize();
	}
} // VertexBasedMeshRef::CalcTriNormals

// VertexBasedMeshRef::CalcNormals(int, int, int)
void VertexBasedMeshRef::CalcNormals(int cx, int cy, int r) {
	assert(pOrigin);
	if(!m_TriNums.NValues) return;

	// Calcing normals in tris:
	cVectors TriNormals;
	CalcTriNormals(TriNormals);
	if(!TriNormals.Count()) return;
	// Extracting verts, in which we should calc normals:
	VertexBasedMeshRef Ref;
	GetRefCylinder(cx, cy, r, Ref);
	if(!Ref.m_TriNums.NValues) return;
	DWORDS VertNums;
	cHashIndex reIndexingTable;
	Ref.ExtractUniqueVerts(VertNums, reIndexingTable);
	if(!VertNums.Count()) return;
	// Enumerating tris and calcing normals in verts:
	int ii0, k, oi, key, index;
	cVectors VertNormals;
	VertNormals.Lock(VertNums.Count());
	for(int iTri = 0; iTri < m_TriNums.NValues; iTri++) {
		const cVector &t = TriNormals[iTri];
		ii0 = 3 * m_TriNums[iTri];
		for(k = 0; k < 3; k++) {
			oi = pOrigin->Idxs[ii0 + k];
			key = reIndexingTable.GenerateKey(oi);
			for(index = reIndexingTable.First(key); index != -1; index = reIndexingTable.Next(index)) {
				if(VertNums[index] == oi) {
					VertNormals[index] += t;
					break;
				}
			}
		}
	}
	// Storing:
	for(int i = 0; i < VertNums.Count(); i++) {
		cVector &n = VertNormals[i];
		if(cUtils::NotZero(n.Length())) {
			n.Normalize();
			n *= OneMapVertex::NormalLen;
			OneMapVertex &v = pOrigin->Vert[VertNums[i]];
			v.nx = n.x, v.ny = n.y, v.nz = n.z;
		}
	}
} // VertexBasedMeshRef::CalcNormals(int, int, int)

// VertexBasedMesh::CalcNormals
void VertexBasedMesh::CalcNormals() {
	// Calcing normals in tris:
	int NTris = Idxs.NValues / 3;
	VertexBasedMeshRef Ref;
	Ref.pOrigin = this;
	for(int nTri = 0; nTri < NTris; nTri++) {
		Ref.AddRef(nTri);
	}
	cVectors TriNormals;
	Ref.CalcTriNormals(TriNormals);
	if(!TriNormals.Count()) return;

	// Calcing normals in verts:
	cVectors VertNormals;
	VertNormals.Lock(Vert.NValues);
	for(int i = 0; i < Idxs.NValues; i++) {
		VertNormals[Idxs[i]] += TriNormals[i / 3];
	}
	// Storing:
	for(i = 0; i < Vert.NValues; i++) {
		cVector &n = VertNormals[i];
		if(cUtils::NotZero(n.Length())) {
			n.Normalize();
			n *= OneMapVertex::NormalLen;
			OneMapVertex &v = Vert[i];
			v.nx = n.x, v.ny = n.y, v.nz = n.z;
		}
	}
} // CalcNormals

/*
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
}*/

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

// VertexBasedMesh::GetRefAll
void VertexBasedMesh::GetRefAll(VertexBasedMeshRef &Ref) {
	Ref.Free();
	Ref.pOrigin = this;
	int TriCount = Idxs.NValues / 3;
	for(int nTri = 0; nTri < TriCount; nTri++)
		Ref.m_TriNums.Add(nTri);
} // VertexBasedMesh::GetRefAll

// UniversalMap::Refresh
int UniversalMap::Refresh() {
	if(!Vert.NValues || !Idxs.NValues) return -1;
	DWORD tStart = GetTickCount();
	Pack();
	CalcNormals();
	VertexBasedMeshRef Ref;
	GetQuadsRefAll(Ref);
	Ref.FillMapOfHeights();
	return GetTickCount() - tStart;
} // UniversalMap::Refresh

// UniversalMap::Refresh(eSSIllumination)
int UniversalMap::Refresh(eSSIllumination Illumination) {
	if(!Vert.NValues || !Idxs.NValues) return -1;
	DWORD tStart = GetTickCount();
	Refresh();
	Shade(m_Shader, Illumination);
	return GetTickCount() - tStart;
} // UniversalMap::Refresh(eSSIllumination)

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
		m_pQuads[m_pQuads.NValues - 1]->m_TriNums.AddValues(pUniversalMap->m_pQuads[nQ]->m_TriNums.GetValues(),
			pUniversalMap->m_pQuads[nQ]->m_TriNums.NValues);
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
//CSun g_Sun(0.4f, 40, 160, /*0*/Vector3D(10, 3, -63), /*1*/Vector3D(14, 5, -20),
//		   /*2*/Vector3D(2, 19, -22), /*3*/Vector3D(8, 16, -64), /*4*/Vector3D(15, 2, -50),
//		   /*5*/Vector3D(3, -12, -16), /*6*/Vector3D(9, 0, -60), /*7*/Vector3D(21, 18, -57),
//		   /*8*/Vector3D(0, 19, -56), /*9*/Vector3D(23, -8, -31),
//		   /*10*/Vector3D(135, 51, 59), /*11*/Vector3D(110, 38, 64), /*12*/Vector3D(127, 61, 64),
//		   /*13*/Vector3D(138, 77, 64), /*14*/Vector3D(109, 48, 68), /*15*/Vector3D(141, 61, 62),
//		   /*16*/Vector3D(132, 69, 59), /*17*/Vector3D(111, 85, 65), /*18*/Vector3D(102, 66, 59),
//		   /*19*/Vector3D(134, 45, 59), Vector3D::null);

UniversalMap g_UniversalMapBackup;
UniversalMap g_UniversalMap(SurfaceShader(), &g_UniversalMapBackup);
bool g_fShowTriNormals = false, g_ShowMapOfHeights = false, g_fShowVertNormals = false;
eTerraFill g_TerraFill = TFSolid;

// VertexBasedMeshRef::CalcZ
int VertexBasedMeshRef::CalcZ(const OneMapVertex &t0, const OneMapVertex &t1, const OneMapVertex &t2, int x, int y) {
	const cVector v0(t0.x, t0.y, t0.z), v1(t1.x, t1.y, t1.z), v2(t2.x, t2.y, t2.z);
	cVector N = (v1 - v0) ^ (v2 - v0);
	if(cUtils::NotZero(N.Length())) {
		N.Normalize();
		if(cUtils::NotZero(N.z)) {
			double hx = x - v0.x, hy = y - v0.y;
			return - (N.x * hx + N.y * hy) / N.z + v0.z;
		}
	}
	return comms::IntMin;
} // VertexBasedMeshRef::CalcZ

// VertexBasedMeshRef::CalcMaxZ
int VertexBasedMeshRef::CalcMaxZ(int cx, int cy) const {
	int z = comms::IntMin;
	// Enumerating tris:
	int nTri, ii0, i0, i1, i2;
	for(int iTri = 0; iTri < m_TriNums.NValues; iTri++) {
		nTri = m_TriNums[iTri];
		ii0 = 3 * nTri;
		i0 = pOrigin->Idxs[ii0], i1 = pOrigin->Idxs[ii0 + 1], i2 = pOrigin->Idxs[ii0 + 2];
		const OneMapVertex &t0 = pOrigin->Vert[i0], &t1 = pOrigin->Vert[i1], &t2 = pOrigin->Vert[i2];
		const int x1 = t0.x, y1 = t0.y;
		const int x2 = t1.x, y2 = t1.y;
		const int x3 = t2.x, y3 = t2.y;
		// Equations of a line:
		int s0 = Sign((x1 - x2) * (cy - y1) - (y1 - y2) * (cx - x1));
		int s1 = Sign((x2 - x3) * (cy - y2) - (y2 - y3) * (cx - x2));
		int s2 = Sign((x3 - x1) * (cy - y3) - (y3 - y1) * (cx - x3));
//		if(!s0 || !s1 || !s2 || (s0 == s1 && s0 == s2)) { // (cx, cy) is within this tri:
		if(s0 == s1 && s0 == s2) {
			z = Max(z, CalcZ(t0, t1, t2, cx, cy));
		}
	}
	return z;
} // VertexBasedMeshRef::CalcMaxZ

const int g_LenOfTHMap = 512 * 512;
cBitMap g_BitMapOfTHMap(g_LenOfTHMap);

//----------------------------------------------------------------------------------------------------------
// VertexBasedMeshRef::Tri2THMap : void(const OneMapVertex &, const OneMapVertex &, const OneMapVertex &)
//----------------------------------------------------------------------------------------------------------
void VertexBasedMeshRef::Tri2THMap(const OneMapVertex &t0, const OneMapVertex &t1, const OneMapVertex &t2) {
	const int x1 = t0.x, x2 = t1.x, x3 = t2.x;
	const int y1 = t0.y, y2 = t1.y, y3 = t2.y;

	// Block size:
	const int qx = 32, qy = 16;

	// Deltas:
	const int Dx12 = x1 - x2, Dx23 = x2 - x3, Dx31 = x3 - x1;
	const int Dy12 = y1 - y2, Dy23 = y2 - y3, Dy31 = y3 - y1;

	// Block deltas:
	const int qDx12 = qy * Dx12, qDx23 = qy * Dx23, qDx31 = qy * Dx31;
	const int qDy12 = qx * Dy12, qDy23 = qx * Dy23, qDy31 = qx * Dy31;

	// Bounding rect:
	int xMin = comms::Min(x1, x2, x3);
	int xMax = comms::Max(x1, x2, x3);
	int yMin = comms::Min(y1, y2, y3);
	int yMax = comms::Max(y1, y2, y3);

	// Start in corner of block:
	xMin &= ~(qx - 1);
	yMin &= ~(qy - 1);

	// Constant parts of half - space functions:
	int C1 = Dy12 * x1 - Dx12 * y1;
	int C2 = Dy23 * x2 - Dx23 * y2;
	int C3 = Dy31 * x3 - Dx31 * y3;

	// Fill convention:
	if(Dy12 < 0 || (Dy12 == 0 && Dx12 > 0)) C1++;
	if(Dy23 < 0 || (Dy23 == 0 && Dx23 > 0)) C2++;
	if(Dy31 < 0 || (Dy31 == 0 && Dx31 > 0)) C3++;

	// Starting values of the half - space functions at the top of the bounding rectangle:
	int Cy1 = C1 + Dx12 * yMin - Dy12 * xMin;
	int Cy2 = C2 + Dx23 * yMin - Dy23 * xMin;
	int Cy3 = C3 + Dx31 * yMin - Dy31 * xMin;

	// Scanning:
	for(int y = yMin; y < yMax; y += qy) {
		// Starting values for horizontal scan:
		int Cx1 = Cy1, Cx2 = Cy2, Cx3 = Cy3;
		for(int x = xMin; x < xMax; x += qx) {
			if(Cx1 > 0 && Cx2 > 0 && Cx3 > 0) {
				int x64 = x & 0x3F /* %64 */, x32 = x & 0x1F /* %32 */;
				int y32 = y & 0x1F /* %32 */, y16_32 = (y + 16) & 0x1F /* %32 */;
				int n = -1;
				if(!x64 && !y32) {
					n = x / 32 + VertInLine * y / 32;
				} else if(x64 && !x32 && !y16_32) {
					n = x / 32 + VertInLine * (y + 16) / 32;
				}
				if(comms::RangeCheck(n, 0, g_LenOfTHMap - 1)) {
					cVector v0(t0.x, t0.y, t0.z), v1(t1.x, t1.y, t1.z), v2(t2.x, t2.y, t2.z);
					cVector u = v1 - v0;
					cVector v = v2 - v0;
					cVector N = u.Cross(v);
					if(cUtils::NotZero(N.z)) {
						N.Normalize();
						double hx = x - x1, hy = y - y1;
						int z = - (N.x * hx + N.y * hy) / N.z + int(t0.z);
						if(!g_BitMapOfTHMap[n]) {
							THMap[n] = z;
							g_BitMapOfTHMap.Set(n);
						} else {
							if(THMap[n] < z) THMap[n] = z;
						}
					}
				}
			}
			Cx1 -= qDy12;
			Cx2 -= qDy23;
			Cx3 -= qDy31;
		}
		Cy1 += qDx12;
		Cy2 += qDx23;
		Cy3 += qDx31;
	}
} // VertexBasedMeshRef::Tri2THMap


// VertexBasedMeshRef::FillMapOfHeights
void VertexBasedMeshRef::FillMapOfHeights() const {
	assert(pOrigin);
	int nTris = m_TriNums.NValues;
	if(!nTris) return;

	g_BitMapOfTHMap.ClearAll();
	// Rasterizing tris to THMap nodes:
	int nTri, ii0, i0, i1, i2;
	for(int iTri = 0; iTri < nTris; iTri++) {
		nTri = m_TriNums[iTri];
		ii0 = 3 * nTri;
		i0 = pOrigin->Idxs[ii0], i1 = pOrigin->Idxs[ii0 + 1], i2 = pOrigin->Idxs[ii0 + 2];
		Tri2THMap(pOrigin->Vert[i0], pOrigin->Vert[i2], pOrigin->Vert[i1]);
	}
} // VertexBasedMeshRef::FillMapOfHeights

// VertexBasedMeshRef::SmoothColors
void VertexBasedMeshRef::SmoothColors() {
	assert(pOrigin);
	const int kS = 10, ks = 1;
	const int S = kS + 2 * ks;
	int i[3], k, r[3], g[3], b[3], rTotal, gTotal, bTotal, R[3], G[3], B[3];
	OneMapVertex *p[3];
	for(int nTri = 0; nTri < m_TriNums.NValues; nTri++) {
		pOrigin->GetTri(m_TriNums[nTri], i[0], i[1], i[2]);
		rTotal = gTotal = bTotal = 0;
		for(k = 0; k < 3; k++) {
			p[k] = &pOrigin->Vert[i[k]];
			argb2rgb(p[k]->Color, r[k], g[k], b[k]);
			rTotal += r[k];
			gTotal += g[k];
			bTotal += b[k];
		}
		for(k = 0; k < 3; k++) {
			R[k] = (kS * r[k] + ks * r[(k + 1) % 3] + ks * r[(k + 2) % 3]) / S;
			G[k] = (kS * g[k] + ks * g[(k + 1) % 3] + ks * g[(k + 2) % 3]) / S;
			B[k] = (kS * b[k] + ks * b[(k + 1) % 3] + ks * b[(k + 2) % 3]) / S;
		}
		// Balance:
		int RTotal = 0, GTotal = 0, BTotal = 0;
		for(k = 0; k < 3; k++) {
			RTotal += R[k];
			GTotal += G[k];
			BTotal += B[k];
		}
		int *pR[3] = {&R[0], &R[1], &R[2]}, *pG[3] = {&G[0], &G[1], &G[2]}, *pB[3] = {&B[0], &B[1], &B[2]};
		int nCur = 0;
		for(int nTotalTimes = 0; nTotalTimes < 1000; nTotalTimes++) {
			if(RTotal < rTotal && *pR[nCur] < 255) {
				(*pR[nCur])++;
				RTotal++;
			}
			if(GTotal < gTotal && *pG[nCur] < 255) {
				(*pG[nCur])++;
				GTotal++;
			}
			if(BTotal < bTotal && *pB[nCur] < 255) {
				(*pB[nCur])++;
				BTotal++;
			}
			nCur++;
			if(nCur == 3) nCur = 0;
		}
		for(k = 0; k < 3; k++) {
			rgb2argb(R[k], G[k], B[k], p[k]->Color);
		}
	}
} // VertexBasedMeshRef::SmoothColors

// ================================================================================
// UniversalMap::CreateHollow
// ================================================================================
/*void UniversalMap::CreateHollow(int cx, int cy, int r, int depth)
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
			VBMeshRef.m_TriNums.AddValues(pQuads[i]->m_TriNums.GetValues(),
			pQuads[i]->m_TriNums.GetAmount());
		VBMeshRef.pOrigin = pQuads[0]->pOrigin;
		// in which triangle we have hit?
		N = VBMeshRef.m_TriNums.GetAmount();
		int nTri = 0, i0 = 0, i1 = 0, i2 = 0, nXTri = 0;
		VertexBasedMesh *pOrigin = NULL;
		OneMapVertex vtx0, vtx1, vtx2;
		Vector3D v3dA, v3dB, v3dC;
		bool bX = false;
		float Z = 0;
		pOrigin = VBMeshRef.pOrigin;
		for(i = 0; i < N; i++)
		{
			nTri = VBMeshRef.m_TriNums[i];
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
				nXTri = VBMeshRef.m_TriNums[i];
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
//		FillMapOfHeights();
//		float CreateNaturalShadow(VertexBasedMesh *, int);
//		CreateNaturalShadow(this, 2048);
		void ResetGroundCache();
		ResetGroundCache();
}*/

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

// VertexBasedMesh::AddTri(DWORD, DWORD, DWORD)
void VertexBasedMesh::AddTri(DWORD i0, DWORD i1, DWORD i2) {
	Idxs.Add(i0);
	Idxs.Add(i1);
	Idxs.Add(i2);
} // VertexBasedMesh::AddTri(DWORD, DWORD, DWORD)

// VertexBasedMesh::AddTri(const OneMapVertex &, const OneMapVertex &, const OneMapVertex &)
void VertexBasedMesh::AddTri(const OneMapVertex &v0, const OneMapVertex &v1, const OneMapVertex &v2) {
	int i0 = Vert.Add(v0), i1 = Vert.Add(v1), i2 = Vert.Add(v2);
	Idxs.Add(i0), Idxs.Add(i1), Idxs.Add(i2);
} // VertexBasedMesh::AddTri(const OneMapVertex &, const OneMapVertex &, const OneMapVertex &)

// VertexBasedMesh::GenMarker
void VertexBasedMesh::GenMarker(const OneMapVertex &Center, int Dim, bool fLive) {
	Free();
	// Unit Cube:
	const cVector Src[8] = {cVector(0.5, 0.5, - 0.5) /* 0 */, cVector(0.5, -0.5, -0.5) /* 1 */,
		cVector(-0.5, -0.5, -0.5) /* 2 */, cVector(-0.5, 0.5, -0.5) /* 3 */,
		cVector(0.5, 0.5, 0.5) /* 4 */, cVector(0.5, -0.5, 0.5) /* 5 */,
		cVector(-0.5, -0.5, 0.5) /* 6 */, cVector(-0.5, 0.5, 0.5) /* 7 */};
	// OneMark:
	VertexBasedMesh OneMark;
	// TriTopology:
	// Bottom:
	OneMark.AddTri(0, 2, 3);
	OneMark.AddTri(0, 1, 2);
	// Top:
	OneMark.AddTri(4, 7, 6);
	OneMark.AddTri(4, 6, 5);
	// Front:
	OneMark.AddTri(0, 4, 5);
	OneMark.AddTri(0, 5, 1);
	// Back:
	OneMark.AddTri(2, 6, 7);
	OneMark.AddTri(2, 7, 3);
	// Right:
	OneMark.AddTri(0, 3, 7);
	OneMark.AddTri(0, 7, 4);
	// Left:
	OneMark.AddTri(2, 1, 5);
	OneMark.AddTri(2, 5, 6);
	// Verts:
	OneMark.Vert.Add(OneMapVertex(0, 0, 0, Center.Color, 128, Center.nFacture), 8);

	// Transforming unit cube to requested:
	cMatrix T = cMatrix::OfScale(cVector(Dim, Dim, Dim));
	if(fLive) {
		DWORD t = GetTickCount();
		const int tCycle = 300; // (ms)
		double Angle = comms::c2Pi * double(t % tCycle) / double(tCycle);
		double s = 0.8 + sin(Angle) / 5.;
		T *= cMatrix::OfScale(cVector(s, s, s));
		T *= cMatrix::OfRotation(cVector::zAxis, Angle / 4.);
	}
	// Gening:
	cMatrix M = cMatrix::OfTranslation(cVector(Center.x, Center.y, Center.z)) * T;
	// cVector -> OneMapVertex:
	for(int i = 0; i < 8; i++) {
		cVector vec = Src[i] * M;
		OneMark.Vert[i].Set((short)vec.x, (short)vec.y, (short)vec.z);
	}
	Sum(OneMark);
} // VertexBasedMesh::GenMarker

// VertexBasedMesh::AddMarker
void VertexBasedMesh::AddMarker(const OneMapVertex &l0, const OneMapVertex &l1, int d, DWORD argbColor) {
	const byte Shadow = 255;
	OneMapVertex t0, t1, t2;
	t0.Set(l0.x - d / 2, l0.y, l0.z, argbColor, Shadow);
	t1.Set(l1.x - d / 2, l1.y, l1.z, argbColor, Shadow);
	t2.Set(l1.x + d / 2, l1.y, l1.z, argbColor, Shadow);
	AddTri(t0, t1, t2);
	t0.Set(l0.x - d / 2, l0.y, l0.z, argbColor, Shadow);
	t1.Set(l1.x + d / 2, l1.y, l1.z, argbColor, Shadow);
	t2.Set(l0.x + d / 2, l0.y, l0.z, argbColor, Shadow);
	AddTri(t0, t1, t2);
	t0.Set(l0.x, l0.y - d / 2, l0.z, argbColor, Shadow);
	t1.Set(l1.x, l1.y - d / 2, l1.z, argbColor, Shadow);
	t2.Set(l1.x, l1.y + d / 2, l1.z, argbColor, Shadow);
	AddTri(t0, t1, t2);
	t0.Set(l0.x, l0.y - d / 2, l0.z, argbColor, Shadow);
	t1.Set(l1.x, l1.y + d / 2, l1.z, argbColor, Shadow);
	t2.Set(l0.x, l0.y + d / 2, l0.z, argbColor, Shadow);
	AddTri(t0, t1, t2);
} // VertexBasedMesh::AddMarker

// VertexBasedMesh::AddLine
void VertexBasedMesh::AddLine(const OneMapVertex &l0, const OneMapVertex &l1, DWORD argbColor) {
	const byte Shadow = 255;
	OneMapVertex t0, t1, t2;
	t0.Set(l0.x, l0.y, l0.z, argbColor, Shadow);
	t1.Set(l1.x, l1.y, l1.z, argbColor, Shadow);
	t2.Set(l1.x + 1, l1.y, l1.z, argbColor, Shadow);
	AddTri(t0, t1, t2);
	t0.Set(l0.x, l0.y, l0.z, argbColor, Shadow);
	t1.Set(l1.x + 1, l1.y, l1.z, argbColor, Shadow);
	t2.Set(l0.x + 1, l0.y, l0.z, argbColor, Shadow);
	AddTri(t0, t1, t2);
	t0.Set(l0.x, l0.y, l0.z, argbColor, Shadow);
	t1.Set(l1.x, l1.y, l1.z, argbColor, Shadow);
	t2.Set(l1.x, l1.y + 1, l1.z, argbColor, Shadow);
	AddTri(t0, t1, t2);
	t0.Set(l0.x, l0.y, l0.z, argbColor, Shadow);
	t1.Set(l1.x, l1.y + 1, l1.z, argbColor, Shadow);
	t2.Set(l0.x, l0.y + 1, l0.z, argbColor, Shadow);
	AddTri(t0, t1, t2);
} // VertexBasedMesh::AddLine


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
//		extern CSun g_Sun;
//		cMesh.Shade(g_Sun); // SHADE

//		SURF.TexturizeSurface(cMesh);
		Sum(cMesh);
		// uninit
//		cVert.Clear();
		cVertices.Clear();
//		cIndices.Clear();

		cMesh.Free();
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

/*void UniversalMap::ExtractQuadsInRadius(CUniversalQuadsPointers &rQuadsPointers, int cx, int cy, int r)
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
}*/

/*bool UniversalMap::ExtractQuadsInQuad_InQuad(int x0, int y0, int x1, int y1, int X0, int Y0, int X1, int Y1)
{
	int x[] = {x0, x0 + QuadLx, x0 + QuadLx, x0}, y[] = {y0, y0, y0 + QuadLy, y0 + QuadLy};
	for(int i = 0; i < 4; i++) if(x[i] >= X0 && x[i] <= X1 && y[i] >= Y0 && y[i] <= Y1) return true;
	return false;
}*/
/*void UniversalMap::ExtractQuadsInQuad(CUniversalQuadsPointers &rQuadsPointers, int cx, int cy, int S)
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
}*/

/*void UniversalMap::ExtractQuads(CUniversalQuadsPointers &rQuadsPointers, const VertexBasedMesh &rVBMesh)
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
}*/

// ================================================================================
// VertexBaseMesh::fromModelByName
// ================================================================================
bool VertexBasedMesh::fromModelByName(const char *strModelName, const Matrix4D &m4dWorld)
{
	Free();
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
/*
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
}*/

bool VertexBasedMesh::CrossSectionMesh(const OneMapVertex &rVertexA, const OneMapVertex &rVertexB)
{
	int NTri = Idxs.NValues / 3;
	for(int nTri = 0; nTri < NTri; nTri++)
	{
		int r = 0;//CrossSectionTriangle(rVertexA, rVertexB, nTri);
		if((r == -1) || (r == 1)) return true;
	}
	return false;
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
/*void UniversalMap::CombineModelByName(const char *strModelName, const Matrix4D &m4dWorld)
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
}*/

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
		pVBMRef->Free();
		pVBMRef->pOrigin = this;
		OneUniversalMapQuad *pQuad = NULL;
		DynArray<DWORD> nAllTriangles;
		int N = 0, i = 0;
		N = pQuads.GetAmount();
		if(N == 0) return;
		for(i = 0; i < N; i++)
		{
			pQuad = pQuads[i];
			nAllTriangles.AddValues(pQuad->m_TriNums.GetValues(),
				pQuad->m_TriNums.GetAmount());
		}
		pQuad = NULL;
		pQuads.Clear();
		// Copying nAllTriangles to pVBMRef->nTriangles whose at least one vertex is in r
		// and calculating bound rect
		N = nAllTriangles.GetAmount();
		if(N == 0) return;
		int nTri = 0, i0 = 0, i1 = 0, i2 = 0, r2 = 0;
		r2 = r * r;
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
				pVBMRef->m_TriNums.Add(nTri);
			}
		}
		nAllTriangles.Clear();
}

// VertexBasedMeshRef::reIndex
void VertexBasedMeshRef::reIndex(DWORDS &reInds, const DWORDS &VertNums, const cHashIndex &reIndexingTable) const {
	assert(pOrigin);
	reInds.Free();

	int ii0, oi, key;
	for(int iTri = 0; iTri < m_TriNums.NValues; iTri++) {
		int ii0 = 3 * m_TriNums[iTri];
		for(int k = 0; k < 3; k++) {
			oi = pOrigin->Idxs[ii0 + k];
			key = reIndexingTable.GenerateKey(oi);
			for(int index = reIndexingTable.First(key); index != -1; index = reIndexingTable.Next(index)) {
				if(VertNums[index] == oi) break;
			}
			assert(index != -1);
			reInds.Add(index);
		}
	}
} // VertexBasedMeshRef::reIndex

// VertexBasedMeshRef::ExtractUniqueVerts
void VertexBasedMeshRef::ExtractUniqueVerts(DWORDS &VertNums, cHashIndex &reIndexingTable) const {
	assert(pOrigin);
	VertNums.Free();
	reIndexingTable.Clear();
	if(!m_TriNums.NValues) return;

	ints oiEntries;
	oiEntries.Lock(3 * m_TriNums.NValues, -1); // Vert count surely will be <= ind count
	int ii0, k, oi, key, index;
	for(int iTri = 0; iTri < m_TriNums.NValues; iTri++) {
		ii0 = 3 * m_TriNums[iTri];
		for(k = 0; k < 3; k++) {
			oi = pOrigin->Idxs[ii0 + k];
			key = reIndexingTable.GenerateKey(oi);
			for(index = reIndexingTable.First(key); index != -1; index = reIndexingTable.Next(index)) {
				if(oiEntries[index] == oi) break;
			}
			if(index == -1) {
				index = VertNums.Add(oi);
				reIndexingTable.Add(key, index);
				oiEntries[index] = oi;
			}
		}
	}
} // VertexBasedMeshRef::ExtractUniqueVerts

//void VertexBasedMeshRef::SupplementShadow(const CSun *pSun)
void VertexBasedMeshRef::SupplementShadow(const SurfaceShader &Shader) {
	assert(pOrigin);
	DWORDS VertNums;
	cHashIndex reIndexingTable;
	ExtractUniqueVerts(VertNums, reIndexingTable);
	for(int i = 0; i < VertNums.Count(); i++) {
		Shader.ShadeOneVertex(&pOrigin->Vert[VertNums[i]]);
	}
}

// VertexBasedMesh::Shade
void VertexBasedMesh::Shade(const SurfaceShader &Shader, eSSIllumination Illumination) {
	Shader.Shade(this, Illumination);
} // VertexBasedMesh::Shade

// VertexBasedMeshRef::AddRef(const VertexBasedMeshRef &)
void VertexBasedMeshRef::AddRef(const VertexBasedMeshRef &Ref) {
	assert(pOrigin == Ref.pOrigin);
	m_TriNums += Ref.m_TriNums;
}

// VertexBasedMeshRef::AddRef(int)
void VertexBasedMeshRef::AddRef(int nTri) {
	assert(pOrigin);
	assert(nTri >= 0);
	assert(nTri < pOrigin->Idxs.NValues / 3);
	m_TriNums += nTri;
}

// VertexBasedMeshRef::GetRefCylinder
void VertexBasedMeshRef::GetRefCylinder(int cx, int cy, int r, VertexBasedMeshRef &Ref) const {
	assert(pOrigin);
	Ref.Free();
	Ref.pOrigin = pOrigin;
	// Enumerating tris and extracting requested:
	int nTri, ii0, i0, i1, i2;
	for(int iTri = 0; iTri < m_TriNums.NValues; iTri++) {
		nTri = m_TriNums[iTri];
		ii0 = 3 * nTri;
		i0 = pOrigin->Idxs[ii0], i1 = pOrigin->Idxs[ii0 + 1], i2 = pOrigin->Idxs[ii0 + 2];
		const OneMapVertex &v0 = pOrigin->Vert[i0], &v1 = pOrigin->Vert[i1], &v2 = pOrigin->Vert[i2];
		// selection criterion:
		if(InRound(cx, cy, r, v0) && InRound(cx, cy, r, v1) && InRound(cx, cy, r, v2))
			Ref.AddRef(nTri);
	}
} // VertexBasedMeshRef::GetRefCylinder

// UniversalMap::QuadsTotalBRect
void UniversalMap::QuadsTotalBRect(const ints &QuadNums, int &x0, int &y0, int &x1, int &y1) const {
	x0 = y0 = x1 = y1 = 0;
	if(!QuadNums.Count()) return;
	x0 = comms::IntMax;
	y0 = comms::IntMax;
	x1 = comms::IntMin;
	y1 = comms::IntMin;
	// Enuming quads:
	int iQ, iQx, iQy, Qx0, Qy0;
	for(int i = 0; i < QuadNums.Count(); i++) {
		iQ = QuadNums[i];
		iQx = iQ % QuadsNx, iQy = iQ / QuadsNx;
		Qx0 = QuadX0 + QuadLx * iQx;
		Qy0 = QuadY0 + QuadLy * iQy;
		x0 = Min(x0, Qx0);
		y0 = Min(y0, Qy0);
		x1 = Max(x1, Qx0 + QuadLx);
		y1 = Max(y1, Qy0 + QuadLy);
	}
} // UniversalMap::QuadsTotalBRect

// UniversalMap::GetQuadsRefAll
void UniversalMap::GetQuadsRefAll(VertexBasedMeshRef &Ref) {
	Ref.Free();
	Ref.pOrigin = this;
	// Ref to all quads:
	for(int iQ = 0; iQ < m_pQuads.NValues; iQ++) {
		Ref.AddRef(*m_pQuads[iQ]);
	}
} // UniversalMap::GetQuadsRefAll

// UniversalMap::QuadsInCylinder
void UniversalMap::QuadsInCylinder(int cx, int cy, int r, ints &QuadNums) {
	QuadNums.Free();
	// Quad rough radius:
	int rQ = sqrt(Square(QuadLx) + Square(QuadLy));
	// Selection radius:
	int R2 = Square(r + rQ);
	// Enumerating quads and extracting requested:
	int iQx, iQy, cQx, cQy;
	for(int iQ = 0; iQ < m_pQuads.NValues; iQ++) {
		iQx = iQ % QuadsNx, iQy = iQ / QuadsNx;
		cQx = QuadX0 + QuadLx * iQx + QuadLx / 2;
		cQy = QuadY0 + QuadLy * iQy + QuadLy / 2;
		if(Square(cQx - cx) + Square(cQy - cy) < R2) QuadNums.Add(iQ);
	}
} // UniversalMap::QuadsInCylinder

// UniversalMap::QuadsRefCylinder
void UniversalMap::QuadsRefCylinder(int cx, int cy, int r, VertexBasedMeshRef &Ref, eRefAction RefAction) {
	Ref.Free();
	Ref.pOrigin = this;
	// Requested quads:
	ints QuadNums;
	QuadsInCylinder(cx, cy, r, QuadNums);
	// Ref to requested quads:
	for(int i = 0; i < QuadNums.Count(); i++) {
		Ref.AddRef(*m_pQuads[QuadNums[i]]);
		if(RefAction == CutRef) m_pQuads[QuadNums[i]]->m_TriNums.Clear();
	}
} // UniversalMap::QuadsRefCylinder

// UniversalMap::GetQuadsRefCylinder
void UniversalMap::GetQuadsRefCylinder(int cx, int cy, int r, VertexBasedMeshRef &Ref) {
	QuadsRefCylinder(cx, cy, r, Ref, GetRef);
} // UniversalMap::GetQuadsRefCylinder

// UniversalMap::CutQuadsRefCylinder
void UniversalMap::CutQuadsRefCylinder(int cx, int cy, int r, VertexBasedMeshRef &Ref) {
	QuadsRefCylinder(cx, cy, r, Ref, CutRef);
} // UniversalMap::CutQuadsRefCylinder

// ToolChangeHeightParabolic
void ToolChangeHeightParabolic(int cx, int cy, int r, int dH, OneMapVertex &v) {
	if(r < 1) return;

	float dx2 = Square(int(v.x) - cx), dy2 = Square(int(v.y) - cy);
	float l = sqrt(dx2 + dy2);
	if(l < r) {
		float dz = - dH * (Square(l) / float(Square(r)) - 1);
		v.z += short(dz);
	}
} // ToolChangeHeightParabolic

// ToolDeformParabolic
void ToolDeformParabolic(const cVector &Orig, const cVector &Dir, int r, int dH, OneMapVertex &v) {
	if(r < 1) return;

	cVector vec(v.x, v.y, v.z);
	double l = vec.DistToLine(Orig, Dir);
	if(l < r) {
		double k = - (Square(l) / double(Square(r)) - 1.);
		//cVector d = k * Dir;
		cVector d = k * dH * Dir;
		v.x += d.x;
		v.y += d.y;
		v.z += d.z;
	}
} // ToolDeformParabolic

// ToolDeformPlanar
void ToolDeformPlanar(const cVector &Orig, const cVector &Dir, int r, int dH, OneMapVertex &v) {
	if(r < 1) return;

	cVector vec(v.x, v.y, v.z);
	double l = vec.DistToLine(Orig, Dir);
	if(l < r) {
		cVector d = dH * Dir;
		v.x += d.x;
		v.y += d.y;
		v.z += d.z;
	}
} // ToolDeformPlanar

// ToolNormalParabolicDeformer
void ToolNormalParabolicDeformer(int cx, int cy, int r, int dH, OneMapVertex &v) {
	if(r < 1) return;

	float dx2 = Square(int(v.x) - cx), dy2 = Square(int(v.y) - cy);
	float l = sqrt(dx2 + dy2);
	if(l < r) {
		float d = - dH * (Square(l) / float(Square(r)) - 1);
		cVector N(v.nx, v.ny, v.nz);
		if(cUtils::IsZero(N.Length())) return;
		N.Normalize();
		v.x += short(N.x * d);
		v.y += short(N.y * d);
		v.z += short(N.z * d);
	}
}

// ToolPlanarDeformer
void ToolPlanarDeformer(int cx, int cy, int r, int dH, OneMapVertex &v) {
	float l = sqrt(Square(cx - int(v.x)) + Square(cy - int(v.y)));
	if(l <= r) v.z += short(dH);
} // ToolPlanarDeformer

// ToolFacturizeOneVertex
void ToolFacturizeOneVertex(OneMapVertex &v, int nFacture) {
	const int min_PlanarMapping_nz  = 70, max_VerticalMapping_nz = 110;
	int GetFactureUsage(int);
	int nUsage = GetFactureUsage(nFacture);
	if(nUsage == 0 && v.nz >= min_PlanarMapping_nz) v.nFacture = nFacture;
    if(nUsage == 1 && v.nz <= max_VerticalMapping_nz) v.nFacture = nFacture;
} // ToolFacturizeOneVertex

// ToolColorizeOneVertex
void ToolColorizeOneVertex(int cx, int cy, int r, int d, DWORD *pColors, int nColors, OneMapVertex &v) {
	if(!nColors) return;
	//              |   d   |
	// ----------------------
	// |          r         |
	// |____________________|
	if(r < 1 || d >= r || d < 1) return;

	int r_d = r - d;

	DWORD Color = pColors[rand() % nColors];
	float t = 0, fR = 0, fG = 0, fB = 0;
	int cR = 0, cG = 0, cB = 0, cr = 0, cg = 0, cb = 0, l = 0;
	const float cK = 1.0f;
	DWORD clr = 0;
	l = sqrt((cx - v.x) * (cx - v.x) + (cy - v.y) * (cy - v.y));
	if(l < r) // we should change color of this vertex
	{
		if(l <= r_d) v.Color = Color; // in this radius setting color without any gradient
		else
		{
			t = float(l - r_d) / (cK * float(d));
			if(t > cK) t = cK;
			// cR, cG, cB
			clr = v.Color;
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
			v.Color = cB + (cG << 8) + (cR << 16) + 0xFF000000;
		}
	}
} // ToolColorizeOneVertex

// ToolEvenHeight
void ToolEvenHeight(int cx, int cy, int r, VertexBasedMeshRef &Ref, DWORDS &VertNums) {
	// Calculating base high
/*	int N = Ref.m_TriNums.GetAmount();
	if(!N) return;
	const float cNotIntersected = -9999.99f;
	float Z = cNotIntersected, T = 0;
	for(int i = 0; i < N; i++)
	{
		int nTri = Ref.m_TriNums[i];
		int i0 = Ref.pOrigin->Idxs[3 * nTri + 0],
			i1 = Ref.pOrigin->Idxs[3 * nTri + 1],
			i2 = Ref.pOrigin->Idxs[3 * nTri + 2];
		OneMapVertex &rvtx0 = Ref.pOrigin->Vert[i0], 
			&rvtx1 = Ref.pOrigin->Vert[i1],
			&rvtx2 = Ref.pOrigin->Vert[i2];
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
		if(zXTri(&T, v3dA, v3dB, v3dC))
			if(T > Z) Z = T;
	}
	if(Z == cNotIntersected) return;*/
	int z = Ref.CalcMaxZ(cx, cy);
	if(z == comms::IntMin) return;
	// Enumerating all verts and assigning new z:
	for(int i = 0; i < VertNums.Count(); i++) {
		OneMapVertex &v = Ref.pOrigin->Vert[VertNums[i]];
		v.z = z;
	}
} // ToolEvenHeight

// ToolExtrude
void ToolExtrude(const cVector &C, double R, VertexBasedMesh *pOrigin, DWORDS &VertNums) {
	assert(pOrigin);
	// Enumerating verts:
	for(int i = 0; i < VertNums.Count(); i++) {
		OneMapVertex &v = pOrigin->Vert[VertNums[i]];
		/*cVector Dir = cVector(v.x, v.y, v.z) - C;
		if(cUtils::IsZero(Dir.Length())) continue;
		Dir.Normalize();
		Dir *= R;
		Dir += C;
		v.Set(short(Dir.x), short(Dir.y), short(Dir.z));*/
	}
} // ToolExtrude

// UniversalMap::ToolOrigDir
bool UniversalMap::ToolOrigDir(const DWORDS &VertNums, cVector &Orig, cVector &Dir) const {
	if(!VertNums.Count()) return false;
    
	cVectors vecs;
	for(int i = 0; i < VertNums.Count(); i++) {
		const OneMapVertex &vtx = Vert[VertNums[i]];
		vecs.Add(cVector(vtx.x, vtx.y, vtx.z));
	}
	return cSolver::AggregateOfPointsApproximation(vecs, Orig, Dir, 1.);
} // UniversalMap::ToolOrigDir

//#define TOOL_TIMING // COMMENT THIS!

#ifdef TOOL_TIMING
LARGE_INTEGER liTBeginTime, liTEndTime;
LONGLONG llTTotalTime = 0;
DWORD TCallsCount = 0;
#endif // TOOL_TIMING

// UniversalMap::Tool
void UniversalMap::Tool(eToolType ToolType, const ToolParams &Params) {
#ifdef TOOL_TIMING
	assert(QueryPerformanceCounter(&liTBeginTime));
#endif // TOOL_TIMING

	const int rMargin = 100;
	int cx = Params.cx, cy = Params.cy, r = Params.r, dH = Params.dH, d = Params.d;
	cList<OneMapVertex> srcList, destList; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//	VertexBasedMeshRef TRef;
	// Refs:
	int R = r + rMargin;
	ints QuadNums;
	QuadsInCylinder(cx, cy, R, QuadNums);
	// Ref to requested quads:
	VertexBasedMeshRef QuadsRef;
	QuadsRef.pOrigin = this;
	for(int i = 0; i < QuadNums.Count(); i++) {
		QuadsRef.AddRef(*m_pQuads[QuadNums[i]]);
	}
	VertexBasedMeshRef Ref;
	QuadsRef.GetRefCylinder(cx, cy, r, Ref);

	// Extracting verts to tool:
	DWORDS VertNums;
	cHashIndex reIndexingTable;
	Ref.ExtractUniqueVerts(VertNums, reIndexingTable);
	if(!VertNums.Count()) return;

	cVectors ns, vecs;
	cVector Orig, Dir;

	// Tooling:
	bool fGeometryIsChanged = false;
	switch(ToolType) {
		case TTChangeHeightParabolic:
			for(int i = 0; i < VertNums.Count(); i++) {
				ToolChangeHeightParabolic(cx, cy, r, dH, Vert[VertNums[i]]);
				//ToolNormalParabolicDeformer(cx, cy, r, dH, Vert[VertNums[i]]);
			}
			fGeometryIsChanged = true;
			break;
		case TTDeformParabolic:
			if(ToolOrigDir(VertNums, Orig, Dir)) {
				fGeometryIsChanged = true;
				for(i = 0; i < VertNums.Count(); i++) {
					ToolDeformParabolic(Orig, Dir, r, dH, Vert[VertNums[i]]);
				}
				Ref.Relax(VertexBasedMeshRef::RelaxArgs(0.2, 1, true));
			}
			break;
		case TTDeformPlanar:
			if(ToolOrigDir(VertNums, Orig, Dir)) {
				fGeometryIsChanged = true;
				for(i = 0; i < VertNums.Count(); i++) {
					ToolDeformPlanar(Orig, Dir, r, dH, Vert[VertNums[i]]);
				}
			}
			break;
		case TTChangeHeightPlanar:
			for(int i = 0; i < VertNums.Count(); i++) {
				ToolPlanarDeformer(cx, cy, r, dH, Vert[VertNums[i]]);
			}
			fGeometryIsChanged = true;
			break;
		case TTEvenHeight:
			ToolEvenHeight(cx, cy, r, Ref, VertNums);
			fGeometryIsChanged = true;
			break;
		case TTSmooth:
			Ref.Relax();
			fGeometryIsChanged = true;
			break;
		case TTExtrude:
			for(int i = 0; i < VertNums.Count(); i++) {
				ToolNormalParabolicDeformer(cx, cy, r, 10, Vert[VertNums[i]]);
			}
			fGeometryIsChanged = true;
//			ToolExtrude(cVector(cx, cy, Params.cz), r, Ref.pOrigin, VertNums);
			//Fn.Create(Vector3D(cx, cy, Params.cz), Vector3D(Params.Dir.x, Params.Dir.y, Params.Dir.z), r);
//			Fn.Create(Vector3D(cx, cy, Params.cz), Vector3D(0, 0, -1), r);
//			Fn.Create(Vector3D(cx, cy, Params.cz));
//			for(int i = 0; i < VertNums.Count(); i++) {
//				OneMapVertex &v = Vert[VertNums[i]];
//				Fn.ShiftVertex(v);
//				srcList.Add(Vert[VertNums[i]]);
//				destList.Add(Vert[VertNums[i]]);
//				OneMapVertex &v = destList[i];
//				Fn.ShiftVertex(v);
//			}

			//Ref.ModifySurface(2, &Fn);
//			GetQuadsRefAll(TRef);
//			Fn.DeleteDirectionalSurface(TRef);
//			fGeometryIsChanged = true;
			break;
		case TTFacturize:
			if(GetKeyState(VK_CAPITAL)){
				void TestRuleApply(int x,int y,int r);
				TestRuleApply(cx,cy,r);
			}else{
				for(int i = 0; i < VertNums.Count(); i++) {
					ToolFacturizeOneVertex(Vert[VertNums[i]], Params.nFacture);
				}
			}
			break;
		case TTColorize:			
			for(int i = 0; i < VertNums.Count(); i++) {
				ToolColorizeOneVertex(cx, cy, r, d, Params.pColors, Params.nColors, Vert[VertNums[i]]);
			}
			break;
		case TTSmoothColors:
			Ref.SmoothColors();
			break;
		default:
			break;
	}
	// If geometry is changed:
	if(fGeometryIsChanged) {
		QuadsRef.CalcNormals(cx, cy, R);
		Ref.SupplementShadow(m_Shader);
		Ref.FillMapOfHeights();
		for(i = 0; i < QuadNums.Count(); i++) {
			m_pQuads[QuadNums[i]]->Init();
		}
	}

	// Invalidating:
	Rct rc(cx - R, cy - R, 2 * R, 2 * R);
	ITerra->InvalidateGeometry(&rc);

#ifdef TOOL_TIMING
	assert(QueryPerformanceCounter(&liTEndTime));
	llTTotalTime += liTEndTime.QuadPart - liTBeginTime.QuadPart;
	TCallsCount++;
	if(GetKeyState(VK_LSHIFT) & 0x8000) {
		// in counts:
		char Str[4096], Buffer[1024];
		LONGLONG llAverageTime = llTTotalTime / TCallsCount;
		sprintf(Str, "Average <UniversalMap::Tool> timing: %d (counts)", llAverageTime);
		// in ms (accurate within .01):
		LARGE_INTEGER liFrequency;
		assert(QueryPerformanceFrequency(&liFrequency));
		llAverageTime *= 100000;
		llAverageTime /= liFrequency.QuadPart;
		double ms = double(llAverageTime) / 100.;
		sprintf(Buffer, " / %.2f (ms)", ms);
		strcat(Str, Buffer);

		MessageBox(NULL, Str, "--- Info ---", 0);
	}
#endif // TOOL_TIMING
} // UniversalMap::Tool

// UniversalMap::AdvTool
void UniversalMap::AdvTool(eAdvToolType AdvToolType, AdvToolParams &AdvParams) {
	// Extracting verts to tool:
	VertexBasedMeshRef Ref;
	Ref.pOrigin = this;
	Ref.m_TriNums.AddValues(AdvParams.TriNums.Ptr(), AdvParams.TriNums.Count());
	DWORDS VertNums;
	cHashIndex reIndexingTable;
	Ref.ExtractUniqueVerts(VertNums, reIndexingTable);
	if(!VertNums.Count()) return;

	bool fGeometryIsChanged = false;
	// Tooling:
	switch(AdvToolType) {
		case ATColorize:
			for(int i = 0; i < VertNums.Count(); i++) {
				OneMapVertex &v = Vert[VertNums[i]];
				v.Color = AdvParams.argbColor;
			}
			break;
	}

	if(fGeometryIsChanged) {
	}
	// Invalidating:
	OneMapVertex AABBMin, AABBMax;
	Ref.CalcBoundBox(AABBMin, AABBMax);
	const int dMargin = 100;
	Rct rc(AABBMin.x - dMargin, AABBMin.y - dMargin, AABBMax.x - AABBMin.x + 2 * dMargin, AABBMax.y - AABBMin.y + 2 * dMargin);
	ITerra->InvalidateGeometry(&rc);
} // UniversalMap::AdvTool



// ================================================================================
// UniversalMap::ChangeHeightPlanar
// ================================================================================
/*void UniversalMap::ChangeHeightPlanar(int cx, int cy, int r, int dH)
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
//	SupplementMapOfHeightsInRect(VBMRef.x0, VBMRef.y0, VBMRef.x0 + VBMRef.w, VBMRef.y0 + VBMRef.h);
//	FillMapOfHeights(OneMapVertex(VBMRef.x0, VBMRef.y0), OneMapVertex(VBMRef.x0 + VBMRef.w, VBMRef.y0 + VBMRef.h));
	VBMRef.SupplementNormals(cx, cy, r);
	VBMRef.SupplementShadow(m_Shader);
	Rct rc(VBMRef.x0, VBMRef.y0, VBMRef.w, VBMRef.h);
	ITerra->InvalidateGeometry(&rc);
}*/

// ================================================================================
// UniversalMap::EvenHeightInRadius
// ================================================================================
/*void UniversalMap::EvenHeightInRadius(int cx, int cy, int r)
{
	VertexBasedMeshRef VBMRef;
	GetMeshRefCylinder(cx, cy, r, &VBMRef);
	// Calculating base high
	int N = 0;
	N = VBMRef.m_TriNums.GetAmount();
	if(N == 0) return;
	int i = 0, i0 = 0, i1 = 0, i2 = 0, nTri = 0;
	const float cNotIntersected = -9999.99f;
	float Z = cNotIntersected, T = 0;
	for(i = 0; i < N; i++)
	{
		nTri = VBMRef.m_TriNums[i];
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
//	SupplementMapOfHeightsInRect(VBMRef.x0 - offset, VBMRef.y0 - offset,
//		VBMRef.x0 + VBMRef.w + 2 * offset, VBMRef.y0 + VBMRef.h + 2 * offset);
//	FillMapOfHeights(OneMapVertex(VBMRef.x0 - offset, VBMRef.y0 - offset),
//		OneMapVertex(VBMRef.x0 + VBMRef.w + 2 * offset, VBMRef.y0 + VBMRef.h + 2 * offset));
	VBMRef.SupplementNormals(cx, cy, r);
	VBMRef.SupplementShadow(m_Shader);
	Rct rc(VBMRef.x0, VBMRef.y0, VBMRef.w, VBMRef.h);
	ITerra->InvalidateGeometry(&rc);
}*/

// ================================================================================
// UniversalMap::SmoothHeightInRadius
// ================================================================================
/*void UniversalMap::SmoothHeightInRadius(int cx, int cy, int r)
{
	VertexBasedMeshRef VBMRef;
	GetMeshRefCylinder(cx, cy, r, &VBMRef);
	OneSplitMesh Splitted;
	Splitted.Split(VBMRef);
	Splitted.Modify(OneMapVertex(0, 0, 10), 0x40000000, 0x00FFFFFF, false);
	Splitted.Draw();
	VBMRef.Smooth(0.5, 2);
	// Invalidating...
//	SupplementMapOfHeightsInRect(VBMRef.x0, VBMRef.y0, VBMRef.x0 + VBMRef.w, VBMRef.y0 + VBMRef.h);
//	FillMapOfHeights(OneMapVertex(VBMRef.x0, VBMRef.y0), OneMapVertex(VBMRef.x0 + VBMRef.w, VBMRef.y0 + VBMRef.h));
	VBMRef.SupplementNormals(cx, cy, r);
	VBMRef.SupplementShadow(m_Shader);
	Rct rc(VBMRef.x0, VBMRef.y0, VBMRef.w, VBMRef.h);
	ITerra->InvalidateGeometry(&rc);
}*/

// VertexBasedMesh::InvertNormals
void VertexBasedMesh::InvertNormals() {
	int NTris = Idxs.NValues / 3;
	for(int nTri = 0; nTri < NTris; nTri++) Swap(Idxs[3 * nTri + 1], Idxs[3 * nTri + 2]);
} // VertexBasedMesh::InvertNormals

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

// UniversalMap::AddRef2Quads
void UniversalMap::AddRef2Quads(int nTri) {
	int ii0 = 3 * nTri;
	int i0 = Idxs[ii0], i1 = Idxs[ii0 + 1], i2 = Idxs[ii0 + 2];
	const OneMapVertex &v0 = Vert[i0], &v1 = Vert[i1], &v2 = Vert[i2];
	int cx = (int(v0.x) + int(v1.x) + int(v2.x)) / 3, cy = (int(v0.y) + int(v1.y) + int(v2.y)) / 3;
	if(cx >= QuadX0 && cx < QuadX0 + UMLx && cy >= QuadY0 && cy < QuadY0 + UMLy) {
		int nQx = (cx - QuadX0) / QuadLx, nQy = (cy - QuadY0) / QuadLy;
		m_pQuads[nQx + QuadsNx * nQy]->m_TriNums.Add(nTri);
	}
} // UniversalMap::AddRef2Quads

void UniversalMap::QuadsSystem_AddRef(const OneMapVertex &v0, const OneMapVertex &v1, const OneMapVertex &v2, int nTri)
{
	int UMLx = QuadLx * QuadsNx, UMLy = QuadLy * QuadsNy, cx = (int(v0.x) + int(v1.x) + int(v2.x)) / 3,
		cy = (int(v0.y) + int(v1.y) + int(v2.y)) / 3;
	if((cx >= QuadX0) && (cx < QuadX0 + UMLx) && (cy >= QuadY0) && (cy < QuadY0 + UMLy))
	{
		int nQx = (cx - QuadX0) / QuadLx, nQy = (cy - QuadY0) / QuadLy;
		m_pQuads[nQx + QuadsNx * nQy]->m_TriNums.Add(nTri);
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
	rRef.Free();
	rRef.pOrigin = this;
	int NTri = Idxs.NValues / 3, r2 = r * r;
	for(int nTri = 0; nTri < NTri; nTri++)
	{
		int i0 = Idxs[3 * nTri + 0], i1 = Idxs[3 * nTri + 1], i2 = Idxs[3 * nTri + 2];
		OneMapVertex &v0 = Vert[i0], &v1 = Vert[i1], &v2 = Vert[i2];
		int x = (int(v0.x) + int(v1.x) + int(v2.x)) / 3,
			y = (int(v0.y) + int(v1.y) + int(v2.y)) / 3;
		int d = (x - cx) * (x - cx) + (y - cy) * (y - cy);
		if(d < r2) rRef.m_TriNums.Add(nTri);
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
		if(!inConst[i0] && !inConst[i1] && !inConst[i2]) Ref.m_TriNums.Add(nTri);
	}

	for(i = 0; i < inInvariable.NValues; i++) if(!inInvariable[i]) rMesh.Vert[i].z = AllZ[i];
	// Smoothing:
	Ref.Relax(VertexBasedMeshRef::RelaxArgs(0.5, 60, true));

	// unInit:
	inConst.Clear(), inInvariable.Clear();
	Ref.Free();
	AllZ.Clear();
}

// UniversalMap::Add(const OneMapVertex &, const OneMapVertex &, const OneMapVertex &)
int UniversalMap::Add(const OneMapVertex &v0, const OneMapVertex &v1, const OneMapVertex &v2) {
	int NVerts = Vert.NValues;
	int NTris = Idxs.NValues / 3;
	Vert.Add(v0), Vert.Add(v1), Vert.Add(v2);
	for(int k = 0; k < 3; k++) Idxs.Add(NVerts + k);
	AddRef2Quads(NTris);
	return NTris;
} // UniversalMap::Add(const OneMapVertex &, const OneMapVertex &, const OneMapVertex &)

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
		m_pQuads[QuadsNx * nQy + nQx]->m_TriNums.Add(nTri);
	}

	for(int iQ = 0; iQ < m_pQuads.NValues; iQ++) {
		m_pQuads[iQ]->Init();
	}
}



void UniversalMap::QuadsSystem_unInit(void)
{
	for(int nQ = 0; nQ < m_pQuads.NValues; nQ++)
	{
		m_pQuads[nQ]->m_TriNums.Clear();
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

// VertexBasedMeshRef::ConvertTo(VertexBasedMesh &)
void VertexBasedMeshRef::ConvertTo(VertexBasedMesh &Mesh) const {
	assert(pOrigin);
	Mesh.Free();

	// Extracting unique vert inds:
	DWORDS VertInds;
	cHashIndex reIndexingTable(16384, 16384);
	ExtractUniqueVerts(VertInds, reIndexingTable);
	// Storing verts:
	Mesh.Vert.Check(VertInds.Count());
	for(int i = 0; i < VertInds.Count(); i++) {
		Mesh.Vert.Add(pOrigin->Vert[VertInds[i]]);
	}
	// reIndexing tris:
	DWORDS reInds;
	reIndex(reInds, VertInds, reIndexingTable);
	for(i = 0; i < reInds.Count(); i++) { // FIX - IT! Will be changed to reIndex(Mesh.m_Inds, ...
		Mesh.Idxs.Add(reInds[i]);
	}
} // VertexBasedMeshRef::ConvertTo(VertexBasedMesh &)

// VertexBasedMeshRef::PackTo(VertexBasedMesh &)
void VertexBasedMeshRef::PackTo(VertexBasedMesh &Mesh) const {
	assert(pOrigin);
	Mesh.Free();
	cHashIndex reIndexingTable(16384, 16384);
	int nTri, ii0, oi[3], key, index;
	OneMapVertex v[3];
	cVector e0, e1, N;
	for(int iTri = 0; iTri < m_TriNums.NValues; iTri++) {
		nTri = m_TriNums[iTri];
		ii0 = 3 * nTri;
		for(int k = 0; k < 3; k++) oi[k] = pOrigin->Idxs[ii0 + k], v[k] = pOrigin->Vert[oi[k]];
		// If this tri has small square:
		e0.Set(v[1].x - v[0].x, v[1].y - v[0].y, v[1].z - v[0].z);
		e1.Set(v[2].x - v[0].x, v[2].y - v[0].y, v[2].z - v[0].z);
		N = e1.Cross(e0);
		if(cUtils::IsZero(N.Length(), 0.01)) continue; // we should skip it!

		for(int k = 0; k < 3; k++) {
			key = reIndexingTable.GenerateKey(v[k].CalcHashCode());
			for(index = reIndexingTable.First(key); index != -1; index = reIndexingTable.Next(index)) {
				if(Mesh.Vert[index].Equals(v[k])) break;
			}
			if(index == -1) {
				index = Mesh.Vert.Add(v[k]);
				reIndexingTable.Add(key, index);
			}
			Mesh.Idxs.Add(index);
		}
	}
} // VertexBasedMeshRef::PackTo(VertexBasedMesh &)

// UniversalMap::Pack
void UniversalMap::Pack() {
	// Ref to all quads:
	VertexBasedMeshRef Ref;
	Ref.pOrigin = this;
	for(int iQ = 0; iQ < m_pQuads.NValues; iQ++)
		Ref.AddRef(*m_pQuads[iQ]);
	// Packing:
	VertexBasedMesh Mesh;
	Ref.PackTo(Mesh);
	// Finalizing:
	Vert.Clear();
	Vert += Mesh.Vert;
	Idxs.Clear();
	Idxs += Mesh.Idxs;
	QuadsSystem_Init();
} // UniversalMap::Pack

void UniversalMap::Compile(CIndices *pContour)
{
	COneMapVertices cVertices;
	CIndices cIndices;
	nHashTable hTable;
	int flags[] = {0, 1, 2}, oi[3], i[3];
	OneMapVertex Verts[3];
	cVector u, v, N;
	for(int nQ = 0; nQ < m_pQuads.NValues; nQ++)
		for(int iTri = 0; iTri < m_pQuads[nQ]->m_TriNums.NValues; iTri++)
		{
			int nTri = m_pQuads[nQ]->m_TriNums[iTri];
			for(int k = 0; k < 3; k++) oi[k] = Idxs[3 * nTri + k], Verts[k] = Vert[oi[k]];
			u.Set(Verts[1].x - Verts[0].x, Verts[1].y - Verts[0].y, Verts[1].z - Verts[0].z);
			v.Set(Verts[2].x - Verts[0].x, Verts[2].y - Verts[0].y, Verts[2].z - Verts[0].z);
			N = v.Cross(u);
			if(cUtils::IsZero(N.Length(), .01)) continue;
			CtrlCV_Update(oi, i, Verts, cVertices, hTable, flags);
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

void VertexBasedMesh::Sum(const VertexBasedMesh &rMesh)
{
	int V = Vert.NValues;
	Vert += rMesh.Vert;
	for(int i = 0; i < rMesh.Idxs.NValues; i++) Idxs.Add(V + rMesh.Idxs[i]);
}

void UniversalMap::Sum(const VertexBasedMesh &rMesh, CIndices *pContour)
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

bool UniversalMap::MeshIsWithin(const VertexBasedMesh &rMesh) const
{
	int x0, y0, x1, y1;
	rMesh.CalcBoundRect(x0, y0, x1, y1);
	int X0 = QuadX0, Y0 = QuadY0, X1 = QuadX0 + UMLx, Y1 = QuadY0 + UMLy;
	return x0 < X0 || x0 > X1 || x1 < X0 || x1 > X1 || y0 < Y0 || y0 > Y1 || y1 < Y0 || y1 > Y1;
}

void VertexBasedMesh::Offset(const OneMapVertex &rOffset)
{
	for(int nVertex = 0; nVertex < Vert.NValues; nVertex++)
	{
		OneMapVertex &v = Vert[nVertex];
		v.x += rOffset.x;
		v.y += rOffset.y;
		v.z += rOffset.z;
	}
}


int VertexBasedMesh::ClassifyVertex(const OneMapVertex &rVertex, bool bUseBoundBox, int C) const
{
/*	if(C > 10) return VERTEX_BELONGS;
	if(bUseBoundBox && (rVertex.x < m_BBox0.x || rVertex.x > m_BBox1.x || rVertex.y < m_BBox0.y || rVertex.y > m_BBox1.y ||
		rVertex.z < m_BBox0.z || rVertex.z > m_BBox1.z)) return VERTEX_OUTSIDE;
	double P[3] = {rVertex.x, rVertex.y, rVertex.z}, Q[3], T[3][3], t, u, v;

	double Phi = c_DoublePI * double(rand()) / double(RAND_MAX), Theta = c_PI * double(rand()) / double(RAND_MAX);
	double Dir[3] = {sin(Phi) * sin(Theta), cos(Theta), cos(Phi) * sin(Theta)};

	// Computing end point Q:
	const int c_A = 10000, c_dZ = 10000;
	Q[0] = P[0] - c_A / 2 + rand() % c_A,
		Q[1] = P[1] - c_A / 2 + rand() % c_A,
		Q[2] = P[2] + c_dZ;
	// Testing for intersections:
	int r, NTris = Idxs.NValues / 3;
	OneMapVertex V[3];
	int cIntersections = 0;
	for(int nTri = 0; nTri < NTris; nTri++)
	{
		GetTriangle(nTri, V[0], V[1], V[2]);
		for(int k = 0; k < 3; k++) T[k][0] = V[k].x, T[k][1] = V[k].y, T[k][2] = V[k].z;
		r = XTri(P, Dir, T[0], T[1], T[2], t, u, v);
		if(r == 1) cIntersections++;
		if(r == -1) return ClassifyVertex(rVertex, true, C + 1);
	}
	if(cIntersections & 1) return VERTEX_INSIDE;*/
	return VERTEX_OUTSIDE;
}

void UniversalMap::CutRefCylinder(int cx, int cy, int r, VertexBasedMeshRef &rRef)//, DWORDS *pBoundaryVertsNums)
{
	if(!m_pQuads.NValues) return;
	rRef.Free();
	rRef.pOrigin = this;
	int r2 = r * r;
	for(int nQ = 0; nQ < m_pQuads.NValues; nQ++)
		for(int iTri = 0; iTri < m_pQuads[nQ]->m_TriNums.NValues;)
		{
			int nTri = m_pQuads[nQ]->m_TriNums[iTri];
			int i0 = Idxs[3 * nTri + 0], i1 = Idxs[3 * nTri + 1], i2 = Idxs[3 * nTri + 2];
			const OneMapVertex &v0 = Vert[i0], &v1 = Vert[i1], &v2 = Vert[i2];
			int x = (int(v0.x) + int(v1.x) + int(v2.x)) / 3,
				y = (int(v0.y) + int(v1.y) + int(v2.y)) / 3;
			int d2 = (x - cx) * (x - cx) + (y - cy) * (y - cy);
			if(d2 < r2)
			{
				rRef.m_TriNums.Add(nTri);
				m_pQuads[nQ]->m_TriNums.Del(iTri, 1);
				continue;
			}
			iTri++;
		}
}

// VertexBasedMesh2cRawMesh
bool VertexBasedMesh2cRawMesh(const VertexBasedMesh &Src, cRawMesh &Dest) {
	Dest.Free();

	int NTris = Src.Idxs.NValues / 3;
	bool fMultipleBy3 = 3 * NTris == Src.Idxs.NValues;
	assert(fMultipleBy3);
	if(!fMultipleBy3) return false;

	DWORDS Tris;
	for(int i = 0; i < Src.Idxs.NValues; i++) {
		int index = Src.Idxs[i];
		bool fInVertsRange = comms::RangeCheck(index, 0, Src.Vert.NValues - 1);
		assert(fInVertsRange);
		if(!fInVertsRange) return false;
		Tris.Add(index);
	}
	Dest.m_Tris = Tris;

	for(i = 0; i < Src.Vert.NValues; i++) {
		const OneMapVertex &vtx = Src.Vert[i];
		Dest.m_Verts.Add(cVector((double)vtx.x, (double)vtx.y, (double)vtx.z));
	}

	Dest.m_Type = comms::RMTTTris;
	return true;
} // VertexBasedMesh2cRawMesh

// cRawMesh2VertexBasedMesh
bool cRawMesh2VertexBasedMesh(const cRawMesh &Src, VertexBasedMesh &Dest) {
	Dest.Free();

	cRawMesh M = Src;
	if(!M.Type(comms::RMTTTris)) return false;

	int NTris = Src.m_Tris.Count() / 3;
	bool fMultipleBy3 = 3 * NTris == Src.m_Tris.Count();
	assert(fMultipleBy3);
	if(!fMultipleBy3) return false;

	DynArray<DWORD> Idxs;
	for(int i = 0; i < Src.m_Tris.Count(); i++) {
		int index = Src.m_Tris[i];
		bool fInVertsRange = comms::RangeCheck(index, 0, Src.m_Verts.Count() - 1);
		assert(fInVertsRange);
		if(!fInVertsRange) return false;
		Idxs.Add(index);
	}
	Dest.Idxs += Idxs;

	for(i = 0; i < Src.m_Verts.Count(); i++) {
		const cVector &vec = Src.m_Verts[i];
		Dest.Vert.Add(OneMapVertex((short)vec.x, (short)vec.y, (short)vec.z));
	}
	return true;
} // cRawMesh2VertexBasedMesh

#ifdef MAYA_BOOLEANS
// BEGIN: MAYA! MUST BE DELETED!
#include "Maya\include\MFnMesh.h"
#include "Maya\include\MPointArray.h"
#include "Maya\include\MIntArray.h"
#include "Maya\include\MObject.h"
#include "Maya\include\MStatus.h"
#include "Maya\include\MString.h"
#include "Maya\include\MLibrary.h"
#include "Maya\include\MGlobal.h"
#include "Maya\include\MSelectionList.h"
#include "Maya\include\MItSelectionList.h"
#include "Maya\include\MDagPath.h"
#include "Maya\include\MItMeshVertex.h"

bool cRawMesh2MFnMesh(const cRawMesh &Src, MObject &MObject, MFnMesh &MFNMesh) {
	cRawMesh M = Src;
	if(!M.Type(comms::RMTTTris)) return false;

	int NVerts = M.m_Verts.Count(), NTris = M.m_Tris.Count() / 3;
	// Verts:
	MPointArray vertexArray;
	for(int i = 0; i < NVerts; i++) {
		const cVector &vec = M.m_Verts[i];
		vertexArray.append(vec.x, vec.y, vec.z);
	}
	assert(NVerts == vertexArray.length());
	// Tris:
	MIntArray polygonCounts, polygonConnects;
	int i0, i1, i2;
	for(int nTri = 0; nTri < NTris; nTri++) {
		M.GetTri(nTri, i0, i1, i2);
		polygonCounts.append(3);
		polygonConnects.append(i0);
		polygonConnects.append(i1);
		polygonConnects.append(i2);
	}
	assert(NTris == polygonCounts.length());
	assert(3 * NTris == polygonConnects.length());
	// Create:
	MStatus ReturnStatus;
	MObject = MFNMesh.create(NVerts, NTris, vertexArray, polygonCounts, polygonConnects,
		MObject::kNullObj, &ReturnStatus);
	assert(MS::kSuccess == ReturnStatus);
	return true;
}

bool MFnMesh2cRawMesh(const MFnMesh &Src, cRawMesh &Dest) {
	Dest.Free();

	int NVerts = Src.numVertices(), NPolys = Src.numPolygons(),
		NEdges = Src.numEdges(), NFaceVerts = Src.numFaceVertices();

	// Verts:
	MPointArray vertexArray;
	Src.getPoints(vertexArray, MSpace::kWorld);
	int i0, i1, i2;
	for(int i = 0; i < vertexArray.length(); i++) {
		const MPoint &p = vertexArray[i];
		Dest.m_Verts.Add(cVector(p.x, p.y, p.z));
	}
	// Tris:
	MStatus ReturnStatus;
	MIntArray vertexList;
	for(i = 0; i < Src.numPolygons(&ReturnStatus); i++) {
		assert(MS::kSuccess == ReturnStatus);
		assert(MS::kSuccess == Src.getPolygonVertices(i, vertexList));
		assert(3 == vertexList.length());
		i0 = vertexList[0], i1 = vertexList[1], i2 = vertexList[2];
		assert(i0 >= 0);
		assert(i0 < Dest.m_Verts.Count());
		assert(i1 >= 0);
		assert(i1 < Dest.m_Verts.Count());
		assert(i2 >= 0);
		assert(i2 < Dest.m_Verts.Count());
		Dest.m_Tris.Add(i0);
		Dest.m_Tris.Add(i1);
		Dest.m_Tris.Add(i2);
	}
	Dest.m_Type = comms::RMTTTris;
	return true;
}
// END: MAYA! MUST BE DELETED!
#endif // MAYA_BOOLEANS

#include "SurfaceGenerator.h"

// ================================================================================
// UniversalMap::Combine
// ================================================================================
void UniversalMap::Combine(VertexBasedMesh &Mesh, eCombineOp CombineOp) {
#ifdef MAYA_BOOLEANS
	// BEGIN: MAYA! MUST BE DELETED!

	extern MStatus g_MayaStatus;
	if(!g_MayaStatus)return;

	static int NumCalls = 0;

	// RawMeshA && RawMeshB, VertexBasedMeshA:
	VertexBasedMeshRef Ref;
	Mesh.GetRefAll(Ref);
	cRawMesh RawMeshB;
	VertexBasedMesh vbMeshT;
	Ref.PackTo(vbMeshT);
	assert(VertexBasedMesh2cRawMesh(vbMeshT, RawMeshB));
	cVector brCentre;
	double brRadius;
	RawMeshB.m_Verts.BRadius(brCentre, brRadius);

	CutRefCylinder((int)brCentre.x, (int)brCentre.y, (int)brRadius, Ref);
	VertexBasedMesh VertexBasedMeshA;
	Ref.ConvertTo(VertexBasedMeshA);
	if(CombineOp == DifferenceOp) VertexBasedMeshA.InvertNormals(); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	cRawMesh RawMeshA;
	assert(VertexBasedMesh2cRawMesh(VertexBasedMeshA, RawMeshA));
	// Maya:
	// A:
	MObject newTransformA;
	MFnMesh fnPolyA;
	assert(cRawMesh2MFnMesh(RawMeshA, newTransformA, fnPolyA));
	assert(fnPolyA.numVertices() == RawMeshA.m_Verts.Count());
	assert(fnPolyA.numPolygons() == RawMeshA.m_Tris.Count() / 3);
	// B:
	MObject newTransformB;
	MFnMesh fnPolyB;
	assert(cRawMesh2MFnMesh(RawMeshB, newTransformB, fnPolyB));
	assert(fnPolyB.numVertices() == RawMeshB.m_Verts.Count());
	assert(fnPolyB.numPolygons() == RawMeshB.m_Tris.Count() / 3);
	// Selecting operands:
	assert(MGlobal::selectByName(fnPolyA.name(), MGlobal::kReplaceList) == MS::kSuccess);
	assert(MGlobal::selectByName(fnPolyB.name(), MGlobal::kAddToList) == MS::kSuccess);
	// executing polyBoolOp:
	MString BoolCommand("polyBoolOp -op ");
	BoolCommand += "1 ";
//	if(CombineOp == UnionOp) BoolCommand += "1 ";
//	else if(CombineOp == DifferenceOp) BoolCommand += "2 ";
//	else return;
	BoolCommand += fnPolyB.name();
	BoolCommand += " ";
	BoolCommand += fnPolyA.name();
	assert(MS::kSuccess == MGlobal::executeCommand(BoolCommand));
	// Result:
	MString ResultName("polySurface");
	ResultName += (3 * (NumCalls + 1));
	// polyTriangulate:
	MString TriCommand("polyTriangulate ");
	TriCommand += ResultName;
	assert(MS::kSuccess == MGlobal::executeCommand(TriCommand));
	// Selecting result (object mode):
	assert(MS::kSuccess == MGlobal::selectByName(ResultName, MGlobal::kReplaceList));

	// Extracting result:
	MSelectionList slist;
	assert(MGlobal::getActiveSelectionList(slist) == MS::kSuccess);
	MItSelectionList iter(slist);
	assert(!iter.isDone());

	MDagPath objectPath;
	MObject component;
	int N = 0;
	VertexBasedMesh MeshR;
	MObject mesh;
	MStatus ReturnStatus;
	cRawMesh RawMeshC;
	for(; !iter.isDone(); iter.next()) {
		ReturnStatus = iter.getDagPath(objectPath, component);
		assert(MS::kSuccess == ReturnStatus);
		if(objectPath.hasFn(MFn::kMesh)) {
			assert(MS::kSuccess == objectPath.extendToShape());
			mesh = objectPath.node(&ReturnStatus);
			assert(MS::kSuccess == ReturnStatus);
			MFnMesh C(mesh, &ReturnStatus);
			assert(MS::kSuccess == ReturnStatus);
			if(MFnMesh2cRawMesh(C, RawMeshC)) {
				VertexBasedMesh vbMeshR;
				if(cRawMesh2VertexBasedMesh(RawMeshC, vbMeshR)) {
					if(CombineOp == DifferenceOp) vbMeshR.InvertNormals(); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
					vbMeshR.CalcNormals();
					vbMeshR.Shade(m_Shader, SSLocalIllumination);
					extern SurfaceGenerator SURF;
					SURF.TexturizeSurface(vbMeshR);
					vbMeshR.GetRefAll(Ref);
					Ref.FillMapOfHeights();
					Sum(vbMeshR);
					Pack();
					NumCalls++;
				}
			}
		}
	}
	// END: MAYA! MUST BE DELETED!
#endif // MAYA_BOOLEANS
}

//-----------------------------------------------------------------------------
// UniversalMap::Create : void(int, DWORD)
//-----------------------------------------------------------------------------
void UniversalMap::Create(int Size, DWORD Color) {
	Free();
	GenHexagonalPlane(Size, Size, 64);
	for(int i = 0; i < Vert.NValues; i++) {
		OneMapVertex &r = g_UniversalMap.Vert[i];
		r.Color = Color;
		r.Shadow = 128;
		r.z = 50;
		r.nFacture=0;
	}
	VertexBasedMeshRef Ref;
	GetRefAll(Ref);
	Ref.FillMapOfHeights();
	QuadsSystem_Init();
	CalcNormals();

	void InitLayersSystem();
	InitLayersSystem();
}

//-------------------------------------------------------------------------------------------------
// UniversalMap::PickTri : int(const OneMapVertex &, const OneMapVertex &, const OneMapVertex &)
//-------------------------------------------------------------------------------------------------
int UniversalMap::PickTri(const OneMapVertex &t0, const OneMapVertex &t1, const OneMapVertex &t2) {
	const int x1 = t0.x, x2 = t1.x, x3 = t2.x;
	const int y1 = t0.y, y2 = t1.y, y3 = t2.y;

	// Deltas:
	const int Dx12 = x1 - x2, Dx23 = x2 - x3, Dx31 = x3 - x1;
	const int Dy12 = y1 - y2, Dy23 = y2 - y3, Dy31 = y3 - y1;

	// Constant parts of half - space functions:
	int C1 = Dy12 * x1 - Dx12 * y1;
	int C2 = Dy23 * x2 - Dx23 * y2;
	int C3 = Dy31 * x3 - Dx31 * y3;

	int S1 = Sign(C1);
	int S2 = Sign(C2);
	int S3 = Sign(C3);

	// Edge t0 - t1:
	if(S1 == 0 && ((S2 < 0 && S3 < 0) || (S2 > 0 && S3 > 0))) {
		return (t0.z + t1.z) / 2;
	}
	// Edge t1 - t2:
	if(S2 == 0 && ((S1 < 0 && S3 < 0) || (S1 > 0 && S3 > 0))) {
		return (t1.z + t2.z) / 2;
	}
	// Edge t0 - t2:
	if(S3 == 0 && ((S1 < 0 && S2 < 0) || (S1 > 0 && S2 > 0))) {
		return (t0.z + t2.z) / 2;
	}
	// Inside tri:
	if((S1 < 0 && S2 < 0 && S3 < 0) || (S1 > 0 && S2 > 0 && S3 > 0)) {
		return (t0.z + t1.z + t2.z) / 3;
	}
	return 0;
}

//-----------------------------------------------------------------------------
// UniversalMap::Pick : bool(PickArgs *)
//-----------------------------------------------------------------------------
bool UniversalMap::Pick(PickArgs *pArgs) {
	pArgs->Free();
	pArgs->RefTo.pOrigin = this;

	// GetPickRay -> Orig, Dir:
	cVector Orig, Dir;
	Line3D l = ICam->GetPickRay(pArgs->xScreen, pArgs->yScreen);
	Vector3D O = l.Orig(), D = l.Dir();
	Orig.Set(O.x, O.y, O.z);
	Dir.Set(D.x, D.y, D.z);

	// Ref to potential Tris:
	ints QuadNums;
	for(int iQ = 0; iQ < m_pQuads.NValues; iQ++) {
		const OneMapVertex &src = m_pQuads[iQ]->m_Centre;
		cVector vec(src.x, src.y, src.z);
		double l = vec.DistToLine(Orig, Dir);
		if(l < m_pQuads[iQ]->m_Radius + pArgs->Radius) {
			QuadNums.Add(iQ);
		}
	}

	VertexBasedMeshRef Ref;
	Ref.pOrigin = this;
	for(int i = 0; i < QuadNums.Count(); i++) {
		Ref.AddRef(*m_pQuads[QuadNums[i]]);
	}

	// VertNums, Tris:
	DWORDS VertNums;
	cHashIndex reIndexingTable;
	Ref.ExtractUniqueVerts(VertNums, reIndexingTable);
	if(!VertNums.Count()) return false;
	DWORDS Tris;
	Ref.reIndex(Tris, VertNums, reIndexingTable);

	// Projecting verts:
	OneMapVerts pVerts;
	pVerts.Lock(VertNums.Count());
	cMatrix T = cMatrix::OfTransform(Dir, Orig);
	cVector vec;
	for(int i = 0; i < VertNums.Count(); i++) {
		const OneMapVertex &src = Vert[VertNums[i]];
		vec.Set(src.x, src.y, src.z);
		vec *= T;
		pVerts[i].Set((short)vec.x, (short)vec.y, (short)vec.z);
	}

	// Nearest nTri -> nPicked:
	int ii0, k, Z, zMax = comms::IntMin, nPickedTri = -1;
	for(int nTri = 0; nTri < Ref.m_TriNums.NValues; nTri++) {
		ii0 = 3 * nTri;
		const OneMapVertex &p0 = pVerts[Tris[ii0 + 0]],
			&p1 = pVerts[Tris[ii0 + 1]],
			&p2 = pVerts[Tris[ii0 + 2]];
		// Picking:
		if(!p0.x && !p0.y) Z = p0.z;
		else if(!p1.x && !p1.y) Z = p1.z;
		else if(!p2.x && !p2.y) Z = p2.z;
		else Z = PickTri(p0, p1, p2);
		if(Z != 0 && Z > zMax) {
			zMax = Z;
			nPickedTri = nTri;
		}
	}

	if( -1 == nPickedTri ) return false;

	// Nearest nVert -> nPickedVert;
	int d2Min = comms::IntMax, d2, nPickedVert = -1;
	for(k = 0; k < 3; k++) {
		i = Tris[3 * nPickedTri + k];
		const OneMapVertex &p = pVerts[i];
		d2 = Square(Square((int)p.x) + Square((int)p.y));
		if(d2 < d2Min) {
			d2Min = d2;
			nPickedVert = i;
		}
	}

	// JustPick :
	if(PickArgs::JustPick == pArgs->Type) {
		int oiPickedVert = VertNums[nPickedVert];
		pArgs->VertsTo.Add(oiPickedVert);
		int oiPickedTri = Ref.m_TriNums[nPickedTri];
		pArgs->RefTo.AddRef(oiPickedTri);
		return true;
	}

	// inSphere flags:
	cBitMap inSphere(pVerts.Count());
	const OneMapVertex &c = pVerts[nPickedVert];
	for(i = 0; i < pVerts.Count(); i++) {
		const OneMapVertex &v = pVerts[i];
		if(InSphere(c, pArgs->Radius, v)) inSphere.Set(i);
		else inSphere.Clear(i);
	}
	inSphere.Set(nPickedVert);

	cBitMap IsInval(VertNums.Count());
	// Extracting verts in sphere along surface:
	// Initing with nPickedVert:
	int index = pArgs->VertsTo.Add(nPickedVert);
	cHashIndex svHash;
	int key = svHash.GenerateKey(nPickedVert);
	svHash.Add(key, index);
	ints svEntries;
	svEntries.Lock(VertNums.Count(), -1);
	svEntries[index] = nPickedVert;
	// Enuming tris:
	int nTri, TriCount = Tris.Count() / 3, inds[3], i0, i1, i2;
	cBitMap RefToMap(TriCount);
	while(true) {
		bool fAdded = false;
		for(nTri = 0; nTri < TriCount; nTri++) {
			ii0 = 3 * nTri;
			for(k = 0; k < 3; k++) {
				inds[k] = Tris[ii0 + k];
			}
			for(k = 0; k < 3; k++) {
				i0 = k, i1 = (k + 1) % 3, i2 = (k + 2) % 3;
				key = svHash.GenerateKey(inds[i0]);
				for(index = svHash.First(key); index != -1; index = svHash.Next(index)) {
					if(svEntries[index] == inds[i0]) break;
				}
				if(index != -1 && inSphere[inds[i1]]) {
					key = svHash.GenerateKey(inds[i1]);
					for(index = svHash.First(key); index != -1; index = svHash.Next(index)) {
						if(svEntries[index] == inds[i1]) break;
					}
					if(index == -1) {
						index = pArgs->VertsTo.Add(inds[i1]);
						svHash.Add(key, index);
						svEntries[index] = inds[i1];
						fAdded = true;
					}
				}
				if(index != -1 && inSphere[inds[i2]]) {
					key = svHash.GenerateKey(inds[i2]);
					for(index = svHash.First(key); index != -1; index = svHash.Next(index)) {
						if(svEntries[index] == inds[i2]) break;
					}
					if(index == -1) {
						index = pArgs->VertsTo.Add(inds[i2]);
						svHash.Add(key, index);
						svEntries[index] = inds[i2];
						fAdded = true;
					}
				}
			}
		}
		if(!fAdded) break;
	}

	// RefTo:
	for(nTri = 0; nTri < TriCount; nTri++) {
		ii0 = 3 * nTri;
		for(k = 0; k < 3; k++) {
			inds[k] = Tris[ii0 + k];
		}
		for(k = 0; k < 3; k++) {
			key = svHash.GenerateKey(inds[k]);
			for(index = svHash.First(key); index != -1; index = svHash.Next(index)) {
				if(svEntries[index] == inds[k]) break;
			}
			if(index == -1) break;
		}
		if(k == 3) {
			pArgs->RefTo.m_TriNums.Add(Ref.m_TriNums[nTri]);
		}
	}

	for(nTri = 0; nTri < TriCount; nTri++) {
		if(RefToMap[nTri]) {
			pArgs->RefTo.m_TriNums.Add(Ref.m_TriNums[nTri]);
		}
	}

	// IsInval bitmask && reIndexing VertsTo:
	for(k = 0; k < pArgs->VertsTo.Count(); k++) {
		i = pArgs->VertsTo[k];
		IsInval.Set(i);
		pArgs->VertsTo[k] = VertNums[i];
	}
	// --------------------------------------
	pArgs->Ref = Ref;
	pArgs->QuadNums = QuadNums;
	pArgs->VertNums = VertNums;
	pArgs->Tris = Tris;
	pArgs->IsInval = IsInval;

	return true;
}