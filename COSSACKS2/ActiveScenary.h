#pragma once
#include "MapTemplates.h"
//
class UnitsGroup;
int RemoveNUnitsFast(UnitsGroup* DST,UnitsGroup* SRC,int N,int NIndex);
//
class UnitsGroup;
struct GrpOrder;
typedef void GrpLink(UnitsGroup* UG);
struct GrpOrder{
	GrpOrder* Next;
	GrpLink* Link;
	void* Data;
	int DataSize;
	DWORD ID;
};
void CheckDynamicalPtr(void* ptr);
class UnitsGroup;
class UnitsGroup{
public:
	word* IDS;
	word* SNS;
	int N;
	int Max;

	byte NI;
	byte NMASK;
	int Index;
	GrpOrder* GOrder;

	//-----------debug---------
	char* file;
	int   Line;
	//-------------------------

	void Clear(){
		N=0;
	}
	void Free(){
		Max=0;
		if(IDS){ free(IDS);	IDS=NULL; }
		if(SNS){ free(SNS);	SNS=NULL; }
	}

	GrpOrder* CreateOrder(byte Type);
	void DeleteLastOrder();
	void ClearOrders();
	void Allocate(int n){
		CheckDynamicalPtr(IDS);
		CheckDynamicalPtr(SNS);
		if(n>Max){
			Max=n+32;
			IDS=(word*)realloc(IDS,Max<<1);
			SNS=(word*)realloc(SNS,Max<<1);
		};
		CheckDynamicalPtr(IDS);
		CheckDynamicalPtr(SNS);
	};
	void AddNewUnit(OneObject* OB);
	void RemoveUnit(OneObject* OB);
	inline void RemoveUnits(UnitsGroup* Dest, int N, word NIndex){
		RemoveNUnitsFast(Dest,this,N,NIndex);
	}
	void CopyUnits(UnitsGroup* Dest,int start,int Nc,bool add,bool remove){
		CheckDynamicalPtr(IDS);
		CheckDynamicalPtr(SNS);
		if(start>=N)start=N-1;
		if(start<0)start=0;
		if(start+Nc>N)Nc=N-start;
		if(add){
			Dest->Allocate(Dest->N+Nc);
			memcpy(Dest->IDS+Dest->N,IDS+start,Nc<<1);
			memcpy(Dest->SNS+Dest->N,SNS+start,Nc<<1);
			Dest->N+=Nc;
		}else{
			Dest->Allocate(Nc);
			memcpy(Dest->IDS,IDS+start,Nc<<1);
			memcpy(Dest->SNS,SNS+start,Nc<<1);
			Dest->N=Nc;
		};
		if(remove&&Nc){
			int ncp=(N-start-Nc)<<1;
			memcpy(IDS+start,IDS+start+Nc,ncp);
			memcpy(SNS+start,SNS+start+Nc,ncp);
			N-=Nc;
		};
		CheckDynamicalPtr(IDS);
		CheckDynamicalPtr(SNS);
	};
	UnitsGroup(){
		memset(this,0,sizeof *this);
	};
	~UnitsGroup(){
		ClearOrders();
		if(IDS)free(IDS);
		if(SNS)free(SNS);
		memset(this,0,sizeof *this);
	};
	void RefreshUnits(){
		int NR=0;
		for(int i=0;i<N;i++){
			if(IDS[i]!=0xFFFF){
				OneObject* OB=Group[IDS[i]];
				if(OB&&OB->Serial==SNS[i]){
					IDS[NR]=IDS[i];
					SNS[NR]=SNS[i];
                    NR++;
				}
			}
		}
		N=NR;
	}
};
struct UnitsPosition{
	word* Type;
	int*  coor;
	int N;
};
struct ZonesGroup{
	int Index;
	word* ZoneID;
	int N;
};
typedef void StdVoid();
struct GTimer{
	int Time;
	bool Used;
	bool First;
	int LastCheckAnimTime;
	bool trueTime;
};
struct LightSpot{
	int x,y,Type;
};
class ScenaryInterface{
public:
	void** SaveZone;
	int *  SaveSize;
	int    NSaves;
	int    MaxSaves;
	HINSTANCE hLib;
	char*  DLLName;

	DynArray<UnitsGroup> UGRP;	
	DynArray<ZonesGroup> ZGRP;

	char** Messages;
	int    NMess;
	int    MaxMess;

	char** Sounds;
	int    NSnd;
	int    MaxSnds;

	int NErrors;

	//char*  MissText;
	//int    TextSize;

	int NPages;
	int MaxPages;
	char** Page;
	int*   PageSize;
	char** PageID;
	char** PageBMP;

	bool   TextDisable[52];
	
	bool   StandartVictory;
	bool   Victory;
	char*  VictoryText;

	bool   LooseGame;
	char*  LooseText;

	char*  CentralText;
	int CTextTime;

	GTimer TIME[32];
	word   TRIGGER[512];
	LightSpot LSpot[64];
	StdVoid* ScenaryHandler;
	ScenaryInterface();
	~ScenaryInterface();
	void Load(char* Name,char* Text);
	void UnLoading();
	_inline UnitsGroup* GetNewGroup(){
		for(int i=0;i<UGRP.GetAmount();i++){
			if(UGRP[i].Index==-1){
				UGRP[i].Index=i;
				return &UGRP[i];
			}
		}
		UnitsGroup UG;
		UG.Index=UGRP.GetAmount();
		UGRP.Add(UG);
		return &UGRP[UG.Index];
	}
	_inline UnitsGroup* AddNewGroup(){
		UnitsGroup UG;
		UG.Index=UGRP.GetAmount();
		UGRP.Add(UG);
		return &UGRP[UG.Index];
	}
	_inline int GetNewGroupIndexEx(char* File,int Line){
		UnitsGroup* UG=GetNewGroup();
		UG->file=File;
		UG->Line=Line;
		return UG->Index;
	}
	_inline void DeleteGroup(int Index){
		if(Index>=0&&Index<=UGRP.GetAmount()){
			UGRP[Index].ClearOrders();
			UGRP[Index].N=0;
			UGRP[Index].Index=-1;
		}
	}
	_inline ZonesGroup* GetNewZone(){
		for(int i=0;i<ZGRP.GetAmount();i++){
			if(ZGRP[i].Index==-1){
                ZGRP[i].N=0;
				ZGRP[i].ZoneID=NULL;
				ZGRP[i].Index=i;
				return &ZGRP[i];
			}
		}
		ZonesGroup ZG;
		ZG.N=0;
		ZG.ZoneID=NULL;
		ZG.Index=ZGRP.GetAmount();
		ZGRP.Add(ZG);
		return &ZGRP[ZG.Index];
	}
	_inline void DeleteZone(int idx){
		if(idx>=0&&idx<ZGRP.GetAmount()){
			ZGRP[idx].Index=-1;
		}
	}
};
#define GetNewGroupIndex() GetNewGroupIndexEx(__FUNCTION__,__LINE__)
extern ScenaryInterface SCENINF;
class SingleMission{
public:
	char* ID;
	char* DLLPath;
	char* MapName;
	char* Name;
	char* Description;
	int NIntro;
	char** Intro;
};
class MissPack{
public:
	int GetID(char* Name){
		for(int i=0;i<NMiss;i++){
			if(!strcmp(MISS[i].ID,Name)){
				return i;
			}
		}
		return -1;
	}
	int GetIDbyName(char* Name){
		for(int i=0;i<NMiss;i++){
			if(!strcmp(MISS[i].Name,Name)){
				return i;
			}
		}
		return -1;
	}
	SingleMission* MISS;
	int NMiss;
	int MaxMiss;
	int CurrentMission;
	int* SingleMS;
	int* RoSMS;  //<-------
	int MSMiss;
	int RoSMiss; //<--------
	void LoadMissions();
	MissPack();
	~MissPack();
};

struct OneSinBattle{
	word GID;	// global index
	SingleMission* SM;

	word DifBonus[5];
	
	word ArmyMen[64];
	word ArmyBonus[64];
	int NArmyBonus;

	byte ArmyValue[4096];

	word GetMaxDifBonus(){ return DifBonus[4];}

	word GetArmyBonus(word Men){ for(int i=NArmyBonus-1;i>=0;i--) if(Men>ArmyMen[i]) return ArmyBonus[i]; return 0; }
	word GetMaxArmyBonus(){ return ArmyBonus[NArmyBonus-1];}

	bool Load(word id);
};

struct SinBattles{
	SinBattles();
	void Init();
	OneSinBattle Battle[128];
	int N;
};

class OneBattle{
public:
	char* ID;
	char* Map;
	char* Text;
	char* Brief;
	char* BigMap;
	char* MiniMap;
	char* BigHeader;
	char* SmallHeader;
	char* Date;
	char* RedUnits;
	char* BlueUnits;
	int NHints;
	char** Hints;
	int* Coor;
};
class OneWar{
public:
	char* Name;
	char* Date;
	char* Text;
	int NBatles;
	int* BattleList;
};
class WarPack{
public:
	int NWars;
	OneWar* Wars;
	int NBattles;
	OneBattle* Battles;
	WarPack();
	~WarPack();
};
extern WarPack WARS;
extern MissPack MISSLIST;
struct SingleCampagin{
	char* CampMessage;
	char* CampText;
	char* CampBmp;
	int NMiss;
	int* Miss;
	DWORD* OpenIndex;
};
class CampaginPack{
public:
	int NCamp;
	SingleCampagin* SCamp;
	void Init();
	CampaginPack();
	~CampaginPack();
};
extern CampaginPack CAMPAGINS;
extern byte CurAINation;
extern City* CCIT;
extern Nation* CNAT;
extern bool AiIsRunNow;
extern int CurrentCampagin;
extern int CurrentMission;
CEXPORT void LoadPlayerData();
CEXPORT void SavePlayerData();

CEXPORT void ShowMessageII(int FileID,int SpriteID,char* MessageID);