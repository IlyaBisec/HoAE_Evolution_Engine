#include "stdheader.h"
#include "SuperEditor.h"
#include "unitability.h"
#include "HeroFunctions.h"

HeroVariableStorage* CurrentHeroAbility=NULL;

GetHeroVariable::GetHeroVariable()
{
	StrParam="";
}

void GetHeroVariable::SetArgument(int index, Operand* O)
{
	if(index==0)
	{
		StringType ST;
		if(O&&O->Get(&ST))
		{
			StrParam=ST.String.pchar();
		}
	}
}
bool GetHeroVariable::Get(BaseType* BT)
{
	bool rez=false;
	if(CurrentHeroAbility)
	{
		NumericalType* NT = dynamic_cast<NumericalType*> (BT);
		if(NT)
		{
			NT->Value=0;
			int* v=CurrentHeroAbility->GetVarRef(StrParam.pchar());
			if(v)
			{
				NT->Value=*v;
			}
			rez=true;
		}
	}
	return rez;	
}

//////////////////////////////////////////////////////////////////////////
////////////////////Acceptor and caster parameters////////////////////////
//////////////////////////////////////////////////////////////////////////
HeroVariableStorage* DetectHero(OneObject* OB);
bool AcceptorHeroLevel::Get(BaseType* BT){	
	NumericalType* NT=(NumericalType*)BT;
	NT->Value=0;
	OneObject* OB=GlobalUnitsContext::getAcceptor();
	if(OB){
		int Level=OB->newMons->NewParams.Level;
		HeroVariableStorage* HVS=DetectHero(OB);
		if(HVS){
			Level=HVS->Level;
		}
		OB->ActiveAbility->ActiveAbilities.modifyLevelForEffects(Level,Level);
		NT->Value=Level;
	}
	return true;
}

bool CasterHeroLevel::Get(BaseType* BT){	
	NumericalType* NT=(NumericalType*)BT;
	NT->Value=0;
	OneObject* OB=GlobalUnitsContext::getCaster();
	if(OB){
		int Level=OB->newMons->NewParams.Level;
		HeroVariableStorage* HVS=DetectHero(OB);
		if(HVS){
			Level=HVS->Level;
		}
		if(OB->ActiveAbility)
			OB->ActiveAbility->ActiveAbilities.modifyLevelForEffects(Level,Level);
		NT->Value=Level;
	}
	return true;
}
bool CasterTrueLevel::Get(BaseType* BT){	
	NumericalType* NT=(NumericalType*)BT;
	NT->Value=0;
	OneObject* OB=GlobalUnitsContext::getCaster();
	if(OB){
		int Level=OB->newMons->NewParams.Level;
		HeroVariableStorage* HVS=DetectHero(OB);
		if(HVS){
			Level=HVS->Level;
		}
	NT->Value=Level;
	}
	return true;
}
bool AcceptorTrueLevel::Get(BaseType* BT){	
	NumericalType* NT=(NumericalType*)BT;
	NT->Value=0;
	OneObject* OB=GlobalUnitsContext::getAcceptor();
	if(OB){
		int Level=OB->newMons->NewParams.Level;
		HeroVariableStorage* HVS=DetectHero(OB);
		if(HVS){
			Level=HVS->Level;
		}
		NT->Value=Level;
	}
	return true;
}
bool AcceptorMana::Get(BaseType* BT){	
	NumericalType* NT=(NumericalType*)BT;
	NT->Value=0;
	OneObject* OB=GlobalUnitsContext::getAcceptor();
	if(OB){
		NT->Value=OB->Mana;
	}
	return true;
}
bool CasterMana::Get(BaseType* BT){	
	NumericalType* NT=(NumericalType*)BT;
	NT->Value=0;
	OneObject* OB=GlobalUnitsContext::getCaster();
	if(OB){
		NT->Value=OB->Mana;
	}
	return true;
}
bool AcceptorLife::Get(BaseType* BT){	
	NumericalType* NT=(NumericalType*)BT;
	NT->Value=0;
	OneObject* OB=GlobalUnitsContext::getAcceptor();
	if(OB){
		NT->Value=OB->Life;
	}
	return true;
}
bool CasterLife::Get(BaseType* BT){	
	NumericalType* NT=(NumericalType*)BT;
	NT->Value=0;
	OneObject* OB=GlobalUnitsContext::getCaster();
	if(OB){
		NT->Value=OB->Life;
	}
	return true;
}
bool AcceptorMaxLife::Get(BaseType* BT){	
	NumericalType* NT=(NumericalType*)BT;
	NT->Value=0;
	OneObject* OB=GlobalUnitsContext::getAcceptor();
	if(OB){
		NT->Value=OB->MaxLife;
	}
	return true;
}
bool CasterMaxLife::Get(BaseType* BT){	
	NumericalType* NT=(NumericalType*)BT;
	NT->Value=0;
	OneObject* OB=GlobalUnitsContext::getCaster();
	if(OB){
		NT->Value=OB->MaxLife;
	}
	return true;
}
bool CasterAbilityCasts::Get(BaseType* BT){
	UnitAbility* UA=Ability.Get();
	NumericalType* NT=(NumericalType*)BT;
	NT->Value=0;
	OneObject* OB=GlobalUnitsContext::getCaster();
	if(OB){
		itr_AA.Create(OB);
		int ncast=0;
		while(ActiveUnitAbility* AA=itr_AA.Next()){
				if( AA->TypeAbil==0 && AA->GetA()==UA ){
					ncast++;
				}
		}
		if (ncast==UA->MaxCasts) ncast=UA->MaxCasts;
		NT->Value=ncast;
	}
	return true;
}
void CasterAbilityCasts::GetViewMask(DString& ST){
	UnitAbility* UA=Ability.Get();
	ST="CasterAbilityCasts";
	if(UA)
		ST.print("(%s)",UA->Name.pchar());
	else
		ST+="(NotDefine)";
}
bool CasterAbilityCasts::GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
	dest="CasterAbilityCasts(%$Ability$%)";
	return true;
}
bool AcceptorAbilityCasts::Get(BaseType* BT){	
	UnitAbility* UA=Ability.Get();
	NumericalType* NT=(NumericalType*)BT;
	NT->Value=0;
	OneObject* OB=GlobalUnitsContext::getAcceptor();
	if(OB){
		itr_AA.Create(OB);
		int ncast=0;
		while(ActiveUnitAbility* AA=itr_AA.Next()){
				if( AA->TypeAbil==0 && AA->GetA()==UA ){
					ncast++;
				}
		}
		if (ncast==UA->MaxCasts) ncast=UA->MaxCasts;
		NT->Value=ncast;
	}
	return true;
}
/*
				if( AA->TypeAbil==0 && AA->GetA()==UA ){
					ncast++;
				}
						if(AA->GetA()==UA){
		NT->Value=AA->EffectCount;
		}
*/
/*
	if(OB){
		itr_AA.Create(OB);
		while(ActiveUnitAbility* AA=itr_AA.Next()){
		if(AA->GetA()==UA){
		NT->Value=AA->EffectCount;
		}
		}
	}
	return true;
*/
void AcceptorAbilityCasts::GetViewMask(DString& ST){
	UnitAbility* UA=Ability.Get();
	ST="AcceptorAbilityCasts";
	if(UA)
		ST.print("(%s)",UA->Name.pchar());
	else
		ST+="(NotDefine)";
}
bool AcceptorAbilityCasts::GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
	dest="AcceptorAbilityCasts(%$Ability$%)";
	return true;
}
//bool AM_Level_Skill_A::Get(BaseType* BT){
//	NumericalType* NT=(NumericalType*)BT;
//	NT->Value=0;
//	OneObject* OB=GlobalUnitsContext::getCaster();
//	if( OB && OB->ActiveAbility ){
//		NT->Value=OB->ActiveAbility->AML_SkillA;
//	}
//	return true;
//}
//bool AM_Level_Skill_B::Get(BaseType* BT){
//	NumericalType* NT=(NumericalType*)BT;
//	NT->Value=0;
//	OneObject* OB=GlobalUnitsContext::getCaster();
//	if( OB && OB->ActiveAbility ){
//		NT->Value=OB->ActiveAbility->AML_SkillB;
//	}
//	return true;
//}

bool AntiMageSkillLevel::Get(BaseType* BT){
	NumericalType* NT=(NumericalType*)BT;
	NT->Value=0;
	UnitAbility* rUA=RefUA.Get();
	OneObject* OB=GlobalUnitsContext::getCaster();
	units_iterator::ActiveAbilities ItrAA;
	ItrAA.Create(OB);
	while(ActiveUnitAbility* AA=ItrAA.Next()){
		UnitAbility* UA=AA->GetA();
		if( rUA==UA && AA->TypeAbil==2 ){
			NT->Value=AA->Level;
			break;
		}
	}
	return true;
}

void AntiMageSkillLevel::GetViewMask(DString& ST){
	//BasicHeroParams::GetViewMask(ST);
	UnitAbility* UA=RefUA.Get();
	ST="AntiMageSkillLevel";
	if(UA)
		ST.print("(%s)",UA->Name.pchar());
	else
		ST+="(NotDefine)";
}
bool AntiMageSkillLevel::GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
	dest="AntiMageSkillLevel(%$RefUA$%)";
	return true;
}

bool BHP_Damage::Get(BaseType* BT){
	NumericalType* NT=(NumericalType*)BT;
	NT->Value=0;
	OneObject* OB=(Activator==0)?GlobalUnitsContext::getCaster():GlobalUnitsContext::getAcceptor();
	if(OB){
		AdvCharacter* ADC=OB->MoreCharacter;
		int dam=ADC->MaxDamage[0]+OB->AddDamage;
		if( OB->ActiveAbility && dam ){
			OB->ActiveAbility->ActiveAbilities.modifyDamage(0,dam,dam,NULL);
		}
		NT->Value=dam;
	}
	return true;
};
void BHP_Damage::GetViewMask(DString& ST){
	//BasicHeroParams::GetViewMask(ST);
	Enumerator* E=ENUM.Get("HeroParamsActivator");
	ST="Damage";
	ST.print("(%s)",E->GetStr(Activator));
};
bool BHP_Damage::GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
	dest="Damage(%$Activator$%)";
	return true;
};

extern ActiveUnitAbility* CurProcAA;

bool BHP_AbilityEffectTime::Get(BaseType* BT){
	NumericalType* NT=(NumericalType*)BT;
	NT->Value=0;
	if( CurProcAA!=NULL ){
		//UnitAbility* UA=CurProcAA->GetA();
		//if( UA && (CurProcAA->TypeAbil==0 || UA->ClassType==2 ) ){
		if( CurProcAA->TypeAbil==0 /*&& !UA->Aura*/ ){
			//OneObject* OB=GlobalUnitsContext::getAcceptor();
			//if(OB){
				int dT=TrueTime-CurProcAA->DeleteTime;
				NT->Value=dT;
			//}
		}
	}
	return true;
};
void BHP_AbilityEffectTime::GetViewMask(DString& ST){
	ST="AbilityEffectTime";
};
bool BHP_AbilityEffectTime::GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
	dest="AbilityEffectTime";
	return true;
};
