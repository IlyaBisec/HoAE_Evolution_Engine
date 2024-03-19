#pragma once
// BrigadeAI.h: interface for the BrigadeAI class.
//
//////////////////////////////////////////////////////////////////////
#ifdef COSSACKS2
#ifndef AFX_BRIGADEAI_H__1A25F3B9_A008_46AD_A880_B90797A576B7__INCLUDED_
#define AFX_BRIGADEAI_H__1A25F3B9_A008_46AD_A880_B90797A576B7__INCLUDED_

#include <ClassEngine.h>
#include "Polk.h"

#define DIALOGS_API __declspec(dllexport)

//====================================================================//
int GetDamage(word ShooterID,word VictimID,int AttType);
word GetDir(int dx,int dy);
void MakeReformation(byte NI,word BrigadeID,byte FormType);
class DString;

typedef tpBirthCallback(int NewbornIndex);

class SaveBuf;
//====================================================================//
// Класс надстройка над Brigade + пушки
// т.е. для каждого отряда или пушки создается экземпляр Atomа
class Atom
{
public:
	Atom();
	Atom(Brigade* Br);
	Atom(OneObject* Ob);
	virtual ~Atom();
	int AtomIndex;
	int x;
	int y;
	int Force;
	byte Dir;
	int CheckTime;
	int CheckTimeInterval;
	Brigade* BSelf;
	int CannonID;
	OneObject* C;
	int Tower_ID;
	OneObject* Tower;
	int UnitsAmo;
	int FireReady;
	int FireTimeDelayLeft;
	int Type;
	int Speed;
	int Tired;
	int SumLife;
	byte NI;
	void UpdateParams();
	bool IsStand;
	int GetSizeFront();
	int GetSizeBack();
	int GetSizeLeft();
	int GetSizeRight();
	int LockType;
	int StandTime;
	byte NMask;
	virtual byte GetNMask();
	virtual bool IsAlive();
	word GetLockType();
	DWORD Serial;
	int GetStandTime();
	word NearestEnmBrigade;
	int DistToNearestEnmBrigade;
	int CheckTimeNearestEnmBrigade;
	int LastAlarmTime;
};
//====================================================================//
class AreaInfo
{
public:
	AreaInfo();
	AreaInfo(int I);
	~AreaInfo();
	int Index;
	int x;
	int y;
	int H;
	int dH;
	byte DieDir;
	byte NTrees;
	byte NWay[8];
	byte CWay[8];
	void ClearNWay();
};
//====================================================================//
struct KeyPoint
{
	int x,y;
	int Width;
	byte State;
	byte Type;
	byte LR;//0-L;1-R
	word Creator;
};
//====================================================================//
class AutoForm;
struct sector
{
	int dest;
	Atom* A;
	AutoForm* AF;
};
struct Direction
{
	sector Sectors[32];// отсортерованы по возрастанию
	int SectorsAmo;
	int SumDang;
	int Dang;
	int FireDang;
	int SumFireDang;
	byte Dir;
};
//====================================================================//
class Rules;
class Rule;
//Отряд с включеным ИИ - это основной класс, новые команды отрядам добовлять здеся.
class AutoForm : public Atom
{
public:
	AutoForm();
	AutoForm(Brigade* B);
	virtual ~AutoForm();

	int Index;
	int RegimentIndex; // индекс полка к которому отряд пренадлежит (deff -1)
	int NextProcesedTime;
	int TimeInterval;
	int FormationTypeIndex;
	void virtual Process();
	virtual void MoveTo(int x, int y);
	virtual void MoveTo(int x, int y, byte dir);

	virtual void SimpleMoveTo(int x, int y);
	virtual void SimpleMoveTo(int x, int y, byte dir);
	
	Direction EnmAng[16];   // враги вокруг, окружность разбита на 16 секторов
	Direction FriendAng[16];// аналогично друзья
	void ClearEnmAng();
	void AddToEnmAng(Atom* A);
	void ClearFriendAng();
	void AddToFriendAng(AutoForm* AF);
	
	int LastTimeCheckXY;
	int AimX; // куда идем
	int AimY;
	byte AimDir;
	
	int mtX;
	int mtY;
	byte mtDir;
	int mtXr;
	int mtYr;
	int mtXl;
	int mtYl;

	byte AvrEnmDir;
	void CalkAvrEnmDir();
	byte GetAvrEndDirIn(int x, int y);

	int GetDistToAim();
	void MoveToAim();
	void StepForward();
	void StepBack();
	void MoveBack();
	void TurnLeft();
	void TurnRight();
	void EnableFire();
	void FireNow();
	void DisableFire();
	void SetReadyToFire();
	void SetNotReadyToFire();
	void SetStandGround(bool vle);
	void FormateToLine();
	void FormateToSquare();
	void FormateToKare();
	void Stop();
	void Castle();
	void MoveToMT();
	void TurnToMT();
	void FireToMT();
	
	int GetMainTargetAngl();
	Atom* GetMaxDanderAtom();
	Atom* GetNearestAtom();
	Atom* GetNearestAtomIn(int x, int y);
	//
	void FormateTo012(int Type);
	//Situation
	int IsFireReady(Brigade* Br);
	int GetDistTo(Brigade* Br);
	int GetMoral(Brigade* Br);
	int GetNTo30Limit(Brigade* Br,int* VictimAmo);
	int GetDieAmount(Brigade* Br,int* ReadyShuters);
	int CanIKill(Brigade* Br);
	//
	virtual int GetSituationByIndex(int I);
	int GetMaxValueSituationByIndex(int I);
	//
	byte GetMaxDangDirections();
	void TurnTo(byte dir);
	int Moral();
	int GetFireReady();
	int UnitsAmount();
	int GetTired();
	//
	Atom* MainTarget;
	//
	int DistToMT();
	int CanIKillMT();
	int MoralMT();
	int MoralDiff();
	int MoralMTAfterFire();
	int KillAmoMT();
	int FireReadyMT();
	int UnitsAmountMT();
	int TiredMT();
	int AngToMT();
	//int DistToSecondaryTarget();
	int SumDangAndForce();
	bool HaveReserv();
	//
	//
  
	bool MessageArr[16];
	//bool OrderArr[16];
	int ActionsArr[128];
	int LastCom;
	int GetMaxActions();
	void ExecuteActions(int ActionsID);
	int TimeToDoActions[16];
		/*
		0 - MoveToMT
		1 - TurnToMT
		2 - MoveBack
		3 - SetReadyToFire
		4 - EnableFire
		5 - DisableFire
		6 - Stop
		7 - FormateToLine
		8 - FormateToSquare
		9 - FormateToKare
		10- FindPlaceInFront();
		11- TurnToMaxDang();
		12-MoveToAim
		*/
	bool StateArr[32];
	bool static NonStateArr[32][32];
		/*
		0  - Attack
		1  - Reserv
		2  - MoveBack
		3  - Stand
		4  - Front
		5  - March
		6  - NearAim
		7  - MoveFront
		8  - BattleFormation
		9  - Rest
		10 - Move
		11 - ReadyToFire
		12 - LineFormation
		13 - SquareFormation
		14 - KareFormation
		15 - EnableFire
		16 - DisableFire
		*/
	bool GetState(int Index);
	void SetState(int Index, bool Vle);

	int RulesID;
	Rules* RulesList;
	void ProcessRules();
	int NextTimeProcessRules;

	void MoveToFrontOf(AutoForm* A);
	void MoveToBackOf(AutoForm* A);
	void MoveToLeftOf(AutoForm* A);
	void MoveToRightOf(AutoForm* A);

	void FindPlaceInFront();
	void FindPlaceInReserv();
	void TurnToMaxDang();
	
	//
	AutoForm* GetFriendInFront();
	AutoForm* GetFriendInBack();
	AutoForm* GetFriendInLeft();
	AutoForm* GetFriendInRight();
	int GetMaxDangDirectionMinus();
	int GetNearestFriendDirection();
	bool IsInBatleFormation();
	//
	bool IsNearAim();
	int ForceDifference();
	int FireForceDifference();
	int FireDanger();
	int Encircle();
	//
	int DistToMainTarget;

	int DistInFront();
	void KeepDistInFront();
	
	int DistInReserv();
	void KeepDistInReserv();

	void TakeInTheRear();

	virtual void Fire();
	virtual void SplashFire();
	int GetStand();
	bool IsEnemOnWay();
	int GetNGrenades();
	void ThrowGrenade();
	void ThrowGrenadeInABuilding();
	void MeleeAttack();
	void AttackAloneEnmUnits();
	int GetHorseDanger();
	void KeepPositionHorce();
	int GetForestDensity(Brigade* B);
	int GetSelfForestDensity();
	int GetMTForestDensity();
	int IsMTCannon();
	int KartechDanger();
	int FriendlyKartechDanger();
	int IsInBatter();
	bool IsInFront();
	int GetBarrierToMT();
	int GetFOOD();
	int GetCOAL();

	KeyPoint* KeyPoints;
	int NKeyPoints;
	void FillKeyPoints();
	void ClearKeyPoints();
	bool FindNearestKeyPoints(int &p1, int &p2);
	void FindMoveToCoord(int &x, int &y, byte &dr, int p1, int p2);

	int GetMTStandTime();
	int GetMeleeForceDiff();
	int GetNUnitsDiff();
	int Random();
	int IsStandGround();
	int IsMTStandGround();
	int PolkSynchronCommand;
	int GetPolkCommand();
	int GetDistToEnmTower();
	int GetDistToEnmBuilding();

	void OneLineShot();
	

	int GetAloneEnmUnits();
	static bool IsEnmAlone(OneObject* OB,void* param);
	static bool GetEnmAlone(OneObject* OB,void* param);

	bool Movement;
	
	GAMEOBJ RemanUnits;
	int virtual GetRealNUnitsToReman(word& Type);
	int virtual GetNUnitsToReman(word& Type);
	void Reman();
	int virtual GetUnitsAmount();

	void DoNothing();
	int LastCommTime;
	int GetLastCommTime();
	int GetDistToFriendlyForm();
	int GetNFormationsInMyPolk();

	int GetGameTime();
	int GetFrontage();

	int GetDiff();
	int IsMTAI();

	int GetProcDistToAimInPolk();

	void ArmAttack();
	
	//Cannon
	virtual int GetKartechEffect();
	virtual int GetMaxDensityInZerroSector();
	virtual int GetMaxDensityInSectors();
	int GetPushkaChargeState0();
	int GetPushkaChargeState1();
	int GetFillingAbility();
	void ChargeCannon0();
	void ChargeCannon1();
	void TurnCannonToMaxEnmFormationSector();
	void FireOnEnmTower();
	void FireOnEnmBuilding();
	//
	void Ram();
	int GetMTAngl();
	int GetMaxHightInMTDirection();
	int GetNApparentFriendlyForm();
	int GetNApparentEnemyForm();

	int CanSetBomb();
	void SetBomb();

	int NeroTypeIndex;
	int NeroNameIndex;
	
	float NeuroTempInaccuracy;

	virtual void ClearOrders();

	int virtual Load(SaveBuf* SB);
	void virtual Save(SaveBuf* SB);
	
	void virtual ShowInfo();//Debug
private:
	void SetNoSearchVictim(bool vle);
	void AttackBrigade(Brigade* B);
	void MeleeAttackBrigade(Brigade* B);
	void SetFireState(Brigade* B, bool State);
	void GetCoordToMTWay(int &x, int &y, AutoForm* FR);
	AutoForm* FindNearestFriendInMTDirection();
	bool CorrectDestPoint(int x, int y, byte dir);
	OneObject* GetNearestEnmBuilding();
	static bool BuildIsEnm(OneObject* B,void* param);
	int HorceDanger;
	int FunctionResultCash[150][2];
};
//====================================================================//
// Пушка под ИИ
class Cannon : public AutoForm
{
public:
	Cannon();
	Cannon(int Index);
	virtual ~Cannon();
	
	//void Process();
	//void UpdateParams();

	int FireToX;
	int FireToY;
	int FindBatterCoordInSector(int SectorN, int& x, int& y);
	void SetBatterCoord(int x, int y);
	void PreKartech();
	
	void Fire();
	void SplashFire();
	int GetKartechEffect();
	int GetMaxDensityInZerroSector();
	int GetMaxDensityInSectors();

	virtual void MoveTo(int x, int y, byte dir);
	virtual void SimpleMoveTo(int x, int y, byte dir);

	int static NnFr;
	int static NnEn;
	int static KsFr;
	int static KsEn;
	void static CheckUnitsInAngle(OneObject* OB,WeaponInSector* WS,byte Angle,int param);
	
	int ShotType;
	int virtual GetNUnitsToReman(int Type);
	int virtual GetUnitsAmount();

	int Load(SaveBuf* SB);
	void Save(SaveBuf* SB);

	void ShowInfo();//Debug
};
//====================================================================//
struct Situation
{
	byte CompareID;
	int FunctionID1;
	int Value1;
	int FunctionID2;
	int Value2;
};
struct Action2
{
	int ActionID;
	int Weight;
	int Value;
};
//====================================================================//
// одно правило
class Rule
{
public:
	Rule();
	virtual ~Rule();
	Situation Situations[32];// условия
	Action2 Actions[16];   // действия
	int Weight;
	int Type;
	Rule* Next;
	bool CheckRule(AutoForm *A);
	bool CheckSituation(AutoForm *A, int i);
	void FillSituations(DString *s);
	void FillActions(DString *s);
private:
	void SetSituation(char* p1,char *iff, char *p2, Situation *sit);
	int GetFunctionID(char* p);
	int GetCompareID(char* iff);
	int GetActionID(char* aa);
	int GetStateID(char* aa);
};
//====================================================================//
// правила поведения для юнитов 
class Rules
{
public:
	Rules();
	~Rules();
	int Index;
	DString m_fileName;
	//int UnitType;
	//char TType[32];
	DynArray<int> UnitsT; // типы юнитов для которых эти правила
	Rule* First;
	Rule* Last;
	void VerifySituation(AutoForm *A);
	////bool Compare(Situation* s1, Situation* s2);
	void LoadRules(DString *d);
private:
	Rule* Pointer;
};
//====================================================================//
// Типо ИИ полка но единственное что он делает это 
// установление дистанции до противника отрядам полка
// и расстановка отрядов на месте прибытия.
class Regiment : public Polk
{
public:
	Regiment();
	Regiment(int I, AutoForm** AFL, int* MaxAFL);
	~Regiment();
	void MovePolkTo(int x, int y, int Dir=0);
	int EnemyForce;
	int GetEnemyForce();
	int GetPolkIndex();

	int Index;
	int DistX;
	int DistY;
	int DistDir;
	int Radius;
	int SearchEnemyRadius;
	bool AddBrigade(Brigade* B);
	bool AddCannon(OneObject* Ob);
	void AddSelectedToPolk(byte Nat);
	void AddBrigadeToPolk(word Nat, int BrIndex);
	bool ReSetFormation(int FromIndex);
	AutoForm* GetAutoForm(int UnitType);
	int GetFormationIndex(int UnitType);
	int  GetNFormations();
	bool virtual GetFormation(int Index, byte& NI, int& BrigID);
	bool virtual GetPolkFrontCenter(int& x, int& y);
	void Process();
	AutoForm** AFList;
	int* MaxAFList;
	int AddFormation(Brigade* B);
	void FillTable();
	void AnalizeTable();
	int TableFillTime;
	AutoForm* P[128];
	Atom* E[128];
	int MaxP;
	int MaxE;
	int TDist[128][128];
	bool TVisible[128][128];
	bool TNearestP[128][128];
	bool TNearestE[128][128];
	bool Chase;
	bool IsChase();
	void SetChaseOn();
	void SetChaseOff();
	int Load(SaveBuf* SB);
	void Save(SaveBuf* SB);
	GAMEOBJ RemanGroup;
	void RemanPolkFrom(GAMEOBJ* Group);
	void RemanProces();
	int  GetUnitsAmountInPolk(bool reman=false);
	int SynchronCommand;
	int SynchronCommandTime;
private:
	int AddE(Atom* A);
	void AddFillEn(AutoForm* AF);
	int GetActualDist();
	int GetActualDistOnWay();
	int GetActualDistInAim();
	void SetActualDist();
	void SetDistIfEnmAlone();
	void FillDestPoints();
};
//====================================================================//
// ИИ отрядов и ему подобных - создается в единственном экземпляре
class BrigadeAI  
{
public:
	BrigadeAI();
	virtual ~BrigadeAI();
	void Init();
	void Process();
	static int AddAutoBrigade(Brigade *B);
	static int AddAutoCannon(OneObject *Ob);
	static bool FirstStart;
	static AutoForm* AF[512];  // Формации для которых включен ИИ
	static int MaxAF;
	static Regiment* Reg[128]; // Полки
	static int MaxReg;
	static int CreateRegiment();
	Polk* GetPolkByIndex(int PIndex);
	
	//static Rules AllRules[32]; // Правила поведения отрядов и пр.
	static DynArray<Rules*> AllRules;
	void LoadAllRules(char* p, const char *shotfilename);
	void LoadAddRules(char* filesextension);

	//static Atom* AllForm[8][256];
	//static int AllFormAmo[8];
	static DynArray<Atom*> AllForm; // Все(ИИ и не его) формации и пушки на карте
	static DynArray<Atom*> AllFormGarbage; // not used Atoms, but not freed because of they are referred in other places
	static int AllFormFillTime;
	void NewBrigadeIsBorn(Brigade* B);
	static tpBirthCallback* lpBirthCallback;
	static int BirthCallback(int NewbornIndex);
	static AreaInfo** AreaMap; // не используется
	static int NA;
	static void CreateAreaMap(); // не используется
	static void UnLoading();
	static void ClearMainTaget(Atom* Deleted);
	static DWORD LastSerial;

	void Load(SaveBuf* SB);
	void Save(SaveBuf* SB);

	static word GetBrigadeNIndex(Brigade *B);
private:
	//void FillNeighbors();
	void FillEnmAng();
	void FillFriendAng();
	int FillNeighborsTime;
	void FillAllForm();
	static word GetRulesIndex(word UnitNIndex);
};
//====================================================================//
#endif // !defined(AFX_BRIGADEAI_H__1A25F3B9_A008_46AD_A880_B90797A576B7__INCLUDED_)
#endif //COSSACKS2
