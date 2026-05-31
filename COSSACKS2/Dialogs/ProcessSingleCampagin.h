bool ProcessSingleCampagin(int n){
	KeyPressed=0;
	LastKey=0;

	//Vitya
	if (strcmp(CAMPAGINS.SCamp[n].CampMessage,"Total War.")==0)
		{
		//if (PlayerInterface()) ProcessBigMap(n);//grey		
		//ProcessBigMap(n);		
		return false;
		}

	//SlowUn
	LoadPalette("1\\agew_1.pal");
	SQPicture Back(CAMPAGINS.SCamp[n].CampBmp);

	LocalGP BTNS("Interface\\One_Campaign");
	LocalGP BTN1("Interface\\Single_mission");
	LocalGP OK1("Interface\\anm_start_1_on");
	LocalGP OK2("Interface\\anm_start_1_off");
	LocalGP CANCEL1("Interface\\anm_cancel_on");
	LocalGP CANCEL2("Interface\\anm_cancel_off");
	LocalGP ENC1("Interface\\anm_enc_on");
	LocalGP ENC2("Interface\\anm_enc_off");
	//LocalGP SCROL("Interface\\_Slider");
	
	LoadFog(1);
	_strupr(CAMPAGINS.SCamp[n].CampBmp);
	bool Style=1; //strstr(CAMPAGINS.SCamp[n].CampBmp,".NEW.")!=NULL;


/*
	LocalGP FONT("interf3\\Fonts\\FontC10");
	RLCFont FontY(FONT.GPID);
	FontY.SetColorTable(3);
	LoadOptionalTable(3,"1\\orange.grd");
	RLCFont FontW(FONT.GPID);
	FontW.SetColorTable(4);
	LoadOptionalTable(4,"1\\white.grd");
*/
	DialogsSystem MMenu(0,0);
	MMenu.OkSound=GetSound("START");
	MMenu.addPicture(NULL,0,0,&Back,&Back,&Back);

	
	///////////////////////////////////////////////////////////////////////////

	// Graphics
	LocalGP SCROL("Interf3\\elements\\scroll");
	LocalGP CMB("Interf2\\Combo");
	LocalGP LGP("Interf2\\ListBox");
	LocalGP MapMask("Interf2\\GameMap");
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
	RLCFont* FMissText = &fon18y3;
	RLCFont* FHint = &SmallYellowFont;

	// Hint
	int HintX = 440;
	int HintY = 745;

	MMenu.HintFont=FHint;
	MMenu.HintX=-HintX;
	MMenu.HintY=-HintY;	

	TextButton* HintTxt = MMenu.addTextButton(NULL,HintX,HintY,"                                                                                                                                 ",FHint,FHint,FHint,1);

	//
	VScrollBar* SB=MMenu.addNewGP_VScrollBar(NULL,495,136,476,1,0,SCROL.GPID,0);
	ListBox* LB=MMenu.addGP_ListBox(NULL,94,156,10,LGP.GPID,3,26,FAList,FPList,SB);
	//LB->M_Over=11;
	LB->Active=1;
	LB->FontDy=-3;
	LB->FontDx-=16;
	
	ComboBox* DIFF=MMenu.addGP_ComboBox(NULL,213,638,CMB.GPID,0,9,0,FACombo,FPCombo,NULL);
	DIFF->CurLine=1;
	FILE* f=fopen("smiss.opt","r");
	if(f){
		int v=0;
		fscanf(f,"%d",&v);
		fclose(f);
		DIFF->CurLine=v;
	};

	DIFF->Center-=5;
	DIFF->FontDy-=2;
	DIFF->OneDy-=1;

	// Buttons
	int BInChatX = 839;
	int OkX = 899;
	int CancelX = 959;
	int ButY = 706;

	//VideoButton* OkBtn=MMenu.addVideoButton(NULL,869,718,OK1.GPID,OK2.GPID);
	//VideoButton* CancelBtn=MMenu.addVideoButton(NULL,862,668,CANCEL1.GPID,CANCEL2.GPID);
	GP_Button* OkBtn=MMenu.addGP_Button(NULL,OkX,ButY,GmBtn.GPID,0,1);
	GP_Button* CancelBtn=MMenu.addGP_Button(NULL,CancelX,ButY,GmBtn.GPID,3,4);

	OkBtn->OnUserClick=&MMItemChoose;
	OkBtn->UserParam=mcmOk;
	OkBtn->Hint=GetTextByID("MISS_RUN");
	//OkBtn->Visible=0;
		
	CancelBtn->OnUserClick=&MMItemChoose;
	CancelBtn->UserParam=mcmCancel;
	CancelBtn->Hint=GetTextByID("WAR_CANCEL");//GetTextByID("MISS_CANCEL");
	//CancelBtn->Visible=0;

	///////////////////////////////////////////////////////////////////////////

	VideoButton* ENC=MMenu.addVideoButton(NULL,862,468,ENC1.GPID,ENC2.GPID);
	ENC->UserParam=0;
	ENC->OnUserClick=&RunEnciclopia;
	ENC->Hint=GetTextByID("ENCICL");
	ENC->Visible=0;
	
	LocalGP HFONT("interf3\\Fonts\\FontC10");
	RLCFont hfnt(HFONT.GPID);
	hfnt.SetWhiteColor();

	SingleCampagin* SCM=CAMPAGINS.SCamp+n;
	TextViewer* TV[32];
	VScrollBar* TSB[32];
	// Map preview
	BPXView* PREV[32];
	SQPicture PRV[32];	
	

	/*
	VScrollBar* SB;
	if(Style)SB=MMenu.addNewGP_VScrollBar(NULL,450,157,221-52,1,0,SCROL.GPID,0);
	else SB=MMenu.addNewGP_VScrollBar(NULL,450,157,221,1,0,SCROL.GPID,0);
	for(int i=0;i<SCM->NMiss&&i<8;i++){
		if((SCM->OpenIndex[i]&255)&&i<8-Style-Style)MMenu.addGPPicture(NULL,46,166+26*i,BTNS.GPID,6);
	};
	ListBox* LB;
	if(Style)LB=MMenu.addGP_ListBox(NULL,46,166,6,BTNS.GPID,3,26,&WhiteFont,&YellowFont,SB);
	else LB=MMenu.addGP_ListBox(NULL,46,166,8,BTNS.GPID,3,26,&WhiteFont,&YellowFont,SB);
	LB->M_Over=0;
	LB->Active=1;
	LB->FontDy=-2;
	*/
	int MaxMiss=0;
	for(int i=0;i<SCM->NMiss;i++){
		if(SCM->OpenIndex[i]&255)MaxMiss=i+1;
	};

	int TextX = 605;
	int TextY = 445;
	int TextLx = 342;
	int TextLy = 226;

	bool VISMASK[64];
	memset(VISMASK,0,sizeof VISMASK);
	for(int i=0;i<MaxMiss;i++){
		//if(SCM->OpenIndex[i]&255){
		LB->AddItem(MISSLIST.MISS[SCM->Miss[i]].Name,0);
		LB->CurItem=LB->NItems-1;
		if(LB->CurItem-LB->FLItem>LB->ny){
			LB->FLItem=LB->CurItem-LB->ny+1;
			SB->SPos=LB->FLItem*32;
		};
		//};		
		if(MISSLIST.MISS[SCM->Miss[i]].NIntro&&strstr(MISSLIST.MISS[SCM->Miss[i]].Intro[0],".txt")){
			char *introfile=MISSLIST.MISS[SCM->Miss[i]].Intro[0];
			if(Style)TV[i]=MMenu.addTextViewer(NULL,TextX,TextY,TextLx,TextLy,introfile,FMissText);
				else TV[i]=MMenu.addTextViewer(NULL,TextX,TextY,TextLx,TextLy,introfile,FMissText);
			TV[i]->SymSize=21;
			TV[i]->PageSize=(TV[i]->y1-TV[i]->y)/TV[i]->SymSize;
			TV[i]->Visible=0;
			if(Style)TSB[i]=MMenu.addNewGP_VScrollBar(NULL,957,434,238,1,0,SCROL.GPID,0);
				else TSB[i]=MMenu.addNewGP_VScrollBar(NULL,957,434,238,1,0,SCROL.GPID,0);
			TSB[i]->Visible=0;
			TSB[i]->ScrDy=7;
			TV[i]->AssignScroll(TSB[i]);
						
			int ifl=strlen(introfile);
			if(ifl){
				char* mapname = (char*) malloc(ifl+1);
				strcpy(mapname,introfile);
				mapname[ifl-3] = 'b';
				mapname[ifl-2] = 'm';
				mapname[ifl-1] = 'p';				
				
				ResFile ff1=RReset(mapname);
				if(ff1!=INVALID_HANDLE_VALUE){
					PRV[i].LoadPicture(mapname);
				}
				
				free(mapname);
			}
			if(PRV[i].GetPixelData())
				PREV[i]=MMenu.addBPXView(NULL,588,120,400,224,1,1,1,(byte*)(PRV[i].GetPixelData()),NULL);
			else
				PREV[i]=NULL;
		}else{
			TV[i]=NULL;
			TSB[i]=NULL;
			PREV[i]=NULL;
		};
		VISMASK[i]=strstr(MISSLIST.MISS[SCM->Miss[i]].ID,"@")!=NULL;
	};

	// Map mask
	GPPicture* PMASK=MMenu.addGPPicture(NULL,580,115,MapMask.GPID,0);

	DIFF->AddLine(GetTextByID("@RMID_EASY"));
	DIFF->AddLine(GetTextByID("@RMID_NORMAL"));
	DIFF->AddLine(GetTextByID("@RMID_HARD"));
	DIFF->AddLine(GetTextByID("@RMID_VERYHARD"));
	DIFF->AddLine(GetTextByID("@RMID_IMPOSIBLE"));
	DIFF->Hint=GetTextByID("DIFFICULT5");

	/*
	VideoButton* OkBtn=MMenu.addVideoButton(NULL,862,568,OK1.GPID,OK2.GPID);
	OkBtn->OnUserClick=&MMItemChoose;
	OkBtn->UserParam=mcmOk;
	OkBtn->Hint=GetTextByID("MISS_RUN");
	VideoButton* CancelBtn=MMenu.addVideoButton(NULL,862,668,CANCEL1.GPID,CANCEL2.GPID);
	CancelBtn->OnUserClick=&MMItemChoose;
	CancelBtn->UserParam=mcmCancel;
	CancelBtn->Hint=GetTextByID("MISS_CANCEL");
	*/

	// Text
	MMenu.addTextButton(NULL,512,18,GetTextByID("INTF_CMISS_T0"),FScr,FScr,FScr,1);

	MMenu.addTextButton(NULL,303,107,GetTextByID("INTF_SMISS_T1"),FWin,FWin,FWin,1);
	MMenu.addTextButton(NULL,787,106,GetTextByID("INTF_SMISS_T2"),FWin,FWin,FWin,1);
	MMenu.addTextButton(NULL,787,401,GetTextByID("INTF_SMISS_T3"),FWin,FWin,FWin,1);
	
	MMenu.addTextButton(NULL,98,640,GetTextByID("INTF_SMISS_DIFF"),FDialog,FDialog,FDialog,0);

HHH1:
	ItemChoose=-1;
	int mm=1;
	KeyPressed=0;
	StartMixing();
	do{
		if(MMenu.Hint){
			strcpy(HintTxt->Message, MMenu.Hint);
			HintTxt->Visible=1;
		}else
			HintTxt->Message[0] = 0;

		ProcessMessages();
		StdKeys();
		MMenu.MarkToDraw();
		MMenu.ProcessDialogs();
		ProcessMixing();
		MMenu.RefreshView();
		for(int j=0;j<MaxMiss;j++){
			if(TV[j]){
				if(j==LB->CurItem){
					TV[j]->Visible=1;
					TSB[j]->Visible=1;
					TV[j]->Active=0;
					if(PREV[j]) PREV[j]->Visible=1;
				}else{
					TV[j]->Visible=0;
					TSB[j]->Visible=0;
					TV[j]->Active=0;
					if(PREV[j]) PREV[j]->Visible=0;
				};
			};
			if(j==LB->CurItem){
				DIFF->Visible=VISMASK[j];
				DIFF->Enabled=DIFF->Visible;
			};
		};
		LB->Active=1;
		if(mm){
			LoadPalette("1\\agew_1.pal");
			mm=0;
		};
		StdKeys();
	}while(ItemChoose==-1);
	f=fopen("smiss.opt","w");
	if(f){
		int v=0;
		fprintf(f,"%d",DIFF->CurLine);
		fclose(f);
	};
	if(ItemChoose==mcmOk){
		CurrentCampagin=n;
		CurrentMission=LB->CurItem;
		if(!ProcessSingleMission(SCM->Miss[LB->CurItem],DIFF->CurLine)){
			//SlowUnLoadPalette("2\\agew_1.pal");
			CurrentCampagin=-1;
			CurrentMission=-1;
			goto HHH1;
		}else return true;
	}else{
		CurrentCampagin=-1;
		CurrentMission=-1;
	};
	//SlowUnLoadPalette("2\\agew_1.pal");
	return false;
};