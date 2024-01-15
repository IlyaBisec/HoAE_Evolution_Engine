#ifndef __PARAM_FUNCTION__
#define __PARAM_FUNCTION__

class	be_CParamFunction		: public be_CBaseParamFunction
{
public:
	be_CParamFunction();
	~be_CParamFunction();
	virtual const	char*	GetThisElementView(const char* LocalName);
public:
	virtual	int		CallFunction();

	be_CBaseUnaryStatement		FunctionBody;

	// FAROM PARENT
	virtual		BE_USAGE_FLAGS	GetParentUSAGE();

	SAVE(be_CParamFunction);
		REG_PARENT(be_CBaseParamFunction);
		REG_AUTO(FunctionBody);
	ENDSAVE;
};

class	be_CBaseLUAFunction		: public be_CBaseParamFunction
{
public:
	be_CBaseLUAFunction();
	~be_CBaseLUAFunction();
	virtual const	char*	GetThisElementView(const char* LocalName);
public:
	_str	LUA_File;	// lua file name with this function

	void*	pDLL_Function;
public:
	virtual	int		CallFunction();

			bool	DLL_VAILID();

	// FAROM PARENT
	virtual		BE_USAGE_FLAGS	GetParentUSAGE();

public:
	SAVE(be_CBaseLUAFunction);
		REG_PARENT(be_CBaseParamFunction);
		REG_FILEPATH(LUA_File,".script,.lua,.luac");
	ENDSAVE;
};

class	be_CRefParamFunction	: public be_CBaseParamFunction
{
public:
	virtual const	char*	GetThisElementView(const char* LocalName);

	_str_ref			CallingFunctionName;	// Select function by name

	virtual	int			CallFunction();
	virtual	int			CallFunction(ActiveUnitAbility* _pAbility);
	virtual	int			CallFunction(ActiveUnitAbility* _pAbility,OneObject* pOB);
	
	// FAROM PARENT
	virtual		BE_USAGE_FLAGS	GetParentUSAGE();

	void	CreateUseFnName();
	class CCreateParamList : public BaseFunction
	{
	public:
		void	EvaluateFunction();
		SAVE(CCreateParamList);
			REG_PARENT(BaseFunction);
		ENDSAVE;
	}fCreateParamList;

public:
	SAVE(be_CRefParamFunction);
		REG_BASE(be_CFunctionBase);
		REG_BASE(be_CBaseParamFunction);
		REG_ENUM(_strindex,CallingFunctionName,BE_FUNCTION_ENUM);
		REG_AUTO(fCreateParamList);
		REG_AUTO(InArgList);
		INVISIBLE REG_AUTO(Name);
	ENDSAVE;
};

void	be_REG_CLASS_ParamFunction();

#endif//__PARAM_FUNCTION__
























