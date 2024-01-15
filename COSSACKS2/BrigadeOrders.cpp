//=================================================================================================================//
#include "stdheader.h"
//=================================================================================================================//
void AttackObjLink(OneObject* OBJ);
extern int AnimTime;
int GetDamage(word ShooterID,word VictimID,int AttType);
bool CheckIfRoadZone(int Zone);
int GetNextWayPoints(int From, int Dest, int *PointsXYList);
int GetNextWayPoints(int From, int Dest, int *PointsXYList, word &Flag, DWORD Requestor);
int SetPreLock(int From, int Dest);
int SetPreLock(int From, int Dest, DWORD Requestor);
int GetTopology(int x,int y, byte LockType=0);
int GetTopology(int* x,int* y, byte LockType=0);
word GetDir(int dx,int dy);
bool CheckIfPossibleToBreakOrder(OneObject* OB);
void ApplySwap(Brigade* BR,word* swp);
extern int tmtmt;
extern MotionField UnitsField;
bool RotateUnit(OneObject* OB,char Dir);
bool CalcCenter(Brigade* BR,int* x,int* y);
word GetBrigLockType(Brigade* BR);
void MakeReformation(byte NI,word BrigadeID,byte FormType);
bool CheckBDirectWay(int x0,int y0,int x1,int y1,byte LockType=0);
extern int PeaceTimeLeft;
#define MaxIsl 64
extern byte IslPrs[MaxIsl];
int CleanGroup(UnitsGroup* GRP);
void TakeResLink(OneObject* OBJ);
int FindCostPoint(int x,int y,byte Isl);
int FindCostPointEx(int x,int y,byte Mask);
void LeaveAll(OneObject* OB);
void StayForSomeTime(OneObject* OB,byte OrdType,int Time);
byte GetBrigadeDirectionByUnitPositions(Brigade* BR);
int GetPointToLineDistEx(int x,int y,int x1,int y1,int x2,int y2,int& xn,int& yn,int& Weight);
//=================================================================================================================//
CBrigadeOrder::CBrigadeOrder()
{
}
CBrigadeOrder::~CBrigadeOrder()
{
}
CBrigadeOrder::CBrigadeOrder(Brigade* Br)
{
	Init(Br);
}
bool CBrigadeOrder::Process(Brigade* Br)
{
	return false;
}
bool CBrigadeOrder::Init(Brigade* Br)
{
	return false;
}
byte CBrigadeOrder::GetBrigadeOrderID()
{
	return 0;
}
byte CBrigadeOrder::GetBrigadeOrderPrio()
{
	return 0;
}
bool CBrigadeOrder::IsFast()
{
	return false;
}
char* CBrigadeOrder::GetBrigadeOrderMessage()
{
	return NULL;
}
int CBrigadeOrder::SaveBinaryData(void* Data)
{
	return 0;
}
bool CBrigadeOrder::LoadBinaryData(void* Data)
{
	return false;
}
//=================================================================================================================//
BrigadeOrder_RifleAttack::BrigadeOrder_RifleAttack()
{
	Brig=NULL;
	FirstProces=false;
}
BrigadeOrder_RifleAttack::~BrigadeOrder_RifleAttack()
{
	if(Brig&&Brig->WarType>0&&Brig->Memb)
	{
		/*int NFireReady=0;
		for(int i=0;i<Brig->NMemb;i++)
		{
			OneObject* OB=Group[Brig->Memb[i]];
			if(OB&&OB->RifleAttack&&OB->delay==0)
				NFireReady++;
		}
		if(NFireReady==0)
		{*/
			for(int i=0;i<Brig->NMemb;i++)
			{
				OneObject* OB=Group[Brig->Memb[i]];
				if(OB)
					OB->RifleAttack=false;
			}
			ClearTargetObj(Brig);
		//}
	}
}
int BrigadeOrder_RifleAttack::EX=0;
int BrigadeOrder_RifleAttack::EY=0;
void BrigadeOrder_RifleAttack::ClearTargetObj(Brigade *BR)
{
	if(BR)
	{
		for(int i=0;i<BR->NMemb;i++)
		{
			OneObject *OB=Group[BR->Memb[i]];
			if(OB&&OB->Attack)
			{
				if(OB->LocalOrder&&OB->LocalOrder->DoLink==AttackObjLink){	
					int cd=(OB->NewAnm->Code/100)*100;
					if(cd==anm_Attack||cd==anm_PAttack){
						if(!OB->LocalOrder->info.BuildObj.SN)OB->LocalOrder->info.BuildObj.SN=1;
					}else OB->DeleteLastOrder();
				}
				OB->EnemyID=0xFFFF;
				OB->EnemySN=0xFFFF;
			}
		}
	}
}
void BrigadeOrder_RifleAttack::ClearLastOrders(Brigade *BR)
{
	if(BR)
	{
		for(int i=0;i<BR->NMemb;i++)
		{
			OneObject *OB=Group[BR->Memb[i]];
			if(OB)
			{
				void NewAttackPointLink(OneObject* OBJ);
				if(OB->LocalOrder&&OB->LocalOrder->DoLink!=AttackObjLink&&OB->LocalOrder->DoLink!=&NewAttackPointLink){
					OB->DeleteLastOrder();
				}
			}
		}
	}
}
bool BrigadeOrder_RifleAttack::ProcessPre(Brigade *BR)
{
	addrand(BR->ID);
	bool rez=true;
	if(FillTime<AnimTime)
	{
		bool Rec=true;
		bool RTF=false;
		NewMonster* Mon=NULL;
		for(int i=0;i<BR->NMemb&&Rec;i++)
		{
			OneObject *OB=Group[BR->Memb[i]];
			if(OB&&OB->RifleAttack)
			{
				RTF=true;
				if((!Mon)&&i>3)
					Mon=OB->newMons;
				if(OB->delay==0&&OB->EnemyID!=0xFFFF)
				{
					OneObject *OBt=Group[OB->EnemyID]; 
					if(OBt&&(!OBt->Sdoxlo))
					{
						Rec=false;
					}
				}
			}
		}
		if(!RTF)
		{
			return false;
		}
		if(Rec)
		{
			FillEnemyList(BR);
			//FillTime=tmtmt+((160*GameSpeed)>>8);  
			FillTime=AnimTime+(42+29+21)*256; 
			rez=false;
			if(Mon&&Mon->ShotAlwaysOn)
				rez=true;
		}
	}
	NEnemyLife=NEnemy;
	addrand(NEnemy);
	for(int i=0;i<BR->NMemb&&NEnemyLife>0;i++)
	{
		addrand(i);
		addrand(FireOrder[i]);
		if(FireOrder[i]!=0xFFFF)
		{
			OneObject *OB=Group[FireOrder[i]];
			if(OB&&OB->RifleAttack&&OB->delay==0)
			{
				if(OB&&(!OB->Attack))
				{
					addrand(OB->Index);
					if(SetTargetObj(OB))
						rez=true;
				}
				else
					if(OB&&OB->Attack)
					{
						if(OB->EnemyID!=0xFFFF)
						{
							OneObject* BB=Group[OB->EnemyID]; 
							if(BB&&BB->Sdoxlo)
							{
								addrand(OB->Index);
								if(SetTargetObj(OB))
									rez=true;
							}
						}
						else{
							addrand(OB->Index);
							if(SetTargetObj(OB))
								rez=true;
						}
					}
			}
			else
				if(OB&&(OB->Index==BR->Memb[0]||OB->Index==BR->Memb[1]||OB->Index==BR->Memb[2]))
					OB->ClearOrders();
		}
		NEnemyLife=NEnemy;
	}
	return rez;
}
bool BrigadeOrder_RifleAttack::SetTargetObj(OneObject *OB)
{
	addrand(OB->Index);
	bool rez=false;
	int dist=9999999;
	int TIndex=-1;
	int ni=NEnemy/8+5;
	if(OB->Usage==EgerID)
	{
		ni=NEnemy/2;
	}
	if(NEnemy<16)ni=NEnemy;
	addrand(ni);
	for(int i=0;i<ni;i++)
	{
		int rn=rando()%NEnemy;
		OneObject *tOB=NULL;
		tOB=Group[EnemyList[rn]];
		if(tOB&&(!tOB->Sdoxlo))
		{
			addrand(tOB->Index);
			if(tOB->Life>(EnemyLife[rn]-40)&&((EnemyFireCount[rn]<5&&tOB->Life<101)||tOB->Life>100))
			{
				int nd=Norma((OB->RealX>>4)-(tOB->RealX>>4),(OB->RealY>>4)-(tOB->RealY>>4));
				addrand(nd);
				if(OB->Usage==EgerID)
				{
					nd=nd/(tOB->Ref.General->NeedRes[1]+1);
				}
				//int sea=OB->newMons->VisRange>>4;
				int dz=OB->RZ-tOB->RZ;
				if(dz<0)dz=0;
				if(dz>150)dz=150;
				dz<<=1;
				int sea=(OB->MoreCharacter->VisRange>>4)+dz;
				if(nd<dist&&nd<=sea)
				{
					/*
					Coor3D C3D;
					tOB->GetDamagePoint(OB->x,OB->y,&C3D,0);
					int x0,y0,z0,x1,y1,z1;
					x0=(OB->RealX>>4);
					y0=(OB->RealY>>4);
					z0=OB->RZ+32;
					x1=C3D.x;
					y1=C3D.y;
					z1=C3D.z;
					int CheckBreaksBetweenPoints(int x,int y,int z,int xd,int yd,int zd,int Index);
					//int r=CheckBreaksBetweenPoints(x0,y0,z0,x1,y1,z1,OB->Index);
					//if(r==-1)
					*/
					{
						addrand(nd);
						addrand(rn);
						dist=nd;
						TIndex=rn;
					}
				}
			}
		}
	}
	addrand(TIndex);
	if(TIndex!=-1)
	{
		addrand(EnemyList[TIndex]);
		if(OB->AttackObj(EnemyList[TIndex],128+15,0,1))
		{
			EnemyLife[TIndex]+=GetDamage(OB->Index,EnemyList[TIndex],1);
			EnemyFireCount[TIndex]++;
			rez=true;
		}
	}

	return rez;
}

typedef bool tpUnitsCallback(OneObject* OB,void* param);
int PerformActionOverUnitsInRadius(int xc,int yc,int R,tpUnitsCallback* CB,void* Param);
void BrigadeOrder_RifleAttack::FillEnemyList(Brigade* BR)
{
	byte dir;
	if(!( BR->Enabled && BR->Memb && BR->NMemb ) )return;
	BR->GetCenter(&x,&y,&dir);
	NEnemy=0;
	memset(EnemyList,0,512*2);
	memset(EnemyLife,0,512*2);
	memset(EnemyFireCount,0,512*2);
	word mask=BR->GetNMask();
	EnemyX=0;
	EnemyY=0;
	NEnemy=0;
	word eger=0;
	word cou=0;
	if(BR->Memb[3]!=0xFFFF)
	{
		OneObject* OB =Group[BR->Memb[3]];
		if(OB&&OB->Usage==EgerID)
		{
			eger=1;
		}
	}
	word* ex=(word*)(&EnemyX);
	word* ey=(word*)(&EnemyY);
	word* pr[8]={EnemyList,&NEnemy,&mask,ex,ey,&OnlyOneBrig,&eger,&cou};
	addrand(mask);
	addrand(eger);
	addrand(OnlyOneBrig);
	PerformActionOverUnitsInRadius(x,y,1200,&BrigadeOrder_RifleAttack::AddEnemy,(void*)pr);
	addrand(cou);
	addrand(EnemyX);
	addrand(EnemyY);
	addrand(NEnemy);
	if(NEnemy)
	{
		EnemyX=EnemyX/NEnemy;
		EnemyY=EnemyY/NEnemy;
	}
	for(int i=0;i<BR->NMemb;i++){
		FireOrder[i]=BR->Memb[i];
		addrand(FireOrder[i]);
	}
	EX=EnemyX;
	EY=EnemyY;
	addrand(EX);
	addrand(EY);
	if(NEnemy<40)
	{
		qsort(FireOrder,BR->NMemb,sizeof(word),BrigadeOrder_RifleAttack::compare);
	}
#ifdef CHECK_SYNC_FAILURE
	for(int i=0;i<BR->NMemb;i++){		
		addrand(FireOrder[i]);
	}
#endif
}
int BrigadeOrder_RifleAttack::compare(const void *p1,const void *p2)
{
	int rez=0;
	word* i1=(word*)p1;
	word* i2=(word*)p2;
	if((*i1)==0xFFFF)
		rez++;
	if((*i2)==0xFFFF)
		rez--;
	addrand(*i1);
	addrand(*i2);
	if((*i1)!=0xFFFF&&(*i2)!=0xFFFF)
	{
		OneObject* OB1=Group[(*i1)];
		OneObject* OB2=Group[(*i2)];
		if(OB1&&OB2)
		{
			int d1=Norma((OB1->RealX>>4)-EX,(OB1->RealY>>4)-EY);
			int d2=Norma((OB2->RealX>>4)-EX,(OB2->RealY>>4)-EY);
			addrand(d1);
			addrand(d2);
			rez=d1-d2;
		}   
	}
	addrand(rez);
	return rez;
}
bool BrigadeOrder_RifleAttack::AddEnemy(OneObject *OB, void *param)
{
	bool rez=false;
	word** pr=(word**)param;
	word* EL=pr[0];
	word* Nen=pr[1];
	int* ex=(int*)pr[3];
	int* ey=(int*)pr[4];
	word* OnlyBrig=pr[5];
	byte mask=(byte)(*(pr[2]));
	addrand(OB->Index);
	addrand(*Nen);
	addrand(*(pr[6]));
	addrand(*(pr[7]));
	if((!(mask&OB->NMask))&&(((*OnlyBrig)==0xFFFF)||((*OnlyBrig)==OB->BrigadeID))&&!OB->Sdoxlo)
		if((*Nen)<240)
		{
			if((!(*(pr[6])))||((*(pr[6]))&&*Nen<30&&(OB->Ref.General->NeedRes[1]||(*(pr[7]))%5==0)))
			{
				EL[(*Nen)]=OB->Index;
				addrand(OB->Index);
				(*Nen)++;
				(*ex)+=(OB->RealX>>4);
				(*ey)+=(OB->RealY>>4);
				rez=true;
			}
			(*(pr[7]))++;
		}
	return rez;
}
bool BrigadeOrder_RifleAttack::Process(Brigade *BR)
{
	addrand(BR->ID);
	Brig=BR;
	if(!FirstProces)
	{
		ClearTargetObj(BR);
		ClearLastOrders(BR);
		FirstProces=true;
		FillTime=0;
		FillEnemyList(BR);
	}
	if(!BR->NMemb)
	{
		BR->DeleteAll();
		BR->Enabled=false;
		BR->AttEnm=0;
	}
	else
		if(!ProcessPre(BR))
		{
			BR->DeleteNewBOrder();
			ClearTargetObj(BR);
			for(int i=0;i<BR->NMemb;i++)
			{
				OneObject* OB=Group[BR->Memb[i]];
				if(OB)OB->RifleAttack=false;
			}
		}
	return true;
}
bool BrigadeOrder_RifleAttack::Init(Brigade *BR)
{
	return Process(BR);
}
byte BrigadeOrder_RifleAttack::GetBrigadeOrderID()
{
	return BRIGADEORDER_RIFLEATTACK;
}
//=================================================================================================================//
#define RoadSpeed (64+32)
#define MaxSpdLimit 240
BrigadeOrder_GoOnRoad::BrigadeOrder_GoOnRoad()
{
	DestTopZone=-1;
	IsEnemyOnWay=false;
}
BrigadeOrder_GoOnRoad::~BrigadeOrder_GoOnRoad()
{}
bool BrigadeOrder_GoOnRoad::AskGoAway(OneObject* OB,void* p)
{
	bool rez=false;
	if(OB&&!OB->Sdoxlo)
	{
		int* P=(int*)p;
		byte Mask=(byte)P[0];
		if(OB->NMask&Mask&&OB->StandTime>1&&OB->LocalOrder==NULL)
		{
			//int ddx=OB->RealX/16-P[1];
			//int ddy=OB->RealY/16-P[2];
			/*
			int ddx=P[4]-P[1];
			int ddy=P[5]-P[2];
			int nn=Norma(ddx,ddy);
			if(nn)
			{
				int pp=0;
				while(pp<100&&!OB->FrameFinished){
					OB->SetNextFrame();
					pp++;
				}
				//StayForSomeTimeLink
				if(OB->BrigadeID!=0xFFFF)
					StayForSomeTime(OB,1,15*25*256);
				char mdr=GetDir(P[4]-P[1],P[5]-P[2]);
				char pbd=GetDir(OB->RealX/16-P[1],OB->RealY/16-P[2]);
				//char dd=mdr-pbd;
				OB->Speed=64;

				//int f1=(((OB->RealX/16-P[1])*TCos[mdr])>>8)+(((OB->RealY/16-P[2])*TSin[mdr])>>8);
				//byte abbs=abs(mdr);
				if(((mdr<65&&mdr>-1)||mdr<-64))
				{
					if(mdr>pbd)
					{
						OB->NewMonsterSendTo(OB->RealX+ddx*128*16/nn,OB->RealY-ddy*98*16/nn,128+16,1);
					}
					else
					{
						OB->NewMonsterSendTo(OB->RealX-ddx*128*16/nn,OB->RealY+ddy*98*16/nn,128+16,1);
					}
				}
				else
				{
					if(mdr<pbd)
					{
						OB->NewMonsterSendTo(OB->RealX+ddx*128*16/nn,OB->RealY-ddy*98*16/nn,128+16,1);
					}
					else
					{
						OB->NewMonsterSendTo(OB->RealX-ddx*128*16/nn,OB->RealY+ddy*98*16/nn,128+16,1);
					}
				}
			}
			*/
			int ddx=P[4]-P[1];
			int ddy=P[5]-P[2];
			int nn=Norma(ddx,ddy);
			if(nn)
			{
				int pp=0;
				while(pp<100&&!OB->FrameFinished){
					OB->SetNextFrame();
					pp++;
				}
				if(OB->BrigadeID!=0xFFFF)
					StayForSomeTime(OB,1,10*25*256);
				char mdr=GetDir(P[4]-P[1],P[5]-P[2]);
				char pbd=GetDir(OB->RealX/16-P[1],OB->RealY/16-P[2]);
				OB->Speed=64;
				OB->UnitSpeed=64;

				int Wt,xn,yn;
				int dd=GetPointToLineDistEx(OB->RealX/16,OB->RealY/16,P[1],P[2],P[4],P[5],xn,yn,Wt);
				int raz=P[3]-dd;
				if(raz>0)
				{
					if(abs(mdr-pbd)<65)
					{
						raz=( raz*( 16+(rando()&15) ) )>>5;
						if((mdr-pbd)>0)
						{
							OB->NewMonsterSendTo(OB->RealX+ddy*raz*16/nn,OB->RealY-ddx*raz*16/nn,128+16,1);
						}
						else
						{
							OB->NewMonsterSendTo(OB->RealX-ddy*raz*16/nn,OB->RealY+ddx*raz*16/nn,128+16,1);
						}
						rez=true;
					}
				}
			}
		}
		if(!(OB->NMask&Mask))
		{
			if(OB->BrigadeID!=0xFFFF)
			{
				rez=true;
			}
		}
	}
	return rez;
}
bool BrigadeOrder_GoOnRoad::GoAway()
{
	bool rez=false;
	if(HeadOBJIndex!=0xFFFF&&MaxPIndex)
	{
		OneObject* OB=Group[HeadOBJIndex];
		if(OB&&!OB->Sdoxlo)
		{
			int cx=OB->RealX/16;
			int cy=OB->RealY/16;
			int dx=PointsXY[(MaxPIndex-1)*2];
			int dy=PointsXY[(MaxPIndex-1)*2+1];
			int R=Norma(cx-dx,cy-dy);
			int P[8];
			P[0]=(int)OB->NMask;
			P[1]=cx;
			P[2]=cy;
			P[3]=128;//(int)this;
			P[4]=dx;
			P[5]=dy;
			int enmcou=PerformActionOverUnitsInRadius(P[1],P[2],128,&BrigadeOrder_GoOnRoad::AskGoAway,P);
			//P[1]=(cx+dx)/2;
			//P[2]=(cy+dy)/2;
			enmcou+=PerformActionOverUnitsInRadius((cx+dx)/2,(cy+dy)/2,128,&BrigadeOrder_GoOnRoad::AskGoAway,P);
			if(enmcou>10)
				rez=true;
		}
	}
	return rez;
}
void BrigadeOrder_GoOnRoad::RangeInLineMemb(Brigade* BR)
{
	bool ff=false;
	int lf=-1;
	for(int i=3;i<(BR->NMemb-1);i++)
	{
		if(BR->Memb[i]==0xFFFF)
		{
			if(!ff)
			{
				lf=i;
				ff=true;
			}
		}
		else
		{
			if(ff)
			{
				for(int j=i;j<BR->NMemb;j++)
				{
					BR->Memb[lf+j-i]=BR->Memb[j];
					BR->MembSN[lf+j-i]=BR->MembSN[j];
					OneObject* OB = Group[BR->Memb[lf+j-i]];
					if(OB)
					{
						OB->BrIndex=lf+j-i;
					}
					BR->Memb[j]=0xFFFF;
					BR->MembSN[j]=0xFFFF;
				}
				ff=false;
			}
		}
	}
}


void BrigadeOrder_GoOnRoad::SortBrigUnits(Brigade *BR, int xx, int yy)
{
	fBrig=BR;  
	fx=xx;
	fy=yy;
	for(int i=3;i<BR->NMemb;i++)
		SA[i-3]=i;
	qsort((void*)SA,BR->NMemb-3,sizeof(word),BrigadeOrder_GoOnRoad::compare);
	int N=BR->NMemb-3;
	for(int i=0;i<N;i++)
	{
		SA[i]=BR->Memb[SA[i]];
	} 
	for(int i=0;i<N;i++)
	{
		BR->Memb[i+3]=SA[i];
		if(BR->Memb[i+3]!=0xFFFF)
		{
			OneObject* OB=Group[BR->Memb[i+3]];
			if(OB)
			{
				OB->BrIndex=i+3;
				BR->MembSN[i+3]=OB->Serial;
			}
		}
	}
}

void BrigadeOrder_GoOnRoad::SetUnitsSpeed(Brigade* BR)
{
	if(BR)
	{
		int AvrDst=0;
		int NCalk=0;
		for(int i=0;i<BR->NMemb;i++)
		{
			if(BR->Memb[i]!=0xFFFF)
			{
				OneObject* OB=Group[BR->Memb[i]];
				if(OB)
				{
					int ds=Norma((OB->RealX>>4)-(OB->DestX>>4),(OB->RealY>>4)-(OB->DestY>>4));
					if(ds<200)
					{
						AvrDst+=ds;
						NCalk++;
					}
				}
			}
		}
		if(NCalk)
		{
			AvrDst/=NCalk;
			for(int i=0;i<BR->NMemb;i++)
			{
				if(BR->Memb[i]!=0xFFFF)
				{
					OneObject* OB=Group[BR->Memb[i]];
					if(OB)
					{
						int ds=Norma((OB->RealX>>4)-(OB->DestX>>4),(OB->RealY>>4)-(OB->DestY>>4))+1;
						int spd=(RoadSpeed*ds)/(AvrDst+1);
						if(spd<32)spd=32;
						if(spd>MaxSpdLimit)spd=MaxSpdLimit; 
						OB->UnitSpeed=spd;
					}
				}
			}
		}
	}
}

Brigade* BrigadeOrder_GoOnRoad::fBrig=NULL;
int BrigadeOrder_GoOnRoad::fx=-1;
int BrigadeOrder_GoOnRoad::fy=-1;
void BrigadeOrder_GoOnRoad::LockNextPoint()
{
	if(HeadTopZoneIndex!=DestTopZone)
	{
		int NA=GetNAreas(lockType);
		int NTP=GetMotionLinks(DestTopZone+NA*HeadTopZoneIndex,lockType,(Owner>>16)&0xFF);
		if(NTP<0xFFFE&&CheckIfRoadZone(NTP))
		{
			SetPreLock(HeadTopZoneIndex, NTP, Owner);
		}
	}
}
void BrigadeOrder_GoOnRoad::SetOffPoss(int LineI,Brigade* BR)
{
	int xx,yy;
	if(BR->Memb[0]!=0xFFFF)
	{
		OneObject* OB=Group[BR->Memb[0]];
		if(OB)
		{
			GetUnitCoordInColumn(LineI,0, xx, yy);
			OB->DestX=xx<<4; 
			OB->DestY=yy<<4;
			addrand(OB->DestX);
			addrand(OB->DestY);
			BR->posX[0]=xx<<4;
			BR->posY[0]=yy<<4; 
			if(OffPlaseHead)
				HeadOBJIndex=BR->Memb[0];
		}
	}
	if(BR->Memb[1]!=0xFFFF)
	{
		OneObject* OB=Group[BR->Memb[1]];
		if(OB)
		{
			GetUnitCoordInColumn(LineI,UnitsInLine/2, xx, yy);
			OB->DestX=xx<<4; 
			OB->DestY=yy<<4;
			addrand(OB->DestX);
			addrand(OB->DestY);
			BR->posX[1]=xx<<4;
			BR->posY[1]=yy<<4; 
			if(OffPlaseHead)
				HeadOBJIndex=BR->Memb[1];
		}
	}
	if(BR->Memb[2]!=0xFFFF)
	{
		OneObject* OB=Group[BR->Memb[2]]; 
		if(OB)
		{
			GetUnitCoordInColumn(LineI,UnitsInLine-1, xx, yy);
			OB->DestX=xx<<4; 
			OB->DestY=yy<<4;
			addrand(OB->DestX);
			addrand(OB->DestY);
			BR->posX[2]=xx<<4;
			BR->posY[2]=yy<<4; 
		}
	}
}
int BrigadeOrder_GoOnRoad::GetMinDistBrigToPoint(Brigade* BR, int x, int y)
{
	int rez=-1;
	if(BR)
	{
		for(int i=0;i<BR->NMemb;i++)
		{
			if(BR->Memb[i]!=0xFFFF)
			{
				OneObject* OB = Group[BR->Memb[i]];
				if(OB)
				{
					int ds=Norma((OB->RealX>>4)-x,(OB->RealY>>4)-y);
					if(rez==-1)
						rez=ds;
					else
						if(rez>ds)
							rez=ds;
				}
			}
		}
	}
	return rez;
}
int BrigadeOrder_GoOnRoad::compare(const void *a1, const void *a2)
{
	int rez=0;
	word i1=*(word*)a1;
	word i2=*(word*)a2;
	if(fBrig->Memb[i1]==0xFFFF)
		rez--;
	if(fBrig->Memb[i2]==0xFFFF)
		rez++;
	if(fBrig->Memb[i1]!=0xFFFF&&fBrig->Memb[i2]!=0xFFFF)
	{
		OneObject* OB1=Group[fBrig->Memb[i1]];
		OneObject* OB2=Group[fBrig->Memb[i2]];
		if(OB1&&OB2)
		{
			int d1=Norma((OB1->RealX>>4)-fx,(OB1->RealY>>4)-fy);
			int d2=Norma((OB2->RealX>>4)-fx,(OB2->RealY>>4)-fy);
			return d1-d2;
		}
	}
	return rez;
}
bool BrigadeOrder_GoOnRoad::ProcessPre(Brigade *BR)
{
	addrand(BR->ID);
	//NTP=GetMotionLinks(FinalTop+NA*NTP,lockType);
	//if(NTP<0xFFFE&&NTP!=FinalTop&&CheckIfRoadZone(NTP)){
	if(tmtmt>LastMoveTime)
	{
		if(HeadIndex==-1)
		{
			int xx,yy;
			BR->GetCenter(&xx,&yy);
			HeadTopZoneIndex=GetTopology(xx,yy,lockType);
			if(HeadTopZoneIndex==DestTopZone)
				return false;
		}
		if(HeadTopZoneIndex==DestTopZone&&((MaxPIndex-HeadIndex)<2))
			return false;
		if((MaxPIndex-HeadIndex)<8)
		{
			int NA=GetNAreas(lockType);
			int NTP=GetMotionLinks(DestTopZone+NA*HeadTopZoneIndex,lockType,(Owner>>16)&255);
			if(NTP<0xFFFE&&HeadIndex==-1)
			{
				HeadTopZoneIndex=NTP;
				NTP=GetMotionLinks(DestTopZone+NA*HeadTopZoneIndex,lockType,(Owner>>16)&255);
			}
			if(NTP<0xFFFE&&CheckIfRoadZone(NTP))
			{
				int addM=GetNextWayPoints(HeadTopZoneIndex,NTP,PointsXY+MaxPIndex*2,RoadFlag,Owner);
				addrand(addM);
				if(addM)
				{
					MaxPIndex+=addM;
					if(HeadIndex==-1)
					{
						addrand(BR->ID);
						/*
						bool flag=true;
						for(;flag;)
						{
						//if(GetMinDistBrigToPoint(BR,PointsXY[(MaxPIndex-1)*2],PointsXY[(MaxPIndex-1)*2+1])>300)
						if(GetMinDistBrigToPoint(BR,PointsXY[(HeadIndex+1)*2],PointsXY[(HeadIndex+1)*2+1])>100) 
						flag=false;
						else
						{
						HeadIndex=MaxPIndex-1;
						HeadTopZoneIndex=NTP;
						NTP=GetMotionLinks(DestTopZone+NA*HeadTopZoneIndex,lockType);
						if(NTP<0xFFFE&&CheckIfRoadZone(NTP))
						{
						int addMn=GetNextWayPoints(HeadTopZoneIndex,NTP,PointsXY+MaxPIndex*2,RoadFlag,Owner);
						MaxPIndex+=addMn;
						addM+=addMn;
						HeadTopZoneIndex=NTP;
						LockNextPoint();
						}
						else
						flag=false;
						}
						}
						*/
						if(NLines>MaxPIndex)
						{
							int ax,ay;
							byte aang;
							BR->GetCenter(&ax,&ay,&aang);
							byte apd=GetDir(ax-PointsXY[(MaxPIndex-1)*2],ay-PointsXY[(MaxPIndex-1)*2+1]);
							int np=NLines-MaxPIndex;
							for(int j=MaxPIndex-1;j>-1;j--)
							{
								PointsXY[(j+np)*2]=PointsXY[j*2];
								PointsXY[(j+np)*2+1]=PointsXY[j*2+1];
							}
							//memmove(PointsXY+np*2,PointsXY,4*2*np);
							int pdd=25;
							for(int i=0;i<np;i++)
							{
								int nx=PointsXY[np*2]+((pdd*(i+1)*TCos[apd])>>8);
								int ny=PointsXY[np*2+1]+((pdd*(i+1)*TSin[apd])>>8);
								PointsXY[(np-1-i)*2]=nx;
								PointsXY[(np-1-i)*2+1]=ny;
							}
							MaxPIndex+=np;
							addM=MaxPIndex;
						}
						HeadIndex=MaxPIndex-1;
					}
					//setDir
					for(int i=(MaxPIndex-addM);i<(MaxPIndex-1);i++)
					{
						PointsDir[i]=GetDir(PointsXY[(i+1)*2]-PointsXY[i*2],PointsXY[(i+1)*2+1]-PointsXY[i*2+1]);
					}
					PointsDir[MaxPIndex-1]=PointsDir[MaxPIndex-2];

					HeadTopZoneIndex=NTP;
					addrand(BR->ID);
					LockNextPoint();
					int const MemShift=80;
					if(HeadIndex>(NLines*PassNPoints+MemShift)&&HeadIndex>MemShift)
					{
						memmove(PointsXY,PointsXY+MemShift*2,4*2*(MaxPIndex-MemShift));
						memmove(PointsDir,PointsDir+MemShift,MaxPIndex-MemShift);
						MaxPIndex-=MemShift;
						HeadIndex-=MemShift;
					}
					//GoAway();
				}
				else{
					addrand(BR->ID);
					if(!MaxPIndex){
						addrand(BR->ID);
						return false;
					}
				}
			}
			else{
				addrand(BR->ID);
				if(HeadIndex>=(MaxPIndex-1)){
					addrand(BR->ID);
					return false;
				}
			}
		}
		addrand(BR->ID);

		OneObject* OB=NULL;
		if(HeadOBJIndex!=0xFFFF)
		{
			OB=Group[HeadOBJIndex];
			if(OB&&OB->Sdoxlo)
			{
				OB=NULL;
				HeadOBJIndex=0xFFFF;
			}
		}
		addrand(BR->ID);
		int dds=0;
		if(OB)dds=Norma(PointsXY[HeadIndex*2]-(OB->RealX>>4),PointsXY[HeadIndex*2+1]-(OB->RealY>>4));
		if(tmtmt%20)
			IsEnemyOnWay=GoAway();
		if(HeadIndex==-1)
		{
			if(!IsEnemyOnWay)
			{
				SetNextPosAndDest(BR);
				LastMoveTime=tmtmt;
			}
		}
		else
		if(dds<100||(HeadIndex<30&&dds<150)) 
		{
			if(HeadIndex<(MaxPIndex-1)&&(RoadFlag!=1))
			{
				if(!IsEnemyOnWay)
				{
					addrand(BR->ID);
					SetNextPosAndDest(BR);
					LastMoveTime=tmtmt;
				}
			}
		}
		else
		{
			if(HeadOBJIndex!=0xFFFF)
			{
				OB=Group[HeadOBJIndex];
				if(OB&&(OB->StandTime>40||(tmtmt-LastStepTime)>40))
				{
					HeadOBJIndex=0xFFFF;
					if(BR->NMemb>20&&(BR->Memb[3]==0xFFFF||BR->Memb[4]==0xFFFF||BR->Memb[5]==0xFFFF))
					{
						RangeInLineMemb(BR);
						HeadOBJIndex=0xFFFF;
					}
					if(!IsEnemyOnWay)
					{
						addrand(BR->ID);
						SetNextPosAndDest(BR);
						LastMoveTime=tmtmt;
					}
				}
			}
			else
			{
				addrand(BR->ID);
				SetUnitsSpeed(BR);
			}
		}
	}
	addrand(BR->ID);
	return true;
}
void BrigadeOrder_GoOnRoad::SetNextPosAndDest(Brigade *BR)
{
	addrand(BR->ID);
	if(BR&&BR->WarType&&(HeadIndex<(MaxPIndex-1)))
	{
		HeadIndex++;
		LastStepTime=tmtmt;
		if(FirstStep)
		{
			// определяем место офицерского состава (спереди-сзади:))
			// и порядок юнитов в колонне
			OffPlaseHead=true;
			if(BR->Memb[0]!=0xFFFF)
			{
				OneObject* OB=Group[BR->Memb[0]];
				if(OB)
				{
					if(Norma((OB->RealX>>4)-PointsXY[(HeadIndex)*2],(OB->RealY>>4)-PointsXY[(HeadIndex)*2+1])>300)
						OffPlaseHead=false;
				}
			}
			bool inHead=false;
			bool inBack=false;
			for(int i=3;i<(UnitsInLine+3);i++)
				if(BR->Memb[i]!=0xFFFF)
				{
					OneObject* OB=Group[BR->Memb[i]];
					if(OB)
					{
						if(Norma((OB->RealX>>4)-PointsXY[(HeadIndex)*2],(OB->RealY>>4)-PointsXY[(HeadIndex)*2+1])<300)
							inHead=true;
					}
				}
				for(int i=(BR->NMemb-UnitsInLine);i<BR->NMemb;i++)
					if(BR->Memb[i]!=0xFFFF)
					{
						OneObject* OB=Group[BR->Memb[i]];
						if(OB)
						{
							if(Norma((OB->RealX>>4)-PointsXY[(HeadIndex)*2],(OB->RealY>>4)-PointsXY[(HeadIndex)*2+1])<300)
								inBack=true;
						}
					}

					if(inHead)
					{
						//идем в томже порядке
						//RangeInLineMemb(BR);
					}
					else
						if(inBack)
						{
							//реверс
							int N=BR->NMemb-3;
							for(int j=0;j<N;j++)
							{
								SA[j]=BR->Memb[BR->NMemb-j-1];
							}
							for(int i=0;i<N;i++)
							{
								BR->Memb[i+3]=SA[i];
								if(BR->Memb[i+3]!=0xFFFF)
								{
									OneObject* OB=Group[BR->Memb[i+3]];
									if(OB)
									{
										OB->BrIndex=i+3;
										BR->MembSN[i+3]=OB->Serial;
									}
								}
							}
							//RangeInLineMemb(BR);
						}
						else
						{
							// не то и не се - сортируем по новому 
							SortBrigUnits(BR,PointsXY[(HeadIndex)*2],PointsXY[(HeadIndex)*2+1]);
							//RangeInLineMemb(BR);
						}

						FirstStep=false;
		}
		HeadOBJIndex=0xFFFF;
		if(OffPlaseHead)
		{
			SetOffPoss(HeadIndex,BR);
		}
		else
		{
			//if((HeadIndex-NLines)>=0)
				//SetOffPoss(HeadIndex-NLines,BR);
			SetOffPoss(max(HeadIndex-4,0),BR);
		}
		int AvrDist=0;
		int NMemb=0;

		int BestHD=0xFFFF;
		int MinHDst=10000;

		for(int i=3;i<BR->NMemb;i++)
		{
			if(BR->Memb[i]!=0xFFFF)
			{
				OneObject* OB=Group[BR->Memb[i]];
				if(OB)
				{
					int ln=(i-3)/UnitsInLine;
					int pi=HeadIndex-ln*PassNPoints;
					if(OffPlaseHead)
						pi--;
					if(pi>-1)
					{
						int pos=(i-3)%UnitsInLine; 
						int xx=0,yy=0;
						GetUnitCoordInColumn(pi,pos, xx, yy);
						int AD=Norma((OB->RealX>>4)-xx,(OB->RealY>>4)-yy);
						if(AD<300)
						{
							AvrDist+=AD;
							NMemb++;
						}
						if(OB->NewState!=0){
							OB->NewState=0;
							TryToStand(OB,0);
						}
						OB->DestX=xx<<4; 
						OB->DestY=yy<<4;
						addrand(OB->DestX);
						addrand(OB->DestY);
						BR->posX[i]=xx<<4;
						BR->posY[i]=yy<<4; 
						if(HeadOBJIndex==0xFFFF&&pos==(UnitsInLine/2)){
                            if(AD<400)
							{
								OneObject* OO=Group[BR->Memb[i]];
								if(OO&&!OO->Sdoxlo)
								{
									HeadOBJIndex=BR->Memb[i];
								}
							}else{
								if(AD<MinHDst){
									OneObject* OO=Group[BR->Memb[i]];
									if(OO&&!OO->Sdoxlo)
									{
										MinHDst=AD;
										BestHD=BR->Memb[i];
									}
								}
							}
						}
					}
				}
			}
		}
		if(HeadOBJIndex==0xFFFF){
			HeadOBJIndex=BestHD;
		}

		if(NMemb)
		{
			AvrDist/=NMemb;
			int LastSpeed=0;
			/*
			if(BR->Memb[0]!=0xFFFF)
			{
				OneObject* OB=Group[BR->Memb[0]];
				if(OB)
				{
					int ds=Norma((OB->RealX>>4)-(OB->DestX>>4),(OB->RealY>>4)-(OB->DestY>>4));
					int spd=(RoadSpeed*ds)/(AvrDist+1);
					if(spd<32)spd=32;
					if(spd>240)spd=240; 
					OB->UnitSpeed=spd;
					if(!OffPlaseHead&&(HeadIndex-NLines*PassNPoints)<0)
					{
						int xx=0,yy=0;
						GetUnitCoordInColumn(0,0, xx, yy);
						OB->DestX=xx<<4;
						OB->DestY=yy<<4;
						addrand(OB->DestX);
						addrand(OB->DestY);
						BR->posX[0]=xx<<4;
						BR->posY[0]=yy<<4;
						//OB->UnitSpeed=64; 
						addrand(OB->UnitSpeed);
					}
					//if(HeadIndex<30){
					//OB->UnitSpeed=100; 
					addrand(OB->UnitSpeed);
				}
			}
			if(BR->Memb[1]!=0xFFFF)
			{
				OneObject* OB=Group[BR->Memb[1]];
				if(OB)
				{
					int ds=Norma((OB->RealX>>4)-(OB->DestX>>4),(OB->RealY>>4)-(OB->DestY>>4));
					int spd=(RoadSpeed*ds)/(AvrDist+1);
					if(spd<32)spd=32;
					if(spd>240)spd=240; 
					OB->UnitSpeed=spd;

					if(!OffPlaseHead&&(HeadIndex-NLines*PassNPoints)<0)
					{
						int xx=0,yy=0;
						GetUnitCoordInColumn(0,UnitsInLine/2, xx, yy);
						OB->DestX=xx<<4;
						OB->DestY=yy<<4;
						addrand(OB->DestX);
						addrand(OB->DestY);
						BR->posX[1]=xx<<4;
						BR->posY[1]=yy<<4;
						//OB->UnitSpeed=64; 
					}
					//if(HeadIndex<30)
					//OB->UnitSpeed=100;  
				}
			}
			if(BR->Memb[2]!=0xFFFF)
			{
				OneObject* OB=Group[BR->Memb[2]];
				if(OB)
				{
					int ds=Norma((OB->RealX>>4)-(OB->DestX>>4),(OB->RealY>>4)-(OB->DestY>>4));
					int spd=(RoadSpeed*ds)/(AvrDist+1);
					if(spd<32)spd=32;
					if(spd>240)spd=240; 
					OB->UnitSpeed=spd;
					if(!OffPlaseHead&&(HeadIndex-NLines*PassNPoints)<0)
					{
						int xx=0,yy=0;
						GetUnitCoordInColumn(0,UnitsInLine*PassNPoints-1, xx, yy);
						OB->DestX=xx<<4;
						OB->DestY=yy<<4;
						addrand(OB->DestX);
						addrand(OB->DestY);
						BR->posX[2]=xx<<4;
						BR->posY[2]=yy<<4;
						//OB->UnitSpeed=64; 
					}
					//if(HeadIndex<30)
					//OB->UnitSpeed=100; 
				}
			}
			*/
			for(int k=0;k<3;k++)
			{
				OneObject* OB=Group[BR->Memb[k]];
				if(OB)
				{
					int ds=Norma((OB->RealX>>4)-(OB->DestX>>4),(OB->RealY>>4)-(OB->DestY>>4));

					int spd=(RoadSpeed*ds)/(AvrDist+1);
					if(spd<32)spd=32;
					if(spd>MaxSpdLimit)spd=MaxSpdLimit; 
					OB->UnitSpeed=spd;

					if(HeadIndex<15&&OB->UnitSpeed<72)
					{
						OB->UnitSpeed=100;
					}
				}
			}
			for(int i=3;i<BR->NMemb;i++)
			{
				if(BR->Memb[i]!=0xFFFF)
				{
					OneObject* OB=Group[BR->Memb[i]];
					if(OB)
					{
						int ln=(i-3)/UnitsInLine;
						int pi=HeadIndex-ln*PassNPoints;
						if(OffPlaseHead)
							pi--;
						if(pi>-1)
						{
							int ds=Norma((OB->RealX>>4)-(OB->DestX>>4),(OB->RealY>>4)-(OB->DestY>>4));

							//int spd=(RoadSpeed*ds)/(AvrDist+1);
							if(AvrDist<1)AvrDist=1;
							int spd=(RoadSpeed*ds)/AvrDist;
							if(spd<32)spd=32;
							if(spd>MaxSpdLimit)spd=MaxSpdLimit; 
							OB->UnitSpeed=spd;

							if(HeadIndex<30&&pi<6&&OB->UnitSpeed<72)
								OB->UnitSpeed=100;
							addrand(OB->UnitSpeed);
						}
						else
						{
							if(pi>-4)
							{
								int pos=(i-3)%UnitsInLine; 
								int xx=0,yy=0;
								GetUnitCoordInColumn(0,pos, xx, yy);
								OB->DestX=xx<<4;
								OB->DestY=yy<<4;
								addrand(OB->DestX);
								addrand(OB->DestY);
								BR->posX[i]=xx<<4;
								BR->posY[i]=yy<<4;
								//int ds=Norma((OB->RealX>>4)-(OB->DestX>>4),(OB->RealY>>4)-(OB->DestY>>4));
								//OB->UnitSpeed=(32*ds)/AvrDist+pi*5;  
								//if(OB->UnitSpeed<0)
								//OB->UnitSpeed=0;
								OB->UnitSpeed=32;							
							}
							else
							{
								int pos=(i-3)%UnitsInLine; 
								int xx=0,yy=0;
								GetUnitCoordInColumn(0,pos, xx, yy);
								OB->DestX=xx<<4;
								OB->DestY=yy<<4;
								addrand(OB->DestX);
								addrand(OB->DestY);
								BR->posX[i]=xx<<4;
								BR->posY[i]=yy<<4;
								OB->UnitSpeed=32;//0;
								/*
								byte ddr=GetDir((OB->RealX>>4)-xx,(OB->RealY>>4)-yy);
								if(OB->RealDir!=ddr)
								{
								OB->DeleteLastOrder();
								OB->RealDir=ddr;
								OB->GraphDir=OB->RealDir;
								}
								*/
							}
						}
					}
				}
			}
		}
	}
	addrand(BR->ID);
}
void BrigadeOrder_GoOnRoad::GetUnitCoordInColumn(int PointI,byte ColumnI, int &xxx, int &yyy)
{
	if(PointI>-1&&PointI<MaxPIndex)
	{
		int bx=PointsXY[PointI*2];
		int by=PointsXY[PointI*2+1];
		xxx=bx-((ShiftInColumn[ColumnI]*TSin[PointsDir[PointI]])>>8);
		yyy=by+((ShiftInColumn[ColumnI]*TCos[PointsDir[PointI]])>>8);
	}
}
bool BrigadeOrder_GoOnRoad::Init(Brigade *BR)
{
	LastMoveTime=0;
	MaxPIndex=0;
	HeadIndex=-1;
	HeadOBJIndex=0xFFFF;
	lockType=GetBrigLockType(BR);
	int H=100;
	DistInColumn=16;
	PassNPoints=1;
	BR->AttEnm=false;
	for(int i=3;i<BR->NMemb;i++)
	{
		if(BR->Memb[i]!=0xFFFF)
		{
			OneObject* OB = Group[BR->Memb[i]];
			if(OB)
			{
				/*
				DistInColumn=OB->newMons->Radius2/10+2;
				PassNPoints+=(DistInColumn/19)*3;
				*/
				DistInColumn=OB->newMons->Radius2/14+4;
				PassNPoints+=((OB->newMons->Radius2/10+2)/19)*3;
				OB->EnemyID=0xFFFF;
				OB->EnemySN=0xFFFF;
				OB->Attack=false;
				break;
			}
		}
	}
	UnitsInLine=H/DistInColumn;
	int Hig=(DistInColumn*(UnitsInLine-1))/2;
	for(int i=0;i<UnitsInLine;i++)
		ShiftInColumn[i]=i*DistInColumn-Hig;
	NLines=BR->NMemb/UnitsInLine+1;
	if(BR->NMemb%UnitsInLine)NLines++;
	for(int i=3;i<BR->NMemb;i++)
		SA[i-3]=i;
	FirstStep=true;
	OffPlaseHead=true;
	Owner=BR->CT->NI<<16;
	Owner+=BR->ID;
	RoadFlag=0;
	LastStepTime=0;
	return true;
}
bool BrigadeOrder_GoOnRoad::Process(Brigade *BR)
{
	addrand(BR->ID);
	if(DestTopZone==-1)
	{
		BR->DeleteNewBOrder();
		return true;
	}
	if(!ProcessPre(BR))
	{
		if(MaxPIndex){
			int x,y;
			byte ddr;
			BR->GetCenter(&x,&y,&ddr);
			addrand(BR->ID);
			addrand(PointsXY[(MaxPIndex-1)*2]<<4);
			addrand(PointsXY[(MaxPIndex-1)*2+1]<<4);
			addrand(MaxPIndex);
			BR->Direction=ddr;
			BR->CreateOrderedPositions(PointsXY[(MaxPIndex-1)*2]<<4,PointsXY[(MaxPIndex-1)*2+1]<<4,ddr);
			BR->ResortMembByPos();
			int p=Prio;
			BR->DeleteNewBOrder();
			BR->KeepPositions(1,p);
		}
	}
	return true;
}
byte BrigadeOrder_GoOnRoad::GetBrigadeOrderID()
{
	return BRIGADEORDER_GOONROAD;
}
bool BrigadeOrder_GoOnRoad::IsFast()
{
	return true;
}
//=================================================================================================================//
BrigadeOrder_Bitva::BrigadeOrder_Bitva()
{
	StartTop=0xFFFF;
	memset(BitMask,0,1024*8);
	memset(Enm,0,512*2);
	memset(EnSN,0,512*2);
	memset(NDang,0,512);
	NEn=0;
}
word GetTopFast(int x,int y);
void BrigadeOrder_Bitva::AddEnemXY(int x,int y,int MyTop,byte mask){
	if(NEn>=512)return;
	int x0=x<<1;
	int y0=y<<1;

	int HimTop1=GetTopFast(x0,y0);
	int HimTop2=GetTopFast(x0+1,y0);
	int HimTop3=GetTopFast(x0,y0+1);
	int HimTop4=GetTopFast(x0+1,y0+1);
	if(HimTop1>=0xFFFE&&HimTop2>=0xFFFE&&HimTop3>=0xFFFE&&HimTop4>=0xFFFE)return;
	int NTP=MyTop*GetNAreas();
	if(HimTop1<0xFFFE){
		if(HimTop1!=MyTop&&GetLinksDist(HimTop1+NTP)>30)return;
	};
	if(HimTop2<0xFFFE){
		if(HimTop2!=MyTop&&GetLinksDist(HimTop2+NTP)>30)return;
	};
	if(HimTop3<0xFFFE){
		if(HimTop3!=MyTop&&GetLinksDist(HimTop3+NTP)>30)return;
	};
	if(HimTop4<0xFFFE){
		if(HimTop4!=MyTop&&GetLinksDist(HimTop4+NTP)>30)return;
	};
	int cell=x+1+((y+1)<<VAL_SHFCX);
	if(cell>0&&cell<MAXCIOFS){
		int NMon=MCount[cell];
		if(!NMon)return;
		int ofs1=cell<<SHFCELL;
		word MID;
		for(int i=0;i<NMon;i++){
			MID=GetNMSL(ofs1+i);
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&!(OB->NMask&mask||OB->Sdoxlo||OB->LockType==1)){
					int id=OB->Index;
					int ms=1<<(id&7);
					int of=id>>3;
					if(!(BitMask[of]&ms)){
						BitMask[of]|=ms;
						Enm[NEn]=id;
						EnSN[NEn]=OB->Serial;
						int xx=OB->RealX>>10;
						int yy=OB->RealY>>10;
						if(xx>0&&yy>0){
							int off=xx+(yy<<TopSH);
							NDang[NEn]=0;//((InflMap[off-1]&255)+(InflMap[off+1]&255)+(InflMap[off+TopLx]&255)+(InflMap[off-TopLx]&255))>>2;
						}else NDang[NEn]=0;
						NEn++;
						if(NEn>=512)return;
					};
				};
			};
		};
	};
};
bool CheckAttDist(int x,int y,word Victim);
void BrigadeRifleAttack(OneObject *OB);
bool BrigadeOrder_Bitva::Process(Brigade* BR)
{
	addrand(BR->ID);
	for(int i=0;i<BR->NMemb;i++)
	{
		if(BR->Memb[i]!=0xFFFF)
		{
			OneObject* OB = Group[BR->Memb[i]];
			if(OB&&(!OB->Sdoxlo)&&OB->delay==0&&OB->RifleAttack)
			{
				BR->DeleteNewBOrder();
				BrigadeRifleAttack(OB);
				return true;
			}
		}
	}
	//Brigade_Bitva* OR1=(Brigade_Bitva*)BR->BOrder;
	byte Mask=1<<BR->CT->NI;
	//1.Check range of battle
	int xc=0;
	int yc=0;
	BR->GetCenter(&xc,&yc);
	if(rando()<1024||StartTop==0xFFFF){
		word Top=0xFFFF;
		int MinX2=10000000;
		int MinY2=10000000;
		int MaxX2=0;
		int MaxY2=0;	
		int N=BR->NMemb;
		word* Mem=BR->Memb;
		word* MSN=BR->MembSN;
		for(int j=0;j<N;j++){
			word MID=Mem[j];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==MSN[j]){
					if(Top==0xFFFF){
						Top=GetTopology(OB->RealX>>4,OB->RealY>>4);
						StartTop=Top;
					};
					int xx=OB->RealX>>11;
					int yy=OB->RealY>>11;
					if(xx<MinX2)MinX2=xx;
					if(yy<MinY2)MinY2=yy;
					if(xx>MaxX2)MaxX2=xx;
					if(yy>MaxY2)MaxY2=yy;
				};
			};
		};
		if(MaxX>=MinX){
			MinX2-=4;
			MaxX2+=4;
			MinY2-=4;
			MaxY2+=4;
			if(MinX<0)MinX=0;
			if(MinY<0)MinY=0;
			MinX=MinX2;
			MaxX=MaxX2;
			MinY=MinY2;
			MaxY=MaxY2;
		}else{
			BR->DeleteNewBOrder();
			return true;
		};
	};
	//2.renew enemy list
	if(rando()<1024){
		int N=NEn;
		word* Mem=Enm;
		word* MSN=EnSN;
		byte* ATT=NDang;
		for(int i=0;i<N;i++){
			OneObject* OB=Group[Mem[i]];
			if((!OB)||(OB&&(OB->Sdoxlo||OB->Serial!=MSN[i]))){
				//delete enemy from list
				if(i<N-1){
					memcpy(Mem+i,Mem+i+1,(N-i-1)<<1);
					memcpy(MSN+i,MSN+i+1,(N-i-1)<<1);
					memcpy(ATT+i,ATT+i+1,N-i-1);
				};
				i--;
				N--;
			};
		};
		NEn=N;
	};
	int MaxAttServ=64;
	bool InBattle=false;
	//3.Adding new units to battle
	int MinX2=MinX;
	int MinY2=MinY;
	int Dx=MaxX-MinX+1;
	int Dy=MaxY-MinY+1;
	for(int p=0;p<64;p++){
		int xx=((int(rando())*Dx)>>15)+MinX;
		int yy=((int(rando())*Dy)>>15)+MinY;		
		AddEnemXY(xx,yy,StartTop,Mask);
	};
	//4.Attack service
	bool MorPresent=false;
	int N=BR->NMemb;
	word* Mem=BR->Memb;
	word* MSN=BR->MembSN;

	bool SomeoneAtt=false;
	for(int j=0;j<N;j++){
		word MID=Mem[j];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==MSN[j]){
				if(OB->EnemyID!=0xFFFF){
					OneObject* EOB=Group[OB->EnemyID];
					if(EOB && EOB->BrigadeID!=0xFFFF && Norma(EOB->RealX-OB->RealX,EOB->RealY-OB->RealY)<120*16){
						SomeoneAtt=true;
					}
				}
			}
		}
	}


	for(int j=0;j<N;j++){
		word MID=Mem[j];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			void NewAttackPointLink(OneObject* OBJ);			
			if(OB&&OB->Serial==MSN[j]&&!OB->newMons->Artilery){
				if(OB->EnemyID!=0xFFFF){
					if(!(CheckAttDist(xc,yc,OB->EnemyID)||OB->RifleAttack)){
						OB->ClearOrders();
					}else InBattle=true;
				}else{
					if((!(OB->LocalOrder))||(OB->EnemyID==0xFFFF&&CheckIfPossibleToBreakOrder(OB)&&OB->ActivityState<2)){
						//need to find enemy
						NewMonster* NM=OB->newMons;
						AdvCharacter* ADC=OB->MoreCharacter;
						byte mms=NM->KillMask;
						int MinR=ADC->MinR_Attack;
						int MaxR=ADC->MaxR_Attack;
						int MaxMaxR=300; //GEC Radius Ubegania za vragom ot centra stroia
						int ARD=OB->newMons->ArmRadius;
						if(!OB->RifleAttack){
							if(ARD<200)ARD=200;
							MaxR=ARD;							
						}
						int myx=OB->RealX;
						int myy=OB->RealY;
						if(MaxR){
							//search best enemy
							int NearDist=1000000;
							int ReadyDist=1000000;
							if(!OB->RifleAttack){
								//NearDist=ARD;
								//ReadyDist=ARD;
							}
							word NearMID=0xFFFF;
							word ReadyMID=0xFFFF;
							//int NEn=NEn;
							word* Mem=Enm;
							word* MSN=EnSN;
							byte* DANG=NDang;
							char mdr=OB->RealDir;
							for(int t=0;t<NEn;t++){
								word MID=Mem[t];
								OneObject* EOB=Group[MID];
								int dan=DANG[t];
								NewMonster* ENM=NULL;
								if(EOB)ENM=EOB->newMons;
								if(EOB&&(ENM->MathMask&mms)&&(!EOB->Sdoxlo)&&EOB->Serial==MSN[t]){
									int Eusage=ENM->Usage;
									if(Eusage==MortiraID)MorPresent=true;
									//nash paren!
									int R=Norma(myx-EOB->RealX,myy-EOB->RealY)>>4;
									if(R>MinR){
										int NA=ATTLIST.GetNAttackers(EOB);
										int RE=R+NA*160*16;
										if((RE<NearDist&&(dan<7||R<MaxR||MorPresent))||(RE<NearDist&&OB->SearchOnlyThisBrigadeToKill!=0xFFFF&&EOB->BrigadeID==OB->SearchOnlyThisBrigadeToKill&&R<MaxMaxR)){
											//checking for direction of motion
											if(EOB->InMotion){
												int dr1=GetDir(EOB->RealX-myx,EOB->RealY-myy);
												char dr=EOB->RealDir-dr1;
												if(abs(dr)>64){
													NearMID=MID;
													NearDist=RE;
												};
											}else{
												NearMID=MID;
												NearDist=RE;
											};
										};
										if(R<MaxR&&RE<ReadyDist){
											ReadyMID=MID;
											ReadyDist=RE;
										};
									};
								};
							};
							if(ReadyMID==0xFFFF)ReadyMID=NearMID;
							if(ReadyMID!=0xFFFF&&(CheckAttDist(xc,yc,ReadyMID)||OB->RifleAttack)){								
								OneObject* EOB=Group[ReadyMID];
								int R=Norma(OB->RealX-EOB->RealX,OB->RealY-EOB->RealY)>>4;
								if(OB->RifleAttack||R<100||(BR->OnlyThisBrigadeToKill!=0xFFFF&&EOB->BrigadeID==BR->OnlyThisBrigadeToKill)){
									if( SomeoneAtt && BR->OnlyThisBrigadeToKill!=0xFFFF ){
										OB->SearchOnlyThisBrigadeToKill=BR->OnlyThisBrigadeToKill;
									}
									if(!OB->AttackObj(ReadyMID,128+8)){
										if(Norma((BR->posX[j]<<4)-OB->RealX,(BR->posY[j]<<4)-OB->RealY)>16*16){
											OB->NewMonsterPreciseSendTo(BR->posX[j]<<4,BR->posY[j]<<4,128+1,0);
										}
									}else InBattle=true;
								}else{
									if(Norma((BR->posX[j]<<4)-OB->RealX,(BR->posY[j]<<4)-OB->RealY)>16*16){
										OB->NewMonsterPreciseSendTo(BR->posX[j]<<4,BR->posY[j]<<4,128+1,0);
									}
								}								
							}else{
								addrand(128+1);
								if(Norma((BR->posX[j]<<4)-OB->RealX,(BR->posY[j]<<4)-OB->RealY)>16*16){
									OB->NewMonsterPreciseSendTo(BR->posX[j]<<4,BR->posY[j]<<4,128+1,0);
								}
							};
						};
					}else{
						//if(!(rando()<8192&&!OB->RifleAttack))InBattle=true;
					}
				}
			}
		}
	}
	if(!InBattle){
		BR->DeleteNewBOrder();
	};
	return true;
};
byte BrigadeOrder_Bitva::GetBrigadeOrderID()
{
	return BRIGADEORDER_BITVA;
}
//=================================================================================================================//

bool BrigadeOrder_HumanEscape::Process(Brigade* BR)
{
	addrand(BR->ID);
	if(BR->NMemb<4){
		BR->DeleteNewBOrder();
		addrand(654);
		return true;
	};
	//B_HSend* BS=(B_HSend*)BR->BOrder;
	//int x=BS->x;
	//int y=BS->y;
	
	int xc=0;
	int yc=0;
	int N=0;
	int AR=0;
	for(int i=NBPERSONAL;i<BR->NMemb;i++){
		word MID=BR->Memb[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&!OB->Sdoxlo){
				xc+=OB->RealX;
				yc+=OB->RealY;
				N++;
				if(BR->PosCreated){
					//AR+=Norma(BR->posX[i]-(OB->RealX>>4),BR->posY[i]-(OB->RealY>>4));
				};
			}
		};
	};
	if(N){
		xc/=N;
		yc/=N;
		AR/=N;
	}else return true;
	if(Norma(x-xc,y-yc)<256){
		BR->DeleteNewBOrder();
		return true;
	};
	byte Dir2;
	if(Dir>256)Dir2=GetDir((x-xc)>>4,(y-yc)>>4);
	else Dir2=Dir;
	//Dir=FilterDir(Dir);
	//char dd=char(Dir2)-char(BR->Direction);
	byte ND = GetBrigadeDirectionByUnitPositions(BR);
	char dd=char(Dir2)-char(ND);
	byte pri=Prio;
	if(abs(dd)<32||AR>128){
		addrand(x);
		addrand(y);
		//BR->CreateSimpleOrderedPositions(x,y,Dir);
		BR->CreateOrderedPositions(x,y,Dir);
		/*
		if(AR>32&&dd>0){
			BR->ResortMembByPos();
		}
		*/
		BR->Direction=Dir;
	}else{
		OrderDescription* ODE=ElementaryOrders+BR->WarType-1;
		if(ODE->Sym4f){
			//maximal symmetry(rotation pi/4)
			if(dd>=32&&dd<96)ApplySwap(BR,ODE->Sym4f);
			else if(dd<=-32&&dd>-96)ApplySwap(BR,ODE->Sym4i);
			else ApplySwap(BR,ODE->SymInv);
			addrand(x);
			addrand(y);
			//BR->CreateSimpleOrderedPositions(x,y,Dir);
			BR->CreateOrderedPositions(x,y,Dir);
			BR->Direction=Dir;
		}else
			if(ODE->SymInv){
				if(dd>64||dd<-64)ApplySwap(BR,ODE->SymInv);
				addrand(x);
				addrand(y);
				//BR->CreateSimpleOrderedPositions(x,y,Dir);
				BR->CreateOrderedPositions(x,y,Dir);
				BR->Direction=Dir;
			}else{
				if(dd>0){
					addrand(x);
					addrand(y);
					//BR->CreateSimpleOrderedPositions(x,y,BR->Direction+32);
					BR->CreateOrderedPositions(x,y,BR->Direction+32);
					BR->Direction+=32;
				}else{
					addrand(x);
					addrand(y);
					//BR->CreateSimpleOrderedPositions(x,y,BR->Direction-32);
					BR->CreateOrderedPositions(x,y,BR->Direction-32);
					BR->Direction-=32;
				};
			};
	};
	for(int i=0;i<BR->NMemb;i++){
		word MID=BR->Memb[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&!OB->Sdoxlo){
				addrand(pri);
				addrand(BR->posX[i]);
				addrand(BR->posY[i]);
				OB->NewMonsterPreciseSendTo(int(BR->posX[i])<<4,int(BR->posY[i])<<4,pri,0);
			};
		};
	};
	byte Ord=OrdType;
	BR->DeleteNewBOrder();
	addrand(pri);
	BR->KeepPositions(Ord,pri);
	return true;
}
//=================================================================================================================//
bool BrigadeOrder_KeepPositions::Process(Brigade *BR)
{
	addrand(BR->ID);
	byte pri=GetBrigadeOrderPrio();
	bool InAttack=0;
	int dt=(AnimTime-OrderTime)>>8;
	if(pri==128){
		word* Memb=BR->Memb;
		int N=BR->NMemb;
		for(int i=0;i<N;i++){
			word MID=Memb[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&(!OB->Sdoxlo)&&(OB->Attack&&!OB->ActivityState==1))return true;
				if(OB&&(!OB->Sdoxlo)&&OB->delay==0&&OB->RifleAttack)
				{
					BrigadeRifleAttack(OB);
					return true;
				}
			};
		};
	};
	if(BR->PosCreated){
		// Change unit speed
		int RS=0;
		int RS2=0;
		int RSOld=0;
		int NR=0;
		int i=0;
		int Min=9999999;
		int cx=0;
		int cy=0;
		int dx=0;
		int dy=0;
		int Dstt=0;
		for(i=0;i<BR->NMemb;i++)
		{
			word MID=BR->Memb[i];
			if(MID!=0xFFFF)
			{
				OneObject* OB=Group[MID];
				if(OB&&!OB->Sdoxlo)
				{
					//if(!OB->newMons->BRandomSpeed)
					{
						int R=Norma((OB->RealX>>4)-BR->posX[i],(OB->RealY>>4)-BR->posY[i]);
						if(R<Min)
						{
							Min=R;
						}
						cx+=OB->RealX>>4;
						cy+=OB->RealY>>4;
						dx+=BR->posX[i];
						dy+=BR->posY[i];
						addrand(BR->posX[i]+BR->posY[i]);
						addrand(OB->RealX+OB->RealY);
						addname(OB->Index);
						NR++;
					}
				}
			}
		}
		if(NR)
		{
			cx/=NR;
			cy/=NR;
			dx/=NR;
			dy/=NR;
			Dstt=Norma(cx-dx,cy-dy);
			addrand(Dstt);
		}
		for(i=0;i<BR->NMemb;i++)
		{
			word MID=BR->Memb[i];
			if(MID!=0xFFFF)
			{
				OneObject* OB=Group[MID];
				if(OB&&!OB->Sdoxlo)
				{
					//if(!OB->newMons->BRandomSpeed)
					{
						int R=Norma((OB->RealX>>4)-BR->posX[i],(OB->RealY>>4)-BR->posY[i]);
						addrand(R);
						RSOld+=R;
						if(Dstt>200)
						{
							R=R-(Min/2);
						}
						int V=OB->newMons->MotionDist;
						if(OB->NewState!=0&&OB->newMons->Rate[0]!=16)
							V=(V*OB->newMons->Rate[0])>>4;
						//RS+=R*R;
						//RS2+=R;
						R*=48;
						if(V)
							R/=V;
						RS+=R;
						RS2++;
					}
				}
			}
		}

		if(RS2)
		{
			RS=RS/RS2;
			//RSOld=RSOld/NR;
			//if(RSOld>1000)
			//RS=RSOld;
		}

		if(RS>0)
		{
			int AvrV=0;
			int NV=0;
			for(i=0;i<BR->NMemb;i++)
			{
				word MID=BR->Memb[i];
				if(MID!=0xFFFF)
				{
					OneObject* OB=Group[MID];
					if(OB&&!OB->Sdoxlo)
					{
						addrand(OB->BrIndex);
						addrand(OB->Serial);
						addname(OB->Index);
						addrand(OB->RealX);
						addrand(OB->RealY);
						addrand(BR->posX[i]);
						addrand(BR->posY[i]);
						addrand(i);
						int R=Norma((OB->RealX>>4)-BR->posX[i],(OB->RealY>>4)-BR->posY[i]);
						if(Dstt>200)
						{
							R=R-(Min/2);
							addrand(R);
						}
						addrand(R);
						addrand(Min);
						R*=48;
						int Vu=OB->newMons->MotionDist;
						if(OB->NewState!=0&&OB->newMons->Rate[0]!=16)
							Vu=(Vu*OB->newMons->Rate[0])>>4;
						if(Vu)
							R/=Vu;
						addrand(Vu);
						int V=(R*64)/RS;
						addrand(RS);

						if(OB->newMons->BRandomSpeed)
						{
							V+=OB->newMons->BRandomSpeed/2-(OB->Index*V+OB->BrIndex*OB->Serial)%OB->newMons->BRandomSpeed;
						}
						
						if(V>128)V=128;
						if(V<10)V=10;
						addrand(V);
						OB->UnitSpeed=V;
						AvrV+=V;
						NV++;
					}
				}
			}
			
			if(NV&&AvrV/NV<48)
			{
				AvrV/=NV;
				int pr=100*(64-AvrV)/64;
				for(i=0;i<BR->NMemb;i++)
				{
					word MID=BR->Memb[i];
					if(MID!=0xFFFF)
					{
						OneObject* OB=Group[MID];
						if(OB&&!OB->Sdoxlo)
						{
							OB->UnitSpeed=OB->UnitSpeed*(100+pr)/100;
						}
					}
				}
			}
		}
		//
		void NewAttackPointLink(OneObject* OBJ);
		int State=0;
		bool Ready=1;
		if(BR->AttEnm)State=1;
		int Immediate=0;
		if(BR->PosCreated){
			for(int p=NBPERSONAL;p<BR->NMemb;p++){
				word MID=BR->Memb[p];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&!OB->Sdoxlo){
						if(OB->InMotion)Immediate++;
						if(OB->ActivityState==2)State=1;
						if(OB->ActivityState==1)State=0;
					};
				};			
			};
		};
		if(Immediate<6&&!BR->DontWait){
			int cycl=0;
			for(int p=NBPERSONAL;p<BR->NMemb;p++){
				word MID=BR->Memb[p];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];					
					if(OB&&OB->Serial==BR->MembSN[p]){
						cycl=OB->newMons->BrigadeWaitingCycles;
						if(!((OB->ActivityState==2&&OB->Attack)||OB->InMotion||OB->Sdoxlo||(!CheckIfPossibleToBreakOrder(OB)))){
							if(Norma((OB->RealX>>4)-BR->posX[p],(OB->RealY>>4)-BR->posY[p])>5){
								if(OB->NewState!=State){							
										if((!BR->AttEnm)&&OB->Attack)OB->ClearOrders();
									OB->NewState=State;
									OB->GroundState=State;
									addrand(OB->GroundState+1);
									TryToStand(OB,0);
								}
								bool FN=0;
								if(!(OB->LocalNewState==State&&(FN=(OB->FrameFinished||OB->NewAnm->CanBeBrokenAfterFrame<=OB->NewCurSprite)))){
									Ready=0;
									if(!FN){
										OB->SetNextFrame();
									};
								};
								OB->StandTime=0;
								char dd=BR->Direction-OB->RealDir;
								char ad=char(abs(int(dd)));
								if(ad&&!NATIONS[OB->NNUM].AI_Enabled){
									int md=OB->newMons->MinRotator;//*4;
									if(ad<md){
										OB->RealDir=BR->Direction;
									}else{
										if(dd>0)OB->RealDir+=md;
										else OB->RealDir-=md;
									};
									OB->GraphDir=OB->RealDir;
								}
							}
						}
					}
				}
			}
			if(cycl==0)cycl=EngSettings.BrigadeWaitingCycles;
			if(dt<cycl&&!Ready)return true;
		};
		bool Done=true;
		word* Memb=BR->Memb;
		int N=BR->NMemb;
		int NFAIL=0;
		char DIRC=BR->Direction;
		int MAXD=0;
		for(int i=0;i<N;i++){
			word MID=Memb[i];
			int PD=Done;			
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&!(OB->Sdoxlo||(!CheckIfPossibleToBreakOrder(OB)))){
					int xi=int(BR->posX[i])<<4;
					int yi=int(BR->posY[i])<<4;
					int dd=Norma(xi-OB->RealX,yi-OB->RealY);
					if(dd>MAXD&&i>=NBPERSONAL)MAXD=dd;
					if((!OB->LocalOrder)||(OB->EnemyID!=0xFFFF&&(pri&127))){
						word CheckMotionThroughEnemyAbility(OneObject* OB,int px,int py);
						word ENMID=CheckMotionThroughEnemyAbility(OB,OB->RealX,OB->RealY);
						if(OB->ActivityState==2&&OB->Attack)ENMID=OB->EnemyID;
						if(ENMID==0xFFFF){
							int Lx=OB->Lx;
							if(dd>100){
								bool PDN=Done;
								Done=false;
								NFAIL++;
								if(dd<768){
									int xx1=(xi-(Lx<<7))>>8;
									int yy1=(yi-(Lx<<7))>>8;
									if(!UnitsField.CheckBar(xx1,yy1,Lx,Lx)){
										addrand(pri);
										OB->NewMonsterPreciseSendTo(xi,yi,pri,0);
									}else{
										if(RotateUnit(OB,DIRC))Done=false;
										else{
											Done=PDN;
											NFAIL--;
										}
									};
								}else{
									addrand(pri+1);
									OB->NewMonsterPreciseSendTo(xi,yi,pri,0);
								}								
							}else{
								if(RotateUnit(OB,DIRC)){
									Done=false;
									NFAIL++;
								}
							}
						}
					}else{
						Done=false;
						NFAIL++;
					}

				};
			};
			if(PD&&!Done){
				if(rando()<18000)Done=true;
			};
		};
		if(NFAIL<4||(MAXD<1600&&(BR->DontWait||dt>1200)))Done=1;
		if(Done){
			BR->DeleteNewBOrder();
			void MakeStandGroundTemp(Brigade *B);
			MakeStandGroundTemp(BR);
		};		
	};
}
byte BrigadeOrder_KeepPositions::GetBrigadeOrderID()
{
	return BRIGADEORDER_KEEPPOSITION;
}
//=================================================================================================================//
bool BrigadeOrder_HumanGlobalSendTo::Process(Brigade *BR)
{
	addrand(BR->ID);
	//B_SmartSend* BS=(B_SmartSend*)BR->BOrder;
	int x=xx;
	int y=yy;
	//int NextX=BS->NextX;
	//int NextY=BS->NextY;
	//int NextTop=BS->NextTop;
	byte prior=Prio;
	short dir=Dir;
	int xc,yc;
	if(!CalcCenter(BR,&xc,&yc)){
		BR->DeleteNewBOrder();
		return true;
	};
	byte lockType=GetBrigLockType(BR);
	int CTOP=GetTopology(xc>>4,yc>>4,lockType);
	int FinalTop=GetTopology(&x,&y,lockType);
	int bcx=xc;
	int bcy=yc;
#ifdef COSSACKS2
	//checking for roads
	if(BR->WarType){
		OrderDescription* ODS=ElementaryOrders+BR->WarType-1;
		if(ODS->OrdUsage==1||ODS->OrdUsage==3){//square or move
			int NA=GetNAreas(lockType);
			//checking if there is road on the way to final point
			int NTP=CTOP;
			bool CheckIfRoadZone(int Zone);
			if(!CheckIfRoadZone(CTOP)){
				if(ODS->OrdUsage==3&&ODS->GroupID!=0xFF){
					SingleGroup* FGD=FormGrp.Grp+ODS->GroupID;
					CBrigadeOrder* BO=BR->NewBOrder;
					BR->BOrder=NULL;
					MakeReformation(BR->CT->NI,BR->ID,FGD->IDX[1]);
					BR->DontWait=0;
					BR->NewBOrder=BO;
					addrand(Prio|128);
					BR->KeepPositions(1,Prio|128);
					return true;
				};
				int NTP0;
				int NATT=0;
				do{
					NTP0=NTP;
					NTP=GetMotionLinks(FinalTop+NA*NTP,lockType,BR->CT->NI);
					if(NTP<0xFFFE&&NTP!=FinalTop&&CheckIfRoadZone(NTP)){
						Area* AR=GetTopMap(NTP,lockType);
						int xc,yc;
						void GetPreciseTopCenter(int Zone,int LockType,int& x,int& y);
						GetPreciseTopCenter(NTP,lockType,xc,yc);
						BR->HumanGlobalSendTo(xc,yc,512,Prio|128,1);
						if(Prio==128)BR->AttEnm=1;
						return true;
					};
					NATT++;
				}while(NTP<0xFFFE&&NTP!=FinalTop&&NTP!=NTP0&&NATT<10);
			}else{				
NTOP:;
				int RoadsLockType=lockType;
				int CTOPr=GetTopology(xc>>4,yc>>4,RoadsLockType);
				int FinalTopR=GetTopology(&x,&y,RoadsLockType);
				int RoadsNA=GetNAreas(RoadsLockType);
				int NTOP=GetMotionLinks(FinalTopR+RoadsNA*CTOPr,RoadsLockType,BR->CT->NI);
				if(NTOP<0xFFFE&&CheckIfRoadZone(NTOP))
				{
					bool b1=false;
					int NTOP1=GetMotionLinks(FinalTopR+RoadsNA*NTOP,RoadsLockType,BR->CT->NI);
					/*
					if(NTOP1<0xFFFE&&NTOP1!=FinalTopR&&BR->AttEnm==0)
					{
						int NTOP2=GetMotionLinks(FinalTopR+RoadsNA*NTOP1,RoadsLockType,BR->CT->NI);
						if(NTOP2<0xFFFE&&NTOP2!=FinalTopR)
							if(CheckIfRoadZone(NTOP1)&&CheckIfRoadZone(NTOP2))
								b1=true;
					}
					*/					
					
					if(NTOP1<0xFFFE&&NTOP1!=FinalTopR/*&&BR->AttEnm==0*/)
					{
						if( CheckIfRoadZone(NTOP1) ) b1=true;
					}
					
					//b1=true;

					int CTOP3=GetTopology(xc>>4,yc>>4,3);
					int FinalTop3=GetTopology(x,y,3);
					int TopDst=GetLinksDist(FinalTop3+GetNAreas(3)*CTOP3,3,BR->CT->NI)<<6;
					if(b1&&CheckIfRoadZone(CTOPr)&&TopDst>EngSettings.MinTopDistanceToEnterRoad)
					{
						/*
						B_GoOnRoad* OR1=(B_GoOnRoad*)BR->CreateOrder(1,sizeof(B_GoOnRoad));

						DWORD BrigadeID=BR->CT->NI<<16;
						BrigadeID+=BR->ID;
						B_GoOnRoad* OldOr=(B_GoOnRoad*)MusorStorage.GetLastBrigadeLinkData(BrigadeID);
						bool useold=false;
						if(OldOr)
						{
							word ST11=GetMotionLinks(FinalTopR+RoadsNA*CTOPr,RoadsLockType,BR->CT->NI);
							word ST12=GetMotionLinks(OldOr->DestTopZone+RoadsNA*CTOPr,RoadsLockType,BR->CT->NI);
							if(ST12==ST11&&ST11!=0xFFFF)
							{
								word ST21=GetMotionLinks(FinalTopR+RoadsNA*ST11,RoadsLockType,BR->CT->NI);
								word ST22=GetMotionLinks(OldOr->DestTopZone+RoadsNA*ST12,RoadsLockType,BR->CT->NI);
								if(ST22==ST21&&ST21!=0xFFFF)
								{
									useold=true;

									OldOr->OrdType=OR1->OrdType;
									OldOr->Message=OR1->Message;
									OldOr->Next=OR1->Next;
									memcpy(OR1,OldOr,sizeof(B_GoOnRoad));
								}
							}
						}
						if(!useold)
						{
							OR1->Init(BR);
						}
						OR1->Fast=1;
						OR1->BLink=&B_GoOnRoadLink;
						BR->AttEnm=0;
						OR1->Prio=Prio;
						addrand(Prio);
						OR1->DestTopZone=FinalTopR;
						OR1->Size=sizeof(B_GoOnRoad);
						*/
						if(BR->AttEnm){
							BR->AttEnm=false;
							for(int i=NBPERSONAL;i<BR->NMemb;i++){
								word ID=BR->Memb[i];
								if(ID!=0xFFFF){
									OneObject* OB=Group[ID];
									if(OB&&OB->Serial==BR->MembSN[i]){
										if(OB->newMons->ArmAttack){											
											OB->GroundState=0;
											OB->NewState=0;
										}										
									}
								}
							}
						}
						BrigadeOrder_GoOnRoad* Ro =		new BrigadeOrder_GoOnRoad();
						Ro->DestTopZone=FinalTopR;
						Ro->Prio=Prio;
						Ro->Init(BR);
						BR->CreateNewBOrder(1,Ro);
						return true;

						
					}else{
						if(ODS->OrdUsage==3&&ODS->GroupID!=0xFF){
							SingleGroup* FGD=FormGrp.Grp+ODS->GroupID;
							//BrigadeOrder* BO=BR->BOrder;
							CBrigadeOrder* BO=BR->NewBOrder;
							//BR->BOrder=NULL;
							BR->NewBOrder=NULL;
							MakeReformation(BR->CT->NI,BR->ID,FGD->IDX[1]);
							BR->DontWait=0;
							//BR->BOrder=BO;
							BR->NewBOrder=BO;
							addrand(Prio|128);
							BR->KeepPositions(1,Prio|128);
							return true;
						};
					};

				};
			};
		};
		if(ODS->OrdUsage==3&&ODS->GroupID!=0xFF){
			SingleGroup* FGD=FormGrp.Grp+ODS->GroupID;
			//BrigadeOrder* BO=BR->BOrder;
			CBrigadeOrder* BO=BR->NewBOrder;
			//BR->BOrder=NULL;
			BR->NewBOrder=NULL;
			MakeReformation(BR->CT->NI,BR->ID,FGD->IDX[1]);
			BR->DontWait=0;
			//BR->BOrder=BO;
			BR->NewBOrder=BO;
			addrand(Prio|128);
			BR->KeepPositions(1,Prio|128);
			return true;
		};
		BR->DontWait=0;
	};	
#endif
	if(CheckBDirectWay(xc>>4,yc>>4,x,y,lockType)){
		//bool au=EngSettings.AutoChangeFormationType;
		//EngSettings.AutoChangeFormationType=0;
		BR->DeleteNewBOrder();
		//EngSettings.AutoChangeFormationType=au;
		//addrand(33);
		BR->HumanLocalSendTo(x<<4,y<<4,dir,prior,1);
		return true;
	};
	if(NextTop==0xFFFF){
		NextTop=CTOP;//GetTopology(xc>>4,yc>>4,lockType);
		if(NextTop==0xFFFF){
			BR->DeleteNewBOrder();
			return true;
		};
	};

	if(FinalTop==0xFFFF){
		BR->DeleteNewBOrder();
		return true;
	};
	xx=x;
	yy=y;
	int NA=GetNAreas(lockType);
	word NextNextTop=GetMotionLinks(FinalTop+NA*NextTop,lockType,BR->CT->NI);
	if(NextNextTop==FinalTop||FinalTop==NextTop){
		BR->DeleteNewBOrder();
		addrand(34);
		BR->HumanLocalSendTo(x<<4,y<<4,dir,prior,1);
		return true;
	};
	if(NextNextTop!=0xFFFF){
		//atttempt to optomise way
		int MaxPre=5;
		int cox=xc>>4;
		int coy=yc>>4;
		bool NSteps=0;		
		do{
			int Next2=GetMotionLinks(FinalTop+NA*NextNextTop,lockType,BR->CT->NI);
			if(Next2!=0xFFFF){
				Area* AR2=GetTopMap(Next2,lockType);
				int nx=(int(AR2->x)<<6)+32;
				int ny=(int(AR2->y)<<6)+32;
				int rx=nx;
				int ry=ny;
				if(CheckBDirectWay(cox,coy,rx,ry,lockType)||NSteps<1){
					NextNextTop=Next2;
					MaxPre--;
				}else MaxPre=0;
			}else MaxPre=0;
			NSteps++;
		}while(MaxPre);
		Area* NextAr=GetTopMap(NextNextTop,lockType);
		//------------------------
		int nx=(int(NextAr->x)<<6)+32;
		int ny=(int(NextAr->y)<<6)+32;
		NextX=nx;
		NextY=ny;
		NextTop=NextNextTop;
		addrand(35);
		BR->HumanLocalSendTo(nx<<4,ny<<4,512,prior,1);
		return true;
	}else{
		BR->DeleteNewBOrder();
		return true;
	};
	return true;
}
byte BrigadeOrder_HumanGlobalSendTo::GetBrigadeOrderID()
{
	return BRIGADEORDER_HUMANGLOBALSENDTO;
}
//=================================================================================================================//
bool BrigadeOrder_MakeDiversion::Process(Brigade *BR)
{
	addrand(BR->ID);
	if(PeaceTimeLeft)return true;
	City* CT=BR->CT;
	if(CT->DesantStartGroup==0xFFFF)return true;
	//CheckArmies(CT);
	//B_DiversionOrder* DORD=(B_DiversionOrder*)BR->BOrder;
	int MaxInMy=50;
	if(IslPrs[CT->MyIsland]&1)MaxInMy=150;
	if(BR->NMemb){
		OneObject* TRANS=Group[BR->Memb[0]];
		OneObject* CurTrans=NULL;
		if(CT->TransportID!=0xFFFF){
			CurTrans=Group[CT->TransportID];
			if(!(CurTrans&&CurTrans->Serial==CT->TransportSN&&CurTrans->DstX>0)){
				CurTrans=NULL;
				CT->TransportID=0xFFFF;
				CT->TransportSN=0xFFFF;
			};
		};
		if(TRANS&&(!TRANS->Sdoxlo)&&TRANS->Serial==BR->MembSN[0]){
			if(TRANS->StandTime>200&&TRANS->DstX<=0){
				TRANS->Die();
				return true;
			};
			if(TRANS->delay>100&&TRANS->delay<130){
				TRANS->Die();
				return true;
			};
			if(TRANS->DstX>0&&Phase==0){
				if(NU<MaxU){
					if(CurTrans==TRANS){
						OneObject* UNI=NULL;
						UnitsGroup* UG=SCENINF.UGRP+CT->DesantStartGroup;
						CleanGroup(UG);
						if(UG->N){
							UNI=Group[UG->IDS[UG->N-1]];
							UG->N--;
						};
UNIFOUND:
						if(UNI){
							CT->UnRegisterNewUnit(UNI);
							UNI->NewMonsterSmartSendTo(TRANS->DstX>>4,TRANS->DstY>>4,0,0,128+16,0);
							IDX[NU]=UNI->Index;
							USN[NU]=UNI->Serial;
							NU++;
						};
					}else{
						if(CurTrans==NULL){
							CT->TransportID=TRANS->Index;
							CT->TransportSN=TRANS->Serial;
							CurTrans=TRANS;
						};
					};
				}else{
					if(CurTrans==TRANS){
						CT->TransportID=0xFFFF;
						CT->TransportSN=0xFFFF;
						CurTrans=NULL;
					};
					if(TRANS->NInside>=MaxU||tmtmt-LastPassangerTime>512){
						Phase=1;
						CostPointIndex=-1;
						TRANS->DstX=-1;
						TRANS->StandTime=0;
					};
				};
				//conrol of loading
				int x0=TRANS->DstX;
				int y0=TRANS->DstY;
				for(int q=0;q<NU;q++){
					OneObject* OB=Group[IDX[q]];
					bool remove=false;
					if(OB&&OB->Serial==USN[q]){
						if(!OB->Sdoxlo){
							if((!OB->LocalOrder)||(OB->LocalOrder&&OB->LocalOrder->DoLink==&TakeResLink)){
								if(Norma(OB->RealX-x0,OB->RealY-y0)<160*16){
									OB->GoToTransport(TRANS->Index,128+16);
								}else OB->NewMonsterSmartSendTo(x0>>4,y0>>4,0,0,128+16,0);
							};
							LastPassangerTime=tmtmt;
						};
						if(OB->Sdoxlo&&!OB->Hidden)remove=true;
					}else remove=true;
					if(remove){
						if(q<NU-1){
							memcpy(IDX+q,IDX+q+1,(NU-q-1)<<1);
							memcpy(USN+q,USN+q+1,(NU-q-1)<<1);
						};
						q--;
						NU--;
					};
				};
				if(TRANS->NInside){
					if(StartTime>tmtmt){
						StartTime=tmtmt;
					}else{
						if(tmtmt-LastPassangerTime>256){
							for(int q=0;q<NU;q++){
								OneObject* OB=Group[IDX[q]];
								bool remove=false;
								if(OB&&OB->Serial==USN[q]){
									if(!OB->Sdoxlo){
										CT->RegisterNewUnit(OB);
										if(q<NU-1){
											memcpy(IDX+q,IDX+q+1,(NU-q-1)<<1);
											memcpy(USN+q,USN+q+1,(NU-q-1)<<1);
										};
										q--;
										NU--;
									};
								};
							};
							MaxU=NU;
							StartTime=tmtmt;
						};
					};
				};
			}else{
				if(TRANS->DstX<=0&&Phase==0){
					//goto my cost
					if(CostPointIndex==-1){
						CostPointIndex=FindCostPoint(TRANS->RealX>>8,TRANS->RealY>>8,CT->MyIsland);
						if(CostPointIndex!=-1){
							CostPlace* CP=COSTPL+CostPointIndex;
							TRANS->NewMonsterSmartSendTo(CP->xw<<4,CP->yw<<4,0,0,128+16,0);
						};
					};
					if(CostPointIndex!=-1){
						CostPlace* CP=COSTPL+CostPointIndex;
						if((!CP->Access)||(CP->Transport!=0xFFFF&&CP->Transport!=TRANS->Index))CostPointIndex=-1;
						else{
							if(!TRANS->LocalOrder){
								if(Norma((TRANS->RealX>>8)-CP->xw,(TRANS->RealY>>8)-CP->yw)<18){
									TRANS->NewMonsterSmartSendTo(CP->xc<<4,CP->yc<<4,0,0,128+16,0);
								}else TRANS->NewMonsterSmartSendTo(CP->xw<<4,CP->yw<<4,0,0,128+16,0);
							};

						};
					};
				};
				if(Phase==1&&TRANS->DstX<=0){
					if(rando()<512)CostPointIndex=-1;
					if(CostPointIndex==-1){
						CostPointIndex=FindCostPointEx(TRANS->RealX>>8,TRANS->RealY>>8,1<<CT->NI);
						if(CostPointIndex!=-1){
							CostPlace* CP=COSTPL+CostPointIndex;
							TRANS->NewMonsterSmartSendTo(CP->xw<<4,CP->yw<<4,0,0,128+16,0);
						};
					};
					if(CostPointIndex!=-1){
						CostPlace* CP=COSTPL+CostPointIndex;
						if((!CP->Access)||(CP->Transport!=0xFFFF&&CP->Transport!=TRANS->Index))CostPointIndex=-1;
						else{
							int NN=Norma((TRANS->RealX>>8)-CP->xw,(TRANS->RealY>>8)-CP->yw);
							if(NN<40||!TRANS->LocalOrder){
								if(NN<40){
									if(!TRANS->LocalOrder)TRANS->NewMonsterSmartSendTo(CP->xc<<4,CP->yc<<4,0,0,128+16,0);
								}else TRANS->NewMonsterSmartSendTo(CP->xw<<4,CP->yw<<4,0,0,128+16,0);
							};

						};
					};
				};
				if(Phase==1&&TRANS->DstX>0){
					if(TRANS->NInside&&!TRANS->LocalOrder){
						LeaveAll(TRANS);
					};
					//attempt to register units
					int NUN=0;
					for(int i=0;i<NU;i++){
						OneObject* OB=Group[IDX[i]];
						if(OB&&OB->Serial==USN[i]&&OB->BrigadeID==0xFFFF){
							if(OB->Hidden||OB->UnlimitedMotion){
								NUN++;
							}else{
								if(CT->DesantDestGroup!=0xFFFF){
									UnitsGroup* UG=SCENINF.UGRP+CT->DesantDestGroup;
									UG->AddNewUnit(OB);
									if(i<NU-1){
										memcpy(IDX+i,IDX+i+1,(NU-i-1)<<1);
										memcpy(USN+i,USN+i+1,(NU-i-1)<<1);
									};
									NU--;
									i--;
								};
							};
						};
					};
					if(!(TRANS->NInside||NUN)){
						TRANS->DstX=-1;
						Phase=0;
						CostPointIndex=-1;
						NU=0;
						//CT->Brigs[TRANS->BrigadeID].DeleteAll();
						//CT->Brigs[TRANS->BrigadeID].Enabled=false;
						TRANS->delay=350;
						TRANS->StandTime=0;
					};
				};

			};
		}else{
			BR->NMemb=0;
		};
	}else{
		BR->DeleteAll();
		BR->Enabled=false;
	};
	return true;
}
byte BrigadeOrder_MakeDiversion::GetBrigadeOrderID()
{
	return BRIGADEORDER_MAKEDIVERSION;
}
//=================================================================================================================//