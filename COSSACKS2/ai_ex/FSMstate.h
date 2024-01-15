#ifndef __FSM_STATE__
#define __FSM_STATE__

class	be_FSMstate : public BaseClass
{
public:
	ClonesArray<_str>		m_pstrInputs;		// input array for tranistions
	ClonesArray<_str>		m_pstrOutputState;	// output state array
	_str					m_strStateName;		// the unique Name of this state
	
public:
	// constructor accepts an ID for this state and the number of transitions to support
	be_FSMstate ();
	be_FSMstate	( const char* strStateName );
	// destructor cleans up allocated arrays
	~be_FSMstate();

	// access the state ID
	char*	GetName			();
	// add a state transition to the array
	void	AddTransition	( const char* strInput, const char* strOutputStateName );
	// remove a state transation from the array
	void	DeleteTransition( const char* strOutputStateName );
	// get the output state and effect a transistion
	char*	GetOutput		( const char* strInput );
	// get amount of elements
	int		GetAmount		();

private:
    int		FindID( ClonesArray<_str>& strList, const char* str );
};

#endif//__FSM_STATE__


































