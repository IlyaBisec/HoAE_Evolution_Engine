int ProcessWars(){
	if(!WARS.NWars)return false;
	KeyPressed=0;
	LastKey=0;
	
	//SQPicture Back("Interface\\Background_Historical_Wars.bmp");
	SQPicture Back("Interf2\\Background_Historical_Create.bmp");

	LocalGP BTNS("Interface\\Wars");
	LocalGP OCAM("Interface\\One_Campaign");
	LocalGP SCROL("Interface\\_Slider");
	LocalGP OK1("Interface\\anm_start_on");
	LocalGP OK2("Interface\\anm_start_off");
	LocalGP CANCEL1("Interface\\anm_cancel_on");
	LocalGP CANCEL2("Interface\\anm_cancel_off");
	LocalGP ENC1("Interface\\anm_enc_on");
	LocalGP ENC2("Interface\\anm_enc_off");
	LocalGP FONT("interf3\\Fonts\\FontC10");
	RLCFont FontY(FONT.GPID);
	FontY.SetColorTable(3);
	LoadOptionalTable(3,"2\\orange.grd");

	RLCFont FontW(FONT.GPID);
	FontW.SetWhiteColor();
	DialogsSystem MMenu(0,0);

	LocalGP HFONT("interf3\\Fonts\\FontC10");
	RLCFont hfnt(HFONT.GPID);
	hfnt.SetWhiteColor();
	MMenu.HintFont=&hfnt;
	MMenu.HintY=701;
	MMenu.HintX=18;

	MMenu.addPicture(NULL,0,0,&Back,&Back,&Back);
	VideoButton* ENC=MMenu.addVideoButton(NULL,862,468,ENC1.GPID,ENC2.GPID);
	ENC->UserParam=0;
	ENC->OnUserClick=&RunEnciclopia;
	ENC->Hint=GetTextByID("ENCICL");
	VideoButton* OkBtn=MMenu.addVideoButton(NULL,862,568,OK1.GPID,OK2.GPID);
	OkBtn->OnUserClick=&MMItemChoose;
	OkBtn->UserParam=mcmOk;
	OkBtn->Hint=GetTextByID("WAR_RUN");
	VideoButton* CancelBtn=MMenu.addVideoButton(NULL,862,668,CANCEL1.GPID,CANCEL2.GPID);
	CancelBtn->OnUserClick=&MMItemChoose;
	CancelBtn->UserParam=mcmCancel;
	CancelBtn->Hint=GetTextByID("WAR_CANCEL");

	for(int i=0;i<WARS.NWars&&i<5;i++)
		MMenu.addGPPicture(NULL,68,166+i*26,OCAM.GPID,6);

	GPPicture* ZBTL[5];
	for(i=0;i<5;i++){
		ZBTL[i]=MMenu.addGPPicture(NULL,444,166+26*i,OCAM.GPID,6);
		ZBTL[i]->Visible=0;
	};
	VScrollBar* CWS=MMenu.addNewGP_VScrollBar(NULL,395,156,146,1,0,SCROL.GPID,0);
	ListBox* CWar=MMenu.addGP_ListBox(NULL,68,166,5,BTNS.GPID,6,26,&WhiteFont,&YellowFont,CWS);
	CWar->FontDy-=2;
	CWar->M_Over=9;
	VScrollBar* CBS=MMenu.addNewGP_VScrollBar(NULL,755,156,146,1,0,SCROL.GPID,0);
	ListBox* CBat=MMenu.addGP_ListBox(NULL,444,166,5,BTNS.GPID,0,26,&WhiteFont,&YellowFont,CBS);
	CBat->FontDy-=2;
	CBat->M_Over=3;
	TextViewer* TV[32];
	VScrollBar* VS[32];
	for(i=0;i<WARS.NWars;i++){
		CWar->AddItem(WARS.Wars[i].Name,0);
		TV[i]=MMenu.addTextViewer(NULL,78,353,663,275,WARS.Wars[i].Text,&YellowFont);
		TV[i]->Visible=0;
		TV[i]->Active=0;
		TV[i]->Enabled=0;
		TV[i]->SymSize=19;
		TV[i]->PageSize=(TV[i]->y1-TV[i]->y)/TV[i]->SymSize;
		TV[i]->Enabled=false;
		TV[i]->Visible=false;
		VS[i]=MMenu.addNewGP_VScrollBar(NULL,755,338,291,1,0,SCROL.GPID,0);
		TV[i]->AssignScroll(VS[i]);
		VS[i]->Visible=0;
	};
	int CurWar=-1;
WStart:;
	int mm=1;
	ItemChoose=-1;
	KeyPressed=0;
	do{
		if(CWar->CurItem!=CurWar){
			CBat->ClearItems();
			CurWar=CWar->CurItem;
			for(int j=0;j<WARS.Wars[CurWar].NBatles;j++){
				CBat->AddItem(WARS.Battles[WARS.Wars[CurWar].BattleList[j]].SmallHeader,0);
			};
		};
		ProcessMessages();
		StdKeys();
		MMenu.MarkToDraw();
		MMenu.ProcessDialogs();
		MMenu.RefreshView();
		for(i=0;i<WARS.NWars;i++){
			TV[i]->Enabled=false;
			TV[i]->Visible=false;
			TV[i]->Active=false;
			VS[i]->Visible=false;
		};
		TV[CWar->CurItem]->Enabled=1;
		TV[CWar->CurItem]->Visible=1;
		TV[CWar->CurItem]->Active=1;
		VS[CWar->CurItem]->Visible=VS[CWar->CurItem]->SMaxPos>0;
		int NBTL=WARS.Wars[CWar->CurItem].NBatles;
		for(i=0;i<5;i++)ZBTL[i]->Visible=i<NBTL;
		if(mm){
			LoadPalette("1\\agew_1.pal");
			mm=0;
		};
		StdKeys();
	}while(ItemChoose==-1);
	if(ItemChoose==mcmOk){
		LoadPalette("1\\agew_1.pal");
		if(!ProcessOneBattle(WARS.Wars[CWar->CurItem].BattleList[CBat->CurItem])){
			//SlowUnLoadPalette("2\\agew_1.pal");
			goto WStart;
		};
		return WARS.Wars[CWar->CurItem].BattleList[CBat->CurItem];
	};
	LoadPalette("1\\agew_1.pal");
	return -1;
};

bool MPL_WaitingBattleGame(bool Host,int BattleID){
	ClearLPACK();
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
	LoadFog(1);
	LocalGP BTNS("Interface\\Multi_opt");
	LocalGP OK1("Interface\\anm_start_1_on");
	LocalGP OK2("Interface\\anm_start_1_off");
	LocalGP CANCEL1("Interface\\anm_cancel_on");
	LocalGP CANCEL2("Interface\\anm_cancel_off");
	LocalGP DIS("Interface\\an_start_disable");
	LocalGP SCROL("Interface\\_Slider");	
	LocalGP MASKA("Interface\\History_mask1");
	LocalGP NewFlag("Interface\\NewFlag");
	LocalGP INCHAT("Interface\\IButtons");
	PSUMM.ClearPingInfo();
	PlayerInfo MYPINF;
	memset(&MYPINF,0,sizeof MYPINF);

	//SQPicture Back("Interface\\Background_Historical_Create.bmp");
	SQPicture Back("Interf2\\Background_Historical_Create.bmp");
	
	SQPicture Prev(WARS.Battles[BattleID].MiniMap);

	LocalGP FONT("interf3\\Fonts\\FontC10");
	RLCFont FontY(FONT.GPID);
	FontY.SetColorTable(3);
	LoadOptionalTable(3,"2\\orange.grd");
	RLCFont FontW(FONT.GPID);
	FontW.SetColorTable(4);
	LoadOptionalTable(4,"2\\white.grd");

	DialogsSystem MENU(0,0);
	MENU.OkSound=GetSound("START");
	LocalGP HFONT("interf3\\Fonts\\FontC10");
	RLCFont hfnt(HFONT.GPID);
	hfnt.SetWhiteColor();
	MENU.HintFont=&hfnt;
	MENU.HintY=701;
	MENU.HintX=18;
	Picture* GPPB=MENU.addPicture(NULL,0,0,&Back,&Back,&Back);
	GPPicture*  NameBack[2];
	GPPicture*  NationBack[2];
	GPPicture*  ColorBack[2];
	GPPicture*  Flags[2];
	GPPicture*  CD_Back[2];
	GPPicture*  CD_Present[2];
	GPPicture*  ReadyBack[2];
	InputBox*   MNAME  [2];
	ComboBox*   MNATION[2];
	TextButton* ReadyFlow[2];
	//ColoredBar* MCOLOR [8];
	TextButton* VersTextShadow[8];
	TextButton* VersText[8];
	GPPicture*  MREADY[2];
	//MENU.addInputBox(NULL,0,0,GSets.CGame.cgi_CurrentMap,64,400,25,&FontW,&FontW);
	ChatViewer* CHVIEW=MENU.addChatViewer(NULL,506,424,7,26,250,&ChatMess,&ChatSender,&NCHATS);
	char CHATMESSAGE[256]="";
	InputBox* CHATBOX=MENU.addInputBox(NULL,588-14,612,CHATMESSAGE,250,190,22,&YellowFont,&WhiteFont);
	CHATBOX->Active=1;
	VScrollBar* CHSCR=MENU.addNewGP_VScrollBar(NULL,778,409,198,1,0,SCROL.GPID,0);
	CHSCR->Visible=0;
	Picture* PREV=MENU.addPicture(NULL,496,156,&Prev,&Prev,&Prev);
	GPPicture* PMASK=MENU.addGPPicture(NULL,496,156,MASKA.GPID,0);
	VScrollBar* TXSB=MENU.addNewGP_VScrollBar(NULL,443,283,351,1,0,SCROL.GPID,0);
	
	ResFile FF1=RReset(WARS.Battles[BattleID].Brief);
	if(FF1!=INVALID_HANDLE_VALUE){
		ResFile FF2=RRewrite("miss.txt");
		if(FF2!=INVALID_HANDLE_VALUE){
			RBlockWrite(FF2,WARS.Battles[BattleID].BigHeader,strlen(WARS.Battles[BattleID].BigHeader));
			char cx=0x0D;
			RBlockWrite(FF2,&cx,1);
			cx=0x0A;
			RBlockWrite(FF2,&cx,1);
			cx=0x0D;
			RBlockWrite(FF2,&cx,1);
			cx=0x0A;
			RBlockWrite(FF2,&cx,1);
			byte* Data=znew(byte,RFileSize(FF1));
			RBlockRead(FF1,Data,RFileSize(FF1));
			RBlockWrite(FF2,Data,RFileSize(FF1));
			RClose(FF2);
		};
		RClose(FF1);
	};

	TextViewer* BTTXT=MENU.addTextViewer(NULL,47,294,385,330,"miss.txt",&YellowFont);
	BTTXT->SymSize=20;
	BTTXT->PageSize=(BTTXT->y1-BTTXT->y)/BTTXT->SymSize;

	BTTXT->AssignScroll(TXSB);
	for(int i=0;i<2;i++){
		NameBack[i]=MENU.addGPPicture(NULL,38+5,161+i*26,BTNS.GPID,9+(i%3));
		NameBack[i]->Visible=0;
		MNAME[i]=MENU.addInputBox(NULL,32+5+5,160+i*26,GSets.CGame.PL_INFO[i].name,20,128,20,&WhiteFont,&YellowFont);
		MNAME[i]->Visible=0;
		NationBack[i]=MENU.addGPPicture(NULL,183,161+i*26,BTNS.GPID,i%3);
		NationBack[i]->Visible=0;
		ColorBack[i]=MENU.addGPPicture(NULL,295+8,161+i*26,BTNS.GPID,6+(i%3));
		ColorBack[i]->Visible=0;
		Flags[i]=MENU.addGPPicture(NULL,295-5+19+1,161-11+12+i*26,NewFlag.GPID,(i*17)%45);
		Flags[i]->Visible=0;
		CD_Back[i]=MENU.addGPPicture(NULL,358+5,161+i*26,BTNS.GPID,3+(i%3));
		CD_Back[i]->Visible=0;
		CD_Present[i]=MENU.addGPPicture(NULL,365,162+i*26,BTNS.GPID,25);
		CD_Present[i]->Visible=0;
		VersTextShadow[i]=MENU.addTextButton(NULL,376+12-12,161+6+i*26,"        ",&SmallBlackFont,&SmallBlackFont,&SmallBlackFont,1);
		VersTextShadow[i]->Visible=0;
		VersText[i]=MENU.addTextButton(NULL,376+12-12-1,161+6-2+i*26,"        ",&SmallYellowFont,&SmallYellowFont,&SmallYellowFont,1);
		VersText[i]->Visible=0;
		ReadyBack[i]=MENU.addGPPicture(NULL,399+4,161+i*26,BTNS.GPID,15+(i%3));
		ReadyBack[i]->Visible=0;

		MNAME[i]->Hint=GetTextByID("MOCHANGENAME");
		MNAME[i]->Visible=false;
		MNAME[i]->Enabled=false;
		MNATION[i]=MENU.addGP_ComboBox(NULL,183,161+i*26,BTNS.GPID,28+8*(i%3),8,0,&WhiteFont,&YellowFont,NULL);
		MNATION[i]->Hint=GetTextByID("MMCHANGENAT");
		MREADY[i]=MENU.addGPPicture(NULL,413,162+i*26,BTNS.GPID,26);
		MREADY[i]->Visible=0;
		MNATION[i]->AddLine(WARS.Battles[BattleID].RedUnits);
		MNATION[i]->AddLine(WARS.Battles[BattleID].BlueUnits);
		MNATION[i]->CurLine=i;
		MNATION[i]->Enabled=Host;

		MNATION[i]->FontDx-=2;
		//MNATION[i]->OneDx-=2;
		//MCOLOR[i]=MENU.addColoredBar(32+318,160+i*25,32,20,0xF7);
		//MCOLOR[i]->Hint=GetTextByID("MMCHANGECOLOR");
		//MCOLOR[i]->OnMouseOver=&ColorMouseOver;
		ReadyFlow[i]=MENU.addTextButton(NULL,332+100-8,164+i*26,"100%",&YellowFont,&YellowFont,&YellowFont,1);
		ReadyFlow[i]->Visible=0;
		ReadyFlow[i]->Enabled=0;
	};
	
	GPPicture* DISBL=MENU.addGPPicture(NULL,862,568,DIS.GPID,0);
	VideoButton* OkBtn;
	if(Host){
		OkBtn=MENU.addVideoButton(NULL,862,568,OK1.GPID,OK2.GPID);
		OkBtn->Hint=GetTextByID("MOSTART");
	}else{
		OkBtn=MENU.addVideoButton(NULL,862,568,OK1.GPID,OK2.GPID);
		OkBtn->Hint=GetTextByID("MOREADY");
	};
	OkBtn->UserParam=mcmOk;
	OkBtn->OnUserClick=&MMItemChoose;
	VideoButton* CancelBtn=MENU.addVideoButton(NULL,862,668,CANCEL1.GPID,CANCEL2.GPID);
	CancelBtn->UserParam=mcmCancel;
	CancelBtn->OnUserClick=&MMItemChoose;
	CancelBtn->Hint=GetTextByID("MOCANCEL");
	if(TPEN.Connected&&TPEN.MyRoom){
		GP_Button* ENC=MENU.addGP_Button(NULL,862,468,INCHAT.GPID,9,8);
		ENC->UserParam=99;
		ENC->OnUserClick=&MMItemChoose;
		ENC->Hint=GetTextByID("BACKINCHAT");
	};

	ItemChoose=-1;
	memset(GSets.CGame.PL_INFO,0,sizeof GSets.CGame.PL_INFO);
	GameInProgress=0;
	PIEnumeratePlayers(GSets.CGame.PL_INFO,0);
	if(NPlayers>2)NPlayers=2;
	byte CUSED=0;
	for(i=0;i<NPlayers;i++){
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
	for(i=0;i<NPlayers;i++){
		if(GSets.CGame.PL_INFO[i].PlayerID==MyDPID){
			GSets.CGame.PL_INFO[i].ColorID=cc1;
			ColorBack[i]->Nation=cc1;
			if(Host)GSets.CGame.PL_INFO[i].Ready=1;else GSets.CGame.PL_INFO[i].Ready=0;
			GSets.CGame.PL_INFO[i].Host=Host;
			GSets.CGame.PL_INFO[i].CD=1;
			GSets.CGame.PL_INFO[i].Version=dwVersion;
		}else{
			memset(GSets.CGame.PL_INFO+i,0,sizeof PlayerInfo);
		};
	};
	PIEnumeratePlayers(GSets.CGame.PL_INFO,0);
	if(NPlayers>2)NPlayers=2;
	PlayerMenuMode=1;
	GameInProgress=0;
	tmtmt=0;
	REALTIME=0;
	GLOBALTIME=0;
	PGLOBALTIME=0;
	int pp=1;
	KeyPressed=0;
	PBACK.Clear();
	PSUMM.ClearPingInfo();
	ReceiveAll();
	byte MSS1=0;
	bool GMREADY=true;
	int HostID=-1;
	int NCLINES=0;
	char* PNAME="";
	int DYL;
	char oldname[64]="";
	bool MyOldVers=0;
	bool PresOldVers=0;
	int PREVSD=GetTickCount();
	int NOMREADY=-1;
	word MaxVers=0;
	char* OLDV=GetTextByID("OLDVER");
	for(i=0;i<2;i++)GSets.CGame.PL_INFO[i].MapStyle=BattleID;
	if(Host)strcpy(MENU.DefaultHint,GetTextByID("DEVHISUS"));
	do{
		for(i=0;i<NPlayers;i++){
			if(GSets.CGame.PL_INFO[i].PlayerID==MyDPID){
				if(Host)GSets.CGame.PL_INFO[i].Ready=1;
				GSets.CGame.PL_INFO[i].Host=Host;
				GSets.CGame.PL_INFO[i].CD=1;
				GSets.CGame.PL_INFO[i].Version=dwVersion;
			};
		};
		int CLY=19;
		int NCL=7;
		bool AddChat=0;
		HostID=-1;
		for(i=0;i<NPlayers;i++){
			if(GSets.CGame.PL_INFO[i].Host)HostID=i;
		};
		ProcessMessages();
		if(KeyPressed&&Host){
			if(LastKey==109){
				BattleID=SetCurrentBattle(BattleID-1,&Prev,MNATION,BTTXT);
				KeyPressed=0;
				GSets.CGame.PL_INFO[HostID].MapStyle=BattleID;
			};
			if(LastKey==107){
				BattleID=SetCurrentBattle(BattleID+1,&Prev,MNATION,BTTXT);
				KeyPressed=0;
				GSets.CGame.PL_INFO[HostID].MapStyle=BattleID;
			};
		};
		if(GameInProgress)goto FinCLC;
		if(KeyPressed&&LastKey==13){
			if(CHATBOX->Active&&CHATMESSAGE[0]){
				ControlLingvo(CHATMESSAGE);
				SendChat(CHATMESSAGE,0);
				AddChatString(
					GetPlNameByID(MyDPID),
					CHATMESSAGE,250,
					&WhiteFont,ChatMess,ChatSender,NCHATS,MAXCHATS);
				AddChat=1;
				CHATMESSAGE[0]=0;
			};
			KeyPressed=0;
			LastKey=0;
		};
		if(CHATSTRING[0]){
			AddChatString(
					GetPlNameByID(CHATDPID),
					CHATSTRING,250,
					&WhiteFont,ChatMess,ChatSender,NCHATS,MAXCHATS);
			CHATSTRING[0]=0;
			AddChat=1;
		};
		NCLINES=0;
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
		DYL=CHSCR->SPos;
		
		SendPings();
		StdKeys();
		if(ItemChoose==mcmOk&&!OkBtn->Enabled)ItemChoose=-1;
		AnalyseMessages();
		if(GameInProgress)goto FinCLC;
		if(MyOldVers)GPPB->Hint=OLDV;
		MENU.MarkToDraw();
		MENU.ProcessDialogs();
		if(GameInProgress)goto FinCLC;
		char ccc[128];
		if(PSUMM.NPL&&PSUMM.PSET[0].NPings){
			int png=GetMaxRealPing();
			
			//png=GetAveragePing();
			//MaxPingTime=png;
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
			if(png<MinPing0)MaxPingTime=0;//FIXIT!!!!!!!!!!!
			if(png>=MinPing0&&png<300)MaxPingTime=300;
			if(MaxPingTime)MaxPingTime+=500;
			_str cc2;
			CreateDiffStr(cc2);		
			sprintf(ccc,"max ping: %d  (%s)",png,cc2.pchar());
			ShowString(10,768-24,ccc,&SmallYellowFont);
		};
		
		MENU.RefreshView();
		if(GameInProgress)goto FinCLC;
		
		ReadyFlow[0]->Visible=0;
		ReadyFlow[1]->Visible=0;
		if(Host){
			if(HostID!=-1){
				GSets.CGame.PL_INFO[HostID].MapName[0]=0;
				strcpy(GSets.CGame.PL_INFO[HostID].MapName,WARS.Battles[BattleID].Map);
				strcpy(GSets.CGame.cgi_CurrentMap,GSets.CGame.PL_INFO[HostID].MapName);
				MNATION[HostID]->Enabled=1;
				MNATION[1-HostID]->Enabled=0;
				int PPP;
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
		}else{
			MNATION[0]->Enabled=0;
			MNATION[1]->Enabled=0;
			ReadyFlow[0]->Visible=0;
			ReadyFlow[1]->Visible=0;

		};
		if(HostID!=-1){
			MNATION[1-HostID]->CurLine=1-MNATION[HostID]->CurLine;
			for(int i=0;i<2;i++){
				ColorBack[i]->Nation=MNATION[i]->CurLine;
				Flags[i]->Nation=MNATION[i]->CurLine;
			};
		};
		MaxVers=0;
		for(i=0;i<NPlayers;i++){
			if(GSets.CGame.PL_INFO[i].Version>MaxVers)MaxVers=GSets.CGame.PL_INFO[i].Version;
		};
		for(i=0;i<2;i++){
			if(i<NPlayers){
				if(GSets.CGame.PL_INFO[i].PlayerID==MyDPID){
					GSets.CGame.PL_INFO[i].NationID=MNATION[i]->CurLine;
					GSets.CGame.PL_INFO[i].ColorID=ColorBack[i]->Nation;
					bool ch=memcmp(&MYPINF,GSets.CGame.PL_INFO+i,sizeof MYPINF)!=0;
					if(ch||GetTickCount()-PREVSD>3000){
						SETPLAYERDATA(MyDPID,(void*)&GSets.CGame.PL_INFO[i].NationID,sizeof(PlayerInfo)-36,ch);
						memcpy(&MYPINF,GSets.CGame.PL_INFO+i,sizeof MYPINF);
						PREVSD=GetTickCount();
					};
					DPNAME dpName;
					ZeroMemory(&dpName, sizeof(DPNAME));
					dpName.dwSize = sizeof(DPNAME);
					dpName.lpszShortNameA = MNAME[i]->Str;
					dpName.lpszLongNameA = NULL;
					if(strcmp(dpName.lpszShortNameA,oldname)){
						SETPLAYERNAME(&dpName,strcmp(dpName.lpszShortNameA,oldname)!=0);
						strcpy(oldname,dpName.lpszShortNameA);
					};
					//MNATION[i]->Enabled=true;
					//MGRP[i]->Enabled=true;
						NameBack[i]->Visible=1;
					MNAME[i]->Visible=1;
					MNAME[i]->Enabled=0;//1;
					NationBack[i]->Visible=1;
					NationBack[i]->Enabled=1;
					CD_Back[i]->Visible=1;
					//MNATION[i]->Enabled=1;
					ColorBack[i]->Visible=1;
					ColorBack[i]->Enabled=1;
					ReadyBack[i]->Visible=1;
					MyOldVers=GSets.CGame.PL_INFO[i].Version!=MaxVers;
				}else{
					NationBack[i]->Visible=1;
					NationBack[i]->Enabled=1;
					MNAME[i]->Visible=1;
					MNAME[i]->Enabled=0;
					MNAME[i]->Active=0;
					NameBack[i]->Visible=0;
					//MNATION[i]->Enabled=0;
					CD_Back[i]->Visible=0;
					ColorBack[i]->Visible=0;
					ColorBack[i]->Enabled=0;
					ReadyBack[i]->Visible=0;
					//MGRP[i]->Enabled=false;
				};
				if(MNAME[i]->Active)NameBack[i]->SpriteID=12+(i%3);
				else NameBack[i]->SpriteID=9+(i%3);
				Flags[i]->SpriteID=(i*229+GetRealTime()/30)%45;
				Flags[i]->Nation=ColorBack[i]->Nation;
				MNATION[i]->Visible=true;
				//MCOLOR[i]->Visible=true;
				MREADY[i]->SpriteID=26+(GSets.CGame.PL_INFO[i].Ready!=0);
				if(i!=NOMREADY){
					MREADY[i]->Visible=true;
					MREADY[i]->Enabled=true;
				};
				Flags[i]->Visible=1;
				CD_Present[i]->Visible=1;
				//MREADY[i]->Visible=1;
				MNATION[i]->Visible=1;
				//ReadyBack[i]->Visible=0;
				//MGRP[i]->Visible=true;
				int v=GSets.CGame.PL_INFO[i].Version;
				char VERS[128];
				if(v>255){
					sprintf(VERS,"M%d",v>>8);
				}else{
					sprintf(VERS,"%d.%d%d",v/100,(v/10)%10,v%10);
				};
				strcpy(VersText[i]->Message,VERS);
				strcpy(VersTextShadow[i]->Message,VERS);
				if(v!=MaxVers&&(GetTickCount()/300)&1){
					VersText[i]->ActiveFont=&SmallRedFont;
					VersText[i]->PassiveFont=&SmallRedFont;
				}else{
					VersText[i]->ActiveFont=&SmallYellowFont;
					VersText[i]->PassiveFont=&SmallYellowFont;
				};
				VersText[i]->Visible=1;
				VersTextShadow[i]->Visible=1;
				PresOldVers=v!=MaxVers;
			}else{
				NameBack[i]->Visible=0;
				NationBack[i]->Visible=0;
				ColorBack[i]->Visible=0;
				Flags[i]->Visible=0;
				CD_Back[i]->Visible=0;
				CD_Present[i]->Visible=0;
				ReadyBack[i]->Visible=0;
				MNAME[i]->Visible=0;
				MNAME[i]->Enabled=0;
				MNAME[i]->Active=0;
				MREADY[i]->Visible=0;
				MNATION[i]->Visible=0;
				//MGRP[i]->Visible=false;
				VersText[i]->Visible=0;
				VersTextShadow[i]->Visible=0;
			};
		};
		if(!PIEnumeratePlayers(GSets.CGame.PL_INFO,1)){
			ItemChoose=mcmCancel;
		};
		if(NPlayers>2)NPlayers=2;
		if(!Host){
			int HostID=-1;
			int MyID=-1;
			for(int i=0;i<NPlayers;i++){
				if(GSets.CGame.PL_INFO[i].Host)HostID=i;
				if(GSets.CGame.PL_INFO[i].PlayerID==MyDPID)MyID=i;
			};
			if(HostID!=-1&&MyID!=-1){
				if(GSets.CGame.PL_INFO[MyID].MapStyle!=GSets.CGame.PL_INFO[HostID].MapStyle){
					GSets.CGame.PL_INFO[MyID].MapStyle=GSets.CGame.PL_INFO[HostID].MapStyle;
					BattleID=SetCurrentBattle(GSets.CGame.PL_INFO[MyID].MapStyle,&Prev,MNATION,BTTXT);
					MYPINF.MapStyle=GSets.CGame.PL_INFO[MyID].MapStyle;
				};
				strcpy(GSets.CGame.PL_INFO[MyID].MapName,GSets.CGame.PL_INFO[HostID].MapName);
				//GSets.CGame.PL_INFO[MyID].MapStyle=0;
				GSets.CGame.PL_INFO[MyID].HillType=0;
				GSets.CGame.PL_INFO[MyID].StartRes=0;
				GSets.CGame.PL_INFO[MyID].ResOnMap=0;
				strcpy(GSets.CGame.cgi_CurrentMap,GSets.CGame.PL_INFO[HostID].MapName);
			};
		};
		for(i=0;i<NPlayers;i++){
			MNATION[i]->CurLine=GSets.CGame.PL_INFO[i].NationID;
			ColorBack[i]->Nation=GSets.CGame.PL_INFO[i].ColorID;
			if(GSets.CGame.PL_INFO[i].PlayerID==MyDPID&&ItemChoose==mcmOk&&!Host){
				GSets.CGame.PL_INFO[i].Ready=1;
			};
		};
		GMREADY=true;
		for(i=0;i<NPlayers;i++){
			if(GSets.CGame.PL_INFO[i].PlayerID!=MyDPID&&!GSets.CGame.PL_INFO[i].Ready)GMREADY=false;
		};
		MSS1=0;
		for(i=0;i<NPlayers;i++){
			byte MSS2=1<<GSets.CGame.PL_INFO[i].ColorID;
			if(MSS1&MSS2)GMREADY=false;
			MSS1|=MSS2;
			if(!GSets.CGame.PL_INFO[i].Ready)GMREADY=false;
		};
		if(Host){
			if(NPlayers<2)GMREADY=false;
			if(!CheckPingsReady())GMREADY=false;
			if(PresOldVers)GMREADY=0;
			OkBtn->Enabled=GMREADY;
			OkBtn->Visible=GMREADY;
			if(TPEN.MyRoom&&TPEN.MyRoom->RoomConnected){
				TPEN.CreateMyRoomInfoString(TPEN.HostMessage);
			};
		}else{
			OkBtn->Enabled=!MyOldVers;
			OkBtn->Visible=!MyOldVers;
		};
		//checking readyness
		if(ItemChoose==mcmOk&&!Host)ItemChoose=-1;
		if(pp){
			SlowLoadPalette("2\\agew_1.pal");
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
	}while(ItemChoose==-1&&PlayerMenuMode==1);
	GSets.CGame.PL_INFO[0].GameTime=0;
	GSets.CGame.PL_INFO[0].VictCond=0;
	extern int StartGTime;
	extern int StartVCond;
	void ClearCompstart();
	ClearCompstart();
	StartGTime=0;
	StartVCond=0;
	if(NCHATS){
		for(int i=0;i<NCHATS;i++){
			free(ChatMess[i]);
			free(ChatSender[i]);
		};
		free(ChatMess);
		free(ChatSender);
	};
	return (ItemChoose==mcmOk)||PlayerMenuMode!=1;//ItemChoose==mcmOk;
};