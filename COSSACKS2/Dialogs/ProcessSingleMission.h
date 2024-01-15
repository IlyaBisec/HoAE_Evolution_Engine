
// MANOWAR
#include "mp3\oggvor.h"

void ClearCompstart();
void  UnloadBeforeMission(){
	memset(COMPSTART,0,16);
	SetMyNation(0);
	GSets.CGame.PL_INFO[0].ColorID=0;
	for(int i=0;i<8;i++)GSets.CGame.PL_INFO[i].GroupID=0;
	GSets.CGame.UnitTransform=0;
}
void RunMission(int n,int Diff){
	//UnloadBeforeMission();	
	//
	strcpy(GSets.CGame.cgi_CurrentMap,MISSLIST.MISS[n].MapName);
	CreateNationalMaskForMap(GSets.CGame.cgi_CurrentMap);
	for(int i=0;i<7;i++){
		CITY[i].Difficulty=Diff;
	};	
	PrepareGameMedia(0,0,1);
	InitGame();
	ItemChoose=mcmSingle;
	//
	extern int StartGTime;
	extern int StartVCond;
	StartGTime=0;
	StartVCond=0;
	MISSLIST.CurrentMission=n;
	SCENINF.Load(MISSLIST.MISS[n].DLLPath,MISSLIST.MISS[n].Description);
	UnPress();	
	RunMethod=0xFFFFFFFF;//0x12340000+n+(Diff<<8);		
	//
	extern byte vmDifficulty[8];
	for(int i=0;i<8;i++){		
		vmDifficulty[i]=Diff;
	}	

}
//extern HRESULT mpl_GetStreamLength(DWORD &dwLength);
bool ProcessSingleMission(int n,int Diff){
	MaxPingTime=0;
	SingleMission* SMS=MISSLIST.MISS+n;

	KeyPressed=0;
	LastKey=0;

	/*
	for(int i=0;i<SMS->NIntro;i++){
		if(!strstr(SMS->Intro[i],".txt")){
			SQPicture Pan1(SMS->Intro[i]);
			SlowUnLoadPalette("1\\agew_1.pal");
			Pan1.Draw(0,0);
			FlipPages();
			SlowLoadPalette("1\\agew_1.pal");
			int t0=GetRealTime();
			KeyPressed=0;
			LastKey=0;
			Lpressed=false;
			do{
				ProcessMessages();
			}while(!(GetRealTime()-t0>15000||Lpressed||KeyPressed));
			Lpressed=false;
			if(LastKey==27)return false;
		};
	};
	*/

	if(SMS->NIntro>2){		
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

		SQPicture Back("Interf2\\Mission_Loading.bmp");
		DialogsSystem MISS(0,0);
		MISS.addPicture(NULL,0,0,&Back,&Back,&Back);

		// Description

		// Text
		int did;		
		if(SMS->NIntro>=3) did=2;
			else did=0;
		char STEXT[128];
		char MP3[128];
		ParseMP3_String(SMS->Intro[did],STEXT,MP3);

		ResFile f=RReset(STEXT);
		if(f!=INVALID_HANDLE_VALUE){
			byte crlf[6]={0x20,0x0D,0x0A,0x0D,0x0A};
			int sz=RFileSize(f);
			byte* buf=znew(byte,sz);
			RBlockRead(f,buf,sz);
			RClose(f);
			f=RRewrite("tmp.txt");
			RBlockWrite(f,crlf,sizeof crlf);
			RBlockWrite(f,buf,sz);
			RBlockWrite(f,crlf,sizeof crlf);
			RClose(f);
			free(buf);
		};
		MISS.addClipper(0,465,RealLx,620/*597*/);
		TextViewer* TV=MISS.addTextViewer(NULL,337,465+2,350,132,"tmp.txt",FMissText);

		// mask
		LocalGP PIC_mask("Interf2\\Miss_pr_mask");
		MISS.addGPPicture(NULL,297,443,PIC_mask.GPID,0);

		MISS.addClipper(0,0,RealLx,RealLy);
		
		// Picture
		LocalGP PIC("Interf2\\MissPreview");
		int pid;
		if(SMS->NIntro<2) pid=0;
			else pid=atoi(SMS->Intro[1]);
		MISS.addGPPicture(NULL,245,135,PIC.GPID,pid);

		
		TV->SymSize=17;
		TV->PageSize=480;	
		TV->Active=0;
		TV->Enabled=0;

		int TextLy=TV->NLines*TV->SymSize-130;
		
		// Interface
		int HintX = 470;
		int HintY = 745;

		MISS.HintFont=FHint;
		MISS.HintX=HintX;
		MISS.HintY=HintY;

		// Buttons
		int BInChatX = 839;
		int OkX = 899;
		int CancelX = 959;
		int ButY = 706;

		GP_Button* OkBtn=MISS.addGP_Button(NULL,CancelX,ButY,GmBtn.GPID,0,1);

		OkBtn->OnUserClick=&MMItemChoose;
		OkBtn->UserParam=mcmOk;
		OkBtn->Hint=GetTextByID("MISS_RUN");

		// Text
		MISS.addTextButton(NULL,512,18,SMS->Name,FScr,FScr,FScr,1);		

		KeyPressed=0;

		ItemChoose=-1;

		int CurTime=GetTickCount();
		
		int StartScrol=CurTime+7000;
		int ExitPreview=CurTime+1000000;
		
		bool scroll=true;

		int NextStep=CurTime;
		
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
					T=((SoundLength-8)*1000/TextLength);
					T=T*26/27;
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
					ExitPreview=CurTime+8000;
					scroll=false;					
				}else{
					if(CurTime>StartScrol&&CurTime>NextStep){
						TextLy-=1;
						TV->y-=1;
						NextStep=CurTime+T;
					}
				}
			}
			
			if(CurTime>ExitPreview) ItemChoose=mcmOk;
		}while(ItemChoose==-1);
		PlayMP3File("");
	}

	if(SMS->NIntro==-1){
		ResFile f=RReset(SMS->Intro[2]);
		if(f!=INVALID_HANDLE_VALUE){
			byte crlf[6]={0x20,0x0D,0x0A,0x0D,0x0A};
			int sz=RFileSize(f);
			byte* buf=znew(byte,sz);
			RBlockRead(f,buf,sz);
			RClose(f);
			f=RRewrite("tmp.txt");
			RBlockWrite(f,crlf,sizeof crlf);
			RBlockWrite(f,buf,sz);
			RBlockWrite(f,crlf,sizeof crlf);
			RClose(f);
			free(buf);
		};
		LoadOptionalTable(5,"2\\dark.grd");
		LocalGP BTNS("Interface\\Mission_text");
		LocalGP PIC(SMS->Intro[1]);
		LocalGP ITL("it_Small");
		RLCFont itsm(ITL.GPID);
		//itsm.SetColorTable(5);
		LocalGP HDR("interf3\\Fonts\\FontC14");
		RLCFont hdrf(HDR.GPID);
		//hdrf.SetColorTable(5);
		SQPicture Back("Interface\\Background_Mission_Text.bmp");
		DialogsSystem MISS(0,0);
		MISS.addPicture(NULL,0,0,&Back,&Back,&Back);
		MISS.addGPPicture(NULL,48,119,PIC.GPID,0);
		//GPS.SetOptionalColor(PIC.GPID,5);
		//GPS.SetOptionalColor(PIC.GPID,2);
		//Back.Draw(0,0);
		//GPS.ShowGP(48,119,
		TextViewer* TV=MISS.addTextViewer(NULL,532,258,445,424,"tmp.txt",&itsm);
		TV->SymSize=25;
		TV->PageSize=18;
		TV->Active=0;
		TV->Enabled=0;

		MISS.addGPPicture(NULL,522,258-25,BTNS.GPID,2);
		MISS.addGPPicture(NULL,522+233,258-25,BTNS.GPID,3);
		MISS.addGPPicture(NULL,522,656,BTNS.GPID,0);
		MISS.addGPPicture(NULL,522+233,656,BTNS.GPID,1);
		MISS.addTextButton(NULL,522,204,SMS->Name,&hdrf,&hdrf,&hdrf,0);
		int z=0;
		sscanf(SMS->Intro[0],"%d",&z);
		int LX=GPS.GetGPWidth(BTNS.GPID,8+z);
		MISS.addGPPicture(NULL,512-LX/2,7,BTNS.GPID,8+z);
		MISS.addGPPicture(NULL,512-LX/2-425,7,BTNS.GPID,6);
		MISS.addGPPicture(NULL,512+LX/2,7,BTNS.GPID,7);
		char* tex=GetTextByID(SMS->Intro[3]);
		LX=GetRLCStrWidth(tex,&YellowFont);
		MISS.addGPPicture(NULL,512-LX/2-502,736,BTNS.GPID,4);
		MISS.addGPPicture(NULL,512+LX/2,736,BTNS.GPID,5);
		MISS.addTextButton(NULL,512,736+3,tex,&YellowFont,&YellowFont,&YellowFont,1);

		int LinesUp=0;
		int LinesDn=0;
		int ShiftY=0;
		bool exit=false;
		do{
			if(KeyPressed&&LastKey==27)exit=true;
			if(Lpressed||(KeyPressed&&(LastKey==34||LastKey==13||LastKey==' '))){
				LinesUp=8;
				LinesDn=0;
				KeyPressed=0;
				if(TV->NLines-TV->Line<16){
					if(!ShiftY)LinesUp=0;
					else LinesUp=1;
					if(Lpressed)exit=true;
				};
			};
			if(KeyPressed&&LastKey==33){
				LinesUp=0;
				LinesDn=9;
				KeyPressed=0;
			};
			if(LinesUp){
				if(ShiftY<25){
					switch(ShiftY){
					case 0:
						ShiftY=9;
						break;
					case 9:
						ShiftY=9+8;
						break;
					case 9+8:
						ShiftY=25;
						break;
					};
				}else{
					TV->Line++;
					LinesUp--;
					if(!TV->Line)LinesUp=0;
					ShiftY=0;
				};
			};
			if(LinesDn){
				if(!ShiftY){
					if(TV->Line){
						TV->Line--;
						ShiftY=9+8;
						LinesDn--;
					}else LinesDn=0;
				}else{
					switch(ShiftY){
					case 9+8:
						ShiftY=9;
						break;
					case 9:
						ShiftY=0;
						break;
					};
				};
			};
			TV->y=258-ShiftY;
			ProcessMessages();
			MISS.MarkToDraw();
			MISS.ProcessDialogs();
			MISS.RefreshView();
		}while(!exit);
	};
	//ShowTable(GetTextByID("LOADING"),GetTextByID("WAITPL"));
	RunMission(n,Diff);
	return true;
};