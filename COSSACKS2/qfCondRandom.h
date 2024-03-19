//////////////////////////////////////////////////////////////////////////
#pragma once
#include "supereditor.h"
//////////////////////////////////////////////////////////////////////////
class qfCondRandom: public BoolReturner
{
public:
	ClassPtr<NumericalReturner> Op;
	SAVE(qfCondRandom)
		REG_PARENT(BoolReturner);
		REG_PARENT(Returner);
		REG_AUTO(Op);
	ENDSAVE

	virtual void GetViewMask(DString& ST) { ST.Add("Random(<0>)"); };
	virtual int GetNArguments() { return 1; };
	virtual Operand* GetArgument(int Index) { if(Index==0) return Op.Get(); return NULL; };
	virtual void SetArgument(int index, Operand* Op);
	virtual bool Get(BaseType* BT);
	virtual const char* GetFunctionName() { return "Random"; };
};

class CGetGameTime: public NumericalReturner
{
public:
	SAVE(CGetGameTime)
		REG_PARENT(NumericalReturner);
		REG_PARENT(Returner);
	ENDSAVE

	virtual void GetViewMask(DString& ST) { ST.Add("GetGameTime()"); };
	virtual int GetNArguments() { return 0; };
	virtual bool Get(BaseType* BT);
	virtual const char* GetFunctionName() { return "GetGameTime"; };
};