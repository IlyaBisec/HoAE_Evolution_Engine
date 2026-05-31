bool SingleOptions(){
	if(MPL_WaitingGame(1,1)){
		StartIGame(1);
		return true;
	}else return false;
};
int MM_ProcessSinglePlayer(){
	
	LoadFog(1);
	LocalGP BTNS("Interface\\Single_Player");	

	SQPicture MnPanel("Interface\\Background_Single_Player.bmp");
	DialogsSystem MMenu(0,0);
	LocalGP HFONT("interf3\\Fonts\\FontC10");
	RLCFont hfnt(HFONT.GPID);
	hfnt.SetWhiteColor();
	MMenu.HintFont=&hfnt;
	MMenu.HintY=701;
	MMenu.HintX=18;
	int Dy=27;
	Picture* PIC=MMenu.addPicture(NULL,0,0,&MnPanel,&MnPanel,&MnPanel);
	

	//LoadPlayerData();//();
	//LoadFog(2);
	LoadPlayerData();//();
	//LoadFog(2);
	//SlowUnLoadPalette("2\\agew_1.pal");
	
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
		MMenu.ProcessDialogs();
		MMenu.RefreshView();
		if(pp){
//			SlowLoadPalette("1\\agew_1.pal");
			pp=0;
		};
		//if((ItemChoose==1||ItemChoose==2)&&!CAMPAGINS.NCamp)ItemChoose=-1;
/*
#ifdef TUTORIAL_DEMOVERSION
	if(ItemChoose==2)ItemChoose=-1;
#endif 
*/
	}while(ItemChoose==-1);
	if(ItemChoose==3){
//		SlowUnLoadPalette("1\\agew_1.pal");
		SingleOptions();
	};
	if(ItemChoose==1){
//		SlowUnLoadPalette("1\\agew_1.pal");
		ProcessCampagins(-1);
	};
//#ifndef TUTORIAL_DEMOVERSION
	if(ItemChoose==2){
//		SlowUnLoadPalette("1\\agew_1.pal");
		SelectSingleMission();
	};
//#endif
	if(ItemChoose==5){
//		SlowUnLoadPalette("1\\agew_1.pal");
	};
	return ItemChoose;
};