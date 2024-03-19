#include "stdafx.h"

void	DeclareteDLL_expFN_Node(){
	
	GI::AddFunctionToDeclarationList("NODE_CHENGE_NMASK");
	GI::AddParamsToFunctionInList("NODE_CHENGE_NMASK",	"INT"	, "Nation"	);
	GI::AddParamsToFunctionInList("NODE_CHENGE_NMASK",	"NODE"	, "Node"	);
	GI::AddParamsToFunctionInList("NODE_CHENGE_NMASK",	"INT"	, "NMask"	);

	GI::AddFunctionToDeclarationList("CAMERA_Play");
	GI::AddParamsToFunctionInList("CAMERA_Play",		"CAMERA",	"Camera"	);

	GI::AddFunctionToDeclarationList("CAMERA_Stop");
	GI::AddParamsToFunctionInList("CAMERA_Stop",		"CAMERA",	"Camera"	);

	GI::AddFunctionToDeclarationList("CAMERA_Free");
	GI::AddParamsToFunctionInList("CAMERA_Free",		"CAMERA",	"Camera"	);

	GI::AddFunctionToDeclarationList("CAMERA_IsPlaying");
	GI::AddParamsToFunctionInList("CAMERA_IsPlaying",	"CAMERA",	"Camera"	);

};

EDITOR_FUNCTION NODE_CHENGE_NMASK					(__PARAMS__){
	__INT		( Nation );
	__ROUND_ZONE( Node	 );
	__INT		( NMask	 );

	pUnitsList UL;
	Node.CollectUnits(UL);
	UL.LimitByNation(Nation);
	UL.RemoveDeadUnits();
	ForEachUnit( UL, unit.SetMask(NMask) );

	END_OK;
};

EDITOR_FUNCTION	CAMERA_Play							(__PARAMS__){
	__CAMERA( Camera );

	GI::CAMERA_Play( Camera );

	END_OK;
}

EDITOR_FUNCTION	CAMERA_Stop							(__PARAMS__){
	__CAMERA( Camera );

	GI::CAMERA_Stop( Camera );

	END_OK;
}

EDITOR_FUNCTION	CAMERA_Free							(__PARAMS__){
	__CAMERA( Camera );

	GI::CAMERA_Free( Camera );

	END_OK;
}

EDITOR_FUNCTION	CAMERA_IsPlaying					(__PARAMS__){
	__CAMERA( Camera );

	if ( GI::CAMERA_IsPlaying( Camera ) ) END_OK;

	END_FAIL;
}

//////////////////////////////////////////////////////////////////////////






















