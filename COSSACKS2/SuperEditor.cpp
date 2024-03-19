//==================================================================================================================//
#include "stdheader.h"
#include ".\supereditor.h"
//#include <ctype.h>
//==================================================================================================================//
template <class Class>
Class safe_dynamic_cast(BaseClass* ptr){
	try{
        return dynamic_cast<Class>(ptr);
	}catch(...){
		return NULL;

	}
}

SuperEditor::SuperEditor(void)
{
}

SuperEditor::~SuperEditor(void)
{
}

//==================================================================================================================//
Operand::Operand()
{}
Operand::~Operand()
{}
bool Operand::Preview(DString& ST,word Shift)
{
	ST.Add(GetTextByID("NULL"));
	return true;
}
void Operand::GetViewMask(DString& ST)
{
	ST.Add(GetTextByID("Operand"));
}
void Operand::GetShortViewMask(DString& ST)
{
	ST.Add(GetTextByID("Operand"));
}
int Operand::GetNArguments()
{
	return 0;
}
Operand* Operand::GetArgument(int NIndex)
{
	return NULL;
}
void Operand::SetArgument(int Index, Operand* Op)
{
}
/*
void Operand::GetAssembledView(DString& ST,bool SetURL)
{
	DString DMask;
	DString DM;
	GetViewMask(DMask);
	if(DMask.L)
	{
		if(SetURL)
			CreateURL(DM,DMask,this);
		else
			DM=DMask;
		int n=GetNArguments();
		for(int i=0;i<n;i++)
		{
			DString Rep;
			Rep.Add("<");Rep.Add(i);Rep.Add(">");
			char* fp=strstr(DMask.pchar(),Rep.pchar());
			if(fp)
			{
				DString Spas;
				Spas.Add("\\");
				for(char* k=fp-1;k>=DMask.pchar();k--)
				{
					if(k[0]==' ')
					{
						Spas.Add(" ");
					}
					else
					{
						break;
					}
				}
				DString AMsk;
				AMsk="";
				if(SetURL)AMsk.Add("{C}");
				if(GetArgument(i))
				{
					GetArgument(i)->GetAssembledView(AMsk,SetURL);
				}
				else
				{
					AMsk.Add(GetTextByID("NotDef"));
				}
				AMsk.Replace0("\\",Spas.pchar());
				if(SetURL)AMsk.Add("{R FFFF0000 ");
				if(SetURL)AMsk.Add((int)this);
				if(SetURL)AMsk.Add("}");
				DM.Replace0(Rep.pchar(),AMsk.pchar());
			}
		}
		ST.Add(DM);
	}
}
*/
void Operand::GetAssembledView(DString& ST,bool SetURL)//for qpp recovering
{
	DString DMask;
	DString DM;
	GetViewMask(DMask);
	if(DMask.L)
	{
		if(SetURL)
			CreateURL(DM,DMask,this);
		else
			DM=DMask;
		int n=GetNArguments();
		for(int i=0;i<n;i++)
		{
			DString Rep;
			Rep.Add("<");Rep.Add(i);Rep.Add(">");
			char* fp=strstr(DMask.pchar(),Rep.pchar());
			if(fp)
			{
				DString Spas;
				Spas.Add("\n");
				for(char* k=fp-1;k>=DMask.pchar();k--)
				{
					if(k[0]==' ')
					{
						Spas.Add(" ");
					}
					else
					{
						break;
					}
				}
				DString AMsk;
				AMsk="";
				if(SetURL)AMsk.Add("{C}");
				if(GetArgument(i))
				{
					GetArgument(i)->GetAssembledView(AMsk,SetURL);
				}
				else
				{
					AMsk.Add(GetTextByID("NotDef"));
				}
				AMsk.Replace0("\\",Spas.pchar());
				if(SetURL)AMsk.Add("{R FFFF0000 ");
				if(SetURL)AMsk.Add((int)this);
				if(SetURL)AMsk.Add("}");
				DM.Replace0(Rep.pchar(),AMsk.pchar());
			}
		}
		ST.Add(DM);
	}
}
const char* Operand::GetThisElementView(const char* LocalName)
{
	static char s[256];
	DString ST;
	GetAssembledView(ST,false); 
	if(ST.L)
	{
		char* s1=strstr(ST.pchar(),"\\");
		if(s1)s1[0]=0;
		strncpy(s,ST.pchar(),200);
	}
	s[200]=0;
	return s;
}
void Operand::CreateURL(DString& ST,DString& Tx,Operand* URL)
{
	CreateURL(ST,Tx.pchar(),URL);
}
void Operand::CreateURL(DString& ST,char* Tx,Operand* URL)
{
	ST.Add("{R FFFF0000 ");
	ST.Add((int)URL);
	ST.Add("}");
	ST.Add(Tx);
	ST.Add("{C}");
}
bool Operand::Get(BaseType* BT)
{
	return false;
}
bool Operand::Set(BaseType* BT)
{
	return false;
}
//==================================================================================================================//
const char* BaseType::GetID()
{
	return "BaseTypeEnum";
}
bool BaseType::GetValue(BaseType* BT)
{
	return false;
}
bool BaseType::SetValue(BaseType* BT)
{
	return false;
}
void BaseType::GetViewMask(DString& ST)
{
	ST.Add("Type");
}
const char* BaseType::GetTypeName()
{
	return "BaseType";
}
//==================================================================================================================//
OperandBlock::OperandBlock()
{}
OperandBlock::~OperandBlock()
{}
bool OperandBlock::Execute()
{
	bool rez=false;
	int n=Operands.GetAmount();
	for(int i=0;i<n;i++)
	{
		if(Operands[i]->Execute())
			rez=true;
	}
	return rez;
}
bool OperandBlock::Preview(DString& ST,word Shift)
{
	int n=Operands.GetAmount();
	ST.Add("Begin\\");
	for(int i=0;i<n;i++)
	{
		ST.Add("    ");
		Operands[i]->Preview(ST,Shift+1);
		ST.Add("\\");
	}
	ST.Add("End");
	return true;
}
void OperandBlock::GetViewMask(DString& ST)
{
	int n=Operands.GetAmount();
	ST.Add(GetTextByID("Begin"));//ST.Add("Begin\\");GetTextByID
	for(int i=0;i<n;i++)
	{
		ST.Add("    ");
		ST.Add("<");ST.Add(i);ST.Add(">");
		ST.Add("\\");
	}
	ST.Add(GetTextByID("End"));//ST.Add("End");
}
void OperandBlock::GetShortViewMask(DString& ST)
{
	ST.Add("Begin ... End");
}
int OperandBlock::GetNArguments()
{
	return Operands.GetAmount();
}
Operand* OperandBlock::GetArgument(int NIndex)
{
	return Operands[NIndex];
}
void OperandBlock::SetArgument(int NIndex, Operand* Op)
{
	if(Operands[NIndex])
		delete (Operands[NIndex]);
	Operands[NIndex]=(Procedure*)Op;
}
//==================================================================================================================//
OperandBlock2::OperandBlock2()
{}
OperandBlock2::~OperandBlock2()
{}
void OperandBlock2::GetViewMask(DString& ST)
{
	int n=Operands.GetAmount();
	for(int i=0;i<n;i++)
	{
		ST.Add("    ");
		ST.Add("<");ST.Add(i);ST.Add(">");
		ST.Add("\\");
	}
}
//==================================================================================================================//
const char* Returner::GetFunctionName()
{
	return "";
}
//==================================================================================================================//
BaseType* Acceptor::GetArgumentType()
{
	return NULL;
}
//==================================================================================================================//
NumericalType NumericalAcceptor::TypeExample;
BaseType* NumericalAcceptor::GetArgumentType()
{
	return (BaseType*) (&TypeExample);
}
//==================================================================================================================//
NumericalType BoolAcceptor::TypeExample;
BaseType* BoolAcceptor::GetArgumentType()
{
	return (BaseType*) (&TypeExample);
}
//==================================================================================================================//
NumericalArray NumericalArrayAcceptor::TypeExample;
BaseType* NumericalArrayAcceptor::GetArgumentType()
{
	return (BaseType*) (&TypeExample);
}
//==================================================================================================================//
void NumericalConst::GetViewMask(DString& ST)
{
	ST.Add(Value.Value);
}
bool NumericalConst::Get(BaseType* BT)
{
	bool rez=false;
	NumericalType* NT = safe_dynamic_cast<NumericalType*> (BT);
	if(NT)
	{
		NT->Value=Value.Value;
		rez=true;
	}
	return rez;
}
//==================================================================================================================//
Variable::Variable()
{
	Name.Clear();
	Name.Add("");
}
Variable::~Variable()
{}
void Variable::GetViewMask(DString& ST)
{
	if(VarType.Get())
	{
		ST.Add((char*)VarType.Get()->GetTypeName());
		ST.Add(" ");
	}
	if(Name.L)
	{
		ST.Add(Name);
	}
	else
	{
		ST.Add("Variable");
	}
}
//==================================================================================================================//
NumericalType::NumericalType()
{
	Value=0;
}
const char* NumericalType::GetID()
{
	return "NumericalTypeEnum";
}
bool NumericalType::GetValue(BaseType* BT)
{
	bool rez=false;
	NumericalType* NT = safe_dynamic_cast<NumericalType*> (BT);
	if(NT)
	{ 
		NT->Value=Value;
		rez=true;
	}
	return rez;
}
bool NumericalType::SetValue(BaseType* BT)
{
	bool rez=false;
	NumericalType* NT = safe_dynamic_cast<NumericalType*> (BT);
	if(NT)
	{
		Value=NT->Value;
		rez=true;
	}
	return rez;
}
const char* NumericalType::GetTypeName()
{
	return "int";
}
//==================================================================================================================//
NumericalTypePointer::NumericalTypePointer()
{
	pInt=0;
}
NumericalTypePointer::NumericalTypePointer(int* P)
{
	pInt=P;
}
bool NumericalTypePointer::GetValue(BaseType* BT)
{
	bool rez=false;
	NumericalType* NT = safe_dynamic_cast<NumericalType*> (BT);
	if(NT&&pInt)
	{ 
		NT->Value=(*pInt);
		rez=true;
	}
	return rez;
}
bool NumericalTypePointer::SetValue(BaseType* BT)
{
	bool rez=false;
	NumericalType* NT = safe_dynamic_cast<NumericalType*> (BT);
	if(NT&&pInt)
	{
		(*pInt)=NT->Value;
		rez=true;
	}
	return rez;
}
//==================================================================================================================//
BoolType::BoolType()
{
	Value=0;
} 
const char* BoolType::GetID()
{
	return "BoolTypeEnum";
}
bool BoolType::GetValue(BaseType* BT)
{
	bool rez=false;
	BoolType* NT = safe_dynamic_cast<BoolType*> (BT);
	if(NT)
	{
		NT->Value=Value;
		rez=true;
	}
	return rez;
}
bool BoolType::SetValue(BaseType* BT)
{
	bool rez=false;
	BoolType* NT = safe_dynamic_cast<BoolType*> (BT);
	if(NT)
	{
		Value=NT->Value;
		rez=true;
	}
	return rez;
}
const char* BoolType::GetTypeName()
{
	return "bool";
}
//==================================================================================================================//
template<class T>
ArrayType<T>::ArrayType()
{}
template<class T>
bool ArrayType<T>::GetElement(int Index, BaseType* BT)
{
	bool rez=false;
	if(Index<Values.GetAmount())
	{
		rez=BT->SetValue(Values[Index]);
	}
	return rez;
}
template<class T>
bool ArrayType<T>::SetElement(int Index, BaseType* BT)
{
	bool rez=false;
	int n=Values.GetAmount();
	for(int i=0;i<(Index-n+1);i++)
	{
		Values.Add((T*)GetNewElement());
	}
	return Values[Index]->SetValue(BT);
}
template<class T>
bool ArrayType<T>::AddElement(BaseType* BT)
{
	bool rez=false;
	BaseType* NE=GetNewElement();
	if(NE)
	{
		if(NE->SetValue(BT))
		{
			Values.Add((T*)NE);
			rez=true;
		}
		else
			delete NE;
	}
	return rez;
}
template<class T>
void ArrayType<T>::Clear()
{
	Values.Clear();
}
template<class T>
int ArrayType<T>::GetNElements()
{
	return Values.GetAmount();
}
template<class T>
bool ArrayType<T>::GetValue(BaseType* BT)
{
	bool rez=false;
	ArrayType* DA = safe_dynamic_cast<ArrayType*> (BT);
	if(DA)
	{
		DA->Clear();
		int n=GetNElements();
		for(int i=0;i<n;i++)
		{
			if(DA->AddElement(Values[i]))
			{
				rez=true;
			}
			else
			{
				DA->Clear();
				break;
			}
		}
	}
	return rez;
}
template<class T>
bool ArrayType<T>::SetValue(BaseType* BT)
{
	bool rez=false;
	ArrayType* DA = safe_dynamic_cast<ArrayType*> (BT);
	if(DA)
	{
		Clear();
		int n=DA->GetNElements();
		for(int i=0;i<n;i++)
		{
			BaseType* NE = GetNewElement();
			if(DA->GetElement(i,NE))
			{
				Values.Add((T*)NE);
				rez=true;
			}
			else
			{
				Clear();
				break;
			}
		}
	}
	return rez;
}
template<class T>
BaseType* ArrayType<T>::GetNewElement()
{
	return new T;
}
template<class T>
const char* ArrayType<T>::GetID()
{
	return "ArrayTypeEnum";
}
//==================================================================================================================//
VariablesList::VariablesList()
{}
VariablesList::~VariablesList()
{}
void VariablesList::GetViewMask(DString& ST)
{
	int n=Variables.GetAmount();
	ST.Add(GetTextByID("Variables"));//ST.Add("Variables\\");
	for(int i=0;i<n;i++)
	{
		ST.Add("    ");
		ST.Add("<");ST.Add(i);ST.Add(">");
		ST.Add("\\");
	}
}
void VariablesList::GetShortViewMask(DString& ST)
{
	ST.Add("Variables");
}
int VariablesList::GetNArguments()
{
	return Variables.GetAmount();
}
Operand* VariablesList::GetArgument(int NIndex)
{
	return (Operand*)Variables[NIndex];
}
void VariablesList::RefreshEnumerator()
{
	int n=Variables.GetAmount();
	for(int i=0;i<n;i++)
	{
		if(Variables[i]->Name.L)
		{
			if(Variables[i]->VarType.Get())
			{
				Enumerator* En = ENUM.Get(Variables[i]->VarType.Get()->GetID());
				En->Clear();
			}
		}
	}
	for(i=0;i<n;i++)
	{
		if(Variables[i]->Name.L)
		{
			if(Variables[i]->VarType.Get())
			{
				Enumerator* En = ENUM.Get(Variables[i]->VarType.Get()->GetID());
				En->Add(Variables[i]->Name.pchar(),(DWORD)(Variables[i]));
			}
		}
	}
}
bool VariablesList::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){
	bool r=BaseClass::Load(xml,ClassPtr,Error,Extra);
	((VariablesList*)ClassPtr)->RefreshEnumerator();
	return r;
}
//==================================================================================================================//
struct ActiveObjectIndexses;
extern ActiveObjectIndexses  ActiveIndexses;
BrigadeVariablesList::BrigadeVariablesList(){
	IsInit=false;
}
void BrigadeVariablesList::Init()
{
	if(!IsInit)
	{
		FormID.VarType.Set(new NumericalTypePointer(&ActiveIndexses.FormationSituation.FormID));
		FormID.Name.Add("FormationType");
		UnitID.VarType.Set(new NumericalTypePointer(&ActiveIndexses.FormationSituation.UnitID));
		UnitID.Name.Add("BrigadeUnitsType");
		FinalStateOrder.VarType.Set(new NumericalTypePointer(&ActiveIndexses.FormationSituation.FinalStateOrder));
		FinalStateOrder.Name.Add("FinalStateOrder");
		NShooters.VarType.Set(new NumericalTypePointer(&ActiveIndexses.FormationSituation.NShooters));
		NShooters.Name.Add("NShooters");
		NMelee.VarType.Set(new NumericalTypePointer(&ActiveIndexses.FormationSituation.NMelee));
		NMelee.Name.Add("NMelee");
		NDying.VarType.Set(new NumericalTypePointer(&ActiveIndexses.FormationSituation.NDying));
		NDying.Name.Add("NDying");
		NMoving.VarType.Set(new NumericalTypePointer(&ActiveIndexses.FormationSituation.NMoving));
		NMoving.Name.Add("NMoving");
		NStanding.VarType.Set(new NumericalTypePointer(&ActiveIndexses.FormationSituation.NStanding));
		NStanding.Name.Add("NStanding");
		NAttMoving.VarType.Set(new NumericalTypePointer(&ActiveIndexses.FormationSituation.NAttMoving));
		NAttMoving.Name.Add("NAttMoving");
		NAttStanding.VarType.Set(new NumericalTypePointer(&ActiveIndexses.FormationSituation.NAttStanding));
		NAttStanding.Name.Add("NAttStanding");
		Panic.VarType.Set(new NumericalTypePointer(&ActiveIndexses.FormationSituation.Panic));
		Panic.Name.Add("Panic");
		Tired.VarType.Set(new NumericalTypePointer(&ActiveIndexses.FormationSituation.Tired));
		Tired.Name.Add("Tired");
		NRecharging.VarType.Set(new NumericalTypePointer(&ActiveIndexses.FormationSituation.NRecharging));
		NRecharging.Name.Add("NRecharging");
		NPAttack.VarType.Set(new NumericalTypePointer(&ActiveIndexses.FormationSituation.NPAttack));
		NPAttack.Name.Add("NPAttack");
		NUAttack.VarType.Set(new NumericalTypePointer(&ActiveIndexses.FormationSituation.NUAttack));
		NUAttack.Name.Add("NUAttack");
		NFireReady.VarType.Set(new NumericalTypePointer(&ActiveIndexses.FormationSituation.NFireReady));
		NFireReady.Name.Add("NFireReady");
		DistToMainTarget.VarType.Set(new NumericalTypePointer(&ActiveIndexses.FormationSituation.DistToMainTarget));
		DistToMainTarget.Name.Add("DistToMainTarget");
		IsStandGround.VarType.Set(new NumericalTypePointer(&ActiveIndexses.FormationSituation.IsStandGround));
		IsStandGround.Name.Add("IsStandGround");
		Arsenal.VarType.Set(new NumericalTypePointer(&ActiveIndexses.FormationSituation.Arsenal));
		Arsenal.Name.Add("Arsenal");
		Morale.VarType.Set(new NumericalTypePointer(&ActiveIndexses.FormationSituation.Morale));//GEC
		Morale.Name.Add("Morale");//GEC
		Food.VarType.Set(new NumericalTypePointer(&ActiveIndexses.FormationSituation.Food));//GEC
		Food.Name.Add("Food");//GEC
		IsInit=true;
	}
}
void InitBrigVars(){
	extern OneScript VideoSeqScript;
	extern OneScript VidOfSeqScript;
	VideoSeqScript.Init();
	VidOfSeqScript.Init();
}
void BrigadeVariablesList::GetViewMask(DString& ST)
{
	int n=Variables.GetAmount();
	ST.Add(GetTextByID("BrigadeVariables\\"));
	for(int i=0;i<n;i++)
	{
		ST.Add("    ");
		ST.Add("<");ST.Add(i);ST.Add(">");
		ST.Add("\\");
	}
}
void BrigadeVariablesList::GetShortViewMask(DString& ST)
{
	ST.Add("BrigadeVariables");
}
void BrigadeVariablesList::RefreshEnumerator()
{
	Init();
	int n=Variables.GetAmount();
	static Enumerator* En = ENUM.Get("BrigadeVariablesListEnum");
	En->Clear();
	
	En->Add(FormID.Name.pchar(),(DWORD)(&FormID));
	En->Add(UnitID.Name.pchar(),(DWORD)(&UnitID));
	En->Add(FinalStateOrder.Name.pchar(),(DWORD)(&FinalStateOrder));
	En->Add(NShooters.Name.pchar(),(DWORD)(&NShooters));
	En->Add(NMelee.Name.pchar(),(DWORD)(&NMelee));
	En->Add(NDying.Name.pchar(),(DWORD)(&NDying));
	En->Add(NMoving.Name.pchar(),(DWORD)(&NMoving));
	En->Add(NStanding.Name.pchar(),(DWORD)(&NStanding));
	En->Add(NAttMoving.Name.pchar(),(DWORD)(&NAttMoving));
	En->Add(NAttStanding.Name.pchar(),(DWORD)(&NAttStanding));
	En->Add(Panic.Name.pchar(),(DWORD)(&Panic));
	En->Add(Tired.Name.pchar(),(DWORD)(&Tired));
	En->Add(NRecharging.Name.pchar(),(DWORD)(&NRecharging));
	En->Add(NPAttack.Name.pchar(),(DWORD)(&NPAttack));
	En->Add(NUAttack.Name.pchar(),(DWORD)(&NUAttack));
	En->Add(NFireReady.Name.pchar(),(DWORD)(&NFireReady));
	En->Add(DistToMainTarget.Name.pchar(),(DWORD)(&DistToMainTarget));
	En->Add(IsStandGround.Name.pchar(),(DWORD)(&IsStandGround));
	En->Add(Arsenal.Name.pchar(),(DWORD)(&Arsenal));
	En->Add(Morale.Name.pchar(),(DWORD)(&Morale));//GEC
	En->Add(Food.Name.pchar(),(DWORD)(&Food));//GEC

	for(int i=0;i<n;i++)
	{
		if(Variables[i]->Name.L)
		{
			if(Variables[i]->VarType.Get())
			{
				En->Add(Variables[i]->Name.pchar(),(DWORD)(Variables[i]));
			}
		}
	}
}
bool BrigadeVariablesList::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){
	bool r=BaseClass::Load(xml,ClassPtr,Error,Extra);
	((BrigadeVariablesList*)ClassPtr)->RefreshEnumerator();
	return r;
}
//==================================================================================================================//
bool Procedure::Execute()
{
	return false;
}
bool Procedure::SetArguments(Operand* ArgumentList)
{
	bool rez=false;
	if(ArgumentList)
	{
		OperandParamList* OPL = safe_dynamic_cast<OperandParamList*>(ArgumentList);
		if(OPL)
		{
			int nn = OPL->Params.GetAmount();
			for(int i=0;i<nn;i++)
				SetArgument(i,OPL->Params[i]);
		}
	}
	return rez;
}
const char* Procedure::GetProcedureName()
{
	return "Procedure";
}
void Procedure::GetViewMask(DString& ST)
{
	DString tm;
	tm=GetProcedureName();
	ST.Add(tm);
	ST.Add("(");
	int n=GetNArguments();
	for(int i=0;i<n;i++)
	{
		if(i>0)
			ST.Add(",");
		ST.Add("<");ST.Add(i);ST.Add(">");
	}
	ST.Add(");");
}
//==================================================================================================================//
void VariableReference::GetViewMask(DString& ST)
{
	if(VarPoint)
	{
		Variable* Vr=(Variable*)VarPoint;
		ST.Add(Vr->Name);
	}
	else
	{
		ST.Add(GetTextByID("Variable_Accept_int"));//ST.Add("Variable Accept int");
	}
}
bool VariableReference::Get(BaseType* BT)
{
	bool rez=false;
	if(VarPoint)
	{
		Variable* Vr=(Variable*)VarPoint;
		if(Vr->VarType.Get())
		{
			rez=Vr->VarType.Get()->GetValue(BT);
		}
	}
	return rez;
}
bool VariableReference::Set(BaseType* BT)
{
	bool rez=false;
	if(VarPoint)
	{
		Variable* Vr=(Variable*)VarPoint;
		if(Vr->VarType.Get())
		{
			rez=Vr->VarType.Get()->SetValue(BT);
		}
	}
	return rez;
}
//==================================================================================================================//
bool NumericalVariableReference::Get(BaseType* BT)
{
	bool rez=false;
	Variable* Vr=(Variable*)VarPoint;
	if(Vr)
	{
		NumericalType* VV=(NumericalType*)Vr->VarType.Get();
		NumericalType* NT = safe_dynamic_cast<NumericalType*> (BT);
		if(NT)
		{
			NT->Value=VV->Value;
			rez=true;
		}
	}
	return rez;
}
bool NumericalVariableReference::Set(BaseType* BT)
{
	bool rez=false;
	Variable* Vr=(Variable*)VarPoint;
	if(Vr)
	{
		NumericalType* VV=(NumericalType*)Vr->VarType.Get();
		NumericalType* NT = safe_dynamic_cast<NumericalType*> (BT);
		if(NT)
		{
			VV->Value=NT->Value;
			rez=true;
		}
	}
	return rez;
}
void NumericalVariableReference::GetViewMask(DString& ST)
{
	VariableReference::GetViewMask(ST);
}
//==================================================================================================================//
void NumericalArrayVariableReference::GetViewMask(DString& ST)
{
	if(VarPoint)
	{
		Variable* Vr=(Variable*)VarPoint;
		ST.Add(Vr->Name);ST.Add("[]");
	}
	else
	{
		ST.Add(GetTextByID("NumericalArray"));//ST.Add("Variable Accept int");
	}
}
//==================================================================================================================//
Op_Add::Op_Add()
{}
Op_Add::~Op_Add()
{}
bool Op_Add::Preview(DString& ST,word Shift)
{
	if(Op1.Get())
	Op1.Get()->Preview(ST,Shift);
	ST.Add("+");
	if(Op2.Get())
	Op2.Get()->Preview(ST,Shift);
	return true;
}
void Op_Add::GetViewMask(DString& ST)
{
	static Enumerator* E=ENUM.Get("ARIPHMETICALOPS");
	if(Op<E->GetAmount())
	{	
		ST.Add("<0>");
		if(Op==3)ST.Add("/");
		else ST.Add(E->GetStr(Op));
		ST.Add("<1>");
	}
}
void Op_Add::GetShortViewMask(DString& ST)
{
	ST.Add("<A>+<B>");
}
int Op_Add::GetNArguments()
{
	return 2;
}
Operand* Op_Add::GetArgument(int NIndex)
{
	if(NIndex==0) 
		return Op1.Get();
	if(NIndex==1)
		return Op2.Get();
	return NULL;
}
void Op_Add::SetArgument(int Index, Operand* Op)
{
	if(Index==0)
	{
		if(Op1.Get())
			delete (Op1.Get());
		Op1.Set((NumericalReturner*)Op);
	}
	if(Index==1)
	{
		if(Op2.Get())
			delete (Op2.Get());
		Op2.Set((NumericalReturner*)Op);
	}
}
bool Op_Add::Get(BaseType* BT)
{
	bool rez=false;
	if(Op1.Get()&&Op2.Get())
	{
		NumericalType* NT = safe_dynamic_cast<NumericalType*> (BT);
		if(NT)
		{
			NumericalType NT1;
			NumericalType NT2;
			if(Op1.Get()->Get(&NT1)&&Op2.Get()->Get(&NT2))
			{		
				int p1=NT1.Value;
				int p2=NT2.Value;

				if(Op==0)
				NT->Value=p1+p2;
				else
				if(Op==1)
				NT->Value=p1-p2;
				else
				if(Op==2)
				NT->Value=p1*p2;
				else
				if(Op==3&&p2)
				NT->Value=p1/p2;

				rez=true;
			}
		}
	}
	return rez;
}
//==================================================================================================================//
void UnarMinus::GetViewMask(DString& ST)
{
	ST.Add("-<0>");
}
int UnarMinus::GetNArguments()
{
	return 1;
}
Operand* UnarMinus::GetArgument(int NIndex)
{
	if(NIndex==0)
	{
		return Op1.Get();
	}
	return NULL;
}
void UnarMinus::SetArgument(int NIndex, Operand* Op)
{
	if(NIndex==0)
	{
		if(Op1.Get())
			delete (Op1.Get());
		Op1.Set((NumericalReturner*)Op);
	}
}
bool UnarMinus::Get(BaseType* BT)
{
	bool rez=false;
	if(Op1.Get())
	{
		NumericalType* NT = safe_dynamic_cast<NumericalType*> (BT);
		if(NT)
		{
			NumericalType NT1;
			if(Op1.Get()->Get(&NT1))
			{
				NT->Value=-NT1.Value;
				rez=true;
			}
		}
	}
	return rez;
}
//==================================================================================================================//
void Brackets::GetViewMask(DString& ST)
{
	ST.Add("(<0>)");
}
int Brackets::GetNArguments()
{
	return 1;
}
Operand* Brackets::GetArgument(int NIndex)
{
	if(NIndex==0)
	{
		return Op1.Get();
	}
	return NULL;
}
void Brackets::SetArgument(int NIndex, Operand* Op)
{
	if(NIndex==0)
	{
		if(Op1.Get())
			delete (Op1.Get());
		Op1.Set((NumericalReturner*)Op);
	}
}
bool Brackets::Get(BaseType* BT)
{
	bool rez=false;
	if(Op1.Get())
	{
		if(Op1.Get()->Get(BT))
		{
			rez=true;
		}
	}
	return rez;
}
//==================================================================================================================//
NumericalArrayElement::NumericalArrayElement()
{
	Arr=0;
}
void NumericalArrayElement::GetViewMask(DString& ST)
{
	ST.Add("<0>[<1>]");
}
int NumericalArrayElement::GetNArguments()
{
	return 2;
}
Operand* NumericalArrayElement::GetArgument(int NIndex)
{
	Operand* rez=NULL;
	if(NIndex==0)
		rez=(Operand*)Arr;
	if(NIndex==1)
		rez=(Operand*)ArrIndex.Get();
	return rez;
}
void NumericalArrayElement::SetArgument(int Index, Operand* Op)
{
	if(Index==1)
	{
		if(ArrIndex.Get())
			delete (ArrIndex.Get());
		ArrIndex.Set((NumericalReturner*)Op);
	}
}
bool NumericalArrayElement::Get(BaseType* BT)
{
	bool rez=false;
	if(Arr&&ArrIndex.Get())
	{
		NumericalType NTi;
		ArrIndex.Get()->Get(&NTi);
		NumericalArray* NA=(NumericalArray*)((Variable*)Arr)->VarType.Get();
		if(NA)
			rez=NA->GetElement(NTi.Value,BT);
	}
	return rez;
}
bool NumericalArrayElement::Set(BaseType* BT)
{
	bool rez=false;
	if(Arr&&ArrIndex.Get())
	{
		NumericalType NTi;
		ArrIndex.Get()->Get(&NTi);
		NumericalArray* NA=(NumericalArray*)((Variable*)Arr)->VarType.Get();
		if(NA)
			rez=NA->SetElement(NTi.Value,BT);
	}
	return rez;
}
//==================================================================================================================//
void Assignment::GetViewMask(DString& ST)
{
	ST.Add("<0>=<1>");
}
int Assignment::GetNArguments()
{
	return 2;
}
Operand* Assignment::GetArgument(int NIndex)
{
	if(NIndex==0)
		return (Operand*)Op1.Get();
	if(NIndex==1)
		return (Operand*)Op2.Get();
	return NULL;
}
void Assignment::SetArgument(int index, Operand* Op)
{
	if(index==0)
	{
		if(Op1.Get())
			delete (Op1.Get());
		Op1.Set((Acceptor*)Op);
	}
	if(index==1)
	{
		if(Op2.Get())
			delete (Op2.Get());
		Op2.Set((Returner*)Op);
	}
}
bool Assignment::Execute()
{
	bool rez=false;
	if(Op1.Get()&&Op2.Get()&&Op1.Get()->GetArgumentType())
	{
		if(Op2.Get()->Get(Op1.Get()->GetArgumentType()))
		{
			if(Op1.Get()->Set(Op1.Get()->GetArgumentType()))
			{
				rez=true;
			}
		}
	}
	return rez;
}
//==================================================================================================================//
bool If_Else::Execute()
{
	int rez=0;
	if(Op1.Get())
	{
		BoolType Boo;
		if(Op1.Get()->Get(&Boo))
		{
			if(Boo.Value)
			{
				OB1.Execute();
			}
			else
			{
				OB2.Execute();
			}
			return true;
		}
	}
	return false;
}
void If_Else::GetViewMask(DString& ST)
{
	ST.Add(GetTextByID("if_defini"));//ST.Add("if (<0>)\\<1>");
	if(OB2.GetNArguments())
	{
		ST.Add(GetTextByID("if_else"));//ST.Add("else\\<2>");
	}
	ST.Add(GetTextByID("if_endif"));//ST.Add("endif");
}
int If_Else::GetNArguments()
{
	return 3;
}
Operand* If_Else::GetArgument(int NIndex)
{
	if(NIndex==0)
		return Op1.Get();
	if(NIndex==1)
		return &OB1;
	if(NIndex==2)
		return &OB2;
	return NULL;
}
void If_Else::SetArgument(int index, Operand* Op)
{
	if(index==0)
	{
		if(Op1.Get())
			delete (Op1.Get());
		Op1.Set((BoolReturner*)Op);
	}
}
const char* If_Else::GetElementView(int Index,const char* LocalName){
	if(Index==0)return BaseClass::GetElementView(0,LocalName);
	if(Index==1)return "then";
	else return "else";
}
//==================================================================================================================//
ForEachNumerical::ForEachNumerical()
{
	
}
bool ForEachNumerical::Execute()
{
	bool rez=false;
	int n=OB.GetNArguments();
	Variable* NAv=(Variable*)NumericalArr.VarPoint;
	Variable* NVv=(Variable*)NumericalVar.VarPoint;
	NumericalArray* NA = safe_dynamic_cast<NumericalArray*>(NAv->VarType.Get());
	NumericalType* NV = safe_dynamic_cast<NumericalType*>(NVv->VarType.Get());
	NumericalType* Temp=NV;
	if(NA&&NV)
	{
		int m=NA->GetNElements();
		for(int i=0;i<m;i++)
		{
			NVv->VarType.Set(NA->Values[i]);
			OB.Execute();
		}
		NVv->VarType.Set(Temp);
	}
	return rez;
}
void ForEachNumerical::GetViewMask(DString& ST)
{
	ST.Add("ForEach <0> as <1> do\\<2>EndForEach");
}
int ForEachNumerical::GetNArguments()
{
	return 3;
}
Operand* ForEachNumerical::GetArgument(int NIndex)
{
	Operand* rez=NULL;
	if(NIndex==0)rez=(NumericalAcceptor*)&NumericalArr;
	if(NIndex==1)rez=(NumericalAcceptor*)&NumericalVar;
	if(NIndex==2)rez=(Operand*)&OB;
	return rez;
}
//==================================================================================================================//
ForFromTo::ForFromTo()
{
}
bool ForFromTo::Execute()
{
	bool rez=false;
	if(From.Get()&&To.Get()&&OB.GetNArguments())
	{
		Variable* Varr=(Variable*)NumericalVar.VarPoint;
		NumericalType* NV = safe_dynamic_cast<NumericalType*>(Varr->VarType.Get());
		NumericalType NFrom;
		NumericalType NTo;
		From.Get()->Get(&NFrom);
		To.Get()->Get(&NTo);
		for(int i=NFrom.Value;i<=NTo.Value;i++)
		{
			NFrom.Value=i;
			NV->Value=i;
			OB.Execute();
			To.Get()->Get(&NTo);
		}
	}
	return rez;
}
void ForFromTo::GetViewMask(DString& ST)
{
	ST.Add("From <0>=<1> to <2> do\\<3>EndFor");
}
int ForFromTo::GetNArguments()
{
	return 4;
}
Operand* ForFromTo::GetArgument(int NIndex)
{
	Operand* rez=NULL;
	if(NIndex==0)rez=(NumericalAcceptor*)&NumericalVar;
	if(NIndex==1)rez=(Operand*)From.Get();
	if(NIndex==2)rez=(Operand*)To.Get();
	if(NIndex==3)rez=(Operand*)&OB;
	return rez;
}
//==================================================================================================================//
void Logical::GetViewMask(DString& ST)
{
	static Enumerator* E=ENUM.Get("LOGICALOPS");
	if(Op<E->GetAmount())
	{	
		ST.Add("(<0> ");
		ST.Add(E->GetStr(Op));	
		ST.Add(" <1>)");	
	}
}
int Logical::GetNArguments()
{
	return 2;
}
Operand* Logical::GetArgument(int NIndex)
{
	if(NIndex==0)
		return Op1.Get();
	if(NIndex==1)
		return Op2.Get();
	return NULL;
}
bool Logical::Get(BaseType* BT)
{
	bool rez=false;
	if(Op1.Get()&&Op2.Get())
	{
		BoolType* Boo = safe_dynamic_cast<BoolType*> (BT);
		if(Boo)
		{
			BoolType Boo1, Boo2;
			if(Op1.Get()->Get(&Boo1)&&Op2.Get()->Get(&Boo2))
			{
				switch(Op) 
				{
				case 0:
					Boo->Value=Boo1.Value&&Boo2.Value;
					break;
				case 1:
					Boo->Value=Boo1.Value||Boo2.Value;
					break;
				}
				rez=true;
			}
		}
	}
	return rez;
}
void Logical::SetArgument(int Index, Operand* Op)
{
	if(Index==0)
	{
		if(Op1.Get())
			delete (Op1.Get());
		Op1.Set((BoolReturner*)Op);
	}
	if(Index==1)
	{
		if(Op2.Get())
			delete (Op2.Get());
		Op2.Set((BoolReturner*)Op);
	}
}
//==================================================================================================================//
void CompareFun::GetViewMask(DString& ST)
{
	static Enumerator* E=ENUM.Get("COMPAREOPS");
	if(Op<E->GetAmount())
	{	
		ST.Add("(<0>");
		ST.Add(E->GetStr(Op));
		ST.Add("<1>)");
	}
}
int CompareFun::GetNArguments()
{
	return 2;
}
Operand* CompareFun::GetArgument(int NIndex)
{
	if(NIndex==0)
		return Op1.Get();
	if(NIndex==1)
		return Op2.Get();
	return NULL;
}
bool CompareFun::Get(BaseType* BT)
{
	bool rez=false;
	if(Op1.Get()&&Op2.Get())
	{
		BoolType* Boo = safe_dynamic_cast<BoolType*> (BT);
		if(Boo)
		{
			NumericalType NT1, NT2;
			if(Op1.Get()->Get(&NT1)&&Op2.Get()->Get(&NT2))
			{
				switch(Op) 
				{
				case 0:
					Boo->Value=NT1.Value==NT2.Value;
					break;
				case 1:
					Boo->Value=NT1.Value>NT2.Value;
					break;
				case 2:
					Boo->Value=NT1.Value<NT2.Value; 
					break;
				case 3:
					Boo->Value=NT1.Value>=NT2.Value;
					break;
				case 4:
					Boo->Value=NT1.Value<=NT2.Value;
					break;
				case 5:
					Boo->Value=NT1.Value!=NT2.Value;
					break;			
				}
				rez=true;
			}
		}
	}
	return rez;
}
void CompareFun::SetArgument(int Index, Operand* Op)  
{
	if(Index==0)
	{
		if(Op1.Get())
			delete (Op1.Get());
		Op1.Set((NumericalReturner*)Op);
	}
	if(Index==1)
	{
		if(Op2.Get())
			delete (Op2.Get());
		Op2.Set((NumericalReturner*)Op);
	}
}
//==================================================================================================================//
void OneStringExpressions::GetViewMask(DString& ST)
{
	ST.Add("<0>");
}
int OneStringExpressions::GetNArguments()
{
	return 1;
}
Operand* OneStringExpressions::GetArgument(int NIndex)
{
	if(NIndex==0)
		return Op.Get();
	return NULL;
}
bool OneStringExpressions::Get(BaseType* BT)
{
	bool rez=false;
	if(Op.Get())
	{
		NumericalType* Boo = safe_dynamic_cast<NumericalType*> (BT);
		if(Boo)
		{
			rez = Op.Get()->Get(Boo);
		}
	}
	return rez;
}
void OneStringExpressions::SetArgument(int index, Operand* nOp)
{
	if(index==0)
	{
		if(Op.Get())
			delete (Op.Get());
		Op.Set((NumericalReturner*)nOp);
	}
}
//==================================================================================================================//
licsema::licsema()
{
	Add("");
	SecondName="";
}
//==================================================================================================================//
typedef DynArray<void*> ExpandInfo;
bool FindNodesPath(void* Adres,BaseClass* Class, void* Data, ExpandInfo& SavedPath)
{
	if(Adres&&Data)
	{
		if(Adres==Data)
		{
			SavedPath.Add(Data);
			return true;
		}		
		if(Class->CheckDirectCasting())
		{
			BaseClass* B=(BaseClass*)Data;
			int n=B->GetAmountOfElements();
			for(int i=0;i<n;i++)
			{
				if(FindNodesPath(Adres,B->GetElementClass(i),B->GetElementPtr(i,Data),SavedPath))
				{
					SavedPath.Add(Data);
					return true;
				}
			}
		}
	}
	return false;
} 
void SetClassEditorScrollPosTo(void* Adres,ClassEditor* CE)
{
	if(Adres&&CE)
	{
		CE->ExpInf.Clear();
		if(FindNodesPath(Adres,CE->_EdClass,CE->_EdClass,CE->ExpInf))
		{
			CE->ExpInfChanged=1;
			CE->Process();
			int n=CE->ElParams.GetAmount();
			int YY=-1;
			for(int i=0;i<n;i++)
			{
				if(CE->ElParams[i]->BC==Adres)
				{
					YY=CE->ElParams[i]->YPos;
					break;
				}
			}
			if(YY!=-1)
			{
				CE->MainScroller->SPos=YY;
			}
		}
	}
}
//==================================================================================================================//
Enumerator* OneScript::BrigadeFormationType = ENUM.Get("BrigadeFormationTypeEnum");
OneScript::OneScript(){

}
void OneScript::Init()
{
	BrigadesVariables.Init();
	if(BrigadeFormationType)
	{
		if(BrigadeFormationType->GetAmount()==0)
		{
			for(int p=0;p<NEOrders;p++)
			{
				BrigadeFormationType->Add(ElementaryOrders[p].ID,(DWORD)p);
			}
		}
	}
	VAR.RefreshEnumerator();
	BrigadesVariables.RefreshEnumerator();
}

bool OneScript::ReadQppFile(char* file)
{
	bool rez=false;
	DString Text;
	Text.Clear();
	Text.ReadFromFile(file);
	if(Text.L>0)
	{
		CQParser Pars;
		Pars.ParseAndFillOneScript(this,&Text);
		/*
		FillWordList(&Text);
		Point=0;
		if(ReadVariablesList(&VAR))
		{
			MarkVariables();
			VAR.RefreshEnumerator();
			if(ReadVariablesList(&BrigadesVariables))
			{
				BrigadesVariables.RefreshEnumerator();
			}
		}
		ParseOperand(&Script,Point);
		*/
	}
	return rez;
}
bool FillWordList_(DString* DS,ClassArray<licsema> *WordList);
bool OneScript::FillWordList(DString* DS)
{
	return FillWordList_(DS,&Words);
	/*
	if(DS)
	{
		int n=DS->L;
		bool NewWord=false;
		int nwi=0;
		bool NewDigit=false;
		int ndi=0;
		bool NewCom=false;
		int nci=0;
		for(int i=0;i<n;i++)
		{
			if(NewWord)
			{
				if(isalnum(DS->pchar()[i]))
				{
					;
				}
				else
				{
					licsema* Wor = new licsema();
					Wor->Allocate(i-nwi);
					Wor->Type=2;
					strncat(Wor->pchar(),DS->pchar()+nwi,i-nwi);
					Wor->L=i-nwi;
					Words.Add(Wor);
					NewWord=false;
				}
			}
			if((!NewWord)&&isalpha(DS->pchar()[i]))
			{
				nwi=i;
				NewWord=true;
			}

			if(NewDigit)
			{
				if(!isdigit(DS->pchar()[i]))
				{
					licsema* Wor = new licsema();
					Wor->Allocate(i-ndi);
					Wor->Type=1;
					strncat(Wor->pchar(),DS->pchar()+ndi,i-ndi);
					Wor->L=i-ndi;
					Words.Add(Wor);
					NewDigit=false;
				}
			}
			
			if((!NewDigit)&&isdigit(DS->pchar()[i])&&(!NewWord))
			{
				ndi=i;
				NewDigit=true;
				if(NewCom)
				{
					if(DS->pchar()[i-1]=='-')
					{
						NewCom=false;
						ndi--;
					}
				}
			}
			
			bool enn=false;
			if(DS->pchar()[i]=='('||DS->pchar()[i]==')')
			{
				enn=true;
			}
			else
			if((!isalnum(DS->pchar()[i]))&&(!isspace(DS->pchar()[i])))
			{
				if(DS->pchar()[i]=='\"')
				{
					char* st = strstr(DS->pchar()+i+1,"\"");
					if(st)
					{
						int ni = st-(DS->pchar()+i);
						licsema* Wor = new licsema();
						Wor->Allocate(ni);
						Wor->Type=4;
						strncat(Wor->pchar(),DS->pchar()+i+1,ni-1);
						Wor->L=ni-1;
						Words.Add(Wor);
						i=i+ni+1;
					}
				}
				else
				if(!NewCom)
				{
					nci=i;
					NewCom=true;
				}
			}
			else
			{
				enn=true;
			}
			if(enn)
			{
				if(NewCom)
				{
					licsema* Wor = new licsema();
					Wor->Allocate(i-nci);
					Wor->Type=3;
					strncat(Wor->pchar(),DS->pchar()+nci,i-nci);
					Wor->L=i-nci;
					Words.Add(Wor);
					NewCom=false;
				}
			}
			if(DS->pchar()[i]=='('||DS->pchar()[i]==')')
			{
				licsema* Wor = new licsema();
				Wor->Allocate(1);
				Wor->Type=3;
				strncat(Wor->pchar(),DS->pchar()+i,1);
				Words.Add(Wor);
			}

			if(isspace(DS->pchar()[i]))
			{
				if(DS->pchar()[i]==13&&Words.GetAmount()>0)
				{
					if(!(Words.End()->L&&Words.End()->pchar()[0]==13))
					{
						licsema* Wor = new licsema();
						Wor->Add("\n");
						Wor->Type=0;
						Words.Add(Wor);
					}
				}
			}
		}
	}
	return true;
	*/
}
bool OneScript::ReadVariablesList(VariablesList* VL)
{
	bool rez=true;
	DString vart;
	VL->GetViewMask(vart);
	DString Brigvar;
	BrigadesVariables.GetViewMask(Brigvar);
	DString begint;
	Script.GetViewMask(begint);
	bool sem=false;
	int n = Words.GetAmount();
	bool haveend=false;
	BaseType* T=NULL;
	if(strstr(vart.pchar(),Words[Point]->pchar()))
	{
		rez=false;
		Point++;
		for(int i=Point;i<n;i++)
		{
			if(strstr(begint.pchar(),Words[i]->pchar())||strstr(Brigvar.pchar(),Words[i]->pchar()))
			{
				Point=i;
				haveend=true;
				break;
			}
			else
			if(Words[i]->Type==2)
			{
				if(!T)
				{
					T=GetTypeByName(Words[i]->pchar());
					if(!T)
						ErrMsg.Add("Wrong viriable type\n");
				}
				else
				{
					Variable* NV = new Variable();
					NV->Name.Add(Words[i]->pchar());
					if(NV->VarType.Get())
					{
						delete (NV->VarType.Get());
					}
					NV->VarType.Set(T);
					VL->Variables.Add(NV);
					T=NULL;
				}
			}
		}
		if(!haveend)
		{
			ErrMsg.Add("Net nachala scripta\n");
		}
		else
			rez=true;
	}
	return rez;
}
void OneScript::MarkVariables()
{
	int n=VAR.Variables.GetAmount();
	int m=Words.GetAmount();
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<m;j++)
		{
			if(Words[j]->Type==2)
			{
				if(!strcmp(Words[j]->pchar(),VAR.Variables[i]->Name.pchar()))
				{
					Words[j]->SecondName.Clear();
					Words[j]->SecondName.Add("NumericalVariable");
				}
			}
		}
	}
}
BaseType* OneScript::GetTypeByName(char* T)
{
	BaseType* rez=NULL;
	if(T)
	{
		OneClassStorage* OCS = CGARB.GetClass("BaseType");
		if(OCS&&OCS->OneMemb)
		{
			int n=OCS->Children.GetAmount();
			for(int i=0;i<n;i++)
			{
				OneClassStorage* OCSc = CGARB.GetClass(OCS->Children[i]);
				if(OCSc&&OCSc->OneMemb)
				{
					BaseType* Ch=(BaseType*)OCSc->OneMemb;
					if(!strcmp(T,Ch->GetTypeName()))
					{
						return (BaseType*)Ch->new_element();
					}
				}
			}
		}
	}
	return rez;
}
bool ParseOperand_(Operand* Op,int& point, ClassArray<licsema> *WordList);
bool OneScript::ParseOperand(Operand* Op,int& point)
{
	return ParseOperand_(Op,point,&Words);
	/*
	if(Op)
	{
		int intp=point;
		int maxintp=Words.GetAmount();
		if(intp<maxintp)
		{
			OperandBlock* OB = safe_dynamic_cast<OperandBlock*> (Op);
			Procedure* PR = safe_dynamic_cast<Procedure*> (Op);
			NumericalReturner* NR = safe_dynamic_cast<NumericalReturner*> (Op);
			if(OB)
			{
				int nob = OB->Operands.GetAmountOfEditableElements(&OB->Operands,7);//GetAmountOfElements();
				if(nob==1)
				{
					DString Mas;
					OB->GetViewMask(Mas);
					if(Mas.L>0)
					{
						if(strstr(Mas.pchar(),Words[intp]->pchar()))
						{
							intp++;
							if(Words[intp]->Type==0) //\n
								intp++;
							else
							{
								ErrMsg.Add("dolgen bit \\n \n");
								return false;
							}
						}
						else
						{
							return false;
						}
					}
					ClassExpParams Ep;
					if(OB->Operands.GetEditableElementExpansionList(Ep,0,(void*)(&OB->Operands),7))
					{
						int m=Ep.ExpList.GetAmount();
						bool present=false;
						do 
						{
							present=false;
							for(int j=0;j<m;j++)
							{
								OneClassStorage* OCS = CGARB.GetClass(Ep.ExpList[j]);
								if(OCS&&OCS->OneMemb)
								{
									BaseClass* BC = OCS->OneMemb->new_element();
									Procedure* On = safe_dynamic_cast<Procedure*> (BC);
									if(On)
									{
										if(ParseOperand(On,intp))
										{
											OB->Operands.Add(On);
											j=m;
											present=true;
											if(Words[intp]->Type==0)
												intp++;
											else
											{
												ErrMsg.Add("dolgen bit \\n \n");
												return false;
											}
										}
										else
										{
											delete BC;
										}
									}
								}
							}
						} 
						while(present);
						if(Mas.L>0)
						{
							if(intp>=maxintp)
								return false;
							if(strstr(Mas.pchar(),Words[intp]->pchar()))
							{
								intp++;
								if(Words[intp]->Type==0)
									intp++;
								else
								{
									ErrMsg.Add("dolgen bit \\n \n");
									return false;
								}
								if(intp<maxintp)
									point=intp;
								else
									point=maxintp-1;
								return true;
							}
							else
							{
								return false;
							}
						}
						if(intp<maxintp)
							point=intp;
						else
							point=maxintp-1;
					}
					return true;
				}
			}

			VariableReference* VR = safe_dynamic_cast<VariableReference*> (Op);
			if(VR)
			{
				NumericalVariableReference* NVR = safe_dynamic_cast<NumericalVariableReference*> (Op);
				if(NVR)
				{
					if(Words[intp]->SecondName=="NumericalVariable")
					{
						Enumerator* Enm = ENUM.Get("NumericalTypeEnum");
						if(Enm)
						{
							int n=Enm->Get(Words[intp]->pchar());
							if(n!=-1)
							{
								NVR->VarPoint=n;
								intp++;
								if(intp<maxintp)
									point=intp;
								else
									point=maxintp;
								return true;
							}
						}
					}
				}
				return false;
			}

			StringConst* SC = safe_dynamic_cast<StringConst*> (Op);
			if(SC)
			{
				if(Words[intp]->Type==4)
				{
					SC->Value.String.Add(Words[intp]->pchar());
					intp++;
					if(intp<maxintp)
						point=intp;
					else
						point=maxintp;
					return true;
				}
				return false;
			}
			NumericalConst* NC = safe_dynamic_cast<NumericalConst*> (Op);
			if(NC)
			{
				if(Words[intp]->Type==1)
				{
					NC->Value.Value=atoi(Words[intp]->pchar());
					intp++;
					if(intp<maxintp)
						point=intp;
					else
						point=maxintp;
					return true;
				}
				return false;
			}
			NumericalArrayElement* NAE = safe_dynamic_cast<NumericalArrayElement*> (Op);
			if(NAE)
			{
				return false;
			}
			
			If_Else* IfE = safe_dynamic_cast<If_Else*> (Op);
			if(IfE)
			{
				ClassArray<licsema> L1;
				ClassArray<licsema> L2;
				ClassArray<licsema> L3;
				DString D1;D1.Add(GetTextByID("if_defini"));
				DString D2;D2.Add(GetTextByID("if_else"));
				DString D3;D3.Add(GetTextByID("if_endif"));
				ParseDString(D1,&L1);
				ParseDString(D2,&L2);
				ParseDString(D3,&L3);
				int p=0;
				if(!strcmp(L1[0]->pchar(),Words[intp]->pchar()))//If
				{
					intp++;
					if(!strcmp(L1[1]->pchar(),Words[intp]->pchar()))//(
					{
						intp++;
						if(L1[2]->Type==99)// <0>
						{
							if(FindAndSetEditableElement(IfE,0,intp,0))
							{
								if(!strcmp(L1[3]->pchar(),Words[intp]->pchar()))//)
								{
									intp++;
									if(!strcmp(L1[4]->pchar(),Words[intp]->pchar()))//\n
									{
										intp++;
										if(ParseOperand((Operand*)(&IfE->OB1),intp))// Block
										{
											if(!strcmp(L2[0]->pchar(),Words[intp]->pchar()))// else
											{
												intp++;
												if(!strcmp(L2[1]->pchar(),Words[intp]->pchar()))// \n
												{
													intp++;
													if(ParseOperand((Operand*)(&IfE->OB2),intp))// Block2
													{;}
													else
														return false;
												}
											}
											if(!strcmp(L3[0]->pchar(),Words[intp]->pchar())) // endif
											{
												intp++;
												point=intp;
												return true;
											}
										}
									}
								}
							}
						}
					}
				}
				return false;
			}
			BVariables* BVR = safe_dynamic_cast<BVariables*> (Op);
			if(BVR)
			{
				ClassArray<licsema> LL;
				ParseViewMask(Op,&LL);
				if(!strcmp(LL[0]->pchar(),Words[intp]->pchar()))
				{
					intp++;
					if(!strcmp(LL[1]->pchar(),Words[intp]->pchar()))
					{
						intp++;
						static Enumerator* E_BVR = ENUM.Get("BrigadeVariablesListEnum");
						int nen=E_BVR->Get(Words[intp]->pchar());
						if(nen!=-1)
						{
							intp++;
							BVR->VarPoint=nen;
							point=intp;
							return true;
						}
					}
				}
				return false;
			}
			Op_Add* OpA = safe_dynamic_cast<Op_Add*> (Op);
			Logical* Log = safe_dynamic_cast<Logical*> (Op);
			CompareFun* CF = safe_dynamic_cast<CompareFun*> (Op);
			//if(PR)
			{
				bool secses=true;
				ClassArray<licsema> LL;
				ParseViewMask(Op,&LL);
				int EditblIndex=0;
				int EditblAmo=Op->GetAmountOfElements(); 
				int nll=LL.GetAmount();
				if(nll==0)return false;
				for(int i=0;i<nll&&secses;i++)
				{
					if(LL[i]->Type==99)
					{
						int np;
						if(sscanf(LL[i]->pchar(),"%d",&np)==1)
						{
							BaseClass* Ar = Op->GetArgument(np);
							if(Ar)
							{
								secses=ParseOperand((Operand*)Ar,intp);
							}
							else
							{
								bool find=false;
								for(int k=EditblIndex;k<EditblAmo&&(!find);k++)
								{
									find=FindAndSetEditableElement(Op,k,intp,np);
									EditblIndex=k+1;
								}
								if(!find)
								{
									return false;
								}
							}
						}
					}
					else
					{
						if(strcmp(LL[i]->pchar(),Words[intp]->pchar()))
						{
							if(OpA)
							{
								Enumerator* Enm = ENUM.Get("ARIPHMETICALOPS");
								int en = Enm->FindStr(Words[intp]->pchar());
								if(en!=-1)
								{
									OpA->Op=en;
								}
								else
									return false;
							}
							else
							if(Log)
							{
								Enumerator* Enm = ENUM.Get("LOGICALOPS");
								int en = Enm->FindStr(Words[intp]->pchar());
								if(en!=-1)
								{
									Log->Op=en;
								}
								else
									return false;
							}
							else
							if(CF)
							{
								Enumerator* Enm = ENUM.Get("COMPAREOPS");
								int en = Enm->FindStr(Words[intp]->pchar());
								if(en!=-1)
								{
									CF->Op=en;
								}
								else
									return false;
							}
							else
							return false;
						}
						intp++;
					}
				}
				if(secses)
				{
					if(intp<maxintp)
						point=intp;
					else
						point=maxintp;
					return true;
				}
			}
		}
	}
	return false;
}
bool ParseViewMask_(Operand* Op,ClassArray<licsema> *W);
bool OneScript::ParseViewMask(Operand* Op,ClassArray<licsema> *W)
{
	return ParseViewMask_(Op,W);
	/*
	bool rez=false;
	if(Op)
	{
		DString Mask;
		Op->GetViewMask(Mask);
		rez=ParseDString(Mask,W);
	}
	return rez;
	*/
}
bool ParseDString_(DString& Mask,ClassArray<licsema> *W);
bool OneScript::ParseDString(DString& Mask,ClassArray<licsema> *W)
{
	return ParseDString_(Mask,W);
	/*
	bool rez=false;
	if(W)
	{
		W->Clear();
		Mask.Add(" ");
		bool NewWord=false;
		int nwi=0;
		bool NewDigit=false;
		int ndi=0;
		bool NewCom=false;
		int nci=0;
		bool NewIdent=false;
		int ni=0;
		for(int i=0;i<Mask.L;i++)
		{
			if(NewWord)
			{
				if(isalnum(Mask.pchar()[i]))
				{
					;
				}
				else
				{
					licsema* Wor = new licsema();
					Wor->Allocate(i-nwi);
					Wor->Type=2;
					strncat(Wor->pchar(),Mask.pchar()+nwi,i-nwi);
					Wor->L=i-nwi;
					W->Add(Wor);
					NewWord=false;
				}
			}
			if((!NewWord)&&isalpha(Mask.pchar()[i]))
			{
				nwi=i;
				NewWord=true;
			}

			if(NewDigit)
			{
				if(!isdigit(Mask.pchar()[i]))
				{
					licsema* Wor = new licsema();
					Wor->Allocate(i-ndi);
					Wor->Type=1;
					strncat(Wor->pchar(),Mask.pchar()+ndi,i-ndi);
					Wor->L=i-ndi;
					W->Add(Wor);
					NewDigit=false;
				}
			}
			if((!NewDigit)&&isdigit(Mask.pchar()[i])&&(!NewWord)&&(!NewIdent))
			{
				ndi=i;
				NewDigit=true;
			}

			bool endcom=false;
			bool slesh=false;
			if((!isalnum(Mask.pchar()[i]))&&(!isspace(Mask.pchar()[i])))
			{
				if(Mask.pchar()[i]=='<')
				{
					ni=i;
					NewIdent=true;
					endcom=true;
				}
				if(Mask.pchar()[i]=='\\')
				{
					slesh=true;
				}
				else
					if((!NewCom)&&(!NewIdent))
					{
						if(Mask.pchar()[i]=='('||Mask.pchar()[i]==')')
						{
							licsema* Wor = new licsema();
							Wor->Allocate(1);
							Wor->Type=3;
							strncat(Wor->pchar(),Mask.pchar()+i,1);
							W->Add(Wor);
						}
						else
						{
							nci=i;
							NewCom=true;
						}
					}
					if(NewIdent&&Mask.pchar()[i]=='>')
					{
						licsema* Wor = new licsema();
						Wor->Allocate(i-ni);
						Wor->Type=99;
						strncat(Wor->pchar(),Mask.pchar()+ni+1,i-ni-1);
						Wor->L=i-ni-1;
						W->Add(Wor);
						NewIdent=false;
					}
			}
			else
				endcom=true;

			if(NewCom&&endcom)
			{
				licsema* Wor = new licsema();
				Wor->Allocate(i-nci);
				Wor->Type=3;
				strncat(Wor->pchar(),Mask.pchar()+nci,i-nci);
				Wor->L=i-nci;
				W->Add(Wor);
				NewCom=false;
			}
			if(slesh)
			{
				licsema* Wor = new licsema();
				Wor->Add("\n");
				Wor->Type=0;
				W->Add(Wor);
				endcom=true;
			}
		}
	}
	return rez;
	*/
}
bool FindAndSetEditableElement_(Operand* Op, int ElementIndex, int& Np, int ArgumentIndex, ClassArray<licsema> *WordList);
bool OneScript::FindAndSetEditableElement(Operand* Op, int ElementIndex, int& Np, int ArgumentIndex)
{
	return FindAndSetEditableElement_(Op, ElementIndex, Np, ArgumentIndex, &Words);
	/*
	bool rez=false;
	if(Op)
	{
		ClassExpParams Ep;
		BaseClass* Arg = Op->GetElementClass(ElementIndex);
		void* ArgPtr = Op->GetElementExtraPtr(ElementIndex);
		if(Arg->GetEditableElementExpansionList(Ep,0,ArgPtr,7))
		{
			int m=Ep.ExpList.GetAmount();
			for(int j=0;j<m&&(!rez);j++)
			{
				OneClassStorage* OCS = CGARB.GetClass(Ep.ExpList[j]);
				if(OCS&&OCS->OneMemb)
				{
					BaseClass* BC = OCS->OneMemb->new_element();
					Operand* On = safe_dynamic_cast<Operand*> (BC);
					if(On&&ParseOperand(On,Np))
					{
						rez=true;
						Op->SetArgument(ArgumentIndex,On);
					}
					else
					{
						delete BC;
					}
				}
			}
		}
	}
	return rez;
	*/
}
//==================================================================================================================//
bool FillWordList_(DString* DS,ClassArray<licsema> *WordList)
{
	if(DS&&WordList)
	{
		int n=DS->L;
		bool NewWord=false;
		int nwi=0;
		bool NewDigit=false;
		int ndi=0;
		bool NewCom=false;
		int nci=0;
		for(int i=0;i<n;i++)
		{
			if(NewWord)
			{
				if(isalnum(DS->pchar()[i]))
				{
					;
				}
				else
				{
					licsema* Wor = new licsema();
					Wor->Allocate(i-nwi);
					Wor->Type=2;
					strncat(Wor->pchar(),DS->pchar()+nwi,i-nwi);
					Wor->L=i-nwi;
					WordList->Add(Wor);
					NewWord=false;
				}
			}
			if((!NewWord)&&isalpha(DS->pchar()[i]))
			{
				nwi=i;
				NewWord=true;
			}

			if(NewDigit)
			{
				if(!isdigit(DS->pchar()[i]))
				{
					licsema* Wor = new licsema();
					Wor->Allocate(i-ndi);
					Wor->Type=1;
					strncat(Wor->pchar(),DS->pchar()+ndi,i-ndi);
					Wor->L=i-ndi;
					WordList->Add(Wor);
					NewDigit=false;
				}
			}

			if((!NewDigit)&&isdigit(DS->pchar()[i])&&(!NewWord))
			{
				ndi=i;
				NewDigit=true;
				if(NewCom)
				{
					if(DS->pchar()[i-1]=='-')
					{
						NewCom=false;
						ndi--;
					}
				}
			}

			bool enn=false;
			if(DS->pchar()[i]=='('||DS->pchar()[i]==')')
			{
				enn=true;
			}
			else
				if((!isalnum(DS->pchar()[i]))&&(!isspace(DS->pchar()[i])))
				{
					if(DS->pchar()[i]=='\"')
					{
						char* st = strstr(DS->pchar()+i+1,"\"");
						if(st)
						{
							int ni = st-(DS->pchar()+i);
							licsema* Wor = new licsema();
							Wor->Allocate(ni);
							Wor->Type=4;
							strncat(Wor->pchar(),DS->pchar()+i+1,ni-1);
							Wor->L=ni-1;
							WordList->Add(Wor);
							i=i+ni+1;
						}
					}
					else
						if(!NewCom)
						{
							nci=i;
							NewCom=true;
						}
				}
				else
				{
					enn=true;
				}
				if(enn)
				{
					if(NewCom)
					{
						licsema* Wor = new licsema();
						Wor->Allocate(i-nci);
						Wor->Type=3;
						strncat(Wor->pchar(),DS->pchar()+nci,i-nci);
						Wor->L=i-nci;
						WordList->Add(Wor);
						NewCom=false;
					}
				}
				if(DS->pchar()[i]=='('||DS->pchar()[i]==')')
				{
					licsema* Wor = new licsema();
					Wor->Allocate(1);
					Wor->Type=3;
					strncat(Wor->pchar(),DS->pchar()+i,1);
					WordList->Add(Wor);
				}

				if(isspace(DS->pchar()[i]))
				{
					if(DS->pchar()[i]==13&&WordList->GetAmount()>0)
					{
						if(!(WordList->End()->L&&WordList->End()->pchar()[0]==13))
						{
							licsema* Wor = new licsema();
							Wor->Add("\n");
							Wor->Type=0;
							WordList->Add(Wor);
						}
					}
				}
		}
	}
	return true;
}
bool ParseDString_(DString& Mask,ClassArray<licsema> *W)
{
	bool rez=false;
	if(W)
	{
		W->Clear();
		Mask.Add(" ");
		bool NewWord=false;
		int nwi=0;
		bool NewDigit=false;
		int ndi=0;
		bool NewCom=false;
		int nci=0;
		bool NewIdent=false;
		int ni=0;
		for(int i=0;i<Mask.L;i++)
		{
			if(NewWord)
			{
				if(isalnum(Mask.pchar()[i]))
				{
					;
				}
				else
				{
					licsema* Wor = new licsema();
					Wor->Allocate(i-nwi);
					Wor->Type=2;
					strncat(Wor->pchar(),Mask.pchar()+nwi,i-nwi);
					Wor->L=i-nwi;
					W->Add(Wor);
					NewWord=false;
				}
			}
			if((!NewWord)&&isalpha(Mask.pchar()[i]))
			{
				nwi=i;
				NewWord=true;
			}

			if(NewDigit)
			{
				if(!isdigit(Mask.pchar()[i]))
				{
					licsema* Wor = new licsema();
					Wor->Allocate(i-ndi);
					Wor->Type=1;
					strncat(Wor->pchar(),Mask.pchar()+ndi,i-ndi);
					Wor->L=i-ndi;
					W->Add(Wor);
					NewDigit=false;
				}
			}
			if((!NewDigit)&&isdigit(Mask.pchar()[i])&&(!NewWord)&&(!NewIdent))
			{
				ndi=i;
				NewDigit=true;
			}

			bool endcom=false;
			bool slesh=false;
			if((!isalnum(Mask.pchar()[i]))&&(!isspace(Mask.pchar()[i])))
			{
				if(Mask.pchar()[i]=='<')
				{
					ni=i;
					NewIdent=true;
					endcom=true;
				}
				if(Mask.pchar()[i]=='\\')
				{
					slesh=true;
				}
				else
					if((!NewCom)&&(!NewIdent))
					{
						if(Mask.pchar()[i]=='('||Mask.pchar()[i]==')')
						{
							licsema* Wor = new licsema();
							Wor->Allocate(1);
							Wor->Type=3;
							strncat(Wor->pchar(),Mask.pchar()+i,1);
							W->Add(Wor);
						}
						else
						{
							nci=i;
							NewCom=true;
						}
					}
					if(NewIdent&&Mask.pchar()[i]=='>')
					{
						licsema* Wor = new licsema();
						Wor->Allocate(i-ni);
						Wor->Type=99;
						strncat(Wor->pchar(),Mask.pchar()+ni+1,i-ni-1);
						Wor->L=i-ni-1;
						W->Add(Wor);
						NewIdent=false;
					}
			}
			else
				endcom=true;

			if(NewCom&&endcom)
			{
				licsema* Wor = new licsema();
				Wor->Allocate(i-nci);
				Wor->Type=3;
				strncat(Wor->pchar(),Mask.pchar()+nci,i-nci);
				Wor->L=i-nci;
				W->Add(Wor);
				NewCom=false;
			}
			if(slesh)
			{
				licsema* Wor = new licsema();
				Wor->Add("\n");
				Wor->Type=0;
				W->Add(Wor);
				endcom=true;
			}
		}
	}
	return rez;
}
bool ParseViewMask_(Operand* Op,ClassArray<licsema> *W)
{
	bool rez=false;
	if(Op)
	{
		DString Mask;
		Op->GetViewMask(Mask);
		rez=ParseDString_(Mask,W);
	}
	return rez;
}
bool FindAndSetEditableElement_(Operand* Op, int ElementIndex, int& Np, int ArgumentIndex, ClassArray<licsema> *WordList);
bool ParseOperand_(Operand* Op,int& point, ClassArray<licsema> *WordList)
{
	if(Op)
	{
		int intp=point;
		int maxintp=WordList->GetAmount();
		if(intp<maxintp)
		{
			OperandBlock* OB = safe_dynamic_cast<OperandBlock*> (Op);
			Procedure* PR = safe_dynamic_cast<Procedure*> (Op);
			NumericalReturner* NR = safe_dynamic_cast<NumericalReturner*> (Op);
			if(OB)
			{
				int nob = OB->Operands.GetAmountOfEditableElements(&OB->Operands,7);//GetAmountOfElements();
				if(nob==1)
				{
					DString Mas;
					OB->GetViewMask(Mas);
					if(Mas.L>0)
					{
						if(strstr(Mas.pchar(),(*WordList)[intp]->pchar()))
						{
							intp++;
							if((*WordList)[intp]->Type==0) //\n
								intp++;
							else
							{
								//ErrMsg.Add("dolgen bit \\n \n");
								return false;
							}
						}
						else
						{
							return false;
						}
					}
					ClassExpParams Ep;
					if(OB->Operands.GetEditableElementExpansionList(Ep,0,(void*)(&OB->Operands),7))
					{
						int m=Ep.ExpList.GetAmount();
						bool present=false;
						do 
						{
							present=false;
							for(int j=0;j<m;j++)
							{
								OneClassStorage* OCS = CGARB.GetClass(Ep.ExpList[j]);
								if(OCS&&OCS->OneMemb)
								{
									BaseClass* BC = OCS->OneMemb->new_element();
									Procedure* On = safe_dynamic_cast<Procedure*> (BC);
									if(On)
									{
										if(ParseOperand_(On,intp,WordList))
										{
											OB->Operands.Add(On);
											j=m;
											present=true;
											if((*WordList)[intp]->Type==0)
												intp++;
											else
											{
												//ErrMsg.Add("dolgen bit \\n \n");
												return false;
											}
										}
										else
										{
											delete BC;
										}
									}
								}
							}
						} 
						while(present);
						if(Mas.L>0)
						{
							if(intp>=maxintp)
								return false;
							if(strstr(Mas.pchar(),(*WordList)[intp]->pchar()))
							{
								intp++;
								if((*WordList)[intp]->Type==0)
									intp++;
								else
								{
									//ErrMsg.Add("dolgen bit \\n \n");
									return false;
								}
								if(intp<maxintp)
									point=intp;
								else
									point=maxintp-1;
								return true;
							}
							else
							{
								return false;
							}
						}
						if(intp<maxintp)
							point=intp;
						else
							point=maxintp-1;
					}
					return true;
				}
			}

			VariableReference* VR = safe_dynamic_cast<VariableReference*> (Op);
			if(VR)
			{
				NumericalVariableReference* NVR = safe_dynamic_cast<NumericalVariableReference*> (Op);
				if(NVR)
				{
					if((*WordList)[intp]->SecondName=="NumericalVariable")
					{
						Enumerator* Enm = ENUM.Get("NumericalTypeEnum");
						if(Enm)
						{
							int n=Enm->Get((*WordList)[intp]->pchar());
							if(n!=-1)
							{
								NVR->VarPoint=n;
								intp++;
								if(intp<maxintp)
									point=intp;
								else
									point=maxintp;
								return true;
							}
						}
					}
				}
				return false;
			}

			StringConst* SC = safe_dynamic_cast<StringConst*> (Op);
			if(SC)
			{
				if((*WordList)[intp]->Type==4)
				{
					SC->Value.String.Add((*WordList)[intp]->pchar());
					intp++;
					if(intp<maxintp)
						point=intp;
					else
						point=maxintp;
					return true;
				}
				return false;
			}
			NumericalConst* NC = safe_dynamic_cast<NumericalConst*> (Op);
			if(NC)
			{
				if((*WordList)[intp]->Type==1)
				{
					NC->Value.Value=atoi((*WordList)[intp]->pchar());
					intp++;
					if(intp<maxintp)
						point=intp;
					else
						point=maxintp;
					return true;
				}
				return false;
			}
			NumericalArrayElement* NAE = safe_dynamic_cast<NumericalArrayElement*> (Op);
			if(NAE)
			{
				return false;
			}

			If_Else* IfE = safe_dynamic_cast<If_Else*> (Op);
			if(IfE)
			{
				ClassArray<licsema> L1;
				ClassArray<licsema> L2;
				ClassArray<licsema> L3;
				DString D1;D1.Add(GetTextByID("if_defini"));
				DString D2;D2.Add(GetTextByID("if_else"));
				DString D3;D3.Add(GetTextByID("if_endif"));
				ParseDString_(D1,&L1);
				ParseDString_(D2,&L2);
				ParseDString_(D3,&L3);
				int p=0;
				if(!strcmp(L1[0]->pchar(),(*WordList)[intp]->pchar()))//If
				{
					intp++;
					if(!strcmp(L1[1]->pchar(),(*WordList)[intp]->pchar()))//(
					{
						intp++;
						if(L1[2]->Type==99)// <0>
						{
							if(FindAndSetEditableElement_(IfE,0,intp,0,WordList))
							{
								if(!strcmp(L1[3]->pchar(),(*WordList)[intp]->pchar()))//)
								{
									intp++;
									if(!strcmp(L1[4]->pchar(),(*WordList)[intp]->pchar()))//\n
									{
										intp++;
										if(ParseOperand_((Operand*)(&IfE->OB1),intp,WordList))// Block
										{
											if(!strcmp(L2[0]->pchar(),(*WordList)[intp]->pchar()))// else
											{
												intp++;
												if(!strcmp(L2[1]->pchar(),(*WordList)[intp]->pchar()))// \n
												{
													intp++;
													if(ParseOperand_((Operand*)(&IfE->OB2),intp,WordList))// Block2
													{;}
													else
														return false;
												}
											}
											if(!strcmp(L3[0]->pchar(),(*WordList)[intp]->pchar())) // endif
											{
												intp++;
												point=intp;
												return true;
											}
										}
									}
								}
							}
						}
					}
				}
				return false;
			}
			BVariables* BVR = safe_dynamic_cast<BVariables*> (Op);
			if(BVR)
			{
				ClassArray<licsema> LL;
				ParseViewMask_(Op,&LL);
				if(!strcmp(LL[0]->pchar(),(*WordList)[intp]->pchar()))
				{
					intp++;
					if(!strcmp(LL[1]->pchar(),(*WordList)[intp]->pchar()))
					{
						intp++;
						static Enumerator* E_BVR = ENUM.Get("BrigadeVariablesListEnum");
						int nen=E_BVR->Get((*WordList)[intp]->pchar());
						if(nen!=-1)
						{
							intp++;
							BVR->VarPoint=nen;
							point=intp;
							return true;
						}
					}
				}
				return false;
			}
			Op_Add* OpA = safe_dynamic_cast<Op_Add*> (Op);
			Logical* Log = safe_dynamic_cast<Logical*> (Op);
			CompareFun* CF = safe_dynamic_cast<CompareFun*> (Op);
			//if(PR)
			{
				bool secses=true;
				ClassArray<licsema> LL;
				ParseViewMask_(Op,&LL);
				int EditblIndex=0;
				int EditblAmo=Op->GetAmountOfElements(); 
				int nll=LL.GetAmount();
				if(nll==0)return false;
				for(int i=0;i<nll&&secses;i++)
				{
					if(LL[i]->Type==99)
					{
						int np;
						if(sscanf(LL[i]->pchar(),"%d",&np)==1)
						{
							BaseClass* Ar = Op->GetArgument(np);
							if(Ar)
							{
								secses=ParseOperand_((Operand*)Ar,intp,WordList);
							}
							else
							{
								bool find=false;
								for(int k=EditblIndex;k<EditblAmo&&(!find);k++)
								{
									find=FindAndSetEditableElement_(Op,k,intp,np,WordList);
									EditblIndex=k+1;
								}
								if(!find)
								{
									return false;
								}
							}
						}
					}
					else
					{
						if(LL[i]->pchar()&&WordList->GetAmount()>intp&&strcmp(LL[i]->pchar(),(*WordList)[intp]->pchar()))
						{
							if(OpA)
							{
								Enumerator* Enm = ENUM.Get("ARIPHMETICALOPS");
								int en = Enm->FindStr((*WordList)[intp]->pchar());
								if(en!=-1)
								{
									OpA->Op=en;
								}
								else
									return false;
							}
							else
								if(Log)
								{
									Enumerator* Enm = ENUM.Get("LOGICALOPS");
									int en = Enm->FindStr((*WordList)[intp]->pchar());
									if(en!=-1)
									{
										Log->Op=en;
									}
									else
										return false;
								}
								else
									if(CF)
									{
										Enumerator* Enm = ENUM.Get("COMPAREOPS");
										int en = Enm->FindStr((*WordList)[intp]->pchar());
										if(en!=-1)
										{
											CF->Op=en;
										}
										else
											return false;
									}
									else
										return false;
						}
						intp++;
					}
				}
				if(secses)
				{
					if(intp<maxintp)
						point=intp;
					else
						point=maxintp;
					return true;
				}
			}
		}
	}
	return false;
}
bool FindAndSetEditableElement_(Operand* Op, int ElementIndex, int& Np, int ArgumentIndex,ClassArray<licsema> *WordList)
{
	bool rez=false;
	if(Op)
	{
		ClassExpParams Ep;
		BaseClass* Arg = Op->GetElementClass(ElementIndex);
		void* ArgPtr = Op->GetElementExtraPtr(ElementIndex);
		if(Arg&&Arg->GetEditableElementExpansionList(Ep,0,ArgPtr,7))
		{
			int m=Ep.ExpList.GetAmount();
			for(int j=0;j<m&&(!rez);j++)
			{
				OneClassStorage* OCS = CGARB.GetClass(Ep.ExpList[j]);
				if(OCS&&OCS->OneMemb)
				{
					BaseClass* BC = OCS->OneMemb->new_element();
					Operand* On = safe_dynamic_cast<Operand*> (BC);
					if(On&&ParseOperand_(On,Np,WordList))
					{
						rez=true;
						Op->SetArgument(ArgumentIndex,On);
					}
					else
					{
						delete BC;
					}
				}
			}
		}
	}
	return rez;
}
//==================================================================================================================//
bool ParseAndFillExpression(Operand* OP,DString* DS)
{
	bool rez=false;
	if(OP&&DS)
	{
		DS->Add("\n");
		ClassArray<licsema> WordList;
		if(FillWordList_(DS,&WordList))
		{
			int point=0;
			rez=ParseOperand_(OP,point,&WordList);
		}
	}
	return rez;
}
//==================================================================================================================//
bool UserFriendlyNumericalReturner::Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra){
	if(xml.GetNSubQuotes()){
		return BaseClass::Load(xml,ClassPtr,Error,Extra);
	}else if(xml.Get_string() && xml.Get_string()[0]){        
		DString D=(char*)xml.Get_string();
		CQParser Parser;	
		Value.Set(NULL);
		return Parser.ParseAndFillExpression(&((UserFriendlyNumericalReturner*)ClassPtr)->Value,&D);
	}
	return false;
}