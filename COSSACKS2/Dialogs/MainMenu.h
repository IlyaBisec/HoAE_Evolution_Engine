//#define AC_CLUB
//#define TESTVERSION

int processSingleMenu(DialogsSystem *Menu);
int processMultiMenu(DialogsSystem *Menu);

CIMPORT void CloseBattle();
#include "ClassEditor.h"
int processMainMenu()
{
	{		
		void ProcessDialogsEditor();
		//ProcessDialogsEditor();
		//ClassEditor CE;
		//ClassEditor CE2;
		//BaseClass* PrevBC=NULL;
		//void* PrevData=NULL;
		//xmlQuote xml;
		//xml.ReadFromFile("MainMenu.xml");
		//DialogsSystem MMenu(0,0);
		//ErrorPager Error;
		//MMenu.Load(xml,&MMenu,&Error);
		//DialogsDesk* DD1=MMenu.AddDialogsDesk(100,100,300,400,"StandartBorder");
		//DialogsDesk* DD2=DD1->AddDialogsDesk(10,10,190,600,"StandartBorder");
		//DialogsDesk* DD3=DD2->AddDialogsDesk(10,10,140,900,"StandartBorder");
		//CE.CreateFromClass(30,200,500,200+200,&MMenu,CED_TREE);
		//CE2.CreateFromClass(30,200+200+10,500,600,&MMenu,CED_MEMBERS);
		//PrevBC=CE.GetCurrentClass();
		//PrevData=CE.GetCurrentSelectedPtr();
		////MMenu.SetEditMode(1);
		//do{
		//	IRS->ClearDeviceTarget();
		//	if(CE.GetCurrentClass()!=PrevBC||CE.GetCurrentSelectedPtr()!=PrevData){
		//		PrevBC=CE.GetCurrentClass();
		//		PrevData=CE.GetCurrentSelectedPtr();
		//		if(PrevData){
  //                  CE2.ClearAll();
		//			if(PrevBC->CheckDirectCasting()){
		//				BaseClass* BC=(BaseClass*)PrevData;
		//				CE2.CreateFromClass(30,200+200+10,500,600,BC,CED_MEMBERS);
		//			}
		//		}else CE2.ClearAll();
		//	}
		//	ProcessMessages();
		//	MMenu.ProcessDialogs();
		//	CE.Process();
		//	CE2.Process();
		//	MMenu.RefreshView();
		//}while(LastKey!=27);	
	}
#ifdef AMERICANDEMO
	GlobalAI.NAi=2;
#endif 
	//MPL_ChooseConnection();
	//MPL_WaitingGame(1,1);
	//processMultiplayer();
	//return mcmExit;
	
	SetRLCWindow(0,0,RealLx,RSCRSizeY,SCRSizeX);
	CBar(0,0,RealLx,RSCRSizeY,0);
	
	LoadFog(1);
	LoadPalette("1\\agew_1.pal");
	

	// Controls
	LocalGP BTN("Interf2\\Buttons");

	LocalGP BTNS("Interface\\Main_Menu");
	LocalGP HFONT("interf3\\Fonts\\FontC10");
	RLCFont hfnt(HFONT.GPID);
	hfnt.SetWhiteColor();

	// Fonts
	RLCFont *FPMain = &fonG60y5;
	RLCFont *FAMain = &fonG60w;

	// Menu
	DialogsSystem MMenu(0,0);
	
	// Main Background
	//SQPicture MnPanel("Interface\\Background_Main_Menu.bmp");
	SQPicture MnPanel("Interf2\\Background_Main_Menu.bmp");	
	Picture* PIC=MMenu.addPicture(NULL,0,0,&MnPanel,&MnPanel,&MnPanel);

	// Hint
	MMenu.HintFont=&SmallWhiteFont;
	MMenu.HintY=701;
	MMenu.HintX=18;	

	// Main menu pictures
	int MX = 510;	
	int MY = 475;

	int SDX = 340;
	int SDY = -250;

	int MDX = -340;
	int MDY = -250;
	
	// Single player
	int SinX = MX+SDX-20;
	int SinY = MY+SDY+38;
	int SindY = 35;
#if defined(INTF_AC_ADD)
	SinY+=SindY/2;
#endif

	// Multiplayer
	int MulX = MX+MDX+10;
	int MulY = MY+MDY+38;
	int MuldY = 35;
	
	// Main menu buttons
	int MainX = MX;
	int MainY = MY;
	int MaindY = 35;

	LocalGP MGP("Interf2\\MainMenu");

	// for Text align
	//SQPicture MenuBack("Interf2\\map_template.bmp");
	//Picture* MBackTemp=MMenu.addPicture(NULL,0,210,&MenuBack,&MenuBack,&MenuBack);


	MMenu.addClipper(342,464,676,707); // Obrezka lishney chasti main menu

	// Main menu back
	GPPicture* MBack[4];
	for(int i=0; i<4; i++) MBack[i]=MMenu.addGPPicture(NULL,i*255,210,MGP.GPID,i+1);

	// Main
	//GP_Button* Single=MMenu.addGP_Button(NULL,76,140,BTNS.GPID,0,1);
	//GP_TextButton* Single=MMenu.addGP_TextButton(NULL,MX,MY,GetTextByID("INTF_MAIN_0"),BTN.GPID,0,FAMain,FPMain);
	TextButton* Single=MMenu.addTextButton(NULL,MainX,MainY,GetTextByID("INTF_MAIN_0"),FAMain,FPMain,FPMain,1);
	TextButton* Multi=MMenu.addTextButton(NULL,MainX,MainY+MaindY,GetTextByID("INTF_MAIN_1"),FAMain,FPMain,FPMain,1);
	TextButton* Load=MMenu.addTextButton(NULL,MainX,MainY+MaindY*2,GetTextByID("INTF_MAIN_2"),FAMain,FPMain,FPMain,1);
	TextButton* Options=MMenu.addTextButton(NULL,MainX,MainY+MaindY*3,GetTextByID("INTF_MAIN_3"),FAMain,FPMain,FPMain,1);
	TextButton* Video=MMenu.addTextButton(NULL,MainX,MainY+MaindY*4,GetTextByID("INTF_MAIN_4"),FAMain,FPMain,FPMain,1);
	TextButton* Exit=MMenu.addTextButton(NULL,MainX,MainY+MaindY*5,GetTextByID("INTF_MAIN_5"),FAMain,FPMain,FPMain,1);
	
	int InetY=MulY+MuldY*0;
	int DeatY=MulY+MuldY*1;
	int BattY=MulY+MuldY*2;

#if defined(INTF_AC_ADD)
	InetY=MulY+MuldY*1;
	DeatY=MulY+MuldY*2;
	BattY=MulY+MuldY*0;
#endif

	// Multiplayer
	TextButton* INetGame=MMenu.addTextButton(NULL,MulX,InetY,GetTextByID("INTF_PROT_B3"),FAMain,FPMain,FPMain,1);
	TextButton* DeathM=MMenu.addTextButton(NULL,MulX,DeatY,GetTextByID("INTF_MAIN_M0"),FAMain,FPMain,FPMain,1);
	TextButton* HistBatt=MMenu.addTextButton(NULL,MulX,BattY,GetTextByID("INTF_MAIN_M1"),FAMain,FPMain,FPMain,1);
	TextButton* Back2=MMenu.addTextButton(NULL,MulX,MulY+MuldY*3,GetTextByID("INTF_MAIN_B"),FAMain,FPMain,FPMain,1);

	// Single
#if defined(INTF_AC_ADD)
	TextButton* SinBattle=MMenu.addTextButton(NULL,SinX,SinY-SindY,GetTextByID("INTF_MAIN_S3"),FAMain,FPMain,FPMain,1);
#endif
	TextButton* Compagin=MMenu.addTextButton(NULL,SinX,SinY,GetTextByID("INTF_MAIN_S0"),FAMain,FPMain,FPMain,1);
	TextButton* Mission=MMenu.addTextButton(NULL,SinX,SinY+SindY,GetTextByID("INTF_MAIN_S1"),FAMain,FPMain,FPMain,1);
	TextButton* Random=MMenu.addTextButton(NULL,SinX,SinY+SindY*2,GetTextByID("INTF_MAIN_S2"),FAMain,FPMain,FPMain,1);
	TextButton* Back=MMenu.addTextButton(NULL,SinX,SinY+SindY*3,GetTextByID("INTF_MAIN_B"),FAMain,FPMain,FPMain,1);

	MMenu.addClipper(0,0,RealLx,RealLy);

	//Menu mask	
	GPPicture* MMask=MMenu.addGPPicture(NULL,339,458,MGP.GPID,0);


	// Single player buttons
	Single->UserParam=mcmSingle;
#ifndef INETTESTVERSION
#ifndef EDITORONLY
	Single->OnUserClick=&MMItemChoose;
#endif
#endif
	//Single->Hint=GetTextByID("MMSINGLE");
	Single->AssignSound(GETS("@MOUSESOUND"),MOUSE_SOUND);
	
	Multi->UserParam=mcmMulti;
#ifndef EDITORONLY
	Multi->OnUserClick=&MMItemChoose;
#endif
	//Multi->Hint=GetTextByID("MMMULTI");
	Multi->AssignSound(GETS("@MOUSESOUND"),MOUSE_SOUND);
	
	Load->UserParam=mcmLoad;
#ifndef EDITORONLY
	Load->OnUserClick=&MMItemChoose;
#endif
	//Load->Hint=GetTextByID("MMLOAD");
	Load->AssignSound(GETS("@MOUSESOUND"),MOUSE_SOUND);

	Options->UserParam=mcmEdit;
	Options->OnUserClick=&MMItemChoose;
	//Options->Hint=GetTextByID("MMOPTIONS");
	Options->AssignSound(GETS("@MOUSESOUND"),MOUSE_SOUND);	
	
	Video->UserParam=mcmVideo;
#ifndef AC_BATTLE_ONLY
#ifndef INETTESTVERSION
#ifndef PUBLICDEMO
#ifndef AMERICANDEMO
	Video->OnUserClick=&MMItemChoose;
#endif
#endif
#endif
#endif

	//Video->Hint=GetTextByID("MMVIDEO");
	Video->AssignSound(GETS("@MOUSESOUND"),MOUSE_SOUND);
	
	Exit->UserParam=mcmExit;
	Exit->OnUserClick=&MMItemChoose;
	//Exit->Hint=GetTextByID("MMEXIT");
	Exit->AssignSound(GETS("@MOUSESOUND"),MOUSE_SOUND);

// ---
#ifdef INTF_AC_ADD
	// Single battle
	SinBattle->UserParam=0xE0;
	SinBattle->OnUserClick=&MMItemChoose;
	SinBattle->AssignSound(GETS("@MOUSESOUND"),MOUSE_SOUND);
#endif
// ---
	Compagin->UserParam=-1;
#ifndef AMERICANDEMO
#ifndef INETTESTVERSION
#ifndef AC_BATTLE_ONLY
#ifndef AC_DEMO
#ifndef PUBLICDEMO
	Compagin->UserParam=1;
	Compagin->OnUserClick=&MMItemChoose;
#endif
#endif
#endif
#endif
#endif
	//Compagin->Hint=GetTextByID("SCOMPAGIN");
	Compagin->AssignSound(GETS("@MOUSESOUND"),MOUSE_SOUND);

	Mission->UserParam=-1;
//#ifndef AC_BATTLE_ONLY
#ifndef INETTESTVERSION
#ifndef AMERICANDEMO
	Mission->UserParam=2;
	Mission->OnUserClick=&MMItemChoose;
#endif
#endif
//#endif
	//Mission->Hint=GetTextByID("SMISSION");
	Mission->AssignSound(GETS("@MOUSESOUND"),MOUSE_SOUND);

	Random->UserParam=-1;
#ifndef AC_BATTLE_ONLY
#ifndef AC_DEMO
#ifndef PUBLICDEMO
	Random->UserParam=3;
	Random->OnUserClick=&MMItemChoose;
#endif
#endif
#endif
	//Random->Hint=GetTextByID("SRANDOM");
	Random->AssignSound(GETS("@MOUSESOUND"),MOUSE_SOUND);
		
	Back->UserParam=5;
	Back->OnUserClick=&MMItemChoose;
	//Back->Hint=GetTextByID("SBACK");
	Back->AssignSound(GETS("@MOUSESOUND"),MOUSE_SOUND);

	// Multiplayer menu buttons	
	INetGame->UserParam=3;
	INetGame->OnUserClick=&MMItemChoose;
	//INetGame->Hint=GetTextByID("MDEATHM");
	INetGame->AssignSound(GETS("@MOUSESOUND"),MOUSE_SOUND);

	DeathM->UserParam=-1;
#ifndef AC_BATTLE_ONLY
	DeathM->UserParam=1;
	DeathM->OnUserClick=&MMItemChoose;
#endif
	//DeathM->Hint=GetTextByID("MDEATHM");
	DeathM->AssignSound(GETS("@MOUSESOUND"),MOUSE_SOUND);

	HistBatt->UserParam=-1;
#ifndef AC_DEMO
#ifndef PUBLICDEMO
#ifndef AMERICANDEMO
	HistBatt->UserParam=2;
	HistBatt->OnUserClick=&MMItemChoose;
#endif
#endif
#endif
	//HistBatt->Hint=GetTextByID("MHISTBATT");
	HistBatt->AssignSound(GETS("@MOUSESOUND"),MOUSE_SOUND);

	Back2->UserParam=5;
	Back2->OnUserClick=&MMItemChoose;
	//Back->Hint=GetTextByID("MBACK");
	Back2->AssignSound(GETS("@MOUSESOUND"),MOUSE_SOUND);

	// Version
	MMenu.addTextButton(NULL,1024-GetRLCStrWidth(BuildVersion,&SmallYellowFont1),751,BuildVersion,&SmallYellowFont,&SmallYellowFont1,&SmallYellowFont,0);

	int StTime=GetTickCount();
	CloseMPL();
	//SetDarkPalette();
	InGame=false;
	GameMode=1;
	
	NPlayers=1;
	MultiplayerStart=false;
	ItemChoose=-1;
#ifdef TESTVERSION
	Single->OnUserClick=NULL;
	Multi->OnUserClick=NULL;
	Options->OnUserClick=NULL;
	Video->OnUserClick=NULL;
	Load->OnUserClick=NULL;
#endif
	int nn=0;
	StartMixing();
loop1:
	
	if(CurrentSinBattle!=-1){
		SelectSingleBattle();
		if(ItemChoose==mcmSingle) return ItemChoose;
		StartMixing();
	}
	
	CloseBattle();

	if(CurrentCampagin!=-1&&CurrentMission!=-1){
		ProcessCampagins(CurrentCampagin);
		if(ItemChoose==mcmSingle) return ItemChoose;
		StartMixing();
	};
	
	if(nn){
		//SlowUnLoadPalette("1\\agew_1.pal");
		//nn=0;
	};
	ItemChoose=-1;
	
	StTime=GetTickCount();
	CloseLogin();
	LastKey=0;
	KeyPressed=0;

	do{
		if(ADDSH==1&&bActive&&(GetTickCount()-StTime>60000||RetryVideo)){
			GFILE* f=Gopen("Demo\\demo.run","r");
			if(f){
				int N;
				int z=Gscanf(f,"%d",&N);
				if(z==1){
					//int M=GetRndVid(N);
					int M=0;
					char NAME[64];
					for(int i=0;i<M;i++)Gscanf(f,"%s",NAME);
					Gscanf(f,"%s",NAME);
					RGAME.ReadStream(NAME);
					if(RGAME.Size){
						PlayGameMode=1;
						ItemChoose=43;
						RetryVideo=1;
						KeyPressed=0;
						break;
					};
				};
				Gclose(f);
				StTime=GetTickCount();
			};
		};
		InMainMenuLoop=1;
		ProcessMessages();
		MMenu.ProcessDialogs();
		ProcessMixing();
		MMenu.RefreshView();
		InMainMenuLoop=0;
		if(!nn){
			//SlowLoadPalette("1\\agew_1.pal");
			//LoadFog(1);
		};
		nn++;
		//if(ItemChoose==mcmVideo&&!(GetKeyState(VK_CONTROL)&0x8000))ItemChoose=-1;
		if(RUNMAPEDITOR){
			ItemChoose=mcmVideo;
		};
		if(RUNUSERMISSION){
			ItemChoose=111;
		};
		if(TOTALEXIT)ItemChoose=mcmExit;		
		//ItemChoose=mcmVideo;
	}while(ItemChoose==-1);
	if(ItemChoose==mcmSingle){
		
		int dx;
		int dy;

		int n=25;
		
		dx=-SDX/n;
		dy=-SDY/n;
		
		for(int i=0; i<n; i++){
			Single->xc+=dx; Single->x+=dx; Single->x1+=dx; Single->y+=dy; Single->y1+=dy;
			Multi->xc+=dx; Multi->x+=dx; Multi->x1+=dx; Multi->y+=dy; Multi->y1+=dy;
			Load->xc+=dx; Load->x+=dx; Load->x1+=dx; Load->y+=dy; Load->y1+=dy;
			Options->xc+=dx; Options->x+=dx; Options->x1+=dx; Options->y+=dy; Options->y1+=dy;
			Video->xc+=dx; Video->x+=dx; Video->x1+=dx; Video->y+=dy; Video->y1+=dy;
			Exit->xc+=dx; Exit->x+=dx; Exit->x1+=dx; Exit->y+=dy; Exit->y1+=dy;

			SinBattle->xc+=dx; SinBattle->x+=dx; SinBattle->x1+=dx; SinBattle->y+=dy; SinBattle->y1+=dy;
			Compagin->xc+=dx; Compagin->x+=dx; Compagin->x1+=dx; Compagin->y+=dy; Compagin->y1+=dy;
			Mission->xc+=dx; Mission->x+=dx; Mission->x1+=dx; Mission->y+=dy; Mission->y1+=dy;
			Random->xc+=dx; Random->x+=dx; Random->x1+=dx; Random->y+=dy; Random->y1+=dy;
			Back->xc+=dx; Back->x+=dx; Back->x1+=dx; Back->y+=dy; Back->y1+=dy;

			for(int i=0; i<4; i++){MBack[i]->x+=dx; MBack[i]->y+=dy;}
			
			ProcessMessages();
			MMenu.ProcessDialogs();
			MMenu.RefreshView();
		}

		do{
			processSingleMenu(&MMenu);			
		}while(ItemChoose==mcmCancel);
		//if(ItemChoose==mcmCancel){
		//	nn = 0;
		//}		
		if(ItemChoose==5){

			dx=SDX/n;
			dy=SDY/n;
		
			for(i=0; i<n; i++){
				Single->xc+=dx; Single->x+=dx; Single->x1+=dx; Single->y+=dy; Single->y1+=dy;
				Multi->xc+=dx; Multi->x+=dx; Multi->x1+=dx; Multi->y+=dy; Multi->y1+=dy;
				Load->xc+=dx; Load->x+=dx; Load->x1+=dx; Load->y+=dy; Load->y1+=dy;
				Options->xc+=dx; Options->x+=dx; Options->x1+=dx; Options->y+=dy; Options->y1+=dy;
				Video->xc+=dx; Video->x+=dx; Video->x1+=dx; Video->y+=dy; Video->y1+=dy;
				Exit->xc+=dx; Exit->x+=dx; Exit->x1+=dx; Exit->y+=dy; Exit->y1+=dy;	

				SinBattle->xc+=dx; SinBattle->x+=dx; SinBattle->x1+=dx; SinBattle->y+=dy; SinBattle->y1+=dy;
				Compagin->xc+=dx; Compagin->x+=dx; Compagin->x1+=dx; Compagin->y+=dy; Compagin->y1+=dy;
				Mission->xc+=dx; Mission->x+=dx; Mission->x1+=dx; Mission->y+=dy; Mission->y1+=dy;
				Random->xc+=dx; Random->x+=dx; Random->x1+=dx; Random->y+=dy; Random->y1+=dy;
				Back->xc+=dx; Back->x+=dx; Back->x1+=dx; Back->y+=dy; Back->y1+=dy;

				for(int i=0; i<4; i++){MBack[i]->x+=dx; MBack[i]->y+=dy;}	

				ProcessMessages();
				MMenu.ProcessDialogs();
				MMenu.RefreshView();
			}

			goto loop1;
		};

	};
#ifdef MAKE_PTC
	if(ItemChoose==mcmMulti){
		
		int dx;
		int dy;

		int n=25;
		
		dx=-MDX/n;
		dy=-MDY/n;
		
		for(int i=0; i<n; i++){
			Single->xc+=dx; Single->x+=dx; Single->x1+=dx; Single->y+=dy; Single->y1+=dy;
			Multi->xc+=dx; Multi->x+=dx; Multi->x1+=dx; Multi->y+=dy; Multi->y1+=dy;
			Load->xc+=dx; Load->x+=dx; Load->x1+=dx; Load->y+=dy; Load->y1+=dy;
			Options->xc+=dx; Options->x+=dx; Options->x1+=dx; Options->y+=dy; Options->y1+=dy;
			Video->xc+=dx; Video->x+=dx; Video->x1+=dx; Video->y+=dy; Video->y1+=dy;
			Exit->xc+=dx; Exit->x+=dx; Exit->x1+=dx; Exit->y+=dy; Exit->y1+=dy;

			INetGame->xc+=dx; INetGame->x+=dx; INetGame->x1+=dx; INetGame->y+=dy; INetGame->y1+=dy;
			DeathM->xc+=dx; DeathM->x+=dx; DeathM->x1+=dx; DeathM->y+=dy; DeathM->y1+=dy;
			HistBatt->xc+=dx; HistBatt->x+=dx; HistBatt->x1+=dx; HistBatt->y+=dy; HistBatt->y1+=dy;
			Back2->xc+=dx; Back2->x+=dx; Back2->x1+=dx; Back2->y+=dy; Back2->y1+=dy;
			
			for(int i=0; i<4; i++){MBack[i]->x+=dx; MBack[i]->y+=dy;}

			ProcessMessages();
			MMenu.ProcessDialogs();
			MMenu.RefreshView();
		}
		
		do{
			processMultiMenu(&MMenu);
		}while(ItemChoose==mcmCancel);

		if(ItemChoose==mcmCancel){
			nn=0;
		}
		if(ItemChoose==5){
			dx=MDX/n;
			dy=MDY/n;
		
			for(i=0; i<n; i++){
				Single->xc+=dx; Single->x+=dx; Single->x1+=dx; Single->y+=dy; Single->y1+=dy;
				Multi->xc+=dx; Multi->x+=dx; Multi->x1+=dx; Multi->y+=dy; Multi->y1+=dy;
				Load->xc+=dx; Load->x+=dx; Load->x1+=dx; Load->y+=dy; Load->y1+=dy;
				Options->xc+=dx; Options->x+=dx; Options->x1+=dx; Options->y+=dy; Options->y1+=dy;
				Video->xc+=dx; Video->x+=dx; Video->x1+=dx; Video->y+=dy; Video->y1+=dy;
				Exit->xc+=dx; Exit->x+=dx; Exit->x1+=dx; Exit->y+=dy; Exit->y1+=dy;	

				INetGame->xc+=dx; INetGame->x+=dx; INetGame->x1+=dx; INetGame->y+=dy; INetGame->y1+=dy;
				DeathM->xc+=dx; DeathM->x+=dx; DeathM->x1+=dx; DeathM->y+=dy; DeathM->y1+=dy;
				HistBatt->xc+=dx; HistBatt->x+=dx; HistBatt->x1+=dx; HistBatt->y+=dy; HistBatt->y1+=dy;
				Back2->xc+=dx; Back2->x+=dx; Back2->x1+=dx; Back2->y+=dy; Back2->y1+=dy;	

				for(int i=0; i<4; i++){MBack[i]->x+=dx; MBack[i]->y+=dy;}
			
				ProcessMessages();
				MMenu.ProcessDialogs();
				MMenu.RefreshView();
			}

			goto loop1;
		};
	};
#endif
	if(ItemChoose==mcmVideo){
		ShowLoading();
		LOADNATMASK=0xFFFFFFFF;
		PrepareEditMedia(0);
		ItemChoose=mcmSingle;
	};
	if(ItemChoose==mcmEdit){
//		SlowUnLoadPalette("1\\agew_1.pal");
		ProcessMenuOptions();
		StartMixing();
		//SlowLoadPalette("1\\agew_1.pal");
		goto loop1;
	};
	if(ItemChoose==mcmLoad){
//		SlowUnLoadPalette("1\\agew_1.pal");
		processMLoadGame();
		if(ItemChoose==mcmCancel){
			StartMixing();
			goto loop1;
		}
	};
	if(ItemChoose==mcmExit){
//		SlowUnLoadPalette("1\\agew_1.pal");
		SlideShow();
	};
	if(ItemChoose==43){
		EditMapMode=0;
		ItemChoose=mcmSingle;
	};
	if(ItemChoose==111){
		if(!PlName[0])strcpy(PlName,"Player");
		memset(GSets.CGame.PL_INFO,0,sizeof GSets.CGame.PL_INFO);
		strcpy(GSets.CGame.PL_INFO[0].name,PlName);
		EditMapMode=0;
		ItemChoose=mcmSingle;
		strcpy(GSets.CGame.cgi_CurrentMap,USERMISSPATH);
		CreateNationalMaskForMap(GSets.CGame.cgi_CurrentMap);
		CurrentCampagin=-1;
		CurrentMission=-1;
		PrepareGameMedia(0,0);
		InitGame();
		ItemChoose=mcmSingle;
		RUNUSERMISSION=0;
	};
	
	GameMode=0;
	xmlQuote xml;
    //MMenu.Save(xml,&MMenu);
	//xml.WriteToFile("MainMenu.xml");
	return ItemChoose;
};

int processSingleMenu(DialogsSystem* Menu){
loop1:
	UseGSC_Login=0;
	ItemChoose=-1;
	int pp=1;
	KeyPressed=false;
	LastKey=0;
	do{
		ProcessMessages();
		if(KeyPressed&&LastKey==27){
			ItemChoose=5;
			KeyPressed=0;
		};
		Menu->ProcessDialogs();
		ProcessMixing();
		Menu->RefreshView();
		if(pp){
			//SlowLoadPalette("1\\agew_1.pal");
			pp=0;
		};
		//if((ItemChoose==1||ItemChoose==2)&&!CAMPAGINS.NCamp)ItemChoose=-1;

		#ifdef AC_CLUB
		if(ItemChoose==5) return ItemChoose;
		if(ItemChoose!=1) ItemChoose=-1;
		#endif
		
		#ifdef TUTORIAL_DEMOVERSION
		if(ItemChoose==1) ItemChoose=-1;
		#endif

		if(ItemChoose==1||ItemChoose==2){
			
		}

	}while(ItemChoose==-1);
	if(ItemChoose==0xE0){
		if(!EnterName()){
			StartMixing();
			goto loop1;
		}
		SelectSingleBattle();
		StartMixing();
	}
#ifndef AC_BATTLE_ONLY
	if(ItemChoose==3){
//		SlowUnLoadPalette("1\\agew_1.pal");
		if(!EnterName()){
			StartMixing();
			goto loop1;
		}
		SingleOptions();
		StartMixing();
	};
	if(ItemChoose==1){
//		SlowUnLoadPalette("1\\agew_1.pal");
		if(!EnterName()){
			StartMixing();
			goto loop1;
		}
		ProcessCampagins(-1);
		StartMixing();
		LoadPalette("1\\agew_1.pal");
		LoadFog(1);
	};
#endif

//#ifndef TUTORIAL_DEMOVERSION

	if(ItemChoose==2){
//		SlowUnLoadPalette("1\\agew_1.pal");
		if(!EnterName()){
			StartMixing();
			goto loop1;
		}
		SelectSingleMission();
		StartMixing();
	};
//#endif
	return ItemChoose;
};

bool InternetGame=false;

int processMultiMenu(DialogsSystem *Menu){	
	ItemChoose=-1;
	UnPress();
	Lpressed=0;
	LastKey=0;
	KeyPressed=0;
	int pp=1;
	do{
		ProcessMessages();
		if(KeyPressed&&LastKey==27){
			ItemChoose=5;
			KeyPressed=0;
		};
		Menu->ProcessDialogs();
		ProcessMixing();
		Menu->RefreshView();
		if(pp){
			//SlowLoadPalette("1\\agew_1.pal");
			pp=0;
		};

		if(ItemChoose==5){
			return ItemChoose;
		}

		#ifdef AC_CLUB
		ItemChoose=-1;
		#endif

#ifdef TUTORIAL_DEMOVERSION
		if(ItemChoose==2)ItemChoose=-1;
#else
		if(ItemChoose==2&&!WARS.NWars)ItemChoose=-1;
#endif
	}while(ItemChoose==-1);
		
#ifndef AC_BATTLE_ONLY
	if(ItemChoose==1){
//		SlowUnLoadPalette("1\\agew_1.pal");
		InternetGame=false;
		UseGSC_Login=0;
		processMultiplayer();
	};
#endif
	if(ItemChoose==2 && WARS.NWars){
//		SlowUnLoadPalette("1\\agew_1.pal");
		InternetGame=false;
		UseGSC_Login=0;
		processBattleMultiplayer();
		StartMixing();
		//ProcessWars();
	};
	if(ItemChoose==3){
		InternetGame=true;
		UseGSC_Login=1;
		CurProtocol=3;
		processMultiplayer();
	}
	if(PlayerMenuMode||TOTALEXIT){		
		ItemChoose = mcmCancel;
		StartMixing();
		return mcmCancel;
	}
	return ItemChoose;
};