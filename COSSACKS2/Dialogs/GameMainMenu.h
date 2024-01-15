CIMPORT bool CheckIfGameRated();

byte DisableSave=0;

int ProcessGMainMenu(){
	LoadPointer("cur_mn.rlc");

	if(NPlayers>1){
		if(LastLookTime&&(GetTickCount()-LastLookTime<180000)){
			int v=(180000-GetTickCount()+LastLookTime)/1000;
			ShowOptMessage("#F12LOCK",v);
			return mcmResume;
		};
	};
	LastLookTime=GetTickCount();
	LocalGP BTNS("Interf2\\GameMenu");
	ContinueGame=false;
	GameMode=2;
	//bool MIMOD=MiniMode;
	int mmmx=mapx;
	int mmmy=mapy;
	//ClearMiniMode();
	//int dx=smapx+(nsmaplx<<4)-(GPS.GetGPWidth(BTNS.GPID,1)>>1);
	//int dy=smapy+(mul3(nsmaply)<<2)-(GPS.GetGPHeight(BTNS.GPID,1)>>1);
	int dx=((RealLx-470)>>1)-14;
	int dy=(RealLy-415)>>1;
	GPS.ShowGP(dx,dy,BTNS.GPID,0,0);
	
	DialogsSystem GMM(dx,dy);

	// Fonts
	RLCFont* FWin = &fon16y5;
	RLCFont* FAPad = &fon18w;
	RLCFont* FPPad = &fon18y3;

	//GMM.addGPPicture(NULL,0,0,BTNS.GPID,0);
	GMM.addGPPicture(NULL,0,0,BTNS.GPID,1);

	// Text
	GMM.addTextButton(NULL,254,36,GetTextByID("INTF_GMENU_WIN"),FWin,FWin,FWin,1);

	int PadX = 46;
	int PadY = 75;
	int PadDy = 26;

	//GP_Button* ResumeBtn=GMM.addGP_Button(NULL,PadX,PadY,BTNS.GPID,3,2);
	//GP_Button* SaveBtn=GMM.addGP_Button(NULL,PadX,PadY+PadDy*1,BTNS.GPID,7,6);
	//GP_Button* OptionsBtn=GMM.addGP_Button(NULL,PadX,PadY+PadDy*3,BTNS.GPID,9,8);
	//GP_Button* ExitBtn=GMM.addGP_Button(NULL,PadX,PadY+PadDy*4,BTNS.GPID,11,10);
	GP_TextButton* ResumeBtn=GMM.addGP_TextButton(NULL,PadX,PadY,GetTextByID("INTF_GMENU_PAD0"),BTNS.GPID,3,FAPad,FPPad);
	GP_TextButton* SaveBtn=GMM.addGP_TextButton(NULL,PadX,PadY+PadDy*1,GetTextByID("INTF_GMENU_PAD1"),BTNS.GPID,5,FAPad,FPPad);
	GP_TextButton* OptionsBtn=GMM.addGP_TextButton(NULL,PadX,PadY+PadDy*3,GetTextByID("INTF_GMENU_PAD3"),BTNS.GPID,9,FAPad,FPPad);
	GP_TextButton* ExitBtn=GMM.addGP_TextButton(NULL,PadX,PadY+PadDy*4,GetTextByID("INTF_GMENU_PAD4"),BTNS.GPID,11,FAPad,FPPad);

	ResumeBtn->FontDy--;
	SaveBtn->FontDy--;
	OptionsBtn->FontDy--;
	ExitBtn->FontDy--;
	
	ResumeBtn->UserParam=mcmResume;
	ResumeBtn->OnUserClick=&MMItemChoose;
	
	SaveBtn->UserParam=mcmSave;
	SaveBtn->OnUserClick=&MMItemChoose;

	GP_TextButton* LoadBtn;
#ifdef COSSACKS2
	if(!(IsGameActive()||PlayGameMode||CheckIfGameRated())){
#else
	if(!(IsGameActive()||PlayGameMode||CheckIfGameRated()||NPlayers>1)){
#endif
		//GP_Button* LoadBtn=GMM.addGP_Button(NULL,PadX,PadY+PadDy*2,BTNS.GPID,5,4);
		LoadBtn=GMM.addGP_TextButton(NULL,PadX,PadY+PadDy*2,GetTextByID("INTF_MAIN_2"),BTNS.GPID,5,FAPad,FPPad);
		LoadBtn->UserParam=mcmLoad;
		LoadBtn->OnUserClick=&MMItemChoose;
	}else{
		//GP_Button* LoadBtn=GMM.addGP_Button(NULL,PadX,PadY+PadDy*2,BTNS.GPID,4,4);
		LoadBtn=GMM.addGP_TextButton(NULL,PadX,PadY+PadDy*2,GetTextByID("INTF_MAIN_2"),BTNS.GPID,4,FPPad,FPPad);
	};

	if(DisableSave){
		SaveBtn->Enabled=0;
		LoadBtn->Enabled=0;
	}

	OptionsBtn->UserParam=mcmOptions;
	OptionsBtn->OnUserClick=&MMItemChoose;
	
	ExitBtn->UserParam=mcmExit;
	ExitBtn->OnUserClick=&MMItemChoose;
	//TextButton* HelpBtn=GMM.addTextButton(PIC1,352>>1,30+160,"ŽŒŽ™œ",&FActive,&FPassive,&FDisable,1);
	//HelpBtn->UserParam=mcmHelp;
	//HelpBtn->OnUserClick=&MMItemChoose;
stg:
	ItemChoose=-1;
	int StartTime=GetTickCount();
	do{
#ifdef _USE3D
		void ProcessScreen();
		ProcessScreen();
#endif //_USE3D
		ProcessMessages();
		GMM.ProcessDialogs();
		GMM.RefreshView();
		if(GetTickCount()-StartTime>15000&&NPlayers>1){
			ShowOptMessage("#LOOKOPT",0);
			ItemChoose=mcmResume;
			//SpecCmd=199;
			//IAmLeft();
		};
	}while(ItemChoose==-1);
	if(ItemChoose==mcmLoad){
		ProcessGLoadGame();
		GMM.MarkToDraw();
		if(!ContinueGame)goto stg;
	};
	if(ItemChoose==mcmSave){
		ProcessGSaveGame();
		GMM.MarkToDraw();
		if(!ContinueGame)goto stg;
	};
	if(ItemChoose==mcmOptions){
		//ProcessOptions(smapx+(nsmaplx<<4)-(288>>1),smapy+(nsmaply<<4)-(288>>1));
		ProcessGameOptions();
		ContinueGame=true;
		ItemChoose=mcmResume;
	};
	if(ItemChoose==mcmHelp){
		ProcessHelp();
	};
	//if(MIMOD)SetMiniMode();
	mapx=mmmx;
	mapy=mmmy;
	GameMode=0;
	ClearMStack();

	KeyPressed=0;
	return ItemChoose;	
};