int ProcessE_GMainMenu(){
	//LocalGP BTNS("Interface\\E_GameMenu");
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
	GMM.addTextButton(NULL,254,36,GetTextByID("INTF_EMENU_WIN"),FWin,FWin,FWin,1);

	int PadX = 46;
	int PadY = 75;
	int PadDy = 26;

	//GP_Button* ResumeBtn=GMM.addGP_Button(NULL,43,70,BTNS.GPID,3,2);
	//GP_Button* SaveBtn=GMM.addGP_Button(NULL,43,70+31*1,BTNS.GPID,7,6);
	//GP_Button* LoadBtn=GMM.addGP_Button(NULL,43,70+31*2,BTNS.GPID,5,4);
	//GP_Button* OptionsBtn=GMM.addGP_Button(NULL,43,70+31*3,BTNS.GPID,9,8);
	//GP_Button* ExitBtn=GMM.addGP_Button(NULL,43,70+31*4,BTNS.GPID,11,10);
	GP_TextButton* ResumeBtn=GMM.addGP_TextButton(NULL,PadX,PadY,GetTextByID("INTF_EMENU_RESM"),BTNS.GPID,3,FAPad,FPPad);
	GP_TextButton* SaveBtn=GMM.addGP_TextButton(NULL,PadX,PadY+PadDy*1,GetTextByID("INTF_EMENU_SAVE"),BTNS.GPID,5,FAPad,FPPad);
	GP_TextButton* LoadBtn=GMM.addGP_TextButton(NULL,PadX,PadY+PadDy*2,GetTextByID("INTF_EMENU_LOAD"),BTNS.GPID,5,FAPad,FPPad);
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
	
	LoadBtn->UserParam=mcmLoad;
	LoadBtn->OnUserClick=&MMItemChoose;
	
	OptionsBtn->UserParam=mcmOptions;
	OptionsBtn->OnUserClick=&MMItemChoose;
	
	ExitBtn->UserParam=mcmExit;
	ExitBtn->OnUserClick=&MMItemChoose;
	//TextButton* HelpBtn=GMM.addTextButton(PIC1,352>>1,30+160,"ŽŒŽ™œ",&FActive,&FPassive,&FDisable,1);
	//HelpBtn->UserParam=mcmHelp;
	//HelpBtn->OnUserClick=&MMItemChoose;
stg:
	ItemChoose=-1;
	do{
#ifdef _USE3D
		void ProcessScreen();
		ProcessScreen();
#endif //_USE3D
		ProcessMessages();
		GMM.ProcessDialogs();
		GMM.RefreshView();
	}while(ItemChoose==-1);
	if(ItemChoose==mcmLoad){
		ProcessGLoadMap();
		GMM.MarkToDraw();
		if(!ContinueGame)goto stg;
	};
	if(ItemChoose==mcmSave){
		ProcessGSaveMap();
		GMM.MarkToDraw();
		if(!ContinueGame)goto stg;
	};
	if(ItemChoose==mcmOptions){
		ProcessGameOptions();
		//ProcessOptions(smapx+(nsmaplx<<4)-(288>>1),smapy+(nsmaply<<3)-(288>>1));
		ContinueGame=true;
		ItemChoose=mcmResume;
	};

	mapx=mmmx;
	mapy=mmmy;
	GameMode=0;
	ClearMStack();

	KeyPressed=0;
	return ItemChoose;	
};