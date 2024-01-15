#include "cvi_MainMenu.h";
CIMPORT void GoHomeAnyway();
int MM_ProcessMultiPlayer(){	
	GoHomeAnyway();
	LocalGP BTNS("Interface\\Multi_Player");
	LocalGP HFONT("interf3\\Fonts\\FontC10");
	RLCFont hfnt(HFONT.GPID);
	hfnt.SetWhiteColor();

	//SQPicture MnPanel("Interface\\Background_Multi_Player.bmp");
	SQPicture MnPanel("Interface\\Background_Multi_Player.bmp");
	
	
	DialogsSystem MMenu(0,0);
	MMenu.HintFont=&hfnt;
	MMenu.HintY=701;
	MMenu.HintX=18;

	int Dy=110;
	Picture* PIC=MMenu.addPicture(NULL,0,0,&MnPanel,&MnPanel,&MnPanel);
	GP_Button* DeathM=MMenu.addGP_Button(NULL,76,140+Dy,BTNS.GPID,0,1);
	DeathM->UserParam=1;
	DeathM->OnUserClick=&MMItemChoose;
	DeathM->Hint=GetTextByID("MDEATHM");
	DeathM->AssignSound(GETS("@MOUSESOUND"),MOUSE_SOUND);
	GP_Button* HistBatt=MMenu.addGP_Button(NULL,76,140+82+Dy,BTNS.GPID,2,3);
	HistBatt->UserParam=2;
	HistBatt->OnUserClick=&MMItemChoose;
	HistBatt->Hint=GetTextByID("MHISTBATT");
	HistBatt->AssignSound(GETS("@MOUSESOUND"),MOUSE_SOUND);
	GP_Button* Back=MMenu.addGP_Button(NULL,76,140+82*2+Dy,BTNS.GPID,4,5);
	Back->UserParam=5;
	Back->OnUserClick=&MMItemChoose;
	Back->Hint=GetTextByID("MBACK");
	Back->AssignSound(GETS("@MOUSESOUND"),MOUSE_SOUND);
	//SlowUnLoadPalette("2\\agew_1.pal");
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
		MMenu.ProcessDialogs();
		MMenu.RefreshView();
		if(pp){
//			SlowLoadPalette("1\\agew_1.pal");
			pp=0;
		};
#ifdef TUTORIAL_DEMOVERSION
		if(ItemChoose==2)ItemChoose=-1;
#else
		if(ItemChoose==2&&!WARS.NWars)ItemChoose=-1;
#endif
	}while(ItemChoose==-1);
//	SlowUnLoadPalette("1\\agew_1.pal");
	if(ItemChoose==1){
		processMultiplayer();
		if(TOTALEXIT)return mcmCancel;
	};
	if(ItemChoose==2){
		if(WARS.NWars)processBattleMultiplayer();
		//ProcessWars();
	};
	return ItemChoose;
};


bool ProcessNewInternetLogin();

bool processMultiplayer(){
	void PerformCheckCD();
	PerformCheckCD();
	GoHomeAnyway();
	byte AddrBuf[128];
	memset(AddrBuf,0,128);
	int crs=0;
RetryConn:;
	if(IEMMOD)goto REINCONN;
	crs=MPL_ChooseConnection();
	if(TOTALEXIT)return 0;
	if(crs==mcmCancel)return 0;
	
	if(CurProtocol==3){
		if(!InternetGameLogin())return false;
REINCONN:;
		int r=ProcessInternetConnection(1);
		if(!r)return 0;
		if(r==2)crs=10;//Join
		if(r==1)crs=11;//Host DM
		if(r==3)crs=13;//Host BT
	}else{
		if(!CommEnterName())return 0;
	};
	DoNewInet=1;
	CurProtocol=2;
	if(!DoNewInet){
		if(!lpDirectPlay3A){
			CreateMultiplaterInterface();
			if(!lpDirectPlay3A)return 0;
		};
		LPDIRECTPLAYLOBBYA	lpDPlayLobbyA = NULL;
		LPDIRECTPLAYLOBBY2A	lpDPlayLobby2A = NULL;
		if FAILED(DirectPlayLobbyCreate(NULL, &lpDPlayLobbyA, NULL, NULL, 0)) return 0;
			// get ANSI DirectPlayLobby2 interface
		HRESULT hr = lpDPlayLobbyA->QueryInterface(IID_IDirectPlayLobby2A, (LPVOID *) &lpDPlayLobby2A);
		if FAILED(hr)return 0;
				// don't need DirectPlayLobby interface anymore
		lpDPlayLobbyA->Release();
		lpDPlayLobbyA = NULL;
		DPCOMPOUNDADDRESSELEMENT	addressElements[3];
			DWORD sz=128;
		char* cc="";
		switch(CurProtocol){
		case 0://IPX
			addressElements[0].guidDataType = DPAID_ServiceProvider;
			addressElements[0].dwDataSize = sizeof(GUID);
			addressElements[0].lpData = (LPVOID) &DPSPGUID_IPX;
			lpDPlayLobby2A->CreateCompoundAddress(addressElements,1,AddrBuf,&sz);
			/*
			memcpy(AddrBuf,&DPAID_TotalSize,16);
			AddrBuf[16]=56;
			memcpy(AddrBuf+20,&DPAID_ServiceProvider,16);
			AddrBuf[36]=16;
			memcpy(AddrBuf+40,&DPSPGUID_IPX,16);
			*/
			break;
		case 1://TCP/IP
			IPADDR[0]=0;
		case 3:
		case 2:
			addressElements[0].guidDataType = DPAID_ServiceProvider;
			addressElements[0].dwDataSize = sizeof(GUID);
			addressElements[0].lpData = (LPVOID) &DPSPGUID_TCPIP;
			addressElements[1].guidDataType = DPAID_INet;
			addressElements[1].dwDataSize = strlen(IPADDR)+1;
			addressElements[1].lpData = (LPVOID) IPADDR;
			lpDPlayLobby2A->CreateCompoundAddress(addressElements,2,AddrBuf,&sz);
			/*
			memcpy(AddrBuf,&DPAID_ServiceProvider,16);
			AddrBuf[16]=16;
			memcpy(AddrBuf+20,&DPSPGUID_TCPIP,16);
			memcpy(AddrBuf+36,&DPAID_INet,16);
			AddrBuf[40]=1;
			AddrBuf[41]=0;
			*/
			break;
		};
		lpDPlayLobby2A->Release();
		CloseMPL();
		CreateMultiplaterInterface();
		HRESULT HR=lpDirectPlay3A->InitializeConnection(AddrBuf,0);
		if(FAILED(HR))goto RetryConn;
	}else{
		CloseMPL();
		CreateMultiplaterInterface();
	};
	switch(crs){
	case mcmHost:
		if(CreateNamedSession(PlName,0,GMMAXPL))WaitingHostGame(0);
		break;
	case mcmJoin:
		MPL_JoinGame(0);
		break;
	case 11://Inet Host(Deathmatch)
		{
			CurrentGameInfo* g=&GSets.CGame;
			if(g->CreateRoom(LobbyVersion,7)){
				memset(COMPSTART,0,sizeof COMPSTART);
				extern bool v_MM_Host;
				extern cvi_MainMenu v_MainMenu;
				extern _str PlayerName;
				v_MM_Host=true;
				//cva_MM_Start::LeftClick(SD);
				PlayerName=GlobalRIF.Nick;
				strcpy(PlName,PlayerName.pchar());
				extern bool NeedToPerformGSC_Report;
				NeedToPerformGSC_Report=true;
				//
				extern int mRND;
				mRND=GetTickCount();
				//
				v_MainMenu.StartDS("MultiRoom");
				//
				NeedToPerformGSC_Report=false;
				g->LeaveRoom();
				//v_MainMenu.CloseDS();
				if(!GameInProgress)goto REINCONN;
			}		
		}
		//
		break;
		/*
		PlayerMenuMode=1;
		if(UseGSC_Login){
			int OPT=0;
			if(GlobalRIF.GameID[0]=='H'&&GlobalRIF.GameID[1]=='B'){
				OPT=1;
				strcpy(GlobalRIF.GameID,GlobalRIF.GameID+2);
			};
			if(CreateSession(GlobalRIF.Name,GlobalRIF.Nick,OPT,DoNewInet,GlobalRIF.MaxPlayers)){
				NeedToPerformGSC_Report=1;
				WaitingHostGame(OPT);
				NeedToPerformGSC_Report=0;
				if(PlayerMenuMode==1){
					//need to leave the room there
					LeaveGSCRoom();
					goto REINCONN;
				}else{
					char* PLAYERS[8];
					int Profiles[8];
					char NAT[8][32];
					char* Nations[8];
					int Teams[8];
					int Colors[8];
					for(int i=0;i<NPlayers;i++){
						PLAYERS[i]=GSets.CGame.PL_INFO[i].name;
						sprintf(NAT[i],"%d",GSets.CGame.PL_INFO[i].NationID+48);
						Nations[i]=NAT[i];
						Profiles[i]=GSets.CGame.PL_INFO[i].ProfileID;
						Teams[i]=GSets.CGame.PL_INFO[i].GroupID;
						Colors[i]=GSets.CGame.PL_INFO[i].ColorID;
					};
					StartGSCGame("",GSets.CGame.PL_INFO[0].MapName,NPlayers,Profiles,Nations,Teams,Colors);
					NeedToReportInGameStats=1;
					LastTimeReport_tmtmt=0;
				};
			}else{ 
				LeaveGSCRoom();
				goto REINCONN;
			};
		}else{
			if(TPEN.MyRoom&&CreateSession(TPEN.MyRoom->Name,PlName,0,DoNewInet,GMMAXPL)){
				do{
					ProcessMessages();
				}while(TPEN.MyRoom&&!TPEN.MyRoom->RoomConnected);
				if(TPEN.MyRoom)WaitingHostGame(0);
				else{
					WaitWithMessage(GetTextByID("ICUNCR"));
					CloseMPL();
				};

			}else{
				WaitWithMessage(GetTextByID("ICUNCR"));
				goto REINCONN;
			};
			if(PlayerMenuMode==1){
				NoWaitWithMessage(GetTextByID("ICDISG"));
				do{
					ProcessMessages();
				}while(TPEN.MyRoom&&!TPEN.MyRoom->RoomConnected);
				if(TPEN.MyRoom)TPEN.LeaveMyRoom();
				goto REINCONN;
			}else{
				do{
					ProcessMessages();
				}while(TPEN.MyRoom&&!TPEN.MyRoom->RoomConnected);
				//if(TPEN.Connected&&TPEN.MyRoom){
					//TPEN.MyRoom->RStart=1;
					//peerStateChanged(TPEN.Peer);
				//};
				if(TPEN.MyRoom&&TPEN.MyRoom->RoomConnected){
					//char GCHAT[512]="";
					//sprintf(GCHAT,"[~ADMI@START~]%s",TPEN.MyRoom->Name);
					//TPEN.CreateMyRoomInfoString(GCHAT);
					//if(TPEN.HostMessage[0])TPEN.SendGlobalChat(TPEN.HostMessage);
				};
				//TPEN.Disconnect();
				TPEN.StartGame(true);
			};
		};
		*/
		break;
	case 13:
		PlayerMenuMode=1;
		if(TPEN.MyRoom&&CreateSession(TPEN.MyRoom->Name,PlName,1,DoNewInet,2)){
			do{
				ProcessMessages();
			}while(TPEN.MyRoom&&!TPEN.MyRoom->RoomConnected);
			if(TPEN.MyRoom)WaitingHostGame(1);
			else{
				WaitWithMessage(GetTextByID("ICUNCR"));
				CloseMPL();
			};
		}else{
			WaitWithMessage(GetTextByID("ICUNCR"));
			do{
				ProcessMessages();
			}while(TPEN.MyRoom&&!TPEN.MyRoom->RoomConnected);
			if(TPEN.MyRoom)TPEN.LeaveMyRoom();
			goto REINCONN;
		};
		if(PlayerMenuMode==1){
			NoWaitWithMessage(GetTextByID("ICDISG"));
			do{
				ProcessMessages();
			}while(TPEN.MyRoom&&!TPEN.MyRoom->RoomConnected);
			if(TPEN.MyRoom)TPEN.LeaveMyRoom();
			goto REINCONN;
		}else{
			do{
				ProcessMessages();
			}while(TPEN.MyRoom&&!TPEN.MyRoom->RoomConnected);
			//TPEN.Disconnect();
			TPEN.StartGame(false);
			//if(TPEN.Connected&&TPEN.MyRoom){
				//TPEN.MyRoom->RStart=1;
				//peerStateChanged(TPEN.Peer);
			//};
		};
		break;
	case 10://Inet Join(Deathmatch)
		{
			extern bool v_MM_Host;
			extern cvi_MainMenu v_MainMenu;
			extern _str PlayerName;
			extern bool GameInProgress;
			extern DWORD vWaitServerInfo;
			//
			CurrentGameInfo* g=&GSets.CGame;
			PlayerName=GlobalRIF.Nick;
			//
			strcpy(PlName,PlayerName.pchar());
			strcpy(IPADDR,GlobalRIF.RoomIP);
			extern _str PlayerName;
			PlayerName=PlName;

			//			
			if(IPADDR[0]&&g->JoinRoom(IPADDR,GlobalRIF.Name)){
				vWaitServerInfo=GetTickCount()+10000;
				v_MM_Host=false;
				GameInProgress=false;
				//cva_MM_Start::LeftClick(SD);
				v_MainMenu.StartDS("MultiRoom");
				g->LeaveRoom();
				//v_MainMenu.CloseDS();				
			};
			if(!GameInProgress)goto REINCONN;
		}
		break;
		//
		if(UseGSC_Login){
			PlayerMenuMode=1;
			strcpy(IPADDR,GlobalRIF.RoomIP);
			int OPT=0;
			if(GlobalRIF.GameID[0]=='H'&&GlobalRIF.GameID[1]=='B'){
				OPT=1;
				strcpy(GlobalRIF.GameID,GlobalRIF.GameID+2);
			};
			if(!FindSessionAndJoin(ROOMNAMETOCONNECT,GlobalRIF.Nick,DoNewInet)){
				LeaveGSCRoom();
				WaitWithMessage(GetTextByID("ICUNJ"));
			}else{
				WaitingJoinGame(GMTYPE);
			};
			if(PlayerMenuMode==1){
				LeaveGSCRoom();
				goto REINCONN;
			}else{
				char* PLAYERS[8];
				int Profiles[8];
				char NAT[8][32];
				char* Nations[8];
				int Teams[8];
				int Colors[8];
				for(int i=0;i<NPlayers;i++){
					PLAYERS[i]=GSets.CGame.PL_INFO[i].name;
					sprintf(NAT[i],"%d",GSets.CGame.PL_INFO[i].NationID+80);//for Lazar&Tolik - bilo +48
					Nations[i]=NAT[i];
					Profiles[i]=GSets.CGame.PL_INFO[i].ProfileID;
					Teams[i]=GSets.CGame.PL_INFO[i].GroupID;
					Colors[i]=GSets.CGame.PL_INFO[i].ColorID;
				};
				StartGSCGame("",GSets.CGame.PL_INFO[0].MapName,NPlayers,Profiles,Nations,Teams,Colors);
				NeedToReportInGameStats=1;
				LastTimeReport_tmtmt=0;
			};
		}else{
			PlayerMenuMode=1;
			if(!FindSessionAndJoin(ROOMNAMETOCONNECT,TPEN.MyNick,DoNewInet)){
				WaitWithMessage(GetTextByID("ICUNJ"));
				do{
					ProcessMessages();
				}while(TPEN.MyRoom&&!TPEN.MyRoom->RoomConnected);
				if(TPEN.MyRoom)TPEN.LeaveMyRoom();
				goto REINCONN;
			}else{
				/*
				do{
					ProcessMessages();
				}while(TPEN.MyRoom&&!TPEN.MyRoom->RoomConnected);
				*/
				WaitingJoinGame(GMTYPE);
				/*
				if(TPEN.MyRoom)WaitingJoinGame(GMTYPE);
				else{
					WaitWithMessage(GetTextByID("ICUNJ"));
					CloseMPL();
				};
				*/
			};	
			if(PlayerMenuMode==1){
				NoWaitWithMessage(GetTextByID("ICDISG"));
				do{
					ProcessMessages();
				}while(TPEN.MyRoom&&!TPEN.MyRoom->RoomConnected);
				if(TPEN.MyRoom)TPEN.LeaveMyRoom();
				goto REINCONN;
			}else TPEN.StartGame(false);
				//TPEN.Disconnect();
		};
		break;
	};	
	return 1;
};


void processBattleMultiplayer(){
TryConnection:;
	byte AddrBuf[128];
	memset(AddrBuf,0,128);
	int crs;
	crs=MPL_ChooseConnection();
	if(TOTALEXIT)return;
	if(crs==mcmCancel)return;
	if(CurProtocol==3){
		IEMMOD=1;
		processMultiplayer();
		if(TOTALEXIT)return;
		IEMMOD=0;
		return;
	};
	if(!CommEnterName())return;
	
	//if(crs==mcmHost){
	//	BTLID=ProcessWars();
	//};

	if(BTLID==-1)goto TryConnection;
	if(!lpDirectPlay3A){
		DoNewInet=0;
		CreateMultiplaterInterface();
		if(!lpDirectPlay3A)return;
	};

	LPDIRECTPLAYLOBBYA	lpDPlayLobbyA = NULL;
	LPDIRECTPLAYLOBBY2A	lpDPlayLobby2A = NULL;
	if FAILED(DirectPlayLobbyCreate(NULL, &lpDPlayLobbyA, NULL, NULL, 0)) return;
	// get ANSI DirectPlayLobby2 interface
	HRESULT hr = lpDPlayLobbyA->QueryInterface(IID_IDirectPlayLobby2A, (LPVOID *) &lpDPlayLobby2A);
	if FAILED(hr)return;

	// don't need DirectPlayLobby interface anymore
	lpDPlayLobbyA->Release();
	lpDPlayLobbyA = NULL;
	DPCOMPOUNDADDRESSELEMENT	addressElements[3];
	DWORD sz=128;
	char* cc="";
	switch(CurProtocol){
	case 0://IPX
		addressElements[0].guidDataType = DPAID_ServiceProvider;
		addressElements[0].dwDataSize = sizeof(GUID);
		addressElements[0].lpData = (LPVOID) &DPSPGUID_IPX;
		lpDPlayLobby2A->CreateCompoundAddress(addressElements,1,AddrBuf,&sz);
		break;
	case 1://TCP/IP
		IPADDR[0]=0;
	case 3:
	case 2:
		addressElements[0].guidDataType = DPAID_ServiceProvider;
		addressElements[0].dwDataSize = sizeof(GUID);
		addressElements[0].lpData = (LPVOID) &DPSPGUID_TCPIP;
		addressElements[1].guidDataType = DPAID_INet;
		addressElements[1].dwDataSize = strlen(IPADDR)+1;
		addressElements[1].lpData = (LPVOID) IPADDR;
		lpDPlayLobby2A->CreateCompoundAddress(addressElements,2,AddrBuf,&sz);
		break;

	};
	lpDPlayLobby2A->Release();
	CloseMPL();
	CreateMultiplaterInterface();
	if FAILED(lpDirectPlay3A->InitializeConnection(AddrBuf
		/*lplpConnectionBuffer[CurProtocol]*/,0)) return;
	switch(crs){
	case mcmHost:
		if(CreateNamedSession(PlName,BTLID+1,2))WaitingHostGame(BTLID+1);
		break;
	case mcmJoin:
		MPL_JoinGame(BTLID+1);
		break;
	};
};