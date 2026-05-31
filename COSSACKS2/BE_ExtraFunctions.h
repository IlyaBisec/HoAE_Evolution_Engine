class massplay_CheckIfServer : public be_CFunctionBase // lvCCheckReadyBuildingBetwinPosition
{	
public:
	virtual	int	CallFunction();

	SAVE(massplay_CheckIfServer);
		REG_BASE(be_CFunctionBase);

	ENDSAVE;
};
class massplay_CheckIfAnyUnitUnderMyControl : public be_CFunctionBase
{
public:
	virtual	int	CallFunction();

	SAVE(massplay_CheckIfAnyUnitUnderMyControl);
	REG_BASE(be_CFunctionBase);	
	ENDSAVE;    
};
class massplay_CheckIfSomeUnitUnderMyControl : public be_CFunctionBase
{
public:
	virtual	int	CallFunction();

	SAVE(massplay_CheckIfSomeUnitUnderMyControl);
	REG_BASE(be_CFunctionBase);	
	ENDSAVE;    
};
class massplay_AddToGlobalNetVariable : public be_CFunctionBase
{
public:
	virtual	int	CallFunction();

	SAVE(massplay_AddToGlobalNetVariable);
	REG_BASE(be_CFunctionBase);	
	ENDSAVE;    
};
class massplay_GetGlobalNetVariable : public be_CFunctionBase
{
public:
	virtual	int	CallFunction();

	SAVE(massplay_GetGlobalNetVariable);
	REG_BASE(be_CFunctionBase);	
	ENDSAVE;    
};