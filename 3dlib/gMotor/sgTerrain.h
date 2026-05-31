/*****************************************************************************/
/*	File:	sgTerrain.h
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	11-12-2003
/*****************************************************************************/
#ifndef __SGTERRAIN_H__
#define __SGTERRAIN_H__
#include "ITerrain.h"
#include "kArray.hpp"

namespace sg{

enum TerrainQuadFlags
{
	qfAlreadyDrawn = 0x01
}; // enum TerrainQuadFlags

const int c_MaxQuadGeoms = 16;
/*****************************************************************************/
/*	Class:	TerrainQuad
/*	Desc:	Single quad in the terrain quadtree
/*****************************************************************************/
class TerrainQuad
{
protected:

	AABoundBox			m_QuadAABB;		            //  extents on the heightmap
	WORD				m_TexID;		            //  ID of the texture surface used
	WORD				m_GeomID[c_MaxQuadGeoms];	//  ID of the geometry instance used
    int                 m_NGeoms;

	WORD				m_LOD;			            //  level-of-detail index
	WORD				m_Flags;		            //  flags
	int					m_Index;		            //  index in the array of quads
	DWORD				m_LastFrame;	            //  last frame we were shown at
    TerrainQuad*        m_pChild[4];                //  pointer to children
    
    ITerrainChunk*      m_pChunk;                   //  pointer to the chunk client interface

	static const WORD c_BadID = 0xFFFF;
	friend class Terrain;

public:
	_inl				TerrainQuad ();
	bool				InvalidTexture	() const { return (m_TexID == c_BadID); }
	bool				InvalidGeometry	() const { return (m_NGeoms == 0); }

	int					GetTextureID	() const { return m_TexID; }
	int					GetGeometryID	( int idx ) const { return m_GeomID[idx]; }
    int                 GetNGeoms       () const { return m_NGeoms; }
    void                SetNGeoms       ( int val ) { m_NGeoms = val; }
	void				SetTextureID	( WORD val ) { m_TexID = val; }
	_inl void		    SetGeometryID	( int idx, WORD id );
    _inl void           AddGeometryID   ( WORD id );
    _inl void           RemoveGeometryID( WORD id );

	void				SetAlreadyDrawn ( bool val = true ) { SetFlag( m_Flags, qfAlreadyDrawn, val ); }
	bool				IsAlreadyDrawn  () const { return ((m_Flags&qfAlreadyDrawn) != 0); }

	const AABoundBox&	GetQuadAABB		() const { return m_QuadAABB; }

	void				SetQuadAABB		( const AABoundBox& aabb ) { m_QuadAABB = aabb; }

	int					GetLastFrame	() const { return m_LastFrame; }
	void				SetLastFrame	( DWORD val ) { m_LastFrame = val; }

	int					GetLOD			() const { return m_LOD; }
	_inl Rct			GetExtents		() const;
	int					GetIndex		() const { return m_Index; }
	void				SetIndex		( int val ) { m_Index = val; }

    TerrainQuad*        GetChild        ( int idx ) { return m_pChild[idx]; }
}; // class TerrainQuad 

struct TerrainTextureItem
{
	TerrainTextureItem() : m_pTexture( NULL ), m_QuadIndex( -1 ) {}
	Texture*		m_pTexture;
	int				m_QuadIndex;
}; // struct TerrainTextureItem

struct TerrainGeometryItem
{
	TerrainGeometryItem() : m_pGeometry( NULL ), m_QuadIndex( -1 ) {}

	Geometry*		m_pGeometry;
	int				m_QuadIndex;
}; // struct TerrainGeometryItem

/*****************************************************************************/
/*	Class:	TerrainBorderElement
/*	Desc:	Single piece of the terrain border description
/*****************************************************************************/
struct TerrainBorderElement
{
    int         texID;      //  texture ID
    Rct         uv;         //  texture coordinates on this texture
}; // struct TerrainBorderElement

const int   c_MaxTextureCacheSize	= 128;
const int   c_GeometryCacheSize	    = 800;
const int   c_MaxTerraQuadLevels	= 16;
const float c_MinTerraHeight        = -1000.0f;
const float c_MaxTerraHeight        = 3000.0f;
const float c_TerrainCacheVMEMQuote = 0.3; 

struct CameraFrame
{
    Vector3D        pos;
    Vector3D        dir;
}; // struct CameraFrame
       
/*****************************************************************************/
/*	Class:	Terrain
/*	Desc:	Terrain renderer
/*****************************************************************************/
class Terrain : public Node, public ITerrain, public IDeviceClient
{
public:
						Terrain				();
						~Terrain			();

	void				Reset				();
	virtual void		Render				();
	virtual void		Expose				( PropertyMap&	pmap	);
	virtual void		Serialize			( OutStream&	os		) const;
	virtual void		Unserialize			( InStream&		is		);
    virtual void        Init                ();

	virtual void		SetPerSidePow		( PowerOfTwo val );
	virtual void		OnDestroyRenderSystem();
	virtual void		OnCreateRenderSystem(){}

	_inl Vector3D		GetNormal			( float x, float y ) const;

	void				ProcessCulling		();
	int					GetDrawQueueSize	() const { return m_QDrawn.size(); }
    virtual void        ResetDrawQueue      () { m_QDrawn.clear(); } 

	_inl PowerOfTwo		GetPerSidePow		() const { return (PowerOfTwo)m_TilesPerSide; }
	_inl int			GetSegmentsPerTile	() const { return m_SegmentsPerTile; }
	_inl float			GetMinTileSide		() const { return m_OneTileSide; }

	virtual void		SetCallback			( GetHeightCallback	callb ) { GetHeight			= callb; }
	virtual void		SetCallback			( SetHeightCallback	callb ) { SetHeight			= callb; }
	virtual void		SetCallback			( TextureCallback	callb ) { CreateTexture		= callb; }
	virtual void		SetCallback			( GeometryCallback	callb ) { CreateGeometry	= callb; }
	virtual void		SetCallback			( AABBCallback		callb ) { GetAABB			= callb; }
	virtual void		SetCallback			( PVSCallback		callb ) { GetPVS			= callb; }
    virtual void		SetCallback			( FactoryCallback   callb ) { CreateChunk       = callb; }
    
    virtual void        ClearPVS            () { m_QDrawn.clear(); }

	virtual void		ForceLOD			( int lod ) { m_MinLOD = lod; }

	bool				Pick				( const Vector3D& orig, const Vector3D& dir, Vector3D& pt );
	bool				Pick				( int mX, int mY, Vector3D& pt );

	void				SetH				( int nx, int ny, float h ) { SetHeight( nx, ny, h ); }
	float				GetH				( float nx, float ny ) const { return GetHeight( nx, ny ); }
	
	virtual void		SetLODBias			( float bias ) { m_LODBias = bias; }

	void				SetDrawCulling		( bool bValue = true ) { m_bDrawCulling		= bValue; }
	void				SetDrawGeomCache	( bool bValue = true ) { m_bDrawGeomCache	= bValue; }
	void				SetDrawTexCache		( bool bValue = true ) { m_bDrawTexCache	= bValue; }
	void				SetDrawAABB			( bool bValue = true ) { m_bDrawAABB		= bValue; }

	void				EnableVertexLighting( bool val = true );
	bool				VertexLightingEnabled() const;

	virtual void		InvalidateAABB		( const Rct* rct = NULL );
	virtual void		InvalidateGeometry	( const Rct* rct = NULL );
	virtual void		InvalidateTexture	( const Rct* rct = NULL );

	bool		        PrecacheTexture		( TerrainQuad* pQuad );
	bool		        PrecacheGeometry	( TerrainQuad* pQuad );

	void				SetTextureCacheSize	( int nSurf );
	void				SetGeometryCacheSize( int nGeom );

	int					GetGeometryCacheSize() const { return m_GeometryCache.size(); }
	int					GetTextureCacheSize	() const { return m_TextureCache.size(); }
	
	void				DoVisibilityCulling	();

	virtual void		SetExtents			( const Rct& ext );
	virtual void		SetHeightmapPow		( int hpow );
	virtual void		Show 				( bool bShow = true ) { SetDisabled( !bShow ); SetInvisible( !bShow ); }
	virtual bool		IsShown				() const { return !IsDisabled() && !IsInvisible(); }

	virtual Rct			GetExtents			() const { return m_Extents; }
	_inl TerrainQuad*	RootQuad			() { return m_Quads.size() ? &m_Quads[0] : NULL; }
	
	int					AllocateGeometryItem();
	int					AllocateTextureItem	();
    virtual BaseMesh*   AllocateGeometry    ();

    virtual void        DrawBorder          ();
    virtual bool        SetBorderConfigFile ( const char* fname );
    virtual void        ShowNormals         ( bool bShow = true ) { m_bShowNormals = bShow; }

	typedef VertexN		TerrainVertex;

	NODE(Terrain,Node,TERR);

protected:

	static const int c_GridStep	= 32;

	typedef c2::static_array<TerrainQuad*, c_GeometryCacheSize*8>	QuadPVSArray;
	QuadPVSArray						m_QDrawn;
	std::vector<TerrainQuad>			m_Quads;

    struct QuadLevel
    {
        int                             firstQuad;
        float                           qWidth;
        float                           qHeight;
        int                             nQuads;
        int                             nSideQuads;
    }; // struct QuadLevel

	QuadLevel						    m_QuadLevel[c_MaxTerraQuadLevels];
	int									m_NQuadLevels;

	Rct									m_Extents;

    int                                 m_TextureCacheSize;
	int									m_TexturesCreated;
	int									m_GeomCreated;
	int									m_MinLOD;
    bool                                m_bNeedPrecache;

    int                                 m_CurrentQuad;

	Group*								m_pTextureCache;
	Group*								m_pGeometryCache;

	std::vector<TerrainTextureItem>		m_TextureCache;
	std::vector<TerrainGeometryItem>	m_GeometryCache;

    static int              DistCmp( const void *q1, const void *q2 );
    static Ray3D            s_PickRay;

	void					DrawCulling			();
	void					DrawTexCache		();
	void					DrawGeomCache		();
    void                    DrawNormals         ();

	void					FreezeCamera		( bool freeze = true );
	bool					IsFrozenCamera		() const { return m_FreezeCamera != NULL; }
	
	_inl void				InvalidateAABB		( TerrainQuad* pQuad );
	_inl void				InvalidateTexture	( TerrainQuad* pQuad ); 
	_inl void				InvalidateGeometry	( TerrainQuad* pQuad );

	int						GetTexSet			() const;
	void					SetTexSet			( int val ); 

	float					GetPShiftX			() const { return s_PShift.x; }
	float					GetPShiftY			() const { return s_PShift.y; }
	float					GetPShiftZ			() const { return s_PShift.z; }

	void					SetPShiftX			( float val ) { s_PShift.x = val; }
	void					SetPShiftY			( float val ) { s_PShift.y = val; }
	void					SetPShiftZ			( float val ) { s_PShift.z = val; }
																 
	friend class			TerrainEditor;						 

private:

	static int				DefGetHeight		( int, int );
	static void				DefSetHeight		( int, int, float ) {}
	static bool				DefCreateTexture	( int texID, const Rct& mapExt );
	static bool				DefCreateGeometry	( const Rct& mapExt, int lod );
	static void				DefGetAABB			( const Rct& mapExt, AABoundBox& aabb );
	static int				DefGetPVS			( DWORD* qID, int maxq );
    static ITerrainChunk*   DefCreateChunk      ();

	bool					CreateQuadTexture	( int quadIdx, int geomID );

    bool                    InitBorderElement   ( TerrainBorderElement& bel, XMLNode* pNode );
    _inl TerrainQuad*       GetQuad             ( int lod, float x, float y );
    void                    PrecacheQuad        ( TerrainQuad* pQ );

	GetHeightCallback		GetHeight;
	SetHeightCallback		SetHeight;

	TextureCallback			CreateTexture;
	GeometryCallback		CreateGeometry;
	AABBCallback			GetAABB;
	PVSCallback				GetPVS;
    FactoryCallback         CreateChunk;

	static Vector3D			s_PShift;

	bool					m_bShowNormals;
	bool					m_bDrawCulling;
	bool					m_bDrawGeomCache;
	bool					m_bDrawTexCache;
	bool					m_bDrawAABB;
	float					m_ShowNormalLen;

	int						m_TilesPerSide;
	int						m_QuadTextureSide;
	float					m_OneTileSide;
	int						m_SegmentsPerTile;
	
	float					m_GroundHeight;
	float					m_LODBias;
	int						m_LastGeometryItem;
	int						m_LastTextureItem;

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

    //  terrain border 
    TerrainBorderElement    m_BorderT;
    TerrainBorderElement    m_BorderB;
    TerrainBorderElement    m_BorderL;
    TerrainBorderElement    m_BorderR;
    TerrainBorderElement    m_BorderLT;
    TerrainBorderElement    m_BorderLB;
    TerrainBorderElement    m_BorderRT;
    TerrainBorderElement    m_BorderRB;
    bool                    m_bDrawBorder;
    float                   m_BorderLeftEdge;  
    float                   m_BorderRightEdge; 
    float                   m_BorderTopEdge;   
    float                   m_BorderBottomEdge;
    float                   m_BorderMinZ;
    float                   m_BorderMaxZ;
    DWORD                   m_BorderColor;

	BaseCamera*				m_FreezeCamera;
}; // class Terrain

/*****************************************************************************/
/*	Class:	TerrainChunk
/*	Desc:	Terrain chunk implementation
/*****************************************************************************/
class TerrainChunk : public ITerrainChunk
{
public:
    virtual             ~TerrainChunk(){}

}; // class TerrainChunk

}; // namespace sg

#ifdef _INLINES
#include "sgTerrain.inl"
#endif // _INLINES

#endif // __SGTERRAIN_H__
