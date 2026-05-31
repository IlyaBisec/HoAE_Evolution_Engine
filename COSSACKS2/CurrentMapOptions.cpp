#include "stdheader.h"
#include "CurrentMapOptions.h"
#include ".\supereditor.h"
#include "BE_HEADERS.h"
#include "GameInterface.h"
#include "Camera\Navigator.h"
MapOptions MOptions;
//CEXPORT int ItemChoose;
void KillUnusableClassRegistrations();
bool MMItemChoose(SimpleDialog* SD);
void EditMapOptions(){
	xmlQuote xml;
	ItemChoose=-1;
	if(xml.ReadFromFile("Dialogs\\MapOptionsDialog.DialogsSystem.xml")){
		DialogsSystem DSS;
		ErrorPager EP;
		DSS.Load(xml,&DSS,&EP);
		SimpleDialog* Desk=DSS.Find("OptionsDesk");
		SimpleDialog* OK=DSS.Find("OK");
		SimpleDialog* CANCEL=DSS.Find("CANCEL");
		if(Desk&&OK&&CANCEL){
			int x0,y0,x1,y1;
			DSS.GetDialogsFrame(x0,y0,x1,y1);
			if(x1>x0){
				DSS.x=(RealLx-x1+x0)/2;
				DSS.y=(RealLy-y1+y0)/2;
				OK->OnUserClick=&MMItemChoose;
				OK->UserParam=1;
				CANCEL->OnUserClick=&MMItemChoose;
				CANCEL->UserParam=1;
				ClassEditor CE;
				CE.CreateFromClass(Desk,0,0,Desk->x1-Desk->x,Desk->y1-Desk->y,&MOptions,3,"EmptyBorder");
				do{
                    ProcessMessages();					
					DSS.ProcessDialogs();
					CE.Process();
					DSS.RefreshView();
				}while(ItemChoose==-1);
			}
		}
	}
}
extern char GlobalTextCommand[256];
class ScriptEd:public BaseClass{
public:
	OneScript Script;
	SAVE(ScriptEd);
	REG_AUTO(Script);
	ENDSAVE;
};
void KeyTestMem();
void EditScripts(){
	xmlQuote xml;
	ItemChoose=-1;
	if(xml.ReadFromFile("Dialogs\\ScriptEditor.DialogsSystem.xml")){
		DialogsSystem DSS;
		ErrorPager EP;
		DSS.Load(xml,&DSS,&EP);
		SimpleDialog* Desk=DSS.Find("CLASSEDITOR");
		SimpleDialog* CmlDesk=DSS.Find("SOURCEDESK");
		SimpleDialog* OK=DSS.Find("OK");
		SimpleDialog* CANCEL=DSS.Find("CANCEL");
		DString ScriptText;ScriptText.Add("");
		//Privet {CG}{R FFFF0000 Shuriku}Shuriku{C} ot\\ {CG}{R FFFF0000 druzej} druzej{C} po rabote"
		TextButton* TB=CmlDesk->addTextButton(NULL,0,0,ScriptText.pchar(),&WhiteFont,&WhiteFont,&WhiteFont,0);

		//CmlDesk->addInputBox(NULL,0,100,GlobalTextCommand,100,100,20,&WhiteFont,&WhiteFont);
		if(Desk&&OK&&CANCEL){
			int x0,y0,x1,y1;
			DSS.GetDialogsFrame(x0,y0,x1,y1);
			if(x1>x0){
				DSS.x=(RealLx-x1+x0)/2;
				DSS.y=(RealLy-y1+y0)/2;
				OK->OnUserClick=&MMItemChoose;
				OK->UserParam=1;
				CANCEL->OnUserClick=&MMItemChoose;
				CANCEL->UserParam=1;
				ClassEditor CE;
				//OperandBlock FirstOperand;
				ScriptEd Script;
				Script.Script.ReadQppFile("AI\\test.qpp");
				//FirstOperand.Add( (Operand*) new OperandBlock());
				CE.CreateFromClass(Desk,0,0,Desk->x1-Desk->x,Desk->y1-Desk->y,&Script,3,"EmptyBorder");
				do{
					if(strlen(GlobalTextCommand)>0)
					{
						int ii=atoi(GlobalTextCommand);
						void* Adres=(void*)ii;
						if(Adres)
						{
							void SetClassEditorScrollPosTo(void* Adres,ClassEditor* CE);
							SetClassEditorScrollPosTo(Adres,&CE);
						}
						GlobalTextCommand[0]=0;
					}
					ScriptText.Clear();
					Script.Script.VAR.RefreshEnumerator();
					Script.Script.VAR.GetAssembledView(ScriptText,false);
					Script.Script.BrigadesVariables.RefreshEnumerator();
					Script.Script.BrigadesVariables.GetAssembledView(ScriptText,false);
					Script.Script.Script.GetAssembledView(ScriptText,false);
					ScriptText.WriteToFile("gec.qpp");
					//Script.Script.Script.Execute();
					TB->Message=ScriptText.pchar();
					ProcessMessages();					
					DSS.ProcessDialogs();
					if(ItemChoose!=-1)break;
					CE.Process();
					ItemChoose=-1;
					DSS.RefreshView();
				}while(ItemChoose==-1);
			}
		}
	}
}
//==================================================================================================================//
int PlayGameProcessList::GetExpansionRules()
{
	return 2; 
}
bool PlayGameProcess::Process()
{
	return false;
}
//------------------------------------------------------------------------------------------------------------------//
bool PlayGameProcessList::Process()
{
	bool rez=false;
	int n=GetAmount();
	for(int i=0;i<n;i++)
	{
		rez=(*this)[i]->Process()||rez;
	}
	return rez;
}
//------------------------------------------------------------------------------------------------------------------//
StartTacticalAI::StartTacticalAI()
{
	NI=0;
}
//
bool vf_CheckTackticalAIStart(byte NI);
void ActivateTacticalAI(byte NI);
//
bool StartTacticalAI::Process()
{
	/*
	CurrentGameInfo* g=&GSets.CGame;
	for(int i=NPlayers;i<8&&COMPSTART[i];i++){
		PlayerInfo* I=g->PL_INFO+i;
		ActivateTacticalAI(I->ColorID);
	}
	return true;
	*/
	if(vf_CheckTackticalAIStart(NI)){
		ActivateTacticalAI(NI);
		return true;
	}
	return false;	
}
//------------------------------------------------------------------------------------------------------------------//
bool PreviewBinkVideo::Process(){
	extern word NPlayers;
	if(NPlayers==1&&!BinkFile.isClear()){
		void PlayFullscreenVideo(char* name,float px,float py, bool Skipable=true);
		PlayFullscreenVideo(BinkFile.pchar(), 0.0f, 0.17f);
	}
	return false;
}
//
int StartResScope::GetN_Single(){
	int n=0;
	for(int i=0;i<=MaxNatColors;i++){
		if(!Player[i].DisableInSingle){
			n++;
		}
	}
	return n;
};
int StartResScope::GetN_Multi(){
	int n=0;
	for(int i=0;i<=MaxNatColors;i++){
		if(!Player[i].DisableInMultiplayer){
			n++;
		}
	}
	return n;
};

//==================================================================================================================//
void UnInstallExtension(GameExtension* GI);
void InstallExtension(GameExtension* Ext,const char* Name);
class ExposedInterface{
public:
    HINSTANCE LIB_ID;
	DWORD Code;
	IExchangeInterface* Interface;	
};

DynArray<ExposedInterface*> ExposedIntfs;
IExchangeInterface* GetIExchangeInterface(DWORD Code){
	for(int i=0;i<ExposedIntfs.GetAmount();i++){
		if(ExposedIntfs[i]->Code==Code)return ExposedIntfs[i]->Interface;
	}
    return NULL;
}
void PrecessScriptForExposedInterfaces(){
	if(DriveMode()->PROCESS){
		int N=MOptions.Plugins.GetAmount();
		for(int i=0;i<N;i++){
            BaseClass* BC=MOptions.Plugins[i]->Interface;
			if(BC){
				((GameInterface*)BC)->ProcessScript();
			}
		}
	}
}
GameInterface* LoadPluginDLL(const char* Name,HINSTANCE& H){

	KillUnusableClassRegistrations();
	H=LoadLibrary(Name);
	KillUnusableClassRegistrations();
	if(H){
		fnDllInterface* fn=(fnDllInterface*)GetProcAddress(H,"?GetInterface@@YAPAVBaseClass@@XZ");        
		if(fn){
			GameInterface* GI=(GameInterface*)fn();
			if(GI){
				IExchangeInterface* IEE=NULL;
				int idx=0;
				do{
					IExchangeInterface* IEEP=IEE;
                    IEE=GI->ExposeMyInterfaces(idx++);
					if(IEE && IEE!=IEEP){
						ExposedInterface* EE=new ExposedInterface;
						EE->LIB_ID=H;
                        EE->Interface=IEE;
						EE->Code=IEE->GetUniqID();
						ExposedIntfs.Add(EE);
					}else break;
				}while(true);
				InstallExtension(GI,Name);
				return GI;
            }else{
                Log.Error("Unable to get GameInterface pointer in plugin DLL: %s",Name);
            }
        }else{
            Log.Error("Unable to find 'Get interface' in plugin DLL: %s",Name);
        }
    }else{
        Log.Error("Unable to load plugin DLL: %s",Name);
	}
	return NULL;
}
void UnLoadDLL(HINSTANCE H,GameInterface* GI){
	if(GI){		
		GI->OnClassUnRegister();		
		for(int i=0;i<ExposedIntfs.GetAmount();i++){
			if(ExposedIntfs[i]->LIB_ID==H){
				ExposedIntfs.Del(i--,1);
			}
		}
        UnInstallExtension(GI);		
		KillUnusableClassRegistrations();
	}
}
bool CompileCppScript(const char* cppList,_str& OutDllName,_str& outErrors);

void DllExtension::LoadDll(const char* Name, bool Permanent){
	_str tname=Name;
	_str err;
	if(strstr(Name,".cl")){
		if(CompileCppScript(Name,tname,err)){
            //well done
		}else{
			//error
		}
	}
	UnLoadDll();
    LastLoadDllName=Name;
	if(!DllName.equal(Name))DllName=Name;
	Interface=LoadPluginDLL(tname.pchar(),LibID);	
	if(Interface){
        GameInterface* GI=(GameInterface*)Interface;
		if(!Permanent)
			GI->OnClassRegistration();
		GI->OnGameStart();
	}
}
void DllExtension::UnLoadDll(){
	if(Interface){
		UnLoadDLL(LibID,(GameInterface*)Interface);
		Interface=NULL;
	}
	if(LibID){
		FreeLibrary(LibID);
		LibID=NULL;
	}    
}
void ProcessPluginsSystem(){
	for(int i=0;i<MOptions.Plugins.GetAmount();i++){
		if(strcmp(MOptions.Plugins[i]->LastLoadDllName.pchar(),MOptions.Plugins[i]->DllName.pchar())){
			MOptions.Plugins[i]->LoadDll(MOptions.Plugins[i]->DllName.pchar(),false);
		}
	}
}
ClassArray<DllExtension> PermanentDLLS;
void LoadPermanentPlugins(){
	FileList DLLS;
	CreateSortedFileList("Plugins\\","*.dll",DLLS,false);
	for(int i=0;i<DLLS.GetAmount();i++){
		DllExtension* E=new DllExtension;
		char c[256];
		sprintf(c,"Plugins\\%s",DLLS[i]->pchar());
		E->LoadDll(c,true);
        PermanentDLLS.Add(E);
	}
}
void* FindFunctionInPermanentDLLs(const char* Name){
	for(int i=0;i<PermanentDLLS.GetAmount();i++)    	{
		if(PermanentDLLS[i]->LibID){
			void* f=GetProcAddress(PermanentDLLS[i]->LibID,Name);
			if(f)return f;
		}
	}
	return NULL;
}
bool launchProgram(const char* appName, const char* cmdLine, bool isWait);
bool Exec(const char *pFileName, const char *pConsoleOutputFile, const char *pCmdArgs = NULL); 
extern HWND hwnd;
bool CompileCppScript(const char* cppList,_str& OutDllName,_str& OutErrors){	
	char drive  [_MAX_PATH];
	char dir    [_MAX_PATH];
	char fname  [_MAX_PATH];
	char ext    [_MAX_PATH];
	char tname  [_MAX_PATH];	
	_splitpath( cppList, drive, dir, fname, ext );

	FILE* F=fopen(cppList,"r");
	if(F){
		int z;
		//compiling
		_str acm;		
		do{
			z=0;
			char name[64];			
			z=fscanf(F,"%s",name);
			if(z==1){
				_str temp;
				temp.DString::ReadFromFile("cpp\\clopt.txt");				
				sprintf(tname,"%s%s%s.cpp",drive,dir,name);				
				temp+=tname;
				//temp+=">cpp\\temp\\cl_log.txt";
                Exec("cpp\\cl.exe","cpp\\temp\\cl_log.txt",temp.pchar());
				sprintf(tname,"\"cpp\\temp\\%s.obj\" ",name);
                acm+=tname;
				_str err;
				err.DString::ReadFromFile("cpp\\temp\\cl_log.txt");
				if(strstr(err.pchar()," error ")){
					MessageBox(hwnd,err.pchar(),name,0);										
					OutErrors+=err.pchar();
					OutErrors+="\n";
				}				
			}
		}while(z==1);		
		fclose(F);
		//linking
		_str temp;
		temp.DString::ReadFromFile("cpp\\linkopt.txt");
        sprintf(tname,"%s%s%s.dll",drive,dir,fname);
		_str temp2;
		temp2.print(temp.pchar(),tname);
		temp2+=acm;
		//temp2+=">cpp\\temp\\link_log.txt";
        Exec("cpp\\link.exe","cpp\\temp\\link_log.txt",temp2.pchar());
		OutDllName=tname;
		_str err;
		err.DString::ReadFromFile("cpp\\temp\\link_log.txt");
		if(strstr(err.pchar()," error ")){
			MessageBox(hwnd,err.pchar(),cppList,0);						
			OutErrors+=err.pchar();
			OutErrors+="\n";
		}
	}
	return true;
}

// MapOptions::OnChangeMember
bool MapOptions::OnChangeMember(BaseClass *, void *, void *, const char *) {
/*	static float PrevGlobalScale = 0.0f;
	if(PrevGlobalScale != GetGlobalScale()) {
		PrevGlobalScale = GetGlobalScale();
		ResetGroundCache();
	}*/
	return false;
}

//*****************************************************************************
// MapOptionsLimitCamArgs
//*****************************************************************************

// MapOptions::MapOptionsLimitCamArgs::CheckIfElementInvisible
bool MapOptions::MapOptionsLimitCamArgs::CheckIfElementInvisible(int Index, void *, DWORD) {
	if(1 == Index) { // PrevUseFromMap
		return true;
	}
	return UseFromMap ? false : Index > 0;
}

// MapOptions::MapOptionsLimitCamArgs::OnChangeMember
bool MapOptions::MapOptionsLimitCamArgs::OnChangeMember(BaseClass *, void *, void *, const char *) {
	if(PrevUseFromMap != UseFromMap) {
		PrevUseFromMap = UseFromMap;
		return true;
	}
	return false;
}

// MapOptions::MapOptionsLimitCamArgs::ZeroAuxVars
void MapOptions::MapOptionsLimitCamArgs::ZeroAuxVars() {
	LeftShift = RightShift = TopShift = BottomShift = 0;
	LastShiftCalcMinMapX = LastShiftCalcMinMapY = 0;
	LastShiftCalcMaxMapX = LastShiftCalcMaxMapY = 0;
	LastShiftCalcXRes = LastShiftCalcYRes = 0;
	LastShiftCalcToEyeEx = 0.0f;
}

// MapOptions::MapOptionsLimitCamArgs::SetFromEngineSettings
void MapOptions::MapOptionsLimitCamArgs::SetFromEngineSettings() {
	const RECT rc = EngSettings.LimitCamArgs.GetRect(); // Either for editor or game
	MinMapX = rc.left;
	MaxMapX = rc.right;
	MinMapY = rc.top;
	MaxMapY = rc.bottom;
}

// MapOptions::MapOptionsLimitCamArgs::LimitCamArgsCopy::EvaluateFunction
void MapOptions::MapOptionsLimitCamArgs::LimitCamArgsCopy::EvaluateFunction() {
	const char Text[] = "Are you sure you want to copy \"Limit Cam Args\" from EngineSettings?";
	const char Caption[] = "Confirm Copy \"Limit Cam Args\" From EngineSettings";
	extern HWND g_hWnd;
	if(MessageBox(g_hWnd, Text, Caption, MB_YESNO) == IDYES) {
		MapOptionsLimitCamArgs *pArgs = dynamic_cast<MapOptionsLimitCamArgs *>(ParentBC);
		if(pArgs) {
			pArgs->SetFromEngineSettings();
		}
	}
}

// MapOptions::MapOptionsLimitCamArgs::IsValid
bool MapOptions::MapOptionsLimitCamArgs::IsValid() {
	const RECT rc = GetRect();
	return LastShiftCalcXRes == RealLx && LastShiftCalcYRes == RealLy &&
		LastShiftCalcMinMapX == rc.left && LastShiftCalcMinMapY == rc.top &&
		LastShiftCalcMaxMapX == rc.right && LastShiftCalcMaxMapY == rc.bottom &&
		cMath::Equals(LastShiftCalcToEyeEx, (float)g_Navigator.m_Args.toEyeEx); // To avoid recalcing during tend.
}

// MapOptions::MapOptionsLimitCamArgs::Validate
void MapOptions::MapOptionsLimitCamArgs::Validate() {
	extern bool g_SetupCameraLimitCamera;
	g_SetupCameraLimitCamera = false;
	// Storing Prev Cam Args:
	extern float CameraMovingStage;
	float PrevCameraMovingStage = CameraMovingStage;
	CameraMovingStage = 0;
//	float PrevToEyeEx = g_Navigator.m_Args.toEyeEx;
//	g_Navigator.m_Args.toEyeEx = 0.0f;
	float PrevAzim = g_Navigator.m_Args.Azim;
	g_Navigator.m_Args.Azim = 0.0f;
	SetupCamera();
	// Calcing Eye:
	const Frustum Fr = ICam->GetFrustum();
	const Vector3D EyeOrig = 0.25f * (Fr.ltn() + Fr.lbn() + Fr.rtn() + Fr.rbn());
	const Vector3D EyeDir = ICam->GetDir();
	// Calc Central LeftOrig;
	Vector3D LeftOrig;
	Fr.plLeft.intersect(Line3D(EyeOrig, Vector3D(-1.0f, 0.0f, 0.0f)), LeftOrig);
	// Projecting Eye to XY plane:
	Plane XY(Vector3D::null, Vector3D::oZ);
	Vector3D EyePr;
	XY.intersect(Line3D(EyeOrig, EyeDir), EyePr);
	// Central Hor Plane:
	cPlane HorPlane(cVec3(EyeOrig.x, EyeOrig.y, EyeOrig.z), cVec3(EyePr.x, EyePr.y, EyePr.z),
		cVec3(LeftOrig.x, LeftOrig.y, LeftOrig.z));
	cPlane LeftPlane(Fr.plLeft.a, Fr.plLeft.b, Fr.plLeft.c, Fr.plLeft.d);
	cVec3 cLeftDir;
	LeftShift = RightShift = TopShift = BottomShift = 0.0f;
	if(LeftPlane.PlaneIntersection(HorPlane, NULL, &cLeftDir)) {
		Vector3D LeftDir(cLeftDir.x, cLeftDir.y, cLeftDir.z);
		Vector3D LeftPr;
		XY.intersect(Line3D(LeftOrig, LeftDir), LeftPr);
		const Vector3D EyeLeftPrDelta = EyePr - LeftPr;
		LeftShift = EyeLeftPrDelta.x / c_WorldGridStepX;
		RightShift = -LeftShift;
		// TopShift:
		Vector3D BottomOrig;
		Fr.plBottom.intersect(Line3D(EyeOrig, Vector3D(0.0f, 1.0f, 0.0f)), BottomOrig);
		// Central secant Vert Plane:
		cPlane VertPlane(cVec3(EyeOrig.x, EyeOrig.y, EyeOrig.z), cVec3(EyePr.x, EyePr.y, EyePr.z),
			cVec3(BottomOrig.x, BottomOrig.y, BottomOrig.z));
		cPlane BottomPlane(Fr.plBottom.a, Fr.plBottom.b, Fr.plBottom.c, Fr.plBottom.d);
		cVec3 cBottomDir;
		if(BottomPlane.PlaneIntersection(VertPlane, NULL, &cBottomDir)) {
			Vector3D BottomDir(cBottomDir.x, cBottomDir.y, cBottomDir.z);
			Vector3D BottomPr;
			XY.intersect(Line3D(BottomOrig, BottomDir), BottomPr);
			const Vector3D EyeBottomPrDelta = EyePr - BottomPr;
			BottomShift = EyeBottomPrDelta.y / c_WorldGridStepY;
			TopShift = -BottomShift;
		}
	}

	// Restoring Cam Args:
	CameraMovingStage = PrevCameraMovingStage;
	//	g_Navigator.m_Args.toEyeEx = PrevToEyeEx;
	g_Navigator.m_Args.Azim = PrevAzim;
	SetupCamera();

	LastShiftCalcXRes = RealLx;
	LastShiftCalcYRes = RealLy;
	const RECT rc = GetRect();
	LastShiftCalcMinMapX = rc.left;
	LastShiftCalcMaxMapX = rc.right;
	LastShiftCalcMinMapY = rc.top;
	LastShiftCalcMaxMapY = rc.bottom;
	LastShiftCalcToEyeEx = g_Navigator.m_Args.toEyeEx;
	g_SetupCameraLimitCamera = true;
}

// MapOptions::MapOptionsLimitCamArgs::GetRect
const RECT MapOptions::MapOptionsLimitCamArgs::GetRect() {
	if(!UseFromMap) {
		SetFromEngineSettings();
	}
	RECT rc;
	rc.left = MinMapX;
	rc.right = MaxMapX;
	rc.top = MinMapY;
	rc.bottom = MaxMapY;
	return rc;
}

// MapOptions::MapOptionsLimitCamArgs::GetRectFixed
const cBounds MapOptions::MapOptionsLimitCamArgs::GetRectFixed() {
	if(!IsValid()) {
		Validate();
	}
	const RECT rcSrc = GetRect();
	cBounds rc;
	rc.SetMin(cVec3((float)rcSrc.left + LeftShift, (float)rcSrc.top + TopShift, 0.0f));
	rc.SetMax(cVec3((float)rcSrc.right + RightShift, (float)rcSrc.bottom + BottomShift, 0.0f));
	return rc;
}

//*****************************************************************************
// MapOptionsFogArgs
//*****************************************************************************

// MapOptions::MapOptionsFogArgs::CheckIfElementInvisible
bool MapOptions::MapOptionsFogArgs::CheckIfElementInvisible(int Index, void *, DWORD) {
	if(1 == Index) { // PrevUseFromMap
		return true;
	}
	return UseFromMap ? false : Index > 0;
}

// MapOptions::MapOptionsFogArgs::OnChangeMember
bool MapOptions::MapOptionsFogArgs::OnChangeMember(BaseClass *, void *, void *, const char *) {
	if(PrevUseFromMap != UseFromMap) {
		PrevUseFromMap = UseFromMap;
		return true;
	}
	return false;
}

// MapOptions::MapOptionsFogArgs::SetFromEngineSettings
void MapOptions::MapOptionsFogArgs::SetFromEngineSettings() {
	FogType = EngSettings.FogArgs.FogType;
	FogDensity = EngSettings.FogArgs.FogDensity;
	start_FogFarPlane = EngSettings.FogArgs.start_FogFarPlane;
	start_FogNearPlane = EngSettings.FogArgs.start_FogNearPlane;
	final_FogFarPlane = EngSettings.FogArgs.final_FogFarPlane;
	final_FogNearPlane = EngSettings.FogArgs.final_FogNearPlane;
	HorisonColorModulator = EngSettings.FogArgs.HorisonColorModulator;
	HorisonColorBlender = EngSettings.FogArgs.HorisonColorBlender;
	FogRingTotalWidth = EngSettings.FogArgs.FogRingTotalWidth;
	FogRingHorisonWidth = EngSettings.FogArgs.FogRingHorisonWidth;
	FogRingHorMargin = EngSettings.FogArgs.FogRingHorMargin;
	SkyRotationPeriod = EngSettings.FogArgs.SkyRotationPeriod;
	SkyRadius = EngSettings.FogArgs.SkyRadius;
	SkyShift = EngSettings.FogArgs.SkyShift;
	SkyRingScanFromFr = EngSettings.FogArgs.SkyRingScanFromFr;
	SkyRingScanStripeFr = EngSettings.FogArgs.SkyRingScanStripeFr;
	ShowSkyDome = EngSettings.FogArgs.ShowSkyDome;
}

// MapOptions::MapOptionsFogArgs::FogArgsCopy::EvaluateFunction
void MapOptions::MapOptionsFogArgs::FogArgsCopy::EvaluateFunction() {
	char Text[] = "Are you sure you want to copy \"Fog Args\" from EngineSettings?";
	char Caption[] = "Confirm Copy \"Fog Args\" From EngineSettings";
	extern HWND g_hWnd;
	if(MessageBox(g_hWnd, Text, Caption, MB_YESNO) == IDYES) {
		MapOptionsFogArgs *pArgs = dynamic_cast<MapOptionsFogArgs *>(ParentBC);
		if(pArgs) {
			pArgs->SetFromEngineSettings();
		}
	}
}

//*****************************************************************************
// MapOptionsCamArgs
//*****************************************************************************

// MapOptions::MapOptionsCamArgs::CheckIfElementInvisible
bool MapOptions::MapOptionsCamArgs::CheckIfElementInvisible(int Index, void *, DWORD) {
	if(1 == Index) { // PrevUseFromMap
		return true;
	}
	return UseFromMap ? false : Index > 0;
}

// MapOptions::MapOptionsCamArgs::OnChangeMember
bool MapOptions::MapOptionsCamArgs::OnChangeMember(BaseClass *, void *, void *, const char *) {
	if(PrevUseFromMap != UseFromMap) {
		PrevUseFromMap = UseFromMap;
		return true;
	}
	return false;
}

// MapOptions::MapOptionsCamArgs::SetFromEngineSettings
void MapOptions::MapOptionsCamArgs::SetFromEngineSettings() {
	Args = EngSettings.CamArgs.Args;
}

// MapOptions::MapOptionsCamArgs::CamArgsCopy::EvaluateFunction
void MapOptions::MapOptionsCamArgs::CamArgsCopy::EvaluateFunction() {
	char Text[] = "Are you sure you want to copy \"Cam Args\" from EngineSettings?";
	char Caption[] = "Confirm Copy \"Cam Args\" From EngineSettings";
	extern HWND g_hWnd;
	if(MessageBox(g_hWnd, Text, Caption, MB_YESNO) == IDYES) {
		MapOptionsCamArgs *pArgs = dynamic_cast<MapOptionsCamArgs *>(ParentBC);
		if(pArgs) {
			pArgs->SetFromEngineSettings();
		}
	}
}

// MapOptions::reset_class
void MapOptions::Init(){
	for(int i=0;i<=MaxNatColors;i++){
		Players.Player[i].StartRes.Wood=5000;	Players.Player[i].UseStartPos=false;
		Players.Player[i].StartRes.Gold=5000;	Players.Player[i].UseStartPos=false;
		Players.Player[i].StartRes.Stone=5000;	Players.Player[i].UseStartPos=false;
		Players.Player[i].StartRes.Food=5000;	Players.Player[i].UseStartPos=false;
		Players.Player[i].StartRes.Iron=5000;	Players.Player[i].UseStartPos=false;
		Players.Player[i].StartRes.Coal=5000;	Players.Player[i].UseStartPos=false;
	}
	FogColor=0xFFA0A0A0;
//	GetWaterColor=0x3C233246;
	SkyBoxModel=-1;
	IsActive = false;
	GlobalMapScale=256;
};
void MapOptions::reset_class(void *DataPtr) {
	BaseClass::reset_class(this);
	MapOptions();
	BaseClass *pBC = (BaseClass *)DataPtr;
	MapOptions *pMO = dynamic_cast<MapOptions *>(pBC);
	if(pMO) {
		pMO->FogArgs.SetFromEngineSettings();
		pMO->CamArgs.SetFromEngineSettings();
	}
	CameraTracks.Clear();
	HoldBlackScreenTillTrackStarts = false;
	Archways.Clear();
	FogArgs.UseFromMap = FogArgs.PrevUseFromMap = false;
	CamArgs.UseFromMap = CamArgs.PrevUseFromMap = false;
	LimitCamArgs.UseFromMap = LimitCamArgs.PrevUseFromMap = false;
	Plugins.Clear();
	LimitUnitsInSettlement.GetParmList()->CLEAR();
	Init();
}

// MapOptions::LimitUnitsInSettlementRemoveDups
void MapOptions::LimitUnitsInSettlementRemoveDups() {
	// Searching dups from elem #1:
	be_CParamList *pList = LimitUnitsInSettlement.GetParmList();
	for(int i = 1; i < pList->NValues;) {
		be_CPrametrBase *pCurElem = pList->Values[i];
		int j;
		for(j = 0; j < i; j++) {
			be_CPrametrBase *pCmpTo = pList->Values[j];
			if(pCurElem->ParamName.equal(pCmpTo->ParamName) && pCurElem->ParamType.equal(pCmpTo->ParamType)) { // This is dup:
				pList->DelElement(i);
				break;
			}
		}
		if(j == i) { // No dups:
			i++;
		}
	}
}

//*****************************************************************************
// MapOptionsGameSaver
//*****************************************************************************

// MapOptionsGameSaver::OnGameSaving
bool MapOptionsGameSaver::OnGameSaving(xmlQuote &xml) {
	MOptions.Save(xml, &MOptions);
	return true;
}

// MapOptionsGameSaver::OnGameLoading
bool MapOptionsGameSaver::OnGameLoading(xmlQuote &xml) {
	MOptions.reset_class(&MOptions);
	ErrorPager EP(0);
	MOptions.Load(xml, &MOptions, &EP);
//	MOptions.LimitUnitsInSettlementRemoveDups(); // Not needed!
	void CreateHeightmap();
	CreateHeightmap();
	return true;
}

// MapOptionsGameSaver::OnCheatEntering
bool MapOptionsGameSaver::OnCheatEntering(const char *CheatStr) {
	if(cStr::EqualsNoCase(CheatStr, "limitunitsinsettlementremovedups")) {
		MOptions.LimitUnitsInSettlementRemoveDups();
		return true;
	}
	return false;
}