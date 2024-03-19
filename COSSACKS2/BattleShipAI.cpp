//------------------------------------------------------------------------------------------------------------------//
#include "stdheader.h"
#include "GameExtension.h"
#include "BattleShipAI.h"
#include "BrigadeAI.h"
//------------------------------------------------------------------------------------------------------------------//
extern int tmtmt;
extern int AnimTime;
extern BrigadeAI WiselyFormations;
void GetPreciseTopCenter(int Zone,int LockType,int& x,int& y);
//------------------------------------------------------------------------------------------------------------------//
TopInfo::TopInfo()
{
	NKillsF=0;
	NKillsE=0;
	NDamageF=0;
	NDamageE=0;
	NShots=0;
	MinDistToEnmBld=0xFFFF;
	MinDistToFrnBld=0xFFFF;
	EnmBldW=0;
	FrdBldW=0;
}
int TopInfo::GetBldW(int Dist)
{
	int rez=0;
	if(Dist<1500)
	{
		rez=(1500-Dist)/100;
	}
	return rez;
}
//------------------------------------------------------------------------------------------------------------------//
NationalNavy::NationalNavy()
{
	NI=-1;
}
NationalNavy::NationalNavy(int N, byte Top)
{
	NI=N;
	TopType=Top;
	ProcessTime=0;
	int NAR=GetNAreas(TopType);
	for(int i=0;i<NAR;i++)
	{
		TopZons.Add(new TopInfo());
	}
	for(int i=0;i<MAXOBJECT;i++)
	{
		OneObject* OB = Group[i];
		if(OB&&OB->NewBuilding)
		{
			RegisterBuilding(OB, 1);
		}
	}
}
NationalNavy::~NationalNavy()
{}
byte NationalNavy::GetMask()
{
	if(NI>-1)
	{
		return NATIONS[NI].NMask;
	}
	return 0;
}
void NationalNavy::RegisterBuilding(OneObject* Blb, int K)
{
	int x=Blb->RealX/16;
	int y=Blb->RealY/16;
	int NAR=GetNAreas(TopType);
	for(int i=0;i<NAR;i++)
	{
		int TopX;
		int TopY;
		GetPreciseTopCenter(i,TopType,TopX,TopY);
		int Dist=Norma(x-TopX,y-TopY);	
		TopInfo* TI = TopZons[i];
		if(TI)
		{
			int W=TI->GetBldW(Dist)*K;
			if(GetMask()&Blb->NMask)
			{
				if(Dist<TI->MinDistToFrnBld)
					TI->MinDistToFrnBld=Dist;
				TI->FrdBldW+=W;
			}
			else
			{
				if(Dist<TI->MinDistToEnmBld)
					TI->MinDistToEnmBld=Dist;
				TI->EnmBldW+=W;
			}
		}
	}
}
void NationalNavy::Process()
{
	if((ProcessTime+256*25*10)<AnimTime)
	{
		int NAR=GetNAreas(TopType);
		int Max=0;
		int Mx=-1;
		int My=-1;
		for(int i=0;i<NAR;i++)
		{
			if(TopZons[i]->EnmBldW>Max)
			{
				GetPreciseTopCenter(i,TopType,Mx,My);
				Max=TopZons[i]->EnmBldW;
			}
		}
		if(Mx!=-1)
		{
			for(int i=0;i<WiselyFormations.MaxAF;i++)
			{
				AutoForm* AF = WiselyFormations.AF[i];
				if(AF&&AF->NI==NI&&AF->C&&AF->C->Usage==LinkorID)
				{
					AF->AimX=Mx;
					AF->AimY=My;
				}
			}
		}
		ProcessTime=AnimTime;
	}
}
//------------------------------------------------------------------------------------------------------------------//
BattleShipAI::BattleShipAI()
{}
BattleShipAI::~BattleShipAI()
{}
void BattleShipAI::ProcessingGame()
{
	int NAi = NationalsNavyInfo.GetAmount();
	if(NAi)
	{
		NationalsNavyInfo[tmtmt%NAi]->Process();
	}
}
void BattleShipAI::OnGameStart()
{}
void BattleShipAI::OnGameEnd()
{
	NationalsNavyInfo.Clear();
}
bool BattleShipAI::OnMapLoading(xmlQuote& xml)
{
	return false;
}
bool BattleShipAI::OnGameSaving(xmlQuote& xml)
{
	return false;
}
void BattleShipAI::OnUnitBirth(OneObject* NewUnit)
{
	if(NewUnit)
	{
		if(NewUnit->Usage==LinkorID)
		{
			if(NATIONS[NewUnit->NNUM].AI_Enabled)
			{
				int n=NationalsNavyInfo.GetAmount();
				bool alst=false;
				for(int i=0;i<n;i++)
				{
					if(NationalsNavyInfo[i]->NI==NewUnit->NNUM)
					{
						alst=true;
						break;
					}
				}
				if(!alst)
				{
					NationalsNavyInfo.Add(new NationalNavy(NewUnit->NNUM,NewUnit->LockType));
				}
				WiselyFormations.AddAutoCannon(NewUnit);
			}
		}
		if(NewUnit->NewBuilding)
		{
			int NAi = NationalsNavyInfo.GetAmount();
			for(int j=0;j<NAi;j++)
			{
				NationalsNavyInfo[j]->RegisterBuilding(NewUnit,1);
			}
		}
	}
}
bool BattleShipAI::OnUnitDie(OneObject* Dead,OneObject* Killer)
{
	if(Dead)
	{
		if(Dead->NewBuilding)
		{
			int NAi = NationalsNavyInfo.GetAmount();
			for(int j=0;j<NAi;j++)
			{
				NationalsNavyInfo[j]->RegisterBuilding(Dead,-1);
			}
		}
	}
	return true;
}
bool BattleShipAI::OnUnitDamage(OneObject* DamagedUnit,OneObject* Damager,int& Damage,byte AttType)
{
	return true;
}
//------------------------------------------------------------------------------------------------------------------//
