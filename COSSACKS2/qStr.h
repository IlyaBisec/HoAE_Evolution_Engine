//////////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////////
class StringType : public BaseType
{
public:
	_str String;
	SAVE(StringType);
		REG_PARENT(BaseType);
		REG_AUTO(String);
	ENDSAVE;
	virtual const char* GetID(){ return "StringTypeEnum"; };
	virtual const char* GetTypeName() { return "String"; };

};
class StringAcceptor : public Acceptor
{
public:
	SAVE(StringAcceptor);
		//REG_PARENT(Acceptor);
	ENDSAVE;
};
class StringReturner : public Returner
{
public:
	SAVE(StringReturner);
		//REG_PARENT(Returner);
	ENDSAVE;
};
class StringVariableReference : public StringAcceptor,  public StringReturner, public VariableReference
{
public:
	int VarPoint;
	SAVE_EX(StringVariableReference,StringAcceptor);
		REG_PARENT(Operand);
		REG_PARENT(StringAcceptor);
		REG_PARENT(StringReturner);
		REG_PARENT(Acceptor);
		REG_PARENT(Returner);
		REG_ENUM(_index,VarPoint,StringTypeEnum);
	ENDSAVE;
	
	virtual void GetViewMask(DString& ST){ VariableReference::GetViewMask(ST); };
	//virtual void GetShortViewMask(DString& ST);
	virtual bool Get(BaseType* BT);
	virtual bool Set(BaseType* BT);
};
class StringConst : public StringReturner
{
public:
	StringType Value;
	SAVE(StringConst)
		REG_PARENT(StringReturner);
		REG_PARENT(Returner);
		REG_AUTO(Value);
	ENDSAVE;

	virtual void GetViewMask(DString& ST) { ST.Add("\"");ST.Add(Value.String);ST.Add("\""); };
	virtual bool Get(BaseType* BT);
};
//////////////////////////////////////////////////////////////////////////
