#include "stdafx.h"
#include "sg.h"
#include "sgSkybox.h"

IMPLEMENT_CLASS( Skybox );

/*****************************************************************************/
/*    Skybox implementation
/*****************************************************************************/
bool Skybox::s_bFrozen = false;

Skybox::Skybox()
{
    topColor = bottomColor = 0xFFFFFFFF;
    bNeedReassignColors = false;
}

void Skybox::Render()
{
    if (s_bFrozen) return;

    if (bNeedReassignColors) ReassignColors();

    Matrix4D skyTM( m_LocalTM );
    
    //  place skybox center into camera position
    ICamera* pCam = GetCamera();
    if (pCam)
    {
        Vector3D pos = pCam->GetPosition();
        skyTM.setTranslation( pos );
    }

    s_TMStack.Push( skyTM );
    m_WorldTM = s_TMStack.Top();

    SNode::Render();
    s_TMStack.Pop();

} // Skybox::Render

void Skybox::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << topColor << bottomColor;
} // Skybox::Serialize

void Skybox::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
    is >> topColor >> bottomColor;
    bNeedReassignColors = true;
} // Skybox::Unserialize

void Skybox::ReassignColors()
{
    /*SNode::Iterator it( this, Geometry::FnFilter );
    while (it)
    {
        SNode* pNode = (SNode*)it;
        Geometry* pGeom = (Geometry*)pNode;
        BaseMesh& bm = pGeom->GetMesh();
        AABoundBox aabb;
        bm.GetAABB( aabb );
        float zMin = aabb.minv.z;
        float zMax = aabb.maxv.z;

        VertexIterator vit;
        vit << bm;
        
        while (vit)
        {
            Vector3D& vec = vit;
            float grad = (vec.z - zMin)/(zMax - zMin);
            vit.diffuse() = ColorValue::Gradient( bottomColor, topColor, grad );
            ++vit;
        }

        ++it;    
    }*/

    bNeedReassignColors = false;
} // Skybox::ReassignColors


