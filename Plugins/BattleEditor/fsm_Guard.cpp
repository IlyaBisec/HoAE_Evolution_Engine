#include "stdafx.h"

void	DeclareteDLL_fsm_Guard(){
	
	// Guard
	GI::AddFunctionToDeclarationList("ACF_GuardZone00_arr");
	GI::AddParamsToFunctionInList("ACF_GuardZone00_arr",	"ARRAY"	, "ObjectArr"	);
	GI::AddParamsToFunctionInList("ACF_GuardZone00_arr",	"BOOL"	, "bGuardZone"	);
	GI::AddParamsToFunctionInList("ACF_GuardZone00_arr",	"INT"	, "R"			);
	GI::AddParamsToFunctionInList("ACF_GuardZone00_arr",	"INT"	, "iAddR"		);
	GI::AddParamsToFunctionInList("ACF_GuardZone00_arr",	"INT"	, "RespawnDelay");	

	GI::AddFunctionToDeclarationList("ACF_GuardZone00");
	GI::AddParamsToFunctionInList("ACF_GuardZone00",	"GROUP"	, "Object"		);
	GI::AddParamsToFunctionInList("ACF_GuardZone00",	"BOOL"	, "bGuardZone"	);
	GI::AddParamsToFunctionInList("ACF_GuardZone00",	"INT"	, "R"			);
	GI::AddParamsToFunctionInList("ACF_GuardZone00",	"INT"	, "iAddR"		);
	GI::AddParamsToFunctionInList("ACF_GuardZone00",	"INT"	, "RespawnDelay");

	GI::AddFunctionToDeclarationList("ACF_GuardZone01");
	GI::AddParamsToFunctionInList("ACF_GuardZone01",	"GROUP"	, "Object"		);
	GI::AddParamsToFunctionInList("ACF_GuardZone01",	"BOOL"	, "bGuardZone"	);
	GI::AddParamsToFunctionInList("ACF_GuardZone01",	"INT"	, "R"			);
	GI::AddParamsToFunctionInList("ACF_GuardZone01",	"INT"	, "iAddR"		);
	GI::AddParamsToFunctionInList("ACF_GuardZone01",	"INT"	, "MarkDelay"	);

	GI::AddFunctionToDeclarationList("ACF_GuardZone02");
	GI::AddParamsToFunctionInList("ACF_GuardZone02",	"GROUP"	, "Object"		);
	GI::AddParamsToFunctionInList("ACF_GuardZone02",	"BOOL"	, "bGuardZone"	);
	GI::AddParamsToFunctionInList("ACF_GuardZone02",	"INT"	, "R"			);
	GI::AddParamsToFunctionInList("ACF_GuardZone02",	"INT"	, "iAddR"		);
	GI::AddParamsToFunctionInList("ACF_GuardZone02",	"INT"	, "MarkDelay"	);
	GI::AddParamsToFunctionInList("ACF_GuardZone02",	"GROUP"	, "SecuredGroup");

	// Ambush
	GI::AddFunctionToDeclarationList("ACF_AmbushZone00");
	GI::AddParamsToFunctionInList("ACF_AmbushZone00",	"GROUP"	, "Object"		);
	GI::AddParamsToFunctionInList("ACF_AmbushZone00",	"NODE"	, "GuardNode"	);
	GI::AddParamsToFunctionInList("ACF_AmbushZone00",	"BOOL"	, "bGuardZone"	);
	GI::AddParamsToFunctionInList("ACF_AmbushZone00",	"INT"	, "R"			);
	GI::AddParamsToFunctionInList("ACF_AmbushZone00",	"INT"	, "iAddR"		);

	GI::AddFunctionToDeclarationList("ACF_AmbushMultyZone");
	GI::AddParamsToFunctionInList("ACF_AmbushMultyZone",	"GROUP"	, "Object"	);
	GI::AddParamsToFunctionInList("ACF_AmbushMultyZone",	"INT"	, "R"		);
	GI::AddParamsToFunctionInList("ACF_AmbushMultyZone",	"INT"	, "iAddR"	);
	GI::AddParamsToFunctionInList("ACF_AmbushMultyZone",	"ARRAY"	, "arrNode"	);

	
	// Patrol
	GI::AddFunctionToDeclarationList("ACF_PatrolZone00");
	GI::AddParamsToFunctionInList("ACF_PatrolZone00",	"GROUP"	, "Object"	);
	GI::AddParamsToFunctionInList("ACF_PatrolZone00",	"ARRAY"	, "arrNode"	);
	GI::AddParamsToFunctionInList("ACF_PatrolZone00",	"INT"	, "iR"		);
	GI::AddParamsToFunctionInList("ACF_PatrolZone00",	"INT"	, "iAddR"	);	
	
};

//////////////////////////////////////////////////////////////////////////
// FSM_GuardZone		++
//////////////////////////////////////////////////////////////////////////
/*
	USE DEFF PARAMS:
		"bGuardZone"	- атаковать врагов в охраняемой зоне	( ==false 	)
		"R"				- радиус охраняемой зоны				( ==500 	)
		"iAddR"			- дополнительный радикс					( ==500 	)
*/
EDITOR_FUNCTION	FSM_GuardZone_00	( pSMARTOBJECT& C_OBJ, _str& state, pMSG& msg ){	// +++

	FSM_CHECKP();

	//if ( SO_GROUP_IS_DEAD(C_OBJ) ) END_FAIL;
	pGroup _group 	= C_OBJ.getOBJ_GROUP();
	if ( _group.Valid()==false || _group.getUN()==0 ){
		/*
		pParamList _par = C_OBJ.getPARAMS();
		if ( _par.check("RespawnDelay") ){
			int& _RespawnDelay = _par.getINT("RespawnDelay");
			if( _RespawnDelay>0 ){
				if( state != "ST_IDLE" ){
					//state="ST_IDLE";
					//msg="MSG_ENTER";
					__SetState(ST_IDLE);
				}
				return ST_IDLE_00	( C_OBJ, msg );
			}
		}
		SO_GROUP_IS_DEAD(C_OBJ);
		END_FAIL;
		*/
		if( state != "ST_IDLE" ){
			__SetState(ST_IDLE);
		}
	}
	
	if 			( state == "ST_GLOBAL" 	){
		return ST_GLOBAL_00	( C_OBJ, msg );
	}else if 	( state == "ST_IDLE" 	){
		return ST_IDLE_00	( C_OBJ, msg );
	}else if 	( state == "ST_BACK"	){
		return ST_BACK_00	( C_OBJ, msg );
	}else if 	( state	== "ST_ATTACK"	){
		return ST_ATTACK_00	( C_OBJ, msg );
	};

	END_FAIL;
};

EDITOR_FUNCTION	ACF_GuardZone00_local( pGroup& _gGroup, pParamList& paramData ){
	if ( paramData.Valid()==false || _gGroup.Valid()==false || _gGroup.getUN()<=0 )	return 0;
	_gGroup.setAIGuard(true);
	if ( GI::CheckObjectFSM( _gGroup.getName(), "FSM_GUARD" ) ) return 1;
	// Free SO for this Group if need
	GI::FreeSO_pVAR(_gGroup);
	// Get free or create new SO
	pSMARTOBJECT _soObject = GI::GET_SO("FSM_GUARD");
	if ( _soObject.Valid()==false ) return 0;
	// Get SO params
	pParamList soParam = _soObject.getPARAMS();
	if ( soParam.Valid()==false )	return 0;
	soParam.CLEAR();
	// Set controled object for dialog
//	_soObject.setOBJ(_gGroup.getName());
	_soObject.setOBJ(_gGroup);
	// Fill params for SO
	if ( paramData.check("bGuardZone") ){
		bool& bGZ = soParam.addBOOL("bGuardZone");
		bGZ = paramData.getBOOL("bGuardZone");
	};
	if ( paramData.check("R") ){
		int& R = soParam.addINT("R");
		R = paramData.getINT("R");
	};
	if ( paramData.check("iAddR") ){
		int& addR = soParam.addINT("iAddR");
		addR = paramData.getINT("iAddR");
	};
	if ( paramData.check("bNewPos") ){
		bool& NP = soParam.addBOOL("bNewPos");
		NP = paramData.getBOOL("bNewPos");
	};
	if ( paramData.check("soName") ){
		_str& Name = paramData.getSTR("soName");
		Name = _soObject.getName();
	};
	int& MD = soParam.addINT("MarkDelay");
	MD = 300;
	if ( paramData.check("MarkDelay") ){
		MD = paramData.getINT("MarkDelay");
	};
	if ( paramData.check("SecuredGroup") ){
		soParam.addGROUP("SecuredGroup");
		soParam.setREF("SecuredGroup",paramData.getGROUP("SecuredGroup"));
	};
	if ( paramData.check("RespawnDelay") ){
		int& addR = soParam.addINT("RespawnDelay");
		addR = paramData.getINT("RespawnDelay");
	};
	// Run new dialog 
	_soObject.setSTATE("ST_GLOBAL");
	GI::SendMSG( "MSG_ENTER", _soObject, _soObject );
	
	return 1;
};


EDITOR_FUNCTION	ACF_GuardZone00_arr			(__PARAMS__){
	__ARRAY(ObjectArr);
	if ( ObjectArr.Valid()==false ) return 0;
	int N = ObjectArr.getSIZE();
	while ( N-- ){
		ACF_GuardZone00_local( ObjectArr.getGROUP(N), pParamList(pPL) );
	};
	return 1;
};

EDITOR_FUNCTION	ACF_GuardZone00				(__PARAMS__){
	__GROUP(Object);
	return ACF_GuardZone00_local( Object, pParamList(pPL) );
};

EDITOR_FUNCTION	ACF_GuardZone01				(__PARAMS__){
	return ACF_GuardZone00(pPL);
};

EDITOR_FUNCTION	ACF_GuardZone02				(__PARAMS__){
	return ACF_GuardZone00(pPL);
};

/*
	Засада. SO запоминает позиции юнитов и после атаки
	возвращает их наместо. Атакуем юнитов в зоне используя 
	функции охраны.
	USE DEFF PARAMS:
		"GuardNode"		- зона засады
		"R"				- радиус охраняемой территории		 ( ==500 	)
		"iAddR"			- дополнительный радиус				 ( ==500 	)
*/
EDITOR_FUNCTION	FSM_AmbushZone_00	( pSMARTOBJECT& C_OBJ, _str& state, pMSG& msg ){
	
	FSM_CHECKP();

	if ( SO_GROUP_IS_DEAD(C_OBJ) ) END_FAIL;

	if 		( state == "ST_GLOBAL" 	){
		ST_GLOBAL_00		( C_OBJ, msg );
		(C_OBJ.getPARAMS()).getBOOL("bGuardZone") = true;
		END_OK;
	}else if 	( state == "ST_IDLE" 	){
		return ST_IDLE_00	( C_OBJ, msg );
	}else if 	( state == "ST_BACK"	){
		return ST_BACK_00	( C_OBJ, msg );
	}else if 	( state	== "ST_ATTACK"	){
		return ST_ATTACK_00	( C_OBJ, msg );
	};
	
	END_FAIL;
};
EDITOR_FUNCTION	ACF_AmbushZone00			(__PARAMS__){
	__GROUP(Object);
	if ( Object.Valid()==false || Object.getUN()<=0 )	return 0;

	Object.setAIGuard(true);
	if ( GI::CheckObjectFSM(Object.getName(),"FSM_Ambush") ) return 1;
	// Free SO for this Group if need 
	GI::FreeSO_pVAR(Object);
	// Get free or create new SO 
	pSMARTOBJECT _soObject = GI::GET_SO("FSM_Ambush");
	if ( _soObject.Valid()==false ) return 0;
	// Get SO params 
	pParamList soParam = _soObject.getPARAMS();
	if ( soParam.Valid()==false ) return 0;
	soParam.CLEAR();
	// Set controled object for dialog
//	_soObject.setOBJ(Object.getName());
	_soObject.setOBJ(Object);
	// Fill params for SO 
	if ( pPL->check("GuardNode") ){
		soParam.addNODE("GuardNode");
		soParam.setREF("GuardNode",pPL->getNODE("GuardNode"));
	};
	if ( pPL->check("bGuardZone") ){
		bool& GZ = soParam.addBOOL("bGuardZone");
		GZ = pPL->getBOOL("bGuardZone");
	};
	if ( pPL->check("R") ){
		int& R = soParam.addINT("R");
		R = pPL->getINT("R");
	};
	if ( pPL->check("iAddR") ){
		int& addR = soParam.addINT("iAddR");
		addR = pPL->getINT("iAddR");
	};
	if ( pPL->check("bNewPos") ){
		bool& NP = soParam.addBOOL("bNewPos");
		NP = pPL->getBOOL("bNewPos");
	};
	if ( pPL->check("soName") ){
		_str& SOName = pPL->getSTR("soName");
		SOName = _soObject.getName();
	};
	soParam.addINT("MarkDelay") = -1;
	// Run new dialog
	_soObject.setSTATE("ST_GLOBAL");
	GI::SendMSG("MSG_ENTER",_soObject,_soObject);
	
	END_OK;
};

/*
	Multy Ambush zone + Guard
	Используется для контроля сразу нескольких зон,
	а также юниты охраняют свои позиции.
	PARMS:
		"R"				- радиус охраняемой территории	 ( ==500 	)
		"iAddR"			- дополнительный радиус			 ( ==500 	)
		"arrNode"		- зоны засады
*/
EDITOR_FUNCTION	FSM_AmbushZone_01	( pSMARTOBJECT& C_OBJ, _str& state, pMSG& msg ){
	FSM_CHECKP();
	if ( SO_GROUP_IS_DEAD(C_OBJ) ) END_FAIL;

	pGroup	_group 	= C_OBJ.getOBJ_GROUP();
	if ( _group.Valid()==false || _group.getUN()==0 )	END_FAIL;
	pParamList _par	= C_OBJ.getPARAMS();
	if ( _par.Valid()==false ) END_FAIL;
	
	if 		( state == "ST_GLOBAL" 	){
		if ( msg.str() == "MSG_ENTER" ){
			// Group guard own zone
			    // Fill param list 
				// SET "Object" 
				_par.addGROUP("Object");
				_par.setREF("Object",_group);
				// SET "bGuardZone"
				_par.addBOOL("bGuardZone") = true;
				// SET "soName"
				_par.addSTR("soName");
				// SET "GuardNode" 
				_par.addNODE("GuardNode");
				// SET "bNewPos" 
				_par.addBOOL("bNewPos") = true;
			// ------------------------------
			ACF_GuardZone00( &_par );
//			C_OBJ.setOBJ(_group.getName());
			C_OBJ.setOBJ(_group);
			// --------------------------
			// GO in next state 
			__SetState(ST_IDLE);
		};
		END_OK;
	}else if 	( state == "ST_IDLE" 	){
		// Only in this state group can change it FSM
		if 		( msg.str() == "MSG_UPDATE" ){
			_str&			soName   = _par.getSTR("soName");
			pSMARTOBJECT	soObject = GI::FindSObyName( soName.pchar() );
			if ( soObject.Valid() && soObject.getSTATE() == "ST_IDLE" ){
				pARRAY	arrNode 	= _par.getARRAY("arrNode");
				int		arrN		= arrNode.getSIZE();
				iNode	_Node;
				int		_NMask		= _group.getNMASK();
				bool	attack		= false;
				while ( arrN-- && attack==false ){
					_Node = arrNode.getNODE( arrN );
					attack = (getEnemyAmount(_NMask,_Node)>0)&&_group.getAttackPossibility(_Node);
				};
				if ( attack && _Node.Valid() ){
					_par.setREF("GuardNode",_Node);
					pParamList oldSO_PAR = soObject.getPARAMS();
					oldSO_PAR.CLEAR();
					ACF_AmbushZone00( &_par );
//					C_OBJ.setOBJ(_group.getName());
					C_OBJ.setOBJ(_group);
					__SetState(ST_ATTACK);
				};
			};
		};
		END_OK;
	}else if  ( state == "ST_ATTACK" 	){
		_str&		soName		= _par.getSTR("soName");
		pSMARTOBJECT soObject	= GI::FindSObyName( soName.pchar() );
		if 		( msg.str() == "MSG_UPDATE" ){
			if ( soObject.getSTATE() == "ST_IDLE" ){
				// GO in next state 
				__SetState(ST_IDLE);
			};
		}else if 	( msg.str() == "MSG_EXIT" ){
			pParamList oldSO_PAR = soObject.getPARAMS();
			oldSO_PAR.CLEAR();
			ACF_GuardZone00( &_par );
//			C_OBJ.setOBJ(_group.getName());
			C_OBJ.setOBJ(_group);
		};
		END_OK;
	};
	END_FAIL;
};

EDITOR_FUNCTION	ACF_AmbushMultyZone			(__PARAMS__){
	__GROUP(Object);
	if ( Object.Valid()==false || Object.getUN()<=0 )	END_FAIL;
	Object.setAIGuard(true);
	if ( GI::CheckObjectFSM( Object.getName(), "FSM_AmbushMultyZone" ) ) END_OK;
	// Free SO for this Group if need
	GI::FreeSO_pVAR(Object);
	// Get free or create new SO
	pSMARTOBJECT _soObject = GI::GET_SO("FSM_AmbushMultyZone");
	if ( _soObject.Valid()==false ) END_FAIL;
	// Get SO params 
	pParamList soParam = _soObject.getPARAMS();
	if ( soParam.Valid()==false ) END_FAIL;
	soParam.CLEAR();
	// Set controled object for dialog 
//	_soObject.setOBJ(Object.getName());
	_soObject.setOBJ(Object);
	// Fill params for SO 
	if ( pPL->check("R") )		soParam.addINT("R")		= pPL->getINT("R");
	if ( pPL->check("iAddR") )	soParam.addINT("iAddR")	= pPL->getINT("iAddR");
	soParam.addARRAY("arrNode");
	if ( pPL->check("arrNode") )soParam.setREF("arrNode",pPL->getARRAY("arrNode"));
	else	END_FAIL;
	// Run new dialog 
	_soObject.setSTATE("ST_GLOBAL");
	GI::SendMSG( "MSG_ENTER", _soObject, _soObject );
	
	END_OK;
};

//////////////////////////////////////////////////////////////////////////
// FSM_Patrol			++
//////////////////////////////////////////////////////////////////////////
/*
	USE DEFF PARAMS:
		"iR"			- радиус вокруг юнитов для охраны ( == 500 )
		"iAddR"			- дополнительный радиус ( == 500 )
		"arrNode"		- список патрулируемых зон 
*/
EDITOR_FUNCTION	FSM_PatrolZone_00	( pSMARTOBJECT& C_OBJ, _str& state, pMSG& msg ){
	FSM_CHECKP();
	if ( SO_GROUP_IS_DEAD(C_OBJ) ) END_FAIL;
	
	pGroup		_group 	= C_OBJ.getOBJ_GROUP();
	pParamList	_par	= C_OBJ.getPARAMS();
		
	if 		( state == "ST_GLOBAL" 	){
		if 	( msg.str() == "MSG_ENTER" 	){
			// Parameter ENEMY GROUP
			if ( _par.check("EnemyGroup")==false )	_par.addGROUP("EnemyGroup");
			// Охраняемая зона. Движиться за группой.
			iNode _nNode;
			if ( _par.check("nNode")==false )		_nNode = _par.addNODE("nNode");
			else									_nNode = _par.getNODE("nNode");
			_nNode.setX( _group.getXc() );
			_nNode.setY( _group.getYc() );
			if ( _par.check("iR") ) _nNode.setR( _par.getINT("iR") );
			else					_nNode.setR( 500 );
			// Задаем текущий индекс зоны для следования патруля
			if ( _par.check("iNID")==false ){
				int&	_iNID	= _par.addINT("iNID");
				if ( _par.check("arrNode")==false ) _iNID = -1;
				else	_iNID = GetNearestNodeID( _par.getARRAY("arrNode"), _nNode.getX(), _nNode.getY(), true );
			};
			// Составляем относительный массив позиций
			/*
			pARRAY arrGrpPos;
			if ( _par.check("arrGrpPos")==false ){
				arrGrpPos = _par.addARRAY("arrGrpPos");
			}else{
				arrGrpPos = _par.getARRAY("arrGrpPos");
				arrGrpPos.CLEAR();
			};
			pParamList arrParams = arrGrpPos.getParamList();
			iNode node;
			int UN = _group.getUN();
			for ( int i=0; i<UN; i++ ){
				node = arrParams.addNODE( _str(i).pchar() );
				node.setX( _group.getXi(i) - _nNode.getX() );
				node.setY( _group.getYi(i) - _nNode.getY() );
				node.setR( 0 );
				node.setDIR( _group.getDIRi(i) );
			};
			*/
			// GO in next state 
			__SetState(ST_IDLE);
		};
		END_OK;
	}else if( state == "ST_IDLE" 	){
		iNode	_nNode		= _par.getNODE("nNode");
		// Link set node position
		pARRAY	_arrNode	= _par.getARRAY("arrNode");
		int		centerID	= GetNearestNodeID( _arrNode, _group.getXc(), _group.getYc(), true );
		iNode	_lNode10	= GetSafeNODE( _arrNode, centerID-1 );
		iNode	_lNode00	= GetSafeNODE( _arrNode, centerID+0 );
		iNode	_lNode01	= GetSafeNODE( _arrNode, centerID+1 );
		LinkNodeToGroup00( _nNode, _group, _lNode10, _lNode00, _lNode01 );
		// ---------------------- //
		int		_addR		= _par.getINT("iAddR");
		int		_GUN	 	= _group.getUN();
		int		_NMask		= _group.getNMASK();
		int&	_iNID		= _par.getINT("iNID");
		if 		( msg.str() == "MSG_ENTER" ){
			_group.setSearchEnemy(false);
			_group.ClearOrders();
			_iNID = GetNearestNodeID( _arrNode, _group.getXc(), _group.getYc(), true );
			END_OK;
		}else if( msg.str() == "MSG_UPDATE" 	){
			if ( getEnemyAmount(_NMask,_nNode)>0 || _group.getUnderAttack()  ){
				__SetState(ST_ATTACK);
			}else if( _group.getNO_ORDERS_UN()>=(int)((float)_GUN*0.8f+0.5f) ){
				iNode _lNode = _arrNode.getNODE( _iNID );
				if ( _group.getUN(_lNode)==_GUN || 
					 GI::NORMA(_lNode.getX(),_group.getXc(),_lNode.getY(),_group.getYc())<=_lNode.getR() 
				   )
				{
					_iNID++;
					if ( _iNID>=_arrNode.getSIZE() ) _iNID = 0;
				};
				// Send to pos Array
				int UN = _group.getUN();
				_group.setDestDelta( _lNode.getR() );
				for ( int uid=0; uid<UN; uid++ ){
					_group.SendID(_lNode,vgMOVE|vgSOME_ORDER|vgBRIG_ORDER,uid);
				};
				/*
				_lNode = _arrNode.getNODE(_iNID);
				_group.Send(_lNode);	
				pARRAY	arrGrpPos	= _par.getARRAY("arrGrpPos");
				int		arrN		= arrGrpPos.getSIZE();
				iNode	node;
				int		NNN = _GUN;
				if ( NNN>arrN ) NNN=arrN;
				for ( int i=0; i<NNN; i++ ){
					node = arrGrpPos.getNODE(i);
					_group.SendID(i,node.getX()+_lNode.getX(),node.getY()+_lNode.getY(),node.getDIR());
				};
				*/
			};
		}else if( msg.str() == "MSG_EXIT" ){
			_group.ClearOrders();
		};
		END_OK;
	}else if( state	== "ST_ATTACK"	){
		// Attack script 
		iNode	_nNode	= _par.getNODE("nNode");
		int		_addR	= _par.getINT("iAddR");
		pGroup	_enemy	= _par.getGROUP("EnemyGroup");
		int		_Nat 	= _group.getNation();
		int		_NMask	= _group.getNMASK();
		// Link set node position
		pARRAY	_arrNode	= _par.getARRAY("arrNode");
		int		centerID	= GetNearestNodeID( _arrNode, _group.getXc(), _group.getYc(), true );
		iNode	_lNode10 = GetSafeNODE( _arrNode, centerID-1 );
		iNode	_lNode00 = GetSafeNODE( _arrNode, centerID+0 );
		iNode	_lNode01 = GetSafeNODE( _arrNode, centerID+1 );
		LinkNodeToGroup00( _nNode, _group, _lNode10, _lNode00, _lNode01 );
		// ---------------------- //
		if 		( msg.str() == "MSG_ENTER" ){
			_group.setSearchEnemy(true);
			_enemy.RemAllUnits();
			GI::SendMSG("MSG_CREATE_EN",C_OBJ,C_OBJ);
			END_OK;
		}else if( msg.str() == "MSG_CREATE_EN" ){
			_enemy.AddEnemyUnitsInZone(_Nat,_nNode.getX(),_nNode.getY(),_nNode.getR()+_addR*2,false);
			if ( _enemy.getUN()>0 ){
				if (_enemy.getNew_UN(true)||_enemy.getRem_UN(true)||_group.getNO_ORDERS_UN()){
					GI::SendMSG("MSG_ATTACK_EN",C_OBJ,C_OBJ);
				};
			};
			END_OK;
		}else if( msg.str() == "MSG_ATTACK_EN" ){
			// Перераспредилить цели для атаки
			_group.AttackUseNearest(_enemy);
			END_OK;
		}else if( msg.str() == "MSG_UPDATE" ){
			int  EnN = getEnemyAmount(_NMask,_nNode,_addR);
			bool EnY = _group.getAttackPossibility(_nNode,_addR);
			if ( EnN==0 || EnY==false ){
				__SetState(ST_IDLE);
			}else if ( EnN!=_enemy.getUN() || _group.getNO_ORDERS_UN() ){
				GI::SendMSG("MSG_CREATE_EN",C_OBJ,C_OBJ);
			};
			END_OK;
		}else if( msg.str() == "MSG_EXIT" ){
			_group.setUnderAttack(false);
			_enemy.RemAllUnits();
			END_OK;
		};
		// ------------- //
	};
	END_FAIL;
};

EDITOR_FUNCTION	ACF_PatrolZone00			(__PARAMS__){
	__GROUP( Object );
	if ( Object.getUN()<=0 ) END_FAIL;
	Object.setAIGuard(true);
	if ( GI::CheckObjectFSM(Object.getName(),"FSM_PATROL") ) END_OK;
	// Free SO for this Group if need
	GI::FreeSO_pVAR(Object);
	// Get free or create new SO
	pSMARTOBJECT _soObject = GI::GET_SO("FSM_PATROL");
	if ( _soObject.Valid()==false ) END_FAIL;
	// Get SO params 
	pParamList soParam = _soObject.getPARAMS();
	if ( soParam.Valid()==false ) END_FAIL;
	soParam.CLEAR();
	// Set controled object for dialog 
//	_soObject.setOBJ(Object.getName());
	_soObject.setOBJ(Object);
	// Fill params for SO 
	if ( pPL->check("iR") )			soParam.addINT("iR")	= pPL->getINT("iR");
	if ( pPL->check("iAddR") )		soParam.addINT("iAddR")	= pPL->getINT("iAddR");
	soParam.addARRAY("arrNode");
	if ( pPL->check("arrNode") ) soParam.setREF("arrNode",pPL->getARRAY("arrNode"));
	else						 soParam.setREF("arrNode",NULL);
	// Run new dialog
	_soObject.setSTATE("ST_GLOBAL");
	GI::SendMSG("MSG_ENTER",_soObject,_soObject);
	
	END_OK;
};

//////////////////////////////////////////////////////////////////////////


































