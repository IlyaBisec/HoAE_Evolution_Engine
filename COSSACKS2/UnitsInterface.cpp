#include "stdheader.h"
#include "MapTemplates.h"
#include "UnitsInterface.h"
#include ".\cvi_HeroButtons.h"
#include ".\cvi_MainMenu.h"
#include "vui_Actions.h"

#include "ua_Item.h"
#include "ComplexShooter.h"

void BrigPanelSet(cvs_BrigPanel& BP);
void BrigPanelShowAll();

//
extern int FI_File;
class OneTrigger;
extern OneTrigger VideoSeq;
extern OneTrigger VidOfSeq;
extern int LastNI,LastBID;
void ProcessVideoForBrigade(OneTrigger* OT,byte NI,word BrigID,int Action,int BackGP,int BackSprite,int VideoX,int VideoY, int vdx, int vdy, int PlayerID);
void ACT(int x);
//

#define DIALOGS_USER
//////////////////////////////////////////////////////////////////////////
bool GetPushkaChargeState(OneObject*,int&,int&);
extern byte PlayGameMode;
extern bool EditMapMode;
extern int RealLx;
extern int RealLy;
void CmdChangeNPID(byte,word);
int	GetAmount(word ID);
int GetProgress(word,int*);
extern bool BuildMode;
void GetBrigadeParams(Brigade*,BrigParam*);
DLLEXPORT int GetCurrentUnits(byte NI);
DLLEXPORT int GetMaxUnits(byte NI);
void NewAttackPointLink(OneObject* OBJ);
//////////////////////////////////////////////////////////////////////////

cDialogsHistory DialogsHistory;

//initial params
int FI_X0=0;
int FI_Y0=0;
int FI_File=-1;
int FI_WeapFile=-1;
int FI_ResFile=-1;
int FI_PortBackFile=-1;
int FI_IFile=-1;
int FI_Awards=-1;
int FI_PortretID=0;
int FI_PortretLX;
int FI_PortretLY;
int FI_RifleID=2;
int FI_RifleLX;
int FI_RifleLY;
int FI_SabreID=5;
int FI_SabreLX;
int FI_SabreLY;
int FI_GrenadeX;
int FI_GrenadeID=2;
int FI_GrenadeY;
int FI_GrenadeLX;
int FI_GrenadeLY;
int FI_IconSizeX=42;
int FI_IconSizeY=42;
#define FI_IconLX FI_IconSizeX
#define FI_IconLY FI_IconSizeY

char* FI_NetralName=NULL;

int FI_RifleX;
int FI_RifleY;
int FI_SabreX;
int FI_SabreY;
int FI_BoardX;
int FI_BoardY;

extern City CITY[8];
extern int FillF_Pos;
extern int FILLFORM_ICON;

void FILLFORM(int i);
void DelAbil(int i);
void UseAbil(int i);
void ThrowGrenade(int i);
void ENBLINE(int i);
void REFORMA(int i);
void SETATTSTATE_Pro(int i);
int LastNI,LastBID;
bool UseGrenades=0;

#include "Parser.h"
void ImSelBrigade(byte NI,byte Type,byte BNAT,int ID);
void SelBrigade(byte NI,byte Type,byte BNat,int ID);
extern OneTrigger VideoSeq;
extern OneTrigger VidOfSeq;
void ProcessVideoForBrigade(OneTrigger* OT,byte NI,word BrigID,int Action,int BackGP,int BackSprite,int VideoX,int VideoY, int vdx, int vdy, int PlayerID);

bool ReadOnly=0;
bool SetAtt(SimpleDialog* SD){
	if(ReadOnly)return false;
	SETATTSTATE_Pro(SD->UserParam);
	Lpressed=0;
	switch(SD->UserParam){
		case 1:
			ACT(5);//MELEE
			break;
		case 129:
			ACT(3);//RIFLE
			break;
		case 128:
			ACT(4);//NORIFLE
			break;
	};
	return 0;
}
bool MassSetAtt(SimpleDialog* SD){
	SETATTSTATE_Pro(SD->UserParam);
	Lpressed=0;
	return 0;
}
void ENBLINE(int i);
bool ShotLine(SimpleDialog* SD){
	if(ReadOnly)return false;
	ENBLINE(SD->UserParam);
	Lpressed=0;
	int V=SD->UserParam;
	int L=V&7;
	if(V>=8192){
		ACT(7+L);
	}else{
		ACT(10+L);
	}
	return 0;
}

void ThrowGrenade(int i);
bool Grenade(SimpleDialog* SD){
	if(ReadOnly)return false;
	ThrowGrenade(SD->UserParam);
	return 0;
}
void REFORMA(int i);
bool Reform(SimpleDialog* SD){
	if(ReadOnly)return false;
	REFORMA(SD->UserParam);
	Lpressed=0;
	return 0;
}
void FILLFORM(int i);
bool FillForm(SimpleDialog* SD){
	if(!ReadOnly&&SD->UserParam!=0xFFFF){
		FILLFORM(SD->UserParam);
	}
	Lpressed=0;
	return 0;
}
void FREEORD(int i);
bool FreeForm(SimpleDialog* SD){
	if(ReadOnly)return false;
	FREEORD(SD->UserParam);
	Lpressed=0;
	return 0;
}
void MSTANDGR(int i);
bool StandGround(SimpleDialog* SD){
	if(ReadOnly)return false;
	MSTANDGR(SD->UserParam);
	Lpressed=0;
	return 0;
}
bool NoClick(SimpleDialog* SD){
	if(ReadOnly)return false;
	Lpressed=0;
	return 0;
}
//void CmdChooseSelBrig(byte NI,word ID);
bool ClickOnBrig(SimpleDialog* SD){
	if(ReadOnly)return false;	
	if(GetKeyState(VK_MENU)&0x6000){
		if(SD->Diffuse==0xFFFFFFFF) SD->Diffuse=0xFF808080;
		else SD->Diffuse=0xFFFFFFFF;
	}else
	if(SD->UserParam<0xFFFF){
		if(GetKeyState(VK_SHIFT)&0x6000) CmdChooseUnSelBrig(MyNation,SD->UserParam);
			else CmdChooseSelBrig(MyNation,SD->UserParam);			
	}else{
		word id=SD->UserParam>>16;
		if(GetKeyState(VK_SHIFT)&0x6000) CmdChooseUnSelType(MyNation,id);
			else CmdChooseSelType(MyNation,id);
	}
	Lpressed=0;
	return 0;
}
void UseAbil(int i);
void DelAbil(int i);
bool AblMouseOver(SimpleDialog* SD){
	if(ReadOnly)return false;
	if(Lpressed){
		UseAbil(SD->UserParam);
        Lpressed=0;
	}
	if(Rpressed){
		DelAbil(SD->UserParam);
		Rpressed=0;
	}
	return 0;
}
#define HINT(x,y)\
{\
	static const char* y=GetTextByID(#y);\
	x->Hint=(char*)y;\
}

DialogsSystem SL_Interface;
DWORD CurSelHash=-1;
word CheckIfFormationInterface(byte& NI){
	word BrigID=0xFFFF;
	byte Nat;
	int N=ImNSL[NI];
	word* IDS=ImSelm[NI];
	word* SNS=ImSerN[NI];
	for(int i=0;i<N;i++){
		word MID=IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]&&!OB->Sdoxlo){
				if(OB->BrigadeID==0xFFFF)return 0xFFFF;
				else{
					if(BrigID==0xFFFF){
						BrigID=OB->BrigadeID;
						NI=OB->NNUM;
					}else if(BrigID!=OB->BrigadeID)return 0xFFFF;
				}
			}
		}
	}
	return BrigID;
}
extern int RealLy;
void GetBrigadeParams(Brigade* BR,BrigParam* BP){
	if(BR->WarType==0||!BR->Enabled) return;
	int L=0;
	int N=0;
	int NS=0;
	int T=0;
	BP->RifleAttack=0;
	int Delay=0;
	int MaxDelay=0;
	for(int i=NBPERSONAL;i<BR->NMemb;i++){
		word MID=BR->Memb[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB){
				L+=OB->Life;
				N++;
				if(!OB->delay)NS++;
				T+=OB->GetTired/1000;
				if(OB->RifleAttack)BP->RifleAttack=1;
				Delay+=OB->delay;
				MaxDelay+=OB->MaxDelay;
			}
		}
	}
	if(N){
		L/=N;
		T/=N;
		Delay/=N;
		MaxDelay/=N;
	}
	BP->NLiveMembers=N;
	BP->Life=L;
	if(BR->Morale>=0) BP->Morale=BR->Morale/10000;
		else BP->Morale=0;	
	BP->MaxMorale=BR->MaxMorale/10000;
	BP->MaxLife=NATIONS[BR->CT->NI].Mon[BR->MembID]->MoreCharacter->Life;
	if (BP->MaxLife==0) BP->MaxLife=1;
	BP->NShots=NS;
	BP->NKills=BR->GetBrigExp();
	BP->NGrenades=BR->NGrenades;
	BP->Tiring=T;
	BP->ReadyPercent=MaxDelay?100*(MaxDelay-Delay)/MaxDelay:100;
}
void ShowAString(int x,int y,RLCFont* F,byte Align,char* s,...){
	char ach[256];
    va_list va;

    va_start( va, s );
    vsprintf ( ach, s, va );   
    va_end( va );

	if(Align==1){
		ShowString(x-GetRLCStrWidth(ach,F)/2,y,ach,F);
	}else
	if(Align==2){
		ShowString(x-GetRLCStrWidth(ach,F),y,ach,F);
	}else ShowString(x,y,ach,F);
}
void ShowStringEx(int x, int y, LPCSTR lps, lpRLCFont lpf, bool Horizontal);
void ShowAStringEx(int x,int y,RLCFont* F,byte Align,bool Horizontal,char* s,...){
	char ach[256];
    va_list va;

    va_start( va, s );
    vsprintf ( ach, s, va );   
    va_end( va );

	if(Align==1){
		if(Horizontal){
			x-=GetRLCStrWidth(ach,F)/2;
		}else{
			y+=GetRLCStrWidth(ach,F)/2;
		}
		ShowStringEx(x,y,ach,F,Horizontal);
	}else
	if(Align==2){
		if(Horizontal){
			x-=GetRLCStrWidth(ach,F);
		}else{
			y+=GetRLCStrWidth(ach,F);
		}
		ShowStringEx(x,y,ach,F,Horizontal);
	}else ShowStringEx(x,y,ach,F,Horizontal);
}
void LineRGB(int x,int y,int x1,int y1,DWORD Color);
int scl(int r,int g,int b,int s){
	return 0xFF000000+(((r*s)/100)<<16)+(((g*s)/100)<<8)+((b*s)/100);
}
void DrawVertLine(int x,int y,int L,int R,int G,int B){
	LineRGB(x-1,y,x-1,y-L,scl(R,G,B,60));
	LineRGB(x,y,x,y-L,scl(R,G,B,100));
	LineRGB(x+1,y,x+1,y-L,scl(R,G,B,60));
}
void DrawHorLine(int x,int y,int L,int R,int G,int B){
	LineRGB(x,y-3,x+L,y-3,scl(R,G,B,80));
	LineRGB(x,y-2,x+L,y-2,scl(R,G,B,100));
	LineRGB(x,y-1,x+L,y-1,scl(R,G,B,100));
	LineRGB(x,y  ,x+L,y  ,scl(R,G,B,100));
	LineRGB(x,y+1,x+L,y+1,scl(R,G,B,100));
	LineRGB(x,y+2,x+L,y+2,scl(R,G,B,100));
	LineRGB(x,y+3,x+L,y+3,scl(R,G,B,80));
}
extern byte   WeaponFlags[32];
// global var
void InitGlobalFIVar(){
	if(FI_File==-1){
		FI_File=GPS.PreLoadGPImage("Interf3\\FormInterface");
		if(FI_File!=-1){
			FI_IFile=GPS.PreLoadGPImage("Interf3\\f_icons");
			FI_Awards=GPS.PreLoadGPImage("Interf3\\awards");
			FI_PortretLX=GPS.GetGPWidth(FI_File,FI_PortretID);
			FI_PortretLY=GPS.GetGPHeight(FI_File,FI_PortretID);
            FI_RifleLX=68;//GPS.GetGPWidth(FI_File,FI_RifleID);
			FI_RifleLY=GPS.GetGPHeight(FI_File,FI_RifleID);
			FI_SabreLX=GPS.GetGPWidth(FI_File,FI_SabreID);
			FI_SabreLY=GPS.GetGPHeight(FI_File,FI_SabreID);
			FI_WeapFile=GPS.PreLoadGPImage("Interf3\\BigWeapon");
			FI_ResFile=GPS.PreLoadGPImage("Interf3\\ResPanel");
			FI_PortBackFile=GPS.PreLoadGPImage("Interf3\\PortBackBranch");
			FI_NetralName=GetTextByID("NetralSettlementName");
		}else{
			CEXPORT void VitalError(char*);
			VitalError("UnitsInterface.cpp: InitGlobalFIVar");
		}
	}	
}

extern OISelection OIS;
void ShowStringEx(int x, int y, LPCSTR lps, lpRLCFont lpf, bool Horizontal);
void ShowAStringEx(int x,int y,RLCFont* F,byte Align,bool Horizontal,char* s,...);

#define MAXSEL 20
DialogsSystem DS_Sel[MAXSEL];
DialogsSystem DS_But;

DWORD LastTop=0xFFFFFFFF;
bool UI_ShiftSel=false;

CIMPORT void SetTopSelBrigID(word BID);
void CreateSelInterface(){

	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];

	//RunPF((5<<8)+1,"CreateSelInterface 1");

	InitGlobalFIVar();

	int N=min(OIS.NSelBr+OIS.NSelObj,MAXSEL);
	if(N==0) return;

	bool BrRifle=OIS.RifleAttEnabled;//true;
	BrigParam BP[200];

	word LTID=N-1;
	for(int i=0;i<OIS.NSelBr;i++){
		word bid=OIS.SelBr[i];
		Brigade* BR=CITY[NI].Brigs+bid;		
		GetBrigadeParams(BR,BP+i);
		//if(!BR->WarType||!BP[i].RifleAttack){
		//	BrRifle=false;
		//}
		if(LastTop==bid){
			LTID=i;
		}
	}
	for(i=0;i<OIS.NSelObj;i++){
		if(LastTop==(OIS.SelObj[i]<<16)){
			LTID=OIS.NSelBr+i;
		}
	}

	//StopPF((5<<8)+1);
	//RunPF((5<<8)+2,"CreateSelInterface 2");

	int NewTID=-1;
	
	for(i=0;i<N&&i<LTID;i++){
		DWORD ID;
		if(i<OIS.NSelBr){
			ID=OIS.SelBr[i];
		}else ID=OIS.SelObj[i-OIS.NSelBr]<<16;
	}
	//StopPF((5<<8)+2);
	//RunPF((5<<8)+3,"CreateSelInterface 3");
	
	// find selected GeneralObject
	GeneralObject* TopGO=NULL;
	Brigade* TopBR=NULL;
	i=LTID;
	DWORD ID;
	if(i<OIS.NSelBr){		
		ID=OIS.SelBr[i];
		TopBR=CITY[OIS.SelNation].Brigs+ID;
		if(TopBR->Enabled&&TopBR->WarType){
			TopGO=NATIONS[OIS.SelNation].Mon[TopBR->MembID];
		}			
	}else{
		ID=OIS.SelObj[i-OIS.NSelBr]<<16;
		TopGO=NATIONS[OIS.SelNation].Mon[ID>>16];
	}

	for(i=N-1;i>=LTID;i--){
		DWORD ID;
		if(i<OIS.NSelBr){
			ID=OIS.SelBr[i];
			if(i==LTID) SetTopSelBrigID(ID);
		}else{
			ID=0xFFFF|(OIS.SelObj[i-OIS.NSelBr]<<16);
		}
	}

	if(NewTID!=-1){
		if(NewTID<OIS.NSelBr) LastTop=OIS.SelBr[NewTID];
			else LastTop=OIS.SelObj[NewTID-OIS.NSelBr]<<16;
	}
	//StopPF((5<<8)+3);

	// Select shift mode	
	if((GetKeyState(VK_MENU)&0x6000)){
		if(!UI_ShiftSel){
			UI_ShiftSel=true;
			for(int i=0;i<N;i++) DS_Sel[i].DSS[0]->Diffuse=0xFF808080;
		}
	}else{
		if(UI_ShiftSel){
			UI_ShiftSel=false;
			for(int i=0;i<N;i++) if(DS_Sel[i].DSS[0]->Diffuse==0xFFFFFFFF){
				UI_ShiftSel=true;
				break;
			};
			for(i=0;i<N;i++){
				SimpleDialog* SD=DS_Sel[i].DSS[0];
				if(UI_ShiftSel&&SD->Diffuse!=0xFFFFFFFF){
					if(i<OIS.NSelBr) CmdChooseUnSelBrig(MyNation,OIS.SelBr[i]);
						else CmdChooseUnSelType(MyNation,OIS.SelObj[i-OIS.NSelBr]);
				}
				SD->Diffuse=0xFFFFFFFF;
			}
		
			UI_ShiftSel=false;
		}
	}

	// mass buttons

	GP_Button* RIFLE=(GP_Button*)DS_But.Find(1);
	GP_Button* SABRE=(GP_Button*)DS_But.Find(2);
	GPPicture* RIF  =(GPPicture*)DS_But.Find(3);
	GPPicture* SAB  =(GPPicture*)DS_But.Find(4);

	if(!RIFLE){
		DS_But.BaseY=RealLy;

		RIFLE=DS_But.addGP_Button(NULL,FI_RifleX,FI_RifleY,FI_File,3,2);
		RIFLE->AssignID(1);
		RIFLE->OnUserClick=&MassSetAtt;
		RIFLE->UserParam=129;

		SABRE=DS_But.addGP_Button(NULL,FI_SabreX,FI_SabreY,FI_File,6,5);
		SABRE->AssignID(2);
		SABRE->OnUserClick=&MassSetAtt;
		SABRE->UserParam=1;

		RIF=DS_But.addGPPicture(NULL,FI_RifleX+8,FI_RifleY+20,FI_File,4);
		RIF->AssignID(3);
		
		SAB=DS_But.addGPPicture(NULL,FI_SabreX,FI_SabreY+20,FI_File,7);
		SAB->AssignID(4);
		//SL_Interface.addTextButton(NULL,CenterX,-FI_PortretLY+20,NM->Message,Font,Font,Font,1);
	}

	DS_But.SetHintStyle(0,1,1,30,0,0,34*(N-1)+FI_PortretLX+4,RealLy-FI_PortretLY+17,400,FI_PortretLY-FI_RifleLY-30,&SmallWhiteFont,0x90404040,0);

	if(SABRE){
		SABRE->x=34*(N-1)+FI_PortretLX;
		SABRE->x1=SABRE->x+FI_SabreLX;
		SABRE->y=RealLy+FI_SabreY;
		SABRE->y1=SABRE->y+FI_SabreLY;
		HINT(SABRE,USE_MELEE_ATTACK);
	}
	if(SAB){
		SAB->x=SABRE->x;
		//RIF->x1=RIF->x+FI_RifleLX;
		SAB->y=SABRE->y+20;
		//RIF->y1=RIF->y+FI_RifleLY;
		if(TopGO){
			if(TopGO->newMons->BigWeapFile){
				SAB->SpriteID=TopGO->newMons->BigFireWeapSprite;
				SAB->FileID=TopGO->newMons->BigWeapFile;
			}
		}
	}
	if(RIFLE){
		RIFLE->x=SABRE->x+FI_SabreLX;
		RIFLE->x1=RIFLE->x+FI_RifleLX;
		RIFLE->y=RealLy+FI_RifleY;
		RIFLE->y1=RIFLE->y+FI_RifleLY;
		if(TopBR){
			RIFLE->ActiveFrame=3;
			RIFLE->PassiveFrame=2;
		}else{
			RIFLE->ActiveFrame=6;
			RIFLE->PassiveFrame=5;
		}
		HINT(RIFLE,DISABLE_RIFLE_ATTACK);
		RIFLE->Visible=OIS.RifleAttackAllowed;
	}
	if(RIF){	
		if(BrRifle){
			RIFLE->UserParam=128;
			RIF->Diffuse=0xFFFFFFFF;
			HINT(RIFLE,DISABLE_RIFLE_ATTACK);
		}else{ 
			RIFLE->UserParam=129;
			RIF->Diffuse=0x60FF8080;
			HINT(RIFLE,ENABLE_RIFLE_ATTACK);
		}
		RIF->x=RIFLE->x+8;
		//RIF->x1=RIF->x+FI_RifleLX;
		RIF->y=RIFLE->y+20;
		//RIF->y1=RIF->y+FI_RifleLY;
		if(TopGO){
			if(TopGO->newMons->BigWeapFile){
				SAB->SpriteID=TopGO->newMons->BigColdWeapSprite;
				SAB->FileID=TopGO->newMons->BigWeapFile;
			}
		}
		RIF->Visible=OIS.RifleAttackAllowed;
	}

	if(!TopGO) DS_But.ProcessDialogs();

	// weapon charachters
	if(TopGO){
		RLCFont* Font=&SmallYellowFont;
		AdvCharacter* ADC=TopGO->MoreCharacter;		

		char stxt[128]="";
		char rtxt[128]="";

		for(int i=0;i<2;i++){
			int dm=ADC->MaxDamage[i];
			int wk=ADC->WeaponKind[i];
			int ADM=0;
			char str[128];
			if(TopBR){			
				if(TopGO->newMons->SkillDamageMask&(1<<i)){
					int SC=TopGO->newMons->SkillDamageFormationBonusStep;
					int V;
					if(SC)V=(TopBR->GetBrigExp()/SC)*SC;
					else V=TopBR->GetBrigExp();
					ADM=V*int(TopGO->newMons->SkillDamageFormationBonus)/100;
				};
				if(i==0){
					ADM+=TopBR->GetAbilityValue(32);
				}else if(i==1){
					ADM+=TopBR->GetAbilityValue(33);
				}
				if(WeaponFlags[wk]&4)ADM+=TopBR->AddDamage;
			}

			if(ADM){
				sprintf(str,"%d+%d",dm,ADM);
			}else{
				sprintf(str,"%d",dm);
			};
			if(i==0){
				if(dm+ADM){
					strcpy(stxt,str);
					SAB->Visible=1;
					SABRE->Visible=1;					
				}else{
					SAB->Visible=0;
					SABRE->Visible=0;
				}
			}else{
				if(dm+ADM){
					strcpy(rtxt,str);
					RIF->Visible=1;
					RIFLE->Visible=1;					
				}else{
					RIF->Visible=0;
					RIFLE->Visible=0;
				}				
			}
		}
		DS_But.ProcessDialogs();

		if(stxt[0])ShowAString(SABRE->x+FI_SabreLX/2,RealLy-21,Font,1,"%s",stxt);
		if(rtxt[0])ShowAString(RIFLE->x+FI_RifleLX/2,RealLy-21,Font,1,"%s",rtxt);

		if(TopBR){
			BrigParam BP;
			GetBrigadeParams(TopBR,&BP);

			ShowAString(RIFLE->x+FI_RifleLX/2,RealLy-FI_RifleLY+3,Font,1,"%d",BP.NShots);
			DrawVertLine(RIFLE->x+FI_RifleLX-4,RealLy-9,BP.ReadyPercent*190/100,40,40,40);
			DrawVertLine(RIFLE->x+FI_RifleLX-6,RealLy-9,BP.ReadyPercent*190/100,0,255,0);
			DrawVertLine(RIFLE->x+FI_RifleLX-5,RealLy-9,BP.ReadyPercent*190/100,0,255,0);				
		}
	}

}
//
CEXPORT void SelBrigadeExp(byte NI, byte Type, int ID){
	ImSelBrigade(NI,Type,GSets.CGame.cgi_NatRefTBL[NI],ID);
	SelBrigade(NI,Type,GSets.CGame.cgi_NatRefTBL[NI],ID);	
};
//
extern OISelection OIS;
void GetUnitsSelGroups(byte NI){
	OIS.Process(NI);

	int i;
	//return;

	// кресть€н в конец
	GeneralObject** GO=NATIONS[NI].Mon;
	for(i=OIS.NSelObj-2;i>=0;i--){
		NewMonster* NM=GO[OIS.SelObj[i]]->newMons;
		NewMonster* N2=GO[OIS.SelObj[i+1]]->newMons;
		if(NM->Peasant&&!N2->Peasant){
			for(int j=i+2;j<OIS.NSelObj;j++){
				N2=GO[OIS.SelObj[j]]->newMons;
				if(N2->Peasant){
					break;
				}
			}
			//if(j>OIS.NSelObj) j=OIS.NSelObj;			
			swap(OIS.SelObj[i],OIS.SelObj[j-1]);
			swap(OIS.SelObjA[i],OIS.SelObjA[j-1]);
		}
	}
}

// ================================================================================
// ui_UnitInfoFill
// ================================================================================
void ui_UnitInfoFill(vui_UnitInfo *I, OneObject *Obj)
{
	NewMonster *NM = Obj->newMons;
	GeneralObject *GO = Obj->Ref.General, *gObj = NULL;
	AdvCharacter *ADC = GO->MoreCharacter;
	Nation *pNation = NULL;
	NewUpgrade *pNewUpgrade = NULL;
	int n = 0, i = 0;

	I->Speed = Obj->newMons->MotionDist * 
		Obj->MoreCharacter->Speed / 100;
	I->Spread = ADC->Razbros;
	I->BuildingTime = ADC->ProduceStages;
	memcpy(I->Price, ADC->NeedRes, sizeof(I->Price));
	I->Cost = NM->Ves;
	I->Vision = NM->VisionType;
	I->AttackRadius = NM->AttackRadius2[0];
	if(I->AttackRadius == 0) I->AttackRadius = NM->AttackRadius2[1];
	I->MoraleRegeneration = NM->MoraleRegenerationSpeed;
	I->StrikeProbability = NM->StrikeProbability;
	I->VeteranKills = NM->VeteranKills;
	I->ExpertKills = NM->ExpertKills;
	// Calculating AttackSpeed (in what units?)
	float GetAttSpped(OneObject *, int);
	I->AttackSpeed = 0;
	I->AttackSpeed2 = 0;
	for(int i = 0; i <= 1; i++)
	{
		float AttackSpeed = GetAttSpped(Obj, i);
		if(AttackSpeed > 0.0f){
			I->AttackSpeed = AttackSpeed;
			break;
		}
	}
	// -----------------------------
	// extracting upgrades info
	// -----------------------------
	int GetWeaponType(char *);
	int iChopping = GetWeaponType("chopping"), 
		iPiercing = GetWeaponType("piercing"),
		iCrushing = GetWeaponType("crushing");

	I->AttackUpgrades[0] = I->AttackUpgrades[1] = I->AttackUpgrades[2] = 0;
	ZeroMemory(I->DefenceUpgrades, sizeof(I->DefenceUpgrades));
	pNation = &NATIONS[GO->NatID];
	if(pNation != NULL)
		for(n = 0; n < pNation->NUpgrades; n++)
		{
			pNewUpgrade = pNation->UPGRADE[n];
			if(pNewUpgrade != NULL)
			{
				if((pNewUpgrade->CtgUpgrade != 12) && // AttackUpgrades
					(pNewUpgrade->CtgUpgrade != 2) && // DefenceUpgrades
					(pNewUpgrade->UnitGroup == NULL) &&
					(pNewUpgrade->UnitValue == Obj->NIndex) &&
					(I->RUNumber < 10))
				{
					I->RUIconFileIDs[I->RUNumber] = 
						pNewUpgrade->IconFileID;
					I->RUIconSpriteIDs[I->RUNumber] = 
						pNewUpgrade->IconSpriteID;
					I->RUHints[I->RUNumber] = 
						pNewUpgrade->Message;
					I->RUNumber++;
				}
				if(pNewUpgrade->CtgUpgrade == 12) // AttackUpgrades
				{
					if((pNewUpgrade->UnitGroup == NULL) && 
						(pNewUpgrade->UnitValue == Obj->NIndex))
					{
						if((pNewUpgrade->Level >= 2) && (pNewUpgrade->Level <= 4))
							I->AttackUpgrades[pNewUpgrade->Level - 2] = 
							pNewUpgrade->Value;
					}

				}
				if(pNewUpgrade->CtgUpgrade == 2)	// DefenceUpgrades
				{
					if((pNewUpgrade->UnitGroup == NULL) &&
						(pNewUpgrade->UnitValue == Obj->NIndex))
						if((pNewUpgrade->Level >= 2) && (pNewUpgrade->Level <= 4))
							for(i = 0; i < pNewUpgrade->NCtg; i++)
							{
								if(pNewUpgrade->CtgGroup[i] == iChopping)	// 3
									I->DefenceUpgrades[0][pNewUpgrade->Level - 2] = 
									pNewUpgrade->Value;
								if(pNewUpgrade->CtgGroup[i] == iPiercing)	// 0
									I->DefenceUpgrades[1][pNewUpgrade->Level- 2] =
									pNewUpgrade->Value;
								if(pNewUpgrade->CtgGroup[i] == iCrushing)	// 2
									I->DefenceUpgrades[2][pNewUpgrade->Level -2] =
									pNewUpgrade->Value;
							}
				}				
				if(pNewUpgrade->UnitGroup != NULL)
				{
					for(i = 0; i < pNewUpgrade->NUnits; i++)
						if(pNewUpgrade->UnitGroup[i] == Obj->NIndex)
							if(I->RUNumber < 10)
							{
								// Prohibiting upgrades,
								// which have Message strarting from HER
								bool bProhibited = false;
								if(strlen(pNewUpgrade->Message) > 3)
									if((pNewUpgrade->Message[0] == 'H') &&
										(pNewUpgrade->Message[1] == 'E') &&
										(pNewUpgrade->Message[2] == 'R'))
										bProhibited = true;
								if(bProhibited == false)
								{
									I->RUIconFileIDs[I->RUNumber] =
										pNewUpgrade->IconFileID;
									I->RUIconSpriteIDs[I->RUNumber] =
										pNewUpgrade->IconSpriteID;
									I->RUHints[I->RUNumber] =
										pNewUpgrade->Message;
									I->RUNumber++;
								}
							}
				}				
			}
		}
		// LifeMax
		I->LifeMax = Obj->MaxLife;

		// AttackType
		if(Obj->newMons->Peasant == false)
			I->WeapType[0] = ADC->WeaponKind[0],
			I->WeapType[1] = ADC->WeaponKind[1];
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
byte ProtWeap[4] = {3,0,2,1};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void vui_SelPoint::Init(OneObject* Obj){
	//ZeroMemory(this, sizeof(*this));
	//vui_UpgradeInfo ui;

	OB=Obj;
	GeneralObject* GO=Obj->Ref.General;

	NewMonster* NM=Obj->newMons;
	AdvCharacter* ADC=Obj->MoreCharacter;

	NatID=GO->NatID;
	NI=Obj->NNUM;
	//NIndex=Obj->NIndex;
	rX=Obj->RealX;
	rY=Obj->RealY;
	SearchVictim=!Obj->NoSearchVictim;
	if(NM->VUI==0){	//	Cannon
		Type=ve_UT_cannon;
		vui_CannInfo* I=&Inf.Cannon;
		I->OB=Obj;
		GetPushkaChargeState(Obj,I->ChargeType,I->ChargeStage);
		I->Shield=ADC->Shield;
		I->NKills=Obj->Kills;

		int dam=ADC->MaxDamage[0]+OB->AddDamage;
		if(Obj->ActiveAbility&&dam){
			Obj->ActiveAbility->ActiveAbilities.modifyDamage(0,dam,dam,NULL);
		}
		I->Damage[0]=dam;

		I->Damage[1]=ADC->MaxDamage[1]+OB->AddDamage;
		I->Damage[2]=ADC->MaxDamage[2]+OB->AddDamage;
		if(Obj->LocalOrder&&Obj->LocalOrder->DoLink==NewAttackPointLink) I->NShots=1;
		else I->NShots=0;
	}else
		if(Obj->BrigadeID!=0xFFFF){	// Brigade
			Type=ve_UT_brigade;
			vui_BrigInfo *I = &Inf.Brigade;		
			I->BrigID=Obj->BrigadeID;
			I->MaxLife=-1;

			//ZeroMemory(&I->UI, sizeof(Inf.Units));

		}else
			if(!Obj->newMons->Building){
				Type = ve_UT_units;	// Units
				vui_UnitInfo *I = &Inf.Units;

				//ZeroMemory(I, sizeof(Inf.Units));

				I->GroundState=(Obj->GroundState==1);
				I->pAbilities = NM->Ability;
				OIS.ActiveState|=(1<<Obj->ActivityState);
				I->Amount=1;
				I->Life=Obj->Life;
				I->Damage[0]=0;
				I->Damage[1]=0;
				I->Defence[0]=OB->AddShield;
				I->Defence[1]=OB->AddShield;
				I->Defence[2]=OB->AddShield;
				I->Defence[3]=OB->AddShield;
				I->Shield=ADC->Shield;
				I->NKills=Obj->Kills;

				I->Patrol=false;
				Order1* Or=Obj->LocalOrder;
				while(Or){
					void PatrolLink(OneObject* OBJ);
					if(Or->DoLink==PatrolLink){
						I->Patrol=true;
						break;
					}
					Or=Or->NextOrder;
				}

				I->HoldPosition=Obj->StandGround;

				if(Obj->newMons->Peasant){
					//char NATID[5];
					//int NPID=CITY[NI].NationalPeasantID;
					//if(NPID!=0xFFFF&&!PlayGameMode){
					//	char* S=NATIONS->Mon[NPID]->MonsterID;
					//	int L=strlen(S);
					//	strcpy(NATID,S+L-4);
					//}else{
					//	NATID[0]=0;
					//};
					//bool CANDO1=1;
					//if(NATID[0]){
					//	CANDO1=strstr(Obj->Ref.General->MonsterID,NATID);
					//};
					//bool OKK=1;
					//if(Obj&&Obj->newMons->Peasant&&!(EditMapMode||PlayGameMode)){
					//	if(NPID==0xFFFF){
					//		CITY[NI].NationalPeasantID=Obj->NIndex;
					//		CmdChangeNPID(NI,Obj->NIndex);
					//	}else{
					//		if(NPID!=Obj->NIndex)OKK=0;
					//	};
					//};
					//bool CANDO=1;
					//if(NATID[0]){
					//	CANDO=CANDO1;
					//};
					//I->Peasant=OKK&&CANDO&&Obj->Ready;
					I->Peasant=true;
				}

				// AttackType
				//if(Obj->newMons->Peasant == false){
				//	I->WeapType[0] = ADC->WeaponKind[0],
				//	I->WeapType[1] = ADC->WeaponKind[1];
				//}
				if(!Obj->newMons->Peasant||EngSettings.vInterf.ShowPeasantDamage){
					//I->Peasant=false;
					I->WeapType[0]=ADC->WeaponKind[0];
					I->WeapType[1]=ADC->WeaponKind[1];
					I->Damage[0]+=ADC->MaxDamage[0];	

					//if(ADC->MaxDamage[1] > 0) I->Damage[1] = ADC->MaxDamage[1];
					//	else I->Damage[1] = ADC->MaxDamage[2];
					I->Damage[1] = ADC->MaxDamage[1];

					if(I->Damage[0]) I->Damage[0]+=OB->AddDamage;
					if(I->Damage[1]) I->Damage[1]+=OB->AddDamage;

					if(Obj->ActiveAbility){
						for(int i=0;i<2;i++){
							if(I->Damage[i]){
								Obj->ActiveAbility->ActiveAbilities.modifyDamage(i,I->Damage[i],I->Damage[i],NULL);
								Obj->ActiveAbility->ActiveAbilities.modifyWeaponKind(I->WeapType[i],I->WeapType[i],i);
							}
						}
					}

					for(int i=0;i<4;i++){
						I->Defence[i]+=ADC->Protection[ProtWeap[i]];
					}
					I->WeapFileID=NM->BigWeapFile;
					I->WeapColdSprite=NM->BigColdWeapSprite;
					I->WeapFireSprite=NM->BigFireWeapSprite;
					I->RifleAttack=Obj->RifleAttack;
					I->NShots=Obj->delay?0:1;
					I->Delay=Obj->delay;
					I->DelayMax=Obj->MaxDelay;
				}
				I->Morale=Obj->Morale/10000;
				I->MoraleMax=Obj->MaxMorale?Obj->MaxMorale/10000:1;
				I->Kinetic=Obj->KineticPower;
				I->KineticMax=NM->KineticLimit;

				// LifeMax
				I->LifeMax = Obj->MaxLife;

			}else{	// Buildings
				Type=ve_UT_building;
				vui_BuildInfo* I=&Inf.Buildings;

				//ZeroMemory(&I->UI, sizeof(Inf.Units));

				I->Amount=1;
				I->Ready=Obj->Ready;
				I->Stage=Obj->Stage;
				I->StageMax=Obj->NStages;
				I->Places=NM->NInFarm;
				I->Population=GetCurrentUnits(NI);
				I->PopulationMax=GetMaxUnits(NI);
				I->Life=Obj->Life;
				I->LifeMax=Obj->MaxLife?Obj->MaxLife:1;
				I->SingleUpgLevel=Obj->SingleUpgLevel;
				I->OB[0]=Obj;
				// attack params: the same as in Units
				I->UI.WeapType[0] = ADC->WeaponKind[0];
				I->UI.WeapType[1] = ADC->WeaponKind[1];

				I->UI.Damage[0] = ADC->MaxDamage[0];

				//if(ADC->MaxDamage[1] > 0) I->UI.Damage[1] = ADC->MaxDamage[1];
				//	else I->UI.Damage[1] = ADC->MaxDamage[2];
				I->UI.Damage[1] = ADC->MaxDamage[1];

				if(I->UI.Damage[0]) I->UI.Damage[0]+=OB->AddDamage;
				if(I->UI.Damage[1]) I->UI.Damage[1]+=OB->AddDamage;

				if(Obj->ActiveAbility){
					for(int i=0;i<2;i++){
						if(I->UI.Damage[i]){
							Obj->ActiveAbility->ActiveAbilities.modifyDamage(i,I->UI.Damage[i],I->UI.Damage[i],NULL);
							Obj->ActiveAbility->ActiveAbilities.modifyWeaponKind(I->UI.WeapType[i],I->UI.WeapType[i],i);
						}
					}
				}

				// defence params: the same as in Units
				I->UI.Defence[0] = OB->AddShield;
				I->UI.Defence[1] = OB->AddShield;
				I->UI.Defence[2] = OB->AddShield;
				I->UI.Defence[3] = OB->AddShield;
				for(int i=0;i<4;i++){
					I->UI.Defence[i]+=ADC->Protection[ProtWeap[i]];
				}
				//
				I->AllowShoot=!OB->NoSearchVictim;
			}

			// view sort priority
			//Sort=Type;
			//if(vHeroButtons.isHero(Obj)){
			//	Sort=ve_UT_hero;
			//}

			Abl=NM->Ability;
			ActAbl=Obj->ActiveAbility;
};
void vui_SelPoint::PreInit(OneObject* Obj){
	//memset(this,0,sizeof *this);
	ZeroMemory(this, sizeof(*this));
	vui_UpgradeInfo ui;

	//OB=Obj;
	GeneralObject* GO=Obj->Ref.General;

	NewMonster* NM=Obj->newMons;
	AdvCharacter* ADC=Obj->MoreCharacter;

	//NatID=GO->NatID;
	//NI=Obj->NNUM;
	NIndex=Obj->NIndex;
	rX=Obj->RealX;
	rY=Obj->RealY;
	//SearchVictim=!Obj->NoSearchVictim;
	if(NM->VUI==0){	//	Cannon
		Type=ve_UT_cannon;
		//vui_CannInfo* I=&Inf.Cannon;
		//I->OB=Obj;
		//GetPushkaChargeState(Obj,I->ChargeType,I->ChargeStage);
		//I->Shield=ADC->Shield;
		//I->NKills=Obj->Kills;

		//int dam=ADC->MaxDamage[0]+ADC->WeaponKind[0];
		//if(Obj->ActiveAbility&&dam){
		//	Obj->ActiveAbility->ActiveAbilities.modifyDamage(0,dam,dam,NULL);
		//}
		//I->Damage[0]=dam;

		//I->Damage[1]=ADC->MaxDamage[1]+ADC->WeaponKind[1];
		//I->Damage[2]=ADC->MaxDamage[2]+ADC->WeaponKind[2];
		//if(Obj->LocalOrder&&Obj->LocalOrder->DoLink==NewAttackPointLink) I->NShots=1;
		//else I->NShots=0;
	}else
		if(Obj->BrigadeID!=0xFFFF){	// Brigade
			Type=ve_UT_brigade;
			vui_BrigInfo *I = &Inf.Brigade;		
			I->BrigID=Obj->BrigadeID;
			I->MaxLife=-1;

			ZeroMemory(&I->UI, sizeof(Inf.Units));

		}else
			if(!Obj->newMons->Building){
				Type = ve_UT_units;	// Units
				vui_UnitInfo *I = &Inf.Units;

				//ZeroMemory(I, sizeof(Inf.Units));

				//I->GroundState=(Obj->GroundState==1);
				//I->pAbilities = NM->Ability;
				//OIS.ActiveState|=(1<<Obj->ActivityState);
				//I->Amount=1;
				I->Life=Obj->Life;
				//I->Damage[0]=0;
				//I->Damage[1]=0;
				//I->Defence[0]=OB->AddShield;
				//I->Defence[1]=OB->AddShield;
				//I->Defence[2]=OB->AddShield;
				//I->Defence[3]=OB->AddShield;
				//I->Shield=ADC->Shield;
				I->NKills=Obj->Kills;

				//I->Patrol=false;
				//Order1* Or=Obj->LocalOrder;
				//while(Or){
				//	void PatrolLink(OneObject* OBJ);
				//	if(Or->DoLink==PatrolLink){
				//		I->Patrol=true;
				//		break;
				//	}
				//	Or=Or->NextOrder;
				//}

				//I->HoldPosition=Obj->StandGround;

				if(Obj->newMons->Peasant){
					//char NATID[5];
					//int NPID=CITY[NI].NationalPeasantID;
					//if(NPID!=0xFFFF&&!PlayGameMode){
					//	char* S=NATIONS->Mon[NPID]->MonsterID;
					//	int L=strlen(S);
					//	strcpy(NATID,S+L-4);
					//}else{
					//	NATID[0]=0;
					//};
					//bool CANDO1=1;
					//if(NATID[0]){
					//	CANDO1=strstr(Obj->Ref.General->MonsterID,NATID);
					//};
					//bool OKK=1;
					//if(Obj&&Obj->newMons->Peasant&&!(EditMapMode||PlayGameMode)){
					//	if(NPID==0xFFFF){
					//		CITY[NI].NationalPeasantID=Obj->NIndex;
					//		CmdChangeNPID(NI,Obj->NIndex);
					//	}else{
					//		if(NPID!=Obj->NIndex)OKK=0;
					//	};
					//};
					//bool CANDO=1;
					//if(NATID[0]){
					//	CANDO=CANDO1;
					//};
					//I->Peasant=OKK&&CANDO&&Obj->Ready;
					I->Peasant=true;
				}

				// AttackType
				//if(Obj->newMons->Peasant == false){
				//	I->WeapType[0] = ADC->WeaponKind[0],
				//	I->WeapType[1] = ADC->WeaponKind[1];
				//}
				//if(!Obj->newMons->Peasant||EngSettings.vInterf.ShowPeasantDamage){
				//	//I->Peasant=false;
				//	I->WeapType[0]=ADC->WeaponKind[0];
				//	I->WeapType[1]=ADC->WeaponKind[1];
				//	I->Damage[0]+=ADC->MaxDamage[0];	

				//	//if(ADC->MaxDamage[1] > 0) I->Damage[1] = ADC->MaxDamage[1];
				//	//	else I->Damage[1] = ADC->MaxDamage[2];
				//	I->Damage[1] = ADC->MaxDamage[1];

				//	if(I->Damage[0]) I->Damage[0]+=OB->AddDamage;
				//	if(I->Damage[1]) I->Damage[1]+=OB->AddDamage;

				//	if(Obj->ActiveAbility){
				//		for(int i=0;i<2;i++){
				//			if(I->Damage[i]){
				//				Obj->ActiveAbility->ActiveAbilities.modifyDamage(i,I->Damage[i],I->Damage[i],NULL);
				//				Obj->ActiveAbility->ActiveAbilities.modifyWeaponKind(I->WeapType[i],I->WeapType[i],i);
				//			}
				//		}
				//	}

				//	for(int i=0;i<4;i++){
				//		I->Defence[i]+=ADC->Protection[ProtWeap[i]];
				//	}
				//	I->WeapFileID=NM->BigWeapFile;
				//	I->WeapColdSprite=NM->BigColdWeapSprite;
				//	I->WeapFireSprite=NM->BigFireWeapSprite;
				//	I->RifleAttack=Obj->RifleAttack;
				//	I->NShots=Obj->delay?0:1;
				//	I->Delay=Obj->delay;
				//	I->DelayMax=Obj->MaxDelay;
				//}
				//I->Morale=Obj->Morale/10000;
				//I->MoraleMax=Obj->MaxMorale?Obj->MaxMorale/10000:1;
				//I->Kinetic=Obj->KineticPower;
				//I->KineticMax=NM->KineticLimit;

				//// LifeMax
				//I->LifeMax = Obj->MaxLife;

			}else{	// Buildings
				Type=ve_UT_building;
				vui_BuildInfo* I=&Inf.Buildings;

				//ZeroMemory(&I->UI, sizeof(Inf.Units));

				//I->Amount=1;
				//I->Ready=Obj->Ready;
				I->Stage=Obj->Stage;
				//I->StageMax=Obj->NStages;
				//I->Places=NM->NInFarm;
				//I->Population=GetCurrentUnits(NI);
				//I->PopulationMax=GetMaxUnits(NI);
				I->Life=Obj->Life;
				//I->LifeMax=Obj->MaxLife?Obj->MaxLife:1;
				//I->SingleUpgLevel=Obj->SingleUpgLevel;
				I->OB[0]=Obj;
				// attack params: the same as in Units
				//I->UI.WeapType[0] = ADC->WeaponKind[0];
				//I->UI.WeapType[1] = ADC->WeaponKind[1];

				//I->UI.Damage[0] = ADC->MaxDamage[0];

				//I->UI.Damage[1] = ADC->MaxDamage[1];

				//if(I->UI.Damage[0]) I->UI.Damage[0]+=OB->AddDamage;
				//if(I->UI.Damage[1]) I->UI.Damage[1]+=OB->AddDamage;

				//if(Obj->ActiveAbility){
				//	for(int i=0;i<2;i++){
				//		if(I->UI.Damage[i]){
				//			Obj->ActiveAbility->ActiveAbilities.modifyDamage(i,I->UI.Damage[i],I->UI.Damage[i],NULL);
				//			Obj->ActiveAbility->ActiveAbilities.modifyWeaponKind(I->UI.WeapType[i],I->UI.WeapType[i],i);
				//		}
				//	}
				//}

				// defence params: the same as in Units
				//I->UI.Defence[0] = OB->AddShield;
				//I->UI.Defence[1] = OB->AddShield;
				//I->UI.Defence[2] = OB->AddShield;
				//I->UI.Defence[3] = OB->AddShield;
				//for(int i=0;i<4;i++){
				//	I->UI.Defence[i]+=ADC->Protection[ProtWeap[i]];
				//}

				//I->AllowShoot=!OB->NoSearchVictim;
			}
			// view sort priority
			Sort=Type;
			if(vHeroButtons.isHero(Obj)){
				Sort=ve_UT_hero;
			}

			//void PerformUpgradeLink(OneObject*);
			//CanUpg=NULL;
			//if(Obj->LocalOrder&&Obj->LocalOrder->DoLink==PerformUpgradeLink){
			//	vu_UpgInfo U;
			//	U.ID=Obj->LocalOrder->info.PUpgrade.NewUpgrade;
			//	U.Stage=Obj->LocalOrder->info.PUpgrade.Stage;
			//	U.StageMax=Obj->LocalOrder->info.PUpgrade.NStages;
			//	U.SingleUpgLevel=Obj->SingleUpgLevel;
			//	Upg.Add(U);
			//}else{		
			//	CanUpg=Obj;
			//}

			//Abl=NM->Ability;
			//ActAbl=Obj->ActiveAbility;
			void PerformUpgradeLink(OneObject*);
			CanUpg=NULL;
			if(Obj->LocalOrder&&Obj->LocalOrder->DoLink==PerformUpgradeLink){
				vu_UpgInfo U;
				U.ID=Obj->LocalOrder->info.PUpgrade.NewUpgrade;
				U.Stage=Obj->LocalOrder->info.PUpgrade.Stage;
				U.StageMax=Obj->LocalOrder->info.PUpgrade.NStages;
				U.SingleUpgLevel=Obj->SingleUpgLevel;
				Upg.Add(U);
			}else{		
				CanUpg=Obj;
			}
};
int vui_SelPoint::Cmp(vui_SelPoint* SP){
	// 1 - SP<this, 0 - equal, - 1 - SP>this
	if(Sort==SP->Sort){
		if(NIndex==SP->NIndex){
			switch(Sort){
				case ve_UT_cannon:
					return SP->Inf.Cannon.OB->Index-Inf.Cannon.OB->Index;
				case ve_UT_brigade:
					return SP->Inf.Brigade.BrigID-Inf.Brigade.BrigID;
				case ve_UT_building:{
					vui_BuildInfo* SI=&SP->Inf.Buildings;
					vui_BuildInfo* I=&Inf.Buildings;
					if(SI->Stage==I->Stage) return 0;
					//if(SI->Stage<SI->StageMax) return 1;
					//if(I->Stage<I->StageMax) return -1;
					break;
									}
			}
			return 0;
		}else{
			switch(Sort){
				case ve_UT_units:
					if(Inf.Units.Peasant&&!SP->Inf.Units.Peasant) return -1;
					if(!Inf.Units.Peasant&&SP->Inf.Units.Peasant) return 1;
			}
			return SP->NIndex-NIndex;
		}		
	}else{
		return SP->Sort-Sort;
	}
};
bool vui_SelPoint::Add(vui_SelPoint* SP){
	if(Cmp(SP)==0){
		if(SP->SearchVictim)SearchVictim=true;

		// добавл€ем информацию о исполн€ющихс€ усовершенствований
		if(!CanUpg){
			CanUpg=SP->CanUpg;
		}
		if(SP->Upg.GetAmount()){
			Upg.Add(SP->Upg[0]);
		}

		switch(Type){
			case ve_UT_cannon:
				break;
			case ve_UT_brigade: {
				vui_BrigInfo* I=&Inf.Brigade;
				break; }
			case ve_UT_units: {
				vui_UnitInfo* I=&Inf.Units;
				I->Amount++;
				rX=(rX+SP->rX)/2;
				rY=(rY+SP->rY)/2;
				I->Life+=SP->Inf.Units.Life;
				I->NKills+=SP->Inf.Units.NKills;
				//I->NShots+=SP->Inf.Units.NShots;
				//I->Delay+=SP->Inf.Units.Delay;
				//if(SP->Inf.Units.DelayMax>I->DelayMax)I->DelayMax=SP->Inf.Units.DelayMax;
				//if(SP->Inf.Units.LifeMax>I->LifeMax){
				//	I->LifeMax=SP->Inf.Units.LifeMax?SP->Inf.Units.LifeMax:1;
				//}
				//I->GroundState|=SP->Inf.Units.GroundState;
				//I->HoldPosition|=SP->Inf.Units.HoldPosition;
				break; }
			case ve_UT_building: {
				vui_BuildInfo* I=&Inf.Buildings;
				if(I->Amount<10){
					I->OB[I->Amount]=SP->Inf.Buildings.OB[0];
				}				
				I->Amount++;
				I->Life+=SP->Inf.Buildings.Life;
				//if(SP->Inf.Buildings.LifeMax>I->LifeMax){
				//	I->LifeMax=SP->Inf.Buildings.LifeMax?SP->Inf.Buildings.LifeMax:1;
				//}
				//I->AllowShoot|=SP->Inf.Buildings.AllowShoot;
				break; }
		}
		return true;
	}
	return false;
};
//
void OISelection::Clear() {
	SelPoint.FastClear();
	// old
	NSelBr=0; NSelObj=0;
	int n=Bld.GetAmount();
	for(int i=0;i<n;i++){
		if(Bld[i])free(Bld[i]);
	}
	Bld.Clear();
	Settlement=0xFFFF;
	Oboz=0xFFFF;
	RifleAttEnabled=1;
	RifleAttackAllowed=0;
}

void OISelection::Process(byte NI){
	static bool s=true;
	if(s){
		RegisterMembers();
		s=false;
	}
	OIS.ActiveState=0;
	// Init
	vui_SelPoint LSP;
	if(LastSP<SelPoint.GetAmount()){
		LSP=SelPoint[LastSP];

		//int ConvertNationToSkirmish(int SkirmisID);
		//itr_GetImSelected.Create(NI);
		//while(OneObject* OB=itr_GetImSelected.Next()){
		//	GSets.CGame.InterfaceNatID=ConvertNationToSkirmish(OB->Ref.General->NatID);            
		//	//if(LSP.OB)GSets.CGame.InterfaceNatID=ConvertNationToSkirmish(LSP.OB->Ref.General->NatID);
		//	break;
		//}

	}
	// AddObjects
	CreateFromSelection(NI);
	// Find LastSP
	LastSP=0xFFFF;
	for(int i=0;i<SelPoint.GetAmount();i++){
		if(SelPoint[i].Cmp(&LSP)==0){
			LastSP=i;
			vui_SelPoint* SP=SelPoint+i;
			for(int j=0;j<NAttTypes;j++)
				SP->TimeLastActivationDefence[j]=LSP.TimeLastActivationDefence[j];
			break;
		}
	}
	if(LastSP==0xFFFF) LastSP=0;
	SetProduce();
	SetUpgrade();

	TankCannon=false;
	TankMachineGun=false;

	vui_SelPoint* SP=GetLastSelPoint();
	if(SP){
		itr_AA.Create(SP->OB);
		while(ActiveUnitAbility* AA=itr_AA.Next()){
			if( !strcmp(AA->GetClassName(),"Active_3D_shooter_simulator") ){
				Active_3D_shooter_simulator* aa=(Active_3D_shooter_simulator*)AA;
				ExtraShootingElement* ua=(ExtraShootingElement*)aa->GetA();
				if( ua->BulletsAmount<3 ){
					TankCannon=true;
					//TankCannonReload=100;
					TankCannonReload=100-aa->LastShotTime*30*100/ua->TotalRechargeTime/256;
				}else{
					TankMachineGun=true;
					//TankMachineGunReload=100;
					//TankMachineGunCharge=200;
					//TankMachineGunReload=100-AA->LastShotTime*30*100/UA->TotalRechargeTime/256;
					if(aa->Bullets>0){
						TankMachineGunReload=aa->Bullets*100/ua->BulletsAmount;
						TankMachineGunCharge=aa->Bullets;
					}else{
						TankMachineGunReload=100-(aa->CartridgeReadyTime-TrueTime)*100/ua->CartridgeRechargeTime;
						TankMachineGunCharge="";
					}
				}
			}
		}
	}

}
bool BuyBlankBuilding(byte NI, word NIndex){
	Nation* Nat=NATIONS+NI;
	//
	/*
	int NBuild=Nat->PACount[BuilderNIndex];
	word* Build=Nat->PAble[BuilderNIndex];
	char* AIndex=Nat->AIndex[BuilderNIndex];
	*/
	//
	word* unID=NatList[NI];	// list units in nation
	int unN=NtNUnits[NI];
	//byte bAm[2048];	// amount current availible for construct
	//word bID[2048];	// index in Group[]
	//memset(bAm,0,sizeof(bAm));
	//memset(bID,0xFF,sizeof(bID));
	for(int i=0;i<unN;i++,unID++){
		OneObject* OB=Group[*unID];
		if(OB&&(!OB->Sdoxlo)&&OB->NewBuilding&&OB->NIndex==NIndex&&OB->Stage<OB->NStages){
			vui_IS_MakeMaxStage Com;
			Com.Data.Index=*unID;
			Com.InterfAction();
			return true;
		}
	}
	return false;
}
void OISelection::SetUpgrade(){
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	//
	Upgrade.FastClear();
	if(SelPoint.GetAmount()!=1) return;
	vui_SelPoint* SP=SelPoint+LastSP;
	//
	if(SP->Type==ve_UT_building&&(SP->Inf.Buildings.Stage<SP->Inf.Buildings.StageMax)) return;
	if(SP->NI!=NI) return;
	//
	Nation* Nat=NATIONS+NI;
	GeneralObject* GO=Nat->Mon[SP->NIndex];
	int NUpg=GO->NUpgrades;
	word* Upg=GO->Upg;

	for(int i=0;i<NUpg;i++,Upg++){
		NewUpgrade* U=Nat->UPGRADE[*Upg];
		if(U->ManualDisable) continue;
		bool IsDoing=false;
		if(U->IsDoing){
			for(int i=0;i<SP->Upg.GetAmount();i++){
				if(SP->Upg[i].ID==*Upg){
					IsDoing=true;
					break;
				}
			}
		}
		if(U&&!U->Done&&(U->ManualEnable||U->Enabled||IsDoing||U->Level==0)){
			bool ok=true;			
			if(SP->Type==ve_UT_building&&U->Individual&&
				SP->Inf.Buildings.SingleUpgLevel!=U->Level) ok=false;
			if(ok){
				vui_UpgradeInfo UI;

				UI.Enable=U->ManualEnable||U->Enabled||IsDoing;
				UI.Upg=U;
				UI.Index=*Upg;
				UI.FileID=U->IconFileID;
				UI.SpriteID=U->IconSpriteID;
				UI.Message=U->Message;
				//UI.Building=nm->Building;
				//PI.Enabled=go->Enabled;
				//PI.NIndex=*Build;
				UI.IconPosition=U->IconPosition;
				//if(UI.IconPosition>=24){
				//	UI.IconPosition-=24;
				//}
				UI.x=U->IconPosition%12;
				UI.y=U->IconPosition/12;
				/*
				if(PI.Building){
				if(SP->Type==ve_UT_building){
				PI.NProduce=bAm[*Build];
				PI.NUnlimit=bID[*Build];
				if(bID[*Build]==0xFFFF) PI.NIndex=0xFFFF;
				}else{
				PI.NProduce=0;
				PI.NUnlimit=0;
				}			
				PI.Stage=0;
				PI.MaxStage=1;
				}else{
				PI.NProduce=GetAmount(*Build);
				PI.NUnlimit=0;
				if(PI.NProduce>=1200){
				PI.NUnlimit=PI.NProduce/1200;
				PI.NProduce=PI.NProduce%1200;
				}
				PI.Stage=GetProgress(*Build,&PI.MaxStage);
				};
				if(PI.MaxStage==0) PI.MaxStage=1;
				*/
				//PI.HotKey=go->newMons->BuildHotKey;
				Upgrade.Add(UI);
			}
		}
	}
}
bool DetectMagazine(OneObject* OB);
void OISelection::SetProduce(){
	byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
	//
	Produce.FastClear();
	if(SelPoint.GetAmount()!=1) return;
	vui_SelPoint* SP=SelPoint+LastSP;
	if(BuildMode) return;
	//if(SP->Type==ve_UT_brigade) return;
	//if(SP->Type==ve_UT_building&&(!SP->Inf.Buildings.Ready||SelPoint.GetAmount()!=1)&&SP->Upg.ID==0xFFFF) return;
	if(SP->Type==ve_UT_building&&(!SP->Inf.Buildings.Ready||SelPoint.GetAmount()!=1)&&SP->Upg.GetAmount()==0) return;
	if(SP->NI!=NI && !DetectMagazine(SP->OB)) return;
	//			
	Nation* Nat=NATIONS+NI;
	City* CT=CITY+NI;
	word NIndex=SP->NIndex;
	//
	int NBuild=Nat->PACount[NIndex];
	word* Build=Nat->PAble[NIndex];
	char* AIndex=Nat->AIndex[NIndex];
	//	
	bool bld=false;	// if produce buildings
	word* unID=NatList[NI];	// list units in nation
	int unN=NtNUnits[NI];
	byte bAm[2048];	// amount current availible for construct
	word bID[2048];	// index in Group[]
	memset(bAm,0,sizeof(bAm));
	memset(bID,0xFF,sizeof(bID));
	for(int i=0;i<unN;i++,unID++){
		OneObject* OB=Group[*unID];
		if(OB&&(!OB->Sdoxlo)&&OB->NewBuilding&&OB->Stage<OB->NStages){
			word NIndex=OB->NIndex;
			bAm[NIndex]++;
			if(bID[NIndex]==0xFFFF) bID[NIndex]=*unID;
		}
	}
	//
	for(i=0;i<NBuild;i++,Build++,AIndex++){
		GeneralObject* go=Nat->Mon[*Build];
		NewMonster* nm=go->newMons;
		bld|=nm->Building&&!nm->SelfTransform;
		if(go->ManualDisable) continue;
		//if(!go->Enabled) continue;		
		/*
		bool okk=1;
		if(go->StageMask){
		byte m=go->StageMask;
		word s=OBJ->StageState;
		for(int i=0;i<5;i++){
		if(m&1){
		byte s1=s&7;
		if(s1!=2)okk=0;
		};
		m>>=1;
		s>>=3;
		};
		};
		if(!okk) continue;	
		*/
		//
		vui_ProduceInfo PI;
		PI.Building=nm->Building&&!nm->SelfTransform;
		//
		PI.Enabled=(go->Enabled||go->ManualEnable)&&!go->ManualDisable;
		PI.Disabled=go->ManualDisable;
		if(nm->ArtSet){
			int n=Nat->NArtUnits[nm->ArtSet-1];
			int m=Nat->NArtdep[nm->ArtSet-1]*nm->NInArtDepot;
			if(n>=m){
				PI.Enabled=false;
			}						
		}				
		if(go->LockID!=0xFFFF){
			int n=CT->UnitAmount[go->LockID];
			int m=go->NLockUnits;
			if(n>=m){
				PI.Enabled=false;
			}						
		};				
		//
		PI.NIndex=*Build;
		PI.AIndex=(*AIndex);
		PI.x=(*AIndex)%12;
		PI.y=(*AIndex)/12;
		if(PI.Building){
			if(SP->Type==ve_UT_building){
				PI.NProduce=bAm[*Build];
				PI.NUnlimit=bID[*Build];
				if(bID[*Build]==0xFFFF) PI.NIndex=0xFFFF;
			}else{
				PI.NProduce=0;
				PI.NUnlimit=0;
			}			
			PI.Stage=0;
			PI.MaxStage=1;
		}else{
			PI.NProduce=GetAmount(*Build);
			PI.NUnlimit=0;
			if(PI.NProduce>=1200){
				PI.NUnlimit=PI.NProduce/1200;
				PI.NProduce=PI.NProduce%1200;
			}
			PI.Stage=GetProgress(*Build,&PI.MaxStage);
		};
		if(PI.MaxStage==0) PI.MaxStage=1;
		PI.HotKey=go->newMons->BuildHotKey;
		Produce.Add(PI);
	}
	if(SP->Type==ve_UT_building&&bld){
		for(i=0;i<Produce.GetAmount();i++){
			word* NInd=&Produce[i].NIndex;
			bool Bld=Produce[i].Building;
			(*NInd)|=0xFFFF*word(!Bld);
		}
	}
};
extern bool InfoMode;
void OISelection::CreateFromSelection(byte NI){
	Clear();
	int N=ImNSL[NI];
	word* IDS=ImSelm[NI];
	word* SNS=ImSerN[NI];
	for(int i=0;i<N;i++){
		if(IDS[i]!=0xFFFF){
			OneObject* OB=Group[IDS[i]];
			if(OB&&OB->Serial==SNS[i]&&!OB->Sdoxlo){
				if( !OB->newMons->NewParams.PortretDisable || InfoMode )
					AddObj(OB);			
			}
		}
	}
	// Rome Unit Info
	N=SelPoint.GetAmount();
	extern bool m_U_Info;
	if(N==1&&m_U_Info) for(i=0;i<N;i++){
		vui_SelPoint* SP=SelPoint+i;
		if(SP){
			switch(SP->Type){
				case ve_UT_building:
					{
						//ZeroMemory(&SP->Inf.Buildings.UI, sizeof(SP->Inf.Buildings.UI));
						ui_UnitInfoFill(&SP->Inf.Buildings.UI, SP->OB);
						// Attack Speed is specific for this unit type
						OneObject *Obj = SP->OB;
						SP->Inf.Buildings.UI.AttackSpeed = 0;
						for(int i = 0; i <= 1; i++)
						{
							float AttackSpeed =
								Obj->MoreCharacter->AttackPause[i];
							if(AttackSpeed > 0.0f){
								SP->Inf.Buildings.UI.AttackSpeed = AttackSpeed;
								break;
							}
						}
						// Buildtime is specific for this unit type
						if(Obj->newMons != NULL)
							SP->Inf.Buildings.UI.BuildingTime *=
							Obj->newMons->BuildPtX.NValues;
					}
					break;
				case ve_UT_units:
					//ZeroMemory(&SP->Inf.Units, sizeof(SP->Inf.Units));
					ui_UnitInfoFill(&SP->Inf.Units, SP->OB);
					break;
				case ve_UT_brigade:
					//ZeroMemory(&SP->Inf.Brigade.UI, sizeof(SP->Inf.Brigade.UI));
					ui_UnitInfoFill(&SP->Inf.Brigade.UI, SP->OB);
					break;
			};
		}
	}
};
void OISelection::AddObj(OneObject* OB){

	if(v_ISys.DisableSelectionEnemy){
		if(OB->NNUM!=0) return;
		if(DetectBonus(OB)) return;
		if(DetectChest(OB)) return;
		if(DetectMagazine(OB)) return;
	}

	// ignore commanders and flagbear
	//if(OB->BrigadeID!=0xFFFF){
	//	Brigade* Br=CITY[OB->NNUM].Brigs+OB->BrigadeID;
	//	if(OB->BrIndex<3||OB->Ref.General->UsualFormID!=0xFFFF) return; //OB->NIndex!=Br->MembID
	//}

	/*if(OB->NNUM!=7||!(OB->NewBuilding||OB->Stuff==0xFFFF)||OB->BrigadeID!=0xFFFF)*/{
	int pos=SelPoint.GetAmount();
	static vui_SelPoint SP;	
	SP.PreInit(OB);
	for(int i=0;i<pos;i++){
		int cmp=SelPoint[i].Cmp(&SP);
		if(cmp<0){
			pos=i;
			break;
		}else
			if(cmp==0){
				SelPoint[i].Add(&SP);
				pos=-1;
				break;
			};
	}
	if(pos>=0){
		if(SP.Type==ve_UT_brigade)SP.Inf.Brigade.SetFromBrig(&SP);
		SP.Init(OB);
		SelPoint.Insert(pos,SP);
	}
	}	

	return;
	//if(OB->NNUM==7&&OB->Stuff==0xFFFF) return;

	SelNation=OB->NNUM;
	word BID=OB->BrigadeID;
	word NIndex=OB->NIndex;
	if(OB->newMons->DamWeap[1]){
		if(!OB->RifleAttack)RifleAttEnabled=0;
		RifleAttackAllowed=1;
	}
	if(BID!=0xFFFF){
		for(int i=0;i<NSelBr;i++){
			if(SelBr[i]==BID) return;
		}
		if(NSelBr>=MaxSelBr){
			MaxSelBr=NSelBr+32;
			SelBr=(word*)realloc(SelBr,MaxSelBr<<1);
		}
		SelBr[NSelBr]=BID;
		NSelBr++;
	}else{
		if(OB->NewBuilding){
			if(Settlement==0xFFFF&&OB->Stuff!=0xFFFF){
				Settlement=OB->Index;
				return;
			}
			int n=Bld.GetAmount();
			OIS_Bld* bld=NULL;
			for(int i=0;i<n;i++){
				if(Bld[i]->NIndex==OB->NIndex){
					bld=Bld[i];
					break;
				}
			}			
			if(!bld){
				bld=new OIS_Bld;
				bld->NIndex=OB->NIndex;
				Bld.Add(bld);
			}
			bld->ID.Add(OB->Index);
		}else{
			if(Oboz==0xFFFF&&OB->Stuff!=0xFFFF){
				Oboz=OB->Index;
				return;
			}
			for(int i=0;i<NSelObj;i++){
				if(SelObj[i]==NIndex){
					SelObjA[i]++;
					return;
				}
			}
			if(NSelObj>=MaxSelObj){
				MaxSelObj=NSelObj+32;
				SelObj=(word*)realloc(SelObj,MaxSelObj<<1);
				SelObjA=(word*)realloc(SelObjA,MaxSelObj<<1);
			}
			SelObj[NSelObj]=NIndex;
			SelObjA[NSelObj]=1;
			NSelObj++;
		}
	}
};
word OISelection::GetNIndex(word SelPointID){
	if(SelPointID<SelPoint.GetAmount()){
		return SelPoint[SelPointID].NIndex;
	}
	return 0;
};
GeneralObject* OISelection::GetGeneralObject(word SelPointID){
	if(SelPointID<SelPoint.GetAmount()){
		byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
		return NATIONS[NI].Mon[SelPoint[SelPointID].NIndex];
	}
	return NULL;
};
GeneralObject* OISelection::GetGeneralObject(ParentFrame* PF){
	if(PF){
		return GetGeneralObject(((SimpleDialog*)PF)->ID);
	}
	return NULL;
};
vui_SelPoint* OISelection::GetSelPoint(ParentFrame* PF){
	if(PF){
		word id=((SimpleDialog*)PF)->ID;
		if(id<SelPoint.GetAmount()){
			return SelPoint+id;
		}		
	}
	return NULL;
};
vui_SelPoint* OISelection::GetLastSelPoint(){
	if(SelPoint.GetAmount()>0&&OIS.LastSP>=0){
		if(OIS.LastSP<SelPoint.GetAmount())return SelPoint+LastSP;
		else return &SelPoint[0];
	}
	return NULL;
};
bool OISelection::SetLastSP(word LSP){
	if(LSP<SelPoint.GetAmount()){
		LastSP=LSP;
		return true;
	}
	LastSP=0;
	return false;
};

vui_ProduceInfo* OISelection::GetProduceInfo(word ID){
	if(ID<Produce.GetAmount()){
		return Produce+ID;
	}
	return NULL;
};
int OISelection::GetProduceAmount(){
	return Produce.GetAmount();
}
vui_ProduceInfo* OISelection::GetProduceInfoByAIndex(byte AIndex){
	int n=Produce.GetAmount();
	for(int i=0;i<n;i++){
		vui_ProduceInfo* PI=Produce+i;
		if(PI->AIndex==AIndex){
			return PI;
		}
	}
	return NULL;
};
int	OISelection::GetProduceAmountByAIndex(){
	int a=0;
	int n=Produce.GetAmount();
	for(int i=0;i<n;i++){
		vui_ProduceInfo* PI=Produce+i;
		int aa=PI->AIndex+1;
		if(a<aa){
			a=aa;
		}
	}
	return a;
};

vui_UpgradeInfo* OISelection::GetUpgradeInfo(word ID){
	if(ID<Upgrade.GetAmount()){
		return Upgrade+ID;
	}
	return NULL;
};

vui_UpgradeInfo* OISelection::GetUpgradeInfoByIconPosition(word IconPosition){
	vui_UpgradeInfo* UpgInf=NULL;
	int n=Upgrade.GetAmount();
	for(int i=0;i<n;i++){
		vui_UpgradeInfo* UI=Upgrade+i;
		if( UI && UI->IconPosition==IconPosition ){
			UpgInf=UI;
			break;
		}
	}
	return UpgInf;
};

int OISelection::GetUpgradeAmountByIconPosition(){
	int a=0;
	int n=Upgrade.GetAmount();
	for(int i=0;i<n;i++){
		vui_UpgradeInfo* UI=Upgrade+i;
		if(a<=UI->IconPosition){
			a=UI->IconPosition+1;
		}
	}
	return a;
};

int OISelection::GetUpgradeAmount(){
	return Upgrade.GetAmount();
}
//
//////////////////////////////////////////////////////////////////////////
//
DynAbilityArray* vui_SelPoint::GetActiveUnitAbilityArray(int Type){
	DynAbilityArray* DA=NULL;
	enAbilityDialogs T=(enAbilityDialogs)Type;
	switch(T){
		case adEffects:
			DA=&Effects;
			break;
		case adButtons:
			DA=&Buttons;
			break;
		case adPassive:
			DA=&Passive;
			break;
		case adLevelUps:
			DA=&LevelUps;
			break;
		case adItems:
			DA=&Items;
			break;
		case adItemsTakeOff:
			DA=&ItemsTakeOff;
			break;
		case adLibShop:
			DA=&LibShop;
			break;
		case adLibHero:
			DA=&LibHero;
			break;
		case adDialog:
			DA=&Dialog;
			break;
		case adButtonAuras:
			DA=&ButtonAuras;
			break;
		case adDebug:
			DA=&Debug;
			break;
		case adBasketBottles:
			DA=&BasketBottles;
			break;
		case adBasketMagicCards:
			DA=&BasketMagicCards;
			break;
		case adAntiMageSkills:
			DA=&AntiMageSkills;
			break;
	}
	if(T>=adHelm&&T<adHelm+10){
		DA=&Inventory[T-adHelm];
	}
	return DA;
};

int vui_SelPoint::GetActiveUnitAbilityAmount(int Type){
	DynAbilityArray* DA=GetActiveUnitAbilityArray(Type);
	if(DA){
		return DA->GetAmount();
	}
	return 0;
};

ActiveUnitAbility* vui_SelPoint::GetActiveUnitAbility(int Index, int Type){
	DynAbilityArray* DA=GetActiveUnitAbilityArray(Type);
	if(DA&&Index<DA->GetAmount()){
		return (*DA)[Index];
	}
	return NULL;
}

extern const vClotheN;
extern int vClotheSet[];
DLLEXPORT bool GetBuildingEntryPoint(int* x,int* y,word Index,int AddDistance);

int vui_SelPoint::GetAbilitySlotAmount(int Type){
	//_AbilityDialogs
	//0 "Effects", 
	//1 "Passive", 
	//2 "Buttons", 
	//3 "LevelUps", 
	if(Type==adItems) //4 "Items", т.е. по€с с бутылками
		if(OIS.SelPoint.GetAmount()==1)
			return 13;
		else
			return 8;
	if( Type>=adHelm && Type<=adWeapon ){ // adHelm=5, 5+/*vClotheN*/10
		return vClotheSet[Type-adHelm];
	}
	if(Type==adItemsTakeOff) //15 "ItemsTakeOff", 
		return 10;
	//16 "LibShop", 
	//17 "LibHero", 
	//18 "Dialog", 
	//19 "ButtonAuras", 
	//20 "Debug", 
	//21 "BasketMagicCard",	
	if(Type==adBasketBottles) //22 "BasketBottles"
		return 36;
	return 0; // unlimited
};

int DynAbilityArray::Add(ActiveUnitAbility* V){
	if(V){
		int n=GetAmount();
		if( V->Slot==-1 ){
			for(int i=0;i<n;i++){
				if( Values[i]==NULL ){
					Values[i]=V;
					return i;
				}
			}
		}else{
			if( V->Slot<n ){
				if( Values[V->Slot]!=NULL ){
					bool add=true;
					for(int i=0;i<n;i++){
						if( i!=V->Slot && Values[i]==NULL ){
							Values[i]=Values[V->Slot];
							add=false;
							break;
						}
					}
					if(add){
						ActiveUnitAbility* AA=Values[V->Slot];
						DynArray<ActiveUnitAbility*>::Add(AA);
					}
				}
				Values[V->Slot]=V;
				return V->Slot;
			}
		}
		return DynArray<ActiveUnitAbility*>::Add(V);
	}
	return -1;
};

void vui_SelPoint::SetActiveUnitAbility(UnitActiveAbilityList* UAAL){

	extern int SpellHotKeyID;
	SpellHotKeyID=0;

	Effects.Clear();
	Passive.Clear();
	Buttons.Clear();
	ButtonAuras.Clear();
	LevelUps.Clear();
	Items.Clear();
	for(int i=0;i<10;i++){
		Inventory[i].Clear();
	}
	ItemsTakeOff.Clear();
	LibShop.Clear();
	LibHero.Clear();
	Debug.Clear();

	BasketMagicCards.Clear();
	AntiMageSkills.Clear();

	BasketBottles.Clear();

	int ShopCategory=-1;

	if(!UAAL)
		return;

	OIS.AntiMage=OB->newMons->NewParams.AntiMage;
	OIS.NotAntiMage=!OIS.AntiMage;
	OIS.AntiMageSkillPoints="";
	HeroVariableStorage* HVS=DetectHero(OB);
	if(HVS){
		HeroAbility* HA=(HeroAbility*)HVS->GetA();
		int L=HVS->AntiMageLevel;
		int NL=HA->AntiMageLevels.GetAmount()-1;
		if( L>NL )
			L=NL;
		int LevelSP=(L>=0)?HA->AntiMageLevels[L]:200;
		if( OB->ActiveAbility->AntiMageSkillPoints<LevelSP ){
			OIS.AntiMageSkillPoints.print("{CR}%d{C}/%d",OB->ActiveAbility->AntiMageSkillPoints,LevelSP);
		}else{
			OIS.AntiMageSkillPoints.print("%d/%d",OB->ActiveAbility->AntiMageSkillPoints,LevelSP);
		}
	}

	// создаем слоты
	for(int i=0;i<23;i++){
		int n=GetAbilitySlotAmount(i);
		if(n>0){
			DynAbilityArray* DAA=GetActiveUnitAbilityArray(i);
			DAA->Clear();
			DAA->DynArray<ActiveUnitAbility*>::Add(NULL,n);
		}
	}

	if(UAAL->LibID!=0xFFFF){
		OneObject* Lib=Group[UAAL->LibID];
		if (Lib && !Lib->Sdoxlo && Lib->Serial==UAAL->LibSN){

			OneMagazine* DetectMagazine2(OneObject* OB);
			OneMagazine* OM=DetectMagazine2(Lib);

			if(OM){

				NewMagazine* NM=(NewMagazine*)OM->GetA();
				ShopCategory=NM->ShopCategory;

				itr_AA.Create(Lib);
				while (ActiveUnitAbility* AA=itr_AA.Next()) {
					UnitAbility* UA=AA->GetA();
					if (UA&&AA!=OM) {
						switch(ShopCategory){
							case 0: //MagicCard
								if(AA->TypeAbil==1&&!(UA->Aura||UA->System)){
									LibShop.Add(AA);
								}
								break;
							case 1: //Clothe
								if(AA->TypeAbil==4){
									LibShop.Add(AA);
								}
								break;
							case 2: //Bottle
								if(AA->TypeAbil==5){
									LibShop.Add(AA);
								}
								break;
						}
					}
				}
			}
		}
	}

	uaa_Chest* DetectChest(OneObject* OB);
	uaa_Chest* Chest=DetectChest(OB);

	bool DetectMagazine(OneObject* OB);
	OneBonus* DetectBonus(OneObject* OB);
	bool Shop=DetectMagazine(OB);
	bool Bonus=DetectBonus(OB);

	bool UnitShowButton=!(Chest||Shop||Bonus);

	bool GetHero(OneObject* OB);
	Hero=GetHero(OB);

	//TankCannon=NULL;
	//TankMachineGun=NULL;

	int na=UAAL->ActiveAbilities.GetAmount();
	for(int i=0;i<na;i++){
		ActiveUnitAbility* AA=UAAL->ActiveAbilities[i];
		if(AA&&AA->Visible){

			//if(!Hero)Hero=AA->IsHero();

			UnitAbility* UA=AA->GetA();
			if(UA){
				//if( !strcmp(AA->GetClassName(),"Active_3D_shooter_simulator") ){
				//	ExtraShootingElement* ESE=(ExtraShootingElement*)AA->GetA();
				//	if( ESE->BulletsAmount<3 ){
				//		TankCannon=AA;
				//	}else{
				//		TankMachineGun=AA;
				//	}
				//}

				if(AA->TypeAbil!=7){
					if( UA->System || AA->Deleted || AA->Droped || !Hero&&(AA->TypeAbil==4||AA->TypeAbil==5||AA->ItemDrop>0) ){
						Debug.Add(AA);
						continue;
					}
				}
			}

			if(Chest){
				if( AA->TypeAbil==1&&AA!=Chest || AA->TypeAbil==4 || AA->TypeAbil==5 ){
					Passive.Add(AA);
				}
			}else
				switch(AA->TypeAbil){
				case 0:
					if(UA){
						// бутылки мгновенного действи€
						if(UA->Type==0){
							// файл еффекта равен файлу кнопочки
							if(AA->FileID==UA->FileID){
								//continue;
							}
						}
					}
					Effects.Add(AA);
					break;
				case 1:
					{
						if( ShopCategory==0 && UA && !UA->Aura ){
							LibHero.Add(AA);
						}
					}
				case 2:
					{
						if( OB->newMons->NewParams.AntiMage && UA->Type==7 ){
							if( AA->TypeAbil==1 ){
								AntiMageSkills.Add(AA);
							}else{
								if( UA && UA->Conditions.Check(AA) ){
									if( UA->AntiMageSupportSkill ){
										//ButtonAuras.Add(AA);
									}else{
										Buttons.Add(AA);
									}
								}
							}
						}else if(Hero){
							if(AA->TypeAbil==2){
								Passive.Add(AA);
							}else{
								if(UA){
									if( UA->Aura || UA->Type==4 ){
										ButtonAuras.Add(AA);
									}else{
										//Buttons.Add(AA);
										//if(!UA->CanNotBeClothe)
										//	AA->Clothed=true;
										if(AA->Clothed)
											Buttons.Add(AA);
										else
											BasketMagicCards.Add(AA);
									}
								}
							}
						}
						else	// Edit by Xeno, fix the "reindeer bug"
						{
							if(UnitShowButton && UA)
							{
								if(UA->Aura) Passive.Add(AA);
								else Buttons.Add(AA);
							}
						}
					}
					break;
				case 3:
					if(UAAL->BornNIndex==UAAL->OB->NIndex){
						LevelUps.Add(AA);
					}
					break;				
				case 5:
					if( AA->Clothed /*&& Items.GetAmount()<10*/ && Items.Add(AA)!=-1 ){
						//Items.Add(AA);
					}else{
						BasketBottles.Add(AA);
					}
					if(ShopCategory==2){
						LibHero.Add(AA);
					}
					break;
				case 4:
					{
						UnitAbility* A=AA->GetA();
						if(A){
							int i=A->TypeClothe;
							if(i<11 && AA->Clothed /*&& Inventory[i].GetAmount()<vClotheSet[i]*/){
								Inventory[i].Add(AA);
							}else{
								ItemsTakeOff.Add(AA);
							}
						}
						if(ShopCategory==1){
							LibHero.Add(AA);
						}
					}
					break;
				case 7:
					Dialog.Add(AA);
					break;
			};

		}
	}

	//int n=EngSettings.vInterf.MagicCardsVisibleAmount-ButtonAuras.GetAmount();
	//if(n<0)n=0;
	//int nb=Buttons.GetAmount();
	//for(i=n;i<nb;i++){
	//	BasketMagicCards.Add(Buttons[i]);
	//	Buttons[i]->Clothed=false;
	//}
	//Buttons.Del(n,nb);

};
//
//////////////////////////////////////////////////////////////////////////
//
bool vui_BrigInfo::SetFromBrig(vui_SelPoint* SP){
	byte NI=SP->NI;
	BR=CITY[NI].Brigs+BrigID;
	if(BR->WarType==0) return false;
	GeneralObject* GO=NATIONS[NI].Mon[BR->MembID];

	OneObject* Obj=NULL;
	NewMonster* NM=NULL;
	AdvCharacter* ADC=NULL;

	int L=0;
	int N=0;
	int NS=0;
	int T=0;
	RifleAttack=0;
	int Delay=0;
	int MaxDelay=0;
	//ActiveState=0;
	//MaxLife=GO->MoreCharacter->BirthLife;
	MaxLife=0;
	SP->rX=0;
	SP->rY=0;
	
	for(int i=NBPERSONAL;i<BR->NMemb;i++){
		word MID=BR->Memb[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB){
				if(!Obj){
					Obj=OB;
					NM=Obj->newMons;
					ADC=Obj->MoreCharacter;
				}
				L+=OB->Life;
				N++;
				if(!OB->delay)NS++;
				T+=OB->GetTired/1000;
				if(OB->RifleAttack)RifleAttack=1;
				Delay+=OB->delay;
				MaxDelay+=OB->MaxDelay;
				OIS.ActiveState|=(1<<OB->ActivityState);
				if(OB->MaxLife>MaxLife){
					MaxLife=OB->MaxLife;
				}
				SP->rX+=OB->RealX;
				SP->rY+=OB->RealY;
			}
		}
	}
	if(MaxLife==0) MaxLife=1;
	if(N){
		L/=N;
		T/=N;
		Delay/=N;
		MaxDelay/=N;
		NLiveMembers=N;
		Life=L;
		SP->rX/=N;
		SP->rY/=N;
		//ReadyPercent=NS*100/N;
	}else{
		NLiveMembers=1;
		Life=1;
		MaxDelay=1;
		//ReadyPercent=0;
	}		
	ReadyPercent=MaxDelay?100*(MaxDelay-Delay)/MaxDelay:100;
	NMembers=BR->NMemb-NBPERSONAL;
	if(BR->Morale>=0) Morale=BR->Morale/10000;
		else Morale=0;	
	MaxMorale=BR->MaxMorale/10000;	
	if(MaxLife==0) MaxLife=1;
	NShots=NS;	
	Grenades=BR->NGrenades;
	GrenadesMax=GO->newMons->MaxGrenadesInFormation*NMembers/100;
	Tiring=T;
	isGrenaders=GO->newMons->MaxGrenadesInFormation;
	WeapFileID=NM->BigWeapFile;
	WeapColdSprite=NM->BigColdWeapSprite;
	WeapFireSprite=NM->BigFireWeapSprite;
	Shield=0;//ADC->Shield;
	ShieldAdd=BR->AddShield;
	int dam,adam;

	// extracting max defence add values
	ZeroMemory(MaxDefenceAdd, sizeof(MaxDefenceAdd));
	MaxDefenceAdd[0] = MaxDefenceAdd[1] = MaxDefenceAdd[2] = BR->AddShield;

	BR->GetBrigadeProtection(3,dam,adam);
	Defence[0]=dam;
	DefenceAdd[0]=adam;

	BR->GetBrigadeProtection(0,dam,adam);
	Defence[1]=dam;
	DefenceAdd[1]=adam;

	BR->GetBrigadeProtection(2,dam,adam);
	Defence[2]=dam;
	DefenceAdd[2]=adam;

	BR->GetBrigadeProtection(1,dam,adam);
	Defence[3]=dam;
	DefenceAdd[3]=adam;

	// extracting max damage additional values
	ZeroMemory(MaxDamageAdd, sizeof(MaxDamageAdd));
	MaxDamageAdd[0] = MaxDamageAdd[1] = BR->AddDamage;

	for(int i=0;i<3;i++){
		BR->GetBrigadeDamage(i,dam,adam);
		Damage[i]=dam;
		DamageAdd[i]=i==0?adam:0;
		int wk=ADC->WeaponKind[i];
		WeapType[i]=wk;		
	}
	if(Obj&&Obj->ActiveAbility){
		int dam=Damage[0];
		if(dam){
			Obj->ActiveAbility->ActiveAbilities.modifyDamage(0,dam,dam,NULL);
			Damage[0]=dam;
		}
	}
	if(Damage[1]==0){
		Damage[1]=Damage[2];
		DamageAdd[1]=DamageAdd[2];
		WeapType[1]=WeapType[2];
	}
	NKills=BR->GetBrigExp();
	// Fill
	isFillable=BR->InStandGround&&REALTIME>BR->FillDelay;
	// Reform
	byte fID=BR->GetFormIndex();
	memset(FormID,0xFF,sizeof(FormID));
	OrderDescription* ODE=ElementaryOrders+BR->WarType-1;
	if(ODE->GroupID!=0xFF){
		SingleGroup* FGD=FormGrp.Grp+ODE->GroupID;
		for(int j=0;j<FGD->NCommon&&j<3;j++){
			FormID[j]=FGD->IDX[j];
			if(fID==FormID[j]){
				CurForm=j;
			}
		}
	}
	ScaleFactor=BR->ScaleFactor;
	// Shot Line
	for(int p=0;p<3;p++){
		ShotLine[p]=0xFFFF;
	}
	int NA=ODE->NActualLines;
	if(NA&&GO->newMons->ArmAttack){
		int IDXS[3]={-1,-1,-1};
		bool LENB[3]={false,false,false};
		bool LPRS[3]={false,false,false};
		int FL=ODE->FirstActualLine;
		int bp=NBPERSONAL;
		int CP=0;		
		for(p=0;p<NA;p++){		
			int NU=ODE->LineNU[p+FL];
			if(NU){
				if(CP<3){
					IDXS[CP]=p;
					for(int q=0;q<NU;q++){
						if(bp<BR->NMemb){
							word ID=BR->Memb[bp];
							if(ID!=0xFFFF){
								OneObject* OB=Group[ID];
								if(OB&&OB->Serial==BR->MembSN[bp]){
									LPRS[CP]=true;
									if(OB->RifleAttack)LENB[CP]=true;
								};
							};
							bp++;
						};
					};
				};
				CP++;
			};
		}
		if(CP==3){
			for(int p=0;p<3;p++){
				if(LPRS[p]&&IDXS[p]!=-1){
					if(LENB[p]){
						ShotLine[p]=1*8*1024+IDXS[p];
					}else{
						ShotLine[p]=IDXS[p];
					}
				}
			}
		}
	}
	// Stand ground
	BrigDelay=BR->BrigDelay;
	BrigDelayMax=BR->MaxBrigDelay?BR->MaxBrigDelay:1;
	if(!BR->InStandGround&&BrigDelay==0) BrigDelay=BrigDelayMax;
	AttEnm=BR->AttEnm;
	NoOrder=BR->NewBOrder==NULL;	
	return true;
}
//////////////////////////////////////////////////////////////////////////////////
// cva_OIS_Scroll
int oisSX=0;
int oisLx=10000;
int oisSDx=64;
void cva_OIS_Scroll::SetFrameState(SimpleDialog* SD){
	SD->Visible=(OIS.SPSideLx<oisLx)&&(OIS.SelPoint.GetAmount()>1);
}
// cva_OIS_ScrollLeft
bool cva_OIS_ScrollLeft::LeftClick(SimpleDialog* SD){
	oisSX-=oisSDx;
	if(oisLx+oisSX<OIS.SPSideLx) oisSX=OIS.SPSideLx-oisLx;
	return true;
}
// cva_OIS_ScrollRight
bool cva_OIS_ScrollRight::LeftClick(SimpleDialog* SD){
	oisSX+=oisSDx;
	if(oisSX>0) oisSX=0;
	return true;
}
// cva_OIS_Fantasy
void cva_OIS_Fantasy::SetFrameState(SimpleDialog* SD){
	int NSP=OIS.SelPoint.GetAmount();
	if( NSP==1 && !Multy ){		
		SD->Visible=true;
	}else
	if( NSP>1 && Multy ){
		SD->Visible=true;
		ListDesk* LD=dynamic_cast<ListDesk*>(SD);
		if(LD){
			// установить кнопки управлени€ героем
			bool InitSP=false;
			int d=0;
			for(int i=0;i<NSP;i++){
				vui_SelPoint* SP=OIS.SelPoint+i;
				if( SP && SP->Sort==UnitType ){
					if( UnitType==ve_UT_hero && !InitSP ){
						InitSP=true;
						OIS.LastSP=i;
					}
					if( d>=LD->DSS.GetAmount() ){
						LD->AddElement("");
					}
					SimpleDialog* sdE=LD->DSS[d];
					sdE->Visible=true;
					sdE->ID=i;
					d++;
				}
			}
			int n=LD->DSS.GetAmount();
			for(;d<n;d++){
				SimpleDialog* sdE=LD->DSS[d];
				sdE->Visible=false;
			}
		}
	}else{
		SD->Visible=false;
	}
}
// cva_OIS_Rome
cva_OIS_Rome::cva_OIS_Rome(){
	mNLines=2;
	mWidth=60+4;
	mHeight=90+4;
};
void cva_OIS_Rome::SetFrameState(SimpleDialog* SD){

	SD->Visible=false;

	int NSP=OIS.SelPoint.GetAmount();
	if(NSP>0){
		
		SD->Visible=true;

		int W=0;
		DialogsDesk* ddS=ddSingle.Get();
		DialogsDesk* ddM=ddMulti.Get();
		if(ddS&&ddM){
			if(NSP==1){

				ddS->Visible=true;
				ddM->Visible=false;

				W=ddS->x+ddS->GetWidth();
				OIS.SPSideLx=W;

			}else{

				// установить кнопки управлени€ героем
				for(int i=0;i<NSP;i++){
					vui_SelPoint* SP=OIS.SelPoint+i;
					if( SP && SP->Sort==ve_UT_hero ){
						OIS.LastSP=i;
						break;
					}
				}

				ddS->Visible=false;
				ddM->Visible=true;

				int d=0;
				//int i=0;

				for(int t=0;t<5;t++){
					//if(d<ddM->DSS.GetAmount()){
					if(t<ddM->DSS.GetAmount()){
						DialogsDesk* ddType=(DialogsDesk*)ddM->DSS[t];
						ListDesk* LD=dynamic_cast<ListDesk*>(ddType);

						//GP_TextButton* tbTitle=DD->Find("Title");
						//GPPicture* gpFirst=DD->Find("First");

						GPPicture* ddFirst=NULL;
						int x=0,y=0,w=30,h=30;
						int j=0;
						if( LD==NULL && ddType->DSS.GetAmount()>2 ){
							ddFirst=(GPPicture*)ddType->DSS[2];
							x=ddFirst->Getx();
							y=ddFirst->Gety();
							w=ddFirst->GetWidth();
							h=ddFirst->GetHeight();
							j=2;
						}

						int dW=1;

						int NLines=mNLines;
						w=mWidth;
						h=mHeight;

						bool find=false;
						for(int i=0;i<NSP;i++){
							vui_SelPoint* SP=OIS.SelPoint+i;
							if(SP->Sort==t){
								find=true;
								GPPicture* DD=NULL;
								if(j<ddType->DSS.GetAmount()){
									DD=(GPPicture*)ddType->DSS[j];
								}else{
									if(ddFirst){
										DD=new GPPicture;
										ddFirst->Copy(DD);
										ddType->DSS.Add(DD);
									}else{
										if(LD){
											LD->AddElement("");
											DD=(GPPicture*)ddType->DSS[j];
										}
									}
								}
								if(DD){
									DD->Visible=true;
									DD->ID=i;
									if(j-2<NLines*2){
										DD->Setx(x+((j-2)%2)*w);
										DD->Sety(y+((j-2)/2)*h);
									}else{
										dW=(2+(j-2-NLines*2)/NLines);
										DD->Setx(x+dW*w);
										DD->Sety(y+((j-2-NLines*2)%NLines)*h);									
									}
								}
								j++;
							}
						}					
						for(;j<ddType->DSS.GetAmount();j++){
							ddType->DSS[j]->Visible=false;
						}
						if(find){
							ddType->Visible=true;
							GP_TextButton* tbTitle=NULL;
							if(ddType->DSS.GetAmount()>1){
								tbTitle=(GP_TextButton*)ddType->DSS[1];
								_str txt;
								txt.print("#SPTitle_%d",t);
								tbTitle->SetMessage(GetTextByID(txt.pchar()));
							}
							d++;
						}
					}			
				}
				if(d>0){
					int Lx=ddM->GetWidth();
					oisLx=ddM->DSS[d-1]->x1-ddM->DSS[0]->x;
					if(oisLx<Lx){
						OIS.SPSideLx=oisLx;
						oisSX=0;
					}else{
						OIS.SPSideLx=Lx;
					}				
				}			
				for(;d<ddM->DSS.GetAmount();d++){
					ddM->DSS[d]->Visible=false;				
				}

				W=OIS.SPSideLx+50;

			}
		}

		SD->SetWidth(W);
	}

}

int cva_OIS_Produce::curID=0;
void cva_OIS_Produce::SetFrameState(SimpleDialog* SD){
	
	SD->Visible=false;

	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if( SPClass.Check(SP) ){

		SD->Visible=true;

		//int N=OIS.GetProduceAmount();
		int N=OIS.GetProduceAmountByAIndex();
		//SD->Visible=N;

		ListDesk* LD=dynamic_cast<ListDesk*>(SD);
		if(LD){
			int Need=N-LD->DSS.GetAmount();
			for(;Need>0;Need--){
				LD->AddElement(NULL);
			}
		}

		curID=0;
	}
}

int cva_OIS_Upgrade::curID=0;
void cva_OIS_Upgrade::SetFrameState(SimpleDialog* SD){
	
	SD->Visible=false;

	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP){

		SD->Visible=true;

		//int N=OIS.GetUpgradeAmount();
		int N=OIS.GetUpgradeAmountByIconPosition();

		ListDesk* LD=dynamic_cast<ListDesk*>(SD);
		if(LD){
			int Need=N-LD->DSS.GetAmount();
			for(;Need>0;Need--){
				LD->AddElement(NULL);
			}
		}

		curID=0;
	}
}

cva_OIS_ControlPanel::cva_OIS_ControlPanel(){
	HideForPeasants=true;
	HideForBuildings=true;
}
void cva_OIS_ControlPanel::SetFrameState(SimpleDialog* SD){

	static int Width=SD->GetWidth();

	SD->Visible=false;
	
	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP){

		SD->Visible=true;

		int W=Width;
		if( HideForPeasants && SP->Type==ve_UT_units && SP->Inf.Units.Peasant){
			W=26;
            SD->Visible=false;
		}
		if( HideForBuildings && SP->Type==ve_UT_building ){
			W=26;
            SD->Visible=false;
		}
		SD->SetWidth(W);
	}

}

void cva_OIS_Abilities::SetFrameState(SimpleDialog* SD){
	
	SD->Visible=false;

	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP){

		SD->Visible=true;

	}
}

bool InsideDialogState=true;
bool GetInsideDialogState(){
	return InsideDialogState;
}
void SetInsideDialogState(bool State){
	InsideDialogState=State;
}

void cva_OIS_Inside::SetFrameState(SimpleDialog* SD){
	
	SD->Visible=false;

	vui_SelPoint* SP=OIS.GetLastSelPoint();
	if(SP){

		OneObject* OB=SP->OB;
		SD->Visible=( InsideDialogState && !OB->newMons->NewParams.ControlButtonsDisable );

		//(SP->Type==ve_UT_building&&SP->Inf.Buildings.Amount==1||
		//SP->OB->NInside&&SP->Type==ve_UT_units&&SP->Inf.Units.Amount==1)){
		
		if(SD->Visible){
            ListDesk* UL=(ListDesk*)UnitsList.Get();
            if(UL){
                word* ID=OB->Inside;

                DynArray<word> InLstN;
                DynArray<word> InMaxN;
                DynArray<word> NIndex;

                for(int i=0;i<OB->NInside;i++){
                    OneObject* ob=Group[ID[i]];
                    if(ob){
                        AdvCharacter* adc=ob->MoreCharacter;
                        int idx=-1;
                        for(int j=0;j<NIndex.GetAmount();j++){
                            if(ob->NIndex==NIndex[j]){
                                InLstN[j]++;
                                idx=j;
                                break;
                            }else				
                            if(ob->NIndex>NIndex[j]){
                                InLstN.Insert(j,1);
                                InMaxN.Insert(j,adc->MaxInside+ob->AddInside);
                                NIndex.Insert(j,ob->NIndex);
                                idx=j;
                                break;
                            }
                        }
                        if(idx==-1){
                            InLstN.Add(1);
                            InMaxN.Add(adc->MaxInside+ob->AddInside);
                            NIndex.Add(ob->NIndex);
                        }
                    }
                }

                int nUL=UL->DSS.GetAmount();
                int n=NIndex.GetAmount();
                for(i=0;i<n;i++){

                    SimpleDialog* SD=NULL;
                    if(i<nUL){
                        SD=UL->DSS[i];
                    }else{
                        int uid=UL->AddElement(10000,"",0);
                        SD=UL->GetElementByUID(uid);
                    }
                    if(SD){
                        SD->Visible=true;
                        SD->ID=NIndex[i];
                        SD->UserParam=InLstN[i];
                    }

                }
                for(;i<nUL;i++){
                    UL->DSS[i]->Visible=false;
                }


            }
		}

	}
}

//
cvs_BrigPanel vBrigPanel;
void SetBrigPanel(cvs_BrigPanel& BP){
	vBrigPanel=BP;
};
void BrigPanelShowAll(){	
	vBrigPanel.Restore();
};
// Bink Voideo Interface Commands for Scripting
//  0 - nothing
//	1 - MOVE motion
//	2 - ATTMOVE motion with attack
//	3 - rifle enable
//	4 - rifle disable
//	5 - melee
//	6 - grenade
//	7 - line 0 enable
//	8 - line 1 enable
//	9 - line 2 enable
// 10 - line 0 disable
// 11 - line 1 disable
// 12 - line 2 disable
// 13 - 
// 14 - 
// 15 - 
// 16 - disband formation
// 17 - fill formation
// 18 - stop
// 20 - 39 - forma tion ID (line,squre,care and e.t.) Nres.dat - [ORDERICONS]
// 40 - resorce not enought coal
// 41 - resorce not enought food
// 42 - army not enought living places (neLivingPlaces)
// 43 - detect enemy far
// 44 - detect enemy near
// 45 - detect enemy very close
// 46 - 
//
// 50 - settlement captured
// 51 - settlement lost
// 60 - 
// 61 - enemy make damage
// 62 - enemy melee
// 63 - enemy cannon make damage
// 64 - enemy cavalery make damage
// 70 - 
// 73 - formation lost (neBrigadeLost)
// 74 - formation defeat enemy (neBrigadeTerminated)
// 75 - game victory
// 76 - game defeat
// 80 - 
// 81 - first select
// 82 - peasant voice
// 90 -
// 92 - detect enemy cannon
// 93 - enemy cannon dispirited
// 94 - my cannon dispirited
// 95 - enemy cannon captured
// 96 - my cannon captured
// 100 - cannon fire yadro
// 101 - cannon fire
// 102 - cannon reload yadro
// 103 - cannon reload kartech
// 104 - cannon autofire on
// 105 - cannon autofire off
// 106 - cannon fill
// 107 - cannon turn
// 108 - cannon move
// 110 -
// 111 - detect enemy brigade near my settlemnt
// 112 - detect enemy settlemnt
// 113 - detect enemy brigade near my town center
// 114 - detect enemy town center
// 115 - detect enemy brigade near my strategic location
// 116 - detect enemy strategic location
// 120 - 
// 121 - my settlement protect last brigade
// 122 - enemy settlement protect last brigade
// 123 - my town center protect last brigade
// 124 - enemy town center protect last brigade
// 125 - my strategic location protect last brigade
// 126 - enemy strategic location protect last brigade

// 
void ACT(int x){ 
	if(GSets.CGame.SilenceMessageEvents) return;
	if(OIS.SelPoint.GetAmount()==1&&OIS.SelPoint[0].Type==ve_UT_brigade){
		LastNI=OIS.SelPoint[0].NI;
		LastBID=OIS.SelPoint[0].Inf.Brigade.BrigID;
		ProcessVideoForBrigade(&VideoSeq,LastNI,LastBID,x,FI_File,1,RealLx-535,RealLy-185,-14,-21,0);
	}
	extern ActiveObjectIndexses ActiveIndexses;
	ActiveIndexses.FormationSituation.FinalStateOrder=x;
	if(x){
		//
		void ExecSoundEvents();
		ExecSoundEvents();
	}
}
//////////////////////////////////////////////////////////////////////////////////

OISelection OIS;

OISelection::OISelection(){	
}

void OISelection::COnBeginDragFn::EvaluateFunction(){
	
	OISelection* ois=get_parent<OISelection>();
	if(ois){
		ois->DragState=-1;

		SimpleDialog* EDrag=SimpleDialog::CallParams.Dragged;//->GetDialogConnection().BC;
		SimpleDialog* EDrop=SimpleDialog::CallParams.DropOver;//->GetDialogConnection().BC;
		if(EDrag/*&&EDrop*/){

			if(Shop){
				ois->DragState=0;
			}else
			if(Basket){
				ois->DragState=1;
			}else
			if(Body){
				ois->DragState=2;
			}

		}
	}

	Shop=false;
	Basket=false;
	Body=false;
}

extern int LastMx;
extern int LastMy;

void OISelection::COnDropFn::EvaluateFunction(){
	OISelection* ois=get_parent<OISelection>();
	if(ois){

		vui_SelPoint* SP=OIS.GetLastSelPoint();
		byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
		
		SimpleDialog* EDrag=SimpleDialog::CallParams.Dragged;
		SimpleDialog* EDrop=SimpleDialog::CallParams.DropOver;

		if( SP && SP->NI==NI && EDrag && EDrop ){
			
			if( (ois->DragState==1||ois->DragState==2) && Shop ){ // продать

				SimpleDialog* SD=EDrag;
				ActiveUnitAbility* AA=SP->GetActiveUnitAbility(SD->ID,SD->UserParam);
				if (AA && AA->TypeAbil!=0) {

					UnitAbility* UA=AA->GetA();

					if(UA && SP->OB->ActiveAbility && SP->OB->ActiveAbility->LibID!=0xFFFF ){
						OneObject* Lib=Group[SP->OB->ActiveAbility->LibID];
						if(Lib&&Lib->Serial==SP->OB->ActiveAbility->LibSN){
							//sell
							int GetAbilitySellPrice(OneObject* Seller, ActiveUnitAbility* AA);
							int price = GetAbilitySellPrice(SP->OB,AA);
							vui_IS_AbilityTrade AT(SP->OB,Lib,AA,NI,price);
							AT.InterfAction();
						}
					}
				}

			}else if( ois->DragState==0 && ( Body || Basket ) ){ // купить

				SimpleDialog* SD=EDrag;
				ActiveUnitAbility* AA=SP->GetActiveUnitAbility(SD->ID,SD->UserParam);
				if( AA && AA->TypeAbil!=0 ){

					OneObject* Lib=Group[SP->OB->ActiveAbility->LibID];
					if(Lib&&Lib->Serial==SP->OB->ActiveAbility->LibSN){

						UnitAbility* UA=AA->GetA();
						if( UA && SD->UserParam==adLibShop ){
							// buy
							int res=XRESRC(NI,MoneyID);
							int GetAbilityBuyPrice(OneObject* Buyer, ActiveUnitAbility* AA);
							int price = GetAbilityBuyPrice(SP->OB,AA);
							if( price>0 && price<=res ){
								vui_IS_AbilityTrade AT(Lib,SP->OB,AA,NI,-price);
								AT.InterfAction();
							}
						}
					}

				}

			}else if( ois->DragState==2 && Basket ){ // сн€ть

				SimpleDialog* SD=EDrag;
				ActiveUnitAbility* AA=SP->GetActiveUnitAbility(SD->ID,SD->UserParam);
				if( AA && AA->TypeAbil!=0 ){

					UnitAbility* UA=AA->GetA();
					int Index=-1;
					if(UA){
						Index=UA->Index;
					}

					vui_IS_Abl_ClothedSlot IS;
					IS.Data.NI=MyNation;
					IS.Data.NIndex=SP->NIndex;
					IS.Data.AblID=Index;
					IS.Data.AblSerial=AA->Serial;
					IS.Data.Clothed=false;
					IS.Data.Slot=EDrop->ID;
					IS.InterfAction();
				}

			}else if( ois->DragState==1 && Body ){ // одеть

				SimpleDialog* SD=EDrag;
				ActiveUnitAbility* aaDrag=SP->GetActiveUnitAbility(SD->ID,SD->UserParam);
				if( aaDrag ){
					UnitAbility* uaDrag=aaDrag->GetA();
					if( uaDrag ){
						if( aaDrag->TypeAbil==4 && (EDrop->UserParam==uaDrag->TypeClothe+adHelm || EDrop->UserParam==adItemsTakeOff) ||
							aaDrag->TypeAbil==5 && (EDrop->UserParam==adItems || EDrop->UserParam==adBasketBottles)){
								vui_IS_Abl_ClothedSlot IS;
								IS.Data.NI=MyNation;
								IS.Data.NIndex=SP->NIndex;
								IS.Data.AblID=-1;
								IS.Data.AblSerial=aaDrag->Serial;
								IS.Data.Clothed=true;
								IS.Data.Slot=EDrop->ID;
								IS.InterfAction();
						}
					}
				}

			}else if( ois->DragState==1 && Basket || ois->DragState==2 && Body ){ // помен€ть слот
				
				if( EDrag->UserParam==EDrop->UserParam ){
					ActiveUnitAbility* AADrag=SP->GetActiveUnitAbility(EDrag->ID,EDrag->UserParam);
					ActiveUnitAbility* AADrop=SP->GetActiveUnitAbility(EDrop->ID,EDrop->UserParam);
					if(AADrag){
						AADrag->Slot=EDrop->ID;
						if(AADrop){
							AADrop->Slot=EDrag->ID;
						}
					}
				}

			}else if( (ois->DragState==1||ois->DragState==2) && Ground ){ // выбросить

				SimpleDialog* SD=EDrag;
				//UnitActiveAbilityList* MA=SP->ActAbl;
				ActiveUnitAbility* AA=SP->GetActiveUnitAbility(SD->ID,SD->UserParam);
				if( AA && (AA->TypeAbil==4||AA->TypeAbil==5) ){
					vui_IS_Abl_TakeOff IS(AA,MyNation,LastMx,LastMy);
					IS.InterfAction();
				}

			}

		}
	}

	Shop=false;
	Basket=false;
	Body=false;
	Ground=false;
	SimpleDialog::CallParams.Dragged=NULL;
};

void CmdCancelStandGround(byte NI);
void CmdStandGround(byte NI);

void OISelection::HoldPositionLeftClick::EvaluateFunction() {
	OISelection* P=get_parent<OISelection>();
	if(P){
		vui_SelPoint* SP=OIS.GetLastSelPoint();
		byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];
		if( SP && SP->NI==NI ){
			//if(P->HoldPosition){
			//	CmdCancelStandGround(NI);
			//}else{
				CmdStandGround(NI);
			//}
		}
	}
};

//void VUI_IExchangeInterface::SetUnitInfo(OneObject* OB, int* Price){
//	if(OB){
//		oi_UnitInfo* UI=&OI.UnitInfo;
//		UI->PriceShow=Price;
//		//int Price[6];
//		//int Attack[2];
//		//int Defence[4];
//		//int KillCount;
//		UI->LifeMax=OB->MaxLife;
//		UI->ManaMax=OB->MaxMana;
//		//int AttackRate;
//		//int AttackDistance;
//		//int MotionSpeed;
//		//char* Message;
//	}
//};

void cDialogsHistory::SetMessage(SimpleDialog* SD){
	if(Income){
		Income=false;
		((TextButton*)SD)->SetMessage(Text.pchar());
	}
};
