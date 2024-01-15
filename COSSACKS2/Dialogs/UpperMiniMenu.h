#include "BE_HEADERS.h"
extern DialogsSystem INFORM;
extern DialogsSystem UNITINF;
extern DialogsSystem MINIMENU;
extern bool ToolsHidden;
extern bool OptHidden;
void ClearMINIMENU(){
	MINIMENU.CloseDialogs();
};
int InformMode=0;
bool Recreate=0;
int GlobalInfDY=0;
int MaxGlobalInfDY=0;
bool InfClick(SimpleDialog* SD){
	InformMode=SD->UserParam;
	Lpressed=false;
	VScrollBar* VS=(VScrollBar*)INFORM.DSS[0];
	VS->SPos=0;
	GlobalInfDY=0;
	return true;
};
char* MMITM[5]={"UM_MENU","UM_INF1","UM_INF2","UM_MINI","UM_HIST"};
void ShowHistory();
extern int MenuType;
extern bool MakeMenu;
extern byte LockGrid;
byte ShowHSTR=0;
extern bool TransMode;
void CreateNEWMAP();
void PrepareGameMedia(byte myid,bool);
void PrepareGameMedia(byte myid,bool SaveNR,bool DisableDLL);
void ShowLoading();
void RetryCQuest();
void CmdOfferVoting();
extern bool RESMODE;
extern int HISPEED;
void ReverseLMode();
void FlipDipDialog();
extern bool DIP_DSS_Vis;
extern int InfDX0;
bool MiniChoose(SimpleDialog* SD){
	switch(SD->UserParam){
	case 0://Menu
		MenuType=1;
		MakeMenu=1;
		break;
	case 1://Unit info
		if(Inform!=2)Inform=2;else{ 
			Inform=0;
			InfDX=InfDX0;
		};
		MiniActive=0;
		DIP_DSS_Vis=0;
		extern int InfDX0;
		InfDX=InfDX0;
		Recreate=1;
		break;
	case 2://game info
		if(Inform!=1)Inform=1;
		else{
			Inform=0;
			InfDX=InfDX0;
		};
		break;
	case 3://Mini map
		FullMini=!FullMini;
		break;
	case 4:
		LockGrid=1-LockGrid;
		break;
	case 5:
		TransMode=!TransMode;
		break;
	case 6://Surrender
		LockPause=1;
		WaitState=1;
		ShowStat=SCENINF.StandartVictory||!SCENINF.hLib;
		if(SCENINF.hLib&&!SCENINF.StandartVictory){
			SCENINF.LooseGame=1;
		};
		CmdEndGame(MyNation,1,116);
		break;
	case 37://Replay
		ShowHSTR=2;
		break;
	case 38://History
		if(NATIONS[MyNation].NHistory)ShowHSTR=1;
		break;
	case 39://show toolbox
		ToolsHidden=0;
		break;
	case 40://show toolbox options
		OptHidden=0;
		break;
	case 41:
		ShowHSTR=23;
		break;
	case 32://tutorial
		TutOver=1;
		break;
	case 33://current issue
		ShowHSTR=3;
		break;
	case 72:
		RESMODE=1;
		UnPress();
		KeyPressed=0;
		break;
	case 42:
		HISPEED=0;
		break;
	case 43:
		HISPEED=2;
		break;
	case 44:
		HISPEED=50;
		break;
	case 45:
		FogMode=0;
		break;
	case 46:
		FogMode=1;
		break;
	case 47:
	case 48:
		ReverseLMode();
		break;
	case 49:
		FlipDipDialog();
		break;
	case 73:
		CmdOfferVoting();
		break;
	};
	Recreate=1;
	Lpressed=false;
	MiniActive=0;
	return false;
};

bool IsGameActive();
extern bool VotingMode;
int MiniMenuX;
CIMPORT bool CheckIfGameRated();
void CreateMiniMenu(){
	MINIMENU.CloseDialogs();
	Recreate=0;
	char* MMITM[32];
	int MMID[32];
	int NInf=0;
	if(Tutorial){
		MMITM[NInf]=GetTextByID("TUTORIAL");
		MMID[NInf]=32;
		NInf++;
		MMITM[NInf]=GetTextByID("UM_CURR");
		MMID[NInf]=33;
		NInf++;
	};
	MMITM[NInf]=GetTextByID("UM_MENU");
	MMID[NInf]=0;
	NInf++;
	if(Inform==2)MMITM[NInf]=GetTextByID("UM_INF2");
	else MMITM[NInf]=GetTextByID("UM_INF1");
	MMID[NInf]=1;
	NInf++;
	if(Inform==1)MMITM[NInf]=GetTextByID("UM_INF4");
	else MMITM[NInf]=GetTextByID("UM_INF3");
	MMID[NInf]=2;
	NInf++;
	MMITM[NInf]=GetTextByID("UM_MINI");
	MMID[NInf]=3;
	NInf++;
	if(LockGrid)MMITM[NInf]=GetTextByID("UI_LOCK2");
	else MMITM[NInf]=GetTextByID("UI_LOCK1");
	MMID[NInf]=4;
	NInf++;
	if(TransMode)MMITM[NInf]=GetTextByID("UM_TRANS1");
	else MMITM[NInf]=GetTextByID("UM_TRANS2");
	MMID[NInf]=5;
	NInf++;
	if(LMode){
		MMITM[NInf]=GetTextByID("4XSCALE_OFF");
		MMID[NInf]=47;
		NInf++;
	}else{
		MMITM[NInf]=GetTextByID("4XSCALE_ON");
		MMID[NInf]=48;
		NInf++;
	};
	if(!(PlayGameMode||EditMapMode)){
		MMITM[NInf]=GetTextByID("UM_SURRENDER");
		MMID[NInf]=6;
		NInf++;
	};
	if(NPlayers>1&&(IsGameActive()||CheckIfGameRated())&&!(VotingMode||PlayGameMode)){
		MMITM[NInf]=GetTextByID("UM_NORT");
		MMID[NInf]=73;
		NInf++;
	};
	if(NPlayers==1&&!(PlayGameMode||EditMapMode)){
		MMITM[NInf]=GetTextByID("UM_REPLAY");
		MMID[NInf]=37;
		NInf++;
	};
	if(NPlayers>1&&!PlayGameMode){
		MMITM[NInf]=GetTextByID("SENDR_L1");
		MMID[NInf]=72;
		NInf++;
	};
	if(NATIONS[MyNation].NHistory){
		MMITM[NInf]=GetTextByID("UM_HIST");
		MMID[NInf]=38;
		NInf++;
	};
	if(ToolsHidden&&EditMapMode){
		MMITM[NInf]=GetTextByID("ED_TLSHOW");
		MMID[NInf]=39;
		NInf++;
	};
	if(OptHidden&&(EditMapMode||PlayGameMode==2||CheckFlagsNeed())){
		MMITM[NInf]=GetTextByID("ED_TOPSHOW");
		MMID[NInf]=40;
		NInf++;
	};
	if(EditMapMode){
		MMITM[NInf]=GetTextByID("NEWMPHDR");
		MMID[NInf]=41;
		NInf++;

	};
	if(!EditMapMode){
		//MMITM[NInf]=GetTextByID("SHOW_DIPLOMACY");
		//MMID[NInf]=49;
		//NInf++;
	};
	if(PlayGameMode){
		if(HISPEED!=1){
			MMITM[NInf]=GetTextByID("REP_NORM");
			MMID[NInf]=42;
			NInf++;
		};
		if(HISPEED!=2){
			MMITM[NInf]=GetTextByID("REP_FAST");
			MMID[NInf]=43;
			NInf++;
		};
		if(HISPEED!=50){
			MMITM[NInf]=GetTextByID("REP_VERY");
			MMID[NInf]=44;
			NInf++;
		};
		if(FogMode){
			MMITM[NInf]=GetTextByID("REP_NOFOG");
			MMID[NInf]=45;
			NInf++;
		}else{
			MMITM[NInf]=GetTextByID("REP_FOG");
			MMID[NInf]=46;
			NInf++;
		};
	};
	int MaxL=0;
	for(int i=0;i<NInf;i++){
		int L=GetRLCStrWidth(MMITM[i],&WhiteFont);
		if(L>MaxL)MaxL=L;
	};
	MaxL+=24;
	MINIMENU.BaseX=MiniMenuX-(MaxL>>1);
	MINIMENU.BaseY=60;
	int NMinLines=6+(NATIONS[MyNation].NHistory!=0);
	if(!MiniActive){
		NMinLines=0;
		if(Tutorial)NInf=2;
	};
	SimpleDialog* VP=MINIMENU.addViewPort(0,0,MaxL,NInf*24);
	VP->Enabled=1;
	int y0=0;
	//if(Tutorial){
	//	GP_TextButton* TB=MINIMENU.addGP_TextButtonLimited(NULL,0,0,MMITM[7],BordGP,74+(i%3),77+(i%3),MaxL,&WhiteFont,&YellowFont);
	//	TB->OnUserClick=&MiniChoose;
	//	TB->UserParam=32;
	//	y0+=24;
	//};
	for(int i=0;i<NInf;i++){
		GP_TextButton* TB=MINIMENU.addGP_TextButtonLimited(NULL,0,y0,MMITM[i],BordGP,74+(i%3),77+(i%3),MaxL,&WhiteFont,&YellowFont);
		TB->OnUserClick=&MiniChoose;
		TB->UserParam=MMID[i];
		y0+=24;
	};
	//MiniActive=0;
};
int DetermineNationAI(byte Nat);
void LoadAIFromDLL(byte Nat,char* Name);
int StartGTime=0;
int StartVCond=0;
void REPLAY(){
	//
	GameExit=false;
	for(int i=0;i<7;i++){
		NATIONS[i].VictState=0;
	}	
	//
	bool lib=SCENINF.hLib!=NULL;
	int n=MISSLIST.CurrentMission;
	int LAND[8];
	int DIFF[8];
	int RONM[8];
	int STRS[8];
	for(int j=0;j<8;j++){
		LAND[j]=CITY[j].LandType;
		DIFF[j]=CITY[j].Difficulty;
		RONM[j]=CITY[j].ResOnMap;
		STRS[j]=CITY[j].StartRes;
	};
	ShowLoading();
	PrepareGameMedia(0,0,1);
	for(int j=0;j<7;j++){
		CITY[j].LandType=LAND[j];
		CITY[j].Difficulty=DIFF[j];
		CITY[j].ResOnMap=RONM[j];
		CITY[j].StartRes=STRS[j];
	};

	
	GSets.CGame.PL_INFO[0].GameTime=StartGTime;
	GSets.CGame.PL_INFO[0].VictCond=StartVCond;
	
	if(lib&&!(SCENINF.hLib||n==-1)){
		MISSLIST.CurrentMission=n;
		for(int i=0;i<7;i++)CITY[i].Difficulty=DIFF[i];
		SCENINF.Load(MISSLIST.MISS[n].DLLPath,MISSLIST.MISS[n].Description);
	};

	// For missions
	DriveMode()->StartMission();
};
extern bool HideBorderMode;
void ProcessMiniMenu(){
	return;
	if(PlayGameMode==1||HideBorderMode)return;

	int ResPanelW = 716;
	int x0=RealLx-ResPanelW;
	int RPMButX=x0-65;
	int RPLButX=x0-65*2;
	
	MiniMenuX = RPMButX+33;
	if(Recreate) CreateMiniMenu();	

	bool MInZone=mouseX>(RPMButX+23)&&mouseX<(RPMButX+44)&&mouseY>8&&mouseY<26;
	if(MInZone){
		//GPS.ShowGP((RealLx-28)/2,1,BordGP,82,0);
		//ShowString(RPMButX+29, 8, "M", &YellowFont);
	};
	bool MInLZone=mouseX>(RPLButX+23)&&mouseX<(RPLButX+44)&&mouseY>8&&mouseY<26;

	if(Lpressed&&!NoPress){
		if(MInZone){
			MiniActive=!MiniActive;
			CreateMiniMenu();
			Lpressed=0;
			UnPress();
		}
		//if(MInLZone) ReverseLMode();
	};


	if(!MINIMENU.DSS[0])CreateMiniMenu();
	MouseOverMiniMenu=MINIMENU.DSS[0]->MouseOver;
	if(Lpressed&&!(MInZone||MouseOverMiniMenu)){
		MiniActive=0;
	};
	if(MiniActive){
		if(NoPress){
			bool Lpress=Lpressed;
			Lpressed=0;
			ShowHSTR=0;
			MINIMENU.ProcessDialogs();
			Lpressed=Lpress;
		}else{ 
			ShowHSTR=0;
			MINIMENU.ProcessDialogs();
		};
		DrawStdRect(MINIMENU.BaseX,MINIMENU.BaseY,MINIMENU.DSS[0]->x1,MINIMENU.DSS[0]->y1);
		if(ShowHSTR==1)ShowHistory();
		if(ShowHSTR==2)REPLAY();
		if(ShowHSTR==3)RetryCQuest();
		if(ShowHSTR==23)CreateNEWMAP();
	}else{
		if(Tutorial){
			if(!MINIMENU.DSS[1])CreateMiniMenu();
			if(NoPress){
				bool Lpress=Lpressed;
				Lpressed=0;
				ShowHSTR=0;
				MINIMENU.ProcessDialogs();
				Lpressed=Lpress;
			}else{
				ShowHSTR=0;
				MINIMENU.ProcessDialogs();
			};
			if(MenuType!=1||MakeMenu!=1){
				DrawStdRect(MINIMENU.BaseX,MINIMENU.BaseY,MINIMENU.DSS[0]->x1,MINIMENU.DSS[0]->y1);
			}
			if(ShowHSTR==1)ShowHistory();
			if(ShowHSTR==2)REPLAY();
			if(ShowHSTR==3)RetryCQuest();
		};
	};
};