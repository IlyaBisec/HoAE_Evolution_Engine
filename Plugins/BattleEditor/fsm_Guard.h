#ifndef __FSM_GUARD__
#define __FSM_GUARD__

void	DeclareteDLL_fsm_Guard();

// GUARD
EDITOR_FUNCTION	FSM_GuardZone_00	( pSMARTOBJECT& C_OBJ, _str& state, pMSG& msg );
EDITOR_FUNCTION	ACF_GuardZone00_local( pGroup& _gGroup, pParamList& paramData );
EDITOR_FUNCTION	ACF_GuardZone00_arr			(__PARAMS__);
EDITOR_FUNCTION	ACF_GuardZone00				(__PARAMS__);
EDITOR_FUNCTION	ACF_GuardZone01				(__PARAMS__);
EDITOR_FUNCTION	ACF_GuardZone02				(__PARAMS__);

// AMBUSH
EDITOR_FUNCTION	FSM_AmbushZone_00	( pSMARTOBJECT& C_OBJ, _str& state, pMSG& msg );
EDITOR_FUNCTION	ACF_AmbushZone00			(__PARAMS__);
EDITOR_FUNCTION	FSM_AmbushZone_01	( pSMARTOBJECT& C_OBJ, _str& state, pMSG& msg );
EDITOR_FUNCTION	ACF_AmbushMultyZone			(__PARAMS__);

// PATROL
EDITOR_FUNCTION	FSM_PatrolZone_00	( pSMARTOBJECT& C_OBJ, _str& state, pMSG& msg );
EDITOR_FUNCTION	ACF_PatrolZone00			(__PARAMS__);

#endif//__FSM_GUARD__

//////////////////////////////////////////////////////////////////////////































