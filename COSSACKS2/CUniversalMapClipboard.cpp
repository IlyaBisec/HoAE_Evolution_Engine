#include "CUniversalMapClipboard.h"

extern UniversalMap g_UniversalMap;
CUniversalMapClipboard g_UniversalMapClipboard(&g_UniversalMap);

const int CUniversalMapClipboard::c_MinOffset = 300;
const int CUniversalMapClipboard::c_MinRadius = 250;

// ================================================================================
// CUniversalMapClipboard::Clear
// ================================================================================
void CUniversalMapClipboard::Clear(void)
{
	m_Mesh.Release();
	m_Cache.Release();
	m_Contour.Clear();
	m_SplittedClipboard.Release();
	m_Radius = m_ShiftX = m_ShiftY = m_ShiftZ = 0;
	m_bInvalid = true;
	m_bRedden = false;
	m_fAlpha = 0.0f;
} // CUniversalMapClipboard::Clear

// ================================================================================
// CUniversalMapClipboard::Release
// ================================================================================
void CUniversalMapClipboard::Release(void)
{
	Clear();
	m_pUniversalMap = NULL;
} // CUniversalMapClipboard::Release

// ================================================================================
// CUniversalMapClipboard::Cache
// ================================================================================
void CUniversalMapClipboard::Cache(void)
{
	m_Cache.Release();
	m_Cache.Vert.AddValues(m_Mesh.Vert.GetValues(), m_Mesh.Vert.NValues);
	m_Cache.Idxs.AddValues(m_Mesh.Idxs.GetValues(), m_Mesh.Idxs.NValues);
	float sinAlpha = sin(m_fAlpha), cosAlpha = cos(m_fAlpha);
	for(int i = 0; i < m_Cache.Vert.NValues; i++)
	{
		OneMapVertex &v = m_Cache.Vert[i];
		float x = v.x, y = v.y;
		float X = x * cosAlpha + y * sinAlpha, Y = - x * sinAlpha + y * cosAlpha;
		v.x = short(X), v.y = short(Y);
	}
	m_Cache.CalcNormals();
	extern CSun g_Sun;
	m_Cache.Shade(false, m_pUniversalMap->m_pSun);
} // CUniversalMapClipboard::Cache

// ================================================================================
// CUniversalMapClipboard::Rotate
// ================================================================================
void CUniversalMapClipboard::Rotate(int dir)
{
	if(dir == 0) return;
	const float dAngle = 0.2;
	m_bInvalid = true; // Next Draw() call time we should cache and split.
	if(dir > 0) m_fAlpha += dAngle;
	else m_fAlpha -= dAngle;
} // CUniversalMapClipboard::Rotate

// ================================================================================
// CUniversalMapClipboard::Offset
// ================================================================================
void CUniversalMapClipboard::Offset(int ox, int oy, int oz)
{
	for(int nBaseMesh = 0; nBaseMesh < m_SplittedClipboard.pBaseMeshes.NValues; nBaseMesh++)
	{
		BaseMesh *pBaseMesh = m_SplittedClipboard.pBaseMeshes[nBaseMesh];
		if(!pBaseMesh) continue;
		VERTEX *pVertices = (VERTEX*)pBaseMesh->getVertexData();
		if(!pVertices) continue;
		int NVertices = pBaseMesh->getNVert();
		for(int n = 0; n < NVertices; n++)
		{
			pVertices->x += ox;
			pVertices->y += oy;
			pVertices->z += oz;
			pVertices->diffuse &= 0xA0FFFFFF;
			pVertices++;
		}
	}
} // CUniversalMapClipboard::Offset

// ================================================================================
// CUniversalMapClipboard::Redden
// ================================================================================
void CUniversalMapClipboard::Redden(void)
{
	for(int nBaseMesh = 0; nBaseMesh < m_SplittedClipboard.pBaseMeshes.NValues; nBaseMesh++)
	{
		BaseMesh *pBaseMesh = m_SplittedClipboard.pBaseMeshes[nBaseMesh];
		if(!pBaseMesh) continue;
		VERTEX *pVertices = (VERTEX*)pBaseMesh->getVertexData();
		if(!pVertices) continue;
		int NVertices = pBaseMesh->getNVert();
		for(int n = 0; n < NVertices; n++)
		{
			pVertices->diffuse &= 0xFF00FFFF;
			pVertices->diffuse += 0x00FF0000;
			pVertices++;
		}
	}
} // CUniversalMapClipboard::Redden

// ================================================================================
// CUniversalMapClipboard::Draw
// ================================================================================
void CUniversalMapClipboard::Draw(int CoordX, int CoordY, int CoordZ, bool bRedden)
{
	if(bRedden != m_bRedden)
	{
		m_bRedden = bRedden;
		m_bInvalid = true;
	}
	if(m_bInvalid)
	{
		m_ShiftX = m_ShiftY = m_ShiftZ = 0;
		if(!m_Mesh.Vert.NValues || !m_Mesh.Idxs.NValues || !m_Contour.NValues) return;
		Cache();
		VertexBasedMeshRef Ref;
		Ref.pOrigin = &m_Cache;
		int NTri = m_Cache.Idxs.NValues / 3;
		for(int nTri = 0; nTri < NTri; nTri++) Ref.nTriangles.Add(nTri);
		m_SplittedClipboard.AdvSplitMesh(&Ref);
		//SplittedClipboard.SpeedySplit(Ref);
		Ref.Release();
		if(m_bRedden) Redden();
		m_bInvalid = false;
	}
	int Sx = CoordX - m_ShiftX, Sy = CoordY - m_ShiftY, Sz = CoordZ - m_ShiftZ;
	m_ShiftX = CoordX, m_ShiftY = CoordY, m_ShiftZ = CoordZ;
	if(Sx || Sy || Sz) Offset(Sx, Sy, Sz);
	m_SplittedClipboard.Draw();
} // CUniversalMapClipboard::Draw

// ================================================================================
// CUniversalMapClipboard::Update
// ================================================================================
void CUniversalMapClipboard::Update(int *oi, int *i, OneMapVertex *v, COneMapVertices &rVertices,
									nHashTable &rTable, int *flags)
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
} // CUniversalMapClipboard::Update

// ================================================================================
// CUniversalMapClipboard::InRound
// ================================================================================
bool CUniversalMapClipboard::InRound(int cx, int cy, int r, const OneMapVertex &rVertex)
{
	int r2 = r * r, dx = int(rVertex.x) - cx, dy = int(rVertex.y) - cy;
	return dx * dx + dy * dy < r2;
} // CUniversalMapClipboard::InRound

// ================================================================================
// CUniversalMapClipboard::Loop
// ================================================================================
bool CUniversalMapClipboard::Loop(CIndices &rPairs, CIndices &rContour)
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
} // CUniversalMapClipboard::Loop

// ================================================================================
// CUniversalMapClipboard::Copy
// ================================================================================
void CUniversalMapClipboard::Copy(int cx, int cy, int R)
{
	Clear();
	m_Radius = R;
	// Offset check:
	int x0 = cx - R, y0 = cy - R, x1 = cx + R, y1 = cy + R;
	if(x0 < c_MinOffset || y0 < c_MinOffset || m_pUniversalMap->UMLx - x1 < c_MinOffset ||
		m_pUniversalMap->UMLy - y1 < c_MinOffset || R < c_MinRadius) return;
	// Essentially copying to clipboard:
	nHashTable hTable;
	OneMapVertex v[3];
	int oi[3], i[3];
	bool r[3];
	CIndices ContourPairs;
	for(int nQ = 0; nQ < m_pUniversalMap->m_pQuads.NValues; nQ++)
		for(int n = 0; n < m_pUniversalMap->m_pQuads[nQ]->nTriangles.NValues; n++)
		{
			int nTri = m_pUniversalMap->m_pQuads[nQ]->nTriangles[n], flags[] = {0, 1, 2};
			for(int k = 0; k < 3; k++)
			{
				oi[k] = m_pUniversalMap->Idxs[3 * nTri + k];
				v[k] = m_pUniversalMap->Vert[oi[k]];
				v[k].x -= cx, v[k].y -= cy;
				r[k] = InRound(0, 0, R, v[k]);				
			}
			if(r[0] && r[1] && r[2]) // The whole triangle lies within round:
			{
				Update(oi, i, v, m_Mesh.Vert, hTable, flags);
				for(k = 0; k < 3; k++) m_Mesh.Idxs.Add(i[k]);
			}
			if(!r[0] && r[1] && r[2])
			{
				flags[0] = -1;
				Update(oi, i, v, m_Mesh.Vert, hTable, flags);
				ContourPairs.Add(i[1]), ContourPairs.Add(i[2]);
			}
			if(r[0] && !r[1] && r[2])
			{
				flags[1] = -1;
				Update(oi, i, v, m_Mesh.Vert, hTable, flags);
				ContourPairs.Add(i[0]), ContourPairs.Add(i[2]);
			}
			if(r[0] && r[1] && !r[2])
			{
				flags[2] = -1;
				Update(oi, i, v, m_Mesh.Vert, hTable, flags);
				ContourPairs.Add(i[0]), ContourPairs.Add(i[1]);
			}
		}
	// Looping ContourPairs to rClipboard.Contour:
	if(!Loop(ContourPairs, m_Contour))
	{
		Log.Error("CUniversalMapClipboard::Copy() FAILED! Geometry is too complex! Try another position!");
		Clear();
	}
	// unInit:
	hTable.reset();
	ContourPairs.Clear();
} // CUniversalMapClipboard::Copy

// ================================================================================
// CUniversalMapClipboard::AlignWithStencil
// ================================================================================
void CUniversalMapClipboard::AlignWithStencil(VertexBasedMesh &rMesh, VertexBasedMesh &rStencil,
											  int cx, int cy, int R)
{
	const short lInvariableOffset = 100, lSmoothOffset = lInvariableOffset + 100;
	int RInvariable = R - lInvariableOffset, RConst = R - lSmoothOffset;

	// Deleting unnecessary triangles in rStencil:
	DynArray<bool> inInvariableStencil;
	inInvariableStencil.Add(false, rStencil.Vert.NValues);
	for(int i = 0; i < rStencil.Vert.NValues; i++)
	{
		OneMapVertex &v = rStencil.Vert[i];
		inInvariableStencil[i] = InRound(cx, cy, RInvariable, v);
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
		if(InRound(cx, cy, RConst, v)) inConst[i] = true, inInvariable[i] = true;
		else if(InRound(cx, cy, RInvariable, v)) inInvariable[i] = true;
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

// ================================================================================
// CUniversalMapClipboard::ExtractlVertices
// ================================================================================
void CUniversalMapClipboard::ExtractlVertices(const CIndices &rContour, ClVertices &rlVertices,
											  bool fromStuffing, float cx, float cy, float l)
{
	ClVertex lVertex;
	float ux, uy, s, px, py;
	for(int i = 0; i < rContour.NValues; i++)
	{
		OneMapVertex &v = m_pUniversalMap->Vert[rContour[i]];
		ux = float(v.x) - cx, uy = float(v.y) - cy, s = l / sqrt(ux * ux + uy * uy);
		px = cx + s * ux, py = cy + s * uy;
		lVertex.v = v;
		lVertex.v.x = short(px), lVertex.v.y = short(py);
		lVertex.lindex = rContour[i];
		lVertex.fromStuffing = fromStuffing;
		rlVertices.Add(lVertex);
	}
} // CUniversalMapClipboard::ExtractlVertices

// ================================================================================
// CUniversalMapClipboard::Paste
// ================================================================================
void CUniversalMapClipboard::Paste(int CoordX, int CoordY, int CoordZ)
{
	// Offset check:
	int x0 = CoordX - m_Radius, y0 = CoordY - m_Radius,
		x1 = CoordX + m_Radius, y1 = CoordY + m_Radius;
	if(!m_Mesh.Vert.NValues || !m_Mesh.Idxs.NValues || !m_Contour.NValues ||
		!m_pUniversalMap->Vert.NValues || !m_pUniversalMap->Idxs.NValues || !m_pUniversalMap->m_pQuads.NValues ||
		x0 - m_pUniversalMap->QuadX0 < c_MinOffset || y0 - m_pUniversalMap->QuadY0 < c_MinOffset ||
		m_pUniversalMap->QuadX0 + m_pUniversalMap->UMLx - x1 < c_MinOffset ||
		m_pUniversalMap->QuadY0 + m_pUniversalMap->UMLy - y1 < c_MinOffset) return;
	
	const int dFilletDisk = 30, dMargin = 60;
	int HoleRadius = m_Radius + dFilletDisk;

	// Extracting numbers of triangles to delete, deleting their reference in quads,
	// adding these numbers to new reference (VertexBasedMeshRef Ref) to create stencil (VertexBasedMesh Stencil),
	// forming contour pairs (CIndices ContourPairs).
	CIndices ContourPairs;
	OneMapVertex v[3];
	int oi[3];
	bool r[3];
	VertexBasedMeshRef Ref;
	for(int nQ = 0; nQ < m_pUniversalMap->m_pQuads.NValues; nQ++)
		for(int iTri = 0; iTri < m_pUniversalMap->m_pQuads[nQ]->nTriangles.NValues;)
		{
			int nTri = m_pUniversalMap->m_pQuads[nQ]->nTriangles[iTri];
			for(int k = 0; k < 3; k++)
			{
				oi[k] = m_pUniversalMap->Idxs[3 * nTri + k];
				v[k] = m_pUniversalMap->Vert[oi[k]];
				r[k] = InRound(CoordX, CoordY, HoleRadius, v[k]);
			}
			if(r[0] && !r[1] && !r[2]) ContourPairs.Add(oi[1]), ContourPairs.Add(oi[2]);
			if(!r[0] && r[1] && !r[2]) ContourPairs.Add(oi[0]), ContourPairs.Add(oi[2]);
			if(!r[0] && !r[1] && r[2]) ContourPairs.Add(oi[0]), ContourPairs.Add(oi[1]);

			if(r[0] || r[1] || r[2])
			{
				Ref.nTriangles.Add(nTri);
				m_pUniversalMap->m_pQuads[nQ]->nTriangles.Del(iTri, 1);
				continue;
			}
			iTri++;
		}

	VertexBasedMesh Stencil, Stuffing;
	ClVertices lVertices, toTheLeft, toTheRight;
	VertexBasedMeshRef refFilletDisk;
	CIndices refFilletDiskIndices;

	// Creating / caching Contour and StuffingContour:
	CIndices Contour, StuffingContour;
	if(!Loop(ContourPairs, Contour))
	{
		Log.Error("CUniversalMapClipboard::Paste() FAILED! Geometry is too complex! Try another position!");
		goto CUniversalMapClipboard_unInit;
	}

	// Creating Stencil and Stuffing, compiling UniversalMap with Contour reindexing:
	Ref.pOrigin = m_pUniversalMap;
	Ref.toVertexBasedMesh(Stencil);

	m_pUniversalMap->Compile(&Contour);

	int NVert = m_pUniversalMap->Vert.NValues,
		NTris = m_pUniversalMap->Idxs.NValues / 3;
	// caching vertices:
//	m_ShiftX = CoordX, m_ShiftY = CoordY, m_ShiftZ = CoordZ, m_fAlpha = fAlpha;
	Cache();
	Stuffing.Vert.AddValues(m_Cache.Vert.GetValues(), m_Cache.Vert.NValues);
	Stuffing.Idxs.AddValues(m_Cache.Idxs.GetValues(), m_Cache.Idxs.NValues);
	for(int i = 0; i < Stuffing.Vert.NValues; i++)
	{
		OneMapVertex &v = Stuffing.Vert[i];
		v.x += CoordX, v.y += CoordY, v.z += CoordZ;
	}
	StuffingContour.AddValues(m_Contour.GetValues(), m_Contour.NValues);

	AlignWithStencil(Stuffing, Stencil, CoordX, CoordY, m_Radius);
	m_pUniversalMap->Sum(Stuffing, &StuffingContour);

	// LINKING:
	int l = m_Radius + dFilletDisk / 2;
	ExtractlVertices(StuffingContour, lVertices, true, float(CoordX), float(CoordY), float(l));
	ExtractlVertices(Contour, lVertices, false, float(CoordX), float(CoordY), float(l));
	short Z;
	for(i = 0; i < lVertices.NValues; i++)
		if(Stencil.CalcZ(lVertices[i].v, Z)) lVertices[i].v.z = Z;
		else MessageBox(NULL, "CUniversalMapClipboard::Paste:LINKING Stencil.CalcZ FAILED!", "--- ERROR ---", 0);
	for(i = 0; i < lVertices.NValues; i++)
	{
		ClVertex &lVertex = lVertices[i];
		float x = float(lVertex.v.x) - CoordX, y = float(lVertex.v.y) - CoordY;
		if(x < 0) lVertex.toTheLeft = true;
		else lVertex.toTheLeft = false;
		lVertex.fAngle = atan(y / x);
	}
	// Splitting by position (left / right) criterion:
	for(i = 0; i < lVertices.NValues; i++)
	{
		ClVertex &lVertex = lVertices[i];
		if(lVertex.toTheLeft) toTheLeft.Add(lVertex);
		else toTheRight.Add(lVertex);
	}
	lVertices.Clear();
	// Sorting toTheLeft && toTheRight:
	for(i = 0; i < toTheLeft.NValues - 1; i++)
		for(int j = 1; j < toTheLeft.NValues - i; j++)
			if(toTheLeft[j - 1].fAngle < toTheLeft[j].fAngle)
			{
				ClVertex lVertex = toTheLeft[j - 1];
				toTheLeft[j - 1] = toTheLeft[j];
				toTheLeft[j] = lVertex;
			}
	for(i = 0; i < toTheRight.NValues - 1; i++)
		for(int j = 1; j < toTheRight.NValues - i; j++)
			if(toTheRight[j - 1].fAngle < toTheRight[j].fAngle)
			{
				ClVertex lVertex = toTheRight[j - 1];
				toTheRight[j - 1] = toTheRight[j];
				toTheRight[j] = lVertex;
			}
	// Merging:
	lVertices.AddValues(toTheLeft.GetValues(), toTheLeft.NValues);
	lVertices.AddValues(toTheRight.GetValues(), toTheRight.NValues);
	for(i = 0; i < lVertices.NValues; i++)
	{
		ClVertex &lVertex = lVertices[i];
		lVertex.index = m_pUniversalMap->Vert.NValues;
		m_pUniversalMap->Vert.Add(lVertex.v);
	}

	// Crucial moment (essentially linking):
	ClVertex &lVertex = lVertices[0];
	int StuffingIndex = -1, UniversalMapIndex = -1, _index = lVertices[0].index, nlVertex = 1;
	if(lVertex.fromStuffing) StuffingIndex = lVertices[0].lindex;
	else UniversalMapIndex = lVertices[0].lindex;
	bool _fromStuffing = lVertices[0].fromStuffing, bSupplyFromStuffing = !lVertices[0].fromStuffing;

	bool bReLinking = false;

//	int _debugNTri = m_pUniversalMap->Idxs.NValues / 3; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

CUniversalMapClipboard_Linking:
	
	bool bCanAddToStuffing = (!bReLinking || bSupplyFromStuffing),
		bCanAddToUniversalMap = (!bReLinking || !bSupplyFromStuffing);

	for(i = 1; i <= lVertices.NValues; i++)
	{
		ClVertex &lVertex = lVertices[i % lVertices.NValues];
		int index = lVertex.index, lindex = lVertex.lindex;
		bool fromStuffing = lVertex.fromStuffing;

		if(fromStuffing && _fromStuffing)
		{
			if(bCanAddToStuffing)
			{
				m_pUniversalMap->Sum(_index, index, lindex);
				m_pUniversalMap->Sum(_index, lindex, StuffingIndex);
			}

			if(bCanAddToUniversalMap)
				m_pUniversalMap->Sum(index, _index, UniversalMapIndex);

			StuffingIndex = lindex;
		}
		if(!fromStuffing && !_fromStuffing)
		{
			if(bCanAddToUniversalMap)
			{
				m_pUniversalMap->Sum(index, _index, UniversalMapIndex);
				m_pUniversalMap->Sum(index, UniversalMapIndex, lindex);
			}

			if(bCanAddToStuffing) m_pUniversalMap->Sum(_index, index, StuffingIndex);

			UniversalMapIndex = lindex;
		}
		if(fromStuffing && !_fromStuffing)
		{
			if(bCanAddToUniversalMap) m_pUniversalMap->Sum(index, _index, UniversalMapIndex);
			if(bCanAddToStuffing)
			{
				m_pUniversalMap->Sum(_index, index, StuffingIndex);
				m_pUniversalMap->Sum(index, lindex, StuffingIndex);
			}
			StuffingIndex = lindex;
		}
		if(!fromStuffing && _fromStuffing)
		{
			if(bCanAddToStuffing) m_pUniversalMap->Sum(_index, index, StuffingIndex);
			if(bCanAddToUniversalMap)
			{
				m_pUniversalMap->Sum(index, _index, UniversalMapIndex);
				m_pUniversalMap->Sum(index, UniversalMapIndex, lindex);
			}
			UniversalMapIndex = lindex;
		}
		_index = index;
		_fromStuffing = fromStuffing;
		if(StuffingIndex == -1 || UniversalMapIndex == -1) nlVertex++;
		if(bReLinking && i == nlVertex) break;
	}
	if(!bReLinking)
	{
		bReLinking = true;
		goto CUniversalMapClipboard_Linking;
	}

//	int debugNTri = m_pUniversalMap->Idxs.NValues / 3 - _debugNTri; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//	char Buffer[4096]; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//	sprintf(Buffer, "%d triangles were added as a result of linking.", debugNTri); // <<<<<<<<<<<<<<<<<<<<
//	MessageBox(NULL, Buffer, "--- Info ---", 0); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	// We need to smooth fillet disk:
	m_pUniversalMap->GetMeshRefCylinder(CoordX, CoordY, m_Radius + dFilletDisk + dMargin, &refFilletDisk);
	refFilletDisk.SupplementNormals(CoordX, CoordY, m_Radius + dFilletDisk + dMargin);
	refFilletDisk.SupplementShadow(m_pUniversalMap->m_pSun); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	OneMapVertex v0, v1, v2;
	for(int n = 0; n < refFilletDisk.nTriangles.NValues; n++)
	{
		int nTri = refFilletDisk.nTriangles[n];
		for(int k = 0; k < 3; k++)
		{
			oi[k] = m_pUniversalMap->Idxs[3 * nTri + k];
			v[k] = m_pUniversalMap->Vert[oi[k]];
			r[k] = InRound(CoordX, CoordY, m_Radius - 2 * dMargin, v[k]);
		}
		if(!r[0] || !r[1] || !r[2]) refFilletDiskIndices.Add(nTri);
	}
	refFilletDisk.nTriangles.Clear();
	refFilletDisk.nTriangles.AddValues(refFilletDiskIndices.GetValues(), refFilletDiskIndices.NValues);
	for(i = 0; i < 30; i++) refFilletDisk.Smooth(0.5, 2);
	// Rect:
	int RR = m_Radius + dFilletDisk + dMargin;
	m_pUniversalMap->SupplementMapOfHeightsInRect(CoordX - RR, CoordY - RR, CoordX + RR, CoordY + RR);

	// unInit:
CUniversalMapClipboard_unInit:
	refFilletDiskIndices.Clear();
	refFilletDisk.Release();
	toTheLeft.Clear(), toTheRight.Clear();
	lVertices.Clear();
	Stencil.Release(), Stuffing.Release();
	Contour.Clear(), StuffingContour.Clear();
	Ref.Release();
	ContourPairs.Clear();
} // CUniversalMapClipboard::Paste