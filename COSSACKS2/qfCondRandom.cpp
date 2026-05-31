#include "stdheader.h"
//////////////////////////////////////////////////////////////////////////
#include "qfCondRandom.h"
//////////////////////////////////////////////////////////////////////////
extern int AnimTime;

bool qfCondRandom::Get(BaseType* BT){	
	BoolType* Ret=dynamic_cast<BoolType*>(BT);
	if(Ret){
		NumericalType op;
		if(Op.Get()&&Op.Get()->Get(&op)){
			Ret->Value=op.Value>((rando()*100)>>15);
			return true;
		}
	}
	return false;
};
void qfCondRandom::SetArgument(int index, Operand* Opr)
{
	if(index==0)
	{
		if(Op.Get())
		{
			delete (Op.Get());
		}
		Op.Set((NumericalReturner*)Opr);
	}
}

extern int TrueTime;
bool CGetGameTime::Get(BaseType* BT){	
	NumericalType* Ret=dynamic_cast<NumericalType*>(BT);
	if(Ret)
	{
		Ret->Value=TrueTime;
		return true;
	}
	return false;
};