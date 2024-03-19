#include "stdafx.h"

void	DeclareteDLL_fsm_Saboteur(){

	GI::AddFunctionToDeclarationList("ACF_SABOTEUR");
	GI::AddParamsToFunctionInList("ACF_SABOTEUR",	"GROUP"	, "Object"		);
	GI::AddParamsToFunctionInList("ACF_SABOTEUR",	"INT"	, "AREAID"		);
	GI::AddParamsToFunctionInList("ACF_SABOTEUR",	"BOOL"	, "FREE_HUNT"	);

};

/*
 Description: 
	ƒанный конечный автомат предназначен дл€ следовани€ к
	намеченной позиции избега€ стычек с врагом. Ќе принимает
	самосто€тельно решений о изменении позиции следовани€. 
	ѕо прибытии атакует все и вс€. ћожет вступить по-пути в бой
	только в случаее если враг слабее или был атакован им.
 Parameters:
	"GROUP"		- юниты вход€щие в группу
	"AREAID"	- позици€ следовани€
 MSGS:
	"msg_RESTART"	-	остановитьс€ и начать движение заново,
						пересчитав путь ( как параметр - нова€
						позици€ "NEW_AREAID" )
 STATES:
	"ST_GLOBAL"		- начальное состо€ние дл€ FSM 				-> ST_IDLE
	"ST_MOVE"		- выполн€ем приказ по передвижению			-> ST_NEWSTEP
	"ST_NEWSTEP"	- считаем новый путь и отправл€ем в путь	-> ST_MOVE, ST_ATTACK
	"ST_ATTACK"		- атакуем всех р€дом						-> ST_NEWSTEP
	"ST_TERMINATE"	- точка назначени€, убиваем всех			-> ST_NEWSTEP
 */

EDITOR_FUNCTION	FSM_SABOTEUR( pSMARTOBJECT& C_OBJ, _str& state, pMSG& msg ){
	FSM_CHECKP();
	if ( SO_GROUP_IS_DEAD(C_OBJ) ) END_FAIL;

	pGroup		_group 	= C_OBJ.getOBJ_GROUP();
	pParamList	_par	= C_OBJ.getPARAMS();
	if ( _par.Valid()==false || _group.Valid()==false )	END_FAIL;
			
	if ( state=="ST_GLOBAL" ){
		// »спользуем параметр свободного действи€	//
		bool& _FreeHunt = _par.getBOOL("FREE_HUNT");
		if ( _FreeHunt ){
			if ( _par.check("HUNT_WAIT")==false )	_par.addINT("HUNT_WAIT")=20;
			int& _HuntWait = _par.getINT("HUNT_WAIT");
			if ( _HuntWait<=0 ){
				_par.getINT("AREAID") = -1;
				FINDE_USE_AST_RATE( C_OBJ, "AREAID" );
				_HuntWait = 20;
			}else{
				_HuntWait -= 1;
			};
		};
		// *************************************** //
	};
			
	// ≈сли в группе добавились новые юниты -->> ST_NEWSTEP !!! //
	if ( state!="ST_GLOBAL" && _group.getNew_UN(true)>0 ){
		C_OBJ.next_STATE("ST_NEWSTEP");
		C_OBJ.FSC(true);
	};
	// ******************************************************** //
	
	if 		( state == "ST_GLOBAL" 	){
		// ѕодготовить объект -> ST_NEWSTEP			//
		if 		( msg.str() == "MSG_UPDATE"  ){
			if ( _par.getINT("AREAID")!=-1 ) __SetState(ST_NEWSTEP);
		};
		END_OK;
	}else if( state == "ST_NEWSTEP"	){
		ST_SABOTEUR_STEP_00( C_OBJ, msg, "AREAID", "ST_MOVE", "ST_ATTACK", "ST_TERMINATE" );
		END_OK;
	}else if( state == "ST_MOVE" 	){
		// —ледим за передвижением к позиции		//
		ST_MOVE_00( C_OBJ, msg, "l_x", "l_y", "ST_NEWSTEP" );
		END_OK;
	}else if( state == "ST_ATTACK" 	){
		// Ѕъем всех в округе до победы или смерти	//
		ST_ATTACK_01( C_OBJ, msg, "ST_NEWSTEP" );
		END_OK;
	}else if( state == "ST_TERMINATE" 	){
		// ƒостигли цель - убить всех и вс€			//
		ST_ATTACK_00_01( C_OBJ, msg, "l_x", "l_y", "l_R", "AREAID","ST_NEWSTEP" );
		END_OK;
	};
	
	END_OK;
};

EDITOR_FUNCTION	ACF_SABOTEUR(__PARAMS__){
	__GROUP(Object);
	
	if ( GI::CheckObjectFSM( Object.getName(), "FSM_SABOTEUR" ) ){
		pSMARTOBJECT _soObject = GI::FindSObyName(Object.getName());
		if ( _soObject.Valid()==false ) END_FAIL;
		pParamList soParam = _soObject.getPARAMS();
		if ( soParam.Valid()==false ) END_FAIL;
		int& areaID = soParam.getINT("AREAID");
		areaID = -1;
		if ( pPL->check("AREAID") ) {
			areaID = pPL->getINT("AREAID");
			_soObject.next_STATE("ST_NEWSTEP");
			_soObject.FSC(true);
		};
		END_OK;
	};
	// Free SO for this Group if need 
	GI::FreeSO_pVAR(Object);
	// Get free or create new SO
	pSMARTOBJECT _soObject = GI::GET_SO("FSM_SABOTEUR");
	if ( _soObject.Valid()==false ) END_FAIL;
	// Get SO params 
	pParamList soParam = _soObject.getPARAMS();
	if ( soParam.Valid()==false ) END_FAIL;
	soParam.CLEAR();
	// Set controled object for dialog 
	_soObject.setOBJ(Object.getName());
	// Fill params for SO 
	int& areaID = soParam.addINT("AREAID");
	areaID = -1;
	if ( pPL->check("AREAID") )		areaID = pPL->getINT("AREAID");
	bool& free_hunt = soParam.addBOOL("FREE_HUNT");
	free_hunt = false;
	if ( pPL->check("FREE_HUNT") ) 	free_hunt = pPL->getBOOL("FREE_HUNT");
	// Run new dialog 
	_soObject.setSTATE("ST_GLOBAL");
	GI::SendMSG( "MSG_ENTER",_soObject,_soObject);
	
	END_OK;
};

//////////////////////////////////////////////////////////////////////////
























