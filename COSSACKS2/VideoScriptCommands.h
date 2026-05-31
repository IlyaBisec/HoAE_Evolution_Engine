#pragma once
//==================================================================================================================//
class PlayVideo : public Procedure
{
public:
	ClassPtr<StringReturner> Name;
	SAVE(PlayVideo)
		REG_PARENT(Procedure);
		REG_AUTO(Name);
	ENDSAVE;
	virtual void GetViewMask(DString& ST);
	virtual int GetNArguments();
	virtual Operand* GetArgument(int NIndex);
	virtual void SetArgument(int index, Operand* Op);
	virtual bool Execute();
	virtual const char* GetProcedureName();
};
//==================================================================================================================//
class CPlaySound : public Procedure
{
public:
	ClassPtr<StringReturner> Name;
	SAVE(CPlaySound)
		REG_PARENT(Procedure);
		REG_AUTO(Name);
	ENDSAVE;
	virtual int GetNArguments();
	virtual Operand* GetArgument(int NIndex);
	virtual void SetArgument(int index, Operand* Op);
	virtual bool Execute();
	virtual const char* GetProcedureName();
};
//==================================================================================================================//