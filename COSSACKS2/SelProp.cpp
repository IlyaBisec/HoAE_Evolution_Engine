/* Properties of the selected objects viewering and *
 * corresponding mouse handling                     */
#include "stdheader.h"
void EconomyInterface(IconSet* ISET);
void ClearEconomy();
extern int UNGROUP_ICON;
extern int DOGROUP_ICON;
extern int GroupIconPos;
bool CheckGroupPossibility(byte NI);
bool CheckUnGroupPossibility(byte NI);
extern byte   WeaponFlags[32];
extern int MessageUX;
extern int MessageUY;
extern int SGenIcon;
extern int PanelY;
extern int IconLx;
void GetMonCost(byte NI,word NIndex,char* st);
void AssignHintLo(char* s,int time);
extern short WeaponIcn [32];
extern short ProtectIcn[32];
char* GetSprResourceName(OneObject* OB);
#define MaxO 200
void AssignHint1(char* s,int time);
bool SelSoundDobe;
bool SelSoundReady;
int SelSoundType;
int CreateRZone(int x,int y,int lx,int ly,HandlePro* HPro,HandlePro* RHPro,int Index,char* Hint);
int PrpX;
int PrpY;
int PrpNx;
int PrpNy;
int AblX;
int AblY;
int AblNx;
int AblNy;
static bool PropClean=false;
static bool AbltClean=false;
int IconLx;
int IconLy;
int AddIconLx;
int AddIconLy;
extern short FearIcons[NFEARSUBJ];
extern char* FearTxt[NFEARSUBJ];
extern int FILLFORM_ICON;
extern int ZasevIconX;
extern int ZasevIconY;
extern int ZasevIconID;
int FillF_Pos=12;
struct MonInf{
	GeneralObject* RF;
	int Life;
	int MaxLife;
	int Magic;
	int MaxMag;
	int N;
	int Pause;
	int MaxPause;
	word Last;
	word ID;
};
struct AblInf{
	Nation* NT;
	word OInd;
	word Kind;
	word UPIND;
	char HotKey;
	char IconIndex;
};
MonInf MList[MaxO];
AblInf AList[MaxO];
word ArmList[MaxO];
int GetProgress(word ID,int* MaxProgress);
int GetAmount(word ID);
int SubIcon;
static GeneralObject* LastGO;
static Nation* LastNT;
static word LastAmount;
static word LastID;
extern int curptr;
extern int curdx;
extern int curdy;
//uniq properties
bool GetCoord;
UniqMethood* UNIM;
void InitPrpBar(){
	PrpNx=4;
	PrpNy=2;
	PrpX=10;
	PrpY=(msy>>1)+miniy+16;
	AblX=PrpX;
	AblY=PrpY+PrpNy*(IconLy+2)+16;
	AblNx=PrpNx;
	AblNy=6;
	SubIcon=-1;
};
void ResFon(int x,int y,int Lx,int Ly){

};
void ScrCopy(int x,int y,int Lx,int Ly){
	if(!bActive)return;
	//if(DDebug){
//
//		return;
//	};
	int x1=(x&(65535-3));
	int lx=((x+Lx+4)&(65535-3))-x1;
	int StDy=RSCRSizeX-lx;
	int FONDy=SCRSizeX-lx;
	int DsOf=int(RealScreenPtr)+x1+y*RSCRSizeX;
	int SrOf=int(ScreenPtr)+x1+y*SCRSizeX;
	int lx1=lx>>2;
	__asm{
		push	esi
		push	edi
		cld
		mov		eax,Ly
		mov		esi,SrOf
		mov		edi,DsOf
		mov		ebx,StDy
jj1:	mov		ecx,lx1
		rep		movsd
		add		esi,FONDy
		add		edi,ebx
		dec		eax	
		jnz		jj1
	};
};
int NINF,NABL,NARMINF;
void GetUnitCost(byte NI,word NIndex,int* Cost,int x,int y);
extern int LastMx;
extern int LastMy;
void GetMonCost(byte NI,word NIndex,char* st){
	Nation* NT=&NATIONS[NI];
	GeneralObject* GO=NT->Mon[NIndex];
	char uu[128];
	bool AddSp=false;
	uu[0]=0;
	AdvCharacter* AC=GO->MoreCharacter;
	int Cost[8];
	GetUnitCost(NI,NIndex,Cost,LastMx,LastMy);
	if(!EngSettings.Resource.EnableSprHint){
		strcat(st," - ");
		for(int i=0;i<8;i++){
			if(AC->NeedRes[i]){
				//if(AddSp)strcat(st," -");
				if(Cost[i]>XRESRC(NI,i)){
					sprintf(uu,"{CR}%s: %d{C} ",RDS[i].Name,Cost[i]);
				}else{
					sprintf(uu,"%s: %d ",RDS[i].Name,Cost[i]);
				}			
				strcat(st,uu);
				AddSp=true;
			};
		};
	}else{
		_str txt;
		for(int i=0;i<8;i++){
			if(AC->NeedRes[i]==0){
				Cost[i]=0;
			}
		}
		EngSettings.Resource.SetHint(NI,Cost,&txt);
		strcat(st,txt.pchar());
	}
};
void GeneralObject::GetMonsterCostString(char* st){
	st[0]=0;
};
void Nation::GetUpgradeCostString(char* st,word UI){
	char uu[128];
	bool AddSp=false;
	for(int i=0;i<8;i++){
		if(UPGRADE[UI]->Cost[i]){
			if(AddSp)strcat(st," ");
			sprintf(uu,"%s:%d",RDS[i].Name,int(UPGRADE[UI]->Cost[i]));
			strcat(st,uu);
			AddSp=true;
		};
	};
};
word SELSET[32];
int NSelSet=0;
void HPSEL(int i){
	if(GetKeyState(VK_CONTROL)&0x8000){
		CmdChooseUnSelType(MyNation,i);
	}else{
		if(GetKeyState(VK_SHIFT)&0x8000){
			if(NSelSet<32){
				SELSET[NSelSet]=i;
				NSelSet++;
			};
		}else{
			CmdChooseSelType(MyNation,i);
		};
	};
};
void HPSELBRIG(int i){
	if(GetKeyState(VK_CONTROL)&0x8000){
		CmdChooseUnSelBrig(MyNation,i);
	}else{
		if(GetKeyState(VK_SHIFT)&0x8000){
			if(NSelSet<32){
				SELSET[NSelSet]=i+32768;
				NSelSet++;
			};
		}else{
			CmdChooseSelBrig(MyNation,i);
		};
	};
};
bool IsItInSelSet(word ID){
	for(int i=0;i<NSelSet;i++)if(SELSET[i]==ID)return true;
	return false;
};
void FREEORD(int i){
	CmdErseBrigs(MyNation);
};
void MSTANDGR(int i){
	CmdMakeStandGround(MyNation);
};
void CSTANDGR(int i){
	CmdCancelStandGround(MyNation);
};
extern int Zone1X;
extern int Zone1Y;
extern int Zone1LX;
extern int Zone1LY;
extern int BrigPnX;
extern int BrigPnY;
extern int BrigNx;
extern int BrigNy;
extern int PreviewBrig;
bool FullBrigMode=false;
void OLIHPRO(int i){
	FullBrigMode=!FullBrigMode;
};
void SELBRIG(int i){
	if(GetKeyState(VK_SHIFT)&0x8000){
		CmdSelBrig(MyNation,1,i);
	}else{
		CmdSelBrig(MyNation,0,i);
		//FullBrigMode=false;
	};
};
void CmdSetSrVictim(byte NI,byte val);
void PreBrig(int i){
	PreviewBrig=i;
};
void EATTACK_Pro(int i){
	CmdSetSrVictim(MyNation,0);
};
void DATTACK_Pro(int i){
	CmdSetSrVictim(MyNation,1);
};

void CmdSetShotMyUnitsState(byte NI,byte val);
void CmdSetArmAttackState(byte NI,byte val);
void CmdSetStopBuildState(byte NI,byte val);
void CmdKartech(byte NI);
void CmdPreKartech(byte NI);
void CmdFishing(byte NI,byte val);
void CmdAutozasev(byte NI,byte val);
void SHOTMYUNITS_Pro(int i){
	CmdSetShotMyUnitsState(MyNation,1);
};
void KEEPMYUNITS_Pro(int i){
	CmdSetShotMyUnitsState(MyNation,0);
};
void ARMATTACK_Pro(int i){
	CmdSetArmAttackState(MyNation,1);
};
#ifdef SIMPLEMANAGE
void SETATTSTATE_Pro(int i){
	CmdSetArmAttackState(MyNation,i);
};
#endif //SIMPLEMANAGE
void ANYATTACK_Pro(int i){
	CmdSetArmAttackState(MyNation,0);
};
void STOPBUILD_Pro(int i){
	CmdSetStopBuildState(MyNation,i);
};
void KARTECH_Pro(int i){
	CmdKartech(MyNation);
};
void PREKARTECH_Pro(int i){
	CmdPreKartech(MyNation);
};
void FISHING_Pro(int i){
	CmdFishing(MyNation,i);
};
void ZASEV_Pro(int i){
	CmdAutozasev(MyNation,i);
};
void CmdShotLine(byte NI,int i);
void ENBLINE(int i){
	CmdShotLine(MyNation,i);
};
void CmdThrowGrenade(byte,int);
void ThrowGrenade(int i){
	CmdThrowGrenade(GSets.CGame.cgi_NatRefTBL[MyNation],i);

};
extern int PATROL_ICON;
extern bool PATROLMODE;
void DOPATROL(int i){
	PATROLMODE=1;
};
void PATROL_Pro(int i){
};
void GOATTA_Pro(int i){
};
void CmdMakeReformation(byte,word,byte);
void REFORMA(int i){
	CmdMakeReformation(MyNation,i&65535,i>>16);
};
CEXPORT void CmdUseAbility(byte NI,word BrigadeID,word AblType);
void UseAbil(int i){
	CmdUseAbility(MyNation,i&8191,i/8192);
};
CEXPORT void CmdDelAbility(byte NI,word BrigadeID,word AblType);
void DelAbil(int i){
	CmdDelAbility(MyNation,i&8191,i/8192);
};
void CmdFillFormation(byte NI,word BrigadeID);
void FILLFORM(int i){
	CmdFillFormation(MyNation,i);
};
bool GoAndAttackMode=0;
void GONATTACK(int i){
	GoAndAttackMode=1;
};
extern City CITY[8];
extern int GOAT_Icon;
extern int GOAT_PX;
extern int GOAT_PY;
extern int HISTORY_Icon;
void ShowHistory();
void HISTORY_Call(int i){
	ShowHistory();
};
extern bool GoAndAttackMode;
extern char** NatNames;
char* GetTextByID(char* ID);
extern int STOP_ICON;
bool BreefUInfo=0;
extern int ATTGR_ICON;
extern int AttGrPos;
void ATTGR_PRO(int p);
void CmdDoGroup(byte);
void CmdUnGroup(byte);
void COM_DOGROUP(int i){
	CmdDoGroup(MyNation);
};
void COM_UNGROUP(int i){
	CmdUnGroup(MyNation);
};
int GetPanicProbability(OneObject* OB,int FearID);
char* FEAR_TXT[6]={NULL,NULL,NULL,NULL,NULL,NULL};
char* STOPBUILD_OFF_TXT=NULL;
char* STOPBUILD_ON_TXT=NULL;
extern int STOPBUILD_OFF;
extern int STOPBUILD_ON;
extern int STOPBUILD_X;
extern int STOPBUILD_Y;
extern int KARTECH_ICON;
extern int KARTECH_X;
extern int KARTECH_Y;
extern int PREKARTECH_ICON;
extern int PREKARTECH_X;
extern int PREKARTECH_Y;
extern int FISHING_ON_ICON;
extern int FISHING_OFF_ICON;
extern int FISHING_X;
extern int FISHING_Y;

char* KARTECH_TXT=NULL;
char* PREKARTECH_TXT=NULL;
char* FISHING_ON_TXT=NULL;
char* FISHING_OFF_TXT=NULL;
char* DONTMAKEZASEV=NULL;
char* MAKEZASEV=NULL;
char* NOARMATTACK=NULL;
char* NORIFLEATTACK=NULL;
char* RIFLEATTACK=NULL;
#define IC_DISTX 55
#define IC_ADISTX 55
#define IC_DISTY 19
byte FCLR[8]={39,39,126,126,194,194,208,208};
int FEARMAX=30;
int GetFScaled(int x){
 	int v=50+atan(float(FEARMAX)*log(float(x)/30.0)/60.0)*31.83;
	if(v<0)v=0;
	if(v>100)v=100;
	return v;
};
extern int HintTime;
extern char HintStrLo[256];
extern bool WasLoHint;
#ifdef GETTIRED
void CBar(int x,int y,int Lx,int Ly,byte c);
void ShowTiringInfo(Brigade* BR,int x,int y,int LX,int LY);
void ShowTiringInfo(OneObject* OB,int x,int y,int LX,int LY){
	if(OB->BrigadeID!=0xFFFF){
		ShowTiringInfo(CITY[OB->NNUM].Brigs+OB->BrigadeID,x,y,LX,LY);
		return;
	};
	x+=20;
	y+=39;
	Xbar(x-1,y-1,LX+2,LY+2,clrYello);
	LX=(LX*(OB->GetTired/100))/1000;
	CBar(x,y,LX,LY,clrGreen);
};
void ShowTiringInfo(Brigade* BR,int x,int y,int LX,int LY){
	x+=20;
	y+=39;
	word* IDS=BR->Memb;
	word* SNS=BR->MembSN;
	int N=BR->NMemb;
	int T=0;
	int NT=0;
	//int MINT=100*10000;
	for(int i=3;i<N;i++){
		word MID=IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]){
				T+=OB->GetTired/1000;
				NT++;
				//if(MINT>OB->GetTired)MINT=OB->GetTired;
			};
		};
	};
	if(NT){
		T/=NT;
		Xbar(x-1,y-1,LX+2,LY+2,clrYello);
		LX=(LX*T)/100;
		CBar(x,y,LX,LY,clrGreen);
	};
};
#else //GETTIRED
#define ShowTiringInfo(OB,x,y,LX,LY)
#endif //GETTIRED
bool AllowArt=0;
bool RejectInterface=0;
typedef bool SubstInterfFunc(word IDX);
SubstInterfFunc* CheckInterface=NULL;
int DrawUInterface(int x,int y,int Lx0,int Ly0,int x1,int y1,int UIndex);
int DrawUnitsInterface();
int InterfSubst=0;
void ShowProp(){
	if(!(GSets.CGame.ViewMask&4))return;
	InterfSubst=0;
	int CurNat=GSets.CGame.cgi_NatRefTBL[MyNation];
	bool ReadOnly=0;
	InterfSubst=DrawUnitsInterface();
	void ShowBrigGlobalInterface();
	ShowBrigGlobalInterface();
	if(InterfSubst==4){
		return;
	};	
};

int GetRLCStrWidth(char* str,lpRLCFont lpf);
extern byte PlayGameMode;
char* ARTCAPS[8]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
extern bool HideBorderMode;
void ShowTextDiscription(){
	if(PlayGameMode==1||HideBorderMode)return;
	RunPF(4,"ShowTextDescription");
	StopPF(4);
    if(NINF==1){
		//Xbar(Zone1X,PanelY+Zone1Y+1,Zone1LX,Zone1LY,0xD0+(MyNation<<2));
		//Xbar(Zone1X-1,PanelY+Zone1Y,Zone1LX+2,Zone1LY+2,0xD0+(MyNation<<2)+1);
		MonInf* MI=&(MList[0]);
		OneObject* OBJ=Group[MI->Last];
		if(!OBJ)return;
		RunPF(4,"ShowTextDescription");
		if(OBJ->Kills&&!(HintTime&&WasLoHint)){
			char str[128];
			sprintf(str,KILLUNI,OBJ->Kills);//"Lñøû:%d"
			ShowString(Zone1X+10,PanelY+Zone1Y-20,str,&SmallWhiteFont);
		};
		if(OBJ->Ready){
			int DYY=-7;
			char str[128];
			int x1=Zone1X+IconLx+4+6-4;
			int y1=PanelY+Zone1Y+6+DYY;
			AdvCharacter* ADC=OBJ->MoreCharacter;
			NewMonster* NM=OBJ->newMons;
			int NN=0;

			int EFFICACY=1000;
			if(NM->ShotPtZ){
				int NS=0;
				int MAXS=OBJ->MoreCharacter->MaxInside;
				for(int i=0;i<OBJ->NInside;i++){
					word MID=OBJ->Inside[i];
					if(MID!=0xFFFF){
						OneObject* IOB=Group[MID];
						if(IOB){
							byte USE=IOB->newMons->Usage;
							if(USE==StrelokID||USE==HorseStrelokID||USE==ArcherID)NS++;
						};
					};
				};
				if(MAXS){
					EFFICACY=(NS*1000)/MAXS;
				};
			};
			byte skm=NM->SkillDamageMask;
			for(int i=0;i<NAttTypes;i++){
				if(ADC->MaxDamage[i]&&(!(WeaponFlags[NM->WeaponKind[i]]&64))&&NN<2){
					GPS.ShowGP(x1+NN*IC_ADISTX+1,y1+1,1,WeaponIcn[ADC->WeaponKind[i]],0);
					int DMIN=NM->MinDamage[i];
					int dm=ADC->MaxDamage[i];
					
					dm=DMIN+(((dm-DMIN)*EFFICACY)/1000);
					int adm=0;
					if(skm&(1<<i)){
						adm=(int(OBJ->Kills)*int(NM->SkillDamageBonus))/10;
					};

					//dm=(dm+(dm>>1))>>1;
					if(OBJ->AddDamage+adm>0)sprintf(str,"%d+%d",dm,OBJ->AddDamage+adm);
					else if(OBJ->AddDamage+adm<0)sprintf(str,"%d-%d",dm,-OBJ->AddDamage+adm);
					else sprintf(str,"%d",dm);
					ShowString(x1+NN*IC_ADISTX+IC_DISTY,y1+5,str,&SmallWhiteFont);
					NN++;
				};
			};
			sprintf(str,"%d",int(OBJ->Life));
			int LLL=GetRLCStrWidth(str,&SmallWhiteFont);
			ShowString(Zone1X+(IconLx-LLL>>1)+6,PanelY+Zone1Y+IconLx+1+6+DYY,str,&SmallWhiteFont);
			y1+=21;
			GPS.ShowGP(x1+1,y1+1,1,SGenIcon,0);
			if(OBJ->AddShield>0)sprintf(str,"%d+%d",ADC->Shield,OBJ->AddShield);
			else if(OBJ->AddShield<0)sprintf(str,"%d-%d",ADC->Shield,-OBJ->AddShield);
			else sprintf(str,"%d",ADC->Shield);
			ShowString(x1+21,y1+12+DYY,str,&SmallWhiteFont);
			y1=PanelY+Zone1Y+IconLx+10+9+DYY-2;
			x1=Zone1X+4;
			NN=0;
			if(!NM->Building){
				for(i=0;i<16;i++){
					if(ADC->Protection[i]&&NN<6){
						int ix=x1+(NN>>1)*IC_DISTX;
						int iy=y1+(NN&1)*IC_DISTY;
						GPS.ShowGP(ix+3,iy+1,1,ProtectIcn[i],0);
						int dm=ADC->Protection[i];
						//  ;-)  //dm=((dm+1)*100)>>8;
						sprintf(str,"%d",dm);
						ShowString(ix+22,iy+5,str,&SmallWhiteFont);
						NN++;
					};
				};
			};
			int xf=x1;
			int yf=y1+IC_DISTY*2;
			int nf=0;
			ShowTiringInfo(OBJ,x1,y1+36-8,120,4);
#ifdef NEWMORALE
			void DrawNewMoraleInterface(int x,int y,OneObject* OB);
			if(!OBJ->newMons->NoMorale)DrawNewMoraleInterface(x1,y1+36-8,OBJ);
#else //NEWMORALE
#ifdef COSSACKS2
				int FFY0=y1+36;
				if(!FEAR_TXT[0]){
					FEAR_TXT[0]=GetTextByID("FEAR_TXT0");
					FEAR_TXT[1]=GetTextByID("FEAR_TXT1");
					FEAR_TXT[2]=GetTextByID("FEAR_TXT2");
					FEAR_TXT[3]=GetTextByID("FEAR_TXT3");
					FEAR_TXT[4]=GetTextByID("FEAR_TXT4");
					FEAR_TXT[5]=GetTextByID("FEAR_TXT5");
				};
				FFY0+=5;
				for(i=0;i<NFEARSUBJ;i++){
					int F=GetPanicProbability(OBJ,i)*1000/32768;
					int V=GetFScaled(F);
					int DLX=77;
					int L=DLX*V/100;
					int c=FCLR[V*8/101];
					void CBar(int x,int y,int Lx,int Ly,byte c);
					void Xbar(int x,int y,int lx,int ly,byte c);
					int x0=x1+4+(i%2)*(DLX+4);
					int y0=FFY0+(i/2)*15;
					for(int j=0;j<8;j++){
						if((j+1)*DLX/8<DLX-L)CBar(x0+j*DLX/8,y0,(DLX*(j+1)/8)-(DLX*j/8),13,FCLR[7-j]);
						else break;
						
					};
					CBar(x0+j*DLX/8,y0,DLX-L-1-(DLX*j/8),13,FCLR[7-j]);

					sprintf(str,"%s",FEAR_TXT[i]);
					ShowString(x1+4+(i%2)*(DLX+4),FFY0+(i/2)*15,str,&fn10);
					//FFY0+=12;
				};
#endif //COSSACKS2
#endif //NEWMORALE
			/*
			for(i=0;i<NFEARSUBJ;i++)if(FearIcons[i]!=-1&&NM->FearFactor[i]){
				int F=(GetPanicProbability(OBJ,i)+1)*1000/32768;
				sprintf(str,"%d.%d%%",FEAR_TXT[i],F/10,F%10);
				GPS.ShowGP(xf+3,yf,1,FearIcons[i],0);
				ShowString(xf+23,yf+3,str,&SmallWhiteFont);
				xf+=IC_DISTX;
				nf++;
				if(nf>2){
					xf=x1;
					yf+=IC_DISTY;
					nf=0;
				};
			};
			int FFY0=y1+36;
			if(!FEAR_TXT[0]){
				FEAR_TXT[0]=GetTextByID("FEAR_TXT0");
				FEAR_TXT[1]=GetTextByID("FEAR_TXT1");
				FEAR_TXT[2]=GetTextByID("FEAR_TXT2");
				FEAR_TXT[3]=GetTextByID("FEAR_TXT3");
				FEAR_TXT[4]=GetTextByID("FEAR_TXT4");
				FEAR_TXT[5]=GetTextByID("FEAR_TXT5");
			};
			for(i=0;i<NFEARSUBJ;i++)if(NM->FearFactor[i]){
				int F=(GetPanicProbability(OBJ,i)+1)*1000/32768;
				sprintf(str,"%s: %d.%d%%",FEAR_TXT[i],F/10,F%10);
				ShowString(x1+2,FFY0,str,&fn10);
				FFY0+=12;
			};
			*/
			if(NM->FishAmount){
				sprintf(str,FISHSTR,OBJ->Ustage,ADC->FishAmount);//"¦vñð:%d/%d"
				ShowString(x1+3,y1,str,&fn10);
				y1+=12;
			};
			if(NM->Farm){
				Nation* NT=OBJ->Nat;
				/*
				City* CT=NT->CITY;
				sprintf(str,"T¸õóþ ¯õ¨ü:%d",CT->ReadyAmount[OBJ->NIndex]);
				ShowString(x1,y1,str,&fn10);
				y1+=12;
				sprintf(str,"=ºöýþ:%d",div(NT->NGidot,UnitsPerFarm).quot+1);
				ShowString(x1,y1,str,&fn8);
				y1+=12;
				*/
				sprintf(str,HABITAN,NT->NGidot);//"+ñøªðªõûõù:%d"
				ShowString(x1+3,y1,str,&fn10);
				y1+=12;
				sprintf(str,MAXHABI,NT->NFarms);//"¦ðú¸.þñøªðªõûõù:%d"
				ShowString(x1+3,y1,str,&fn10);
				y1+=12;

			};
			if(NM->ArtDepo){
				if(!ARTCAPS[0]){
					ARTCAPS[0]=GetTextByID("__ARTCAP0");
					ARTCAPS[1]=GetTextByID("__ARTCAP2");
					ARTCAPS[2]=GetTextByID("__ARTCAP1");
					ARTCAPS[3]=GetTextByID("__ARTCAP3");
				};
				Nation* NT=OBJ->Nat;
				for(int i=0;i<4;i++){
					if(ARTCAPS[i][0]!='_'&&NM->ArtCap[i]){
						sprintf(str,ARTCAPS[i],NT->NArtUnits[i],NM->ArtCap[i]*NT->NArtdep[i]);//"+ñøªðªõûõù:%d"
						ShowString(x1+3,y1,str,&fn10);
						y1+=12;
					};
				};

			};
			if(ADC->MaxInside){
#ifdef COSSACKS2
				int NINS=OBJ->NInside;
				int NMAX=OBJ->MoreCharacter->MaxInside+OBJ->AddInside;
				if(NINS>NMAX){
					sprintf(str,INSISTR,NMAX);
					sprintf(str+strlen(str),"+%d",NINS-NMAX);
					ShowString(x1+3,y1,str,&SmallRedFont);
				}else{
					sprintf(str,INSISTR,NINS);
					ShowString(x1+3,y1,str,&fn10);
				};
#else
				sprintf(str,INSISTR,OBJ->NInside);
				ShowString(x1+3,y1,str,&fn10);
#endif
				y1+=12;
				if(OBJ->AddInside){
					sprintf(str,MAXINSI,int(ADC->MaxInside),OBJ->AddInside);//"¦ðú¸.Týºª¨ø:%d+%d"
					ShowString(x1+3,y1,str,&fn10);
					y1+=12;
				}else{
					sprintf(str,MAXINS1,int(ADC->MaxInside));//"¦ðú¸.Týºª¨ø:%d"
					ShowString(x1+3,y1,str,&fn10);
					y1+=12;
				};
				char* RNAME=GetSprResourceName(OBJ);
				if(RNAME){
					ShowString(x1+3,y1,RNAME,&fn10);
					y1+=12;
				};
			};
			/*
			if(OBJ->Repair){
				ShowString(x1,y1,"¦õüþýªýøú",&fn10);
				y1+=16;
			};
			*/
			if(OBJ->newMons->ShowDelay){
				int L=0;
				int LMAX=(Zone1LY+25);
				if(MI->MaxPause){
					L=div(LMAX*MI->Pause,MI->MaxPause).quot;
				}; 
				if(L<LMAX){
					Vline(Zone1X+Zone1LX-5,PanelY+Zone1Y+1+L,PanelY+Zone1Y+LMAX-2,clrGreen);
					Vline(Zone1X+Zone1LX-4,PanelY+Zone1Y+1+L,PanelY+Zone1Y+LMAX-2,clrGreen);
					Vline(Zone1X+Zone1LX-3,PanelY+Zone1Y+1+L,PanelY+Zone1Y+LMAX-2,clrGreen);
					Vline(Zone1X+Zone1LX-2,PanelY+Zone1Y+1+L,PanelY+Zone1Y+LMAX-2,clrGreen);
					//Vline(Zone1X/*+Zone1LX-1*/+3,PanelY+Zone1Y+1+L,PanelY+Zone1Y+Zone1LY-2,clrGreen);
					//Vline(Zone1X/*+Zone1LX-1*/+4,PanelY+Zone1Y+1+L,PanelY+Zone1Y+Zone1LY-2,clrGreen);
				};
			};
		};
		StopPF(4);
	};
//	StopPF(35);
};
extern bool BuildMode;
extern OneSlide* OSB;
extern byte blx;
extern byte bly;
extern word BuildingID;
extern Nation* BNat;
void CmdUnProduceObj(byte NI,word Type);
void RHPR(int i){
	AblInf* AI=&AList[i];
	Nation* NT=AI->NT;
	word j=AI->OInd;
	GeneralObject* GO=NT->Mon[j];
	if(!GO->newMons->Building){
		if(GetKeyState(VK_SHIFT)&0x8000){
			for(int n=0;n<20;n++){
				CmdUnProduceObj(MyNation,j);
			}
			//CmdUnProduceObj(MyNation,j);
			//CmdUnProduceObj(MyNation,j);
			//CmdUnProduceObj(MyNation,j);
			//CmdUnProduceObj(MyNation,j);
		}else CmdUnProduceObj(MyNation,j);
	};
};
void CmdFieldBar(byte NI,word n);
bool CheckCostHint(byte NI,word NIndex);
int GetTotalUnits();
extern bool NOPAUSE;
void HPR(int i){
	//if(!NOPAUSE)return;
	if(GetTotalUnits()>LULIMIT){
		AssignHint1(UNILIMIT,50,32);
		return;
	};
	AblInf* AI=&AList[i];
	Nation* NT=AI->NT;
	word j=AI->OInd;
	GeneralObject* GO=NT->Mon[j];
	NewMonster* NM=GO->newMons;
	if(NM->SpriteObject&&!NM->Wall){
		CmdFieldBar(MyNation,j);
		return;
	};
	if(NM->Building||NM->Wall){
		if(NM->Wall||CheckCostHint(GSets.CGame.cgi_NatRefTBL[MyNation],j)){
			BuildMode=true;
			BuildingID=j;
			BNat=NT;
		};
	}else{
		if(CheckCostHint(GSets.CGame.cgi_NatRefTBL[MyNation],j)){
			if(GetKeyState(VK_SHIFT)&0x8000){
				for(int n=0;n<30;n++){
					CmdProduceObj(MyNation,j);
				}				
				//CmdProduceObj(MyNation,j);
				//CmdProduceObj(MyNation,j);
				//CmdProduceObj(MyNation,j);
				//CmdProduceObj(MyNation,j);
			}else CmdProduceObj(MyNation,j);
		};
	}
};
void HPR1(int i){
	//if(!NOPAUSE)return;
	Nation* NT=&NATIONS[MyNation];
	GeneralObject* GO=NT->Mon[i];
	NewMonster* NM=GO->newMons;
	if(NM->Building){
		BuildMode=true;
		BuildingID=i;
		BNat=NT;
	}else{
		if(GetKeyState(VK_SHIFT)&0x8000){
			CmdProduceObj(MyNation,i);
			CmdProduceObj(MyNation,i);
			CmdProduceObj(MyNation,i);
			CmdProduceObj(MyNation,i);
			CmdProduceObj(MyNation,i);
		}else CmdProduceObj(MyNation,i);
	}
};
void DoUPG(int i){
	AblInf* AI=&AList[i];
	Nation* NT=AI->NT;
	word j=AI->OInd;
	CmdPerformUpgrade(MyNation,j);
};
void DoUPG1(int i){
	CmdPerformUpgrade(MyNation,i);
};
void EnterIn(int i){
	if(i==0xFFFF)SubIcon=-1;
	WIcon* IC=NATIONS[MyNation].wIcons[i];
	if(!IC->SubList){
		SubIcon=-1;
		return;
	};
	SubIcon=i;
};
void OrderMove(int i,int x,int y){
	CmdSendToXY(MyNation,x,y,512);
};
void OrderAttack(int i,int x,int y){
	//word MID=GetOwner((x<<5)+16,(y<<5)+16);
	/*if(MID!=0xFFFF){
		OneObject* OB=Group[MID];
		byte Msk=1<<MyNation;
		if(OB->NMask&Msk)CmdAttackToXY(MyNation,x,y);
		else CmdAttackObj(MyNation,MID);
	}else CmdAttackToXY(MyNation,x,y);
	*/
};
void OrderPatrol(int i,int x,int y){
	CmdPatrol(MyNation,x,y);
};
void CmdSetRprState(byte NI,byte State);
void OrderRepair(int i,int x,int y){
	CmdSetRprState(MyNation,1);
};
void OrderNucAtt(int i,int x,int y){
	CmdNucAtt(MyNation,x,y);
};
void OrderSetDst(int i,int x,int y){
	CmdSetDst(MyNation,x,y);
};
void SelPage1(int par){
	NewMonster* NM=(NewMonster*)par;
	NM->CurrPage=0;
};
void SelPage2(int par){
	NewMonster* NM=(NewMonster*)par;
	NM->CurrPage=1;
};
void SelPage3(int par){
	NewMonster* NM=(NewMonster*)par;
	NM->CurrPage=2;
};
word MaxMagic;
extern int MousDX[32];
extern int MousDY[32];
CEXPORT
void SetCurPtr(int v);
void UNIPARAM(int i){
	SetCurPtr(2);
	//curdx=16;
	//curdy=15;
	curdx=MousDX[curptr];
	curdy=MousDY[curptr];
	GetCoord=true;
	UNIM=NULL;
	switch(i){
	case 1://Move to xy
		UNIM=&OrderMove;
		break;
	case 2://Attack near (x,y)
		UNIM=&OrderAttack;
		break;
	case 5://Patrol
		UNIM=&OrderPatrol;
		break;
	case 6://Repair
		UNIM=&OrderRepair;
		break;
	case 7:
		//UNIM=&OrderGetResource;
		break;
	case 8:
		//UNIM=&OrderUnload;
		break;
	case 9:
		//UNIM=&OrderAttPt;
		break;
	case 10:
		UNIM=&OrderNucAtt;
		break;
	case 11:
		UNIM=&OrderSetDst;
		break;
	};
};
void NOPARAM(int i){
	UNIM=NULL;
	switch(i){
	case 3://Stop
		CmdStop(MyNation);
		break;
	case 4://Stand Ground
		CmdStandGround(MyNation);
		break;
	case 5:
		CmdSetRprState(MyNation,1);
		break;
	case 6:
		CmdSetRprState(MyNation,0);
		break;
	case 7://Build wall
		//SetBuildWallMode();
		break;
	};
};
void OrderPointN(int i,int x,int y){
	
};
void OrderPoint0(int i,int x,int y){
	OrderPointN(0,x,y);
};
void OrderPoint1(int i,int x,int y){
	OrderPointN(1,x,y);
};
void OrderPoint2(int i,int x,int y){
	OrderPointN(2,x,y);
};
void OrderPoint3(int i,int x,int y){
	OrderPointN(3,x,y);
};
void OrderPoint4(int i,int x,int y){
	OrderPointN(4,x,y);
};
void OrderPoint5(int i,int x,int y){
	OrderPointN(5,x,y);
};
void OrderPoint6(int i,int x,int y){
	OrderPointN(6,x,y);
};
void OrderPoint7(int i,int x,int y){
	OrderPointN(7,x,y);
};
void OrderPoint8(int i,int x,int y){
	OrderPointN(8,x,y);
};
void OrderPoint9(int i,int x,int y){
	OrderPointN(9,x,y);
};
void OrderPoint10(int i,int x,int y){
	OrderPointN(10,x,y);
};
void OrderPoint11(int i,int x,int y){
	OrderPointN(11,x,y);
};
void STRANGEWEAPON(int i){
	//if(MaxMagic<NATIONS[MyNation].WMagic[i])return;
	SetCurPtr(3);
	//curdx=16;
	//curdy=16;
	curdx=MousDX[curptr];
	curdy=MousDY[curptr];
	GetCoord=true;
	UNIM=NULL;

	switch(i){
	case 0:
		UNIM=&OrderPoint0;
		break;
	case 1:
		UNIM=&OrderPoint1;
		break;
	case 2:
		UNIM=&OrderPoint2;
		break;
	case 3:
		UNIM=&OrderPoint3;
		break;
	case 4:
		UNIM=&OrderPoint4;
		break;
	case 5:
		UNIM=&OrderPoint5;
		break;
	case 6:
		UNIM=&OrderPoint6;
		break;
	case 7:
		UNIM=&OrderPoint7;
		break;
	case 9:
		UNIM=&OrderPoint9;
		break;
	case 10:
		UNIM=&OrderPoint10;
		break;
	};
};
bool CreateInsideList(IconSet* IS,byte NI);
bool ECOSHOW;
extern byte PlayGameMode;
extern bool HideBorderMode;
bool CheckCost(byte NI,word NIndex,int x,int y);
extern byte BaloonState;
extern byte CannonState;
extern byte XVIIIState;
extern bool EditMapMode;
extern int LastMx;
extern int LastMy;

bool PAGE1PRESENT=0;
bool PAGE2PRESENT=0;
bool PAGE3PRESENT=0;
extern NewMonster* LASTNM;

void CmdChangeNPID(byte,word);
extern bool RejectInterface;
void ShowAbility(){	
};
void PropCopy(){
	int AY=NABL>>2;
	if(NABL&3)AY++;
	int PY=NINF>>2;
	if(NINF&3)PY++;
	if(AY>8)AY=0;
	if(PY>8)PY=0;
	AY=4;
	PY=2;
	if(PY)ScrCopy(PrpX,PrpY,PrpNx*(IconLx+AddIconLx),PY*(IconLy+AddIconLy));
	if(AY)ScrCopy(AblX,AblY,AblNx*(IconLx+AddIconLx),AY*(IconLy+AddIconLy));
};
//-------------------MESSAGES-------------------//
void LoadConstStr();
//lpCHAR* GMessIDS=NULL;
//lpCHAR* GMessage=NULL;
//int NMess=0;
//int MaxMess=0;
StringsHash* GMessIDS=NULL;
StringsHash* GMessage=NULL;
DynArray<int>* G_IDS_Index;
DynArray<int>* G_Mess_Index;
void ClearMessages(){
	GMessIDS->clear();
	GMessage->clear();
	delete(GMessIDS);
	delete(GMessage);
	(*G_IDS_Index).Clear();
	(*G_Mess_Index).Clear();
	delete(G_IDS_Index);
	delete(G_Mess_Index);
};
void DosToWin(char*);
void normstr(char* str);
extern bool ProtectionMode;
void LoadMessagesFromFile(char* Name){
	if(!strstr(Name,".dat"))ProtectionMode=1;
	GFILE* f=Gopen(Name,"r");
	ProtectionMode=0;
	AFile(Name);
	if(!f)return;
	int z;//,z1;
	char IDN[4096];
	//char STR[4096];
	DString STR;
	do{
		z=Gscanf(f,"%s",IDN);
		//if(strlen(IDN)>64){
		if(strlen(IDN)>4000){ // made in japan
			//sprintf(STR,"Identifier  too long: %s",IDN);
			STR.print("Identifier  too long: %s",IDN);
			ErrM(STR.pchar());
		}
		if( z==1 ){
			char s=IDN[0];
			bool wrongid=( s==' ' || s=='/' || s=='\\' || s=='-' || s=='*' );
			if(!wrongid)
				Ggetch(f);
			STR.Clear();
			//STR[0]=0;
			int cc=0;
			//z1=0;
			//int nn=0;
			bool nospace=false;
			while(!(cc==0x0A||cc==EOF)){
				cc=Ggetch(f);
				if(!(cc==0x0A||cc==EOF)){
					if(cc>32||nospace){
						//STR[nn]=cc;
						STR.Add(char(cc));
						//nn++;
						nospace=true;
					}
				}else{
					//Ggetch(f);
				};
			};
			//STR[nn]=0;
			if( !wrongid ){
				if(GMessIDS->find(IDN)==-1){
					GMessIDS->add(IDN);	
					int id=GMessIDS->SHash.numElem()-1;
					//DString sTR=STR;
					while(true){
						//int m1=GMessage->find(sTR.pchar());
						int m1=GMessage->find(STR.pchar());
						if(m1==-1){
							//GMessage->add(sTR.pchar());
							GMessage->add(STR.pchar());
							(*G_Mess_Index).Add(id);
							(*G_IDS_Index).Add(GMessage->SHash.numElem()-1);
							break;
						}else{
							//(*G_IDS_Index).Add(m1);
							STR.Add(char(31));
						}
					}
				}else{
					Log.Warning("Duplicate TextID: %s",IDN);
				}
			}
		}
	}while(z==1);
	Gclose(f);
};
void LoadTextFromDir(char* dirname){
	char cc[256];
	sprintf(cc,"%s*.txt",dirname);
	TGSCFindInfo* FD=GSFILES.gFindFirst(cc);
	if(FD){
		//char cc[128];
		sprintf(cc,"%s%s",dirname,FD->m_FileName);
		LoadMessagesFromFile(cc);
		bool good=false;
		do{
			good=GSFILES.gFindNext(FD);
			if(good){
				sprintf(cc,"%s%s",dirname,FD->m_FileName);
				LoadMessagesFromFile(cc);
			};
		}while(good);
		//delete(FD);
	};
}
void LoadMessages(){
	GMessIDS=new StringsHash;
	GMessage=new StringsHash;
	G_IDS_Index=new DynArray<int>;
	G_Mess_Index=new DynArray<int>;
	//
	LoadTextFromDir("Text\\Add2\\");
	LoadTextFromDir("Text\\Add\\");
	//
	LoadMessagesFromFile("Comment.txt");
	TGSCFindInfo* FD=GSFILES.gFindFirst("Missions\\miss_*.txt");
	if(FD){
		char cc[128];
		sprintf(cc,"Missions\\%s",FD->m_FileName);
		LoadMessagesFromFile(cc);
		bool good=false;
		do{
			good=GSFILES.gFindNext(FD);
			if(good){
				sprintf(cc,"Missions\\%s",FD->m_FileName);
				LoadMessagesFromFile(cc);
			};
		}while(good);
		///delete(FD);
	};
	bool ReadFromFile	 (const char* Name);
	//GameSettings GSettings;
	LangSettings.ReadFromFile("Text\\LanguageSettings.xml");
	if(LangSettings.LangIndex==0){
	LoadTextFromDir("Text\\Eng\\");
	LoadTextFromDir("Missions\\Text\\Eng\\");
	};
	if(LangSettings.LangIndex==1){
	LoadTextFromDir("Text\\Rus\\");
	LoadTextFromDir("Missions\\Text\\Rus\\");
	};
	//
	LoadMessagesFromFile("moretx.dat");	
	static char* NRT="NORSTX";
	if(NRT[1]!='O'){
		int NL=0;
		int NR=0;
		for(int i=0;i<GMessage->SHash.numElem();i++){
			char* s=GMessage->SHash.elem(i).str;
			if(s){
				int L=strlen(s);
				for(int i=0;i<L;i++){
					char c=s[i];
					if(c<0)NR++;
					else NL++;
				}
			}
		}
		if(NR>NL){
			for(int i=0;i<GMessage->SHash.numElem();i++){
				char* s=GMessage->SHash.elem(i).str;
				if(s){
					GMessage->SHash.elem(i).str="";
				}
			}
		}
	}
}
bool GetHintDebugMode();
StringsHash UnkIDS;
bool v_DialogsMissed=false;
CEXPORT char* GetTextByID(char* id){
	if(id==NULL||strlen(id)>64) return id;

	static bool IsInit=false;
	if(!IsInit){
		IsInit=true;
		LoadMessages();		
	}
	char* ID=id;
	static char temp[128];
	if(strchr(id,' ')){
        strcpy(temp,id);
		int p=0;
		while(id[p]){
			if(id[p]==' ')temp[p]='_';			
			p++;
		}
		ID=temp;
	}
	int idx=GMessIDS->find(ID);
	if(idx!=-1)return GMessage->get((*G_IDS_Index)[idx]);

	// åñëè òåêñò óæå áûë ëîêàëèçèðîâàí
	idx=GMessage->find(ID);
	if(idx==-1){
		// óæå áûë äîáàâëåí â íåèçâåñòíûå èäåíòèôèêàòîðû
		int id=UnkIDS.find(ID);
		if(id==-1){
			UnkIDS.add(ID);
			if(GetHintDebugMode()){
				Log.Warning("Unknown text id: [%s]",ID);
				static bool initMissed=false;
				if(!initMissed){
					initMissed=true;
					FILE* F;		
					F=fopen("Text\\__AllEditors.missed_text","w"); if(F)fclose(F);
					F=fopen("Text\\__TextIDLoad.missed_text","w"); if(F)fclose(F);
					F=fopen("Text\\__Other.missed_text","w"); if(F)fclose(F);
					F=fopen("Text\\__LoadedID.missed_text","w"); 
					if(F){
						int n=GMessIDS->SHash.numElem();
						for(int i=0;i<n;i++){
							fprintf(F,"%s %s\n",GMessIDS->get(i),GMessage->get(i));
						}
						fclose(F);
					}
				}
				if(ID[0]=='@'&&strstr(ID,"::")){
					FILE* F=fopen("Text\\__AllEditors.missed_text","a");
					if(F){
						fprintf(F,"%s  \n",ID);
						fclose(F);
					}
				}else
				if(v_DialogsMissed){
					FILE* F=fopen("Text\\__TextIDLoad.missed_text","a");
					if(F){
						//_str sid;
						//if(ID[0]=='#'){
						fprintf(F,"%s \n",ID);
						//}else{
						//	sid.print("#_%d %s\n",(int)GetTickCount(),id);
						//	fprintf(F,"%s",sid.pchar());
						//}
						fclose(F);
					}
				}else{
					FILE* F=fopen("Text\\__Other.missed_text","a");
					if(F){
						fprintf(F,"%s\n",ID);
						fclose(F);
					}
				}
			}
		}
		if(!GetHintDebugMode()){
			char ch=ID[0];
			if( ch=='#' || ch=='(' )
				return "";
		}
	}
	return id;
};
int GetTextIDByID(char* ID){
	return (*G_IDS_Index)[GMessIDS->find(ID)];
};
CEXPORT
char* GetIDByText(char* Text){
	if(Text[0]==0)return "";
	int idx=GMessage->find(Text);
	if(idx!=-1)return GMessIDS->get((*G_Mess_Index)[idx]);
	return Text;
}

CEXPORT
void* AddIconToPanel(int x,int y,int FileID,int SpriteID){
	return NULL;
}
CEXPORT
void AddHintToIcon(void* OI,char* Hint){
	((OneIcon*)OI)->CreateHint(Hint);
};
CEXPORT
void AddProcToIcon(void* OI,HandlePro* L,int Lparam,HandlePro* R,int Rparam){
	((OneIcon*)OI)->AssignLeft(L,Lparam);
	((OneIcon*)OI)->AssignRight(R,Rparam);
}
CEXPORT
void AddStageToIcon(void* OI,int Value,int MaxValue){
	((OneIcon*)OI)->AssignStage(Value,MaxValue);
}
CEXPORT
void AddNumberToIcon(void* OI,int Value){
	((OneIcon*)OI)->AssignIntVal(Value);
}
CEXPORT
void AddLevelToIcon(void* OI,int Value){
	((OneIcon*)OI)->AssignLevel(Value);
}