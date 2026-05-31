/*****************************************************************************/
/*    File:    sgShadow.cpp
/*    Desc:    Shadow mapping vodoo
/*    Author:    Ruslan Shestopalyuk
/*    Date:    08-28-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include "sgShadow.h"

#include "IMediaManager.h"
#include "mHeightmap.h"
#include "uiControl.h"
#include "ITerrain.h"
#include "IShadowManager.h"

IMPLEMENT_CLASS( ShadowMapper            );
IMPLEMENT_CLASS( BlobShadowMapper        );
IMPLEMENT_CLASS( DBlobShadowMapper        );
IMPLEMENT_CLASS( ProjectiveShadowMapper );
IMPLEMENT_CLASS( ShadowVolumeMapper        );
IMPLEMENT_CLASS( IDShadowMapper            );
IMPLEMENT_CLASS( ShadowBlob                );
IMPLEMENT_CLASS( ProjectiveBlob            );

/*****************************************************************************/
/*    ShadowMapper implementation
/*****************************************************************************/
ShadowMapper::ShadowMapper()
{
    m_pLightSource = NULL;
    m_pLightSource = Root::instance()->FindChild<DirectionalLight>( "GameLight" );
    AddInput( m_pLightSource );
}

void ShadowMapper::AddCaster( DWORD nodeID, const Matrix4D& tm )
{
    ShadowCaster caster( nodeID, tm );
    Node* pNode = NodePool::instance().GetNode( nodeID );
    if (!pNode) return;
    caster.m_AABB = CalculateAABB( pNode );
    caster.m_AABB.Transform( tm );

    m_Casters.push_back( caster );
} // ShadowMapper::AddCaster

/*****************************************************************************/
/*    BlobShadowMapper implementation
/*****************************************************************************/
BlobShadowMapper::BlobShadowMapper()
{
    m_pTextureMatrix = NULL;

    m_BlobWidth            = 120.0f;
    m_BlobHeight        = 50.0f;
    m_BlobType            = 0;
    m_BlobUV.Set( 0.0f, 0.0f, 1.0f, 1.0f );
    m_ModelPivotHeight    = 20.0f;
    m_BlobColor            = 0xFF000000;
    m_Pivot.zero();
}

void BlobShadowMapper::Render()
{    
    //if (!m_pShadowCaster || !m_pLightCamera) return;

    //OrthoCamera* pCam = (OrthoCamera*) m_pLightCamera;
    //pCam->SetW( m_BlobWidth, m_BlobWidth / m_BlobHeight, 1.0f, 10000.0f );
    //pCam->SetPos( m_Pivot  );
    //
    //BaseCamera* pCurCamera = BaseCamera::GetActiveCamera();
    //if (!pCurCamera) return;
    //
    //Matrix4D texTM;

    ////  light camera projection space to texture space matrix
    //Matrix4D proj2tex;
    //proj2tex.st( Vector3D( 0.5f, -0.5f, 1.0f ), Vector3D( 0.5f, 0.5f, 0.0f ) );
    ////  current camera view space to world space
    //const Matrix4D& cam2world = pCurCamera->GetTransform();
    //
    ////  world space to light camera projection space
    //Matrix4D world2proj = pCam->ToSpace( sWorld, sProjection, );

    //texTM = proj2tex;
    //texTM.mulLeft( world2proj );
    //texTM.mulLeft( cam2world );

    //m_pTextureMatrix->SetTransform( texTM );
    //IRS->SetTextureFactor( m_BlobColor );

    //m_pTextureMatrix->Render();
    //m_pBlobTexture->Render();
    //Texture::Freeze();
    //m_pDecalZBias->Render();
    ////m_pBlobShadowShader->Render();
    //Texture::Unfreeze();
} // BlobShadowMapper::Render

void BlobShadowMapper::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "BlobShadowMapper", this );
    pm.f( "BlobWidth", m_BlobWidth );
    pm.f( "BlobHeight", m_BlobHeight );
    pm.f( "ModelPivotHeight", m_ModelPivotHeight );
    pm.f( "BlobColor", m_BlobColor, "color" );
    pm.f( "Pivot", m_Pivot, "direction" );
}

void BlobShadowMapper::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
}

void BlobShadowMapper::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
}

void BlobShadowMapper::Create()
{
    //ShadowMapper::Create();
    //m_pBlobTexture        = AddChild<Texture>( "blob_round.tga" );
    //m_pLightCamera        = AddChild<OrthoCamera>( "LightCamera" );
    //m_pDecalZBias        = AddChild<ZBias>( "DecalZBias" );
    //m_pDecalZBias->SetBias( 0.00005f );
    //m_pBlobShadowShader    = m_pDecalZBias->AddChild<Shader>( "blobShadow" );
    //m_pBlobShadowShader->AddInput( m_pShadowReceiver );

    //m_pTextureMatrix    = AddChild<TextureMatrix>( "TexCoorTM" );

    //m_pLightCamera->SetTransform( m_pLightSource->GetTransform() );

} // BlobShadowMapper::Create            

/*****************************************************************************/
/*    DBlobShadowMapper implementation
/*****************************************************************************/
DBlobShadowMapper::DBlobShadowMapper()
{
    m_pQuadMesh        = NULL;
}

void DBlobShadowMapper::Create()
{
    BlobShadowMapper::Create();
    m_pQuadMesh = AddChild<Geometry>( "QuadMesh" );
    m_pQuadMesh->Create( 4, 0, vfVertex2t );
    m_pQuadMesh->GetMesh().setIsQuadList( true );

    if (m_pBlobShadowShader)
    {
        m_pBlobShadowShader->ReleaseChildren();
        m_pBlobShadowShader->AddInput( m_pQuadMesh );
    }
} // DBlobShadowMapper::Create

void DBlobShadowMapper::Render()
{
    Parent::Render();
} // DBlobShadowMapper::Render

void DBlobShadowMapper::SetBlobQuad( const Vector3D& a,
                                     const Vector3D& b,
                                     const Vector3D& c,
                                     const Vector3D& d )
{
    m_pQuadMesh->GetMesh().setNVert( 0 );
    m_pQuadMesh->GetMesh().setNPri( 0 );
    m_pQuadMesh->AddQuad( a, b, c, d, &m_BlobUV );
} // DBlobShadowMapper::SetBlobQuad

/*****************************************************************************/
/*    ProjectiveShadowMapper implementation
/*****************************************************************************/
ProjectiveShadowMapper::ProjectiveShadowMapper()
{
}

void ProjectiveShadowMapper::Render()
{
    m_pLightSource = (LightSource*)GetInput( 0 );
    if (!m_pLightSource->IsA<LightSource>()) { m_pLightSource = NULL; return; }

    int nObj = m_Casters.size();
    for (int i = 0; i < nObj; i++)
    {
        const ShadowCaster& inst = m_Casters[i];
        IMM->Render( inst.m_ModelID, &inst.m_TM );
    }

    if (m_pLightSource)
    {
        for (int i = 0; i < nObj; i++)
        {
            const ShadowCaster& inst = m_Casters[i];
            DrawAABB( inst.m_AABB, 0, ColorValue::Red );
            Frustum frustum = m_pLightSource->GetFrustum( inst.m_AABB );
            DrawFrustum( frustum, 0, ColorValue::Yellow, true );
        }
        IRS->ResetWorldTM();
        rsFlushLines3D();
    }

    m_Casters.clear();
} // ProjectiveShadowMapper::Render

void ProjectiveShadowMapper::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "ProjectiveShadowMapper", this );
}

void ProjectiveShadowMapper::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
}

void ProjectiveShadowMapper::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
}

/*****************************************************************************/
/*    ShadowVolumeMapper implementation
/*****************************************************************************/
ShadowVolumeMapper::ShadowVolumeMapper()
{

}

void ShadowVolumeMapper::Render()
{

}

void ShadowVolumeMapper::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "ShadowVolumeMapper", this );
}

void ShadowVolumeMapper::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
}

void ShadowVolumeMapper::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
}
/*****************************************************************************/
/*    IDShadowMapper implementation
/*****************************************************************************/
IDShadowMapper::IDShadowMapper()
{

}

void IDShadowMapper::Render()
{

}

void IDShadowMapper::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "IDShadowMapper", this );
}

void IDShadowMapper::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
}

void IDShadowMapper::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
}

/*****************************************************************************/
/*    ShadowBlob implementation
/*****************************************************************************/
bool ShadowBlob::s_bFrozen = false;
ShadowBlob::ShadowBlob()
{
    m_Height        = 100.0f;
    m_Width            = 100.0f;
    m_Color            = 0xFFFFFFFF;
    m_NSegments     = 2;

    m_ShiftCenterX     = 0;
    m_ShiftCenterY     = 0;
} // ShadowBlob::ShadowBlob    

void ShadowBlob::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_Height << m_Width << m_Color << m_NSegments << m_ShiftCenterX << m_ShiftCenterY;
}

void ShadowBlob::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
    is >> m_Height >> m_Width >> m_Color >> m_NSegments >> m_ShiftCenterX >> m_ShiftCenterY;
}

void ShadowBlob::Render()
{
    if (IShadowMgr->GetShadowQuality() != sqBlobs) return;
    if (s_bFrozen) return;
    IRS->SetTextureFactor    ( m_Color );
    Node::Render            ();
    RenderMorphedGeometry    ();
} // ShadowBlob::Render

void ShadowBlob::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "ShadowBlob", this );
    pm.p( "BlobWidth",        GetBlobWidth,    SetBlobWidth );
    pm.p( "BlobHeight",        GetBlobHeight,    SetBlobHeight );
    pm.f( "BlobColor",        m_Color, "color"     );
    pm.p( "ShiftCenterX",    GetShiftX, SetShiftX );
    pm.p( "ShiftCenterY",    GetShiftY, SetShiftY );
    pm.p( "NumSegments",    GetNSegments, SetNSegments );
} // ShadowBlob::Expose

void ShadowBlob::OnProcessGeometry()
{
    const Matrix4D& tm = TransformNode::TMStackTop();
    Vertex2t* sV = (Vertex2t*)m_BaseMesh.getVertexData();
    Vertex2t* dV = (Vertex2t*)GetMesh().getVertexData();
    int nV = GetMesh().getNVert();

    if (ITerra)
    {
        for (int i = 0; i < nV; i++)
        {
            Vector3D v = sV[i];
            tm.transformPt( v );
            v.z = ITerra->GetH( v.x, v.y );
            dV[i].diffuse = m_Color;
            dV[i] = v;
        }
    }
    else
    {
        for (int i = 0; i < nV; i++)
        {
            Vector3D v = sV[i];
            tm.transformPt( v );
            v.z = 0.0f;
            dV[i].diffuse = m_Color;
            dV[i] = v;
        }
    }
} // ShadowBlob::OnProcessGeometry

void ShadowBlob::OnChangeStructure()
{
    Rct ext( -m_Width *0.5f + m_ShiftCenterX, -m_Height*0.5f + m_ShiftCenterY, m_Width, m_Height );
    CreatePatchGrid<Vertex2t>( GetMesh(), ext, m_NSegments, m_NSegments );
    ReplicateMesh();
} // ShadowBlob::OnChangeStructure

/*****************************************************************************/
/*    ProjectiveBlob implementation
/*****************************************************************************/
ProjectiveBlob::ProjectiveBlob()
{
    DirectionalLight* pLight = Root::instance()->FindChild<DirectionalLight>( "GameLight" );
    if (pLight)
    {
        m_LightDir = pLight->GetDir();
    }
    else
    {
        m_LightDir.set(    -sin( DegToRad( 30 ) ), -cos( DegToRad( 26 ) ), -sin( DegToRad( 90 - 26 ) ) );
        m_LightDir.normalize();
    }

    Vector3D up( Vector3D::oZ );
    Vector3D right;
    Vector3D dir( m_LightDir );
    dir.reverse();
    dir.z = 0.0f;
    right.cross( dir, up );
    dir.orthonormalize( up, right );

    m_PatchRotTM.SetRows( right, dir, up );

    m_pTarget        = NULL;
    m_pTexture        = NULL;
    m_pReceiverDSS    = NULL;
    m_pShadowMapDSS = NULL;

    m_ShadowMapSide = 128;
}

const float c_ZNear = -1000.0f;
const float c_ZFar = 1000.0f;
void ProjectiveBlob::Render()
{
    if (s_bFrozen) return;

    if (!m_pTarget)            m_pTarget        = FindChild<RenderTarget>    ( "ShadowTarget"        );
    if (!m_pShadowMapDSS)    m_pShadowMapDSS    = m_pTarget->FindChild<Shader>    ( "ShadowMap"            );
    if (!m_pTexture)        m_pTexture        = FindChild<Texture>        ( "ShadowMap"            );
    if (!m_pReceiverDSS)    m_pReceiverDSS    = FindChild<Shader>    ( "ShadowMapReceiver"    );

    if (!m_pTarget || !m_pTexture) return;
    m_pTarget->SetTarget( m_pTexture );
    
    Matrix4D objTM = TransformNode::TMStackTop();

    m_LightCamera.SetPosition( objTM.getTranslation() );
    m_LightCamera.SetOrientation( Vector3D::oX, Vector3D::oZ, m_LightDir );
    float objScale = objTM.GetXRow().norm();
    m_LightCamera.SetViewVolume( objScale * m_Width, c_ZNear, c_ZFar, objScale * m_Width / m_Height );
    ICamera* pCurCamera = GetCamera();

    Matrix4D vTM;
    vTM.translation( m_ShiftCenterX, m_ShiftCenterY, 0.0f );
    vTM.mulLeft( m_LightCamera.GetViewTM() );
    IRS->SetViewTM( vTM );
    IRS->SetProjTM( m_LightCamera.GetProjTM() );

    ShadowBlob::Freeze();
    m_pTarget->Render();
    ShadowBlob::Unfreeze();

    if (pCurCamera) pCurCamera->Render();
    
    m_pTexture->Render();
    m_pReceiverDSS->Render();

    //  find where light ray coming from object center intersects receiver geometry
    if (fabs( m_LightDir.z ) < c_SmallEpsilon) return;
    float t = - objTM.e32 / m_LightDir.z;

    Vector3D cPatch( objTM.e30 + m_LightDir.x * t, objTM.e31 + m_LightDir.y * t, 0 ); 
    Matrix4D wtm( Vector3D( objScale, objScale, objScale ), m_PatchRotTM, cPatch );
    TransformNode::ResetTMStack( &wtm );
    RenderMorphedGeometry();
    TransformNode::ResetTMStack();
} // ProjectiveBlob::Render

void ProjectiveBlob::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "ProjectiveBlob", this );
    pm.f( "ShadowMapSide", m_ShadowMapSide );
}

void ProjectiveBlob::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
}

void ProjectiveBlob::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
}

void ProjectiveBlob::OnChangeStructure()
{
    ShadowBlob::OnChangeStructure();

    m_pTarget = GetChild<RenderTarget>( "ShadowTarget" );
    m_pTarget->SetClearColor( 0xFF000000 );
    m_pTarget->EnableClear();

    m_pShadowMapDSS    = m_pTarget->GetChild<Shader>( "ShadowMap" );
    m_pReceiverDSS = GetChild<Shader>( "ShadowMapReceiver" );

    if (!m_pTexture) m_pTexture = FindChild<Texture>( "ShadowMap" );
    if (!m_pTexture)
    {
        m_pTexture = AddChild<Texture>( "ShadowMap" );
        m_pTexture->SetUsage        ( tuRenderTarget         );
        m_pTexture->SetMemoryPool    ( tmpDefault             );
        m_pTexture->SetWidth        ( m_ShadowMapSide         );
        m_pTexture->SetHeight        ( m_ShadowMapSide         );
        m_pTexture->SetNMips        ( 1                         );
        m_pTexture->SetColorFormat    ( cfRGB565                 );
        m_pTexture->CreateTexture    ();
        m_pTarget->SetTarget        ( m_pTexture );    
    }

    ReplicateMesh();
} // ProjectiveBlob::OnChangeStructure


