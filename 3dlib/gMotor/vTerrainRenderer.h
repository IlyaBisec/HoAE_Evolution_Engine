/*****************************************************************************/
/*    File:    vTerrainRenderer.h
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    11-12-2003
/*****************************************************************************/
#ifndef __VTERRAINRENDERER_H__
#define __VTERRAINRENDERER_H__
#include "ITerrain.h"
#include "kStaticArray.hpp"

#include "mNoise.h"

const int c_MaxQuadGeoms = 32;
/*****************************************************************************/
/*    Class:    TerrainQuad
/*    Desc:    Single quad in the terrain quadtree
/*****************************************************************************/
class TerrainQuad
{
public:
    AABoundBox              m_QuadAABB;                    //  extents on the heightmap
    WORD                    m_TexID;                    //  ID of the texture surface used
    WORD                    m_GeomID[c_MaxQuadGeoms];    //  ID of the geometry instance used
    int                     m_NGeoms;

    WORD                    m_LOD;                        //  level-of-detail index
    bool                    m_bAlreadyDrawn;                    
    int                     m_Index;                    //  index in the array of quads
    DWORD                   m_LastFrame;                //  last frame we were shown at
    TerrainQuad*            m_pChild[4];                //  pointer to children
    TerrainQuad*            m_pParent;

    ITerrainChunk*          m_pChunk;                   //  pointer to the chunk client interface
    static const WORD       c_BadID = 0xFFFF;


    _inl                    TerrainQuad     ();
    bool                    InvalidTexture  () const { return (m_TexID == c_BadID); }
    bool                    InvalidGeometry () const { return (m_NGeoms == 0); }

    int                     GetTextureID    () const { return m_TexID; }
    int                     GetGeometryID   ( int idx ) const { return m_GeomID[idx]; }
    int                     GetNGeoms       () const { return m_NGeoms; }
    void                    SetNGeoms       ( int val ) { m_NGeoms = val; }
    void                    SetTextureID    ( WORD val ) { m_TexID = val; }
    _inl void               SetGeometryID   ( int idx, WORD id );
    _inl void               AddGeometryID   ( WORD id );
    _inl void               RemoveGeometryID( WORD id );

    void                    SetAlreadyDrawn ( bool val = true ) { m_bAlreadyDrawn = val; }
    bool                    IsAlreadyDrawn  () const { return m_bAlreadyDrawn; }
    const AABoundBox&       GetQuadAABB     () const { return m_QuadAABB; }
    void                    SetQuadAABB     ( const AABoundBox& aabb ) { m_QuadAABB = aabb; }

    int                     GetLastFrame    () const { return m_LastFrame; }
    void                    SetLastFrame    ( DWORD val ) { m_LastFrame = val; }

    int                     GetLOD          () const { return m_LOD; }
    _inl Rct                GetExtents      () const;
    int                     GetIndex        () const { return m_Index; }
    void                    SetIndex        ( int val ) { m_Index = val; }
    _inl bool               SatisfiesLOD    ( const Vector3D& vpos, float quality );

    TerrainQuad*            GetChild        ( int idx ) { return m_pChild[idx]; }
}; // class TerrainQuad 

struct TerrainTextureItem
{
    TerrainTextureItem() : m_TexID( -1 ), m_QuadIndex( -1 ) {}
    int                m_TexID;
    int                m_QuadIndex;
}; // struct TerrainTextureItem

struct TerrainGeometryItem
{
    TerrainGeometryItem() : m_QuadIndex( -1 ), m_IBStamp(0), m_VBStamp(0) {}

    BaseMesh        m_Mesh;
    int             m_QuadIndex;
    
    DWORD           m_IBStamp;
    DWORD           m_VBStamp;

    int             m_IBPos;
    int             m_VBPos;

    void Free()
    {
        m_IBStamp   = 0;
        m_VBStamp   = 0;
    }
}; // struct TerrainGeometryItem

const int   c_TextureCacheSize        = 96;
const int   c_GeometryCacheSize        = 2000;
const int   c_MaxTerraQuadLevels    = 16;
const float c_MinTerraHeight        = -1000.0f;
const float c_MaxTerraHeight        = 3000.0f;

struct CameraFrame
{
    Vector3D        pos;
    Vector3D        dir;
}; // struct CameraFrame

const int c_TerrainIBufferBytes = 1024*1024*2;
const int c_TerrainVBufferBytes = 1024*1024*16;

/*****************************************************************************/
/*    Class:    PerlinHeightMap
/*    Desc:    Simple procedural on-the-fly heightmap generator
/*****************************************************************************/
class PerlinHeightMap
{
public:
    float GetH( float x, float y ) const
    {
        return 0.0f;
        float f = PerlinNoise( ((float)x + 1530.0f)*0.002f, 
                            ((float)y + 230.0f)*0.002f )*3600.0f + 
                PerlinNoise( ((float)x + 530.0f)*0.0005f, 
                            ((float)y + 130.0f)*0.0005f )*8000.0f;
        float k = (x*x + y*y)/2048.0f/2048.0f;
        clamp( k, 0.0f, 1.0f );
        f *= 1.0f - k;
        f += 100.0f;
        return f;
    } 
    void SetHeight( float x, float y, float h ){}
    void SetExtents( const Rct& ext )
    {
    }

    Vector3D GetNormal( float x, float y ) const
    {
        const float c_NormalStep = 32.0f;

        float l = GetH( x - c_NormalStep, y );        
        float r = GetH( x + c_NormalStep, y );        
        float u = GetH( x, y - c_NormalStep );        
        float d = GetH( x, y + c_NormalStep );        

        Vector3D normal;
        normal.x = l - r;
        normal.y = u - d;
        normal.z = 2.0f*c_NormalStep;
        normal.normalize();
        return normal;
    } // GetNormal

    Vector3D GetAvgNormal( float x, float y, float radius ) const
    {
        const float c_NormalStep = 32.0f;
        float s = radius*2.0f;
        Vector3D n;
        n.zero();
        for (float cx = 0.0f; cx <= s; cx += c_NormalStep)
        {
            for (float cy = 0.0f; cy <= s; cy += c_NormalStep)
            {   
                n += GetNormal( cx + x, cy + y );
            }
        }
        n.normalize();
        return n;
    } // HeightMap::GetNormal
}; // class PerlinHeightMap

/*****************************************************************************/
/*    Class:    TableHeightMap
/*    Desc:    
/*****************************************************************************/
class TableHeightMap
{
    AlignedBuffer<WORD,32>      m_Val;
    AlignedBuffer<DWORD,32>     m_Color;
    int                         m_Width;
    int                         m_Height;
    Rct                         m_Ext;
    float                       m_WCellSide;
    float                       m_HCellSide;


public:

    WORD GetValue( int x, int y ) const
    {
        if (!m_Val.GetSize()) return 0;
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (x >= m_Width ) x = m_Width  - 1;
        if (y >= m_Height) y = m_Height - 1;
        return m_Val[x + y*m_Width];
    }

    float GetH( float x, float y ) const
    {
        // find square
        float bX = (x - m_Ext.x)*m_WCellSide;
        float bY = (y - m_Ext.y)*m_HCellSide;

        const float minX = float( floor( bX ) );
        const float minY = float( floor( bY ) );
        int intX = int( minX );
        int intY = int( minY );

        const float wx = bX - minX;
        const float wy = bY - minY;

        if (fabs( wx ) < c_SmallEpsilon && fabs( wy ) < c_SmallEpsilon) 
        {
            return GetValue( intX, intY );
        }
        const float p11 = GetValue( intX,        intY     );
        const float p12 = GetValue( intX,        intY + 1 );
        const float p21 = GetValue( intX + 1,    intY     );
        const float p22 = GetValue( intX + 1,    intY + 1 );

        const float px1 = p11 + wx * ( p21 - p11 );
        const float px2 = p12 + wx * ( p22 - p12 );
        const float p = px1 + wy * ( px2 - px1 );

        return p*0.3f;
    } 
    
    Vector3D GetNormal( float x, float y ) const
    {
        const float c_NormalStep = 32.0f;

        float l = GetH( x - c_NormalStep, y );        
        float r = GetH( x + c_NormalStep, y );        
        float u = GetH( x, y - c_NormalStep );        
        float d = GetH( x, y + c_NormalStep );        

        Vector3D normal;
        normal.x = l - r;
        normal.y = u - d;
        normal.z = 2.0f*c_NormalStep;
        normal.normalize();
        return normal;
    } // GetNormal

    Vector3D GetAvgNormal( float x, float y, float radius ) const
    {
        const float c_NormalStep = 32.0f;
        float s = radius*2.0f;
        Vector3D n;
        n.zero();
        for (float cx = 0.0f; cx <= s; cx += c_NormalStep)
        {
            for (float cy = 0.0f; cy <= s; cy += c_NormalStep)
            {   
                n += GetNormal( cx + x, cy + y );
            }
        }
        n.normalize();
        return n;
    } // HeightMap::GetNormal

    void SetHeight( float x, float y, float h ){}

    void SetExtents( const Rct& ext )
    {
        m_WCellSide = m_Width/ext.w; 
        m_HCellSide = m_Height/ext.h; 
        m_Ext = ext;
    }

    TableHeightMap()
    {
        m_WCellSide = 1.0f;
        m_HCellSide = 1.0f;
        
        Create( "q:\\models\\terra.raw", 513, 513, Rct( -2560, -2560, 5130, 5130 ) );
    }

    bool Create( const char* fname, int wNodes, int hNodes, const Rct& ext )
    {
        m_Width  = wNodes;
        m_Height = hNodes;
        m_Val.Resize( m_Width*m_Height );
        m_Val.Reset( 0 );

        SetExtents( ext );

        FInStream is( fname );
        if (is.NoFile()) return false;
        is.Read( (BYTE*)m_Val.GetData(), m_Width*m_Height*2 );
        return true;
    }
}; // class TableHeightMap

/*****************************************************************************/
/*    Class:    DefaultTerrainCore
/*****************************************************************************/
class DefaultTerrainCore : public ITerrainCore
{
    //TableHeightMap      m_HeightMap;
    PerlinHeightMap      m_HeightMap;

    typedef Vertex2t        TerrainVertex;
public:
    virtual bool                CreateTexture    ( int texID, const Rct& mapExt );
    virtual bool                CreateGeomery    ( const Rct& mapExt, int lod );
    virtual bool                GetAABB         ( const Rct& mapExt, AABoundBox& aabb );
    virtual float                GetHeight        ( float x, float y );
    virtual VertexDeclaration   GetVDecl        () const;
    virtual void                SetExtents      ( const Rct& rct );
    virtual Vector3D            GetNormal        ( float x, float y ) { return m_HeightMap.GetNormal( x, y ); }
    virtual Vector3D            GetAvgNormal    ( float x, float y, float radius ) { return m_HeightMap.GetAvgNormal( x, y, radius ); }

}; // class ITerrainCore

/*****************************************************************************/
/*    Class:    TerrainRenderer
/*    Desc:    Chunked LOD quadtree terrain renderer
/*****************************************************************************/
class TerrainRenderer : public IReflected, 
                        public ITerrain, 
                        public IDeviceClient
{
public:    
                        TerrainRenderer        ();
    void                Reset                ();
    virtual void        Render                ();
    virtual void        Expose                ( PropertyMap&    pmap    );
    virtual void        Serialize            ( OutStream&    os        ) const;
    virtual void        Unserialize            ( InStream&        is        );
    virtual void        Init                ();

    virtual void        SetPerSidePow        ( PowerOfTwo val );
    virtual void        OnDestroyRS         ();
    virtual void        OnCreateRS          (){}

    void                ProcessCulling        ();
    int                    GetDrawQueueSize    () const { return m_QDrawn.size(); }
    virtual void        ResetDrawQueue      () { m_QDrawn.clear(); } 
    _inl PowerOfTwo        GetPerSidePow        () const { return (PowerOfTwo)m_TilesPerSide; }
    _inl int            GetSegmentsPerTile    () const { return m_SegmentsPerTile; }
    _inl float            GetMinTileSide        () const { return m_OneTileSide; }
    
    virtual void        ForceLOD            ( int lod ) { m_MinLOD = lod; m_MaxLOD = lod; }

    bool                Pick                ( const Vector3D& orig, const Vector3D& dir, Vector3D& pt );
    bool                Pick                ( int mX, int mY, Vector3D& pt );

    void                SetH                ( int nx, int ny, float h ) { m_pCore->SetHeight( nx, ny, h ); }
    float                GetH                ( float nx, float ny ) const { return m_pCore->GetHeight( nx, ny ); }
    
    virtual void        SetLODBias            ( float bias ) { m_LODBias = bias; }

    void                SetDrawCulling        ( bool bValue = true ) { m_bDrawCulling        = bValue; }
    void                SetDrawGeomCache    ( bool bValue = true ) { m_bDrawGeomCache    = bValue; }
    void                SetDrawTexCache        ( bool bValue = true ) { m_bDrawTexCache    = bValue; }
    void                SetDrawAABB            ( bool bValue = true ) { m_bDrawAABB        = bValue; }

    virtual void        InvalidateAABB        ( const Rct* rct = NULL );
    virtual void        InvalidateGeometry    ( const Rct* rct = NULL );
    virtual void        InvalidateTexture    ( const Rct* rct = NULL );
    virtual void        SetInvalidateCallback( tpInvalidateQuadCallback* cb);

    virtual void        SetCore             ( ITerrainCore* pCore );
    virtual void        SubstShader         ( int shaderID ); 

    bool                PrecacheTexture        ( TerrainQuad* pQuad );
    bool                PrecacheGeometry    ( TerrainQuad* pQuad );

    void                SetTextureCacheSize    ( int nSurf );
    void                SetGeometryCacheSize( int nGeom );

    int                    GetGeometryCacheSize() const { return m_GeometryCache.size(); }
    int                    GetTextureCacheSize    () const { return m_TextureCache.size(); }
    
    void                DoVisibilityCulling    ();
    void                ProcessQuadCulling  ( TerrainQuad* pQuad );

    virtual void        SetExtents            ( const Rct& ext );
    virtual void        SetHeightmapPow        ( int hpow );

    virtual Rct            GetExtents            () const { return m_Extents; }
    _inl TerrainQuad*    RootQuad            () { return m_Quads.size() ? &m_Quads[0] : NULL; }
    
    int                    AllocateGeometryItem();
    int                    AllocateTextureItem    ();
    virtual BaseMesh*   AllocateGeometry    ();
    

    virtual void        DrawPatch           (   float worldX, float worldY, 
                                                float width, float height, 
                                                float rotation, 
                                                const Rct& uv, float ux, float uy,
                                                int texID, DWORD color, bool bAdditive );
    virtual void        FlushPatches        ();

    virtual void        ShowNormals         ( bool bShow = true ) { m_bShowNormals = bShow; }

    Vector3D GetNormal( float x, float y ) const
    {
        return m_pCore->GetNormal( x, y );
    } // GetNormal

    Vector3D GetAvgNormal( float x, float y, float radius ) const
    {
        return m_pCore->GetAvgNormal( x, y, radius );
    }

protected:

    static const int c_GridStep    = 32;

    typedef static_array<TerrainQuad*, c_GeometryCacheSize*8>    QuadPVSArray;
    QuadPVSArray                        m_QDrawn;
    std::vector<TerrainQuad>            m_Quads;
    std::vector<tpInvalidateQuadCallback*> m_InvalidateCallbacks;

    struct QuadLevel
    {
        int                             firstQuad;
        float                           qWidth;
        float                           qHeight;
        int                             nQuads;
        int                             nSideQuads;
    }; // struct QuadLevel

    QuadLevel                            m_QuadLevel[c_MaxTerraQuadLevels];
    int                                    m_NQuadLevels;

    Rct                                    m_Extents;

    int                                    m_TexturesCreated;
    int                                    m_GeomCreated;
    int                                    m_MinLOD;
    int                                 m_MaxLOD;
    bool                                m_bNeedPrecache;
    float                               m_Quality;
    int                                 m_SubstShader;

    int                                 m_CurrentQuad;
    int                                 m_IBID, m_VBID;

    Vector3D                            m_ViewerPos;   
    Frustum                             m_CullFrustum; 

    std::vector<TerrainTextureItem>        m_TextureCache;
    std::vector<TerrainGeometryItem>    m_GeometryCache;

    VertexDeclaration                   m_VDecl;

    static int              DistCmp( const void *q1, const void *q2 );

    void                    DrawCulling            ();
    void                    DrawTexCache        ();
    void                    DrawGeomCache        ();
    void                    DrawNormals         ();

    void                    InvalidateAABB        ( TerrainQuad* pQuad );
    void                    InvalidateTexture    ( TerrainQuad* pQuad ); 
    void                    InvalidateGeometry    ( TerrainQuad* pQuad );

    friend class            TerrainEditor;                         

private:

    bool                    CreateQuadTexture    ( int quadIdx, int geomID );
    void                    PrecacheQuad        ( TerrainQuad* pQ );
    TerrainQuad*            GetQuad             ( int lod, float x, float y )
    {
        const QuadLevel& ql = m_QuadLevel[lod];
        int nx = (x - m_Extents.x)/ql.qWidth;
        int ny = (y - m_Extents.y)/ql.qHeight;
        int qidx = nx + ny*ql.nSideQuads;
        if (qidx < 0 || qidx >= ql.nQuads) return NULL;
        return &m_Quads[ql.firstQuad + qidx];
    } // GetQuad
    
    ITerrainCore*           m_pCore;

    bool                    m_bShowNormals;
    bool                    m_bDrawCulling;
    bool                    m_bDrawGeomCache;
    bool                    m_bDrawTexCache;
    bool                    m_bDrawAABB;
    float                    m_ShowNormalLen;

    int                        m_TilesPerSide;
    int                        m_QuadTextureSide;
    float                    m_OneTileSide;
    int                        m_SegmentsPerTile;
    
    float                    m_GroundHeight;
    float                    m_LODBias;
    int                        m_LastGeometryItem;
    int                        m_LastTextureItem;

    float                   m_TopBand;
    float                   m_BottomBand;
    float                   m_LeftBand;
    float                   m_RightBand;
    float                   m_CullBand;

    CameraFrame             m_LastCamera;
    int                     m_ShadowLayerShader;
    bool                    m_bVertexLighting;

    int                     m_NTexVisible;
    int                     m_NPrecached;
}; // class TerrainRenderer

#include "vTerrainRenderer.inl"

#endif // __VTERRAINRENDERER_H__
