#pragma once

#ifndef __UNITABILITY_H__
#define __UNITABILITY_H__

/////////// FROM BE ///////////  
//#include "BE_HEADERS.h"
//#include "Parameter.h"
//#include "Function.h"
//#include "ComplexFunction.h"
//#include "ParamFunction.h"

#include "VariableBase.h"

///////////////////////////////  
#include "bitset.h"

#define ColorPositive "{C FF10EE10}";
#define ColorNegative "{C FFEE1010}";
#define ColorNeutral "{C FF3030EE}";

enum enActionType { atRise, atDecrease, atNeutral };

class vOperand: public BaseClass{
public:		
	int sign; //0-"+", 1-"-", 2-"*", 3-"/", 4-"set"
	UserFriendlyNumericalReturner num;
	int numType;
	int percentType;
	int MinValue;

	__constructor(vOperand){
		MinValue=-10000;
	}

	SAVE(vOperand){
		REG_ENUM_EX(_index,sign,ARIPHMETICALOPS,Operator);
		REG_AUTO_EX(num,Operand);
		REG_ENUM_EX(_index,numType,OperandValueType,OperandType);
		SAVE_SECTION(2);
		REG_ENUM_EX(_index,percentType,OperandPercentType,Percent);
		SAVE_SECTION(0xFFFFFFFF);
		REG_MEMBER(_int,MinValue);
	}ENDSAVE;

	DWORD GetClassMask(){
		DWORD M=1;
		if(numType==1){
			M=2;
		}
		return M;
	}

	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags);
	enActionType GetActionType(bool Reverse=false);

	template <class X> void Calculate(X Base, X& Value, bool Reverse=false);
	template <class X> void CalculateRatio(X Base, X& Value, X &Relative, bool Reverse=false);
};

void StopUpg(byte Nat);
typedef bitset<2> uaModMask;

//============ Modificators ===================
//#include "ua_Modificators.h"
class UnitModificatorCondition: public  ReferableBaseClass{
public:
	bool Not;

	SAVE(UnitModificatorCondition){
		REG_MEMBER(_bool,Not);
	}ENDSAVE;	

	virtual bool Check (ActiveUnitAbility* AA) { return true; }
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(Not) dest="!"; else dest=""; return true;
	}
};
class UnitModificatorConditionArray: public  ClassArray<UnitModificatorCondition>{
public:
	SAVE(UnitModificatorConditionArray){
	}ENDSAVE;	

	inline bool Check (ActiveUnitAbility* AA);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags);
	//virtual bool ForceSimplification() { return true; }
};
class UnitModificator: public  ReferableBaseClass {
public:
	static DWORD CurSerial;
	DWORD Serial;

	DWORD InfluenceMask;
	UnitModificatorConditionArray Conditions;

	UnitModificator(){
		InfluenceMask=0xFFFF;
		Serial=CurSerial++;
	}
	SAVE(UnitModificator){
		REG_AUTO(Conditions);
	}ENDSAVE;

	virtual bool Modify (ActiveUnitAbility* AA) { return true; }
	virtual enActionType GetActionType() { return atNeutral; }
};
class UnitModificatorArray: public  ClassArray<UnitModificator> {
public:
	DynArray<uaModMask> InfluenceMasks;

	SAVE(UnitModificatorArray){
	}ENDSAVE;	

	int Add(UnitModificator* V);
	inline void Del(int pos,int N);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags);
	//virtual bool ForceSimplification() { return true; }
};
class ModifySprite: public BaseClass {
public:

	UnitModificatorConditionArray Condition;
	short FileID;
	int SpriteID;

	SAVE(ModifySprite){
		REG_AUTO(Condition);
		REG_MEMBER(_gpfile,FileID);
		REG_SPRITE(SpriteID,FileID);
	}ENDSAVE;

};

//
class OneObject;
//==================================================================================================================//
#include "SuperEditor.h"
#include "ua_State.h"
//class UserFriendlyNumericalReturner;
class HeroVariable;
class HeroVariableStorage;
class ActiveUnitAbility;

class MoveSpeedCast;
class ShieldCast;
class ShieldCast;
class AttSpeedCast;

class UnitAbilityIcon;
class VisualEffectArray;
class MagicSpell:public BaseClass
{
public:
	MagicSpell();
	//	int operand; //0-Life, 1-AttSpeed, 2-MoveSpeed, 3-Shild
	int sign; //0-"+", 1-"-", 2-"*", 3-"/"
	int num;
	char* name;

	SAVE(MagicSpell)
		//		REG_MEMBER(_int,operand);
		REG_ENUM(_index,sign,ARIPHMETICALOPS);
	REG_MEMBER(_int,num);
	ENDSAVE;

	virtual int CastSpell(OneObject* OB);
	virtual int CancelSpell(OneObject* OB);

	int realchng;
};
//============ UnitAbility ===================
class be_CVariableBase;
class DIALOGS_API UnitAbility : public be_CVariableBase {
public:
	
	void reset_class(void* p) { ((UnitAbility*)p)->Init(); }
	UnitAbility() { Init(); }
	void Init();

	~UnitAbility();
	
	int Index;
	int ClassType; // Image, Effect
	int Type; // ItemBottle, ItemClothe, Bonus, Magazine, MagicCard, Aura, Ability, AntiMageSkill=7
	
	int ItemDrop;
	int ShopPrice;
	float SellMultiplier;
	bool EnableCustomReward;
	int  FoodReward;
	int  WoodReward;
	int  StoneReward;
	int  IronReward;
	int  GoldReward;
	int  CoalReward;
	int  RandomFoodReward;
	int  RandomWoodReward;
	int  RandomStoneReward;
	int  RandomIronReward;
	int  RandomGoldReward;
	int  RandomCoalReward;

	////////////////// States ///////////////////////////
	BaseClassUnitType GroundMD;
	bool PickUp;
	bool DropDown;
	bool Buy;
	bool Sell;
	ClonesArray<ua_State> States;	
	bool System;

	////////////////// NewAbility ///////////////////////////

	bool MoveToEndAfterUse;
	bool Consumable;
	bool Extra;
	bool SellAfterUse;

	bool Permanent; // не отображается, переходят из миссии в миссию
	bool Button; // может ли юзер нажать на ability
	bool Aura; // навешивать еффект самой ability или создавать eWeapon
	int AuraCastPeriod; // время между генерацией импулсов ауры

	int TargetMaxAmount;
	
	ClonePtr<NewAnimation> eAura; // эффект над героем излучающим ауру
	bool DisableAnotherAuras;
	bool AutoCast; // только если кнопка, возможность вкл/выкл автоиспользования с правилами
	
	//int PrepareDuration; // length prepare casting

	ClonePtr<NewAnimation> ePrepareCasting; // before start casting play animation
	ClonePtr<NewAnimation> eDirectCasting; // hero busy when casting
	bool PrepareCasting;
	bool DirectCasting;
	UserFriendlyNumericalReturner CastingShotAmount; // amount of shots
	int CastingShotDuration; // length of casting one effect weapon
	int CastingOrderType;		// тип приказа, 1 - не прерывать предидущий приказ

	bool UnlimitedAmount;
	int Amount;

	UserFriendlyNumericalReturner EffectDuration;
	bool OnDieDeleteEffect;

	/////////////////////////////////////////////////////////////

	// State
	UserFriendlyNumericalReturner CoolDownDuration; // перезарядка
	int ManaValue;	// стоимость маны
	int MoneyValue;	// стоимость по деньгам

	// SubState
	bool AntiMageNotBurning;
	bool AntiMageSupportSkill;	
	//int AntiMageSkillName;
	ClonesArray< ClassRef<UnitAbility> > SupportSkills;
	ClonesArray< ClassRef<UnitAbility> > NecessarySkills; // for enable button
	bool CanNotBeClothe;
	int TypeClothe;
	bool BreakInvisibility;
	bool DontBreakInvis;

	// Process and order
	bool MultyCast;
	int MaxCasts;
	bool IgnoreMagicImmunity;
    bool NonIgnoredSkill;
	bool ImpossibleToFall;
	ClonesArray< ClassRef<UnitAbility> > Blocking;
	ClonesArray< ClassRef<UnitAbility> > UnCast;
	bool Teleport;
	bool Direction;	
	UserFriendlyNumericalReturner Damage;
	int DamageShots;
	int DamageDelay;
	UserFriendlyNumericalReturner Heal;
	int HealShots;
	int HealDelay;
	// Modificators
	UserFriendlyNumericalReturner LongTime; // длительность действия эффекта
	
	UnitModificatorConditionArray Conditions;
	UnitModificatorArray Modificators;
	VisualEffectArray VisualEffects;
	
	// Target selector
	bool EnemyTarget;
	bool FriendlyTarget;

	bool LiveBody;
	bool DeadBody;
	bool Bones;

	bool BuildingTarget;
	bool OverWater;
	byte BodyStyleTarget;
	bool SummonTarget;

	UserFriendlyNumericalReturner Radius; // радиус зоны поиска, пример - (Hero:Level+1)*100
	
	// ИИ
	int SpellDangerTime;		// время действия урона
	int SpellDanger;			// количество урона, которое может нанести этот спел
	int SpellAddRadius;			// дополнительный радиус спела, может быть отрицательным
	//
	
	DWORD CursColor;
	int CursType;

	UserFriendlyNumericalReturner Distance; // макс дальность кастования
	UserFriendlyNumericalReturner NoCastIfOutOfDistance;
	// Animation and Effects
	ClonePtr<NewAnimation> eAn;
	int eAnDuration;
	// User interface
	bool Visible;

	ClassArray<ModifySprite> Sprite;
	
	short FileID;
	int SpriteID;
	short FileID_Char;
	short FileID_Inv;
	short EffectFileID;
	int EffectSpriteID;
	char* HintName;
	char* Hint;
	ClassRef<UnitAbility> HintUA;
	int SoundExecute;

	// SubSection
	SubSection State;
	SubSection SubState;
	SubSection UserInterface;
	SubSection Process;
	SubSection Modificator;
	SubSection TargetSelector;
	SubSection Effects;

	//bool Summon;
	//BaseClassUnitType ChildType;
	//int ChildAmount;
	ClassArray <MagicSpell> MSCast;
	//int AddMaxLife;
	
	//int WeaponShots;
	ClassRef<WeaponModificator> eWeapon;
	ClassRef<UnitAbility> eAbility;
	ClassRef<UnitAbility> eBeforeCast;

	ClassRef<WeaponModificator> OnDieCreateWeapon;
	ClassRef<WeaponModificator> OnEndEffectCreateWeapon;
	int ImpulseCreateWeaponPeriod; // ms
	ClassRef<WeaponModificator> OnImpulseCreateWeapon;

	bool OnEndEffectDie;
	bool OnEndEffectErase;

	bool EndEffectIfEnemyNear;
	int EndEffectIfEnemyNearStartTime;
	int EndEffectIfEnemyNearRadius;

	bool HeroLife;

	int FirstDirectSoundID;	

	Delimiter D;

	DWORD GetClassMask();

	SAVE(UnitAbility){
		REG_BASE(be_CVariableBase);
		
		//READONLY REG_MEMBER(_int,Index); // устанавливается при загрузке в AbilityList::LoadAbilities

		REG_MEMBER(_str,Name);
		REG_ENUM(_index,ClassType,UA_ClassType);

		REG_AUTO(D);
		SAVE_SECTION(1);
			//REG_MEMBER(_bool,PickUp);
			//REG_MEMBER(_bool,DropDown);
			//REG_MEMBER(_bool,Buy);
			//REG_MEMBER(_bool,Sell);
			//REG_AUTO(States);

		SAVE_SECTION(0xFFFFFFFF);
			//REG_AUTO(UserInterface);
			REG_MEMBER(_bool,Visible);

		REG_AUTO(Sprite);

		SAVE_SECTION(1);
			REG_MEMBER(_gpfile,FileID);
			REG_SPRITE(SpriteID,FileID);
			REG_MEMBER(_gpfile,FileID_Char);
			REG_MEMBER(_gpfile,FileID_Inv);

		SAVE_SECTION(2);
			REG_MEMBER(_gpfile,EffectFileID);
			REG_SPRITE(EffectSpriteID,EffectFileID);

		SAVE_SECTION(0xFFFFFFFF);
			REG_MEMBER(_textid,HintName);
			REG_MEMBER(_textid,Hint);
			REG_AUTO(HintUA);
			REG_ENUM(_index,SoundExecute,ALL_SOUNDS);

		REG_AUTO(D);
		SAVE_SECTION(1);
			REG_ENUM(_index,Type,UnitAbilityType);
			REG_MEMBER(_bool,System);
			REG_MEMBER(_bool,MoveToEndAfterUse);
			REG_MEMBER(_bool,Consumable);
			REG_MEMBER(_bool,SellAfterUse);
			REG_MEMBER(_int,ItemDrop);
			REG_AUTO(GroundMD);
			//REG_AUTO(State);
			REG_MEMBER(_int,ShopPrice);
			REG_MEMBER(_float,SellMultiplier);
		SAVE_SECTION(1);
			REG_MEMBER(_int,ManaValue);
			REG_MEMBER(_int,MoneyValue);
			REG_AUTO(CoolDownDuration); //REG_MEMBER(_int,CoolDownDuration);
			REG_MEMBER(_bool,Button);
			REG_MEMBER(_bool,UnlimitedAmount);
			REG_MEMBER(_int,Amount);
			REG_MEMBER(_bool,BreakInvisibility);
			REG_MEMBER(_bool,DontBreakInvis);
		SAVE_SECTION(3);
			REG_MEMBER(_bool,Aura);
			REG_MEMBER(_int,AuraCastPeriod);
			REG_MEMBER(_bool,DisableAnotherAuras);
		//SAVE_SECTION(1);
			REG_MEMBER(_bool,PrepareCasting);
			REG_MEMBER(_bool,DirectCasting);
			REG_AUTO(CastingShotAmount);
			REG_MEMBER(_int,CastingShotDuration);
			REG_MEMBER(_int,CastingOrderType);
			REG_ENUM(_index,FirstDirectSoundID,ALL_SOUNDS);

		SAVE_SECTION(0xFFFFFFFF);
		REG_AUTO(D);
		SAVE_SECTION(2);
			REG_MEMBER(_bool,AntiMageNotBurning);
			REG_MEMBER(_bool,AntiMageSupportSkill);
			//REG_ENUM(_index,AntiMageSkillName,AMS_Levels);
			REG_AUTO(NecessarySkills);
			REG_AUTO(SupportSkills);
		//SAVE_SECTION(2);
			REG_MEMBER(_bool,Permanent);
			REG_AUTO(EffectDuration);
			REG_MEMBER(_bool,MultyCast);
			REG_MEMBER(_int,MaxCasts);
			REG_MEMBER(_bool,IgnoreMagicImmunity);
			REG_MEMBER(_bool,NonIgnoredSkill);
			REG_MEMBER(_bool,ImpossibleToFall);

		SAVE_SECTION(0xFFFFFFFF);
			REG_AUTO(OnDieCreateWeapon);
		SAVE_SECTION(2);
			REG_MEMBER(_bool,OnDieDeleteEffect);
			
			REG_AUTO(OnEndEffectCreateWeapon);
			REG_MEMBER(_int,ImpulseCreateWeaponPeriod);
			REG_AUTO(OnImpulseCreateWeapon);

			REG_MEMBER(_bool,OnEndEffectDie);
			REG_MEMBER(_bool,OnEndEffectErase);

			REG_MEMBER(_bool,EndEffectIfEnemyNear);
			REG_MEMBER(_int,EndEffectIfEnemyNearStartTime);
			REG_MEMBER(_int,EndEffectIfEnemyNearRadius);

			REG_AUTO(Blocking);
			REG_AUTO(UnCast);

			REG_MEMBER(_bool,Teleport);
			REG_MEMBER(_bool,Direction);

			REG_MEMBER(_bool,HeroLife);
		
		SAVE_SECTION(0xFFFFFFFF);
			REG_MEMBER(_int,SpellDangerTime);
			REG_MEMBER(_int,SpellDanger);
			REG_MEMBER(_int,SpellAddRadius);

		REG_AUTO(D);
			REG_AUTO(Conditions);
			REG_AUTO(Modificators);
			REG_AUTO(VisualEffects);

		SAVE_SECTION(1);
		REG_AUTO(D);
			REG_AUTO(eWeapon);
		SAVE_SECTION(4);
			REG_AUTO(eAbility);
			REG_AUTO(eBeforeCast);

		//REG_AUTO(D); //REG_AUTO(Effects);
			//REG_AUTO(eAn);
			//REG_MEMBER(_int,eAnDuration);
			//REG_AUTO(eAura);
			//REG_AUTO(ePrepareCasting);
			//REG_AUTO(eDirectCasting);

		SAVE_SECTION(0xFFFFFFFF);
		REG_AUTO(D); //REG_AUTO(TargetSelector);

		SAVE_SECTION(1);
			REG_AUTO(Distance);
			REG_AUTO(NoCastIfOutOfDistance);
			REG_AUTO(Radius);

		// TargetSelector
		SAVE_SECTION(1); //4
			REG_MEMBER(_bool,EnemyTarget);
			REG_MEMBER(_bool,FriendlyTarget);
		SAVE_SECTION(4);
			REG_MEMBER(_bool,LiveBody);
			REG_MEMBER(_bool,DeadBody);
			REG_MEMBER(_bool,Bones);

			REG_MEMBER(_bool,BuildingTarget);
			REG_MEMBER(_bool,OverWater);
			REG_MEMBER(_BYTE,BodyStyleTarget);
			REG_MEMBER(_bool,SummonTarget);
			REG_MEMBER(_int,TargetMaxAmount);
		REG_AUTO(D);
			REG_MEMBER(_bool,EnableCustomReward);
			REG_MEMBER(_int,FoodReward);
			REG_MEMBER(_int,WoodReward);
			REG_MEMBER(_int,StoneReward);
			REG_MEMBER(_int,IronReward);
			REG_MEMBER(_int,GoldReward);
			REG_MEMBER(_int,CoalReward);
			REG_MEMBER(_int,RandomFoodReward);
			REG_MEMBER(_int,RandomWoodReward);
			REG_MEMBER(_int,RandomStoneReward);
			REG_MEMBER(_int,RandomIronReward);
			REG_MEMBER(_int,RandomGoldReward);
			REG_MEMBER(_int,RandomCoalReward);
		SAVE_SECTION(1);
			REG_ENUM(_index,CursType,TEXTURE_CURSOR_TYPES);
			REG_MEMBER(_color,CursColor);

	}ENDSAVE;

	virtual void OnAddToObject(OneObject* OB) {}
	virtual bool OnUnitBirth(OneObject* Newbie);
	virtual bool CheckIfObjectIsGlobal(){return true;}
	virtual ActiveUnitAbility* GetNewActiveAbility();
	virtual const char* GetThisElementView(const char* LocalName);

	// by vital
	virtual void CopyToActive(ActiveUnitAbility* ab);

	bool GetSprite(ActiveUnitAbility* AA, int& File, int& Spr);
}; 
//============================================
//#define ABL(x) (1<<x)
#define ABL(x) x

#define ABL_MotionSpeed      	ABL(0)
#define ABL_Protection       	ABL(1)
#define ABL_Shield           	ABL(2)
#define ABL_Damage           	ABL(3)
#define ABL_AttackRate       	ABL(4)
#define ABL_MagicImmunity    	ABL(5)
#define ABL_LifeRegeneration 	ABL(6)
#define ABL_Weapon			 	ABL(7)
#define ABL_Invisibility	 	ABL(8)
#define ABL_Summon			 	ABL(9)
#define ABL_Vampire				ABL(10)
#define ABL_AttackRadiusMax		ABL(11)
#define ABL_Silence				ABL(12) // тишина - невозможность кастовать магию
#define ABL_Compassion			ABL(13) // жалость - невозможность атаковать врагов
#define ABL_Invulnerability		ABL(14)
#define ABL_ManaValue			ABL(15)
#define ABL_Life				ABL(16)
#define ABL_Mana				ABL(17)
#define ABL_LifeMax				ABL(18)
#define ABL_ManaMax				ABL(19)
#define ABL_Material			ABL(20)
#define ABL_OverEarth			ABL(21)
#define ABL_Plymorph			ABL(22)
#define ABL_ManaRegeneration 	ABL(23)
#define ABL_Vision				ABL(24)
#define ABL_Fear				ABL(25)
#define ABL_AttackCaster		ABL(26)
#define ABL_ProduceEnable		ABL(27)
#define ABL_Scale				ABL(28)
#define ABL_Frozen			 	ABL(29)
#define ABL_Control			 	ABL(30)
#define ABL_Insubordination	 	ABL(31)

#define ABL_ModelAnimation	 	ABL(32)
#define ABL_LevelForEffects	 	ABL(33)
#define ABL_TradeSkill		 	ABL(34)
#define ABL_Learning		 	ABL(35)
#define ABL_WeaponKind	 		ABL(36)
#define ABL_Evasion				ABL(37)
#define ABL_Miss				ABL(38)
#define ABL_ExtraAnimation	 	ABL(39)
#define ABL_Thorn				ABL(40)
#define ABL_ItemPickUp			ABL(41)
#define ABL_ExtraAbility	    ABL(42)
#define ABL_SearchVictim		ABL(43)
#define ABL_Symbol				ABL(44)
#define ABL_MotionStyle			ABL(45)
#define ABL_LockType			ABL(46)
#define ABL_CenterScreen		ABL(47)

#define ABL_SpellOnAttack	ABL(48) //Modified by TheBlackHunter, Old name - SpellOnMakeDamage
#define ABL_DeleteAbilityOnReceiveDamage ABL(49)
#define ABL_KillMask			ABL(50)
#define ABL_DeleteAbilityAfterNShots ABL(51)
#define ABL_NMask				ABL(52)
//TheBlackHunter's modificators
#define ABL_SkillImmunity       ABL(53)
#define ABL_Alteration            ABL(54)
#define ABL_SkillSilence       ABL(55)
#define ABL_Death       ABL(56)
#define ABL_DeleteAbility       ABL(57)
#define ABL_DamReduct           ABL(58)
#define ABL_IgnoreProtection    ABL(59)


//typedef uaModMask bitset<2>;

//for Weapons and abilities - to calculate unit runtime values
class GlobalUnitsContext{
	static word CasterID;
	static word CasterSN;
	static word AcceptorID;
	static word AcceptorSN;
public:	
	static void Reset(){
		CasterID   = 0xFFFF;
		CasterSN   = 0xFFFF;
		AcceptorID = 0xFFFF;
		AcceptorSN = 0xFFFF;
	}
	inline static void setCaster(word ID,word SN){
		CasterID=ID;
		CasterSN=SN;
	}
	inline static void setAcceptor(word ID,word SN){
		AcceptorID=ID;
		AcceptorSN=SN;
	}
	static OneObject* getCaster();
	static OneObject* getAcceptor();
};

class AblName;

class DIALOGS_API ActiveUnitAbility : public BaseClass
{
public:
	int Tx,Ty,Tz;
	word tIndex, tSerial;

public:
	~ActiveUnitAbility(void);

	__constructor(ActiveUnitAbility){
		//Name="";
		//UnitIndex=0xFFFF;
		Type=0;
		TypeAbil=2;
		Visible=true;
		FileID=0;
		SpriteID=0;
		//Hint="";
		UseTime=0;
		IconInfo=NULL;
		//InfluenceMask=0;
		//
		WeaponShots=0;
		DamageShots=0;
		HealShots=0;
		//
		ParentNIndex=0xFFFF;

		BaseUA=NULL;

		CasterID=0xFFFF;
		EffectTime=-100000;

		LastImpulseWeaponTime=0;

		Slot=-1;
		TargetUnitType=0xFFFF;
	}

public:
	int eAuraFrame;
	int eAnStartTime;
	int eAuraStartTime;
	byte eHandler;

	_str Name;

	int Amount;

	//bool MoveToEnd;

	word TargetUnitType;

	int ExecuteTime; // время следующего активирования

	int EffectCount;

	int LastImpulseWeaponTime;

	word CasterID; // who cast effect to Object
	word CasterSN;
	int CasterLevel;

	int ItemDrop;

	DWORD Serial;
	static DWORD CurSerial;
	int BirthTime; // добавление в массив юнита	

	int LifeShot; // life modificators counter
	int ManaShot; // mana modificators counter
	int LastLifeShotTime;
	int LastManaShotTime;

	bool AutoCast; // включен режим автоиспользования

	bool Casting; // устанавливается Execute
	bool Prepared; // true если анимация подготовки отиграна

	int CastingShotsPrepared;

	int CastingTime;

	bool mdChange;
	DWORD mdSerial;
	word ParentNIndex;

	int EffectTime;
	int DieTime;

	bool Droped;
	bool Deleted;

	int Type;
	int TypeAbil; // -1 - None, 0 - Effect, 1 - Button, 2 - Passive, 3 - UpLevels, 4 - Item clothe, 5 - Item bottle, 6 - Hidden, 7 - Dialog	
	int UpdatesType; // 0 - netral, 1 - positive, 2 - negative, 3 - ...
	//
	bool Clothed;
	int Slot;
	//
	bool Visible; // true - use in interface, false - not use in interface
	short FileID;
	int SpriteID;
	_str Hint;

	OneObject* OB;

	int WeaponShots;
	int DamageShots;
	int DamageTime;
	int HealShots;
	int HealTime;

	byte InvisDetected;
	DWORD ParentAbilitySerial; // если эффект, то Serial родительской абилки

	int DeleteTime; // время самоуничтожения
	int DeleteShot; // если счетчик выстрелов превысит это значение, абилка удалиться

	NewAnimation* Eff;

	bool EnableStates; // States
	int CurState;
	int StateTime;

	bool trInit[64]; // Tracks
	byte trHandle[64];

	bool evLeftClick; // Event
	bool evRightClick;
	bool evTimelineIsDone;

	bool opDisappear; // Operation

	bool AreaSelected; // Area
	int arX;
	int arY;

	int toState;

	bool DeleteAfterUse;

	int Level;
	bool Opened; // for NecessarySkill

public: // BE scripts

	AblName* be_Dialog;

	char* GetAbilityName(){
		UnitAbility* UA=GetA();
		if(UA)
			return UA->Name.pchar();
		return NULL;
	}

	SAVE(ActiveUnitAbility){

		//REG_AUTO(UnitAbilityRef);
		//REG_AUTO(UnitAbilityPtr);

		//int eAuraFrame;
		//int eAnStartTime;
		REG_MEMBER(_int,eAuraStartTime);
		//byte eHandler;

		REG_MEMBER(_str,Name);

		REG_MEMBER(_int,Amount);
		//REG_MEMBER(_bool,MoveToEnd);
		REG_MEMBER(_int,ExecuteTime); // время следующего активирования

		REG_MEMBER(_int,EffectCount);
		REG_MEMBER(_int,LastImpulseWeaponTime);

		REG_MEMBER(_WORD,CasterID); // who cast effect to Object
		REG_MEMBER(_WORD,CasterSN);
		REG_MEMBER(_int,CasterLevel);

		REG_MEMBER(_int,ItemDrop);

		REG_MEMBER(_DWORD,Serial);
		REG_MEMBER(_int,BirthTime); // добавление в массив юнита	

		REG_MEMBER(_int,LifeShot); // life modificators counter
		REG_MEMBER(_int,ManaShot); // mana modificators counter
		REG_MEMBER(_int,LastLifeShotTime);
		REG_MEMBER(_int,LastManaShotTime);

		//int WeaponShots;
		//int DamageShots;
		//int DamageTime;
		//int HealShots;
		//int HealTime;

		REG_MEMBER(_bool,AutoCast); // включен режим автоиспользования

		REG_MEMBER(_bool,Casting); // устанавливается Execute
		REG_MEMBER(_bool,Prepared); // true если анимация подготовки отиграна

		REG_MEMBER(_int,CastingShotsPrepared);
		REG_MEMBER(_int,CastingTime);

		//bool mdChange;
		//DWORD mdSerial;
		//word ParentNIndex;

		REG_MEMBER(_int,EffectTime);
		//int DieTime;

		REG_MEMBER(_bool,Droped);
		REG_MEMBER(_bool,Deleted);

		REG_MEMBER(_int,Type);
		REG_MEMBER(_int,TypeAbil);

		//int UpdatesType; // 0 - netral, 1 - positive, 2 - negative, 3 - ...

		REG_MEMBER(_bool,Clothed);
		REG_MEMBER(_int,Slot);

		REG_MEMBER(_bool,Visible);
		REG_MEMBER(_gpfile,FileID);
		REG_MEMBER(_int,SpriteID);
		REG_MEMBER(_str,Hint);

		//OneObject* OB;

		REG_MEMBER(_BYTE,InvisDetected);

		//DWORD ParentAbilitySerial; // если эффект, то Serial родительской абилки
		REG_MEMBER(_int,DeleteTime); // время самоуничтожения
		REG_MEMBER(_int,DeleteShot);
		//
		//NewAnimation* Eff;

		//bool EnableStates; // States
		//int CurState;
		//int StateTime;
		//
		//bool trInit[64]; // Tracks
		//byte trHandle[64];
		//
		//bool evLeftClick; // Event
		//bool evRightClick;
		//bool evTimelineIsDone;
		//
		//bool opDisappear; // Operation
		//
		REG_MEMBER(_bool,AreaSelected); // Area
		REG_MEMBER(_int,arX);
		REG_MEMBER(_int,arY);
		//
		//int toState;

		REG_MEMBER(_bool,DeleteAfterUse);
		REG_MEMBER(_int,Level);
		REG_MEMBER(_bool,Opened);
	}ENDSAVE;

	//-----Influence of unit ability on different unit parameters-----//
	uaModMask InfluenceMask;//contains combination of ABL_... flags. 
	//If flag is set, then this ability can ifluence on this type of action	
	//constructor should FILL this field.
	//WARNING! every of the next functions can change properties of OneObject-s
	//only if it accepts OneObject* p and p!=NULL. if p==NULL it mostly means
	//that interface system requires some information, so sync can fail if
	//OneObject-s properties will change
	
	virtual void modifyMotionSpeed       (int BasicSpeed,int& CurrentSpeed,bool& Stun);
	virtual void modifyProtection        (int AttType,int BasicProtection,int& CurrentProtection,OneObject* Damager);
	virtual void modifyShield            (int BasicShield,int& CurrentShield,OneObject* Damager);
	virtual void modifyDamage            (int AttType,int BasicDamage,int& CurrentDamage,OneObject* Victim);
	virtual void modifyAttackRate        (int AttackType,int BasicRate,int& ChangedRate,OneObject* Victim);
	virtual void modifyMagicImmunity     (bool Basic,bool& Current,OneObject* Caster);
	virtual void modifySkillImmunity     (bool Basic,bool& Current,OneObject* Caster);
	virtual void modifyLifeRegeneration  (int Basic,int& Current); //HP per 100 sec
	virtual void modifyWeapon			 (Weapon* Basic,Weapon** Current,bool Action);
	virtual void modifyInvisibility		 (bool Basic,bool& Current,byte& Detected,bool& EnableSearchVictim);
	//virtual void modifyDamagerLife		 (word Basic,word& Current,int Damage,OneObject* Damager,OneObject* Victim);
	virtual void modifyDamReduct	     (int Basic, int& Current,int ReductType);
	virtual void modifyIgnoreProtection	 (int Basic, int& Current);
	virtual void modifyVampire			 (word Basic,word& Current,int Damage,OneObject* Damager,OneObject* Victim);
	virtual void modifyThorn			 (word Basic,word& Current,int Damage,OneObject* Damager,OneObject* Victim,byte AttType);
	virtual void modifyAlteration		 (int Basic,int& Current,OneObject* Target);
	virtual void modifyAttackRadiusMax	 (int Basic,int& Current);
	
	virtual void modifyManaValue		 (int Basic,int& Current);
	virtual void modifyLife				 (word Basic,word& Current);
	virtual void modifyMana				 (word Basic,word& Current);
	virtual void modifyLifeMax			 (word Basic,word& Current);
	virtual void modifyManaMax			 (word Basic,word& Current);
	virtual void modifyMaterial			 (byte Basic, byte& Current);

	virtual void modifyManaRegeneration	 (int Basic,int& Current);
	virtual void modifyVision			 (byte Basic, byte& Current);
	virtual void modifyProduceEnable	 (bool Basic, bool& Current);
	virtual void modifyScale			 (word Basic, word& Current);

	virtual void modifyInsubordination	(bool Basic,bool& Current);

	virtual void modifyModelAnimation	(bool Basic,bool& Current);

	virtual void modifyLevelForEffects	(int Basic, int& Current);
	virtual void modifyTradeSkill		(int Basic, int& Current);
	virtual void modifyNMask			(byte Basic, byte& Current);
	virtual void modifyLearning			(int Basic, int& Current);
	virtual void modifyWeaponKind		(int Basic, int& Current, int AttType);
	virtual void modifyEvasion			(int Basic, int& Current);
	virtual void modifyMiss				(int Basic, int& Current);
	virtual void modifyExtraAnimation	(int Basic, int& Current);
	virtual void modifyExtraAbility			(OneObject* NewUnit);
	virtual void modifyDeleteAbility			(OneObject* NewUnit);

	virtual void modifySearchVictim		(OneObject*& Victim, int Radius, int& NShots);

	virtual void modifyMotionStyle		(byte Basic, byte& Current);
	virtual void modifyLockType			(byte Basic, byte& Current);

	inline bool isSilence();
	inline bool isSkillSilence();
	inline bool isDeath();
	inline bool isInvulnerability();
	inline bool isCompassion();
	inline bool isSummon();
	inline bool isFrozen();
	inline bool isCenterScreen();

	virtual void execSpellOnAttack(OneObject* Enemy, int ActType, int TargetType);
	virtual void execDeleteAbilityOnReceiveDamage();
	
	//int UnitIndex;
	inline int GetUnitIndex();

	virtual bool Process();
	virtual bool Process(OneObject* OB);
	virtual bool OnClick();
	virtual bool OnRightClick();
	virtual bool Execute(word TargetUnit, int x, int y, int z);	
	virtual bool isTarget();
	virtual bool CanTarget(word TargetUnit, int x, int y, int z);

	virtual int GetCoolDownProc() { return 0; } // 0 - ready, 1-100 - not ready
	virtual void StartCoolDown() {}

	//
	virtual bool CanApply(); // ????????
	//
	virtual bool isActive();
	virtual bool OnMakeDamage(OneObject* Take,int& Damage);
	//
	virtual bool DrawCursor(int x,int y);
	//
	virtual bool IsHero();
	virtual int GetRadius();
	virtual bool ShowRadius();

	virtual bool CanYouAddToObject(OneObject* OB,void* Param);
	virtual bool AddToObject(OneObject* OB,void* Param);
	virtual bool AddToObject(OneObject* OB);
	virtual bool FillParam(OneObject* OB, void* Param);
	virtual ActiveUnitAbility* GetActionAbilityExample();
	virtual int GetUsePause();

	virtual int AddActionAbilityOverUnitsInRadius();

	virtual const char* GetHint();
	void GetButton(int& FileID, int& SpriteID, int& FileID_Char, int& FileID_Inv);

	UnitAbility* BaseUA; // родительская абилка
	inline UnitAbility* GetA(){
		if(BaseUA==NULL){
			BaseUA=UnitAbilityRef.Get();
		}
		return BaseUA;
	}
	void SetA(UnitAbility* A);
	UnitAbility* GetAW();
	UnitAbilityIcon* IconInfo;
	UnitAbilityIcon* GetUnitAbilityIconInfo();

private:
	static bool ApplyActionAbility(OneObject* OB,void* Param);
	int UseTime;
	ClassPtr<UnitAbility> UnitAbilityPtr;
	ClassRef<UnitAbility> UnitAbilityRef; 

public:
	bool ChangeState(int NewState);
	
	void SetInfluenceMask();

	virtual NewAnimation* GetAuraAnimation() { return NULL; }
	virtual bool CheckAnimationMask() { return false; }

	virtual void CastingSpell() {}
	virtual void DropDown(int x=-1, int y=-1) {}

	virtual void SetVisible(bool State);
	virtual bool GetAuraActiveState() { return true; }

	void MoveToEnd();

	bool TryClothe(bool ClotheState, int Slot, bool ErrorHint);
	bool SetClothe(bool ClotheState, int Slot);
	bool SeekSameAndClothe();

	virtual void LevelUp();

};
//
#define ListWalk(x) if(InfluenceMask.check_bit(ABL_##x)) for(int i=0;i<GetAmount();i++)\
	if( Values[i] && Values[i]->InfluenceMask.check_bit(ABL_##x) && Values[i]->GetA()->Conditions.Check(Values[i])) Values[i]->modify##x

#define ListBool(x) if(InfluenceMask.check_bit(ABL_##x)) for(int i=0;i<GetAmount();i++)\
	if( Values[i] && Values[i]->InfluenceMask.check_bit(ABL_##x) && Values[i]->GetA()->Conditions.Check(Values[i])) Current|=Values[i]->is##x

#define ListExec(x) if(InfluenceMask.check_bit(ABL_##x)) for(int i=0;i<GetAmount();i++)\
	if( Values[i] && Values[i]->InfluenceMask.check_bit(ABL_##x) && Values[i]->GetA()->Conditions.Check(Values[i])) Values[i]->exec##x

//#define ListBool(x) if(InfluenceMask.check_bit(ABL_##x))\
//	for(int i=0;i<GetAmount();i++){\
//		ActiveUnitAbility* AA=Values[i];\
//		UnitAbility* UA=AA->GetA();\
//		if( AA && AA->InfluenceMask.check_bit(ABL_##x) && UA->Conditions.Check(AA) ){\
//			UnitModificatorsIterator iterator_UM;\
//			iterator_UM.Create(AA,ABL_##x);\
//			while( /*ABL_Compassion* M=(ABL_Compassion*)*/iterator_UM.Next() ){\
//				Current=true;\
//				break;\
//			}\
//		}\
//	}

#define ListABL(x) for(int i=0;i<GetAmount();i++)\
	if (Values[i]->InfluenceMask.check_bit(ABL_##x) && Values[i]->GetA()->Conditions.Check(Values[i])) Values[i]->modify##x
//

class ActiveAbilityListArray : public ClassArray<ActiveUnitAbility>
{
public:	
	uaModMask InfluenceMask; //contains combination of ABL_... flags. 
	//If flag is set, then this ability can ifluence on this type of action	
	//constructor should FILL this field.
	bool AnimationMask; //one of ability draw effect

	int InvisibleEnableTime;

	inline void modifyMotionSpeed       (int BasicSpeed,int& CurrentSpeed,bool& Stun){
		if(InfluenceMask.check_bit(ABL_MotionSpeed)){
			ListABL(MotionSpeed)(BasicSpeed,CurrentSpeed,Stun);
			if(CurrentSpeed<0)CurrentSpeed=0;
		}
	}
	inline void modifyProtection        (int AttType,int BasicProtection,int& CurrentProtection,OneObject* Damager){
		ListWalk(Protection)(AttType,BasicProtection,CurrentProtection,Damager);
	}
	inline void modifyShield            (int BasicShield,int& CurrentShield,OneObject* Damager){
		ListWalk(Shield)(BasicShield,CurrentShield,Damager);
	}
	inline void modifyDamage            (int AttType,int BasicDamage,int& CurrentDamage,OneObject* Victim){
		ListWalk(Damage)(AttType,BasicDamage,CurrentDamage,Victim);
	}
	inline void modifyAttackRate        (int AttType,int BasicRate,int& ChangedRate,OneObject* Victim){
		ListWalk(AttackRate)(AttType,BasicRate,ChangedRate,Victim);
	}
	inline void modifyMagicImmunity     (bool Basic,bool& Current,OneObject* Caster){
		ListWalk(MagicImmunity)(Basic,Current,Caster);
	}
	inline void modifySkillImmunity     (bool Basic,bool& Current,OneObject* Caster){
		ListWalk(SkillImmunity)(Basic,Current,Caster);
	}
	inline void modifyLifeRegeneration  (int Basic,int& Current){//HP per 100 sec
		ListWalk(LifeRegeneration)(Basic,Current);
	}
	inline void modifyWeapon			(Weapon* Basic,Weapon** Current,bool Action){
		ListWalk(Weapon)(Basic,Current,Action);
	}
	inline void modifyInvisibility		(bool Basic,bool& Current,byte& Detected,bool& EnableSearchVictim);//{
	//	if( TrueTime>InvisibleEnableTime ){
	//		ListWalk(Invisibility)(Basic,Current,Detected,EnableSearchVictim);
	//	}
	//}
	//inline void modifyDamagerLife		(word Basic,word& Current,int Damage,OneObject* Damager,OneObject* Victim){
	//	ListWalk(DamagerLife)(Basic,Current,Damage,Damager,Victim);
	//}
	inline void modifyDamReduct           (int Basic, int& Current,int ReductType){
		ListWalk(DamReduct)(Basic,Current,ReductType);
	}
	inline void modifyIgnoreProtection    (int Basic, int& Current){
		ListWalk(IgnoreProtection)(Basic,Current);
	}
	inline void modifyVampire			(word Basic,word& Current,int Damage,OneObject* Damager,OneObject* Victim){
		ListWalk(Vampire)(Basic,Current,Damage,Damager,Victim);
	}
	inline void modifyThorn				(word Basic,word& Current,int Damage,OneObject* Damager,OneObject* Victim,byte AttType){
		ListWalk(Thorn)(Basic,Current,Damage,Damager,Victim, AttType);
	}
    inline void modifyAlteration				(int Basic,int& Current, OneObject* Target){
		ListWalk(Alteration)(Basic,Current,Target);
	}
	inline void modifyAttackRadiusMax	(int Basic,int& Current){
		ListWalk(AttackRadiusMax)(Basic,Current);
	}

	inline void modifySilence			(bool Basic,bool& Current){
		ListBool(Silence)();
	}
	inline void modifySkillSilence			(bool Basic,bool& Current){
		ListBool(SkillSilence)();
	}
	inline void modifyDeath			(bool Basic,bool& Current){
		ListBool(Death)();
	}
	inline void modifyCompassion		(bool Basic,bool& Current){
		ListBool(Compassion)();
	}
	inline void modifyInvulnerability	(bool Basic,bool& Current){
		ListBool(Invulnerability)();
	}
	inline void modifyInsubordination	(bool Basic,bool& Current){
		ListWalk(Insubordination)(Basic,Current);
	}

	inline void modifyManaValue			(int Basic,int& Current){
		ListWalk(ManaValue)(Basic,Current);
	};
	inline void modifyLife				(word Basic,word& Current){
		ListWalk(Life)(Basic,Current);
	};
	inline void modifyMana				(word Basic,word& Current){
		ListWalk(Mana)(Basic,Current);
	};
	inline void modifyLifeMax			(word Basic,word& Current){
		ListWalk(LifeMax)(Basic,Current);
	};
	inline void modifyManaMax			(word Basic,word& Current){
		ListWalk(ManaMax)(Basic,Current);
	};
	inline void modifyMaterial			(byte Basic, byte& Current){
		ListWalk(Material)(Basic,Current);
	};	
	
	inline void modifyManaRegeneration	(int Basic,int& Current){
		ListWalk(ManaRegeneration)(Basic,Current);
	};
	inline void modifyVision			(byte Basic, byte& Current){
		ListWalk(Vision)(Basic,Current);
	};
	inline void modifyProduceEnable		(bool Basic, bool& Current){
		ListWalk(ProduceEnable)(Basic,Current);
	};

	inline void modifyScale				(word Basic, word& Current){
		ListWalk(Scale)(Basic,Current);
	};
	//
	inline void modifySummon			(bool& Current){
		ListBool(Summon)();
	}
	inline void modifyFrozen			(bool& Current){
		ListBool(Frozen)();
	}
	//
	inline void modifyModelAnimation	(bool Basic, bool& Current){
		ListWalk(ModelAnimation)(Basic,Current);
	}

	//inline void modifyModelAnimation	(int Basic, int& Current){
	//	ListMod(ExtraAnimation)
	//}

	inline void modifyLevelForEffects	(int Basic, int& Current){
		ListWalk(LevelForEffects)(Basic,Current);
	}
	inline void modifyTradeSkill		(int Basic, int& Current){
		ListWalk(TradeSkill)(Basic,Current);
		if( Current>158 )
			Current=158;
	}
	inline void modifyLearning			(int Basic, int& Current){
		ListWalk(Learning)(Basic,Current);
	}
	inline void modifyWeaponKind		(int Basic, int& Current, int AttType){
		ListWalk(WeaponKind)(Basic,Current,AttType);
	}
	inline void modifyEvasion			(int Basic, int& Current){
		ListWalk(Evasion)(Basic,Current);
	}
	inline void modifyMiss				(int Basic, int& Current){
		ListWalk(Miss)(Basic,Current);
	}

	inline void modifyExtraAnimation	(int Basic, int& Current){
		ListWalk(ExtraAnimation)(Basic,Current);
	}

	inline void modifyItemPickUp		(bool Basic, bool& Current){
		if(InfluenceMask.check_bit(ABL_ItemPickUp)){
			Current=false;
		};
	}
	inline void modifyExtraAbility(OneObject* NewUnit){
		ListWalk(ExtraAbility)(NewUnit);
	}
	inline void modifyDeleteAbility(OneObject* NewUnit){
	ListWalk(DeleteAbility)(NewUnit);
	}
	inline void modifySearchVictim		(OneObject*& Victim, int Radius, int& NShots){
		ListWalk(SearchVictim)(Victim,Radius,NShots);
	}
	inline void modifyMotionStyle		(byte Basic, byte& Current){
		ListWalk(MotionStyle)(Basic,Current);
	}	
	inline void modifyLockType			(byte Basic, byte& Current){
		ListWalk(LockType)(Basic,Current);
	}
	inline void modifyNMask				(byte Basic, byte& Current){
		ListWalk(NMask)(Basic,Current);
	}
	inline bool isCenterScreen(){
		bool Current=false;
		ListBool(CenterScreen)();
		return Current;
	}
	virtual void execSpellOnAttack(OneObject* Enemy,int ActType,int TargetType){
		ListExec(SpellOnAttack)(Enemy,ActType,TargetType);
	};
	virtual void execDeleteAbilityOnReceiveDamage(){
		ListExec(DeleteAbilityOnReceiveDamage)();
	};

	int Add(ActiveUnitAbility* V);
	void Del(int pos,int N);

	void ResetInfluenceMask();
};

class mod_Plymorph;
class mod_ModelAnimation;
class UnitActiveAbilityList : public BaseClass
{
public:
	ActiveAbilityListArray ActiveAbilities;

	bool CheckHero;
	bool Hero;

	bool CheckChest;
	bool Chest;

	int CardManaRegeneration;

	bool Summon;
	bool Frozen;

	word LibID;	// library index
	word LibSN;	// library serial

	OneObject* OB;

	word BornNIndex; // first type of unit
	int BornState;
	byte BornNNUM;
	byte BornNMask;
	NewMonster* BornNM;
	AdvCharacter* BornAC;
	byte BornKillMask;
	short BornFlyHeight;

	bool ChangingNIndex;

	word BornModelAnimationNIndex;
	word ModelAnimationNIndex;

	word AttackCasterID;
	word AttackCasterSN;

	int TransparencyStartTime;
	byte Transparency;
	byte TransparencyLast;

	mod_Plymorph* Morph;
	
	mod_ModelAnimation* ModelAnimation;
	word TargetModelAnimationUnitType;
	byte ControlNI;
	byte NewNMask;
	bool MaskWasEdited;
	//bool NMaskBeginProcess;
	int NMask;
	byte CurKillMask;
	short FlyHeight;

	int InvisibleEnableTime;

	int MagicsAmount;

	int AntiMageSkillPoints;

	int AML_SkillA;
	int AML_SkillB;

	// используеться чтобы удалять абилки по истечению некоторого количества определенного действия
	int NShots;

	SAVE(UnitActiveAbilityList){
		REG_AUTO(ActiveAbilities);

		REG_MEMBER(_bool,Hero);
		REG_MEMBER(_bool,Chest);

		//int CardManaRegeneration;

		REG_MEMBER(_bool,Summon);
		REG_MEMBER(_bool,Frozen);

		REG_MEMBER(_WORD,LibID);
		REG_MEMBER(_WORD,LibSN);

		REG_MEMBER(_short,BornNIndex);
		//int BornState;
		REG_MEMBER(_BYTE,BornNNUM);
		REG_MEMBER(_BYTE,BornNMask);
		//NewMonster* BornNM;
		//AdvCharacter* BornAC;
		REG_MEMBER(_BYTE,BornKillMask);
		REG_MEMBER(_short,BornFlyHeight);

		REG_MEMBER(_bool,ChangingNIndex);

		REG_MEMBER(_WORD,BornModelAnimationNIndex);
		REG_MEMBER(_WORD,ModelAnimationNIndex);

		REG_MEMBER(_short,AttackCasterID);
		REG_MEMBER(_short,AttackCasterSN);

		REG_MEMBER(_int,TransparencyStartTime);
		REG_MEMBER(_BYTE,Transparency);
		REG_MEMBER(_BYTE,TransparencyLast);

		//mod_Plymorph* Morph;
		//mod_ModelAnimation* ModelAnimation;
		REG_MEMBER(_BYTE,ControlNI);

		REG_MEMBER(_int,InvisibleEnableTime);

		REG_MEMBER(_int,AntiMageSkillPoints);

		REG_MEMBER(_int,AML_SkillA);
		REG_MEMBER(_int,AML_SkillB);

		REG_MEMBER(_int,NShots);

	}ENDSAVE;

	void Init();
	void reset_class(void* p) { ((UnitActiveAbilityList*)p)->Init(); }
	UnitActiveAbilityList();
	UnitActiveAbilityList(OneObject* OB);

	bool Process();
	bool PostProcess();

	bool AddActiveUnitAbility(ActiveUnitAbility* AUA);
	ActiveUnitAbility* GetActiveUnitAbility(const char* name);

	void SetBornNIndex();
};
//============================================
class ArrayAbilities:public ClassArray<UnitAbility>
{
public:
	int GetExpansionRules();
	bool SafeReadFromFile(const char* Name);
	bool ReadFromFile(const char* Name);
	bool WriteToFile     (const char* Name);
};
class AbilityList : public BaseClass
{
public:
	//AbilityList(void);
	ArrayAbilities Abilities;
	SAVE(AbilityList)
		REG_AUTO(Abilities);
	ENDSAVE;		
	bool LoadAbilities(char* FileName);
};
//============================================
/*	Type: 
5 - CoolDown
6 - ChangeMDAbilitiActive 
7 - HeroVariableStorage
8 - ChooseUpHeroParam
9 - ActiveGreeting
10 - ActiveChangedMoralForBrigade
11 - ActiveTimeChangedMoralForBrigade
12 - ActiveBrigadeBonus
13 - ActiveMineBonus
14 - ActiveAdditionalLife
15 - ActiveBuildingShield
16 - PassiveAddUnitBonus
*/
//============================================
class UnitAbility2 : public UnitAbility
{
public:
	UnitAbility2();
	short FileID2;
	int SpriteID2;
	SAVE(UnitAbility2){
		REG_PARENT(UnitAbility)
		REG_MEMBER(_gpfile,FileID2);
		REG_SPRITE(SpriteID2,FileID2);
	}ENDSAVE;
};
class UnitTypeList:public ClassArray<BaseClassUnitType>
{
public:
	int GetExpansionRules();
};
class UnitAbilityAura : public UnitAbility
{
public:
	UnitAbilityAura();
	int AddDamage;
	int AddShield;
	int Cure;
	int Radius;
	bool SumAuraEffect;
	UnitTypeList ChoiceUnitType;
	bool EnemyUnitEffect;
	bool FriendlyUnitEffect;
	SAVE(UnitAbilityAura)
		REG_PARENT(UnitAbility);
	REG_MEMBER(_int,AddDamage);
	REG_MEMBER(_int,AddShield);
	REG_MEMBER(_int,Cure);
	REG_MEMBER(_int,Radius);
	REG_MEMBER(_bool,SumAuraEffect);
	REG_AUTO(ChoiceUnitType);
	REG_MEMBER(_bool,EnemyUnitEffect);
	REG_MEMBER(_bool,FriendlyUnitEffect);
	ENDSAVE;
	virtual bool OnUnitBirth(OneObject* Newbie);
};
//
class ActiveUnitAbilityAura : public ActiveUnitAbility
{
public:
	ActiveUnitAbilityAura();
	SAVE(ActiveUnitAbilityAura)
		REG_PARENT(ActiveUnitAbility);
	ENDSAVE;
	virtual bool Process();
	virtual int GetRadius();
	static bool CheckAbil(OneObject* OB,void* param);
};
//
class OneUnitInComposition:public BaseClass{
public:
	OneUnitInComposition(){
		ZAccelerationWhenDie=7;
		XSpeedWhenDie=0;
		YSpeedWhenDie=-50;   
	}
	int UnitType;
    int dx,dy,dz,LeadDist;

	int ZAccelerationWhenDie;
	int XSpeedWhenDie;
	int YSpeedWhenDie;

	int ModelID;
	_str NodeName;
	int LeadingType;
	SAVE(OneUnitInComposition);
		REG_MEMBER(_UnitType,UnitType);
		REG_MEMBER(_int,dx);
		REG_MEMBER(_int,dy);
		REG_MEMBER(_int,dz);

		REG_MEMBER(_int,ZAccelerationWhenDie);
		REG_MEMBER(_int,XSpeedWhenDie);
		REG_MEMBER(_int,YSpeedWhenDie);

		REG_MEMBER(_int,LeadDist);    
		REG_MEMBER(_ModelID,ModelID);
		REG_AUTO(NodeName);
		REG_ENUM(_index,LeadingType,LEADING_TYPE);
	ENDSAVE;	
};
class UnitsComposition:public UnitAbility{
public:
	//UnitsComposition();
	ClonesArray<OneUnitInComposition> Units;	
	SAVE(UnitsComposition);
		REG_PARENT(UnitAbility);	
		REG_AUTO(Units);
	ENDSAVE;
	virtual bool OnUnitBirth(OneObject* OBJ);
};
class PushAllOnTheWay:public UnitAbility{
public:
	PushAllOnTheWay(){
		PushForwardRadius=100;		
		PushLeftRightRadius=60;
		PushAngle=80;
		MaxDamage=30;
		MinDamage=50;
		PushForce=100;
		PushEnemyUnits=true;
		PushFriendlyUnits=true;
		DamageFriendlyUnits=true;
		DamageEnemyUnits=true;
		KillNonPushableUnits=true;
		AskGoAwayDistance=200;
		AskFriendlyUnitsGoAway=true;
		UseAttack1NearEnemy=0;
		CanScare=false;
		MaxBoldness=0;
		BoldnessRegeneration=0;
		PushCenterShift=0;
	}
	int PushForwardRadius;	
	int PushLeftRightRadius;
	int PushAngle;
	int MaxDamage;
	int MinDamage;
	int PushForce;
	int AskGoAwayDistance;	
    bool Move3DUnitsFromArea;
	bool PushEnemyUnits;
	bool PushFriendlyUnits;
	bool DamageFriendlyUnits;
	bool DamageEnemyUnits;	
	bool AskFriendlyUnitsGoAway;
	bool KillNonPushableUnits;
	bool UseAttack1NearEnemy;
	bool CanScare;
	int MaxBoldness;
	int BoldnessRegeneration;
	int PushCenterShift;
	SAVE(PushAllOnTheWay);
		REG_PARENT(UnitAbility);	
		REG_MEMBER(_int,PushForwardRadius);
		REG_MEMBER(_int,PushLeftRightRadius);
		REG_MEMBER(_int,PushAngle);
		REG_MEMBER(_int,MaxDamage);
		REG_MEMBER(_int,MinDamage);
		REG_MEMBER(_int,PushForce);
		REG_MEMBER(_int,AskGoAwayDistance);
        REG_MEMBER(_bool,Move3DUnitsFromArea);
		REG_MEMBER(_bool,PushEnemyUnits);
		REG_MEMBER(_bool,PushFriendlyUnits);	
		REG_MEMBER(_bool,DamageEnemyUnits);
		REG_MEMBER(_bool,DamageFriendlyUnits);	
		REG_MEMBER(_bool,AskFriendlyUnitsGoAway);
		REG_MEMBER(_bool,KillNonPushableUnits);
		REG_MEMBER(_bool,UseAttack1NearEnemy);
		REG_MEMBER(_bool,CanScare);
		REG_MEMBER(_int,MaxBoldness);
		REG_MEMBER(_int,BoldnessRegeneration);
		REG_MEMBER(_int,PushCenterShift);
	ENDSAVE;
	virtual bool OnUnitBirth(OneObject* OBJ);
	virtual ActiveUnitAbility* GetNewActiveAbility();
};
//==================================================================================================================//
#define NSlowDegrees 10
class BeSlowNearUnits:public UnitAbility{
public:
	int Radius;
	int ShiftFromCenter;
	int MaxUnitsAmount;
	bool BeSlowOnlyInEnemyGroup;
	float SlowDegree[NSlowDegrees];
	BeSlowNearUnits(){
		Radius=64;
		ShiftFromCenter=0;
		MaxUnitsAmount=30;
		BeSlowOnlyInEnemyGroup=true;
		for(int i=0;i<NSlowDegrees;i++)SlowDegree[i]=1.0f*(NSlowDegrees-i-1)/(NSlowDegrees-1);
	}
	SAVE(BeSlowNearUnits);
		REG_PARENT(UnitAbility);		
        REG_MEMBER(_int,Radius);
		REG_MEMBER(_int,ShiftFromCenter);
		REG_MEMBER(_bool,BeSlowOnlyInEnemyGroup);
		REG_MEMBER(_int,MaxUnitsAmount);
		for(int i=0;i<NSlowDegrees;i++){
			char cc[16];
			sprintf(cc,"SlowDegree%d",i);
			REG_MEMBER_EX2(_float01,SlowDegree[i],cc);
		}
	ENDSAVE;	
	virtual ActiveUnitAbility* GetNewActiveAbility();
	virtual void CopyToActive(ActiveUnitAbility* ab);
};
class aa_BeSlowNearUnits:public ActiveUnitAbility{
public:
	BeSlowNearUnits ABL;
	SAVE(aa_BeSlowNearUnits);
	REG_PARENT(ActiveUnitAbility);
	REG_AUTO(ABL);
	ENDSAVE;
	virtual bool Process(OneObject* OB);
};
class MakeDamageOnDeath:public UnitAbility{
public:
	bool DamageEnemy;
	bool DamageFriens;
	int xc;
	int yc;
	int R;
	int DamageValue;
	SAVE(MakeDamageOnDeath);
		REG_PARENT(UnitAbility);
		REG_MEMBER(_bool,DamageEnemy);
		REG_MEMBER(_bool,DamageFriens);
		REG_MEMBER(_int,xc);
		REG_MEMBER(_int,yc);
		REG_MEMBER(_int,R);
		REG_MEMBER(_int,DamageValue);
	ENDSAVE;
	MakeDamageOnDeath(){
		DamageEnemy=true;
		DamageFriens=true;
		xc=0;
		yc=0;
		R=50;
		DamageValue=1000;
	}
	virtual ActiveUnitAbility* GetNewActiveAbility();
	virtual void CopyToActive(ActiveUnitAbility* ab);
};
class aa_MakeDamageOnDeath:public ActiveUnitAbility{
public:
	MakeDamageOnDeath MD;
	bool Done;
	SAVE(aa_MakeDamageOnDeath);
	REG_AUTO(MD);
	REG_MEMBER(_bool,Done);
	ENDSAVE;
	virtual bool Process(OneObject* OB);
	aa_MakeDamageOnDeath(){
        Done=false;
	}
};
class UnitAbilityMagicEffect : public UnitAbility
{
public:
	UnitAbilityMagicEffect();
	_str WeaponModificatorName;
	UserFriendlyNumericalReturner MinDist;
	UserFriendlyNumericalReturner MaxDist;
	UserFriendlyNumericalReturner Damage;
	UserFriendlyNumericalReturner Radius;
	UserFriendlyNumericalReturner ManaCost;
	UserFriendlyNumericalReturner CoolDownTime;
	bool ContinueUse;
	UserFriendlyNumericalReturner UsePause;
	UserFriendlyNumericalReturner N;
	int AttType;
	bool EnemyUnitTarget;
	bool FriendlyUnitTarget;
	bool SetPointTargetPoint;
	bool NeedTarget;
	bool TipaAura;
	bool ManualOnOff;
	ClonesArray<HeroVariable> Variables;
	SAVE(UnitAbilityMagicEffect)
		REG_PARENT(UnitAbility);
		REG_MEMBER(_str,WeaponModificatorName);
		REG_AUTO(MinDist);
		REG_AUTO(MaxDist);
		REG_AUTO(Damage);
		REG_AUTO(Radius);
		REG_AUTO(ManaCost);
		REG_AUTO(CoolDownTime);
		REG_MEMBER(_bool,ContinueUse);
		REG_AUTO(UsePause);
		REG_AUTO(N);
		REG_MEMBER(_int,AttType);
		REG_MEMBER(_bool,EnemyUnitTarget);
		REG_MEMBER(_bool,FriendlyUnitTarget);
		REG_MEMBER(_bool,SetPointTargetPoint);
		REG_MEMBER(_bool,NeedTarget);
		REG_MEMBER(_bool,TipaAura);
		REG_MEMBER(_bool,ManualOnOff);
		REG_AUTO(Variables);
	ENDSAVE;

	virtual bool OnUnitBirth(OneObject* OBJ);
};
class ActiveUnitAbilityMagicEffect : public ActiveUnitAbility
{
public:
	ActiveUnitAbilityMagicEffect();
	SAVE(ActiveUnitAbilityMagicEffect)
		REG_PARENT(ActiveUnitAbility);
	ENDSAVE;
	virtual bool Process();
	virtual bool isTarget();
	virtual bool CanTarget(word TargetUnit, int x, int y, int z);
	virtual bool Execute(word TargetUnit, int x, int y, int z);
	virtual bool OnClick();
	int TempRadius;
	virtual bool DrawCursor(int x,int y);

	int Tx;
	int Ty;
	int Tz;
	int UseTime;
	word Target;
	int CoolDownTime;
	bool OnOff;

	HeroVariableStorage* HVS;
	bool CheckHero;
private:
	bool ApplayWeapon();
};
class IntelligentAura : public UnitAbility 
{
public:
	IntelligentAura();
	int Radius;
	SAVE(IntelligentAura)
		REG_PARENT(UnitAbility);
		REG_MEMBER(_int,Radius);
	ENDSAVE;
};
class VampiricAbility : public UnitAbility
{
public:
	VampiricAbility();
	//int DamageProc;
	int LifeProc;
	SAVE(VampiricAbility)
		REG_PARENT(UnitAbility);
		REG_MEMBER(_int,LifeProc);
	ENDSAVE;
	virtual bool OnUnitBirth(OneObject* OBJ);
	//virtual bool Execute(word Unit, word TargetUnit, int x, int y, int z);
};
class ActiveVampiricAbility : public  ActiveUnitAbility
{
public:
	SAVE(ActiveVampiricAbility)
		REG_PARENT(ActiveUnitAbility);
	ENDSAVE;
	virtual bool OnMakeDamage(OneObject* Take,int& Damage);
	virtual bool Process();
};
class BlizardAbility : public UnitAbility
{
public:
	int MaxDist;
	UserFriendlyNumericalReturner Damage;
	UserFriendlyNumericalReturner Radius;
	int AttType;
	int DamagePause;
	_str EffectName;
	UserFriendlyNumericalReturner ManaCost;
	UserFriendlyNumericalReturner CoolDownTime;
	ClonesArray<HeroVariable> Variables;
	SAVE(BlizardAbility)
		REG_PARENT(UnitAbility);
		REG_MEMBER(_int,MaxDist);
		REG_AUTO(Damage);
		REG_AUTO(Radius);
		REG_MEMBER(_int,AttType);
		REG_MEMBER(_int,DamagePause);
		REG_MEMBER(_str,EffectName);
		REG_AUTO(ManaCost);
		REG_AUTO(CoolDownTime);
		REG_AUTO(Variables);
	ENDSAVE;
	virtual bool OnUnitBirth(OneObject* OBJ);
};
class ActiveBlizardAbility : public ActiveUnitAbility
{
public:
	ActiveBlizardAbility();

	int Tx;
	int Ty;
	int Tz;
	int InUseTime;
	int CoolDownTime;

	SAVE(ActiveBlizardAbility)
		REG_PARENT(ActiveUnitAbility);
		REG_MEMBER(_int,Tx);
		REG_MEMBER(_int,Ty);
		REG_MEMBER(_int,InUseTime);
		REG_MEMBER(_int,CoolDownTime);
	ENDSAVE;

	HeroVariableStorage* HVS;
	bool CheckHero;

	virtual bool Process();
	virtual bool isTarget();
	virtual bool CanTarget(word TargetUnit, int x, int y, int z);
	virtual bool Execute(word TargetUnit, int x, int y, int z);
	int TempRadius;
	virtual bool DrawCursor(int x,int y);
};
//======================================
class LeadSeveralUnitsAbility:public ActiveUnitAbility{//Drew
public:
	//LeadSeveralUnitsAbility();
	word ObjID;
	word ObjSN;
	int dx,dy,dz;
	int LeadDistance;
	int ActionType;//0-set position 1-lead using DestX
	DWORD ModelID;
	DWORD NodeID;
	SAVE(LeadSeveralUnitsAbility);
	REG_PARENT(ActiveUnitAbility);
	ENDSAVE;
	virtual bool Process();
};
class PushUnitsAbility:public ActiveUnitAbility{//Drew
public:
	PushUnitsAbility();
	PushAllOnTheWay Params;
	word OwnerID;
	word OwnerSN;
	int Boldness;
	int LastTimeCheck;
	SAVE(PushUnitsAbility);
		REG_PARENT(ActiveUnitAbility);
		REG_AUTO(Params);
		REG_MEMBER(_short,OwnerID);
		REG_MEMBER(_short,OwnerSN);
		REG_MEMBER(_int,Boldness);
		REG_MEMBER(_int,LastTimeCheck);
	ENDSAVE;

	virtual void OnAddToObject(OneObject* OB);
	virtual bool Process();

};
class BeSlaveOfUnit:public ActiveUnitAbility{//Drew
public:
	void LeadSeveralUnitsAbility();
	word ObjID;
	word ObjSN;

	int DeathAccelerationZ;
	int DeathSpeedX;
	int DeathSpeedY;
	int LastX;
	int LastY;
	int LastZ;

	SAVE(BeSlaveOfUnit);
	REG_PARENT(ActiveUnitAbility);
	ENDSAVE;
	virtual bool Process();
};
class CoolDown : public ActiveUnitAbility
{
public:
	CoolDown(void);
	CoolDown(int UnitAbilityIndex, int Value);
	int UnitAbilityIndex;
	int Value;
	int LastProcesedTime;
	SAVE(CoolDown)
		REG_PARENT(ActiveUnitAbility);
		REG_MEMBER(_int,UnitAbilityIndex);
		REG_MEMBER(_int,Value);
		REG_MEMBER(_int,LastProcesedTime);
	ENDSAVE;
	virtual bool Process();
};
class AuraEffect : public ActiveUnitAbility
{
public:
	AuraEffect(void);
	int Creator;
	int AbilityIndex;
	int LastProcesedTime;
	//int AddDamage;
	//int AddShield;
	//int Cure;
	//int Dist;
	SAVE(AuraEffect)
		REG_PARENT(ActiveUnitAbility);
		REG_MEMBER(_int,Creator);
		REG_MEMBER(_int,AbilityIndex);
		REG_MEMBER(_int,LastProcesedTime);
		//REG_MEMBER(_int,AddDamage);
		//REG_MEMBER(_int,AddShield);
		//REG_MEMBER(_int,Cure);
		//REG_MEMBER(_int,Dist);
	ENDSAVE;
	virtual bool Process();
};
class IntelligentAuraEffect : public ActiveUnitAbility
{
public:
	int Creator;
	int AbilityIndex;
	IntelligentAuraEffect();
	SAVE(IntelligentAuraEffect)
		REG_PARENT(ActiveUnitAbility);
	ENDSAVE;
};
class CannonAutoShot : public UnitAbility
{
public:
	CannonAutoShot();
	SAVE(CannonAutoShot)
		REG_PARENT(UnitAbility);
	ENDSAVE;
	virtual bool OnUnitBirth(OneObject* Newbie);
};
class CannonAutoShotActive : public ActiveUnitAbility
{
public:
	CannonAutoShotActive();
	bool On;
	SAVE(CannonAutoShotActive)
		REG_PARENT(ActiveUnitAbility);
		REG_MEMBER(_bool,On);
	ENDSAVE;
	virtual bool Process();
	virtual bool OnClick();
};
class LeveledActiveUnitAbility : public ActiveUnitAbility
{
public:
	LeveledActiveUnitAbility();
	int Level;
	bool VirtualUp;

	int* Le;
	bool* Vi;
    int HintParam;

	SAVE(LeveledActiveUnitAbility)
		REG_PARENT(ActiveUnitAbility);
		REG_MEMBER(_int,Level);
		REG_MEMBER(_bool,VirtualUp);
	ENDSAVE;
	virtual bool UpLevel();
	virtual bool OnClick();
	virtual bool CanApply();
	virtual const char* GetHint();
	virtual const char* GetHintOnLevel(int Lev);
	virtual int GetHintParamOnLevel(int Lev);
private:
	DString TempHint;
	DString TempHintLv;
};
class RomeHero : public UnitAbility
{
public:
	RomeHero();
	LinearArray<int,_int> LevelUp;
	int ExpProcIfNotKiller;
	bool IsHero;
	bool GatherExperienceInCampaign;
	int UpOrder[3];
	bool StayBack;
	SAVE(RomeHero)
		REG_PARENT(UnitAbility);
		REG_AUTO(LevelUp);
		REG_MEMBER(_int,ExpProcIfNotKiller);
		REG_MEMBER(_bool,IsHero);
		REG_MEMBER(_bool,GatherExperienceInCampaign);
		REG_MEMBER(_int,UpOrder[0]);
		REG_MEMBER(_int,UpOrder[1]);
		REG_MEMBER(_int,UpOrder[2]);
		REG_MEMBER(_bool,StayBack);
	ENDSAVE;
	virtual ActiveUnitAbility* GetNewActiveAbility();
};
class ActiveRomeHero : public ActiveUnitAbility
{
public:
	ActiveRomeHero();
	~ActiveRomeHero();
	int Level;
	int Expa;
	int ExpaToNextLevel;
	bool WaitForAbilUp;
	bool LoadFromProfile;
	
	int* Le;
	int* Ex;
	bool* Wa;
	
	SAVE(ActiveRomeHero)
		REG_PARENT(ActiveUnitAbility);
		REG_MEMBER(_int,Level);
		REG_MEMBER(_int,Expa);
		REG_MEMBER(_int,ExpaToNextLevel);
		REG_MEMBER(_bool,WaitForAbilUp);
	ENDSAVE;

	virtual bool Process(OneObject* OB);
	virtual bool IsHero();
	void GatherExperience(OneObject* Victim,OneObject* Killer, byte Part);

	int GetExperienceToNextLevel();
	int UpLevel();
	int AddExperience(int Expa);
	int GetLevel();
	int GetExperience();
	int GetAmountFreeLevels();
};
class RomeHeroCollector : public BaseClass
{
public:
	RomeHeroCollector();
	//DynArray<ActiveRomeHero*> Herosima;
	DynArray<int> NHeroInMap;
	bool Calk;

	void ClearAll();
	bool AddExp(OneObject* Victim,OneObject* Killer);
	void AddRomeHero(ActiveRomeHero* Her);
	void DelRomeHero(ActiveRomeHero* Her);
};
class LifeLimitation : public UnitAbility
{
public:
	LifeLimitation();
	UserFriendlyNumericalReturner LifeLength;
	_str DieWeaponEffect;
	SAVE(LifeLimitation)
		REG_PARENT(UnitAbility);
		REG_AUTO(LifeLength);
		REG_AUTO(DieWeaponEffect);
	ENDSAVE;
	virtual bool OnUnitBirth(OneObject* Newbie);
};
class ActiveLifeLimitation : public ActiveUnitAbility
{
public:
	ActiveLifeLimitation();
	int DieTime;
	SAVE(ActiveLifeLimitation)
		REG_PARENT(ActiveUnitAbility);
		REG_MEMBER(_int,DieTime);
	ENDSAVE;
	virtual bool Process();
};
class NationBonus : public UnitAbility2
{
public:
	NationBonus();
	int AddDamage;
	int AddShield;
	UnitTypeList UnitsType;
	bool UseUpgrade;
	int UpgradeBonus;
	int IfDieDowngrade;
	int UpgradeBonusL2;
	int IfDieDowngradeL2;
	int UpgradeBonusL3;
	int IfDieDowngradeL3;
	int UpgradeBonusL4;
	int IfDieDowngradeL4;
	int UpgradeBonusL5;
	int IfDieDowngradeL5;
	int HintParam[5];
	SAVE(NationBonus)
		REG_BASE(UnitAbility);
		REG_PARENT(UnitAbility2);
		REG_MEMBER(_int,AddDamage);
		REG_MEMBER(_int,AddShield);
		REG_AUTO(UnitsType);
		REG_MEMBER(_bool,UseUpgrade);
		REG_ENUM(_index,UpgradeBonus,ALL_UPGRADES);
		REG_ENUM(_index,IfDieDowngrade,ALL_UPGRADES);
		REG_ENUM(_index,UpgradeBonusL2,ALL_UPGRADES);
		REG_ENUM(_index,IfDieDowngradeL2,ALL_UPGRADES);
		REG_ENUM(_index,UpgradeBonusL3,ALL_UPGRADES);
		REG_ENUM(_index,IfDieDowngradeL3,ALL_UPGRADES);
		REG_ENUM(_index,UpgradeBonusL4,ALL_UPGRADES);
		REG_ENUM(_index,IfDieDowngradeL4,ALL_UPGRADES);
		REG_ENUM(_index,UpgradeBonusL5,ALL_UPGRADES);
		REG_ENUM(_index,IfDieDowngradeL5,ALL_UPGRADES);
		REG_MEMBER(_int,HintParam[0]);
		REG_MEMBER(_int,HintParam[1]);
		REG_MEMBER(_int,HintParam[2]);
		REG_MEMBER(_int,HintParam[3]);
		REG_MEMBER(_int,HintParam[4]);
	ENDSAVE;
	virtual bool OnUnitBirth(OneObject* Newbie);
};
class ActiveNationBonus : public LeveledActiveUnitAbility
{
public:
	ActiveNationBonus();
	bool m_die;
	SAVE(ActiveNationBonus)
		REG_BASE(ActiveUnitAbility);
		REG_PARENT(LeveledActiveUnitAbility);
		REG_MEMBER(_bool,m_die); 
	ENDSAVE;
	virtual bool Process();
	virtual bool UpLevel();
	virtual int GetHintParamOnLevel(int Lev);
};
class HeroLight : public UnitAbility
{
public:
	HeroLight();
	_str EffectName; 
	_str DieEffect;
	bool IsHero;
	SAVE(HeroLight)
		REG_PARENT(UnitAbility);
		REG_MEMBER(_str,EffectName);
		REG_MEMBER(_str,DieEffect);
		REG_MEMBER(_bool,IsHero);
	ENDSAVE;
	virtual bool OnUnitBirth(OneObject* Newbie);
};
class ActiveHeroLight : public ActiveUnitAbility
{
public:
	ActiveHeroLight();
	bool IsInit;
	SAVE(ActiveHeroLight)
		REG_PARENT(ActiveUnitAbility);
		REG_MEMBER(_bool,IsInit);
	ENDSAVE;
	virtual bool IsHero();
	virtual bool Process();
};
class LifeRegeneration : public UnitAbility
{
public:
	LifeRegeneration();
	int Regeneration;
	int Radius;
	int UsePause;
	int CoolDownTime;
	SAVE(LifeRegeneration)
		REG_PARENT(UnitAbility);
		REG_MEMBER(_int,Regeneration);
		REG_MEMBER(_int,Radius);
		REG_MEMBER(_int,UsePause);
		REG_MEMBER(_int,CoolDownTime);
	ENDSAVE;
	virtual ActiveUnitAbility* GetNewActiveAbility();
};
class ActiveLifeRegeneration : public ActiveUnitAbility
{
public:
	ActiveLifeRegeneration();
	SAVE(ActiveLifeRegeneration)
		REG_PARENT(ActiveUnitAbility);
	ENDSAVE;
	virtual bool Process();
	virtual bool CanApply();
	virtual int GetCoolDownProc();
	virtual bool ShowRadius();
	virtual bool OnClick();
private:
	int LastUseTime;
	bool Exec();
	static bool AddLife(OneObject* OB, void* Param);
};
class SetMineBonus : public UnitAbility2
{
public:
	SetMineBonus();
	int Bonus;
	int Radius;
	int BonusAddForLevel;
	//UnitTypeList MineType;
	SAVE(SetMineBonus)
		REG_BASE(UnitAbility);
		REG_PARENT(UnitAbility2);
		REG_MEMBER(_int,Bonus);
		REG_MEMBER(_int,Radius);
		REG_MEMBER(_int,BonusAddForLevel);
		//REG_AUTO(MineType);
	ENDSAVE;
	virtual ActiveUnitAbility* GetNewActiveAbility();
};
class ActiveSetMineBonus : public LeveledActiveUnitAbility
{
public:
	ActiveSetMineBonus();
	SAVE(ActiveSetMineBonus)
		REG_BASE(ActiveUnitAbility);
		REG_PARENT(LeveledActiveUnitAbility);
	ENDSAVE;
	virtual bool Process();
	virtual int GetRadius();
	virtual int GetHintParamOnLevel(int Lev);
private:
	static bool AddMineBonus(OneObject* OB, void* Param);
	int LastUseTime;
};
class ActiveMineBonus : public ActiveUnitAbility
{
public:
	ActiveMineBonus();
	SAVE(ActiveMineBonus)
		REG_PARENT(ActiveUnitAbility);
	ENDSAVE;
	int Radius;
	int Bonus;
	int HeroIndex;
	virtual bool Process();
};
class IncreaseMaxLife : public UnitAbility2
{
public:
	IncreaseMaxLife();
	int Points;
	int Radius;
	UnitTypeList UnitType;
	int AddForLevel;
	SAVE(IncreaseMaxLife)
		REG_BASE(UnitAbility);
		REG_PARENT(UnitAbility2);
		REG_MEMBER(_int,Points);
		REG_MEMBER(_int,Radius);
		REG_AUTO(UnitType);
		REG_MEMBER(_int,AddForLevel);
	ENDSAVE;
	virtual ActiveUnitAbility* GetNewActiveAbility();	
};
class ActiveAdditionalLife : public ActiveUnitAbility
{
public:
	ActiveAdditionalLife();
	int HeroIndex;
	int LifeAdded;
	SAVE(ActiveAdditionalLife) 
		REG_PARENT(ActiveUnitAbility);
		REG_MEMBER(_int,HeroIndex);
		REG_MEMBER(_int,LifeAdded);
	ENDSAVE;

	virtual bool Process(OneObject* OB);
	virtual bool CanYouAddToObject(OneObject* OB,void* Param);
	virtual bool FillParam(OneObject* OB, void* Param);
};
class ActiveIncreaseMaxLife : public LeveledActiveUnitAbility
{
public:
	ActiveIncreaseMaxLife();
	SAVE(ActiveIncreaseMaxLife)
		REG_BASE(ActiveUnitAbility);
		REG_PARENT(LeveledActiveUnitAbility);
	ENDSAVE;
	ActiveAdditionalLife Example;

	virtual bool Process(OneObject* OB);
	virtual int GetRadius();
	virtual ActiveUnitAbility* GetActionAbilityExample();
	virtual int GetUsePause();
	virtual int GetHintParamOnLevel(int Lev);
};
class AddUnitBonus : public UnitAbility2
{
public:
	AddUnitBonus();
	int Radius;
	int AddDamage;
	int AddShield;
	bool EnemyTarget;
	int CoolDownTime;
	int EffectTime;
	int UsePause;
	UnitTypeList UnitType;
	int AddForLevel;
	SAVE(AddUnitBonus)
		REG_BASE(UnitAbility);
		REG_PARENT(UnitAbility2);
		REG_MEMBER(_int,Radius);
		REG_MEMBER(_int,AddDamage);
		REG_MEMBER(_int,AddShield);
		REG_MEMBER(_bool,EnemyTarget);
		REG_MEMBER(_int,CoolDownTime);
		REG_MEMBER(_int,EffectTime);
		REG_MEMBER(_int,UsePause);
		REG_AUTO(UnitType);
		REG_MEMBER(_int,AddForLevel);
	ENDSAVE;
	virtual ActiveUnitAbility* GetNewActiveAbility();
};
class ActiveAddUnitBonus : public LeveledActiveUnitAbility
{
public:
	ActiveAddUnitBonus();
	SAVE(ActiveAddUnitBonus)
		REG_BASE(ActiveUnitAbility);
		REG_PARENT(LeveledActiveUnitAbility);
	ENDSAVE;

	virtual bool Process(OneObject* OB);
	virtual bool CanApply();
	virtual int GetCoolDownProc();
	virtual int GetRadius();
	virtual bool OnClick();
	virtual int GetHintParamOnLevel(int Lev);
private:
	int LastUseTime;
	bool Exec();
	static bool SetBonus(OneObject* OB, void* Param);
};
class PassiveAddUnitBonus : public ActiveUnitAbility
{
public:
	PassiveAddUnitBonus();
	int Radius;
	int AddDamage;
	int AddShield;

	int HeroIndex;
	int SetTime;
	SAVE(PassiveAddUnitBonus)
		REG_PARENT(ActiveUnitAbility);
		REG_MEMBER(_int,Radius);
		REG_MEMBER(_int,AddDamage);
		REG_MEMBER(_int,AddShield);
		REG_MEMBER(_int,HeroIndex);
		REG_MEMBER(_int,SetTime);
	ENDSAVE;
	virtual bool Process(OneObject* OB);
};
class BuildingShield : public UnitAbility2
{
public:
	BuildingShield();
	int Radius;
	int AddShield;
	bool EnemyTarget;
	int ShieldAddForLevel;
	SAVE(BuildingShield)
		REG_BASE(UnitAbility);
		REG_PARENT(UnitAbility2);
		REG_MEMBER(_int,Radius);
		REG_MEMBER(_int,AddShield);
		REG_MEMBER(_bool,EnemyTarget);
		REG_MEMBER(_int,ShieldAddForLevel);
	ENDSAVE;
	virtual ActiveUnitAbility* GetNewActiveAbility();
};
class ActiveAddBuildingShield : public LeveledActiveUnitAbility
{
public:
	ActiveAddBuildingShield();
	SAVE(ActiveAddBuildingShield)
		REG_BASE(ActiveUnitAbility);
		REG_PARENT(LeveledActiveUnitAbility);
	ENDSAVE;
	int LastUseTime;
	virtual bool Process(OneObject* OB);
	virtual int GetRadius();

	static bool AddBuildingShield(OneObject* Ob, void* param);
	virtual int GetHintParamOnLevel(int Lev);
};
class ActiveBuildingShield : public ActiveUnitAbility
{
public:
	ActiveBuildingShield();
	SAVE(ActiveBuildingShield)
		REG_PARENT(ActiveUnitAbility);
		REG_MEMBER(_int,HeroIndex);
		REG_MEMBER(_int,AddShield);
	ENDSAVE;
	int HeroIndex;
	int LastUseTime;
	int AddShield;
	virtual bool Process(OneObject* OB);
};
class FollowBrigade : public UnitAbility
{
public:
	FollowBrigade();
	bool MoveOutEnemy;
	SAVE(FollowBrigade)
		REG_PARENT(UnitAbility);
		REG_MEMBER(_bool,MoveOutEnemy);
	ENDSAVE;
	virtual ActiveUnitAbility* GetNewActiveAbility();
};
class ActiveFollowBrigade : public ActiveUnitAbility
{
public:
	ActiveFollowBrigade();
	SAVE(ActiveFollowBrigade)
		REG_PARENT(ActiveUnitAbility);
	ENDSAVE;
	virtual bool Process(OneObject* OB);
	static bool CheckEnemy(OneObject* OB,void* param);
private:
	int lastUseTime;
	int lastMoveBack;
};
class Behaviour : public UnitAbility
{
public:
	Behaviour();
	int MoveDist;
	int MoveTime;
	int ChangeDir;
	int MaxRestTime;
	int Radius;
	SAVE(Behaviour)
		REG_PARENT(UnitAbility);
		REG_MEMBER(_int,MoveDist);
		REG_MEMBER(_int,MoveTime);
		REG_MEMBER(_int,ChangeDir);
		REG_MEMBER(_int,MaxRestTime);
		REG_MEMBER(_int,Radius);
	ENDSAVE;
	virtual ActiveUnitAbility* GetNewActiveAbility();
};
class ActiveBehaviour : public ActiveUnitAbility
{
public:
	ActiveBehaviour();
	int EndMoveTime;
	int RestTime;
	int BaseX;
	int BaseY;
	SAVE(ActiveBehaviour)
		REG_PARENT(ActiveUnitAbility);
		REG_MEMBER(_int,EndMoveTime);
		REG_MEMBER(_int,RestTime);
		REG_MEMBER(_int,BaseX);
		REG_MEMBER(_int,BaseY);
	ENDSAVE;
	virtual bool Process(OneObject* OB);
private:
	void Move(OneObject* OB, Behaviour* B);
	static bool CheckSprite(OneSprite* OS,void* Param);

};

//
class LifeCast:public MagicSpell
{
public:
	LifeCast();

	SAVE(LifeCast)
		REG_PARENT(MagicSpell);
	ENDSAVE;

	virtual int CastSpell(OneObject* OB);
	virtual int CancelSpell(OneObject* OB);
};
class AttSpeedCast:public MagicSpell
{
public:
	AttSpeedCast();

	SAVE(AttSpeedCast)
		REG_PARENT(MagicSpell);
	ENDSAVE;

	virtual int CastSpell(OneObject* OB);
	virtual int CancelSpell(OneObject* OB);
};
class MoveSpeedCast:public MagicSpell
{
public:
	MoveSpeedCast();

	SAVE(MoveSpeedCast)
		REG_PARENT(MagicSpell);
	ENDSAVE;

	virtual int CastSpell(OneObject* OB);
	virtual int CancelSpell(OneObject* OB);
};
class ShieldCast:public MagicSpell
{
public:
	ShieldCast();

	SAVE(ShieldCast)
		REG_PARENT(MagicSpell);
	ENDSAVE;

	virtual int CastSpell(OneObject* OB);
	virtual int CancelSpell(OneObject* OB);
};
class PermanentLandTypeChange:public UnitAbility{
public:	
	_str DestLandName;
	_str WeaponOfBirth;
	int  WeaponBirthProbability;
	int  PeriodOfSpotBirth;
	int  SpotGrowTime;
	int  SpotRadius;
	int  FirstSpotRadius;
	int  RadiusGrowOnPeriod;

	void CreateNewSpot(int x,int y,int R);

	SAVE(PermanentLandTypeChange);
		REG_PARENT(UnitAbility);
		REG_AUTO(DestLandName);
		REG_AUTO(WeaponOfBirth);
		REG_MEMBER(_int,WeaponBirthProbability);
		REG_MEMBER(_int,PeriodOfSpotBirth);
		REG_MEMBER(_int,SpotGrowTime);
		REG_MEMBER(_int,SpotRadius);
		REG_MEMBER(_int,FirstSpotRadius);
		REG_MEMBER(_int,RadiusGrowOnPeriod);
	ENDSAVE;
	virtual ActiveUnitAbility* GetNewActiveAbility();
};
class ActivePermanentLandTypeChange:public ActiveUnitAbility{
public:
    int CurRadius;
	int TimeOfPrevSpot;
	SAVE(ActivePermanentLandTypeChange);
		REG_PARENT(ActiveUnitAbility);
		REG_MEMBER(_int,CurRadius);
		REG_MEMBER(_int,TimeOfPrevSpot);
	ENDSAVE;
	virtual bool Process(OneObject* OB);
};

//=====================================

class MDState : public BaseClass {
public:
	short FileID;
	int SpriteID;
	char* Hint;
	BaseClassUnitType Type;
	int ThroughState;
	ClonesArray< ClassRef<UnitAbility> > Abilities;
	SAVE(MDState){
		REG_MEMBER(_gpfile,FileID);
		REG_SPRITE(SpriteID,FileID);
		REG_MEMBER(_textid,Hint);
		REG_AUTO(Type);
		REG_MEMBER(_int,ThroughState);
		REG_AUTO(Abilities);
		//REG_PARENT(UnitAbility);
	}ENDSAVE;
};
class ChangeMDAbiliti : public UnitAbility {
public:
	ClonesArray<MDState> MD;

	SAVE(ChangeMDAbiliti){
		REG_AUTO(MD);
		REG_PARENT(UnitAbility);
	}ENDSAVE;

	ChangeMDAbiliti();
	virtual ActiveUnitAbility* GetNewActiveAbility();	
};
class ChangeMDAbilitiActive : public ActiveUnitAbility {
public:
	int CurMD;
	DynArray<DWORD> ASerial;

	SAVE(ChangeMDAbilitiActive){
		REG_PARENT(ActiveUnitAbility);
		REG_MEMBER(_int,CurMD);
		//REG_AUTO(ASerial);
	}ENDSAVE;

	ChangeMDAbilitiActive();
	virtual bool Process();
	virtual bool OnClick();
	virtual int GetCoolDownProc();
};

//==================================================================================================================//
class GreetingMe : public UnitAbility
{
public:
	GreetingMe();
	int Radius;
	SAVE(GreetingMe)
		REG_PARENT(UnitAbility);
		REG_MEMBER(_int,Radius);
	ENDSAVE;
	virtual bool OnUnitBirth(OneObject* Newbie);
};
class ActiveGreetingMe : public ActiveUnitAbility
{
public:
	ActiveGreetingMe();
	SAVE(ActiveGreetingMe)
		REG_PARENT(ActiveUnitAbility);
	ENDSAVE;
	int UseTime;
	virtual bool Process();
private:
	static bool AddActiveGreeting(OneObject* OB,void* Param);
};
class ActiveGreeting : public ActiveUnitAbility
{
public:
	ActiveGreeting();
	SAVE(ActiveGreeting)
		REG_PARENT(ActiveUnitAbility);
	ENDSAVE;
	int PrevDir;
	int State;
	OneObject* GreetingObject;
	virtual bool Process();
};

#include "heroability.h"
#include "BrigadeAbility.h"

enum enAbilityDialogs { 
	adEffects, adPassive, adButtons, adLevelUps, adItems, adHelm, adCloak, adMedalion, adArmor, adGloves, adRing, adBracelet, adBoots, adBelt, 
	adWeapon, adItemsTakeOff, adLibShop, adLibHero, adDialog, adButtonAuras, adDebug, adBasketMagicCards, adBasketBottles, adAntiMageSkills 
};

extern int vClotheSet[];

//#include "BE_HEADERS.h"

class UA_Lazy: public UnitAbility{
public:

	int Delay;
	int Radius;
	BaseClassUnitType Supervisor;

	SAVE(UA_Lazy){
		REG_PARENT(UnitAbility);
		REG_MEMBER(_int,Delay);
		REG_MEMBER(_int,Radius);
		REG_AUTO(Supervisor);
	}ENDSAVE;

	__constructor(UA_Lazy){
		Delay=10000;
		Radius=300;
	}

	virtual ActiveUnitAbility* GetNewActiveAbility();
	bool OnUnitBirth(OneObject* Newbie){
		return UnitAbility::OnUnitBirth(Newbie);
	};

};
class AA_Lazy: public ActiveUnitAbility{
private:
	int CheckTime;
public:	

	SAVE(AA_Lazy){
		REG_PARENT(ActiveUnitAbility);
		REG_MEMBER(_int,CheckTime);
	}ENDSAVE;

	virtual bool Process();
	//virtual bool isTarget();

	//virtual bool OnClick();
	//virtual bool OnRightClick();
	//virtual bool Execute(word TargetUnit, int x, int y, int z);

	//virtual bool DrawCursor(int x,int y);
	//virtual bool CanTarget(word TargetUnit, int x, int y, int z);

	//virtual int GetCoolDownProc();
	//virtual void StartCoolDown();

	//virtual void CastingSpell();
	//virtual void DropDown(int x=-1, int y=-1);

};


#endif