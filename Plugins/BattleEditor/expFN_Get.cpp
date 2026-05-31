#include "stdafx.h"

void	DeclareteDLL_expFN_Get(){

	GI::AddFunctionToDeclarationList("GET_NoOrders_UN");
	GI::AddParamsToFunctionInList("GET_NoOrders_UN",	"GROUP"	, "Group"	);

	GI::AddFunctionToDeclarationList("GET_Move_UN");
	GI::AddParamsToFunctionInList("GET_Move_UN",	"GROUP"	, "Group"	);

	GI::AddFunctionToDeclarationList("GET_Attack_UN");
	GI::AddParamsToFunctionInList("GET_Attack_UN",	"GROUP"	, "Group"	);

	GI::AddFunctionToDeclarationList("GET_GroupHP");
	GI::AddParamsToFunctionInList("GET_GroupHP",	"GROUP"	, "Group"	);

	GI::AddFunctionToDeclarationList("GET_NKilled");
	GI::AddParamsToFunctionInList("GET_NKilled",	"INT"		, "Nation"	);
	GI::AddParamsToFunctionInList("GET_NKilled",	"UnitType"	, "UnitType");

	GI::AddFunctionToDeclarationList("GET_HeroLevel");
	GI::AddParamsToFunctionInList("GET_HeroLevel",	"GROUP"	, "gHero"	);

	GI::AddFunctionToDeclarationList("GET_InsideDialogState");

	GI::AddFunctionToDeclarationList("GET_TankMode");

	GI::AddFunctionToDeclarationList("GET_GroupDeadUN");
	GI::AddParamsToFunctionInList("GET_GroupDeadUN", "GROUP", "Group");

	GI::AddFunctionToDeclarationList("GET_CheckIfCameraIsMoving");

};

EDITOR_FUNCTION	GET_NoOrders_UN						(__PARAMS__){
	__GROUP( Group );

	int N = 0;
	if ( Group.Valid() ){
		N = Group.getNO_ORDERS_UN();
	};

	return N;
};

EDITOR_FUNCTION	GET_Move_UN							(__PARAMS__){
	__GROUP( Group );

	int N = 0;
	if ( Group.Valid() ){
		N = Group.getMOVE_UN();
	};

	return N;
};

EDITOR_FUNCTION	GET_Attack_UN						(__PARAMS__){
	__GROUP( Group );

	int N = 0;
	if ( Group.Valid() ){
		N = Group.getATTACK_UN();
	};

	return N;
};

EDITOR_FUNCTION	GET_GroupHP							(__PARAMS__){		// SUM(GR[i].HP,GR.N)/GR.N
	__GROUP( Group );
	
	if ( Group.Valid() ){
		return Group.getHP();
	};

	END_FAIL;	
};

EDITOR_FUNCTION	GET_NKilled							(__PARAMS__){
	__INT		( Nation   );
	__UNIT_TYPE	( UnitType );	
	
	return GI::GetNKilled( Nation, UnitType );
};

EDITOR_FUNCTION	GET_HeroLevel						(__PARAMS__){
	__GROUP( gHero );
	
	if ( gHero.Valid() && gHero.getUN()==1 && gHero[0].isHero()==true ){
		return gHero[0].GetHeroLevel();
	};
	
	END_FAIL;
};

EDITOR_FUNCTION	GET_InsideDialogState				(__PARAMS__){
	return (bool)(GI::GetInsideDialogState());
};

EDITOR_FUNCTION	GET_TankMode						(__PARAMS__){
	return GI::GetTankMode();	
};

EDITOR_FUNCTION	GET_GroupDeadUN						(__PARAMS__){
	__GROUP( Group );
	
	if ( Group.Valid() ) return Group.getDeadUN();

    END_FAIL;
};

EDITOR_FUNCTION	GET_CheckIfCameraIsMoving			(__PARAMS__){
	return	GI::CheckIfCameraIsMoving();	
};

//////////////////////////////////////////////////////////////////////////




























