/*****************************************************************************/
/*	File:	sgGeometry.inl
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	22.04.2003
/*****************************************************************************/


/*****************************************************************************/
/*	Geometry	implementation
/*****************************************************************************/
_inl int Geometry::AddVertex( void* pVert )
{
	int nV = m_Mesh.getNVert();
	assert( nV < m_Mesh.getMaxVert() );
	int stride = Vertex::GetStride( m_Mesh.getVertexFormat() );
	memcpy( m_Mesh.getVertexData() + stride * nV, pVert, stride );
	m_Mesh.setNVert( nV + 1 );
	return nV + 1;
} // Geometry::AddVertex

_inl int Geometry::AddPoly( WORD v1, WORD v2, WORD v3 )
{
	int nI = m_Mesh.getNInd();
	assert( nI + 3 <= m_Mesh.getMaxInd() );
	WORD* pIdx = m_Mesh.getIndices() + nI;
	pIdx[0] = v1; pIdx[1] = v2; pIdx[2] = v3;
	m_Mesh.setNInd( nI + 3 );
	m_Mesh.setNPri( m_Mesh.getNPri() + 1 );
	return nI + 3;
} // Geometry::AddPoly

_inl bool Geometry::AddQuad( const Vector3D& a, const Vector3D& b,
							 const Vector3D& c, const Vector3D& d,
							 const Rct* pUV )
{
	int nV = m_Mesh.getNVert();
	int nI = m_Mesh.getNInd();
	WORD* pIdx = m_Mesh.getIndices() + nI;
	
	if (pIdx && nI + 6 >= m_Mesh.getMaxInd())	return false;
	if (nV + 4 > m_Mesh.getMaxVert())			return false;

	int stride = Vertex::GetStride( m_Mesh.getVertexFormat() );
	BYTE* pVert = m_Mesh.getVertexData() + stride * nV;

	*((Vector3D*)pVert) = a; pVert += stride;
	*((Vector3D*)pVert) = b; pVert += stride;
	*((Vector3D*)pVert) = c; pVert += stride;
	*((Vector3D*)pVert) = d; 

	if (pIdx)
	{
		pIdx[0] = nV;		pIdx[1] = nV + 1; pIdx[2] = nV + 2;
		pIdx[3] = nV + 2;	pIdx[4] = nV + 1; pIdx[5] = nV + 3;
	}

	if (pUV)
	{
		VertexIterator it;
		it << GetMesh();
        it.uv( nV,     0 ).u = pUV->x;			   it.uv( nV,	  0 ).v = pUV->y;
        it.uv( nV + 1, 0 ).u = pUV->GetRight();    it.uv( nV + 1, 0 ).v = pUV->y;
        it.uv( nV + 2, 0 ).u = pUV->x;			   it.uv( nV + 2, 0 ).v = pUV->GetBottom();
        it.uv( nV + 3, 0 ).u = pUV->GetRight();    it.uv( nV + 3, 0 ).v = pUV->GetBottom();
	}

	m_Mesh.setNVert( nV + 4 );
	m_Mesh.setNPri ( m_Mesh.getNPri() + 2 );
	return true;
} // Geometry::AddQuad

/*****************************************************************************/
/*	GeometryRef	implementation
/*****************************************************************************/
_inl GeometryRef::GeometryRef() : pool(NULL)
{
}


