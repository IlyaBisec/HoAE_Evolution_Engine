bool ProcessGameOptions(){
	// Graphics
	LocalGP BTNS("Interf2\\G_Load_Game");	
	LocalGP MGP("Interf2\\Options_for_Game");

	LocalGP CMB("Interf3\\elements\\Combo");
	LocalGP SCR("Interf3\\elements\\scroll");
	
	LocalGP HFONT("interf3\\Fonts\\FontC10");
	RLCFont hfnt(HFONT.GPID);
	hfnt.SetWhiteColor();
	LoadPointer("cur_mn.rlc");
	//SQPicture MnPanel("Interface\\Background_options.bmp");
	int dx=-63+6+((RealLx-484)>>1);
	int dy=-166+((RealLy-425)>>1);
	DialogsSystem MMenu(dx,dy);
	MMenu.HintFont=&hfnt;
	MMenu.HintY=701;
	MMenu.HintX=18;
	
	// Fonts
	RLCFont* FWin = &fon16y5;
	RLCFont* FABut = &fon18y5;
	RLCFont* FPBut = &fon18y5;
	RLCFont* FText = &fon18y3;
	RLCFont* FWhite = &fon18w;
	RLCFont* FYellow = &fon18y3;

	//int Dx=((RealLx-GPS.GetGPWidth(BTNS.GPID,1))>>1)-85-125;
	//int Dy=(RealLy-GPS.GetGPHeight(BTNS.GPID,1))>>1;	
	//MMenu.addGPPicture(NULL,Dx,Dy,MGP.GPID,0);
	//MMenu.addGPPicture(NULL,dx,dy,BTNS.GPID,1);

	if(FPSTime>100)FPSTime=100;
	if(FPSTime<0)FPSTime=0;
	//game speed (#3)
	if(GSets.SVOpt.ScrollSpeed<2)GSets.SVOpt.ScrollSpeed=2;

	int X = -60;
	int Y = -40;

	GPS.ShowGP(X+dx+63,Y+dy+166,MGP.GPID,0,0);
	GPPicture* PIC=MMenu.addGPPicture(NULL,X+63,Y+166,MGP.GPID,1);	
	
	int SecX = X+443-317;//150;
	int SecDx = 27;
	int SecY = Y+258-15;//244;
	TextButton* VO=MMenu.addTextButton(NULL,SecX,SecY,GetTextByID("INTF_OPT_VO"),FWhite,FWhite,FWhite,0);
	TextButton* AO=MMenu.addTextButton(NULL,SecX,SecY+SecDx*2,GetTextByID("INTF_OPT_AO"),FWhite,FWhite,FWhite,0);
	TextButton* GO=MMenu.addTextButton(NULL,SecX,SecY+SecDx*6,GetTextByID("INTF_OPT_GO"),FWhite,FWhite,FWhite,0);
	
	int DY=27;
	int SubSecX = SecX+10;
	TextButton* VO0=MMenu.addTextButton(NULL,SubSecX,SecY+SecDx*1,GetTextByID("INTF_OPT_VO_0"),FYellow,FYellow,FYellow,0);
	TextButton* AO0=MMenu.addTextButton(NULL,SubSecX,SecY+SecDx*3,GetTextByID("INTF_OPT_AO_0"),FYellow,FYellow,FYellow,0);
	TextButton* AO1=MMenu.addTextButton(NULL,SubSecX,SecY+SecDx*4,GetTextByID("INTF_OPT_AO_1"),FYellow,FYellow,FYellow,0);
	TextButton* AO2=MMenu.addTextButton(NULL,SubSecX,SecY+SecDx*5,GetTextByID("INTF_OPT_AO_2"),FYellow,FYellow,FYellow,0);
	TextButton* GO0=MMenu.addTextButton(NULL,SubSecX,SecY+SecDx*7,GetTextByID("INTF_OPT_GO_0"),FYellow,FYellow,FYellow,0);
	TextButton* GO1=MMenu.addTextButton(NULL,SubSecX,SecY+SecDx*8,GetTextByID("INTF_OPT_GO_1"),FYellow,FYellow,FYellow,0);
	TextButton* GO2=MMenu.addTextButton(NULL,SubSecX,SecY+SecDx*9,GetTextByID("INTF_OPT_GO_2"),FYellow,FYellow,FYellow,0);

	int ComX = X+299;
	int ComY = Y+271;
	ComboBox* VMode=MMenu.addGP_ComboBox(NULL,ComX,ComY-2,CMB.GPID,0,9,0,&WhiteFont,&YellowFont,NULL);
	ComboBox* MUS=MMenu.addGP_ComboBox(NULL,ComX,ComY+2*DY/*323*/,CMB.GPID,0,9,0,&WhiteFont,&YellowFont,NULL);
	ComboBox* FMode=MMenu.addGP_ComboBox(NULL,ComX,ComY+6*DY/*432*/,CMB.GPID,0,9,0,&WhiteFont,&YellowFont,NULL);

	int X0=X+618-317;
	int LX=290+2;
	int ScrDy=8;
	
	VScrollBar* SVL=MMenu.addGP_ScrollBarL(NULL,X0,ScrDy+1+ComY+3*DY,100,GSets.SVOpt.SoundVolume,SCR.GPID,0,LX,12,0,0);
	VScrollBar* MVL=MMenu.addGP_ScrollBarL(NULL,X0,ScrDy+ComY+4*DY,100,GSets.SVOpt.MusicVolume,SCR.GPID,0,LX,12,0,0);	
	VScrollBar* SSP=MMenu.addGP_ScrollBarL(NULL,X0,ScrDy+1+ComY+7*DY,8,GSets.SVOpt.ScrollSpeed-2,SCR.GPID,0,LX,12,0,0);
	VScrollBar* HB=MMenu.addGP_ScrollBarL(NULL,X0,ScrDy+ComY+8*DY,100,100-FPSTime,SCR.GPID,0,LX,12,0,0);

	int ButY = Y+527;
	GP_TextButton* OkBtn=MMenu.addGP_TextButton(NULL,X+476-337,ButY,GetTextByID("INTF_ACCEPT"),BTNS.GPID,1,FABut, FPBut);
	GP_TextButton* ExitBtn=MMenu.addGP_TextButton(NULL,X+708-337,ButY,GetTextByID("INTF_CANCEL"),BTNS.GPID,1,FABut, FPBut);	

	OkBtn->FontDy += 3;
	ExitBtn->FontDy += 3;
	OkBtn->FontDx += 8;
	ExitBtn->FontDx += 8;

	// Text
	MMenu.addTextButton(NULL,X+680-317,Y+217-15,GetTextByID("INTF_OPT_TITLE1"),FWin,FWin,FWin,1);	

	VMode->Center-=5;
	VMode->FontDy-=2;
	VMode->OneDy-=1;
	MUS->Center-=5;
	MUS->FontDy-=2;
	MUS->OneDy-=1;
	FMode->Center-=5;
	FMode->FontDy-=2;
	FMode->OneDy-=1;

	GPPicture* P1=MMenu.addGPPicture(NULL,240,317+11,BTNS.GPID,15);
	GPPicture* P2=MMenu.addGPPicture(NULL,240,343+11,BTNS.GPID,16);
	GPPicture* P3=MMenu.addGPPicture(NULL,240,447+11,BTNS.GPID,17);
	GPPicture* P4=MMenu.addGPPicture(NULL,240,473+11,BTNS.GPID,18);
	P1->Visible=0;
	P2->Visible=0;
	P3->Visible=0;
	P4->Visible=0;
	
	OkBtn->UserParam=mcmOk;
	OkBtn->OnUserClick=&MMItemChoose;
	
	ExitBtn->UserParam=mcmExit;
	ExitBtn->OnUserClick=&MMItemChoose;
	//scroll speed (#4)
	
	//sound volume (#1)
	GSets.SVOpt.MusicVolume=GetCDVolume();
	
	//music volume (#2)
	
	VMode->CurLine=100;
	for(int i=0;i<NModes;i++){
		char cc[30];
		sprintf(cc,"%dx%d",ModeLX[i],ModeLY[i]);
		VMode->AddLine(cc);
		if(RealLx==ModeLX[i]&&RealLy==ModeLY[i])VMode->CurLine=i;
	};
	if(VMode->CurLine==100)VMode->CurLine=0;
	
	FMode->AddLine(SLOWMOD);
	FMode->AddLine(FASTMOD);
	FMode->CurLine=exFMode;

	
	MUS->AddLine(GetTextByID("NOCD"));
	//MUS->AddLine(GetTextByID("NATMUS"));
	MUS->AddLine(GetTextByID("RNDMUS"));
	MUS->CurLine=GSets.SVOpt.MusicPlayMode!=0;

	ItemChoose=-1;
	//SVL->SPos=WarSound;
	//MVL->SPos=MidiSound;
	//int pp=1;
	int PrevPlayMode=MUS->CurLine!=0;
	KeyPressed=0;
	int ppp=GSets.SVOpt.MusicPlayMode;
	int StartTime=GetTickCount();
	do{
#ifdef _USE3D
		void ProcessScreen();
		ProcessScreen();
#endif //_USE3D
		if(GetTickCount()-StartTime>10000&&NPlayers>1){
			ItemChoose=mcmCancel;
			ShowOptMessage("#LOOKOPT",0);
			//SpecCmd=199;
			//IAmLeft();
		};
		ProcessMessages();
		StdKeys();		
		MMenu.MarkToDraw();
		MMenu.ProcessDialogs();
		MMenu.RefreshView();
		SetCDVolume(MVL->SPos);
		if(PrevPlayMode!=(MUS->CurLine!=0)){
			GSets.SVOpt.MusicPlayMode=MUS->CurLine!=0;
			if(MUS->CurLine)PlayRandomTrack();
			else StopPlayCD();
			PrevPlayMode=MUS->CurLine!=0;
		};
	}while(ItemChoose==-1);
	if(ItemChoose==mcmOk){
		FPSTime=100-HB->SPos;
		GSets.SVOpt.ScrollSpeed=SSP->SPos+2;
		exFMode=FMode->CurLine;
		if(RealLx!=ModeLX[VMode->CurLine]||RealLy!=ModeLY[VMode->CurLine])SetGameDisplayModeAnyway(ModeLX[VMode->CurLine],ModeLY[VMode->CurLine]);
		GSets.SVOpt.SoundVolume=SVL->SPos;
		GSets.SVOpt.MusicVolume=MVL->SPos;
		GSets.SVOpt.MusicPlayMode=MUS->CurLine!=0;
	}else{
		GSets.SVOpt.MusicPlayMode=ppp;
		if((GSets.SVOpt.MusicPlayMode!=0)!=PrevPlayMode){
			if(GSets.SVOpt.MusicPlayMode)PlayRandomTrack();
			else StopPlayCD();
		};
	};
	return false;
};