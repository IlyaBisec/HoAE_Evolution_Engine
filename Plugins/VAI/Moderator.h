
#pragma once

class Soldier;
class Squadron;
class Moderator;

//class SoldierRef : public ClassRef<Soldier> {
//public:
//};

class Soldier {
public:
	// base constant
	byte NI;
	word NIndex;

	// current states
	int State;	// 0 - поиск цели, 1 - бой с защитником цели, 2 - атака цели, 3 - окружаем цель

	// context
	int TimeLastSetContext;

	iVector2D Center;
	int Top;

	int Life;
	int Damage;
	int WeaponKind;
	int Damage2;
	int WeaponKind2;

	int Price; // сумма по ресурсам, потом можно учесть с каким ресурсом (наличие и скорость добычи) у противника проблема в данный момент
	int PriceDrone;
	int PriceMiner;

public:

	Soldier();

	virtual void Process() {}
	virtual void SetContext() {}
	virtual void Tactics() {}

	virtual void Clear();
	virtual void Add(pUnit& u);

	void GetForce(Soldier& Enemy, int& Force, int& eForce );
	int GetForceRelative(Soldier& Enemy);
	int GetForcePercent(Soldier& Enemy);

};

class SoldierMap : public uni_hash<Soldier,DWORD,503> {
public:
};

class Squad : public ReferableBaseClass, public Soldier {

public:
	// отладочная информация
	DynArray<int> TargetWay;

public:
	ClassRef<Moderator> Shepherd;
	ClassRef<Squadron> Deacon;
	pUnitsList Body;

	DynArray<short> DangerMap;
	DynArray<word> TargetMap;

	// unit lists
	//ClonesArray<SoldierRef> Clients; // юниты, которые нужно охранять
	//ClonesArray<SoldierRef> Defenders; // охранники

	//ClonesArray<SoldierRef> Targets; // вражеские юниты, которые необходимо атаковать
	//ClonesArray<SoldierRef> Opponents; // защитники вражеских юнитов
	//ClonesArray<SoldierRef> Suspects; // возможные защитники

	pUnitsList Reinforce;
	int ReinforceTop;
	int TimeLastReinforce;

	int TargetArmyID;		// вражеская армия, которую нужно вести

	SAVE(Squad){
		
		REG_AUTO(Shepherd);
		REG_AUTO(Deacon);
		REG_AUTO(Body);

		REG_MEMBER(_BYTE,NI);
		REG_MEMBER(_WORD,NIndex);

		REG_MEMBER(_int,State);
		REG_MEMBER(_int,TimeLastSetContext);
		REG_MEMBER(_int,Top);

		REG_MEMBER(_int,Life);
		REG_MEMBER(_int,Damage);
		REG_MEMBER(_int,WeaponKind);
		REG_MEMBER(_int,Damage2);
		REG_MEMBER(_int,WeaponKind2);

		REG_MEMBER(_int,Price);
		REG_MEMBER(_int,PriceDrone);
		REG_MEMBER(_int,PriceMiner);

		REG_MEMBER(_BYTE,Owner);
		REG_MEMBER(_int,LastMoveTime);
		REG_MEMBER(_int,LastTop);

		REG_AUTO(Reinforce);
		REG_MEMBER(_int,ReinforceTop);
		REG_MEMBER(_int,TimeLastReinforce);

	}ENDSAVE;

	Squad();

	virtual void Process();
	virtual void SetContext();
	virtual void Tactics();

	virtual void Clear();
	virtual void Add(pUnit& u);
	void Refresh();

public:
	// 
	GAMEOBJ Group;

	bool SingleUnitMotion;

	GAMEOBJ StrGrp;
	word StormID;
	word StormSN;

	byte Owner;

	int LastTop;

	int LastMoveTime;

	int Difficulty;

	bool CannonGuard;	// охрана пушки

};

class SquadRef : public ClassRef<Squad> {
public:
};

class Squadron : public Squad {
public:
	ClassRef<Moderator> Commander;
	ClonesArray<SquadRef> Staff;

	// Target
	int Task; // 0 - Clean area, 1 - Protect client, 2 - Attack target
	iVector2D tPoint;
	//pUnitsList tUnits;
	Squad tUnits;

	SAVE(Squadron){
		REG_PARENT(Squad);
		
		REG_AUTO(Commander);
		REG_AUTO(Staff);

		REG_MEMBER(_int,Task);

	}ENDSAVE;

	virtual void Process();
};

class TypeLinks : public BaseClass {
public:
	word Type;
	DynArray<int> Links;
};

class TypeStats: public BaseClass {
public:
	int NIndex; // Type
	// статистика
	int BattleNumber;
	int LastBattleTime;
	int AcquiredDamageInLastBattle;
	int AllBattleTime;
	int AllAcquiredDamage;
	int AllAcquiredDamageFromSpell;
	// настройки
	int SquadStrengthMax;
	int DesiredNextBattleTime;

	SAVE(TypeStats){
		REG_MEMBER(_int,NIndex);
	}ENDSAVE;
};

class Army: public BaseClass {
public:
	int Action;			// действие по отношению к этой армии
	int FrontDistance;	// минимальное расстояние от ведущих до этой армии, фронт ( 0 - нападаем )
	SAVE(Army){
	}ENDSAVE;
};

class Moderator : public Squad {
public:

	bool gInit;
	int gFear[256];
	int stDangTime;
	short* stDang[8];

public:
	byte NI;

	SoldierMap AllSoldiers;					// вражеские солдаты
	DynArray<bool> ProtectBaseMap;			// зоны, нуждающиеся в охране

	ClonesArray<Squad> Squads;				// отряды модератора
	ClonesArray<Squadron> Squadrons;		// эскадроны, они выполняют задачи модератора

	ClonesArray<TypeLinks> UnitTypeLinks;	// подчинение одних типов юнитов другим по NIndex

	DynArray<bool> BuildingYard;			// карта топологических зон разрешенных для строительства
	DynArray<int> DistFromBase;				// растояние топологии от базы
	iVector2D BaseCenter;					// первое здание базы
	word BaseTopCenter;

	DynArray<int> SpellDangerTime;			// время действия вражеских спелов
	DynArray<int> SpellDanger;				// опасность вражеских спелов
	int SpellDangerSafeTopTime;				// время следующего создания карты обхода вражеских спелов
	DynArray<word> SpellDangerSafeTop;		// карта безопастных зон, для обхода вражеских спелов

	DynArray<word> ArmiesMap;				// местонахождение врагов
	DynArray<word> ArmiesFrontMap;			// цели для наших отрядов, чтобы окружать врагов

	// статистика
	ClonesArray<TypeStats> Stats;			// статистика и настройки для типов войск
	TypeStats* GetStats(int Type);
	
	void ProcessReinforce(Squad* SQ);

	SAVE(Moderator){
		REG_AUTO(Squads);
		REG_AUTO(Squadrons);
		REG_MEMBER(_WORD,BaseTopCenter);
		REG_AUTO(Stats);
	}ENDSAVE;

	__constructor(Moderator){
		BaseTopCenter=0xFFFF;
	}

	void Clear();
	void InitProcess();
	virtual void Process();

	bool AddGrp(GAMEOBJ* Grp);

	bool CheckCleanArea(iVector2D& A);
	int GetDistFromBase(int top);
	
	int GetSplashBonus(word NIndex);
	void SetSplashBonus(word NIndex, int Bonus);

	//void ClearUnitTypeLinks();
	void SetUnitTypeLinks(word NIndex, LinearArray<int,_int>* Links);
	DynArray<int>* GetUnitTypeLinks(word NIndex);

	void OnCastSpell(UnitAbility* UA, int x, int y, int r);
	
	void ProcessResearch();
	bool ProcessGameObj(Squad* Grp);
};

class Battlefield : public BaseClass {

public:
	// отладочная информация
	SquadRef CurSquad;
	
public:
	bool ShowDebugInfo;
	Moderator M[8];

	DynArray<int> SplashBonus; // влияние сплеша по NIndex

	SAVE(Battlefield){
		REG_MEMBER(_bool,ShowDebugInfo);
		REG_AUTO(M[0]);
		REG_AUTO(M[1]);
		REG_AUTO(M[2]);
		REG_AUTO(M[3]);
		REG_AUTO(M[4]);
		REG_AUTO(M[5]);
		REG_AUTO(M[6]);
		REG_AUTO(M[7]);
	}ENDSAVE;

};

extern Battlefield BF;

///////////////////////////////////////////////////////////////////////////////////////////////

#define CIMPORT __declspec(dllimport)
#define DLLIMPORT extern "C" __declspec(dllimport)

struct OneUnit{
	word Index;
	word Serial;
	word Life;
	word MaxLife;
	byte AddDamage;
	byte AddShield;
	word Stage;
	word MaxStage;
	word Kills;
	word NIndex;
	byte Usage;
	byte Building;
	int  x,y;
	byte NI;
	byte Direction;
	byte IsBuilt;
	byte ChargeProgress;
	byte GroundState;
	byte NMask;
	byte Busy;
	byte Attack;
	byte Reserved[8];
};

DLLIMPORT int GetRND(int Max);
CIMPORT int maiGetDangIteration();
DLLIMPORT void CreateDangerMapForStorm(byte NI,short* DangMap,int MaxSize,int* FearArray,int ItrAmount);
DLLIMPORT int GetNZones();
DLLIMPORT void DeleteHiddenUnits(GAMEOBJ* Grp);
DLLIMPORT void LimitUnitsMotion(GAMEOBJ* Grp);
DLLIMPORT int CleanGroup(GAMEOBJ* Grp);
DLLIMPORT DWORD GetCurrentPurpose(GAMEOBJ* GRP);
DLLIMPORT void RemoveGroup(GAMEOBJ* Source,GAMEOBJ* Dest);
DLLIMPORT bool GetGrpCenter(GAMEOBJ* Grp,int* x,int* y);
DLLIMPORT int GetTopZone(int x,int y);
CIMPORT void maiSetGroupID(word ID, byte Type);
DLLIMPORT bool GetUnitInfo(GAMEOBJ* Units,int Index,OneUnit* Uni);
//CIMPORT bool GiveHeroOrder( byte NI, bool& Retreat, int& MoveArea );
DLLIMPORT bool GetTopZRealCoor(int Zone,int* x,int* y);
DLLIMPORT int GetZonesDist(int Start,int Fin);
DLLIMPORT bool AttackEnemyInZone2(GAMEOBJ* Grp,GAMEOBJ* Zone,byte NI);

class UnitsGroup{
public:
	word* IDS;
	word* SNS;
	int N;
	int Max;

	byte NI;
	byte NMASK;
	int Index;
	void* GOrder;

	//-----------debug---------
	char* file;
	int   Line;
	//-------------------------

};

CIMPORT UnitsGroup* GetUnitsGroup(GAMEOBJ* Group);
DLLIMPORT void SGP_MoveToPoint(byte NI,GAMEOBJ* Grp,int x,int y,int Direction,int DF,int DR,DWORD Flags);
DLLIMPORT word MakeOneShotToBuild(GAMEOBJ* Zone,GAMEOBJ* Grp,byte NI);
DLLIMPORT void SetDangerMap(int* Map);
CIMPORT void maiSetDangerMap(int* DangerMap);
DLLIMPORT int GetNUnits(GAMEOBJ* Units);
CIMPORT void SetMinEnemy(int N);
DLLIMPORT void CreateTopListEnArmy(word* IDS,byte NI,int MinPS);
CIMPORT void maiSetTargets(word* Targets);
DLLIMPORT int FindNextZoneOnTheSafeWayToObject(int Start,short* DangerMap,word* ObjIds,int* MaxDanger,int DangSteps,word* DestObj);
CIMPORT void maiSetWayArray();
DLLIMPORT bool GetUnitGlobalInfo(int Index,OneUnit* Uni);
DLLIMPORT OneObject* GetOBJ(int Index);
DLLIMPORT void OBJ_SendTo(word Index,int x,int y,byte Prio,byte OrdType);
DLLIMPORT int GetTopDist(int xa,int ya,int xb,int yb);
DLLIMPORT int GetZonesDist2(int Start,int Fin,byte TopType);
CIMPORT word GetDirEx(int dx,int dy);
DLLIMPORT void SetAI_Guard(GAMEOBJ* Grp,bool val);
DLLIMPORT void SetGroupNMASK(GAMEOBJ* Grp,byte NMASK);
DLLIMPORT byte GetNatNMASK(byte NI);

DLLIMPORT void RegisterDynGroupEx(GAMEOBJ* Units,char*,int);
#define RegisterDynGroup(x) RegisterDynGroupEx(x,__FUNCTION__,__LINE__)

DLLIMPORT void CopyUnits(GAMEOBJ* Src,GAMEOBJ* Dest,int start,int n,bool add,bool remove);
DLLIMPORT int GetLastFullWay(word** buf);
DLLIMPORT int GetMyNation();