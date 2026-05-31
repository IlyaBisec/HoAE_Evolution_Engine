#include "stdafx.h"

void	DeclareteDLL_fsm_Attack(){
	
	GI::AddFunctionToDeclarationList("ACF_AttackZone00");
	GI::AddParamsToFunctionInList("ACF_AttackZone00",	"GROUP"	, "Object"	);
	GI::AddParamsToFunctionInList("ACF_AttackZone00",	"INT"	, "wait"	);
	GI::AddParamsToFunctionInList("ACF_AttackZone00",	"NODE"	, "destNode");

	GI::AddFunctionToDeclarationList("ACF_AttackZone01");
	GI::AddParamsToFunctionInList("ACF_AttackZone01",	"GROUP"	, "Object"		);
	GI::AddParamsToFunctionInList("ACF_AttackZone01",	"INT"	, "wait"		);
	GI::AddParamsToFunctionInList("ACF_AttackZone01",	"ARRAY"	, "arryNode"	);
	GI::AddParamsToFunctionInList("ACF_AttackZone01",	"INT"	, "iDestDelta"	);
	GI::AddParamsToFunctionInList("ACF_AttackZone01",	"BOOL"	, "bCiclik"		);	
	
};

//////////////////////////////////////////////////////////////////////////
// FSM_Attack			++
//////////////////////////////////////////////////////////////////////////
/*	Ходьба с атакой
	USE DEFF PARAMS:
		"wait"		- задержка update
		"tick"		- последнее время обновления
		"destNode"	- зона следования
*/
EDITOR_FUNCTION	FSM_AttackZone_00	( pSMARTOBJECT& C_OBJ, _str& state, pMSG& msg ){
	FSM_CHECKP();
	if ( SO_GROUP_IS_DEAD(C_OBJ) ) END_FAIL;
	
	pGroup		_group 	= C_OBJ.getOBJ_GROUP();
	pParamList	_par	= C_OBJ.getPARAMS();
	
	if	( state == "ST_GLOBAL" ){
		if ( msg.str() == "MSG_ENTER" ){
			if ( _par.check("iDestDelta") ) _group.setDestDelta( _par.getINT("iDestDelta"));
			else							_group.setDestDelta( 128 );
			
			if ( _par.check("wait")==false ) _par.addINT("wait")=10;
			int& _wait = _par.getINT("wait");
			_par.addINT("tick") = _wait;
			// Select way point sending type
			int& _wayType = _par.addINT("wayType");
			if 		( _par.check("destNode") )	_wayType = 0;
			else if	( _par.check("arryNode") ){
				_wayType = 1;
				int& maxD = _par.addINT("maxD");
				maxD = 0;
				pARRAY ARR = _par.getARRAY("arryNode");
				if ( _par.getBOOL("bCiclik") ){
					maxD = GI::NORMA( 
									 ARR.getNODE(0).getX(),ARR.getNODE(ARR.getSIZE()-1).getX(),
									 ARR.getNODE(0).getY(),ARR.getNODE(ARR.getSIZE()-1).getY()
									);
				};
				for	( int i=0; i<ARR.getSIZE()-1; i++ ){
					int DDD = GI::NORMA( 
										ARR.getNODE(i).getX(),ARR.getNODE(i+1).getX(),
										ARR.getNODE(i).getY(),ARR.getNODE(i+1).getY()
									   );
					if ( maxD==0 || maxD<DDD ) maxD=DDD;
				};	
			}else								_wayType = 2;
			// ----------------------------- //
			__SetState(ST_ATTACK);
		};
		END_OK;
	}else if( state == "ST_ATTACK" ){
		if ( msg.str() == "MSG_UPDATE" ){
			int& _wayType	= _par.getINT("wayType");
			int& _tick		= _par.getINT("tick");
			_tick--;
//			if ( _wayType==1 ){
//				GI::ControlNodeArray(_par.getARRAY("arryNode"),_group.getLockType(),_group.getNation());
//			};
			bool reSend = false;
			if ( _wayType==1 ){
				pARRAY ARR = _par.getARRAY("arryNode");
				reSend = GI::MarkBadNode(ARR,_group.getLockType(),_group.getNation());
			};
			bool reSendONORD = ( _group.getNO_ORDERS_UN()>0/*_group.getUN()/3*/ && _tick>0 );
			if (_tick<=0 || reSend || reSendONORD ){
				// need HARD UPDATE !!!
				if ( _tick<=0 ){
					if ( _wayType==1 ){
						pARRAY ARR = _par.getARRAY("arryNode");
						_group.AttackUseIDS	(	
												vgMOVE|vgATTACK|vgSOME_ORDER|vgBRIG_ORDER,
												true,
												_par.getINT("maxD"),
												ARR
											);
					}else
						_group.AttackUseIDS	( vgMOVE|vgATTACK|vgSOME_ORDER|vgBRIG_ORDER );
				}else if ( reSendONORD ){
					if ( _wayType==1 ){
						pARRAY ARR = _par.getARRAY("arryNode");
						_group.AttackUseIDS	(	
												vgNO_ORDERS,
												true,
												_par.getINT("maxD"),
												ARR
											);
					}else
						_group.AttackUseIDS	( vgNO_ORDERS );
				}
				
				// ***************** //

				// Mark bad node
				if( _wayType==1 || _wayType==0 ){
					int		_NU		= _group.getUN();
					pARRAY _arrNode;
					if ( _wayType==1 ) _arrNode = _par.getARRAY("arryNode");
					iNode	_lNode;
					bool _cirkle = false;
					if ( _par.check("bCiclik") ) _cirkle=_par.getBOOL("bCiclik");
					// use hash
					int maxD	= 300;
					int lx=-1,ly=-1;
					int gx,gy;
					int _iNID	= -1;
					bool reSEND=false;
					int arrN = 0;
					if ( _wayType==1 ) arrN = _arrNode.getSIZE();
					while( _NU-- ){
						if ( 
								!( _group.getORDERi(_NU) & (vgMOVE|vgSOME_ORDER) ) || reSend ||
								( _group.getORDERi(_NU)==vgNO_ORDERS && reSendONORD )
						   )
						{
							// Link set node position
							gx=_group.getXi(_NU); gy=_group.getYi(_NU);
							addrand(gx);
							addrand(gy);
							if ( _wayType==1 ){
								if ( _iNID*lx*ly<0 || ( GI::NORMA(lx,gx,ly,gy)>maxD ) ){
									lx=gx; ly=gy;
									_iNID = GetNearestNodeID( _arrNode, gx, gy, _cirkle );
								};
								_lNode = _arrNode.getNODE( _iNID );
							};
							if ( _wayType==0 ) _lNode = _par.getNODE("destNode");
							int		first = 0;
							if ( _lNode.getR()>=maxD ) maxD = _lNode.getR();
							bool UnitInpos = GI::NORMA(gx,_lNode.getX(),gy,_lNode.getY())<=maxD;
							if ( _wayType==1){
								if ( _iNID<arrN-1 ){
									for ( int n=_iNID; n<arrN; n++ ){
										_lNode = _arrNode.getNODE( n );
										if ( _lNode.getSFR()!=100 || n==_iNID ){
											_group.SendID(_lNode,vgMOVE|vgSOME_ORDER|vgBRIG_ORDER,_NU,DIR_AUTO,first);
											first = 2;
										}else n=arrN+1;
									};
								}else if ( _iNID==arrN-1 && !UnitInpos ){
									if ( _lNode.getSFR()!=100 ){
										_group.SendID(_lNode,vgMOVE|vgSOME_ORDER|vgBRIG_ORDER,_NU,DIR_AUTO,first);
									};
								}else if ( _cirkle ){
									for ( int n=0; n<arrN; n++ ){
										_lNode = _arrNode.getNODE( n );
										if ( _lNode.getSFR()!=100 || n==_iNID ){
											_group.SendID(_lNode,vgMOVE|vgSOME_ORDER|vgBRIG_ORDER,_NU,DIR_AUTO,first);
											first = 2;
										}else n=arrN+1;
									};
								};
							}else if (_wayType==0 ){
								if (!UnitInpos){
                                    _group.SendID(_lNode,vgMOVE|vgSOME_ORDER|vgBRIG_ORDER,_NU,DIR_AUTO,first,false);
								}else if ( GI::random(300)==0 ){
									addrand(123);
									_group.MarkTime(_lNode,20);	
								};
							};

                            // ---------------------- //
						};
					};
				};
				int& _wait	= _par.getINT("wait");
				_tick = _wait;
			};
		};
		END_OK;
	};
	
	END_FAIL;
};

EDITOR_FUNCTION	ACF_AttackZone00			(__PARAMS__){
	__GROUP(Object);
	if ( Object.getUN()<=0 ) END_FAIL;
	if ( GI::CheckObjectFSM( Object.getName(),"FSM_AttackZone00" ) ) END_OK;
	// Free SO for this Group if need 
	GI::FreeSO_pVAR( Object );
	// Get free or create new SO
	pSMARTOBJECT _soObject = GI::GET_SO("FSM_AttackZone00");
	if ( _soObject.Valid()==false ) END_FAIL;
	// Get SO params 
	pParamList soParam = _soObject.getPARAMS();
	if ( soParam.Valid()==false )	END_FAIL;
	soParam.CLEAR();
	// Set controled object for dialog 
//	if ( _soObject.setOBJ(Object.getName())==false ) END_FAIL;
	if ( _soObject.setOBJ(Object)==false ) END_FAIL;
	// Fill params for SO 
	if ( pPL->check("wait") ) soParam.addINT("wait") = pPL->getINT("wait");
	if ( pPL->check("destNode") ){
//		iNode _node = soParam.addNODE("destNode");
//		_node.setX( (pPL->getNODE("destNode")).getX() );
//		_node.setY( (pPL->getNODE("destNode")).getY() );
//		_node.setR( (pPL->getNODE("destNode")).getR() );
		soParam.addNODE("destNode");
		soParam.setREF("destNode",pPL->getVOID("destNode"));
	};
	// Run new dialog
	_soObject.setSTATE("ST_GLOBAL");
	GI::SendMSG("MSG_ENTER",_soObject,_soObject);
	END_OK;
};

EDITOR_FUNCTION	ACF_AttackZone01			(__PARAMS__){
	__GROUP(Object);
	if ( Object.getUN()<=0 )	END_FAIL;
	if ( GI::CheckObjectFSM(Object.getName(),"FSM_AttackZone00") ) END_OK;
	// Free SO for this Group if need 
	GI::FreeSO_pVAR(Object);
	// Get free or create new SO 
	pSMARTOBJECT _soObject = GI::GET_SO("FSM_AttackZone00");
	if ( _soObject.Valid()==false ) END_FAIL;
	// Get SO params 
	pParamList soParam = _soObject.getPARAMS();
	if ( soParam.Valid()==false)	END_FAIL;
	soParam.CLEAR();
	// Set controled object for dialog 
//	if ( _soObject.setOBJ(Object.getName())==false ) END_FAIL;
	if ( _soObject.setOBJ(Object)==false ) END_FAIL;
	// Fill params for SO 
	if ( pPL->check("wait") ){
		soParam.addINT("wait");
		soParam.setREF("wait",pPL->getVOID("wait"));
	};
	if ( pPL->check("arryNode") ){
		soParam.addARRAY("arryNode");
		soParam.setREF("arryNode",pPL->getARRAY("arryNode"));
	};
	if ( pPL->check("iDestDelta") ) soParam.addINT("iDestDelta") = pPL->getINT("iDestDelta");
	if ( pPL->check("bCiclik") )	soParam.addBOOL("bCiclik") = pPL->getBOOL("bCiclik");
	// Run new dialog 
	_soObject.setSTATE("ST_GLOBAL");
	GI::SendMSG("MSG_ENTER",_soObject,_soObject);
	END_OK;
};

//////////////////////////////////////////////////////////////////////////



















