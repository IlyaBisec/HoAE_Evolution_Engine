#include "stdheader.h"
//////////////////////////////////////////////////////////////////////////
#include "supereditor.h"
//////////////////////////////////////////////////////////////////////////
bool StringVariableReference::Get(BaseType* BT)
{
	Variable* Vr=(Variable*)VarPoint;
	if(Vr)
	{
		StringType* VV=(StringType*)Vr->VarType.Get();
		StringType* ST = dynamic_cast<StringType*> (BT);
		if(ST&&VV&&VV->String.pchar())
		{
			ST->String=VV->String;
			return true;
		}
	}
	return false;
}
bool StringVariableReference::Set(BaseType* BT)
{
	Variable* Vr=(Variable*)VarPoint;
	if(Vr)
	{
		StringType* VV=(StringType*)Vr->VarType.Get();
		StringType* ST = dynamic_cast<StringType*> (BT);
		if(ST&&ST->String.pchar())
		{
			VV->String=ST->String;
			return true;
		}
	}
	return false;
}
bool StringConst::Get(BaseType* BT)
{
	StringType* ST = dynamic_cast<StringType*> (BT);
	if(ST&&Value.String.pchar())
	{
		ST->String=Value.String;
		return true;
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////