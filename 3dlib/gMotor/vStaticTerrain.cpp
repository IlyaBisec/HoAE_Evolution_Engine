/*****************************************************************************/
/*    File:    vStaticTerrain.cpp
/*    Desc:    Static terrain interface implementation
/*    Author:    Ruslan Shestopalyuk
/*****************************************************************************/
#include "stdafx.h"
#include "vStaticTerrain.h"

float StaticTerrainDB::GetHeight( float x, float y )
{
    return 0.0f;
} // StaticTerrainDB::GetHeight

void  StaticTerrainDB::Serialize( OutStream& os )
{
    os << m_TexTable << m_ShaderTable << m_Chunks << 
            m_TilesPerSide << m_Extents << m_NQuadLevels;
    for (int i = 0; i < m_NQuadLevels; i++) os << m_QuadLevel[i];
} // StaticTerrainDB::Serialize

void StaticTerrainDB::Unserialize( InStream& is )
{
    is >> m_TexTable >> 
        m_ShaderTable >> 
        m_Chunks >> 
        m_TilesPerSide >> 
        m_Extents >> 
        m_NQuadLevels;
    for (int i = 0; i < m_NQuadLevels; i++) is >> m_QuadLevel[i];    

    //  reindex shaders/textures
    int nTex = m_TexTable.size();
    int nSha = m_ShaderTable.size();
    for (int i = 0; i < m_Chunks.size(); i++)
    {
        StaticTerrainChunk& chunk = m_Chunks[i];
        for (int j = 0; j < chunk.m_Meshes.size(); j++)
        {
            BaseMesh& bm = *chunk.m_Meshes[j];
            int tex0 = bm.getTexture( 0 );
            int tex1 = bm.getTexture( 1 );
            int sha = bm.getShader();

            if (tex0 >= 0) bm.setTexture( IRS->GetTextureID( m_TexTable[tex0].m_Name.c_str() ), 0 );
            if (tex1 >= 0) bm.setTexture( IRS->GetTextureID( m_TexTable[tex1].m_Name.c_str() ), 1 );
            if (sha >= 0) bm.setShader( IRS->GetShaderID( m_ShaderTable[sha].m_Name.c_str() ) );
        }
    }
} // StaticTerrainDB::Unserialize

void StaticTerrainDB::SetExtents( int tilesPerSide, float x, float y, float w, float h )
{
    m_Extents       = Rct( x, y, w, h );
    m_TilesPerSide  = tilesPerSide;
    int nQuads      = 0;
    m_NQuadLevels   = 0;
    DWORD sz = m_TilesPerSide;
    while (sz) { nQuads += sz * sz; sz >>= 1; m_NQuadLevels++; }

    m_Chunks.resize( nQuads );
    if (nQuads == 0) return;

    StaticTerrainChunk& qroot   = m_Chunks[0];
   
    m_QuadLevel[0].m_FirstQuad  = 0;
    m_QuadLevel[0].m_QuadWidth  = m_Extents.w;
    m_QuadLevel[0].m_QuadHeight = m_Extents.h;
    m_QuadLevel[0].m_NQuads     = 1;
    m_QuadLevel[0].m_NSideQuads = 1;

    m_Chunks[0].m_LOD = GetPower( (PowerOfTwo)m_TilesPerSide );
    m_Chunks[0].m_Ext = m_Extents;

    int nLevelQuads = 4;
    int cQuad       = 1;
    int side        = 2;
    for (int lod = 1; lod < m_NQuadLevels; lod++)
    {
        nLevelQuads = side*side;
        float qw = m_Extents.w / float( side );
        float qh = m_Extents.h / float( side );
        m_QuadLevel[lod].m_FirstQuad  = cQuad;
        m_QuadLevel[lod].m_QuadWidth  = qw;
        m_QuadLevel[lod].m_QuadHeight = qh;
        m_QuadLevel[lod].m_NQuads     = nLevelQuads;
        m_QuadLevel[lod].m_NSideQuads = side;

        for (int j = 0; j < nLevelQuads; j++)
        {
            float qx = m_Extents.x + qw*float( j%side );
            float qy = m_Extents.y + qh*float( j/side );
            Rct qrct( qx, qy, qw, qh );

            StaticTerrainChunk& q  = m_Chunks[cQuad];
            q.m_Ext    = qrct;
            q.m_LOD    = m_NQuadLevels - lod - 1;
            cQuad++;
        }
        side *= 2;
    }    
} // StaticTerrainDB::SetExtents

int StaticTerrainDB::GetTextureID( const char* name )
{
    for (int i = 0; i < m_TexTable.size(); i++)
    {
        if (!stricmp( name, m_TexTable[i].m_Name.c_str() )) return i;
    }
    m_TexTable.push_back( StaticTerrainTex( name ) );
    return m_TexTable.size() - 1;
} // StaticTerrainDB::GetTextureID

int StaticTerrainDB::GetShaderID( const char* name )
{
    for (int i = 0; i < m_ShaderTable.size(); i++)
    {
        if (!stricmp( name, m_ShaderTable[i].m_Name.c_str() )) return i;
    }
    m_ShaderTable.push_back( StaticTerrainShader( name ) );
    return m_ShaderTable.size() - 1;
} // StaticTerrainDB::GetShaderID

StaticTerrainChunk* StaticTerrainDB::GetChunk( const Rct& ext ) 
{
    //  FIXME
    for (int i = 0; i < m_Chunks.size(); i++)
    {
        StaticTerrainChunk& chunk = m_Chunks[i];
        if (fabs( chunk.m_Ext.x - ext.x ) + 
            fabs( chunk.m_Ext.y - ext.y ) +
            fabs( chunk.m_Ext.w - ext.w ) +
            fabs( chunk.m_Ext.h - ext.h ) < c_Epsilon) return &chunk;
    }
    return NULL;
} // StaticTerrainDB::GetChunk

