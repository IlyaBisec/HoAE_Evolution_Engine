#include "stdafx.h"

void	DeclareteDLL_expFN_Group(){

	GI::AddFunctionToDeclarationList("RemoveUnitsInZoneToGroup");
	GI::AddParamsToFunctionInList("RemoveUnitsInZoneToGroup",	"GROUP"	, "srceGR"	);
	GI::AddParamsToFunctionInList("RemoveUnitsInZoneToGroup",	"GROUP"	, "destGR"	);
	GI::AddParamsToFunctionInList("RemoveUnitsInZoneToGroup",	"NODE"	, "trnsND"	);

	GI::AddFunctionToDeclarationList("GROUP_AddSelected");
	GI::AddParamsToFunctionInList("GROUP_AddSelected",	"GROUP"	, "Group"	);
	GI::AddParamsToFunctionInList("GROUP_AddSelected",	"INT"	, "Nation"	);

	GI::AddFunctionToDeclarationList("GROUP_AddInZone");
	GI::AddParamsToFunctionInList("GROUP_AddInZone",	"GROUP"	, "Group"	);
	GI::AddParamsToFunctionInList("GROUP_AddInZone",	"INT"	, "Nation"	);
	GI::AddParamsToFunctionInList("GROUP_AddInZone",	"NODE"	, "Node"	);
	GI::AddParamsToFunctionInList("GROUP_AddInZone",	"BOOL"	, "Add"		);

	GI::AddFunctionToDeclarationList("GROUP_AddTypeInZone");
	GI::AddParamsToFunctionInList("GROUP_AddTypeInZone",	"GROUP"		, "Group"	);
	GI::AddParamsToFunctionInList("GROUP_AddTypeInZone",	"INT"		, "Nation"	);
	GI::AddParamsToFunctionInList("GROUP_AddTypeInZone",	"UnitType"	, "UnitType");
	GI::AddParamsToFunctionInList("GROUP_AddTypeInZone",	"NODE"		, "Node"	);
	GI::AddParamsToFunctionInList("GROUP_AddTypeInZone",	"BOOL"		, "Add"		);

	GI::AddFunctionToDeclarationList("GROUP_SetLockType");
	GI::AddParamsToFunctionInList("GROUP_SetLockType",	"GROUP"	, "Group"	);
	GI::AddParamsToFunctionInList("GROUP_SetLockType",	"INT"	, "LockType");

	GI::AddFunctionToDeclarationList("GROUP_PutRandFormation");
	GI::AddParamsToFunctionInList("GROUP_PutRandFormation",	"GROUP"		, "Group"	);
	GI::AddParamsToFunctionInList("GROUP_PutRandFormation",	"NODE"		, "Node"	);
	GI::AddParamsToFunctionInList("GROUP_PutRandFormation",	"INT"		, "Nation"	);
	GI::AddParamsToFunctionInList("GROUP_PutRandFormation",	"UnitType"	, "UnitType");
	GI::AddParamsToFunctionInList("GROUP_PutRandFormation",	"INT"		, "minN"	);
	GI::AddParamsToFunctionInList("GROUP_PutRandFormation",	"INT"		, "maxN"	);

	GI::AddFunctionToDeclarationList("GROUP_GetNationAmout");
	GI::AddParamsToFunctionInList("GROUP_GetNationAmout",	"GROUP"	, "Group"	);
	GI::AddParamsToFunctionInList("GROUP_GetNationAmout",	"INT"	, "Nation"	);

	GI::AddFunctionToDeclarationList("GROUP_RemoveAllUnits");
	GI::AddParamsToFunctionInList("GROUP_RemoveAllUnits",	"GROUP"	, "Group"	);

	GI::AddFunctionToDeclarationList("NATION_ClearSelection");
	GI::AddParamsToFunctionInList("NATION_ClearSelection",	"INT"	, "Nation"	);

	GI::AddFunctionToDeclarationList("GROUP_FreeFromFSM");
	GI::AddParamsToFunctionInList("GROUP_FreeFromFSM",	"GROUP"	, "Group"	);

	// ATTACK_FN

	GI::AddFunctionToDeclarationList("GROUP_ATTACK_GROUP");
	GI::AddParamsToFunctionInList("GROUP_ATTACK_GROUP",	"GROUP"	, "gAttack"	);
	GI::AddParamsToFunctionInList("GROUP_ATTACK_GROUP",	"GROUP"	, "gTarget"	);

	GI::AddFunctionToDeclarationList("GROUP_ATTACK_NODE");
	GI::AddParamsToFunctionInList("GROUP_ATTACK_NODE",	"GROUP"	, "gGroup"	);
	GI::AddParamsToFunctionInList("GROUP_ATTACK_NODE",	"NODE"	, "nNode"	);

	GI::AddFunctionToDeclarationList("ADD_STORM");
	GI::AddParamsToFunctionInList("ADD_STORM",	"GROUP"	, "gGroup"	);

	GI::AddFunctionToDeclarationList("GROUP_ATTACK_00");
	GI::AddParamsToFunctionInList("GROUP_ATTACK_00",	"GROUP"	, "gAttaker"	);
	GI::AddParamsToFunctionInList("GROUP_ATTACK_00",	"GROUP"	, "gTarget"		);
	GI::AddParamsToFunctionInList("GROUP_ATTACK_00",	"INT"	, "iPower"		);

	GI::AddFunctionToDeclarationList("GROUP_EXPLORING_MAP");
	GI::AddParamsToFunctionInList("GROUP_EXPLORING_MAP",	"GROUP"	, "gGroup"	);

};

EDITOR_FUNCTION RemoveUnitsInZoneToGroup			(__PARAMS__){
	__GROUP	( srceGR );
	__GROUP	( destGR );
	__NODE	( trnsND );

	if ( srceGR.Valid() && destGR.Valid() && trnsND.Valid() ){
		return srceGR.RemUnitsInZone(trnsND,destGR);
	};

	END_FAIL;
};

EDITOR_FUNCTION GROUP_AddSelected					(__PARAMS__){
	__GROUP	( Group	 );
	__INT	( Nation );

	if ( Group.Valid() ){
		return Group.AddSelectedUnits(Nation);
	};

	END_FAIL;
};

EDITOR_FUNCTION GROUP_AddInZone						(__PARAMS__){
	__GROUP	( Group	 );
	__INT	( Nation );
	__NODE	( Node	 );
	__BOOL	( Add	 );

	if ( Group.Valid() && Node.Valid() ){
		Group.AddUnitsInZone(Nation,Node,Add);
		return 1;
	};

	END_FAIL;
};

EDITOR_FUNCTION GROUP_AddTypeInZone					(__PARAMS__){
	__GROUP		( Group    );
	__INT		( Nation   );
	__UNIT_TYPE	( UnitType );
	__NODE		( Node     );
	__BOOL		( Add	   );
	
	if ( Group.Valid() && Node.Valid() ){ 
		Group.AddUnitsTypeInZone(Nation,UnitType.GetType(),Node,Add);
		return 1;
	};

	END_FAIL;
};

EDITOR_FUNCTION GROUP_SetLockType					(__PARAMS__){
	__GROUP	( Group	   );
	__INT	( LockType );

	if ( Group.Valid() ){
		Group.setLockType(LockType);
		return 1;
	};

	END_FAIL;
};

EDITOR_FUNCTION GROUP_PutRandFormation				(__PARAMS__){
	__GROUP		( Group		);
	__NODE		( Node		);
	__INT		( Nation	);
	__UNIT_TYPE	( UnitType	);
	__INT		( minN		);
	__INT		( maxN		);
	
	if ( Group.Valid() && Node.Valid() ){
		int _UN = GI::random(minN,maxN);
		pUnitsList UL;
		GI::CreateRandomUnits(_UN,Node.getX(),Node.getY(),Node.getR(),Nation,UnitType,&UL);
		ForEachUnit( UL, Group._add(unit) );
	};

	END_FAIL;
};

EDITOR_FUNCTION GROUP_GetNationAmout				(__PARAMS__){
	__GROUP	( Group	 );
	__INT	( Nation );
	
	if ( Group.Valid() ){
		return Group.getUN(Nation);
	};

	END_FAIL;
};


EDITOR_FUNCTION GROUP_RemoveAllUnits				(__PARAMS__){
	__GROUP( Group );
	
	if ( Group.Valid() ){
		Group.RemAllUnits();
		return 1;
	};

	END_FAIL;
};

EDITOR_FUNCTION NATION_ClearSelection				(__PARAMS__){
	__INT( Nation );
	
	(pSelected::units(Nation)).Clear();
	(pImSelected::units(Nation)).Clear();
	
	END_OK;
};

EDITOR_FUNCTION	GROUP_FreeFromFSM					(__PARAMS__){
	__GROUP( Group );

	if ( Group.Valid() ){
		if ( GI::FreeSO_pVAR(Group) ){
			Group.setAIGuard(false);
			END_OK;
		};
	};

	END_FAIL;
};

//////////////////////////////////////////////////////////////////////////
// ATTACK_FN			
//////////////////////////////////////////////////////////////////////////
EDITOR_FUNCTION GROUP_ATTACK_GROUP					(__PARAMS__){
	__GROUP( gAttack );
	__GROUP( gTarget );
		
	if ( gAttack.Valid() && gTarget.Valid() ){
		gAttack.AttackUseNearest( gTarget );
		END_OK;
	};

	END_FAIL;
};

EDITOR_FUNCTION GROUP_ATTACK_NODE					(__PARAMS__){
	__GROUP( gGroup );
	__NODE ( nNode  );
	
	if ( gGroup.Valid() && nNode.Valid() ){
		pGroup gTarget;
		gTarget.CreateLocal();
		gTarget.AddEnemyUnitsInZone( gGroup.getNation(), nNode, false );
		if ( gTarget.getUN()>0 ){
			gGroup.AttackUseNearest( gTarget );
		};
		END_OK;
	};

	END_FAIL;
};

EDITOR_FUNCTION ADD_STORM							(__PARAMS__){
	__GROUP( gGroup );

	if ( gGroup.Valid() ){
		GI::AddTomahawksAttackers( gGroup, gGroup.getNation(),true,0,0,0);
		END_OK;
	};
	
	END_FAIL;
};

EDITOR_FUNCTION GROUP_ATTACK_00						(__PARAMS__){
	__GROUP( gAttaker );
	__GROUP( gTarget  );
	__INT  ( iPower   );
	
	if ( gAttaker.Valid() ){
		gAttaker.Attack( gTarget, iPower );
		END_OK;
	};

	END_FAIL;
};

EDITOR_FUNCTION GROUP_EXPLORING_MAP					(__PARAMS__){
	__GROUP( gGroup );
	
	if ( gGroup.Valid() ){
		gGroup.ExploringMap();
		END_OK;
	};

	END_FAIL;
};


//////////////////////////////////////////////////////////////////////////























