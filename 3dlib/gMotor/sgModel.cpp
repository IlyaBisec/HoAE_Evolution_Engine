/*****************************************************************************/
/*    File:    sgModel.cpp
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    15.04.2003
/*****************************************************************************/
#include "stdafx.h"
#include "sgNodePool.h"
#include "sgNode.h"
#include "sgTransformNode.h"
#include "sgDummy.h"
#include "sgModel.h"
#include "sgGeometry.h"
#include "kFilePath.h"
#include "vSkin.h"

IMPLEMENT_CLASS(Model);

/*****************************************************************************/
/*    Model implementation
/*****************************************************************************/
void Model::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Version << m_AABB;
}

void Model::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
    is >> m_Version >> m_AABB;
}

void  PrefetchTextures( SNode* pRoot, const FilePath& path );
SNode* CompileModel( SNode* pRoot, const char* path );

bool Model::Reload()
{
    int resID = IRM->FindResource( m_FileName.c_str() );
    if (resID == -1) return false;
    InStream& is = IRM->LockResource( m_FileName.c_str() );

    FilePath path( IRM->GetPath( resID ) );
    _chdir( path.GetDrive() );
    _chdir( path.GetDir() );

    SNode* pModel = SNode::UnserializeSubtree( is );            
    is.Close();
    _chdir( IRM->GetHomeDirectory() );

    if (!pModel) return c_BadID;

    PrefetchTextures( pModel, path );
    pModel = CompileModel( (SNode*)pModel, path.GetFullPath() );

    if (pModel->IsA<Model>()) 
    {
        ReleaseChildren();
        for (int i = 0; i < pModel->GetNChildren(); i++)
        {
            AddChild( pModel->GetChild( i ) );
        }
        pModel->ClearChildren();
        pModel->Release();
    }
    else
    {
        AddChild( pModel );
    }

    m_bLoaded = true;
    
    _chdir( IRM->GetHomeDirectory() );
    return true;
} // Model::Reload

bool Model::Load()
{
    if (m_bLoaded) return true;
	m_bLoaded=true;
    return Reload();
} // Model::Load

bool Model::Dispose()
{
    if (GetNChildren() == 0) return false;
    ReleaseChildren();
    return true;
} // Model::Dispose

void Model::Render()
{
    SNode::Render();
#ifdef _DEBUG
    //rsFlush();
    //AABoundBox aabb = m_AABB;
    //aabb.Transform( TransformNode::TMStackTop() );
    //DrawAABB( aabb, 0x33FF3333, 0xFFFF3333 );
    //rsFlush();
#endif // _DEBUG
} // Model::Render

void Model::CreateShell()
{
    m_Shell.clear();
    const int c_XSegments = 3;
    const int c_YSegments = 2;
    const int c_ZSegments = 3;
    

    float cx = m_AABB.GetDX()/c_XSegments;
    float cy = m_AABB.GetDY()/c_YSegments;
    float cz = m_AABB.GetDZ()/c_ZSegments;
    
    for (int nx = 0; nx < c_XSegments; nx++)
    {
        for (int ny = 0; ny < c_YSegments; ny++)
        {
            for (int nz = 0; nz < c_ZSegments; nz++)
            {
                AABoundBox cell(    m_AABB.minv.x + cx*nx, m_AABB.minv.y + cy*ny, m_AABB.minv.z + cz*nz, 
                                    m_AABB.minv.x + cx*(nx + 1), 
                                    m_AABB.minv.y + cy*(ny + 1), 
                                    m_AABB.minv.z + cz*(nz + 1) );
                AABoundBox box( FLT_MAX, FLT_MAX, FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX );
                SNode::Iterator it( this );
                while (it)
                {
                    Geometry* pGeom = dynamic_cast<Geometry*>( *it );
                    if (pGeom->IsA<Skin>()) 
                    {
                        m_Shell.clear();
                        return;
                    }
                    ++it;
                    if (!pGeom) continue;
                    Matrix4D tm = GetWorldTM( pGeom );
                    BaseMesh& bm = pGeom->GetMesh();
                    VertexIterator v( bm.getVertexData(), bm.getNVert(), 
                                        CreateVertexDeclaration( bm.getVertexFormat() ) );
                    int nTri = bm.getNPri();
                    WORD* idx = bm.getIndices();
                    for (int i = 0; i < nTri; i++)
                    {
                        Vector3D a = v.pos( idx[i*3 + 0] );
                        Vector3D b = v.pos( idx[i*3 + 1] );
                        Vector3D c = v.pos( idx[i*3 + 2] );
                        tm.transformPt( a );
                        tm.transformPt( b );
                        tm.transformPt( c );

                        if ((cell.PtIn( a ) && cell.PtIn( b )) ||
                            (cell.PtIn( b ) && cell.PtIn( c )) ||
                            (cell.PtIn( c ) && cell.PtIn( a )))
                        {
                            if (a.x > box.maxv.x) box.maxv.x = a.x;
                            if (a.y > box.maxv.y) box.maxv.y = a.y;
                            if (a.z > box.maxv.z) box.maxv.z = a.z;
    
                            if (a.x < box.minv.x) box.minv.x = a.x;
                            if (a.y < box.minv.y) box.minv.y = a.y;
                            if (a.z < box.minv.z) box.minv.z = a.z;


                            if (b.x > box.maxv.x) box.maxv.x = b.x;
                            if (b.y > box.maxv.y) box.maxv.y = b.y;
                            if (b.z > box.maxv.z) box.maxv.z = b.z;

                            if (b.x < box.minv.x) box.minv.x = b.x;
                            if (b.y < box.minv.y) box.minv.y = b.y;
                            if (b.z < box.minv.z) box.minv.z = b.z;


                            if (c.x > box.maxv.x) box.maxv.x = c.x;
                            if (c.y > box.maxv.y) box.maxv.y = c.y;
                            if (c.z > box.maxv.z) box.maxv.z = c.z;

                            if (c.x < box.minv.x) box.minv.x = c.x;
                            if (c.y < box.minv.y) box.minv.y = c.y;
                            if (c.z < box.minv.z) box.minv.z = c.z;
                        }
                    }
                }
                if (box.GetDX() > c_SmallEpsilon && 
                    box.GetDY() > c_SmallEpsilon &&
                    box.GetDZ() > c_SmallEpsilon)
                {
                    m_Shell.push_back( box );
                }
            }
        }
    }
} // Model::CreateShell





