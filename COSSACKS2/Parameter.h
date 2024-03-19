#ifndef	__PARAMETER__FOR__FUNCTION___ 
#define	__PARAMETER__FOR__FUNCTION___

enum	BE_USAGE_FLAGS 
{
	BE_IF_EDIT = 0,
	BE_IF_USE  = 1,
	BE_IF_CALL = 2,
	BE_IF_DEFF = 3
};
extern	BE_USAGE_FLAGS	g_BE_USAGE_FLAG;

// UNIVERSAL PARAM - USE FRO CREATE LOCAL OR GLOBAL PARAM ////////////////
class be_CVariableBase;
class be_CGROUP;
class be_CNODE;
class be_CBOOL;
class be_CINT;
class be_CUnitType;
class be_CFurmationType;
class be_CDWORD;
class be_CFLOAT;
class be_CSTR;
class be_CARRAY;
class be_CClassPtrVB : public ClassPtr<be_CVariableBase>
{
public:
	virtual bool ForceSimplification()	{ return true; };
	virtual int	 GetExpansionRules()	{ return 0;	   };
};
class be_CUniveraslParam : public BaseClass
{
public:
	be_CUniveraslParam();
	~be_CUniveraslParam();
public:
	bool			Local;				// Set true for use as local parameter
	// GLOBAL	
	_str_ref		GlobalReference;	// Universal Reference for parameter data (local/global)
	// LOCAL
	be_CClassPtrVB	LocalObject;		// Use for store local data for parameter
	void	ChangeLocal();
	void	SetLocal(bool local);
	bool	GetClassTypeFromParent(_str& ClassName);
	void	PrepareParam(bool forCreateENUM=false);
	class CLocalGlobal : public BaseFunction
	{
	public:
		virtual const	char*	GetThisElementView(const char* LocalName);
						void	EvaluateFunction();
		SAVE(CLocalGlobal);
		REG_PARENT(BaseFunction);
		ENDSAVE;
	}setUsage;
	// MODIFICATORS
	virtual bool			ForceSimplification()	{ return true; };
	virtual	DWORD			GetClassMask();
	virtual bool			OnChangeMember(BaseClass* MembClass,void* MembPtr,void* MembExtra,const char* MembName);
	// FAROM PARENT
	virtual	BE_USAGE_FLAGS	GetParentUSAGE();
public:
    SAVE(be_CUniveraslParam);

		INVISIBLE	REG_MEMBER( _bool,		Local );
		
		REG_AUTO  ( setUsage );

		SAVE_SECTION(0x00000001);	// Local == false
			REG_ENUM  ( _strindex,	GlobalReference, BE_OBJECT_ENUM );
			INVISIBLE REG_AUTO  ( LocalObject );
		SAVE_SECTION(0x00000002);	// Local == true
			INVISIBLE REG_ENUM  ( _strindex,	GlobalReference, BE_OBJECT_ENUM );
			REG_AUTO  ( LocalObject );

	ENDSAVE;
};
// PARAMETERS ////////////////////////////////////////////////////////////
class be_CBasePrametr;
class be_CPrametrBase : public ReferableBaseClass
{
public:

	virtual const	char*	GetThisElementView(const char* LocalName);
	virtual			DWORD	GetClassMask();
	virtual			bool	SetParamData(be_CBasePrametr* paramData){ return false; };

	// DESCRIPTION
	_str	ParamName;	// Parametr name for LOCAL usage
	_str	ParamType;	// Parametr type for LOCAL usage

	static	char	sPARNAME[256];
					char*	GetParamType();

	virtual			bool	SetParamName(const char* par_name);
	virtual			bool	SetParamType(const char* par_type);
	virtual			bool	SetParam(const char* par_name, const char* par_type);

	virtual	be_CVariableBase*	GetVariable(){ return NULL; };

	// FAROM PARENT
	virtual			BE_USAGE_FLAGS	GetParentUSAGE();

	// SYSTEM
	virtual	void	UpdateEnumForOpenParam();

	virtual	void	_string( _str& _string );

	SAVE(be_CPrametrBase);
		SAVE_SECTION(0x00000001);	// Only for create
			REG_AUTO(ParamName);
			REG_ENUM(_strindex,ParamType,BE_CLASS_ENUM);
	ENDSAVE;
};

class be_CBasePrametr : public be_CPrametrBase
{
public:
	be_CBasePrametr();
	virtual const	char*	GetThisElementView(const char* LocalName);

	be_CUniveraslParam			UniveraslParam;	// Global ID of object in sustem for this param
	virtual				void	PrepareParam(bool forCreateENUM=false);
	virtual				bool	SetParamData(be_CBasePrametr* paramData);
	virtual				bool	SetParam(const char* par_name, const char* par_type, bool local);
	virtual	be_CVariableBase*	GetVariable();

	virtual				void	_string( _str& _string );

	SAVE(be_CBasePrametr);
		REG_PARENT(be_CPrametrBase);
		SAVE_SECTION(0x00000001);
			INVISIBLE REG_AUTO( UniveraslParam);
		SAVE_SECTION(0x00000002);	// For set it value
			INVISIBLE REG_AUTO(ParamName);
			INVISIBLE REG_ENUM(_strindex,ParamType,BE_CLASS_ENUM);
			REG_AUTO( UniveraslParam);
	ENDSAVE;

};

class be_CPrametr : public be_CBasePrametr
{
public:
	be_CPrametr(){};
	virtual		DWORD	GetClassMask(){ return 0x00000003; };
	SAVE(be_CPrametr);
		REG_BASE(be_CPrametrBase);
		REG_PARENT(be_CBasePrametr);
	ENDSAVE;
};

class be_CParamList : public ClassArray<be_CPrametrBase>
{
public:
	be_CParamList();
public:
	virtual bool		ForceSimplification() { return true; };
	virtual const char*	GetThisElementView(const char* LocalName);
			bool		CreateCopyFrom(const be_CParamList& paramList);

	virtual	bool		CheckParam( const char* pParamName );
	virtual	bool		AddParam( const char* par_type, const char* par_name, bool local );
	virtual	void		DelParam( const char* par_name );
	virtual	void		CLEAR();

	virtual	bool		SetParamData(be_CParamList& ParamList);
	virtual	bool		SetParamData( const char* par_name, be_CVariableBase* pData );
			bool		SetParam( const char* par_name, be_CVariableBase* pData );
	
	be_CVariableBase*	GetParam(const char* Name);
	be_CVariableBase*	GetValue(const char* Name);
	be_CBasePrametr*	GetParamObj(const char* Name);
	bool				IsLocal(char* Name);

	// DRAW
	virtual	void		vDraw();

	// AS STRING
	void	_string( _str& _string );

	// use for exchange data in FSM
	be_CGROUP*			CreateParam_GROUP		(const char* Name);
	be_CNODE*			CreateParam_NODE		(const char* Name);
	be_CFLOAT*			CreateParam_FLOAT		(const char* Name);
	be_CSTR*			CreateParam_STR			(const char* Name);
	be_CINT*			CreateParam_INT			(const char* Name);
	be_CDWORD*			CreateParam_DWORD		(const char* Name);
	be_CBOOL*			CreateParam_BOOL		(const char* Name);
	be_CARRAY*			CreateParam_ARRAY		(const char* Name);
	UnitAbility*		CreateParam_UintAbil	(const char* Name);
	WeaponModificator*	CreateParam_WeaponMod	(const char* Name);

	// FAROM PARENT
	virtual		BE_USAGE_FLAGS	GetParentUSAGE();

	// SYSTEM
	static	be_CBasePrametr*	OpenParam;	// Use for recteate ENUM for open param, seted in GetThisElementView
	virtual	void		UpdateEnumForOpenParam();
};

void be_REG_CLASS_Parameter();

#endif//__PARAMETER__FOR__FUNCTION___







































