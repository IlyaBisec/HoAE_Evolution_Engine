#include "stdheader.h"
#include "qUnitType.h"
//==================================================================================================================//
qUnitType::qUnitType()
{
	Type=-1;
}
void qUnitType::GetViewMask(DString& ST)
{
	ST.Add(GetFunctionName());
	ST.Add(":");
	if(Type>-1)
	{
		static Enumerator* E_UT=ENUM.Get("ALL_UNITS");
		ST.Add(E_UT->Get(Type));
	}
}
int qUnitType::GetNArguments()
{
	return 1;
}
void qUnitType::SetArgument(int index, Operand* Op)
{
	if(index==0&&Op)
	{
		StringConst* ST= dynamic_cast<StringConst*>(Op);
		if(ST)
		{
			static Enumerator* E_UT=ENUM.Get("ALL_UNITS");
			char* ut=ST->Value.String.pchar();
			char* _ut=strstr(ut,"_");
			if(_ut)
			{
				char* ut_=strstr(_ut+1,"_");
				if(ut_)
				{
					*_ut='(';
					*ut_=')';
				}
			}
			int ref=E_UT->Get(ut);
			if(ref!=-1)
			{
				Type=ref;
			}
		}
		delete Op;
	}
}
bool qUnitType::Get(BaseType* BT)
{
	if(BT)
	{
		NumericalType* NT=dynamic_cast<NumericalType*>(BT);
		if(NT)
		{
			NT->Value=Type;
			return true;
		}
	}
	return false;
}
const char* qUnitType::GetFunctionName()
{
	return "UnitType";
}
//==================================================================================================================//
qBrigadeFormationType::qBrigadeFormationType()
{
	Type=-1;
}
void qBrigadeFormationType::GetViewMask(DString& ST)
{
	ST.Add(GetFunctionName());
	ST.Add(":");
	if(Type>-1)
	{
		static Enumerator* E_UT=ENUM.Get("BrigadeFormationTypeEnum");
		const char* fn=E_UT->Get(Type);
		ST.Add(fn+1);
	}
}
int qBrigadeFormationType::GetNArguments()
{
	return 1;
}
void qBrigadeFormationType::SetArgument(int index, Operand* Op)
{
	if(index==0&&Op)
	{
		StringConst* ST= dynamic_cast<StringConst*>(Op);
		if(ST)
		{
			static Enumerator* E_UT=ENUM.Get("BrigadeFormationTypeEnum");
			char* ut=ST->Value.String.pchar();
			int f=E_UT->GetAmount();
			DString st;
			st.Add("#");st.Add(ut);
			int ref=E_UT->Get(st.pchar());
			if(ref!=-1)
			{
				Type=ref;
			}
		}
		delete Op;
	}
}
bool qBrigadeFormationType::Get(BaseType* BT)
{
	if(BT)
	{
		NumericalType* NT=dynamic_cast<NumericalType*>(BT);
		if(NT)
		{
			NT->Value=Type;
			return true;
		}
	}
	return false;
}
const char* qBrigadeFormationType::GetFunctionName()
{
	return "BrigadeFormation";
}
//==================================================================================================================//
