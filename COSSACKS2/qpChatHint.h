//////////////////////////////////////////////////////////////////////////
#pragma once
#include "supereditor.h"
//////////////////////////////////////////////////////////////////////////
class qpChatHint : public Procedure
{
public:
	ClassPtr<StringReturner> Op;
	SAVE(qpChatHint)
		REG_PARENT(Procedure);
		REG_AUTO(Op);
	ENDSAVE;

	virtual void GetViewMask(DString& ST){ ST.Add("ChatHint(<0>)"); };
	virtual int GetNArguments() { return 1; };
	virtual Operand* GetArgument(int Index) { if(Index==0) return Op.Get(); return NULL; };
	virtual bool Execute();
};
