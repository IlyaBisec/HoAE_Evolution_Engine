#pragma once
#include "stdheader.h"
#include "GameInterface.h"
#include "UnitAbility.h"
#include "unitability.h"
#include "NewMonster.h"
#include "OneSprite.h"
#include "vui_Action.h"
#include "vui_InterfSynchro.h"
#include "GameExtension.h"
#include "UnitsInterface.h"
#include "PluginsInterface.h"
//#include "cvi_HeroButtons.h"
//#define GUID_VUI_IExchange 0x12340001
//class VUI_IExchange: public IExchangeInterface {
//public:	
//	virtual DWORD GetUniqID() { return GUID_VUI_IExchange; }
//
//	//virtual void SetUnitInfo(OneObject* OB, int* Price) = 0;
//	virtual void AddOnUnitInfoArray(OneObject* OB) = 0;
//};
class suAI_Ability: public BaseClass{
public:
	ClassRef<UnitAbility> UA;
	int MinTargetAmount;
	bool TargetFriendly;
	bool TargetEnemy;
	bool TargetHero;
	int CastingDelay;
	bool Finite;
	bool Continuous;
	int DeleteTime;
	bool NeedMatherial;
	int Matherial;
	//int CastingRadius;
	
	SAVE(suAI_Ability){
		REG_AUTO(UA);
		REG_MEMBER(_int,MinTargetAmount);
		REG_MEMBER(_bool,TargetFriendly);
		REG_MEMBER(_bool,TargetEnemy);
		REG_MEMBER(_bool,TargetHero);
		REG_MEMBER(_bool,Finite);
		REG_MEMBER(_bool,NeedMatherial);
		REG_ENUM(_index,Matherial,Matherials);
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