#ifndef __WEAPONSYSTEM_H__
#define __WEAPONSYSTEM_H__

//#include "ua_Modificators.h"

#pragma once
#pragma pack ( push )
#pragma pack ( 1 ) 

#include "unitability.h"
#include "SuperEditor.h"
#include "QParser.h"
//==================================================================================================================//

class WeaponParams;

class TargetDesignation : public BaseClass
{
public:
	int UnitIndex;
	int x;
	int y;
	int z;
	WeaponParams* WP;

	SAVE(TargetDesignation);
		REG_MEMBER(_int,UnitIndex);
		REG_MEMBER(_int,x);
		REG_MEMBER(_int,y);
		REG_MEMBER(_int,z);
	ENDSAVE;

	TargetDesignation();
	void Duplicate(TargetDesignation* Dest){
		if(Dest){
			Dest->UnitIndex=UnitIndex; Dest->x=x; Dest->y=y; Dest->z=z; Dest->WP=WP;
		}
	}
	
};
//==================================================================================================================//
class AdditionalWeaponParams : public BaseClass
{
public:
	AdditionalWeaponParams();
	int Damage;
	int AttType;
	int Radius;
	int NI;
	int N;
	int EffectDuration;
	SAVE(AdditionalWeaponParams);
		REG_MEMBER(_int,Damage);
		REG_MEMBER(_int,AttType);
		REG_MEMBER(_int,Radius);
		REG_MEMBER(_int,NI);
		REG_MEMBER(_int,N);
		REG_MEMBER(_int,EffectDuration);
	ENDSAVE
};
//==================================================================================================================//
class WeaponModificator;
class HeroVariableStorage;
class WeaponParams : public BaseClass
{
public:
	WeaponParams();

	_str WeaponModificatorName;
	WeaponModificator* WeaponModificatorP;

	//
	DynArray<DWORD> TrigSerial;
	DynArray<int> TrigN;
	DynArray<DWORD> TimeSerial;
	DynArray<int> TimeDelay;
	//

	TargetDesignation From;
	int OwnerWeaponIndex;
	//int Damage;
	//int AttType;

	TargetDesignation To;

	int BirthTime;
	int LastMoveTime;
	int TraveledDistance;
	int x;
	int y;
	int z;
	bool NeedDelete;
	
	int V;
	int VDir;

	int Dir;
	int DirZ;
	
	ClassPtr<AdditionalWeaponParams> AdditionalParams;
	
    //  Silver, 3.08.2004
    //  Current direction of the missile, used for rendering purposes only
    float fiDir;    //  yaw
    float fiOrt;    //  pitch
	//---additional graphics params---//
	float Scale;
	DWORD Color;
	float ExtraFiDir;
	float ExtraFiOrt;
	float ExtraDir;
	//--------------------------------//

	// хоть раз попало за пределы видимости
	// использовать только в DrawOne для каждого игрока свое
	bool OutOfScreen[8];
	bool OnceDrawed[8];

	int Serial;
	bool OnceProcesed;

	word ReachedUnitID;
	word ReachedUnitSN;

	bool TargetIsReached;

	bool PermanentDraw;

	SAVE(WeaponParams){
		
		REG_ENUM(_strindex,WeaponModificatorName,WeaponModificatorEnum);
		
		//REG_MEMBER(_str,WeaponModificatorName);
		REG_AUTO(From);
		REG_MEMBER(_int,OwnerWeaponIndex);
		//REG_MEMBER(_int,Damage);
		//REG_MEMBER(_int,AttType);
		REG_AUTO(To);
		REG_MEMBER(_int,BirthTime);
		REG_MEMBER(_int,LastMoveTime);
		REG_MEMBER(_int,TraveledDistance);
		REG_MEMBER(_int,x);
		REG_MEMBER(_int,y);
		REG_MEMBER(_int,z);
		REG_MEMBER(_int,V);
		REG_MEMBER(_int,Dir);
		REG_MEMBER(_int,DirZ);
		REG_AUTO(AdditionalParams);
		REG_MEMBER(_int,Serial);
		REG_MEMBER(_bool,OnceProcesed);

		//REG_MEMBER(_int,Vx);
		//REG_MEMBER(_int,Vy);
		//REG_MEMBER(_int,Vz);
		
		REG_MEMBER(_WORD,ReachedUnitID);
		REG_MEMBER(_WORD,ReachedUnitSN);

		REG_MEMBER(_bool,TargetIsReached);

		REG_MEMBER(_bool,PermanentDraw);

	}ENDSAVE;

	bool Process();
	bool Draw();
	bool IsOnScreen();
	HeroVariableStorage* GetHeroStorage();
private:
	HeroVariableStorage* HVS;
	bool CheckHero;
};
//==================================================================================================================//
class PointModificator : public BaseClass
{
public:
	SAVE(PointModificator);
	ENDSAVE;

	virtual bool MakeOneStep(WeaponParams* WP);
	virtual bool CanDraw(WeaponParams* WP);
	virtual bool Draw(WeaponParams* WP);

	//virtual bool ForceSimplification() { return true; }
};
//==================================================================================================================//
class WeaponEvent : public BaseClass
{
public:
	SAVE(WeaponEvent);
	ENDSAVE;

	virtual bool Check(WeaponParams* WP);
};
//==================================================================================================================//
class WeaponEventPtr: public ClassPtr<WeaponEvent>
{
public:
	SAVE(WeaponEventPtr){
		REG_PARENT(ClassPtr<WeaponEvent>);
	}ENDSAVE;
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="%$0$%";
		return true;
	};
	virtual bool ForceSimplification() { return true; }
};
class WeaponModificatorList: public ClassArray<PointModificator>
{
public:
	int GetExpansionRules();
	virtual bool ForceSimplification() { return true; }
};

class WeaponProcess : public BaseClass
{
public:
	//ClassPtr<WeaponEvent> Event;
	WeaponEventPtr Event;
	WeaponModificatorList WeaponModificators;
	SAVE(WeaponProcess);
		REG_AUTO(Event);
		REG_AUTO(WeaponModificators);
	ENDSAVE;
	bool Check(WeaponParams* WP);
	bool Process(WeaponParams* WP);
	bool CanDraw(WeaponParams* WP);
	bool Draw(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags);
};
//==================================================================================================================//
class WP_list:public ClonesArray<WeaponProcess>{
public:
	virtual bool ForceSimplification(){	return true; }
};
class WeaponModificator : public be_CVariableBase
{
public:
	//_str Name;
	Weapon* WOwner;
	WeaponModificator();
	~WeaponModificator();
	WP_list WPL;
	SAVE(WeaponModificator);
		REG_BASE(be_CVariableBase);
		REG_AUTO(Name);
		REG_AUTO(WPL);
	ENDSAVE;

	bool Process(WeaponParams* WP);
	bool Draw(WeaponParams* WP);
	virtual bool CheckIfObjectIsGlobal(){return true;}
	const char* GetThisElementView(const char* LocalName);
	//virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags);
};
//==================================================================================================================//
class WeaponArray:public ClonesArray<WeaponModificator>{
public:
	bool SafeReadFromFile(const char* Name);
	bool ReadFromFile(const char* Name);
	bool WriteToFile     (const char* Name);
};
class WeaponSystem : public BaseClass
{
public:
	WeaponArray AllWeaponModificators;
	ClonesArray<WeaponParams> ActiveWeapons;
	DynArray<Weapon*> Weapons;
	int LastSerial;
	
	SAVE(WeaponSystem);
		REG_AUTO(AllWeaponModificators);
		REG_AUTO(ActiveWeapons);
		REG_MEMBER(_int,LastSerial);
	ENDSAVE;

	WeaponSystem(void);

	void RefreshEnumerator();
	void Process();
	void Draw();

	bool LoadAllWeaponModificators(char* FileName);
	void ClearAllActiveWeapons();
	void AddActiveWeapon(WeaponParams* W);
	//virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags);
};
//==================================================================================================================//
//==================================================================================================================//
class DrawOne : public PointModificator
{
public:
	__constructor(DrawOne){
		Frame=0;
		ScaleToRadius=0;
		DisableIfOutOfScreen=true;
		PermanentDraw=false;
	}
	NewAnimation Anim;
	int Frame;
	int ScaleToRadius;
	int AddDirZ;
	byte DrawingPosition;
	byte TargetOfEffect;
	bool AttachStartPointToBone;
	bool UseDirectionFromBone;
	_str BoneNameMask;
	
	bool DisableIfOutOfScreen;
	bool PermanentDraw;

	SAVE(DrawOne){
		REG_PARENT(PointModificator);
		REG_AUTO(Anim);
		//REG_MEMBER(_int,Frame);
		//
		//REG_MEMBER(_int,AddDirZ);
		REG_ENUM(_byte_index,DrawingPosition,EFFDRAWPOS);
		REG_ENUM(_byte_index,TargetOfEffect,TARGETLIST);

		REG_MEMBER(_int,ScaleToRadius);

		REG_MEMBER(_bool,PermanentDraw);
		REG_MEMBER(_bool,DisableIfOutOfScreen);
		REG_MEMBER(_bool,AttachStartPointToBone);
		SAVE_SECTION(2);
		REG_MEMBER(_bool,UseDirectionFromBone);
		REG_AUTO(BoneNameMask);
	}ENDSAVE;
	DWORD GetClassMask(){
		return AttachStartPointToBone?2:1;
	}

	virtual bool CanDraw(WeaponParams* WP);
	virtual bool Draw(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="Draw(%$Anim,graphparam$%)";
		return true;
	}
};
//===========================GRAPHICAL MODIFICATORS=====================================//
class DurationPoint:public BaseClass{
public:
	bool ForceSimplification(){
		return true;
	}
	int type;
	SAVE(DurationPoint);
		REG_ENUM_EX(_index,type,WS_GRMODIF_TYPE_DUR,PositionMeaning);
	ENDSAVE;
	virtual int GetDuration(WeaponParams* WP);
};
class ScalePoint:public BaseClass{
public:
	bool ForceSimplification(){
		return true;
	}
	Delimiter D;
	float FinalScale;
    float   Duration;	
	SAVE(ScalePoint);		
		REG_MEMBER_EX(_float0_10,FinalScale,Scale);
		REG_MEMBER_EX(_float01,Duration,Position);
		REG_AUTO(D);
	ENDSAVE;
	bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="%$Scale,%.02f$%";
		return true;
	}
};
class ScalePointArray:public ClonesArray<ScalePoint>{
	bool ForceSimplification(){
		return true;
	}
};
class graph_ChangeScale:public PointModificator{
public:
	float StartScale;
	Delimiter D;
	ScalePointArray ScalePoints;
	DurationPoint	  typeD;
	bool Use_Zoom;
	int Zoom;
	SAVE(graph_ChangeScale);
		REG_PARENT(PointModificator);
		REG_AUTO(typeD);
		REG_MEMBER(_float0_10,StartScale);		
		REG_AUTO(D);
		REG_AUTO(ScalePoints);
		SAVE_SECTION(0x00000001);
			REG_MEMBER(_bool,Use_Zoom);
		SAVE_SECTION(0x00000004);
			REG_MEMBER(_bool,Use_Zoom);
			REG_MEMBER(_int,Zoom);
		REG_CLASS(ScalePoint);
	ENDSAVE;
	DWORD GetClassMask();
	virtual bool CanDraw(WeaponParams* WP);
	virtual bool Draw(WeaponParams* WP);
	bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		_str s="NULL";
		int tx,ty,dtx,dty;
		tx=0;
		dtx=0;
		ty=-StartScale*2;
		dty=StartScale*2;
		int nn=ScalePoints.GetAmount();
		s="graph_ChangeScale( {plot 100 20 C FF00FF00 M 0 0 L 100 0 M 100 0 L 0 20 M 0 20 L -100 0 M -100 0 L 0 -20 C FFFF0000";
		if (nn>0) {
			for(int pp=0;pp<=nn-1;pp++){
				s+=" M ";
				s+=tx;
				s+=" ";
				s+=ty;
				s+=" L ";
				tx=ScalePoints[pp]->Duration*100-dtx;
				dtx=ScalePoints[pp]->Duration*100;
				ty=dty-ScalePoints[pp]->FinalScale*2;
				dty=ScalePoints[pp]->FinalScale*2;
				s+=tx;
				s+=" ";
				s+=ty;
			}
			s+=" M ";
			s+=tx;
			s+=" ";
			s+=ty;
			s+=" L ";
			s+=100-dtx;
			s+=" 0";
		}
		s+="} )";
		dest=s.pchar()/*"graph_ChangeScale(%$StartScale,%.02f$%->%$ScalePoints,->$%)"*/;
		return true;
	}
};
class ColorPoint:public BaseClass{
public:
	bool ForceSimplification(){
		return true;
	}
	Delimiter D;
	DWORD FinalColor;
	float   Duration;
	SAVE(ColorPoint);		
		REG_MEMBER_EX(_color,FinalColor,Color);
		REG_MEMBER_EX(_float01,Duration,Position);
		REG_AUTO(D);
	ENDSAVE;
	bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		char buffr[128];
		sprintf(buffr,"%x",FinalColor);
		dest=/*"%$Color$%";//*/buffr;
		return true;
	}
};
class ColorPointArray:public ClonesArray<ColorPoint>{
public:
	bool ForceSimplification(){
		return true;
	}
};
class graph_ChangeColor:public PointModificator{
public:
	DWORD StartColor;
	ColorPointArray ColorPoints;
	DurationPoint   typeD;
	Delimiter D;
	bool Use_Zoom;
	int Zoom;
	SAVE(graph_ChangeColor);
		REG_PARENT(PointModificator);
		REG_AUTO(typeD);
		REG_MEMBER(_color,StartColor);
		REG_AUTO(D);
		REG_AUTO(ColorPoints);		
		SAVE_SECTION(0x00000001);
			REG_MEMBER(_bool,Use_Zoom);
		SAVE_SECTION(0x00000004);
			REG_MEMBER(_bool,Use_Zoom);
			REG_MEMBER(_int,Zoom);
		REG_CLASS(ColorPoint);
	ENDSAVE;
	DWORD GetClassMask();
	virtual bool CanDraw(WeaponParams* WP);
	DWORD GetCurrentColor(int step, int CurPos, int koef);
	virtual bool Draw(WeaponParams* WP);
	bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
/*		char buffer[128];
		_str dsch;
		sprintf(buffer,"%x",StartColor);
		dsch="graph_ChangeColor(";
		dsch+=buffer;
		dsch+="->%$ColorPoints,->$%)";*/
//
		_str s="NULL";
		int nn=ColorPoints.GetAmount();
		DWORD cccc;
		cccc=StartColor;
		s="graph_ChangeColor( {plot 100 20";
		char sc1[128],sc2[128];
		sprintf(sc1,"%x",cccc);
		if (nn>0) {
			for(int pp=0;pp<=nn-1;pp++){
				s+=" D 0 0 ";
				if(pp!=0)
					s+=(ColorPoints[pp]->Duration-ColorPoints[pp-1]->Duration)*100;
				else
					s+=ColorPoints[pp]->Duration*100;
				s+=" 20 ";
				cccc=ColorPoints[pp]->FinalColor;
				sprintf(sc2,"%x",cccc);
				s+=sc1;
				s+=" ";
				s+=sc2;
				s+=" ";
				s+=sc1;
				s+=" ";
				s+=sc2;
				s+=" M ";
				if(pp!=0)
					s+=(ColorPoints[pp]->Duration-ColorPoints[pp-1]->Duration)*100;
				else
					s+=ColorPoints[pp]->Duration*100;
				s+=" 0";
				sprintf(sc1,"%x",cccc);
			}
			s+=" C ";
			s+=sc2;
			s+=" B 0 0 ";
			s+=100-ColorPoints[nn-1]->Duration*100;
			s+=" 20";
		}else{
			s+=" C ";
			s+=sc1;
			s+=" B 0 0 100 20";
		}
		s+="} )";
//
		dest=s.pchar()/*"graph_ChangeColor(%$StartColor$%->%$ColorPoints,->$%)"//dsch.pchar()*/;
		return true;
	}
};
class DirPoint:public BaseClass{
public:
	bool ForceSimplification(){
		return true;
	}
	Delimiter D;
	float FinalFiDir;
	bool cycle1;
	float FinalFiOrt;
	bool cycle2;
	float FinalDir;
	bool cycle3;
	float   Duration;
	SAVE(DirPoint);		
		REG_MEMBER_EX(_float,FinalFiDir,FiDir);
		REG_MEMBER_EX(_bool,cycle1,CycleFiDir);
		REG_MEMBER_EX(_float,FinalFiOrt,FiOrt);
		REG_MEMBER_EX(_bool,cycle2,CycleFiOrt);
		REG_MEMBER_EX(_float,FinalDir,Rotate);
		REG_MEMBER_EX(_bool,cycle3,CycleRotate);
		REG_MEMBER_EX(_float01,Duration,Position);
		REG_AUTO(D);
	ENDSAVE;
	bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="[%$FiDir$%,%$FiOrt$%,%$Rotate$%]";
		return true;
	}
};
class DirPointArray:public ClonesArray<DirPoint>{
public:
	bool ForceSimplification(){
		return true;
	}
};
class graph_Rotate:public PointModificator{
public:
	float StartFiDir;
	float StartFiOrt;
	float StartDir;
	DirPointArray DirPoints;
	DurationPoint   typeD;
	Delimiter D;
	bool Use_Zoom;
	int Zoom;
	SAVE(graph_Rotate);
		REG_PARENT(PointModificator);
		REG_AUTO(typeD);
		REG_MEMBER(_float,StartFiDir);
		REG_MEMBER(_float,StartFiOrt);
		REG_MEMBER(_float,StartDir);
		REG_AUTO(D);
		REG_AUTO(DirPoints);		
		SAVE_SECTION(0x00000001);
			REG_MEMBER(_bool,Use_Zoom);
		SAVE_SECTION(0x00000004);
			REG_MEMBER(_bool,Use_Zoom);
			REG_MEMBER(_int,Zoom);
		REG_CLASS(DirPoint);
	ENDSAVE;
			DWORD GetClassMask();
	virtual bool CanDraw(WeaponParams* WP);
	virtual bool Draw(WeaponParams* WP);
	bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		_str s="NULL";
		char sc1[128],sc2[128];
		int xx,yy,xx1,yy1,nn;
		xx=10;yy=0;
		s="graph_Rotate( [ {plot 30 30 C FF00FF00 R 15 15 15 M 15 15 L ";
		xx1=/*xx*/+15.0*cos(StartFiDir*c_DoublePI);
		sprintf(sc1,"%d",xx1);
		s+=sc1;
		s+=" ";
		yy1=/*yy*/-15.0*sin(StartFiDir*c_DoublePI);
		sprintf(sc2,"%d",yy1);
		s+=sc2;
		s+="} , {plot 30 30 C FF00FF00 R 15 15 15 M 15 15 L ";
		xx1=/*xx*/+15.0*cos(StartFiOrt*c_DoublePI);
		sprintf(sc1,"%d",xx1);
		s+=sc1;
		s+=" ";
		yy1=/*yy*/-15.0*sin(StartFiOrt*c_DoublePI);
		sprintf(sc2,"%d",yy1);
		s+=sc2;
		s+="} , {plot 30 30 C FF00FF00 R 15 15 15 M 15 15 L ";
		xx1=/*xx*/+15.0*cos(StartDir*c_DoublePI);
		sprintf(sc1,"%d",xx1);
		s+=sc1;
		s+=" ";
		yy1=/*yy*/-15.0*sin(StartDir*c_DoublePI);
		sprintf(sc2,"%d",yy1);
		s+=sc2;
		s+="} ]";
		nn=DirPoints.GetAmount();
		if (nn>0) {
			for(int pp=0;pp<nn;pp++){
				s+="->[ {plot 30 30 C FF00FF00 R 15 15 15 ";
				if (DirPoints[pp]->cycle1) {
					s+="FF5F0000 ";
				}
				s+="M 15 15 L ";
				xx1=/*xx*/+15.0*cos(DirPoints[pp]->FinalFiDir*c_DoublePI);
				sprintf(sc1,"%d",xx1);
				s+=sc1;
				s+=" ";
				yy1=/*yy*/-15.0*sin(DirPoints[pp]->FinalFiDir*c_DoublePI);
				sprintf(sc2,"%d",yy1);
				s+=sc2;
				s+="} , {plot 30 30 C FF00FF00 R 15 15 15 ";
				if (DirPoints[pp]->cycle2) {
					s+="FF5F0000 ";
				}
				s+="M 15 15 L ";
				xx1=/*xx*/+15.0*cos(DirPoints[pp]->FinalFiOrt*c_DoublePI);
				sprintf(sc1,"%d",xx1);
				s+=sc1;
				s+=" ";
				yy1=/*yy*/-15.0*sin(DirPoints[pp]->FinalFiOrt*c_DoublePI);
				sprintf(sc2,"%d",yy1);
				s+=sc2;
				s+="} , {plot 30 30 C FF00FF00 R 15 15 15 ";
				if (DirPoints[pp]->cycle3) {
					s+="FF5F0000 ";
				}
				s+="M 15 15 L ";
				xx1=/*xx*/+15.0*cos(DirPoints[pp]->FinalDir*c_DoublePI);
				sprintf(sc1,"%d",xx1);
				s+=sc1;
				s+=" ";
				yy1=/*yy*/-15.0*sin(DirPoints[pp]->FinalDir*c_DoublePI);
				sprintf(sc2,"%d",yy1);
				s+=sc2;
				s+="} ]";
			}
		}
		s+=" )";
		dest=s.pchar();// graph_Rotate([%$StartFiDir$%,%$StartFiOrt$%,%$StartDir$%]->%$DirPoints,->$%)";
		return true;
	}
};
//==================================================================================================================//
class SelfMurder : public PointModificator
{
public:
	SAVE(SelfMurder);
		REG_PARENT(PointModificator);
	ENDSAVE;
	virtual bool MakeOneStep(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="SelfMurder";
		return true;
	}
};
//==================================================================================================================//
class ChangeAttachedEffectMask : public PointModificator
{
public:
    DWORD Mask;
    SAVE(ChangeAttachedEffectMask);
    REG_PARENT(PointModificator);
    REG_MEMBER(_int,Mask);
    ENDSAVE;
    virtual bool MakeOneStep(WeaponParams* WP);
    virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
        dest="ChangeAttachedEffectMask(%$Mask$%)";
        return true;
    }
};
//==================================================================================================================//
class StaticMotion : public PointModificator
{
public: 
	StaticMotion();
	int Vx;
	int Ax;
	int Vy;
	int Ay;
	int Vz;
	int Az;
	SAVE(StaticMotion);
		REG_PARENT(PointModificator);
		REG_MEMBER(_int,Vx);
		REG_MEMBER(_int,Ax);
		REG_MEMBER(_int,Vy);
		REG_MEMBER(_int,Ay);
		REG_MEMBER(_int,Vz);
		REG_MEMBER(_int,Az);
	ENDSAVE;
	bool FirstStep;
	int Vmx;
	int Amx;
	int Vmy;
	int Amy;	
	word Dir;
	virtual bool MakeOneStep(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="StaticMotion(V=(%$Vx$%,%$Vy$%,%$Vz$%),A=(%$Ax$%,%$Ay$%,%$Az$%))";
		return true;
	}
};
//==================================================================================================================//
class BalisticMotion : public PointModificator
{
public:
	BalisticMotion();
	
	bool HorizontalFly;
	int ConstSpeed;

	int ConstHieght;

	int g;
	bool SetTargetHieghtOnGround;

	bool StopInDestPoint;
	bool StopOnGround;

	SAVE(BalisticMotion){
		REG_PARENT(PointModificator);
		REG_MEMBER(_bool,HorizontalFly);
		REG_MEMBER(_int,ConstSpeed);

		SAVE_SECTION(1);
		REG_MEMBER(_int,ConstHieght);

		SAVE_SECTION(2);
		REG_MEMBER(_int,g);
		REG_MEMBER(_bool,SetTargetHieghtOnGround);

		SAVE_SECTION(0xFFFFFFFF);
		REG_MEMBER(_bool,StopInDestPoint);
		REG_MEMBER(_bool,StopOnGround);
	}ENDSAVE;

	DWORD GetClassMask(){
		DWORD result=2;
		if(ConstSpeed==0)
			result|=1;
		if(HorizontalFly) 
			result=4;
		return result;
	}

	virtual bool MakeOneStep(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="BalisticMotion(V=%$ConstSpeed$% G=%$g$%)";
		return true;
	}
};
class pm_Roam : public PointModificator
{
public:
	int MaxSpeed;

	SAVE(pm_Roam){
		REG_PARENT(PointModificator);
		REG_MEMBER(_int,MaxSpeed);
	}ENDSAVE;

	pm_Roam();
	virtual bool MakeOneStep(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="RoamMotion(V=%$MaxSpeed$%)";
		return true;
	}
};

//==================================================================================================================//
class Jump : public PointModificator
{
public:
	Jump();
	
	int JumpDist;
	bool JumpToEnd;
	bool SetTargetHeightOnGround;
	bool DestinationIsObject;

	SAVE(Jump);
		REG_PARENT(PointModificator);
		REG_MEMBER(_int,JumpDist);
		REG_MEMBER(_bool,JumpToEnd);
		REG_MEMBER(_bool,SetTargetHeightOnGround);
		REG_MEMBER(_bool,DestinationIsObject);
	ENDSAVE;

	virtual bool MakeOneStep(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="Jump(Dist=%$JumpDist$%,FromEnd=%$JumpToEnd$%)";
		return true;
	}

};
//==================================================================================================================//
class Motion :  public PointModificator
{
public:
	SAVE(Motion);
		REG_PARENT(PointModificator);
	ENDSAVE;
	virtual bool MakeOneStep(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="Motion()";
		return true;
	}
};
//==================================================================================================================//
class HarmonicMotion : public PointModificator
{
public:
	HarmonicMotion();
	int Hx;
	int Tx;
	int Dx;
	int Hy;
	int Ty;
	int Dy;
	int Hz;
	int Tz;
	int Dz;
	SAVE(HarmonicMotion);
		REG_PARENT(PointModificator);
		REG_MEMBER(_int,Hx);
		REG_MEMBER(_int,Tx);
		REG_MEMBER(_int,Dx);
		REG_MEMBER(_int,Hy);
		REG_MEMBER(_int,Ty);
		REG_MEMBER(_int,Dy);
		REG_MEMBER(_int,Hz);
		REG_MEMBER(_int,Tz);
		REG_MEMBER(_int,Dz);
	ENDSAVE;
	virtual bool MakeOneStep(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="HarmonicMotion()";
		return true;
	}
};
//==================================================================================================================//
class FollowUnit : public PointModificator 
{
public:
	FollowUnit();
	int F;
	SAVE(FollowUnit);
		REG_PARENT(PointModificator);
		REG_MEMBER(_int,F);
	ENDSAVE;
	virtual bool MakeOneStep(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="FollowUnit()";
		return true;
	}
};
//==================================================================================================================//
class BirthNew : public PointModificator
{
public:
	BirthNew();
	_str NewWeaponModificator;
	int DamageChange;
	int BirthPause;
	//bool LeaveFromPoint;
	int WhereDoesItBorns;
	int Fr_AddX;
	int Fr_AddY;
	int Fr_AddZ;
	int To_RandomUnitInRadius;
	int To_RandomFrendlyUnitInRadius;
	int To_RandomEnemyUnitInRadius;
	int To_RandomPosInRadius;
	int To_AddX;
	int To_AddY;
	int To_AddZ;
	SAVE(BirthNew);
		REG_PARENT(PointModificator);
		//REG_ENUM(_index,NewWeaponModificator,WeaponModificatorEnum);
		REG_ENUM(_strindex,NewWeaponModificator,WeaponModificatorEnum);
		REG_MEMBER(_int,DamageChange);
		REG_MEMBER(_int,BirthPause);
		//REG_MEMBER(_bool,LeaveFromPoint);
		REG_ENUM(_index,WhereDoesItBorns,WeaponSrc);
		REG_MEMBER(_int,Fr_AddX);
		REG_MEMBER(_int,Fr_AddY);
		REG_MEMBER(_int,Fr_AddZ);
		REG_MEMBER(_int,To_RandomUnitInRadius);
		REG_MEMBER(_int,To_RandomFrendlyUnitInRadius);
		REG_MEMBER(_int,To_RandomEnemyUnitInRadius);
		REG_MEMBER(_int,To_RandomPosInRadius);
		REG_MEMBER(_int,To_AddX);
		REG_MEMBER(_int,To_AddY);
		REG_MEMBER(_int,To_AddZ);
	ENDSAVE;
	virtual bool MakeOneStep(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="BirthNew(%$NewWeaponModificator$%)";
		return true;
	}
};
//==================================================================================================================//
class TargetFinder : public BaseClass
{
public:
	SAVE(TargetFinder);
	ENDSAVE;

	virtual void Init(WeaponParams* WP) {}
	virtual bool GetTargetDesignation(WeaponParams* WP, int N,TargetDesignation* TD) {return false;}
	virtual void SetMaxUnits(){};
};

//==================================================================================================================//
class WeaponInRadius : public TargetFinder
{
private:
	DynArray<WeaponParams*> Finded;
public:
	UserFriendlyNumericalReturner Radius;
	ClassRef<WeaponModificator> Weapon;

	SAVE(WeaponInRadius){
		REG_PARENT(TargetFinder);

		REG_AUTO(Radius);
		//REG_AUTO(MinRadius);
		//REG_AUTO(MaxUnits);
		REG_AUTO(Weapon);
	}ENDSAVE;

	WeaponInRadius();
	virtual void Init(WeaponParams* WP);
	virtual bool GetTargetDesignation(WeaponParams* WP, int N,TargetDesignation* TD);

	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="WeaponInRadius: R=%$Radius$%,Weapon=%$Weapon$%)";
		return true;
	}
};
class SpriteInRadius : public TargetFinder {
private:
	DynArray<int> FindedUnits;
	bool FillList;
	word Owner;
public:
	SpriteInRadius();
	UserFriendlyNumericalReturner Radius;
	UserFriendlyNumericalReturner MinRadius;
	UserFriendlyNumericalReturner MaxUnits;

	bool DeleteSprite;

	bool UseSpriteType;
	int SpriteType;

	bool SprGroupTrees;
	bool SprGroupStones;

	int  UnitsSelectionRule;

	SAVE(SpriteInRadius){
		REG_PARENT(TargetFinder);
		REG_AUTO(Radius);
		REG_AUTO(MinRadius);
		REG_AUTO(MaxUnits);
		REG_MEMBER(_bool,DeleteSprite);
		REG_MEMBER(_bool,UseSpriteType);
		SAVE_SECTION(1);
		REG_ENUM(_index,SpriteType,TREESTYPES);
		SAVE_SECTION(2);
		REG_MEMBER(_bool,SprGroupTrees);
		REG_MEMBER(_bool,SprGroupStones);
		SAVE_SECTION(0xFFFFFFFF);
		REG_ENUM(_index,UnitsSelectionRule,UNITSSELRULE);//RandomUnits,NearestUnits,Unsorted
	}ENDSAVE;

	virtual bool GetTargetDesignation(WeaponParams* WP, int N,TargetDesignation* TD);
//	static bool CheckUnitsInRadius(OneObject* OB,void* param);

	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="SpriteInRadius: R=%$Radius$%,MaxSprites=%$MaxUnits$%)";
		return true;
	}

	DWORD GetClassMask(){
		if(UseSpriteType) return 1;
			else return 2;
		return 0xFFFFFFFF;
	}
};
class UnitsInRadius : public TargetFinder
{
private:
	DynArray<int> FindedUnits;
	bool FillList;
	word Owner;
public:
	bool TargetUnit;
	bool Caster;
	bool Units;
	bool Buildings;

	int N;

	UserFriendlyNumericalReturner MinRadius;
	UserFriendlyNumericalReturner Radius;
	UserFriendlyNumericalReturner Sector;

	bool MaxUnitsDependRadius;
	UserFriendlyNumericalReturner MaxUnits;	
	
	bool Frendly;
	bool Enemy;

	bool LiveBody;
	bool LiveWounded;	// раненый
	bool DeadBody;
	bool Bones;

	int  UnitsSelectionRule;
	
	SAVE(UnitsInRadius){
		REG_PARENT(TargetFinder);

		REG_MEMBER(_bool,MaxUnitsDependRadius);
		REG_AUTO(MaxUnits);

		REG_MEMBER(_bool,TargetUnit);
		REG_MEMBER(_bool,Caster);

		REG_AUTO(MinRadius);
		REG_AUTO(Radius);
		REG_AUTO(Sector);
		
		REG_MEMBER(_bool,Frendly);
		REG_MEMBER(_bool,Enemy);

		REG_MEMBER(_bool,Units);
		REG_MEMBER(_bool,Buildings);

		REG_MEMBER(_bool,LiveBody);
		REG_MEMBER(_bool,LiveWounded);
		REG_MEMBER(_bool,DeadBody);
		REG_MEMBER(_bool,Bones);

		REG_ENUM(_index,UnitsSelectionRule,UNITSSELRULE); //RandomUnits,NearestUnits,Unsorted
	}ENDSAVE;

	UnitsInRadius();
	virtual void Init(WeaponParams* WP);
	virtual bool GetTargetDesignation(WeaponParams* WP, int N,TargetDesignation* TD);
	static bool CheckUnitsInRadius(OneObject* OB,void* param);

	virtual void SetMaxUnits();

	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(Sector.Get()){
			dest="UnitsInRadius: R=%$Radius$%,Sector=%$Sector$%,MaxUnits=%$MaxUnits$%,Frendly=%$Frendly$%,Enemy=%$Enemy$%,Rule=%$UnitsSelectionRule$%)";
		}else{
			dest="UnitsInRadius: R=%$Radius$%,MaxUnits=%$MaxUnits$%,Frendly=%$Frendly$%,Enemy=%$Enemy$%,Rule=%$UnitsSelectionRule$%)";
		}
		return true;
	}
};
class RandomPosInRadius : public TargetFinder
{
public:
	UserFriendlyNumericalReturner MinRadius;
	UserFriendlyNumericalReturner Radius;
	UserFriendlyNumericalReturner N;
	SAVE(RandomPosInRadius){
		REG_PARENT(TargetFinder);
		REG_AUTO(MinRadius);
		REG_AUTO(Radius);
		REG_AUTO(N);
	}ENDSAVE;
	virtual bool GetTargetDesignation(WeaponParams* WP, int N,TargetDesignation* TD);

	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="RandomPosInRadius: R=%$Radius$%,N=%$N$%)";
		return true;
	}
};
class UserDefinedPointCoor:public BaseClass
{
public:
	float x;
	float y;
	float z;
	SAVE(UserDefinedPointCoor){
		REG_MEMBER(_float,x);
		REG_MEMBER(_float,y);
		REG_MEMBER(_float,z);
	}ENDSAVE;
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="(%$x$%,%$y$%,%$z$%)";
		return true;
	}
};
class UserDefinedPoints : public TargetFinder
{
public:
	UserFriendlyNumericalReturner Radius;
	class _fnAddPointsInRadius:public BaseFunction{
	public:
		float Radius;
		float z;
		int NPoints;
		void EvaluateFunction(){
			UserDefinedPoints* u=get_parent<UserDefinedPoints>();
			if(u){
				for(int i=0;i<NPoints;i++){
					UserDefinedPointCoor* p=new UserDefinedPointCoor;
					float ang=2*c_PI*i/NPoints;
					p->x=Radius*cos(ang);
					p->y=Radius*sin(ang);
					p->z=z;
					u->Points.Add(p);
				}
			}
		}
		SAVE(_fnAddPointsInRadius);
			REG_PARENT(BaseFunction);
			REG_MEMBER(_float,Radius);
			REG_MEMBER(_float,z);
			REG_MEMBER(_int,NPoints);
		ENDSAVE;
	};
	_fnAddPointsInRadius AddPointsInRadius;
	class _fnClearAllPoints:public BaseFunction{
	public:
		void EvaluateFunction(){
			UserDefinedPoints* u=get_parent<UserDefinedPoints>();
			if(u){
				u->Points.Clear();				
			}
		}
		SAVE(_fnClearAllPoints);
		REG_PARENT(BaseFunction);
		ENDSAVE;
	};
	_fnClearAllPoints ClearAllPoints;
	ClonesArray<UserDefinedPointCoor> Points;
	SAVE(UserDefinedPoints)
		REG_PARENT(TargetFinder);
		REG_AUTO(Radius);
		REG_AUTO(AddPointsInRadius);
		REG_AUTO(ClearAllPoints);
		REG_AUTO(Points);
	ENDSAVE;   
	const char* GetThisElementView(const char* ){
		_str s="NULL";
//		if(Points){
			int nn=Points.GetAmountOfElements();
			if (nn>0) {
				s="{plot";
				s+=" ";
				s+=Points[0]->x;
				s+=" ";
				s+=Points[0]->y;
				for(int pp=1;pp<=nn-1;pp++){
					s+=" M ";
					s+=Points[pp]->x;
					s+=" ";
					s+=Points[pp]->y;
					s+=" L ";
					s+=1;
					s+=" ";
					s+=1;
				}
				s+="}";
			}
//		}
		return s.pchar();
	}
	virtual bool GetTargetDesignation(WeaponParams* WP, int N,TargetDesignation* TD);
};
//==================================================================================================================//
class MassModifyCollect: public BaseClass {
public:
	SAVE(MassModifyCollect){
	}ENDSAVE;

	virtual int Collect(OneObject* From) { return 0; }
};
class mmc_UnitsPrice: public MassModifyCollect {
public:
	bool GiveToOwner;
	SAVE(mmc_UnitsPrice){
		REG_PARENT(MassModifyCollect);
		REG_MEMBER(_bool,GiveToOwner);
	}ENDSAVE;

	virtual int Collect(OneObject* From);
};
class mmc_Resurrect: public MassModifyCollect {
public:
	bool OnlySameNatID;
	bool OnlySameColorID;
	SAVE(mmc_Resurrect){
        REG_MEMBER(_bool,OnlySameNatID);
		REG_MEMBER(_bool,OnlySameColorID);
		REG_PARENT(MassModifyCollect);
	}ENDSAVE;

	virtual int Collect(OneObject* From);
};
class mmc_UnitsAmount: public MassModifyCollect {
public:
	SAVE(mmc_UnitsAmount){
		REG_PARENT(MassModifyCollect);
	}ENDSAVE;

	virtual int Collect(OneObject* From);
};
class mmc_UnitsLife: public MassModifyCollect {
public:
	SAVE(mmc_UnitsLife){
		REG_PARENT(MassModifyCollect);
	}ENDSAVE;

	virtual int Collect(OneObject* From);
};


class MassModifyDistribute: public BaseClass {
public:
	vOperand Operand;

	SAVE(MassModifyDistribute){
		REG_AUTO(Operand);
	}ENDSAVE;

	virtual void Distribute(WeaponParams* WP, OneObject* To, int Amount) {}
};
class mmd_Summon: public MassModifyDistribute {
public:

	BaseClassUnitType UT;
	UserFriendlyNumericalReturner Life;
	UserFriendlyNumericalReturner CollectorMultiplicator;
	ClonesArray< ClassRef<UnitAbility> > AdditionalAbilites;

	SAVE(mmd_Summon){
		//REG_PARENT(MassModifyDistribute);
		REG_BASE(MassModifyDistribute);
		REG_AUTO(UT);
		REG_AUTO(Life);
		REG_AUTO(CollectorMultiplicator);
		REG_AUTO(AdditionalAbilites);
	}ENDSAVE;

	virtual void Distribute(WeaponParams* WP, OneObject* To, int Amount);
};
class mmd_TakeResource: public MassModifyDistribute {
public:
	int ResType;
	SAVE(mmd_TakeResource){
		REG_ENUM(_index, ResType, RESTYPE);
		REG_PARENT(MassModifyDistribute);
	}ENDSAVE;

	virtual void Distribute(WeaponParams* WP, OneObject* To, int Amount);
};
class mmd_TakeLife: public MassModifyDistribute {
public:

	SAVE(mmd_TakeLife){
		REG_PARENT(MassModifyDistribute);
	}ENDSAVE;

	virtual void Distribute(WeaponParams* WP, OneObject* To, int Amount);
};
class mmd_TakeMana: public MassModifyDistribute {
public:

	SAVE(mmd_TakeMana){
		REG_PARENT(MassModifyDistribute);
	}ENDSAVE;

	virtual void Distribute(WeaponParams* WP, OneObject* To, int Amount);
};

class /*pm_*/MassModify: public BaseClass/*PointModificator*/ {
public:
	int Result;
	ClassPtr<MassModifyCollect> Collector;
	ClassPtr<MassModifyDistribute> Distributer;
	//ClassPtr<TargetFinder> TargetList;    	

	SAVE(/*pm_*/MassModify){
		REG_AUTO(Collector);
		REG_AUTO(Distributer);
		//REG_MEMBER(_int,DamageChange);
		//REG_MEMBER(_int,BirthPause);
		//REG_ENUM(_index,WhereDoesItBorns,WeaponSrc);

		//REG_MEMBER(_bool,LeaveFromPoint);
		//REG_MEMBER(_bool,LeaveFromPoint);

		//REG_AUTO(NewTargetList);
		//REG_PARENT(PointModificator);
	}ENDSAVE;

	/*pm_*/ //MassModify();
	//virtual bool MakeOneStep(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="MassModify(%$Collector$%,%$Distributer$%)";
		return true;
	}
};
//==================================================================================================================//
class TargetFinderPtr : public ClassPtr<TargetFinder> {
public:
	SAVE(TargetFinderPtr){
		REG_PARENT(ClassPtr<TargetFinder>);
	}ENDSAVE;
	virtual bool ForceSimplification() { return true; }
};

class v_LandTypeMask : public BaseClass{
public:
};

class MassBirthNew: public PointModificator {
public:
	__constructor(MassBirthNew ){
		Probability=100;
		DamageChange=100;
		BirthPause=0;
	}

	//_str NewWeaponModificator;
	//int Probability;
	UserFriendlyNumericalReturner Probability;
	ClassRef<WeaponModificator> NewWeaponModificator;
	ClassRef<UnitAbility> NewUnitAbility;
	int DamageChange;
	int BirthPause;

	int WhereDoesItBorns;
	int WhereDoesTheyGo;

	int LockType;

	TargetFinderPtr NewTargetList;
	MassModify TargetModify;

	SAVE(MassBirthNew){
		//REG_ENUM(_strindex,NewWeaponModificator,WeaponModificatorEnum);
		//REG_MEMBER(_int,Probability);
		REG_AUTO(Probability);

		REG_AUTO(NewWeaponModificator);
		REG_AUTO(NewUnitAbility);

		REG_MEMBER(_int,DamageChange);
		REG_MEMBER(_int,BirthPause);

		REG_ENUM(_index,WhereDoesItBorns,WeaponSrc);
		REG_ENUM(_index,WhereDoesTheyGo,WeaponDst);
		
		REG_ENUM(_index,LockType,LOCKTYPE);

		REG_AUTO(NewTargetList);
		REG_AUTO(TargetModify);
		REG_PARENT(PointModificator);
	}ENDSAVE;

	virtual bool MakeOneStep(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="MassBirthNew(%$NewWeaponModificator$%,%$NewUnitAbility$%,targets=(%$NewTargetList$%))";
		return true;
	}
};
/*class Teleport: public PointModificator {
public:
	__constructor(MassBirthNew ){
		Probability=100;
		DamageChange=100;
		BirthPause=0;
	}

	//_str NewWeaponModificator;
	//int Probability;
	UserFriendlyNumericalReturner Probability;
	ClassRef<WeaponModificator> NewWeaponModificator;
	ClassRef<UnitAbility> NewUnitAbility;
	int DamageChange;
	int BirthPause;

	int WhereDoesItBorns;
	int WhereDoesTheyGo;

	int LockType;

	TargetFinderPtr NewTargetList;
	MassModify TargetModify;

	SAVE(MassBirthNew){
		//REG_ENUM(_strindex,NewWeaponModificator,WeaponModificatorEnum);
		//REG_MEMBER(_int,Probability);
		REG_AUTO(Probability);

		REG_AUTO(NewWeaponModificator);
		REG_AUTO(NewUnitAbility);

		REG_MEMBER(_int,BirthPause);

		REG_ENUM(_index,WhereDoesItBorns,WeaponSrc);
		REG_ENUM(_index,WhereDoesTheyGo,WeaponDst);
		
		REG_ENUM(_index,LockType,LOCKTYPE);

		REG_AUTO(NewTargetList);
		REG_PARENT(PointModificator);
	}ENDSAVE;

	virtual bool MakeOneStep(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="MassBirthNew(%$NewWeaponModificator$%,%$NewUnitAbility$%,targets=(%$NewTargetList$%))";
		return true;
	}
};*/
class MassDelete : public PointModificator
{
public:
	//ClassRef<WeaponModificator> Weapon;
	//ClassRef<UnitAbility> NewUnitAbility;

	WeaponInRadius WeaponList;

	SAVE(MassDelete){
		//REG_AUTO(Weapon);
		//REG_AUTO(NewUnitAbility);
		REG_AUTO(WeaponList);
		REG_PARENT(PointModificator);
	}ENDSAVE;

	MassDelete() {}
	virtual bool MakeOneStep(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="MassDelete(targets=(%$WeaponList$%))";
		return true;
	}
};
//==================================================================================================================//
class ChangeModificator : public PointModificator
{
public:
	ChangeModificator();
	_str NewWeaponModificator;
	//bool LeaveFromPoint;
	//int WhereDoesItBorns;
	SAVE(ChangeModificator);
		REG_PARENT(PointModificator);
		REG_ENUM(_strindex,NewWeaponModificator,WeaponModificatorEnum);
		//REG_MEMBER(_bool,LeaveFromPoint);
		//REG_ENUM(_index,WhereDoesItBorns,WeaponSrc);
	ENDSAVE;
	_str CheckedName;
	WeaponModificator* Wm;
	virtual bool MakeOneStep(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="ChangeWeaponModificator(%$NewWeaponModificator$%)";
		return true;
	}
};
//==================================================================================================================//
class ChangeTreesLandTypeInRadius:public PointModificator{
public:
	int R;
	int LandType;
	SAVE(ChangeTreesLandTypeInRadius);
		REG_PARENT(PointModificator);
		REG_MEMBER(_int,R);
		REG_ENUM(_index,LandType,LANDLAYERS);
	ENDSAVE;
	virtual bool MakeOneStep(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="ChangeTreesLandType(R=%$R$%, Land=%$LandType$%)";
		return true;
	}
};
//==================================================================================================================//
class pm_FogSpot : public PointModificator{
public:
	bool Clear;
	UserFriendlyNumericalReturner Radius; // 1-8

	SAVE(pm_FogSpot){
		REG_MEMBER(_bool,Clear);
		SAVE_SECTION(1);
		REG_AUTO(Radius);
		SAVE_SECTION(0xFFFFFFFF);
		REG_PARENT(PointModificator);
	}ENDSAVE;

	virtual bool MakeOneStep(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(Clear) dest="FogSpot(Clear)";
			else  dest="FogSpot(R=%$Radius$%)";
		return true;
	}
	DWORD GetClassMask(){
		if(Clear) return 2;
		return 0xFFFFFFFF;
	}
};
class pm_ApplyLandTypeSlow : public PointModificator{
public:
	pm_ApplyLandTypeSlow();
	_str From;
	_str To;
	int R1;
	int R2;
	int RiseTime;
	int GrowTime;
	int RemainTime;
	int EliminateTime;
	bool Forever;
	_str BirthWeapon;
	int BirthProbability;
	_str UndoWeapon;
	int UndoProbability;
	//
	SAVE(pm_ApplyLandTypeSlow){
		//REG_AUTO(From);
		REG_ENUM(_strindex,To,LANDLAYERS);
		REG_MEMBER(_int,R1);
		REG_MEMBER(_int,R2);
		//
		REG_MEMBER(_int,RiseTime);
		REG_MEMBER(_int,GrowTime);
		REG_MEMBER(_int,RemainTime);
		REG_MEMBER(_int,EliminateTime);
		REG_MEMBER(_bool,Forever);
		REG_AUTO(BirthWeapon);
		REG_MEMBER(_int,BirthProbability);
		REG_AUTO(UndoWeapon);
		REG_MEMBER(_int,UndoProbability);
		//
		REG_PARENT(PointModificator);
	}ENDSAVE;
	virtual bool MakeOneStep(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		//dest="ApplyLandTypeSlow(From=%$From$%,To=%$To$%)";
		dest="ApplyLandTypeSlow(To=%$To$%)";
		return true;
	}
};
class pm_ApplyLandType : public PointModificator{
public:
	_str From;
	_str To;
	int R1;
	int R2;
	SAVE(pm_ApplyLandType){
		//REG_AUTO(From);
		REG_ENUM(_strindex,To,LANDLAYERS);
		REG_MEMBER(_int,R1);
		REG_MEMBER(_int,R2);
		//
		REG_PARENT(PointModificator);
	}ENDSAVE;
	virtual bool MakeOneStep(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		//dest="ApplyLandType(From=%$From$%,To=%$To$%)";
		dest="ApplyLandTypeSlow(To=%$To$%)";
		return true;
	}
};
class pm_OnWaterClearLockPoints : public PointModificator{
public:
	int Radius;
	SAVE(pm_OnWaterClearLockPoints){
		REG_MEMBER(_int,Radius);
		//
		REG_PARENT(PointModificator);
	}ENDSAVE;
	virtual bool MakeOneStep(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="OnWaterClearLockPoints(Radius=%$Radius$%)";
		return true;
	}
};
class pm_OnWaterRestoreLockPoints : public PointModificator{
public:
	int Radius;
	SAVE(pm_OnWaterRestoreLockPoints){
		REG_MEMBER(_int,Radius);
		//
		REG_PARENT(PointModificator);
	}ENDSAVE;
	virtual bool MakeOneStep(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="OnWaterRestoreLockPoints(Radius=%$Radius$%)";
		return true;
	}
};
class pm_OnLandSetLockPoints : public PointModificator{
public:
	UserFriendlyNumericalReturner Radius;
	SAVE(pm_OnLandSetLockPoints){
		REG_AUTO(Radius);
		REG_PARENT(PointModificator);
	}ENDSAVE;
	virtual bool MakeOneStep(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="OnLandSetLockPoints(Radius=%$Radius$%)";
		return true;
	}
};
class pm_OnLandRestoreLockPoints : public PointModificator{
public:
	UserFriendlyNumericalReturner Radius;
	SAVE(pm_OnLandRestoreLockPoints){
		REG_AUTO(Radius);
		//
		REG_PARENT(PointModificator);
	}ENDSAVE;
	virtual bool MakeOneStep(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="OnLandRestoreLockPoints(Radius=%$Radius$%)";
		return true;
	}
};
class pm_ApplyStandartChasm : public PointModificator{
public:
	UserFriendlyNumericalReturner R;
	UserFriendlyNumericalReturner Duration;
	
	SAVE(pm_ApplyStandartChasm){
		REG_AUTO(R);
		REG_AUTO(Duration);
		REG_PARENT(PointModificator);
	}ENDSAVE;

	pm_ApplyStandartChasm();
	virtual bool MakeOneStep(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="ApplyStandartChasm(R=%$R$%, Duration=%$Duration$%)";
		return true;
	}
};
class pm_StealResourceFromBuildings : public PointModificator{
public:
	UserFriendlyNumericalReturner R;
	UserFriendlyNumericalReturner AmountFromEachBuilding;
	int ResType;
	SAVE(pm_StealResourceFromBuildings){
		REG_AUTO(R);
		REG_AUTO(AmountFromEachBuilding);
		REG_ENUM(_index, ResType, RESTYPE);
		REG_PARENT(PointModificator);
	}ENDSAVE;

	pm_StealResourceFromBuildings();
	virtual bool MakeOneStep(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="StealResourceFromBuildings(Amount=%$AmountFromEachBuilding$%)";
		return true;
	}
};
//==================================================================================================================//
class Polymorph : public PointModificator
{
public:
	Polymorph();
	BaseClassUnitType UT;
	//UserFriendlyNumericalReturner UnitLife;
	bool TargetFriendly;
	bool TargetEnemy;	
	UserFriendlyNumericalReturner Radius;
	int SourceAmount;
	int DestAmount;
	UserFriendlyNumericalReturner Life;
	bool AddSourceLifeToDest;	
	ClonesArray< ClassRef<UnitAbility> > AdditionalAbilites;
	SAVE(Polymorph){
		REG_PARENT(PointModificator);
		REG_AUTO(UT);
		REG_MEMBER(_bool,TargetFriendly);
		REG_MEMBER(_bool,TargetEnemy);
		REG_AUTO(Radius);
		REG_MEMBER(_int,SourceAmount);
		REG_MEMBER(_int,DestAmount);
		REG_AUTO(Life);
		REG_MEMBER(_bool,AddSourceLifeToDest);		
		REG_AUTO(AdditionalAbilites);
	}ENDSAVE;	
	virtual bool MakeOneStep(WeaponParams* WP);
	//static bool MakeDam(OneObject* OB,void* param);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="Polymorph(R=%$Radius$%,SrcN=%$SourceAmount$%,DstN=%$DestAmount$%,Life=%$Life$%,AddLife=%$AddSourceLifeToDest$%)";
		return true;
	}
};
//==================================================================================================================//
//==================================================================================================================//
class MakeDamage : public PointModificator
{
public:
	MakeDamage();

	bool TargetedUnit;
	int SplashDamageChange;
	int InRadius;
	int MaxUnits;
	bool OnlyEnemyUnits;
	bool DamageBuilding;
	int PushUnitsForce;	

	SAVE(MakeDamage){
		REG_PARENT(PointModificator);
		REG_MEMBER(_bool,TargetedUnit);
		REG_MEMBER(_int,SplashDamageChange);
		REG_MEMBER(_int,InRadius);
		REG_MEMBER(_int,MaxUnits);
		REG_MEMBER(_bool,OnlyEnemyUnits);
		REG_MEMBER(_bool,DamageBuilding);
		REG_MEMBER(_int,PushUnitsForce);
	}ENDSAVE;

	virtual bool MakeOneStep(WeaponParams* WP);
	static bool MakeDam(OneObject* OB,void* param);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(TargetedUnit){
			if(InRadius<10){
				dest="MakeDamage(Only TargetedUnit {InRadius<10||SplashDamageChange==0}: PushUnitsForce=%$PushUnitsForce$%)";
			}else{
				dest="MakeDamage(TargetedUnit and Splash: R=%$InRadius$%,MaxUnits=%$MaxUnits$%,OnlyEnemy=%$OnlyEnemyUnits$%,DamageBuilding=%$DamageBuilding$%,PushUnitsForce=%$PushUnitsForce$%)";
			}
		}else{
			dest="MakeDamage(Splash without TargetedUnit: R=%$InRadius$%,MaxUnits=%$MaxUnits$%,OnlyEnemy=%$OnlyEnemyUnits$%,DamageBuilding=%$DamageBuilding$%,PushUnitsForce=%$PushUnitsForce$%)";
		}
		return true;
	}
};
//==================================================================================================================//
class Wave : public PointModificator
{
public:
	Wave();
	int H;
	int MaxR;
	int MinR;
	int PushUnitsForce;
	int LinearWidth;
	bool OnlyEnemyUnits;
	bool Damage;
	SAVE(Wave);
		REG_PARENT(PointModificator);
		REG_MEMBER(_int,H);
		REG_MEMBER(_int,MaxR);
		REG_MEMBER(_int,MinR);
		REG_MEMBER(_int,PushUnitsForce);
		REG_MEMBER(_int,LinearWidth);
		REG_MEMBER(_bool,OnlyEnemyUnits);
		REG_MEMBER(_bool,Damage);
	ENDSAVE;
	virtual bool MakeOneStep(WeaponParams* WP);
	static bool MakeWave(OneObject* OB,void* param);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="Wave()";
		return true;
	}
};
//==================================================================================================================//
class HangUpActiveAbility : public PointModificator
{
public:
	HangUpActiveAbility();
	SAVE(MakeDamage);
		REG_PARENT(PointModificator);
	ENDSAVE;
	virtual bool MakeOneStep(WeaponParams* WP);
};
//
class UnitAbility;
//class BaseClassUnitType;
//
//==================================================================================================================//
class BirthNewAbility : public PointModificator
{
public:
	bool From;
	bool To;
	ClonesArray< ClassRef<UnitAbility> > AdditionalAbilites;
	SAVE(BirthNewAbility);
		REG_PARENT(PointModificator);
		REG_AUTO(AdditionalAbilites);
		REG_MEMBER(_bool,From);
		REG_MEMBER(_bool,To);
	ENDSAVE;
	virtual bool MakeOneStep(WeaponParams* WP);
	virtual bool GetClassPresentation(_str& dest,const char* options,DWORD Flags){
		dest="BirthNewAbility(Ability=(%$AdditionalAbilites$%))";
		return true;
	}
};
class BirthNewUnit : public PointModificator
{
public:
	BaseClassUnitType UT;
	UserFriendlyNumericalReturner UnitLife;
	ClonesArray< ClassRef<UnitAbility> > AdditionalAbilites;
	bool Illusion;
	bool SendTo;
	SAVE(BirthNewUnit);
		REG_PARENT(PointModificator);
		REG_AUTO(UT);
		REG_AUTO(UnitLife);
		REG_AUTO(AdditionalAbilites);
		REG_MEMBER(_bool,Illusion);
		REG_MEMBER(_bool,SendTo);
	ENDSAVE;
	virtual bool MakeOneStep(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="BirthNewUnit(Unit=%$UT$%,AddAbility=(%$AdditionalAbilites$%))";
		return true;
	}
};
class BirthNewUnitsFromSprites : public PointModificator
{
public:
	BirthNewUnitsFromSprites();
	BaseClassUnitType UT;
	UserFriendlyNumericalReturner MaxUnits;
	UserFriendlyNumericalReturner Radius;
	bool DeleteSprites; 
	UserFriendlyNumericalReturner UnitLife;
	ClonesArray< ClassRef<UnitAbility> > AdditionalAbilites;
	SAVE(BirthNewUnitsFromSprites);
		REG_PARENT(PointModificator);
		REG_AUTO(UT);
		REG_AUTO(MaxUnits);
		REG_AUTO(Radius);
		REG_MEMBER(_bool,DeleteSprites);
		REG_AUTO(UnitLife);
		REG_AUTO(AdditionalAbilites);
	ENDSAVE;
	virtual bool MakeOneStep(WeaponParams* WP);

	static bool CheckSprite(OneSprite* OS,void* Param);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="BirthNewUnitsFromSprites(Unit=%$UT$%,AddAbility=(%$AdditionalAbilites$%))";
		return true;
	}
};
//==================================================================================================================//
class DeleteSpritesInRadius:public PointModificator{
public:
    UserFriendlyNumericalReturner Radius;
	SAVE(DeleteSpritesInRadius);
		REG_PARENT(PointModificator);
		REG_AUTO(Radius);
	ENDSAVE;
	virtual bool MakeOneStep(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="DeleteSpritesInRadius(R=%$Radius$%)";
		return true;
	}
};
//==================================================================================================================//
class RandomizePosition:public PointModificator{
public:
	UserFriendlyNumericalReturner RandomAmplitude;
	int PointToModify;
	int AmplitudeUsage;
	SAVE(RandomizePosition);
		REG_PARENT(PointModificator);
		REG_AUTO(RandomAmplitude);
		REG_ENUM(_index,PointToModify,ModifyPt);
		REG_ENUM(_index,AmplitudeUsage,AmpUsage);
	ENDSAVE;
	virtual bool MakeOneStep(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="RandomizePosition(Amplitude=%$RandomAmplitude$%)";
		return true;
	}
};
//==================================================================================================================//
class ChangeNation : public PointModificator
{
public:
	ChangeNation();
	int FromNI;
	bool AnyEnemyNation;
	bool AnyFriendlyNation;
	int ToNI;
	int Radius;
	int NUnits;	
	bool OnlyTargetUnit;
	ClonesArray<BaseClassUnitType> TypeList;
	SAVE(ChangeNation);
		REG_PARENT(PointModificator);
		REG_MEMBER(_int,FromNI);
		REG_MEMBER(_bool,AnyEnemyNation);
		REG_MEMBER(_bool,AnyFriendlyNation);
		REG_MEMBER(_int,ToNI);
		REG_MEMBER(_int,Radius); 
		REG_MEMBER(_int,NUnits);
		REG_MEMBER(_bool,OnlyTargetUnit);
		REG_AUTO(TypeList);
	ENDSAVE;
	virtual bool MakeOneStep(WeaponParams* WP);
	static bool ChangeObjectNation(OneObject* OB,void* param);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="ChangeNation()";
		return true;
	}
};
//==================================================================================================================//
class PlaySomeSound : public PointModificator
{
public:
	PlaySomeSound(){SoundID=-1;};
	int SoundID;
	SAVE(PlaySomeSound);
	REG_PARENT(PointModificator);
	REG_ENUM(_index,SoundID,ALL_SOUNDS);
	ENDSAVE;
	virtual bool MakeOneStep(WeaponParams* WP);	
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="PlaySomeSound(%$SoundID$%)";
		return true;
	}

};
class True : public WeaponEvent
{
public:
	SAVE(True); 
		REG_PARENT(WeaponEvent);
	ENDSAVE;
	//
	virtual bool Check(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="True";
		return true;
	}
};
//==================================================================================================================//
class Trigger : public WeaponEvent{
private:
	DWORD Serial;
public:
	int N;

	SAVE(Trigger){
		REG_MEMBER(_int,N);
		REG_PARENT(WeaponEvent);
	}ENDSAVE;

	Trigger();
	virtual bool Check(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="Trigger(N=%$N$%)";
		return true;
	}
};
class we_Timer : public WeaponEvent{
private:
	DWORD Serial;
public:
	int StartTime;
	int Delay;
	
	SAVE(we_Timer){
		REG_MEMBER(_int,StartTime);
		REG_MEMBER(_int,Delay);
		REG_PARENT(WeaponEvent);
	}ENDSAVE;

	we_Timer();
	virtual bool Check(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		if(StartTime){
			dest="ImpulseTimer(Delay=%$Delay$%, FirstImpulseTime=%$StartTime$%)";
		}else{
			dest="ImpulseTimer(Delay=%$Delay$%)";
		}
		return true;
	}
};
class we_Random : public WeaponEvent{
public:
	int Percentage;
	
	SAVE(we_Random){
		REG_MEMBER(_int,Percentage);
		REG_PARENT(WeaponEvent);
	}ENDSAVE;

	we_Random();
	virtual bool Check(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="Random(Percentage=%$Percentage$%)";
		return true;
	}
};
//==================================================================================================================//
class IsTargetDie : public WeaponEvent
{
public:
	SAVE(IsTargetDie); 
		REG_PARENT(WeaponEvent);
	ENDSAVE;

	virtual bool Check(WeaponParams* WP);
};
//==================================================================================================================//
class IsTargetInvisible : public WeaponEvent
{
public:
	IsTargetInvisible();
	bool Not;
	SAVE(IsTargetInvisible); 
		REG_PARENT(WeaponEvent);
		REG_MEMBER(_bool,Not);
	ENDSAVE;

	virtual bool Check(WeaponParams* WP);
};
//==================================================================================================================//
class Conditions : public WeaponEvent
{
public:
	Conditions();
	int LifeTimeMore;
	int LifeTimeLess;
	int TraveledDistanceMore;
	int TraveledDistanceLess;
	int RemainderDistanceMore;
	int RemainderDistanceLess;
	int HeightMore;
	int HeightLess;
	bool IsInBuilding;
	int AbsHeightMore;
	int AbsHeightLess;
	SAVE(Conditions);
		REG_PARENT(WeaponEvent);
		REG_MEMBER(_int,LifeTimeMore);
		REG_MEMBER(_int,LifeTimeLess);
		REG_MEMBER(_int,TraveledDistanceMore);
		REG_MEMBER(_int,TraveledDistanceLess);
		REG_MEMBER(_int,RemainderDistanceMore);
		REG_MEMBER(_int,RemainderDistanceLess);
		REG_MEMBER(_int,HeightMore);
		REG_MEMBER(_int,HeightLess);
		REG_MEMBER(_bool,IsInBuilding);
		REG_MEMBER(_int,AbsHeightMore);
		REG_MEMBER(_int,AbsHeightLess);
	ENDSAVE;

	virtual bool Check(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="Conditions(LifeTimeMore=%$LifeTimeMore$%)";
		return true;
	}
};
//==================================================================================================================//
class TargetReached: public WeaponEvent {
public:
	TargetReached(){
		EarthOrWaterReached=true;
		TargetObjectReached=true;
		TargetPointReached=true;
		TargetPointDistance=30;
		IsInsideBuilding=true;
	}

	bool TargetObjectReached;
	bool TargetPointReached;
	bool OneEnemyReached;
	int  TargetPointDistance;

	bool EarthOrWaterReached;
	bool IsInsideBuilding;

	SAVE(TargetReached);

		REG_PARENT(WeaponEvent);

		REG_MEMBER(_bool,EarthOrWaterReached);
		
		REG_MEMBER(_bool,TargetObjectReached);

		REG_MEMBER(_bool,TargetPointReached);
		REG_MEMBER(_bool,OneEnemyReached);
		SAVE_SECTION(2);
		REG_MEMBER(_int,TargetPointDistance);
        SAVE_SECTION(1);
		REG_MEMBER(_bool,IsInsideBuilding);

	ENDSAVE;

	DWORD GetClassMask(){
		return (TargetPointReached||OneEnemyReached)?3:1;
	}
	virtual bool Check(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="TargetReached(EarthOrWaterReached=%$EarthOrWaterReached$%,TargetPointReached=%$TargetPointReached$%)";
		return true;
	}
};
class TimePeriod : public WeaponEvent{
public:
	UserFriendlyNumericalReturner StartTime;
	UserFriendlyNumericalReturner FinalTime;

	SAVE(TimePeriod);
		REG_PARENT(WeaponEvent);
		REG_AUTO(StartTime);
		REG_AUTO(FinalTime);
	ENDSAVE;

	TimePeriod();
	virtual bool Check(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="TimePeriod(%$StartTime$%..%$FinalTime$%)";
		//dest="";
		//dest.print("TimePeriod(%$StartTime$%,Duration=%d)",FinalTime-StartTime);
		return true;
	}
};
//==================================================================================================================//
class OR_several_events : public WeaponEvent
{
public:
	ClassArray<WeaponEvent> OR_events;
	
	SAVE(OR_several_events);
		REG_PARENT(WeaponEvent);
		REG_AUTO(OR_events);
	ENDSAVE;
	virtual bool Check(WeaponParams* WP){
		for(int i=0;i<OR_events.GetAmount();i++){
			if(OR_events[i]->Check(WP))return true;
		}
		return false;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="%$OR_events, or $%";
		return true;
	}
};
//==================================================================================================================//
class AND_several_events : public WeaponEvent
{
public:
	ClassArray<WeaponEvent> AND_events;

	SAVE(AND_several_events);
		REG_PARENT(WeaponEvent);
		REG_AUTO(AND_events);
	ENDSAVE;
	virtual bool Check(WeaponParams* WP){
		for(int i=0;i<AND_events.GetAmount();i++){
			if(!(AND_events[i]->Check(WP)))return false;
		}
		return true;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="(%$AND_events, and $%)";
		return true;
	}
};
class NOT_event: public WeaponEvent{
public:
	ClassPtr<WeaponEvent> Not;
	SAVE(NOT_event);
		REG_PARENT(WeaponEvent);
		REG_AUTO(Not);
	ENDSAVE;
	virtual bool Check(WeaponParams* WP){
		if(Not.Get()){
            return Not.Get()->Check(WP);
		}else return true;
	}
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="(not %$Not$%)";
		return true;
	}
};
class WaterIsThere : public WeaponEvent{
public:
    SAVE(WaterIsThere);
		REG_PARENT(WeaponEvent);
	ENDSAVE;
	virtual bool Check(WeaponParams* WP);
};
class TerritoryLockedForUnits : public WeaponEvent{
public:
    int RadiusForCheck;
	SAVE(TerritoryLockedForUnits);
		REG_PARENT(WeaponEvent);
		REG_MEMBER(_int,RadiusForCheck);
	ENDSAVE;
	virtual bool Check(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="LockedForUnits(R=%$RadiusForCheck$%)";
		return true;
	}
};
class TerritoryLockedForShips : public WeaponEvent{
public:
	int RadiusForCheck;
	SAVE(TerritoryLockedForShips);
		REG_PARENT(WeaponEvent);
		REG_MEMBER(_int,RadiusForCheck);
	ENDSAVE;
	virtual bool Check(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="LockedForShips(R=%$RadiusForCheck$%)";
		return true;
	}
};
class RandomEvent : public WeaponEvent{
public:
	float Probability;
	SAVE(RandomEvent);
		REG_PARENT(WeaponEvent);
		REG_MEMBER(_float01,Probability);
	ENDSAVE;
	virtual bool Check(WeaponParams* WP);
	virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
		dest="Random(P=%$Probability,%.02f$%";
		return true;
	}
};
//==================================================================================================================//
class IsFirstStep : public WeaponEvent
{
public:
	IsFirstStep();
	bool Not;
	SAVE(IsFirstStep); 
		REG_PARENT(WeaponEvent);
		REG_MEMBER(_bool,Not);
	ENDSAVE;

	virtual bool Check(WeaponParams* WP);
};
//==================================================================================================================//

//==================================================================================================================//
extern WeaponSystem GameWeaponSystem;
#pragma pack ( pop )


#endif