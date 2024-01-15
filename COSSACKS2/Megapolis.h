#pragma once
#define N_KINDS 5
#define OrdDist 320
#define NBPERSONAL 3
struct OneBrigAbility{
	byte AbilityKind;
	//0 - [HEAL]            Healing all units (+Value to every unit hp)
	//1 - [HEALTIME]        Healing (+Value/1000 ticks)
	//2 - [HEALPERCENTTIME] Healing (+Value% of total unit hp/1000 ticks)
	//....
	//16 - [TIRING]         Tiring + Value
	//17 - [TIRINGSPEED]    Tiring speed (-Value%)
	//....
	//32 - [ARMDAMAGE]      Damage bonus (arm) + Value
	//33 - [RIFLEDAMAGE]    Damage bonus (rifle) + Value
	//....
	//48 - [SHIELD]         Shield (general) bonus + Value
	//....
	//64 - [MORALEINCSPEED] Increases morale increment speed +Value%
	//65 - [ADDMORALE]      Increases Morale +Value
	//....
	//80 - [SPEED]          Changes speed of motion +Value%
	//....
	//254 - [RADIATE]       Extends abilities in radius
	//255 - [COLLECT]       Collected ability, Value - amount of abils, Collection[] Contains indices
	byte UsageKind;
	//0 - permanent usage
	//1 - one time usage
	//...?...
	word ExistTime;
	int Radius;
	int Value;
	short Collection[4];
	int Cost[8];
	word FileID;
	short IconID;
	char* Message;
	char* ID;
};
class Abilky{
public:
	OneBrigAbility* Abils;
	int NAbils;
	int MaxAbils;
	Abilky();
	~Abilky();
	void Load(char* File);
	int GetAbility(char* ID);
};
#ifdef COSSACKS2
extern Abilky ABILS;
#endif
class AI_Army;
class UnitsGroup;
class Cell8x8{
public:
	union{
		struct{
			byte Peasants;//���-�� �������� �� ���� ������
			byte Towers;//------//------�����
			byte Warriors;
			byte Farms;
			byte Buildings;
			byte Fly;
			byte WaterPeas;
			byte WaterObj;
		};
		byte UnitsAmount[8];
	};
	//byte P_MoveUp;//���-�� ����������� ������� �������
	//byte P_MoveDn;
	//byte P_MoveLf;
	//byte P_MoveRi;
	//byte Protection;  //=4*Towers+Warriors
	//byte Agressivity; //=Warriors
	//byte Importance;  //=Farms+Buildings
	word Neighbor;    //(0x001)  bit0 - UP
					  //(0x002)  bit1 - UP-RIGHT
					  //(0x004)  bit2 - RIGHT
					  //(0x008)  bit3 - DN-RIGHT
					  //(0x010)  bit4 - DN
					  //(0x020)  bit5 - DN-LEFT
					  //(0x040)  bit6 - LEFT
					  //(0x080)  bit7 - UP-LEFT
					  //(0x100)  bit8 - UP+2
					  //(0x200)  bit9 - RIGHT+2
					  //(0x400)  bit10- DN+2
					  //(0x800)  bit11- RIGHT+2
	//byte Resrv[3];
	//word Enemy;
	//word EnemyTime;
	Cell8x8();
};
typedef Cell8x8 CellsInfo [64][64];
typedef CellsInfo TotalCInfo [8];
//extern TotalCInfo  TCInf;
class CityProject{
public:

};
class ProposedProject{
public:
	byte PKind;//=0-Monster,==1-Upgrade
	word NIndex;//����� ���� �������/Upgrade index
	word ProducerIndex;//������ ���������� �������������
	word Percent;//������� �����, ������������� �� ������������
	//int Cost;//���� 
	//int Useful[NBRANCH];
};
struct BuildProject{
	bool Used:1;
	bool PlaceFound:1;
	bool Founded:1;
	bool PeasantsCalled:1;
	byte Usage;
	short NearX;
	short NearY;
	int  x,y;
	int Options;
	word AttemptsToStand;
	word AttemptsToFindApprPlace;
	word Index;
	word NIndex;
	byte MaxPeasants;
	byte MinPeasants;
	byte NPeasantsCalled;
};
class CityCell{
public:
	int x,y;
	word WorkTimes;//==0xFFFF if inside the city
	CityCell* NextCell;
};
class CityCluster{
public:
	CityCell* FirstCell;
	void AddCell(int x,int y);
};
typedef int Functional(int* x,int* y,int Lx,int Ly,byte NI);
typedef int CheckWorkFn(OneObject* OB,City* CT);
class SmartGroup;
class Brigade;
class City;
typedef void BrigadeLink(Brigade* Brig);
class BrigadeOrder;
class BrigadeOrder{
public:
	byte OrdType;
	byte Fast;
	byte Prio;
	word Size;
	char* Message;
	BrigadeOrder* Next;
	BrigadeLink* BLink;
};
struct BrigMemb{
	word Peons;
	word Infantry;
	word Strelkov;
	word Mortir;
	word Pushek;
	word Grenaderov;
	word Other;
	word reserv;
};
int GetBMIndex(OneObject* OB);
#define MAXABL 8
class BrigadeActiveAbilityList;
class CBrigadeOrder;
#include ".\cvi_alert.h"
class Brigade{
public:
	City* CT;
	word* Memb;
	word* MembSN;
	word  SN;
	int* posX;
	int* posY;
	word NMemb;
	word MaxMemb;
	word MembID;
	//bonuses
	char  AddDamage;
	char  AddShield;
	short AddAttackRange;
	short UnitsSpeedBonus;//100=normal speeed
	short ExpGrowSpeed;   //100=normal speed
	short AttackRateBonus;//100=normal speed	
	//end bonus
	void IncBrigExperience(int Value);//100=1 kills
	int GetBrigExp();
	byte WarType;
	byte Direction;
	word ID;
	int BrigDelay;
	int MaxBrigDelay;
	word LastTopology;
	int LastOrderTime;
	word ArmyID;
	word ErasureTime;
	short ScaleFactor;
	bool Enabled:1;
	bool PosCreated:1;
	bool Precise:1;
	bool Strelki:1;
	bool AttEnm:1;
#ifdef COSSACKS2
	bool DontWait:1;
	bool InStandGround:1;
	short VeraVPobedu;
	int NKills;
	word OfficerID;
	word BarID;
	word FlagID;
	byte NGrenades;
	int GrenDelay;
#endif
#ifdef NEWMORALE
	int Morale;
	int MaxMorale;
	int AddMaxMorale;
	int PrevMorale;
	bool WasLess30Lost;
#endif
#ifdef GETTIRED
	bool IsTired;
	int LastTireCheckTime;
#endif
	BrigMemb BM;
	CheckWorkFn* CFN;
	BrigadeOrder* BOrder;
	word LastEnemyID;
	word LastEnemySN;
	//fear factors
	int LastSuddenAttackTime;

	word Abilki[MAXABL];
	word LeftTime[MAXABL];
	word MaxLeftTime[MAXABL];
	byte NABL;
	int GetActivity();
	bool SetActivityState(int State);
	//int GetReformaStatus();
	__forceinline int GetAbilityValue(int Kind){
		int VV=0;
		for(int i=0;i<NABL;i++){
			byte V;
			int K;
			if((V=ABILS.Abils[(K=Abilki[i])].AbilityKind)==Kind){
				VV+=ABILS.Abils[K].Value;
			}else if(V==255){
				int V1=ABILS.Abils[K].Value;
				short* Collection=ABILS.Abils[K].Collection;
				for(int p=0;p<V1;p++){
					if((V=ABILS.Abils[(K=Collection[p])].AbilityKind)==Kind){
						VV+=ABILS.Abils[K].Value;
					};
				};
			};
		};
		return VV;
	};
	void GetBrigadeDamage     (int AttType,int& Damage,int& AddDamage,OneObject* Killer=NULL,OneObject* Victim=NULL);
	void GetBrigadeProtection (int AttType,int& Shield,int& AddShield,OneObject* Killer=NULL,OneObject* Victim=NULL);
	int GetSkillStatus(float& avkills);
	void SetIndex();
	void Init(City* CT,word ID);
	void AddObject(OneObject* OB);
	void CheckMembers(City* CT);
	void RemoveObjects(int NObj,Brigade* Dest);
	bool RemoveOne(int Index,Brigade* Dest);
	void RemovePeasants(Brigade* Dest);
	void FreeMember(int Idx);
	void DeleteAll();
	void CreateSquare();
	void CreateConvoy(byte Type);
	int AddInRadius(int x,int y,int r,BrigMemb* BMem,Brigade* Dest);
	int AddInRadius(int x,int y,int r,BrigMemb* BMemb);
	BrigadeOrder* CreateBOrder(byte OrdType,int Size);
	int SelectPeasants(byte NI);
	void Rospusk();
	bool DisbandOne();
	int lastTimeGetCenter;
	int lastCenterX;
	int lastCenterY;
	byte lastDir;
	bool GetCenter(int* x,int* y); 
	bool GetCenter(int* x,int* y,byte* Dir); 
	//----------------------ORDERS----------------------//
	bool LocalSendTo(int x,int y,byte prio,byte OrdType);
	bool LinearLocalSendTo(int x,int y,byte prio,byte OrdType);
	bool WideLocalSendTo(int x,int y,byte prio,byte OrdType);
	bool GlobalSendTo(int x,int y,byte prio,byte OrdType);
	bool CaptureMine(int SID,byte prio,byte OrdType);
	bool AttackEnemy(int x,int y,byte prio,byte OrdType);
	void MakeBattle();
	void ProtectFarMines();
	//-----------------Human functions------------------//
	bool CreateNearOfficer(OneObject* OB,word Type,int ODIndex);
	bool CreateFromGroup(UnitsGroup* UG,int ODIndex,byte Dir);
	void CreateOrderedPositions(int x,int y,char dir);
	void CreateSimpleOrderedPositions(int x,int y,char dir);
	void HumanCheckUnits();
	void HumanLocalSendTo(int x,int y,short Dir,byte Prio,byte OrdType);
	void HumanGlobalSendTo(int x,int y,short Dir,byte Prio,byte OrdType);
	void KeepPositions(byte OrdType,byte Prio);
	void Bitva();
	//--------------------------------------------------//
	void ClearBOrders();
	void DeleteBOrder();
	void ResortMembByPos();
	
	void SetNMask(byte Mask);
	byte GetNMask();

	byte GetFormIndex();	// line,squad,care,...
	Brigade* GetNearesEnmBrigade();
	byte GetDirToNearesEnmBrigade();

	word AutoFormID;
	int FillDelay;	// time when can reinforce after attack by enemy which disable reformation

	byte NMask;
	word OnlyThisBrigadeToKill;
	byte NIOnlyThisBrigadeToKill;

	word vLastEnemyID;	// last units which made damage on this brigade
	word vLastEnemySN;
	word vLastEnemyNI;	// nation which erase this brigade
	BrigadeActiveAbilityList* Ability;
	
	CBrigadeOrder* NewBOrder;
	void ClearNewBOrders();
	void DeleteNewBOrder();
	bool CreateNewBOrder(byte OrdType,CBrigadeOrder* Order);
	bool GetAbilityChange(int& AddDam, int& AddShd);
	bool GetBrigadeDestination(int& dx, int& dy);
	OneObject* ThrowOneGrenade(OneObject* EOB);//EOB - enemy object
	byte NUnitsNotInPos;
	// by vital, Map Morale Desk
	int M;
	int HideTime;
	int mmActive;
	//
	cvi_Alert Alert;
};
typedef void GroupMaker(SmartGroup* GS);
class GroupOrder;
class GroupOrder{
public:
	byte		OrderType;
	GroupOrder* NextOrder;
	GroupMaker* GroupLink;
	void*       GroupData;
	int         GDataSize;
};

class Idea;
typedef void IdeaBrain(Idea* IDEA);
typedef void FreeIdea(Idea* ID);
class Idea{
public:
	City* CT;
	Idea* PrevIdea;
	Idea* NextIdea;
	IdeaBrain* Brain;
	void* IdeaData;
	FreeIdea* FI;
	int   DataSize;
	void ClearIdea();
};
class Inform;
//typedef void I_Save(SaveBuf* SB,Inform* Inf);
//typedef void I_Load(SaveBuf* SB,Inform** Inf);
typedef void I_Clear(Inform* Inf);
class Inform{
public:
	word ID;
	word Essence;
	Inform* Next;
	Inform* Previous;
	City*   CT;
	I_Clear* IClr;
	word Size;
};
class MineBase:public Inform{
public:
	word   M_ID;
	word   M_SN;
	short  topx;
	short  topy;
	byte   ResKind;
	byte   UpgLevel;
	byte   PSend;
	word   DefendersID;
	word   DefendersSN;
	word   MinersID;
	word   MinersSN;	
};
class PRM_Info:public Inform{
public:
	int NBrigs;
	word BrigsID[5];
	word BrigsSN[5];
};
class SmartGroup : public Brigade{
	bool Enabled;
	word Index;
	word NFarInf;
	word NNearInf;
	word NFarCav;
	word NNearCav;
	short CenterX;
	short CenterY;
	GroupOrder* Order;
};
#define MBR4    16
#define MaxBrig (MBR4*32)
#define MaxArm 128
class DefendInfo{
public:
	byte  x;
	byte  y;
	byte  Importance;
	word  NDefenders; 
	word  MaxDefs;
	word* Def;
	word* DefSN;
	void AddUnit(OneObject* OB);
};
class AI_Army;
//----------------------ARMY--------------------//

#define PolkSize   36
#define NBrigTypes 6
class ExtendedBrigade{
public:
	byte BrigadeType;
	//0-Short range infantry
	//1-Long range infantry
	//2-Fast cavalry
	//3-Hard cavalry
	//4-Long range cavalry
	//5-Artillery
	//6-Sapers
	//7-Builders
	int Force;
	int NeedMembers;
	word NextBrigade;
	Brigade* Brig;
};
typedef void ArmyLink(AI_Army* ARM);
class ArmyOrder;
class ArmyOrder{
public:
	byte OrdType;
	byte Prio;
	word Size;
	char* Message;
	ArmyOrder* Next;
	ArmyLink*  ALink;
};
struct FormInfo{
	word ComID;
	word ComSN;
	word BarID;
	word BarSN;
};
class AI_Army{
public:
	int NExBrigs;
	int MaxExBrigs;
	bool Enabled:1;
	bool SpecialOrder:1;
	int TopPos;
	int x,y;
	int Force;
	byte Spec;
	word ArmyID;
	ExtendedBrigade* ExBrigs;
	//Additional information
	City* CT;
	Nation* NT;
	byte NI;
	int LastBitvaTime;
	word FirstBrig;
	//Order information
	ArmyOrder* AOrder;
	//Formation info;
	word ComID[8];
	word ComSN[8];
	word NCom;
	word BarID[8];
	word BarSN[8];
	word NBar;
	word NFreeUnits;
	//Functions
	void InitArmy(City* CT);
	void ClearArmy();
	void CreateMinimalArmyFromBrigade(Brigade* BR,int Type);
	void AddBrigade(Brigade* BR);
	void AddUnit(OneObject* OB);
	void CloseArmy();
	void MoveArmyTo(int x,int y);
	void CreateBrigLink();
	void AttackArea(int x,int y);
	ArmyOrder* CreateOrder(byte OrdType,int Size);
	void DeleteAOrder();
	void ClearAOrders();
	void LocalSendTo(int x,int y,byte Prio,byte OrdType);
	int  GetArmyDanger(int x,int y);
	void SendToMostSafePosition();
	void WideLocalSendTo(int x,int y,byte Prio,byte OrdType);
	void Parad();
	void ConnectToArmy(int ID,byte Prio,byte OrdType);
	void MakeBattle();
	void Bitva();
	void Diversia();
	void SetZombi();
	void ClearZombi();
	bool CheckArtillery();
	int  GetAmountOfBrigs(byte Type);
	void SendGrenaders();
};
#define MaxIslGroup 32
class City{
public:
	//new AI implementation
	int AllPeasantsGroup;
	int LasyPeasantsGroup;
	int OnWoodGroup;
	int OnStoneGroup;
	int OnFoodGroup;	
	int WoodSkladGroup;
	int StoneSkladGroup;
	int MelnGroup;
	int BridgeGroup;
	int NBridgeBuilders;	// need to exist for constructing

	int SettlementPriory[6];

	//---------------------
	Nation* Nat;
	byte NI;
	//int InX,InY,InX1,InY1,OutX,OutY,OutX1,OutY1;
	//int TryTimes;
	//int ExpandSize;
	int Amount;
	//int GoldPeasPart;
	//int WoodPeasPart;
	//int BuildPeasPart;
	//word KindMax[N_KINDS];//����. ���-�� ������ ���� Kind
	//word *MKList[N_KINDS];//������ ���������� �� ������� � ��������� �������� ������� ����
	//Functional* FNC[N_KINDS];//������ ������������ ��� ������ �����, ���� ����� ������ �� ��������, �� �����. -1
	//word BranchPart[NBRANCH];
	int AutoEraseTime;
	word BestProj[128];
	word NBestProjects;
	byte NWoodSklads;
	byte NStoneSklads;
	char AIID[48];
	byte StartRes;
	byte ResOnMap;
	byte Difficulty;
	byte LandType;
	word TransportID;
	word TransportSN;
	int MaxInTransport;

	byte NMeln;
	word MelnID[4];
	word MelnSN[4];

	int  MelnCoorX[4];
	int  MelnCoorY[4];
	int  NMelnCoor;

	int  FieldsID[512];
	word FieldsSN[512];
	word NFields;

	word Construction[64];
	word ConstructionSN[64];
	word ConstructionTime[64];
	word NConstructions;
	word DestBldGroup;
	word DestUnitsGroup;
	word DestEnemyGroup;
	word NationalPeasantID;
	word DesantStartGroup;
	word DesantDestGroup;
	word AllWariorsGroup;

	word FarmsUnderConstruction;
	int Account;

	bool FieldReady;
	bool AllowWood;
	bool AllowStone;
	bool AllowMine;
	bool DefenceStage;

	word NeedOnField;
	word NeedOnStone;
	word NeedOnWood;

	word** GroupsSet;
	word*  NGroupsInSet;
	int    NGroups;

    int NBrigs;
	Brigade Brigs[MaxBrig-11];
	Brigade Settlers;
	Brigade Defenders;
	Brigade Guards;
	Brigade Agressors;
	Brigade OnField;
	Brigade OnStone;
	Brigade OnWood;
	Brigade Builders;
	Brigade Free;
	Brigade InMines;
	Brigade FreeArmy;
	byte N_mineUp[10];
	word MaxFields;
	word BestOnField;
	word WaterBrigs[128];
	//------------------ARMIES-------------------//
	AI_Army ARMS[MaxArm];
	word* DefArms;
	int NDefArms;
	int MaxDefArms;
	int NDivr;
	byte LockUpgrade[8];
	byte MyIsland;
	//----------morale influence params----------//
	int LastGolodTime;
	int LastBarrackDestroyTime;
	int LastPremiumTime;
	//-------------Peasants balance--------------//
	short NPeas;
	short NeedPF;
	short NeedPW;
	short NeedPS;
	short FreePS;
	int   WoodAmount;
	int   StoneAmount;
	int   FoodAmount;
	int   LastTime;
	int  WoodSpeed;
	int  StoneSpeed;
	int  FoodSpeed;
	// academy info (by vital)
	int ResourcePeasants[6];
	//-------------------------------------------//
	int   LFarmIndex;
	int   LFarmRadius;
	short FarmLX;
	short FarmLY;		
	//-------------------------------------------//
	word  MAX_WORKERS;
	//-------------------------------------------//
	bool AI_DEVELOPMENT		:1;
	bool AI_WATER_BATTLE	:1;
	bool AI_LAND_BATTLE		:1;
	bool AI_MINES_CPTURE	:1;
	bool AI_TOWN_DEFENCE	:1;
	bool AI_MINES_UPGRADE	:1;
	bool AI_FAST_DIVERSION	:1;
	//-----------DEFENDING THE BASE--------------//
	DefendInfo* DefInf;
	int NDefn;
	int MaxDefn;
	int MaxGuards;
	int AbsMaxGuards;
	//-------------------------------------------//
    DynArray<word> TheyMovesInto;
	DynArray<word> MoversIndices;
	DynArray<word> TheyMovesInto_prev;
	DynArray<word> MoversIndices_prev;
	word NP;
	word NAgressors;
	word MinType;
	word MaxType;
	word UnitAmount[2048];	// Amount of units of some kind
	word ReadyAmount[2048];
	word PRPIndex[2048];	// index of project to produce this type of monster,if not proposed,0xFFFF
	word UPGIndex[4096];
	word UnBusyAmount[2048]; // ������� ������� �����������
	word Producer[2048];
	bool PresentProject:1;
	bool IntellectEnabled:1;
	bool CenterFound:1;
	short CenterX;
	short CenterY;
	word BUILDLISTID[64];
	word BUILDLISTSN[64];
	int  NBuild;
	BuildProject BPR;
	word NProp;
	ProposedProject Prop[100];
	Idea* IDEA;
	Inform* INFORM;
	Idea* AddIdea(IdeaBrain* IBR,bool Dup);
	void AddInform(Inform* Inf,I_Clear* ICL);
	void DelInform();
	void DelInform(Inform* Inf);
	Inform* SearchInform(word ID,word Essence,Inform* inf);
	Inform* SearchInform(word ID,Inform* inf);
	void HandleIdeas();
	void DelIdeas();
	word EnemyList[32];
	void CreateCity(byte NI);
	bool CheckTZone(int x,int y,int Lx,int Ly);
	bool TryToFindPlace(int* x,int* y,int Lx,int Ly,byte Kind);
	void StartEnumUnits();
    void PostEnumUnits();
    void EnumUnits();
    void EnumOneUnit(OneObject* OB);
    static void EnumAllUnits();
	void AddProp(byte NI,word NIN,GeneralObject* GO,word prop,word per);
	void AddUpgr(byte NI,word NIN,word prod,word per);
	void EnumProp();
	void ProcessCreation();
	void HelpMe(word ID);
	void HandleFly();
	word FindNeedProject();
	void RefreshAbility();
	bool FindApproximateBuildingPlace(GeneralObject* GO);
	bool FindPreciseBuildingPlace(int Type);
	void AddConstruction(OneObject* OB);
	void HandleConstructions();
	void MarkUnusablePlace();
	void CloseCity();
	void HandleFields();
	void MakeZasev();
	void MakeSlowZasev();
	void HandleBrigades(int NP);
	void RegisterNewUnit(OneObject* OB);
	int  GetFreeBrigade();
	void ExecuteBrigades();
	void CalculateBalance();
	void GetNextPlaceForFarm(int* x,int* y);
	void UnRegisterNewUnit(OneObject* OB);
	void RegisterWaterBrigade(int ID);
	void UnRegisterWaterBrigade(int ID);
	//---------------Armies--------------
	int GetFreeArmy();

	//--------------Ideas----------------
	void TakeNewMine();
	void UpgradeSomeMine();
	void CreateWallNearMine();
	void BuildTower();
	void UpgradeMines();
	void SendAgressors();
	void ProtectMine();
	void BuildWallsNearMines();
	//----------------defending--------------//
	void HandleDefending();
	int  CheckDefending(byte x,byte y); 
	void AddDefending(byte x,byte y,byte Importance);
	void AddUnitDefender(OneObject* OB);
};
class Forces{
public:
	int MinX;
	int MinY;
	int MaxX;
	int MaxY;
	int Damage;
	int Life;
	int Shield;
	int Protection;
	int Population;
	int StrongFactor;
	int CenterX;
	int CenterY;
	int NBuild;
	void SetDefaults();
};
#define SendPTime 10000
class SendPInform:public Inform{
public:
	int time;
};
struct DangerInfo{
	word ID;
	word SN;
	word Kind;
	word UpgradeLevel;
	word Life;
	word MaxLife;
	int x,y,z;
	int MinR;
	int MaxR;
};

extern Forces NForces[8];
extern int BestForce;
extern int BestNat;
extern int NextBest;
void OutCInf(byte x,byte y);
extern int COUNTER;
extern DWORD BMASK[MBR4];
void InitSuperMortiraCells();
void CreateEnmBuildList();
void CreateProtectionMap();
struct CostPlace{
	int xw,yw;
	int xc,yc;
	byte Island;
	byte R;
	byte Access;
	word Transport;
};
extern CostPlace* COSTPL;
extern int NCost;
extern int MaxCost;