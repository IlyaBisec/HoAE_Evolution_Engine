#include "stdafx.h"

void	DeclareteDLL_fsm_Dialog(){
	
	GI::AddFunctionToDeclarationList("ACF_CloudDialog");
	GI::AddParamsToFunctionInList("ACF_CloudDialog",	"GROUP"	, "Object"		);
	GI::AddParamsToFunctionInList("ACF_CloudDialog",	"ARRAY"	, "DialogArr"	);
	GI::AddParamsToFunctionInList("ACF_CloudDialog",	"BOOL"	, "bDone"		);
	GI::AddParamsToFunctionInList("ACF_CloudDialog",	"BOOL"	, "PauseGame"	);
	GI::AddParamsToFunctionInList("ACF_CloudDialog",	"BOOL"	, "AddToHistory");

	GI::AddFunctionToDeclarationList("GET_CloudDialogFree");	
	
};

//////////////////////////////////////////////////////////////////////////
// FSM_CloudDialog		++
//////////////////////////////////////////////////////////////////////////
/*
Обеспечивает разговор двух-трех персонажей в игре
*/
/*
	"Object" 	- controled object
	"DialogArr"	- dialogs array
*/
EDITOR_FUNCTION FSM_CloudDialog		( pSMARTOBJECT& C_OBJ, _str& state, pMSG& msg ){
	FSM_CHECKP();
	if ( SO_GROUP_IS_DEAD(C_OBJ) ) END_FAIL;
	
	pGroup		_group 	= C_OBJ.getOBJ_GROUP();
	pParamList	_par	= C_OBJ.getPARAMS();
	
	if 		( state == "ST_GLOBAL" 	){
		if ( ProcMsgFree( msg, false ) ) END_OK;
		_par.addINT("curID") = 0;
		_par.addINT("delay");
		_par.addINT("time");
		_par.addBOOL("oggPresent") = false;
		__SetState(ST_END);
	}else if( state == "ST_NEW_MSG" ){
		if ( ProcMsgFree( msg, false ) ) END_OK;
		if 		( msg.str() == "MSG_ENTER" 	){
			int&	_curID = _par.getINT("curID");
			int&	_delay = _par.getINT("delay");
			int&	_time  = _par.getINT("time");
			if ( _par.check("DialogArr")==false ) END_FAIL;
			pARRAY&	_arrGS = _par.getARRAY("DialogArr");
			int		_arrN  = _arrGS.getSIZE();
			if ( _curID >= _arrN )	__SetState(ST_END);

			pSTR_DIALOG _strOBJ = _arrGS.getDIALOG( _curID );
			
			_delay	= 2500 * GI::dl_GetLineCountInCloudDialog( _strOBJ ) + GI::dl_getAdditionalDelay();
			_time	= GI::GetGlobalTime();
			GI::dl_ShowCloudDialog( _strOBJ,_group );
			if ( _strOBJ.get_ogFile() && GI::FilePresent( _strOBJ.get_ogFile() ) && GI::ogg_GetSoundVolume()!=0 ){
				GI::ogg_Play( _strOBJ.get_ogFile(), 1 );
				_par.getBOOL("oggPresent") = true;
				_delay = 1000;
			};

			if (_par.getBOOL("Pause")&&_curID==0) {
				GI::SetLockPause(true);
				GI::PauseScript();
				// Move screen to position bitvine speakers
				int x,y;
				if ( GI::dl_getCenterOfSPEAKERS(x,y,_group) ){
					GI::ScrollToPositionOnMap(x,y,400);
				};
			};
			
			_curID++;
		}else if( msg.str() == "MSG_UPDATE" ){
			int/*&*/	_delay		= 1000;//_par.getINT("delay");
			int&	_time		= _par.getINT("time");
			bool&	_oggPresent = _par.getBOOL("oggPresent");
			// Cancel dialog by left click
			if ( GI::dl_GetCloudDilogCancel() ){
				GI::dl_SetCloudDilogCancel(false);
				//GI::ogg_Stop(1);
				GI::SendMSG( "MSG_ENTER", C_OBJ, C_OBJ );
			};
			// Center dialog by right click
			if (GI::dl_GetCloudDilogCenter()) {
				GI::dl_SetCloudDilogCenter(false);
				int x,y;
				if ( GI::dl_getCurrentSpeakerPosition(x,y) ){
					GI::ScrollToPositionOnMap(x,y,400);
				};
			};
			if ( !_par.getBOOL("Pause") && GI::GetGlobalTime()-_time > _delay ){
				if ( _oggPresent && GI::ogg_StreamFinished(1) ){
					_oggPresent = false;
					_time	= GI::GetGlobalTime();
					_delay  = 1000;//GI::dl_getAdditionalDelay();
				}else if ( _oggPresent == false ){
					GI::SendMSG( "MSG_ENTER", C_OBJ, C_OBJ );
				};
			};
		}else if( msg.str() == "MSG_EXIT" ){
			if ( _par.check("bDone") ){
				_par.getBOOL("bDone") = true;
				_par.setREF("bDone",NULL);
			};
			GI::dl_ClearCloudDilogSPEAKERS();
			GI::dl_HideCloudDialog();
			if ( _par.getBOOL("Pause") ){
				GI::SetLockPause(false);
				GI::RunScript();
			};
		};
		END_OK;
	}else if( state == "ST_END" 	){
		if ( ProcMsgFree( msg, true ) ) END_OK;
		if	( msg.str() == "MSG_RUN" ){
			_par.getINT("curID") = 0;
			// Add Spekers list to CloudDialog
			if ( _par.check("DialogArr") ){
				pARRAY	_arrGS			= _par.getARRAY("DialogArr");
				bool&	_addToHistory	= _par.getBOOL("AddToHistory");
				if ( !_addToHistory ) 
					GI::AddDialogToHistory( _arrGS );
				int		_arrN  = _arrGS.getSIZE();
				for ( int i=0; i<_arrN; i++ ){
					pSTR_DIALOG _strOBJ = _arrGS.getDIALOG( i );
					GI::dl_AddCloudDilogSPEAKERS( _strOBJ.get_Group() );
				};
			};
			// ------------------------------- //
			__SetState(ST_NEW_MSG);
		};
		END_OK;
	};
	
	END_FAIL;
};

EDITOR_FUNCTION	ACF_CloudDialog				(__PARAMS__){
	pSMARTOBJECT	soDialog = GI::FindSO( "FSM_CloudDialog" );
	if ( soDialog.Valid()==false ){
		soDialog = GI::GET_SO( "FSM_CloudDialog" );
		if ( soDialog.Valid()==false ) END_FAIL;
		soDialog.setName( "g_soCloudDialogs" );
	}else{
		if ( soDialog.getSTATE()=="ST_NEW_MSG" ) GI::SendMSG("MSG_EXIT",soDialog,soDialog);
	};
	// Get SO params 
	pParamList	soParam = soDialog.getPARAMS();
	if ( soParam.Valid()==false ) END_FAIL;
	soParam.CLEAR();
	// Set controled object for dialog 
	if ( pPL->check("Object")==false ) END_FAIL;
	pGroup DialogObj = pPL->getGROUP("Object");
	soDialog.setOBJ(DialogObj.getName());
	// Set dialog array for new dialog 
	if ( pPL->check("DialogArr")==false ) END_FAIL;
	pARRAY DialogARR = pPL->getARRAY("DialogArr");
	soParam.addARRAY("DialogArr");
	soParam.setREF("DialogArr",DialogARR);
	// Set check done variable 
	bool& Done = soParam.addBOOL("bDone");
	if ( pPL->check("bDone") )	soParam.setREF("bDone",pPL->getVOID("bDone"));
	else						soParam.setREF("bDone",NULL);
	// Create curID and delay 
	soParam.addINT("curID");
	soParam.addINT("delay");
	soParam.addINT("time");
	soParam.addBOOL("oggPresent") = false;
	// If need pause game for dialog
	bool& _pause = soParam.addBOOL("Pause");
	_pause = false;
	if ( pPL->check("PauseGame") ) _pause = pPL->getBOOL("PauseGame");
	bool& _addToHistory = soParam.addBOOL("AddToHistory");
	_addToHistory = false;
	if ( pPL->check("AddToHistory") ) _addToHistory = pPL->getBOOL("AddToHistory");
	// Run new dialog 
	if ( (soDialog.getSTATE()).pchar()==NULL ){
		soDialog.setSTATE("ST_GLOBAL");
		GI::SendMSG("MSG_ENTER",soDialog,soDialog);
	};
	soDialog.setSTATE("ST_END");
	soDialog.FSC(false);
	GI::SendMSG("MSG_RUN",soDialog,soDialog);
	END_OK;
};

EDITOR_FUNCTION	GET_CloudDialogFree			(__PARAMS__){
	pSMARTOBJECT soDialog = GI::FindSO( "FSM_CloudDialog" );
	if ( soDialog.Valid()==false || (soDialog.getSTATE()).pchar()==NULL ) END_OK;
	if ( soDialog.getSTATE()!="ST_END" ) END_FAIL;
	END_OK;	
};

//////////////////////////////////////////////////////////////////////////





























