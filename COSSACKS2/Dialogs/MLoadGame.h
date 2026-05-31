void AC_InstallName(ListBox* LB,ListBox* DT,WIN32_FIND_DATA* FD,char* StartDir){
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
				
				fnames[NNames]=znew(char,strlen(CCC)+1);
				strcpy(fnames[NNames],CCC);				

				strcpy(CCC,FD->cFileName);
				LB->AddItem(CCC,NNames);				
				
				SYSTEMTIME ft;
				FILETIME FT;
				FileTimeToLocalFileTime(&FD->ftLastWriteTime,&FT);
				FileTimeToSystemTime(&FT,&ft);
				char dt[256];
				sprintf(dt,"%02d:%02d:%02d   %02d.%02d.%d",ft.wHour,ft.wMinute,ft.wSecond,ft.wDay,ft.wMonth,ft.wYear);
				//for(i=0; i<NNames; i++)
				
				if(DT) DT->AddItem(dt,NNames);
				
				//DT->AddItem(dt,0);
				
				NNames++;
			}else RClose(ff1);
		}else RClose(ff1);
	};
};

void CreateGamesList(ListBox* LB, ListBox* DT){	
	WIN32_FIND_DATA FD;
	const NList = 100;
	WIN32_FIND_DATA FDL[NList];
	int i;
	HANDLE HF=FindFirstFile("Save\\*.sav",&FD);
	if(HF!=INVALID_HANDLE_VALUE){
		i=0;
		do{
			for(int j=0;j<i;j++) if(CompareFileTime(&FD.ftLastWriteTime,&FDL[j].ftLastWriteTime)==-1){
				memcpy(&FDL[i],&FDL[j],sizeof FD);
				memcpy(&FDL[j],&FD,sizeof FD);
				break;
			}
			if(i==j){
				memcpy(&FDL[j],&FD,sizeof FD);
			}			
			i++;
		}while(FindNextFile(HF,&FD)&&i<NList);
		int n=i;
		for(i=n-1;i>=0;i--){ // Po vozrostaniu
			AC_InstallName(LB,DT,FDL+i,"Save\\");
		}
	};
};
void CreateRecList(ListBox* LB, ListBox* DT){
	WIN32_FIND_DATA FD;
	HANDLE HF=FindFirstFile("*.rec",&FD);
	if(HF!=INVALID_HANDLE_VALUE){
		AC_InstallName(LB,DT,&FD,"");
		while(FindNextFile(HF,&FD))AC_InstallName(LB,DT,&FD,"");
		FindClose(HF);
	};
	HF=FindFirstFile("Autorecord\\*.rec",&FD);
	if(HF!=INVALID_HANDLE_VALUE){
		AC_InstallName(LB,DT,&FD,"Autorecord\\");
		while(FindNextFile(HF,&FD)) AC_InstallName(LB,DT,&FD,"Autorecord\\");
		FindClose(HF);
	};
};

// Old function

void CreateGamesList(ListBox* LB){
	WIN32_FIND_DATA FD;
	ClearNames();
	HANDLE HF=FindFirstFile("Save\\*.sav",&FD);
	if(HF!=INVALID_HANDLE_VALUE){
		do{
			AC_InstallName(LB,NULL,&FD,"Save\\");
		}while(FindNextFile(HF,&FD));
	};
};
void CreateRecList(ListBox* LB){
	WIN32_FIND_DATA FD;
	ClearNames();
	HANDLE HF=FindFirstFile("Save\\*.rec",&FD);
	if(HF!=INVALID_HANDLE_VALUE){
		InstallName(LB,&FD,"");
		while(FindNextFile(HF,&FD))InstallName(LB,&FD,"");
		FindClose(HF);
	};
	HF=FindFirstFile("Autorecord\\*.rec",&FD);
	if(HF!=INVALID_HANDLE_VALUE){
		InstallName(LB,&FD,"Autorecord\\");
		while(FindNextFile(HF,&FD))InstallName(LB,&FD,"Autorecord\\");
		FindClose(HF);
	};
};
void processMLoadGame(){	
	LoadFog(1);	
	//SlowUnLoadPalette("1\\agew_1.pal");

	// Controls
	LocalGP BTN("Interf2\\Buttons");
	LocalGP VSC("Interf3\\elements\\scroll");
	LocalGP BTNS("Interf2\\LoadGame");

	int Set0[30] = {4,4,4,4,4, 5,5,5,5,5, 5,5,5,5,5,
				    4,4,4,4,4, 5,5,5,5,5, 5,5,5,5,5};

	// Fonts
	RLCFont* FScreen = &fon40y5;
	RLCFont* FWindow = &fon16y5;
	RLCFont* FButton = &fon18y5;
	RLCFont* FWhite = &fon18w;
	RLCFont* FYellow = &fon18y3;
	RLCFont* FHint = &fon13y2;
	RLCFont* FASwitch = &fon16y1;
	RLCFont* FPSwitch = &fon16y3;

	//LocalGP HFONT("interf3\\Fonts\\FontC10");
	//RLCFont hfnt(HFONT.GPID);
	//hfnt.SetWhiteColor();
	
	LoadPointer("cur_mn.rlc");
	SQPicture MnPanel("Interf2\\Background_Load_Game.bmp");
	DialogsSystem MMenu(0,0);
	
	// Hint
	MMenu.HintFont=FHint;
	MMenu.HintX=513;
	MMenu.HintY=745;

	//int dx=((RealLx-GPS.GetGPWidth(BTNS.GPID,1))>>1)-85-125;
	//int dy=(RealLy-GPS.GetGPHeight(BTNS.GPID,1))>>1;
	Picture* PIC=MMenu.addPicture(NULL,0,0,&MnPanel,&MnPanel,&MnPanel);
	//MMenu.addGPPicture(NULL,dx,dy,BTNS.GPID,0);
	//MMenu.addGPPicture(NULL,dx,dy,BTNS.GPID,1);
	
	VScrollBar* VS=MMenu.addNewGP_VScrollBar(NULL,901,240+26,264-26,1000,0,VSC.GPID,0);
	VScrollBar* RVS=MMenu.addNewGP_VScrollBar(NULL,901,240+26,264-26,1000,0,VSC.GPID,0);

	VS->ScrDy=14;
	RVS->ScrDy=14;
	RVS->Visible=0;
	
	// Load game list
	MMenu.addClipper(0,0,709,RealLy);
	ListBox* LB=MMenu.addGP_ListBox(NULL,446,256+26,8,BTNS.GPID,0,26,FWhite,FYellow,VS);
	MMenu.addClipper(0,0,RealLx,RealLy);
	ListBox* DT=MMenu.addGP_ListBox(NULL,714,256+26,8,BTNS.GPID,-3,26,FWhite,FYellow,VS); // Data / Time

	// Load demorec list
	MMenu.addClipper(0,0,709,RealLy);
	ListBox* RName=MMenu.addGP_ListBox(NULL,446,256+26,8,BTNS.GPID,0,26,FWhite,FYellow,RVS);
	MMenu.addClipper(0,0,RealLx,RealLy);
	ListBox* RDT=MMenu.addGP_ListBox(NULL,714,256+26,8,BTNS.GPID,-3,26,FWhite,FYellow,RVS); // Data / Time

	LB->FontDx -= 16;
	LB->FontDy -= 3;
	LB->Active=true;

	DT->FontDx -= 16;
	DT->FontDy -= 3;

	RName->FontDx -= 16;
	RName->FontDy -= 3;
	RName->Active=true;

	RDT->FontDx -= 16;
	RDT->FontDy -= 3;

	ClearNames();
	CreateGamesList(LB,DT);
	CreateRecList(RName,RDT);

	// Page buttons
	int PageX =506;
	int PageY =244;
	int PageW =165;

	UniversalButton* Page0=MMenu.addUniversalButton(PageX,PageY,PageW,GetTextByID("INTF_LOAD_P0"),BTN.GPID,Set0,0,1,0,FPSwitch,FPSwitch,FASwitch);
	UniversalButton* Page1=MMenu.addUniversalButton(PageX+PageW+21,PageY,PageW,GetTextByID("INTF_LOAD_P1"),BTN.GPID,Set0,0,0,0,FPSwitch,FPSwitch,FASwitch);

	Page0->Hint=GetTextByID("INTF5_HINT13");
	Page1->Hint=GetTextByID("INTF5_HINT14");

	Page0->FontDy -= 1;
	Page1->FontDy -= 1;

	// Button
	GP_TextButton* OkBtn=MMenu.addGP_TextButton(NULL,472,577,GetTextByID("INTF_LOAD"),BTN.GPID,0,FButton,FButton);
	OkBtn->UserParam=mcmOk;
	OkBtn->OnUserClick=&MMItemChoose;
	
	GP_TextButton* CancelBtn=MMenu.addGP_TextButton(NULL,704,577,GetTextByID("INTF_CANCEL"),BTN.GPID,0,FButton,FButton);
	CancelBtn->UserParam=mcmCancel;
	CancelBtn->OnUserClick=&MMItemChoose;

	OkBtn->FontDy -= 1;
	CancelBtn->FontDy -= 1;
	
	char* OkSave=GetTextByID("INTF5_HINT11");
	char* OkReplay=GetTextByID("INTF5_HINT12");

	CancelBtn->Hint=GetTextByID("SOCANCEL");

	// Text
	TextButton* Scr=MMenu.addTextButton(NULL,512,18,GetTextByID("INTF_LOAD_TITLE0"),FScreen,FScreen,FScreen,1);	
	TextButton* Win0=MMenu.addTextButton(NULL,680,203,GetTextByID("INTF_LOAD_TITLE1"),FWindow,FWindow,FWindow,1);		
	TextButton* SelTxt=MMenu.addTextButton(NULL,446,528,GetTextByID("INTF_LOAD_CG"),FYellow,FYellow,FYellow,0);	
	
	TextButton* CurSave=MMenu.addTextButton(NULL,602,528,"                                                            ",FWhite,FWhite,FWhite,0);	

	ItemChoose=-1;
	int pp=1;
	int LastCTime=GetRealTime();	
	KeyPressed=0;
	SetMyNation(0);

	ListBox* ALB=NULL;

	StartMixing();	
	do{
		MMenu.MarkToDraw();
		ProcessMessages();
		StdKeys();		
		if(Page0->State){
			OkBtn->Hint=OkSave;
			ALB = LB;
			DT->CurItem=LB->CurItem;
			LB->Enabled=1;
			LB->Visible=1;
			DT->Visible=1;
			VS->Visible=1;
			RName->Enabled=0;
			RName->Visible=0;
			RDT->Visible=0;
			RVS->Visible=0;
		}else{
			OkBtn->Hint=OkReplay;
			ALB = RName;
			RDT->CurItem=RName->CurItem;
			LB->Enabled=0;
			LB->Visible=0;
			DT->Visible=0;
			VS->Visible=0;
			RName->Enabled=1;
			RName->Visible=1;
			RDT->Visible=1;
			RVS->Visible=1;
		}
		if(ALB->CurItem==-1||!ALB->NItems){
			OkBtn->Enabled=false;
			if(ItemChoose==mcmOk)ItemChoose=-1;
			GameName[0]=0;
		}else{
			OkBtn->Enabled=true;
			strcpy(GameName,ALB->GetItem(ALB->CurItem)->Message);
			//CurSave->Message=ALB->GetItem(ALB->CurItem)->Message;
		}
		strcpy(CurSave->Message,GameName);
		
		MMenu.ProcessDialogs();
		ProcessMixing();
		MMenu.RefreshView();
		
		/*
		if(ItemChoose==1||(UpBtn->MouseOver&&realLpressed&&GetRealTime()-LastCTime>180)){//up
			if(LB->FLItem>0)LB->FLItem--;
			LastCTime=GetRealTime();
			if(ItemChoose!=-1)LastCTime+=600;
			ItemChoose=-1;
			Lpressed=false;
		};
		if(ItemChoose==2||(DnBtn->MouseOver&&realLpressed&&GetRealTime()-LastCTime>180)){//dn
			if(LB->NItems-LB->FLItem>LB->ny)LB->FLItem++;
			LastCTime=GetRealTime();
			if(ItemChoose!=-1)LastCTime+=600;
			ItemChoose=-1;
			Lpressed=false;
		};
		*/
		if(pp){
//			SlowLoadPalette("1\\agew_1.pal");
			pp=0;
		};
	}while(ItemChoose==-1);
	if(ItemChoose==mcmOk&&ALB->CurItem!=-1){
		ShowLoading();
		PrepareToGame();
		char* LFN=fnames[ALB->GetItem(ALB->CurItem)->Param1];
		if(strstr(CurSave->Message,".rec")||strstr(CurSave->Message,".REC")){			
			RGAME.ReadStream(LFN); //CurSave->Message);
			if(RGAME.Size){
					PlayGameMode=2;
					//ItemChoose=43;
					//RetryVideo=1;
					KeyPressed=0;
					EditMapMode=0;
					ItemChoose=mcmSingle;
				};
		}else{
			DLL2(SFLB_LoadGame,LFN,1);
			HideFlags();
			ContinueGame=true;
			ItemChoose=mcmSingle;
		};
	}else{		
	};
//	SlowUnLoadPalette("1\\agew_1.pal");
	FreeNames();
};