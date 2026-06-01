#include "stdheader.h"
#include "WeaponSystem.h"
#include "CurrentMapOptions.h"
#include "SuperEditor.h"
#include "Surface\Surface.h"
//#include "CUniversalMapClipboard.h"
#include "ITerrain.h"
#include ".\cvi_MainMenu.h"
#include "ExMapGenerator.h"
#include "IWater.h"
#include "UnitsInterface.h"
#include "TreesMesh.h"

void ACT(int x);

//
//extern UniversalMap g_UniversalMap;
//extern CUniversalMapClipboard g_UniversalMapClipboard;

#define AI_CHEATS
int BTLID=1;	// Start Battle ID
#define CHECKNSL for(int i=0;i<7;i++)addrand(NSL[i]+1);
int PREVNSL=-1;
int PREVNSL1=-1;
#define CHNSL assert((NSL[1]==PREVNSL1&&NSL[0]==PREVNSL)||PREVNSL==-1);
void TestCGARB();
#define DLLIMPORT extern "C" __declspec(dllimport)

//#define FRANCEVERSION
//#define SINGLETESTNETWORK
//#define TESTVERSION
//#define TUTORIAL_DEMOVERSION

//#define AC_BATTLE_ONLY

//#define INTF_AC

#define INTF_AC_ADD

extern bool RUNMAPEDITOR;
extern bool RUNUSERMISSION;
extern char USERMISSPATH[128];
extern bool vFastUpg;

extern int CurrentSinBattle;

byte RedColor2=0xBB;
extern int CurrentAnswer;
bool CommEnterName();
bool NotifyFirewallState();
void UnPress();
int CurrentNation=-1;
extern int PitchTicks;
extern int MaxPingTime;
void StartPing(DWORD DPID,int ID);
void EndPing(int ID);
bool CheckLogin();
char* GetLString(DWORD DPID);
#define MinPing0 1
void ShowLoading();
CEXPORT int CurProtocol=0;
extern char CHATSTRING[256];
extern DWORD CHATDPID;
void SendChat(char* str,bool);
extern int ModeLX[32];
extern int ModeLY[32];
extern int NModes;
extern int RM_LandType;
extern int RM_Resstart;
extern int RM_Restot;
void CenterScreen();
bool Camp_EnterName();
bool ProcessBigMap(int n);							//Vitya
//bool PlayerInterface();								//grey
bool ProcessSingleCampagin(int n);
void CreateNationalMaskForMap(char* Name);
extern bool realLpressed;
void CreateMiniMap();
void CreateGamesList(ListBox* LB);
char* GetTextByID(char* ID);
void CreateRandomMap(char* Name);
void RandomMapDialog(char* Result);
void RandomMapDialog1(char* Result,int N,char* Nats);
bool IgnoreSlow=false;
void PrepareGameMedia(byte myid,bool);
void PrepareGameMedia(byte myid,bool SaveNR,bool DisableDLL);
extern int exRealLx;
extern int exRealLy;
bool CHKV(char* Name);
bool WaitingHostGame(int);
void NewMap(int szX,int szY);
bool WaitingJoinGame(int);
void SlowLoadPalette(LPCSTR lpFileName);
void SlowUnLoadPalette(LPCSTR lpFileName);
void SetDarkPalette();
void CopyToScreen(int x,int y,int Lx,int Ly);
void ChooseInterior(int Type);
void Save3DMap(char* Map);
void Load3DMap(char* Map);
void EditMiniMap();
extern short WheelDelta;
SFLB_DLLEXPORT LoadMEDButtons();
int time1,time2,time3,time4,time5;
extern int time8;
SFLB_DLLEXPORT processMLoadGame();
extern char GameName[128];
bool ContinueGame;
extern word rpos;
//char GSets.CGame.cgi_CurrentMap[64];
void CBar(int x,int y,int Lx,int Ly,byte c);
void PrepareEditMedia(byte myid);
extern bool EditMapMode;
extern int RES[8][8];
extern byte mapPos[16];
void PreLoadExtendedMap(char* s);
void PostLoadExtendedMap(char* s);
void InitGame();
int nsmaplx;
int nsmaply;
int GameMode;
extern int LastKey;
extern bool KeyPressed;
void MFix();
//SQPicture MainMenuGround;
CEXPORT int WarSound;
CEXPORT int WorkSound;
CEXPORT int OrderSound;
CEXPORT int MidiSound;
/*
SQPicture w164x28d;
SQPicture r164x28e;
SQPicture r164x28p;
SQPicture w224x28d;
SQPicture r224x28e;

SQPicture sb_up_e;
SQPicture sb_up_p;
SQPicture sb_up_d;

SQPicture sb_dn_e;
SQPicture sb_dn_p;
SQPicture sb_dn_d;

SQPicture sb_lf_e;
SQPicture sb_lf_p;
SQPicture sb_lf_d;

SQPicture sb_ri_e;
SQPicture sb_ri_p;
SQPicture sb_ri_d;

SQPicture sb_vbar;
SQPicture sb_vbare;
SQPicture sb_hbar;
SQPicture sb_hbare;

SQPicture sb_btn;

SQPicture chk1_on;
SQPicture chk1_off;
SQPicture chk2_on;
SQPicture chk2_off;
*/
//SQPicture w160x160;
//SQPicture w160x256;
//SQPicture y288x256;
//SQPicture y288x128;
//SQPicture w288x256;
//SQPicture w288x128;

byte RPGRTSNat;
int RPGRTSLand;
int RPGRTSMoney;
int RPGRTSResOnMap;
int RPGRTSDifficulty;

void EraseObject(OneObject* OB);

SQPicture LoPanel;
RLCTable TPassive;
RLCTable TActive;
RLCTable TDisable;
RLCFont FPassive;
RLCFont FActive;
RLCFont FDisable;
extern bool CheapMode;
int PanelLx;
int PanelLy;
bool MultiplayerStart;
bool GameNeedToDraw;
extern bool ChoosePosition;
extern word NPlayers;
void CloseMPL();
void IBEnumeratePlayers(InputBox** IB);
void LBEnumerateSessions(ListBox* LB,int);
void AnalyseMessages();
bool CreateNamedSession(char* Name,DWORD User2,int Max);
bool InGame;
bool InEdit;
bool JoinNameToSession(int ns,char* Name);
void FreeDDObjects( void );
bool StartIGame(bool);
extern bool GameInProgress;
byte MyRace;
extern LPDIRECTPLAY3A		lpDirectPlay3A;
extern word PlayerMenuMode;
extern LPVOID lplpConnectionBuffer[16];
extern bool SHOWSLIDE;
void PlayVideo();
void ProcessNetCash();
void ProcessReceive();
int NPROCM=0;
int TPROCM=0;
int LastCTRLPressTime=0;
void ProcessLogin();
int PrevProgStage=0;
char* PROGSTR=NULL;
extern int CurPalette;
extern int RealLx;
extern int RealLy;
void ShowCharUNICODE(int x,int y,byte* strptr,lpRLCFont lpr);
void ShowProgStr(char* ss,int prog){
	
	if(CurPalette){
		LocalGP LOAD("Interf2\\_Loading");
		int x=(RealLx-GPS.GetGPWidth(LOAD.GPID,0))/2;
		int y=(RealLy-GPS.GetGPHeight(LOAD.GPID,0))/2;
		GPS.ShowGP(x,y,LOAD.GPID,1,0);		
	}else{
		DrawStdBar((RealLx-330)/2+20,(RealLy-70)/2+18,(RealLx-330)/2+318,(RealLy-70)/2+55);
	};
	
	int L=strlen(ss);
	L=(L*prog)/100;
	int x0=(RealLx-GetRLCStrWidth(ss,&fon30y1))/2+5;
	int y0=(RealLy-GetRLCHeight(fon30y1.RLC,'W'))/2-7;
	int spos=0;
	do{
		if(spos<=L)ShowCharUNICODE(x0,y0,(byte*)(ss+spos),&fon30y1);
		else ShowCharUNICODE(x0,y0,(byte*)(ss+spos),&fon30y3);
		int LL;
		x0+=GetRLCWidthUNICODE(fon30y3.RLC,(byte*)(ss+spos),&LL);
		spos+=LL;
	}while(ss[spos]);
	CopyToScreen(0,0,RealLx,RSCRSizeY);
};
void ProcessNewInternet();
int PrevT=0;
int COUNTER1=0;
int LastBActiveTime=0;
void IAmLeft();
extern byte SpecCmd;
void ShowOptMessage(char* ID,int v){
	char* str=GetTextByID(ID);
	if(str[0]!='#'){
		char HHH[512];
		sprintf(HHH,str,v);
		AssignHint1(HHH,200,32);
	};
};
bool NeedToPerformGSC_Report=0;
bool NeedToReportInGameStats=0;
int PrevReportTime=0;
int LastTimeReport_tmtmt=0;
extern int tmtmt;
extern City CITY[8];
void EndGSC_Reporting(){
	NeedToReportInGameStats=0;
	NeedToPerformGSC_Report=0;
	LeaveGSCRoom();
};
void ProcessExplorer(int Index);
bool PDIF_Inside=0;
extern int TIME1;
int PrevProcTime=0;
int PrevDelayTime=0;
int PrevInGameAliveTime=0;
extern DWORD MyDPID;
void ProcessMP3(bool anyway);
CIMPORT
void GetGameID(char* s);
void ExplorerOpenRef(int Index,char* ref);
bool ServerResponce=1;
void ReportProcessMessages(int T);
bool IgnoreProcessMessages = false;
bool IgnoreProcessMessages2 = false;
void InternalProcess(){
	int TT=GetTickCount();

    //  Commented by Silver 9.07.2004 - popped in profiling
    //ReportProcessMessages(TT);
	
    if(!PrevDelayTime)PrevDelayTime=TT;
	if(TT-PrevDelayTime>560){
		Sleep(2);
		PrevDelayTime=TT;
		ProcessMP3(false);
	};
	if(!PrevProcTime)PrevProcTime=TT;
	if(TT-PrevProcTime>3000){
		extern bool InternetGame;
		PDIF_Inside=1;
		//if(InternetGame){
			if(ProcessDownloadInternetFiles)ProcessDownloadInternetFiles();
			ProcessExplorer(16);
		//}
		PDIF_Inside=0;
		PrevProcTime=TT;
	};
	if(NeedToPerformGSC_Report){
		int T=TT;
		if(!PrevReportTime)PrevReportTime=T;
		if(T-PrevReportTime>60000){
			int NZeros=0;
			int PROFS[8];
			for(int i=0;i<NPlayers;i++){
				if(!GSets.CGame.PL_INFO[i].ProfileID)NZeros++;
				PROFS[i]=GSets.CGame.PL_INFO[i].ProfileID;
			};
			if(!NZeros){
				ReportAliveState(NPlayers,PROFS);
				PrevReportTime=T;
			};
		};
	};
	if(NeedToReportInGameStats&&UseGSC_Login){
		int T=TT;
		if(!PrevInGameAliveTime)PrevInGameAliveTime=T;
		if(T-PrevInGameAliveTime>20000){
			if(!ServerResponce){
				//AssignHint1(GetTextByID("SERVERWARNING"),200,32);
			};
			char GMID[64];
			GetGameID(GMID);
			if(GMID[0]){
				char REF[128];
				sprintf(REF,"GW|gmalive&%s&%X&1",GMID,&ServerResponce);
				ExplorerOpenRef(0,REF);
				PrevInGameAliveTime=T;
				ServerResponce=0;
			};
		};
	};
	int T1=TT;
	if((!PrevT)||T1-PrevT>15){
		GSets.CGame.ProcessRoom();
		int TT0=T1;
		if(rand()<1024)
			Sleep(5);
		ProcessNewInternet();
		TT0=TT-T1;
		if(TT0>COUNTER)COUNTER=TT0;
		ProcessLogin();
		TPEN.Process();
		//NPROCM++;
		//int T0=TT;
		if(GetKeyState(VK_CONTROL)&0x8000){
			LastCTRLPressTime=TT;
		};
		TT0=TT;
		ProcessReceive();
		TT0=TT-TT0;
		if(TT0>COUNTER1)COUNTER1=TT0;
		ProcessNetCash();
		PrevT=T1;
		if(bActive){
			LastBActiveTime=T1;
		}else{
			/*
			if(GameInProgress&&NPlayers>1&&T1-LastBActiveTime>30000){
				if(NATIONS[MyNation].VictState!=2){
					IAmLeft();
					SpecCmd=199;
					ShowOptMessage("#ALTTABDEFEAT",0);
				}
			}
			*/
		}
	}
}

extern bool EnterChatMode;
void ProcessChatKeys();
void ProcessVotingKeys();
bool ProcessMessagesEx(){
	if(GetKeyState(VK_CONTROL)&0x8000){
		LastCTRLPressTime=GetTickCount();
	};
	ProcessVotingKeys();
	if(EnterChatMode){
		ProcessChatKeys();
	};
	MSG         msg;
	//ProcessNetCash();
	bool MSR=0;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
		if (msg.message == WM_QUIT)
		{
			//FreeDDObjects();
			//PostQuitMessage(msg.wParam);
			return true;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		MSR=1;
	}
	return MSR;
};
void normstr(char* str);
CEXPORT char PlName[64];
void ProcessHelp();
SFLB_DLLEXPORT SFLB_ReadFirstPlayerName(char* ccc);
#ifdef MAKE_PTC
SFLB_DLLEXPORT SFLB_LoadMEDButtons();
SFLB_DLLEXPORT SFLB_InitDialogs(){
	DLL1(SFLB_ReadFirstPlayerName,PlName);
	unsigned long dws=20;
	if(!PlName[0]){
		GetComputerName(PlName,&dws);
		normstr(PlName);
	};
    FPassive=YellowFont;
    FActive=WhiteFont;
    FDisable=YellowFont;
	DLL0(SFLB_LoadMEDButtons);
};
#endif
CEXPORT int ItemChoose;
CEXPORT
bool MMItemChoose(SimpleDialog* SD){
	ItemChoose=SD->UserParam;
	Lpressed=false;
	return true;
};
bool CHANGESORT(SimpleDialog* SD){
	if(TPEN.SORTTYPE==SD->UserParam)TPEN.SORTTYPE=-TPEN.SORTTYPE;
	else TPEN.SORTTYPE=SD->UserParam;
	Lpressed=0;
	return true;
};
CEXPORT
void StdKeys(){
	if(KeyPressed&&(LastKey==27||LastKey==13)){
		if(LastKey==13)ItemChoose=mcmOk;
		if(LastKey==27)ItemChoose=mcmCancel;
		KeyPressed=0;
		LastKey=0;
	};
};
extern bool LB_SORT_ITEM;
void InstallMap(ListBox* LB,char* Name){
	LB_SORT_ITEM=1;
	LB->AddItem(Name,0);
	LB_SORT_ITEM=0;
};
void CreateMapsList(ListBox* LB){
	TGSCFindInfo* FD=GSFILES.gFindFirst("*.m3d");
	if(FD){
		InstallMap(LB,FD->m_FileName);
		while(GSFILES.gFindNext(FD))InstallMap(LB,FD->m_FileName);
		//delete(FD);
	};
};
void InstallNewMap(ListBox* LB,WIN32_FIND_DATA* FD){
	LB->AddItem(FD->cFileName,0);
};
void CreateNewMapsList(ListBox* LB){
	TGSCFindInfo* FD=GSFILES.gFindFirst("*.m3d");
	if(FD){
		InstallMap(LB,FD->m_FileName);
		while(GSFILES.gFindNext(FD))InstallMap(LB,FD->m_FileName);
		//delete(FD);
	};
};
//----------------Graphics menu----------------//
bool InitScreen();
int OldSizeX;
int OldSizeY;
int mul3(int);
extern LPDIRECTDRAW            lpDD;
int GetRealTime();
bool CheckMode(){
	return true;
	SQPicture Pan("pan17.bpx");
	RLCFont gf24w(GETS("@SMFONT1"));
	RLCFont gf24y(GETS("@SMFONT0"));
	int mmlx=Pan.GetLx();
	int mmly=Pan.GetLy();
	ContinueGame=false;
	DialogsSystem GMM(smapx+(nsmaplx<<4)-(mmlx>>1),smapy+(mul3(nsmaply)<<2)-(mmly>>1));
	Picture* PIC1=GMM.addPicture(NULL,0,0,&Pan,&Pan,&Pan);
	//TextButton* Mess=GMM.addTextButton(PIC1,288>>1,30,"???????? ?",&FActive,&FPassive,&FDisable,1);
	TextButton* OkBtn=GMM.addTextButton(PIC1,GETV("VMQOKX"),GETV("VMQOKY"),GETS("@VMQOKSTR"),&gf24w,&gf24y,&gf24y,1);
	TextButton* CancelBtn=GMM.addTextButton(PIC1,GETV("VMQNOX"),GETV("VMQNOY"),GETS("@VMQNOSTR"),&gf24w,&gf24y,&gf24y,1);
	OkBtn->UserParam=mcmOk;
	OkBtn->OnUserClick=&MMItemChoose;
	CancelBtn->UserParam=mcmCancel;
	CancelBtn->OnUserClick=&MMItemChoose;
	int time=GetRealTime();
	ItemChoose=-1;
	do{
		ProcessMessages();
		GMM.ProcessDialogs();
		GMM.RefreshView();
	}while(ItemChoose==-1&&GetRealTime()-time<8000);
	if(ItemChoose==mcmOk)return true;
	else return false;

};
void GSSetup800();
void DrawAllScreen();
extern int COPYSizeX;
extern int RealLx;
extern int RealLy;
void SERROR(){
	PlayEffect(0,0,0);
	int time=GetRealTime();
	do{}while(GetRealTime()-time<1000);
};
void SERROR1(){
	PlayEffect(1,0,0);
	int time=GetRealTime();
	do{}while(GetRealTime()-time<1000);
};
void SERROR2(){
	PlayEffect(2,0,0);
	int time=GetRealTime();
	do{}while(GetRealTime()-time<1000);
};
extern int SCRSZY;
void ClearScreen(){
	if(RealScreenPtr)memset(RealScreenPtr,0,RSCRSizeX*SCRSZY);
};
extern byte fog[8192+1024];
#ifdef MAKE_PTC
CEXPORT
void DarkScreen(){
#ifdef _USE3D
	return;
#endif // _USE3D
	
	byte* sptr=(byte*)ScreenPtr;
	int sz=ScrWidth*RealLy;
	//for(int i=0;i<sz;i++)sptr[i]=fog[4096+1024+sptr[i]];
	int ofs=0;
	int ry=RealLy>>1;
	for(int iy=0;iy<ry;iy++){
		ofs=ScrWidth*iy*2;
		for(int ix=0;ix<RealLx;ix+=2){
			sptr[ofs]=0;
			ofs+=2;
		};
		ofs=ScrWidth*(iy*2+1)+1;
		for(int ix=0;ix<RealLx;ix+=2){
			sptr[ofs]=0;
			ofs+=2;
		};
	};
};
#endif
extern int CurPalette;
#ifdef MAKE_PTC
void ShowLoading(){
	return;
	DarkScreen();
	if(CurPalette){
		LocalGP LOAD("Interf2\\_Loading");
		int x=(RealLx-GPS.GetGPWidth(LOAD.GPID,0))/2;
		int y=(RealLy-GPS.GetGPHeight(LOAD.GPID,0))/2;
		GPS.ShowGP(x,y,LOAD.GPID,0,0);
	}else{
		DrawStdBar((RealLx-330)/2+20,(RealLy-70)/2+18,(RealLx-330)/2+318,(RealLy-70)/2+55);
	};
	char* ss=GetTextByID("LOADING");
	ShowString((RealLx-GetRLCStrWidth(ss,&fon30y3))/2+5,(RealLy-GetRLCHeight(fon30y3.RLC,'W'))/2-7,ss,&fon30y3);
	CopyToScreen(0,0,RealLx,RSCRSizeY);
};
#endif

void SetGameDisplayModeAnyway(int SizeX,int SizeY)
{
	ClearScreen();
	RealLx=SizeX;
	RealLy=SizeY;	
	RSCRSizeX = SizeX;
	RSCRSizeY = SizeY;
	SCRSizeX  = SizeX;
	SCRSizeY  = SizeY;
	COPYSizeX = SizeX;

	ScreenProp prop;
	prop.m_Width		 = SizeX;
	prop.m_Height		 = SizeY;

//	prop.m_bTrueColor = GSets.SVOpt.TrueColor.Enabled;
	prop.m_ColorFormat = cfRGB565;

	if (DDDebug)
	{
		prop.m_bFullScreen  = false; 
	}
	else
	{
		prop.m_bFullScreen  = true; 
	}
	prop.m_bCoverDesktop = false; 
	extern IRenderSystem* IRS;
	IRS->SetScreenProp( prop );

	SizeX=GetSystemMetrics(SM_CXSCREEN);
	SizeY=GetSystemMetrics(SM_CYSCREEN);
	RealLx=SizeX;
	RealLy=SizeY;	
	RSCRSizeX = SizeX;
	RSCRSizeY = SizeY;
	SCRSizeX  = SizeX;
	SCRSizeY  = SizeY;
	COPYSizeX = SizeX;

	GPS.SetClipArea( 0, 0, RealLx, RealLy );
    IRS->SetViewPort( Rct( 0, 0, RealLx, RealLy ) );
	InitScreen();
	GSSetup800();
	SetCursorPos(512,300);
	SetMPtr(512,300,0);
	void MakeAllDirtyGBUF();
	MakeAllDirtyGBUF();
	void CreateMiniMap();
	CreateMiniMap();

	// To avoid stretched trees after display mode change:
	TreesMesh.ClearPrevHash();
	TreesMesh.CreateMeshes();

	FlipPages();
};

/*void ReSetDisplayColorMode() {
	int Width, Height;
	bool TrueColor;
	IRS->GetCurrentDisplayMode(&Width, &Height, &TrueColor);
	if(TrueColor != GSets.SVOpt.TrueColor.Enabled) {
		SetGameDisplayModeAnyway(Width, Height);
	}
}*/

bool SetGameDisplayMode(int SizeX,int SizeY){
	ClearScreen();
	RealLx=SizeX;
	RealLy=SizeY;
	GSets.SVOpt.ScreenSizeX=RealLx;
	GSets.SVOpt.ScreenSizeY=RealLy;
	//FreeDDObjects();
	InitScreen();
	GSSetup800();
	DrawAllScreen();
	SetCursorPos(512,300);
	SetMPtr(512,300,0);
	if(CheckMode())return true;
	else{
		RealLx=OldSizeX;
		RealLy=OldSizeY;
		//FreeDDObjects();
		InitScreen();
		GSSetup800();
		DrawAllScreen();
		return false;
	};
	return false;
};
extern int ScrollSpeed;
CEXPORT
int FPSTime;

//-----------------Speed menu------------------//
void ProcessSpeed(int x,int y){
	
};
//----------------SOUND----------------

void ProcessSound(int x,int y){
	
};
//-----------------Options menu----------------//
void ProcessOptions(int x,int y){
	
};
//-----------------SINGLE PLAYER---------------//
void UnLoading();
static byte Ntn[8];
bool NatChoose(SimpleDialog* SD){
	BpxTextButton* TB=(BpxTextButton*)SD;
	char* qq=TB->Message;
	if(qq[0]=='?')strcpy(TB->Message,"????");
	else strcpy(TB->Message,"????");
	SD->NeedToDraw=true;
	Lpressed=true;
	return true;
};	
int coorX[8];
int coorY[8];
void setCoor(){
	for(int i=0;i<8;i++){
		coorX[i]=-1;
		coorY[i]=-1;
	};
};
void LoadAIFromDLL(byte Nat,char* Name);
void LoadNation(char* fn,byte msk,byte NIndex,byte);
extern City CITY[8];
void ProcessMissions(){
	
};
void OldprocessSinglePlayer(){
	
};
//-----------------WAITING FOR OPPONENTS(HOST)-------------------//
bool processHostConnection(){
	return 0;
};
//-----------------WAITING FOR OPPONENTS(JOIN)-------------------//
bool processJoinConnection(){
	return 0;
};
//--------------------JOIN  TO SESSION---------------------//
bool processJoinGame(){
	return false;
};
extern char ROOMNAMETOCONNECT[128];
bool JOINTOINETGAME(char* SNAME){

	return false;
};
bool ProcessOneBattle(int BtlID);

//--------------------HOST THE SESSION---------------------//
bool processHostGame(){
	return false;
};
bool MPL_EnterName();
extern CEXPORT char IPADDR[128];
void WaitWithMessage(char* Message);
void NoWaitWithMessage(char* Message);
#ifdef MAKE_PTC
int ProcessInternetConnection(bool Active);
bool FindSessionAndJoin(char* Name,char* Nick,bool Style);
bool CreateSession(char* SessName,char* Name,DWORD User2,bool Style,int MaxPlayers);
extern int GMTYPE;
int IEMMOD=0;
bool DoNewInet=0;
bool InternetGameLogin();
extern int GMMAXPL;
extern CEXPORT bool TOTALEXIT;
bool ProcessNewInternetLogin();
extern char** NatsIDS;

#endif
bool ProcessOneBattle(int BtlID);
int ProcessWars();

//--------------------MULTIPLAYER GAME---------------------//
extern char SaveFileName[128];
void OldprocessMultiplayer(){
	
};
//---------------------Map options-------------------------//
//int MO_LandType=0;
//int MO_StartRes=0;
//int MO_ResOnMap=0;
char MapScenaryDLL[200]="";
void ProcessMapOptions(){
	LocalGP FONT("interf3\\Fonts\\FontC12");
	RLCFont fntb(FONT.GPID);
	fntb.SetBlackColor();
	RLCFont fntr(FONT.GPID);
	fntr.SetRedColor();
	int x0=(RealLx-64*7)>>1;
	int y0=(RealLy-64*2)>>1;
	DialogsSystem GMM(x0,y0);
	char DLLSC[200];
	strcpy(DLLSC,MapScenaryDLL);
	GMM.addTextButton(NULL,32*7,-20,GetTextByID("SCDLL"),&BigYellowFont,&BigYellowFont,&BigYellowFont,1);
	TextButton* Ok=GMM.addTextButton(NULL,32*7-100,80,"Ok",&fntr,&fntb,&fntb,1);
	Ok->UserParam=mcmOk;
	Ok->OnUserClick=&MMItemChoose;
	TextButton* Cancel=GMM.addTextButton(NULL,32*7+100,80,"Cancel",&fntr,&fntb,&fntb,1);
	Cancel->UserParam=mcmCancel;
	Cancel->OnUserClick=&MMItemChoose;
	int ibx=21;
	int iby=44;
	int iblx=400;
	int ibly=20;
	InputBox* EB=GMM.addInputBox(NULL,ibx,iby,DLLSC,100,iblx,ibly,&BlackFont,&RedFont);
	EB->Active=1;
	ItemChoose=-1;
	do{
		DrawPaperPanel(x0,y0,x0+64*7,y0+64*2);
		Xbar(x0+ibx,y0+iby,iblx+10,ibly+4,0x3C);
		GMM.MarkToDraw();
		ProcessMessages();
		GMM.ProcessDialogs();
		GMM.RefreshView();
	}while(ItemChoose==-1);
	if(ItemChoose==mcmOk){
		strcpy(MapScenaryDLL,DLLSC);
	};
};
char DEFPLNAMES[8][64];
//--------------------RESOURCE EDITOR----------------------//
void MemScreenPart(int x,int y,int lx,int ly,byte* Ptr);
extern int CBB_GPFILE;
int ProcessResEdit(){
	void EditMapOptions();
	void EditScripts();
	if(GetKeyState(VK_CONTROL)&0x8000)EditScripts();
	else EditMapOptions();	
	for(int i=0;i<=MaxNatColors;i++){
		RES[i][0]=MOptions.Players.Player[i].StartRes.Wood;
		RES[i][1]=MOptions.Players.Player[i].StartRes.Gold;
		RES[i][2]=MOptions.Players.Player[i].StartRes.Stone;
		RES[i][3]=MOptions.Players.Player[i].StartRes.Food;
		RES[i][4]=MOptions.Players.Player[i].StartRes.Iron;
		RES[i][5]=MOptions.Players.Player[i].StartRes.Coal;
	}
	return 0;

	LocalGP BTNS("Interf2\\AI_Res");

	int x0=(RealLx-532)>>1;
	int y0=(RealLy-540)>>1;
	DialogsSystem DSS(x0,y0);

	byte* Back=(byte*)malloc(RealLx*RealLy);
	MemScreenPart(0,0,RealLx,RealLy,Back);	
	
	DSS.addBPXView(NULL,-x0,-y0,RealLx,RealLy,1,1,1,Back,NULL);
	
	DSS.addGPPicture(NULL,0,0,BTNS.GPID,0);
	DSS.addGPPicture(NULL,0,0,BTNS.GPID,1);
	
	// Fonts
	RLCFont* FWin = &fon16y5;
	RLCFont* FABut = &fon18y5;
	RLCFont* FPBut = &fon18y5;
	RLCFont* FSYellow = &fon16y3;
	RLCFont* FYellow = &fon18y3;

	// Text
	DSS.addTextButton(NULL,294,34,GetTextByID("INTF_EDIT_RESOURCE_AI"),FWin,FWin,FWin,1);

	DSS.addTextButton(NULL,50,80,GetTextByID("INTF_EDIT_PLAYER"),FYellow,FYellow,FYellow,0);
	DSS.addTextButton(NULL,298,80,GetTextByID("INTF_EDIT_NAME"),FYellow,FYellow,FYellow,0);

	DSS.addTextButton(NULL,50,143,GetTextByID("INTF_EDIT_WOOD"),FSYellow,FSYellow,FSYellow,0);
	DSS.addTextButton(NULL,50,143+1*26,GetTextByID("INTF_EDIT_FOOD"),FSYellow,FSYellow,FSYellow,0);
	DSS.addTextButton(NULL,50,143+2*26,GetTextByID("INTF_EDIT_STONE"),FSYellow,FSYellow,FSYellow,0);
	DSS.addTextButton(NULL,50,143+3*26,GetTextByID("INTF_EDIT_GOLD"),FSYellow,FSYellow,FSYellow,0);
	DSS.addTextButton(NULL,50,143+4*26,GetTextByID("INTF_EDIT_IRON"),FSYellow,FSYellow,FSYellow,0);
	DSS.addTextButton(NULL,50,143+5*26,GetTextByID("INTF_EDIT_COAL"),FSYellow,FSYellow,FSYellow,0);

	DSS.addTextButton(NULL,268,147,GetTextByID("INTF_EDIT_LANDTYPE"),FSYellow,FSYellow,FSYellow,0);
	DSS.addTextButton(NULL,268,147+52,GetTextByID("INTF_EDIT_STARTRES"),FSYellow,FSYellow,FSYellow,0);
	DSS.addTextButton(NULL,268,147+2*52,GetTextByID("INTF_EDIT_MAPRES"),FSYellow,FSYellow,FSYellow,0);

	DSS.addTextButton(NULL,50,333,GetTextByID("INTF_EDIT_SCENDLL"),FYellow,FYellow,FYellow,0);
	
	
	ComboBox* PLAYER=DSS.addGP_ComboBoxDLX(NULL,126,79,157,CBB_GPFILE,0,9,9,&WhiteFont,&YellowFont,NULL);
	PLAYER->Center-=5;
	PLAYER->FontDy-=2;
	PLAYER->OneDy-=1;

	char* cc=GetTextByID(GetTextByID("$PLAYER%D"));
	for(int i=0;i<8;i++){
		char ccx[128];
		sprintf(ccx,cc,i+1);
		PLAYER->AddLine(ccx);
	};
	PLAYER->CurLine=MyNation;
	PLAYER->AddLine(GetTextByID("$PLAYERALL"));

	char ResST[22][16];
	char PLNAMES[9][64];
	for(int i=0;i<8;i++)strcpy(PLNAMES[i],DEFPLNAMES[i]);
	PLNAMES[8][0]=0;
	byte RESRID[8];
	int RESLOC[8];
	int NRes=0;
	for(int i=0;i<8;i++){
		if(RDS[i].Enabled){
			int resid=i;
			if(i==1) resid=3;
				else if(i==3) resid=1;
			RESRID[NRes]=resid;
			itoa(RES[MyNation][resid],ResST[i],10);
			RESLOC[resid]=RES[MyNation][resid];
			NRes++;
		};
	};

	for(int i=0;i<NRes;i++){
		DSS.addInputBox(NULL,128,139+26*i,ResST[i],12,105,24,&YellowFont,&WhiteFont);
	};
	InputBox* IBN=DSS.addInputBox(NULL,336,335,PLNAMES[MyNation],32,270,24,&YellowFont,&WhiteFont);

	char DLLPT[200];
	strcpy(DLLPT,MapScenaryDLL);
	DSS.addInputBox(NULL,235,331,DLLPT,190,270,24,&YellowFont,&WhiteFont);
	RandomMapDesc RMP;
	RMP.Load("Terrain\\Res.dat");
	ComboBox* CBSTYLE=DSS.addGP_ComboBoxDLX(NULL,267,167,255,CBB_GPFILE,0,9,0,&WhiteFont,&YellowFont,NULL);
	CBSTYLE->Center-=5;
	CBSTYLE->FontDy-=2;
	CBSTYLE->OneDy-=1;
	for(int p=0;p<RMP.NSTY;p++){
		CBSTYLE->AddLine(RMP.STY[p].Name);
	};
	CBSTYLE->CurLine=RM_LandType;
	ComboBox* CBRESST=DSS.addGP_ComboBoxDLX(NULL,267,167+52,255,CBB_GPFILE,0,9,0,&WhiteFont,&YellowFont,NULL);
	CBRESST->Center-=5;
	CBRESST->FontDy-=2;
	CBRESST->OneDy-=1;
	for(int p=0;p<RMP.NRES;p++){
		CBRESST->AddLine(RMP.RES[p].Name);
	};
	CBRESST->CurLine=RM_Resstart;
	ComboBox* CBRESTOT=DSS.addGP_ComboBoxDLX(NULL,267,167+52*2,255,CBB_GPFILE,0,9,0,&WhiteFont,&YellowFont,NULL);
	CBRESTOT->Center-=5;
	CBRESTOT->FontDy-=2;
	CBRESTOT->OneDy-=1;
	for(int p=0;p<RMP.NMINES;p++){
		CBRESTOT->AddLine(RMP.MINES[p].Name);
	};
	CBRESTOT->CurLine=RM_Restot;
	ItemChoose=-1;

	GP_TextButton* OkBtn=DSS.addGP_TextButton(NULL,69,373,GetTextByID("INTF_GENERATE"),BTNS.GPID,5,FABut,FPBut);
	OkBtn->OnUserClick=&MMItemChoose;
	OkBtn->UserParam=mcmOk;
	GP_TextButton* CancelBtn=DSS.addGP_TextButton(NULL,301,373,GetTextByID("INTF_CANCEL"),BTNS.GPID,3,FABut,FPBut);
	CancelBtn->OnUserClick=&MMItemChoose;
	CancelBtn->UserParam=mcmCancel;

	OkBtn->FontDy += 4;
	CancelBtn->FontDy += 4;
	OkBtn->FontDx += 8;
	CancelBtn->FontDx += 8;

	int prevNat=MyNation;
	do{
		if(PLAYER->CurLine!=prevNat){
			prevNat=PLAYER->CurLine;
			if(prevNat<8){
				for(int i=0;i<NRes;i++){
					RESLOC[RESRID[i]]=RES[prevNat][RESRID[i]];
					sprintf(ResST[i],"%d",RESLOC[RESRID[i]]);
				};
			};
			IBN->Str=PLNAMES[prevNat];
		};
		for(int i=0;i<NRes;i++){
			RESLOC[RESRID[i]]=atoi(ResST[i]);
		};
		ProcessMessages();
		DSS.ProcessDialogs();
		DSS.RefreshView();
	}while(ItemChoose==-1);
	if(ItemChoose==mcmOk){
		if(PLAYER->CurLine==8){
			for(int i=0;i<8;i++){
				for(int j=0;j<NRes;j++){
					RES[i][RESRID[j]]=RESLOC[RESRID[j]];
				};
			};
		}else{
			for(int i=0;i<NRes;i++){
				RES[prevNat][RESRID[i]]=RESLOC[RESRID[i]];
			};
		};
		if(ItemChoose==mcmOk){
			RM_LandType=CBSTYLE->CurLine;
			RM_Resstart=CBRESST->CurLine;
			RM_Restot=CBRESTOT->CurLine;
		};
		strcpy(MapScenaryDLL,DLLPT);
	};
	free(Back);
	return ItemChoose;
};
//---------------------------------------------------------//
int TM_Angle0=0;
int TM_Angle1=89;
int TM_Tan0;
int TM_Tan1;
int TM_Height0=0;
int TM_Height1=1024;
bool TexMapMod=false;


//---------------------------------------------------------//
void ProcessFranceMission();
bool SingleOptions();

CEXPORT int NameChoose=-1;
CEXPORT
bool MMChooseName(SimpleDialog* SD){
	NameChoose=SD->UserParam;
	Lpressed=false;
	return false;
};
void __stdcall CDGINIT_INIT2();
void __stdcall CDGINIT_INIT1();
#ifdef MAKE_PTC
SFLB_DLLEXPORT SFLB_ReadFirstPlayerName(char* ccc){
	CDGINIT_INIT1();
	CDGINIT_INIT2();
	GFILE* F=Gopen("players.txt","r");
	int N=0;
	ccc[0]=0;
	if(F){
		ccc[0]=0;
		int cc=0;
		int nn=0;
		while(!(cc==0x0A||cc==EOF)){
			cc=Ggetch(F);
			if(!(cc==0x0A||cc==EOF)){
				ccc[nn]=cc;
				nn++;
			}else{
				//Ggetch(f);
			};
		};
		ccc[nn]=0;
		Gclose(F);
	};
};
#endif
bool ENRETVAL=0;
#ifdef MAKE_PTC

void EnterName(char* BackGround){
	ENRETVAL=EnterName();
};

#endif
bool MPL_EnterName(){
	EnterName("Interface\\Multi_opt\\Background_Multi_Options.bmp");
	return ENRETVAL;
};
bool Camp_EnterName(){
	EnterName("Interface\\Campagins\\Background_Campagin.bmp");
	return ENRETVAL;
};
bool CommEnterName(){
	EnterName("Interface\\MainMenu\\Background_main_opt.bmp");
	return ENRETVAL;
};
CEXPORT char IPADDR[128]="";
CEXPORT bool TOTALEXIT=0;

extern DPID MyDPID;
bool PIEnumeratePlayers(PlayerInfo* PIN,bool DoMsg);
//PlayerInfo GSets.CGame.PL_INFO[8];
char Names[32][8];
int tmm1,tmm2,tmm3;
extern int COUN;
extern char** NatNames;
extern char** NatScripts;
extern int NNations;
char* Unknm="?";
char* GetPName(int i){
	for(int j=0;j<8;j++)if(GSets.CGame.PL_INFO[j].ColorID==i)return GSets.CGame.PL_INFO[j].name;
	return Unknm;
};
bool ColorMouseOver(SimpleDialog* SD){
	if(!SD->Enabled)return false; 
	GPPicture* CB=(GPPicture*)SD;
	if(Lpressed){
		CB->Nation++;
		if(CB->Nation>6)CB->Nation=0;
		Lpressed=false;
	};
	if(Rpressed){
		CB->Nation--;
		if(CB->Nation<0)CB->Nation=6;
		Rpressed=false;
	};
	return true;
};
bool ChangeAlias(SimpleDialog* SD){
	if((!SD->Enabled)||(!SD->Visible))return false;
	GPPicture* GPP=(GPPicture*)SD;
	if(Lpressed){
		if(GPP->Nation<7)GPP->Nation++;
		else GPP->Nation=0;
		Lpressed=0;
	};
	if(Rpressed){
		if(GPP->Nation>0)GPP->Nation--;
		else GPP->Nation=7;
		Rpressed=0;

	};
	return true;
};
extern int tmtmt;
bool GetPreview(char* Name,byte* Data);
void Decode(byte* data,byte* xlt,int Size){
	for(int i=0;i<Size;i++)data[i]=xlt[data[i]];
};
char* UNKN="?UNKNOWN?";
char* GetPlNameByID(DWORD ID){
	for(int i=0;i<NPlayers;i++){
		if(GSets.CGame.PL_INFO[i].PlayerID==ID){
			return GSets.CGame.PL_INFO[i].name;
		};
	};
	return UNKN;
};
void ControlLingvo(char* str){
	char* c1=strstr(str,"fuck");
	if(c1){
		c1[0]='?';
		c1[1]='?';
		c1[2]='?';
		c1[3]='?';
	};
	c1=strstr(str,"FUCK");
	if(c1){
		c1[0]='?';
		c1[1]='?';
		c1[2]='?';
		c1[3]='?';
	};
};
void StopConnectionToSession(LPDIRECTPLAY3A lpDirectPlay3A);
extern int GLOBALTIME;
extern int PGLOBALTIME;
extern int CurrentStartTime[8];
extern int NextStartTime[8];
CEXPORT
void SendPings();
bool CheckPingsReady();

void ClearCTime(){
	memset(CurrentStartTime,0xFF,8*4);
	memset(NextStartTime,0xFF,8*4);
};
int LoadGP=0;
void ShowCentralMessage(char* Message,int GPIDX){
	char cc[256];
	strcpy(cc,GPS.GetGPName(GPIDX));
	strupr(cc);
	if(!strcmp(cc,"INTERFACE\\BOR2")){
		if(!LoadGP){
			LoadGP=GPS.PreLoadGPImage("Interface\\_Loading");
			if(LoadGP>0)GPS.LoadGP(LoadGP);
		};
		if(LoadGP>0){
			int L=GPS.GetGPWidth(LoadGP,0);
			int DX=0;
			GPS.ShowGP((RealLx-L)/2,(RealLy-80)/2,LoadGP,0,0);
			L=GetRLCStrWidth(Message,&WhiteFont);
			ShowString((RealLx-L)/2+1-DX,(RealLy-GetRLCHeight(YellowFont.RLC,'W'))/2+1,Message,&BlackFont);
			ShowString((RealLx-L)/2-DX,(RealLy-GetRLCHeight(YellowFont.RLC,'W'))/2,Message,&YellowFont);
			return;
		};
	};
	int L=GetRLCStrWidth(Message,&WhiteFont);
	int DX=0;
	DrawStdBar2((RealLx-L-64)/2-DX,(RealLy-70)/2+18,(RealLx+L+64)/2-DX,(RealLy-70)/2+55,GPIDX);
	ShowString((RealLx-L)/2+1-DX,(RealLy-GetRLCHeight(YellowFont.RLC,'W'))/2+1,Message,&BlackFont);
	ShowString((RealLx-L)/2-DX,(RealLy-GetRLCHeight(YellowFont.RLC,'W'))/2,Message,&YellowFont);
};
CEXPORT
void ShowClanString(int x,int y,char* s,byte State,RLCFont* Fn,RLCFont* Fn1,int DY,int LX);
void xLine(int x,int y,int x1,int y1,byte c);
int SearchPlayer(char* Nick){
	for(int i=0;i<TPEN.NPlayers;i++){
		if(!strcmp(Nick,TPEN.Players[i].Name))return i;
	};
	for(int i=0;i<TPEN.ABPL.NPlayers;i++){
		if(!strcmp(Nick,TPEN.ABPL.Names[i]))return i+TPEN.NPlayers;
	};
	return -1;
};
void Draw_PLIST(int x,int y,int Lx,int Ly,int Index,byte Active,int param){
	if(Index<TPEN.NPlayers){
		int GPF=param;
		RLCFont* FONT=Active==2?&WhiteFont:&YellowFont;
		RLCFont* FONT1=NULL;
		char cc3[256];
		strcpy(cc3,TPEN.Players[Index].Name);
		//char* cc=strstr(cc3,"[inside]");
		//if(cc)cc[0]=0;
		bool Really=1;
		int DYF=0;
		if(TPEN.Players[Index].ProfState==15&&TPEN.Players[Index].ProfileID){
			//strcat(cc3,"(R)");
			FONT=Active==2?&SpecialWhiteFont:&SpecialYellowFont;
			FONT1=&SpecialRedFont;
			DYF=-5;
			Really=1;
		};
		if(TPEN.Players[Index].ProfState==1||
			(TPEN.Players[Index].ProfState==3&&TPEN.Players[Index].ProfileID)){
			strcat(cc3,"(?)");
		};
		if(Really)ShowClanString(x+45-6,y+3,cc3,Active,FONT,FONT1,DYF,Lx);
		else{
			if(FONT1)ShowString(x+45-6+1,y+3+DYF+1,cc3,FONT1);
			ShowString(x+45-6,y+3+DYF,cc3,FONT);
		};
		int png=TPEN.Players[Index].Ping;
		int spp=0;
		if(png<0)spp=16;
		else if(png<300)spp=12;
		else if(png<500)spp=13;
		else if(png<900)spp=14;
		else spp=15;
		GPS.ShowGP(x,y,GPF,spp,0);
		if(TPEN.Players[Index].Muted){
			xLine(x+45-7,y+12,x+45+GetRLCStrWidth(cc3,&YellowFont),y+12,RedColor2);
			xLine(x+45-7,y+13,x+45+GetRLCStrWidth(cc3,&YellowFont),y+13,RedColor2);
		};

	}else{
		Index-=TPEN.NPlayers;
		//if(Index<TPEN.NRoomPlayers){
		if(Index<TPEN.ABPL.NPlayers){
			RLCFont* FONT=&RedFont;
			int GPF=param;
			ShowString(x+45,y+3,TPEN.ABPL.Names[Index],FONT);
			GPS.ShowGP(x,y,GPF,17,0);
		};
	};
};
char* BATTLTXT=NULL;
char* DEATHTXT=NULL;
void LimitString(char* str,lpRLCFont FONT,int L){
	if(L<0)return;
	if(str&&str[0]){
		if(strstr(str,"{")){
			return;
		}
		int L0;
		do{
			L0=GetRLCStrWidth(str,FONT);
			if(L0>L){
				int LL=strlen(str);
				//str[LL-4]='>';
				//str[LL-3]='>';
				//str[LL-2]='>';
				str[LL-1]=0;
			};
		}while(L0>L);
	}
};
extern word dwVersion;
void CreateRoomsHintString(int Idx,char* hint,bool Active){
	hint[0]=0;
	if(Idx<TPEN.NFRooms){
		int Index=TPEN.FiltRef[Idx];
		if(Index<1000000){
			if(Index>=TPEN.NRooms)return;
			GServer G=TPEN.Room[Index].G;
			strcpy(hint,TPEN.Room[Index].Name);
			char* cc=ServerGetStringValue(G,"mapname","");
			cc=strstr(cc,"|");
			if(cc){
				strcat(hint,", ");
				strcat(hint,cc+1);
				strcat(hint,"\\");
			}else strcat(hint,"\\");
			cc=ServerGetStringValue(G,"gamemode","x");
			if(!strcmp(cc,"closedplaying")){
				strcat(hint,GetTextByID("GAMINPR"));
			}else{
				if(strcmp(cc,"wait")){
					strcat(hint,GetTextByID("GAMINAR"));
				}else{
					if(Active){
						cc=ServerGetStringValue(G,"gamever","0");
						char test[16];
						sprintf(test,"%d.%d%d",dwVersion/100,(dwVersion/10)%10,dwVersion%10);
						if(strcmp(cc,test)){
							sprintf(hint+strlen(hint),GetTextByID("IINVVER"),test);
						}else{
							int max=ServerGetIntValue(G,"maxplayers",0);
							int np=ServerGetIntValue(G,"numplayers",0);
							if(np>=max){
								strcat(hint,GetTextByID("ROMFULL"));
							}else{
								cc=ServerGetStringValue(G,"password","0");
								if(cc[0]=='1'){
									strcat(hint,GetTextByID("RMPASSW"));
								}else{
									int L=GetCEW(G,1);
									if(L==3)strcat(hint,GetTextByID("CANJOIN3")); else 
									if(L==2)strcat(hint,GetTextByID("CANJOIN2")); else 
									if(L==1)strcat(hint,GetTextByID("CANJOIN1")); else 
									strcat(hint,GetTextByID("CANJOIN0"));
								};
							};
						};
					};
				};
			};
		};
	}else hint[0]=0;
};
void CreatePlayerHintString(int Index,char* hint){ 
	if(Index<TPEN.NPlayers){
		if(TPEN.Players[Index].Ping==-1){
			sprintf(hint,GetTextByID("DBLCLICKU"),
				TPEN.Players[Index].Name);
		}else{
			sprintf(hint,GetTextByID("DBLCLICK"),
				TPEN.Players[Index].Name,TPEN.Players[Index].Ping);
		};
	}else{
		Index-=TPEN.NPlayers;
		if(Index<TPEN.ABPL.NPlayers){
			sprintf(hint,GetTextByID("PLINGAME"),TPEN.ABPL.Names[Index]);
		};
	};
};
void Draw_RLIST(int x,int y,int Lx,int Ly,int Idx,byte Active,int param){
	if(Idx<TPEN.NFRooms){
		int Index=TPEN.FiltRef[Idx];
		if(Index<1000000){
			int Sign=0;
			if(Index>=TPEN.NRooms)return;
			int GPF=param;
			RLCFont* FONT=Active==2?&WhiteFont:&YellowFont;
			GServer GG=TPEN.Room[Index].G;
			char ccc3[256];
			char* ccx1=ServerGetStringValue(GG,"password","0");
			strcpy(ccc3,TPEN.Room[Index].Name);
			char* ccv1=ServerGetStringValue(GG,"mapname","0");
			ccv1=strstr(ccv1,"|");
			if(ccv1){
				strcat(ccc3,", ");
				strcat(ccc3,ccv1+1);
			}
			if(ccx1[0]=='1')Sign=1;//strcat(ccc3," [PASSWORD]");
			LimitString(ccc3,FONT,310);
			ShowString(x+45,y+3,ccc3,FONT);
			int png=ServerGetPing(GG);
			char cc[32];
			if(png!=9999){
				sprintf(cc,"%d",png);
				ShowString(486-GetRLCStrWidth(cc,FONT)/2,y+3,cc,FONT);
			};
			char* ccx=ServerGetStringValue(GG,"gamever","?");
			if(ccx){
				ShowString(552-GetRLCStrWidth(ccx,FONT)/2,y+3,ccx,FONT);
			};
			int max=ServerGetIntValue(GG,"maxplayers",0);
			int np=ServerGetIntValue(GG,"numplayers",0);
			sprintf(cc,"%d/%d",np,max);
			ShowString(422-GetRLCStrWidth(cc,FONT)/2,y+3,cc,FONT);
			if(!BATTLTXT){
				BATTLTXT=GetTextByID("BATTLTXT");
				DEATHTXT=GetTextByID("DEATHTXT");
			};
			int TYPE=GetCEW(GG,0);
			if(TYPE){
				ShowString(593,y+3,BATTLTXT,FONT);
			}else{
				ShowString(593,y+3,DEATHTXT,FONT);
			};
			ccx=ServerGetStringValue(GG,"gamemode","x");
			if(!strcmp(ccx,"closedplaying")){
				GPS.ShowGP(x,y,GPF,17,0);
			}else
			if(strcmp(ccx,"wait")){
				GPS.ShowGP(x,y,GPF,60,0);//spy icon
			}else if(Sign){
				GPS.ShowGP(x,y,GPF,61,0);//lock
			};
			int Lev=GetCEW(GG,1);
			if(Lev&&Lev<4)GPS.ShowGP(731,y+2,GPF,61+Lev,0);
			/*
			int png=TPEN.Players[Index].Ping;
			int spp=0;
			if(png<0)spp=16;
			else if(png<300)spp=12;
			else if(png<500)spp=13;
			else if(png<900)spp=14;
			else spp=15;
			GPS.ShowGP(x,y,GPF,spp,0);
			*/
		}else{
			Index-=1000000;
			if(Index<TPEN.NRInGame){
				RoomInGame* RIG=TPEN.RInGame+Index;
				int GPF=param;
				RLCFont* FONT=&RedFont;
				ShowString(x+45,y+3,RIG->Name,FONT);
				char cc[32];
				sprintf(cc,"%d",RIG->NPlayers);
				ShowString(422-GetRLCStrWidth(cc,FONT)/2,y+3,cc,FONT);
				GPS.ShowGP(x,y,GPF,17,0);
				if(!BATTLTXT){
					BATTLTXT=GetTextByID("BATTLTXT");
					DEATHTXT=GetTextByID("DEATHTXT");
				};
				if(RIG->Type){
					ShowString(593,y+3,BATTLTXT,FONT);
				}else{
					ShowString(593,y+3,DEATHTXT,FONT);
				};
			};
		};
	};
};
char SessionName[128]="";
char SessPassword[64]="";
int GMMOD=0;
int GMLEV=0;
int GMMAXPL=7;
void DRAWBOX(int x,int y,int Lx,int Ly,int Idx,byte Active,int param){
	DrawStdBar2(x,y,x+Lx-1,y+Ly-1,param);
}
bool EnterPassword(){
	LocalGP BARS("Interface\\bor2");
	DarkScreen();
	DialogsSystem DSS(0,0);
	SessPassword[0]=0;
	CustomBox* CBOX=DSS.addCustomBox(RealLx/2-170,RealLy/2-44,340,80,&DRAWBOX);
	CBOX->param=BARS.GPID;
	InputBox* IB=DSS.addInputBox(NULL,RealLx/2-143,RealLy/2-2,SessPassword,40,160,20,&WhiteFont,&YellowFont);
	IB->Active=1;
	char* cc=GetTextByID("ENTERPASSWORD");
	DSS.addTextButton(NULL,RealLx/2,RealLy/2-40,cc,&BigYellowFont,&BigYellowFont,&BigYellowFont,1);
	ColoredBar* CB=DSS.addColoredBar(RealLx/2-150,RealLy/2-4,300,24,0xAD);
	CB->Style=1;
	ItemChoose=-1;
	LastKey=0;
	KeyPressed=0;
	do{
		ProcessMessages();
		StdKeys();
		DSS.MarkToDraw();
		DSS.ProcessDialogs();
		DSS.RefreshView();
	}while(ItemChoose==-1);
	return ItemChoose==mcmOk;
};
void EnterPersonalMessage(char* Nick){
	char nick[128];
	strcpy(nick,Nick);
	LocalGP BARS("Interface\\bor2");
	DarkScreen();
	DialogsSystem DSS(0,0);
	int DL=60;
	int Y0=RealLy/2-44+10+70+3;
	int X0=RealLx/2-70-DL-80;
	DSS.addGPPicture(NULL,X0,Y0,BARS.GPID,14);
	GPS.ShowGP(X0,Y0,BARS.GPID,17,0);
	GP_TextButton* OK=DSS.addGP_TextButton(NULL,X0+9,Y0+20,GetTextByID("@OK"),BARS.GPID,15,&WhiteFont,&YellowFont);
	OK->OnUserClick=&MMItemChoose;
	OK->UserParam=mcmOk;

	X0=RealLx/2+70+DL-80;
	DSS.addGPPicture(NULL,X0,Y0,BARS.GPID,14);
	GPS.ShowGP(X0,Y0,BARS.GPID,17,0);
	GP_TextButton* CANCEL=DSS.addGP_TextButton(NULL,X0+9,Y0+20,GetTextByID("@CANCEL"),BARS.GPID,15,&WhiteFont,&YellowFont);
	CANCEL->OnUserClick=&MMItemChoose;
	CANCEL->UserParam=mcmCancel;

	char MESSAGE[256]="";
	
	CustomBox* CBOX=DSS.addCustomBox(RealLx/2-170-DL,RealLy/2-44+10,340+DL*2,80-10,&DRAWBOX);
	CBOX->param=BARS.GPID;
	InputBox* IB=DSS.addInputBox(NULL,RealLx/2-143-10-DL,RealLy/2-2,MESSAGE,120,290+DL*2,20,&WhiteFont,&YellowFont);
	IB->Active=1;
	char cc2[128];
	sprintf(cc2,GetTextByID("ENTERMESS"),nick);
	DSS.addTextButton(NULL,RealLx/2,RealLy/2-40+10,cc2,&YellowFont,&YellowFont,&YellowFont,1);
	ColoredBar* CB=DSS.addColoredBar(RealLx/2-150-DL,RealLy/2-4+1,300+DL*2,24-1,135);//107
	CB->Style=1;
	

	ItemChoose=-1;
	LastKey=0;
	KeyPressed=0;
	do{
		ProcessMessages();
		StdKeys();
		DSS.MarkToDraw();
		DSS.ProcessDialogs();
		DSS.RefreshView();
	}while(ItemChoose==-1&&!GameInProgress);
	if(ItemChoose==mcmOk){
		if(TPEN.Peer&&TPEN.Connected){
			if(MESSAGE[0]){
				peerMessagePlayer(TPEN.Peer,nick,MESSAGE);
				char ccc[128];
				sprintf(ccc,"%s-->%s",TPEN.MyNick,nick);
				TPEN.GlobalChat.Add(ccc,MESSAGE);
			};

		};
	};
	ItemChoose=-1;
};
bool CreateSessionDialog(){
	if(SessionName[0]==127)strcpy(SessionName,SessionName+5);
	LocalGP BTNS("Interface\\CreateIGame");
	DialogsSystem DSS(((1024-452)>>1)-80,(768-237-26)>>1);
	GPS.ShowGP(DSS.BaseX,DSS.BaseY,BTNS.GPID,1,1);
	DSS.addGPPicture(NULL,0,0,BTNS.GPID,0);
	GP_Button* OKBTN=DSS.addGP_Button(NULL,67,192+26,BTNS.GPID,4,5);
	OKBTN->UserParam=mcmOk;
	OKBTN->OnUserClick=&MMItemChoose;
	GP_Button* CANCELBTN=DSS.addGP_Button(NULL,256,192+26,BTNS.GPID,2,3);
	CANCELBTN->UserParam=mcmCancel;
	CANCELBTN->OnUserClick=&MMItemChoose;
	InputBox* SESSN=DSS.addInputBox(NULL,191,58,SessionName,120,230,20,&YellowFont,&WhiteFont);
	SESSN->Active=1;
	InputBox* SESSPS=DSS.addInputBox(NULL,191,136+26+3,SessPassword,63,230,20,&YellowFont,&WhiteFont);
	GPPicture* GPNP=DSS.addGPPicture(NULL,195+(236-195)*(GMMAXPL-2),111+26,BTNS.GPID,15);
	GP_TextButton* NPL[6];
	for(int i=0;i<6;i++){
		char cc1[3]="2";
		cc1[0]='2'+i;
		NPL[i]=DSS.addGP_TextButton(NULL,195+(236-195)*i,111+26,cc1,BTNS.GPID,14,&WhiteFont,&YellowFont);
		NPL[i]->OnUserClick=&MMItemChoose;
		NPL[i]->UserParam=i;
	};
	ComboBox* LEV=DSS.addGP_ComboBox(NULL,195,85,BTNS.GPID,6,9,0,&WhiteFont,&YellowFont,NULL);
	LEV->AddLine(GetTextByID("LEV_0M"));
	LEV->AddLine(GetTextByID("LEV_1M"));
	LEV->AddLine(GetTextByID("LEV_2M"));
	LEV->AddLine(GetTextByID("LEV_3M"));
	LEV->CurLine=GMLEV;

	ComboBox* GCB=DSS.addGP_ComboBox(NULL,195,85+26,BTNS.GPID,6,9,0,&WhiteFont,&YellowFont,NULL);
	GCB->AddLine(GetTextByID("Deathmatch"));
	GCB->AddLine(GetTextByID("Battle"));
	GCB->AddLine(GetTextByID("Rated_Deathmatch"));
	GCB->CurLine=2;
	ItemChoose=-1;
	do{
		ProcessMessages();
		StdKeys();
		DSS.ProcessDialogs();
		DSS.RefreshView();
		GPNP->x=DSS.BaseX+195+(236-195)*(GMMAXPL-2);
		if(GCB->CurLine==1){
			for(int i=0;i<6;i++){
				NPL[i]->Visible=0;
				NPL[i]->Enabled=0;
			};
			GPNP->Visible=0;
		}else{
			for(int i=0;i<6;i++){
				NPL[i]->Visible=1;
				NPL[i]->Enabled=1;
				/*
				if(i==GMMAXPL-2){
					NPL[i]->ActiveFont=&BigWhiteFont;
					NPL[i]->PassiveFont=&BigYellowFont;
					NPL[i]->FontDy=-4;
				}else{
					NPL[i]->ActiveFont=&WhiteFont;
					NPL[i]->PassiveFont=&YellowFont;
					NPL[i]->FontDy=0;
				};
				*/
			};
			GPNP->Visible=1;
		};
		if(ItemChoose>=0&&ItemChoose<7){
			GMMAXPL=ItemChoose+2;
			ItemChoose=-1;
		};
		for(int i=0;i<6;i++){
			NPL[i]->Sprite=14+(i==(GMMAXPL-2));
		};
		OKBTN->Enabled=SessionName[0]!=0;
	}while(ItemChoose==-1);
	if(GMMOD=GCB->CurLine==2){
		char ccc[256];
		strcpy(ccc,"0017");
		strcat(ccc,SessionName);
		strcpy(SessionName,ccc);
	};
	GMMOD=GCB->CurLine&1;
	GMLEV=LEV->CurLine;
	return ItemChoose==mcmOk;
};
void NoWaitWithMessage(char* Message){
	LocalGP BOR2("Interface\\bor2");
	DarkScreen();
	ShowCentralMessage(Message,BOR2.GPID);
	FlipPages();
};
void WaitWithMessage(char* Message){
	LocalGP BOR2("Interface\\bor2");
	DarkScreen();
	ShowCentralMessage(Message,BOR2.GPID);
	FlipPages();
	int T=GetTickCount();
	KeyPressed=0;
	do{
		ProcessMessages();
	}while(GetTickCount()-T<3000&&!KeyPressed);
	KeyPressed=0;
};
char ROOMNAMETOCONNECT[128];
extern char LobbyVersion[32];
bool CheckJoin(GServer G){
	char* ccx=ServerGetStringValue(G,"gamemode","x");
	char* ccv=ServerGetStringValue(G,"gamever","x");
	//sprintf(cc3,"%d.%d%d",dwVersion/100,(dwVersion/10)%10,dwVersion%10);
	return strcmp(ccx,"wait")==0&&!strcmp(ccv,LobbyVersion);
};
extern word dwVersion;


int NCHATS=0;
int MAXCHATS=0;
char** ChatMess=NULL;
char** ChatSender=NULL;
void AddPrimitiveChat(char* Nick,char* str,
				  char** &ChatMess,char** &ChatSender,
				  int &NCHATS,int &MAXCHATS){
	if(NCHATS>=MAXCHATS){
		MAXCHATS+=32;
		ChatMess=(char**)realloc(ChatMess,4*MAXCHATS);
		ChatSender=(char**)realloc(ChatSender,4*MAXCHATS);
	};
	if(NCHATS>512){
		free(ChatMess[0]);
		free(ChatSender[0]);
		memcpy(ChatMess,ChatMess+1,NCHATS*4);
		memcpy(ChatSender,ChatSender+1,NCHATS*4);
		NCHATS--;
	};
	ChatMess[NCHATS]=znew(char,strlen(str)+1);
	strcpy(ChatMess[NCHATS],str);
	ChatSender[NCHATS]=znew(char,strlen(Nick)+1);
	strcpy(ChatSender[NCHATS],Nick);
	NCHATS++;
};
void AddChatString(char* Nick,char* str,int MaxLx,lpRLCFont FONT,
				  char** &ChatMess,char** &ChatSender,
				  int &NCHATS,int &MAXCHATS){
	bool FLine=1;
	char ACCAMULATOR[256];
	int pp=0;
	int sp=0;
	int LL=strlen(str);
	sprintf(ACCAMULATOR,"%s: ",Nick);
	int LName=GetRLCStrWidth(ACCAMULATOR,FONT);
	if(LName==32)LName++;
	ACCAMULATOR[0]=0;
	do{
		int Lx=GetRLCStrWidth(ACCAMULATOR,FONT)+LName;
		int Lx1=strlen(ACCAMULATOR);
		ACCAMULATOR[Lx1]=str[sp];
		ACCAMULATOR[Lx1+1]=0;
		sp++;
		if(Lx>MaxLx){
			if(LName!=32){
				AddPrimitiveChat(Nick,ACCAMULATOR,ChatMess,ChatSender,NCHATS,MAXCHATS);
				LName=32;
				ACCAMULATOR[0]=0;
			}else{
				AddPrimitiveChat("",ACCAMULATOR,ChatMess,ChatSender,NCHATS,MAXCHATS);
				ACCAMULATOR[0]=0;
			};
		};
	}while(sp<LL);
	if(ACCAMULATOR[0]){
		if(LName!=32){
			AddPrimitiveChat(Nick,ACCAMULATOR,ChatMess,ChatSender,NCHATS,MAXCHATS);
			LName=32;
			ACCAMULATOR[0]=0;
		}else{
			AddPrimitiveChat("",ACCAMULATOR,ChatMess,ChatSender,NCHATS,MAXCHATS);
			ACCAMULATOR[0]=0;
		};
	};
};
bool SENDPRIVMESS(SimpleDialog* SD){
	ComplexBox* CB=(ComplexBox*)SD;
	if(CB->CurrentItem>=0){
		if(CB->CurrentItem<TPEN.NPlayers){
			EnterPersonalMessage(TPEN.Players[CB->CurrentItem].Name);
		}else{
			if(CB->CurrentItem-TPEN.NPlayers<TPEN.ABPL.NPlayers){
				EnterPersonalMessage(TPEN.ABPL.Names[CB->CurrentItem-TPEN.NPlayers]);
			};
		};
	};
	KeyPressed=0;
	LastKey=0;
	return true;
};
bool DisableCreate=0;
extern bool UnderFirewall;

extern char LobbyVersion[32];
void CheckProfileInit();
int GetGerbByScore(int Score){
	if(Score<5*6)return Score/5;
	Score-=5*6;
	if(Score<10*7)return 6+Score/10;
	Score-=10*7;
	if(Score<20*4)return 13+Score/20;
	Score-=20*4;
	if(Score<30*2)return 17+Score/30;
	Score-=30*2;
	if(Score<50*3)return 19+Score/50;
	Score-=50*3;
	if(Score<100*3)return 22+Score/100;
	Score-=100*3;
	if(Score<150*5)return 25+Score/150;
	Score-=150*5;
	int rr=30+Score/200;
	if(rr>41)rr=41;
	return rr;
};
int GetRankByScore(int Score){
	int rr=GetGerbByScore(Score);
	if(rr<5 )return 0;
	if(rr<12)return 1;
	if(rr<16)return 2;
	if(rr<18)return 3;
	if(rr<21)return 4;
	if(rr<24)return 5;
	if(rr<29)return 6;
	if(rr<34)return 7;
	return 8;
};
extern char** NatNames;
void GPI_callback3(GPConnection* gp,void* arg,void* par){
	GPGetInfoResponseArg* GIRA=(GPGetInfoResponseArg*)arg;
	GPGetInfoResponseArg* GPI=(GPGetInfoResponseArg*)par;
	*GPI=*GIRA;
};
extern GPConnection* LOGIN_gp;
void GETCOUNTRY(char* code,char* res);
void DRAW_TOP100(int x,int y,int Lx,int Ly,int Index,byte Active,int param){
	if(Index>=0&&Index<100){
		int fdy=3;
		RLCFont* FNT=&YellowFont;
		if(Active==2){
			FNT=&WhiteFont;
		};
		if(T100.CLIENT[Index].PLINF
			&&T100.CLIENT[Index].PLINF->nick[0]){
			ShowString(x+40,y+fdy,T100.CLIENT[Index].PLINF->nick,FNT);
			char ccc[64];
			if(T100.CLIENT[Index].Score){
				sprintf(ccc,"%d",T100.CLIENT[Index].Score-1);
			}else{
				strcpy(ccc,"???");
			};
			ShowString(x+698-GetRLCStrWidth(ccc,FNT)/2,y+fdy,ccc,FNT);
			sprintf(ccc,"%d",Index+1);
			ShowString(x+15-GetRLCStrWidth(ccc,FNT)/2,y+fdy,ccc,FNT);
			strcpy(ccc,T100.CLIENT[Index].PLINF->homepage);
			ShowString(x+261-GetRLCStrWidth(ccc,FNT)/2,y+fdy,ccc,FNT);
			SYSTEMTIME BIRTH;
			SYSTEMTIME CURTIME;
			FILETIME FT_BIRTH;
			FILETIME FT_CURTIME;
			GetSystemTime(&CURTIME);
			memset(&BIRTH,0,sizeof BIRTH);
			BIRTH.wYear=T100.CLIENT[Index].PLINF->birthyear;
			BIRTH.wMonth=T100.CLIENT[Index].PLINF->birthmonth;
			BIRTH.wDay=T100.CLIENT[Index].PLINF->birthday;
			SystemTimeToFileTime(&BIRTH,&FT_BIRTH);
			SystemTimeToFileTime(&CURTIME,&FT_CURTIME);
			LARGE_INTEGER L_BIRTH;
			LARGE_INTEGER L_CURTIME;
			memcpy(&L_BIRTH,&FT_BIRTH,8);
			memcpy(&L_CURTIME,&FT_CURTIME,8);
			L_CURTIME.QuadPart-=L_BIRTH.QuadPart;
			memcpy(&FT_CURTIME,&L_CURTIME,8);
			FileTimeToSystemTime(&FT_CURTIME,&CURTIME);
			sprintf(ccc,"%d",CURTIME.wYear-1601);
			ShowString(x+378-GetRLCStrWidth(ccc,FNT)/2,y+fdy,ccc,FNT);

			int NT=T100.CLIENT[Index].NBattl;
			int NV=T100.CLIENT[Index].NVict;
			int NI=T100.CLIENT[Index].NIncomp;

			sprintf(ccc,"%d",NT+NI);
			ShowString(x+459-GetRLCStrWidth(ccc,FNT)/2,y+fdy,ccc,FNT);
			sprintf(ccc,"%d",NV);
			ShowString(x+539-GetRLCStrWidth(ccc,FNT)/2,y+fdy,ccc,FNT);
			if(NT)sprintf(ccc,"%d(%d%%)",NI,NI*100/(NT+NI));
			else sprintf(ccc,"%d",NI);
			ShowString(x+618-GetRLCStrWidth(ccc,FNT)/2,y+fdy,ccc,FNT);
		};
	};
};
typedef byte GR_ARR[8][32]; 
int DrawGraph(Canvas* CAN,int y0,int Npt,GR_ARR Data,byte* Col,int N,word* Max,char* Text){
	byte COL=0x7C;
	byte COL1=0x88;
	int L1=16;
	int L2=102;
	int L3=L2-2;
	int Lx=391;
	CAN->AddRect(0,y0,Lx+2,L1,COL);
	CAN->AddText(10,y0+2,Text,&SmallWhiteFont);
	y0+=L1;

	CAN->AddRect(0,y0,Lx+2,L2,COL);
	CAN->AddBar(1,y0+1,Lx,L2-2,COL1);
	int MaxV=1;
	for(int i=0;i<N;i++)if(Max[i]>MaxV)MaxV=Max[i];
	int divisor=100*MaxV;
	for(int i=0;i<N;i++){
		byte C=0xD0+Col[i]*4;
		byte* data=&Data[i][0];
		int Maxi=Max[i];
		for(int j=1;j<Npt;j++){
			int x0=1+((j-1)*Lx)/Npt;
			int x1=1+(j*Lx)/Npt;
			CAN->AddLine(1+((j-1)*Lx)/Npt,
						 y0+1+L3-(int(data[j-1])*Maxi*(L3-2))/divisor,
				         1+(j*Lx)/Npt,
						 y0+1+L3-(int(data[j])*Maxi*(L3-2))/divisor,C);
		};
	};
	char cc[32];
	sprintf(cc,"%d",MaxV);
	CAN->AddText(10,y0+2,cc,&SmallWhiteFont);
	return L2+L1;
};
int  RESPOND=0;
//1-ok
//2-password
//3-invalid version
//4-room is full
//5-not created
//6-room is in the game
DWORD  PASSHASH=0;
word JP_ver=0;
DWORD CalcPassHash(char* pass){
	int L=strlen(pass);
	DWORD S=0;
	int i;
	for(i=0;i<L;i++)S+=pass[i];
	for(i=0;i<L-1;i++)S+=pass[i]*pass[i+1];
	for(i=0;i<L-2;i++)S+=pass[i]*pass[i+1]*pass[i+2];
	for(i=0;i<L-3;i++)S+=pass[i]*pass[i+1]*pass[i+2]*pass[i+3];
	for(i=0;i<L-4;i++)S+=pass[i]*pass[i+1]*pass[i+2]*pass[i+3]*pass[i+4];
	for(i=0;i<L-5;i++)S+=pass[i]*pass[i+1]*pass[i+2]*pass[i+3]*pass[i+4]*pass[i+5];
	return S;
};
bool GetPLIP(char* Nick,char* IP);
extern char RESPNICK[128];
bool TryToJoinToPlayer(char* name){
	char Name[128];
	strcpy(Name,name);
	LocalGP BOR2("Interface\\bor2");
	RESPOND=0;
	PASSHASH=0;
	JP_ver=0;
	peerMessagePlayer(TPEN.Peer,Name,"@@@I_WANT_TO_JOIN");
	int T0=GetTickCount();
	DarkScreen();
	ShowCentralMessage(GetTextByID("ICJOIN"),BOR2.GPID);
	FlipPages();
	int TT=GetTickCount();
	do{
		if(GetTickCount()-TT>3000){
			peerMessagePlayer(TPEN.Peer,Name,"@@@I_WANT_TO_JOIN");
			TT=GetTickCount();
		};
		ProcessMessages();
		if(RESPOND&&_stricmp(RESPNICK,Name))RESPOND=0;
	}while(GetTickCount()-T0<20000&&RESPOND==0);
	char* ERR=NULL;
	switch(RESPOND){
	case 0:
		ERR=GetTextByID("JNPG_E3");
		break;
	case 2:
		if(EnterPassword()){
			DWORD V=CalcPassHash(SessPassword);
			int T0=GetTickCount();
			do{
				ProcessMessages();
			}while(GetTickCount()-T0<1000);
			if(V!=PASSHASH){
				ERR=GetTextByID("JNPG_E6");
				return false;
			};
		}else return false;
		break;
	case 3:
		ERR=GetTextByID("JNPG_E2");
		break;
	case 4:
		ERR=GetTextByID("JNPG_E4");
		break;
	case 5:
		ERR=GetTextByID("JNPG_E1");
		break;
	case 6:
		ERR=GetTextByID("JNPG_E5");
		break;
	};
	if(ERR){
		DarkScreen();
		ShowCentralMessage(ERR,BOR2.GPID);
		FlipPages();
		int T0=GetTickCount();
		KeyPressed=0;
		do{
			ProcessMessages();
		}while(GetTickCount()-T0<10000&&!KeyPressed);
		return false;
	};
	return true;
};
void RunHTTPC(){
	STARTUPINFO			StartUpInfo;
	PROCESS_INFORMATION		ProcessInformation;
	memset(&StartUpInfo,0x00,sizeof(STARTUPINFO));
	StartUpInfo.cb=sizeof(STARTUPINFO);	
	StartUpInfo.wShowWindow=SW_SHOWMINNOACTIVE;
	StartUpInfo.dwFlags=STARTF_USESHOWWINDOW;

	CreateProcess(	"httpc.exe",
				NULL,
				NULL,
				NULL,
				FALSE,
				0x00,
				NULL,
				NULL,
				&StartUpInfo,
				&ProcessInformation);
};
bool GetPLIP(char* Nick,char* IP);
bool T100_cmd_done=0;
void DecodeGS_Password(char* pass,char* result);
bool CheckPlayerToExit();
extern bool RejectThisPlayer;
void SendPlayerRequest();
bool UseGSC_Login=0;
CEXPORT bool CheckGSC_Login(){
	return UseGSC_Login;
};
RoomInfo GlobalRIF;
int GetGSC_Profile(){
	return GlobalRIF.Profile;
};
int ProcessInternetConnection(bool Active){
	//if(UseGSC_Login){
		UseGSC_Login=true;
		v_MainMenu.Cross=0;
		int r=Process_GSC_ChatWindow(Active,&GlobalRIF);
		return r;
	//};
	if(Active){
		ItemChoose=-1;
		GameInProgress=0;
		PlayerMenuMode=1;
		/*
		for(int i=0;i<NCHATS;i++){
			free(ChatMess[i]);
			free(ChatSender[i]);
		};
		if(MAXCHATS){
			free(ChatMess);
			free(ChatSender);
		};
		NCHATS=0;
		MAXCHATS=0;
		ChatMess=NULL;
		ChatSender=NULL;
		*/
	};
	ClearScreen();
	LoadFog(2);
	LoadPalette("2\\agew_1.pal");
	LocalGP BTNS("Interface\\igame");
	LocalGP OK1("Interface\\anm_start_1_on");
	LocalGP OK2("Interface\\anm_start_1_off");
	LocalGP CANCEL1("Interface\\anm_cancel_on");
	LocalGP CANCEL2("Interface\\anm_cancel_off");
	LocalGP DIS("Interface\\an_start_disable");
	LocalGP SCROL("Interface\\_Slider");	
	LocalGP BOR2("Interface\\bor2");
	LocalGP IBTN("Interface\\IButtons");
	LocalGP GERB("Interface\\gerb_low2");
	LocalGP TOPGP("Interface\\top100");
	LocalGP SYM3("Interface\\isymb3");


	SQPicture Back("Interface\\Backgroung_Internet_Game.bmp");
	SQPicture IPLInfoBack("Interface\\Background_Player_Info.bmp");
	SQPicture ITOPBACK("Interface\\Background_Top100.bmp");
	Back.Draw(0,0);
	DarkScreen();
	ShowCentralMessage(GetTextByID("ICCONN"),BOR2.GPID);
	FlipPages();
	CheckProfileInit();
	if(!TPEN.Connected){
		Back.Draw(0,0);
		DarkScreen();
		ShowCentralMessage(GetTextByID("ICCONN"),BOR2.GPID);
		FlipPages();
		TPEN.Connect(PlName);
		bool NeedDraw=1;
		do{
			ProcessMessages();
			if(bActive){
				if(NeedDraw)FlipPages();
				NeedDraw=0;
			}else NeedDraw=1;
			if(GetTickCount()-TPEN.ConnStartTime>60000){
				TPEN.ErrorType=1;
			};
		}while(!(TPEN.Connected||TPEN.ErrorType));
		if(TPEN.ErrorType){
			Back.Draw(0,0);
			DarkScreen();
			ShowCentralMessage(GetTextByID("ICUNCON"),BOR2.GPID);
			FlipPages();
			bool NeedDraw=1;
			KeyPressed=0;
			int t0=GetTickCount();
			do{
				ProcessMessages();
				if(bActive){
					if(NeedDraw)FlipPages();
					NeedDraw=0;
				}else NeedDraw=1;
			}while((GetTickCount()-t0<20000)&&!KeyPressed);
			KeyPressed=0;
			peerShutdown(TPEN.Peer);
			TPEN.Peer=NULL;
			return false;
		};
		TPEN.MyIP[0]=0;
		GetPLIP(TPEN.MyNick,TPEN.MyIP);
	};
	if(TPEN.MyRoom&&Active)TPEN.LeaveMyRoom();
	DialogsSystem DSS(0,0);

	LocalGP HFONT("interf3\\Fonts\\FontC10");
	RLCFont hfnt(HFONT.GPID);
	hfnt.SetWhiteColor();

	DSS.SetHintStyle(0,0,0,0,1,0,701,18,1024,1024,&hfnt,0,0);

	DSS.addPicture(NULL,0,0,&Back,&Back,&Back);
	int PLIX=32;
	int PLIY=123;

	Picture* IPINF=DSS.addPicture(NULL,PLIX,PLIY,&IPLInfoBack,&IPLInfoBack,&IPLInfoBack);
	IPINF->Visible=0;
	int GRBX=158-177;
	int GRBY=123-133+8;
	//GPPicture* GERBACK=DSS.addGPPicture(NULL,PLIX+GRBX,PLIY+GRBY,GERB.GPID,0);
	//GERBACK->Visible=0;
	GPPicture* GERBVAL=DSS.addGPPicture(NULL,PLIX+GRBX,PLIY+GRBY,GERB.GPID,1);
	GERBVAL->Visible=0;
	//ListBox* PILB=DSS.addListBox(NULL,PLIX+327,PLIY+36,8,300,26,&YellowFont,&YellowFont,NULL);
	//PILB->Visible=0;
	Canvas* PILB=DSS.AddCanvas(PLIX+337,PLIY+36,394,215);
	PILB->BottomY=0;

	VScrollBar* PIVS=DSS.addNewGP_VScrollBar(NULL,PLIX+745,PLIY+28,226,1,0,SCROL.GPID,0);
	PIVS->Visible=0;
	PILB->VS=PIVS;
	PIVS->ScrDy=100;
	PIVS->OnesDy=32;
	//top100
	Picture* TOPBACK=DSS.addPicture(NULL,PLIX,PLIY,&ITOPBACK,&ITOPBACK,&ITOPBACK);
	TOPBACK->Visible=0;
	TOPBACK->Enabled=0;
	ComplexBox* TOPBOX=DSS.addComplexBox(PLIX+4,PLIY+36,8,26,&DRAW_TOP100,TOPGP.GPID,0);
	TOPBOX->Visible=0;
	TOPBOX->Enabled=0;
	TOPBOX->N=100;
	TOPBOX->OnUserClick=&MMItemChoose;
	TOPBOX->Hint=GetTextByID("INHIN2");
	TOPBOX->UserParam=43;
	
	VScrollBar* TOPVS=DSS.addNewGP_VScrollBar(NULL,PLIX+745,PLIY+28,226,1,0,SCROL.GPID,0);
	TOPVS->Visible=0;
	TOPBOX->VS=TOPVS;
	TOPVS->Visible=0;
	TOPVS->Enabled=0;
	TOPVS->ScrDy=70;
	TOPVS->OnesDy=10;

	byte BCL=79;
	ColoredBar* TOPBTN=DSS.addViewPort2(632-10,613,25,19,BCL);
	TOPBTN->OnUserClick=&MMItemChoose;
	TOPBTN->Hint=GetTextByID("INHIN1");
	TOPBTN->UserParam=10;
	ColoredBar* PLIBTN=DSS.addViewPort2(658-10,613,26,19,BCL);
	PLIBTN->OnUserClick=&MMItemChoose;
	PLIBTN->Hint=GetTextByID("INHIN2");
	PLIBTN->UserParam=11;
	ColoredBar* LETBTN=DSS.addViewPort2(686-10,613,27,19,BCL);
	LETBTN->OnUserClick=&MMItemChoose;
	LETBTN->Hint=GetTextByID("INHIN4");
	LETBTN->UserParam=31;
	ColoredBar* SRVBTN=DSS.addViewPort2(607-10,613,25,19,BCL);
	SRVBTN->OnUserClick=&MMItemChoose;
	SRVBTN->Hint=GetTextByID("INHIN3");
	SRVBTN->UserParam=13;
	GPPicture* KICKPIC=DSS.addGPPicture(NULL,714-10+2,613,SYM3.GPID,0);
	ColoredBar* KICKBTN=DSS.addViewPort2(714-10,613,25,19,BCL);
	KICKBTN->OnUserClick=&MMItemChoose;
	char* MUTE=GetTextByID("INHIN5");
	char* NOMUTE=GetTextByID("INHIN6");
	KICKBTN->Hint=MUTE;
	KICKBTN->UserParam=14;
	if(Active){
		ColoredBar* JOINPL=DSS.addViewPort2(715-10+25,613,23,19,BCL);
		JOINPL->OnUserClick=&MMItemChoose;
		JOINPL->Hint=GetTextByID("JOINPLGM");
		JOINPL->UserParam=15;
	}else{
		DSS.addGPPicture(NULL,715-10+25,613,SYM3.GPID,1);
	};
	CHATSTRING[0]=0;
	ChatViewer* CHVIEW=DSS.addChatViewer(NULL,42,418+5,7,26,462,&ChatMess,&ChatSender,&NCHATS);
	char CHATMESSAGE[256]="";
	InputBox* CHATBOX=DSS.addInputBox(NULL,122,612,CHATMESSAGE,200,392,20,&YellowFont,&WhiteFont);
	CHATBOX->Active=1;
	VScrollBar* CHSCR=DSS.addNewGP_VScrollBar(NULL,505,411,197,1,0,SCROL.GPID,0);
	CHSCR->Visible=0;
	
	KeyPressed=0;
	VScrollBar* RMVS=DSS.addNewGP_VScrollBar(NULL,778,152,193,1,0,SCROL.GPID,0);
	RMVS->ScrDy=6*10;
	RMVS->OnesDy=10;
	ComplexBox* RMLIST=DSS.addComplexBox(37,155+5,7,26,&Draw_RLIST,BTNS.GPID,0);
	RMLIST->param=BTNS.GPID;
	RMLIST->VS=RMVS;

	VScrollBar* PLVS=DSS.addNewGP_VScrollBar(NULL,778,438,197-26,1,0,SCROL.GPID,0);
	PLVS->ScrDy=6*10;
	PLVS->OnesDy=10;
	ComplexBox* PLLIST=DSS.addComplexBox(559,447,6,26,&Draw_PLIST,BTNS.GPID,6);
	PLLIST->param=BTNS.GPID;
	PLLIST->VS=PLVS;
	//PLLIST->OnUserClick=&MMItemChoose;
	//PLLIST->UserParam=31;
	int NL=3;
	if(!Active)NL--;
	int LONE=32;
	int LLX=60;
	int LY1=LONE*NL;
	int xc=916;
	int yc=380;
	//CustomBox* BARX=DSS.addCustomBox(xc-LLX,yc-LY1/2-20,LLX*2,LY1+40,&DRAWBOX);
	//BARX->param=BOR2.GPID;
	GP_Button* CREATE=NULL;
	GP_Button* JOIN=NULL;
	GP_Button* REFRESH=NULL;
	if(!Active){
		CREATE=DSS.addGP_Button(NULL,862,668,IBTN.GPID,7,6);
		CREATE->UserParam=78;
		CREATE->OnUserClick=&MMItemChoose;
		CREATE->Hint=GetTextByID("IG_BACK");
		REFRESH=DSS.addGP_Button(NULL,862,668-100,IBTN.GPID,11,10);
		REFRESH->UserParam=77;
		REFRESH->OnUserClick=&MMItemChoose;
		REFRESH->Hint=GetTextByID("IG_REFRESH");
	}else{
		DSS.addGPPicture(NULL,862,668-100,IBTN.GPID,3);
		JOIN=DSS.addGP_Button(NULL,862,668-100,IBTN.GPID,5,4);
		JOIN->UserParam=mcmJoin;
		JOIN->OnUserClick=&MMItemChoose;
		JOIN->Hint=GetTextByID("IG_JOIN");

		DSS.addGPPicture(NULL,862,668-200,IBTN.GPID,0);
		CREATE=DSS.addGP_Button(NULL,862,668-200,IBTN.GPID,2,1);
		CREATE->UserParam=mcmOk;
		CREATE->OnUserClick=&MMItemChoose;
		CREATE->Hint=GetTextByID("IG_CREATE");

		REFRESH=DSS.addGP_Button(NULL,862,668-300,IBTN.GPID,11,10);
		REFRESH->UserParam=77;
		REFRESH->OnUserClick=&MMItemChoose;
		REFRESH->Hint=GetTextByID("IG_REFRESH");
	};
	//VideoButton* OkBtn=DSS.addVideoButton(NULL,862,568,OK1.GPID,OK2.GPID);
	//OkBtn->Hint=GetTextByID("MOSTART");
	//OkBtn->UserParam=mcmOk;
	//OkBtn->OnUserClick=&MMItemChoose;
	if(Active){
		VideoButton* CancelBtn=DSS.addVideoButton(NULL,862,668,CANCEL1.GPID,CANCEL2.GPID);
		CancelBtn->UserParam=mcmCancel;
		CancelBtn->OnUserClick=&MMItemChoose;
		CancelBtn->Hint=GetTextByID("IG_CANCEL");
	};
	
	//FILTERS
	char* ALLTX=GetTextByID("ALL");
	ComboBox* PLAYERS=DSS.addGP_ComboBox(NULL,398,352,BTNS.GPID,18,9,0,&WhiteFont,&YellowFont,NULL);
	PLAYERS->Hint=GetTextByID("FIL_PLAYERS");
	PLAYERS->FontDx-=4;
	PLAYERS->AddLine(ALLTX);
	PLAYERS->AddLine("1");
	PLAYERS->AddLine("2");
	PLAYERS->AddLine("3");
	PLAYERS->AddLine("4");
	PLAYERS->AddLine("5");
	PLAYERS->AddLine("6");
	PLAYERS->AddLine(">1");
	PLAYERS->AddLine(">2");
	PLAYERS->AddLine(">3");
	PLAYERS->AddLine(">4");
	PLAYERS->AddLine(">5");
	PLAYERS->AddLine(">6");
	ComboBox* PING=DSS.addGP_ComboBox(NULL,456,352,BTNS.GPID,26,9,0,&WhiteFont,&YellowFont,NULL);
	PING->Hint=GetTextByID("FIL_PING");
	PING->FontDx-=4;
	PING->AddLine(ALLTX);
	PING->AddLine("<100");
	PING->AddLine("<200");
	PING->AddLine("<300");
	PING->AddLine("<400");
	PING->AddLine("<500");
	PING->AddLine("<600");
	PING->AddLine("<700");
	PING->AddLine("<800");
	ComboBox* VERSION=DSS.addGP_ComboBox(NULL,524,352,BTNS.GPID,26,9,0,&WhiteFont,&YellowFont,NULL);
	VERSION->Hint=GetTextByID("FIL_VERS");
	char cc[32];
	sprintf(cc,"%s",LobbyVersion);
	VERSION->AddLine(ALLTX);
	VERSION->AddLine(cc);
	VERSION->FontDx-=4;
	ComboBox* TYPE=DSS.addGP_ComboBox(NULL,590,352,BTNS.GPID,34,9,0,&WhiteFont,&YellowFont,NULL);
	TYPE->Hint=GetTextByID("FIL_TYPE");
	TYPE->AddLine(ALLTX);
	TYPE->AddLine(GetTextByID("DEATHTXT"));
	TYPE->AddLine(GetTextByID("BATTLTXT"));
	ComboBox* LEVEL=DSS.addGP_ComboBox(NULL,723,352,BTNS.GPID,18,9,0,&WhiteFont,&YellowFont,NULL);
	LEVEL->Hint=GetTextByID("FIL_LEVEL");
	LEVEL->FontDx-=4;
	LEVEL->OneDx-=4;
	LEVEL->AddLine(ALLTX);
	LEVEL->AddLine(GetTextByID("LEV_1"));
	LEVEL->AddLine(GetTextByID("LEV_2"));
	LEVEL->AddLine(GetTextByID("LEV_3"));
	
	char MASK[64]="";
	GP_Button* CBI[6];
	InputBox* MSIB=DSS.addInputBox(NULL,73,351,MASK,60,300,20,&YellowFont,&WhiteFont);
	MSIB->Hint=GetTextByID("FIL_NAME");
	int ccc=152;
	SimpleDialog* SORT_Name=DSS.addViewPort(70,129,321,19);
	SORT_Name->Hint=GetTextByID("SRT_ROOM");
	SORT_Name->UserParam=1;
	SORT_Name->OnUserClick=&CHANGESORT;
	CBI[0]=DSS.addGP_Button(NULL,69+3,128,BTNS.GPID,51,50);
	SimpleDialog* SORT_Players=DSS.addViewPort(395,128,55,19);
	SORT_Players->Hint=GetTextByID("SRT_PLAYERS");
	SORT_Players->UserParam=2;
	SORT_Players->OnUserClick=&CHANGESORT;
	CBI[1]=DSS.addGP_Button(NULL,395+3,128,BTNS.GPID,55,54);
	SimpleDialog* SORT_Ping=DSS.addViewPort(454,129,63,19);
	SORT_Ping->Hint=GetTextByID("SRT_PING");
	SORT_Ping->UserParam=3;
	SORT_Ping->OnUserClick=&CHANGESORT;
	CBI[2]=DSS.addGP_Button(NULL,453+3,128,BTNS.GPID,53,52);
	SimpleDialog* SORT_Vers=DSS.addViewPort(522,129,62,19);
	SORT_Vers->Hint=GetTextByID("SRT_VERS");
	SORT_Vers->UserParam=4;
	SORT_Vers->OnUserClick=&CHANGESORT;
	CBI[3]=DSS.addGP_Button(NULL,521+3,128,BTNS.GPID,59,58);
	SimpleDialog* SORT_Type=DSS.addViewPort(588,129,200-61,19);
	SORT_Type->Hint=GetTextByID("SRT_TYPE");
	SORT_Type->UserParam=5;
	SORT_Type->OnUserClick=&CHANGESORT;
	CBI[4]=DSS.addGP_Button(NULL,587+3,128,BTNS.GPID,57,56);
	SimpleDialog* SORT_Level=DSS.addViewPort(722,129,55,19);
	SORT_Level->Hint=GetTextByID("SRT_LEVEL");
	SORT_Level->UserParam=6;
	SORT_Level->OnUserClick=&CHANGESORT;
	CBI[5]=DSS.addGP_Button(NULL,723,128,BTNS.GPID,66,65);
	int LOSPR[6]={50,54,52,58,56,65};
	//-------//
	int NCLINES=0;
	char* PNAME="";
	int i;
	ItemChoose=-1;
	TPEN.FILTER.CRC+=10;
	bool FIRST=1;
	char ROOMLISTHINT[512]="";
	char PLLISTHINT[512]="";
	RMLIST->Hint=ROOMLISTHINT;
	PLLIST->Hint=PLLISTHINT;
	TextButton* TBNP=DSS.addTextButton(NULL,785,417,"                        ",&SmallYellowFont,&SmallYellowFont,&SmallYellowFont,0);
	TextButton* TBNG=DSS.addTextButton(NULL,386,132,"                        ",&SmallYellowFont,&SmallYellowFont,&SmallYellowFont,0);
	char* tx_NPL=GetTextByID("NPLAYMSS");
	char* tx_NRM=GetTextByID("NROOMMSS");
	int PLINFMOD=0;
	InternetStatsChunks ISC;
	ISC.Clear();
	GPGetInfoResponseArg GPI;
	if(Active){
		RunHTTPC();
	};
	char CURRNICK[128];
	CURRNICK[0]=0;
	int CurLBP=PLAYERS->CurLine;
	bool DOPASS=0;
	if(Active&&CheckLogin()){
		SendPlayerRequest();
	};
	do{
		if(CURRNICK[0]&&CurLBP==PLLIST->CurrentItem&&CurLBP!=-1){
			int cp=SearchPlayer(CURRNICK);
			if(cp!=-1)PLLIST->CurrentItem=cp;
		};
		if(PLLIST->CurrentItem>=0){
			int v=PLLIST->CurrentItem;
			if(v<TPEN.NPlayers){
				strcpy(CURRNICK,TPEN.Players[v].Name);
				CurLBP=v;
			}else{
				v-=TPEN.NPlayers;	
				if(v<TPEN.ABPL.NPlayers){
					strcpy(CURRNICK,TPEN.ABPL.Names[v]);
					CurLBP=v+TPEN.NPlayers;
				};
			};
		};
		
		if(PLLIST->CurrentItem>=0){
			if(PLLIST->CurrentItem<TPEN.NPlayers){
				if(TPEN.Players[PLLIST->CurrentItem].Muted){
					KICKBTN->Hint=NOMUTE;
					KICKPIC->Visible=0;
				}else{
					KICKBTN->Hint=MUTE;
					KICKPIC->Visible=1;
				};
			}else KICKBTN->Hint=MUTE;
		}else KICKBTN->Hint=MUTE;
		if(PLINFMOD==1||PLINFMOD==2){
			for(int p=0;p<6;p++){
				CBI[p]->Enabled=0;
				CBI[p]->Visible=0;
			};
			PLAYERS->Visible=0;
			PING->Visible=0;
			VERSION->Visible=0;
			TYPE->Visible=0;
			LEVEL->Visible=0;

			SORT_Name->Visible=0;
			SORT_Players->Visible=0;
			SORT_Ping->Visible=0;
			SORT_Vers->Visible=0;
			SORT_Type->Visible=0;
			SORT_Level->Visible=0;
			MSIB->Enabled=0;
			MSIB->Visible=0;

			RMLIST->Visible=0;
			RMLIST->Enabled=0;
			RMVS->Visible=0;
			RMVS->Enabled=0;

			IPINF->Visible=1;
			IPINF->Enabled=1;
			//GERBACK->Visible=1;
			//GERBACK->Enabled=1;
			GERBVAL->Visible=1;
			GERBVAL->Enabled=1;
			PILB->Visible=1;
			PILB->Enabled=1;

			TOPBACK->Visible=0;
			TOPBOX->Visible=0;
			TOPVS->Visible=0;
			TOPBACK->Enabled=0;
			TOPBOX->Enabled=0;
			TOPVS->Enabled=0;
			//PIVS->Visible=PILB->NItems>PILB->ny;
			TBNG->Visible=0;
			if(PLINFMOD==1){
				if(GPI.result!=GP_NO_ERROR){
					PLINFMOD=2;
					int y0=PILB->BottomY;
					PILB->AddText(16,y0,GetTextByID("PIF_UNACC"),&RedFont);
					PILB->BottomY+=18;
				};
				if(GPI.nick[0]&&GPI.result==GP_NO_ERROR){
					PLINFMOD=2;
					byte cc=0x7C;
					int y0=PILB->BottomY;
					int tdy=5;
					int msx1=120;
					char ccc[256];

					PILB->AddRect(0,y0,393,27,cc);
					PILB->AddLine(msx1,y0,msx1,y0+26,cc);
					PILB->AddText(16,y0+tdy,GetTextByID("PIF_ST0"),&YellowFont);
					GETCOUNTRY(GPI.countrycode,ccc);
					PILB->AddText(msx1+16,y0+tdy,ccc,&YellowFont);	
					y0+=26;

					PILB->AddRect(0,y0,393,27,cc);
					PILB->AddLine(msx1,y0,msx1,y0+26,cc);
					PILB->AddText(16,y0+tdy,GetTextByID("PIF_ST1"),&YellowFont);
					sprintf(ccc,"%d.%d.%d",GPI.birthday,GPI.birthmonth,GPI.birthyear);
					PILB->AddText(msx1+16,y0+tdy,ccc,&YellowFont);	
					y0+=26;

					PILB->AddRect(0,y0,393,27,cc);
					PILB->AddLine(msx1,y0,msx1,y0+26,cc);
					PILB->AddText(16,y0+tdy,GetTextByID("PIF_ST2"),&YellowFont);
					switch(GPI.sex){
					case GP_MALE:
						PILB->AddText(msx1+16,y0+tdy,GetTextByID("GEN_M"),&YellowFont);	
						break;
					case GP_FEMALE:
						PILB->AddText(msx1+16,y0+tdy,GetTextByID("GEN_F"),&YellowFont);	
						break;
					default:
						PILB->AddText(msx1+16,y0+tdy,GetTextByID("GEN_U"),&YellowFont);	
						break;
					};
					y0+=26;

					PILB->AddRect(0,y0,393,27,cc);
					PILB->AddLine(msx1,y0,msx1,y0+26,cc);
					PILB->AddText(16,y0+tdy,GetTextByID("PIF_ST3"),&YellowFont);
					PILB->AddText(msx1+16,y0+tdy,GPI.email,&YellowFont);	
					y0+=26;

					if(GPI.icquin){
						PILB->AddRect(0,y0,393,27,cc);
						PILB->AddLine(msx1,y0,msx1,y0+26,cc);
						PILB->AddText(16,y0+tdy,GetTextByID("ICQ#"),&YellowFont);
						sprintf(ccc,"%d",GPI.icquin);
						PILB->AddText(msx1+16,y0+tdy,ccc,&YellowFont);	
						y0+=26;
					};

					if(GPI.homepage[0]){
						PILB->AddRect(0,y0,393,27,cc);
						PILB->AddLine(msx1,y0,msx1,y0+26,cc);
						PILB->AddText(16,y0+tdy,GetTextByID("PIF_ST5"),&YellowFont);
						PILB->AddText(msx1+16,y0+tdy,GPI.homepage,&YellowFont);	
						y0+=26;
					};
					//y0+=16;
					PILB->BottomY=y0;
					if(DOPASS)ISC.StartDownload(GPI.profile,1);
					else ISC.StartDownload(GPI.profile,0);
				};
			};
			if(ISC.CDOWN.Started&&(ISC.CDOWN.Finished||ISC.Error)){
				if(ISC.CDOWN.CurPage==1&&!ISC.CDOWN.Error){
					char res[128],rrr[128];
					DecodeGS_Password(res,(char*)ISC.Chunks[0]->Data);
					sprintf(rrr,"password: %s",res);
					PILB->AddText(0,PILB->BottomY,rrr,&YellowFont);
					PILB->BottomY+=24;
					ISC.Clear();
					ISC.StartDownload(GPI.profile,0);
				};
				if(ISC.CDOWN.CurPage==0&&!ISC.CDOWN.Error){
					int NREC=0;
					int CURSCORE=0;
					int NINCOMP=0;
					for(int i=0;i<ISC.N;i++){
						if(ISC.Chunks[i]->Index==0||ISC.Chunks[i]->Index==27){
							int ProfileID=ISC.CDOWN.CurProfile;
							int w1=*((DWORD*)ISC.Chunks[i]->Data);
							int w2=*((DWORD*)(ISC.Chunks[i]->Data+4));
							int w3=*((DWORD*)(ISC.Chunks[i]->Data+8));
							
							int sco1=w1^DWORD(ProfileID);
							int sco2=w2^0xFB47E6C3;
							if(sco1==sco2){
								NREC=w3;
								CURSCORE=sco1;
							};
						};
						if(ISC.Chunks[i]->Index==28){
							int ProfileID=ISC.CDOWN.CurProfile;
							int w1=*((DWORD*)ISC.Chunks[i]->Data);
							int w2=*((DWORD*)(ISC.Chunks[i]->Data+4));
							int w3=*((DWORD*)(ISC.Chunks[i]->Data+8));
							int w5=*((DWORD*)(ISC.Chunks[i]->Data+16));
							int w6=*((DWORD*)(ISC.Chunks[i]->Data+20));
							int sco1=w1^DWORD(ProfileID);
							int sco2=w2^0xFB47E6C3;
							int incomp1=(w5+DWORD(ProfileID))^(0x6523A4+DWORD(ProfileID));
							int incomp2=(w6+DWORD(ProfileID))^(0x1234E6-DWORD(ProfileID));

							if(sco1==sco2&&incomp1==incomp2){
								NREC=w3;
								CURSCORE=sco1;
								NINCOMP=incomp1;
							};
						};
					};
					ISC.Clear();
					ISC.CDOWN.Started=0;
					if(NREC){
						ISC.StartDownload(ISC.CDOWN.CurProfile,2+NREC);
					};
					char ccc[128];
					char cc2[36];
					sprintf(cc2,"RS_RANK_%d",GetRankByScore(CURSCORE));
					sprintf(ccc,GetTextByID("SCORANK"),CURSCORE,GetTextByID(cc2));
					PILB->AddText(0,PILB->BottomY,ccc,&YellowFont);
					PILB->BottomY+=24;
					if(NREC){
						sprintf(ccc,GetTextByID("TX_INCOMP"),NINCOMP,NREC+NINCOMP,(NINCOMP*100)/(NREC+NINCOMP));
						PILB->AddText(0,PILB->BottomY,ccc,&YellowFont);
						PILB->BottomY+=24;
					};
					GERBVAL->SpriteID=GetGerbByScore(CURSCORE)+1;
					GERBVAL->Visible=1;
				}else if(ISC.CDOWN.CurPage>1){
					if(!ISC.CDOWN.Error){
						int TextDy=5;
						int XB0=0;
						int XB1=18;
						int XB2=XB1+18;
						int XB3=XB2+123;
						int XB4=XB3+64;
						int XB5=XB4+90;
						int XB6=XB5+80;
						byte COL=0x7C;
						for(int j=ISC.N-1;j>=0;j--){
							char ccc[128];
							char MapName[128];
							char GameName[128];
							if(ISC.Chunks[j]->Index==81){
								byte* data=ISC.Chunks[j]->Data;
								int pos=0;
								DWORD Date=*((DWORD*)data);
								int Min=Date%60;
								Date/=60;
								int Hour=Date%24;
								Date/=24;
								int Day=(Date%31)+1;
								Date/=31;
								int Month=(Date%12)+1;
								Date/=12;
								int Year=2000+Date;
								int Npl=data[4];
								word PTime=*((word*)(data+5));
								int DScore=data[8];
								int LMap=data[9];
								memcpy(MapName,data+10,LMap);
								MapName[LMap]=0;
								int LGame=data[10+LMap];
								memcpy(GameName,data+11+LMap,LGame);
								GameName[LGame]=0;		
								int Y0=PILB->BottomY;		

								RLCFont* FNT=&YellowFont;
								PILB->AddRect(XB0,Y0,393,27,COL);
								PILB->AddRect(XB0+1,Y0+1,393-2,27-2,COL);
								int xx3=120;
								PILB->AddLine(xx3,Y0,xx3,Y0+26,COL);
								sprintf(ccc,"%d.%d.%d %d:%d%d",Day,Month,Year,Hour,Min/10,Min%10);
								PILB->AddCText((xx3+XB0)>>1,Y0+TextDy,ccc,FNT);
								sprintf(ccc,"%s (%d min)",GameName,PTime);
								PILB->AddText(xx3+16,Y0+TextDy,ccc,FNT);
								Y0+=26;
								/*
								sprintf(ccc,"Map: %s",MapName);
								PILB->AddRect(XB0,Y0,393,27,COL);
								PILB->AddText(0,Y0,ccc,&YellowFont);
								Y0+=26;
								*/

								pos=11+LMap+LGame;
								//PILB->AddItem("Players:",0);
								int TeamID=0;
								byte pmask=0;	

								byte Popul[8][32];
								byte SCORE[8][32];
								word MaxPopul[8];
								word MaxScore[8];
								byte ColorN[8];

								for(int i=0;i<Npl;i++){

									PILB->AddRect(XB0,Y0,393,27,COL);
									PILB->AddLine(XB1,Y0,XB1,Y0+26,COL);
									PILB->AddLine(XB2,Y0,XB2,Y0+26,COL);
									PILB->AddLine(XB3,Y0,XB3,Y0+26,COL);
									PILB->AddLine(XB4,Y0,XB4,Y0+26,COL);
									PILB->AddLine(XB5,Y0,XB5,Y0+26,COL);

									int LNick=data[pos+6+7];
									memcpy(MapName,data+pos+7+7,LNick);
									MapName[LNick]=0;
									DWORD Score=*((DWORD*)(data+pos+2));
									byte Mask=data[pos+6];
									if(!(Mask&pmask)){
										pmask=Mask;
										TeamID++;
									};
									byte Color=data[pos+7];
									ColorN[i]=Color;
									byte NationID=data[pos+8];
									//sprintf(ccc,"%s (%.2f), ",MapName,float(Score)/100);
									sprintf(ccc,"%d",TeamID);
									int xc=(XB1+XB2)>>1;
									PILB->AddBar(xc-4,Y0+13-4,8,8,0xD0+Color*4);
									PILB->AddCText((XB0+XB1)>>1,Y0+TextDy,ccc,FNT);
									PILB->AddText(XB2+5,Y0+TextDy,MapName,FNT);
									sprintf(ccc,"%.2f",float(Score)/100);
									PILB->AddCText((XB3+XB4)>>1,Y0+TextDy,ccc,FNT);
									if(NationID<NNations)PILB->AddCText((XB4+XB5)>>1,Y0+TextDy,NatNames[NationID],FNT);
									ccc[0]=0;
									switch(data[pos+1]){
									case 1://defeat
										strcat(ccc,"Defeat.");
										break;
									case 0://victory
										strcat(ccc,"Victory!");
										break;
									case 3://disconnectted
										strcat(ccc,"???");
										break;
									default:
										strcat(ccc,"?unknown?");
										break;
									};
									pos+=7+7+LNick;
									PILB->AddCText((XB5+XB6)>>1,Y0+TextDy,ccc,FNT);
									Y0+=26;
									MaxScore[i]=word(*((DWORD*)(data+pos)));
									MaxPopul[i]=word(*((DWORD*)(data+pos+2)));
									pos+=4;
									memcpy(SCORE[i],data+pos,32);
									pos+=32;
									memcpy(Popul[i],data+pos,32);
									pos+=32;
								};

								word CSCOR=*((word*)(data+pos));
								char dscor=data[pos+2];
								//pos+3;

								Y0+=DrawGraph(PILB,Y0,32,SCORE,ColorN,Npl,MaxScore,GetTextByID("HDR_SCORE:"));
								Y0+=DrawGraph(PILB,Y0,32,Popul,ColorN,Npl,MaxPopul,GetTextByID("HDR_POPUL:"));
								sprintf(ccc,GetTextByID("HDR_DSCORE"),CSCOR,dscor);
								PILB->AddText(0,Y0,ccc,FNT);
								Y0+=26;
								Y0+=16;
								PILB->BottomY=Y0;
							};
						};
					};
					ISC.Clear();
					if(ISC.CDOWN.CurPage>2){
						ISC.StartDownload(ISC.CDOWN.CurProfile,ISC.CDOWN.CurPage-1);
					};
				};
			};
			if(ISC.Error)PLINFMOD=0;
			if(!GERBVAL->SpriteID)GERBVAL->Visible=0;
		}else if(PLINFMOD==3){
			TOPBACK->Visible=1;
			TOPBOX->Visible=1;
			TOPBACK->Enabled=1;
			TOPBOX->Enabled=1;
			MSIB->Enabled=0;
			MSIB->Visible=0;
			RMLIST->Visible=0;
			RMLIST->Enabled=0;

			IPINF->Visible=0;
			IPINF->Enabled=0;
			GERBVAL->Visible=0;
			GERBVAL->Enabled=0;
			PILB->Visible=0;
			PILB->Enabled=0;
			PIVS->Visible=0;
			RMVS->Visible=0;
			RMVS->Enabled=0;
			for(int p=0;p<6;p++){
				CBI[p]->Enabled=0;
				CBI[p]->Visible=0;
			};
			PLAYERS->Visible=0;
			PING->Visible=0;
			VERSION->Visible=0;
			TYPE->Visible=0;
			LEVEL->Visible=0;
			TBNG->Visible=0;

			SORT_Name->Visible=0;
			SORT_Players->Visible=0;
			SORT_Ping->Visible=0;
			SORT_Vers->Visible=0;
			SORT_Type->Visible=0;
			SORT_Level->Visible=0;

			TOPBACK->Visible=1;
			TOPBACK->Enabled=1;
			TOPBOX->Visible=1;
			TOPBOX->Enabled=1;
		}else{
			RMLIST->Visible=1;
			RMLIST->Enabled=1;

			MSIB->Enabled=1;
			MSIB->Visible=1;

			IPINF->Visible=0;
			IPINF->Enabled=0;
			//GERBACK->Visible=0;
			//GERBACK->Enabled=0;
			GERBVAL->Visible=0;
			GERBVAL->Enabled=0;
			PILB->Visible=0;
			PILB->Enabled=0;
			PIVS->Visible=0;
			for(int p=0;p<6;p++){
				CBI[p]->Enabled=1;
				CBI[p]->Visible=1;
			};
			PLAYERS->Visible=1;
			PING->Visible=1;
			VERSION->Visible=1;
			TYPE->Visible=1;
			LEVEL->Visible=1;
			TBNG->Visible=1;

			SORT_Name->Visible=1;
			SORT_Players->Visible=1;
			SORT_Ping->Visible=1;
			SORT_Vers->Visible=1;
			SORT_Type->Visible=1;
			SORT_Level->Visible=1;

			TOPBACK->Visible=0;
			TOPBOX->Visible=0;
			TOPVS->Visible=0;
			TOPBACK->Enabled=0;
			TOPBOX->Enabled=0;
			TOPVS->Enabled=0;
		};
		if(UnderFirewall){
			DisableCreate=NotifyFirewallState();
			UnderFirewall=0;
		};
		sprintf(TBNP->Message,tx_NPL,TPEN.NPlayers+TPEN.ABPL.NPlayers);
		sprintf(TBNG->Message,tx_NRM,RMLIST->N);
		TBNP->x=785-GetRLCStrWidth(TBNP->Message,&YellowFont);
		TBNG->x=386-GetRLCStrWidth(TBNG->Message,&YellowFont);
		if(!Active){
			PIEnumeratePlayers(GSets.CGame.PL_INFO,false);
			SendPings();
			if(NPlayers>7)NPlayers=7;
		};
		for(i=0;i<6;i++){
			if(abs(TPEN.SORTTYPE)==i+1){
				CBI[i]->PassiveFrame=LOSPR[i];
			}else{
				CBI[i]->PassiveFrame=1000;
			}
			//CBI[i]->Visible=abs(TPEN.SORTTYPE)==i+1;
			//CBI[i]->Enabled=abs(TPEN.SORTTYPE)==i+1;
			//CBI[i]->Style=1;
		};
		if(RMLIST->M_OvrItem>=0){
			CreateRoomsHintString(RMLIST->M_OvrItem,ROOMLISTHINT,Active);
		};
		if(PLLIST->M_OvrItem>=0){
			CreatePlayerHintString(PLLIST->M_OvrItem,PLLISTHINT);
		};
		TPEN.FILTER.CHK_Ver=VERSION->CurLine!=0;
		TPEN.FILTER.CHK_Ping=PING->CurLine!=0;
		TPEN.FILTER.MinPing=PING->CurLine*100;
		TPEN.FILTER.CHK_Type=TYPE->CurLine;
		TPEN.FILTER.CHK_Level=LEVEL->CurLine;
		strcpy(TPEN.FILTER.MASKSTR,MASK);
		if(PLAYERS->CurLine){
			if(PLAYERS->CurLine<7){
				TPEN.FILTER.CHK_Players=1;
				TPEN.FILTER.CHK_MinPlayers=0;
				TPEN.FILTER.NPlayers=PLAYERS->CurLine;
			}else{ 
				TPEN.FILTER.CHK_Players=0;
				TPEN.FILTER.CHK_MinPlayers=1;
				TPEN.FILTER.NPlayers=PLAYERS->CurLine-6;
			};
		}else{
			TPEN.FILTER.CHK_Players=0;
			TPEN.FILTER.CHK_MinPlayers=0;
		};
		TPEN.ProcessRoomsInGame();
		PLLIST->N=TPEN.NPlayers+TPEN.ABPL.NPlayers;//TPEN.NRoomPlayers;
		GServer CSRV=NULL;
		if(RMLIST->CurrentItem!=-1&&RMLIST->CurrentItem<TPEN.NFRooms&&TPEN.FiltRef[RMLIST->CurrentItem]<1000000){
			CSRV=TPEN.Room[TPEN.FiltRef[RMLIST->CurrentItem]].G;
		};
		TPEN.ProcessFilter();
		RMLIST->CurrentItem=-1;
		for(i=0;i<TPEN.NFRooms;i++)if(TPEN.FiltRef[i]<1000000)if(TPEN.Room[TPEN.FiltRef[i]].G==CSRV)
			RMLIST->CurrentItem=i;
		if(PLLIST->CurrentItem>=PLLIST->N)PLLIST->CurrentItem=PLLIST->N-1;
		RMLIST->N=TPEN.NFRooms;//+TPEN.NRInGame;
		if(RMLIST->FirstVisItem+RMLIST->NOnScr>=RMLIST->N)
			RMLIST->FirstVisItem=RMLIST->N-RMLIST->NOnScr;
		if(RMLIST->FirstVisItem<0)RMLIST->FirstVisItem=0;
		if(RMLIST->CurrentItem>=RMLIST->N)RMLIST->CurrentItem=RMLIST->N-1;
		int NCL=7;
		bool AddChat=FIRST;
		FIRST=0;
		if(TPEN.GlobalChat.NCStr){
			for(int j=0;j<TPEN.GlobalChat.NCStr;j++){
				AddChatString(
					TPEN.GlobalChat.CStr[j].PlName,
					TPEN.GlobalChat.CStr[j].Message,430,
					&WhiteFont,ChatMess,ChatSender,NCHATS,MAXCHATS);
				AddChat=1;
			};
			TPEN.GlobalChat.Clear();
		};
		PNAME="";
		if(NCHATS>NCL){
			CHSCR->SMaxPos=NCHATS-NCL;
			if(AddChat)CHSCR->SPos=CHSCR->SMaxPos;
			CHSCR->Visible=1;
			CHVIEW->ChatDY=CHSCR->SPos;
		}else{
			CHSCR->Visible=0;
			CHSCR->SPos=0;
			CHVIEW->ChatDY=0;
		};
		ProcessMessages();
		ISC.ProcessDownload();
		T100.Process();
		if(!TPEN.Connected){
			DarkScreen();
			ShowCentralMessage(GetTextByID("ICCONN"),BOR2.GPID);
			FlipPages();
			TPEN.Connect(PlName);
			bool NeedDraw=1;
			do{
				ProcessMessages();
				if(bActive){
					if(NeedDraw)FlipPages();
					NeedDraw=0;
				}else NeedDraw=1;
				if(GetTickCount()-TPEN.ConnStartTime>60000){
					TPEN.ErrorType=1;
				};
			}while(!(TPEN.Connected||TPEN.ErrorType));
			if(TPEN.ErrorType){
				Back.Draw(0,0);
				DarkScreen();
				ShowCentralMessage(GetTextByID("ICUNCON"),BOR2.GPID);
				FlipPages();
				bool NeedDraw=1;
				KeyPressed=0;
				int t0=GetTickCount();
				do{
					ProcessMessages();
					if(bActive){
						if(NeedDraw)FlipPages();
						NeedDraw=0;
					}else NeedDraw=1;
				}while((GetTickCount()-t0<20000)&&!KeyPressed);
				KeyPressed=0;
				peerShutdown(TPEN.Peer);
				TPEN.Peer=NULL;
				return false;
			};
			TPEN.MyIP[0]=0;
			GetPLIP(TPEN.MyNick,TPEN.MyIP);
		};
		if(KeyPressed&&LastKey==13){
			KeyPressed=0;
			if(CHATMESSAGE[0]){
				TPEN.SendGlobalChat(CHATMESSAGE);
				CHATMESSAGE[0]=0;
			};
		};
		if(GameInProgress)return 0;
		DSS.MarkToDraw();
		DSS.ProcessDialogs();
		DSS.RefreshView();

		if(RMLIST->CurrentItem>=0&&TPEN.FiltRef&&(!DisableCreate)
			&&TPEN.FiltRef[RMLIST->CurrentItem]<1000000
			&&TPEN.FiltRef[RMLIST->CurrentItem]<TPEN.NRooms
			&&RMLIST->CurrentItem<TPEN.NFRooms
			&&CheckJoin(TPEN.Room[TPEN.FiltRef[RMLIST->CurrentItem]].G)){
				if(JOIN){
					JOIN->Visible=1;
					JOIN->Enabled=1;
				};
			}else{
				if(JOIN){
					JOIN->Visible=0;
					JOIN->Enabled=0;
				};
			};
			if(CREATE){
				CREATE->Visible=!DisableCreate;
				CREATE->Enabled=!DisableCreate;
			}
			if(ItemChoose==mcmJoin){
				if(RMLIST->CurrentItem>=0&&TPEN.FiltRef[RMLIST->CurrentItem]<1000000&&RMLIST->CurrentItem<TPEN.NFRooms
					&&CheckJoin(TPEN.Room[TPEN.FiltRef[RMLIST->CurrentItem]].G)){
						char* gg=ServerGetStringValue(TPEN.Room[TPEN.FiltRef[RMLIST->CurrentItem]].G,"password","0");
						bool okk=0;
						if(gg[0]=='1'){
							okk=EnterPassword();
							KeyPressed=0;
						}else{
							SessPassword[0]=0;
							okk=1;
						};
						if(okk){
							DarkScreen();
							ShowCentralMessage(GetTextByID("ICJOIN"),BOR2.GPID);
							FlipPages();
							strcpy(TPEN.HostMessage,TPEN.Room[TPEN.FiltRef[RMLIST->CurrentItem]].Name);
							strcpy(ROOMNAMETOCONNECT,TPEN.Room[TPEN.FiltRef[RMLIST->CurrentItem]].Name);
							TPEN.JoinRoom(TPEN.Room+TPEN.FiltRef[RMLIST->CurrentItem],SessPassword);
							if(!TPEN.MyRoom){
								//Unable to connect.
								ItemChoose=-1;
							}else{
								if(SessPassword[0]=='1'){
									do{
										ProcessMessages();
									}while(TPEN.MyRoom&&!TPEN.MyRoom->RoomConnected);
									if(TPEN.MyRoom&&TPEN.MyRoom->RoomConnected){
										strcpy(IPADDR,ServerGetAddress(TPEN.Room[TPEN.FiltRef[RMLIST->CurrentItem]].G));
										return 2;
									}else{
										WaitWithMessage(GetTextByID("ICUNJ"));
										IPADDR[0]=0;
									};
								}else{
									strcpy(IPADDR,ServerGetAddress(TPEN.Room[TPEN.FiltRef[RMLIST->CurrentItem]].G));
									return 2;
								};
							};
						};
					};
				ItemChoose=-1;
			}else
				if(ItemChoose==mcmOk){
					if(CreateSessionDialog()){
						DarkScreen();
						ShowCentralMessage(GetTextByID("ICCREATE"),BOR2.GPID);
						FlipPages();
						if(GMMOD&1)TPEN.CreateStagingRoom(SessionName,SessPassword,2,GMMOD,GMLEV);
						else TPEN.CreateStagingRoom(SessionName,SessPassword,GMMAXPL,GMMOD,GMLEV);
						return 1+GMMOD*2;
					}else{
						ItemChoose=-1;
					};
				}else if(ItemChoose==77){
					ItemChoose=-1;
					TPEN.RefreshSessions();
				}else if(ItemChoose==31){
					ItemChoose=-1;
					//if(PLLIST->CurrentItem>=0&&PLLIST->CurrentItem<TPEN.NPlayers){
					//	EnterPersonalMessage(TPEN.Players[PLLIST->CurrentItem].Name);
					//};
					if(PLLIST->CurrentItem>=0){
						if(PLLIST->CurrentItem<TPEN.NPlayers){
							EnterPersonalMessage(TPEN.Players[PLLIST->CurrentItem].Name);
						}else{
							if(PLLIST->CurrentItem-TPEN.NPlayers<TPEN.ABPL.NPlayers){
								EnterPersonalMessage(TPEN.ABPL.Names[PLLIST->CurrentItem-TPEN.NPlayers]);
							};
						};
					};
					KeyPressed=0;
					LastKey=0;
				}else if(ItemChoose==10){//top100
					if(CheckLogin()){
						//if(!T100_cmd_done){
						T100.Clear();
						T100.Download();
						T100.Started=1;
						//	T100_cmd_done=1;
						//};
						TOPVS->SPos=0;
						PLINFMOD=3;
					};
					ItemChoose=-1;
				}else if(ItemChoose==11){//player info
					if(PLLIST->CurrentItem>=0&&CheckLogin()){
						if(PLLIST->CurrentItem<TPEN.NPlayers){
							PILB->L=0;
							PILB->BottomY=0;
							if(TPEN.Players[PLLIST->CurrentItem].ProfileID){
								//ISC.StartDownload(TPEN.Players[PLLIST->CurrentItem].ProfileID,0);
								ISC.CDOWN.Started=0;
								ISC.Error=0;
								ISC.Clear();
								PLINFMOD=1;
								char* Name=TPEN.Players[PLLIST->CurrentItem].Name;
								PILB->AddText(2,PILB->BottomY+2,Name,&BigRedFont);
								PILB->AddText(0,PILB->BottomY,Name,&BigYellowFont);
								PILB->BottomY+=32;
								memset(&GPI,0,sizeof GPI);
								GPResult Res=gpGetInfo(LOGIN_gp,TPEN.Players[PLLIST->CurrentItem].ProfileID,GP_DONT_CHECK_CACHE,GP_NON_BLOCKING,&GPI_callback3,&GPI);
								PIVS->SPos=0;
							};
						};
					};
					ItemChoose=-1;
					GERBVAL->SpriteID=0;
				}else if(ItemChoose==43){//t100 click
					int Index=TOPBOX->CurrentItem;
					if(Index<100&&T100.CLIENT[Index].PLINF){
						int prof=T100.CLIENT[Index].PLINF->profile;
						if(prof){
							PILB->L=0;
							PILB->BottomY=0;
							ISC.CDOWN.Started=0;
							ISC.Error=0;
							ISC.Clear();
							PLINFMOD=1;
							char* Name=T100.CLIENT[Index].PLINF->nick;
							PILB->AddText(2,PILB->BottomY+2,Name,&BigRedFont);
							PILB->AddText(0,PILB->BottomY,Name,&BigYellowFont);
							PILB->BottomY+=32;
							memset(&GPI,0,sizeof GPI);
							GPResult Res=gpGetInfo(LOGIN_gp,prof,GP_DONT_CHECK_CACHE,GP_NON_BLOCKING,&GPI_callback3,&GPI);
							PIVS->SPos=0;
						};
						GERBVAL->SpriteID=0;
					};
					ItemChoose=-1;
				}else if(ItemChoose==13){//servers list
					PLINFMOD=0;
					ItemChoose=-1;
				}else if(ItemChoose==14){
					if(PLLIST->CurrentItem>=0){
						if(PLLIST->CurrentItem<TPEN.NPlayers){
							TPEN.Players[PLLIST->CurrentItem].Muted=!TPEN.Players[PLLIST->CurrentItem].Muted;
						};
					};
					ItemChoose=-1;
				}else if(ItemChoose==15){
					if(PLLIST->CurrentItem<TPEN.NPlayers){
						if(TryToJoinToPlayer(TPEN.Players[PLLIST->CurrentItem].Name)){
							return 2;
						}else ItemChoose=-1;
					};

				};
				if(RejectThisPlayer||CheckPlayerToExit())ItemChoose=mcmCancel;
	}while(ItemChoose==-1&&!GameInProgress);
	if(Active&&TPEN.Connected){
		TPEN.Disconnect();
	};
	return 0;
};
CEXPORT
void SendPings();
bool CheckPingsReady();
int GetAveragePing();
extern int CurStatus;
void ReceiveAll();
void CreateDiffStr(_str& str);
extern int NPings;
int GetMaxRealPing();
extern int CurrentMaxPing[8];
int PPTIME=0;
bool SendToAllPlayers(DWORD Size,LPVOID lpData);
int GetMapSUMM(char* Name);
int GetReadyPercent();
bool CheckExistConn();
void PrintBadConn(char* str);
char* _engNOCONN="No direct connection established with:";
byte MPL_NatRefTBL[8]={0,1,2,3,4,5,6,7};
void SetStdTBL(){
	for(int i=0;i<8;i++)MPL_NatRefTBL[i]=i;
};
int GetLogRank();
void SETPLAYERDATA(DWORD ID,void* Data,int size,bool);
void SETPLAYERNAME(DPNAME* lpdpName,bool);
extern DPID ServerDPID;
void ClearLPACK();
bool GetPreviewName(char* Name,char* ccc);
void DeepDeletePeer(DWORD ID);
int GetMyProfile();

int SetCurrentBattle(int BattleID,
					 SQPicture* Prev,
					 ComboBox** MNATION,
					 TextViewer* BTTXT){
						 if(BattleID<0)return 0;
						 if(BattleID>=WARS.NBattles)return WARS.NBattles-1;
						 Prev->LoadPicture(WARS.Battles[BattleID].MiniMap);
						 for(int i=0;i<2;i++){
							 int cl=MNATION[i]->CurLine;
							 MNATION[i]->Clear();
							 MNATION[i]->AddLine(WARS.Battles[BattleID].RedUnits);
							 MNATION[i]->AddLine(WARS.Battles[BattleID].BlueUnits);
							 MNATION[i]->CurLine=cl;
						 };
						 ResFile FF1=RReset(WARS.Battles[BattleID].Brief);
						 if(FF1!=INVALID_HANDLE_VALUE){
							 ResFile FF2=RRewrite("miss.txt");
							 if(FF2!=INVALID_HANDLE_VALUE){
								 RBlockWrite(FF2,WARS.Battles[BattleID].BigHeader,strlen(WARS.Battles[BattleID].BigHeader));
								 char c=0x0D;
								 RBlockWrite(FF2,&c,1);
								 c=0x0A;
								 RBlockWrite(FF2,&c,1);
								 c=0x0D;
								 RBlockWrite(FF2,&c,1);
								 c=0x0A;
								 RBlockWrite(FF2,&c,1);
								 byte* Data=znew(byte,RFileSize(FF1));
								 RBlockRead(FF1,Data,RFileSize(FF1));
								 RBlockWrite(FF2,Data,RFileSize(FF1));
								 RClose(FF2);
							 };
							 RClose(FF1);
						 };
						 BTTXT->LoadFile("miss.txt");
						 return BattleID;
					 };
void CreateNationalMaskForRandomMap(char*);
void CreateMaskForSaveFile(char*);
void CreateNationalMaskForMap(char*);
int DetermineNationAI(byte Nat){
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&OB->NNUM==Nat&&OB->newMons->Peasant&&!OB->Sdoxlo){
			char* name=OB->Ref.General->MonsterID;
			for(int j=0;j<GlobalAI.NAi;j++){
				if(!strcmp(GlobalAI.Ai[j].Peasant_ID,name))return j;
			};
		};
	};
	return -1;
};
bool GetPreview(char* Name,byte* Data);
bool RunEnciclopia(SimpleDialog* SD){
	WinExec("HView.exe",SW_NORMAL);
	/*
	char cc[256];
	GetWindowsDirectory(cc,256);
	char cc1[300];
	sprintf(cc1,"%s\\command\\start.exe",cc);
	GFILE* f=Gopen("Info\\index.htm","r");
	if(f){
	Gclose(f);
	char cc2[300];
	sprintf(cc2,"%s /max Info\\index.htm",cc1);
	WinExec(cc2,SW_MINIMIZE);
	return 0;
	};
	for(int i='d';i<='z';i++){
	char cd[2]="a";
	char cc3[300];
	cd[0]=i;
	sprintf(cc3,"%s:\\GSC_Info\\index.htm",cd);
	GFILE* f=Gopen(cc3,"r");
	if(f){
	Gclose(f);
	char cc2[300];
	sprintf(cc2,"%s /max %s",cc1,cc3);
	WinExec(cc2,SW_MINIMIZE);
	return 0;
	};

	};
	*/
	return 0;
};
extern int RunMethod;

int ProcessWars();
void processBattleMultiplayer();
/*
Accept      - 153x239
Cancel      - 304x239
Low_Speed   - 224x120
High_Speed  - 397x120
Low_Scrol   - 224x151
High_Scrol  - 397x151
Low_Music   - 224x182
High_Music  - 397x182
Low_Sound   - 224x214
High_Sound  - 397x214

???????:
Low - ????? ??????? ????? ???????????? ????????,
High - ????? ??????? ????? ??????????? ????????.

Low_Speed   - 233x120
High_Speed  - 383x120
Low_Scrol   - 233x151
High_Scrol  - 383x151
Low_Music   - 233x182
High_Music  - 383x182
Low_Sound   - 233x214
High_Sound  - 383x214
*/
extern int exRealLx;
extern int exRealLy;
extern int exFMode;
int GetCDVolume();
void SetCDVolume(int);
int PlayMode=2;
void StopPlayCD();
void PlayRandomTrack();
extern byte TracksMask[16];
CEXPORT char RECFILE[128]="";

bool CreateMultiplaterInterface();
void ProcessMissions();
void PlayVideo();
extern char BuildVersion[32];
void SlideShow();
void ShowPreview();
extern byte PlayGameMode;
extern bool RetryVideo;
int GetRndVid(int N);
void CloseLogin();
void processMLoadGame();
bool InMainMenuLoop=0;

//--------------=< G A M E   S C R E E N >=------------//
extern int PrpX;
extern int PrpY;
extern int PrpNx;
extern int PrpNy;
extern int AblX;
extern int AblY;
extern int AblNx;
extern int AblNy;
extern int IconLx;
extern int AddIconLx;
extern int AddIconLy;
//extern int IconLx;
extern int IconLy;
DialogsSystem GSYS(0,0);
SimpleDialog* GVPort;
SimpleDialog* MiniVPort;
SimpleDialog* AblVPort;
SimpleDialog* PrpVPort;
void GSYSDRAW(){
	v_MainMenu.ProcessDSinGame();
	GSYS.RefreshView();
};
int TAVR=0;
void ProcessScreen();
void DrawScr(){
	int T0=GetTickCount();
	ProcessScreen();
	GSYSDRAW();
	int DT=GetTickCount()-T0;
	if(TAVR)TAVR=(TAVR*7+DT)>>3;
	else TAVR=DT;
};
//Picture* Pan1;
//Picture* Pan2;
//Picture* Pan3;
ColoredBar* Pan4;
ColoredBar* Pan5;
bool GameExit;
int MenuType;
bool MakeMenu;

int NNames;
char* names[128];
char* fnames[128];
int   serials[128];
void ClearNames(){
	for(int i=0;i<128;i++)names[i]=NULL;
	NNames=0;
};
void FreeNames(){
	for(int i=0;i<128;i++)if(names[i]){
		free(names[i]);
		free(fnames[i]);
		names[i]=NULL;
		fnames[i]=NULL;
	};
	NNames=0;
};
void ErrorBox(char* str){
	return;
};
extern int sfVersion;
#ifdef MAKE_PTC
void InstallName(ListBox* LB,WIN32_FIND_DATA* FD,char* StartDir){
	char CCC[256];
	sprintf(CCC,"%s%s",StartDir,FD->cFileName);
	ResFile ff1=RReset(CCC);
	if(ff1!=INVALID_HANDLE_VALUE){
		int sig,lap;
		RBlockRead(ff1,&sig,4);
		if(sig=='FSAK'||sig=='[CER'||sig==']CER'){
			if(sig=='[CER'||sig==']CER')sig=sfVersion;
			else{
				RBlockRead(ff1,&sig,4);
				RBlockRead(ff1,&lap,4);
			};
			if(sig==sfVersion&&NNames<512){
				//int nlen=0;
				//RBlockRead(ff1,&nlen,2);
				//nlen=0;
				//RBlockRead(ff1,&nlen,1);
				//RBlockRead(ff1,nam,nlen);
				RClose(ff1);
				LB->AddItem(CCC,NNames);
				fnames[NNames]=znew(char,strlen(CCC)+1);
				//serials[NNames]=lap;
				//strcpy(names[NNames],nam);
				strcpy(fnames[NNames],CCC);
				NNames++;
			}else RClose(ff1);
		}else RClose(ff1);
	};
};
#endif

//----------------LOAD GAME--------------//
void CmdLoadNetworkGame(byte NI,int ID,char* Name);
SFLB_DLLEXPORT SFLB_LoadGame(char* fnm,bool LoadNation);
void OldProcessGLoadGame(){

};
extern EXBUFFER EBufs[MaxPL];
DWORD MAPREPL[8];
bool CheckFileIdentity(char* Name){
	if(NPlayers==1)return true;
	DWORD FID[512];
	FID[0]='FIDN';
	DWORD S=GetMapSUMM(Name);
	FID[1]=S;
	if(FID[1]==0xFFFFFFFF)return false;
	memset(FID+2,0,128);
	strcpy((char*)(FID+2),Name);
	memset(MAPREPL,0,8*4);
	for(int i=0;i<NPlayers;i++)
		if(GSets.CGame.PL_INFO[i].PlayerID==MyDPID)MAPREPL[i]=S;
	SendToAllPlayers(128+8,FID);
	int t0=GetTickCount();
	int t1=t0;
	do{
		if(GetTickCount()-t1>500){
			t1=GetTickCount();
			SendToAllPlayers(128+8,FID);
		};
		ProcessMessages();
		int OK=1;
		for(int i=0;i<NPlayers;i++)if(EBufs[i].Enabled){
			if(MAPREPL[i]){
				if(MAPREPL[i]!=S)return false;
			}else OK=0;
		};
		if(OK)return true;
	}while(GetTickCount()-t0<10000);
	return false;
};

void ProcessFranceMission(){
	SQPicture Pan1("Interface\\Missions\\MTFrance_Back_1.bmp");
	SQPicture Pan2("Interface\\Missions\\MTFrance_Back_2.bmp");
	SlowUnLoadPalette("1\\agew_1.pal");
	Pan1.Draw(0,0);
	FlipPages();
	SlowLoadPalette("1\\agew_1.pal");
	int t0=GetRealTime();
	KeyPressed=0;
	do{
		ProcessMessages();
	}while(!(GetRealTime()-t0>15000||Lpressed||KeyPressed));
	if(LastKey==27)return;
	LastKey=0;
	SlowUnLoadPalette("1\\agew_1.pal");
	Pan2.Draw(0,0);
	FlipPages();
	SlowLoadPalette("1\\agew_1.pal");
	t0=GetRealTime();
	KeyPressed=0;
	do{
		ProcessMessages();
	}while(!(GetRealTime()-t0>15000||Lpressed||KeyPressed));
	if(LastKey==27)return;

	strcpy(GSets.CGame.cgi_CurrentMap,MISSLIST.MISS[0].MapName);
	CreateNationalMaskForMap(GSets.CGame.cgi_CurrentMap);
	CurrentCampagin=-1;
	CurrentMission=-1;
	PrepareGameMedia(0,0,1);
	InitGame();
	ItemChoose=mcmSingle;
	//InitExplosions();
	MISSLIST.CurrentMission=0;
	SCENINF.Load(MISSLIST.MISS[0].DLLPath,MISSLIST.MISS[0].Description);
};
//----------------SAVE GAME--------------//
void CmdSaveNetworkGame(byte NI,int ID,char* Name);
bool CheckName(char* str){
	for(int i=0;i<NNames;i++)if(strstr(names[i],str))return false;
	return true;
};
bool ChangeNation;
bool MultiTvar;
extern int FogMode;
bool EnterVal(int * val,char* Message){
	Lpressed=false;
	char passw[128];
	passw[0]=0;
	ItemChoose=-1;
	sprintf(passw,"%d",*val);
	DialogsSystem DSY(smapx+(nsmaplx<<4)-(288>>1),smapy+(mul3(nsmaply)<<2)-(288>>1));
	//Picture* PIC1=DSY.addPicture(NULL,0,0,&y288x128,&y288x128,&y288x128);
	TextButton* LoadBtn=DSY.addTextButton(NULL,288>>1,5,Message,&FActive,&FPassive,&FDisable,1);
	InputBox* IB=DSY.addInputBox(NULL,30,45,passw,20,224,28,&WhiteFont,&WhiteFont);
	IB->Active=true;
	TextButton* OkBtn=DSY.addTextButton(NULL,55+164/2,90,"OK",&WhiteFont,&YellowFont,&YellowFont,1);
	OkBtn->UserParam=mcmOk;
	OkBtn->OnUserClick=&MMItemChoose;
	KeyPressed=false;
	do{
		DrawStdBar(DSY.BaseX,DSY.BaseY,DSY.BaseX+288,DSY.BaseY+128);
		DSY.MarkToDraw();
		ProcessMessages();
		if(KeyPressed){
			if(LastKey==13){
				ItemChoose=mcmOk;
				KeyPressed=false;
			};
			if(LastKey==27){
				ItemChoose=mcmCancel;
				KeyPressed=false;
			};
		};
		DSY.ProcessDialogs();
		DSY.RefreshView();
	}while(ItemChoose==-1);
	if(ItemChoose==mcmOk){
		int z=sscanf(passw,"%d",val);
		if(z==0)return false;
		else return true;
	}else return false;
};
bool EnterStr(char * str,char* Message){
	Lpressed=false;
	char passw[128];
	passw[0]=0;
	ItemChoose=-1;
	DialogsSystem DSY(smapx+(nsmaplx<<4)-(288>>1),smapy+(mul3(nsmaply)<<2)-(288>>1));
	//Picture* PIC1=DSY.addPicture(NULL,0,0,&y288x128,&y288x128,&y288x128);
	TextButton* LoadBtn=DSY.addTextButton(NULL,288>>1,5,Message,&YellowFont,&YellowFont,&YellowFont,1);
	InputBox* IB=DSY.addInputBox(NULL,30,45,str,30,224,28,&BigWhiteFont, &BigWhiteFont);//&RedFont,&RedFont);
	IB->Active=true;
	TextButton* OkBtn=DSY.addTextButton(NULL,55+164/2,90,"OK",&WhiteFont,&YellowFont,&YellowFont,1);
	OkBtn->UserParam=mcmOk;
	OkBtn->OnUserClick=&MMItemChoose;
	KeyPressed=false;
	do{
		DrawStdBar(DSY.BaseX,DSY.BaseY,DSY.BaseX+288,DSY.BaseY+128);
		DSY.MarkToDraw();
		ProcessMessages();
		if(KeyPressed){
			if(LastKey==13){
				ItemChoose=mcmOk;
				KeyPressed=false;
			};
			if(LastKey==27){
				ItemChoose=mcmCancel;
				KeyPressed=false;
			};
		};
		DSY.ProcessDialogs();
		DSY.RefreshView();
	}while(ItemChoose==-1);
	if(ItemChoose==mcmOk){
		return true;
	}else return false;
};
void MemScreenPart(int x,int y,int lx,int ly,byte* Ptr){
#ifndef _USE3D
	for(int yy=0;yy<ly;yy++){
		int ofs=x+(y+yy)*ScrWidth;
		memcpy(Ptr,((byte*)ScreenPtr)+ofs,lx);
		Ptr+=lx;
	};
#endif
};
int EnterHi(int * val,int Type){

	LocalGP LONGBOX("Interface\\LongBox0");
	Lpressed=false;
	char passw[128];
	passw[0]=0;
	ItemChoose=-1;
	sprintf(passw,"%d",*val);
	int LX=400;
	int LY=118;	
	DialogsSystem DSY((RealLx-LX)>>1,(RealLy-LY)>>1);	
	DSY.addBorder(0,0,LX,LY,0,1);
	DSY.addGPPicture(NULL,LX/4-75,LY+6,LONGBOX.GPID,8);
	DSY.addGPPicture(NULL,LX/4-75,LY+6,LONGBOX.GPID,11);
	DSY.addGPPicture(NULL,3*LX/4-75,LY+6,LONGBOX.GPID,8);
	DSY.addGPPicture(NULL,3*LX/4-75,LY+6,LONGBOX.GPID,11);
	GP_TextButton* OkBtn=DSY.addGP_TextButton(NULL,LX/4-75+23,LY+6+20,GetTextByID("@OK"),LONGBOX.GPID,9,&WhiteFont,&YellowFont);
	OkBtn->UserParam=mcmOk;
	OkBtn->OnUserClick=&MMItemChoose;
	GP_TextButton* CancelBtn=DSY.addGP_TextButton(NULL,3*LX/4-75+23,LY+6+20,GetTextByID("@CANCEL"),LONGBOX.GPID,9,&WhiteFont,&YellowFont);
	CancelBtn->UserParam=mcmCancel;
	CancelBtn->OnUserClick=&MMItemChoose;
	//Picture* PIC1=DSY.addPicture(NULL,0,0,&y288x256,&y288x256,&y288x256);
	DSY.addTextButton(NULL,6,1,GetTextByID("$#HIHDR"),&YellowFont,&YellowFont,&YellowFont,0);
	TextButton* LoadBtn=DSY.addTextButton(NULL,10,87,GetTextByID("$#HEIGHT"),&YellowFont,&YellowFont,&YellowFont,0);
	InputBox* IB=DSY.addInputBox(NULL,120,80,passw,20,224,28,&WhiteFont,&WhiteFont);
	//ColoredBar* CBR=DSY.addColoredBar(120,80,LX-140,23,0x4c);
	//CBR->Style=1;
	DSY.addTextButton(NULL,10,51,GetTextByID("$#ACTION"),&YellowFont,&YellowFont,&YellowFont,0);
	ComboBox* CB=DSY.addGP_ComboBoxDLX(NULL,120,44,LX-140,LONGBOX.GPID,0,9,9,&WhiteFont,&YellowFont,NULL);
	CB->AddLine("Cubic line");
	CB->AddLine("Quad. line");
	CB->AddLine("Softing");
	KeyPressed=false;
	do{
		//DrawStdBar(DSY.BaseX,DSY.BaseY,DSY.BaseX+288,DSY.BaseY+256);
		bool LP=Lpressed;
		Lpressed=0;
		if(bActive){
			if(GameMode!=1)ProcessScreen();
			else IRS->ClearDevice();
		}
		Lpressed=LP;
		DSY.MarkToDraw();
		ProcessMessages();
		if(KeyPressed){
			if(LastKey==13){
				ItemChoose=mcmOk;
				KeyPressed=false;
			};
			if(LastKey==27){
				ItemChoose=mcmCancel;
				KeyPressed=false;
			};
		};
		DSY.ProcessDialogs();
		DSY.RefreshView();
	}while(ItemChoose==-1);
	if(ItemChoose==mcmOk){
		int z=sscanf(passw,"%d",val);
		if(*val<-500)*val=0;
		if(*val>500)*val=500;
		if(z==0)return false;
		else return CB->CurLine+1;
	}else return false;
};
DLLEXPORT void ShowVictory(char* Message=NULL, byte NI=0xFF);
DLLEXPORT void SelChangeNation(byte SrcNat,byte DstNat);
CEXPORT void Add_VI_Zone(int x, int y, DWORD color);

void EnterChat(){
	if(NPlayers>1)return;
	Lpressed=false;
	char passw[128];
	passw[0]=0;
	ItemChoose=-1;
	DialogsSystem DSY(smapx+(nsmaplx<<4)-(288>>1),smapy+(mul3(nsmaply)<<2)-(288>>1));
	//Picture* PIC1=DSY.addPicture(NULL,0,0,&y288x128,&y288x128,&y288x128);
	TextButton* LoadBtn=DSY.addTextButton(NULL,288>>1,5,"Enter password:",&FActive,&FPassive,&FDisable,1);
	InputBox* IB=DSY.addInputBox(NULL,30,45,passw,20,224,28,&WhiteFont,&WhiteFont);
	IB->Active=true;
	TextButton* OkBtn=DSY.addTextButton(NULL,55+164/2,90,"OK",&WhiteFont,&YellowFont,&YellowFont,1);
	OkBtn->UserParam=mcmOk;
	OkBtn->OnUserClick=&MMItemChoose;
	KeyPressed=false;
	do{
		DrawStdBar(DSY.BaseX,DSY.BaseY,DSY.BaseX+288,DSY.BaseY+128);
		DSY.MarkToDraw();
		ProcessMessages();
		if(KeyPressed&&LastKey==13){
			ItemChoose=mcmOk;
			KeyPressed=false;
		};
		DSY.ProcessDialogs();
		DSY.RefreshView();
	}while(ItemChoose==-1);
	if(ItemChoose==mcmOk){
		if(!strcmp(passw,"qwe")){
//			ChangeNation=1;
//			MultiTvar=1;
//			FogMode=0;
//			extern int NCallsMakeDamage;
//			NCallsMakeDamage=0;
		}else
			if(!strcmp(passw,"walkover")){
#ifndef AI_CHEATS
				CurrentSinBattle=-1;
#endif
				if(NPlayers<2)ShowVictory();
			}else
				if(!strcmp(passw,"ntch"))ChangeNation=!ChangeNation;
			if(!strcmp(passw,"nastycheat"))MultiTvar=!MultiTvar;
			if(!strcmp(passw,"Iseeall"))FogMode=!FogMode;
			if(!strcmp(passw,"deposits")&&CurrentSinBattle==0){
#ifndef AI_CHEATS
				CurrentSinBattle=-1;
#endif
				AddXRESRC(MyNation,0,150000);
				AddXRESRC(MyNation,1,150000);
				AddXRESRC(MyNation,2,150000);
				AddXRESRC(MyNation,3,150000);
				AddXRESRC(MyNation,4,150000);
				AddXRESRC(MyNation,5,150000);
			}else{
				char cc1[64];
				int n1,n2;
				int z=sscanf(passw,"%s%d%d",cc1,&n1,&n2);
				if(z==3&&!strcmp(cc1,"change")){
					if(n1>=0&&n1<8&&n2>=0&&n2<=8)SelChangeNation(n1,n2);
				};
				if(NPlayers==1){
					char cc2[128];
					z=sscanf(passw,"%s%s",cc1,cc2);
					if(z==2&&!strcmp(cc1,"changeunit")){
						word Type=0xFFFF;
						for(int i=0;i<NATIONS->NMon;i++){
							if(!strcmp(cc2,NATIONS->Mon[i]->MonsterID)){
								GeneralObject* GO=NATIONS->Mon[i];
								if(!GO->newMons->Building){
									//found!!!
									int N=NSL[MyNation];
									word* IDS=Selm[MyNation];
									word* SMS=SerN[MyNation];
									for(int j=0;j<N;j++){
										word MID=IDS[j];
										if(MID!=0xFFFF){
											OneObject* OB=Group[MID];
											if(OB&&!OB->Sdoxlo){
												OB->Ref.General=GO;
												OB->NIndex=i;
												OB->newMons=GO->newMons;
											};
										};
									};
								};
							};
						};
					};
				};
			};
	};
};
void SaveGame(char* fnm,char* Messtr,int ID);

//----------------SAVE MAP--------------//
void NormalName(char* s){
	if(!strstr(s,".m3d")){
		strcat(s,".m3d");
	};
};
bool LBCLICK(SimpleDialog* SD){
	ListBox* LB=(ListBox*)SD;
	if(LB->CurItem!=-1){
		strcpy(GameName,(LB->GetItem(LB->CurItem))->Message);
	};
	return false;
};
void SaveExtendedMap(char* fnm);
void PushScreen(byte** Ptr);
void PopScreen(byte* Ptr);
bool ProcessSavingFile(char* Mask,char* DestName,int Header,bool clear);
void ProcessGSaveMap(){
	/*
	byte* p;
	PushScreen(&p);
	if(strstr(GameName,".sav")||strstr(GameName,".SAV"))GameName[0]=0;	
	if(ProcessSavingFile("*.m3d",GameName,7,0)){
	*/
		NormalName(GameName);
		ResFile F=RReset(GameName);
		if(F!=INVALID_HANDLE_VALUE){
			RClose(F);
			char ccc[128];
			strcpy(ccc,GameName);
			char* cc=strstr(ccc,".m3d");
			if(cc){
				
				// загадка началло
				strcpy(cc,".bak");
				DeleteFile(cc);
				// загадка конец
				
				//SetFileAttributes(ccc,FILE_ATTRIBUTE_NORMAL);

				void RenameAnyway(char* src,char* dst);
				RenameAnyway(GameName,ccc);
				
				//SetFileAttributes(ccc,FILE_ATTRIBUTE_NORMAL);

				//if( attr!=INVALID_FILE_ATTRIBUTES && attr&FILE_ATTRIBUTE_READONLY ){

				//	HANDLE hf = CreateFile(GameName, 0/*GENERIC_READ | GENERIC_WRITE*/,
				//		(DWORD)0, NULL, CREATE_ALWAYS, attr/*FILE_ATTRIBUTE_NORMAL*/, (HANDLE)NULL);
				//	CloseHandle(hf);

				//}

			};
		};
		Save3DMap(GameName);
	/*
	};
	PopScreen(p);
	*/
};
//----------------LOAD Map--------------//
void PreLoadExtendedMap(char* fnm);
void PostLoadExtendedMap(char* fnm);
bool ProcessLoadingFile(char* Mask,char* DestName,int Header);
extern bool PeaceMode;
void ProcessGLoadMap(){
	ClearLoadMark();
	AddLoadMark("Nations",NNations*15);
	AddLoadMark("PrepareGameMedia",100);	
	/*
	byte* p;
	PushScreen(&p);
	if(ProcessLoadingFile("*.m3d",GameName,4)){
	*/
		//UnLoading();
		//void PrepareEditMedia(byte,byte);
		//PrepareEditMedia(MyNation,ADDSH);
		void ClearAllDipData();
		ClearAllDipData();
		Load3DMap(GameName);
		ACTION_AfterLoadingMapInEditor();
		InitDipForThisMap();
		PeaceMode=1;	
	/*
	};
	PopScreen(p);
	*/
	/*
	SQPicture Pan("Pan15.bpx");
	SQPicture scr_U("scr_v_u.bpx");
	SQPicture scr_D("scr_v_d.bpx");
	SQPicture scr_V("scr_v.bpx");
	SQPicture scr_Line("scr_15.bpx");
	RLCFont gf24w(GETS("@SMFONT1"));
	RLCFont gf24y(GETS("@SMFONT0"));
	RLCFont ListF1(GETS("@CULISTF1"));
	RLCFont ListF0(GETS("@CULISTF0"));
	int mmlx=Pan.GetLx();
	int mmly=Pan.GetLy();

	DialogsSystem DSY(smapx+(nsmaplx<<4)-(mmlx>>1),smapy+(mul3(nsmaply)<<2)-(mmly>>1));
	Picture* PIC1=DSY.addPicture(NULL,0,0,&Pan,&Pan,&Pan);
	//TextButton* LoadBtn=DSY.addTextButton(PIC1,352>>1,30,"????????? ?????",&FActive,&FPassive,&FDisable,1);
	ListBox* LB=DSY.addListBox(PIC1,GETV("LMLSTX"),GETV("LMLSTY"),GETV("LMLSTNY"),GETV("LMLSTLX"),GETV("LMLSTLY"),&ListF1,&ListF0,NULL);
	VScrollBar* VS=DSY.addVScrollBar(PIC1,GETV("LMSCRX"),GETV("LMSCRY"),1000,4,&scr_U,&scr_U,&scr_U,&scr_D,&scr_D,&scr_D,&scr_Line,&scr_Line,&scr_V);
	LB->VS=VS;
	TextButton* OkBtn=DSY.addTextButton(PIC1,GETV("LMOKX"),GETV("LMOKY"),GETS("@LMOKSTR"),&gf24w,&gf24y,&gf24y,1);
	OkBtn->UserParam=mcmOk;
	OkBtn->OnUserClick=&MMItemChoose;
	TextButton* CancelBtn=DSY.addTextButton(PIC1,GETV("LMNOX"),GETV("LMNOY"),GETS("@LMNOSTR"),&gf24w,&gf24y,&gf24y,1);
	CancelBtn->UserParam=mcmCancel;
	CancelBtn->OnUserClick=&MMItemChoose;
	CreateNewMapsList(LB);
	ItemChoose=-1;
	do{
	DSY.MarkToDraw();
	ProcessMessages();
	StdKeys();
	if(LB->CurItem==-1){
	OkBtn->Enabled=false;
	if(ItemChoose==mcmOk)ItemChoose=-1;
	}else OkBtn->Enabled=true;
	DSY.ProcessDialogs();
	DSY.MarkToDraw();
	DSY.RefreshView();
	}while(ItemChoose==-1);
	if(ItemChoose==mcmOk&&LB->CurItem!=-1){
	Load3DMap((LB->GetItem(LB->CurItem))->Message);
	strcpy(GameName,(LB->GetItem(LB->CurItem))->Message);
	};
	//FreeNames();
	*/
};
//-----------------OPTIONS---------------//


//---------------------------------------//
void NewMap(int szX,int szY);
extern byte mapPos[16];
void ClearAllZones();
void CreateNewMap(int SizeX,int SizeY){
	UnLoading();
	InitAllGame();
	NewMap(SizeX,SizeY);
	SetMyNation(0);
	ItemChoose=mcmSingle;
	msx=SizeX;
	msy=SizeY;
	InitExplosions();
	void SetInitialScrollLimitations();
	SetInitialScrollLimitations();
	//ClearAllZones();
};
//--------Choose units Dialog------------//
void CreateUnitList(ListBox* LB,int Country){
	Nation* NT=&NATIONS[MyNation];
	LB->CurItem=0;
	LB->FirstItem=0;
	LB->FLItem=0;
	LB->VS->SPos=0;
	if(!Country){
		LB->AddStaticItem("UNITS:    ",0);
		for(int i=0;i<NT->NMon;i++){
			NewMonster* NM=NT->Mon[i]->newMons;
			if(!NM->Building)
				LB->AddItem(NM->Message,i);
		};
		LB->AddStaticItem("BUILDINGS:   ",0);
		for(int i=0;i<NT->NMon;i++){
			NewMonster* NM=NT->Mon[i]->newMons;
			if(NM->Building)
				LB->AddItem(NM->Message,i);
		};
		return;
	};
	Country--;
	int NUNITS=NT->NUnits[Country];
	if(NUNITS){
		for(int j=0;j<NUNITS;j++){
			int UIDS=NT->UnitsIDS[Country][j];
			if(UIDS==0xFFFF){
				LB->AddStaticItem(NT->UnitNames[Country][j],0xFFFF);
			}else{
				NewMonster* NM=NT->Mon[UIDS]->newMons;
				LB->AddItem(NM->Message,UIDS);
			};
		};
	};
};
extern int NNations;
extern char** NatNames;
void UnPress();
void ChooseUnit(){
	ItemChoose=-1;
	SQPicture Pan("Units.bpx");
	//SQPicture Lstr("195x20.bpx");
	SQPicture scr_U("scr_v_u.bpx");
	SQPicture scr_D("scr_v_d.bpx");
	SQPicture scr_V("scr_v.bpx");
	SQPicture scr_Line("scr_13.bpx");
	RLCFont BtnF0(GETS("@CUFONT0"));
	RLCFont BtnF1(GETS("@CUFONT1"));
	RLCFont BtnL0(GETS("@CULISTF0"));
	RLCFont BtnL1(GETS("@CULISTF1"));
	RLCFont SelF (GETS("@CULISTF2"));
	int mmlx=Pan.GetLx();
	int mmly=Pan.GetLy();
	DialogsSystem DSY(smapx+(nsmaplx<<4)-(mmlx>>1),smapy+(mul3(nsmaply)<<2)-(mmly>>1));
	Picture* PIC1=DSY.addPicture(NULL,0,0,&Pan,&Pan,&Pan);
	//TextButton* LoadBtn=DSY.addTextButton(PIC1,352>>1,30,"????? ???????",&FActive,&FPassive,&FDisable,1);
	ListBox* LBU=DSY.addListBox(PIC1,GETV("CUUNITX"),GETV("CUUNITY"),GETV("CUNY"),GETV("CULX"),GETV("CULY"),&BtnL1,&BtnL0,NULL);
	ListBox* LBB=DSY.addListBox(PIC1,GETV("CUBUILDX"),GETV("CUBUILDY"),GETV("CUNY"),GETV("CULX"),GETV("CULY"),&BtnL1,&BtnL0,NULL);
	LBB->DFont=&SelF;
	VScrollBar* VSU=DSY.addVScrollBar(PIC1,GETV("CUUNITSX"),GETV("CUUNITSY"),1000,4,&scr_U,&scr_U,&scr_U,&scr_D,&scr_D,&scr_D,&scr_Line,&scr_Line,&scr_V);
	VScrollBar* VSB=DSY.addVScrollBar(PIC1,GETV("CUBUILDSX"),GETV("CUBUILDSY"),1000,4,&scr_U,&scr_U,&scr_U,&scr_D,&scr_D,&scr_D,&scr_Line,&scr_Line,&scr_V);

	LBU->VS=VSU;
	LBB->VS=VSB;

	TextButton* OkBtn=DSY.addTextButton(PIC1,GETV("CUOKX"),GETV("CUOKY"),GETS("@CUOKSTR"),&BtnF0,&BtnF1,&BtnF1,1);
	OkBtn->UserParam=mcmOk;
	OkBtn->OnUserClick=&MMItemChoose;
	TextButton* CancelBtn=DSY.addTextButton(PIC1,GETV("CUNOX"),GETV("CUNOY"),GETS("@CUNOSTR"),&BtnF0,&BtnF1,&BtnF1,1);
	CancelBtn->UserParam=mcmCancel;
	CancelBtn->OnUserClick=&MMItemChoose;
	Nation* NT=&NATIONS[MyNation];
	LBU->AddItem("ALL UNITS",0);
	for(int j=0;j<NNations;j++){
		LBU->AddItem(NatNames[j],j+1);
	};
	LBU->CurItem=0;
	LBU->FLItem=0;
	LBB->CurItem=0;
	LBB->FLItem=0;
	VSU->SPos=0;
	VSB->SPos=0;
	int CurNation=0;
	CreateUnitList(LBB,CurNation);
	ItemChoose=-1;
	int curpan=-1;
	do{
		ProcessMessages();
		if(!LBB->NItems)OkBtn->Enabled=false;
		else OkBtn->Enabled=true;
		DSY.MarkToDraw();
		DSY.ProcessDialogs();
		//DSY.MarkToDraw();
		DSY.RefreshView();
		if(CurNation!=LBU->CurItem){
			LBB->ClearItems();
			CurNation=LBU->CurItem;
			CreateUnitList(LBB,CurNation);
		};
	}while(ItemChoose==-1);
	if(ItemChoose==mcmOk){
		if(LBB->NItems){
			Creator=(LBB->GetItem(LBB->CurItem))->Param1;
		}else Creator=4096+255;
		ContinueGame=true;
	};
	UnPress();
};
void ChooseUnitOld(){
	ItemChoose=-1;
	SQPicture Pan("Units.bpx");
	SQPicture Lstr("195x20.bpx");
	SQPicture scr_U("scr_v_u.bpx");
	SQPicture scr_D("scr_v_d.bpx");
	SQPicture scr_V("scr_v.bpx");
	SQPicture scr_Line("scr_13.bpx");
	RLCFont BtnF0(GETS("@CUFONT0"));
	RLCFont BtnF1(GETS("@CUFONT1"));
	RLCFont BtnL0(GETS("@CULISTF0"));
	RLCFont BtnL1(GETS("@CULISTF1"));
	int mmlx=Pan.GetLx();
	int mmly=Pan.GetLy();
	DialogsSystem DSY(smapx+(nsmaplx<<4)-(mmlx>>1),smapy+(mul3(nsmaply)<<2)-(mmly>>1));
	Picture* PIC1=DSY.addPicture(NULL,0,0,&Pan,&Pan,&Pan);
	//TextButton* LoadBtn=DSY.addTextButton(PIC1,352>>1,30,"????? ???????",&FActive,&FPassive,&FDisable,1);
	ListBox* LBU=DSY.addListBox(PIC1,GETV("CUUNITX"),GETV("CUUNITY"),GETV("CUNY"),GETV("CULX"),GETV("CULY"),&BtnL1,&BtnL0,NULL);
	ListBox* LBB=DSY.addListBox(PIC1,GETV("CUBUILDX"),GETV("CUBUILDY"),GETV("CUNY"),GETV("CULX"),GETV("CULY"),&BtnL1,&BtnL0,NULL);

	VScrollBar* VSU=DSY.addVScrollBar(PIC1,GETV("CUUNITSX"),GETV("CUUNITSY"),1000,4,&scr_U,&scr_U,&scr_U,&scr_D,&scr_D,&scr_D,&scr_Line,&scr_Line,&scr_V);
	VScrollBar* VSB=DSY.addVScrollBar(PIC1,GETV("CUBUILDSX"),GETV("CUBUILDSY"),1000,4,&scr_U,&scr_U,&scr_U,&scr_D,&scr_D,&scr_D,&scr_Line,&scr_Line,&scr_V);

	LBU->VS=VSU;
	LBB->VS=VSB;

	TextButton* OkBtn=DSY.addTextButton(PIC1,GETV("CUOKX"),GETV("CUOKY"),GETS("@CUOKSTR"),&BtnF0,&BtnF1,&BtnF1,1);
	OkBtn->UserParam=mcmOk;
	OkBtn->OnUserClick=&MMItemChoose;
	TextButton* CancelBtn=DSY.addTextButton(PIC1,GETV("CUNOX"),GETV("CUNOY"),GETS("@CUNOSTR"),&BtnF0,&BtnF1,&BtnF1,1);
	CancelBtn->UserParam=mcmCancel;
	CancelBtn->OnUserClick=&MMItemChoose;
	Nation* NT=&NATIONS[MyNation];
	for(int j=0;j<NT->NMon;j++){
		NewMonster* NM=NT->Mon[j]->newMons;
		if(NM->Building)LBB->AddItem(NT->Mon[j]->Message,j);
		else LBU->AddItem(NT->Mon[j]->Message,j);
	};
	LBU->CurItem=0;
	LBU->FLItem=0;
	LBB->CurItem=0;
	LBB->FLItem=0;

	ItemChoose=-1;
	int curpan=-1;
	do{
		ProcessMessages();
		if(curpan==-1)OkBtn->Enabled=false;
		else OkBtn->Enabled=true;
		DSY.MarkToDraw();
		DSY.ProcessDialogs();
		//DSY.MarkToDraw();
		DSY.RefreshView();
		if(curpan==-1){
			if(LBU->CurItem!=-1)curpan=1;
			if(LBB->CurItem!=-1)curpan=2;
		};
		if(curpan==1&&LBB->CurItem!=-1){
			LBU->CurItem=-1;
			curpan=2;
		};
		if(curpan==2&&LBU->CurItem!=-1){
			LBB->CurItem=-1;
			curpan=1;
		};
	}while(ItemChoose==-1);
	if(curpan!=-1&&ItemChoose==mcmOk){
		if(curpan==1)Creator=(LBU->GetItem(LBU->CurItem))->Param1;
		if(curpan==2)Creator=(LBB->GetItem(LBB->CurItem))->Param1;
		ContinueGame=true;
	};
	UnPress();
};
//------MAIN MENU IN MAP EDITOR----------//

int ProcessE_GMainMenu();
int ProcessGMapEditMainMenu(){
	return ProcessE_GMainMenu();
	ContinueGame=false;
	SQPicture EPanel("edmenu.bpx");
	int mmlx=EPanel.GetLx();
	int mmly=EPanel.GetLy();
	RLCFont gf32w(GETS("@EMFONT1"));
	RLCFont gf32y(GETS("@EMFONT0"));
	DialogsSystem GMM(smapx+(nsmaplx<<4)-(mmlx>>1),smapy+(mul3(nsmaply)<<2)-(mmly>>1));
	Picture* PIC1=GMM.addPicture(NULL,0,0,&EPanel,&EPanel,&EPanel);
	int emmx=GETV("EMMENUX");
	TextButton* ExitBtn=GMM.addTextButton(PIC1,emmx,GETV("EMRETY"),GETS("@EMRET"),&gf32w,&gf32y,&gf32y,1);
	TextButton* SaveBtn=GMM.addTextButton(PIC1,emmx,GETV("EMSAVEMY"),GETS("@EMSAVEM"),&gf32w,&gf32y,&gf32y,1);
	SaveBtn->UserParam=mcmSave;
	SaveBtn->OnUserClick=&MMItemChoose;
	TextButton* LoadBtn=GMM.addTextButton(PIC1,emmx,GETV("EMLOADMY"),GETS("@EMLOADM"),&gf32w,&gf32y,&gf32y,1);
	LoadBtn->UserParam=mcmLoad;
	LoadBtn->OnUserClick=&MMItemChoose;
	TextButton* NewBtn=GMM.addTextButton(PIC1,emmx,GETV("EMNEWMY"),GETS("@EMNEWM"),&gf32w,&gf32y,&gf32y,1);
	TextButton* HelpBtn=GMM.addTextButton(PIC1,emmx,GETV("EMOPTY"),GETS("@EMOPT"),&gf32w,&gf32y,&gf32y,1);
	HelpBtn->UserParam=mcmOptions;
	HelpBtn->OnUserClick=&MMItemChoose;
	ExitBtn->UserParam=mcmExit;
	ExitBtn->OnUserClick=&MMItemChoose;
	TextButton* ResumeBtn=
		GMM.addTextButton(PIC1,emmx,GETV("EMCANCELY"),GETS("@EMCANCEL"),&gf32w,&gf32y,&gf32y,1);
	ResumeBtn->UserParam=mcmResume;
	ResumeBtn->OnUserClick=&MMItemChoose;
	ItemChoose=-1;
	do{
		ProcessMessages();
		GMM.ProcessDialogs();
		GMM.RefreshView();
	}while(ItemChoose==-1);
	if(ItemChoose==mcmLoad){
		ProcessGLoadMap();
		GMM.MarkToDraw();
		ItemChoose=mcmResume;
		//if(!ContinueGame)goto stg;
	};
	if(ItemChoose==mcmSave){
		ProcessGSaveMap();
		GMM.MarkToDraw();
		ItemChoose=mcmResume;
		//if(!ContinueGame)goto stg;
	};
	if(ItemChoose==mcmOptions){
		ProcessGameOptions();
		//ProcessOptions(smapx+(nsmaplx<<4)-(288>>1),smapy+(nsmaply<<3)-(288>>1));
		ContinueGame=true;
		ItemChoose=mcmResume;
	};
	return ItemChoose;	
}; 
//------------------Selecting Texture dialog--------------
extern byte EditMedia;
extern bool MEditMode;
extern bool EditMapMode;
extern int HeightEditMode;
word TexList[128];
word NTextures;
void RefreshED_BV();
word GetTexture(){
	if(!NTextures)return 0;
	return TexList[(rando()*NTextures)>>15];
};
void MSetTexture(int tex,bool add){
	if(add){
		if(NTextures>=128)return;
		TexList[NTextures]=tex;
		NTextures++;
		Lpressed=0;
	}else{
		TexList[0]=tex;
		NTextures=1;
	};
	RefreshED_BV();
};
extern int MaxTex;
int SelectTexture(){	
	return ItemChoose;	
};
//----------MAIN MENU IN GAME------------//
bool IsGameActive();
int LastLookTime=0;


//---------------------------------------//
extern int HintX;
extern int HintY;
byte ZoneOpt[128];
//  /-------------------------------------------------------\
//  | /--------\  2    3    4    5    6    7    8   /------\|       
//  | | 1      |                                    |      ||
//  | |        |  9    10   11   12  ...            |      ||
//  | |        |                                    |      ||
//  | \--------/  ...                               \------/|
//  \-------------------------------------------------------/
// ZoneOpt[i]:  0   -   empty
//              |1  -   used now
//              |2  -   was used
int Zone1X;
int Zone1Y;
int Zone1LX;
int Zone1LY;
int ZonesNy;
int ZonesNx;
int LeftListNx;
int PanelY;
int MessageUX;
int MessageUY;
int PropertyX;
int PropertyY;
//---------
int BrigPnX;
int BrigPnY;
int BrigNx;
int BrigNy;

void CopyBPX2SCR(int xs,int ys,int xb,int yb,int Lx,int Ly,SQPicture* P){
	if(!Ly)return;
	word* Pic=(word*)P->GetPixelData();
	int BLx=P->GetLx();
	int BLy=P->GetLy();
	int bofs=xb+yb*BLx+int(Pic);
	int sofs=xs+ys*SCRSizeX+int(ScreenPtr);
	int badd=BLx-Lx;
	int sadd=SCRSizeX-Lx;
	int Lx4=Lx>>2;
	int Lx1=Lx&3;
	__asm{
		pushf
			push    esi
			push    edi
			cld
			mov     esi,bofs
			mov     edi,sofs
			mov     edx,Ly
lpp1:   mov     ecx,Lx4
		rep     movsd
		mov     ecx,Lx1
		rep     movsb
		add     esi,badd
		add     edi,sadd
		dec     edx
		jnz     lpp1
		pop     edi
		pop     esi
		popf
	};

};
void ShowTextDiscription();
void DelUnusedZones(){	
};
void EconomePostDraw();
extern bool ECOSHOW;
extern byte PlayGameMode;
extern bool PAGE1PRESENT;
extern bool PAGE2PRESENT;
extern bool PAGE3PRESENT;
bool Page3HaveItems=0;
bool Page2HaveItems=0;
bool Page1HaveItems=0;
NewMonster* LASTNM=NULL;
extern bool RejectInterface;
extern bool HideBorderMode;
void DrawZones(){
	
};
void CopyZonesToScreen(){
	if(ZoneOpt[0]){
		CopyToScreen(Zone1X,Zone1Y,Zone1LX,Zone1LY);
	};
	int maxzon=ZonesNx*ZonesNy;
	for(int i=1;i<=maxzon;i++)if(ZoneOpt[i]){
		div_t ddt=div(i-1,ZonesNx);
		int x=ddt.rem*(IconLx+AddIconLx)+PrpX;
		int y=ddt.quot*(IconLy+AddIconLy)+PropertyY;
		CopyToScreen(x,y,IconLx+2,IconLy+2);
	};
};
bool NeedLoadGamePalette;
bool FullScreenMode=1;
void CreateINFORM();
extern int InfAddY;
extern int InfDY;
extern int InfDX;
extern int InfY1;
extern int InfY2;
DialogsSystem GLOBHINT(0,0);
void GSSetup800(){
	SetDarkPalette();
	CBar(0,0,SCRSizeX,SCRSizeY,0);
	memset(ZoneOpt,0,128);
	int DMiniX,DMiniY;
	int PanelLy;

	BrigPnX=6;
	BrigPnY=16+15;
	BrigNx=5;
	BrigNy=7;
	//if(RealLy>=768)
	InfAddY=16;
	//else InfAddY=0;
	InfDY=32+InfAddY;
	InfY1=450+InfAddY;
	InfY2=450-22+InfAddY;
	if(RealLy<=600){
		InfAddY=8;
		InfDY=32+InfAddY;
		InfY2=450-22-12-8;
		InfY1=450-12-8;
	}else{
		InfAddY=16;
		InfDY=32+InfAddY;
		InfY1=450+InfAddY;
		InfY2=450-22+InfAddY;
		if(RealLy<768){
			InfY1+=RealLy-768;
			InfY2+=RealLy-768;
		};
	};
	CreateINFORM();

	if(!CHKV("Zone1X")){
		Zone1X=6;
		Zone1Y=6;
		Zone1LX=132+40;
		Zone1LY=100;
		ZonesNy=6;
		ZonesNx=12;
		LeftListNx=3;
		MessageUX=75;
		MessageUY=8;
		PanelLy=144;
		PropertyX=142+40;
		PropertyY=6;
		IconLx=40;
		IconLy=40;
		DMiniX=152;
		DMiniY=7;
		AddIconLx=2;
		AddIconLy=2;
	}else{
		Zone1X=GETV("Zone1X");;
		Zone1Y=GETV("Zone1Y");
		Zone1LX=GETV("Zone1LX");
		Zone1LY=GETV("Zone1LY");
		ZonesNy=GETV("ZonesNy");
		ZonesNx=GETV("ZonesNx");
		LeftListNx=GETV("LeftListNx");
		MessageUX=GETV("MessageUX");;
		MessageUY=GETV("MessageUY");
		PanelLy=GETV("PanelLy");
		PropertyX=GETV("PropertyX");
		PropertyY=GETV("PropertyY");
		IconLx=GETV("IconLx");
		IconLy=GETV("IconLy");
		DMiniX=GETV("DMiniX");
		DMiniY=GETV("DMiniY");
		AddIconLx=GETV("AddIconLx");
		AddIconLy=GETV("AddIconLy");
	};
	MiniMode=false;
	Shifter=5;
	if(LMode)Shifter=3;
	Multip=0;
	//-------
	int scly=RealLy;
	if(!FullScreenMode)scly-=PanelLy;
	int CLY=mul3(32)>>2;
	int Y1=RealLy-PanelLy;
	smaply=div(scly,CLY).quot;
	smaplx=RealLx>>5;
	if(LMode){
		smaplx<<=2;
		smaply<<=2;
	};
	smapx=0;
	if(LMode)smapy=RealLy-smaply*4;
	else smapy=RealLy-smaply*16;
	minix=RealLx-DMiniX;
	miniy=RealLy-PanelLy+DMiniY;
	nsmaplx=smaplx;
	nsmaply=smaply;
	HintX=smapx+4;
	HintY=Y1-40-120;
	MiniMode=false;
	//ZonesNx=div(RealLx-PropertyX-DMiniX,IconLx+AddIconLx).quot;
	//if(ZonesNx<=7)PrpNx=3;else PrpNx=4;
	Multip=0;
	PrpX=PropertyX;
	PrpY=Y1+PropertyY;
	PrpNx=ZonesNx>>1;
	PrpNy=ZonesNy;

	AblX=PrpX;//+(PrpNx)*(IconLx+2);
	AblY=Y1+PropertyY;
	AblNy=ZonesNy;
	AblNx=PrpNx=ZonesNx;

	GSYS.~DialogsSystem();

	LoPanel.~SQPicture();
	//LoPanel.LoadPicture("800.bmp");
	/*
	switch(RealLx){
	case 1024:
	//LoPanel.LoadPicture("1024.bpx");
	break;
	case 1152:
	//LoPanel.LoadPicture("1152.bpx");
	break;
	case 1280:
	//LoPanel.LoadPicture("1280.bpx");
	break;
	default:
	LoPanel.LoadPicture("800.bmp");
	};
	*/
	GVPort=GSYS.addViewPort(0,0,RealLx,RealLy);
	PanelY=RealLy-PanelLy;
	MessageUY+=PanelY;
	//GSYS.addPicture(NULL,0,PanelY,&LoPanel,&LoPanel,&LoPanel);
	//MiniVPort=GSYS.addViewPort(minix,miniy,msx>>1,msy>>1);
	//AblVPort=GSYS.addViewPort(AblX,AblY,AblNx*(IconLx+2),AblNy*(IconLy+2));
	//PrpVPort=GSYS.addViewPort(PrpX,PrpY,PrpNx*(IconLx+2),PrpNy*(IconLy+2));
	//agew.pal");//grassw.pal");//dmw.pal");//"darkw.pal");
	NeedLoadGamePalette=true;
	if(mapx+smaplx>msx+1)mapx=msx-smaplx+1;
	if(mapy+smaply>msy+1)mapy=msy-smaply+1;

	GLOBHINT.SetHintStyle(1,1,1,30,3,3,20,20,170,200,&SmallBlackFont,0x80FFFFFF,0xFF000000);

	int x1=RealLx-1024+150;
	int dx0 = 140;

	SimpleDialog* MONEY;
	GLOBHINT.CloseDialogs();
	MONEY=GLOBHINT.addViewPort(x1,3,dx0-25,20);
	MONEY->Hint=RESHINT0;
	MONEY=GLOBHINT.addViewPort(x1+dx0,3,dx0-25,20);
	MONEY->Hint=RESHINT1;
	MONEY=GLOBHINT.addViewPort(x1+dx0*2,3,dx0-25,20);
	MONEY->Hint=RESHINT2;
	MONEY=GLOBHINT.addViewPort(x1+dx0*3,3,dx0-25,20);
	MONEY->Hint=RESHINT3;
	MONEY=GLOBHINT.addViewPort(x1+dx0*4,3,dx0-25,20);
	MONEY->Hint=RESHINT4;
	MONEY=GLOBHINT.addViewPort(x1+dx0*5,3,dx0-25,20);
	MONEY->Hint=RESHINT5;	

};
void GFieldShow();
void GMiniShow();
void GlobalHandleMouse();
void DRAW_MAP_TMP();
void DrawAllScreen(){	
	NeedLoadGamePalette=false;
	PRFUNC(GFieldShow());
	GMiniShow();
	extern bool IgnoreProcessMessages;
	IgnoreProcessMessages=1;
	ShowProp();
	ShowAbility();
	memset(ZoneOpt,0,128);
	DrawZones();
	DRAW_MAP_TMP();
	CopyToScreen(0,0,RealLx,RSCRSizeY);
	GlobalHandleMouse();
	IgnoreProcessMessages=0;
	MFix();

};
void GlobalHandleMouse();
void ShowAlexVisualSystem();
void ShowTimeSystem();
int GraphicsTime=0;
void FastScreenProcess(){	
	PGR1.Start(0);
	GraphicsTime=GetTickCount();
	RunPF(7,"FastScreenProcess");
	//	SERROR2();
	///RunPF(14);
	RunPF((256*5)+8,"GFieldShow");	
	PRFUNC(GFieldShow());
	if(GameExit)return;
	StopPF((256*5)+8);
	if(SHOWSLIDE){
		RunPF(11,"GMiniShow");
		GMiniShow();
		StopPF(11);
		extern bool IgnoreProcessMessages;
		IgnoreProcessMessages=1;
		ShowProp();
		ShowAbility();
		// for Alexander visual interface
		ShowAlexVisualSystem();
		ShowTimeSystem();
		SUSP("DRAWZONES");
		DrawZones();
		DRAW_MAP_TMP();
		RunPF(10,"GSYS.ProsessDialogs");		
		StopPF(10);
		GVPort->NeedToDraw=true;
	};
	PGR1.End(0);
	PGR1.Start(1);
	SUSP("HANDLEMOUS");
	ext_DrawEditorInterface(); // by SERGYI (for Facturize editor)
	//HandleMouse(mouseX,mouseY);
	RunPF(9,"GlobalHandleMouse");	
	GlobalHandleMouse();	
	IgnoreProcessMessages=0;
	PGR1.End(1);
	StopPF(9);
	MFix();

	// -- Inserted by Silver 24.12.03
	DrawKangaroo();	
	// --------------------- cut here

	
	SUSP("ENDFAST");
	NeedLoadGamePalette=false;
	StopPF(7);
};
void PreDrawGameProcess();
void PostDrawGameProcess();
void IAmLeft();
extern bool LockPause;
bool ShowStatistics();
extern bool ShowStat;
extern int WaitState;
void CmdEndGame(byte NI,byte state,byte cause);
long StartAboutTime=0;
long FinishAboutTime=0xFFFFFFF;
int NAboutLn=-1;

void ShowAbout(){
	TempWindow TM;
	PushWindow(&TM);
    IRS->SetViewPort(Rct(0,0,RealLx,RealLy));
	int Left=v_ISys.CreditsLeft;
	int Right=v_ISys.CreditsRight;
	int Top=v_ISys.CreditsTop;
	int Bottom=v_ISys.CreditsBottom;
	int Speed=v_ISys.CreditsSpeed;	
	NAboutLn=400;
	int LDis=v_ISys.CreditsLDis;
	RLCFont* TextFon=v_ISys.CreditsFont;	
	
	int y0=Bottom-(GetTickCount()-StartAboutTime)/Speed;	
	
	char pp[100];

	for(int i=0;i<NAboutLn;i++){
		if(y0>Top){
			if(y0<Bottom){
				sprintf(pp,"ABLINE%d",i);
				char* tx=GetTextByID(pp);
				if(tx!=pp){
					ShowString(((Right+Left)-GetRLCStrWidth(tx,TextFon))>>1,y0,tx,TextFon);
				}else{
					if(y0<Top+3*LDis&&FinishAboutTime==0xFFFFFFF){
						FinishAboutTime=GetTickCount();
					}
					break;
				}
			}else{
				break;
			}
		}
		y0+=LDis;
	};	
	ISM->Flush();
	PopWindow(&TM);
};

bool Lobby=0;
void REPLAY();
void LOOSEANDEXITFAST();
void EndGSC_Reporting();
bool ShowPostScreen=0;
void ClearCompstart();
void PlayGame(){
	_dbgPrintState("LoadStage","In PlayGame");
	GSSetup800();
	LoadFog(0);
	InitGame();
	if(GSets.SVOpt.ScreenSizeX!=RealLx)SetGameDisplayModeAnyway(GSets.SVOpt.ScreenSizeX,GSets.SVOpt.ScreenSizeY);
	DrawAllScreen();
	GameNeedToDraw=false;
	GameExit=false;
	MakeMenu=false;
	InGame=true;
	ShowPostScreen=0;
	if(PlayGameMode)StartAboutTime=GetTickCount();
StartPlay:	
	CIMPORT word GetNTribes();
	//LogBattle("^ST_GSTART^ %d (%s)",GetNTribes(),GSets.CGame.);
	LogBattle("^ST_GSTART^ %d",GetNTribes());
	do{		
		_dbgPrintState("LoadStage","In GameCycle t=%d",tmtmt);
		_dbgPrintState("GameStage","PlayGame_001");
		if(RUNMAPEDITOR||RUNUSERMISSION){
			GameExit=true;
		};
		time1=GetRealTime();
		//ProcessMessages();
        //ProcessWindowMessages();
		if(bActive||NPlayers>1){
			GameMode=0;			
			SUSP("Process1");
			ProcessMessages();
			if(bActive)RunPF(1024+4,"FastScreenProcess");
			PREVNSL=NSL[0];
			PREVNSL1=NSL[1];
			_dbgPrintState("GameStage","PlayGame_004");
			FastScreenProcess();
			_dbgPrintState("GameStage","PlayGame_005");
			PREVNSL=-1;
			PREVNSL1=-1;

            RunPF(1024+3,"PreDrawGameProcess");
            PGR1.Start(2);
            _dbgPrintState("GameStage","PlayGame_002");
            PRFUNC(PreDrawGameProcess());
            _dbgPrintState("GameStage","PlayGame_003");
            PGR1.End(2);
            StopPF(1024+3);

            if(SHOWSLIDE){
                int time0=GetRealTime();
                SUSP("REFRESHVIEW");
                RunPF(1024+5,"GSYS.RefreshView()");
                v_MainMenu.ProcessDSinGame();
                GSYS.RefreshView();
                StopPF(1024+5);
                time8=GetRealTime()-time0;
            };

			StopPF(1024+4);
			SUSP("Process2");
			ProcessMessages();
			time1=GetRealTime();
			SUSP("PostDraw");
			PGR1.Start(3);
			_dbgPrintState("GameStage","PlayGame_006");
			PostDrawGameProcess();
			_dbgPrintState("GameStage","PlayGame_007");
			PGR1.End(3);
			time5=GetRealTime()-time1;
			if(MakeMenu){
				switch(MenuType){
				case 1:	
					break;
				case 2: ChooseUnit();
					break;
				case 3: EnterChat();
					break;
				case 4: ProcessHelp();
					break;
				case 5: SelectTexture();
					break;
				};
				MakeMenu=false;
			}else{
				if(GameExit){
					IAmLeft();
					LOOSEANDEXITFAST();
					CloseMPL();
					ShutdownMultiplayer(0);
				};
			};
        }else{
            GameNeedToDraw=true;
            FlipPages();
        }
		//static int time=GetTickCount();
		/*
		if(GetTickCount()-time>5000&&!PlayGameMode){
			void FastSaveRGAME();
			FastSaveRGAME();
			time=GetTickCount();			
		}
		*/
		//
		//v_MainMenu.ProcessDSinGame();
		//
resgame:;
	}while(!GameExit);
	_dbgPrintState("GameStage","PlayGame_exit");
	_dbgPrintState("LoadStage","Outside PlayGame");
	//
	v_MainMenu.RestoreDSinGame();
	//
	InGame=false;

	IAmLeft();
	void StopLoopSounds();
	StopLoopSounds();

	if(EditMapMode)ext_OnEditorEnd();
	else ext_OnGameEnd();

	ACTION_WhenGameEnds();
	Lobby=0;
	PlayGameMode=0;
	if(RecordMode&&!SCENINF.hLib){
		RGAME.Save();
		RecordMode=0;
	};
	EndGSC_Reporting();
	UseGSC_Login=0;
	// MANOWAR GAD
	//RetryVideo=0;
	if(RetryVideo){
		GFILE* f=Gopen("Demo\\demo.run","r");
		if(f){
			int N;
			int z=Gscanf(f,"%d",&N);
			if(z==1){
				int M=GetRndVid(N);
				char NAME[64];
				for(int i=0;i<M;i++)Gscanf(f,"%s",NAME);
				Gscanf(f,"%s",NAME);
				RGAME.ReadStream(NAME);
				if(RGAME.Size){
					PlayGameMode=1;
					RetryVideo=1;
					KeyPressed=0;
				};
			};
			Gclose(f);
			GameExit=0;
			goto StartPlay;
		}else RetryVideo=0;
	};
	IgnoreSlow=true;
	//SlowUnLoadPalette("0\\agew_1.pal");
	int ExRX=RealLx;
	int ExRY=RealLy;
	if(!GSets.LeibzigDemo.Enabled){
		if(RealLx!=1024||RealLy!=768)SetGameDisplayModeAnyway(1024,768);
	}
	/*
	if(ShowStat){
		if(!ShowStatistics()){
			GameExit=false;
			WaitState=1;
			if(RealLx!=ExRX||RealLy!=ExRY)SetGameDisplayModeAnyway(ExRX,ExRY);
			if(ItemChoose==131)REPLAY();
			goto resgame;
		};
		SlowUnLoadPalette("0\\agew_1.pal");
	};
	*/
	ClearScreen();
	IgnoreSlow=false;
	UnLoading();
	GSets.CGame.ArcadeMode=false;
	LOADNATMASK = 0xFFFFFFFF;
	//LoadAllNations(1,0);
	ClearCompstart();
	extern word NPlayers;
	NPlayers=1;
	GameInProgress=false;
	for(int i=0;i<8;i++)NATIONS[i].NMask=1<<i;
};
void DrawAllEditScreen(){
	if(NeedLoadGamePalette){
		if(IgnoreSlow){
			LoadPalette("0\\agew_1.pal");
			LoadFog(0);
		}else{
			SlowLoadPalette("0\\agew_1.pal");
			LoadFog(0);
		};
		CreateMiniMap();
	};
	NeedLoadGamePalette=false;
	PRFUNC(GFieldShow());
	GMiniShow();
	ShowProp();
	ShowAbility();
	DRAW_MAP_TMP();
	CopyToScreen(0,0,RealLx,RSCRSizeY);
};
void DRAW_MAP_TMP();
void FastEditScreenProcess(){
	//RunPF(14);
	PRFUNC(GFieldShow());
	ProcessMessages();
	GMiniShow();	
	extern bool IgnoreProcessMessages;
	IgnoreProcessMessages=1;
	ShowProp();
	ShowAbility();	
	DRAW_MAP_TMP();
	GVPort->NeedToDraw=true;
	MiniVPort->NeedToDraw=true;
	AblVPort->NeedToDraw=true;
	PrpVPort->NeedToDraw=true;	
	GlobalHandleMouse();	
	IgnoreProcessMessages=0;
	MFix();
	GSYS.RefreshView();		
};
void CheckGP();
void EditGame(){
	LoadFog(0);
	GSSetup800();
	if(GSets.SVOpt.ScreenSizeX!=RealLx)SetGameDisplayModeAnyway(GSets.SVOpt.ScreenSizeX,GSets.SVOpt.ScreenSizeY);
	DrawAllEditScreen();
	GameNeedToDraw=false;
	GameExit=false;
	MakeMenu=false;
	InEdit=true;
	PlayGameMode=false;
	do{
		ProcessMessages();
		if(bActive){
			if(RUNUSERMISSION){
				GameExit=true;
			};
			if(RUNMAPEDITOR){
				Load3DMap(USERMISSPATH);
				ACTION_AfterLoadingMapInEditor();
				strcpy(GameName,USERMISSPATH);
				RUNMAPEDITOR=0;
			};
			//SERROR2();
			//RunPF(15);			
			//SERROR1();
			SUSP("Process3");
			ProcessMessages();
			//if(GameNeedToDraw){
			//	DrawAllScreen();
			//	GameNeedToDraw=false;
			//}else 
			SUSP("FastScreen1");
			//			StopPF(15);
			FastScreenProcess();
			
			//DrawAllScreen();
			//SERROR();
			SUSP("Process4");
			ProcessMessages();
			SUSP("PostDraw");
			//RunPF(16);
            PGR1.Start(2);
            PRFUNC(PreDrawGameProcess());
            PGR1.End(2);
			PGR1.Start(3);
			PostDrawGameProcess();
			PGR1.End(3);
            if(SHOWSLIDE){
                int time0=GetRealTime();
                SUSP("REFRESHVIEW");
                RunPF(1024+5,"GSYS.RefreshView()");
                v_MainMenu.ProcessDSinGame();
                GSYS.RefreshView();
                StopPF(1024+5);
                time8=GetRealTime()-time0;
            };
			//			StopPF(16);
			if(MakeMenu){
				switch(MenuType){
				case 1:	
					/*
					if(ProcessGMapEditMainMenu()==mcmExit){
						GameExit=true;
						ext_OnManualExit(MyNation);
					}
					*/
					//v_MainMenu.StartDS("F12game",true);
					break;
				case 2: ChooseUnit();
					break;
				case 3: EnterChat();
					break;
				case 31: ProcessResEdit();
					break;
				case 5: SelectTexture();
					break;
				case 6: EditMiniMap();
					break;
				case 7: //ChooseInterior(0);
					break;
				case 8: //ChooseInterior(1);
					break;
				case 9: //ChooseInterior(2);
					break;
				};
				MakeMenu=false;
			};
		}else{
			Sleep(20);
            FlipPages();
			GameNeedToDraw=true;
		}

		//for(int i=0;i<8;i++)
		//	for(int j=0;j<8;j++)SetXRESRC(i,j,10000000);


		//
		//v_MainMenu.ProcessDSinGame();
		//
	}while(!GameExit);
	//
	v_MainMenu.RestoreDSinGame();
	//
	void StopLoopSounds();
	//StopLoopSounds();
	InEdit=false;
	IgnoreSlow=true;
	SetGameDisplayModeAnyway(1024,768);
	IgnoreSlow=false;
};
//--------------ALL GAME IS IN THIS PROCEDURE!-------------//
BOOL FAR PASCAL EnumAddressCallback1(
									 REFGUID guidDataType,
									 DWORD dwDataSize,
									 LPCVOID lpData,
									 LPVOID lpContext
									 ){
										 if(guidDataType==DPAID_INet){
											 strcpy(IPADDR,(char*)lpData);
											 return false;
										 };
										 return true;
									 };

void ShowFailure(int CreateGame){
	ClearScreen();
	LoadFog(0);
	LoadPalette("0\\agew_1.pal");
	KeyPressed=0;
	bool NEEDDRAW=1;
	do{
		if(bActive){
			if(NEEDDRAW){
				CBar(0,0,RealLx,RealLy,0);
				char* ss[5];
				int NS=0;
				//if(CreateGame){
				char ccx[256];
				if(CreateGame==1){
					ss[0]=GetTextByID("LOBUNC");
					ss[1]=GetTextByID("LOBPR");
					NS=2;
				}else if(CreateGame==0){
					ss[0]=GetTextByID("LOBUNJ1");
					sprintf(ccx,GetTextByID("LOBUNJ5"),IPADDR);
					ss[1]=GetTextByID("LOBUNJ2");
					ss[2]=ccx;
					ss[3]=GetTextByID("LOBPR");
					NS=4;
				}else{
					ss[0]=GetTextByID("LOBUNJ3");
					sprintf(ccx,GetTextByID("LOBUNJ5"),IPADDR);
					ss[1]=GetTextByID("LOBUNJ4");
					ss[2]=ccx;
					ss[3]=GetTextByID("LOBPR");
					NS=4;
				};
				int L=0;
				for(int i=0;i<NS;i++){
					int LL=GetRLCStrWidth(ss[i],&BigWhiteFont);
					if(LL>L)L=LL;
				};
				DrawStdBar(512-32-L/2,380-NS*15-15,512+32+L/2,380+NS*15+15);
				for(int i=0;i<NS;i++)
					ShowString(512-GetRLCStrWidth(ss[i],&BigWhiteFont)/2,380-NS*15+i*30,ss[i],&BigWhiteFont);
				FlipPages();
			};
			NEEDDRAW=0;
		}else NEEDDRAW=true;
		ProcessMessages();
	}while(!KeyPressed);
	KeyPressed=0;
	ClearScreen();
	LoadFog(2);
	LoadPalette("2\\agew_1.pal");
};
void WaitWithMessage(char* Message);
bool AskQuestion2(char* Header,char* Text,char* OKTXT,char* CANCELTXT){
	ItemChoose=-1;
	int MPPLX=450;
	int MPPLY=350;
	int MPLDX=20;
	int MPRDX=20;
	int MPDDY=20;
	int MPUDY=20;
	LocalGP BOR2("Interface\\Bor2");
	DialogsSystem GMM((RealLx-MPPLX)>>1,(RealLy-MPPLY)>>1);


	int DY=0;

	int LX=0;
	int LY=0;
	GMM.addTextButton(NULL,MPPLX/2,20,Header,&BigWhiteFont,&BigWhiteFont,&BigWhiteFont,1);
	DY+=40;
	LY=40;
	TextViewer* TV;
	TV=GMM.addTextViewer(NULL,MPLDX,MPUDY+LY,MPPLX-MPLDX-MPRDX,MPPLY-LY-MPDDY-MPUDY-40,Text,&YellowFont);
	ItemChoose=-1;
	LY+=TV->NLines*TV->SymSize;
	TV->y1=TV->y;
	TextButton* YesBtn=GMM.addTextButton(NULL,MPPLX/4,LY+30,OKTXT,&BigWhiteFont,&BigYellowFont,&BigYellowFont,1);
	YesBtn->UserParam=mcmOk;
	YesBtn->OnUserClick=&MMItemChoose;
	TextButton* NoBtn=GMM.addTextButton(NULL,MPPLX*3/4,LY+30,CANCELTXT,&BigWhiteFont,&BigYellowFont,&BigYellowFont,1);
	NoBtn->UserParam=mcmCancel;
	NoBtn->OnUserClick=&MMItemChoose;
	LY+=70;
	int DDD=(MPPLY-LY)/2;
	GMM.BaseY+=DDD;
	for(int i=0;i<GMM.DSS.GetAmount();i++)if(GMM.DSS[i]){
		GMM.DSS[i]->y+=DDD;
		GMM.DSS[i]->y1+=DDD;
	};
	MPPLY=LY;
	DrawStdBar2(GMM.BaseX,GMM.BaseY,GMM.BaseX+MPPLX,GMM.BaseY+MPPLY,BOR2.GPID);
	ItemChoose=-1;
	do{
		DrawStdBar2(GMM.BaseX,GMM.BaseY,GMM.BaseX+MPPLX,GMM.BaseY+MPPLY,BOR2.GPID);
		ProcessMessages();
		if(KeyPressed){
			if(LastKey==13)ItemChoose=mcmOk;
			else if(LastKey==27)ItemChoose=mcmCancel;
			KeyPressed=false;
		};
		GMM.MarkToDraw();
		GMM.ProcessDialogs();
		GMM.RefreshView();
	}while(ItemChoose==-1);
	UnPress();
	if(ItemChoose==mcmOk){
		ItemChoose=-1;
		return true;
	}else{
		ItemChoose=-1;
		return false;
	};
};
bool NotifyFirewallState(){
	char* txt=GetTextByID("FRW_TXT");
	ResFile F=RRewrite("tmp.txt");
	RBlockWrite(F,txt,strlen(txt));
	RClose(F);
	return AskQuestion2(GetTextByID("FRW_0"),"tmp.txt",GetTextByID("FRW_YES"),GetTextByID("FRW_IGN"));
};
bool DPL_CreatePlayer(LPDIRECTPLAY3A lpDirectPlay3A,
					  LPGUID lpguidSessionInstance, LPDPNAME lpszPlayerName,bool Host);
#ifdef MAKE_PTC
int CheckLobby(){
	return false;
#ifndef _USE3D
	WindX=0;
	WindY=0;
	WindX1=1023;
	WindY1=767;
	WindLx=1024;
	WindLy=768;
#else
	GPS.SetClipArea( 0, 0, 1024, 768 );
#endif

	LPDIRECTPLAYLOBBYA	lpDPlayLobbyA = NULL;
	LPDIRECTPLAYLOBBY2A	lpDPlayLobby2A = NULL;
	if FAILED(DirectPlayLobbyCreate(NULL, &lpDPlayLobbyA, NULL, NULL, 0)) return false;
	// get ANSI DirectPlayLobby2 interface
	HRESULT hr = lpDPlayLobbyA->QueryInterface(IID_IDirectPlayLobby2A, (LPVOID *) &lpDPlayLobby2A);
	if FAILED(hr)return false;
	DWORD Size=0;
	HRESULT dpres=lpDPlayLobby2A->GetConnectionSettings(0,NULL,&Size);
	if(!Size){
		lpDPlayLobby2A->Release();
		return false;
	};
	LPDPLCONNECTION cset=(LPDPLCONNECTION)malloc(Size);
	dpres=lpDPlayLobby2A->GetConnectionSettings(0,cset,&Size);
	if(dpres==DP_OK){
		LPDIRECTPLAY2A lpd2A;
		CreateMultiplaterInterface();
		if(lpDirectPlay3A){
			LoadFog(2);
			LoadPalette("2\\agew_1.pal");
			ShowLoading();
			//assert(0);
			char cc[128]="";
			if(cset->lpSessionDesc->lpszSessionNameA)
				strcpy(cc,cset->lpSessionDesc->lpszSessionNameA);
			cc[8]=0;
			bool BATTL=!strcmp(cc,"[BATTLE]");
			if(BATTL){
				cset->lpSessionDesc->dwMaxPlayers=2;
				cset->lpSessionDesc->dwUser2=1;
			}else{
				cset->lpSessionDesc->dwMaxPlayers=7;
				cset->lpSessionDesc->dwUser2=0;
			};
			dpres=lpDPlayLobby2A->Connect(0,&lpd2A,NULL);
			lpDPlayLobby2A->EnumAddress(&EnumAddressCallback1,cset->lpAddress,cset->dwAddressSize,NULL);

			if(dpres!=DP_OK){
				if(BATTL){
					if(cset->lpSessionDesc->dwFlags==DPLCONNECTION_CREATESESSION)ShowFailure(1);
					else ShowFailure(2);
				}else{
					//lpDPlayLobby2A->EnumAddress(&EnumAddressCallback1,cset->lpAddress,cset->dwAddressSize,NULL);
					ShowFailure(cset->lpSessionDesc->dwFlags==DPLCONNECTION_CREATESESSION);
				};
				lpDPlayLobby2A->Release();
				return false;
			};
			lpDPlayLobby2A->Release();
			dpres= lpd2A->QueryInterface(IID_IDirectPlay3A, (LPVOID*)&lpDirectPlay3A);
			if(dpres!=DP_OK)return false;
			ClearScreen();
			LoadFog(2);
			LoadPalette("2\\agew_1.pal");
			if(dpres==DP_OK){
				if(cset->dwFlags==DPLCONNECTION_CREATESESSION){
					DPL_CreatePlayer(lpDirectPlay3A,&cset->guidSP,cset->lpPlayerName,1);
					if(!BATTL){
						if(!MPL_WaitingGame(true,0))return false;
					}else{
						int BTLID=1;//ProcessWars();
						if(!MPL_WaitingBattleGame(true,1))return false;
					};
					StopConnectionToSession(lpDirectPlay3A);
					StartIGame(0);
				}else{
					DPL_CreatePlayer(lpDirectPlay3A,&cset->guidSP,cset->lpPlayerName,1);
					if(!BATTL){
						if(!MPL_WaitingGame(false,0))return false;
					}else{
						if(!MPL_WaitingBattleGame(false,1))return false;
					};
				};
				return 1;
			}else{
				return false;
			};
		}else{
			return false;
		};
	}else{
		return false;
	};
};
#endif
int prevVid=-1;
int prevVid1=-1;
int prevVid2=-1;
int prevVid3=-1;
int GetRndVid(int N){
	int dt=GetTickCount()&1023;
	for(int i=0;i<dt;i++)rand();
	int cv=-1;
	if(N>4){
		int na=0;
		do{
			int n=rand()%N;
			if(n!=prevVid&&n!=prevVid1&&n!=prevVid2&&n!=prevVid3)cv=n;
			na++;
		}while(cv==-1&&na<100);
	};
	if(cv==-1&&N>3){
		int na=0;
		do{
			int n=rand()%N;
			if(n!=prevVid&&n!=prevVid1&&n!=prevVid2)cv=n;
			na++;
		}while(cv==-1&&na<100);
	};
	if(cv==-1&&N>2){
		int na=0;
		do{
			int n=rand()%N;
			if(n!=prevVid&&n!=prevVid1)cv=n;
			na++;
		}while(cv==-1&&na<100);
	};
	if(cv==-1&&N>1){
		int na=0;
		do{
			int n=rand()%N;
			if(n!=prevVid)cv=n;
			na++;
		}while(cv==-1&&na<100);
	};
	if(cv==-1)cv=0;
	prevVid3=prevVid2;
	prevVid2=prevVid1;
	prevVid1=prevVid;
	prevVid=cv;
	return cv;
};
void PlayVideo();
//#ifdef MAKE_PTC
int vi_MainMenu();
void KeyTestMem();
void AllGame(){
	SetCDVolume(GSets.SVOpt.MusicVolume);
	if(CheckLobby()){
		EditMapMode=0;
		Lobby=1;
	}else{
		_dbgPrintState("LoadStage","Before PlayVideo in AllGame");
		PlayVideo();
		ShowPreview();
	};
stgame:
	int mret;
	if(Lobby)mret=mcmSingle;
	else{
		static bool rm=RUNUSERMISSION;
		RUNUSERMISSION=false;
		_dbgPrintState("LoadStage","Before MainMenu");
		if(rm) mret=processMainMenu();
			else mret=vi_MainMenu();
	}
	if(mret==mcmExit)return;
	if(mret==mcmSingle){
		if(EditMapMode){
			EditGame();
		}
		else PlayGame();
		UnLoading();
		// MANOWAR GAD
		//return;
		goto stgame;
	}else goto stgame;
};
//#else
/*
void AllGame(){
GFILE* f=Gopen("Demo\\demo.run","r");
if(f){
int N;
int z=Gscanf(f,"%d",&N);
if(z==1){
int M=GetRndVid(N);
char NAME[64];
for(int i=0;i<M;i++)Gscanf(f,"%s",NAME);
Gscanf(f,"%s",NAME);
RGAME.ReadStream(NAME);
if(RGAME.Size){
PlayGameMode=1;
ItemChoose=43;
RetryVideo=1;
KeyPressed=0;
};
};
Gclose(f);
PlayGame();
};
};
*/
//#endif
extern int PLNAT[8];
void PrepareGameMedia(byte myid,bool);
void MakeGlobalReport();
void FreeArrays();
void SetupArrays();
void CreateTotalLocking();
void ResearchIslands();
void KillTimers();
void PrepareEditMedia(byte myid,byte sh){
	UnPress();
	ClearLoadMark();
	AddLoadMark("SetupArrays",5);
	AddLoadMark("UnLoading",10);
	AddLoadMark("NewMap",10);
	AddLoadMark("Nations",NNations*15);
	AddLoadMark("CreateTotalLocking",10);
	AddLoadMark("CreateMiniMap",5);
	ShowLoadProgress("SetupArrays",0,2);
	KillTimers();
	FreeArrays();
	ADDSH=sh;
	ShowLoadProgress("SetupArrays",1,2);
	SetupArrays();
	rpos=0;
	setCoor();
	ShowLoadProgress("UnLoading",0,1);		
	UnLoading();
	ShowLoadProgress("NewMap",0,1);	
	NewMap(240<<ADDSH,240<<ADDSH);
	SetMyNation(0);
	for(int i=0;i<8;i++){
		if(!PLNAT[i])
			LoadAllNations(1<<i,i);
		else LoadAllNations(1<<i,i);
		SetMyNation(myid);
	};	
	ItemChoose=mcmSingle;
	//MultiplayerStart=true;
	InitExplosions();
	PrepareToEdit();
	ItemChoose=mcmSingle;
	InitGame();
	MakeGlobalReport();
	EraseAreas();
	rando();//!!
	ShowLoadProgress("CreateTotalLocking",0,1);	
	CreateTotalLocking();
	ResearchIslands();
	if(InitDipForThisMap)InitDipForThisMap();
	
	// First resource set up 
	for(int i=0;i<8;i++)
		for(int j=0;j<8;j++)SetXRESRC(i,j,3000);

	//for(int p=0;p<MaxPointIndex;p++)THMap[p]=50;
	void CreateMiniMap();
	ShowLoadProgress("CreateMiniMap",0,1);	
	CreateMiniMap();
	ShowLoadProgress("CreateMiniMap",1,1);	
	ClearLoadMark();
	ext_OnEditorStart();
};
void PrepareEditMedia(byte myid){
	PrepareEditMedia(myid,1);
}
void RedrawGameBackground(){
	if(GameMode==1){//main menu
		//int mmlx=MainMenuGround.PicPtr[0];
		//int mmly=MainMenuGround.PicPtr[1];
		//MainMenuGround.Draw((RealLx-mmlx)>>1,(RSCRSizeY-mmly)>>1);
		FlipPages();
	}else{
		DrawAllScreen();		
	};
};

extern bool ProtectionMode;
int ProcessComplexQuestion(int Nx,char* Bmp1,byte or1,char* Text1,char* Bmp2,byte or2,char* Text2,char* Quest){
	int Lx=Nx<<6;
	DialogsSystem GMM((RealLx-Lx)>>1,80);
	SQPicture SPU;
	LocalGP CPIC1;
	LocalGP CPIC2;
	int pid1=-1;
	int pid2=-1;
	char cc1[100];
	if(or1>2)or1=0;
	if(or2>2)or2=0;
	if(strcmp(Bmp1,"NONE")){
		sscanf(Bmp1,"%s%d",cc1,&pid1);
		if(pid1!=-1){
			CPIC1.Load(cc1);
		};
	};
	if(strcmp(Bmp2,"NONE")){
		sscanf(Bmp2,"%s%d",cc1,&pid2);
		if(pid2!=-1){
			CPIC2.Load(cc1);
		};
	};

	ResFile RF=RRewrite("tmp1.txt");
	RBlockWrite(RF,Text1,strlen(Text1));
	RClose(RF);
	RF=RRewrite("tmp2.txt");
	RBlockWrite(RF,Text2,strlen(Text2));
	RClose(RF);

	LocalGP LGP;
	LocalGP FONT("interf3\\Fonts\\FontC10");
	//LocalGP FONT1("interf3\\Fonts\\FontC10");
	LocalGP FONTB("interf3\\Fonts\\FontC12");
	LocalGP FONTRB("interf3\\Fonts\\FontC12");

	RLCFont fntb(FONTB.GPID);
	RLCFont fntrb(FONTRB.GPID);
	RLCFont f16x16w(FONT.GPID);
	RLCFont f16x16g(FONT.GPID);
	f16x16w.SetRedColor();
	fntrb.SetRedColor();

	int y0=16;
	int LX1=0;
	int LY1=0;
	int x0=16;
	int plx=0;
	if(pid1!=-1){
		LX1=GPS.GetGPWidth(CPIC1.GPID,pid1);
		LY1=GPS.GetGPHeight(CPIC1.GPID,pid1)+16;
		if(or1==1){
			GMM.addGPPicture(NULL,x0,y0,CPIC1.GPID,pid1);
			x0+=LX1;
			plx=LX1;
		}else if(or1==2){
			GMM.addGPPicture(NULL,Lx-16-LX1,y0,CPIC1.GPID,pid1);
			plx=LX1;
		}else{
			GMM.addGPPicture(NULL,(Lx-LX1)>>1,y0,CPIC1.GPID,pid1);
			y0+=LY1+8;
		};
	};
	TextViewer* TV1;
	TextViewer* TV2;
	TV1=GMM.addTextViewer(NULL,x0,y0,Lx-32-plx,1000,"tmp1.txt",&f16x16w);
	TV1->Enabled=0;
	int dy=TV1->NLines*TV1->SymSize;
	if(or1)y0+=dy>LY1?dy:LY1;
	else y0+=dy;
	int LX2=0;
	int LY2=0;
	x0=16;
	plx=0;
	if(pid2!=-1){
		LX2=GPS.GetGPWidth(CPIC2.GPID,pid2);
		LY2=GPS.GetGPHeight(CPIC2.GPID,pid2)+16;
		if(or2==1){
			GMM.addGPPicture(NULL,x0,y0,CPIC2.GPID,pid2);
			x0+=LX2;
			plx=LX2;
		}else if(or2==2){
			GMM.addGPPicture(NULL,Lx-16-LX2,y0,CPIC2.GPID,pid2);
			plx=LX2;
		}else{
			GMM.addGPPicture(NULL,(Lx-LX1)>>1,y0,CPIC2.GPID,pid2);
			y0+=LY2+8;
		};
	}else y0+=18;;
	TV2=GMM.addTextViewer(NULL,x0,y0,Lx-32-plx,1000,"tmp2.txt",&f16x16g);
	TV2->Enabled=0;
	dy=TV2->NLines*TV2->SymSize;
	if(or1)y0+=dy>LY2?dy:LY2;
	else y0+=dy;
	int Ny=((y0+38)>>6)+1;
	int NTB=1;
	int LL=strlen(Quest);
	for(int j=0;j<LL;j++)if(Quest[j]=='|')NTB++;
	x0=Lx/2/NTB;
	int DX=Lx/NTB;
	char cc[128];
	int pos;
	for(int q=0;q<NTB;q++){
		pos=0;
		char c;
		do{
			c=Quest[0];
			if(c!='|'){
				cc[pos]=c;
				pos++;
				cc[pos]=0;
			};
			Quest++;
		}while(c!=0&&c!='|');
		TextButton* TB=GMM.addTextButton(NULL,x0+q*DX,Ny*64-33,GetTextByID(cc),&fntrb,&fntb,&fntb,1);
		TB->OnUserClick=&MMItemChoose;
		TB->UserParam=q;
		if(PlayGameMode){
			TB->Enabled=0;
			if(q==CurrentAnswer)TB->PassiveFont=&fntrb;
		};
	};
	int DDY=(RealLy-64*Ny)/2-80;
	GMM.BaseY+=DDY;
	for(int p=0;p<GMM.DSS.GetAmount();p++){
		if(GMM.DSS[p]&&GMM.DSS[p]->Visible){
			GMM.DSS[p]->y+=DDY;
			GMM.DSS[p]->y1+=DDY;
		};
	};
	ItemChoose=-1;
	DrawPaperPanelShadow((RealLx-Lx)>>1,80+DDY-32,((RealLx-Lx)>>1)+Lx,80+DDY+Ny*64+20);
	int T0=GetTickCount();
	do{
		ProcessMessages();
		//if(KeyPressed&&(LastKey==13||LastKey==27)){
		//	KeyPressed=false;
		//	ItemChoose=mcmOk;
		//};
		DrawPaperPanel((RealLx-Lx)>>1,80+DDY-32,((RealLx-Lx)>>1)+Lx,80+DDY+Ny*64+20);
		GMM.MarkToDraw();
		GMM.ProcessDialogs();
		GMM.RefreshView();
		if(PlayGameMode&&GetTickCount()-T0>5000)ItemChoose=CurrentAnswer;
	}while(ItemChoose==-1);
	ContinueGame=true;
	UnPress();
	return ItemChoose;
};

bool AskMissionQuestion(char* Bmp,char* Text){

	int MPPLX=GETV("MPPLX");
	int MPPLY=GETV("MPPLY");
	int MPLDX=GETV("MPLDX");
	int MPRDX=GETV("MPRDX");
	int MPDDY=GETV("MPDDY");
	int MPUDY=GETV("MPDDY");

	/*
	//char* BmpName=GETS("@MISSPANEL");
	DialogsSystem GMM(smapx+(nsmaplx<<4)-(MPPLX>>1),smapy+(mul3(nsmaply)<<2)-(MPPLY>>1));
	//SQPicture SP(BmpName);

	SQPicture SPU;
	SPU.PicPtr=NULL;
	//if(strcmp(Bmp,"NONE")){
	//	SPU.LoadPicture(Bmp);
	//};

	ResFile RF=RRewrite("tmp.txt");
	RBlockWrite(RF,Text,strlen(Text));
	RClose(RF);
	LocalGP LGP;
	LocalGP FONT("interf3\\Fonts\\FontC10");
	LocalGP FONTB("interf3\\Fonts\\FontC12");
	//LocalGP FONTRB("vinrBig");
	RLCFont f16x16w(FONT.GPID);//"interf3\\Fonts\\FontC10");
	RLCFont fntb(FONTB.GPID);
	RLCFont fntrb(FONTB.GPID);
	fntrb.SetRedColor();
	LocalGP CPIC;
	int pid=-1;
	char cc1[100];
	if(strcmp(Bmp,"NONE")){
	sscanf(Bmp,"%s%d",cc1,&pid);
	if(pid!=-1){
	CPIC.Load(cc1);
	};
	};	
	int DY=0;
	if(SPU.PicPtr){
	GMM.addPicture(NULL,0,0,&SPU,&SPU,&SPU);
	DY=SPU.GetLy();
	};
	int LX=0;
	int LY=0;
	if(pid!=-1){
	LX=GPS.GetGPWidth(CPIC.GPID,pid);
	LY=GPS.GetGPHeight(CPIC.GPID,pid);
	GMM.addGPPicture(NULL,(MPPLX-LX)>>1,MPUDY,CPIC.GPID,pid);
	};
	TextViewer* TV;
	TV=GMM.addTextViewer(NULL,MPLDX,MPUDY+LY,MPPLX-MPLDX-MPRDX,MPPLY-LY-MPDDY-MPUDY-40,"tmp.txt",&f16x16w);
	ItemChoose=-1;
	*/

	// Font
	RLCFont* FWindow = &fon16y5;
	RLCFont* FButton = &fon18y5;
	RLCFont* FWhite = &fon18w;
	RLCFont* FYellow = &fon18y3;
	RLCFont* FText = &fon18y3;
	RLCFont* FHint = &fon13y2;

	LocalGP MSG("interf2\\messages");
	LocalGP BTN("interf2\\button2");
	LocalGP SCROL("Interf3\\elements\\scroll2");
	LocalGP LN("interf2\\line2");

	int WinID=1;

	int W=GPS.GetGPWidth(MSG.GPID,WinID*2);
	int H=GPS.GetGPHeight(MSG.GPID,WinID*2);
	int x0=(RealLx-W)/2;
	int y0=(RealLy-H)/2;

	int TitleY=34;

	int BX0=75;
	int BX1=307;
	int BY=521;

	int PY=75;

	int TX0=55;
	int TX1=520;
	int TY1=495;

	DialogsSystem GMM(x0,y0);

	GPS.ShowGP(x0,y0,MSG.GPID,WinID*2+1,0);
	GMM.addGPPicture(NULL,0,0,MSG.GPID,WinID*2);

	SQPicture SPU;
	LocalGP CPIC;
	int pid=-1;
	char cc1[100];
	if(strcmp(Bmp,"NONE")){
		sscanf(Bmp,"%s%d",cc1,&pid);
		if(pid!=-1){
			CPIC.Load(cc1);
		};
	};

	ResFile RF=RRewrite("tmp.txt");
	RBlockWrite(RF,Text,strlen(Text));
	RClose(RF);
	LocalGP LGP;

	// title text
	GMM.addTextButton(NULL,W>>1,TitleY,GetTextByID("INTF_MESSAGE"),FWindow,FWindow,FWindow,1);

	// picture
	int LX=0;
	int LY=0;
	if(pid!=-1){
		LX=GPS.GetGPWidth(CPIC.GPID,pid);
		LY=GPS.GetGPHeight(CPIC.GPID,pid)+16;
		GMM.addGPPicture(NULL,(W-LX)>>1,PY,CPIC.GPID,pid);
	};

	// line delimiter
	int DW=GPS.GetGPWidth(LN.GPID,0);
	int DH=GPS.GetGPHeight(LN.GPID,0);
	GP_Button* Line=GMM.addGP_Button(NULL,(W-DW)>>1,PY+LY,LN.GPID,0,0);

	// message text	
	int SBW=GPS.GetGPWidth(SCROL.GPID,0);
	int TY0=PY+LY+10+DH;
	VScrollBar* SB=GMM.addNewGP_VScrollBar(NULL,TX1-SBW,TY0,TY1-TY0,1,0,SCROL.GPID,0);
	TextViewer* TV=GMM.addTextViewer(NULL,TX0,TY0,TX1-SBW-5-TX0,TY1-TY0,"tmp.txt",FText);
	TV->AssignScroll(SB);
	TV->SymSize=21;
	TV->PageSize=div(TY1-TY0,TV->SymSize).quot;
	//TV->

	// butons
	//TextButton* YesBtn=GMM.addTextButton(NULL,MPPLX/4,441,GetTextByID("@MSYES"),&fntrb,&fntb,&fntb,1);
	GP_TextButton* YesBtn=GMM.addGP_TextButton(NULL,BX0,BY,GetTextByID("@MSOK"),BTN.GPID,3,FButton,FButton);
	YesBtn->UserParam=mcmOk;
	YesBtn->OnUserClick=&MMItemChoose;
	if(PlayGameMode){
		//if(CurrentAnswer)YesBtn->PassiveFont=&fntrb;
		YesBtn->Enabled=1;
	};

	//TextButton* NoBtn=GMM.addTextButton(NULL,MPPLX*3/4,441,GetTextByID("@MSNO"),&fntrb,&fntb,&fntb,1);
	GP_TextButton* NoBtn=GMM.addGP_TextButton(NULL,BX1,BY,GetTextByID("@MSNO"),BTN.GPID,3,FButton,FButton);
	NoBtn->UserParam=mcmCancel;
	NoBtn->OnUserClick=&MMItemChoose;
	if(PlayGameMode){
		//if(!CurrentAnswer)NoBtn->PassiveFont=&fntrb;
		NoBtn->Enabled=1;
	};

	ItemChoose=-1;
	//DrawPaperPanelShadow(GMM.BaseX,GMM.BaseY,GMM.BaseX+MPPLX,GMM.BaseY+MPPLY);
	int T0=GetTickCount();
	do{
		//DrawPaperPanel(GMM.BaseX,GMM.BaseY,GMM.BaseX+MPPLX,GMM.BaseY+MPPLY);
		ProcessMessages();
		if(KeyPressed){
			if(LastKey==13)ItemChoose=mcmOk;
			else if(LastKey==27)ItemChoose=mcmCancel;
			KeyPressed=false;
		};
		GMM.MarkToDraw();
		GMM.ProcessDialogs();
		GMM.RefreshView();
		if(PlayGameMode&&GetTickCount()-T0>5000){
			ItemChoose=CurrentAnswer==0?mcmCancel:mcmOk;
		};
	}while(ItemChoose==-1);

	ContinueGame=true;
	UnPress();
	if(ItemChoose==mcmOk)return true;
	else return false;
};
//----------------------MiniMap editor interface------------------
SQPicture trans1u;
SQPicture trans1p;
SQPicture trans2u;
SQPicture trans2p;
SQPicture trans3u;
SQPicture trans3p;
SQPicture trans4u;
SQPicture trans4p;
SQPicture trans5u;
SQPicture trans5p;
SQPicture trans6u;
SQPicture trans6p;
SQPicture pen1u;
SQPicture pen1p;
SQPicture pen2u;
SQPicture pen2p;
SQPicture x480;
SQPicture getc_p;
SQPicture getc_u;
SQPicture btflu;
SQPicture btflu0;
SQPicture btrough;
SQPicture btrough0;
SQPicture btsoft;
SQPicture btsoft0;
SQPicture btless;
SQPicture btless0;
SQPicture prepu;
SQPicture prepp;
byte* higrad=NULL;
int grsize;
#ifdef MAKE_PTC
SFLB_DLLEXPORT SFLB_LoadMEDButtons(){

};
#endif
//-------------------MiniMap images----------------
int HiLx;
int HiLy;
int HiStepX;
int HiStepY;
int winX;
int winY;
int winX1;
int winY1;
byte* HiMap;
byte* HiMap1;

void SetupHiMap(){
	if(RealLx>=1024){
		HiLx=496;//248;
		HiLy=496;
	}else{
		HiLx=248;
		HiLy=248;
	};
	HiStepX=div(msx<<(8+5),HiLx).quot;
	HiStepY=div(msy<<(8+5),HiLy).quot;
	HiMap=znew(byte,HiLx*HiLy);
	HiMap1=znew(byte,HiLx*HiLy);
};
void DeleteHiMap(){
	free(HiMap);
	free(HiMap1);
};
byte GetPaletteColor(int r,int g,int b);
void CreateHiMap(){
	if(!higrad){
		grsize=24;
		higrad=znew(byte,grsize);
		for(int i=0;i<grsize;i++)higrad[i]=GetPaletteColor(i*255/grsize,i*255/grsize,i*255/grsize);
	};
	int mpx=256;
	int mpy=256;
	for(int iy=0;iy<HiLy;iy++){
		for(int ix=0;ix<HiLx;ix++){
			int Hig=(GetHeight(mpx>>8,mpy>>8)+256)>>4;
			if(Hig<0)Hig=0;
			if(Hig<grsize)Hig=higrad[Hig];else Hig=higrad[grsize-1];
			HiMap[ix+iy*HiLx]=Hig;
			mpx+=HiStepX;
		};
		mpx=256;
		mpy+=HiStepY;
	};
};
//----------------------LOADING THE FILE--------------------//
void CreateFilesList(char* Mask,ListBox* LB){
	WIN32_FIND_DATA FD;
	HANDLE H=FindFirstFile(Mask,&FD);
	if(H!=INVALID_HANDLE_VALUE){
		do{
			LB->AddItem(FD.cFileName,0);
		}while(FindNextFile(H,&FD));
		FindClose(H);
	};
};


//------------------TRANSFORMATIONS LIBRARY-----------------
int xc,yc;//center of the transformation
static int HiMode=0;
static int tpr1=256;//Height
static int tpr2=0;  //R1
static int tpr3=256;//R2
static int tpr4=512;//frquency
static int tpr5=512;//Amplitude
static int tpr6=512;//Phase
static int FractAmpl=0;
static int FractScale=128;
static int HiBrush=0;
static int HiRound=0;
int getR(double a,int r){
	int nn=tpr4>>6;
	double tm=double(GetRealTime())/1000;
	return int(r*(1.0+(sin(a*(nn)+tpr6/256+tm)+
		sin(a*(nn+1)+tpr6/256+tm*1.5)+
		sin(a*(nn+2)+tpr6/256-tm/2)+
		sin(a*(nn+3)+tpr6/256+tm*1.4)+
		sin(a*(nn+4)+tpr6/256-tm*1.5)+
		sin(a*(nn+5)+tpr6/256+tm*1.3)+
		sin(a*(nn+6)+tpr6/256-tm*1.1)+
		sin(a*(nn+7)+tpr6/256+tm*0.8)+
		sin(a*(nn+8)+tpr6/256+tm*0.7)+
		sin(a*(nn+9)+tpr6/256+tm*0.6))*double(tpr5)/1000.0));
};
void Pix(int x,int y,byte c){
	if(x>=winX&&y>=winY&&x<winX1&&y<winY1){
		(HiMap1+x-winX+(y-winY)*HiLx)[0]=c;
		//((byte*)(int(ScreenPtr)+x+y*SCRSizeX))[0]=c;
		//((byte*)(int(RealScreenPtr)+x+y*RSCRSizeX))[0]=c;
	};
};
void Circ(int x,int y,int r,int n,byte c){
	r=(r*HiLx)/496;
	if(HiRound==1){
		for(int i=0;i<n;i++){
			double alpha=3.1415*2*i/n;
			int r2=getR(alpha,r);
			Pix(int(x+r2*sin(alpha)),int(y+r2*cos(alpha)),c);
		};
	}else{
		double time=double(GetRealTime())/1000;
		for(int i=0;i<n;i++){
			double alpha=3.1415*2*i/n;
			Pix(int(x+r*sin(alpha)),int(y+r*cos(alpha)),c);
		};
	};
};
void Circ2(int x,int y,int r,DWORD Color,bool Curved){	
	int n=r*3+1;
	if(Curved){
		int xp=0;
		int yp=0;
		for(int i=0;i<=n;i++){
			double alpha=3.1415*2*i/n;
			int r2=getR(alpha,r);
			int xx=int(x+r2*sin(alpha));
			int yy=int(y+r2*cos(alpha));
			if(i>0)GPS.DrawLine(xp,yp,xx,yy,Color);
			xp=xx;
			yp=yy;
		};
	}else{
		int xp=0;
		int yp=0;
		for(int i=0;i<=n;i++){
			double alpha=3.1415*2*i/n;
			int xx=int(x+r*sin(alpha));
			int yy=int(y+r*cos(alpha));
			if(i)GPS.DrawLine(xp,yp,xx,yy,Color);
            xp=xx;
			yp=yy;
		};
	};
};
int func1(int r){
	if(r<tpr2)return 1024;
	if(r>tpr2+tpr3)return 0;
	int dr=r-tpr2-(tpr3>>1);
	if(dr>0)return div((tpr2+tpr3-r)*(tpr2+tpr3-r)<<11,tpr3*tpr3).quot;
	else return 1024-div((r-tpr2)*(r-tpr2)<<11,tpr3*tpr3).quot;
};

int func2(int r){
	if(r<tpr2)return 1024;
	if(r>tpr2+tpr3)return 0;
	int dr=r-tpr2-tpr3;
	return div((dr*dr)<<10,tpr3*tpr3).quot;
};
int func3(int r){
	if(r<tpr2)return 1024;
	if(r>tpr2+tpr3)return 0;
	int dr=tpr2-r;
	return 1024-div((dr*dr)<<10,tpr3*tpr3).quot;
};
int func4(int r){
	if(r<tpr2)return 1024;
	if(r>tpr2+tpr3)return 0;
	int dr=r-tpr2;
	return div(dr<<10,tpr3).quot;
};
int func5(int r){
	if(r<tpr2)return 1024;
	if(r>tpr2+tpr3)return 0;
	int dr=r-tpr2-(tpr3>>1);
	if(dr>0)return div((tpr2+tpr3-r)*(tpr2+tpr3-r)<<11,tpr3*tpr3).quot;
	else return 1024-div((r-tpr2)*(r-tpr2)<<11,tpr3*tpr3).quot;
};
typedef int fpar(int);
int UniTrans(int x,int y,int Hi,fpar* ff1){
	if(!tpr3)tpr3=1;
	int pk;
	int r=int(sqrt((x-xc)*(x-xc)+(y-yc)*(y-yc)));
	if(HiRound==1){
		int tp2=tpr2;
		int tp3=tpr3;
		double ang=atan2(x-xc,y-yc);
		tpr2=getR(ang,tpr2);
		tpr3=getR(ang,tpr3);
		pk=ff1(r);
		tpr2=tp2;
		tpr3=tp3;
	}else pk=ff1(r);
	int GetFractalVal(int x,int y);
	int FV=GetFractalVal((x+y)*FractScale/30,(y-x)*FractScale/30);
	int HH=tpr1*(256+(FV-256)*FractAmpl/256)/256;
	switch(HiMode){
	case 0:
		return Hi+(((HH-Hi)*pk)>>10);
	case 1:{
		int za=(GetHeight(x-32,y)+GetHeight(x+32,y)+
			GetHeight(x,y-32)+GetHeight(x,y+32))>>2;
		return div(tpr1*za+(Hi<<9),512+tpr1).quot;
		   };
	case 2:
		return Hi+((tpr1*pk)>>15);
	case 3:
		pk=Hi-((tpr1*pk)>>15);
		if(pk<0)pk=0;
		return pk;	
	default:
		return Hi;
	};
};
int Transf1(int x,int y,int Hi){
	return UniTrans(x,y,Hi,&func1);
};
byte* smoothMask=NULL;
int TransfTex(int x,int y,int Hi){
	int VT=tpr1;
	tpr1=255;
	int V=UniTrans(x,y,0,&func1);
	tpr1=VT;
	if(V>128){
		int vx=x>>5;
		int vy=vx&1?(y-16)>>5:y>>5;
		int V=vx+vy*VertInLine;
		SetTextureInPointUsingPreset(V,-1);
		smoothMask[V]=1;
	}
	return Hi;
};
int Transf2(int x,int y,int Hi){
	return UniTrans(x,y,Hi,&func2);
};
int Transf3(int x,int y,int Hi){
	return UniTrans(x,y,Hi,&func3);
};
int Transf4(int x,int y,int Hi){
	return UniTrans(x,y,Hi,&func4);
};
int Transf5(int x,int y,int Hi){
	return UniTrans(x,y,Hi,&func5);
};
//----------------------------------------------------------
void CopyToOffScreen(int zx,int zy,
					 int srLx,int srLy,
					 byte* data);
void CopyToRealScreen(int zx,int zy,
					  int srLx,int srLy,
					  byte* data);
void CreateMiniMap();
void ClearRender();
int mul3(int);
bool PrepareMap(SimpleDialog* SD){
	for(int x=0;x<VertInLine;x++){
		for(int y=MaxTH-2;y>0;y--){
			int vert=x+y*VertInLine;
			int hi0=(mul3(GetTriY(vert))>>2)-THMap[vert];
			int hi1=(mul3(GetTriY(vert+VertInLine))>>2)-THMap[vert+VertInLine];
			if(hi1-hi0<0)THMap[vert]+=hi0-hi1;
		};
	};
	ItemChoose=-2;
	return true;
};
bool RandomMap(SimpleDialog* SD){
	for(int x=0;x<VertInLine;x++){
		for(int y=MaxTH-2;y>0;y--){
			int vert=x+y*VertInLine;
			THMap[vert]+=((rando()&3)<<1)-3;
		};
	};
	ItemChoose=-2;
	return true;
};
bool LoadBitmapLikeGrayscale(char* Name,int* Lx,int* Ly,byte** res);
int GetResVal(byte* res,int LX,int LY,int RLX,int RLY,int x,int y);
bool LoadClick(SimpleDialog* SD){
	char BMPS[128];
	if(ProcessLoadingFile("UserBitmaps\\*.bmp",BMPS,3)){
		byte* DATA;
		char NAME[128];
		sprintf(NAME,"UserBitmaps\\%s",BMPS);
		int LX,LY;
		if(LoadBitmapLikeGrayscale(NAME,&LX,&LY,&DATA)){
			int maxx=7680<<ADDSH;
			int maxv=(MaxTH+1)*MaxTH;
			int h0=GetResVal(DATA,LX,LY,maxx,maxx,0,0);
			for(int i=0;i<maxv;i++){
				int x=GetTriX(i);
				int y=GetTriY(i);
				THMap[i]=(GetResVal(DATA,LX,LY,maxx,maxx,x,y)-128)*2;
			};
		};
	};
	ItemChoose=-2;
	return true;
};
int MapGP=-1;
int ScrollGP=-1;
void ProcessTerrainEditor();
extern bool ShowHeightMode;

void EditMiniMap2(){
	
}
void EditMiniMap(){
	
};
//---------------------------------------------------------------//
//-------------/    Choosing elements of interface     \---------//
//-------------\         trees,stones,....             /---------//
//---------------------------------------------------------------//
int InteriorType;
word TreeItems[256];
word NTreeItems;
extern int BlobMode;


int GetTreeItem(){
	return TreeItems[(rando()*NTreeItems)>>15];
};
//Multiplayer Interface

//-------------Multiplayer settings--------------

bool WaitingGame(bool Host){
	char STRI[128];
	SQPicture   MPL    ("multi_m0.bpx");
	InputBox*   MNAME  [8];
	ComboBox*   MNATION[8];
	ColoredBar* MCOLOR [8];
	TextButton* MGRP   [8];
	RLCFont     FontA(GETS("@MNMENUACTF"));
	RLCFont     FontP(GETS("@MNMENUPASF"));
	DialogsSystem MPLAY(0,0);
	MPLAY.addPicture(NULL,0,0,&MPL,&MPL,&MPL);
	char NAMY[16];
	int NameX=GETV("MINAMEX");
	int NatX=GETV("MINATX");
	int ClrX=GETV("MICLRX");
	int CmdX=GETV("MIGRPX");
	int NameLx=GETV("MINAMELX");
	int StrLy=GETV("MINAMELY");
	int ClrLx=GETV("MICLRLX");
	int NatLx=GETV("MINATLX");
	InputBox* IBB1=MPLAY.addInputBox(NULL,0,0,STRI,64,300,32,&FontP,&FontP);
	LoadPalette("2w.pal");
	for(int i=0;i<8;i++){
		sprintf(NAMY,"MIY%d",i+1);
		int y1=GETV(NAMY);
		MNAME[i]=MPLAY.addInputBox(NULL,NameX,y1,GSets.CGame.PL_INFO[i].name,16,NameLx,StrLy,&FontP,&FontA);
		MNATION[i]=MPLAY.addComboBox(NULL,NatX+8,y1+3,NatLx-16,StrLy-3,20,0,3,&FontA,&FontP,NULL);
		for(int j=0;j<GlobalAI.NAi;j++){
			MNATION[i]->AddLine(GlobalAI.Ai[j].Message);
		};
		MNATION[i]->FontDx=4;
		MNATION[i]->FontDy=2;
		MCOLOR[i]=MPLAY.addColoredBar(ClrX,y1,ClrLx,StrLy,0x80);
		MCOLOR[i]->OnMouseOver=&ColorMouseOver;
		sprintf(NAMY,"%d",i);
		MGRP[i]=MPLAY.addTextButton(NULL,CmdX,y1,NAMY,&FontA,&FontP,&FontP,0);


	};
	TextButton* OkBtn;
	if(Host)OkBtn=MPLAY.addTextButton(NULL,GETV("MIOKX"),GETV("MIOKY"),GETS("@MIHOSTOK"),&FontA,&FontP,&FontP,1);
	else OkBtn=MPLAY.addTextButton(NULL,GETV("MIOKX"),GETV("MIOKY"),GETS("@MIJOINOK"),&FontA,&FontP,&FontP,1);
	TextButton* CancelBtn=MPLAY.addTextButton(NULL,GETV("MICANCELX"),GETV("MICANCELY"),GETS("@MICANCEL"),&FontA,&FontP,&FontP,1);
	CancelBtn->UserParam=mcmCancel;
	CancelBtn->OnUserClick=&MMItemChoose;
	ItemChoose=-1;
	OkBtn->UserParam=mcmOk;
	OkBtn->OnUserClick=&MMItemChoose;
	PIEnumeratePlayers(GSets.CGame.PL_INFO,0);
	byte CUSED=0;
	for(int i=0;i<NPlayers;i++){
		if(GSets.CGame.PL_INFO[i].PlayerID!=MyDPID){
			CUSED|=(1<<GSets.CGame.PL_INFO[i].ColorID);
		};
	};
	int cc1=0;
	byte mss1=1;
	while(CUSED&mss1){
		cc1++;
		mss1<<=1;
	};
	for(int i=0;i<NPlayers;i++){
		if(GSets.CGame.PL_INFO[i].PlayerID==MyDPID){
			GSets.CGame.PL_INFO[i].ColorID=cc1;
			MCOLOR[i]->color=cc1+0x80;
			if(Host)GSets.CGame.PL_INFO[i].Ready=1;
			else GSets.CGame.PL_INFO[i].Ready=0;
		}else{
			GSets.CGame.PL_INFO[i].Ready=0;
		};
	};
	do{
		tmm1=GetRealTime();
		ProcessMessages();
		tmm1=GetRealTime()-tmm1;
		tmm2=GetRealTime();
		AnalyseMessages();
		tmm2=GetRealTime()-tmm2;
		MPLAY.MarkToDraw();
		MPLAY.ProcessDialogs();
		tmm3=GetRealTime();
		MPLAY.RefreshView();
		tmm3=GetRealTime()-tmm3;
		sprintf(STRI,"%d %d %d %d",COUN,tmm1,tmm2,tmm3);
		for(int i=0;i<8;i++){
			if(i<NPlayers){
				if(GSets.CGame.PL_INFO[i].PlayerID==MyDPID){
					GSets.CGame.PL_INFO[i].NationID=MNATION[i]->CurLine;
					GSets.CGame.PL_INFO[i].ColorID=MCOLOR[i]->color-0x80;
					lpDirectPlay3A->SetPlayerData(MyDPID,(void*)&GSets.CGame.PL_INFO[i].NationID,4,DPSET_REMOTE);
					DPNAME dpName;
					ZeroMemory(&dpName, sizeof(DPNAME));
					dpName.dwSize = sizeof(DPNAME);
					dpName.lpszShortNameA = MNAME[i]->Str;
					dpName.lpszLongNameA = NULL;
					lpDirectPlay3A->SetPlayerName(MyDPID,&dpName,DPSET_REMOTE);
					MNATION[i]->Enabled=true;
					MCOLOR[i]->Enabled=true;
					MGRP[i]->Enabled=true;
				}else{
					MNATION[i]->Enabled=false;
					MCOLOR[i]->Enabled=false;
					MGRP[i]->Enabled=false;
				};
				MNATION[i]->Visible=true;
				MCOLOR[i]->Visible=true;
				MGRP[i]->Visible=true;
			}else{
				MNATION[i]->Enabled=false;
				MCOLOR[i]->Enabled=false;
				MGRP[i]->Enabled=false;
				MNATION[i]->Visible=false;
				MCOLOR[i]->Visible=false;
				MGRP[i]->Visible=false;
			};
		};
		PIEnumeratePlayers(GSets.CGame.PL_INFO,0);
		for(int i=0;i<8;i++){
			if(i<NPlayers){
				MNATION[i]->CurLine=GSets.CGame.PL_INFO[i].NationID;
				MCOLOR[i]->color=GSets.CGame.PL_INFO[i].ColorID+0x80;
			};
		};
		for(int i=0;i<NPlayers;i++){
			if(GSets.CGame.PL_INFO[i].PlayerID==MyDPID&&ItemChoose==mcmOk&&!Host){
				GSets.CGame.PL_INFO[i].Ready=1;
			};
		};
		bool GMREADY=true;
		for(int i=0;i<NPlayers;i++){
			if(GSets.CGame.PL_INFO[i].PlayerID!=MyDPID&&!GSets.CGame.PL_INFO[i].Ready)GMREADY=false;
		};
		int MSS1=0;
		for(int i=0;i<NPlayers;i++){
			byte MSS2=1<<GSets.CGame.PL_INFO[i].ColorID;
			if(MSS1&MSS2)GMREADY=false;
			MSS1|=MSS2;
			if(!GSets.CGame.PL_INFO[i].Ready)GMREADY=false;
		};
		if(Host){
			OkBtn->Enabled=GMREADY;
		};
		//checking readyness
		if(ItemChoose==mcmOk&&!Host)ItemChoose=-1;
	}while(ItemChoose==-1&&PlayerMenuMode==1);
	return true;
};
bool WaitingHostGame(int ID){
	if(ID)MPL_WaitingBattleGame(true,ID-1);
	else MPL_WaitingGame(true,0);

	if(ItemChoose==mcmCancel){
		CloseMPL();
		ItemChoose=-1;
	}else{
		if(ItemChoose==mcmOk){
			//ShowLoading();
			StopConnectionToSession(lpDirectPlay3A);
			StartIGame(0);
			return true;
		};
	};
	return false;
};
bool WaitingJoinGame(int ID){
	PlayerMenuMode=1;
	if(ID)MPL_WaitingBattleGame(false,ID-1);
	else MPL_WaitingGame(false,0);
	if(ItemChoose==mcmCancel){
		CloseMPL();
		ItemChoose=-1;
	}else{
		//ShowLoading();
		if(!PlayerMenuMode)return true;
	};
	return false;
};
extern bool NotInGame;
void CenterScreen();
extern bool NoWinner;
void CreateInfoMap();
extern word COMPSTART[8];
DLLEXPORT
void StartAI(byte Nat,char* Name,int Land,int Money,int ResOnMap,int Difficulty);
extern int RM_LandType;
extern int RM_Resstart;
extern int RM_Restot;
void FreeArrays();
void SetupArrays();
extern byte* RivDir;
extern byte XVIIIState;
void PerformNewUpgrade(Nation* NT,int UIndex,OneObject* OB);
extern int RunDataSize;
extern byte RunData[2048];
CEXPORT void StartAIEx(byte Nat,char* Name,int Land,int Money,int ResOnMap,int Difficulty);
void CleanRec();
void LoadMessagesFromFile(char* Name);
extern byte BaloonState;
extern byte CannonState;
extern byte XVIIIState;
extern byte CaptState;
extern byte SaveState;
void GetOptionsFromMap(char* Name){
	if(Name[0]=='R'&&Name[1]=='N'&&Name[3]==' '){
		int v1,v2,v3,ADD_PARAM;
		char ccc[32];
		int z=sscanf(Name,"%s%x%x%x%d",ccc,&v1,&v2,&v3,&ADD_PARAM);
		if(z==5){
			BaloonState=(ADD_PARAM/10)%10;
			CannonState=(ADD_PARAM/100)%10;
			XVIIIState=(ADD_PARAM/10000)%10;
			CaptState=(ADD_PARAM/100000)%10;
			SaveState=(ADD_PARAM/1000000)%10;
		};
	};
};
extern char AI_Log[256];
extern char CurrentGameLogDir[256];
void WriteAI_Comment(char* name,int idx,char* mask,...){
	if(AI_Log[0]&&CurrentGameLogDir[0]){
		char cc2[512];
		sprintf(cc2,"%s\\%s%d.dat",CurrentGameLogDir,name,idx);
		FILE* F=fopen(cc2,"a");
		if(F){
			va_list va;
			va_start(va,mask);
			vfprintf(F,mask,va);
			va_end(va);
			fclose(F);
		};
	};
};
void WriteResources(char* name,int idx,int* res){
	WriteAI_Comment(name,idx,"W:%7d G:%7d S:%7d F:%7d I:%7d C:%7d",res[0],res[1],res[2],res[3],res[4],res[5]);
};
void WriteResources(char* name,int idx,word* res){
	WriteAI_Comment(name,idx,"W:%7d G:%7d S:%7d F:%7d I:%7d C:%7d",res[0],res[1],res[2],res[3],res[4],res[5]);
};
void WriteUpgradeAction(byte NI,NewUpgrade* NU){
	WriteAI_Comment("",NI,"%6d: UPGRADE %s (%s)\n",tmtmt,NU->Name,NU->Message);
	WriteAI_Comment("",NI,"COST  :");
	WriteResources("",NI,NU->Cost);
	WriteAI_Comment("",NI,"\nTOTAL :");
	int RS[6];
	for(int j=0;j<6;j++)RS[j]=XRESRC(NI,j);
	WriteResources("",NI,RS);
	WriteAI_Comment("",NI,"\nPERCENT:");
	for(int j=0;j<6;j++)if(RS[j])RS[j]=NU->Cost[j]*100/RS[j];
	WriteResources("",NI,RS);
	WriteAI_Comment("",NI,"\n\n");
};
void WriteUnitAction(byte NI,word NIndex,int* R0,int* R1){
	GeneralObject* GO=NATIONS[NI].Mon[NIndex];
	NewMonster* NM=GO->newMons;
	int NU=CITY[NI].UnitAmount[NIndex];
	int CC[6];
	for(int i=0;i<6;i++)CC[i]=R1[i]-R0[i];
	if(NM->Building){
		WriteAI_Comment("",NI,"%6d: BUILDING %s [now:%d] (%s)\n",tmtmt,GO->MonsterID,NU,GO->Message);
		WriteAI_Comment("",NI,"COST   :");
		WriteResources("",NI,CC);
		WriteAI_Comment("",NI,"\nTOTAL  :");
		int RS[6];
		//for(int j=0;j<6;j++)RS[j]=XRESRC(NI,j);
		WriteResources("",NI,R0);
		WriteAI_Comment("",NI,"\nPERCENT:");
		for(int j=0;j<6;j++)if(R0[j])RS[j]=CC[j]*100/R0[j];
		WriteResources("",NI,RS);
		WriteAI_Comment("",NI,"\n\n");
	}else if(NU%10==0){
		WriteAI_Comment("",NI,"%6d: UNITS_GROW %s [now:%d] (%s)\n",tmtmt,GO->MonsterID,NU,GO->Message);
		WriteAI_Comment("",NI,"COST  :");
		WriteResources("",NI,CC);
		WriteAI_Comment("",NI,"\nTOTAL  :");
		int RS[6];
		//for(int j=0;j<6;j++)RS[j]=XRESRC(NI,j);
		WriteResources("",NI,R0);
		WriteAI_Comment("",NI,"\nPERCENT:");
		for(int j=0;j<6;j++)if(R0[j])RS[j]=CC[j]*100/R0[j];
		WriteResources("",NI,RS);
		WriteAI_Comment("",NI,"\n\n");
	};
};
/*
void WriteGameSnapshot(byte NI){
int TotPS=0;
int PS_MineG=0;
int PS_MineI=0;
int PS_MineC=0;
int PS_MineS=0;
int PS_Field=0;
int PS_Build=0;
int PS_Trees=0;
int PS_Idle=0;
};*/
DLLEXPORT void ChangeUnitColor(int Index,byte DstNI);
CIMPORT word GetTribeID(OneObject* OB);
CIMPORT byte GetTribeOwner(word Index);
CIMPORT bool SetTribeOwner(word TribeID, byte Owner);
//
void TestMonNames(){
	int N=NATIONS[0].NMon;
	for(int i=0;i<N;i++){
		assert(NATIONS[0].Mon[i]->Message);
	}
}

void KillTimers();
void TestMem();
void ShowLoadProgress(char* Mark, int v, int vMax);
void DeleteAbilities(OneObject* OB);
bool LoadAsIs=false;
void PrepareGameMedia(byte myid,bool SaveNR,bool DisableDLL){
	_dbgPrintState("LoadStage","PrepareGameMedia: PerformCheckCD");
	void PerformCheckCD();
	PerformCheckCD();
	_dbgPrintState("LoadStage","PrepareGameMedia: ClearLoadMark");
	ClearLoadMark();
	AddLoadMark("SetupArrays",5);	
	AddLoadMark("NewMap",10);
	AddLoadMark("UnLoading",10);
	AddLoadMark("MapUnLoading",5);
	AddLoadMark("Nations",NNations*10);
	if(!strncmp(GSets.CGame.cgi_CurrentMap,"RN0 ",4)){
		AddLoadMark("SetupMLayers",3);
		AddLoadMark("SetupExLandscape",5);		
		AddLoadMark("CreatingLayers",50);
		AddLoadMark("Relief",90);
		AddLoadMark("Painting",180);
		AddLoadMark("Textures smoothing",20);
		AddLoadMark("Edges smoothing",5);
		AddLoadMark("CreateLightMap",5);
		AddLoadMark("CreateTotalLocking",15);
		AddLoadMark("ResetGroundCache",5);
		AddLoadMark("CreateMiniMap",5);		
	}else{
		AddLoadMark("LoadMap",70);	
		AddLoadMark("CreateTotalLocking",10);
		AddLoadMark("CreateMiniMap",5);
		AddLoadMark("CreateAreas",40);
		AddLoadMark("CreateLightMap",10);
		AddLoadMark("ResetGroundCache",10);		
	}
	ShowLoadProgress("SetupArrays",0,2);
	KillTimers();
	RandomMapDesc RMP;
	RMP.Load("Terrain\\Res.dat");
	extern int Start_rpos;
	rpos=Start_rpos;
	CleanRec();
	setCoor();
	//FreeArrays();
	if(!(strstr(GSets.CGame.cgi_CurrentMap,".SAV")||strstr(GSets.CGame.cgi_CurrentMap,".sav"))){
		if(GSets.CGame.cgi_CurrentMap[0]=='R'&&GSets.CGame.cgi_CurrentMap[1]=='N'&&GSets.CGame.cgi_CurrentMap[3]==' '){
			ADDSH=GSets.CGame.cgi_CurrentMap[2]-'0'+1;
		}else{
		};
	}else{
		///NoWinner=1;
	};

	ShowLoadProgress("SetupArrays",0,1);
	_dbgPrintState("LoadStage","PrepareGameMedia: SetupArrays");
	SetupArrays();
	ShowLoadProgress("NewMap",0,1);
	_dbgPrintState("LoadStage","PrepareGameMedia: NewMap");
	NewMap(240<<ADDSH,240<<ADDSH);
	byte NRTB[8];
	memcpy(NRTB,GSets.CGame.cgi_NatRefTBL,8);
	ShowLoadProgress("UnLoading",0,1);
	_dbgPrintState("LoadStage","PrepareGameMedia: UnLoading");
	UnLoading();
	if(SaveNR)memcpy(GSets.CGame.cgi_NatRefTBL,NRTB,8);
	SetMyNation(0);
	SetMyNation(GSets.CGame.PL_INFO[myid].ColorID);
	PlayerInfo PL_INFO[8];
	memcpy(&PL_INFO,GSets.CGame.PL_INFO,sizeof PL_INFO);
	if(NPlayers>1){
		//NatRefTBL assignment
		int NC=0;
		int cc[8];
		for(int i=0;i<7;i++)GSets.CGame.cgi_NatRefTBL[i]=0xFF;
		for(int i=0;i<7;i++)if(i<NPlayers||COMPSTART[i]){
			byte CL=GSets.CGame.PL_INFO[i].ColorID;
			if(GSets.CGame.cgi_NatRefTBL[CL]==0xFF){
				GSets.CGame.cgi_NatRefTBL[CL]=CL;				
			}else{
                cc[NC]=i;
				NC++;
			}
		}
		for(int j=0;j<NC;j++){
			int i=cc[j];
			byte CL=GSets.CGame.PL_INFO[i].ColorID;
			for(int q=0;q<7;q++){
				if(GSets.CGame.cgi_NatRefTBL[q]==0xFF){
					GSets.CGame.cgi_NatRefTBL[q]=CL;
					GSets.CGame.PL_INFO[i].ColorID=q;
					if(i==myid){						
						SetMyNation(q);
					}
					break;
				}
			}
		}
		for(int i=0;i<7;i++){
			if(GSets.CGame.cgi_NatRefTBL[i]==0xFF)GSets.CGame.cgi_NatRefTBL[i]=i;
		}
	}
	for(int i=0;i<8;i++){		
		CITY[i].CreateCity(i);
	//	LoadAllNations(1<<i/*GSets.CGame.PL_INFO[i].ColorID*/,i/*GSets.CGame.PL_INFO[i].ColorID*/);		
	};
	_dbgPrintState("LoadStage","PrepareGameMedia: PrepareToGame");
	PrepareToGame();
	int RD_VERSION=101;
	//RUN DATA
	RunMethod=0xFFFFFFFE;
	RunData[0]=101;//version of the run data
	RunData[1]=byte(dwVersion);//version of the game
	RunData[2]=MyNation;
	RunDataSize=3;
	memcpy(RunData+RunDataSize,PL_INFO,sizeof PL_INFO);
	RunDataSize+=sizeof PL_INFO;
	memcpy(RunData+RunDataSize,COMPSTART,sizeof COMPSTART);
	RunDataSize+=sizeof COMPSTART;
	memcpy(RunData+RunDataSize,&RM_LandType,4);
	RunDataSize+=4;
	memcpy(RunData+RunDataSize,&RM_Resstart,4);
	RunDataSize+=4;
	memcpy(RunData+RunDataSize,&RM_Restot,4);
	RunDataSize+=4;
	memcpy(RunData+RunDataSize,NRTB,8);
	RunDataSize+=8;
	assert(RunDataSize<2048);
	//----------
	//Aliances
	_dbgPrintState("LoadStage","PrepareGameMedia: Aliances");	
	for(int i=NPlayers;i<7;i++){
		if(COMPSTART[i]){
			word W=COMPSTART[i];
			int Nat=W>>11;
			int Color=W&15;
			int Diff=(W>>8)&7;
			if(Diff>0)Diff--;
			int Team=(W>>4)&15;
			GSets.CGame.PL_INFO[i].ColorID=Color;
			GSets.CGame.PL_INFO[i].NationID=Nat;
			GSets.CGame.PL_INFO[i].GroupID=Team;
			GSets.CGame.PL_INFO[i].MapStyle=Diff;
			strcpy(GSets.CGame.PL_INFO[i].name,GlobalAI.Diff[Diff].Name);
		}else{
			GSets.CGame.PL_INFO[i].MapStyle=0;
		};
	};
	for(int i=0;i<7;i++)if(GSets.CGame.PL_INFO[i].name[0]||i<NPlayers||GSets.CGame.PL_INFO[i].MapStyle){
		if(GSets.CGame.PL_INFO[i].GroupID==0)NATIONS[GSets.CGame.cgi_NatRefTBL[GSets.CGame.PL_INFO[i].ColorID]].NMask=1<<GSets.CGame.cgi_NatRefTBL[GSets.CGame.PL_INFO[i].ColorID];
		else{
			int gid=GSets.CGame.PL_INFO[i].GroupID;
			byte mas=0;
			for(int j=0;j<7;j++)if(GSets.CGame.PL_INFO[j].name[0]||j<NPlayers||GSets.CGame.PL_INFO[j].MapStyle){
				if(GSets.CGame.PL_INFO[j].GroupID==gid)mas|=1<<GSets.CGame.cgi_NatRefTBL[GSets.CGame.PL_INFO[j].ColorID];
			};
			NATIONS[GSets.CGame.cgi_NatRefTBL[GSets.CGame.PL_INFO[i].ColorID]].NMask=mas;
		}
	}
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB){
			OB->NMask=NATIONS[OB->NNUM].NMask;
		}
	}
	CheapMode=false;
	ItemChoose=mcmSingle;
	MultiplayerStart=true;
	InitExplosions();
	InitGame();
	NotInGame=true;
	if(!(strstr(GSets.CGame.cgi_CurrentMap,".SAV")||strstr(GSets.CGame.cgi_CurrentMap,".sav"))){
		byte RRR[8];
		memcpy(RRR,GSets.CGame.cgi_NatRefTBL,8);
		byte NMASKS[8];
		for(int i=0;i<8;i++)NMASKS[i]=NATIONS[i].NMask;
		TestMonNames();
		_dbgPrintState("LoadStage","PrepareGameMedia: Load3DMap");
		Load3DMap(GSets.CGame.cgi_CurrentMap);
		ACTION_AfterLoadingMapInGame();		
		//
		if(InitDipForThisMap)InitDipForThisMap();
		//
		TestMonNames();
		_dbgPrintState("LoadStage","PrepareGameMedia: GetOptionsFromMap");
		GetOptionsFromMap(GSets.CGame.cgi_CurrentMap);
		for(int i=0;i<8;i++)NATIONS[i].NMask=NMASKS[i];
		if(LoadAsIs)MOptions.RandomizePlayersPositions=false;
		if(MapScenaryDLL[0]){
			if(!DisableDLL){
				char ccc[128];
				char cc3[128];
				strcpy(ccc,MapScenaryDLL);
				char* cc1=strstr(ccc,".");
				if(cc1)strcpy(cc1,".txt");
				strcpy(cc3,MapScenaryDLL);
				cc1=strstr(cc3,".");
				if(cc1)strcpy(cc1,".pchar()");
				FILE* F=fopen(cc1,"r");
				if(F){
					fclose(F);
					LoadMessagesFromFile(cc3);
				};
				SCENINF.Load(MapScenaryDLL,ccc);//"Battles\\Battles.txt");
			};
		}else{
            extern int Start_rpos;
            rpos=Start_rpos;
            if(NPlayers<=1 && !PlayGameMode){
                rpos=GetTickCount()%8191;
            }

			// change start position
			if(MOptions.RandomizePlayersPositions){				
				int clrarray[8];
				int nclr=0;
				DWORD CMASK=0;
				int DECODER[8]={-1,-1,-1,-1,-1,-1,-1,-1};
				for(int i=0;i<MAXOBJECT;i++){
					OneObject* OB=Group[i];
					if(OB){
						if(!MOptions.Players.Player[OB->NNUM].DisableInMultiplayer){
							CMASK|=1<<OB->NNUM;
						}else{
							DECODER[OB->NNUM]=OB->NNUM;
						}
					}
				}
				for(int i=0;i<7;i++)if(CMASK&(1<<i)){
                    clrarray[nclr]=i;
					nclr++;
				}
				if(nclr>1){
					for(int p=0;p<100;p++){
                        int p1=rando()%nclr;
						int p2=rando()%nclr;
						if(p1!=p2)swap(clrarray[p1],clrarray[p2]);
					}
				}				
				bool colordone[8]={0,0,0,0,0,0,0,0};
				for(int i=0;i<7;i++)if(i<NPlayers||COMPSTART[i]){
					if(nclr){
						int pc=GSets.CGame.cgi_NatRefTBL[GSets.CGame.PL_INFO[i].ColorID];
						if(!colordone[pc]){
							colordone[pc]=true;
							DECODER[clrarray[nclr-1]]=pc;
							nclr--;
						}
					}
				}
				for(int i=0;i<MAXOBJECT;i++){
					OneObject* OB=Group[i];
					if(OB){
						if(!(OB->NNUM==7||MOptions.Players.Player[OB->NNUM].DisableInMultiplayer)){
							int dc=DECODER[OB->NNUM];
							if(dc==-1){
								EraseObject(OB);
								NATIONS[OB->NNUM].ThereWasUnit=false;
							}else{
								if(dc!=OB->NNUM){
									NATIONS[OB->NNUM].NProduced[OB->NIndex]--;
									ChangeUnitColor(OB->Index,dc);
									NATIONS[OB->NNUM].NProduced[OB->NIndex]++;
								}
							}
						}
						// transform tribes
						word tid=GetTribeID(OB);
						if(tid!=0xFFFF){
							byte ni=GetTribeOwner(tid);
							SetTribeOwner(tid,DECODER[ni]);
						}
					}
				}
				void CenterScreen();
				CenterScreen();
			}

			//realisation of [AutoTransferUnitsToNearestTeam], LockTeam required
			for(int i=0;i<7;i++){
				if(MOptions.Players.Player[i].AutoTransferUnitsToNearestTeam&&MOptions.Players.Player[i].LockTeam){
					int team=MOptions.Players.Player[i].Team;
                    //checking if human or AI player have this color
					int bestdest=-1;//index op human/ai player with the same team
					bool have=false;
					for(int j=0;j<7;j++)if(j<NPlayers||COMPSTART[j]){
						int CC=GSets.CGame.cgi_NatRefTBL[GSets.CGame.PL_INFO[j].ColorID];
						if(CC==i){
                            have=true;
							break;
						}
						if(team==GSets.CGame.PL_INFO[j].GroupID){
							int id=GSets.CGame.cgi_NatRefTBL[GSets.CGame.PL_INFO[j].ColorID];
							if(j<NPlayers){
                                bestdest=id;
							}else{
                                if(bestdest==-1)bestdest=id;
							}
						}
					}
					if(bestdest!=-1&&!have){
                        //repainting units and giving resources
						for(int z=0;z<MAXOBJECT;z++){
							OneObject* OB=Group[z];
							if(OB&&OB->NNUM==i){
								if(bestdest!=OB->NNUM){	
									if(OB->BrigadeID!=0xFFFF){
										void ChangeBrigNation(word SBID, byte SrcNat, byte DstNat);
										ChangeBrigNation(OB->BrigadeID,OB->NNUM,bestdest);
									}else{	
										ChangeUnitColor(OB->Index,bestdest);
									}
								}
								// transform tribes
								word tid=GetTribeID(OB);
								if(tid!=0xFFFF){
									byte ni=GetTribeOwner(tid);
									SetTribeOwner(tid,bestdest);
								}
							}
						}
						//giving resounrces
						for(int q=0;q<6;q++){
							AddXRESRC(bestdest,q,XRESRC(i,q));
						}
					}
				}
			}
			
			// Converting peasants or other units
			
			bool GetHeroUnitType(int RoleID, byte NationID, int& NewUnitType);
			bool GetNewUnitType(int OldUnitType,byte NewNationID, int& NewUnitType);
			OneObject* TransformUnitType(OneObject* OB,int DestType);
			bool GetHero(OneObject* OB);

			_dbgPrintState("LoadStage","PrepareGameMedia: Converting peasants");
			if(GSets.CGame.UnitTransform&&NPlayers>0) for(int j=0;j<MAXOBJECT;j++){
				OneObject* OB=Group[j];
				if(OB&&!OB->Sdoxlo){ //&&OB->newMons->Peasant
					bool hero=GetHero(OB);
					for(int i=0;i<NPlayers||COMPSTART[i];i++){
						PlayerInfo* I=GSets.CGame.PL_INFO+i;
						if(OB->NNUM==I->ColorID){
							int t;
							if(hero){
								//int role=GSets.RoomParams.Players[i]->Hero;
								int role=I->Hero;
								if(GetHeroUnitType(role,I->NationID,t) && !MOptions.DoNotChangeHeroes){
									TransformUnitType(OB,t);

									// delete netral creeps
									itr_UnitsInRadius.Create(OB->RealX>>4,OB->RealY>>4,400);
									while(OneObject* Creep=itr_UnitsInRadius.Next()){
										if(Creep->NNUM==7){
											EraseObject(Creep);
										}
									}

									break;
								}
							}else{
								if(GetNewUnitType(OB->NIndex,I->NationID,t)){
									TransformUnitType(OB,t);
									break;
								}
							}
						}
					}
				};
			};

			// select Hero on start map
			//byte MyNI=GSets.CGame.cgi_NatRefTBL[MyNation];
			//for(byte NI=0;NI<7;NI++){
			//	OneObject* OB=vHeroButtons.GetObject(NI,0);
			//	if(OB){
			//		pSelected Sel(NI);
			//		pImSelected ImSel(NI);
			//		Sel.Clear();
			//		ImSel.Clear();
			//		pUnit U(OB);
			//		Sel += U;
			//		ImSel += U;
			//		if(NI==MyNI)
			//			SetScreenCenterToXY(OB->RealX>>4,OB->RealY>>4);
			//	}
			//}
			void SelectHeroOnStartMap();
			SelectHeroOnStartMap();

			RPGRTSNat = GSets.CGame.cgi_NatRefTBL[MyNation];
			RPGRTSLand = RMP.STY[RM_LandType].AI_Style;
			RPGRTSMoney = RM_Resstart;
			RPGRTSResOnMap = RM_Restot;
			RPGRTSDifficulty = GSets.CGame.PL_INFO[MyNation].MapStyle;
			
			for(int i=0;i<MAXOBJECT;i++){
				OneObject* OB=Group[i];
				if(OB){
					OB->NMask=NATIONS[OB->NNUM].NMask;
				}
			}

			// start ai
			_dbgPrintState("LoadStage","PrepareGameMedia: start ai");
			for(int i=1;i<7;i++){	//for(i=NPlayers;i<7;i++){
				if(COMPSTART[i]){
					int Diff=GSets.CGame.PL_INFO[i].MapStyle;
					int c=GSets.CGame.PL_INFO[i].ColorID;
					MapPlayerInfo* P=MOptions.Players.Player+c;
					if(P&&P->DisableStrategicAI){
						//CITY[c].Difficulty=COMPSTART[i]-1000;
					}else{
						int Nat=-1;
						for(int j=0;j<MAXOBJECT;j++){
							OneObject* OB=Group[j];
							if(OB&&OB->NNUM==c&&!OB->Sdoxlo&&(OB->newMons->Peasant||OB->NewBuilding||GetHero(OB))){
								Nat=OB->Ref.General->NatID;
								break;
							};
						};
						if(Nat!=-1){
							char ccx[48];
							char* NtID=NatsIDS[Nat];
							sprintf(ccx,"%s.%d",NtID,0);
							char* nm=ccx;
							char* GetBestAI(int NI,char* mapname);
							char* N=GetBestAI(GSets.CGame.PL_INFO[i].ColorID,GSets.CGame.cgi_CurrentMap);
							if(N)nm=N;
							StartAIEx(GSets.CGame.PL_INFO[i].ColorID,nm,RMP.STY[RM_LandType].AI_Style,RM_Resstart,RM_Restot,Diff);
						};
					}
				};
			};
		};
		memcpy(GSets.CGame.cgi_NatRefTBL,RRR,8);
	}else{
		//NoWinner=1;
		void LoadNetworkGame(byte NI,int ID,char* NAME);
		LoadNetworkGame(0,0,GSets.CGame.cgi_CurrentMap);
		//
		//if(InitDipForThisMap)InitDipForThisMap(); may be not need
		//
	};
	void ReadMoraleData();
	ReadMoraleData();
	GNFO.Clear();
	GNFO.Setup();	
	//HideFlags();
	NotInGame=false;
	_dbgPrintState("LoadStage","PrepareGameMedia: CenterScreen");
	CenterScreen();
	CreateInfoMap();
	InitExplosions();
	City::EnumAllUnits();	
	if(XVIIIState==2){
		int N=NATIONS[0].NUpgrades;
		for(int i=0;i<N;i++){
			for(int NI=0;NI<7;NI++){
				NewUpgrade* NU=NATIONS[NI].UPGRADE[i];
				if((!NU->Done)&&NU->Options&8){
					PerformNewUpgrade(NATIONS+NI,i,NULL);
					NU->Done=1;
					NU->PermanentEnabled=0;
				};
			};
		};
	};
	/*for(i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&!(EngSettings.AllyNetralBuildings&&OB->NNUM==7&&OB->NewBuilding)){
			OB->NMask=NATIONS[OB->NNUM].NMask;
		};
	};*/
	
	//if(InitDipForThisMap)InitDipForThisMap(); old position
	if(AI_Log[0]){
		SYSTEMTIME ST;
		GetLocalTime(&ST);
		char CNAME[256]="";
		DWORD sz=255;
		GetComputerName(CNAME,&sz);
		sprintf(CurrentGameLogDir,"%s%s_%d.%d.%d_%d_%d",AI_Log,CNAME,ST.wDay,ST.wMonth,ST.wYear,ST.wHour,ST.wMinute);
		if(!CreateDirectory(CurrentGameLogDir,0)){
			CurrentGameLogDir[0]=0;
		}else{

		};
	}else{
		CurrentGameLogDir[0]=0;
	};
	for(int i=0;i<7;i++)if(i<NPlayers){
		int nid=GSets.CGame.PL_INFO[i].NationID;
		if(nid>=0&&nid<GlobalAI.NAi){
			WriteAI_Comment("strart_conditions",0,"PLAYER: %3d %12.12s %12.12s\n",GSets.CGame.PL_INFO[i].ColorID,GSets.CGame.PL_INFO[i].name,GlobalAI.Ai[nid].NationID);
		};
	}else if(NATIONS[i].AI_Enabled){
		int nid=GSets.CGame.PL_INFO[i].NationID;
		if(nid>=0&&nid<GlobalAI.NAi){
			WriteAI_Comment("strart_conditions",0,"AI    : %3d %12.12s %12.12s\n",GSets.CGame.PL_INFO[i].ColorID,GSets.CGame.PL_INFO[i].name,GlobalAI.Ai[nid].NationID);
		};
	};
	//diplomasy starting
	void HideDipDialog();
	CEXPORT void ActivateDipDialog(char* request);		
	if(CurrentSinBattle==-1)ActivateDipDialog("DS|cnlist");
	HideDipDialog();
	GSets.RoomParams.InitBeforeGame();
	ACTION_WhenGameStarts();
	ClearLoadMark();
	void ResetAnimationQuality(bool LowQ);
	if(MOptions.VictoryRule.Get()){		
		ResetAnimationQuality(true);
	}else{
		ResetAnimationQuality(GSets.AnimQuality);
	}
	GSets.CGame.SpecialGameSpeed=0;
	for(int i=0;i<EngSettings.SpecifySpeedForMaps.GetAmount();i++){
        int n=EngSettings.SpecifySpeedForMaps[i]->MapsList.GetAmount();
		for(int j=0;j<n;j++){
			char* s=EngSettings.SpecifySpeedForMaps[i]->MapsList[j]->MapName.pchar();
			if(s&&!stricmp(s,GSets.CGame.cgi_CurrentMap)){
				GSets.CGame.SpecialGameSpeed=EngSettings.SpecifySpeedForMaps[i]->Speed;
			}
		}
	}
	rpos=Start_rpos;
	//  added by Silver, 25/08/2004
	_dbgPrintState("LoadStage","PrepareGameMedia: ITerra");
    ITerra->ResetDrawQueue();
    ITerra->InvalidateAABB();
    ITerra->InvalidateTexture();
    ITerra->InvalidateGeometry();
    IWater->Reset();
    
	_dbgPrintState("LoadStage","PrepareGameMedia: finished");
};
void PrepareGameMedia(byte myid,bool SaveNR){
	PrepareGameMedia(myid,SaveNR,0);	
};
//----------------------RANDOM MAP GENERATING DIALOG------------------
void RandomMapDialog(char* Result){
	DialogsSystem DSY((RealLx-352)>>1,(RealLy-352)>>1);
	RLCFont FONT1("rf32y.rlc");
	RLCFont FONT2(GETS("@SMPASSIVE"));
	RLCFont FONT3(GETS("@SMACTIVE"));
	DSY.addTextButton(NULL,352>>1,20,GETS("@RMIHEADER"),&FONT1,&FONT1,&FONT1,1);
	int y=60;
	int x0=20;
	int x=195;
	RandomMapDesc RMP;
	RMP.Load("Terrain\\Res.dat");
	DSY.addTextButton(NULL,x0,y,GETS("@RMISTYLE"),&FONT2,&FONT2,&FONT2,0);
	ComboBox* CBSTYLE=DSY.addComboBox(NULL,x,y,144,25,20,0,255,&FONT2,&FONT3,NULL);
	for(int p=0;p<RMP.NSTY;p++){
		CBSTYLE->AddLine(RMP.STY[p].Name);
	};
	y+=40;
	DSY.addTextButton(NULL,x0,y,GETS("@RMIMOUNTAINS"),&FONT2,&FONT2,&FONT2,0);
	ComboBox* CBMOUNT=DSY.addComboBox(NULL,x,y,144,25,20,0,255,&FONT2,&FONT3,NULL);
	for(int p=0;p<RMP.NRelief;p++){
		CBMOUNT->AddLine(RMP.Relief[p]);
	};
	y+=40;
	int CurSty=0;
	DSY.addTextButton(NULL,x0,y,GETS("@RMIPLAYERS"),&FONT2,&FONT2,&FONT2,0);
	ComboBox* CBPLAY=DSY.addComboBox(NULL,x,y,144,25,20,0,255,&FONT2,&FONT3,NULL);
	for(int p=0;p<RMP.STY[CurSty].NPl;p++){
		char ccc[16];
		sprintf(ccc," %d",RMP.STY[CurSty].Players[p].NPlayers);
		CBPLAY->AddLine(ccc);
	};
	y+=40;
	DSY.addTextButton(NULL,x0,y,GETS("@RMIRESSTART"),&FONT2,&FONT2,&FONT2,0);
	ComboBox* CBRESST=DSY.addComboBox(NULL,x,y,144,25,20,0,255,&FONT2,&FONT3,NULL);
	for(int p=0;p<RMP.NRES;p++){
		CBRESST->AddLine(RMP.RES[p].Name);
	};
	y+=40;
	DSY.addTextButton(NULL,x0,y,GETS("@RMIRESTOT"),&FONT2,&FONT2,&FONT2,0);
	ComboBox* CBRESTOT=DSY.addComboBox(NULL,x,y,144,25,20,0,255,&FONT2,&FONT3,NULL);
	for(int p=0;p<RMP.NMINES;p++){
		CBRESTOT->AddLine(RMP.MINES[p].Name);
	};
	y+=40;
	DSY.addTextButton(NULL,x0,y,"Code:",&FONT2,&FONT2,&FONT2,0);
	char ccc[128];
	sprintf(ccc,"%d",GetRealTime()&32767);
	InputBox* IB=DSY.addInputBox(NULL,x,y,ccc,20,144,28,&FONT2,&FONT2);
	y+=40;
	TextButton* OkBtn=DSY.addTextButton(NULL,10+164/2,315,"OK",&FONT2,&FONT3,&FONT3,1);
	OkBtn->UserParam=mcmOk;
	OkBtn->OnUserClick=&MMItemChoose;
	TextButton* CancelBtn=DSY.addTextButton(NULL,180+164/2,315,"??????",&FONT2,&FONT3,&FONT3,1);
	CancelBtn->UserParam=mcmCancel;
	CancelBtn->OnUserClick=&MMItemChoose;

	ItemChoose=-1;
	do{
		ProcessMessages();
		if(KeyPressed&&(LastKey==13||LastKey==27)){
			KeyPressed=false;
			ItemChoose=mcmOk;
		};
		DrawStdBar(DSY.BaseX,DSY.BaseY,DSY.BaseX+351,DSY.BaseY+351);
		DSY.MarkToDraw();
		DSY.ProcessDialogs();
		DSY.RefreshView();
		if(CurSty!=CBSTYLE->CurLine){
			CurSty=CBSTYLE->CurLine;
			CBPLAY->Clear();
			for(int p=0;p<RMP.STY[CurSty].NPl;p++){
				char ccc[16];	
				sprintf(ccc," %d",RMP.STY[CurSty].Players[p].NPlayers);
				CBPLAY->AddLine(ccc);
			};
		};
	}while(ItemChoose==-1);
	UnPress();
	if(ItemChoose==mcmOk){
		int r=GetRealTime()&65535;
		sscanf(ccc,"%d",&r);
		int q=CBSTYLE->CurLine+(CBPLAY->CurLine<<4)+(CBMOUNT->CurLine<<8)+(CBRESST->CurLine<<12)+
			(CBRESTOT->CurLine<<16);
		sprintf(Result,"RND %X %X 11111111.m3d",r,q);
	}else Result[0]=0;
};
int RM_LandType =0;
int RM_Resstart =0;
int RM_Restot   =0;
void EnterRandomParams(){
	DialogsSystem DSY((RealLx-64*6)>>1,(RealLy-64*5)>>1);
	LocalGP vinB("interf3\\Fonts\\FontC12");
	LocalGP vin("interf3\\Fonts\\FontC10");
	RLCFont fnti(vinB.GPID);
	RLCFont fntir(vinB.GPID);
	fntir.SetRedColor();
	RLCFont fntis(vin.GPID);
	RLCFont fntisr(vin.GPID);
	fntisr.SetRedColor();
	DSY.addTextButton(NULL,32*6,10,GetTextByID("AIPARM"),&BigYellowFont,&BigYellowFont,&BigYellowFont,1);
	int y=85;
	int x0=10;
	int x=200;
	RandomMapDesc RMP;
	RMP.Load("Terrain\\Res.dat");
	DSY.addTextButton(NULL,x0,y,GETS("@RMISTYLE"),&fntis,&fntis,&fntis,0);
	ComboBox* CBSTYLE=DSY.addComboBox(NULL,x,y,164,25,20,0x3B,255,&fntisr,&fntis,NULL);
	for(int p=0;p<RMP.NSTY;p++){
		CBSTYLE->AddLine(RMP.STY[p].Name);
	};
	CBSTYLE->CurLine=RM_LandType;
	y+=50;
	DSY.addTextButton(NULL,x0,y,GETS("@RMIRESSTART"),&fntis,&fntis,&fntis,0);
	ComboBox* CBRESST=DSY.addComboBox(NULL,x,y,164,25,20,0x3B,255,&fntisr,&fntis,NULL);
	for(int p=0;p<RMP.NRES;p++){
		CBRESST->AddLine(RMP.RES[p].Name);
	};
	CBRESST->CurLine=RM_Resstart;
	y+=45;
	DSY.addTextButton(NULL,x0,y,GETS("@RMIRESTOT"),&fntis,&fntis,&fntis,0);
	ComboBox* CBRESTOT=DSY.addComboBox(NULL,x,y,164,25,20,0x3B,255,&fntisr,&fntis,NULL);
	for(int p=0;p<RMP.NMINES;p++){
		CBRESTOT->AddLine(RMP.MINES[p].Name);
	};
	CBRESTOT->CurLine=RM_Restot;
	y+=55;
	TextButton* OkBtn=DSY.addTextButton(NULL,92-5,y,"OK",&fntir,&fnti,&fnti,1);
	OkBtn->UserParam=mcmOk;
	OkBtn->OnUserClick=&MMItemChoose;
	TextButton* CancelBtn=DSY.addTextButton(NULL,180+82,y,"Cancel",&fntir,&fnti,&fnti,1);
	CancelBtn->UserParam=mcmCancel;
	CancelBtn->OnUserClick=&MMItemChoose;

	ItemChoose=-1;
	do{
		int x0=(RealLx-64*6)>>1;
		int y0=(RealLy-64*4)>>1;
		DrawPaperPanel(x0,y0,x0+64*6,y0+64*4);
		ProcessMessages();
		DSY.MarkToDraw();
		DSY.ProcessDialogs();
		DSY.RefreshView();
		if(KeyPressed){
			if(LastKey==13)ItemChoose=mcmOk;
			if(LastKey==27)ItemChoose=mcmCancel;
			KeyPressed=false;
		};
	}while(ItemChoose==-1);
	if(ItemChoose==mcmOk){
		RM_LandType=CBSTYLE->CurLine;
		RM_Resstart=CBRESST->CurLine;
		RM_Restot=CBRESTOT->CurLine;
	};
	UnPress();
};
void RandomMapDialog1(char* Result,int NPlayers,char* Nats){

};
int CurrentCampagin=-1;
int CurrentMission=-1;
int CurrentSinBattle=-1;
//int NSinBattle;

struct OneAddMission{
	char Title[128];
	char Map[128];
	char Preview[128];
	char Bitmap[128];
};
class AddMissionsPack{
public:
	OneAddMission* Pack;
	int NMiss;
	AddMissionsPack();
	~AddMissionsPack();
};
bool ReadWinString(GFILE* F,char* STR,int Max);
AddMissionsPack::AddMissionsPack(){
	Pack=NULL;
	NMiss=0;
	WIN32_FIND_DATA SR;
	HANDLE H=FindFirstFile("UserMissions\\*.add",&SR);
	if(H!=INVALID_HANDLE_VALUE){
		do{
			char ccc[128];
			sprintf(ccc,"UserMissions\\%s",SR.cFileName);
			GFILE* F=Gopen(ccc,"r");
			if(F){
				Pack=(OneAddMission*)realloc(Pack,sizeof(OneAddMission)*(NMiss+1));
				memset(Pack+NMiss,0,sizeof OneAddMission);
				ReadWinString(F,Pack[NMiss].Title,127);
				ReadWinString(F,Pack[NMiss].Map,127);
				ReadWinString(F,ccc,127);
				_strupr(ccc);
				char* c1=strstr(ccc,".DLL");
				if(!c1)c1=strstr(ccc,".CMS");
				if(c1){
					strcpy(c1,".dsc");
					strcpy(Pack[NMiss].Preview,ccc);
					strcpy(c1,".bmp");
					FILE* F1=fopen(ccc,"r ");
					if(F1){
						strcpy(Pack[NMiss].Bitmap,ccc);
						fclose(F1);
					};
				};
				Gclose(F);
				NMiss++;
			};
		}while(FindNextFile(H,&SR));
		FindClose(H);
	};
};
AddMissionsPack::~AddMissionsPack(){
	if(Pack)free(Pack);
	memset(this,0,sizeof AddMissionsPack);
};

void SlideShow(){
	GFILE* f=Gopen("Slides\\pic.lst","r");
	if(f){
		char cc[128];
		int z;
		do{
			z=Gscanf(f,"%s",cc);
			if(z==1){
				ResFile F=RReset(cc);
				if(F!=INVALID_HANDLE_VALUE){
					RClose(F);
					DialogsSystem DSS(0,0);
					SQPicture PIC;
					PIC.LoadPicture(cc);
					DSS.addPicture(NULL,0,0,&PIC,&PIC,&PIC);
					Lpressed=0;
					KeyPressed=0;
					PIC.Draw(0,0);
					FlipPages();
					SlowLoadPalette("2\\agew_1.pal");
					do{
						DSS.ProcessDialogs();
						DSS.RefreshView();
						ProcessMessages();
					}while(!(KeyPressed||Lpressed));
					SlowUnLoadPalette("2\\agew_1.pal");
				};
			};
		}while(z==1);
	};
};
void ShowPreview(){
	ResFile F=RReset("Slides\\Preview.bmp");
	if(F!=INVALID_HANDLE_VALUE){
#ifndef _USE3D
		WindX=0;
		WindY=0;
		WindLx=1024;
		WindLy=768;
		WindX1=1023;
		WindY1=767;
#else
		GPS.SetClipArea( 0, 0, RealLx, RealLy );
#endif
		RClose(F);
		ClearScreen();
		LoadFog(2);
		DialogsSystem DSS(0,0);
		SQPicture PIC;
		PIC.LoadPicture("Slides\\Preview.bmp");
		DSS.addPicture(NULL,0,0,&PIC,&PIC,&PIC);
		Lpressed=0;
		KeyPressed=0;
		PIC.Draw(0,0);
		FlipPages();
		SlowLoadPalette("2\\agew_1.pal");
		int t0=GetTickCount();
		do{
			DSS.ProcessDialogs();
			DSS.RefreshView();
			ProcessMessages();
		}while(!(KeyPressed||Lpressed||GetTickCount()-t0>3000));
		SlowUnLoadPalette("2\\agew_1.pal");
	};
};
void ClearScreen();
char* ROME[6]={"II","III","IV","V","VI","VII"};
int ShowStatScreen(bool Next,bool Prev,byte Kind);
int ShowUserStat(bool Prev,bool Next);
int LPAL=1;
void PrintNum(int N,char* res){
	char tmp[32];
	int ps=0;
	bool sign=N<0;
	N=abs(N);
	while(N!=0){
		int v=N%10;
		N=N/10;
		tmp[ps]='0'+v;
		ps++;
		if((ps==3||ps==7||ps==11||ps==15)&&N!=0){
			tmp[ps]='.';
			ps++;
		};
	};
	if(sign){
		for(int j=0;j<ps;j++)res[j+1]=tmp[ps-j-1];
		res[ps+1]=0;
		res[0]='-';
	}else{
		for(int j=0;j<ps;j++)res[j]=tmp[ps-j-1];
		res[ps]=0;
	};
};
bool ShowStatistics(){
	//return true;
	int res=0;
	int cur=0;
	LPAL=1;
	do{
		switch(cur){
		case 0:
			res=ShowStatScreen(true,false,0);
			break;
		case 1:
			res=ShowStatScreen(true,true,1);
			break;
		case 2:
			res=ShowUserStat(true,false);
			break;
		};
		if(res==1)cur--;
		if(res==4)cur++;
		cur%=3;
	}while(res!=2&&res!=3&&res!=131);
	return res==3; 
};
void CreateCommName(byte Nat,char* Res){
	Res[0]=0;
	for(int i=0;i<7;i++){
		if(GSets.CGame.cgi_NatRefTBL[i]==Nat){
			if(Res[0])strcat(Res,"+");
			strcat(Res,GetPName(i));
		};
	};
};
int ShowStatScreen(bool Next,bool Prev,byte Kind){
	bool ExStyle=(NPlayers<2&&!PlayGameMode);
	int x=9;
	int y=13;
	int Lx=1006;
	int Ly=690;
	//ClearScreen();
	int NNAT=0;
	int NATS[7];
	int MaxPop=0;
	int MaxUpgTime=0;
	int MaxGrTime=0;
	int ODY=24;
	LocalGP BTNS;
	SQPicture BACK("Interf2\\statistic_background.bmp");
	BTNS.Load("Interf2\\stats");
	y+=ODY;
	Ly-=ODY;
	word* Val[8];
	int NVal[8];
	char* Header;
	if(Kind){
		for(int k=0;k<7;k++){
			Val[k]=NATIONS[k].Account;
			NVal[k]=NATIONS[k].NAccount;
		};
		Header=GetTextByID("ST_ACC");
	}else{
		for(int k=0;k<7;k++){
			Val[k]=NATIONS[k].Popul;
			NVal[k]=NATIONS[k].NPopul;
		};
		Header=GetTextByID("ST_MAX");
	};
	for(int i=0;i<7;i++){
		int max=0;
		Nation* NT=NATIONS+i;
		int N=NVal[i];
		word* VAL=Val[i];
		MaxGrTime=N<<8;
		for(int j=0;j<N;j++)if(VAL[j]>max)max=VAL[j];
		if(max){
			NATS[NNAT]=i;
			NNAT++;
			if(max>MaxPop)MaxPop=max;
		};
		N=NT->NUpgMade;
		if(N&&NT->UpgTime[N-1]>MaxUpgTime)MaxUpgTime=NT->UpgTime[N-1];
	};
	int sdx=3;
	int slx=Lx-70-sdx*2;
	MaxUpgTime+=(40*MaxUpgTime)/(slx-40);
	if(MaxGrTime>MaxUpgTime)MaxUpgTime=MaxGrTime;
	int LXX[8];
	int XXI;
	int YYI[8];
	if(MaxPop<5)MaxPop=5;
	if(NNAT){
		int OneLy=59+4;
		int OneDy=8+2;
		int GrLy=Ly-NNAT*OneLy;
		int GrpX=40;
		int GrpY=40;
		int GrpLx=Lx-2*GrpX;
		int GrpLy=GrLy-2*GrpY;
		int deal=(MaxPop*40)/GrpLy;
		if(deal<5)deal=5;
		else if(deal<10)deal=10;
		else if(deal<15)deal=15;
		else if(deal<20)deal=20;
		else if(deal<25)deal=25;
		else if(deal<40)deal=40;
		else if(deal<50)deal=50;
		else if(deal<75)deal=75;
		else if(deal<100)deal=100;
		else if(deal<150)deal=150;
		else if(deal<200)deal=200;
		else if(deal<250)deal=250;
		else if(deal<300)deal=300;
		else if(deal<400)deal=400;
		else if(deal<500)deal=500;
		else deal=750;
		int NGridY=(MaxPop/deal)+1;
		MaxPop=NGridY*deal;

		DialogsSystem STAT(0,0);
		STAT.addPicture(NULL,0,0,&BACK,&BACK,&BACK);
		int DDX=110;
		//if(!ExStyle)DDX=0;
		if(ExStyle){
			GP_TextButton* BT_REPLAY=STAT.addGP_TextButton(NULL,415+6,723,GetTextByID("REPLAY"),BTNS.GPID,6,&fon18y5,&fon18y5);
			BT_REPLAY->OnUserClick=&MMItemChoose;
			BT_REPLAY->UserParam=131;
		}else{
			GP_TextButton* BT_REPLAY=STAT.addGP_TextButton(NULL,415+6,723,GetTextByID("REPLAY"),BTNS.GPID,6,&fon18y3,&fon18y3);
			BT_REPLAY->OnUserClick=&MMItemChoose;
			BT_REPLAY->UserParam=131;
			BT_REPLAY->Enabled=0;
		};
		//GP_Button* BT=STAT.addGP_Button(NULL,263-DDX,723,BTNS.GPID,0,1);
		GP_TextButton* RGAME=STAT.addGP_TextButton(NULL,263+36-DDX,723,GetTextByID("ST_GAM"),BTNS.GPID,6,&fon18y5,&fon18y5);
		RGAME->OnUserClick=&MMItemChoose;
		RGAME->UserParam=2;

		GP_TextButton* RMENU;
		if(PlayGameMode){
			RMENU=STAT.addGP_TextButton(NULL,526+17+DDX,723,GetTextByID("ST_MEN"),BTNS.GPID,6,&fon18y5,&fon18y5);
			RMENU->Enabled=0;
		}else RMENU=STAT.addGP_TextButton(NULL,526+17+DDX,723,GetTextByID("ST_MEN"),BTNS.GPID,6,&fon18y5,&fon18y5);
		RMENU->OnUserClick=&MMItemChoose;
		RMENU->UserParam=3;

		GP_Button* BACK=STAT.addGP_Button(NULL,218-11-DDX,723,BTNS.GPID,5,1);
		BACK->OnUserClick=&MMItemChoose;
		BACK->UserParam=1;
		BACK->Enabled=Prev;
		BACK->Visible=Prev;

		GP_Button* NEXT=STAT.addGP_Button(NULL,790-16+DDX,723,BTNS.GPID,4,0);
		NEXT->OnUserClick=&MMItemChoose;
		NEXT->UserParam=4;
		NEXT->Enabled=Next;
		NEXT->Visible=Next;

		STAT.HintFont=&SmallBlackFont1;
		STAT.HintX=-1;
		STAT.HintY=250;
		char ccc[100];
		STAT.addTextButton(NULL,512,26,Header,&fon18y5,&fon18y5,&fon18y5,1);
		int yy=y+GrLy;
		int xp=-1000;
		for(int i=0;i<NNAT;i++){
			xp=-1000;
			Nation* NT=NATIONS+NATS[i];
			char commname[128];
			CreateCommName(NATS[i],commname);
			//char* pnm=GetPName(NATS[i]);
			LXX[i]=GetRLCStrWidth(commname,&SmallWhiteFont1)+40;
			int dx=x+GrpX+(GrpLx-LXX[i])/2;
			STAT.addTextButton(NULL,dx+33,yy-8,commname,&SmallWhiteFont1,&SmallWhiteFont1,&SmallWhiteFont1,0);
			STAT.addColoredBar(dx+6,yy-2,24,1,0xD0+NATS[i]*4);
			XXI=x+34;
			YYI[i]=yy;
			int N=NT->NUpgMade;
			int DXX=10;
			ColoredBar* CB=STAT.addColoredBar(GrpX+DXX,yy-10,1024-GrpX*2-DXX*2,62,0xEC);
			CB->Style=1;
			CB=STAT.addColoredBar(GrpX+DXX,yy+6,1024-GrpX*2-DXX*2,0,0xEC);
			CB->Style=1;
			//CB=STAT.addColoredBar(GrpX+DXX,yy-10,1024-GrpX*2-DXX*2,16,0xEC);
			CB=STAT.addColoredBar(GrpX+DXX-1,yy-1-10,1024-GrpX*2+2-DXX*2,62+2,0xEC);
			CB->Style=1;
			for(int j=0;j<N;j++){
				int xx=x+3+30+(NT->UpgTime[j]*slx)/MaxUpgTime;
				if(xx<xp){
					xx=xp+2;
				};
				xp=xx;
				NewUpgrade* NU=NT->UPGRADE[NT->UpgIDS[j]];
				if(NU->CtgUpgrade!=23){
					SimpleDialog* SD;
					if(NU->IconFileID!=0xFFFF)SD=STAT.addGPPicture(NULL,xx,yy+OneDy,NU->IconFileID,NU->IconSpriteID);
					else SD=STAT.addColoredBar(xx,yy+OneDy,40,40,0);
					if(SD)SD->Hint=NU->Message;
					ColoredBar* CB=STAT.addColoredBar(xx,yy+OneDy,40,40,0xD0+NATS[i]*4);
					if(CB)CB->Style=1;
					if(SD)SD->Child=CB;
					/*
					if(NU->Level>1){
					TextButton* TB=STAT.addTextButton(NULL,xx+2,yy+OneDy+25,ROME[NU->Level-2],&SmallWhiteFont,&SmallWhiteFont,&SmallWhiteFont,0);
					TB->Enabled=0;
					CB->Child=TB;
					};
					*/
				};
			};
			yy+=OneLy;
		};
		ItemChoose=-1;
		KeyPressed=0;
		Lpressed=0;
		byte mm=1;
		do{
			//GPS.ShowGP(0,0,BTNS.GPID,2,0);
			//for(int i=0;i<NNAT;i++)DrawStHeader(GrpX+x+GrpLx/2,YYI[i],LXX[i]);
			STAT.MarkToDraw();
			STAT.ProcessDialogs();
			//DrawStatTable(x,y-ODY,Lx,GrLy+ODY,OneLy,NNAT);
			int gx0=GrpX+x;
			int gx1=GrpX+x+GrpLx-1;
			int gy0=GrpY+y;
			int gy1=GrpY+y+GrpLy-1;
			Xbar(gx0-1,gy0-1,gx1-gx0+4,gy1-gy0+4,0xEC);
			//Xbar(gx0-2,gy0-2,gx1-gx0+5,gy1-gy0+5,0x3A);
			//Xbar(gx0-3,gy0-3,gx1-gx0+7,gy1-gy0+7,0x39);
			//Xbar(gx0-4,gy0-4,gx1-gx0+9,gy1-gy0+9,0xAE);
			//Xbar(gx0-5,gy0-5,gx1-gx0+11,gy1-gy0+11,0xAE);
			//Xbar(gx0-6,gy0-6,gx1-gx0+13,gy1-gy0+13,0xAE);
			//drawing grids
			for(int ix=0;ix<8;ix++){
				int gx0=x+GrpX+((GrpLx*ix)>>3);
				int gx1=x+GrpX+((GrpLx*(ix+1))>>3);
				for(int iy=0;iy<NGridY;iy++){
					int gy0=y+GrpY+((GrpLy*iy)/NGridY);
					int gy1=y+GrpY+((GrpLy*(iy+1))/NGridY);
					Xbar(gx0,gy0,gx1-gx0+1,gy1-gy0+1,0xEC);
					//Xbar(gx0+1,gy0+1,gx1-gx0-1,gy1-gy0-1,0x3B);
					//Xbar(gx0+2,gy0+2,gx1-gx0-3,gy1-gy0-3,0x3A);
					//Xbar(gx0+3,gy0+3,gx1-gx0-5,gy1-gy0-5,0x39);
					//Xbar(gx0+4,gy0+4,gx1-gx0-7,gy1-gy0-7,0xAE);
					//Xbar(gx0+5,gy0+5,gx1-gx0-9,gy1-gy0-9,0xAE);
					//Xbar(gx0+6,gy0+6,gx1-gx0-11,gy1-gy0-11,0xAE);
					if(ix==0){
						sprintf(ccc,"%d",(NGridY-iy-1)*deal);
						ShowString(gx0+3,gy1-14,ccc,&SmallBlackFont);
					};
				};
			};
			int X0,Y0,X1,Y1;
			for(int i=0;i<NNAT;i++){
				Nation* NT=NATIONS+NATS[i];
				byte c=NATS[i]*4+0xD0;
				int N=NVal[NATS[i]];
				word* VAL=Val[NATS[i]];
				for(int j=0;j<N;j++){
					if(j>0){
						X0=X1;
						Y0=Y1;
					};
					X1=x+GrpX+((j<<8)*GrpLx)/MaxUpgTime;
					Y1=y+GrpY+GrpLy-(VAL[j]*(GrpLy-10))/MaxPop;
					if(j>0){
						DrawLine(X0,Y0,X1,Y1,c);
						DrawLine(X0+1,Y0,X1+1,Y1,c);
						DrawLine(X0+1,Y0+1,X1+1,Y1+1,c);
						DrawLine(X0,Y0+1,X1,Y1+1,c);
					};
				};
			};
			ProcessMessages();
			STAT.RefreshView();
			if(LPAL){
				SlowLoadPalette("0\\agew_1.pal");
				LPAL=0;
			};
			//if(KeyPressed||Lpressed){
			//	ItemChoose=1;
			//};
		}while(ItemChoose==-1);
	}else return 3;
	KeyPressed=false;
	Lpressed=0;
	UnPress();
	return ItemChoose;
};
int ShowUnitsList(int x,int y,int Lx,word* Value);
#define Statfont &fon16y3
int ShowUserStat(bool Prev,bool Next){
	bool ExStyle=(NPlayers<2&&!PlayGameMode);
	int x=9;
	int y=13;
	int Lx=1006;
	int Ly=690;
	LocalGP BTNS;
	SQPicture BACK2("Interf2\\statistic_background.bmp");
	BTNS.Load("Interf2\\stats");
	DialogsSystem STAT(0,0);
	STAT.addPicture(NULL,0,0,&BACK2,&BACK2,&BACK2);
	int DDX=110;
	//if(!ExStyle)DDX=0;
	if(ExStyle){
		GP_TextButton* BT_REPLAY=STAT.addGP_TextButton(NULL,415+6,723,GetTextByID("REPLAY"),BTNS.GPID,6,&fon18y5,&fon18y5);
		BT_REPLAY->OnUserClick=&MMItemChoose;
		BT_REPLAY->UserParam=131;
	}else{
		GP_TextButton* BT_REPLAY=STAT.addGP_TextButton(NULL,415+6,723,GetTextByID("REPLAY"),BTNS.GPID,6,&fon18y3,&fon18y3);
		BT_REPLAY->OnUserClick=&MMItemChoose;
		BT_REPLAY->UserParam=131;
		BT_REPLAY->Enabled=0;
	};
	//GP_Button* BT=STAT.addGP_Button(NULL,263-DDX,723,BTNS.GPID,0,1);
	GP_TextButton* RGAME=STAT.addGP_TextButton(NULL,263+36-DDX,723,GetTextByID("ST_GAM"),BTNS.GPID,6,&fon18y5,&fon18y5);
	RGAME->OnUserClick=&MMItemChoose;
	RGAME->UserParam=2;

	GP_TextButton* RMENU;
	if(PlayGameMode){
		RMENU=STAT.addGP_TextButton(NULL,526+17+DDX,723,GetTextByID("ST_MEN"),BTNS.GPID,6,&fon18y5,&fon18y5);
		RMENU->Enabled=0;
	}else RMENU=STAT.addGP_TextButton(NULL,526+17+DDX,723,GetTextByID("ST_MEN"),BTNS.GPID,6,&fon18y5,&fon18y5);
	RMENU->OnUserClick=&MMItemChoose;
	RMENU->UserParam=3;

	GP_Button* BACK=STAT.addGP_Button(NULL,218-11-DDX,723,BTNS.GPID,5,1);
	BACK->OnUserClick=&MMItemChoose;
	BACK->UserParam=1;
	BACK->Enabled=Prev;
	BACK->Visible=Prev;

	GP_Button* NEXT=STAT.addGP_Button(NULL,790-16+DDX,723,BTNS.GPID,4,0);
	NEXT->OnUserClick=&MMItemChoose;
	NEXT->UserParam=4;
	NEXT->Enabled=Next;
	NEXT->Visible=Next;
	ListBox* LB=STAT.addListBox(NULL,x+25+20,y+35,30,180,26,&WhiteFont,&YellowFont,NULL);
	byte NATS[8];
	int NNAT=0;
	for(int i=0;i<7;i++){
		int max=0;
		Nation* NT=NATIONS+i;
		int N=NT->NPopul;
		word* VAL=NT->Popul;
		for(int j=0;j<N;j++)if(VAL[j]>max)max=VAL[j];
		if(max){
			NATS[NNAT]=i;
			NNAT++;
			char commname[256];
			CreateCommName(i,commname);
			LimitString(commname,&YellowFont,200);
			LB->AddItem(commname,i);
		};
	};
	LB->CurItem=0;
	ItemChoose=-1;
	int mm=1;
	char* hdrs[8];
	hdrs[0]=GetTextByID("ST_TOTAL");
	hdrs[1]=GetTextByID("ST_UPGR");
	hdrs[2]=GetTextByID("ST_MINE");
	hdrs[3]=GetTextByID("ST_UNITS");
	hdrs[4]=GetTextByID("ST_BLD");
	hdrs[5]=GetTextByID("ST_LIFE");
	hdrs[6]=GetTextByID("ST_BUY");
	hdrs[7]=GetTextByID("ST_SELL");
	char* STHDR=GetTextByID("ST_UHDR");
	char* KHDR=GetTextByID("ST_KILL");
	char* PHDR=GetTextByID("ST_PROD");
	int curnat=0;

	int ux=230-30;
	int uy=320;
	int ulx=353;
	int uly=373;
	int kx=ux+ulx+45;
	VScrollBar* PBR=STAT.addNewGP_VScrollBar(NULL,ux+ulx+11-15,uy+1+2,uly-2-2,1,0,3,0);
	VScrollBar* KBR=STAT.addNewGP_VScrollBar(NULL,kx+ulx+11-15,uy+1+2,uly-2-2,1,0,3,0);
	PBR->ScrDy=200;
	KBR->ScrDy=200;
	do{
		ProcessMessages();
		STAT.MarkToDraw();
		STAT.ProcessDialogs();
		curnat=LB->CurItem;
		int cnt=NATS[curnat];
		int* ResSrc=(int*)(&NATIONS[cnt].ResTotal);
		//GPS.ShowGP(0,0,BTNS.GPID,2,0);
		//DrawStatTable1(x,y,x+Lx-1,y+Ly-1,x+200);
		for(int i=0;i<LB->NItems;i++){
			CBar(LB->x-15,LB->y+15+i*25,12,3,0xD0+GSets.CGame.cgi_NatRefTBL[NATS[i]]*4);
		};
		ShowString(512-GetRLCStrWidth(STHDR,&fon18y5)/2,26,STHDR,&fon18y5);
		ShowString(ux+(ulx>>1)+7-(GetRLCStrWidth(PHDR,&YellowFont)>>1),uy-22,PHDR,Statfont);
		ShowString(kx+(ulx>>1)+7-(GetRLCStrWidth(KHDR,&YellowFont)>>1),uy-22,KHDR,Statfont);
		int rx=190;
		int ry=y+48;
		int rx1=rx+219+5;
		int rxl=80;
		for(int q=0;q<6;q++){
			ShowString(rx1+rxl*q+rxl+(rxl>>1)-(GetRLCStrWidth(RDS[q].Name,&YellowFont)>>1),ry+6,RDS[q].Name,Statfont);
		};
		Hline(rx,ry,rx1+rxl*7,0xEC);
		ry+=26;
		Vline(rx,ry-26,ry+8*26,0xEC);
		for(int q=0;q<8;q++){
			Hline(rx,ry,rx1+rxl*7,0xEC);
			ShowString(rx+15,ry+6,hdrs[q],Statfont);
			for(int j=0;j<6;j++){
				char cc1[32];
				PrintNum(ResSrc[q*8+j],cc1);
				//sprintf(cc1,"%d",ResSrc[q*8+j]);
				ShowString(rx1+rxl*j+rxl+(rxl>>1)-(GetRLCStrWidth(cc1,&YellowFont)>>1),ry+6,cc1,Statfont);
				if(q==0){
					Vline(rx1+rxl*j+rxl,ry-26,ry+8*26,0xEC);
				};
			};
			if(q==0){
				Vline(rx1+rxl*7,ry-26,ry+8*26,0xEC);
			};
			ry+=26;
		};
		Hline(rx,ry,rx1+rxl*7,0xEC);
		TempWindow TMW;
		PushWindow(&TMW);
		Xbar(ux-10,uy,ulx+25+10,uly,0xEC);
		IntersectWindows(ux-8,uy+1,ux+ulx+15,uy+uly-2);
		int y0=uy-PBR->SPos;
		int yfin=ShowUnitsList(ux,y0,ulx-12,NATIONS[cnt].NProduced);
		int mps=yfin-uly-2+3;
		if(mps>0){
			PBR->Visible=1;
			PBR->Enabled=1;
			PBR->SMaxPos=mps;
		}else{
			PBR->Visible=0;
			PBR->Enabled=0;
			PBR->SPos=0;
		};
		PopWindow(&TMW);
		Xbar(kx-10,uy,ulx+25+10,uly,0xEC);
		IntersectWindows(kx-8,uy+1,kx+ulx+15,uy+uly-2);
		y0=uy-KBR->SPos;
		yfin=ShowUnitsList(kx,y0,ulx-12,NATIONS[cnt].NKilled);
		mps=yfin-uly-2+3;
		if(mps>0){
			KBR->Visible=1;
			KBR->Enabled=1;
			KBR->SMaxPos=mps;
		}else{
			KBR->Visible=0;
			KBR->Enabled=0;
			KBR->SPos=0;
		};
		PopWindow(&TMW);
		STAT.RefreshView();
		if(mm){
			//SlowLoadPalette("0\\agew_1.pal");
			//mm=0;
		};
		//if(KeyPressed||Lpressed){
		//	ItemChoose=1;
		//};
	}while(ItemChoose==-1);
	KeyPressed=false;
	Lpressed=0;
	UnPress();
	return ItemChoose;
};
void CenterScreen(){
	if ( MOptions.Players.Player[MyNation].UseStartPos ){
		float xc=(float)(MOptions.Players.Player[MyNation].cX);
		float yc=(float)(MOptions.Players.Player[MyNation].cY);

		xc = (xc/32.f)-(float)smaplx/2.f;
		yc = (yc/32.f)-(float)smaply/2.f;

		void SetCameraPos(float x,float y);
		SetCameraPos(xc,yc);
	}else{
		int xc=0;
		int yc=0;
		int N=0;
		int mnt=GSets.CGame.cgi_NatRefTBL[MyNation];
		for(int i=0;i<MAXOBJECT;i++){
			OneObject* OB=Group[i];
			if(OB&&(!OB->Sdoxlo)&&OB->NNUM==mnt){
				xc+=OB->RealX>>4;
				yc+=OB->RealY>>4;
				N++;
			};
		};
		if(N){
			int x=xc/N;
			int y=yc/N;
			// find nearest brigade
			int dist=1000000;
			int nb=CITY[mnt].NBrigs;
			for(int j=0;j<nb;j++){
				Brigade* BR=CITY[mnt].Brigs+j;
				if(BR->Enabled&&BR->WarType&&BR->NMemb>40){
					int xb,yb;
					if(BR->GetCenter(&xb,&yb)){
						int d=Norma(x-xb,y-yb);
						if(dist>d){
							dist=d;
							x=xb;
							y=yb;
						}
					}
				}
			}
			// Actually this is not right....
/*			x=(x-(RealLx>>1))>>5;
			y=(y-RealLy)>>5;
			if(x<1)x=1;
			if(y<1)y=1;
			if(x+smaplx>=msx-1)x=msx-smaplx-1;
			if(y+smaply>=msy-1)y=msy-smaply-1;
			extern float fMapX;
			extern float fMapY;
			fMapX=x;
			fMapY=y;*/
			// Better use this:
			void SetScreenCenterToXY(int, int);
			SetScreenCenterToXY(x, y);
		};
	};
};
extern bool EnterChatMode;
extern char ChatString[128];
bool Superuser=0;
void AssignHint1(char* s,int time);
void CmdGiveMoney(byte SrcNI,byte DstNI,byte Res,int Amount);
void CmdMoney(byte NI);
int ReadKey();
extern int NKeys;
extern char CHATSTRING[256];
int ChatCursPos=0;
bool CheckFNSend(int idx);
extern byte LastAscii;
void HandleSMSChat(char* Mess);
void HandlwSMSMouse();
void CmdOfferVoting();
extern int CurAINF_Show;
DLLEXPORT
int FindNextZoneOnTheSafeWay(int Start,int Fin,short* DangerMap,int* MaxDanger,int DangSteps);
DLLEXPORT void SelectUnits(GAMEOBJ* Units,bool Add);
int WayID=0;
extern int NWay;
extern bool WayMode;

CIMPORT void AddStorm(GAMEOBJ* Src, byte Owner, int Diff);
CIMPORT void AddFirers(GAMEOBJ* Src,byte Owner);
CIMPORT void AddPsKillers(GAMEOBJ* Src,byte Owner,bool SeakMine=true);
CIMPORT void AddTomahawks(GAMEOBJ* Src,byte Owner,word Base=0,int DriftRadius=0, int CriticalMass=0);

DLLEXPORT void RegisterDynGroup(GAMEOBJ* Units);
DLLEXPORT void SaveSelectedUnits(byte NI,GAMEOBJ* Units,bool Add);
GAMEOBJ TempGrp;

CIMPORT void MindCheats(byte NI, char* com);
byte MindNI=0xFF;

extern word FULLWAY[128];
extern int NWPTS;
extern int TOPTYPE;

int DrawPathID=0xFFFF;
int DrawDestID=0xFFFF;
bool HideBorderMode=0;

extern ADSStruct DVS;
void SaveUnitsStatsToFile(char* file);
CEXPORT void VitalError(char*);
DLLEXPORT void ChangeFriends(byte Nat,byte Flags);

void ProcessChatKeys(){
KRT:
	if(!NKeys)return;
	LastKey=ReadKey();
	KeyPressed=0;
	if(LastKey==27){
		EnterChatMode=0;
		ChatString[0]=0;
		return;
	};
	if(LastKey==13){
		HandleSMSChat(ChatString);
		EnterChatMode=0;
		if(GSets.ShowChat) AssignHint1(ChatString,250,16+GSets.CGame.cgi_NatRefTBL[MyNation]);
		SendChat(ChatString,(GetKeyState(VK_CONTROL)&0x8000)!=0);
		ChatCursPos=0;
		AssignHint1(ChatString,256,0);
#ifdef NO_CHEATS_IN_MULTIPLAYER
		if(NPlayers>1){
			ChatString[0]=0;
			return;
		}
#endif //NO_CHEATS_IN_MULTIPLAYER
		if(ext_OnCheatEntering(ChatString)){
			ChatString[0]=0;
			return;
		}
		if(!strcmp(ChatString,"runprof")){
			void RunProfiler();
			RunProfiler();
		}else
		if(!strcmp(ChatString,"endprof")){
			void StopProfiler();
			StopProfiler();
			void ShowProfInfo();
			ShowProfInfo();
		}else			
		if(!strcmp(ChatString,"vprof")){
			void ShowProfInfo();
			ShowProfInfo();
		}
		/*if(NPlayers<2)*/{
#ifdef AI_CHEATS
			if(!strcmp(ChatString,"arm")){
				CEXPORT void DLLSetAttState(byte NI,byte i);
				DLLSetAttState(0,1);
			}else
			if(!strcmp(ChatString,"wledit")){
				void EditWalls();
				EditWalls();
			}else
			if(!strcmp(ChatString,"AVI")||!strcmp(ChatString,"avi")){
				if(DVS.Active) DVS.Active=false; else DVS.Active=true;
				if(!DVS.Active){
					DVS.DS.CloseDialogs();
					DVS.Menu.CloseDialogs();
				}
			}else
			if(strstr(ChatString,"RunScript")||strstr(ChatString,"exec")){
				char com[128];
				char file[128];
				int z=sscanf(ChatString,"%s%s",com,file);
				if(z==2){
					xmlQuote xml;					
					extern OneScript* ActionScript;
					if(ActionScript) delete(ActionScript);
					ActionScript=new OneScript;						
					ActionScript->SafeReadFromFile(file);
				}
			}else
			if(strstr(ChatString,"sklad")){
				char txt[128];
				int set;
				int z=sscanf(ChatString,"%s%d",txt,&set);
				if(z==2){
					int n=ImNSL[MyNation];
					word* id=ImSelm[MyNation];
					word* sn=ImSerN[MyNation];
					for(int i=0;i<n;i++,id++,sn++){
						OneObject* OB=Group[*id];
						if(OB&&OB->Serial==*sn&&OB->NewBuilding){
							if(set) OB->AbRes=0xFFFF;
								else OB->AbRes=0;
						}
					}
				}
			}else
			if(strstr(ChatString,"erase")){
				int n=ImNSL[MyNation];
				word* id=ImSelm[MyNation];
				//word* sn=ImSerN[MyNation];
				for(int i=0;i<n;i++,id++){
					OneObject* OB=Group[*id];
					if(OB){
						EraseObject(OB);
					}
				}
			}
			else if(strstr(ChatString,"chfr"))
			{
				int nat, flags;
				char com[128];
				int z = sscanf( ChatString,"%s%d%d", com, &nat, &flags );
				if ( z > 2 )
				{
					ChangeFriends(nat, flags);
				}				
			}
			else if(strstr(ChatString,"FWAY")){
				int T0,T1;
				char SS[128];
				int z=sscanf(ChatString,"%s%d%d",SS,&T0,&T1);
				if(z==3){
					short DM[2048];
					int MAXD;
					memset(DM,0,sizeof DM);
					FindNextZoneOnTheSafeWay(T0,T1,DM,&MAXD,256);
					CurAINF_Show=7;
				};
			}else
			if(strstr(ChatString,"fway")){
				int T0,T1;
				char SS[128];
				int z=sscanf(ChatString,"%s%d%d",SS,&T0,&T1);
				if(z==3){
					int NA=GetNAreas();
					NWPTS=1;
					FULLWAY[0]=T0;
					do{
						T0=GetMotionLinks(T0*NA+T1);
						if(T0>=0xFFFE) break;
						FULLWAY[NWPTS]=T0;
						NWPTS++;
						addrand(NWPTS);
					}while(T0!=T1&&NWPTS<127);
					CurAINF_Show=7;
				};
			}else
			if(strstr(ChatString,"locktype")){
				char Com[128];
				int t;
				int z=sscanf(ChatString,"%s%d",Com,&t);
				if(z>1){
					if(t<0||t>=NMFIELDS) TOPTYPE=0;
						else TOPTYPE=t;
				}
			}else
			if(strstr(ChatString,"toplink")||strstr(ChatString,"tlink")){	// show top link
				word Top,LockType;
				char Com[128];
				int z=sscanf(ChatString,"%s%d%d",Com,&Top,&LockType);
				if(z>1){
					if(z==2||LockType>=NMFIELDS) LockType=TOPTYPE;
					int NA=GetNAreas(LockType);
					if(Top<NA){						
						Area* AR=GetTopMap(Top,LockType);
						int NLink=AR->NLinks;
						//word* Link=AR->Link;
						for(int i=0;i<NLink;i++){
							FULLWAY[i+i]=Top;
							FULLWAY[i+i+1]=AR->Link[i].NextAreaID;
						}
						NWPTS=NLink<<1;	
						TOPTYPE=LockType;
					}
					CurAINF_Show=7;
				}
			}else
			if(strstr(ChatString,"SW "))
			{
				char tmp[8];
				int nat;
				if(sscanf(ChatString,"%s %d",tmp, &nat))
					if(nat>-1&&nat<8)
						if(NSL[nat]>0)
						{
							OneObject* OB=Group[Selm[nat][0]];
							//if(OB&&OB->BrigadeID!=0xFFFF)
							{
								void AddBrigadeInWiselyFormations(int Na);
								AddBrigadeInWiselyFormations(nat);
							}
						}
			}
			else
#endif
			/*
			if(!strcmp(ChatString,"SGAI0")){
				CurAINF_Show=0;
			}else
			if(!strcmp(ChatString,"SGAI1")){
				CurAINF_Show=1;
			}else
			if(!strcmp(ChatString,"SGAI2")){
				CurAINF_Show=2;
			}else
			if(!strcmp(ChatString,"SGAI3")){
				CurAINF_Show=3;
			}else
			if(!strcmp(ChatString,"SGAI4")){
				CurAINF_Show=4;
			}else
			if(!strcmp(ChatString,"SGAI5")){
				CurAINF_Show=5;
			}else
			if(!strcmp(ChatString,"SGAI6")){
				CurAINF_Show=6;
			}else
			if(!strcmp(ChatString,"SGAI7")){
				CurAINF_Show=7;
			}else
			if(!strcmp(ChatString,"NOSGAI")){
				CurAINF_Show=-1;
			}else
			*/
			if(strstr(ChatString,"savestats")){
				char ID[512];
				char file[512];
				int z=sscanf(ChatString,"%s%s",ID,file);
				if(z==2){
					SaveUnitsStatsToFile(file);
				}else{
					VitalError("savestats <filename(without extention)>");
				}
			}else
			if(strstr(ChatString,"path")){
				char ID[512];
				int PARAM=0;
				int z=sscanf(ChatString,"%s%d",ID,&PARAM);
				if(z==2){
					DrawPathID=PARAM;
				}
			}else
			if(strstr(ChatString,"dest")){
				char ID[512];
				int PARAM=0;
				int z=sscanf(ChatString,"%s%d",ID,&PARAM);
				if(z==2){
					DrawDestID=PARAM;
				}
			}else
			if(!strcmp(ChatString,"hbor")){
				HideBorderMode=!HideBorderMode;
			}else
			if(!strcmp(ChatString,"illusion")){
				int NS=NSL[MyNation];
				word* IDS=Selm[MyNation];
				word* SNS=SerN[MyNation];
				for(int i=0;i<NS;i++){
					word MID=IDS[i];
					if(MID!=0xFFFF){
						OneObject* OB=Group[MID];
						if(OB)OB->Illusion=1;
					}
				}
			}else
			if(!strcmp(ChatString,"fast")){
				extern int RequiredMsPerFrame;
				GSets.SVOpt.RequiredMsPerFrame=3;
			}else
			// BEGIN: NEW TERRAIN CHEATS
			// CtrlC
			if((ChatString[0] == 'C') && (ChatString[1] == 't') && (ChatString[2] == 'r') &&
				(ChatString[3] == 'l') && (ChatString[4] == 'C'))
			{
/*				int Size = g_UniversalMap.Vert.NValues * sizeof(OneMapVertex) +
					g_UniversalMap.Idxs.NValues * sizeof(DWORD);

				char tokens[] = "xyr";
				float v[] = {-1, -1, UniversalMap::c_CtrlCVrMin};
				g_UniversalMap.ArgsParser(ChatString, tokens, v);
				int x = (int)v[0], y = (int)v[1], r = (int)v[2];
				extern int LastMx, LastMy;
				if(x == -1) x = LastMx;
				if(y == -1) y = LastMy;
				g_UniversalMapClipboard.Copy(x, y, r);*/
			}else
			// CtrlV
			if((ChatString[0] == 'C') && (ChatString[1] == 't') && (ChatString[2] == 'r') &&
				(ChatString[3] == 'l') && (ChatString[4] == 'V'))
			{
/*				char tokens[] = "xy";
				float v[] = {-1, -1};
				g_UniversalMap.ArgsParser(ChatString, tokens, v);
				int x = (int)v[0], y = (int)v[1];
				extern int LastMx, LastMy;
				if(x == -1) x = LastMx;
				if(y == -1) y = LastMy;
				//g_UniversalMapClipboard.Paste(x, y, 0);
				void ResetGroundCache();
				ResetGroundCache();*/
			}else
			// invnorms
			if((ChatString[0] == 'i') && (ChatString[1] == 'n') &&
				(ChatString[2] == 'v') && (ChatString[3] == 'n') && 
				(ChatString[4] == 'o') && (ChatString[5] == 'r') &&
				(ChatString[6] == 'm') && (ChatString[7] == 's'))
			{
//				int N = g_UniversalMap.Idxs.NValues / 3;
//				for(int i = 0; i < N; i++)
//					swap(g_UniversalMap.Idxs[3 * i + 1], g_UniversalMap.Idxs[3 * i + 2]);
//				MessageBox(NULL, "Normals were inverted!", "invnorms", 0);
			}else
			// refresh
			if((ChatString[0] == 'r') && (ChatString[1] == 'e') && (ChatString[2] == 'f') &&
				(ChatString[3] == 'r') && (ChatString[4] == 'e') && (ChatString[5] == 's') &&
				(ChatString[6] == 'h') && (ChatString[7] == 0))
			{
/*				bool CheckIfNewTerrain(void);
				if(CheckIfNewTerrain() == true)
				{
//					g_UniversalMap.Refresh();
					void ResetGroundCache();
					ResetGroundCache();
				}*/
			}
			else
			// wire
			if((ChatString[0] == 'w') && (ChatString[1] == 'i') && (ChatString[2] == 'r') && (ChatString[3] == 'e'))
			{
				bool CheckIfNewTerrain(void);
				if(CheckIfNewTerrain())
				{
//					g_UniversalMap.bWire = !g_UniversalMap.bWire;
					void ResetGroundCache();
					ResetGroundCache();
				}
			}
			else
			// END: NEW TERRAIN CHEATS
			if(!strcmp(ChatString,"slow")){
				extern int RequiredMsPerFrame;
				GSets.SVOpt.RequiredMsPerFrame=40;
			}else
			if(strstr(ChatString,"susptime")){
				extern int susptime;
				char cc[64];
                sscanf(ChatString,"%s%d",cc,&susptime);
			}else
			if(strstr(ChatString,"fear")){
				vui_IS_SelFearDown f;
				f.Data.NI=MyNation;
				f.InterfAction();
			}else
			if(strstr(ChatString,"rtf")){
				vui_IS_SelRTF f;
				f.Data.NI=MyNation;
				f.Data.State=1;
				f.InterfAction();
			}else
			if(strstr(ChatString,"rtnf")){
				vui_IS_SelRTF f;
				f.Data.NI=MyNation;
				f.Data.State=0;
				f.InterfAction();
			}else
			if(strstr(ChatString,"charge")){
				vui_IS_SelChargeAll com;
				com.Data.NI=MyNation;				
				com.InterfAction();
			}else
			if(strstr(ChatString,"hold")){
				vui_IS_BrigHoldPosit com;
				com.Data.NI=MyNation;				
				com.InterfAction();
			}else				
			if(strstr(ChatString,"rifle")){
				vui_IS_SelRifle com;
				com.Data.NI=MyNation;
				com.Data.State=1;
				com.InterfAction();
			}else
			if(!strcmp(ChatString,"pulse")){
				if(ImNSL[MyNation]){
					OneObject* OB=Group[ImSelm[MyNation][0]];
					if(OB){
						Add_VI_Zone(OB->RealX>>4,OB->RealY>>4,0xFFFFFFFF);
					}
				}			
			}else
			if(!strcmp(ChatString,"estats")){
				bool Draw_EW2_CampaignStats(SimpleDialog* SD);//grey [14.01.05]
				Draw_EW2_CampaignStats(NULL);
			}else
			if(!strcmp(ChatString,"stats")){
				void rcm_InitStatistic();
				rcm_InitStatistic();
				v_MainMenu.StartDS("PostGameStat",true);
				v_MainMenu.StartDS("EndGameMessage",true);				
			}else
			if(!strcmp(ChatString,"heroes")){
				v_MainMenu.StartDS("HeroesUpgrades", true);
			}else
			if(!strcmp(ChatString,"trestore")){
				void RestoreMarketPrice();
				RestoreMarketPrice();				
			}else
			if(!strcmp(ChatString,"tchange")){				
				bool ChangeMarketPrice(byte ResID, float Changer);
				ChangeMarketPrice(0,4);
			}else
			if(!strcmp(ChatString,"3in1")){
#ifndef AI_CHEATS
//				CurrentSinBattle=-1;
#endif
//				extern int NCallsMakeDamage;
//				NCallsMakeDamage=0;
				ChangeNation=1;
				MultiTvar=1;
				FogMode=0;
			}else
			if(!strcmp(ChatString,"skiptotheendplease")){
#ifndef AI_CHEATS
				CurrentSinBattle=-1;
#endif
				if(NPlayers<2)ShowVictory();
			}else
			if(!strcmp(ChatString,"ntch"))ChangeNation=!ChangeNation;
			if(!strcmp(ChatString,"beasts"))MultiTvar=!MultiTvar;
			if(!strcmp(ChatString, "fastupg")) vFastUpg=true;
			if(!strcmp(ChatString, "slowupg")) vFastUpg=false;
			if(!strcmp(ChatString,"farseer")){
#ifndef AI_CHEATS
				CurrentSinBattle=-1;
#endif
				FogMode=!FogMode;
			}
#ifdef AI_CHEATS
			char* c=NULL;
			void ChangeBrigNation(word SBID, byte SrcNat, byte DstNat);
			if(strstr(ChatString,"setbrignat")){
				char com[200];
				int dnat;
				int z=sscanf(ChatString,"%s%d",com,&dnat);
				if(z==2&&dnat>=0&&dnat<8){
					int N=ImNSL[MyNation];
					word* ID=ImSelm[MyNation];
					for(int i=0;i<N;i++){
						OneObject* OB=Group[ID[i]];
						if(OB&&!OB->Sdoxlo&&OB->InArmy&&OB->BrigadeID!=0xFFFF){
							ChangeBrigNation(OB->BrigadeID,OB->NNUM,dnat);
							break;
						}
					}
				}
			}else
			if(strstr(ChatString,"setmind")){
				char com[200];
				int nat;
				int z=sscanf(ChatString,"%s%d",com,&nat);
				if(z==2&&nat<7){
					MindNI=nat;
				}
			}else
			if(c=strstr(ChatString,"mind")){				
				if(MindNI!=0xFF){
					//char com[200];
					//int z=sscanf(ChatString,"%s",com);
					//if(z==2){
						MindCheats(MindNI, c+4);
					//}				
				}
			}else
			if(strstr(ChatString,"addtom")){
				char com[200];
				byte nat;
				int strelok=0;
				int z=sscanf(ChatString,"%s%d%d",com,&nat,&strelok);
				if(z>2){
					if(TempGrp.Index!='UNIT') RegisterDynGroup(&TempGrp);
					SaveSelectedUnits(nat,&TempGrp,0);
					AddTomahawks(&TempGrp,nat,strelok);
				}
			}else
			if(strstr(ChatString,"addfr")){
				char com[200];
				byte nat;
				int strelok=0;
				int z=sscanf(ChatString,"%s%d%d",com);
				if(z>0){
					if(TempGrp.Index!='UNIT') RegisterDynGroup(&TempGrp);
					SaveSelectedUnits(MyNation,&TempGrp,0);
					AddFirers(&TempGrp,MyNation);
				}
			}else
			if(strstr(ChatString,"addpk")){
				char com[200];
				byte nat;
				int strelok=0;
				int z=sscanf(ChatString,"%s%d%d",com);
				if(z>0){
					if(TempGrp.Index!='UNIT') RegisterDynGroup(&TempGrp);
					SaveSelectedUnits(MyNation,&TempGrp,0);
					AddPsKillers(&TempGrp,MyNation,false);
				}
			}else
			if(strstr(ChatString,"addstm")){
				char com[200];
				//byte nat;
				int dif=0;
				int z=sscanf(ChatString,"%s%d%d",com,&dif);
				if(z>0){
					if(TempGrp.Index!='UNIT') RegisterDynGroup(&TempGrp);
					SaveSelectedUnits(MyNation,&TempGrp,0);
					AddStorm(&TempGrp,MyNation,dif);
				}
			}else
#endif
			if(!strcmp(ChatString,"treasure")){
#ifndef AI_CHEATS
				CurrentSinBattle=-1;
#endif
				CmdMoney(MyNation);
			}
			else if(NPlayers<2){
				char cc1[64];
				int n1,n2,n3;
				int z=sscanf(ChatString,"%s%d%d%d",cc1,&n1,&n2,&n3);
				if(z==3&&!strcmp(cc1,"change")){
					if(n1>=0&&n1<8&&n2>=0&&n2<=8)SelChangeNation(n1,n2);
				};
				if(z==2&&!strcmp(cc1,"addhi")){
					for(int i=0;i<MaxPointIndex;i++){
						THMap[i]+=n1;
					}
					void MakeAllDirtyGBUF();
					MakeAllDirtyGBUF();
				}
				if(z==4&&!strcmp(cc1,"rgb")){
					extern int WaterR;
					extern int WaterG;
					extern int WaterB;
					WaterR=n1;
					WaterG=n2;
					WaterB=n3;
				}
				char cc2[128];
				z=sscanf(ChatString,"%s%s",cc1,cc2);
				if(z==2&&!strcmp(cc1,"changeunit")){
					word Type=0xFFFF;
					for(int i=0;i<NATIONS->NMon;i++){
						if(!strcmp(cc2,NATIONS->Mon[i]->MonsterID)){
							GeneralObject* GO=NATIONS->Mon[i];
							if(!GO->newMons->Building){
								//found!!!
								int N=NSL[MyNation];
								word* IDS=Selm[MyNation];
								word* SMS=SerN[MyNation];
								for(int j=0;j<N;j++){
									word MID=IDS[j];
									if(MID!=0xFFFF){
										OneObject* OB=Group[MID];
										if(OB&&!OB->Sdoxlo){
											OB->Ref.General=GO;
											OB->NIndex=i;
											OB->newMons=GO->newMons;
										};
									};
								};
							};
						};
					};
				};
				int vv=100;
				z=sscanf(ChatString,"%s%d",cc1,&vv);
				if(z==2&&!strcmp(cc1,"bstage")){
					int N=ImNSL[MyNation];
					word* ID=ImSelm[MyNation];
					for(int i=0;i<N;i++){
						OneObject* OB=Group[ID[i]];
						if(OB&&!OB->Sdoxlo&&OB->NewBuilding){
							if(vv>100)vv=100;
							if(vv==100){
								OB->Stage=OB->MoreCharacter->ProduceStages;                                
								OB->Life=OB->MaxLife-1;
								OB->Ready=false;
								OB->NextStage();
							}else{
								OB->Stage=OB->MoreCharacter->ProduceStages*vv/100;
								OB->Life=OB->MaxLife*vv/100;
								OB->Ready=false;
								OB->NextStage();
							}
						}
					}
				}
			};
		}
		/*else{
			//if(!strcmp(ChatString,"SuPeRuSeR"))Superuser=1;
		};*/
		if(ChatString[0]=='g'&&ChatString[1]=='i'&&ChatString[2]=='v'&&ChatString[3]=='e'&&ChatString[4]==' '){
			char* sss=ChatString+5;
			char cc1[100];
			int p=0;
			char c;
			while(sss[0]==' ')sss++;
			do{
				c=sss[0];
				cc1[p]=c;
				if(c!=' ')p++;
				sss++;
			}while(c!=0&&c!=' '&&p<20);
			cc1[p]=0;
			if(c==0)return;
			int res=-1;
			if(!strcmp(cc1,"wood"))res=0;
			else if(!strcmp(cc1,"gold"))res=1;
			else if(!strcmp(cc1,"stone"))res=2;
			else if(!strcmp(cc1,"food"))res=3;
			else if(!strcmp(cc1,"iron"))res=4;
			else if(!strcmp(cc1,"coal"))res=5;
			if(res==-1)return;
			while(sss[0]==' ')sss++;
			p=0;
			do{
				c=sss[0];
				cc1[p]=c;
				if(c>='0'&&c<='9')c=0;
				cc1[p]=c;
				if(c){
					sss++;
					p++;
				};
			}while(c!=0&&p<40);
			int L;
			do{
				L=strlen(cc1);
				if(L&&cc1[L-1]==' ')cc1[L-1]=0;
				else L=0;
			}while(L);
			int Him=-1;
			for(int i=0;i<NPlayers;i++){
				if(!strcmp(cc1,GSets.CGame.PL_INFO[i].name))Him=GSets.CGame.PL_INFO[i].ColorID;
			};
			if(Him==-1)return;
			while(sss[0]==' ')sss++;
			int Amn;
			int z=sscanf(sss,"%d",&Amn);
			if(z!=1)return;
			CmdGiveMoney(MyNation,Him,res,Amn);
		};
#ifdef AI_CHEATS
		if(ChatString[0]=='t'&&ChatString[1]=='o'&&ChatString[2]=='p'&&ChatString[3]==' '){
			int tz,locktype;
			int z=sscanf(ChatString+4,"%d%d",&tz,&locktype);
			if(z){
				if(z==1) locktype=0;
				if(tz>=0&&tz<GetNAreas(locktype)){
					Area* AR=GetTopMap(tz,locktype);
					mapx=(AR->x<<1)-(smaplx>>1);
					mapy=(AR->y<<1)-(smaply>>1);
					if(mapx<=0)mapx=1;
					if(mapy<=0)mapy=1;
					if(mapx+smaplx>msx+1)mapx=msx-smaplx+1;
					if(mapy+smaply>msy+1)mapy=msy-smaply+1;
				};
			};
		};
		if(ChatString[0]=='s'&&ChatString[1]=='e'&&ChatString[2]=='g'&&ChatString[3]==' '){
			int g;
			int n=sscanf(ChatString+4,"%d",&g);
			if(n==1){
				GAMEOBJ grp;
				grp.Index=g;
				grp.Type='UNIT';
				SelectUnits(&grp,0);
			}
		}
		if(ChatString[0]=='w'&&ChatString[1]=='a'&&ChatString[2]=='y'){
			WayMode=1;
			NWay=0;
			if(NSL[MyNation]){
				WayID=Selm[MyNation][0];
			};
		}
#endif
		ChatString[0]=0;
		return;
	};
	if(LastKey>=VK_F1&&LastKey<VK_F9){
		if(CheckFNSend(LastKey-VK_F1)){
			EnterChatMode=0;	
			NKeys=0;
			return;
		};
	};
	
	if(LastKey==VK_LEFT){
		if(ChatCursPos>0)ChatCursPos--;
		KeyPressed=0;
		LastKey=0;
		goto KRT;
	}else
	if(LastKey==VK_RIGHT){
		if(ChatCursPos<int(strlen(ChatString)))ChatCursPos++;
		KeyPressed=0;
		LastKey=0;
		goto KRT;
	}else
	if(LastKey==VK_END){
		ChatCursPos=strlen(ChatString);
		KeyPressed=0;
		LastKey=0;
		goto KRT;
	}else
	if(LastKey==VK_HOME){
		ChatCursPos=0;
		KeyPressed=0;
		LastKey=0;
		goto KRT;
	}else
	if(LastKey==VK_BACK){
		if(ChatCursPos>0){
			strcpy(ChatString+ChatCursPos-1,ChatString+ChatCursPos);
			ChatCursPos--;
		};
		//if(strlen(IB->Str)>0)IB->Str[strlen(IB->Str)-1]=0;
		goto KRT;
	}else
	if(LastKey==46){//VK_DEL
		if(ChatCursPos<int(strlen(ChatString)))strcpy(ChatString+ChatCursPos,ChatString+ChatCursPos+1);
		//if(strlen(IB->Str)>0)IB->Str[strlen(IB->Str)-1]=0;
		goto KRT;
	}else{
		if(LastAscii){
			LastKey=LastAscii;
			char xx[2];
			xx[1]=0;
			xx[0]=char(LastKey);
			if(strlen(ChatString)+1<80){
				char ccc[2048];
				strcpy(ccc,ChatString);
				ccc[ChatCursPos]=0;
				strcat(ccc,xx);
				strcat(ccc,ChatString+ChatCursPos);
				strcpy(ChatString,ccc);
				ChatCursPos++;
			};
			goto KRT;
		};
	};
	if(NKeys)goto KRT;
	ChatString[80]=0;
	if(ChatCursPos>int(strlen(ChatString)))ChatCursPos=strlen(ChatString);
/*







	//if(!KeyPressed)return;
KRT:
	if(!NKeys)return;
	LastKey=ReadKey();
	KeyPressed=0;
	if(LastKey==27){
		EnterChatMode=0;
		ChatString[0]=0;
		return;
	};
	if(LastKey>=VK_F1&&LastKey<=VK_F9){
		if(CheckFNSend(LastKey-VK_F1)){
			EnterChatMode=0;	
			NKeys=0;
			return;
		};
	};
	if(LastKey==13){
		EnterChatMode=0;
		AssignHint1(ChatString,250,16+MyNation);
		SendChat(ChatString,GetKeyState(VK_CONTROL)&0x8000);
		if(NPlayers<2){
			if(!strcmp(ChatString,"www")){
				ChangeNation=1;
				MultiTvar=1;
				FogMode=0;
			}else
			if(!strcmp(ChatString,"VICTORY")){
				if(NPlayers<2)ShowVictory();
			}else
			if(!strcmp(ChatString,"IZMENA")||!strcmp(ChatString,"izmena"))ChangeNation=!ChangeNation;
			if(!strcmp(ChatString,"MULTITVAR")||!strcmp(ChatString,"multitvar"))MultiTvar=!MultiTvar;
			if(!strcmp(ChatString,"SUPERVIZOR")||!strcmp(ChatString,"supervizor"))FogMode=!FogMode;
			if(!strcmp(ChatString,"MONEY")||!strcmp(ChatString,"money"))CmdMoney(MyNation);
			else if(NPlayers<2){
				char cc1[64];
				int n1,n2;
				int z=sscanf(ChatString,"%s%d%d",cc1,&n1,&n2);
				if(z==3&&!strcmp(cc1,"change")){
					if(n1>=0&&n1<8&&n2>=0&&n2<=8)SelChangeNation(n1,n2);
				};
			};
		}else{
			//if(!strcmp(ChatString,"SuPeRuSeR"))Superuser=1;
		};
		if(ChatString[0]=='g'&&ChatString[1]=='i'&&ChatString[2]=='v'&&ChatString[3]=='e'&&ChatString[4]==' '){
			char* sss=ChatString+5;
			char cc1[100];
			int p=0;
			char c;
			while(sss[0]==' ')sss++;
			do{
				c=sss[0];
				cc1[p]=c;
				if(c!=' ')p++;
				sss++;
			}while(c!=0&&c!=' '&&p<20);
			cc1[p]=0;
			if(c==0)return;
			int res=-1;
			if(!strcmp(cc1,"wood"))res=0;
			else if(!strcmp(cc1,"gold"))res=1;
			else if(!strcmp(cc1,"stone"))res=2;
			else if(!strcmp(cc1,"food"))res=3;
			else if(!strcmp(cc1,"iron"))res=4;
			else if(!strcmp(cc1,"coal"))res=5;
			if(res==-1)return;
			while(sss[0]==' ')sss++;
			p=0;
			do{
				c=sss[0];
				cc1[p]=c;
				if(c>='0'&&c<='9')c=0;
				cc1[p]=c;
				if(c){
					sss++;
					p++;
				};
			}while(c!=0&&p<40);
			int L;
			do{
				L=strlen(cc1);
				if(L&&cc1[L-1]==' ')cc1[L-1]=0;
				else L=0;
			}while(L);
			int Him=-1;
			for(int i=0;i<NPlayers;i++){
				if(!strcmp(cc1,GSets.CGame.PL_INFO[i].name))Him=GSets.CGame.PL_INFO[i].ColorID;
			};
			if(Him==-1)return;
			while(sss[0]==' ')sss++;
			int Amn;
			int z=sscanf(sss,"%d",&Amn);
			if(z!=1)return;
			CmdGiveMoney(MyNation,Him,res,Amn);
		};
		ChatString[0]=0;
		return;
	};
	if(LastKey==VK_BACK){
		if(strlen(ChatString)>0)ChatString[strlen(ChatString)-1]=0;
		return;
	}else{
		LastKey=LastAscii;
		if(LastKey){
		
			char xx[2];
			xx[1]=0;
			xx[0]=char(LastKey);
			if(strlen(ChatString)<80)strcat(ChatString,xx);
		};
	};
	if(NKeys)goto KRT;
	*/
};
extern char CHATSTRING[256];
extern DWORD CHATDPID;
void ShowMultiplayerChat();
void ShowChat(){
	/*if(NPlayers>0){
		ShowMultiplayerChat();
		return;
	};*/
	if(EnterChatMode){
		int y0=smapy+(smaply<<(Shifter-2));
		int x0=smapx+(smaplx<<(Shifter-1));
		int Dy=GetRLCHeight(BigYellowFont.RLC,'y')+5;		
		//
		static char* ctit=GetTextByID("#ChatTitle");
		static char* caly=GetTextByID("#ChatAlly");
		static char* call=GetTextByID("#ChatToAll");
		static char* ccht=GetTextByID("#ChatCheat");		
		char* cstr=(GetKeyState(VK_CONTROL)&0x8000)?caly:call;
		if(NPlayers==1){
			cstr=ccht;
		}
		//
		int L0=GetRLCStrWidth(cstr,&BigBlackFont);
		int L=(GetRLCStrWidth(ChatString,&YellowFont)+L0)>>1;
		if(L<100)L=100;
		int L1=GetRLCStrWidth(ctit,&SmallYellowFont);
		if(L<L1)
			L=L1;
		//
		int addH=14;
		int addY0=26;
		if(NPlayers==1){
			addH=0;
			addY0=0;			
		}
		//
		y0-=addH/2;
		//
		GPS.SetCurrentDiffuse(0xB0FFFFFF);
		DrawStdBar(x0-L-16,y0-8,x0+L+16,y0+27+addH);
		GPS.SetCurrentDiffuse(0xFFFFFFFF);
		//
		if(NPlayers!=1){
			ShowString(x0-L-4,y0-10+v_ISys.ChatTitleDy+addY0,ctit,&SmallYellowFont);
		}		
		ShowString(x0-L-4,y0-10+v_ISys.ChatTitleDy,cstr,&BigYellowFont);
		//
		y0+=v_ISys.ChatStringDy;
		//		
		if((GetRealTime()/200)&1)ShowString(x0-L+L0+4,y0-10,ChatString,&WhiteFont);
		else{
			int xx=0;
			for(int i=0;i<ChatCursPos;i++)xx+=GetRLCWidth(WhiteFont.RLC,ChatString[i]);
			//sprintf(cc2,"%s|",ChatString);
			ShowString(x0-L+L0+4,y0-10,ChatString,&WhiteFont);
			ShowString(x0-L+L0+xx+4,y0-10,"|",&WhiteFont);
		};
	};
	if(CHATSTRING[0]){
		for(int i=0;i<NPlayers;i++)if(GSets.CGame.PL_INFO[i].PlayerID==CHATDPID){
			char cc1[250];
			sprintf(cc1,"%s: %s",GSets.CGame.PL_INFO[i].name,CHATSTRING);
			AssignHint1(cc1,250,16+GSets.CGame.cgi_NatRefTBL[GSets.CGame.PL_INFO[i].ColorID]);
			PlayEffect(GetSound("START"),0,0);
			CHATSTRING[0]=0;
			i=NPlayers;
		};
	};
};
int NGSPlayers;
char GSPLAYERS[9][32];
bool ProcessIndividualMessages(char* nick,char* message){
	if(GameInProgress){
		char cc3[1024];
		sprintf(cc3,GetTextByID("COMCHAT"),nick,message);
		AssignHint1(cc3,400,0);
		PlayEffect(GetSound("START"),0,0);
		if(NGSPlayers>7)return true;
		for(int i=0;i<NGSPlayers;i++){
			if(!_stricmp(GSPLAYERS[i],nick))return true;
		};
		strcpy(GSPLAYERS[NGSPlayers],nick);
		NGSPlayers++;
		return true;
	}else return false;
};

char* CHATHELP1=NULL;
char* CHATHELP2=NULL;
char* CHATHELP3=NULL;
void DrawDoubleTable2(int x0,int y0,int x1,int y1,int ym);
void ShowMultiplayerChat(){
	if(EnterChatMode){
		if(!CHATHELP1){
			CHATHELP1=GetTextByID("CHATHELP1");
			CHATHELP2=GetTextByID("CHATHELP2");
			CHATHELP3=GetTextByID("CHATHELP3");
		};
		int DDD=20;
		int y0=smapy+(smaply<<(Shifter-2))-50-DDD/2;
		int x0=smapx+(smaplx<<(Shifter-1));
		int Dy=GetRLCHeight(BigYellowFont.RLC,'y')+5;
		int L0=GetRLCStrWidth(ENCHAT,&BigYellowFont);
		int L=GetRLCStrWidth(ChatString,&YellowFont)+L0;
		int L1=GetRLCStrWidth(CHATHELP1,&SmallWhiteFont);
		int L2=GetRLCStrWidth(CHATHELP2,&SmallWhiteFont);
		if(L2>L1)L1=L2;
		char cc3[256];
		for(int i=0;i<NGSPlayers;i++){
			sprintf(cc3,CHATHELP3,i+1,GSPLAYERS);
			L2=GetRLCStrWidth(cc3,&SmallWhiteFont);
			if(L2>L1)L1=L2;
		};
		if(L<L1)L=L1;
		L>>=1;
		if(L<100)L=100;
		//DrawStdBar(x0-L-16,y0-8,x0+L+16,y0+20+7);
		DrawDoubleTable2(x0-L-16,y0-8,x0+L+16,y0+20+7+DDD*(2+NGSPlayers)+16,y0+20+7);
		ShowString(x0-L-4,y0-5-8,ENCHAT,&BigWhiteFont);
		if((GetRealTime()/200)&1)ShowString(x0-L+L0+4,y0-4+7,ChatString,&YellowFont);
		else{
			int xx=0;
			for(int i=0;i<ChatCursPos;i++)xx+=GetRLCWidth(YellowFont.RLC,ChatString[i]);
			//sprintf(cc2,"%s|",ChatString);
			ShowString(x0-L+L0+4,y0-4+7,ChatString,&YellowFont);
			ShowString(x0-L+L0+xx+4,y0-4+7,"|",&YellowFont);
		};
		ShowString(x0-GetRLCStrWidth(CHATHELP1,&SmallYellowFont)/2,y0+20+7+6+DDD/2,CHATHELP1,&SmallYellowFont);
		ShowString(x0-GetRLCStrWidth(CHATHELP2,&SmallYellowFont)/2,y0+20+7+6+DDD/2+DDD,CHATHELP2,&SmallYellowFont);
		for(int i=0;i<NGSPlayers;i++){
			sprintf(cc3,CHATHELP3,i+1,GSPLAYERS[i]);
			L2=GetRLCStrWidth(cc3,&SmallWhiteFont);
			ShowString(x0-L2/2,y0+20+7+6+DDD/2+DDD*(i+2),cc3,&SmallYellowFont);
		};
	};
	if(CHATSTRING[0]){
		for(int i=0;i<NPlayers;i++)if(GSets.CGame.PL_INFO[i].PlayerID==CHATDPID){
			char cc1[250];
			sprintf(cc1,"%s: %s",GSets.CGame.PL_INFO[i].name,CHATSTRING);
			AssignHint1(cc1,250,16+GSets.CGame.cgi_NatRefTBL[GSets.CGame.PL_INFO[i].ColorID]);
			PlayEffect(GetSound("START"),0,0);
			CHATSTRING[0]=0;
			i=NPlayers;
		};
	};
};
bool CheckFNSend(int idx){
	if(!EnterChatMode)return false;
	if(idx<NGSPlayers){
		if(TPEN.Peer&&TPEN.Connected&&ChatString[0]){
			peerMessagePlayer(TPEN.Peer,GSPLAYERS[idx],ChatString);
			char cc3[1024];
			sprintf(cc3,GetTextByID("CHATHELP4"),GSPLAYERS[idx],ChatString);
			AssignHint1(cc3,100,0);
			ChatString[0]=0;
		};
		EnterChatMode=0;
		return true;
	};
	return false;
};
void GetTimeString(int t,char* str){
	int h=t/3600;
	int m=(t/60)%60;
	int s=(t%60);
	sprintf(str,"%d:%d%d:%d%d",h,m/10,m%10,s/10,s%10);
};
int SortPlayers(byte* Res,int* par);
void ShowVictInFormat(char* Res,int idx,byte Type);
void SendInfoString(char* nick){
	if(!(TPEN.Peer&&TPEN.Connected&&!TPEN.ChatConnected))return;
	char BIGINFO[2048];
	sprintf(BIGINFO,"@@@GMRIN %s\\",TPEN.HostMessage);
	switch(GSets.CGame.PL_INFO[0].VictCond){
	case 0:
		strcat(BIGINFO,"Total Destruction\\");
		break;
	case 1 :
		strcat(BIGINFO,"Territory Capture ");
		GetTimeString(GSets.CGame.PL_INFO[0].GameTime,BIGINFO+strlen(BIGINFO));
		strcat(BIGINFO,"\\");
		break;
	case 2 :
		strcat(BIGINFO,"Game on score ");
		GetTimeString(GSets.CGame.PL_INFO[0].GameTime,BIGINFO+strlen(BIGINFO));
		strcat(BIGINFO,"\\");
		break;
	default:
		return;
	};
	byte ord[8];
	int  par[8];
	int no=SortPlayers(ord,par);
	byte prevms=0;
	int teamidx=0;
	if(no){
		byte prevms=0;
		for(int q=0;q<no;q++){
			byte ms=NATIONS[ord[q]].NMask;
			if(!(ms&prevms))teamidx++;
			prevms=ms;
			char ccr[64];
			ShowVictInFormat(ccr,ord[q],GSets.CGame.PL_INFO[0].VictCond);
			sprintf(BIGINFO+strlen(BIGINFO),"%d %s (%s)\\",teamidx,GetPName(ord[q]),ccr);
		};
	};
	if(strlen(BIGINFO)<200)peerMessagePlayer(TPEN.Peer,nick,BIGINFO);
};
#include "Dialogs\EditorToolsPanel.h"
//-----------------Clans support----------------//
struct ClanInfo{
	char* ClanName;
	int IconID;
};
ClanInfo* CLINFO=NULL;
int NClans=0;
void ReadClanData(){
	WIN32_FIND_DATA FD;
	HANDLE H;
	H=FindFirstFile("Internet\\Cash\\Clans\\tmp@*.g2d",&FD);
	if(H!=INVALID_HANDLE_VALUE){
		do{
			char* ccc;
			ccc=FD.cFileName;
			char fnm[128];
			sprintf(fnm,"Internet\\Cash\\Clans\\%s",ccc+4);
			ResFile F=RRewrite(fnm);
			if(F!=INVALID_HANDLE_VALUE){
				sprintf(fnm,"Internet\\Cash\\Clans\\%s",FD.cFileName);
				ResFile F1=RReset(fnm);
				if(F1!=INVALID_HANDLE_VALUE){
					int sz=RFileSize(F1);
					void* Buf=malloc(sz);
					RBlockRead(F1,Buf,sz);
					RBlockWrite(F,Buf,sz);
					free(Buf);
					RClose(F1);
					DeleteFile(fnm);
				}
				RClose(F);
			}
		}while(FindNextFile(H,&FD));
	}
	FindClose(H);
	H=FindFirstFile("Internet\\Cash\\Clans\\*.g2d",&FD);
	if(H!=INVALID_HANDLE_VALUE){
		do{
			char ccc[64];
			int ch=0;
			while(FD.cFileName[ch] && FD.cFileName[ch]!=46){
				ccc[ch]=FD.cFileName[ch];
				ch++;
			}
			ccc[ch]=0;
			CLINFO=(ClanInfo*)realloc(CLINFO,(NClans+1)*sizeof ClanInfo);
			memset(CLINFO+NClans,0,sizeof ClanInfo);
			char cc1[128];
			sprintf(cc1,"Internet\\Cash\\Clans\\%s",ccc);
			CLINFO[NClans].IconID=GPS.PreLoadGPImage(cc1);
			CLINFO[NClans].ClanName=znew(char,strlen(ccc)+1);
			strcpy(CLINFO[NClans].ClanName,ccc);
			NClans++;
			
		}while(FindNextFile(H,&FD));
	}
	FindClose(H);

	/*byte pset[256];
	ResFile F=RReset("1\\old2new.set");
	if(F==INVALID_HANDLE_VALUE)return;
	RBlockRead(F,pset,256);
	RClose(F);
	WIN32_FIND_DATA FD;
	HANDLE H=FindFirstFile("Clans\\*.clan",&FD);
	if(H!=INVALID_HANDLE_VALUE){
		do{
			char ccc[128];
			sprintf(ccc,"Clans\\%s",FD.cFileName);
			FILE* F=fopen(ccc,"r");
			if(F){
				char cc1[128];
				char cc2[128];
				char cc3[128];
				int z=fscanf(F,"%s%s%s",cc1,cc2,cc3);
				if(z==3){
					SQPicture P1;
					sprintf(ccc,"Clans\\%s",cc1);
					P1.LoadPicture(ccc);
					if(P1.GetLx()<256&&P1.GetLy()==20){
						sprintf(ccc,"Clans\\%s",cc2);
						P1.LoadPicture(ccc);
						if(P1.GetLx()<256&&P1.GetLy()==20){
							sprintf(ccc,"Clans\\%s",cc3);
							P1.LoadPicture(ccc);
							if(P1.GetLx()<256&&P1.GetLy()==20){
								CLINFO=(ClanInfo*)realloc(CLINFO,(NClans+1)*sizeof ClanInfo);
								memset(CLINFO+NClans,0,sizeof ClanInfo);
								sprintf(ccc,"Clans\\%s",cc1);
								CLINFO[NClans].Over.LoadPicture(ccc);
								CLINFO[NClans].Over.Xlat(pset);
								sprintf(ccc,"Clans\\%s",cc2);
								CLINFO[NClans].Active.LoadPicture(ccc);
								CLINFO[NClans].Active.Xlat(pset);
								sprintf(ccc,"Clans\\%s",cc3);
								CLINFO[NClans].Passive.LoadPicture(ccc);
								CLINFO[NClans].Passive.Xlat(pset);
								strcpy(ccc,FD.cFileName);
								char* ccx=strstr(ccc,".clan");
								if(!ccx)ccx=strstr(ccc,".CLAN");
								if(!ccx)ccx=strstr(ccc,".Clan");
								if(ccx)ccx[0]=0;
								CLINFO[NClans].ClanName=znew(char,strlen(ccc)+1);
								strcpy(CLINFO[NClans].ClanName,ccc);
								NClans++;
							};
						};
					};
				};
				fclose(F);
			};
		}while(FindNextFile(H,&FD));
		FindClose(H);
	};*/
	//sorting
	bool change;
	ClanInfo CIN;
	do{
		change=0;
		for(int i=1;i<NClans;i++){
			if(strlen(CLINFO[i-1].ClanName)<strlen(CLINFO[i].ClanName)){
				CIN=CLINFO[i-1];
				CLINFO[i-1]=CLINFO[i];
				CLINFO[i]=CIN;
				change=1;
			};
		};
	}while(change);
	memset(&CIN,0,sizeof CIN);

};
CEXPORT
void ShowClanString(int x,int y,char* s,byte State,RLCFont* Fn,RLCFont* Fn1,int DY,int LX){
	char NICK[128];
	char CLAN[128];
	strcpy(NICK,s);
	_strupr(NICK);
	for(int i=0;i<NClans;i++){
		sprintf(CLAN,"[%s]",CLINFO[i].ClanName);
		_strupr(CLAN);
		char* cc=strstr(NICK,CLAN);
		if(cc && cc==NICK && CLINFO[i].IconID!=-1){
			//ok, found!
			int height=GPS.GetGPHeight(CLINFO[i].IconID,0);
			int DX=GPS.GetGPWidth(CLINFO[i].IconID,0);
			GPS.ShowGP(x,y+(DY-height)/2+1,CLINFO[i].IconID,0,0);
			strcpy(cc,s+strlen(CLAN));
			LimitString(cc,Fn,LX-DX);
			if(Fn1) ShowString(x+DX+7,y-5,cc,Fn1);
			ShowString(x+DX+6,y-5-1,cc,Fn);
			return;
			/*cc=ccc;
			cc+=strlen(CLINFO[i].ClanName);
			SQPicture* p;
			switch(State){
			case 0:
				p=&CLINFO[i].Passive;
				break;
			case 1:
				p=&CLINFO[i].Active;
				break;
			case 2:
				p=&CLINFO[i].Over;
				break;
			};
			p->Draw(x,y-2);
			if(Fn1)ShowString(x+p->GetLx()+1+3,y+DY+1,cc,Fn1);
			ShowString(x+p->GetLx()+3,y+DY,cc,Fn);
			return;*/
		};
	};
	LimitString(s,Fn,LX);
	if(Fn1) ShowString(x+1,y-5,s,Fn1);
	ShowString(x,y-5-1,s,Fn);
	return;
};
//--------------DIPLOMACY IMPLEMENTATION-------------
DialogsSystem DIP_DSS(0,0);
bool DIP_DSS_Init=0;
bool DIP_DSS_Vis=0;
char CUR_Request[256]="LF|LW_new|open&req1.txt";
void RunExplorer(int Index,char* ref,int x,int y,int x1,int y1);
int LastRealLX=-1;
void SetupDIP_DSS(){
	LastRealLX=RealLx;
	DIP_DSS.CloseDialogs();
	int DD1=75;
	int D=18;
	int x0=RealLx-InfDX;
	int y0=InfDY;
	DIP_DSS.BaseX=x0-1;
	DIP_DSS.BaseY=y0+31;
	//DIP_DSS.addBorder(0,-31,InfDX-10,RealLy-380,0,1);
	DIP_DSS.addTextButton(NULL,10,-29,GetTextByID("DIP_HDR"),&YellowFont,&YellowFont,&YellowFont,0);
	RunExplorer(1,CUR_Request,DIP_DSS.BaseX-2,DIP_DSS.BaseY-2,DIP_DSS.BaseX+InfDX-10+3,RealLy-298);
};
void ExplorerResize(int Index,int x,int y,int x1,int y1);
extern int InformMode;
int PrevDipDL=0;
extern int DipDlgDX;
void ProcessDIP_DSS(){
	if(RealLx!=LastRealLX||PrevDipDL!=InfDX){
		LastRealLX=RealLx;
		DIP_DSS.CloseDialogs();
		int DD1=75;
		int D=18;
		int x0=RealLx-InfDX;
		int y0=InfDY;
		DIP_DSS.BaseX=x0-1;
		DIP_DSS.BaseY=y0+31;
		//DIP_DSS.addBorder(0,-31,InfDX-10,RealLy-380,0,1);
		//DIP_DSS.addTextButton(NULL,10,-24,GetTextByID("DIP_HDR"),&YellowFont,&YellowFont,&YellowFont,0);
		ExplorerResize(1,DIP_DSS.BaseX-2,DIP_DSS.BaseY-2,DIP_DSS.BaseX+InfDX-10+3,InfY1-27);
	};
	PrevDipDL=InfDX;
	if((!Inform)||(Inform&&InformMode!=4)||(EditMapMode&&!OptHidden))return;
	if(NoPress&&mouseX>RealLx-InfDX-32)return;
	if(!DIP_DSS_Init){
		DIP_DSS_Init=1;
		SetupDIP_DSS();
	};
	if(DIP_DSS_Vis){
		DIP_DSS.ProcessDialogs();
		ProcessExplorer(1);
	};
};
CEXPORT
void ActivateDipDialog(char* request){
	Inform=1;
	InformMode=4;
	OptHidden=1;
	DIP_DSS_Vis=1;
	strcpy(CUR_Request,request);
	if(!DIP_DSS_Init){
		DIP_DSS_Init=1;
		SetupDIP_DSS();
	}else RunExplorer(1,CUR_Request,DIP_DSS.BaseX-2,DIP_DSS.BaseY-2,DIP_DSS.BaseX+InfDX-10+3,InfY1-27);
};
CEXPORT
void ActivateCMLInterface(int Idx,char* request){
	RunExplorer(Idx,request,0,0,RealLx,RealLy);
};
CEXPORT
void DrawCMLInterface(int Idx){
	ProcessExplorer(Idx);		
};
CEXPORT
void ChangeDipDialog(char* request){
	strcpy(CUR_Request,request);
	if(!DIP_DSS_Init){
		DIP_DSS_Init=1;
		SetupDIP_DSS();
	}else RunExplorer(1,CUR_Request,DIP_DSS.BaseX-2,DIP_DSS.BaseY-2,DIP_DSS.BaseX+InfDX-10+3,InfY1-27);
};
extern int InformMode;
CEXPORT
void PopupDipDialog(){
	Inform=1;
	OptHidden=1;
	DIP_DSS_Vis=1;
	InformMode=4;
};
extern int InfDX0;
void FlipDipDialog(){
	if(Inform||!OptHidden)DIP_DSS_Vis=0;
	if(!DIP_DSS_Vis){
		Inform=1;
		InformMode=4;
		OptHidden=1;
		DIP_DSS_Vis=1;
	}else{
		Inform=0;
		DIP_DSS_Vis=0;
		InfDX=InfDX0;
	};
};
void HideDipDialog(){
	Inform=0;
	DIP_DSS_Vis=0;
	InfDX=InfDX0;
};
void CmdSendData(byte* Data,int size);
byte* DIP_Data[64];
int DIP_DSize[64];
int NDipItems=0;
CEXPORT
void SendDipCommand(char* Data,int size){
	if(NDipItems<64){
		DIP_Data[NDipItems]=(byte*)malloc(size);
		memcpy(DIP_Data[NDipItems],Data,size);
		DIP_DSize[NDipItems]=size;
		NDipItems++;
	};
	//CmdSendData((byte*)Data,size);
};
//#define AC_DEMO
__declspec(dllexport)
bool CheckUsingAI(){
	for(int i=0;i<8;i++)if(NATIONS[i].AI_Enabled)return true;
	return false;
};
//#define  EDITORONLY
bool PIEnumeratePlayers(PlayerInfo* PIN,bool DoMsg);
CEXPORT bool EnumPlr(){
	return PIEnumeratePlayers(GSets.CGame.PL_INFO,false);
};
extern int curptr;
extern int MouseNext[256];
int prevptr=-1;
CEXPORT void SetCurPtr(int v){
	/*
	if(v==0&&curptr)prevptr=curptr;
	else curptr=prevptr;
	int N=30;
	int v0=v;
	do{
		v0=MouseNext[v0];
		if(v0==curptr)return;
		N--;
	}while(N);
	*/
	curptr=v;
};
CEXPORT int GetCurPtr(){
	return curptr;
}
void DelFinSpaces(char* s){
	int L=strlen(s);
	while(L>0&&s[L-1]==' ')L--;
	s[L]=0;
};
void ParseMP3_String(char* src,char* txt,char* mp3){
	char* ms=strstr(src,"MP3:");
	if(ms){
		strcpy(txt,src);
		txt[ms-src]=0;
		ms+=4;
		while(ms[0]==' ')ms++;
		strcpy(mp3,ms);
	}else{
		strcpy(txt,src);
		mp3[0]=0;
	};
	DelFinSpaces(txt);
	DelFinSpaces(mp3);
};
char* GetCurrentMP3File();
void StopPlayingMP3();
void PlayMP3File(char* FileName);
BOOL mpl_IsStreamFinished(void);

#include "Dialogs\ProcessSingleMission.h"
#include "Dialogs\ProcessSingleCampagin.h"
#include "Dialogs\EnterName.h"
#include "Dialogs\MainMenu.h"
#include "Dialogs\MenuOptions.h"
#include "Dialogs\ProcessCampagins.h"
#include "Dialogs\ProcessMultiplayer.h"
#include "Dialogs\ProcessSinglePlayer.h"
#include "Dialogs\SelectProtocol.h"
#include "Dialogs\SingleOptions.h"
#include "Dialogs\MultiplayerRoom.h"
#include "Dialogs\SelectSingleMission.h"
#include "Dialogs\MLoadGame.h"
#include "Dialogs\InGameLoadGame.h"
#include "Dialogs\InGameSaveGame.h"
#include "Dialogs\GameMainMenu.h"
#include "Dialogs\EditMainMenu.h"
#include "Dialogs\ProcessLoadingFile.h"
#include "Dialogs\ProcessSavingFile.h"
#include "Dialogs\SelectGameToJoin.h"
#include "Dialogs\GameOptions.h"
#include "Dialogs\HistoryBattle.h"
#include "Dialogs\Messages.h"
#include "Dialogs\PostCampaignMessage.h"
#include "Dialogs\SelectSingleBattle.h"
#include "AI_Scripts.h"								//Vitya //  [11/11/2003]
#include "Dialogs\MISSIONSCRIPT\MissionScript.h"	//Vitya //  [11/11/2003]
#include "Dialogs\MISSIONSCRIPT\EndMissionData.h"	//Vitya //  [11/12/2003]
#include "Dialogs\BigMapQuest\CQuest.h"				//Grey		[27.02.2004]
#include "Dialogs\ClasDeclaration.h"				//Vitya	
#include "Dialogs\CPlayerSave.hpp"					//Grey	//  [02/10/2003]
#include "Dialogs\BigMapDataStr.h"					//Vitya
#include "Dialogs\DiplomacyStr.h"					//Grey
#include "Dialogs\ProcessBigMap.h"					//Vitya
//#include "Dialogs\PlayerInterface.h"				//Grey	//  [02/10/2003]
#include "GlobalBrigDialog.h"


// ShowTimeSystem
bool ShowTime(SimpleDialog* SD){
	Lpressed=0;
	return 0;
};
#include <typeinfo>
#define REGFUNC(x) RegisterOnClickFunction(#x,x)
CEXPORT bool RegisterOnClickFunction(char* Identf, VCall*  Fun);
DialogsSystem* STS_DS=NULL;
void ShowTimeSystem(){
	//const char* s=typeid(RegisterOnClickFunction).raw_name();
	if(!STS_DS){
		STS_DS=new DialogsSystem;
		REGFUNC(ShowTime);
		STS_DS->LoadScreenFromFile("Interf3\\ShowTime.xml");
	}
//	STS_DS->ProcessDialogs();
};
bool GetGlobalBrigProp(byte NI){
	const int MC=10;
	const int MO=40;
	const int MT=40;
	word CID[MC];
	word NIndex[MO*MT];
	word Amount[MC*MO*MT];
	byte UO[MC*MO];
	byte UD[MC*MO];
	byte UF[MC*MO];
	//
	extern bool EnableGlobalCreateBrig;
	EnableGlobalCreateBrig=true;
	//
	GetGlobalCreateBrigList(NI,MC,CID,MO,NIndex,Amount,MT,UO,UD,UF);
	//
	if(!NATIONS[NI].AI_Enabled) return false;
	//
	GeneralObject** GO=NATIONS[NI].Mon;
	// Find best proposition
	int C,O,T,A=0,BA=0;	// ba -  ?????????? ? ????? ??????
	word *NINDEX, *AMOUNT;
	word prio=0xFFFF;
	int Gold=GetResource(NI,1);
	for(int c=0;c<MC;c++){
		//x=x0;
		if(CID[c]>=MAXOBJECT) break;
		OneObject* CO=Group[CID[c]];		
		for(int o=0;o<MO;o++){
			word* type=NIndex+MT*o;
			GeneralObject* OfGO=GO[type[0]];			
			for(int t=3;t<MT;t++){				
				word* aa=Amount+MT*(MO*c+o);
				int a=aa[t];
				if(a>0&&OfGO->OFCR){
					OfficerRecord* OfRec=OfGO->OFCR;
					int ba=OfRec->SDES[0].Amount[0];					
					if(Gold>500&&a>(ba*2/3)){
						word CenID=CID[c];
						if(CenID!=0xFFFF){
							OneObject* COB=Group[CenID];
							if(COB){								
								Nation* Nat=NATIONS+NI;
								int NBuild=Nat->PACount[COB->NIndex];
								word* Build=Nat->PAble[COB->NIndex];
								if(EngSettings.MoraleType==0){//Cossacks
									for(int i=0;i<3;i++)if(i!=2||Gold>3000){
										word NIdx=type[i];
										if(NIdx!=0xFFFF){
											GeneralObject* GO=NATIONS[COB->NNUM].Mon[NIdx];
											if(GO->Enabled){
												int oa=COB->CheckAbility(NIdx);
												if(oa==-1){
													switch(i){
														case 0:	oa=UO[MO*c+o]; break;
														case 1:	oa=UD[MO*c+o]; break;
														case 2:	oa=UF[MO*c+o]; break;
													};
												}
												oa+=aa[i];
												if(!oa){
													for(int p=0;p<NBuild;p++) if(NIdx==Build[p]){
														CmdGBD_ProduceUnit(CenID,NIdx);
														break;
													}
												}
											}
										}			
									}
									word NIdx=type[t];								
									if(a<ba){
										int np=a;
										Order1* OR1=COB->LocalOrder;
										while(OR1){
											void ProduceObjLink(OneObject* OBJ);
											if(OR1->DoLink==&ProduceObjLink&&OR1->info.Produce.ObjIndex==NIdx)np++;
											OR1=OR1->NextOrder;
										}
										if(a<ba){										
											for(int p=0;p<NBuild;p++) if(NIdx==Build[p]){
												CmdGBD_ProduceUnit(CenID,NIdx);
												break;
											}
										}
									}
								}
							}
						}
					}
					if(a/ba==0) continue;
					word typ=type[t];
					if(typ==0xFFFF) break;
					if(GPPrio[typ]==0xFFFF) continue;
					if(a>A&&GPPrio[typ]<=prio){ 
						A=a; C=c; O=o; T=t; prio=GPPrio[typ]; BA=ba;
						NINDEX=type; AMOUNT=aa;
					}
				}
			}			
		}
	}
	if(BA){		
		BGParam=NINDEX[0];
		CmdCreateBrigadeGlobal(CID[C],NINDEX[T]);
		return true;
	}
	return false;
}
