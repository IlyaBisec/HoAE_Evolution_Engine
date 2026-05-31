#include <stdio.h>
#include <time.h>

DLLEXPORT bool RegisterUnitType(GAMEOBJ*,char*);

bool OneSinBattle::Load(word id){
	memset(this,0,sizeof *this);

	if(id<MISSLIST.NMiss){
		GID=id;
		//SingleMission* 
		SM=MISSLIST.MISS+id;
		////
		char ScName[64];
		strcpy(ScName,SM->MapName);
		int L=strlen(ScName);
		ScName[L-3]='s';
		ScName[L-2]='c';
		ScName[L-1]='r';
		GFILE* F=Gopen(ScName,"r");
		if(F){
			char line[256];
			int curm=0;			// current mode
			int n;
			while(ReadWinString(F,line,256)){
				if(line[0]=='/'&&line[1]=='/') continue;
				if(	strstr(line,"[Player")||
					strstr(line,"[Defeat]")||
					strstr(line,"[AIUpg")||
					strstr(line,"[UpgEnabled]")||
					strstr(line,"[ArmyValue]")){
					curm=0;					
				}else
				if(strstr(line,"[DifBonus]")){
					curm=1;
					n=0;
				}else
				if(strstr(line,"[ArmyBonus]")){
					curm=2;
					NArmyBonus=0;					
				}else

				if(curm){
					if(curm==1&&n<5){
						if(sscanf(line,"%d",DifBonus+n)){
							n++;
						}						
					}else
					if(curm==2&&NArmyBonus<64){
						if(sscanf(line,"%d%d",ArmyMen+NArmyBonus,ArmyBonus+NArmyBonus)==2){
							NArmyBonus++;
						}
						
					}//else
				
				}
			};
			Gclose(F);
		}
		////
		return true;
	}else{
		GID=0xFFFF;
		return false;
	}
};
SinBattles::SinBattles(){
}
void SinBattles::Init(){
	N=0;
	GFILE* F=Gopen("Missions\\SingleBatl.txt","r");
	if(F){	
		if(Gscanf(F,"%d",&N)){
			char MissID[256];
			int id=0;
			for(int j=0;j<N;j++){
				if(Gscanf(F,"%s",MissID)) for(int i=0;i<MISSLIST.NMiss;i++){
					SingleMission* SM=MISSLIST.MISS+i;
					if(!strcmp(SM->ID,MissID)){						
						if(Battle[id].Load(i)){
							id++;
							break;
						}else{
							char buf[256];
							sprintf(buf,"Invalid battles: %s",MissID);
							ErrM(buf);
						}
					}
				}
			}
		}else{
			ErrM("Invalid number of battles in Missiions\\SingleBatl.txt");
		};
		Gclose(F);
	}else{
		MessageBox(NULL,"Could not open Missions\\SingleBatl.txt","Battles loading failed...",MB_TOPMOST);
		assert(0);
	}
};

extern SinBattles SBattles;

int GetNSBattle(){
	return SBattles.N;
};

CEXPORT byte* GetBattleArmyValue(){
	if(CurrentSinBattle!=-1){
		return SBattles.Battle[CurrentSinBattle].ArmyValue;
	}else{
		return NULL;
	}
}

extern int SBDScore[128];
extern int SBTScore[128];

///////////////////

struct OneSinPlayer{
	char Name[64];
	//int BattleScore[128];
	
	int DifScore[128];	// difficulty score
	int TacScore[128];	// tactical score

	int GetFullScore();

	void Save();
	void Load(char *name);
};

int OneSinPlayer::GetFullScore(){
	int pts=0;
	for(int i=0;i<GetNSBattle();i++){
		pts+=DifScore[i];
		pts+=TacScore[i];
	}
	return pts;
};

void SavePlayerData(char* Name);
void OneSinPlayer::Save(){
	for(int i=0;i<GetNSBattle();i++){
		SBDScore[i]=DifScore[i];
		SBTScore[i]=TacScore[i];
	}
	SavePlayerData(Name);
};

void LoadPDATA(char* Name);
void OneSinPlayer::Load(char *name){
	strcpy(Name,name);
	
	LoadPDATA(name);
	
	for(int i=0;i<GetNSBattle();i++){
		OneSinBattle* SB=SBattles.Battle+i;
		DifScore[i]=0;
		TacScore[i]=0;
		if(SBDScore[i]>0&&SBDScore[i]<=SB->GetMaxDifBonus()) DifScore[i]=SBDScore[i];
		if(SBTScore[i]>0&&SBTScore[i]<=SB->GetMaxArmyBonus()) TacScore[i]=SBTScore[i];
	}
}

OneSinPlayer SPlayer[64];
int NSPlayers=0;

void GetSPlayerBScore(char* Name, int* Dif, int* Tac){
	for(int i=0;i<NSPlayers&&i<128;i++){
		if(!strcmp(Name,SPlayer[i].Name)){
			Dif=SPlayer[i].DifScore;
			Tac=SPlayer[i].TacScore;
		}
	}	
};

void GetCurSPlayerBScore(int* Dif, int* Tac){
	if(NSPlayers){
		Dif=SPlayer[NSPlayers-1].DifScore;
		Tac=SPlayer[NSPlayers-1].TacScore;
	}
}

///////////////////

int BatDifResult;
int BattleResult;

void SetBattleResult(){
	if(CurrentSinBattle!=-1){
	OneSinBattle* BT=SBattles.Battle+CurrentSinBattle;
		
		byte NI=MyNation;
		int val=0;
		for(int i=0;i<MAXOBJECT;i++){
			OneObject* OB=Group[i];
			if(OB&&(!OB->Sdoxlo||OB->Hidden)&&OB->NNUM==NI){
				val+=BT->ArmyValue[OB->NIndex];
			}
		}

		BattleResult=BT->GetArmyBonus(val);
		BatDifResult=BT->DifBonus[CITY[NI].Difficulty];

	}
};

///////////////////

CIMPORT void CloseBattle();
CIMPORT void CloseMind();

int NSinBattle;
int BattleTopScore(){
	
	SQPicture Back("Interf2\\Background_Battles.bmp");
	DialogsSystem MMenu(0,0);
	MMenu.addPicture(NULL,0,0,&Back,&Back,&Back);

	// Controls
	LocalGP BTN("Interf2\\Buttons");
	LocalGP Pic("Battle\\BatPict");
	LocalGP Win("Battle\\BatWin");

	// Fonts
	RLCFont* FScreen = &fon40y5;
	RLCFont* FWindow = &fon16y5;
	RLCFont* FButton = &fon18y5;
	RLCFont* FHint = &fon13y2;

	// Hint
	MMenu.HintFont=FHint;
	MMenu.HintX=513;
	MMenu.HintY=745;

	// Text
	TextButton* T0=MMenu.addTextButton(NULL,512,18,GetTextByID("INTF_SBATL_T0"),FScreen,FScreen,FScreen,1);
	char* WinTitle=NULL;

	// Buttons
	char* BatQStart=NULL;
	
	// Loading top score

	char Name[32][64];
	int Score[32];
	int NResult=0;
	
	int pts;
	char name[64];

	GFILE* F;
	// loading each players
	F=Gopen("Players.txt","r");
	if(F){
		NSPlayers=0;
		while(Gscanf(F,"%s",name)){			
			
			OneSinPlayer* CP=SPlayer+NSPlayers;

			CP->Load(name);		
			
			NSPlayers++;
			
			strcpy(Name[NResult],name);
			Score[NResult]=CP->GetFullScore();			
			NResult++;
		}
	}

	// loading start score
	F=Gopen("Battle\\MainScript.txt","r");
	if(F){
		for(int n=0;n<7;n++) if(Gscanf(F,"%s%d",name,&pts)==2){
			strcpy(Name[NResult],name);
			Score[NResult]=pts;
			NResult++;
		}
		Gclose(F);
	}
	
	char buf[32];

	srand( (unsigned)time( NULL ) );
	int pic=2+rand()%4;

	int* PDS=SPlayer[0].DifScore;
	int* PTS=SPlayer[0].TacScore;

	if(CurrentSinBattle!=-1){ // Calculating result of cur played battle
		
		CloseBattle();
		CloseMind();

		OneSinBattle* BT=SBattles.Battle+CurrentSinBattle;

		//word vict=NATIONS[MyNation].VictState;
		//NATIONS[MyNation].VictState=0;

		WinTitle=GetTextByID("INTF_BT_Res");
		BatQStart=GetTextByID("INTF_BT_MENU2");
		
		char buf[256];
		sprintf(buf,"%s: %s",GetTextByID("INTF_MAIN_S3"), BT->SM->Name);

		MMenu.addTextButton(NULL,445,261,buf,&fon18w,&fon18w,&fon18w,0);

		int MDB=BT->GetMaxDifBonus();
		int MTB=BT->GetMaxArmyBonus();

		if(BatDifResult<0||BatDifResult>MDB) BatDifResult=0;
		if(BattleResult<0||BattleResult>MTB) BattleResult=0;

		int SumR=BattleResult+BatDifResult;
		if(SumR>PDS[CurrentSinBattle]+PTS[CurrentSinBattle]){
			// save
			PDS[CurrentSinBattle]=BatDifResult;
			PTS[CurrentSinBattle]=BattleResult;
			Score[0]=SPlayer[0].GetFullScore();
			SPlayer[0].Save();
		}else{
			BatDifResult=PDS[CurrentSinBattle];
			BattleResult=PTS[CurrentSinBattle];
		}

		if(SumR){ // victory
			pic=0;
		}else{ // defeat
			pic=1;			
		}

		// show result

		MMenu.addGP_Button(NULL,417,514,Win.GPID,0,0);

		int rx=516;
		int rdx=164;

		int rty=498;

		MMenu.addTextButton(NULL,rx,rty,GetTextByID("INTF_BT_S0"),&fon18w,&fon18w,&fon18w,1);
		MMenu.addTextButton(NULL,rx+rdx,rty,GetTextByID("INTF_BT_S1"),&fon18w,&fon18w,&fon18w,1);
		MMenu.addTextButton(NULL,rx+rdx*2,rty,GetTextByID("INTF_BT_S2"),&fon18w,&fon18w,&fon18w,1);

		int ry=524;

		sprintf(buf,"%d / %d",BatDifResult,MDB);
		MMenu.addTextButton(NULL,rx+rdx*0,ry,buf,&fon16y3,&fon16y3,&fon16y3,1);
		sprintf(buf,"%d / %d",BattleResult,MTB);
		MMenu.addTextButton(NULL,rx+rdx*1,ry,buf,&fon16y3,&fon16y3,&fon16y3,1);
		//itoa(PS[CurrentSinBattle],buf,10);
		sprintf(buf,"%d / %d",PDS[CurrentSinBattle]+PTS[CurrentSinBattle],MDB+MTB);
		MMenu.addTextButton(NULL,rx+rdx*2,ry,buf,&fon16y3,&fon16y3,&fon16y3,1);

		const int lx=458;
		const int ly=521;
		const int lw=120;
		const int ldx=7;

		const float nl=lw/ldx;
		const float n66=nl*66/100;
		const float n33=nl*33/100;

		int n;
		int col;

		if(MDB) n=BatDifResult*nl/MDB; else n=0;
		if(n>n66){ col=1;
			}else if(n>n33){ col=3;
				}else{ col=2; }
		for(int i=0;i<n;i++) MMenu.addGP_Button(NULL,lx+i*ldx,ly,Win.GPID,col,col);

		if(MTB) n=BattleResult*nl/MTB; else n=0;
		if(n>n66){ col=1;
			}else if(n>n33){ col=3;
				}else{ col=2; }
		for(int i=0;i<n;i++) MMenu.addGP_Button(NULL,lx+i*ldx+rdx,ly,Win.GPID,col,col);

		if(MDB+MTB) n=(BatDifResult+BattleResult)*nl/(MDB+MTB); else n=0;
		if(n>n66){ col=1;
			}else if(n>n33){ col=3;
				}else{ col=2; }
		for(int i=0;i<n;i++) MMenu.addGP_Button(NULL,lx+i*ldx+rdx*2,ly,Win.GPID,col,col);

	}else{
		WinTitle=GetTextByID("INTF_TOPSCOR1");
		BatQStart=GetTextByID("INTF_BT_MENU1");
	}
	
	NSinBattle=0;	
	while(NSinBattle<GetNSBattle()&&PDS[NSinBattle]+PTS[NSinBattle]){
		NSinBattle++;
	}
	if(NSinBattle<GetNSBattle()) NSinBattle++;
		else NSinBattle=GetNSBattle();
	
	if(WinTitle) MMenu.addTextButton(NULL,680,217,WinTitle,FWindow,FWindow,FWindow,1);

	if(BatQStart){
		GP_TextButton* OkBtn=MMenu.addGP_TextButton(NULL,472,577,BatQStart,BTN.GPID,0,FButton,FButton);
		OkBtn->UserParam=mcmOk;
		OkBtn->OnUserClick=&MMItemChoose;
		OkBtn->FontDy -= 1;
		OkBtn->Hint=GetTextByID("INTF5_HINT15");
	}

	GP_TextButton* ExitBtn=MMenu.addGP_TextButton(NULL,704,577,GetTextByID("INTF_BT_MENU0"),BTN.GPID,0,FButton,FButton);
	ExitBtn->UserParam=mcmCancel;
	ExitBtn->OnUserClick=&MMItemChoose;
	ExitBtn->FontDy -= 1;
	ExitBtn->Hint=GetTextByID("INTF5_HINT16");

	// Show picture
	MMenu.addGP_Button(NULL,700,292,Pic.GPID,pic,pic);

	// show top table
	int xn=456;
	int xname=477;
	int xpoint=650;

	int y=289;

	int dy=25;

	for(int i=0;i<8;i++){
		word best=0xFFFF;
		if(i==7&&Score[0]>=0){
			best=0;
			sprintf(buf,"...");
		}else{
			for(int n=0;n<NResult;n++){
				if(best==0xFFFF || Score[best]<Score[n]){
					best=n;
				}
			}
			sprintf(buf,"%d.",i+1);
		}

		RLCFont* fnt=&fon18y3;
		if(!best) fnt=&fon18w;
				
		MMenu.addTextButton(NULL,xn,y,buf,fnt,fnt,fnt,1);
		
		MMenu.addTextButton(NULL,xname,y,Name[best],fnt,fnt,fnt,0);
		
		itoa(Score[best],buf,10);
		MMenu.addTextButton(NULL,xpoint,y,buf,fnt,fnt,fnt,1);

		Score[best]=-1;
		y+=dy;

	}	

	ItemChoose=-1;	
	KeyPressed=0;
	
	StartMixing();	
	do{		
	
		ProcessMessages();
		//StdKeys();
		MMenu.MarkToDraw();
		MMenu.ProcessDialogs();
		ProcessMixing();
		MMenu.RefreshView();		

	}while(ItemChoose==-1);
	
	return ItemChoose;
};

extern int NSinBattle;

bool SelectSingleBattle(){
	if(!MISSLIST.NMiss)return false;
	
	int BatRes=BattleTopScore();

	//SlowUnLoadPalette("1\\agew_1.pal");
	AddMissionsPack AMSP;
	//SQPicture Back("Interface\\Single_Mission_Background.bmp");
	SQPicture Back("Interf2\\Single_Mission_Background.bmp");

	LocalGP SCROL("Interf3\\elements\\scroll");
	LocalGP CMB("Interf2\\Combo");
	LocalGP LGP("Interf2\\ListBox");
	LocalGP MapMask("Interf2\\GameMap");
	LocalGP GmBtn("Interf2\\GameButtons");


	LocalGP BTNS("Interface\\Single_mission");
	LocalGP OK1("Interface\\anm_start_1_on");
	LocalGP OK2("Interface\\anm_start_1_off");
	LocalGP CANCEL1("Interface\\anm_cancel_on");
	LocalGP CANCEL2("Interface\\anm_cancel_off");
	LocalGP ENC1("Interface\\anm_enc_on");
	LocalGP ENC2("Interface\\anm_enc_off");	
	LocalGP MASKA("Interface\\preview_mask");
	LoadFog(1);	

	// Fonts
	RLCFont* FScr = &fon40y5;
	RLCFont* FWin = &fon16y5;
	RLCFont* FDialog = &fon18y3;
	RLCFont* FAList = &fon18w;
	RLCFont* FPList = &fon18y3;
	RLCFont* FACombo = &fon18w;
	RLCFont* FPCombo = &fon18y3;
	RLCFont* FMissTitle = &fon18y3;
	RLCFont* FMissText = &fon18y3;
	RLCFont* FHint = &SmallYellowFont;

	DialogsSystem MMenu(0,0);
	MMenu.addPicture(NULL,0,0,&Back,&Back,&Back);
	
	/*
	VideoButton* ENC=MMenu.addVideoButton(NULL,862,468,ENC1.GPID,ENC2.GPID);
	ENC->UserParam=0;
	ENC->OnUserClick=&RunEnciclopia;
	ENC->Hint=GetTextByID("ENCICL");
	ENC->Visible=0;
	*/

	SQPicture PRV("Maps2\\Battle1.bmp");//
	//MMenu.addClipper(588,120,588+198,120+221);
	BPXView* PREV=MMenu.addBPXView(NULL,588,120,400,224/*292,192*/,1,1,1,(byte*)(PRV.GetPixelData())/*Preview+4*/,NULL);
	//MMenu.addClipper(0,0,RealLx,RealLy);
	PREV->Visible=0;
	GPPicture* PMASK=MMenu.addGPPicture(NULL,580,115,MapMask.GPID,0);
	//PMASK->Visible=false;

	LocalGP HFONT("interf3\\Fonts\\FontC10");
	RLCFont hfnt(HFONT.GPID);
	hfnt.SetWhiteColor();

	int HintX = 440;
	int HintY = 745;

	MMenu.HintFont=FHint;
	MMenu.HintX=-HintX;
	MMenu.HintY=-HintY;

	TextButton* HintTxt = MMenu.addTextButton(NULL,HintX,HintY,"                                                                                               ",FHint,FHint,FHint,1);

	VScrollBar* SB=MMenu.addNewGP_VScrollBar(NULL,495,136,476,1,0,SCROL.GPID,0);
	ListBox* LB=MMenu.addGP_ListBox(NULL,94,156,17,LGP.GPID,3,26,FAList,FPList,SB);
	//LB->M_Over=11;
	LB->Active=1;
	LB->FontDy=-3;
	LB->FontDx-=16;
	
	ComboBox* DIFF=MMenu.addGP_ComboBox(NULL,213,638,CMB.GPID,0,9,0,FACombo,FPCombo,NULL);
	DIFF->CurLine=1;
	FILE* f=fopen("smiss.opt","r");
	if(f){
		int v=0;
		fscanf(f,"%d",&v);
		fclose(f);
		DIFF->CurLine=v;
	};

	DIFF->Center-=5;
	DIFF->FontDy-=2;
	DIFF->OneDy-=1;

	DIFF->AddLine(GetTextByID("@RMID_EASY"));
	DIFF->AddLine(GetTextByID("@RMID_NORMAL"));
	DIFF->AddLine(GetTextByID("@RMID_HARD"));
	DIFF->AddLine(GetTextByID("@RMID_VERYHARD"));
	DIFF->AddLine(GetTextByID("@RMID_IMPOSIBLE"));
	DIFF->Hint=GetTextByID("DIFFICULT5");

	int TextX = 605;
	int TextY = 445;
	int TextLx = 342;
	int TextLy = 226;

	TextViewer* TV[128];
	VScrollBar* TSB[128];

	char Buf[256];
	for(int j=0;j<NSinBattle/*GetNSBattle()*/;j++){
		TV[j]=NULL;
		TSB[j]=NULL;
		SingleMission* SM=SBattles.Battle[j].SM;//MISSLIST.MISS+i;
		if(SM){
			LB->AddItem(SM->Name,0);
			TV[j]=MMenu.addTextViewer(NULL,TextX,TextY,TextLx,TextLy,SM->Intro[0],FMissText);
			TV[j]->SymSize=21;
			TV[j]->PageSize=(TV[j]->y1-TV[j]->y)/TV[j]->SymSize;
			TV[j]->Visible=0;
			TSB[j]=MMenu.addNewGP_VScrollBar(NULL,957,434,238,1,0,SCROL.GPID,0);
			TSB[j]->Visible=0;
			TSB[j]->ScrDy=7;
			TV[j]->AssignScroll(TSB[j]);
						
			sprintf(Buf,"%d",SPlayer->DifScore[j]+SPlayer->TacScore[j]);
			MMenu.addTextButton(NULL,425,157+j*26,Buf,FMissText,FMissText,FMissText,1);
			
			OneSinBattle* SB=SBattles.Battle+j;
			sprintf(Buf,"(%d)", SB->GetMaxDifBonus()+SB->GetMaxArmyBonus());
			MMenu.addTextButton(NULL,425+35,157+j*26,Buf,FMissText,FMissText,FMissText,1);
			
		}
	}

	KeyPressed=0;
	LastKey=0;
	
	/*
	int GetNSBattle()=MISSLIST.MSMiss;
	for(i=0;i<AMSP.NMiss;i++){
		LB->AddItem(AMSP.Pack[i].Title,0);
		TV[GetNSBattle()+i]=MMenu.addTextViewer(NULL,TextX,TextY,TextLx,TextLy,AMSP.Pack[i].Preview,FMissText);
		TV[GetNSBattle()+i]->SymSize=21;
		TV[GetNSBattle()+i]->PageSize=(TV[i]->y1-TV[i]->y)/TV[i]->SymSize;
		TV[GetNSBattle()+i]->Visible=0;
		TSB[GetNSBattle()+i]=MMenu.addNewGP_VScrollBar(NULL,957,434,238,1,0,SCROL.GPID,0);
		TSB[GetNSBattle()+i]->Visible=0;
		TSB[GetNSBattle()+i]->ScrDy=7;
		TV[GetNSBattle()+i]->AssignScroll(TSB[i]);
	};
	*/

	// Buttons
	int BInChatX = 839;
	int OkX = 899;
	int CancelX = 959;
	int ButY = 706;

	//VideoButton* OkBtn=MMenu.addVideoButton(NULL,869,718,OK1.GPID,OK2.GPID);
	//VideoButton* CancelBtn=MMenu.addVideoButton(NULL,862,668,CANCEL1.GPID,CANCEL2.GPID);
	GP_Button* OkBtn=MMenu.addGP_Button(NULL,OkX,ButY,GmBtn.GPID,0,1);
	GP_Button* CancelBtn=MMenu.addGP_Button(NULL,CancelX,ButY,GmBtn.GPID,3,4);

	OkBtn->OnUserClick=&MMItemChoose;
	OkBtn->UserParam=mcmOk;
	OkBtn->Hint=GetTextByID("MISS_RUN");
		
	CancelBtn->OnUserClick=&MMItemChoose;
	CancelBtn->UserParam=mcmCancel;
	CancelBtn->Hint=GetTextByID("SOCANCEL");//GetTextByID("MISS_CANCEL");

	// Text
	MMenu.addTextButton(NULL,512,18,GetTextByID("INTF_SBATL_T0"),FScr,FScr,FScr,1);

	MMenu.addTextButton(NULL,303,107,GetTextByID("INTF_SBATL_T1"),FWin,FWin,FWin,1);
	MMenu.addTextButton(NULL,787,106,GetTextByID("INTF_SMISS_T2"),FWin,FWin,FWin,1);
	MMenu.addTextButton(NULL,787,401,GetTextByID("INTF_SMISS_T3"),FWin,FWin,FWin,1);
	
	MMenu.addTextButton(NULL,98,640,GetTextByID("INTF_SMISS_DIFF"),FDialog,FDialog,FDialog,0);

	int prmiss=-1;

	if(CurrentSinBattle<GetNSBattle() && CurrentSinBattle!=-1) 
		LB->CurItem=CurrentSinBattle;
HHB1:
	
	if(BatRes==mcmOk){
		ItemChoose=mcmOk;
		if(CurrentSinBattle==-1){
			LB->CurItem=rand()%NSinBattle/*GetNSBattle()*/;
		}
	}else{
		ItemChoose=-1;
	}

	CurrentSinBattle=-1;
	
	int mm=1;
	KeyPressed=0;
	
	StartMixing();
	while(ItemChoose==-1){		
		if(MMenu.Hint){
			strcpy(HintTxt->Message, MMenu.Hint);
			HintTxt->Visible=1;
		}else
			HintTxt->Message[0] = 0;
		
		if(prmiss!=LB->CurItem){
			//if(GetPreview(MISSLIST.MISS[MISSLIST.SingleMS[LB->CurItem]].MapName,Preview+4)){
			//	PREV->Visible=1;
			//	Decode(Preview+4,XLAT,292*190);
			//}else{
			//	PREV->Visible=false;
			//};
			if(LB->CurItem>=GetNSBattle()){
				/*
				char ccc[200];
				strcpy(ccc,AMSP.Pack[LB->CurItem-GetNSBattle()].Bitmap);
				ResFile ff1=RReset(ccc);
				if(ff1!=INVALID_HANDLE_VALUE){
					PRV.LoadPicture(ccc);
					PREV->Ptr=(byte*)(PRV.getPixelData());
				}else{
					//PREV->Ptr=NULL;
					PREV->Visible=0;
				}
				prmiss=LB->CurItem;
				*/
			}else{
				char ccc[200];
				sprintf(ccc,"Maps2\\Battle%d.bmp",LB->CurItem+1);
				ResFile ff1=RReset(ccc);
				if(ff1!=INVALID_HANDLE_VALUE){
					PRV.LoadPicture(ccc);
					PREV->Ptr=(byte*)(PRV.GetPixelData());
					PREV->Visible=1;
				}else{
					//PREV->Ptr=NULL;
					PREV->Visible=0;
				}
				prmiss=LB->CurItem;
			};
		};
		ProcessMessages();
		StdKeys();
		MMenu.MarkToDraw();
		MMenu.ProcessDialogs();
		ProcessMixing();
		MMenu.RefreshView();		
		
		for(int j=0;j<NSinBattle/*GetNSBattle()*/;j++){
			if(TV[j]){
				if(j==LB->CurItem){
					TV[j]->Visible=1;
					TSB[j]->Visible=1;
					TV[j]->Active=0;					
					/*
					if(TOPNAME[j][0]) for(int n=0;n<7;n++){
						TOPNAME[j][n]->Visible=1;
						TOPPTS[j][n]->Visible=1;
					}*/
				}else{
					TV[j]->Visible=0;
					TSB[j]->Visible=0;
					TV[j]->Active=0;
					/*
					if(TOPNAME[j][0]) for(int n=0;n<7;n++){
						TOPNAME[j][n]->Visible=0;
						TOPPTS[j][n]->Visible=0;
					}*/
				};
			};
		};
		LB->Active=1;
		if(mm){
//			SlowLoadPalette("1\\agew_1.pal");
			mm=0;
		};
		StdKeys();
	}
	f=fopen("smiss.opt","w");
	if(f){
		fprintf(f,"%d",DIFF->CurLine);
		fclose(f);
	};
	if(ItemChoose==mcmOk){
		CurrentCampagin=-1;
		CurrentMission=-1;
		//*if(LB->CurItem<GetNSBattle()){
			CurrentSinBattle=LB->CurItem;
			//SaveState=6;
			if(!ProcessSingleMission(SBattles.Battle[LB->CurItem].GID,DIFF->CurLine)){
//				SlowUnLoadPalette("1\\agew_1.pal");
				CurrentCampagin=-1;
				CurrentMission=-1;
				goto HHB1;
			}else{
				SaveState=6;
				DisableSave=1;
				//CurrentSinBattle=LB->CurItem;
				ItemChoose=mcmSingle;
				BattleResult=0;
				BatDifResult=0;

				//void SetGSpeed(byte Speed);
				//SetGSpeed(128);
				return true;
			}
		/*}else{
			void ClearCompstart();
			ClearCompstart();
			ShowLoading();
			ItemChoose=mcmSingle;
			strcpy(GSets.CGame.cgi_CurrentMap,AMSP.Pack[LB->CurItem-GetNSBattle()].Map);
			CreateNationalMaskForMap(GSets.CGame.cgi_CurrentMap);
			PrepareGameMedia(0,0);
			InitGame();
			for(int i=0;i<7;i++){
				CITY[i].Difficulty=DIFF->CurLine;
				GSets.CGame.PL_INFO[i].ColorID=i;
				if(i)strcpy(GSets.CGame.PL_INFO[i].name,"???");
			};
			SetMyNation(0);
			strcpy(GSets.CGame.PL_INFO[0].name,PlName);
			GSets.CGame.PL_INFO[0].GameTime=0;
			GSets.CGame.PL_INFO[0].VictCond=0;
			extern int StartGTime;
			extern int StartVCond;
			StartGTime=0;
			StartVCond=0;		
			
		};*/
//		SlowUnLoadPalette("1\\agew_1.pal");
	}else{
		CurrentCampagin=-1;
		CurrentMission=-1;
		CurrentSinBattle=-1;
//		SlowUnLoadPalette("1\\agew_1.pal");		
	};
	
	return false;
};