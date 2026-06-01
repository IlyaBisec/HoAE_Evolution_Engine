#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
// ST_Attack	
//////////////////////////////////////////////////////////////////////////
/*	Simple Attack -> attack enemy in zones
	USE :
		C_OBJ	- current smart object
		msg		- current message
		_group	- controled group
		_enemy 	- enemy group
		_zone	- zone for guard
		_addR	- aditional R for serch archers
*/
int		ST_ATTACK_00		( pSMARTOBJECT& C_OBJ, pMSG& msg ){
	pGroup		_group	= C_OBJ.getOBJ_GROUP();
	pParamList	_par	= C_OBJ.getPARAMS();
	iNode		_zone	= _par.getNODE("GuardNode");
	int			_addR	= _par.getINT("iAddR");
	pGroup		_enemy	= _par.getGROUP("EnemyGroup");
	int			_Nat 	= _group.getNation();
	int			_NMask  = _group.getNMASK();
	if 			( msg.str() == "MSG_ENTER" 		){
		_group.setSearchEnemy(true);
		_group.setAIGuard(false);
		_enemy.RemAllUnits();
		GI::SendMSG("MSG_CREATE_EN",C_OBJ,C_OBJ);
		return 1;
	}else if 	( msg.str() == "MSG_CREATE_EN" 	){
		_enemy.AddEnemyUnitsInZone(_Nat,_zone,_addR*2,false);
		if ( _enemy.getUN() ){
			if (_enemy.getNew_UN(true)||_enemy.getRem_UN(true)||_group.getNO_ORDERS_UN()){
				GI::SendMSG("MSG_ATTACK_EN",C_OBJ,C_OBJ);
			};
		};
		return 1;
	}else if 	( msg.str() == "MSG_ATTACK_EN" 	){
		// Reorder targets for attack
		_group.AttackUseNearest(_enemy);
		return 1;
	}else if	( msg.str() == "MSG_UPDATE" 		){
		int  EnN = getEnemyAmount(_NMask,_zone,_addR*2);
		int  EbN = getEnemyAmount(_NMask,_zone,_addR*2,COLLECT_BUILDINGS);
		bool EnY = _group.getAttackPossibility(_zone,_addR*2);
		if ( (EnN+EbN)==0 || EnY==false ){
			__SetState(ST_BACK);
		}else if ( (EnN!=_enemy.getUN()&&EbN!=_enemy.getUN()) || _group.getNO_ORDERS_UN() ){
			GI::SendMSG("MSG_CREATE_EN",C_OBJ,C_OBJ);
		};
		return 1;
	}else if	( msg.str() == "MSG_EXIT" 	){
		_group.setUnderAttack(false);
		if  ( _par.check("SecuredGroup") )	(_par.getGROUP("SecuredGroup")).setUnderAttack(false);
		_enemy.RemAllUnits();
		return 1;
	};

	return 1;
};

int		ST_ATTACK_00_01		(pSMARTOBJECT& C_OBJ,pMSG& msg,char* strX,char* strY,char* strR,char* strAREAID,char* strNextSTATE ){
	pGroup		_group	= C_OBJ.getOBJ_GROUP();
	pParamList	_par	= C_OBJ.getPARAMS();
	int			_x		= _par.getINT(strX) * 64 + 32;
	int			_y		= _par.getINT(strY) * 64 + 32;
	int			_R		= _par.getINT(strR);
	if ( _par.check("EnemyGroup")==false )	_par.addGROUP("EnemyGroup");
	pGroup	_enemy	= _par.getGROUP("EnemyGroup");
	int		_Nat 	= _group.getNation();
	if 		( msg.str() == "MSG_ENTER" 		){
		_group.setSearchEnemy(true);
		_group.setAIGuard(false);
		_enemy.RemAllUnits();
		GI::SendMSG( "MSG_CREATE_EN", C_OBJ, C_OBJ );
		// Запоминаем текущую AreaID для атаки
		if ( _par.check("curAREAID")==false )	_par.addINT("curAREAID");
		_par.getINT("curAREAID") = _par.getINT( strAREAID );
	}else if( msg.str() == "MSG_CREATE_EN" 	){
		_enemy.AddEnemyUnitsInZone(_Nat,_x,_y,_R,false);
		if ( _enemy.getUN() > 0 ){
			if ( _enemy.getNew_UN(true) || _enemy.getRem_UN(true)>0 || _group.getNO_ORDERS_UN() ){
				GI::SendMSG( "MSG_ATTACK_EN", C_OBJ, C_OBJ );
			};
		};
	}else if( msg.str() == "MSG_ATTACK_EN" 	){
		// перераспредилить цели для аттаки
		_group.AttackUseNearest(_enemy);
	}else if( msg.str() == "MSG_UPDATE" 		){
		// Аварийно выходим - смена позиции атаки //
		if ( _par.getINT(strAREAID) != _par.getINT("curAREAID") ){
			C_OBJ.next_STATE(strNextSTATE);
			C_OBJ.FSC(true);
			END_OK;
		};
		// ************************************** //
		int _NMask = _group.getNMASK();
		int  EnN = getEnemyAmount(_NMask,_x,_y,_R);
		int  EbN = getEnemyAmount(_NMask,_x,_y,_R,COLLECT_BUILDINGS);
		bool EnY = _group.getAttackPossibility(_x,_y,_R);
		if ( (EnN+EbN)==0 || EnY==false ){
			C_OBJ.next_STATE(strNextSTATE);
			C_OBJ.FSC(true);
		}else if ( (EnN!=_enemy.getUN()&&EbN!=_enemy.getUN()) || _group.getNO_ORDERS_UN() ){
			GI::SendMSG("MSG_CREATE_EN",C_OBJ,C_OBJ);
		};
	}else if( msg.str() == "MSG_EXIT" 	){
		_group.setUnderAttack(false);
		_enemy.RemAllUnits();
		_par.getINT(strAREAID) = -1;
	};
	END_OK;
};



/* Simple Attack -> attack nearest enemy */
/* USE :
		C_OBJ	- current smart object
		msg		- current message
		_group	- controled group
*/
/*
Description:
	-	Просто атакуем всех кто рядом. После переходим в
		заддоное состояние.
*/
int		ST_ATTACK_01		( pSMARTOBJECT& C_OBJ, pMSG& msg, char* strNextSTATE ){
	pGroup		_group	= C_OBJ.getOBJ_GROUP();
	pParamList	_par	= C_OBJ.getPARAMS();
	if 		( msg.str() == "MSG_ENTER" 		){
		// Не обрабатываем пустую группу 
		if ( _group.Valid()==false || _group.getUN()<=0 ){
			GI::SendMSG( "MSG_EXIT", C_OBJ, C_OBJ );
			END_OK;
		};
		// Готовим объект к использованию 
		_group.setSearchEnemy(true);
		_group.setAIGuard(false);
		// Добавим задержку на поиск врага (def == 5) 
		if ( _par.check("l_ticker")==false ) _par.addINT("l_ticker");
		_par.getINT("l_ticker") = 0;
	}else if( msg.str() == "MSG_UPDATE" 	){
		int& l_ticker = _par.getINT("l_ticker");
		if ( l_ticker == 0 ){
			_group.AttackUseIDS(vgMOVE|vgATTACK|vgSOME_ORDER|vgBRIG_ORDER);
			//	Выходим из состояния: 
			//		- Все погибли
			//		- Нет врагов
			if ( _group.getUN()<=0 || _group.getEnemyAround_UN(1000 + (_group.getUN()/100)*100)<=0 ){
				C_OBJ.next_STATE(strNextSTATE);
				C_OBJ.FSC(true);
				END_OK;
			};
		};
		( (l_ticker>5) ? (l_ticker=0) : (l_ticker+=1) );
	}else if( msg.str() == "MSG_EXIT" 	){
		if ( _group.Valid() && _group.getUN() ){
			_group.setSearchEnemy(false);
			_group.setUnderAttack(false);
		};
	};
	END_OK;
};
//////////////////////////////////////////////////////////////////////////
// ST_Back			
//////////////////////////////////////////////////////////////////////////
/* Simple Back -> react on enemy in zone or archers
	USE :
		C_OBJ	- current smart object
		msg		- current message
		_group	- controled group
		_zone	- zone for guard
		_addR	- aditional R for serch archers
*/
__declspec(dllimport) void UnitAbilityTest2();
int		ST_BACK_00			( pSMARTOBJECT& C_OBJ, pMSG& msg ){
	//UnitAbilityTest2();
	pGroup		_group			= C_OBJ.getOBJ_GROUP();
	pParamList	_par			= C_OBJ.getPARAMS();
	iNode		_zone			= _par.getNODE("GuardNode");
	int			_addR			= _par.getINT("iAddR");
	if 		( msg.str() == "MSG_ENTER" 	){
		//UnitAbilityTest2();
		_group.setSearchEnemy(false);
		// Send to pos Array
		_group.Send(_zone,DIR_AUTO,0);
		//UnitAbilityTest2();
		if ( _par.check("arrGrpPos") ){
			pARRAY arrGrpPos = _par.getARRAY("arrGrpPos");
			int N = min( arrGrpPos.getSIZE(), _group.getUN() );
			iNode l__nodeID;
			//UnitAbilityTest2();
			for (int i=0; i<N; i++){
				l__nodeID = arrGrpPos.getNODE(i);
				if (l__nodeID.Valid()) _group.SendID(i,l__nodeID,l__nodeID.getDIR(),0);
			};
			//UnitAbilityTest2();
		};
	}else if ( msg.str() == "MSG_UPDATE" 	){
		int	_Nat 		 = _group.getNation();
		int _NMask		 = _group.getNMASK();
		int _EnN_R 		 = getEnemyAmount(_NMask,_zone);
		int _EnN_addR 	 = getEnemyAmount(_NMask,_zone,_addR);
		int _En_YES_R	 = _group.getAttackPossibility(_zone,_addR);
		int _En_YES_addR = _group.getAttackPossibility(_zone,_addR*2);
		if ( ( _EnN_R || ( _group.getUnderAttack()==true && _EnN_addR ) ) && _En_YES_R ){
			__SetState(ST_ATTACK);
		}else if ( _En_YES_addR&&_par.check("SecuredGroup")&&(_par.getGROUP("SecuredGroup")).getUnderAttack()){
			__SetState(ST_ATTACK);
		}else if ( _group.getNO_ORDERS_UN()==_group.getUN() ){
			if ( _par.check("arrGrpPos")&&GroupCheckPosArray(_group,_par.getARRAY("arrGrpPos")) ){
				__SetState(ST_IDLE);
			}else{
				GI::SendMSG("MSG_ENTER",C_OBJ,C_OBJ);
			};
		};
	};
	return 1;
};

//////////////////////////////////////////////////////////////////////////
// ST_Idle			
//////////////////////////////////////////////////////////////////////////
/* FOR UNITS */
/* Simple Idle -> react on enemy if it attack me
	USE :
		"GuardNode"	- guarded position
		"iAddR"		- additional radius
*/
int		ST_IDLE_00			( pSMARTOBJECT& C_OBJ, pMSG& msg ){
	pGroup		_group		= C_OBJ.getOBJ_GROUP();
	pParamList	_par		= C_OBJ.getPARAMS();
	iNode		_zone		= _par.getNODE("GuardNode");
	int			_addR		= _par.getINT("iAddR");
	bool		_bGuardZone = _par.getBOOL("bGuardZone");
	int			_Nat	 	= _group.getNation();
	int			_NMask		= _group.getNMASK();
	int zx=_zone.getX();
	int zy=_zone.getY();
	addrand(zx);
	addrand(zy);
	if 			( msg.str() == "MSG_ENTER" ){
		_group.setSearchEnemy(false);
		_group.setAIGuard(!_bGuardZone);
		if ( _par.check("RespawnDelay") ){
			int& _RespawnDelay = _par.getINT("RespawnDelay");
			if( _RespawnDelay>0 ){
				bool NeedRespawn=false;
				pARRAY arrGrpPos = _par.getARRAY("arrGrpPos");
				int NP=arrGrpPos.getSIZE();
				int	NU=_group.getUN();
				//for(int i=0;i<n;i++){
				//	//pUnit U=_group[i];
				//	if( !U.Valid() || U.GetLiveStatus()==0 ){
				//		NeedRespawn=true;
				//		break;
				//	}
				//}
				NeedRespawn=NP>NU;
				if( NeedRespawn ){
					if( !_par.check("RespawnTime") ) _par.addINT("RespawnTime");
					int& _RespawnTime = _par.getINT("RespawnTime");
					int T=GameInterface::GetGlobalTime();
					_RespawnTime=T+_RespawnDelay;
				}
			}
		}
		return 1;
	}else if	( msg.str() == "MSG_UPDATE" 	){
		
		bool	setStateAttack = (
					// hack by vital
					//(	
					//	getEnemyAmount(_NMask,_zone,0,COLLECT_BUILDINGS) 
					//	&&
					//	_group.getAttackPossibility(_zone,_addR*2)
					//)
					//||
					(
						!_bGuardZone
						&&
						_group.getUnderAttack()
						&&
						getEnemyAmount(_NMask,_zone,_addR*2)
						&&
						_group.getAttackPossibility(_zone,_addR*2)
					)
					||
					(
						_bGuardZone
						&&
						(
							_group.getUnderAttack()
							||
							getEnemyAmount(_NMask,_zone)
						)
						&&
						_group.getAttackPossibility(_zone,_addR*2)
					)
					||
					(
						_par.check("SecuredGroup")
						&&
						(_par.getGROUP("SecuredGroup")).getUnderAttack()
					)
				);
		if ( setStateAttack ) __SetState(ST_ATTACK);
		/*
		bool	canAttack = _group.getAttackPossibility(_zone,_addR*2);
		if ( canAttack ){
			// Attack if enemy begin building near
			if (getEnemyAmount(_NMask,_zone,0,COLLECT_BUILDINGS)) 
				__SetState(ST_ATTACK);
			// Attack if enemy attack me
			if (!_bGuardZone&&_group.getUnderAttack()&&getEnemyAmount(_NMask,_zone,_addR*2))
				__SetState(ST_ATTACK);
			// Attack enemy in guarded zone if we guard it
			if (_bGuardZone&&(_group.getUnderAttack()||getEnemyAmount(_NMask,_zone)))
				__SetState(ST_ATTACK);
			// chaeck secured group protection need
			if (_par.check("SecuredGroup")&&(_par.getGROUP("SecuredGroup")).getUnderAttack())
				__SetState(ST_ATTACK);
		};
		*/
		if ( _par.check("MarkDelay") ){
			int& _MarkDelay = _par.getINT("MarkDelay");
			if ( _MarkDelay!=-1 ){
				int _rnd = GI::random(_MarkDelay);
				bool move = _rnd==0;
				addrand(234);
				if ( move ) _group.MarkTime(_zone,20);
				/*
				int& _CurDelay = _par.getINT("CurDelay");
				_CurDelay--;
				if ( _CurDelay<=0 ){
					_CurDelay = _par.getINT("MarkDelay");
					_group.MarkTime(_zone,20);
				};
				*/
			};
		};        
		if ( _par.check("RespawnTime") ){
			int T=GameInterface::GetGlobalTime();
			int& _RespawnTime = _par.getINT("RespawnTime");
			if( T>_RespawnTime ){
				if ( _par.check("arrGrpPos") ){
					pARRAY arrGrpPos = _par.getARRAY("arrGrpPos");
					//pParamList arrParams = arrGrpPos.getParamList();
					int NP=arrGrpPos.getSIZE();
					int	NU=_group.getUN();
					if( NP>NU ){
						DynArray<int> Types;
						for(int i=0;i<NP;i++){
							iNode node=arrGrpPos.getNODE(i);
							Types.Add(node.getR());
						}
						for(int i=0;i<NU;i++){
							int Type=pUnitType(_group[i]).GetType();
							for(int t=0;t<NP;t++){
								if( Type==Types[t] ){
									Types.Del(t,1);
									break;
								}
							}
						}
						//int	n=_group.getUN();
						for(int i=NU;i<NP;i++){
							//pUnit U=_group[i];
							//if( !U.Valid() || U.GetLiveStatus()==0 ){
							iNode node=arrGrpPos.getNODE(i);
							int Type=Types[0];
							Types.Del(0,1);
							pUnit U=GameInterface::CreateUnit(node.getX(),node.getY(),7,pUnitType(Type),true,(directions_list)node.getDIR());
							_group._add(U);
							//}
						}
					}
					_RespawnTime=T+5000;
				}
			}
		}
	}else if 			( msg.str() == "MSG_EXIT" ){
		_group.setAIGuard(false);
		return 1;
	}               
	return 1;
};
/* FRO BUILDINGS */
// Wait for order - begin produse units 
int		ST_IDLE_B_00		( pSMARTOBJECT& C_OBJ, pMSG& msg ){
	if ( msg.str() == "MSG_ORDER" 	){
		pParamList	_par		= C_OBJ.getPARAMS();
		if ( (msg.getPARAMS()).Valid()==false ) END_FAIL;
		pParamList	_msg_par 	= msg.getPARAMS();
		if ( _msg_par.check("iUnitType")==false )	END_FAIL;
		if ( _msg_par.check("iUnitNUM") ==false )	END_FAIL;
		if ( _msg_par.check("iNewNation")==false && _msg_par.check("iNewOwnerNation")==false )	END_FAIL;
		int _iUT 		= _msg_par.getINT("iUnitType");
		int _iUN 		= _msg_par.getINT("iUnitNUM");
		_par.addINT("iUnitType") = _iUT;
		_par.addINT("iUnitNUM")  = _iUN;
		if ( _msg_par.check("iNewNation")==false )	_par.addINT("iNewNation") = -1;
		else										_par.addINT("iNewNation") = _msg_par.getINT("iNewNation");
		if ( _msg_par.check("iNewOwnerNation")==false )
			_par.addINT("iNewOwnerNation")	= -1;
		else
			_par.addINT("iNewOwnerNation")	= _msg_par.getINT("iNewOwnerNation");
		__SetState(ST_BEGIN_PR);
	};
	END_OK;
};

int		ST_IDLE_B_01		( pSMARTOBJECT& C_OBJ, pMSG& msg ){
	if	( msg.str() == "MSG_ORDER" 	){
		__SetState(ST_BEGIN_PR);
	};
	END_OK;
};
//////////////////////////////////////////////////////////////////////////
// ST_Produce			
//////////////////////////////////////////////////////////////////////////
// ST_BEGIN_PR - prepare for produce   
/*
	Использует параметры
	"iUnitType"	- тип производимого юнита
	"iUnitNUM"	- кол-во юнитов для производства
*/
int		ST_BEGIN_PR_00		( pSMARTOBJECT& C_OBJ, pMSG& msg ){
	pParamList	_par	= C_OBJ.getPARAMS();
	if ( msg.str() == "MSG_ENTER" ){
		if ( _par.check("iUnitType")==false || _par.check("iUnitNUM")==false )	__SetState(ST_IDLE);
		int _iUT = _par.getINT("iUnitType");
		int _iUN = _par.getINT("iUnitNUM");
		pGroup	_gBarak = C_OBJ.getOBJ_GROUP();
		_gBarak.Produce( _iUT, _iUN );
		__SetState(ST_PRODUCE);
	};
	END_OK;
};

// ST_PRODUCE - control produse state  
int		ST_PRODUCE_00		( pSMARTOBJECT& C_OBJ, pMSG& msg ){
	pGroup		_gBarak = C_OBJ.getOBJ_GROUP();
	pParamList	_par	= C_OBJ.getPARAMS();
	pGroup		_gUnits = _par.getGROUP("gGroup");
	if 		( msg.str() == "MSG_NEW_UNIT" ){
		if ( (msg.getPARAMS()).Valid()==false || (msg.getPARAMS()).check("inewGID")==false ) END_FAIL;
		int __GID = (msg.getPARAMS()).getINT("inewGID");
		_gUnits.AddUnitGID( __GID );
		_gUnits.setNMASK( _gBarak.getNMASK() );
		END_OK;
	}else if 	( msg.str() == "MSG_UPDATE" ){
		int _iUN = _par.getINT("iUnitNUM");
		if ( _gBarak.getNO_ORDERS_UN()==_gBarak.getUN() && _gUnits.getReady() ){
			__SetState(ST_END_PR);
		};
		END_OK;
	};
	END_OK;
};

// ST_END_PR - some operation on produced units 
int		ST_END_PR_00		( pSMARTOBJECT& C_OBJ, pMSG& msg ){
	pParamList	_par	= C_OBJ.getPARAMS();
	pGroup		_gUnits = _par.getGROUP("gGroup");
	if 		( msg.str() == "MSG_UPDATE" 	){
		if ( _gUnits.getReady() )	__SetState(ST_IDLE);
	}else if( msg.str() == "MSG_EXIT" 	){
		int _iNewNation = _par.getINT("iNewNation");
		int _iNewOwner  = _par.getINT("iNewOwnerNation");
		if ( _iNewNation != -1 ){
			_gUnits.setNation( _iNewNation );
		}else if ( _iNewOwner != -1 ){
			_gUnits.setNMASK( GetNationNMASK(_iNewOwner) );
			GI::AddTomahawksAttackers(_gUnits,_iNewOwner,true);
		};
		_gUnits.RemAllUnits();
		END_OK;
	};
	END_OK;
};

int		ST_END_PR_01		( pSMARTOBJECT& C_OBJ, pMSG& msg ){
	pGroup		_gBarak = C_OBJ.getOBJ_GROUP();
	pParamList	_par	= C_OBJ.getPARAMS();
	pGroup		_gUnits = _par.getGROUP("gGroup");
	if 		( msg.str() == "MSG_UPDATE" ){
		if ( _gUnits.getReady() ) __SetState(ST_IDLE);
	}else if( msg.str() == "MSG_EXIT" 	){
		int _iNewNat = _par.getINT("iVillageOwner");
		// _gUnits:SetNation(_iNewNat.Value);
		_gUnits.setNMASK( GetNationNMASK( _iNewNat ) );
		GI::AddTomahawksAttackers( _gUnits, _iNewNat, true, 0, 0, 0 );
		int _iTimer = _par.getINT("iWaitTime");
		// _iNewNat.Value = _gBarak.Nat;
		GI::SendMSG( "MSG_ORDER", _iTimer, C_OBJ, C_OBJ );
		END_OK;
	};
	END_OK;
};
//////////////////////////////////////////////////////////////////////////
// ST_Global			
//////////////////////////////////////////////////////////////////////////
/* FOR ATTACKS */
int		ST_GLOBAL_00		( pSMARTOBJECT& C_OBJ, pMSG& msg ){
	pGroup		_group	= C_OBJ.getOBJ_GROUP();
	pParamList	_par	= C_OBJ.getPARAMS();

	if 		( msg.str() == "MSG_ENTER" 	){
		// Parameter GUARDED ZONE
		if ( _par.check("GuardNode")==false ){
			iNode _zone = _par.addNODE("GuardNode");
			int   zR = 500;
			if ( _par.check("R") ) zR = _par.getINT("R");
			_zone.setX( _group.getXc() );
			_zone.setY( _group.getYc() );
			_zone.setR(zR);
		};
		if ( _par.check("iAddR")==false ) _par.addINT("iAddR") = 500;
		// Parametr bGuardZone
		if ( _par.check("bGuardZone")==false ) _par.addBOOL("bGuardZone") = false;
		// Parameter ENEMY GROUP
		if ( _par.check("EnemyGroup")==false ) _par.addGROUP("EnemyGroup");
		// MarkDelay
		int& _CurDelay = _par.addINT("CurDelay");
		if ( _par.check("MarkDelay") ) _CurDelay = _par.getINT("MarkDelay");
		// Create Array of positions 
		bool NewPos = true;
		if ( _par.check("bNewPos") ) NewPos = _par.getBOOL("bNewPos");
		pARRAY arrGrpPos;
		if ( _par.check("arrGrpPos")==false ){
			arrGrpPos = _par.addARRAY("arrGrpPos");
			if ( _par.check("bNewPos") ) _par.getBOOL("bNewPos") = true;
		}else if ( NewPos ){
			arrGrpPos = _par.getARRAY("arrGrpPos");
			arrGrpPos.CLEAR();
		};

		if ( NewPos ){
			pParamList arrParams = arrGrpPos.getParamList();
			iNode node;
			int		UN = _group.getUN();
			for (int i=0; i<UN; i++){
				node = arrParams.addNODE( _str(i).pchar() );
				pUnit U=_group[i];
				node.setX	( _group.getXi(i) );
				node.setY	( _group.getYi(i) );
				int Type=pUnitType(_group[i]).GetType();
				node.setR	( Type );
				node.setDIR ( _group.getDIRi(i) );
			};
		};

		//pARRAY aTypePos;
		//if ( _par.check("aTypePos")==false ){
		//	aTypePos = _par.addARRAY("arrGrpPos");
		//}else{
		//	aTypePos = _par.getARRAY("arrGrpPos");
		//}
		//pParamList apTypePos = aTypePos.getParamList();
		//int		UN = _group.getUN();
		//for (int i=0; i<UN; i++){
		//	apTypePos.addINT()
		//}

		if ( _par.check("bNewPos") ) _par.getBOOL("bNewPos") = false;

		// перенесено в ACF_GuardZone00_local
		// время респаунда убитых воинов
		//if ( !_par.check("RespawnDelay") ) _par.addINT("RespawnDelay") = 15000;

		// GO in next state
		__SetState(ST_IDLE);
	};

	return 1;
};
/* FOR BUILDINGS */
// ST_GLOBAL - for prepare to produce units 
/*
	Создает локальную группу							- "gGroup"
	Создает локальную переменную для владельца			- "iVillageOwner"
	Выставляет точку сбора для бараков если есть нода	- "nRallyPoint"
*/
int		ST_GLOBAL_B_00		( pSMARTOBJECT& C_OBJ, pMSG& msg ){
	pGroup		_gBarak = C_OBJ.getOBJ_GROUP();
	pParamList	_par	= C_OBJ.getPARAMS();
	if 	( msg.str() == "MSG_ENTER" 	){
		// Group for store prodused units 
		if ( _par.check("gGroup")==false ){
			_par.addGROUP("gGroup");
		};
		// Set Rally Point
		if ( _par.check("nRallyPoint") )			_gBarak.SetRallyPoint( _par.getNODE("nRallyPoint") );
		// Create and set owner 
		if ( _par.check("iVillageOwner")==false )	_par.addINT("iVillageOwner") = _gBarak.getNation();
		else										_par.getINT("iVillageOwner") = _gBarak.getNation();
		// Set mask for trader 
		if ( _par.check("gTrader") )				(_par.getGROUP("gTrader")).setNMASK(255);
		// Set NMASK - freandly for all 
		_gBarak.setNMASK(255);
		// GO in next state 
		__SetState(ST_IDLE);
	};
	END_FAIL;
};

//////////////////////////////////////////////////////////////////////////
// ST_SomeOrder			
//////////////////////////////////////////////////////////////////////////
/*
	Обрабатывает приказ на покупку села.
	Использует параметры :
		"iVillageOwner"	- текущий владелец
		"iResType"		- тип ресурса
		"iPrice"		- цена продажи
	Сообщение должно содержать :
		"iNewOwner"		- новый владелец
*/
int		ST_ORDER_00			( pSMARTOBJECT& C_OBJ, pMSG& msg ){
	pGroup		_gBarak = C_OBJ.getOBJ_GROUP();
	pParamList	_par	= C_OBJ.getPARAMS();
	if ( msg.str() == "MSG_SET_OWNER" ){
		int& _iOwner = _par.getINT("iVillageOwner");
		if ( _iOwner == _gBarak.getNation() ){
			if ( (msg.getPARAMS()).check("iNewOwner") ) _iOwner = (msg.getPARAMS()).getINT("iNewOwner");
			else										_iOwner = GI::GetMsgSenderNation(msg.getSender());
			if ( _iOwner!=-1 && _par.check("iResType") && _par.check("iPrice") ){
				int _iResType  = _par.getINT("iResType");
				int _iPrice    = _par.getINT("iPrice");
				if ( GI::GetResource(_iOwner,_iResType) >= _iPrice ){
					if ( GI::AddRessource( _iOwner, _iResType, -_iPrice )==false )  END_OK;
					_gBarak.setNMASK( GetNationNMASK( _iOwner ) ); 
					if ( _par.check("gTrader") ) (_par.getGROUP("gTrader")).setNMASK( _gBarak.getNMASK() );
					GI::SendMSG( "MSG_ORDER", C_OBJ, C_OBJ );
				}else	_iOwner = _gBarak.getNation();
			};
		};
		END_OK;
	};
	END_OK;
};

//////////////////////////////////////////////////////////////////////////
// ST_Move				--
//////////////////////////////////////////////////////////////////////////
int		ST_MOVE_00( pSMARTOBJECT& C_OBJ, pMSG& msg, char* strX, char* strY, char* strNextSTATE ){
	pGroup		_group	= C_OBJ.getOBJ_GROUP();
	pParamList	_par	= C_OBJ.getPARAMS();
	if 		( msg.str() == "MSG_ENTER" 		){
		// Не обрабатываем пустую группу
		if ( _group.Valid()==false || _group.getUN()<=0 ){
			C_OBJ.next_STATE(strNextSTATE);
			C_OBJ.FSC(true);
			END_OK;
		};
		// Страховка: не успели сдвинуться с места 
		if ( _par.check("l_itick") ) _par.getINT("l_itick") = 3;
		else						 _par.addINT("l_itick") = 3;
		// Готовим объект к использованию 
		_group.setSearchEnemy(false);
		GI::SendMSG( "MSG_SEND", C_OBJ, C_OBJ );
	}else if( msg.str() == "MSG_SEND" 	){
		// Получаем позиция следования (x,y) 
		if ( _par.check(strX)==false || _par.check(strY)==false ){
			C_OBJ.next_STATE(strNextSTATE);
			C_OBJ.FSC(true);
			END_OK;
		};
		int l_x = _par.getINT(strX);
		int l_y = _par.getINT(strY);
		// Сохраняем позицию текущего движения 
		if ( _par.check("l_LocalDestNode")==false ) _par.addNODE("l_LocalDestNode");
		iNode	l_DestNode = _par.getNODE("l_LocalDestNode");
		// Посылам группу к позиции (x,y) 
		_group.SendSmartly(l_x, l_y, 5, l_DestNode);
	}else if( msg.str() == "MSG_UPDATE" 	){
		// Системная обработка приказа идти 
		int& l_itick = _par.getINT("l_itick");
		// Получаем локальные следования (x,y) 
		iNode	l_DestNode	= _par.getNODE("l_LocalDestNode");
		int		l_x			= _par.getINT(strX);
		int		l_y			= _par.getINT(strY);
		// Нормально выходим по достижении нужной позиции 
		if ( GI::NORMA(_group.getXc(),l_x*64+32,_group.getYc(),l_y*64+32)<=500 ){
			C_OBJ.next_STATE(strNextSTATE);
			C_OBJ.FSC(true);
			END_OK;
		};
		// Нужно отдать приказ заново если кто-то стоит без дела 
		if ( _group.getNO_ORDERS_UN() && l_itick==0 ){
			if ( GI::NORMA(_group.getXc(),l_DestNode.getX(),_group.getYc(),l_DestNode.getY())<=200 ){
				GI::SendMSG( "MSG_SEND", C_OBJ, C_OBJ );
				END_OK;
			};
		};
		if ( l_itick>0 ) l_itick -= 1;
		// Аварийно выходим если нас атаковали 
		int EnemyUnitsAroundNumS = _group.getEnemyAround_UN( 400 + (_group.getUN()/50)*50 );
		if ( _group.getUnderAttack() || EnemyUnitsAroundNumS ){
			if ( _group.getAttackPossibility(_group.getXc(),_group.getYc(),600+(_group.getUN()/50)*100) ){
				C_OBJ.next_STATE(strNextSTATE);
				C_OBJ.FSC(true);
				END_OK;
			};
		};
		// Аварийно выходим смена позиции следования 
		int AID = _par.getINT("AREAID");
		if ( GI::GetAreaX(AID,0)!=l_x || GI::GetAreaY(AID,0)!=l_y ){
			C_OBJ.next_STATE(strNextSTATE);
			C_OBJ.FSC(true);
			END_OK;
		};
	}else if( msg.str() == "MSG_EXIT" 	){
		if ( _group.Valid() && _group.getUN() )	 _group.setSearchEnemy(true);
	};
	END_OK;
};
//////////////////////////////////////////////////////////////////////////
// ST_SABOTEUR_STEP
//////////////////////////////////////////////////////////////////////////
/*
Description:
	- Решаем что нам делать используя strAREAID как параметр для следования
	  в нужную позицию. Выбор простой - идти, атаковать случайных встреченных,
	  по прибытии уничтожить всеж.
*/
int		ST_SABOTEUR_STEP_00( pSMARTOBJECT& C_OBJ, pMSG& msg, char* strAREAID, char* strMOVE, char* strATTACK, char* strTERMINATE ){
	pGroup		_group 	= C_OBJ.getOBJ_GROUP();
	pParamList	_par	= C_OBJ.getPARAMS();
	
	if 		( msg.str() == "MSG_ENTER"  ){
		// Страховка на слечай безделия 
		if ( _par.check("l_itick")==false ) _par.addINT("l_itick");
		_par.getINT("l_itick") = 10;
		// Решаем что делать 
		_str l_newOrder = "Empty";
		int l_x = -1;
		int l_y = -1;
		if ( _group.getUnderAttack() ){
			l_newOrder = "ATTACK";
		}else{
			if ( _par.check(strAREAID) ){
				int& AID = _par.getINT(strAREAID);
				if ( AID!=-1 ){
					// Есть точка следования 
					l_x = GI::GetAreaX( AID, 0);
					l_y = GI::GetAreaY( AID, 0);
					if ( l_x!=-1 && l_y!=-1 ){
						int EnemyUnitsAroundNum  = _group.getEnemyAround_UN( 1000 + (_group.getUN()/50)*100 );
						int EnemyUnitsAroundNumS = _group.getEnemyAround_UN( 400 + (_group.getUN()/50)*50 );
						int destLastD = GI::NORMA(_group.getXc(),l_x*64+32,_group.getYc(),l_y*64+32);
						if ( ( destLastD<=600 && EnemyUnitsAroundNum ) ){
							l_newOrder = "TERMINATE";
						}else if( EnemyUnitsAroundNumS || _group.getUnderAttack() ){
							l_newOrder = "ATTACK";
						}else if( _group.getUnderAttack()==false && destLastD>600 ){
							l_newOrder = "MOVE";
						}else if( destLastD<=600 && EnemyUnitsAroundNum==0 ){
							AID = -1;
						};
					};
				};
			};
		};
		// Отдаем приказ к действию 
		if ( l_newOrder=="MOVE" || l_newOrder=="TERMINATE" ){
			// Расчитать новую точку следования 
            if ( _par.check("l_x")==false ) _par.addINT("l_x");
			_par.getINT("l_x") = l_x;
			if ( _par.check("l_y")==false ) _par.addINT("l_y");
			_par.getINT("l_y") = l_y;
		};
		if 		( l_newOrder == "MOVE" ){
			C_OBJ.next_STATE(strMOVE);
			C_OBJ.FSC(true);
		}else if( l_newOrder == "ATTACK" ){
			if ( _group.getAttackPossibility(_group.getXc(),_group.getYc(),600+(_group.getUN()/50)*100) ){
				C_OBJ.next_STATE(strATTACK);
				C_OBJ.FSC(true);
			};
		}else if( l_newOrder == "TERMINATE" ){
			if ( _group.getAttackPossibility(_group.getXc(),_group.getYc(),1500) ){
				if ( _par.check("l_R")==false ){
					_par.addINT("l_R") = 1500;
				};
				C_OBJ.next_STATE(strTERMINATE);
				C_OBJ.FSC(true);
			};
		};
		if ( l_newOrder=="Empty" && _par.getINT(strAREAID)==-1 ){
			// try global state
			C_OBJ.next_STATE("ST_GLOBAL");
			C_OBJ.FSC(true);
		}
	}else if( msg.str() == "MSG_UPDATE" ){
		// Нечего делать, просто ждем чегото 
		int& l_itick = _par.getINT("l_itick");
		if ( l_itick==0 )	GI::SendMSG( "MSG_ENTER", C_OBJ, C_OBJ );
		else				l_itick -= 1;
		// Что-то произошло -> действуем 
		int& AID = _par.getINT(strAREAID);
		if ( _group.getUnderAttack() || AID!=-1 )	GI::SendMSG( "MSG_ENTER", C_OBJ, C_OBJ );
	}else if( msg.str() == "MSG_EXIT"   ){
		// Ничего не нужно, все сделанно 
	};
	END_OK;
};
//////////////////////////////////////////////////////////////////////////































