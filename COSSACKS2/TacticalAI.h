#ifndef __TACTICALAI_H__
#define __TACTICALAI_H__
//------------------------------------------------------------------------------------------------------------------//
#include "stdheader.h"
//#include <xmlQuote.h>
//#include "..\ClassEngine\ClassEngine.h"
//#include <typeinfo.h>
//#include "..\ClassEngine\more_types.h"
#include "ZonesAndGroupsExtraInfo.h"
#include "BrigadeAI.h"
//------------------------------------------------------------------------------------------------------------------//
#define CHECKTIMEDELAY 120 
//------------------------------------------------------------------------------------------------------------------//
class OneMPProperty : public BaseClass
{
public:
	OneMPProperty();
	~OneMPProperty();
	_str Name;
	int LifeTimeEnd;

	// ... //

	SAVE (OneMPProperty)
		REG_AUTO(Name);
		REG_MEMBER(_int,LifeTimeEnd);
	ENDSAVE
};
//------------------------------------------------------------------------------------------------------------------//
class MPProperty : public BaseClass
{
public:
	MPProperty();
	~MPProperty();
	ClassArray<OneMPProperty> PropertyList;
	SAVE (MPProperty)
		REG_AUTO(PropertyList);
	ENDSAVE
};
//------------------------------------------------------------------------------------------------------------------//
// ������������ ���������� � �����
class NationMainPointInfo : public BaseClass
{
public:
	NationMainPointInfo();
	~NationMainPointInfo();
	int NI;
	int TWeight;
	int HWeight;
	int Tw;
	int Hw;
	int EnemyForce;
	int EnemyForcePr;
	int FriendlyForce;
	int FriendlyForcePr;
	int State;
	int Type;
	int CheckTime;
	word RegisteredPolk;
	MPProperty Property;
	void ShowInfo(int x, int y);

	SAVE (NationMainPointInfo)
		REG_MEMBER(_int,NI);
		//REG_MEMBER(_int,Weight);
		REG_MEMBER(_int,EnemyForce);
		REG_MEMBER(_int,FriendlyForce);
		REG_MEMBER(_int,State);
		REG_MEMBER(_int,Type);
		REG_MEMBER(_int,CheckTime);
		REG_AUTO(Property);
	ENDSAVE
};
//------------------------------------------------------------------------------------------------------------------//
// ����� ... � ��� ��������� �� �����
class UnitImpact : public BaseClass
{
public:
	UnitImpact();
	UnitImpact(Atom* A);
	~UnitImpact();
	int AtomIndex;
	int AtomSerial;
	int TopDist;
	int LastCheckTime;
	SAVE (UnitImpact)
		REG_MEMBER(_int,AtomIndex);
		REG_MEMBER(_int,AtomSerial);
		REG_MEMBER(_int,TopDist);
		REG_MEMBER(_int,LastCheckTime);
	ENDSAVE
};
//------------------------------------------------------------------------------------------------------------------//
class MainPoint : public BaseClass 
{
public:
	MainPoint();
	~MainPoint();
	int x,y;
	int Radius;
	int MPZoneIndex;
	int Amount;
	int Type;
	word ZoneIndex;
	word SeloIndex;
	word BildIndex;
	//LinearArray<word,_WORD> RoadTo;
	//LinearArray<word,_WORD> RoadTopZon;
	//LinearArray<word,_WORD> WayPriceTo;
	ClassArray<UnitImpact> UnitsImpact;// ������ ����������� ����� ���� �����
	//ClassArray<NationMainPointInfo> NationMPInfo; 
	
	SAVE (MainPoint)
		REG_MEMBER(_int,x);
		REG_MEMBER(_int,y);
		REG_MEMBER(_int,Radius);
		REG_AUTO(UnitsImpact);
		//REG_AUTO(RoadTopZon);
	ENDSAVE

	//NationMainPointInfo* GetMPInfo(int NI);
	//void SetMPInfo(NationMainPointInfo* PI);
	//int GetEnemyForcePr(int NI);
	//int GetFriendlyForcePr(int NI);
	int GetTopDistToAtom(word AtomIndex);
	int RegisterAtom(Atom* A);
	int GetAmount();
	int GetOwner();
};
//------------------------------------------------------------------------------------------------------------------//
struct Polk2Point
{
	int W;
	word AFIndex;
	word PointIndex;
	word PolkIndex;
};
//------------------------------------------------------------------------------------------------------------------//
// ����� ����������� ������� �� ���� ����
class NationalAI : public BaseClass
{
public:
	NationalAI();
	~NationalAI();
	int NI;
	bool Active;
	bool FirstStart;
	//LinearArray<word,_WORD> BrigList;
	LinearArray<word,_WORD> PolkList;//������ �������� ������(BrigadeAI::Reg) �������� ���������
	int SumEnmForce;
	int SumFreForce;
	
	LinearArray<word,_WORD> MainPointsOrder;
	LinearArray<word,_WORD> MainForceWeight;
	LinearArray<word,_WORD> MainPSetForce;

	ClassArray<NationMainPointInfo> NationMPInfo; //���������� ����� �� ������ �������� �����
	NationMainPointInfo* GetMPInfo(int MPIndex);
	void SetMPInfo(int MPIndex,NationMainPointInfo* PI);

	//-- ��� ���� ������
	void ClearPolkRegistration();//������
	void FillMainPointsOrder();//���������� ���� ������ � ��������� �� ��������
	void SetPolkRegistration();//���������� ���� ����� � �������� ������ �����
	DynArray<Polk2Point*> TempSet;
	void SetBrigade2Polk();// ������������ ������ �� ������
	//-- ���.
	static int comPolk2Point (const void *p1,const void *p2);

	static word* wh;
	static int compare (const void *p1,const void *p2);

	byte MaxPolk;
	//byte MaxActivPolk;
	void Process();
	
	void NewAutoForm(AutoForm* AF);

	void ShowInfo(int YShift);
	void SaveSB(SaveBuf *SB);
	void LoadSB(SaveBuf *SB);

	SAVE (NationalAI)
		//REG_AUTO(BrigList);
		REG_AUTO(PolkList);
	ENDSAVE
};
//------------------------------------------------------------------------------------------------------------------//
// ����������� �� 
// ������������ ��������� ����� ������ T_AI.
// ������ �������� ������� �������� ����� �� �����:
//  - ����������� �����, �������, ������ � ������ ������������
//  - +���� ����������� �� ����� �� �������� AiImportantPoint 
class TacticalAI : public BaseClass
{
public:
	TacticalAI(void);
	~TacticalAI(void);
	bool FirstStart;
	bool StartAI;
	bool IsResearchMap;
	static ClassArray<MainPoint> MainPoints;//�������� ����� 
	static ClassArray<NationalAI> ActiveAI;// ���������� ��������� �����
										 // ��������� �������� ��������� ����� ������� �� ��� ����� �����.
	LinearArray<word,_WORD> DistBetweenMP;
	word MaxPointDistBetweenMP;
	
	SAVE (TacticalAI)
		REG_AUTO(MainPoints);
		REG_AUTO(ActiveAI);
	ENDSAVE

	int ProcessTime;
	int NationalProcessIndex;
	//int MPIndex;

	void Process();
	void Activate(byte NI);// ��������� �� ��� ����� ����� NI
	void Stop(byte NI);
	void ResearchMap();//

	void FillDest();
	void ClearAll();//
	void SetMPWeight();//
	void SetMPWeight(word NI);// �������� �������� � ��������� � ������ ����� ��� �����
	void NewBrigadeIsBorn(Brigade *B);//
	int GetDistBetweenMP(word i, word j);//
	void ShowInfo();//
	static int BirthCallback(int NewbornIndex);
	void SaveSB(SaveBuf *SB);
	void LoadSB(SaveBuf *SB);

	static bool GetNewTowerCoord(byte NI, int &x, int &y); 
};
//------------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------//
#endif