#ifndef __VICTORYCONDITIONS_H__
#define __VICTORYCONDITIONS_H__
//#include "stdheader.h"
#include <more_types.h>
#include "LoadSave.h"
#include "ActiveZone.h"

//////////////////////////////////////////////////////////////////////////

#define MaxNatColors 7

//////////////////////////////////////////////////////////////////////////

class VictoryConditionRoot:public BaseClass{
public:	
	//char* GameF1Text;
	_str GameF1Text;
	char* VicMessage;
	char* DefMessage;
	virtual ~VictoryConditionRoot(){};
	virtual char* GetID(){return "???";};
	virtual void CheckVictoryAndDefeat(){};
	virtual void DrawOnMiniMap(int x,int y){};
	virtual void GetPlayerStatusString(byte NI,char* Dest){};
	virtual void InitOnCurrentMap(){
		//if(GameF1Text&&GameF1Text[0]){
		/*
		if(!GameF1Text.isClear()){
			extern _str vm_F1_VictoryCond;
			vm_F1_VictoryCond=newstr(GetTextByID(GameF1Text.pchar()));
		}
		*/
	};	
	virtual void SaveData(SaveBuf* SB){};
	virtual void LoadData(SaveBuf* SB){};
	virtual void Reset(){};
	virtual char* CheckIfValidForThisMap(){return NULL;}//returns NULL if valid, othervice the cause of failure 
	SAVE(VictoryConditionRoot){
        //REG_MEMBER(_textid, GameF1Text);
		//REG_AUTO(GameF1Text);
		REG_MEMBER(_textid, VicMessage);
		REG_MEMBER(_textid, DefMessage);
	}ENDSAVE;
};

//////////////////////////////////////////////////////////////////////////

const int VC_KingOfTheHillPretendingTime = 20;
const int VC_KingOfTheHillVictoryTime    = 300;

class VC_TheKingOfTheHill:public VictoryConditionRoot{	
	int xc,yc,r;
    int CurrentOwner;
	int CurrentPretender;
    int OwnerStartTime;
	int PretenderStartTime;
public:
	int PretendingTime;
	int VictoryTime;
	VC_TheKingOfTheHill(){
		PretendingTime=20;
		VictoryTime=300;
	}
	virtual char* GetID(){return "VC_KingOfTheHill";}
	virtual void CheckVictoryAndDefeat();
	virtual void DrawOnMiniMap(int x,int y);
	virtual void GetPlayerStatusString(byte NI,char* Dest);
	virtual void InitOnCurrentMap();
	virtual void SaveData(SaveBuf* SB);
	virtual void LoadData(SaveBuf* SB);
	virtual void Reset();
	virtual char* CheckIfValidForThisMap();
	SAVE(VC_TheKingOfTheHill);
		REG_PARENT(VictoryConditionRoot);
		REG_MEMBER(_int,PretendingTime);
		REG_MEMBER(_int,VictoryTime);
	ENDSAVE;
};
class VC_BaseFantasy: public VictoryConditionRoot {
	LinearArray<bool,_bool> InitPeasants;
	LinearArray<bool,_bool> InitTownHalls;
	int NextRefreshTime;
public:
	int MinimalPeasants;
	int MinimalTownHalls;
	VC_BaseFantasy(){ 
		Reset();
		GameF1Text=newstr(GetTextByID("#BaseFantasyF1"));
		VicMessage=newstr(GetTextByID("#BaseFantasyVic"));
		DefMessage=newstr(GetTextByID("#BaseFantasyDef"));
	}
	virtual char* GetID(){ return "VC_BaseFantasy"; }
	virtual void CheckVictoryAndDefeat();
	virtual void DrawOnMiniMap(int x,int y){};
	virtual void GetPlayerStatusString(byte NI,char* Dest);
	virtual void InitOnCurrentMap();
	virtual void SaveData(SaveBuf* SB){};
	virtual void LoadData(SaveBuf* SB){};
	virtual void Reset();
	virtual char* CheckIfValidForThisMap(){ return NULL; };
	SAVE(VC_BaseFantasy){
		REG_PARENT(VictoryConditionRoot);
		REG_MEMBER(_int,MinimalPeasants);
		REG_MEMBER(_int,MinimalTownHalls);
		SetInvisibleMode(); REG_AUTO(InitPeasants);
		SetInvisibleMode(); REG_AUTO(InitTownHalls);
	}ENDSAVE;
};
class VC_DestroyTownHalls:public VictoryConditionRoot {
	bool Init[MaxNatColors];
	int Amount[MaxNatColors];
	int NextRefreshTime;
public:
	int MinimalAmount;
	int TimeToBuildMinimal;
	VC_DestroyTownHalls(){ 
		Reset();
		GameF1Text=newstr(GetTextByID("#DestroyTownHallsF1"));
		VicMessage=newstr(GetTextByID("#DestroyTownVic"));
		DefMessage=newstr(GetTextByID("#DestroyTownDef"));
	}
	virtual char* GetID(){ return "VC_DestroyTownHalls"; }
	virtual void CheckVictoryAndDefeat();
	virtual void DrawOnMiniMap(int x,int y){};
	virtual void GetPlayerStatusString(byte NI,char* Dest);
	virtual void InitOnCurrentMap();
	virtual void SaveData(SaveBuf* SB){};
	virtual void LoadData(SaveBuf* SB){};
	virtual void Reset();
	virtual char* CheckIfValidForThisMap(){ return NULL; };
	SAVE(VC_DestroyTownHalls);
		REG_PARENT(VictoryConditionRoot);
		REG_MEMBER(_int,MinimalAmount);	
		REG_MEMBER(_int,TimeToBuildMinimal);			
		for(int i=0;i<MaxNatColors;i++){
			SetInvisibleMode();
			char cc[32];
			sprintf(cc,"Init%d",i);
			REG_MEMBER_EX2(_bool,Init[i],cc);
			sprintf(cc,"Amount%d",i);
			SetInvisibleMode();
			REG_MEMBER_EX2(_int,Amount[i],cc);
		}
	ENDSAVE;
};
class VC_AnnihilateHero:public VictoryConditionRoot{
	int NHero;
	int Amount[MaxNatColors];
public:
	VC_AnnihilateHero(){ 
		Reset();	
		GameF1Text=newstr(GetTextByID("#VC_HeroHint"));
		VicMessage=newstr(GetTextByID("#AnnihilateHeroVic"));
		DefMessage=newstr(GetTextByID("#AnnihilateHeroDef"));
	}
	virtual char* GetID(){ return "VC_AnnihilateHero"; }
	virtual void CheckVictoryAndDefeat();
	virtual void DrawOnMiniMap(int x,int y){};
	virtual void GetPlayerStatusString(byte NI,char* Dest);
	virtual void InitOnCurrentMap();
	virtual void SaveData(SaveBuf* SB){};
	virtual void LoadData(SaveBuf* SB){};
	virtual void Reset();
	virtual char* CheckIfValidForThisMap(){ return NULL; };
	SAVE(VC_AnnihilateHero)
		REG_PARENT(VictoryConditionRoot);
		REG_MEMBER(_int,NHero);
		for(int i=0;i<MaxNatColors;i++){
			SetInvisibleMode();
			char n[32];
			sprintf(n,"Amount%d",i);
			REG_MEMBER_EX2(_int,Amount[i],n);
		}
	ENDSAVE;
};
class VC_ScoreGame:public VC_DestroyTownHalls{	
public:
	int TimeLimit;
	int LeftTime;
	VC_ScoreGame(){
		GameF1Text=newstr(GetTextByID("#VC_ScoreHint"));
		VicMessage=newstr(GetTextByID("#ScoreGameDef"));
		DefMessage=newstr(GetTextByID("#ScoreGameVic"));
	}
	virtual char* GetID(){ return "VC_ScoreGame"; }
	virtual void CheckVictoryAndDefeat();
	virtual void DrawOnMiniMap(int x,int y);
	virtual void GetPlayerStatusString(byte NI,char* Dest);
	virtual void InitOnCurrentMap();
	virtual void SaveData(SaveBuf* SB){};
	virtual void LoadData(SaveBuf* SB){};	
	virtual char* CheckIfValidForThisMap(){ return NULL; };
	SAVE(VC_ScoreGame);
		REG_PARENT(VictoryConditionRoot);
		REG_MEMBER(_int,TimeLimit);
		SetInvisibleMode();
		REG_MEMBER(_int,LeftTime);
	ENDSAVE;
};
class VC_CaptureGame:public VC_ScoreGame{	
public:	
	VC_CaptureGame(){
		GameF1Text=newstr(GetTextByID("#VC_MinesHint"));
		VicMessage=newstr(GetTextByID("#CaptureGameDef"));
		DefMessage=newstr(GetTextByID("#CaptureGameVic"));
	}
	virtual char* GetID(){ return "VC_CaptureGame"; }
	virtual void CheckVictoryAndDefeat();
	//virtual void DrawOnMiniMap(int x,int y){};
	virtual void GetPlayerStatusString(byte NI,char* Dest);
	//virtual void InitOnCurrentMap();
	//virtual void SaveData(SaveBuf* SB){};
	//virtual void LoadData(SaveBuf* SB){};
	//virtual void Reset();
	virtual char* CheckIfValidForThisMap(){ return NULL; };
	SAVE(VC_CaptureGame);
		REG_PARENT(VictoryConditionRoot);
		SetInvisibleMode();
		REG_MEMBER(_int,LeftTime);
	ENDSAVE;
};
//
/*
class c_CapturableZone: public BaseClass{
	byte Owner;
public:
	_AZone AZone;
	SAVE(c_CapturableZone);
		//REG_PARENT(ActiveZoneArray);
		REG_AUTO(AZone);		
	ENDSAVE;
};
*/

class carr_CapturableZone: public ClonesArray<_AZone>{
public:	
	virtual const char* GetThisElementView();
	virtual bool MoveElement(int index,char opType){ return false; }
	virtual int ExpandWith(const char* ElmName,void* base);
	virtual bool DelElement(int Index);
	//AUTONEW(carr_CapturableZone);
	SAVE(carr_CapturableZone)
	ENDSAVE;
};
class VC_CZDB_Player: public BaseClass{
public:
	int Nzones;		// if 0 - defeat
	int Nbrigs;		// if 0 - defeat
	byte VictState;	// not synchro from Nation.VictState
	bool DetectDefeat(byte NMask=0);
	//
	//byte Team;
	//
	bool Init;
	byte ColorID;
	int AmountForLoose;
	carr_CapturableZone AZones;	
	SAVE(VC_CZDB_Player)
		REG_MEMBER(_bool,Init);
		SAVE_SECTION(2);		
		//REG_MEMBER(_BYTE,ColorID);
		REG_MEMBER(_int,AmountForLoose);
		REG_AUTO(AZones);
	ENDSAVE;
	DWORD GetClassMask(){
		DWORD M=1;
		if(Init) M|=2;
		return 0xFFFFFFFF; //M
	}
};
class VC_CaptureZonesDestroyBrigades:public VictoryConditionRoot{
public:
	VC_CaptureZonesDestroyBrigades(){
		for(int i=0;i<MaxNatColors;i++){
			VC_CZDB_Player* p=Player+i;
			p->ColorID=i;			
		}
		GameF1Text=newstr(GetTextByID("#CaptureZonesDestroyBrigadesF1"));
	}	
	VC_CZDB_Player Player[7];	
	virtual char* GetID(){ return "VC_CaptureZonesDestroyBrigades"; }
	virtual void CheckVictoryAndDefeat();
	virtual void DrawOnMiniMap(int x,int y){};
	virtual void GetPlayerStatusString(byte NI,char* Dest);
	virtual void InitOnCurrentMap();
	//virtual void SaveData(SaveBuf* SB){};
	//virtual void LoadData(SaveBuf* SB){};
	//virtual void Reset();
	virtual char* CheckIfValidForThisMap(){ return NULL; };
	SAVE(VC_CaptureZonesDestroyBrigades);
		REG_PARENT(VictoryConditionRoot);
		for(int i=0;i<MaxNatColors;i++){
			char cc[32];
			sprintf(cc,"Player%d",i);
			REG_AUTO_EX2(Player[i],cc);
		}
	ENDSAVE;
};

//////////////////////////////////////////////////////////////////////////

_inline void RegisterAllVictoryConditions(){
	REG_CLASS(VictoryConditionRoot);
	//
	REG_CLASS(VC_BaseFantasy);
	REG_CLASS(VC_TheKingOfTheHill);
	REG_CLASS(VC_DestroyTownHalls);
	REG_CLASS(VC_ScoreGame);
	REG_CLASS(VC_CaptureGame);
	REG_CLASS(VC_AnnihilateHero);
	//
	REG_CLASS(_AZone);
	//REG_CLASS(c_CapturableZone);
	REG_CLASS(carr_CapturableZone);
	REG_CLASS(VC_CZDB_Player);
	REG_CLASS(VC_CaptureZonesDestroyBrigades);

}

//////////////////////////////////////////////////////////////////////////

#endif //__VICTORYCONDITIONS_H__