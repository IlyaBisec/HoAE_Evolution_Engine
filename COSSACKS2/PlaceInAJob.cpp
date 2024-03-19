#include "stdheader.h"
//-----------------------------------------------------------------------------------------------------------------//
#include "GameExtension.h"
#include "PlaceInAJob.h"
//-----------------------------------------------------------------------------------------------------------------//
extern int AnimTime;
void TakeResLink(OneObject* OBJ);
void TakeResources(UnitsGroup* UGR,byte Type,byte NI,byte ResType,GAMEOBJ* Zone);
void UGR_TakeResourcesLink(UnitsGroup* UGR);
void SetScreenCenterToXY(int x, int y);
//-----------------------------------------------------------------------------------------------------------------//
void PlaceInAJob::OnGameStart()
{
	LastProcessTime=4*25*256;
}
void PlaceInAJob::ProcessingGame()
{
	const byte NRes=6;
	if(GSets.PeasantAutoWork&&(LastProcessTime+30*256)<AnimTime)
	{
		for(byte i=0;i<8;i++)
		{
			if(GSets.CGame.isHumanPlayer(i))
			{
				word* id=NatList[i];
				int   n=NtNUnits[i];
				int NotWork=0;
				int InWork=0;
				int InRes[NRes];
				memset(InRes,0,sizeof InRes);
				int Cx=0;
				int Cy=0;
				for(int j=0;j<n;j++)
				{
					OneObject* OB=Group[id[j]];
					if(OB)
					{
						NewMonster* NM=OB->newMons;
						if(NM->Peasant)
						{
							if(OB->StandTime>200&&!OB->LocalOrder)
							{
								NotWork++;
							}
							else
							{
								Order1* Ord=OB->LocalOrder;
								while(Ord)
								{
									if(Ord->DoLink==&TakeResLink)
									{
										byte ResType=Ord->info.TakeRes.ResID;
										if(ResType<NRes)
										{
											InRes[ResType]++;
											InWork++;
											Cx+=OB->RealX/16;
											Cy+=OB->RealY/16;
											break;
										}
									}
									Ord=Ord->NextOrder;
								}
							}
						}
					}
				}
				if(NotWork&&InWork)
				{
					Cx/=InWork;
					Cy/=InWork;
					static UnitsGroup UG[NRes];
					for(byte k=0;k<NRes;k++)
					{
						UG[k].Clear();
						InRes[k]=(InRes[k]*NotWork*100)/InWork;
					}
					int lastres=-1;
					for(int j=0;j<n;j++)
					{
						OneObject* OB=Group[id[j]];
						if(OB)
						{
							NewMonster* NM=OB->newMons;
							if(NM->Peasant&&OB->StandTime>200&&(!OB->LocalOrder)&&Norma(OB->RealX/16-Cx,OB->RealY/16-Cy)<2000)
							{
								bool st=false;
								for(byte f=0;f<NRes;f++)
								{
									if(InRes[f]>0)
									{
										UG[f].AddNewUnit(OB);
										InRes[f]-=100;
										lastres=f;
										st=true;
									}
								}
								if(!st&&lastres!=-1)
									UG[lastres].AddNewUnit(OB);
							}
						}
					}
					for(byte r=0;r<NRes;r++)
					{
						TakeResources(&UG[r],0,i,r,NULL);
						UGR_TakeResourcesLink(&UG[r]);
					}
				}
			}
		}
		LastProcessTime=AnimTime;
	}
}
void PlaceInAJob::OnInitAfterMapLoading()
{
	//int CurNat=GSets.CGame.cgi_NatRefTBL[MyNation];
	//word* id=NatList[CurNat];
	//int   n=NtNUnits[CurNat];
	//int cx=0;
	//int cy=0;
	//int nc=0;
	//int cpx=0;
	//int cpy=0;
	//int ncp=0;
	//for(int j=0;j<n;j++)
	//{
	//	OneObject* OB=Group[id[j]];
	//	if(OB)
	//	{
	//		NewMonster* NM=OB->newMons;
	//		if(NM->Peasant)
	//		{
	//			cpx+=OB->RealX/16;
	//			cpy+=OB->RealY/16;
	//			ncp++;
	//		}
	//		cx+=OB->RealX/16;
	//		cy+=OB->RealY/16;
	//		nc++;
	//	}
	//}
	//if(ncp)
	//{
	//	SetScreenCenterToXY(cpx/ncp,cpy/ncp);
	//}
	//else
	//	if(nc)
	//	{
	//		SetScreenCenterToXY(cx/nc,cy/nc);
	//	}
}
//-----------------------------------------------------------------------------------------------------------------//
