#ifndef __FSM_STATES__
#define __FSM_STATES__

int		ST_ATTACK_00		( pSMARTOBJECT& C_OBJ, pMSG& msg );
int		ST_ATTACK_00_01		( 
							  pSMARTOBJECT& C_OBJ, pMSG& msg, 
							  char* strX, char* strY, char* strR, 
							  char* strAREAID,
							  char* strNextSTATE 
							);
int		ST_ATTACK_01		( pSMARTOBJECT& C_OBJ, pMSG& msg, char* strNextSTATE );
int		ST_BACK_00			( pSMARTOBJECT& C_OBJ, pMSG& msg );
int		ST_IDLE_00			( pSMARTOBJECT& C_OBJ, pMSG& msg );
int		ST_IDLE_B_00		( pSMARTOBJECT& C_OBJ, pMSG& msg );
int		ST_IDLE_B_01		( pSMARTOBJECT& C_OBJ, pMSG& msg );
int		ST_BEGIN_PR_00		( pSMARTOBJECT& C_OBJ, pMSG& msg );
int		ST_PRODUCE_00		( pSMARTOBJECT& C_OBJ, pMSG& msg );
int		ST_END_PR_00		( pSMARTOBJECT& C_OBJ, pMSG& msg );
int		ST_END_PR_01		( pSMARTOBJECT& C_OBJ, pMSG& msg );
int		ST_GLOBAL_00		( pSMARTOBJECT& C_OBJ, pMSG& msg );
int		ST_GLOBAL_B_00		( pSMARTOBJECT& C_OBJ, pMSG& msg );
int		ST_ORDER_00			( pSMARTOBJECT& C_OBJ, pMSG& msg );
int		ST_MOVE_00			( pSMARTOBJECT& C_OBJ, pMSG& msg, char* strX, char* strY, char* strNextSTATE );
int		ST_SABOTEUR_STEP_00	( 
							  pSMARTOBJECT& C_OBJ, pMSG& msg, 
							  char* strAREAID, 
							  char* strMOVE, 
							  char* strATTACK, 
							  char* strTERMINATE 
							);

#endif//__FSM_STATES__
//////////////////////////////////////////////////////////////////////////































