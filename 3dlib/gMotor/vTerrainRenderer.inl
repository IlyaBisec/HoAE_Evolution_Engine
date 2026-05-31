/*****************************************************************************/
/*	File:	vTerrainRenderer.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	11-12-2003
/*****************************************************************************/
#include "kNAryIterator.hpp"

/*****************************************************************************/
/*	TerrainQuad implementation
/*****************************************************************************/
inline TerrainQuad::TerrainQuad() 
{
	m_QuadAABB		= AABoundBox::null;		
	m_TexID			= c_BadID;	
	m_LOD			= 0;		
	m_LastFrame		= 0;
    m_pChunk        = NULL;
    m_NGeoms        = 0;
    m_pParent       = NULL;

    SetAlreadyDrawn( false );
} // TerrainQuad::TerrainQuad

inline Rct TerrainQuad::GetExtents() const
{ 
	return Rct( m_QuadAABB.minv.x, m_QuadAABB.minv.y, 
				m_QuadAABB.GetDX(), m_QuadAABB.GetDY() ); 
} // TerrainQuad::GetExtents

inline void TerrainQuad::SetGeometryID( int idx, WORD id ) 
{ 
    if (m_NGeoms >= c_MaxQuadGeoms) return; 
    m_GeomID[idx] = id; 
}

inline void TerrainQuad::AddGeometryID( WORD id ) 
{ 
    if (m_NGeoms >= c_MaxQuadGeoms) return; 
    m_GeomID[m_NGeoms++] = id; 
}   

inline void TerrainQuad::RemoveGeometryID( WORD id )
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

inline bool TerrainQuad::SatisfiesLOD( const Vector3D& vpos, float quality )
{
    float qs = (m_QuadAABB.maxv.x - m_QuadAABB.minv.x);
    qs *= qs;
    Vector3D c = m_QuadAABB.GetCenter();
    c -= vpos;
    float d = c.norm2();
    d -= qs;
    return (d > qs*quality);
}

