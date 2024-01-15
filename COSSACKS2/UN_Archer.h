#ifndef __UN_Archer__
#define __UN_Archer__

// aiOBJECT_STATES_ENUM ////////////////////////////////////////////////////////////
enum aiOBJECT_STATES_ENUM {
	aiOSE_FREE		= 0,
	aiOSE_ATTACK	= 1,
	aiOSE_DEFENCE	= 2,
	aiOSE_ESCAPE	= 3,
	aiOSE_SEARCH	= 4,

	aiOSE_CLEAR		= 254,

	aiOSE_ERROR		= 255
};

__forceinline void GetObjectSt_StrONE(aiOBJECT_STATES_ENUM st, char* nameST, size_t length ){
	switch(st) {
	case aiOSE_FREE:
		strncpy(nameST,"FREE",length-1);
		break;
	case aiOSE_ATTACK:
		strncpy(nameST,"ATTACK",length-1);
		break;
	case aiOSE_DEFENCE:
		strncpy(nameST,"DEFENCE",length-1);
		break;
	case aiOSE_ESCAPE:
		strncpy(nameST,"ESCAPE",length-1);
		break;
	case aiOSE_CLEAR:
		strncpy(nameST,"CLEAR",length-1);
		break;
	case aiOSE_ERROR:
		strncpy(nameST,"ERROR",length-1);
		break;
	default:
		strncpy(nameST,"ERROR",length-1);
		break;
	};
	nameST[length-1] = 0;
};

__forceinline aiOBJECT_STATES_ENUM CovertArcherSt_to_ObjectSt(aiARCHER_STATES ArcherST){
	aiOBJECT_STATES_ENUM retValue = aiOSE_CLEAR;
	switch(ArcherST) {
	case aiFREE:
		retValue = aiOSE_FREE;
		break;
	case aiATACK:
		retValue = aiOSE_ATTACK;
		break;
	case aiESCAPE:
		retValue = aiOSE_ESCAPE;
		break;
	default:
		retValue = aiOSE_CLEAR;
	};
	return retValue;
};

// aiOBJECT ////////////////////////////////////////////////////////////////////////
class aiOBJECT
{
public:
			 aiOBJECT() { STATE=newSTATE=aiOSE_CLEAR; updateST=false; };
	virtual	~aiOBJECT() {};

	virtual	 void					UpdateStateStr()				{};
	virtual	 void					ShowStateStr(bool show=false)	{};

	virtual	 bool					SetState(aiOBJECT_STATES_ENUM newState);
	virtual	 aiOBJECT_STATES_ENUM	GetState();

	virtual	 bool					PROCESS();
protected:
	virtual	 bool					OnEnterState()	{ return true; };	// Добиваться newSTATE.
	virtual	 bool					UpdateState()	{ return true; };	// Оставаться в  STATE.
	virtual	 bool					OnExitState()	{ return true; };	// Выходить из   STATE.
	
	aiOBJECT_STATES_ENUM	STATE;
	aiOBJECT_STATES_ENUM	newSTATE;
	bool					updateST;
};

// aiARCHER //////////////////////////////////////////////////////////////
class aiARCHER : public aiOBJECT
{ 
public:
			 aiARCHER()	{ u_logicType=aiFL_MAX; u_id=u_sn=0xFFFF; strSATE="C-C"; };
	virtual ~aiARCHER() {};
private:
	word				u_id;
	word				u_sn;
	aiFL_TYPE			u_logicType;
public:
	_str				strSATE;
	virtual	 void					UpdateStateStr();
	virtual	 void					ShowStateStr(bool show=false);

	virtual	 bool					SetState	(aiOBJECT_STATES_ENUM newState) { return aiOBJECT::SetState(newState);	};
	void							SetUnit		(word id);
	void							SetLogic	(aiFL_TYPE type);

	void							GetFiling	(iaARCHER_FILINGS& filings);
	virtual	 aiOBJECT_STATES_ENUM	GetState	()								{ return aiOBJECT::GetState();			};

	virtual	 bool					PROCESS		()								{ return aiOBJECT::PROCESS();			}
protected:
	OneObject*	GetUnitOB();
	
	virtual	 bool	OnEnterState();	// Добиваться newSTATE.
	virtual	 bool	UpdateState ();	// Оставаться в  STATE.
	virtual	 bool	OnExitState ();	// Выходить из   STATE.
	
	virtual	 bool	Attack_ON		();
	virtual	 bool	Attack_DO		();
	virtual	 bool	Attack_OFF		();

	virtual	 bool	Escape_ON		();
	virtual	 bool	Escape_DO		();
	virtual	 bool	Escape_OFF		();

	virtual	 bool	FreeAc_ON		();
	virtual	 bool	FreeAc_DO		();
	virtual	 bool	FreeAc_OFF		();
};

// aiARCHERS_GRP /////////////////////////////////////////////////////////
class aiARCHERS_GRP
{
public:
	aiARCHERS_GRP()  { Clean(); showST=false; };
	~aiARCHERS_GRP() { Clean(); };

	bool				showST;

	DynArray<aiARCHER*>	ArchersGrp;
	aiARCHER_LOGIC*		pLOGIC;

	void	SetGrp(DWORD grpID,aiARCHER_LOGIC* pLogic);
	void	Clean();

	void	PROCESS();
	void	ShowState();
};

//////////////////////////////////////////////////////////////////////////
#endif//__UN_Archer__