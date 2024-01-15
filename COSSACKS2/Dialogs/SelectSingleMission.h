bool SelectSingleMission(){
	if(!MISSLIST.MSMiss)return false;
	KeyPressed=0;
	LastKey=0;

	

	//SlowUnLoadPalette("1\\agew_1.pal");
	AddMissionsPack AMSP;
	//SQPicture Back("Interface\\Single_Mission_Background.bmp");
	SQPicture Back("Interf2\\Single_Mission_Background.bmp");

	LocalGP SCROL("Interf3\\elements\\scroll");
	LocalGP CMB("Interf2\\Combo");
	LocalGP LGP("Interf2\\ListBox");
	LocalGP MapMask("Interf2\\GameMap");
	LocalGP GmBtn("Interf2\\GameButtons");


	LocalGP BTNS("Interface\\Single_mission");
	LocalGP OK1("Interface\\anm_start_1_on");
	LocalGP OK2("Interface\\anm_start_1_off");
	LocalGP CANCEL1("Interface\\anm_cancel_on");
	LocalGP CANCEL2("Interface\\anm_cancel_off");
	LocalGP ENC1("Interface\\anm_enc_on");
	LocalGP ENC2("Interface\\anm_enc_off");	
	LocalGP MASKA("Interface\\preview_mask");
	LoadFog(1);	

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

	DialogsSystem MMenu(0,0);
	MMenu.addPicture(NULL,0,0,&Back,&Back,&Back);
	
	/*
	VideoButton* ENC=MMenu.addVideoButton(NULL,862,468,ENC1.GPID,ENC2.GPID);
	ENC->UserParam=0;
	ENC->OnUserClick=&RunEnciclopia;
	ENC->Hint=GetTextByID("ENCICL");
	ENC->Visible=0;
	*/

	SQPicture PRV("Maps2\\Single1.bmp");
	//MMenu.addClipper(588,120,588+198,120+221);
	BPXView* PREV=MMenu.addBPXView(NULL,588,120,400,224/*292,192*/,1,1,1,(byte*)(PRV.GetPixelData())/*Preview+4*/,NULL);
	//MMenu.addClipper(0,0,RealLx,RealLy);
	//PREV->Visible=false;
	GPPicture* PMASK=MMenu.addGPPicture(NULL,580,115,MapMask.GPID,0);
	//PMASK->Visible=false;

	LocalGP HFONT("interf3\\Fonts\\FontC10");
	RLCFont hfnt(HFONT.GPID);
	hfnt.SetWhiteColor();

	int HintX = 440;
	int HintY = 745;

	MMenu.HintFont=FHint;
	MMenu.HintX=-HintX;
	MMenu.HintY=-HintY;

	TextButton* HintTxt = MMenu.addTextButton(NULL,HintX,HintY,"                                                                                               ",FHint,FHint,FHint,1);

	VScrollBar* SB=MMenu.addNewGP_VScrollBar(NULL,495,136,476,1,0,SCROL.GPID,0);
	ListBox* LB=MMenu.addGP_ListBox(NULL,94,156,17,LGP.GPID,3,26,FAList,FPList,SB);
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

	DIFF->AddLine(GetTextByID("@RMID_EASY"));
	DIFF->AddLine(GetTextByID("@RMID_NORMAL"));
	DIFF->AddLine(GetTextByID("@RMID_HARD"));
	DIFF->AddLine(GetTextByID("@RMID_VERYHARD"));
	DIFF->AddLine(GetTextByID("@RMID_IMPOSIBLE"));
	DIFF->Hint=GetTextByID("DIFFICULT5");

	int TextX = 605;
	int TextY = 445;
	int TextLx = 342;
	int TextLy = 226;

	TextViewer* TV[128];
	VScrollBar* TSB[128];
	for(int i=0;i<MISSLIST.MSMiss;i++){
		LB->AddItem(MISSLIST.MISS[MISSLIST.SingleMS[i]].Name,0);
		if(MISSLIST.MISS[MISSLIST.SingleMS[i]].NIntro&&strstr(MISSLIST.MISS[MISSLIST.SingleMS[i]].Intro[0],".txt")){
			TV[i]=MMenu.addTextViewer(NULL,TextX,TextY,TextLx,TextLy,MISSLIST.MISS[MISSLIST.SingleMS[i]].Intro[0],FMissText);
			TV[i]->SymSize=21;
			TV[i]->PageSize=(TV[i]->y1-TV[i]->y)/TV[i]->SymSize;
			TV[i]->Visible=0;
			TSB[i]=MMenu.addNewGP_VScrollBar(NULL,957,434,238,1,0,SCROL.GPID,0);
			TSB[i]->Visible=0;
			TSB[i]->ScrDy=7;
			TV[i]->AssignScroll(TSB[i]);
		}else{
			TV[i]=NULL;
			TSB[i]=NULL;
		};
	};
	int N0=MISSLIST.MSMiss;
	for(int i=0;i<AMSP.NMiss;i++){
		LB->AddItem(AMSP.Pack[i].Title,0);
		TV[N0+i]=MMenu.addTextViewer(NULL,TextX,TextY,TextLx,TextLy,AMSP.Pack[i].Preview,FMissText);
		TV[N0+i]->SymSize=21;
		TV[N0+i]->PageSize=(TV[i]->y1-TV[i]->y)/TV[i]->SymSize;
		TV[N0+i]->Visible=0;
		TSB[N0+i]=MMenu.addNewGP_VScrollBar(NULL,957,434,238,1,0,SCROL.GPID,0);
		TSB[N0+i]->Visible=0;
		TSB[N0+i]->ScrDy=7;
		TV[N0+i]->AssignScroll(TSB[i]);
	};
	
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
		
	CancelBtn->OnUserClick=&MMItemChoose;
	CancelBtn->UserParam=mcmCancel;
	CancelBtn->Hint=GetTextByID("SOCANCEL");//GetTextByID("MISS_CANCEL");

	// Text
	MMenu.addTextButton(NULL,512,18,GetTextByID("INTF_SMISS_T0"),FScr,FScr,FScr,1);

	MMenu.addTextButton(NULL,303,107,GetTextByID("INTF_SMISS_T1"),FWin,FWin,FWin,1);
	MMenu.addTextButton(NULL,787,106,GetTextByID("INTF_SMISS_T2"),FWin,FWin,FWin,1);
	MMenu.addTextButton(NULL,787,401,GetTextByID("INTF_SMISS_T3"),FWin,FWin,FWin,1);
	
	MMenu.addTextButton(NULL,98,640,GetTextByID("INTF_SMISS_DIFF"),FDialog,FDialog,FDialog,0);

	
	int prmiss=-1;
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
		
		if(prmiss!=LB->CurItem){
			//if(GetPreview(MISSLIST.MISS[MISSLIST.SingleMS[LB->CurItem]].MapName,Preview+4)){
			//	PREV->Visible=1;
			//	Decode(Preview+4,XLAT,292*190);
			//}else{
			//	PREV->Visible=false;
			//};
			if(LB->CurItem>=N0){
				char ccc[200];
				strcpy(ccc,AMSP.Pack[LB->CurItem-N0].Bitmap);
				ResFile ff1=RReset(ccc);
				if(ff1!=INVALID_HANDLE_VALUE){
					PRV.LoadPicture(ccc);
					PREV->Ptr=(byte*)(PRV.GetPixelData());
				}else{
					//PREV->Ptr=NULL;
					PREV->Visible=0;
				}
				prmiss=LB->CurItem;
			}else{
				char ccc[200];
				sprintf(ccc,"Maps2\\single%d.bmp",LB->CurItem+1);
				ResFile ff1=RReset(ccc);
				if(ff1!=INVALID_HANDLE_VALUE){
					PRV.LoadPicture(ccc);
					PREV->Ptr=(byte*)(PRV.GetPixelData());
					PREV->Visible=1;
				}else{
					//PREV->Ptr=NULL;
					PREV->Visible=0;
				}
				prmiss=LB->CurItem;
			};
		};
		ProcessMessages();
		StdKeys();
		MMenu.MarkToDraw();
		MMenu.ProcessDialogs();
		ProcessMixing();
		MMenu.RefreshView();		
		
		for(int j=0;j<MISSLIST.MSMiss+AMSP.NMiss;j++){
			if(TV[j]){
				if(j==LB->CurItem){
					TV[j]->Visible=1;
					TSB[j]->Visible=1;
					TV[j]->Active=0;
				}else{
					TV[j]->Visible=0;
					TSB[j]->Visible=0;
					TV[j]->Active=0;
				};
			};
		};
		LB->Active=1;
		if(mm){
//			SlowLoadPalette("1\\agew_1.pal");
			mm=0;
		};
		StdKeys();
	}while(ItemChoose==-1);
	f=fopen("smiss.opt","w");
	if(f){
		fprintf(f,"%d",DIFF->CurLine);
		fclose(f);
	};
	if(ItemChoose==mcmOk){
		CurrentCampagin=-1;
		CurrentMission=-1;
		if(LB->CurItem<N0){
			if(!ProcessSingleMission(MISSLIST.SingleMS[LB->CurItem],DIFF->CurLine)){
//				SlowUnLoadPalette("1\\agew_1.pal");
				CurrentCampagin=-1;
				CurrentMission=-1;
				goto HHH1;
			}else return true;
		}else{
			void ClearCompstart();
			ClearCompstart();
			ShowLoading();
			ItemChoose=mcmSingle;
			strcpy(GSets.CGame.cgi_CurrentMap,AMSP.Pack[LB->CurItem-N0].Map);
			CreateNationalMaskForMap(GSets.CGame.cgi_CurrentMap);
			PrepareGameMedia(0,0);
			InitGame();
			for(int i=0;i<7;i++){
				CITY[i].Difficulty=DIFF->CurLine;
				GSets.CGame.PL_INFO[i].ColorID=i;
				if(i)strcpy(GSets.CGame.PL_INFO[i].name,"???");
			};
			SetMyNation(0);
			strcpy(GSets.CGame.PL_INFO[0].name,PlName);
			GSets.CGame.PL_INFO[0].GameTime=0;
			GSets.CGame.PL_INFO[0].VictCond=0;
			extern int StartGTime;
			extern int StartVCond;
			StartGTime=0;
			StartVCond=0;
			
		};
//		SlowUnLoadPalette("1\\agew_1.pal");
	}else{
		CurrentCampagin=-1;
		CurrentMission=-1;
//		SlowUnLoadPalette("1\\agew_1.pal");		
	};
	
	return false;
};