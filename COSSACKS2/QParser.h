#ifndef __QPARSER_H__
#define __QPARSER_H__

#include "SuperEditor.h"

class OperandParamList : public Operand
{
public:
	OperandParamList();
	OperandParamList(Operand* OP);
	DynArray<Operand*> Params;
	void Add(Operand* OP);
};

class CQParser
{
public:
	CQParser();
	Operand* OnScript(Operand* functionlist);
	Operand* OnDeclareVariable(const char* Type, const char* Name);
	Operand* OnVariable(const char* Name);
	Operand* OnBrigadeVariable(const char* Type, const char* Name);
	Operand* OnProcedure(const char* Name, Operand* argumentlist);
	Operand* OnAssigment(Operand* Kuda, Operand* Chto);
	Operand* OnIf(Operand* boolexp, Operand* functions);
	Operand* OnIfElse(Operand* boolexp, Operand* functions1, Operand* functions2);
	Operand* OnOperandBlock(Operand* functionlist);
	Operand* OnFunctionList(Operand* OP);
	Operand* OnFunctionList(Operand* OP1, Operand* OP2);
	Operand* OnCompareFun(Operand* OP1,int action, Operand* OP2);
	Operand* OnLogical(Operand* OP1,int action, Operand* OP2);
	
	Operand* OnExpression(Operand* OP);
	Operand* OnIntegerConst(const char* st);
	Operand* OnStringConst(const char* st);
	Operand* OnFunction(const char* FName, Operand* PL);
	Operand* OnFunction2(const char* FName, const char* StringParam);
	Operand* OnAddFunctionArgument(Operand* OP);
	Operand* OnAddFunctionArgument(Operand* OP, Operand* PL);
	Operand* OnArifmetical(Operand* OP1,int action, Operand* OP2);
	Operand* OnUnarMinus(Operand* OP1);
	Operand* OnBracket(Operand* OP1);
	int yyInput( char* buf, int max_size );
	bool ParseAndFillExpression(Operand* OP,DString* DS);
	bool ParseAndFillOneScript(OneScript* OS,DString* DS);
private:
	Operand* Return;
	DString Source;
	char* m_BufPtr;
	Operand* ResultExpression;
	OneScript* OneScriptRef;
	Returner* FindFunction(const char* FName);
	Procedure* FindProcedure(const char* FName);
	BaseType* FindType(const char* TName);
	Variable* FindVariableByName(const char* Name);
};
#endif
