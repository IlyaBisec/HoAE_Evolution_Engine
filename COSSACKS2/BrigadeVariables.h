#pragma once
//==================================================================================================================//
class BVariables : public NumericalAcceptor, public NumericalReturner 
{
public:
	BVariables(void);
	int VarPoint;


	SAVE_EX(BVariables,NumericalAcceptor);
		REG_PARENT(NumericalAcceptor);
		REG_PARENT(NumericalReturner);
		REG_PARENT(Returner);
		REG_PARENT(Acceptor);
		REG_ENUM(_index,VarPoint,BrigadeVariablesListEnum);
	ENDSAVE;

	virtual void GetViewMask(DString& ST);
	virtual int GetNArguments();
	virtual Operand* GetArgument(int NIndex);
	virtual void SetArgument(int index, Operand* Op);
	
	virtual bool Get(BaseType* BT);
	virtual bool Set(BaseType* BT);
	virtual const char* GetFunctionName();
};
//==================================================================================================================//
class GetPlayerNation : public NumericalReturner
{
public:
	GetPlayerNation();
	SAVE(GetPlayerNation)
		REG_PARENT(NumericalReturner);
		REG_PARENT(Returner);
	ENDSAVE;

	virtual void GetViewMask(DString& ST);
	virtual bool Get(BaseType* BT);
	virtual const char* GetFunctionName();
};
//==================================================================================================================//
class GetPlayerSelectedBrigadeID : public NumericalReturner
{
public:
	ClassPtr<NumericalReturner> NI;
	SAVE(GetPlayerSelectedBrigadeID)
		REG_PARENT(NumericalReturner);
		REG_PARENT(Returner);
		REG_AUTO(NI);
	ENDSAVE;

	virtual void GetViewMask(DString& ST);
	virtual int GetNArguments();
	virtual Operand* GetArgument(int NIndex);
	virtual void SetArgument(int index, Operand* Op);
	virtual bool Get(BaseType* BT);
	virtual const char* GetFunctionName();
};
//==================================================================================================================//
class SetCurrentNationColor : public Procedure
{
public:
	ClassPtr<NumericalReturner> NI;
	SAVE(SetCurrentNationColor)
		REG_PARENT(Procedure);
		REG_AUTO(NI);
	ENDSAVE;
	virtual void GetViewMask(DString& ST);
	virtual int GetNArguments();
	virtual Operand* GetArgument(int NIndex);
	virtual void SetArgument(int index, Operand* Op);
	virtual bool Execute();
	virtual const char* GetProcedureName();
};
//==================================================================================================================//
class SetCurrentBrigadeID : public Procedure
{
public:
	ClassPtr<NumericalReturner> NI;
	ClassPtr<NumericalReturner> BID;
	SAVE(SetCurrentBrigadeID)
		REG_PARENT(Procedure);
		REG_AUTO(NI);
		REG_AUTO(BID);
	ENDSAVE;
	virtual void GetViewMask(DString& ST);
	virtual int GetNArguments();
	virtual Operand* GetArgument(int NIndex);
	virtual void SetArgument(int index, Operand* Op);
	virtual bool Execute();
	virtual const char* GetProcedureName();
};
//==================================================================================================================//
//==================================================================================================================//
//==================================================================================================================//
//extern struct FormSituation;
struct ActiveObjectIndexses
{
	int NI;
	int BrigadeID;
	int UnitIndex;
	FormSituation FormationSituation;
	char** ComList;
	int NCom;
	char* CurrentPlayingBik;
	int BrigadeUnitsType;
	
	DynArray<DString*> OggSoundComList;
	DString CurrentPlayingOgg;
};
//==================================================================================================================//
