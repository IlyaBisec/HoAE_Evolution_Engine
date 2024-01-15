/*****************************************************************************/
/*    File:    uiModelEditor.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10-31-2003
/*****************************************************************************/
#include "stdafx.h"

#include "kCommand.h"
#include "kSystemDialogs.h"

#include "vCamera.h"

#include "IFontManager.h"
#include "IEffectManager.h"
#include "IMediaManager.h"
#include "IShadowManager.h"
#include "ISkyRenderer.h"
#include "IWater.h"
#include "ITerrain.h"
#include "mLodder.h"
#include "mSkin.h"
#include "kContext.h"
#include "kHose.h"
#include "kFilePath.h"

#include "rsRenderSystem.h"

#include "sgModel.h"

#include "uiWidget.h"
#include "uiWindow.h"
#include "uiCheckBox.h"
#include "uiButton.h"
#include "uiFrameWindow.h"
#include "uiMainFrame.h"
#include "uiMayaController.h"
#include "uiObjectInspector.h"
#include "uiNodeTree.h"
#include "uiModelView.h"

#include "uiModelEditor.h"

#include "sg.h"
#include "sgEffect.h"
#include "sgAnimation.h"
#include "vSkin.h"

#include "sgPhysics.h"

bool s_FontsInit=false;
int s_FontsID[256];
int s_CurrentFont=8;

void s_InitFonts(){
	if(!s_FontsInit){
		s_FontsInit=true;
		memset(&s_FontsID,0xFF,sizeof s_FontsID);
	}	    
}
DIALOGS_API void SetCurrentFont(BYTE Size){
    s_InitFonts();
	s_CurrentFont=Size;
}

int    GetEditorFontID()
{
	s_InitFonts();
	if(s_FontsID[s_CurrentFont]==-1){
		s_FontsID[s_CurrentFont]=IWM->CreateFont( "Tahoma", s_CurrentFont );
	}    
    return s_FontsID[s_CurrentFont];;
}

int    GetEditorGlyphsID()
{
    static int s_GlyphsID = IWM->CreateUniformFont( "sgIcons.tga", 16, 16 );
    return s_GlyphsID;
}

ISceneEditor* IScEd = NULL;

DIALOGS_API bool DrawText( int x, int y, DWORD color, const char* format, ... )
{
    char buffer[1024]; 
    va_list argList;
    va_start( argList, format );
    vsprintf( buffer, format, argList );
    va_end( argList );
    IWM->DrawString( GetEditorFontID(), buffer, Vector3D( x, y, 0 ), color );
    return true;
} // DrawText
DIALOGS_API void DrawChar(int x,int y,char c,DWORD Color){
    IWM->DrawChar( GetEditorFontID(), Vector3D( x, y, 0 ), c, Color );
}
DIALOGS_API int GetCharWidth(char c){
    return IWM->GetCharWidth( GetEditorFontID(),c );
}
DIALOGS_API int GetCharHeight(char c){
    return IWM->GetCharHeight( GetEditorFontID(),c );
}

DIALOGS_API bool DrawTextEx( int x, int y, DWORD color, const char* str,int MaxWidth,int AddLineHeight)
{
	IWM->DrawString( GetEditorFontID(), str, Vector3D( x, y, 0 ), color );
	return true;
} // DrawText

DIALOGS_API int GetTextWidth( const char* format, ... )
{
    char buffer[1024]; 
    va_list argList;
    va_start( argList, format );
    vsprintf( buffer, format, argList );
    va_end( argList );
    return IWM->GetStringWidth( GetEditorFontID(), buffer );
} // GetTextWidth

DIALOGS_API int GetTextHeightEx( const char* str ,int& MaxWidth,int AddLineHeight)
{
	MaxWidth = IWM->GetStringWidth( GetEditorFontID(), str );
	return GetTextHeight();
} // GetTextHeightEx

DIALOGS_API int GetTextHeight()
{
    return IWM->GetCharHeight( GetEditorFontID(), 'p' );
} // GetTextWidth

DIALOGS_API bool DrawText( const Vector3D& pos, DWORD color, const char* format, ... )
{
    char buffer[1024]; 
    va_list argList;
    va_start( argList, format );
    vsprintf( buffer, format, argList );
    va_end( argList );
    IWM->DrawStringW( GetEditorFontID(), buffer, pos, color );
    return true;
} // DrawText

DIALOGS_API void FlushText()
{
	for(int i=0;i<256;i++)if(s_FontsID[i]!=-1)
    IWM->FlushText( s_FontsID[i] );
}

/*****************************************************************************/
/*    ModelEditor implementation
/*****************************************************************************/
IMPLEMENT_CLASS(ModelEditor);
ModelEditor::ModelEditor()
{
    int sz = sizeof( VertexDeclaration );
    IScEd = this;
    
    AddChild( m_Inspector       );
    AddChild( m_TreeL           );
    AddChild( m_TreeR           );
    AddChild( m_PalTree         );
    AddChild( m_MayaController  );
    AddChild( m_Toolbar         );
    AddChild( m_Measure         );
    
    m_Measure.AlignToParent();

    m_Inspector.SetName( "Inspector" );
    m_Inspector.SetVAligh( vaTop );
    m_Inspector.SetHAligh( haCenter );
    
    AlignToParent();
    m_MayaController.AlignToParent();

    m_bShowCameras          = false;
    m_bShowLights           = false;
    m_bShowLocators         = false;
    m_bShowTransformNodes   = false;
    m_bShowBones            = false;
    m_bShowNormals          = false;
    m_bShowModelStats       = false;
    m_bShowBackdrop         = false;
    m_bDrawModel            = true;

    m_Measure.SetVisible( false );
    m_bDrawMeasure          = false;

    m_FrustumColor          = 0x0;
    m_FrustumLinesColor     = 0x88FF0000;

    m_AABBFillColor         = 0x0;
    m_AABBLinesColor        = 0x660000FF;
    m_Size                  = 0.001f;
    m_HandleSide            = 2.0f;
    m_AnimSpeed             = 1.0f;
    m_ActiveCamera          = aecEditor;

    m_bShowGrid             = true;                
    m_GridColor             = 0xFF000000;
    m_GridSubColor          = 0xB8454545;
    m_GridSide              = 800.0f;            
    m_NGridCells            = 20.0f;

    m_pSelectedNode         = NULL;
    m_pRootNode             = NULL;

    m_bPlayAnim             = false;
    m_bPauseAnim            = false;


    m_bInited               = false;

    m_ModelTransparency     = 0.5f;
    m_ModelColor            = 0xFFFFFFFF;

    SetTopOnFocus   ( false );
    SetDraggable    ( false );
    SetKeptAligned  ( true );

    SetName         ( "ModelEditor" );
    m_MeasureTM.setIdentity();

} // ModelEditor::ModelEditor

ModelEditor::~ModelEditor()
{
}

bool ModelEditor::IsModelLoaded() const 
{ 
    return (m_pRootNode != NULL); 
}

bool ModelEditor::IsAnimLoaded() const 
{ 
    return (m_AnimID != -1); 
}

void ModelEditor::OnInit()
{
    m_ModelID   = -1;
    m_AnimID    = -1;

    m_TreeL.SetName             ( "LeftTree"    );
    m_TreeL.SetRootNode         ( NULL          );
    m_TreeL.SetDragLeafsOnly    ( false         );

    m_TreeR.SetName             ( "RightTree"   );
    m_TreeR.SetRootNode         ( NULL          );
    m_TreeR.SetDragLeafsOnly    ( false         );
    m_TreeR.SetRightHand        ( true          );
    m_TreeR.SetVisible          ( false         );

    m_PalTree.SetName           ( "PalTree" );
    m_PalTree.SetRootNode       ( NULL );
    m_PalTree.SetRightHand      ();
    m_PalTree.SetVisibleRoot    ( false );
    m_PalTree.SetAcceptOnDrop   ( false );
    m_PalTree.SetDraggable      ( true  );
    m_PalTree.SetEditable       ( false );
    m_PalTree.SetVisible        ( false );

    Rct ext = IRS->GetViewPort  ();
    m_PalTree.SetRootPos        ( ext.w - 10, 200.0f );

    //  editor camera
    m_EditorCamera.SetName      ( "EditorCamera" );

    //  perspective game camera
    m_GamePerspCamera.SetName   ( "AltCamera" );
    //  ortho game camera
    m_GameOrthoCamera.SetName   ( "GameCamera" ); 

    ResetCameras();
    m_bInited = true;

    m_Toolbar.CreateFromScript( "model_editor_toolbar" );

    m_LastPath.SetPath( IRM->GetHomeDirectory() );
    m_LastPath.AppendDir( "Models\\" );

    IShadowMgr->SetShadowQuality( sqHigh );
} // ModelEditor::Init

IReflected* ModelEditor::GetSelectedNode() 
{ 
    return m_TreeL.GetSelectedNode(); 
}

void  ModelEditor::SelectNode( IReflected* pNode ) 
{ 
    m_TreeL.SelectNode( pNode ); 
}

bool ModelEditor::OnMouseLBDown( int mX, int mY )
{
    return false;
} // ModelEditor::OnMouseLBDown

float ModelEditor::GetCurAnimTimeRatio() const
{
    return m_AnimTimer.seconds()*1000.0f/IMM->GetAnimTime( m_AnimID );
} // ModelEditor::GetCurAnimTimeRatio

void ModelEditor::SetCurAnimTimeRatio( float t )
{
    m_AnimTimer.set( t*IMM->GetAnimTime( m_AnimID )/1000.0f );
} // ModelEditor::SetCurAnimTimeRatio

void ModelEditor::Expose( PropertyMap& pm )
{    
    pm.start<FrameWindow>( "ModelEditor", this );
    pm.p( "ShowCameras", &ModelEditor::GetShowCameras, &ModelEditor::ShowCameras );
    pm.f( "ShowLights",            m_bShowLights                );
    pm.f( "ShowLocators",        m_bShowLocators                );
    pm.f( "ShowBones",            m_bShowBones                );
    pm.f( "ShowTransformNodes",    m_bShowTransformNodes        );
    pm.f( "ShowNormals",        m_bShowNormals              );
    pm.f( "ShowGrid",            m_bShowGrid                    );
    pm.f( "ShowBackdrop",        m_bShowBackdrop                );
    pm.f( "ShowModelStats",     m_bShowModelStats           );
    pm.f( "ActiveCamera",        m_ActiveCamera                );
    pm.f( "AnimSpeed",          m_AnimSpeed                 );
    pm.p( "CurAnimTimeRatio", &ModelEditor::GetCurAnimTimeRatio, &ModelEditor::SetCurAnimTimeRatio );
    pm.p( "ModelLoaded", &ModelEditor::IsModelLoaded               );
    pm.p( "AnimLoaded", &ModelEditor::IsAnimLoaded                );
    pm.f( "AnimPlayed",         m_bPlayAnim                 );
    pm.f( "AnimPaused",         m_bPauseAnim                );
    pm.f( "ModelTransparency",  m_ModelTransparency         );
    pm.f( "ModelColor",         m_ModelColor                );

    pm.m( "Play", &ModelEditor::Play                        );
    pm.m( "Pause", &ModelEditor::Pause                      );
    pm.m( "Stop", &ModelEditor::Stop                        );

    pm.m( "ResetModel", &ModelEditor::ResetModel                 );
    pm.m( "LoadModel", &ModelEditor::LoadModel                   );
    pm.m( "SaveModel", &ModelEditor::SaveModel                   );
    pm.m( "LoadAnimation", &ModelEditor::LoadAnimation               );
	pm.m( "ConvertSkin2GPU", &ModelEditor::ConvertSkin2GPU             );
} // ModelEditor::Expose

void ModelEditor::Pause()
{
    if (!m_bPlayAnim) return;
    m_bPauseAnim = !m_bPauseAnim;
    if (m_bPauseAnim) 
    {
        m_AnimTimer.pause(); 
    }
    else 
    {
        m_AnimTimer.resume();
    }
} // ModelEditor::Pause

void ModelEditor::Stop()
{
    m_bPlayAnim     = false;
    m_bPauseAnim    = false;
    m_AnimTimer.pause();
}

void ModelEditor::Play()
{
    m_AnimTimer.start( IMM->GetAnimTime( m_AnimID )/1000.0f );
    m_bPlayAnim         = true;
    m_bPauseAnim        = false;
} // ModelEditor::Play

void ModelEditor::ShowCameras( bool val )
{
    m_bShowCameras = val;
} // ModelEditor::ShowCameras

void ModelEditor::SetModel( int mdlID )
{
    assert( false );
    m_TreeL.SetRootNode( NULL );
    m_TreeR.SetRootNode( NULL );
} // ModelEditor::SetModel

void TestThumbnail()
{
    static int mdlID  = IMM->GetModelID( "Units\\UndHKni\\UndHKniM.c2m" );
    static int anmID = IMM->GetModelID( "Units\\UndHKni\\UndHKniA_walk1.c2m" );
    float totalTime = IMM->GetAnimTime( anmID );
    static Timer timer( totalTime );
    float t = fmodf( timer.seconds()*1000.0f, totalTime );
    
    Rct vp = Rct( 200, 100, 400, 500 );
    Rct tRect = Rct( 100, 200, 200, 100 );

    rsFrame( vp, 0.0f, 0xFFFF0000 );
    rsFlush();

    IRS->SetViewPort( vp );
    DrawThumbnail( mdlID, tRect, anmID, t, 0.5, 0xFF5555FF );
} // TestThumbnail
static SNode* pPhysObj = NULL;
CollideHashSpace* pCHSpace=NULL;
JointCluster* pCluster=NULL;

Body* CreateRagdoll(SNode* Root){
	return NULL;

	if(!pCluster){
		pCluster=pPhysObj->AddChild<JointCluster>();
	}
	TransformNode* T=dynamic_cast<TransformNode*>(Root);
	int n=Root->GetNChildren();
	if(T){		
		CollideSphere* pSphere1=pCHSpace->AddChild<CollideSphere>();
		Body* pBody    = pPhysObj->AddChild<Body>();		
		pBody->SetBone( T );
		pBody->SetDrawGizmo();
		pBody->SetMass( 1.0f );
		pBody->CreateBody();
		
		pSphere1->SetLocalTM( T->GetWorldTM() );
		pSphere1->SetRadius( 20.0f );
		pSphere1->AttachToBody( pBody );
		pSphere1->SetDrawGizmo();

		for(int i=0;i<n;i++){
            Body* B=CreateRagdoll(Root->GetChild(i));
			if(B){

			}
		}
		return pBody;
	}
	for(int i=0;i<n;i++){
		CreateRagdoll(Root->GetChild(i));
	}
	return NULL;
}
void TestPhysics()
{
    
    static int mdlID = IMM->GetModelID( "sphere" );
    static TransformNode* pTransform = NULL;

    if (!pPhysObj)
    {
        IMM->StartModel( mdlID );
        IMM->DrawModel();

        SNode::Iterator it( IMM->GetModelRoot( mdlID ) );
        while (it)
        {
            pTransform = (TransformNode*)*it;
            if (pTransform->IsA<TransformNode>()) break;
            ++it;
        }
		if(pTransform){
			Matrix4D initTM;
			initTM.translation( 0.0f, 0.0f, 600.0f );
			pTransform->SetTransform( initTM );

			pPhysObj = new PhysicsObject();
			pCHSpace = pPhysObj->AddChild<CollideHashSpace>();
			CollidePlane*     pGround  = pCHSpace->AddChild<CollidePlane>();
			Vector3D n( 0.1f, 0.0f, 1.0f ); n.normalize();
			pGround->SetPlane( Plane( Vector3D::null, n ) );
			pGround->SetDrawGizmo();
			CollideSphere*    pSphere1 = pCHSpace->AddChild<CollideSphere>();
			CollideSphere*    pSphere2 = pCHSpace->AddChild<CollideSphere>();

			Body*             pBody    = pPhysObj->AddChild<Body>();
			pBody->SetBone( pTransform );
			pBody->SetDrawGizmo();
			pBody->SetMass( 1.0f );
			pBody->CreateBody();
			Vector3D vel( -1.0f, 0.0f, 0.0f );
			pBody->SetVelocity( vel );
			pBody->SetAngVelocity( vel );

			Matrix4D sTM1;
			sTM1.translation( 0.0f, 0.0f, -100.0f );
			pSphere1->SetLocalTM( sTM1 );
			pSphere1->SetRadius( 180.0f );
			pSphere1->AttachToBody( pBody );
			pSphere1->SetDrawGizmo();

			Matrix4D sTM2;
			sTM2.translation( 0.0f, 0.0f, 100.0f );
			pSphere2->SetLocalTM( sTM2 );
			pSphere2->SetRadius( 150.0f );
			pSphere2->AttachToBody( pBody );
			pSphere2->SetDrawGizmo();
		}
    }

    if(pPhysObj)pPhysObj->Render();
    //if(pTransform)pTransform->Render();
    PhysicsSystem::inst().Render();    
} // TestPhysics

void TestGPMagic()
{
    static int gpID1 = ISM->GetPackageID( "G\\DruDruG" );
    static int gpID2 = ISM->GetPackageID( "Cash\\Border" );
    DWORD m1 = ISM->GetPackageMagic( gpID1 );
    DWORD m2 = ISM->GetPackageMagic( gpID2 );
}

void TestGP2()
{
    static int gpID1 = ISM->GetPackageID( "G\\DruCenC" );
    static int gpID2 = ISM->GetPackageID( "G\\DruCenG" );
    static int shID  = IRS->GetShaderID( "sprite_natcolor" );

    ISM->Flush();
    
    const int c_NDir = 16;

    int nF1 = ISM->GetNFrames( gpID1 )/c_NDir - 1;
    int nF2 = ISM->GetNFrames( gpID2 )/c_NDir - 1;
    
    float fps = 20.0f;

    float cx = 126;
    float cy = 153;

    static Timer timer;
    static float t = timer.seconds();
    float curT = timer.seconds();
    float dt = t - curT;
    t = curT;    

    int dir = int( t )%16;

    ISM->SetCurrentDiffuse( 0xFFFFFFFF );
    ISM->SetCurrentShader( shID );
    ISM->DrawWSprite( gpID1, dir + 16*(int( t*fps + dir )%nF1), Vector3D( 100, 100, 0 ), false, 0xFF0000FF );
    ISM->DrawWSprite( gpID2, dir + 16*(int( t*fps + dir )%nF2), Vector3D( 120, 100, 0 ), false, 0xFF0000FF );
 
    //ISM->DrawWSprite( gpID1, 545, Vector3D( 100, 100, 0 ), false, 0xFF0000FF );
    
    ISM->Flush();
    
    rsFlush();

} // TestGP2

void TestModelInstancing()
{
    static int mdlID  = IMM->GetModelID( "Units\\UndHKni\\UndHKniM.c2m" );
    static int anmID = IMM->GetModelID( "Units\\UndHKni\\UndHKniA_walk1.c2m" );
    float totalTime = IMM->GetAnimTime( anmID );
    static Timer timer( totalTime );

    Matrix4D tm;
    tm.srt( 0.2f, Vector3D::oZ, c_PI, Vector3D( 200.0f, 200.0f, 0.0f ) );

    PushEntityContext( 0xDEADBEEF );
    float t = fmodf( timer.seconds()*1000.0f, totalTime );
    
    IMM->StartModel( mdlID, tm );
    IMM->AnimateModel( anmID, t );
    IMM->DrawModel();

    PopEntityContext();
} // TestModelInstancing

void TestAnimationBlending()
{
    //static int mdlID  = IMM->GetModelID( "Units\\UndHKni\\UndHKniM" );
    //static int anm1ID = IMM->GetModelID( "Units\\UndHKni\\UndHKniA_walk1" );
    //static int anm2ID = IMM->GetModelID( "Units\\UndHKni\\UndHKniA_attack1" );

    static int mdlID  = IMM->GetModelID( "Units\\DruUUni\\DruUUniM" );
    static int anm1ID = IMM->GetModelID( "Units\\DruUUni\\DruUUniA_stand1" );
    static int anm2ID = IMM->GetModelID( "Units\\DruUUni\\DruUUniA_walk1" );

    SNode* pRoot1 = IMM->GetModelRoot( anm1ID );
    SNode* pRoot2 = IMM->GetModelRoot( anm2ID );

    SNode::Iterator it1( pRoot1 );
    while (it1)
    {
        SNode::Iterator it2( pRoot2 );
        bool bHasPair = false;
        while (it2)
        {
            if (!stricmp( (*it1)->GetName(), (*it2)->GetName() )) bHasPair = true;
            ++it2;
        }
        if (!bHasPair)
        {
            OutputDebugString( (*it1)->GetName() );
            OutputDebugString( "\n" );
        }
        ++it1;
    }

    static bool bAnm1 = false;
    static int cAnm = anm1ID;
    static Timer timer( 0.00001f );
    if (timer.iteration() > 0)
    {
        if (!bAnm1)
        {
            timer.start( IMM->GetAnimTime( anm2ID )/1000.0f );
            cAnm = anm2ID;
            bAnm1 = true;
        }
        else
        {
            timer.start( IMM->GetAnimTime( anm1ID )/1000.0f );
            cAnm = anm1ID;
            bAnm1 = false;
        }
    }

    Matrix4D tm1;
    Matrix4D tm2;
    tm1.srt( 0.5f, Vector3D::oZ, 1.0, Vector3D( 200.0f, 200.0f, 0.0f ) );
    tm2.srt( 0.5f, Vector3D::oZ, 1.0, Vector3D( 200.0f, 200.0f, 0.0f ) );

    Matrix4D* pTM = bAnm1 ? &tm1 : &tm2;

    IMM->StartModel( mdlID, *pTM, 0xDEADB00F );
    IMM->AnimateModel( cAnm, timer.seconds()*1000.0f );
    IMM->DrawModel();
    IRS->ResetWorldTM();
} // TestAnimationBlending

void TestAABBGeometry()
{
    AABoundBox aabb( Rct( -200.0f, -200.0f, 400.0f, 600.0f ), 10.0f, 100.0f );
    static Geometry* pGeom = new Geometry();
    BaseMesh& bm = pGeom->GetMesh();
    bm.createAABB( aabb );
    IRS->SetShader( IRS->GetShaderID( "lines3D" ) );
    pGeom->Render();
}

void TestWater()
{
    static int mID1 = IMM->GetModelID( "Models\\Ships\\UndSSp1M_stand.c2m" );
    static int mID2 = IMM->GetModelID( "Models\\Ships\\UndSSp2M_stand.c2m" );
    static int mID3 = IMM->GetModelID( "Models\\Ships\\UndSSp3M_stand.c2m" );

    Matrix4D tm1, tm2, tm3;
    tm1.srt( 1.0f, Vector3D::oZ, 2.0f,  Vector3D( 500.0, 200.0f, 0.0f ) );
    tm2.srt( 1.0f, Vector3D::oZ, -2.0f, Vector3D( -500.0, 600.0f, 0.0f ) );
    tm3.srt( 1.0f, Vector3D::oZ, -1.0f, Vector3D( 700.0, 1100.0f, 0.0f ) );

    IRMap->AddObject( mID1, &tm1 );
    IRMap->AddObject( mID2, &tm2 );
    IRMap->AddObject( mID3, &tm3 );
    IRMap->Render();

    IShadowMgr->AddCaster( mID1, tm1 );
    IShadowMgr->AddCaster( mID2, tm2 );
    IShadowMgr->AddCaster( mID3, tm3 );
    IShadowMgr->Render();

    ITerra->Render();    
    IWater->Render();

    IMM->StartModel( mID1, tm1, mID1 );
    IMM->DrawModel();

    IMM->StartModel( mID2, tm2, mID2 );
    IMM->DrawModel();
    
    IMM->StartModel( mID3, tm3, mID3 );
    IMM->DrawModel();

} // TestWater

void TestEffects()
{
    int efID[3];
    efID[0] = IMM->GetModelID( "Models\\Effects\\Weapons\\Strela\\Strela_01.c2m" );
    efID[1] = IMM->GetModelID( "Models\\Effects\\Weapons\\fire_fairy\\enchanting_ball_01.c2m" );
    efID[2] = IMM->GetModelID( "Models\\Effects\\Weapons\\Katapult\\UndedKatapult_Vz.c2m" );

    rndInit( 0xDEADBEEF );
    const int c_NInst = 2000;
    for (int i = 0; i < c_NInst; i++)
    {
        Matrix4D tm;
        tm.translation( rndValuef( -1000.0f, 1000.0f ), 
                        rndValuef( -1000.0f, 1000.0f ), 
                        0.0f );
        int ef = efID[rndValue()%3];
        IMM->StartModel( ef, tm, i );
        IMM->DrawModel();
    }

    IEffMgr->Evaluate();
    IEffMgr->PreRender();
    IEffMgr->PostRender();
    IEffMgr->PostEvaluate();
} // TestEffects

void TestVertex2F()
{
    BaseMesh bm;
    bm.create( 3, 0, vfVertex2F, ptTriangleList );
    Vertex2F* v = (Vertex2F*)bm.getVertexData();
    
    v[0].diffuse  = 0x00000000;
    v[0].specular = 0x00000000;

    v[1].diffuse  = 0x00800000;
    v[1].specular = 0x00800000;

    v[2].diffuse  = 0x00800000;
    v[2].specular = 0x00000000;

    bm.setNVert( 3 );
    bm.setNPri( 1 );

    static int shID = IRS->GetShaderID( "test2f" );
    IRS->SetShader( shID );
    DrawPrimBM( bm );
} // TestVertex2F

void TestPrimitives()
{
    DrawSphere( Sphere( Vector3D( 200.0f, 200.0f, 200.0f ), 50.0f ), 0x554444FF, 0xFF4444FF );
    DrawCapsule( Capsule( Vector3D( 200.0f, -200.0f, 200.0f ), Vector3D::oZ, 30.0f ), 0x554444FF, 0xFF4444FF );
}

void TestBuildingCollapse()
{
    static int mID= -1;
    static AABoundBox aabb;
    if (mID == -1)
    {
        mID = IMM->GetModelID( "Models\\BuildingsNeutral\\livre_necro_gauche_R_1024x512\\temp.c2m" );
        SNode* pModel = IMM->GetModelRoot( mID );
        SNode::Iterator it( pModel );
        Rct rcUV( -1, -1, 3, 3 );
        while (it)
        {
            Geometry* pGeom = (Geometry*)*it;
            if (pGeom->IsA<Geometry>() && !pGeom->HasName( "Leafs" ))
            {
                aabb = pGeom->GetAABB();
                BaseMesh& bm = pGeom->GetMesh();
                int nV = bm.getNVert();
                Vertex2F* pV = new Vertex2F[nV];
                VertexN* v = (VertexN*)bm.getVertexData();
                assert(bm.getVertexFormat() == vfVertexN);
                for (int i = 0; i < nV; i++)
                {
                    VertexN& sv = v[i];
                    Vertex2F& dv = pV[i];
                    float x = 255.0f*(sv.x - aabb.minv.x)/(aabb.maxv.x - aabb.minv.x);
                    float y = 255.0f*(sv.y - aabb.minv.y)/(aabb.maxv.y - aabb.minv.y);
                    float z = 65535.0f*(sv.z - aabb.minv.z)/(aabb.maxv.z - aabb.minv.z);
                    float u = 65535.0f*(sv.u - rcUV.x)/rcUV.w;
                    float v = 65535.0f*(sv.v - rcUV.y)/rcUV.h;
                    DWORD wx = (DWORD)x;
                    DWORD wy = (DWORD)y;
                    DWORD wz = (DWORD)z;
                    DWORD wu = (DWORD)u;
                    DWORD wv = (DWORD)v;
                    dv.diffuse  = (wx << 24) | (wy << 16) | wz;
                    dv.specular = (wu << 16) | wv;
                }
                bm.setVertexFormat( vfVertex2F );
                bm.setVertexPtr( pV );
            }
            ++it;
        }
        FOutStream os( "Models\\BuildingsNeutral\\livre_necro_gauche_R_1024x512\\out.c2m" );
        pModel->SerializeSubtree( os );
    }
    
    Matrix4D m( aabb.minv, aabb.maxv, Vector3D::null, Vector3D::null );
    IRS->SetTextureTM( m );
    Matrix4D tm;
    tm.translation( 300.0f, 0.0f, 0.0f );
    IMM->StartModel( mID, tm, 0xDEADBEEF );
    IMM->DrawModel();
} // TestBuildingCollapse

void SaveToVRML( DWORD mID, const char* path );
void TestVRML()
{
    static int mID = -1;
    if (mID == -1)
    {
        mID = IMM->GetModelID( "Models\\BuildingsNeutral\\livre_necro_gauche_R_1024x512\\livre_necro_gauche_R_1024x512.c2m" );
        SaveToVRML( mID, "Models\\BuildingsNeutral\\livre_necro_gauche_R_1024x512\\livre_necro_gauche_R_1024x512.wrl" );    
    }
} // TestVRML

void TestAnimation()
{
    static int mdlID  = IMM->GetModelID( "Units\\UndHKni\\UndHKniM" );
    static int anmID[2];
    anmID[0] = IMM->GetModelID( "Units\\UndHKni\\UndHKniA_walk1" );
    anmID[1] = IMM->GetModelID( "Units\\UndHKni\\UndHKniA_attack1" );

    static Timer timer( 0.0f );

    float t[2];
    t[0] = IMM->GetAnimTime( anmID[0] );
    t[1] = IMM->GetAnimTime( anmID[1] );

    const int c_NMdl = 100;
    
    float cTime = timer.seconds()*1000.0f;

    rndInit( 0xDEADBEEF );
    for (int i = 0; i < c_NMdl; i++)
    {
        Vector3D pos; pos.random( -1000.0f, 1000.0f, -1000.0f, 1000.0f, 0.0f, 0.0f );
        Matrix4D tm; tm.st( Vector3D( 0.2f, 0.2f, 0.2f ), pos );
        int k = rndValue()%2;
        float ct = fmodf( cTime, t[k] );
        IShadowMgr->AddCaster( mdlID, tm );
        IMM->StartModel( mdlID, tm, i );
        IMM->AnimateModel( anmID[k], ct );
        IMM->DrawModel();
    }

    //IShadowMgr->Render();
    //ITerra->Render();
} // TestAnimation

void TestCreateShell()
{
    int mID = IMM->GetModelID( "Models\\NeutralObjects\\D_Sunduk01\\D_Sunduk01.C2M" );
    int aID = IMM->GetModelID( "Models\\NeutralObjects\\D_Sunduk01\\D_Sunduk01_A.C2M" );

    
    AABoundBox aabb = IMM->GetBoundBox( mID );
    Model* pRoot = dynamic_cast<Model*>( IMM->GetModelRoot( mID ) );
    
    Matrix4D tm;
    tm.translation( 300, 100, 200 );
    
    static Timer timer;
    float t = fmodf( timer.seconds()*1000.0f, IMM->GetAnimTime( aID ) );

    IMM->StartModel( mID, tm );
    IMM->AnimateModel( aID, t );
    IMM->DrawModel();

    DWORD clr = 0x77FF0000;
    POINT pt;
    GetCursorPos( &pt );
    float dist = 0.0f;
    if (IMM->Intersects( pt.x, pt.y, mID, tm, dist )) clr = 0x77FFFF00;

    int nS = pRoot->GetShellSize();
    for (int i = 0; i < nS; i++)
    {
        AABoundBox aabb = pRoot->GetShellElem( i );
        aabb.Transform( tm );
        DrawAABB( aabb, 0, clr );
    }
} // TestCreateShell

void DrawSpriteAsBillboard( int spID, int frID,       
                            const Vector3D& pos,      
                            const Matrix3D& camTM,    
                            int cx, int cy,    
                            DWORD color,
                            float scale ); 

void DrawSpriteAsShadow(    int spID, int frID,        
                            const Vector3D& lightDir,  
                            const Vector3D& pos,       
                            int cx, int cy,            
                            float scale );              


int CalcDirIndex( const Vector3D& dir, int nSectors )
{
    float ang = atan2f( dir.y, dir.x );
    ang -= c_PI/float( nSectors );
    int d = float( nSectors )*(c_PI + c_HalfPI - ang)/c_DoublePI;
    return d%nSectors;
} // CalcDirIndex


Vector3D g_ViewDir;
Vector3D m_LightDir;
Matrix3D g_CamTM;
DWORD    g_Diffuse      = 0xFF808080;
DWORD    m_ShadowColor  = 0xAA222222;
float    g_Scale        = 1.0f;

void DrawShadowedUnit( int gpID, int frameID, 
                        const Vector3D& pos, 
                        float cx, float cy,
                        float dir, DWORD natColor, 
                        int nDirSectors )
{
    static int shID0 = IRS->GetShaderID( "sprite_bboard_vs" );
    static int shID1 = IRS->GetShaderID( "sprite_shadow_vs" );

    int lFrame = CalcDirIndex( m_LightDir, nDirSectors ) + frameID*nDirSectors;
    int bFrame = CalcDirIndex( g_ViewDir, nDirSectors )  + frameID*nDirSectors;

    ISM->SetCurrentShader   ( shID1 );
    ISM->SetCurrentDiffuse  ( m_ShadowColor );
    DrawSpriteAsShadow      ( gpID, lFrame, m_LightDir, pos, cx, cy, g_Scale ); 
    ISM->Flush();
    
    ISM->SetCurrentDiffuse  ( g_Diffuse );
    ISM->SetCurrentShader   ( shID0 );
    DrawSpriteAsBillboard   ( gpID, bFrame, pos, g_CamTM, cx, cy, natColor, g_Scale );
    ISM->Flush();
} // DrawShadowedUnit

class Formation
{
    int         m_VSlots;       //  vertical slots
    int         m_HSlots;       //  horisontal slots
    
    float       m_Width;    
    float       m_Height;   

    Vector3D    m_Pos;          //  current center position
    float       m_Dir;          //  current facing direction

    Vector3D    m_TargetPos;    
    float       m_MoveSpeed;

    Matrix4D    m_FormTM;

public:
    Formation();

    Vector3D    GetSlotPos  ( int idx ) const;
    int         GetNSlots   () const { return m_VSlots*m_HSlots; }
    void        SetTarget   ( const Vector3D& target );
    float       GetDirection() const { return m_Dir; }
    void        Update      ( float dt );

}; // class Formation

Formation::Formation() 
{
    m_Width     = 2000.0f;
    m_Height    = 2000.0f; 
    m_VSlots    = 10;
    m_HSlots    = 10;
    m_MoveSpeed = 100.0f;
    m_Dir       = 0.0f;
    m_Pos       = Vector3D( 100, -300, 0 );
    m_TargetPos = m_Pos;
} // Formation::Formation

void Formation::SetTarget( const Vector3D& target ) 
{ 
    m_TargetPos = target;
    Vector3D dir( target );
    dir -= m_Pos;
    dir.normalize();
    m_Dir = atan2f( dir.y, dir.x );
} // Formation::SetTarget

void Formation::Update( float dt )
{
    m_FormTM.srt( 1.0f, Vector3D::oZ, m_Dir, m_Pos );
    Vector3D moveDir( m_TargetPos );
    moveDir -= m_Pos;
    moveDir.normalize();
    m_Dir = atan2f( moveDir.y, moveDir.x );
    moveDir *= m_MoveSpeed*dt;
    m_Pos += moveDir;
} // Formation::Update

Vector3D Formation::GetSlotPos( int idx ) const
{
    if (idx < 0 || idx >= GetNSlots()) return Vector3D::null;
    int row = idx/m_HSlots;
    int col = idx%m_HSlots;
    float cx = -m_Width*0.5f  + float( col*m_Width )/m_HSlots;
    float cy = -m_Height*0.5f + float( row*m_Height )/m_VSlots;
    Vector3D pos( cx, cy, 0.0f ); 
    m_FormTM.transformPt( pos );
    pos.z = ITerra->GetH( pos.x, pos.y );
    return pos;
} // Formation::GetSlotPos

Formation g_Form;
void TestUnitShadow()
{
    static int gpID = ISM->GetPackageID( "G\\RatG" );

    int nFTotal = ISM->GetNFrames( gpID );
    const int c_NDir = 16;
    int nF = nFTotal/c_NDir - 1;
    float fps = 20.0f;

    g_CamTM = GetCamera()->GetWorldTM();
    static float phi = float( GetTickCount() * 0.001f );
    m_LightDir = Vector3D( cosf( phi ), sinf( phi ), -0.5 );
    m_LightDir.normalize();
    g_ViewDir = Vector3D( g_CamTM.e20, g_CamTM.e21, g_CamTM.e22 );


    float cx = 126;
    float cy = 153;

    static Timer timer;
    static float t = timer.seconds();
    float curT = timer.seconds();
    float dt = t - curT;
    t = curT;
    g_Form.Update( dt );

    for (int i = 0; i < g_Form.GetNSlots(); i++)
    {
        rndInit( i );
        int frame = int( t*fps + rndValue( 0, nF ) )%nF;
        Vector3D pos = g_Form.GetSlotPos( i );
        DrawShadowedUnit( gpID, frame, pos, cx, cy, 0.0f, 0xFFFF0000, c_NDir );
    }
    rsFlush();
} // TestUnitShadow

void TestShaderParam()
{
    static int mdlID = IMM->GetModelID( "Units\\UndHKni\\test.c2m" );
    static int shID = IRS->GetShaderID( "test_shader" );
    static int vID1 = IRS->GetShaderVarID( shID, "shift1" );
    static int vID2 = IRS->GetShaderVarID( shID, "shift2" );

    float shift1 = sinf( float( GetTickCount() )/500.0f )*200.0f;
    float shift2 = sinf( float( GetTickCount() + 400 )/500.0f )*200.0f;
    IRS->SetShaderVar( shID, vID1, shift1 );
    IRS->SetShaderVar( shID, vID2, shift2 );

    IMM->StartModel( mdlID );
    IMM->DrawModel();
} // TestShaderParam

void TestMultipleModels()
{
    static int mdlID = IMM->GetModelID( "Units\\UndHKni\\UndHKniM.c2m" );
    static int anmID = IMM->GetModelID( "Units\\UndHKni\\UndHKniA_walk1.c2m" );

    int mID[3];
    int aID[3];

    mID[0] = IMM->GetModelID( "Ships\\UnsSSh1M.c2m" );
    mID[1] = IMM->GetModelID( "Ships\\UnsSSh2M.c2m" );
    mID[2] = IMM->GetModelID( "Ships\\UnsSSh3M.c2m" );

    aID[0] = IMM->GetModelID( "Ships\\UnsSSh1A_stand1.c2m" );
    aID[1] = IMM->GetModelID( "Ships\\UnsSSh2A_stand1.c2m" );
    aID[2] = IMM->GetModelID( "Ships\\UnsSSh3A_stand1.c2m" );

    static Timer timer;
    static float t;
    t = timer.seconds();
    
    static float ft = timer.seconds();
    float curT = timer.seconds();
    float dt = ft - curT;
    ft = curT;
    g_Form.Update( dt );

    for (int i = 0; i < g_Form.GetNSlots(); i++)
    {
        rndInit( i );
        Vector3D pos = g_Form.GetSlotPos( i );
        Matrix4D tm;
        tm.st( 0.3f, pos );

        PushEntityContext( i );
        
        IShadowMgr->AddCaster( mdlID, tm );
        rndInit( i );
        int is = rndValue( 0, 3 );
        IMM->StartModel( mdlID, tm, NO_CONTEXT );
        float anmT = fmodf( t*1000.0f /*+ rndValuef( 0.0f, anmTime )*/, 
                            IMM->GetAnimTime( anmID ) );
        IMM->AnimateModel( anmID, anmT );
        IMM->DrawModel();
        
        PopEntityContext();
    }
} // TestMultipleModels

void TestMultiPass()
{
    static int shID = IRS->GetShaderID( "testmp" );
    
} // TestMultiPass

void TestMultiAnimation()
{
    static int mdlID = IMM->GetModelID( "Ships\\MecSIroM.c2m" );
    
    int aID[3];
    aID[0] = IMM->GetModelID( "Ships\\MecSIroA_stand1.C2M" );
    aID[1] = IMM->GetModelID( "Ships\\MecSIroA_turret1_rotate.C2M" );
    aID[2] = IMM->GetModelID( "Ships\\MecSIroA_turret1_shot.C2M" );

    static Timer timer;
    static float t;
    t = timer.seconds();

    PushEntityContext( 0xFF00FF00 );

    Matrix4D tm;
    tm.translation( 400, 400, 0 );

    IShadowMgr->AddCaster( mdlID, tm );
    rndInit( 10 );
    int is = rndValue( 0, 3 );
    IMM->StartModel( mdlID, tm, NO_CONTEXT );
    
    float anmT = fmodf( t*1000.0f, IMM->GetAnimTime( aID[0] ) );
    IMM->AnimateModel( aID[0], anmT );

    anmT = fmodf( t*100.0f, IMM->GetAnimTime( aID[1] ) );
    IMM->AnimateModel( aID[1], anmT );

    anmT = fmodf( t*1000.0f, IMM->GetAnimTime( aID[2] ) );
    IMM->AnimateModel( aID[2], anmT );

    IMM->DrawModel();

    PopEntityContext();
} // TestMultiAnimation

void TestEffectTransform()
{
    static int effID = IMM->GetModelID( "Models\\Effects\\Weapons\\Mech Enforcer_tekUm\\Mech Enforcer_StvolUm" );
    
    Matrix4D tm1;
    tm1.srt( 0.2f, Vector3D::oZ, c_HalfPI, Vector3D( 200.0f, 200.0f, 0.0f ) );
    
} // TestEffectTransform

void TestScene()
{
    static int mdlID = IMM->GetModelID( "Units\\UndHKni\\UndHKniM.c2m" );
    static int anmID = IMM->GetModelID( "Units\\UndHKni\\UndHKniA_stand2.c2m" );
} // TestScene

void TestDXT1()
{
    int tID = IRS->GetTextureID( "dxt3" );
    int tID1 = IRS->GetTextureID( "dxt3C" );
    IRS->SetTexture( tID );
    IRS->SetTexture( tID1 );
    ColorFormat cf1 = IRS->GetTextureFormat( tID );
    ColorFormat cf2 = IRS->GetTextureFormat( tID1 );
} // TestDXT1

void TestLog()
{
    //Log.Warning( "Log Message!!!" );
} // TestLog
DWORD meCurInstID = 12345;
extern bool _dbgDrawBonesMode;
void ModelEditor::Render()
{    
    if (!m_bInited) Init();

    m_Selection.Render();
    IMM->SetAttachedEffectMask(0xFFFFFFFF);
    _dbgDrawBonesMode=GetKeyState(VK_SCROLL)!=0 ;

    //IRS->SetFog( 0, 0.0f, 10000.0f, 0, 0 );

    m_pSelectedNode = (SNode*)m_TreeL.GetSelectedNode();
    IReflected* pInspectorNode = m_pSelectedNode;
    IReflected* pExposed = dynamic_cast<IReflected*>( pInspectorNode );
    m_Inspector.Bind( pExposed );
    //  set camera
    if (m_ActiveCamera == aecEditor)
    {
        m_EditorCamera.Render();
    }
    else if (m_ActiveCamera == aecGamePersp)
    {
        m_GamePerspCamera.Render();
    }
    else if (m_ActiveCamera == aecGameOrtho)
    {
        m_GameOrthoCamera.Render();
    }
    
    if (m_bShowBackdrop) 
    {
        ISky->Render();
    }   

    float anmT = m_AnimTimer.seconds()*1000.0f;
    
    if (m_bShowBackdrop) IShadowMgr->AddCaster( m_ModelID, Matrix4D::identity );

    //  set national color/transparency
    ColorValue c( m_ModelColor );
    c.a = m_ModelTransparency;
    IRS->SetTextureFactor( c );

    if (m_bShowBackdrop) 
    {
        IShadowMgr->Render();
        ITerra->Render();
    }

    //  render grid
    if (m_bShowGrid) DrawGrid();

    IRS->SetTextureFactor( c );

	void TestObj();
	TestObj();

	Matrix4D mi=Matrix4D::identity;	
	if(GetKeyState('R')&0x8000){
		mi.rotation(Vector3D::oZ,GetTickCount()/2000.0f);
	}
	IRS->SetShaderConst(0,50000);
	if(GetKeyState('I')&0x8000){		
		IMM->StartModel( m_ModelID, Matrix4D::identity,meCurInstID);
		IMM->AnimateModel( m_AnimID, anmT );
		if(m_AnimID1>0)IMM->AnimateModel( m_AnimID1, anmT,0 );
		if(m_AnimID2>0)IMM->AnimateModel( m_AnimID2, anmT,0 );
		//for(int i=-6;i<=6;i++){
			//for(int j=-6;j<=6;j++){
				//mi.srt(1,Vector3D::oZ,i+j*1.56+GetTickCount()/2000.0f,Vector3D(i*200,j*200,0));
				//IRS->SetWorldTM(mi);
				IMM->DrawModel();
			//}
		//}
	}else
	if(false){//GetKeyState('M')){
		for(int i=-6;i<=6;i++){
			for(int j=-6;j<=6;j++){
				mi.srt(1,Vector3D::oZ,i+j*1.56+GetTickCount()/2000.0f,Vector3D(i*200,j*200,0));
				IMM->StartModel( m_ModelID, mi,meCurInstID+i+j*256 );
				IMM->AnimateModel( m_AnimID, anmT );
				if(m_AnimID1>0)IMM->AnimateModel( m_AnimID1, anmT,0 );
				if(m_AnimID2>0)IMM->AnimateModel( m_AnimID2, anmT,0 );
				IMM->DrawModel();
			}
		}
	}else{
		IMM->StartModel( m_ModelID, mi,meCurInstID );
		IMM->AnimateModel( m_AnimID, anmT );
		if(m_AnimID1>0)IMM->AnimateModel( m_AnimID1, anmT,0 );
		if(m_AnimID2>0)IMM->AnimateModel( m_AnimID2, anmT,0 );
		IMM->DrawModel();
	}

    IEffMgr->Evaluate();
    IEffMgr->PreRender();    
    IEffMgr->PostRender();
    IEffMgr->PostEvaluate();
    

    if (m_bDrawMeasure)
    {
        char str[256];
        sprintf( str, "Position: %.04f %.04f %.04f", m_MeasureTM.e30, m_MeasureTM.e31, m_MeasureTM.e32 );
        DrawText( 10, 60, 0xFFFFFF00, str );
        FlushText();
    }

    ///IWater->Render();

    DrawSkeleton    ();
    DrawBoundBoxes  ();
    DrawBoundSpheres();
    DrawSlots       ();

    if (m_bShowCameras)             DrawCameras     ();
    if (m_bShowLights)              DrawLights      ();
    if (m_bShowLocators)            DrawLocators    ();
    if (m_bShowTransformNodes)      DrawTransforms  ();
    if (m_bShowNormals)             DrawNormals     ();
    if (m_bShowModelStats)          DrawModelStats  ();

    //TestThumbnail();
    //TestScene();
    //TestEffectTransform();
    //TestUnitShadow();
    //TestMultipleModels();
    //TestShaderParam();
    //TestCreateShell();
    //TestAnimation();
    //TestPrimitives();
    ///TestAABBGeometry();
    //TestAnimationBlending();
    //TestPhysics();
    //TestWater();
    //TestEffects();
    ///TestMultiAnimation();
    //TestVertex2F();
    //TestBuildingCollapse();
    //TestVRML();
    //TestModelInstancing();
    //TestGP2();
    //TestGPMagic();
    //TestMultiPass();
    //TestDXT1();
    //TestLog();
} // ModelEditor::Render

const float c_RightB = 100.0f;
const float c_NumB   = 30.0f;
const float c_TopB   = 30.0f;
const float c_Height = 12.0f;   
void ModelEditor::DrawModelStats()
{
    int nPoly       = 0;
    int nVertices   = 0;
    int nShaders    = 0;
    int nTextures   = 0;
    int nBones      = 0;
    int nSubmeshes  = 0;
    int nVert1W     = 0;
    int nVert2W     = 0;
    int nVert3W     = 0;
    int nVert4W     = 0;
    
    float cX = IRS->GetViewPort().w - c_RightB;
    float cN = c_NumB;
    float cY = c_TopB;
    
    DrawText( cX, cY, 0xFFCCCCFF, "Polygons:" );
    cY += c_Height;
    
    DrawText( cX, cY, 0xFFCCCCFF, "Vertices:" );
    cY += c_Height;

    DrawText( cX, cY, 0xFFCCCCFF, "Shaders:" );
    cY += c_Height;

    DrawText( cX, cY, 0xFFCCCCFF, "Textures:" );
    cY += c_Height;

    DrawText( cX, cY, 0xFFCCCCFF, "Bones:" );
    cY += c_Height;

    DrawText( cX, cY, 0xFFCCCCFF, "Meshes:" );
    cY += c_Height;

    DrawText( cX, cY, 0xFFCCCCFF, "Vert1W:" );
    cY += c_Height;

    DrawText( cX, cY, 0xFFCCCCFF, "Vert2W:" );
    cY += c_Height;

    DrawText( cX, cY, 0xFFCCCCFF, "Vert3W:" );
    cY += c_Height;

    DrawText( cX, cY, 0xFFCCCCFF, "Vert4W:" );
    cY += c_Height;
    
    FlushText();
} // ModelEditor::DrawModelStats

void ModelEditor::DrawGrid()
{
    IRS->ResetWorldTM();
    ::DrawGrid( m_GridSide, m_NGridCells, m_GridColor, m_GridSubColor );
} // ModelEditor::DrawGrid

void ModelEditor::DrawNormals()
{
} // ModelEditor::DrawNormals

void ModelEditor::DrawLights()
{
    /*IRS->ResetWorldTM();
    IReflected::Iterator dit( m_pModel, DirectionalLight::FnFilter );
    while (dit)
    {
        DirectionalLight* pLight = (DirectionalLight*)(IReflected*)dit;

        Matrix4D lightTM = pLight->GetWorldTM();
        Ray3D ray = pLight->GetLightRay();
        ray.Normalize();
        if (!pLight->IsInvisible()) DrawRay( ray, 0xFFFFFF00 );
        ++dit;
    }

    IRS->ResetWorldTM();
    IReflected::Iterator pit( m_pModel, PointLight::FnFilter );
    while (pit)
    {
        PointLight* pLight = (PointLight*)(IReflected*)pit;
        Sphere sp = pLight->GetLightSphere();
        sp.SetRadius( 40.0f );
        
        if (!pLight->IsInvisible()) DrawStar( sp, 0xFFFFFF00, 0x00FFFF00, 16 );
        ++pit;
    }

    rsFlushPoly3D();
    rsFlushLines3D();*/
} // ModelEditor::DrawLights

void ModelEditor::DrawTransforms()
{
    IRS->ResetWorldTM();
    /*IReflected::Iterator it( m_pModel, TransformNode::FnFilter );
    while (it)
    {
        TransformNode* pNode = (TransformNode*)(IReflected*)it;
        if (pNode->IsA<Locator>()) { ++it; continue; }

        Matrix4D wTM = pNode->GetTopTM();
        Vector3D pos = wTM.getTranslation();
        AABoundBox box( pos, m_HandleSide );
        DrawAABB( box, 0, 0xFFFFFF00 );
        Vector3D posX( pos ); 
        posX.addWeighted( Vector3D( wTM.e00, wTM.e01, wTM.e02 ), m_HandleSide * 3.0f );
        Vector3D posY( pos );
        posY.addWeighted( Vector3D( wTM.e10, wTM.e11, wTM.e12 ), m_HandleSide * 3.0f );
        Vector3D posZ( pos );
        posZ.addWeighted( Vector3D( wTM.e20, wTM.e21, wTM.e22 ), m_HandleSide * 3.0f );
        rsLine( pos, posX, 0xFF000044, 0xFF000044 );
        rsLine( pos, posY, 0xFF00FF00, 0xFF00FF00 );
        rsLine( pos, posZ, 0xFF0000FF, 0xFF0000FF );
        ++it;
    }

    rsFlushPoly3D();
    rsFlushLines3D();*/
} // ModelEditor::DrawTransforms

void ModelEditor::DrawLocators()
{
    IRS->ResetWorldTM();
    /*IReflected::Iterator it( m_pModel, Locator::FnFilter );
    while (it)
    {
        Locator* pNode = (Locator*)(IReflected*)it;

        Matrix4D wTM = pNode->GetWorldTM();
        Vector3D pos = wTM.getTranslation();
        AABoundBox box( pos, m_HandleSide );
        DrawAABB( box, 0, 0xFFFFFFFF );
        Vector3D posX( pos ); 
        posX.addWeighted( Vector3D( wTM.e00, wTM.e01, wTM.e02 ), m_HandleSide * 3.0f );
        Vector3D posY( pos );
        posY.addWeighted( Vector3D( wTM.e10, wTM.e11, wTM.e12 ), m_HandleSide * 3.0f );
        Vector3D posZ( pos );
        posZ.addWeighted( Vector3D( wTM.e20, wTM.e21, wTM.e22 ), m_HandleSide * 3.0f );
        rsLine( pos, posX, 0xFF000044, 0xFF000044 );
        rsLine( pos, posY, 0xFF00FF00, 0xFF00FF00 );
        rsLine( pos, posZ, 0xFF0000FF, 0xFF0000FF );
        ++it;
    }

    rsFlushPoly3D();
    rsFlushLines3D();*/
} // ModelEditor::DrawLocators

void ModelEditor::DrawEffectEmitters()
{

}

void ModelEditor::DrawCameras()
{
    //IReflected::Iterator it( m_pModel, BaseCamera::FnFilter );
    //while (it)
    //{
    //    BaseCamera* pCam = (BaseCamera*)(IReflected*)it;
    //    
    //    //  render frustum
    //    Frustum frustum;
    //    pCam->GetWorldSpaceFrustum( frustum );
    //    IRS->ResetWorldTM();
    //    DrawFrustum( frustum, m_FrustumColor, m_FrustumLinesColor, true );
    //    
    //    ++it;
    //}

    //rsFlushPoly3D();
    //rsFlushLines3D();
} // ModelEditor::DrawCameras

void ModelEditor::DrawBoundSpheres()
{
    IRS->ResetWorldTM();
    
} // ModelEditor::DrawBoundSpheres

void ModelEditor::DrawBoundBoxes()
{
    IRS->ResetWorldTM();
    
} //  ModelEditor::DrawAABB

const float c_HandleSide = 2.0f;
void ModelEditor::DrawSlots()
{
    
} // ModelEditor::DrawSlots

void ModelEditor::DrawSkeleton()
{
    IRS->ResetWorldTM();
    /*rsEnableZ( false );
    float hside = c_HandleSide;
    ModelObject* pModel = IModelMgr->GetModel();
    if (!m) return;
    int nBones = m->GetNBones();
    for (int i = 0; i < nBones; i++)
    {
        BoneInstance* pBone = m->GetBoneInstance( i );
        if (!pBone) continue;
        const Matrix4D wtm = m->GetBoneWorldTM( i );
        
        if (pModel->m_Skeleton.DoDrawBones())
        {
            DrawCircle8( wtm.getTranslation(), wtm.getV0(), hside, 0, 0xCCAA0000 );
            DrawCircle8( wtm.getTranslation(), wtm.getV1(), hside, 0, 0xCC00AA00 );
            DrawCircle8( wtm.getTranslation(), wtm.getV2(), hside, 0, 0xCC0000AA );

            BoneInstance* pParent = pBone->GetParent();
            if (pParent && pParent->GetID() != 0)
            {
                const Matrix4D ptm = pParent->GetWorldTM();
                DWORD clrStart = 0xCCFFFF00;
                DWORD clrEnd = 0x33FFFF00;
                rsLine( wtm.getTranslation(), ptm.getTranslation(), clrStart, clrEnd );
            }
        }
        
        if (pModel->m_Skeleton.DoDrawBoneLabels())
        {
            DrawText( wtm.getTranslation(), 0xFFFFFF00, pModel->m_Skeleton[i].GetName() );
        }
    }
    rsFlushLines3D();
    FlushText();*/
} // ModelEditor::DrawBones
    
void ModelEditor::DeleteNode()
{
    if (m_pSelectedNode) 
    {
        SNode* pParent = m_pSelectedNode->GetParent();
        if (pParent) 
        {
            pParent->DelChild( m_pSelectedNode );
            m_pSelectedNode = pParent;
        }
    }
} // ModelEditor::DeleteNode

void ModelEditor::CycleActiveCamera()
{
    int cCam = (int)m_ActiveCamera;
    cCam++;
    m_ActiveCamera = (ActiveEditorCamera)cCam;
    if (m_ActiveCamera >= aecLAST) m_ActiveCamera = aecEditor;
}

void ModelEditor::InsertNode()
{
    //if (keyCode == VK_INSERT)
    //{
    //    m_bInsertMode = true;
    //    POINT pt;
    //    GetCursorPos( &pt );
    //    CheckInsertMode( pt.x, pt.y );
    //}
} // ModelEditor::InsertNode

void ModelEditor::ToggleHideModel()
{
    //m_pModel->SetInvisible( !m_pModel->IsInvisible() );
} // ModelEditor::ToggleHideModel

void ModelEditor::ApplyTranslateTool()
{
} // ModelEditor::ApplyTranslateTool

void ModelEditor::ApplyRotateTool()
{
} // ModelEditor::ApplyRotateTool

void ModelEditor::ApplyScaleTool()
{
} // ModelEditor::ApplyScaleTool

void ModelEditor::ApplySelectionTool()
{
} // ModelEditor::ApplySelectionTool

void ModelEditor::ZoomSelection()
{
} // ModelEditor::ZoomSelection

void ModelEditor::ZoomExtents()
{
} // ModelEditor::ZoomExtents

void ModelEditor::ResetModel()
{
    IMM->DeleteNode( m_AnimID );
    IMM->DeleteNode( m_ModelID );
    m_ModelID       = -1;
    m_AnimID        = -1;
    m_bPlayAnim     = false;
    m_bPauseAnim    = false;

    m_pRootNode = NULL;
    m_TreeL.SetRootNode( NULL );
    m_TreeR.SetRootNode( NULL ); 
} // ModelEditor::ResetModel

void ModelEditor::SaveModel()
{
    const char* fileName = NULL;
    const char* fileExt = NULL;

    m_LastPath.SetCWD();

    SaveFileDialog dlg;
    dlg.AddFilter( "Binary ModelObject Files", "*.c2m" );
    dlg.AddFilter( "XML ModelObject Files", "*.x2m" );
    dlg.SetDefaultExtension( "c2m" );
    static char lpstrFile[_MAX_PATH];
    if (!dlg.Show()) return;

    fileName = dlg.GetFilePath();
    if (!fileName) return;

    SNode* pNode = dynamic_cast<SNode*>( m_pRootNode );
    if (pNode) 
    {
        FOutStream os( fileName );
        pNode->SerializeSubtree( os );
    }

    /*IModelMgr->PushContext( 0xBEEEBEEE );
    IModelMgr->PushContext( m_ModelID );

    ModelObject* pModel = IModelMgr->GetModel();
    if (pModel)
    {
        FOutStream os( fileName );
        pModel->Serialize( os );
    }

    IModelMgr->PopContext();
    IModelMgr->PopContext();*/
} // ModelEditor::SaveModel

struct LeafNode
{
    SNode*       m_pNode;
    Texture*    m_pTexture;
    LeafNode( SNode* pNode, Texture* pTex ) : m_pNode( pNode ), m_pTexture( pTex ) {}
    bool operator <( const LeafNode& ln ) const 
    { 
        if (!m_pTexture) return true;
        if (!ln.m_pTexture) return false;
        int nc = stricmp( m_pTexture->GetName(), ln.m_pTexture->GetName() );
        if (nc != 0) return (nc < 0);
        return (m_pTexture->GetStage() < ln.m_pTexture->GetStage());
    } 
}; // struct LeafNode

SNode* CompileModel( SNode* pRoot, const char* path )
{
    if (pRoot->IsA<Switch>()) return pRoot;
    //  instance all "inputs"    
    SNode::Iterator it( pRoot );
    SNode* pTextures = NULL;
    SNode* pShaders  = NULL;
    SNode* pLights   = NULL;
    SNode* pCameras  = NULL;
    while (it)
    {
        SNode* pNode = (SNode*)*it;
        SNode* pParent = (SNode*)it.GetParent();
        ++it;
        if (pNode->IsA<Group>() && pNode->HasNameCI( "textures" )) pTextures = pNode;
        if (pNode->IsA<Group>() && pNode->HasNameCI( "cameras"  )) pCameras  = pNode;
        if (pNode->IsA<Group>() && pNode->HasNameCI( "lights"   )) pLights   = pNode;
        if (pNode->IsA<Group>() && pNode->HasNameCI( "devicestates"  )) pShaders  = pNode;

        if (pNode->IsA<Skin>())
        {
            Skin* pSkin = dynamic_cast<Skin*>( pNode );
            int nCh = pSkin->GetNChildren();
            for (int i = 0; i < nCh; i++)
            {
                TransformNode* pBone = (TransformNode*)pSkin->GetChild( i );
                if (!pBone->IsA<TransformNode>()) continue;
                const char* boneName = pBone->GetName();
                pSkin->AddInputBone( boneName );
            }
            pSkin->ReleaseChildren();
            it.Up();
        }
        if (pParent && !pParent->Owns( pNode ))
        {
            pParent->ReplaceChild( pNode, dynamic_cast<SNode*>( pNode->Clone() ) );
        }
    }

    if (pTextures) 
    { 
        pTextures->ReleaseChildren(); 
        pTextures->GetParent()->DelChild( pTextures );
    }

    if (pCameras) 
    { 
        pCameras->ReleaseChildren(); 
        pCameras->GetParent()->DelChild( pCameras );
    }

    if (pLights) 
    { 
        pLights->ReleaseChildren(); 
        pLights->GetParent()->DelChild( pLights );
    }

    if (pShaders) 
    { 
        pShaders->ReleaseChildren(); 
        pShaders->GetParent()->DelChild( pShaders );
    }

    //  collect billboarding nodes
    it.Reset();
    std::vector<LeafNode> leafs;
    Texture* pCurTex = NULL;
    while (it)
    {
        SNode* pNode = (SNode*)*it;
        ++it;
        if (pNode->IsA<Texture>()) pCurTex = (Texture*)pNode;
        if (!strnicmp( pNode->GetName(), "#billboard", 6 ) && 
            pNode->IsA<Geometry>())
        {
            leafs.push_back( LeafNode( pNode, pCurTex ) );
        }
    }

    int nLeafs = leafs.size(); 
    if (nLeafs > 0)
    {
        pCurTex = NULL;
        int nGeom = 0; 
        std::sort( leafs.begin(), leafs.end() );
        Group* pLeafs = pRoot->AddChild<Group>( "Billboards" );
        pLeafs->AddChild<Shader>( "leafs_impostor" );
        int i = 0;
        while (i < nLeafs)
        {
            int nG   = 0;
            int nV   = 0;
            int nIdx = 0;
            for (int j = i; j < nLeafs; j++)
            {
                if (leafs[j].m_pTexture &&
                    leafs[i].m_pTexture &&
                    !leafs[j].m_pTexture->IsEqual( leafs[i].m_pTexture )) break;
                Geometry* pGeom = (Geometry*)(leafs[j].m_pNode);
                nV += pGeom->GetMesh().getNVert();
                nIdx += pGeom->GetMesh().getNInd();
                nG++;
            }
            
            //  bake all billboards into one geometry node
            char geomName[256];
            sprintf( geomName, "Billboards%d", nGeom );
            nGeom++;
            Texture* pTex = leafs[i].m_pTexture ? (Texture*)leafs[i].m_pTexture->Clone() : NULL;
            pLeafs->AddChild( pTex );
            Geometry* pGeom = pLeafs->AddChild<Geometry>( geomName );
            pGeom->Create( nV, nIdx, vfVertex2t );
            BaseMesh&   gbm  = pGeom->GetMesh();
            WORD*       pIdx = gbm.getIndices();
            Vertex2t*   pV   = (Vertex2t*)gbm.getVertexData();
            int cV = 0;
            int cI = 0;
            for (int j = 0; j < nG; j++)
            {
                LeafNode&   lf = leafs[i + j];
                Matrix4D    tm = GetWorldTM( lf.m_pNode );
                BaseMesh&   bm = ((Geometry*)lf.m_pNode)->GetMesh();
                VertexIterator vit( bm.getVertexData(), 
                                    bm.getNVert(), 
                                    CreateVertexDeclaration( bm.getVertexFormat() ) );
                int         nv = bm.getNVert();
                int         ni = bm.getNInd();
                float       cx = tm.e30;
                float       cy = tm.e31;
                float       cz = tm.e32;
                
                for (int k = 0; k < ni; k++) pIdx[cI++] = (bm.getIndices())[k] + cV;

                while (vit)
                {
                    Vector3D wpos( vit.pos() ); 
                    tm.transformPt( wpos );
                    
                    DWORD clr = vit.diffuse(); 
                    if (clr == 0) clr = 0xFFFFFFFF;

                    pV[cV].x        = cx;
                    pV[cV].y        = cy;
                    pV[cV].z        = cz;
                    pV[cV].u        = vit.uv( 0 ).u;
                    pV[cV].v        = vit.uv( 0 ).v;
                    pV[cV].diffuse  = clr;
                    pV[cV].u2       = wpos.y - cy;
                    pV[cV].v2       = wpos.z - cz;

                    cV++;
                    ++vit;
                }       

            }
            gbm.setNVert( cV );
            gbm.setNInd ( cI );
            gbm.setNPri ( cI/3 );
            i += nG;
        }
        // remove original leafs from the model
        for (int i = 0; i < nLeafs; i++)
        {
            LeafNode& lf = leafs[i];
            SNode* pParent  = lf.m_pNode->GetParent();
            if (!pParent) continue;
            pParent->DelChild( lf.m_pNode );
            SNode* pGrandpa = pParent->GetParent();
            if (!pGrandpa) continue;
            pGrandpa->DelChild( pParent );
        }
    }

    if (pRoot->IsA<Animation>())
    {
    
    }
    else if (pRoot->IsA<PEmitter>())
    {
    
    }
    else if (!pRoot->IsA<Model>())
    {
        Model* pModel = new Model();
        pModel->AddChild( pRoot );
        pModel->SetAABB( CalculateAABB( pModel ) );
        pRoot = pModel;

		FOutStream os( path );
        FilePath p( path );
        char buf[_MAX_PATH];
        strcpy( buf, p.GetFileName() );
        strcat( buf, "_old" );
        p.SetFileName( buf );        

        IRM->CheckFileAccess( path );
		IRM->CheckFileAccess( p.GetFullPath() );
		remove( p.GetFullPath() );
        rename( path, p.GetFullPath() );

        pRoot->SerializeSubtree( os );
    }
    
    return pRoot;
} // CompileModel

bool ConvertMetaModelToC2M( char* path );
void ModelEditor::LoadModel()
{
    const char* fileName = NULL;
    m_LastPath.SetCWD();

    OpenFileDialog dlg;
    dlg.AddFilter( "Binary ModelObject Files",    "*.c2m"     );
    //  show "Open File" dialog
    dlg.SetDefaultExtension( "c2m" );
    if (!dlg.Show()) return;

    ResetModel();

    fileName = dlg.GetFilePath();
    m_LastPath.SetPath( fileName );

    _chdir( IRM->GetHomeDirectory() );
    if (!fileName) return;
    
    m_ModelID = IMM->GetModelID( fileName );
    if (m_ModelID == 0xFFFFFFFF) 
    {
        Log.Warning( "Could not load model: %s", fileName );
        return;
    }
    
    Stop();
    m_AnimID = 0xFFFFFFFF;

    m_pRootNode = IMM->GetModelRoot( m_ModelID );    
	Body* CreateRagdoll(SNode* Root);
	CreateRagdoll(m_pRootNode);
    m_TreeL.SetRootNode( m_pRootNode );
    m_TreeR.SetRootNode( m_pRootNode );
} // ModelEditor::LoadModel

void ModelEditor::LoadSubtree()
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

    SNode* pParent = dynamic_cast<SNode*>( m_TreeL.GetSelectedNode() );
    if (pParent) pParent->AddChild( pSubtree );    
} // ModelEditor::LoadModel

void ConvertModels( const char* root );
void ModelEditor::UpdateAllModels()
{
    ConvertModels( "q:\\root" );
} // UpdateAllModels

void ModelEditor::CreateVIPM()
{
    /*IReflected::Iterator it( m_pModel, Geometry::FnFilter );
    while (it)
    {
        Geometry* pGeom = (Geometry*)(IReflected*)it;
        Primitive& pri = pGeom->GetMesh();
        VIPMLodder lodder;
        int nV = pri.getNVert();
        int nF = pri.getNPri();
        VertexIterator vit; vit << pri;
        for (int i = 0; i < nV; i++) lodder.AddVertex( vit.pos( i ) );
        WORD* idx = pri.getIndices();
        for (int i = 0; i < nF; i++) lodder.AddFace( idx[i*3], idx[i*3 + 1], idx[i*3 + 2] );

        FILE* fp = fopen( "c:\\dumps\\vipm.txt", "at+" );
        fprintf( fp, "\n\n%s\n", pGeom->GetName() );
        fclose( fp );
        lodder.Process();
        ++it;
    }*/
} // ModelEditor::CreateVIPM
void ModelEditor::LoadAnimation(){
	LoadAnimation(0);
}
void ModelEditor::LoadAnimation(int Index)
{
    const char* fileName = NULL;
    m_LastPath.SetCWD();

    OpenFileDialog dlg;
    dlg.AddFilter( "ModelObject Files", "*.c2m" );
    dlg.SetDefaultExtension( "c2m" );
    static char lpstrFile[_MAX_PATH];
    if (!dlg.Show()) return;
    fileName = dlg.GetFilePath();
    if (!fileName) return;
    m_LastPath.SetPath( fileName );
	if(Index==0)m_AnimID  = IMM->GetModelID( fileName );
    if(Index==1)m_AnimID1 = IMM->GetModelID( fileName );
	if(Index==2)m_AnimID2 = IMM->GetModelID( fileName );
    Play();
} //  ModelEditor::LoadAnimation

void ModelEditor::SaveEffects()
{
    const char* fileName = NULL;
    const char* fileExt = NULL;

    //_chdir( GetRootDirectory() );
    _chdir( "Models\\Scripts" );

    SaveFileDialog dlg;
    dlg.AddFilter( "XML Files", "*.x2m" );
    dlg.SetDefaultExtension( "x2m" );
    static char lpstrFile[_MAX_PATH];
    if (!dlg.Show()) return;

    //_chdir( GetRootDirectory() );

    fileName = dlg.GetFilePath();
    if (!fileName) return;    
    FOutStream os( fileName );
    if (os.NoFile()) return;
    
    /*XMLNode root;
    root.SetTag( "AnimDescr" );
    IReflected::Iterator it( m_pModel, PEffect::FnFilter );
    while (it)
    {
        PEffect* pEff = (PEffect*)(IReflected*)it;
        root.AddChild( pEff->ToXML() );
        ++it;
    }
    root.Write( os );*/
} // ModelEditor::SaveEffects

void ModelEditor::LoadEffects()
{
    const char* fileName = NULL;
    //_chdir( GetRootDirectory() );
    _chdir( "Models\\Scripts" );

    OpenFileDialog dlg;
    dlg.AddFilter( "XML ModelObject Files", "*.x2m" );
    dlg.SetDefaultExtension( "x2m" );
    static char lpstrFile[_MAX_PATH];
    if (!dlg.Show()) return;
    fileName = dlg.GetFilePath();

    //_chdir( GetRootDirectory() );

    if (!fileName) return;
    FInStream is( fileName );
    if (is.NoFile()) return;
    XMLNode root( is );
    int nEff = root.GetNChildren();
    XMLNode* pChild = root.FirstChild();
    /*for (int i = 0; i < nEff; i++)
    {
        PEffect* pEff = new PEffect();
        pEff->FromXML( pChild );
        IReflected* pBone = m_pModel->FindChild<TransformNode>( pEff->GetParentBoneName() );
        if (!pBone) pBone= m_pModel->FindChild<Group>( pEff->GetParentBoneName() );
        if (pBone) pBone->AddChild( pEff );
        pChild = pChild->NextSibling();
    }*/
} // ModelEditor::LoadEffects

void ModelEditor::ToggleStatistics()
{
}

void ModelEditor::ToggleDrawTerrain()
{
    m_bDrawTerrain = !m_bDrawTerrain;
    ITerra->SetDrawCulling  ( m_bDrawTerrain );
} // ModelEditor::ToggleDrawTerrain

void ModelEditor::ToggleLeftPane()
{
    m_TreeL.SetVisible( !m_TreeL.IsVisible() );
    if (!m_TreeL.IsVisible()) m_TreeL.SetExtents( IRS->GetViewPort() );
} // ModelEditor::ToggleLeftPane

void ModelEditor::ToggleRightPane()
{
    m_TreeR.SetVisible( !m_TreeR.IsVisible() );
    if (!m_TreeR.IsVisible()) 
    {
        Rct vp = IRS->GetViewPort();
        m_TreeR.SetExtents( vp );
        m_TreeR.SetRootPos( vp.w - 50, vp.h*0.5f );
    }
} // ModelEditor::ToggleRightPane

bool ModelEditor::OnChar( DWORD keyCode, DWORD flags )
{
    if (keyCode == '`') { ToggleInvisible(); return true;}
    return false;
} // ModelEditor::OnChar

bool ModelEditor::OnKeyDown( DWORD keyCode, DWORD flags )
{
	if (keyCode == VK_HOME    ) ResetCameras();

	return false;

    if (GetKeyState( VK_CONTROL ) < 0)
    {
        if (keyCode == 'F')     ToggleStatistics();
        if (keyCode == 'A')     ToggleInspector();
        if (keyCode == VK_UP)    MoveUpNode();
        if (keyCode == VK_DOWN) MoveDownNode();
        if (keyCode == 'C')     CopyNode();
        if (keyCode == 'X')     CutNode();
        if (keyCode == 'V')     PasteNode();
        if (keyCode == 'U')     UpdateAllModels();
        if (keyCode == 'L')     LoadSubtree();
        return false;
    }

    if (keyCode == 'L'        ) LoadModel();
    if (keyCode == 'S'        ) SaveModel();
    if (keyCode == VK_HOME    ) ResetCameras();
    if (keyCode == VK_F1    ) ToggleLeftPane();
    if (keyCode == VK_F2    ) ToggleRightPane();
    //if (keyCode == VK_DELETE) DeleteNode();

    if (keyCode == VK_TAB    ) ToggleHideModel();
    if (keyCode == 'V'        ) CreateVIPM();
	if (keyCode == 'A'        ){		
		if( (GetKeyState(VK_MENU)&0x8000) && (GetKeyState(VK_SHIFT)&0x8000) )LoadAnimation(2);
		else if(GetKeyState(VK_SHIFT)&0x8000)LoadAnimation(1);
		else LoadAnimation();
	}
    if (keyCode == VK_PAUSE    ) Pause();

    if (keyCode == 'Q'      ) ApplySelectionTool();
    if (keyCode == 'W'      ) ApplyTranslateTool();
    if (keyCode == 'E'      ) ApplyRotateTool   ();
    if (keyCode == 'R'      ) ApplyScaleTool    ();
    //if (keyCode == 'Z'      ) CycleActiveCamera ();
    if (keyCode == 'M'      ) ToggleMeasure     ();

    return false;
} // ModelEditor::OnKeyDown

void ModelEditor::ToggleInspector()
{
    m_Inspector.SetVisible( !m_Inspector.IsVisible() );
}

void ModelEditor::ToggleMeasure()
{
    m_Measure.SetVisible( !m_Measure.IsVisible() );
    m_bDrawMeasure = !m_bDrawMeasure;
    m_Measure.BindTM( &m_MeasureTM );
} // ModelEditor::ToggleMeasure

void ModelEditor::ToggleInvisible()
{
    m_bDrawModel = !m_bDrawModel;
} // ModelEditor::ToggleInvisible

void ModelEditor::CopyNode()
{
    IReflected* pNode = GetSelectedNode();
    if (!pNode) return;
} // ModelEditor::CopyNode

void ModelEditor::PasteNode()
{
    IReflected* pNode = GetSelectedNode();
    if (!pNode) return;
} // ModelEditor::PasteNode

void ModelEditor::CutNode()
{
    IReflected* pNode = GetSelectedNode();
    if (!pNode) return;
} // ModelEditor::CutNode

void ModelEditor::MoveUpNode()
{
    IReflected* pNode = GetSelectedNode();
    if (!pNode) return;
    IReflected* pParent = pNode->Parent();
    if (!pParent) return;
    int cIdx = pParent->ChildIdx( pNode );
    if (pParent)
    {
        int nIdx = cIdx - 1;
        IReflected* pTmp = pParent->Child( nIdx );
        pParent->SetChild( nIdx, pNode );
        pParent->SetChild( cIdx, pTmp );
        SelectNode( pNode );
    }
} // ModelEditor::MoveUpNode

void ModelEditor::MoveDownNode()
{
    IReflected* pNode = GetSelectedNode();
    if (!pNode) return;
    IReflected* pParent = pNode->Parent();
    if (!pParent) return;
    int cIdx = pParent->ChildIdx( pNode );
    if (pParent)
    {
        int nIdx = cIdx + 1;
        IReflected* pTmp = pParent->Child( nIdx );
        pParent->SetChild( nIdx, pNode );
        pParent->SetChild( cIdx, pTmp );
        SelectNode( pNode );
    }
} // ModelEditor::MoveDownNode
void ModelEditor::ConvertSkin2GPU(){
	IMM->ExportToObj(m_ModelID,"test.obj");
	SNode* pNode = dynamic_cast<SNode*>( m_pRootNode );
	if(pNode){
        ConvertNodesToSkinGPU(pNode);
	}
}
void ModelEditor::ResetCameras()
{
    float aspect = IRS->GetViewPort().GetAspect();
    //  default camera
	m_EditorCamera.SetWorldTM(Matrix4D::identity);
    float volBound = 400.0f;
    float fovx = DegToRad( 90 );
    Vector3D gdir( 0.0f, -cos( c_PI/3.0f ), -sin( c_PI/3.0f ) );
    m_EditorCamera.SetOrientation( Vector3D::oX, Vector3D::oZ, gdir );
    gdir.reverse();
    float cdist = volBound * 1.8f / tan( fovx * 0.5f );
    gdir *= cdist;
    float zn = 12.0f;
    cdist -= zn;
    float zf = zn + cdist*100.0f;

    m_EditorCamera.SetPosition( gdir );
    m_EditorCamera.SetFOVx( fovx, zn, zf, aspect );

    //  game camera
    volBound    = 1024.0f;
    cdist       = volBound * 2.0f;
    zn          = cdist * 0.2f;
    zf          = zn + cdist*4.0f;
    Vector3D dir( 0.0f, -cos( c_PI/6.0f ), -sin( c_PI/6.0f ) );
    m_GameOrthoCamera.SetOrientation( Vector3D::oX, Vector3D::oZ, dir );
    dir.reverse();
    dir *= cdist;
    m_GameOrthoCamera.SetPosition( dir );
    m_GameOrthoCamera.SetViewVolume( volBound, zn, zf, IRS->GetViewPort().GetAspect() );

    //  alternative game camera
    fovx = DegToRad( 20.0f );
    gdir = Vector3D( 0.0f, -cos( c_PI/6.0f ), -sin( c_PI/6.0f ) );
    m_GamePerspCamera.SetOrientation( Vector3D::oX, Vector3D::oZ, gdir );
    zn = 1200.0f; 
    zf = 10400.0f;
    m_GamePerspCamera.SetPosition( Vector3D( 0.0f, 2500, 1500 ) );
    m_GamePerspCamera.SetFOVx( fovx, zn, zf, IRS->GetViewPort().GetAspect() );

    SetCamera( &m_GamePerspCamera );
} // ModelEditor::ResetCameras


