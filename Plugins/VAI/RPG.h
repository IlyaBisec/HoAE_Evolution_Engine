
#pragma once

///////////////////////////////////////////////////////////////////////////////

class RPG_AI_Condition: public BaseClass {
public:
};

//class RPG_AI_Condition: public BaseClass {
//public:
//};

class aic_Block_And: public RPG_AI_Condition {
public:
	SAVE(aic_Block_And){
		REG_PARENT(RPG_AI_Condition);
	}ENDSAVE;
};

class aic_Block_Or: public RPG_AI_Condition {
public:
	SAVE(aic_Block_Or){
		REG_PARENT(RPG_AI_Condition);
	}ENDSAVE;
};

// Boolean expression
class expr_Boolean: public BaseClass {
public:
	SAVE(expr_Boolean){
		//REG_PARENT();
	}ENDSAVE;
};

// Numerical expression
class expr_Numerical: public BaseClass {
public:
	SAVE(expr_Numerical){
		//REG_PARENT();
	}ENDSAVE;
};

// Hero section

///////////////////////////////////////////////////////////////////////////////

//class RPG_AI_Action: public BaseClass {
//public:
//	ClassArray<RPG_AI_Condition> Conditions;
//};

class AI_Set_Condition: public BaseClass {
public:
	SAVE(AI_Set_Condition){
		//REG_PARENT();
	}ENDSAVE;
};

class RPG_AI_Set: public BaseClass {
public:
	SAVE(RPG_AI_Set){
		//REG_PARENT();
	}ENDSAVE;
};

//class RPG_AI_Set: public BaseClass {
//public:
//	SAVE(RPG_AI_Set){
//		//REG_PARENT();
//	}ENDSAVE;
//};

//class RPG_AI_Clothes: public RPG_AI_Set {
//public:
//	SAVE(RPG_AI_Clothes){
//		REG_PARENT(RPG_AI_Set);
//	}ENDSAVE;
//};

class RPG_AI_Editor: public BaseClass {
public:
	//RPG_AI_Action MainAction;

	RPG_AI_Set Cards;
	RPG_AI_Set Clothes;
	RPG_AI_Set Bottles;

	SAVE(RPG_AI_Editor){
		//REG_AUTO(MainAction);
		REG_AUTO(Cards);
		REG_AUTO(Clothes);
		REG_AUTO(Bottles);
	}ENDSAVE;
};

// ������ ������� �� ��������� ����������������� ������ � ���������� ���� ������
// ������������� ���� ������ ������������ �������

//
class ai_HeroArmyInfo {
public:
	byte NI;
	int Force;
	int Life;
	int Damage[4];
};
class ai_HeroBaseInfo {
public:
	byte NI;
	int Force;
	int Life;
	int Damage[4];
	int Mines[6];
};
class ai_HeroMiner {
public:
	int GroupID;
	int BaseID;

	int NPeasants;
	int Force;
};
class HeroVariableStorage;

class ai_HeroBase : public BaseClass {
public:
	ClassRef<UnitAbility> peonUA;
	bool peonInit;

	SAVE(ai_HeroBase){
	}ENDSAVE;

	virtual void Process(HeroVariableStorage* HVS) {}
};

class ai_Hero : public ai_HeroBase {
public:
	//ai_Hero(){
	//}

	int HomeArea;

	//int Fear[256];
	int DangerTime;
	int SendTime;

	int CastTime;
	int BottleTime;

	bool upLife;

	DynArray<int> Danger;
	word ArmyMap[8192];

	DynArray<word> TopList;					// ������� �� ������� �����
	DynArray<ai_HeroArmyInfo> TopArmies;	// ������ �����

	int hForce;

	int LifePrev;							// ����� �� ��������� �����, ���� ������� ����� ���������

	int MoveArea;							// �������� ��� ���������
	bool Retreat;							// ��������� ������ ��������� � MoveArea ��� ������ ��� �����

	int MultiplayerForCriping;				// �������� ���������� ����, ����� ��������� ����������

	//////////////////////////////////////////////////////////////////////// vs Enemy

	int EnemyPeasantNIndex;
	int EnemyMineNIndex;

	int EnemyArmyForce;
	int EnemyBaseForce;
	//int NetralForce;
	int EnemyHomeBaseID;
	//int EnemyHomePeasant;

	int MyArmyForce;
	int MyBaseForce;
	int MyHomeBaseID;

	DynArray<ai_HeroBaseInfo> AllBases;		// ������ ���� ��������� ��� � ������� � ������������ ��� ��� ����

	DynArray<ai_HeroMiner> Miners;			// ������� ����������� ����
	DynArray<int> MinersBaseMap;			// ���� ��� �������

	// � ���� ����� ���������?
	// ���� ����� ��������?

	////////////////////////////////////////////////////////////////////////

	SAVE(ai_Hero){
		REG_PARENT(ai_HeroBase);
	}ENDSAVE;

	virtual void Process(HeroVariableStorage* HVS);
	void CreateDangerMap(OneObject* Hero/*, int ItrAmount*/);

	int CollectArmy(int BaseArea, word* ArmyMap, int ArmyID);
};

class AI_Obj_SetPoint: public BaseClass {
public:
	ClassRef<UnitAbility> Ability;
	int Priority;
	SAVE(AI_Obj_SetPoint){
		//REG_PARENT(ai_HeroBase);
		REG_AUTO(Ability);
		REG_MEMBER(_int,Priority);
	}ENDSAVE;
	AI_Obj_SetPoint(){}
	//AI_Obj_SetPoint(UnitAbility* UA,int Value){ ChangePriority(UA,Value); }
	AI_Obj_SetPoint(UnitAbility* UA){ Ability.Set(UA); }
	void ChangePriority(/*UnitAbility* UA,*/int Value){
		Priority+=Value;
	}
};

class AI_Obj_Set: public ClonesArray<AI_Obj_SetPoint> {
public:
	SAVE(AI_Obj_Set){
		REG_PARENT(ClonesArray<AI_Obj_SetPoint>);
	}ENDSAVE;
	void Change(UnitAbility* UA,int Value);
	void Sort();
	int Get(ActiveUnitAbility* AA);
};







enum ObjState { 
	osGiveBonus,
	osGiveChest,
	osBuySettlement,
	osCasting,
	osCreeping,
	osFighting,
	osEscaping,
	osShopping
};

class RPG_AI_Object;

class RPG_AI_ObjectState: public BaseClass {
public:
	int Priority;
	ObjState State;
	SAVE(RPG_AI_ObjectState){
		REG_MEMBER(_int,Priority);
	}ENDSAVE;
	virtual bool Process(RPG_AI_Object* Obj){ return false; }
	virtual void ShowDebug(int x,int y){}
};

class AOS_GiveBonus: public RPG_AI_ObjectState {
public:
	SAVE(AOS_GiveBonus){
		REG_PARENT(RPG_AI_ObjectState);
	}ENDSAVE;
	virtual bool Process(RPG_AI_Object* Obj);
};

class AOS_GiveChest: public RPG_AI_ObjectState {
public:
	SAVE(AOS_GiveChest){
		REG_PARENT(RPG_AI_ObjectState);
	}ENDSAVE;
	virtual bool Process(RPG_AI_Object* Obj);
};

class AOS_BuySettlement: public RPG_AI_ObjectState {
public:
	SAVE(AOS_BuySettlement){
		REG_PARENT(RPG_AI_ObjectState);
	}ENDSAVE;
	virtual bool Process(RPG_AI_Object* Obj);
};

class AOS_Casting: public RPG_AI_ObjectState {
public:
	SAVE(AOS_Casting){
		REG_PARENT(RPG_AI_ObjectState);
	}ENDSAVE;
	virtual bool Process(RPG_AI_Object* Obj);
};

class AOS_Creeping: public RPG_AI_ObjectState {
public:
	SAVE(AOS_Creeping){
		REG_PARENT(RPG_AI_ObjectState);
	}ENDSAVE;
	virtual bool Process(RPG_AI_Object* Obj);
};

class AOS_Fighting: public RPG_AI_ObjectState {
public:
	SAVE(AOS_Fighting){
		REG_PARENT(RPG_AI_ObjectState);
	}ENDSAVE;
	virtual bool Process(RPG_AI_Object* Obj);
};

class AOS_Escaping: public RPG_AI_ObjectState {
public:
	SAVE(AOS_Escaping){
		REG_PARENT(RPG_AI_ObjectState);
	}ENDSAVE;
	virtual bool Process(RPG_AI_Object* Obj);
};

class AOS_Shopping: public RPG_AI_ObjectState {
public:
	SAVE(AOS_Shopping){
		REG_PARENT(RPG_AI_ObjectState);
	}ENDSAVE;
	virtual bool Process(RPG_AI_Object* Obj);
};







class RPG_AI_Object: public BaseClass/*public pUnit, public ai_Hero*/ {
public:
	pUnit Unit;
	ai_Hero AI;

	bool RPG;

	// ������ ��������� RPG ���������
	//
	// - ��������� ��������
	// - ������� ������� ��� �������������� ��������
	// - ��������� ����
	// - ������� ������� ��� �������������� ����
	// - ������� ����
	// - ��������� ����
	// - ������� ��������
	// - ��������� ��������
	// - ���������� �� ����������: �����, ��������, ������� � �.�.
	// - ���������� � ������ ��������� ��: �����, ��������, ������� � �.�.
	//

	AI_Obj_Set Cards;
	AI_Obj_Set Clothes;
	AI_Obj_Set Bottles;

	ClassArray<RPG_AI_ObjectState> State;
	void AddStatePrio(ObjState OS,int Prio);
	void StateProcess();
	void StateScript();

	SAVE(RPG_AI_Object){
		REG_PARENT(pUnitsList);
		REG_AUTO(Unit);
		REG_AUTO(AI);
		REG_AUTO(Cards);
		REG_AUTO(Clothes);
		REG_AUTO(Bottles);
		REG_AUTO(State);
	}ENDSAVE;

	virtual void Process(/*HeroVariableStorage* HVS*/);

	virtual void ScriptCards();
	virtual void ScriptClothes();
	virtual void ScriptBottles();

	//
	// ������ ��������� �������� RPG ���������
	//
	// move - ��� ����������� �������� ���������� ��������������� ����������� �� �������� ����
	//

	void GiveBonus(); // - ��������� ������ (move)
	void GiveChest(); // - �������� �������� (move)

	void BuySettlement(); // - ������� ��� (move)

	void Casting();		// - ��������� �������� (move)
	void Creeping();	// - ������� - ����� ����������� ������ (move)
	void Fighting();	// - �������� � ��� � ����������� ��� ������� ������� �� ���� (move)
	void Escaping();	// - ��������� ��� ���������� ���������� (move)

	void Shopping();	// - �������: �������, ��������, �������� (move)
	// - �������: �������, ��������, �������� (move)

	void Dressing();	// - �����/����� ������� ()
	void Drinking();	// - ������ ������� ()

	void Reflexion();	// - ��������/��������� ���� ()

	void ActivateAbility(); // - �������� ��� ��� ���������� ����������� ()
	void ActivateSkill(); // - ����������� ��� ����� ()

	// - ��������� - ��������� ����������� ������, ����� ���� �� ������������� (move)
	// - ������� �� �������� - ��������� ���������� ����� �� ����� ��� �������� (move)

};

class RPG_AI_Group: public ClonesArray<RPG_AI_Object> {
public:	
	SAVE(RPG_AI_Group){
		REG_PARENT(ClonesArray<RPG_AI_Object>);
	}ENDSAVE;
	void Process(/*HeroVariableStorage* HVS*/);
};





class RPG_Unit: public BaseClass {
public:
	SAVE(RPG_Unit){
	}ENDSAVE;
};

class RPG_U_Bonus: public RPG_Unit {
public:
	// actions
	// ��������������
	// ��������� �����������
	SAVE(RPG_U_Bonus){
		REG_PARENT(RPG_Unit);
	}ENDSAVE;
};

class RPG_U_Chest: public RPG_Unit {
public:
	// actions
	// ���������
	SAVE(RPG_U_Chest){
		REG_PARENT(RPG_Unit);
	}ENDSAVE;
};

class RPG_U_Settlement: public RPG_Unit {
public:
	// actions
	// ��������� ������
	// ���������� ������
	// ��������� � ������� ���� ����������� ������
	// ������
	SAVE(RPG_U_Settlement){
		REG_PARENT(RPG_Unit);
	}ENDSAVE;
};

class RPG_U_Shop: public RPG_Unit {
public:
	// actions
	// ����� � �������
	// ����� �� ��������
	// ������
	// �������
	SAVE(RPG_U_Shop){
		REG_PARENT(RPG_Unit);
	}ENDSAVE;
};

class RPG_U_Creep: public RPG_Unit {
public:
	pUnitsList Units;
	LinearArray<int,_int> Tops;
	bool Agressive;
	int Force;
	SAVE(RPG_U_Creep){
		REG_PARENT(RPG_Unit);
		REG_AUTO(Units);
		REG_AUTO(Tops);
		REG_MEMBER(_bool,Agressive);
		REG_MEMBER(_int,Force);
	}ENDSAVE;
	// actions
	// ��������� - ����������
	// ��������� � ������� ���� ����������� - ������������ ������ (��������� ����������)
};

class RPG_U_Friend: public RPG_Unit {
public:
	// ������, ������� � ����������, ����� �������
	// ��������� ������, ����������, �������� �������
	SAVE(RPG_U_Friend){
		REG_PARENT(RPG_Unit);
	}ENDSAVE;
};

class RPG_U_Enemy: public RPG_Unit {
public:
	// �������� � �����
	// ����� �� �����
	// ���������, ������ �� ������������ ��������� �� �����, ������������ ��� ����������
	SAVE(RPG_U_Enemy){
		REG_PARENT(RPG_Unit);
	}ENDSAVE;
};

class RPG_U_Battle: public RPG_Unit {
public:
	// �������� � �����
	// ����� �� �����
	SAVE(RPG_U_Battle){
		REG_PARENT(RPG_Unit);
	}ENDSAVE;
};

class RPG_AI: public BaseClass {
public:
	RPG_AI_Editor Editor;
	RPG_AI_Group Group;

	//
	// ������ ��������
	//
	bool Init;
	ClonesArray<RPG_U_Bonus> Bonuses; // - ������� (bonus)
	ClonesArray<RPG_U_Chest> Chests; // - ������ (chest)
	ClonesArray<RPG_U_Settlement> Settlements; // - ���� (magazine)
	ClonesArray<RPG_U_Shop> Shops;	// - ������� ������� (magazine)
									// - ������� ����� (magazine)
									// - ������� �������� (magazine)
	DynArray<word> CreepIndex;
	ClonesArray<RPG_U_Creep> Creeps; // - ����� (creep)
	ClonesArray<RPG_U_Friend> Friends; // - ��������
	ClonesArray<RPG_U_Enemy> Enemies; // - �����
	ClonesArray<RPG_U_Battle> Battles; // - ����� (battle) ����� ��������� ��� � ������� ���������� ��������
	// - �������� ���������� ����� (curse) ���������, �����������, �����������
	// - �������� ���������� ����� (bless) �������, ����������, �����������
	// 

	SAVE(RPG_AI){
		REG_AUTO(Editor);
		REG_AUTO(Group);
		REG_MEMBER(_bool,Init);
		REG_AUTO(Bonuses);
		REG_AUTO(Chests);
		REG_AUTO(Settlements);
		REG_AUTO(Shops);
		REG_AUTO(Creeps);
		REG_AUTO(Friends);
		REG_AUTO(Enemies);
		REG_AUTO(Battles);
	}ENDSAVE;
	void Process();
	void OnAIStarted(byte NI);
	void AddUnit(/*OneObject* OB*/pUnit& U);
	void Unload();
};
extern RPG_AI RPG;

///////////////////////////////////////////////////////////////////////////////////////////////

#define CIMPORT __declspec(dllimport)
#define DLLIMPORT extern "C" __declspec(dllimport)

//
//DLLEXPORT int GetTopZone(int x,int y);
//DLLEXPORT int GetRND(int Max);
DLLIMPORT int FindNextZoneOnTheSafeWayEx(int Start,int Fin,int* DangerMap,int* MaxDanger,int DangSteps);
DLLIMPORT int FindNextZoneOnTheSafeWayToObjectEx(int Start,int* DangerMap,word* ObjIds,int* MaxDanger,int DangSteps,word* DestObj);
DLLIMPORT bool GetTopZRealCoor(int Zone,int* x,int* y);
DLLIMPORT int GetTopDist(int xa,int ya,int xb,int yb);

class WeaponParams;
//ActiveUnitAbility* AddOneEffect(WeaponParams* WP, UnitAbility* UA, OneObject* Dest, OneObject* Caster);
//bool CheckInfluenceMask(ActiveUnitAbility* V, DWORD Mask);
//void SmoothDangerMap(short* DangMap, int ItrAmount);

DLLIMPORT int GetRND(int Max);