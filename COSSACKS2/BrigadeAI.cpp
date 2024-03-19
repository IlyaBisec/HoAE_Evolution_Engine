#include "stdheader.h"
#include "unitability.h"
#include "BrigadeAI.h"
#include "TacticalAI.h"
#include "CurrentMapOptions.h"
#include "BrigadeAINeuro.h"

VOID KeyTestMem();

#ifdef COSSACKS2

extern int tmtmt;
extern int AnimTime;
extern City CITY[8];
extern int MaxMorale_LostCriticalPercent;
extern int Morale_KillDec;
extern int Morale_DecWhen30Lost;
extern int Morale_DamageDec1;
extern int Morale_FormationShield15;
extern int Morale_FormationShield196;
extern RLCFont YellowFont;
extern int GetHeight(int x, int y);
extern void xLine(int x,int y,int x1,int y1,byte c);
DLLEXPORT int GetTopZone(int x,int y);
DLLEXPORT bool GetTopZoneCoor(int Zone,int* x,int* y);
DLLEXPORT int GetNextZone(int Start,int Fin);
extern void ComThrowGrenade(byte NI,int ID);
extern byte GetTreeShotPass(int x, int y);
extern void CancelStandGroundAnyway(Brigade *B);
extern void MakeStandGroundTemp(Brigade *B);
extern int GetFriendUnitsAmount(byte NI,int xc,int yc,int R);
extern int GetEnemyUnitsAmount(byte NI,int xc,int yc,int R);
typedef void AnglActor(OneObject* OB,WeaponInSector* WS,byte Angle,int param);
extern void ActInSector(int x,int y,WeaponInSector* WS,byte Angle,AnglActor* ANGA,int param);
CEXPORT tpBirthCallback* SetBirthCallback(tpBirthCallback* dc);
extern void FillFormation(Brigade* BR);
DLLEXPORT int GetListOfNearZones(int Zone,OneLinkInfo** ZonesAndDist);
extern void RunPF(int i,const char* Desc);
extern void StopPF(int i);
extern void ClearCallbacks();
extern int GetTopDistance(int xa,int ya,int xb,int yb,int LT);
extern word GetBrigLockType(Brigade* BR);
typedef bool tpUnitsCallback(OneObject* OB,void* param);
int PerformActionOverUnitsInRadius(int xc,int yc,int R,tpUnitsCallback* CB,void* Param);
int PerformActionOverBuildingsInRadius(int xc,int yc,int R,tpUnitsCallback* CB,void* Param);
DLLEXPORT void CopyUnits(GAMEOBJ* Src,GAMEOBJ* Dest,int start,int n,bool add,bool remove);
DLLEXPORT void DeleteDynGroup(GAMEOBJ* Units);
DLLEXPORT bool GetGrpCenter(GAMEOBJ* Grp,int* x,int* y);
DLLEXPORT int GetTotalAmount0(GAMEOBJ* Units);
DLLEXPORT void RegisterDynGroup(GAMEOBJ* Units);
DLLEXPORT bool FillFormationFromGroup(byte BrigNI, word BrigID, GAMEOBJ* Source, GAMEOBJ* Dest=NULL);
DLLEXPORT void SGP_MoveToPoint(byte NI,GAMEOBJ* Grp,int x,int y,int Direction,int DF,int DR,DWORD Flags);
DLLEXPORT int RemoveNUnits(GAMEOBJ* Dst,GAMEOBJ* Src,int N,int NIndex);
DLLEXPORT void ClearGroup(GAMEOBJ* Grp);
extern int Norma(int,int);
void ComShotLine(byte NI,byte State,int Line,int BID);
DLLEXPORT bool RegisterUnitType(GAMEOBJ* GOBJ,char* Name);
extern void ErrBox(char* sz,...);
DLLEXPORT int GetResource(byte Nat,byte ID);
int FindCoordForCannon(OneObject* Cannon,int &xx,int &yy, byte Dir);
void RotateMon(OneObject* OB,char angle);
bool SetDestDirForComplexObject(OneObject* OB,byte DestDir);   
DLLEXPORT int GetDiff(byte NI);
DLLEXPORT int GetMyNation();
extern int FORMDIST;
extern EngineSettings EngSettings;
void AssignHint1(char* s,int time);
int GetDistanceToBuilding(OneObject* BLD,int x,int y);
void BuildObjLink(OneObject* OBJ);
//====================================================================//
CNeuroStorage NeuroStorage;
//
void ShotWay(word Size,DWORD* Matrix,word* Result)
{
	if(Size==1){
		Result[0]=0;
		return;
	}
	byte* A=(byte*)malloc(Size);
	memset(A,0,Size);
	byte* B=(byte*)malloc(Size);	
	memset(B,0,Size);
	//word* AminB=(word*)malloc(Size<<1);
	//memset(AminB,0,Size<<1);
	for(int v=0;v<Size;v++)
	{
		word i,j;
		word mI=0;
		word mJ=0;
		word max=0;
		word maxA=0;
		word maxB=0;
		for(i=0;i<Size;i++)
		{
			if(!B[i])
			{
				DWORD min=0x7FFFFFFF;
				for(j=0;j<Size;j++)
				{
					if(!A[j])
					{
						if(Matrix[j*Size+i]<=min)
						{
							min=Matrix[j*Size+i];
							//AminB[i]=min;
							//Result[j]=i;
							mI=j;
							mJ=i;
						}
					}
				}
				if((min>=max)||(min==max&&i==j))
				{
					max=min;
					maxA=mI;
					maxB=mJ;//Result[maxA];
				}
			}
		}
		for(i=0;i<Size;i++)
		{
			if(!A[i])
			{
				DWORD min=0x7FFFFFFF;
				for(j=0;j<Size;j++)
				{
					if(!B[j])
					{
						if(Matrix[i*Size+j]<=min)
						{
							min=Matrix[i*Size+j];
							//AminB[i]=min;
							//Result[i]=j;
							mI=i;
							mJ=j;
						}
					}
				}
				if((min>=max)||(min==max&&i==j))
				{
					max=min;
					maxA=mI;
					maxB=mJ;//Result[maxA];
				}
			}
		}
		A[maxA]=1;
		B[maxB]=1;
		assert(maxA<Size);
		assert(maxB<Size);
		Result[maxA]=maxB;
	}
	free(A);
	free(B);
	//free(AminB);
}
void FillDistMatrix(word Amo,int* FromXY,int* ToXY,DWORD* DistMatrix)
{
	for(int i=0;i<Amo;i++)
	{
		for(int j=0;j<Amo;j++)
		{
			DistMatrix[i*Amo+j]=Norma(FromXY[i*2]-ToXY[j*2],FromXY[i*2+1]-ToXY[j*2+1]);
		}
	}
}
void FindShotWayPoint(word Amo,int* FromXY,int* ToXY,word* Result)
{
	DWORD* DM=(DWORD*)malloc((Amo*Amo)<<2);
	FillDistMatrix(Amo, FromXY, ToXY, DM);
	ShotWay(Amo, DM, Result);
	free(DM);
}
//
//====================================================================//
Atom::Atom()
{
	BSelf=NULL;
	CheckTimeInterval=3;
	Force=100;
	Type=0;
	CheckTime=-1;
	C=NULL;
	Tower=NULL;
	AtomIndex=-1;
	NearestEnmBrigade=0xFFFF;
	DistToNearestEnmBrigade=999999;
	CheckTimeNearestEnmBrigade=-1;
	LastAlarmTime=-1;
}
Atom::Atom(Brigade* Br)
{
	BSelf=Br;
	AtomIndex=-1;
	CheckTimeInterval=3;
	Force=100;
	Type=1;//strelki
	CheckTime=-1;
	UpdateParams();
	IsStand=true;
	NI=Br->CT->NI;
	C=NULL;
	Tower=NULL;
	if(Br)
	{
		OneObject* OB = Group[Br->Memb[4]];
		if(OB)
			if(OB->MaxLife>150)
				Type=2;//Horce
	}
	LockType=GetBrigLockType(Br);
	StandTime=0;
	NearestEnmBrigade=0xFFFF;
	DistToNearestEnmBrigade=999999;
	CheckTimeNearestEnmBrigade=-1;
	LastAlarmTime=-1;
}
Atom::Atom(OneObject* Ob)
{
	BSelf=NULL;
	C=NULL;
	Tower=NULL;
	CheckTimeInterval=3;
	Type=0;
	CheckTime=-1;
	IsStand=true;
	if(Ob&&Ob->newMons->Usage==PushkaID||Ob->newMons->Usage==LinkorID)
	{
		CannonID=Ob->Index;
		C=Ob;
		Force=150;
		NI=Ob->NNUM;
		Type=3;
		UpdateParams();
	}
	else
	{
		if(Ob&&Ob->newMons->Usage==TowerID)
		{
			Tower_ID=Ob->Index;
			Tower=Ob;
			Force=500;
			Speed=0;
			Type=4;
			UpdateParams();
		}
	}
	LockType=Ob->LockType;
	StandTime=0;
	NearestEnmBrigade=0xFFFF;
	DistToNearestEnmBrigade=999999;
	CheckTimeNearestEnmBrigade=-1;
	LastAlarmTime=-1;
}


Atom::~Atom()
{
	Type=0;
	//BrigadeAI::ClearMainTaget(this);
}
void Atom::UpdateParams()
{
	if(CheckTime!=tmtmt&&tmtmt>(CheckTime+CheckTimeInterval))
	{
		int NnStand=0;
		if(BSelf&&BSelf->WarType!=0)
		{
			BSelf->GetCenter(&x,&y,&Dir);
			UnitsAmo=0;
			FireReady=0;
			FireTimeDelayLeft=0;
			SumLife=0;
			int maxdelay=0;
			Tired=0;
			IsStand=true;
			OneObject* OBt=NULL;
			for(int i=3;i<BSelf->NMemb;i++)
			{
				if(BSelf->Memb[i]!=0xFFFF)
				{
					OneObject* OB=Group[BSelf->Memb[i]];
					if(OB)
					{
						UnitsAmo++;
						SumLife+=OB->Life;
						if(OB->Usage==StrelokID||OB->Usage==MentID||OB->Usage==EgerID)
						{
							if(OB->delay==0)
								FireReady++;
							else
							{
								FireTimeDelayLeft+=OB->delay;
								maxdelay+=OB->MaxDelay;
							}
						}
						Tired+=OB->GetTired;
						if(OB->DestX<0)
							NnStand++;
							//IsStand=false;
						if(!OBt)OBt=OB;
					}
				}
			}
			if(FireTimeDelayLeft&&maxdelay)
				FireTimeDelayLeft=(FireTimeDelayLeft*100)/maxdelay;
			else FireTimeDelayLeft=-1;
			int com=0;
			if(BSelf->Memb[0]!=0xFFFF)
			{
				OneObject* OB=Group[BSelf->Memb[0]];
				if(OB){Tired+=OB->GetTired;com++;}
			}
			if(BSelf->Memb[1]!=0xFFFF)
			{
				OneObject* OB=Group[BSelf->Memb[1]];
				if(OB){Tired+=OB->GetTired;com++;}
			}
			if(BSelf->Memb[2]!=0xFFFF)
			{
				OneObject* OB=Group[BSelf->Memb[2]];
				if(OB){Tired+=OB->GetTired;com++;}
			}
			if(UnitsAmo)
				Tired=Tired/(UnitsAmo+com);
			else
				Type=0;
			Force=UnitsAmo;
			if(OBt)
			{
				Speed=OBt->newMons->MotionDist;
				int MeleeF=(100*(OBt->MoreCharacter->MaxDamage[0]-4))/10;
				int RefleF=(100*OBt->MoreCharacter->MaxDamage[1])/100;
				Force=(SumLife*(MeleeF+RefleF))/(10000);
			}
		}
		else
		if(C&&Type==3)
		{
			UnitsAmo=1;
			if(C->delay==0)
				FireReady=1;
			C=Group[CannonID];
			IsStand=true;
			if(C&&Type==3&&!C->Sdoxlo)
			{
				x=C->RealX>>4;
				y=C->RealY>>4;
				Dir=C->RealDir;
				FireTimeDelayLeft=((C->newMons->AttackPause[4]-C->delay)*100)/(C->newMons->AttackPause[4]+1);
				if(C->DestX>0)
					IsStand=false;
				else
					NnStand=1;
			}
			else
			{
				C=NULL;
				Type=0;
			}
		}
		else
		if(Tower&&Type==4)
		{
			UnitsAmo=1;
			x=Tower->RealX>>4;
			y=Tower->RealY>>4;
		}
		else
			Type=0;
		if((100*(NnStand+1))/(UnitsAmo+1)>80)
			IsStand=true;
		else
			IsStand=false;
		if(IsStand&&StandTime==0)
			StandTime=AnimTime/(256*25);
		if(!IsStand)
			StandTime=0;
		CheckTime=tmtmt;
	}
}
int Atom::GetSizeFront()
{
	int rez=100;
	if(BSelf&&BSelf->WarType>0)
		rez=ElementaryOrders[BSelf->WarType-1].Hight/2+120;
	return rez;
}
int Atom::GetSizeBack()
{
	return GetSizeFront();
}
int Atom::GetSizeLeft()
{
	int rez=0;
	if(BSelf&&BSelf->WarType>0)
		rez=ElementaryOrders[BSelf->WarType-1].Width/2+50;
	return rez;
}
int Atom::GetSizeRight()
{
	return GetSizeLeft();
}
byte Atom::GetNMask()
{
	NMask=0;
	if(BSelf)
		NMask=BSelf->GetNMask();
	else
	if(C)
		NMask=C->NMask;
	else
	if(Tower)
		NMask=Tower->NMask;
	return NMask;
}
bool Atom::IsAlive()
{
	bool rez=false;
	if(BSelf&&BSelf->WarType>0&&BSelf->Enabled)
		rez=true;
	else
	if(C&&(!C->Sdoxlo))
		rez=true;
	else
	if(Tower&&(!Tower->Sdoxlo))
		rez=true;
	return rez;
}
word Atom::GetLockType()
{
	word rez=0;
	if(BSelf)
		rez=GetBrigLockType(BSelf);
	else
	if(C)
		rez=C->LockType;
	return rez;
}
int Atom::GetStandTime()
{
	if(IsStand)
		return AnimTime/(256*25)-StandTime;
	return 0;
}
//====================================================================//
AreaInfo::AreaInfo()
{}
AreaInfo::AreaInfo(int I)
{
	Index=I;
	GetTopZoneCoor(I,&x,&y);
	x<<=6;
	y<<=6;
	H=GetHeight(x,y);
	ClearNWay();
}
AreaInfo::~AreaInfo()
{}
void AreaInfo::ClearNWay()
{
	for(byte i=0;i<8;i++)
	{
		NWay[i]=0;
		CWay[i]=0;
	}
}
//====================================================================//
// таблица несовместимых состояний
bool AutoForm::NonStateArr[32][32] = 
{
					   //0 1 2 3 4 5 6 7 8 9 10111213141516
/*0-Attack*/			{0,0,1,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*1-Reserv*/			{0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*2-MoveBack*/			{1,0,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*3-Stand*/				{1,0,1,0,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*4-Front*/				{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*5-March*/				{1,0,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*6-NearAim*/			{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*7-MoveFront*/			{0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*8-BattleFormation*/	{0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*9-Rest*/				{1,0,1,0,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*10-Move*/				{1,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*11-ReadyToFire*/		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*12-LineFormation*/	{0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*13-SquareFormation*/	{0,0,0,0,0,1,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*14-KareFormation*/	{0,0,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*15-EnableFire*/		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*16-DisableFire*/		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};
AutoForm::AutoForm()
{
	BSelf=NULL;
	MainTarget=NULL;
	Type=0;
	for(int i=0;i<16;i++)
	{
		EnmAng[i].SumDang=0;
		EnmAng[i].Dang=0;
		EnmAng[i].Dir=i<<4;
		EnmAng[i].SectorsAmo=0;
		
		FriendAng[i].SumDang=0;
		FriendAng[i].Dang=0;
		FriendAng[i].Dir=i<<4;
		FriendAng[i].SectorsAmo=0;
	}
	KeyPoints=NULL;
	NKeyPoints=0;
	StandTime=0;
	LastCommTime=AnimTime;
	NeroTypeIndex=-1;
	NeroNameIndex=-1;
	
}
AutoForm::AutoForm(Brigade* Bn)
{
	RegimentIndex=-1;
	NextProcesedTime=0;
	NextTimeProcessRules=0;
	Force=70;

	//Atom::Atom(Bn);
	BSelf=Bn;
	UpdateParams();

	NI=Bn->CT->NI;

	AimX=-1;
	AimY=-1;
	
	Type=1;
	if(Bn->WarType)
	{
		FormationTypeIndex=ElementaryOrders[Bn->WarType].GroupID;
		OneObject* OB = Group[Bn->Memb[4]];
		if(OB)
			if(OB->MaxLife>150)
				Type=2;
	}
	
	for(int i=0;i<16;i++)
	{
		EnmAng[i].SumDang=0;
		EnmAng[i].Dang=0;
		EnmAng[i].Dir=i<<4;
		EnmAng[i].SectorsAmo=0;
		
		FriendAng[i].SumDang=0;
		FriendAng[i].Dang=0;
		FriendAng[i].Dir=i<<4;
		FriendAng[i].SectorsAmo=0;
	}
	SetState(16,false);DisableFire();
	DistToMainTarget=-1;
	//SetNoSearchVictim(true);
	Movement=false;
	
	MainTarget=NULL;
	LockType=GetBrigLockType(Bn);
	KeyPoints=NULL;
	NKeyPoints=0;
	StandTime=0;

	mtX=x;
	mtY=y;
	mtDir=Dir;
	mtXr=x-((TSin[mtDir]*GetSizeRight())>>8);
	mtYr=y+((TCos[mtDir]*GetSizeRight())>>8);
	mtXl=x+((TSin[mtDir]*GetSizeLeft())>>8);
	mtYl=y-((TCos[mtDir]*GetSizeLeft())>>8);
	RegisterDynGroup(&RemanUnits);
	LastCom=-1;
	PolkSynchronCommand=8;
	LastCommTime=AnimTime;
	NeroTypeIndex=-1;
	NeroNameIndex=-1;

}
AutoForm::~AutoForm()
{
	Type=0;
	BSelf=NULL;
	ClearKeyPoints();
	DeleteDynGroup(&RemanUnits);
	//BrigadeAI::ClearMainTaget((Atom*) this);
}
void AutoForm::MoveTo(int xx, int yy)
{
	//DisableFire();
	byte dd=(byte)(GetDir(xx-x,yy-y));
	MoveTo(xx,yy,dd);
}
void AutoForm::MoveTo(int xx, int yy, byte dir)
{
	/*
	bool cor=CorrectDestPoint(x,y,dir);
	for(int i=0;i<30&&!cor;i++)
		cor=CorrectDestPoint(mtX,mtY,dir);
		*/
	//
	Atom* tmp=GetNearestAtomIn(xx,yy);
	if(tmp)
	{
		//dir=GetAvrEndDirIn(xx,yy);
		dir=GetDir(tmp->x-xx,tmp->y-yy);
	}
	//
	byte dd=(dir+8)>>4;dd=dd<<4;
	mtDir=dd;
	mtX=xx;
	mtY=yy;
	mtXr=xx-((TSin[mtDir]*GetSizeRight())>>8);
	mtYr=yy+((TCos[mtDir]*GetSizeRight())>>8);
	mtXl=xx+((TSin[mtDir]*GetSizeLeft())>>8);
	mtYl=yy-((TCos[mtDir]*GetSizeLeft())>>8);

	FillKeyPoints();
	int p1, p2;
	if(FindNearestKeyPoints( p1, p2))
	{
		FindMoveToCoord( mtX, mtY, mtDir, p1, p2);
	}
	SimpleMoveTo(mtX,mtY,mtDir);
}
void AutoForm::SimpleMoveTo(int xx, int yy)
{
	byte dd=(byte)(GetDir(xx-x,yy-y));
	SimpleMoveTo(xx,yy,dd);
}
void AutoForm::SimpleMoveTo(int xx, int yy, byte dir)
{
	DisableFire();
	//ClearOrders();
	Movement=true;
	mtX=xx;
	mtY=yy;
	byte dd=(dir+8)>>4;dd=dd<<4;
	//CancelStandGroundAnyway(BSelf);
	byte prio=128;
	if(BSelf)
	{
		if(BSelf->AttEnm==0)
		{
			prio=128+16;
		}
	}

	
	//MakeStandGroundTemp(BSelf);
	mtDir=dd;
	mtXr=mtX-((TSin[mtDir]*GetSizeRight())>>8);
	mtYr=mtY+((TCos[mtDir]*GetSizeRight())>>8);
	mtXl=mtX+((TSin[mtDir]*GetSizeLeft())>>8);
	mtYl=mtY-((TCos[mtDir]*GetSizeLeft())>>8);

	
	//SetStandGround(false);
	int dss=Norma(mtX-x,mtY-y);
	addrand(40);
	//if(dss<1000)
	//	BSelf->HumanLocalSendTo(mtX<<4,mtY<<4,dd,prio,0);
	//else
	if(BSelf)
	{	
		BSelf->HumanGlobalSendTo(mtX,mtY,dd,prio,0);
		
		if(EngSettings.GameName>0)// all except Rome
		{
			bool KARE=0;
			if(BSelf->WarType){
				KARE=ElementaryOrders[BSelf->WarType-1].OrdUsage==2;
			};
			if(!KARE)CancelStandGroundAnyway(BSelf);	
			if((!BSelf->AttEnm)&&prio&127){
				void MakeStandGroundTemp(Brigade* BR);
				BSelf->LastOrderTime=0;
				MakeStandGroundTemp(BSelf);
			}
		}	
	}
}
void AutoForm::Process()
{
	if(C&&NI!=C->NNUM)//cannon captured
		return;
	if(AnimTime>NextProcesedTime&&IsAlive())
	{
		if(IsStand)
			Movement=false;
		TimeInterval=3;
		//if(GetState(0))TimeInterval=1;
		Atom* tm=MainTarget;
		MainTarget=GetNearestAtom();
		/*
		if(tm&&MainTarget!=tm&&GetState(0))//Attack finish
		{
			if(MainTarget)
				MoveTo(x,y,GetDir(MainTarget->x-x,MainTarget->y-y));
			else
				MoveTo(x,y,Dir);
			SetState(0,false);
		}
		else
		if(MainTarget!=tm&&MainTarget&&IsStand)
		{
			//DisableFire();
			TurnToMT();
			SetState(0,false);
		}
		*/
		//if(MainTarget&&DistToMT()<200&&GetFireReady()>80&&!GetState(0))
			//SetState(0,true);
		//int a2=GetMainTargetAngl();
		if(GetStandTime()>5)
		{
			StateArr[0]=false;//Attack
			StateArr[5]=false;//March
		}

		if(IsInFront())
			SetState(4,true);//Front
		else
			SetState(1,true);//Reserv;

		if(EngSettings.NeuroLearn)
		{
			ProcessRules();
			NeuroStorage.AutoFormProcess(this);
		}
		else
		if(EngSettings.NeuroUse)
		{
			NeuroStorage.AutoFormProcess(this);
		}
		else
		{
			ProcessRules();
		}
		int ACT=GetMaxActions();
		ExecuteActions(ACT);
		if(ACT==-1)
		{
			if(IsStand&&GetStandTime()%11==0&&BSelf&&(!BSelf->NewBOrder))
			{
				Reman();
			}
		}
		else
		{
			LastCom=ActionsArr[ACT];
		}

		/*
		if(IsStand&&ACT==-1&&GetTired()>90)
		{
			//if((BSelf->Direction>(Dir+17)||BSelf->Direction<(Dir-17)))
			//	MoveTo(x-10,y-10,BSelf->Direction);
			
			bool mm=false;
			for(byte i=192;i!=80&&!mm;i=i+16)
			{
				byte in=i>>4;
				if(FriendAng[in].SumDang)
				{
					AutoForm* A=FriendAng[in].Sectors[0].AF;
					if(A&&A->IsStand&&FriendAng[in].Sectors[0].dest<300)
						mm=true;
				}
			}
			if(mm&&tmtmt%5)
				StepBack();
		}
		*/
		//if(!IsStand&&ACT==-1&&Movement)
		//	MoveTo(mtX, mtY); 
		int DiffPause=2-GetDiff();
		NextProcesedTime=AnimTime+TimeInterval*256+DiffPause*256*20*2;
	}
}
void AutoForm::StepForward()
{
	//UpdateParams();
	int S=400;
	byte dirr=0;
	if(BSelf)
	{
		dirr=BSelf->Direction;
	}
	else
	if(C)
	{
		dirr=C->RealDir;
	}
	int dx=(TCos[dirr]*S)>>8;
	int dy=(TSin[dirr]*S)>>8;
	MoveTo(x+dx,y+dy,dirr);
}
void AutoForm::StepBack()
{
	//UpdateParams();
	int S=400;
	byte dr=BSelf->Direction+128;
	int dx=(TCos[dr]*S)>>8;
	int dy=(TSin[dr]*S)>>8;
	MoveTo(x+dx,y+dy,BSelf->Direction);
}
void AutoForm::MoveBack()
{
	//int a=GetMaxDangDirectionMinus();
	/*
	int a=GetMainTargetAngl();
	if(a>-1)
	{
		//byte ang=(byte) (a<<4);
		SetNotReadyToFire();
		byte ang=a+BSelf->Direction+128;
		int S=500;
		int dx=(TCos[ang]*S)>>8;
		int dy=(TSin[ang]*S)>>8;
		ang+=128;
		SimpleMoveTo(x+dx,y+dy,ang);
	}
	*/
	int S=(350*Speed)/40;
	byte ang=Dir+128;
	/* 
	
	if(MainTarget)
		ang=GetDir(x-MainTarget->x,y-MainTarget->y);
	int dx=(TCos[ang]*S)>>8;
	int dy=(TSin[ang]*S)>>8;
	if(MainTarget)
		ang=GetDir(dx+x-MainTarget->x,dy+y-MainTarget->y);
		*/
	if(MainTarget)
		ang=AvrEnmDir+128;
	int dx=(TCos[ang]*S)>>8;
	int dy=(TSin[ang]*S)>>8;
	if(BSelf) 
	{
		ClearOrders();
		BSelf->AttEnm=false;
		SetState(11,false);
	}
	SimpleMoveTo(x+dx,y+dy,ang+128);

}
void AutoForm::TurnTo(byte dir)
{
	MoveTo(x,y,dir);
}
void AutoForm::TurnLeft()
{
	UpdateParams();
	BSelf->Direction+=-16;
	addrand(41);
	BSelf->HumanLocalSendTo(x<<4,y<<4,BSelf->Direction,128+1,0);
}
void AutoForm::TurnRight()
{
	UpdateParams();
	BSelf->Direction+=16;
	addrand(42);
	BSelf->HumanLocalSendTo(x<<4,y<<4,BSelf->Direction,128+1,0);
}
void AutoForm::SetNoSearchVictim(bool vle)
{
	if(BSelf->WarType)
	{
		for(int i=0;i<BSelf->NMemb;i++)
		{
			word ID=BSelf->Memb[i];
			if(ID!=0xFFFF)
			{
				OneObject* OB=Group[ID];
				if(OB&&OB->Serial==BSelf->MembSN[i])
				{
					OB->NoSearchVictim=vle;
				}
			}
		}
	}
}
void AutoForm::AttackBrigade(Brigade *Br)
{
	if(Br&&Br->WarType&&BSelf->WarType)
	{
		//EnableFire();
		for(int i=3;i<BSelf->NMemb;i++)
		{
			if(BSelf->Memb[i]!=0xFFFF)
			{
				int rnu=-1;
				int dist=999999;
				OneObject* OBs=Group[BSelf->Memb[i]];
				//OBs->newMons->
				if(Br->NMemb&&OBs&&OBs->delay==0&&!OBs->Attack)
				{
					for(int j=0;j<10;j++)
					{
						int in=-1;
						bool flag=true;	
						for(int iter=0;flag;iter++)
						{	
							in=rando()%Br->NMemb;
							if(Br->Memb[in]!=0xFFFF)
							{
								//if(Group[Br->Memb[in]]->Life>life[in])
									flag=false;
								//if(iter>10)
								//	flag=false;
							}
						}
						OneObject* OBg=Group[Br->Memb[in]];
						if(OBg)
						{
							int dd=Norma((OBs->RealX>>4)-(OBg->RealX>>4),(OBs->RealY>>4)-(OBg->RealY>>4));
							if(dd<dist)
							{
								dist=dd;
								rnu=in;
							}
						}
					}
					if(rnu>-1)
					{
						//OBs->NoSearchVictim=true;
						OBs->RifleAttack=true;
						OBs->AttackObj(Br->Memb[rnu],128+16,0,2);
						OBs->RifleAttack=false;
						//OBs->StandGround=false;
					}
					//life[rnu]+=GetDamage(BSelf->Memb[i],Br->Memb[rnu],1);
				}
			}
		}
	}
}
void AutoForm::MeleeAttackBrigade(Brigade *Br)
{
	if(Br&&Br->WarType&&BSelf->WarType)
	{
		//EnableFire();
		BSelf->AttEnm=true;
		CancelStandGroundAnyway(BSelf);
		for(int i=0;i<BSelf->NMemb;i++)
		{
			if(BSelf->Memb[i]!=0xFFFF)
			{
				int rnu=-1;
				int dist=999999;
				OneObject* OBs=Group[BSelf->Memb[i]];
				if(Br->NMemb&&OBs)
				{
					for(int j=0;j<10;j++)
					{
						int in=-1;
						bool flag=true;	
						for(int iter=0;flag&&iter<30;iter++)
						{	
							in=rando()%Br->NMemb;
							if(Br->Memb[in]!=0xFFFF)
							{
								flag=false;
							}
						}
						OneObject* OBg=Group[Br->Memb[in]];
						if(OBg)
						{
							int dd=Norma((OBs->RealX>>4)-(OBg->RealX>>4),(OBs->RealY>>4)-(OBg->RealY>>4));
							if(dd<dist)
							{
								dist=dd;
								rnu=in;
							}
						}
					}
					if(rnu>-1)
					{
						//OBs->NoSearchVictim=true;
						OBs->StandGround=false;
						OBs->RifleAttack=false;
						OBs->AttackObj(Br->Memb[rnu],128+16);
						//OBs->RifleAttack=false;
					}
				}
			}
		}
	}
}
void AutoForm::SetFireState(Brigade* Bn, bool State)
{
	if(Bn->WarType)
	{
		for(int i=0;i<Bn->NMemb;i++)
		{
			word ID=Bn->Memb[i];
			if(ID!=0xFFFF)
			{
				OneObject* OB=Group[ID];
				if(OB&&OB->Serial==Bn->MembSN[i])
				{
					OB->RifleAttack=State;
					if(MainTarget&&MainTarget->BSelf&&DistToMT()<800)
					{
						OB->SearchOnlyThisBrigadeToKill=MainTarget->BSelf->ID;
						Bn->OnlyThisBrigadeToKill=MainTarget->BSelf->ID;
						Bn->NIOnlyThisBrigadeToKill=MainTarget->BSelf->CT->NI;
					}
					else
					{
						OB->SearchOnlyThisBrigadeToKill=0xFFFF;
						Bn->OnlyThisBrigadeToKill=0xFFFF;
					}
				}
			}
		}
	}
}
void AutoForm::GetCoordToMTWay(int &xt, int &yt, AutoForm* FR)
{
	if(MainTarget&&DistToMainTarget!=-1)
	{
		byte Dr = GetDir(MainTarget->x-x,MainTarget->y-y);
		int Step=500;
		int MTdst=DistToMT();
		int ndst=DistToMainTarget;
		if(DistToMainTarget>MTdst&&(DistToMainTarget-MTdst)>Step)
			ndst=Step;
		int xx=x+((ndst*TCos[Dr])>>8);
		int yy=y+((ndst*TSin[Dr])>>8);
		/*
		if(FR)
		{
			int Shift=Norma(FR->x-xx,FR->y-yy);
			int li=GetSizeLeft()+FR->GetSizeLeft();
			if(Shift<li)
			{
				byte D= GetDir(xx-FR->x,yy-FR->y);
				xx=FR->x+((li*TCos[D])>>8);
				yy=FR->y+((li*TSin[D])>>8);
			}
		}
		*/
		xt=xx;
		yt=yy;
	}
}
AutoForm* AutoForm::FindNearestFriendInMTDirection()
{
	AutoForm* rez=NULL;
	if(MainTarget)
	{
		int an = GetMainTargetAngl();
		if(an>-1)
		{
			byte ang = (byte) an;
			byte From = ang - 32;
			byte To = ang + 48;
			int dst=9999999;
			for(byte i=From;i!=To;i=i+16)
			{
				byte ii=i>>4;
				if(ii<16&&FriendAng[ii].SumDang)
					if(FriendAng[ii].Sectors[0].dest<dst)
					{
						dst=FriendAng[ii].Sectors[0].dest;
						rez=FriendAng[ii].Sectors[0].AF;
					}
			}
		}
	}
	return rez;
}
bool AutoForm::CorrectDestPoint(int xx,int yy, byte dirr)
{
	
	bool rez=true;
	mtX=xx;
	mtY=yy;
	mtDir=dirr;
	for(int an=0;an<16;an++)
		for(int i=0;i<FriendAng[an].SectorsAmo;i++)
		{
			AutoForm* FR=FriendAng[an].Sectors[i].AF;
			if(FR&&FR->IsStand&&FR->BSelf&&FR->mtX!=-1)
			{
				int dist=Norma(FR->mtX-mtX,FR->mtY-mtY);

				byte drr=GetDir(mtX-FR->mtX,mtX-FR->mtX);
				char drrc=(char)drr;
				char adr=(char)FR->BSelf->Direction;
				byte dim=abs(drrc-adr);
				if(dim>64)dim=dim-64;
				int fdist=(int)sqrt(((FR->GetSizeLeft()*TSin[dim])>>8)*((FR->GetSizeLeft()*TSin[dim])>>8)+((FR->GetSizeFront()*TCos[dim])>>8)*((FR->GetSizeFront()*TCos[dim])>>8));

				byte drr2=drr+128;
				char drrc2=(char)drr2;
				char adr2=(char)dirr;
				byte dim2=abs(drrc2-adr2);
				if(dim2>64)dim2=dim2-64;
				int fdist2=(int)sqrt(((GetSizeLeft()*TSin[dim2])>>8)*((GetSizeLeft()*TSin[dim2])>>8)+((GetSizeFront()*TCos[dim2])>>8)*((GetSizeFront()*TCos[dim2])>>8));

				int ndist=fdist2+fdist;
				if(ndist>dist)
				{
					mtX=FR->mtX+(((dist+100)*TSin[drr])>>8);
					mtY=FR->mtY+(((dist+100)*TCos[drr])>>8);	
					rez=false;
				}
			}
		}
	return rez;
}
// незабть отключить в релизе!!!
void AutoForm::ShowInfo()
{
	char st[128];
	if(!RulesList)
		return;
	if(!RulesList->m_fileName.pchar())
		return;
	if(Type==1||Type==2)
	{
		if(GetState(1))
			sprintf(st,"F:%d Reserv,AT(%d):TI(%d),LastCom:%d, %s",BSelf->ID,GetState(0),TimeInterval,LastCom,RulesList->m_fileName.pchar());
		else
		if(GetState(4))
			sprintf(st,"F:%d Front,AT(%d):TI(%d),LastCom:%d, %s",BSelf->ID,GetState(0),TimeInterval,LastCom,RulesList->m_fileName.pchar());
		else
			sprintf(st,"F:%d Gluck,TI:%d,LastCom:%d, %s",BSelf->ID,TimeInterval,LastCom,RulesList->m_fileName.pchar());
	}
	if(Type==3)
		sprintf(st,"Cannon ID(%d)",CannonID);
	
	int xx=x-(mapx<<5);
	int yy=y/2-GetHeight(x,y)-(mapy<<4);
	ShowString(xx,yy,st,&YellowFont);
	if(AimX!=-1)
	{
		xLine(x-(mapx<<5),y/2-GetHeight(x,y)-(mapy<<4),AimX-(mapx<<5),AimY/2-GetHeight(AimX,AimY)-(mapy<<4),0xEF);
		byte ddd=GetDir(AimX-x,AimY-y);
		int avx=x+((200*TCos[ddd])>>8);
		int avy=y+((200*TSin[ddd])>>8);
		char dst[8];
		sprintf(dst,"%d",Norma(AimX-x,AimY-y));
		ShowString(avx-(mapx<<5),avy/2-GetHeight(avx,avy)-(mapy<<4),dst,&YellowFont);
	}
	if(MainTarget)
	{
		int p=15;
		char st2[64];sprintf(st2,"FireReadyMT:%d ",FunctionResultCash[3][0]);ShowString(xx,yy+p*1,st2,&YellowFont);
		char st3[64];sprintf(st3,"SumDangAndForce:%d ",FunctionResultCash[12][0]);ShowString(xx,yy+p*2,st3,&YellowFont);
		char st4[64];sprintf(st4,"FireForceDifference:%d ",FunctionResultCash[13][0]);ShowString(xx,yy+p*3,st4,&YellowFont);
		char st5[64];sprintf(st5,"FireDanger:%d ",FunctionResultCash[28][0]);ShowString(xx,yy+p*4,st5,&YellowFont);
		char st6[64];sprintf(st6,"GetMTForestDensity:%d ",FunctionResultCash[22][0]);ShowString(xx,yy+p*5,st6,&YellowFont);
		char st7[64];sprintf(st7,"GetSelfForestDensity:%d ",FunctionResultCash[41][0]);ShowString(xx,yy+p*6,st7,&YellowFont);
		char st8[64];sprintf(st8,"GetPolkCommand:%d ",FunctionResultCash[39][0]);ShowString(xx,yy+p*7,st8,&YellowFont);
		char st9[64];sprintf(st9,"MoralMTAfterFire:%d ",FunctionResultCash[6][0]);ShowString(xx,yy+p*8,st9,&YellowFont);
		char st10[64];sprintf(st10,"Encircle:%d ",FunctionResultCash[14][0]);ShowString(xx,yy+p*9,st10,&YellowFont);
		char st11[64];sprintf(st11,"AngToMT:%d ",FunctionResultCash[16][0]);ShowString(xx,yy+p*10,st11,&YellowFont);
		char st12[64];sprintf(st12,"DistInFront:%d ",FunctionResultCash[17][0]);ShowString(xx,yy+p*11,st12,&YellowFont);
		char st13[64];sprintf(st13,"GetStandTime:%d ",FunctionResultCash[29][0]);ShowString(xx,yy+p*12,st13,&YellowFont);
		char st14[64];sprintf(st14,"GetMeleeForceDiff:%d ",FunctionResultCash[32][0]);ShowString(xx,yy+p*13,st14,&YellowFont);
		char st15[64];sprintf(st15,"GetLastCommTime:%d ",GetLastCommTime());ShowString(xx,yy+p*14,st15,&YellowFont);
		char st16[64];sprintf(st16,"DistInReserv:%d ",FunctionResultCash[18][0]);ShowString(xx,yy+p*15,st16,&YellowFont);
		char st17[64];sprintf(st17,"NeuroInaccuracy:%f ",NeuroTempInaccuracy);ShowString(xx,yy+p*16,st17,&YellowFont);

		xLine(x-(mapx<<5),y/2-GetHeight(x,y)-(mapy<<4),MainTarget->x-(mapx<<5),MainTarget->y/2-GetHeight(MainTarget->x,MainTarget->y)-(mapy<<4),0xFF);
		int avx=x+((500*TCos[AvrEnmDir])>>8);
		int avy=y+((500*TSin[AvrEnmDir])>>8);
		xLine(x-(mapx<<5),y/2-GetHeight(x,y)-(mapy<<4),avx-(mapx<<5),avy/2-GetHeight(avx,avy)-(mapy<<4),0xF8);
		int ds=DistToMT();
		sprintf(st,"%d",ds); 
		int xx=(x+MainTarget->x)/2-15;  
		int yy=(y+MainTarget->y)/2-15;
		ShowString(xx-(mapx<<5),yy/2-GetHeight(xx,yy)-(mapy<<4),st,&YellowFont);
		char ss[64];
		sprintf(ss,"StandTime %d",GetMTStandTime());
		ShowString(MainTarget->x-(mapx<<5),MainTarget->y/2-GetHeight(MainTarget->x,MainTarget->y)-(mapy<<4),ss,&YellowFont);
	}

	ShowString(mtXr-(mapx<<5),mtYr/2-GetHeight(mtXr,mtYr)-(mapy<<4),"R",&YellowFont);
	ShowString(mtXl-(mapx<<5),mtYl/2-GetHeight(mtXl,mtYl)-(mapy<<4),"L",&YellowFont);
	xLine(mtXr-(mapx<<5),mtYr/2-GetHeight(mtXr,mtYr)-(mapy<<4),mtXl-(mapx<<5),mtYl/2-GetHeight(mtXl,mtYl)-(mapy<<4),0xF0);

}
void AutoForm::EnableFire()
{
	//if(!IsStand)
	//	FormateToLine();
	if(!GetState(15))//EnableFire
	{
		//SetNoSearchVictim(false);
		//BSelf->ClearBOrders();
		//BSelf->AttEnm=1;
		BSelf->ClearNewBOrders();
		SetFireState(BSelf,true);
		//SetNoSearchVictim(true);
		SetState(15,true);
	}
	//AttackBrigade(MainTarget->BSelf);
}
void AutoForm::FireNow()
{
	if(!GetState(15))//EnableFire
	{
		//BSelf->ClearBOrders();
		BSelf->AttEnm=1;
		Stop();
		//SetNoSearchVictim(false);
		SetFireState(BSelf,true);
		SetState(15,true);
	}
	//AttackBrigade(MainTarget->BSelf);
}
void AutoForm::DisableFire()
{
	if(!GetState(16))
	{
		SetFireState(BSelf,false);
		//SetNoSearchVictim(true);
		SetState(16,true);
		//BSelf->AttEnm=0;
	}
}
int AutoForm::GetMoral(Brigade* Br)
{
	if(Br)
	{
		if(Br->Morale>0)
			return Br->Morale/10000;
		return 0;
	}
	else
	if(C)
		return 100;
	else
		return -1;
}
int AutoForm::GetDistTo(Brigade* Br)
{
	int xx, yy;
	byte dd;
	Br->GetCenter(&xx,&yy,&dd);
	return Norma(x-xx,y-yy);
}
//Сколько убьем если стрельним по Br
int AutoForm::GetDieAmount(Brigade* Br,int* ReadyShuters)
{
	int rez=0;
	if(Br)
	{
		word* life = (word*)malloc(Br->NMemb<<1);
		memset(life,0,Br->NMemb<<1);
		*ReadyShuters=0;
		int InForestAmo=0;
		int InForestProc=0;
		if(Br&&Br->WarType&&BSelf->WarType)
		for(int i=3;i<BSelf->NMemb;i++)
		{
			if(BSelf->Memb[i]!=0xFFFF)
			{
				int rnu=-1;
				int dist=999999;
				OneObject* OBs=Group[BSelf->Memb[i]];
				//OBs->newMons->
				if(OBs&&Br->NMemb&&OBs->delay==0)
				{
					int xx=0;
					int yy=0;
					for(int j=0;j<10;j++)
					{
						int in=-1;
						bool flag=true;	
						for(int iter=0;iter<10&&flag;iter++)
						{	
							in=rando()%Br->NMemb;
							if(Br->Memb[in]!=0xFFFF)
							{
								if(Group[Br->Memb[in]]&&Group[Br->Memb[in]]->Life>life[in])
									flag=false;
							}
						}
						if(Br->Memb[in]!=0xFFFF)
						{
							OneObject* OBg=Group[Br->Memb[in]];
							if(OBg)
							{
								int dd=Norma((OBs->RealX>>4)-(OBg->RealX>>4),(OBs->RealY>>4)-(OBg->RealY>>4));
								if(dd<dist)
								{
									dist=dd;
									rnu=in;
									xx=OBg->RealX;
									yy=OBg->RealY;
								}
							}
						}
					}
					//int sh=256;
					//if(dist>300)
						//sh=GetTreeShotPass(xx>>6,yy>>6);
					//life[rnu]+=((GetDamage(BSelf->Memb[i],Br->Memb[rnu],1)*sh)>>7);
					life[rnu]+=GetDamage(BSelf->Memb[i],Br->Memb[rnu],1);
					(*ReadyShuters)++;
				}
			}
		}
		for(int i=0;i<Br->NMemb;i++)
		{
			if(Br->Memb[i]!=0xFFFF)
				if(Group[Br->Memb[i]]&&Group[Br->Memb[i]]->Life<=life[i])
					rez++;
		}
		free(life);
	}
	return rez;
}
// на сколько упадет мораль если стрельнуть по *Br 
int AutoForm::CanIKill(Brigade *Br)
{
	if(Br)
	{
		int av=0;
		int mmor=0;
		int ReadyShuters;
		for(int i=0;i<3;i++)
			av+=GetDieAmount(Br,&ReadyShuters);
		av=(int)(av/3);//if(av%5)av++;
			//SetReadyToFire();
		int mor=Br->Morale;
		mmor=av*Morale_KillDec;
		//mmor+=((2*ReadyShuters*(1100-DistToMT()))/2200)*Morale_DamageDec1;
		mmor+=ReadyShuters*Morale_DamageDec1;
		int VictimAmo;
		int lim=GetNTo30Limit(Br,&VictimAmo);
		if(lim>0&&lim<=av)
			mmor+=Morale_DecWhen30Lost;
		
		int MoraleShild=Morale_FormationShield15+(VictimAmo-15)*(Morale_FormationShield196-Morale_FormationShield15)/(196-15);	
		mmor=(mmor*MoraleShild)/100;
		//char c[32];
		//sprintf(c,"MoraleDownTo - %d(%d)",(mor-mmor)/10000,av);
		//AssignHint1(c,30);
		//if((mor-mmor)/10000<46)
		return (mor-mmor)/10000;
	}
	return -1;
}
int AutoForm::GetNTo30Limit(Brigade *Br,int* VictimAmo)
{
	int amo=0;
	for(int i=3;i<Br->NMemb;i++)
		if(Br->Memb[i]!=0xFFFF)
			amo++;
	int NU=ElementaryOrders[Br->WarType-1].NUnits;
	*VictimAmo=amo;
	return amo-(NU*(100-MaxMorale_LostCriticalPercent))/100;
}
// опустить ружья
void AutoForm::SetReadyToFire()
{
	if(!GetState(11))
	{
		BSelf->AttEnm=true;
		for(int i=3;i<BSelf->NMemb;i++)
		{
			int index=BSelf->Memb[i];
			if(index!=0xFFFF)
			{
				OneObject* OB=Group[index];
				if(OB)
				{
					OB->NewState=1;
					//OB->GroundState=1; //Old
				}
			}
		}
		SetState(11,true);
	}
}
// поднять ружья
void AutoForm::SetNotReadyToFire()
{
	if(GetState(11))
	{
		BSelf->AttEnm=false;
		for(int i=3;i<BSelf->NMemb;i++)
		{
			int index=BSelf->Memb[i];
			if(index!=0xFFFF)
			{
				OneObject* OB=Group[index];
				if(OB)
				{
					OB->NewState=0;
					//OB->GroundState=1; //Old
				}
			}
		}
		SetState(11,false);
	}
}
void AutoForm::SetStandGround(bool vle)
{
	if(BSelf)
	{
		for(int i=3;i<BSelf->NMemb;i++)
		{
			int index=BSelf->Memb[i];
			if(index!=0xFFFF)
			{
				OneObject* OB=Group[index];
				if(OB)
				{
					OB->GroundState=vle;
				}
			}
		}
	}
}
void AutoForm::FormateToLine()
{
	if(MainTarget)
		BSelf->Direction=GetDir(MainTarget->x-x,MainTarget->y-y);
	FormateTo012(0);
}
void AutoForm::FormateToSquare()
{
	FormateTo012(1);
}
void AutoForm::FormateToKare()
{
	FormateTo012(2);
}
void AutoForm::Stop()
{
	ClearOrders();
	//BSelf->ClearBOrders();
	
	byte d=Dir;
	if(MainTarget)
	{
		d=GetDir(MainTarget->x-x,MainTarget->y-y);
	}
	SimpleMoveTo(x,y,d);
	/*
	if(BSelf)
	{
		BSelf->ClearNewBOrders();
		for(int i=0;i<BSelf->NMemb;i++)
		{
			word mid=BSelf->Memb[i];
			if(mid!=0xFFFF)
			{
				OneObject* OB=Group[mid];
				if(OB&&(!OB->Sdoxlo))
				{	
					OB->ClearOrders();
				}
			}
		}
		BSelf->HumanGlobalSendTo(x,y,d,128+16,0);
	}
	*/
}
void AutoForm::Castle()
{
	AutoForm* R=GetFriendInBack();
	if(R&&R->GetState(1)&&Type==R->Type)
	{
		R->TimeInterval=100;
		R->DisableFire();
		R->SetReadyToFire();
		R->SimpleMoveTo(x,y,BSelf->Direction);
		DisableFire();
		StepBack();
	}
}
void _dbgPrintState(const char* var,char* mask,...);
void AutoForm::FormateTo012(int T)
{
	if(BSelf&&BSelf->WarType!=0&&FormationTypeIndex!=T&&T>-1&&T<3)
	{
		//BSelf->ClearNewBOrders();
		OrderDescription* ODE=ElementaryOrders+BSelf->WarType-1;
		if(ODE->GroupID!=0xFF){
			SingleGroup* FGD=FormGrp.Grp+ODE->GroupID;
			ClearOrders();
			//SimpleMoveTo(x,y);
			MakeReformation(BSelf->CT->NI,BSelf->ID,FGD->IDX[T]);
			FormationTypeIndex=T;
			/*
			mtXr=x-((TSin[BSelf->Direction]*GetSizeRight())>>8);
			mtYr=y+((TCos[BSelf->Direction]*GetSizeRight())>>8);

			mtXl=x+((TSin[BSelf->Direction]*GetSizeLeft())>>8);
			mtYl=y-((TCos[BSelf->Direction]*GetSizeLeft())>>8);
			*/
			SimpleMoveTo(x,y);
		}
	}
}
void AutoForm::ClearEnmAng()
{
	for(int i=0;i<16;i++)
	{	
		EnmAng[i].SectorsAmo=0;
		EnmAng[i].SumDang=0;
		EnmAng[i].Dang=0;
		EnmAng[i].FireDang=0;
		EnmAng[i].SumFireDang=0;
	}
}
void AutoForm::ClearFriendAng()
{
	for(int i=0;i<16;i++)
	{
		FriendAng[i].SectorsAmo=0;
		FriendAng[i].SumDang=0;
		FriendAng[i].Dang=0;
		FriendAng[i].FireDang=0;
		FriendAng[i].SumFireDang=0;
	}
}
void AutoForm::AddToEnmAng(Atom *A)
{
	int dist=Norma(A->x-x,A->y-y);
	if(dist<3000)
	{
		byte v2=(byte)(GetDir(A->x-x,A->y-y));
		byte alfa=v2-Dir+8;
		byte ani=alfa>>4;
		Direction* D=&(EnmAng[ani]);

		if(ani&&GetState(12))//Line
		{
			dist-=abs(TSin[alfa]*(GetSizeLeft()-200))>>8;
			if(dist<1)dist=1;
		}
		
		if(D->SectorsAmo<31)
		{
			D->Sectors[D->SectorsAmo].A=A;
			D->Sectors[D->SectorsAmo].dest=dist;
			D->SectorsAmo++;
			int dd=A->Force*(3000-dist)/3000;
			if(dist<600)
			{
				byte lef=(alfa-16);
				byte rat=(alfa+16);
				EnmAng[lef>>4].Dang+=dd/3;
				EnmAng[rat>>4].Dang+=dd/3;
				dd/=3;
			}
			D->Dang+=dd;
			D->SumDang+=A->Force;
			if(dist<2000)
			{
				D->FireDang+=A->FireReady*(2000-dist)/2000;
				/*
				//D->FireDang+=(A->FireReady*100)/(dist+1);
				if(A->BSelf)
				{
					for(int k=3;k<A->BSelf->NMemb;k++)
					{
						if(A->BSelf->Memb[k]!=0xFFFF)
						{
							OneObject* OB =Group[A->BSelf->Memb[k]];
							if(OB)
							{
							}
						}
					}
				}
				else
					D->FireDang+=GetDamFall(dist,AO->Sender->newMons->DamageDecr[AO->AttType],AO->Damage);
				int damage=GetDamFall(RR,AO->Sender->newMons->DamageDecr[AO->AttType],AO->Damage);
				*/
			}
			D->SumFireDang+=A->FireReady;

			for(int i=(D->SectorsAmo-1);i>0;i--)
			{
				if(D->Sectors[i].dest<D->Sectors[i-1].dest)
				{
					Atom* aa=D->Sectors[i].A;
					int dd=D->Sectors[i].dest;
					D->Sectors[i].A=D->Sectors[i-1].A;
					D->Sectors[i].dest=D->Sectors[i-1].dest;
					D->Sectors[i-1].A=aa;
					D->Sectors[i-1].dest=dd;
				}
			}
		}
	}
}
void AutoForm::AddToFriendAng(AutoForm *AF)
{
	int dist=Norma(AF->x-x,AF->y-y);
	if(dist<3000)
	{
		if(!dist)dist=1;
		byte v2=(byte)(GetDir(AF->x-x,AF->y-y));
		byte alfa=v2-Dir+8;
		byte ani=alfa>>4;
		Direction* D=&(FriendAng[ani]);
		
		if(D->SectorsAmo<31)
		{
			D->Sectors[D->SectorsAmo].AF=AF;
			D->Sectors[D->SectorsAmo].dest=dist;
			D->SectorsAmo++;
			int dd=AF->Force*(3000-dist)/3000;
			if(dist<700)
			{
				byte lef=(alfa-16);
				byte rat=(alfa+16);
				FriendAng[lef>>4].Dang+=dd/6;
				FriendAng[rat>>4].Dang+=dd/6;
				dd-=dd/3;
			}
			if(dist<300)
			{
				byte lef=(alfa-32);
				byte rat=(alfa+32);
				FriendAng[lef>>4].Dang+=dd/12;
				FriendAng[rat>>4].Dang+=dd/12;
				dd-=dd/6;
			}
			D->Dang+=dd;
			D->SumDang+=AF->Force;
			if(dist<2000)
			{
				D->FireDang+=AF->FireReady*(2000-dist)/2000;
				//D->FireDang+=(AF->FireReady*100)/(dist+1);
			}
			D->SumFireDang+=AF->FireReady;

			for(int i=(D->SectorsAmo-1);i>0;i--)
			{
				if(D->Sectors[i].dest<D->Sectors[i-1].dest)
				{
					AutoForm* aa=D->Sectors[i].AF;
					int dd=D->Sectors[i].dest;
					D->Sectors[i].AF=D->Sectors[i-1].AF;
					D->Sectors[i].dest=D->Sectors[i-1].dest;
					D->Sectors[i-1].AF=aa;
					D->Sectors[i-1].dest=dd;
				}
			}
		}
	}
}
byte AutoForm::GetMaxDangDirections()
{
	int md=0;
	byte rez=0;
	for(int i=0;i<16;i++)
		if(EnmAng[i].Dang>md)
		{
			rez=EnmAng[i].Dir;
			md=EnmAng[i].Dang;
		}
	return rez;
}
//сюда добавлять новые функции
int AutoForm::GetSituationByIndex(int I)
{
	int rez=-1;
    try{//o, gore mne!
	
	    if(FunctionResultCash[I][1]==tmtmt)
	    {
		    rez=FunctionResultCash[I][0];
	    }
	    else
	    {
		    switch (I)
		    {
		    case  0: rez=DistToMT(); break;
		    case  1: rez=MoralMT(); break;
		    case  2: rez=KillAmoMT(); break;
		    case  3: rez=FireReadyMT(); break;
		    case  4: rez=UnitsAmountMT(); break;
		    case  5: rez=TiredMT(); break;
		    case  6: rez=MoralMTAfterFire(); break;
		    case  7: rez=Moral(); break;
		    case  8: rez=GetFireReady(); break;
		    case  9: rez=UnitsAmount(); break;
		    case 10: rez=GetTired(); break;
		    case 11: rez=GetDistToAim(); break;
		    case 12: rez=SumDangAndForce(); break;
		    case 13: rez=FireForceDifference(); break;
		    case 14: rez=Encircle(); break;
		    case 15: rez=HaveReserv(); break;
		    case 16: rez=AngToMT(); break;
		    case 17: rez=DistInFront(); break;
		    case 18: rez=DistInReserv(); break;
		    case 19: rez=GetStand(); break;
		    case 20: rez=IsEnemOnWay(); break;
		    case 21: rez=GetNGrenades(); break;
		    case 22: rez=GetMTForestDensity(); break;
		    case 23: rez=IsMTCannon(); break;
		    case 24: rez=KartechDanger(); break;
		    case 25: rez=FriendlyKartechDanger(); break;
		    case 26: rez=IsInBatter(); break;
		    case 27: rez=GetBarrierToMT(); break;
		    case 28: rez=FireDanger(); break;
		    case 29: rez=GetStandTime(); break;
		    case 30: rez=GetMTStandTime(); break;
		    case 31: rez=GetAloneEnmUnits(); break;
		    case 32: rez=GetMeleeForceDiff(); break;
		    case 33: rez=GetNUnitsDiff(); break;
		    case 34: rez=Random(); break;
		    case 35: rez=IsStandGround(); break;
		    case 36: rez=IsMTStandGround(); break;
		    case 37: rez=GetHorseDanger(); break;
		    case 38: rez=MoralDiff(); break;
		    case 39: rez=GetPolkCommand(); break;
		    case 40: rez=GetDistToEnmTower(); break;
		    case 41: rez=GetSelfForestDensity(); break;
		    case 42: rez=GetFOOD(); break;
		    case 43: rez=GetCOAL(); break;
		    case 44: rez=GetLastCommTime(); break;
		    case 45: rez=GetDistToFriendlyForm(); break;
		    case 46: rez=GetNFormationsInMyPolk(); break;
		    case 47: rez=GetDistToEnmBuilding(); break;
		    case 48: rez=GetGameTime(); break;
		    case 49: rez=GetFrontage(); break;
		    //Cannon
		    case 50: rez=GetMaxDensityInZerroSector(); break;
		    case 51: rez=GetMaxDensityInSectors(); break;
		    case 52: rez=GetKartechEffect(); break;
		    case 53: rez=GetPushkaChargeState0(); break;
		    case 54: rez=GetPushkaChargeState1(); break;
		    case 55: rez=GetFillingAbility(); break;
		    //
		    case 56: rez=GetDiff(); break;
		    case 57: rez=IsMTAI(); break;
		    case 58: rez=GetMTAngl(); break;
		    case 59: rez=GetMaxHightInMTDirection(); break;
		    case 60: rez=GetProcDistToAimInPolk(); break;
		    case 61: rez=GetNApparentFriendlyForm(); break;
		    case 62: rez=GetNApparentEnemyForm(); break;
		    case 63: rez=CanSetBomb(); break;
		    //case 64: rez=GetDiff(); break;
		    }
		    if(I>99&&I<132)
			    rez=GetState(I-100);
    		
		    FunctionResultCash[I][0]=rez;
		    FunctionResultCash[I][1]=tmtmt;
	    }
    }catch(...){};
    return rez;
}
int AutoForm::GetMaxValueSituationByIndex(int I)
{
	int rez=1;
	switch(I) 
	{
		case  0: rez=10000; break;
		case  1: rez=300; break;
		case  2: rez=200; break;
		case  3: rez=200; break;
		case  4: rez=300; break;
		case  5: rez=500; break;
		case  6: rez=1000; break;
		case  7: rez=1000; break;
		case  8: rez=200; break;
		case  9: rez=200; break;
		case 10: rez=1000; break;
		case 11: rez=65535; break;
		case 12: rez=10000; break;
		case 13: rez=10000; break;
		case 14: rez=64; break;
		case 15: rez=1; break;
		case 16: rez=64; break;
		case 17: rez=100; break;
		case 18: rez=100; break;
		case 19: rez=1; break;
		case 20: rez=1; break;
		case 21: rez=16; break;
		case 22: rez=100; break;
		case 23: rez=1; break;
		case 24: rez=1000; break;
		case 25: rez=1000; break;
		case 26: rez=1; break;
		case 27: rez=2000; break;
		case 28: rez=10000; break;
		case 29: rez=500; break;
		case 30: rez=500; break;
		case 31: rez=300; break;
		case 32: rez=5000; break;
		case 33: rez=100; break;
		case 34: rez=1000; break;
		case 35: rez=1; break;
		case 36: rez=1; break;
		case 37: rez=1000; break;
		case 38: rez=100; break;
		case 39: rez=10; break;
		case 40: rez=1000000; break;
		case 41: rez=100; break;
		case 42: rez=10000000; break;
		case 43: rez=10000000; break;
		case 44: rez=100; break;
		case 45: rez=3000; break;
		case 46: rez=10; break;
		case 47: rez=1000000; break;
		case 48: rez=60; break;
		case 49: rez=1500; break;
			//Cannon
		case 50: rez=1000; break;
		case 51: rez=1000; break;
		case 52: rez=1000; break;
		case 53: rez=100; break;
		case 54: rez=100; break;
		case 55: rez=1; break;
			//
		case 56: rez=100; break;
		case 57: rez=1; break;
		case 58: rez=256; break;
		case 59: rez=1000; break;
	}
	return rez;
}
int AutoForm::DistToMT()
{
	int rez=-1;
	if(MainTarget&&MainTarget->Type)
	{
		rez=Norma(x-MainTarget->x,y-MainTarget->y);
		/*
		if(MainTarget->BSelf)
		{
			char dd=(char)(GetDir(x-MainTarget->x,y-MainTarget->y));
			char ds=(char)MainTarget->Dir;
			byte ot=abs(dd-ds)&0xF0;
			rez-=abs((MainTarget->GetSizeLeft()*TSin[ot])>>8);
		}
		if(BSelf)
		{
			char dd=(char)(GetDir(MainTarget->x-x,MainTarget->y-y));
			char ds=(char)Dir;
			byte ot=abs(dd-ds)&0xF0;
			rez-=abs((GetSizeLeft()*TSin[ot])>>8);
		}
		if(rez<1)rez=1;
		*/
	}
	else
		rez=10000;
	return rez;
}
int AutoForm::CanIKillMT()
{
	int rez=0;
	if(MainTarget)
		rez=CanIKill(MainTarget->BSelf);
	return rez;
}
int AutoForm::MoralMT()
{
	int rez=0;
	if(MainTarget&&MainTarget->BSelf)
		rez=GetMoral(MainTarget->BSelf);
	return rez;
}
int AutoForm::MoralDiff()
{
	if(MainTarget&&MoralMT())
		return (Moral()*100)/(MoralMT()+2);
	return -1;
}
int AutoForm::KillAmoMT()
{
	int rez=0;
	int ReadyShuters;
	if(MainTarget)
		rez=GetDieAmount(MainTarget->BSelf,&ReadyShuters);
	return rez;
}
int AutoForm::FireReadyMT()
{
	int rez=-1;
	if(MainTarget)
	{
		if(MainTarget->UnitsAmo)
		{
			rez=(MainTarget->FireReady*100)/(MainTarget->UnitsAmo+1);
		}
		else
		{
			rez=0;
		}
	}
	return rez;
}
int AutoForm::UnitsAmountMT()
{
	if(MainTarget)
		return MainTarget->UnitsAmo;
	return -1;
}
int AutoForm::TiredMT()
{
	int rez=-1;
	if(MainTarget)
		rez=MainTarget->Tired/1000;
	return rez;
}
int AutoForm::AngToMT()
{
	int rez=-1;
	if(MainTarget)
	{
		char dd=(char)(GetDir(MainTarget->x-x,MainTarget->y-y));
		char ds=(char)Dir;
		char ot=dd-ds;
		rez=abs(ot);
	}
	return rez;
}
int AutoForm::SumDangAndForce()
{
	int rez=Force;
	for(int i=0;i<16;i++)
	{
		rez+=FriendAng[i].Dang;
		rez-=EnmAng[i].Dang;
	}
	return rez;
}
bool AutoForm::HaveReserv()
{
	bool rez=false;
	AutoForm* R=GetFriendInBack();
	if(R)
	{
		if(R->GetFireReady()>90&&R->GetState(1)&&R->Type==Type)
			rez=true;
	}
	return rez;
}
int AutoForm::MoralMTAfterFire()
{
	if(MainTarget)
		return CanIKill(MainTarget->BSelf);
	else
		return -1;
}
int AutoForm::Moral()
{
	return GetMoral(BSelf);
}
int AutoForm::GetFireReady()
{
	if(UnitsAmo)
		return (FireReady*100)/UnitsAmo;
	else
		return -1;
}
int AutoForm::UnitsAmount()
{
	return UnitsAmo;
}
int AutoForm::GetTired()
{
	return Tired/1000;
}
void AutoForm::TurnToMT()
{
	if(MainTarget)
	{
		byte a=GetDir(MainTarget->x-x,MainTarget->y-y);
		if(MainTarget->BSelf&&BSelf)
		{
			int dst=Norma(MainTarget->x-x,MainTarget->y-y);
			int x1=MainTarget->x+((dst*TCos[MainTarget->BSelf->Direction])>>8);
			int y1=MainTarget->y+((dst*TSin[MainTarget->BSelf->Direction])>>8);
			int delt=Norma(x-x1,y-y1);
			if(delt<170)
			{
				MoveTo(x1,y1,MainTarget->BSelf->Direction+128);
				return;
			}
		}
		TurnTo(a);
		
		/*for(int i=0;i<16;i++)
			if(EnmAng[i].Dang>0&&EnmAng[i].Sectors[0].A==MainTarget)
			{
				byte nd=BSelf->Direction+EnmAng[i].Dir;
				TurnTo(nd);
				return;
			}
			*/
	}
}
void AutoForm::MoveToMT()
{
	if(MainTarget)
	{
		int dist=150;
		byte d1=MainTarget->Dir;
		if(MainTarget->BSelf)
			d1=MainTarget->BSelf->Direction;
		int x1=MainTarget->x+((dist*TCos[d1])>>8);
		int y1=MainTarget->y+((dist*TSin[d1])>>8);

		byte d2=d1+128;
		if(MainTarget->BSelf)
			d2=MainTarget->BSelf->Direction+128;
		int x2=MainTarget->x+((dist*TCos[d2])>>8);
		int y2=MainTarget->y+((dist*TSin[d2])>>8);

		//SetReadyToFire();

		if(Norma(x1-x,y1-y)>Norma(x2-x,y2-y))
		{
			SimpleMoveTo(x2,y2,d1);
		}
		else
		{
			SimpleMoveTo(x1,y1,d2);
		}
	}
}
int AutoForm::GetMainTargetAngl()
{
	if(MainTarget)
		for(int i=0;i<16;i++)
			if(EnmAng[i].Dang>0&&EnmAng[i].Sectors[0].A==MainTarget)
				return EnmAng[i].Dir;
	return -1;
}
Atom* AutoForm::GetMaxDanderAtom()
{
	int md=0;
	Atom* rez=NULL;
	for(int i=0;i<16;i++)
		if((EnmAng[i].Dang-FriendAng[i].Dang)>md)
		{
			rez=EnmAng[i].Sectors[0].A;
			md=EnmAng[i].Dang-FriendAng[i].Dang;
		}
	return rez;
}
Atom* AutoForm::GetNearestAtom()
{
	Atom* rez=NULL;
	rez=GetNearestAtomIn(x, y); 
	/*
	for(int i=0;i<16;i++)
		if(EnmAng[i].SumDang&&EnmAng[i].Sectors[0].dest<md&&EnmAng[i].Sectors[0].A->Type!=0)
		{
			rez=EnmAng[i].Sectors[0].A;
			md=EnmAng[i].Sectors[0].dest;
		}
		
	if(rez&&MainTarget&&MainTarget->Type!=0)
	{
		int md=Norma(x-rez->x,y-rez->y);
		int pr=(md*100)/Norma(x-MainTarget->x,y-MainTarget->y);
		char d1=(char)GetDir(MainTarget->x-x,MainTarget->y-y);
		char d2=(char)GetDir(rez->x-x,rez->y-y);
		if(pr>80&&abs(d1-d2)>16) 
			rez=MainTarget;
	}
	*/
	return rez;
}
Atom* AutoForm::GetNearestAtomIn(int xx, int yy)
{
	int md=99999999;
	Atom* rez=NULL;
	for(int i=0;i<16;i++)
	{
		if(EnmAng[i].SumDang)
		{
			for(int j=0;j<EnmAng[i].SectorsAmo;j++)
			{
				int ex=EnmAng[i].Sectors[j].A->x;
				int ey=EnmAng[i].Sectors[j].A->y;
				int ds = Norma(ex-xx,ey-yy);
				if(ds<md&&EnmAng[i].Sectors[j].A->Type!=4)
				{
					rez=EnmAng[i].Sectors[j].A;
					md=ds;
				}
			}
		}	
	}
	if(rez&&MainTarget&&MainTarget->Type!=0)
	{
		int md=Norma(xx-rez->x,yy-rez->y);
		int pr=(md*100)/(Norma(xx-MainTarget->x,yy-MainTarget->y)+1);
		char d1=(char)GetDir(MainTarget->x-xx,MainTarget->y-yy);
		char d2=(char)GetDir(rez->x-xx,rez->y-yy);
		if(pr>90&&pr<110&&abs(d1-d2)>16) 
			rez=MainTarget;
	}
	return rez;
}
void AutoForm::ProcessRules()
{
	if(RulesList)
		RulesList->VerifySituation(this);
}
bool AutoForm::GetState(int Index)
{
	if(Index==11&&BSelf)
		return BSelf->AttEnm;
	if(Index>-1&&Index<32)
		return StateArr[Index];
	else
		return false;
}
void AutoForm::SetState(int Index, bool Vle)
{
	if(Index>-1&&Index<32)
	{
		StateArr[Index]=Vle;
		if(Vle)
			for(int i=0;i<32;i++)
				if(NonStateArr[Index][i])
					StateArr[i]=false;
	}
}
int AutoForm::GetMaxActions()
{
	int rez=-1;
	int max=0;
	for(int i=0;i<128;i++)
		if(max<ActionsArr[i])
		{
			max=ActionsArr[i];
			rez=i;
		}
	return rez;
}
// а сюда новые команды
void AutoForm::ExecuteActions(int ActionsID)
{
	/*
	0 - MoveToMT !
	1 - TurnToMT !
	2 - MoveBack 
	3 - SetReadyToFire !
	4 - EnableFire !
	5 - DisableFire !
	6 - Stop
	7 - FormateToLine ! 
	8 - FormateToSquare !
	9 - FormateToKare !
	10- FindPlaceInFront();
	11- TurnToMaxDang();
	12 -MoveToAim
		*/
		/*
		0  - Attack
		1  - Reserv
		2  - MoveBack
		3  - Stand
		4  - Front
		5  - March
		6  - NearAim
		7  - MoveFront
		8  - BattleFormation
		9  - Rest
		10 - Move
		11 - ReadyToFire
		12 - LineFormation
		13 - SquareFormation
		14 - KareFormation
		*/
	int div=5;
	
	if(ActionsID>-1)
	{
		//   LOG   //
		/*
		if(ImNSL[NI]&&BSelf)
		{
			word* Sel=ImSelm[NI];
			if(Sel[0]!=0xFFFF)
			{
				OneObject* OB=Group[Sel[0]];
				if(OB&&!OB->Sdoxlo&&OB->BrigadeID==BSelf->ID)
				{
					FILE* F=fopen("BrigadeAI.log","a");
					if(F){
						fprintf(F,"Time:%d NI:%d BrigadeID:%d Command:%d\n",AnimTime/(25*256),NI,BSelf->ID,ActionsID);
						fclose(F);
					}
				}
			}
		}
		*/
		//         //
		switch (ActionsID)
		{
		case -1: 
			break;
		case 0:
			MoveToMT();
			SetState(0,true);
			TimeInterval=140/div;// время в кадрах на исполнение команды
			break;
		case 1:
			TurnToMT();
			SetState(3,true);
			TimeInterval=200/div;
			break;
		case 2:
			MoveBack();
			SetState(2,true);
			TimeInterval=300/div;
			break;
		case 3:
			SetReadyToFire();
			TimeInterval=46/div;
			break;
		case 4:
			EnableFire();
			SetState(15,true);
			TimeInterval=46+29+8;
			break;
		case 5:
			DisableFire();
			SetState(16,true);
			TimeInterval=4/div;
			break;
		case 6:
			Stop();    //Rest
			SetState(9,true);
			TimeInterval=149/div;
			break;
		case 7:
			FormateToLine();
			SetState(12,true);
			TimeInterval=150/div;
			break;
		case 8:
			FormateToSquare();
			SetState(13,true);
			TimeInterval=160/div;
			break;
		case 9:
			FormateToKare();
			SetState(14,true);
			TimeInterval=250/div;
			break;
		case 10:
			FindPlaceInFront();
			//SetState(8,true);
			TimeInterval=270/div;
			break;
		case 11:
			TurnToMaxDang();
			TimeInterval=250/div;
			break;
		case 12:
			MoveToAim();
			SetState(5,true);//March
			TimeInterval=111/div;
			break;
		case 13:
			Castle();//рокировка
			TimeInterval=222/div;
			break;
		case 14:
			FireNow();
			TimeInterval=2/div;
			break;
		case 15:
			KeepDistInFront();
			TimeInterval=360/div;
			break;
		case 16:
			KeepDistInReserv();
			TimeInterval=210/div;
			break;
		case 17:
			Fire();
			SetState(0,true);
			TimeInterval=33/div;
			break;
		case 18:
			SetNotReadyToFire();
			TimeInterval=12/div;
			break;
		case 19:
			ThrowGrenade();
			TimeInterval=34/div;
			break;
		case 20:
			MeleeAttack();
			SetState(0,true);
			TimeInterval=440/div;
			break;
		case 21:
			OneLineShot();
			SetState(15,true);
			TimeInterval=(46+29+8)/div;
			break;
		case 22:
			TakeInTheRear();
			SetState(0,true);
			TimeInterval=50/div;
			break;
		case 23:
			AttackAloneEnmUnits();
			SetState(0,true);
			TimeInterval=51/div;
			break;
		case 24:
			DoNothing();
			TimeInterval=3/div;
			break;
		case 25:
			StepForward();
			TimeInterval=30/div;
			break;
		case 51:
			ChargeCannon0();
			TimeInterval=150/div;
			break;
		case 52:
			ChargeCannon1();
			TimeInterval=150/div;
			break;
		case 53:
			TurnCannonToMaxEnmFormationSector();
			TimeInterval=150/div;
			break;
		case 54:
			FireOnEnmTower();
			TimeInterval=150/div;
			break;
		case 55:
			FireOnEnmBuilding();
			TimeInterval=150/div;
			break;
		case 56:
			ArmAttack();
			TimeInterval=150/div;
			break;
		case 57:
			Ram();
			SetState(0,true);
			TimeInterval=10/div;
			break;
		case 58:
			SplashFire();
			SetState(0,true);
			TimeInterval=10/div;
			break;
		case 59:
			ThrowGrenadeInABuilding();
			TimeInterval=10/div;
			break;
		case 60:
			SetBomb();
			TimeInterval=100/div;
			break;
		}
		LastCommTime=AnimTime;
	}
	int dst=GetSituationByIndex(0);//DistToMT
	int DistTimeInterval=dst/200;
	if(DistTimeInterval>100)DistTimeInterval=100;
	TimeInterval+=DistTimeInterval;
	if(TimeInterval==0)TimeInterval=1;
}
void AutoForm::CalkAvrEnmDir()
{
	int dx=0;
	int dy=0; 
	for(int i=0;i<16;i++)
	{
		for(int j=0;j<EnmAng[i].SectorsAmo;j++)
		{
			int ds=EnmAng[i].Sectors[j].dest+1;
			int xx=((EnmAng[i].Sectors[j].A->x-x)*100)/ds;
			int yy=((EnmAng[i].Sectors[j].A->y-y)*100)/ds;
			if(ds<2000)
			{
				dx+=xx*(2000-ds)/3000;
				dy+=yy*(2000-ds)/3000;
			}
		}
	}
	AvrEnmDir=GetDir(dx,dy);
}
byte AutoForm::GetAvrEndDirIn(int xx,int yy)
{
	int dx=0;
	int dy=0;
	for(int i=0;i<16;i++)
	{
		for(int j=0;j<EnmAng[i].SectorsAmo;j++)
		{
			int xx1=EnmAng[i].Sectors[j].A->x-xx;
			int yy1=EnmAng[i].Sectors[j].A->y-yy;
			int dst=Norma(xx,yy);
			if(dst<2000)
			{
				dx+=(xx1*(2000-dst))/2000;
				dy+=(yy1*(2000-dst))/2000;
			}
		}
	}
	return GetDir(dx,dy);
}
int AutoForm::GetDistToAim()
{
	int rez=-1;
	if(AimX>-1&&AimY>-1)
		rez=Norma(AimX-x,AimY-y);
	return rez;
}
void AutoForm::MoveToAim()
{
	if(AimX>-1&&AimY>-1)
	{
		SetNotReadyToFire();
		if(AimDir!=-1)
			MoveTo(AimX,AimY,AimDir);
		else
			MoveTo(AimX,AimY);
		SetState(5,true);//Marsh
	}
}
void AutoForm::MoveToFrontOf(AutoForm *A)
{
	if(A)
	{
		int r=A->GetSizeBack()+GetSizeFront();
		byte al=A->BSelf->Direction;
		int xx=A->x+((r*TCos[al])>>8);
		int yy=A->y+((r*TSin[al])>>8);
		MoveTo(xx,yy,A->BSelf->Direction);
	}
}
void AutoForm::MoveToBackOf(AutoForm *A)
{
	if(A)
	{
		int r=A->GetSizeBack()+GetSizeFront();
		byte al=0;
		if(A->BSelf)
			al=A->BSelf->Direction+128;
		if(A->C)
			al=A->C->RealDir+128;
		int xx=A->x+((r*TCos[al])>>8);
		int yy=A->y+((r*TSin[al])>>8);
		MoveTo(xx,yy,al+128);
	}
}
void AutoForm::MoveToLeftOf(AutoForm *A)
{
	if(A)
	{
		int r=A->GetSizeLeft()+GetSizeRight();
		byte al=A->BSelf->Direction-64;
		int xx=A->x+((r*TCos[al])>>8);
		int yy=A->y+((r*TSin[al])>>8);
		MoveTo(xx,yy,A->BSelf->Direction);
	}
}
void AutoForm::MoveToRightOf(AutoForm *A)
{
	if(A)
	{
		int r=A->GetSizeRight()+GetSizeLeft();
		byte al=A->BSelf->Direction+64;
		int xx=A->x+((r*TCos[al])>>8);
		int yy=A->y+((r*TSin[al])>>8);
		MoveTo(xx,yy,A->BSelf->Direction);
	}
}
AutoForm* AutoForm::GetFriendInFront()
{
	AutoForm* rez=NULL;
	byte an=4;
	for(int i=14;i<16;i++)
		if(FriendAng[i].Dang&&(16-i)<an)
		{
			an=16-i;
			rez=FriendAng[i].Sectors[0].AF;
		}
	for(i=0;i<3;i++)
		if(FriendAng[i].Dang&&i<an)
		{
			an=i;
			rez=FriendAng[i].Sectors[0].AF;
		}
	return rez;
}
AutoForm* AutoForm::GetFriendInBack()
{
	AutoForm* rez=NULL;
	byte an=3;
	for(int i=6;i<11;i++)
		if(FriendAng[i].SumDang&&abs(8-i)<an&&FriendAng[i].Sectors[0].dest<1200&&FriendAng[i].Sectors[0].AF->GetState(1))
		{
			an=abs(8-i);
			rez=FriendAng[i].Sectors[0].AF;
		}
	return rez;
}
AutoForm* AutoForm::GetFriendInLeft()
{
	AutoForm* rez=NULL;
	byte an=2;
	for(int i=3;i<6;i++)
		if(FriendAng[i].Dang&&abs(4-i)<an&&FriendAng[i].Sectors[0].dest<1200)
		{
			an=abs(4-i);
			rez=FriendAng[i].Sectors[0].AF;
		}
	return rez;
}
AutoForm* AutoForm::GetFriendInRight()
{
	AutoForm* rez=NULL;
	byte an=2;
	for(int i=11;i<14;i++)
		if(FriendAng[i].Dang&&abs(12-i)<an&&FriendAng[i].Sectors[0].dest<1200)
		{
			an=abs(12-i);
			rez=FriendAng[i].Sectors[0].AF;
		}
	return rez;
}
void AutoForm::FindPlaceInFront()
{
	int a1=GetNearestFriendDirection();
	int a2=GetMaxDangDirectionMinus();
	char fr=(char)(a1<<4);
	char en=(char)(a2<<4);
	if(a1==-1||(abs(fr)>50&&abs(en)<64))
	{
		SetState(4,true);//Front
	}
	else
	{
		SetState(1,true);//Reserv;
	}
	//if(!IsInBatleFormation())
 	if(a1>-1&&a2>-1)
	{
		char ang=fr-en;
		if(GetState(1)&&abs(ang)<55)
		{
			if(FriendAng[a1].Sectors[0].AF->GetState(4))
			{
				bool flag=false;
				if(ang>0)
					if(!FriendAng[a1].Sectors[0].AF->GetFriendInLeft())
					{
						MoveToLeftOf(FriendAng[a1].Sectors[0].AF);
						SetState(4,true);
						flag=true;
					}
				else
					if(!FriendAng[a1].Sectors[0].AF->GetFriendInRight())
					{
						MoveToRightOf(FriendAng[a1].Sectors[0].AF);
						SetState(4,true);
						flag=true;
					}
				if(!flag)				
				{
					if(!FriendAng[a1].Sectors[0].AF->GetFriendInBack())
						MoveToBackOf(FriendAng[a1].Sectors[0].AF);
					SetState(1,true);
				}
				SetState(8,true);
			}
			else
			{
				//wait
			}
		}
		/*
		else
		{
			//TurnToMaxDang();
			SetState(8,true);//BattleFormation
			SetState(4,true);//Front
		}
		*/
	}
}
void AutoForm::FindPlaceInReserv()
{
	if(GetState(1))//Reserv
	{
		int dd=99999999;
		AutoForm* Frontovic=NULL;
		for(int i=0;i<16;i++)
			if(FriendAng[i].Dang)
				for(int j=0;j<FriendAng[i].SectorsAmo;j++)
					if(FriendAng[i].Sectors[j].AF->GetState(4))
						if(FriendAng[i].Sectors[j].dest<dd)
							if(!(FriendAng[i].Sectors[j].AF->GetFriendInBack()&&FriendAng[i].Sectors[j].AF->GetFriendInBack()->GetState(1)))
							{
								dd=FriendAng[i].Sectors[j].dest;
								Frontovic=FriendAng[i].Sectors[j].AF;
							}
		if(Frontovic)
			MoveToBackOf(Frontovic);
	}
}
int  AutoForm::GetMaxDangDirectionMinus()
{
	int rez=-1;
	int dd=-99999999;
	for(int i=0;i<16;i++)
		if((EnmAng[i].Dang-FriendAng[i].Dang)>dd)
		{
			rez=i;
			dd=EnmAng[i].Dang-FriendAng[i].Dang;
		}
	return rez;
}
int AutoForm::GetNearestFriendDirection()
{
	int rez=-1;
	int dd=999999;
	for(int i=0;i<16;i++)
		if((FriendAng[i].SumDang>0)&&(FriendAng[i].Sectors[0].dest<dd))
		{
			rez=i;
			dd=FriendAng[i].Sectors[0].dest;
		}
	return rez;
}
bool AutoForm::IsInBatleFormation()
{
	AutoForm* Fa=GetFriendInFront();
	AutoForm* Ba=GetFriendInBack();
	AutoForm* La=GetFriendInLeft();
	AutoForm* Ra=GetFriendInRight();
	if((Fa&&Fa->GetState(8))||(Ba&&Ba->GetState(8))||(La&&La->GetState(8))||(Ra&&Ra->GetState(8)))
		return true;
	return false;
}
bool AutoForm::IsNearAim()
{
	bool rez=true;
	if(AimX!=-1&&AimY!=-1)
		if(Norma(x-AimX,y-AimY)>600)
			rez=false;
	return rez;
}
void AutoForm::TurnToMaxDang()
{
	/*
	int d=GetMaxDangDirectionMinus();
	if(d>0)
	{
		byte an=(byte) d;
		an=an<<4;
		byte ra=BSelf->Direction-an;
		//BSelf->HumanLocalSendTo(x<<4,y<<4,ra,128,0);
		MoveTo(x,y,ra);
	}
	*/
	if(MainTarget)
		MoveTo(x,y,AvrEnmDir);
}
int AutoForm::ForceDifference()
{
	int rez=0;
	for(int i=0;i<16;i++)
		rez+=FriendAng[i].Dang-EnmAng[i].Dang;
	return rez;
}
int AutoForm::FireForceDifference()
{
	int rez=0;
	for(int i=0;i<16;i++)
		rez+=FriendAng[i].FireDang-EnmAng[i].FireDang;
	return rez;
}
int AutoForm::FireDanger()
{
	int rez=0;
	for(int i=0;i<16;i++)
		rez+=EnmAng[i].FireDang;
	return rez;
}
int AutoForm::Encircle()
{
	int co=0;
	byte razv=0;
	byte vr=0;
	for(int i=0;i<16;i++)
	{
		if(co)
			vr++;
		if(EnmAng[i].SumDang)
		{
			if(EnmAng[i].Sectors[0].dest<1100)
			{
				if(co)
				{ 
					if(vr>8)vr=16-vr;
					razv+=vr;
					vr=0;
				}
				co++;
			}
		}
	}
	return razv<<4;
}
//UpLevel
int AutoForm::DistInFront()
{
	int rez=-1;
	if(DistToMainTarget>0&&GetState(4))
	{
		int d=DistToMT();
		rez=(d*100)/(DistToMainTarget+1);
	}
	return rez;
}
void AutoForm::KeepDistInFront()
{
	if(DistToMainTarget!=-1)
	{
		//GetCoordToMTWay(xx, yy, FindNearestFriendInMTDirection());
		if(MainTarget)
		{
			byte ang=GetDir(x-MainTarget->x,y-MainTarget->y);
			int dx=(TCos[ang]*DistToMainTarget)>>8;
			int dy=(TSin[ang]*DistToMainTarget)>>8;
			MoveTo(MainTarget->x+dx,MainTarget->y+dy,ang+128);
		}
	}
}
int AutoForm::DistInReserv()
{
	int rez=-1;
	if(GetState(1))
	{
		AutoForm* Front = NULL;//FindNearestFriendInMTDirection();
		int dd=999999;
		for(int i=0;i<16;i++)
		{
			if(FriendAng[i].Dang)
			{
				for(int j=0;j<FriendAng[i].SectorsAmo;j++)
				{
					if(FriendAng[i].Sectors[j].AF->GetState(4))
					{
						if(FriendAng[i].Sectors[j].dest<dd)
						{
							Front=FriendAng[i].Sectors[j].AF;
							dd=FriendAng[i].Sectors[j].dest;
						}
					}
				}
			}
		}
		if(Front)
		{
			int dst=Norma(x-Front->x,y-Front->y);
			if(!dst)dst=1;
			rez=(dst*100)/(Front->GetSizeBack()+GetSizeFront()+1);
		}
	}
	return rez;
}
void AutoForm::KeepDistInReserv()
{
	if(GetState(1))
	{
		AutoForm* Front = FindNearestFriendInMTDirection();
		if(Front)
			MoveToBackOf(Front);
		else
			KeepDistInFront();
	}
}
void AutoForm::TakeInTheRear()
{
	if(MainTarget&&MainTarget->BSelf&&MainTarget->BSelf->WarType>0)
	{
		int MinDist=200;
		int Radius=700;
		char mtd=MainTarget->BSelf->Direction;
		char d=(char) GetDir(MainTarget->x-x,MainTarget->y-y);
		char raz=abs(mtd-d);
  		byte dB=MainTarget->BSelf->Direction+128;  

		if(raz>70)
		{
			byte dR=MainTarget->BSelf->Direction+64;
			int rx=MainTarget->x+((Radius*TCos[dR])>>8);
			int ry=MainTarget->y+((Radius*TSin[dR])>>8);
			int dr=Norma(rx-x,ry-y);
			byte dL=MainTarget->BSelf->Direction-64;
			int lx=MainTarget->x+((Radius*TCos[dL])>>8);
			int ly=MainTarget->y+((Radius*TSin[dL])>>8);
 			int dl=Norma(lx-x,ly-y);

			int dsx=rx;
			int dsy=ry;
			if(dr>dl)
			{
				dsx=lx;
				dsy=ly;
			}
			int md=Norma(x-dsx,y-dsy);
			if(md>600)
			{
				dsx=(x+dsx)/2; 
				dsy=(y+dsy)/2;
				byte mdl=GetDir(dsx-MainTarget->x,dsy-MainTarget->y);
				dsx=MainTarget->x+((Radius*TCos[mdl])>>8);
				dsy=MainTarget->y+((Radius*TSin[mdl])>>8);
			}
			MoveTo(dsx,dsy);
		}
		if(raz<71&&raz>36)
		{
			int RadMin=Radius-300;
			int bx=MainTarget->x+((RadMin*TCos[dB])>>8);
			int by=MainTarget->y+((RadMin*TSin[dB])>>8);
			int md=Norma(x-bx,y-by);
			if(md>600)
			{
				bx=(x+bx)/2; 
				by=(y+by)/2;
				byte mdl=GetDir(bx-MainTarget->x,by-MainTarget->y);
				bx=MainTarget->x+((RadMin*TCos[mdl])>>8); 
				by=MainTarget->y+((Radius*TSin[mdl])>>8);
			}
			MoveTo(bx,by);
		}
		if(raz<37)
		{
			int mx=MainTarget->x+((MinDist*TCos[dB])>>8);
			int my=MainTarget->y+((MinDist*TSin[dB])>>8);
			MoveTo(mx,my);
		}	
	}
}
void AutoForm::Fire()
{
	if(MainTarget)
		AttackBrigade(MainTarget->BSelf);
}
void AutoForm::SplashFire()
{
}
int AutoForm::GetStand()
{
	return IsStand;
}
bool AutoForm::IsEnemOnWay()
{
	bool rez=false;
	int ed=DistToMT();
	if((!IsNearAim())&&ed<2000&&MainTarget)
	{
		int nx,ny;
		if(GetTopZoneCoor(GetNextZone(GetTopZone(x,y),GetTopZone(AimX,AimY)),&nx,&ny))
		{
			int ned=Norma((nx<<6)-MainTarget->x,(ny<<6)-MainTarget->y);
			if(((ned*100)/(ed+1))<70)
				rez=true;
		}
	}
	return rez;
}
int AutoForm::GetNGrenades()
{
	return BSelf->NGrenades;
}
void AutoForm::ThrowGrenade()
{
	ComThrowGrenade(NI,BSelf->ID);
}
void AutoForm::ThrowGrenadeInABuilding()
{
	if(BSelf&&GetNGrenades())
	{
		OneObject* B = GetNearestEnmBuilding();
		if(B)
		{
			BSelf->ThrowOneGrenade(B);
		}
	}
}
void AutoForm::MeleeAttack()
{
	DisableFire();
	//SetReadyToFire();
	if(MainTarget)
	{
		MeleeAttackBrigade(MainTarget->BSelf);
		//SimpleMoveTo(MainTarget->x,MainTarget->y);
	}
}
void AutoForm::AttackAloneEnmUnits()
{
	if(BSelf)
	{
		int Param[3];
		Param[0]=(int)GetNMask();
		word il[100];
		Param[1]=(int)il;
		Param[2]=0;
		int fu=PerformActionOverUnitsInRadius(x, y, 800,AutoForm::GetEnmAlone,(void*) Param);
		if(fu)
		{
			for(int i=0;i<BSelf->NMemb;i++)
			{
				if(BSelf->Memb[i]!=0xFFFF)
				{
					OneObject* OB = Group[BSelf->Memb[i]];
					if(OB&&OB->Attack==0)
					{
						OB->StandGround=false;
						OB->RifleAttack=false;
						OB->AttackObj(il[rando()%Param[2]],128+16);
					}
				}
			}
		}
	}
}
int AutoForm::GetHorseDanger()
{
	int rez=0;
	int i;
	for(i=0;i<16;i++)
		if(EnmAng[i].SumDang)
			for(int j=0;j<EnmAng[i].SectorsAmo;j++)
				if(EnmAng[i].Sectors[j].A->Speed>50)
					rez-=(EnmAng[i].Sectors[j].A->Force*(3000-EnmAng[i].Sectors[j].dest))/3000;
	for(i=0;i<16;i++)
		if(FriendAng[i].SumDang)
			for(int j=0;j<FriendAng[i].SectorsAmo;j++)
				if(FriendAng[i].Sectors[j].AF->Speed>50)
					rez+=(FriendAng[i].Sectors[j].AF->Force*(3000-EnmAng[i].Sectors[j].dest))/3000;
	HorceDanger=rez;
	return HorceDanger;
}
void AutoForm::KeepPositionHorce()
{
	//Find enemy horce
	for(int i=0;i<16;i++)
		if(EnmAng[i].SumDang)
			for(int j=0;j<EnmAng[i].SectorsAmo;j++)
				;
	
	//fireforce

	//FrontLine

	//MoveTo->
}
int AutoForm::GetForestDensity(Brigade *B)
{
	int rez=-1;
	if(B)
	{
		int ua=0;
		int den=0;
		for(int i=0;i<B->NMemb;i=i+5)
		{
			if(B->Memb[i]!=0xFFFF)
			{
				OneObject* OB=Group[B->Memb[i]];
				if(OB)
				{
					den+=GetTreeShotPass(OB->RealX>>4,OB->RealY>>4);
					ua++;
				}
			}
		}
		if(ua)
			den=den/ua;
		rez=((255-den)*100)/255;
	}
	return rez;
}
int AutoForm::GetSelfForestDensity()
{
	return GetForestDensity(BSelf);
}
int AutoForm::GetMTForestDensity()
{
	if(MainTarget)
		return GetForestDensity(MainTarget->BSelf);
	return -1;
}
int AutoForm::IsMTCannon()
{
	if(MainTarget&&MainTarget->C)
		return 1;
	return 0;
}
int AutoForm::KartechDanger()
{
	int rez=0;
	for(int i=0;i<16;i++)
		for(int j=0;j<EnmAng[i].SectorsAmo;j++)
		{
			if(EnmAng[i].Sectors[j].A->Type==3)
			{
				Atom* A= EnmAng[i].Sectors[j].A;	
				if(A->C&&(Norma(A->x-A->C->DstX,A->y-A->C->DstY)<260))
				{
					char Cd=char(A->C->RealDir);
					if(BSelf)
					{
						for(int k=0;k<BSelf->NMemb;k=k+5)
						{
							if(BSelf->Memb[k]!=0xFFFF)
							{
								OneObject* OB=Group[BSelf->Memb[k]];
								if(OB)
								{
									int dst=Norma(A->x-(OB->RealX>>4),A->y-(OB->RealY>>4));
									if(dst<2000)
									{
										char dd = char(GetDir((OB->RealX>>4)-A->x,(OB->RealY>>4)-A->y));
										if(abs(dd-Cd)<32)
											rez++;
									}
								}
							}
						}
					}
				}
			}
		}
	return rez;
}
int AutoForm::FriendlyKartechDanger()
{
	int rez=0;
	for(int i=0;i<16;i++)
		for(int j=0;j<FriendAng[i].SectorsAmo;j++)
		{
			if(FriendAng[i].Sectors[j].AF->Type==3)
			{
				Cannon* A=(Cannon*)FriendAng[i].Sectors[j].AF;	
				if(A->C&&A->ShotType==5&&A->FireTimeDelayLeft>50)
				{
					char Cd=char(A->C->RealDir);
					if(BSelf)
					{
						for(int k=0;k<BSelf->NMemb;k=k+5)
						{
							if(BSelf->Memb[k]!=0xFFFF)
							{
								OneObject* OB=Group[BSelf->Memb[k]];
								if(OB)
								{
									int dst=Norma(A->x-(OB->RealX>>4),A->y-(OB->RealY>>4));
									if(dst<2000)
									{
										char dd = char(GetDir((OB->RealX>>4)-A->x,(OB->RealY>>4)-A->y));
										if(abs(dd-Cd)<32)
											rez++;
									}
								}
							}
						}
					}
				}
			}
		}
	return rez;
}
int AutoForm::IsInBatter()
{
	int rez=0;
	for(int i=0;i<16;i++)
		for(int j=0;j<EnmAng[i].SectorsAmo;j++)
		{
			Atom* A=EnmAng[i].Sectors[j].A;	
			if(A->Type==3&&A->C)
			{
				if(Norma(x-(A->C->DstX>>4),y-(A->C->DstY>>4))<1000)
					rez++;
			}
		}			
	return rez;
}
bool AutoForm::IsInFront()
{
	int MTd=DistToMT();
	if(BSelf&&MainTarget&&DistToMT()<4000)
	{
		byte MTDr=GetDir(MainTarget->x-x,MainTarget->y-y);
		byte NMask=GetNMask();
		for(int i=50;i<MTd;i+=50)  
		{
			int cx=(x+((TCos[MTDr]*i)>>8))>>7;
			int cy=(y+((TSin[MTDr]*i)>>8))>>7;
			if(cx>=0&&cx<(VAL_MAXCX-1)&&cy>=0&&cy<(VAL_MAXCX-1))
			{
				int cell=cx+1+((cy+1)<<VAL_SHFCX);
				int N=MCount[cell];
				int ofs=cell<<SHFCELL;
				for(int j=0;j<N;j++)
				{
					word MID=GetNMSL(ofs+j);
					if(MID!=0xFFFF)
					{
						OneObject* OB=Group[MID];
						if(OB&&(!OB->Sdoxlo)&&OB->BrigadeID!=0xFFFF)
						{
							if(OB->NMask&NMask)
							{
								if(OB->BrigadeID!=BSelf->ID)
									return false;
							}
							else
							{
								return true;
							}							
						}
					}
				}
			}
		}
		return true;
	}
	return false;
}
int AutoForm::GetBarrierToMT()
{
	int rez=0;
	if(MainTarget)
	{
		int Barr=GetTopDistance(x>>6,y>>6,MainTarget->x>>6,MainTarget->y>>6,LockType)<<6;
		int MTd=Norma(x-MainTarget->x,y-MainTarget->y)+1;
		rez=((Barr-MTd)*100)/MTd;
	}
	return rez;
}
int AutoForm::GetFOOD()
{
	return GetResource(NI,3);
}
int AutoForm::GetCOAL()
{
	return GetResource(NI,5);
}
// замутки что б отряды красиво становились
void AutoForm::FillKeyPoints()
{
	ClearKeyPoints();
	int Np=0;
	for(int i=0;i<16;i++)
		for(int j=0;j<FriendAng[i].SectorsAmo;j++)
			if(FriendAng[i].Sectors[j].AF->Type>0)
				Np++;
	NKeyPoints=4*Np;
	KeyPoints=(KeyPoint*)malloc(sizeof(KeyPoint)*NKeyPoints);
	int N=0;
	for(int i=0;i<16;i++)
	{
		for(int j=0;j<FriendAng[i].SectorsAmo;j++)
		{
			if(FriendAng[i].Sectors[j].AF->Type>0&&FriendAng[i].Sectors[j].AF->IsAlive())
			{
				KeyPoints[N*4+0].x=FriendAng[i].Sectors[j].AF->mtXl;
				KeyPoints[N*4+0].y=FriendAng[i].Sectors[j].AF->mtYl;
				KeyPoints[N*4+0].LR=1;//R
				KeyPoints[N*4+0].State=1;
				KeyPoints[N*4+0].Type=1;
				KeyPoints[N*4+0].Width=3000;
				KeyPoints[N*4+0].Creator=FriendAng[i].Sectors[j].AF->Index;

				KeyPoints[N*4+1].x=FriendAng[i].Sectors[j].AF->mtXr;
				KeyPoints[N*4+1].y=FriendAng[i].Sectors[j].AF->mtYr;
				KeyPoints[N*4+1].LR=0;//L
				KeyPoints[N*4+1].State=1;
				KeyPoints[N*4+1].Type=1;
				KeyPoints[N*4+1].Width=3000;
				KeyPoints[N*4+1].Creator=FriendAng[i].Sectors[j].AF->Index;

				byte drr=FriendAng[i].Sectors[j].AF->mtDir+128;
				int r=FriendAng[i].Sectors[j].AF->GetSizeBack()+GetSizeFront();				
				
				KeyPoints[N*4+2].x=KeyPoints[N*4+0].x+((r*TCos[drr])>>8);
				KeyPoints[N*4+2].y=KeyPoints[N*4+0].y+((r*TSin[drr])>>8);
				KeyPoints[N*4+2].LR=0;//L
				KeyPoints[N*4+2].State=1;
				KeyPoints[N*4+2].Type=0;
				KeyPoints[N*4+2].Width=500;
				KeyPoints[N*4+2].Creator=FriendAng[i].Sectors[j].AF->Index;

				KeyPoints[N*4+3].x=KeyPoints[N*4+1].x+((r*TCos[drr])>>8);
				KeyPoints[N*4+3].y=KeyPoints[N*4+1].y+((r*TSin[drr])>>8);
				KeyPoints[N*4+3].LR=1;//R
				KeyPoints[N*4+3].State=1;
				KeyPoints[N*4+3].Type=0;
				KeyPoints[N*4+3].Width=500;
				KeyPoints[N*4+3].Creator=FriendAng[i].Sectors[j].AF->Index;

				N++;
			}
		}
	}
	
	for(int i=0;i<NKeyPoints;i++)
	{
		KeyPoint* P1=&(KeyPoints[i]);
		//if(P1->State!=0)
		{
			for(int j=0;j<NKeyPoints;j++)
			{
				if(i!=j)
				{
					KeyPoint* P2=&(KeyPoints[j]);
					//if(P2->State!=0)
					{
						if(P1->Creator!=P2->Creator)
						{
							int dst=Norma(P1->x-P2->x,P1->y-P2->y);
							if(dst<200) 
							{
								if(P1->Type==1&&P2->Type==1&&(P1->LR+P2->LR)==1)
								{
									P1->State=0;
									P2->State=0;
								}
								if(P1->Type==1&&P2->Type==0)
								{
									P2->State=0;
								}
								if(P1->Type==0&&P2->Type==0)
								{
									if(P1->LR==P2->LR)
									{
										if(P1->State==1)
										{
											P1->x=(P1->x+P2->x)/2;
											P1->y=(P1->y+P2->y)/2;
											P2->State=0;
										}
									}
									else
									{
										int xx=(P1->x+P2->x)/2;
										int yy=(P1->y+P2->y)/2;
										P1->x=xx;
										P1->y=yy;
										P2->x=xx;
										P2->y=yy;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}
void AutoForm::ClearKeyPoints()
{
	if(KeyPoints)
	{
		free(KeyPoints);
		KeyPoints=NULL;
		NKeyPoints=0;
	}
}
// замутки что б отряды красиво становились
bool AutoForm::FindNearestKeyPoints(int &p1, int &p2)
{
	bool rez=false;
	p1=-1;
	p2=-1;
	int MaxL=0;
	int MaxR=0;
	for(int i=0;i<NKeyPoints;i++)
	{
		if(KeyPoints[i].State!=0)
		{
			if(KeyPoints[i].LR==0)//L
			{
				int dst=Norma(KeyPoints[i].x-mtXl,KeyPoints[i].y-mtYl);
				if(dst<900)
				{
					int w=(KeyPoints[i].Width*(900-dst))/900;
					if(w>MaxL)
					{
						p1=i;
						MaxL=w;
						rez=true;
					}
				}
			}
			else
			if(KeyPoints[i].LR==1)//R
			{
				int dst=Norma(KeyPoints[i].x-mtXr,KeyPoints[i].y-mtYr);
				if(dst<900)
				{
					int w=(KeyPoints[i].Width*(900-dst))/900;
					if(w>MaxR)
					{
						p2=i;
						MaxR=w;
						rez=true;
					}
				}
			}
		}
	}
	return rez;
}
// замутки что б отряды красиво становились
void AutoForm::FindMoveToCoord(int &xx, int &yy, byte &dr, int p1, int p2)
{
	int wl=0;
	if(p1!=-1)
	{
		int dst=Norma(KeyPoints[p1].x-mtXl,KeyPoints[p1].y-mtYl);
		wl=(KeyPoints[p1].Width*(2000-dst))/2000;
	}
	int wr=0;
	if(p2!=-1)
	{
		int dst=Norma(KeyPoints[p2].x-mtXr,KeyPoints[p2].y-mtYr);
		wr=(KeyPoints[p2].Width*(2000-dst))/2000;
	}
	byte drr;
	byte drrDif;
	bool drrFl=false;
	if(wl&&wr&&KeyPoints[p1].Creator==KeyPoints[p2].Creator&&KeyPoints[p1].Type==1&&KeyPoints[p2].Type==1)
	{
		if(wl>wr)
			p2=-1;
		else
			p1=-1;
	}
	if(p1!=-1&&p2!=-1)
	{
		drr=GetDir(KeyPoints[p2].x-KeyPoints[p1].x,KeyPoints[p2].y-KeyPoints[p1].y)+64;
		drrFl=true;
		char cdrr=(char)drr;
		//char cd=(char)mtDir;AvrEnmDir
		char cd=(char)AvrEnmDir;
		drrDif=abs(cdrr-cd);
	}
	
	if(wl>wr)
	{
		if(drrFl&&drrDif<33)
		{
			xx=KeyPoints[p1].x+((GetSizeLeft()*TCos[drr-64])>>8);
			yy=KeyPoints[p1].y+((GetSizeLeft()*TSin[drr-64])>>8);
			dr=drr;
		}
		else
		{
			xx=mtX+(KeyPoints[p1].x-mtXl);
			yy=mtY+(KeyPoints[p1].y-mtYl);
			Atom* tmp=GetNearestAtomIn(xx,yy);
			if(tmp)
			{
				dr=GetDir(tmp->x-xx,tmp->y-yy);
			}
			else
				dr=mtDir;
			//dr=GetAvrEndDirIn(xx, yy)+128;
		}
	}
	if(wr>wl)
	{
		if(drrFl&&drrDif<33)
		{
			xx=KeyPoints[p2].x+((GetSizeRight()*TCos[drr+64])>>8);
			yy=KeyPoints[p2].y+((GetSizeRight()*TSin[drr+64])>>8);
			dr=drr;
		}
		else
		{
			xx=mtX+(KeyPoints[p2].x-mtXr);
			yy=mtY+(KeyPoints[p2].y-mtYr);
			Atom* tmp=GetNearestAtomIn(xx,yy);
			if(tmp)
			{
				dr=GetDir(tmp->x-xx,tmp->y-yy);
			}
			else
				dr=mtDir;
			//dr=GetAvrEndDirIn(xx, yy)+128;
		}
	}
}
int AutoForm::GetMTStandTime()
{
	if(MainTarget)
	{
		if(MainTarget->IsStand)
			return AnimTime/(256*25)-MainTarget->StandTime;
		else
			return 0;
	}
	return -1;
}
int AutoForm::GetMeleeForceDiff()
{
	int rez=-1;
	if(BSelf&&MainTarget&&MainTarget->BSelf)
	{
		word b1=0xFFFF;
		word b2=0xFFFF;
		for(int i=3;i<BSelf->NMemb&&b1==0xFFFF;i++)
		{
			b1=BSelf->Memb[i];
		}
		for(i=3;i<MainTarget->BSelf->NMemb&&b2==0xFFFF;i++)
		{
			b2=MainTarget->BSelf->Memb[i];
		}
		if(b1!=0xFFFF&&b2!=0xFFFF)
		{
			int dam1=GetDamage(b1,b2,0);
			int dam2=GetDamage(b2,b1,0);
			//int SL1=SumLife/(UnitsAmo*dam2+1);
			//int SL2=MainTarget->SumLife/(MainTarget->UnitsAmo*dam1+1);
			//rez=(dam1*UnitsAmo*100*SL1)/(dam2*MainTarget->UnitsAmo*SL2+1);
			rez=(dam1*SumLife*100)/(dam2*MainTarget->SumLife+1);
		}
	}
	return rez;
}
int AutoForm::GetNUnitsDiff()
{
	int rez=-1;
	if(UnitsAmo&&MainTarget&&MainTarget->UnitsAmo)
	{
		rez=(UnitsAmo*100)/MainTarget->UnitsAmo;
	}
	return rez;
}
int AutoForm::Random()
{
	return rando()%1000;
}
int AutoForm::IsStandGround()
{
	int rez=0;
	if(BSelf&&BSelf->InStandGround)
		rez=1;
	return rez;
}
int AutoForm::IsMTStandGround()
{
	int rez=0;
	if(MainTarget&&MainTarget->BSelf&&MainTarget->BSelf->InStandGround)
		rez=1;
	return rez;
}
int AutoForm::GetPolkCommand()
{
	return PolkSynchronCommand;
}
int AutoForm::GetDistToEnmTower()
{
	int rez=999999;
	for(int i=0;i<16;i++)
	{
		for(int j=0;j<EnmAng[i].SectorsAmo;j++)
		{
			Atom* A=EnmAng[i].Sectors[j].A;	
			if(A->Type==4&&A->IsAlive()&&EnmAng[i].Sectors[j].dest<rez)
			{
				rez=EnmAng[i].Sectors[j].dest;
			}
		}			
	}
	return rez;
}
bool AutoForm::BuildIsEnm(OneObject* B,void* param)
{
	bool rez=false;
	if(B&&!B->Sdoxlo)
	{
		int* p = (int*) param;
		byte mm = (byte)p[2];
		if(B->NNUM!=7&&!(mm&B->NMask)&&!B->newMons->Immortal)
		{
			int ds=Norma(p[3]-(B->RealX>>4),p[4]-(B->RealY>>4));
			if(ds<p[1])
			{
				p[1]=ds;
				p[0]=(int)B;
				rez=true;
			}
		}
	}
	return rez;
}
OneObject* AutoForm::GetNearestEnmBuilding()
{
	OneObject* rez=NULL;
	int p[8];
	p[0]=0;
	p[1]=999999;
	p[2]=(int)NMask;
	p[3]=x;
	p[4]=y;
	if(PerformActionOverBuildingsInRadius(x,y,3000,&AutoForm::BuildIsEnm,(void*)p))
	{
		rez=(OneObject*)p[0];
	}
	return rez;
}
int AutoForm::GetDistToEnmBuilding()
{
	int rez=999999;
	OneObject* B = GetNearestEnmBuilding();
	if(B)
	{
		rez=Norma(x-(B->RealX>>4),y-(B->RealY>>4));
	}
	return rez;
}
void AutoForm::OneLineShot()
{
	if(BSelf)
	{
		OrderDescription* ODE=ElementaryOrders+BSelf->WarType-1;
		int NA=ODE->NActualLines;
		int IDXS[3]={-1,-1,-1};
		bool LENB[3]={false,false,false};
		bool LPRS[3]={false,false,false};
		int FL=ODE->FirstActualLine;
		int bp=NBPERSONAL;
		int CP=0;
		for(int p=0;p<NA;p++)
		{
			int NU=ODE->LineNU[p+FL];
			if(NU){
				if(CP<3){
					IDXS[CP]=p;
					for(int q=0;q<NU;q++){
						if(bp<BSelf->NMemb){
							word ID=BSelf->Memb[bp];
							if(ID!=0xFFFF){
								OneObject* OB=Group[ID];
								if(OB&&OB->Serial==BSelf->MembSN[bp]){
									LPRS[CP]=true;
									if(OB->delay>0)LENB[CP]=true;
								};
							};
							bp++;
						};
					};
				};
				CP++;
			};
		}
		if(!LENB[2])
			ComShotLine(NI,0,4,BSelf->ID);
		else
		if(!LENB[1])
			ComShotLine(NI,0,2,BSelf->ID);
		else
		if(!LENB[0])
			ComShotLine(NI,0,0,BSelf->ID);
	}
}
int AutoForm::GetAloneEnmUnits()
{
	int rez=0;
	int R=400;
	int Param=(int)GetNMask();
	rez=PerformActionOverUnitsInRadius(x, y, R,AutoForm::IsEnmAlone,(void*) Param);
	if(BSelf)
	{
		if(mtXr!=-1&&mtYr!=-1)
			rez+=PerformActionOverUnitsInRadius(mtXr, mtYr, R-100,AutoForm::IsEnmAlone,(void*) Param);
		if(mtXl!=-1&&mtYl!=-1)
			rez+=PerformActionOverUnitsInRadius(mtXl, mtYl, R-100,AutoForm::IsEnmAlone,(void*) Param);
	}
	return rez;
}
bool AutoForm::IsEnmAlone(OneObject* OB,void* param)
{
	bool rez=false;
	if(OB&&(!OB->Sdoxlo))
	{
		byte mask=(byte)param;
		if((!(OB->NMask&mask))&&OB->BrigadeID==0xFFFF)
			rez=true;
	}
	return rez;
}
bool AutoForm::GetEnmAlone(OneObject* OB,void* param)
{
	bool rez=false;
	if(OB&&(!OB->Sdoxlo))
	{
		int* pr=(int*)param;
		byte mask=(byte)pr[0];
		word* il=(word*)pr[1];
		if(pr[2]<100)
		{
			if((!(OB->NMask&mask))&&OB->BrigadeID==0xFFFF)
			{
				il[pr[2]]=OB->Index;
				pr[2]++;
				rez=true;
			}
		}
	}
	return rez;
}
int AutoForm::GetRealNUnitsToReman(word& Type)
{
	int rez=0;
	bool set=false;
	if(BSelf)
	{
		for(int i=3;i<BSelf->NMemb;i++)
		{
			if(BSelf->Memb[i]==0xFFFF)
			{
				rez++;
			}
			else
			{
				if(!set)
				{
					OneObject* OB=Group[BSelf->Memb[i]];
					if(OB)
					{
						Type=OB->NIndex;
						set=true;
					}
				}
			}
		}
	}
	return rez;

}
int AutoForm::GetNUnitsToReman(word& Type)
{
	int rez=GetRealNUnitsToReman(Type);
	if(rez)
	{
		int n=GetTotalAmount0(&RemanUnits);
		rez=rez-n;
		if(rez<0)rez=0;
	}
	return rez;
}
void AutoForm::Reman()
{
	int n=GetTotalAmount0(&RemanUnits);
	if(n)
	{
		int xx, yy;
		if(GetGrpCenter(&RemanUnits,&xx,&yy))
		{
			int ds=Norma(xx-x,yy-y);
			if(ds>800)
			{
				byte al=BSelf->Direction+128;
				int xxx=x+((500*TCos[al])>>8);
				int yyy=y+((500*TSin[al])>>8);
				SGP_MoveToPoint(NI,&RemanUnits,xxx,yyy,BSelf->Direction,0,0,0);
			}
			else
			{
				word tt;
				if(GetRealNUnitsToReman(tt))
				{
					FillFormationFromGroup(NI,BSelf->ID,&RemanUnits);
					ClearGroup(&RemanUnits);
				}
			}
		}
	}
}
int AutoForm::GetUnitsAmount()
{
	int rez=0;
	if(BSelf&&IsAlive())
	{
		for(int i=0;i<BSelf->NMemb;i++)
		{
			if(BSelf->Memb[i]!=0xFFFF)
				rez++;
		}
	}
	return rez+GetTotalAmount0(&RemanUnits);
}
void AutoForm::DoNothing()
{
}
int AutoForm::GetLastCommTime()
{
	return (AnimTime-LastCommTime)/(25*256);
}
int AutoForm::GetDistToFriendlyForm()
{
	int rez=-1;
	for(int an=0;an<16;an++)
	{
		if(FriendAng[an].SectorsAmo)
		{
			if(rez==-1||rez>FriendAng[an].Sectors[0].dest)
				rez=FriendAng[an].Sectors[0].dest;
		}
	}
	return rez;
}
int AutoForm::GetNFormationsInMyPolk()
{
	int rez=0;
	if(RegimentIndex!=-1)
	{
		Regiment* Polk = BrigadeAI::Reg[RegimentIndex];
		if(Polk)
		{
			rez=Polk->MaxP;
		}
	}
	return rez;
}
int AutoForm::GetGameTime()
{
	return AnimTime/(25*256*10);
}
int AutoForm::GetFrontage()
{
	int rez=-1;
	if(MainTarget)
	{
		byte md = GetDir(MainTarget->x-x,MainTarget->y-y);
		byte mdl = md - 64;
		byte mdr = md + 64;
		int dist = Norma(MainTarget->x-x,MainTarget->y-y);
		if(dist>200&&dist<1500)
		{
			MotionField* MF=&MFIELDS[LockType];
			int wth = 1200;//GetSizeLeft()*3;// s zapasom
			//MF->CheckBar(-1,-1,3,3))
			int step=50;
			rez=wth;
			for(int ray=200;ray<dist;ray+=step)
			{
				int rx = x+((ray*TCos[md])>>8);
				int ry = y+((ray*TSin[md])>>8);
				int wl=0;
				int wr=0;
				if(!MF->CheckPt(rx>>4,ry>>4))
					wl++;
				for(int cross=step;cross<wth/2;cross+=step)
				{
					int rxl = rx+((cross*TCos[mdl])>>8);
					int ryl = ry+((cross*TSin[mdl])>>8);
					if(!MF->CheckPt(rxl>>4,ryl>>4))
						wl++;
					int rxr = rx+((cross*TCos[mdr])>>8);
					int ryr = ry+((cross*TSin[mdr])>>8);
					if(!MF->CheckPt(rxr>>4,ryr>>4))
						wr++;
				}
				int W = (wl+wr)*step; 
				if(W<rez)
					rez=W;
			}

		}
	}
	return rez;
}

int AutoForm::IsMTAI()
{
	if(MainTarget)
	{
		AutoForm* Tr = dynamic_cast<AutoForm*>(MainTarget);
		if(Tr)
			return 1;
	}
	return 0;
}
int AutoForm::GetProcDistToAimInPolk()
{
	int rez=-1;
	if(AimX!=-1&&RegimentIndex!=-1)
	{
		int avrDist=0;
		int nForm=0;
		for(int i=0;i<BrigadeAI::MaxAF;i++)
		{
			AutoForm *af=BrigadeAI::AF[i];
			if(af&&af->IsAlive()&&af->RegimentIndex==RegimentIndex)
			{
				if(af->AimX!=-1)
				{
					avrDist+=af->GetDistToAim();
					nForm++;
				}
			}
		}
		if(nForm)
		{
			avrDist/=nForm;
			rez=GetDistToAim()*100/avrDist;
		}
	}
	return rez;
}
void AutoForm::ArmAttack()
{
	if(MainTarget)
	{
		byte drr = GetDir(MainTarget->x-x,MainTarget->y-y);
		if(BSelf)
		{
			SetState(0,true);
			BSelf->AttEnm=true;
		}
		SimpleMoveTo(MainTarget->x,MainTarget->y,drr);
		
	}
}
int AutoForm::GetKartechEffect()
{
	return -1;
}
int AutoForm::GetMaxDensityInZerroSector()
{
	return -1;
}
int AutoForm::GetMaxDensityInSectors()
{
	return -1;
}
bool GetPushkaChargeState(OneObject* OB,int& ChargeType,int& ChargeStage);
int AutoForm::GetPushkaChargeState0()
{
	int rez=-1;
	if(C)
	{
		int ChargeType;
		int ChargeStage;
		if(GetPushkaChargeState(C,ChargeType,ChargeStage))
		{
			rez=0;
			if(ChargeType==0)
				rez=ChargeStage;
		}
	}
	return rez;
}
int AutoForm::GetPushkaChargeState1()
{
	int rez=-1;
	if(C)
	{
		int ChargeType;
		int ChargeStage;
		if(GetPushkaChargeState(C,ChargeType,ChargeStage))
		{
			rez=0;
			if(ChargeType==1)
				rez=ChargeStage;
		}
	}
	return rez;
}
int AutoForm::GetFillingAbility()
{
	bool TestFillingAbility(OneObject* OB);
	if(C&&TestFillingAbility(C)){		
		void FillObjectByUnits(OneObject* OB);
		FillObjectByUnits(C);
        return 1;
	}
	return 0;
}
CEXPORT void SetOrderedStateForComplexObject(OneObject* OB, int State);
void AutoForm::ChargeCannon0()
{
	if(C)
	{
		SetOrderedStateForComplexObject(C, 0);
	}
}
void AutoForm::ChargeCannon1()
{
	if(C)
	{
		SetOrderedStateForComplexObject(C, 1);
	}
}
void AutoForm::TurnCannonToMaxEnmFormationSector()
{
	if(C)
	{
		int Su[16];memset(Su,0,4*16);
		for(int an=0;an<16;an++)
		{
			if(EnmAng[an].SectorsAmo)
			{
				for(int i=0;i<EnmAng[an].SectorsAmo;i++)
				{
					if(EnmAng[an].Sectors[i].A)
					{
						int dx=EnmAng[an].Sectors[i].A->x;
						int dy=EnmAng[an].Sectors[i].A->y;
						char EnDir=char(GetDir(dx-x,dy-y));
						int enmd=((EnDir+8+512)>>4)&15;
						Su[enmd]+=EnmAng[an].Sectors[i].A->Force;
					}
				}
			}
		}
		int Md=-1;
		int mm=0;
		for(int i=0;i<16;i++)
		{
			if(Su[i]>mm)
			{
				mm=Su[i];
				Md=i;
			}
		}
		if(Md!=-1)
		{
			SimpleMoveTo(x,y,Md*16);
		}
	}
}
void AutoForm::FireOnEnmTower()
{
	if(C||BSelf)
	{
		int dst=999999;
		OneObject* Tar=NULL;
		for(int i=0;i<16;i++)
		{
			for(int j=0;j<EnmAng[i].SectorsAmo;j++)
			{
				Atom* A=EnmAng[i].Sectors[j].A;	
				if(A->Type==4&&A->IsAlive()&&EnmAng[i].Sectors[j].dest<dst&&A->Tower)
				{
					dst=EnmAng[i].Sectors[j].dest;
					Tar=A->Tower;
				}
			}			
		}
		if(C&&Tar)
		{
			C->AttackObj(Tar->Index,128+16,0,1);
		}
		else
		if(BSelf)
		{
			for(int i=0;i<BSelf->NMemb;i++)
			{
				if(BSelf->Memb[i]!=0xFFFF)
				{
					OneObject* BrOB = Group[BSelf->Memb[i]];
					if(BrOB)
					{
						BrOB->AttackObj(Tar->Index,128+16);
					}
				}
			}
		}
	}
}
void AutoForm::FireOnEnmBuilding()
{
	if(C)
	{
		OneObject* B = GetNearestEnmBuilding();
		if(B)
		{
			C->AttackObj(B->Index,128+16,1);
		}
	}
	else
	if(BSelf)
	{
		OneObject* B = GetNearestEnmBuilding();
		if(B)
		{
			for(int i=0;i<BSelf->NMemb;i++)
			{
				if(BSelf->Memb[i]!=0xFFFF)
				{
					OneObject* BrOB = Group[BSelf->Memb[i]];
					if(BrOB)
					{
						BrOB->AttackObj(B->Index,128+16);
					}
				}
			}
		}
	}
}
void AutoForm::Ram()
{
	if(C&&MainTarget&&MainTarget->C)
	{
		if(C->Usage==LinkorID&&MainTarget->C->Usage==LinkorID)
		{
			int sx=C->RealX/16;
			int sy=C->RealY/16;
			int tx=MainTarget->C->RealX/16;
			int ty=MainTarget->C->RealY/16;
			int Dst=Norma(sx-tx,sy-ty);
			if(Dst>100)
			{
				byte dr=GetDir(sx-tx,sy-ty);
				int xx=tx+((90*TCos[dr])>>8);
				int yy=ty+((90*TSin[dr])>>8);
				SimpleMoveTo(xx,yy);
			}
		}
	}
}
int AutoForm::GetMTAngl()
{
	int rez=-1;
	if(MainTarget)
	{
		byte rr=0;
		byte dr=GetDir(x-MainTarget->x,y-MainTarget->y);
		if(MainTarget->BSelf)
			rr=MainTarget->BSelf->Direction-dr;
		if(MainTarget->C)
			rr=MainTarget->C->RealDir-dr;
		rez=rr;
	}
	return rez;
}
int AutoForm::GetMaxHightInMTDirection()
{
	if(MainTarget)
	{
		int myh = GetHeight(x,y);
		int mth = GetHeight(MainTarget->x,MainTarget->y);
		byte Dr=GetDir(MainTarget->x-x,MainTarget->y-y);
		int Dst=Norma(MainTarget->x-x,MainTarget->y-y);
		int ChDst=0;
		int Step=150;
		int MaxH=0;
		int DeltaH=mth-myh;
		if(Dst>0)
		{
			while(ChDst<(Dst-Step))
			{
				ChDst+=Step;
				int xxx=x+((ChDst*TCos[Dr])>>8);
				int yyy=y+((ChDst*TSin[Dr])>>8);
				int Height=GetHeight(xxx,yyy);
				int LineH=myh+DeltaH*ChDst/Dst;
				int H=Height-LineH;
				if(ChDst<301)
				{
					H+=H*(300-ChDst)/200;
				}
				else
				{
					int Le=Dst-ChDst;
					if(Le>0&&Le<301)
					{
						H+=H*(300-Le)/200;
					}
				}
				if(H>MaxH)
				{
					MaxH=H;
				}
			}
		}
		return MaxH;
	}
	return -1;
}
int AutoForm::GetNApparentFriendlyForm()
{
	int rez=0;
	for(int i=0;i<16;i++)
	{
		for(int j=0;j<FriendAng[i].SectorsAmo;j++)
		{
			Atom* A=(Atom*)FriendAng[i].Sectors[j].AF;
			if(!A)
				A=FriendAng[i].Sectors[j].A;
			if(A&&A->BSelf&&A->IsAlive()&&A->NI!=7&&FriendAng[i].Sectors[j].dest<1300)
			{
				rez++;
			}
		}			
	}
	return rez;
}
int AutoForm::GetNApparentEnemyForm()
{
	int rez=0;
	for(int i=0;i<16;i++)
	{
		for(int j=0;j<EnmAng[i].SectorsAmo;j++)
		{
			Atom* A=EnmAng[i].Sectors[j].A;	
			if(A&&A->BSelf&&A->IsAlive()&&A->NI!=7&&EnmAng[i].Sectors[j].dest<1300)
			{
				rez++;
			}
		}			
	}
	return rez;
}
int AutoForm::CanSetBomb()
{
	if(BSelf)
	{
		itr_GetNotCommandUnitsOfBrigade.Create(BSelf);
		OneObject* Un;
		if(Un=itr_GetNotCommandUnitsOfBrigade.Next())
		{
			int NBuild=Un->Nat->PACount[Un->NIndex];
			if(NBuild>0)
			{
				word* Build=Un->Nat->PAble[Un->NIndex];
				for(int i=0;i<NBuild;i++)
				{
					NewMonster* nm=Un->Nat->Mon[Build[i]]->newMons;
					if(nm->MineDamage&&nm->BuildNearBuildingRadius)
					{
						return 1;
					}
				}
			}
		}
	}
	return 0;
}
void AutoForm::SetBomb()
{
	if(BSelf)
	{
		itr_GetNotCommandUnitsOfBrigade.Create(BSelf);
		if(OneObject* Un=itr_GetNotCommandUnitsOfBrigade.Next())
		{
			int NBuild=Un->Nat->PACount[Un->NIndex];
			if(NBuild>0)
			{
				word* Build=Un->Nat->PAble[Un->NIndex];
				for(int i=0;i<NBuild;i++)
				{
					NewMonster* nm=Un->Nat->Mon[Build[i]]->newMons;
					if(nm->MineDamage&&nm->BuildNearBuildingRadius)
					{
						if(OneObject* B=GetNearestEnmBuilding())
						{
							int r=GetDistanceToBuilding(B,x,y);
							if(r>0&&r<1000)
							{
								//
								int dst=Norma(x-B->RealX/16,y-B->RealY/16);
								byte drr=GetDir(x-B->RealX/16,y-B->RealY/16);
								int xx=B->RealX/16+(((dst-r+10)*TCos[drr])>>8);
								int yy=B->RealY/16+(((dst-r+10)*TSin[drr])>>8);;
								int newb=Un->Nat->CreateNewMonsterAt(xx*16,yy*16,Build[i],true);
								if(newb>-1)
								{
									int nb=0;
									for(int i=0;i<BSelf->NMemb;i++)
									{
										word MID=BSelf->Memb[i];
										if(MID!=0xFFFF&&nb<5)
										{
											OneObject* OB=Group[MID];
											if(OB&&!(OB->UnlimitedMotion||OB->LockedInBuilding!=0xFFFF))
											{
												if(!(OB->LocalOrder&&OB->LocalOrder->DoLink==&BuildObjLink))
												{
													Order1* OR1=OB->LocalOrder;
													bool BLD=0;
													while(OR1&&!BLD)
													{
														BLD=OR1->DoLink==&BuildObjLink;
														OR1=OR1->NextOrder;
													};
													if(!BLD)
													{
														OB->BuildObj(newb,128+16,0,1);
														nb++;
													}
													OB->ClearBuildPt();
												};
											};
										};
									};

								}
							}
						}
						return;
					}
				}
			}
		}
	}
}
int AutoForm::GetDiff()
{
	return CITY[NI].Difficulty;
}

void AutoForm::ClearOrders()
{
	if(BSelf)
	{
		BSelf->ClearNewBOrders();
		for(int i=0;i<BSelf->NMemb;i++)
		{
			word ID=BSelf->Memb[i];
			if(ID!=0xFFFF)
			{
				OneObject* OB=Group[ID];
				if(OB&&OB->Serial==BSelf->MembSN[i])
				{
					OB->ClearOrders();
					BSelf->posX[i]=OB->RealX/16;
					BSelf->posY[i]=OB->RealY/16;
					OB->EnemyID=0xFFFF;
					OB->EnemySN=0xFFFF;
				}
			}
		}
	}
}
void AutoForm::Save(SaveBuf *SB)
{
	xBlockWrite(SB,&Type,4);
	//
	if(Type>0)
	{
		xBlockWrite(SB,&Index,4);
		xBlockWrite(SB,&RegimentIndex,4);
		xBlockWrite(SB,&Type,4);
		xBlockWrite(SB,&NextProcesedTime,4);
		xBlockWrite(SB,&TimeInterval,4);
		xBlockWrite(SB,&FormationTypeIndex,4);
		xBlockWrite(SB,&LastTimeCheckXY,4);
		xBlockWrite(SB,&AimX,4);
		xBlockWrite(SB,&AimY,4);
		xBlockWrite(SB,&AimDir,1);
		xBlockWrite(SB,&mtX,4);
		xBlockWrite(SB,&mtY,4);
		xBlockWrite(SB,&mtDir,1);
		xBlockWrite(SB,&mtXr,4);
		xBlockWrite(SB,&mtYr,4);
		xBlockWrite(SB,&mtXl,4);
		xBlockWrite(SB,&mtYl,4);
		xBlockWrite(SB,StateArr,32);
		xBlockWrite(SB,&DistToMainTarget,4);
		xBlockWrite(SB,&RulesID,4);
		
		xBlockWrite(SB,&NI,1);
		xBlockWrite(SB,&BSelf->ID,2);
		xBlockWrite(SB,&CheckTime,4);
		xBlockWrite(SB,&Force,4);
	}
}
int AutoForm::Load(SaveBuf *SB)
{
	xBlockRead(SB,&Index,4);
	xBlockRead(SB,&RegimentIndex,4);
	xBlockRead(SB,&Type,4);
	xBlockRead(SB,&NextProcesedTime,4);
	xBlockRead(SB,&TimeInterval,4);
	xBlockRead(SB,&FormationTypeIndex,4);
	xBlockRead(SB,&LastTimeCheckXY,4);
	xBlockRead(SB,&AimX,4);
	xBlockRead(SB,&AimY,4);
	xBlockRead(SB,&AimDir,1);
	xBlockRead(SB,&mtX,4);
	xBlockRead(SB,&mtY,4);
	xBlockRead(SB,&mtDir,1);
	xBlockRead(SB,&mtXr,4);
	xBlockRead(SB,&mtYr,4);
	xBlockRead(SB,&mtXl,4);
	xBlockRead(SB,&mtYl,4);
	xBlockRead(SB,StateArr,32);
	xBlockRead(SB,&DistToMainTarget,4);
	xBlockRead(SB,&RulesID,4);
	
	xBlockRead(SB,&NI,1);
	word BSelfID;
	xBlockRead(SB,&BSelfID,2);
	xBlockRead(SB,&CheckTime,4);
	xBlockRead(SB,&Force,4);
	
	BSelf=&(CITY[NI].Brigs[BSelfID]);
	CheckTime=-1;
	UpdateParams();
	return Index;
}
//====================================================================//
//====================================================================//
Cannon::Cannon()
{}
Cannon::~Cannon()
{
	Type=0;
	C=NULL;
}
Cannon::Cannon(int Index)
{
	//OBJ->newMons->Usage!=PushkaID
	CannonID=Index;
	C=Group[Index];
	Type=3;

	RegimentIndex=-1;
	NextProcesedTime=0;
	NextTimeProcessRules=0;
	
	Force=150;

	BSelf=NULL;

	NI=C->NNUM;

	AimX=-1;
	AimY=-1;
	
	UpdateParams();
	for(int i=0;i<16;i++)
	{
		EnmAng[i].SumDang=0;
		EnmAng[i].Dang=0;
		EnmAng[i].Dir=i<<4;
		EnmAng[i].SectorsAmo=0;
		
		FriendAng[i].SumDang=0;
		FriendAng[i].Dang=0;
		FriendAng[i].Dir=i<<4;
		FriendAng[i].SectorsAmo=0;
	}
	
	DistToMainTarget=-1;
	Movement=false;
	mtXr=-1;
	mtYr=-1;
	mtXl=-1;
	mtYl=-1;
	MainTarget=NULL;
	LockType=C->LockType;
	CheckTimeInterval=10;
	StandTime=0;
}
/*
void Cannon::Process()
{
	if(tmtmt>NextProcesedTime)
	{
		//bool GetPushkaChargeState(OneObject* OB,int& ChargeType,int& ChargeStage)
		//C->NewAttackPoint();
		//void FillObjectByUnitsLink(OneObject* OBJ)
		//bool TestFillingAbility(OneObject* OB);
		//void RotUnit(OneObject* OB,char Dir,byte OrdType)
		//CEXPORT void SetOrderedStateForComplexObject(OneObject* OB, int State)
		
		TimeInterval=15;
		IsStand=false;
		if(C&&((C->NewAnm==&(C->newMons->Stand))||(C->NewAnm==&(C->newMons->PStand[4]))||(C->NewAnm==&(C->newMons->PStand[5]))))
			IsStand=true;
		if(IsStand&&GetFireReady())
		{
			int xx,yy;
			if(FindBatterCoordInSector(0,xx,yy))
			{
				SetBatterCoord(xx,yy);
				Fire();
			}
		}
		//if(!(rand()%5))
			//int dd=GetKartechEffect();
		NextProcesedTime=tmtmt+TimeInterval;
	}
}
*/
/*
void Cannon::UpdateParams()
{
	if(CheckTime!=tmtmt&&tmtmt>(CheckTime+CheckTimeInterval))
	{
		C=Group[CannonID];
		if(C&&Type==3&&!C->Sdoxlo)
		{
			x=C->RealX>>4;
			y=C->RealY>>4;
			Dir=C->RealDir;
			FireTimeDelayLeft=C->delay;
		}
		CheckTime=tmtmt;
	}
}
*/
int Cannon::FindBatterCoordInSector(int SectorN, int& xx, int& yy)
{
	int rez=0;
	if(SectorN>-1&&SectorN<16)
	{
		rez = FindCoordForCannon(C,xx,yy,C->RealDir+SectorN*16);
	}
	return rez;
}
void Cannon::SetBatterCoord(int x, int y)
{
	FireToX=x;
	FireToY=y;
	ShotType=4;
	//C->NewCurSprite==1
}
void Cannon::PreKartech()
{
	if(C&&C->newMons->WSECT)
		C->NewState=C->newMons->WSECT->AttIndex+1;
	ShotType=5;
}
void Cannon::Fire()
{
	if (C) 
	{
		if(C->Usage==LinkorID&&MainTarget)
		{
			if(MainTarget->C)
				C->AttackObj(MainTarget->C->Index,128+16);
			if(MainTarget->BSelf)
			{
				int cen=MainTarget->BSelf->NMemb/2;
				word tar=0xFFFF;
				for(int i=0;i<cen;i++)
				{
					word u1=MainTarget->BSelf->Memb[cen+i];
					word u2=MainTarget->BSelf->Memb[cen-i];
					if(u1!=0xFFFF)
					{
						OneObject* OB=Group[u1];
						if(OB&&!OB->Sdoxlo)
						{
							tar=OB->Index;
							break;
						}
					}
					if(u2!=0xFFFF)
					{
						OneObject* OB=Group[u2];
						if(OB&&!OB->Sdoxlo)
						{
							tar=OB->Index;
							break;
						}
					}
				}
				if(tar!=0xFFFF)
					C->AttackObj(tar,128+16);
			}
		}
		else
		{
			int ChargeType,ChargeStage;
			if(GetPushkaChargeState(C,ChargeType,ChargeStage))
			{
				if(ChargeType==0)
				{
					if(GetMaxDensityInZerroSector())
					{
						C->NewAttackPoint(FireToX,FireToY,128+16,0,0);
					}
				}
				if(ChargeType==1)
				{
					C->NewAttackPoint((C->RealX>>4)+TCos[C->RealDir],(C->RealY>>4)+TSin[C->RealDir],128+16,0,1);
				}
			}
		}
	}
}
void Cannon::SplashFire()
{
	if(C&&C->delay==0)
	{
		AdvCharacter* ADC=C->MoreCharacter;
		int ChargeType=0;
		int Step=64;
		int MinDist=ADC->AttackRadius1[ChargeType];
		int MaxDist=ADC->AttackRadius2[ChargeType];

		int x1=(x-MaxDist)/128;
		int y1=(y-MaxDist)/128;
		int x2=(x+MaxDist)/128+1;
		int y2=(y+MaxDist)/128+1;
		int xs=x2-x1;
		int ys=y2-y1;
		int siz=ys*xs;
		int* Map = new int[siz];
		memset(Map,0,sizeof(int)*siz);
		for(int i=x1;i<x2;i++)
		{
			for(int j=y1;j<y2;j++)
			{
				if(i>=0&&j>=0&&i<VAL_MAXCX&&j<VAL_MAXCX)
				{
					int xx=i*128+64;
					int yy=j*128+64;
					int dst=Norma(xx-x,yy-y);
					if(dst<=MaxDist&&dst>=MinDist)
					{
						int cell=i+(j<<VAL_SHFCX);
						int NMon=MCount[cell];
						if(NMon)
						{
							int ofs1=cell<<SHFCELL;
							word MID;
							int W=0;
							for(int k=0;k<NMon;k++)
							{
								MID=GetNMSL(ofs1+k);
								if(MID!=0xFFFF)
								{
									OneObject* OB=Group[MID];
									if(OB)
									{
										if(OB->NMask&C->NMask)
										{
											W-=OB->Life*3;
										}
										else
										{
											W+=OB->Life;
										}
									}
								}
							}
							Map[(i-x1)+xs*(j-y1)]=W;
						}
					}
				}
			}
		}
		int Max=0;
		int Mx=-1;
		int My=-1;
		for(int i=x1+1;i<(x2-1);i++)
		{
			for(int j=y1+1;j<(y2-1);j++)
			{
				if(i>=0&&j>=0&&i<VAL_MAXCX&&j<VAL_MAXCX)
				{
					int W=Map[(i-x1)+xs*(j-y1)];
					W+=Map[(i-x1)+xs*(j-y1+1)];
					W+=Map[(i-x1)+xs*(j-y1-1)];
					W+=Map[(i-x1-1)+xs*(j-y1)];
					W+=Map[(i-x1-1)+xs*(j-y1-1)];
					W+=Map[(i-x1-1)+xs*(j-y1+1)];
					W+=Map[(i-x1+1)+xs*(j-y1)];
					W+=Map[(i-x1+1)+xs*(j-y1-1)];
					W+=Map[(i-x1+1)+xs*(j-y1+1)];
					if(W>Max)
					{
						Max=W;
						Mx=i;
						My=j;
					}
				}
			}
		}
		delete[] Map;
		if(Max)
		{
			int cell=Mx+(My<<VAL_SHFCX);
			int NMon=MCount[cell];
			if(NMon)
			{
				int ofs1=cell<<SHFCELL;
				word MID;
				int W=0;
				for(int k=0;k<NMon;k++)
				{
					MID=GetNMSL(ofs1+k);
					if(MID!=0xFFFF)
					{
						OneObject* OB=Group[MID];
						if(OB)
						{
							if(!(OB->NMask&C->NMask))
							{
								C->AttackObj(MID,128+16,0,1);
								break;
							}
						}
					}
				}
			}
		}
	}
}
int Cannon::GetKartechEffect()
{
	int rez=0;
	//200
	int angl=60;//(255)
	int MaxDist=1000;
	//20
	NnFr=0;
	NnEn=0;
	KsFr=0;
	KsEn=0;
	if(C)
	{
		WeaponInSector WS;
		WS.Angle=64;
		WS.AttIndex=5;
		WS.AnglFactor=0;
		WS.MaxDamage=0;
		WS.MinDamage=0;
		WS.RMax=2500;
		WS.RMin=1;
		ActInSector(x,y,&WS,C->RealDir,&Cannon::CheckUnitsInAngle,int(this));
		/*
		if(NnEn)
		{
			rez=KsEn/NnEn;
			if(NnFr)
				rez-=KsFr/NnFr;
		}
		*/
	}
	return KsEn-KsFr;
}
int Cannon::GetMaxDensityInZerroSector()
{
	int rez=0;
	int xx, yy;
	rez=FindBatterCoordInSector(0, xx, yy);
	if(rez)
	{
		FireToX=xx;
		FireToY=yy;
	}
	return rez;

}
int Cannon::GetMaxDensityInSectors()
{
	int rez=0;
	int xx, yy;
	for (int i=0;i<16;i++)
	{
		int den=FindBatterCoordInSector(i, xx, yy);
		if(den>rez)
		{
			rez=den;
			FireToX=xx;
			FireToY=yy;
		}
	}
	return rez;
}
void Cannon::MoveTo(int xx, int yy, byte dir)
{
	SimpleMoveTo(xx,yy,dir);
}
void Cannon::SimpleMoveTo(int xx, int yy, byte dir)
{
	if(EngSettings.AutoChangeFormationType==true){
		C->ActivityState=0;
	}
	int dss=Norma(x-xx,y-yy);
	if(dss<100)
	{
		SetDestDirForComplexObject(C,dir);
		StandTime=0;
		//RotateMon(C,dir-C->RealDir);
	}
	if(dss<800)
	{
		C->NewMonsterSendTo(xx<<4,yy<<4,128+16,0);
	}
	else
	{
		C->NewMonsterSmartSendTo(xx,yy,0,0,128+16,0);
	}
}
void Cannon::CheckUnitsInAngle(OneObject* OB,WeaponInSector* WS,byte Angle,int param)
{
	Cannon* c=(Cannon*)param;
	if(c->C!=OB)
	{
		int dH=c->C->RZ-OB->RZ;
		int Kmin=50;
		int Kmax=200;
		int R=Norma(c->x-(OB->RealX>>4),c->y-(OB->RealY>>4));
		int dHmin=(Kmin*R*R)/1000000+40;
		//int dHmax=(Kmax*R*R)/1000000+20;
		if(dHmin>dH)
		{
			if(OB->NMask&c->C->NMask)
			{
				NnFr++;
				KsFr+=10000/(R+1);
			}
			else 
			{
				NnEn++;
				KsEn+=10000/(R+1);
			}
		}
	}
}
int Cannon::GetNUnitsToReman(int Type)
{
	return 0;
}
int Cannon::GetUnitsAmount()
{
	if(IsAlive())
		return 1;
	return 0;
}
void Cannon::ShowInfo()
{
	char st[128];
	
	if(Type==3)
	{
		//sprintf(st,"Cannon ID(%d)",CannonID);
		if(GetState(1))
			sprintf(st,"Cannon ID:%d Reserv,AT(%d):TI(%d),LastCom:%d",C->Index,GetState(0),TimeInterval,LastCom);
		else
			if(GetState(4))
				sprintf(st,"Cannon ID:%d Front,AT(%d):TI(%d),LastCom:%d",C->Index,GetState(0),TimeInterval,LastCom);
			else
				sprintf(st,"Cannon ID:%d Gluck,TI:%d,LastCom:%d",C->Index,TimeInterval,LastCom);
		ShowString(x-(mapx<<5),y/2-GetHeight(x,y)-(mapy<<4),st,&YellowFont);
		if(MainTarget)
			xLine(x-(mapx<<5),y/2-GetHeight(x,y)-(mapy<<4),MainTarget->x-(mapx<<5),MainTarget->y/2-GetHeight(MainTarget->x,MainTarget->y)-(mapy<<4),0xFF);
		if(mtXr!=-1)
		{
			ShowString(mtXr-(mapx<<5),mtYr/2-GetHeight(mtXr,mtYr)-(mapy<<4),"R",&YellowFont);
			ShowString(mtXl-(mapx<<5),mtYl/2-GetHeight(mtXl,mtYl)-(mapy<<4),"L",&YellowFont);
			xLine(mtXr-(mapx<<5),mtYr/2-GetHeight(mtXr,mtYr)-(mapy<<4),mtXl-(mapx<<5),mtYl/2-GetHeight(mtXl,mtYl)-(mapy<<4),0xF0);
		}
		if(FireToX&&FireToY)
		{
			int x1,x2,x3,x4;
			int y1,y2,y3,y4;
			x1=x2=FireToX;
			y1=FireToY-50;
			y2=FireToY+50;
			y3=y4=FireToY;
			x3=FireToX-50;
			x4=FireToX+50;
			xLine(x1-(mapx<<5),y1/2-GetHeight(x1,y1)-(mapy<<4),x2-(mapx<<5),y2/2-GetHeight(x2,y2)-(mapy<<4),0xF0);
			xLine(x3-(mapx<<5),y3/2-GetHeight(x3,y3)-(mapy<<4),x4-(mapx<<5),y2/2-GetHeight(x4,y4)-(mapy<<4),0xF0);
			byte DR=GetDir(FireToX-x,FireToY-y);
			char st2[128];
			sprintf(st2,"%d-%d=%d",C->RealDir,DR,C->RealDir-DR);
			ShowString(x2-(mapx<<5),y2/2-GetHeight(x2,y2)-(mapy<<4),st2,&YellowFont);
		}
	}
}
void Cannon::Save(SaveBuf *SB)
{
	xBlockWrite(SB,&Type,4);
	//
	if(Type>0)
	{
		xBlockWrite(SB,&Index,4);
		xBlockWrite(SB,&RegimentIndex,4);
		xBlockWrite(SB,&Type,4);
		xBlockWrite(SB,&NextProcesedTime,4);
		xBlockWrite(SB,&TimeInterval,4);
		xBlockWrite(SB,&FormationTypeIndex,4);
		xBlockWrite(SB,&LastTimeCheckXY,4);
		xBlockWrite(SB,&AimX,4);
		xBlockWrite(SB,&AimY,4);
		xBlockWrite(SB,&AimDir,1);
		xBlockWrite(SB,&mtX,4);
		xBlockWrite(SB,&mtY,4);
		xBlockWrite(SB,&mtDir,1);
		xBlockWrite(SB,&mtXr,4);
		xBlockWrite(SB,&mtYr,4);
		xBlockWrite(SB,&mtXl,4);
		xBlockWrite(SB,&mtYl,4);
		xBlockWrite(SB,StateArr,32);
		xBlockWrite(SB,&DistToMainTarget,4);
		xBlockWrite(SB,&RulesID,4);
		
		xBlockWrite(SB,&NI,1);
		xBlockWrite(SB,&CannonID,4);
		xBlockWrite(SB,&CheckTime,4);
		xBlockWrite(SB,&Force,4);
	}
}
int Cannon::Load(SaveBuf *SB)
{
	xBlockRead(SB,&Index,4);
	xBlockRead(SB,&RegimentIndex,4);
	xBlockRead(SB,&Type,4);
	xBlockRead(SB,&NextProcesedTime,4);
	xBlockRead(SB,&TimeInterval,4);
	xBlockRead(SB,&FormationTypeIndex,4);
	xBlockRead(SB,&LastTimeCheckXY,4);
	xBlockRead(SB,&AimX,4);
	xBlockRead(SB,&AimY,4);
	xBlockRead(SB,&AimDir,1);
	xBlockRead(SB,&mtX,4);
	xBlockRead(SB,&mtY,4);
	xBlockRead(SB,&mtDir,1);
	xBlockRead(SB,&mtXr,4);
	xBlockRead(SB,&mtYr,4);
	xBlockRead(SB,&mtXl,4);
	xBlockRead(SB,&mtYl,4);
	xBlockRead(SB,StateArr,32);
	xBlockRead(SB,&DistToMainTarget,4);
	xBlockRead(SB,&RulesID,4);
	
	xBlockRead(SB,&NI,1);
	xBlockRead(SB,&CannonID,4);
	xBlockRead(SB,&CheckTime,4);
	xBlockRead(SB,&Force,4);

	C=Group[CannonID];
	CheckTime=-1;
	UpdateParams();
	return Index;
}
int Cannon::NnFr=0;
int Cannon::NnEn=0;
int Cannon::KsFr=0;
int Cannon::KsEn=0;
//====================================================================//
//====================================================================//
AutoForm* BrigadeAI::AF[512];
int BrigadeAI::MaxAF=0;
Regiment* BrigadeAI::Reg[128];
int BrigadeAI::MaxReg=0;
//DynArray<Rules*> BrigadeAI::AllRules;
//Atom* BrigadeAI::AllForm[8][256];
//int BrigadeAI::AllFormAmo[8];

DynArray<Atom*> BrigadeAI::AllForm;
DynArray<Atom*> BrigadeAI::AllFormGarbage;

int BrigadeAI::AllFormFillTime=0;
tpBirthCallback* BrigadeAI::lpBirthCallback=NULL;
AreaInfo** BrigadeAI::AreaMap=NULL;
int BrigadeAI::NA=0;
bool BrigadeAI::FirstStart=true;
DWORD BrigadeAI::LastSerial=0;

BrigadeAI::BrigadeAI()
{
	memset(AF,0,512*4);
	memset(Reg,0,128*4);

	FirstStart=true;
}
BrigadeAI::~BrigadeAI()
{
	/*for(int i=0;i<MaxAF;i++)
		if(AF[i]!=NULL)
			delete (AF[i]);
	*/

	int nnn=AllForm.GetAmount();
	for(int i=0;i<nnn;i++)
		if(AllForm[i])
		{
			delete (AllForm[i]);
			AllForm[i]=NULL;
		}
	AllForm.Clear();

	nnn=AllFormGarbage.GetAmount();
	for(int i=0;i<nnn;i++)
		if(AllFormGarbage[i])
		{
			delete (AllFormGarbage[i]);
			AllFormGarbage[i]=NULL;
		}
	AllFormGarbage.Clear();

	for(i=0;i<MaxReg;i++)
		if(Reg[i])
		{
			delete (Reg[i]);
			Reg[i]=NULL;
		}
	MaxReg=0;
	int nr=AllRules.GetAmount();
	for(int i=0;i<nr;i++)
	{
		if(AllRules[i])
		{
			delete AllRules[i];
			AllRules[i]=NULL;
		}
	}
	AllRules.Clear();
}
int BrigadeAI::AddAutoBrigade(Brigade* B)
{
	int rez=-1;
	for(int i=0;i<MaxAF;i++)
		if((AF[i])&&AF[i]->BSelf==B)
			return AF[i]->Index;
	for(i=0;i<(MaxAF+1);i++)
	{
		if(AF[i]==NULL||i==MaxAF)
		{
			AF[i] = new AutoForm(B);
			AF[i]->Index=i;
			AF[i]->BSelf->AutoFormID=i;
			rez=i;
			/*
			OneObject* OB=Group[B->Memb[4]];
			if(OB)
			{
				if(OB->MaxLife<101)
					AF[i]->RulesID=0;//Shuter
				else
					AF[i]->RulesID=1;//Horce
				if(OB->Usage==MentID)
					AF[i]->RulesID=3;//Ment
				if(OB->newMons->NShotRes==0)
				{
					AF[i]->RulesID=4;//Pehota
					AF[i]->Type=4;
				}
			}
			*/
			word tt=GetBrigadeNIndex(B);
			word RID=GetRulesIndex(tt);
			if(RID!=0xFFFF)
			{
				AF[i]->RulesID=RID;
				AF[i]->RulesList=AllRules[AF[i]->RulesID];
				//AF[i]->Type=AllRules[AF[i]->RulesID].Index;
				//int NI=AF[i]->NI;
				int nnn=AllForm.GetAmount();
				for(int j=0;j<nnn;j++)
				{
					if(AllForm[j]&&AllForm[j]->BSelf&&(AllForm[j]->BSelf->ID==AF[i]->BSelf->ID
						&&AllForm[j]->BSelf->CT->NI==AF[i]->BSelf->CT->NI))
					{
						AllForm[j]->Type=0;
						AllForm[j]->BSelf=NULL;
						AllForm[j]->C=NULL;
						AF[i]->Serial=AllForm[j]->Serial;
						AF[i]->AtomIndex=j;
						AF[i]->Index=i;
						//delete(AllForm[j]);
						AllFormGarbage.Add(AllForm[j]);
						AllForm[j]=(Atom*)AF[i];						
						rez=AF[i]->Index;
						if(i==MaxAF)MaxAF=i+1;
						return rez;
					}
				}
			}
			if(rez==-1)
			{
				AllForm.Add((Atom*)AF[i]);
				rez=AF[i]->Index;
			}
			if(i==MaxAF)MaxAF=i+1;
			return rez;
		}
	}
	return rez;
}
int BrigadeAI::AddAutoCannon(OneObject *Ob)
{
	int rez=-1;
	if(Ob->newMons->Usage==PushkaID||Ob->newMons->Usage==LinkorID)
	{
		for(int i=0;i<MaxAF;i++)
			if((AF[i])&&AF[i]->Type==3)
			{
				if(AF[i]->C==Ob)
					return AF[i]->Index;
			}
		for(i=0;i<(MaxAF+1);i++)
		{
			if(AF[i]==NULL)
			{
				AF[i] = new Cannon(Ob->Index);
				AF[i]->Index=i;
				rez=i;
				word RID=GetRulesIndex(Ob->NIndex);
				if(RID!=0xFFFF)
				{
					AF[i]->RulesID=RID;
					AF[i]->RulesList=AllRules[AF[i]->RulesID];
					AF[i]->Type=3;//AllRules[AF[i]->RulesID].Index;
					//int NI=AF[i]->NI;
					int nnn=AllForm.GetAmount();
					for(int j=0;j<nnn;j++)
					{
						if(AllForm[j]&&AllForm[j]->Type==3&&(AllForm[j]->CannonID==Ob->Index))
						{
							AllForm[j]->Type=0;
							AllForm[j]->BSelf=NULL;
							AllForm[j]->C=NULL;
							AF[i]->Serial=AllForm[j]->Serial;
							AF[i]->AtomIndex=j;
							//delete (AllForm[j]);
							AllFormGarbage.Add(AllForm[j]);
							AllForm[j]=(Atom*)AF[i];
							rez=AF[i]->Index;
							if(i==MaxAF)MaxAF=i+1;
							return rez;
						}
					}
					if(rez==-1)
					{
						AllForm.Add((Atom*)AF[i]);
						rez=AF[i]->Index;
					}
					if(i==MaxAF)MaxAF=i+1;
					return rez;
				}
			}
		}
		if(i==MaxAF)MaxAF=i+1;
	}
	return rez;
}
void CheckMemoryIntegrity();
void BrigadeAI::Init()
{
	if(AF)memset(AF,0,512*4);
	if(Reg)memset(Reg,0,128*4);
	MaxAF=0;
	MaxReg=0;
	FillNeighborsTime=0;
	AllFormFillTime=0;
	//AllFormAmo[0]=0;
	//AllFormAmo[1]=0;
	//AllFormAmo[2]=0;
	//AllFormAmo[3]=0;
	//AllFormAmo[4]=0;
	//AllFormAmo[5]=0;
	//AllFormAmo[6]=0;
	//AllFormAmo[7]=0;
	//CreateAreaMap();
	int nr=AllRules.GetAmount();
	for(int i=0;i<nr;i++)
	{
		if(AllRules[i])
		{
			delete AllRules[i];
			AllRules[i]=NULL;
		}
	}
	AllRules.Clear();
	if(MOptions.Players.Player[0].DisableNationSelect)
		LoadAllRules("AI/BrigadeAI/Rules.ai","Rules.ai");
	else 
		LoadAllRules("AI/BrigadeAI/BattleRules.ai","BattleRules.ai");
	LoadAddRules("sia");
	lpBirthCallback=SetBirthCallback(&BrigadeAI::BirthCallback);
	int nnn=AllForm.GetAmount();
	for(int i=0;i<nnn;i++)
		if(AllForm[i])
			delete(AllForm[i]);
	AllForm.Clear();
	nnn=AllFormGarbage.GetAmount();
	for(int i=0;i<nnn;i++)
		if(AllFormGarbage[i])
			delete(AllFormGarbage[i]);
	AllFormGarbage.Clear();
}
void CheckMemoryIntegrity();
void KeyTestMem();
void BrigadeAI::Process()
{
	int  i;
	if(FirstStart)
	{
		Init();
		FirstStart=false;
		FillAllForm();
	}
	for(i=0;i<MaxAF;i++)
	{
		if(AF[i]!=NULL)
		{
			if(AF[i]->Type&&(AF[i]->BSelf||AF[i]->C||AF[i]->Tower)&&AF[i]->IsAlive())
				;
			else
			{
				AF[i]->Type=0;
				AF[i]=NULL;
			}
		}
	}
	if(tmtmt>FillNeighborsTime)
	{
		int nnn=AllForm.GetAmount();
		for(int j=0;j<nnn;j++)
		{	
			if(AllForm[j])
			{
				if(AllForm[j]->IsAlive())
					AllForm[j]->UpdateParams();
				else
				{
					//delete(AllForm[j]);
					AllFormGarbage.Add(AllForm[j]);
					AllForm[j]=NULL;
					//AllForm.Del(j,1);
					//nnn--;
				}
			}
		}
		//FillNeighbors();
		FillEnmAng();
		FillFriendAng();
		FillNeighborsTime=tmtmt+2+MAXOBJECT/1000;
	}
	for(i=0;i<MaxReg;i++)
	{
		if(Reg[i]!=NULL)
		{
			Reg[i]->Process();
		}
	}
	for(i=0;i<MaxAF;i++)
	{
		if(AF[i]!=NULL)
		{
			if(AF[i]->Type&&(AF[i]->BSelf||AF[i]->C||AF[i]->Tower)&&AF[i]->IsAlive())
				AF[i]->Process();
			else
			{
				AF[i]->Type=0;
				AF[i]=NULL;
			}
		}
	}
}

void BrigadeAI::FillEnmAng()
{
	for(int i=0;i<MaxAF;i++)
	{
		AutoForm* Fo=AF[i];
		if (Fo&&Fo->IsAlive()) 
		{
			Fo->ClearEnmAng();
			byte NI=Fo->NI;
			byte NMask=Fo->GetNMask();//CITY[NI].Nat->NMask;
			if(Fo&&Fo->Type>0)
			{
				int nnn=AllForm.GetAmount();
				for(int k=0;k<nnn;k++)
				{
					Atom* A=AllForm[k];
					if(A&&A->IsAlive()&&(!(NMask&A->GetNMask())))
						Fo->AddToEnmAng(A);
				}
				Fo->CalkAvrEnmDir();
			}
		}
	}
}
void BrigadeAI::FillFriendAng()
{
	for(int i=0;i<MaxAF;i++)
	{
		AutoForm* Fo=AF[i];
		if (Fo&&Fo->IsAlive()) 
		{
			Fo->ClearFriendAng();
			byte NMask=Fo->GetNMask();//NATIONS[Fo->NI].NMask;
			if(Fo&&Fo->Type!=0)
				for(int j=0;j<MaxAF;j++)
				{
					AutoForm* AA=AF[j];
					if(AA&&AA->IsAlive())
					{
						byte NMask2=AA->GetNMask();//NATIONS[AA->NI].NMask;
						if(Fo!=AA&&(NMask&NMask2)&&AA->Type!=0)
							Fo->AddToFriendAng(AA);
					}
				}
		}
	}
}
void BrigadeAI::FillAllForm()
{
	//if(AllFormFillTime<tmtmt)
	AllForm.Clear();
	int Aindex=0;
	{
		for(int i=0;i<8;i++)
		{	
			for(int j=0;j<MaxBrig;j++)
			{
				Brigade* BR=&(CITY[i].Brigs[j]);
				if(BR->Enabled&&BR->WarType!=0)
				{
					Atom* A = new Atom(BR);
					A->Serial=++LastSerial;
					A->AtomIndex=Aindex++;
					AllForm.Add(A);
				}
			}
			if(NtNUnits[i])
			{
				for(int j=0;j<NtNUnits[i];j++)
				{
					word UMID=NatList[i][j];
					if(UMID!=0xFFFF)
					{
						OneObject* OB=Group[UMID];
						if(OB&&(OB->newMons->Usage==PushkaID||OB->newMons->Usage==TowerID||OB->newMons->Usage==LinkorID))
						{
							Atom* A = new Atom(OB);
							A->Serial=++LastSerial;
							A->AtomIndex=Aindex++;
							AllForm.Add(A);
						}
					}
				}
			}
		}
		AllFormFillTime=999999999;
	}
}
word BrigadeAI::GetBrigadeNIndex(Brigade *B)
{
	word rez=0xFFFF;
	if(B)
	{
		for(int i=3;i<B->NMemb;i++)
		{
			word in=B->Memb[i];
			if(in!=0xFFFF)
			{
				OneObject* OB = Group[in];
				if(OB&&OB->Ref.General->UsualFormID==0xFFFF)
				{
					return OB->NIndex;
				}
			}
		}
	}
	return rez;
}
word BrigadeAI::GetRulesIndex(word UnitNIndex)
{
	word rez=0xFFFF;
	if(UnitNIndex!=0xFFFF)
	{
		for(int i=0;i<AllRules.GetAmount();i++)
		{
			int n=AllRules[i]->UnitsT.GetAmount();
			for(int j=0;j<n;j++)
			{
				if(AllRules[i]->UnitsT[j]==UnitNIndex)
					return AllRules[i]->Index;
			}
		}
	}
	return rez;
}
void BrigadeAI::NewBrigadeIsBorn(Brigade *B)
{
	if(B&&B->WarType!=0)
	{
		//int n=B->CT->NI;
		AllForm.Add(new Atom(B));
		AllForm.End()->Serial=++LastSerial;
		AllForm.End()->AtomIndex=AllForm.GetAmount()-1;
	}
}
void CheckMemoryIntegrity();
void BrigadeAI::LoadAllRules(char *p, const char *shotfilename)
{
	DString R, fw, NIn;
	R.ReadFromFile(p);
	bool f=true;
	int m=AllRules.GetAmount();
	if(R.L)
	for(int i=0;i<1024&&f;i++)
	{
		//R.ExtractWord2(fw);
		R.ExtractLine(fw);
		fw.ExtractWord(NIn);
		if(NIn.pchar()[0]=='#')
		{
			Rules *r = new Rules;
			r->m_fileName=shotfilename;
			r->Index=AllRules.GetAmount();
			AllRules.Add(r);
			do 
			{
				fw.ExtractWord(NIn);
				if(NIn.L>0)
				{
					GAMEOBJ Type;
					if(RegisterUnitType(&Type,NIn.pchar()))
					{
						r->UnitsT.Add(Type.Index);
					}
					else
					{
						ErrBox("Rules.ai : Unknown unit type %s", NIn.pchar());
					}
				}
			} 
			while(NIn.L>0);
			//AllRules[i].UnitType=
			r->LoadRules(&R);
			r->Index=m;
			m++;
			//f=false;
		}
	}
}
void BrigadeAI::LoadAddRules(char* filesextension)
{
	WIN32_FIND_DATA	wfd;
	HANDLE			hFindFile;
	char dr[256];
	char p[256];
	char fp[256];
	bool found;
	strcpy(dr,".\\AI\\BrigadeAI\\");
	strcpy(p,dr);
	strcat(p,"*.");
	strcat(p,filesextension);

	hFindFile=FindFirstFile(p,&wfd);

	if(hFindFile!=INVALID_HANDLE_VALUE)
		found=true;
	else
		found=false;
	while(found)
	{
		if((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=FILE_ATTRIBUTE_DIRECTORY)
		{
			strcpy(fp,dr);
			strcat(fp,wfd.cFileName);
			LoadAllRules(fp,wfd.cFileName);
		}
		found=FindNextFile(hFindFile,&wfd);
	}
}
int BrigadeAI::CreateRegiment()
{
	int rez=-1;
	if(MaxReg<127)
	{
		Reg[MaxReg]= new Regiment(MaxReg,AF,&MaxAF);
		rez=MaxReg;
		MaxReg++;
	}
	return rez;
}
Polk* BrigadeAI::GetPolkByIndex(int PIndex)
{
	Polk* rez=NULL;
	if(PIndex>-1&&PIndex<MaxReg)
		rez=(Polk*) Reg[PIndex];
	return rez;
}
int BrigadeAI::BirthCallback(int NewbornIndex)
{
	//if(lpBirthCallback)
	//	lpBirthCallback(NewbornIndex);
	if(NewbornIndex!=0xFFFF)
	{
		OneObject* OB=Group[NewbornIndex];
		if(OB&&(OB->newMons->Usage==PushkaID||OB->newMons->Usage==TowerID||OB->newMons->Usage==LinkorID))
		{
			//byte N=OB->NNUM;
			AllForm.Add(new Atom(OB));
			AllForm.End()->Serial=++LastSerial;
			AllForm.End()->AtomIndex=AllForm.GetAmount()-1;
			//AllFormAmo[N]++;
		}
	}
	return 0;
}
void BrigadeAI::CreateAreaMap()
{

}
void BrigadeAI::UnLoading()
{
	int nnn=AllForm.GetAmount();
	for(int j=0;j<nnn;j++)
		if(AllForm[j])
		{
			delete (AllForm[j]);
			AllForm[j]=NULL;
		}
	AllForm.Clear();

	nnn=AllFormGarbage.GetAmount();
	for(int i=0;i<nnn;i++)
		if(AllFormGarbage[i])
		{
			delete (AllFormGarbage[i]);
			AllFormGarbage[i]=NULL;
		}
	AllFormGarbage.Clear();

	ClearCallbacks();
	int i;
	for(i=0;i<MaxReg;i++)
		if(Reg[i])
		{
			delete (Reg[i]);
			Reg[i]=NULL;
		}
	MaxReg=0;
	for(i=0;i<NA;i++)
		if(AreaMap[i])
		{
			delete (AreaMap[i]);
			AreaMap[i]=NULL;
		}
	int nr=AllRules.GetAmount();
	for(int i=0;i<nr;i++)
	{
		if(AllRules[i])
		{
			delete AllRules[i];
			AllRules[i]=NULL;
		}
	}
	AllRules.Clear();
	NA=0;
	if(AreaMap)free(AreaMap);
	AreaMap=NULL;
	FirstStart=true;
	MaxAF=0;
	MaxReg=0;
	lpBirthCallback=NULL;
	if(AF)memset(AF,0,512*4);
	if(Reg)memset(Reg,0,128*4);
}
void BrigadeAI::ClearMainTaget(Atom* Deleted)
{
	if(Deleted)
	{
		for(int i=0;i<MaxAF;i++)
		{
			if(AF[i]&&AF[i]->MainTarget==Deleted)
				AF[i]->MainTarget=NULL;
		}
	}
}
void BrigadeAI::Save(SaveBuf *SB)
{
	int i;
	int nn=-1;
	xBlockWrite(SB,&MaxAF,4);
	for(i=0;i<MaxAF;i++)
		if(AF[i])
			AF[i]->Save(SB);
		else
			xBlockWrite(SB,&nn,4);
	xBlockWrite(SB,&MaxReg,4);
	for(i=0;i<MaxReg;i++)
	{
		if(Reg[i])
		{
			byte tmp=1;
			xBlockWrite(SB,&tmp,1);
			Reg[i]->Save(SB);
		}
		else
		{
			byte tmp=0;
			xBlockWrite(SB,&tmp,1);
		}
	}
	byte end = 100;
	xBlockWrite(SB,&end,1);
}
void BrigadeAI::Load(SaveBuf *SB)
{
	int i;
	int MAF;
	xBlockRead(SB,&MAF,4);
	if(MAF)
	{
		Init();
		MaxAF=MAF;
		FirstStart=false;
		AllFormFillTime=-1;
		FillAllForm();
		for(i=0;i<MaxAF;i++)
		{
			int Type;
			int n;
			xBlockRead(SB,&Type,4);
			bool AlrFind=false;
			if(Type==1||Type==2)
			{
				AutoForm* F= new AutoForm();
				n=F->Load(SB);
				AF[n]=F;
				//byte NI=F->NI;
				int nnn=AllForm.GetAmount();
				F->RulesList=AllRules[F->RulesID];
				for(int j=0;j<nnn;j++)
				{
					if(AllForm[j]&&AllForm[j]->BSelf&&(AllForm[j]->BSelf->ID==F->BSelf->ID
						&&AllForm[j]->BSelf->CT->NI==F->BSelf->CT->NI))
					{
						if(!AlrFind)
						{
							AllForm[j]->Type=0;
							AllForm[j]->BSelf=NULL;
							AllForm[j]->C=NULL;
							ClearMainTaget(AllForm[j]);
							delete (AllForm[j]);
							AllForm[j]=(Atom*)F;
							AlrFind=true;
						}
						else
						{
							ClearMainTaget(AllForm[j]);
							delete (AllForm[j]);
							AllForm[j]=NULL;
						}
					}
				}
			}
			else
			if(Type==3)
			{
				Cannon* C= new Cannon();
				n=C->Load(SB);
				AF[n]=C;
				//int NI=C->NI;
				int nnn=AllForm.GetAmount();
				C->RulesList=AllRules[C->RulesID];
				for(int j=0;j<nnn;j++)
				{
					if(AllForm[j]&&AllForm[j]->Type==3&&(AllForm[j]->CannonID==C->C->Index))
					{
						if(!AlrFind)
						{
							AllForm[j]->Type=0;
							AllForm[j]->BSelf=NULL;
							AllForm[j]->C=NULL;
							ClearMainTaget(AllForm[j]);
							delete (AllForm[j]);
							AllForm[j]=(Atom*)C;
							AlrFind=true;
						}
						else
						{
							ClearMainTaget(AllForm[j]);
							delete (AllForm[j]);
							AllForm[j]=NULL;
						}
					}
				}
			}
		}
	}
	xBlockRead(SB,&MaxReg,4);
	for(i=0;i<MaxReg;i++)
	{
		byte tmp=0;
		xBlockRead(SB,&tmp,1);
		if(tmp==1)
		{
			Regiment* R = new Regiment(0,AF,&MaxAF);
			int n = R->Load(SB);
			Reg[n]=R;
		}
	}
	byte end=0;
	xBlockRead(SB,&end,1);
}
//====================================================================//
Rule::Rule()
{
	for(int i=0;i<16;i++)
	{
		Situations[i].CompareID=0;
		Situations[i].FunctionID1=-1;
		Situations[i].FunctionID2=-1;
	}
	for(i=0;i<16;i++)
	{
		Actions[i].ActionID=-1;
	}
	Next=NULL;
}
Rule::~Rule()
{
	if(Next)
		delete Next;
}
bool Rule::CheckSituation(AutoForm *A, int i)
{
	bool rez=true;
	int vle1, vle2;

	if(Situations[i].FunctionID1!=-1)
	{
		vle1=A->GetSituationByIndex(Situations[i].FunctionID1);
		if(vle1==-1)return false;
	}
	else
		vle1=Situations[i].Value1;
	if(Situations[i].FunctionID2!=-1)
	{
		vle2=A->GetSituationByIndex(Situations[i].FunctionID2);
		if(vle2==-1)return false;
	}
	else
		vle2=Situations[i].Value2;

	switch (Situations[i].CompareID)
	{
	case 0: 
		break;
	case 1: //NOT
		if(Situations[i].FunctionID1!=-1)
			rez=!A->GetSituationByIndex(Situations[i].FunctionID1);
		break;
	case 2: // ">"		
		rez=(vle1>vle2);
		break;
	case 3: // "<"
		rez=(vle1<vle2);
		break;
	case 4: // "="
		rez=(vle1==vle2);
		break;
	case 5: // ">="
		rez=(vle1>=vle2);
		break;
	case 6: // "<="
		rez=(vle1>=vle2);
		break;
	case 7: // "!="
		rez=(vle1!=vle2);
		break;
	}
	return rez;
}
bool Rule::CheckRule(AutoForm *A)
{
	bool rez=true;
	for(int i=0;i<16&&rez;i++)
		rez=CheckSituation(A, i);
	return rez;
}
void Rule::FillSituations(DString *s)
{
	int poi=0;
	DString p1, iff, p2;
	while(s->L)
	{
		s->ExtractWord2(p1);
		s->ExtractWord2(iff);
		s->ExtractWord2(p2);
		assert(poi<32);
		SetSituation(p1.pchar(), iff.pchar(), p2.pchar(), &(Situations[poi]));
		poi++;
	}
}
void Rule::SetSituation(char* p1,char *iff, char *p2, Situation *sit)
{
	//
	if(isdigit(p1[0])||p1[0]=='-')
		sit->Value1=atoi(p1);
	else
		sit->FunctionID1=GetFunctionID(p1);
	
	if(isdigit(p2[0])||p2[0]=='-')
		sit->Value2=atoi(p2);
	else
		sit->FunctionID2=GetFunctionID(p2);
	
	sit->CompareID=GetCompareID(iff);
}
int Rule::GetCompareID(char *iff)
{
	int rez=0;
	if(!strcmp(iff,"!"))  rez=1;
	else
	if(!strcmp(iff,">"))  rez=2;
	else
	if(!strcmp(iff,"<"))  rez=3;
	else
	if(!strcmp(iff,"="))  rez=4;
	else
	if(!strcmp(iff,">=")) rez=5;
	else
	if(!strcmp(iff,"<=")) rez=6;
	else
	if(!strcmp(iff,"!=")) rez=7;
	else
	if(strlen(iff)>0)
		ErrBox("Rules.ai : Unknown comparison %s",iff);

	return rez;
}
int Rule::GetFunctionID(char *fff)
{
	int rez=-1;
	if(!strcmp(fff,"DistToMT"))         rez=0;
	else
	if(!strcmp(fff,"MoralMT"))          rez=1;
	else
	if(!strcmp(fff,"KillAmoMT"))        rez=2;
	else
	if(!strcmp(fff,"FireReadyMT"))      rez=3;
	else
	if(!strcmp(fff,"UnitsAmountMT"))    rez=4;
	else
	if(!strcmp(fff,"TiredMT"))          rez=5;
	else
	if(!strcmp(fff,"MoralMTAfterFire"))	rez=6;
	else
	if(!strcmp(fff,"Moral"))            rez=7;
	else
	if(!strcmp(fff,"GetFireReady"))     rez=8;
	else
	if(!strcmp(fff,"UnitsAmount"))      rez=9;
	else
	if(!strcmp(fff,"GetTired"))         rez=10;
	else
	if(!strcmp(fff,"DistToAim"))      rez=11;
	else
	if(!strcmp(fff,"SumDangAndForce"))      rez=12;
	else
	if(!strcmp(fff,"FireForceDifference"))      rez=13;
	else
	if(!strcmp(fff,"Encircle"))      rez=14;
	else
	if(!strcmp(fff,"HaveReserv"))      rez=15;
	else
	if(!strcmp(fff,"AngToMT"))      rez=16;
	else
	if(!strcmp(fff,"DistInFront"))      rez=17;
	else
	if(!strcmp(fff,"DistInReserv"))      rez=18;
	else
	if(!strcmp(fff,"GetStand"))      rez=19;
	else
	if(!strcmp(fff,"IsEnemOnWay")) rez=20;
	else
	if(!strcmp(fff,"GetNGrenades")) rez=21;
	else
	if(!strcmp(fff,"GetMTForestDensity")) rez=22;
	else
	if(!strcmp(fff,"IsMTCannon")) rez=23;
	else
	if(!strcmp(fff,"KartechDanger")) rez=24;
	else
	if(!strcmp(fff,"FriendlyKartechDanger")) rez=25;
	else
	if(!strcmp(fff,"IsInBatter")) rez=26;
	else
	if(!strcmp(fff,"GetBarrierToMT")) rez=27;
	else
	if(!strcmp(fff,"FireDanger")) rez=28;
	else
	if(!strcmp(fff,"GetStandTime")) rez=29;
	else
	if(!strcmp(fff,"GetMTStandTime")) rez=30;
	else
	if(!strcmp(fff,"GetAloneEnmUnits")) rez=31;
	else
	if(!strcmp(fff,"GetMeleeForceDiff")) rez=32;
	else
	if(!strcmp(fff,"GetNUnitsDiff")) rez=33;
	else
	if(!strcmp(fff,"Random")) rez=34;
	else
	if(!strcmp(fff,"IsStandGround")) rez=35;
	else
	if(!strcmp(fff,"IsMTStandGround")) rez=36;
	else
	if(!strcmp(fff,"GetHorseDanger")) rez=37;
	else
	if(!strcmp(fff,"MoralDiff")) rez=38;
	else
	if(!strcmp(fff,"GetPolkCommand")) rez=39;
	else
	if(!strcmp(fff,"GetDistToEnmTower")) rez=40;
	else
	if(!strcmp(fff,"GetSelfForestDensity")) rez=41;
	else
	if(!strcmp(fff,"GetFOOD")) rez=42;
	else
	if(!strcmp(fff,"GetCOAL")) rez=43;
	else
	if(!strcmp(fff,"GetLastCommTime")) rez=44;
	else
	if(!strcmp(fff,"GetDistToFriendlyForm")) rez=45;
	else
	if(!strcmp(fff,"GetNFormationsInMyPolk")) rez=46;
	else
	if(!strcmp(fff,"GetDistToEnmBuilding")) rez=47;
	else
	if(!strcmp(fff,"GetGameTime")) rez=48;
	else
	if(!strcmp(fff,"GetFrontage")) rez=49;
	//Cannon
	else
	if(!strcmp(fff,"GetMaxDensityInZerroSector")) rez=50;
	else
	if(!strcmp(fff,"GetMaxDensityInSectors")) rez=51;
	else
	if(!strcmp(fff,"GetKartechEffect")) rez=52;
	else
	if(!strcmp(fff,"GetPushkaChargeState0")) rez=53;
	else
	if(!strcmp(fff,"GetPushkaChargeState1")) rez=54;
	else
	if(!strcmp(fff,"GetFillingAbility")) rez=55;
	//
	else
	if(!strcmp(fff,"GetDiff")) rez=56;
	else
	if(!strcmp(fff,"IsMTAI")) rez=57;
	else
	if(!strcmp(fff,"GetMTAngl")) rez=58;
	else
	if(!strcmp(fff,"GetMaxHightInMTDirection")) rez=59;
	else
	if(!strcmp(fff,"GetProcDistToAimInPolk")) rez=60;
	else
	if(!strcmp(fff,"GetNApparentFriendlyForm")) rez=61;
	else
	if(!strcmp(fff,"GetNApparentEnemyForm")) rez=62;
	else
	if(!strcmp(fff,"CanSetBomb")) rez=63;
	else	
	//if(!strcmp(fff,"GetDiff")) rez=64;
	//else	
	{
		rez=GetStateID(fff);
		if(rez>-1)
			rez+=100;
	}
	if(rez==-1)
	{
		if(strlen(fff)>3)
			ErrBox("Rules.ai : Unknown function %s",fff);
	}

	return rez;
}
void Rule::FillActions(DString *s)
{
	int poi=0;
	DString a, w;
	while(s->L)
	{
		s->ExtractWord2(a);
		s->ExtractWord2(w);
		int aid=GetActionID(a.pchar());
		if(aid>-1)
		{
			Actions[poi].ActionID=aid;
			Actions[poi].Weight=atoi(w.pchar());
			poi++;
		}
	}
}
int Rule::GetActionID(char *aa)
{
	int rez=-1;
	if(!strcmp(aa,"AttackMT"))       rez=0;
	else
	if(!strcmp(aa,"TurnToMT"))       rez=1;
	else
	if(!strcmp(aa,"MoveBack"))       rez=2;
	else
	if(!strcmp(aa,"SetReadyToFire")) rez=3;
	else
	if(!strcmp(aa,"EnableFire"))     rez=4;
	else
	if(!strcmp(aa,"DisableFire"))    rez=5;
	else
	if(!strcmp(aa,"Stop"))           rez=6;
	else
	if(!strcmp(aa,"FormateToLine"))  rez=7;
	else
	if(!strcmp(aa,"FormateToSquare"))rez=8;
	else
	if(!strcmp(aa,"FormateToKare"))  rez=9;
	else
	if(!strcmp(aa,"FindPlaceInFront"))  rez=10;
	else
	if(!strcmp(aa,"TurnToMaxDang"))  rez=11;
	else
	if(!strcmp(aa,"MoveToAim"))  rez=12;
	else
	if(!strcmp(aa,"Castle"))  rez=13;
	else
	if(!strcmp(aa,"FireNow"))  rez=14;
	else
	if(!strcmp(aa,"KeepDistInFront"))  rez=15;
	else
	if(!strcmp(aa,"KeepDistInReserv")) 	rez=16;
	else
	if(!strcmp(aa,"Fire")) 	rez=17;
	else
	if(!strcmp(aa,"SetNotReadyToFire")) rez=18;
	else
	if(!strcmp(aa,"ThrowGrenade")) rez=19;
	else
	if(!strcmp(aa,"MeleeAttack")) rez=20;
	else
	if(!strcmp(aa,"OneLineShot")) rez=21;
	else
	if(!strcmp(aa,"TakeInTheRear")) rez=22;
	else
	if(!strcmp(aa,"AttackAloneEnmUnits")) rez=23;
	else
	if(!strcmp(aa,"DoNothing")) rez=24;
	else
	if(!strcmp(aa,"StepForward")) rez=25;
	//Cannon
	else
	if(!strcmp(aa,"TurnToMaxDensitySector")) rez=50;
	else
	if(!strcmp(aa,"ChargeCannon0")) rez=51;
	else
	if(!strcmp(aa,"ChargeCannon1")) rez=52;
	else
	if(!strcmp(aa,"TurnCannonToMaxEnmFormationSector")) rez=53;
	else
	if(!strcmp(aa,"FireOnEnmTower")) rez=54;
	else
	if(!strcmp(aa,"FireOnEnmBuilding")) rez=55;
	else
	if(!strcmp(aa,"ArmAttack")) rez=56;
	else
	if(!strcmp(aa,"Ram")) rez=57;
	else
	if(!strcmp(aa,"SplashFire")) rez=58;
	else
	if(!strcmp(aa,"ThrowGrenadeInABuilding")) rez=59;
	else
	if(!strcmp(aa,"SetBomb")) rez=60;
	else
	if(strlen(aa)>3)
		ErrBox("Rules.ai : Unknown action %s",aa);
	
	return rez;
}
int Rule::GetStateID(char* aa)
{
	int rez=-1;
	if(strstr(aa,"St_")!=NULL)
	{
		char* aaa = aa+3;
		if(!strcmp(aaa,"Attack"))       rez=0;
		else
		if(!strcmp(aaa,"Reserv"))       rez=1;
		else
		if(!strcmp(aaa,"MoveBack"))       rez=2;
		else
		if(!strcmp(aaa,"Stand")) rez=3;
		else
		if(!strcmp(aaa,"Front"))     rez=4;
		else
		if(!strcmp(aaa,"March"))    rez=5;
		else
		if(!strcmp(aaa,"NearAim"))           rez=6;
		else
		if(!strcmp(aaa,"MoveFront"))  rez=7;
		else
		if(!strcmp(aaa,"BattleFormation"))rez=8;
		else
		if(!strcmp(aaa,"Rest"))  rez=9;
		else
		if(!strcmp(aaa,"Move"))  rez=10;
		else
		if(!strcmp(aaa,"ReadyToFire"))  rez=11;
		else
		if(!strcmp(aaa,"LineFormation"))     rez=12;
		else
		if(!strcmp(aaa,"SquareFormation"))    rez=13;
		else
		if(!strcmp(aaa,"KareFormation"))      rez=14;
		else
		if(!strcmp(aaa,"EnableFire"))  rez=15;
		else
		if(!strcmp(aaa,"DisableFire"))rez=16;
	}
	return rez;
}
//====================================================================//
Rules::Rules()
{
	First=NULL;
	Last=NULL;
}
Rules::~Rules()
{
	if(First)
	{
		delete First;
		First=NULL;
	}
}
void Rules::VerifySituation(AutoForm *A)
{
	if(A)
	{
		for(int i=0;i<128;i++)
			A->ActionsArr[i]=0;
		Pointer=First;
		while(Pointer)
		{
			if(Pointer->CheckRule(A))
				for(int i=0;i<8;i++)
					if(Pointer->Actions[i].ActionID>-1)
						A->ActionsArr[Pointer->Actions[i].ActionID]+=Pointer->Actions[i].Weight;
			Pointer=Pointer->Next;
		}
	}
}
void Rules::LoadRules(DString *d)
{
	DString ds;
	d->ExtractWord2(ds);
	while(ds.pchar()[0]!='#'&&ds.pchar()[0]!=0)
	{
		if(!strcmp(ds.pchar(),"if"))
		{
			Rule* R = new Rule();

			if(!First)First=R;
			if(Last)Last->Next=R;
			Last=R;
			d->ExtractLine(ds);
			if(ds.pchar()[ds.L-1]==13)
				ds.DeleteChars(ds.L-1,1);
			R->FillSituations(&ds);
			d->ExtractWord2(ds);
			if(!strcmp(ds.pchar(),"do"))
			{
				d->ExtractLine(ds);
				R->FillActions(&ds);
			}
		}
		d->ExtractWord2(ds);
	}
}

//====================================================================//
Polk::Polk()
{}
Polk::~Polk()
{}
void Polk::MovePolkTo(int x, int y, int Dir)
{}
int Polk::GetEnemyForce()
{
	return 0;
}
int Polk::GetPolkIndex()
{
	return -1;
}
bool Polk::ReSetFormation(int FormIndex)
{
	return false;
}
void Polk::AddSelectedToPolk(byte Nat)
{
}
void Polk::AddBrigadeToPolk(word Nat, int BrIndex)
{
}
int Polk::GetFormationIndex(int UnitType)
{
	return -1;
}
int Polk::GetNFormations()
{
	return -1;
}
bool Polk::GetFormation(int Index, byte& NI, int& BrigID){
	return false;
};
void Polk::RemanPolkFrom(GAMEOBJ* Group)
{
}
int Polk::GetUnitsAmountInPolk(bool reman)
{
	return 0;
}
bool Polk::AddBrigade(Brigade* B)
{
	return false;
}
bool Polk::AddCannon(OneObject* Ob)
{
	return false;
}
bool Polk::GetPolkFrontCenter(int& x, int& y)
{
	return false;
}
//====================================================================//
Regiment::Regiment()
{
	Index=-1;
	DistX=-1;
	DistY=-1;
	Radius=1200;
	SearchEnemyRadius=3000;
}
Regiment::~Regiment()
{
 ;//
}
Regiment::Regiment(int I, AutoForm** AFL, int* MaxAFL)
{
	Index=I;
	DistX=-1;
	DistY=-1;
	DistDir=-1;

	//DistX=11000;
	//DistY=11000;
	//DistDir=64;

	AFList=AFL;
	MaxAFList=MaxAFL;
	TableFillTime=0;
	EnemyForce=-1;
	Chase=true;
	RegisterDynGroup(&RemanGroup);
	SynchronCommand=0;
	SynchronCommandTime=0;
}
void Regiment::MovePolkTo(int x, int y, int Dir)
{
	DistX=x;
	DistY=y;
	DistDir=Dir;
	FillDestPoints();
}
int Regiment::GetEnemyForce()
{
	return EnemyForce;
}
int Regiment::GetPolkIndex()
{
	return Index;
}
bool Regiment::AddBrigade(Brigade *B)
{
	int in= BrigadeAI::AddAutoBrigade(B);
	if(in>-1)
	{
		AFList[in]->RegimentIndex=Index;
		AFList[in]->AimX=DistX;
		AFList[in]->AimY=DistY;
		return true;
	}	
	return false;
}
bool Regiment::AddCannon(OneObject *Ob)
{
	int in= BrigadeAI::AddAutoCannon(Ob);
	if(in>-1)
	{
		AFList[in]->RegimentIndex=Index;
		AFList[in]->AimX=DistX;
		AFList[in]->AimY=DistY;
		return true;
	}
	return false;
}
AutoForm* Regiment::GetAutoForm(int UnitType)
{
	for(int i=0;i<*MaxAFList;i++)
		if(AFList[i]->RegimentIndex==Index&&AFList[i]->BSelf->MembID==UnitType)
			return AFList[i];
	return NULL;
}
int Regiment::GetFormationIndex(int UnitType)
{
   	for(int i=0;i<*MaxAFList;i++)
		if(AFList[i]->RegimentIndex==Index&&AFList[i]->BSelf->MembID==UnitType)
			return i;
	return -1;
}
int Regiment::GetNFormations()
{
	int col=0;
	for(int i=0;i<BrigadeAI::MaxAF;i++)
		if(BrigadeAI::AF[i]&&BrigadeAI::AF[i]->RegimentIndex==Index)
			col++;
	return col;
}
bool Regiment::GetFormation(int Ind, byte& NI, int& BrigID){
	int dd=0;
	for(int i=0;i<BrigadeAI::MaxAF;i++)
		if(BrigadeAI::AF[i]&&BrigadeAI::AF[i]->RegimentIndex==Index)
		{
			Brigade* BR=BrigadeAI::AF[i]->BSelf;
			if(BR)
			{
				if(dd==Ind)
				{
					NI=BR->CT->NI;
					BrigID=BR->ID;
					return true;
				}
			}
			dd++;
		}
	return false;
};
bool Regiment::GetPolkFrontCenter(int& x, int& y)
{
	bool rez=false;
	int dd=0;
	int MinDist;
	int cx=0;
	int cy=0;
	for(int i=0;i<BrigadeAI::MaxAF;i++)
	{
		if(BrigadeAI::AF[i]&&BrigadeAI::AF[i]->RegimentIndex==Index)
		{
			int dist = BrigadeAI::AF[i]->DistToMT();
			int k=1;
			if(dist<5000)
			{
				k=50-dist/100;
			}
			cx+=BrigadeAI::AF[i]->x*k;
			cy+=BrigadeAI::AF[i]->y*k;
			//9999999
			dd+=k;
		}
	}
	if(dd)
	{
		x=cx/dd;
		y=cy/dd;
	}
	return rez;
}
bool Regiment::ReSetFormation(int FormIndex)
{
	if(FormIndex>-1)
		if(AFList[FormIndex])
		{
			AFList[FormIndex]->RegimentIndex=Index;
			AFList[FormIndex]->AimX=DistX;
			AFList[FormIndex]->AimY=DistY;
			return true;
		}
	return false;
}
void Regiment::Process()
{
	if(tmtmt>(TableFillTime+75))
	{
		TableFillTime=tmtmt;
		FillTable();
		if(MaxP)
		{
			SetActualDist();
			RemanProces();
		}
	}
	if(AnimTime>(SynchronCommandTime+256*25*10))
	{
		SynchronCommand++;
		if(SynchronCommand>9)
			SynchronCommand=0;
		for(int i=0;i<*MaxAFList;i++)
		{
			AutoForm* AF=AFList[i];
			if(AF&&AF->BSelf&&AF->BSelf->WarType!=0&&AF->Type!=0&&AF->IsAlive())
				if(AF->RegimentIndex==Index)
					AF->PolkSynchronCommand=SynchronCommand;
		}
		SynchronCommandTime=AnimTime;
	}
}
void Regiment::AddSelectedToPolk(byte Nat)
{
	if(Nat>-1&&Nat<8&&NSL[Nat]>0)
	{
		for(int i=0;i<NSL[Nat];i++)
		{
			OneObject* OB=Group[Selm[Nat][i]];
			if(OB&&OB->BrigadeID!=0xFFFF)
				AddBrigade(&(CITY[Nat].Brigs[OB->BrigadeID]));
			else
			if(OB->newMons->Usage==PushkaID||OB->newMons->Usage==TowerID||OB->newMons->Usage==LinkorID)
				AddCannon(OB);
		}
		FillTable();
	}
}
void Regiment::AddBrigadeToPolk(word Nat, int BrIndex)
{
	if(Nat>-1&&Nat<8)
	{
		AddBrigade(&(CITY[Nat].Brigs[BrIndex]));
	}
}
void Regiment::FillTable()
{
	MaxP=0;
	MaxE=0;
	for(int d=0;d<128;d++)
		for(int f=0;f<128;f++)
		{
			TNearestP[d][f]=false;
			TNearestE[d][f]=false;
			TVisible[d][f]=false;
			TDist[d][f]=-1;
		}
	for(int i=0;i<*MaxAFList;i++)
	{
		AutoForm* AF=AFList[i];
		if(AF&&AF->IsAlive())
			if(AF->RegimentIndex==Index)
				AddFillEn(AF);
	}
	for(i=0;i<MaxP;i++)
	{
		int md=99999999;
		int ind=-1;
		for(int j=0;j<MaxE;j++)
			if(TDist[i][j]<md)
			{
				md=TDist[i][j];
				ind=j;
			}
		TNearestE[i][ind]=true;
	}
	for(i=0;i<MaxE;i++)
	{
		int md=99999999;
		int ind=-1;
		for(int j=0;j<MaxP;j++)
			if(TDist[j][i]>0&&TDist[j][i]<md)
			{
				md=TDist[j][i];
				ind=j;
			}
		TNearestP[ind][i]=true;
	}
}
void Regiment::AnalizeTable()
{
	if(MaxP)
		EnemyForce = (MaxE*100)/MaxP;
	else
		EnemyForce = 1000;
}
int Regiment::AddE(Atom *A)
{
	if(A&&A->Type>0)
	{
		for(int i=0;i<MaxE;i++)
			if(E[i]==A)
				return i;
		E[MaxE]=A;
		MaxE++;
		return MaxE-1;
	}
	return -1;
}
void Regiment::AddFillEn(AutoForm *AF)
{
	if(AF&&AF->Type)
	{
		P[MaxP]=AF;
		int Pi=MaxP;
		MaxP++;
		for(int i=0;i<16;i++)
			if(AF->EnmAng[i].SumDang)
				for(int j=0;j<AF->EnmAng[i].SectorsAmo;j++)
				{
					int Ei=AddE(AF->EnmAng[i].Sectors[j].A);
					TDist[Pi][Ei]=AF->EnmAng[i].Sectors[j].dest;
					if(AF->FriendAng[i].Dang&&j==0)
						TVisible[Pi][Ei]=false;
					else
						TVisible[Pi][Ei]=true;
				}
	}
}
int Regiment::GetActualDist()
{
	int rez=-1;
	int mindist=999999999;
	int round=0;
	int OnWayAvrDst=0;
	int OnWayCount=0;
	for(int i=0;i<MaxP;i++)
	{
		if(P[i]->IsEnemOnWay())
		{
			OnWayAvrDst+=P[i]->DistToMT();
			OnWayCount++;
		}
		int c=0;
		for(int j=0;j<MaxE;j++)
		{
			if(TNearestP[i][j])
			{
				c++;
				if(TDist[i][j]>0&&TDist[i][j]<mindist)
					mindist=TDist[i][j];
			}
		}
		if(c>1)
			round++;
	}
	if(round)
		rez=mindist-20*round;
	else
		rez=mindist;
	//if(MaxE==1)rez=300;
	/*
	if(OnWayCount)
	{
		mindist=mindist-mindist/5;
		OnWayAvrDst=OnWayAvrDst/OnWayCount;
		if((rez>-1&&rez<OnWayAvrDst)||(rez==-1))
			rez=OnWayAvrDst-OnWayAvrDst/5;
	}
	*/
	if(rez>1500)
		rez=-1;
	return rez;
}
void Regiment::SetActualDist()
{
	int dd=GetActualDist();
	if(Chase)
	{
		for(int i=0;i<MaxP;i++)
			if(P[i])
				P[i]->DistToMainTarget=dd;
	}
	else
	{
		for(int i=0;i<MaxP;i++)
			if(P[i])
				P[i]->DistToMainTarget=dd-dd/3;
	}
	//SetDistIfEnmAlone();
	FillDestPoints();
}
void Regiment::SetDistIfEnmAlone()
{
	for(int i=0;i<MaxE;i++)
	{
		int co=0;
		bool twis=true;
		for(int j=0;j<MaxP&&twis;j++)
		{
			if(TNearestE[j][i])
			{
				bool flg=true;
				for(int k=0;k<MaxE&&flg;k++)
					if(TNearestP[j][k]&&k!=i)
					{
						flg=false;
						twis=false;
					}
				if(flg)
					co++;
			}			
		}
		if(co>1&&twis)
		{
			int dst=800-100*co;
			if(dst<400)dst=400;
			for(int j=0;j<MaxP;j++)
			{
				bool flg=false;
				if(TNearestE[j][i])
				{
					flg=true;
					for(int k=0;k<MaxE&&flg;k++)
						if(TNearestP[j][k]&&k!=i)
							flg=false;
				}
				if(flg)
					P[j]->DistToMainTarget=dst;	
			}
		}
	}
}
void Regiment::FillDestPoints()
{
	if(DistX!=-1&&DistY!=-1&&MaxP)
	{
		if(DistDir==-1)DistDir=0;
		for(int i=0;i<MaxP;i++)
		{
			if(P[i])
			{
				P[i]->AimX=-1;
				P[i]->AimY=-1;
				P[i]->AimDir=-1;
			}
		}

		int column = (int)sqrt(MaxP*10000*2);
		if(column%100)
			column=column/100+1;
		else
			column=column/100;
		int line = MaxP/column;
		if(MaxP%column)line++;
		int* DestPoints=(int*)malloc((column*line)<<3);
		int* FromPoints=(int*)malloc((column*line)<<3);
		int RangeX = 200;
		int RangeY = 400;
		int dd=0;
		for(int y=0;y<column;y++)
		{
			for(int x=0;x<line;x++)
			{
				if(dd<MaxP)
				{
					int xx=DistX-(column*RangeX)/2+x*RangeX;
					int yy=DistY-(line*RangeY)/2+y*RangeY;

					int xa=DistX+(((xx-DistX)*TCos[DistDir])>>8)-(((yy-DistY)*TSin[DistDir])>>8);
					int ya=DistY+(((xx-DistX)*TSin[DistDir])>>8)+(((yy-DistY)*TCos[DistDir])>>8);
					//
					DestPoints[dd*2]=xa;
					DestPoints[dd*2+1]=ya;
					dd++;
					//
					/*
					AutoForm* Nr=NULL;
					int dst=9999999;
					for(int i=0;i<MaxP;i++)
						if(P[i])
							if(P[i]->AimX==-1)
							{
								int dd=Norma(P[i]->x-xa,P[i]->y-ya);
								if(dd<dst)
								{
									dst=dd;
									Nr=P[i];
								}
							}
					if(Nr)
					{
						Nr->AimX=xa;
						Nr->AimY=ya;
						if(DistDir!=-1)
							Nr->AimDir=DistDir;
					}
					*/
				}
			}
		}

		for(i=0;i<MaxP;i++)
		{
			if(P[i])
			{
				FromPoints[i*2]=P[i]->x;
				FromPoints[i*2+1]=P[i]->y;
			}
		}
		word* DestIndex=(word*)malloc(MaxP<<2);
		static word* T2=DestIndex;
		memset(DestIndex,0,MaxP<<2);
		FindShotWayPoint(MaxP,FromPoints,DestPoints,DestIndex);
		for(i=0;i<MaxP;i++)
		{
			if(P[i])
			{
				P[i]->AimX=DestPoints[DestIndex[i]*2];
				P[i]->AimY=DestPoints[DestIndex[i]*2+1];
				if(DistDir!=-1)
					P[i]->AimDir=DistDir;
			}
		}
		free(DestIndex);
		free(DestPoints);
		free(FromPoints);
	}
	/*
	int NonHorceAmo=0;
	int i;
	for(i=0;i<MaxP;i++)
		if(P[i]->Type!=2)
			NonHorceAmo++;
	if(NonHorceAmo&&MaxE)
	for(i=0;i<MaxP;i++)
	{
		if(P[i]->Type==2)
		{
			P[i]->AimX=-1;
			P[i]->AimY=-1;
			int MaxHorceDangID=-1;
			int MaxHorceDang=0;
			int MaxDangID=-1;
			int MaxDang=0;
			int IfMarshID=-1;
			int IfMarsh=999999999;
			for(int k=0;k<MaxP;k++)
			{
				if(P[k]->Type!=2)
				{
					int hd=P[k]->GetHorceDanger();
					if(hd<MaxHorceDang)
					{
						MaxHorceDang=hd;
						MaxHorceDangID=k;
					}
					if(P[k]->FireForceDifference()<MaxDang)
					{
						MaxDang=P[k]->FireForceDiffer
						e();
						MaxDangID=k;
					}
					if(hd<IfMarsh)
					{
						IfMarsh=hd;
						IfMarshID=k;
					}
				}
			}
			if(MaxHorceDangID>-1)
			{
				P[i]->AimX=P[MaxHorceDangID]->x;
				P[i]->AimY=P[MaxHorceDangID]->y;
			}
			else
			{
				if(MaxDangID>-1)
				{
					P[i]->AimX=P[MaxDangID]->x;
					P[i]->AimY=P[MaxDangID]->y;
				}
				else
				{
					if(IfMarshID>-1)
					{
						P[i]->AimX=P[IfMarshID]->x;
						P[i]->AimY=P[IfMarshID]->y;
					}
				}
			}
		}
	}
	*/
}
bool Regiment::IsChase()
{
	return Chase;
}
void Regiment::SetChaseOff()
{
	Chase=false;
}
void Regiment::SetChaseOn()
{
	Chase=true;
}
void Regiment::Save(SaveBuf *SB)
{
	xBlockWrite(SB,&Index,4);
	xBlockWrite(SB,&DistX,4);
	xBlockWrite(SB,&DistY,4);
	xBlockWrite(SB,&DistDir,4);
	xBlockWrite(SB,&TableFillTime,4);
	xBlockWrite(SB,&Chase,1);
	xBlockWrite(SB,&RemanGroup,sizeof(RemanGroup));
}
int Regiment::Load(SaveBuf *SB)
{
	xBlockRead(SB,&Index,4);
	if(Index!=-1)
	{
		xBlockRead(SB,&DistX,4);
		xBlockRead(SB,&DistY,4);
		xBlockRead(SB,&DistDir,4);
		xBlockRead(SB,&TableFillTime,4);
		xBlockRead(SB,&Chase,1);
		xBlockRead(SB,&RemanGroup,sizeof(RemanGroup));
	}
	return Index;
}
void Regiment::RemanPolkFrom(GAMEOBJ* Group)
{
	CopyUnits(Group,&RemanGroup,0,65535,1,1);
}
void Regiment::RemanProces()
{
	int n=GetTotalAmount0(&RemanGroup);
	if(n&&MaxP)
	{
		for(int i=0;i<MaxP;i++)
		{
			word type;
			int m=P[i]->GetNUnitsToReman(type);
			if(m)
			{
				RemoveNUnits(&P[i]->RemanUnits,&RemanGroup,m,type);
				//CopyUnits(&P[i]->RemanUnits,&RemanGroup,0,m,1,1);
			}
		}
		n=GetTotalAmount0(&RemanGroup);
		if(n)
		{
			int xx,yy;
			if(GetGrpCenter(&RemanGroup,&xx,&yy))
			{
				int avx=0;
				int avy=0;
				for(int i=0;i<MaxP;i++)
				{
					if(P[i])
					{
						avx+=P[i]->x;
						avy+=P[i]->y;
					}
				}
				avx/=MaxP;
				avy/=MaxP;
				int ds=Norma(avx-xx,avy-yy);
				if(ds>800)
				{
					//byte al=BSelf->Direction+128;
					//int xxx=avx+((500*TCos[al])>>8);
					//int yyy=avy+((500*TSin[al])>>8);
					SGP_MoveToPoint(P[0]->NI,&RemanGroup,avx,avy,512,0,0,0);
				}
			}
		}
	}
}
int Regiment::GetUnitsAmountInPolk(bool reman)
{
	int rez=0;
	for(int i=0;i<*MaxAFList;i++)
	{
		if(AFList[i]&&AFList[i]->RegimentIndex==Index)
			rez+=AFList[i]->GetUnitsAmount();
	}
	if(reman){
		rez+=GetTotalAmount0(&RemanGroup);
	}
	return rez;
}
//====================================================================//
//----------------------------------------------------------------------------------------------------------------//
History::History(float* Inn, int NIn, float* Outt, int NOut)
{
	vIn = new float[NIn];
	memcpy(vIn, Inn, NIn*sizeof(float));
	vOut = new float[NOut];
	memcpy(vOut, Outt, NOut*sizeof(float));
	Next=NULL;
}
History::~History()
{
	
	if(vIn)
	{
		delete[] vIn;
		vIn=NULL;
	}
	if(vOut)
	{
		delete[] vOut;
		vOut=NULL;
	}
	if(Next)
	{
		delete Next;
		Next=NULL;
	}
}
//----------------------------------------------------------------------------------------------------------------//
void NeuroInit::EvaluateFunction()
{
	Neuro* N  = get_parent<Neuro>();
	if(N)
	{
		BaseClass* B=N->GetParent();
		if(B)
		{
			ClonesArray<Neuro>* CAN =  dynamic_cast< ClonesArray<Neuro>*>(B);
			if(CAN)
			{
				BaseClass* BB=CAN->GetParent();
				if(B)
				{
					Neuro2AutoForm* NN =  dynamic_cast< Neuro2AutoForm*>(BB);
					if(NN)
					{
						N->Init(NN->InputParamIndex.GetAmount(),NN->OutputCommandIndex.GetAmount());
					}
				}
			}
		}
	}
}
//----------------------------------------------------------------------------------------------------------------//
void SaveHistory::EvaluateFunction()
{
	Neuro* N  = get_parent<Neuro>();
	if(N)
	{
		History* H = N->RezHistory;
		FILE* fp;
		if((fp=fopen("AI\\History.dump","ab"))!=NULL)
		{
			while (H) 
			{
				fwrite(H->vIn,sizeof(float),N->vNIn,fp);
				fwrite(H->vOut,sizeof(float),N->vNOut,fp);
				H=H->Next;
			}
			fclose(fp);
		}
	}
}
//----------------------------------------------------------------------------------------------------------------//
void LoadHistory::EvaluateFunction()
{
	Neuro* N = get_parent<Neuro>();
	if(N)
	{	
		History* H = N->RezHistory;
		FILE* fp;
		if((fp=fopen("AI\\History.dump","rb"))!=NULL)
		{
			float* vIn = new float[N->vNIn];
			float* vOut = new float[N->vNOut];
			while(!feof(fp))
			{
				fread(vIn,sizeof(float),N->vNIn,fp);
				fread(vOut,sizeof(float),N->vNOut,fp);
				History* H = new History(vIn,N->vNIn,vOut,N->vNOut);
				H->Next = N->RezHistory;
				N->RezHistory = H;
			}
			delete[] vIn;
			delete[] vOut;
			fclose(fp);
		}
	}
}
//----------------------------------------------------------------------------------------------------------------//
void CLearnNeuro::EvaluateFunction()
{
	Neuro* N = get_parent<Neuro>();
	if(N)
	{	
		N->Learn(N->MaxError,N->NIter,N->Step);
	}
}
//----------------------------------------------------------------------------------------------------------------//
void CMakeHistory::EvaluateFunction()
{
	Neuro* N = get_parent<Neuro>();
	if(N)
	{	
		//
	}
}
//----------------------------------------------------------------------------------------------------------------//
Neuro::Neuro()
{
	RezHistory=NULL;
}
Neuro::~Neuro()
{
	if(RezHistory)
	{
		delete RezHistory;
		RezHistory=NULL;
	}
}
void Neuro::Init(int NIn, int NOut)
{
	Clear();
	vNIn = NIn;
	vNOut = NOut;
	for(int i=0;i<NOut;i++)
	{
		LinearArray<float, _float>* Wi = new LinearArray<float, _float>;
		Wi->Add((float)1);
		for(int j=0;j<NIn;j++)
		{
			Wi->Add(float(32767-rando()*2)/(float)32767);
		}
		Matr.Add(Wi);
	}
}
bool Neuro::Get(float* Inn, float* Outt)
{
	for(int i=0;i<vNOut;i++)
	{
		float Sum = 0.0;
		LinearArray<float, _float>* Wi = Matr[i];
		Sum+=(*Wi)[0];
		for(int j=0;j<vNIn;j++)
		{
			Sum+=Inn[j]*(*Wi)[j+1];
		}
		const float Alfa=0.3;
		Outt[i]=1/(1+exp(-Alfa*Sum));
	}
	
	NGet++;
	return true;
}
float Neuro::Learn(float MaxErr, int MaxIter, float V)
{
	float rez=0.0;
	int ii=0;
	float* TOut = new float[vNOut];
	int MM=MaxIter;
	float Mee=MaxErr;
	for(ii=0;ii<MM;ii++)
	{
		History* H = RezHistory;
		rez=0.0;
		while (H) 
		{
			int rand = rando()%100;
			for(int i=0;i<rand&&H->Next;i++)
				H=H->Next;
			float sum=0.0;
			Get(H->vIn,TOut);
			for(int i=0;i<vNOut;i++)
			{
				if(H->vOut[i]>0||TOut[i]>0)
				{
					float Sigm = H->vOut[i] - TOut[i];
					sum+=abs(Sigm);

					LinearArray<float, _float>* Wi = Matr[i];
					(*Wi)[0] = (*Wi)[0] + V*Sigm*1;
					for(int j=0;j<vNIn;j++)
					{
						(*Wi)[j+1] = (*Wi)[j+1] + V*Sigm*H->vIn[j]; 
					}
				}
			}
			H=H->Next;
			rez=sum;
		}
		NIter=ii;
		MaxError=rez;
	}
	delete[] TOut;
	return rez;
}
float Neuro::Learn(float* Inn, float* Outt, float V)
{
	float rez=0.0;
	float* TOut = new float[vNOut];
	
	History* H = new History(Inn,vNIn,Outt,vNOut);
	H->Next = RezHistory;
	RezHistory = H;
	while (H) 
	{
		int rand = rando()%100;
		for(int i=0;i<rand&&H->Next;i++)
			H=H->Next;
		float sum=0.0;
		Get(H->vIn,TOut);
		for(int i=0;i<vNOut;i++)
		{
			if(H->vOut[i]>0||TOut[i]>0)
			{
				float Sigm = H->vOut[i] - TOut[i];
				sum+=Sigm;

				LinearArray<float, _float>* Wi = Matr[i];
				(*Wi)[0] = (*Wi)[0] + V*Sigm*1;
				for(int j=0;j<vNIn;j++)
				{
					(*Wi)[j+1] = (*Wi)[j+1] + V*Sigm*H->vIn[j]; 
				}
			}
		}
		H=H->Next;
		if(abs(sum)>abs(rez))
			rez=sum;
	}
	delete[] TOut;
	NLessons++;
	return rez;
}
void Neuro::Clear()
{
	Matr.Clear();
	vNIn = 0;
	vNOut = 0;
	if(RezHistory)
	{
		delete RezHistory;
		RezHistory = NULL;
	}
	NLessons=0;
	NGet=0;
	NGood=0;
	NBad=0;
}
void Neuro::YouAreGood(float LearnV)
{
	NGood++;
}
void Neuro::YouAreBad(float LearnV)
{
	NBad++;
}
//----------------------------------------------------------------------------------------------------------------//
Neuro2AutoForm::Neuro2AutoForm()
{
}
bool Neuro2AutoForm::AutoFormProcess(AutoForm* A, float V)
{
	bool rez = false;
	if(A)
	{
		int NNl = NeuroList.GetAmount();
		if(NNl>0)
		{
			if(A->NeroNameIndex==-1)
			{
				int n = UnitsType.GetAmount();
				word UnitNindex=0xFFFF;
				if(A->BSelf)
					UnitNindex = BrigadeAI::GetBrigadeNIndex(A->BSelf);
				if(A->C)
					UnitNindex = A->C->NIndex;
				for(int i=0;i<n;i++)
				{
					if(UnitsType[i]->UnitType==UnitNindex)
					{
						A->NeroNameIndex=0;
						break;
					}
				}
			}
			if(A->NeroNameIndex!=-1)
			{
				rez = true;
				Neuro* Ne = NeuroList[A->NeroNameIndex];
				
				int NIn = InputParamIndex.GetAmount();
				int NOut = OutputCommandIndex.GetAmount();
				
				if(NIn>0&&NOut>0)
				{
					float* Inn = new float[NIn];
					float* Outt = new float[NOut];

					for(int i=0;i<NIn;i++)
					{
						//Inn[i] = A->GetSituationByIndex(InputParamIndex[i]);
						Inn[i] = (float)A->GetSituationByIndex(InputParamIndex[i])/
									(float)A->GetMaxValueSituationByIndex(InputParamIndex[i]);
					}
					if(EngSettings.NeuroLearn)
					{
						memset(Outt,0,NOut*sizeof(float));
						int MaxAction = A->GetMaxActions();
						
						for(int i=0;i<NOut;i++)
						{
							if(MaxAction==-1&&OutputCommandIndex[i]==24)//DoNothing
							{
								Outt[i]=0.98;
								break;
							}
							else
							if(OutputCommandIndex[i]==MaxAction)
							{
								Outt[i]=0.98;
								break;
							}
						}
						A->NeuroTempInaccuracy=Ne->Learn(Inn,Outt,V);
					}
					else
					if(EngSettings.NeuroUse)
					{
						memset(A->ActionsArr,0,128*sizeof(int));
						Ne->Get(Inn,Outt);
						for(int i=0;i<NOut;i++)
						{
							A->ActionsArr[OutputCommandIndex[i]]=Outt[i]*1000;
						}
					}
				}
			}
		}
	}
	return rez;
}
//----------------------------------------------------------------------------------------------------------------//
CNeuroStorage::CNeuroStorage()
{
	V=0.3;
}
int CNeuroStorage::AutoFormProcess(AutoForm* A)
{
	int NTy = GenoFond.GetAmount();
	if(!IsLoad&&NTy==0)
	{
		LoadFromFile();
		IsLoad=true;
	}
	if(A)
	{
		if(A->NeroTypeIndex==-1)
		{
			for(int i=0;i<NTy;i++)
			{
				if(GenoFond[i]->AutoFormProcess(A,V))
				{
					A->NeroTypeIndex=i;
					break;
				}
			}
		}
		else
		if(A->NeroTypeIndex<NTy)
		{
			GenoFond[A->NeroTypeIndex]->AutoFormProcess(A,V);
		}
	}
	return -1;
}
bool CNeuroStorage::SetNeroMatrix(AutoForm* A, const char* Name)
{
	return false;
}
void CNeuroStorage::LoadFromFile()
{
	//SafeReadFromFile("AI\\NeuroStorage.xml");
	ReadFromFile("AI\\NeuroStorage.xml");
}
//----------------------------------------------------------------------------------------------------------------//
/*
int CTempAutoForm::GetSituationByIndex(int I)
{
	if(m_pH)
	{
	}
}
*/
//----------------------------------------------------------------------------------------------------------------//
//====================================================================//
extern BrigadeAI WiselyFormations;
extern TacticalAI T_AI;
void CheckAllLinks();
void KeyTestMem();
void WiselyFormationsProcess()
{	
	RunPF(256,"BrigAI");
	if(EngSettings.EnableTacticalAI)
		T_AI.Process();
	WiselyFormations.Process();
	StopPF(256);
}
void AddBrigadeInWiselyFormations(int Nn)
{
	//if(B)
	{
		int reg=WiselyFormations.CreateRegiment();
		WiselyFormations.Reg[reg]->AddSelectedToPolk(Nn);
		/*for(int j=0;j<MaxBrig;j++)
		{
			Brigade* BR=&(CITY[NI].Brigs[j]);
			if(BR->Enabled&&BR->WarType!=0)
				WiselyFormations.AddAutoBrigade(BR);
		}
		*/
	}
}
bool g_BrigadeAttackPoint(int NI, word index, int x, int y, const char *rulesFileName=NULL)
{
	if(NI>-1&&NI<8&&index<MaxBrig)
	{
		Brigade* BR=&(CITY[NI].Brigs[index]);
		if(BR)
		{
			int in= BrigadeAI::AddAutoBrigade(BR);
			if(in>-1)
			{
				BrigadeAI::AF[in]->AimX=x;
				BrigadeAI::AF[in]->AimY=y;
				if(rulesFileName)
				{
					if(strstr(rulesFileName,"\\"))
						AssignHint1((char*)rulesFileName,100);

					word UnitNIndex=BrigadeAI::GetBrigadeNIndex(BR);
					int n=BrigadeAI::AllRules.GetAmount();
					for(int i=0;i<n;i++)
					{
						if(BrigadeAI::AllRules[i]&&BrigadeAI::AllRules[i]->m_fileName.pchar())
						{
							if(!stricmp(BrigadeAI::AllRules[i]->m_fileName.pchar(),rulesFileName))
							{
								int m=BrigadeAI::AllRules[i]->UnitsT.GetAmount();
								for(int j=0;j<m;j++)
								{
									if(BrigadeAI::AllRules[i]->UnitsT[j]==UnitNIndex)
									{
										BrigadeAI::AF[in]->RulesID=BrigadeAI::AllRules[i]->Index;
										BrigadeAI::AF[in]->RulesList=BrigadeAI::AllRules[i];
										break;
									}
								}
							}
						}
					}
				}
				return true;
			}	
		}
	}
	return false;
}
void ShowAddInfo(int x,int y)
{
	for(int i=0;i<WiselyFormations.MaxAF;i++)
	{
		AutoForm* FF=WiselyFormations.AF[i];
		if(FF&&FF->Type!=0)
			FF->ShowInfo();
	}
	T_AI.ShowInfo();
}
CEXPORT int CreatePolk()
{
	return WiselyFormations.CreateRegiment();
}
CEXPORT Polk* GetPolkByIndex(int PIndex)
{
	return WiselyFormations.GetPolkByIndex(PIndex);
}
void NewBrigadeIsBorn(Brigade* B)
{
	WiselyFormations.NewBrigadeIsBorn(B);
	T_AI.NewBrigadeIsBorn(B);
}
void BrigadeAIUnLoading()
{
	T_AI.ClearAll();
	BrigadeAI::UnLoading();
}
void LoadBrigadeAI(SaveBuf* SB)
{
	BrigadeAIUnLoading();
	WiselyFormations.Load(SB);
	T_AI.LoadSB(SB);
}
void SaveBrigadeAI(SaveBuf* SB)
{
	WiselyFormations.Save(SB);
	T_AI.SaveSB(SB);
}
CEXPORT void ActivateTacticalAI(byte NI)
{
	T_AI.Activate(NI);
}
CEXPORT void StopTacticalAI(byte NI)
{
	T_AI.Stop(NI);
}
bool GetNewTowerCoord(byte NI, int &x, int &y)
{
	return T_AI.GetNewTowerCoord(NI,x,y);
}
DLLEXPORT int GetMyNation();
CEXPORT void AddPulseSquare(int x, int y);
bool AddPulseSquare(int x, int y, int deadRadius, int actulAnimTime);
void BrigadeAlarmEvent()
{
	int n = WiselyFormations.AllForm.GetAmount();
	for(int i=0;i<n;i++)
	{
		Atom* A=WiselyFormations.AllForm[i];
		if(A&&A->BSelf&&A->NI==GetMyNation()&&A->IsAlive())
		{
			//static int GlobalLastTime=0;
			if(/*(GlobalLastTime+3*20*256)<AnimTime&&*/A->CheckTimeNearestEnmBrigade<AnimTime)
			{
				Vector3D point(A->x,A->y,0);			
				if((A->LastAlarmTime+2*20*256)<AnimTime)
				{
					int maxx=65535;
					word maxenm=0xFFFF;
					Atom* EN = NULL;
					for(int j=0;j<n;j++)
					{
						Atom* E=WiselyFormations.AllForm[j];
						if(E&&E->BSelf&&E->IsAlive()/*&&E->GetStandTime()==0*/&&!(A->GetNMask()&E->GetNMask())&&E->NI!=7)
						{
							int d=Norma(A->x-E->x,A->y-E->y);  
							if(d<maxx)
							{
								maxx=d;
								maxenm=E->BSelf->ID;
								EN=E;
							}
						}
					}
					if(EN)
					{
						if(A->NearestEnmBrigade!=maxenm)
						{
							A->DistToNearestEnmBrigade=32000;
						}
						bool isAlarm=false;
						extern int ActCommand;
						if(maxx<EngSettings.BrigadeAlarmDist1&&A->DistToNearestEnmBrigade>EngSettings.BrigadeAlarmDist1)
						{
							isAlarm=true;
							ActCommand=43;
						}
						if(maxx<EngSettings.BrigadeAlarmDist2&&A->DistToNearestEnmBrigade>EngSettings.BrigadeAlarmDist2)
						{
							isAlarm=true;
							ActCommand=44;
						}
						if(maxx<EngSettings.BrigadeAlarmDist3&&A->DistToNearestEnmBrigade>EngSettings.BrigadeAlarmDist3)
						{
							isAlarm=true;
							ActCommand=45;
						}
						if(maxx>EngSettings.BrigadeAlarmDist1)A->DistToNearestEnmBrigade=maxx;
						if(isAlarm)
						{
							//void PlaySound(char* Name);
							//PlaySound("VIDIM_VRAGA");
							A->LastAlarmTime=AnimTime;
							//GlobalLastTime=AnimTime;
							A->NearestEnmBrigade=maxenm;
							A->DistToNearestEnmBrigade=maxx;
							//if(!IMM->IsPointVisible(point))
								//AddPulseSquare(A->x, A->y, EngSettings.BrigadeAlarmRadius, EngSettings.BrigadeAlarmRadiusTime*20*256);

							//AddPulseSquare(A->x,A->y);
						}
						ActCommand=0;
						/*
						if(maxx<EngSettings.BrigadeAlarmDist1)
						{
							if((A->NearestEnmBrigade==maxenm&&maxx<A->DistToNearestEnmBrigade)||
								A->NearestEnmBrigade!=maxenm)
							{
								void PlaySound(char* Name);
								PlaySound("VIDIM_VRAGA");
								A->LastAlarmTime=AnimTime;
								GlobalLastTime=AnimTime;
								A->NearestEnmBrigade=maxenm;
								A->DistToNearestEnmBrigade=maxx;
								AddPulseSquare(A->x,A->y);
							}
						}
						*/
					}
					A->CheckTimeNearestEnmBrigade=AnimTime+5*20*256*min(1000,maxx)/1000;
				}
			}
		}
	}
}
byte GetBrigadeDirectionByUnitPositions(Brigade* BR)
{
	byte rez=0;
	if(BR)
	{
		if(BR->lastTimeGetCenter==tmtmt)
			return BR->lastDir;

		OrderDescription* ODS=ElementaryOrders+BR->WarType-1;
		int NA=ODS->NActualLines;
		int FL=ODS->FirstActualLine;
		int bp=NBPERSONAL;
		int CP=0;

		int NLine=0;
		int Interval=FORMDIST*FORMDIST/(256*16);

		int cx=0;
		int cy=0;
		BR->GetCenter(&cx,&cy);

		int bcx = NA*Interval/2;
		int bcy=0;

		int RX=0;
		int RY=0;
		
		for(int p=0;p<NA;p++)
		{
			int NU=ODS->LineNU[p+FL];
			if(NU)
			{
				short* Line = ODS->Lines[p+FL];
				for(int q=0;q<NU;q++)
				{
					if(bp<BR->NMemb)
					{
						word ID=BR->Memb[bp];
						if(ID!=0xFFFF)
						{
							OneObject* OB = Group[ID];
							if(OB)
							{
								int bRad = Norma(p*Interval-bcx,Line[q]*Interval);
								char bDir = GetDir(p*Interval-bcx,Line[q]*Interval);
							
								byte Dir = GetDir(OB->RealX/16-cx,OB->RealY/16-cy);

								byte RezDir = Dir + bDir;
								RezDir+=128;
								int RezX = (bRad*TCos[RezDir])>>8;
								int RezY = (bRad*TSin[RezDir])>>8;

								RX+=RezX;
								RY+=RezY;
							}
						}
					}
					bp++;
				}
			}
		}
		rez=GetDir(RX,RY);
		BR->lastTimeGetCenter=tmtmt;
		BR->lastCenterX=cx;
		BR->lastCenterY=cy;
		BR->lastDir=rez;
	}
	return rez;
}
//====================================================================//
#else

void WiselyFormationsProcess()
{}
void AddBrigadeInWiselyFormations(Brigade* B)
{}
void NewBrigadeIsBorn(Brigade* B)
{}
#endif COSSACKS2