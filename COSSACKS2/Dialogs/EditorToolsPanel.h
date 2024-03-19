//---------------------MAP EDITOR INTERFACE--------------//
#include "kContext.h"
byte MI_Mode=1;

// Fonts
RLCFont* WinTool=&fon16y1;

int CurrentEditMode;
DialogsSystem MAPTOOLS(2,0);
void LoadMapInterface(){
	MapGP=GPS.PreLoadGPImage("Interf3\\ToolBox");
	GPS.PreLoadGPImage("Interf3\\ToolOpt");
	ScrollGP=GPS.PreLoadGPImage("Interf3\\elements\\scroll");
};
#define HDR_FDY -27
void ClearModes();
extern int WaterEditMode;
extern byte LockMode;
extern int ZoneCmd;
void CreateTOOL_PARAM_UNIT();
void CreateTOOL_PARAM_SPRITE(byte ID);
void CreateTOOL_PARAM_DEFAULT();
void CreateTOOL_PARAM_TEXTURE();
void CreateTOOL_PARAM_PIECES();
void CreateTOOL_PARAM_WATER();
void CreateTOOL_PARAM_BRIGHTNESS(byte type);
void CreateTOOL_PARAM_RELIEF(byte);
void CreateTOOL_PARAM_PAINT();
void CreateTOOL_PARAM_GROUND();
void CreateTOOL_PARAM_EDITROAD();
void PressCurrentEdModeButton(int cn){
	if(!EditMapMode)return;
	GP_Button* GB=(GP_Button*)MAPTOOLS.DSS[cn+1];
	if(CurrentEditMode!=-1){
		GP_Button* GBO=(GP_Button*)MAPTOOLS.DSS[CurrentEditMode+1];
		GBO->PassiveFrame=GBO->UserParam*3+2;
		GBO->ActiveFrame=GBO->UserParam*3+3;
	};
	GB->PassiveFrame=GB->UserParam*3+1;
	GB->ActiveFrame=GB->PassiveFrame;
	CurrentEditMode=GB->UserParam;
};
extern int HiStyle;
extern bool RiverEditMode;
extern int Inform;
extern bool OptHidden;
extern byte NeedToPopUp;
extern int InfDX0;
DialogsSystem TOOL_PARAM(0,0);

void ClearModes();
void CreateTOOL_PARAM_DEFAULT();
//-----------------------------------------------------------------------------
// ClearSurfTool
//-----------------------------------------------------------------------------
extern int CUR_TOOL_MODE;
void ClearSurfTool() {
	CUR_TOOL_MODE=0;
	CreateTOOL_PARAM_DEFAULT();
	CurrentEditMode = 0;
	Creator = 4096 + 255;
	//OptHidden = 0;
	Inform = 0;
	InfDX = InfDX0;
	ClearModes();
	if(!MAPTOOLS.DSS.NValues) return;
	if(CurrentEditMode != -1) {
		GP_Button *pButton = (GP_Button*)MAPTOOLS.DSS[CurrentEditMode + 1];
		pButton->PassiveFrame = 3 * CurrentEditMode + 2;
		pButton->ActiveFrame = 3 * CurrentEditMode + 3;
	}
	GP_Button *pButton = (GP_Button *)MAPTOOLS.DSS[1];
	pButton->PassiveFrame = 1;
	pButton->ActiveFrame = pButton->PassiveFrame;

	
	//CreateTOOL_PARAM_DEFAULT();
}

bool ON_TOOL_CLICK(SimpleDialog* SD){
	GP_Button* GB=(GP_Button*)SD;
	if(CurrentEditMode!=-1){
		GP_Button* GBO=(GP_Button*)MAPTOOLS.DSS[CurrentEditMode+1];
		GBO->PassiveFrame=GBO->UserParam*3+2;
		GBO->ActiveFrame=GBO->UserParam*3+3;
	};
	GB->PassiveFrame=GB->UserParam*3+1;
	GB->ActiveFrame=GB->PassiveFrame;
	CurrentEditMode=GB->UserParam;
	ClearModes();
	Creator=4096+255;
	OptHidden=0;
	Inform=0;
	InfDX=InfDX0;
	switch(GB->UserParam){
	case 0:
		CreateTOOL_PARAM_DEFAULT();
		break;
	case 1:
		ActivateSurfTool("Colorize");
//		CreateTOOL_PARAM_TEXTURE();
/*		CreateTOOL_PARAM_DEFAULT();*/
		break;
	case 2://select trees
		CreateTOOL_PARAM_SPRITE(0);
		break;
	case 3://select stones
		CreateTOOL_PARAM_SPRITE(1);
		break;
	case 4://erase objects
		CreateTOOL_PARAM_RELIEF(1);
		HeightEditMode=3;
		MEditMode=false;
		EditMedia=5;
		break;
	case 5:
		CreateTOOL_PARAM_DEFAULT();
		RiverEditMode=1;
		break;
	case 6://set water
		CreateTOOL_PARAM_WATER();
		WaterEditMode=1;
		break;
	case 7://clear water
		CreateTOOL_PARAM_WATER();
		WaterEditMode=2;
		break;
	case 8://set bright water
		CreateTOOL_PARAM_BRIGHTNESS(0);
		WaterEditMode=3;
		break;
	case 9://clear bright water
		CreateTOOL_PARAM_BRIGHTNESS(1);
		WaterEditMode=4;
		break;
	case 10://Hill up/down
		ActivateSurfTool("Deform");
//		CreateTOOL_PARAM_RELIEF(0);
//		HeightEditMode=1;
//		HiStyle=0;
		break;
	case 11://plane up/down
//		CreateTOOL_PARAM_RELIEF(0);
//		HeightEditMode=10;
//		HiStyle=0;
		break;
	case 12://equalise/distort surface
		ActivateSurfTool("Smooth");
//		CreateTOOL_PARAM_RELIEF(0);
//		HeightEditMode=11;
//		HiStyle=0;
		break;
	case 13://soft/distort surface
//		CreateTOOL_PARAM_RELIEF(0);
//		HeightEditMode=12;
//		HiStyle=0;
		break;
	case 14://draw curve with relief
		if(GetKeyState(VK_CONTROL)&0x8000){
			CreateTOOL_PARAM_DEFAULT();
			HeightEditMode=13;
			HiStyle=0;
		}else{
			CreateTOOL_PARAM_EDITROAD();
			void SetRoadsEditMode(bool Rmode);
			SetRoadsEditMode(1);			
		}
		break;
	case 15://paste piece
		CreateTOOL_PARAM_PIECES();
		break;
	case 16://unblock mode
		CreateTOOL_PARAM_DEFAULT();
		LockMode=1;
		break;
	case 17://block mode
		CreateTOOL_PARAM_DEFAULT();
		LockMode=2;
		break;
	case 18://default blocking
		CreateTOOL_PARAM_DEFAULT();
		LockMode=3;
		break;
	case 19://set unit
		CreateTOOL_PARAM_UNIT();
		break;
	case 20://create zone
		CreateTOOL_PARAM_DEFAULT();
		ZoneCmd=1;
		break;
	case 21://create group
		CreateTOOL_PARAM_DEFAULT();
		ZoneCmd=2;
		break;
	case 28:
		CreateTOOL_PARAM_PAINT();
		//ClearModes();
		//DrawPixMode=1;
		//NeedToPopUp=21;
		break;
	case 29:
		ActivateSurfTool("Facturize");
//		CreateTOOL_PARAM_GROUND();
//		ClearModes();
//		NeedToPopUp=22;
/*		CreateTOOL_PARAM_DEFAULT();
		ActivateEditorByName("Facturize");*/
		break;
	};
	Lpressed=0;
	UnPress();
	return true;
};
char* UNKED="???";

bool ToolsHidden=0;
bool OptHidden=0;
void ProcessTerrainEditor();
void CreateTotalLocking();
void ResearchIslands();
void Reset3D();
extern bool PeaceMode;
extern byte NeedToPopUp;
bool TBOX_DIALOGS(SimpleDialog* SD){
	switch(SD->UserParam){
	case 1:
		EditMiniMap();
		break;
	case 2:
		//ProcessTerrainEditor();
		SpecCmd=235;
		break;
	case 3:
		EraseAreas();
		rando();//!!
		CreateTotalLocking();
		ResearchIslands();
		break;
	case 4:
		Reset3D();
		break;
	case 5:
		//ProcessResEdit();
//		void SimpleEditClass(char* Name,BaseClass* BC,const char* XmlName,bool Autosave);
//		SimpleEditClass("Map options",&MOptions,"",false);
		void ShowMapOptions();
		ShowMapOptions();

		break;
	case 6:
		PeaceMode=!PeaceMode;
		break;
	};
	return true;
};
bool HideTool(SimpleDialog* SD){
	ToolsHidden=1;
	return true;
};
void CreateMapInterface(){
	MAPTOOLS.CloseDialogs();
	MAPTOOLS.HintX=-1;
	MAPTOOLS.HintY=280;
	MAPTOOLS.HintFont=&SmallBlackFont1;
	
	int x0=2;
	int y0=42;
		
	MAPTOOLS.addGPPicture(NULL,x0,y0,MapGP,0);
	//GPS.ShowGP(x0,y0,MapGP,93,0);
	
	int bx=x0+20;
	int by=y0+43;

	int bx2=x0+20;
	int by2=y0+410;

	int bdx=30;
	int bdy=30;

	int CrossDx=x0+57;
	int CrossDy=y0+11;

	for(int i=0;i<22;i++){
		int ii=i;
		if(ii>1) ii+=2;		
		GP_Button* BT=MAPTOOLS.addGP_Button(NULL,bx+(ii&1)*bdx,by+(ii/2)*bdy,MapGP,3+i*3,2+i*3);
		if(i==28||ii==7||ii==8||ii==9||ii==10||ii==11){
            BT->Diffuse=0x80000000;
			char HINID[256];
			sprintf(HINID,"$EHIN_%d",i);
			BT->Hint=GetTextByID(HINID);
			if(BT->Hint[0]=='$')BT->Hint=UNKED;
		}else{
			BT->OnUserClick=&ON_TOOL_CLICK;
			BT->UserParam=i;
			char HINID[256];
			sprintf(HINID,"$EHIN_%d",i);
			BT->Hint=GetTextByID(HINID);
			if(BT->Hint[0]=='$')BT->Hint=UNKED;
		}
	};	

	GP_Button* B2=NULL;

	B2=MAPTOOLS.addGP_Button(NULL,bx2,by2,MapGP,69,68);
	B2->OnUserClick=&TBOX_DIALOGS;
	B2->UserParam=1;
	B2->Hint=GetTextByID("$EHIN__0");

	B2=MAPTOOLS.addGP_Button(NULL,bx2+bdx,by2,MapGP,72,71);
	B2->OnUserClick=&TBOX_DIALOGS;
	B2->UserParam=2;
	B2->Hint=GetTextByID("$EHIN__1");

	B2=MAPTOOLS.addGP_Button(NULL,bx2,by2+bdy,MapGP,75,74);
	B2->OnUserClick=&TBOX_DIALOGS;
	B2->UserParam=3;
	B2->Hint=GetTextByID("$EHIN__2");

	B2=MAPTOOLS.addGP_Button(NULL,bx2+bdx,by2+bdy,MapGP,78,77);
	B2->OnUserClick=&TBOX_DIALOGS;
	B2->UserParam=4;
	B2->Hint=GetTextByID("$EHIN__3");

	B2=MAPTOOLS.addGP_Button(NULL,bx2,by2+bdy*2,MapGP,81,80);
	B2->OnUserClick=&TBOX_DIALOGS;
	B2->UserParam=5;
	B2->Hint=GetTextByID("$EHIN__4");

	B2=MAPTOOLS.addGP_Button(NULL,bx2+bdx,by2+bdy*2,MapGP,84,83);
	B2->OnUserClick=&TBOX_DIALOGS;
	B2->UserParam=6;
	B2->Hint=GetTextByID("$EHIN__5");

	B2=MAPTOOLS.addGP_Button(NULL,bx+(2&1)*bdx,by+(2/2)*bdy,MapGP,3+28*3,2+28*3);
	//B2->OnUserClick=&ON_TOOL_CLICK;
	//B2->UserParam=28;
	B2->Hint=GetTextByID("$EHIN__6");
	B2->Diffuse=0x80000000;

	B2=MAPTOOLS.addGP_Button(NULL,bx+(3&1)*bdx,by+(3/2)*bdy,MapGP,3+29*3,2+29*3);
	B2->OnUserClick=&ON_TOOL_CLICK;
	B2->UserParam=29;
	B2->Hint=GetTextByID("$EHIN__7");

	// cross
	GP_Button* HIDE=MAPTOOLS.addGP_Button(NULL,CrossDx,CrossDy,MapGP,92,91);
	HIDE->OnUserClick=&HideTool;

	MAPTOOLS.addGPPicture(NULL,x0,y0,MapGP,93);
};
bool DRAWLOCK=0;
void ClearUnderDialog(DialogsSystem* DSS){
	for(int i=0;i<DSS->DSS.GetAmount();i++){
		SimpleDialog* SD=DSS->DSS[i];
		if(SD&&SD->Visible&&mouseX>=SD->x&&mouseY>=SD->y&&mouseX<=SD->x1&&mouseY<=SD->y1){
			UnPress();
			Lpressed=0;
			ClearMStack();
			DRAWLOCK=1;
		};
	};
};
void ProcessTOOL_PARAM();
extern int CUR_TOOL_MODE;
extern byte NeedToPopUp;
void ProcessDIP_DSS();
void ProcessMapInterface(){
	DRAWLOCK=0;
	if(PlayGameMode){
		if(CUR_TOOL_MODE){
			NeedToPopUp=1;
		};
	};
	if(EditMapMode){		
		if(MI_Mode==1){
			MI_Mode=2;
			CreateMapInterface();
		};
		if(MI_Mode==2&&!ToolsHidden){
			//MAPTOOLS.ProcessDialogs();
			//ClearUnderDialog(&MAPTOOLS);
			void ShowAllToolsToolbar();
			ShowAllToolsToolbar();
		};
	};
	ProcessTOOL_PARAM();
	ISM->Flush();
	rsFlush();
};
ColoredBar* NCLB=NULL;
bool TP_Made=0;
int CUR_TOOL_MODE=0;
bool CH_NATION_CLICK(SimpleDialog* SD){
	NCLB->x=SD->x-2;
	NCLB->y=SD->y-2;
	NCLB->x1=NCLB->x+23-1;
	NCLB->y1=NCLB->y+17-1;
	SetMyNation(SD->UserParam);
	NCLB->Diffuse=((ColoredBar*)SD)->Diffuse;
	return true;
};
void CmdChangeNatRefTBL(byte* TBL);
extern bool BuildMode;
bool CH_NATION_CLICK_MP(SimpleDialog* SD){
	BuildMode=0;
	NCLB->x=SD->x-2;
	NCLB->y=SD->y-2;
	NCLB->x1=NCLB->x+23-1;
	NCLB->y1=NCLB->y+17-1;
	byte RTB[8];
	memcpy(RTB,GSets.CGame.cgi_NatRefTBL,8);
	RTB[MyNation]=SD->UserParam;
	CmdChangeNatRefTBL(RTB);
	NCLB->color=((ColoredBar*)SD)->color;
	return true;
};
bool CloseToolOpt(SimpleDialog* SD){
	OptHidden=1;
	realLpressed=0;
	Lpressed=0;
	UnPress();
	return true;
};
bool CheckFlagsNeed(){
	if(NPlayers<2)return false;
	if(SCENINF.hLib)return false;
	byte Mask=NATIONS[GSets.CGame.cgi_NatRefTBL[MyNation]].NMask;
	int N=0;
	for(int i=0;i<7;i++){
		if(NATIONS[i].NMask&Mask&&NATIONS[i].VictState!=1&&NATIONS[i].VictState!=2)N++;
	};
	return N>1;
};
CEXPORT DWORD GetNatColor( int natIdx );
void CreateFlags(int x0,int y0){
	if(NPlayers>1&&PlayGameMode==0){
		byte Mask=NATIONS[GSets.CGame.cgi_NatRefTBL[MyNation]].NMask;
		for(int i=0;i<8;i++){
			if(NATIONS[i].NMask&Mask&&NATIONS[i].VictState!=1&&NATIONS[i].VictState!=2){
				ColoredBar* BCX=TOOL_PARAM.addColoredBar(x0+2+i*28,y0+2,18,12,0xD0+i*4);
				BCX->OnUserClick=&CH_NATION_CLICK_MP;
				BCX->UserParam=i;
				if(i==GSets.CGame.cgi_NatRefTBL[MyNation]){
					NCLB=TOOL_PARAM.addColoredBar(x0+i*24,y0,22,16,0xFF);
                    NCLB->Diffuse=GetNatColor(i);
					NCLB->x=BCX->x-2;
					NCLB->y=BCX->y-2;
					NCLB->x1=NCLB->x+23-1;
					NCLB->y1=NCLB->y+17-1;
					NCLB->Style=1;
				};
			}else{
				ColoredBar* BCX=TOOL_PARAM.addColoredBar(x0+2+i*28+7,y0+2+4,18-14,12-8,0xFF);
				BCX->Diffuse=GetNatColor(i);
				BCX->Style=1;
			};
		};
	}else{
		for(int i=0;i<8;i++){
			ColoredBar* BCX=TOOL_PARAM.addColoredBar(x0+2+i*28,y0+2,18,12,0xFF);
			BCX->Diffuse=GetNatColor(i);
			BCX->OnUserClick=&CH_NATION_CLICK;
			BCX->UserParam=i;
			if(i==MyNation){
				NCLB=TOOL_PARAM.addColoredBar(x0+i*24,y0,22,16,0xFF);
				NCLB->Diffuse=GetNatColor(i);
				NCLB->x=BCX->x-2;
				NCLB->y=BCX->y-2;
				NCLB->x1=NCLB->x+23-1;
				NCLB->y1=NCLB->y+17-1;
				NCLB->Style=1;
			};
		};
	};
	GP_Button* B=TOOL_PARAM.addGP_Button(NULL,InfDX-32-1,-27-6,MapGP,92,91);
	B->OnUserClick=&CloseToolOpt;
};
extern int UNI_LINEDLY1;
extern int UNI_LINEDY1;
extern int  ReliefBrush;
extern VScrollBar* TP_Scroll;
void StdWheel(int Mult=1){
	if(mouseX>RealLx-256-20&&TP_Scroll&&TP_Scroll->Visible){
		if(WheelDelta){
			TP_Scroll->SPos-=WheelDelta*Mult/120;
			if(TP_Scroll->SPos<0)TP_Scroll->SPos=0;
			if(TP_Scroll->SPos>TP_Scroll->SMaxPos)TP_Scroll->SPos=TP_Scroll->SMaxPos;
		};
	}else{ 
		if(WheelDelta){
			ReliefBrush+=WheelDelta/120;
		};
#ifdef _USE3D
		if(ReliefBrush<0)ReliefBrush=0;
#else
		if(ReliefBrush<1)ReliefBrush=1;
#endif
		if(ReliefBrush>110)ReliefBrush=110;
	};
	WheelDelta=0;
};
void StdWheel2(int Mult=1){
	if(mouseX>RealLx-256-20&&TP_Scroll&&TP_Scroll->Visible){
		if(WheelDelta){
			TP_Scroll->SPos-=WheelDelta*Mult/120;
			if(TP_Scroll->SPos<0)TP_Scroll->SPos=0;
			if(TP_Scroll->SPos>TP_Scroll->SMaxPos)TP_Scroll->SPos=TP_Scroll->SMaxPos;
			WheelDelta=0;
		};
	}	
};
void CreateTOOL_PARAM_DEFAULT(){
	CUR_TOOL_MODE=0;
	int DY=16+UNI_LINEDLY1;
	int DDY=UNI_LINEDY1;
	
	TOOL_PARAM.CloseDialogs();
	TP_Scroll=NULL;
	int DD1=75;
	int D=18;
	int x0=RealLx-InfDX;
	int y0=InfDY;
	TOOL_PARAM.BaseX=x0-1;
	TOOL_PARAM.BaseY=y0+31;
	TOOL_PARAM.addBorder(0,-31,InfDX-10,38,0,1);
	TOOL_PARAM.addTextButton(NULL,10,HDR_FDY,GetTextByID("ED_HDR00"),WinTool,WinTool,WinTool,0);
	CreateFlags(10,10);
};
//UNIT parameters
int TP_CurNation=0;
ComboBox* TP_GPB=NULL;
SimpleDialog* TP_STARTSCROLL;
SimpleDialog* TP_ENDSCROLL;
VScrollBar* TP_Scroll=NULL;
int TP_YPREV=0;
//----------------

bool TP_UNIT_CLICK(SimpleDialog* SD){
	ColoredBar* CB=(ColoredBar*)SD;
	//byte CC=0x65;
	for(int i=0;i<TOOL_PARAM.DSS.GetAmount();i++)if(TOOL_PARAM.DSS[i]&&TOOL_PARAM.DSS[i]->UserParam>=10000){
		((ColoredBar*)TOOL_PARAM.DSS[i])->Diffuse=0xFF808080;
	};
	CB->Diffuse=0xFFFFFF00;
	Creator=CB->UserParam-10000;
	return true;
};
bool _DrawExIcon(SimpleDialog* SD){
	NewMonster* NM=NATIONS->Mon[SD->UserParam]->newMons;
	NM->ExIcon.Draw(SD->x,SD->y,SD->x1-SD->x,SD->y1-SD->y,MyNation);
	return true;
}
void CreateTOOL_PARAM_UNIT(){
	CUR_TOOL_MODE=1;
	int DY=16+UNI_LINEDLY1;
	int DDY=UNI_LINEDY1;
	
	TOOL_PARAM.CloseDialogs();
	TP_Scroll=NULL;
	int DD1=75;
	int D=18;
	int x0=RealLx-InfDX;
	int y0=InfDY;
	TOOL_PARAM.HintFont=&SmallWhiteFont;
	TOOL_PARAM.HintX=-1;
	TOOL_PARAM.HintY=450;
	TOOL_PARAM.BaseX=x0-10;
	TOOL_PARAM.BaseY=y0+31;
	TOOL_PARAM.addBorder(0,-31,InfDX-10,386,48+26,3);
	TOOL_PARAM.addTextButton(NULL,10,HDR_FDY,GetTextByID("ED_HDR01"),WinTool,WinTool,WinTool,0);
	CreateFlags(10,10);
	TP_GPB=TOOL_PARAM.addGP_ComboBoxDLX(NULL,10,10+26,224,CBB_GPFILE,0,9,0,&BlackFont,&BlackFont,NULL);
	for(int i=0;i<NNations;i++)TP_GPB->AddLine(NatNames[i]);
	TP_GPB->CurLine=TP_CurNation;
	//Units list
	TP_Scroll=TOOL_PARAM.addNewGP_VScrollBar(NULL,InfDX-21-13,48+26+1+7,312-9,100,0,ScrollGP,0);
	TP_Scroll->ScrDy=200;
	TP_Scroll->OnesDy=32;
	TOOL_PARAM.addClipper(TOOL_PARAM.BaseX+4,TOOL_PARAM.BaseY+48+26+1,RealLx,TOOL_PARAM.BaseY+311+48+26+1);
	TP_STARTSCROLL=NULL;
	if( TP_CurNation<0 || TP_CurNation>=NNations ) TP_CurNation=0;
	int NUNITS=NATIONS->NUnits[TP_CurNation];
	int xx=0;
	int yy=48+26+6;
	byte CC=0x65;
	int IconLX=40;
	int IconLY=40;
	int igp=GPS.PreLoadGPImage("icons");
	IconLX=GPS.GetGPWidth(igp,0);
	IconLY=GPS.GetGPHeight(igp,0);
	if(IconLX==0)IconLX=40;
	if(IconLY==0)IconLY=40;
	int NU=45*5/(IconLX+4);
	for(int j=0;j<NUNITS;j++){
		int UIDS=NATIONS->UnitsIDS[TP_CurNation][j];
		if(UIDS==0xFFFF){
			if(xx)yy+=IconLY+4;
			TextButton* TB=TOOL_PARAM.addTextButton(NULL,10,yy,GetTextByID(NATIONS->UnitNames[TP_CurNation][j]),&YellowFont,&YellowFont,&YellowFont,0);
			if(!TP_STARTSCROLL)TP_STARTSCROLL=TB;
			yy+=20;
			xx=0;
		}else {
			GeneralObject* GO=NATIONS->Mon[UIDS];
			if(xx>=NU){
				xx=0;
				yy+=IconLY+4;
			};
			if(GO->newMons->IconID!=0xFFFF){
				if(!GO->newMons->ExIcon.Icons.GetAmount())TOOL_PARAM.addGPPicture(NULL,6+xx*(IconLX+4),yy,0,GO->newMons->IconID);
				else{
					ColoredBar* CB=TOOL_PARAM.addColoredBar(5+xx*(IconLX+4),yy-1,IconLX+1,IconLY+1,0xFF);
					CB->Style=1;					
					CB->UserParam=UIDS;					
					CB->OnDraw=&_DrawExIcon;
				}
				ColoredBar* CB=TOOL_PARAM.addColoredBar(5+xx*(IconLX+4),yy-1,IconLX+1,IconLY+1,0xFF);
				CB->Style=1;
				CB->OnUserClick=&TP_UNIT_CLICK;
				CB->UserParam=10000+UIDS;
				CB->Diffuse=0xFF808080;
				CB->Hint=GO->Message;
			};
			xx++;
		};
	};
	if(xx)yy+=IconLY+4;
	TP_ENDSCROLL=TOOL_PARAM.addClipper(0,0,RealLx-1,RealLy-1);
	TP_YPREV=0;
	TP_Scroll->SMaxPos=yy-48-26-4-312+8;
	if(TP_Scroll->SMaxPos<0)TP_Scroll->Visible=0;
};
extern bool NoPress;
void ProcessTOOL_PARAM_UNIT(){
	if(TP_GPB){
		if(TP_GPB->CurLine!=TP_CurNation){
			TP_CurNation=TP_GPB->CurLine;
			CreateTOOL_PARAM_UNIT();
		};
		StdWheel2(40);
		bool StartChange=0;
		int CURRPOS=TP_Scroll->SPos;
		for(int i=0;i<TOOL_PARAM.DSS.GetAmount();i++){
			SimpleDialog* SD=TOOL_PARAM.DSS[i];
			if(SD){
				if(SD==TP_STARTSCROLL)StartChange=1;
				if(SD==TP_ENDSCROLL)StartChange=0;
				if(StartChange){
					SD->y+=TP_YPREV-CURRPOS;
					SD->y1+=TP_YPREV-CURRPOS;
				};
			};
		};
		TP_YPREV=CURRPOS;
	};
};
//----------ROADS----------
#ifdef _USE3D
ComboBox* TP_REDP=NULL;
VScrollBar* RSCRL=NULL;
void EnumGS_Types(char* dest);
int TP_CurRoad=0;
int R_CurGrp=0;
int ActualNRoads=0;
int ARoadsRef[256];
int RTEXDY=0;
bool NeedRoadRefresh=0;
bool ChangeRoad(SimpleDialog* SD){
	TP_CurRoad=SD->UserParam;
	NeedRoadRefresh=1;
	return true;
};

bool CreateRoadPreview(int t,BaseMeshDialog* BM);

void CreateTOOL_PARAM_EDITROAD(){
	CUR_TOOL_MODE=14;
	int DY=16+UNI_LINEDLY1;
	int DDY=UNI_LINEDY1;
	TOOL_PARAM.CloseDialogs();
	TP_Scroll=NULL;
	int DD1=75;
	int D=18;
	int x0=RealLx-InfDX;
	int y0=InfDY;
	TOOL_PARAM.HintFont=&SmallWhiteFont;
	TOOL_PARAM.HintX=-1;
	TOOL_PARAM.HintY=450;
	TOOL_PARAM.BaseX=x0-10;
	TOOL_PARAM.BaseY=y0+31;
	TOOL_PARAM.addBorder(0,-31,InfDX-10,386,48+26,3);
	TOOL_PARAM.addTextButton(NULL,10,HDR_FDY,GetTextByID("ED_ROADEDITOR"),WinTool,WinTool,WinTool,0);
	CreateFlags(10,10);
	TP_REDP=TOOL_PARAM.addGP_ComboBoxDLX(NULL,10,10+26,224,CBB_GPFILE,0,9,0,&WhiteFont,&YellowFont,NULL);
	void EnumGS_Types(char* dest);
	char* GetRoadsNames();
	TP_REDP->AddComplexLine(GetRoadsNames());
	TP_REDP->CurLine=R_CurGrp;
	int GetRoadIMG(int i,int& spr,int& grp);
	int GetNRTypes();
	int N=GetNRTypes();
	int Y0=82;
	int Y00=Y0;
	int NN=0;
	int xx0=TOOL_PARAM.BaseX;
	int yy0=TOOL_PARAM.BaseY;
	RSCRL=TOOL_PARAM.addNewGP_VScrollBar(NULL,220,Y0,305,10000,0,ScrollGP,0);
	TOOL_PARAM.addClipper(xx0+10,yy0+Y0,xx0+210+10,yy0+Y0+300);
	int CURY=-1;
	ActualNRoads=0;
	for(int i=0;i<N;i++){
		BaseMeshDialog* BMD=TOOL_PARAM.addBaseMeshDialog(10,Y0-RTEXDY,210,10);		
		if(CreateRoadPreview(i,BMD)){
			ARoadsRef[ActualNRoads]=i;			
			if(ActualNRoads==TP_CurRoad){
				ColoredBar* CB=TOOL_PARAM.addColoredBar(10,Y0-RTEXDY,BMD->GetWidth(),BMD->GetHeight(),0xFF);
				CB->Style=1;
				CURY=Y0-RTEXDY;
			};
			BMD->OnUserClick=&ChangeRoad;
			BMD->UserParam=ActualNRoads;
			NN++;
			Y0+=BMD->GetHeight();
			ActualNRoads++;
			BMD=new BaseMeshDialog;
		}else{
			TOOL_PARAM.DSS.DelElement(TOOL_PARAM.DSS.GetAmount()-1);
		}
	};

	TOOL_PARAM.addClipper(0,0,RealLx-1,RealLy-1);
	RSCRL->SetMaxPos(Y0-300-Y00);
	RSCRL->SPos=RTEXDY;
	if(CURY==-1){
		TP_CurRoad=0;
		NeedRoadRefresh=1;
	};
	if(RSCRL->SMaxPos<=0){
		if(RTEXDY)NeedRoadRefresh=1;
		RTEXDY=0;
		RSCRL->SPos=0;		
	};
	if(CURY<Y00){
		RSCRL->SPos-=Y00-CURY;
	};
	if(CURY>Y00+200){
		RSCRL->SPos+=CURY-Y00-200;
	};
};
extern bool NoPress;
void ProcessTOOL_PARAM_EDITROAD(){
	if(TP_REDP){
		if(TP_REDP->CurLine!=R_CurGrp){
			R_CurGrp=TP_REDP->CurLine;
			NeedRoadRefresh=0;
			CreateTOOL_PARAM_EDITROAD();
		};
		if(WheelDelta){
			int N=ActualNRoads;
			if(WheelDelta>0){
				TP_CurRoad--;
				NeedRoadRefresh=1;
			}else{
				TP_CurRoad++;
				NeedRoadRefresh=1;
			};
			WheelDelta=0;
			if(TP_CurRoad<0)TP_CurRoad=N-1;
			if(TP_CurRoad>=N)TP_CurRoad=0;
		};
		if(NeedRoadRefresh){
			NeedRoadRefresh=0;
			CreateTOOL_PARAM_EDITROAD();			
		};
		RSCRL->Enabled=RSCRL->SMaxPos>0;
		if(RSCRL->SMaxPos<0)RSCRL->SPos=0;
		if(RSCRL->SPos!=RTEXDY && RSCRL->SMaxPos>0){
			int DY=RSCRL->SPos-RTEXDY;
			RTEXDY=RSCRL->SPos;
			//CreateTOOL_PARAM_EDITROAD();
			int CANMOVE=0;
			for(int i=0;i<TOOL_PARAM.DSS.GetAmount();i++){
				SimpleDialog* SD=TOOL_PARAM.DSS[i];
				if(SD){
					if(SD->x==0)CANMOVE=0;
					if(CANMOVE>2){
						SD->y-=DY;
						SD->y1-=DY;
					};
					if(SD==RSCRL){
						CANMOVE=1;
					};
				};
				if(CANMOVE)CANMOVE++;
			};
		};
	};
};
#endif //_USE3D
//---------SPRITES---------
SprGroup* CURSG;
extern SprGroup WALLS;
#define AC_COLOR 0xFF
#define PS_COLOR 92
bool ON_SPRITE_CLICK(SimpleDialog* SD){
	ColoredBar* CB=(ColoredBar*)SD;
	NTreeItems=0;
	if((GetKeyState(VK_CONTROL)&0x8000)&&(GetKeyState(VK_SHIFT)&0x8000)){
		for(int i=0;i<TOOL_PARAM.DSS.GetAmount();i++)if(TOOL_PARAM.DSS[i]){
			if(TOOL_PARAM.DSS[i]->UserParam>=10000){
				((ColoredBar*)TOOL_PARAM.DSS[i])->color=AC_COLOR;
			};
		};
	}else
	if(GetKeyState(VK_CONTROL)&0x8000){
		if(CB->color==PS_COLOR)CB->color=AC_COLOR;
		else CB->color=PS_COLOR;
	}else{
		for(int i=0;i<TOOL_PARAM.DSS.GetAmount();i++)if(TOOL_PARAM.DSS[i]){
			if(TOOL_PARAM.DSS[i]->UserParam>=10000){
				((ColoredBar*)TOOL_PARAM.DSS[i])->color=PS_COLOR;
			};
		};
		CB->color=AC_COLOR;
	};
	NTreeItems=0;
	for(int i=0;i<TOOL_PARAM.DSS.GetAmount();i++)if(TOOL_PARAM.DSS[i]){
		if(TOOL_PARAM.DSS[i]->UserParam>=10000){
			if(((ColoredBar*)TOOL_PARAM.DSS[i])->color==AC_COLOR){
				TreeItems[NTreeItems]=TOOL_PARAM.DSS[i]->UserParam-10000;
				NTreeItems++;
			};
		};
	};
	if(NTreeItems){
		HeightEditMode=2;
		MEditMode=false;
		BlobMode=-1;
		if(CURSG==&TREES)InteriorType=0;
		if(CURSG==&STONES)InteriorType=1;
		if(CURSG==&ANMSPR)InteriorType=2;
		if(CURSG==&WALLS)InteriorType=3;
	}else{
		ClearModes();
		HeightEditMode=false;
		BlobMode=-1;
		WaterEditMode=0;
		Creator=4096+255;
	};
	return true;
};
void SelectSprItem(bool Dir){
	if(NTreeItems!=1)return;
	int Items[256];
	int NItm=0;
	int citm=-1;
	for(int i=0;i<TOOL_PARAM.DSS.GetAmount();i++)if(TOOL_PARAM.DSS[i]){
		if(TOOL_PARAM.DSS[i]->UserParam>=10000){
			if(NItm<256){
				ColoredBar* CB=(ColoredBar*)TOOL_PARAM.DSS[i];
				if(CB->color==AC_COLOR)citm=NItm;
				Items[NItm]=CB->UserParam-10000;
				NItm++;
				CB->color=PS_COLOR;
			};
		};
	};
	if(citm!=-1){
		if(Dir){
			citm++;
			if(citm>=NItm)citm=0;
		}else{
			citm--;
			if(citm<0)citm=NItm-1;
		};
		int cpr=Items[citm]+10000;
		for(int i=0;i<TOOL_PARAM.DSS.GetAmount();i++)if(TOOL_PARAM.DSS[i]){
			if(TOOL_PARAM.DSS[i]->UserParam==cpr){
				ColoredBar* CB=(ColoredBar*)TOOL_PARAM.DSS[i];
				CB->color=AC_COLOR;
				TreeItems[0]=cpr-10000;
			};
		};
	};
};
VScrollBar* TP_BRS;
int PrevRelBr=10;
int Cur_SprID=0;
int CurSection[4]={0,0,0,0};
ComboBox* TP_CSECT=NULL;
int cursection=0;
int SP_SCROLL0=0;
int SP_SCROLL1=0;
bool DrawSprThumbnail(SimpleDialog* SD){
	try{
		TempWindow TW;
		IRS->SetShaderConst(0,5000);
		PushWindow(&TW);
		PushEntityContext(DWORD(SD));
		DrawThumbnail(SD->UserParam,Rct(SD->x,SD->y,SD->GetWidth(),SD->GetHeight()),0xFFFFFFFF,0,0.01,0,0.7);
		PopEntityContext();
		IntersectWindows(0,0,1,1);
		PopWindow(&TW);
	}catch(...){
		FILE* F=fopen("badeffects.txt","a");
		if(F){
			fprintf(F,"%s\n",IMM->GetModelFileName(SD->UserParam));
			fclose(F);
		}
	}
	return true;
}

float sprMaxFiAngle=0.0f;
float sprMaxScale=1.0f;
float sprMaxThetaAngle=0.0f;
float sprExraZ=0.0f;

void CreateTOOL_PARAM_SPRITE(byte ID){
	sprExraZ=0;
	CUR_TOOL_MODE=2;
	ClearModes();
	BlobMode=-1;
	WaterEditMode=0;
	Creator=4096+255;
	HeightEditMode=2;
	int DY=16+UNI_LINEDLY1;
	int DDY=UNI_LINEDY1;

	if(ID==0)CURSG=&TREES;
	else if(ID==1)CURSG=&STONES;
	else if(ID==2)CURSG=&ANMSPR;
	else if(ID==3)CURSG=&WALLS;
	else CURSG=&HOLES;
	
	TOOL_PARAM.CloseDialogs();
	TP_Scroll=NULL;
	int DD1=75;
	int D=18;
	int x0=RealLx-InfDX;
	int y0=InfDY;
	TOOL_PARAM.HintFont=&SmallWhiteFont;
	TOOL_PARAM.HintX=-1;
	TOOL_PARAM.HintY=450;
	TOOL_PARAM.BaseX=x0-1;
	TOOL_PARAM.BaseY=y0+31;
	int DL=24;
	TOOL_PARAM.addBorder(0,-31,InfDX-10,386,88+DL/*+CURSG->NSections?24:0*/,3);
	TOOL_PARAM.addTextButton(NULL,10,HDR_FDY,GetTextByID("ED_HDR02"),WinTool,WinTool,WinTool,0);
	CreateFlags(10,10);

	int Y0=36;
	TOOL_PARAM.addGP_Button(NULL,10,Y0-4,MapGP+1,0,0);
	TOOL_PARAM.addGP_Button(NULL,10+34,Y0,MapGP+1,3,2);
	TP_BRS=TOOL_PARAM.addGP_ScrollBarL(NULL,10+35,Y0+10,110,ReliefBrush,MapGP+1,4,192,12,0,0);
	
	//Units list
	if(CURSG->Sections.GetAmount()){
		TP_CSECT=TOOL_PARAM.addGP_ComboBoxDLX(NULL,10,48+1+DL,230,CBB_GPFILE,0,9,0,&BlackFont,&BlackFont,NULL);
		TP_CSECT->Center-=5;
		for(int i=0;i<CURSG->Sections.GetAmount();i++)TP_CSECT->AddLine(CURSG->Sections[i]->SectionName.pchar());
		TP_CSECT->CurLine=CurSection[ID];
		DL+=24;
	};
	DL+=22;
	TP_Scroll=TOOL_PARAM.addNewGP_VScrollBar(NULL,InfDX-21-16,48+1+DL,312+26-DL,100,0,ScrollGP,0);
	TP_Scroll->ScrDy=200;
	TP_Scroll->OnesDy=32;
	TOOL_PARAM.addClipper(TOOL_PARAM.BaseX,TOOL_PARAM.BaseY+48+1+DL,TOOL_PARAM.BaseX+223,TOOL_PARAM.BaseY+311+48+26+1);
	
	int xx=0;
	int yy=48+4+DL;
	int MaxLy=0;
	int Nx=0;
	int TotLx=0;
	int MaxLx=InfDX-35;
	TP_STARTSCROLL=NULL;
	int nsect=CURSG->Sections.GetAmount();
	int csect=CurSection[ID];
	cursection=csect;
	int n=nsect?CURSG->Sections[csect]->ObjectsList.GetAmount():CURSG->Objects.GetAmount();
	Cur_SprID=ID;
	for(int i=0;i<=n;i++){
		int sprx,spr;
		ObjCharacter* OC;
		if(i<n){
			if(nsect){
				OC=CURSG->Sections[csect]->ObjectsList[i]->Get();
				if(OC){
					sprx=-1;
					for(int q=0;q<CURSG->Objects.GetAmount();q++)if(CURSG->Objects[q]==OC)sprx=q;
					if(q==-1){
						sprx=0;
						OC=NULL;
					}
				}
			}else{
				sprx=i;
				OC=CURSG->Objects[sprx];
			}			
		}else{
			sprx=0;
			spr=0;
			OC=NULL;
		}		
		if(OC&&OC->FixDx){
			int y0=30;
			if(spr>=4096){
				GPPicture* GPP=GPP=TOOL_PARAM.addGPPicture(NULL,xx+OC->FixDx+OC->CenterX
					,y0+OC->FixDy-OC->CenterY,OC->FileID_forPreview,OC->SpriteID_forPreview);
			}else{
				GPPicture* GPP=GPP=TOOL_PARAM.addGPPicture(NULL,xx+OC->FixDx-OC->CenterX
					,y0+OC->FixDy-OC->CenterY,OC->FileID_forPreview,OC->SpriteID_forPreview);
			};
			byte ang=OC->FixDir;
			int r=OC->FixR;
			int xx0=xx+OC->FixDx+((TCos[ang]*r)>>8);
			int yy0=y0+OC->FixDy+((TSin[ang]*r)>>9);
			ColoredBar* CB=TOOL_PARAM.addColoredBar(xx0-4,yy0-4,8,8,PS_COLOR);
			CB->OnUserClick=&ON_SPRITE_CLICK;
			CB->UserParam=sprx+10000;
			CB->Style=1;
		}else if(OC){
			int Lx,Ly,dxx,dyy;
			if(OC->ModelManagerID>0){
                Lx=64;
				Ly=80;
				dxx=32;
				dyy=32;
			}else{
				Lx=GPS.GetGPWidth(OC->FileID_forPreview,OC->SpriteID_forPreview&4095);
				Ly=GPS.GetGPHeight(OC->FileID_forPreview,OC->SpriteID_forPreview&4095);			
				dxx,dyy;
				Rct F;
				GPS.GetGPBoundFrame(OC->FileID_forPreview,OC->SpriteID_forPreview&4095,F);
				dxx=F.x;
				dyy=F.y;
				Lx=F.w;
				Ly=F.h;		
			}			
			if(TotLx+Lx>=MaxLx||i>=n-1){							
				if(i==n-1){
					Nx++;
					TotLx+=Lx;
					if(Ly>MaxLy)MaxLy=Ly;
					i++;
				}
				int dx=MaxLx-TotLx;
				for(int j=max(0,i-Nx);j<i;j++){
					int tdx0=((j-i+Nx)*dx)/Nx;
					int tdx1=((j-i+Nx+1)*dx)/Nx;
					int L1=tdx1-tdx0;
					int spr1x=0;
					ObjCharacter* OCX=NULL;
					if(nsect){
						OCX=CURSG->Sections[csect]->ObjectsList[j]->Get();
						if(OC){
							sprx=-1;
							for(int q=0;q<CURSG->Objects.GetAmount();q++)if(CURSG->Objects[q]==OCX)spr1x=q;
							if(q==-1){
								spr1x=0;
								OCX=NULL;
							}
						}
					}
					if(OCX){
						int Lx,Ly,dxx,dyy;
						if(OCX->ModelManagerID>0){
							Lx=64;
							Ly=80;
							dxx=32;
							dyy=32;
							ColoredBar* CB0=TOOL_PARAM.addColoredBar(xx+tdx0,yy,L1+Lx-1,MaxLy-1,0);
							CB0->UserParam=OCX->ModelManagerID;
							CB0->OnDraw=&DrawSprThumbnail;
							ColoredBar* CB=TOOL_PARAM.addColoredBar(xx+tdx0,yy,L1+Lx-1,MaxLy-1,PS_COLOR);
							CB->OnUserClick=&ON_SPRITE_CLICK;
							CB->UserParam=spr1x+10000;
							CB->Style=1;
							if(!TP_STARTSCROLL)TP_STARTSCROLL=CB0;
							xx+=Lx;
						}else{
							int spr1=OCX->SpriteID_forPreview;
							Lx=GPS.GetGPWidth(OCX->FileID_forPreview,spr1&4095);
							int RLX=Lx;
							Ly=GPS.GetGPHeight(OCX->FileID_forPreview,spr1&4095);
							Rct F;
							GPS.GetGPBoundFrame(OCX->FileID_forPreview,spr1&4095,F);
							int dxx=F.x;
							int dyy=F.y;
							Lx=F.w;
							Ly=F.h;		
							GPPicture* GPP;
							if(spr1>=4096){
								int dx2=OCX->CenterX;
								GPP=TOOL_PARAM.addGPPicture(NULL,xx+tdx0+L1/2-dxx+dx2+dx2,yy-dyy+(MaxLy-Ly)/2,OCX->FileID_forPreview,spr1);
							}else GPP=TOOL_PARAM.addGPPicture(NULL,xx+tdx0+L1/2-dxx,yy-dyy+(MaxLy-Ly)/2,OCX->FileID_forPreview,spr1);
							if(!TP_STARTSCROLL)TP_STARTSCROLL=GPP;
							ColoredBar* CB=TOOL_PARAM.addColoredBar(xx+tdx0,yy,L1+Lx-1,MaxLy-1,PS_COLOR);
							CB->OnUserClick=&ON_SPRITE_CLICK;
							CB->UserParam=spr1x+10000;
							CB->Style=1;
							xx+=Lx;
						}						
					}
				};
				xx=0;
				yy+=MaxLy;
				TotLx=0;
				MaxLy=0;
				Nx=0;
			};
			Nx++;
			TotLx+=Lx;
			if(Ly>MaxLy)MaxLy=Ly;			
		};
	};
	TP_ENDSCROLL=TOOL_PARAM.addClipper(0,0,RealLx-1,RealLy-1);
	TP_YPREV=0;
	TP_Scroll->SMaxPos=yy-48-4-312+8-20;
	if(TP_Scroll->SMaxPos<0)TP_Scroll->Visible=0;
	if(CURSG==&TREES)TP_Scroll->SPos=SP_SCROLL0;
	if(CURSG==&STONES)TP_Scroll->SPos=SP_SCROLL1;
	NTreeItems=0;
};
int GetTotalHeight(int,int);

void ProcessTOOL_PARAM_SPRITES(){

	if(ReliefBrush!=PrevRelBr){
		PrevRelBr=ReliefBrush;
		TP_BRS->SPos=ReliefBrush;
	};
	ReliefBrush=TP_BRS->SPos;
	bool StartChange=0;
	int CURRPOS=TP_Scroll->SPos;
	for(int i=0;i<TOOL_PARAM.DSS.GetAmount();i++){
		SimpleDialog* SD=TOOL_PARAM.DSS[i];
		if(SD){
			if(SD==TP_STARTSCROLL)StartChange=1;
			if(SD==TP_ENDSCROLL)StartChange=0;
			if(StartChange){
				SD->y+=TP_YPREV-CURRPOS;
				SD->y1+=TP_YPREV-CURRPOS;
			};
		};
	};
	TP_YPREV=CURRPOS;
	//StdWheel();
	ObjCharacter* OCC=NULL;
	int sp=TreeItems[0];
	if(sp<CURSG->Objects.GetAmount()){
		OCC=CURSG->Objects[sp];
	}
	//if(OCC&&!OCC->EditableModel){
	bool SHIFT=GetKeyState(VK_SHIFT)&0x8000;
	bool ALT=GetKeyState(VK_MENU)&0x8000;
	bool CONTROL=GetKeyState(VK_CONTROL)&0x8000;
	if(!(ALT||SHIFT)){
		if(GetKeyState(VK_CONTROL)&0x8000){
			StdWheel();
		}else if(WheelDelta){
			if(WheelDelta>0)SelectSprItem(1);
			else SelectSprItem(0);
			WheelDelta=0;
		}
	}else{
		if(WheelDelta){
			if(ALT){
				if(OCC && OCC->EditableModel)sprExraZ+=WheelDelta/120.0f;
				else sprMaxThetaAngle+=WheelDelta/2400.0f;
			}
			if(SHIFT)sprMaxScale+=WheelDelta/2400.0f;
			if(sprMaxScale<0.01)sprMaxScale=0.01;
			if(GetKeyState(VK_HOME)&0x8000){
				sprMaxScale=1.0f;
				sprMaxThetaAngle=0.0f;
			}
		}
	}
	//}
	if(CURSG->Sections.GetAmount()&&cursection!=TP_CSECT->CurLine){
		CurSection[Cur_SprID]=TP_CSECT->CurLine;
		CreateTOOL_PARAM_SPRITE(Cur_SprID);
		TP_Scroll->SPos=0;
	};
	if(NTreeItems>0){
		int sp=TreeItems[0];
		if(sp<CURSG->GetNSprites()){
			ObjCharacter* OC=CURSG->Objects[sp];
			if((GetKeyState(VK_TAB)||InteriorType==3)&&mouseX<RealLx-256-20&&Shifter==5){
				extern int LastMx;
				extern int LastMy;
				int x=LastMx;
				int y=LastMy;
				if(!(GetKeyState(VK_CONTROL)&0x8000)){
					void FindBestPositionForSprite(SprGroup* SG,int Index,int& x,int& y);
					FindBestPositionForSprite(CURSG,sp,x,y);
				}
				int z=GetTotalHeight(x,y);	
				if(OC->ViewType==1){
					int z=OC->FixHeight>-1000?OC->FixHeight:GetTotalHeight(x,y);
					Matrix4D M4;
					M4.translation(x,y,z);
					OC->GetMatrix4D(M4,x,y,z);
					Matrix4D S;
					S.srt(sprMaxScale,Vector3D(1,0,0),sprMaxThetaAngle,Vector3D(0,0,sprExraZ));
					M4.mulLeft(S);
					GPS.EnableZBuffer(true);
					PushEntityContext(DWORD(OC));
					IMM->StartModel(OC->ModelManagerID,M4);
                    IMM->DrawModel();
					PopEntityContext();
				}else
				if(OC->HaveAligning){
					OneSprite OS;
					OS.x=x;
					OS.y=y;
					OS.OC=OC;
					OS.M4=NULL;
					OS.CreateMatrix();
					GPS.DrawWSprite(OC->FileID_forPreview,OC->SpriteID_forPreview,*OS.M4,0);				
					delete(OS.M4);
				}else{
					x-=mapx<<5;
					y=(y>>1)-(mapy<<4)-z;
					GPS.ShowGP(x-CURSG->Objects[sp]->CenterX,y-CURSG->Objects[sp]->CenterY,CURSG->Objects[sp]->FileID_forPreview,CURSG->Objects[sp]->SpriteID_forPreview,0);
				}
			}
		}
	}
	if(CURSG==&TREES)SP_SCROLL0=TP_Scroll->SPos;
	if(CURSG==&STONES)SP_SCROLL1=TP_Scroll->SPos;
};
#ifdef _USE3D
int CUR_GS_TYPE=0;
void CreateTOOL_PARAM_GSPRITE(){
	CUR_TOOL_MODE=23;
	ClearModes();
	BlobMode=-1;
	WaterEditMode=0;
	Creator=4096+255;
	HeightEditMode=128+CUR_GS_TYPE;
	int DY=16+UNI_LINEDLY1;
	int DDY=UNI_LINEDY1;
	TOOL_PARAM.CloseDialogs();
	TP_Scroll=NULL;
	int DD1=75;
	int D=18;
	int x0=RealLx-InfDX;
	int y0=InfDY;
	TOOL_PARAM.HintFont=&SmallWhiteFont;
	TOOL_PARAM.HintX=-1;
	TOOL_PARAM.HintY=450;
	TOOL_PARAM.BaseX=x0-1;
	TOOL_PARAM.BaseY=y0+31;
	int DL=24;
	TOOL_PARAM.addBorder(0,-31,InfDX-10,386,88+DL/*+CURSG->NSections?24:0*/,3);
	TOOL_PARAM.addTextButton(NULL,10,HDR_FDY,GetTextByID("ED_GSPRITE"),WinTool,WinTool,WinTool,0);
	CreateFlags(10,10);

	int Y0=36;
	TOOL_PARAM.addGP_Button(NULL,10,Y0-4,MapGP+1,0,0);
	TOOL_PARAM.addGP_Button(NULL,10+34,Y0,MapGP+1,3,2);
	TP_BRS=TOOL_PARAM.addGP_ScrollBarL(NULL,10+35,Y0+10,110,ReliefBrush,MapGP+1,4,192,12,0,0);
	
	TP_CSECT=TOOL_PARAM.addGP_ComboBoxDLX(NULL,10,48+1+DL,230,CBB_GPFILE,0,9,0,&YellowFont,&WhiteFont,NULL);
	TP_CSECT->Center-=5;
	void EnumGS_Types(char* dest);
	char gstemp[1024];
	EnumGS_Types(gstemp);
	TP_CSECT->AddComplexLine(gstemp);
	TP_CSECT->CurLine=CUR_GS_TYPE;
	DL+=22;
};
void ProcessTOOL_PARAM_GSPRITES(){

	if(ReliefBrush!=PrevRelBr){
		PrevRelBr=ReliefBrush;
		TP_BRS->SPos=ReliefBrush;
	};
	ReliefBrush=TP_BRS->SPos;
	StdWheel();
	CUR_GS_TYPE=TP_CSECT->CurLine;
	HeightEditMode=128+CUR_GS_TYPE;
};
#endif //_USE3D
char HEIGHTSTR[12]="";

ComboBox* ED_CB0;
ComboBox* ED_CB1;
ComboBox* ED_CB2;
ComboBox* ED_CB3;

BPXView* ED_BV=NULL;
byte BT_COLOR=0x4E;
int CurrTexSet=0;

//Textures dialog
//void CreatePlus(DialogsSystem* DSS,int x,int y,VCall* UPR){
//	DSS->Add
//};

bool MINHI_CLICK(SimpleDialog* SD){
	TM_Height0+=SD->UserParam;
	return true;
};
bool MAXHI_CLICK(SimpleDialog* SD){
	TM_Height1+=SD->UserParam;
	return true;
};
bool MINANG_CLICK(SimpleDialog* SD){
	TM_Angle0+=SD->UserParam;
	return true;
};
bool MAXANG_CLICK(SimpleDialog* SD){
	TM_Angle1+=SD->UserParam;
	return true;
};
extern int  ReliefBrush;
int TX_SCROLL=0;
bool TexOnClick(SimpleDialog* SD){
	bool ctrl=GetKeyState(VK_CONTROL)&0x8000;
	if(!ctrl)NTextures=0;
	int tidx=SD->ID-1024;
	if(tidx<64&&tidx>=0){
		bool erase=0;
		//for(int i=0;i<NTextures;i++)if(TexList[i]==tidx){
		//	if(i<NTextures-1)memcpy(TexList+i,TexList+i+1,(NTextures-i-1)*2);
		//  i--;
		//	NTextures--;
		//	erase=1;
		//}
		if(NTextures<128&&!erase){
			TexList[NTextures]=tidx;
			NTextures++;
		}
	}
	return true;
}
void CreateTOOL_PARAM_TEXTURE(){
	char ccc[256];
	sprintf(ccc,"Data\\TEXSET%d.DAT",CurrTexSet);
	FILE* F=fopen(ccc,"r");
	if(F){
		int v;
		fscanf(F,"%d %d %d %d %d ",&TM_Height0,&TM_Height1,&TM_Angle0,&TM_Angle1,&v);
		NTextures=v;
		for(int i=0;i<NTextures;i++){
			fscanf(F,"%d",&v);
			TexList[i]=v;
		};
		fclose(F);
	};
	CUR_TOOL_MODE=3;
	ClearModes();
	HeightEditMode=false;
	BlobMode=-1;
	WaterEditMode=0;
	Creator=4096+255;
	int DY=16+UNI_LINEDLY1;
	int DDY=UNI_LINEDY1;
	
	TOOL_PARAM.CloseDialogs();
	TP_Scroll=NULL;
	int DD1=75;
	int D=18;
	int x0=RealLx-InfDX;
	int y0=InfDY;
	TOOL_PARAM.HintFont=&SmallWhiteFont;
	TOOL_PARAM.HintX=-1;
	TOOL_PARAM.HintY=450;
	TOOL_PARAM.BaseX=x0-1;
	TOOL_PARAM.BaseY=y0+31;
	int DLY=107+56;
	TOOL_PARAM.addBorder(0,-31,InfDX-10,386,48+DLY,3);
	TOOL_PARAM.addTextButton(NULL,10,HDR_FDY,GetTextByID("ED_HDR04"),WinTool,WinTool,WinTool,0);	
	//CreateFlags(10,10);

	int Y0=4;
	int X0=5;
	int ODY=26;
	int maxx=0;
	char ccx[256];
	for(int i=0;i<5;i++){
		sprintf(ccx,"EDTP_0%d",i);
		int LX=GetRLCStrWidth(GetTextByID(ccx),&YellowFont);
		if(LX>maxx)maxx=LX;
	};
	maxx+=16;

	VScrollBar* VS;
	int CMLX=InfDX-20-maxx;

	TOOL_PARAM.addTextButton(NULL,X0,Y0,GetTextByID("EDTP_01"),&YellowFont,&YellowFont,&YellowFont,0);
	ED_CB0=TOOL_PARAM.addGP_ComboBoxDLX(NULL,X0+maxx,Y0,CMLX,CBB_GPFILE,0,9,0,&YellowFont,&WhiteFont,NULL);
	ED_CB0->Center-=5;
	ED_CB0->FontDx+=5;
	ED_CB0->CreateRuler(-200,1200);
	ED_CB0->AssignScroll(&TOOL_PARAM,&VS,3,0,10);
	ED_CB0->CurLine=TM_Height0+200;

	Y0+=ODY;
	TOOL_PARAM.addTextButton(NULL,X0,Y0,GetTextByID("EDTP_02"),&YellowFont,&YellowFont,&YellowFont,0);
	ED_CB1=TOOL_PARAM.addGP_ComboBoxDLX(NULL,X0+maxx,Y0,CMLX,CBB_GPFILE,0,9,0,&YellowFont,&WhiteFont,NULL);
	ED_CB1->Center-=5;
	ED_CB1->CreateRuler(-200,1200);
	ED_CB1->AssignScroll(&TOOL_PARAM,&VS,3,0,10);
	ED_CB1->CurLine=TM_Height1+200;


	Y0+=ODY;
	TOOL_PARAM.addTextButton(NULL,X0,Y0,GetTextByID("EDTP_03"),&YellowFont,&YellowFont,&YellowFont,0);
	ED_CB2=TOOL_PARAM.addGP_ComboBoxDLX(NULL,X0+maxx,Y0,CMLX,CBB_GPFILE,0,9,0,&YellowFont,&WhiteFont,NULL);
	ED_CB2->Center-=5;
	ED_CB2->CreateRuler(0,90);
	ED_CB2->AssignScroll(&TOOL_PARAM,&VS,3,0,12);
	ED_CB2->CurLine=TM_Angle0;

	Y0+=ODY;
	TOOL_PARAM.addTextButton(NULL,X0,Y0,GetTextByID("EDTP_04"),&YellowFont,&YellowFont,&YellowFont,0);
	ED_CB3=TOOL_PARAM.addGP_ComboBoxDLX(NULL,X0+maxx,Y0,CMLX,CBB_GPFILE,0,9,0,&YellowFont,&WhiteFont,NULL);
	ED_CB3->Center-=5;
	ED_CB3->CreateRuler(0,90);
	ED_CB3->AssignScroll(&TOOL_PARAM,&VS,3,0,12);
	ED_CB3->CurLine=TM_Angle1;

	Y0+=ODY;
	TOOL_PARAM.addTextButton(NULL,X0,Y0,GetTextByID("EDTP_05"),&YellowFont,&YellowFont,&YellowFont,0);
	InputBox* IB=TOOL_PARAM.addInputBox(NULL,X0+maxx,Y0-5,HEIGHTSTR,20,150,20,&YellowFont,&YellowFont);
	IB->Enabled=0;
	Y0+=ODY;
	TP_GPB=TOOL_PARAM.addGP_ComboBoxDLX(NULL,0,Y0,InfDX-15,CBB_GPFILE,0,9,0,&WhiteFont,&YellowFont,NULL);
	TP_GPB->Center-=5;
	TP_GPB->CurLine=CurrTexSet;
	char* IDX=GetTextByID("EDTX_S0");
	
	for(i=0;i<10;i++){
		sprintf(ccx,IDX,i+1);
		TP_GPB->AddLine(ccx);
	};
	
	Y0+=ODY+10;
	TOOL_PARAM.addGP_Button(NULL,10,Y0-4,MapGP+1,0,0);
	TOOL_PARAM.addGP_Button(NULL,10+34,Y0,MapGP+1,3,2);
	TP_BRS=TOOL_PARAM.addGP_ScrollBarL(NULL,10+36,Y0+10,110,ReliefBrush,MapGP+1,4,193,12,0,0);
	
	//TP_Scroll=TOOL_PARAM.addNewGP_VScrollBar(NULL,InfDX-21-15,48+4+DLY,312+23-DLY,100,0,ScrollGP,0);
	//TP_Scroll->ScrDy=3;
	//TP_Scroll->SPos=TX_SCROLL;
	///ED_BV=TOOL_PARAM.addBPXView(NULL,0,48+DLY+4,56,56,4,3,MaxTex>>2,tex1,TP_Scroll);
	//ED_BV->DECLX=8;
	//int N=ED_BV->Nx*ED_BV->RealNy;
	//for(i=0;i<N;i++){
	//	ED_BV->Choosed[i]=0;
	//};
	//for(i=0;i<NTextures;i++){
	//	ED_BV->Choosed[TexList[i]]++;
	//};
    DialogsDesk* DD=TOOL_PARAM.AddDialogsDesk(0,48+DLY+4,56*4+26,56*3,"EmptyBorder");
	DD->ID=512;
	BaseMeshDialog* BM=DD->addBaseMeshDialog(0,0,56*4,56*16);
	BM->BM=new BaseMesh;
	BM->BM->create(4*64,6*64,vfVertexTnL);
	word* idx=BM->BM->getIndices();
	VertexTnL* VT=(VertexTnL*)BM->BM->getVertexData();
	BM->BM->setTexture(IRS->GetTextureID("Textures\\GroundTex.bmp"));
	BM->BM->setShader(IRS->GetShaderID("hud"));
	BM->BM->setNVert(4*64);
	BM->BM->setNInd(6*64);
	BM->BM->setNPri(2*64);
	DD->VScroller->SPos=TX_SCROLL;

	int sqLx=56;
	float tlx=float(sqLx)/512.0f;
	for(int i=0;i<64;i++){
		int sx=(i&3)*sqLx;
		int sy=(i/4)*sqLx;
        float tx=float(int(i%8))/8.0f;
		float ty=float(int(i/8))/8.0f;

		ColoredBar* CB=DD->addColoredBar(sx,sy,sqLx-1,sqLx-1,255);
		CB->OnUserClick=&TexOnClick;
		CB->ID=1024+i;
		CB->Style=1;
		CB->Diffuse=0;

		TextButton* TB=DD->addTextButton(NULL,sx+4,sy+4,"        ",&SmallWhiteFont,&SmallWhiteFont,&SmallWhiteFont,0);
		TB->ID=2048+i;

		char cc[256];
		sprintf(cc,"%d",i);

		TB=DD->addTextButton(NULL,sx+4,sy+56-16,cc,&SmallWhiteFont,&SmallWhiteFont,&SmallWhiteFont,0);
		TB->ID=4096+i;

		VT[0].u=tx;
		VT[0].v=ty;
		VT[0].x=sx;
		VT[0].y=sy;
		VT[0].w=1.0f;
		VT[0].diffuse=0xFFFFFFFF;

		VT[1].u=tx+tlx;
		VT[1].v=ty;
		VT[1].x=sx+sqLx;
		VT[1].y=sy;
		VT[1].w=1.0f;
		VT[1].diffuse=0xFFFFFFFF;

		VT[2].u=tx;
		VT[2].v=ty+tlx;
		VT[2].x=sx;
		VT[2].y=sy+sqLx;
		VT[2].w=1.0f;
		VT[2].diffuse=0xFFFFFFFF;

		VT[3].u=tx+tlx;
		VT[3].v=ty+tlx;
		VT[3].x=sx+sqLx;
		VT[3].y=sy+sqLx;
		VT[3].w=1.0f;
		VT[3].diffuse=0xFFFFFFFF;

		int i6=i*4;
        idx[0]=i6;idx[1]=i6+1;idx[2]=i6+2;
		idx[3]=i6+1;idx[4]=i6+3;idx[5]=i6+2;
		VT+=4;
		idx+=6;
	}
};
void RefreshED_BV(){
	/*int N=ED_BV->Nx*ED_BV->RealNy;
	for(int i=0;i<N;i++){
		ED_BV->Choosed[i]=0;
	};
	for(i=0;i<NTextures;i++){
		ED_BV->Choosed[TexList[i]]++;
	};*/
};
void ProcessTOOL_PARAM_TEXTURE(){
	if(ReliefBrush!=PrevRelBr){
		PrevRelBr=ReliefBrush;
		TP_BRS->SPos=ReliefBrush;
	};
	ReliefBrush=TP_BRS->SPos;
	if(TP_GPB->CurLine!=CurrTexSet){
		CurrTexSet=TP_GPB->CurLine;
		char ccc[256];
		sprintf(ccc,"Data\\TEXSET%d.DAT",CurrTexSet);
		FILE* F=fopen(ccc,"r");
		if(F){
			int v;
			fscanf(F,"%d %d %d %d %d ",&TM_Height0,&TM_Height1,&TM_Angle0,&TM_Angle1,&v);
			NTextures=v;
			for(int i=0;i<NTextures;i++){
				fscanf(F,"%d",&v);
				TexList[i]=v;
			};
			fclose(F);
			ED_CB0->CurLine=TM_Height0+200;
			ED_CB1->CurLine=TM_Height1+200;
			ED_CB2->CurLine=TM_Angle0;
			ED_CB3->CurLine=TM_Angle1;
			//int N=ED_BV->Nx*ED_BV->RealNy;
			/*for(i=0;i<N;i++){
				ED_BV->Choosed[i]=0;
			};
			for(i=0;i<NTextures;i++){
				ED_BV->Choosed[TexList[i]]++;
			};*/
		};
	};
	bool Change=0;
	if(TM_Height0!=ED_CB0->CurLine+ED_CB0->MinDeal){
		TM_Height0=ED_CB0->CurLine+ED_CB0->MinDeal;
		Change=1;
	};
	if(TM_Height1!=ED_CB1->CurLine+ED_CB1->MinDeal){
		TM_Height1=ED_CB1->CurLine+ED_CB1->MinDeal;
		Change=1;
	};
	if(TM_Angle0!=ED_CB2->CurLine+ED_CB2->MinDeal){
		TM_Angle0=ED_CB2->CurLine+ED_CB2->MinDeal;
		Change=1;
	};
	if(TM_Angle1!=ED_CB3->CurLine+ED_CB3->MinDeal){
		TM_Angle1=ED_CB3->CurLine+ED_CB3->MinDeal;
		Change=1;
	};

	if(TM_Angle0!=90)TM_Tan0=int(tan(double(TM_Angle0)*3.1415/180)*256);else TM_Tan0=100000;
	if(TM_Angle1!=90)TM_Tan1=int(tan(double(TM_Angle1)*3.1415/180)*256);else TM_Tan1=100000;
	int ntx=0;
	/*int nn=ED_BV->Nx*ED_BV->RealNy;
	for(int i=0;i<nn;i++){
		if(ED_BV->Choosed[i]){
			for(int j=0;j<ED_BV->Choosed[i];j++){
			if(ntx<256){
					TexList[ntx]=i;
					ntx++;
				};
			};
		};
	};*/
	for(int i=0;i<64;i++){
		SimpleDialog* SD=TOOL_PARAM.Find(1024+i);
		if(SD){
			SD->Diffuse=0;
		}
		TextButton* TB=dynamic_cast<TextButton*>(TOOL_PARAM.Find(2048+i));
		if(TB)TB->Message[0]=0;
		TB=dynamic_cast<TextButton*>(TOOL_PARAM.Find(4096+i));
		if(TB){
			TB->Visible=(GetKeyState(VK_SHIFT)&0x8000)!=0;
		}
	}
	for(int i=0;i<NTextures;i++){
		SimpleDialog* SD=TOOL_PARAM.Find(1024+TexList[i]);
		if(SD){
			SD->Diffuse=0xFFFFFFFF;
			TextButton* TB=dynamic_cast<TextButton*>(TOOL_PARAM.Find(2048+TexList[i]));
			if(TB){
				int idx=0;
				idx=atoi(TB->Message);
				idx++;
				sprintf(TB->Message,"%d",idx);
			}
		}
	}
	//if(ntx!=NTextures)Change=1;
	if(NTextures){
		//NTextures=ntx;
		HeightEditMode=false;
		MEditMode=true;
		TexMapMod=true;
	}else{
		HeightEditMode=false;
		MEditMode=false;
		TexMapMod=false;
	};
	if(Change){
		char ccc[256];
		sprintf(ccc,"Data\\TEXSET%d.DAT",CurrTexSet);
		FILE* F=fopen(ccc,"w");
		if(F){
			fprintf(F,"%d %d %d %d %d ",TM_Height0,TM_Height1,TM_Angle0,TM_Angle1,NTextures);
			for(int i=0;i<NTextures;i++){
				fprintf(F,"%d ",TexList[i]);
			};
			fclose(F);
		};
	};
	StdWheel();
	DialogsDesk* DD=dynamic_cast<DialogsDesk*>(TOOL_PARAM.Find(512));
	if(DD){
		TX_SCROLL=DD->VScroller?DD->VScroller->SPos:0;
	}
};
byte NeedToPopUp=0;
//------------------COPY PIECES------------------//
char** PIECES_NAMES=NULL;
int NPIECES=0;
int MAX_PIECES=0;
void AddPiece(char* str){
	if(NPIECES>=MAX_PIECES){
		MAX_PIECES+=256;
		PIECES_NAMES=(char**)realloc(PIECES_NAMES,MAX_PIECES<<2);
	};
	for(int i=0;i<NPIECES;i++){
		if(_stricmp(str,PIECES_NAMES[i])<0){
			if(i<NPIECES){
				for(int j=NPIECES-1;j>=i;j--)PIECES_NAMES[j+1]=PIECES_NAMES[j];
				PIECES_NAMES[i]=znew(char,strlen(str)+1);
				strcpy(PIECES_NAMES[i],str);
				NPIECES++;
				return;
			};
		};
	};
	PIECES_NAMES[NPIECES]=znew(char,strlen(str)+1);
	strcpy(PIECES_NAMES[NPIECES],str);
	NPIECES++;
};
void ClearPieces(){
	for(int i=0;i<NPIECES;i++)free(PIECES_NAMES[i]);
	if(PIECES_NAMES)free(PIECES_NAMES);
	PIECES_NAMES=NULL;
	NPIECES=0;
	MAX_PIECES=0;
};
void ReadAllPieces(){
	ClearPieces();
	WIN32_FIND_DATA FD;
	HANDLE H=FindFirstFile("UserPieces\\*.smp",&FD);
	if(H!=INVALID_HANDLE_VALUE){
		do{
			AddPiece(FD.cFileName);
		}while(FindNextFile(H,&FD));
		FindClose(H);
	};
};
ListBox* ED_PLIST;
extern int CurrPiece=-1;
void CreateTOOL_PARAM_PIECES(){
	CurrPiece=-1;
	CUR_TOOL_MODE=4;
	int DY=16+UNI_LINEDLY1;
	int DDY=UNI_LINEDY1;
	
	TOOL_PARAM.CloseDialogs();
	TP_Scroll=NULL;
	int DD1=75;
	int D=18;
	int x0=RealLx-InfDX;
	int y0=InfDY;
	TOOL_PARAM.HintFont=&SmallWhiteFont;
	TOOL_PARAM.HintX=-1;
	TOOL_PARAM.HintY=450;
	TOOL_PARAM.BaseX=x0-1;
	TOOL_PARAM.BaseY=y0+31;
	TOOL_PARAM.addBorder(0,-31,InfDX-10,386,48+26,3);
	TOOL_PARAM.addTextButton(NULL,10,HDR_FDY,GetTextByID("ED_HDR05"),WinTool,WinTool,WinTool,0);
	CreateFlags(10,10);
	TP_Scroll=TOOL_PARAM.addNewGP_VScrollBar(NULL,InfDX-21-16,48+26+1+7,312-9,100,0,ScrollGP,0);
	TP_Scroll->ScrDy=20;
	ED_PLIST=TOOL_PARAM.addListBox(NULL,5,48+26+1,16,200,20,&WhiteFont,&YellowFont,TP_Scroll);
	ReadAllPieces();
	for(int i=0;i<NPIECES;i++){
		ED_PLIST->AddItem(PIECES_NAMES[i],i);
	};
	CurrPiece=-1;
	ED_PLIST->CurItem=-1;
};
void ProcessRM_LoadEx(int x,int y,char* Name);
void ProcessTOOL_PARAM_PIECES(){
	if(ED_PLIST->NItems){
		if(WheelDelta<0){
			ED_PLIST->SetCurrentItem(ED_PLIST->CurItem+1);
			//if(ED_PLIST->CurItem>=ED_PLIST->NItems)ED_PLIST->CurItem=ED_PLIST->NItems-1;
			if(ED_PLIST->CurItem>=ED_PLIST->FLItem+ED_PLIST->ny)ED_PLIST->FLItem=ED_PLIST->CurItem-ED_PLIST->ny+1;
			if(ED_PLIST->FLItem<0)ED_PLIST->FLItem=0;
			TP_Scroll->SPos=ED_PLIST->FLItem*32;
		};
		if(WheelDelta>0){
			ED_PLIST->SetCurrentItem(ED_PLIST->CurItem-1);
			//if(ED_PLIST->CurItem<0)ED_PLIST->CurItem=0;
			if(ED_PLIST->CurItem<=ED_PLIST->FLItem)ED_PLIST->FLItem=ED_PLIST->CurItem;
			TP_Scroll->SPos=ED_PLIST->FLItem*32;
		};
		WheelDelta=0;
	};
	if(CurrPiece!=ED_PLIST->CurItem){
		CurrPiece=ED_PLIST->CurItem;
		ProcessRM_LoadEx((mapx<<5)+(RealLx>>1)-100,(mapy<<5)+(RealLy),(ED_PLIST->GetItem(CurrPiece))->Message);
	};
};
//-----------------------WATER EDITOR---------------------//
VScrollBar* TP_BRS1;
VScrollBar* TP_BRS2;
extern int CostThickness;
void CreateTOOL_PARAM_WATER(){
	
	CUR_TOOL_MODE=5;
	ClearModes();
	BlobMode=-1;
	WaterEditMode=0;
	Creator=4096+255;
	int DY=16+UNI_LINEDLY1;
	int DDY=UNI_LINEDY1;
	
	TOOL_PARAM.CloseDialogs();
	TP_Scroll=NULL;
	int DD1=75;
	int D=18;
	int x0=RealLx-InfDX;
	int y0=InfDY;
	TOOL_PARAM.HintFont=&SmallWhiteFont;
	TOOL_PARAM.HintX=-1;
	TOOL_PARAM.HintY=450;
	TOOL_PARAM.BaseX=x0-1;
	TOOL_PARAM.BaseY=y0+31;
	int DL=24;
	TOOL_PARAM.addBorder(0,-31,InfDX-10,150,0,1);
	TOOL_PARAM.addTextButton(NULL,10,HDR_FDY,GetTextByID("ED_HDR06"),WinTool,WinTool,WinTool,0);
	CreateFlags(10,10);

	int Y0=36;
	TOOL_PARAM.addTextButton(NULL,5,Y0,GetTextByID("ED_PARM01"),&YellowFont,&YellowFont,&YellowFont,0);
	Y0+=24;
	TOOL_PARAM.addGP_Button(NULL,10,Y0-4,MapGP+1,0,0);
	TOOL_PARAM.addGP_Button(NULL,10+34,Y0,MapGP+1,3,2);
	TP_BRS=TOOL_PARAM.addGP_ScrollBarL(NULL,10+34,Y0+10,110,ReliefBrush,MapGP+1,4,195,12,0,0);
	Y0+=26;
	TOOL_PARAM.addTextButton(NULL,5,Y0,GetTextByID("ED_PARM02"),&YellowFont,&YellowFont,&YellowFont,0);
	Y0+=24;
	TOOL_PARAM.addGP_Button(NULL,10,Y0-4,MapGP+1,0,0);
	TOOL_PARAM.addGP_Button(NULL,10+34,Y0,MapGP+1,3,2);
	TP_BRS2=TOOL_PARAM.addGP_ScrollBarL(NULL,10+34,Y0+10,25,CostThickness,MapGP+1,4,195,12,0,0);
	Y0+=26;
};
int PrevCostW=-1;
void ProcessTOOL_PARAM_WATER(){
	StdWheel();
	if(ReliefBrush!=PrevRelBr){
		PrevRelBr=ReliefBrush;
		TP_BRS->SPos=ReliefBrush;
	};
	ReliefBrush=TP_BRS->SPos;
	if(CostThickness!=PrevCostW){
		PrevCostW=CostThickness;
		TP_BRS2->SPos=CostThickness;
	};
	CostThickness=TP_BRS2->SPos;
};
void CreateTOOL_PARAM_BRIGHTNESS(byte type){
	
	CUR_TOOL_MODE=6;
	ClearModes();
	BlobMode=-1;
	WaterEditMode=0;
	Creator=4096+255;
	int DY=16+UNI_LINEDLY1;
	int DDY=UNI_LINEDY1;
	
	TOOL_PARAM.CloseDialogs();
	TP_Scroll=NULL;
	int DD1=75;
	int D=18;
	int x0=RealLx-InfDX;
	int y0=InfDY;
	TOOL_PARAM.HintFont=&SmallWhiteFont;
	TOOL_PARAM.HintX=-1;
	TOOL_PARAM.HintY=450;
	TOOL_PARAM.BaseX=x0-1;
	TOOL_PARAM.BaseY=y0+31;
	int DL=24;
	TOOL_PARAM.addBorder(0,-31,InfDX-10,95,0,1);
	int Y0=36;
	if(type){
		TOOL_PARAM.addTextButton(NULL,10,HDR_FDY,GetTextByID("ED_HDR08"),WinTool,WinTool,WinTool,0);
		TOOL_PARAM.addTextButton(NULL,5,Y0,GetTextByID("ED_PARM04"),&YellowFont,&YellowFont,&YellowFont,0);
	}else{
		TOOL_PARAM.addTextButton(NULL,10,HDR_FDY,GetTextByID("ED_HDR07"),WinTool,WinTool,WinTool,0);
		TOOL_PARAM.addTextButton(NULL,5,Y0,GetTextByID("ED_PARM03"),&YellowFont,&YellowFont,&YellowFont,0);
	};
	CreateFlags(10,10);
	Y0+=24;
	if(ReliefBrush>20)ReliefBrush=20;
	TOOL_PARAM.addGP_Button(NULL,10,Y0-4,MapGP+1,0,0);
	TOOL_PARAM.addGP_Button(NULL,10+34,Y0,MapGP+1,3,2);
	TP_BRS=TOOL_PARAM.addGP_ScrollBarL(NULL,10+34,Y0+10,20,ReliefBrush,MapGP+1,4,195,12,0,0);
	Y0+=26;
};
void ProcessTOOL_PARAM_BRIGHTNESS(){
	StdWheel();
	if(ReliefBrush>20)ReliefBrush=20;
	if(ReliefBrush!=PrevRelBr){
		PrevRelBr=ReliefBrush;
		TP_BRS->SPos=ReliefBrush;
	};
	ReliefBrush=TP_BRS->SPos;
};
void CreateTOOL_PARAM_RELIEF(byte type){
	
	CUR_TOOL_MODE=7;
	if(!type){
		ClearModes();
		BlobMode=-1;
		WaterEditMode=0;
		Creator=4096+255;
	};
	int DY=16+UNI_LINEDLY1;
	int DDY=UNI_LINEDY1;
	
	TOOL_PARAM.CloseDialogs();
	TP_Scroll=NULL;
	int DD1=75;
	int D=18;
	int x0=RealLx-InfDX;
	int y0=InfDY;
	TOOL_PARAM.HintFont=&SmallWhiteFont;
	TOOL_PARAM.HintX=-1;
	TOOL_PARAM.HintY=450;
	TOOL_PARAM.BaseX=x0-1;
	TOOL_PARAM.BaseY=y0+31;
	int DL=24;
	TOOL_PARAM.addBorder(0,-31,InfDX-10,95,0,1);
	int Y0=36;
	if(type){
		TOOL_PARAM.addTextButton(NULL,10,HDR_FDY,GetTextByID("ED_HDR09"),WinTool,WinTool,WinTool,0);
		TOOL_PARAM.addTextButton(NULL,5,Y0,GetTextByID("ED_PARM01"),&YellowFont,&YellowFont,&YellowFont,0);
	}else{
		TOOL_PARAM.addTextButton(NULL,10,HDR_FDY,GetTextByID("ED_HDR03"),WinTool,WinTool,WinTool,0);
		TOOL_PARAM.addTextButton(NULL,5,Y0,GetTextByID("ED_PARM01"),&YellowFont,&YellowFont,&YellowFont,0);
	};
	CreateFlags(10,10);
	Y0+=24;
	TOOL_PARAM.addGP_Button(NULL,10,Y0-4,MapGP+1,0,0);
	TOOL_PARAM.addGP_Button(NULL,10+34,Y0,MapGP+1,3,2);
	TP_BRS=TOOL_PARAM.addGP_ScrollBarL(NULL,10+34,Y0+10,110,ReliefBrush,MapGP+1,4,195,12,0,0);
	Y0+=26;
};

void ProcessTOOL_PARAM_RELIEF(){
	StdWheel();
	if(ReliefBrush!=PrevRelBr){
		PrevRelBr=ReliefBrush;
		TP_BRS->SPos=ReliefBrush;
	};
	ReliefBrush=TP_BRS->SPos;
};
extern int DrawPixMode;
int PEN_RADIUS=8;
int PEN_BRIGHTNESS=255;
int PrevPRAD=8;
int PrevPBRIGHT=255;
int CurDrMod=3;
ComboBox* DRMOD=NULL;
void CreateTOOL_PARAM_PAINT(){
	
	CUR_TOOL_MODE=21;
	ClearModes();
	BlobMode=-1;
	WaterEditMode=0;
	Creator=4096+255;
	DrawPixMode=1;
	int DY=16+UNI_LINEDLY1;
	int DDY=UNI_LINEDY1;
	
	TOOL_PARAM.CloseDialogs();
	TP_Scroll=NULL;
	int DD1=75;
	int D=18;
	int x0=RealLx-InfDX;
	int y0=InfDY;
	TOOL_PARAM.HintFont=&SmallWhiteFont;
	TOOL_PARAM.HintX=-1;
	TOOL_PARAM.HintY=450;
	TOOL_PARAM.BaseX=x0-1;
	TOOL_PARAM.BaseY=y0+31;
	int DL=24;
	TOOL_PARAM.addBorder(0,-31,InfDX-10,180,0,1);
	TOOL_PARAM.addTextButton(NULL,10,HDR_FDY,GetTextByID("ED_PAINT"),WinTool,WinTool,WinTool,0);
	CreateFlags(10,10);

	int Y0=36;
	TOOL_PARAM.addTextButton(NULL,5,Y0,GetTextByID("ED_PENRADIUS"),&YellowFont,&YellowFont,&YellowFont,0);
	Y0+=22;
	TOOL_PARAM.addGP_Button(NULL,10,Y0-4,MapGP+1,0,0);
	TOOL_PARAM.addGP_Button(NULL,10+34,Y0,MapGP+1,3,2);
	TP_BRS=TOOL_PARAM.addGP_ScrollBarL(NULL,10+34,Y0+10,64,PEN_RADIUS,MapGP+1,4,195,12,0,0);
	Y0+=22;
	TOOL_PARAM.addTextButton(NULL,5,Y0,GetTextByID("ED_PENBRIGHTNESS"),&YellowFont,&YellowFont,&YellowFont,0);
	Y0+=22;
	TOOL_PARAM.addGP_Button(NULL,10,Y0-4,MapGP+1,0,0);
	TOOL_PARAM.addGP_Button(NULL,10+34,Y0,MapGP+1,3,2);
	TP_BRS2=TOOL_PARAM.addGP_ScrollBarL(NULL,10+34,Y0+10,255,PEN_BRIGHTNESS,MapGP+1,4,195,12,0,0);
	Y0+=24;
	DRMOD=TOOL_PARAM.addGP_ComboBoxDLX(NULL,10+6,Y0+15,224,CBB_GPFILE,0,9,0,&WhiteFont,&YellowFont,NULL);
	DRMOD->AddLine("None");
	DRMOD->AddLine("Brightness");
	DRMOD->AddLine("Radius");
	DRMOD->AddLine("Brightness&Radius");
	DRMOD->CurLine=CurDrMod;
};
void ProcessTOOL_PARAM_PAINT(){
	StdWheel();
	if(PEN_RADIUS!=PrevPRAD){
		PrevPRAD=PEN_RADIUS;
		TP_BRS->SPos=PEN_RADIUS;
	};
	PEN_RADIUS=TP_BRS->SPos;
	PrevPRAD=PEN_RADIUS;
	if(PEN_BRIGHTNESS!=PrevPBRIGHT){
		PrevPBRIGHT=PEN_BRIGHTNESS;
		TP_BRS2->SPos=PEN_BRIGHTNESS;
	};
	PEN_BRIGHTNESS=TP_BRS2->SPos;
	PrevPBRIGHT=PEN_BRIGHTNESS;
	CurDrMod=DRMOD->CurLine;
};

extern int CostThickness;
extern int DrawGroundMode;
VScrollBar* GR_BRIG=NULL;
int GroundBrightness=100;
byte* GRPTR=NULL;
extern int CurGroundTexture;
BaseMeshDialog* GroBV=NULL;
extern DIALOGS_API IRenderSystem*	IRS;
void CreateTOOL_PARAM_GROUND(){
	
	CUR_TOOL_MODE=22;
	ClearModes();
	BlobMode=-1;
	WaterEditMode=0;
	Creator=4096+255;
	int DY=16+UNI_LINEDLY1;
	int DDY=UNI_LINEDY1;
	
	TOOL_PARAM.CloseDialogs();
	TP_Scroll=NULL;
	int DD1=75;
	int D=18;
	int x0=RealLx-InfDX;
	int y0=InfDY;
	TOOL_PARAM.HintFont=&SmallWhiteFont;
	TOOL_PARAM.HintX=-1;
	TOOL_PARAM.HintY=450;
	TOOL_PARAM.BaseX=x0-1;
	TOOL_PARAM.BaseY=y0+31;
	int DL=24;
	TOOL_PARAM.addBorder(0,-31,InfDX-10,390,180,3);
	TOOL_PARAM.addTextButton(NULL,10,HDR_FDY,GetTextByID("ED_EDITGROUND"),WinTool,WinTool,WinTool,0);
	CreateFlags(10,10);

	int Y0=30;
	TOOL_PARAM.addTextButton(NULL,5,Y0+2,GetTextByID("ED_GR_RADIUS"),&YellowFont,&YellowFont,&YellowFont,0);
	Y0+=22;
	TOOL_PARAM.addGP_Button(NULL,10,Y0-4,MapGP+1,0,0);
	TOOL_PARAM.addGP_Button(NULL,10+34,Y0,MapGP+1,3,2);
	TP_BRS=TOOL_PARAM.addGP_ScrollBarL(NULL,10+34,Y0+10,110,ReliefBrush,MapGP+1,4,195,12,0,0);
	Y0+=22;
	TOOL_PARAM.addTextButton(NULL,5,Y0+2,GetTextByID("ED_GR_WIDTH"),&YellowFont,&YellowFont,&YellowFont,0);
	Y0+=22;
	TOOL_PARAM.addGP_Button(NULL,10,Y0-4,MapGP+1,0,0);
	TOOL_PARAM.addGP_Button(NULL,10+34,Y0,MapGP+1,3,2);
	TP_BRS2=TOOL_PARAM.addGP_ScrollBarL(NULL,10+34,Y0+10,25,CostThickness,MapGP+1,4,195,12,0,0);
	Y0+=22;
	TOOL_PARAM.addTextButton(NULL,5,Y0+2,GetTextByID("ED_GR_BRIGHT"),&YellowFont,&YellowFont,&YellowFont,0);
	Y0+=22;
	TOOL_PARAM.addGP_Button(NULL,10,Y0-4,MapGP+1,0,0);
	TOOL_PARAM.addGP_Button(NULL,10+34,Y0,MapGP+1,3,2);
	GR_BRIG=TOOL_PARAM.addGP_ScrollBarL(NULL,10+34,Y0+10,100,GroundBrightness,MapGP+1,4,195,12,0,0);
	Y0+=22;
	BMPformat BM;
	char cc[256];
	const char* GetFactureName(int id);
	strcpy(cc,GetFactureName(CurGroundTexture));
	sprintf(cc,"Ground\\tex%d.bmp",CurGroundTexture);
	if(GRPTR)free(GRPTR);
	GRPTR=NULL;
	int GetNFactures();
	TP_Scroll=TOOL_PARAM.addNewGP_VScrollBar(NULL,InfDX-21-16,Y0+22,207,GetNFactures()-1,CurGroundTexture,ScrollGP,0);
	TP_Scroll->ScrDy=10;
	TP_Scroll->OnesDy=1;
	GroBV=TOOL_PARAM.addBaseMeshDialog(0,Y0+22,225,207);
	GroBV->CreateSquareTnL(0,0,1.0,1.0,"hud",0xFFFFFFFF);
	char TN[256];
	//sprintf(TN,"Ground\\tex%d.bmp",TP_Scroll->SPos+1);
	const char* GetFactureName(int id);
	strcpy(TN,GetFactureName(TP_Scroll->SPos));

	GroBV->BM->setTexture(IRS->GetTextureID(TN));	
	TextButton* TB1=TOOL_PARAM.addTextButton(NULL,10,Y0+32,"          ",&SmallWhiteFont,&SmallWhiteFont,&SmallWhiteFont,0);
	TB1->AssignID(1277);
	DrawGroundMode=1;
};
extern bool ManualTex;
void ProcessTOOL_PARAM_GROUND(){
	TextButton* TB1=(TextButton*)TOOL_PARAM.Find(1277);
	if(TB1){
		sprintf(TB1->Message,"%d",CurGroundTexture);
	}
	StdWheel();
	if(ReliefBrush!=PrevRelBr){
		PrevRelBr=ReliefBrush;
		TP_BRS->SPos=ReliefBrush;
	};
	ReliefBrush=TP_BRS->SPos;
	if(CostThickness!=PrevCostW){
		PrevCostW=CostThickness;
		TP_BRS2->SPos=CostThickness;
	};
	GroundBrightness=GR_BRIG->SPos;
	CostThickness=TP_BRS2->SPos;
	if(ManualTex&&TP_Scroll->SPos!=CurGroundTexture){
		TP_Scroll->SPos=CurGroundTexture;
		CurGroundTexture=-1;
	};
	ManualTex=0;
	if(CurGroundTexture!=TP_Scroll->SPos){
		char TN[256];
		//sprintf(TN,"Ground\\tex%d.bmp",TP_Scroll->SPos+1);
		const char* GetFactureName(int id);
		strcpy(TN,GetFactureName(TP_Scroll->SPos));
		GroBV->BM->setTexture(IRS->GetTextureID(TN));	
		CurGroundTexture=TP_Scroll->SPos;
	};
};
int PrevScrLx=1024;
void ProcessTOOL_PARAM(){
	if(NoPress&&mouseX>RealLx-InfDX-32)return;
	if(TP_Made&&PrevScrLx!=RealLx){
		TP_Made=0;
	};
	PrevScrLx=RealLx;
	if(NeedToPopUp){
		Inform=0;
		InfDX=InfDX0;
		OptHidden=0;
		ClearModes();
		HeightEditMode=false;
		BlobMode=-1;
		WaterEditMode=0;
		Creator=4096+255;
		switch(NeedToPopUp){
		case 1://default
			CreateTOOL_PARAM_DEFAULT();
			PressCurrentEdModeButton(0);
			break;
        case 2://units
			CreateTOOL_PARAM_UNIT();
			PressCurrentEdModeButton(19);
			break;
		case 3://trees
			CreateTOOL_PARAM_SPRITE(0);
			PressCurrentEdModeButton(2);
			break;
		case 4://stones
			CreateTOOL_PARAM_SPRITE(1);
			PressCurrentEdModeButton(3);
			break;
		case 5://holes???
			CreateTOOL_PARAM_SPRITE(2);
			PressCurrentEdModeButton(3);
			break;
		case 6://texture
			CreateTOOL_PARAM_TEXTURE();
			PressCurrentEdModeButton(1);
			break;
		case 7://erase sprites
			CreateTOOL_PARAM_RELIEF(1);
			PressCurrentEdModeButton(4);
			HeightEditMode=3;
			MEditMode=false;
			EditMedia=5;
			break;
		case 8://Set water
			CreateTOOL_PARAM_WATER();
			PressCurrentEdModeButton(6);
			WaterEditMode=1;
			break;
		case 9://clear water
			CreateTOOL_PARAM_WATER();
			PressCurrentEdModeButton(7);
			WaterEditMode=2;
			break;
		case 10://Set brightness
			CreateTOOL_PARAM_BRIGHTNESS(0);
			PressCurrentEdModeButton(8);
			WaterEditMode=3;
			break;
		case 11://clear brightness
			CreateTOOL_PARAM_BRIGHTNESS(1);
			PressCurrentEdModeButton(9);
			WaterEditMode=4;
			break;
		case 12://unlock mode
			CreateTOOL_PARAM_DEFAULT();
			PressCurrentEdModeButton(16);
			LockMode=1;
			break;
		case 13://lock mode
			CreateTOOL_PARAM_DEFAULT();
			PressCurrentEdModeButton(17);
			LockMode=2;
			break;
		case 14://default lock mode
			CreateTOOL_PARAM_DEFAULT();
			PressCurrentEdModeButton(18);
			LockMode=3;
			break;
		case 15://edit height (1) hill
			CreateTOOL_PARAM_RELIEF(0);
			HeightEditMode=1;
			HiStyle=0;
			break;
		case 16://edit height (2) plane
			CreateTOOL_PARAM_RELIEF(0);
			HeightEditMode=10;
			HiStyle=0;
			break;
		case 17://edit height (3) equ
			CreateTOOL_PARAM_RELIEF(0);
			HeightEditMode=11;
			HiStyle=0;
			break;
		case 18://edit height (4) soft
			CreateTOOL_PARAM_RELIEF(0);
			HeightEditMode=12;
			HiStyle=0;
			break;
		case 30://group
			CreateTOOL_PARAM_DEFAULT();
			ZoneCmd=2;
			break;
		case 31://zone
			CreateTOOL_PARAM_DEFAULT();
			ZoneCmd=1;
			break;
		case 32://streams
			RiverEditMode=1;
			break;
		case 19://edit height (5) line
#ifdef _USE3D
			CreateTOOL_PARAM_EDITROAD();
			void SetRoadsEditMode(bool Rmode);
			SetRoadsEditMode(1);
#else
			CreateTOOL_PARAM_DEFAULT();
			HeightEditMode=13;
			HiStyle=0;
#endif
			break;
		case 20:
			CreateTOOL_PARAM_PIECES();
			PressCurrentEdModeButton(15);
			break;
		case 21:
			CreateTOOL_PARAM_PAINT();
			PressCurrentEdModeButton(0);
			break;
		case 22:
			CreateTOOL_PARAM_GROUND();
			PressCurrentEdModeButton(0);
			break;
#ifdef _USE3D
		case 23:
			CreateTOOL_PARAM_GSPRITE();
			PressCurrentEdModeButton(0);
			break;
#endif
		case 24:
			CreateTOOL_PARAM_SPRITE(3);
			PressCurrentEdModeButton(0);
			break;

		};
		NeedToPopUp=0;
	};
	if(!TP_Made){
		TP_GPB=NULL;
		switch(CUR_TOOL_MODE){
		case 0:
			CreateTOOL_PARAM_DEFAULT();
			break;
        case 1:
			CreateTOOL_PARAM_UNIT();
			break;
		case 2:
			CreateTOOL_PARAM_SPRITE(0);
			break;
		case 3:
			CreateTOOL_PARAM_TEXTURE();
			break;
		case 4:
			CreateTOOL_PARAM_PIECES();
			break;
		case 5:
			CreateTOOL_PARAM_WATER();
			break;
		case 6:
			CreateTOOL_PARAM_BRIGHTNESS(0);
			break;
		case 7:
			CreateTOOL_PARAM_RELIEF(0);
			break;
		};
		TP_Made=1;
	};
	if(PlayGameMode==2||!OptHidden)TOOL_PARAM.ProcessDialogs();
	//ProcessTOOL_PARAM_UNIT();
	switch(CUR_TOOL_MODE){
	case 1:
		ProcessTOOL_PARAM_UNIT();
		break;
	case 2:
		ProcessTOOL_PARAM_SPRITES();
		break;
	case 3:
		ProcessTOOL_PARAM_TEXTURE();
		break;
	case 4:
		ProcessTOOL_PARAM_PIECES();
		break;
	case 5:
		ProcessTOOL_PARAM_WATER();
		break;
	case 6:
		ProcessTOOL_PARAM_BRIGHTNESS();
		break;
	case 7:
		ProcessTOOL_PARAM_RELIEF();
		break;
	case 21:
		ProcessTOOL_PARAM_PAINT();
		break;
	case 22:
		ProcessTOOL_PARAM_GROUND();
		break;
#ifdef _USE3D
	case 23:
		ProcessTOOL_PARAM_GSPRITES();
		break;
	case 14:
		ProcessTOOL_PARAM_EDITROAD();
		break;
#endif
	};
	if(!OptHidden){
		if(mouseY<TOOL_PARAM.BaseY+394&&mouseX>RealLx-InfDX-7)ClearUnderDialog(&TOOL_PARAM);
	};
	if(LockMode){
		StdWheel();
	}
};
#define PenLX 60
#define Pen2 30
#define PenLY 60
int ET_TREES_COLOR=0;
int ET_SOFT_COLOR=0;
int ET_MOUNT_COLOR[16];
void ClearPen(byte* BUF){
	memset(BUF,0,PenLX*PenLY);
};
void DrawTreePen(byte* BUF,int v1,int v2){
	ClearPen(BUF);
	int r=(Pen2*v1)>>10;
	int N=v2;
	BUF[30+30*PenLX]=ET_TREES_COLOR;
	for(int i=0;i<N;i++){
		int x=(int(randoma[i+i])%PenLX)-Pen2;
		int y=int(randoma[i+i+1])%PenLY-Pen2;
		int rr=int(sqrt(x*x+y*y));
		if(rr<r)BUF[x+30+(y+30)*PenLX]=ET_TREES_COLOR;
	};
};
void DrawPenCircle(byte* BUF,int r,byte c){
	ClearPen(BUF);
	int r2=r*r+r;
	for(int i=0;i<=r;i++){
		int v=int(sqrt(r2-i*i));
		memset(BUF+(Pen2-i)*PenLX+Pen2-v,c,v+v);
		memset(BUF+(Pen2+i)*PenLX+Pen2-v,c,v+v);
	};
};
void DrawWaterPen(byte* BUF,int v1){
	int r=(Pen2*v1)>>10;
	DrawPenCircle(BUF,r,0xB4);
};
void DrawHillPen(byte* BUF,int v1){
	int r=(Pen2*v1)>>10;
	DrawPenCircle(BUF,r,ET_MOUNT_COLOR[15]);
};
void DrawSoftPen(byte* BUF,int v1){
	int r=(Pen2*v1)>>10;
	DrawPenCircle(BUF,r,ET_SOFT_COLOR);
};
double GetPenH(int x,int y,int r,int v);
void DrawHeightPen(byte* BUF,int v1,int v2){
	ClearPen(BUF);
	int r0=(Pen2*v1)>>10;
	if(r0<1)r0=1;
	int pos=0;
	for(int ix=0;ix<PenLX;ix++){
		for(int iy=0;iy<PenLY;iy++){
			BUF[pos]=ET_MOUNT_COLOR[byte(GetPenH(ix-Pen2,iy-Pen2,r0,v2)*15)];
			pos++;
		};
	};
};
#define TERRSH 8
byte* TerrHI;
byte* SoftArea;
byte* TerrMap;
byte* ResultMap;


void RenderToResult(int x0,int y0,int x1,int y1){
	
};
struct UndoChunk{
	byte* UndoData;
	int Size;
	int MaxSize;
};
class PixUndo{
public:
	UndoChunk* Chunks;
	int NChunks;
	PixUndo();
	~PixUndo();
	void AddUndoData(byte Type,byte x,byte y,byte color);
	void OpenNewChunk();
	void PerformUndo();
	void Clear();
};
void PixUndo::Clear(){
	for(int i=0;i<NChunks;i++){
		if(Chunks[i].UndoData)free(Chunks[i].UndoData);
	};
	if(NChunks)free(Chunks);
	Chunks=NULL;
	NChunks=0;
};

PixUndo::PixUndo(){
	NChunks=0;
	Chunks=NULL;
};
PixUndo::~PixUndo(){
	Clear();
};
void PixUndo::OpenNewChunk(){
	if((!NChunks)||Chunks[NChunks-1].Size){
		Chunks=(UndoChunk*)realloc(Chunks,(NChunks+1)*sizeof UndoChunk);
		Chunks[NChunks].Size=0;
		Chunks[NChunks].MaxSize=0;
		Chunks[NChunks].UndoData=NULL;
		NChunks++;
	};
};
void PixUndo::AddUndoData(byte Type,byte x,byte y,byte color){
	if(NChunks){
		if(Chunks[NChunks-1].Size+5>Chunks[NChunks-1].MaxSize){
			Chunks[NChunks-1].MaxSize+=512;
			Chunks[NChunks-1].UndoData=(byte*)realloc(Chunks[NChunks-1].UndoData,Chunks[NChunks-1].MaxSize);
		};
		byte* data=Chunks[NChunks-1].UndoData+Chunks[NChunks-1].Size;
		Chunks[NChunks-1].Size+=4;
		data[3]=Type;
		data[0]=x;
		data[1]=y;
		data[2]=color;
	};
};
void PixUndo::PerformUndo(){
	if(NChunks){
		byte* data=Chunks[NChunks-1].UndoData;
		int sz=Chunks[NChunks-1].Size;
		int sz0=sz;
		while(sz>0){
			switch(data[sz-1]){
			case 0:
				TerrHI[(int(data[sz-3])<<8)+data[sz-4]]=data[sz-2];
				sz-=4;
				break;
			case 1:
				SoftArea[(int(data[sz-3])<<8)+data[sz-4]]=data[sz-2];
				sz-=4;
				break;
			case 2:
				TerrMap[(int(data[sz-3])<<8)+data[sz-4]]=data[sz-2];
				sz-=4;
				break;
			};
		};
		Chunks[NChunks-1].Size=0;
		Chunks[NChunks-1].MaxSize=0;
		free(Chunks[NChunks-1].UndoData);
		Chunks[NChunks-1].UndoData=NULL;
		NChunks--;
		if(!sz0)PerformUndo();
	};
};
PixUndo* CURRUNDO;
void PutTerrPixel(byte Map,int x,int y,byte c){
	if(x<0||x>255||y<0||y>255)return;
	int ofs=x+(y<<TERRSH);
	switch(Map){
	case 0:
		if(TerrHI[ofs]!=c){
			CURRUNDO->AddUndoData(0,x,y,TerrHI[ofs]);
			TerrHI[ofs]=c;
		};
		break;
	case 1:
		if(SoftArea[ofs]!=c){
			CURRUNDO->AddUndoData(1,x,y,SoftArea[ofs]);
			SoftArea[ofs]=c;
		};
		break;
	case 2:
		if(TerrMap[ofs]!=c){
			CURRUNDO->AddUndoData(2,x,y,TerrMap[ofs]);
			TerrMap[ofs]=c;
		};
		break;
	};
};
extern bool realRpressed;
void DrawTerrCircle(byte Type,int x,int y,int r,byte c);
void TerrDrawTrees(int x,int y,int v1,int v2){
	if(realRpressed){
		DrawTerrCircle(2,x,y,(Pen2*v1)>>10,0);
		return;
	};
	int r=(Pen2*v1)>>10;
	int N=v2;
	PutTerrPixel(2,x,y,ET_TREES_COLOR);
	for(int i=0;i<N;i++){
		int xx=(int(randoma[i+i])%PenLX)-Pen2;
		int yy=int(randoma[i+i+1])%PenLY-Pen2;
		int rr=int(sqrt(xx*xx+yy*yy));
		if(rr<r)PutTerrPixel(2,x+xx,y+yy,ET_TREES_COLOR);
	};
};
void DrawTerrCircle(byte Type,int x,int y,int r,byte c){
	int r2=r*r+r;
	for(int i=0;i<=r;i++){
		int v=int(sqrt(r2-i*i));
		for(int j=-v;j<=v;j++){
			PutTerrPixel(Type,x+j,y-i,c);
			PutTerrPixel(Type,x+j,y+i,c);
		};
	};
};
void TerrDrawHills(int x,int y,int v1){
	if(realRpressed)DrawTerrCircle(2,x,y,(Pen2*v1)>>10,0);
	else DrawTerrCircle(2,x,y,(Pen2*v1)>>10,0xFF);
};
void TerrDrawWater(int x,int y,int v1){
	if(realRpressed)DrawTerrCircle(2,x,y,(Pen2*v1)>>10,0);
	else DrawTerrCircle(2,x,y,(Pen2*v1)>>10,0xB4);
};
void TerrDrawSoft(int x,int y,int v1){
	if(realRpressed)DrawTerrCircle(1,x,y,(Pen2*v1)>>10,0);
	else DrawTerrCircle(1,x,y,(Pen2*v1)>>10,0xFF);
};
double GetPenH(int x,int y,int r,int v){
	int rr=int(sqrt(x*x+y*y));
	if(rr>r)return 0;
	if(!r)return 255;
	double D=double(rr)/double(r);
	return 1.0-exp((double(v)/128.0+1)*log(D));
};
void TerrDrawHi(int x,int y,int v1,int v2){
	int r=(Pen2*v1)>>10;
	double DD=double(GetTickCount()%10)/10.0;
	for(int ix=-r;ix<r;ix++){
		for(int iy=-r;iy<r;iy++){
			int xx=x+ix;
			int yy=y+iy;
			if(xx>=0&&yy>=0&&xx<256&&yy<256){
				double DH=GetPenH(ix,iy,r,v2);
				int DY=int(DH*5.0+DD);
				int ofs=xx+(yy<<TERRSH);
				int v=TerrHI[ofs];
				if(realRpressed)v-=DY;
				else v+=DY;
				if(v<0)v=0;
				if(v>255)v=255;
				PutTerrPixel(0,xx,yy,v);
			};
		};
	};
};
void SaveHData(char* Name,byte* data){
	ResFile F=RRewrite(Name);
	if(F!=INVALID_HANDLE_VALUE){
		RBlockWrite(F,data,65535);
		RClose(F);
	};
};
void LoadHData(char* Name,byte* data){
	ResFile F=RReset(Name);
	if(F!=INVALID_HANDLE_VALUE){
		RBlockRead(F,data,65535);
		RClose(F);
	};
};
void SetupTerr(){
	TerrHI=znew(byte,65536);
	SoftArea=znew(byte,65536);
	TerrMap=znew(byte,65536);
	ResultMap=znew(byte,65536);
	memset(TerrHI,0xFF,65536);
	memset(SoftArea,0,65536);
	memset(TerrMap,0,65536);
	memset(ResultMap,0,65536);
	LoadHData("Data\\TerrHI.dat",TerrHI);
	LoadHData("Data\\SoftArea.dat",SoftArea);
	LoadHData("Data\\TerrMap.dat",TerrMap);
	RenderToResult(0,0,255,255);
};
void FreeTerr(){
	SaveHData("Data\\TerrHI.dat",TerrHI);
	SaveHData("Data\\SoftArea.dat",SoftArea);
	SaveHData("Data\\TerrMap.dat",TerrMap);
	free(TerrHI);
	free(SoftArea);
	free(TerrMap);
	free(ResultMap);
};
DIALOGS_API void DrawCircle(int x,int y,int r,byte c){
	char* xi=Rarr[r].xi;
	char* yi=Rarr[r].yi;
	int N=Rarr[r].N;
	for(int i=0;i<N;i++)Hline(x+xi[i],y+yi[i],x+xi[i],c);
};
//-------------------TERRAIN EDITOR----------------//
void PushScreen(byte** Ptr){
	*Ptr=znew(byte,ScrWidth*RealLy);
	memcpy(*Ptr,ScreenPtr,ScrWidth*RealLy);
};
void PopScreen(byte* Ptr){
	memcpy(ScreenPtr,Ptr,ScrWidth*RealLy);
	free(Ptr);
};

void SaveToBMP24(char* Name,int Lx,int Ly,byte* data);
void SaveTerrainBitmap(char* Name){
	byte* TMP24=znew(byte,256*(256+1)*3*3);
	byte* DATA24=TMP24;
	int ofs3=0;
	for(int i=0;i<65536;i++){
		byte v=TerrMap[i];
		if(v==ET_TREES_COLOR){//tree
			DATA24[ofs3]=0;
			DATA24[ofs3+1]=255;
			DATA24[ofs3+2]=0;
		}else if(v==0xFF){//hills
			DATA24[ofs3]=0xFF;
			DATA24[ofs3+1]=0xFF;
			DATA24[ofs3+2]=0xFF;
		}else if(v==0xB4){//water
			DATA24[ofs3]=0xFF;
			DATA24[ofs3+1]=0;
			DATA24[ofs3+2]=0;
		}else{
			DATA24[ofs3]=0;
			DATA24[ofs3+1]=0;
			DATA24[ofs3+2]=0;
		};
		ofs3+=3;
	};
	DATA24+=65536*3;
	for(i=0;i<768;i++){
		if((i&3)==1)DATA24[0]=0xFF;
		else DATA24[0]=0;
		DATA24++;
	};
	ofs3=0;
	for(i=0;i<65536;i++){
		byte v=TerrHI[i];
		DATA24[ofs3]=v;
		DATA24[ofs3+1]=v;
		DATA24[ofs3+2]=v;
		ofs3+=3;
	};
	DATA24+=65536*3;
	for(i=0;i<768;i++){
		if((i&3)==1)DATA24[0]=0xFF;
		else DATA24[0]=0;
		DATA24++;
	};
	ofs3=0;
	for(i=0;i<65536;i++){
		byte v=SoftArea[i];
		if(v){
			DATA24[ofs3]=0;
			DATA24[ofs3+1]=0;
			DATA24[ofs3+2]=0xFF;
		}else{
			DATA24[ofs3]=0;
			DATA24[ofs3+1]=0;
			DATA24[ofs3+2]=0;
		};
		ofs3+=3;
	};
	DATA24+=65536*3;
	for(i=0;i<768;i++){
		if((i&3)==1)DATA24[0]=0xFF;
		else DATA24[0]=0;
		DATA24++;
	};
	SaveToBMP24(Name,256,256*3+3,TMP24);
	free(TMP24);
};
bool LoadTerrainBitmap(char* Name){
	byte* TMP24;
	BMPformat BM;
	if(ReadBMP24(Name,&BM,&TMP24)){
		if(BM.biWidth!=256||BM.biHeight!=256*3+3){
			free(TMP24);
			return false;
		};
		byte* DATA24=TMP24;
		int ofs3=0;
		for(int i=0;i<65536;i++){
			int B=DATA24[ofs3  ];
			int G=DATA24[ofs3+1];
			int R=DATA24[ofs3+2];
			byte v=0;
			if(G>(B+R)*2)v=ET_TREES_COLOR;
			else if(B>200&&G>200&&R>200)v=0xFF;
			else if(B>(G+R)*2)v=0xB4;
			TerrMap[i]=v;
			ofs3+=3;
		};
		DATA24+=65536*3+256*3;
		ofs3=0;
		for(i=0;i<65536;i++){
			TerrHI[i]=DATA24[ofs3+1];
			ofs3+=3;
		};
		DATA24+=65536*3+256*3;
		ofs3=0;
		for(i=0;i<65536;i++){
			if(DATA24[ofs3+2]>=128)SoftArea[i]=0xFF;
			else SoftArea[i]=0;
			ofs3+=3;
		};
		free(TMP24);
		return true;
	}else return false;
};
void WaitWithError(char* ID,int GPID);
void ProcessLoadBitmap(){
	byte* p;
	PushScreen(&p);
	LocalGP BOR2("Interface\\Bor2");
	char BMPS[256];
	if(ProcessLoadingFile("UserTerrain\\*.bmp",BMPS,3)){
		char ccc[256];
		sprintf(ccc,"UserTerrain\\%s",BMPS);
		if(!LoadTerrainBitmap(ccc)){
			WaitWithError("INCRBMFORM",BOR2.GPID);
		};
	};
	RenderToResult(0,0,255,255);
	PopScreen(p);
	CURRUNDO->Clear();
	ItemChoose=-1;
};
void ProcessSaveBitmap(){
	byte* p;
	PushScreen(&p);
	char BMPS[256];
	if(ProcessSavingFile("UserTerrain\\*.bmp",BMPS,6,1)){
		char ccc[256];
		_strupr(BMPS);
		if(!strstr(BMPS,".BMP"))strcat(BMPS,".BMP");
		sprintf(ccc,"UserTerrain\\%s",BMPS);
		SaveTerrainBitmap(ccc);
	};
	RenderToResult(0,0,255,255);
	PopScreen(p);
	ItemChoose=-1;
};
void ProcessTerrainEditor(){
	//void PaintMapWith();
	//PaintMapWith();
	void UserFriendlyMapGenerator();
	UserFriendlyMapGenerator();
	return;
	// Fonts
	RLCFont* FWin = &fon16y5;
	RLCFont* FAPad = &fon18w;
	RLCFont* FPPad = &fon18y3;
	RLCFont* FABut = &fon18y5;
	RLCFont* FPBut = &fon18y5;
	RLCFont* FText = &fon18y3;
	RLCFont* FWhite = &fon18w;
	RLCFont* FYellow = &fon18y3;
	RLCFont* FASmCmb = &fon16y1;
	RLCFont* FPSmCmb = &fon16y3;

	// Graphics
	LocalGP BTNS("Interf2\\EditTerra");
	LocalGP CMB("Interf3\\elements\\Combo");

	PixUndo PUNDO;
	CURRUNDO=&PUNDO;
	SetupTerr();
	byte PenBuf[PenLX*PenLY];	

	int wLx=GPS.GetGPWidth(BTNS.GPID,0);
	int wLy=GPS.GetGPHeight(BTNS.GPID,0);

	int x0=(RealLx-wLx)>>1;
	int y0=(RealLy-wLy)>>1;

	DialogsSystem DSS(x0,y0);
	byte* BACK=znew(byte,RealLx*RealLy);
	MemScreenPart(0,0,RealLx,RealLy,BACK);
	//GPS.ShowGP((RealLx-524)>>1,(RealLy-400)>>1,BTNS.GPID,1,0);
	DSS.addBPXView(NULL,-x0,-y0,RealLx,RealLy,1,1,1,BACK,NULL);
	
	DSS.HintX=-1;
	DSS.HintY=180;
	DSS.HintFont=&SmallBlackFont;

	DSS.addGPPicture(NULL,0,0,BTNS.GPID,0);
	DSS.addGPPicture(NULL,0,0,BTNS.GPID,1);

	GP_TextButton* OkBtn=DSS.addGP_TextButton(NULL,84,364,GetTextByID("INTF_GENERATE"),BTNS.GPID,3,FABut,FPBut);
	OkBtn->UserParam=mcmOk;
	OkBtn->OnUserClick=&MMItemChoose;
	GP_TextButton* CancelBtn=DSS.addGP_TextButton(NULL,314,364,GetTextByID("INTF_CANCEL"),BTNS.GPID,5,FABut,FPBut);
	CancelBtn->UserParam=mcmCancel;
	CancelBtn->OnUserClick=&MMItemChoose;
	GP_TextButton* Load=DSS.addGP_TextButton(NULL,347,280,GetTextByID("INTF_BITM_LOAD"),BTNS.GPID,7,FPSmCmb,FPSmCmb); //162
	Load->UserParam=1;
	Load->OnUserClick=&MMItemChoose;
	Load->FontDy-=1;
	GP_TextButton* Save=DSS.addGP_TextButton(NULL,347,310,GetTextByID("INTF_BITM_SAVE"),BTNS.GPID,9,FPSmCmb,FPSmCmb);
	Save->UserParam=2;
	Save->OnUserClick=&MMItemChoose;
	Save->FontDy-=1;
	CheckBox* TREES =DSS.addGP_CheckBox(NULL,335,80,"",&YellowFont,&YellowFont,1,0,BTNS.GPID,10,11,12);
	TREES->Hint=GetTextByID("$TE_HN1");
	CheckBox* WATER =DSS.addGP_CheckBox(NULL,366,80,"",&YellowFont,&YellowFont,1,0,BTNS.GPID,13,14,15);
	WATER->Hint=GetTextByID("$TE_HN2");
	CheckBox* HILLS =DSS.addGP_CheckBox(NULL,397,80,"",&YellowFont,&YellowFont,1,0,BTNS.GPID,16,17,18);
	HILLS->Hint=GetTextByID("$TE_HN3");
	CheckBox* SOFT  =DSS.addGP_CheckBox(NULL,335,111,"",&YellowFont,&YellowFont,1,0,BTNS.GPID,19,20,21);
	SOFT->Hint=GetTextByID("$TE_HN4");
	CheckBox* HEIGHT=DSS.addGP_CheckBox(NULL,366,111,"",&YellowFont,&YellowFont,1,0,BTNS.GPID,22,23,24);
	HEIGHT->Hint=GetTextByID("$TE_HN5");
	GP_Button* UNDO  =DSS.addGP_Button(NULL,397,111,BTNS.GPID,27,26);
	UNDO->Hint=GetTextByID("$TE_HN6");
	UNDO->UserParam=3;
	UNDO->OnUserClick=&MMItemChoose;
	VScrollBar* HB1=DSS.addGP_ScrollBarL(NULL,336,206+10,1024,64,MapGP+1,4,186,12,0,0);
	VScrollBar* HB2=DSS.addGP_ScrollBarL(NULL,336,248+10,1024,64,MapGP+1,4,186,12,0,0);
	DSS.addBPXView(NULL,462,81,60,60,1,1,1,PenBuf,NULL);
	
	TextButton* TxtClimZon = DSS.addTextButton(NULL,336,146,GetTextByID("INTF_CROOM_O5"),FPSmCmb,FPSmCmb,FPSmCmb,0);
	ComboBox* CmbClimZon = DSS.addGP_ComboBoxDLX(NULL,334,164,190,CMB.GPID,0,9,0,FASmCmb,FPSmCmb,NULL);
	CmbClimZon->Center-=5;
	CmbClimZon->FontDy-=2;
	CmbClimZon->OneDy-=1;
	
	int cz=0;
	char* cztxt;
	do{
		char clzon[256];
		sprintf(clzon,"#CL_ZONE%d",cz);
		cztxt=GetTextByID(clzon);
#ifdef LIMITOPTIONS
		if(*cztxt!='#'&&CmbClimZon->NLines<1){
			CmbClimZon->AddLine(cztxt);
			cz++;
		}else break;
#else
		if(*cztxt!='#'){
			CmbClimZon->AddLine(cztxt);
			cz++;
		}else break;
#endif
	}while(true);	

	// Text
	DSS.addTextButton(NULL,295,35,GetTextByID("INTF_EDIT_TERRAIN"),FWin,FWin,FWin,1);
	DSS.addTextButton(NULL,338,188,GetTextByID("INTF_BRUSH_SZ"),FPSmCmb,FPSmCmb,FPSmCmb,0);
	DSS.addTextButton(NULL,338,230,GetTextByID("INTF_BRUSH_BR"),FPSmCmb,FPSmCmb,FPSmCmb,0);	

	ItemChoose=-1;
	int CurPen=-1;
	ClearPen(PenBuf);
	BPXView* VIEW=DSS.addBPXView(NULL,56,79,256,256,1,1,1,ResultMap,NULL);
	int spp1=-1;
	int spp2=-1;
	int pcp=-1;
	do{
		if(!(realLpressed||realRpressed))PUNDO.OpenNewChunk();
		ProcessMessages();
		if(KeyPressed&&LastKey=='Z'){
			PUNDO.PerformUndo();
			RenderToResult(0,0,255,255);
			LastKey=0;
			KeyPressed=0;
		};
		DSS.ProcessDialogs();
		if(VIEW->MouseOver){
			TempWindow TW;
			PushWindow(&TW);
			IntersectWindows(VIEW->x,VIEW->y,VIEW->x1,VIEW->y1);
			DrawCircle(mouseX,mouseY,(HB1->SPos*30)>>10,0xFF);
			PopWindow(&TW);
		};
		DSS.RefreshView();
		if(TREES->State)CurPen=0;
		if(WATER->State)CurPen=1;
		if(HILLS->State)CurPen=2;
		if(SOFT->State)CurPen=3;
		if(HEIGHT->State)CurPen=4;
		//DrawTreePen(PenBuf,HB1->SPos,HB2->SPos);
		//DrawWaterPen(PenBuf,HB1->SPos);
		if(spp1!=HB1->SPos||spp2!=HB2->SPos||pcp!=CurPen){
			spp1=HB1->SPos;
			spp2=HB2->SPos;
			pcp=CurPen;
			switch(CurPen){
			case 0:
				DrawTreePen(PenBuf,HB1->SPos,HB2->SPos);
				break;
			case 1:
				DrawWaterPen(PenBuf,HB1->SPos);
				break;
			case 2:
				DrawHillPen(PenBuf,HB1->SPos);
				break;	
			case 3:
				DrawSoftPen(PenBuf,HB1->SPos);
				break;
			case 4:
				DrawHeightPen(PenBuf,HB1->SPos,HB2->SPos);
				break;
			};
		};
		if((realLpressed||realRpressed)&&VIEW->MouseOver){
			int x=mouseX-VIEW->x;
			int y=mouseY-VIEW->y;
			switch(CurPen){
			case 0:
				TerrDrawTrees(x,y,HB1->SPos,HB2->SPos);
				break;
			case 1:
				TerrDrawWater(x,y,HB1->SPos);
				break;
			case 2:
				TerrDrawHills(x,y,HB1->SPos);
				break;
			case 3:
				TerrDrawSoft(x,y,HB1->SPos);
				break;
			case 4:
				TerrDrawHi(x,y,HB1->SPos,HB2->SPos);
				//DrawHeightPen(PenBuf,HB1->SPos,HB2->SPos);
				break;
			};
			RenderToResult(0,0,255,255);
		};
		HB1->SPos+=WheelDelta/4;
		if(HB1->SPos<0)HB1->SPos=0;
		if(HB1->SPos>HB1->SMaxPos)HB1->SPos=HB1->SMaxPos;
		WheelDelta=0;
		if(ItemChoose==1){
			ProcessLoadBitmap();
			ItemChoose=-1;
		};
		if(ItemChoose==2){
			ProcessSaveBitmap();
			ItemChoose=-1;
		};
		if(ItemChoose==3){
			PUNDO.PerformUndo();
			RenderToResult(0,0,255,255);
			LastKey=0;
			KeyPressed=0;
			ItemChoose=-1;
		};

	}while(ItemChoose==-1);
	if(ItemChoose==mcmOk){
		//generating
		byte* DATA24=znew(byte,65536*3);
		int ofs3=0;
		for(int i=0;i<65536;i++){
			byte v=TerrMap[i];
			if(v==ET_TREES_COLOR){//tree
				DATA24[ofs3]=0;
				DATA24[ofs3+1]=255;
				DATA24[ofs3+2]=0;
			}else if(v==0xFF){//hills
				DATA24[ofs3]=0xFF;
				DATA24[ofs3+1]=0xFF;
				DATA24[ofs3+2]=0xFF;
			}else if(v==0xB4){//water
				DATA24[ofs3]=0xFF;
				DATA24[ofs3+1]=0;
				DATA24[ofs3+2]=0;
			}else{
				DATA24[ofs3]=0;
				DATA24[ofs3+1]=0;
				DATA24[ofs3+2]=0;
			};
			ofs3+=3;
		};
		SaveToBMP24("Data\\01.bmp",256,256,DATA24);
		ofs3=0;
		for(i=0;i<65536;i++){
			byte v=TerrHI[i];
			DATA24[ofs3]=v;
			DATA24[ofs3+1]=v;
			DATA24[ofs3+2]=v;
			ofs3+=3;
		};
		SaveToBMP24("Data\\02.bmp",256,256,DATA24);
		ofs3=0;
		for(i=0;i<65536;i++){
			byte v=SoftArea[i];
			if(v){
				DATA24[ofs3]=0;
				DATA24[ofs3+1]=0;
				DATA24[ofs3+2]=0xFF;
			}else{
				DATA24[ofs3]=0;
				DATA24[ofs3+1]=0;
				DATA24[ofs3+2]=0;
			};
			ofs3+=3;
		};
		SaveToBMP24("Data\\03.bmp",256,256,DATA24);
		free(DATA24);
		///GenerateMapForMission("Data\\01.bmp","hehe","Data\\02.bmp","Data\\03.bmp",CmbClimZon->CurLine);
	};
	free(BACK);
	FreeTerr();
};
#define CM_WLx 380
#define CM_WLy 130
void DrawHdrTable(int x0,int y0,int x1,int y1);
void DeleteAllSprites();
void DeleteAllUnits();
void ClearAllZones();
void CreateNEWMAP(){
	int X0=(RealLx-CM_WLx)>>1;
	int Y0=(RealLy-CM_WLy)>>1;
	
	byte* p=znew(byte,(CM_WLx+64)*(CM_WLy+140));
	MemScreenPart(X0-32,Y0-32,(CM_WLx+64),(CM_WLy+140),p);
	DialogsSystem DSS(X0,Y0);
	DSS.addBPXView(NULL,-32,-32,(CM_WLx+64),(CM_WLy+140),1,1,1,p,NULL);
	DSS.addBorder(0,0,CM_WLx,CM_WLy,0,1);
	int FDY=40;
	DSS.addTextButton(NULL,16,0    ,GetTextByID("NEWMPHDR"),&SpecialWhiteFont,&SpecialWhiteFont,&SpecialWhiteFont,0);
	DSS.addTextButton(NULL,16,FDY   ,GetTextByID("SELMSZ"),&YellowFont,&YellowFont,&YellowFont,0);
	ComboBox* CB=DSS.addGP_ComboBoxDLX(NULL,150,FDY   ,180,CBB_GPFILE,0,9,0,&WhiteFont,&YellowFont,NULL);
	CB->AddLine(GetTextByID("SEL_MS0"));
	CB->AddLine(GetTextByID("SEL_MS1"));
	CB->AddLine(GetTextByID("SEL_MS2"));
	
	LocalGP But("Interf2\\Button2");

	int XB=10;
	int YB=FDY+26;
	
	DSS.addGPPicture(NULL,XB,YB,But.GPID,0);
	GP_TextButton* OKBTN=DSS.addGP_TextButton(NULL,XB+23,YB+20,GetTextByID("@OK"),But.GPID,1,&SpecialBlackFont,&SpecialBlackFont);
	OKBTN->OnUserClick=&MMItemChoose;
	OKBTN->UserParam=mcmOk;
	XB=180;
	//YB=CM_WLy+7;
	DSS.addGPPicture(NULL,XB,YB,But.GPID,0);	
	GP_TextButton* CANCELBTN=DSS.addGP_TextButton(NULL,XB+23,YB+20,GetTextByID("@CANCEL"),But.GPID,1,&SpecialBlackFont,&SpecialBlackFont);
	CANCELBTN->OnUserClick=&MMItemChoose;
	CANCELBTN->UserParam=0;
	ItemChoose=-1;
	do{
		ProcessMessages();
		DSS.ProcessDialogs();
		DSS.RefreshView();
	}while(ItemChoose==-1);
	if(ItemChoose==mcmOk){
		DeleteAllSprites();
		DeleteAllUnits();
		ClearAllZones();
		void ClearRoads();
		ClearRoads();
		ADDSH=1+CB->CurLine;
		msx=240<<ADDSH;
		msy=240<<ADDSH;
		void SetInitialScrollLimitations();
		SetInitialScrollLimitations();
		void PrepareEditMedia(byte,byte);
		PrepareEditMedia(MyNation,ADDSH);
		//PrepareEditMedia();
		//SetupArrays();
		//EraseAreas();
		rando();//!!
		CreateTotalLocking();
		ResearchIslands();
		ACTION_NewMapInEditor();
	};
	free(p);
};