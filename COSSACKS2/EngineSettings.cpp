#include "stdheader.h"
#include "EngineSettings.h"
#include "Camera\Navigator.h"
#include "CurrentMapOptions.h"

//
LocalizationSettings LocSettings;
DWORD GetObjectsShadowColor()
{
    return EngSettings.ShadowsColor;
}

int GetGameIndex()
{
    return EngSettings.GameName;
}

float GetCameraFactor()
{
    return EngSettings.CameraFactor;
}

bool ResourceSettings::SetHint(byte NI, int* Cost, _str* txt){
	if(EnableSprHint){
		txt->Clear();
		(*txt)+="{dy4}";
		for(int i=0;i<6;i++){
			if(Cost[i]){
				//txt->print("{G %d %d %d %d %d %d} ", File, SprHint[i], dx, dy, Lx, Ly);
				txt->print("{G %d %d} ", File, SprHint[i]);
				if(Cost[i]>XRESRC(NI,i)){
					//txt->Add("{CR}");
					txt->Add("{C FFff0000}");
				}else{
					//txt->Add("{CB}");
					txt->Add("{C FFefd7a7}");
				}
				txt->print("%d ",Cost[i]);				
			}
		}
		(*txt)+="{dy4}";
		return true;
	}
	return false;
};

bool cHintTemplate::AddHotKey(_str* txt, int Key){
	if(!HotKey.isClear()){
		txt->print(HotKey.pchar(),Key);
		return true;
	}
	return false;
};
//
void ACT(int x);
extern int TrueTime;
//
bool fes_bLivingPlaces=true;
int fes_tLivingPlaces=0;
int fes_aLivingPlaces=0;
void fes_neLivingPlaces(){
	if(EngSettings.vInterf.neLivingPlaces&&fes_bLivingPlaces){
		byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
		Nation* NT=NATIONS+NI;
		if(fes_tLivingPlaces<TrueTime||fes_aLivingPlaces<NT->NFarms){
			fes_aLivingPlaces=NT->NFarms;
			fes_tLivingPlaces=TrueTime+60000; //EngSettings.vInterf.neTime
			ACT(42);
			AssignHint1(EngSettings.vInterf.neLivingPlaces,160);
			fes_bLivingPlaces=false;
		}
	}
}
//
CEXPORT void AddPulseSquare(int x, int y);
//
int fes_tTownHall=0;
void fes_neTownHall(OneObject* TH){
	if(fes_tTownHall<TrueTime){
		fes_tTownHall=TrueTime+45000;
		ACT(113);
		static char* h=GetTextByID("#TownHallUnderAttack");
		AssignHint1(h,160);
		AddPulseSquare(TH->RealX>>4,TH->RealY>>4);
	}
}
//
int fes_tSklad=0;
void fes_neSklad(){
	if(EngSettings.vInterf.neSklad){
		if(fes_tSklad<TrueTime){
			fes_tSklad=TrueTime+EngSettings.vInterf.neTime;
			AssignHint1(EngSettings.vInterf.neSklad,160);
		}
	}
}
int fes_tResourceForProduce=0;
void fes_neResourceForProduce(char* UnitMess){
	if(EngSettings.vInterf.neResourceForProduce){
		if(fes_tResourceForProduce<TrueTime){
			fes_tResourceForProduce=TrueTime+EngSettings.vInterf.neTime;
			char txt[150];
			sprintf(txt,EngSettings.vInterf.neResourceForProduce,UnitMess);
			AssignHint1(txt,60);
		}
	}
}
bool fes_bAmunition[8192]; // alredy alarmed
int fes_tAmmunition=0;
void fes_neAmmunition(word NIndex, char* Message=NULL){
	if(EngSettings.vInterf.neAmmunition&&!fes_bAmunition[NIndex]){
		if(fes_tAmmunition<TrueTime){
			fes_tAmmunition=TrueTime+EngSettings.vInterf.neTime;
			ACT(40);
			if(!Message){
				Message=EngSettings.vInterf.neAmmunition;
			}
			AssignHint1(Message,160);			
			fes_bAmunition[NIndex]=true;
		}
	}
}
int fes_tSettlementCapture=0;
CIMPORT word GetNTribes();
CEXPORT word GetNTribes(byte Owner);
CEXPORT void fes_neSettlementCapture(byte AgressorNI){	
	if(fes_tSettlementCapture<TrueTime){
		fes_tSettlementCapture=TrueTime+EngSettings.vInterf.neTime;
		byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
		if(AgressorNI==NI){
			ACT(50);
			if(EngSettings.vInterf.neSettlementCaptured) AssignHint1(EngSettings.vInterf.neSettlementCaptured,100);
			LogBattle(NI,"^ST_CAPV^ %d.",GetNTribes(NI),GetNTribes());
		}else{
			ACT(51);
			if(EngSettings.vInterf.neSettlementLost) AssignHint1(EngSettings.vInterf.neSettlementLost,160);
			LogBattle(NI,"^ST_LOSV^ %d.",GetNTribes(NI),GetNTribes());
		}
	}
}
int fes_tErasedBrigade=0;
void fes_neErasedBrigade(byte BrigadeNI){	
	if(fes_tErasedBrigade<TrueTime){
		fes_tErasedBrigade=TrueTime+EngSettings.vInterf.neTime;
		byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
		if(BrigadeNI==NI){
			ACT(73);
			if(EngSettings.vInterf.neBrigadeLost) AssignHint1(EngSettings.vInterf.neBrigadeLost,160);
		}else{
			ACT(74);
			if(EngSettings.vInterf.neBrigadeTerminated) AssignHint1(EngSettings.vInterf.neBrigadeTerminated,160);
		}
	}
}
void fes_Init(){
	fes_tLivingPlaces=0;
	fes_aLivingPlaces=0;
	fes_tSklad=0;
	fes_tResourceForProduce=0;
	fes_tAmmunition=0;
	fes_tSettlementCapture=0;
	fes_tErasedBrigade=0;
	fes_tTownHall=0;
	//
	memset(fes_bAmunition,0,sizeof fes_bAmunition);
}
//
#define alstr(s,x) if(s==NULL){s=(char*)malloc(strlen(x)+1); strcpy(s,x);}
void engInterface::Init(){

	MagicCardsVisibleAmount=9;//15 for Reborn; 9 for 1.1
	//NumOfRaceStyles=2;//2 for 1.1
	VolumeFullScreenBik=100;
	VolumeSoundEvents=100;
	VolumeMissDialogs=100;
	//
	fSettlement=-1;
	sprSettlementAni=true;
	SettlementSpriteHeight=120;
	neTime=8000;
	selBorder=0;
	selColor=0x801010EE; //0x80EBEBEB
	HintX=80;
	HintY=-225;
	svFrescoMusic=40;
	svFrescoVoice=70;
	//
	alstr(neLivingPlaces,"#neLivingPlaces");
	alstr(neSklad,"#neSklad");
	alstr(neResourceForProduce,"#neResourceForProduce");
	alstr(neAmmunition,"#neAmmunition");
	alstr(neSettlementCaptured,"#neSettlementCaptured");
	alstr(neSettlementLost,"#neSettlementLost");
	alstr(neBrigadeTerminated,"#neBrigadeTerminated");
	alstr(neBrigadeLost,"#neBrigadeLost");
}
/*void GetCameraParams(float& FOW0,float& FOW1,float& Angle0,float& Angle1,float& Scale0,float& Scale1){
	FOW0=c_PI/30+EngSettings.FOW_start*c_PI*5/6;
	FOW1=c_PI/30+EngSettings.FOW_final*c_PI*5/6;
	Angle0=EngSettings.HorisontrAngle_start*c_PI/2;
	Angle1=EngSettings.HorisontrAngle_final*c_PI/2;
	Scale0=-(EngSettings.Distance_start)*344;
	Scale1=-(EngSettings.Distance_final)*344;
}*/

void EngineSettings::bf_SetDialogSoundVolume::EvaluateFunction() {
	//ListDesk* LD=get_parent<ListDesk>();
	void ov_SetVolume(BYTE ucVolume,BYTE ucStream);
	ov_SetVolume(GSets.SVOpt.MusicVolume*(GSets.SVOpt.RelativDialogSoundVolume+0.5),1);
}

#include "comms\comms.h"

//--------------------------------------------------------------------------------------
// ThirdPersonCameraArgs
//--------------------------------------------------------------------------------------
void EngineSettings::ThirdPersonCameraArgs::ThirdPersonCameraReset::EvaluateFunction() {
	char Text[] = "Are you sure you want to reset third person camera to defaults?";
	char Caption[] = "Confirm Reset Third Person Camera";
	extern HWND g_hWnd;
	if(MessageBox(g_hWnd, Text, Caption, MB_YESNO) == IDYES) {
		EngineSettings::ThirdPersonCameraArgs *pArgs = (EngineSettings::ThirdPersonCameraArgs *)ParentBC;
		assert(pArgs);
		pArgs->Defaults();
	}
}

void EngineSettings::ThirdPersonCameraArgs::Defaults() {
	ForceCameraParams = true;
	final_FogNearPlane = 1000;
	final_FogFarPlane = 1500;
	CameraMovingStage = 1.0f;
	FlyingUnitZOffset = 0;
	GroundUnitZOffset = 0;
	ElevOffset = 30;
	toEye = 237;
	GroundUnitMinRotator = 2;
	WASD = true;
	ESDF = false;
}

bool EngineSettings::ThirdPersonCameraArgs::CheckIfElementInvisible(int index, void *, DWORD) {
	return false;
}

bool EngineSettings::ThirdPersonCameraArgs::GetClassPresentationMask(_str &Mask, void *, void *, const char *, DWORD) {
	Mask = "Third Person Camera";
	return true;
}

bool EngineSettings::ThirdPersonCameraArgs::OnChangeMember(BaseClass *, void *, void *, const char *Name) {
	if(!strcmp(Name, "WASD")) {
		if(WASD) ESDF = false;
		else if(!ESDF) WASD = true;
	}
	if(!strcmp(Name, "ESDF")) {
		if(ESDF) WASD = false;
		else if(!WASD) ESDF = true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// CollidingCameraArgs
//-----------------------------------------------------------------------------
void EngineSettings::CollidingCameraArgs::CollidingCameraReset::EvaluateFunction() {
	char Text[] = "Are you sure you want to reset colliding camera to defaults?";
	char Caption[] = "Confirm Reset Colliding Camera";
	extern HWND g_hWnd;
	if(MessageBox(g_hWnd, Text, Caption, MB_YESNO) == IDYES) {
		EngineSettings::CollidingCameraArgs *pArgs = (EngineSettings::CollidingCameraArgs *)ParentBC;
		assert(pArgs);
		pArgs->Defaults();
	}
}

void EngineSettings::CollidingCameraArgs::Defaults() {
	Radius = 120;
	Mass = 10;
	Bump = 10;
	AirFriction = 0.01f;
	ContactFriction = 0.05f;
    MoveForce = 300;
	JumpForce = 600;
	Gravity = 15;
	MaxVelocity = 1000;
	WASD = true;
	ESDF = false;
    MouseSpeed = 0.002f;
	HideUI = true;
	pForceCameraParams = ForceCameraParams = true;
    final_FogNearPlane = 1000;
	final_FogFarPlane = 1500;
	CameraMovingStage = 1.0f;
	SkyShift = -2000;
	TimeStep = 0.05f;
	TimeScale = 0.01f;
	TimeOut = 500;
	Floatage = 0.2f;
}

bool EngineSettings::CollidingCameraArgs::CheckIfElementInvisible(int index, void *, DWORD) {
	if(ForceCameraParams) return false;
	else return cMath::IsInRange(index, 18, 20);
}

bool EngineSettings::CollidingCameraArgs::GetClassPresentationMask(_str &Mask, void *, void *, const char *, DWORD) {
	Mask = "Colliding Camera";
	return true;
}

bool EngineSettings::CollidingCameraArgs::OnChangeMember(BaseClass *, void *, void *, const char *Name) {
	if(!strcmp(Name, "WASD")) {
		if(WASD) ESDF = false;
		else if(!ESDF) WASD = true;
	}
	if(!strcmp(Name, "ESDF")) {
		if(ESDF) WASD = false;
		else if(!WASD) ESDF = true;
	}
	if(pForceCameraParams != ForceCameraParams) {
		pForceCameraParams = ForceCameraParams;
		return true;
	}
	return false;
}

// ENUM:RTSMODECAMERACONTROLS
DWORD FN_ENUM_RTSMODECAMERACONTROLS(RefreshableEnumeratorContext *in, EnumeratorOutputStream *out, DWORD PrevCallHash) {
	if(PrevCallHash == 3) return PrevCallHash;
	out->add("Rotate", EngineSettings::RTSModeCameraArgs::ROTATE, NULL);
	out->add("Dolly", EngineSettings::RTSModeCameraArgs::DOLLY, NULL);
	out->add("Lift", EngineSettings::RTSModeCameraArgs::LIFT, NULL);
	return 3;
}

void CREATE_ENUM_RTSMODECAMERACONTROLS() {
	Enumerator *pEnum = ENUM.Get("RTSMODECAMERACONTROLS");
	assert(pEnum);
	pEnum->SetProcEnum(new RefreshableEnumerator(&FN_ENUM_RTSMODECAMERACONTROLS));
} // ENUM: RTSMODECAMERACONTROLS


//-----------------------------------------------------------------------------
// RTSModeCameraArgs
//-----------------------------------------------------------------------------
void EngineSettings::RTSModeCameraArgs::Defaults() {
	LiftSpeed = RTSModeCameraArgs_DefLiftSpeed;
	LiftSpringConstant = RTSModeCameraArgs_DefLiftSpringConstant;

	DollyMin = RTSModeCameraArgs_DefDollyMin;
	DollyMax = RTSModeCameraArgs_DefDollyMax;
	DollySpeed = RTSModeCameraArgs_DefDollySpeed;
	DollySpringConstant = RTSModeCameraArgs_DefDollySpringConstant;

	RotationSpeed = RTSModeCameraArgs_DefRotationSpeed;
	RotationSpringConstant = RTSModeCameraArgs_DefRotationSpringConstant;

	OnMouseWheel = 0;
	OnInsertDelete = 1;
	OnPageUpPageDown = 2;
}

void EngineSettings::RTSModeCameraArgs::RTSModeCameraArgsReset::EvaluateFunction() {
	char Text[] = "Are you sure you want to reset \"RTS Mode Camera\" to defaults?";
	char Caption[] = "Confirm Reset \"RTS Mode Camera\"";
	extern HWND g_hWnd;
	if(MessageBox(g_hWnd, Text, Caption, MB_YESNO) == IDYES) {
		EngineSettings::RTSModeCameraArgs *pArgs = dynamic_cast<EngineSettings::RTSModeCameraArgs *>(ParentBC);
		if(pArgs) {
			pArgs->Defaults();
		}
	}
}

bool EngineSettings::RTSModeCameraArgs::GetClassPresentationMask(_str &Mask, void *, void *, const char *, DWORD) {
	Mask = "RTS Mode Camera";
	return true;
}

//-----------------------------------------------------------------------------
// TankController
//-----------------------------------------------------------------------------
void EngineSettings::TankControllerArgs::Defaults() {
	ElevationShift = TankControllerArgs_DefElevationShift;
	ZOffset = TankControllerArgs_DefZOffset;
	ToEye = TankControllerArgs_DefToEye;
}

void EngineSettings::TankControllerArgs::TankControllerArgsReset::EvaluateFunction() {
	char Text[] = "Are you sure you want to reset \"Tank Controller\" to defaults?";
	char Caption[] = "Confirm Reset \"Tank Controller\"";
	extern HWND g_hWnd;
	if(MessageBox(g_hWnd, Text, Caption, MB_YESNO) == IDYES) {
		EngineSettings::TankControllerArgs *pArgs = dynamic_cast<EngineSettings::TankControllerArgs *>(ParentBC);
		if(pArgs) {
			pArgs->Defaults();
		}
	}
}

bool EngineSettings::TankControllerArgs::GetClassPresentationMask(_str &Mask, void *, void *, const char *, DWORD) {
	Mask = "Tank Controller";
	return true;
}

//-----------------------------------------------------------------------------
// CamArgs
//-----------------------------------------------------------------------------

// EngineSettings::EngineSettingsCamArgs::SetDefaults
void EngineSettings::EngineSettingsCamArgs::SetDefaults() {
	Args.YFov_fm = EngineSettingsCamArgs_DefYFov_fm;
	Args.YFov_to = EngineSettingsCamArgs_DefYFov_to;
	Args.Pitch_fm = EngineSettingsCamArgs_DefPitch_fm;
	Args.Pitch_to = EngineSettingsCamArgs_DefPitch_to;
	Args.ToEye_fm = EngineSettingsCamArgs_DefToEye_fm;
	Args.ToEye_to = EngineSettingsCamArgs_DefToEye_to;
	Args.ZNear = EngineSettingsCamArgs_DefZNear;
	Args.ZFarFogFarDelta = EngineSettingsCamArgs_DefZFarFogFarDelta;
	Args.FacturesScale = EngineSettingsCamArgs_DefFacturesScale;
}

// EngineSettings::EngineSettingsCamArgs::EvaluateFunction
void EngineSettings::EngineSettingsCamArgs::CamArgsDefaults::EvaluateFunction() {
	char Text[] = "Are you sure you want to set \"Cam Args\" to defaults?";
	char Caption[] = "Confirm Set \"Cam Args\" To Defaults";
	extern HWND g_hWnd;
	if(MessageBox(g_hWnd, Text, Caption, MB_YESNO) == IDYES) {
		EngineSettings::EngineSettingsCamArgs *pArgs = dynamic_cast<EngineSettings::EngineSettingsCamArgs *>(ParentBC);
		if(pArgs) {
			pArgs->SetDefaults();
		}
	}
}

// EngineSettings::EngineSettingsCamArgs::GetYFovDeg
float EngineSettings::EngineSettingsCamArgs::GetYFovDeg() const {
	extern float CameraMovingStage;
	return cMath::Lerp(Args.YFov_fm, Args.YFov_to, CameraMovingStage);
}

// EngineSettings::EngineSettingsCamArgs::GetPitchDeg
float EngineSettings::EngineSettingsCamArgs::GetPitchDeg() const {
	extern float CameraMovingStage;
	return cMath::Lerp(Args.Pitch_fm, Args.Pitch_to, CameraMovingStage);
}

// EngineSettings::EngineSettingsCamArgs::GetYFovRad
float EngineSettings::EngineSettingsCamArgs::GetYFovRad() const {
	return cMath::Rad(GetYFovDeg());
}

// EngineSettings::EngineSettingsCamArgs::GetPitchRad
float EngineSettings::EngineSettingsCamArgs::GetPitchRad() const {
	return cMath::Rad(GetPitchDeg());
}

// EngineSettings::EngineSettingsCamArgs::GetToEye
float EngineSettings::EngineSettingsCamArgs::GetToEye() const {
	extern float CameraMovingStage;
	return cMath::Lerp(Args.ToEye_fm, Args.ToEye_to, CameraMovingStage);
}

// EngineSettings::EngineSettingsCamArgs::GetZNear
float EngineSettings::EngineSettingsCamArgs::GetZNear() const {
	return Args.ZNear;
}

// EngineSettings::EngineSettingsCamArgs::GetZFar
float EngineSettings::EngineSettingsCamArgs::GetZFar() const {
	return MOptions.FogArgs.GetFogFarPlane() + Args.ZFarFogFarDelta;
}

// EngineSettings::EngineSettingsCamArgs::GetFacturesScale
float EngineSettings::EngineSettingsCamArgs::GetFacturesScale() const {
	return Args.FacturesScale;
}

// EngineSettings::EngineSettingsCamArgs::Push
void EngineSettings::EngineSettingsCamArgs::Push() {
	m_Stack.Add(Args);
}

// EngineSettings::EngineSettingsCamArgs::Pop
bool EngineSettings::EngineSettingsCamArgs::Pop() {
	if(!m_Stack.Count()) {
		return false;
	}
	Args = m_Stack.GetBack();
	m_Stack.RemoveBack();
	return true;
}

// EngineSettings::EngineSettingsCamArgs::EngineSettingsCamArgsStruct::ForceSimplification
bool EngineSettings::EngineSettingsCamArgs::EngineSettingsCamArgsStruct::ForceSimplification() {
	return true;
}

//-----------------------------------------------------------------------------
// FogArgs
//-----------------------------------------------------------------------------
void EngineSettings::EngineSettingsFogArgs::SetDefaults() {
	FogType = 3;
	FogDensity = 0.0003f;
	start_FogNearPlane = 3100.0f;
	start_FogFarPlane = 4050.0f;
	final_FogNearPlane = 1500.0f;
	final_FogFarPlane = 4200.0f;
	FogRingTotalWidth = 768;
	FogRingHorisonWidth = 64;
	HorisonColorModulator = 0xFF949BAC;
	HorisonColorBlender = 0x007ABFFD;
	SkyRotationPeriod = 1200;
	SkyRadius = EngineSettings_DefSkyRadius;
	SkyShift = EngineSettings_DefSkyShift;
	SkyRingScanFromFr = EngineSettings_DefSkyRingScanFromFr;;
	SkyRingScanStripeFr = EngineSettings_DefSkyRingScanStripeFr;
	FogRingHorMargin = EngineSettings_DefFogRingHorMargin;
	ShowSkyDome = true;
}

void EngineSettings::EngineSettingsFogArgs::FogArgsDefaults::EvaluateFunction() {
	char Text[] = "Are you sure you want to set \"Fog Args\" to defaults?";
	char Caption[] = "Confirm Set \"Fog Args\" To Defaults";
	extern HWND g_hWnd;
	if(MessageBox(g_hWnd, Text, Caption, MB_YESNO) == IDYES) {
		EngineSettings::EngineSettingsFogArgs *pArgs = dynamic_cast<EngineSettings::EngineSettingsFogArgs *>(ParentBC);
		if(pArgs) {
			pArgs->SetDefaults();
		}
	}
}

// EngineSettings::EngineSettingsFogArgs::GetFogNearPlane
float EngineSettings::EngineSettingsFogArgs::GetFogNearPlane() const {
	extern float CameraMovingStage;
	return cMath::Lerp(start_FogNearPlane, final_FogNearPlane, CameraMovingStage) + g_Navigator.m_Args.toEyeEx;

}

// EngineSettings::EngineSettingsFogArgs::GetFogFarPlane
float EngineSettings::EngineSettingsFogArgs::GetFogFarPlane() const {
	extern float CameraMovingStage;
	return cMath::Lerp(start_FogFarPlane, final_FogFarPlane, CameraMovingStage) + g_Navigator.m_Args.toEyeEx;
}

//*****************************************************************************
// LimitCamArgs
//*****************************************************************************

// EngineSettings::EngineSettingsLimitCamArgs::SetDefaults
void EngineSettings::EngineSettingsLimitCamArgs::SetDefaults() {
	MinMapXEdit = 0;
	MaxMapXEdit = 500;
	MinMapYEdit = 0;
	MaxMapYEdit = 500;
	MinMapXGame = 9;
	MaxMapXGame = 478;
	MinMapYGame = 20;
	MaxMapYGame = 490;
}

// EngineSettings::EngineSettingsLimitCamArgs::LimitCamArgsDefaults::EvaluateFunction
void EngineSettings::EngineSettingsLimitCamArgs::LimitCamArgsDefaults::EvaluateFunction() {
	char Text[] = "Are you sure you want to set \"Limit Cam Args\" to defaults?";
	char Caption[] = "Confirm Set \"Limit Cam Args\" To Defaults";
	extern HWND g_hWnd;
	if(MessageBox(g_hWnd, Text, Caption, MB_YESNO) == IDYES) {
		EngineSettings::EngineSettingsLimitCamArgs *pArgs = dynamic_cast<EngineSettings::EngineSettingsLimitCamArgs *>(ParentBC);
		if(pArgs) {
			pArgs->SetDefaults();
		}
	}
}

// EngineSettings::EngineSettingsLimitCamArgs::GetRect
const RECT EngineSettings::EngineSettingsLimitCamArgs::GetRect() const {
	RECT rc;
	extern bool EditMapMode;
	if(EditMapMode) {
		rc.left = MinMapXEdit;
		rc.right = MaxMapXEdit;
		rc.top = MinMapYEdit;
		rc.bottom = MaxMapYEdit;
	} else {
		rc.left = MinMapXGame;
		rc.right = MaxMapXGame;
		rc.top = MinMapYGame;
		rc.bottom = MaxMapYGame;
	}
	return rc;
}