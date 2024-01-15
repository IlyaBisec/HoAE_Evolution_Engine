#include "stdafx.h"

void	DeclareteDLL_fsm_Produce(){
	
	GI::AddFunctionToDeclarationList("ACF_ProduseUnits_02");
	GI::AddParamsToFunctionInList("ACF_ProduseUnits_02",	"GROUP"	, "Object"		);
	GI::AddParamsToFunctionInList("ACF_ProduseUnits_02",	"GROUP"	, "g_gGuard"	);
	GI::AddParamsToFunctionInList("ACF_ProduseUnits_02",	"INT"	, "iOwnerNat"	);
	GI::AddParamsToFunctionInList("ACF_ProduseUnits_02",	"NODE"	, "nRallyPoint"	);
	GI::AddParamsToFunctionInList("ACF_ProduseUnits_02",	"ARRAY"	, "arrTN"		);
	GI::AddParamsToFunctionInList("ACF_ProduseUnits_02",	"INT"	, "iWaitTime"	);
	GI::AddParamsToFunctionInList("ACF_ProduseUnits_02",	"BOOL"	, "g_bActive"	);
	GI::AddParamsToFunctionInList("ACF_ProduseUnits_02",	"BOOL"	, "g_bGuardZone");

	GI::AddFunctionToDeclarationList("ACF_ProduseUnits_03");
	GI::AddParamsToFunctionInList("ACF_ProduseUnits_03",	"GROUP"	, "Object"		);
	GI::AddParamsToFunctionInList("ACF_ProduseUnits_03",	"GROUP"	, "g_gGuard"	);
	GI::AddParamsToFunctionInList("ACF_ProduseUnits_03",	"INT"	, "iOwnerNat"	);
	GI::AddParamsToFunctionInList("ACF_ProduseUnits_03",	"NODE"	, "nRallyPoint"	);
	GI::AddParamsToFunctionInList("ACF_ProduseUnits_03",	"ARRAY"	, "arrTN"		);
	GI::AddParamsToFunctionInList("ACF_ProduseUnits_03",	"INT"	, "iWaitTime"	);
	GI::AddParamsToFunctionInList("ACF_ProduseUnits_03",	"BOOL"	, "g_bActive"	);
	GI::AddParamsToFunctionInList("ACF_ProduseUnits_03",	"GROUP"	, "g_gDestGroup");
	GI::AddParamsToFunctionInList("ACF_ProduseUnits_03",	"BOOL"	, "g_bGuardZone");

	GI::AddFunctionToDeclarationList("ACF_ProduseUnits_04");
	GI::AddParamsToFunctionInList("ACF_ProduseUnits_04",	"GROUP"	, "Object"		);
	GI::AddParamsToFunctionInList("ACF_ProduseUnits_04",	"GROUP"	, "g_gGuard"	);
	GI::AddParamsToFunctionInList("ACF_ProduseUnits_04",	"INT"	, "iOwnerNat"	);
	GI::AddParamsToFunctionInList("ACF_ProduseUnits_04",	"NODE"	, "nRallyPoint"	);
	GI::AddParamsToFunctionInList("ACF_ProduseUnits_04",	"ARRAY"	, "arrTN"		);
	GI::AddParamsToFunctionInList("ACF_ProduseUnits_04",	"INT"	, "iWaitTime"	);
	GI::AddParamsToFunctionInList("ACF_ProduseUnits_04",	"BOOL"	, "g_bActive"	);
	GI::AddParamsToFunctionInList("ACF_ProduseUnits_04",	"GROUP"	, "g_gDestGroup");
	GI::AddParamsToFunctionInList("ACF_ProduseUnits_04",	"BOOL"	, "g_bGuardZone");
	
};

//////////////////////////////////////////////////////////////////////////
// FSM_ProduseUnits		++
//////////////////////////////////////////////////////////////////////////
/*
Строит отряд размером "iUnitNUM", юнитов типа "iUnitType" 
в группу <<"gGroup">> и перекрашивает их в цвет "iNewNation"
*/
EDITOR_FUNCTION	FSM_ProduseUnits_00	( pSMARTOBJECT& C_OBJ, _str& state, pMSG& msg ){
	FSM_CHECKP();
	if ( SO_GROUP_IS_DEAD(C_OBJ) ) END_FAIL;
	
	if 		( state == "ST_GLOBAL" 	){
		if ( ProcMsgFree( msg, false ) ) END_OK;
		ST_GLOBAL_B_00( C_OBJ, msg );
		END_OK;
	}else if( state == "ST_IDLE" 	){		// Wait any command 			
		if ( ProcMsgFree( msg, true  ) ) END_OK;
		ST_IDLE_B_00( C_OBJ, msg );
		END_OK;
	}else if( state == "ST_BEGIN_PR" ){		// Prepare params for produse 	
		if ( ProcMsgFree( msg, false ) ) END_OK;
		ST_BEGIN_PR_00( C_OBJ, msg );
		END_OK;
	}else if( state == "ST_PRODUCE" ){		// Produse units
		if ( ProcMsgFree( msg, false ) ) END_OK;
		ST_PRODUCE_00( C_OBJ, msg );
		END_OK;
	}else if( state == "ST_END_PR" ){		// Prosess use prepare command
		if ( ProcMsgFree( msg, false ) ) END_OK;
		ST_END_PR_00( C_OBJ, msg );
		END_OK;
	};
	END_FAIL;
};

/*
	USE DEFF PARAMS:
		"gTrader"		- продавец
		"nRallyPoint"	- точка сбора для бараков
		"iResType"		- тип ресурса
		"iPrice"		- цена покупки
		"iUnitType"		- тип производимых юнитов
		"iUnitNUM"		- кол-во производимых юнитов
		"iWaitTime"		- задержка между производством
*/
EDITOR_FUNCTION	FSM_ProduseUnits_01	( pSMARTOBJECT& C_OBJ, _str& state, pMSG& msg ){
	FSM_CHECKP();
	if ( SO_GROUP_IS_DEAD(C_OBJ) ) END_FAIL;
	
	if 		( state == "ST_GLOBAL" 	){
		if ( ProcMsgFree( msg, false ) ) END_OK;
		ST_GLOBAL_B_00	( C_OBJ, msg );
		END_OK;
	}else if	( state == "ST_IDLE" 	){
		if ( ProcMsgFree( msg, true  ) ) END_OK;
		ST_ORDER_00		( C_OBJ, msg );
		ST_IDLE_B_01	( C_OBJ, msg );
		END_OK;
	}else if ( state == "ST_BEGIN_PR" ){
		if ( ProcMsgFree( msg, false ) ) END_OK;
		ST_BEGIN_PR_00	( C_OBJ, msg );
		END_OK;
	}else if ( state == "ST_PRODUCE" ){
		if ( ProcMsgFree( msg, false ) ) END_OK;
		ST_PRODUCE_00	( C_OBJ, msg );
		END_OK;
	}else if ( state == "ST_END_PR" ){
		if ( ProcMsgFree( msg, false ) ) END_OK;
		ST_END_PR_01	( C_OBJ, msg );
		END_OK;
	};
	END_FAIL;
};

/*
	DESCR:
		Как только активируется то начинает производить юнитов с
		задержкой во времени "iWaitTime" в соответствии с масивом
		типов и кол-вом в "arrTN" выставляя им маску соответствующую
		нации "iOwnerNat", юниты собираются в точке "nRallyPoint"
	CONTROLED OBJECT:
		Baraks			- группа зданий для производства	!!!	-GLOBAL
	USE PARAMS:
		"iOwnerNat"		- нация купившего					!!!
		"nRallyPoint"	- точка сбора для бараков			!!!
		"arrTN"			- массив: (тип юнита, кол-во)		!!!	-GLOBAL
		"iWaitTime"		- задержка между производством		!!!
	ITERNAL PARM:
		"gGroup"		- группа в которую производим юнитов
*/
EDITOR_FUNCTION	FSM_ProduseUnits_02	( pSMARTOBJECT& C_OBJ, _str& state, pMSG& msg ){
	FSM_CHECKP();
	
	pGroup		_gBarak = C_OBJ.getOBJ_GROUP();
	pParamList	_par	= C_OBJ.getPARAMS();

	if ( _gBarak.Valid()==false ) END_FAIL;
	// if barak destroy why building -> complete operation
	if ( state!="ST_PRODUCE" && state!="ST_END_PR" ){
		if ( _gBarak.getUN()==0 ){
			GI::FreeSO_pVAR(_gBarak);
			(C_OBJ.getPARAMS()).CLEAR();
			END_FAIL;
		};
	};

	// Каждый раз проверяем маску и ставим правильную
	if ( _par.check("iMyStartNat") && _par.check("iOwnerNat") ){
		int& _iMyStartNat	= _par.getINT("iMyStartNat");
		int  _iOwner		= _par.getINT("iOwnerNat");
		bool _bAct			= _par.getBOOL("g_bActive");
		if ( _bAct && _iMyStartNat != _iOwner ){
			int _iOwnerNMASK = GetNationNMASK( _iOwner );
			if ( _iOwner!=128 )		_iOwnerNMASK += 128;
			if ( _gBarak.Valid() ){
				//_gBarak.setNation( _iOwner );
				_gBarak.setNMASK( _iOwnerNMASK );
				_gBarak.setCapturability(false);
			};
			if ( _par.check("gGuardGroup") ) (_par.getGROUP("gGuardGroup")).setNMASK(_iOwnerNMASK);
			if ( _par.check("gGroup") )		 (_par.getGROUP("gGroup")).setNMASK(_iOwnerNMASK);
			_iMyStartNat = _iOwner;
		};
	};
	
	// Only for TEST - save current STATE 
	if ( _par.check("strSTATE") ) _par.getSTR("strSTATE") = state;
	
	if ( _par.check("GG_Tick") ){
		int& _ggTick = _par.getINT("GG_Tick");
		if ( _ggTick<=0 ){
			// Send message for try support gurd group 
			if ( state=="ST_IDLE" && _par.getSTR("strPRODUCE_TYPE")!="ORDER" ){
				_par.getSTR("strPRODUCE_TYPE") = "SUPPORT_GG";
			};
			_ggTick = 10;
		}else{
			_ggTick--;
		};
	};
	
	// Повторить приказ по строительству юнитов
	if	( msg.str()=="MSG_ORDER" && state!="ST_IDLE" ){
		GI::SendMSG( "MSG_ORDER", 1000, C_OBJ, C_OBJ );
		END_OK;
	};
		
//-------------------------------------------------------//
	if 		( state == "ST_GLOBAL" 	){
		if ( msg.str() == "MSG_ENTER" 	){
			// Group for store prodused units 
			_par.addGROUP("gGroup");
			// Set Rally Point 
			if ( _par.check("nRallyPoint") ) _gBarak.SetRallyPoint( _par.getNODE("nRallyPoint") );
			// Set Barak NMASK ot iOwnerNat 
			int _iOwner = _par.getINT("iOwnerNat");
			int _iOwnerNMASK = GetNationNMASK( _iOwner );
			int _iMyStartNat = _par.getINT("iMyStartNat");
			if ( _iOwner!=_iMyStartNat ) _iOwnerNMASK += GetNationNMASK(_iMyStartNat);
			_gBarak.setNMASK(_iOwnerNMASK);
			// Set value for store produce type - for PLAYER or fro GUARD
			_par.addSTR("strPRODUCE_TYPE") = "UNKNOWN";
			// Create ticker for corect delta time for support guard group 
			_par.addINT("GG_Tick") = 150;
			
			// Only for TEST - save current STATE 
			_par.addSTR("strSTATE") = "ST_GLOBAL";
			
			// Не теряем статус активности
			_par.addBOOL("bStartBStatus") = _par.getBOOL("g_bActive");
			
			// GO in next state
			C_OBJ.next_STATE("ST_IDLE");
			C_OBJ.FSC(true);
			GI::SendMSG( "MSG_ORDER", 1000, C_OBJ, C_OBJ );
		};
		END_OK;
//-------------------------------------------------------//
	}else if( state == "ST_IDLE" 	){
		_str& sProdType = _par.getSTR("strPRODUCE_TYPE");
		if ( sProdType=="SUPPORT_GG" ){
			if ( _par.check("arrTN_GG") && _par.check("gGuardGroup") ){
				pARRAY _arrTN	= _par.getARRAY("arrTN_GG");
				pGroup _gGuard	= _par.getGROUP("gGuardGroup");

				bool DestNotGuard = true;
				if ( _par.check("g_gDestGroup") ){
					pGroup _gDestGroup = _par.getGROUP("g_gDestGroup");
					DestNotGuard = (strcmp(_gGuard.getName(),_gDestGroup.getName())!=0);
				};
				
				if ( DestNotGuard ){
					if ( Get_arrTN_UnitNumberSum(_arrTN)!=_gGuard.getUN() ){
						_par.getINT("GG_Tick") = 1000;
						// Создаем новую схему обработки - пополняем группу охраны
						pARRAY	_arrTN_SGG;
						if ( _par.check("arrTN_SGG")==false )	_arrTN_SGG = _par.addARRAY("arrTN_SGG");
						else									_arrTN_SGG = _par.getARRAY("arrTN_SGG");
						_gGuard.FillDIFF_TN_array( _arrTN, _arrTN_SGG, _gBarak.getUN() );
						// Переходим в состояние производства
						C_OBJ.next_STATE("ST_BEGIN_PR");
						C_OBJ.FSC(true);
					};
				};
			};
		};
			
		bool& _bAct = _par.getBOOL("g_bActive");
		if	( msg.str() == "MSG_ORDER" 	){
			if ( !_bAct ){
				GI::SendMSG( "MSG_ORDER", 1000, C_OBJ, C_OBJ );
				END_OK;
			};
			sProdType="ORDER";
			C_OBJ.next_STATE("ST_BEGIN_PR");
			C_OBJ.FSC(true);
		};
		
		// Прверяем статус активности, возможно стоит начать строительство
		bool& _oldStatus = _par.getBOOL("bStartBStatus");
		if ( !_oldStatus && _bAct )	GI::SendMSG( "MSG_ORDER", 1000, C_OBJ, C_OBJ );
		_oldStatus = _bAct;
		// **************************************************************** //
		END_OK;
//-------------------------------------------------------//
	}else if( state == "ST_BEGIN_PR" ){
		if ( msg.str() == "MSG_ENTER" ){
			int _iUT = -1;
			int _iUN = -1;
			pARRAY _arrTN = _par.getARRAY("arrTN");
			// use suport 
			if ( _par.getSTR("strPRODUCE_TYPE")=="SUPPORT_GG" ) _arrTN = _par.getARRAY("arrTN_SGG");
			// ********** //
			// use map params for build not all units
			int		_iCom		 = _par.getINT("iCommandID");
            // **************************************
			int _arrN  = _arrTN.getSIZE();
			for ( int i=0; i<(_arrN/2); i++ ){
				_iUT = _arrTN.getINT(2*i);
				_iUN = _arrTN.getINT(2*i+1);
				if ( _iCom==0 ){
					pGroup	_gStoreGroup = _par.getGROUP("l_gStoreGroup");
					_gBarak.Produce(_iUT,_iUN,_gStoreGroup);
				}else{
					_gBarak.Produce(_iUT,_iUN);
				};
			};
			__SetState(ST_PRODUCE);
		};
		END_OK;
//-------------------------------------------------------//
	}else if( state == "ST_PRODUCE" ){
		pGroup	_gUnits = _par.getGROUP("gGroup");
		int		_iCom   = _par.getINT("iCommandID");
		if 		( msg.str() == "MSG_NEW_UNIT" ){
			_gUnits.AddUnitGID( (msg.getPARAMS()).getINT("inewGID") );
			_gUnits.setNMASK(_gBarak.getNMASK());
			_gUnits.DeleteItemDrops();
			if ( _iCom==2 ){
				// remove all ready units in dest group imediatly
				_gUnits.setAIGuard(false);
			}else{
				_gUnits.setAIGuard(true);
			};
		}else if( msg.str() == "MSG_UPDATE" ){
			_str& sProdType = _par.getSTR("strPRODUCE_TYPE");
			if ( sProdType!="SUPPORT_GG" && _iCom==2 && _par.check("g_gDestGroup") ){
				_gUnits.RemNReadyUnitsToGroup( _par.getGROUP("g_gDestGroup") );
			};
            if (
				_gBarak.getUN()==0 
				|| 
				( 
				_gBarak.getNO_ORDERS_UN()==_gBarak.getUN()  && 
				_gUnits.getReady()							&& 
				(_iCom==0||_iCom==1||(_iCom==2&&sProdType=="SUPPORT_GG")) 
				) 
				||
				( 
				sProdType!="SUPPORT_GG"						&&
				_gBarak.getNO_ORDERS_UN()==_gBarak.getUN()	&& 
				_gUnits.getUN()==0							&&
				_par.check("g_gDestGroup")					&&
				_par.getGROUP("g_gDestGroup").getReady()	&& 
				_iCom==2									
				)
			   )
			{
				__SetState(ST_END_PR);
			};
		};
		END_OK;
//-------------------------------------------------------//
	}else if( state == "ST_END_PR" ){
		pGroup	_gUnits = _par.getGROUP("gGroup");
		if 		( msg.str() == "MSG_UPDATE" ){
			if ( _gUnits.getReady() ) __SetState(ST_IDLE);
		}else if( msg.str() == "MSG_EXIT" 	){
			_gUnits.DeleteItemDrops();
			int _iOwner		 = _par.getINT("iOwnerNat");
			int _iMyStartNat = _par.getINT("iMyStartNat");
			if ( _iOwner!=_iMyStartNat ) _iOwner = _iMyStartNat;
			// Если мы строили юнитов для guard group
			_str& sProdType = _par.getSTR("strPRODUCE_TYPE");
			if ( sProdType=="SUPPORT_GG" ){
				sProdType = "UNKNOWN";
				_par.getINT("GG_Tick") = 150;
				pGroup _gGuard		= _par.getGROUP("gGuardGroup");
				bool  _reStartGuard = false;
				if ( _gGuard.getUN()<=0 ) _reStartGuard=true;
				_gUnits.RemNUnitsToGroup(_gGuard,_gUnits.getUN());
				int _iMyStartNat = _par.getINT("iMyStartNat");
				int _iOwnerNMASK = GetNationNMASK( _iMyStartNat );
				if ( _iOwner!=_iMyStartNat ) _iOwnerNMASK += GetNationNMASK(_iOwner);
				_gGuard.setNMASK(_iOwnerNMASK);
				if ( _reStartGuard ){
					// Set new Guard MASK 
					_gGuard.ClearOrders();
				 	// Set for Guard -> guard zone
					pSMARTOBJECT _soGuard = GI::FindSObyName( _gGuard.getName() );
					if ( _soGuard.Valid()==false ){
						pParamList paramData;
						paramData.CreateOBJ_PL();
						paramData.addBOOL("bGuardZone") = _par.getBOOL("g_bGuardZone");
						paramData.addINT("R")			= 500;
						paramData.addINT("iAddR")		= 350;
						paramData.addGROUP("SecuredGroup");	
						paramData.setREF("SecuredGroup",C_OBJ.getOBJ_GROUP());
						iNode ND = paramData.addNODE("GuardNode");
						ND.setX( _gBarak.getXc() ); 
						ND.setY( _gBarak.getYc() ); 
						ND.setR( 500 ); 
						ACF_GuardZone00_local( _gGuard, paramData );
						paramData.DeleteOBJ_PL();
					};
				};
				END_OK;
			};
			// На основании флага - решаем что делать с группой
			if ( _par.check("iCommandID") ){
				int _iCom = _par.getINT("iCommandID");
				if 		( _iCom==0 ){
					// Add new units in store group
					pGroup	_gStoreGroup = _par.getGROUP("l_gStoreGroup");
					_gStoreGroup.AddUnitsFromGroup(_gUnits);
					GI::AddTomahawksAttackers(_gUnits,_iOwner,true);
				}else if( _iCom==1 ){
					if ( _par.check("g_gDestGroup") ) _gUnits.RemNUnitsToGroup( _par.getGROUP("g_gDestGroup") );
				}else if( _iCom==2 ){
					// ...
				};
			};
			// ------------------------------------------------ //
			sProdType = "UNKNOWN";
			GI::SendMSG( "MSG_ORDER", _par.getINT("iWaitTime"), C_OBJ, C_OBJ );
		};
		END_OK;
//-------------------------------------------------------//
	};
	END_FAIL;
};

/*
ADDITION:
		UpdateGuard = true
		Во время ничего не делания - обновляем охрану.
		Активна поумолчанию, наличие группы охраны обязательно!!!
*/
EDITOR_FUNCTION	AFC_ProduseUnits_02_local	(__PARAMS__){
	__GROUP	( Object	 );
	__INT	( iOwnerNat	 );
	__ARRAY	( arrTN		 );
	__INT	( iWaitTime	 );
	__INT	( iCommandID );

	if ( Object.getUN()==0 ) END_FAIL;
	Object.setAIGuard(true);
	//-------------------------------------------//
	if ( GI::CheckObjectFSM( Object.getName(), "FSM_PRODUCE_02" ) ) END_OK;
	// Get free or create new SO 
	pSMARTOBJECT _soObject = GI::GET_SO("FSM_PRODUCE_02");
	if ( _soObject.Valid()==false ) END_FAIL;
	// Get SO params 
	pParamList soParam = _soObject.getPARAMS();
	if ( soParam.Valid()==false )	END_FAIL;
	soParam.CLEAR();
	// Set controled object for dialog 
//	_soObject.setOBJ(Object.getName());
	_soObject.setOBJ(Object);
	// Fill params for SO 
	
	soParam.addINT("iOwnerNat");
	if ( pPL->IsLocal("iOwnerNat") )	soParam.getINT("iOwnerNat") = iOwnerNat;
	else								soParam.setREF("iOwnerNat",pPL->getVOID("iOwnerNat"));
		
	soParam.addINT("iMyStartNat") = Object.getNation();
	
	if ( pPL->check("nRallyPoint") ){
		iNode ndRP = soParam.addNODE("nRallyPoint");
		ndRP.setX( (pPL->getNODE("nRallyPoint")).getX() );
		ndRP.setY( (pPL->getNODE("nRallyPoint")).getY() );
	};
	
	soParam.addINT("iWaitTime");
	if ( pPL->IsLocal("iWaitTime") )	soParam.getINT("iWaitTime") = iWaitTime;
	else								soParam.setREF("iWaitTime",pPL->getVOID("iWaitTime"));
	
	soParam.addARRAY("arrTN");
	soParam.setREF("arrTN",arrTN);
	
	soParam.addBOOL("g_bActive") = true;
	if ( pPL->check("g_bActive") ){
		if ( pPL->IsLocal("g_bActive") )	soParam.getINT("g_bActive") = pPL->getINT("g_bActive");
		else								soParam.setREF("g_bActive",pPL->getVOID("g_bActive"));
	};
	
	soParam.addINT("iCommandID") = iCommandID;
	
	if ( ( iCommandID==1 || iCommandID==2 ) && pPL->check("g_gDestGroup") ){
		// Добавим g_gDestGroup в список параметров для FSM 
		soParam.addGROUP("g_gDestGroup");
		soParam.setREF("g_gDestGroup",pPL->getGROUP("g_gDestGroup"));
	};
	if ( iCommandID==0 ){
		// Add group for store all produceed units
		soParam.addGROUP("l_gStoreGroup");
	};
	
	soParam.addBOOL("g_bGuardZone") = false;
	if ( pPL->check("g_bGuardZone") ) soParam.getBOOL("g_bGuardZone") = pPL->getBOOL("g_bGuardZone");

	//-------------------------------------------//
	
	// Run new dialog
	_soObject.setSTATE("ST_GLOBAL");
	GI::SendMSG( "MSG_ENTER", _soObject, _soObject );
	
	//-------------------------------------------//
	// Change Guard
	if ( pPL->check("g_gGuard") ){
		__GROUP( g_gGuard );
		// Set new Guard MASK
		int _iOwnerNMASK = GetNationNMASK( iOwnerNat );
		int _iMyStartNat = soParam.getINT("iMyStartNat");
		if ( iOwnerNat!=_iMyStartNat ) _iOwnerNMASK += GetNationNMASK(_iMyStartNat);
		g_gGuard.setNMASK(_iOwnerNMASK);
		g_gGuard.ClearOrders();
		// Set for Guard -> guard zone
		pSMARTOBJECT _soGuard =	GI::FindSObyName( g_gGuard.getName() );
		if ( _soGuard.Valid()==false ){
			pParamList paramData;
			paramData.CreateOBJ_PL();
			if ( pPL->check("g_bGuardZone") )	paramData.addBOOL("bGuardZone")		= pPL->getBOOL("g_bGuardZone");
			else								paramData.addBOOL("bGuardZone")		= false;
			paramData.addINT("R")				= 500;
			paramData.addINT("iAddR")			= 350;
			paramData.addGROUP("SecuredGroup");
			paramData.setREF("SecuredGroup",Object);
			ACF_GuardZone00_local( g_gGuard, paramData );
			paramData.DeleteOBJ_PL();
		};
		_soGuard = GI::FindSObyName( g_gGuard.getName() );
		if ( _soGuard.Valid() ){
			if ( (_soGuard.getPARAMS()).check("bGuardZone") ){	
				if ( pPL->check("g_bGuardZone") ){
					(_soGuard.getPARAMS()).getBOOL("bGuardZone") = pPL->getBOOL("g_bGuardZone");
				}else{
					(_soGuard.getPARAMS()).getBOOL("bGuardZone")=false;
				};
			};
		};

		//-------------------------------------------//
		// Try activate guard group support
		if ( pPL->check("UpdateGuard")==false || pPL->getBOOL("UpdateGuard") ){
			// Add guard group ref to FSM param
			soParam.addGROUP("gGuardGroup");
			soParam.setREF("gGuardGroup",g_gGuard);		
			// Create guard group units data array
			pARRAY _arrTN_GG = soParam.addARRAY("arrTN_GG");
			g_gGuard.FillTN_array(_arrTN_GG);
		};
		//-------------------------------------------//
	};
		
	END_OK;
};

/*
	ACTION:
		CommandID = 0;
		Группа отправляется в бой по команде AddTomahawks(...)
*/
EDITOR_FUNCTION	ACF_ProduseUnits_02			(__PARAMS__){
	if (pPL==NULL||pPL->Valid()==false) END_FAIL;
	pPL->addINT("iCommandID") = 0;
	return AFC_ProduseUnits_02_local(pPL);
};

/*
	ACTION:
		CommandID = 1;
		Группа скидывает себя в группу "g_gDestGroup" и ничего не делает
*/
EDITOR_FUNCTION	ACF_ProduseUnits_03			(__PARAMS__){
	if (pPL==NULL||pPL->Valid()==false) END_FAIL;
	pPL->addINT("iCommandID") = 1;
	return AFC_ProduseUnits_02_local(pPL);
};

/*
	ACTION:
		CommandID = 2;
		Группа скидывает себя всегда при окончании постройки 
		в группу "g_gDestGroup" и ничего не делает
*/
EDITOR_FUNCTION	ACF_ProduseUnits_04			(__PARAMS__){
	if (pPL==NULL||pPL->Valid()==false) END_FAIL;
	pPL->addINT("iCommandID") = 2;
	return AFC_ProduseUnits_02_local(pPL);
};

//////////////////////////////////////////////////////////////////////////























