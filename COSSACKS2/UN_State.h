#ifndef __UN_State__
#define __UN_State__

// UN_StateManager ///////////////////////////////////////////////////////
class UN_StateManager
{
public:
	 UN_StateManager() {};
	~UN_StateManager() {};
};

// UN_BaseState //////////////////////////////////////////////////////////
class UN_BaseState
{
public:
			 UN_BaseState()	{};
	virtual	~UN_BaseState() {};

	virtual	 bool	OnEnter();
	virtual	 bool	Update();
	virtual	 bool	OnExit();
};

// UN_AttackState ////////////////////////////////////////////////////////
class UN_AttackState : public UN_BaseState
{
public:
			 UN_AttackState() {};
	virtual ~UN_AttackState() {};

	virtual	 bool	OnEnter();
	virtual	 bool	Update();
	virtual	 bool	OnExit();
};

// UN_EscapeState ////////////////////////////////////////////////////////
class UN_EscapeState : public UN_BaseState
{
public:
			 UN_EscapeState() {};
	virtual	~UN_EscapeState() {};

	virtual	 bool	OnEnter();
	virtual	 bool	Update();
	virtual	 bool	OnExit();
};

// UN_FreeState //////////////////////////////////////////////////////////
class UN_FreeState : public UN_BaseState
{
public:
			 UN_FreeState() {};
	virtual	~UN_FreeState() {};

	virtual	 bool	OnEnter();
	virtual	 bool	Update();
	virtual	 bool	OnExit();
};

//////////////////////////////////////////////////////////////////////////
#endif//__UN_State__

















