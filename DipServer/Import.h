#if !defined(DMCR_EXPORT_FOR_MISSION)
#define DMCR_EXPORT_FOR_MISSION

# define DIALOGS_API

#include "stdafx.h"
#include "stdio.h"
#define DLLEXPORT extern "C" __declspec(dllexport)
#define DLLIMPORT extern "C" __declspec(dllimport)

#define CEXPORT __declspec(dllexport)
#define CIMPORT __declspec(dllimport)

#pragma pack(1)

typedef unsigned short word;
typedef unsigned char byte;
struct OneLinkInfo{
	word  NextAreaID;
	word  NextAreaDist;	
	DWORD LinkNatMask;
};
/*
struct GAMEOBJ{
	word Index;
	word Serial;
	int  Type;
	//
	bool isGroup();
	word FindUnits(word ID, word SN);
};
*/

//-------------External functions---------------//
DLLIMPORT int GetMaxAttRadius(int Index);
DLLIMPORT
void AssignNation(byte Src,byte Dst);
DLLIMPORT
bool RegisterUnits(GAMEOBJ* GOBJ,char* Name);
DLLIMPORT
bool RegisterString(GAMEOBJ* GOBJ,char* ID);
DLLIMPORT
bool RegisterSound(GAMEOBJ* GOBJ,char* Name);
DLLIMPORT
void RegisterVar(void* Var,int size);
DLLIMPORT
void RegisterZone(GAMEOBJ* GOBJ,char* Name);
DLLIMPORT
void RegisterVisibleZone(GAMEOBJ* GOBJ,char* Name);
DLLIMPORT
bool RegisterUnitType(GAMEOBJ* GOBJ,char* Name);
DLLIMPORT
bool RegisterUpgrade(GAMEOBJ* GOBJ,char* Name);
DLLIMPORT
void InitialUpgrade(char* Grp,char* Upgrade);
DLLIMPORT
bool RegisterFormation(GAMEOBJ* GOBJ,char* Name);
DLLIMPORT
bool RegisterUnitsForm(GAMEOBJ* GOBJ,char* Name);
DLLIMPORT
int  GetUnitsAmount0(GAMEOBJ* Zone,byte Nation);
DLLIMPORT
int  GetUnitsAmount1(GAMEOBJ* Zone,GAMEOBJ* Units);
DLLIMPORT
int  GetUnitsAmount2(GAMEOBJ* Zone,GAMEOBJ* UnitType,byte Nation);
DLLIMPORT
int  GetTotalAmount0(GAMEOBJ* Units);
DLLIMPORT
int  GetTotalAmount1(GAMEOBJ* UnitType,byte Nation);
DLLIMPORT
int  GetReadyAmount(GAMEOBJ* UnitType,byte Nation);
DLLIMPORT
bool IsUpgradeDoing(GAMEOBJ* Upgrade,byte Nation);
DLLIMPORT
bool IsUpgradeDone(GAMEOBJ* Upgrade,byte Nation);
DLLIMPORT
int GetAmountOfWarriors(byte Nat); // не работает
DLLIMPORT
bool IsUpgradeEnabled(GAMEOBJ* Upgrade,byte Nation);
DLLIMPORT
int  GetDied(GAMEOBJ* UnitType,byte Nation);
DLLIMPORT
bool CreateObject0(GAMEOBJ* DstObj,GAMEOBJ* Form,GAMEOBJ* UnitType,byte NatID,GAMEOBJ* Zone,byte Direction);
DLLIMPORT
void ClearSelection(byte Nat);
DLLIMPORT
void SelectUnits(GAMEOBJ* Units,bool Add);
DLLIMPORT
void SelectUnitsType(GAMEOBJ* UnitsType,byte Nat,bool Add);
DLLIMPORT
void SelectUnits1(byte Nat,GAMEOBJ* Units,bool Add);
DLLIMPORT
int GetNInside(byte Nat);
DLLIMPORT
int GetMaxInside(byte Nat);
DLLIMPORT
void PushUnitAway(byte Nat);
DLLIMPORT
bool CheckLeaveAbility(byte Nat);
DLLIMPORT
void SendUnitsToTransport(byte Nat);
DLLIMPORT
void PushAllUnitsAway(byte Nat);
DLLIMPORT
void AttackZoneByArtillery(GAMEOBJ* ArtGroup,GAMEOBJ* Zone,byte Nat);
DLLIMPORT
void AttackBuildingsInZone(GAMEOBJ* ArtGroup,GAMEOBJ* Zone,byte Nat);
DLLIMPORT
bool SelDie(byte Nat);
DLLIMPORT
bool SelOpenGates(byte Nat);
DLLIMPORT
bool SelCloseGates(byte Nat);
DLLIMPORT
bool SelSendTo(byte Nat,GAMEOBJ* Zone,byte Dir,byte Type);
DLLIMPORT
bool SelSendAndKill(byte Nat,GAMEOBJ* Zone,byte Dir,byte Type);
DLLIMPORT
void SelAttackGroup(byte Nat,GAMEOBJ* Enemy);
DLLIMPORT
bool Patrol(byte Nat,GAMEOBJ* Zone,byte Dir);
DLLIMPORT
void ChangeFriends(byte Nat,byte Flags);
DLLIMPORT
void SelChangeNation(byte SrcNat,byte DstNat);
DLLIMPORT
void SetStandGround(byte Nat,byte val);
DLLIMPORT
void AllowAttack(byte Nat,byte val);
DLLIMPORT
void SelAutoKill(byte Nat);
DLLIMPORT
void HINT(GAMEOBJ* Hint,int time);
DLLIMPORT
void DisableMission(char MISSID);
DLLIMPORT
void EnableMission(char MISSID);
DLLIMPORT
void SetVictoryText(char* ID);
DLLIMPORT
void SetLooseText(char* ID);
DLLIMPORT void ShowVictory(char* Message=NULL, byte NI=0xFF);
DLLIMPORT void LooseGame(char* Message=NULL, byte NI=0xFF);
DLLIMPORT
void ShowCentralText(char* ID,int time);
DLLIMPORT
void ShowPage(char* Name);
DLLIMPORT
void ShowPageParam(char* Name,...);
DLLIMPORT
bool AskQuestion(char* Name);
DLLIMPORT
int GetResource(byte Nat,byte ID);
DLLIMPORT
void AddResource(byte Nat,byte ID,int Amount);
DLLIMPORT
void SetResource(byte Nat,byte ID,int Amount);
DLLIMPORT
int GetUnitCost(byte Nat,GAMEOBJ* UnitType,byte ResID);
DLLIMPORT
int GetUpgradeCost(byte Nat,GAMEOBJ* Upgrade,byte ResID);
DLLIMPORT
void RunTimer(byte ID,int Long);
DLLIMPORT
bool TimerDone(byte ID);
DLLIMPORT
bool TimerDoneFirst(byte ID);
DLLIMPORT
bool TimerIsEmpty(byte ID);
DLLIMPORT
void FreeTimer(byte ID);
DLLIMPORT
int GetTime(byte ID);
DLLIMPORT
int GetGlobalTime();
DLLIMPORT
byte Trigg(byte ID);
DLLIMPORT
void SetTrigg(byte ID,byte Val);
DLLIMPORT
void RunAI(byte Nat);
DLLIMPORT
void RunAIWithPeasants(byte Nat,char* P_Name);
DLLIMPORT
void SetStartPoint(GAMEOBJ* Zone);
DLLIMPORT
bool UnitsCenter(GAMEOBJ* DstZone,GAMEOBJ* Units,word R);
DLLIMPORT
bool SelCenter(GAMEOBJ* DstZone,byte Nat,int R);
DLLIMPORT
bool CreateZoneNearUnit(GAMEOBJ* DstZone,GAMEOBJ* Zone,GAMEOBJ* UnitType,byte Nat,int R);
DLLIMPORT
bool CreateZoneNearGroup(GAMEOBJ* DstZone,GAMEOBJ* Zone,GAMEOBJ* Grp,int R);
DLLIMPORT
void SetLightSpot(GAMEOBJ* Zone,int R,byte index);
DLLIMPORT
void ClearLightSpot(byte index);
DLLIMPORT
int GetTopDst(GAMEOBJ* Z1,GAMEOBJ* Z2);
DLLIMPORT
void AttackEnemyInZone(GAMEOBJ* Grp,GAMEOBJ* Zone,byte EnmNation);
DLLIMPORT
void StartAI(byte Nat,char* Name,int Land,int Money,int ResOnMap,int Difficulty);
DLLIMPORT
void DoNotUseSelInAI(byte Nat);
DLLIMPORT
void SetAIProperty(byte NAT,int Prop,int Val);
DLLIMPORT
int AskComplexQuestion(int Nx,char* Name1,byte or1,char* Name2,byte or2,char* Quest);
DLLIMPORT
void SetTutorial(bool State);
DLLIMPORT
bool GetQuestPressed();
DLLIMPORT
void DisableUpgrade(byte Nat,GAMEOBJ* Upg);
DLLIMPORT
void EnableUnit(byte Nat,GAMEOBJ* Type,bool State);
DLLIMPORT
int GetMyNation();
DLLIMPORT
void SelErase(byte NI);
//--------------------------NEW!!!-----------------------//
DLLIMPORT
int AskMultilineQuestion(int Nx,char* Name1,byte or1,char* Quest);
DLLIMPORT
void TakeFood(GAMEOBJ* Units);
DLLIMPORT
void TakeWood(GAMEOBJ* Units);
DLLIMPORT
void TakeStone(GAMEOBJ* Units);
DLLIMPORT
void RepairBuildingsBySel(byte Nat,GAMEOBJ* Buildings);
DLLIMPORT
bool CheckBuildingsComplete(GAMEOBJ* Buildings);
DLLIMPORT
int GetKilled(GAMEOBJ* Units);
DLLIMPORT
int GetUnitsByNation(GAMEOBJ* Units,byte Nat);
DLLIMPORT
void ProduceUnit(GAMEOBJ* Units,GAMEOBJ* UnitType,GAMEOBJ* DestGroup);
DLLIMPORT
void ProduceUnitFast(GAMEOBJ* Units,GAMEOBJ* UnitType,GAMEOBJ* DestGroup,int Speed);
DLLIMPORT
bool CreateBuilding(byte Nat,GAMEOBJ* Zone,GAMEOBJ* UnitType,GAMEOBJ* DestGroup);
DLLIMPORT
void SetDestPoint(GAMEOBJ* Units,GAMEOBJ* Zone);
DLLIMPORT void RegisterDynGroupEx(GAMEOBJ* Units,char*,int);
#define RegisterDynGroup(x) RegisterDynGroupEx(x,__FUNCTION__,__LINE__)
DLLIMPORT
bool NationIsErased(byte Nat);
DLLIMPORT
void SetReadyState(GAMEOBJ* Units,bool State);
DLLIMPORT
int GetNUnits(GAMEOBJ* Units);
DLLIMPORT
void ShowAlarm(GAMEOBJ* Zone);
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
DLLIMPORT
void RefreshScreen();
DLLIMPORT
bool GetUnitInfo(GAMEOBJ* Units,int Index,OneUnit* Uni);
DLLIMPORT
void SetUnitInfo(OneUnit* Uni);
DLLIMPORT
void RemoveGroup(GAMEOBJ* Source,GAMEOBJ* Dest);
DLLIMPORT
void SaveSelectedUnits(byte NI,GAMEOBJ* Units,bool Add);
DLLIMPORT
bool CheckProduction(GAMEOBJ* Units);
DLLIMPORT
void SelectUnitsInZone(GAMEOBJ* Zone,byte Nation,bool add);
DLLIMPORT
void SelectTypeOfUnitsInZone(GAMEOBJ* Zone,GAMEOBJ* Type,byte Nation,bool add);
DLLIMPORT
void ChangeUnitParam(GAMEOBJ* Type,byte NI,int AddDamage,int DamType,int AddShield);
DLLIMPORT
int GetRandomIndex();
/////////////////////////////////////////////////////////////
//                                                         //
//                       AAA     IIII                      //
//                      AA AA     II                       //
//                     AA   AA    II                       //
//                     AAAAAAA    II                       //
//                     AA   AA   IIII                      //
//                                                         //
/////////////////////////////////////////////////////////////
DLLIMPORT
void SetAIRegister(int Reg,int Val);
DLLIMPORT
int GetAIRegister(int Reg); // Default=0
DLLIMPORT
int GetAINation();
DLLIMPORT
bool TryUnit(GAMEOBJ* UnitType,int Max,byte CostPercent,byte Probability);
DLLIMPORT
bool TryUpgrade(GAMEOBJ* Upgrade,int CostPercent,int Probability);
DLLIMPORT
void SetMineBalanse(int N,word* Bal);
DLLIMPORT
void SetPDistribution(int OnFood,int OnWood,int OnStone);
DLLIMPORT
void AssignAmountOfMineUpgrades(int MU);
DLLIMPORT
bool AssignMineUpgrade(word U,char* Str,word val);
DLLIMPORT
void SET_MINE_CAPTURE_RADIUS(int x);
DLLIMPORT
void SET_MINE_UPGRADE1_RADIUS(int x);
DLLIMPORT
void SET_MINE_UPGRADE2_RADIUS(int x);
DLLIMPORT
void SET_DEFAULT_MAX_WORKERS(int x);
DLLIMPORT
void SET_MIN_PEASANT_BRIGADE(int x);
DLLIMPORT
int GetMoney(byte id);
DLLIMPORT
int GetUnits(GAMEOBJ* UnitType);
DLLIMPORT
int GetReadyUnits(GAMEOBJ* UnitType);
DLLIMPORT
bool UpgIsDone(GAMEOBJ* Upgrade);
DLLIMPORT
bool UpgIsRun(GAMEOBJ* Upgrade);
DLLIMPORT
void AssignMine(char* Name);
DLLIMPORT
void AssignPeasant(char* Name);
DLLIMPORT
void AssignHouse(char* Name);
DLLIMPORT
void AssignWall(char* Name);
DLLIMPORT
bool FieldExist();
DLLIMPORT
int GetDifficulty();
DLLIMPORT
int GetStartRes();
DLLIMPORT
int GetResOnMap();
DLLIMPORT
int GetLandType();
DLLIMPORT
void MissErrorMessage(char*,char*);
//--------------
DLLIMPORT
void SetStandartVictory();
DLLIMPORT
void SetPlayerName(byte Nat,char* ID);
DLLIMPORT
int GetDiff(byte NI);
DLLIMPORT
void DoMessagesBrief();
inline int  GetTotalAmount(GAMEOBJ* Units){
	return GetTotalAmount0(Units);
};
inline int  GetTotalAmount(GAMEOBJ* UnitType,byte Nation){
	return GetTotalAmount1(UnitType,Nation);
};
//------------------IDS-----------------//
#define NoneID		0x00

#define MelnicaID	0x01
#define MelnicaIDS  "MELNICA"

#define FarmID		0x02
#define FarmIDS		"FARM"

#define CenterID	0x03
#define CenterIDS	"CENTER"

#define SkladID		0x04
#define SkladIDS	"SKLAD"

#define TowerID		0x05
#define TowerIDS	"TOWER"

#define FieldID		0x06
#define FieldIDS	"FIELD"

#define MineID		0x07
#define MineIDS		"MINE"

#define FastHorseID	0x08
#define FastHorseIDS "FASTHORSE"

#define MortiraID	0x09
#define MortiraIDS  "MORTIRA"

#define PushkaID	0x0A
#define PushkaIDS   "PUSHKA"

#define GrenaderID  0x0B
#define GrenaderIDS "GRENADER"

#define HardWallID  0x0C
#define HardWallIDS "HARDWALL"

#define WeakWallID  0x0D
#define WeakWallIDS "WEAKWALL"

#define LinkorID	0x0E
#define LinkorIDS	"LINKOR"

#define WeakID		0x0F
#define WeakIDS	    "WEAK"

#define FisherID	0x10
#define FisherIDS	"FISHER"

#define ArtDepoID	0x11
#define ArtDepoIDS  "ARTDEPO"

#define SupMortID	0x12
#define SupMortIDS	"SUPERMORTIRA"

#define PortID		0x13
#define PortIDS	    "PORT"

#define LightInfID	0x14
#define LightInfIDS	"LIGHTINFANTRY"

#define StrelokID	0x15
#define StrelokIDS	"STRELOK"

#define HardHorceID	0x16
#define HardHorceIDS "HARDHORCE"

#define PeasantID	0x17
#define PeasantIDS	"PEASANT"

#define HorseStrelokID	0x18
#define HorseStrelokIDS "HORSE-STRELOK"

#define FregatID	0x19
#define FregatIDS   "FREGAT"

#define GaleraID	0x1B
#define GaleraIDS   "GALERA"

#define IaxtaID	    0x1C
#define IaxtaIDS    "IAXTA"

#define ShebekaID	 0x1E
#define ShebekaIDS   "SHEBEKA"

#define ParomID      0x1F
#define ParomIDS     "PAROM"

#define ArcherID    0x20
#define ArcherIDS   "ARCHER"

#define MultiCannonID 0x21
#define MultiCannonIDS "MCANNON"

#define DiplomatID 0x22
#define DiplomatIDS "DIPLOMAT"

#define EgerID 0x24
#define EgerIDS "EGER"

//-----------------------------------------//

#define REG(x)   RegisterVar(&##x,sizeof x)
#define WOOD  0
#define GOLD  1
#define STONE 2
#define FOOD  3
#define IRON  4
#define COAL  5

#define LAND_AI     0
#define MEDITERR_AI 1
#define WATER_AI    2

#define LT_LAND				0
#define LT_MEDITERRANEAN	1
#define LT_PENINSULAS		2
#define LT_ISLANDS			3
#define LT_CONTINENTS		4
#define LT_CONTINENT		5


#define EASY		0
#define NORMAL		1
#define HARD		2
#define VERY_HARD	3

#define START_NORMAL  0
#define START_RICH    1
#define START_MILLION 2

#define RES_POOR   0
#define RES_NORMAL 1
#define RES_RICH   2

#define DEVELOPMENT	   0x1001
#define WATER_BATTLE   0x1002
#define LAND_BATTLE    0x1003
#define MINES_CAPTURE  0x1004
#define TOWN_DEFENCE   0x1005
#define MINES_UPGRADE  0x1006
#define FAST_DIVERSION 0x1007

//----- My function -----//

enum PlayerColor {	
	red=0,
	blue=1,
	turquoise=2,
	violet=3,
	orange=4,
	black=5,
	white=6,
	brown=7
};
//----------new----------
DLLIMPORT
void SetGroupNMASK(GAMEOBJ* Grp,byte NMASK);
DLLIMPORT
void EraseGroup(GAMEOBJ* Grp);
DLLIMPORT
byte GetNatNMASK(byte NI);
DLLIMPORT
void SetGroupStartNMASK(GAMEOBJ* Grp,byte Mask);
DLLIMPORT
void SetUnitInfoNew(OneUnit* Uni);
DLLIMPORT
int GetTSin(byte Idx);
DLLIMPORT
int GetTCos(byte Idx);
DLLIMPORT void OBJ_AttackObj(word Index,word Enemy,byte Prio,int NTimes);
DLLIMPORT
void OBJ_SendTo(word Index,int x,int y,byte Prio,byte OrdType);
DLLIMPORT
void OBJ_SmartSendTo(word Index,int x,int y,int dx,int dy,byte Prio,byte OrdType);
DLLIMPORT
bool GetZoneR(GAMEOBJ* Zone,int* R);
DLLIMPORT
char* GetMoneyString(int* res,char* tmp);
DLLIMPORT
int GetRND_X(int Max,int Line,const char* file);
#define GetRND(x) GetRND_X(x,__LINE__,__FILE__)
DLLIMPORT
bool CheckIfBuildingIsFired(word Index);

DLLIMPORT int RemovePanicUnits(GAMEOBJ* Src,GAMEOBJ* Dst);

DLLEXPORT bool GetGroupCharge(GAMEOBJ* Grp);
DLLEXPORT void FilterUnitsByCategory(GAMEOBJ* G_Src,GAMEOBJ* G_Dst,int Ctg);

DLLIMPORT void CreateNonFiredEnemyBuildingsTopList(word* IDS,byte NI);
DLLIMPORT void CreateTopListForStorm(word* IDS,byte NI, word StrmNIndex);
DLLIMPORT void CreateTopListForStormHard(word* IDS,byte NI,int NMax, word StrmNIndex);
DLLIMPORT void CreateEnemyBuildingsTopList(word* IDS,byte NI);

DLLIMPORT void AddUnitsInZoneToGroup(GAMEOBJ* Zone,GAMEOBJ* GROUP,byte NI,int Usage=0);
DLLIMPORT
int FindNextZoneOnTheSafeWayToObject(int Start,short* DangerMap,word* ObjIds,int* MaxDanger,int DangSteps,word* DestObj);
DLLIMPORT int FindNextZoneOnTheSafeWay(int Start,int Fin,short* DangerMap,int* MaxDanger,int DangSteps);
DLLIMPORT
int FindNextZoneOnTheSafeWayToLimitedObject(int Start,short* DangerMap,word* ObjIds,word* Type,word NeedType,int* MaxDanger,int DangSteps,word* DestObj);
DLLIMPORT
bool CheckIfNotBusy(GAMEOBJ* Grp);
DLLIMPORT
bool GetGrpCenter(GAMEOBJ* Grp,int* x,int* y);
DLLIMPORT
bool GetTopZoneCoor(int Zone,int* x,int* y);

DLLIMPORT void CreateDangerMap(byte NI,short* DangMap,int MaxSize,int* FearArray,int ItrAmount);
DLLIMPORT void CreateDangerMapForStorm(byte NI,short* DangMap,int MaxSize,int* FearArray,int ItrAmount);
DLLIMPORT void CreateDangerMapForStormHard(byte NI,short* DangMap,int MaxSize,int* FearArray,int ItrAmount);
DLLIMPORT void CreateDangerMapForFire(byte NI,short* DangMap,int MaxSize,int* FearArray,int ItrAmount);
CIMPORT void CreateDangerMapForHealer(byte NI,short* DangMap,int MaxSize,int ItrAmount);

DLLIMPORT
int GetNZones();
DLLIMPORT
int GetTopDist(int xa,int ya,int xb,int yb);
DLLIMPORT
bool SGP_AttackByArchers(byte NI,GAMEOBJ* GRP,int R,DWORD Flags);

DLLIMPORT int SGP_MakeOneStepTo(byte NI,GAMEOBJ* Grp,int x,int y,int Direction,DWORD Flags);
DLLIMPORT int SGP_ExMakeOneStepTo(byte NI,GAMEOBJ* Grp,int x,int y,int DF,int DR,int Direction,DWORD Flags);
DLLIMPORT void SGP_MoveToPoint(byte NI,GAMEOBJ* Grp,int x,int y,int Direction,int DF,int DR,DWORD Flags);

DLLIMPORT
void SGP_RepairBuilding(GAMEOBJ* Grp,byte Type,int Index);
DLLIMPORT
void SGP_ComeIntoBuilding(GAMEOBJ* Grp,byte Type,int Index);
DLLIMPORT
void SGP_LeaveBuilding(GAMEOBJ* Grp);

DLLIMPORT void SGP_TakeResources(GAMEOBJ* Grp,byte Type,byte NI,int ResType);
DLLIMPORT void SGP_TakeResourcesZone(GAMEOBJ* Grp,byte Type,byte NI,int ResType,GAMEOBJ* Zone);

DLLIMPORT void CancelCurrentPurpose(GAMEOBJ* GRP);
DLLIMPORT void CancelAllPurposes(GAMEOBJ* GRP);

DLLIMPORT void CopyUnits(GAMEOBJ* Src,GAMEOBJ* Dest,int start,int n,bool add,bool remove);
DLLIMPORT void RemoveAttackingUnits(GAMEOBJ* Grp);

DLLIMPORT void GetTargetObject(word AttackID, word* Index, word* Serial);

DLLIMPORT int SGP_ExMakeOneStepTo(byte NI,GAMEOBJ* Grp,int x,int y,int DF,int DR,int Direction,DWORD Flags);

DLLIMPORT int GetTopZone(int x,int y);
DLLIMPORT bool GrpAttackObject(byte NI,GAMEOBJ* Grp,int TargetID);
DLLIMPORT int GetTopZone(int x,int y);
DLLIMPORT bool GetTopZRealCoor(int Zone,int* x,int* y);
DLLIMPORT int GetListOfNearZones(int Zone,OneLinkInfo** ZonesAndDist);
DLLIMPORT int GetNextZone(int Start,int Fin);
DLLIMPORT int GetZonesDist(int Start,int Fin);
DLLIMPORT bool CheckIfPointsNear(int xa,int ya,int xb,int yb);

DLLIMPORT void CreateDangerMapWithoutPeasants(byte NI,short* DangMap,int MaxSize,int* FearArray,int ItrAmount);
DLLIMPORT void CreateDangerMapForTom(byte NI,short* DangMap,int MaxSize,int* FearArray,int ItrAmount);

DLLIMPORT bool AttackEnemyPeasantsInZone(GAMEOBJ* Grp,GAMEOBJ* Zone,byte EnmNation);

DLLIMPORT bool CreatePeasantsTopList(word* IDS,byte NI,int MinPS, bool NotSeakMine=false);
DLLIMPORT void CreateTopListEnArmy(word* IDS,byte NI,int MinPS);

DLLIMPORT void SetUnitsState(GAMEOBJ* GRP,bool SearchEnemy,bool StandGround,bool ArmAttack,bool FriendlyFire);

#include "AddScenario.h"

DLLIMPORT int GetEnemyForce(GAMEOBJ* Zone,byte NI);
DLLIMPORT int GetAlliesForce(GAMEOBJ* Zone,byte NI);
DLLIMPORT void CreateFriendBuildingsTopList(word* IDS,byte NI);
DLLIMPORT int GetGroupForce(GAMEOBJ* Grp);
DLLIMPORT bool Attack3(GAMEOBJ* Grp,GAMEOBJ* Zone,byte NI, int& NArm);
DLLIMPORT bool AttackByTomahawks(GAMEOBJ* Grp,GAMEOBJ* Zone,byte NI, int& NArm);
DLLIMPORT bool Attack6(GAMEOBJ* Grp,GAMEOBJ* Zone,byte NI, int& NArm);
DLLIMPORT bool AttackEnemyInZone2(GAMEOBJ* Grp,GAMEOBJ* Zone,byte NI);
DLLIMPORT word MakeOneShotToBuild(GAMEOBJ* Zone,GAMEOBJ* Grp,byte NI/*, bool SeakInFire*/);

DLLIMPORT bool ArtAttackGroud(byte NI,GAMEOBJ* Grp, int x, int y);
DLLIMPORT bool ArtAttackObject(byte NI,GAMEOBJ* Grp,int TargetID);

DLLIMPORT int GetLastFullWay(word** buf);

DLLIMPORT void SetDangerMap(int* Map);
DLLIMPORT void DieUnit(word MID);

DLLIMPORT void SetAI_Guard(GAMEOBJ* Grp,bool val);

CIMPORT void AssignHint(char* s,int time);
CIMPORT void AssignHint1(char* s,int time);

DLLIMPORT void SetToInternalResourcesZero(GAMEOBJ* Grp);

DLLIMPORT bool RemoveNearestUnit(GAMEOBJ* G_Src,GAMEOBJ* G_Dst,int x,int y,int MaxR);
DLLIMPORT bool GetBuildingEntryPoint(int* x,int* y,word Index,int AddDistance);

DLLIMPORT bool GetBuildingOposit(word Index, int& N);

#pragma pack(1)
class GeneralObject;
class Nation;
class Visuals;
class ObjectUpgrade;
class ParameterUpgrade;
class NewMonster;
class OneObjectEx;
class SelGroup;
class FireInfo;
class NewAnimation;
class UnitActiveAbilityList;
class Order1;
class GOrder;
class Coor3D;
class Brigade;
class AdvCharacter;
#include "..\Cossacks2\OneObject.h"

CIMPORT int GetMaxObject();

DLLIMPORT OneObject* GetOBJ(int Index);

DLLIMPORT bool isUnitAbsorber(word Index);
DLLIMPORT bool CheckLandLocking(int x,int y,int Lx,int Ly);

DLLIMPORT void SetUnitsCollector(byte NI,GAMEOBJ* Grp);
DLLIMPORT int InsertUnitToGroup(GAMEOBJ* Src,GAMEOBJ* Dst,int Index);

CIMPORT void BrigReformation(byte NI,word BrigadeID,byte FormType);
CIMPORT void GetEnemyTopInfo(byte NI, int* N, int *xc, int *yc);

CIMPORT void CreateAGroup(byte NI,char* gName);
CIMPORT void CreateAZone(char* Name,int x,int y,int R);
CIMPORT void ClearAZones();


CIMPORT void CreateArmyInfo(byte NI, int* Life, int* Damage, int MaxSize);
CIMPORT long int GetGroupPower(GAMEOBJ* Grp);

CIMPORT void SetMapOfShooters(byte NI, int* Vistrel, int* Kartech);

DLLIMPORT int GetUnitIcon(int Type);

DLLIMPORT bool CreateFormation(byte NI,GAMEOBJ* Officers,GAMEOBJ* Drummers,GAMEOBJ* FlagBearers,GAMEOBJ* Soldiers,GAMEOBJ* DestGroup,char* FormID);

CIMPORT bool DetPeasant(word NIndex);

typedef bool SubstInterfFunc(word IDX);
extern SubstInterfFunc* CheckInterface;

DLLIMPORT void SetInterfaceCallback(SubstInterfFunc* F);
CIMPORT void ActivateCMLInterface(int Idx,char* request);
CIMPORT void DrawCMLInterface(int Idx);

CIMPORT bool DetAbsorbeAbility(word BuildNIndex, word UnitNIndex);

CEXPORT void AddTomahawks(GAMEOBJ* Src,byte Owner,word Base,int DriftRadius, int CriticalMass);
CEXPORT void AddStorm(GAMEOBJ* Src, byte Owner, int Diff);

CIMPORT bool DetArcher(word NIndex);

CIMPORT void EraseBrig(byte NI,word BrigadeID);

DLLIMPORT int RandNew(char*,int);
DLLIMPORT void AddRandNew(char*,int,int);
DLLIMPORT void AddUN(char* File,int Line,int Param,int Type);
#define addrand(x) AddRandNew(__FILE__,__LINE__,x)

CIMPORT char* GetResourceName(byte ResID);

CIMPORT void RunPF(int i,const char* Desc);
CIMPORT void StopPF(int i);

CIMPORT bool ProduceUnitTicks(GAMEOBJ* Units,GAMEOBJ* UnitType,GAMEOBJ* DestGroup,int Ticks);
CIMPORT bool ProduceUnitAtLeast(GAMEOBJ* Units,GAMEOBJ* UnitType,GAMEOBJ* DestGroup,int CollectAmount,int Ticks);
CIMPORT void GetCostStringExp(word* Cost,char* str);
CIMPORT void GetCostStringExp(int* Cost,char* str);

DLLIMPORT int GetOrderType(OneObject* OB);
DLLIMPORT void DeleteUnit(int Index);

DLLIMPORT bool OBJ_LeaveMine(word Index,word NIndex);
DLLIMPORT bool OBJ_GoToMine(word Index,word DestMine,byte prio,byte OrdType);
DLLIMPORT void OBJ_ChangeNation(OneObject* OB, byte DstNat);
DLLIMPORT void OBJ_PreciseSendTo(word Index,int x,int y,byte Prio,byte OrdType);
DLLIMPORT void OBJ_Rotate(word Index,byte Dir,byte OrdType);
CIMPORT bool GetBrDir(byte Owner, int BrigID, byte& Dir);
CIMPORT bool GetBrAddDS(byte Owner, int BrigID, char& AddDamage, char& AddShield);
DLLIMPORT void SetOBRifleAttack(OneObject* OB, word RifleAttack);
DLLIMPORT int OBJ_GetProducingUnits(word Index,word NIndex);

CIMPORT int GetFreePlacesInBrig(byte Owner, int BrigID, int MaxPlaces, int* posx, int* posy, int* pos, int& CenterX, int& CenterY);
CIMPORT bool GetBrGroup(byte Owner, int BrigID, int* N, word** ID, word** SN);

CIMPORT word FindNearestSklad(byte NI, byte ResID, int x, int y);
CIMPORT word FindNearestSklad(byte NI, byte ResID);

typedef tpDrawOnMiniMapCallback(int x,int y,int Lx,int Ly,int mx0,int my0);
CIMPORT tpDrawOnMiniMapCallback* SetDrawOnMiniMapCallback(tpDrawOnMiniMapCallback* dc);

typedef tpDrawOnMapCallback();
CIMPORT tpDrawOnMapCallback* SetDrawOnMapCallback(tpDrawOnMapCallback* dc);

CIMPORT void SetDrawScale(float scale);
CIMPORT void DrawPicture(int x,int y,int File,int Sprite,byte Nat);
CIMPORT int GetPictureIndex(char* name);
CIMPORT void SetCurrentDiffuse(DWORD Color);

CIMPORT DWORD GetNatColor(int i);

DLLIMPORT bool GetFogSpotVisibility(int x,int y);

typedef tpBirthCallback(int NewbornIndex);
CIMPORT tpBirthCallback* SetBirthCallback(tpBirthCallback* dc);
DLLIMPORT void DeleteDynGroup(GAMEOBJ* Units);
DLLIMPORT void AddOneUnitToGroup(GAMEOBJ* Units,OneObject* OB);
CIMPORT word getDir(int dx,int dy);
CIMPORT void RotUnit(int Index,char Dir,byte OrdType);

CIMPORT byte GetTribeOwner(GAMEOBJ* Tribe);	
CIMPORT bool GetPeaceMode(); // пис мод в редакторе, юниты не воюют
CIMPORT bool CheckNoCityLife();
CIMPORT bool isBrigGlobalInterfaceCanMakeBrigade(); // достаточно ли юнитов возле бараков для постройки отряда с помощью GlobalBrigInterface

DLLIMPORT int GetMyRefNation();

CIMPORT word CreateFormationFromGroup(byte BrigNI, GAMEOBJ* Source, int x, int y, byte dir);

#include "..\Cossacks2\Polk.h"
CIMPORT Polk* GetPolkByIndex(int PIndex);
CIMPORT int CreatePolk();

#include "..\Cossacks2\ZonesAndGroupsExtraInfo.h"

CIMPORT char* GetMonsterMD(int NIndex);
CIMPORT char* GetMonsterID(int NIndex);

CIMPORT void PlayerStatistic_AddTotalRes(byte NI, byte ResID, int Amount);

DIALOGS_API void PlaySound(char* Name,int x, int y);
DIALOGS_API void PlaySound(char* Name);

DLLIMPORT int GetAnimTime();

CIMPORT void DeleteOrders(GAMEOBJ* Grp);
DLLIMPORT void OBJ_SetNotSelectable(word Index, bool Value);
DLLIMPORT bool FillFormationFromGroup(byte BrigNI, word BrigID, GAMEOBJ* Source, GAMEOBJ* Dest=NULL);

#endif
