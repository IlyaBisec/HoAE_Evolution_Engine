#ifndef	__COMPLEX_FUNCTION__ 
#define	__COMPLEX_FUNCTION__ 

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	UNARY FUNCTION SECTION
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class	be_CBaseUnaryOperator		: public be_CBaseUnary< be_CBasePtrFunction >
{
public:
	be_CBaseUnaryOperator();
	virtual void	checkModificator();
	virtual	int		CallFunction();

	SAVE(be_CBaseUnaryOperator);
		REG_PARENT(be_CFunctionBase);
		REG_ENUM(_strindex,Modificator,BE_UO_CFMODES_ENUM);
		REG_AUTO(FirstOperand);
	ENDSAVE;
};

class	be_CBaseUnaryStatement		: public be_CBaseUnary< be_CBaseArrayFunction >
{
public:
	be_CBaseUnaryStatement();
	virtual void	checkModificator();
	virtual	int		CallFunction();

	bool		Hide;
	_str		Descr;
	virtual		DWORD	GetClassMask();

	SAVE(be_CBaseUnaryStatement);
		REG_PARENT(be_CFunctionBase);
		REG_ENUM(_strindex,Modificator,BE_US_CFMODES_ENUM);
	SAVE_SECTION(0x00000002);
		REG_MEMBER(_bool,Hide);
	SAVE_SECTION(0x00000004);
		REG_AUTO(Descr);
		INVISIBLE REG_AUTO(FirstOperand);
	SAVE_SECTION(0x00000001);
		INVISIBLE REG_AUTO(Descr);
		REG_AUTO(FirstOperand);
	ENDSAVE;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BINARY FUNCTION SECTION
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class	be_CBaseBinaryOperator		: public be_CBaseBinary< be_CBasePtrFunction >
{
public:
	be_CBaseBinaryOperator();
	virtual void	checkModificator();
	virtual	int		CallFunction();

	SAVE(be_CBaseBinaryOperator);
		REG_PARENT(be_CFunctionBase);
		REG_ENUM(_strindex,Modificator,BE_BO_CFMODES_ENUM);
		REG_AUTO(FirstOperand);
		REG_AUTO(SecondOperand);
	ENDSAVE;
};

class	be_CBaseBinaryStatement		: public be_CBaseBinary< be_CBasePtrFunction, be_CBaseArrayFunction >
{
public:
	be_CBaseBinaryStatement();
	virtual void	checkModificator();
	virtual	int		CallFunction();

	SAVE(be_CBaseBinaryStatement);
		REG_PARENT(be_CFunctionBase);
		REG_ENUM(_strindex,Modificator,BE_BS_CFMODES_ENUM);
		REG_AUTO(FirstOperand);
		REG_AUTO(SecondOperand);
	ENDSAVE;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TRINARY FUNCTION SECTION
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class	be_CBaseTrinaryOperator		: public be_CBaseTrinary< be_CBasePtrFunction >
{
public:
	be_CBaseTrinaryOperator();
	virtual void	checkModificator();
	virtual	int		CallFunction();

	SAVE(be_CBaseTrinaryOperator);
		REG_PARENT(be_CFunctionBase);
		REG_ENUM(_strindex,Modificator,BE_TO_CFMODES_ENUM);
		REG_AUTO(FirstOperand);
		REG_AUTO(SecondOperand);
		REG_AUTO(ThirdOperand);
	ENDSAVE;
};

class	be_CBaseTrinaryStatement	: public be_CBaseTrinary< be_CBasePtrFunction, be_CBaseArrayFunction >
{
public:
	be_CBaseTrinaryStatement();
	virtual void	checkModificator();
	virtual	int		CallFunction();

	SAVE(be_CBaseTrinaryStatement);
		REG_PARENT(be_CFunctionBase);
		REG_ENUM(_strindex,Modificator,BE_TS_CFMODES_ENUM);
		REG_AUTO(FirstOperand);
		REG_AUTO(SecondOperand);
		REG_AUTO(ThirdOperand);
	ENDSAVE;
};

//////////////////////////////////////////////////////////////////////////

void be_REG_CALSS_ComplexFunction();

#endif//__COMPLEX_FUNCTION__ 