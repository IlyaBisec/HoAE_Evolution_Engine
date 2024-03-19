#ifndef	__SMART__GROUP__ 
#define	__SMART__GROUP__

// SMART GROUP ///////////////////////////////////////////////////////////
class be_CSMARTOBJECT	: public ReferableBaseClass
{
public:

	_str_ref		stateName;			//the current state of the state machine
	_str_ref		next_stateName;		//the next state when a state change was requested
	bool			force_state_change;	//whether a state change has been requested

	virtual	bool	ProcessStateMachine(be_CMsgObject* msg);
};


//////////////////////////////////////////////////////////////////////////
#endif//__SMART__GROUP__


































