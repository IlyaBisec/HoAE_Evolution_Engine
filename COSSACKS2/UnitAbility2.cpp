#include "stdheader.h"
#include "unitability.h"
#include "unitability2.h"
#include "AI_Scripts.h"
#include "WeaponSystem.h"
#include "BrigadeAI.h"
#include "cvi_singleplayerdata.h"
#include "cvi_Campaign.h"
#include "UnitAbilityIcon.h"
//-----------------------------------------------------------------------------------------------------------------//
extern int TrueTime;
extern int AnimTime;
extern int tmtmt;
typedef bool tpUnitsCallback(OneObject* OB,void* param);
int PerformActionOverUnitsInRadius(int xc,int yc,int R,tpUnitsCallback* CB,void* Param);
int PerformActionOverBuildingsInRadius(int xc,int yc,int R,tpUnitsCallback* CB,void* Param);
void CreateNewActiveWeapon(char* WMName,int Index,int sx, int sy, int sz, int DestIndex, int dx, int dy, int dz, int Damage, int AttType);
void CreateNewActiveWeapon(char* WMName,int Index,int sx, int sy, int sz, int DestIndex, int dx, int dy, int dz, AdditionalWeaponParams* AddParams);
word GetDir(int dx,int dy);
extern Nation NATIONS[8];
extern int GetHeight(int x, int y);
extern HeroVariableStorage* CurrentHeroAbility;
HeroVariableStorage* GetHeroVariableStorage(OneObject* OB);
void RotateMon(OneObject* OB,char angle);
void PanicUnit(OneObject* OBJ);
void PerformNewUpgrade(Nation* NT,int UIndex,OneObject* OB);
//void CreateNewActiveWeapon(char* WMName,int Index,int sx, int sy, int sz, int DestIndex, int dx, int dy, int dz, AdditionalWeaponParams* AddParams);
void AddGroundCircle(int x,int y,int R,DWORD Color);
extern BrigadeAI WiselyFormations;
typedef bool cbCheckSprite(OneSprite* OS, void* Param);
int GetSpritesInRadius(int x, int y, int Radius, cbCheckSprite* cbF, void* Param);
int GetPointToLineDist(int x,int y,int x1,int y1,int x2,int y2);
CEXPORT bool GetPeaceMode();
extern int vmCampID;
extern ClonesArray<cvi_Campaign> vmCampaigns;
extern veGameMode vGameMode;
char* GetTextByID(char* id);
extern bool vGameLoaing;
void StayForSomeTime(OneObject* OB,byte OrdType,int Time);
int GetPointToLineDistEx(int x,int y,int x1,int y1,int x2,int y2,int& xn,int& yn,int& Weight);
void NewMonsterSendToLink(OneObject* OBJ);
void RotUnitLink(OneObject* OB);
void NewAttackPointLink(OneObject* OBJ);
//-----------------------------------------------------------------------------------------------------------------//
int SumCoord(int x, int y, int &nx, int &ny)
{
	int W=0;
	int cell=x+(y<<VAL_SHFCX);
	int NMon=MCount[cell];
	if(NMon)
	{
		int ofs1=cell<<SHFCELL;
		word MID;
		for(int k=0;k<NMon;k++)
		{
			MID=GetNMSL(ofs1+k);
			if(MID!=0xFFFF)
			{
				OneObject* OB=Group[MID];
				if(OB)
				{
					W++;
					nx+=OB->RealX/16;
					ny+=OB->RealY/16;
				}
			}
		}
	}
	return W;
};
int FindMaxDensityEnemyForce(int cx, int cy, int radius, byte nmask, int &rx, int &ry)
{

	int x1=(cx-radius)/128;
	int y1=(cy-radius)/128;
	int x2=(cx+radius)/128+1;
	int y2=(cy+radius)/128+1;
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
				int dst=Norma(xx-cx,yy-cy);
				if(dst<=radius)
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
								if(OB&&!OB->Sdoxlo)
								{
									if(OB->NMask&nmask)
									{
										W-=2;
									}
									else
									{
										W+=2;
									}
								}
							}
						}
						int k=(i-x1)+xs*(j-y1);
						if(k<siz)
							Map[k]=W;
						else
							k=0;
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
				W+=Map[(i-x1)+xs*(j-y1+1)]/2;
				W+=Map[(i-x1)+xs*(j-y1-1)]/2;
				W+=Map[(i-x1-1)+xs*(j-y1)]/2;
				W+=Map[(i-x1-1)+xs*(j-y1-1)]/2;
				W+=Map[(i-x1-1)+xs*(j-y1+1)]/2;
				W+=Map[(i-x1+1)+xs*(j-y1)]/2;
				W+=Map[(i-x1+1)+xs*(j-y1-1)]/2;
				W+=Map[(i-x1+1)+xs*(j-y1+1)]/2;
				if(W>Max)
				{
					Max=W;
					Mx=i;
					My=j;
				}
			}
		}
	}
	if(Max)
	{
		rx=0;
		ry=0;
		int s=SumCoord(Mx,My,rx,ry);
		s+=SumCoord(Mx,My-1,rx,ry);
		s+=SumCoord(Mx,My+1,rx,ry);
		s+=SumCoord(Mx-1,My,rx,ry);
		s+=SumCoord(Mx-1,My-1,rx,ry);
		s+=SumCoord(Mx-1,My+1,rx,ry);
		s+=SumCoord(Mx+1,My,rx,ry);
		s+=SumCoord(Mx+1,My-1,rx,ry);
		s+=SumCoord(Mx+1,My+1,rx,ry);
		if(s)
		{
			rx/=s;
			ry/=s;
		}
		/*
		rx=Mx*128;//+64;
		ry=My*128;//+64;
		//int x=Mx-x1;
		//int y=My-y1;
		int cell=Mx+(My<<VAL_SHFCX);
		int NMon=MCount[cell];
		rx=cx;
		ry=cy;
		if(NMon)
		{
			int ofs1=cell<<SHFCELL;
			word MID;
			int W=0;
			rx=0;
			ry=0;
			for(int k=0;k<NMon;k++)
			{
				MID=GetNMSL(ofs1+k);
				if(MID!=0xFFFF)
				{
					OneObject* OB=Group[MID];
					if(OB)
					{
						W++;
						rx+=OB->RealX/16;
						ry+=OB->RealY/16;
					}
				}
			}
			if(W)
			{
				rx/=W;
				ry/=W;
			}
		}
		*/
	}
	delete[] Map;
	return Max;
}
//-----------------------------------------------------------------------------------------------------------------//
bool AskGoAwayRef(OneObject* OB, void* p)
{
	bool rez=false;
	if(OB&&!OB->Sdoxlo)
	{
		int* P=(int*)p;
		byte Mask=(byte)P[0];
		if(OB->NMask&Mask&&OB->StandTime>1&&OB->LocalOrder==NULL)
		{
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
					StayForSomeTime(OB,1,5*25*256);
				char mdr=GetDir(P[4]-P[1],P[5]-P[2]);
				char pbd=GetDir(OB->RealX/16-P[1],OB->RealY/16-P[2]);
				OB->Speed=64;

				int Wt,xn,yn;
				int dd=GetPointToLineDistEx(OB->RealX/16,OB->RealY/16,P[1],P[2],P[4],P[5],xn,yn,Wt);
				int raz=P[3]-dd;
				if(raz>0)
				{
					if(abs(mdr-pbd)<65)
					{
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
	}
	return rez;
}
int AskGoAway(OneObject* OB,int Radius, int Width)
{
	int rez=0;
	if(OB&&Radius>0&&Width>0&&OB->DestX>0)
	{
		int cx=OB->RealX/16;
		int cy=OB->RealY/16;
		int dx=OB->DestX/16;
		int dy=OB->DestY/16;
		int R=Norma(cx-dx,cy-dy);
		int P[8];
		P[0]=(int)OB->NMask;
		P[1]=cx;
		P[2]=cy;
		P[3]=Width/2;
		P[4]=dx;
		P[5]=dy;
		rez=PerformActionOverUnitsInRadius(cx,cy,Radius,&AskGoAwayRef,P);
	}
	return rez;
}
//-----------------------------------------------------------------------------------------------------------------//

BigUnit::BigUnit()
{
	Radius=0;
	Width=0;
	LockRadiusIfStand=0;
}
ActiveUnitAbility* BigUnit::GetNewActiveAbility()
{
	return new ActiveBigUnit();
}
//-----------------------------------------------------------------------------------------------------------------//
ActiveBigUnit::ActiveBigUnit()
{
	FirstStand=false;
}
bool ActiveBigUnit::Process(OneObject* OB)
{
	if(OB&&!OB->Sdoxlo)
	{
		BigUnit* B = (BigUnit*) GetA();
		if(B)
		{
			if(OB->DestX>0)
			{
				if(B->Width!=0&&(tmtmt+OB->Index)%3==0)
				{
					AskGoAway(OB,B->Radius,B->Width);
				}
				FirstStand=true;
			}
			else
			{
				if(B->LockRadiusIfStand)
				{
					if(!OB->GLock)
					{
						if(FirstStand)
						{
							OB->Lx=B->LockRadiusIfStand/16;
							//MFIELDS[OB->LockType].SetRound(OB->x,OB->y,OB->Lx);
							//OB->GLock=true;
						}
					}
				}
			}
			return true;
		}
	}
	return false;
}
//-----------------------------------------------------------------------------------------------------------------//
AggressiveMove::AggressiveMove()
{
	Radius=0;
	MinDist=0;
}
ActiveUnitAbility* AggressiveMove::GetNewActiveAbility()
{
	return new ActiveAggressiveMove();
}
//-----------------------------------------------------------------------------------------------------------------//
ActiveAggressiveMove::ActiveAggressiveMove()
{
	m_baseX=-1;
	m_baseY=-1;
	m_dir=-1;
	m_tempX=-1;
	m_tempY=-1;
	m_lastMoveTime=0;
}
bool ActiveAggressiveMove::Process(OneObject* OB)
{
	if(OB&&!OB->Sdoxlo)
	{
		if(OB->ActivityState==2&&(m_lastMoveTime+256*25*5)<AnimTime) //0-normal, 1-peacefull, 2-aggressive
		{
			if(OB->LocalOrder==NULL)
			{
				AggressiveMove* A = (AggressiveMove*) GetA();
				if(A)
				{
					if(OB->StandTime>0)
					{
						int nx, ny;
						int w=FindMaxDensityEnemyForce(OB->RealX/16,OB->RealY/16,A->Radius,OB->NMask,nx,ny);
						if(w>0)
						{
							byte md=GetDir(nx-OB->RealX/16,ny-OB->RealY/16);
							int d1=Norma(nx-OB->RealX/16,ny-OB->RealY/16)+A->MinDist/2;
							if(d1<A->MinDist)
								d1=A->MinDist;
							m_tempX=OB->RealX/16+((d1*TCos[md])>>8);
							m_tempY=OB->RealY/16+((d1*TSin[md])>>8);
							OB->NewMonsterSendTo(m_tempX*16,m_tempY*16,128+16,0);
							m_lastMoveTime=AnimTime;
						}
					}
				}
			}
		}
		return true;
	}
	return false;
}
//-----------------------------------------------------------------------------------------------------------------//
AggressiveSplashFire::AggressiveSplashFire()
{
	Radius=0;
	MinDist=0;
	MinUnits=0;
}
ActiveUnitAbility* AggressiveSplashFire::GetNewActiveAbility()
{
	return new ActiveAggressiveSplashFire();
}
//-----------------------------------------------------------------------------------------------------------------//
ActiveAggressiveSplashFire::ActiveAggressiveSplashFire()
{
	m_lastAttackTime=0;
}
bool ActiveAggressiveSplashFire::Process(OneObject* OB)
{
	if(OB&&!OB->Sdoxlo)
	{
		if(OB->ActivityState==2&&(m_lastAttackTime+256*25*2)<AnimTime) //0-normal, 1-peacefull, 2-aggressive
		{
			//(m_lastMoveTime+256*25*5)<AnimTime
			//OB->LocalOrder==NULL&&
			if(OB->delay==0)
			{
				AggressiveSplashFire* A = (AggressiveSplashFire*) GetA();
				if(A)
				{
					int nx, ny;
					int w=FindMaxDensityEnemyForce(OB->RealX/16,OB->RealY/16,A->Radius,OB->NMask,nx,ny);
					if(w>0&&(w>A->MinUnits*2||A->MinUnits==0))
					{
						if(OB->LocalOrder&&OB->LocalOrder->DoLink!=&NewAttackPointLink)
						{
							OB->NewAttackPoint(nx,ny,128+16,1,0);
						}
						else
						{
							if((OB->DstX/16-nx)>64||(OB->DstY/16-ny)>64)
							{
								OB->DeleteLastOrder();
								OB->NewAttackPoint(nx,ny,128+16,1,0);
							}
						}
					}
					else
					{
						if(OB->LocalOrder&&OB->LocalOrder->DoLink==&NewAttackPointLink)
						{
							OB->DeleteLastOrder();
						}
					}
					m_lastAttackTime=AnimTime;
					/*
					if(OB->StandTime>0)
					{
						int nx, ny;
						int w=FindMaxDensityEnemyForce(OB->RealX/16,OB->RealY/16,A->Radius,OB->NMask,nx,ny);
						if(w>0)
						{
							byte md=GetDir(nx-OB->RealX/16,ny-OB->RealY/16);
							int d1=Norma(nx-OB->RealX/16,ny-OB->RealY/16)+A->MinDist/2;
							if(d1<A->MinDist)
								d1=A->MinDist;
							m_tempX=OB->RealX/16+((d1*TCos[md])>>8);
							m_tempY=OB->RealY/16+((d1*TSin[md])>>8);
							OB->NewMonsterSendTo(m_tempX*16,m_tempY*16,128+16,0);
							m_lastMoveTime=AnimTime;
						}
					}
					*/
				}
			}
		}
		return true;
	}
	return false;
}
//-----------------------------------------------------------------------------------------------------------------//
