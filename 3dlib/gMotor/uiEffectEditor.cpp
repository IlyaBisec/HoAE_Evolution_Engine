/*****************************************************************************/
/*    File:    uiEffectEditor.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    11-23-2003
/*****************************************************************************/
#include "stdafx.h"
#include "vCamera.h"

#include "sg.h"
#include "uiControl.h"
#include "sgEffect.h"

#include "kFilePath.h"

#include "uiWidget.h"
#include "uiWindow.h"
#include "uiNodeTree.h"
#include "uiEffectEditor.h"
#include "ITerrain.h"
#include "kSystemDialogs.h"
#include "IFontManager.h"
#include "ISkyRenderer.h"
#include "IShadowManager.h"

extern PEffectManager g_EMgr;

void EffectTreeRoot::Expose( PropertyMap& pm )
{
    
} // EffectTreeRoot::Expose

/*****************************************************************************/
/*    EffectEditor implementation
/*****************************************************************************/
IMPLEMENT_CLASS(EffectEditor);
IMPLEMENT_CLASS(EffectTreeRoot);
EffectEditor::EffectEditor()
{
    m_InstanceID        = -1;
    m_AlphaFactor       = 1.0f;
    m_IntensityFactor   = 1.0f;
    m_CurShotTime       = 0.0f;
    m_bShot             = false;
    m_bShowBackdrop     = true;
    m_PlayRate          = 1.0f;
    m_bLowPersp         = false;
    m_bShowGrid         = false;
	m_bInvertMouse		= false;
    m_bShowBackdrop     = true;
    m_Transform         = Matrix4D::identity;
    m_pEffect           = NULL;
    m_PaletteNodes      = NULL;
    m_bSaveTextures     = false;
    m_DirectionAngle    = 0.0f;
    m_bLockFPS          = true;
    m_bDumpSetupMode    = false;

	m_nRace = 0;
    
    m_DumpSide          = 512;
    m_DumpFPS           = 25.0f;


    SetEffectFireMode       ( fmParabolic        );
//    AddChild                ( m_Mover            );
	AddChild(m_MoveTool);
//	AddChild                ( m_TargetMover      );
	AddChild(m_AuxMoveTool);
    AddChild                ( m_Scaler           );
    AddChild                ( m_CameraController );
    AddChild                ( m_EffectTree       );
    AddChild                ( m_Inspector        );
    AddChild                ( m_Palette          );

    m_EffectTree.SetRootNode( &m_EffectRoot      );
    m_Inspector.SetVAligh   ( vaTop              );
    m_Inspector.SetHAligh   ( haCenter           );
    AlignToParent           ();
    AddChild                ( m_ToolBar          );

    SetName                 ( "EffectEditor"     );

    SetDraggable            ( false              );
    SetKeptAligned          ( true               );

    m_TargetTransform.translation( 0.0f, 0.0f, 300.0f );
} // EffectEditor::EffectEditor

void EffectEditor::SetupCameras()
{
    float aspect = IRS->GetViewPort().GetAspect();
    float volBound = 400.0f;
    float fovx = DegToRad( 90 );
    Vector3D gdir( 0.0f, -cos( c_PI/3.0f ), -sin( c_PI/3.0f ) );
    m_HighPerspCamera.SetOrientation( Vector3D::oX, Vector3D::oZ, gdir );
    gdir.reverse();
    float cdist = volBound * 1.8f / tan( fovx * 0.5f );
    gdir *= cdist;
    gdir.z += 80.0f;

    float zn = 50.0f;
    cdist -= zn;
    float zf = zn + cdist*1000.0f;
    m_HighPerspCamera.SetPosition( gdir );
    m_HighPerspCamera.SetFOVx( fovx, zn, zf, aspect );

    fovx = DegToRad( 20.0f );
    gdir = Vector3D( 0.0f, -cos( c_PI/6.0f ), -sin( c_PI/6.0f ) );
    m_LowPerspCamera.SetOrientation( Vector3D::oX, Vector3D::oZ, gdir );
    zn = 1200.0f; 
    zf = 10400.0f;
    m_LowPerspCamera.SetPosition( Vector3D( 0.0f, 2500, 1600 ) );
    m_LowPerspCamera.SetFOVx( fovx, zn, zf, IRS->GetViewPort().GetAspect() );
} // EffectEditor::SetupCameras

void EffectEditor::SetEffectFireMode( EffectFireMode fm )
{
    if (fm == fmParabolic)
    {
        m_ShotTime      = 5.0f;
        m_ShotVelocity  = 200.0f;
        m_ShotAngle     = c_PI/4.0f;
        m_ShotHeight    = 30.0f;
        m_ShotGravity   = 100.0f;
    }
    else if (fm == fmGroundMove)
    {
        m_ShotTime      = 500.0f;
        m_ShotVelocity  = 50.0f;
        m_ShotAngle     = 0.0f;
        m_ShotHeight    = 30.0f;
        m_ShotGravity   = 0.0f;
    }
    m_FireMode = fm;
} // EffectEditor::SetEffectFireMode

void EffectEditor::Expose( PropertyMap& pm )
{
    pm.start( "PEffect", this );
    pm.f( "AlphaFactor",        m_AlphaFactor       );    
    pm.f( "IntensityFactor",    m_IntensityFactor   );    
    pm.f( "ShotTime",           m_ShotTime          );
    pm.f( "ShotVelocity",       m_ShotVelocity      );
    pm.f( "ShotAhgle",          m_ShotAngle         );
    pm.f( "ShotHeight",         m_ShotHeight        );
    pm.f( "ShotGravity",        m_ShotGravity       );
    pm.p( "FireMode", &EffectEditor::GetEffectFireMode, &EffectEditor::SetEffectFireMode );
    pm.m( "Fire", &EffectEditor::Fire                );
    pm.m( "ResetCameras", &EffectEditor::ResetCameras        );

    pm.p( "ShowBackdrop", &EffectEditor::IsShowBackdrop, &EffectEditor::SetShowBackdrop );
    pm.p( "ShowGrid", &EffectEditor::ShowGrid, &EffectEditor::ShowGrid         );
	pm.p("InvertMouse", &EffectEditor::InvertMouse, &EffectEditor::InvertMouse);
    pm.f( "SaveTextures",       m_bSaveTextures            );

    pm.p( "Camera", &EffectEditor::GetCameraMode, &EffectEditor::SetCameraMode    );

    pm.p( "NumEmitters", &EffectEditor::GetNEmitters                );
    pm.p( "NumParticles", &EffectEditor::GetNParticles               );
    pm.p( "ParticleArea", &EffectEditor::GetParticleArea             );
    pm.p( "DangerColor", &EffectEditor::GetDangerColor              );
    pm.p( "IsEffectLoaded", &EffectEditor::IsEffectLoaded              );
    pm.f( "LowPersp",           m_bLowPersp                 );
    pm.f( "LockFPS",            m_bLockFPS                  );
    pm.p( "CurEffectTimeRatio", &EffectEditor::GetTimeRatio, &EffectEditor::SetTimeRatio  );

    pm.f( "DumpSide",           m_DumpSide                  );
    pm.f( "DumpFPS",            m_DumpFPS                   );
    pm.f( "DumpSetupMode",      m_bDumpSetupMode            );
    pm.m( "DumpSequence", &EffectEditor::DumpSequence                );
    pm.m( "KillAutoUpdate", &EffectEditor::KillAutoUpdate              );
    pm.m( "DumpSequence", &EffectEditor::DumpSetup                   );

    pm.p( "TotalTime", &EffectEditor::GetTotalTime                );
    pm.p( "DirAngle", &EffectEditor::GetDirAngle, &EffectEditor::SetDirAngle    );

    pm.p( "CurrentTime", &EffectEditor::GetCurTime                  );
    pm.p( "PlayRate", &EffectEditor::GetPlayRate, &EffectEditor::SetPlayRate    );
    pm.p( "EffectFile", &EffectEditor::GetEffectFile               );
    pm.m( "Reset", &EffectEditor::Reset                       );
    pm.m( "Load", &EffectEditor::Load                        );
    pm.m( "Save", &EffectEditor::Save                        );
    pm.m( "SaveAs", &EffectEditor::SaveAs                      );
    pm.m( "Play", &EffectEditor::PlayEffect                  );
    pm.m( "Pause", &EffectEditor::Pause                       );
    pm.m( "Stop", &EffectEditor::StopEffect                  );
} // EffectEditor::Expose

int EffectEditor::GetNEmitters() const
{
    return IEffMgr->GetNEmitterInst();
}

int EffectEditor::GetNParticles() const
{
    return IEffMgr->GetNParticles();
}

void EffectEditor::Fire()
{
    PlayEffect();
    m_bShot = true;
    m_CurShotTime = 0.0f;
    m_ShotTimer.start();
} // EffectEditor::Fire

float EffectEditor::GetPlayRate() const
{
    return m_PlayRate;
}

void EffectEditor::SetPlayRate( float rate )
{
    m_PlayRate = rate;
    IEffMgr->SetPlayRate( rate );
}


void EffectEditor::OnInit()
{
    m_ToolBar.CreateFromScript( "effect_editor_toolbar" );
    
    if (m_PaletteNodes) m_PaletteNodes->Release();
    m_PaletteNodes = CreatePalette();

    m_Palette.SetRootNode( m_PaletteNodes );
    m_Palette.SetRightHand();
    m_Palette.SetVisibleRoot( false );
    m_Palette.SetAcceptOnDrop( false );
    m_Palette.SetDragLeafsOnly( true );

    m_Palette.AlignToViewport();

    m_EffectTree.SetRootNode( &m_EffectRoot );

    Rct ext = IRS->GetViewPort();

    m_Palette.SetRootPos( ext.w - 5.0f, ext.h/2 );
    m_Palette.SetExtents( ext );

    CreatePalette();
    SetupCameras();

    _chdir( IRM->GetHomeDirectory() );
    _chdir( "\\Models\\Effects\\" );
    m_LastPath.GetCWD();

} // EffectEditor::OnInit

class PEmitterStub : public Group { virtual int GetGlyphID() const { return 1; } };
class PRenderStub  : public Group { virtual int GetGlyphID() const { return 2; } };
class PInitStub    : public Group { virtual int GetGlyphID() const { return 3; } };
class PPlacerStub  : public Group { virtual int GetGlyphID() const { return 4; } };
class PShooterStub : public Group { virtual int GetGlyphID() const { return 5; } };
class PVelocityStub: public Group { virtual int GetGlyphID() const { return 6; } };
class PColorStub   : public Group { virtual int GetGlyphID() const { return 7; } };
class PSizeStub    : public Group { virtual int GetGlyphID() const { return 8; } };
class PTextureStub : public Group { virtual int GetGlyphID() const { return 9; } };
class PTriggerStub : public Group { virtual int GetGlyphID() const { return 10; } };
class PTargetStub  : public Group { virtual int GetGlyphID() const { return 11; } };
class PMiscStub    : public Group { virtual int GetGlyphID() const { return 0; } };

SNode* EffectEditor::CreatePalette()
{
    Group* pCreate = new Group();
    pCreate->SetName( "Create" );    
    PEmitterStub* pEmitter = pCreate->AddChild<PEmitterStub>( "Emitter" );

    pEmitter->AddChild<PModularEmitter>         ( "Modular"             );
    pEmitter->AddChild<PStaticEmitter>          ( "Static"              );
    pEmitter->AddChild<PConstEmitter>           ( "Const"               );
    pEmitter->AddChild<PBurstEmitter>           ( "Burst"               );
    pEmitter->AddChild<PRampEmitter>            ( "Ramp"                );

    PRenderStub* pRender = pCreate->AddChild<PRenderStub>( "Render" );
    pRender->AddChild<PBillboardRenderer>       ( "Billboard"           );
    pRender->AddChild<PChainRenderer>           ( "Chain"               );
    pRender->AddChild<PSphereRenderer>          ( "Sphere"              );
    pRender->AddChild<PConeRenderer>            ( "Cone"                );
    pRender->AddChild<PTerrainDecal>            ( "TerraDecal"          );
    pRender->AddChild<PModelRendererWithStop>   ( "Model"               );

    PInitStub* pInit = pCreate->AddChild<PInitStub>( "Init" );
    pInit->AddChild<PSizeInit>                  ( "SizeInit"            );
    pInit->AddChild<PColorInit>                 ( "ColorInit"           );
    pInit->AddChild<PColorRampInit>             ( "ColorRamp"           );
    pInit->AddChild<PFrameInit>                 ( "FrameInit"           );

    PPlacerStub* pPlacer = pCreate->AddChild<PPlacerStub>( "Placer" );
    pPlacer->AddChild<PSpherePlacer>            ( "Sphere"              );
    pPlacer->AddChild<PBoxPlacer>               ( "Box"                 );
    pPlacer->AddChild<PCylinderPlacer>          ( "Cylinder"            );
    pPlacer->AddChild<PLinePlacer>              ( "Line"                );
    pPlacer->AddChild<PCirclePlacer>            ( "Circle"              );
    pPlacer->AddChild<PPointPlacer>             ( "Point"               );
    pPlacer->AddChild<PTargetPlacer>            ( "Target"              );
    pPlacer->AddChild<PModelPlacer>             ( "Model"               );

    PShooterStub* pShooter = pCreate->AddChild<PShooterStub>( "Shooter" );
    pShooter->AddChild<PConeShooter>            ( "Cone"                );
    pShooter->AddChild<PRadialShooter>          ( "Radial"              );
    pShooter->AddChild<PDirectShooter>          ( "Direct"              );
    pShooter->AddChild<PRampShooter>            ( "Ramp"                );

    PVelocityStub* pVelo = pCreate->AddChild<PVelocityStub>    ( "Velocity" );
    pVelo->AddChild<PForce>                     ( "Force"               );
    pVelo->AddChild<PAttract>                   ( "Attract"             );
    pVelo->AddChild<PTorque>                    ( "Torque"              );
    pVelo->AddChild<PVelRamp>                   ( "VelRamp"             );
    pVelo->AddChild<PAVelRamp>                  ( "AVelRamp"            );
    pVelo->AddChild<PWind>                      ( "Wind"                );
    pVelo->AddChild<PFollow>                    ( "Follow"              );
    pVelo->AddChild<PFluctuate>                 ( "Fluctuate"           );
    pVelo->AddChild<PDrag>                      ( "Drag"                );
    pVelo->AddChild<PVortex>                    ( "Vortex"              );
    pVelo->AddChild<POrbit>                     ( "Orbit"               );
    pVelo->AddChild<PRevolve>                   ( "Revolve"             );
    pVelo->AddChild<PTurbulence>                ( "Turbulence"          );
    pVelo->AddChild<PClampVelocity>             ( "ClampV"              );
    pVelo->AddChild<PCoastBreak>                ( "CoastBreak"          );

    PColorStub* pColor = pCreate->AddChild<PColorStub>( "Color" );
    pColor->AddChild<PColorRamp>                ( "ColorRamp"           );
    pColor->AddChild<PAlphaFade>                ( "AlphaFade"           );
    pColor->AddChild<PAlphaRamp>                ( "AlphaRamp"           );
    pColor->AddChild<PNatcolor>                 ( "Natcolor"            );

    PSizeStub* pSize = pCreate->AddChild<PSizeStub>( "Size" );
    pSize->AddChild<PSizeFade>                  ( "SizeFade"          );
    pSize->AddChild<PSizeRamp>                  ( "SizeRamp"          );

    PTextureStub* pTex = pCreate->AddChild<PTextureStub>( "Texture" );
    pTex->AddChild<PFrame>                      ( "FrameAnim"         );
    pTex->AddChild<PUVMove>                     ( "UVMove"           );

    PTriggerStub* pTrigger = pCreate->AddChild<PTriggerStub>( "Trigger" );
    pTrigger->AddChild<POnDeath>            ( "Death"                  );
    pTrigger->AddChild<POnBirth>            ( "Birth"                  );
    pTrigger->AddChild<POnTimer>            ( "Timer"                  );
    pTrigger->AddChild<POnHitGround>        ( "HitGround"              );
    pTrigger->AddChild<POnHitWater>            ( "HitWater"              );

    PTargetStub* pTarget = pCreate->AddChild<PTargetStub>( "Target" );
    pTarget->AddChild<PLightning>            ( "Lightning"              );
    pTarget->AddChild<PHoming>                ( "Homing"              );

    PMiscStub* pMisc = pCreate->AddChild<PMiscStub>( "Misc" );
    pMisc->AddChild<PMouseBind>                ( "MouseBind"            );
    pMisc->AddChild<PEffect>                ( "Instance"            );

    return pCreate;
} // EffectEditor::CreatePalette

void EffectEditor::SetDirAngle( float ang )
{
    Vector3D scale, trans;
    Quaternion rot;
    m_Transform.Factorize( scale, rot, trans );
    m_Transform = Matrix4D( scale, Vector3D::oZ, c_DoublePI*ang/360.0f, trans );
    m_DirectionAngle = ang;
} // EffectEditor::SetDirAngle

//-----------------------------------------------------------------------------
// EffectEditor::GenQuad : void(BaseMesh &, int, int, int)
//-----------------------------------------------------------------------------
void EffectEditor::GenQuad(BaseMesh &bm, int L, int idTex, int idSh) {
	bm.create(4, 6, vfVertexTS);

	// Topology:
	WORD *pInds = bm.getIndices();
	int Topology[6] = {0, 1, 3, 1, 2, 3};
	for(int i = 0; i < 6; i++) {
		pInds[i] = Topology[i];
	}

	// Verts:
	VertexTS *pVerts = (VertexTS*)bm.getVertexData();
	int x = - L / 2, y = - L / 2;
	float xPos[4] = {x, x, x + L, x + L};
	float yPos[4] = {y, y + L, y + L, y};
	float u[4] = {0.f, 0.f, 1.f, 1.f};
	float v[4] = {0.f, 1.f, 1.f, 0.f};
	for(int i = 0; i < 4; i++) {
		pVerts->x = xPos[i];
		pVerts->y = yPos[i];
		pVerts->z = 0;
		pVerts->u = u[i];
		pVerts->v = v[i];
		pVerts->diffuse = 0xffffffff;
		pVerts++;
	}

	bm.setNInd(6);
	bm.setNVert(4);
	bm.setNPri(2);
	bm.setTexture(idTex);
	bm.setShader(idSh);
}
DWORD g_Inst=0xFF7AFF7A;
void EffectEditor::Render()
{    
    if (m_bLowPersp) m_LowPerspCamera.Render(); else m_HighPerspCamera.Render();

    IRS->ResetWorldTM();
    IRS->SetFog( 0, 0.0f, 10000.0f, 0, 0 );
    
    m_Inspector.Bind( m_EffectTree.GetSelectedNode() );
    m_pEffect = m_EffectRoot.GetChild( 0 );

    Matrix4D tm = m_Transform;
    
    if (m_bLockFPS)
    {
        Sleep( 40 );
    }


    if (m_bShot && !IEffMgr->IsPaused())
    {
        float tPass = m_ShotTimer.seconds();
        if (tPass > m_ShotTime)
        {
            StopEffect();
            m_bShot = false;
            return;
        }
        m_CurShotTime = tPass;
        float x = m_ShotVelocity*cosf( m_ShotAngle )*m_CurShotTime;
        float y = m_ShotVelocity*sinf( m_ShotAngle )*m_CurShotTime - 
                        0.5f*m_ShotGravity*m_CurShotTime*m_CurShotTime;
        tm.e30 = x;
        tm.e32 = y;

        Vector3D vX( m_ShotVelocity*cosf( m_ShotAngle ), 0.0f, 
                     m_ShotVelocity*sinf( m_ShotAngle ) - m_ShotGravity*m_CurShotTime );
        Vector3D vZ( Vector3D::oY );
        Vector3D vY;
        vX.normalize();
        vY.cross( vX, vZ );
        vY.normalize();
        tm.getV0() = vX;
        tm.getV1() = vY;
        tm.getV2() = vZ;
        tm.e03 = tm.e13 = tm.e23 = 0;
    }

    if (m_bShowBackdrop) 
    {
        //if (ISky) ISky->Render();
        IShadowMgr->Render();

		BaseMesh bm;
		char TexFileName[128];
		sprintf(TexFileName, "Scripts\\F%d.dds", m_nRace + 1);
		int idTex = IRS->GetTextureID(TexFileName);
		int idSh = IRS->GetShaderID("kang_backdrop");
		GenQuad(bm, 1024, idTex, idSh);
		DrawBM(bm);

//        if (ITerra) ITerra->Render();


        IRS->Flush();
    }

    if (m_bShowGrid) DrawGrid( 800.0f, 10, 0xAF000022, 0xA8353545 );
    
    IEffMgr->SetAlphaFactor ( m_InstanceID, m_AlphaFactor );
    IEffMgr->SetIntensity   ( m_InstanceID, m_IntensityFactor );
    //if(!(GetKeyState(VK_SCROLL) & 0x8000))IEffMgr->UpdateInstance ( m_InstanceID, tm );    
    
    PushEntityContext( g_Inst );
    PushEntityContext( (DWORD)m_pEffect );    

    if(!(GetKeyState(VK_SCROLL) & 0x8000)){
        if (m_pEffect) m_InstanceID = IEffMgr->InstanceEffect( m_pEffect->GetID() );    
        IEffMgr->UpdateInstance( tm );
    }
    IEffMgr->SetTarget( m_TargetTransform.getTranslation() );
    PopEntityContext();
    PopEntityContext();

    IEffMgr->Evaluate();
    IEffMgr->PreRender();
    IEffMgr->PostRender();
    IEffMgr->PostEvaluate();
    
    //  drawing gizmos etc.
    TransformNode::ResetTMStack( &tm );
    if (m_pEffect) m_pEffect->Render();  
} // EffectEditor::Render

bool EffectEditor::OnChar( DWORD charCode, DWORD flags )
{
    if (charCode == ' ') PlayEffect();
    
    return false;
} // EffectEditor::OnChar

bool EffectEditor::OnKeyDown( DWORD keyCode, DWORD flags )
{
	if(keyCode >= VK_F1 && keyCode <= VK_F5) {
		m_nRace = keyCode - VK_F1;
	}

    if (keyCode == VK_RETURN){
        //g_Inst++;
        Fire();
    }
    if (keyCode == VK_HOME)     
    {
        ResetCameras();
        m_Transform.setIdentity();
//        m_Mover.BindTM( &m_Transform );
		m_MoveTool.BindTM(&m_Transform);
        m_Scaler.BindTM( &m_Transform );
  //      m_TargetMover.BindTM( &m_TargetTransform );
		m_AuxMoveTool.BindTM(&m_TargetTransform);
//        m_Mover.SetVisible( false );
		m_MoveTool.SetVisible(false);
        m_Scaler.SetVisible( false );
    }
    if (keyCode == 'Z')         m_bLowPersp = !m_bLowPersp;
    if (keyCode == 'W') 
    {
        m_Scaler.SetVisible( false );
//        m_Mover.SetVisible( !m_Mover.IsVisible() );
		m_MoveTool.SetVisible(!m_MoveTool.IsVisible());
//        m_Mover.BindTM( &m_Transform );
		m_MoveTool.BindTM(&m_Transform);
    }
    if (keyCode == 'R') 
    {
//        m_Mover.SetVisible( false );
		m_MoveTool.SetVisible(false);
//        m_TargetMover.SetVisible( false );
		m_AuxMoveTool.SetVisible(false);
        m_Scaler.SetVisible(!m_MoveTool.IsVisible());
		m_Scaler.BindTM(&m_Transform);
    }
    if (keyCode == 'Q') 
    {
//        m_Mover.SetVisible( false );
		m_MoveTool.SetVisible(false);
        m_Scaler.SetVisible( false );
//        m_TargetMover.SetVisible( false );
		m_AuxMoveTool.SetVisible(false);
    }
    if (keyCode == 'T') 
    {
        m_Scaler.SetVisible( false );
//        m_TargetMover.SetVisible( !m_TargetMover.IsVisible() );
		m_AuxMoveTool.SetVisible(!m_AuxMoveTool.IsVisible());
//        m_TargetMover.BindTM( &m_TargetTransform );
		m_AuxMoveTool.BindTM(&m_TargetTransform);
    }

    if (keyCode == 'L' && GetKeyState( VK_CONTROL ) < 0)
    {
        LoadSubtree();   
    }

    if (keyCode == 'S' && GetKeyState( VK_CONTROL ) < 0)
    {
        SaveSubtree();
    }

    return false;
} // EffectEditor::OnKeyDown

void EffectEditor::SaveSubtree()
{
    const char* fileName = NULL;
    m_LastPath.SetCWD();

    SaveFileDialog dlg;
    dlg.AddFilter( "Binary ModelObject Files",    "*.c2m"     );
    //  show "Open File" dialog
    dlg.SetDefaultExtension( "c2m" );
    if (!dlg.Show()) return;

    fileName = dlg.GetFilePath();
    m_LastPath.SetPath( fileName );

    _chdir( IRM->GetHomeDirectory() );
    if (!fileName) return;

    SNode* pParent = dynamic_cast<SNode*>( m_EffectTree.GetSelectedNode() );
    if (!pParent) return;
    pParent->SerializeSubtree( FOutStream( fileName ) );
} // ModelEditor::SaveSubtree

void EffectEditor::LoadSubtree()
{
    const char* fileName = NULL;
    m_LastPath.SetCWD();

    OpenFileDialog dlg;
    dlg.AddFilter( "Binary ModelObject Files",    "*.c2m"     );
    //  show "Open File" dialog
    dlg.SetDefaultExtension( "c2m" );
    if (!dlg.Show()) return;

    fileName = dlg.GetFilePath();
    m_LastPath.SetPath( fileName );

    _chdir( IRM->GetHomeDirectory() );
    if (!fileName) return;

    SNode* pSubtree = SNode::UnserializeSubtree( FInStream( fileName ) );

    if (!pSubtree) return;

    SNode* pParent = dynamic_cast<SNode*>( m_EffectTree.GetSelectedNode() );
    if (pParent) pParent->AddChild( pSubtree );    
} // ModelEditor::LoadSubtree

void EffectEditor::ResetCameras()
{
    SetupCameras();
} // EffectEditor::OnKeyDown

void EffectEditor::SetShowBackdrop( bool val )
{
     m_bShowBackdrop = val;
}

bool EffectEditor::IsShowBackdrop() const
{
    return m_bShowBackdrop;
}

void EffectEditor::Reset()
{
    IEffMgr->DestroyInstance( m_InstanceID );
    m_InstanceID = 0xFFFFFFFF;
    if (m_pEffect)
    {
        m_pEffect->Release();
        m_pEffect = NULL;
    }
    m_EffectFile = "";
    m_EffectRoot.ReleaseChildren();
    IEffMgr->Reset();
} // EffectEditor::Reset

void EffectEditor::Pause()
{
    if (IEffMgr->IsPaused()) 
    {
        m_ShotTimer.pause();
    }
    else
    {
        m_ShotTimer.resume();
    }

    IEffMgr->Pause( !IEffMgr->IsPaused() );
} // EffectEditor::Pause

void EffectEditor::Load()
{
    m_LastPath.SetCWD();

    const char* fileName = NULL;
    OpenFileDialog dlg;
    dlg.AddFilter( "Binary ModelObject Files", "*.c2m" );
    dlg.AddFilter( "XML Effect Files", "*.eff" );
    dlg.SetDefaultExtension( "c2m" );

    static char lpstrFile[_MAX_PATH];
    bool dlgRes = dlg.Show();
    ///RelaxDialog();
    if (!dlgRes) return;

    fileName = dlg.GetFilePath();
    m_LastPath.SetPath( fileName );
    m_LastPath.SetFileName( "" );
    m_LastPath.SetExt( "" );

    if (fileName)
    {
        m_LastPath.SetCWD();
        
        int mID = IMM->GetModelID( fileName );
        IMM->StartModel( mID );
        IMM->DrawModel();

        FInStream is( fileName );
        SNode* pEffect = SNode::UnserializeSubtree( is );
        if (!pEffect) 
        {
            Log.Error( "Could not find effect file: %s", fileName );
            return;
        }
        IMM->GetModelID( fileName );
        if (!pEffect->IsA<PEmitter>())
        {
            Log.Error( "Bad effect file: %s", fileName );
            return;
        }

        Reset();
        m_pEffect = pEffect;

        char drive        [_MAX_DRIVE];
        char directory    [_MAX_DIR  ];
        char fname        [_MAX_PATH ];
        char ext        [_MAX_EXT  ];

        _splitpath( fileName, drive, directory, fname, ext );
        strcat( fname, ext );

        m_EffectRoot.ReleaseChildren();
        m_EffectRoot.AddChild( m_pEffect );
        PlayEffect();
        m_EffectFile = fname;
    }
} // EffectEditor::Load

void EffectEditor::Save()
{
    _chdir( IRM->GetHomeDirectory() );
    _chdir( "Models\\Effects\\" );

    FOutStream os( m_EffectFile.c_str() );
    if (os.NoFile()) 
    {
        SaveAs();
    }
    else
    {
        SNode* pEmitter = dynamic_cast<SNode*>( m_EffectTree.Child( 0 ) );
        if (!pEmitter) return;
        pEmitter->SerializeSubtree( os );
    }

    if (m_bSaveTextures) SaveEffectTextures( m_EffectFile.c_str() );

} // EffectEditor::Save

void EffectEditor::SaveAs()
{
    _chdir( IRM->GetHomeDirectory() );
    _chdir( "Models\\Effects\\" );

    SaveFileDialog dlg;

    dlg.AddFilter( "Binary ModelObject Files", "*.c2m" );
    dlg.AddFilter( "XML ModelObject Files", "*.eff" );
    dlg.SetDefaultExtension( "c2m" );

    if (!m_pEffect) return;
    bool dlgRes = dlg.Show();
    if (!dlgRes) return;
    m_EffectFile = dlg.GetFilePath();
    FOutStream os( m_EffectFile.c_str() );
    if (os.NoFile()) return;
    m_pEffect->SerializeSubtree( os );
    if (m_bSaveTextures) SaveEffectTextures( m_EffectFile.c_str() );
} // EffectEditor::SaveAs

void EffectEditor::SaveEffectTextures( const char* path )
{
    if (!m_pEffect) return;
    FilePath tpath( path );
    tpath.SetFileName( "" );
    SNode::Iterator it( m_pEffect );
    while (it)
    {
        PRenderer* pRenderer = (PRenderer*)*it;
        if (pRenderer->IsA<PRenderer>())
        {
            FilePath srcPath( IRS->GetTexturePath( pRenderer->GetTexID() ) );
            tpath.SetFileName( srcPath.GetFileName() );
            tpath.SetExt( srcPath.GetExt() );

			IRM->CheckFileAccess(tpath.GetFullPath());
            CopyFile( srcPath.GetFullPath(), tpath.GetFullPath(), FALSE );

        }
        ++it;
    }
} // EffectEditor::SaveEffectTextures

void EffectEditor::PlayEffect()
{
    IEffMgr->DestroyInstance( m_InstanceID );
    IEffMgr->Pause( false );
    m_bShot = false;
    
    if (!m_pEffect) return;

    PushEntityContext( 0xFF7AFF7A );
    PushEntityContext( (DWORD)m_pEffect );
    m_InstanceID = IEffMgr->InstanceEffect( m_pEffect->GetID() );    
    PopEntityContext();
    PopEntityContext();

} // EffectEditor::PlayEffect

void EffectEditor::StopEffect()
{
    IEffMgr->DestroyInstance( m_InstanceID );
    IEffMgr->Pause( false );
}

void EffectEditor::SetEffectFile( const char* fname )
{
    m_EffectFile = fname;
    m_pEffect = NULL;
} // EffectEditor::SetEffectFile

float EffectEditor::GetTotalTime() const
{
    return IEffMgr->GetTotalTime( m_InstanceID ); 
}

float EffectEditor::GetTimeRatio() const 
{ 
    return GetCurTime()/GetTotalTime(); 
}

void EffectEditor::SetTimeRatio( float t )
{
    float cTime = t*IEffMgr->GetTotalTime( m_InstanceID );
    //IEffMgr->RewindEffect( m_InstanceID, cTime );
} // EffectEditor::SetTimeRatio

float EffectEditor::GetCurTime() const
{
    return IEffMgr->GetCurTime( m_InstanceID ); 
}

int EffectEditor::GetParticleArea() const
{
    return IEffMgr->GetParticleArea();
} // EffectEditor::GetParticleArea

const int c_DangerHigh      = 1000000; 
const int c_DangerMiddle    = 100000;
const int c_DangerLow       = 10000;
DWORD EffectEditor::GetDangerColor() const
{
    int area = IEffMgr->GetParticleArea();
    if (area > c_DangerHigh)    return 0xFFFF0000;
    if (area > c_DangerMiddle)  return 0xFFFFFF00;
    if (area > c_DangerLow)     return 0xFF00FF00;
    return 0xFFFFFFFF;
} // EffectEditor::GetDangerColor

void EffectEditor::DumpSetup()
{
    
} 
void EffectEditor::KillAutoUpdate()
{
    if (!m_pEffect) return;
    SNode::Iterator it( m_pEffect );
    while (it)
    {
        SNode* pNode = (SNode*)*it;
        if (pNode->IsA<PEmitter>()){
            PEmitter* pe = (PEmitter*)pNode;
            pe->SetAutoUpdated(false);
        }
        ++it;
    }
}

void EffectEditor::DumpSequence()
{
    m_bDumpSetupMode = false;

    if (!m_pEffect) return;
    SaveFileDialog dlg;

    dlg.AddFilter( "Tga Files", "*.tga" );
    dlg.SetDefaultExtension( "tga" );

    bool dlgRes = dlg.Show();
    if (!dlgRes) return;
    const char* sampleName = dlg.GetFilePath();
    char drive  [_MAX_PATH];
    char dir    [_MAX_PATH];
    char file   [_MAX_PATH];
    char ext    [_MAX_PATH];
    _splitpath( sampleName, drive, dir, file, ext );
        
    float timeTotal = IEffMgr->GetTotalTime( m_InstanceID ); 

    IRS->DeleteTexture( IRS->GetTextureID( "SpriteSurf80" ) );
    IRS->DeleteTexture( IRS->GetTextureID( "SpriteSurf81" ) );
    IRS->DeleteTexture( IRS->GetTextureID( "SpriteSurf82" ) );
    IRS->DeleteTexture( IRS->GetTextureID( "SpriteSurf83" ) );
    IRS->DeleteTexture( IRS->GetTextureID( "SpriteSurf84" ) );
    IRS->DeleteTexture( IRS->GetTextureID( "SpriteSurf85" ) );
    IRS->DeleteTexture( IRS->GetTextureID( "SpriteSurf86" ) );
    IRS->DeleteTexture( IRS->GetTextureID( "SpriteSurf87" ) );
    IRS->DeleteTexture( IRS->GetTextureID( "SpriteSurf88" ) );
    IRS->DeleteTexture( IRS->GetTextureID( "SpriteSurf89" ) );
    IRS->DeleteTexture( IRS->GetTextureID( "SpriteSurf90" ) );
    IRS->DeleteTexture( IRS->GetTextureID( "SpriteSurf91" ) );
    IRS->DeleteTexture( IRS->GetTextureID( "SpriteSurf92" ) );
    IRS->DeleteTexture( IRS->GetTextureID( "SpriteSurf93" ) );
    IRS->DeleteTexture( IRS->GetTextureID( "SpriteSurf94" ) );
    IRS->DeleteTexture( IRS->GetTextureID( "SpriteSurf95" ) );
    IRS->DeleteTexture( IRS->GetTextureID( "SpriteSurf96" ) );
    IRS->DeleteTexture( IRS->GetTextureID( "SpriteSurf97" ) );
    IRS->DeleteTexture( IRS->GetTextureID( "SpriteSurf98" ) );
    IRS->DeleteTexture( IRS->GetTextureID( "SpriteSurf99" ) );

    int ds = IRS->CreateTexture( "EffDS", m_DumpSide, m_DumpSide, 
        cfUnknown, 1, tmpDefault, false, dsfD16 );
    
    int target = IRS->CreateTexture( "EffectDumpTarget", m_DumpSide, m_DumpSide, 
        cfARGB8888, 1, tmpDefault, true );

    
    rsFlush();
    Rct vp = IRS->GetViewPort();

    IRS->PushRenderTarget( target, ds );
    IRS->SetViewPort( Rct( 0.0f, 0.0f, m_DumpSide, m_DumpSide ) );

    float t = 0.0f;
    float dt = 1.0f/m_DumpFPS;
    int curTex = 0;

    IEffMgr->DestroyInstance( m_InstanceID );
    IEffMgr->Pause( false );
    m_bShot = false;
    
    if (!m_pEffect) return;

    //  setup camera
    ICamera* curCam = GetCamera();
    curCam->Render();
    PerspCamera* pCam = dynamic_cast<PerspCamera*>( curCam );
    OrthoCamera* oCam = dynamic_cast<OrthoCamera*>( curCam );

    if (pCam)
    {
        PerspCamera cam;
        cam.SetViewTM( pCam->GetViewTM() );
        cam.SetFOVx( pCam->GetFOVx(), pCam->GetZn(), pCam->GetZf(), 1.0f );
        IRS->SetProjTM( cam.GetProjTM() );
    }
    else if (oCam) 
    {
        OrthoCamera cam;
        cam.SetViewTM( oCam->GetViewTM() );
        cam.SetViewVolume( oCam->GetViewVolume(), oCam->GetZn(), oCam->GetZf(), 1.0f );
        IRS->SetProjTM( cam.GetProjTM() );
    }

    //  start effect
    PushEntityContext( 0xFF7AFF7A );
    PushEntityContext( (DWORD)m_pEffect );
    m_InstanceID = IEffMgr->InstanceEffect( m_pEffect->GetID() );    
    

    while (t <= timeTotal)
    {
        char texName [_MAX_PATH];
        char fileName[_MAX_PATH];
        
        sprintf( fileName, "%s%03d", file, curTex );
        _makepath( texName, drive, dir, fileName, "tga" );

        m_InstanceID = IEffMgr->InstanceEffect( m_pEffect->GetID() );    
        IEffMgr->UpdateInstance();
        IEffMgr->SetTarget( m_TargetTransform.getTranslation() );

        IRS->ClearDevice( 0x00000000, true, true );
        IEffMgr->Evaluate( dt );
        IEffMgr->PreRender();
        IEffMgr->PostRender();
    
        IRS->SaveTexture( target, texName );
        curTex++;
        t += dt;
    }

    PopEntityContext();
    PopEntityContext();

    //  stop effect
    IEffMgr->DestroyInstance( m_InstanceID );

    IRS->PopRenderTarget();
    IRS->SetViewPort( vp );
    
    IRS->DeleteTexture( target );
    IRS->DeleteTexture( ds );
} // EffectEditor::DumpSequence
    
