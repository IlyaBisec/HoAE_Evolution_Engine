
#ifndef __FSM_CLASS__
#define __FSM_CLASS__

class be_FSMclass : public BaseClass
{
public:
	be_FSMclass ();
	be_FSMclass ( char* pstrStateName );	// set initial state of the FSM
	~be_FSMclass();							// clean up memory usage
public:
	ClonesArray<be_FSMstate>	m_StateArray;		// map containing all states of this FSM
	_str						m_strCurrentState;	// the m_iStateID of the current state

public:
	// return the current state ID
	char*			GetCurrentState	()								{ return m_strCurrentState.pchar();		 }
	// set current state
	void			SetCurrentState	( const char* pstrStateName )	{ m_strCurrentState = pstrStateName; }

	be_FSMstate*	GetState		( const char* pstrStateName );	// return the FSMstate object pointer
	void			AddState		( be_FSMstate* pState );		// add a FSMstate object pointer to the map
	void			DeleteState		( const char* pstrStateName );	// delete a FSMstate object pointer from the map

	char*			StateTransition	( const char* pstrInput );		// perform a state transition based on input and current state
private:
	int				FindID( ClonesArray<be_FSMstate>& strList, const char* str );
};

#endif//__FSM_CLASS__