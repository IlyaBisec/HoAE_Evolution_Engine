//--------------Resource sending--------------//
extern DialogsSystem RESSEND;
bool RESMODE=0;
bool PrevRESMODE=0;
int RSN_CurPlayerTo=0;
int RSN_CurResType=0;
int RSN_CurResAmount=1000;
ComboBox* RSN_USER=NULL;
ComboBox* RSN_RESID=NULL;
InputBox* RSN_IB=NULL;
char RSN_RESAM[32]="";
char* STR1=NULL;
char* STR2=NULL;
char* STR3=NULL;
char* STR4=NULL;
char* STR5=NULL;
char* STR6=NULL;
int CBB_GPFILE=0;
#define WinLX 350
#define WinLY 180
extern char* ResNames[8];
int GAME_CLICK_RESULT=-1;
bool ON_GAME_MCLICK(SimpleDialog* SD){
	GAME_CLICK_RESULT=SD->UserParam;
	return 0;
};
void CreateRESSEND(){
	RESSEND.CloseDialogs();
	int X0=(RealLx-WinLX)>>1;
	int Y0=(RealLy-WinLY)>>1;
	if(!STR1){
		STR1=GetTextByID("SENDR_L1");
		STR2=GetTextByID("SENDR_L2");
		STR3=GetTextByID("SENDR_L3");
		STR4=GetTextByID("SENDR_L4");
		STR5=GetTextByID("SENDR_L5");
		STR6=GetTextByID("SENDR_L6");
	};
	int FDY=40;
	RESSEND.addTextButton(NULL,X0+16,Y0    ,STR1,&YellowFont,&YellowFont,&YellowFont,0);
	RESSEND.addTextButton(NULL,X0+16,Y0+FDY   ,STR2,&YellowFont,&YellowFont,&YellowFont,0);
	RESSEND.addTextButton(NULL,X0+16,Y0+FDY+26,STR3,&YellowFont,&YellowFont,&YellowFont,0);
	RESSEND.addTextButton(NULL,X0+16,Y0+FDY+52,STR4,&YellowFont,&YellowFont,&YellowFont,0);
	RSN_USER= RESSEND.addGP_ComboBoxDLX(NULL,X0+150,Y0+FDY   ,180,CBB_GPFILE,0,9,0,&WhiteFont,&YellowFont,NULL);
	for(int i=0;i<NPlayers;i++){
		RSN_USER->AddLine(GSets.CGame.PL_INFO[i].name);
	};
	RSN_USER->CurLine=RSN_CurPlayerTo;
	if(RSN_USER->CurLine>=RSN_USER->NLines)RSN_USER->CurLine=0;
	RSN_RESID=RESSEND.addGP_ComboBoxDLX(NULL,X0+150,Y0+FDY+26,180,CBB_GPFILE,0,9,0,&WhiteFont,&YellowFont,NULL);
	for(int i=0;i<6;i++){
		RSN_RESID->AddLine(RDS[i].Name);
	};
	RSN_RESID->CurLine=RSN_CurResType;
	if(RSN_RESID->CurLine>=RSN_RESID->NLines)RSN_RESID->CurLine=0;
	ColoredBar* CB=RESSEND.addColoredBar(X0+150,Y0+FDY+26*2,180,21,0x4D);
	CB->Style=1;
	RSN_IB=RESSEND.addInputBox(NULL,X0+150,Y0+FDY+26*2,RSN_RESAM,10,120,26,&YellowFont,&WhiteFont);
	
	int ID=GPS.PreLoadGPImage("Interf2\\Button2");

	int XB=X0+10-8;
	int YB=Y0+WinLY-48-10;

	RESSEND.addGPPicture(NULL,XB,YB,CBB_GPFILE,8);
	RESSEND.addGPPicture(NULL,XB,YB,CBB_GPFILE,11);

	RESSEND.addGPPicture(NULL,XB,YB,ID,0);
	GP_TextButton* OKBTN=RESSEND.addGP_TextButton(NULL,XB+23,YB+20,STR5,ID,1,&SpecialBlackFont,&SpecialBlackFont);
	OKBTN->OnUserClick=&ON_GAME_MCLICK;
	OKBTN->UserParam=1;
	XB=X0+180-8;
	
	//RESSEND.addGPPicture(NULL,XB,YB,CBB_GPFILE,8);
	//RESSEND.addGPPicture(NULL,XB,YB,CBB_GPFILE,11);
	RESSEND.addGPPicture(NULL,XB,YB,ID,0);
	GP_TextButton* CANCELBTN=RESSEND.addGP_TextButton(NULL,XB+23,YB+20,STR6,ID,1,&SpecialBlackFont,&SpecialBlackFont);
	CANCELBTN->OnUserClick=&ON_GAME_MCLICK;
	CANCELBTN->UserParam=0;
	sprintf(RSN_RESAM,"%d",RSN_CurResAmount);
};
void DrawHdrTable(int x0,int y0,int x1,int y1);
void CmdGiveMoney(byte SrcNI,byte DstNI,byte Res,int Amount);
int ReadKey();
void ProcessRESSEND(){
	if(RESMODE&&!PrevRESMODE){
		CreateRESSEND();
	};
	PrevRESMODE=RESMODE;
	if(RESMODE){
		int x0=(RealLx-WinLX)>>1;
		int y0=(RealLy-WinLY)>>1;
		DrawHdrTable(x0,y0,x0+WinLX,y0+WinLY);
		GAME_CLICK_RESULT=-1;
		int K;
		do{
			KeyPressed=0;
			K=ReadKey();
			if(K==13){
				GAME_CLICK_RESULT=1;
				K=-1;
			};
			if(K==27){
				GAME_CLICK_RESULT=0;
				K=-1;
			};
			if(K!=-1){
				KeyPressed=1;
				LastKey=K;
				RSN_IB->OnKeyDown(RSN_IB);
			};
		}while(K!=-1);
		KeyPressed=0;
		LastKey=0;
		RESSEND.ProcessDialogs();
		if(GAME_CLICK_RESULT==0){
			PrevRESMODE=0;
			RESMODE=0;
		};
		if(GAME_CLICK_RESULT==1){
			PrevRESMODE=0;
			RESMODE=0;
			int z=sscanf(RSN_RESAM,"%d",&RSN_CurResAmount);
			if(z==1)CmdGiveMoney(GSets.CGame.cgi_NatRefTBL[MyNation],GSets.CGame.cgi_NatRefTBL[GSets.CGame.PL_INFO[RSN_USER->CurLine].ColorID],RSN_RESID->CurLine,RSN_CurResAmount);
		};
		RSN_CurResType=RSN_RESID->CurLine;
		RSN_CurPlayerTo=RSN_USER->CurLine;
		Lpressed=0;
		UnPress();
	};
};