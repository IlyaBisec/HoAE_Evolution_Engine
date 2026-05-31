#ifndef __FN_HELPER__
#define __FN_HELPER__

int l_SendGroupByWay(pGroup& GR,pARRAY& AR);

//////////////////////////////////////////////////////////////////////////
// HELPER_FSM			--
//////////////////////////////////////////////////////////////////////////
bool	SO_GROUP_IS_DEAD( pSMARTOBJECT& C_OBJ );
int		getEnemyAmount(int NMask,int x, int y, int r, int addR=0,collect_option Flags=COLLECT_UNITS);
int		getEnemyAmount(int NMask,iNode& ND,int addR=0,collect_option Flags=COLLECT_UNITS);
int		GroupCheckPosArray( pGroup& _group, pARRAY& _arrPos );
bool	ProcMsgFree( pMSG& msg, bool curSTATE );
int		GetNationNMASK( int Nat );
int		Get_arrTN_UnitNumberSum( pARRAY& arrTN );
iNode	GetSafeNODE( pARRAY& arrNode, int id );
int		GetNearestNodeID( pARRAY& arrNode, int x, int y, bool cirkle=false );
int		LinkNodeToGroup00( iNode& Node, pGroup& Group, iNode& Node10, iNode& Node00, iNode& Node01 );
bool	FINDE_USE_AST_RATE( pSMARTOBJECT& C_OBJ, char* pszAreaID );
int 	FindeEnemyAreaID( pGroup& pGRP, int iPower );
int		AL_AST_RATE( be_CTotalALPD& ENEMY_T_ALPD, int iPrivMinRate );

#endif//__FN_HELPER__

//////////////////////////////////////////////////////////////////////////

















