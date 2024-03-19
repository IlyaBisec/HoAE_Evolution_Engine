bool EnterName(){
	LoadFog(1);
	LocalGP BTN("Interf2\\Buttons");
	LocalGP TB1("Interf2\\TextBox1");
	int Set[3][30] = {0,1,2,3,4, 5,6,7,8,9, 5,6,7,8,9,
			       0,1,2,3,4, 5,6,7,8,9, 5,6,7,8,9,
					  0,1,2,3,4, 5,8,6,8,9, 5,8,6,8,9,
			       0,1,2,3,4, 5,8,6,8,9, 5,8,6,8,9,
					  0,1,2,3,4, 5,8,7,6,9, 5,8,7,6,9,
			       0,1,2,3,4, 5,8,7,6,9, 5,8,7,6,9};

	//LocalGP BTNS("Interf2\\Player_Name");
	LocalGP FONT1("interf3\\Fonts\\FontC10");
	LocalGP FONT2("interf3\\Fonts\\FontC10");
	RLCFont Font1(FONT1.GPID);
	RLCFont Font2(FONT2.GPID);

	RLCFont hfnt(FONT1.GPID);

	SQPicture Back("Interf2\\Background_Player_Name.bmp");
	DialogsSystem MENU(0,0);
	MENU.UserClickSound=-1;
	MENU.addPicture(NULL,0,0,&Back,&Back,&Back);

	// Fonts
	RLCFont* FScreen = &fon40y5;
	RLCFont* FWindow = &fon16y5;
	RLCFont* FButton = &fon18y5;
	RLCFont* FWhite = &fon18w;
	RLCFont* FYellow = &fon18y3;
	RLCFont* FHint = &fon13y2;

	// Text
	TextButton* Title0=MENU.addTextButton(NULL,512,18,GetTextByID("INTF_PLAYER_TITLE0"),FScreen,FScreen,FScreen,1);
	TextButton* Title1=MENU.addTextButton(NULL,680,211,GetTextByID("INTF_PLAYER_TITLE1"),FWindow,FWindow,FWindow,1);
	TextButton* CurPlayer=MENU.addTextButton(NULL,446,528,GetTextByID("INTF_PLAYER_CP"),FYellow,FYellow,FYellow,0);

	// Hint
	int HintX = 490;
	int HintY = 745;

	MENU.HintFont=FHint;
	MENU.HintX=HintX;
	MENU.HintY=HintY;

	InputBox* IB=MENU.addInputBox(NULL,594,524,PlName,16,308,40,FYellow,FWhite,0);	
	IB->Active=true;
	UniversalButton* TBS[14];
	GPPicture* GPP[14];
	
	const NPlayers = 10;
	int XL0=446;
	int YL0=255;
	for(int i=0;i<NPlayers;i++)TBS[i]=NULL;
	GFILE* F=Gopen("players.txt","r");
	int N=0;
	KeyPressed=0;
	if(F){
		char ccc[128];
		do{
			ccc[0]=0;
			int cc=0;
			int nn=0;
			while(!(cc==0x0A||cc==EOF)){
				cc=Ggetch(F);
				if(!(cc==0x0A||cc==EOF)){
					ccc[nn]=cc;
					nn++;
				}else{
					//Ggetch(f);
				};
			};
			ccc[nn]=0;
			if(ccc[0]){
				if(N<NPlayers){
					//GPP[N]=MENU.addGPPicture(NULL,XL0,YL0,BTNS.GPID,0+(N%3));
					//GPP[N]->Visible=0;
					TBS[N]=MENU.addUniversalButton(XL0,YL0,467,ccc,TB1.GPID,Set[N%3],0,0,0,FYellow,FYellow,FWhite);
					//TBS[N]->FontDy++;
					TBS[N]->OnUserClick=&MMChooseName;
					TBS[N]->UserParam=N;
					TBS[N]->y1-=2;
					YL0+=26;
					N++;
				};
			};
		}while(ccc[0]&&N<NPlayers);
		Gclose(F);
	};
	int LastName=-1;
	if(N){
		strcpy(PlName,TBS[0]->Message);
		TBS[0]->State=1;
		LastName=0;
	};

	GP_TextButton* Accept=MENU.addGP_TextButton(NULL,472,577,GetTextByID("INTF_ACCEPT"),BTN.GPID,0,FButton,FButton);
	GP_TextButton* Cancel=MENU.addGP_TextButton(NULL,704,577,GetTextByID("INTF_CANCEL"),BTN.GPID,0,FButton,FButton);

	Accept->FontDy -=1;
	Cancel->FontDy -=1;

	Accept->UserParam=mcmOk;	
	Cancel->UserParam=mcmCancel;

	Accept->OnUserClick=&MMItemChoose;
	Cancel->OnUserClick=&MMItemChoose;

	Accept->Hint=GetTextByID("IHINT_ACEPT_NAME");
	Cancel->Hint=GetTextByID("SOCANCEL");
	
	ItemChoose=-1;
	NameChoose=-1;
	int pp=1;
	KeyPressed=0;
	
	StartMixing();
	do{
		ProcessMessages();
		StdKeys();
		MENU.ProcessDialogs();
		ProcessMixing();
		MENU.RefreshView();		
		if(NameChoose!=-1){
			strcpy(PlName,TBS[NameChoose]->Message);
			LastName=NameChoose;
			NameChoose=-1;
			IB->Active=true;
			IB->CursPos=strlen(PlName);
		};
		Accept->Enabled=strlen(PlName)!=0;
		if(!Accept->Enabled&&ItemChoose==mcmOk)ItemChoose=-1;
		if(pp){
//			SlowLoadPalette("1\\agew_1.pal");
			pp=0;
		};
		//for(int i=0;i<N;i++)GPP[i]->Visible=i==LastName;
	}while(ItemChoose==-1);
	if(ItemChoose==mcmOk){
		bool orig=true;
		for(int i=0;i<N;i++)if(!strcmp(PlName,TBS[i]->Message))orig=false;
		FILE* F=fopen("players.txt","w");
		if(F){
			int NN=0;
			fprintf(F,"%s\n",PlName);
			NN++;
			for(i=0;i<N;i++){
				if(NN<12&&strcmp(PlName,TBS[i]->Message)){
					fprintf(F,"%s\n",TBS[i]->Message);
					NN++;
				};
			};
			fclose(F);
		};
		LoadPlayerData();//();
		SavePlayerData();
//		SlowUnLoadPalette("1\\agew_1.pal");
		return 1;
	};
//	SlowUnLoadPalette("1\\agew_1.pal");
	return 0;
};