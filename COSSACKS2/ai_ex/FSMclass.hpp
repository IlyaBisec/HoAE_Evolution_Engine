//////////////////////////////////////////////////////////////////////////
//
// Victor Levchenko, 21.02.2005
//
// FSMstate.cpp: implementation of the FSMclass classes.
//
//////////////////////////////////////////////////////////////////////////

#include "../stdheader.h"
#include "FSMstate.h"
#include "FSMclass.h"

//************************************************************************
// Base class for diferent type of an individual FSM machine
//************************************************************************
//////////////////////////////////////////////////////////////////////////
// Construction
//////////////////////////////////////////////////////////////////////////
template< class T_FSMID, class T_STATEID, class T_FSMstate,	class T_PARA, class T_STORE, class T_BASE_CLASS >
FSMclass_base<T_FSMID, T_STATEID,T_FSMstate,T_PARA,T_STORE,T_BASE_CLASS>::FSMclass_base( T_FSMID typeFSM_ID ){
	SetFSM_TypeID(typeFSM_ID);
};
//////////////////////////////////////////////////////////////////////////
// Destruction
//////////////////////////////////////////////////////////////////////////
template< class T_FSMID, class T_STATEID, class T_FSMstate,	class T_PARA, class T_STORE, class T_BASE_CLASS >
FSMclass_base<T_FSMID, T_STATEID,T_FSMstate,T_PARA,T_STORE,T_BASE_CLASS>::~FSMclass_base	(){
	T_PARA* pPara = NULL;
	int NSM = m_map.GetAmount();
	for ( int i=0; i<NSM; ++i ){
		pPara = m_map[i];
		SAFEDELETE(pPara);
		m_map[i] = NULL;
	};

	m_map.Clear();

	SetFSM_TypeID(UNIVERSAL_EMPTY_ID);
};
//////////////////////////////////////////////////////////////////////////
// get FSM type ID
//////////////////////////////////////////////////////////////////////////
template< class T_FSMID, class T_STATEID, class T_FSMstate,	class T_PARA, class T_STORE, class T_BASE_CLASS >
T_FSMID		FSMclass_base<T_FSMID, T_STATEID,T_FSMstate,T_PARA,T_STORE,T_BASE_CLASS>::GetFSM_TypeID(){
	return m_TYPE_UID;
};
//////////////////////////////////////////////////////////////////////////
// set FSM type ID
//////////////////////////////////////////////////////////////////////////
template< class T_FSMID, class T_STATEID, class T_FSMstate,	class T_PARA, class T_STORE, class T_BASE_CLASS >
void		FSMclass_base<T_FSMID, T_STATEID,T_FSMstate,T_PARA,T_STORE,T_BASE_CLASS>::SetFSM_TypeID( T_FSMID FSMtypeID ){
	m_TYPE_UID = FSMtypeID;
};
//////////////////////////////////////////////////////////////////////////
// return the FSMstate object pointer
//////////////////////////////////////////////////////////////////////////
template< class T_FSMID, class T_STATEID, class T_FSMstate,	class T_PARA, class T_STORE, class T_BASE_CLASS >
T_FSMstate*	FSMclass_base<T_FSMID, T_STATEID,T_FSMstate,T_PARA,T_STORE,T_BASE_CLASS>::GetState( T_STATEID StateID ){
	T_FSMstate* pState = NULL;
	int NFSM_State = m_StateStore.GetAmount();

	// try to find this FSMstate in the map
	while ( NFSM_State-- && pState==NULL ) {
		if ( m_StateStore[NFSM_State]->m_first == StateID ){
			pState = &(m_StateStore[NFSM_State]->m_second);
		};
	};

	return pState;
};
//////////////////////////////////////////////////////////////////////////
// add a FSMstate object pointer to the map
//////////////////////////////////////////////////////////////////////////
template<class T_FSMID, class T_STATEID, class T_FSMstate,	class T_PARA, class T_STORE, class T_BASE_CLASS >
void		FSMclass_base<T_FSMID, T_STATEID,T_FSMstate,T_PARA,T_STORE,T_BASE_CLASS>::AddState( T_FSMstate*	pState ){
	if ( pState == NULL ) return;

	T_FSMstate *pState = GetState(pState->GetState());

	// if the FSMstate object pointer is already in the map, return
	if( pState != NULL )	return;

	// otherwise put the FSMstate object pointer into the map
	m_StateStore.Add( new T_PARA( pState->GetState(), *(pState) ) );
};	
//////////////////////////////////////////////////////////////////////////
// delete a FSMstate object pointer from the map
//////////////////////////////////////////////////////////////////////////
template< class T_FSMID, class T_STATEID, class T_FSMstate,	class T_PARA, class T_STORE, class T_BASE_CLASS >
void		FSMclass_base<T_FSMID, T_STATEID,T_FSMstate,T_PARA,T_STORE,T_BASE_CLASS>::DeleteState( T_STATEID StateID ){
	T_FSMstate *pState = NULL;
	
	int NFSM_State = m_StateStore.GetAmount();
	while ( NFSM_State-- ) {
		if ( m_StateStore[NFSM_State]->m_first == StateID ){
			pState = &(m_StateStore[NFSM_State]->m_second);
			SAFEDELETE(pState);
			m_StateStore.DelElement( NFSM_State );
			return;
		};
	};
};	
//************************************************************************


//************************************************************************
// FSM class 
//************************************************************************
//////////////////////////////////////////////////////////////////////////
// Construction
//////////////////////////////////////////////////////////////////////////
template< class T_FSMID, class T_STATEID, class T_INPUT, class T_FSMstate >
FSMclass<T_FSMID, T_STATEID,T_INPUT,T_FSMstate >::FSMclass( T_FSMID typeFSM_ID, T_STATEID StateID ) : FSMclass_base(typeFSM_ID) {
	SetCurrentState(StateID);
};
//////////////////////////////////////////////////////////////////////////
// Destruction
//////////////////////////////////////////////////////////////////////////
template< class T_FSMID, class T_STATEID, class T_INPUT, class T_FSMstate >
FSMclass<T_FSMID, T_STATEID,T_INPUT,T_FSMstate >::~FSMclass(){
	SetCurrentState(UNIVERSAL_EMPTY_ID);
};
//////////////////////////////////////////////////////////////////////////
// return the current state ID
//////////////////////////////////////////////////////////////////////////
template< class T_FSMID, class T_STATEID, class T_INPUT, class T_FSMstate >
T_STATEID	FSMclass<T_FSMID, T_STATEID,T_INPUT,T_FSMstate >::GetCurrentState(){
	return m_CurrentState;
};
//////////////////////////////////////////////////////////////////////////
// set current state
//////////////////////////////////////////////////////////////////////////
template< class T_FSMID, class T_STATEID, class T_INPUT, class T_FSMstate >
void		FSMclass<T_FSMID, T_STATEID,T_INPUT,T_FSMstate >::SetCurrentState( T_STATEID StateID ){
	m_CurrentState = StateID;
};
//////////////////////////////////////////////////////////////////////////
// perform a state transition based on input and current state
//////////////////////////////////////////////////////////////////////////
template< class T_FSMID, class T_STATEID, class T_INPUT, class T_FSMstate >
T_STATEID	FSMclass<T_FSMID, T_STATEID,T_INPUT,T_FSMstate >::StateTransition( T_INPUT Input ){
	return UNIVERSAL_EMPTY_ID;
};			
//************************************************************************


























