#include "stdheader.h"
#include "BE_HEADERS.h"
#include "Pinger.h"
#include "CurrentMapOptions.h"
#include "Camera\Navigator.h"

#define MinPing0 80

extern City CITY[8];
extern CCommCore IPCORE;
extern char PlName[64];
extern MapOptions MOptions;
void CreateNationalMaskForMap(char* Name);
void PlayGame();
void PrepareGameMedia(byte myid,bool SaveNR);
extern bool GameExit;
byte vmDifficulty[8];
//
void ClearLoadMark();
void AddLoadMark(char* Mark, int Value);
void ShowLoadProgress(char* Mark, int v, int vMax);
//
void SetRoomOptions(char* s);
DWORD GetObjectsShadowQuality()
{
    return min(GSets.ShadowQuality,1);
}
extern word NPlayers;
void CurrentGameInfo::StartSinglePlayerGame(const char* MapName,int Color){
	strcpy(GSets.CGame.cgi_CurrentMap,MapName);
	extern short WheelDelta;
	WheelDelta=0;

	NPlayers=1;	
	SetMyNation(Color);
	CreateNationalMaskForMap(cgi_CurrentMap);	
	PrepareGameMedia(0,0);
	GameExit=false;
	strcpy(PL_INFO[0].name,PlName);
	DriveMode()->StartMission();	
	for(int i=0;i<8;i++){		
		CITY[i].Difficulty=vmDifficulty[i];
	}
}
void CurrentGameInfo::PlaySinglePlayerGame(){
	PlayGame();	
}
void CurrentGameInfo::PostSinglePlayerGame(){	
	memset(vmDifficulty,0,sizeof vmDifficulty);
}
void CurrentGameInfo::RunSingleplayerGame(byte Color){	
	
	// обнулить колесико
	extern short WheelDelta;
	WheelDelta=0;

	NPlayers=1;	
	SetMyNation(Color);
	CreateNationalMaskForMap(cgi_CurrentMap);	
	PrepareGameMedia(0,0);
	GameExit=false;
	strcpy(PL_INFO[0].name,PlName);
	DriveMode()->StartMission();	
	for(int i=0;i<8;i++){		
		CITY[i].Difficulty=vmDifficulty[i];
	}	
	PlayGame();
	memset(vmDifficulty,0,sizeof vmDifficulty);
}
extern word NPlayers;
void RunInetStart(){
	extern bool UseGSC_Login;
	if(!UseGSC_Login)return;
	char* PLAYERS[8];
	int Profiles[8];
	char NAT[8][32];
	char* Nations[8];
	int Teams[8];
	int Colors[8];
	for(int i=0;i<NPlayers;i++){
		PLAYERS[i]=GSets.CGame.PL_INFO[i].name;
		sprintf(NAT[i],"%d",GSets.CGame.PL_INFO[i].NationID+80);//+48
		Nations[i]=NAT[i];
		Profiles[i]=GSets.CGame.PL_INFO[i].ProfileID;
		Teams[i]=GSets.CGame.PL_INFO[i].GroupID;
		Colors[i]=GSets.CGame.PL_INFO[i].ColorID;
	};
	StartGSCGame("",GSets.CGame.PL_INFO[0].MapName,NPlayers,Profiles,Nations,Teams,Colors);
	extern bool NeedToReportInGameStats;
	NeedToReportInGameStats=1;
	extern int LastTimeReport_tmtmt;
	LastTimeReport_tmtmt=0;
}
void CurrentGameInfo::RunHostGame(){
	IPCORE.DisableServer();
	IPCORE.CloseSession();	
	bool StartIGame(bool SINGLE);
	StartIGame(0);
	GameExit=false;
	CurState=3;
	RunInetStart();
	SetRoomOptions("");
	PlayGame();	
}
void CurrentGameInfo::RunClientGame(){	
	GameExit=false;
	CurState=3;
	RunInetStart();
	SetRoomOptions("");
	PlayGame();
}
extern int CurrentMaxPing[8];
extern int MaxPingTime;
extern DWORD MyDPID;
extern word NPlayers;
int GetMaxRealPing();
int GetRealTime();
bool PIEnumeratePlayers(PlayerInfo* PIN,bool DoMsg);
bool SendToAllPlayers(DWORD Size,LPVOID lpData);
void SETPLAYERDATA(DWORD ID,void* Data,int size,bool change);
CEXPORT
void SendPings();
void CurrentGameInfo::ProcessRoom(){
	if(CurState!=1&&CurState!=2)return;
	extern bool GameInProgress;
	if(GameInProgress)return;
	GSets.CGame.UnitTransform=true;
	bool Host=CurState==1;
	static PlayerInfo MYPINF;
	BEGIN_ONCE
	memset(&MYPINF,0,sizeof MYPINF);
	END_ONCE
	//analysing ping
	SendPings();
	PlayerInfo* PI=GSets.CGame.GetCurrentPlayerInfo();
	int GetMyProfile();
	if(PI)PI->ProfileID=GetMyProfile();
	if(Host){
		PlayerInfo* PI=GetCurrentPlayerInfo();
		if(PI){
			PI->Host=1;
			strcpy(cgi_CurrentMap,PI->MapName);
			DWORD GetMapHash(char* Name);
			PI->MapHashValue=GetMapHash(PI->MapName);
			PI->ProfileID=GetMyProfile();
			extern word COMPSTART[8];
			memcpy(PI->COMPINFO,COMPSTART,16);
			static int prevCommTime=GetTickCount();
			if(GetTickCount()-prevCommTime>6000 || GSets.RoomParams.CheckChanges()){
				//translating common parameters
				int L=0;
                byte* p=GSets.RoomParams.GetCompressed(L);
				DWORD* tos=new DWORD[12+L/4];
				tos[0]='COMM';
				tos[1]=L;
				memcpy(tos+2,p,L);
				SendToAllPlayers(L+8,tos);
				delete[]tos;
				prevCommTime=GetTickCount();
			}
		}
	}else{
		if(PI){
			PlayerInfo* SPI=GSets.CGame.GetHostPlayerInfo();
			if(SPI){
				extern word COMPSTART[8];
				memcpy(COMPSTART,SPI->COMPINFO,16);
			}
		}
	}
	for(int p=NPlayers;p<8;p++)CurrentMaxPing[p]=-1;
	if(PSUMM.NPL&&PSUMM.PSET[0].NPings){
		int png=GetMaxRealPing();
		static int PPTIME=GetTickCount()-1000;
		if(GetRealTime()-PPTIME>1000){
			DWORD lpp[3];
			lpp[0]='PNTF';
			lpp[1]=png;
			lpp[2]=lpp[0]+lpp[1];
			SendToAllPlayers(12,lpp);
			PPTIME=GetRealTime();
			for(int p=0;p<NPlayers;p++)if(GSets.CGame.PL_INFO[p].PlayerID==MyDPID)CurrentMaxPing[p]=png;
		};
		for(int p=0;p<NPlayers;p++)if(CurrentMaxPing[p]>png)png=CurrentMaxPing[p];
		MaxPingTime=png;
		if(png<MinPing0)MaxPingTime=0;
		if(png>=MinPing0&&png<200)MaxPingTime=200;
		if(MaxPingTime)MaxPingTime+=150;
		//_str cc2;
		//CreateDiffStr(cc2);
		//sprintf(ccc,"max ping: %d  (%s)",png,cc2.pchar());		
	}
	//analysing change of player data
	for(int i=0;i<8;i++){
		if(i<NPlayers){
			if(GSets.CGame.PL_INFO[i].PlayerID==MyDPID){
				bool ch=memcmp(&MYPINF,GSets.CGame.PL_INFO+i,sizeof MYPINF)!=0;
				static int PREVSD=GetTickCount()-3000;
				if(ch||GetTickCount()-PREVSD>3000){					
					SETPLAYERDATA(MyDPID,&PL_INFO[i].NationID,sizeof(PlayerInfo)-36,ch);					
					memcpy(&MYPINF,PL_INFO+i,sizeof MYPINF);
					PREVSD=GetTickCount();
				};
				if(ch&&Host)PSUMM.ClearPingInfo();				
			}
		}
	}
	PIEnumeratePlayers(PL_INFO,true);
	if(NPlayers>7)NPlayers=7;
	void AnalyseMessages();
	AnalyseMessages();
}
bool CreateMultiplaterInterface();
extern int CurStatus;
extern bool GameInProgress;
extern int GLOBALTIME;
extern int PGLOBALTIME;
extern int PitchTicks;
extern word PlayerMenuMode;
extern int tmtmt;
void ClearCTime();
void ReceiveAll();
extern bool DoNewInet;
bool CurrentGameInfo::CreateRoom(char* RoomName,int MaxPlayers){
	void PerformCheckCD();
	PerformCheckCD();
	CreateMultiplaterInterface();
	bool CreateSession(char* SessName,char* Name,DWORD User2,bool Style,int MaxPlayers);
	if(CreateSession(RoomName,PlName,0,1,MaxPlayers)){
		memset(CurrentMaxPing,0xFF,4*8);
		void ClearLPACK();
		ClearLPACK();
		ReceiveAll();
		CurStatus=0;
		PitchTicks=8;
		MaxPingTime=0;
		ClearCTime();
		PlayerMenuMode=1;
		GameInProgress=0;
		tmtmt=0;
		REALTIME=0;
		GLOBALTIME=0;
		PGLOBALTIME=0;
		PIEnumeratePlayers(PL_INFO,false);
		PL_INFO[0].Host=1;		
		IPCORE.EnableServer();
		CurState=1;
		PSUMM.ClearPingInfo();
		return true;
	}else return false;
}
bool FindSessionAndJoin(char* Name,char* Nick,bool Style);
bool CurrentGameInfo::JoinRoom(char* ip,char* RoomName){
	DoNewInet=true;
	CreateMultiplaterInterface();
	extern char IPADDR[128];
	strcpy(IPADDR,ip);
	if(FindSessionAndJoin(RoomName,PlName,1)){
		memset(CurrentMaxPing,0xFF,4*8);
		ReceiveAll();
		void ClearLPACK();
		ClearLPACK();
		CurStatus=0;
		PitchTicks=8;
		MaxPingTime=0;
		ClearCTime();
		PlayerMenuMode=1;
		GameInProgress=0;
		tmtmt=0;
		REALTIME=0;
		GLOBALTIME=0;
		PGLOBALTIME=0;
		PIEnumeratePlayers(PL_INFO,false);		
		CurState=2;
		PSUMM.ClearPingInfo();
		void PerformCheckCD();
		PerformCheckCD();
		return true;
	}return false;
}
void CloseMPL();
CIMPORT
void LeaveGSCRoom();
void CurrentGameInfo::LeaveRoom(){
	CloseMPL();
	CurState=0;
	MyDPID=0;
	SetRoomOptions("");
	extern bool UseGSC_Login;
	if(UseGSC_Login)LeaveGSCRoom();
}
PlayerInfo* CurrentGameInfo::GetCurrentPlayerInfo(){
	for(int i=0;i<NPlayers;i++)if(PL_INFO[i].PlayerID==MyDPID)return PL_INFO+i;
	return NULL;
}
PlayerInfo* CurrentGameInfo::GetHostPlayerInfo(){
	for(int i=0;i<NPlayers;i++)if(PL_INFO[i].Host)return PL_INFO+i;
	return NULL;
}
int GetReadyPercent();
int CurrentGameInfo::GetReadyPercent(){
	return ::GetReadyPercent();
};
int GetPing(DPID pid);
int CurrentGameInfo::GetPing(DWORD PlayerID){
	return ::GetPing(PlayerID);
};
bool CurrentGameInfo::Kick(DWORD PlayerID){
	PlayerInfo* I=GetCurrentPlayerInfo();
	if(I&&I->Host){
		return IPCORE.DeletePeer(PlayerID);
	}
	return false;
};
//
EngineSettings EngSettings;
//
void GameSettings::Init(){
	CheckG17_dates=false;
	AnimQuality=1;
	AllowFormationsStatesProcessing=0;
	ShowFPS=false;
	ShowWayOnRoad=true;
	GameDayLength=600;
	HintDebugMode=false;
	PeasantAutoWork=true;
	ShadowQuality=0; // high shadow quality by default
	EnableSubtitres=true;
	AutoSavePeriod=3;
	Localization=0;
}
void GameSettings::reset_class(void* DataPtr){
	GameClass::reset_class(DataPtr);
	SelBarGP=GPS.PreLoadGPImage("interf3\\selbar");
	((GameSettings*)DataPtr)->Init();
}


extern bool InEdit;

bool CurrentGameInfo::isHumanPlayer(byte Color){
	if(InEdit){
		return false;
	}
	if(NPlayers==1){
		return Color==MyNation;
	}else
	for(int i=0;i<NPlayers;i++){
		PlayerInfo* PI=PL_INFO+i;
		if(PI->ColorID==Color){
			return true;
		}
	}
	return false;
}
bool CLeibzigDemo::StartRecord(){
	int nr=RecordsList.GetAmount();
	if(nr){
		int p=rand()%nr;
		char* rname=GetStr(&RecordsList,p);
		//HideBorderMode=1;
		//NoMoveMode=1;
		void PlayRecfile(char*);
		//IRS->ClearDeviceTarget();
		PlayRecfile(rname);
		//if(!GSets.LeibzigDemo.RecBroken)return 0;
		//HideBorderMode=0;
		//NoMoveMode=0;
		return true;
	}
	return false;
};
//
CEXPORT int GetCDVolume();
void SetCDVolumeEx(int Vol);
//
bool CLeibzigDemo::StartVideo(){
	Lpressed=false;
	void PlayFullscreenVideo(char* name,float px,float py, bool Skipable=true);
	int nr=GSets.LeibzigDemo.VideoList.GetAmount();
/*
	if(nr){
		int p=rand()%nr;
		char* rname=GSets.LeibzigDemo.GetStr(&GSets.LeibzigDemo.VideoList,p);
		PlayFullscreenVideo(rname,float(100-GSets.LeibzigDemo.VideoScaleX)/100.0f,float(100-GSets.LeibzigDemo.VideoScaleY)/100.0f);
		return true;
	}
*/
	for(int p=0;p<nr;p++){		
		char* rname=GSets.LeibzigDemo.GetStr(&GSets.LeibzigDemo.VideoList,p);
		PlayFullscreenVideo(rname,float(100-GSets.LeibzigDemo.VideoScaleX)/100.0f,float(100-GSets.LeibzigDemo.VideoScaleY)/100.0f);
		//return true;
	}	
	return false;
};
bool IsCameraLocked(){
	return !(GSets.CGame.ViewMask& 4); 
}

//////////////////////////////////////////////////////////////////////////
//CommonRoomParams
//////////////////////////////////////////////////////////////////////////

CEXPORT bool isInCampaign();

void CommonRoomParams::Init(){
	GameSpeed=1; // Normal
	for(int i=0;i<6;i++){
		PlayerParams* PP=new PlayerParams;
		Players.Add(PP);
	}
}

void CommonRoomParams::InitBeforeGame(){
	switch(GameSpeed){
		case 0://Slow
			GSets.SVOpt.RequiredMsPerFrame=30;
			break;
		case 1://Normal
			GSets.SVOpt.RequiredMsPerFrame=40;
			break;
		case 2://Fast
			GSets.SVOpt.RequiredMsPerFrame=60;
			break;
		default:
			GSets.SVOpt.RequiredMsPerFrame=30/GameSpeed;
			break;
	}

	if( isInCampaign() ){
		GSets.SVOpt.RequiredMsPerFrame=EngSettings.DefaultGameSpeedForCampaign;
	}

	/* ---- EDITED BY XENO ----
	Now there is no need to add case to the switch, the value is setted by parsing the name
	of the variable.

	int R=1000;
	switch(StartRes){
		case 1:
            R=3000;
			break;
		case 2:
			R=10000;
			break;
		case 3: 
			R=50000; 
			break; 
		case 4: 
			R=1000000; 
			break;
	}*/
	extern char* _STARTRES[];
	int R = atoi(_STARTRES[StartRes]+strlen("#mpopt_res_"));


	extern int RES[8][8];
	for(int i=0;i<8;i++){
		for(int j=0;j<6;j++){
            RES[i][j]=R;
			SetXRESRC(i,j,R);
		}
	}
	if(GameType){
		int t=100000;
		extern char* _GAMETYPE[];
        t=atoi(_GAMETYPE[GameType]+strlen("#mpopt_ScoreGame_"));
		for(int i=0;i<7;i++){
			GSets.CGame.PL_INFO[i].VictCond=1;
			GSets.CGame.PL_INFO[i].GameTime=t*60;
		}
	}

	if (MOptions.RPGOnly) GSets.RoomParams.RTS_mode = 2;
	else if (MOptions.RTSOnly) GSets.RoomParams.RTS_mode = 1;

	if (MOptions.ThirdPersonGame) g_Navigator.LinkToUnitToggle();
}
byte* CommonRoomParams::GetCompressed(int& L){
    xmlQuote xml;
	Save(xml,this);
	DString d;
	xml.GetXMLSource(&d);
	if(tempBuf)delete[]tempBuf;
	tempBuf=new byte[d.L+1];
	memcpy(tempBuf,d.pchar(),d.L+1);
	L=d.L+1;
	return tempBuf;
}
void CommonRoomParams::SetCompressed(byte* data,int size){
    xmlQuote xml;
	xml.Load((char*)data);
	ErrorPager ep;
	Load(xml,this,&ep);
}
bool CommonRoomParams::CheckChanges(){
	xmlQuote xml;
	Save(xml,this);
	DString d;
	xml.GetXMLSource(&d);
	bool ch=d.equal(prevXml);
	prevXml=d;
	return !ch;
}
int CommonRoomParams::GetNParams(){
    int n=GetAmountOfElements();
	int nv=0;
	for(int i=0;i<n;i++){
		BaseClass* b=GetElementClass(i);
		if(!strcmp(b->GetClassName(),"_index")){
			nv++;
		}
	}
	return nv;
}
Enumerator* CommonRoomParams::GetEnum(int index){
	int n=GetAmountOfElements();
	int nv=0;
	for(int i=0;i<n;i++){
		BaseClass* b=GetElementClass(i);
		if(!strcmp(b->GetClassName(),"_index")){
			if(nv==index){
                return (Enumerator*)GetElementExtraPtr(i);
			}
			nv++;			
		}
	}
	return NULL;	
}

char* CommonRoomParams::GetFieldValue(int index){
	int n=GetAmountOfElements();
	int nv=0;
	for(int i=0;i<n;i++){
		BaseClass* b=GetElementClass(i);
		if(!strcmp(b->GetClassName(),"_index")){
			if(nv==index){
				Enumerator* E=(Enumerator*)GetElementExtraPtr(i);
				if(E){
					return GetTextByID(E->Get(*((DWORD*)GetElementPtr(i,this))));
				}
				return "";
			}
			nv++;
		}
	}
	return "";
}

void CommonRoomParams::SetFieldValue(int index,char* value){
	int n=GetAmountOfElements();
	int nv=0;
	for(int i=0;i<n;i++){
		BaseClass* b=GetElementClass(i);
		if(!strcmp(b->GetClassName(),"_index")){
			if(nv==index){
				Enumerator* E=(Enumerator*)GetElementExtraPtr(i);
				if(E){
					char* GetIDByText(char*);
					char* ID=GetIDByText(value);
					DWORD v=E->Get(ID);
					*((int*)GetElementPtr(i,this))=v;
				}
				return;
			}
			nv++;			
		}
	}	
}
char* CommonRoomParams::GetFieldName(int index){
	int n=GetAmountOfElements();
	int nv=0;
	for(int i=0;i<n;i++){
		BaseClass* b=GetElementClass(i);
		if(!strcmp(b->GetClassName(),"_index")){
			if(nv==index){
				return (char*)GetElementID(i);
			}
			nv++;			
		}
	}
	return "";
}

//-----------------------------------------------------------------------------
// AntialiasingArgs
//-----------------------------------------------------------------------------
void SoundAndVideoOptions::AntialiasingArgs::Defaults() {
	pEnabled = Enabled = false;
}

void rsAntialiasingMode(bool Enable) {
	if(Enable == IRS->AntialiasingIsEnabled()) return;

	if(Enable) IRS->AntialiasingEnable();
	else IRS->AntialiasingDisable();

	FlipPages();
	IRS->ClearDevice();
}

bool SoundAndVideoOptions::AntialiasingArgs::Update() {
	if(Enabled != pEnabled) {
		pEnabled = Enabled;
		rsAntialiasingMode(Enabled);
		return true;
	}
	return false;
}

bool SoundAndVideoOptions::AntialiasingArgs::GetClassPresentationMask(_str &Mask, void *, void *, const char *, DWORD) {
	Update();

	Mask = "Antialiasing (";
	if(Enabled) {
		Mask += "Enabled";
	} else {
		Mask += "Disabled";
	}
	Mask += ")";
	return true;
}

bool SoundAndVideoOptions::AntialiasingArgs::OnChangeMember(BaseClass *, void *, void *, const char *) {
	return Update();
}

void SoundAndVideoOptions::AntialiasingArgs::EvaluateFunction() {
	Enabled = !Enabled;
	Update();
}

//-----------------------------------------------------------------------------
// TrueColorArgs
//-----------------------------------------------------------------------------
void SoundAndVideoOptions::TrueColorArgs::Defaults() {
	pEnabled = Enabled = true;
	ShowDeviceProperties = false;
}

void rsTrueColorMode(bool Enable) {
	if(Enable == IRS->TrueColorIsEnabled()) return;

	if(Enable) IRS->TrueColorEnable();
	else IRS->TrueColorDisable();

	int W, H, Bpp, RR;
	IRS->GetDeviceDisplayMode(&W, &H, &Bpp, &RR);
	
	ScreenProp P;
	P.m_Width = W;
	P.m_Height = H;
	P.m_ColorFormat = cfRGB565;
	P.m_bFullScreen = false;
	P.m_bCoverDesktop = false;
	P.m_RefreshRate = RR;

	IRS->SetScreenProp(P);

	FlipPages();
	IRS->ClearDevice();
}

bool SoundAndVideoOptions::TrueColorArgs::Update() {
	if(Enabled != pEnabled) {
		pEnabled = Enabled;
		rsTrueColorMode(Enabled);
		return true;
	}
	return false;
}

bool SoundAndVideoOptions::TrueColorArgs::GetClassPresentationMask(_str &Mask, void *, void *, const char *, DWORD) {
	Update();

	Mask = "TrueColor (";
	if(Enabled) {
		Mask += "Enabled";
	} else {
		Mask += "Disabled";
	}
	Mask += ")";
	return true;
}

bool SoundAndVideoOptions::TrueColorArgs::OnChangeMember(BaseClass *, void *, void *, const char *) {
	return Update();
}

void SoundAndVideoOptions::TrueColorArgs::EvaluateFunction() {
	Enabled = !Enabled;
	Update();
}

//-----------------------------------------------------------------------------
// SafeRefreshRateArgs
//-----------------------------------------------------------------------------
/*void SoundAndVideoOptions::SafeRefreshRateArgs::Defaults() {
	pEnabled = Enabled = false;
}

void rsSafeRefreshRateMode(bool Enable) {
	if(Enable == IRS->RefreshRateOverrideIsEnabled()) return;

	extern int g_nSafeRefreshRate;
	if(Enable) IRS->RefreshRateOverrideEnable(g_nSafeRefreshRate);
	else IRS->RefreshRateOverrideDisable();

	int W, H, Bpp, RR;
	IRS->GetDeviceDisplayMode(&W, &H, &Bpp, &RR);
	
	ScreenProp P;
	P.m_Width = W;
	P.m_Height = H;
	P.m_ColorFormat = cfRGB565;
	P.m_bFullScreen = false;
	P.m_bCoverDesktop = false;
	P.m_RefreshRate = RR;

	IRS->SetScreenProp(P);

	FlipPages();
	IRS->ClearDevice();
}

bool SoundAndVideoOptions::SafeRefreshRateArgs::Update() {
	if(Enabled != pEnabled) {
		pEnabled = Enabled;
		rsSafeRefreshRateMode(Enabled);
		return true;
	}
	return false;
}

bool SoundAndVideoOptions::SafeRefreshRateArgs::GetClassPresentationMask(_str &Mask, void *, void *, const char *, DWORD) {
	Update();

	Mask = "Safe Refresh Rate (";
	if(Enabled) {
		Mask += "Enabled";
	} else {
		Mask += "Disabled";
	}
	Mask += ")";
	return true;
}

bool SoundAndVideoOptions::SafeRefreshRateArgs::OnChangeMember(BaseClass *, void *, void *, const char *) {
	return Update();
}

void SoundAndVideoOptions::SafeRefreshRateArgs::EvaluateFunction() {
	Enabled = !Enabled;
	Update();
}*/
//---------------------------------------------------------------------
/*#ifndef __LANGUAGE_SETTINGS_H__
#define __LANGUAGE_SETTINGS_H__
#include "..\ClassEngine\ClassEngine.h"
#include "..\ClassEngine\more_types.h"
#pragma pack( push )
#pragma pack( 1 )
#include "EngineSettings.h"
class LangClass:public BaseClass{
public:
	void SaveToFile(char* fName);
	bool LoadFromFile(char* fname);
	SAVE(LangClass)		
	ENDSAVE
};
class LanguageSettings:public LangClass{
public:
	virtual void reset_class(void* DataPtr);
	int LangIndex;

	//-----------registration section--------------
	SAVE(LanguageSettings){
		REG_MEMBER(_int,LangIndex);
	}ENDSAVE;
	void Save(){
        SaveToFile("Text\\LanguageSettings.xml");
	}
	void Update() {
	}
	void Load(){
		LoadFromFile("Text\\LanguageSettings.xml");
	}
	void Init();
	LanguageSettings(){		
		Init();
	}
};
//
#ifdef IMPLEMENT_CLASS_FACTORY
LanguageSettings LangSettings;
//
void LangClass::SaveToFile(char* fName){
	xmlQuote xml("LanguageSettings");
	BaseClass::Save(xml,this);
	xml.WriteToFile(fName);
}
bool LangClass::LoadFromFile(char* fname){
	xmlQuote xml;
	if(xml.ReadFromFile(fname)){
		ErrorPager EP;
		BaseClass::Load(xml,this,&EP);
		return true;
	}else return false;
}
#else //IMPLEMENT_CLASS_FACTORY
extern LanguageSettings LangSettings;
#endif //IMPLEMENT_CLASS_FACTORY
#pragma pack( pop )
#endif //__GAME_SETTINGS_H__
#include "Language.h"
#include "stdheader.h"
#include "BE_HEADERS.h"
#include "Pinger.h"
#include "CurrentMapOptions.h"
#include "Camera\Navigator.h"
void LanguageSettings::Init(){
	LangIndex=0;
}
void LanguageSettings::reset_class(void* DataPtr){
	LangClass::reset_class(DataPtr);
	((LanguageSettings*)DataPtr)->Init();
}*/