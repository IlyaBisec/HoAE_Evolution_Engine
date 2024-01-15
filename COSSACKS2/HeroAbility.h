#ifndef __HEROABILITY_H__
#define __HEROABILITY_H__

#include "WeaponSystem.h"
//==================================================================================================================//
class Probability : public BaseClass
{
public:
	int Level;
	int	Weight;
	SAVE(Probability)
		REG_MEMBER(_int,Level);
		REG_MEMBER(_int,Weight);
	ENDSAVE;
};
class UpHeroParam : public BaseClass
{
public:
	UpHeroParam();
	int Index;
	short FileID;
	int SpriteID;
	_str Hint;
	bool IsInPass;
	bool Special;
	ClonesArray<Probability> ProbabilityList;
	SAVE(UpHeroParam)
		REG_MEMBER(_int,Index);
		REG_MEMBER(_gpfile,FileID);
		REG_SPRITE(SpriteID,FileID);
		REG_MEMBER(_str,Hint);
		REG_MEMBER(_bool,IsInPass);
		REG_AUTO(ProbabilityList);
		//REG_MEMBER(_Bool,Special);
	ENDSAVE;
	virtual bool Realize(OneObject* OB);
	virtual int GetWeight(HeroVariableStorage* HVS, int Prob) { return ProbabilityList[Prob]->Weight; }
};
//==================================================================================================================//
class UpLife : public UpHeroParam
{
public:
	UpLife();
	int AddMaxLife;
	SAVE(UpLife){
		REG_PARENT(UpHeroParam);
		REG_MEMBER(_int,AddMaxLife);
	}ENDSAVE;
	virtual bool Realize(OneObject* OB);
};
class UpMana : public UpHeroParam
{
public:
	UpMana();
	int AddMaxMana;
	SAVE(UpMana){
		REG_PARENT(UpHeroParam);
		REG_MEMBER(_int,AddMaxMana);
	}ENDSAVE;
	virtual bool Realize(OneObject* OB);
};
class UpAttack : public UpHeroParam
{
public:
	UpAttack();
	int AttackAdd;
	SAVE(UpAttack)
		REG_PARENT(UpHeroParam);
		REG_MEMBER(_int,AttackAdd);
	ENDSAVE;
	virtual bool Realize(OneObject* OB);
};
class UpVision : public UpHeroParam
{
public:
	UpVision();
	int VisionAdd;
	SAVE(UpVision)
		REG_PARENT(UpHeroParam);
	REG_MEMBER(_int,VisionAdd);
	ENDSAVE;
	virtual bool Realize(OneObject* OB);
};
class UpRange : public UpHeroParam
{
public:
	UpRange();
	int RageAdd;
	int AttackType;
	SAVE(UpRange)
		REG_PARENT(UpHeroParam);
		REG_MEMBER(_int,RageAdd);
		REG_MEMBER(_int,AttackType);
	ENDSAVE;
	virtual bool Realize(OneObject* OB);
	virtual int GetWeight(HeroVariableStorage* HVS, int Prob);
};
class UpAttackSpeed : public UpHeroParam
{
public:
	UpAttackSpeed();
	int SpeedAdd;
	SAVE(UpAttackSpeed)
		REG_PARENT(UpHeroParam);
		REG_MEMBER(_int,SpeedAdd);
	ENDSAVE;
	virtual bool Realize(OneObject* OB);
};
class UpMotionSpeed : public UpHeroParam
{
public:
	UpMotionSpeed();
	int MotionSpeedAdd;
	SAVE(UpMotionSpeed)
		REG_PARENT(UpHeroParam);
		REG_MEMBER(_int,MotionSpeedAdd);
	ENDSAVE;
	virtual bool Realize(OneObject* OB);
};
class UpShield : public UpHeroParam
{
public:
	UpShield();
	int ShieldAdd;
	int AttackType;
	SAVE(UpShield)
		REG_PARENT(UpHeroParam);
		REG_MEMBER(_int,ShieldAdd);
		REG_MEMBER(_int,AttackType);
	ENDSAVE;
	virtual bool Realize(OneObject* OB);
};
class UpLifeRegeneration : public UpHeroParam
{
public:
	int RegenerationAdd;
	
	SAVE(UpLifeRegeneration){
		REG_PARENT(UpHeroParam);
		REG_MEMBER(_int,RegenerationAdd);
	}ENDSAVE;

	UpLifeRegeneration();
	virtual bool Realize(OneObject* OB);
};
class UpManaRegeneration : public UpHeroParam{
public:
	int RegenerationAdd;

	SAVE(UpManaRegeneration)
		REG_PARENT(UpHeroParam);
		REG_MEMBER(_int,RegenerationAdd);
	ENDSAVE;

	UpManaRegeneration();
	virtual bool Realize(OneObject* OB);
};
class UpSearchEnemyRadius : public UpHeroParam
{
public:
	UpSearchEnemyRadius();
	int RadiusAdd;
	SAVE(UpSearchEnemyRadius)
		REG_PARENT(UpHeroParam);
		REG_MEMBER(_int,RadiusAdd);
	ENDSAVE;
	virtual bool Realize(OneObject* OB);
};
class UpVariable : public UpHeroParam
{
public:
	UpVariable();
	_str VarName;
	int AddValue;
	SAVE(UpVariable)
		REG_PARENT(UpHeroParam);
		REG_AUTO(VarName);
		REG_MEMBER(_int,AddValue);
	ENDSAVE;
	virtual bool Realize(OneObject* OB);
};
//==================================================================================================================//
class LetPass : public UpHeroParam
{
public:
	LetPass();
	SAVE(LetPass)
		REG_PARENT(UpHeroParam);
	ENDSAVE;
	virtual bool Realize(OneObject* OB);
};
//==================================================================================================================//
class ChooseUpHeroParam : public ActiveUnitAbility
{
public:
	ChooseUpHeroParam();
	//int HeroAbilityIndex;
	ClassRef<UnitAbility> HeroAbilityRef;
	int UpIndex;
	SAVE(ChooseUpHeroParam)
		REG_PARENT(ActiveUnitAbility);
		//REG_MEMBER(_int,HeroAbilityIndex);
		REG_AUTO(HeroAbilityRef);
		REG_MEMBER(_int,UpIndex);
	ENDSAVE;
	virtual bool OnClick();
	virtual bool Process();
};
//==================================================================================================================//
class UpLevelParam : public ChooseUpHeroParam
{
public:
	SAVE(UpLevelParam)
		REG_PARENT(ActiveUnitAbility);
		REG_PARENT(ChooseUpHeroParam);
	ENDSAVE;
	virtual bool OnClick();
	virtual bool Process();
};
//==================================================================================================================//
class MagicCardActive : public ActiveUnitAbility
{
public:
	MagicCardActive();

	bool fst;
	int timer1;
	int timer2;
	int Damage;
	int Radius; 
	//int Tx,Ty;

	SAVE(MagicCardActive){
		REG_PARENT(ActiveUnitAbility);
	}ENDSAVE;

	virtual bool Process();
	virtual bool OnClick();
	virtual bool OnRightClick();
	virtual bool isTarget();
	virtual bool CanTarget(word TargetUnit, int x, int y, int z); //
	virtual bool Execute(word TargetUnit, int x, int y, int z);
	virtual bool DrawCursor(int x,int y);
	virtual int GetCoolDownProc();
};
class CardRef;
class MagicCard : public UnitAbility
{
public:
	MagicCard();
	//
	UserFriendlyNumericalReturner ManaCost; // можно задать формулой типа - (Hero:Level+1)*100 
	_str EffectName;	
	int ProcTime;
	//
	SAVE(MagicCard){
		REG_PARENT(UnitAbility);
		REG_AUTO(ManaCost);
		REG_MEMBER(_str,EffectName);
	}ENDSAVE;
	//
	virtual bool CanApply(OneObject* her, HeroVariableStorage* storage);// для интерфейса те. иконка нажемаемая
	virtual int GetCoolDownProc(OneObject* her, HeroVariableStorage* storage);// 0 - 100%
	virtual int GetRadius(OneObject* her, HeroVariableStorage* storage);
	virtual bool OnClick(OneObject* her, HeroVariableStorage* storage);
	virtual bool Execute(OneObject* her, HeroVariableStorage* storage , word TargetUnit, int x, int y, int z);
	virtual bool isTarget(OneObject* her, HeroVariableStorage* storage); // если надо указать место применения
	virtual bool CanTarget(OneObject* her, HeroVariableStorage* storage, word TargetUnit, int x, int y, int z); //
	virtual bool isActive(OneObject* her, HeroVariableStorage* storage); // 
	//
	virtual ActiveUnitAbility* GetNewActiveAbility();
	bool OnUnitBirth(OneObject* Newbie);
};
class CardList:public ClassArray<MagicCard>
{
public:
	//int GetExpansionRules();
};
class CardRef:public BaseClass
{
public:
	ClassRef<MagicCard> cardplace;
	SAVE(CardRef)
		REG_AUTO(cardplace);
	ENDSAVE;
};
//==================================================================================================================//
class UpHeroParamList:public ClassArray<UpHeroParam>
{
public:
	int GetExpansionRules();
};
//==================================================================================================================//

class ua_AtHome : public UnitAbility {
public:
	int HomeLand;
	SAVE(ua_AtHome){
		REG_ENUM(_index,HomeLand,LANDLAYERS);
		REG_PARENT(UnitAbility);
	}ENDSAVE;
	virtual ActiveUnitAbility* GetNewActiveAbility();
};
class aa_AtHome : public ActiveUnitAbility {
public:
	SAVE(aa_AtHome){
		//REG_ENUM(_index,LandType,LANDLAYERS);
	}ENDSAVE;
	virtual bool Process();
};

class HeroAbility : public UnitAbility
{
public:
	
	LinearArray<int,_int> LevelUp;
	UpHeroParamList UpHeroParams;
	UpHeroParamList UpParamForLevel;
	int ExperienceRadius;
	int GetExperienceProc;
	int GetExperienceProcAlly;
	int LifeRegeneration;
	//int ReBornTime;
	int Mana;
	int ManaRegeneration;
	int SpeedDownIfTired;
	int PlaceCard;
	UserFriendlyNumericalReturner CardRegeneration;
	//CardList baseColoda;

	ClassRef<UnitAbility> OnDieUnitAbility;
	int Lives;

	LinearArray<int,_int> AntiMageLevels;

	SAVE(HeroAbility){

		REG_MEMBER(_int,Lives);
		REG_AUTO(OnDieUnitAbility);
		
		REG_MEMBER(_int,ExperienceRadius);
		REG_MEMBER(_int,GetExperienceProc);
		REG_MEMBER(_int,GetExperienceProcAlly);
		REG_MEMBER(_int,LifeRegeneration);
		REG_AUTO(LevelUp);
		REG_AUTO(UpHeroParams);
		REG_AUTO(UpParamForLevel);
		//REG_MEMBER(_int,ReBornTime);
		REG_MEMBER(_int,Mana);
		REG_MEMBER(_int,ManaRegeneration);
		REG_MEMBER(_int,SpeedDownIfTired);
		REG_MEMBER(_int,PlaceCard);
		REG_AUTO(CardRegeneration);
//		REG_AUTO(baseColoda);

		REG_AUTO(AntiMageLevels);

		REG_PARENT(UnitAbility);
		
	}ENDSAVE;

	HeroAbility();

	virtual void OnAddToObject(OneObject* OB);

	virtual bool OnUnitBirth(OneObject* Newbie);
	virtual ActiveUnitAbility* GetNewActiveAbility();
};
//==================================================================================================================//
class HeroVariable : public BaseClass
{
public:
	HeroVariable();
	_str Name;
	int Value;
	SAVE(HeroVariable)
		REG_AUTO(Name);
		REG_MEMBER(_int, Value);
	ENDSAVE;
};

//==================================================================================================================//
class DIALOGS_API HeroVariableStorage : public ActiveUnitAbility {
public:
	
	__constructor(HeroVariableStorage){
		Type=7;
		HeroAbilityIndex=-1;
		Level=1;
		
		Experience=0;
		AddExperienceRadius=0;;
		EnableCollectExpa=true;

		Knowledge=100;
		PassSelectHeroParametr=false;
		SelectHeroParamState=false;
		Hero=NULL;
		ExperienceToNextLevel=-1;
		Visible=true;
		UpLifeRegeneration=0;
		LastUpLifeRegenerationTime=-1;
		LastUpManaRegenerationTime=-1;
		DieTime=0;
		//IsTired=false;
		colodaPointer=0;
		indexx=1;
		TypeAbil=2;
		//freePlaceCard=0;

		LoadFromProfile=false;
	}

	HeroAbility* Hero;

	bool LoadFromProfile;
	int HeroAbilityIndex;	
	//int Level;
	int Experience;
	int AddExperienceRadius;
	bool EnableCollectExpa;
	
	int Knowledge;
	bool PassSelectHeroParametr;
	bool SelectHeroParamState;
	LinearArray<int,_int> Param;
	int UpLifeRegeneration;
	int LastUpLifeRegenerationTime;
	int UpManaRegeneration;
	int LastUpManaRegenerationTime;
	//bool IsTired;
	//int SpeedMinus;
	//int freePlaceCard;
	ClonesArray<HeroVariable> Variables;
	
	int ExperienceToNextLevel;
	//int DieTime;
	//int ReBornTime;
	CardList coloda;
	int indexx;
	int colodaPointer;
	int CardRegeneration;

	int Lives;
	int AntiMageLevel;

	SAVE(HeroVariableStorage){
		REG_PARENT(ActiveUnitAbility);

		REG_MEMBER(_bool,LoadFromProfile);
		REG_MEMBER(_int,HeroAbilityIndex);
		//REG_MEMBER(_int,Level);
		REG_MEMBER(_int,Experience);
		REG_MEMBER(_int,ExperienceToNextLevel);
		REG_MEMBER(_int,AddExperienceRadius);
		REG_MEMBER(_bool,EnableCollectExpa);

		REG_MEMBER(_int,Knowledge);
		REG_MEMBER(_bool,PassSelectHeroParametr);
		REG_MEMBER(_bool,SelectHeroParamState);
		REG_AUTO(Param);

		REG_MEMBER(_int,UpLifeRegeneration);
		REG_MEMBER(_int,LastUpLifeRegenerationTime);
		REG_MEMBER(_int,UpManaRegeneration);
		REG_MEMBER(_int,LastUpManaRegenerationTime);

		//REG_MEMBER(_bool,IsTired);
		//int SpeedMinus;
		//int freePlaceCard;

		REG_AUTO(Variables);

		//int DieTime;
		//int ReBornTime;
		//CardList coloda;
		REG_MEMBER(_int,indexx);
		//int colodaPointer;
		//int CardRegeneration;

		REG_MEMBER(_int,Lives);
		REG_MEMBER(_int,AntiMageLevel);
	}ENDSAVE;

	int* GetVarRef(const char* Name);

	int GatherExperience(OneObject* Victim,word Killer);
	virtual bool Process();

	void AddChooseUpHeroParam(int UpHeroParamIndex);
	void UpLevel();
	int GetExperienceToNextLevel();

private:
	void CreateOptionUpHeroParams();
	void CreateLevelUpParam();
};
HeroVariableStorage* DetectHero(OneObject* OB);
//==================================================================================================================//
class CHeroesCollector :public BaseClass
{
public:
	CHeroesCollector();
	//DynArray<HeroVariableStorage*> Herosima;
	DynArray<word> HeroID;
	DynArray<word> HeroSN;
	void OnDieProcess(OneObject* Victim,word Killer);
	void Reset();
};
//==================================================================================================================//
class CUnitExperienceParm : public BaseClass
{
public:
	CUnitExperienceParm();
	int LifeK;
	int DamageK;
	int RangeK;
	int SpeedK;
	SAVE(CUnitExperienceParm)
		REG_MEMBER(_int,LifeK);
		REG_MEMBER(_int,DamageK);
		REG_MEMBER(_int,RangeK);
		REG_MEMBER(_int,SpeedK);
	ENDSAVE;
};

#endif