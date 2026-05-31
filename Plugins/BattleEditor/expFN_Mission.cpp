#include "stdafx.h"

void	DeclareteDLL_expFN_Mission(){

	GI::AddFunctionToDeclarationList("MISS01_JoinAllyNearHero");
	GI::AddParamsToFunctionInList("MISS01_JoinAllyNearHero",	"GROUP"	, "gHero"		);
	GI::AddParamsToFunctionInList("MISS01_JoinAllyNearHero",	"NODE"	, "nHero"		);
	GI::AddParamsToFunctionInList("MISS01_JoinAllyNearHero",	"INT"	, "iAllyNI"		);
	GI::AddParamsToFunctionInList("MISS01_JoinAllyNearHero",	"INT"	, "iRandPow"	);
	GI::AddParamsToFunctionInList("MISS01_JoinAllyNearHero",	"GROUP"	, "dstGroup"	);
	GI::AddParamsToFunctionInList("MISS01_JoinAllyNearHero",	"BOOL"	, "addUnits"	);
	GI::AddParamsToFunctionInList("MISS01_JoinAllyNearHero",	"GROUP"	, "regGroup"	);
	GI::AddParamsToFunctionInList("MISS01_JoinAllyNearHero",	"BOOL"	, "addRegUnits"	);
	
	GI::AddFunctionToDeclarationList("MISS01_JoinAllyTypeNearHero");	
	GI::AddParamsToFunctionInList("MISS01_JoinAllyTypeNearHero",	"GROUP"		, "gHero"		);
	GI::AddParamsToFunctionInList("MISS01_JoinAllyTypeNearHero",	"NODE"		, "nHero"		);
	GI::AddParamsToFunctionInList("MISS01_JoinAllyTypeNearHero",	"UnitType"	, "iUnitType"	);
	GI::AddParamsToFunctionInList("MISS01_JoinAllyTypeNearHero",	"INT"		, "iAllyNI"		);
	GI::AddParamsToFunctionInList("MISS01_JoinAllyTypeNearHero",	"INT"		, "iRandPow"	);
	GI::AddParamsToFunctionInList("MISS01_JoinAllyTypeNearHero",	"GROUP"		, "dstGroup"	);
	GI::AddParamsToFunctionInList("MISS01_JoinAllyTypeNearHero",	"BOOL"		, "addUnits"	);
	GI::AddParamsToFunctionInList("MISS01_JoinAllyTypeNearHero",	"GROUP"		, "regGroup"	);
	GI::AddParamsToFunctionInList("MISS01_JoinAllyTypeNearHero",	"BOOL"		, "addRegUnits"	);

	GI::AddFunctionToDeclarationList("MISS01_SetGroupHP");
	GI::AddParamsToFunctionInList("MISS01_SetGroupHP",	"GROUP"	, "Group"	);
	GI::AddParamsToFunctionInList("MISS01_SetGroupHP",	"INT"	, "HP"		);

	GI::AddFunctionToDeclarationList("MISS01_ErazeAllUnitsInZone");
	GI::AddParamsToFunctionInList("MISS01_ErazeAllUnitsInZone",	"NODE"	, "Node"	);

	GI::AddFunctionToDeclarationList("MISS01_SendGroupByWay");
	GI::AddParamsToFunctionInList("MISS01_SendGroupByWay",	"GROUP"	, "Group"	);
	GI::AddParamsToFunctionInList("MISS01_SendGroupByWay",	"ARRAY"	, "PosArr"	);

	GI::AddFunctionToDeclarationList("MISS01_CreateAndSendByWay");
	GI::AddParamsToFunctionInList("MISS01_CreateAndSendByWay",	"ARRAY"			, "PosArr"			);
	GI::AddParamsToFunctionInList("MISS01_CreateAndSendByWay",	"INT"			, "Nation"			);
	GI::AddParamsToFunctionInList("MISS01_CreateAndSendByWay",	"UnitType"		, "UnitType"		);
	GI::AddParamsToFunctionInList("MISS01_CreateAndSendByWay",	"FurmationType"	, "FormationType"	);

	GI::AddFunctionToDeclarationList("MISS03_SetLivingSpace");
	GI::AddParamsToFunctionInList("MISS03_SetLivingSpace",	"INT"	, "iNation"			);
	GI::AddParamsToFunctionInList("MISS03_SetLivingSpace",	"INT"	, "iLivingSpace"	);
	GI::AddParamsToFunctionInList("MISS03_SetLivingSpace",	"BOOL"	, "bSET"			);	

};

EDITOR_FUNCTION	MISS01_JoinAllyNearHero				(__PARAMS__){
	__GROUP		( gHero		);
	__ROUND_ZONE( nHero		);
	__INT		( iAllyNI	);
	__INT		( iRandPow	);

	int			HeroNI 	= gHero.getNation();
	pUnitsList	UL;
	nHero.CollectUnits(UL);
	UL.LimitByNation(iAllyNI);
	UL.RemoveDeadUnits();
	pUnitsList	dstUL;
	pUnitsList	regUL;
	ForEachUnit	(	UL, 
					if ( GI::random(0,100)<=iRandPow )
					{ 
						unit.SetNation( HeroNI );
						unit.ClearOrders();
						dstUL += unit;
					}else{
						regUL += unit;
					};
				);
	if ( pPL->check("dstGroup") ){
		// fill destenation group with current units
		__GROUP( dstGroup );
		if ( pPL->check("addUnits") ){
			__BOOL(addUnits);
			if ( addUnits==false ) dstGroup.RemAllUnits();
		};
		ForEachUnit( dstUL, dstGroup.AddUnitGID( unit.ID ); );
	};
	if ( pPL->check("regGroup") ){
		// fill destenation group with current units
		__GROUP( regGroup );
		if ( pPL->check("addRegUnits") ){
			__BOOL(addRegUnits);
			if ( addRegUnits==false ) regGroup.RemAllUnits();
		};
		ForEachUnit( regUL, regGroup.AddUnitGID( unit.ID ); );
	};
	
	END_FAIL;
};

EDITOR_FUNCTION MISS01_JoinAllyTypeNearHero			(__PARAMS__){
	__GROUP		( gHero		);
	__ROUND_ZONE( nHero		);
	__INT		( iAllyNI	);
	__UNIT_TYPE	( iUnitType	);
	__INT		( iRandPow	);
	
	int			HeroNI 	= gHero.getNation();
	pUnitsList	UL;
	nHero.CollectUnits(UL);
	UL.LimitByNation(iAllyNI);
	UL.LimitByType(iUnitType);
	UL.RemoveDeadUnits();
	pUnitsList	dstUL;
	pUnitsList	regUL;
	ForEachUnit	(	UL, 
					if ( GI::random(0,100)<=iRandPow )
					{ 
						unit.SetNation( HeroNI );
						unit.ClearOrders();
						dstUL += unit;
					}else{
						regUL += unit;
					};
				);

	if ( pPL->check("dstGroup") ){
		// fill destenation group with current units
		__GROUP( dstGroup );
		if ( pPL->check("addUnits") ){
			__BOOL(addUnits);
			if ( addUnits==false ) dstGroup.RemAllUnits();
		};
		ForEachUnit( dstUL, dstGroup.AddUnitGID( unit.ID ); );
	};
	if ( pPL->check("regGroup") ){
		// fill destenation group with current units
		__GROUP( regGroup );
		if ( pPL->check("addRegUnits") ){
			__BOOL(addRegUnits);
			if ( addRegUnits==false ) regGroup.RemAllUnits();
		};
		ForEachUnit( regUL, regGroup.AddUnitGID( unit.ID ); );
	};
		
	END_OK;
};

EDITOR_FUNCTION	MISS01_SetGroupHP					(__PARAMS__){
	__GROUP	( Group );
	__INT	( HP	);
	
	Group.setHP(HP);

	END_OK;
};

EDITOR_FUNCTION	MISS01_ErazeAllUnitsInZone			(__PARAMS__){
	__ROUND_ZONE(Node);

	pUnitsList	UL;
	Node.CollectUnits(UL);
	UL.RemoveDeadUnits();
	ForEachUnit( UL, unit.Erase() );

	END_OK;
};

EDITOR_FUNCTION MISS01_SendGroupByWay				(__PARAMS__){
	__GROUP(Group);
	__ARRAY(PosArr);
	
	return	l_SendGroupByWay(Group,PosArr);

	END_FAIL;
};
EDITOR_FUNCTION MISS01_CreateAndSendByWay			(__PARAMS__){
	__ARRAY		( PosArr		);
	__INT		( Nation		);
	__UNIT_TYPE	( UnitType		);
	__INT		( FormationType	);
		
	iNode	Node = PosArr.getNODE(0);
	if (Node.Valid()==false) return 0;
	pGroup gAlly;
	GI::CreateFormation(gAlly,Node.getX(),Node.getY(),Nation,UnitType,FormationType);
	l_SendGroupByWay(gAlly,PosArr);
	
	END_OK;
};
EDITOR_FUNCTION	MISS03_SetLivingSpace				(__PARAMS__){
	__INT(iNation);
	__INT(iLivingSpace);
	__BOOL(bSET);

	return GI::SetAddFarms(iNation,iLivingSpace,bSET);

	END_FAIL;
};
//////////////////////////////////////////////////////////////////////////






















