#include "stdheader.h"
//==================================================================================================================//
#include "TacticalAI.h"
//------------------------------------------------------------------------------------------------------------------//
extern int tmtmt;
int GetNRoads();
int GetNRoadsLink(int index);
DLLEXPORT bool GetTopZoneCoor(int Zone,int* x,int* y);
extern BrigadeAI WiselyFormations;
extern int GetTopDistance(int xa,int ya,int xb,int yb,int LT);
CEXPORT int CreatePolk();
CEXPORT Polk* GetPolkByIndex(int PIndex);
DLLEXPORT int GetAmountOfZones();
DLLEXPORT int GetZoneX(int Index);
DLLEXPORT int GetZoneY(int Index);
extern void xBlockRead(SaveBuf* SB,void* Data,int Size);
extern void xBlockWrite(SaveBuf* SB,void* Data,int Size);

//------------------------------------------------------------------------------------------------------------------//
OneMPProperty::OneMPProperty()
{
}
OneMPProperty::~OneMPProperty()
{
}
//------------------------------------------------------------------------------------------------------------------//
MPProperty::MPProperty()
{
}
MPProperty::~MPProperty()
{
}
//------------------------------------------------------------------------------------------------------------------//
NationMainPointInfo::NationMainPointInfo()
{
	RegisteredPolk=0xFFFF;
	TWeight=0;
	HWeight=0;
	Tw=0;
	Hw=0;
	EnemyForce=0;
	EnemyForcePr=0;
	FriendlyForce=0;
	FriendlyForcePr=0;
}
NationMainPointInfo::~NationMainPointInfo()
{
}
void NationMainPointInfo::ShowInfo(int x, int y)
{
	char st[256];
	sprintf(st,"%d:(T:%d(%d) H:%d(%d)",NI,Tw,EnemyForcePr,Hw,FriendlyForcePr);
	ShowString(x-(mapx<<5),y/2-GetHeight(x,y)-(mapy<<4),st,&YellowFont);
}
//------------------------------------------------------------------------------------------------------------------//
UnitImpact::UnitImpact()
{
}
UnitImpact::~UnitImpact()
{
}
UnitImpact::UnitImpact(Atom* A)
{
	AtomIndex=A->AtomIndex;
	AtomSerial=A->Serial;
}
//------------------------------------------------------------------------------------------------------------------//
MainPoint::MainPoint()
{
	x=-1;
	y=-1;
	Radius=-1;
	MPZoneIndex=-1;
	Amount=0;;
	Type=-1;
	ZoneIndex=0xFFFF;
	SeloIndex=0xFFFF;
	BildIndex=0xFFFF;
}
MainPoint::~MainPoint()
{
	/*
	int n=NationMPInfo.GetAmount();
	for(int i=0;i<n;i++)
		if(NationMPInfo[i])
			delete (NationMPInfo[i]);
			*/
	//NationMPInfo.Clear();
}
int MainPoint::GetTopDistToAtom(word AtomIndex)
{
	int rez=-1;
	if(WiselyFormations.AllForm.GetAmount()>AtomIndex)
	{
		//int n=UnitsImpact.GetAmount();
		bool find=false;
		Atom* A = WiselyFormations.AllForm[AtomIndex];
		/*
		for(int i=0;i<n;i++)
		{
			if(UnitsImpact[i]&&UnitsImpact[i]->AtomIndex==AtomIndex)
			{
				if(A&&A->IsAlive()&&A->Serial==UnitsImpact[i]->AtomSerial)
				{
					find=true;
					if((UnitsImpact[i]->LastCheckTime+CHECKTIMEDELAY)<tmtmt)
					{
						rez=UnitsImpact[i]->TopDist;
					}
					else
					{
						word LockType = A->GetLockType();
						//int TopDist=GetTopDistance(A->x>>6,A->y>>6,x>>6,y>>6,LockType)<<6;
						int TopDist=Norma(A->x-x,A->y-y);
						rez=TopDist;
						UnitsImpact[i]->TopDist=TopDist;
						UnitsImpact[i]->LastCheckTime=tmtmt;
					}
					break;
				}
			}
		}
		*/
		if(!find)
		{
			rez=RegisterAtom(A);
		}
	}
	return rez;
}
int MainPoint::RegisterAtom(Atom* A)
{
	int rez=-1;
	if(A)
	{
		/*
		int n=UnitsImpact.GetAmount();
		bool find=false;
		for(int i=0;i<n;i++)
		{
			if(UnitsImpact[i]&&UnitsImpact[i]->AtomIndex==A->AtomIndex)
			{
				if(A->Serial==UnitsImpact[i]->AtomSerial)
				{
					find=true;
					word LockType = A->GetLockType();
					int TopDist=-1;
					if(A->GetStandTime()>0||(UnitsImpact[i]->LastCheckTime+CHECKTIMEDELAY)<tmtmt)
					{
						TopDist=UnitsImpact[i]->TopDist;
					}
					else
					{
						TopDist=GetTopDistance(A->x>>6,A->y>>6,x>>6,y>>6,LockType)<<6;
						UnitsImpact[i]->TopDist=TopDist;
						UnitsImpact[i]->LastCheckTime=tmtmt;
					}
					UnitsImpact[i]->TopDist=TopDist;
					UnitsImpact[i]->LastCheckTime=tmtmt;
					rez=TopDist;
					break;
				}
				else
				{
					delete (UnitsImpact[i]);
					UnitsImpact[i]=NULL;
				}
			}
		}
		if(!find)
		{
			int n=UnitsImpact.GetAmount();
			bool insert=false;
			word LockType = A->GetLockType();
			int TopDist=GetTopDistance(A->x>>6,A->y>>6,x>>6,y>>6,LockType)<<6;

			UnitImpact* UI = new UnitImpact(A);
			UI->TopDist=TopDist;
			UI->LastCheckTime=tmtmt+rando()%100;
			UI->AtomIndex=A->AtomIndex;
			UI->AtomSerial=A->Serial;

			rez=TopDist;

			for(int i=0;i<n;i++)
			{
				if(UnitsImpact[i]==NULL)
				{
					insert=true;
					UnitsImpact[i]=UI;
					break;
				}
			}
			if(!insert)
			{
				UnitsImpact.Add(UI);
			}
		}
		*/
		rez=Norma(A->x-x,A->y-y);
	}
	return rez;
}
int MainPoint::GetOwner()
{
	int rez=-1;
	if(Type==2)
	{
		DIP_SimpleBuilding** Sela;
		int NSel;
		GetDipSimpleBuildings(NSel, Sela);
		DIP_SimpleBuilding* Selo = Sela[SeloIndex];
		rez=NATIONS[Selo->Owner].NMask;
	}
	if(Type==3)
	{
		if(BildIndex!=0xFFFF)
		{
			OneObject* OB=Group[BildIndex];
			if(OB&&(!OB->Sdoxlo))
			{
				rez=OB->NMask;
			}
		}
	}
	return -1;
}
//------------------------------------------------------------------------------------------------------------------//
word* NationalAI::wh = NULL;
//------------------------------------------------------------------------------------------------------------------//
NationalAI::NationalAI()
{
	NationMPInfo.Add(NULL,TacticalAI::MainPoints.GetAmount());
	FirstStart=false;
}
NationalAI::~NationalAI()
{
	int k=TempSet.GetAmount();
	for(int j=0;j<k;j++)
	{
		if(TempSet[j])
			delete(TempSet[j]);
	}
	TempSet.Clear();
	NationMPInfo.Clear();
}
NationMainPointInfo* NationalAI::GetMPInfo(int MPIndex)
{
	NationMainPointInfo* rez=NULL;
	int n = NationMPInfo.GetAmount();
	int mm = TacticalAI::MainPoints.GetAmount();
	if(MPIndex<mm)
	{
		if(n<=MPIndex)
		{
			NationMPInfo.Add(NULL,mm-n+1);
		}
		rez = NationMPInfo[MPIndex];
		if(!rez)
		{
			rez = new NationMainPointInfo();
			NationMPInfo[MPIndex] = rez;
		}
	}
	return rez;
}
void NationalAI::SetMPInfo(int MPIndex,NationMainPointInfo* PI)
{
	int n = NationMPInfo.GetAmount();
	if(MPIndex<n)
	{
		NationMPInfo[MPIndex]=PI;
	}
}
void NationalAI::ClearPolkRegistration()
{
	int n=PolkList.GetAmount();
	int i;
	//for(i=0;i<n;i++)
	{
		int m = WiselyFormations.MaxAF;
		for(int j=0;j<m;j++)
		{
			AutoForm* AF = WiselyFormations.AF[j];
			if(AF&&AF->IsAlive())
			{
				if(AF->NI==NI)//if(AF->NI==NI&&AF->RegimentIndex==PolkList[i])
				{
					AF->RegimentIndex=-1;
				}
			}
		}
	}
	n = TacticalAI::MainPoints.GetAmount();
	for(i=0;i<n;i++)
	{
		NationMainPointInfo* MPI = GetMPInfo(i); 
		MPI->RegisteredPolk=0xFFFF;
	}
}
void NationalAI::FillMainPointsOrder()
{
	int n= TacticalAI::MainPoints.GetAmount();
	MainForceWeight.Clear();
	MainForceWeight.Add(0,n);
	MainPointsOrder.Clear();
	for(int i=0;i<n;i++)
	{
		int Mas = TacticalAI::MainPoints[i]->GetOwner();
		byte maska=0;
		if(Mas!=-1)maska=(byte)Mas;
		NationMainPointInfo* MPI = GetMPInfo(i); 
		MainForceWeight[i]=0;
		//if(MPI&&((MPI->FriendlyForcePr&&((MPI->EnemyForcePr*100)/MPI->FriendlyForcePr)>30)||MPI->EnemyForce>0))  
		if(TacticalAI::MainPoints[i]->Amount!=0)//GEC LAZAR CHANGE
		{
			//int w1=(MPI->Hw*MPI->EnemyForcePr)/1000;
			//int w2=MPI->Tw; 
			
			int w1=((MPI->Hw+MPI->Tw)*MPI->EnemyForcePr)/100;
			int w2=0;
			if(Mas!=-1&&!(maska&NATIONS[NI].NMask))
			{
				w2=MPI->Tw;
			}
			/*
			if((((MPI->EnemyForcePr*100)/(MPI->FriendlyForcePr+1))<50&&Mas==-1)||(Mas!=-1&&(maska&NATIONS[NI].NMask)))
			{
				//w1=0;
				w2=0;
			}
			*/
			MainForceWeight[i]=w1+w2;
			if(MPI->Hw==10000||MPI->Tw==10000)
			{
				MainForceWeight[i]=100*(MPI->EnemyForcePr+1);
			}
			/*
			int w1=MPI->Hw*MPI->EnemyForcePr;
			int w2=MPI->Tw*MPI->FriendlyForcePr;
			MainForceWeight[i]=w1+w2;
			*/
			/*
			if(w1>w2)
				MainForceWeight[i]=w1;
			else
				MainForceWeight[i]=w2;
				*/
			//MainForceWeight[i]=(MPI->Tw+MPI->Hw)*abs(MPI->EnemyForcePr-MPI->FriendlyForcePr);
			//MainForceWeight[i]=abs(MPI->Hw*MPI->EnemyForcePr-MPI->Tw*MPI->FriendlyForcePr);
			//int dev=MPI->FriendlyForcePr;
			//if(!dev)dev=1;
			//MainForceWeight[i]=((MPI->Hw+MPI->Tw)*MPI->EnemyForcePr)/dev;
			/*
			int dev=MPI->Tw*MPI->FriendlyForcePr;
			if(!dev)dev=1;
			MainForceWeight[i]=(MPI->Hw*MPI->EnemyForcePr*100)/dev;
			*/
			/*
			int dev=abs(MPI->EnemyForcePr-MPI->FriendlyForcePr);
			if(!dev)dev=1;
			MainForceWeight[i]=((MPI->Tw+MPI->Hw)*MPI->EnemyForcePr)/dev;
			*/
			/*
			if(MPI->EnemyForce>MPI->FriendlyForce)
				MainForceWeight[i]=MPI->Hw*MPI->EnemyForcePr;
			else
				MainForceWeight[i]=MPI->Tw*MPI->FriendlyForcePr;
				*/
		}
		MainPointsOrder.Add(i);
	}
	wh = (word*) MainForceWeight.GetValues();
	qsort((void*)MainPointsOrder.GetValues(),n,sizeof(word),&NationalAI::compare);
}
int NationalAI::compare(const void *p1,const void *p2)
{
	word* i1 = (word*) p1;
	word* i2 = (word*) p2;
	return wh[*i2]-wh[*i1];
}
void NationalAI::SetPolkRegistration()
{
	MainPSetForce.Clear();
	//Clear nearest
	int n= TacticalAI::MainPoints.GetAmount();
	MainPSetForce.Add(0,n);
	for(int i=0;i<n;i++)
	{
		if(MainForceWeight[MainPointsOrder[i]])
		{
			int xx = TacticalAI::MainPoints[MainPointsOrder[i]]->x;
			int yy = TacticalAI::MainPoints[MainPointsOrder[i]]->y;
			for(int j=0;j<n;j++)
			{
				if(MainForceWeight[j]&&j!=MainPointsOrder[i])
				{
					int x2 = TacticalAI::MainPoints[j]->x;
					int y2 = TacticalAI::MainPoints[j]->y;
					if(Norma(xx-x2,yy-y2)<1101)
					{
						MainForceWeight[j]=0;
					}
				}
			}
		}
	}
	//
	MaxPolk=32;
	//
	byte MaxNActivPolk=0;
	for(i=0;i<n&&MaxNActivPolk<MaxPolk;i++)
	{
		if(MainForceWeight[MainPointsOrder[i]])
		{
			byte np = PolkList.GetAmount();
			Polk* P = NULL;
			if(np>MaxNActivPolk)
			{
				P = GetPolkByIndex(PolkList[MaxNActivPolk]);
			}
			else
			{
				int np = CreatePolk();
				PolkList.Add(np);
				P = GetPolkByIndex(PolkList[MaxNActivPolk]);
			}
			if(P)
			{
				int xx=TacticalAI::MainPoints[MainPointsOrder[i]]->x;
				int yy=TacticalAI::MainPoints[MainPointsOrder[i]]->y;
				P->MovePolkTo(xx,yy);
				NationMainPointInfo* MPI = GetMPInfo(MainPointsOrder[i]); 
				MPI->RegisteredPolk=P->GetPolkIndex();
				MaxNActivPolk++;
			}
		}
	}
	//
}
void NationalAI::SetBrigade2Polk()
{
	int k=TempSet.GetAmount();
	for(int j=0;j<k;j++)
	{
		if(TempSet[j])
			delete(TempSet[j]);
	}
	TempSet.Clear();
	int n= TacticalAI::MainPoints.GetAmount();
	int SumDang=0;
	for(int i=0;i<n;i++)
	{
		if(MainForceWeight[i])
		{
			NationMainPointInfo* MPI = GetMPInfo(i); 
			int m = WiselyFormations.MaxAF;
			SumDang+=MPI->EnemyForcePr;
			for(int j=0;j<m;j++)
			{
				AutoForm* AF = WiselyFormations.AF[j];
				if(AF&&AF->IsAlive()&&AF->NI==NI&&AF->AtomIndex!=0xFFFF&&(AF->BSelf==NULL||(AF->BSelf&&AF->BSelf->AutoFormID!=0xEEEE)))
				{
					//int dst=TacticalAI::MainPoints[i]->GetTopDistToAtom(AF->AtomIndex);
					int dst=TacticalAI::MainPoints[i]->RegisterAtom(AF);
					if(dst>-1)
					{
						int w=0;
						//int MaxDist=6000;
						//if(dst>0&&dst<MaxDist)
							w=MainForceWeight[i]*1000/(min(dst,500));//changed by Drew
							//w=(MainForceWeight[i]*(MaxDist-dst))/MaxDist;//AF->Force need Speed
						Polk2Point *PP = new Polk2Point();
						PP->W=w;
						PP->AFIndex=AF->Index;
						PP->PointIndex=i;
						PP->PolkIndex=MPI->RegisteredPolk;
						TempSet.Add(PP);
					}
					else
					{
						int gg=44;
					}
				}
			}
		}
	}
	qsort(TempSet.GetValues(),TempSet.GetAmount(),sizeof(Polk2Point*),&NationalAI::comPolk2Point);
	//
	n=TempSet.GetAmount();
	for(i=0;i<n;i++)
	{
		AutoForm* AF = WiselyFormations.AF[TempSet[i]->AFIndex];
		int EnFor=GetMPInfo(TempSet[i]->PointIndex)->EnemyForce;
		if(AF->BSelf&&AF->RegimentIndex==-1&&TempSet[i]->PolkIndex!=0xFFFF)// tolko otradi
		{
			if(MainPSetForce[TempSet[i]->PointIndex]==0||MainPSetForce[TempSet[i]->PointIndex]<EnFor)
			{
				AF->RegimentIndex=TempSet[i]->PolkIndex;
				AF->AimX=TacticalAI::MainPoints[TempSet[i]->PointIndex]->x;
				AF->AimY=TacticalAI::MainPoints[TempSet[i]->PointIndex]->y;
				MainPSetForce[TempSet[i]->PointIndex]+=AF->Force;
			}
		}
	}
	for(i=0;i<n;i++)
	{
		AutoForm* AF = WiselyFormations.AF[TempSet[i]->AFIndex];
		int EnFor=GetMPInfo(TempSet[i]->PointIndex)->EnemyForce;
		if(AF->RegimentIndex==-1&&TempSet[i]->PolkIndex!=0xFFFF)
		{
			bool cset = true;
			if(AF->C)
			{
				cset=false;
				for(int k=0;k<WiselyFormations.MaxAF;k++)
				{
					AutoForm* AFp = WiselyFormations.AF[k];
					if(AFp&&AFp->BSelf&&AFp->RegimentIndex==TempSet[i]->PolkIndex)
					{
						cset=true;
						break;
					}
				}
			}
			if(cset)
			{
				AF->RegimentIndex=TempSet[i]->PolkIndex;
				AF->AimX=TacticalAI::MainPoints[TempSet[i]->PointIndex]->x;
				AF->AimY=TacticalAI::MainPoints[TempSet[i]->PointIndex]->y;
				MainPSetForce[TempSet[i]->PointIndex]+=AF->Force;
			}
		}
	}

}
int NationalAI::comPolk2Point(const void *p1,const void *p2)
{
	Polk2Point* s1 = *(Polk2Point**) p1;
	Polk2Point* s2 = *(Polk2Point**) p2;
	return s2->W-s1->W;
}
void NationalAI::Process()
{
	if(!FirstStart)
	{
		int v=WiselyFormations.AllForm.GetAmount();
		for(int i=0;i<v;i++)
		{
			Atom* A=WiselyFormations.AllForm[i];
			if(A&&A->IsAlive()&&A->NI==NI)
			{
				if(A->BSelf)
					WiselyFormations.AddAutoBrigade(A->BSelf);
				else
				if(A->C)
					WiselyFormations.AddAutoCannon(A->C);
			}
		}
		FirstStart=true;
	}
	ClearPolkRegistration();
	FillMainPointsOrder();
	SetPolkRegistration();
	SetBrigade2Polk();
}
void NationalAI::NewAutoForm(AutoForm* AF)
{
	if(AF&&AF->IsAlive()&&(AF->BSelf==NULL||(AF->BSelf&&AF->BSelf->AutoFormID!=0xEEEE)))
	{
		if(AF->RegimentIndex!=-1)
		{
			//int n=TacticalAI::MainPoints.GetAmount();
			int n=MainForceWeight.GetAmount();
			int max=0;
			int maxPIndex=-1;
			if(n)
			{
				for(int i=0;i<n;i++)
				{
					if(MainForceWeight[i])
					{
						
						int dst=TacticalAI::MainPoints[i]->GetTopDistToAtom(AF->AtomIndex);
						int w=0;
						int MaxDist=6000;
						int Dist=500;
						if(dst>0&&dst<MaxDist)
							w=(MainForceWeight[i]*(MaxDist-dst))/(tmax(MaxDist,Dist)); //AF->Force*			
						if(w>max)
						{
							max=w;
							maxPIndex=i;
						}
					}
				}
				if(maxPIndex>0)
				{
					AF->RegimentIndex=maxPIndex;
					AF->AimX=TacticalAI::MainPoints[maxPIndex]->x;
					AF->AimY=TacticalAI::MainPoints[maxPIndex]->y;
				}
			}
		}
	}
}
void NationalAI::ShowInfo(int YShift)
{
	int n= TacticalAI::MainPoints.GetAmount();
	int m=MainForceWeight.GetAmount();
	for(int i=0;i<n;i++)
	{
		NationMainPointInfo* MPI = GetMPInfo(i); 
		MPI->ShowInfo(TacticalAI::MainPoints[i]->x,TacticalAI::MainPoints[i]->y+YShift);
		char st[256];
		if(i<m)
		{
			sprintf(st,"W:(%d)",MainForceWeight[i]);
			int xx = TacticalAI::MainPoints[i]->x+30;
			int yy = TacticalAI::MainPoints[i]->y+YShift+25;
			ShowString(xx-(mapx<<5),yy/2-GetHeight(xx,yy)-(mapy<<4),st,&YellowFont);
		}
	}	
}
void NationalAI::SaveSB(SaveBuf *SB)
{
	xBlockWrite(SB,&NI,4);
	xBlockWrite(SB,&Active,1);
	byte np=PolkList.GetAmount();
	xBlockWrite(SB,&np,1);
	for(int i=0;i<np;i++)
	{
		xBlockWrite(SB,&(PolkList[i]),2);
	}
}
void NationalAI::LoadSB(SaveBuf *SB)
{
	xBlockRead(SB,&NI,4);
	xBlockRead(SB,&Active,1);
	byte np=0;
	xBlockRead(SB,&np,1);
	PolkList.Clear();
	for(int i=0;i<np;i++)
	{
		word tmp;
		xBlockRead(SB,&tmp,2);
		PolkList.Add(tmp);
	}
}
//------------------------------------------------------------------------------------------------------------------//
ClassArray<MainPoint> TacticalAI::MainPoints;
ClassArray<NationalAI> TacticalAI::ActiveAI;
//------------------------------------------------------------------------------------------------------------------//
TacticalAI::TacticalAI(void)
{
	FirstStart=false;
	StartAI=false;
	IsResearchMap=false;
	ProcessTime=0;
	MaxPointDistBetweenMP=0;
	NationalProcessIndex=0;
	//MPIndex=0;
}
TacticalAI::~TacticalAI(void)
{
	ClearAll();
}
void TacticalAI::ResearchMap()
{
	MainPoints.Clear();

	int nn=GetAmountOfZones();
	for(int i=0;i<nn;i++)
	{
		AiImportantPoint* IP = (AiImportantPoint*) GetExZoneInfo(GetZoneHandle(i),AI_IMPORTANT_POINT,0);
		if(IP)
		{
			if(IP->Amount>0)
			{
				MainPoint* MP = new MainPoint();
				MP->x=GetZoneX(i);
				MP->y=GetZoneY(i);
				MP->Amount=IP->Amount;
				MP->MPZoneIndex=MainPoints.GetAmount();
				MP->Type=1;
				MP->ZoneIndex=i;
				MainPoints.Add(MP);
			}
		}
	}
	DIP_SimpleBuilding** Sela;
	int NSel;
	GetDipSimpleBuildings(NSel, Sela);
	for(i=0;i<NSel;i++)
	{
		DIP_SimpleBuilding* Selo = Sela[i];
		if(Selo)
		{
			MainPoint* MP = new MainPoint();
			Selo->GetCenter(MP->x,MP->y);
			MP->Amount=100;
			MP->MPZoneIndex=MainPoints.GetAmount();
			MP->Type=2;
			MP->SeloIndex=i;
			MainPoints.Add(MP);
		}
	}
	int n=GetNRoads();
	if(EngSettings.DontUseRoads)n=0;
	for(i=0;i<n;i++)
	{
		if(GetNRoadsLink(i)>2)
		{
			int x,y;
			if(GetTopZoneCoor(i,&x,&y))
			{
				MainPoint* MP = new MainPoint();
				MP->x=x<<6;
				MP->y=y<<6;
				MP->MPZoneIndex=MainPoints.GetAmount();
				MP->Type=0;
				MainPoints.Add(MP);
			}
		}
	}
	//FillDest();
}
void TacticalAI::FillDest() 
{
	/*
	int n=MainPoints.GetAmount();
	int NA=GetNAreas(0);
	for(int i=0;i<n;i++)
	{
		MainPoints[i]->WayPriceTo.Add(0xFFFF,n);
		for(int j=0;j<n;j++)
		{
			if(i!=j)
			{
				word dist=GetLinksDist(MainPoints[i]->TopZoneIndex+MainPoints[j]->TopZoneIndex*NA,0);
				MainPoints[i]->WayPriceTo[j]=dist;
			}
		}
	}
	*/
}
void TacticalAI::ClearAll()
{
	
	/*int n=MainPoints.GetAmount();
	for(int i=0;i<n;i++)
		if(MainPoints[i])
			delete (MainPoints[i]);
			*/
	ActiveAI.Clear();
	MainPoints.Clear();
	DistBetweenMP.Clear();
	FirstStart=false;
	StartAI=false;
	IsResearchMap=false;
	ProcessTime=0;
	NationalProcessIndex=0;
	//MPIndex=0;
}
void TacticalAI::Process()
{
	if((ProcessTime+50)<tmtmt)
	{
		if(!StartAI)
		{
			bool rese=false;
			for(int i=0;i<8;i++)
			{
				if(NATIONS[i].AI_Enabled)
				{
					if(!IsResearchMap)
					{
						ResearchMap();
						IsResearchMap=true;
					}
					Activate(NATIONS[i].CITY->NI);
				}
			}
			StartAI=true;
		}
		int n=ActiveAI.GetAmount();
		if(NationalProcessIndex<n)
		{
			if(ActiveAI[NationalProcessIndex]->Active)
			{
				if(!FirstStart)
				{
					FirstStart=true;	
					typedef int tpBirthCallback(int NewbornIndex);
					tpBirthCallback* SetBirthCallback(tpBirthCallback* dc);
					SetBirthCallback(&BirthCallback);
				}
				if(!IsResearchMap)
				{
					ResearchMap();
					IsResearchMap=true;
				}
				SetMPWeight(NationalProcessIndex);
				ActiveAI[NationalProcessIndex]->Process();
				ProcessTime+=100;
			}
			NationalProcessIndex++;
		}
		else
		{
			NationalProcessIndex=0;
			ProcessTime=tmtmt;
		}
	}
	/*
	int b=MainPoints.GetAmount();
	if(MPIndex<b)
	{
		MainPoint* MP=MainPoints[MPIndex];
		if(MP)
		{
			int v=WiselyFormations.AllForm.GetAmount();
			for(int l=0;l<v;l++)
			{
				Atom* Form=WiselyFormations.AllForm[l];
				if(Form&&Form->IsAlive())
				{
					MP->RegisterAtom(Form);
				}
			}
		}
		MPIndex++;
	}
	else
	{
		MPIndex=0;
	}
	*/
}
void TacticalAI::Activate(byte NI)
{
	int n=ActiveAI.GetAmount();
	bool isReady=false;
	for(int i=0;i<n;i++)
	{
		if(ActiveAI[i]->NI==NI)
		{
			ActiveAI[i]->Active=true;
			isReady=true;
			break;
		}
	}
	if(!isReady)
	{
		NationalAI* NA = new NationalAI();
		NA->NI=NI;
		NA->Active=true;
		ActiveAI.Add(NA);
	}
}
void TacticalAI::Stop(byte NI)
{
	int n=ActiveAI.GetAmount();
	for(int i=0;i<n;i++)
	{
		if(ActiveAI[i]->NI==NI)
		{
			ActiveAI[i]->Active=false;
			break;
		}
	}

}
void TacticalAI::SetMPWeight(word NI)
{
	int n=ActiveAI.GetAmount();
	if(NI<n&&ActiveAI[NI]->Active)
	{
		byte Mask=NATIONS[ActiveAI[NI]->NI].NMask;
		int v=WiselyFormations.AllForm.GetAmount();
		ActiveAI[NI]->SumEnmForce=0;
		ActiveAI[NI]->SumFreForce=0;
		for(int k=0;k<v;k++)
		{
			Atom* Form=WiselyFormations.AllForm[k];
			if(Form&&Form->IsAlive())
			{
				if(Form->GetNMask()&Mask)
				{	
					ActiveAI[NI]->SumFreForce+=Form->Force;
				}
				else
				{
					ActiveAI[NI]->SumEnmForce+=Form->Force;
				}
			}
		}

		int b=MainPoints.GetAmount();
		bool ManagetPoints=false;
		for(int j=0;j<b;j++)
		{
			MainPoint* MP=MainPoints[j];
			NationMainPointInfo* MPI=ActiveAI[NI]->GetMPInfo(j);
			if(MPI)
			{
				MPI->FriendlyForce=0;
				MPI->FriendlyForcePr=0;
				MPI->EnemyForce=0;
				MPI->EnemyForcePr=0;
				MPI->Hw=0;
				MPI->Tw=0;
				MPI->HWeight=0;
				MPI->TWeight=0;
			}
			for(int l=0;l<v;l++)
			{
				Atom* Form=WiselyFormations.AllForm[l];
				if(Form&&Form->IsAlive())
				{
					word LockType = Form->GetLockType();
					//int TopDist=GetTopDistance(Form->x>>6,Form->y>>6,MP->x>>6,MP->y>>6,LockType)<<6;
					int TopDist = MP->RegisterAtom(Form);
					int WH=0;
					int WHPr=0;
					if (Form->Force&&TopDist<4000) 
					{
						if(Form->GetNMask()&Mask)
						{
							WH=(((Form->Force*10000)/(ActiveAI[NI]->SumFreForce+1))*(4000-TopDist))/400000; 
							if(TopDist<1200)
								MPI->FriendlyForce+=Form->Force;
							MPI->FriendlyForcePr+=WH; 
						}
						else
						{
							WH=(((Form->Force*10000)/(ActiveAI[NI]->SumEnmForce+1))*(4000-TopDist))/400000;
							if(TopDist<1200)
								MPI->EnemyForce+=Form->Force;
							MPI->EnemyForcePr+=WH;
						}
					}
				}
			}
			//if(MP->Amount&&MPI->FriendlyForcePr)
			{
				int Amo=MP->Amount;
				if(MP->Type==1)
				{
					AiImportantPoint* IP = (AiImportantPoint*) GetExZoneInfo(GetZoneHandle(MP->ZoneIndex),AI_IMPORTANT_POINT,0);
					Amo=IP->Amount;
					MP->Amount=Amo;
					if(Amo==10000)ManagetPoints=true;
				}
				if(MP->Type==2)
				{
					Amo=0;
					DIP_SimpleBuilding** Sela;
					int NSel;
					GetDipSimpleBuildings(NSel, Sela);
					DIP_SimpleBuilding* Selo = Sela[MP->SeloIndex];
					if(Selo)
					{
						for(byte i=0;i<6;i++)
						{
							if(Selo->Produce[i])
							{
								CEXPORT	int GetSettlementPriory(byte NI,byte ResID);
								int pr=GetSettlementPriory(ActiveAI[NI]->NI,i);
								Amo+=(MP->Amount*pr)/100;
							}
						}
					}
				}
				if(MP->Type==3)
				{
					if(MP->BildIndex!=0xFFFF)
					{
						OneObject* OB=Group[MP->BildIndex];
						if(OB&&(!OB->Sdoxlo))
						{
							//ok
						}
						else
						{
							MP->Amount=0;
							MP->BildIndex=0xFFFF;
						}
					}
				}
				int dev=MPI->FriendlyForcePr;
				if(!dev)dev=1;
				MPI->Hw=0;
				MPI->Tw=0;
				if(((MPI->EnemyForcePr*100)/dev)<20&&MPI->EnemyForce==0)
				{
					if(!ManagetPoints)
					{
						MPI->HWeight=Amo;
						MPI->TWeight=0;
					}
					else
					{
						MPI->HWeight=0;
						MPI->TWeight=0;
						if(Amo==10000)
						{
							MPI->HWeight=Amo;
							MPI->TWeight=0;
						}
					}
				}
				else
				{
					if(!ManagetPoints)
					{
						MPI->HWeight=0;
						MPI->TWeight=Amo;
					}
					else
					{
						MPI->HWeight=0;
						MPI->TWeight=0;
						if(Amo==10000)
						{
							MPI->HWeight=0;
							MPI->TWeight=Amo;
						}
					}
				}
			}
		}
		for(j=0;j<b;j++)
		{
			MainPoint* MP=MainPoints[j];
			NationMainPointInfo* MPI=ActiveAI[NI]->GetMPInfo(j);
			if(MPI) 
			{
				MPI->Hw=MPI->HWeight;
				MPI->Tw=MPI->TWeight;
			}
		}
		if(!ManagetPoints)
		for(j=0;j<b;j++)
		{
			MainPoint* MP=MainPoints[j];
			NationMainPointInfo* MPI=ActiveAI[NI]->GetMPInfo(j);
			if(MPI) 
			{
				//MPI->Hw+=MPI->HWeight;
				//MPI->Tw+=MPI->TWeight;
				for(int i=0;i<b;i++)
				{
					if(i!=j)
					{
						MainPoint* MPi=MainPoints[i];
						NationMainPointInfo* MPIi=ActiveAI[NI]->GetMPInfo(i);
						if(MPIi&&(MPIi->HWeight||MPIi->TWeight))
						{
							int dst=GetDistBetweenMP(i,j);
							if(dst>0&&dst<10000)
							{
								MPI->Hw+=(MPIi->HWeight*(10000-dst))/50000;
								MPI->Tw+=(MPIi->TWeight*(10000-dst))/50000;
							}
						}
					}
				}
			}
		}
	}
}
void TacticalAI::SetMPWeight()
{
	int n=ActiveAI.GetAmount();
	for(int i=0;i<n;i++)
	{
		SetMPWeight(i);
	}
}
void TacticalAI::NewBrigadeIsBorn(Brigade *B)
{
	word NI = B->CT->NI;
	int n=ActiveAI.GetAmount();
	for(int i=0;i<n;i++)
	{
		if(ActiveAI[i]->NI==NI&&ActiveAI[i]->Active)
		{
			WiselyFormations.AddAutoBrigade(B);
			ActiveAI[i]->NewAutoForm(WiselyFormations.AF[WiselyFormations.MaxAF-1]);
			break;
		}
	}
}
int TacticalAI::GetDistBetweenMP(word i, word j)
{
	int rez=-1;
	int n=DistBetweenMP.GetAmount();
	word ii=i;
	word jj=j;
	if(j>i)
	{
		ii=j;
		jj=i;
	}
	int in=(ii+1)*ii/2+jj;
	if(in<n)
	{
		rez=DistBetweenMP[in];
	}
	else
	{
		int mp=MainPoints.GetAmount();
		if(ii<mp)
		{
			for(word i=MaxPointDistBetweenMP;i<mp;i++)
			{
				MainPoint* MPi=MainPoints[i];
				for(word j=0;j<(i+1);j++)
				{
					word TopDist=0;
					if(j!=i)
					{
						MainPoint* MPj=MainPoints[j];
						//TopDist=GetTopDistance(MPi->x>>6,MPi->y>>6,MPj->x>>6,MPj->y>>6,0)<<6;
						TopDist=Norma( (MPi->x-MPj->x)>>6 , (MPi->y-MPj->y)>>6 )<<6;
					}
					DistBetweenMP.Add(TopDist);
				}
			}
		}
	}
	return rez;
}
void TacticalAI::ShowInfo()
{
	int n=ActiveAI.GetAmount();
	int YShift=0;
	for(int i=0;i<n;i++)
	{
		if(ActiveAI[i]->Active)
		{
			ActiveAI[i]->ShowInfo(YShift);
			YShift+=10;
		}
	}
}
int TacticalAI::BirthCallback(int NewbornIndex)
{
	if(NewbornIndex!=0xFFFF)
	{
		OneObject* OB=Group[NewbornIndex];
		if(OB)
		{
			if(OB->newMons->CommandCenter)
			{
				MainPoint* MP = new MainPoint();
				MP->x=OB->RealX>>4;
				MP->y=OB->RealY>>4;
				MP->Amount=200;
				MP->MPZoneIndex=MainPoints.GetAmount();
				MP->Type=3;
				MP->BildIndex=OB->Index;
				MainPoints.Add(MP);
			}
			else
			if(OB->Usage==CenterID)
			{
				MainPoint* MP = new MainPoint();
				MP->x=OB->RealX>>4;
				MP->y=OB->RealY>>4;
				MP->Amount=300;
				MP->MPZoneIndex=MainPoints.GetAmount();
				MP->Type=3;
				MP->BildIndex=OB->Index;
				MainPoints.Add(MP);
			}
			else
			if(OB->Usage==PushkaID)
			{
				int n=ActiveAI.GetAmount();
				for(int i=0;i<n;i++)
				{
					if(ActiveAI[i]->NI==OB->NNUM&&ActiveAI[i]->Active)
					{
						int pin = WiselyFormations.AddAutoCannon(OB);
						if(pin!=-1)
							ActiveAI[i]->NewAutoForm(WiselyFormations.AF[pin]);
						break;
					}
				}
			}
		}
	}
	return 0;
}
void TacticalAI::SaveSB(SaveBuf *SB)
{
	byte nai=(byte)ActiveAI.GetAmount();
	xBlockWrite(SB,&nai,1);
	for(int i=0;i<nai;i++)
	{
		ActiveAI[i]->SaveSB(SB);
	}
	byte end=101;
	xBlockWrite(SB,&end,1);
}
void TacticalAI::LoadSB(SaveBuf *SB)
{
	byte nai=0;
	xBlockRead(SB,&nai,1);
	ActiveAI.Clear();
	for(int i=0;i<nai;i++)
	{
		ActiveAI.Add(new NationalAI());
		ActiveAI.End()->LoadSB(SB);
	}
	byte end=101;
	xBlockRead(SB,&end,1);
}
bool TacticalAI::GetNewTowerCoord(byte NI, int &x, int &y)
{
	bool rez=false;
	int n=ActiveAI.GetAmount();
	for(int i=0;i<n;i++)
	{
		if(ActiveAI[i]->NI==NI)
		{
			int b=MainPoints.GetAmount();
			int max=0;;
			for(int j=0;j<b;j++)
			{
				MainPoint* MP=MainPoints[j];
				NationMainPointInfo* MPI=ActiveAI[NI]->GetMPInfo(j);
				if(MPI)
				{
					if(MPI->HWeight==0&&MPI->TWeight==0&&(MPI->Tw*2+MPI->Hw)>max)
					{
						if(MPI->EnemyForce==0&&((MPI->EnemyForcePr*100)/MPI->FriendlyForcePr)<20)
						{
							int m=WiselyFormations.AllForm.GetAmount();
							bool cek=true;
							for(int k=0;k<m&&cek;k++)
							{
								Atom* A = WiselyFormations.AllForm[k];
								if(A&&A->IsAlive()&&A->Tower!=0)
								{
									int dds=Norma(MP->x-A->x,MP->y-A->y);
									if(dds<1000)
										cek=false;
								}
							}
							if(cek)
							{
								x=MP->x;
								y=MP->y;
								max=MPI->Tw*2+MPI->Hw;
								rez=true;
							}
						}
					}
				}
			}
			break;
		}
	}
	return rez;
}
//------------------------------------------------------------------------------------------------------------------//