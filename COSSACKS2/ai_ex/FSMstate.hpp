//////////////////////////////////////////////////////////////////////////
//
// Victor Levchenko, 15.02.2005
//
// FSMstate.cpp: implementation of the FSMstate,FuSMstate classes.
//
//////////////////////////////////////////////////////////////////////////

#include "../stdheader.h"
#include "FSMstate.h"

//************************************************************************
// Base class for diferent type of an individual state
//************************************************************************
//////////////////////////////////////////////////////////////////////////
// Construction
//////////////////////////////////////////////////////////////////////////
template< class T_STATEID >
FSMstate_base<T_STATEID>::FSMstate_base	(){
	m_StateID = UNIVERSAL_EMPTY_ID;
};

template< class T_STATEID >
FSMstate_base<T_STATEID>::FSMstate_base( const T_STATEID& stateID ){
	m_StateID = stateID;
};

//////////////////////////////////////////////////////////////////////////
// Destruction
//////////////////////////////////////////////////////////////////////////
template< class T_STATEID >
FSMstate_base<T_STATEID>::~FSMstate_base	(){
};
//////////////////////////////////////////////////////////////////////////
// access the state ID
//////////////////////////////////////////////////////////////////////////
template< class T_STATEID >
T_STATEID	FSMstate_base<T_STATEID>::GetState(){
	return	m_StateID;
};
//************************************************************************

//************************************************************************
// This is the definition of an individual state, its inputs and 
// transition arrays
//************************************************************************
//////////////////////////////////////////////////////////////////////
// FSMstate() - create a new instance and allocate arrays
//////////////////////////////////////////////////////////////////////
template< class T_STATEID, class T_INPUT, class T_PARA,	class T_STORE, class T_BASE_STATE >
FSMstate< T_STATEID, T_INPUT, T_PARA, T_STORE, T_BASE_STATE >::FSMstate(){
	m_StateID = UNIVERSAL_EMPTY_ID;
};
template< class T_STATEID, class T_INPUT, class T_PARA,	class T_STORE, class T_BASE_STATE >
FSMstate< T_STATEID, T_INPUT, T_PARA, T_STORE, T_BASE_STATE >::FSMstate( const T_STATEID& StateID, const int usTransitions ) : T_BASE_STATE( StateID )
{
	CreateNew( usTransitions );
};
template< class T_STATEID, class T_INPUT, class T_PARA,	class T_STORE, class T_BASE_STATE >
FSMstate< T_STATEID, T_INPUT, T_PARA, T_STORE, T_BASE_STATE >::FSMstate	( const FSMstate& state ) : T_BASE_STATE( state.GetState() ){
	CreateNew( state.GetTransitionNUM() );
	for ( int i=0; i<GetTransitionNUM(); i++ ){
		AddTransition( state.m_Input_vs_OutputState[i]->m_first, state.m_Input_vs_OutputState[i]->m_second );
	};
};
//////////////////////////////////////////////////////////////////////
// ~FSMstate() - clean up by returning memory used for arrays
//////////////////////////////////////////////////////////////////////
template< class T_STATEID, class T_INPUT, class T_PARA,	class T_STORE, class T_BASE_STATE >
FSMstate< T_STATEID, T_INPUT, T_PARA, T_STORE, T_BASE_STATE >::~FSMstate(){
	T_PARA* pPara = NULL;
	for ( int i=0; i<GetTransitionNUM(); ++i ){
		pPara = m_Input_vs_OutputState[i];
		SAFEDELETE(pPara);
		m_Input_vs_OutputState[i] = NULL;
	};

	m_Input_vs_OutputState.Clear();
};
//////////////////////////////////////////////////////////////////////////
// delete old and create new transition list
//////////////////////////////////////////////////////////////////////////
template< class T_STATEID, class T_INPUT, class T_PARA,	class T_STORE, class T_BASE_STATE >
void	FSMstate< T_STATEID, T_INPUT, T_PARA, T_STORE, T_BASE_STATE >::CreateNew( const int usTransitions ){
	// now allocate each array and fill it with empty state
	T_PARA*		pEmptyPara		= NULL;
	T_INPUT*	pEmptyInput		= new T_INPUT	(UNIVERSAL_EMPTY_ID);
	T_STATEID*	pEmptyOutputID	= new T_STATEID	(UNIVERSAL_EMPTY_ID);
	for ( int i=0; i<usTransitions; ++i ){
		pEmptyPara		= new T_PARA( *pEmptyInput, *pEmptyOutputID );
		m_Input_vs_OutputState.Add( pEmptyPara );
		pEmptyPara		= NULL;
	};
	SAFEDELETE(pEmptyInput);
	SAFEDELETE(pEmptyOutputID);
};
//////////////////////////////////////////////////////////////////////
// AddTransition() - accept an input transition threshhold and the
// output state ID to associate with it
//////////////////////////////////////////////////////////////////////
template< class T_STATEID, class T_INPUT, class T_PARA,	class T_STORE, class T_BASE_STATE >
void	FSMstate< T_STATEID, T_INPUT, T_PARA, T_STORE, T_BASE_STATE >::AddTransition( const T_INPUT& Input, const T_STATEID& OutputStateID, bool overWrite=true ){

	// Finde if transition already present
	for( int i=0; i<GetTransitionNUM(); ++i ){
		if ( m_Input_vs_OutputState[i]->m_first == Input )	break;
	};
	
	// overwrite presetn id if need
	if( i < GetTransitionNUM() && overWrite ){
		m_Input_vs_OutputState[i]->m_second = OutputStateID;
		m_Input_vs_OutputState[i]->m_first  = Input;
	}
	else if ( i >= GetTransitionNUM() ) {
		m_Input_vs_OutputState.Add( ( new T_PARA( Input, OutputStateID ) ) );
	};

};
//////////////////////////////////////////////////////////////////////
// DeleteTransition() - remove an output state ID and its associated
// input transition value from the arrays and zero out the slot used
//////////////////////////////////////////////////////////////////////
template< class T_STATEID, class T_INPUT, class T_PARA,	class T_STORE, class T_BASE_STATE >
void	FSMstate< T_STATEID, T_INPUT, T_PARA, T_STORE, T_BASE_STATE >::DeleteTransition( const T_STATEID& OutputStateID ){
	
	int TN = GetTransitionNUM();

	while ( TN-- ) {
		if ( m_Input_vs_OutputState[TN]->m_second == OutputStateID ){
			SAFEDELETE( m_Input_vs_OutputState[TN] );
			m_Input_vs_OutputState.DelElement(TN);
		};
	};
};
//////////////////////////////////////////////////////////////////////
// GetOutput() - accepts an input transition value and finds the 
// input transition value stored in m_piInputs[] that is associated
// with an output state ID and returns that output state ID
//
// NOTE: this function acts as a state transition function and could
// be replaced with a different transition approach depending on the
// needs of your FSM
//////////////////////////////////////////////////////////////////////
template< class T_STATEID, class T_INPUT, class T_PARA,	class T_STORE, class T_BASE_STATE >
T_STATEID	FSMstate< T_STATEID, T_INPUT, T_PARA, T_STORE, T_BASE_STATE >::GetOutput( const T_INPUT& Input ){
	T_STATEID OutputID = m_StateID;	// output state to be returned

	// for each possible transistion
	for( int i=0; i<GetTransitionNUM(); ++i ){
		// state transition function: look for a match with the input value
		if( Input == m_Input_vs_OutputState[i]->m_first ){
			OutputID = m_Input_vs_OutputState[i]->m_second;	// output state id
			break;
		};
	};
	// returning either this m_iStateID to indicate no output 
	// state was matched by the input (ie. no state transition
	// can occur) or the transitioned output state ID
	return OutputID;	
};
//////////////////////////////////////////////////////////////////////////
// get n of transition for current state
//////////////////////////////////////////////////////////////////////////
template< class T_STATEID, class T_INPUT, class T_PARA,	class T_STORE, class T_BASE_STATE >
int		FSMstate< T_STATEID, T_INPUT, T_PARA, T_STORE, T_BASE_STATE >::GetTransitionNUM(){
	return m_Input_vs_OutputState.GetAmount();
};
//************************************************************************

//************************************************************************
// This is the definition of an individual fuzzy state
//************************************************************************
/*
FuSMstate::FuSMstate						( int iStateID, int iLowRange, int iHighRange ){
};
FuSMstate::~FuSMstate						(){
};
void	FuSMstate::GetMembershipRanges		( int& iLow, int& iHigh ){
};
int		FuSMstate::GetDegreeOfMembership	(){
	return 0;
};
int		FuSMstate::GetValueOfMembership		(){
	return 0;
};
bool	FuSMstate::DoTransition				( int iInputValue ){
	return false;
};
*/
//************************************************************************































































