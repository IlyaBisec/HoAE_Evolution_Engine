#define PREVIEWLX 400
#define PREVIEWLY 224
char ROOMOPT[512]="";
CEXPORT void SetRoomOptions(char* s){
	strcpy(ROOMOPT,s);
};
CEXPORT
void AddToGChat(char* Nick,char* Message){
	TPEN.GlobalChat.Add(Nick,Message);
};
CEXPORT
void ClearGChat(){
	TPEN.GlobalChat.Clear();
};
CIMPORT
void SendPrivateMessage(char* Nick,char* MESSAGE);
bool CheckForPersonalChat(char* STR){
	if(STR[0]=='-'&&STR[1]=='-'&&STR[2]=='>'){
		char* SS=strchr(STR,':');
		if(SS){
			int L=SS-STR-3;
			if(L>0){
				char SS3[256];
				memcpy(SS3,STR+3,L);
				SS3[L]=0;
				if(UseGSC_Login){
					SendPrivateMessage(SS3,SS+1);
				}else
				if(TPEN.Peer&&TPEN.Connected){
					peerMessagePlayer(TPEN.Peer,SS3,SS+1);
					char ccc[128];
					sprintf(ccc,"%s-->%s",TPEN.MyNick,SS3);
					TPEN.GlobalChat.Add(ccc,SS+1);
					return true;
				};
			};
		};
	};
	return false;
};
extern char LASTCLICKCHATNAME[128];
bool CheckPersonality(char* MESSAGE){
	if(LASTCLICKCHATNAME[0]){
		char CC4[512];
		CC4[0]=0;
		if(MESSAGE[0]=='-'&&MESSAGE[1]=='-'&&MESSAGE[2]=='>'){
			char* SS=strchr(MESSAGE,':');
			if(SS){
				strcpy(CC4,SS+1);
			}else strcpy(CC4,MESSAGE);
		}else{
			strcpy(CC4,MESSAGE);
		};
		sprintf(MESSAGE,"-->%s:%s",LASTCLICKCHATNAME,CC4);
		LASTCLICKCHATNAME[0]=0;
		return true;
	};
	return false;
};
CIMPORT
void ChatProcess();
bool MPL_WaitingGame(bool Host,bool SINGLE){

	int DefaultTeam=-1;
	int DefaultColor=-1;
	int DefaultNation=-1;
	int DefaultLand=-1;
	int DefaultCZone=-1;
	int DefaultVCond=-1;
	int DefaultGTime=-1;
	int DefaultPTime=-1;
	int DefaultMapSize=-1;
	int DefaultPage=-1;
	int DefaultRes=-1;
	int DefaultMines=-1;
	int DefaultRelief=-1;
	int DefaultCannons=-1;
	int DefaultArmy=-1;
	int DefaultAutosave=-1;
	int DefaultVision=-1;
	bool NOAI=0;
	bool NOEQCOLOR=0;
	if(ROOMOPT[0]){
		//options
		char* s;
		s=strstr(ROOMOPT,"*TM");
		if(s)sscanf(s+3,"%d",&DefaultTeam);
		s=strstr(ROOMOPT,"*CL");
		if(s)sscanf(s+3,"%d",&DefaultColor);
		s=strstr(ROOMOPT,"*NT");
		if(s)sscanf(s+3,"%d",&DefaultNation);
		s=strstr(ROOMOPT,"*LT");
		if(s)sscanf(s+3,"%d",&DefaultLand);
		s=strstr(ROOMOPT,"*CZ");
		if(s)sscanf(s+3,"%d",&DefaultCZone);
		s=strstr(ROOMOPT,"*VC");
		if(s)sscanf(s+3,"%d",&DefaultVCond);
		s=strstr(ROOMOPT,"*GT");
		if(s)sscanf(s+3,"%d",&DefaultGTime);
		s=strstr(ROOMOPT,"*PT");
		if(s)sscanf(s+3,"%d",&DefaultPTime);
		s=strstr(ROOMOPT,"*MS");
		if(s)sscanf(s+3,"%d",&DefaultMapSize);
		s=strstr(ROOMOPT,"*PG");
		if(s)sscanf(s+3,"%d",&DefaultPage);
		s=strstr(ROOMOPT,"*RS");
		if(s)sscanf(s+3,"%d",&DefaultRes);
		s=strstr(ROOMOPT,"*MN");
		if(s)sscanf(s+3,"%d",&DefaultMines);
		s=strstr(ROOMOPT,"*RL");
		if(s)sscanf(s+3,"%d",&DefaultRelief);
		s=strstr(ROOMOPT,"*CN");
		if(s)sscanf(s+3,"%d",&DefaultCannons);
		s=strstr(ROOMOPT,"*AR");
		if(s)sscanf(s+3,"%d",&DefaultArmy);
		s=strstr(ROOMOPT,"*SV");
		if(s)sscanf(s+3,"%d",&DefaultAutosave);
		s=strstr(ROOMOPT,"*VS");
		if(s)sscanf(s+3,"%d",&DefaultVision);
		NOAI=strstr(ROOMOPT,"*NOAI")!=NULL;
		NOEQCOLOR=strstr(ROOMOPT,"*DIFFC")!=NULL;

	};
	ROOMOPT[0]=0;

	ClearLPACK();
	ServerDPID=0;
	int r0=GetTickCount();
	PPTIME=r0;
	memset(CurrentMaxPing,0xFF,4*8);
	ReceiveAll();
	CurStatus=0;
	PitchTicks=8;
	MaxPingTime=0;
	ClearCTime();
	int NCHATS=0;
	CHATSTRING[0]=0;
	int MAXCHATS=0;
	char** ChatMess=NULL;
	char** ChatSender=NULL;
	PlayerInfo MYPINF;
	memset(&MYPINF,0,sizeof MYPINF);

	byte* Preview=znew(byte,(PREVIEWLX*PREVIEWLY)+4);
	char Currand[200];
	((word*)Preview)[0]=PREVIEWLX;
	((word*)Preview)[1]=PREVIEWLY;
	bool PrPresent=false;
	char LastPrName[200]="";
	byte XLAT[256];
	ResFile fx=RReset("1\\0to2.set");
	if(fx!=INVALID_HANDLE_VALUE){
		RBlockRead(fx,XLAT,256);
		RClose(fx);
	};
	LoadFog(1);

	// Fonts
	RLCFont* FScreen = &fon40y5;
	RLCFont* FWindow = &fon16y5;
	RLCFont* FWhite = &fon18w;
	RLCFont* FYellow = &fon18y3;
	RLCFont* FHint = &fon13y2;
	RLCFont* FMess = &fon16y3;//fon16y1;
	RLCFont* FASwitch = &fon16y1;
	RLCFont* FPSwitch = &fon16y3;
	RLCFont* FASmCmb = &fon16y1;
	RLCFont* FPSmCmb = &fon16y3;

	// Graphics
	LocalGP CMB("Interf2\\Combo");
	LocalGP VSC("Interf3\\elements\\scroll");
	LocalGP PL("Interf2\\PlayerList");
	LocalGP MASKA("Interf2\\GameMap");
	LocalGP BTN("Interf2\\Buttons");
	LocalGP CGR("Interf2\\CreateGameRoom");
	LocalGP GmBtn("Interf2\\GameButtons");
	
	int Set0[30] = {2,2,2,2,2, 3,3,3,3,3, 3,3,3,3,3,
				    2,2,2,2,2, 3,3,3,3,3, 3,3,3,3,3};
	
	LocalGP BTNS("Interface\\Multi_opt");
	LocalGP OK1("Interface\\anm_start_1_on");
	LocalGP OK2("Interface\\anm_start_1_off");
	LocalGP CANCEL1("Interface\\anm_cancel_on");
	LocalGP CANCEL2("Interface\\anm_cancel_off");
	LocalGP DIS("Interface\\an_start_disable");
	LocalGP SCROL("Interface\\_Slider");		
	LocalGP INCHAT("Interface\\IButtons");
	LocalGP CCOM("Interface\\CompCombo");
	LocalGP SINGL("Interface\\Random_map");
	LocalGP LONGBOX("Interf2\\Combo");	
	LocalGP ISYM3("Interface\\isymb3");

	SQPicture Back;
	char* MainTitle=NULL;
	if(SINGLE){
		//Back.LoadPicture("Interface\\Random_Map_Background.bmp");
		Back.LoadPicture("Interf2\\Random_Map_Background.bmp");
		MainTitle=GetTextByID("INTF_CROOM_SCR1");
	}else{
		//Back.LoadPicture("Interface\\Multiplayer_Create_Background.bmp");
		Back.LoadPicture("Interf2\\Multiplayer_Create_Background.bmp");
		MainTitle=GetTextByID("INTF_CROOM_SCR0");
	};

	LocalGP FONT("interf3\\Fonts\\FontC10");
	RLCFont FontY(FONT.GPID);
	FontY.SetColorTable(3);
	LoadOptionalTable(3,"1\\orange.grd");
	RLCFont FontW(FONT.GPID);
	FontW.SetColorTable(4);
	LoadOptionalTable(4,"1\\white.grd");

	LocalGP HFONT("interf3\\Fonts\\FontC10");
	RLCFont hfnt(HFONT.GPID);
	hfnt.SetWhiteColor();
	
	DialogsSystem MENU(0,0);
	
	MENU.OkSound=GetSound("START");

	MENU.HintFont=FHint;
	MENU.HintX=440;
	MENU.HintY=745;

	Picture* GPPB=MENU.addPicture(NULL,0,0,&Back,&Back,&Back);
	
	TextButton* SHOWREC=MENU.addTextButton(NULL,1010,4,"0018",FYellow,FYellow,FYellow,0);
	
	//GPPicture*  NameBack[8];
	//GPPicture*  NationBack[8];
	GPPicture*  ColorBack[8];
	GPPicture*  Flags[8];
	GPPicture*  CD_Back[8];
	GPPicture*  CD_Present[8];
	//GPPicture*  ReadyBack[8];
	GPPicture*  AliasBack[8];
	ComboBox*   COMPS[8];
	ComboBox*   KICK[8];
	//GPPicture*  BCOMP[8];

	ComboBox*   ADD_OPT;
	ComboBox*   ADD_OPT_VAL;
	bool COMPPREV[8];
	memset(COMPPREV,0,sizeof COMPPREV);

	TextButton* AliasID[8];
	InputBox*   MNAME  [8];
	ComboBox*   MNATION[8];
	TextButton* ReadyFlow[8];
	//char        VersID[8][8];
	//TextButton* VersTextShadow[8];
	TextButton* VersText[8];
	//ColoredBar* MCOLOR [8];
	GPPicture*  MREADY[8];
	byte COMCOLOR[8];
	byte COMNATION[8];
	byte COMALLY[8];
	int NComp=0;
	memset(COMCOLOR,0,sizeof COMCOLOR);
	memset(COMNATION,0,sizeof COMNATION);
	memset(COMALLY,0,sizeof COMALLY);
	int HostID=-1;
	char* ADDCOM="Add a Computer";
	//MENU.addInputBox(NULL,0,0,GSets.CGame.cgi_CurrentMap,64,400,25,&FontW,&FontW);
	
	// Chat with other players	
	int NCL=9; // Chat scroll after NCL line
	ChatViewer* CHVIEW=NULL;
	if(!SINGLE)CHVIEW=MENU.addChatViewer(NULL,92,467,NCL,18,408,&ChatMess,&ChatSender,&NCHATS);
	//CHVIEW->
	char CHATMESSAGE[256]="";
	InputBox* CHATBOX=NULL;
	if(!SINGLE){
		CHATBOX=MENU.addInputBox(NULL,173,639,CHATMESSAGE,250,320,22,FYellow,FWhite);
		CHATBOX->Active=1;
	};
	VScrollBar* CHSCR=MENU.addNewGP_VScrollBar(NULL,491,465,167,1,0,VSC.GPID,0);
	CHSCR->Visible=0;
	
	// Previous played map
	BPXView* PREV=MENU.addBPXView(NULL,588,458,PREVIEWLX,PREVIEWLY,1,1,1,Preview+4,NULL);	
	PREV->Visible=false;
	
	GPPicture* PMASK=MENU.addGPPicture(NULL,580,450,MASKA.GPID,0);
	//PMASK->Visible=false;
	//Local

	// Text
	MENU.addTextButton(NULL,512,18,MainTitle,FScreen,FScreen,FScreen,1);
	
	MENU.addTextButton(NULL,301,107,GetTextByID("INTF_CROOM_WIN0"),FWindow,FWindow,FWindow,1);
	MENU.addTextButton(NULL,786,107,GetTextByID("INTF_CROOM_WIN1"),FWindow,FWindow,FWindow,1);
	MENU.addTextButton(NULL,786,441,GetTextByID("INTF_CROOM_WIN2"),FWindow,FWindow,FWindow,1);
	
	MENU.addTextButton(NULL,98,145,GetTextByID("INTF_CROOM_PL0"),FMess,FMess,FMess,0);
	MENU.addTextButton(NULL,228,145,GetTextByID("INTF_CROOM_PL1"),FMess,FMess,FMess,0);
	if(SINGLE){
		MENU.addTextButton(NULL,388,145,GetTextByID("INTF_CROOM_PL2"),FMess,FMess,FMess,1);
		MENU.addTextButton(NULL,470,145,GetTextByID("INTF_CROOM_PL3"),FMess,FMess,FMess,1);
	}else{
		MENU.addTextButton(NULL,352,145,GetTextByID("INTF_CROOM_PL2"),FMess,FMess,FMess,0);
		MENU.addTextButton(NULL,393,145,GetTextByID("INTF_CROOM_PL3"),FMess,FMess,FMess,0);
		MENU.addTextButton(NULL,441,145,GetTextByID("INTF_CROOM_PL4"),FMess,FMess,FMess,0);
		MENU.addTextButton(NULL,467,145,GetTextByID("INTF_CROOM_PL5"),FMess,FMess,FMess,0);
	}
	
	TextButton* Mess=MENU.addTextButton(NULL,92,644,GetTextByID("INTF_CROOM_CHAT0"),FMess,FMess,FMess,0);
	if(SINGLE) Mess->Visible=0;

	char* rtxt=GetTextByID("???");
	int RNATID=GlobalAI.NAi;
	///////////////////////////////////////////////////////////	
	int x1 = 0;
	int y1 = 162;
	int wC1 = 126;
	
	int NatX = 223;

	int CompX = 93;
	int CompW = 128;
	
	int NameX = CompX;
	int NameDy = -2;
	
	int FlagX;	 // Flags
	int FlagDy = 1;

	int TeamX;
	int TeamDy = 2;

	if(SINGLE){
		FlagX = 371;
		TeamX = 470;
	}else{
		FlagX = 355;
		TeamX = 413;
	}
	

	///////////////////////////////////////////////////////////
	for(int i=0;i<8;i++){
		COMPS[i]=MENU.addGP_ComboBoxDLX(NULL,CompX,y1+i*26,CompW,CMB.GPID,0,8,0,FWhite,FYellow,NULL);
		
		COMPS[i]->Center-=4;
		COMPS[i]->FontDy-=2;
		COMPS[i]->OneDy-=1;
		//BCOMP[i]=MENU.addGPPicture(NULL,38,161+i*26,SINGL.GPID,90+(i%3));
		
		//AliasBack[i]=MENU.addGPPicture(NULL,397,165+i*26,PL.GPID,10);		
		AliasBack[i]=MENU.addGPPicture(NULL,TeamX-15,y1+3+TeamDy+i*26,PL.GPID,10);		
		AliasID[i]=MENU.addTextButton(NULL,TeamX,y1+TeamDy+i*26,"X",FYellow,FYellow,FYellow,1);
		
		//ColorBack[i]=MENU.addGPPicture(NULL,409,161+i*26,SINGL.GPID,93+(i%3));
		Flags[i]=MENU.addGPPicture(NULL,FlagX,y1+FlagDy+i*26,PL.GPID,i);

		MNATION[i]=MENU.addGP_ComboBoxDLX(NULL,NatX,y1+i*26,wC1,CMB.GPID,0,8,0,FWhite,FYellow,NULL);

		MNATION[i]->Center-=4;
		MNATION[i]->FontDy-=2;
		MNATION[i]->OneDy-=1;

		//AliasBack[i]=(GPPicture*)AliasID[i];
		ColorBack[i]=Flags[i];
		AliasBack[i]->OnMouseOver=&ChangeAlias;
		
		if(SINGLE){

			KICK[i]=NULL;			
			
			//AliasID[i]->OnMouseOver=&ChangeAlias;			
			AliasID[i]->Visible=0;

			AliasBack[i]->Nation=0;
			
			ColorBack[i]->Visible=0;
			ColorBack[i]->OnMouseOver=&ColorMouseOver;
			//ColorBack[i]->Hint=GetTextByID("MMCHANGECOLOR");
			//Flags[i]->Visible=0;
			//ColorBack[i]->Child=Flags[i];			
		}else{
			if(Host){
				KICK[i]=MENU.addGP_ComboBoxDLX(NULL,NameX,y1+i*26,CompW,CMB.GPID,0,8,0,FWhite,FYellow,NULL);
				
				KICK[i]->Center-=4;
				KICK[i]->FontDy-=2;
				KICK[i]->OneDy-=1;
				
				KICK[i]->AddLine("                                                   ");
				KICK[i]->AddLine(GetTextByID("KICK"));
				KICK[i]->Visible=0;
			}else{
				KICK[i]=NULL;
			}
			//COMPS[i]=MENU.addGP_ComboBoxDLX(NULL,x1,y1+i*26,wC1,CMB.GPID,0,8,0,FWhite,FYellow,NULL);			
			//BCOMP[i]=MENU.addGPPicture(NULL,38,161+i*26,CCOM.GPID,24+(i%3));
			//NameBack[i]=MENU.addGPPicture(NULL,38,161+i*26,BTNS.GPID,9+(i%3));
			//NationBack[i]=MENU.addGPPicture(NULL,173,161+i*26,BTNS.GPID,i%3);
			//AliasBack[i]=MENU.addGPPicture(NULL,332,161+i*26,BTNS.GPID,96+(i%3));
			//AliasID[i]=MENU.addTextButton(NULL,332+18,165+i*26,"X",&SmallYellowFont,&SmallYellowFont,&SmallYellowFont,1);
			//ColorBack[i]=MENU.addGPPicture(NULL,285,161+i*26,BTNS.GPID,6+(i%3));
			//Flags[i]=MENU.addGPPicture(NULL,295-5+19-14,161-11+12+i*26,NewFlag.GPID,(i*17)%45);
			//MNATION[i]=MENU.addGP_ComboBoxDLX(NULL,x1,y1+i*26,wC1,CMB.GPID,0,8,0,FWhite,FYellow,NULL);
			
			//NameBack[i]->Visible=0;

			//NationBack[i]->Visible=0;
			
			int CDX = 440;
			int CDdY = 1;
			
			int VerDx = 10;
			int VerDy = 4;
			
			int ReadyX = 482;
			int ReadyDy = 5;

			int LoadInfDx = 8;
			int LoadInfDy = 2;

			//CD_Back[i]=MENU.addGPPicture(NULL,CDX,y1+CDdY+i*26,PL.GPID,7);
			CD_Present[i]=MENU.addGPPicture(NULL,CDX,y1+CDdY+i*26,PL.GPID,7);
			//VersTextShadow[i]=MENU.addTextButton(NULL,376+12,161+6+i*26,"        ",&SmallBlackFont,&SmallBlackFont,&SmallBlackFont,1);
			VersText[i]=MENU.addTextButton(NULL,CDX+VerDx,y1+VerDy+i*26,"                                                            ",FYellow,FYellow,FYellow,1);
			//ReadyBack[i]=MENU.addGPPicture(NULL,408,161+i*26,BTNS.GPID,15+(i%3));
			
			ReadyFlow[i]=MENU.addTextButton(NULL,ReadyX+LoadInfDx,y1+LoadInfDy+i*26,"100%",FYellow,FYellow,FYellow,1);
			MREADY[i]=MENU.addGPPicture(NULL,ReadyX,y1+ReadyDy+i*26,PL.GPID,9);

			//CD_Back[i]->Visible=0;
			
			CD_Present[i]->Visible=0;
			
			//VersTextShadow[i]->Visible=0;
			
			VersText[i]->Visible=0;
			
			//ReadyBack[i]->Visible=0;
			
			ReadyFlow[i]->Visible=0;
			ReadyFlow[i]->Enabled=0;
			
			MREADY[i]->Visible=0;
			
			AliasBack[i]->Visible=0;
			AliasBack[i]->Nation=0;
			
			AliasID[i]->Visible=0;
			//AliasID[i]->OnMouseOver=&ChangeAlias;
			
			ColorBack[i]->Visible=0;
			ColorBack[i]->OnMouseOver=&ColorMouseOver;
			//ColorBack[i]->Hint=GetTextByID("MMCHANGECOLOR");
			Flags[i]->Visible=0;
			//ColorBack[i]->Child=Flags[i];
		};
		//BCOMP[i]->Visible=0;
		COMPS[i]->AddLine(GetTextByID("None"));
		if(!NOAI){
			COMPS[i]->AddLine(GetTextByID("@RMID_EASY"));
			COMPS[i]->AddLine(GetTextByID("@RMID_NORMAL"));
			COMPS[i]->AddLine(GetTextByID("@RMID_HARD"));
			COMPS[i]->AddLine(GetTextByID("@RMID_VERYHARD"));
		};

		COMPS[i]->Visible=0;
		COMPS[i]->Enabled=0;
		
		MNAME[i]=MENU.addInputBox(NULL,NameX,y1+NameDy+i*26,GSets.CGame.PL_INFO[i].name,20,120,24,FWhite,FYellow);
		
		MNAME[i]->Visible=0;		
		//MNAME[i]->Hint=GetTextByID("MOCHANGENAME");
		MNAME[i]->Enabled=false;
		//MNATION[i]->Hint=GetTextByID("MMCHANGENAT");
		
#ifdef TUTORIAL_DEMOVERSION
		int NG=1;
#else
		int NG=GlobalAI.NAi;
#endif
		for(int j=0;j<NG;j++){
			MNATION[i]->AddLine(GlobalAI.Ai[j].Message);
		};
#ifndef TUTORIAL_DEMOVERSION
		MNATION[i]->AddLine(rtxt);
#endif
		MNATION[i]->FontDx-=2;

		//MNATION[i]->OneDx-=2;
		//MCOLOR[i]=MENU.addColoredBar(32+318,160+i*25,32,20,0xF7);
		//MCOLOR[i]->Hint=GetTextByID("MMCHANGECOLOR");
		//MCOLOR[i]->OnMouseOver=&ColorMouseOver;
	};

	
	///////////////////////////////////////////////////////////////////////
	int cc1=0;
	byte mss1=1;
	byte MSS1=0;
	bool GMREADY=true;
	
	//-----------------------------------
	// List of saved games and designed maps & scrollers
	int MapListX = 605;
	int MapListY = 186;

	int MabScrX = 955;
	int MabScrY = 175;

	//--------------- Page 1 --------------

	// --- Game settings -----------------------------------------------------------------
	int y=161;
	int y0=y;
	int x=739;//506;
	int x0=x+128+20;
	RandomMapDesc RMP;
	RMP.Load("Terrain\\Res.dat");
		
	ComboBox* CMGRP1[20];
	TextButton* TGRP1[20];
	for(int i=0; i<20; i++){
		CMGRP1[i]=NULL;
		TGRP1[i]=NULL;
	}
	int NGRP1=13-2;

	int x2=739;
	int y2=MapListY;
	int w2=208;

	int Tx2=606;
	
	int FAO=6; // First number of additional options

	SimpleDialog* ListClip=MENU.addClipper(601,185,952,340);
	//SimpleDialog* ListClip=MENU.addClipper(0,0,RealLx,RealLy);
	
	TGRP1[0] = MENU.addTextButton(NULL,Tx2,0,GetTextByID("INTF_CROOM_O0"),FYellow,FYellow,FYellow,0);
	TGRP1[1] = MENU.addTextButton(NULL,Tx2,0,GetTextByID("INTF_CROOM_O1"),FYellow,FYellow,FYellow,0);
	TGRP1[3] = MENU.addTextButton(NULL,Tx2,0,GetTextByID("INTF_CROOM_O2"),FYellow,FYellow,FYellow,0);
	TGRP1[4] = MENU.addTextButton(NULL,Tx2,0,GetTextByID("INTF_CROOM_O3"),FYellow,FYellow,FYellow,0);
	TGRP1[5] = MENU.addTextButton(NULL,Tx2,0,GetTextByID("INTF_CROOM_O4"),FYellow,FYellow,FYellow,0);

	CMGRP1[0]=MENU.addGP_ComboBoxDLX(NULL,x2,0,w2,CMB.GPID,0,9,0,FASmCmb,FPSmCmb,NULL);
	CMGRP1[1]=MENU.addGP_ComboBoxDLX(NULL,x2,0,w2,CMB.GPID,0,9,0,FASmCmb,FPSmCmb,NULL);
	CMGRP1[3]=MENU.addGP_ComboBoxDLX(NULL,x2,0,w2,CMB.GPID,0,9,0,FASmCmb,FPSmCmb,NULL);
	CMGRP1[4]=MENU.addGP_ComboBoxDLX(NULL,x2,0,w2,CMB.GPID,0,9,0,FASmCmb,FPSmCmb,NULL);
	CMGRP1[5]=MENU.addGP_ComboBoxDLX(NULL,x2,0,w2,CMB.GPID,0,9,0,FASmCmb,FPSmCmb,NULL);

	CMGRP1[0]->CurLine=RMP.STY_DEF;
	int RSTYID=RMP.NSTY;
#ifdef LIMITOPTIONS
	for(int p=0;p<RMP.NSTY&&p<2;p++){
		CMGRP1[0]->AddLine(RMP.STY[p].Name);
	};
	if(CMGRP1[0]->CurLine>=CMGRP1[0]->NLines)CMGRP1[0]->CurLine=0;
#else
	for(int p=0;p<RMP.NSTY;p++){
		CMGRP1[0]->AddLine(RMP.STY[p].Name);
	};
	CMGRP1[0]->AddLine(rtxt);
#endif
	//CMGRP1[0]->Hint=GetTextByID("SOMAPSTYLE");
	
	y+=25;
	int RRELID=RMP.NRelief;

	CMGRP1[1]->CurLine=RMP.Relief_DEF;
#ifdef LIMITOPTIONS
	for(p=0;p<RMP.NRelief&&p<2;p++){
		CMGRP1[1]->AddLine(RMP.Relief[p]);
	};
	if(CMGRP1[1]->CurLine>=CMGRP1[1]->NLines)CMGRP1[1]->CurLine=0;
#else
	for(int p=0;p<RMP.NRelief;p++){
		CMGRP1[1]->AddLine(RMP.Relief[p]);
	};
	CMGRP1[1]->AddLine(rtxt);
#endif
	
	//CMGRP1[1]->Hint=GetTextByID("SOMOUNT");
	y+=25;
	int RRESID=RMP.NRES;
	
	CMGRP1[3]->CurLine=RMP.RES_DEF;
	for(int p=0;p<RMP.NRES;p++){
		CMGRP1[3]->AddLine(RMP.RES[p].Name);
	};
	CMGRP1[3]->AddLine(rtxt);
	//CMGRP1[3]->Hint=GetTextByID("SORESTOT");
	y+=25;
	
	CMGRP1[4]->CurLine=RMP.MINES_DEF;
	int RMINEID=RMP.NMINES;
	for(int p=0;p<RMP.NMINES;p++){
		CMGRP1[4]->AddLine(RMP.MINES[p].Name);
	};
	CMGRP1[4]->AddLine(rtxt);
	//CMGRP1[4]->Hint=GetTextByID("SORESONMAP");

	
	CMGRP1[5]->AddLine(GetTextByID("MS_NORMAL"));
#ifndef LIMITOPTIONS
	CMGRP1[5]->AddLine(GetTextByID("MS_LARGE"));
	CMGRP1[5]->AddLine(GetTextByID("MS_HUGE"));
#endif
	CMGRP1[5]->CurLine=0;

	ADD_OPT=MENU.addGP_ComboBoxDLX(NULL,0,0,w2,CMB.GPID,0,0,0,FWhite,FYellow,NULL);
	ADD_OPT_VAL=MENU.addGP_ComboBoxDLX(NULL,0,26,100+100+64-26,LONGBOX.GPID,0,0,0,FWhite,FYellow,NULL);
	//GetTextByID("INTF_OPT_TITLE0")

	// Additional options
	int CUROPTVAL[12]={0,0,0,0,0,0,0,0,0,0,0,0};	
	GFILE* F=Gopen("MuliExOpt.dat","r");
	if(F){
		Gscanf(F,"%d%d%d%d%d%d%d",CUROPTVAL,CUROPTVAL+1,CUROPTVAL+2,CUROPTVAL+3,CUROPTVAL+4,CUROPTVAL+5,CUROPTVAL+6);
		Gclose(F);
	};
	for(int a=0; a<7; a++)if(a==4||a==5){
		FAO--;
	}else{
		char cc[128];
		sprintf(cc, "GMTP_TP%d", a+1);
		TGRP1[a+FAO]=MENU.addTextButton(NULL,Tx2,0,GetTextByID(cc),FYellow,FYellow,FYellow,0);
		CMGRP1[a+FAO]=MENU.addGP_ComboBoxDLX(NULL,x2,0,w2,CMB.GPID,0,9,0,FASmCmb,FPSmCmb,NULL);
#ifdef LIMITOPTIONS
		int N=a<3?1:10;
		for(i=0;i<N;i++){
			char ccc[128];
			sprintf(ccc,"@GMTP_TP%d_0%d",a+1,i);
			char* text=GetTextByID(ccc);
			if(text[0]!='@')CMGRP1[a+FAO]->AddLine(text);
		};
		CMGRP1[a+FAO]->CurLine=0;
#else
		for(int i=0;i<10;i++){
			char ccc[128];
			sprintf(ccc,"@GMTP_TP%d_0%d",a+1,i);
			char* text=GetTextByID(ccc);
			if(text[0]!='@')CMGRP1[a+FAO]->AddLine(text);
		};
		CMGRP1[a+FAO]->CurLine=CUROPTVAL[a+FAO-6];
#endif
	}
	
	TGRP1[2] = MENU.addTextButton(NULL,Tx2,0,GetTextByID("INTF_CROOM_O5"),FYellow,FYellow,FYellow,0);
	CMGRP1[2]=MENU.addGP_ComboBoxDLX(NULL,x2,0,w2,CMB.GPID,0,9,0,FASmCmb,FPSmCmb,NULL);
	int cz=0;
	char* cztxt;
	do{
		char clzon[100];
		sprintf(clzon,"#CL_ZONE%d",cz);
		cztxt=GetTextByID(clzon);
#ifdef LIMITOPTIONS
		if(*cztxt!='#'&&CMGRP1[2]->NLines<1){
			CMGRP1[2]->AddLine(cztxt);
			cz++;
		}else break;
#else
		if(*cztxt!='#'){
			CMGRP1[2]->AddLine(cztxt);
			cz++;
		}else break;
#endif
	}while(true);	

	/*for(i=0;i<NGRP1;i++){
		if(TGRP1[i]) TGRP1[i]->y+=5;
	}*/
	
	//GPPicture* AOPPIC=MENU.addGPPicture(NULL,633-65-64,161+26*7,ISYM3.GPID,2);
	//AOPPIC->Visible=0;
	//TextButton* AOPTB=MENU.addTextButton(NULL,633-65-64+2,161+26*5+3,GetTextByID("ADDMPOPT"),FWhite,FWhite,FWhite,0);
	//AOPTB->Visible=0;
	int PrevCOMBO=-1;

	GPPicture* ListBack=MENU.addGPPicture(NULL,MapListX-19,MapListY-6,CGR.GPID,3);	
	ListBack->Visible=0;

	MENU.addClipper(0,0,RealLx,RealLy);

	int VictW = w2-70;
	int TimeW = 66;

	TextButton* VICTxt = MENU.addTextButton(NULL,Tx2,y2+26*6,GetTextByID("INTF_CROOM_VIC"),FYellow,FYellow,FYellow,0);	
	ComboBox* VICT=MENU.addGP_ComboBoxDLX(NULL,x2,y2+26*6,VictW,CMB.GPID,0,9,0,FASmCmb,FPSmCmb,NULL);
	CMGRP1[11] = VICT;
	ComboBox* TIME=MENU.addGP_ComboBoxDLX(NULL,x2+VictW+4,y2+26*6,TimeW,CMB.GPID,0,9,0,FASmCmb,FPSmCmb,NULL);
	CMGRP1[12] = TIME;

	int VicGpLx = GPS.GetGPWidth(CMB.GPID,0);

	for(int i=0;i<NGRP1+2;i++){
		CMGRP1[i]->Center-=5;
		CMGRP1[i]->FontDy-=1;
		CMGRP1[i]->OneDy-=0;
	}

	VScrollBar* AOScr=MENU.addNewGP_VScrollBar(NULL,MabScrX,MabScrY,202-26,NGRP1-6,0,VSC.GPID,0);
	//GPPicture* Exam=MENU.addGPPicture(NULL,495,413,BTNS.GPID,95);
	
	//--------------- Pages 0,2 --------------

	// Select designed map
	VScrollBar* VSB=MENU.addNewGP_VScrollBar(NULL,MabScrX,MabScrY,202/*-26*/,1,0,VSC.GPID,0);	
	ListBox* LBMaps=MENU.addGP_ListBox(NULL,MapListX,MapListY,7,CGR.GPID,0,26,FWhite,FYellow,VSB);

	// Select saved game
	VScrollBar* VSS=MENU.addNewGP_VScrollBar(NULL,MabScrX,MabScrY,202,1,0,VSC.GPID,0);
	ListBox* LBSav=MENU.addGP_ListBox(NULL,MapListX,MapListY,7,CGR.GPID,0,26,FWhite,FYellow,VSS);

	LBMaps->FontDy -= 3;
	LBSav->FontDy -= 3;
	
	LBMaps->FontDx -= 16;
	LBSav->FontDx -= 16;

	//---------------  Options of the Current Game  --------------
	GP_PageControl* GPP=MENU.addPageControl(NULL,11,-150,BTNS.GPID,0);

	int PageX = 602;
	int PageW = 125;
	int PageY = 144;

	UniversalButton* Page0=MENU.addUniversalButton(PageX,PageY,PageW,GetTextByID("INTF_CROOM_GS0"),BTN.GPID,Set0,0,0,0,FPSwitch,FPSwitch,FASwitch);
	UniversalButton* Page1=MENU.addUniversalButton(PageX+PageW,PageY,PageW,GetTextByID("INTF_CROOM_GS1"),BTN.GPID,Set0,0,1,0,FPSwitch,FPSwitch,FASwitch);
	UniversalButton* Page2=MENU.addUniversalButton(PageX+PageW*2,PageY,PageW,GetTextByID("INTF_CROOM_GS2"),BTN.GPID,Set0,0,0,0,FPSwitch,FPSwitch,FASwitch);

	Page0->Hint=GetTextByID("INTF5_HINT23");
	Page1->Hint=GetTextByID("INTF5_HINT24");
	Page2->Hint=GetTextByID("INTF5_HINT25");

	Page0->FontDy -= 1;
	Page1->FontDy -= 1;
	Page2->FontDy -= 1;

	GPP->Hint=GetTextByID("MOPAGES");

	GPP->AddPage(495,128,495+88,128+22,22);
	GPP->AddPage(495+88,128,495+197,128+22,23);
	GPP->AddPage(495+197,128,495+292,128+22,24);

	GPP->CurPage=1;

	// Options of game
	for(int i=0;i<=9;i++){
		char ccx[32];
		sprintf(ccx,"VICOND%d",i);
		char* cc2=GetTextByID(ccx);
		if(cc2!=ccx){
			VICT->AddLine(cc2);
		}//else goto ffe1;
	};
	//VICT->CurLine=0;
	//VICT->Visible=0;
	//VICT->Enabled=0;
ffe1:;
	for(int i=0;i<=9;i++){
		char ccx[32];
		sprintf(ccx,"TIME%d",i);
		char* cc2=GetTextByID(ccx);
		if(cc2!=ccx){
			TIME->AddLine(cc2);
		}//else goto ffe2;
	};
ffe2:;
	if(!Host){
		for(int j=0;j<5;j++)CMGRP1[j]->Enabled=false;
		GPP->Enabled=false;
	};
	
	// Buttons
	int BInChatX = 839;
	int OkX = 899;
	int CancelX = 959;
	int ButY = 706;
	
	if(TPEN.Connected||UseGSC_Login){ // Iternet game
		MENU.addGPPicture(NULL,672,689,CGR.GPID,7);
		GP_Button* ENC=MENU.addGP_Button(NULL,BInChatX,ButY,GmBtn.GPID,5,6);
		ENC->UserParam=99;
		ENC->OnUserClick=&MMItemChoose;
		ENC->Hint=GetTextByID("BACKINCHAT");
	};
	GP_Button* OkBtn=MENU.addGP_Button(NULL,OkX,ButY,GmBtn.GPID,1,0);
	GP_Button* CancelBtn=MENU.addGP_Button(NULL,CancelX,ButY,GmBtn.GPID,3,4);	
	
	if(Host){
		if(SINGLE)
			OkBtn->Hint=GetTextByID("SOSTART");
		else 
			OkBtn->Hint=GetTextByID("MOSTART");
	}else{
		OkBtn->Hint=GetTextByID("MOREADY");
	};
	OkBtn->UserParam=mcmOk;
	OkBtn->OnUserClick=&MMItemChoose;
	OkBtn->Enabled=0;	
	
	CancelBtn->UserParam=mcmCancel;
	CancelBtn->OnUserClick=&MMItemChoose;
	
	if(SINGLE)CancelBtn->Hint=GetTextByID("SOCANCEL");
		else CancelBtn->Hint=GetTextByID("SOCANCEL");
	
	//
	ItemChoose=-1;
	memset(GSets.CGame.PL_INFO,0,sizeof GSets.CGame.PL_INFO);
	for(int i=0;i<8;i++)GSets.CGame.PL_INFO[i].Version=dwVersion;
	if(SINGLE){
		NPlayers=1;
		memset(&GSets.CGame.PL_INFO[0],0,sizeof GSets.CGame.PL_INFO[0]);
		GSets.CGame.PL_INFO[0].PlayerID=0x12345678;
		MyDPID=0x12345678;
		GSets.CGame.PL_INFO[0].ColorID=0;
		strcpy(GSets.CGame.PL_INFO[0].name,PlName);

	}else PIEnumeratePlayers(GSets.CGame.PL_INFO,false);
	if(NPlayers>7)NPlayers=7;
	
	byte CUSED=0;
	for(int i=0;i<NPlayers;i++){
		if(GSets.CGame.PL_INFO[i].PlayerID!=MyDPID){
			CUSED|=(1<<GSets.CGame.PL_INFO[i].ColorID);
		};
	};
	cc1=0;
	mss1=1;
	while(CUSED&mss1){
		cc1++;
		mss1<<=1;
	};
	GameInProgress=0;
	if(!SINGLE)PIEnumeratePlayers(GSets.CGame.PL_INFO,false);
	if(NPlayers>7)NPlayers=7;
	for(int i=0;i<NPlayers;i++){
		if(GSets.CGame.PL_INFO[i].PlayerID==MyDPID){
			GSets.CGame.PL_INFO[i].ColorID=cc1;
			ColorBack[i]->Nation=cc1;
			if(Host)GSets.CGame.PL_INFO[i].Ready=1;else GSets.CGame.PL_INFO[i].Ready=0;
			GSets.CGame.PL_INFO[i].Host=Host;
			GSets.CGame.PL_INFO[i].CD=CMGRP1[2]->CurLine;
			GSets.CGame.PL_INFO[i].Version=dwVersion;
			GSets.CGame.PL_INFO[i].Rank=GetLogRank();
			GSets.CGame.PL_INFO[i].ProfileID=GetMyProfile();
		}else{
			memset(GSets.CGame.PL_INFO+i,0,sizeof PlayerInfo);
		};
	};
	if(Host){
		CreateGamesList(LBSav);
		CreateMapsList(LBMaps);
	};
	PlayerMenuMode=1;
	GameInProgress=0;
	tmtmt=0;
	REALTIME=0;
	GLOBALTIME=0;
	PGLOBALTIME=0;
	int pp=1;
	GFILE* f=Gopen("multi.opt","r");
	if(f){
		int v=0;
		Gscanf(f,"%d",&v);
		if(v==1234){	
			for(int i=0;i<=5;i++){
				v=0;
				Gscanf(f,"%d",&v);
				CMGRP1[i]->CurLine=v;
			};
			v=0;
			Gscanf(f,"%d",&v);
			VICT->CurLine=v;
			Gscanf(f,"%d",&v);
			TIME->CurLine=v;
		};
		Gclose(f);

	};
	KeyPressed=0;
	PSUMM.ClearPingInfo();
	PBACK.Clear();
	ReceiveAll();
	bool P1E;
	bool P2E;
	bool P3E;
	int DYL;
	//int p;
	char* PNAME="";
	int NCLINES=0;
	char oldname[64]="";
	bool MyOldVers=0;
	bool PresOldVers=0;
	int PREVSD=GetTickCount()-3000;
	word MaxVers=0;
	int CURSUMM=0;
	int NOMREADY=-1;
	char* OLDV=GetTextByID("OLDVER");
	char* NOCONN=GetTextByID("$NOCONN");
	if(NOCONN[0]=='$')NOCONN=_engNOCONN;
	int FInTime=GetTickCount();
	char* COMPTXT="Computer";
	char* DIFSAV=GetTextByID("DIFSAV");
	//char* DIFMAP=GetTextByID("DIFMAP");
	bool PrevPress=0;
	//TPEN.HostMessage[0]=0;
	if(SINGLE){
		VICT->CurLine=0;
		VICT->Visible=0;
	};
	
	
	int MAXOPTVAL[8]={10,3,3,10,3,4,7,9};
	int NOPTV=7;
	int PREVCUROPT=-1;
	int PREVCNAME=GetTickCount();

	/*
	if(Single){
		CUROPTVAL[0]=0;
		CUROPTVAL[4]=0;
		MAXOPTVAL[0]=0;
		MAXOPTVAL[4]=0;
	};
	*/
	LARGE_INTEGER V1;
	QueryPerformanceCounter(&V1);
	LARGE_INTEGER V2;
	QueryPerformanceFrequency(&V2);
	DWORD GMGUID=V1.LowPart+V2.LowPart;
	if(SINGLE){
		FILE* F1=fopen("comp.opt","r");
		if(F1){
			for(int i=0;i<6;i++){
				int h=0;
				int z=fscanf(F1,"%d",&h);
				if(z==1){
					if(h>4)h=4;
					if(h<0)h=0;
					COMPS[i+1]->CurLine=h;
				};
			};
			fclose(F1);
		}else{
			COMPS[1]->CurLine=1;
		};
	};
	StartMixing();
	char* ONLY="(private)";//GetTextByID("ONLYOU");
	int NCS0=0;
	//TIME->CurLine=4;
	//VICT->CurLine=1;
	int NL0=CMGRP1[0]->NLines;
	do{
		if(UseGSC_Login){
			ChatProcess();
			if(CheckPersonality(CHATMESSAGE)){
				CHATBOX->CursPos=strlen(CHATMESSAGE);
			};
		};
		if(OkBtn->Enabled){
			OkBtn->ActiveFrame=0;
			OkBtn->PassiveFrame=1;
		}else{
			OkBtn->ActiveFrame=2;
			OkBtn->PassiveFrame=2;
		}

		if(Host)ServerDPID=MyDPID;

		if(SINGLE){
			ADD_OPT->Visible=1;
			ADD_OPT->Enabled=1;
			ADD_OPT_VAL->Visible=1;
			//AOPPIC->Visible=1;
			//AOPTB->Visible=1;
			ADD_OPT_VAL->Enabled=1;
#ifdef INETTESTVERSION
		//	for(int t=0;t<12;t++)CUROPTVAL[t]=0;
		//	ADD_OPT_VAL->CurLine=0;
		//	CMGRP1[0]->CurLine=0;
		//	CMGRP1[4]->CurLine=0;
		//	GPP->CurPage=1;
#endif
		}else{
			ADD_OPT->Visible=1;
			ADD_OPT->Enabled=1;
			ADD_OPT_VAL->Visible=Host;
			//AOPPIC->Visible=Host;
			//AOPTB->Visible=Host;
			ADD_OPT_VAL->Enabled=Host;
		};
		int a=AOScr->GetPos();
		for(int i=0; i<NGRP1; i++){
			int y = y2+(i-a)*26;
			int y1 = y+CMGRP1[i]->y1-CMGRP1[i]->y;
			if(CMGRP1[i]){
				CMGRP1[i]->y=y;
				CMGRP1[i]->y1=y1;
			}
			if(TGRP1[i]) TGRP1[i]->y=y;
		};
		FAO=6;
		for(int i=0; i<5;i++){
			CUROPTVAL[i]=CMGRP1[i+6]->CurLine;
		};
		CUROPTVAL[6]=CMGRP1[1]->CurLine;
		/*
		if(PrevCOMBO!=ADD_OPT->CurLine||CUROPTVAL[ADD_OPT->CurLine]!=ADD_OPT_VAL->CurLine||!Host){
			//PrevCOMBO=ADD_OPT->CurLine;
			int aocl=ADD_OPT->CurLine;
			int aovcl=ADD_OPT_VAL->CurLine;
			if(PrevCOMBO==aocl&&Host){
				CUROPTVAL[aocl]=aovcl;
			};
			ADD_OPT->Clear();
			for(int i=0;i<NOPTV;i++){
				char ccc[100];
				sprintf(ccc,"GMTP_TP%d",i+1);
				char cc1[128];
				char cc2[256];
				sprintf(cc1,"@GMTP_TP%d_0%d",i+1,CUROPTVAL[i]);
				sprintf(cc2,"%s: %s",GetTextByID(ccc),GetTextByID(cc1));
				LimitString(cc2,FYellow,230);
				ADD_OPT->AddLine(cc2);
			};
			ADD_OPT->CurLine=aocl;
			int n=MAXOPTVAL[aocl];
			ADD_OPT_VAL->Clear();
			for(i=0;i<10;i++){
				char ccc[128];
				sprintf(ccc,"@GMTP_TP%d_0%d",aocl+1,i);
				char* text=GetTextByID(ccc);
				if(text[0]!='@')ADD_OPT_VAL->AddLine(text);
			};
			if(PrevCOMBO==aocl){
				ADD_OPT_VAL->CurLine=aovcl;
			}else{ 
				ADD_OPT_VAL->CurLine=CUROPTVAL[aocl];
			};
			PrevCOMBO=aocl;
		};
		*/
		for(int i=0;i<NPlayers;i++){
			if(GSets.CGame.PL_INFO[i].PlayerID==MyDPID){
				if(Host)GSets.CGame.PL_INFO[i].Ready=1;
				GSets.CGame.PL_INFO[i].Host=Host;
				//GSets.CGame.PL_INFO[i].CD=;
				GSets.CGame.PL_INFO[i].Version=dwVersion;
				GSets.CGame.PL_INFO[i].Rank=GetLogRank();
				GSets.CGame.PL_INFO[i].ProfileID=GetMyProfile();
				if(Host)GSets.CGame.PL_INFO[i].Game_GUID=GMGUID;
			};
		};
		int CLY=19;
		
		
		bool AddChat=0;
		ProcessMessages();
		if(GameInProgress)goto FinCLC;
		if(!SINGLE){
			if(KeyPressed&&LastKey==13){
				if(CHATBOX->Active&&CHATMESSAGE[0]){
					ControlLingvo(CHATMESSAGE);
					if(UseGSC_Login&&strstr(CHATMESSAGE,"-->")){
						CheckForPersonalChat(CHATMESSAGE);
					}else SendChat(CHATMESSAGE,0);
					AddChatString(
						GetPlNameByID(MyDPID),
						CHATMESSAGE,375,
							&WhiteFont,ChatMess,ChatSender,NCHATS,MAXCHATS);
					AddChat=1;
					CHATMESSAGE[0]=0;
				};
				KeyPressed=0;
				LastKey=0;
			}else
			if(TPEN.GlobalChat.NCStr>NCS0){
				for(int j=NCS0;j<TPEN.GlobalChat.NCStr;j++){
					if(strstr(TPEN.GlobalChat.CStr[j].PlName,ONLY)){
						AddChatString(
							TPEN.GlobalChat.CStr[j].PlName,
							TPEN.GlobalChat.CStr[j].Message,375,
								&WhiteFont,ChatMess,ChatSender,NCHATS,MAXCHATS);
						AddChat=1;
					};
				};
				NCS0=TPEN.GlobalChat.NCStr;
			};
			if(CHATSTRING[0]){
				if(CHATSTRING[0]=='@'&&CHATSTRING[1]=='@'&&CHATSTRING[2]=='@'&&CHATSTRING[3]==127){
					DWORD ID;
					char ccx[64];
					int z=sscanf(CHATSTRING,"%s%d",ccx,&ID);
					if(z==2&&ID==MyDPID){ItemChoose=mcmCancel;
						CHATSTRING[0]=0;
						WaitWithMessage(GetTextByID("YOUWKICK"));
					};
				}else{
					AddChatString(
							GetPlNameByID(CHATDPID),
							CHATSTRING,375,
							FWhite,ChatMess,ChatSender,NCHATS,MAXCHATS);
					CHATSTRING[0]=0;
					AddChat=1;
				};
			};
				
			SendPings();
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
		};
		DYL=CHSCR->SPos;	
		P1E=GPP->CurPage==0;
		P2E=GPP->CurPage==1;
		P3E=GPP->CurPage==2;
		//Exam->Visible=P2E;
		VSB->Visible=P1E;
		LBMaps->Visible=P1E;
		AOScr->Visible=P2E;
		for(int p=0;p<NGRP1;p++){
			CMGRP1[p]->Visible=P2E;
			if(TGRP1[p]) TGRP1[p]->Visible=P2E;
			CMGRP1[p]->Enabled=P2E&&Host;
		};
		VSS->Visible=P3E;
		LBSav->Visible=P3E;
		//RMS->Visible=P2E;

		VICT->Visible=P2E;
		TIME->Visible=!P3E;
		if(VICT->CurLine==0){
			TIME->Visible = 0;
		}else{			
			TIME->Visible = VICT->Visible;
		};
		VICTxt->Visible = VICT->Visible;
		if(TIME->Visible){
			VICT->x1 = VICT->x+VictW-1;
			VICT->DLX = VicGpLx-VictW;
			VICT->OneLx = VictW;
		}else{
			VICT->x1 = VICT->x+w2-1;
			VICT->DLX = VicGpLx-w2;
			VICT->OneLx = w2;
		}

		ADD_OPT->Visible=P2E;
		ADD_OPT->Enabled=P2E;
		ADD_OPT_VAL->Visible=P2E&&Host;
		ADD_OPT_VAL->Enabled=P2E&&Host;
		//AOPPIC->Visible=P2E&&Host;
		//AOPTB->Visible=P2E&&Host;
		
		//tmm1=GetRealTime();
		StdKeys();
		if(ItemChoose==mcmOk&&!OkBtn->Enabled)ItemChoose=-1;
		//tmm1=GetRealTime()-tmm1;
		//tmm2=GetRealTime();
		AnalyseMessages();
		if(GameInProgress)goto FinCLC;
		//tmm2=GetRealTime()-tmm2;
		if(MyOldVers)GPPB->Hint=OLDV;
		else GPPB->Hint=NULL;
		//processing optoons
		for(int i=0;i<NPlayers;i++){
			if(GSets.CGame.PL_INFO[i].PlayerID==MyDPID){
				if(DefaultTeam!=-1){
					AliasBack[i]->Enabled=0;
					AliasBack[i]->Nation=DefaultTeam;
					AliasID[i]->Message[0]=DefaultTeam?'0'+DefaultTeam:'X';
				};
				if(DefaultColor!=-1){
					ColorBack[i]->Enabled=0;
					ColorBack[i]->Nation=DefaultColor;
					Flags[i]->Nation=ColorBack[i]->Nation;
					Flags[i]->SpriteID=ColorBack[i]->Nation;
				};
				if(DefaultNation!=-1){
					MNATION[i]->CurLine=DefaultNation;
					MNATION[i]->Enabled=0;
				};
			};
		};
		if(Host){
			if(DefaultLand!=-1){
				CMGRP1[0]->CurLine=DefaultLand;
				CMGRP1[0]->Enabled=0;
			};
			if(DefaultCZone!=-1){
				CMGRP1[2]->CurLine=DefaultCZone;
				CMGRP1[2]->Enabled=0;
			};
			if(DefaultMapSize!=-1){
				CMGRP1[5]->CurLine=DefaultMapSize;
				CMGRP1[5]->Enabled=0;
			};
			if(DefaultVCond!=-1){
				VICT->CurLine=DefaultVCond;
				VICT->Enabled=0;
			};
			if(DefaultGTime!=-1){
				TIME->CurLine=DefaultGTime;
				TIME->Enabled=0;
			};
			if(DefaultPTime!=-1){
				CMGRP1[9]->CurLine=DefaultPTime;
				CMGRP1[9]->Enabled=0;
			};
			if(DefaultPage!=-1){
				GPP->CurPage=DefaultPage;
				GPP->Enabled=0;
				Page0->Enabled=0;
				Page1->Enabled=0;
				Page2->Enabled=0;
				Page0->State=DefaultPage==0;
				Page1->State=DefaultPage==1;
				Page2->State=DefaultPage==2;
			};
			if(DefaultRelief!=-1){
				CMGRP1[1]->CurLine=DefaultRelief;
				CMGRP1[1]->Enabled=0;
			};
			if(DefaultRes!=-1){
				CMGRP1[3]->CurLine=DefaultRes;
				CMGRP1[3]->Enabled=0;
			};
			if(DefaultMines!=-1){
				CMGRP1[4]->CurLine=DefaultMines;
				CMGRP1[4]->Enabled=0;
			};
			if(DefaultArmy!=-1){
				CMGRP1[6]->CurLine=DefaultArmy;
				CMGRP1[6]->Enabled=0;
			};
			if(DefaultVision!=-1){
				CMGRP1[7]->CurLine=DefaultVision;
				CMGRP1[7]->Enabled=0;
			};
			if(DefaultCannons!=-1){
				CMGRP1[8]->CurLine=DefaultCannons;
				CMGRP1[8]->Enabled=0;
			};
			if(DefaultAutosave!=-1){
				CMGRP1[10]->CurLine=DefaultAutosave;
				CMGRP1[10]->Enabled=0;
			};
		};
		

		MENU.MarkToDraw();
		MENU.ProcessDialogs();
#ifdef AC_DEMO
		CMGRP1[4]->CurLine=0;
#endif
		
		// Page activate
		if(Host){		
			if(Page0->State){
				GPP->CurPage = 0;
			}else
			if(Page1->State){
				GPP->CurPage = 1;
			}else
			if(Page2->State){
				GPP->CurPage = 2;
			};
		}else{
			Page0->Enabled=0;
			Page1->Enabled=0;
			Page2->Enabled=0;
			Page0->State=0;
			Page1->State=0;
			Page2->State=0;
			if(GPP->CurPage==0){
				Page0->State=1;
			}else
			if(GPP->CurPage==1){
				Page1->State=1;
			}else
			if(GPP->CurPage==2){
				Page2->State=1;
			};
		}


		if(GameInProgress)goto FinCLC;
		//tmm3=GetRealTime();
		char ccc[128];
		for(int p=NPlayers;p<8;p++)CurrentMaxPing[p]=-1;
		if(PSUMM.NPL&&PSUMM.PSET[0].NPings){
			int png=GetMaxRealPing();			
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
			if(png>=MinPing0&&png<300)MaxPingTime=300;
			if(MaxPingTime)MaxPingTime+=500;
			_str cc2;
			CreateDiffStr(cc2);
			sprintf(ccc,"max ping: %d  (%s)",png,cc2.pchar());
			//ShowString(10,768-24,ccc,&SmallYellowFont);
			MENU.DefaultHint.Assign(ccc);
		};		
		SHOWREC->Visible=RecordMode;	
		ProcessMixing();
		MENU.RefreshView();

		if(GameInProgress)goto FinCLC;
		for(int i=NPlayers;i<NPlayers+NComp;i++){
			COMALLY[i-NPlayers]=AliasBack[i]->Nation;
			COMNATION[i-NPlayers]=MNATION[i]->CurLine;
		};
		//tmm3=GetRealTime()-tmm3;
		//sprintf(STRI,"%d %d %d %d",COUN,tmm1,tmm2,tmm3);
		
		//TIME->Enabled=TIME->Visible;
		NOMREADY=-1;
		if(!SINGLE)for(int i=0;i<8;i++)ReadyFlow[i]->Visible=0;
		if(Host){
			int HostID=-1;
			byte CMask=0;
			for(int i=0;i<NPlayers;i++){
				if(GSets.CGame.PL_INFO[i].Host)HostID=i;
			};
			VICT->Visible=!SINGLE;
			//TIME->Enabled=TIME->Visible;
			for(int q=0;q<NPlayers;q++){
				CMask|=1<<GSets.CGame.PL_INFO[q].ColorID;
			};
			int NN=0;
			for(int q=NPlayers;q<7;q++)if(GSets.CGame.PL_INFO[HostID].COMPINFO[q]){
				word W=GSets.CGame.PL_INFO[HostID].COMPINFO[q];
				int Color=W&15;
				CMask|=1<<Color;
			};
			for(int q=0;q<8;q++)if(CMask&(1<<q))NN++;
			/*
			if(NN>4){
				CMGRP1[0]->Enabled=0;
				CMGRP1[0]->CurLine=0;
			};
			*/
			if(HostID!=-1){
				int PPP;
				if(!SINGLE){
					if((PPP=GetReadyPercent())<100){
						sprintf(ReadyFlow[HostID]->Message,"%d%%",PPP);
						ReadyFlow[HostID]->Visible=1;
						MREADY[HostID]->Visible=0;
						MREADY[HostID]->Enabled=0;
						NOMREADY=HostID;
					}else{
						ReadyFlow[HostID]->Visible=0;
						MREADY[HostID]->Visible=1;
						MREADY[HostID]->Enabled=1;
						NOMREADY=-1;;
					};
				};
				GSets.CGame.PL_INFO[HostID].MapName[0]=0;
				if(GPP->CurPage==0){
					if(LBMaps->CurItem>=0&&LBMaps->NItems){
						strcpy(GSets.CGame.PL_INFO[HostID].MapName,LBMaps->GetItem(LBMaps->CurItem)->Message);
					};
					strcpy(GSets.CGame.cgi_CurrentMap,GSets.CGame.PL_INFO[HostID].MapName);
				}else
				if(GPP->CurPage==1){
					GSets.CGame.PL_INFO[HostID].MapStyle=CMGRP1[0]->CurLine;
					GSets.CGame.PL_INFO[HostID].HillType=CMGRP1[1]->CurLine;
					GSets.CGame.PL_INFO[HostID].CD      =CMGRP1[2]->CurLine;
					GSets.CGame.PL_INFO[HostID].StartRes=CMGRP1[3]->CurLine;
					GSets.CGame.PL_INFO[HostID].ResOnMap=CMGRP1[4]->CurLine;
					if(VICT->CurLine){
						GSets.CGame.PL_INFO[HostID].VictCond=VICT->CurLine+1;
					}else{
						GSets.CGame.PL_INFO[HostID].VictCond=VICT->CurLine;
					}
					GSets.CGame.PL_INFO[HostID].GameTime=TIME->CurLine;
					for(int i=0;i<7;i++)
						GSets.CGame.PL_INFO[HostID].UserParam[i]=CUROPTVAL[i];
					char Nats[9];
					char Nats1[9];
					strcpy(Nats,"00000000");
					strcpy(Nats1,"00000000");
					char* NTCHAR="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
					int NP=0;
					for(int q=0;q<NPlayers;q++){
						Nats[7-GSets.CGame.PL_INFO[q].ColorID]=NTCHAR[MNATION[q]->CurLine+1];
						//Nats1[7-GSets.CGame.PL_INFO[q].ColorID]='1';
					};
					for(int q=NPlayers;q<7;q++)if(GSets.CGame.PL_INFO[HostID].COMPINFO[q]){
						word W=GSets.CGame.PL_INFO[HostID].COMPINFO[q];
						int Nat=W>>11;
						int Color=W&15;
						int Diff=(W>>8)&7;
						int Team=(W>>4)&15;
						Nats[7-Color]=NTCHAR[Nat+1];
					};
					for(int u=0;u<8;u++)if(Nats[u]!='0')NP++;
					int r=r0&65535;
					//sscanf(ccc,"%d",&r);
					int s=CMGRP1[0]->CurLine+((NP-2)<<4)+(CMGRP1[1]->CurLine<<8)
						+(CMGRP1[3]->CurLine<<12)+(CMGRP1[4]->CurLine<<16)
						+(CMGRP1[2]->CurLine<<20);
					//sprintf(GSets.CGame.cgi_CurrentMap,"RND %X %X %s.m3d",r,s,Nats);
					int op=CUROPTVAL[0]+CUROPTVAL[1]*10+CUROPTVAL[2]*100+CUROPTVAL[3]*1000+CUROPTVAL[4]*1000000;
					sprintf(GSets.CGame.PL_INFO[HostID].MapName,"RN%d %X %X %s %d.m3d",CMGRP1[5]->CurLine,r,s,Nats,op);
					int sty=CMGRP1[0]->CurLine==RSTYID?rand()%RSTYID:CMGRP1[0]->CurLine;
					int rel=CMGRP1[1]->CurLine==RRELID?rand()%RRELID:CMGRP1[1]->CurLine;
					int res=CMGRP1[3]->CurLine==RRESID?rand()%RRESID:CMGRP1[3]->CurLine;
					int min=CMGRP1[4]->CurLine==RMINEID?rand()%RMINEID:CMGRP1[4]->CurLine;
					strcpy(Nats,"00000000");
					for(int q=0;q<NPlayers;q++){
						Nats[7-GSets.CGame.PL_INFO[q].ColorID]=MNATION[q]->CurLine==RNATID?
							NTCHAR[1+(rand()%RNATID)]:NTCHAR[MNATION[q]->CurLine+1];
					};
					for(int q=NPlayers;q<7;q++)if(GSets.CGame.PL_INFO[HostID].COMPINFO[q]){
						word W=GSets.CGame.PL_INFO[HostID].COMPINFO[q];
						int Nat=W>>11;
						int Color=W&15;
						int Diff=(W>>8)&7;
						int Team=(W>>4)&15;
						Nats[7-Color]=MNATION[q]->CurLine==RNATID?
							NTCHAR[1+(rand()%RNATID)]:NTCHAR[MNATION[q]->CurLine+1];
						//NP++;
					};
					if(NP<2)NP=2;
					s=sty+((NP-2)<<4)+(rel<<8)
						+(res<<12)+(min<<16)+(CMGRP1[2]->CurLine<<20);
					sprintf(GSets.CGame.cgi_CurrentMap,"RN%d %X %X %s %d.m3d",CMGRP1[5]->CurLine,r,s,Nats,op);
					CMGRP1[0]->NLines=NL0;
					if(CMGRP1[5]->CurLine==0){
						if(NP>4)CMGRP1[0]->NLines=2;
						if(CMGRP1[0]->CurLine>=CMGRP1[0]->NLines)CMGRP1[0]->CurLine=0;
					};
				}else{
					if(LBSav->CurItem>=0&&LBSav->NItems){
						strcpy(GSets.CGame.PL_INFO[HostID].MapName,LBSav->GetItem(LBSav->CurItem)->Message);
					};
					strcpy(GSets.CGame.cgi_CurrentMap,GSets.CGame.PL_INFO[HostID].MapName);
				};
			};
		}else{
			VICT->Enabled=0;
			TIME->Enabled=0;
			for(int i=0;i<NPlayers;i++){
				if(GSets.CGame.PL_INFO[i].Host){
					if(GSets.CGame.PL_INFO[i].VictCond)
						VICT->CurLine=GSets.CGame.PL_INFO[i].VictCond-1;
					else
						VICT->CurLine=GSets.CGame.PL_INFO[i].VictCond;
					TIME->CurLine=GSets.CGame.PL_INFO[i].GameTime;
					GSets.CGame.PL_INFO[0].VictCond=GSets.CGame.PL_INFO[i].VictCond;
					GSets.CGame.PL_INFO[0].GameTime=GSets.CGame.PL_INFO[i].GameTime;
					extern int StartGTime;
					extern int StartVCond;
					StartGTime=GSets.CGame.PL_INFO[0].GameTime;
					StartVCond=GSets.CGame.PL_INFO[0].VictCond;
					char* MPN=GSets.CGame.PL_INFO[i].MapName;
					if(MPN[0]=='R'&&MPN[1]=='N'&&MPN[3]==' '){
						int sz=MPN[2]-'0';
						if(sz>2)sz=2;
						if(sz<0)sz=0;
						CMGRP1[5]->CurLine=sz;
					};
				};
				if(!SINGLE)ReadyFlow[i]->Visible=0;
			};
			for(int i=0;i<NGRP1;i++)CMGRP1[i]->Enabled=0;
			VICT->Enabled=0;
			TIME->Enabled=0;
			NOMREADY=-1;
		};
		// ------ Visualisation of pages -----------
		if(GPP->CurPage==0){
			ListBack->Visible=1;
			ListClip->Enabled=0;
			if(strcmp(GSets.CGame.cgi_CurrentMap,LastPrName)){
				strcpy(LastPrName,GSets.CGame.cgi_CurrentMap);
				if(GetPreview(GSets.CGame.cgi_CurrentMap,Preview+4)){
					PREV->Visible=1;
					Decode(Preview+4,XLAT,PREVIEWLX*PREVIEWLY);
				}else{
					PREV->Visible=false;
				};
			};
		}else
		if(GPP->CurPage==1){
			ListBack->Visible=0;
			ListClip->Enabled=1;
			char Nats[9];
			char Nats1[9];
			strcpy(Nats,"00000000");
			strcpy(Nats1,"00000000");
			char* NTCHAR="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
			for(int q=0;q<NPlayers;q++){
				Nats[7-GSets.CGame.PL_INFO[q].ColorID]=NTCHAR[MNATION[q]->CurLine+1];
				//Nats1[7-GSets.CGame.PL_INFO[q].ColorID]='1';
			};
			for(int q=NPlayers;q<7;q++)if(GSets.CGame.PL_INFO[HostID].COMPINFO[q]){
				word W=GSets.CGame.PL_INFO[HostID].COMPINFO[q];
				int Nat=W>>11;
				int Color=W&15;
				int Diff=(W>>8)&7;
				int Team=(W>>4)&15;
				Nats[7-Color]=NTCHAR[Nat+1];
			};
			int r=(r0)&65535;
			//sprintf(ccc,"%d",&r);
			//int NP=NPlayers;
			//for(q=NPlayers;q<7;q++)if(GSets.CGame.PL_INFO[HostID].COMPINFO[q]){
			//	NP++;
			//};
			int NP=0;
			for(int u=0;u<8;u++)if(Nats[u]!='0')NP++;
			if(NP<2)NP=2;
			for(int j=0;j<NP;j++)Nats1[7-j]='1';
			sprintf(Currand,"Preview\\P%d%d%d",CMGRP1[0]->CurLine,CMGRP1[2]->CurLine,NP);
			if(strcmp(Currand,LastPrName)){
				strcpy(LastPrName,Currand);
				if(GetPreview(Currand,Preview+4)){
					PREV->Visible=1;
					Decode(Preview+4,XLAT,PREVIEWLX*PREVIEWLY);
				}else{
					PREV->Visible=false;
				};
			};
		}else{
			ListBack->Visible=1;
			ListClip->Enabled=0;
			PREV->Visible=0;
			if(strcmp(GSets.CGame.cgi_CurrentMap,LastPrName)){
				strcpy(LastPrName,GSets.CGame.cgi_CurrentMap);
				int S=GetMapSUMM(GSets.CGame.cgi_CurrentMap);
				for(int v=0;v<NPlayers;v++)if(GSets.CGame.PL_INFO[v].PlayerID==MyDPID)GSets.CGame.PL_INFO[v].CHKSUMM=S;
			};
		};
		MaxVers=0;
		for(int i=0;i<NPlayers;i++){
			if(GSets.CGame.PL_INFO[i].Version>MaxVers)MaxVers=GSets.CGame.PL_INFO[i].Version;
		};
		for(int i=0;i<8;i++){
			if(i<NPlayers){
				if(GSets.CGame.PL_INFO[i].PlayerID==MyDPID){
					GSets.CGame.PL_INFO[i].NationID=MNATION[i]->CurLine;
					GSets.CGame.PL_INFO[i].ColorID=ColorBack[i]->Nation;
					GSets.CGame.PL_INFO[i].GroupID=AliasBack[i]->Nation;
					if(Host)GSets.CGame.PL_INFO[i].Page=GPP->CurPage;
					bool ch=memcmp(&MYPINF,GSets.CGame.PL_INFO+i,sizeof MYPINF)!=0;
					if(ch||GetTickCount()-PREVSD>3000){
						if(!SINGLE){
							SETPLAYERDATA(MyDPID,(void*)&GSets.CGame.PL_INFO[i].NationID,sizeof(PlayerInfo)-36,ch);
						};
						memcpy(&MYPINF,GSets.CGame.PL_INFO+i,sizeof MYPINF);
						PREVSD=GetTickCount();
					};
					if(ch&&Host&&!SINGLE)PSUMM.ClearPingInfo();
					DPNAME dpName;
					ZeroMemory(&dpName, sizeof(DPNAME));
					dpName.dwSize = sizeof(DPNAME);
					dpName.lpszShortNameA = MNAME[i]->Str;
					dpName.lpszLongNameA = NULL;
					if(strcmp(dpName.lpszShortNameA,oldname)||GetTickCount()-PREVCNAME>3000){
						if(!SINGLE)SETPLAYERNAME(&dpName,strcmp(dpName.lpszShortNameA,oldname)!=0);
						strcpy(oldname,dpName.lpszShortNameA);
						PREVCNAME=GetTickCount();
					};
					MNATION[i]->Enabled=P2E;
					MNATION[i]->Visible=P2E;
					AliasBack[i]->Visible=1;
					AliasBack[i]->Enabled=1;
					AliasID[i]->Visible=1;
					AliasID[i]->Enabled=1;
					CurrentNation=MNATION[i]->CurLine;
					//MGRP[i]->Enabled=true;
					//if(!SINGLE)NameBack[i]->Visible=1;
					MNAME[i]->Visible=1;
					MNAME[i]->Enabled=0;//1;
					if(strstr(TPEN.HostMessage,"0017")&&!SINGLE){
						MNAME[i]->Enabled=0;
					};
					if(KICK[i]){
						KICK[i]->Visible=0;
						KICK[i]->Enabled=0;
					};
					if(!SINGLE){
						//NationBack[i]->Visible=0;
						//NationBack[i]->Enabled=0;
						//CD_Back[i]->Visible=1;
					};
					ColorBack[i]->Visible=1;
					ColorBack[i]->Enabled=1;
					//if(!SINGLE)ReadyBack[i]->Visible=1;
					MyOldVers=GSets.CGame.PL_INFO[i].Version!=MaxVers;
				}else{
					if(!SINGLE){
						//NationBack[i]->Visible=1;
						//NationBack[i]->Enabled=1;
						//CD_Back[i]->Visible=0;
					};
					AliasBack[i]->Visible=0;
					AliasBack[i]->Enabled=0;
					if(KICK[i]){
						KICK[i]->Visible=1;
						KICK[i]->Enabled=1;
						strcpy(KICK[i]->Lines[0],MNAME[i]->Str);
						while(GetRLCStrWidth(KICK[i]->Lines[0],FYellow)>99){
							KICK[i]->Lines[0][strlen(KICK[i]->Lines[0])-1]=0;
						};
						if(KICK[i]->CurLine==1){
							char cc[32];
							sprintf(cc,"@@@0002KICK %d",GSets.CGame.PL_INFO[i].PlayerID);
							SendChat(cc,0);
							KICK[i]->CurLine=0;
							if(DoNewInet){
								//assert(GSets.CGame.PL_INFO[i].PlayerID!=1);
								DeepDeletePeer(GSets.CGame.PL_INFO[i].PlayerID);
							};
						};
						MNAME[i]->Visible=0;//!!!!!!!!
						MNAME[i]->Enabled=0;
						MNAME[i]->Active=0;
					}else{
						MNAME[i]->Visible=1;//!!!!!!!!
						MNAME[i]->Enabled=0;
						MNAME[i]->Active=0;
					};
					//if(!SINGLE)NameBack[i]->Visible=0;
					MNATION[i]->Enabled=0;
					ColorBack[i]->Visible=0;
					ColorBack[i]->Enabled=0;
					//if(!SINGLE)ReadyBack[i]->Visible=0;
					//MGRP[i]->Enabled=false;
				};
				if(!SINGLE){
					//if(MNAME[i]->Active)NameBack[i]->SpriteID=12+(i%3);
					//else NameBack[i]->SpriteID=9+(i%3);
				};
				//Flags[i]->SpriteID=(i*229+GetRealTime()/30)%45;
				Flags[i]->Nation=ColorBack[i]->Nation;
				Flags[i]->SpriteID=ColorBack[i]->Nation;
				//AliasID[i]->
				
				if(AliasBack[i]->Nation){
					AliasID[i]->Message[0]='0'+AliasBack[i]->Nation;
				}else{
					AliasID[i]->Message[0]='X';
				};
				
				MNATION[i]->Visible=true;
				//MCOLOR[i]->Visible=true;
				if(!SINGLE){
					MREADY[i]->SpriteID=8+(GSets.CGame.PL_INFO[i].Ready!=0);
					if(i!=NOMREADY){
						MREADY[i]->Visible=true;
						MREADY[i]->Enabled=true;
					};
				};
				Flags[i]->Visible=1;
				if(!SINGLE)CD_Present[i]->Visible=1;
				//MREADY[i]->Visible=1;
				MNATION[i]->Visible=1;
				AliasID[i]->Visible=1;
				AliasID[i]->Enabled=1;
				int v=GSets.CGame.PL_INFO[i].Version;
				if(!SINGLE){
					char VERS[128];
					if(v>255){
						sprintf(VERS,"M%d",v>>8);
					}else{
						sprintf(VERS,"%d.%d%d",v/100,(v/10)%10,v%10);
					};
					strcpy(VersText[i]->Message,VERS);
					//strcpy(VersTextShadow[i]->Message,VERS);
				
					if(v!=MaxVers&&(GetTickCount()/300)&1){
						VersText[i]->ActiveFont=&SmallRedFont;
						VersText[i]->PassiveFont=&SmallRedFont;
					}else{
						VersText[i]->ActiveFont=&SmallYellowFont;
						VersText[i]->PassiveFont=&SmallYellowFont;
					};
					VersText[i]->Visible=1;
					//VersTextShadow[i]->Visible=1;
				};
				PresOldVers=v!=MaxVers;
				//ReadyBack[i]->Visible=0;
				//MGRP[i]->Visible=true;
			}else{
				if(i<NComp+NPlayers){
					if(!SINGLE){
						//NameBack[i]->Visible=0;
						//NationBack[i]->Visible=1;
						//CD_Back[i]->Visible=1;
						CD_Present[i]->Visible=1;
						//ReadyBack[i]->Visible=1;
						VersText[i]->Visible=0;
						//VersTextShadow[i]->Visible=0;
						MREADY[i]->Visible=0;
					};
					ColorBack[i]->Visible=1;
					Flags[i]->Visible=1;
					
					MNAME[i]->Visible=1;
					MNAME[i]->Enabled=0;
					if(KICK[i]){
						KICK[i]->Visible=1;
						KICK[i]->Enabled=1;
					};
					MNAME[i]->Str=COMPTXT;
					MNAME[i]->Active=0;
					MNATION[i]->Visible=1;
					MNATION[i]->Enabled=1;
					int pp=i-NPlayers;
					MNATION[i]->CurLine=COMNATION[pp];
					AliasBack[i]->Visible=1;
					AliasBack[i]->Enabled=1;
					AliasID[i]->Visible=1;
					AliasID[i]->Enabled=1;
					
					if(COMALLY[pp])AliasID[i]->Message[0]='0'+COMALLY[pp];
					else AliasID[i]->Message[0]='X';
					
					//MGRP[i]->Visible=false;
					//Flags[i]->SpriteID=(i*229+GetRealTime()/30)%45;
					Flags[i]->Nation=ColorBack[i]->Nation;
					Flags[i]->SpriteID=ColorBack[i]->Nation;
				}else{
					if(!SINGLE){
						//NameBack[i]->Visible=0;
						//NationBack[i]->Visible=0;
						//CD_Back[i]->Visible=0;
						CD_Present[i]->Visible=0;
						//ReadyBack[i]->Visible=0;
						VersText[i]->Visible=0;
						//VersTextShadow[i]->Visible=0;
						MREADY[i]->Visible=0;
					};
					ColorBack[i]->Visible=0;
					Flags[i]->Visible=0;
					
					MNAME[i]->Visible=0;
					MNAME[i]->Enabled=0;
					if(KICK[i]){
						KICK[i]->Visible=0;
						KICK[i]->Enabled=0;
					};
					MNAME[i]->Active=0;
					MNATION[i]->Visible=0;
					MNATION[i]->Enabled=0;
					AliasBack[i]->Visible=0;
					AliasBack[i]->Enabled=0;
					AliasID[i]->Visible=0;
					AliasID[i]->Enabled=0;
					//MGRP[i]->Visible=false;
					
				};
			};
		};
		PrevPress=realLpressed;
		if(Host){
			//if(KeyPressed){
			//	if(LastKey==107&&NComp+NPlayers<7)NComp++;
			//	if(LastKey==109&&NComp)NComp--;
			//};
		};
		if(!SINGLE){
			if(!PIEnumeratePlayers(GSets.CGame.PL_INFO,true)){
				ItemChoose=mcmCancel;
			};
		};
		if(NPlayers>7)NPlayers=7;
		if(!Host){
			int HostID=-1;
			int MyID=-1;
			for(int i=0;i<NPlayers;i++){
				if(GSets.CGame.PL_INFO[i].Host)HostID=i;
				if(GSets.CGame.PL_INFO[i].PlayerID==MyDPID)MyID=i;
			};
			if(HostID!=-1&&MyID!=-1){
				switch(GSets.CGame.PL_INFO[HostID].Page){
				case 1:
					GPP->CurPage=1;
					CMGRP1[0]->CurLine=GSets.CGame.PL_INFO[HostID].MapStyle;
					CMGRP1[1]->CurLine=GSets.CGame.PL_INFO[HostID].HillType;
					CMGRP1[2]->CurLine=GSets.CGame.PL_INFO[HostID].CD;
					CMGRP1[3]->CurLine=GSets.CGame.PL_INFO[HostID].StartRes;
					CMGRP1[4]->CurLine=GSets.CGame.PL_INFO[HostID].ResOnMap;
					for(int i=0;i<7;i++){
						int v=GSets.CGame.PL_INFO[HostID].UserParam[i];
						if(v!=CUROPTVAL[i]){
							CUROPTVAL[i]=v;
							PREVCUROPT=-1;
						};
					};
					break;
				case 2:
					GPP->CurPage=2;
					LBSav->ClearItems();
					LBSav->AddItem(GSets.CGame.PL_INFO[HostID].MapName,0);
					break;
				case 0:
					GPP->CurPage=0;
					LBMaps->ClearItems();
					LBMaps->AddItem(GSets.CGame.PL_INFO[HostID].MapName,0);
					break;
				};
				strcpy(GSets.CGame.PL_INFO[MyID].MapName,GSets.CGame.PL_INFO[HostID].MapName);
				GSets.CGame.PL_INFO[MyID].MapStyle=GSets.CGame.PL_INFO[HostID].MapStyle;
				GSets.CGame.PL_INFO[MyID].HillType=GSets.CGame.PL_INFO[HostID].HillType;
				GSets.CGame.PL_INFO[MyID].CD=GSets.CGame.PL_INFO[HostID].CD;
				GSets.CGame.PL_INFO[MyID].StartRes=GSets.CGame.PL_INFO[HostID].StartRes;
				GSets.CGame.PL_INFO[MyID].ResOnMap=GSets.CGame.PL_INFO[HostID].ResOnMap;
				GSets.CGame.PL_INFO[MyID].Game_GUID=GSets.CGame.PL_INFO[HostID].Game_GUID;
				strcpy(GSets.CGame.cgi_CurrentMap,GSets.CGame.PL_INFO[HostID].MapName);
				for(int t=0;t<5;t++)CMGRP1[t+6]->CurLine=CUROPTVAL[t];
				CMGRP1[1]->CurLine=CUROPTVAL[6];
			};
		};
		for(int i=0;i<8;i++){
			if(i<NPlayers){
				MNATION[i]->CurLine=GSets.CGame.PL_INFO[i].NationID;
				ColorBack[i]->Nation=GSets.CGame.PL_INFO[i].ColorID;
				AliasBack[i]->Nation=GSets.CGame.PL_INFO[i].GroupID;
			};
		};
		for(int i=0;i<NPlayers;i++){
			if(GSets.CGame.PL_INFO[i].PlayerID==MyDPID&&ItemChoose==mcmOk&&!Host){
				GSets.CGame.PL_INFO[i].Ready=1;
			};
		};
		HostID=-1;
		for(int i=0;i<NPlayers;i++){
			if(GSets.CGame.PL_INFO[i].Host)HostID=i;
		};
		if(HostID!=-1){
			if(Host){
				for(int i=0;i<7;i++){
#ifndef AC_DEMO					
					COMPS[i]->Visible=(i>=NPlayers);
#endif
					COMPS[i]->Enabled=COMPS[i]->Visible;
					//BCOMP[i]->Visible=0;
					if(COMPS[i]->Visible&&COMPS[i]->CurLine){
						if(!COMPPREV[i]){
							byte msk=0;
							for(int j=0;j<NPlayers;j++)msk|=1<<ColorBack[j]->Nation;
							for(int j=NPlayers;j<7;j++){
								if(COMPS[j]->Visible&&COMPS[j]->CurLine){
									msk|=1<<ColorBack[j]->Nation;
								};
							};
							int defc=-1;
							for(int j=0;j<7&&defc==-1;j++){
								if(!(msk&(1<<j)))defc=j;
							};
							if(defc==-1)defc=0;
							ColorBack[i]->Nation=defc;
							AliasBack[i]->Nation=7;
						};

						if(!SINGLE){
							//NameBack[i]->Visible=0;
							//NationBack[i]->Visible=1;
							//CD_Back[i]->Visible=0;
							CD_Present[i]->Visible=0;
							//ReadyBack[i]->Visible=0;
							MREADY[i]->Visible=0;
							VersText[i]->Visible=0;
							//VersTextShadow[i]->Visible=0;
						};
						ColorBack[i]->Visible=1;
						Flags[i]->Visible=1;
						MNAME[i]->Visible=0;
						MNAME[i]->Enabled=0;
						if(KICK[i]){
							KICK[i]->Visible=0;
							KICK[i]->Enabled=0;
						};
						MNAME[i]->Active=0;
						MNATION[i]->Visible=1;
						MNATION[i]->Enabled=1;
						int pp=i-NPlayers;
						AliasBack[i]->Visible=1;
						AliasBack[i]->Enabled=1;
						AliasID[i]->Visible=1;
						AliasID[i]->Enabled=1;
						
						if(AliasBack[i]->Nation){
							AliasID[i]->Message[0]='0'+AliasBack[i]->Nation;
						}else{
							AliasID[i]->Message[0]='X';
						};

						//MGRP[i]->Visible=false;
						//Flags[i]->SpriteID=(i*229+GetRealTime()/30)%45;
						Flags[i]->Nation=ColorBack[i]->Nation;
						Flags[i]->SpriteID=ColorBack[i]->Nation;
						word W=int(Flags[i]->Nation)+
							(int(AliasBack[i]->Nation)<<4)+
							(int(COMPS[i]->CurLine)<<8)+(int(MNATION[i]->CurLine)<<11);
						GSets.CGame.PL_INFO[HostID].COMPINFO[i]=W;
					}else GSets.CGame.PL_INFO[HostID].COMPINFO[i]=0;
					COMPPREV[i]=COMPS[i]->Visible&&COMPS[i]->CurLine;
				};
			}else{
				for(int i=0;i<7;i++){
					if(i>=NPlayers){
						word W=GSets.CGame.PL_INFO[HostID].COMPINFO[i];
						if(W){
#ifndef AC_DEMO
							COMPS[i]->Visible=1;
#endif
							COMPS[i]->CurLine=(W>>8)&7;
							
							if(!SINGLE){
								//NameBack[i]->Visible=0;
								//NationBack[i]->Visible=1;
								//CD_Back[i]->Visible=0;
								CD_Present[i]->Visible=0;
								//ReadyBack[i]->Visible=0;
								MREADY[i]->Visible=0;
								VersText[i]->Visible=0;
								//VersTextShadow[i]->Visible=0;
							};
							ColorBack[i]->Visible=1;
							Flags[i]->Visible=1;
							MNAME[i]->Visible=0;
							MNAME[i]->Enabled=0;
							if(KICK[i]){
								KICK[i]->Visible=0;
								KICK[i]->Enabled=0;
							};
							MNAME[i]->Active=0;
							MNATION[i]->Visible=1;
							MNATION[i]->Enabled=1;
							MNATION[i]->CurLine=W>>11;
							int pp=i-NPlayers;
							AliasBack[i]->Visible=1;
							AliasBack[i]->Enabled=1;
							AliasID[i]->Visible=1;
							AliasID[i]->Enabled=1;
							AliasBack[i]->Nation=(W&255)>>4;
							
							if(AliasBack[i]->Nation){
								AliasID[i]->Message[0]='0'+AliasBack[i]->Nation;
							}else{
								AliasID[i]->Message[0]='X';
							};

							//MGRP[i]->Visible=false;
							//Flags[i]->SpriteID=(i*229+GetRealTime()/30)%45;
							ColorBack[i]->Nation=W&15;
							Flags[i]->Nation=ColorBack[i]->Nation;
							Flags[i]->SpriteID=ColorBack[i]->Nation;
							//BCOMP[i]->Visible=0;//1;
						}else{
							COMPS[i]->Visible=0;
							//BCOMP[i]->Visible=0;
						};
						COMPS[i]->Enabled=0;
					}else{
						COMPS[i]->Visible=0;
						COMPS[i]->Enabled=0;
						//BCOMP[i]->Visible=0;
					};
				};
			};
		};
		GMREADY=true;
		for(int i=0;i<NPlayers;i++){
			if(GSets.CGame.PL_INFO[i].PlayerID!=MyDPID&&!GSets.CGame.PL_INFO[i].Ready)GMREADY=false;
		};
		MSS1=0;
		/*
		for(i=0;i<NPlayers;i++){
			byte MSS2=1<<GSets.CGame.PL_INFO[i].ColorID;
			if(MSS1&MSS2)GMREADY=false;
			MSS1|=MSS2;
			if(!GSets.CGame.PL_INFO[i].Ready)GMREADY=false;
		};
		*/
		for(int i=0;i<7;i++){
			if(COMPS[i]->Visible&&COMPS[i]->CurLine){
				byte MSS2=1<<ColorBack[i]->Nation;
				if(MSS1&MSS2&&Host)GMREADY=false;
				MSS1|=MSS2;
			};
		};
	
		if(Host){
//#ifndef SINGLETESTNETWORK
			if(!SINGLE){
				if(NPlayers<2)GMREADY=false;
				if(!CheckPingsReady())GMREADY=false;
				byte bs=0;
				for(int i=0;i<NPlayers;i++)bs|=1<<AliasBack[i]->Nation;
				if(!(bs&1)){
					bs|=1;
					if(bs==2||bs==4||bs==8||bs==16||bs==32||bs==64||bs==128)GMREADY=false;
				};
				for(int i=0;i<NPlayers;i++)if(CurrentMaxPing[i]==-1)GMREADY=0;
				if(PresOldVers)GMREADY=0;
			};

			//teams calculating
			int ntm=0;
			byte ams=0;
			int cur_tm[8];
			memset(cur_tm,0xFF,sizeof cur_tm);
			byte HumMask=0;
			byte AiMask=0;
			for(int i=0;i<8;i++)if(AliasID[i]->Visible){
				int tm=AliasBack[i]->Nation;
				int cl=ColorBack[i]->Nation;
				if(cur_tm[cl]!=-1){
					if(tm!=cur_tm[cl]||!cur_tm[cl]){
						GMREADY=0;
					};
				}else cur_tm[cl]=tm;
				int alb=AliasBack[i]->Nation;
				if(!alb)ntm++;
				else{
					if(!(ams&(1<<alb))){
						ntm++;
						ams|=1<<alb;
					};
				};
				//checking for different color set for AI & human opponents
				byte ms=1<<cl;
				if(i<NPlayers){
					HumMask|=ms;
				}else{
					AiMask|=ms;
				};
			};
			if(ntm<2||(HumMask&AiMask))GMREADY=0;
			

			if(NOEQCOLOR){
				byte MAS=0;
				for(int i=0;i<8;i++)if(AliasID[i]->Visible){
					int cl=1<<ColorBack[i]->Nation;
					if(MAS&cl)GMREADY=0;
					MAS|=cl;
				};
			};
//#endif
			//MENU.DefaultHint[0]=0;
			if(GPP->CurPage==2){
				int MINS=0;
				for(int i=0;i<NPlayers;i++)
					if(GSets.CGame.PL_INFO[i].PlayerID==MyDPID)MINS=GSets.CGame.PL_INFO[i].CHKSUMM;
				/*
				for(i=0;i<NPlayers;i++)if(GSets.CGame.PL_INFO[i].CHKSUMM!=MINS){
					GMREADY=0;
					strcpy(MENU.DefaultHint,DIFSAV);
				};
				*/
			};
			OkBtn->Enabled=GMREADY;
			//OkBtn->Visible=GMREADY;
			if(TPEN.MyRoom&&TPEN.MyRoom->RoomConnected){
				TPEN.CreateMyRoomInfoString(TPEN.HostMessage);
			};
		}else{
			OkBtn->Enabled=!MyOldVers;
			//OkBtn->Visible=!MyOldVers;
			if(!CheckExistConn()){
				OkBtn->Enabled=0;
				//OkBtn->Visible=0;
				if(GetTickCount()-FInTime>2000){
					MENU.DefaultHint.Assign(NOCONN);
					char C[512];
					PrintBadConn(C);
					MENU.DefaultHint.Add(C);
				};
			};
			if(P1E){
				//OkBtn->Enabled=PREV->Visible;
				//OkBtn->Visible=PREV->Visible;
				if(!PREV->Visible){
					if((GetTickCount()/200)&1){
						LBMaps->AFont=FWhite;
						LBMaps->PFont=FYellow;
					}else{
						LBMaps->AFont=&RedFont;
						LBMaps->PFont=&RedFont;
					};
				}else{
					LBMaps->AFont=FWhite;
					LBMaps->PFont=FYellow;
				};
			};
		};
		
		//checking readyness
		if(ItemChoose==mcmOk&&!Host)ItemChoose=-1;
		if(pp){
//			SlowLoadPalette("1\\agew_1.pal");
			pp=0;
			ReceiveAll();
			//PSUMM.ClearPingInfo();
		};
		if(ItemChoose==99){
			ProcessInternetConnection(0);
			ItemChoose=-1;
			if(PlayerMenuMode!=1)ItemChoose=mcmSingle;
		};
FinCLC:;
	   if(RejectThisPlayer||CheckPlayerToExit())ItemChoose=mcmCancel;
	}while(ItemChoose==-1&&PlayerMenuMode==1);
	if(Host){
		FILE* Fx=fopen("MuliExOpt.dat","w");
		if(F){
			fprintf(Fx,"%d %d %d %d %d %d %d",CUROPTVAL[0],CUROPTVAL[1],CUROPTVAL[2],CUROPTVAL[3],CUROPTVAL[4],CUROPTVAL[5],CUROPTVAL[6],CUROPTVAL[7]);
			fclose(Fx);
		};
	};
	if(SINGLE){
		FILE* Fx=fopen("comp.opt","w");
		if(Fx){
			for(int i=0;i<6;i++){
				fprintf(Fx,"%d ",COMPS[i+1]->CurLine);
			};
			fclose(Fx);
		};
	};
	if(Host&&PlayerMenuMode!=-1){
		int i;
		for(i=0;i<8;i++)MPL_NatRefTBL[i]=0xFF;
		int NN[8]={0,0,0,0,0,0,0,0};
		for(i=NPlayers;i<7;i++)if(GSets.CGame.PL_INFO[HostID].COMPINFO[i]){
			word W=GSets.CGame.PL_INFO[HostID].COMPINFO[i];
			int Nat=W>>11;
			int Color=W&15;
			MPL_NatRefTBL[Color]=Color;
			NN[Color]=1;
		};
		for(i=0;i<NPlayers;i++){
			int c=GSets.CGame.PL_INFO[i].ColorID;
			if(MPL_NatRefTBL[c]==0xFF){
				MPL_NatRefTBL[c]=c;
			};
		};
		for(i=0;i<NPlayers;i++){
			int c=GSets.CGame.PL_INFO[i].ColorID;
			if(NN[c]){
				for(int j=0;j<8;j++)if(MPL_NatRefTBL[j]==0xFF){
					MPL_NatRefTBL[j]=c;
					GSets.CGame.PL_INFO[i].ColorID=j;
					j=8;
				};
			}else{
				NN[c]=1;
			}
		};
		for(i=0;i<8;i++)if(MPL_NatRefTBL[i]==0xFF)MPL_NatRefTBL[i]=i;
		if(SINGLE){
			memcpy(GSets.CGame.cgi_NatRefTBL,MPL_NatRefTBL,8);
		};
	};
	if(NCHATS){
		for(int i=0;i<NCHATS;i++){
			free(ChatMess[i]);
			free(ChatSender[i]);
		};
		free(ChatMess);
		free(ChatSender);
	};
	if(GSets.CGame.PL_INFO[0].VictCond){
		int t=60;
		char cc3[32];
		sprintf(cc3,"-TIME%d",GSets.CGame.PL_INFO[0].GameTime);
		sscanf(GetTextByID(cc3),"%d",&t);
		GSets.CGame.PL_INFO[0].GameTime=t*60;
	};
	if(Host){
		f=Gopen("multi.opt","w");
		if(f){
			if(VICT->CurLine)
				Gprintf(f,"1234 %d %d %d %d %d %d %d %d",CMGRP1[0]->CurLine,CMGRP1[1]->CurLine,CMGRP1[2]->CurLine,CMGRP1[3]->CurLine,CMGRP1[4]->CurLine,CMGRP1[5]->CurLine,VICT->CurLine+1,TIME->CurLine);
			else
				Gprintf(f,"1234 %d %d %d %d %d %d %d %d",CMGRP1[0]->CurLine,CMGRP1[1]->CurLine,CMGRP1[2]->CurLine,CMGRP1[3]->CurLine,CMGRP1[4]->CurLine,CMGRP1[5]->CurLine,VICT->CurLine,TIME->CurLine);
			Gclose(f);
		};
	};
	free(Preview);
	if(ItemChoose==mcmCancel){
		CloseMPL();
	};
	return (ItemChoose==mcmOk)||PlayerMenuMode!=1;
};