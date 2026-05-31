//==================================================================================================================//
#include "stdheader.h"
#include "SuperEditor.h"
#include "WeaponSystem.h"
#include "unitability.h"
#include "unitability2.h"
#include "UnitTypeGroup.h"
#include "cvi_singleplayerdata.h"
#include "BrigadeAINeuro.h"
#include "HeroFunctions.h"
//==================================================================================================================//
void RegisterSuperEditorClasses(){
	REG_CLASS(Operand);
	REG_CLASS_EX(NumericalConst,"Ariphmetical");
	REG_CLASS_EX(Op_Add,"Ariphmetical");
	REG_CLASS_EX(UnarMinus,"Ariphmetical");
	REG_CLASS_EX(Brackets,"Ariphmetical");
	REG_CLASS_AMBIGUOUS_EX(NumericalVariableReference,NumericalAcceptor,"Ariphmetical");
	REG_CLASS_AMBIGUOUS_EX(NumericalArrayVariableReference,NumericalArrayAcceptor,"Ariphmetical");
	REG_CLASS_AMBIGUOUS_EX(NumericalArrayElement,NumericalAcceptor,"Ariphmetical");
	REG_CLASS_EX(Assignment,"Ariphmetical");
	REG_CLASS_EX(Logical,"Ariphmetical");
	REG_CLASS_EX(CompareFun,"Ariphmetical");
	REG_CLASS_EX(If_Else,"Ariphmetical");
	REG_CLASS_EX(ForEachNumerical,"Ariphmetical");
	REG_CLASS_EX(ForFromTo,"Ariphmetical");
	REG_CLASS(BaseType);
	REG_CLASS_EX(NumericalType,"Ariphmetical");
	REG_CLASS_EX(BoolType,"Ariphmetical");
	REG_CLASS_EX(NumericalArray,"Ariphmetical");
	REG_CLASS_EX(qUnitType,"MissionsScript");
	REG_CLASS_EX(qBrigadeFormationType,"MissionsScript");
	//REG_CLASS_EX(BoolArray,"Ariphmetical");
	REG_CLASS(Variable);
	REG_CLASS(Returner); 
	REG_CLASS(Acceptor);
	REG_CLASS(NumericalReturner);
	REG_CLASS(NumericalAcceptor);	
	REG_CLASS(BoolReturner);
	REG_CLASS(BoolAcceptor);
	REG_CLASS(StringReturner);
	REG_CLASS(StringAcceptor);

	// types
	REG_CLASS_EX(StringType,"Ariphmetical");
	REG_CLASS_EX(StringConst,"Ariphmetical");	
	REG_CLASS_AMBIGUOUS_EX(StringVariableReference,StringAcceptor,"Ariphmetical");
	// functions
	REG_CLASS_EX(qfCondRandom,"Ariphmetical");
	// procedures
	REG_CLASS_EX(qpChatHint,"Ariphmetical");
	//
	//REG_CLASS_EX(AM_Level_Skill_A,"AntiMageSkills");
	//REG_CLASS_EX(AM_Level_Skill_B,"AntiMageSkills");
	REG_CLASS_EX(BHP_AbilityEffectTime,"UnitAbility");

	REG_CLASS_EX(AntiMageSkillLevel,"AcceptorAndCasterParams");
	REG_CLASS_EX(BHP_Damage,"AcceptorAndCasterParams");
	REG_CLASS_EX(AcceptorHeroLevel,"AcceptorAndCasterParams");
	REG_CLASS_EX(CasterHeroLevel,"AcceptorAndCasterParams");
	REG_CLASS_EX(AcceptorTrueLevel,"AcceptorAndCasterParams");
	REG_CLASS_EX(CasterTrueLevel,"AcceptorAndCasterParams");
	REG_CLASS_EX(AcceptorMana,"AcceptorAndCasterParams");
	REG_CLASS_EX(CasterMana,"AcceptorAndCasterParams");
	REG_CLASS_EX(AcceptorLife,"AcceptorAndCasterParams");
	REG_CLASS_EX(CasterLife,"AcceptorAndCasterParams");
	REG_CLASS_EX(AcceptorMaxLife,"AcceptorAndCasterParams");
	REG_CLASS_EX(CasterMaxLife,"AcceptorAndCasterParams");
	REG_CLASS_EX(CasterAbilityCasts,"AcceptorAndCasterParams");
	REG_CLASS_EX(AcceptorAbilityCasts,"AcceptorAndCasterParams");

	REG_CLASS_EX(CGetGameTime,"MissionsScript"); 
	REG_CLASS_AMBIGUOUS_EX(BVariables,NumericalAcceptor,"MissionsScript");
	REG_CLASS_EX(GetPlayerNation,"MissionsScript"); 
	REG_CLASS_EX(GetPlayerSelectedBrigadeID,"MissionsScript"); 
	REG_CLASS_EX(SetCurrentNationColor,"MissionsScript"); 
	REG_CLASS_EX(SetCurrentBrigadeID,"MissionsScript"); 

	REG_CLASS_EX(PlayVideo,"Video"); 
	REG_CLASS_EX(CPlaySound,"Video"); 
	REG_CLASS_EX(OneStringExpressions,"MissionsScript");
	REG_CLASS_EX(GetHeroVariable,"MissionsScript");
	
	//==============================================================//
	REG_CLASS(NewFrame);
	//==============================================================//

	//==============================================================//
	//=================         Neuro          =====================//
	//==============================================================//
	REG_CLASS(Neuro);
	REG_CLASS(Neuro2AutoForm);
	REG_CLASS(CNeuroStorage);
	//==============================================================//

}
//==================================================================================================================//
