#include "stdheader.h"
#include "UndoMaster.h"
#include "AllTools.h"
#include "Surface\Surface.h"
#include "Surface\SurfaceGenerator.h"
#include "ITerrain.h"
#include "kSystemDialogs.h"
#include "camera\Navigator.h"
#include "TankController.h"
#include "CollidingCamera.h"
#include "Obj3D.h"

AllTools::AllTools(){
    Name="AllTools";
	GetObjectGlobalID();
}
bool AllTools::KeepUV=false;
void AllTools::Undo(){
	UndoMaster::Undo();
}
bool ActivateSurfTool(const char *pToolName);
void AllTools::Deform(){
	ActivateSurfTool("Deform");
}
void AllTools::Smooth(){
	ActivateSurfTool("Smooth");
}
void AllTools::Texture(){
	ActivateSurfTool("Facturize");
}
void AllTools::Paint(){
	ActivateSurfTool("Colorize");
}
void AllTools::Boolean(){
	ActivateSurfTool("Combine");
}
void AllTools::AddModel(){
	ActivateSurfTool("ModelsInScene");
}
void AllTools::EditModels(){
	SceneObjExtension::EditMode=true;
}
void AllTools::Chamfer(){
    ActivateSurfTool("Chamfer");
}
void AllTools::Race(){
	ActivateSurfTool("Racist");
}
void AllTools::Archway(){
	ActivateSurfTool("Archway");
}
void AllTools::Layers(){
	ActivateSurfTool("Layer");
}
void AllTools::LocalGenerator(){
	ActivateSurfTool("LocalGenerator");
}
void AllTools::Grass(){
	ActivateSurfTool("Grass");
}
void ShowSurfGenerator();
void AllTools::Generator(){
	if(SURF.m_fActive) {
		SURF.m_fActive = false;
	} else {
		ShowSurfGenerator();
	}
}
void AllTools::Scissors(){
	ActivateSurfTool("Scissors");
}	
void AllTools::Export(){
	ActivateSurfTool("LocalExporter");
}
void AllTools::Illuminate(){	
	g_SurfEditor.Refresh.EvaluateFunction();
}
void AllTools::Refresh(){	
	EraseAreas();	
	void CreateTotalLocking();
	CreateTotalLocking();						
	void ResearchIslands();
	ResearchIslands();
	void CreateMiniMap();
	CreateMiniMap();
	void UnLoadAITools();
	UnLoadAITools();
	void RefreshRoads();
	RefreshRoads();
	void PutSpritesOnEarth();
	PutSpritesOnEarth();
	g_UniversalMap.CalcNormals();						
	ITerra->InvalidateGeometry();
}
extern byte NeedToPopUp;
void AllTools::Trees(){
	NeedToPopUp=3;
}    
void AllTools::Stones(){
    NeedToPopUp=4;
}
void AllTools::Effects(){
	NeedToPopUp=24;
}
void AllTools::KillObjects(){
    NeedToPopUp=7;
}
void AllTools::Units(){
	NeedToPopUp=2;
}	
void AllTools::CustomizeUnit(){
    ProcessEditor("Selected unit character editor");
}
void AllTools::Roads(){
	NeedToPopUp=19;    
}
void AllTools::SetLock(){
	NeedToPopUp=13;    
}
void AllTools::ClearLock(){
	NeedToPopUp=12;
}
void AllTools::DefaultLock(){
	NeedToPopUp=14;
}
void AllTools::Script(){
	be_CLegoDriver::setVisible(true);    
}
void AllTools::Nodes(){
	be_CNodesDriver::setVisible(true);
}
void AllTools::Groups(){
    be_CGroupsDriver::setVisible(true);
}
void AllTools::Debug(){
	g_beDebuger.setVisible(true);
}
void AllTools::MapOptions(){
	ProcessEditor("MapOptions");
}
void AllTools::EngineSettings(){
	ProcessEditor("EngineSettings");
}
void AllTools::FacturesProperties(){
	ProcessEditor("Factures");
}	
void AllTools::GrassProperties(){
	ProcessEditor("GrassProperties");
}	
void AllTools::GameSettings(){
	ProcessEditor("GameSettings");
}
void AllTools::DialogsEditor(){
	ProcessEditor("Dialogs");    
}
void AllTools::AbilitiesEditor(){
	ProcessEditor("Solution");
}
void AllTools::RoadsEditor(){
	ProcessEditor("Roads");    
}	
void AllTools::AllEditors(){
	void SelectEditor();
	SelectEditor();
}
void AllTools::DefaultCursor(){
	void ClearSurfTool();
    ClearSurfTool();
}
void AllTools::ScrollGroups(){
    NeedToPopUp=30;
}
void AllTools::Zones(){
    NeedToPopUp=31;
}
void AllTools::SetRiver(){
    NeedToPopUp=32;
}
char mapsDir[512]="Maps\\";
void AllTools::SaveMap(){
	SaveFileDialog dlg;	
	dlg.AddFilter( "m3d-files",    "*.m3d"     );	
	dlg.SetDefaultExtension( "m3d" );
	_chdir(mapsDir);
	if (dlg.Show()){
		_getcwd(mapsDir,512);
		_chdir(IRM->GetHomeDirectory());
		void Save3DMap(char* Map);
		Save3DMap((char*)dlg.GetFilePath());
	}
	_chdir(IRM->GetHomeDirectory());    
}
void AllTools::LoadMap(){
	OpenFileDialog dlg;	
	dlg.AddFilter( "m3d-files",    "*.m3d"     );	
	dlg.SetDefaultExtension( "m3d" );
	_chdir(mapsDir);
	if (dlg.Show()){
		_getcwd(mapsDir,512);
		_chdir(IRM->GetHomeDirectory());
		void Load3DMap(char* Map);
		Load3DMap((char*)dlg.GetFilePath());
	}
	_chdir(IRM->GetHomeDirectory());
}
extern bool PeaceMode;
void AllTools::Peace(){
    PeaceMode=!PeaceMode;
}
void AllTools::FreeCamera(){
	if(!g_SurfEditor.FreeCamera) {
		g_SurfEditor.FreeCamera = true;
		g_SurfEditor.Update();
		g_Navigator.Toggle(true);
	}else{
		g_Navigator.Home();
	}    
}
void AllTools::ToggleHero(){
    g_Navigator.LinkToUnitToggle();
}
void AllTools::ToggleTank(){
	g_TankController.Toggle();
}
void AllTools::CollCamera(){
	g_CollidingCamera.Toggle();
}
extern byte LockGrid;
void AllTools::ShowLock(){
	LockGrid=!LockGrid;
	if(LockGrid){
		LockGrid=DriveMode()->LockType+1;
	}
}
void AllTools::ShowTopology(){
    DriveMode()->ShowHTT=!DriveMode()->ShowHTT;
	DriveMode()->ShowLinks=DriveMode()->ShowHTT;
}
void AllTools::ViewLand(){
    if(LockGrid)LockGrid=1;
	DriveMode()->LockType=0;
}
void AllTools::ViewWater(){
	if(LockGrid)LockGrid=2;
	DriveMode()->LockType=1;
}
void AllTools::ViewLowAir(){
	if(LockGrid)LockGrid=3;
	DriveMode()->LockType=2;
}
void AllTools::ViewHiAir(){
	if(LockGrid)LockGrid=4;
	DriveMode()->LockType=3;
}

void ShowAllToolsToolbar(){
    static DialogsSystem* D=NULL;
	if(!D){
		D=new DialogsSystem;
		D->SafeReadFromFile("Dialogs\\Tools.DialogsSystem.xml");
	}
	if(! (		
		be_CNodesDriver::getVisible() ||
		be_CGroupsDriver::getVisible() ||
		be_CLegoDriver::getVisible() ) ){
			D->ProcessDialogs();
		}
}
AllTools* TOOLS=NULL;
void InitTools(){
	REG_CLASS(AllTools);    
}
