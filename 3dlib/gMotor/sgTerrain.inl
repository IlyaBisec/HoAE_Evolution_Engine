/*****************************************************************************/
/*	File:	sgTerrain.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	11-12-2003
/*****************************************************************************/
BEGIN_NAMESPACE(sg)
/*****************************************************************************/
/*	Terrain implementation
/*****************************************************************************/
_inl Vector3D Terrain::GetNormal( float x, float y ) const
{
	float ndenom = 0.5f / c_GridStep;

	float l = GetHeight( x - c_GridStep, y );		
	float r = GetHeight( x + c_GridStep, y );		
	float u = GetHeight( x, y - c_GridStep );		
	float d = GetHeight( x, y + c_GridStep );		

	Vector3D normal;
	normal.x = (l - r) * ndenom;
	normal.y = (u - d) * ndenom;
	normal.z = 2.0f;
	normal.normalize();
	return normal;
} // Terrain::GetNormal

_inl void Terrain::InvalidateAABB( TerrainQuad* pQuad )
{
	AABoundBox aabb;
    //  do speculative bounding box estimate
    //  later real bounding box will be substituted, when geometry will be available
    aabb = AABoundBox( pQuad->GetExtents(), c_MinTerraHeight, c_MaxTerraHeight );
    //GetAABB( pQuad->GetExtents(), aabb );
	pQuad->SetQuadAABB( aabb );
} // Terrain::InvalidateAABB

_inl void Terrain::InvalidateTexture( TerrainQuad* pQuad ) 
{
	int itemID = pQuad->GetTextureID();
	if (itemID != TerrainQuad::c_BadID) m_TextureCache[itemID].m_QuadIndex = -1;
	pQuad->SetTextureID( TerrainQuad::c_BadID );
} // Terrain::InvalidateTexture

_inl void Terrain::InvalidateGeometry( TerrainQuad* pQuad )
{
    int nGeom = pQuad->GetNGeoms();
    for (int i = 0; i < nGeom; i++)
    {
	    int itemID = pQuad->GetGeometryID( i );
	    if (itemID != TerrainQuad::c_BadID) m_GeometryCache[itemID].m_QuadIndex = -1;
    }
	pQuad->SetNGeoms( 0 );
} // Terrain::InvalidateGeometry

_inl TerrainQuad* Terrain::GetQuad( int lod, float x, float y )
{
    const QuadLevel& ql = m_QuadLevel[lod];
    int nx = (x - m_Extents.x)/ql.qWidth;
    int ny = (y - m_Extents.y)/ql.qHeight;
    int qidx = nx + ny*ql.nSideQuads;
    if (qidx < 0 || qidx >= ql.nQuads) return NULL;
    return &m_Quads[ql.firstQuad + qidx];
} // Terrain::GetQuad

/*****************************************************************************/
/*	TerrainQuad implementation
/*****************************************************************************/
_inl TerrainQuad::TerrainQuad() 
{
	m_QuadAABB		= AABoundBox::null;		
	m_TexID			= c_BadID;	
	m_LOD			= 0;		
	m_LastFrame		= 0;
    m_pChunk        = NULL;
    m_NGeoms        = 0;
    SetAlreadyDrawn( false );
} // TerrainQuad::TerrainQuad

_inl Rct TerrainQuad::GetExtents() const
{ 
	return Rct( m_QuadAABB.minv.x, m_QuadAABB.minv.y, 
				m_QuadAABB.GetDX(), m_QuadAABB.GetDY() ); 
} // TerrainQuad::GetExtents

_inl void TerrainQuad::SetGeometryID( int idx, WORD id ) 
{ 
    if (m_NGeoms >= c_MaxQuadGeoms) return; 
    m_GeomID[idx] = id; 
}

_inl void TerrainQuad::AddGeometryID( WORD id ) 
{ 
    if (m_NGeoms >= c_MaxQuadGeoms) return; 
    m_GeomID[m_NGeoms++] = id; 
}   

_inl void TerrainQuad::RemoveGeometryID( WORD id )
{
    for (int i = 0; i < m_NGeoms; i++)
    {
        if (m_GeomID[i] == id)
        {
            for (int j = i; j < m_NGeoms - 1; j++)
            {
                m_GeomID[j] = m_GeomID[j + 1];
            }
            m_NGeoms--;
            return;
        }
    }
} // TerrainQuad::RemoveGeom

END_NAMESPACE(sg)
