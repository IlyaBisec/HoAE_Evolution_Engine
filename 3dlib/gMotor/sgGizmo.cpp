#include "stdafx.h"
#include "sg.h"
#include "sgNodePool.h"
#include "sgNode.h"
#include "sgTransformNode.h"
#include "sgGeometry.h"
#include "sgGizmo.h"

IMPLEMENT_CLASS( VectorField );

/*****************************************************************************/
/*    VectorField implementation
/*****************************************************************************/
bool VectorField::CreateNormalField( Geometry* pGeom, DWORD color )
{
    if (!pGeom) return false;

    BaseMesh& bm = pGeom->GetMesh();
    int nV = bm.getNVert();
    CreateVectorField( nV );

    AABoundBox aabb = bm.GetAABB();
    float scale = 0.02f * aabb.GetDiagonal();

    Vertex2t vert;
    vert.diffuse = color;

    VertexIterator v; v << bm;
    for (int i = 0; i < nV; i++)
    {
        Vector3D& p = v.pos();
        Vector3D  n = v.normal();
        n.normalize();
        n *= scale;

        AddVector( p, n, color );
        ++v;
    }
    
    BaseMesh& tbm = GetMesh();
    tbm.setNVert( nV*2 );
    tbm.setNPri( nV );

    return true;
} // VectorField::CreateNormalField

bool VectorField::CreateVectorField( int nVectors )
{
    Create( nVectors * 2, 0, vfVertex2t, ptLineList );
    return true;
}

bool VectorField::AddVector( const Vector3D& pos, const Vector3D& dir, DWORD color )
{
    Vertex2t vert;
    vert.diffuse = color;
    BaseMesh& bm = GetMesh();

    int nV = bm.getNVert();
    int nP = bm.getNPri();

    if (nV > bm.getMaxVert() - 2) return false;

    vert.x = pos.x;
    vert.y = pos.y;
    vert.z = pos.z;
    AddVertex( &vert );

    vert.x += dir.x;
    vert.y += dir.y;
    vert.z += dir.z;
    AddVertex( &vert );

    bm.setNVert( nV + 2 );
    bm.setNPri( nP + 1 );
    return true;
} // VectorField::AddVector




