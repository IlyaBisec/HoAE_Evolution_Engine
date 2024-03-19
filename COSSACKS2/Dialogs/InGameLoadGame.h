void ProcessGLoadGame(){
	LocalGP BTNS("Interf2\\G_Load_Game");	
	LocalGP SCROL("Interf2\\GScroll");

	// Fonts
	RLCFont* FWin = &fon16y5;
	RLCFont* FAPad = &fon18w;
	RLCFont* FPPad = &fon18y3;
	RLCFont* FABut = &fon18y5;
	RLCFont* FPBut = &fon18y5;
	RLCFont* FText = &fon18y3;

	LocalGP HFONT("interf3\\Fonts\\FontC10");
	RLCFont hfnt(HFONT.GPID);
	hfnt.SetWhiteColor();
	//LoadPointer("cur_mn.rlc");
	int dx=(RealLx-470)>>1;
	int dy=(RealLy-415)>>1;
	DialogsSystem MMenu(dx,dy);
	
	MMenu.HintFont=&hfnt;
	MMenu.HintY=701;
	MMenu.HintX=18;

	int X = -60;
	int Y = -40;

	//Picture* PIC=MMenu.addPicture(NULL,0,0,&MnPanel,&MnPanel,&MnPanel);
	//MMenu.addGPPicture(NULL,dx,dy,BTNS.GPID,0);
	GPS.ShowGP(X+dx,Y+dy,BTNS.GPID,4,0);
	GPPicture* Wind = MMenu.addGPPicture(NULL,X,Y,BTNS.GPID,5);
	
	//VScrollBar* VS=MMenu.addNewGP_VScrollBar(NULL,901,240+26,264-26,1000,0,VSC.GPID,0);
	VScrollBar* VSB=MMenu.addNewGP_VScrollBar(NULL,X+515,Y+70,255,1,0,SCROL.GPID,0);
	
	int NameX = X+63;
	int DateX = NameX+268;
	int ListY = Y+81;
	
	// Load game list
	ListBox* LB=MMenu.addGP_ListBox(NULL,NameX,ListY,9,BTNS.GPID,6,26,FAPad,FPPad,VSB);
	ListBox* DT=MMenu.addGP_ListBox(NULL,DateX,ListY,9,BTNS.GPID,-3,26,FAPad,FPPad,VSB); // Data / Time

	LB->FontDy -= 3;
	DT->FontDy -= 3;
	LB->FontDx -=16;
	DT->FontDx -=16;

	//VScrollBar* VS=DSY.addVScrollBar(PIC1,LB->x-DSY.BaseX+LB->oneLx-1,1+LB->y-DSY.BaseY,1000,4,&sb_up_d,&sb_up_e,&sb_up_p,&sb_dn_d,&sb_dn_e,&sb_dn_p,&sb_vbare,&sb_vbar,&sb_btn);
	//LB->VS=VS;226x241
	
	int ButY = Y+391;
	GP_TextButton* OkBtn=MMenu.addGP_TextButton(NULL,X+76,ButY,GetTextByID("INTF_LOAD"),BTNS.GPID,1,FABut, FPBut);
	GP_TextButton* CancelBtn=MMenu.addGP_TextButton(NULL,X+308,ButY,GetTextByID("INTF_CANCEL"),BTNS.GPID,1,FABut, FPBut);	

	OkBtn->FontDy += 4;
	CancelBtn->FontDy += 4;
	OkBtn->FontDx += 5;
	CancelBtn->FontDx += 5;

	OkBtn->UserParam=mcmOk;
	OkBtn->OnUserClick=&MMItemChoose;
	
	CancelBtn->UserParam=mcmCancel;
	CancelBtn->OnUserClick=&MMItemChoose;

	// Text
	TextButton* WT = MMenu.addTextButton(NULL,X+295,Y+36,GetTextByID("INTF_LOAD_TITLE1"),FWin,FWin,FWin,1);	
	TextButton* T0 = MMenu.addTextButton(NULL,X+120,Y+353,GetTextByID("INTF_LOAD_CG"),FText,FText,FText,1);

	//InputBox* IB=MMenu.addInputBox(NULL,212,349,GameName,35,302,24,FPPad,FAPad);
	TextButton* CurSave=MMenu.addTextButton(NULL,X+218,Y+353,"                                                            ",FAPad,FAPad,FAPad,0);	
	
	//IB->Enabled=0;

	ClearNames();
	CreateGamesList(LB,DT);
	//CreateGamesList(LB);

	ItemChoose=-1;
	int pp=1;
	int LastCTime=GetRealTime();
	LastKey=0;
	int StartTime=GetTickCount();
	do{
#ifdef _USE3D
		void ProcessScreen();
		ProcessScreen();
#endif //_USE3D
		DT->CurItem=LB->CurItem;
		if(GetTickCount()-StartTime>10000&&NPlayers>1){
			ItemChoose=mcmCancel;
			ShowOptMessage("#LOOKOPT",0);
			//SpecCmd=199;
			//IAmLeft();
		};
		MMenu.MarkToDraw();
		ProcessMessages();
		StdKeys();
		if(LB->CurItem==-1||!LB->NItems){
			OkBtn->Enabled=false;
			if(ItemChoose==mcmOk)ItemChoose=-1;
			GameName[0]=0;
		}else{
			OkBtn->Enabled=true;
			strcpy(GameName,LB->GetItem(LB->CurItem)->Message);
			GameName[strlen(GameName)-4]=0;
		}
		strcpy(CurSave->Message,GameName);

		MMenu.ProcessDialogs();
		MMenu.RefreshView();
	}while(ItemChoose==-1);
	if(ItemChoose==mcmOk&&LB->CurItem!=-1){
		//if(CheckFileIdentity(fnames[LB->CurItem])){
			strcpy(SaveFileName,fnames[LB->GetItem(LB->CurItem)->Param1]);
			if(NPlayers>1)CmdLoadNetworkGame(MyNation,serials[LB->CurItem],SaveFileName);
			else DLL2(SFLB_LoadGame,SaveFileName,1);
		//}else{
		//	AssignHint1(GetTextByID("SFNID"),100,1);
		//};
	};
	ContinueGame=true;
	FreeNames();
	ClearMStack();
};