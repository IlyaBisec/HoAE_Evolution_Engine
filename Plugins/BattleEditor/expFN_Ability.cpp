#include "stdafx.h"

void	DeclareteDLL_expFN_Ability(){

	GI::AddFunctionToDeclarationList("ABIL_AddCarryAbility");
	GI::AddParamsToFunctionInList("ABIL_AddCarryAbility",	"UnitAbility"	, "Ability"	);
	GI::AddParamsToFunctionInList("ABIL_AddCarryAbility",	"GROUP"			, "Group"	);

	GI::AddFunctionToDeclarationList("ABIL_DelCarryAbility_ptr");
	GI::AddParamsToFunctionInList("ABIL_DelCarryAbility_ptr",	"UnitAbility"	, "Ability"	);
	GI::AddParamsToFunctionInList("ABIL_DelCarryAbility_ptr",	"GROUP"			, "Group"	);

	GI::AddFunctionToDeclarationList("ABIL_DelCarryAbility_str");
	GI::AddParamsToFunctionInList("ABIL_DelCarryAbility_str",	"STR"	, "Ability"	);
	GI::AddParamsToFunctionInList("ABIL_DelCarryAbility_str",	"GROUP"	, "Group"	);

	GI::AddFunctionToDeclarationList("ABIL_DetectCarryAbility");
	GI::AddParamsToFunctionInList("ABIL_DetectCarryAbility",	"UnitAbility"	, "Ability"	);
	GI::AddParamsToFunctionInList("ABIL_DetectCarryAbility",	"GROUP"			, "Group"	);

	GI::AddFunctionToDeclarationList("ABIL_SetAbilityState");
	GI::AddParamsToFunctionInList("ABIL_SetAbilityState",	"GROUP"	, "Group"	);
	GI::AddParamsToFunctionInList("ABIL_SetAbilityState",	"STR"	, "AblName"	);
	GI::AddParamsToFunctionInList("ABIL_SetAbilityState",	"INT"	, "State"	);

	GI::AddFunctionToDeclarationList("ABIL_GetAbilityState");
	GI::AddParamsToFunctionInList("ABIL_GetAbilityState",	"GROUP"	, "Group"	);
	GI::AddParamsToFunctionInList("ABIL_GetAbilityState",	"STR"	, "AblName"	);
	GI::AddParamsToFunctionInList("ABIL_GetAbilityState",	"INT"	, "State"	);

	GI::AddFunctionToDeclarationList("ABIL_GetAbilityState_ref");
	GI::AddParamsToFunctionInList("ABIL_GetAbilityState_ref",	"GROUP"			, "Group"	);
	GI::AddParamsToFunctionInList("ABIL_GetAbilityState_ref",	"UnitAbility"	, "Ability"	);
	GI::AddParamsToFunctionInList("ABIL_GetAbilityState_ref",	"INT"			, "State"	);
		
	GI::AddFunctionToDeclarationList("ABIL_GetOwnerNation");
	GI::AddParamsToFunctionInList("ABIL_GetOwnerNation",	"INT"	, "Nation"	);

	GI::AddFunctionToDeclarationList("ABIL_CheckActivatorObject");
	GI::AddParamsToFunctionInList("ABIL_CheckActivatorObject",	"GROUP",	"Group");

	GI::AddFunctionToDeclarationList("ABIL_TryExecute");
	GI::AddParamsToFunctionInList("ABIL_TryExecute",	"UnitAbility"	, "Ability"	);
	GI::AddParamsToFunctionInList("ABIL_TryExecute",	"GROUP"			, "Caster"	);
	GI::AddParamsToFunctionInList("ABIL_TryExecute",	"GROUP"			, "Target"	);
	GI::AddParamsToFunctionInList("ABIL_TryExecute",	"NODE"			, "Node"	);

	GI::AddFunctionToDeclarationList("ABIL_Click_Teleport");

	GI::AddFunctionToDeclarationList("ABIL_Process_Teleport");
	GI::AddParamsToFunctionInList("ABIL_Process_Teleport",	"GROUP"	, "gDestGate"	);

	GI::AddFunctionToDeclarationList("WEAPON_CreateNew");
	GI::AddParamsToFunctionInList("WEAPON_CreateNew",	"WeaponModificators"	, "WeaponMod"	);
	GI::AddParamsToFunctionInList("WEAPON_CreateNew",	"NODE"					, "SrcNode"		);
	GI::AddParamsToFunctionInList("WEAPON_CreateNew",	"NODE"					, "DstNode"		);

	GI::AddFunctionToDeclarationList("WEAPON_CreateNew_ID");
	GI::AddParamsToFunctionInList("WEAPON_CreateNew_ID",	"WeaponModificators"	, "WeaponMod"	);
	GI::AddParamsToFunctionInList("WEAPON_CreateNew_ID",	"NODE"					, "SrcNode"		);
	GI::AddParamsToFunctionInList("WEAPON_CreateNew_ID",	"NODE"					, "DstNode"		);
	GI::AddParamsToFunctionInList("WEAPON_CreateNew_ID",	"GROUP"					, "SrcGrp"		);
	GI::AddParamsToFunctionInList("WEAPON_CreateNew_ID",	"GROUP"					, "DstGrp"		);
	
};

EDITOR_FUNCTION	ABIL_AddCarryAbility				(__PARAMS__){
	__ABILITY( Ability );
	__GROUP	 ( Group   );
	
	int NAA = 0;
	if ( Ability.Valid() && Group.Valid() ){
		int UN = Group.getUN();
		while ( UN-- ){
			if ( GI::AddCarryAbility( Group.getGIDi(UN), Ability ) ) NAA++;
		};
	};
	
	return NAA;
};

EDITOR_FUNCTION	ABIL_DelCarryAbility_ptr			(__PARAMS__){
	__ABILITY( Ability );
	__GROUP	 ( Group   );
	
	int NAA = 0;
	if ( Ability.Valid() && Group.Valid() ){
		int UN = Group.getUN();
		while ( UN-- ){
			if ( GI::DeleteCarryAbility( Group.getGIDi(UN), Ability ) ) NAA++;
		};
	};
	
	return NAA;
};

EDITOR_FUNCTION	ABIL_DelCarryAbility_str			(__PARAMS__){
	__STR(Ability);
	__GROUP(Group);
	
	int NAA = 0;
	if ( Ability.pchar()!=NULL && (Ability.pchar())[0]!=0 && Group.Valid() ){
		int UN = Group.getUN();
		pABILITY ABIL( Ability.pchar() );
		while ( UN-- ){
			if ( GI::DeleteCarryAbility( Group.getGIDi(UN), ABIL ) ) NAA++;
		};
	};
	
	return NAA;
};

EDITOR_FUNCTION	ABIL_DetectCarryAbility				(__PARAMS__){
	__ABILITY( Ability );
	__GROUP	 ( Group   );

	int NAA = 0;
	if ( Ability.Valid() && Group.Valid() ){
		int UN = Group.getUN();
		while ( UN-- ){
			if ( GI::DetectCarryAbility( Group.getGIDi(UN), Ability ) ) NAA++;
		};
	};
	
	return NAA;
};

EDITOR_FUNCTION	ABIL_SetAbilityState				(__PARAMS__){
	__GROUP ( Group   );
	__STR	( AblName );
	__INT	( State	  );
	
	if ( Group.Valid() && AblName.pchar()!=NULL && (AblName.pchar())[0]!=0 ){
		GI::SetAbilityState( Group, AblName, State );
	};
	
	END_OK;
};

EDITOR_FUNCTION	ABIL_GetAbilityState				(__PARAMS__){
	__GROUP	( Group   );
	__STR	( AblName );
	__INT	( State	  );
	
	if ( Group.Valid() && AblName.pchar()!=NULL && (AblName.pchar())[0]!=0 ){
		State = GI::GetAbilityState( Group, AblName );
		END_OK;
	};
	
	END_FAIL;
};

EDITOR_FUNCTION	ABIL_GetAbilityState_ref			(__PARAMS__){
	__GROUP	 ( Group   );
	__ABILITY( Ability );
	__INT	 ( State	  );
	
	if ( Group.Valid() && Ability.Valid() ){
		State=GI::GetAbilityState( Group, Ability );
		return State;
	};
	
	END_FAIL;
};

EDITOR_FUNCTION	ABIL_GetOwnerNation					(__PARAMS__){
	__INT ( Nation );
	
	Nation = GI::GetAbilityOBNation();
	
	END_OK;
};

EDITOR_FUNCTION	ABIL_CheckActivatorObject			(__PARAMS__){
	__GROUP	( Group );	

	return GI::CheckAbilityActivatorObject( Group );

	END_FAIL;
};

EDITOR_FUNCTION	ABIL_TryExecute						(__PARAMS__){
	__ABILITY	( Ability );
	__GROUP		( Caster  );
	pGroup		Target;
	if ( pPL->check("Target") ) Target = pPL->getGROUP("Target");
	iNode		ND;
	if ( pPL->check("Node") ) ND = pPL->getNODE("Node");
	
	if ( Ability.Valid()==false || Caster.Valid()==false ) END_FAIL;
	
	int __x = 0;
	int __y = 0;
	
	if ( ND.Valid() ){
		__x = ND.getX();
		__y = ND.getY();
	};
	
	return GI::AbilityExecute(Ability,Caster,Target,__x,__y);
};
EDITOR_FUNCTION	ABIL_Click_Teleport					(__PARAMS__){

	pGroup gGate; gGate.CreateLocal();
	GI::GetAbilityOwnerObject(gGate);
	if ( gGate.getUN()<=0 ) END_FAIL;
	pGroup gHero; gHero.CreateLocal();
	GI::GetAbilityActivatorObject(gHero);
	if ( gHero.getUN()<=0 ) END_FAIL;
	
	pUnit _uHero; _uHero = gHero.getGIDi(0);
	pUnit _uGate; _uGate = gGate.getGIDi(0);
		
	_uHero.GoToContainer( _uGate );
	
	END_OK;
};

EDITOR_FUNCTION	ABIL_Process_Teleport				(__PARAMS__){
	pGroup gGate; gGate.CreateLocal();
	GI::GetAbilityOwnerObject(gGate);
	pUnit _uGate; _uGate = gGate.getGIDi(0);
	if ( _uGate.GetInsideAmount()>0 ){
		pUnit _uHero; 
		pUnitsList UL; _uGate.GetInsideUnits(UL);
		_uHero = UL[0];

		//if ( _uHero.isHero() ){
		//	if ( _uHero.Hidden()==false && /*_uHero.IsBusy()==false &&*/ _uHero.IsStand() ){
		//		__GROUP(gDestGate);
		//		if ( gDestGate.getUN()>0 ){
		//			_uHero.Hide();
		//			_uHero.RemoveFromContainer();
		//			_uGate = gDestGate.getGIDi(0);
		//			_uHero.InsertToContainer(_uGate);
		//			END_OK;
		//		};
		//	}else if ( _uHero.Hidden() ){
		//		_uHero.UnHide();
		//		_uGate.PushOutUnit(_uHero);
		//		END_OK;
		//	};
		//};

		// by vital
		__GROUP(gDestGate);
		if ( gDestGate.getUN()>0 ){
			//_uHero.Hide();
			_uHero.RemoveFromContainer();
			_uGate = gDestGate.getGIDi(0);
			_uHero.InsertToContainer(_uGate);
			_uGate.PushOutUnit(_uHero);
			END_OK;
		}
	};

	END_FAIL;
};

EDITOR_FUNCTION	WEAPON_CreateNew					(__PARAMS__){
	__WEAPONMOD	( WeaponMod );
	__NODE		( SrcNode	);
	__NODE		( DstNode	);
	
	GI::CreateNewActiveWeaponMod(WeaponMod,-1,SrcNode.getX(),SrcNode.getY(),-1,DstNode.getX(),DstNode.getY());

	END_OK;
};

EDITOR_FUNCTION	WEAPON_CreateNew_ID					(__PARAMS__){
	__WEAPONMOD	( WeaponMod	);
	__NODE		( SrcNode	);
	__NODE		( DstNode	);

	int	l_srcID = -1;
	if ( pPL->check("SrcGrp") ) l_srcID=pPL->getGROUP("SrcGrp").getGIDi(0);
	int	l_dstID = -1;
	if ( pPL->check("DstGrp") ) l_dstID=pPL->getGROUP("DstGrp").getGIDi(0);
	GI::CreateNewActiveWeaponMod(WeaponMod,l_srcID,SrcNode.getX(),SrcNode.getY(),l_dstID,DstNode.getX(),DstNode.getY());
	
	END_OK;
};

//////////////////////////////////////////////////////////////////////////





























