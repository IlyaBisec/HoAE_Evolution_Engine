#pragma once

#ifndef __SUPEREDITOR_H__
#define __SUPEREDITOR_H__

//==================================================================================================================//
class BaseType;
//==================================================================================================================//

class Operand : public BaseClass
{
public:
	Operand();
	~Operand();
	SAVE(Operand)
	ENDSAVE;

	virtual bool Preview(DString& ST,word Shift);
	
	virtual void GetViewMask(DString& ST);
	virtual void GetShortViewMask(DString& ST);
	virtual int GetNArguments();
	virtual Operand* GetArgument(int NIndex); 
	virtual void SetArgument(int Index, Operand* Op);
	virtual void GetAssembledView(DString& ST,bool SetURL);
	virtual const char* GetThisElementView(const char* LocalName);

	virtual bool Get(BaseType* BT);
	virtual bool Set(BaseType* BT);
protected:
	void CreateURL(DString& ST,DString& Tx,Operand* URL);
	void CreateURL(DString& ST,char* Tx,Operand* URL);
};
//==================================================================================================================//
class BaseType : public Operand
{
public:
	SAVE(BaseType);
	ENDSAVE;
	virtual const char* GetID();

	virtual bool GetValue(BaseType* BT);
	virtual bool SetValue(BaseType* BT);

	virtual void GetViewMask(DString& ST);
	virtual const char* GetTypeName();
};
//==================================================================================================================//
class Variable : public Operand
{
public:
	Variable();
	~Variable();
	_str Name;
	ClassPtr<BaseType> VarType;
	SAVE(Variable);
		REG_AUTO(Name); 
		REG_AUTO(VarType);
	ENDSAVE;
	virtual void GetViewMask(DString& ST);
};
//==================================================================================================================//
class NumericalType : public BaseType 
{
public:
	NumericalType();
	int Value;
	SAVE(NumericalType);
		REG_PARENT(BaseType);
		REG_MEMBER(_int,Value);
	ENDSAVE; 

	virtual const char* GetID();

	virtual bool GetValue(BaseType* BT);
	virtual bool SetValue(BaseType* BT);
	virtual const char* GetTypeName();
	virtual bool ForceSimplification(){return true;}
};
//==================================================================================================================//
class NumericalTypePointer : public NumericalType 
{
public:
	NumericalTypePointer();
	NumericalTypePointer(int* p);
	int* pInt;
	SAVE(NumericalTypePointer);
		REG_PARENT(BaseType);
		REG_PARENT(NumericalType);
	ENDSAVE; 

	virtual bool GetValue(BaseType* BT);
	virtual bool SetValue(BaseType* BT);
};
//==================================================================================================================//
class BoolType : public BaseType
{
public:
	BoolType();
	bool Value;
	SAVE(BoolType)
		REG_PARENT(BaseType);
		REG_MEMBER(_bool,Value);
	ENDSAVE;

	virtual const char* GetID();

	virtual bool GetValue(BaseType* BT);
	virtual bool SetValue(BaseType* BT);
	virtual const char* GetTypeName();
};
//==================================================================================================================//
template<class T> class ArrayType : public BaseType
{
public:
	ArrayType();
	ClonesArray<T> Values;
    /*
	SAVE(ArrayType);
		//REG_PARENT(BaseType);
		REG_AUTO(Values);
	ENDSAVE;
	*/
	virtual bool GetElement(int Index, BaseType* BT);
	virtual bool SetElement(int Index, BaseType* BT);
	virtual bool AddElement(BaseType* BT);
	virtual void Clear();
	virtual int GetNElements();
	virtual bool GetValue(BaseType* BT);
	virtual bool SetValue(BaseType* BT);
	//
	virtual BaseType* GetNewElement();
	virtual const char* GetID();
	//
};
//==================================================================================================================//
class NumericalArray : public ArrayType<NumericalType>
{
public:
	NumericalArray() {;}

	SAVE(NumericalArray);
	 REG_PARENT(BaseType);
	 REG_AUTO(Values);
	ENDSAVE;

	virtual const char* GetID() { return "NumericalArrayEnum"; }
	virtual const char* GetTypeName() { return "IntArray"; }

};
//==================================================================================================================//
/*
class BoolArray : public ArrayType<BoolType>
{
public:
	BoolArray() {;}

	SAVE(BoolArray);
		REG_PARENT(BaseType);
		REG_AUTO(Values);
	ENDSAVE;

	virtual const char* GetID() { return "BoolTypeEnum"; }
};
*/
//==================================================================================================================//
class VarList:public ClassArray<Variable>
{
public:
	int GetExpansionRules(){
		return 1;
	}
};

class VariablesList : public Operand
{
public:
	VariablesList();
	~VariablesList(); 
	VarList Variables;
	SAVE(VariablesList);
	REG_AUTO(Variables);
	ENDSAVE;
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra=NULL);
	virtual void GetViewMask(DString& ST);
	virtual void GetShortViewMask(DString& ST);
	virtual int GetNArguments();
	virtual Operand* GetArgument(int NIndex);

	void RefreshEnumerator();
};

//==================================================================================================================//
class BrigadeVariablesList : public VariablesList
{
public: 
	BrigadeVariablesList();
    bool IsInit;
	void Init();
	SAVE(BrigadeVariablesList);
		REG_AUTO(Variables);
	ENDSAVE;
	virtual void GetViewMask(DString& ST);
	virtual void GetShortViewMask(DString& ST);
	
	Variable FormID;
	Variable UnitID;
	Variable FinalStateOrder;
	Variable NShooters;
	Variable NMelee;
	Variable NDying;
	Variable NMoving;
	Variable NStanding;
	Variable NAttMoving;
	Variable NAttStanding;
	Variable Panic;
	Variable Tired;
	Variable NRecharging;
	Variable NPAttack;
	Variable NUAttack;
	Variable NFireReady;
	Variable DistToMainTarget;
	Variable IsStandGround;
	Variable Arsenal;
	Variable Morale;//GEC
	Variable Food;//GEC
	
	void RefreshEnumerator();
	virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra);
};
//==================================================================================================================//
class Procedure : public Operand
{
public:
	SAVE(Procedure);
		REG_PARENT(Operand);
	ENDSAVE;
	virtual bool Execute();
	bool SetArguments(Operand* ArgumentList);
	virtual const char* GetProcedureName();
	virtual void GetViewMask(DString& ST);
};
//==================================================================================================================//
class Returner : public Operand
{
public:
	SAVE(Returner);
		REG_PARENT(Operand);
	ENDSAVE;

	virtual const char* GetFunctionName();
};
//==================================================================================================================//
class Acceptor : public Operand
{
public:
	SAVE(Acceptor);
		REG_PARENT(Operand);
	ENDSAVE;

	virtual BaseType* GetArgumentType();
};
//==================================================================================================================//
class NumericalReturner : public Returner
{
public:
	SAVE(NumericalReturner);
		REG_PARENT(Operand);
		//REG_PARENT(Returner);
	ENDSAVE;
};
//==================================================================================================================//
class NumericalAcceptor : public Acceptor
{
public:
	SAVE(NumericalAcceptor);
		REG_PARENT(Operand);
		//REG_PARENT(Acceptor);
	ENDSAVE;

	static NumericalType TypeExample;
	virtual BaseType* GetArgumentType();
};
//==================================================================================================================//
class BoolReturner : public Returner
{
public:
	SAVE(BoolReturner);
		REG_PARENT(Operand);
		//REG_PARENT(Returner);
	ENDSAVE;
};
//==================================================================================================================//
class BoolAcceptor : public Acceptor
{
public:
	SAVE(BoolAcceptor);
		REG_PARENT(Operand);
		//REG_PARENT(Acceptor);
	ENDSAVE;

	static NumericalType TypeExample;
	virtual BaseType* GetArgumentType();
};
//==================================================================================================================//
class NumericalArrayReturner : public Returner
{
public:
	SAVE(NumericalArrayReturner);
		REG_PARENT(Operand);
		//REG_PARENT(Returner);
	ENDSAVE;
};
//==================================================================================================================//
class NumericalArrayAcceptor : public Acceptor
{
public:
	SAVE(NumericalArrayAcceptor);
		REG_PARENT(Operand);
		//REG_PARENT(Acceptor);
	ENDSAVE;

	static NumericalArray TypeExample;
	virtual BaseType* GetArgumentType();
};
//==================================================================================================================//
class OperandBlock : public Procedure
{
public:
	ClassArray<Procedure> Operands;
	OperandBlock();
	~OperandBlock();
	SAVE(OperandBlock);
		REG_PARENT(Operand);
		REG_PARENT(Procedure);
		REG_AUTO(Operands);
	ENDSAVE;	
	
	virtual bool Execute();
	virtual bool Preview(DString& ST,word Shift);

	virtual void GetViewMask(DString& ST);
	virtual void GetShortViewMask(DString& ST);
	virtual int GetNArguments();
	virtual Operand* GetArgument(int NIndex);
	virtual void SetArgument(int NIndex, Operand* Op);
};
//==================================================================================================================//
class OperandBlock2 : public OperandBlock
{
public:
	OperandBlock2();
	~OperandBlock2();
	SAVE(OperandBlock2);
		REG_PARENT(Operand);
		REG_PARENT(OperandBlock);
	ENDSAVE;	
	virtual void GetViewMask(DString& ST);
};
//==================================================================================================================//
class Const : public Returner
{
public:
	ClassPtr<BaseType> Value;
	SAVE(Const)
		REG_PARENT(Returner);
	ENDSAVE;

	virtual bool Get(BaseType* BT);
	virtual bool ForceSimplification(){return true;}
};
//==================================================================================================================//
class NumericalConst : public NumericalReturner
{
public:
	NumericalType Value;

	SAVE(NumericalConst)
		REG_PARENT(NumericalReturner);
		REG_PARENT(Returner);
		REG_AUTO(Value);
	ENDSAVE

	virtual void GetViewMask(DString& ST);
	virtual bool Get(BaseType* BT);
	virtual bool ForceSimplification(){return true;}
};
//==================================================================================================================//
class VariableReference : public Acceptor, public Returner
{
public:
	int VarPoint;

	SAVE_EX(VariableReference,Acceptor);
		//REG_PARENT(Acceptor);
		//REG_PARENT(Returner);
	ENDSAVE;

	virtual bool Get(BaseType* BT);
	virtual bool Set(BaseType* BT);

	virtual void GetViewMask(DString& ST);
};
//==================================================================================================================//
class NumericalVariableReference : public NumericalAcceptor,  public NumericalReturner, public VariableReference
{
public:
	
	SAVE_EX(NumericalVariableReference,NumericalAcceptor);
		REG_PARENT(Operand);
		REG_PARENT(NumericalAcceptor);
		REG_PARENT(NumericalReturner);
		REG_PARENT(Acceptor);
		REG_PARENT(Returner);
		REG_ENUM(_index,VarPoint,NumericalTypeEnum);
	ENDSAVE;

	virtual bool Get(BaseType* BT);
	virtual bool Set(BaseType* BT);

	virtual void GetViewMask(DString& ST);
};
//==================================================================================================================//
class NumericalArrayVariableReference : public NumericalArrayAcceptor,  public NumericalArrayReturner, public VariableReference
{
public:

	SAVE_EX(NumericalArrayVariableReference,NumericalArrayAcceptor);
	REG_PARENT(Operand);
		REG_PARENT(NumericalArrayAcceptor);
		REG_PARENT(NumericalArrayReturner);
		REG_PARENT(Acceptor);
		REG_PARENT(Returner);
		REG_ENUM(_index,VarPoint,NumericalArrayEnum);
	ENDSAVE;
	
	virtual void GetViewMask(DString& ST);
};
//==================================================================================================================//
class Op_Add : public NumericalReturner
{
public:
	Op_Add();
	~Op_Add();
	ClassPtr<NumericalReturner> Op1;
	int Op;
	ClassPtr<NumericalReturner> Op2;
	SAVE(Op_Add)
		REG_PARENT(NumericalReturner);
		REG_PARENT(Returner);
		REG_AUTO(Op1);
		REG_ENUM(_index,Op,ARIPHMETICALOPS)
		REG_AUTO(Op2);
	ENDSAVE
	
	virtual bool Preview(DString& ST,word Shift);

	virtual void GetViewMask(DString& ST);
	virtual void GetShortViewMask(DString& ST);
	virtual int GetNArguments();
	virtual Operand* GetArgument(int NIndex);
	virtual void SetArgument(int NIndex, Operand* Op);

	virtual bool Get(BaseType* BT);
};
//==================================================================================================================//
class UnarMinus : public NumericalReturner
{
public:
	ClassPtr<NumericalReturner> Op1;
	SAVE(UnarMinus)
		REG_PARENT(NumericalReturner);
		REG_PARENT(Returner);
		REG_AUTO(Op1);
	ENDSAVE

	virtual void GetViewMask(DString& ST);
	virtual int GetNArguments();
	virtual Operand* GetArgument(int NIndex);
	virtual void SetArgument(int NIndex, Operand* Op);
	virtual bool Get(BaseType* BT);
};
//==================================================================================================================//
class Brackets : public NumericalReturner
{
public:
	ClassPtr<NumericalReturner> Op1;
	SAVE(Brackets)
		REG_PARENT(NumericalReturner);
		REG_PARENT(Returner);
		REG_AUTO(Op1);
	ENDSAVE

	virtual void GetViewMask(DString& ST);
	virtual int GetNArguments();
	virtual Operand* GetArgument(int NIndex);
	virtual void SetArgument(int NIndex, Operand* Op);
	virtual bool Get(BaseType* BT);
};
//==================================================================================================================//
class NumericalArrayElement : public NumericalAcceptor, public NumericalReturner 
{
public:
	NumericalArrayElement();

	//ClassPtr<NumericalArray> Arr;
	int Arr;
	ClassPtr<NumericalReturner> ArrIndex;

	SAVE_EX(NumericalArrayElement,NumericalAcceptor);
		REG_PARENT(NumericalAcceptor);
		REG_PARENT(NumericalReturner);
		REG_PARENT(Returner);
		REG_PARENT(Acceptor);
		//REG_AUTO_A(Arr,NumericalReturner);
		REG_ENUM(_index,Arr,NumericalArrayEnum);
		REG_AUTO_A(ArrIndex,NumericalAcceptor);
		//REG_AUTO(ArrIndex);
	ENDSAVE;

	virtual void GetViewMask(DString& ST);
	virtual int GetNArguments();
	virtual Operand* GetArgument(int NIndex);
	virtual void SetArgument(int index, Operand* Op);
	virtual bool Get(BaseType* BT);
	virtual bool Set(BaseType* BT);
};
//==================================================================================================================//
class Assignment : public Procedure
{
public:
	ClassPtr<Acceptor> Op1;
	ClassPtr<Returner> Op2;
	SAVE(Assignment)
		REG_PARENT(Procedure);
		REG_AUTO(Op1);
		REG_AUTO(Op2);
	ENDSAVE

	virtual void GetViewMask(DString& ST);
	virtual int GetNArguments();
	virtual Operand* GetArgument(int NIndex);
	virtual void SetArgument(int index, Operand* Op);
	virtual bool Execute();
};
//==================================================================================================================//
class If_Else : public Procedure
{
public:
	virtual const char* GetElementView(int Index,const char* LocalName);
	ClassPtr<BoolReturner> Op1;
	OperandBlock2 OB1;
	OperandBlock2 OB2;
	SAVE(If_Else)
		REG_PARENT(Procedure);
		REG_AUTO(Op1);
		REG_AUTO(OB1);
		REG_AUTO(OB2);
	ENDSAVE
	
	virtual bool Execute();
	virtual void GetViewMask(DString& ST);
	virtual int GetNArguments();
	virtual Operand* GetArgument(int NIndex);
	virtual void SetArgument(int index, Operand* Op);

};
//==================================================================================================================//
class ForEachNumerical : public Procedure
{
public:
	ForEachNumerical();

	NumericalVariableReference NumericalVar;
	NumericalArrayVariableReference NumericalArr;
	OperandBlock2 OB;
	
	SAVE(ForEachNumerical)
		REG_PARENT(Procedure);
		REG_AUTO_A(NumericalVar,NumericalAcceptor);
		REG_AUTO_A(NumericalArr,NumericalArrayAcceptor);
		REG_AUTO(OB);
	ENDSAVE;

	virtual bool Execute();
	virtual void GetViewMask(DString& ST);
	virtual int GetNArguments();
	virtual Operand* GetArgument(int NIndex);
};
//==================================================================================================================//
class ForFromTo : public Procedure
{
public:
	ForFromTo();
	NumericalVariableReference NumericalVar;
	ClassPtr<NumericalReturner> From;
	ClassPtr<NumericalReturner> To;
	OperandBlock2 OB;

	SAVE(ForFromTo)
		REG_PARENT(Procedure);
		REG_AUTO_A(NumericalVar,NumericalAcceptor);
		REG_AUTO(From);
		REG_AUTO(To);
		REG_AUTO(OB);
	ENDSAVE;

	virtual bool Execute();
	virtual void GetViewMask(DString& ST);
	virtual int GetNArguments();
	virtual Operand* GetArgument(int NIndex);
};
//==================================================================================================================//
class Logical : public BoolReturner
{
public:
	ClassPtr<BoolReturner> Op1;
	int Op;
	ClassPtr<BoolReturner> Op2;
	SAVE(Logical)
		REG_PARENT(BoolReturner);
		REG_PARENT(Returner);
		REG_AUTO(Op1);
		REG_ENUM(_index,Op,LOGICALOPS)
		REG_AUTO(Op2);
	ENDSAVE

	virtual void GetViewMask(DString& ST);
	virtual int GetNArguments();
	virtual Operand* GetArgument(int NIndex);
	virtual bool Get(BaseType* BT);
	virtual void SetArgument(int index, Operand* Op);
};
//==================================================================================================================//
class CompareFun : public BoolReturner
{
public:
	ClassPtr<NumericalReturner> Op1;
	int Op; 
	ClassPtr<NumericalReturner> Op2;
	SAVE(CompareFun)
		REG_PARENT(BoolReturner);
		REG_PARENT(Returner);
		REG_AUTO(Op1);
		REG_ENUM(_index,Op,COMPAREOPS)
		REG_AUTO(Op2);
	ENDSAVE

	virtual void GetViewMask(DString& ST);
	virtual int GetNArguments();
	virtual Operand* GetArgument(int NIndex);
	virtual bool Get(BaseType* BT);
	virtual void SetArgument(int index, Operand* Op);
};
//==================================================================================================================//
class OneStringExpressions : public NumericalReturner
{
public:
	ClassPtr<NumericalReturner> Op;
	SAVE(OneStringExpressions)
		//REG_PARENT(NumericalReturner);
		//REG_PARENT(Returner);
		REG_AUTO(Op);
	ENDSAVE;

	virtual void GetViewMask(DString& ST);
	virtual int GetNArguments();
	virtual Operand* GetArgument(int NIndex);
	virtual bool Get(BaseType* BT);
	virtual void SetArgument(int index, Operand* Op);
	virtual bool ForceSimplification(){return true;}
};
//==================================================================================================================//
class UserFriendlyNumericalReturner: public BaseClass {
public:
	OneStringExpressions Value;
	SAVE(UserFriendlyNumericalReturner){
		REG_AUTO(Value);
	}ENDSAVE;

	operator int() { return Get(); }
	int operator = (UserFriendlyNumericalReturner& v){
        return v.Get();
	}	
	UserFriendlyNumericalReturner& operator = (int v){
		SetConst(v);
		return *this;
	}	

	int Get(){
		NumericalType NT;
		if(Value.Get(&NT))
			return NT.Value;
		return 0;
	}
	void SetConst(int Val){
		reset_class(this);
		NumericalConst* NC=new NumericalConst;
		NC->Value.Value=Val;
		Value.Op.Set(NC);
	}
	bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if((!options) || strcmp(options,"root")){
			NumericalReturner* NR=Value.Op.Get();
			if(NR)NR->GetAssembledView(dest,false);
			else dest="0";
			dest.Replace("/"," div ");
			return true;
		}else return false;
	}
};
//==================================================================================================================//
class SuperEditor 
{
public:
	SuperEditor(void);
	~SuperEditor(void);
};
//==================================================================================================================//
class licsema : public _str
{
public:
	licsema();
	DString SecondName;
	int Type;//0-\n, 1-digit, 2-str, 3-Operator, 4- StringConst
};
//==================================================================================================================//
class OneScript:public BaseClass
{
public:
	OneScript();
	void Init();
    VariablesList VAR;
	BrigadeVariablesList BrigadesVariables;
	OperandBlock Script; 
	SAVE(OneScript);
		REG_LOADSAVE("Ai\\","*.qpp.xml");
		REG_AUTO(VAR);
		REG_AUTO(BrigadesVariables);
		REG_AUTO(Script);
	ENDSAVE;

	DString ErrMsg;
	bool ReadQppFile(char* file);
	//virtual bool Load(xmlQuote& xml,void* ClassPtr,ErrorPager* Error,void* Extra);

private:
	int Point;
	ClassArray<licsema> Words;
	bool FillWordList(DString* DS);
	bool ReadVariablesList(VariablesList* VL);
	void MarkVariables();
	BaseType* GetTypeByName(char* T);
	bool ParseOperand(Operand* Op,int& point);
	bool ParseViewMask(Operand* Op,ClassArray<licsema> *W);
	bool ParseDString(DString& DS,ClassArray<licsema> *W);
	bool FindAndSetEditableElement(Operand* Op, int ElementIndex, int& Np, int ArgumentIndex);
	static Enumerator* BrigadeFormationType;
};
//==================================================================================================================//

//==================================================================================================================//
#include "qTypes.h"
#include "qFunctions.h"
#include "qProcedures.h"
#include "VideoScriptCommands.h"
#include "BrigadeVariables.h"

//==================================================================================================================//
#endif	
