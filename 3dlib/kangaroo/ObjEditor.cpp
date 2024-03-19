#include "common.h"

IMPLEMENT_CLASS(ObjEditor);

ObjEditor::ObjEditor(){
	m_SpecularLevel=0.5f;
	m_PenDepth=1;
	m_PenRadius=0.3;
	m_LockColor=false;
	m_LockSpecular=false;
	SetName("ObjEditor");
}
void ObjEditor::OnInit(){
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

	m_Toolbar.CreateFromScript( "obj_editor_toolbar" );

	m_LastPath.SetPath( IRM->GetHomeDirectory() );
	m_LastPath.AppendDir( "Models\\" );

}

void ObjEditor::Expose( PropertyMap& pm )
{    
	pm.start<FrameWindow>( "ObjEditor", this );
	pm.p( "ShowCameras",        GetShowCameras, ShowCameras );
	pm.f( "ShowLights",         m_bShowLights            );
	pm.f( "ShowLocators",       m_bShowLocators            );
	pm.f( "ShowBones",          m_bShowBones              );
	pm.f( "ShowTransformNodes", m_bShowTransformNodes    );
	pm.f( "ShowNormals",        m_bShowNormals              );
	pm.f( "ShowGrid",           m_bShowGrid                );
	pm.f( "ShowBackdrop",       m_bShowBackdrop            );
	pm.f( "ShowModelStats",     m_bShowModelStats           );
	pm.f( "ActiveCamera",       m_ActiveCamera             );
	pm.f( "AnimSpeed",          m_AnimSpeed                 );
	pm.p( "CurAnimTimeRatio",   GetCurAnimTimeRatio, SetCurAnimTimeRatio );
	pm.p( "ModelLoaded",        IsModelLoaded               );
	pm.p( "AnimLoaded",         IsAnimLoaded                );
	pm.f( "AnimPlayed",         m_bPlayAnim                 );
	pm.f( "AnimPaused",         m_bPauseAnim                );
	pm.f( "ModelTransparency",  m_ModelTransparency         );
	pm.f( "PenDepth",			m_PenDepth                  );
	pm.f( "SpecularLevel",		m_SpecularLevel             );	
	pm.f( "PenRadius",          m_PenRadius                 );
	pm.f( "ModelColor",         m_ModelColor                );

	pm.m( "Play",               Play                        );
	pm.m( "Pause",              Pause                       );
	pm.m( "Stop",               Stop                        );

	pm.m( "ResetModel",         ResetModel                  );
	pm.m( "LoadModel",          LoadModel                   );
	pm.m( "SaveModel",          SaveModel                   );	

	pm.m( "SaveMesh",			SaveMesh					);
	pm.m( "LoadMesh",			LoadMesh					);
	pm.m( "LoadObj",			LoadObj	                    );	
	pm.m( "SaveNormalMap",		SaveNormalMap				);
	pm.m( "SaveTexture",		LoadMesh					);


	pm.f( "LockColor",			m_LockColor					);
	pm.f( "LockSpecular",		m_LockSpecular				);
}

void ObjEditor::SaveMesh(){
	SaveFileDialog dlg;
	dlg.AddFilter( "Surface files", "*.srf" );
	dlg.SetDefaultExtension( "srf" );	
	if (!dlg.Show()) return;
	Mesh->SaveMesh(dlg.GetFilePath());
}
void ObjEditor::LoadMesh(){	
	OpenFileDialog dlg;
	dlg.AddFilter( "Surface files",    "*.srf"     );	
	dlg.SetDefaultExtension( "srf" );
	_chdir(IRM->GetHomeDirectory());
	if (!dlg.Show()) return;
    Mesh->LoadMesh(dlg.GetFilePath());
}
void ObjEditor::LoadObj(){
	OpenFileDialog dlg;
	dlg.AddFilter( "Obj files",    "*.obj"     );	
	dlg.SetDefaultExtension( "obj" );
	_chdir(IRM->GetHomeDirectory());
	if (!dlg.Show()) return;
	Mesh->ReadObj(dlg.GetFilePath());
}
void ObjEditor::SaveNormalMap(){
	SaveFileDialog dlg;
	dlg.AddFilter( "Textures files", "*.dds" );
	dlg.SetDefaultExtension( "dds" );	
	if (!dlg.Show()) return;
	Mesh->SaveNormalmap(dlg.GetFilePath());
}
void ObjEditor::SaveTexture(){
	SaveFileDialog dlg;
	dlg.AddFilter( "Textures files", "*.dds" );
	dlg.SetDefaultExtension( "dds" );	
	if (!dlg.Show()) return;
	Mesh->SaveTexture(dlg.GetFilePath());
}

ObjEditor* m_OED=NULL;
extern PolyMesh PMS;

Widget* GetObjEditor(){
	if(!m_OED){
		LINK_CLASS( ObjEditor );
		m_OED=new ObjEditor;
		m_OED->Mesh=&PMS;
		//m_OED->Init();
	}
	return m_OED;
}



