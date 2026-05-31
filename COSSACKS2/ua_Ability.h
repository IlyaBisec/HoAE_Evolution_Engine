#pragma once
//#include "unitability.h"
#include "HeroAbility.h"
//==================================================================================================================//
//class NewMonster;
//==================================================================================================================//
class NewAbility: public MagicCard{
private:
	int AutoCastNextTime;	
public:

	SAVE(NewAbility){
		//REG_BASE(UnitAbility);
		REG_PARENT(UnitAbility);
		//REG_AUTO(MagicCardSection);
		//REG_PARENT(MagicCard);
	}ENDSAVE;

	NewAbility();	
	virtual ActiveUnitAbility* GetNewActiveAbility();
	bool OnUnitBirth(OneObject* Newbie){
		return UnitAbility::OnUnitBirth(Newbie);
	};

};
class OneAbility: public MagicCardActive{
public:
	//int eAuraStartTime;
private:
	int CurShot; // щетчик выстрелов оружия
	//int Tx,Ty,Tz;
	//word tIndex, tSerial;
	bool overWater;
public:	
	//byte eHandler;
	__constructor(OneAbility){
		TypeAbil=1;
		Casting=false;

		CurShot=0;
		Tx=0; Ty=0; Tz=0;
		tIndex=0xFFFF;
		tSerial=0;
		
		AutoCast=false;
		ExecuteTime=0;
		overWater = false;
	};

	// BE scripts

	be_CRefParamFunction be_Process;
	be_CRefParamFunction be_LeftClick;
	be_CRefParamFunction be_EnterDialog;

	SAVE(OneAbility){
		REG_PARENT(MagicCardActive);

		REG_MEMBER(_int,CurShot);
		REG_MEMBER(_int,Tx);
		REG_MEMBER(_int,Ty);
		REG_MEMBER(_int,Tz);
		REG_MEMBER(_WORD,tIndex);
		REG_MEMBER(_WORD,tSerial);

		REG_AUTO(be_Process);
		REG_AUTO(be_LeftClick);
		REG_AUTO(be_EnterDialog);
	}ENDSAVE;
	
	virtual bool Process();
	virtual bool isTarget();

	virtual bool OnClick();
	virtual bool OnRightClick();
	virtual bool Execute(word TargetUnit, int x, int y, int z);

	virtual bool DrawCursor(int x,int y);
	virtual bool CanTarget(word TargetUnit, int x, int y, int z);

	//virtual bool CanApply();
	virtual int GetCoolDownProc();
	virtual void StartCoolDown();
	//
	virtual NewAnimation* GetAuraAnimation();
	virtual bool CheckAnimationMask();

	virtual void CastingSpell();
	virtual void DropDown(int x=-1, int y=-1);

};

///////////////////
// Image & Effect
///////////////////

class ua_Image: public NewAbility {
public:
	
	SAVE(ua_Image){
	}ENDSAVE;

	virtual ActiveUnitAbility* GetNewActiveAbility();

};
class aa_Image: public OneAbility {
public:
	
	SAVE(aa_Image){
	}ENDSAVE;

};
class ua_Effect: public NewAbility {
public:
	
	SAVE(ua_Effect){
	}ENDSAVE;

	virtual ActiveUnitAbility* GetNewActiveAbility();

};
class aa_Effect: public OneAbility {
public:
	
	SAVE(aa_Effect){
	}ENDSAVE;

};
