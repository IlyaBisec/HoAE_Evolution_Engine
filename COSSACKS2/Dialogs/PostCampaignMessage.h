bool PostCampaignMessage(int n){

	SingleMission* SMS=MISSLIST.MISS+n;

	if(SMS->NIntro>4){
		// Graphics
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
		RLCFont* FMissText = &fon16y5;
		RLCFont* FHint = &SmallYellowFont;

		// Dialog
		SQPicture Back("Interf2\\Mission_Loading.bmp");
		DialogsSystem MISS(0,0);
		MISS.addPicture(NULL,0,0,&Back,&Back,&Back);

		// Hint
		int HintX = 470;
		int HintY = 745;

		MISS.HintFont=FHint;
		MISS.HintX=HintX;
		MISS.HintY=HintY;

		// Picture
		LocalGP PIC("Interf2\\MissPreview");
		int pid=atoi(SMS->Intro[3]);
		MISS.addGPPicture(NULL, 245, 135, PIC.GPID, pid);

		// Text	
		char STEXT[128];
		char MP3[128];
		ParseMP3_String(SMS->Intro[4],STEXT,MP3);
		MISS.addClipper(0,465,RealLx,597);
		TextViewer* TV=MISS.addTextViewer(NULL,337,465,350,132,STEXT,FMissText);
		MISS.addClipper(0,0,RealLx,RealLy);
		TV->SymSize=17;
		TV->PageSize=480;

		int TextLy=TV->NLines*TV->SymSize-130;

		// Buttons
		int BInChatX = 839;
		int OkX = 899;
		int CancelX = 959;
		int ButY = 706;

		GP_Button* OkBtn=MISS.addGP_Button(NULL,CancelX,ButY,GmBtn.GPID,0,1);

		OkBtn->OnUserClick=&MMItemChoose;
		OkBtn->UserParam=mcmOk;
		OkBtn->Hint=GetTextByID("INTF_BACK_CAMP_MENU");

		// Text
		MISS.addTextButton(NULL,512,18,SMS->Name,FScr,FScr,FScr,1);		

		ItemChoose=-1;
		KeyPressed=0;
		int CurTime=GetTickCount();
		
		int StartScrol=CurTime+7000;
		int ExitPreview=CurTime+1000000;
		
		bool scroll=true;

		int NextStep=CurTime;
		//if(MP3[0])PlayMP3File(MP3);
		char MP30[64];
		strcpy(MP30,MP3);

		int T=2500/TV->SymSize;
		int TextLength=TextLy;
		if(!TextLength) TextLength=1;
		DWORD SoundLength=0;
		ShowLoading();
		FlipPages();
		if(MP3[0]){
			SetCDVolume(3);
			int T0=GetTickCount();
			do{
				Sleep(50);
				//ProcessMessages();
			}while(GetTickCount()-T0<1000);
			extern bool RandomMode;
			int R=RandomMode;
			RandomMode=0;
			PlayMP3File(MP3);
			SoundLength=0;
			T0=GetTickCount();
			do{
				Sleep(10);
				ProcessMessages();
				ProcessMP3(1);
				ProcessMP3(1);
				ProcessMP3(1);
				ProcessMP3(1);
				SoundLength=ov_GetStreamLength();
				if(SoundLength){
					if(SoundLength<15)SoundLength=15;
					SetCDVolume(GSets.SVOpt.MusicVolume);
					T=(SoundLength-14)*1000/TextLength;
					MP3[0]=0;
				};
			}while(SoundLength==0&&GetTickCount()-T0<20000);
			RandomMode=R;
		};
		
		StartMixing();
		do{
			if(KeyPressed&&LastKey==27)ItemChoose=mcmOk;

			ProcessMessages();
			StdKeys();
			MISS.MarkToDraw();
			MISS.ProcessDialogs();
			ProcessMixing();
			MISS.RefreshView();

			CurTime=GetTickCount();
			if(scroll){
				if(TextLy<0){
					ExitPreview=CurTime+6000;
					scroll=false;
					if(CurTime>ExitPreview) ItemChoose=mcmOk;
				}else{
					if(CurTime>StartScrol&&CurTime>NextStep){
						TextLy-=1;
						TV->y-=1;
						NextStep=CurTime+T;
					}
				}
			}
			
			
		}while(ItemChoose==-1);
		PlayMP3File("");
		return true;
	};
	return false;	
}