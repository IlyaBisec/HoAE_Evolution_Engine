/*****************************************************************************/
/*    File:    uiTransformTools.cpp
/*    Desc:    Object visual transformation tools
/*    Author:    Ruslan Shestopalyuk
/*    Date:    15.04.2003
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include "sgPRSAnimation.h"
#include "uiTransformTools.h"

IMPLEMENT_CLASS( SelectionTool  );
IMPLEMENT_CLASS( TransformTool  );
IMPLEMENT_CLASS( MoveTool       );

/*****************************************************************************/
/*    TransformTool implementation
/*****************************************************************************/
TransformTool::TransformTool() : m_Size(0.005f), m_pNode(NULL)
{
}

void TransformTool::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "TransformTool", this );
    pm.f( "Size", m_Size );
}

/*****************************************************************************/
/*    MoveTool implementation
/*****************************************************************************/
MoveTool::MoveTool() : m_Arrow(NULL), m_pShader(NULL), m_MoveDir(mdNone)
{
}

bool MoveTool::OnMouseMove( int mX, int mY, DWORD keys )    
{ 
    if (IsInvisible()) return false;

    if (m_bDragging && (!(keys & VK_LBUTTON) || GetKeyState( VK_MENU ) < 0)) m_bDragging = false;
    
    ICamera* pCam = GetCamera();
    if (!pCam || !m_pNode) return false;
    
    Matrix4D m = m_pNode->GetTransform();
    m *= pCam->ToSpace( sWorld, sScreen );
    m *= 1.0f / m.e33;
    
    if (m_bDragging)
    {
        Vector4D tr0( m_Mx, m_My,    m.e32, 1.0f );
        Vector4D tr1( mX,    mY,        m.e32, 1.0f );

        pCam->ToSpace( sScreen, sWorld,  tr0 );
        pCam->ToSpace( sScreen, sWorld,  tr1 );
        
        /*tr0 *= m_pNode->GetWorldTM();
        tr1 *= m_pNode->GetWorldTM();*/

        tr1 -= tr0;
        float trAmt = tr1.norm();

        Matrix4D zTM = GetZTM();
        Vector3D vx( zTM.e00, zTM.e10, zTM.e20 );
        Vector3D vy( zTM.e01, zTM.e11, zTM.e21 );
        Vector3D vz( zTM.e02, zTM.e12, zTM.e22 );

        Matrix4D trTM( Matrix4D::identity );
        if (m_MoveDir == mdX)
        {
            trTM.e30 = tr1.dot( vx );
        }
        if (m_MoveDir == mdY)
        {
            trTM.e31 = tr1.dot( vy );
        }
        if (m_MoveDir == mdZ)
        {
            trTM.e32 = tr1.dot( vz );
        }
        Matrix4D res( m_pNode->GetTransform() );
        res.mulLeft( trTM );
        m_pNode->SetTransform( res );
    }
    else
    {
        float mx = mX;
        float my = mY;
        m_MoveDir = mdNone;
        float minDot = 0.0f;
        mx -= m.e30;
        my -= m.e31;

        Vector2D dirX( m.e00, m.e01 );
        Vector2D dirY( m.e10, m.e11 );
        Vector2D dirZ( m.e20, m.e21 );
        Vector2D mpos( mx, my );

        dirX.normalize();
        dirY.normalize();
        dirZ.normalize();

        float xDot = mpos.dot( dirX );
        float yDot = mpos.dot( dirY );
        float zDot = mpos.dot( dirZ );

        if (xDot > minDot) { m_MoveDir = mdX; minDot = xDot; }
        if (yDot > minDot) { m_MoveDir = mdY; minDot = yDot; }
        if (zDot > minDot) { m_MoveDir = mdZ; minDot = zDot; }
    }

    m_Mx = mX;
    m_My = mY;

    return false; 
} // MoveTool::OnMouseMove

bool MoveTool::OnMouseLBDown( int mX, int mY )                
{ 
    if (GetKeyState( VK_MENU    ) < 0 || 
        GetKeyState( VK_CONTROL ) < 0 ||
        GetKeyState( VK_SHIFT   ) < 0) return false; 
    m_bDragging = true;
    m_Mx = mX;
    m_My = mY;
    return false; 
} // MoveTool::OnMouseLBDown

bool MoveTool::OnMouseLBUp( int mX, int mY )                
{ 
    m_bDragging = false;
    return false; 
}

Matrix4D MoveTool::GetXTM( float scale )
{
    Matrix4D res( Matrix4D::identity );
    
    if (m_pNode) 
    {
        Matrix4D scTM;
        scTM.scaling( scale );

        const Matrix4D& m = m_pNode->GetWorldTM();
        Vector3D vx( res.e00, res.e10, res.e20 );
        Vector3D vy( res.e01, res.e11, res.e21 );
        Vector3D vz( res.e02, res.e12, res.e22 );

        Vector3D::orthonormalize( vx, vy, vz );

        res.e00 = vz.x; res.e01 = vy.x; res.e02 = vx.x; res.e03 = m.e03;
        res.e10 = vz.y; res.e11 = vy.y; res.e12 = vx.y; res.e13 = m.e13;
        res.e20 = vz.z; res.e21 = vy.z; res.e22 = vx.z; res.e23 = m.e23;
        res.e30 = m.e30; res.e31 = m.e31; res.e32 = m.e32; res.e33 = m.e33;

        res.mulLeft( scTM );
    }

    return res;
} // MoveTool::GetXTM

Matrix4D MoveTool::GetYTM( float scale )
{
    Matrix4D res( Matrix4D::identity );

    if (m_pNode) 
    {
        Matrix4D scTM;
        scTM.scaling( scale );

        const Matrix4D& m = m_pNode->GetWorldTM();
        Vector3D vx( res.e00, res.e10, res.e20 );
        Vector3D vy( res.e01, res.e11, res.e21 );
        Vector3D vz( res.e02, res.e12, res.e22 );

        Vector3D::orthonormalize( vx, vy, vz );

        res.e00 = vx.x; res.e01 = vz.x; res.e02 = vy.x; res.e03 = m.e03;
        res.e10 = vx.y; res.e11 = vz.y; res.e12 = vy.y; res.e13 = m.e13;
        res.e20 = vx.z; res.e21 = vz.z; res.e22 = vy.z; res.e23 = m.e23;
        res.e30 = m.e30; res.e31 = m.e31; res.e32 = m.e32; res.e33 = m.e33;

        res.mulLeft( scTM );
    }
    return res;
} // MoveTool::GetYTM

Matrix4D MoveTool::GetZTM( float scale )
{
    Matrix4D res( Matrix4D::identity );

    if (m_pNode) 
    {
        Matrix4D scTM;
        scTM.scaling( scale );
        const Matrix4D& m = m_pNode->GetWorldTM();
        Vector3D vx( res.e00, res.e10, res.e20 );
        Vector3D vy( res.e01, res.e11, res.e21 );
        Vector3D vz( res.e02, res.e12, res.e22 );

        Vector3D::orthonormalize( vx, vy, vz );

        res.e00 = vx.x; res.e01 = vy.x; res.e02 = vz.x; res.e03 = m.e03;
        res.e10 = vx.y; res.e11 = vy.y; res.e12 = vz.y; res.e13 = m.e13;
        res.e20 = vx.z; res.e21 = vy.z; res.e22 = vz.z; res.e23 = m.e23;
        res.e30 = m.e30; res.e31 = m.e31; res.e32 = m.e32; res.e33 = m.e33;
        res.mulLeft( scTM );
    }
    return res;
} // MoveTool::GetZTM

void MoveTool::Render()
{
    if (!m_Arrow)
    {
        m_Arrow = (Geometry*)SNode::CreateFromFile( "Models\\arrow.c2m" );
        AddChild( m_Arrow );
        m_pShader = FindChild<Shader>( "transformtools" );
        if (!m_pShader) m_pShader = AddChild<Shader>( "transformtools" );
    }
    
    if (!m_Arrow || !m_pShader || !m_pNode) return;
    const Matrix4D& m = m_pNode->GetTransform();
    
    //  find scale value, to keep screen space size of the gizmo unchanged
    float sc = 1.0f;
    ICamera* pCam = GetCamera();
    if (pCam) 
    {
        Vector4D pos( m.e30, m.e31, m.e32, 1.0f );
        Vector4D vs( pos );
        pCam->ToSpace( sWorld, sProjection,  vs );
        vs.x += m_Size;
        pCam->ToSpace( sProjection, sWorld,  vs );
        vs -= pos;
        sc = vs.norm();
    }
    
    m_pShader->Render();
        
    IRS->SetWorldTM( GetXTM( sc ) );
    IRS->SetTextureFactor( m_MoveDir == mdX ? 0xFFFFFF00 : 0xFFFF0000 );
    m_Arrow->Render();

    IRS->SetWorldTM( GetYTM( sc ) );
    IRS->SetTextureFactor( m_MoveDir == mdY ? 0xFFFFFF00 : 0xFF00FF00 );
    m_Arrow->Render();
    
    IRS->SetWorldTM( GetZTM( sc ) );
    IRS->SetTextureFactor( m_MoveDir == mdZ ? 0xFFFFFF00 : 0xFF0000FF );
    m_Arrow->Render();

} // MoveTool::Render

/*****************************************************************************/
/*    SelectionTool implementation
/*****************************************************************************/
SelectionTool::SelectionTool()
{
    m_SelectionColor = 0xFFFF0080;
    m_pSelShader = AddChild<Shader>( "selection" );
    m_pSelZBias     = m_pSelShader->AddChild<ZBias>( "SelZBias" );
    m_pSelZBias->SetBias( 0.000001f );

    m_pSelection = AddChild<Group>( "Selection" );

    m_pSelZBias->AddInput( m_pSelection );

    m_pSceneRoot = Root::instance()->FindChild<Group>( "Scene" );
}

void SelectionTool::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "SelectionTool", this );
    pm.f( "SelectionColor", m_SelectionColor, "color" );
}

void SelectionTool::Render()
{
    IRS->SetTextureFactor( m_SelectionColor );
    IRS->ResetWorldTM();
    
    m_pSelShader->Render();
    PRSAnimation::Freeze();
    for (int i = 0; i < GetNSelectedNodes(); i++)
    {
        TransformNode* pNode = GetSelectedNode( i );
        IRS->SetWorldTM( pNode->GetParentWorldTM() );
        pNode->Render();
    }

    PRSAnimation::Unfreeze();
}

bool SelectionTool::OnMouseMove( int mX, int mY, DWORD keys )
{
    return false;
}

bool SelectionTool::OnMouseLBDown( int mX, int mY )
{
    return false;
}

bool SelectionTool::OnMouseLBUp( int mX, int mY )
{///return false;
    if (GetKeyState( VK_CONTROL ) < 0 ||
        GetKeyState( VK_MENU    ) < 0 ||
        GetKeyState( VK_SHIFT    ) < 0) return false; 
    
    //  get pick ray
    ICamera* pCam = GetCamera();
    if (!pCam || !m_pSceneRoot) return false;

    Ray3D ray = pCam->GetPickRay( mX, mY );

    Iterator it( m_pSceneRoot );
    while (it)
    {
        SNode* pNode = (SNode*)*it;
        
        if (pNode->HasFn( Geometry::Magic() ))
        {
            Matrix4D tm = Matrix4D::identity;
            SNode* pParent = pNode->GetParent();
            while (pParent && !pParent->HasFn( TransformNode::Magic() )) pParent = pParent->GetParent();
            if (!pParent) 
            {
                ++it;
                continue;
            }
            TransformNode* pSelNode = (TransformNode*)pParent;
            tm = pSelNode->GetWorldTM();
            Matrix4D invTM;
            invTM.inverse( tm );
            Ray3D tray( ray ); 
            tray.Transform( invTM );
            Geometry* pGeom = (Geometry*) pNode;
            int triIdx = -1;
            
            if (!pGeom->GetAABB().Overlap( tray ))
            {
                ++it;
                continue;
            }

            float dist = pGeom->GetMesh().PickPoly( tray, triIdx );
            if (triIdx >= 0)
            {
                if (IsNodeSelected( pSelNode ))
                {
                    UnselectNode( pSelNode );
                    return false;
                }
                SelectNode( pSelNode );
            }
        }
        ++it;
    }

    return false;
} // SelectionTool::OnMouseLBUp

void SelectionTool::SelectAll()
{
    SNode* pScene = Root::instance()->FindChild<Group>( "Scene" );
    if (!pScene) return;
    m_pSelection->ReleaseChildren();
    m_pSelection->AddInput( pScene );
} // SelectionTool::SelectAll

void SelectionTool::UnselectAll()
{
    m_SelNodes.clear();
    m_pSelection->ReleaseChildren();
}

bool SelectionTool::IsNodeSelected( TransformNode* pNode )
{
    for (int i = 0; i < m_SelNodes.size(); i++)
    {
        if (m_SelNodes[i] == pNode) return true;
    }
    return false;
}

void SelectionTool::UnselectNode( TransformNode* pNode )
{
    for (int i = 0; i < m_SelNodes.size(); i++)
    {
        if (m_SelNodes[i] == pNode) m_SelNodes.erase( i );
    }
} // SelectionTool::UnselectNode

void SelectionTool::SelectNode( TransformNode* pNode )
{
    m_SelNodes.push_back( pNode );
}

int SelectionTool::GetNSelectedNodes()
{
    return m_SelNodes.size();
}

TransformNode* SelectionTool::GetSelectedNode( int idx )
{
    if (idx < 0 || idx >= GetNSelectedNodes()) return NULL;
    return m_SelNodes[idx];
}



