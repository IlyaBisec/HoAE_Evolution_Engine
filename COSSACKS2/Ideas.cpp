class Operator:public BaseClass{
    //accepting interface
	virtual int  GetAcceptFocus(Operator* op);//returns index of native accepting
	virtual bool AcceptInt  (Operator* op);
	virtual bool AcceptFloat(Operator* op);
	virtual bool AcceptBool (Operator* op);
	virtual bool AcceptCoor (Operator* op);	
	//.....
	//returning interface
	virtual bool ReturnInt  (Operator* op);
	virtual bool ReturnFloat(Operator* op);
	virtual bool ReturnBool (Operator* op);
	virtual bool ReturnCoor (Operator* op);
	//array interface
	virtual bool GetElement(int Index,Operator* Op)

	//.....
    //sorting interface...
	//sending interface...
	//scaling interface...
	//....
}
class Op_int_variable:public Operator{
	_str Name;
	int Value;
};
class Op_float_variable:public Operator{
	_str Name;
	float Value;
};
class Op_accept_lvalue:public Operator{
	Operator* VariableRef;
	SAVE(Op_accept_lvalue);
	REG_INDEX(_index,VariableRef,ENUMERATOR_FOR_ALL_VARIABLES_FOR_ALL_TYPES)
	ENDSAVE;
};
class Op_accept_rvalue:public Operator{

};
class Op_accept:public Operator{
    ClassPtr<Op_accept_lvalue> LValue;
	ClassPtr<Op_accept_rvalue> r;
};