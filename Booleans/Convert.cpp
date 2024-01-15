//-----------------------------------------------------------------------------
// Convert : bool(const cRawMesh &, MObject *, MFnMesh *)
//-----------------------------------------------------------------------------
bool Convert(const cRawMesh &Src, MObject &Object, MFnMesh &Mesh) {
	int nVerts = Src.m_Verts.Count(), nTris = Src.m_Tris.Count() / 3;
	// Verts:
	MPointArray vertexArray;
	for(int i = 0; i < nVerts; i++) {
		const cVector &u = Src.m_Verts[i];
		vertexArray.append(u.x, u.y, u.z);
	}
	assert(vertexArray.length() == nVerts);
	// Tris:
	MIntArray polygonCounts, polygonConnects;
	int i0, i1, i2;
	for(int nTri = 0; nTri < nTris; nTri++) {
		Src.GetTri(nTri, i0, i1, i2);
		polygonCounts.append(3);
		polygonConnects.append(i0);
		polygonConnects.append(i1);
		polygonConnects.append(i2);
	}
	assert(polygonCounts.length() == nTris);
	assert(polygonConnects.length() == 3 * nTris);
	// Create:
	MStatus ReturnStatus;
	Object = Mesh.create(nVerts, nTris, vertexArray, polygonCounts, polygonConnects,
		MObject::kNullObj, &ReturnStatus);
	return MS::kSuccess == ReturnStatus;
}

//-----------------------------------------------------------------------------
// Convert : bool(const MFnMesh &, cRawMesh *)
//-----------------------------------------------------------------------------
bool Convert(const MFnMesh &Src, cRawMesh *pDest) {
	pDest->Free();

	int nVerts = Src.numVertices(), nPolys = Src.numPolygons(),
		nEdges = Src.numEdges(), nFaceVerts = Src.numFaceVertices();

	// Verts:
	MPointArray vertexArray;
	Src.getPoints(vertexArray, MSpace::kWorld);
	int i0, i1, i2;
	for(int i = 0; i < (int)vertexArray.length(); i++) {
		const MPoint &p = vertexArray[i];
		pDest->m_Verts.Add(cVector(p.x, p.y, p.z));
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
		assert(i0 < pDest->m_Verts.Count());
		assert(i1 >= 0);
		assert(i1 < pDest->m_Verts.Count());
		assert(i2 >= 0);
		assert(i2 < pDest->m_Verts.Count());
		pDest->m_Tris.Add(i0);
		pDest->m_Tris.Add(i1);
		pDest->m_Tris.Add(i2);
	}
	pDest->m_Type = comms::RMTTTris;
	return true;
}