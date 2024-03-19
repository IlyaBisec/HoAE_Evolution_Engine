/*****************************************************************************/
/*    File:    sgGeometry.h
/*    Author:    Ruslan Shestopalyuk
/*    Date:    15.04.2003
/*****************************************************************************/
#ifndef __SGGEOMETRY_H__
#define __SGGEOMETRY_H__

#include "IMediaManager.h"
#include "sgNode.h"

ENUM(PrimitiveType, "Primitive Type",
     en_val( ptUnknown,         "Unknown"        ) <<        
     en_val( ptPointList,       "PointList"      ) <<    
     en_val( ptLineList,        "LineList"       ) <<    
     en_val( ptLineStrip,       "LineStrip"      ) <<    
     en_val( ptTriangleList,    "TriangleList"   ) <<
     en_val( ptTriangleStrip,   "TriangleStrip"  ) <<    
     en_val( ptTriangleFan,     "TriangleFan"    ) );


/*****************************************************************************/
/*    Class:    Geometry
/*    Desc:    Geometry data container node
/*****************************************************************************/
class Geometry : public SNode, public IGeometry
{
    //  debug purposes
    bool                m_bShowMeshNormals;    
    float                m_NormalLen;

    static int          s_VBuffer;
    static int          s_IBuffer;

protected:
    BaseMesh            m_Mesh;                //  mesh data
    AABoundBox            m_AABB;                //  axis-aligned bounding box    
    
    DWORD               m_ICacheStamp;  //  hardware index caching watermark
    DWORD               m_VCacheStamp;  //  hardware vertex caching watermark   
    int                 m_VBufferPos;
    int                 m_IBufferPos;


public:
                        Geometry    () : m_bShowMeshNormals( false ), m_NormalLen( 20.0f ) 
                        {
                            m_ICacheStamp   = 0;
                            m_VCacheStamp   = 0;

                            m_VBufferPos    = 0;
                            m_IBufferPos    = 0;
                        }

    void                Create        ( int nVert, int nIdx, VertexFormat vertFormat, PrimitiveType ptype = ptTriangleList );
    _inl int            AddVertex    ( void* pVert );
    _inl int            AddPoly        ( WORD v1, WORD v2, WORD v3 );
    _inl bool            AddQuad        (    const Vector3D& a, const Vector3D& b,
                                        const Vector3D& c, const Vector3D& d,
                                        const Rct* pUV = NULL );

     BaseMesh&            GetMesh         ()         { return m_Mesh;  }
     const BaseMesh&    GetMesh         () const { return m_Mesh;  }

    _inl int            GetNPoly        () const { return m_Mesh.getNPri();    }
    _inl int            GetNVert        () const { return m_Mesh.getNVert();   }
    _inl PrimitiveType    GetPriType      () const { return m_Mesh.getPriType(); }
    _inl VertexFormat    GetVertexFormat () const { return m_Mesh.getVertexFormat(); }

    _inl const AABoundBox&    GetAABB        () const { return m_AABB; }
    _inl void                SetAABB        ( const AABoundBox& aabb ) { m_AABB = aabb; }

    void                DumpToCPP        ( FILE* fp );
    void                DumpToCPP        ();
    Sphere                GetBoundSphere  ();

    DWORD                GetDiffuse        () const;
    void                SetDiffuse        ( DWORD color );
    virtual void        CalculateNormals();
    virtual void        FlipNormals        ();

    virtual void        Render            ();
    virtual void        Serialize        ( OutStream&    os ) const;
    virtual void        Unserialize        ( InStream&        is );
    virtual void        Expose            ( PropertyMap&  pm );

    virtual void        GetVertexIterator( VertexIterator& it );
    bool                IsStatic        () const { return m_Mesh.isStatic(); }

    virtual int             GetVertexStride    () const { return m_Mesh.getVertexStride(); }
    virtual int             GetIndexStride    () const { return 2; }
    virtual int             GetNumVertices    () const { return m_Mesh.getNVert(); }
    virtual int             GetNumIndices    () const { return m_Mesh.getNInd(); }
    virtual Vector3D        GetCenter        () const { Vector3D c; m_Mesh.GetCentroid( c ); return c; }
    virtual WORD*           GetIndices        () { return m_Mesh.getIndices(); }
    virtual int             Pick            ( const Vector3D& org, const Vector3D& dir, Vector3D& pt );
    virtual void            SetIsStatic        ( bool val = true ); 
    void                    AttachEffect        ();

    void                PostRender      ();

    DECLARE_SCLASS(Geometry,SNode,GEOM);
}; // class Geometry 

/*****************************************************************************/
/*    Class:    GeometryRef
/*    Desc:    Geometry subset from some polygon soup node
/*****************************************************************************/
class GeometryRef : public SNode
{
    Geometry*            pool;

    DWORD                firstInd;
    DWORD                nInd;
    DWORD                firstVert;
    DWORD                nVert;

public:
    _inl                GeometryRef();

    virtual void        Draw        (){}
    virtual void        Serialize    ( OutStream&    os ) const;
    virtual void        Unserialize    ( InStream&        is );

    DECLARE_SCLASS(GeometryRef,SNode,GREF);

}; // class GeometryRef

enum MeshPointSample
{
    mpsUnknown      = 0,
    mpsVertex       = 1,    //  sampling at each mesh vertex
    mpsPolyCenter   = 2,    //  sampling at center of each polygon
    mpsPolyRandom   = 3,    //  sampling randomly in mesh polygons
}; // enum MeshPointSample
Vector3D    GetRandomPoint( const Primitive& p, MeshPointSample sample = mpsVertex );

Sphere        GetStaticBoundSphere( SNode* pNode );
AABoundBox    CalculateAABB        ( SNode* pNode );


void FlattenStaticHierarchy    ( SNode* pRootNode );
void SortPolys( const Ray3D& ray, BaseMesh& bm );

int  CountPolygons( SNode* pNode );
int  CountVertices( SNode* pNode );

//  splits mesh by plane into two meshes
bool Split( const Primitive& mesh, 
            const Plane& plane, 
            Primitive& posMesh, Primitive& negMesh );

DIALOGS_API void ScanHeightmap( const Primitive& pri, const Matrix4D& tm, float stepx, float stepy, SetHeightCallback put );
DIALOGS_API void ScanHeightmap( const Primitive& pri, const Matrix4D& tm, float stepx, float stepy, VisitHeightCallback put );

SNode* PickNode( const Ray3D& ray, SNode* pNode, Vector3D& pt );



#ifdef _INLINES
#include "sgGeometry.inl"
#endif // _INLINES

#endif // __SGGEOMETRY_H__