#include "stdafx.h"
#include "sg.h"
#include "sgNodePool.h"
#include "sgNode.h"
#include "sgTransformNode.h"
#include "sgShader.h"
#include "sgTexture.h"
#include "sgRoot.h"
#include "sgGeometry.h"
#include "kIOHelpers.h"
#include "kMathTypeTraits.h"

#include "vCamera.h"

#include "sgLensFlare.h"
#include "sgDummy.h"
#include "sgSkybox.h"

IMPLEMENT_CLASS( LensFlare );
IMPLEMENT_CLASS( LensFlareElement );

/*****************************************************************************/
/*    LensFlareElement implementation
/*****************************************************************************/
BaseMesh LensFlareElement::s_QuadMesh;

LensFlareElement::LensFlareElement()
{
    color        = 0xFFFFFFFF;
    radius       = 1.0f;
    position     = 0.0f;
}

void LensFlareElement::Render()
{
    if (s_QuadMesh.getNVert() == 0)
    {
        s_QuadMesh.create( 4, 0, vfVertexTnL, ptTriangleList );
        s_QuadMesh.setIsQuadList( true );
        s_QuadMesh.setNVert( 4 );
        s_QuadMesh.setNPri ( 2 );
    }
    
    VertexTnL* v = (VertexTnL*) s_QuadMesh.getVertexData();
    
    float hside = quadSide * 0.5f;

    v[0].x = screenPos.x - hside;
    v[0].y = screenPos.y - hside;
    v[0].z = 0.0f;
    v[0].w = 1.0f;
    v[0].diffuse = quadColor;
    v[0].u = 0.0f;
    v[0].v = 0.0f;
    
    v[1].x = screenPos.x + hside;
    v[1].y = screenPos.y - hside;
    v[1].z = 0.0f;
    v[1].w = 1.0f;
    v[1].diffuse = quadColor;
    v[1].u = 1.0f;
    v[1].v = 0.0f;
    
    v[2].x = screenPos.x - hside;
    v[2].y = screenPos.y + hside;
    v[2].z = 0.0f;
    v[2].w = 1.0f;
    v[2].diffuse = quadColor;
    v[2].u = 0.0f;
    v[2].v = 1.0f;
    
    v[3].x = screenPos.x + hside;
    v[3].y = screenPos.y + hside;
    v[3].z = 0.0f;
    v[3].w = 1.0f;
    v[3].diffuse = quadColor;
    v[3].u = 1.0f;
    v[3].v = 1.0f;

    DrawPrimBM( s_QuadMesh );
} // LensFlareElement::Render

void LensFlareElement::Expose( PropertyMap&  pm )
{
    pm.start<Parent>( "LensFlareElement", this );
    pm.f( "Color",    color,        "color" );
    pm.f( "Radius",    radius                );
    pm.f( "Position", position            );

} // LensFlareElement::Expose

void LensFlareElement::Serialize( OutStream& os    ) const
{
    Parent::Serialize( os );
    os << color << radius << position;
} // LensFlareElement::Serialize

void LensFlareElement::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
    is >> color >> radius >> position;
} // LensFlareElement::Unserialize

/*****************************************************************************/
/*    LensFlare implementation
/*****************************************************************************/
bool    LensFlare::s_bFrozen = false;

LensFlare::LensFlare()
{
    m_SunDistance    = 100000.0f;
    m_SunRadius      = 0.3f;
    m_Scale          = 128.0f;

    m_pSunCamera     = NULL;
    m_pScene         = NULL;

    m_SunDir.set( 1.0f, 1.0f, 0.1f );
} // LensFlare::LensFlare

void LensFlare::AddElement( const char* texName, float pos, float radius, DWORD color )
{
    AddChild<Texture>( texName );
    char lfName[256];
    sprintf( lfName, "%s_%d", GetName(), GetNChildren() );
    LensFlareElement* pEl = AddChild<LensFlareElement>( lfName );
    pEl->SetPosition    ( pos );
    pEl->SetRadius      ( radius );
    pEl->SetColor       ( color );
} // LensFlare::AddElement

void LensFlare::AddElement( const char* texName, 
                            float pos, 
                            float radius )
{
    AddElement( texName, pos, radius, GetRandomFlareColor() );
} // LensFlare::AddElement

DWORD LensFlare::GetRandomFlareColor() const
{
    float r = rndValuef( c_FlareMinR, c_FlareMaxR );
    float g = rndValuef( c_FlareMinR, c_FlareMaxR );
    float b = rndValuef( c_FlareMinR, c_FlareMaxR );
    
    DWORD clr = 0xFF000000 | (DWORD(r*255.0f) << 16) | 
                    (DWORD(g*255.0f) << 8) | DWORD(b*255.0f);

    return clr;
} // LensFlare::GetRandomFlareColor

void LensFlare::Render()
{
    if (s_bFrozen) return;

    ICamera* pCam = GetCamera();
    if (!pCam) return;

    Rct vp = IRS->GetViewPort();
    //  find where sun is in the screen space    
    Vector3D viewDir;
    Vector3D sunDir( m_SunDir );
    viewDir = pCam->GetDir();
    sunDir.normalize();
    viewDir.normalize();

    float fIntensity = sunDir.dot( viewDir );

    //  clip sun against view direction 
    if (fIntensity < 0.00001f) return;

    ////  setup sun camera
    //if (!m_pSunCamera)
    //{
    //    m_pSunCamera = FindChild<PerspCamera>( "SunCamera" );
    //}

    //if (!m_pScene)
    //{
    //    m_pScene = Root::instance()->FindChild<Group>( "Scene" );
    //    Shader* pDSS = GetChild<Shader>( "sunmap" );
    //    pDSS->ReleaseChildren();
    //    pDSS->AddInput( m_pScene );
    //}

    //if (m_pSunCamera)
    //{
    //    m_pSunCamera->SetPos( pCam->GetPos() );
    //    m_pSunCamera->SetDirUp( sunDir, Vector3D::oZ );
    //    m_pSunCamera->SetPerspWH( m_SunRadius, m_SunRadius, pCam->GetZn(), pCam->GetZf() );
    //}

    Vector3D camPos = pCam->GetPosition();
    Vector3D fc( camPos ); 
    fc.addWeighted( viewDir, m_SunDistance );
    Vector3D sunPos( camPos ); 
    sunPos.addWeighted( sunDir, m_SunDistance/* / fIntensity*/ );
    
    Vector4D vc( fc ); vc.w = 1.0f;
    Vector4D vf( sunPos ); vf.w = 1.0f;

    pCam->ToSpace( sWorld, sScreen,  vc );
    pCam->ToSpace( sWorld, sScreen,  vf );
    

    float side = m_Scale;
    Rct rctSun( vf.x, vf.y, side );
    if (!vp.Overlap( rctSun )) return;

    //  screen center
    float cx = vp.x + vp.w * 0.5f;
    float cy = vp.y + vp.h * 0.5f;
    Vector2D dir( cx - vf.x, cy - vf.y );
    
    float fAlphaMul = 1.0f - 0.8f * dir.norm() / vp.h;
    if (fAlphaMul < 0.0f) fAlphaMul = 0.0f;
    if (fAlphaMul > 1.0f) fAlphaMul = 1.0f;


    for (int i = 0; i < GetNChildren(); i++)
    {
        SNode* pNode = GetChild( i );
        if (!pNode->IsA<LensFlareElement>()) continue;
        LensFlareElement* pEl = (LensFlareElement*)pNode;
        pEl->screenPos.set( cx + pEl->GetPosition()*dir.x*fIntensity, 
                            cy + pEl->GetPosition()*dir.y*fIntensity, 0.0f  );
        pEl->quadSide  = m_Scale * pEl->GetRadius();

        float alpha = fAlphaMul * pEl->GetAlpha();
        pEl->quadColor = (DWORD( alpha*255.0f ) << 24) | (pEl->GetColor() & 0x00FFFFFF);
    }
    
    LensFlare::Freeze();
    if (m_pSunCamera) m_pSunCamera->Render();
    SNode::Render();    
    LensFlare::Unfreeze();

} // LensFlare::Render

void LensFlare::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "LensFlare", this );
    pm.f( "SunDirX",        m_SunDir.x      );
    pm.f( "SunDirY",        m_SunDir.y      );
    pm.f( "SunDirZ",        m_SunDir.z      );

    pm.f( "Sun Distance",   m_SunDistance   );
    pm.f( "Sun Radius",     m_SunRadius     );
    pm.f( "Scale",          m_Scale         );
    pm.m( "CreateSample", &LensFlare::CreateSample    );
} // LensFlare::Expose

void LensFlare::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_SunDir << m_SunDistance << m_Scale << m_SunRadius;
}

void LensFlare::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
    is >> m_SunDir >> m_SunDistance >> m_Scale >> m_SunRadius;
}

void LensFlare::Create( bool bOccluded )
{
    if (!bOccluded)
    {
        AddChild<Shader>( "LensFlare" );
        return;
    }

    //  create all the thingies we need for drawing occluded lens flare
    
    //  sunmap render target
    RenderTarget* pRT = AddChild<RenderTarget>( "SunMapTarget" );
    
    //  sunmap texture
    Texture* pSunMap = pRT->AddChild<Texture>( "SunMap" );
    pSunMap->SetUsage        ( tuRenderTarget );
    pSunMap->SetMemoryPool   ( tmpDefault     );
    pSunMap->SetWidth        ( c_SunMapSide   );
    pSunMap->SetHeight       ( c_SunMapSide   );
    pSunMap->SetNMips        ( 1              );
    pSunMap->SetColorFormat  ( cfRGB565       );
    pSunMap->CreateTexture   ();
    pRT->SetTarget           ( pSunMap );

    //  sunmap viewport 
    Background* pVP = pRT->AddChild<Background>( "SunMapVP" );
    pVP->SetClearColor( true );
    pVP->SetBgColor( 0xFFFFFFFF );
    pVP->SetExtents( 0, 0, c_SunMapSide, c_SunMapSide );

    //  intensity map render target
    RenderTarget* pIRT = AddChild<RenderTarget>( "IntMapTarget" );
    //Group* pIRT = AddChild<Group>( "IntMapTarget" );

    //  intensity map texture    
    Texture* pIntMap = pIRT->AddChild<Texture>( "IntMap" );
    pIntMap->SetUsage        ( tuRenderTarget         );
    pIntMap->SetMemoryPool   ( tmpDefault             );
    pIntMap->SetWidth        ( c_IntensityMapSide     );
    pIntMap->SetHeight       ( c_IntensityMapSide     );
    pIntMap->SetNMips        ( 1                      );
    pIntMap->SetColorFormat  ( cfARGB8888             );
    pIntMap->SetStage        ( 1                      );
    pIntMap->CreateTexture   ();
    pIRT->SetTarget          ( pIntMap );

    pIRT->AddInput( pSunMap );
    pIRT->AddChild<Shader>( "FlareIntensityMap" );

    AddInput( pIntMap );

    //  intensity map viewport 
    Background* pIVP = pIRT->AddChild<Background>( "IntMapVP" );
    pIVP->SetClearColor( true );
    pIVP->SetBgColor( 0x00000000 );
    pIVP->SetExtents( 0, 0, c_IntensityMapSide, c_IntensityMapSide );

    //  intensity map pixel grid mesh
    Geometry* pGeom = pIRT->AddChild<Geometry>( "PixelGrid" );
    pGeom->Create( c_SunMapSide * c_SunMapSide * 4, 0, vfVertexTnL );
    BaseMesh& bm = pGeom->GetMesh();
    bm.setIsQuadList( true );
    bm.setIsStatic  ( true );
    VertexTnL* v = (VertexTnL*)bm.getVertexData();
    int cV = 0;
    float uvStep = 1.0f / c_SunMapSide;
    for (float i = 0; i < 1.0f; i += uvStep)
    {
        for (float j = 0; j < 1.0f; j += uvStep)
        {
            v[cV].x = 0.0f;
            v[cV].y = 0.0f;
            v[cV].z = 0.0f;
            v[cV].w = 1.0f;
            v[cV].u = i;
            v[cV].v = j;
            cV++;            
            
            v[cV].x = c_IntensityMapSide;
            v[cV].y = 0.0f;
            v[cV].z = 0.0f;
            v[cV].w = 1.0f;
            v[cV].u = i + uvStep;
            v[cV].v = j;
            cV++;

            v[cV].x = 0.0f;
            v[cV].y = c_IntensityMapSide;
            v[cV].z = 0.0f;
            v[cV].w = 1.0f;
            v[cV].u = i;
            v[cV].v = j + uvStep; 

            cV++;

            v[cV].x = c_IntensityMapSide;
            v[cV].y = c_IntensityMapSide;
            v[cV].z = 0.0f;
            v[cV].w = 1.0f;
            v[cV].u = i + uvStep;
            v[cV].v = j + uvStep;
            cV++;
        }
    }
    bm.setNVert( cV );
    bm.setNPri( cV / 2 );


    ////  sun camera
    //m_pSunCamera = pRT->AddChild<PerspCamera>( "SunCamera" );

    //
    ////  scene to be rendered
    //Shader* pDSS = pRT->AddChild<Shader>( "sunmap" );
    //
    //m_pScene = Root::instance()->FindChild<Group>( "Scene" );
    //pDSS->AddInput( m_pScene );

    //AddChild<Shader>( "OccludedLensFlare" );
} // LensFlare::Create

void LensFlare::CreateSample()
{
    Create( false );
    SetSunDir( Vector3D( 1.0f, -1.0f, 0.4f ) );
    SetSunDistance( 100000.0f );
    AddElement( "flares\\flare3.dds", 1.3f,  0.60f );
    AddElement( "flares\\flare2.dds", 1.0f,  0.50f );
    AddElement( "flares\\flare2.dds", 0.5f,  0.4f  );
    AddElement( "flares\\flare1.dds", 0.0f,  0.25f );
    AddElement( "flares\\flare1.dds", -0.3f, 0.4f  );
    AddElement( "flares\\flare3.dds", -0.6f, 0.5f  );
    AddElement( "flares\\flare5.dds", -1.0f, 2.0f, 0xFFFFFFFF );
} // LensFlare::CreateTestLensFlare



