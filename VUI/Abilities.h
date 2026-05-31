#pragma once

class suAI_Ability : public BaseClass {
public:
	
	ClassRef<UnitAbility> UA;
	int MinTargetAmount;
	bool TargetFriendly;
	bool TargetEnemy;
	bool TargetEnemyHero;
	bool TargetFriendlyHero;
	int CastingDelay;
	bool Finite;
	bool Continuous;
	int DeleteTime;
	//int CastingRadius;
	
	SAVE(suAI_Ability){
		REG_AUTO(UA);
		REG_MEMBER(_int,MinTargetAmount);
		REG_MEMBER(_bool,TargetFriendly);
		REG_MEMBER(_bool,TargetEnemy);
		REG_MEMBER(_bool,TargetEnemyHero);
		REG_MEMBER(_bool,TargetFriendlyHero);
		REG_MEMBER(_bool,Finite);
		REG_MEMBER(_int,CastingDelay);
		//REG_MEMBER(_int,CastingRadius);
	}ENDSAVE;

	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="AI_Ability %$UA$%, MinTargetAmount=%$MinTargetAmount$%";
		return true;
	}

};

class suAI_Abilities : public ClonesArray<suAI_Ability> {
public:
	
	SAVE(suAI_Abilities){
		REG_PARENT(ClonesArray<suAI_Ability>);
	}ENDSAVE;

	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="Abilities:";
		if(!GameInterface::ClassEditorCheckIfOpen(this)){
			dest+="\n%$this,  \n$%";
		}
		return true;
	}

};

class ua_SingleUnitAI : public UnitAbility {
public:
	
	suAI_Abilities Abilities;
	int CastingRadius;
	
	SAVE(ua_SingleUnitAI){
		REG_BASE(UnitAbility);
		REG_AUTO(Name);
		REG_MEMBER(_int,CastingRadius);
		REG_AUTO(Abilities);
	}ENDSAVE;

	ua_SingleUnitAI();
	virtual ActiveUnitAbility* GetNewActiveAbility();
	//virtual bool ForceSimplification() { return false; }

	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		//if(!GameInterface::ClassEditorCheckIfOpen(this)){
		//	dest="SingleUnitAI: CastingRadius=%$CastingRadius$%\n      %$Abilities,  \n$%";
		//}else{
		//	dest="SingleUnitAI";
		//}
		return true;
	}

};
class HeroVariableStorage;


class aa_SingleUnitAI : public ActiveUnitAbility {
public:
	
	bool mInit; // инициализация на первом такте
	bool FirstCast;
	int FirstCastTryTime;
	int NextCastTime;
	LinearArray<int,_int> aNextCastTime;

	SAVE(aa_SingleUnitAI){
		REG_PARENT(ActiveUnitAbility);
		REG_MEMBER(_bool,mInit);
		REG_MEMBER(_bool,FirstCast);
		REG_MEMBER(_int,FirstCastTryTime);
		REG_MEMBER(_int,NextCastTime);
		REG_AUTO(aNextCastTime);
	}ENDSAVE;

	aa_SingleUnitAI();
	virtual bool Process();

};