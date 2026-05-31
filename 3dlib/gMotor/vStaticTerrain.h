/*****************************************************************************/
/*    File:    vStaticTerrain.h
/*    Desc:    Static terrain interface implementation
/*    Author:    Ruslan Shestopalyuk
/*****************************************************************************/
#ifndef __VSTATICTERRAIN_H__
#define __VSTATICTERRAIN_H__

#include <vector>

#include "kIOHelpers.h"

struct StaticTerrainChunk
{
    Rct                         m_Ext;
    AABoundBox                  m_AABB;
    int                         m_LOD;
    std::vector<BaseMesh*>      m_Meshes;

    friend inline OutStream&    operator <<( OutStream& os, const StaticTerrainChunk& chunk );
    friend inline InStream&        operator >>( InStream& is, StaticTerrainChunk& chunk );

}; // struct StaticTerrainChunk

inline OutStream& operator <<( OutStream& os, const StaticTerrainChunk& chunk ) 
{
    int nMeshes = chunk.m_Meshes.size();
    os << chunk.m_Ext << chunk.m_AABB << chunk.m_LOD << nMeshes;
    for (int i = 0; i < nMeshes; i++) 
    {
        os <<   chunk.m_Meshes[i]->getTexture()     << 
                chunk.m_Meshes[i]->getTexture( 1 )  << 
                chunk.m_Meshes[i]->getShader()      << *chunk.m_Meshes[i];
    }
    return os;
}

inline InStream& operator >>( InStream& is, StaticTerrainChunk& chunk )
{
    int nMeshes = 0;
    is >> chunk.m_Ext >> chunk.m_AABB >> chunk.m_LOD >> nMeshes;
    chunk.m_Meshes.resize( nMeshes );
    for (int i = 0; i < nMeshes; i++) 
    {
        chunk.m_Meshes[i] = new BaseMesh();
        int tex0, tex1, sha;
        is >> tex0 >> tex1 >> sha >> *chunk.m_Meshes[i];
        chunk.m_Meshes[i]->setTexture( tex0, 0 );
        chunk.m_Meshes[i]->setTexture( tex1, 1 );
        chunk.m_Meshes[i]->setShader( sha );
    }
    return is;
}

struct StaticTerrainTex
{
    std::string     m_Name;
    int             m_ID;

    StaticTerrainTex( const char* name ) { m_Name = name; m_ID = -1; }
    StaticTerrainTex() { m_ID = -1; }

    friend inline OutStream&    operator <<( OutStream& os, const StaticTerrainTex& t );
    friend inline InStream&        operator >>( InStream& is, StaticTerrainTex& t );
}; // struct StaticTerrainTex

inline OutStream& operator <<( OutStream& os, const StaticTerrainTex& t )
{
    os << t.m_Name;
    return os;
}

inline InStream& operator >>( InStream& is, StaticTerrainTex& t )
{
    is >> t.m_Name;
    return is;
}

struct StaticTerrainShader
{
    std::string     m_Name;
    int             m_ID;

    StaticTerrainShader( const char* name ) { m_Name = name; m_ID = -1; }
    StaticTerrainShader() { m_ID = -1; }

    friend inline OutStream&    operator <<( OutStream& os, const StaticTerrainShader& t );
    friend inline InStream&        operator >>( InStream& is, StaticTerrainShader& t );
}; // struct StaticTerrainTex

inline OutStream& operator <<( OutStream& os, const StaticTerrainShader& t ) 
{
    os << t.m_Name;
    return os;
}

inline InStream& operator >>( InStream& is, StaticTerrainShader& t )
{
    is >> t.m_Name;
    return is;
}

struct QuadLevel
{
    int       m_FirstQuad;
    float     m_QuadWidth;
    float     m_QuadHeight;
    int       m_NQuads;
    int       m_NSideQuads;

    friend inline OutStream&    operator <<( OutStream& os, const QuadLevel& q );
    friend inline InStream&        operator >>( InStream& is, QuadLevel& q );
}; // struct QuadLevel

inline OutStream& operator <<( OutStream& os, const QuadLevel& q )
{
    os << q.m_FirstQuad << q.m_QuadWidth << q.m_QuadHeight << q.m_NQuads << q.m_NSideQuads;
    return os;
}

inline InStream& operator >>( InStream& is, QuadLevel& q )
{
    is >> q.m_FirstQuad >> q.m_QuadWidth >> q.m_QuadHeight >> q.m_NQuads >> q.m_NSideQuads;
    return is;
}

const int c_MaxQuadLevels = 16;
/*****************************************************************************/
/*  Class:  StaticTerrainDB
/*  Desc:   
/*****************************************************************************/
class StaticTerrainDB
{
    std::vector<StaticTerrainTex>       m_TexTable;
    std::vector<StaticTerrainShader>    m_ShaderTable;
    std::vector<StaticTerrainChunk>     m_Chunks;

    int                                 m_TilesPerSide;
    Rct                                 m_Extents;
    QuadLevel                            m_QuadLevel[c_MaxQuadLevels];
    int                                    m_NQuadLevels;    

public:
    float               GetHeight       ( float x, float y );
    void                Serialize       ( OutStream& os );
    void                Unserialize     ( InStream& is );
    void                SetExtents      ( int tilesPerSide, float x, float y, float w, float h );
    int                 GetTextureID    ( const char* name );
    int                 GetShaderID     ( const char* name );
    StaticTerrainChunk* GetChunk        ( const Rct& ext );


}; // class StaticTerrainDB


#endif // __VSTATICTERRAIN_H__