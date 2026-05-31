#include "stdheader.h"
#include "supereditor.h"

ActiveObjectIndexses  ActiveIndexses;
//==================================================================================================================//
BVariables::BVariables(void)
{
}
void BVariables::GetViewMask(DString& ST)
{
	ST.Add("Brigade."); 
	if(VarPoint)
	{
		Variable* Vr=(Variable*)VarPoint;
		ST.Add(Vr->Name);
	}
}
int BVariables::GetNArguments()
{
	return 1;
}
Operand* BVariables::GetArgument(int NIndex)
{
	if(NIndex==0)
		return (Operand*)(VarPoint);
	return NULL;
}
void BVariables::SetArgument(int index, Operand* Op)
{
	static Enumerator* E_BVR = ENUM.Get("BrigadeVariablesListEnum"); 
	char* tmp = E_BVR->Get(DWORD(Op));
	if(strlen(tmp)>0)
	{
		VarPoint=(int)Op;
	}
	else
	{
		StringConst* ST= dynamic_cast<StringConst*>(Op);
		if(ST)
		{
			
			//int ff=E_BVR->GetAmount();
			int ref=E_BVR->Get(ST->Value.String.pchar());
			if(ref!=-1)
			{
				VarPoint=ref;
			}
		}
		delete Op;
	}
}
bool BVariables::Get(BaseType* BT)
{
	bool rez=false;
	if(VarPoint)
	{
		NumericalArray* NA=dynamic_cast<NumericalArray*>(((Variable*)VarPoint)->VarType.Get());
		if(NA)
		{
			if(ActiveIndexses.BrigadeID>-1&&ActiveIndexses.BrigadeID!=0xFFFF)
				rez=NA->GetElement(ActiveIndexses.BrigadeID*32+ActiveIndexses.NI,BT);
		}
		else
		{
			NumericalTypePointer* NTP=dynamic_cast<NumericalTypePointer*>(((Variable*)VarPoint)->VarType.Get());
			if(NTP)
				rez=NTP->GetValue(BT);
		}
	}
	return rez;
}
bool BVariables::Set(BaseType* BT)
{
	bool rez=false;
	if(VarPoint)
	{
		NumericalArray* NA=dynamic_cast<NumericalArray*>(((Variable*)VarPoint)->VarType.Get());
		if(NA)
		{
			if(ActiveIndexses.BrigadeID!=-1&&ActiveIndexses.BrigadeID!=0xFFFF)
				rez=NA->SetElement(ActiveIndexses.BrigadeID*32+ActiveIndexses.NI,BT);
		}
		else
		{
			NumericalTypePointer* NTP=dynamic_cast<NumericalTypePointer*>(((Variable*)VarPoint)->VarType.Get());
			if(NTP)
				rez=NTP->SetValue(BT);
		}
	}
	return rez;
}
const char* BVariables::GetFunctionName()
{
	return "Brigade";
}
//==================================================================================================================//
GetPlayerNation::GetPlayerNation()
{}
void GetPlayerNation::GetViewMask(DString& ST)
{
	ST.Add("GetPlayerNation()"); 
}
DLLEXPORT int GetMyNation();
bool GetPlayerNation::Get(BaseType* BT)
{
	if(BT)
	{
		NumericalType* NA=(NumericalType*)(BT);
		
		NA->Value=GetMyNation();
		return true;
	}
	return false;
}
const char* GetPlayerNation::GetFunctionName()
{
	return "GetPlayerNation";
}
//==================================================================================================================//
void GetPlayerSelectedBrigadeID::GetViewMask(DString& ST)
{
	ST.Add("GetPlayerSelectedBrigadeID(<0>)"); 
}
int GetPlayerSelectedBrigadeID::GetNArguments()
{
	return 1;
}
Operand* GetPlayerSelectedBrigadeID::GetArgument(int NIndex)
{
	if(NIndex==0)
		return NI.Get();
	return NULL;
}
void GetPlayerSelectedBrigadeID::SetArgument(int index, Operand* Op)
{
	if(index==0&&Op)
	{
		NI.Set((NumericalReturner*)Op);
	}
}
word CheckIfFormationInterface(byte& NI);
bool GetPlayerSelectedBrigadeID::Get(BaseType* BT)
{
	bool rez=false;
	if(BT)
	{
		NumericalType NT;
		NumericalType* NA=(NumericalType*)(BT);
		if(NI.Get()&&NI.Get()->Get(&NT))
		{
			byte nn=(byte)NT.Value;
			word bi=CheckIfFormationInterface(nn);
			if(bi==0xFFFF||nn!=NT.Value)
				NA->Value=-1;
			else
				NA->Value=bi;
			rez=true;
		}
	}
	return rez;
}
const char* GetPlayerSelectedBrigadeID::GetFunctionName()
{
	return "GetPlayerSelectedBrigadeID";
}
//==================================================================================================================//
void SetCurrentNationColor::GetViewMask(DString& ST)
{
	ST.Add("SetCurrentNationColor(<0>)"); 
}
int SetCurrentNationColor::GetNArguments()
{
	return 1;
}
Operand* SetCurrentNationColor::GetArgument(int NIndex)
{
	if(NIndex==0)
		return NI.Get();
	return NULL;
}
void SetCurrentNationColor::SetArgument(int index, Operand* Op)
{
	if(index==0&&Op)
	{
		NI.Set((NumericalReturner*)Op);
	}
}
bool SetCurrentNationColor::Execute()
{
	bool rez=false;
	NumericalType NT;
	if(NI.Get()&&NI.Get()->Get(&NT))
	{
		ActiveIndexses.NI=NT.Value;
		rez=true;
	}
	return rez;
}
const char* SetCurrentNationColor::GetProcedureName()
{
	return "SetCurrentNationColor";
}
//==================================================================================================================//
void SetCurrentBrigadeID::GetViewMask(DString& ST)
{
	ST.Add("SetCurrentBrigadeID(<0>,<1>)"); 
}
int SetCurrentBrigadeID::GetNArguments()
{
	return 2;
}
Operand* SetCurrentBrigadeID::GetArgument(int NIndex)
{
	if(NIndex==0)
		return NI.Get();
	if(NIndex==1)
		return BID.Get();
	return NULL;
}
void SetCurrentBrigadeID::SetArgument(int index, Operand* Op)
{
	if(index==0&&Op)
	{
		NI.Set((NumericalReturner*)Op);
	}
	if(index==1&&Op)
	{
		BID.Set((NumericalReturner*)Op);
	}
}
void EstimateFormationSituation(byte NI,word BrigID,FormSituation* S);
void B_GoOnRoadLink(Brigade* BR);
bool SetCurrentBrigadeID::Execute()
{
	bool rez=false;
	NumericalType NT;
	NumericalType NT1;
	if(NI.Get()&&NI.Get()->Get(&NT)&&BID.Get()&&BID.Get()->Get(&NT1))
	{
		if(NT.Value!=-1)
		{
			ActiveIndexses.BrigadeID=NT.Value;
			ActiveIndexses.NI=NT1.Value;
			if(ActiveIndexses.BrigadeID>-1&&ActiveIndexses.NI>-1)
			{
				//EstimateFormationSituation(ActiveIndexses.NI,ActiveIndexses.BrigadeID,&ActiveIndexses.FormationSituation);
				Brigade* BR = &CITY[ActiveIndexses.NI].Brigs[ActiveIndexses.BrigadeID];
				if(BR&&BR->NewBOrder&&BR->NewBOrder->GetBrigadeOrderID()==BRIGADEORDER_GOONROAD)
					ActiveIndexses.FormationSituation.FormID=222;
			}
			rez=true;
		}
	}
	return rez;
}
const char* SetCurrentBrigadeID::GetProcedureName()
{
	return "SetCurrentBrigadeID";
}
//==================================================================================================================//
