int MMVar_Return;
void TempMM_Options(){
	// init
	static DialogsSystem* DS=NULL;
	if(!DS){
		xmlQuote xml;
		if(xml.ReadFromFile("dialogs\\v\\MM_Options.DialogsSystem.xml")){
			ErrorPager Error;
			DS=new DialogsSystem;
			DS->Load(xml,DS,&Error);
		}
	}
	if(DS){
		MMVar_Return=-1;
		do{
			ProcessMessages();
			DS->ProcessDialogs();
			DS->RefreshView();
		}while(MMVar_Return==-1);
	}
}
bool ProcessMenuOptions(){
	//TempMM_Options();
	//return true;

	extern int RequiredMsPerFrame;
	LoadFog(1);
//	LoadPalette("1\\agew_1.pal");
	
	// Controls
	LocalGP BTN("Interf2\\Buttons");
	LocalGP CMB("Interf2\\Combo");
	LocalGP SCR("Interf3\\elements\\scroll");
	LocalGP CHK("Interf2\\Checker");
	LocalGP TB2("Interf2\\Textbox2");

	LocalGP BTNS("Interf2\\Options");
	
	// Fonts
	RLCFont* FScreen = &fon40y5;
	RLCFont* FWindow = &fon16y5;
	RLCFont* FButton = &fon18y5;
	RLCFont* FWhite = &fon18w;
	RLCFont* FYellow = &fon18y3;
	RLCFont* FHint = &fon13y2;

	// Main menu	
	DialogsSystem MMenu(0,0);	
	
	// Hint
	MMenu.HintFont=FHint;
	MMenu.HintX=513;
	MMenu.HintY=745;
	
	//int dx=((RealLx-GPS.GetGPWidth(BTNS.GPID,1))>>1)-85-125;
	//int dy=(RealLy-GPS.GetGPHeight(BTNS.GPID,1))>>1;
	//MMenu.addGPPicture(NULL,dx,dy,BTNS.GPID,0);
	//MMenu.addGPPicture(NULL,dx,dy,BTNS.GPID,1);
	
	// Background	
	SQPicture MnPanel("Interf3\\MainMenu\\sample_background.bmp");
	Picture* PIC=MMenu.addPicture(NULL,0,0,&MnPanel,&MnPanel,&MnPanel);
	
	// Text
	TextButton* T0=MMenu.addTextButton(NULL,512,18,GetTextByID("INTF_OPT_TITLE0"),FScreen,FScreen,FScreen,1);
	TextButton* T1=MMenu.addTextButton(NULL,680,217,GetTextByID("INTF_OPT_TITLE1"),FWindow,FWindow,FWindow,1);	
	
	int SecX = 443;
	int SecDx = 27;
	int SecY = 258;
	TextButton* VO=MMenu.addTextButton(NULL,SecX,SecY,GetTextByID("INTF_OPT_VO"),FWhite,FWhite,FWhite,0);
	TextButton* AO=MMenu.addTextButton(NULL,SecX,SecY+SecDx*2,GetTextByID("INTF_OPT_AO"),FWhite,FWhite,FWhite,0);
	TextButton* GO=MMenu.addTextButton(NULL,SecX,SecY+SecDx*6,GetTextByID("INTF_OPT_GO"),FWhite,FWhite,FWhite,0);
	
	int SubSecX = SecX+10;
	TextButton* VO0=MMenu.addTextButton(NULL,SubSecX,SecY+SecDx*1,GetTextByID("INTF_OPT_VO_0"),FYellow,FYellow,FYellow,0);
	TextButton* AO0=MMenu.addTextButton(NULL,SubSecX,SecY+SecDx*3,GetTextByID("INTF_OPT_AO_0"),FYellow,FYellow,FYellow,0);
	TextButton* AO1=MMenu.addTextButton(NULL,SubSecX,SecY+SecDx*4,GetTextByID("INTF_OPT_AO_1"),FYellow,FYellow,FYellow,0);
	TextButton* AO2=MMenu.addTextButton(NULL,SubSecX,SecY+SecDx*5,GetTextByID("INTF_OPT_AO_2"),FYellow,FYellow,FYellow,0);
	TextButton* GO0=MMenu.addTextButton(NULL,SubSecX,SecY+SecDx*7,GetTextByID("INTF_OPT_GO_0"),FYellow,FYellow,FYellow,0);
	TextButton* GO1=MMenu.addTextButton(NULL,SubSecX,SecY+SecDx*8,GetTextByID("INTF_OPT_GO_1"),FYellow,FYellow,FYellow,0);
	TextButton* GO2=MMenu.addTextButton(NULL,SubSecX,SecY+SecDx*9,GetTextByID("INTF_OPT_GO_2"),FYellow,FYellow,FYellow,0);
	TextButton* GO3=MMenu.addTextButton(NULL,SubSecX,SecY+SecDx*10,GetTextByID("INTF_OPT_GO_3"),FYellow,FYellow,FYellow,0);
	
	// Buttons
	GP_TextButton* OkBtn=MMenu.addGP_TextButton(NULL,472,577,GetTextByID("INTF_ACCEPT"),BTN.GPID,0,FButton,FButton);
	GP_TextButton* ExitBtn=MMenu.addGP_TextButton(NULL,704,577,GetTextByID("INTF_CANCEL"),BTN.GPID,0,FButton,FButton);

	OkBtn->UserParam=mcmOk;
	OkBtn->OnUserClick=&MMItemChoose;
	OkBtn->Hint=GetTextByID("INTF5_HINT0");
	
	ExitBtn->UserParam=mcmExit;
	ExitBtn->OnUserClick=&MMItemChoose;
	ExitBtn->Hint=GetTextByID("INTF5_HINT1");

	OkBtn->FontDy -= 1;
	ExitBtn->FontDy -= 1;

	//
	if(FPSTime>100)FPSTime=100;
	if(FPSTime<0)FPSTime=0;
	//game speed (#3)
	if(GSets.SVOpt.ScrollSpeed<2)GSets.SVOpt.ScrollSpeed=2;
	int X0=619+2;
	int LX=290+2;
	int DY=10;

	VScrollBar* SVL=MMenu.addGP_ScrollBarL(NULL,X0,366+DY,100,GSets.SVOpt.SoundVolume,SCR.GPID,0,LX,12,0,0);
	VScrollBar* MVL=MMenu.addGP_ScrollBarL(NULL,X0,392+DY,100,GSets.SVOpt.MusicVolume,SCR.GPID,0,LX,12,0,0); //music volume (#2)
	VScrollBar* SSP=MMenu.addGP_ScrollBarL(NULL,X0,474+DY,8,GSets.SVOpt.ScrollSpeed-2,SCR.GPID,0,LX,12,0,0); //sound volume (#1)		
	VScrollBar* HB=MMenu.addGP_ScrollBarL(NULL,X0,500+DY,100,120-GSets.SVOpt.RequiredMsPerFrame,SCR.GPID,0,LX,12,0,0);
	
	SVL->Hint=GetTextByID("INTF5_HINT4");
	MVL->Hint=GetTextByID("INTF5_HINT5");
	SSP->Hint=GetTextByID("INTF5_HINT7");
	HB->Hint=GetTextByID("INTF5_HINT8");

	ComboBox* VMode=MMenu.addGP_ComboBox(NULL,619,284,CMB.GPID,0,9,0,FWhite,FYellow,NULL);
	ComboBox* FMode=MMenu.addGP_ComboBox(NULL,619,448,CMB.GPID,0,9,0,FWhite,FYellow,NULL);
	ComboBox* MUS=MMenu.addGP_ComboBox(NULL,619,340,CMB.GPID,0,9,0,FWhite,FYellow,NULL);

	VMode->Hint=GetTextByID("INTF5_HINT2");
	FMode->Hint=GetTextByID("INTF5_HINT6");
	MUS->Hint=GetTextByID("INTF5_HINT3");

	GSets.SVOpt.MusicVolume=GetCDVolume();

	//VMode->CurLine=100;
	VMode->Center-=5;
	VMode->FontDy-=2;
	VMode->OneDy-=1;
	//VMode->FontDx-=6;

	GP_Button* REC=MMenu.addGP_Button(NULL,619,525,CHK.GPID,1,0);
	REC->OnUserClick=&MMItemChoose;
	REC->UserParam=77;
	
	int TB2Set[30] = {5,6,7,8,9, 5,6,7,8,9, 5,6,7,8,9, 
				      5,6,7,8,9, 5,6,7,8,9, 5,6,7,8,9};
	UniversalButton* EDITPAN=MMenu.addUniversalButton(649,525,267,"",TB2.GPID,TB2Set,0,0,0,FYellow,FYellow,FYellow);
	
	EDITPAN->Hint=GetTextByID("INTF5_HINT9");
	EDITPAN->Enabled=0;

	InputBox* IBREC=MMenu.addInputBox(NULL,659,524,RECFILE,120,248,26,FYellow,FWhite);
	IBREC->Hint=GetTextByID("INTF5_HINT10");

	FMode->AddLine(GetTextByID("SLOWMOD"));
	FMode->AddLine(GetTextByID("FASTMOD"));
	FMode->Center-=5;
	FMode->FontDy-=2;
	FMode->OneDy-=1;
	
	
	MUS->AddLine(GetTextByID("NOCD"));
	//MUS->AddLine(GetTextByID("NATMUS"));
	MUS->AddLine(GetTextByID("RNDMUS"));
	//MUS->CurLine=100;
	MUS->Center-=5;
	MUS->FontDy-=2;
	MUS->OneDy-=1;
	//MUS->FontDx-=6;
	
	for(int i=0;i<NModes;i++){
		char cc[30];
		sprintf(cc,"%dx%d",ModeLX[i],ModeLY[i]);
		VMode->AddLine(cc);
		if(GSets.SVOpt.ScreenSizeX==ModeLX[i]&&GSets.SVOpt.ScreenSizeY==ModeLY[i])VMode->CurLine=i;
	};
	if(VMode->CurLine==100)VMode->CurLine=0;
	bool RECSTATE=RecordMode;
	FMode->CurLine=exFMode;
	MUS->CurLine=GSets.SVOpt.MusicPlayMode!=0;
	ItemChoose=-1;
	int pp=1;
	KeyPressed=0;
	int PrevPlayMode=MUS->CurLine!=0;
	int ppp=GSets.SVOpt.MusicPlayMode;
	
	StartMixing();
	do{
		if(RECSTATE){
			EDITPAN->Visible = 0;
			//EDITPAN->Enabled=1;
			IBREC->Enabled=1;
			REC->ActiveFrame=1;
			REC->PassiveFrame=1;
			IBREC->Active=1;
		}else{
			EDITPAN->Visible = 1;
			//EDITPAN->Enabled=0;
			IBREC->Enabled=0;
			REC->ActiveFrame=0;
			REC->PassiveFrame=0;
			IBREC->Active=0;
		};		
		//P1->Visible=P1->MouseOver;
		//P2->Visible=P2->MouseOver;
		//P3->Visible=P3->MouseOver;
		//P4->Visible=P4->MouseOver;
		ProcessMessages();
		StdKeys();
		MMenu.MarkToDraw();
		MMenu.ProcessDialogs();
		ProcessMixing();
		MMenu.RefreshView();
		if(ItemChoose==77){
			RECSTATE=!RECSTATE;
			ItemChoose=-1;
		};
		if(pp){
			//SlowLoadPalette("1\\agew_1.pal");
			pp=0;
		};
		SetCDVolume(MVL->SPos);
		if(PrevPlayMode!=(MUS->CurLine!=0)){
			GSets.SVOpt.MusicPlayMode=MUS->CurLine!=0;
			if(MUS->CurLine)PlayRandomTrack();
			else StopPlayCD();
			PrevPlayMode=MUS->CurLine!=0;
		};
	}while(ItemChoose==-1);
	
	if(ItemChoose==mcmOk){
		GSets.SVOpt.RequiredMsPerFrame=120-HB->SPos;
		GSets.SVOpt.ScrollSpeed=SSP->SPos+2;
		GSets.SVOpt.ScreenSizeX=ModeLX[VMode->CurLine];
		GSets.SVOpt.ScreenSizeY=ModeLY[VMode->CurLine];
		GSets.SVOpt.SoundVolume=SVL->SPos;
		GSets.SVOpt.MusicVolume=MVL->SPos;
		GSets.SVOpt.MusicPlayMode=MUS->CurLine!=0;
		exFMode=FMode->CurLine;
		RecordMode=RECSTATE;
	}else{
		GSets.SVOpt.MusicPlayMode=ppp!=0;
		if((GSets.SVOpt.MusicPlayMode!=0)!=PrevPlayMode){
			if(GSets.SVOpt.MusicPlayMode)PlayRandomTrack();
			else StopPlayCD();
		};
	};
//	SlowUnLoadPalette("1\\agew_1.pal");
	return false;
};

