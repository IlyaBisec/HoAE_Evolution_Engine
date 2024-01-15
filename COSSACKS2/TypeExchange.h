#pragma once
//struct BaseTriplet{
//	BaseClass*				BC;
//	void*					Data;
//	void*					Extra;
//	OneClassMemberStorage*  OCMS;
//	void					PostWrite();
//};
typedef OneClassMemberInfo BaseTriplet;
class VariableExchangeReference:public BaseClass{
	_str		prev_str;
	union{
		DWORD	prev_DWORD;
	};
	union{
		int		prev_int;
	};
	union{
		short	prev_short;
	};
	union{
		word	prev_word;
	};
	union{
		byte	prev_byte;
	};
	union{
		char	prev_char;
	};
	BaseTriplet ParentMember;
	BaseTriplet ClassMember;	
public:
	ClassRef<ReferableBaseClass> RootClass;
	BaseTriplet FindClass();
	_str		FieldName;
	_str		VariableReferenceName;	
	virtual void _exchange();	
	virtual void _exchange(BaseTriplet& T1,BaseTriplet& T2);
	SAVE(VariableExchangeReference);
		REG_ENUM(_strindex,FieldName,ClassFields_L2);
		REG_AUTO(VariableReferenceName);
		REG_AUTO(RootClass);
	ENDSAVE;
	bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags);
};
class BooleanExchange:public VariableExchangeReference{
public:
    _str VariableValueFor_true;
    _str VariableValueFor_false;
    virtual void _exchange(BaseTriplet& T1,BaseTriplet& T2);
    SAVE(BooleanExchange){
        REG_BASE(VariableExchangeReference);
        REG_ENUM_EX(_strindex,FieldName,ClassFields_L2_bool,_boolFieldName);
        REG_AUTO(VariableReferenceName);
        REG_AUTO(VariableValueFor_true);
        REG_AUTO(VariableValueFor_false);
        REG_AUTO(RootClass);
    }ENDSAVE;
};

class ClassSyncroniser:public ClonesArray<VariableExchangeReference>{
public:
    virtual int GetExpansionRules(){
        return 3;
    }
    void _excahnge();	
	virtual bool ForceProperty(){return true;}
	bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags);
};