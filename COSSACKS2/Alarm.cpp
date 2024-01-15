#include "stdheader.h"
#include "ClassEditorsRegistration.h"

class OneAlarmOptions:public BaseClass{
public:
	int Texture;
	int Sound;
	int Interval_ms;	
	int Duration_ms;
	int PulsingCount;
	float Rotation;
	int MinPulsingRadius;
	int MaxPulsingRadius;
	DWORD StartPulsingColor;
	DWORD FinalPulsingColor;

	SAVE(OneAlarmOptions);
		REG_MEMBER(_TextureID,Texture);
		REG_ENUM(_index,Sound,ALL_SOUNDS);
		REG_MEMBER(_int,Interval_ms);
		REG_MEMBER(_int,Duration_ms);
		REG_MEMBER(_int,PulsingCount);
		REG_MEMBER(_float0_10,Rotation);
		REG_MEMBER(_int,MinPulsingRadius);
		REG_MEMBER(_int,MaxPulsingRadius);
		REG_MEMBER(_color,StartPulsingColor);
		REG_MEMBER(_color,FinalPulsingColor);
	ENDSAVE;

	__constructor(OneAlarmOptions){
		if(IRS)
			Texture=IRS->GetTextureID("alarm.dds");
		Sound=-1;
		Interval_ms=30000;
		Duration_ms=2000;
		PulsingCount=2;
		MinPulsingRadius=10;
		MaxPulsingRadius=60;
		StartPulsingColor=0xFFFFFFFF;
		FinalPulsingColor=0x80FFFFFF;
		Rotation=0;
		reset();
	}

	//////////////////////////////////////////////////////////////////////////	
	//dynamical using
	int LastAlarmTime;
	int AlarmX;
	int AlarmY;
	//////////////////////////////////////////////////////////////////////////	
	void Draw();
	void SetAlarm(int x,int y);
	void TryToAddAlarm(int x,int y);
	void reset();
};
class AlarmOptions:public BaseClass{
public:
    OneAlarmOptions AttackAlarm;
	OneAlarmOptions CaptureAlarm;
	SAVE(AlarmOptions);
		REG_AUTO(AttackAlarm);
		REG_AUTO(CaptureAlarm);
	ENDSAVE;
};

AlarmOptions ALARMS;

void OneAlarmOptions::reset(){
	AlarmX=-1;
	AlarmY=-1;
	LastAlarmTime=-1000000;
}
extern int TrueTime;
extern int MiniLx;
extern int MiniLy;
extern int MiniX,MiniY;
void DrawTexturedBar(float xL,float yL,float W,float H,
					 float txL,float tyL,float tW,float tH,
					 DWORD CLU,DWORD CRU,DWORD CLD,DWORD CRD,
					 int TextureID,int ShaderID,byte Rot);
void OneAlarmOptions::Draw(){
	int dt=TrueTime-LastAlarmTime;
	if(dt>0 && dt<Duration_ms){
		SetRLCWindow(minix,miniy,MiniLx,MiniLy,SCRSizeX);
		int x0=GetXOnMiniMap(AlarmX,AlarmY);
		int y0=GetYOnMiniMap(AlarmX,AlarmY);	
		float R=MinPulsingRadius+sin(c_PI*2*float(dt)*PulsingCount/Duration_ms)*(MaxPulsingRadius-MinPulsingRadius);		
		static int AlarmShader=IRS->GetShaderID("hud_smooth");
		int w=dt*255/Duration_ms;
		DWORD C=MixDWORD(StartPulsingColor,FinalPulsingColor,255-w,w);
		if(Texture!=-1)DrawTexturedBar(x0-R,y0-R,R*2,R*2,0,0,1,1,C,C,C,C,Texture,AlarmShader,byte(Rotation*dt*256.0f/Duration_ms));						
		SetRLCWindow(0,0,COPYSizeX,RSCRSizeY,SCRSizeX);
	}
}
void AddUnlimitedEffect(int x,int y,int id);
void OneAlarmOptions::SetAlarm(int x,int y){
	AlarmX=x;
	AlarmY=y;
	LastAlarmTime=TrueTime;
	if(Sound!=-1)AddUnlimitedEffect(x,y,Sound);
}
void OneAlarmOptions::TryToAddAlarm(int x,int y){
	if(TrueTime-LastAlarmTime<Interval_ms)return;
    Vector3D V(x,y,GetTotalHeight(x,y));
	WorldToScreenSpace(V);
	if(V.x<-30 || V.y<-30 || V.x>RealLx+30 || V.y>RealLy+30){
		SetAlarm(x,y);
	}
}
void AddAlarmEditor(){
	ALARMS.reset_class(&ALARMS);
	ALARMS.SafeReadFromFile("Dialogs\\alarms.xml");
    AddStdEditor("Alarms editor",&ALARMS,"Dialogs\\alarms.xml",RCE_DEFAULT);
}
void ResetAlarms(){
	ALARMS.AttackAlarm.reset();
	ALARMS.CaptureAlarm.reset();
}
void DrawAlarms(){
	ALARMS.AttackAlarm.Draw();
	ALARMS.CaptureAlarm.Draw();
}
void TryAlarm(int x,int y,int reason){
	if(reason==0){//attack
        ALARMS.AttackAlarm.TryToAddAlarm(x,y);
	}else{//capture?
		ALARMS.CaptureAlarm.TryToAddAlarm(x,y);
	}
}