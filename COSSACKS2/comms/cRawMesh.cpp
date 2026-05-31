#include "comms.h"

//-----------------------------------------------------------------------------
// cRawMesh::IsValid
//-----------------------------------------------------------------------------
bool cRawMesh::IsValid(const bool ShowWarning /*= true*/) const {
	if(ShowWarning) {
		cLog::Message("Validating mesh \"%s\"...", GetName());
	}
	int i;
	// Positions:
	cBounds Inf(-cVec3::Infinity, cVec3::Infinity);
	for(i = 0; i < m_Positions.Count(); i++) {
		const cVec3 &u = m_Positions[i];
		// W/o NaN?
		if(!u.IsValid()) {
			if(ShowWarning) {
				cLog::Warning("Position at index %d = { %s } has NaN.", i, u.ToStr().ToCharPtr());
			}
			return false;
		}
		// Within (-Infinity, +Infinity)?
		if(!Inf.ContainsPoint(u)) {
			if(ShowWarning) {
				cLog::Warning("Position at index %d = { %s } is out of infinity.", i, u.ToStr().ToCharPtr());
			}
			return false;
		}
	}
	// TexCoords:
	for(i = 0; i < GetTexCoords().Count(); i++) {
		const cVec2 &t = GetTexCoords()[i];
		// W/o NaN?
		if(!t.IsValid()) {
			if(ShowWarning) {
				cLog::Warning("Texture coordinate at index %d = { %s } has NaN.", i, t.ToStr().ToCharPtr());
			}
			return false;
		}
		// Within (-Infinity, +Infinity)?
		if(!Inf.ContainsPoint(t)) {
			if(ShowWarning) {
				cLog::Warning("Texture coordinate at index %d = { %s } is out of infinity.", i, t.ToStr().ToCharPtr());
			}
			return false;
		}
	}
	//-------------------------------------------------------------------------
	// Normals && Tangents:
	//-------------------------------------------------------------------------
	// Count of normals equals count of tangents (if both are filled)?
	if(!GetNormals().IsEmpty() && !GetTangents().IsEmpty() && GetNormals().Count() != GetTangents().Count()) {
		if(ShowWarning) {
			cLog::Warning("Number of normals = %d is not equal number of tangents = %d.", GetNormals().Count(), GetTangents().Count());
		}
		return false;
	}
	// Normals:
	for(i = 0; i < GetNormals().Count(); i++) {
		const cVec3 &N = GetNormals()[i];
		// W/o NaN?
		if(!N.IsValid()) {
			if(ShowWarning) {
				cLog::Warning("Normal at index %d = { %s } has NaN.", i, N.ToStr().ToCharPtr());
			}
			return false;
		}
		// Within (-Infinity, +Infinity)?
		if(!Inf.ContainsPoint(N)) {
			if(ShowWarning) {
				cLog::Warning("Normal at index %d = { %s } is out of infinity.", i, N.ToStr().ToCharPtr());
			}
			return false;
		}
	}
	// Tangents:
	for(i = 0; i < GetTangents().Count(); i++) {
		const cVec<cVec3, 2> &Tangents = GetTangents()[i];
		// W/o NaN?
		if(!Tangents[0].IsValid()) {
			if(ShowWarning) {
				cLog::Warning("Tangent at index %d = { %s } has NaN.", i, Tangents[0].ToStr().ToCharPtr());
			}
			return false;
		}
		if(!Tangents[1].IsValid()) {
			if(ShowWarning) {
				cLog::Warning("BiTangent at index %d = { %s } has NaN.", i, Tangents[1].ToStr().ToCharPtr());
			}
			return false;
		}
		// Within (-Infinity, +Infinity)?
		if(!Inf.ContainsPoint(Tangents[0])) {
			if(ShowWarning) {
				cLog::Warning("Tangent at index %d = { %s } is out of infinity.", i, Tangents[0].ToStr().ToCharPtr());
			}
			return false;
		}
		if(!Inf.ContainsPoint(Tangents[1])) {
			if(ShowWarning) {
				cLog::Warning("BiTangent at index %d = { %s } is out of infinity.", i, Tangents[1].ToStr().ToCharPtr());
			}
			return false;
		}
	}
	//-------------------------------------------------------------------------
	// Raw:
	//-------------------------------------------------------------------------
	int nPoly = 0;
	int index = 0;
	while(index < GetRaw().Count()) {
		const cVec3i &Info = GetRaw()[index];
		const int Deg = Info[0];
		const int idMtl = Info[1];
		// Deg >= 3?
		if(Deg < 3) {
			if(ShowWarning) {
				cLog::Warning("Polygon %d = { %s } has degree less than 3.", nPoly, cStr::FromIntArray(Info.ToPtr(), Info.GetDimension()));
			}
			return false;
		}
		// idMtl is valid?
		if(idMtl != 0 && (idMtl < 0 || idMtl >= GetMaterials().Count())) {
			if(ShowWarning) {
				cLog::Warning("Polygon %d = { %s } has material index out of available %d.", nPoly, cStr::FromIntArray(Info.ToPtr(), 3).ToCharPtr(), GetMaterials().Count());
			}
			return false;
		}
		// Indices withing arrays:
		for(int k = 0; k < Deg; k++) {
			const int s = index + 1 + k;
			// Face has declared number of indices?
			if(s >= GetRaw().Count()) {
				if(ShowWarning) {
					cLog::Warning("Polygon %d has less than declared number of indices = %d.", nPoly, Info[0]);
				}
				return false;
			}
			const cVec3i &i0 = GetRaw()[s];
			// Is position index valid?
			if(i0[0] < 0 || i0[0] >= GetPositions().Count()) {
				if(ShowWarning) {
					cLog::Warning("Polygon %d has position index %d that is out of available %d.", nPoly, i0[0], GetPositions().Count());
				}
				return false;
			}
			// Is texcoord index valid?
			if(i0[1] < 0 || i0[1] >= GetTexCoords().Count()) {
				// May be zero, if texcoords is empty.
				if(!GetTexCoords().IsEmpty() || i0[1] != 0) {
					if(ShowWarning) {
						cLog::Warning("Polygon %d has texture coordinate index %d that is out of available %d.", nPoly, i0[1], GetTexCoords().Count());
					}
					return false;
				}
			}
			// Is normal index valid?
			const int NormalsCount = cMath::Max(GetNormals().Count(), GetTangents().Count());
			if(i0[2] < 0 || i0[2] >= NormalsCount) {
				// May be zero, if normals is empty.
				if(NormalsCount != 0 || i0[2] != 0) {
					if(ShowWarning) {
						cLog::Warning("Polygon %d has normal index %d that is out of available %d.", nPoly, i0[2], NormalsCount);
					}
					return false;
				}
			}
		}
		index += Deg + 1;
		nPoly++;
	}

	return true;
} // cRawMesh::IsValid

//-----------------------------------------------------------------------------
// cRawMesh::Clear
//-----------------------------------------------------------------------------
void cRawMesh::Clear() {
	m_Positions.Clear();
	m_TexCoords.Clear();
	m_Normals.Clear();
	m_Tangents.Clear();
	m_Raw.Clear();
	m_Name.Empty();
} // cRawMesh::Clear

//-----------------------------------------------------------------------------
// cRawMesh::GetPolyCount
//-----------------------------------------------------------------------------
int cRawMesh::GetPolyCount() const {
	int Count = 0;
	int index = 0;
	while(index < m_Raw.Count()) {
		index += m_Raw[index][0] + 1;
		Count++;
	}
	return Count;
} // cRawMesh::GetPolyCount

//-----------------------------------------------------------------------------
// cRawMesh::IsTriangulated
//-----------------------------------------------------------------------------
bool cRawMesh::IsTriangulated() const {
	int index = 0;
	while(index < m_Raw.Count()) {
		const int D = m_Raw[index][0];
		if(D != 3) {
			return false;
		}
		index += D + 1;
	}
	return true;
} // cRawMesh::IsTriangulated

//-----------------------------------------------------------------------------
// cRawMesh::Triangulate
//-----------------------------------------------------------------------------
void cRawMesh::Triangulate(cList<cVec3i> &TriRaw) const {
	TriRaw.Clear();
	int index = 0;
	while(index < m_Raw.Count()) {
		int nDeg = m_Raw[index][0];
		const int idMtl = m_Raw[index][1];
		if(nDeg < 3) {
			// Skip lines and points.
			index += nDeg + 1;
		} else {
			const cVec3i i0 = m_Raw[++index];
			nDeg -= 2;
			while(nDeg--) {
				index++;
				TriRaw.Add(cVec3i(3, idMtl, 0));
				TriRaw.Add(i0);
				TriRaw.Add(m_Raw[index]);
				TriRaw.Add(m_Raw[index + 1]);
			}
			index += 2;
		}
	}
} // cRawMesh::Triangulate

// cRawMesh::Triangulate

//-----------------------------------------------------------------------------
// cRawMesh::Triangulate
//-----------------------------------------------------------------------------
void cRawMesh::Triangulate() {
	if(IsTriangulated()) {
		return;
	}
	cList<cVec3i> TriRaw;
	Triangulate(TriRaw);
	m_Raw.Copy(TriRaw);
} // cRawMesh::Triangulate

//-----------------------------------------------------------------------------
// cRawMesh::InvertRaw
//-----------------------------------------------------------------------------
void cRawMesh::InvertRaw() {
	int index = 0;
	while(index < m_Raw.Count()) {
		const int Deg = m_Raw[index][0];
		int i = 0;
		while(i < Deg / 2) {
			cMath::Swap(m_Raw[index + 1 + i], m_Raw[index + Deg - i]);
			i++;
		}
		index += Deg + 1;
	}
} // cRawMesh::InvertRaw

//-----------------------------------------------------------------------------
// cRawMesh::CalcNormals
//-----------------------------------------------------------------------------
void cRawMesh::CalcNormals() {
	m_Normals.Clear();

	if(m_Positions.Count() < 1) { // No vertices.
		return;
	}

	m_Normals.SetCount(m_Positions.Count(), cVec3::Zero);

	// Pointing raw to newly created normals:
	int index = 0;
	while(index < m_Raw.Count()) {
		int nDeg = m_Raw[index][0];
		while(nDeg--) {
			index++;
			m_Raw[index][2] = m_Raw[index][0];
		}
		index++;
	}

	// Calcing normals:
	index = 0;
	while(index < m_Raw.Count()) {
		const int nDeg = m_Raw[index][0];
		if(nDeg >= 3) { // Skip lines and points.
			const cVec3i &i0 = m_Raw[index + 1];
			const cVec3i &i1 = m_Raw[index + 2];
			const cVec3i &i2 = m_Raw[index + 3];
			const cVec3 Normal = cVec3::Cross(m_Positions[i1[0]] - m_Positions[i0[0]], m_Positions[i2[0]] - m_Positions[i0[0]]);
			m_Normals[i0[2]] += Normal;
			m_Normals[i1[2]] += Normal;
			m_Normals[i2[2]] += Normal;
			index += nDeg + 1;
		}
		index += nDeg + 1;
	}

	// Normalizing normals:
	for(int i = 0; i < m_Normals.Count(); i++) {
		m_Normals[i].NormalizeSafe();
	}
} // cRawMesh::CalcNormals

//*****************************************************************************
// GenPlane
//*****************************************************************************

//-----------------------------------------------------------------------------
// cRawMesh::PlaneArgs::SetDefaults
//-----------------------------------------------------------------------------
void cRawMesh::PlaneArgs::SetDefaults() {
	Width = 1.0f;
	Height = 1.0f;
	wSubDivs = 10;
	hSubDivs = 10;
	Axis = cVec3::AxisY;
	TwoSided = false;
} // cRawMesh::PlaneArgs::SetDefaults

//-----------------------------------------------------------------------------
// cRawMesh::PlaneArgs::Validate
//-----------------------------------------------------------------------------
void cRawMesh::PlaneArgs::Validate() {
	const PlaneArgs Defs;
	if(Width < cMath::SpaceEpsilon) {
		Width = Defs.Width;
	}
	if(Height < cMath::SpaceEpsilon) {
		Height = Defs.Height;
	}
	if(wSubDivs < 1) {
		wSubDivs = Defs.wSubDivs;
	}
	if(hSubDivs < 1) {
		hSubDivs = Defs.hSubDivs;
	}
	Axis.Normalize();
	if(!Axis.IsNormalized()) {
		Axis = Defs.Axis;
	}
} // cRawMesh::PlaneArgs::Validate

//-----------------------------------------------------------------------------
// cRawMesh::GenPlane
//-----------------------------------------------------------------------------
cRawMesh * cRawMesh::GenPlane(PlaneArgs Args) {
	Args.Validate();
	
	const int NPositionsTexCoords = (Args.wSubDivs + 1) * (Args.hSubDivs + 1);
	const int NNormals = (Args.TwoSided ? 2 : 1) * NPositionsTexCoords;
	const int NPolys = Args.wSubDivs * Args.hSubDivs * (Args.TwoSided ? 2 : 1);

	cVec3 W, H;
	Args.Axis.ToPerps(W, H);
	W.FixDenormals();
	H.FixDenormals();
	
	cRawMesh *Mesh = new cRawMesh;
	Mesh->SetName("Plane");

	// Positions && TexCoords:
	const float iwSubDivs = 1.0f / (float)Args.wSubDivs;
	const float ihSubDivs = 1.0f / (float)Args.hSubDivs;
	
	float w0 = - 0.5f * Args.Width;
	float wCur = w0;
	float hCur = 0.5f * Args.Height;
	float dw = Args.Width / (float)Args.wSubDivs;
	float dh = Args.Height / (float)Args.hSubDivs;
	int iw, ih;
	for(ih = 0; ih <= Args.hSubDivs; ih++) {
		for(iw = 0; iw <= Args.wSubDivs; iw++) {
			Mesh->GetPositions().Add(wCur * W + hCur * H);
			Mesh->GetTexCoords().Add(cVec2((float)iw * iwSubDivs, (float)ih * ihSubDivs));

			wCur += dw;
		}
		hCur -= dh;
		wCur = w0;
	}

	cAssert(Mesh->GetPositions().Count() == NPositionsTexCoords);
	cAssert(Mesh->GetTexCoords().Count() == NPositionsTexCoords);

	// Normals && Tangents:
	for(int iNormal = 0; iNormal < NNormals; iNormal++) {
		if(iNormal < NPositionsTexCoords) {
			Mesh->GetNormals().Add(Args.Axis);
			Mesh->GetTangents().Add(cVec<cVec3, 2>(W, H));
		} else { // TwoSided
			Mesh->GetNormals().Add(-Args.Axis);
			Mesh->GetTangents().Add(cVec<cVec3, 2>(W, -H));
		}
	}

	cAssert(Mesh->GetNormals().Count() == NNormals);
	cAssert(Mesh->GetTangents().Count() == NNormals);
	
	// Raw:
	for(ih = 0; ih < Args.hSubDivs; ih++) {
		for(iw = 0; iw < Args.wSubDivs; iw++) {
			const int i0 = (Args.wSubDivs + 1) * ih + iw;
			const int i1 = (Args.wSubDivs + 1) * (ih + 1) + iw;

			Mesh->GetRaw().Add(cVec3i(4, 0, 0));
			Mesh->GetRaw().Add(cVec3i(i0));
			Mesh->GetRaw().Add(cVec3i(i1));
			Mesh->GetRaw().Add(cVec3i(i1 + 1));
			Mesh->GetRaw().Add(cVec3i(i0 + 1));

			if(Args.TwoSided) {
				Mesh->GetRaw().Add(cVec3i(4, 0, 0));
				Mesh->GetRaw().Add(cVec3i(i0, i0, NPositionsTexCoords + i0));
				Mesh->GetRaw().Add(cVec3i(i0 + 1, i0 + 1, NNormals / 2 + i0 + 1));
				Mesh->GetRaw().Add(cVec3i(i1 + 1, i1 + 1, NNormals / 2 + i1 + 1));
				Mesh->GetRaw().Add(cVec3i(i1, i1, NNormals / 2 + i1));
			}
		}
	}

	cAssert(Mesh->GetPolyCount() == NPolys);

	return Mesh;
} // cRawMesh::GenPlane

//----------------------------------------------------------------------------------------------------------
// cRawMesh::GenPlaneHexagonal
//----------------------------------------------------------------------------------------------------------
cRawMesh * cRawMesh::GenPlaneHexagonal(const float Lx, const float Ly, const float Cell, const bool Noisy) {
	cAssert(Cell > 0.0f);

	cRawMesh *Mesh = new cRawMesh;
	Mesh->SetName("HexagonalPlane");

	// Positions:
	const int xCells = int(Lx / Cell), yCells = int(Ly / Cell);
	int ix, iy;
	for(iy = 0; iy <= yCells; iy++) {
		for(ix = 0; ix <= xCells; ix++) {
			Mesh->GetPositions().Add(cVec3((float)ix * Cell, float(iy) * Cell, 0.0f));
		}
	}
	for(iy = 0; iy < yCells; iy++) {
		for(ix = 0; ix < xCells; ix++) {
			Mesh->GetPositions().Add(cVec3((float)ix * Cell + Cell / 2.0f, float(iy) * Cell + Cell / 2.0f, 0.0f));
		}
	}
	
	int Shift = (xCells + 1) * (yCells + 1);
	for(int yt = 0; yt < yCells; yt++)
		for(int xt = 0; xt < xCells; xt++) {
			// Left:
			int i0 = yt * xCells + xt + Shift;
			int i1 = yt * (xCells + 1) + xt;
			int i2 = i1 + (xCells + 1);

			Mesh->GetRaw().Add(cVec3i(3, 0, 0));
			Mesh->GetRaw().Add(cVec3i(i0, 0, 0));
			Mesh->GetRaw().Add(cVec3i(i2, 0, 0));
			Mesh->GetRaw().Add(cVec3i(i1, 0, 0));
			// Right:
			Mesh->GetRaw().Add(cVec3i(3, 0, 0));
			Mesh->GetRaw().Add(cVec3i(i0, 0, 0));
			Mesh->GetRaw().Add(cVec3i(i1 + 1, 0, 0));
			Mesh->GetRaw().Add(cVec3i(i2 + 1, 0, 0));
			if(0 == yt) { // Top cover:
				Mesh->GetRaw().Add(cVec3i(3, 0, 0));
				Mesh->GetRaw().Add(cVec3i(i0, 0, 0));
				Mesh->GetRaw().Add(cVec3i(i1, 0, 0));
				Mesh->GetRaw().Add(cVec3i(i1 + 1, 0, 0));
			} else {
				int i4 = i0 - xCells;
				// Left - top:
				Mesh->GetRaw().Add(cVec3i(3, 0, 0));
				Mesh->GetRaw().Add(cVec3i(i0, 0, 0));
				Mesh->GetRaw().Add(cVec3i(i1, 0, 0));
				Mesh->GetRaw().Add(cVec3i(i4, 0, 0));
				// Right - top:
				Mesh->GetRaw().Add(cVec3i(3, 0, 0));
				Mesh->GetRaw().Add(cVec3i(i0, 0, 0));
				Mesh->GetRaw().Add(cVec3i(i4, 0, 0));
				Mesh->GetRaw().Add(cVec3i(i1 + 1, 0, 0));
				if(yCells - 1 == yt) { // Bottom cover:
					Mesh->GetRaw().Add(cVec3i(3, 0, 0));
					Mesh->GetRaw().Add(cVec3i(i0, 0, 0));
					Mesh->GetRaw().Add(cVec3i(i2 + 1, 0, 0));
					Mesh->GetRaw().Add(cVec3i(i2, 0, 0));
				}
			}
	}

	if(Noisy) {
		const float h = 0.18f * Cell;
		for(int i = 0; i < Mesh->GetPositions().Count(); i++) {
			cVec3 &u = Mesh->GetPositions()[i];
			u.x += float(cMath::RandRange1() * h);//rand() % A - A / 2);
			u.y += float(cMath::RandRange1() * h);//rand() % A - A / 2);
			u.z += float(cMath::RandRange1() * h);//rand() % A - A / 2);
		}
	}

	return Mesh;
} // cRawMesh::GenPlaneHexagonal

//-----------------------------------------------------------------------------
// cRawMesh::FreeHashTable
//-----------------------------------------------------------------------------
void cRawMesh::FreeHashTable() const {
	for(int i = 0; i < m_HashTable.Count(); i++) {
		HashEntry *pEntry = m_HashTable[i];
		while(pEntry != NULL) {
			HashEntry *pNext = pEntry->pNext;
			delete pEntry;
			pEntry = pNext;
		}
	}
	m_HashTable.Free();
} // cRawMesh::FreeHashTable