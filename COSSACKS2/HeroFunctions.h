//////////////////////////////////////////////////////////////////////////
#pragma once
#include "supereditor.h"
//////////////////////////////////////////////////////////////////////////
class GetHeroVariable: public NumericalReturner
{
public:
	GetHeroVariable();
	//ClassPtr<StringReturner> Op;
	_str StrParam;
	SAVE(GetHeroVariable)
		REG_PARENT(BoolReturner);
		REG_PARENT(Returner);
		REG_AUTO(StrParam);
	ENDSAVE

	virtual void GetViewMask(DString& ST) { ST.Add("Hero:");ST.Add(StrParam.pchar()); };
	virtual int GetNArguments() { return 1; };
	virtual Operand* GetArgument(int Index) { return NULL; };
	virtual void SetArgument(int index, Operand* Op);
	virtual bool Get(BaseType* BT);
	virtual const char* GetFunctionName() { return "Hero"; };
};
class BasicHeroParams:public NumericalReturner{
public:
	virtual void GetViewMask(DString& ST) { ST.Add(GetClassName());ST.Add("()"); };
	virtual int GetNArguments() { return 0; };	
	virtual const char* GetFunctionName() { return GetClassName(); };    
};
class AcceptorHeroLevel:public BasicHeroParams{
public:
	SAVE(AcceptorHeroLevel);
	REG_PARENT(NumericalReturner);
	REG_PARENT(Returner);
	ENDSAVE;
	virtual bool Get(BaseType* BT);
};
class CasterHeroLevel:public BasicHeroParams{
public:
	SAVE(CasterHeroLevel);
	REG_PARENT(NumericalReturner);
	REG_PARENT(Returner);
	ENDSAVE;
	virtual bool Get(BaseType* BT);
};
class AcceptorTrueLevel:public BasicHeroParams{
public:
	SAVE(AcceptorTrueLevel);
	REG_PARENT(NumericalReturner);
	REG_PARENT(Returner);
	ENDSAVE;
	virtual bool Get(BaseType* BT);
};
class CasterTrueLevel:public BasicHeroParams{
public:
	SAVE(CasterTrueLevel);
	REG_PARENT(NumericalReturner);
	REG_PARENT(Returner);
	ENDSAVE;
	virtual bool Get(BaseType* BT);
};
class AcceptorMana:public BasicHeroParams{
public:
	SAVE(AcceptorMana);
	REG_PARENT(NumericalReturner);
	REG_PARENT(Returner);
	ENDSAVE;
	virtual bool Get(BaseType* BT);
};
class CasterMana:public BasicHeroParams{
public:
	SAVE(CasterMana);
	REG_PARENT(NumericalReturner);
	REG_PARENT(Returner);
	ENDSAVE;
	virtual bool Get(BaseType* BT);
};
class AcceptorLife:public BasicHeroParams{
public:
	SAVE(AcceptorLife);
	REG_PARENT(NumericalReturner);
	REG_PARENT(Returner);
	ENDSAVE;
	virtual bool Get(BaseType* BT);
};
class CasterLife:public BasicHeroParams{
public:
	SAVE(CasterLife);
	REG_PARENT(NumericalReturner);
	REG_PARENT(Returner);
	ENDSAVE;
	virtual bool Get(BaseType* BT);
};
class AcceptorMaxLife:public BasicHeroParams{
public:
	SAVE(AcceptorMaxLife);
	REG_PARENT(NumericalReturner);
	REG_PARENT(Returner);
	ENDSAVE;
	virtual bool Get(BaseType* BT);
};
class CasterMaxLife:public BasicHeroParams{
public:
	SAVE(CasterMaxLife);
	REG_PARENT(NumericalReturner);
	REG_PARENT(Returner);
	ENDSAVE;
	virtual bool Get(BaseType* BT);
};
class AcceptorAbilityCasts:public BasicHeroParams{
public:
	ClassRef<UnitAbility> Ability;
	SAVE(AcceptorAbilityCasts);
	REG_PARENT(NumericalReturner);
	REG_PARENT(Returner);
	REG_AUTO(Ability);
	ENDSAVE;
	virtual bool Get(BaseType* BT);
	virtual void GetViewMask(DString& ST);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags);
};
class CasterAbilityCasts:public BasicHeroParams{
public:
	ClassRef<UnitAbility> Ability;
	SAVE(CasterAbilityCasts);
	REG_PARENT(NumericalReturner);
	REG_PARENT(Returner);
	REG_AUTO(Ability);
	ENDSAVE;
	virtual bool Get(BaseType* BT);
	virtual void GetViewMask(DString& ST);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags);
};

//class AM_Level_Skill_A : public BasicHeroParams {
//public:
//	SAVE(AM_Level_Skill_A){
//		REG_PARENT(NumericalReturner);
//		REG_PARENT(Returner);
//	}ENDSAVE;
//	virtual bool Get(BaseType* BT);
//};
//class AM_Level_Skill_B : public BasicHeroParams {
//public:
//	SAVE(AM_Level_Skill_B){
//		REG_PARENT(NumericalReturner);
//		REG_PARENT(Returner);
//	}ENDSAVE;
//	virtual bool Get(BaseType* BT);
//};

class AntiMageSkillLevel : public BasicHeroParams {
public:
	ClassRef<UnitAbility> RefUA;
	SAVE(AntiMageSkillLevel){
		REG_PARENT(NumericalReturner);
		REG_PARENT(Returner);
		REG_AUTO(RefUA);
	}ENDSAVE;
	virtual bool Get(BaseType* BT);
	virtual void GetViewMask(DString& ST);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags);
};
class BHP_Damage : public BasicHeroParams {
public:
	int Activator;
	SAVE(BHP_Damage){
		REG_PARENT(NumericalReturner);
		REG_PARENT(Returner);
		REG_ENUM(_index,Activator,HeroParamsActivator);
	}ENDSAVE;
	virtual bool Get(BaseType* BT);
	virtual void GetViewMask(DString& ST);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags);
};
class BHP_AbilityEffectTime : public BasicHeroParams {
public:
	int Activator;
	SAVE(BHP_AbilityEffectTime){
		REG_PARENT(NumericalReturner);
		REG_PARENT(Returner);
		REG_ENUM(_index,Activator,HeroParamsActivator);
	}ENDSAVE;
	virtual bool Get(BaseType* BT);
	virtual void GetViewMask(DString& ST);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags);
};
