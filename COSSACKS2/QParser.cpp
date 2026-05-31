#include "stdheader.h"
#include "QParser.h"
#include <stdio.h>
#include <string.h>



CQParser* QParser;

OperandParamList::OperandParamList()
{
}
OperandParamList::OperandParamList(Operand* OP)
{
	Params.Add(OP);
}
void OperandParamList::Add(Operand* OP)
{
	Params.Add(OP);
}

CQParser::CQParser()
{
	ResultExpression=NULL;
	OneScriptRef=NULL;
}

Operand* CQParser::OnScript(Operand* functionlist)
{
	if(functionlist&&OneScriptRef)
	{
		OperandParamList* On = dynamic_cast<OperandParamList*>(functionlist);
		if(On)
		{
			int n =On->Params.GetAmount();
			for(int i=0;i<n;i++)
			{
				if(On->Params[i])
					OneScriptRef->Script.Operands.Add((Procedure *)(On->Params[i]));
			}
		}
	}
	return NULL;
}
Operand* CQParser::OnExpression(Operand* OP)
{
	if(ResultExpression)
	{
		ResultExpression->SetArgument(0,OP);
	}
	return OP;
}
Operand* CQParser::OnDeclareVariable(const char* Type, const char* Name)
{
	if(Type&&Name)
	{
		BaseType* BT = FindType(Type);
		if(BT&&OneScriptRef)
		{
			Variable* V = new Variable();
			V->Name=Name;
			V->VarType.Set(BT);
			if(strstr(Name,"Brig"))
			{
				OneScriptRef->BrigadesVariables.Variables.Add(V);
				OneScriptRef->BrigadesVariables.RefreshEnumerator();
			}
			else
			{
				OneScriptRef->VAR.Variables.Add(V);
			}
		}
	}
	return NULL;
}
Operand* CQParser::OnVariable(const char* Name)
{
	Operand* rez=NULL;
	if(Name)
	{
		Variable* V = FindVariableByName(Name);
		if(V)
		{
			NumericalVariableReference* VR = new NumericalVariableReference();
			VR->VarPoint=(int)V;
			rez=(Operand*)((NumericalAcceptor*)VR);
		}
	}
	return rez;
}
Operand* CQParser::OnBrigadeVariable(const char* Type, const char* Name)
{
	return NULL;
}
Operand* CQParser::OnProcedure(const char* Name, Operand* argumentlist)
{
	if(Name)
	{
		Procedure* P = FindProcedure(Name);
		if(P)
		{
			 P->SetArguments(argumentlist);
			return (Operand*) P;
		}
	}
	return NULL;
}
Operand* CQParser::OnAssigment(Operand* Kuda, Operand* Chto)
{
	Operand* rez = NULL;
	if(Kuda&&Chto)
	{
		Assignment* Ass = new Assignment();
		Ass->Op1.Set((Acceptor*)Kuda);
		Ass->Op2.Set((Returner*)Chto);
		rez = (Operand*) Ass;
	}
	return rez;
}
Operand* CQParser::OnIf(Operand* boolexp, Operand* functions)
{
	return OnIfElse(boolexp, functions, NULL);
}
Operand* CQParser::OnIfElse(Operand* boolexp, Operand* functions1, Operand* functions2)
{
	if(boolexp&&functions1)
	{
		If_Else* ff = new If_Else();
		ff->Op1.Set((BoolReturner*)boolexp);
		OperandBlock* OB = dynamic_cast<OperandBlock*>(functions1);
		if(OB)
		{
			int n = OB->Operands.GetAmount();
			for(int i=0;i<n;i++)
			{
				ff->OB1.Operands.Add(OB->Operands[i]);
			}
		}
		else
		{
			ff->OB1.Operands.Add((Procedure*)functions1);
		}
		if(functions2)
		{
			OperandBlock* OB2 = dynamic_cast<OperandBlock*>(functions2);
			if(OB2)
			{
				int n = OB2->Operands.GetAmount();
				for(int i=0;i<n;i++)
				{
					ff->OB2.Operands.Add(OB2->Operands[i]);
				}
			}
			else
			{
				ff->OB2.Operands.Add((Procedure*)functions2);
			}
		}
		return (Operand*)ff;
	}
	return NULL;
}
Operand* CQParser::OnOperandBlock(Operand* functionlist)
{
	if(functionlist)
	{
		OperandParamList* PL = (OperandParamList*)functionlist;
		int n = PL->Params.GetAmount();
		if(n)
		{
			OperandBlock* OB = new OperandBlock();
			for(int i=0;i<n;i++)
			{
				OB->Operands.Add((Procedure*)(PL->Params[i]));
			}
			return (Operand*)OB;
		}
	}
	return NULL;
}
Operand* CQParser::OnFunctionList(Operand* OP)
{
	if(OP)
		return new OperandParamList(OP);
	else
		return new OperandParamList();
}
Operand* CQParser::OnFunctionList(Operand* OP1, Operand* OP2)
{
	if(OP1&&OP2)
	{
		((OperandParamList*)OP1)->Add(OP2);
		return OP1;
	}
	return OP1;
}
Operand* CQParser::OnCompareFun(Operand* OP1,int action, Operand* OP2)
{
	if(OP1&&OP2)
	{
		CompareFun* CF = new CompareFun();
		CF->Op1.Set((NumericalReturner*)OP1);
		CF->Op=action;
		CF->Op2.Set((NumericalReturner*)OP2);
		return (Operand*) CF;
	}
	return NULL;
}
Operand* CQParser::OnLogical(Operand* OP1,int action, Operand* OP2)
{
	if(OP1&&OP2)
	{
		Logical* CF = new Logical();
		CF->Op1.Set((BoolReturner*)OP1);
		CF->Op=action;
		CF->Op2.Set((BoolReturner*)OP2);
		return (Operand*) CF;
	}
	return NULL;
}
Operand* CQParser::OnIntegerConst(const char* st)
{
	NumericalConst* NC = new NumericalConst();
	NC->Value.Value=atoi(st);
	return NC;
}
Operand* CQParser::OnStringConst(const char* st)
{
	StringConst* SC = new StringConst();
	SC->Value.String=st;
	return SC;
}
Returner* CQParser::FindFunction(const char* FName)
{
	Returner* rez=NULL;
	OneClassStorage* OCS = CGARB.GetClass("Returner");
	if(OCS&&OCS->OneMemb)
	{
		int n=OCS->Children.GetAmount();
		for(int i=0;i<n&&!rez;i++)
		{
			OneClassStorage* OCSC = CGARB.GetClass(OCS->Children[i]);
			if(OCSC&&OCSC->OneMemb)
			{
				Returner* On = dynamic_cast<Returner*> (OCSC->OneMemb);
				if(On)
				{
					if(!strcmp(FName,On->GetFunctionName()))
					{
						rez = (Returner*) OCSC->OneMemb->new_element();
					}
				}
			}
		}
	}
	return rez;
}
Procedure* CQParser::FindProcedure(const char* FName)
{
	Procedure* rez=NULL;
	OneClassStorage* OCS = CGARB.GetClass("Procedure");
	if(OCS&&OCS->OneMemb)
	{
		int n=OCS->Children.GetAmount();
		for(int i=0;i<n&&!rez;i++)
		{
			OneClassStorage* OCSC = CGARB.GetClass(OCS->Children[i]);
			if(OCSC&&OCSC->OneMemb)
			{
				Procedure* On = dynamic_cast<Procedure*> (OCSC->OneMemb);
				if(On)
				{
					if(!strcmp(FName,On->GetProcedureName()))
					{
						rez = (Procedure*) OCSC->OneMemb->new_element();
					}
				}
			}
		}
	}
	return rez;
}
BaseType* CQParser::FindType(const char* TName)
{
	BaseType* rez=NULL;
	OneClassStorage* OCS = CGARB.GetClass("BaseType");
	if(OCS&&OCS->OneMemb)
	{
		int n=OCS->Children.GetAmount();
		for(int i=0;i<n&&!rez;i++)
		{
			OneClassStorage* OCSC = CGARB.GetClass(OCS->Children[i]);
			if(OCSC&&OCSC->OneMemb)
			{
				BaseType* On = dynamic_cast<BaseType*> (OCSC->OneMemb);
				if(On)
				{
					if(!strcmp(TName,On->GetTypeName()))
					{
						rez = (BaseType*)OCSC->OneMemb->new_element();
					}
				}
			}
		}
	}
	return rez;
}
Variable*  CQParser::FindVariableByName(const char* Name)
{
	Variable* rez=NULL;
	if(Name&&OneScriptRef)
	{
		int n = OneScriptRef->VAR.Variables.GetAmount();
		for(int i=0;i<n;i++)
		{
			if(!strcmp(Name,OneScriptRef->VAR.Variables[i]->Name.pchar()))
			{
				return (OneScriptRef->VAR.Variables[i]);
			}
		}
	}
	return rez;
}
Operand* CQParser::OnFunction(const char* FName, Operand* PL)
{	
	Operand* rez=NULL;
		
	Returner* NR =FindFunction(FName);
	if(NR)
	{
		if(NR->GetNArguments())
		{
			OperandParamList* OPL = (OperandParamList*)PL; 
			if(PL&&OPL->Params.GetAmount()==NR->GetNArguments())
			{
				for(int j=0;j<NR->GetNArguments();j++)
				{
					NR->SetArgument(j,OPL->Params[j]);
				}
				rez=NR;
			}
		}
		else
		{
			rez=NR;
		}
	}
	if(PL)delete PL;
	if(NR&&!rez)delete NR;
						
	return rez;
}
Operand* CQParser::OnFunction2(const char* FName, const char* StringParam)
{
	Returner* NR =FindFunction(FName);
	if(NR)
	{
		if(NR->GetNArguments()==1)
		{
			StringConst* ST= new StringConst();
			ST->Value.String=StringParam;
			NR->SetArgument(0,ST);
			return NR;
		}
	}
	return NULL;
}
Operand* CQParser::OnAddFunctionArgument(Operand* OP)
{
	return new OperandParamList(OP);
}
Operand* CQParser::OnAddFunctionArgument(Operand* OP, Operand* PL)
{
	if(OP&&PL)
	{
		((OperandParamList*)PL)->Add(OP);
		return PL;
	}
	return NULL;
}
Operand* CQParser::OnArifmetical(Operand* OP1,int action, Operand* OP2)
{
	Op_Add* Ar = new Op_Add();
	Ar->SetArgument(0,OP1);
	Ar->Op=action;
	Ar->SetArgument(1,OP2);
	return Ar;
}
Operand* CQParser::OnUnarMinus(Operand* OP1)
{
	UnarMinus* UM = new UnarMinus();
	UM->SetArgument(0,OP1);
	return UM;
}
Operand* CQParser::OnBracket(Operand* OP1)
{
	Brackets* B = new Brackets();
	B->SetArgument(0,OP1);
	return B;
}
int	CQParser::yyInput( char* buf, int max_size )
{
	int nSym = 0;
	while ( *m_BufPtr && nSym < max_size )
	{
		buf[nSym++] = *(m_BufPtr++);
	}
	return nSym;
} 
int yyparse();
bool CQParser::ParseAndFillExpression(Operand* OP,DString* DS)
{
	Source.Clear();
	DS->Add(";");
	DS->Add(";");
	DS->Add(";");
	DS->Add(";");
	Source.Add(DS->pchar());
	QParser=this;
	m_BufPtr=Source.pchar();
	ResultExpression=OP;
	void yy_reset();	
	int n = yyparse();	
	yy_reset();
	return n==0;
}
bool CQParser::ParseAndFillOneScript(OneScript* OS,DString* DS)
{
	if(OS&&DS)
	{
		OS->BrigadesVariables.RefreshEnumerator();
		OneScriptRef=OS;
		QParser=this;
		m_BufPtr=DS->pchar();
		return yyparse()==0;
	}
	return false;
}
