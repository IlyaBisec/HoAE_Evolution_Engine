bool MPL_JoinGame(int ID){
	LocalGP BTNS("Interface\\Game_Select");
	
	LocalGP FONT1("interf3\\Fonts\\FontC10");
	LocalGP FONT2("interf3\\Fonts\\FontC10");
	RLCFont FontA(FONT2.GPID);
	RLCFont FontP(FONT1.GPID);
	
	LoadFog(1);
	SQPicture Back("Interf2\\Game_Select_Background.bmp");

	DialogsSystem MENU(0,0);
	MENU.addPicture(NULL,0,0,&Back,&Back,&Back);
	
	//int dx=((RealLx-GPS.GetGPWidth(BTNS.GPID,0))>>1)-85;
	//int dy=(RealLy-GPS.GetGPHeight(BTNS.GPID,0))>>1;
	//MENU.addGPPicture(NULL,dx,dy,BTNS.GPID,0);

	// Graphics
	LocalGP BTN("Interf2\\Buttons");
	LocalGP LBX("Interf2\\ListBox");

	// Fonts
	RLCFont* FScr = &fon40y5;
	RLCFont* FWin = &fon16y5;
	RLCFont* FYellow = &fon18y3;
	RLCFont* FButton = &fon18y5;

	// Text
	MENU.addTextButton(NULL,512,18,GetTextByID("INTF_JOINGAME_SCR0"),FScr,FScr,FScr,1);
	MENU.addTextButton(NULL,680,211,GetTextByID("INTF_LOAD_TITLE1"),FWin,FWin,FWin,1);

	MENU.addTextButton(NULL,447,528,GetTextByID("INTF_JOINGAME_TXT0"),FYellow,FYellow,FYellow,0);

	// Buttons
	int ButY = 577;
	//GP_Button* Join=MENU.addGP_Button(NULL,93,594,BTNS.GPID,0,1);	
	//GP_Button* Refresh=MENU.addGP_Button(NULL,238,594,BTNS.GPID,2,3);
	//GP_Button* Cancel=MENU.addGP_Button(NULL,384,594,BTNS.GPID,4,5);
	GP_TextButton* Join=MENU.addGP_TextButton(NULL,447,ButY,GetTextByID("INTF_JOIN"),BTN.GPID,1,FButton,FButton);
	GP_TextButton* Refresh=MENU.addGP_TextButton(NULL,614,ButY,GetTextByID("INTF_REFRESH"),BTN.GPID,1,FButton,FButton);
	GP_TextButton* Cancel=MENU.addGP_TextButton(NULL,781,ButY,GetTextByID("INTF_CANCEL"),BTN.GPID,1,FButton,FButton);

	Join->OnUserClick=&MMItemChoose;
	Join->UserParam=mcmJoin;
	
	Refresh->OnUserClick=&MMItemChoose;
	Refresh->UserParam=mcmRefresh;
	
	Cancel->OnUserClick=&MMItemChoose;
	Cancel->UserParam=mcmCancel;

	int LBY;
	ListBox* LB=MENU.addGP_ListBox(NULL,446,256,9,LBX.GPID,0,26,&WhiteFont,&YellowFont,NULL);
	TextButton* SelGame=MENU.addTextButton(NULL,603,528,"                                                            ",&WhiteFont,&WhiteFont,&WhiteFont,0);
	
	LBEnumerateSessions(LB,ID);
	extern bool IPCORE_INIT;
	if(DoNewInet&&IPCORE_INIT){
		if(WaitingJoinGame(0)){
			return true;
		}else ItemChoose=mcmCancel;
	}

	LB->FontDy-=3;
	LB->FontDx-=16;
	LB->CurItem=-1;
	//LB->M_Over=9;
	if(LB->NItems) LB->CurItem=0;
StartJoin:;
	int pp=1;	
	
	StartMixing();
	do{
		ItemChoose=-1;
		do{
			ListBoxItem* CI = LB->GetItem(LB->CurItem);
			if(CI) strcpy(SelGame->Message,CI->Message);
			ProcessMessages();
			//OkBtn->NeedToDraw=true;
			if(LB->CurItem==-1) 
				Join->Enabled=false;
			else 
				Join->Enabled=true;
			MENU.MarkToDraw();
			MENU.ProcessDialogs();
			for(int i=0;i<LB->NItems;i++){
				char nps[16];
				int ppr=(LB->GetItem(i)->Param1)&65535;
				sprintf(nps,"%d/%d",ppr&255,ppr>>8);
				if(LB->CurItem==i)
					ShowString(LB->x+417,LB->y+2+i*26,nps,&WhiteFont);
				else 
					ShowString(LB->x+417,LB->y+2+i*26,nps,&YellowFont);
			};
			ProcessMixing();
			MENU.RefreshView();
			if(ItemChoose==mcmRefresh){
				LBEnumerateSessions(LB,ID);
				ItemChoose=-1;				
				if(!LB->NItems){
					LB->CurItem=-1;
				}else{
					StartMixing();
				}
			};
			if(pp){
//				SlowLoadPalette("1\\agew_1.pal");
				pp=0;
			};
		}while(ItemChoose==-1);
		if(ItemChoose==mcmJoin&&LB->CurItem!=-1){
			int BID=LB->GetItem(LB->CurItem)->Param1>>16;
			if(BID){
//				SlowUnLoadPalette("1\\agew_1.pal");
				//if(!ProcessOneBattle(BID-1)){
//					SlowUnLoadPalette("1\\agew_1.pal");
				//goto StartJoin;
				//};
			};
			if(JoinNameToSession(LB->CurItem,PlName)){
				if(WaitingJoinGame(LB->GetItem(LB->CurItem)->Param1>>16)){
					return true;
				}else ItemChoose=mcmCancel;
			};
		};
	}while(ItemChoose!=mcmCancel);
//	SlowUnLoadPalette("1\\agew_1.pal");
	return false;
};