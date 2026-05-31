//#pragma once

#ifndef __UnitModificator_H__
#define __UnitModificator_H__

#include "UnitAbility.h"
#include "GameInterface.h"
#include "ua_Item.h"
extern int TrueTime;

extern char* WeaponIDS[32];
ActiveUnitAbility* AddOneEffect(WeaponParams* WP, UnitAbility* UA, OneObject* Dest, OneObject* Caster);
extern bool InAllEditors;
bool DeleteCarryAbility(OneObject* OB, UnitAbility* UA);

//============ Modificators ===================

class umc_Complex: public UnitModificatorCondition{
public:
	UnitModificatorConditionArray Complex;

	SAVE(umc_Complex){
		REG_PARENT(UnitModificatorCondition);
		REG_AUTO(Complex);
	}ENDSAVE;

	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="";
		if(Not) dest+="! ";
		dest+="%$Complex, and\n$%";
		return true;
	}
	virtual bool Check(ActiveUnitAbility* AA);
};
////////////////////////
class umc_DirectionDifference: public UnitModificatorCondition{
public:

	byte Direction;
	byte Difference;

	SAVE(umc_DirectionDifference){
		REG_PARENT(UnitModificatorCondition);
		REG_MEMBER(_BYTE,Direction);
		REG_MEMBER(_BYTE,Difference);
	}ENDSAVE;

	umc_DirectionDifference(){
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="is Direction=%$Direction$% Difference=%$Difference$% between Acceptor and Caster";
		return true;
	}
	virtual bool Check (ActiveUnitAbility* AA);
};
//umc_DamageType
////////////////////////
class umc_Ready: public UnitModificatorCondition{
public:

	SAVE(umc_Ready){
		REG_PARENT(UnitModificatorCondition);
	}ENDSAVE;

	umc_Ready(){
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="Ready";
		return true;
	}
	virtual bool Check (ActiveUnitAbility* AA);
};
class umc_HeroAtHome: public UnitModificatorCondition{
public:

	SAVE(umc_HeroAtHome){
		REG_PARENT(UnitModificatorCondition);
	}ENDSAVE;

	umc_HeroAtHome(){
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="HeroAtHome";
		return true;
	}
	virtual bool Check (ActiveUnitAbility* AA);
};
class umc_TimePeriod: public UnitModificatorCondition{
public:
	int StartTime;
	bool Unlimited;
	int Duration;

	SAVE(umc_TimePeriod){
		REG_PARENT(UnitModificatorCondition);
		REG_MEMBER(_int,StartTime);
		REG_MEMBER(_bool,Unlimited);
		REG_MEMBER(_int,Duration);
	}ENDSAVE;

	umc_TimePeriod(){
		Unlimited=true;
		Duration=1000;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="";
		if(Not) dest+="! ";
		if(Unlimited){
			dest+="Start time: %$StartTime$% ms, Unlimited duration";
		}else{
			dest+="Start time: %$StartTime$% ms, Duration: %$Duration$% ms";
		}
		return true;
	}
	virtual bool Check (ActiveUnitAbility* AA);
};
class umc_MotionStyle: public UnitModificatorCondition{
public:
	int Style;

	SAVE(umc_MotionStyle){
		REG_PARENT(UnitModificatorCondition);
		REG_ENUM(_index,Style,MOTIONSTYLE);
	}ENDSAVE;

	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(Not){
			dest="Motion style != %$Style$%";
		}else{
			dest="Motion style = %$Style$%";
		}
		return true;
	}
	virtual bool Check(ActiveUnitAbility* AA);
};
class umc_LandTypeInPoint: public UnitModificatorCondition{
public:
	int LandType;

	SAVE(umc_LandTypeInPoint){
		REG_PARENT(UnitModificatorCondition);
		REG_ENUM(_index,LandType,LANDLAYERS);
	}ENDSAVE;

	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(Not){
			dest="LandType != %$LandType$%";
		}else{
			dest="LandType = %$LandType$%";
		}
		return true;
	}
	virtual bool Check(ActiveUnitAbility* AA);
};
class umc_UnitType: public UnitModificatorCondition{
private:
	bool init;
	bool type[2048];
public:
	ClonesArray<BaseClassUnitType> Type;

	SAVE(umc_UnitType){
		REG_PARENT(UnitModificatorCondition);
		REG_AUTO(Type);
	}ENDSAVE;

	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(Not){
			dest="Unit type != ";
		}else{
			dest="Unit type = ";
		}
		if(Type.GetAmount()){
			if(!ClassEditor::CheckIfOpen(this)){
				dest+="%$Type, $%";
			}			
		}else{
			dest+="Empty";
		}
		return true;
	}
	virtual bool Check(ActiveUnitAbility* AA);
};
class umc_IsMoving: public UnitModificatorCondition{
private:	
public:

	SAVE(umc_IsMoving){
		REG_PARENT(UnitModificatorCondition);		
	}ENDSAVE;
	
	virtual bool Check(ActiveUnitAbility* AA);
};
class umc_OverWater: public UnitModificatorCondition{
private:	
public:

	SAVE(umc_OverWater){
		REG_PARENT(UnitModificatorCondition);		
	}ENDSAVE;
	
	virtual bool Check(ActiveUnitAbility* AA);
};
class umc_CasterUnitType: public UnitModificatorCondition{
public:
	ClonesArray<BaseClassUnitType> Type;

	SAVE(umc_CasterUnitType){
		REG_PARENT(UnitModificatorCondition);
		REG_AUTO(Type);
	}ENDSAVE;

	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(Not){
			dest="Caster unit type != ";
		}else{
			dest="Caster unit type = ";
		}
		if(Type.GetAmount()){
			if(!ClassEditor::CheckIfOpen(this)){
				dest+="%$Type, $%";
			}			
		}else{
			dest+="Empty";
		}
		return true;
	}
	virtual bool Check(ActiveUnitAbility* AA);
};
class umc_FriendlyToCaster: public UnitModificatorCondition{
public:
	SAVE(umc_FriendlyToCaster){
		REG_PARENT(UnitModificatorCondition);
	}ENDSAVE;

	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(Not){
			dest="! FriendlyToCaster";
		}else{
			dest="FriendlyToCaster";
		}
		return true;
	}
	virtual bool Check(ActiveUnitAbility* AA);
};
class umc_Caster: public UnitModificatorCondition{
public:
	SAVE(umc_Caster){
		REG_PARENT(UnitModificatorCondition);
	}ENDSAVE;

	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(Not){
			dest="! Caster";
		}else{
			dest="Caster";
		}
		return true;
	}
	virtual bool Check(ActiveUnitAbility* AA);
};
class umc_CasterLevel: public UnitModificatorCondition{
public:
	int Level;

	SAVE(umc_CasterLevel){
		REG_PARENT(UnitModificatorCondition);
		REG_MEMBER(_int,Level);
	}ENDSAVE;

	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(Not){
			dest="CasterLevel < %$Level$%";
		}else{
			dest="CasterLevel >= %$Level$%";
		}		
		return true;
	}
	virtual bool Check(ActiveUnitAbility* AA);
};
class umc_Summon: public UnitModificatorCondition{
public:
	SAVE(umc_Summon){
		REG_PARENT(UnitModificatorCondition);
	}ENDSAVE;

	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(Not){
			dest="! Summon";
		}else{
			dest="Summon";
		}
		return true;
	}
	virtual bool Check(ActiveUnitAbility* AA);
};
class umc_Frozen: public UnitModificatorCondition{
public:
	SAVE(umc_Frozen){
		REG_PARENT(UnitModificatorCondition);
	}ENDSAVE;

	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(Not){
			dest="! Frozen";
		}else{
			dest="Frozen";
		}
		return true;
	}
	virtual bool Check(ActiveUnitAbility* AA);
};
class umc_Incorporeal: public UnitModificatorCondition{
public:
	SAVE(umc_Incorporeal){
		REG_PARENT(UnitModificatorCondition);
	}ENDSAVE;

	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(Not){
			dest="! Incorporeal";
		}else{
			dest="Incorporeal";
		}
		return true;
	}
	virtual bool Check(ActiveUnitAbility* AA);
};
class umc_UpgradeIsDone: public UnitModificatorCondition{
public:
	int UpgradeID;
	SAVE(umc_UpgradeIsDone){
		REG_PARENT(UnitModificatorCondition);
		REG_ENUM(_index,UpgradeID,ALL_UPGRADES);
	}ENDSAVE;

	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(Not){
			dest="! UpgradeIsDone(%$UpgradeID$%)";
		}else{
			dest="UpgradeIsDone(%$UpgradeID$%)";
		}
		return true;
	}
	virtual bool Check(ActiveUnitAbility* AA);
};

class umc_Hero: public UnitModificatorCondition{
public:

	SAVE(umc_Hero){
		REG_PARENT(UnitModificatorCondition);
	}ENDSAVE;

	umc_Hero(){
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(Not){
			dest="! Hero";
		}else{
			dest="Hero";
		}
		return true;
	}
	virtual bool Check (ActiveUnitAbility* AA);
};
class umc_Building: public UnitModificatorCondition{
public:

	SAVE(umc_Building){
		REG_PARENT(UnitModificatorCondition);
	}ENDSAVE;

	umc_Building(){
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(Not){
			dest="! Building";
		}else{
			dest="Building";
		}
		return true;
	}
	virtual bool Check (ActiveUnitAbility* AA);
};
class umc_Hidden: public UnitModificatorCondition{
public:

	SAVE(umc_Hidden){
		REG_PARENT(UnitModificatorCondition);
	}ENDSAVE;

	umc_Hidden(){
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(Not){
			dest="! Hidden";
		}else{
			dest="Hidden";
		}
		return true;
	}
	virtual bool Check (ActiveUnitAbility* AA);
};
class umc_LiveBody: public UnitModificatorCondition{
public:

	SAVE(umc_LiveBody){
		REG_PARENT(UnitModificatorCondition);
	}ENDSAVE;

	umc_LiveBody(){
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(Not){
			dest="! LiveBody";
		}else{
			dest="LiveBody";
		}
		return true;
	}
	virtual bool Check (ActiveUnitAbility* AA);
};

class umc_LifeLess: public UnitModificatorCondition{
public:

	int Value;
	int ValueType;

	SAVE(umc_LifeLess){
		REG_PARENT(UnitModificatorCondition);
		REG_MEMBER(_int,Value);
		REG_ENUM_EX(_index,ValueType,OperandValueType,OperandType);
	}ENDSAVE;

	umc_LifeLess(){
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest.Clear();
		if(Not)
			dest="! ";
		dest+="LifeLess than %$Percent$% percent";
		return true;
	}
	virtual bool Check (ActiveUnitAbility* AA);
};

class umc_LockType: public UnitModificatorCondition{
public:

	int LockType;

	SAVE(umc_LockType){
		REG_PARENT(UnitModificatorCondition);
		REG_ENUM(_index,LockType,LOCKTYPE);
	}ENDSAVE;

	umc_LockType(){
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(Not){
			dest="LockType!=%$LockType$%";
		}else{
			dest="LockType=%$LockType$%";
		}
		return true;
	}
	virtual bool Check (ActiveUnitAbility* AA);
};
class umc_AttackRadiusMax: public UnitModificatorCondition{
public:

	int Compare;
	int Radius;
	bool Caster;

	SAVE(umc_AttackRadiusMax){
		REG_PARENT(UnitModificatorCondition);
		REG_ENUM(_index,Compare,OPTYPE);
		REG_MEMBER(_int,Radius);
		REG_MEMBER(_bool,Caster);
	}ENDSAVE;

	umc_AttackRadiusMax(){
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(Not){
			dest="AttackRadiusMax not %$Compare$% %$Radius$%";
		}else{
			dest="AttackRadiusMax %$Compare$% %$Radius$%";
		}
		return true;
	}
	virtual bool Check (ActiveUnitAbility* AA);
};

class umc_WeaponKind: public UnitModificatorCondition{
public:

	int Kind;

	SAVE(umc_WeaponKind){
		REG_PARENT(UnitModificatorCondition);
		REG_ENUM(_index,Kind,WeaponIDS);
	}ENDSAVE;

	umc_WeaponKind(){
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(Not){
			dest="WeaponKind not %$Kind$%";
		}else{
			dest="WeaponKind %$Kind$%";
		}
		return true;
	}
	virtual bool Check (ActiveUnitAbility* AA);
};

class umc_FindTargetNearCaster: public UnitModificatorCondition{
public:

	UserFriendlyNumericalReturner Radius;

	Delimiter Side;
	bool Friend;
	bool Enemy;

	Delimiter Structure;
	bool Units;
	bool Buildings;

	Delimiter Vitality;
	bool LiveBody;
	bool DeadBody;
	bool Bones;

	Delimiter Rules;
	bool Wounded;

	SAVE(umc_FindTargetNearCaster){

		REG_PARENT(UnitModificatorCondition);
		
		REG_AUTO(Radius);

		REG_AUTO(Side);
		REG_MEMBER(_bool,Friend);
		REG_MEMBER(_bool,Enemy);

		REG_AUTO(Structure);
		REG_MEMBER(_bool,Units);
		REG_MEMBER(_bool,Buildings);

		REG_AUTO(Vitality);
		REG_MEMBER(_bool,LiveBody);
		REG_MEMBER(_bool,DeadBody);
		REG_MEMBER(_bool,Bones);

		REG_AUTO(Rules);
		REG_MEMBER(_bool,Wounded);

	}ENDSAVE;

	umc_FindTargetNearCaster(){
		Radius=100;
		Friend=true;
		Units=true;
		LiveBody=true;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest.Clear();
		if(Not)
			dest+="! ";
		dest+="FindTargetNearCaster: R = %$Radius$%";
		dest+=", Side =";		if(Friend) dest+=" Friend";		if(Enemy) dest+=" Enemy";
		dest+=", Struct =";		if(Units) dest+=" Units";		if(Buildings) dest+=" Buildings";
		dest+=", Vitality =";	if(LiveBody) dest+=" LiveBody";	if(DeadBody) dest+=" DeadBody";		if(Bones) dest+=" Bones";
		dest+=", Rules =";		if(Wounded) dest+=" Wounded";
		//dest+=" )";
		return true;
	}
	virtual bool Check (ActiveUnitAbility* AA);
};

class umc_Invisibility: public UnitModificatorCondition{
public:

	int Subject;

	SAVE(umc_Invisibility){
		REG_PARENT(UnitModificatorCondition);
		REG_ENUM(_index,Subject,HeroParamsActivator);
	}ENDSAVE;

	umc_Invisibility(){
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(Not){
			dest="is Visible %$Subject$%";
		}else{
			dest="is Invisibility %$Subject$%";
		}
		return true;
	}
	virtual bool Check (ActiveUnitAbility* AA);
};
//
class UnitModificatorComplex: public  UnitModificator{
public:
	UnitModificatorArray Complex;

	SAVE(UnitModificatorComplex){
		REG_PARENT(UnitModificator);
		//REG_BASE(UnitModificator);
		REG_AUTO(Complex);
	}ENDSAVE;
	
	UnitModificatorComplex(){
		InfluenceMask=0xFFFFFFFF;
	}
	//virtual bool Modify(ActiveUnitAbility* AA);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(!ClassEditor::CheckIfOpen(this)){
			dest="Complex/   %$Conditions$%/   %$Complex$%";
		}else{
			dest="Complex";
		}
		return true;
	}
};

class mod_MotionSpeed: public UnitModificator{
public:
	bool Trap;
	bool Stun;
	vOperand MotionSpeed;
	
	SAVE(mod_MotionSpeed){
		REG_PARENT(UnitModificator);
		REG_MEMBER(_bool,Trap);
		REG_MEMBER(_bool,Stun);
		REG_AUTO(MotionSpeed);		
	}ENDSAVE;
	
	mod_MotionSpeed(){
		InfluenceMask=ABL_MotionSpeed;
	}
	//virtual bool Modify(ActiveUnitAbility* AA);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(Trap){
			static char* st=GetTextByID("#mod_Stun");
			static char* tr=GetTextByID("#mod_Trap");
			if(Stun) dest+=st;
				else dest+=tr;
		}else{
			static char* ms=GetTextByID("#mod_MotionSpeed");
			dest=ms;
			dest+=" %$MotionSpeed$%";
		}
		return true;
	}
	virtual enActionType GetActionType(){ 
		if(Trap)
			return atDecrease;
		else
			return MotionSpeed.GetActionType(); 
	};
};
class mod_AttackRadiusMax: public UnitModificator{
public:
	vOperand Operand;

	SAVE(mod_AttackRadiusMax){
		REG_PARENT(UnitModificator);
		REG_AUTO(Operand);
	}ENDSAVE;

	mod_AttackRadiusMax(){
		InfluenceMask=ABL_AttackRadiusMax;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if( Operand.sign==4 ){
			int R=Operand.num.Get();
			if(R<120){
				static char* ch=GetTextByID("#mod_AttackRadiusMaxMelee");
				dest=ch;
			}else{
				dest.Clear();
			}
		}else{
			static char* ch=GetTextByID("#mod_AttackRadiusMax");
			dest=ch;
			dest+=" %$Operand$%";
		}
		return true;
	}
	virtual enActionType GetActionType() { return Operand.GetActionType(); }
};
class mod_Silence: public UnitModificator{
public:
	SAVE(mod_Silence){
		REG_PARENT(UnitModificator);
	}ENDSAVE;
	mod_Silence(){
		InfluenceMask=ABL_Silence;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		static char* ch=GetTextByID("#mod_Silence");
		dest=ch;
		return true;
	}
	virtual enActionType GetActionType() { return atDecrease; }
};
class mod_SkillSilence: public UnitModificator{
public:
	SAVE(mod_SkillSilence){
		REG_PARENT(UnitModificator);
	}ENDSAVE;
	mod_SkillSilence(){
		InfluenceMask=ABL_SkillSilence;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		static char* ch=GetTextByID("#mod_SkillSilence");
		dest=ch;
		return true;
	}
	virtual enActionType GetActionType() { return atDecrease; }
};
class mod_Death: public UnitModificator{
public:
	SAVE(mod_Death){
		REG_PARENT(UnitModificator);
	}ENDSAVE;
	mod_Death(){
		InfluenceMask=ABL_Death;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		static char* ch=GetTextByID("#mod_Death");
		dest=ch;
		return true;
	}
	virtual enActionType GetActionType() { return atDecrease; }
};
class mod_Compassion: public UnitModificator{
public:
	SAVE(mod_Compassion){
		REG_PARENT(UnitModificator);
	}ENDSAVE;
	mod_Compassion(){
		InfluenceMask=ABL_Compassion;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		static char* ch=GetTextByID("#mod_Compassion");
		dest=ch;
		return true;
	}
	virtual enActionType GetActionType() { return atDecrease; }
};
class mod_Invulnerability: public UnitModificator{
public:
	SAVE (mod_Invulnerability) REG_PARENT(UnitModificator) ENDSAVE;

	mod_Invulnerability(){
		InfluenceMask=ABL_Invulnerability;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		static char* ch=GetTextByID("#mod_Invulnerability");
		dest=ch;
		return true;
	}
	virtual enActionType GetActionType() { return atRise; }
};

class mod_Damage: public UnitModificator{
public:
	vOperand Operand;

	SAVE(mod_Damage){
		REG_PARENT(UnitModificator);
		REG_AUTO(Operand);
	}ENDSAVE;

	mod_Damage(){
		InfluenceMask=ABL_Damage;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		static char* ch=GetTextByID("#mod_Damage");
		dest=ch;
		dest+=" %$Operand$%";
		return true;
	}
	virtual enActionType GetActionType() { return Operand.GetActionType(); }
};
class pOperand:  public vOperand{
public:
	int ID;
	bool Last;
	SAVE (pOperand) REG_PARENT(vOperand) ENDSAVE;
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest=GetTextByID(WeaponIDS[ID]);
		dest+=" ";
		_str d;
		vOperand::GetClassPresentationMask(d,DataPtr,ExtraPtr,options,Flags);
		dest+=d;
		//if(!Last){
		//	dest+="\n";
		//}
		return true;
	}
};
class cProtections:  public ClassArray<pOperand>{
public:
	SAVE(cProtections){
		REG_BASE(ClassArray<pOperand>); 
	}ENDSAVE;
	cProtections() { Init(); }
	void Init(){
		int i=GetAmount();
		while(i<4){
			pOperand* O=new pOperand;
			Add(O);
			i++;
		}
		while(i>4){
			i--;
			DelElement(i);
		}
		for(i=0;i<4;i++){
			Values[i]->ID=i;
		}
	}
	bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL){
		cProtections* mP=(cProtections*)ClassPtr;
		reset_class(ClassPtr);
		bool load=ClassArray<pOperand>::Load(xml,ClassPtr,Error,Extra);
		mP->Init();
		return load;
	}
	virtual int GetExpansionRules(){ return 0; }
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors){
			int Last=-1;
			int N=0;
			if(!ClassEditor::CheckIfOpen(this)){
				for (int i=0;i<GetAmount();i++) if (Values[i]->sign==2 || Values[i]->num!=0) {
					dest+="%$";
					dest+=i;
					dest+="$%";
					Values[i]->Last=false;
					Last=i;
					N++;
				};
			}
			if(N==0) dest+="Empty";
				else Values[Last]->Last=true;
		}else{
			static char* ch=GetTextByID("#mod_Protection");
			int Last=-1;
			int N=0;
			for (int i=0;i<GetAmount();i++) if (Values[i]->sign==2 || Values[i]->num!=0) {
				dest+=ch;
				dest+=" %$";
				dest+=i;
				dest+="$%\n";				
				Values[i]->Last=false;
				Last=i;
				N++;
			};
			if(N==0) dest+="Empty";
				else Values[Last]->Last=true;
		}
		return true;
	}
	virtual enActionType GetActionType() { return GetAmount()>0?(*this)[0]->GetActionType():atNeutral; }
};
class mod_Protection: public UnitModificator{
public:
	cProtections Operand;
	SAVE(mod_Protection){
		REG_PARENT(UnitModificator);
		REG_AUTO(Operand);
	}ENDSAVE;
	mod_Protection(){
		InfluenceMask=ABL_Protection;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors){
			static char* ch=GetTextByID("#mod_Protection");
			dest=ch;
			dest+=" ";
			if(!ClassEditor::CheckIfOpen(this)){
				dest+="/   ";
				dest+="%$Operand$%";
			}
		}else{
			dest="%$Operand$%";
		}
		return true;
	}
	virtual enActionType GetActionType() { return Operand.GetActionType(); }
};
class mod_Shield: public UnitModificator{
public:
	vOperand Operand;
	SAVE(mod_Shield){
		REG_PARENT(UnitModificator);
		REG_AUTO(Operand);
	}ENDSAVE;
	mod_Shield(){
		InfluenceMask=ABL_Shield;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		static char* ch=GetTextByID("#mod_Shield");
		dest=ch;
		dest+=" %$Operand$%";
		return true;
	}	
	virtual enActionType GetActionType() { return Operand.GetActionType(); }
};
class mod_AttackRate: public UnitModificator{
public:
	vOperand Operand;
	SAVE(mod_AttackRate){
		REG_PARENT(UnitModificator);
		REG_AUTO(Operand);
	}ENDSAVE;
	mod_AttackRate(){
		InfluenceMask=ABL_AttackRate;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		static char* ch=GetTextByID("#mod_AttackRate");
		dest=ch;
		dest+=" %$Operand$%";
		return true;
	}	
	virtual enActionType GetActionType() { return Operand.GetActionType(); }
};
class mod_MagicImmunity: public UnitModificator{
public:
	SAVE(mod_MagicImmunity){
		REG_PARENT(UnitModificator);
	}ENDSAVE;
	mod_MagicImmunity(){
		InfluenceMask=ABL_MagicImmunity;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		static char* ch=GetTextByID("#mod_MagicImmunity");
		dest=ch;
		return true;
	}
	virtual enActionType GetActionType() { return atRise; }
};
class mod_SkillImmunity: public UnitModificator{
public:
	SAVE(mod_SkillImmunity){
		REG_PARENT(UnitModificator);
	}ENDSAVE;
	mod_SkillImmunity(){
		InfluenceMask=ABL_SkillImmunity;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		static char* ch=GetTextByID("#mod_SkillImmunity");
		dest=ch;
		return true;
	}
	virtual enActionType GetActionType() { return atRise; }
};
class mod_LifeRegeneration: public UnitModificator{
public:
	vOperand Operand;
	SAVE(mod_LifeRegeneration){
		REG_PARENT(UnitModificator);
		REG_AUTO(Operand);
	}ENDSAVE;
	mod_LifeRegeneration(){
		InfluenceMask=ABL_LifeRegeneration;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		static char* ch=GetTextByID("#mod_LifeRegeneration");
		dest=ch;
		dest+=" %$Operand$%";
		return true;
	}
	virtual enActionType GetActionType() { return Operand.GetActionType(); }
};
class mod_ManaRegeneration: public UnitModificator{
public:
	vOperand Operand;
	SAVE(mod_ManaRegeneration){
		REG_PARENT(UnitModificator);
		REG_AUTO(Operand);
	}ENDSAVE;
	mod_ManaRegeneration(){
		InfluenceMask=ABL_ManaRegeneration;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		static char* ch=GetTextByID("#mod_ManaRegeneration");
		dest=ch;
		dest+=" %$Operand$%";
		return true;
	}
	virtual enActionType GetActionType() { return Operand.GetActionType(); }
};
class mod_Vision: public UnitModificator{
public:
	vOperand Operand;
	SAVE(mod_Vision){
		REG_PARENT(UnitModificator);
		REG_AUTO(Operand);
	}ENDSAVE;
	mod_Vision(){
		InfluenceMask=ABL_Vision;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest.Clear();
		if(vGameMode!=gmCamp){
			static char* ch=GetTextByID("#mod_Vision");
			dest=ch;
			dest+=" %$Operand$%";
		}
		return true;
	}
	virtual enActionType GetActionType() { return Operand.GetActionType(); }
};
class mod_Weapon: public UnitModificator{
public:
	int WeaponShots;
	ClassRef<WeaponModificator> Weapon;
	SAVE(mod_Weapon){
		REG_PARENT(UnitModificator);
		REG_MEMBER(_int,WeaponShots);
		REG_AUTO(Weapon);
	}ENDSAVE;
	mod_Weapon(){
		InfluenceMask=ABL_Weapon;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors){
			static char* ch=GetTextByID("#mod_Weapon");
			dest=ch;
			dest+=" %$Weapon$%";
			if(WeaponShots){
				static char* chr=GetTextByID("#mod_WeaponShots");
				dest.print(" %$WeaponShots$% %s",chr);
			}/*else{
				dest+=" Shots unlimited";
			}*/
		}
		return true;
	}
};
class mod_Invisibility: public UnitModificator{
public:
	bool Invisibility;
	bool Detector;
	bool SearchVictim;
	SAVE(mod_Invisibility){
		REG_PARENT(UnitModificator);
		//REG_AUTO(Operand);
		REG_MEMBER(_bool,Invisibility);
		REG_MEMBER(_bool,Detector);
		REG_MEMBER(_bool,SearchVictim);
	}ENDSAVE;
	mod_Invisibility(){
		InfluenceMask=ABL_Invisibility;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest.Clear();
		if(Invisibility){
			static char* ch=GetTextByID("#mod_Invisibility");
			dest.print("%s ",ch);
		}
		if(Detector){
			static char* ch=GetTextByID("#mod_Detector");
			dest+=ch;
		}
		return true;
	}
	virtual enActionType GetActionType() { return atRise; }
};
class mod_Summon: public UnitModificator{
public:
	SAVE(mod_Summon){
		REG_PARENT(UnitModificator);
	}ENDSAVE;
	mod_Summon(){
		InfluenceMask=ABL_Summon;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors)
			dest="Summon";
		else
			dest.Clear();
		return true;
	}
};
class mod_Frozen: public UnitModificator{
public:
	SAVE(mod_Frozen){
		REG_PARENT(UnitModificator);
	}ENDSAVE;
	mod_Frozen(){
		InfluenceMask=ABL_Frozen;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		static char* ch=GetTextByID("#mod_Frozen");
		dest=ch;
		return true;
	}
	virtual enActionType GetActionType() { return atDecrease; }
};
class mod_Vampire: public UnitModificator{
public:
	//int LifeType;
	int SourceType;
	vOperand Operand;
	SAVE(mod_Vampire){
		REG_PARENT(UnitModificator);
		//REG_ENUM(_index,LifeType,DamagerLifeType);
		REG_ENUM(_index,SourceType,DamagerSourceType);
		REG_AUTO(Operand);
	}ENDSAVE;
	mod_Vampire(){
		InfluenceMask=ABL_Vampire;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors){
			dest="Vampire (%$SourceType$% %$Operand$%)";
			//char txt[]=" %$Operand$%)";
			//if(Operand.sign<2 && Operand.num==0){
			//	txt[0]=')';
			//	txt[1]=0;
			//}		
			//dest+=txt;
		}else{
			static char* ch=GetTextByID("#mod_Vampire");
			dest=ch;
			//dest+=" %$Operand$%";
		}
		return true;
	}
	virtual enActionType GetActionType() { return atRise; }
};
class mod_DamReduct: public UnitModificator{
public:
	int ReductionType;
	vOperand Operand;
	SAVE(mod_DamReduct){
		REG_PARENT(UnitModificator);
		REG_ENUM(_index,ReductionType,DamReductionType);
		REG_AUTO(Operand);
	}ENDSAVE;
	mod_DamReduct(){
		InfluenceMask=ABL_DamReduct;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors){
			dest="Reduction (%$ReductionType$% %$Operand$%)";
			//char txt[]=" %$Operand$%)";
			//if(Operand.sign<2 && Operand.num==0){
			//	txt[0]=')';
			//	txt[1]=0;
			//}		
			//dest+=txt;
		}else{
			static char* ch=GetTextByID("#mod_DamReduct");
			dest=ch;
			dest+=" %$Operand$%";
		}
		return true;
	}
	virtual enActionType GetActionType() { return Operand.GetActionType(); }
};
class mod_IgnoreProtection: public UnitModificator{
public:
	vOperand Operand;
	SAVE(mod_IgnoreProtection){
		REG_PARENT(UnitModificator);
		REG_AUTO(Operand);
	}ENDSAVE;
	mod_IgnoreProtection(){
		InfluenceMask=ABL_IgnoreProtection;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors){
			dest="Ignore Protection(%$Operand$%)";
			//char txt[]=" %$Operand$%)";
			//if(Operand.sign<2 && Operand.num==0){
			//	txt[0]=')';
			//	txt[1]=0;
			//}		
			//dest+=txt;
		}else{
			static char* ch=GetTextByID("#mod_IgnoreProtection");
			dest=ch;
			dest+=" %$Operand$%";
		}
		return true;
	}
	virtual enActionType GetActionType() { return Operand.GetActionType(); }
};
class mod_Alteration: public UnitModificator{
public:
	//int LifeType;
	int ModifType;
	vOperand Operand;
	SAVE(mod_Alteration){
		REG_PARENT(UnitModificator);
		REG_ENUM(_index,ModifType,ModificatorType);
		REG_AUTO(Operand);
	}ENDSAVE;
	mod_Alteration(){
		InfluenceMask=ABL_Alteration;
	}
	
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors){
			dest="Alteration (%$ModifType$% %$Operand$%)";
			//char txt[]=" %$Operand$%)";
			//if(Operand.sign<2 && Operand.num==0){
			//	txt[0]=')';
			//	txt[1]=0;
			//}		
			//dest+=txt;
		}else{
			static char* ch=GetTextByID("#mod_Alteration");
			dest=ch;
		}
		return true;
	}
	virtual enActionType GetActionType() { return Operand.GetActionType(); }
};

class mod_Thorn: public UnitModificator{
public:
	//int LifeType;
	int SourceType;
	bool RangeThorn;
	bool AnyDistanceThorn;
	vOperand Operand;
	SAVE(mod_Thorn){
		REG_PARENT(UnitModificator);
		//REG_ENUM(_index,LifeType,DamagerLifeType);
		REG_ENUM(_index,SourceType,DamagerSourceType);
		REG_MEMBER(_bool,RangeThorn);
		REG_MEMBER(_bool,AnyDistanceThorn);
		REG_AUTO(Operand);
	}ENDSAVE;
	mod_Thorn(){
		InfluenceMask=ABL_Thorn;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors){
			dest="Thorn (%$SourceType$%, Range:%$RangeThorn$%, AnyDistance:%$AnyDistanceThorn$%, %$Operand$%)";
			//char txt[]=" %$Operand$%)";
			//if(Operand.sign<2 && Operand.num==0){
			//	txt[0]=')';
			//	txt[1]=0;
			//}		
			//dest+=txt;
		}else{
			static char* ch=GetTextByID("#mod_Thorn");
			dest=ch;
		}
		return true;
	}
	virtual enActionType GetActionType() { return Operand.GetActionType(); }
};
class mod_ManaValue: public UnitModificator{
public:
	vOperand Operand;
	SAVE(mod_ManaValue){
		REG_PARENT(UnitModificator);
		REG_AUTO(Operand);
	}ENDSAVE;
	mod_ManaValue(){
		InfluenceMask=ABL_ManaValue;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		static char* ch=GetTextByID("#mod_ManaValue");
		dest=ch;
		dest+=" %$Operand$%";
		return true;
	}
	virtual enActionType GetActionType() { return Operand.GetActionType(true); }
};
class mod_Life: public UnitModificator {
public:	
	vOperand Operand;
	int Shots;
	int FirstShotTime;
	int Period;
	bool TakeProtection;
    bool IgnoreDamageOnBuildingMultiply;
	int MinLifeAfterDamage;
	bool ManaBurn;
	SAVE(mod_Life){
		REG_PARENT(UnitModificator);
		REG_AUTO(Operand);
		REG_MEMBER(_int,Shots);
		REG_MEMBER(_int,FirstShotTime);
		REG_MEMBER(_int,Period);
		REG_MEMBER(_bool,TakeProtection);
        REG_MEMBER(_bool,IgnoreDamageOnBuildingMultiply);
		REG_MEMBER(_int,MinLifeAfterDamage);
		REG_MEMBER(_bool,ManaBurn);
	}ENDSAVE;
	mod_Life(){
		InfluenceMask=ABL_Life;
		Shots=1;
		TakeProtection=true;
        IgnoreDamageOnBuildingMultiply=false;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors){
			if(Shots>1){
				dest="Current life %$Operand$% each %$Period$% ms %$Shots$% times. Start time %$FirstShotTime$%";
			}else{
				dest="Current life %$Operand$%. Start time %$FirstShotTime$%";
			}
		}else{
			if( Operand.sign==4 ){
				static char* ch=GetTextByID("#mod_Life");
				dest=ch;
			}else if( Operand.sign==0 || Operand.sign==2 ){
				static char* ch=GetTextByID("#mod_LifeHeal");
				dest=ch;
			}else{
				static char* ch=GetTextByID("#mod_LifeDamage");
				dest=ch;
			}
			if(Shots>1){
				// доработать
				//dest+=" %$Operand$% * %$Shots$%";
				dest+=" %$Operand$%";
			}else{
				dest+=" %$Operand$%";
			}
		}
		if(ManaBurn)
			dest+=" ManaBurn";
		return true;
	}
	virtual enActionType GetActionType() { return Operand.GetActionType(); }
};
class mod_Mana: public UnitModificator{
public:
	vOperand Operand;
	int Shots;
	int FirstShotTime;
	int Period;
	SAVE(mod_Mana){
		REG_PARENT(UnitModificator);
		REG_AUTO(Operand);
		REG_MEMBER(_int,Shots);
		REG_MEMBER(_int,FirstShotTime);
		REG_MEMBER(_int,Period);
	}ENDSAVE;
	mod_Mana(){
		InfluenceMask=ABL_Mana;
		Shots=1;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors){
			if(Shots>1){
				dest="Current mana %$Operand$% each %$Period$% ms %$Shots$% times. Start time %$FirstShotTime$%";
			}else{
				dest="Current mana %$Operand$%. Start time %$FirstShotTime$%";
			}
		}else{
			static char* ch=GetTextByID("#mod_Mana");
			dest=ch;
			if(Shots>1){
				dest+=" %$Operand$% * %$Shots$%";
			}else{
				dest+=" %$Operand$%";
			}
		}
		return true;
	}
	virtual enActionType GetActionType() { return Operand.GetActionType(); }
};
class mod_LifeMax: public UnitModificator{
public:
	vOperand Operand;
	SAVE(mod_LifeMax){
		REG_PARENT(UnitModificator);
		REG_AUTO(Operand);
	}ENDSAVE;
	mod_LifeMax(){
		InfluenceMask=ABL_LifeMax;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		static char* ch=GetTextByID("#mod_LifeMax");
		dest=ch;
		dest+=" %$Operand$%";
		return true;
	}
	virtual enActionType GetActionType() { return Operand.GetActionType(); }
};
class mod_ManaMax: public UnitModificator{
public:
	vOperand Operand;
	SAVE(mod_ManaMax){
		REG_PARENT(UnitModificator);
		REG_AUTO(Operand);
	}ENDSAVE;
	mod_ManaMax(){
		InfluenceMask=ABL_ManaMax;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		static char* ch=GetTextByID("#mod_ManaMax");
		dest=ch;
		dest+=" %$Operand$%";
		return true;
	}
	virtual enActionType GetActionType() { return Operand.GetActionType(); }
};
class mod_AttackCaster: public UnitModificator{
public:
	bool Fanatic;
	bool IncludeUnitWithMana;
	//vOperand Operand;
	SAVE(mod_AttackCaster){
		REG_PARENT(UnitModificator);
		REG_MEMBER(_bool,Fanatic);
		REG_MEMBER(_bool,IncludeUnitWithMana);
		//REG_AUTO(Operand);
	}ENDSAVE;
	mod_AttackCaster(){
		InfluenceMask=ABL_AttackCaster;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors){
			if(Fanatic){
				dest="Attack caster fanatic";
			}else{
				dest="Attack caster";
			}
		}else{
			static char* ch=GetTextByID("#mod_AttackCaster");
			dest=ch;
		}
		return true;
	}
};

class mod_Material: public UnitModificator{
public:
	int Operation;
	int AddMatherial;
	SAVE(mod_Material){
		REG_PARENT(UnitModificator);
		REG_ENUM(_index,Operation,MM_LogicOp);
		REG_ENUM(_index,AddMatherial,Matherials);
	}ENDSAVE;
	mod_Material(){
		InfluenceMask=ABL_Material;
		Operation=2;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors){
			dest="Material %$Operation$% %$AddMatherial$%";
		}else{
			dest.Clear();
		}
		return true;
	}
};
class mod_Position: public UnitModificator{
public:
	bool Direction; // blink
	//vOperand Operand;

	SAVE(mod_Position){
		REG_PARENT(UnitModificator);
		REG_MEMBER(_bool,Direction);
		//REG_AUTO(Operand);
	}ENDSAVE;

	mod_Position(){
		//InfluenceMask=ABL_Position;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(Direction){
			dest="Position blink";
		}else{
			dest="Position teleport";
		}
		return true;
	}
};
class mod_OverEarth: public UnitModificator{
public:
	int FinalHeight;
	int Speed;
	bool Die;
	SAVE(mod_OverEarth){
		REG_PARENT(UnitModificator);
		REG_MEMBER(_int,FinalHeight);
		REG_MEMBER(_int,Speed);
		REG_MEMBER(_bool,Die);
	}ENDSAVE;
	mod_OverEarth(){
		InfluenceMask=ABL_OverEarth;
		Speed=15;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors){
			dest="OverEarth: FinalHeight %$FinalHeight$%, Speed %$Speed$%";
		}else{
			if(FinalHeight==0){
				static char* ch=GetTextByID("#mod_OverEarthDown");
				dest=ch;
			}else{
				static char* ch=GetTextByID("#mod_OverEarth");
				dest=ch;
			}
		}
		return true;
	}
	virtual enActionType GetActionType() { 
		if(FinalHeight==0){
			return atDecrease;
		}else{
			return atNeutral;
		}
	}
};
class mod_Fear: public UnitModificator{
public:
	SAVE(mod_Fear){
		REG_PARENT(UnitModificator);
	}ENDSAVE;
	mod_Fear(){
		InfluenceMask=ABL_Fear;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		static char* ch=GetTextByID("#mod_Fear");
		dest=ch;
		return true;
	}
	virtual enActionType GetActionType() { return atDecrease; }
};

class mod_Plymorph: public UnitModificator{
public:
	//bool Return;
	BaseClassUnitType NewType;
	int ThroughState;
	bool ReInitAbilities;
	bool Permanent;
	SAVE(mod_Plymorph){
		REG_PARENT(UnitModificator);
		//REG_MEMBER(_bool,Return);
		//SAVE_SECTION(1);
			REG_AUTO(NewType);
		//SAVE_SECTION(0xFFFFFFFF);
			REG_MEMBER(_int,ThroughState);
			REG_MEMBER(_bool,ReInitAbilities);
			REG_MEMBER(_bool,Permanent);
	}ENDSAVE;
	mod_Plymorph(){
		InfluenceMask=ABL_Plymorph;
	}
	//DWORD GetClassMask(){
	//	if (!Return) return 1;
	//	return 2;
	//}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors){
			//if(Return) 
			//	dest="Plymorph: Return";
			//else 
				dest="Plymorph: NewType=%$NewType$%, ThroughState=%$ThroughState$%";
		}else{
			static char* ch=GetTextByID("#mod_Plymorph");
			dest=ch;
		}
		return true;
	}
};

class mod_Symbol: public UnitModificator{
public:	
	short FileID;
	int SpriteID;
	short FileID_Char;
	short FileID_Inv;
	char* Hint;
	char* HintName;
	SAVE(mod_Symbol){
		REG_PARENT(UnitModificator);
		REG_MEMBER(_gpfile,FileID);
		REG_SPRITE(SpriteID,FileID);
		REG_MEMBER(_gpfile,FileID_Char);
		REG_MEMBER(_gpfile,FileID_Inv);
		REG_MEMBER(_textid,Hint);
		REG_MEMBER(_textid,HintName);
	}ENDSAVE;
	mod_Symbol(){
		InfluenceMask=ABL_Symbol;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors){
			dest="Interface symbol: SpriteID=%$SpriteID$%, Hint=%$Hint$%";
		}else{
			dest.Clear();
		}
		return true;
	}
};

class mod_ProduceEnable: public UnitModificator{
public:
	bool State;
	SAVE(mod_ProduceEnable){
		REG_PARENT(UnitModificator);
		REG_MEMBER(_bool,State);
	}ENDSAVE;
	mod_ProduceEnable(){
		InfluenceMask=ABL_ProduceEnable;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors){
			dest="ProduceEnable: %$State$%";
		}else{
			if(!State){
				static char* ch=GetTextByID("#mod_ProduceEnable");
				dest=ch;
			}else{
				dest.Clear();
			}
		}
		return true;
	}
	virtual enActionType GetActionType() { return atDecrease; }
};
class mod_Scale: public UnitModificator{
public:
	vOperand Operand;
	SAVE(mod_Scale){
		REG_PARENT(UnitModificator);
		REG_AUTO(Operand);
	}ENDSAVE;
	mod_Scale(){
		InfluenceMask=ABL_Scale;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		static char* ch=GetTextByID("#mod_Scale");
		dest=ch;
		dest+=" %$Operand$%";
		return true;
	}
};
class mod_Control: public UnitModificator{
public:
	SAVE (mod_Control) REG_PARENT(UnitModificator) ENDSAVE;
	mod_Control(){
		InfluenceMask=ABL_Control;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		static char* ch=GetTextByID("#mod_Control");
		dest=ch;
		return true;
	}
	virtual enActionType GetActionType() { return atDecrease; }
};

class mod_NMask: public UnitModificator{
public:
	byte NMask;
	SAVE(mod_NMask){
		REG_PARENT(UnitModificator);
		REG_MEMBER(_BYTE,NMask);
	}ENDSAVE;
	mod_NMask(){
		InfluenceMask=ABL_NMask;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors){
			static char* ch=GetTextByID("#mod_NMask");
			dest=ch;
		}else{
			dest.Clear();
		}
		return true;
	}
	//virtual enActionType GetActionType() { return atDecrease; }
};

class mod_Insubordination: public UnitModificator{
public:
	SAVE (mod_Insubordination) REG_PARENT(UnitModificator) ENDSAVE;
	mod_Insubordination(){
		InfluenceMask=ABL_Insubordination;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		static char* ch=GetTextByID("#mod_Insubordination");
		dest=ch;
		return true;
	}
	virtual enActionType GetActionType() { return atDecrease; }
};
class mod_ModelAnimation: public UnitModificator{
public:
	bool TakeFromTarget;
	BaseClassUnitType Model;
	SAVE (mod_ModelAnimation){
		REG_PARENT(UnitModificator);
		SAVE_SECTION(1);
		REG_MEMBER(_bool,TakeFromTarget);
		SAVE_SECTION(2);
		REG_AUTO(Model);
	}ENDSAVE;
	DWORD GetClassMask(){
		DWORD M=0xFFFFFFFF;
		if(TakeFromTarget){
			M=1;
		}else{
			M=2;
		}
		return M;
	};
	mod_ModelAnimation(){
		InfluenceMask=ABL_ModelAnimation;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors){
			if(TakeFromTarget){
				dest="ModelAnimation TakeFromTarget";
			}else{
				dest="ModelAnimation %$Model$%";
			}
		}else{
			dest.Clear();
		}
		return true;
	}	
};

class mod_LevelForEffects: public UnitModificator{
public:
	vOperand Operand;
	SAVE (mod_LevelForEffects){
		REG_PARENT(UnitModificator);
		REG_AUTO(Operand);
	}ENDSAVE;
	mod_LevelForEffects(){
		InfluenceMask=ABL_LevelForEffects;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		static char* ch=GetTextByID("#mod_LevelForEffects");
		dest=ch;
		dest+=" %$Operand$%";
		return true;
	}
	virtual enActionType GetActionType() { return Operand.GetActionType(); }
};
class mod_TradeSkill: public UnitModificator{
public:
	vOperand Operand;
	SAVE (mod_TradeSkill){
		REG_PARENT(UnitModificator);
		REG_AUTO(Operand);
	}ENDSAVE;
	mod_TradeSkill(){
		InfluenceMask=ABL_TradeSkill;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		static char* ch=GetTextByID("#mod_TradeSkill");
		dest=ch;
		dest+=" %$Operand$%";
		return true;
	}
	virtual enActionType GetActionType() { return Operand.GetActionType(); }
};
class mod_Learning: public UnitModificator{
public:
	vOperand Operand;
	SAVE (mod_Learning){
		REG_PARENT(UnitModificator);
		REG_AUTO(Operand);
	}ENDSAVE;
	mod_Learning(){
		InfluenceMask=ABL_Learning;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		static char* ch=GetTextByID("#mod_Learning");
		dest=ch;
		dest+=" %$Operand$%";
		return true;
	}
	virtual enActionType GetActionType() { return Operand.GetActionType(); }
};
class mod_WeaponKind: public UnitModificator{
public:
	int AttType;
	int Kind;
	SAVE(mod_WeaponKind){
		REG_PARENT(UnitModificator);
		REG_MEMBER(_int,AttType);
		REG_ENUM(_index,Kind,WeaponIDS);
	}ENDSAVE;
	mod_WeaponKind(){
		InfluenceMask=ABL_WeaponKind;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors){
			dest="WeaponKind %$Kind$%, AttackType %$AttType$%";
		}else{
			//static char* ch=GetTextByID("#mod_WeaponKind");
			static DString ch;
			ch="#mod_WeaponKind";
			ch+=Kind;
			dest=GetTextByID(ch.pchar());
		}
		return true;
	}
};
class mod_Evasion: public UnitModificator{
public:
	vOperand Operand;
	SAVE(mod_Evasion){
		REG_PARENT(UnitModificator);
		REG_AUTO(Operand);
	}ENDSAVE;
	mod_Evasion(){
		InfluenceMask=ABL_Evasion;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		static char* ch=GetTextByID("#mod_Evasion");
		dest=ch;
		dest+=" %$Operand$%";
		return true;
	}
	virtual enActionType GetActionType() { return Operand.GetActionType(); }
};
class mod_Miss: public UnitModificator{
public:
	vOperand Operand;
	SAVE(mod_Miss){
		REG_PARENT(UnitModificator);
		REG_AUTO(Operand);
	}ENDSAVE;
	mod_Miss(){
		InfluenceMask=ABL_Miss;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		static char* ch=GetTextByID("#mod_Miss");
		dest=ch;
		dest+=" %$Operand$%";
		return true;
	}
	virtual enActionType GetActionType() { return Operand.GetActionType(); }
};
class mod_ExtraAnimation: public UnitModificator{
public:
	int AnimationID;

	SAVE(mod_ExtraAnimation){
		REG_PARENT(UnitModificator);
		REG_MEMBER(_ModelID,AnimationID);
	}ENDSAVE;

	mod_ExtraAnimation(){
		InfluenceMask=ABL_ExtraAnimation;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors){
			dest="ExtraAnimation %$AnimationID$%";
		}else{
			dest.Clear();
		}
		return true;
	}
	void Modify(int Basic, int& Current){
		if(AnimationID!=-1){
			Current=AnimationID;
		}
	}
};

class mod_ItemPickUp: public UnitModificator{
public:
	//int AnimationID;
	SAVE(mod_ItemPickUp){
		REG_PARENT(UnitModificator);
		//REG_MEMBER(_ModelID,AnimationID);
	}ENDSAVE;
	mod_ItemPickUp(){
		InfluenceMask=ABL_ItemPickUp;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors){
			dest="Disable ItemPickUp";
		}else{
			dest.Clear();
		}
		return true;
	}
	//void Modify(int Basic, int& Current){
	//	if(AnimationID!=-1){
	//		Current=AnimationID;
	//	}
	//}
};
class mod_ExtraAbility: public UnitModificator{
public:
    ClassRef<UnitAbility> ExAbility;
	SAVE(mod_ExtraAbility){
		REG_PARENT(UnitModificator);
		REG_AUTO(ExAbility);
	}ENDSAVE;
	mod_ExtraAbility(){
		InfluenceMask=ABL_ExtraAbility;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors){
			dest="AddAbiltyToUnit(%$ExAbility$%)";
		}else{
			dest.Clear();
		}
		return true;
	}
	void Modify(OneObject* NewUnit){
		UnitAbility* UA=ExAbility.Get();
		if(UA){		
			pUnit unit(NewUnit);
			bool add=true;
              itr_AA.Create(NewUnit);
              while(ActiveUnitAbility* aA=itr_AA.Next()){
                        if(UA==aA->GetA()){
                            add=false;
                      break;
						}
					}
			bool AddActiveUnitAbility(word Unit, ActiveUnitAbility* AUA);
			ActiveUnitAbility* aua=UA->GetNewActiveAbility();
			if(aua){
			if(add) {
				AddActiveUnitAbility(unit.ID,aua); 
			}
			}
		}
	}
			
		
};

class mod_DeleteAbility: public UnitModificator{
public:
    ClassRef<UnitAbility> DelAbility;
	SAVE(mod_DeleteAbility){
		REG_PARENT(UnitModificator);
		REG_AUTO(DelAbility);
	}ENDSAVE;
	mod_DeleteAbility(){
		InfluenceMask=ABL_DeleteAbility;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors){
			dest="DeleteAbility(%$DelAbility$%)";
		}else{
			dest.Clear();
		}
		return true;
	}
	void DeleteAb(OneObject* NewUnit){
		UnitAbility* UA=DelAbility.Get();
		if(UA){		
			itr_AA.Create(NewUnit);
		    while(ActiveUnitAbility* AA=itr_AA.Next()){
						UnitAbility* Abilka=AA->GetA();
							if((Abilka==UA)){
							 AA->Deleted=true;
					
							}
			}
		}
	}
			
		
};

class mod_SearchVictim: public UnitModificator{
public:
	int Type;
	SAVE(mod_SearchVictim){
		REG_PARENT(UnitModificator);
		REG_ENUM(_index,Type,SearchVictimType);
	}ENDSAVE;

	mod_SearchVictim(){
		InfluenceMask=ABL_SearchVictim;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors){
			dest="SearchVictim(%$Type$%)";
		}else{
			dest.Clear();
		}
		return true;
	}
};
class mod_MotionStyle: public UnitModificator{
public:
	int Style;	
	SAVE(mod_MotionStyle){
		REG_PARENT(UnitModificator);
		REG_ENUM(_index,Style,MOTIONSTYLE);
	}ENDSAVE;

	mod_MotionStyle(){
		InfluenceMask=ABL_MotionStyle;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		static char* ch=GetTextByID("#mod_MotionStyle");
		char* chs="%$Style$%";
		Enumerator* E=ENUM.Get("MOTIONSTYLE");
		if(E){
			chs=GetTextByID(E->GetStr(Style));
		}
		dest.Clear();
		dest.print("%s %s",ch,chs);
		return true;
	}
};

class mod_LockType: public UnitModificator{
public:
	int Type;	
	SAVE(mod_LockType){
		REG_PARENT(UnitModificator);
		REG_ENUM(_index,Type,LOCKTYPE);
	}ENDSAVE;

	mod_LockType(){
		InfluenceMask=ABL_LockType;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors){
			dest="LockType(%$Type$%)";
		}else{
			dest.Clear();
		}
		return true;
	}
};

class mod_CenterScreen: public UnitModificator{
public:
	//int Type;	
	SAVE(mod_CenterScreen){
		REG_PARENT(UnitModificator);
		//REG_ENUM(_index,Type,LOCKTYPE);
	}ENDSAVE;

	mod_CenterScreen(){
		InfluenceMask=ABL_CenterScreen;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors){
			dest="CenterScreen";
		}else{
			dest.Clear();
		}
		return true;
	}
};
class mod_SpellOnAttack: public UnitModificator{
public:
	ClassRef<UnitAbility> Spell;
	int ActionType;
	int CastType;
	SAVE(mod_SpellOnAttack){
		REG_PARENT(UnitModificator);
		REG_ENUM(_index,ActionType,AttSpellActionType);
		REG_ENUM(_index,CastType,AttSpellCastType);
		REG_AUTO(Spell);
	}ENDSAVE;

	mod_SpellOnAttack(){
		InfluenceMask=ABL_SpellOnAttack;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors){
		if(ActionType==0){
			dest="SpellOnReceiveDamage (%$CastType$%)";
		} else{
		    dest="SpellOnMakeDamage (%$CastType$%)";
		}
		}else{
			dest.Clear();
		}
	return true;
	}
};
class mod_DeleteAbilityOnReceiveDamage: public UnitModificator{
public:
	SAVE(mod_DeleteAbilityOnReceiveDamage){
		REG_PARENT(UnitModificator);
	}ENDSAVE;

	mod_DeleteAbilityOnReceiveDamage(){
		InfluenceMask=ABL_DeleteAbilityOnReceiveDamage;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors){
			dest="DeleteAbilityOnReceiveDamage";
		}else{
			dest.Clear();
		}
		return true;
	}
};

class mod_KillMask: public UnitModificator{
public:
	int Operation;
	int Material;

	SAVE(mod_KillMask){
		REG_PARENT(UnitModificator);
		REG_ENUM(_index,Operation,MM_LogicOp);
		REG_ENUM(_index,Material,Matherials);
	}ENDSAVE;

	mod_KillMask(){
		InfluenceMask=ABL_KillMask;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors){
			dest="KillMask %$Operation$% %$Material$%";
		}else{
			dest.Clear();
		}
		return true;
	}
};
class mod_DeleteAbilityAfterNShots: public UnitModificator{
public:
	int NShots;

	SAVE(mod_DeleteAbilityAfterNShots){
		REG_PARENT(UnitModificator);
		REG_MEMBER(_int,NShots);
	}ENDSAVE;

	mod_DeleteAbilityAfterNShots(){
		InfluenceMask=ABL_DeleteAbilityAfterNShots;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(InAllEditors){
			dest="DeleteAbilityAfter %$Operation$% shot(s)";
		}else{
			dest.Clear();
		}
		return true;
	}
};
//
class UnitModificatorsIterator{
public:
	int Current;
	int N;
	UnitModificator* UM[2048];
	uaModMask InfluenceMask;
	ActiveUnitAbility* A;

	UnitModificatorsIterator(){
	}
	UnitModificatorsIterator(ActiveUnitAbility* AA, DWORD Mask){
		Create(AA,Mask);
	}
	void Collect(UnitModificatorConditionArray* UMCs, UnitModificatorArray* UMs);
	void CollectAll(UnitModificatorConditionArray* UMCs, UnitModificatorArray* UMs);
	void Create(ActiveUnitAbility* AA, DWORD Mask);
	void Create(UnitAbility* UA);
	UnitModificator* Next();
};

extern UnitModificatorsIterator itr_UM;

#define SetUM(x) if(TypeAbil==0) GlobalUnitsContext::setCaster(CasterID,CasterSN); else GlobalUnitsContext::setCaster(OB->Index,OB->Serial); \
	GlobalUnitsContext::setAcceptor(OB->Index,OB->Serial);\
	UnitModificatorsIterator iterator_UM;\
	iterator_UM.Create(this,ABL_##x);\
	while (mod_##x* M=(mod_##x*)iterator_UM.Next())

#endif