#ifndef ua_Settlement_ua
#define ua_Settlement_ua
#pragma once
//////////////////////////////////////////////////////////////////////////
#include "unitability.h"
#include "ua_Ability.h"

class ua_Settlement: public UnitAbility{
public:
	_str ScriptName;
	//ClassRef<be_CSMARTOBJECT> SObj;
	
	SAVE(ua_Settlement){
		REG_PARENT(UnitAbility);
		REG_AUTO(ScriptName);
		//REG_AUTO(SObj);
	}ENDSAVE;

	virtual ActiveUnitAbility* GetNewActiveAbility();
};
class uaa_Settlement: public ActiveUnitAbility{
public:
	word HeroID;
	word HeroSN;

	SAVE(uaa_Settlement){
		REG_PARENT(ActiveUnitAbility);
	}ENDSAVE;

	virtual bool Process(){
		return true;
	};
};

//

class ua_BeMsg: public NewAbility {
public:
	SAVE(ua_BeMsg){
		REG_PARENT(NewAbility);
	}ENDSAVE;

	ua_BeMsg(){
		Button=true;
	}
	virtual ActiveUnitAbility* GetNewActiveAbility();
	virtual void SendMsg(ActiveUnitAbility* AA) {}
};
class uaa_BeMsg: public OneAbility {
public:
	SAVE(uaa_BeMsg){
		REG_PARENT(OneAbility);
	}ENDSAVE;

	uaa_BeMsg(){
		TypeAbil=7;		
	}
	virtual bool OnClick();
	virtual bool Process();
	virtual const char* GetHint();
};


//////////////////////////////////////////////////////////////////////////
#endif