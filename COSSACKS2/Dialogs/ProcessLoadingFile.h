bool ProcessLoadingFile(char* Mask,char* DestName,int Header){
	LocalGP HBTN("Interface\\LoadSaveBM");
	
	LocalGP BTNS("Interf2\\LoadEditor");
	LocalGP SCROL("Interf3\\elements\\scroll");
	
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
	int dx=(RealLx-GPS.GetGPWidth(BTNS.GPID,5)-10)>>1;
	int dy=(RealLy-GPS.GetGPHeight(BTNS.GPID,5))>>1;
	DialogsSystem MMenu(dx,dy);
	
	//GPS.ShowGP(dx,dy,BTNS.GPID,4,0);

	MMenu.HintFont=&hfnt;
	MMenu.HintY=701;
	MMenu.HintX=18;

	int X = 0;
	int Y = 0;

	//Picture* PIC=MMenu.addPicture(NULL,0,0,&MnPanel,&MnPanel,&MnPanel);
	//MMenu.addGPPicture(NULL,dx,dy,BTNS.GPID,0);
	
	//MMenu.addGPPicture(NULL,0,0,BTNS.GPID,5);
	//VScrollBar* VSB=MMenu.addNewGP_VScrollBar(NULL,436,55,276,1,0,SCROL.GPID,0);
	//ListBox* LB=MMenu.addGP_ListBox(NULL,29,61,10,BTNS.GPID,6,26,&WhiteFont,&YellowFont,VSB);

	GPS.ShowGP(X+dx,Y+dy,BTNS.GPID,4,0);
	GPPicture* Wind = MMenu.addGPPicture(NULL,X,Y,BTNS.GPID,5);

	// Load game list
	int NameX = X+63;
	int DateX = NameX+268;
	int ListY = Y+81;
	
	VScrollBar* VSB=MMenu.addNewGP_VScrollBar(NULL,X+515,Y+70,255,1000,0,SCROL.GPID,0);

	ListBox* LB=MMenu.addGP_ListBox(NULL,NameX,ListY,9,BTNS.GPID,6,26,FAPad,FPPad,VSB);
	ListBox* DT=MMenu.addGP_ListBox(NULL,DateX,ListY,9,BTNS.GPID,-3,26,FAPad,FPPad,NULL); // Data / Time

	LB->FontDy -= 3;
	DT->FontDy -= 3;
	LB->FontDx -=16;
	DT->FontDx -=16;

	//VScrollBar* VS=DSY.addVScrollBar(PIC1,LB->x-DSY.BaseX+LB->oneLx-1,1+LB->y-DSY.BaseY,1000,4,&sb_up_d,&sb_up_e,&sb_up_p,&sb_dn_d,&sb_dn_e,&sb_dn_p,&sb_vbare,&sb_vbar,&sb_btn);
	//LB->VS=VS;226x241
	/*
	GPPicture* HDR=MMenu.addGPPicture(NULL,28,31,HBTN.GPID,Header);
	GP_Button* OkBtn=MMenu.addGP_Button(NULL,49,353,HBTN.GPID,2,0);
	OkBtn->UserParam=mcmOk;
	OkBtn->OnUserClick=&MMItemChoose;
	GP_Button* CancelBtn=MMenu.addGP_Button(NULL,269,353,BTNS.GPID,2,3);
	CancelBtn->UserParam=mcmCancel;
	CancelBtn->OnUserClick=&MMItemChoose;
	*/

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

	CreateFilesList(Mask,LB);
	//ClearNames();
	//CreateGamesList(LB,DT);

	char* Title=GetTextByID("INTF_LOAD");
	switch(Header){
	case 3:
		Title=GetTextByID("INTF_BITM_LOAD");
		break;
	case 4:
		Title=GetTextByID("INTF_LOAD_MAP");
		break;
	};

	// Text
	MMenu.addTextButton(NULL,X+137+319/2,Y+36,Title,FWin,FWin,FWin,1);
	MMenu.addTextButton(NULL,X+70,Y+353,GetTextByID("INTF_FILE_NAME"),FText,FText,FText,0);

	char FileName[250];
	FileName[0]=0;
	InputBox* IB=MMenu.addInputBox(NULL,X+210,Y+349,FileName,35,302,24,FPPad,FAPad);	
	IB->Enabled=0;

	ItemChoose=-1;
	int pp=1;
	int LastCTime=GetRealTime();
	LastKey=0;
	do{
		if(LB->CurItem!=-1&&LB->NItems) strcpy(FileName,LB->GetItem(LB->CurItem)->Message);
		MMenu.MarkToDraw();
		ProcessMessages();
		StdKeys();
		if(LB->CurItem==-1){
			OkBtn->Enabled=false;
			if(ItemChoose==mcmOk)ItemChoose=-1;
		}else OkBtn->Enabled=true;
		MMenu.ProcessDialogs();
		MMenu.RefreshView();
	}while(ItemChoose==-1);
	if(ItemChoose==mcmOk&&LB->CurItem!=-1&&LB->NItems){
		strcpy(DestName,LB->GetItem(LB->CurItem)->Message);
		ClearMStack();
		ItemChoose=-1;
		UnPress();
		return true;
	};
	ClearMStack();
	ItemChoose=-1;
	UnPress();
	return false;
};