#include "stdheader.h"
#include "unitability.h"
#include "MassivePlayer.h"

#include "CurrentMapOptions.h"
void Rept (LPSTR sz,...);
extern DWORD RealTime;
//#define SCMD int STARTOFFS=EBPos;
//#define ECMD Rept("RT=%6d P=%5d L=%6d D=%2x %2x %2x %2x\n",RealTime,STARTOFFS,EBPos-STARTOFFS,ExBuf[STARTOFFS],ExBuf[STARTOFFS+1],ExBuf[STARTOFFS+2],ExBuf[STARTOFFS+3]);
#define SCMD if(EBPos>1200)return;
#define ECMD
bool AddUnitsToCash(byte NI,word ID);
void ClearUniCash();
void GetCorrectMoney(byte NI,int* MONEY);
//extern int RESRC[8][8];
extern int tmtmt;
extern word rpos;
void LoadSaveFile();
bool CreateGates(OneObject* OB);
void CorrectBrigadesSelection(byte NT);
void ImCorrectBrigadesSelection(byte NT);
bool CheckIfPossibleToBreakOrder(OneObject* OB);
byte* NPresence;
extern Nation NATIONS[8];
byte MYNATION;
word* Selm[8];
word* SerN[8];
word* ImSelm[8];
word* ImSerN[8];
word NSL[8];
word ImNSL[8];
word SelCenter[8];
bool CmdDone[ULIMIT];
typedef  bool CHOBJ(OneObject* OB,int N);
void CreateUnit(Nation* NT,byte x,byte y,word ID);
//Execute buffer 
DWORD Signatur[2049];
byte ExBuf[8192];
int EBPos;

//
void InitEBuf(){
	EBPos=0;
};
extern bool NOPAUSE;
//[1][ni][x][y][x1][y1]
void CmdCreateSelection(byte NI,byte x,byte y,byte x1,byte y1){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=1;
	ExBuf[EBPos+1]=NI;
	ExBuf[EBPos+2]=x;
	ExBuf[EBPos+3]=y;
	ExBuf[EBPos+4]=x1;
	ExBuf[EBPos+5]=y1;
	EBPos+=6;
	ECMD;
};
extern bool GoAndAttackMode;
CEXPORT void AddXYPulse(int x,int y);
extern int curptr;
//[2][ni][x][y]
void CmdSendToXY(byte NI,int x,int y,short Dir){
	//if(!NOPAUSE)return;//!!!
	//to avoid fastclick:
	/*
	static int PrevClickTime=0;
	static int PrevNI=0;
	static int PrevX=0;
	static int PrevY=0;
	static int PrevNSL=0;
	if(ImNSL[NI]==PrevNSL && PrevNI==NI && Norma(x-PrevX,y-PrevY)<32 && GetTickCount()-PrevClickTime<1000)return;
	PrevClickTime=GetTickCount();
	PrevX=x;
	PrevY=y;
	PrevNI=NI;
	PrevNSL=ImNSL[NI];
	*/

	SCMD;
	byte Type=0;
	if(GetKeyState(VK_SHIFT)&0x8000)Type=2;
	if( GoAndAttackMode || (GetKeyState(VK_CONTROL)&0x8000) ) Type|=8;
	if(GetKeyState(VK_CONTROL)&0x8000)Type|=16;
	GoAndAttackMode=0;
	ExBuf[EBPos]=2;
	ExBuf[EBPos+1]=NI;
	*(int*)(&ExBuf[EBPos+2])=x;
	*(int*)(&ExBuf[EBPos+6])=y;
	*(short*)(&ExBuf[EBPos+10])=Dir;
	ExBuf[EBPos+12]=Type;
	EBPos+=13;
	if(!curptr)AddXYPulse(x>>4,y>>4);
	ECMD;
};
//[3][ni][w:ObjID]
void CmdAttackObj(byte NI,word ObjID,short DIR){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	byte Type=0;
	if(GetKeyState(VK_SHIFT)&0x8000)Type=2;
	ExBuf[EBPos]=3;
	ExBuf[EBPos+1]=NI;
	*(word*)(&ExBuf[EBPos+2])=ObjID;
	ExBuf[EBPos+4]=Type;
	*(short*)(&ExBuf[EBPos+5])=DIR;
	EBPos+=7;
	ECMD;
};	
//[4][ni][x][y][w:Type]
void CmdCreateTerrain(byte NI,byte x,byte y,word Type){
	SCMD;
	ExBuf[EBPos]=4;
	ExBuf[EBPos+1]=GSets.CGame.cgi_NatRefTBL[NI];
	ExBuf[EBPos+2]=x;
	ExBuf[EBPos+3]=y;
	*(word*)(&ExBuf[EBPos+4])=Type;
	EBPos+=6;
	ECMD;
};
//[5][ni][x:32][y:32][Type:16][OrderType]
void CmdCreateBuilding(byte NI,int x,int y,word Type){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=5;
	ExBuf[EBPos+1]=NI;
	*(int*)(&ExBuf[EBPos+2])=x;
    *(int*)(&ExBuf[EBPos+6])=y;
	*(word*)(&ExBuf[EBPos+10])=Type;
    if(GetKeyState(VK_SHIFT)&0x8000)ExBuf[EBPos+12]=2;
    else ExBuf[EBPos+12]=0;
	EBPos+=13;
	ECMD;
};
//[6][ni][w:ObjID]
int PrevProdPos=-1;
int PrevProdUnit=0;
void CmdProduceObj(byte NI,word Type){
	//if(!NOPAUSE)return;//!!!
	//bool Officer=false;
	//GeneralObject* GO=NATIONS->Mon[Type];
	//if(GO&&(GO->newMons->Usage==CostlyID||GO->newMons->NoInfinity)) Officer=true;

	//bool Ctrl=GetKeyState(VK_SHIFT)&0x8000;
	//if(!(Ctrl^Officer))Type|=8192;

	bool Infinity=false;
	int NProd=1;

	if( GetKeyState(VK_CONTROL)&0x8000 ){
		Infinity=true;
	}else
	if( GetKeyState(VK_SHIFT)&0x8000 ){
		NProd=10;
	}
	
	if(Infinity){
		Type|=8192;
	}
	if(!AddUnitsToCash(NI,Type))return;

	if(PrevProdPos!=-1&&PrevProdUnit==Type){
		int N=ExBuf[PrevProdPos];
		if(N<255)N++;
		ExBuf[PrevProdPos]++;
		return;
	};
	SCMD;
	ExBuf[EBPos]=78;
	ExBuf[EBPos+1]=NI;
	ExBuf[EBPos+2]=NProd;

	//if(Officer){
	//	ExBuf[EBPos+2]=1;
	//}else{
	//	ExBuf[EBPos+2]=10;
	//}

	PrevProdPos=EBPos+2;
	PrevProdUnit=Type;
	*(word*)(&ExBuf[EBPos+3])=Type;
	EBPos+=5;
	ECMD;
};
//[7][ni][index]
void CmdMemSelection(byte NI,byte Index){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=7;
	ExBuf[EBPos+1]=NI;
	ExBuf[EBPos+2]=Index;
	EBPos+=3;
	ECMD;
};
//[8][ni][Index]
void CmdRememSelection(byte NI,byte Index){
	static int LastComTime=GetTickCount()-10000;
	static int LastNI=0;
	if(GetTickCount()-LastComTime<800){        
		if(LastNI==NI+Index*10){
            void GoToCurSelPosition();
			GoToCurSelPosition();
		}		
	}
	LastNI=NI+Index*10;
	LastComTime=GetTickCount();
	//if(!NOPAUSE)return;//!!!
	SCMD;
	byte SHIFT=(GetKeyState(VK_SHIFT)&0x8000)!=0;
	SelSet[NI*10+(Index&127)].ImSelectMembers(NI,SHIFT);	
	ImCorrectBrigadesSelection(NI);
	ExBuf[EBPos]=8;
	ExBuf[EBPos+1]=NI+(SHIFT*32);
	ExBuf[EBPos+2]=Index;
	EBPos+=3;
	ECMD;
};
//[9][ni][ObjID:16][OT]
void CmdBuildObj(byte NI,word ObjID){
	//if(!NOPAUSE)return;//!!!
	if(GetKeyState(VK_CONTROL)&0x8000)return;
	SCMD;
	ExBuf[EBPos]=9;
	ExBuf[EBPos+1]=NI;
	*(word*)(&ExBuf[EBPos+2])=ObjID;
    if(GetKeyState(VK_SHIFT)&0x8000)ExBuf[EBPos+4]=2;
    else ExBuf[EBPos+4]=0;
	EBPos+=5;
	ECMD;
};
void CmdAddBuildObj(byte NI,word ObjID){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=9;
	ExBuf[EBPos+1]=NI;
	*(word*)(&ExBuf[EBPos+2])=ObjID;
    ExBuf[EBPos+4]=1;
	EBPos+=5;
	ECMD;
};
//[0A][NI][kind][owner][count:16][x1][y1][x2][y2]...
//building wall
void CmdBuildWall(byte NI,short xx,short yy){

};
//[0B][NI][xx:16][yy:16][OrdType]
void CmdRepairWall(byte NI,short xx,short yy){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=0xB;
	ExBuf[EBPos+1]=NI;
	*(word*)(&ExBuf[EBPos+2])=xx;
	*(word*)(&ExBuf[EBPos+4])=yy;
	if(GetKeyState(VK_SHIFT)&0x8000)ExBuf[EBPos+6]=2;
	else ExBuf[EBPos+6]=0;
	EBPos+=7;
	ECMD;
};
//[0C][NI][LINK INDEX:16]
void CmdDamageWall(byte NI,word LIN){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=0xC;
	ExBuf[EBPos+1]=NI;
	*(word*)(&ExBuf[EBPos+2])=LIN;
	EBPos+=4;
	ECMD;
};
//[0D][NI][x][y][ResID]
void CmdTakeRes(byte NI,int x,int y,byte ResID){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=0xD;
	ExBuf[EBPos+1]=NI;
	*(int*)(&ExBuf[EBPos+2])=x;
	*(int*)(&ExBuf[EBPos+6])=y;
	ExBuf[EBPos+10]=ResID;
	EBPos+=11;
	ECMD;
};
//[0E][NI][UpgradeIndex:16]
void CmdPerformUpgrade(byte NI,word UI){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=0xE;
	ExBuf[EBPos+1]=NI;
	*(word*)(&ExBuf[EBPos+2])=UI;
	EBPos+=4;
	ECMD;
};
//[0F][NI][OID:16]
void CmdGetOil(byte NI,word UI){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=0xF;
	ExBuf[EBPos+1]=NI;
	*(word*)(&ExBuf[EBPos+2])=UI;
	EBPos+=4;
	ECMD;
};
//[10][NI][x][y][x1][y1][Kind]
void CmdCreateKindSelection(byte NI,byte x,byte y,byte x1,byte y1,byte Kind){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=16;
	ExBuf[EBPos+1]=NI;
	ExBuf[EBPos+2]=x;
	ExBuf[EBPos+3]=y;
	ExBuf[EBPos+4]=x1;
	ExBuf[EBPos+5]=y1;
	ExBuf[EBPos+6]=Kind;
	EBPos+=7;
	ECMD;
};
//[11][NI][x][y][x1][y1][Type]
void CmdCreateTypeSelection(byte NI,byte x,byte y,byte x1,byte y1,word Type){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=17;
	ExBuf[EBPos+1]=NI;
	ExBuf[EBPos+2]=x;
	ExBuf[EBPos+3]=y;
	ExBuf[EBPos+4]=x1;
	ExBuf[EBPos+5]=y1;
	*(word*)(&ExBuf[EBPos+6])=Type;
	EBPos+=8;
	ECMD;
};
void CreateGoodSelection(byte NI,int xx,int yy,int xx1,int yy1,CHOBJ* FN,int NN,bool Addon);
bool FnKind(OneObject* OB,int N){
	return (OB->Kind==N);
};
bool FnType(OneObject* OB,int N){
	return (OB->NIndex==N);
};
//[12][ni][x:16][y:16][x1:16][y1:16]
void CmdCreateGoodSelection(byte NI,int x,int y,int x1,int y1){
	//if(!NOPAUSE)return;//!!!
	bool Addon=false;
	if(GetKeyState(VK_SHIFT)&0x8000)Addon=true;
	CreateGoodSelection(NI,x,y,x1,y1,NULL,0,Addon);
};
//[13][NI][x][y][x1][y1][Kind]
void CmdCreateGoodKindSelection(byte NI,int x,int y,int x1,int y1,byte Kind){
	//if(!NOPAUSE)return;//!!!
	bool Addon=false;
	if(GetKeyState(VK_SHIFT)&0x8000)Addon=true;
	CreateGoodSelection(NI,x,y,x1,y1,&FnKind,Kind,Addon);
};
//[14][NI][x][y][x1][y1][Type]
void CmdCreateGoodTypeSelection(byte NI,int x,int y,int x1,int y1,word Type){
	//if(!NOPAUSE)return;//!!!
	bool Addon=false;
	if(GetKeyState(VK_SHIFT)&0x8000)Addon=true;
	CreateGoodSelection(NI,x,y,x1,y1,&FnType,Type,Addon);
};
//[15][NI][x][y]
void CmdSetDst(byte NI,int x,int y){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=21;
	ExBuf[EBPos+1]=NI;
	*(int*)(&ExBuf[EBPos+2])=x;
	*(int*)(&ExBuf[EBPos+6])=y;
	EBPos+=10;
	ECMD;
};
//[16][NI][x][y]
void CmdSendToPoint(byte NI,byte x,byte y){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=22;
	ExBuf[EBPos+1]=NI;
	ExBuf[EBPos+2]=x;
	ExBuf[EBPos+3]=y;
	EBPos+=4;
	ECMD;
};
//[17][ni][x][y]
void CmdAttackToXY(byte NI,byte x,byte y){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=23;
	ExBuf[EBPos+1]=NI;
	ExBuf[EBPos+2]=x;
	ExBuf[EBPos+3]=y;
	EBPos+=4;
	ECMD;
};
//[18][ni]
void CmdStop(byte NI){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=24;
	ExBuf[EBPos+1]=NI;
	EBPos+=2;
	ECMD;
};
//[19][ni]
void CmdStandGround(byte NI){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=25;
	ExBuf[EBPos+1]=NI;
	EBPos+=2;
	ECMD;
};
//[1A][ni][x][y]
void CmdPatrol(byte NI,int x,int y){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=26;
	ExBuf[EBPos+1]=NI;
	*(int*)(&ExBuf[EBPos+2])=x;
	*(int*)(&ExBuf[EBPos+6])=y;
	EBPos+=10;
	ECMD;
};
//[1B][ni][x][y]
void CmdRepair(byte NI,byte x,byte y){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=27;
	ExBuf[EBPos+1]=NI;
	ExBuf[EBPos+2]=x;
	ExBuf[EBPos+3]=y;
	EBPos+=4;
	ECMD;
};
//[1C][ni][x][y]
void CmdGetResource(byte NI,byte x,byte y){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=28;
	ExBuf[EBPos+1]=NI;
	ExBuf[EBPos+2]=x;
	ExBuf[EBPos+3]=y;
	EBPos+=4;
	ECMD;
};

//[1E][ni][ID:16][kind]

//[1F][ni][ID:16]
void CmdSendToTransport(byte NI,word ID){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=31;
	ExBuf[EBPos+1]=NI;
	*(word*)(&ExBuf[EBPos+2])=ID;
	EBPos+=4;
	ECMD;
};
//[20)][ni][x][y]
void CmdUnload(byte NI,byte x,byte y){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=32;
	ExBuf[EBPos+1]=NI;
	ExBuf[EBPos+2]=x;
	ExBuf[EBPos+3]=y;
	EBPos+=4;
	ECMD;
};
//[21][ni]
void CmdDie(byte NI){
	if(!GSets.CGame.SilenceMessageEvents){
		//if(!NOPAUSE)return;//!!!
		SCMD;
		ExBuf[EBPos]=33;
		ExBuf[EBPos+1]=NI;
		EBPos+=2;
		ECMD;
	}
};
//[22][ni][x][y]
void CmdContinueAttackPoint(byte NI,byte x,byte y){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=34;
	ExBuf[EBPos+1]=NI;
	ExBuf[EBPos+2]=x;
	ExBuf[EBPos+3]=y;
	EBPos+=4;
	ECMD;
};
//[23][ni][x][y]
void CmdContinueAttackWall(byte NI,byte x,byte y){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=35;
	ExBuf[EBPos+1]=NI;
	ExBuf[EBPos+2]=x;
	ExBuf[EBPos+3]=y;
	EBPos+=4;
	ECMD;
};
//[24][ni]
void CmdSitDown(byte NI){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=36;
	ExBuf[EBPos+1]=NI;
	EBPos+=2;
	ECMD;
};
//[25][ni][x][y]
void CmdNucAtt(byte NI,byte x,byte y){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=37;
	ExBuf[EBPos+1]=NI;
	ExBuf[EBPos+2]=x;
	ExBuf[EBPos+3]=y;
	EBPos+=4;
	ECMD;
};
//[26][ni][w:ObjID]
void CmdUnProduceObj(byte NI,word Type){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=38;
	ExBuf[EBPos+1]=NI;
	*(word*)(&ExBuf[EBPos+2])=Type;
	EBPos+=4;
	ECMD;
};
//[27][ni][w:ObjID]
void CmdSetRprState(byte NI,byte State){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=39;
	ExBuf[EBPos+1]=NI;
	ExBuf[EBPos+2]=State;
	EBPos+=3;
	ECMD;
};
char LASTSAVEFILE[64]="";
//[28][ni][ID:32][Length:8][Name...]
void CmdSaveNetworkGame(byte NI,int ID,char* Name){
	SCMD;
	ExBuf[EBPos]=40;
	ExBuf[EBPos+1]=NI;
	*(int*)(&ExBuf[EBPos+2])=ID;
	byte Len=strlen(Name);
	ExBuf[EBPos+6]=Len;
	strcpy((char*)(&ExBuf[EBPos+7]),Name);
	EBPos+=7+Len;
	ECMD;
};
//[29][ni][ID:32][Length:8][Name...]
void CmdLoadNetworkGame(byte NI,int ID,char* Name){
	byte mm[8];
	SCMD;
	ExBuf[EBPos]=41;
	ExBuf[EBPos+1]=NI;
	*(int*)(&ExBuf[EBPos+2])=ID;
	byte Len=strlen(Name);
	ExBuf[EBPos+6]=Len;
	strcpy((char*)(&ExBuf[EBPos+7]),Name);
	EBPos+=7+Len;
	ECMD;
};
//[2A][ni][ID]
void ImChooseUnSelectType(byte NI,word ID);
void ImChooseSelectType(byte NI,word ID);
void CmdChooseSelType(byte NI,word ID){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ImChooseSelectType(NI,ID);
	ExBuf[EBPos]=42;
	ExBuf[EBPos+1]=NI;
	*(word*)(&ExBuf[EBPos+2])=ID;
	EBPos+=4;
	ECMD;
};
//[2B][ni][ID]
void CmdChooseUnSelType(byte NI,word ID){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ImChooseUnSelectType(NI,ID);
	ExBuf[EBPos]=43;
	ExBuf[EBPos+1]=NI;
	*(word*)(&ExBuf[EBPos+2])=ID;
	EBPos+=4;
	ECMD;
};
//[2C][NI][x:16][y:16][ID:16]
void CmdCreateNewTerr(byte NI,int x,int y,word ID){
	//if(!NOPAUSE)return;//!!!
	if(IsMassiveGame()){
        SetMyActiveSubset();
		void CreateNewTerrMons(byte NI,int x,int y,word Type);
		CreateNewTerrMons(NI,x,y,ID);
		SetDefaultActiveSubset();
	}else{
		SCMD;
		ExBuf[EBPos]=44;
		ExBuf[EBPos+1]=GSets.CGame.cgi_NatRefTBL[NI];
		*(int*)(&ExBuf[EBPos+2])=x;
		*(int*)(&ExBuf[EBPos+6])=y;
		*(word*)(&ExBuf[EBPos+10])=ID;
		EBPos+=12;
		ECMD;
	}
};
//[2D][NI][ID:16]
void CmdGoToMine(byte NI,word ID){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=45;
	ExBuf[EBPos+1]=NI;
	*(word*)(&ExBuf[EBPos+2])=ID;
	EBPos+=4;
	ECMD;
};
//[2E][NI][Type:16]
void CmdLeaveMine(byte NI,word Type){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=46;
	ExBuf[EBPos+1]=NI;
	*(word*)(&ExBuf[EBPos+2])=Type;
	EBPos+=4;
	ECMD;
};
void CmdGoToTransport(byte NI,word ID){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=48;
	ExBuf[EBPos+1]=NI;
	*(word*)(&ExBuf[EBPos+2])=ID;
	EBPos+=4;
	ECMD;
};
void CmdCreateGates(byte NI){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=49;
	ExBuf[EBPos+1]=NI;
	EBPos+=2;
	ECMD;
};
void CmdOpenGates(byte NI){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=50;
	ExBuf[EBPos+1]=NI;
	EBPos+=2;
	ECMD;
};
void CmdCloseGates(byte NI){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=51;
	ExBuf[EBPos+1]=NI;
	EBPos+=2;
	ECMD;
};
void CmdCreateWall(byte NI){	
	SCMD;
	ECMD;
};
void ImSelectAllBuildings(byte NI);
void CmdSelectBuildings(byte NI){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ImSelectAllBuildings(NI);
	ExBuf[EBPos]=52;
	ExBuf[EBPos+1]=NI;
	EBPos+=2;
	ECMD;
};
void ImUnSelectBrig(byte NI,word ID);
void ImSelectBrig(byte NI,word ID);
void CmdChooseSelBrig(byte NI,word ID){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ImSelectBrig(NI,ID);
	ExBuf[EBPos]=53;
	ExBuf[EBPos+1]=NI;
	*(word*)(&ExBuf[EBPos+2])=ID;
	EBPos+=4;
	ECMD;
};
void CmdChooseUnSelBrig(byte NI,word ID){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ImUnSelectBrig(NI,ID);
	ExBuf[EBPos]=54;
	ExBuf[EBPos+1]=NI;
	*(word*)(&ExBuf[EBPos+2])=ID;
	EBPos+=4;
	ECMD;
};
void CmdErseBrigs(byte NI){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=55;
	ExBuf[EBPos+1]=NI;
	EBPos+=2;
	ECMD;
};
void CmdMakeStandGround(byte NI){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=56;
	ExBuf[EBPos+1]=NI;
	EBPos+=2;
	ECMD;
};
void CmdCancelStandGround(byte NI){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=57;
	ExBuf[EBPos+1]=NI;
	EBPos+=2;
	ECMD;
};
void CmdCrBig(byte NI,int i){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=58;
	ExBuf[EBPos+1]=NI;
	*(int*)(&ExBuf[EBPos+2])=i;
	EBPos+=6;
	if(ImNSL[NI]){
		int N=ImNSL[NI];
		for(int j=1;j<N;j++){
			word MID=ImSelm[NI][j];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB)OB->ImSelected&=~GM(NI);
			};
		};
		ImNSL[NI]=1;
	};
	ECMD;
};
void ImSelBrigade(byte NI,byte Type,byte BNAT,int ID);
void CmdSelBrig(byte NI,byte Type,word ID){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ImSelBrigade(NI,Type,GSets.CGame.cgi_NatRefTBL[NI],ID);
	ExBuf[EBPos]=59;
	ExBuf[EBPos+1]=NI;
	ExBuf[EBPos+2]=Type;
	*(word*)(&ExBuf[EBPos+3])=ID;
	EBPos+=5;
	ECMD;
};
void CmdTorg(byte NI,byte SellRes,byte BuyRes,int SellAmount){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=60;
	ExBuf[EBPos+1]=NI;
	ExBuf[EBPos+2]=SellRes;
	ExBuf[EBPos+3]=BuyRes;
	*(int*)(&ExBuf[EBPos+4])=SellAmount;
	EBPos+=8;
	ECMD;
};
void CmdFieldBar(byte NI,word n){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=61;
	ExBuf[EBPos+1]=NI;
	*(word*)(&ExBuf[EBPos+2])=n;
	EBPos+=4;
	ECMD;
};
void CmdSetSrVictim(byte NI,byte val){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=62;
	ExBuf[EBPos+1]=NI;
	ExBuf[EBPos+2]=val;
	EBPos+=3;
	ECMD;
};
void CmdSetShotMyUnitsState(byte NI,byte val){
	SCMD;
	ExBuf[EBPos]=94;
	ExBuf[EBPos+1]=NI;
	ExBuf[EBPos+2]=val;
	EBPos+=3;
	ECMD;
};
void CmdSetArmAttackState(byte NI,byte val){
	SCMD;
	ExBuf[EBPos]=95;
	ExBuf[EBPos+1]=NI;
	ExBuf[EBPos+2]=val;
	EBPos+=3;
	ECMD;
};
void CmdSetStopBuildState(byte NI,byte val){
	SCMD;
	ExBuf[EBPos]=97;
	ExBuf[EBPos+1]=NI;
	ExBuf[EBPos+2]=val;
	EBPos+=3;
	ECMD;
};
void CmdKartech(byte NI){
	SCMD;
	ExBuf[EBPos]=98;
	ExBuf[EBPos+1]=NI;
	EBPos+=2;
	ECMD;
};
void CmdPreKartech(byte NI){
	SCMD;
	ExBuf[EBPos]=99;
	ExBuf[EBPos+1]=NI;
	EBPos+=2;
	ECMD;
};
void CmdFishing(byte NI,byte val){
	SCMD;
	ExBuf[EBPos]=100;
	ExBuf[EBPos+1]=NI;
	ExBuf[EBPos+2]=val;
	EBPos+=3;
	ECMD;
};
void CmdAutozasev(byte NI,byte val){
	SCMD;
	ExBuf[EBPos]=101;
	ExBuf[EBPos+1]=NI;
	ExBuf[EBPos+2]=val;
	EBPos+=3;
	ECMD;

};
void CmdChangeNPID(byte NI,word Type){
	SCMD;
	ExBuf[EBPos]=102;
	ExBuf[EBPos+1]=NI;
	*((word*)(ExBuf+EBPos+2))=Type;
	EBPos+=4;
	ECMD;
};
extern City CITY[8];
void ComChangeNPID(byte NI,word Type){
	CITY[NI].NationalPeasantID=Type;
};
void CmdShotLine(byte NI,int i){
	SCMD;
	ExBuf[EBPos]=103;
	ExBuf[EBPos+1]=NI;
	ExBuf[EBPos+2]=i>>13;
	ExBuf[EBPos+3]=i&7;
	*((word*)(ExBuf+EBPos+4))=(i>>3)&1023;
	EBPos+=6;
	ECMD;
};
void ComShotLine(byte NI,byte State,int Line,int BID){
	Brigade* BR=CITY[NI].Brigs+BID;
	if(BR->WarType){
		OrderDescription* ODE=ElementaryOrders+BR->WarType-1;
		int NA=ODE->NActualLines;
		int NL=0;
		//if((NA==1||NA==2||NA==3)&&Line<NA){
			int FL=ODE->FirstActualLine;
			int bp=NBPERSONAL;
			for(int p=0;p<NA;p++){
				int NU=ODE->LineNU[p+FL];
				if(NU){
					for(int q=0;q<NU;q++){
						if(bp<BR->NMemb){
							word ID=BR->Memb[bp];
							if(ID!=0xFFFF&&p==Line){
								OneObject* OB=Group[ID];
								if(OB&&OB->Serial==BR->MembSN[bp]){
#ifdef SIMPLEMANAGE
									if(OB->newMons->ArmAttack){
										OB->RifleAttack=!State;
										if(State){
											if(OB->EnemyID!=0xFFFF&&CheckIfPossibleToBreakOrder(OB))OB->ClearOrders();
										};
									}else{
										OB->NoSearchVictim=State;
										if(State){
											if(OB->EnemyID!=0xFFFF&&CheckIfPossibleToBreakOrder(OB))OB->ClearOrders();
										};
									};

#else //SIMPLEMANAGE
									OB->NoSearchVictim=State;
									if(State){
										if(OB->EnemyID!=0xFFFF&&CheckIfPossibleToBreakOrder(OB))OB->ClearOrders();
									};
#endif //SIMPLEMANAGE
								};
							};
							bp++;
						};
					};
					NL++;
				};
			};
		//};
	};
};
void ComShotLine(byte NI,byte State,int Line){
	City* C=CITY+GSets.CGame.cgi_NatRefTBL[NI];
	for(int i=0;i<C->NBrigs;i++){
		Brigade* BR=C->Brigs+i;
		if(BR->Enabled&&BR->WarType){
			bool sel=false;
			for(int bp=0;bp<BR->NMemb;bp++){
				word ID=BR->Memb[bp];
				if(ID!=0xFFFF){
					OneObject* OB=Group[ID];
					if(OB&&OB->Serial==BR->MembSN[bp]&&!OB->Sdoxlo&&OB->Selected){
						sel=true;
					}
				}
			}
			if(sel) ComShotLine(GSets.CGame.cgi_NatRefTBL[NI],State,Line,i);
		}
	}
}
void CmdMakeOneStep(byte NI,char Dir){
	SCMD;
	ExBuf[EBPos]=104;
	ExBuf[EBPos+1]=NI;
	ExBuf[EBPos+2]=Dir;
	EBPos+=3;
	ECMD;
};
int TryToMove(OneObject* OB,byte NewDir,bool Dirc);

bool GetBrCenter(Brigade* BR,int* x,int* y);
void ComMakeOneStep(byte NI,int BrigID,char Dir){
	Brigade* BR=CITY[GSets.CGame.cgi_NatRefTBL[NI]].Brigs+BrigID;
	word* IDS=BR->Memb;
	word* SNS=BR->MembSN;
	int N=BR->NMemb;
	//for(int i=0;i<N;i++){
	//	word MID=IDS[i];
	//	if(MID!=0xFFFF){
	//		OneObject* OB=Group[MID];
	//		if(OB&&OB->Serial==SNS[i])return;
	//	};
	//};
	byte DIR;
	if(Dir>0)DIR=BR->Direction+128;
	else DIR=BR->Direction;
	int DX=TCos[DIR];
	int DY=TSin[DIR];
	DX<<=2;
	DY<<=2;
	DX*=abs(Dir);
	DY*=abs(Dir);
	REALTIME++;
	if(BR->Enabled&&BR->WarType&&BR->PosCreated){
		if(ElementaryOrders[BR->WarType-1].OrdUsage==2)return;
		int x=0;
		int y=0;
		int N=0;
		for(int j=NBPERSONAL;j<BR->NMemb;j++){
			x+=BR->posX[j];
			y+=BR->posY[j];
			N++;
		};
		if(N){
			x/=N;
			y/=N;
			bool ae=BR->AttEnm;
			addrand(37);
			void CancelStandGroundAnyway(Brigade* BR);
			CancelStandGroundAnyway(BR);
			BR->HumanLocalSendTo((x<<4)+DX,(y<<4)+DY,BR->Direction,128+16,0);
			void MakeStandGroundTemp(Brigade* BR);
			MakeStandGroundTemp(BR);
			BR->AttEnm=ae;
		};
	};
};
void ComRotateBrigade(byte NI,word BrigID,byte Dir){
	Brigade* BR=CITY[NI].Brigs+BrigID;
	if(BR->Enabled&&BR->WarType&&BR->Direction!=Dir){
		int x,y;
		if(GetBrCenter(BR,&x,&y)){
			addrand(38);
			BR->HumanLocalSendTo(x<<4,y<<4,Dir,128+16,0);
		};
	};
};
void ComRotateFormations(byte NI,byte Dir){
	int NBR=0;
	word BRIDS[16];
	word* SMon=Selm[NI];
	word* SNS=SerN[NI];
	int N=NSL[NI];
	for(int i=0;i<N&&NBR<16;i++){
		word MID=SMon[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]){
				int BID=OB->BrigadeID;
				if(BID!=0xFFFF){
					for(int j=0;j<NBR&&BRIDS[j]!=BID;j++);
					if(j>=NBR){
						BRIDS[NBR]=BID;
						NBR++;
						ComRotateBrigade(NI,BID,Dir);
					};
				};
			};
		};
	};
};
void CmdRotateFormations(byte NI,byte Dir){
	SCMD;
	ExBuf[EBPos]=105;
	ExBuf[EBPos+1]=NI;
	ExBuf[EBPos+2]=Dir;
	EBPos+=3;
	ECMD;

};
void CmdConnectObjs(byte NI,word ID,word CurObj){
	SCMD;
	ExBuf[EBPos]=106;
	ExBuf[EBPos+1]=NI;
	*((word*)(ExBuf+EBPos+2))=ID;
	*((word*)(ExBuf+EBPos+4))=CurObj;
	EBPos+=6;
	ECMD;
};

void ComConnectObjs(byte NI,word ID,word CurObj){
#ifdef COSSACKS2
	void ConnectToObject(word Head,word Tail);
	ConnectToObject(ID,CurObj);
#endif
};
void CmdThrowGrenade(byte NI,int i){
	SCMD;
	ExBuf[EBPos]=107;
	ExBuf[EBPos+1]=NI;
	*((word*)(ExBuf+EBPos+2))=i;
	EBPos+=4;
	ECMD;
};
__forceinline decch(char& c,char v){
	if(c>-120)c-=v;
#ifdef _INLINES
	return 0;
#endif
};
__forceinline incch(char& c,char v){
	if(c<120)c+=v;
#ifdef _INLINES
	return 0;
#endif
};
int DIntCmpFunc(const void* v1,const void* v2){
	int V1=*((int*)v1);
	int V2=*((int*)v2);
	if(V1<V2)return 1;
	else if(V2<V1)return -1;
	else return 0;
};
void NewAttackPointLink(OneObject* OBJ);
void ComThrowGrenade(byte NI,int ID){
#ifdef COSSACKS2
	Brigade* BR=CITY[NI].Brigs+ID;
	if(BR->WarType){
		NewMonster* NM=NATIONS[NI].Mon[BR->MembID]->newMons;
		if(NM->MaxGrenadesInFormation&&BR->NGrenades){
			//searching for victims
			int x,y;
			if(BR->GetCenter(&x,&y)){
#define FSIZE 32
#define FSSHIFT 6
#define FCLSHIFT 5
#define FSIZE2 (FSIZE+FSIZE)
				//x>>=4;
				//y>>=4;
				char Field[FSIZE2*FSIZE2];
				memset(Field,0,sizeof Field);
				byte mask=NATIONS[NI].NMask;
				for(int i=0;i<MAXOBJECT;i++){
					OneObject* OB=Group[i];
					if(OB&&!(OB->Sdoxlo||OB->NewBuilding||!(OB->MathMask&1))){
						int CX=(((OB->RealX>>4)-x)>>FCLSHIFT)+FSIZE;
						int CY=(((OB->RealY>>4)-y)>>FCLSHIFT)+FSIZE;
						if(CX>0&&CY>0&&CX<FSIZE2-1&&CY<FSIZE2-1){
							int ofs=CX+(CY<<FSSHIFT);
							if(OB->NMask&mask){
								decch(Field[ofs],2);
								decch(Field[ofs-1],1);
								decch(Field[ofs+1],1);
								decch(Field[ofs-FSIZE2],1);
								decch(Field[ofs+FSIZE2],1);
							}else{
								incch(Field[ofs],1);
							};
						};
					};
				};
				int sdata[128];
				//int idxs[64];
				//int vals[64];
				int NVals=0;
				for(i=0;i<FSIZE-2&&NVals<64;i++){
					char* xi=Rarr[i].xi;
					char* yi=Rarr[i].yi;
					int N=Rarr[i].N;
					for(int j=0;j<N;j++){
						int ofs=(FSIZE+xi[j])+(int(FSIZE+yi[j])<<FSSHIFT);
						int V=Field[ofs];
						if(V>0&&NVals<64){
							sdata[NVals+NVals+1]=ofs;
							sdata[NVals+NVals  ]=V;
							NVals++;
						};
					};
				};
				if(NVals){
					byte UnitUsed[512];
					memset(UnitUsed,0,sizeof UnitUsed);
					qsort(sdata,NVals,8,&DIntCmpFunc);
					//if(NVals>BR->NGrenades)NVals=BR->NGrenades;
					for(int i=0;i<NVals&&BR->NGrenades;i++){
						int V=sdata[i+i+1];
						int vx=x+(((V%FSIZE2)-FSIZE)<<5)+16;
						int vy=y+(((V/FSIZE2)-FSIZE)<<5)+16;
						int RMIN=10000;
						word UID=0xFFFF;
						word BIDX=0;
						int R0=NATIONS[NI].Mon[BR->MembID]->MoreCharacter->AttackRadius2[2]-32;
						int R00=NATIONS[NI].Mon[BR->MembID]->MoreCharacter->AttackRadius1[2];
						for(int j=NBPERSONAL;j<BR->NMemb;j++)if(!UnitUsed[j]){
							word ID=BR->Memb[j];
							if(ID!=0xFFFF){
								OneObject* OB=Group[ID];
								if(OB&&CheckIfPossibleToBreakOrder(OB)){
									int R=Norma((OB->RealX>>4)-vx,(OB->RealY>>4)-vy);
									if(R<RMIN&&R>R00&&R<R0){
										RMIN=R;
										UID=ID;
										BIDX=j;
									};
								};
							};
						};
						if(UID!=0xFFFF){
							UnitUsed[BIDX]=1;
							OneObject* OB=Group[UID];
							if(OB){
								OB->NewAnm=OB->newMons->GetAnimation(anm_Attack+2);
								OB->delay=0;
								OB->SetZeroFrame();
								OB->NewState=3;
								OB->LocalNewState=3;

								if(OB->NewAttackPoint(vx,vy,128+16,1,1)){;
									BR->NGrenades--;
								}
							}
						}
					}
				}
			}
			
		};
	};
#endif //COSSACKS2
};
CEXPORT int AddAbilityToBrigade(byte NI,word BrIndex,int AblIndex){
#ifdef COSSACKS2
	Brigade* BR=CITY[NI].Brigs+BrIndex;
	if(BR->NABL<MAXABL){
		OneBrigAbility* ABL=ABILS.Abils+AblIndex;
		if(ABL->UsageKind!=3&&BR->NABL>=4)return -1;//not enough place
		if(ABL->UsageKind==1){
			for(int i=0;i<BR->NABL;i++)if(BR->Abilki[i]==AblIndex){
				if(BR->LeftTime[i]<0xFFFF){
					BR->LeftTime[i]=ABL->ExistTime;
				}else return -2;//such ability already exists
			}
		}
		if(ABL->UsageKind==2){
			for(int i=0;i<BR->NABL;i++)
				if(ABILS.Abils[BR->Abilki[i]].AbilityKind==ABL->AbilityKind)
					return -3;//ability of such kind exists
		}
		BR->Abilki[BR->NABL]=AblIndex;
		BR->LeftTime[BR->NABL]=ABL->ExistTime?ABL->ExistTime:0xFFFF;
		BR->MaxLeftTime[BR->NABL]=BR->LeftTime[BR->NABL];
		BR->NABL++;
		return 0;//ok. done
	}else return -1;//not enough place
#endif
};
CEXPORT void ApplyAbility(byte NI,word BrIndex,int AblIndex){
#ifdef COSSACKS2
	Brigade* BR=CITY[NI].Brigs+BrIndex;
	OneBrigAbility* ABL=ABILS.Abils+AblIndex;
	switch(ABL->AbilityKind){
	case 0://HEAL
		{
			for(int i=0;i<BR->NMemb;i++){
				word MID=BR->Memb[i];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&OB->Serial==BR->MembSN[i]){
						OB->Life+=ABL->Value;
						if(OB->Life>OB->MoreCharacter->Life)OB->Life=OB->MoreCharacter->Life;
					};
				}
			}
		}
		break;
	case 16://TIRING
		{
			for(int i=0;i<BR->NMemb;i++){
				word MID=BR->Memb[i];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&OB->Serial==BR->MembSN[i]){
						OB->GetTired+=ABL->Value*1000;						
						if(OB->GetTired>100000)OB->GetTired=100000;
					};
				}
			}
		}
		break;
	case 65://ADDMORALE
		{
			BR->Morale+=10000*ABL->Value;
			if(BR->Morale>BR->MaxMorale)BR->Morale=BR->MaxMorale;
		}
		break;
	case 254://RADIATE
		{
			for(int j=0;j<ABL->Value;j++)if(ABL->Collection[j]!=254)ApplyAbility(NI,BrIndex,ABL->Collection[j]);
		}
		break;
	default:
		AddAbilityToBrigade(NI,BrIndex,AblIndex);
	}
#endif
}
CEXPORT void UseAbility(byte NI,word BrIndex,int AblIndex){
#ifdef COSSACKS2
	Brigade* BR=CITY[NI].Brigs+BrIndex;
	OneBrigAbility* ABL=ABILS.Abils+AblIndex;
	for(int i=0;i<BR->NABL;i++)if(BR->Abilki[i]==AblIndex){
		if(i<BR->NABL){
			memcpy(BR->Abilki+i,BR->Abilki+i+1,(BR->NABL-i-1)<<1);
			memcpy(BR->LeftTime+i,BR->LeftTime+i+1,(BR->NABL-i-1)<<1);
		}
		BR->NABL--;
		if(ABL->Radius){
			//applying to all brigades in radius
			word Brigs[256];
			word NIS[256];
			int NBR=0;
			byte Mask=1<<NI;
			int x,y;
			BR->GetCenter(&x,&y);
			int DR=int(ABL->Radius)<<4;
			for(int i=0;i<MAXOBJECT&&NBR<256;i++){
				OneObject* OB=Group[i];
				if(OB&&OB->BrigadeID!=0xFFFF&&OB->NMask&Mask&&(!OB->Sdoxlo)&&Norma(OB->RealX-x,OB->RealY-y)<DR){
					Brigade* BR1=CITY[OB->NNUM].Brigs+OB->BrigadeID;
					if(BR1->ID<1024){
						ApplyAbility(OB->NNUM,OB->BrigadeID,AblIndex);
						BR1->ID|=1024;
						Brigs[NBR]=OB->BrigadeID;
						NIS[NBR]=OB->NNUM;
						NBR++;
					}
				}
			}
			for(i=0;i<NBR;i++){
				Brigade* BR1=CITY[NIS[i]].Brigs+Brigs[i];
				BR1->ID&=1023;
			}
		}else ApplyAbility(NI,BrIndex,AblIndex);
	}
#endif
};
CEXPORT void DelAbility(byte NI,word BrIndex,int AblIndex){
#ifdef COSSACKS2
	Brigade* BR=CITY[NI].Brigs+BrIndex;
	OneBrigAbility* ABL=ABILS.Abils+AblIndex;
	for(int i=0;i<BR->NABL;i++)if(BR->Abilki[i]==AblIndex){
		if(i<BR->NABL){
			memcpy(BR->Abilki+i,BR->Abilki+i+1,(BR->NABL-i-1)<<1);
			memcpy(BR->LeftTime+i,BR->LeftTime+i+1,(BR->NABL-i-1)<<1);
		}
		BR->NABL--;
	}
#endif
}
CEXPORT void CmdUseAbility(byte NI,word BrigadeID,word AblType){
	SCMD;
	ExBuf[EBPos]=108;
	ExBuf[EBPos+1]=NI;
	*((word*)(ExBuf+EBPos+2))=BrigadeID;
	*((word*)(ExBuf+EBPos+4))=AblType;
	EBPos+=6;
	ECMD;
}
CEXPORT void CmdAddAbility(byte NI,word BrigadeID,word AblType){
	SCMD;
	ExBuf[EBPos]=109;
	ExBuf[EBPos+1]=NI;
	*((word*)(ExBuf+EBPos+2))=BrigadeID;
	*((word*)(ExBuf+EBPos+4))=AblType;
	EBPos+=6;
	ECMD;
}
CEXPORT void CmdDelAbility(byte NI,word BrigadeID,word AblType){
	SCMD;
	ExBuf[EBPos]=110;
	ExBuf[EBPos+1]=NI;
	*((word*)(ExBuf+EBPos+2))=BrigadeID;
	*((word*)(ExBuf+EBPos+4))=AblType;
	EBPos+=6;
	ECMD;
}
void CmdDisconnectObjs(byte NI){
	SCMD;
	ExBuf[EBPos]=111;
	ExBuf[EBPos+1]=NI;
	EBPos+=2;
	ECMD;
}
void ComDisconnectObjs(byte NI){
	int N=NSL[NI];
	word* IDS=Selm[NI];
	word* SNS=SerN[NI];
	int MINS=255;
	for(int i=0;i<N;i++){
		word MID=IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]){
				void DisconnectFromObject(OneObject* Head);
				DisconnectFromObject(OB);
			}
		}
	}	
}

void ComMakeOneStep(byte NI,char Dir){
	int NBR=0;
	word BRIDS[16];
	word* SMon=Selm[NI];
	word* SNS=SerN[NI];
	int N=NSL[NI];
	for(int i=0;i<N&&NBR<16;i++){
		word MID=SMon[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]){
				int BID=OB->BrigadeID;
				if(BID!=0xFFFF){
					for(int j=0;j<NBR&&BRIDS[j]!=BID;j++);
					if(j>=NBR){
						BRIDS[NBR]=BID;
						NBR++;
						ComMakeOneStep(NI,BID,Dir);
					};
				};
			};
		};
	};
};
void CmdStopUpgrade(byte NI, int UIndex){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=63;
	ExBuf[EBPos+1]=NI;
	*(int*)(&ExBuf[EBPos+2])=UIndex;
	EBPos+=6;
	ECMD;
};
void CmdStopUpgrade(byte NI){
	CmdStopUpgrade(NI,-1);
}
void CmdUnloadAll(byte NI){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=64;
	ExBuf[EBPos+1]=NI;
	EBPos+=2;
	ECMD;
};
void CmdMakeReformation(byte NI,word BrigadeID,byte FormType){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=65;
	ExBuf[EBPos+1]=NI;
	*(word*)(&ExBuf[EBPos+2])=BrigadeID;
	ExBuf[EBPos+4]=FormType;
	EBPos+=5;
	ECMD;
};
void ImSelectIdlePeasants(byte NI);
void PrepareToImSelection(byte NI);
void FinalizeImSelection(byte NI);
void CmdSelectIdlePeasants(byte NI){
	//if(!NOPAUSE)return;//!!!
	//
	//SCMD;
	PrepareToImSelection(NI);
	ImSelectIdlePeasants(NI);
	FinalizeImSelection(NI);
	//ExBuf[EBPos]=66;
	//ExBuf[EBPos+1]=NI;
	//EBPos+=2;
	//ECMD;
};
void ImSelectIdleMines(byte NI);
void CmdSelectIdleMines(byte NI){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	PrepareToImSelection(NI);
	ImSelectIdleMines(NI);
	FinalizeImSelection(NI);
	ExBuf[EBPos]=67;
	ExBuf[EBPos+1]=NI;
	EBPos+=2;
	ECMD;
};
void CmdChangeSpeed(){
	//if(!NOPAUSE)return;//!!!
	SCMD;
	ExBuf[EBPos]=68;
	ExBuf[EBPos+1]=0;
	EBPos+=2;
	ECMD;
};
extern byte PlayGameMode;
void CmdSetSpeed(word Speed){
	SCMD;
	ExBuf[EBPos]=69;
	*((word*)(ExBuf+EBPos+1))=Speed;
	EBPos+=3;
	ECMD;
};
void CmdPause(byte NI){
	SCMD;
	ExBuf[EBPos]=70;
	ExBuf[EBPos+1]=NI;
	EBPos+=2;
	ECMD;
};
void CmdFreeSelected(byte NI){
	SCMD;
	ExBuf[EBPos]=71;
	ExBuf[EBPos+1]=NI;
	EBPos+=2;
	ECMD;
};
void SelectUnitsTypes(byte NI,bool Re,byte Type);
void SelectAllPeasants(byte NI,bool Re);
void CmdSelAllUnits(byte NI){
	SCMD;
	SelectUnitsTypes(NI,0,0);
	ExBuf[EBPos]=72;
	ExBuf[EBPos+1]=NI;
	EBPos+=2;
	ECMD;
};
void CmdSelAllShips(byte NI){	
	SelectUnitsTypes(NI,0,3);
	SCMD;
	ExBuf[EBPos]=73;
	ExBuf[EBPos+1]=NI;
	EBPos+=2;
	ECMD;
};
void CmdSelAllFlyObjects(byte NI){	
	SelectUnitsTypes(NI,0,4);
	SCMD;
	ExBuf[EBPos]=113;
	ExBuf[EBPos+1]=NI;
	EBPos+=2;
	ECMD;
};
void CmdSelAllShooters(byte NI){	
	SelectUnitsTypes(NI,0,2);
	SCMD;
	ExBuf[EBPos]=114;
	ExBuf[EBPos+1]=NI;
	EBPos+=2;
	ECMD;
};
void CmdSelAllPeasants(byte NI){	
	SelectAllPeasants(NI,0);
	SCMD;
	ExBuf[EBPos]=115;
	ExBuf[EBPos+1]=NI;
	EBPos+=2;
	ECMD;
};
void CmdSelAllWarriors(byte NI){	
	SelectUnitsTypes(NI,0,1);
	SCMD;
	ExBuf[EBPos]=116;
	ExBuf[EBPos+1]=NI;
	EBPos+=2;
	ECMD;
};
extern byte PlayGameMode;
extern word NPlayers;
CIMPORT
void SendVictoryState(int ID,byte State);
bool ProcessMessages();
void PlaySound(char* Name);
DLLEXPORT int GetMyNation();
extern bool NoPress;
bool NOWIN=false;
void CmdEndGame(byte NI,byte state,byte cause){
	if(NOWIN)return;
	NoPress=false;
	if(NI==GetMyNation())
	{
		if(state==1)
			PlaySound("VICTORY");
		else
		if(state==2)
			PlaySound("PORAJENIE_NA_KARTE");
	}
	if(cause==255 && NI==MyNation){
		NATIONS[GSets.CGame.cgi_NatRefTBL[MyNation]].VictState=state;
	}
	if(TPEN.Connected){
		//peerMessagePlayer(TPEN.Peer,"spybot","\\launched\\0\\gamename\\cossacks\\");
		TPEN.Disconnect();
	};
	SCMD;
	ExBuf[EBPos]=92;
	ExBuf[EBPos+1]=NI;
	ExBuf[EBPos+2]=state;
	ExBuf[EBPos+3]=cause;
	EBPos+=4;
	ECMD;
	if(UseGSC_Login&&!PlayGameMode){
		for(int i=0;i<NPlayers;i++)if(GSets.CGame.PL_INFO[i].ColorID==NI){
			//ReportIt("CmdEndGame(%s,%d)\n",GSets.CGame.PL_INFO[i].name,state);
			SendVictoryState(GSets.CGame.PL_INFO[i].ProfileID,state);
			ProcessMessages();
			break;
		};
	};
};
void CmdSendData(byte* Data,int size){
	SCMD;
	ExBuf[EBPos]=93;
	ExBuf[EBPos+1]=0;
	((short*)(ExBuf+EBPos+2))[0]=size;
	memcpy(ExBuf+EBPos+4,Data,size);
	EBPos+=size+4;
	ECMD;
};
DIALOGS_API void CmdSendPureData(byte* Data,int size){
	SCMD;
	ExBuf[EBPos]=112;
	memcpy(ExBuf+EBPos+1,Data,size);
	EBPos+=size+1;
	ECMD;
};
void ComSendData(byte* Data,int size){
	if(PerformDipCommand)PerformDipCommand((char*)Data,size);
	//vui_InterfSynchro::Perform((char*)Data,size);
};
int ComSendPureData(byte* Data){
	//if(PerformDipCommand)PerformDipCommand((char*)Data,size);
	return vui_InterfSynchro::Perform((char*)Data);
};
void CmdMoney(byte NI){
	SCMD;
	ExBuf[EBPos]=75;
	ExBuf[EBPos+1]=NI;
	EBPos+=2;
	ECMD;
};
void DoCmdMoney(byte NI){
	for(int i=0;i<6;i++)AddXRESRC(NI,i,150000);
};
void DoCmdAddMoney(byte NI,int Value){
	int MAX=0;
	for(int i=0;i<6;i++)if(XRESRC(NI,i)>MAX)MAX=XRESRC(NI,i);
	MAX=(MAX*(Value-100))/100;
	for(i=0;i<6;i++)AddXRESRC(NI,i,MAX);
};
void CmdGiveMoney(byte SrcNI,byte DstNI,byte Res,int Amount){
	if(Amount<0)return;
	SCMD;
	ExBuf[EBPos]=76;
	ExBuf[EBPos+1]=SrcNI;
	ExBuf[EBPos+2]=DstNI;
	ExBuf[EBPos+3]=Res;
	*((int*)(ExBuf+EBPos+4))=Amount;
	EBPos+=8;
	ECMD;
};
extern int StartTime[8];
void CmdSetStartTime(int* MASK){
	SCMD;
	ExBuf[EBPos]=77;
	ExBuf[EBPos+1]=MyNation;
	memcpy(ExBuf+EBPos+2,MASK,4*8);
	EBPos+=34;
	ECMD;
};
void CmdSetMaxPingTime(int Time){
	SCMD;
	ExBuf[EBPos]=79;
	ExBuf[EBPos+1]=MyNation;
	*((word*)(ExBuf+EBPos+2))=Time;
	EBPos+=4;
	ECMD;
};
void CmdDoItSlow(word DT){
	SCMD;
	ExBuf[EBPos]=80;
	*((word*)(ExBuf+EBPos+1))=DT;
	EBPos+=3;
	//ADDGR(9,GetTickCount(),DT,255);
	//ADDGR(9,GetTickCount(),0,254);
	ECMD;
};
void CmdDoItSlowEx(word DT,byte* Data,DWORD* sz){
	SCMD;
	Data[*sz]=80;
	*((word*)(Data+(*sz)+1))=DT;
	(*sz)+=3;
	//ADDGR(9,GetTickCount(),DT,255);
	//ADDGR(9,GetTickCount(),0,254);
	ECMD;
};
void CmdFillFormation(byte NI,word BrigadeID){
	SCMD;
	ExBuf[EBPos]=82;
	ExBuf[EBPos+1]=NI;
	*((word*)(ExBuf+EBPos+2))=BrigadeID;
	EBPos+=4;
	ECMD;
};
extern DWORD MyDPID;
void CmdOfferVoting(){
	SCMD;
	ExBuf[EBPos]=83;
	ExBuf[EBPos+1]=0;
	*((DWORD*)(ExBuf+EBPos+2))=MyDPID;
	EBPos+=6;
	ECMD;
};
void ComDoVote(DWORD DPID,byte result);
void CmdVote(byte result){
	SCMD;
	ExBuf[EBPos]=84;
	ExBuf[EBPos+1]=0;
	*((DWORD*)(ExBuf+EBPos+2))=MyDPID;
	ExBuf[EBPos+6]=result;
	EBPos+=7;
	ComDoVote(MyDPID,result);
	ECMD;
};
extern int PeaceTimeLeft;
void CmdEndPT(){
	SCMD;
	ExBuf[EBPos]=85;
	ExBuf[EBPos+1]=0;
	EBPos+=2;
	ECMD;
};
void UnregisterSprite(int N);
void ComEndPT(){
	PeaceTimeLeft=0;
	for(int i=0;i<MaxSprt;i++)if(Sprites[i].SG==&HOLES){
		Sprites[i].Enabled=0;
		UnregisterSprite(i);
	};
};
void CmdSetGuardState(byte NI,word State){
	ExBuf[EBPos]=86;
	ExBuf[EBPos+1]=NI;
	*((word*)(ExBuf+EBPos+2))=State;
	EBPos+=4;
};
void CmdAttackGround(byte NI,int x,int y){
	ExBuf[EBPos]=87;
	ExBuf[EBPos+1]=NI;
	*((int*)(ExBuf+EBPos+2))=x;
	*((int*)(ExBuf+EBPos+6))=y;
	EBPos+=10;
};
void CmdDoGroup(byte NI){
	ExBuf[EBPos]=88;
	ExBuf[EBPos+1]=NI;
	EBPos+=2;
};
void CmdUnGroup(byte NI){
	ExBuf[EBPos]=89;
	ExBuf[EBPos+1]=NI;
	EBPos+=2;
};
void CmdChangeNatRefTBL(byte* TBL){
	ExBuf[EBPos]=90;
	ExBuf[EBPos+1]=MyNation;
	memcpy(ExBuf+EBPos+2,TBL,8);
	EBPos+=10;
};
void CmdChangePeaceTimeStage(int Stage){
	ExBuf[EBPos]=91;
	memcpy(ExBuf+EBPos+1,&Stage,2);
	EBPos+=3;
};
void CmdAddMoney(byte NI,DWORD Value){
	ExBuf[EBPos]=96;
	ExBuf[EBPos+1]=NI;
	*((DWORD*)(ExBuf+EBPos+2))=Value;
	EBPos+=6;
};
extern int MaxPeaceTime;
extern int PeaceTimeStage;
void ComChangePeaceTimeStage(int Stage){
	PeaceTimeStage=Stage;
};
void ChangeNatRefTBL(byte NI,byte* TBL){
	int N=NSL[NI];
	if(N){
		word* mon=Selm[NI];
		word* msn=SerN[NI];
		if(mon){
			for(int i=0;i<N;i++){	
				word ID=mon[i];
				if(ID!=0xFFFF){
					OneObject* OB=Group[ID];
					if(OB){
						OB->Selected&=~GM(NI);
					};
				};
			};
			free(mon);
			free(msn);
		};
		Selm[NI]=NULL;
		SerN[NI]=NULL;
		NSL[NI]=0;
		addrand(1);
	};
	N=ImNSL[NI];
	if(N){
		word* mon=ImSelm[NI];
		word* msn=ImSerN[NI];
		if(mon){
			for(int i=0;i<N;i++){	
				word ID=mon[i];
				if(ID!=0xFFFF){
					OneObject* OB=Group[ID];
					if(OB){
						OB->ImSelected&=~GM(NI);
					};
				};
			};
			free(mon);
			free(msn);
		};
		ImSelm[NI]=NULL;
		ImSerN[NI]=NULL;
		ImNSL[NI]=0;
	};
	memcpy(GSets.CGame.cgi_NatRefTBL,TBL,8);
};
void ComAttackGround(byte NI,int x,int y){
	//x>>=4;
	//y>>=4;
	int N=NSL[NI];
	word* IDS=Selm[NI];
	word* SNS=SerN[NI];
	for(int i=0;i<N;i++){
		word MID=IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]&&OB->newMons->Artpodgotovka&&!OB->Sdoxlo){
				OB->NewAttackPoint(x,y,16+128,0,1);
			};
		};
	};
};
void ComSetGuardState(byte NI,word State){
	int N=NSL[NI];
	word* IDS=Selm[NI];
	word* SNS=SerN[NI];
	if(!(SNS&&IDS))return;
	for(int i=0;i<N;i++){
		word MID=IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&!(OB->Sdoxlo||OB->newMons->Capture||OB->NewBuilding||OB->LockType==1)){
				OB->Guard=State;
				// by vital
				OB->StandGround=true;
				OB->ClearOrders();
				OB->DestX=-1;
			};
		};
	};
};
bool VotingMode=0;
int LastVotingTime=0;
DWORD VotersList[8];
DWORD VotingResult[8];
char VotingText[500];
char* GetTextByID(char* ID);
extern word NPlayers;
void ComOfferVoting(DWORD DPID){
	LastVotingTime=GetTickCount();
	VotingMode=1;
	sprintf(VotingText,GetTextByID("PSR_NOREG1"),"???");
	for(int i=0;i<8;i++){
		if(i<NPlayers){
			VotersList[i]=GSets.CGame.PL_INFO[i].PlayerID;
			VotingResult[i]=0;//GSets.CGame.PL_INFO[i].PlayerID==DPID;
		}else{
			VotersList[i]=0;
			VotingResult[i]=0;
		};
		if(GSets.CGame.PL_INFO[i].PlayerID==DPID){
			sprintf(VotingText,GetTextByID("PSR_NOREG1"),GSets.CGame.PL_INFO[i].name);
		};
	};
};
void ComDoVote(DWORD DPID,byte result){
	LastVotingTime=GetTickCount();
	for(int i=0;i<NPlayers;i++)if(GSets.CGame.PL_INFO[i].PlayerID==DPID){
		VotingResult[i]=result;
	};
};
void DontMakeRaiting();
extern bool EnterChatMode;
extern bool KeyPressed;
extern int LastKey;
void UnPress();
extern EXBUFFER EBufs[MaxPL];
extern byte SpecCmd;
void ExplorerOpenRef(int Index,char* ref);
CIMPORT void DisableRatedGame();
void ProcessVotingKeys(){
	if(VotingMode){
		if(!EnterChatMode){
			if(KeyPressed){
				if(LastKey=='y'||LastKey=='Y'){
					SpecCmd=171;
					LastKey=0;
					KeyPressed=0;
					UnPress();
				};
				if(LastKey=='n'||LastKey=='N'){
					SpecCmd=172;
					LastKey=0;
					KeyPressed=0;
					UnPress();
				};
			};
		};
		bool ALLYES=1;
		for(int i=0;i<NPlayers;i++)if(EBufs[i].Enabled){
			if(VotingResult[i]!=1)ALLYES=0;
			if(VotingResult[i]==2){
				if(GetTickCount()<LastVotingTime+60000*3-4000){
					LastVotingTime=GetTickCount()-60000*3+3000;
					char ccc[200];
					sprintf(ccc,GetTextByID("PSR_DENY"),GSets.CGame.PL_INFO[i].name);
					AssignHint1(ccc,150,32);
				};
			};
		};
		if(ALLYES&&GetTickCount()<LastVotingTime+60000*3-4000){
			LastVotingTime=GetTickCount()-60000*3+3000;
			char ccc[200];
			sprintf(ccc,GetTextByID("PSR_ACCEPT"));
			AssignHint1(ccc,150,32);
			DontMakeRaiting();
			if(UseGSC_Login)DisableRatedGame();
		};
		if(GetTickCount()-LastVotingTime>60000*3){
			VotingMode=0;
		};
	};
};
extern int RealLx;
extern int RealLy;
void DrawDoubleTable2(int x0,int y0,int x1,int y1,int ym);
void ProcessVotingView(){
	if(!EnterChatMode){
		if(VotingMode){
			//drawing the results of voiting
			RLCFont* FNT=&YellowFont;
			int LocL=0;
			int MaxLocLx=0;
			int NL=1;
			int L=strlen(VotingText);
			for(int i=0;i<L;i++){
				if(VotingText[i]=='\\'){
					if(LocL>MaxLocLx)MaxLocLx=LocL;
					LocL=0;
					NL++;
				}else{
					int DL;
					LocL+=GetRLCWidthUNICODE(FNT->RLC,(byte*)(VotingText+i),&DL);
					i+=DL-1;
				};
			};
			if(LocL>MaxLocLx)MaxLocLx=LocL;
			int xc=RealLx>>1;
			int y0=RealLy/2-NPlayers*13-NL*13-32;
			int x0=xc-MaxLocLx/2-20;
			DrawDoubleTable2(x0,y0-16,x0+MaxLocLx+40,y0+16+16+4+(NL+NPlayers)*26,y0+16+NL*26);
			int yt=y0+16;
			char TempStr[128];
			char TempPos=0;
			for(i=0;i<L;i++){
				if(VotingText[i]=='\\'||i==L-1){
					if(VotingText[i]!='\\'){
						TempStr[TempPos]=VotingText[i];
						TempStr[TempPos+1]=0;
						TempPos++;
					};
					ShowString(xc-GetRLCStrWidth(TempStr,FNT)/2,y0,TempStr,FNT);
					y0+=26;
					TempPos=0;
					TempStr[0]=0;
				}else{
					TempStr[TempPos]=VotingText[i];
					TempStr[TempPos+1]=0;
					TempPos++;
				};
			};
			y0+=32;
			for(i=0;i<NPlayers;i++){
				char ccx[65];
				switch(VotingResult[i]){
				case 1:
					sprintf(ccx,"%s:%s",GSets.CGame.PL_INFO[i].name,GetTextByID("VOTE_YES"));
					break;
				case 2:
					sprintf(ccx,"%s:%s",GSets.CGame.PL_INFO[i].name,GetTextByID("VOTE_NO"));
					break;
				default:
					sprintf(ccx,"%s:%s",GSets.CGame.PL_INFO[i].name,GetTextByID("VOTE_UNK"));
					break;
				};
				ShowString(xc-GetRLCStrWidth(ccx,FNT)/2,y0,ccx,FNT);
				y0+=26;
			};
		};
	};
};
extern City CITY[8];
bool SlowMade=0;
extern int CurrentStepTime;
extern int NeedCurrentTime;
void DoItSlow(word DT){
	if(!SlowMade){
		if(CurrentStepTime<160)CurrentStepTime+=max((CurrentStepTime>>4),3);
		//if(DT>3000)NeedCurrentTime+=DT>>1;
		SlowMade=1;
		ADDGR(9,tmtmt,DT,0xD0);
	};
};
int MaxDCT=0;
void DoTimeStep(int DT){
	if(DT>MaxDCT)MaxDCT=DT;
	/*
	assert(MaxDCT<40000);
	FILE* F=fopen("log1.log","a");
	if(F){
		fprintf(F,"DT=%d Max=%d t=%d\n",DT,MaxDCT,tmtmt);
		fclose(F);
	};
	*/
};
void ClearEnemyForSelected(byte NI);
int CURMAXP=-1;
extern int MaxPingTime;
void SETPING(byte NI,int T){
	for(int i=0;i<EBPos;i++)addrand(ExBuf[i]);
	addrand(NI);
	//addrand(MaxPingTime);
	if(T>CURMAXP){
		CURMAXP=T;
		MaxPingTime=T;
		addrand(NI);
		addrand(MaxPingTime);
	};
};
int TIMECHANGE[8]={0,0,0,0,0,0,0,0};
extern int AddTime;
extern int NeedAddTime;
extern word NPlayers;
extern DWORD MyDPID;
int NSTT=0;
void Rept (LPSTR sz,...);
void __DoStartTime(int* MASK){
	PSUMM.ClearPingInfo();
	memcpy(TIMECHANGE,MASK,4*8);
	for(int i=0;i<NPlayers;i++)if(GSets.CGame.PL_INFO[i].PlayerID==MyDPID)
		//NeedAddTime=TIMECHANGE[GSets.CGame.PL_INFO[i].ColorID]<<9;
		NeedAddTime=TIMECHANGE[i]<<7;//!!!CHANGED!!!
	//Rept("NeedAddTime=%d (%d)\n",NeedAddTime,NeedAddTime>>9);
	NSTT++;
};
char* GetTextByID(char* ID);
void AssignHint1(char* s,int time);
void DoCmdGiveMoney(byte SrcNI,byte DstNI,byte Res,int Amount){
	int r=XRESRC(SrcNI,Res);
	if(r<Amount)Amount=r;
	AddXRESRC(SrcNI,Res,-Amount);
	AddXRESRC(DstNI,Res,Amount-(3*(Amount/10)));
	AssignHint1(GetTextByID("MONTR"),200,32);
};
extern word NPlayers;
extern EXBUFFER EBufs[MaxPL];
int WaitState=0;
extern int FogMode;
bool PreNoPause=0;
extern bool Lobby;
int ShowGameScreen=0;
struct OneCommandCenter{
	byte Color;
	byte REFTBL[8];
};
int NCommCenters=0;
byte CommCenters[8][9];
void WinnerControl(bool Anyway);
extern byte MI_Mode;
extern int CUR_TOOL_MODE;
extern bool TP_Made;
extern bool LockPause;
extern bool ShowStat;
void SetBrokenState();
CIMPORT
void SendVictoryState(int ID,byte State);
extern bool NoPress;
void __EndGame(byte NI,byte state){
	void ClearBattleLogs();
	ClearBattleLogs();
	NoPress=false;
	int state0=state;
	if(state>2)SetBrokenState();
	if(state==1&&NATIONS[NI].AI_Enabled){
		NATIONS[NI].AI_Enabled=false;
	}
	if(state==1&&NATIONS[NI].VictState!=1&&MOptions.VictoryRule.Get()){
		byte C=GSets.CGame.cgi_NatRefTBL[NI];
		int N=0;
		for(int i=0;i<7;i++)if(i!=NI){
            byte C1=GSets.CGame.cgi_NatRefTBL[i];
			if(C1==C){
				if(NATIONS[i].VictState!=1)N++;
			}
		}
		if(N==0){
			for(int i=0;i<MAXOBJECT;i++){
				OneObject* OB=Group[i];
				if(OB&&OB->NNUM==NI&&!(OB->Sdoxlo||OB->newMons->Immortal))OB->Die();
			}
		}
	}
	if(state==3||state==4){
		//is there alive friends?
		byte mask=NATIONS[NI].NMask;
		int MaxSC=-1000;
		int BestID=-1;
		for(int i=0;i<7;i++)if(NATIONS[i].NMask&mask&&i!=NI&&NATIONS[i].VictState==0){
			//yes, friends are.
			int score=CITY[i].Account;
			if(score>MaxSC){
				MaxSC=score;
				BestID=i;
			};
		};
		if(BestID==-1){
			//need to eliminate all
			for(int i=0;i<7;i++)if(NATIONS[i].NMask&mask)CITY[i].AutoEraseTime=80;
		};
		if(state==4)state=1;
	};
	if(state==5)state=3;
	NATIONS[NI].VictState=state;
	if(NI==MyNation){
		for(int i=0;i<NPlayers;i++)if(EBufs[i].Enabled)EBufs[i].Enabled=GSets.CGame.PL_INFO[i].ColorID==NI;
		PreNoPause=1;
		if(NPlayers>1)FogMode=0;
		WaitState=1;
		for(i=0;i<NPlayers;i++)if(GSets.CGame.PL_INFO[i].ColorID==NI){
			if(state==1)LOOSEIGAME(GSets.CGame.PL_INFO[i].name);
			else if(state==2)WINIGAME(GSets.CGame.PL_INFO[i].name);
		};
	}else{
		for(int i=0;i<NPlayers;i++)if(EBufs[i].Enabled)EBufs[i].Enabled=GSets.CGame.PL_INFO[i].ColorID!=NI;
	};
	Lobby=0;
	if(TPEN.Connected){
		//peerMessagePlayer(TPEN.Peer,"spybot","\\launched\\0\\gamename\\cossacks\\");
		TPEN.Disconnect();
	};
	WinnerControl(true);
	MI_Mode=1;
	CUR_TOOL_MODE=0;
	TP_Made=0;
	if(NPlayers==2&&SCENINF.hLib&&state==1&&NI!=MyNation){
		for(int i=0;i<NPlayers;i++){
			if(GSets.CGame.PL_INFO[i].PlayerID==MyDPID){
				SCENINF.Victory=1;
				NOPAUSE=0;
				LockPause=1;
				ShowStat=0;
				NATIONS[GSets.CGame.PL_INFO[i].ColorID].VictState=2;
			};
		};
	};
	if(UseGSC_Login&&!PlayGameMode){
		for(int i=0;i<NPlayers;i++)if(GSets.CGame.PL_INFO[i].ColorID==NI){
			//ReportIt("__EndGame(%s,%d)\n",GSets.CGame.PL_INFO[i].name,state0);
			SendVictoryState(GSets.CGame.PL_INFO[i].ProfileID,state0);
			ProcessMessages();
			break;
		};
	};
	if(NPlayers>1&&!PlayGameMode){
		//checking if I am alone
		if(NATIONS[MyNation].VictState==0){
			int NAlive=0;
			int Ndefeat=0;
			for(int i=0;i<NPlayers;i++){
				int vs=NATIONS[GSets.CGame.PL_INFO[i].ColorID].VictState;
				if(!vs)NAlive++;
				if(vs==1)Ndefeat++;
			};
			if(NAlive==1&&!Ndefeat){
				//only me is alive
				//ShowCentralText("PLALONE",400);
				NATIONS[MyNation].VictState=3;
			};
		};
	};
	if(state==2)ext_OnVictory(NI);
	else if(state==1)ext_OnDefeat(NI);
};

void __DoVictory(byte NI){
};
extern bool NOPAUSE;

// special for StarForce
__declspec(dllexport)
void ChangePauseState(){
	NOPAUSE=!NOPAUSE;
};
void InitSelection(){
	int sz=(MAXOBJECT>>2)+128;
	if(sz>16384)sz=16384;
	__asm{
		push	edi
		mov		ecx,sz
		cld
		xor		eax,eax
		mov		edi,offset CmdDone
		rep		stosd
		pop		edi
	};
};


bool PtInside(int x,int y,int x1,int y1,int xp,int yp){
	if(xp>=x&&xp<=x1&&yp>=y&&yp<=y1)return true;
	else return false;
};
void GetRect(OneObject* ZZ,int* x,int* y,int* Lx,int* Ly);
bool ObjInside(OneObject* OB,int xx,int yy,int xx1,int yy1){
	if(!OB)return false;
	int ox,oy,olx,oly;
	GetRect(OB,&ox,&oy,&olx,&oly);
	int ox1=ox+olx-1;
	int oy1=oy+oly-1;
	if( PtInside(ox,oy,ox1,oy1,xx,yy)||
		PtInside(ox,oy,ox1,oy1,xx,yy1)||
		PtInside(ox,oy,ox1,oy1,xx1,yy)||
		PtInside(ox,oy,ox1,oy1,xx1,yy1)||
		PtInside(xx,yy,xx1,yy1,ox,oy)||
		PtInside(xx,yy,xx1,yy1,ox1,oy)||
		PtInside(xx,yy,xx1,yy1,ox,oy1)||
		PtInside(xx,yy,xx1,yy1,ox1,oy1))
		return true;
	else return false;
};
word GoodSelectNewMonsters(byte NI,int xr,int yr,int xr1,int yr1,word *Collect,word* Ser,bool WRITE,CHOBJ* FN,int NN,int MAX,bool InForm,bool NotInForm);
void CorrectShipsSelection(byte NI){
	int N=NSL[NI];
	word* SU=Selm[NI];
	word* SN=SerN[NI];
	int NB=0;
	int NU=0;
	int NS=0;
	int NIN=0;
	bool Usual=0;
	bool Guards=0;
	for(int i=0;i<N;i++){
		word MID=SU[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SN[i]&&!(OB->Sdoxlo||OB->notSelectable())){
				byte Use=OB->newMons->Usage;
				if(OB->LockedInBuilding!=0xFFFF){
                    NIN++;
				}else{
					if(OB->LockType==1){
						if(Use==FisherID)NU++;
						else NS++;
					}else{
						if(OB->NewBuilding||OB->Wall)NB++;
						else{
							NU++;
							if(OB->Guard)Guards=1;
							else Usual=1;
						}
					}
				}
			}
		}
	}
	int RealN=0;
	if(NU&&(NU>NS)){
		//select only units
		for(int i=0;i<N;i++){
			word MID=SU[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==SN[i]&&!(OB->Sdoxlo||OB->notSelectable())){
					OB->Selected&=~GM(NI);
					byte Use=OB->newMons->Usage;
					if(OB->LockType==1){
						if(Use==FisherID){
							SU[RealN]=OB->Index;
							SN[RealN]=OB->Serial;
							RealN++;
							OB->Selected|=GM(NI);
						};
					}else{
						if(!(OB->NewBuilding||OB->Wall)){
							if(!(Usual&&OB->Guard!=0xFFFF)){
								SU[RealN]=OB->Index;
								SN[RealN]=OB->Serial;
								RealN++;
								OB->Selected|=GM(NI);
							};
						};
					};
				};
			};
		};
		NSL[NI]=RealN;
		addrand(NSL[NI]+1);
	}else{
		if(NS){
			//select ships only
			for(int i=0;i<N;i++){
				word MID=SU[i];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&OB->Serial==SN[i]&&!(OB->Sdoxlo||OB->notSelectable())){
						OB->Selected&=~GM(NI);
						byte Use=OB->newMons->Usage;
						if(OB->LockType==1&&Use!=FisherID){
							SU[RealN]=OB->Index;
							SN[RealN]=OB->Serial;
							RealN++;
							OB->Selected|=GM(NI);
						};
					};
				};
			};
			NSL[NI]=RealN;
			addrand(NSL[NI]+1);
		};
	};
};
void ImCorrectShipsSelection(byte NI){
	int N=ImNSL[NI];
	word* SU=ImSelm[NI];
	word* SN=ImSerN[NI];
	int NB=0;
	int NU=0;
	int NS=0;
	int NIN=0;
	int NEn=0;
	int NMy=0;
	int NIX=GSets.CGame.cgi_NatRefTBL[NI];
	bool Usual=0;
	bool Guards=0;
	for(int i=0;i<N;i++){
		word MID=SU[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SN[i]&&!(OB->Sdoxlo||OB->notSelectable())){
				if(OB->NNUM==NIX)NMy++;else NEn++;
				byte Use=OB->newMons->Usage;
				if(OB->LockedInBuilding!=0xFFFF){
                    NIN++;
				}else{
					if(OB->LockType==1){
						if(Use==FisherID)NU++;
						else NS++;
					}else{
						if(OB->NewBuilding||OB->Wall)NB++;
						else{
							NU++;
							if(OB->Guard!=0xFFFF)Guards=1;
							else Usual=1;
						}
					}
				}
			}
		}
	}
	//if(NU>1)Usual=1;
	int RealN=0;
	if(NU&&(NU>NS)){
		//select only units
		for(int i=0;i<N;i++){
			word MID=SU[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==SN[i]&&!(OB->Sdoxlo||OB->notSelectable())){
					OB->ImSelected&=~GM(NI);
					if(NMy==0 || (NMy>0 && OB->NNUM==NIX)){
						byte Use=OB->newMons->Usage;
						if(OB->LockType==1){
							if(Use==FisherID){
								SU[RealN]=OB->Index;
								SN[RealN]=OB->Serial;
								RealN++;
								OB->ImSelected|=GM(NI);
							}
						}else{
							if(!(OB->NewBuilding||OB->Wall)){
								if(!(Usual&&OB->Guard!=0xFFFF)){
									SU[RealN]=OB->Index;
									SN[RealN]=OB->Serial;
									RealN++;
									OB->ImSelected|=GM(NI);	
								}
							}
						}
					}
				}
			}
		}
		ImNSL[NI]=RealN;
	}else{
		if(NS){
			//select ships only
			for(int i=0;i<N;i++){
				word MID=SU[i];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&OB->Serial==SN[i]&&!(OB->Sdoxlo||OB->notSelectable())){
						OB->ImSelected&=~GM(NI);
						if(NMy==0 || (NMy>0 && OB->NNUM==NIX)){
							byte Use=OB->newMons->Usage;
							if(OB->LockType==1&&Use!=FisherID){
								SU[RealN]=OB->Index;
								SN[RealN]=OB->Serial;
								RealN++;
								OB->ImSelected|=GM(NI);
							}
						}
					}
				}
			}
			ImNSL[NI]=RealN;
		}
	}
}

void SmartSelectionCorrector(byte NI,word* Mon,word* SN,int N);
extern int PREVNSL;
extern int PREVNSL1;
extern bool EditMapMode;
#define CHNSL assert((NSL[1]==PREVNSL1&&NSL[0]==PREVNSL)||PREVNSL==-1);
extern bool InfoMode;

void CreateGoodSelection(byte NI,int xx,int yy,int xx1,int yy1,CHOBJ* FN,int NN,bool Addon){
	if(NI<0||NI>7)return;
	SelCenter[NI]=0;
	int x=xx>>5;
	int y=yy>>5;
	int x1=xx1>>5;
	int y1=yy1>>5;
	if(x>1)x--;
	if(y>1)y--;
	if(x1<msx)x1++;
	if(y1<msy)y1++;
	//if(xx1&31)x1++;
	//if(yy1&31)y1++;
	bool InForm=1;
	bool NotInForm=1;
	bool CheckInForm=0;
	if(NI>=16){
		NI-=16;
		CheckInForm=1;
	};
	word MID;
	OneObject* OB;
	word ns=0;
	word Nsel=ImNSL[NI];
	word* SMon=ImSelm[NI];
	word* ser=ImSerN[NI];
	word* tmpSelm=NULL;
	word* tmpSerN=NULL;
	int tmpN=0;
	if(Nsel){
		tmpSelm=znew(word,Nsel);
		tmpSerN=znew(word,Nsel);
		memcpy(tmpSelm,SMon,Nsel<<1);
		memcpy(tmpSerN,ser,Nsel<<1);
		tmpN=Nsel;
	};
	if(CheckInForm){
		InForm=0;
		NotInForm=0;
		for(int i=0;i<Nsel;i++){
			word MID=SMon[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&(!(OB->Sdoxlo||OB->notSelectable()))&&OB->Serial==ser[i]){
					if(OB->BrigadeID!=0xFFFF)InForm=1;
					else NotInForm=1;
				};
			};
		};
	};

	if(!Addon&&Nsel){
		//CmdFreeSelected(NI);
		for(int i=0;i<Nsel;i++){
			MID=SMon[i];
			if(MID!=0xFFFF){
				OB=Group[MID];
				if(int(OB)/*&&OB->NNUM==GSets.CGame.cgi_NatRefTBL[NI]*/&&ser[i]==OB->Serial)OB->ImSelected&=~GM(NI);
			};
		};
	};

	word nnm=GoodSelectNewMonsters(NI,xx,yy,xx1,yy1,NULL,NULL,false,FN,NN,ULIMIT,InForm,NotInForm);

	if( nnm==0 && Nsel!=0 && (!InfoMode) )
		Addon=true;

	if(!Addon&&Nsel){
		if(int(ImSelm[NI])){
			free(ImSelm[NI]);
			ImSelm[NI]=NULL;
		};
		if(int(ImSerN[NI])){
			free(ImSerN[NI]);
			ImSerN[NI]=NULL;
		};
	}

	int Olds=0;
	if( Addon ) Olds=Nsel;

	if(Olds+ns+nnm){
		ImSelm[NI]=znew(word,Olds+ns+nnm);
		ImSerN[NI]=znew(word,Olds+ns+nnm);
	}else{
		ImNSL[NI]=0;
		SmartSelectionCorrector(NI,tmpSelm,tmpSerN,tmpN);
		if(tmpSelm){
			free(tmpSelm);
			free(tmpSerN);
		};
		return;
	};
	if(Addon&&Nsel){
		memcpy(ImSelm[NI],SMon,Olds<<1);
		memcpy(ImSerN[NI],ser,Olds<<1);
	};
	word* SM=ImSelm[NI];
	word* SR=ImSerN[NI];
	word ns1=ns;
	ns=Olds;
	if(nnm) GoodSelectNewMonsters(NI,xx,yy,xx1,yy1,&SM[ns],&SR[ns],true,FN,NN,nnm,InForm,NotInForm);
	
	ImNSL[NI]=ns+nnm;

	if(Addon&&Nsel){
		free(SMon);
		free(ser);
	};
	ImCorrectShipsSelection(NI);
	ImCorrectBrigadesSelection(NI);
	SmartSelectionCorrector(NI,tmpSelm,tmpSerN,tmpN);
	//ImCorrectBrigadesSelection(NI);
	if(tmpSelm){
		free(tmpSelm);
		free(tmpSerN);
	};
};
word* TempIDS=NULL;
word* TempSNS=NULL;
int MaxIDS=0;
int TempNU=0;
void PrepareToImSelection(byte NI){
	TempNU=ImNSL[NI];
	if(TempNU>MaxIDS){
		MaxIDS=TempNU;
		if(TempIDS)delete[](TempIDS);
		if(TempSNS)delete[](TempSNS);
        TempIDS=new word[MaxIDS];
		TempSNS=new word[MaxIDS];
	}
	if(ImNSL[NI]){
		memcpy(TempIDS,ImSelm[NI],TempNU*2);
		memcpy(TempSNS,ImSerN[NI],TempNU*2);
	}	
}
void FinalizeImSelection(byte NI){
	SmartSelectionCorrector(NI,TempIDS,TempSNS,TempNU);
}
OneObject* SearchGate(int x,int y){
	return NULL;
};
void RefreshSelected(byte NI){
	int RNSel=0;
	word* SMon=Selm[NI];
	word* ser=SerN[NI];
	if(!SMon)NSL[NI]=0;
	word Nsel=NSL[NI];
	addrand(NSL[NI]+1);
	word MID;
	for(int k=0;k<Nsel;k++){
		MID=SMon[k];
		addrand(MID);
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			addrand(k);
			if(OB&&OB->Serial==ser[k]&&!(OB->Sdoxlo||OB->notSelectable())){
				addname(OB->Index);
				addrand(k);
				bool Good=true;
				if(OB->InArmy&&OB->BrIndex<NBPERSONAL&&OB->BrigadeID!=0xFFFF){
					OB->Selected&=~GM(NI);
					Good=false;
					addrand(k+1);
				};
				if(Good){
					SMon[RNSel]=SMon[k];
					ser[RNSel]=ser[k];
					RNSel++;
					addrand(RNSel+1);
				};
			};
		};
	};
	NSL[NI]=RNSel;
	addrand(NSL[NI]+1);
	//----------------------------IM-------------------------//
	RNSel=0;
	Nsel=ImNSL[NI];
	SMon=ImSelm[NI];
	ser= ImSerN[NI];
	for(k=0;k<Nsel;k++){
		MID=SMon[k];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==ser[k]){
				if(OB->notSelectable()||OB->Invisible)OB->ImSelected=0;
				if(!(OB->Sdoxlo||OB->notSelectable())){
					bool Good=true;
					if(OB->InArmy&&OB->BrIndex<NBPERSONAL&&OB->BrigadeID!=0xFFFF){
						OB->ImSelected&=~GM(NI);
						Good=false;
					};
					if(Good){
						SMon[RNSel]=SMon[k];
						ser[RNSel]=ser[k];
						RNSel++;
					}
				}
			};
		};
	};
	ImNSL[NI]=RNSel;
};
bool OneDirection=0;
#ifdef COSSACKS2
void SetSpeedToObj(word MID,int Speed,byte AttType){
	if(MID!=0xFFFF){
        OneObject* OB=Group[MID];
		if(OB){
			OB->UnitSpeed=64;
			if(AttType&&OB->newMons->Rate[0]!=16){
                OB->GroupSpeed=Speed*16/OB->newMons->Rate[0];
			}else{
				OB->GroupSpeed=Speed;
			}
		}
	}
}
void EqualizeOfficersSpeed(byte NI,byte AttType);
void EqualizeGroupSpeed(byte NI,byte AttType){
	EqualizeOfficersSpeed(NI,AttType);
	int N=NSL[NI];
	word* IDS=Selm[NI];
	word* SNS=SerN[NI];
	int MINS=255;
	for(int i=0;i<N;i++){
		word MID=IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]){
				OB->GroupSpeed=OB->newMons->MotionDist;
				int rs=OB->newMons->BRandomSpeed;
				if(rs){
					OB->GroupSpeed=OB->GroupSpeed*(100+(rando()%(rs*2))-rs)/100;
				}
			}
		}
	}
	return;
    /*
	int N=NSL[NI];
	word* IDS=Selm[NI];
	word* SNS=SerN[NI];
	int MINS=255;
	for(int i=0;i<N;i++){
		word MID=IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]){
				int V=OB->newMons->MotionDist;//(OB->newMons->MotionDist*OB->MoreCharacter->Speed)/100;
				if(AttType&&OB->newMons->Rate[0]!=16){
					V=(V*OB->newMons->Rate[0])>>4;
				}
				if(V<MINS)MINS=V;
			}
		}
	}
	if(MINS<4)MINS=4;
	for(i=0;i<N;i++){
		word MID=IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]){
				if(AttType&&OB->newMons->Rate[0]!=16){
                    OB->GroupSpeed=MINS*16/OB->newMons->Rate[0];
				}else{
					OB->GroupSpeed=MINS;
				}
				if(OB->BrigadeID!=0xFFFF){
					Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
					if(BR->Enabled){
						SetSpeedToObj(BR->Memb[0],MINS,AttType);
						SetSpeedToObj(BR->Memb[1],MINS,AttType);
						SetSpeedToObj(BR->Memb[2],MINS,AttType);
					}
				}
			}
		}
	}
	*/
}
void EqualizeOfficersSpeed(byte NI,byte AttType){
	int N=NSL[NI];
	word* IDS=Selm[NI];
	word* SNS=SerN[NI];
	int MINS=255;
	for(int i=0;i<N;i++){
		word MID=IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]){
				if(OB->BrigadeID!=0xFFFF&&OB->BrIndex<NBPERSONAL){
					Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
                    OneObject* ONEU=NULL;
					for(int j=NBPERSONAL;j<BR->NMemb;j++){
						word ID=BR->Memb[j];
						if(ID!=0xFFFF){
							OneObject* U=Group[ID];
							if(U&&U->Serial==BR->MembSN[j]){
								ONEU=U;
								break;
							}
						}
					}
					if(ONEU){
						int V=ONEU->newMons->MotionDist;//(OB->newMons->MotionDist*OB->MoreCharacter->Speed)/100;
						if(AttType&&OB->newMons->Rate[0]!=16){
							V=(V*ONEU->newMons->Rate[0])>>4;
						}
						OB->GroupSpeed=V;
					}
				}
			}
		}
	}
}
#endif
void DiscardUnitsOfOtherPlayer(byte NI){
	byte TrueNI=GSets.CGame.cgi_NatRefTBL[NI];
    int N=NSL[NI];
	for(int i=0;i<N;i++){
		word MID=Selm[NI][i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->NNUM!=TrueNI){
				SerN[NI][i]++;
			}
		}
	}
}
void DiscardBackUnitsOfOtherPlayer(byte NI){
	byte TrueNI=GSets.CGame.cgi_NatRefTBL[NI];
    int N=NSL[NI];
	for(int i=0;i<N;i++){
		word MID=Selm[NI][i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->NNUM!=TrueNI){
				SerN[NI][i]++;
			}
		}
	}
}
void SendSelectedToXY(byte NI,int xx,int yy,short Dir,byte Prio,byte Type){
	addrand(NI);
	addrand(xx);
	addrand(yy);
	addrand(Dir);
	addrand(Prio);
	addrand(Type);
	DiscardUnitsOfOtherPlayer(NI);
	OneDirection=Type&16;
	LastDirection=Dir;
	bool InAttackState=false;
	int NAttackingMeleeNow=0;
	int totunits=0;
	bool FarSent=false;
	REALTIME++;
	for(int i=0;i<NSL[NI];i++){
		word ID=Selm[NI][i];
		if(ID!=0xFFFF){
			OneObject* OB=Group[ID];
			if(OB&&OB->Serial==SerN[NI][i]){
				if(OB->BrigadeID!=0xFFFF){
					totunits++;
					if(OB->GroundState || OB->NewState==2)InAttackState=true;
					if(OB->Attack){
						word ID=OB->EnemyID;
						if(ID!=0xFFFF){
							OneObject* EOB=Group[ID];
							if(EOB){
								if(Norma(EOB->RealX-OB->RealX,EOB->RealY-OB->RealY)<150*16){
									NAttackingMeleeNow++;
								}
							}
						}
					}
					Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
					int xb=0;
					int yb=0;
					byte D=0;
					BR->GetCenter(&xb,&yb);
					int RR=OB->newMons->RetreatRadius;
					if(RR==0)RR=500;//GECU: default distance to go in defensive mode
					if(Norma(xb-xx/16,yb-yy/16)>RR){
						FarSent=true;
					}
				}
				if( (!CmdDone[ID]) && OB->Attack && OB->ActivityState==2 )OB->ClearOrders();
			}
		}
	}
	if((NAttackingMeleeNow>totunits/10)||FarSent)InAttackState=false;//GECU - limit of attacked melee units to escate - totunits/10 - 10% required to escape without attack
	GroupSendSelectedTo(NI,xx,yy,InAttackState&&!EngSettings.AutoChangeFormationType?128:Prio,Type&7);
	LastDirection=512;
#ifdef COSSACKS2
	EqualizeGroupSpeed(NI,Prio==128?1:0);
#endif
	DiscardBackUnitsOfOtherPlayer(NI);
	ClearEnemyForSelected(NI);
    return;
};
void SendSelectedToXY(byte NI,int xx,int yy,short Dir,byte Type){
	SendSelectedToXY(NI,xx,yy,Dir,16+128,Type);
}
extern int exFMode;
void ChGSpeed(){
	if(FrmDec==1){
		FrmDec=2;
		SpeedSh=1;
	}else{
		FrmDec=1;
		SpeedSh=0;
	};
	exFMode=SpeedSh;
};
extern bool AutoSpeed;
void SetGSpeed(word Speed){
	GameSpeed=Speed;
};
void AttackToXY(byte NI,byte x,byte y){
	DiscardUnitsOfOtherPlayer(NI);
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	if(!Nsel)return;
	word MID;
	int Sqs=int(sqrt(Nsel));
	int Glx=Sqs;
	int Gly=Sqs;
	if(Glx*Gly>Nsel)Glx--;
	if(Glx*Gly<Nsel)Glx++;else{
		if(Glx*Gly>Nsel)Gly--;
		if(Glx*Gly<Nsel)Gly++;
	};
	int gx1=x-(Glx>>1);
	int gy1=y-(Gly>>1);
	if(gx1<=0)gx1=1;
	if(gy1<=0)gy1=1;
	if(gx1+Glx>msx)gx1=msx-Glx+1;
	if(gy1+Gly>msy)gy1=msy-Gly+1;
	int zx=gx1;
	int zy=gy1;
	for(int i=0;i<Nsel;i++){
		if(zx-gx1+1>Glx){zx=gx1;zy++;};
		MID=SMon[i];
		if(!(CmdDone[MID]||MID==0xFFFF)&&int(Group[MID]))
			//Group[SMon[i]]->SendTo(zx,zy,129);
		zx++;
	};
	DiscardBackUnitsOfOtherPlayer(NI);
};
void GroupAttackSelectedBrigadesTo(byte NI,int x,int y,byte Prio,byte OrdType);
void SetAttState(Brigade* BR,bool Val);
void SetStandState(Brigade* BR,byte State);
void CancelStandGround(Brigade* BR);
void B_BrigadeBattleLink(Brigade* BR);
void SetEnemyForBrigade(Brigade* BR,word EnemyBR, byte NI){
	word* IDS=BR->Memb;
	word* SNS=BR->MembSN;
	int N=BR->NMemb;
	int Old = -1;
	BR->OnlyThisBrigadeToKill=EnemyBR;
	BR->NIOnlyThisBrigadeToKill=NI;
	for(int p=0;p<N;p++){
		word MID=IDS[p];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[p])
			{
				OB->SearchOnlyThisBrigadeToKill=EnemyBR;
				if(OB->EnemyID!=0xFFFF)
				{
					OneObject* OBt=Group[OB->EnemyID];
					if(OBt)
					{
						Old=OBt->BrigadeID;
					}
				}
			}
		}
	}
	//if(Old!=EnemyBR&&BR->BOrder&&BR->BOrder->BLink==&B_BrigadeBattleLink)
	if(Old!=EnemyBR&&BR->NewBOrder&&BR->NewBOrder->GetBrigadeOrderID()==BRIGADEORDER_BITVA)
	{
		BR->DeleteBOrder();
	}
}
void ClearEnemyForSelected(byte NI){
	int N=NSL[NI];
	word* IDS=Selm[NI];
	word* SNS=SerN[NI];
	for(int p=0;p<N;p++){
		word MID=IDS[p];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[p]&&!CmdDone[MID])
			{
				OB->SearchOnlyThisBrigadeToKill=0xFFFF;
				if(OB->BrigadeID!=0xFFFF)
				{
					Brigade* BR = &(CITY[OB->NNUM].Brigs[OB->BrigadeID]);
					if(BR&&BR->WarType)
					{
						BR->OnlyThisBrigadeToKill=0xFFFF;
					}
				}
			}
		}
	}
}
OneObject* Brigade::ThrowOneGrenade(OneObject* EOB){
	if(!NGrenades)return NULL;
	//searching for appropriate grenadier
	int vx=EOB->RealX>>4;
	int vy=EOB->RealY>>4;
	int RMIN=10000;
	word UID=0xFFFF;
	word BIDX=0;
	byte NI=CT->NI;
	int R0=NATIONS[NI].Mon[MembID]->MoreCharacter->AttackRadius2[2]-32+EOB->newMons->AddShotRadius;
	int R0A=NATIONS[NI].Mon[MembID]->MoreCharacter->AttackRadius2[2]-32;
	int R00=NATIONS[NI].Mon[MembID]->MoreCharacter->AttackRadius1[2];
	for(int j=NBPERSONAL;j<NMemb;j++){
		word ID=Memb[j];
		if(ID!=0xFFFF){
			OneObject* OB=Group[ID];
			if(OB&&CheckIfPossibleToBreakOrder(OB)&&OB->NewState!=4){
				int R=Norma((OB->RealX>>4)-vx,(OB->RealY>>4)-vy);
				if(R<RMIN&&R>R00/*&&R<R0*/){					
					RMIN=R;
					UID=ID;
					BIDX=j;
				};
			};
		};
	};
	if(UID!=0xFFFF){
		OneObject* OB=Group[UID];
		//OB->StandGround=0;
		if(OB->NewAttackPoint(vx,vy,128+16,1,1)){
			NGrenades--;			
		};		
	};						
	return UID!=0xFFFF?Group[UID]:NULL;	
}
void ShiftDestPoint(int x0,int y0,int& xd,int& yd,int dr){
	int dx=xd-x0;
	int dy=yd-y0;
	int N=Norma(dx,dy);
	if(N>0){
        xd+=dx*dr/N;
		yd+=dy*dr/N;
	}
}
bool TestIfLineFormation(Brigade* BR){
	if(BR->WarType){
        OrderDescription* ODS=ElementaryOrders+BR->WarType-1;
		return ODS->SymInv && !ODS->Sym4i;
	}
	return false;
}
bool GetObjInsubordination(OneObject* OB);
void AttackSelected(byte NI,word ObjID,byte OrdType,short DIR){
	ClearEnemyForSelected(NI);
	DiscardUnitsOfOtherPlayer(NI);
	LastDirection=DIR;
	OneObject* EOB=Group[ObjID];
	if(!EOB)return;	
	if(OrdType==0){
		int maxR=0;
		int xe=EOB->RealX;
		int ye=EOB->RealY;
		word Nsel=NSL[NI];
		word* SMon=Selm[NI];
		word* SNS=SerN[NI];
		for(int i=0;i<Nsel;i++){
			word MID=SMon[i];
			if(MID!=0xFFFF&&Group[MID]){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==SNS[i]){
					int r=Norma(OB->RealX-xe,OB->RealY-ye)>>4;
					if(r>maxR){
						maxR=r;
					}
				}
			}
		}
		if(maxR>1000){//for GEC - distance of changing attack style
			OrdType=1;
			SendSelectedToXY(NI,EOB->RealX,EOB->RealY,DIR,128+1,0);
		}
	}
	//if(OB&&!OB->Sdoxlo)GroupAttackSelectedBrigadesTo(NI,OB->RealX,OB->RealY,128+1,0);
	int RNSel=0;
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	word* SNS=SerN[NI];
	if(!Nsel)return;
	word MID;
	int NGR=0;
	REALTIME++;
	for(int i=0;i<Nsel;i++){
		MID=SMon[i];
		if(MID!=0xFFFF&&int(Group[MID])){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]){				
				if(GetObjInsubordination(OB)){
					continue;
				}
				if(EngSettings.AutoChangeFormationType&&!EngSettings.DontUseAgressiveState){
					OB->ActivityState=2;
				}
				if(OB->BrigadeID!=0xFFFF&&EOB->NewBuilding&&NGR==0){
					Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
					if(BR->NGrenades){
						OneObject* GR=BR->ThrowOneGrenade(EOB);		
						if(GR){							
							for(int j=0;j<BR->NMemb;j++){
								word ID=BR->Memb[j];
								if(ID!=0xFFFF){
									OneObject* OB=Group[ID];
									if(OB){
                                        CmdDone[ID]=1;
									}
								}
							}
							NGR++;
						}						
                        
					}
				}
				if(!CmdDone[MID]){
					if(OB->BrigadeID!=0xFFFF&&!(EOB->NewBuilding||EOB->newMons->CanBeInFocusOfFormation)){
						Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
						CmdDone[MID]=1;
						OneObject* EOB=Group[ObjID];
						if(EOB&&BR->LastOrderTime!=REALTIME){
							if(EOB->BrigadeID!=0xFFFF)SetEnemyForBrigade(BR,EOB->BrigadeID,EOB->NNUM);
							if(!(BR->NewBOrder&&BR->NewBOrder->GetBrigadeOrderID()==BRIGADEORDER_RIFLEATTACK)){
								//BR->HumanGlobalSendTo(EOB->RealX>>4,EOB->RealY>>4,BR->Direction,128,0);								
								int xc=0;
								int yc=0;
								byte dir=0;
								if(BR->GetCenter(&xc,&yc,&dir)){
									int xd=EOB->RealX>>4;
									int yd=EOB->RealY>>4;
									char DestD=BR->Direction;
									if(EOB->BrigadeID!=0xFFFF){
										Brigade* EBR=CITY[EOB->NNUM].Brigs+EOB->BrigadeID;
										EBR->GetCenter(&xd,&yd,&dir);
										if( TestIfLineFormation(EBR) && TestIfLineFormation(BR) ){
											if(abs(int(char(EBR->Direction-BR->Direction)))<38){
												DestD=EBR->Direction;
											}else
											if(abs(int(char(EBR->Direction-BR->Direction-128)))<38){
												DestD=EBR->Direction+128;
											}
										}
									}
									ShiftDestPoint(xc,yc,xd,yd,120);									
									BR->AttEnm=1;
									BR->HumanLocalSendTo(xd<<4,yd<<4,DestD,128,0);
									BR->LastOrderTime=0;
									SetAttState(BR,0);
									SetStandState(BR,1);
									BR->LastOrderTime=0;
									CancelStandGround(BR);						
									CmdDone[MID]=1;
									BR->AttEnm=1;
									if(EOB->BrigadeID!=0xFFFF)SetEnemyForBrigade(BR,EOB->BrigadeID,EOB->NNUM);
									for(int i=NBPERSONAL;i<BR->NMemb;i++){
										word ID=BR->Memb[i];
										if(ID!=0xFFFF){
											OneObject* OB=Group[ID];
											if(OB&&OB->Serial==BR->MembSN[i]){
												if(OB->newMons->ArmAttack){											
													OB->GroundState=1;
													OB->NewState=OB->GroundState;
												}										
											}
										}
									}
								}
							}
						};
					}else{
						if(OB->AttackObj(ObjID,126+128,OrdType)){
							if(OB->CObjIndex!=0xFFFF){
								void ClearCannonAutoShot(OneObject* OB);
								ClearCannonAutoShot(OB);
							}
							bool doit=true;
							void AttackObjLink(OneObject* OBJ);
							if(OB->LocalOrder&&OB->LocalOrder->DoLink==&AttackObjLink){
								if(ObjID!=0xFFFF){
									OneObject* EOB=Group[ObjID];
									if(EOB&&EOB->NewBuilding){
										OB->LocalOrder->DoLink(OB);
										if(OB->EnemyID==0xFFFF)doit=false;	
									}
								}
							}
							if(doit){
								CmdDone[MID]=1;
								if(!(EOB->newMons->CanBeInFocusOfFormation||EOB->NewBuilding)){
									if(OB->BrigadeID!=0xFFFF){
										Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
										BR->LastOrderTime=REALTIME;
										int N=BR->NMemb;
										for(int i=0;i<N;i++){
											word M=BR->Memb[i];
											if(M!=0xffff){
												OneObject* OB=Group[M];
												if(OB&&OB->Serial==BR->MembSN[i]){
													CmdDone[M]=1;
												}
											}
										}
									}
								}else{
									if(OB->BrigadeID!=0xFFFF){
										Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
										BR->ClearNewBOrders();						
										BR->AttEnm=true;
										if(OB->newMons->ArmAttack){
											OB->GroundState=1;
											OB->NewState=1;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	LastDirection=512;
	DiscardBackUnitsOfOtherPlayer(NI);
};

void BuildObjLink(OneObject*);
void NewMonsterSendToLink(OneObject* OB);
void NewMonsterSmartSendToLink(OneObject* OBJ);
void BuildWithSelected(byte NI,word ObjID,byte OrdType){
	DiscardUnitsOfOtherPlayer(NI);
    if(ObjID==0xFFFF)return;
	int RNSel=0;
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
    word* Sm=SerN[NI];
	if(!Nsel)return;
    OneObject* OBJ=Group[ObjID];
	if(!OBJ)return;
    Nsel=SortUnitsByR(SMon,Sm,Nsel,OBJ->RealX,OBJ->RealY);
    NSL[NI]=Nsel;
	addrand(NSL[NI]+1);
	word MID;
    bool CanAct=false;
	int ni=OBJ->MoreCharacter->MaxInside;
	if(ni==0)ni=1000;
	for(int i=0;i<Nsel && ni;i++){
		MID=SMon[i];
		if((!(MID==0xFFFF/*||CmdDone[MID]*/))&&int(Group[MID]))
			if(OrdType!=2){
				rando();
				if(Group[MID]->BuildObj(ObjID,128+16,true,OrdType)){
					CanAct=true;
					ni--;
				}
			}else{
				OneObject* OB=Group[MID];
				if(OB){
					bool BUILD=false;
					Order1* OR=OB->LocalOrder;					
					while(OR){
						if(OR->DoLink==&BuildObjLink){
							BUILD=true;
							break;
						}
						OR=OR->NextOrder;
					}
					if(BUILD){
						rando();
						if(Group[MID]->BuildObj(ObjID,128+16,true,2)){
							CanAct=true;
							//ni--;
						}
					}else{
						rando();
						if(NATIONS[NI].AI_Enabled){
							if(Group[MID]->BuildObj(ObjID,128+127,true,OrdType)){
								CanAct=true;
								ni--;
							}
						}else{
							if(Group[MID]->BuildObj(ObjID,128+16,true,OrdType)){
								CanAct=true;
								ni--;
							}
						};
					};
				};
			};
	};
    if(CanAct){
        for(int i=0;i<Nsel;i++){
		    MID=SMon[i];
		    if(MID!=0xFFFF&&Group[MID])CmdDone[MID]=true;
        };
    };
	OneObject* OB=Group[ObjID];
	if(OB)OB->ClearBuildPt();
	DiscardBackUnitsOfOtherPlayer(NI);
};
void CreateTerrainMons(byte NI,byte x,byte y,word Type){
	//NATIONS[NI].CreateTerrainAt(x,y,Type);
	//CreateUnit(&NATIONS[NI],x,y,Type);
	NATIONS[NI].CreateNewMonsterAt(int(x)<<9,int(y)<<9,0,false);
};
void CreateNewTerrMons(byte NI,int x,int y,word Type){
	if(Type&32768){
		if(NATIONS[NI].Mon[Type&8191]->newMons->Building){		
			int xx=x<<4;
			int yy=y<<4;
			int CheckSmartCreationAbility(byte NI,int,int* x2i,int* y2i);
			CheckSmartCreationAbility(NI,Type&8191,&xx,&yy);
			x=xx>>4;
			y=yy>>4;
		}
		int r0[8];
		for(int i=0;i<6;i++){
			r0[i]=XRESRC(NI,i);
			SetXRESRC(NI,i,100000000);
		}
		int ID=NATIONS[NI].CreateNewMonsterAt(int(x)<<4,int(y)<<4,Type&8191,false);
		for(int i=0;i<6;i++){
			SetXRESRC(NI,i,r0[i]);
		}
		if(ID!=-1){
			OneObject* OB=Group[ID];
			if(OB){
				NewMonster* NM=OB->newMons;
				if(NM->Building){
					do{
						OB->NextStage();
					}while(OB->Life<OB->MaxLife);
				};
			};
		};
		return;
	};
	//NATIONS[NI].CreateTerrainAt(x,y,Type);
	//CreateUnit(&NATIONS[NI],x,y,Type);
	NATIONS[NI].CreateNewMonsterAt(int(x)<<4,int(y)<<4,Type,false);
};
int CreateNewTerrMons2(byte NI,int x,int y,word Type){
	int ID=NATIONS[NI].CreateNewMonsterAt(x,y,Type&8191,true);
	if(ID!=-1){
		OneObject* OB=Group[ID];
		if(OB){
			NewMonster* NM=OB->newMons;
			if(NM->Building){
				do{
					OB->NextStage();
				}while(OB->Life<OB->MaxLife);
			};
		};
		return ID;
	};
	return -1;
};
int CreateNewTerrMons3(byte NI,int x,int y,word Type,int ReadyPercent){
	int ID=NATIONS[NI].CreateNewMonsterAt(x,y,Type&8191,true);
	if(ID!=-1){
		OneObject* OB=Group[ID];
		if(OB){
			NewMonster* NM=OB->newMons;
			if(NM->Building){
				int ns=OB->NStages*ReadyPercent/100;
				for(int i=0;i<ns;i++)
					OB->NextStage();				
			};
		};
		return ID;
	};
	return -1;
};
void CreateBuilding(byte NI,int x,int y,word Type,byte OrdType){
	if (NOPAUSE){
	if(pSelected::units(NI)._getamount()){
		SetActiveProducer(pSelected::units(NI)[0].object());
	}
    int j=NATIONS[GSets.CGame.cgi_NatRefTBL[NI]].CreateNewMonsterAt(x,y,Type,false);
	SetActiveProducer(NULL);
    if(j!=-1)BuildWithSelected(NI,j,OrdType);
	//else{
	//	AssignHint1(UNILIMIT,50);
	//};
	}
};
void ProduceObject(byte NI,word Type){
	DiscardUnitsOfOtherPlayer(NI);
	int maxp=100000;
	int kk=-1;
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	for(int k=0;k<Nsel;k++){
		word MID=SMon[k];
		if(!(MID==0xFFFF/*||CmdDone[MID]*/)){
			OneObject* OB=Group[SMon[k]];
			if(int(OB)){
				int pp=OB->CheckAbility(Type&8191);
				if(pp==-1&&OB->Ready){
					if(Type&8192){
						if(OB->Produce(Type&8191,0xFFFE)){
							CmdDone[MID]=true;
						}else pp=-1;
					}else{ 
						if(OB->Produce(Type&8191,0xFFFF)){
							CmdDone[MID]=true;
						}else pp=-1;
					};
					if(pp!=-1)return;
				};
				if(pp>0&&pp<maxp){
					maxp=pp;
					kk=k;
				};
			};
		};
	};
	if(kk!=-1){
		word MID=SMon[kk];
		if(!(MID==0xFFFF/*||CmdDone[MID]*/)){
			OneObject* OB=Group[MID];
			if(OB){
				if(Type&8192)OB->Produce(Type&8191,0xFFFE);
				else OB->Produce(Type&8191,0xFFFF);
				CmdDone[MID]=true;
			};
		};
	};
	DiscardBackUnitsOfOtherPlayer(NI);
};
void TakeUnitFromCash(byte NI,word ID);
void ProduceObjects(byte NI,byte N,word Type){
	DiscardUnitsOfOtherPlayer(NI);
	for(int i=0;i<N;i++){
		TakeUnitFromCash(NI,Type);
		ProduceObject(NI,Type);
	};
	DiscardBackUnitsOfOtherPlayer(NI);
};
void GetUnitCost(byte NI,word NIndex,int* Cost,word Power);
void UnProduce(OneObject* OB,word Type){
	if(!(OB->NewBuilding&&OB->LocalOrder))return;
	Order1* ORR=OB->LocalOrder;
	Order1* POR=NULL;
	if(!ORR)return;
	POR=ORR;
	ORR=ORR->NextOrder;
	bool Stage=0;
	if(!ORR){
		Stage=1;
		ORR=POR;
		POR=NULL;
	};
StartUn:;
	do{
		if(ORR->OrderType==13){
			if(ORR->info.Produce.ObjIndex==Type){
				if(POR){
					POR->NextOrder=ORR->NextOrder;
				}else{
					OB->LocalOrder=ORR->NextOrder;
				};
				word Power=ORR->info.Produce.Power;
				if(OB->newMons->Port){
					if(ORR->info.Produce.Progress){
						MFIELDS[1].BClrBar(OB->WallX-7,OB->WallY-7,15);
					};
				};
				OB->FreeOrdBlock(ORR);
				if(Power!=0xFFFF){
					GeneralObject* GO=OB->Nat->Mon[Type];
					int Cost[8];
					GetUnitCost(OB->NNUM,Type,Cost,Power);
					for(int p=0;p<8;p++){
						AddXRESRC(OB->NNUM,p,Cost[p]);
					};
				};
				return;
			};
		};
		POR=ORR;
		ORR=ORR->NextOrder;
		if(Stage)return;
	}while(ORR);
	if(!Stage){
		ORR=OB->LocalOrder;
		POR=NULL;
		Stage=1;
		goto StartUn;
	};
};
void UnProduceObject(byte NI,word Type){
	DiscardUnitsOfOtherPlayer(NI);
	int maxp=0;
	int kk=-1;
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	for(int k=0;k<Nsel;k++){
		word MID=SMon[k];
		if(!(MID==0xFFFF||CmdDone[MID])){
			OneObject* OB=Group[SMon[k]];
			if(int(OB)){
				int pp=OB->CheckAbility(Type);
				if(pp>0&&pp>maxp){
					maxp=pp;
					kk=k;
				};
			};
		};
	};
	if(kk!=-1){
		word MID=SMon[kk];
		if(!(MID==0xFFFF||CmdDone[MID])){
			OneObject* OB=Group[MID];
			if(OB){
				UnProduce(OB,Type);
				//CmdDone[MID]=true;
			};
		};
	};
	DiscardBackUnitsOfOtherPlayer(NI);
};
void MemSelection(byte NI,byte Index){
	SelSet[NI*10+Index].CreateFromSelection(NI);	
};
void RememSelection(byte NI,byte Index){
	SelSet[(NI&31)*10+Index].SelectMembers(NI&31,NI>>5);	
	CorrectBrigadesSelection(NI);	
};
int CreateWall(byte NI,byte* lp){
	return ((word*)lp)[0];
};
void DamageWall(byte NI,word ci){
    
};


void PerformUpgr(byte NI,word UI){
	DiscardUnitsOfOtherPlayer(NI);
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	if(!Nsel)return;
	word MID;
	for(int i=0;i<Nsel;i++){
		MID=SMon[i];
		if(MID!=0xFFFF&&int(Group[MID]))
			Group[SMon[i]]->PerformUpgrade(UI,MID);
	};
	DiscardBackUnitsOfOtherPlayer(NI);
};

void SetDestination(byte NI,int x,int y){
	DiscardUnitsOfOtherPlayer(NI);
	int RNSel=0;
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	if(!Nsel)return;
	word MID;
	for(int i=0;i<Nsel;i++){
		MID=SMon[i];
		OneObject* OB=Group[MID];
		if( MID!=0xFFFF && OB && OB->newMons->ConcPtX.GetAmount() )
			Group[MID]->SetDstPoint(x,y);
	};
	DiscardBackUnitsOfOtherPlayer(NI);
};
void Stopp(byte NI){
	int RNSel=0;
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	if(!Nsel)return;
	word MID;
	for(int i=0;i<Nsel;i++){
		MID=SMon[i];
		if(MID!=0xFFFF&&int(Group[MID])){
			OneObject* OB=Group[MID];
			if(OB){
				OB->ClearOrders();
			};
		};
	};
};
void StandGround(byte NI){
	DiscardUnitsOfOtherPlayer(NI);
	int RNSel=0;
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	if(!Nsel)return;
	word MID;
	for(int i=0;i<Nsel;i++){
		MID=SMon[i];
		if(MID!=0xFFFF&&int(Group[MID])){
			OneObject* OB=Group[MID];
			OB->StandGround=true;
			// by vital
			OB->Guard=0xFFFF;
			//OB->ClearOrders();
			OB->DestX=-1;
		};
	};
	DiscardBackUnitsOfOtherPlayer(NI);
};
void GroupPatrolSelected(byte NI,int x,int y,byte Prio);
void PatrolGroup(byte NI,int x1,int y1,byte Dir){
	DiscardUnitsOfOtherPlayer(NI);
	LastDirection=Dir;
    GroupPatrolSelected(NI,x1,y1,0);
	LastDirection=512;
	DiscardBackUnitsOfOtherPlayer(NI);
};
void GroupAttackPoint(byte NI,byte x,byte y,byte kind){
	DiscardUnitsOfOtherPlayer(NI);
	int RNSel=0;
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	if(!Nsel)return;
	word MID;
	for(int i=0;i<Nsel;i++){
		MID=SMon[i];
		if(MID!=0xFFFF&&int(Group[MID])){
			OneObject* OB=Group[MID];
			if(OB)OB->AttackPoint(x,y,kind,16);
		};
	};
	DiscardBackUnitsOfOtherPlayer(NI);
};
void SendToTransp(byte NI,word ID){
	DiscardUnitsOfOtherPlayer(NI);
	if(Group[ID])CreateTransProcess(Group[ID]);
	DiscardBackUnitsOfOtherPlayer(NI);
};
void UnloadTransp(byte NI,byte x1,byte y1){
	DiscardUnitsOfOtherPlayer(NI);
	int RNSel=0;
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	if(!Nsel)return;
	word MID;
	for(int i=0;i<Nsel;i++){
		MID=SMon[i];
		if(MID!=0xFFFF&&int(Group[MID])){
			OneObject* OB=Group[MID];
			//if(OB)OB->UnloadPassangers(x1,y1);
		};
	};
	DiscardBackUnitsOfOtherPlayer(NI);
};
void DestructBuilding(OneObject* OB);
void DieSelected(byte NI){
	DiscardUnitsOfOtherPlayer(NI);
	int RNSel=0;
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	word* SNS=SerN[NI];
	if(!Nsel)return;
	word MID;
	for(int i=0;i<Nsel;i++){
		MID=SMon[i];
		if(MID!=0xFFFF&&int(Group[MID])){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]){
				
				// Hero è Control
				//if(DetectHero(OB))
				//	continue;
				//if( OB->ActiveAbility && OB->ActiveAbility->BornNNUM!=OB->NNUM )
				//	continue;
				/*if (!((EditMapMode)||(vGameMode==gmSin)||(vGameMode==gmCamp))){
					if(!OB->NewBuilding)
					continue;
					}
				else{*/
				if(DetectHero(OB))
					continue;
				if( OB->ActiveAbility && OB->ActiveAbility->BornNNUM!=OB->NNUM )
					continue;
				}
				extern bool EditMapMode;
				//if( EditMapMode || ( (OB->NewBuilding||NPlayers<=1) && !OB->newMons->Immortal )){
				if( EditMapMode || !OB->newMons->Immortal ){
					DestructBuilding(OB);
				}
			//}
		}
	}
	DiscardBackUnitsOfOtherPlayer(NI);
};
void ComKartech(byte NI){
	DiscardUnitsOfOtherPlayer(NI);
	int RNSel=0;
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	if(!Nsel)return;
	word MID;
	for(int i=0;i<Nsel;i++){
		MID=SMon[i];
		if(MID!=0xFFFF&&int(Group[MID])){
			OneObject* OB=Group[MID];
			if(OB&&OB->newMons->WSECT){
				OB->ClearOrders();
				OB->NewAttackPoint((OB->RealX>>4)+TCos[OB->RealDir],(OB->RealY>>4)+TSin[OB->RealDir],128+16,0,1);
				//OB->GroundState=OB->newMons->WSECT->AttIndex+1;
			};
		};
	};
	DiscardBackUnitsOfOtherPlayer(NI);
};
void ComPreKartech(byte NI){
	DiscardUnitsOfOtherPlayer(NI);
	int RNSel=0;
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	if(!Nsel)return;
	word MID;
	for(int i=0;i<Nsel;i++){
		MID=SMon[i];
		if(MID!=0xFFFF&&int(Group[MID])){
			OneObject* OB=Group[MID];
			if(OB&&OB->newMons->WSECT){
				OB->NewState=OB->newMons->WSECT->AttIndex+1;
				//OB->GroundState=OB->newMons->WSECT->AttIndex+1;
			};
		};
	};
	DiscardBackUnitsOfOtherPlayer(NI);
};

void EliminateBuilding(OneObject* OB);
void DeleteAbilities(OneObject* OB);

void EraseObjectWithoutAbilities(OneObject* OB){
	if(OB){
		units_iterator::ActiveAbilities iAA;
		iAA.Create(OB);
		while(ActiveUnitAbility* AA=iAA.Next()){
			AA->ItemDrop=0;
		}
		int MID=OB->Index;
		//OB->Hidden=true;
		OB->delay=6000;
		if(OB->LockType==1)OB->RealDir=32;
		if(OB->BrigadeID!=0xFFFF){
			Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
			BR->vLastEnemyID=0xFFFE;
		}
		OB->Die();
		//OB=Group[MID];
		if(OB){
			OB->Sdoxlo=6000;
			if(OB->NewBuilding){
				EliminateBuilding(OB);
				//delete(Group[MID]);
				//Group[MID]=NULL;
			};
		};
	};
}
void EraseObject(OneObject* OB){
	if(OB){
		DeleteAbilities(OB);
		EraseObjectWithoutAbilities(OB);
	}
}
void EraseSelected(byte NI){
	int RNSel=0;
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	if(!Nsel)return;
	word MID;
	for(int i=0;i<Nsel;i++){
		MID=SMon[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			EraseObject(OB);
		};
	};
};
void ContinueAttPoint(byte NI,byte x,byte y){
	int RNSel=0;
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	if(!Nsel)return;
	word MID;
	for(int i=0;i<Nsel;i++){
		MID=SMon[i];
		if(MID!=0xFFFF&&int(Group[MID]))
			Group[MID]->ContinueAttackPoint(x,y,16);
	};
};
void ContinueAttWall(byte NI,byte x,byte y){
	int RNSel=0;
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	if(!Nsel)return;
	word MID;
	for(int i=0;i<Nsel;i++){
		MID=SMon[i];
		if(MID!=0xFFFF&&int(Group[MID]))
			Group[MID]->ContinueAttackWall(x,y,16);
	};
};
void SitDown(byte NI){
	int RNSel=0;
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	if(!Nsel)return;
	word MID;
	for(int i=0;i<Nsel;i++){
		MID=SMon[i];
		if(MID!=0xFFFF&&int(Group[MID])){
			OneObject* OB=Group[MID];
			//if(OB)OB->MakeMeSit();
		};
	};
};
void UseNuclo(byte NI,byte x,byte y){
	int RNSel=0;
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	if(!Nsel)return;
	word MID;
	for(int i=0;i<Nsel;i++){
		MID=SMon[i];
		//if(MID!=0xFFFF&&int(Group[MID]))
			//Group[MID]->NuclearAttack(x,y);
	};
};
void SetRepairState(byte NI,byte state){
	
};
//*******************************************************************************//
//****************                                                 **************//
//*******                          SAVING IPX GAME                         ******//
//****************                                                 **************//
//*******************************************************************************//
SFLB_DLLEXPORT SFLB_LoadGame(char* fnm,bool LoadNation);
void SaveGame(char* fnm,char* Messtr,int ID);
#define MaxSFNames 128
extern int sfVersion;
static int   NSFNames;
char* SFNames[MaxSFNames];
char* PLNames[MaxSFNames];
int   PLID[MaxSFNames];
//1. Creating list of save files
void ClearSFNames(){
	for(int i=0;i<NSFNames;i++){
		free(SFNames[i]);
		free(PLNames[i]);
	};
	NSFNames=0;
};
void InstallSFName(WIN32_FIND_DATA* FD){
	ResFile ff1=RReset(FD->cFileName);
	char nam[128];
	if(ff1!=INVALID_HANDLE_VALUE){
		int sig,lap;
		RBlockRead(ff1,&sig,4);
		if(sig=='WSF2'){
			RBlockRead(ff1,&sig,4);
			RBlockRead(ff1,&lap,4);
			if(sig==sfVersion&&NSFNames<MaxSFNames){
				int nlen=0;
				RBlockRead(ff1,&nlen,2);
				nlen=0;
				RBlockRead(ff1,&nlen,1);
				RBlockRead(ff1,nam,nlen);
				RClose(ff1);
				PLNames[NSFNames]=znew(char,strlen(nam)+1);
				SFNames[NSFNames]=znew(char,strlen(FD->cFileName)+1);
				PLID[NSFNames]=lap;
				strcpy(PLNames[NSFNames],nam);
				strcpy(SFNames[NSFNames],FD->cFileName);
				NSFNames++;
			}else RClose(ff1);
		}else RClose(ff1);
	};
};
void CreateSFList(){
	NSFNames=0;
	WIN32_FIND_DATA FD;
	HANDLE HF=FindFirstFile("*.sav",&FD);
	if(HF!=INVALID_HANDLE_VALUE){
		InstallSFName(&FD);
		while(FindNextFile(HF,&FD))InstallSFName(&FD);
	};
};
//2. Search for name of the game,returns -1 if name not found
int FindPLName(char* Name){
	for(int i=0;i<NSFNames;i++){
		if(!strcmp(PLNames[i],Name))return i;
	};
	return -1;
};
//2. Search for name of the save file,returns -1 if name not found
int FindSFName(char* SName){
	for(int i=0;i<NSFNames;i++){
		if(!strcmp(SFNames[i],SName))return i;
	};
	return -1;
};
//Searching for the best name of the save file with the given name
void FindBestSFName(char* SName,char* Name){
	int i=FindPLName(Name);
	if(i==-1){
		int j=0;
		char fname[128]="";
		do{
			sprintf(fname,"save%d.sav",j);
			i=FindSFName(fname);
			if(i==-1){
				strcpy(SName,fname);
				return;
			};
			j++;
		}while(true);
	}else{
		strcpy(SName,SFNames[i]);
		return;
	};
};
void ShowCentralText0(char* sss);
extern bool HideBorderMode;
void SaveNetworkGame(byte NI,int ID,char* Name){
	if(PlayGameMode||HideBorderMode)return;
	CreateDirectory("Save",0);
	//ShowCentralText0(GetTextByID("Autosaving"));
	FlipPages();
	//CreateSFList();
	//char fname[128];
	//FindBestSFName(fname,Name);
	char str[128];
	strcpy(str,Name);
	if(!strstr(str,".sav"))strcat(str,".sav");
	char cc1[128];
	strcpy(cc1,str);
	cc1[12]=0;
	if(!strcmp(cc1,"NetAutoSave ")){
		int N=0;
		for(int i=0;i<NPlayers;i++){
			if(EBufs[i].Enabled)N++;
		};
		cc1[12]='0'+N;
		strcpy(str,cc1);
	};
	strcpy(LASTSAVEFILE,str);
	char CC3[256];
	if(_strnicoll(str,"Save\\",5)!=0) sprintf(CC3,"Save\\%s",str);
		else strcpy(CC3,str);
	//if(strstr(str,"Save\\"))strcat(CC3,str);
	//	else sprintf(CC3,"Save\\%s",str);
	SaveGame(CC3,CC3,ID);
	//ClearSFNames();
};
int FindNetGame(int ID,char* name){
	for(int i=0;i<NSFNames;i++){
		if(PLID[i]==ID&&!strcmp(name,PLNames[i]))return i;
	};
	return -1;
};
void IAmLeft();
extern bool NoWinner;
extern char SaveFileName[128];
extern EXBUFFER EBufs1[MaxPL];
extern word PrevRpos;
extern DWORD RealTime;
int GetRealTime();
bool ProcessMessages();
extern int PitchTicks;
extern int PREVGLOBALTIME;
extern DWORD EBPos1;

extern char PL_Names[8][32];
extern byte PL_Colors[8];
extern int PL_NPlayers;
extern byte PL_NatRefTBL[8];

void LoadNetworkGame(byte NI,int ID,char* NAME){
	//CreateSFList();
	//int i=FindNetGame(ID,Name);
	//if(i!=-1){
	//	LoadGame(SFNames[i]);
	//}else{
	//	IAmLeft();
	//};
	//ClearSFNames();
	//DLL0(SFLB_ShowLoading);
	char Name[256];
	sprintf(Name,"%s",NAME);//"Save\\%s",NAME);
	PlayerInfo LOC_PINFO[8];
	memcpy(LOC_PINFO,GSets.CGame.PL_INFO,sizeof LOC_PINFO);

	byte mm[8];
	memcpy(mm,GSets.CGame.cgi_NatRefTBL,8);
	
	PBACK.Clear();
	RETSYS.Clear();

	int T0=GetRealTime();
	do{
		ProcessMessages();
	}while(GetRealTime()-T0<5000);

	PBACK.Clear();
	RETSYS.Clear();
	strcpy(SaveFileName,Name);
	if(MaxPingTime){
		MaxPingTime=700;
		PitchTicks=8;
	};
	PREVGLOBALTIME=0;
	LoadSaveFile();
	NoWinner=0;
	for(int i=0;i<NPlayers;i++){
		EBufs[i].RealTime=0;
		EBufs[i].Size=0;
		EBufs[i].RandIndex=0;
		EBufs1[i].RealTime=0xFFFFFFFF;
		EBufs1[i].Size=0;
		EBufs[i].RandIndex=0;
	};
	EBPos1=0;
	//PrevRPos=0;
	PrevRpos=0;
	RealTime+=64;
	memcpy(GSets.CGame.cgi_NatRefTBL,mm,8);
	if(NPlayers>1){
		PlayerInfo* PI=GSets.CGame.GetCurrentPlayerInfo();
		if(PI){
			char* MyNick=PI->name;
			void NORMNICK1(char* Nick);
			NORMNICK1(MyNick);
			memcpy(GSets.CGame.PL_INFO,LOC_PINFO,sizeof LOC_PINFO);			
			for(int i=0;i<NPlayers;i++){
				char* Name=GSets.CGame.PL_INFO[i].name;
				void NORMNICK1(char* Nick);
				NORMNICK1(Name);			
				for(int j=0;j<PL_NPlayers;j++){
					NORMNICK1(PL_Names[j]);
					if(!strcmp(Name,PL_Names[j])){
						if(!strcmp(MyNick,PL_Names[j]))SetMyNation(PL_Colors[j]);
						GSets.CGame.PL_INFO[i].ColorID=PL_Colors[j];
					};
				}
			};
		}
		for(i=NPlayers;i<PL_NPlayers;i++){
			char* S=PL_Names[i];
			if(!(strcmp(S,"Very hard")&&strcmp(S,"Hard")&&strcmp(S,"Normal")&&strcmp(S,"Easy"))){
				strcpy(GSets.CGame.PL_INFO[i].name,PL_Names[i]);
				GSets.CGame.PL_INFO[i].ColorID=PL_Colors[i];
			}else GSets.CGame.PL_INFO[i].name[0]=0;
		};
		void CenterScreen();
		CenterScreen();
		memcpy(GSets.CGame.cgi_NatRefTBL,PL_NatRefTBL,8);
	};
};
void ChooseUnSelectType(byte NI,word ID){
	int RNSel=0;
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	word* SNM=SerN[NI];
	if(!Nsel)return;
	word MID;
	for(int i=0;i<Nsel;i++){
		MID=SMon[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB){
				if(OB->NIndex!=ID||OB->InArmy){
					SMon[RNSel]=SMon[i];
					SNM[RNSel]=SNM[i];
					RNSel++;
				}else{
					OB->Selected&=~GM(NI);
				};
			};
		};
	};
	NSL[NI]=RNSel;
	addrand(NSL[NI]+1);
};
void ImChooseUnSelectType(byte NI,word ID){
	int RNSel=0;
	word Nsel=ImNSL[NI];
	word* SMon=ImSelm[NI];
	word* SNM=ImSerN[NI];
	if(!Nsel)return;
	word MID;
	for(int i=0;i<Nsel;i++){
		MID=SMon[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB){
				if(OB->NIndex!=ID||OB->InArmy){
					SMon[RNSel]=SMon[i];
					SNM[RNSel]=SNM[i];
					RNSel++;
				}else{
					OB->ImSelected&=~GM(NI);
				};
			};
		};
	};
	ImNSL[NI]=RNSel;
};
void UnSelectBrig(byte NI,word ID){
	int RNSel=0;
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	word* SNM=SerN[NI];
	if(!Nsel)return;
	word MID;
	for(int i=0;i<Nsel;i++){
		MID=SMon[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB){
				if(OB->BrigadeID!=ID){
					SMon[RNSel]=SMon[i];
					SNM[RNSel]=SNM[i];
					RNSel++;
				}else{
					OB->Selected&=~GM(NI);
				};
			};
		};
	};
	NSL[NI]=RNSel;
	addrand(NSL[NI]+1);
};
void ImUnSelectBrig(byte NI,word ID){
	int RNSel=0;
	word Nsel=ImNSL[NI];
	word* SMon=ImSelm[NI];
	word* SNM=ImSerN[NI];
	if(!Nsel)return;
	word MID;
	for(int i=0;i<Nsel;i++){
		MID=SMon[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB){
				if(OB->BrigadeID!=ID){
					SMon[RNSel]=SMon[i];
					SNM[RNSel]=SNM[i];
					RNSel++;
				}else{
					OB->ImSelected&=~GM(NI);
				};
			};
		};
	};
	ImNSL[NI]=RNSel;
};
void ChooseSelectType(byte NI,word ID){
	int RNSel=0;
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	word* SNM=SerN[NI];
	if(!Nsel)return;
	word MID;
	for(int i=0;i<Nsel;i++){
		MID=SMon[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB){
				if(OB->NIndex==ID&&!OB->InArmy){
					SMon[RNSel]=SMon[i];
					SNM[RNSel]=SNM[i];
					RNSel++;
				}else{
					OB->Selected&=~GM(NI);
				};
			};
		};
	};
	NSL[NI]=RNSel;
	addrand(NSL[NI]+1);
	CorrectBrigadesSelection(NI);
};
void ImChooseSelectType(byte NI,word ID){
	int RNSel=0;
	word Nsel=ImNSL[NI];
	word* SMon=ImSelm[NI];
	word* SNM=ImSerN[NI];
	if(!Nsel)return;
	word MID;
	for(int i=0;i<Nsel;i++){
		MID=SMon[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB){
				if(OB->NIndex==ID&&!OB->InArmy){
					SMon[RNSel]=SMon[i];
					SNM[RNSel]=SNM[i];
					RNSel++;
				}else{
					OB->ImSelected&=~GM(NI);
				};
			};
		};
	};
	ImNSL[NI]=RNSel;
	ImCorrectBrigadesSelection(NI);
};
void SelectBrig(byte NI,word ID){
	int RNSel=0;
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	word* SNM=SerN[NI];
	if(!Nsel)return;
	word MID;
	for(int i=0;i<Nsel;i++){
		MID=SMon[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB){
				if(OB->BrigadeID==ID){
					SMon[RNSel]=SMon[i];
					SNM[RNSel]=SNM[i];
					RNSel++;
				}else{
					OB->Selected&=~GM(NI);
				};
			};
		};
	};
	NSL[NI]=RNSel;
	addrand(NSL[NI]+1);
};
void ImSelectBrig(byte NI,word ID){
	int RNSel=0;
	word Nsel=ImNSL[NI];
	word* SMon=ImSelm[NI];
	word* SNM=ImSerN[NI];
	if(!Nsel)return;
	word MID;
	for(int i=0;i<Nsel;i++){
		MID=SMon[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB){
				if(OB->BrigadeID==ID){
					SMon[RNSel]=SMon[i];
					SNM[RNSel]=SNM[i];
					RNSel++;
				}else{
					OB->ImSelected&=~GM(NI);
				};
			};
		};
	};
	ImNSL[NI]=RNSel;
};
void EraseBrigade(Brigade* BR);
void SetAttState(Brigade* BR,bool Val);
void EraseBrigs(byte NI){
	int RNSel=0;
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	Brigade* BR0=NATIONS[GSets.CGame.cgi_NatRefTBL[NI]].CITY->Brigs;
	if(!Nsel)return;
	word MID;
	for(int i=0;i<Nsel;i++){
		MID=SMon[i];
		if(MID!=0xFFFF&&int(Group[MID])){
			OneObject* OB=Group[MID];
			if(OB&&!OB->Sdoxlo&&OB->BrigadeID!=0xFFFF){
				Brigade* BR=BR0+OB->BrigadeID;
				if(BR->Enabled){
					SetAttState(BR,0);
					BR->vLastEnemyID=0xFFFF;
					EraseBrigade(BR);
				};
			};
		};
	};
};
void GoToMineLink(OneObject* OB);
bool CheckWorker(OneObject* OB){
	Order1* OR=OB->LocalOrder;
	while(OR){
		if(OR->DoLink==&GoToMineLink)return false;
		OR=OR->NextOrder;
	};
	return true;
};
void GoToMineWithSelected(byte NI,word ID){
	OneObject* OB=Group[ID];
	if(!OB)return;
	NewMonster* NM=OB->newMons;
	AdvCharacter* ADC=OB->MoreCharacter;
	if(!(NM->UnitAbsorber||NM->PeasantAbsorber))return;
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	word* SNM=SerN[NI];
	if(!Nsel)return;
	int Maxins=ADC->MaxInside+OB->AddInside-OB->NInside;
#ifdef COSSACKS2
	if(NM->PortionLimit)Maxins+=1000;
#endif //COSSACKS2
	int MaxBrInside=Maxins;
	if(!(NATIONS[NI].NMask&OB->NMask)){
		Maxins+=1000;
		MaxBrInside=ADC->MaxInside+OB->AddInside;
	}
    word MID;
    if(Maxins<=0){
        if(OB->NNUM==GSets.CGame.cgi_NatRefTBL[NI]){
            for(int i=0;i<Nsel&&Maxins;i++){
        		MID=SMon[i];
        		if(MID!=0xFFFF){
        			OneObject* OB=Group[MID];
                    if(OB)CmdDone[MID]=true;
                };
            };

		};
        return;
    };
	bool CheckPossibilityToComeInside(OneObject* Transport,OneObject* Passanger);
	for(int i=0;i<Nsel&&Maxins;i++){
		MID=SMon[i];
		if(!(MID==0xFFFF||CmdDone[MID])){
			OneObject* OBJ=Group[MID];
            if(OB&&CheckWorker(OB)&&CheckPossibilityToComeInside(OB,OBJ)&&OB->NNUM==OBJ->NNUM){
				OBJ->GoToMine(ID,16+128);
			}
		}
	}
    if(OB->NNUM==GSets.CGame.cgi_NatRefTBL[NI]){
        for(int i=0;i<Nsel;i++){
   		MID=SMon[i];
    		if(MID!=0xFFFF){
    			OneObject* OB=Group[MID];
                if(OB)CmdDone[MID]=true;
            };
        };
    };
};
void LeaveMineWithSelected(byte NI,word Type){
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	word* SNM=SerN[NI];
	if(!Nsel)return;
	word MID;
	for(int i=0;i<Nsel;i++){
		MID=SMon[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB)OB->LeaveMine(Type);
		};
	};
};
void GoToTransportWithSelected(byte NI,word ID){
	OneObject* OB=Group[ID];
	if(!OB)return;
	NewMonster* NM=OB->newMons;
	AdvCharacter* ADC=OB->MoreCharacter;
	if(!(NM->Transport))return;
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	word* SNM=SerN[NI];
	if(!Nsel)return;
	int Maxins=ADC->MaxInside+OB->AddInside-OB->NInside;
    word MID;
    if(Maxins<=0){
        if(OB->NNUM==GSets.CGame.cgi_NatRefTBL[NI]){
            for(int i=0;i<Nsel&&Maxins;i++){
        		MID=SMon[i];
        		if(MID!=0xFFFF){
        			OneObject* OB=Group[MID];
                    if(OB)CmdDone[MID]=true;
                };
            };
		};
        return;
    };
	for(int i=0;i<Nsel&&Maxins;i++){
		MID=SMon[i];
		if(!(MID==0xFFFF||CmdDone[MID])){
			OneObject* OB=Group[MID];
            if(OB){
                if(OB->GoToTransport(ID,16))Maxins--;
			};

		};
	};
    if(OB->NNUM==GSets.CGame.cgi_NatRefTBL[NI]){
        for(int i=0;i<Nsel;i++){
   		MID=SMon[i];
    		if(MID!=0xFFFF){
    			OneObject* OB=Group[MID];
                if(OB)CmdDone[MID]=true;
            };
        };
    };
};
void TakeRes(byte NI,int x,int y,byte ResID){
	int RNSel=0;
	int DObj=INITBEST;
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	if(!Nsel)return;
	word MID;
	DiscardUnitsOfOtherPlayer(NI);
	for(int i=0;i<Nsel;i++){
		MID=SMon[i];
		if(MID!=0xFFFF&&!CmdDone[MID]){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SerN[NI][i]){
				int DO=OB->TakeResource(x,y,ResID,128+16,0);
				if(DO==DObj && DO!=INITBEST && rando()<4000){
					Sprites[DO].Surrounded=true;
				};
				DObj=DO;
				if(DO!=INITBEST)CmdDone[MID]=true;
			};
		};
	};
	OneSprite* OS=Sprites;
	for(i=0;i<MaxSprt;i++){
		OS->Surrounded=false;
		OS++;
	};
	DiscardBackUnitsOfOtherPlayer(NI);
};
void CreateGatesFromSelected(byte NI){	
};
void SelectAllBuildings(byte NI){
	int N=NSL[NI];
	if(N){
		word* SMN=Selm[NI];
		word* SRN=SerN[NI];
		for(int i=0;i<N;i++){
			word MID=SMN[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==SRN[i])OB->Selected&=~GM(NI);
			};
		};
	};
	if(Selm[NI]){
		free(Selm[NI]);
		Selm[NI]=NULL;
	};
	if(SerN[NI]){
		free(SerN[NI]);
		SerN[NI]=NULL;
	};
	NSL[NI]=0;
	addrand(NSL[NI]+1);
	N=0;
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&OB->NNUM==GSets.CGame.cgi_NatRefTBL[NI]&&OB->NewBuilding)N++;
	};
	Selm[NI]=znew(word,N);
	SerN[NI]=znew(word,N);
	NSL[NI]=N;
	addrand(NSL[NI]+1);
	N=0;
	word* SMN=Selm[NI];
	word* SRN=SerN[NI];
	for(i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&OB->NNUM==GSets.CGame.cgi_NatRefTBL[NI]&&OB->NewBuilding){
			SMN[N]=OB->Index;
			SRN[N]=OB->Serial;
			N++;
			OB->Selected|=GM(NI);
		};
	};
};
void ImSelectAllBuildings(byte NI){
	int N=ImNSL[NI];
	if(N){
		word* SMN=ImSelm[NI];
		word* SRN=ImSerN[NI];
		for(int i=0;i<N;i++){
			word MID=SMN[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==SRN[i])OB->ImSelected&=~GM(NI);
			};
		};
	};
	if(ImSelm[NI]){
		free(ImSelm[NI]);
		ImSelm[NI]=NULL;
	};
	if(ImSerN[NI]){
		free(ImSerN[NI]);
		ImSerN[NI]=NULL;
	};
	ImNSL[NI]=0;
	N=0;
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&OB->NNUM==GSets.CGame.cgi_NatRefTBL[NI]&&OB->NewBuilding&&!(OB->Sdoxlo||OB->notSelectable()||OB->newMons->NewParams.PortretDisable))N++;
	};
	ImSelm[NI]=znew(word,N);
	ImSerN[NI]=znew(word,N);
	ImNSL[NI]=N;
	N=0;
	word* SMN=ImSelm[NI];
	word* SRN=ImSerN[NI];
	for(i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&OB->NNUM==GSets.CGame.cgi_NatRefTBL[NI]&&OB->NewBuilding&&!(OB->Sdoxlo||OB->notSelectable()||OB->newMons->NewParams.PortretDisable)){
			SMN[N]=OB->Index;
			SRN[N]=OB->Serial;
			N++;
			OB->ImSelected|=GM(NI);
		};
	};
};
void SelectUnitsTypes(byte NI,bool Re, byte Type){
	//Type
	//All Units - 0, Warriors - 1, Shooters - 2, Ships - 3, Fly - 4 
	word* lpN;
	word** UNI;
	word** USN;
	if(Re){
		lpN=NSL+NI;
		UNI=Selm+NI;
		USN=SerN+NI;
	}else{
		lpN=ImNSL+NI;
		UNI=ImSelm+NI;
		USN=ImSerN+NI;
	};
	int N=*lpN;
	if(N){
		word* SMN=*UNI;
		word* SRN=*USN;
		for(int i=0;i<N;i++){
			word MID=SMN[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==SRN[i]){
					if(Re)OB->Selected&=~GM(NI);
					else OB->ImSelected&=~GM(NI);
				};
			};
		};
	};
	if(UNI[0]){
		free(UNI[0]);
		UNI[0]=NULL;
	};
	if(USN[0]){
		free(USN[0]);
		USN[0]=NULL;
	};
	lpN[0]=0;
	N=0;
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		switch(Type){
		case 0://All
			if(OB&&OB->NNUM==GSets.CGame.cgi_NatRefTBL[NI]&&
			!(OB->LockType==1||OB->NewBuilding||OB->newMons->Peasant||OB->Sdoxlo||OB->Wall||OB->UnlimitedMotion||OB->MotionStyle==8||OB->Guard!=0xFFFF||OB->UnlimitedMotion||OB->notSelectable()||OB->newMons->NewParams.PortretDisable))N++;
		break;
		case 1://Warriors
			if(OB&&OB->NNUM==GSets.CGame.cgi_NatRefTBL[NI]&&!(OB->Sdoxlo||OB->NewBuilding||OB->newMons->Peasant||OB->MotionStyle==8||OB->Guard!=0xFFFF||OB->MotionStyle==5||OB->UnlimitedMotion||OB->notSelectable()||OB->newMons->NewParams.PortretDisable)){
			int MaxRadius=0;
			if (NAttTypes){
				for(int j=0;j<NAttTypes;j++)
							{
								if(OB->GetMaxAttackRadius(j)>MaxRadius)
									MaxRadius=OB->GetMaxAttackRadius(j);
							}
			}else{
			MaxRadius=OB->GetMaxAttackRadius(OB->NewState-1);
			}
			if(MaxRadius<240)N++;
		};
		break;
		case 2://Shooters
		if(OB&&OB->NNUM==GSets.CGame.cgi_NatRefTBL[NI]&&!(OB->Sdoxlo||OB->newMons->Peasant||OB->NewBuilding||OB->MotionStyle==8||OB->Guard!=0xFFFF||OB->MotionStyle==5||OB->UnlimitedMotion||OB->notSelectable()||OB->newMons->NewParams.PortretDisable)){
			int MaxRadius=0;
			if (NAttTypes){
				for(int j=0;j<NAttTypes;j++)
							{
								if(OB->GetMaxAttackRadius(j)>MaxRadius)
									MaxRadius=OB->GetMaxAttackRadius(j);
							}
			}else{
			MaxRadius=OB->GetMaxAttackRadius(OB->NewState-1);
			}
			if(MaxRadius>240)N++;
		};
		break;
		case 3://Ships
			if(OB&&OB->NNUM==GSets.CGame.cgi_NatRefTBL[NI]&&OB->LockType==1&&(OB->MotionStyle==5)&&!(OB->Sdoxlo||OB->Guard!=0xFFFF||OB->notSelectable()))N++;
		break;
		case 4://Fly
			if(OB&&OB->NNUM==GSets.CGame.cgi_NatRefTBL[NI]&&(OB->MotionStyle==8)&&!(OB->Sdoxlo||OB->Guard!=0xFFFF||OB->notSelectable()))N++;
		break;
		}
	};
	UNI[0]=znew(word,N);
	USN[0]=znew(word,N);
	lpN[0]=N;
	N=0;
	word* SMN=UNI[0];
	word* SRN=USN[0];
	for(i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		//All Units - 0, Warriors - 1, Shooters - 2, Ships - 3, Fly - 4 
	switch (Type){
		case 0://All Units
			if(OB&&OB->NNUM==GSets.CGame.cgi_NatRefTBL[NI]&&
			!(OB->LockType==1||OB->NewBuilding||OB->newMons->Peasant||OB->Sdoxlo||OB->Wall||OB->UnlimitedMotion||OB->MotionStyle==8||OB->Guard!=0xFFFF||OB->UnlimitedMotion||OB->notSelectable()||OB->newMons->NewParams.PortretDisable)){
			SMN[N]=OB->Index;
			SRN[N]=OB->Serial;
			if(Re)OB->Selected|=GM(NI);
			else OB->ImSelected|=GM(NI);
			N++;
		};
		break;
	    case 1://Warriors
			if(OB&&OB->NNUM==GSets.CGame.cgi_NatRefTBL[NI]&&!(OB->Sdoxlo||OB->newMons->Peasant||OB->NewBuilding||OB->MotionStyle==8||OB->Guard!=0xFFFF||OB->MotionStyle==5||OB->UnlimitedMotion||OB->notSelectable()||OB->newMons->NewParams.PortretDisable)){
			int MaxRadius=0;
			if (NAttTypes){
				for(int j=0;j<NAttTypes;j++)
							{
								if(OB->GetMaxAttackRadius(j)>MaxRadius)
									MaxRadius=OB->GetMaxAttackRadius(j);
							}
			}else{
			MaxRadius=OB->GetMaxAttackRadius(OB->NewState-1);
			}
			if(MaxRadius<240){
				SMN[N]=OB->Index;
				SRN[N]=OB->Serial;
				if(Re)OB->Selected|=GM(NI);
				else OB->ImSelected|=GM(NI);
				N++;
			};
		};
		break;
		case 2://Shooters
			if(OB&&OB->NNUM==GSets.CGame.cgi_NatRefTBL[NI]&&!(OB->Sdoxlo||OB->newMons->Peasant||OB->NewBuilding||OB->MotionStyle==8||OB->Guard!=0xFFFF||OB->MotionStyle==5||OB->UnlimitedMotion||OB->notSelectable()||OB->newMons->NewParams.PortretDisable)){
			int MaxRadius=0;
			if (NAttTypes){
				for(int j=0;j<NAttTypes;j++)
							{
								if(OB->GetMaxAttackRadius(j)>MaxRadius)
									MaxRadius=OB->GetMaxAttackRadius(j);
							}
			}else{
			MaxRadius=OB->GetMaxAttackRadius(OB->NewState-1);
			}
			if(MaxRadius>240){
				SMN[N]=OB->Index;
				SRN[N]=OB->Serial;
				if(Re)OB->Selected|=GM(NI);
				else OB->ImSelected|=GM(NI);
				N++;
			};
		};
		break;
		case 3://Ships
		if(OB&&OB->NNUM==GSets.CGame.cgi_NatRefTBL[NI]&&OB->LockType==1&&(OB->MotionStyle==5)&&!(OB->Sdoxlo||OB->Guard!=0xFFFF||OB->notSelectable())){
				SMN[N]=OB->Index;
				SRN[N]=OB->Serial;
				if(Re)OB->Selected|=GM(NI);
				else OB->ImSelected|=GM(NI);
				N++;
		};
		break;
		case 4://Fly
		    if(OB&&OB->NNUM==GSets.CGame.cgi_NatRefTBL[NI]&&(OB->MotionStyle==8)&&!(OB->Sdoxlo||OB->Guard!=0xFFFF||OB->notSelectable())){
				SMN[N]=OB->Index;
				SRN[N]=OB->Serial;
				if(Re)OB->Selected|=GM(NI);
				else OB->ImSelected|=GM(NI);
				N++;
		};
		break;
	  };
	};
};
void SelectAllPeasants(byte NI,bool Re){
	word* lpN;
	word** UNI;
	word** USN;
	if(Re){
		lpN=NSL+NI;
		UNI=Selm+NI;
		USN=SerN+NI;
	}else{
		lpN=ImNSL+NI;
		UNI=ImSelm+NI;
		USN=ImSerN+NI;
	};
	int N=*lpN;
	if(N){
		word* SMN=*UNI;
		word* SRN=*USN;
		for(int i=0;i<N;i++){
			word MID=SMN[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==SRN[i]){
					if(Re)OB->Selected&=~GM(NI);
					else OB->ImSelected&=~GM(NI);
				};
			};
		};
	};
	if(UNI[0]){
		free(UNI[0]);
		UNI[0]=NULL;
	};
	if(USN[0]){
		free(USN[0]);
		USN[0]=NULL;
	};
	lpN[0]=0;
	N=0;
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&OB->NNUM==GSets.CGame.cgi_NatRefTBL[NI]&&!(OB->Sdoxlo||OB->MotionStyle==8||OB->MotionStyle==5||OB->notSelectable())){
			if(OB->newMons->Peasant)N++;
		};
	};
	UNI[0]=znew(word,N);
	USN[0]=znew(word,N);
	lpN[0]=N;
	N=0;
	word* SMN=UNI[0];
	word* SRN=USN[0];
	for(i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&OB->NNUM==GSets.CGame.cgi_NatRefTBL[NI]&&!(OB->Sdoxlo||OB->MotionStyle==8||OB->MotionStyle==5||OB->notSelectable())){
//			byte Usage=OB->newMons->Usage;
//			if(Usage!=FisherID&&Usage!=ParomID){
			//if NAttTypes=GetMainChar(NI, NIndex, &MC);
			if(OB->newMons->Peasant){
				SMN[N]=OB->Index;
				SRN[N]=OB->Serial;
				if(Re)OB->Selected|=GM(NI);
				else OB->ImSelected|=GM(NI);
				N++;
			};
		};
	};
};
void MakeStandGround(Brigade* BR){
	if(BR->LastOrderTime==REALTIME/*tmtmt*/)return;
	int N=BR->NMemb;
	word* Memb=BR->Memb;
	word* MembSN=BR->MembSN;
	int addD=0;
	int addS=0;
	bool Rotate=0;
	if(BR->WarType){
		OrderDescription* ODS=ElementaryOrders+BR->WarType-1;
		addD=ODS->AddDamage1;
		addS=ODS->AddShield1;
		if(BR->Memb[2]!=0xFFFF){
			addS+=ODS->FAddShield;
			addD+=ODS->FAddDamage;
		};
		if(ODS->OrdUsage==2)Rotate=1;
	};
	BR->AddDamage=addD;
	BR->AddShield=addS;
	BR->InStandGround=1;
	int xc=0;
	int yc=0;
	int NU=0;
	for(int i=0;i<N;i++){
		word MID=Memb[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==MembSN[i]){
				if(i>=NBPERSONAL){
					xc+=OB->RealX>>4;
					yc+=OB->RealY>>4;
					NU++;
				}
				OB->StandGround=true;
				OB->AddShield=addS;
				OB->AddDamage=addD;
			};
		};
	};
	BR->LastOrderTime=REALTIME;//tmtmt;
	if(Rotate&&NU){
		xc/=NU;
		yc/=NU;
		for(int i=0;i<N;i++){
			word MID=Memb[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==MembSN[i]){
					Order1* Ord=OB->LocalOrder;
					bool rot=true;
					while(Ord){
						void RotUnitLink(OneObject* OB);
						if(Ord->DoLink==&RotUnitLink){
							rot=false;
							break;
						}
						Ord=Ord->NextOrder;
					}
					if(rot){
						//OB->ClearOrders();
						word GetDir(int,int);
						void RotUnit(OneObject* OB,char Dir,byte OrdType);
						byte d=GetDir((OB->RealX>>4)-xc,(OB->RealY>>4)-yc);					
						RotUnit(OB,d,2);
					}						
				};
			};
		};
	};
};
word GetDir(int,int);
void RotateMon(OneObject* OB,char angle);
void RotUnit(OneObject* OB,char Dir,byte OrdType);
void RotUnitLink(OneObject* OB);
void MakeStandGroundTemp(Brigade* BR){
	//if(BR->BrigDelay>1)return;
	if(BR->LastOrderTime==REALTIME/*tmtmt*/)return;	
	//if(BR->BOrder){
	//	BR->ClearBOrders();
	//};
	int N=BR->NMemb;
	word* Memb=BR->Memb;
	word* MembSN=BR->MembSN;

	bool Rotate=0;
	/*
	if(BR->WarType){
		OrderDescription* ODS=ElementaryOrders+BR->WarType-1;
		if(strstr(ODS->ID,"#KARE"))Rotate=1;
		
	}
	*/
	int xc=0;
	int yc=0;
	int NU=0;
	
	bool StandGroud=0;

	for(int i=0;i<N;i++){
		word MID=Memb[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==MembSN[i]){
				if(i>2&&OB->StandGround) StandGroud=1;
				NU++;
				xc+=OB->RealX>>4;
				yc+=OB->RealY>>4;
			}
		}
	}
	int BD=BRIGDELAY;
	///if(!StandGroud){
		int addD=0;
		int addS=0;	
		int BB=100;
		if(BR->WarType){
			OrderDescription* ODS=ElementaryOrders+BR->WarType-1;
			addD=ODS->AddDamage2;
			addS=ODS->AddShield2;
			if(BR->Memb[2]!=0xFFFF){
				addS+=ODS->FAddShield;
				addD+=ODS->FAddDamage;
			};	
			BB=ODS->StandGroundBonus;
		};
		BR->AddDamage=addD;
		BR->AddShield=addS;
		BR->InStandGround=0;
		for(i=0;i<N;i++){
			word MID=Memb[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==MembSN[i]){				
					OB->StandGround=true;
					OB->AddShield=addS;
					OB->AddDamage=addD;
					BD=OB->newMons->StandGroundTime;
					//if(OB->LocalOrder&&!OB->NewBuilding)OB->ClearOrders();
				};
			};
		};		
		BR->BrigDelay=BD*BB;
		BR->MaxBrigDelay=BR->BrigDelay;
	//}

	if(Rotate&&NU){
		xc/=NU;
		yc/=NU;
		for(int i=0;i<N;i++){
			word MID=Memb[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==MembSN[i]){
					Order1* Ord=OB->LocalOrder;
					bool rot=true;
					while(Ord){
						if(Ord->DoLink==&RotUnitLink){
							rot=false;
							break;
						}
						Ord=Ord->NextOrder;
					}
					if(rot){
						//OB->ClearOrders();
						byte d=GetDir((OB->RealX>>4)-xc,(OB->RealY>>4)-yc);					
						RotUnit(OB,d,2);
					}						
				};
			};
		};
	};

	BR->LastOrderTime=REALTIME;//tmtmt;
};
void SetAttState(Brigade* BR,bool Val){
	if(BR->LastOrderTime==REALTIME/*tmtmt*/)return;
	int N=BR->NMemb;
	word* Memb=BR->Memb;
	word* MembSN=BR->MembSN;
	for(int i=0;i<N;i++){
		word MID=Memb[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
#ifdef SIMPLEMANAGE
			if(OB&&OB->Serial==MembSN[i]&&!OB->newMons->ArmAttack){
#else //SIMPLEMANAGE
			if(OB&&OB->Serial==MembSN[i]){
#endif
				addrand(OB->NoSearchVictim);
				if(!OB->newMons->Priest)OB->NoSearchVictim=Val;
				addrand(OB->NoSearchVictim);
				if(Val){
					if(OB->EnemyID!=0xFFFF)OB->ClearOrders();
				};
			};
		};
	};
	BR->LastOrderTime=REALTIME;//tmtmt;
};
void NewAttackPointLink(OneObject* OBJ);
void SetSearchVictim(byte NI,byte Val){
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	Brigade* BR0=NATIONS[GSets.CGame.cgi_NatRefTBL[NI]].CITY->Brigs;
	if(!Nsel)return;
	for(int i=0;i<Nsel;i++){
   		word MID=SMon[i];
   		if(MID!=0xFFFF){
   			OneObject* OB=Group[MID];
			if(OB){
				if(OB->BrigadeID!=0xFFFF){
					Brigade* BR=BR0+OB->BrigadeID;
					SetAttState(BR,Val!=0);
				}else{
					if(Val&2){
						addrand(OB->NoSearchVictim);
						OB->NoSearchVictim=Val&1;
						addrand(OB->NoSearchVictim);
						if(OB->EnemyID!=0xFFFF){
							void PerformUpgradeLink(OneObject* OBJ);
							bool upg=OB->LocalOrder&&OB->LocalOrder->DoLink==&PerformUpgradeLink;
							if(!upg)OB->ClearOrders();
						}
					}else{
						if(!OB->newMons->Priest){
							addrand(OB->NoSearchVictim);
							OB->NoSearchVictim=Val&1;
							void PerformUpgradeLink(OneObject* OBJ);
							bool upg=OB->LocalOrder&&OB->LocalOrder->DoLink==&PerformUpgradeLink;
							addrand(OB->NoSearchVictim);
							if(OB->EnemyID!=0xFFFF&&!upg)OB->ClearOrders();
							if(!CheckIfPossibleToBreakOrder(OB)){								
								if(!upg)OB->ClearOrders();                                
							}
						};
					};
				};
			};
        };
	};
};
void SetShotMyUnitsState(byte NI,byte Val){
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	Brigade* BR0=NATIONS[GSets.CGame.cgi_NatRefTBL[NI]].CITY->Brigs;
	if(!Nsel)return;
	for(int i=0;i<Nsel;i++){
   		word MID=SMon[i];
   		if(MID!=0xFFFF){
   			OneObject* OB=Group[MID];
			if(OB&&OB->newMons->FriendlyFire){
				OB->FriendlyFire=Val;
			};
        };
	};
};
void CancelStandGroundAnyway(Brigade* BR);
void B_GoOnRoadLink(Brigade* BR);
void MoveBrigadeForwardToAttack(byte NI,word BrigID,bool AddPos=false){
	REALTIME++;
	Brigade* BR=CITY[NI].Brigs+BrigID;
	int BestDir=-256;
	if(BR->WarType){
		OrderDescription* ODE=ElementaryOrders+BR->WarType-1;
		if(ODE->OrdUsage==2){
			for(int j=NBPERSONAL;j<BR->NMemb;j++){
				word MID=BR->Memb[j];
				if(MID!=0xFFFF){
                    OneObject* OB=Group[MID];
					if(OB&&OB->Serial==BR->MembSN[j]){
						if(OB->newMons->ArmAttack){
							OB->NewState=1;							
						}
					}
				}
			}
			return;
		}else{
			/*
			for(int j=NBPERSONAL;j<BR->NMemb;j++){
				word MID=BR->Memb[j];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&OB->Serial==BR->MembSN[j]){
						if(OB->newMons->ArmAttack){
							OB->NewState=1;							
							OB->GroundState=1;
							BR->AttEnm=true;
							//TryToStand(OB,0);
						}
					}
				}
			}
			*/
		}
		if(ODE->OrdUsage==1){//square formation
            //search for nearest enemy in formation;
			int MINR=1200;
			int x0,y0;
			if(BR->GetCenter(&x0,&y0)){
				byte NMS=BR->GetNMask();
				for(int i=0;i<8;i++){					
					for(int j=0;j<CITY[i].NBrigs;j++){
						Brigade* BR2=CITY[i].Brigs+j;
						if(BR2->Enabled&&BR2->WarType&&!(BR2->GetNMask()&NMS)){
							int x,y;
							if(BR2->GetCenter(&x,&y)){
                                int R=Norma(x-x0,y-y0);
								if(R<MINR){
									MINR=R;
									BestDir=GetDir(x-x0,y-y0);
								}
							}
						}
					}
				}
			}			
		}
	};
	word* IDS=BR->Memb;
	word* SNS=BR->MembSN;
	int N=BR->NMemb;
	byte DIR;
	DIR=BestDir==-256?BR->Direction:BestDir;//+128;
	//else DIR=BR->Direction;
	int DX=TCos[DIR];
	int DY=TSin[DIR];
	DX<<=5;
	DY<<=5;
	//DX*=abs(Dir);
	//DY*=abs(Dir);

	int xx,yy;
	byte dr;
	BR->GetCenter(&xx,&yy,&dr);

	if(BR->Enabled&&BR->WarType&&BR->PosCreated){
		//if(BR->BOrder&&BR->BOrder->BLink==&B_GoOnRoadLink)
		if(BR->NewBOrder&&BR->NewBOrder->GetBrigadeOrderID()==BRIGADEORDER_GOONROAD)
		{			
			//BR->CreateSimpleOrderedPositions(xx<<4,yy<<4,dr);
			BR->CreateOrderedPositions(xx<<4,yy<<4,dr);
			BR->ResortMembByPos();
			BR->Direction=dr;
			DX=0;
			DY=0;
		}

		//new for GEC
		Brigade* GetEnemyForMeleeAttack(Brigade* BR);
		Brigade* EB=GetEnemyForMeleeAttack(BR);
		if(EB){
			int xe,ye;
			byte dire;
			if(EB->GetCenter(&xe,&ye,&dire)){
				CancelStandGroundAnyway(BR);
				addrand(39);
				BR->AttEnm=1;
                char DIR=GetDir(xe-xx,ye-yy);
				SetEnemyForBrigade(BR,EB->ID,EB->CT->NI);
				char DestD=BR->Direction;
				if(abs(int(DIR-char(BR->Direction)))<36)DestD=DIR;
				if( TestIfLineFormation(EB) && TestIfLineFormation(BR) ){
					if(abs(int(char(EB->Direction-BR->Direction)))<38){
						DestD=EB->Direction;
					}else
					if(abs(int(char(EB->Direction-BR->Direction-128)))<38){
						DestD=EB->Direction+128;
					}
				}
				ShiftDestPoint(xx,yy,xe,ye,80);
				BR->AttEnm=1;  
				//if(abs(int(DIR-char(BR->Direction)))<36)BR->HumanLocalSendTo(xe<<4,ye<<4,DIR,128,0);//direction??
				//else 
				BR->HumanLocalSendTo(xe<<4,ye<<4,DestD,128,0);//direction??
				BR->AttEnm=1;  
				for(int i=NBPERSONAL;i<BR->NMemb;i++){
					word ID=BR->Memb[i];
					if(ID!=0xFFFF){
						OneObject* OB=Group[ID];
						if(OB&&OB->Serial==BR->MembSN[i]){
							if(OB->newMons->ArmAttack){											
								OB->GroundState=1;
								OB->NewState=OB->GroundState;
							}										
						}
					}
				}
				return;
			}
		}//otherwice simple prepare to attack
		
		int GState=0;
		/*
		for(int i=NBPERSONAL;i<BR->NMemb;i++){
			word ID=BR->Memb[i];
			if(ID!=0xFFFF){
				OneObject* OB=Group[ID];
				if(OB&&OB->Serial==BR->MembSN[i]){
					GState=OB->GroundState;										
				}
			}
		}
		*/
		if(GState){
			BR->AttEnm=false;
		}else{
			BR->AttEnm=true;
		}
		for(int i=NBPERSONAL;i<BR->NMemb;i++){
			word ID=BR->Memb[i];
			if(ID!=0xFFFF){
				OneObject* OB=Group[ID];
				if(OB&&OB->Serial==BR->MembSN[i]){
					if(OB->newMons->ArmAttack){											
						OB->GroundState=!GState;
						OB->NewState=OB->GroundState;
					}										
				}
			}
		}
		BR->KeepPositions(0,BR->AttEnm?128+1:128+16);
		BR->LastOrderTime=REALTIME;
		
		//end new for GEC

		/*
		int x=0;
		int y=0;
		int cx=0;
		int cy=0;
		int N=0;
		for(int j=NBPERSONAL;j<BR->NMemb;j++){
			if(BR->Memb[j]!=0xFFFF)
			{
				OneObject* Obb = Group[BR->Memb[j]];
				if(Obb)
				{
					x+=BR->posX[j];
					y+=BR->posY[j];
					cx+=Obb->RealX>>4;
					cy+=Obb->RealY>>4;
					N++;
				}
			}
		};
		if(!AddPos)
		{
			x=cx;
			y=cy;
		}
		if(N){ 
			x/=N;
			y/=N;
			CancelStandGroundAnyway(BR);
			addrand(39);
			BR->AttEnm=1;
			BR->HumanLocalSendTo((x<<4)+DX,(y<<4)+DY,BR->Direction,128,0);
			BR->AttEnm=1;
		};
		*/
	};
};
void SetArmAttackState(byte NI,byte Val){
#ifdef COSSACKS2
	
#endif //COSSACKS2
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	Brigade* BR0=NATIONS[GSets.CGame.cgi_NatRefTBL[NI]].CITY->Brigs;
	if(!Nsel)return;
	for(int i=0;i<Nsel;i++){
   		word MID=SMon[i];
   		if(MID!=0xFFFF){
   			OneObject* OB=Group[MID];
			if(OB&&OB->newMons->ArmAttack){
#ifdef SIMPLEMANAGE
				if(Val&128){
					OB->RifleAttack=Val&127;
					if(OB->RifleAttack&&OB->BrigadeID!=0xFFFF)BR0[OB->BrigadeID].AttEnm=1;
				}else{
					//OB->RifleAttack=0;
					//OB->ArmAttack=1;
					
					OB->ArmAttack=Val&1;

					if(OB->BrigadeID!=0xFFFF){
						Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
						if(BR->LastOrderTime!=REALTIME){
							MoveBrigadeForwardToAttack(OB->NNUM,OB->BrigadeID,Val==2);
							BR->LastOrderTime=REALTIME;
						}
						/*
						if(BR->LastOrderTime!=REALTIME){
							int GState=0;
							for(int i=NBPERSONAL;i<BR->NMemb;i++){
								word ID=BR->Memb[i];
								if(ID!=0xFFFF){
									OneObject* OB=Group[ID];
									if(OB&&OB->Serial==BR->MembSN[i]){
										GState=OB->GroundState;										
									}
								}
							}
							if(GState){
								BR->AttEnm=false;
							}else{
								BR->AttEnm=true;
							}
							for(int i=NBPERSONAL;i<BR->NMemb;i++){
								word ID=BR->Memb[i];
								if(ID!=0xFFFF){
									OneObject* OB=Group[ID];
									if(OB&&OB->Serial==BR->MembSN[i]){
										if(OB->newMons->ArmAttack){											
											OB->GroundState=!GState;
											OB->NewState=OB->GroundState;
										}										
									}
								}
							}
							BR->KeepPositions(0,BR->AttEnm?128+1:128+16);
							BR->LastOrderTime=REALTIME;
						}
						*/
					}else{
						int GState=Val&1;
						if(OB->newMons->ArmAttack){											
							OB->GroundState=GState;
							OB->NewState=OB->GroundState;
						}                        
					}
				}
				if(OB->EnemyID!=0xFFFF&&!OB->RifleAttack){
					if(CheckIfPossibleToBreakOrder(OB))OB->ClearOrders();
				}
#else //SIMPLEMANAGE
				OB->ArmAttack=Val;
#endif //SIMPLEMANAGE
			};
        };
	};
};
void SetStopBuildState(byte NI,byte Val){
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	if(!Nsel)return;
	for(int i=0;i<Nsel;i++){
   		word MID=SMon[i];
   		if(MID!=0xFFFF){
   			OneObject* OB=Group[MID];
			if(OB&&OB->newMons->CanStopBuild){
				OB->StopBuildMode=Val;
			};
        };
	};
};
void SetFishingState(byte NI,byte Val){
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	if(!Nsel)return;
	for(int i=0;i<Nsel;i++){
   		word MID=SMon[i];
   		if(MID!=0xFFFF){
   			OneObject* OB=Group[MID];
			if(OB&&OB->newMons->FishSpeed){
				OB->StopBuildMode=Val;
			};
        };
	};
};
extern "C" CEXPORT
void CreateFields(byte NI);
void SetAutozasevState(byte NI,byte Val){
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	if(!Nsel)return;
	for(int i=0;i<Nsel;i++){
   		word MID=SMon[i];
   		if(MID!=0xFFFF){
   			OneObject* OB=Group[MID];
			if(OB&&OB->newMons->Usage==MelnicaID){
				OB->AutoZasev=Val;
			};
        };
	};
	if(!Val)CreateFields(NI);
};
void MakeStandGround(byte NI){
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	Brigade* BR0=NATIONS[GSets.CGame.cgi_NatRefTBL[NI]].CITY->Brigs;
	if(!Nsel)return;
	for(int i=0;i<Nsel;i++){
   		word MID=SMon[i];
   		if(MID!=0xFFFF){
   			OneObject* OB=Group[MID];
			if(OB){
				if(OB->BrigadeID!=0xFFFF){
					Brigade* BR=BR0+OB->BrigadeID;
					MakeStandGroundTemp(BR);
				}else{
					OB->StandGround=true;
				};
			};
        };
	};
};
void CancelStandGround(Brigade* BR){
	if(BR->LastOrderTime==REALTIME/*tmtmt*/)return;
	int N=BR->NMemb;
	word* Memb=BR->Memb;
	word* MembSN=BR->MembSN;
	int addD=0;
	int addS=0;
	if(BR->WarType){
		OrderDescription* ODS=ElementaryOrders+BR->WarType-1;
		if(ODS->OrdUsage==2)return;
		addD=ODS->AddDamage2;
		addS=ODS->AddShield2;
		if(BR->Memb[2]!=0xFFFF){
			addS+=ODS->FAddShield;
			addD+=ODS->FAddDamage;
		};
	};
	BR->AddDamage=addD;
	BR->AddShield=addS;
	BR->InStandGround=0;
	for(int i=0;i<N;i++){
		word MID=Memb[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==MembSN[i]){
				OB->StandGround=false;
				OB->AddShield=addS;
				OB->AddDamage=addD;
			};
		};
	};
	BR->LastOrderTime=REALTIME;//tmtmt;
	BR->BrigDelay=0;
};
void CancelStandGroundAnyway(Brigade* BR){
	int N=BR->NMemb;
	word* Memb=BR->Memb;
	word* MembSN=BR->MembSN;
	int addD=0;
	int addS=0;
	if(BR->WarType){
		OrderDescription* ODS=ElementaryOrders+BR->WarType-1;
		addD=ODS->AddDamage2;
		addS=ODS->AddShield2;
		if(BR->Memb[2]!=0xFFFF){
			addS+=ODS->FAddShield;
			addD+=ODS->FAddDamage;
		};
	};
	BR->AddDamage=addD;
	BR->AddShield=addS;
	BR->InStandGround=0;
	BR->BrigDelay=BR->MaxBrigDelay;
	for(int i=0;i<N;i++){
		word MID=Memb[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==MembSN[i]){
				OB->StandGround=false;
				OB->AddShield=addS;
				OB->AddDamage=addD;
			};
		};
	};
};
void CreateFields(byte NI,int x,int y,int n);
void CreateFieldsBar(byte NI,word n){
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	for(int i=0;i<Nsel;i++){
		word MID=SMon[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->newMons->Usage==MelnicaID&&OB->Nat->PACount[OB->NIndex])CreateFields(NI,OB->RealX,OB->RealY,n);
		};
	};
};
void CancelStandGround(byte NI){
	word Nsel=NSL[NI];
	word* SMon=Selm[NI];
	Brigade* BR0=NATIONS[GSets.CGame.cgi_NatRefTBL[NI]].CITY->Brigs;
	if(!Nsel)return;
	for(int i=0;i<Nsel;i++){
   		word MID=SMon[i];
   		if(MID!=0xFFFF){
   			OneObject* OB=Group[MID];
			if(OB){
				if(OB->BrigadeID!=0xFFFF){
					Brigade* BR=BR0+OB->BrigadeID;
					CancelStandGround(BR);
				}else{
					OB->StandGround=false;
				};
			};
        };
	};
};
void SelBrigade(byte NI,byte Type,byte BNat,int ID){
	if(!Type){
		if(NSL[NI]){
			int N=NSL[NI];
			word* SEL=Selm[NI];
			word* SSN=SerN[NI];
			for(int j=0;j<N;j++){
				word MID=SEL[j];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&OB->Serial==SSN[j]){
						OB->Selected&=~GM(NI);
					};
				};
			};
			free(Selm[NI]);
			free(SerN[NI]);
			Selm[NI]=NULL;
			SerN[NI]=NULL;
			NSL[NI]=0;
			addrand(NSL[NI]+1);
		};
	};
	Brigade* BR=NATIONS[BNat].CITY->Brigs+ID;
	if(BR->Enabled){
		int NAdd=0;
		for(int i=2;i<BR->NMemb;i++){
			word MID=BR->Memb[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&!(OB->Selected&GM(NI))){
					NAdd++;
				};
			};
		};
		if(NAdd){
			int N=NSL[NI];
			Selm[NI]=(word*)realloc(Selm[NI],(N+NAdd)*2);
			SerN[NI]=(word*)realloc(SerN[NI],(N+NAdd)*2);
			for(int i=2;i<BR->NMemb;i++){
				word MID=BR->Memb[i];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&!(OB->Selected&GM(NI))){
						int N=NSL[NI];
						Selm[NI][N]=OB->Index;
						SerN[NI][N]=OB->Serial;
						NSL[NI]++;
						OB->Selected|=GM(NI);
					};
				};
			};
		};
	};
};
void ImSelBrigade(byte NI,byte Type,byte BNAT,int ID){
	if(!Type){
		if(ImNSL[NI]){
			int N=ImNSL[NI];
			word* SEL=ImSelm[NI];
			word* SSN=ImSerN[NI];
			for(int j=0;j<N;j++){
				word MID=SEL[j];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&OB->Serial==SSN[j]){
						OB->ImSelected&=~GM(NI);
					};
				};
			};
			free(ImSelm[NI]);
			free(ImSerN[NI]);
			ImSelm[NI]=NULL;
			ImSerN[NI]=NULL;
			ImNSL[NI]=0;
		};
	};
	Brigade* BR=NATIONS[BNAT].CITY->Brigs+ID;
	if(BR->Enabled){
		int NAdd=0;
		for(int i=2;i<BR->NMemb;i++){
			word MID=BR->Memb[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&!(OB->ImSelected&GM(NI))){
					NAdd++;
				};
			};
		};
		if(NAdd){
			int N=ImNSL[NI];
			ImSelm[NI]=(word*)realloc(ImSelm[NI],(N+NAdd)*2);
			ImSerN[NI]=(word*)realloc(ImSerN[NI],(N+NAdd)*2);
			for(int i=2;i<BR->NMemb;i++){
				word MID=BR->Memb[i];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if( OB && (!(OB->ImSelected&GM(NI))) && (OB->BrIndex==i) && (OB->NNUM==BR->CT->NI) ){
						int N=ImNSL[NI];
						ImSelm[NI][N]=OB->Index;
						ImSerN[NI][N]=OB->Serial;
						ImNSL[NI]++;
						OB->ImSelected|=GM(NI);
					};
				};
			};
		};
	};
};
extern City CITY[8];
void CreateBrOrder(byte NI,int i){
	DWORD p=i;

	word UID=word(p&8191);
	word ort=word((p>>13)&255);
	word Type=word(p>>21);

	//detecting officer
	for(int q=0;q<8;q++){
		OneObject* OOB=Group[UID];
		if(OOB&&OOB->Ref.General->OFCR&&OOB->BrigadeID==0xFFFF){
			q=8;
		}else{
			UID+=8192;
		};
	};
	OneObject* OB=Group[UID];
	if(NSL[NI]){
		int N=NSL[NI];
		for(int j=1;j<N;j++){
			word MID=Selm[NI][j];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB)OB->Selected&=~GM(NI);
			};
		};
		NSL[NI]=1;
		addrand(NSL[NI]+1);
	};
	if(OB&&(!OB->Sdoxlo)&&OB->BrigadeID==0xFFFF){
		int bind=CITY[GSets.CGame.cgi_NatRefTBL[NI]].GetFreeBrigade();
		Brigade* BR=CITY[GSets.CGame.cgi_NatRefTBL[NI]].Brigs+bind;
		BR->Enabled=true;
		if(!BR->CreateNearOfficer(Group[UID],Type,ort)){
			BR->Enabled=0;
		}else{
			CorrectBrigadesSelection(NI);
			ImCorrectBrigadesSelection(NI);
		};
	};
};
void PerformUpgradeLink(OneObject* OB);
void StopUpg(byte Nat, int UIndex){
	byte NAT1=GSets.CGame.cgi_NatRefTBL[Nat];
	int N=NSL[Nat];
	word* Uni=Selm[Nat];
	word* USN=SerN[Nat];
	for(int i=0;i<N;i++){
		word MID=Uni[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==USN[i]&&(!OB->Ready)&&OB->NUstages&&OB->LocalOrder&&OB->LocalOrder->DoLink==&PerformUpgradeLink){
				int UID=OB->LocalOrder->info.PUpgrade.NewUpgrade;
				if(UIndex==-1||UIndex==UID){
					NewUpgrade* NU=NATIONS[NAT1].UPGRADE[UID];
					for(int j=0;j<8;j++)AddXRESRC(NAT1,j,NU->Cost[j]);
					OB->DeleteLastOrder();
					OB->Ready=true;
					OB->NUstages=0;
					OB->Ustage=0;
					if(!(NU->Individual||NU->StageUp)){
						NU->Enabled=true;
						NU->PermanentEnabled=true;
						NU->IsDoing=false;
						NU->CurStage=0;
					};
				}
			};
		};
	};
};
void IndStopUpg(OneObject* OB){
	if((!OB->Ready)&&OB->NUstages&&OB->LocalOrder&&OB->LocalOrder->DoLink==&PerformUpgradeLink){
	byte NAT1=OB->NNUM;
	int UID=OB->LocalOrder->info.PUpgrade.NewUpgrade;
		NewUpgrade* NU=NATIONS[NAT1].UPGRADE[UID];
		for(int j=0;j<8;j++)AddXRESRC(NAT1,j,NU->Cost[j]);
			OB->DeleteLastOrder();
			OB->Ready=true;
			OB->NUstages=0;
			OB->Ustage=0;
			if(!(NU->Individual||NU->StageUp)){
				NU->Enabled=true;
				NU->PermanentEnabled=true;
				NU->IsDoing=false;
				NU->CurStage=0;
			};
	}
}
void StopUpg(byte Nat){
	StopUpg(Nat,-1);
}
void LeaveAll(OneObject* OB);
void SelUnloadAll(byte Nat){
	int N=NSL[Nat];
	word* Uni=Selm[Nat];
	word* USN=SerN[Nat];
	for(int i=0;i<N;i++){
		word MID=Uni[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==USN[i]&&OB->NInside&&OB->Transport&&OB->DstX>0){
				LeaveAll(OB);
			};
		};
	};
};
void CancelStandGround(Brigade* BR);
word GetBrigLockType(Brigade* BR);
extern void B_GoOnRoadLink(Brigade* BR);
void MakeReformation(Brigade* BR,byte FormType){
	if(BR&&BR->Enabled&&BR->WarType){
		addrand(BR->Direction);
		int OldFormType=BR->WarType;
		if(FormType==0xFF||FormType==0xFE||FormType==0xFD){
			int RealNM=0;
			int xc=0;
			int yc=0;
			for(int q=NBPERSONAL;q<BR->NMemb;q++){
				word MID=BR->Memb[q];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&(!OB->Sdoxlo)&&OB->Serial==BR->MembSN[q]){						
						RealNM++;
						xc+=int(BR->posX[q])<<4;//OB->RealX;
						yc+=int(BR->posY[q])<<4;//OB->RealY;
					};
				};
			};
			if(FormType==0xFD){
				if(!EngSettings.LowerFormationDistanceScale)EngSettings.LowerFormationDistanceScale=50;
				BR->ScaleFactor=EngSettings.LowerFormationDistanceScale;				
			}
			if(FormType==0xFE){
				BR->ScaleFactor=100;
			}
			if(FormType==0xFF){
				if(!EngSettings.GreaterFormationDistanceScale)EngSettings.GreaterFormationDistanceScale=200;
				BR->ScaleFactor=EngSettings.GreaterFormationDistanceScale;				
			}
			BR->CreateOrderedPositions(xc/RealNM,yc/RealNM,BR->Direction);
			BR->ResortMembByPos();
			BR->KeepPositions(1,128+16);
			CancelStandGround(BR);
			BR->LastOrderTime=0;
			MakeStandGroundTemp(BR);
			return;
		}
		int RealNM=0;
		for(int i=NBPERSONAL;i<BR->NMemb;i++){
			word MID=BR->Memb[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&(!OB->Sdoxlo)&&OB->Serial==BR->MembSN[i]){
					RealNM++;
				};
			};
		};
		//search for appropriate formation
		SingleGroup* SG=FormGrp.Grp+FormType;
		for(int j=0;j<SG->NForms&&RealNM>ElementaryOrders[SG->Forms[j]].NUnits;j++);
		if(j<SG->NForms){
			int WT=SG->Forms[j]+1;
			OrderDescription* ODE=ElementaryOrders+SG->Forms[j];
			word* TMP=znew(word,ODE->NUnits);
			memset(TMP,0xFF,ODE->NUnits*2);			
			RealNM=0;
			int xc=0;
			int yc=0;
			int bx=0;
			int by=0;
			for(int q=NBPERSONAL;q<BR->NMemb;q++){
				word MID=BR->Memb[q];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&(!OB->Sdoxlo)&&OB->Serial==BR->MembSN[q]){
						TMP[RealNM]=MID;
						RealNM++;
						xc+=int(BR->posX[q])<<4;//OB->RealX;
						yc+=int(BR->posY[q])<<4;//OB->RealY;
						bx+=OB->RealX;
						by+=OB->RealY;
					};
				};
			};
			if(RealNM==0)return;
			if(BR->PosCreated){
				BR->PosCreated=false;
				free(BR->posX);
				free(BR->posY);
				BR->posX=NULL;
				BR->posY=NULL;
			};
			if(BR->MaxMemb<ODE->NUnits+NBPERSONAL){
				BR->MaxMemb=ODE->NUnits+NBPERSONAL;
				BR->Memb=(word*)realloc(BR->Memb,BR->MaxMemb*2);
				BR->MembSN=(word*)realloc(BR->MembSN,BR->MaxMemb*2);
			};
			memcpy(BR->Memb+NBPERSONAL,TMP,ODE->NUnits*2);
			for(q=NBPERSONAL;q<RealNM;q++)BR->MembSN[q]=Group[BR->Memb[q]]->Serial;
			BR->WarType=WT;
			//word LT=GetBrigLockType(BR);
			//if(BR->BOrder&&BR->BOrder->BLink==&B_GoOnRoadLink)
			byte kpOrdType=1;
			if(!EngSettings.AutoChangeFormationType){
				addrand(BR->Direction);
				BR->Direction=BR->GetDirToNearesEnmBrigade();
				addrand(BR->Direction);
				BR->Direction=((BR->Direction+8)/16)*16;
				//if(BR->BOrder&&BR->BOrder->BLink==&B_GoOnRoadLink)
				if(BR->NewBOrder&&BR->NewBOrder->GetBrigadeOrderID()==BRIGADEORDER_GOONROAD)
				{
					//BR->ClearBOrders();
					kpOrdType=0;
					xc=bx;
					yc=by;
				}
			}
			if(OldFormType==WT)
			{
				xc=bx;
				yc=by;
			}

			addrand(BR->Direction);
			addrand(xc);
			addrand(yc);

			BR->CreateOrderedPositions(xc/RealNM,yc/RealNM,BR->Direction);
			BR->ResortMembByPos();
			addrand(128+16);
			BR->KeepPositions(kpOrdType,128+16);
			CancelStandGround(BR);
			BR->LastOrderTime=0;
			MakeStandGroundTemp(BR);
		};
	};
}
void MakeReformation(byte NI,word BrigadeID,byte FormType){
	if(NI>MaxNatColors)return;
	Brigade* BR=CITY[GSets.CGame.cgi_NatRefTBL[NI]].Brigs+BrigadeID;
	MakeReformation(BR,FormType);
};
//
void GotoFinePositionLink(OneObject* OB);
//
void ImClearSelection(byte Nat);
void ReClearSelection(byte Nat);
void SelectIdlePeasants(byte NI){
	addrand(NI);
	ReClearSelection(NI);
	int N=0;
	word* Units=NatList[GSets.CGame.cgi_NatRefTBL[NI]];
	int Nu=NtNUnits[GSets.CGame.cgi_NatRefTBL[NI]];
	for(int i=0;i<Nu;i++){
		OneObject* OB=Group[Units[i]];
		if(OB&&(!(OB->Sdoxlo||OB->notSelectable()))){
			NewMonster* NM=OB->newMons;
			if(NM->Usage==PeasantID&&(!OB->LocalOrder||OB->LocalOrder->DoLink==&GotoFinePositionLink))N++;
		};
	};
	if(N){
		Selm[NI]=znew(word,N);
		SerN[NI]=znew(word,N);
		NSL[NI]=N;
		addrand(NSL[NI]+1);
		N=0;
		for(int i=0;i<Nu;i++){
			OneObject* OB=Group[Units[i]];
			if(OB&&(!(OB->Sdoxlo||OB->notSelectable()))){
				NewMonster* NM=OB->newMons;
				if(NM->Usage==PeasantID&&(!OB->LocalOrder||OB->LocalOrder->DoLink==&GotoFinePositionLink)){
					Selm[NI][N]=OB->Index;
					SerN[NI][N]=OB->Serial;
					OB->Selected&=~GM(NI);
					N++;
				};
			};	
		};
	};
};
void ImSelectIdlePeasants(byte NI){
	ImClearSelection(NI);
	int N=0;
	word* Units=NatList[GSets.CGame.cgi_NatRefTBL[NI]];
	int Nu=NtNUnits[GSets.CGame.cgi_NatRefTBL[NI]];
	for(int i=0;i<Nu;i++){
		OneObject* OB=Group[Units[i]];
		if(OB&&(!(OB->Sdoxlo||OB->notSelectable()))){
			NewMonster* NM=OB->newMons;
			if(NM->Usage==PeasantID&&(!OB->LocalOrder||OB->LocalOrder->DoLink==&GotoFinePositionLink))N++;
		};
	};
	if(N){
		ImSelm[NI]=znew(word,N);
		ImSerN[NI]=znew(word,N);
		ImNSL[NI]=N;
		N=0;
		for(int i=0;i<Nu;i++){
			OneObject* OB=Group[Units[i]];
			if(OB&&(!(OB->Sdoxlo||OB->notSelectable()))){
				NewMonster* NM=OB->newMons;
				if(NM->Usage==PeasantID&&(!OB->LocalOrder||OB->LocalOrder->DoLink==&GotoFinePositionLink)){
					ImSelm[NI][N]=OB->Index;
					ImSerN[NI][N]=OB->Serial;
					OB->ImSelected|=GM(NI);
					N++;
				};
			};	
		};
	};
};
void SelectIdleMines(byte NI){
	addrand(NI);
	ReClearSelection(NI);
	int N=0;
	word* Units=NatList[GSets.CGame.cgi_NatRefTBL[NI]];
	int Nu=NtNUnits[GSets.CGame.cgi_NatRefTBL[NI]];
	for(int i=0;i<Nu;i++){
		OneObject* OB=Group[Units[i]];
		if(OB&&(!(OB->Sdoxlo||OB->notSelectable()))){
			NewMonster* NM=OB->newMons;
			if(NM->Usage==MineID&&OB->Ready&&OB->NInside<OB->AddInside+OB->MoreCharacter->MaxInside)N++;
		};
	};
	if(N){
		Selm[NI]=znew(word,N);
		SerN[NI]=znew(word,N);
		NSL[NI]=N;
		addrand(NSL[NI]+1);
		N=0;
		for(int i=0;i<Nu;i++){
			OneObject* OB=Group[Units[i]];
			if(OB&&(!(OB->Sdoxlo||OB->notSelectable()))){
				NewMonster* NM=OB->newMons;
				if(NM->Usage==MineID&&OB->Ready&&OB->NInside<OB->AddInside+OB->MoreCharacter->MaxInside){
					Selm[NI][N]=OB->Index;
					SerN[NI][N]=OB->Serial;
					OB->Selected|=GM(NI);
					N++;
				};
			};	
		};
	};
};
extern int NoMineSound;
void PlayEffect(int n,int pan,int vol);
void ImSelectIdleMines(byte NI){
	ImClearSelection(NI);
	int N=0;
	word* Units=NatList[GSets.CGame.cgi_NatRefTBL[NI]];
	int Nu=NtNUnits[GSets.CGame.cgi_NatRefTBL[NI]];
	for(int i=0;i<Nu;i++){
		OneObject* OB=Group[Units[i]];
		if(OB&&(!(OB->Sdoxlo||OB->notSelectable()))){
			NewMonster* NM=OB->newMons;
			if(NM->Usage==MineID&&OB->Ready&&OB->NInside<OB->AddInside+OB->MoreCharacter->MaxInside)N++;
		};
	};
	if(N){
		ImSelm[NI]=znew(word,N);
		ImSerN[NI]=znew(word,N);
		ImNSL[NI]=N;
		N=0;
		for(int i=0;i<Nu;i++){
			OneObject* OB=Group[Units[i]];
			if(OB&&(!(OB->Sdoxlo||OB->notSelectable()))){
				NewMonster* NM=OB->newMons;
				if(NM->Usage==MineID&&OB->Ready&&OB->NInside<OB->AddInside+OB->MoreCharacter->MaxInside){
					ImSelm[NI][N]=OB->Index;
					ImSerN[NI][N]=OB->Serial;
					OB->ImSelected|=GM(NI);
					N++;
				};
			};	
		};
	}else{
		PlayEffect(NoMineSound,0,0);
	};
};
void FreeSelected(byte NI){
	int Nsel=NSL[NI];
	word* SMon=Selm[NI];
	word* ser=SerN[NI];
	for(int i=0;i<Nsel;i++){
		word MID=SMon[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB/*&&OB->NNUM==GSets.CGame.cgi_NatRefTBL[NI]*/&&ser[i]==OB->Serial)OB->Selected&=~GM(NI);
		};
	};
	if(Selm[NI]){
		free(Selm[NI]);
		Selm[NI]=NULL;
	};
	if(SerN[NI]){
		free(SerN[NI]);
		SerN[NI]=NULL;
	};
	NSL[NI]=0;
	addrand(NSL[NI]+1);
};
void PerformTorg(byte Nation,byte SellRes,byte BuyRes,int SellAmount);

int _implSelBrigade(byte* Ptr);
int _implUnSelBrigade(byte* Ptr);
int _implUnSelUnitsSet(byte* Ptr);
int _implSelUnitsSet(byte* Ptr);
int _implSelUnitsSet_OLD(byte* Ptr);

extern bool GameExit;
extern int StartTmtmt;
extern byte PlayGameMode;
extern bool ShowStat;
extern int PREVSEUSETPOS;
void GroupSelectedFormations(byte NI);
void UnGroupSelectedUnits(byte NI);
void DoFillFormation(byte NI,word BrigadeID);
extern int HISPEED;
#define CHECKNSL for(int i=0;i<7;i++)addrand(NSL[i]+1);
void ExecuteBuffer(){
	if(PlayGameMode){
		if(!RGAME.Extract()){
			GameExit=true;
			ShowStat=0;
			EBPos=0;
		};
	}else if(RecordMode){
		RGAME.AddRecord();
	}
	if(!(GSets.CGame.ViewMask&4)){
        ///EBPos=0;
		//return;
	}
	for(int i=0;i<8;i++){
        int N=CITY[i].NBrigs;
		for(int j=0;j<N;j++){
			CITY[i].Brigs[j].lastTimeGetCenter=0;
		}
	}
	MaxDCT=0;
	SlowMade=0;
	InitSelection();
	PrevProdPos=-1;
	PREVSEUSETPOS=-1;
	PrevProdUnit=0;
	//ClearUniCash();
	for(int p=0;p<8;p++)RefreshSelected(p);
	int pos=0;
	int len;
	byte tp;
	CURMAXP=-1;
	ADDGR(5,GetTickCount(),EBPos,0xD0);
	//Rept("EXBUFFER: %d\n",EBPos);
	void ProcessMRoom();
	ProcessMRoom();
	char sss[128];
	while(pos<EBPos){
		byte cmd=ExBuf[pos];
		//Rept("%6d:%5X ",pos,cmd);
		pos++;
		addrand(cmd);
		addrand(ExBuf[pos]);
		switch(cmd){
		case 1://Create selection
			//CreateSelection(ExBuf[pos],ExBuf[pos+1],ExBuf[pos+2],
			//	ExBuf[pos+3],ExBuf[pos+4]);
			pos+=5;
			break;
		case 2://Send group to (X,Y)
			tp=ExBuf[pos+11];
			if((tp&15)<8)SendSelectedToXY(ExBuf[pos],*(int*)(&ExBuf[pos+1]),*(int*)(&ExBuf[pos+5]),*(short*)(&ExBuf[pos+9]),tp);
			else SendSelectedToXY(ExBuf[pos],*(int*)(&ExBuf[pos+1]),*(int*)(&ExBuf[pos+5]),*(short*)(&ExBuf[pos+9]),128,tp);
			pos+=12;
			break;
		case 3://Attack object with group
			AttackSelected(ExBuf[pos],*(word*)(&ExBuf[pos+1]),ExBuf[pos+3],*(short*)(&ExBuf[pos+4]));
			pos+=6;
			break;
		case 4://Create terrain object
			CreateTerrainMons(ExBuf[pos],ExBuf[pos+1],ExBuf[pos+2],
				*(word*)(&ExBuf[pos+3]));
			pos+=5;
			break;
		case 5://create building
			CreateBuilding(ExBuf[pos],*(int*)(&ExBuf[pos+1]),*(int*)(&ExBuf[pos+5]),
				*(word*)(&ExBuf[pos+9]),ExBuf[pos+11]);
			pos+=12;
			break;
		case 6://produce object
			ProduceObject(ExBuf[pos],*(word*)(&ExBuf[pos+1]));
			pos+=3;
			break;
		case 7://member selection
			MemSelection(ExBuf[pos],ExBuf[pos+1]);
			pos+=2;
			break;
		case 8://rememhfber selection
			RememSelection(ExBuf[pos],ExBuf[pos+1]);
			pos+=2;
			break;
		case 9://build or repair object
			BuildWithSelected(ExBuf[pos],*(word*)(&ExBuf[pos+1]),ExBuf[pos+3]);
			pos+=4;
			break;
		case 10:
			//pos+=CreateWall(ExBuf[pos],&ExBuf[pos+1]);
			break;
		case 11:
			pos+=6;
			break;
		case 12:
			pos+=3;
			break;
		case 13:
			TakeRes(ExBuf[pos],*(int*)(&ExBuf[pos+1]),*(int*)(&ExBuf[pos+5]),ExBuf[pos+9]);
			pos+=10;
			break;
		case 14:
			PerformUpgr(ExBuf[pos],*(word*)(&ExBuf[pos+1]));
			pos+=3;
			break;
		case 15:
			//GetOil(ExBuf[pos],*(word*)(&ExBuf[pos+1]));
			pos+=3;
			break;
		case 16://Create kind selection
			//CreateKindSelection(ExBuf[pos],ExBuf[pos+1],ExBuf[pos+2],
			//	ExBuf[pos+3],ExBuf[pos+4],ExBuf[pos+5]);
			pos+=6;
			break;
		case 17://Create type selection
			//CreateTypeSelection(ExBuf[pos],ExBuf[pos+1],ExBuf[pos+2],
			//	ExBuf[pos+3],ExBuf[pos+4],*(word*)(&ExBuf[pos+5]));
			pos+=7;
			break;
		case 21://set destination
			SetDestination(ExBuf[pos],*(int*)(&ExBuf[pos+1]),*(int*)(&ExBuf[pos+5]));
			pos+=9;
			break;
		case 22://send to point
			//SendToPoint(ExBuf[pos],ExBuf[pos+1],ExBuf[pos+2]);
			pos+=3;
			break;
		case 23://Send group to (X,Y)
			AttackToXY(ExBuf[pos],ExBuf[pos+1],ExBuf[pos+2]);
			pos+=3;
			break;
		case 24://STOP
			Stopp(ExBuf[pos]);
			pos++;
			break;
		case 25://STAND GROUND
			StandGround(ExBuf[pos]);
			pos++;
			break;
		case 26://Patrol
			PatrolGroup(ExBuf[pos],*((int*)(ExBuf+pos+1)),*((int*)(ExBuf+pos+5)),0);
			pos+=9;
			break;
		case 28://
			break;
		case 29://Complex attack point
			GroupAttackPoint(ExBuf[pos],ExBuf[pos+1],ExBuf[pos+2],ExBuf[pos+3]);
			pos+=4;
			break;
		case 30://Complex attack object
			//GroupComplexAttack(ExBuf[pos],*(word*)(&ExBuf[pos+1]),ExBuf[pos+3]);
			pos+=4;
			break;
		case 31://GoToTransport
			SendToTransp(ExBuf[pos],*(word*)(&ExBuf[pos+1]));
			pos+=3;
			break;
		case 32://Unload Transport
			UnloadTransp(ExBuf[pos],ExBuf[pos+1],ExBuf[pos+2]);
			pos+=3;
			break;
		case 33://Die selected
			DieSelected(ExBuf[pos]);
			pos++;
			break;
		case 34://attack point
			ContinueAttPoint(ExBuf[pos],ExBuf[pos+1],ExBuf[pos+2]);
			pos+=3;
			break;
		case 35://attack point
			ContinueAttWall(ExBuf[pos],ExBuf[pos+1],ExBuf[pos+2]);
			pos+=3;
			break;
		case 36://sit down
			SitDown(ExBuf[pos]);
			pos++;
			break;
		case 37:
			UseNuclo(ExBuf[pos],ExBuf[pos+1],ExBuf[pos+2]);
			pos+=3;
			break;
		case 38://produce object
			UnProduceObject(ExBuf[pos],*(word*)(&ExBuf[pos+1]));
			pos+=3;
			break;
		case 39:
			SetRepairState(ExBuf[pos],ExBuf[pos+1]);
			pos+=2;
			break;
		case 40://save network game
			len=ExBuf[pos+5];
			memcpy(sss,&ExBuf[pos+6],len);
			sss[len]=0;
			SaveNetworkGame(ExBuf[pos],*(int*)(&ExBuf[pos+1]),sss);
			pos+=6+len;
			break;
		case 41://load network game
			len=ExBuf[pos+5];
			memcpy(sss,&ExBuf[pos+6],len);
			sss[len]=0;
			LoadNetworkGame(ExBuf[pos],*(int*)(&ExBuf[pos+1]),sss);
			StartTmtmt=tmtmt;
			tmtmt=0;
			REALTIME=0;
			rpos=0;
			pos+=len+6;
			break;
		case 42://select type
			ChooseSelectType(ExBuf[pos],*(word*)(&ExBuf[pos+1]));
			pos+=3;
			break;
		case 43://select type
			ChooseUnSelectType(ExBuf[pos],*(word*)(&ExBuf[pos+1]));
			pos+=3;
			break;
		case 44:
			CreateNewTerrMons(ExBuf[pos],*(int*)(&ExBuf[pos+1]),*(int*)(&ExBuf[pos+5]),*(word*)(&ExBuf[pos+9]));
			pos+=11;
			break;
		case 45:
			GoToMineWithSelected(ExBuf[pos],*(word*)(&ExBuf[pos+1]));
			pos+=3;
			break;
		case 46:
			LeaveMineWithSelected(ExBuf[pos],*(word*)(&ExBuf[pos+1]));
			pos+=3;
			break;
		case 47:
			pos+=CreateWall(ExBuf[pos+2],&ExBuf[pos]);
			break;
		case 48:
			GoToTransportWithSelected(ExBuf[pos],*(word*)(&ExBuf[pos+1]));
			pos+=3;
			break;
		case 49:
			//CreateGatesFromSelected(ExBuf[pos]);
			pos++;
			break;
		case 50:
			pos++;
			break;
		case 51:
			pos++;
			break;
		case 52:
			SelectAllBuildings(ExBuf[pos]);
			pos++;
			break;
		case 53:
			SelectBrig(ExBuf[pos],*(word*)(&ExBuf[pos+1]));
			pos+=3;
			break;
		case 54:
			UnSelectBrig(ExBuf[pos],*(word*)(&ExBuf[pos+1]));
			pos+=3;
			break;
		case 55:
			EraseBrigs(ExBuf[pos]);
			pos++;
			break;
		case 56:
			MakeStandGround(ExBuf[pos]);
			pos++;
			break;
		case 57:
			CancelStandGround(ExBuf[pos]);
			pos++;
			break;
		case 58:
			CreateBrOrder(ExBuf[pos],*(int*)(&ExBuf[pos+1]));
			pos+=5;
			break;
		case 59:
			SelBrigade(ExBuf[pos],ExBuf[pos+1],GSets.CGame.cgi_NatRefTBL[ExBuf[pos]],*(word*)(&ExBuf[pos+2]));
			pos+=4;
			break;
		case 60:
			PerformTorg(ExBuf[pos],ExBuf[pos+1],ExBuf[pos+2],*(int*)(&ExBuf[pos+3]));
			pos+=7;
			break;
		case 61:
			CreateFieldsBar(ExBuf[pos],*(word*)(&ExBuf[pos+1]));
			pos+=3;
			break;
		case 62:
			SetSearchVictim(ExBuf[pos],ExBuf[pos+1]);
			pos+=2;
			break;
		case 63:
			StopUpg(ExBuf[pos],*(int*)(&ExBuf[pos+1]));
			pos+=5;
			break;
		case 64:
			SelUnloadAll(ExBuf[pos]);
			pos++;
			break;
		case 65:
			MakeReformation(ExBuf[pos],*(word*)(&ExBuf[pos+1]),ExBuf[pos+3]);
			pos+=4;
			break;
		case 66:
			SelectIdlePeasants(ExBuf[pos]);
			pos++;
			break;
		case 67:
			SelectIdleMines(ExBuf[pos]);
			pos++;
			break;
		case 68:
			ChGSpeed();
			pos++;
			break;
		case 69:
			SetGSpeed(*((word*)(ExBuf+pos)));
			pos+=2;
			break;
		case 70:
			ChangePauseState();
			HISPEED=0;
			pos++;
			break;
		case 71:
			FreeSelected(ExBuf[pos]);
			pos++;
			break;
		case 72:
			SelectUnitsTypes(ExBuf[pos],1,0);
			pos++;
			break;
		case 73:
			SelectUnitsTypes(ExBuf[pos],1,3);
			pos++;
			break;
	    case 113:
			SelectUnitsTypes(ExBuf[pos],1,4);
			pos++;
			break;
	    case 114:
			SelectUnitsTypes(ExBuf[pos],1,2);
			pos++;
			break;
	    case 115:
			SelectAllPeasants(ExBuf[pos],1);
			pos++;
			break;
	    case 116:
			SelectUnitsTypes(ExBuf[pos],1,1);
			pos++;
			break;
		case 74:
			__EndGame(ExBuf[pos],ExBuf[pos+1]);
			HISPEED=0;
			pos+=2;
			break;
		case 75:
			DoCmdMoney(ExBuf[pos]);
			pos++;
			break;
		case 76:
			DoCmdGiveMoney(ExBuf[pos],ExBuf[pos+1],ExBuf[pos+2],*((int*)(ExBuf+pos+3)));
			pos+=7;
			break;
		case 77:
			__DoStartTime((int*)(ExBuf+pos+1));
			pos+=33;
			break;
		case 78:
			ProduceObjects(ExBuf[pos],ExBuf[pos+1],*(word*)(&ExBuf[pos+2]));
			pos+=4;
			break;
		case 79:
			SETPING(ExBuf[pos],*((word*)(ExBuf+pos+1)));
			pos+=3;
			break;
		case 80:
			DoItSlow(*((word*)(ExBuf+pos)));
			//HISPEED=0;
			pos+=2;
			break;
		case 81:
			DoTimeStep(*((int*)(ExBuf+pos)));
			pos+=4;
			break;
		case 82:
			DoFillFormation(ExBuf[pos],*((word*)(ExBuf+pos+1)));
			pos+=3;
			break;
		case 83:
			ComOfferVoting(*((DWORD*)(ExBuf+pos+1)));
			pos+=5;
			break;
		case 84:
			ComDoVote(*((DWORD*)(ExBuf+pos+1)),ExBuf[pos+5]);
			pos+=6;
			break;
		case 85:
			ComEndPT();
			pos++;
			break;
		case 86:
			ComSetGuardState(ExBuf[pos],*((word*)(ExBuf+pos+1)));
			pos+=3;
			break;
		case 87:
			ComAttackGround(ExBuf[pos],*((int*)(ExBuf+pos+1)),*((int*)(ExBuf+pos+5)));
			pos+=9;
			break;
		case 88:
			GroupSelectedFormations(ExBuf[pos]);
			pos++;
			break;
		case 89:
			UnGroupSelectedUnits(ExBuf[pos]);
			pos++;
			break;
		case 90:
			ChangeNatRefTBL(ExBuf[pos],ExBuf+pos+1);
			pos+=9;
			break;
		case 91:
			ComChangePeaceTimeStage(*((word*)(ExBuf+pos)));
			pos+=2;
			break;
		case 92:
			__EndGame(ExBuf[pos],ExBuf[pos+1]);
			HISPEED=0;
			pos+=3;
			break;
		case 93:
			ComSendData(ExBuf+pos+3,*((word*)(ExBuf+pos+1)));
			pos+=3+*((word*)(ExBuf+pos+1));
			break;
		case 94:
			SetShotMyUnitsState(ExBuf[pos],ExBuf[pos+1]);
			pos+=2;
			break;
		case 95:
			SetArmAttackState(ExBuf[pos],ExBuf[pos+1]);
			pos+=2;
			break;
		case 96:
			DoCmdAddMoney(ExBuf[pos],*((int*)(ExBuf+pos+1)));
			pos+=5;
			break;
		case 97:
			SetStopBuildState(ExBuf[pos],ExBuf[pos+1]);
			pos+=2;
			break;
		case 98:
			ComKartech(ExBuf[pos]);
			pos++;
			break;
		case 99:
			ComPreKartech(ExBuf[pos]);
			pos++;
			break;
		case 100:
			SetFishingState(ExBuf[pos],ExBuf[pos+1]);
			pos+=2;
			break;
		case 101:
			SetAutozasevState(ExBuf[pos],ExBuf[pos+1]);
			pos+=2;
			break;
		case 102:
			ComChangeNPID(ExBuf[pos],*((word*)(ExBuf+pos+1)));
			pos+=3;
			break;
		case 103:
			//ComShotLine(ExBuf[pos],ExBuf[pos+1],ExBuf[pos+2],*((word*)(ExBuf+pos+3)));
			ComShotLine(ExBuf[pos],ExBuf[pos+1],ExBuf[pos+2]);
			pos+=5;
			break;
		case 104:
			ComMakeOneStep(ExBuf[pos],ExBuf[pos+1]);
			pos+=2;
			break;
		case 105:
			ComRotateFormations(ExBuf[pos],ExBuf[pos+1]);
			pos+=2;
			break;
		case  106:
			ComConnectObjs(ExBuf[pos],*((word*)(ExBuf+pos+1)),*((word*)(ExBuf+pos+3)));
			pos+=5;
			break;
		case 107:
			ComThrowGrenade(ExBuf[pos],*((word*)(ExBuf+pos+1)));
			pos+=3;
			break;
		case 108:
			UseAbility(ExBuf[pos],*((word*)(ExBuf+pos+1)),*((word*)(ExBuf+pos+3)));
			pos+=5;
			break;
		case 109:
			AddAbilityToBrigade(ExBuf[pos],*((word*)(ExBuf+pos+1)),*((word*)(ExBuf+pos+3)));
			pos+=5;
			break;
		case 110:
			DelAbility(ExBuf[pos],*((word*)(ExBuf+pos+1)),*((word*)(ExBuf+pos+3)));
			pos+=5;
			break;
		case 111:
			ComDisconnectObjs(ExBuf[pos]);
			pos++;
			break;
		case 112:
			pos+=ComSendPureData(ExBuf+pos);
			break;
		case 200:
			pos+=_implSelBrigade(ExBuf+pos);
			break;
		case 201:
			pos+=_implUnSelBrigade(ExBuf+pos);
			break;
		case 202:
			pos+=_implUnSelUnitsSet(ExBuf+pos);
			break;
		case 203:
			pos+=_implSelUnitsSet_OLD(ExBuf+pos);
			break;
		case 204:
			pos+=_implSelUnitsSet(ExBuf+pos);
			break;
		};
	};
	//Rept("%6d:END\n",pos);
	//RunPF(17);
	InitEBuf();
//	memset(NPresence,0,VAL_MAXCX*VAL_MAXCX);
//    int ofs;
////	StopPF(17);
//	//RunPF(18);
//	for(int i=0;i<MAXOBJECT;i++){
//		OneObject* OB=Group[i];
//        if(OB){
//            ofs=((OB->RealY>>11)<<VAL_SHFCX)+(OB->RealX>>11);
//            if(ofs>=0&&ofs<VAL_MAXCIOFS)NPresence[ofs]|=1<<OB->NNUM;
//        };
//	};
//	StopPF(18);
	NeedCurrentTime+=MaxDCT;
	for(i=0;i<MaxDCT;i++)rando();

	//ADDGR(6,GetTickCount(),GetTickCount()-NeedCurrentTime,0xD4);
	extern bool HideBorderMode;
	if(PlayGameMode==1||HideBorderMode){
		for(int i=0;i<MAXOBJECT;i++){
			OneObject* OB=Group[i];
			if(OB)OB->ImSelected=0;
		};
		/*
		if(Selm[MyNation]){
			int N=NSL[MyNation];
			ImSelm[MyNation]=(word*)realloc(ImSelm[MyNation],N<<1);
			ImSerN[MyNation]=(word*)realloc(ImSerN[MyNation],N<<1);
			ImNSL[MyNation]=NSL[MyNation];
			memcpy(ImSelm[MyNation],Selm[MyNation],N<<1);
			memcpy(ImSerN[MyNation],SerN[MyNation],N<<1);
			for(int j=0;j<N;j++){
				word MID=ImSelm[MyNation][j];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB)OB->ImSelected=1;
				};
			};
		}else{
		*/
		if(ImSelm[MyNation]){
			ImNSL[MyNation]=0;
			free(ImSelm[MyNation]);
			free(ImSerN[MyNation]);
			ImSelm[MyNation]=NULL;
			ImSerN[MyNation]=NULL;
		};
	};
	//if(!SlowMade)CurrentStepTime-=CurrentStepTime>>5;
};
/*
void SetNextCenter(){
	if(!NSL[MyNation])return;
	if(SelCenter[MyNation]>=NSL[MyNation]){
		SelCenter[MyNation]=0;
	};
	int N=NSL[MyNation];
	int I=SelCenter[MyNation];
	word* Smon=Selm[MyNation];
	word MID=
	word MID=Smon[I];
	
	OneObject* OB=Group[
};
*/
void CmdSelBrigade(byte NI,word ID){
	SCMD;
	ExBuf[EBPos]=200;
	ExBuf[EBPos+1]=NI;
	*((word*)(ExBuf+EBPos+2))=ID;
	EBPos+=4;
	ECMD;
};
int _implSelBrigade(byte* Ptr){
	byte NI=Ptr[0];
	word ID=*((word*)(Ptr+1));
	Brigade* BR=CITY[NI].Brigs+ID;
	word* MEM=BR->Memb;
	int N=BR->NMemb;
	int NAdd=0;
	for(int i=NBPERSONAL;i<N;i++){
		word MID=MEM[i];
		if(MID!=0xFFFF&&Group[MID])NAdd++;
	};
	Selm[NI]=(word*)realloc(Selm[NI],(NAdd+NSL[NI])<<1);
	SerN[NI]=(word*)realloc(Selm[NI],(NAdd+NSL[NI])<<1);
	int NS=NSL[NI];
	for(i=NBPERSONAL;i<N;i++){
		word MID=MEM[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB){
				OB->Illusion=0;
				Selm[NI][NS]=MID;
				SerN[NI][NS]=OB->Serial;
				NS++;
			};
		};
	};
	NSL[NI]=NS;
	addrand(NSL[NI]+1);
	return 3;
};
void CmdUnSelBrigade(byte NI,word ID){
	SCMD;
	ExBuf[EBPos]=201;
	ExBuf[EBPos+1]=NI;
	*((word*)(ExBuf+EBPos+2))=ID;
	EBPos+=4;
	ECMD;
};
int _implUnSelBrigade(byte* Ptr){
	byte NI=Ptr[0];
	word ID=*((word*)(Ptr+1));
	word* SU=Selm[NI];
	word* SN=SerN[NI];
	int N=NSL[NI];
	int N1=0;
	for(int i=0;i<N;i++){
		word MID=SU[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(!(OB&&OB->BrigadeID==ID/*&&OB->NNUM==GSets.CGame.cgi_NatRefTBL[NI]*/)){
				OB->Illusion=0;
				SU[N1]=MID;
				SN[N1]=SN[i];
				N1++;
			};
		};
	};
	NSL[NI]=N1;
	addrand(NSL[NI]+1);
	if(N&&!N1){
		free(Selm[NI]);
		Selm[NI]=NULL;
		free(SerN[NI]);
		SerN[NI]=NULL;
	};
	Brigade* BR=CITY[NI].Brigs+ID;
	SU=BR->Memb;
	SN=BR->MembSN;
	N=BR->NMemb;
	for(i=0;i<N;i++){
		word MID=SU[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
		};
	};
	return 3;
};
void CmdUnSelUnitsSet(byte NI,word* BUF,int NU){
	SCMD;
	ExBuf[EBPos]=202;
	ExBuf[EBPos+1]=NI;
	*((word*)(ExBuf+EBPos+2))=word(NU);
	memcpy(ExBuf+EBPos+4,BUF,NU<<1);
	EBPos+=4+NU*2;
	ECMD;
};
int _implUnSelUnitsSet(byte* Ptr){
	byte NI=Ptr[0];
	word N=*((word*)(Ptr+1));
	word* Units=(word*)(Ptr+3);
	for(int i=0;i<N;i++){
		word MID=Units[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB){
				OB->NNUM+=32;
				OB->Selected=0;
			};
		};
	};
	word* SU=Selm[NI];
	word* SN=SerN[NI];
	int N2=NSL[NI];
	int N1=0;
	for(i=0;i<N2;i++){
		word MID=SU[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(!(OB&&OB->NNUM>=32)){
				OB->Illusion=0;
				SU[N1]=MID;
				SN[N1]=SN[i];
				N1++;
			};
		};
	};
	NSL[NI]=N1;
	addrand(NSL[NI]+1);
	if(N&&!N1){
		if(Selm[NI])free(Selm[NI]);
		Selm[NI]=NULL;
		if(SerN[NI])free(SerN[NI]);
		SerN[NI]=NULL;
	}
	for(i=0;i<N;i++){
		word MID=Units[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->NNUM>=32){
				OB->Illusion=0;
				OB->NNUM-=32;
			};
		};
	};
	//assert(!(NSL[NI]&&!Selm[NI]));
	return 3+N*2;
};
int PACKSET(word* Set,int N,byte* outbuf,bool Write){
	byte BASESET[8192];
	memset(BASESET,0,8192);
	for(int i=0;i<N;i++){
		int p=Set[i];
		if(p<ULIMIT)BASESET[p>>3]|=1<<(p&7);
	};
	byte LEVEL2[128*8];
	memset(LEVEL2,0,128*8);
	int N2=0;
	for(i=0;i<1024*8;i++){
		if(BASESET[i]){
			LEVEL2[i>>3]|=1<<(i&7);
			N2++;
		};
	};
	byte LEVEL1[16*8];
	memset(LEVEL1,0,16*8);
	int N1=0;
	for(i=0;i<128*8;i++){
		if(LEVEL2[i]){
			LEVEL1[i>>3]|=1<<(i&7);
			N1++;
		};
	};
	byte LEVEL0[16];
	memset(LEVEL0,0,16);
	int N0=0;
	for(i=0;i<16*8;i++){
		if(LEVEL1[i]){
			LEVEL0[i>>3]|=1<<(i&7);
			N0++;
		};
	};
	word LEVELX=0;
	int NX=0;
	for(i=0;i<16;i++){
		if(LEVEL0[i]){
			LEVELX|=1<<i;
			NX++;
		};
	};
	if(!Write)return 2+NX+N0+N1+N2;
	memcpy(outbuf,&LEVELX,2);
	int pos=2;
	for(i=0;i<16;i++){
		if(LEVEL0[i]){
			outbuf[pos]=LEVEL0[i];
			pos++;
		};
	};
	for(i=0;i<16*8;i++){
		if(LEVEL1[i]){
			outbuf[pos]=LEVEL1[i];
			pos++;
		};
	};
	for(i=0;i<128*8;i++){
		if(LEVEL2[i]){
			outbuf[pos]=LEVEL2[i];
			pos++;
		};
	};
	for(i=0;i<1024*8;i++){
		if(BASESET[i]){
			outbuf[pos]=BASESET[i];
			pos++;
		};
	};
	return pos;
};
int UNPACKSET(byte* Data,int* L,word* Out,bool Write){
	byte BASESET[1024*8];
	memset(BASESET,0,1024*8);
	byte LEVEL2[128*8];
	memset(LEVEL2,0,128*8);
	byte LEVEL1[16*8];
	memset(LEVEL1,0,16*8);
	byte LEVEL0[16];
	memset(LEVEL0,0,8*2);
	word LEVELX=((word*)Data)[0];
	word msk=1;
	int L0pos=0;
	int L1pos=0;
	int L2pos=0;
	int L3pos=0;
	int N0=0;
	int N1=0;
	int N2=0;
	int N3=0;
	byte* CBF=Data+2;
	for(int i=0;i<16;i++){
		if(LEVELX&msk)N0++;
		msk<<=1;
	};
	for(i=0;i<N0;i++){
		byte ms=1;
		byte cb=CBF[i];
		for(int j=0;j<8;j++){
			if(cb&ms)N1++;
			ms<<=1;
		};
	};
	CBF+=N0;
	for(i=0;i<N1;i++){
		byte ms=1;
		byte cb=CBF[i];
		for(int j=0;j<8;j++){
			if(cb&ms)N2++;
			ms<<=1;
		};
	};
	CBF+=N1;
	for(i=0;i<N2;i++){
		byte ms=1;
		byte cb=CBF[i];
		for(int j=0;j<8;j++){
			if(cb&ms)N3++;
			ms<<=1;
		};
	};
	byte* L0BUF=Data+2;
	byte* L1BUF=Data+2+N0;
	byte* L2BUF=Data+2+N0+N1;
	byte* L3BUF=Data+2+N0+N1+N2;
	msk=1;
	for(i=0;i<16;i++){
		if(LEVELX&msk){
			byte b0=LEVEL0[i]=L0BUF[L0pos];
			L0pos++;
			byte m0=1;
			for(int h=0;h<8;h++){
				if(b0&m0){
					byte b=LEVEL1[i]=L1BUF[L1pos];
					L1pos++;
					byte m=1;
					for(int j=0;j<8;j++){
						if(b&m){
							byte b1=LEVEL2[(i<<3)+j]=L2BUF[L2pos];
							L2pos++;
							byte m1=1;
							for(int k=0;k<8;k++){
								if(b1&m1){
									BASESET[k+(j<<3)+(h<<6)+(i<<9)]=L3BUF[L3pos];
									L3pos++;
								};
								m1<<=1;
							};
						};
						m<<=1;
					};
				};
				m0<<=1;
			};
		};
		msk<<=1;
	};
	int sz=0;
	for(i=0;i<1024*8;i++){
		if(BASESET[i]){
			msk=1;
			byte b=BASESET[i];
			for(int j=0;j<8;j++){
				if(b&msk){
					if(Write){
						Out[sz]=j+(i<<3);
					};
					sz++;
				};
				msk<<=1;
			};
		};
	};
	*L=2+N0+N1+N2+L3pos;
	return sz;
};
int PREVSEUSETPOS=-1;
int PREVSEUSETPOSTART=-1;
int PREVNAT=-1;
word PREVSTREAM[2048];
int SUSTREAMSZ=0;
void CmdSelUnitsSet(byte NI,word* BUF,int NU){
	if(EBPos==PREVSEUSETPOS&&SUSTREAMSZ+NU<2048&&PREVNAT==NI){
		memcpy(PREVSTREAM+SUSTREAMSZ,BUF,NU+NU);
		SUSTREAMSZ+=NU;
		EBPos=PREVSEUSETPOSTART;
		SCMD;
		int sz=PACKSET(PREVSTREAM,SUSTREAMSZ,ExBuf+EBPos+2,1);
		EBPos+=2+sz;
		PREVSEUSETPOS=EBPos;
		PREVNAT=NI;
		ECMD;
	}else{
		SCMD;
		ExBuf[EBPos]=204;
		ExBuf[EBPos+1]=NI+32;
		int sz=PACKSET(BUF,NU,ExBuf+EBPos+2,1);
		PREVSEUSETPOSTART=EBPos;
		EBPos+=2+sz;
		PREVSEUSETPOS=EBPos;
		PREVNAT=NI;
		SUSTREAMSZ=NU;
		memcpy(PREVSTREAM,BUF,NU+NU);
		ECMD;
	};
	
	/*
	*((word*)(ExBuf+EBPos+2))=word(NU);
	memcpy(ExBuf+EBPos+4,BUF,NU<<1);
	*/
	//EBPos+=4+NU*2;
};
int _implSelUnitsSet(byte* Ptr){
	byte NI=Ptr[0];
	word N;
	word* Units;
	word* tempbuf=NULL;
	int sz=0;
	if(NI>=32){
		N=UNPACKSET(Ptr+1,&sz,NULL,0);
		tempbuf=znew(word,N);
		UNPACKSET(Ptr+1,&sz,tempbuf,1);
		Units=tempbuf;
		NI&=15;
	}else{
		N=*((word*)(Ptr+1));
		Units=(word*)(Ptr+3);
	};
	int N1=0;
	for(int i=0;i<N;i++){
		OneObject* OB=Group[Units[i]];
		if(OB/*&&OB->NNUM==GSets.CGame.cgi_NatRefTBL[NI]*/)N1++;
	};
	int N0=NSL[NI];
	Selm[NI]=(word*)realloc(Selm[NI],(N0+N1)*2);
	SerN[NI]=(word*)realloc(SerN[NI],(N0+N1)*2);
	N1=N0;
	for(i=0;i<N;i++){
		OneObject* OB=Group[Units[i]];
		if(OB/*&&OB->NNUM==GSets.CGame.cgi_NatRefTBL[NI]*/){
			OB->Illusion=0;
			Selm[NI][N1]=OB->Index;
			SerN[NI][N1]=OB->Serial;
			OB->Selected|=GM(NI);
			N1++;
		};
	};
	NSL[NI]=N1;
	addrand(NSL[NI]+1);
	//CorrectBrigadesSelection(NI);
	if(tempbuf){
		free(tempbuf);
		return 1+sz;
	};
	return 3+N*2;
};
void ImReSmartProcessor(byte NI){
	/*
	int NIm=ImNSL[NI];
	int NRe=NSL[NI];
	word* ReUnits=Selm[NI];
	word* ImUnits=ImSelm[NI];
	word NUS=0;
	word USBUF[2048];
	word NS=0;
	word SBUF[2048];
	for(int i=0;i<NRe;i++){
		word MID=ReUnits[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&!(OB->Sdoxlo||OB->MadeUnSel)){
				if(OB->BrigadeID!=0xFFFF&&OB->InArmy){
					Brigade* BR=OB->Nat->CITY->Brigs+OB->BrigadeID;
					word* MEM=BR->Memb;
					//word* MSN=BR->MembSN;
					int NUB=BR->NMemb;
					for(int j=2;j<NUB;j++){
						word BMID=MEM[j];
						if(BMID!=0xFFFF){
							OneObject* BOB=Group[BMID];
							if(BOB)BOB->MadeUnSel=1;
						};
					};
					CmdUnSelBrigade(NI,OB->BrigadeID);
				}else{
					if(NUS<1024){
						USBUF[NUS]=MID;
						NUS++;
						OB->MadeUnSel=1;
					};
				};
			};
		};
	};
	for(i=0;i<NIm;i++){
		word MID=ImUnits[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&!(OB->Sdoxlo||OB->MadeSel)){
				if(OB->BrigadeID!=0xFFFF&&OB->InArmy){
					Brigade* BR=OB->Nat->CITY->Brigs+OB->BrigadeID;
					word* MEM=BR->Memb;
					//word* MSN=BR->MembSN;
					int NUB=BR->NMemb;
					for(int j=2;j<NUB;j++){
						word BMID=MEM[j];
						if(BMID!=0xFFFF){
							OneObject* BOB=Group[BMID];
							if(BOB)BOB->MadeSel=1;
						};
					};
					CmdSelBrigade(NI,OB->BrigadeID);
				}else{
					if(NS<1024){
						SBUF[NUS]=MID;
						NS++;
						OB->MadeSel=1;
					};
				};
			};
		};
	};
	if(NUS){
		CmdUnSelUnitsSet(NI,USBUF,NUS);
	};
	if(NS){
		CmdSelUnitsSet(NI,SBUF,NS);
	};*/
};
void SmartSelectionCorrector(byte NI,word* M0,word* SN0,int N0){
	word* M1=ImSelm[NI];
	word* SN1=ImSerN[NI];
	word NADD=0;
	word NSUB=0;
	word SUBM[2048];
	word ADDM[2048];

	int N1=ImNSL[NI];
	for(int i=0;i<N0;i++){
		word MID=M0[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&!(OB->Sdoxlo||OB->notSelectable()))OB->NNUM|=32;
		};
	};
	for(i=0;i<N1;i++){
		word MID=M1[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&!(OB->Sdoxlo||OB->notSelectable())){
				OB->NNUM|=64;
				if(NADD<2048&&!(OB->NNUM&32)){
					ADDM[NADD]=OB->Index;
					NADD++;
				};
			};
		};
	};
	for(i=0;i<N0;i++){
		word MID=M0[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&!(OB->Sdoxlo||OB->notSelectable())){
				if(NSUB<2048&&!(OB->NNUM&64)){
					SUBM[NSUB]=OB->Index;
					NSUB++;
				};
			};
		};
	};
	if(NSUB||NADD){
		if(NSUB+NADD>N1){
			CmdFreeSelected(NI);
			if(N1)CmdSelUnitsSet(NI,M1,N1);
		}else{
			if(NSUB)CmdUnSelUnitsSet(NI,SUBM,NSUB);
			if(NADD)CmdSelUnitsSet(NI,ADDM,NADD);
		};
	};
	//if(NADD>COUNTER)COUNTER=NADD;
	//if(NSUB>COUNTER)COUNTER=NSUB;
	for(i=0;i<N0;i++){
		word MID=M0[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&!(OB->Sdoxlo||OB->notSelectable()))OB->NNUM&=15;
		};
	};
	for(i=0;i<N1;i++){
		word MID=M1[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&!(OB->Sdoxlo||OB->notSelectable()))OB->NNUM&=15;
		};
	};
};
//-----------------OLD UNITS COMPRESSION----------------
int UNPACKSET_OLD(byte* Data,int* L,word* Out,bool Write){
	byte BASESET[1024];
	memset(BASESET,0,1024);
	byte LEVEL2[128];
	memset(LEVEL2,0,128);
	byte LEVEL1[16];
	memset(LEVEL1,0,16);
	word LEVEL0=*((word*)(Data));
	word msk=1;
	int L1pos=0;
	int L2pos=0;
	int L3pos=0;
	int N1=0;
	int N2=0;
	int N3=0;
	byte* CBF=Data+2;
	for(int i=0;i<16;i++){
		if(LEVEL0&msk)N1++;
		msk<<=1;
	};
	for(i=0;i<N1;i++){
		byte ms=1;
		byte cb=CBF[i];
		for(int j=0;j<8;j++){
			if(cb&ms)N2++;
			ms<<=1;
		};
	};
	CBF+=N1;
	for(i=0;i<N2;i++){
		byte ms=1;
		byte cb=CBF[i];
		for(int j=0;j<8;j++){
			if(cb&ms)N3++;
			ms<<=1;
		};
	};
	byte* L1BUF=Data+2;
	byte* L2BUF=Data+2+N1;
	byte* L3BUF=Data+2+N1+N2;
	msk=1;
	for(i=0;i<16;i++){
		if(LEVEL0&msk){
			byte b=LEVEL1[i]=L1BUF[L1pos];
			L1pos++;
			byte m=1;
			for(int j=0;j<8;j++){
				if(b&m){
					byte b1=LEVEL2[(i<<3)+j]=L2BUF[L2pos];
					L2pos++;
					byte m1=1;
					for(int k=0;k<8;k++){
						if(b1&m1){
							BASESET[k+(j<<3)+(i<<6)]=L3BUF[L3pos];
							L3pos++;
						};
						m1<<=1;
					};
				};
				m<<=1;
			};
		};
		msk<<=1;
	};
	int sz=0;
	for(i=0;i<1024;i++){
		if(BASESET[i]){
			msk=1;
			byte b=BASESET[i];
			for(int j=0;j<8;j++){
				if(b&msk){
					if(Write){
						Out[sz]=j+(i<<3);
					};
					sz++;
				};
				msk<<=1;
			};
		};
	};
	*L=2+N1+N2+L3pos;
	return sz;
};
int _implSelUnitsSet_OLD(byte* Ptr){
	byte NI=Ptr[0];
	word N;
	word* Units;
	word* tempbuf=NULL;
	int sz=0;
	if(NI>=32){
		N=UNPACKSET_OLD(Ptr+1,&sz,NULL,0);
		tempbuf=znew(word,N);
		UNPACKSET_OLD(Ptr+1,&sz,tempbuf,1);
		Units=tempbuf;
		NI&=15;
	}else{
		N=*((word*)(Ptr+1));
		Units=(word*)(Ptr+3);
	};
	int N1=0;
	for(int i=0;i<N;i++){
		OneObject* OB=Group[Units[i]];
		if(OB/*&&OB->NNUM==GSets.CGame.cgi_NatRefTBL[NI]*/)N1++;
	};
	int N0=NSL[NI];
	Selm[NI]=(word*)realloc(Selm[NI],(N0+N1)*2);
	SerN[NI]=(word*)realloc(SerN[NI],(N0+N1)*2);
	N1=N0;
	for(i=0;i<N;i++){
		OneObject* OB=Group[Units[i]];
		if(OB/*&&OB->NNUM==GSets.CGame.cgi_NatRefTBL[NI]*/){
			Selm[NI][N1]=OB->Index;
			SerN[NI][N1]=OB->Serial;
			OB->Selected|=GM(NI);
			N1++;
		};
	};
	NSL[NI]=N1;
	addrand(NSL[NI]+1);
	//CorrectBrigadesSelection(NI);
	if(tempbuf){
		free(tempbuf);
		return 1+sz;
	};
	return 3+N*2;
};