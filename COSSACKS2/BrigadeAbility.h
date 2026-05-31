#pragma once
//==================================================================================================================//
class BrigadeIconInfo:public BaseClass{
public:
	word FileID;
	int SpriteID;
	int CenterX;
	int CenterY;
	int MessageX;
	int MessageY;
	int LeftMargin;
	int IconWidth;
	int Value;
	_str HintID;
	SAVE(BrigadeIconInfo);
		REG_MEMBER(_gpfile,FileID);
		REG_SPRITE(SpriteID,FileID);
		REG_MEMBER(_int,CenterX);
		REG_MEMBER(_int,CenterY);
		REG_MEMBER(_int,MessageX);
		REG_MEMBER(_int,MessageY);
		REG_MEMBER(_int,LeftMargin);
		REG_MEMBER(_int,IconWidth);
		REG_MEMBER(_int,Value);
		REG_MEMBER(_str,HintID);
	ENDSAVE;
};
//==================================================================================================================//
class ActiveBrigadeAbility: public ActiveUnitAbility
{
public:
	ActiveBrigadeAbility();
	Brigade* Br;
	int NI;
	int BrigadeIndex;

	SAVE(ActiveBrigadeAbility)
		REG_PARENT(ActiveUnitAbility)
		REG_MEMBER(_int,NI);
		REG_MEMBER(_int,BrigadeIndex);
	ENDSAVE;

	virtual bool Process(Brigade* B);
	virtual BrigadeIconInfo* GetAbilityIcon();
	virtual int GetAddDamage();
	virtual int GetAddShield(); 
};
//==================================================================================================================//
class BrigadeActiveAbilityList : public ClassArray<ActiveBrigadeAbility>
{
public:
	//UnitActiveAbilityList(void);
	//ClassArray<ActiveBrigadeAbility> ActiveAbilities;
	SAVE(BrigadeActiveAbilityList)
		//REG_AUTO(ActiveAbilities);
	ENDSAVE;
	bool Process(Brigade* B);
};
//==================================================================================================================//
class BrigadeChangeMoral : public UnitAbility2
{
public:
	BrigadeChangeMoral();
	int AddMoral;
	int Radius;
	BrigadeIconInfo IconInfo;
	int AddMoralForLevel;
	SAVE(BrigadeChangeMoral)
		REG_BASE(UnitAbility);
		REG_PARENT(UnitAbility2);
		REG_MEMBER(_int,AddMoral);
		REG_MEMBER(_int,Radius);
		REG_AUTO(IconInfo);
		REG_MEMBER(_int,AddMoralForLevel);
	ENDSAVE;
	virtual bool OnUnitBirth(OneObject* Newbie);
};
//==================================================================================================================//
class ActiveBrigadeChangeMoral : public LeveledActiveUnitAbility
{
public:
	ActiveBrigadeChangeMoral();
	SAVE(ActiveBrigadeChangeMoral)
		REG_BASE(ActiveUnitAbility);
		REG_PARENT(LeveledActiveUnitAbility);
	ENDSAVE;
	int LastCheckTime;
	virtual bool Process();
	virtual bool ShowRadius();
	virtual int GetHintParamOnLevel(int Lev);
private:
	static bool SetActiveAbil(OneObject* OB, void* Param);
};
//==================================================================================================================//
class ActiveChangedMoralForBrigade : public ActiveBrigadeAbility
{
public:
	ActiveChangedMoralForBrigade();
	bool Leader;
	int HeroIndex;
	int Radius;
	int MoralAdd;
	Brigade* Br;
	ClassRef<BrigadeChangeMoral> ChangeMoral;
	SAVE(ActiveChangedMoralForBrigade)
		REG_PARENT(ActiveBrigadeAbility);
		REG_AUTO(ChangeMoral);
	ENDSAVE;
	int LastCheckTime;
	virtual bool Process(Brigade* B);
	virtual BrigadeIconInfo* GetAbilityIcon();
};
//==================================================================================================================//
class BrigadeTimeChangeMoral : public UnitAbility2
{
public:
	BrigadeTimeChangeMoral();
	int AddMoral;
	int Radius;
	int CoolDownTime;
	int EffectTime;
	BrigadeIconInfo IconInfo;
	int MoralAddForLevel;
	SAVE(BrigadeTimeChangeMoral)
		REG_BASE(UnitAbility); 
		REG_PARENT(UnitAbility2);
		REG_MEMBER(_int,AddMoral);
		REG_MEMBER(_int,Radius);
		REG_MEMBER(_int,CoolDownTime);
		REG_MEMBER(_int,EffectTime);
		REG_AUTO(IconInfo);
		REG_MEMBER(_int,MoralAddForLevel);
	ENDSAVE;
	virtual bool OnUnitBirth(OneObject* Newbie);
};
//==================================================================================================================//
class ActiveBrigadeTimeChangeMoral : public LeveledActiveUnitAbility
{
public:
	ActiveBrigadeTimeChangeMoral();
	
	SAVE(ActiveBrigadeTimeChangeMoral)
		REG_BASE(ActiveUnitAbility);
		REG_PARENT(LeveledActiveUnitAbility);
	ENDSAVE;

	int LastUseTime;

	virtual bool OnClick();
	virtual bool CanApply();
	virtual bool Process();
	virtual bool ShowRadius();
	virtual int GetCoolDownProc();
	virtual int GetHintParamOnLevel(int Lev);
private:
	static bool SetActiveAbil(OneObject* OB, void* Param);
};
//==================================================================================================================//
class ActiveTimeChangedMoralForBrigade : public ActiveBrigadeAbility
{
public:
	ActiveTimeChangedMoralForBrigade();
	Brigade* Br;
	SAVE(ActiveTimeChangedMoralForBrigade)
		REG_PARENT(ActiveBrigadeAbility);
		REG_MEMBER(_int,SetTime);
		REG_MEMBER(_int,EffectTime);
		REG_MEMBER(_int,AddMoral);
	ENDSAVE;

	int SetTime;
	int EffectTime;
	int AddMoral;

	virtual bool Process(Brigade* B);
	virtual BrigadeIconInfo* GetAbilityIcon();
};
//==================================================================================================================//
class AddBrigadeBonus : public UnitAbility2
{
public:
	AddBrigadeBonus();
	int Radius;
	int AddDamage;
	int AddShield;
	int AddAttackRange;
	int UnitsSpeedBonus;
	int ExpGrowSpeed;
	int AttackRateBonus;
	int CureUnits;
	int AddMaxLife;
	bool EnemyTarget;
	int CoolDownTime;
	int EffectTime;
	int UsePause;
	UnitTypeList UnitType;
	BrigadeIconInfo IconInfo;
	int AddForLevel;
	SAVE(AddBrigadeBonus)
		REG_BASE(UnitAbility);
		REG_PARENT(UnitAbility2);
		REG_MEMBER(_int,Radius);
		REG_MEMBER(_int,AddDamage);
		REG_MEMBER(_int,AddShield);
		REG_MEMBER(_int,AddAttackRange);
		REG_MEMBER(_int,UnitsSpeedBonus);
		REG_MEMBER(_int,ExpGrowSpeed);
		REG_MEMBER(_int,AttackRateBonus);
		REG_MEMBER(_int,CureUnits);
		REG_MEMBER(_int,AddMaxLife);
		REG_MEMBER(_bool,EnemyTarget);
		REG_MEMBER(_int,CoolDownTime);
		REG_MEMBER(_int,EffectTime);
		REG_MEMBER(_int,UsePause);
		REG_AUTO(UnitType);
		REG_AUTO(IconInfo);
		REG_MEMBER(_int,AddForLevel);
	ENDSAVE;
	virtual ActiveUnitAbility* GetNewActiveAbility();
};
//==================================================================================================================//
class ActiveAddBrigadeBonus : public LeveledActiveUnitAbility
{
public:
	ActiveAddBrigadeBonus();
	SAVE(ActiveAddBrigadeBonus)
		REG_BASE(ActiveUnitAbility);
		REG_PARENT(LeveledActiveUnitAbility);
	ENDSAVE;

	virtual bool Process();
	virtual bool CanApply();
	virtual int GetCoolDownProc();
	virtual bool ShowRadius();
	virtual int GetRadius();
	virtual bool OnClick();
	virtual int GetHintParamOnLevel(int Lev);
private:
	int LastUseTime;
	bool Exec();
	static bool SetBonus(OneObject* OB, void* Param);
};
//==================================================================================================================//
class ActiveBrigadeBonus : public ActiveBrigadeAbility
{
public:
	ActiveBrigadeBonus();
	~ActiveBrigadeBonus();
	SAVE(ActiveBrigadeBonus)
		REG_PARENT(ActiveBrigadeAbility);
	ENDSAVE;

	Brigade* ActBr;

	int AddDamage;
	int AddShield;
	int AddAttackRange;
	int UnitsSpeedBonus;
	int ExpGrowSpeed;
	int AttackRateBonus;
	int CureUnits;
	int AddMaxLife;

	int SetTime;
	int EffectTime;
	int OneSecondTime;
	int HeroIndex;
	virtual bool Process(Brigade* B);
	virtual BrigadeIconInfo* GetAbilityIcon();
	virtual int GetAddDamage();
	virtual int GetAddShield(); 
};
//==================================================================================================================//