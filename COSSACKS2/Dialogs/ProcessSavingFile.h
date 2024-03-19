bool ProcessSavingFile(char* Mask,char* DestName,int Header,bool clear){
	LocalGP BTNS("Interf2\\LoadEditor");
	LocalGP SCROL("Interf2\\GScroll");

	LocalGP HBTN("Interface\\LoadSaveBM");

	LocalGP HFONT("interf3\\Fonts\\FontC10");
	RLCFont hfnt(HFONT.GPID);
	hfnt.SetWhiteColor();
	//LoadPointer("cur_mn.rlc");
	int dx=(RealLx-GPS.GetGPWidth(BTNS.GPID,5)-10)>>1;
	int dy=(RealLy-GPS.GetGPHeight(BTNS.GPID,5))>>1;
	DialogsSystem MMenu(dx,dy);
	
	//GPS.ShowGP(dx,dy,BTNS.GPID,4,0);

	// Fonts
	RLCFont* FWin = &fon16y5;
	RLCFont* FAPad = &fon18w;
	RLCFont* FPPad = &fon18y3;
	RLCFont* FABut = &fon18y5;
	RLCFont* FPBut = &fon18y5;
	RLCFont* FText = &fon18y3;
	
	MMenu.HintFont=&hfnt;
	MMenu.HintY=701;
	MMenu.HintX=18;

	int SaveDx = 0;
	int SaveDy = 0;

	//Picture* PIC=MMenu.addPicture(NULL,0,0,&MnPanel,&MnPanel,&MnPanel);
	//MMenu.addGPPicture(NULL,dx,dy,BTNS.GPID,0);
	
	//MMenu.addGPPicture(NULL,0,0,BTNS.GPID,5);
	//VScrollBar* VSB=MMenu.addNewGP_VScrollBar(NULL,436,55,247,1,0,SCROL.GPID,0);
	//ListBox* LB=MMenu.addGP_ListBox(NULL,29,61,9,BTNS.GPID,6,26,&WhiteFont,&YellowFont,VSB);

	GPPicture* Wind = MMenu.addGPPicture(NULL,SaveDx,SaveDy,BTNS.GPID,5);
	GPS.ShowGP(SaveDx+dx,SaveDy+dy,BTNS.GPID,4,0);

	int NameX = SaveDx+63;
	int DateX = NameX+268;
	int ListY = SaveDy+81;

	VScrollBar* VSB=MMenu.addNewGP_VScrollBar(NULL,SaveDx+515,SaveDy+70,255,1,0,SCROL.GPID,0);
	ListBox* LB=MMenu.addGP_ListBox(NULL,SaveDx+63,SaveDy+81,9,BTNS.GPID,6,26,FAPad,FPPad,VSB);
	ListBox* DT=MMenu.addGP_ListBox(NULL,DateX,ListY,9,BTNS.GPID,-3,26,FAPad,FPPad,NULL); // Data / Time

	LB->FontDy -= 3;
	DT->FontDy -= 3;
	LB->FontDx -=16;
	DT->FontDx -=16;

	//VScrollBar* VSB=MMenu.addNewGP_VScrollBar(NULL,SaveDx+515,SaveDy+70,255,1,0,SCROL.GPID,0);
	//ListBox* LB=MMenu.addGP_ListBox(NULL,SaveDx+63,SaveDy+81,9,BTNS.GPID,6,26,FAPad,FPPad,VSB);
	//ListBox* DT=MMenu.addGP_ListBox(NULL,DateX,ListY,9,BTNS.GPID,-3,26,FAPad,FPPad,VSB); // Data / Time
	//GPPicture* HDR=MMenu.addGPPicture(NULL,28,31,HBTN.GPID,Header);
	
	/*
	GP_Button* OkBtn=MMenu.addGP_Button(NULL,49,353,HBTN.GPID,5,1);
	OkBtn->UserParam=mcmOk;
	OkBtn->OnUserClick=&MMItemChoose;
	GP_Button* CancelBtn=MMenu.addGP_Button(NULL,269,353,BTNS.GPID,2,3);
	CancelBtn->UserParam=mcmCancel;
	CancelBtn->OnUserClick=&MMItemChoose;
	*/

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

	char* Title=GetTextByID("INTF_SAVE");
	switch(Header){
	case 3:
		Title=GetTextByID("INTF_BITM_SAVE");
		break;
	case 4:
		Title=GetTextByID("INTF_SAVE_MAP");
		break;
	};
	// Text
	MMenu.addTextButton(NULL,SaveDx+137+319/2,SaveDy+36,Title,FWin,FWin,FWin,1);
	MMenu.addTextButton(NULL,SaveDx+70,SaveDy+353,GetTextByID("INTF_FILE_NAME"),FText,FText,FText,0);

	if(clear){
		DestName[0]=0;
		GameName[0]=0;
	}

	InputBox* IB=MMenu.addInputBox(NULL,SaveDx+210,SaveDy+349,GameName,35,302,24,FPPad,FAPad);	
	IB->Active=true;
	
	ItemChoose=-1;
	CreateFilesList(Mask,LB);
	LB->CurItem=-1;
	int LastCTime=GetRealTime();
	int PrevListVal=LB->CurItem;
	KeyPressed=false;
	do{
		if(PrevListVal!=LB->CurItem){
			PrevListVal=LB->CurItem;
			strcpy(IB->Str,LB->GetItem(LB->CurItem)->Message);
			//IB->Str[strlen(IB->Str)-4]=0;
		};
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
		ClearMStack();
		ItemChoose=-1;
		UnPress();
		strcpy(DestName,GameName);
		return true;
	};
	ClearMStack();
	ItemChoose=-1;
	UnPress();
	return false;
};