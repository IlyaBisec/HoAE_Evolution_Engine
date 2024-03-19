extern int PrevMission;
extern bool ShowPostScreen;
void ProcessCampagins(int Options){
	if(!CAMPAGINS.NCamp){
		ItemChoose=mcmCancel;
		return;
	}
	KeyPressed=0;
	LastKey=0;
	LoadFog(1);
	LoadPalette("1\\agew_1.pal");
	//if(Options==-1)SlowUnLoadPalette("1\\agew_1.pal");
	
	//SQPicture Back("Interface\\Background_Campaign_Menu.bmp");
	SQPicture Back("Interf2\\Background_Campaign.bmp");

	LocalGP BTNS("Interface\\Campaigns");
	//LocalGP SCROL("Interface\\_Slider");
	LocalGP OK1("Interface\\anm_start_on");
	LocalGP OK2("Interface\\anm_start_off");
	LocalGP CANCEL1("Interface\\anm_cancel_on");
	LocalGP CANCEL2("Interface\\anm_cancel_off");
	LocalGP ENC1("Interface\\anm_enc_on");
	LocalGP ENC2("Interface\\anm_enc_off");
	LocalGP FONT("interf3\\Fonts\\FontC10");
	RLCFont FontY(FONT.GPID);
	//FontY.SetColorTable(3);
	//LoadOptionalTable(3,"2\\orange.grd");

	// GP
	LocalGP SCROL("Interf3\\elements\\scroll");
	LocalGP CMB("Interf2\\Combo");
	LocalGP LGP("Interf2\\ListBox");
	LocalGP GmBtn("Interf2\\GameButtons");

	// Fonts
	RLCFont* FScr = &fon40y5;
	RLCFont* FWin = &fon16y5;
	RLCFont* FDialog = &fon18y3;
	RLCFont* FAList = &fon18w;
	RLCFont* FPList = &fon18y3;
	RLCFont* FACombo = &fon18w;
	RLCFont* FPCombo = &fon18y3;
	RLCFont* FMissTitle = &fon18y3;
	RLCFont* FDescrText = &fon16y3;
	RLCFont* FHint = &SmallYellowFont;

	RLCFont FontW(FONT.GPID);
	FontW.SetWhiteColor();
	DialogsSystem MMenu(0,0);
	MMenu.OkSound=GetSound("START");
	LocalGP HFONT("interf3\\Fonts\\FontC10");
	RLCFont hfnt(HFONT.GPID);
	hfnt.SetWhiteColor();
	
	int HintX = 440;
	int HintY = 745;

	MMenu.HintFont=FHint;
	MMenu.HintX=HintX;
	MMenu.HintY=HintY;

	TextButton* HintTxt = MMenu.addTextButton(NULL,HintX,HintY,"                                                                                               ",FHint,FHint,FHint,1);

	MMenu.addPicture(NULL,0,0,&Back,&Back,&Back);
	VideoButton* ENC=MMenu.addVideoButton(NULL,862,468,ENC1.GPID,ENC2.GPID);
	ENC->UserParam=0;
	ENC->OnUserClick=&RunEnciclopia;
	ENC->Hint=GetTextByID("ENCICL");
	ENC->Visible=0;

	// Text
	MMenu.addTextButton(NULL,512,18,GetTextByID("INTF_CAMPAIGN_T0"),FScr,FScr,FScr,1);
	MMenu.addTextButton(NULL,512,334,GetTextByID("INTF_CAMPAIGN_W0"),FWin,FWin,FWin,1);

	// List
	int ListX=311;
	int ListY=377;
	
	int DescrY=542;

	VScrollBar* SB=MMenu.addNewGP_VScrollBar(NULL,705,ListY-13,6*26,100,0,SCROL.GPID,0);
	SB->ScrDy=3;
	//SB->Visible=0;

	ListBox* LB=MMenu.addGP_ListBox(NULL,ListX,ListY,5,LGP.GPID,6,26,FAList,FPList,SB);
	LB->FontDx--;
	//LB->M_Over=0;
	LB->Active=true;
	
	LB->FontDy=-3;
	LB->FontDx-=16;

	TextViewer* TV[32];
	VScrollBar* VS[32];
	int NCAM=CAMPAGINS.NCamp;
	for(int i=0;i<NCAM;i++){
		LB->AddItem(CAMPAGINS.SCamp[i].CampMessage,0);
		VS[i]=MMenu.addNewGP_VScrollBar(NULL,755,312+26,316-26,1,0,SCROL.GPID,0);
		VS[i]->ScrDy=13;
		
		TV[i]=MMenu.addTextViewer(NULL,ListX,DescrY,400,90,CAMPAGINS.SCamp[i].CampText,FDescrText);
		TV[i]->AssignScroll(VS[i]);
		TV[i]->SymSize=21;
		TV[i]->PageSize=(TV[i]->y1-TV[i]->y)/TV[i]->SymSize;
		TV[i]->Enabled=false;
		TV[i]->Visible=false;
	};



	// Buttons
	int BInChatX = 839;
	int OkX = 899;
	int CancelX = 959;
	int ButY = 706;

	GP_Button* OkBtn=MMenu.addGP_Button(NULL,OkX,ButY,GmBtn.GPID,0,1);
	GP_Button* CancelBtn=MMenu.addGP_Button(NULL,CancelX,ButY,GmBtn.GPID,3,4);

	OkBtn->OnUserClick=&MMItemChoose;
	OkBtn->UserParam=mcmOk;
	OkBtn->Hint=GetTextByID("MISS_RUN");
	CancelBtn->OnUserClick=&MMItemChoose;
	CancelBtn->UserParam=mcmCancel;
	CancelBtn->Hint=GetTextByID("SOCANCEL");

	/*
	VideoButton* OkBtn=MMenu.addVideoButton(NULL,862,568,OK1.GPID,OK2.GPID);
	OkBtn->OnUserClick=&MMItemChoose;
	OkBtn->UserParam=mcmOk;
	OkBtn->Hint=GetTextByID("WAR_RUN");
	VideoButton* CancelBtn=MMenu.addVideoButton(NULL,862,668,CANCEL1.GPID,CANCEL2.GPID);
	CancelBtn->OnUserClick=&MMItemChoose;
	CancelBtn->UserParam=mcmCancel;
	CancelBtn->Hint=GetTextByID("WAR_CANCEL");
	*/

HHH1:
	ItemChoose=-1;
	KeyPressed=0;
	if(Options==-1){
		int mm=1;
		StartMixing();
		do{
			if(MMenu.Hint)
				strcpy(HintTxt->Message, MMenu.Hint);
			else
				HintTxt->Message[0] = 0;
			
			ProcessMessages();
			StdKeys();
			MMenu.MarkToDraw();
			MMenu.ProcessDialogs();
			ProcessMixing();
			MMenu.RefreshView();
			for(i=0;i<NCAM;i++){
				TV[i]->Enabled=false;
				TV[i]->Visible=false;
				TV[i]->Active=false;
				VS[i]->Visible=false;
			};
			TV[LB->CurItem]->Enabled=1;
			TV[LB->CurItem]->Visible=1;
			if(VS[LB->CurItem]->SMaxPos>0)VS[LB->CurItem]->Visible=0;
			else VS[LB->CurItem]->Visible=0;
			if(mm){
//				SlowLoadPalette("2\\agew_1.pal");
				mm=0;
			};
			StdKeys();
		}while(ItemChoose==-1);
		if(ItemChoose==mcmOk)Options=LB->CurItem;
		ShowPostScreen=0;
	};
	if(Options!=-1){
//		SlowUnLoadPalette("2\\agew_1.pal");
		if(ShowPostScreen){
			if(CurrentMission!=-1&&PrevMission!=-1){
				ShowPostScreen=PostCampaignMessage(PrevMission);
				if(ShowPostScreen)CurrentCampagin=-1;
			};
		};
		if(!ShowPostScreen){
			if(!ProcessSingleCampagin(Options)){
				//SlowLoadPalette("2\\agew_1.pal");
				//LoadFog(2);
				Options=-1;
				goto HHH1;
			};
		};
	}else{
//		SlowUnLoadPalette("2\\agew_1.pal");
	};
	LoadPalette("1\\agew_1.pal");
};