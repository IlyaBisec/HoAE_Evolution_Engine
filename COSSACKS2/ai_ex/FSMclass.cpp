#include "stdheader.h"
#include "../BE_HEADERS.h"

be_FSMclass::be_FSMclass(){
	m_strCurrentState = "STATE_NULL";	
};
be_FSMclass::be_FSMclass( char* pstrStateName ){
	m_strCurrentState = pstrStateName;
};
be_FSMclass::~be_FSMclass(){
	m_StateArray.Clear();
};
be_FSMstate*	be_FSMclass::GetState( const char* pstrStateName ){
	int _id = FindID(m_StateArray,pstrStateName);
	if (_id!=-1)	return m_StateArray[_id];
	return NULL;
};
void			be_FSMclass::AddState( be_FSMstate* pState ){
	int _id = FindID(m_StateArray,pState->m_strStateName.pchar());
	if (_id==-1) m_StateArray.Add(pState);
};
void			be_FSMclass::DeleteState( const char* pstrStateName ){
	int _id = FindID(m_StateArray,pstrStateName);
	if (_id!=-1) m_StateArray.DelElement(_id);
};
char*			be_FSMclass::StateTransition( const char* pstrInput ){
	// the current state of the FSM must be set to have a transition
	if( m_strCurrentState.pchar()==NULL || m_strCurrentState.pchar()[0]==0 ){
		return "STATE_NULL";
	};

	// get the pointer to the FSMstate object that is the current state
	be_FSMstate *pState = GetState( m_strCurrentState.pchar() );
	if( pState == NULL ){
		// signal that there is a problem
		m_strCurrentState = "STATE_NULL";
		return m_strCurrentState.pchar();
	};

	// now pass along the input transition value and let the FSMstate
	// do the really tough job of transitioning for the FSM, and save
	// off the output state returned as the new current state of the
	// FSM and return the output state to the calling process
	m_strCurrentState = pState->GetOutput( pstrInput );
	return m_strCurrentState.pchar();
}; 
int				be_FSMclass	::FindID( ClonesArray<be_FSMstate>& strList, const char* str ){
	int N=strList.GetAmount();
	while (N--) {
		if ( strcmp(strList[N]->m_strStateName.pchar(),str)==0 ) return N;
	};
	return -1;
};
//////////////////////////////////////////////////////////////////////////





































