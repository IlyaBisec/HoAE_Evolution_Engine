#ifndef __BASE__FUNCTION__
#define __BASE__FUNCTION__

class	ActiveUnitAbility;

class	be_CFunctionBase		: public ReferableBaseClass
{
public:
	virtual			int		CallFunction();
	virtual	const	char*	GetThisElementView(const char* LoaclName);

	// FAROM PARENT
	virtual		BE_USAGE_FLAGS	GetParentUSAGE();
	static	ActiveUnitAbility*	pAbility;
	static	OneObject*			pAbilityActivator;

	virtual		bool			isElementInParentClass(char* FildName);

	LinearArray<int,_int>	dbgCB_IDS;
	void	clearCB();
	void	addCB_ID(int _id);
	int		getCB_ID(int index);

	virtual void	checkModificator(){};

	SAVE(be_CFunctionBase);
	ENDSAVE;
};

class	be_CComment				: public be_CFunctionBase
{
public:
	virtual			int		CallFunction()			{ return 1;		};
	virtual			bool	ForceSimplification()	{ return true;	}

	_str			Comment;

	SAVE(be_CComment);
		REG_PARENT(be_CFunctionBase);
		REG_AUTO(Comment);
	ENDSAVE;
};

class	be_CBaseParamFunction	: public be_CFunctionBase
{
public:
	virtual			int		CallFunction();
	virtual			bool	CheckParam( const char* pParamName );
	virtual			bool	AddParam( const char* par_type, const char* par_name, bool local );
	virtual			bool	SetParamData(be_CParamList& paramData);
	virtual			bool	SetName(const char* fn_name);
	virtual			void	ClearParamList();

	be_CParamList			InArgList;
	
	SAVE(be_CBaseParamFunction);
		REG_BASE(be_CFunctionBase);
		REG_AUTO(Name);
		REG_AUTO(InArgList);
	ENDSAVE;
};

class	be_CBasePtrFunction		: public ClassPtr<be_CFunctionBase>
{
public:
	virtual	int		CallFunction();
			int		getCB_ID(int index){ if (Get()!=NULL) return Get()->getCB_ID(index); return -1; };
	virtual bool	GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags);
	SAVE(be_CBasePtrFunction);
		REG_PARENT(ClassPtr<be_CFunctionBase>);
	ENDSAVE;
};

class	be_CBaseArrayFunction	: public ClassArray<be_CFunctionBase>
{
public:
	virtual	int		CallFunction();
	virtual bool	GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags);
	SAVE(be_CBaseArrayFunction);
		REG_PARENT(ClassArray<be_CFunctionBase>);
	ENDSAVE;
};

class be_CBaseComlexFunction : public be_CFunctionBase
{
public:
			_str	Modificator;
	virtual void	checkModificator(){};
};

template< class T1 >
class	be_CBaseUnary			: public be_CBaseComlexFunction
{
public:
	virtual	int		CallFunction() {
		return FirstOperand.CallFunction();
	};
	
	T1	FirstOperand;
};

template< class T1, class T2=T1 >
class	be_CBaseBinary			: public be_CBaseComlexFunction
{
public:
	virtual	int CallFunction() {
		int retVal =  FirstOperand.CallFunction();
		if ( retVal ) SecondOperand.CallFunction(); 
		return retVal;
	};
	
	T1	FirstOperand;
	T2	SecondOperand;
};

template< class T1, class T2=T1, class T3=T2 >
class	be_CBaseTrinary			: public be_CBaseComlexFunction
{
public:
	virtual	int CallFunction() {
		g_beDebuger.callFunction( getCB_ID(0) );
		int retVal =  FirstOperand.CallFunction();
		if ( retVal ){
			g_beDebuger.callFunction( getCB_ID(1) );
			retVal = SecondOperand.CallFunction();
		}else{
			g_beDebuger.callFunction( getCB_ID(2) );
			retVal = ThirdOperand.CallFunction();
		};
		g_beDebuger.callFunction( getCB_ID(3) );
		return retVal; 
	};
	
	T1	FirstOperand;
	T2	SecondOperand;
	T3	ThirdOperand;
};

void	be_REG_CLASS_Function();

#endif//__FUNCTION__




























