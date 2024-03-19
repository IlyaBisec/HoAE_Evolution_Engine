// BE MACROSES

/*
 *	Define function return type for export it in "Script Editor"
 */
#define EDITOR_FUNCTION __declspec(dllexport) int 

/*
 *	Define return state for export "Script Editor" functions
 */
#define END_OK return 1;
#define END_FAIL return 0;

/*
 *	Define params for export "Script Editor" functions
 */
#define	__PARAMS__ pParamList* pPL

/*
 *	Define some useful macros for check export "Script Editor" and FSM functions param
 */
#define	CHECKP(Name) if(pPL==NULL||pPL->Valid()==false||pPL->check(#Name)==false)return 0;
#define	FSM_CHECKP() if(C_OBJ.Valid()==false||msg.Valid()==false||state.pchar()==NULL||(state.pchar())[0]==0)return 0;

/*
 *	Define some useful macros for wrap receiving(convert) params from << __PARAMS__ >> macros
 *  for use in function
 */
#define __GROUP(Name) CHECKP(Name);pGroup Name=pPL->getGROUP(#Name);
#define __NODE(Name) CHECKP(Name);iNode Name=pPL->getNODE(#Name);
#define __ROUND_ZONE(Name) CHECKP(Name);RoundZone Name=RoundZone(pPL->getNODE(#Name));
#define __SQUARE_ZONE(Name) CHECKP(Name);SquareZone Name=SquareZone(pPL->getNODE(#Name));
#define __BOOL(Name) CHECKP(Name);bool& Name=pPL->getBOOL(#Name);
#define __FLOAT(Name) CHECKP(Name);float& Name=pPL->getFLOAT(#Name);
#define __STR(Name) CHECKP(Name);_str& Name=pPL->getSTR(#Name);
#define __INT(Name) CHECKP(Name);int& Name=pPL->getINT(#Name);
#define __DWORD(Name) CHECKP(Name);DWORD& Name=pPL->getDWORD(#Name);
#define __ARRAY(Name) CHECKP(Name);pARRAY Name=pPL->getARRAY(#Name);
#define __ABILITY(Name) CHECKP(Name);pABILITY Name=pPL->getABILILTY(#Name);
#define __WEAPONMOD(Name) CHECKP(Name);pWEAPONMOD Name=pPL->getWEAPONMOD(#Name);
#define __UNIT_TYPE(Name) CHECKP(Name);pUnitType Name=pUnitType(pPL->getINT(#Name));
#define	__CAMERA(Name) CHECKP(Name);pCAMERA Name=pCAMERA(pPL->getCAMERA(#Name));

// FSM MACROS (FSM)
/*
 *	Define useful macros for chenge FSM state for current SO(Smart Object)
 */
#define	__SetState(Name) {C_OBJ.next_STATE(#Name);C_OBJ.FSC(true);END_OK;};

#ifdef	FSM_MACROS
/*
#define BeginStateMachine   if( STATE_Global == state ) { if(0) {
#define State(a)            return( true ); } } else if( a == state ) { if(0) {
#define OnEnter             return( true ); } else if( MSG_RESERVED_Enter == msg->name ) { 
#define OnExit              return( true ); } else if( MSG_RESERVED_Exit == msg->name ) { 
#define OnUpdate            return( true ); } else if( MSG_RESERVED_Update == msg->name ) { 
#define OnMsg(a)            return( true ); } else if( a == msg->name ) {
#define SetState(a)         go->next_state = a; go->force_state_change = true;
#define EndStateMachine     return( true ); } } else { assert( !"Invalid State" ); \
	                        return( false );}  return( false );
*/
#endif//FSM_MACROS

