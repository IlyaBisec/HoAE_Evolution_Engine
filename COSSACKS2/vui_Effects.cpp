#include "stdheader.h"
#include "vui_Action.h"
#include "vui_Effects.h"
//
class vui_ef_ChangePos:public vui_BasicEffect{
public:
	bool  ChangePosX;
	bool  ChangePosY;
	float Xvalue;
	float Yvalue;
	SAVE(vui_ef_ChangePos);
		REG_PARENT(vui_BasicEffect);
		REG_PARENT(vui_Action);
		REG_MEMBER(_bool,ChangePosX);
		REG_MEMBER(_bool,ChangePosY);
		REG_MEMBER(_float,Xvalue);
		REG_MEMBER(_float,Yvalue);
	ENDSAVE;
	virtual void ApplySet(SimpleDialog* SD,float Value){
		if(ChangePosX)SD->ShiftX=Value*Xvalue;
		if(ChangePosY)SD->ShiftY=Value*Yvalue;
	}
	virtual void ApplyAdd(SimpleDialog* SD,float Value){
		if(ChangePosX)SD->ShiftX+=Value*Xvalue;
		if(ChangePosY)SD->ShiftY+=Value*Yvalue;
	}
	virtual void ApplyMod(SimpleDialog* SD,float Value){
		if(ChangePosX)SD->ShiftX*=Value*Xvalue;
		if(ChangePosY)SD->ShiftY*=Value*Yvalue;
	}
};
class vui_ef_ChangePivot:public vui_BasicEffect{
public:
	bool  ChangePosX;
	bool  ChangePosY;
	float Xvalue;
	float Yvalue;
	SAVE(vui_ef_ChangePivot);
		REG_PARENT(vui_BasicEffect);
		REG_PARENT(vui_Action);
		REG_MEMBER(_bool,ChangePosX);
		REG_MEMBER(_bool,ChangePosY);
		REG_MEMBER(_float,Xvalue);
		REG_MEMBER(_float,Yvalue);
	ENDSAVE;
	virtual void ApplySet(SimpleDialog* SD,float Value){
		if(ChangePosX)SD->PivotDx=Value*Xvalue;
		if(ChangePosY)SD->PivotDy=Value*Yvalue;
	}
	virtual void ApplyAdd(SimpleDialog* SD,float Value){
		if(ChangePosX)SD->PivotDx+=Value*Xvalue;
		if(ChangePosY)SD->PivotDy+=Value*Yvalue;
	}
	virtual void ApplyMod(SimpleDialog* SD,float Value){
		if(ChangePosX)SD->PivotDx*=Value*Xvalue;
		if(ChangePosY)SD->PivotDy*=Value*Yvalue;
	}
};
class vui_ef_ChangeAngle:public vui_BasicEffect{
public:
	float Angle1;
	float Angle0;
	SAVE(vui_ef_ChangeAngle);
		REG_PARENT(vui_BasicEffect);
		REG_PARENT(vui_Action);
		REG_MEMBER(_float,Angle1);
		REG_MEMBER(_float,Angle0);
	ENDSAVE;
	virtual void ApplySet(SimpleDialog* SD,float Value){
        float a=Angle0+Value*(Angle1-Angle0);
		SD->Angle=a;
	}
	virtual void ApplyAdd(SimpleDialog* SD,float Value){
		float a=Angle0+Value*(Angle1-Angle0);
		SD->Angle+=a;
	}
	virtual void ApplyMod(SimpleDialog* SD,float Value){
		float a=Angle0+Value*(Angle1-Angle0);
		SD->Angle*=a;
	}
};
class vui_ef_ChangeScale:public vui_BasicEffect{
public:
	vui_ef_ChangeScale(){
		AmplitudeChangeLow=1;
		StartValue=2.0f;
		StartValue=1.0f;
	}
	bool ChangeScaleX;
	bool ChangeScaleY;
	float StartValue;
	float FinalValue;
	SAVE(vui_ef_ChangeScale);
	REG_PARENT(vui_BasicEffect);
	REG_PARENT(vui_Action);
	REG_MEMBER(_bool,ChangeScaleX);
	REG_MEMBER(_bool,ChangeScaleY);
	REG_MEMBER(_float,StartValue);
	REG_MEMBER(_float,FinalValue);
	ENDSAVE;
	virtual void ApplySet(SimpleDialog* SD,float Value){
		if(ChangeScaleX)SD->ScaleX=FinalValue+Value*(StartValue-FinalValue);
		if(ChangeScaleY)SD->ScaleY=FinalValue+Value*(StartValue-FinalValue);
	}
	virtual void ApplyAdd(SimpleDialog* SD,float Value){
		if(ChangeScaleX)SD->ScaleX+=FinalValue+Value*(StartValue-FinalValue);
		if(ChangeScaleY)SD->ScaleY+=FinalValue+Value*(StartValue-FinalValue);
	}
	virtual void ApplyMod(SimpleDialog* SD,float Value){
		if(ChangeScaleX)SD->ScaleX*=FinalValue+Value*(StartValue-FinalValue);
		if(ChangeScaleY)SD->ScaleY*=FinalValue+Value*(StartValue-FinalValue);
	}
};
class vui_ef_ChangeColor:public vui_BasicEffect{
public:
	vui_ef_ChangeColor(){
		StartColor=0x00FFFFFF;
		StartColor=0xFFFFFFFF;
		AmplitudeChangeLow=1;		
	}
	DWORD StartColor;
	DWORD FinalColor;
	SAVE(vui_ef_ChangeColor);
		REG_PARENT(vui_BasicEffect);
		REG_PARENT(vui_Action);
		REG_MEMBER(_color,StartColor);
		REG_MEMBER(_color,FinalColor);
	ENDSAVE;
	virtual void ApplySet(SimpleDialog* SD,float Value){
		DWORD C=MixDWORD(StartColor,FinalColor,Value*255,255*(1-Value));
		SD->Diffuse=C;		
	}
	virtual void ApplyAdd(SimpleDialog* SD,float Value){
		DWORD C=MixDWORD(StartColor,FinalColor,Value*255,255*(1-Value));
		SD->Diffuse=C;
	}
	virtual void ApplyMod(SimpleDialog* SD,float Value){
		DWORD C=MixDWORD(StartColor,FinalColor,Value*255,255*(1-Value));
		ModDWORD(SD->Diffuse,C);
	}
};

//implementation
vui_BasicEffect::vui_BasicEffect(){
	LastEnterTime=0;
	LastActionTime=0;
	WasMouseOver=0;
	WasVisible=0;	
	WasLpressed=0;
	OperationType=0;
	WhenStartEffect=0;
	AmplitudeChangeLow=0;
	CyclingLow=0;
	Period=500;
	Phase=0;
	ExistingTime=1000;
	for(int i=0;i<10;i++)Table[i]=1.0f-float(i)/9.0f;
}
bool vui_BasicEffect::GetValue(SimpleDialog* SD,float& v){
	v=0.0f;
	if(LastActionTime&&ExistingTime){
		int dt=GetTickCount()-LastActionTime;
		if(dt>ExistingTime){
			if(OperationType==0){
				return true;
			}else{
				return false;
			}			
		}
		float t=float(dt)/ExistingTime;
		if(t>1.0)t=1.0;
		switch(AmplitudeChangeLow){
			case 0://Constant
				break;
			case 1://Linear
				v=1.0f-t;
				if(v<0)v=0;
				break;
			case 2://Exp
				v=exp(-float(dt)/ExistingTime)-t*exp(-3.0f);
				break;
			case 3://OneWave
				if(dt<ExistingTime){
                    v=(cos((float(dt)*2.0f/ExistingTime-1)*3.1415926f)+1)*0.5f;
				}else v=0;
				break;
			case 4://Square
				v=1.0f-t*t;
				if(v<0)v=0;
				break;
			case 5://Qubic
				v=1.0f-t*t*t;
				if(v<0)v=0;
				break;
			case 6://table
				{
					int p=t*9;
					float fr=fmod(t*9.0f,1.0f);
					if(p>=9)v=Table[9];
					else{
						v=Table[p]+fr*(Table[p+1]-Table[p]);
					}
				}
				break;
		}
		if(Period){
			switch(CyclingLow){
				case 0://constant
					break;
				case 1://Sin
					v*=(sin(float(dt)*3.1415926f*2.0f/Period+Phase*3.1415926f/180)+1)*0.5f;
					break;
				case 2://Cos
					v*=(cos(float(dt)*3.1415926f*2.0f/Period+Phase*3.1415926f/180)+1)*0.5f;
					break;
				case 3://Random
					v*=float(rand())/32768;
					break;
				case 4://SignedSin
					v*=sin(float(dt)*3.1415926f*2.0f/Period+Phase*3.1415926f/180);
					break;
				case 5://SignedCos
					v*=cos(float(dt)*3.1415926f*2.0f/Period+Phase*3.1415926f/180);
					break;				
			}
		}
		return true;
	}
	return false;
}
extern bool realLpressed;
void vui_BasicEffect::SetFrameState(SimpleDialog* SD){
    int T=GetTickCount();
	switch(WhenStartEffect){
		case 0://OnDrawFirstTime
			if(SD->Visible&&(LastActionTime==0||T-LastEnterTime>1000))LastActionTime=T;
			break;
		case 1://OnMouseOver
			if(SD->MouseOver&&(LastActionTime==0||T-LastActionTime>ExistingTime)&&!WasMouseOver)LastActionTime=T;
			break;
		case 2://OnMouseNotOver
			if((!SD->MouseOver)&&(LastActionTime==0||T-LastActionTime>ExistingTime)&&WasMouseOver)LastActionTime=T;
			break;
		case 3://OnClick
			if(SD->MouseOver&&(LastActionTime==0||T-LastActionTime>200)&&(realLpressed&&!WasLpressed))LastActionTime=T;
			break;
		case 4://OnProgramer			
			break;
	}
	LastEnterTime=T;
	WasMouseOver=SD->MouseOver;
	WasLpressed=realLpressed;
	WasVisible=SD->Visible;
	float v;
	if(GetValue(SD,v)){
		switch(OperationType){
			case 0://set
				ApplySet(SD,v);
				break;
			case 1://add
				ApplyAdd(SD,v);
				break;
			case 2://mod
				ApplyMod(SD,v);
				break;
		}
	}
}
void RegEffectClass(){
	REG_CLASS(vui_BasicEffect);
	REG_CLASS_EX(vui_ef_ChangePos  ,"Effects");
	REG_CLASS_EX(vui_ef_ChangeScale,"Effects");
	REG_CLASS_EX(vui_ef_ChangeColor,"Effects");
	REG_CLASS_EX(vui_ef_ChangePivot,"Effects");
	REG_CLASS_EX(vui_ef_ChangeAngle,"Effects");
}