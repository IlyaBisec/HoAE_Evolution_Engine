#ifndef __GROUPE_OPERATIONS__
#define __GROUPE_OPERATIONS__

/************************************************************************/
/* Classes for create conditions                                        */
/************************************************************************/
// lvCCondition //////////////////////////////////////////////////////////
class lvCCondition : public BaseClass
{
public:
	lvCCondition()	{ InfID=_lvCCondition_; Descr="Condition"; };
	lvCCondition(lvCCondition* cond);
public:
    DWORD	InfID;
	_str	Descr;
	int		SHIFT;
public:
	virtual		int				GetValue(int time)	{ return 0; };
	virtual		void			SetEdgeXY(int _x0,int _y0,int _x1,int _y1)  {};
	virtual		void			SetSquardID(DWORD sqID)						{};
	virtual		void			GetCopy(lvCCondition** pCopy);
	virtual		int				Power() { return 0; };
	virtual		void			Draw(){};

	virtual		char*			GetSourceCode(int shift = 0);

	virtual		void			Prepare_lua(){};

	class CPRESENTATION	fShowPresentation;
public:
	SAVE(lvCCondition);
//		REG_AUTO(fShowPresentation);
	ENDSAVE;
};
// lvCAriphmetical ///////////////////////////////////////////////////////
class lvCAriphmetical : public lvCCondition
{
public:
	lvCAriphmetical()	{ InfID = _lvCAriphmetical_; };
	lvCAriphmetical(lvCAriphmetical* pAriphmetical);
public:
	ClassPtr<lvCCondition>	COND1;
	int						OPER;
	ClassPtr<lvCCondition>	COND2;
public:
	virtual const	char*				GetThisElementView(const char* LocalName);
	virtual			int					GetValue(int time);
	virtual			void				SetEdgeXY(int _x0,int _y0,int _x1,int _y1);
	virtual			void				SetSquardID(DWORD sqID);
	virtual			void				GetCopy(lvCCondition** pCopy);
	virtual			int					Power();

	virtual			char*				GetSourceCode(int shift = 0);
public:
	SAVE(lvCAriphmetical);
		REG_PARENT(lvCCondition);
		REG_AUTO(COND1);
		REG_ENUM(_index,OPER,ARIPHMETICALOPS);
		REG_AUTO(COND2);
	ENDSAVE;
};
// lvCLogical ////////////////////////////////////////////////////////////
class lvCLogical : public lvCCondition
{
public:
	lvCLogical()	{ InfID = _lvCLogical_; };
	lvCLogical(lvCLogical* pLogical);
public:
	int							OPER;
	ClassArray<lvCCondition>	CONDS;

public:
	virtual const	char*		GetThisElementView(const char* LocalName);
	virtual			int			GetValue(int time);
	virtual			void		SetEdgeXY(int _x0,int _y0,int _x1,int _y1);
	virtual			void		SetSquardID(DWORD sqID);
	virtual			void		GetCopy(lvCCondition** pCopy);
	virtual			int			Power();

	virtual			char*		GetSourceCode(int shift = 0);
public:
	SAVE(lvCLogical);
		REG_PARENT(lvCCondition);
		REG_ENUM(_index,OPER,LOGICALOPS);
		REG_AUTO(CONDS);
	ENDSAVE;
};
// lvCCompare ////////////////////////////////////////////////////////////
class lvCCompare : public lvCCondition
{
public:
	lvCCompare()	{ InfID = _lvCCompare_; };
	lvCCompare(lvCCompare* pCompare);
public:
	ClassPtr<lvCCondition>	COND1;
	int						OPER;
	ClassPtr<lvCCondition>	COND2;

	// For lua operation
	int						OPER_lua;
	virtual			void			Prepare_lua(){ OPER_lua=OPER; };

public:
	virtual const	char*	GetThisElementView(const char* LocalName);
	virtual			int		GetValue(int time);
	virtual			void	SetEdgeXY(int _x0,int _y0,int _x1,int _y1);
	virtual			void	SetSquardID(DWORD sqID);
	virtual			void	GetCopy(lvCCondition** pCopy);
	virtual			int		Power();

	virtual			char*	GetSourceCode(int shift = 0);
public:
	SAVE(lvCCompare);
		REG_PARENT(lvCCondition);
		REG_AUTO(COND1);
		REG_ENUM(_index,OPER,OPTYPE);
		REG_AUTO(COND2);
		REG_ENUM(_index,OPER_lua,OPTYPE_LUA);
	ENDSAVE;
};
// lvCFunctional /////////////////////////////////////////////////////////
// For siplification editor
class CFunctionPtr : public ClassPtr<lvCBaseFunction>
{
public:
	virtual bool ForceSimplification(){
		return true;
	}//if returns true, this node will be hidden in class editor	
};
class lvCFunctional : public lvCCondition
{
public:
	lvCFunctional()	{ InfID = _lvCFunctional_; };
	lvCFunctional(lvCFunctional* pFunctional);
public:
	CFunctionPtr	Function;
public:
	virtual const	char*	GetThisElementView(const char* LocalName);
	virtual			int		GetValue(int time);
	virtual			void	SetEdgeXY(int _x0,int _y0,int _x1,int _y1);
	virtual			void	SetSquardID(DWORD sqID);
	virtual			void	GetCopy(lvCCondition** pCopy);
	virtual			int		Power();

	virtual			char*	GetSourceCode(int shift = 0);
public:
	SAVE(lvCFunctional);
		REG_PARENT(lvCCondition);
		REG_AUTO(Function);
	ENDSAVE;
};
//////////////////////////////////////////////////////////////////////////
void	REG_BE_CONDITION_class();

/************************************************************************/
/* Classes for create comands                                           */
/************************************************************************/

// lvCBaseScript /////////////////////////////////////////////////////////
class lvCBaseScript : public BaseClass
{
public:
	lvCBaseScript()		{ InfID=_lvCBaseScript_; };
	lvCBaseScript(lvCBaseScript* pBaseScript);
	~lvCBaseScript()	{};

	DWORD	InfID;

	virtual			  void	GetCopy(lvCBaseScript** pCopy);
	virtual			  bool	GetConditions()	{ return true; };
	virtual			  int	Process(int time) { return 0; };
	virtual		const char* GetThisElementView(const char* LocalName) { return NULL; };
	virtual			  void	Draw(){};

	class CPRESENTATION	fShowPresentation;

	virtual			void			Prepare_lua(){  };

//	SAVE(lvCBaseScript);
//		REG_AUTO(fShowPresentation);
//	ENDSAVE;
};

// lvCSubSection /////////////////////////////////////////////////////////
class lvCSubSection : public SubSection
{
public:
	lvCSubSection()		{ InfID=_lvCSubSection_; Descr="NewSubSection"; };
	~lvCSubSection()	{};

	DWORD	InfID;
	_str	Descr;

	virtual		const char* GetThisElementView(const char* LocalName);

	SAVE(lvCSubSection);
		REG_PARENT(SubSection);
//		REG_PARENT(lvCBaseScript);
		REG_AUTO(Descr);
	ENDSAVE;
};

// lvCCondForOper //////////////////////////////////////////////////////////
// For siplification editor
class COperationArray : public ClassArray<lvCOperation>
{
public:
	virtual bool ForceSimplification(){
		return true;
	}//if returns true, this node will be hidden in class editor	
};
class CConditionArray : public ClassArray<lvCCondition>
{
public:
	virtual bool ForceSimplification(){
		return true;
	}//if returns true, this node will be hidden in class editor	
};
class lvCCondForOper : public lvCBaseScript
{
public:
	lvCCondForOper()	{ InfID=_lvCCondForOper_; Descr="CondForOper"; myID=-1;	};
	lvCCondForOper(lvCCondForOper* pCopy);
	~lvCCondForOper()	{ Condition.Clear(); Operation.Clear();			};
public:
    _str	Descr;

	SubSection				Condition_Section;
	CConditionArray			Condition;
	SubSection				Operation_Section;
	COperationArray			Operation;

	int myID;
public:
	virtual			  void	GetCopy(lvCBaseScript** pCopy);
	virtual			  bool	GetConditions()	{ return true; };
	virtual			  int	Process(int time);
	virtual		const char* GetThisElementView(const char* LocalName);
	virtual			  void	Draw();

	virtual			  void	Restart();
	virtual			  char* GetSourceCode(int shift = 0);
	virtual			  void	Prepare_lua();
public:
	SAVE(lvCCondForOper);
		REG_PARENT(lvCBaseScript);
		REG_AUTO(Descr);
		REG_AUTO(Condition_Section);
		REG_AUTO(Condition);
		REG_AUTO(Operation_Section);
		REG_AUTO(Operation);
		
	ENDSAVE;
};
//////////////////////////////////////////////////////////////////////////

#endif//__GROUPE_OPERATIONS__

















