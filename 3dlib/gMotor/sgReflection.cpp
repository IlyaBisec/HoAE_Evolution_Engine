/*****************************************************************************/
/*    File:    sgReflection.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    09-18-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include "sgNodePool.h"
#include "sgNode.h"
#include "sgTransformNode.h"
#include "sgShader.h"
#include "sgTexture.h"
#include "sgDummy.h"
#include "sgRoot.h"
#include "sgGeometry.h"
#include "sgLight.h"
#include "kIOHelpers.h"
#include "sgReflection.h"
#include "ITerrain.h"
#include "kTimer.h"

IMPLEMENT_CLASS( ReflectionMap );

/*****************************************************************************/
/*    ReflectionMap implementation
/*****************************************************************************/
ReflectionMap::ReflectionMap()
{
    m_BackdropGridNodes         = 8;
    m_ReflectionMapSide         = 512;
    m_bUseDepthBuffer           = false;
    m_ReflectionPlane.fromPointNormal( Vector3D( 0.0f, 0.0f, 0.0f), Vector3D::oZ );
    m_bRenderReflection         = false;
    m_bRenderBackdrop           = false;
    m_bInited                   = false;
    m_ReflID                    = -1;
    m_DepthID                   = -1;
    m_bDrawDebugInfo            = false;
    m_bReflectTerrain           = false;
} // ReflectionMap::ReflectionMap

void ReflectionMap::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
    os << m_BackdropGridNodes << m_bRenderReflection << m_bRenderBackdrop;
}

void ReflectionMap::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
    is >> m_BackdropGridNodes >> m_bRenderReflection >> m_bRenderBackdrop;
}

void ReflectionMap::Render()
{
    if (!m_bInited) Init();
    ICamera* pCam = GetCamera();
    if (!pCam) return;

    Rct vp = IRS->GetViewPort();
    if (!IRS->PushRenderTarget( m_ReflID, m_DepthID )) return;

    rsFlush();

    IRS->SetViewPort( Rct( 0, 0, m_ReflectionMapSide, m_ReflectionMapSide ) );
    Matrix4D viewTM = IRS->GetViewTM();
    Matrix4D reflectTM = m_ReflectionPlane.ReflectionTM();
    Matrix4D reflViewTM( viewTM );
    reflViewTM.inverse();
    reflViewTM *= reflectTM;
    reflViewTM.inverse();
    IRS->SetViewTM( reflViewTM );
    IRS->ResetWorldTM();
    
    IRS->ClearDevice( 0xFF8888FF, true, false, false );

    static int shHud    = IRS->GetShaderID( "reflection_backdrop" );
    static int cloudsID = IRS->GetTextureID( "oblaka1.dds" );
	//static int cloudsID = IRS->GetTextureID( "sky.dds" );
    rsSetShader( shHud );
    rsSetTexture( cloudsID );

    Vector3D camPos = pCam->GetPosition();
    Rct uv = Rct::unit;
    static Timer timer;
    uv.x = timer.seconds()*0.007f;
    uv.y = timer.seconds()*0.003f;
    uv.w = uv.h = 1.0f;

    rsQuad2D(   Vector3D( 0.0f, 0.0f, 0.0f ),
                Vector3D( m_ReflectionMapSide, 0.0f, 0.0f ),  
                Vector3D( 0.0f, m_ReflectionMapSide, 0.0f ),    
                Vector3D( m_ReflectionMapSide, m_ReflectionMapSide, 0.0f ),
                0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
                uv );
    rsFlush();
    rsRestoreShader();
    rsSetTexture( -1 );

    IRS->SetClipPlane( 0, m_ReflectionPlane );
	IRS->SetClipPlane( 1, m_ReflectionPlane );

    //  render terrain
    if (m_bReflectTerrain)
    {
        static int trID = IRS->GetShaderID( "terra_reflected" );
        ITerra->SubstShader( trID );
        ITerra->Render();
        ITerra->SubstShader( -1 );
    }

    //  TODO: mirror light sources
    //  render objects
    int nObj = m_Object.size();
    static int shRefl = IRS->GetShaderID( "reflection_map" );
    IRS->SetShader( shRefl );

	IRS->SetClipPlane( 1, m_ReflectionPlane );
	IRS->SetClipPlane( 1, m_ReflectionPlane );

	Shader::PushOverride( shRefl );
    for (int i = 0; i < nObj; i++)
    {
        const ReflectedObject& obj = m_Object[i];
        SetEntityContext( obj.m_Context );

		Matrix4D tm( obj.m_TM );
		//  make reflected object look taller than they are
        tm.e22 *= 1.5f; 
        IMM->StartModel( obj.m_ModelID, tm, CUR_CONTEXT );
        IMM->DrawModel();
    }
    ResetEntityContext();
	IRS->SetClipPlane( 1, Plane(Vector3D(0,0,-10000),Vector3D::oZ ));
	Shader::PopOverride();

    //  restore rendering environment
    IRS->PopRenderTarget();
    IRS->SetViewTM( viewTM );
    IRS->SetViewPort( vp );
    
    //  calculate texture matrix
    Matrix4D projTM = IRS->GetProjTM();
    Matrix4D proj2tex;
    proj2tex.st( Vector3D( 0.5f, -0.5f, 1.0f ), Vector3D( 0.5f, 0.5f, 0.0f ) );
    m_TextureTM = proj2tex;
    m_TextureTM.mulLeft( projTM );

    m_Object.clear();

    if (m_bDrawDebugInfo) DrawDebugInfo();
    if (!m_bRenderReflection) return;
    
 //   //  render reflection geometry

    //BaseMesh bm;
    //Rct ext( 0.0f, 0.0f, m_ReflectionMapSide, m_ReflectionMapSide );
    //CreatePatchGrid<VertexTnL>( bm, ext, 
    //    m_BackdropGridNodes, m_BackdropGridNodes );
    //SetW<VertexTnL>( bm, 1.0f );

    //static BaseMesh quad;
    //if (quad.getNVert() == 0)
    //{
    //    float c_Ext = 1000.0f;
    //    Rct ext( -c_Ext, -c_Ext, c_Ext * 2.0f, c_Ext * 2.0f );
    //    CreatePatchGrid<Vertex2t>( quad, ext, 1, 1 );
    //}

    //m_pTexture->Render();
    //IRS->ResetWorldTM();

    //static int reflDSS = IRS->GetShaderID( "reflection" );
    //IRS->SetShader( reflDSS );

    //TransformNode::ResetTMStack();
    //DrawPrimBM( quad );

} // ReflectionMap::Render

Matrix4D ReflectionMap::GetReflectionTexTM() const
{
    return m_TextureTM;
}

void ReflectionMap::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "ReflectionMap", this );
    pm.f( "ReflectionMapSide",  m_ReflectionMapSide );
    pm.f( "UseDepthBuffer",     m_bUseDepthBuffer   );
    pm.f( "BackdropGridNodes",  m_BackdropGridNodes );
    pm.f( "RenderReflection",   m_bRenderReflection );
    pm.f( "RenderBackdrop",     m_bRenderBackdrop   );
    pm.f( "DebugInfo",          m_bDrawDebugInfo    );
    pm.m( "ChangeStructure", &ReflectionMap::Init                );
} // ReflectionMap::Expose

void ReflectionMap::Init()
{    
    m_ReflID = IRS->CreateTexture( "ReflectionTarget", m_ReflectionMapSide, m_ReflectionMapSide,
                                                        cfRGB565, 1, tmpDefault, true );
    if (m_bUseDepthBuffer)
    {
        m_DepthID = IRS->CreateTexture( "ReflectionDepth", m_ReflectionMapSide, 
                                            m_ReflectionMapSide, cfUnknown, 1, 
                                            tmpDefault, false, dsfD16 );
    }
    m_bInited = true;
} // ReflectionMap::Init

void ReflectionMap::AddObject( DWORD id, const Matrix4D* objTM )
{
    if (!objTM) return;
    m_Object.push_back( ReflectedObject( id, *objTM, GetEntityContext() ) );
} // ReflectionMap::AddObject

void ReflectionMap::CleanObjects()
{
    m_Object.clear();
} // ReflectionMap::CleanObjects

int ReflectionMap::GetReflectionTextureID() const
{
    return m_ReflID;
} // ReflectionMap::GetReflectionTextureID

void ReflectionMap::DrawDebugInfo()
{
    static int shHud    = IRS->GetShaderID( "reflection_backdrop" );
    rsSetShader( shHud );
    rsSetTexture( m_ReflID );

    Rct uv = Rct::unit;
    rsQuad2D(   Vector3D( 0.0f, 0.0f, 0.0f ),
        Vector3D( m_ReflectionMapSide, 0.0f, 0.0f ),  
        Vector3D( 0.0f, m_ReflectionMapSide, 0.0f ),    
        Vector3D( m_ReflectionMapSide, m_ReflectionMapSide, 0.0f ),
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
        uv );
    rsFlush();
    rsRestoreShader();
} // ReflectionMap::DrawDebugInfo


