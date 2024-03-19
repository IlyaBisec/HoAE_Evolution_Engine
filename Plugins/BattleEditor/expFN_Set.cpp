#include "stdafx.h"

void	DeclareteDLL_expFN_Set(){

	GI::AddFunctionToDeclarationList("LINC_NODE_TO_GROUP");
	GI::AddParamsToFunctionInList("LINC_NODE_TO_GROUP",	"GROUP"	, "gGroup"	);
	GI::AddParamsToFunctionInList("LINC_NODE_TO_GROUP",	"NODE"	, "nNode"	);

	GI::AddFunctionToDeclarationList("SET_NODE_X");
	GI::AddParamsToFunctionInList("SET_NODE_X",	"NODE"	, "nNode"	);
	GI::AddParamsToFunctionInList("SET_NODE_X",	"INT"	, "iX"		);

	GI::AddFunctionToDeclarationList("SET_NODE_Y");
	GI::AddParamsToFunctionInList("SET_NODE_Y",	"NODE"	, "nNode"	);
	GI::AddParamsToFunctionInList("SET_NODE_Y",	"INT"	, "iY"		);

	GI::AddFunctionToDeclarationList("SET_NODE_R");
	GI::AddParamsToFunctionInList("SET_NODE_R",	"NODE"	, "nNode"	);
	GI::AddParamsToFunctionInList("SET_NODE_R",	"INT"	, "iR"		);

	GI::AddFunctionToDeclarationList("SET_NODE");
	GI::AddParamsToFunctionInList("SET_NODE",	"NODE"	, "nNode"	);
	GI::AddParamsToFunctionInList("SET_NODE",	"INT"	, "iX"		);
	GI::AddParamsToFunctionInList("SET_NODE",	"INT"	, "iY"		);
	GI::AddParamsToFunctionInList("SET_NODE",	"INT"	, "iR"		);

	GI::AddFunctionToDeclarationList("SET_NMASK");
	GI::AddParamsToFunctionInList("SET_NMASK",	"GROUP"	, "gGroup"	);
	GI::AddParamsToFunctionInList("SET_NMASK",	"INT"	, "iNMASK"	);

	GI::AddFunctionToDeclarationList("SET_NMASK_ARRAY");
	GI::AddParamsToFunctionInList("SET_NMASK_ARRAY",	"ARRAY"	, "arrGroups"	);
	GI::AddParamsToFunctionInList("SET_NMASK_ARRAY",	"INT"	, "iNMASK"		);

	GI::AddFunctionToDeclarationList("SET_StendGround");
	GI::AddParamsToFunctionInList("SET_StendGround",	"GROUP"	, "gGroup"	);
	GI::AddParamsToFunctionInList("SET_StendGround",	"BOOL"	, "bState"	);

	GI::AddFunctionToDeclarationList("SET_GROUP_EXPA");
	GI::AddParamsToFunctionInList("SET_GROUP_EXPA",	"GROUP"	, "gGroup"	);
	GI::AddParamsToFunctionInList("SET_GROUP_EXPA",	"INT"	, "iExpa"	);

	GI::AddFunctionToDeclarationList("SET_ALERT");
	GI::AddParamsToFunctionInList("SET_ALERT",	"NODE"	, "Node"	);
	GI::AddParamsToFunctionInList("SET_ALERT",	"INT"	, "Nation"	);

	GI::AddFunctionToDeclarationList("SET_DisableCapturability");
	GI::AddParamsToFunctionInList("SET_DisableCapturability",	"GROUP"	, "Group"	);

	GI::AddFunctionToDeclarationList("SET_InsideDialogState");
	GI::AddParamsToFunctionInList("SET_InsideDialogState",	"BOOL"	, "State"	);

	GI::AddFunctionToDeclarationList("SET_HeroColectExpaStatus");
	GI::AddParamsToFunctionInList("SET_HeroColectExpaStatus",	"GROUP"	, "gHero"	);
	GI::AddParamsToFunctionInList("SET_HeroColectExpaStatus",	"BOOL"	, "State"	);

	GI::AddFunctionToDeclarationList("SET_SwithTankMode");

	GI::AddFunctionToDeclarationList("SET_StopCameraTrack");

	GI::AddFunctionToDeclarationList("SET_RunCameraTrack");
	GI::AddParamsToFunctionInList("SET_RunCameraTrack",			"STR",		"TrackName" );

	GI::AddFunctionToDeclarationList("SET_CurrentAnimation");
	GI::AddParamsToFunctionInList("SET_CurrentAnimation",		"GROUP",	"Group"		);
	GI::AddParamsToFunctionInList("SET_CurrentAnimation",		"INT",		"AnimID"	);

	// Fro develop sub functions
	GI::AddFunctionToDeclarationList("SET_AddUserVariable");

};

EDITOR_FUNCTION	LINC_NODE_TO_GROUP					(__PARAMS__){
	__GROUP( gGroup );
	__NODE ( nNode  );
		
	if ( gGroup.Valid() && nNode.Valid() ){
		nNode.setX( gGroup.getXc() );
		nNode.setY( gGroup.getYc() );
		return 1;
	};
		
	END_FAIL;
};

EDITOR_FUNCTION	SET_NODE_X							(__PARAMS__){
	__NODE( nNode );
	__INT ( iX	  );
	
	if ( nNode.Valid() ){
		nNode.setX( iX );
		return 1;
	};
	
	END_FAIL;
};

EDITOR_FUNCTION	SET_NODE_Y							(__PARAMS__){
	__NODE( nNode );
	__INT ( iY	  );
	
	if ( nNode.Valid() ){
		nNode.setY( iY );
		return 1;
	};
	
	END_FAIL;
};

EDITOR_FUNCTION	SET_NODE_R							(__PARAMS__){
	__NODE( nNode );
	__INT ( iR	  );

	if ( nNode.Valid() ){
		nNode.setR( iR );
		return 1;
	};
	
	END_FAIL;
};

EDITOR_FUNCTION SET_NODE							(__PARAMS__){
	__NODE( nNode );
	__INT ( iX	  );
	__INT ( iY	  );
	__INT ( iR	  );
	
	if ( nNode.Valid() ){
		nNode.setX( iX );
		nNode.setY( iY );
		nNode.setR( iR );
		return 1;
	}

	END_FAIL;
};

EDITOR_FUNCTION SET_NMASK							(__PARAMS__){
	__GROUP	( gGroup );
	__INT	( iNMASK );
	
	if ( gGroup.Valid() ){
		ForEachUnit( gGroup, unit.SetMask( iNMASK ); );
	};

	END_FAIL;
};

EDITOR_FUNCTION SET_NMASK_ARRAY						(__PARAMS__){
	__ARRAY	( arrGroups	);
	__INT	( iNMASK	);
	
	if ( arrGroups.Valid() && arrGroups.getSIZE()>0 ){
		pGroup	GR;
		for ( int i=0; i<arrGroups.getSIZE(); i++ ){
			GR = arrGroups.getGROUP(i);
			if ( GR.Valid() ){
				ForEachUnit( GR, unit.SetMask( iNMASK ); );
			};
		};
		END_OK;
	};

	END_FAIL;
};

EDITOR_FUNCTION SET_StendGround						(__PARAMS__){
	__GROUP( gGroup );
	__BOOL ( bState );
	
	if ( gGroup.Valid() ){
		gGroup.setStandGround( bState );
		END_OK;
	};

	END_FAIL;
};

EDITOR_FUNCTION SET_GROUP_EXPA						(__PARAMS__){
	__GROUP	( gGroup );
	__INT	( iExpa  );
	
	if ( gGroup.Valid() ){
		ForEachUnit( gGroup, if ( unit.isHero() ) unit.GiveExpa( iExpa ) );
		END_OK;
	};

	END_FAIL;
};

EDITOR_FUNCTION SET_ALERT							(__PARAMS__){
	__NODE( Node   );
	__INT ( Nation );
	
	if ( Node.Valid() ){
		GI::AlertOnMap( Node.getX(), Node.getY(), 60*256, Nation );
		END_OK;
	};

	END_FAIL;
};

EDITOR_FUNCTION SET_DisableCapturability			(__PARAMS__){
	__GROUP( Group );
	
	if ( Group.Valid() ){
		Group.setCapturability( false );
		END_OK;
	};

	END_FAIL;
};

EDITOR_FUNCTION SET_InsideDialogState				(__PARAMS__){
	__BOOL( State );
	
	GI::SetInsideDialogState( State );

	END_OK;
};

EDITOR_FUNCTION SET_HeroColectExpaStatus			(__PARAMS__){
	__GROUP( gHero );
	__BOOL ( State );

	if ( gHero.Valid() ){
		ForEachUnit( gHero, if ( unit.isHero() ) unit.SetHeroCollectExpaStatus( State ) );
		END_OK;
	};

	END_FAIL;
};

EDITOR_FUNCTION SET_SwithTankMode					(__PARAMS__){
	GI::SwithTankMode();
	END_OK;
};

EDITOR_FUNCTION SET_RunCameraTrack					(__PARAMS__){
	__STR( TrackName );

	GI::RunCameraTrack( TrackName.pchar() );

	END_OK;
};

EDITOR_FUNCTION SET_StopCameraTrack					(__PARAMS__){
	GI::StopCameraTrack();
	END_OK;
};

EDITOR_FUNCTION SET_CurrentAnimation				(__PARAMS__){
	__GROUP( Group  );
	__INT  ( AnimID );

	if ( Group.Valid() ){
		pUnit	unit;
		for ( int i=0; i<Group.getUN(); i++ ){
			unit.Set( Group.getGIDi(i) );
			unit.SetCurrentAnimation( AnimID );
		}
		END_OK;
	}

	END_FAIL;
};

// Fro develop sub functions
EDITOR_FUNCTION SET_AddUserVariable					(__PARAMS__){
    END_OK;	
};
//////////////////////////////////////////////////////////////////////////























