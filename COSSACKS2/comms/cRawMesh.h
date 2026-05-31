#pragma once

//*****************************************************************************
// cRawMesh
//*****************************************************************************
class cRawMesh {
public:
	bool IsValid(const bool ShowWarning = false) const;
	void Clear();

	const cStr & GetName() const { return m_Name; }
	void SetName(const char *Name) { m_Name = Name; }

	const cList<cVec3> & GetPositions() const { return m_Positions; }
	cList<cVec3> & GetPositions() { return m_Positions; }

	const cList<cVec2> & GetTexCoords() const { return m_TexCoords; }
	cList<cVec2> & GetTexCoords() { return m_TexCoords; }
	
	const cList<cVec3> & GetNormals() const { return m_Normals; }
	cList<cVec3> & GetNormals() { return m_Normals; }

	const cList<cVec<cVec3, 2> > & GetTangents() const { return m_Tangents; }
	cList<cVec<cVec3, 2> > & GetTangents() { return m_Tangents; }

	const cList<cVec3i> & GetRaw() const { return m_Raw; }
	cList<cVec3i> & GetRaw() { return m_Raw; }

	const cList<cMaterial> & GetMaterials() const { return m_Materials; }
	cList<cMaterial> & GetMaterials() { return m_Materials; }

	int GetPolyCount() const;

	bool IsTriangulated() const;
	void Triangulate(cList<cVec3i> &TriRaw) const;
	void Triangulate();

	void CalcNormals();

	void InvertRaw();
	
	//-------------------------------------------------------------------------
	// GenPlane
	//-------------------------------------------------------------------------
	struct PlaneArgs {
		float Width;
		float Height;
		int wSubDivs;
		int hSubDivs;
		cVec3 Axis;
		bool TwoSided;
		
		PlaneArgs() { SetDefaults(); }
		void SetDefaults();
		void Validate();
	};

	static cRawMesh * GenPlane(PlaneArgs = PlaneArgs());

	static cRawMesh * GenPlaneHexagonal(const float Lx, const float Ly, const float Cell, const bool Noisy);
	
	template<typename Vertex> cDrawMesh<Vertex> * ToDrawMesh() const;

protected:
	cStr m_Name;
	cList<cVec3> m_Positions;
	cList<cVec2> m_TexCoords;
	cList<cVec3> m_Normals;
	cList<cVec<cVec3, 2> > m_Tangents; // Tangent and BiTangent.
	// Polygon info or 3 references to the arrays of positions, texture coordinates,
	// and normals with tangents (optional) respectively.
	cList<cVec3i> m_Raw;	// Info0 = { Count0, idMtl, 0 }
							// Indices_0 = { iPosition, iTexCoord, iNormal (iTangent) }
							// ...
							// Indices_(Count0 - 1)
							// Info1
							// ...
	cList<cMaterial> m_Materials;
private:
	// ToDrawMesh hashtable:
	struct HashEntry {
		int Index;
		HashEntry *pNext;
	};
	mutable cList<HashEntry *> m_HashTable;
	template<typename Vertex> int AddVertexToHashTable(cList<Vertex> &Vertices, const Vertex *pVertex, const int HashValue) const;
	void FreeHashTable() const;
};

//-----------------------------------------------------------------------------
// cRawMesh::ToDrawMesh
//-----------------------------------------------------------------------------
template<typename Vertex>
inline cDrawMesh<Vertex> * cRawMesh::ToDrawMesh() const {
	// Selecting source raw:
	const cList<cVec3i> *Raw = &m_Raw;
	cList<cVec3i> TriRaw;
	if(!IsTriangulated()) {
		Triangulate(TriRaw);
		Raw = &TriRaw;
	}

	cDrawMesh<Vertex> *Mesh = new cDrawMesh<Vertex>;
	Vertex u;
	ZeroMemory(&u, sizeof(Vertex)); // Clear "Undefined" or simply unused vertex fields.
	const cVertex::Format Format = Vertex::GetFormat();

	// Enuming tris and filling draw mesh:
	int index = 0;
	while(index < Raw->Count()) {
		const cVec3i &Tri = Raw->GetAt(index++);
		for(int k = 0; k < 3; k++) {
			const cVec3i &i0 = Raw->GetAt(index++);
			const int iPosition = i0[0];
			const int iTexCoord = i0[1];
			const int iNormal = i0[2];
			
			// Position || PositionRhw:
			if(Format.HasUsage(cVertex::Usage::Position)) {
				*u.GetPosition() = m_Positions[iPosition];
			} else if(Format.HasUsage(cVertex::Usage::PositionRhw)) {
				u.GetPositionRhw()->Set(m_Positions[iPosition], 1.0f);
			}
			
			// Normal:
			if(m_Normals.Count() > 0 && Format.HasUsage(cVertex::Usage::Normal)) {
				*u.GetNormal() = m_Normals[iNormal];
			}
			
			// TexCoord:
			if(m_TexCoords.Count() > 0 && Format.HasUsage(cVertex::Usage::TexCoord)) {
				*u.GetTexCoord() = m_TexCoords[iTexCoord];
			}
			
			// Tangent:
			if(m_Tangents.Count() > 0 && Format.HasUsage(cVertex::Usage::Tangent)) {
				*u.GetTangent() = m_Tangents[iNormal][0];
			}

			// BiTangent:
			if(m_Tangents.Count() > 0 && Format.HasUsage(cVertex::Usage::BiTangent)) {
				*u.GetBiTangent() = m_Tangents[iNormal][1];
			}

			AddVertexToHashTable(Mesh->GetVertices(), &u, iPosition);
		}
	}

	FreeHashTable();
	
	return Mesh;
} // cRawMesh::ToDrawMesh

//--------------------------------------------------------------------------------------------------------------------
// cRawMesh::AddVertexToHashTable
//--------------------------------------------------------------------------------------------------------------------
template<typename Vertex>
inline int cRawMesh::AddVertexToHashTable(cList<Vertex> &Vertices, const Vertex *pVertex, const int HashValue) const {
	bool IsFound = false;
	int Index = 0;

	if(m_HashTable.Count() > HashValue) {
		HashEntry *pEntry = m_HashTable[HashValue];
		while(pEntry != NULL) {
			const Vertex *pCacheVertex = &Vertices[pEntry->Index];
			// If this vertex equals the vertex already in the vertex buffer,
			// simply point the index buffer to the existing vertex.
			if(memcmp(pVertex, pCacheVertex, sizeof(Vertex)) == 0) {
				IsFound = true;
				Index = pEntry->Index;
				break;
			}
			pEntry = pEntry->pNext;
		}
	}

	// Vertex was not found. Create a new entry, both within the Vertices list
	// and also within hashtable cache.
	if(!IsFound) {
		// Add to the Vertices list.
		Index = Vertices.Add(*pVertex);

		// Add to the hashtable:
		HashEntry *pNewEntry = new HashEntry;
		pNewEntry->Index = Index;
		pNewEntry->pNext = NULL;

		// Grow the hashtable cash if needed:
		if(m_HashTable.Count() <= HashValue) {
			m_HashTable.Add(NULL, HashValue - m_HashTable.Count() + 1);
		}

		// Add to the end of the linked list:
		HashEntry *pCurEntry = m_HashTable[HashValue];
		if(NULL == pCurEntry) {
			// This is the head element:
			m_HashTable[HashValue] = pNewEntry;
		} else {
			// Find the tail:
			while(pCurEntry->pNext != NULL) {
				pCurEntry = pCurEntry->pNext;
			}
			pCurEntry->pNext = pNewEntry;
		}
	}
	return Index;
} // cRawMesh::AddVertexToHash