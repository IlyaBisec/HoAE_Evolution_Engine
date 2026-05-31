#pragma once
#include "vui_Action.h"
//
class vui_BasicEffect:public vui_Action{
public:
	vui_BasicEffect();
	//temp:
	int   LastEnterTime;
	int   LastActionTime;
	bool  WasMouseOver;
	bool  WasVisible;
	bool  WasLpressed;
	//registered
	byte  OperationType;
	byte  WhenStartEffect;
	byte  AmplitudeChangeLow;
	byte  CyclingLow;
	int   Period;
	int   Phase;
	int   ExistingTime;
	float Table[10];

	void StartEffect(){
		LastActionTime=GetTickCount();
	}
	bool isFinished(){
		return GetTickCount()>LastActionTime+ExistingTime;
	}

	virtual void ApplySet(SimpleDialog* SD,float Value){};
	virtual void ApplyAdd(SimpleDialog* SD,float Value){};
	virtual void ApplyMod(SimpleDialog* SD,float Value){};

	bool GetValue(SimpleDialog* SD,float& v);
	virtual void SetFrameState(SimpleDialog* SD);
	virtual void Init(SimpleDialog* SD){};
	virtual void Accept(SimpleDialog* SD){};
	SAVE(vui_BasicEffect){		
		REG_ENUM(_byte_index,OperationType      ,vbeOpType );
		REG_ENUM(_byte_index,WhenStartEffect    ,vbeEfStart);
		REG_ENUM(_byte_index,AmplitudeChangeLow ,vbeAmpLow );
		SAVE_SECTION(2);
		REG_MEMBER_EX(_float01,Table[0],V0);
		REG_MEMBER_EX(_float01,Table[1],V1);
		REG_MEMBER_EX(_float01,Table[2],V2);
		REG_MEMBER_EX(_float01,Table[3],V3);
		REG_MEMBER_EX(_float01,Table[4],V4);
		REG_MEMBER_EX(_float01,Table[5],V5);
		REG_MEMBER_EX(_float01,Table[6],V6);
		REG_MEMBER_EX(_float01,Table[7],V7);
		REG_MEMBER_EX(_float01,Table[8],V8);
		REG_MEMBER_EX(_float01,Table[9],V9);
		SAVE_SECTION(0xFFFFFFFF);
		REG_ENUM(_byte_index,CyclingLow         ,vbeCycling);
		SAVE_SECTION(1);
		REG_MEMBER(_int,Period);
		REG_MEMBER(_int,Phase);		
		SAVE_SECTION(0xFFFFFFFF);
		REG_MEMBER(_int,ExistingTime);
	}ENDSAVE;
	virtual DWORD GetClassMask(){
		DWORD M=0xFFFFFFFF-3;
		if(CyclingLow)M|=1;
		if(AmplitudeChangeLow==6)M|=2;
		return M;
	}
};
