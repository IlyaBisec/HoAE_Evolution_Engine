#ifndef __BE_BLOCK__
#define __BE_BLOCK__

//************************************************************************
//						QUICK VALUE EDITOR								**
//************************************************************************
class TypeShowMask : public BaseClass
{
public:
	TypeShowMask() { Show=false; /*ClassInfID=0xFFFF;*/ };
	~TypeShowMask(){};

	bool	Show;		// Select current class type for show
//	DWORD	ClassInfID;	// Class InfID for current class
	_str	ClassName;	// Current class type name
    
	virtual const char* GetElementView(int Index,const char* LocalName){
		if ( Index == 0 )
			return ClassName.pchar();
		return LocalName;
	};//if fild is "Show" show it name as ClassName.pchar()

	virtual bool ForceSimplification(){
		return true;
	}//if returns true, this node will be hidden in class editor

	SAVE(TypeShowMask);
		REG_MEMBER(_bool,Show);
	ENDSAVE;
};

class TypeShowMaskArray : public ClonesArray<TypeShowMask>
{
public:
	
	TypeShowMaskArray() : ClonesArray<TypeShowMask>() {};

	virtual bool ForceSimplification(){
		return true;
	}//if returns true, this node will be hidden in class editor

};

class beViewElem : public ClassRef< BaseClass >
{
public:
	virtual bool ForceSimplification(){
		return true;
	}//if returns true, this node will be hidden in class editor

};

class beViewElemList : public ClassArray< beViewElem >
{
public:
	virtual int        GetAmountOfEditableElements    (void* DataPtr,DWORD Options);
};

class beSET_Obj : public beBaseObject
{
public:
	beViewElemList	ARRAY;

	beSET_Obj()	{};
	virtual ~beSET_Obj();

	void	clearARRAY();

	int	createArray( ClassArray<BaseClass>* BaseArray, char* ClassName, bool add=false );

	SAVE( beSET_Obj );
		REG_AUTO( ARRAY );
	ENDSAVE;
};

void	REG_BE_CLASS();
//************************************************************************

//************************************************************************
//						INDEPENDENT FUNCTION							**
//************************************************************************
// FUNCTIONS /////////////////////////////////////////////////////////////
class be_CFunctionBaseOLD : public ReferableBaseClass
{
public:

	virtual const	char*	GetThisElementView(const char* LocalName);

	be_CParamList	InArgList;			// List of input arguments

	bool	CheckParam( const char* pParamName );
	bool	SetParamData(be_CParamList& paramData);

	SAVE(be_CFunctionBaseOLD);
		REG_AUTO(Name);
		REG_AUTO(InArgList);
	ENDSAVE;
};

class be_CBaseBEFunction : public be_CFunctionBaseOLD
{
public:

	//~~~~~~~~~~~~~~~    BODY   ~~~~~~~~~~~~~~~~~~~//
	ClassArray<lvCBaseScript>		FnBodyArray;
	int	 CallFunctions();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

	SAVE(be_CBaseBEFunction);
		REG_PARENT(be_CFunctionBaseOLD);
		REG_AUTO(FnBodyArray);
	ENDSAVE;
};

// FUNCTION STORAGE //////////////////////////////////////////////////////
class be_CFunctionArray : public ClassArray<be_CFunctionBaseOLD>
{
public:
	
	virtual bool	ForceSimplification	()	{ return true; };
	virtual int		GetExpansionRules	()	{ return 2;	   };

};

class be_CFunctionStore	: public BaseClass
{
public:
	
	be_CFunctionArray		FnStore;

	SAVE(be_CFunctionStore);
		REG_LOADSAVE("Missions\\FunctionStore\\","FnStore.xml")
		REG_AUTO( FnStore );
	ENDSAVE;
};



//************************************************************************

//**********************************************************************//
//******************    GENERIC FUNCTIONS    ***************************//
//**********************************************************************//
class	lvCGenericFN : public lvCBaseOperCond
{
public:
	virtual const	char*	GetThisElementView(const char* LocalName);
	
	_str_ref					FunctionName;	// Select function by name
	be_CParamList				InArgList;		// Param list with data for fonction

	virtual int		Process	(int time)		{ return CallFunction(); };
	virtual	int		GetValue(int time)		{ return CallFunction(); };
	virtual	int		CallFunction();

	void	CreateUseFnName();
	class CCreateParamList : public BaseFunction
	{
	public:
		void	EvaluateFunction();
		SAVE(CCreateParamList);
			REG_PARENT(BaseFunction);
		ENDSAVE;
	}fCreateParamList;

	SAVE(lvCGenericFN);
		REG_BASE(lvCBaseFunction);
		REG_BASE(lvCOperation);
		REG_ENUM(_strindex,FunctionName,BE_FUNCTION_ENUM);
		REG_AUTO(fCreateParamList);
		REG_AUTO(InArgList);
	ENDSAVE;
};

void	be_RegBlockENUM();

#endif//__BE_BLOCK__





























