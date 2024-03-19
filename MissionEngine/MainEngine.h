#define	   __MISSION_DLL_ANABLED__
//#define	   MIS_TEST_LVA		//Только для тестирования.

#include "StdAfx.h"

#define free _ExFree
#define malloc _ExMalloc
#define realloc _ExRealloc

#define DLLEXPORT extern "C" __declspec(dllexport)
#define DLLIMPORT extern "C" __declspec(dllimport)

#define CEXPORT __declspec(dllexport)
#define CIMPORT __declspec(dllimport)

//~~~~~~~~~~~~~~~~~MEMORY FUNCTIONS OVERRIDES~~~~~~~~~~~~~~~//
CIMPORT void* _ExMalloc(int Size);
CIMPORT void _ExFree(void* Ptr);
CIMPORT void* _ExRealloc(void* ptr,int Size);
inline void* _cdecl operator new(size_t size)
{
	return malloc(size);
};
inline void* _cdecl operator new[](size_t size)
{
	return malloc(size);
};

inline void __cdecl operator delete(void *ptr)
{
	free(ptr);
};
inline void __cdecl operator delete[](void *ptr)
{
	free(ptr);
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//  

//#pragma comment(linker,"/DEFAULTLIB:..\\lib\\Cossacks2.lib")	#pragma comment(linker,"/DEFAULTLIB:..\\lib\\DipServer.lib")
//#pragma comment(linker,"/DEFAULTLIB:q:\\Cossacks2.lib")		#pragma comment(linker,"/DEFAULTLIB:q:\\DipServer.lib")

#pragma pack(1)

DLLEXPORT	void OnInit();
typedef unsigned short word;
typedef unsigned char byte;
extern	struct GAMEOBJ;
extern	byte	Real_Attacker_NI, Real_Defender_NI,   Real_AI_NI, Real_PLAYER_NI;
extern	int		TRAPS_SQ_LIMIT;
extern int		GetAttackerSQ_NUM;
extern int		GetDefenderSQ_NUM;

extern	char* missID[50];
extern	char* objID[50];

// MAIN DEFINES FOR Mission Engine
#define		_CZM_ARS_LV			102			//FOR MISSION AND STRATEGY AI	( VITYA ). ATTACKER CREATE SIDE.
#define		_CZM_DRS_LV			103			//FOR MISSION AND STRATEGY AI	( VITYA ). DEFENDER CREATE SIDE.
#define		AI_IMPORTANT_POINT	500

#define		MinSQMemb			10
#define		CaptureZoneNum		10
#define		CaptureZoneColor0	0x7FFFFFFF  // White		//0xeeF3EFEF
#define		CaptureZoneColor1	0x7F00FF00	// Green		//0xFF2ead34 //0xFF004C00		//0xFFD4C19C // Yellow		

//-------------Registration commands--------------
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
bool SafeRegisterUpgrade(GAMEOBJ* GOBJ,char* Name);
DLLIMPORT
void InitialUpgrade(char* Grp,char* Upgrade);
DLLIMPORT
void InitialUpgradeNew(byte NI,GAMEOBJ* Upg,word Index); //если Index==0xFFFF то для всей нации
DLLIMPORT
void DisableUpgrade(byte Nat,GAMEOBJ* Upg);
DLLIMPORT
void EnableUpgrade(byte Nat,GAMEOBJ* Upg);
DLLIMPORT
void EnableUnit(byte Nat,GAMEOBJ* Type,bool State);
DLLIMPORT
bool RegisterFormation(GAMEOBJ* GOBJ,char* Name);
DLLIMPORT
bool RegisterUnitsForm(GAMEOBJ* GOBJ,char* Name);
//---------------Checking commands----------------
//1.In zones
DLLIMPORT
int GetUnitsAmount0(GAMEOBJ* Zone,byte Nation);
//1.In zones
DLLIMPORT
void ClearSelection(byte Nat);
DLLIMPORT
void ShowAlarm(GAMEOBJ* Zone);
DLLIMPORT
void SelectUnitsInZone(GAMEOBJ* Zone,byte Nation,bool add);
DLLIMPORT
void ChangeUnitParam(GAMEOBJ* Type,byte NI,int AddDamage,int DamType,int AddShield);
DLLIMPORT
void SelectTypeOfUnitsInZone(GAMEOBJ* Zone,GAMEOBJ* Type,byte Nation,bool add);
DLLIMPORT
int GetUnitsAmount1(GAMEOBJ* Zone,GAMEOBJ* Units);
DLLIMPORT
int GetUnitsAmount2(GAMEOBJ* Zone,GAMEOBJ* UnitType,byte Nation);
//2.total
DLLIMPORT
int GetTotalAmount0(GAMEOBJ* Units);
DLLIMPORT
int GetBrigadsAmount0(byte NI);
DLLIMPORT
int GetBrigadsAmount(GAMEOBJ* pGrp);
DLLIMPORT
int GetTotalAmount1(GAMEOBJ* UnitType,byte Nation);
DLLIMPORT
int GetTotalAmount2(GAMEOBJ* Units,GAMEOBJ* UnitType,byte Nation);
DLLIMPORT
int GetReadyAmount(GAMEOBJ* UnitType,byte Nation);
DLLIMPORT
int GetAmountOfWarriors(byte Nat);
//3.Upgrades
DLLIMPORT
bool IsUpgradeDoing(GAMEOBJ* Upgrade,byte Nation);
DLLIMPORT
bool IsUpgradeDone(GAMEOBJ* Upgrade,byte Nation);
DLLIMPORT
bool IsUpgradeEnabled(GAMEOBJ* Upgrade,byte Nation);
//4.Died units
DLLIMPORT
int GetDied(GAMEOBJ* UnitType,byte Nation);
//---------------Creating/Erasure commands--------------
DLLIMPORT
bool CreateObject0(GAMEOBJ* DstObj,GAMEOBJ* Form,GAMEOBJ* UnitType,byte NatID,GAMEOBJ* Zone,byte Direction);
//---------------------Action commands----------------------
DLLIMPORT
void ClearSelection(byte Nat);
DLLIMPORT
void SelectUnits(GAMEOBJ* Units,bool Add);
DLLIMPORT
void SelectUnits1(byte Nat,GAMEOBJ* Units,bool Add);
DLLIMPORT
void SelectUnitsType(GAMEOBJ* UnitsType,byte Nat,bool Add);
DLLIMPORT
bool SelDie(byte Nat);
DLLIMPORT
void DeleteUnit(int Index);
DLLIMPORT
void DoMessagesBrief();
DLLIMPORT
void SelErase(byte NI);
DLLIMPORT
void SelAttackGroup(byte Nat,GAMEOBJ* Enemy);
DLLIMPORT 
bool SelOpenGates(byte Nat);
DLLIMPORT
int GetNInside(byte Nat);
DLLIMPORT
int GetMaxInside(byte Nat);
DLLIMPORT
void PushUnitAway(byte Nat);
DLLIMPORT
void PushAllUnitsAway(byte Nat);
DLLIMPORT
void SendUnitsToTransport(byte Nat);
DLLIMPORT
bool CheckLeaveAbility(byte Nat);
DLLIMPORT
void AttackZoneByArtillery(GAMEOBJ* ArtGroup,GAMEOBJ* Zone,byte Nat);
DLLIMPORT
void AttackBuildingsInZone(GAMEOBJ* ArtGroup,GAMEOBJ* Zone,byte Nat);
DLLIMPORT
void ProduceOneUnit(byte Nat,GAMEOBJ* UnitType);
DLLIMPORT 
bool SelCloseGates(byte Nat);
DLLIMPORT 
bool SelSendTo(byte Nat,GAMEOBJ* Zone,byte Dir,byte Type);
DLLIMPORT 
bool SelSendAndKill(byte Nat,GAMEOBJ* Zone,byte Dir,byte Type);
DLLIMPORT 
bool Patrol(byte Nat,GAMEOBJ* Zone,byte Dir);
DLLIMPORT
void ChangeFriends(byte Nat,byte Flags);
DLLIMPORT
void	RunTactikalAI(byte NI);
CIMPORT
void ActivateTacticalAI(byte NI);
CIMPORT
void StopTacticalAI(byte NI);
DLLIMPORT
void	SetPlayerNation(int NI);
DLLIMPORT
int		GetPlayerNation();
DLLIMPORT
void SelChangeNation(byte SrcNat,byte DstNat);
DLLIMPORT
void SelAutoKill(byte Nat);
DLLIMPORT
void SetStandGround(byte Nat,byte val);
DLLIMPORT
void AllowAttack(byte Nat,byte val);
//-----------------------INFORMATION COMMANDS----------------------------//
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
bool AskQuestionParam(char* Name,...);
DLLIMPORT
int AskComplexQuestion(int Nx,char* Name1,byte or1,char* Name2,byte or2,char* Quest);
//-----------------------------Resource functions------------------------//
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
//----------------------------TRIGGER FUNCTION---------------------------//
DLLIMPORT
byte Trigg(byte ID);
DLLIMPORT
void SetTrigg(byte ID,byte Val);
DLLIMPORT
word WTrigg(byte ID);
DLLIMPORT
void SetWTrigg(byte ID,word Val);
//------------------------------AI Functions-----------------------------//
DLLIMPORT
void RunAI(byte Nat);
DLLIMPORT 
void RunAIWithPeasants(byte Nat,char* P_Name);
//-----------------------------Timer commands----------------------------//
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
//---------------------------Dynamical zones-----------------------------//
DLLIMPORT
bool UnitsCenter(GAMEOBJ* DstZone,GAMEOBJ* Units,word R);
DLLIMPORT
void RefreshScreen();
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
void SetStartPoint(GAMEOBJ* Zone);
DLLIMPORT 
void AttackEnemyInZone(GAMEOBJ* Grp,GAMEOBJ* Zone,byte EnmNation);
DLLIMPORT
int GetMyNation();
DLLIMPORT
int GetTopDst(GAMEOBJ* Z1,GAMEOBJ* Z2);
DLLIMPORT
void StartAI(byte Nat,char* Name,int Land,int Money,int ResOnMap,int Difficulty);
DLLIMPORT
void DoNotUseSelInAI(byte Nat);
DLLIMPORT
void SetTutorial(bool State);
DLLIMPORT
bool GetQuestPressed();
DLLIMPORT
void SetAIProperty(byte NAT,int Prop,int Val);
DLLIMPORT
int AskMultilineQuestion(int Nx,char* Name1,byte or1,char* Quest);
DLLIMPORT
void SetReadyState(GAMEOBJ* Units,bool State);
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
void SaveSelectedUnits(byte NI,GAMEOBJ* Units,bool add);
DLLIMPORT
bool CheckProduction(GAMEOBJ* Units);
DLLIMPORT
bool CreateBuilding(byte Nat,GAMEOBJ* Zone,GAMEOBJ* UnitType,GAMEOBJ* DestGroup);
DLLIMPORT
void SetDestPoint(GAMEOBJ* Units,GAMEOBJ* Zone);
DLLIMPORT
void RegisterDynGroup(GAMEOBJ* Units);
DLLIMPORT
void DeleteDynGroup(GAMEOBJ* Units);
DLLIMPORT
int GetNUnits(GAMEOBJ* Units);
//	------- OneUnit STRUCTURE -----
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
//	------- 
DLLIMPORT
bool GetUnitInfo(GAMEOBJ* Units,int Index,OneUnit* Uni);
DLLIMPORT
bool GetUnitGlobalInfo(int Index,OneUnit* Uni);
DLLIMPORT
void SetUnitInfo(OneUnit* Uni);
DLLEXPORT
void SetUnitInfoNew(OneUnit* Uni);
DLLIMPORT
word GetUnitBrigadeID(int Index); //grey[20.01.2005]
DLLIMPORT
void PanicUnitByIndex(int Index); //grey[15.02.2005]	(Uni->Index) //very new
DLLIMPORT
void RemoveGroup(GAMEOBJ* Source,GAMEOBJ* Dest);
//-------------------------AI Low level functions------------------------//
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
void AssignMine(char* Name);
DLLIMPORT
void AssignPeasant(char* Name);
DLLIMPORT
void AssignHouse(char* Name);
DLLIMPORT
void AssignWall(char* Name);
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
int GetUnitsByUsage(byte Nat,byte Usage);
DLLIMPORT
int GetTorgResult(byte SellRes,byte BuyRes,int SellAmount);
DLLIMPORT
int GetReadyUnits(GAMEOBJ* UnitType);
DLLIMPORT
void SetUpgradeLock(byte Res,byte Val);
DLLIMPORT
void SetDefSettings(int p1,int p2);
DLLIMPORT
int GetMaxPeasantsInMines();
DLLIMPORT
bool UpgIsDone(GAMEOBJ* Upgrade);
DLLIMPORT
bool UpgIsRun(GAMEOBJ* Upgrade);
DLLIMPORT
void SetMinesBuildingRules(int* Table,int NElm);
DLLIMPORT
void SetMinesUpgradeRules(int* Table);
DLLIMPORT
bool FieldExist();
DLLIMPORT
int GetDifficulty();
//---------------------------------------------------------------------
DLLIMPORT
int GetStartRes();
DLLIMPORT
int GetDiff(byte NI);
DLLIMPORT
int GetResOnMap();
DLLIMPORT
int GetLandType();
DLLIMPORT
void SetStandartVictory();
DLLIMPORT
bool NationIsErased(byte Nat);
DLLIMPORT
void AssignFormUnit(char* Name);
DLLIMPORT
void SetPlayerName(byte Nat,char* ID);
DLLIMPORT
int GetMaxPeaceTime();
DLLIMPORT
int GetPeaceTimeLeft();
DLLIMPORT
int GetRandomIndex();
DLLIMPORT
void SetAIRegister(int Reg,int Val);
DLLIMPORT
int GetAIRegister(int Reg);
DLLIMPORT
int GetExtraction(byte ResID);
DLLIMPORT
void SetDefenseState(byte State);
//-----------------------------------------------------------------------/
DLLIMPORT
void MissErrorMessage(char* Header,char* Message);
DLLIMPORT
void RunMapeditor(char* path);
DLLIMPORT
void RunUserMission(char* path);
//------------------------NEW! FUNCTIONS----------------------//
struct UnitExCaps{
	int Direction;
	int DestX;
	int DestY;
	int Type;
	char* UnitID;
	int NI;
	int StandTime;
	int OrderType;
	};
DLLIMPORT
byte GetUnitNMASK(int Index);
DLLIMPORT
void SetNMASK(int Index,byte NMASK);
DLLIMPORT
int GetUnitIcon(int Type);
DLLIMPORT
int GetAmountOfGroups();
DLLIMPORT
int GetAmountOfZones();
DLLIMPORT
char* GetZoneName(int Index);
DLLIMPORT
char* GetGroupName(int Index);
DLLIMPORT
char* GetUnitName(int Type);
DLLIMPORT
char* GetUnitID(int Type);
DLLIMPORT 
bool GetUnitExCaps(int Index,UnitExCaps* CAPS,bool NeedOrderType);
DLLIMPORT
int InsertUnitToGroup(GAMEOBJ* Src,GAMEOBJ* Dst,int Index);
DLLIMPORT
void RemoveUnitFromGroup(GAMEOBJ* Src,int Index);
DLLIMPORT
bool GetZoneCoor(GAMEOBJ* Zone,int* x,int* y);
DLLIMPORT
bool GetZoneR(GAMEOBJ* Zone,int* R);
DLLIMPORT
void PastePiece(GAMEOBJ* Zone,char* Name);
DLLIMPORT
void PastePiece(GAMEOBJ* Zone,char* Name);
DLLIMPORT
void SelectBuildingsInZone(byte NI,GAMEOBJ* Zone,GAMEOBJ* UTP,bool Add);
DLLIMPORT
void InviteUnitIntoBuilding(int BIndex,int UIndex,byte Type);
DLLIMPORT
void LeaveBuilding(int BIndex,int Type);
DLLIMPORT
int GetInsideList(int BIndex,word** List);
DLLIMPORT
void ChangeUnitColor(int Index,byte DstNI);
DLLIMPORT
void AddUnitToSel(int Index);
DLLIMPORT
void AddUnitsInZoneToGroup(GAMEOBJ* Zone,GAMEOBJ* GROUP,byte NI,int Usage);
DLLIMPORT
void DeleteUnitFromGroup(GAMEOBJ* Grp, int Index);
DLLIMPORT
void CleanGroup(GAMEOBJ* Grp);
DLLIMPORT
int RemoveNUnits(GAMEOBJ* Dst,GAMEOBJ* Src,int N,int NIndex);
DLLIMPORT
int CollectUnits(GAMEOBJ* Dst,GAMEOBJ* Src,int N,int NIndex);
inline int  GetTotalAmount(GAMEOBJ* Units){
	return GetTotalAmount0(Units);
	};
inline int  GetTotalAmount(GAMEOBJ* UnitType,byte Nation){
	return GetTotalAmount1(UnitType,Nation);
	};

DLLIMPORT
int GetInsideAmount(int Index);			//Кол-во юнитов в здании
DLLIMPORT
int GetMaxInsideAmount(int Index);		//Макс кол-во посадочных мест в здании
DLLIMPORT
void CreateFields(byte NI);				//Засеить поля для нации NI

//*************************** FOR ATTACK VS ENEMY *************************************
DLLIMPORT
void SGP_ComeIntoBuilding(GAMEOBJ* Grp,byte Type,int Index);
DLLIMPORT
void SGP_LeaveBuilding(GAMEOBJ* Grp);
DLLIMPORT
void SetEnemyBuildingsCollector(byte NI,GAMEOBJ* Grp);
DLLIMPORT
void SetUnitsCollector(byte NI,GAMEOBJ* Grp);

#define AFL_COOR     1
#define AFL_NUNITS   2
#define AFL_FORCE    4
#define AFL_MINIMAX  8

DLLIMPORT
int EnumEnemyArmies(byte NI,DWORD Fields,DWORD* Data,int BufSize);

#define MSO_ALLOW_ENEMY_SEARCH 1
#define MSO_CANCEL_WHEN_ATTACKED 16

struct GMovementData{
	int DestX;
	int DestY;
	int DestTop;
	int MovementStartTime;
	int FirstUnitReachedTime;
	int UInfo[32];
	};

struct GCoorMovement{
	int StartTime;
	int FirstTime;
	DWORD Flags;
	};

DLLIMPORT
bool GetGrpCenter(GAMEOBJ* Grp,int* x,int* y);
DLLIMPORT
int SGP_MakeOneStepTo(byte NI,GAMEOBJ* Grp,int x,int y,int Direction,DWORD Flags);
DLLIMPORT
DWORD GetCurrentPurpose(GAMEOBJ* GRP);
DLLIMPORT
void CancelCurrentPurpose(GAMEOBJ* GRP);
DLLIMPORT
void CancelAllPurposes(GAMEOBJ* GRP);
DLLIMPORT
void SetUnitsState(GAMEOBJ* GRP,bool SearchEnemy,bool StandGround,bool ArmAttack,bool FriendlyFire);
DLLIMPORT
bool SGP_AttackByArchers(byte NI,GAMEOBJ* GRP,int R,DWORD Flags);
DLLIMPORT
int  GetCurrentUnits(byte NI);
DLLIMPORT
bool DetectPanic(int Index);
DLLIMPORT
int RemovePanicUnits(GAMEOBJ* Src,GAMEOBJ* Dst);

//FOR CREATE WAY
DLLIMPORT
int SGP_MakeOneStepTo(byte NI,GAMEOBJ* Grp,int x,int y,int Direction,DWORD Flags);

DLLIMPORT
int GetTopZone(int x,int y);
DLLIMPORT
bool GetTopZoneCoor(int Zone,int* x,int* y);
DLLIMPORT
int GetListOfNearZones(int Zone,word** ZonesAndDist);
DLLIMPORT
int GetNextZone(int Start,int Fin);
DLLIMPORT
int GetZonesDist(int Start,int Fin);
DLLIMPORT
int GetTopDist(int xa,int ya,int xb,int yb);
DLLIMPORT
int GetNZones();
DLLIMPORT
bool CheckIfPointsNear(int xa,int ya,int xb,int yb);
DLLIMPORT
int FindNextZoneOnTheSafeWay(int Start,int Fin,short* DangerMap,int* MaxDanger,int DangSteps);
DLLIMPORT
int GetLastFullWay(word** buf);
DLLIMPORT
void CreateDangerMap(byte NI,short* DangMap,int MaxSize,int* FearArray,int ItrAmount);

//Статус возможности постройки юнита и абгрейдов.
DLLIMPORT
void SetUpgradeEnableStatus(byte Nat,GAMEOBJ* Upg,int state);
DLLIMPORT
void SetUnitEnableState(byte Nat,GAMEOBJ* Type,int state);

//DLLIMPORT
//int GetOrderType(OneObject* OB);

// DipServer import
CIMPORT void AddStorm(GAMEOBJ* Src, byte Owner, int Diff=0);
CIMPORT void AddFirers(GAMEOBJ* Src,byte Owner);
CIMPORT void AddPsKillers(GAMEOBJ* Src,byte Owner,bool SeakMine=true);
CIMPORT void AddTomahawks(GAMEOBJ* Src,byte Owner,word Base,int DriftRadius, int CriticalMass);
CIMPORT void AddCannon(GAMEOBJ* Src, byte Owner);

//0-archer
//1-short range unit
//2-shooter
//3-peasant
//4-hidden
//5-tamagavshik & kopiemet
DLLIMPORT
void FilterUnitsByCategory(GAMEOBJ* G_Src,GAMEOBJ* G_Dst,int Ctg);

extern "C" CIMPORT
void SGP_RepairBuilding(GAMEOBJ* Grp,byte Type,int Index);

//***********************************************************************************
CIMPORT	char* GetTextByID(char* ID);
CIMPORT void ActivateDipDialog(char* request);
CIMPORT void ChangeDipDialog(char* request);

CIMPORT void AssignHint1(char* s,int time);
//***********************************************************************************

//***********************************************************************************
//Squadron Commands
CIMPORT
void BrigReformation(byte NI,word BrigadeID,byte FormType);
///////////////////

class Brigade_LV
{
public:
	word	ID;
	word	SN;
	word	NMemeb;
	word*	Memb;
	word*	MembSN;
	bool	Enabled;
};
DLLIMPORT
void	GetBrigadeLV(byte NI,word BirgID,Brigade_LV* pBR);
DLLIMPORT
bool	GetBrigadeEnable(byte NI,word BrigID);
DLLIMPORT
word	GetBrigadeSN(byte NI,word BrigID);
DLLIMPORT
bool	SetBrigadeAutoFormID(byte NI,word BrigID, word cond);
DLLIMPORT
word	GetCITYNBrigs(byte NI);//grey  [16/12/2004]
DLLIMPORT
bool	SetBrigadeNewMorale(byte NI,word BrigID,word NewMorale,int kills=-1);//grey
DLLIMPORT
bool	SetBrigadeNoMorale(byte NI,word BrigID,bool state); //grey  [14/07/2004]
CIMPORT
//Brigade* GetBrigade(byte NI, word BrigID);
DLLIMPORT
int	GetNUnitsInBrigade(byte NI,int BrigID);
CIMPORT
bool CreateBrigade(GAMEOBJ* DstGrp, GAMEOBJ* OficerType, GAMEOBJ* SoldierType, byte FormID,
				   byte AmountID, byte NI, GAMEOBJ* Zone, byte Direction);
CIMPORT
int GetFormationIndexBySizeIndex(byte NI, word UnitType, byte SizeIndex);
CIMPORT
int PutNewFormationBySizeIndex(byte NI, word UnitType, byte SizeIndex, int x, int y, byte Dir=0);
CIMPORT
int PutNewFormationBySizeIndex(byte NI,char* UnitType, byte SizeIndex, int x, int y, byte Dir=0);
CIMPORT
int PutNewFormationBySizeIndex2(byte NI,char* UnitType, byte SizeIndex, int x, int y, byte Dir=0);
CIMPORT
bool	SetCOMPSTART( int nation );	//grey [06.02.2004]
CIMPORT
void	StartAIEx(byte Nat,char* Name,int Land,int Money,int ResOnMap,int Difficulty);	//grey [10.02.2004]

CIMPORT	//grey //  [11/08/2004]
int CreateUnitByTypeAt(byte NI, word UnitType, int x, int y);

//***********************************************************************************

#define DIALOGS_API __declspec(dllimport)

class DIALOGS_API Polk
	{
	public:
		Polk();
		virtual ~Polk();
		void virtual MovePolkTo(int x, int y, int Dir=0);//
		int  virtual GetEnemyForce();
		int  virtual GetPolkIndex();
		bool virtual ReSetFormation(int FormIndex);
		void virtual AddSelectedToPolk(byte Nat);
		void virtual AddBrigadeToPolk(word Nat, int BrIndex);//
		int  virtual GetFormationIndex(int UnitType);
	};

CIMPORT int CreatePolk();//1
CIMPORT Polk* GetPolkByIndex(int PIndex);//


//***********************************************************************************
//typedef	void	(*pCALL_FUN)(int,int,int,int,int,int);
CIMPORT
void LineRGB(int x,int y,int x1,int y1,DWORD Color); // RGB(r,g,b)+0xFF000000

typedef tpDrawOnMiniMapCallback(int x,int y,int Lx,int Ly,int mx0,int my0);
CIMPORT 
tpDrawOnMiniMapCallback* SetDrawOnMiniMapCallback(tpDrawOnMiniMapCallback* dc);

typedef tpDrawOnMapCallback();
CIMPORT
tpDrawOnMapCallback* SetDrawOnMapCallback(tpDrawOnMapCallback* dc);

CIMPORT
void DrawPicture(int x,int y,int File,int Sprite,byte Nat);
CIMPORT
int GetPictureIndex(char* name);
CIMPORT
void SetCurrentDiffuse(DWORD Color);
CIMPORT
void SetDrawScale(float scale);
CIMPORT
void FlushPictures();
CIMPORT
void DrawRotatedPicture(int x,int y,int File,int Sprite,byte Nat,
						int dx,int dy,float Angle,float sx,float sy);


int	gDrawOnMiniMap(int x,int y,int Lx,int Ly,int mx0,int my0);
//***********************************************************************************


#define REG(x)   RegisterVar(&##x,sizeof x)
#define WOOD	0
#define GOLD	1
#define STONE	2
#define FOOD	3
#define IRON	4
#define COAL	5
#define RECRT	9	

#define LAND_AI     0
#define MEDITERR_AI 1
#define WATER_AI    2

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

//-----------------------------------------//

#pragma warning(disable : 4035)
inline int Norma(int x,int y){
	__asm{
		mov		eax,x
			cmp		eax,0
			jge		ggg1
			neg		eax
ggg1:	mov		ebx,y
		cmp		ebx,0
		jge		ggg2
		neg		ebx	
ggg2:	mov		ecx,eax
		cmp		eax,ebx
		ja		ggg3
		mov		ecx,ebx
ggg3:	add		ecx,eax
		add		ecx,ebx
		shr		ecx,1
		mov		eax,ecx
		};
	};

//#include "OneObj.h"
using namespace std;

//FROM COSS_2
#include <assert.h>
#include <Resfile.h>
#include <ClassEngine.h>
#include <DString.h>
#include <more_types.h>
#include <ZonesAndGroupsExtraInfo.h>
#include <Dialogs\\MISSIONSCRIPT\\MissionScript.h>
// Vitya FILES
#include "ExternDeclaration.h"
#include "MainFunction.h"
#include "MainClass.h"
#include "MissionInit.h"
#include <Dialogs\\MISSIONSCRIPT\\EndMissionData.h>
#include <MessageMgr.h>

#include "QUESTS\\KillEnemy00.h"

//FROM DIPSERVER
//#include "..\COSSACKS2\stdheader.h"	//grey [22.12.2003]
//#include "..\DipServer\BasicDiploRelation.h"
//#include "..\DipServer\DIP_SimpleBuilding.h"
//#include "..\DipServer\DiplomacySystem.h"
//CIMPORT void GetDipSimpleBuildings(int &NDips, DIP_SimpleBuilding** &Dips);	//grey [22.12.2003]


struct GAMEOBJ /*: public BaseClass*/{

	word Index;
	word Serial;
	int  Type;

/*	SAVE(GAMEOBJ);
	REG_MEMBER( _WORD, Index  );
	REG_MEMBER( _WORD, Serial );
	REG_MEMBER( _int,  Type   );
	ENDSAVE;*/
}; 


// creating Army
DLLIMPORT	void	CreateHeroArmy( GAMEOBJ* pGroup, int NI, ClassArray<CTYPENUM>* pArmy, int X0, int Y0, int Fi );
DLLIMPORT	void	CreateHeroArmy1( GAMEOBJ* pGroup, int NI, ClassArray<CTYPENUM>* pArmy, int X0, int Y0, int Fi );
CIMPORT		void	SetCurrentProgress(char* Message,int Stage,int maxStage);
DLLIMPORT	int		GetAmountOfZones();
DLLIMPORT	char*	GetZoneName(int Index);
DLLIMPORT	int		GetZoneXbyName(char* name);
DLLIMPORT	int		GetZoneYbyName(char* name);
DLLIMPORT	int		GetZoneRbyName(char* name);
DLLIMPORT	int		GetZoneX(int Index);
DLLIMPORT	int		GetZoneY(int Index);
CIMPORT		word	GetObjectBranch(char* NDS_Name);	// род войск юнита: 0 - пехота, 1 - конница
CIMPORT		word	GetObjectBranch(word NIndex);		// род войск юнита: 0 - пехота, 1 - конница
DLLIMPORT	void	SaveEndMissionData(CEndMissionData& EMD);
DLLIMPORT	void	SetBrigadeNKills( int  AttackerNationID, int DefenderNationID ) ; //grey [12.02.2004]
DLLIMPORT	char*	GetResourseName( int id );			//grey [06.02.2004]
DLLIMPORT	bool	HeroCanEscape( int nation );		//grey [02.03.2004]

DLLIMPORT	int		GetSectorDefence( int secID ); //new	//grey [12.07.2004]
DLLIMPORT	int		GetMissionRanks( int& attacker_RankID, int& defenderRank_ID ); //new	//grey [12.07.2004]

DLLIMPORT	int		GetBigMapDisbandSQExp();	  //grey [27.04.2004]
DLLIMPORT	int		GetBigMapSecCaptExp();		  //grey [27.04.2004]
DLLIMPORT	int		GetBigMapHeroDefExp();		  //grey [27.04.2004]
DLLIMPORT	int		GetBigMapQuestDisbSquadExp(); //grey [27.04.2004]
DLLIMPORT	int		GetBigMapQuestCompliteExp();  //grey [27.04.2004]

// Big Map current mission statistics
DLLIMPORT	void SetStatisticsData( int AttSQKilled, int DefSQKilled, 
								    int QuestExp, int QuestNum, int QuestComplete, int QuestSQNum );//grey [14.05.2004]
DLLIMPORT	void SetResStatisticsData( int* AttIncResources, int* DefIncResources, int* AddRes );//grey [14.05.2004]

// FOR AddPulseBox
CIMPORT		void AddPulseBox(int x, int y);
CIMPORT		byte GetMissionPulseZone();
CIMPORT		void SetMissionPulseZone(byte ID);

// FROM DIPSERVER								//grey [22.12.2003]
CIMPORT		word GetNTribes();
CIMPORT		byte GetTribeOwner(word Index);				
CIMPORT		bool SetTribeOwner(word TribeID, byte Owner);
CIMPORT		void GetTribeGoods(word Index, byte& Resource, byte& NPeasants, byte& NPMax);
CEXPORT		bool GetTribeCenter(word Index, int &x, int &y);//new

// VisualInterface CEXT
CIMPORT		void Add_VI_Zone(int x, int y, DWORD color);	//grey [15.06.2004]


// FORMATIONS
extern		GAMEOBJ		fALONE_LV;	//Формация - один юнит.

// strategical capture zones struct ( for capture )
typedef struct {	
		int		Id;				// zone index
		word	rX;				// zone x 
		word	rY;				// zone y
		int		State;			// capture state:	State = 1 - AND, State = 0 - OR
		bool	Captured;		// captured zone in miss
	} stCheckPoint;	// grey [03.03.2004]

// strategical tower&blokhause zones struct ( for build )
typedef struct {	
		int		Id;				// zone index
		word	rX;				// zone x
		word	rY;				// zone y
		int		Weight;			// zone weight:	???
		int		State;			// zone state:	???
		int		BldTime;		// bld build real time
		int		BldIndex;		// bld OneObject* G->Index
	} stTowerBldZone;	// grey [04.08.2004]



//very new
extern	char* UnType2[100];
//very new

extern	int		AttackerIncomeRes[10];
extern	int		DefenderIncomeRes[10];

// MESSAGE SYSTEM
CIMPORT bool	ReLoadMessCampCII();
CIMPORT void	AddMessageMM(BaseMesMgrST* pMessage,char* ParentName);
CIMPORT bool	AddMessageIntoMessCampCII( char* page , char* txt , int id , int param , int state , int x , int y , 
										   char* ParentName ); //grey [18.06.2004]
CIMPORT int		GetActiveMessCampCII(); //grey [18.06.2004]
CIMPORT int		SetActiveMessCampCII(int value); //grey [18.06.2004]


// GAME SOUND
//// CIMPORT void PlaySound(char* Name); 
//DIALOGS_API void PlaySound(char* Name); //grey [04.03.2004]
//DIALOGS_API void PlaySound(char* Name,int x, int y); //grey [04.03.2004]

DLLIMPORT void PlayInMissionSoundCII( char* sound, int sndID = -1 ); //grey [04.03.2004]

// for skirmish //////////////////////////////////////////////////////////
CIMPORT bool	g_addTask(const byte NI,const int x,const int y,const char* name); // position on map in pixel
CIMPORT int		g_delTask(const char* name);

extern	CSAVE_MIS_STRUCT InitData;			// Данные по инициализации миссии.
