void ProcessGSaveGame(){
	LocalGP BTNS("Interf2\\G_Load_Game");
	LocalGP SCROL("Interf2\\GScroll");

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

	// Fonts
	RLCFont* FWin = &fon16y5;
	RLCFont* FAPad = &fon18w;
	RLCFont* FPPad = &fon18y3;
	RLCFont* FABut = &fon18y5;
	RLCFont* FPBut = &fon18y5;
	RLCFont* FText = &fon18y3;

	int SaveDx = -60;
	int SaveDy = -40;

	//Picture* PIC=MMenu.addPicture(NULL,0,0,&MnPanel,&MnPanel,&MnPanel);
	//MMenu.addGPPicture(NULL,dx,dy,BTNS.GPID,0);
	GPPicture* Wind = MMenu.addGPPicture(NULL,SaveDx,SaveDy,BTNS.GPID,5);
	GPS.ShowGP(SaveDx+dx,SaveDy+dy,BTNS.GPID,4,0);

	int NameX = SaveDx+63;
	int DateX = NameX+268;
	int ListY = SaveDy+81;

	VScrollBar* VSB=MMenu.addNewGP_VScrollBar(NULL,SaveDx+515,SaveDy+70,255,1,0,SCROL.GPID,0);
	ListBox* LB=MMenu.addGP_ListBox(NULL,SaveDx+63,SaveDy+81,9,BTNS.GPID,6,26,FAPad,FPPad,VSB);
	ListBox* DT=MMenu.addGP_ListBox(NULL,DateX,ListY,9,BTNS.GPID,-3,26,FAPad,FPPad,VSB); // Data / Time

	//VScrollBar* VS=DSY.addVScrollBar(PIC1,LB->x-DSY.BaseX+LB->oneLx-1,1+LB->y-DSY.BaseY,1000,4,&sb_up_d,&sb_up_e,&sb_up_p,&sb_dn_d,&sb_dn_e,&sb_dn_p,&sb_vbare,&sb_vbar,&sb_btn);
	//LB->VS=VS;226x241
	
	LB->FontDy -= 3;
	DT->FontDy -= 3;
	LB->FontDx -=16;
	DT->FontDx -=16;

	int ButY = 391;
	GP_TextButton* OkBtn=MMenu.addGP_TextButton(NULL,SaveDx+76,SaveDy+ButY,GetTextByID("INTF_SAVE"),BTNS.GPID,1,FABut, FPBut);
	GP_TextButton* CancelBtn=MMenu.addGP_TextButton(NULL,SaveDx+308,SaveDy+ButY,GetTextByID("INTF_CANCEL"),BTNS.GPID,1,FABut, FPBut);	

	OkBtn->FontDy += 4;
	CancelBtn->FontDy += 4;
	OkBtn->FontDx += 5;
	CancelBtn->FontDx += 5;

	OkBtn->UserParam=mcmOk;
	OkBtn->OnUserClick=&MMItemChoose;
	
	CancelBtn->UserParam=mcmCancel;
	CancelBtn->OnUserClick=&MMItemChoose;

	// Text
	TextButton* WT = MMenu.addTextButton(NULL,SaveDx+295,SaveDy+36,GetTextByID("INTF_LOAD_TITLE1"),FWin,FWin,FWin,1);	
	TextButton* T0 = MMenu.addTextButton(NULL,SaveDx+62,SaveDy+353,GetTextByID("INTF_LOAD_CG"),FText,FText,FText,0);
	char* str=strstr(GameName,"Save\\");
	if(str){
		strcpy(GameName,str+5);
	};

	InputBox* IB=MMenu.addInputBox(NULL,SaveDx+210,SaveDy+349,GameName,35,302,24,FPPad,FAPad);	
	IB->Active=true;

	/*
	Wind->x+=SaveDx; Wind->y+=SaveDy; Wind->x1+=SaveDx; Wind->y1+=SaveDy;
	VSB->x+=SaveDx; VSB->y+=SaveDy; VSB->x1+=SaveDx; VSB->y1+=SaveDy;
	LB->x+=SaveDx; LB->y+=SaveDy; LB->x1+=SaveDx; LB->y1+=SaveDy;
	WT->x+=SaveDx; WT->y+=SaveDy; WT->x1+=SaveDx; WT->y1+=SaveDy;
	T0->x+=SaveDx; T0->y+=SaveDy; T0->x1+=SaveDx; T0->y1+=SaveDy;
	IB->x+=SaveDx; IB->y+=SaveDy; IB->x1+=SaveDx; IB->y1+=SaveDy;
	OkBtn->x+=SaveDx; OkBtn->y+=SaveDy; OkBtn->x1+=SaveDx; OkBtn->y1+=SaveDy;
	CancelBtn->x+=SaveDx; CancelBtn->y+=SaveDy; CancelBtn->x1+=SaveDx; CancelBtn->y1+=SaveDy;
	*/
	
	//CreateGamesList(LB);
	ClearNames();
	CreateGamesList(LB,DT);

	LB->CurItem=-1;
	
	ItemChoose=-1;
	int LastCTime=GetRealTime();
	int PrevListVal=LB->CurItem;
	KeyPressed=false;
	int StartTime=GetTickCount();
	do{	
#ifdef _USE3D
		void ProcessScreen();
		ProcessScreen();
#endif //_USE3D
		if(GetTickCount()-StartTime>15000&&NPlayers>1){
			ItemChoose=mcmCancel;
			ShowOptMessage("#LOOKOPT",0);
			//SpecCmd=199;
			//IAmLeft();
		};
		if(PrevListVal!=LB->CurItem){
			PrevListVal=LB->CurItem;
			strcpy(GameName,LB->GetItem(LB->CurItem)->Message);
			GameName[strlen(GameName)-4]=0;
			IB->CursPos = strlen(IB->Str);
			IB->Active = true;
		}else{
			char tmpname[255];
			if(LB->CurItem>=0){
				strcpy(tmpname,LB->GetItem(LB->CurItem)->Message);
				tmpname[strlen(tmpname)-4]=0;
				if(strcmp(GameName,tmpname)){
					LB->CurItem=-1;
					PrevListVal=-1;
				}
			};
		}
		DT->CurItem=LB->CurItem;
		MMenu.MarkToDraw();
		ProcessMessages();
		StdKeys();
		//if(LB->CurItem==-1)OkBtn->Enabled=false;
		//else OkBtn->Enabled=true;
		MMenu.ProcessDialogs();
		MMenu.RefreshView();
		if(strlen(IB->Str)){
			OkBtn->Enabled=true;
		}else{
			OkBtn->Enabled=false;
			if(ItemChoose==mcmOk)ItemChoose=-1;
		};
	}while(ItemChoose==-1);
	if(ItemChoose==mcmOk){
		CmdSaveNetworkGame(MyNation,GetRealTime(),IB->Str);
	};
	ContinueGame=true;
	FreeNames();
	ClearMStack();
};