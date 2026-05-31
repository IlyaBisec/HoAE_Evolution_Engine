#include "stdheader.h"
void EconomyInterface(IconSet* ISET);
void ClearEconomy();
void LeaveTransportLink(OneObject* OB);
void ShowRLCItemDarkN(int x,int y,lpRLCTable lprt,int n,int Ints);
void ShowRLCItemRedN(int x,int y,lpRLCTable lprt,int n,int Ints);
//extern int RESRC[8][8];
extern int ATTGR_ICON;
extern int AttGrPos;
int GetCell(int xs,int ys);
void GoToMineLink(OneObject* OB);
bool OneObject::GoToMine(word ID,byte Prio){
	return GoToMine(ID,Prio,0);
};
void RemoveFromFormation(OneObject* OB);
//last minute change
void FindUnlockedPoint(int& x,int& y){
    int x0=x>>4;
	int y0=y>>4;
	if(CheckPt(x0,y0)){
		for(int i=0;i<20;i++){
			int N=Rarr[i].N;
			char* xi=Rarr[i].xi;
			char* yi=Rarr[i].yi;
			for(int j=0;j<N;j++){
				int xx=x0+xi[j];
				int yy=y0+yi[j];
				if(!CheckPt(xx,yy)){
					x=xx*16+8;
					y=yy*16+8;
				}
			}
		}
	}
}
bool CheckLeaveContainerAbility(OneObject* OB){
	if(OB->NewBuilding){
		return true;
	}
	if(OB->LockType==1){
		NewMonster* NM=OB->newMons;
		if(NM->ConcPtX.GetAmount()){//changes in last minute
			int COS=TCos[byte(OB->RealDir+64)];
			int SIN=TSin[byte(OB->RealDir+64)];
			int x1=(OB->RealX+((COS*NM->ConcPtX[0]-SIN*NM->ConcPtY[0])/16))/16;
			int y1=(OB->RealY+((SIN*NM->ConcPtX[0]+COS*NM->ConcPtY[0])/16))/16;
			int x2=(OB->RealX+((COS*NM->ConcPtX[0]-SIN*(NM->ConcPtY[0]-100))/16))/16;
			int y2=(OB->RealY+((SIN*NM->ConcPtX[0]+COS*(NM->ConcPtY[0]-100))/16))/16;
			int H2=GetTotalHeight(x2,y2);
			int H1=GetTotalHeight(x1,y1);
			if(H1>130)return false;
			if(H1<=0 && H2<=0)return false;
			FindUnlockedPoint(x1,y1);
			int H=GetTotalHeight(x1,y1);
			return (H>0 && H<130);
		}
		bool CheckIfCostIsNearOB(int x,int y,OneObject* OB);
		int x1=(OB->RealX>>4)+int(TCos[OB->RealDir])*500/256;
		int y1=(OB->RealY>>4)+int(TSin[OB->RealDir])*500/256;
		if(abs(GetTotalHeight(x1,y1)-int(OB->RZ))>150)return false;
		return CheckIfCostIsNearOB(OB->RealX>>4,OB->RealY>>4,OB);
	}else{
		NewMonster* NM=OB->newMons;
		if(NM->ConcPtX.GetAmount()){
			int COS=TCos[byte(OB->RealDir+64)];
			int SIN=TSin[byte(OB->RealDir+64)];		
			int x1=(OB->RealX+((COS*NM->ConcPtX[0]-SIN*NM->ConcPtY[0])/16))/256;
			int y1=(OB->RealY+((SIN*NM->ConcPtX[0]+COS*NM->ConcPtY[0])/16))/256;
			//return !CheckBar(x1-2,y1-2,5,5);			
			return !CheckPt(x1,y1);
		}
	}
	return false;
}
int GetAmountOfMoversTo(OneObject* Dest,OneObject* Mover=NULL){
	if(Dest){
        int N=CITY[Dest->NNUM].TheyMovesInto_prev.GetAmount();
		word* W=CITY[Dest->NNUM].TheyMovesInto_prev.GetValues();
		word* G=CITY[Dest->NNUM].MoversIndices_prev.GetValues();
		int v=0;
		for(int i=0;i<N;i++)if(W[i]==Dest->Index){
			OneObject* OB=Group[G[i]];
			if(OB&&OB!=Mover)v+=OB->newMons->NPlacesInTransport;
		}
        return v;
	}
	return 0;
}
bool CheckPossibilityToComeInside(OneObject* Transport,OneObject* Passanger){
	if(Transport->newMons->InviteMask&Passanger->newMons->VisitorMask){
		int NP=0;
		for(int i=0;i<Transport->NInside;i++){
			word ID=Transport->Inside[i];
			if(ID!=0xFFFF){
				OneObject* OB=Group[ID];
				if(OB)NP+=OB->newMons->NPlacesInTransport;
			}
		}
		NP+=GetAmountOfMoversTo(Transport,Passanger);
		if(NP==0)return true;
		AdvCharacter* ADC=Transport->MoreCharacter;
		return NP+Passanger->newMons->NPlacesInTransport<=ADC->MaxInside+Transport->AddInside;
	}
	return false;
}
bool CheckIfSomeoneMovesIntoThisUnit(OneObject* OB){
	if(!OB->newMons->ShowInsideUnits)return false;
	if(GetAmountOfMoversTo(OB,NULL)!=0)return true;
	for(int i=0;i<OB->NInside;i++){
		word ID=OB->Inside[i];
		if(ID!=0xFFFF){
			OneObject* OBI=Group[ID];			
			if(OBI){
				if(OBI->UnlimitedMotion)return true;
				if(OBI->LocalOrder){				
					void NewMonsterPreciseSendToLink(OneObject* OB);
					void RotUnitLink(OneObject* OB);
					if(OBI->LocalOrder->DoLink==&NewMonsterPreciseSendToLink
						/*||OBI->LocalOrder->DoLink==&RotUnitLink*/)return true;
				}
			}
		}
	}
	return false;
}
bool OneObject::GoToMine(word ID,byte Prio,byte Type){
	if(LockedInBuilding!=0xFFFF)return false;
	GroupSpeed=newMons->MotionDist;
	UnitSpeed=64;
	addrand(Index);
	addrand(ID);
	if(UnlimitedMotion)return false;
	if(CheckOrderAbility())return false;	
	NewMonster* NM=newMons;
	OneObject* OB=Group[ID];
	if(!OB)return false;
	if(OB->Sdoxlo||!(OB->Ready||OB->LocalOrder))return false;
	if(NM->HighUnit&&OB->newMons->HighUnitCantEnter)return false;
	if(!CheckLeaveContainerAbility(OB))return false;
	AdvCharacter* ADC=OB->MoreCharacter;
	NewMonster* MiNM=OB->newMons;

	///if(!(newMons->Peasant||newMons->Usage==PeasantID||newMons->CanStorm))return false;

	if(!(NewMonst&&(MiNM->UnitAbsorber||
		  (MiNM->PeasantAbsorber&&NM->Peasant))))return false;
	if((!OB->newMons->PortionLimit)&&NMask&OB->NMask&&!CheckPossibilityToComeInside(OB,this))return false;
	if(!(MiNM->ConcPtX.GetAmount()&&MiNM->BornPtX.GetAmount()))return false;

	if(PrioryLevel>Prio)return false;
    if(LocalOrder&&LocalOrder->OrderType==242
        &&LocalOrder->info.BuildObj.ObjIndex==ID)return true;
	addrand(1003);
	Order1* Or1=CreateOrder(Type);
	if(!int(Or1))return false;
	Or1->PrioryLevel=Prio&127;
	Or1->OrderType=242;
	Or1->OrderTime=0;
	Or1->DoLink=&GoToMineLink;
	int xm,ym;
	OB->GetCornerXY(&xm,&ym);
	Or1->info.BuildObj.ObjIndex=ID;
	Or1->info.BuildObj.SN=OB->Serial;
	Or1->info.BuildObj.ObjX=0;
	//Or1->info.BuildObj.ObjY=ym+MiNM->ConcPtY[0];
	//PrioryLevel=Prio&127;
	UnBlockUnit();
	MakeOrderSound(this,2);
	PrioryLevel=Or1->PrioryLevel;
	RemoveFromFormation(this);
	if(Type==0||Type==1){
		CITY[NNUM].TheyMovesInto.Add(ID);
		CITY[NNUM].MoversIndices.Add(Index);		
		CITY[NNUM].TheyMovesInto_prev.Add(ID);
		CITY[NNUM].MoversIndices_prev.Add(Index);		
	}
	return true;
};
extern int LastAttackDelay;
extern int LastActionX;
extern int LastActionY;
extern int AlarmDelay;
extern short AlarmSoundID;
int GetUnitActivity(OneObject* OB);
word GetDir(int,int);
void LeaveMineLink(OneObject* OBJ);
void DestructBuilding(OneObject* OB);
void FindPositionForUnit(OneObject* OB);
#define ConX(n) ((COS*MiNM->ConcPtX[n]-SIN*MiNM->ConcPtY[n])/256)
#define ConY(n) ((SIN*MiNM->ConcPtX[n]+COS*MiNM->ConcPtY[n])/256)
#define PosX(n) ((COS*MiNM->PosPtX[n]-SIN*MiNM->PosPtY[n])/256)
#define PosY(n) ((SIN*MiNM->PosPtX[n]+COS*MiNM->PosPtY[n])/256)

void GoToMineLink(OneObject* OBJ){
	OBJ->GroupSpeed=OBJ->newMons->MotionDist;
	OBJ->UnitSpeed=64;
	OBJ->PrioryLevel=OBJ->LocalOrder->PrioryLevel;
    int n=OBJ->LocalOrder->info.BuildObj.ObjX;
	word OID=OBJ->LocalOrder->info.BuildObj.ObjIndex;
	word OSN=OBJ->LocalOrder->info.BuildObj.SN;
	OneObject* OB=Group[OID];
	if(!OB){
		if(OBJ->UnlimitedMotion)OBJ->UnlimitedMotion=false;
		OBJ->DeleteLastOrder();
		return;
	};
	//if((!(OBJ->UnlimitedMotion||OB->newMons->PortionLimit))&&OB->NMask&OBJ->NMask&&(!CheckPossibilityToComeInside(OB,OBJ))){
	//	OBJ->DeleteLastOrder();
	//	return;
	//};
	CITY[OBJ->NNUM].TheyMovesInto.Add(OID);
	CITY[OBJ->NNUM].MoversIndices.Add(OBJ->Index);
    int xx,yy,xm,ym;
    OB->GetPreciseCornerXY(&xm,&ym);
    NewMonster* NM=OBJ->newMons;
	NewMonster* MiNM=OB->newMons;
	int ncp=MiNM->ConcPtX.GetAmount();
	if(!(MiNM->ShowInsideUnits||(MiNM->Building==false && MiNM->MaxInside)))OB->RealDir=-64;
	int COS=TCos[byte(OB->RealDir+64)];
	int SIN=TSin[byte(OB->RealDir+64)];	
	int am=MiNM->ConcPtX.GetAmount()-1;
	if(am<0){
		OBJ->Die();
		return;
	}
    xx=xm+ConX(min(am,ncp-1));
    yy=ym+ConY(min(am,ncp-1));
	AdvCharacter* ADC=OB->MoreCharacter;
	if((!OB->Sdoxlo)&&(OB->Serial&0xFFFE)==(OSN&0xFFFE)){
		if(OBJ->UnlimitedMotion && !CheckLeaveContainerAbility(OB)){
			OBJ->UnlimitedMotion=false;
			OBJ->DeleteLastOrder();			
			FindPositionForUnit(OBJ);
			return;
		}
		int dst=Norma((OBJ->RealX>>4)-xx,(OBJ->RealY>>4)-yy);
		int d=64;
		if(MFIELDS[OBJ->LockType].CheckPt(xx>>4,yy>>4)){
			d=160;
		}
		if(OBJ->CObjIndex!=0xFFFF)d=64;
		if(dst<d){//priexali
			OBJ->PrioryLevel=127;
			if(OBJ->NMask&OB->NMask){
				//Hiding		
GGG1:
				word* Ins=znew(word,OB->NInside+1);
				if(OB->NInside){
			    	memcpy(Ins,OB->Inside,OB->NInside<<1);
			    	free(OB->Inside);
				};
				OB->Inside=Ins;
				Ins[OB->NInside]=OBJ->Index;				
				OB->NInside++;
				OB->WatchNInside=0;
				//if((!CheckPossibilityToComeInside(OB,OBJ))&&!OB->newMons->PortionLimit){
				//	OB->LeaveMine(OBJ->NIndex);
				//	if(OB->LocalOrder&&OB->LocalOrder->DoLink==&LeaveMineLink){
				//		OB->LocalOrder->info.BuildObj.ObjX=-1;
				//	};
				//};
				if(OB->newMons->ShowInsideUnits){					
					OBJ->ClearOrders();
					OBJ->UnlimitedMotion=true;
                    int am=MiNM->ConcPtX.GetAmount()-1;
					//OBJ->ClearOrderedUnlimitedMotion(1+128);
					//if(OBJ->CObjIndex==0xFFFF)OBJ->NewMonsterPreciseSendTo((xm+PosX(min(am,OB->NInside-1)))<<4,(ym+PosY(min(am,OB->NInside-1)))<<4,128+16,1+128);
					//OBJ->UnlimitedMotion=true;					
					OBJ->LockedInBuilding=OB->Index;
					OBJ->StandGround=true;
				}else{
					OBJ->HideMe();
					OBJ->ClearOrders();
				}
			}else{
				int ac1=GetUnitActivity(OBJ);
				int ac2=GetUnitActivity(OB);
				if(ac1==-1||ac2==1||OB->AI_Guard){
					if(OBJ->NNUM!=OB->NNUM)OBJ->Die();
					return;
				};
				if(ac1==1&&ac2==-1){
					if(OBJ->NNUM!=OB->NNUM)OB->Die();
					return;
				};
				if(!(OB->NInside||OB->AI_Guard)){
					// hack by vital
					//if(OB->AutoKill)DestructBuilding(OB);
					//void OnCapture(int Victim,byte Src,byte Dst,int Capturer);
					//OnCapture(OB->Index,OB->NNUM,OBJ->NNUM,OBJ->Index);
					//byte OldNat=OB->NNUM;
					//DelObject(OB);
					//bool FULLSTAGE=OB->Stage>=OB->MoreCharacter->ProduceStages;
					//OB->Nat->CITY->UnRegisterNewUnit(OB);
					//OB->Nat->CITY->Account-=EngSettings.LooseModulator*OB->newMons->Expa;
					//int oldst=OB->Stage;
					//int oldmax=OB->MoreCharacter->ProduceStages;
					//OB->Ref.General=NATIONS[OBJ->NNUM].Mon[OB->NIndex];
					//OB->Nat=&NATIONS[OBJ->NNUM];
					//OB->Nat->CITY->Account+=EngSettings.KillModulator*OB->newMons->Expa;
					//OB->NMask=OBJ->Nat->NMask;
					//OB->Serial^=1;
					//OB->Selected=0;
					//OB->ImSelected=0;
					//OB->Zombi=false;
					//if(OB->NewBuilding&&OldNat==MyNation){
					//	void TryAlarm(int x,int y,int reason);
					//	TryAlarm(OB->RealX>>4,OB->RealY>>4,1);
					//};
					//if(OB->LocalOrder)OB->Ready=1;
					//OB->ClearOrders();
					//OB->NNUM=OBJ->NNUM;
					//OB->Nat->CITY->RegisterNewUnit(OB);
					//AddObject(OB);
					//if(OB->NewBuilding)
					//OB->Stage=(oldst*int(OB->MoreCharacter->ProduceStages))/oldmax;
					goto GGG1;
				}else{
					//if(OBJ->NewAnm==OBJ->newMons->Attack){
						//if(OBJ->FrameFinished){
							if(OBJ->newMons->StormForce){
								if(OB->AI_Guard){
									OBJ->Die();
									OB->WatchNInside=0;
									return;
								}else{
									int N=OB->NInside;
									if(N){
										N=(int(rando())*N)>>15;
										word MID=OB->Inside[N];
										if(MID!=0xFFFF){
											OneObject* IOB=Group[MID];
											if(IOB){
												int sf=IOB->newMons->StormForce;
												int min=OB->newMons->MinOposit;
												int max=OB->newMons->MaxOposit;
												int ins=OB->NInside;
												int mins=OB->MoreCharacter->MaxInside;
												int VV=((sf*(min+((max-min)*ins)/mins))*int(rando()))>>15;
												if(VV<OBJ->newMons->StormForce)IOB->Die();
											};
										};
									};
								};
							};
							OBJ->Die();
							OBJ->Sdoxlo=500;
							OB->WatchNInside=1;
						//};
						return;
					//}else{
					//	OBJ->InMotion=0;
					//	OBJ->NewAnm=OBJ->newMons->Attack;
					//	OBJ->NewCurSprite;
					//	return;
					//};
				};
			};
		}else{
			for(int i=ncp-1;i>0;i--){
				int xx=(xm+ConX(min(am,i)))*16;
				int yy=(ym+ConY(min(am,i)))*16;
				OBJ->NewMonsterPreciseSendTo3D(xx,yy,MiNM->ConcPtZ[i-1],MiNM->ConcPtZ[i],128+16,1+128);				
			}
			OBJ->SetOrderedUnlimitedMotion(1+128);
			OBJ->NewMonsterSmartSendTo(xm+ConX(0)+(rando()&63)-32,ym+ConY(0)+(rando()&63)-32,0,0,128+16,1+128);
			return;
		}
	}else{
		OBJ->DeleteLastOrder();
		/*
		if(OBJ->UnlimitedMotion){
			OBJ->ClearOrders();
			OBJ->UnlimitedMotion=false;
			OBJ->SetOrderedUnlimitedMotion(2);
			xx=(xm<<4)+MiNM->ConcPtX[0];
            yy=(ym<<4)+MiNM->ConcPtY[0];
			OBJ->NewMonsterPreciseSendTo(xx<<4,yy<<4,16,2+128);
			OBJ->ClearOrderedUnlimitedMotion(2);
		}else{
			OBJ->DeleteLastOrder();
			OBJ->UnlimitedMotion=false;
		};
		*/
	};
};
void LeaveMineLink(OneObject* OB);
void OneObject::LeaveMine(word Type){
	if(!CheckLeaveContainerAbility(this))return;
	if(UnlimitedMotion)return;
	if(!NInside)return;
	if(newMons->Transport){
		LeaveTransport(Type);
		return;
	};
	addrand(1002);
    Order1* OR1=CreateOrder(1);
	if(OR1){
		OR1->info.BuildObj.ObjIndex=Type;
		OR1->info.BuildObj.ObjX=newMons->ExitPause;
		OR1->DoLink=&LeaveMineLink;
		OR1->PrioryLevel=0;
	}else{
		Log.Error("OneObject::LeaveMine: can not create order");
	}
};
void FindUnitPosition(int* x,int *y,int Type);
word CheckBuildingInCell(int cx,int cy,int dstx,int dsty);
#define ptX(n) ((COS*PTX[n]-SIN*PTY[n])/256)
#define ptY(n) ((SIN*PTX[n]+COS*PTY[n])/256)
void LeaveMineLink(OneObject* OBJ){
	OBJ->GroupSpeed=OBJ->newMons->MotionDist;
	OBJ->UnitSpeed=64;
	addrand(88);
	OBJ->PrioryLevel=OBJ->LocalOrder->PrioryLevel;
    if(OBJ->LocalOrder->info.BuildObj.ObjX>0){
		addrand(99);
		OBJ->LocalOrder->info.BuildObj.ObjX--;
		return;
	};
    if(!OBJ->NInside){
		addrand(77);
        OBJ->DeleteLastOrder();
        return;
    };
    OneObject* OB=NULL;
	word p;
    word Type=OBJ->LocalOrder->info.BuildObj.ObjIndex;
	for(int i=OBJ->NInside-1;i>=0&&!OB;i--){
		p=OBJ->Inside[i];
		OB=Group[p];
		if(OB&&(OB->NIndex==Type||Type==0xFFFF)){
			if(i+1<OBJ->NInside)memcpy(OBJ->Inside+i,OBJ->Inside+i+1,(OBJ->NInside-i-1)<<1);
			OBJ->NInside--;
		}else OB=NULL;
	};
	if(!OB){
		addrand(66);
		OBJ->DeleteLastOrder();
		return;
	};
	if(!OBJ->NInside){
		free(OBJ->Inside);
		OBJ->Inside=NULL;
	};
	int COS=TCos[byte(OBJ->RealDir+64)];
	int SIN=TSin[byte(OBJ->RealDir+64)];		
	NewMonster* MiNM=OBJ->newMons;
	int nc=MiNM->BornPtX.GetAmount();
	short* PTX=MiNM->BornPtX.GetValues();
	short* PTY=MiNM->BornPtY.GetValues();
	if(OB->newMons->Artilery&&OB->newMons->CraftPtX.GetAmount()){
		nc=MiNM->CraftPtX.GetAmount();
		PTX=MiNM->CraftPtX.GetValues();
		PTY=MiNM->CraftPtY.GetValues();
	};
	int xx,yy;
	OBJ->GetCornerXY(&xx,&yy);
	if(OB->LockedInBuilding==0xFFFF){
		OB->RealX=((xx<<4)+ptX(0))<<4;
		OB->RealY=((yy<<4)+ptY(0))<<4;
		addrand(OB->RealX);addrand(OB->RealY);
	}
	int LX=OB->Lx;
	OB->x=(OB->RealX-(LX<<7))>>8;
	OB->y=(OB->RealY-(LX<<7))>>8;
	OB->ShowMe();
	OB->LockedInBuilding=0xFFFF;
	OB->StandGround=false;
	OB->RealDir=byte(GetDir(ptX(1)-ptX(0),ptY(1)-ptY(0)));
	OB->GraphDir=OB->RealDir;
	//int nb=MiNM->NBornPt;
	OB->UnlimitedMotion=0;
	OB->SetOrderedUnlimitedMotion(0);
	for(i=nc-1;i>=0;i--){
		int x2=((xx<<4)+ptX(min(nc-1,nc-i-1)))*16;
		int y2=((yy<<4)+ptY(min(nc-1,nc-i-1)))*16;
		if(i==nc-1){
			int x1=x2+(rando()&1023)-512;
			int y1=y2+(rando()&1023)-512;
			if(!MFIELDS[OB->LockType].CheckPt(x1/256,y1/256)){
				OB->NewMonsterPreciseSendTo(x1,y1,16,2+128);
			}else OB->NewMonsterPreciseSendTo(x2,y2,16,2+128);
		}else OB->NewMonsterPreciseSendTo3D(x2,y2,MiNM->BornPtZ[nc-i-2],MiNM->BornPtZ[nc-i-1],16,2+128);
	};
	OB->ClearOrderedUnlimitedMotion(2);
#ifdef COSSACKS2
	if(OB->RAmount){
		OB->TakeResource(-1,OBJ->Index,OB->RType,128+16,2);
		OB->UnlimitedMotion=true;
		OBJ->DeleteLastOrder();
		addrand(33);
		return;
	};
#endif //COSSACKS2
	if(OBJ->DstX>0&&OBJ->LocalOrder->info.BuildObj.ObjX==0){
		int DSTX=OBJ->DstX>>4;
		int DSTY=OBJ->DstY>>4;
		//determining if it possible to go inside
		int cx=DSTX>>7;
		int cy=DSTY>>7;
		word INS=0xFFFF;
		int dyy=(DSTY>>1)-GetHeight(DSTX,DSTY);
		for(int r=0;r<4;r++){
			char* xi=Rarr[r].xi;
			char* yi=Rarr[r].yi;
			int N=Rarr[r].N;
			for(int j=0;j<N;j++){
				word mid=CheckBuildingInCell(cx+xi[j],cy+yi[j],DSTX,dyy);
				if(mid!=0xFFFF)INS=mid;
			};
		};
		if(INS!=0xFFFF){
			if(INS==OBJ->Index&&OBJ->MoreCharacter->MaxInside+OBJ->AddInside>OBJ->NInside){
				/*
				OB->HideMe();
				OB->ClearOrders();
				OBJ->Inside=(word*)realloc(OBJ->Inside,(OBJ->NInside+1)<<1);
				OBJ->Inside[OBJ->NInside]=OB->Index;
				OBJ->NInside++;
				OBJ->DeleteLastOrder();
				*/
			}else OB->GoToMine(INS,16+128,2);
		}else{
			byte rk=DetermineResource(DSTX,DSTY);
			if(rk<0xFE&&OB->newMons->Peasant){
				int NN=15;
				byte RK1=0xFE;
				int XXX,YYY;
				do{
					XXX=DSTX+((rando()&255)-128);
					YYY=DSTY+((rando()&255)-128);
					if(XXX>0&&YYY>0){
						RK1=DetermineResource(XXX,YYY);
					};
				}while (NN>0&&RK1!=rk);
				if(RK1==rk)OB->TakeResource(XXX,YYY,rk,16,2);
				else OB->TakeResource(DSTX,DSTY,rk,16,2);
			}else{
				int dx=OBJ->DstX;
				int dy=OBJ->DstY;
				FindUnitPosition(&dx,&dy,OB->NIndex);
				OB->NewMonsterSmartSendTo((dx/*OBJ->DstX+((rando()-16384)>>5)*/)>>4,(dy/*OBJ->DstY+((rando()-16384)>>5)*/)>>4,0,0,128,2+128);
			};
		};
	};
	OB->UnlimitedMotion=true;
	addrand(44);
	if(Type!=0xFFFF) OBJ->DeleteLastOrder();

};
void CmdUnSelUnitsSet(byte NI,word* BUF,int NU);
void DeleteFromSelection(OneObject* OB){
    if(!OB)return;
	for(int i=0;i<8;i++){
		if(OB->ImSelected&(1<<i)){
			int Nsel=ImNSL[i];
			word* SMon=ImSelm[i];
			word ID=OB->Index;
			for(int f=0;f<Nsel;f++)if(SMon[f]==ID)SMon[f]=0xFFFF;
			OB->ImSelected&=~GM(i);
			CmdUnSelUnitsSet(i,&ID,1);
		};
	};
};
void OneObject::HideMe(){
	Hidden=true;
	Sdoxlo=1;
    ClearOrders();
    DeleteFromSelection(this);
    DestX=-1;
	DestY=-1;
	NewAnm=newMons->GetAnimation(anm_Stand);
	NewCurSpritePrev=0;
	NewCurSprite=0;
	InMotion=false;
    DeletePath();
};
void CmdOpenGates(byte NI);
void CmdCloseGates(byte NI);
void OneObject::ShowMe(){
	Hidden=false;
	Sdoxlo=0;
};
void PushUnitOutOfMine(int i){
    if(GetKeyState(VK_SHIFT)&0x8000){
		CmdLeaveMine(MyNation,i);
		CmdLeaveMine(MyNation,i);
		CmdLeaveMine(MyNation,i);
		CmdLeaveMine(MyNation,i);
		CmdLeaveMine(MyNation,i);
	}else CmdLeaveMine(MyNation,i);
};
void CmdCreateGates(byte NI);
void DoGates(int i){
	CmdCreateGates(MyNation);
};
void OpGates(int i){
	CmdOpenGates(MyNation);
};
void ClGates(int i){
	CmdCloseGates(MyNation);
};
bool CheckOpenGate(OneObject* OB);
bool CheckCloseGate(OneObject* OB);
word LastOFCR=0xFFFF;
word OFCRStage=0;
void OfficerCallback(int i){
	OFCRStage=i;
};
extern City CITY[8];
void CrBrig(int i){
	CmdCrBig(MyNation,i);
};
bool SetDestMode=false;
void SDS_Pro(int i){
	SetDestMode=true;
};
void CmdStopUpgrade(byte NI);
void ZAGLUXA(int i){
};
void StopUpgrade(int i){
	CmdStopUpgrade(MyNation);
};
void CmdUnloadAll(byte Nat);
void UNLOAD(int i){
	CmdUnloadAll(MyNation);
};
void CmdSetGuardState(byte,word);
extern bool GUARDMODE;
void CHGUARD(int i){
	if(i){
		GUARDMODE=1;
	}else CmdSetGuardState(MyNation,0xFFFF);
};
int EnumUnitsInRound(int x,int y,int r,word Type,byte Nation);
extern int SET_DEST_ICON;
extern int SDS_X;
extern int SDS_Y;
extern char* SDS_Str;
extern int STOP_ICON;
extern int AblX;
extern int AblY;
extern int IconLx;
extern int IconLy;
extern int UNLOAD_ICON;
extern int IDLE_PICON;
extern int IDLE_MICON;
extern int IDLE_PX;
extern int IDLE_PY;
extern int IDLE_MX;
extern int IDLE_MY;
void PerformUpgradeLink(OneObject* OBJ);
void CBar(int x,int y,int Lx,int Ly,byte c);
void CmdSelectIdlePeasants(byte);
void CmdSelectIdleMines(byte);
void SELECT1(int i){
	if(i==0){
		CmdSelectIdlePeasants(MyNation);
	}else if(i==1){
		CmdSelectIdleMines(MyNation);
	};
};
void ChOrdN(int i){
	i-=2;
	if(OFCRStage){
		if(i<0){
			if(OFCRStage>256)OFCRStage-=256;
		}else{
			OFCRStage+=256;
		};
	};
	Lpressed=0;
};
extern byte PlayGameMode;
char* GetTextByID(char* ID);
int DecOrdID=43;
int IncOrdID=47;
extern int NOGUARD_ICON;
extern int DOGUARD_ICON;
bool AttGrMode=0;
void ATTGR_PRO(int p){
	AttGrMode=1;
};
extern bool Page3HaveItems;
extern bool Page1HaveItems;
extern bool HideBorderMode;
bool CreateInsideList(IconSet* IS,byte NI){
	if(PlayGameMode==1||HideBorderMode)return false;
	bool P1H=Page1HaveItems;
    word Nmons[1024];
    memset(Nmons,0,2048);
    int Nsel=ImNSL[MyNation];
    word* SMon=ImSelm[NI];
    if(!Nsel)return false;
    word MID;
	bool OneType=1;
	word TypeID=0xFFFF;
	if(Nsel){
		//checking for gates
		bool OpnGate=false;
		bool ClsGate=false;
		bool OnlyWall=true;
		bool OnlyDest=true;
		bool Usual=0;
		bool Guards=0;
		bool HaveArtpodg=0;
		bool HaveNoArtpodg=0;
		
		for(int i=0;i<Nsel;i++){
			MID=SMon[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB){
					OnlyWall=false;
					if(!OB->Ref.General->CanDest)OnlyDest=false;
					int ID=OB->NIndex;
					if(TypeID==0xFFFF){
						TypeID=ID;
					}else{
						if(TypeID!=ID)OneType=0;
					};
					if(!(OB->Sdoxlo||OB->LockType||OB->newMons->Capture||OB->NewBuilding)){
						if(OB->Guard==0xFFFF)Usual=1;
						else Guards=1;
					};
					if(!OB->Sdoxlo){
						if(OB->newMons->Artpodgotovka)HaveArtpodg=1;
						else HaveNoArtpodg=1;
					};
				};	
			};
		};

		if(OnlyDest){
			OneIcon* OI=IS->AddIconFixed(0,SET_DEST_ICON,SDS_X+SDS_Y*12);
			OI->CreateHint(SDS_Str);
			OI->AssignLeft(&SDS_Pro,0);
		};
		MID=SMon[0];	
		if(MID==0xFFFF)return false;
		OneObject* OB=Group[MID];
		if(!OB)return false;
		GeneralObject* GO=OB->Ref.General;
		if(OneType&&!(OB->newMons->Capture||OB->newMons->CantCapture||OB->NewBuilding||OB->Transport||OB->Wall||OB->newMons->Usage==PeasantID)){
			if(Guards&&!Usual){
				OneIcon* OI=IS->AddIconFixed(0,NOGUARD_ICON,13);
				OI->CreateHint(HNOGUARD);
				OI->AssignLeft(&CHGUARD,0);
			};
			if(!Guards){
				OneIcon* OI=IS->AddIconFixed(0,DOGUARD_ICON,13);
				OI->CreateHint(HDOGUARD);
				OI->AssignLeft(&CHGUARD,1);
			};
		};
		if(HaveArtpodg&&!HaveNoArtpodg&&!SCENINF.hLib){
			OneIcon* OI=IS->AddIconFixed(0,ATTGR_ICON,AttGrPos);
			OI->CreateHint(ATGHINT);
			OI->AssignLeft(&ATTGR_PRO,0);
		};
		if(OB->newMons->ShowIdlePeasants&&OB->Ready&&OneType){
			OneIcon* OI=IS->AddIconFixed(0,IDLE_PICON,OB->newMons->IdlePPos);
			OI->CreateHint(IDLE_P);
			OI->AssignLeft(&SELECT1,0);
		};
		if(OB->newMons->ShowIdleMines&&OB->Ready&&OneType){
			OneIcon* OI=IS->AddIconFixed(0,IDLE_MICON,OB->newMons->IdleMPos);
			OI->CreateHint(IDLE_M);
			OI->AssignLeft(&SELECT1,1);
		};
		if(Nsel==1||(GO->OFCR&&OneType)){
			byte Usage=OB->newMons->Usage;
			if((OB->NewBuilding||OB->LockType)&&OB->NUstages&&(!OB->Ready)&&OB->LocalOrder&&OB->LocalOrder->DoLink==&PerformUpgradeLink){
				NewUpgrade* NU=OB->Nat->UPGRADE[OB->LocalOrder->info.PUpgrade.NewUpgrade];
				OneIcon* OI=IS->AddIconFixed(0,NU->IconSpriteID,0);
				OI->MoreSprite=STOP_ICON;
				OI->CreateHint(STOPUPG);
				OI->AssignLeft(&StopUpgrade,OB->Index);
				OI->AssignColor(0xD0+MyNation*4);
				OI=IS->AddIconFixed(0,NU->IconSpriteID,4);
				OI->CreateHint(NU->Message);
				OI->AssignLeft(&ZAGLUXA,0);
				OI->AssignLevel(NU->Level);
				OI->AssignColor(0xD0+MyNation*4);
				int x0=AblX+IconLx+10;
				int y0=AblY+10;
				int LX=113;
				int LY=IconLy-20;
				int L=((LX)*OB->Ustage)/OB->NUstages;
				TempWindow TW;
				PushWindow(&TW);
				GPS.ShowGP(AblX+44,AblY-4,BordGP,83,0);
				IntersectWindows(AblX,AblY,AblX+44+4+L,AblY+40);
				GPS.ShowGP(AblX+44,AblY-4,BordGP,84,0);
				PopWindow(&TW);
				char cc[20];
				sprintf(cc,"%d%%",(OB->Ustage*100)/OB->NUstages);
				ShowString(AblX+60+44-GetRLCStrWidth(cc,&SmallYellowFont)/2,y0+4,cc,&SmallYellowFont);
			};
			//for officer
			
			if(GO->OFCR){
				if(OB->Index!=LastOFCR){
					LastOFCR=OB->Index;
					OFCRStage=0;
				};
				OfficerRecord* OFCR=GO->OFCR;
				int NBAR=EnumUnitsInRound(OB->RealX,OB->RealY,MobilR*16,OFCR->BarabanID,GSets.CGame.cgi_NatRefTBL[MyNation]);
				for(int i=0;i<OFCR->NStroi;i++){
					int id=OFCR->SDES[i].ID;
					if(OFCRStage&&!i){
						OneIcon* OID=IS->AddIconFixed(0,DecOrdID,OrderDesc[id].IconPos-2);
						OID->AssignLeft(&ChOrdN,3);
						OID->CreateHint(GetTextByID("DECFORM"));
						OneIcon* OII=IS->AddIconFixed(0,IncOrdID,OrderDesc[id].IconPos-1);
						OII->AssignLeft(&ChOrdN,1);
						OII->CreateHint(GetTextByID("INCFORM"));
						//if(OFCRStage<255)OII->Disable();
					};
					OneIcon* OI=IS->AddIconFixed(0,OrderDesc[id].IconID,OrderDesc[id].IconPos);
					if(NBAR||OFCR->BarabanID==0xFFFF){
						OI->AssignLeft(&OfficerCallback,i+1);
						OI->CreateHint(OrderDesc[id].Message);
					}else{
						OI->Disable();
						OI->CreateHint(OrderDesc[id].Message);
						OFCRStage=0;
					};
				};
				if(OFCRStage){
					StroiDescription* SDS=GO->OFCR->SDES+((OFCRStage-1)&255);
					int DD=OFCRStage>>8;
					bool AtLeastOne=0;
					for(int j=0;j<SDS->NUnits;j++){
						int NNear=EnumUnitsInRound(OB->RealX,OB->RealY,MobilR*16,SDS->Units[j],GSets.CGame.cgi_NatRefTBL[MyNation]);
						for(int p=0;p<SDS->NAmount&&SDS->Amount[p]<=NNear;p++);
						OneIcon* OI=IS->AddIconFixed(0,NATIONS[MyNation].Mon[SDS->Units[j]]->newMons->IconID,OrderDesc[SDS->ID].IconPos+j+1);
						p-=DD;
						if(p>0){
							OI->AssignIntVal(SDS->Amount[p-1]);
							OI->AssignLeft(&CrBrig,int(DWORD(OB->Index&8191)+DWORD(SDS->LocalID[p-1])*8192+DWORD(SDS->Units[j])*8192*256));
							OI->CreateHint(NATIONS[MyNation].Mon[SDS->Units[j]]->Message);
							AtLeastOne=1;
						}else OI->Disable();
					};
					if(DD&&!AtLeastOne)DD--;
					OFCRStage=(OFCRStage&255)+(DD<<8);
				};
			}else LastOFCR=0xFFFF;
		}else LastOFCR=0xFFFF;
		if(GO->newMons->Rinok&&OB->Ready){
			EconomyInterface(IS);
		};		
	};
	int Per=false;
	int OwnerNMID=-1;
	bool OneOwner=1;
    for(int i=0;i<Nsel;i++){
        MID=SMon[i];
        if(MID!=0xFFFF){
            OneObject* OB=Group[MID];
            if(OB&&OB->NInside&&OB->Ready){
				if(OB->Transport)Per=true;
				NewMonster* NM=OB->newMons;
				int ID=OB->NIndex;
				if(OwnerNMID==-1){
					OwnerNMID=ID;
				}else if(OwnerNMID!=ID)OneOwner=0;
				int Nins=OB->NInside;
				word* INS=OB->Inside;
				for(int j=0;j<Nins;j++){
					word InMID=INS[j];
					if(InMID!=0xFFFF){
						OneObject* INO=Group[InMID];
						if(INO){
							Nmons[INO->NIndex]++;
							//assert(INO->NIndex<1024);
						};
					};
                };
				Order1* OR1=OB->LocalOrder;
				while(OR1){
					if(OR1->DoLink==&LeaveMineLink||OR1->DoLink==&LeaveTransportLink){
						word IID=OR1->info.BuildObj.ObjIndex;
						//assert(IID<1024);
						if(Nmons[IID])Nmons[IID]--;
					};
					OR1=OR1->NextOrder;
				};
            };
        };
    };
    Nation* NT=&NATIONS[NI];
	bool Pres=0;
	NewMonster* OWNM=NULL;
	if(OneOwner&&OwnerNMID!=-1)OWNM=NT->Mon[OwnerNMID]->newMons;
	if(OWNM&&OWNM->Page3Icon!=0xFFFF){
		int cidx=-1;
		IS->Page=0;
		//if(OWNM->CurrPage==2){
			for(i=0;i<1024;i++){
				if(Nmons[i]){
					Page3HaveItems=1;
					if(OWNM->CurrPage==2){
						cidx++;
						GeneralObject* GO=NT->Mon[i];
						NewMonster* NM=GO->newMons;
						OneIcon* OI=IS->AddIconFixed(NM->IconFileID,NM->IconID,1+(cidx%11)+(cidx/12)*12);
						OI->CreateHint(NM->Message);
						OI->AssignIntVal(Nmons[i]);
						OI->AssignLeft(&PushUnitOutOfMine,i);
						Pres=1;
					};
				};
			};
		//};
	}else{
		for(i=0;i<1024;i++){
			if(Nmons[i]){
				GeneralObject* GO=NT->Mon[i];
				NewMonster* NM=GO->newMons;
				OneIcon* OI=IS->AddIcon(NM->IconFileID,NM->IconID,0,NM->Message);
				OI->AssignIntVal(Nmons[i]);
				OI->AssignLeft(&PushUnitOutOfMine,i);
				Pres=1;
			};
		};
		if(Pres&&Per){
			OneIcon* OI=IS->AddIconFixed(0,UNLOAD_ICON,12*2);
			OI->CreateHint(UNLOADALL);
			OI->AssignLeft(&UNLOAD,0);
		};
	};
	Page1HaveItems=P1H;
    return true;
};
void TakeResourceFromSpriteLink(OneObject* OBJ);
void OneObject::TakeResourceFromSprite(int SID){
	if(SID==INITBEST||SID==-1)return;
	if(UnlimitedMotion)return;
	if(CheckOrderAbility())return;
    NewMonster* NM=newMons;
    OneSprite* OS=&Sprites[SID];
    if(!OS->Enabled)return;
    ObjCharacter* OC=OS->OC;
    if(OC->IntResType>8)return;
    byte msk=1<<OC->IntResType;
    if(msk&NM->ProdType){
        RType=OC->IntResType;
        RAmount=0;
		addrand(1001);
        Order1* OR1=CreateOrder(0);
        OR1->DoLink=&TakeResourceFromSpriteLink;
        PrioryLevel=0;
        OR1->info.BuildObj.ObjIndex=SID;
		OR1->info.BuildObj.SN=0;
    };
};

int NInGold[8];
int NInIron[8];
int NInCoal[8];
int NInStone[8];
int WasInGold[8];
int WasInIron[8];
int WasInCoal[8];
int WasInStone[8];
void TakeResourceFromSpriteLink(OneObject* OBJ){
    int SID=OBJ->LocalOrder->info.BuildObj.ObjIndex;
	if(SID>=MaxSprt){
		OBJ->DeleteLastOrder();
		return;      
	}
    OneSprite* OS=&Sprites[SID];
    if (!(OS && OS->Enabled)) {
        OBJ->DeleteLastOrder();
        return;
    };
    ObjCharacter* OC=OS->OC;
    if(OBJ->RType!=OC->IntResType){
        OBJ->DeleteLastOrder();
        return;
    };
	int NT=OBJ->NNUM;
	int IRES=OBJ->LocalOrder->info.BuildObj.SN;
	NewMonster* NM=OBJ->newMons;
	int NINS=OBJ->NInside;
	int pp=100;
	bool GetMineProduceBonus(OneObject* Mine,int& Proc);
	GetMineProduceBonus(OBJ,pp);
	int NINS1=(pp*NINS)/100;
	int NMAX=OBJ->MoreCharacter->MaxInside+OBJ->AddInside;
	if(NINS>NMAX)NINS=NMAX;
	int work=(NM->FreeAdd+NINS*NM->PeasantAdd)*GameSpeed/8;
	work=(work*pp)/100;
	byte RTP=OBJ->RType;
	if(RTP==GoldID)NInGold[NT]+=NINS1;
	else if(RTP==IronID)NInIron[NT]+=NINS1;
	else if(RTP==CoalID)NInCoal[NT]+=NINS1;
	else if(RTP==StoneID)NInStone[NT]+=NINS1;
	IRES+=work;
#ifndef COSSACKS2
    OBJ->RAmount+=work;
#endif
	if(IRES>1000){
		div_t qq=div(IRES,1000);
#ifdef COSSACKS2
		if(NM->PortionLimit){
			int R=int(OBJ->RAmount)+qq.quot;
			if(R>255)R=255;
			OBJ->RAmount=R;
			if(OBJ->RAmount>=NM->PortionLimit){
				if(OBJ->NInside){
					for(int q=OBJ->NInside-1;q>=0;q--){
						OneObject* IOB=Group[OBJ->Inside[q]];
						if(IOB&&IOB->newMons->Peasant){
							IOB->RAmount=NM->PortionLimit;
							IOB->RType=OBJ->RType;
							OBJ->LeaveMine(IOB->NIndex);
							IOB->UnlimitedMotion=0;
							IOB->TakeResource(-1,OBJ->Index,OBJ->RType,128+16,2);
							IOB->UnlimitedMotion=1;
							OBJ->RAmount-=NM->PortionLimit;
							break;
						};
					};
				};
			};
		}else
#endif //COSSACKS2
		{			
			int work=OS->PerformIntWork(qq.quot);
			AddXRESRC(OBJ->NNUM,OBJ->RType,qq.quot);
			Nation* NT=NATIONS+OBJ->NNUM;
			NT->AddResource(OBJ->RType,qq.quot);
			NT->ResTotal[OBJ->RType]+=qq.quot;
		};
		IRES=qq.rem;
	}
	OBJ->LocalOrder->info.BuildObj.SN=IRES;
};
extern char* ResNames[8];;
char* GetSprResourceName(OneObject* OB){
	Order1* ORD=OB->LocalOrder;
	if(ORD&&ORD->DoLink==&TakeResourceFromSpriteLink){
		int rt=OB->RType;
		if(rt>=8)return NULL;
		return RDS[rt].Name;
	};
	return NULL;
};
void ShowRLCItemTrans8(int x,int y,lpRLCTable lprt,int n);
#define zoom(x) (LMode?(x>>2):x)
void ShowBuilding(NewMonster* NM,int x,int y,byte kind){
	int x0=x+zoom(NM->PicDx);
	int y0=y+zoom(NM->PicDy);
	NewAnimation* UP=NM->GetAnimation(anm_StandHi);
	NewAnimation* DN=NM->GetAnimation(anm_StandLo);
	int NNN=5+int(8*(sin(double(GetTickCount())/100)+2));
	if(kind){
		if(UP->Enabled){
			for(int i=0;i<UP->Parts;i++){
				NewFrame* NF=UP->Frames[i];
				int FID=NF->FileID;
				if(LMode){
					if(!FID)return;
				};
				GPS.ShowGP(x0+i*UP->PartSize,y0,FID,NF->SpriteID,0);
			};
		};
		if(DN->Enabled){
			for(int i=0;i<DN->NFrames;i++){
				NewFrame* NF=DN->Frames[i];
				int FID=NF->FileID;
				if(LMode){
					if(!FID)return;
				};
				GPS.ShowGP(x0,y0,FID,NF->SpriteID,0);
			};
		};
	}else{
		if(UP->Enabled){
			for(int i=0;i<UP->Parts;i++){
				NewFrame* NF=UP->Frames[i];
				int FID=NF->FileID;
				if(LMode){
					if(!FID)return;
				};
				GPS.ShowGP(x0+i*UP->PartSize,y0,FID,NF->SpriteID,0);
			};
		};
		if(DN->Enabled){
			for(int i=0;i<DN->NFrames;i++){
				NewFrame* NF=DN->Frames[i];
				int FID=NF->FileID;
				if(LMode){
					if(!FID)return;
				};
				GPS.ShowGP(x0,y0,FID,NF->SpriteID,0);
			};
		};
	};
};
bool GetRealCoords(int xs,int ys,int* xr,int* yr);
typedef void SprInf(OneSprite* OS);
OneObject* DetermineMineBySprite(int Spr);
void EnumerateScreenSprites(SprInf* SPI){
	//byte Mask=NM->ProdType;
	int cx0=(mapx>>(7-5))-10;
	int cx1=((mapx+smaplx)>>(7-5))+10;
	if(cx0<0)cx0=0;
	if(cx1>=VAL_SPRNX)cx1=VAL_SPRNX-1;
	int xs;
	int ys=mapy<<5;
	int ys1=(mapy+smaply)<<5;
	int cy0=(mapy>>2)-10;
	int cy1=((mapy+smaply)>>2)+10;
	if(cy0<0)cy0=0;
	if(cy1>VAL_SPRNX-1)cy1=VAL_SPRNX-1;

	for(int cx=cx0;cx<=cx1;cx++){
		xs=(cx<<7)+(1<<(7-1));
		//if(GetRealCoords(xs,ys,&xr,&yr))cy0=(yr>>7)-1;else cy0=0;
		//if(GetRealCoords(xs,ys1,&xr,&yr))cy1=(yr>>7)+1;else cy1=63;
		//if(cy0<0)cy0=0;
		//if(cy1>VAL_SPRNX-1)cy1=VAL_SPRNX-1;
		for(int cy=cy0;cy<=cy1;cy++){
			int cell=cx+(cy<<SprShf);
			int NSP=NSpri[cell];
			if(NSP){
				int* SPRF=SpRefs[cell];
				for(int i=0;i<NSP;i++){
					word ID=SPRF[i];
					OneSprite* OS=&Sprites[ID];
					if(OS->Enabled&&!DetermineMineBySprite(ID)){
						SPI(OS);
					};
				};
			};
		};
	};
};
int mul3(int);
extern bool Mode3D;
//NewMonster* NEWMON;
int bMineID;
byte MineMask;
int GetUnitModifiedBySprite(int _MineID,OneSprite* OS){
	GeneralObject* GO=NATIONS->Mon[_MineID];
	NewMonster* NM=GO->newMons;				
	ObjCharacter* OC=OS->OC;
	if(OC->IntResType!=0xFF){
		byte MS=1<<OC->IntResType;		
		if(MS & NM->ProdType)return _MineID;
		if(OS->OC->IntResType<8){
			byte rt=OS->OC->IntResType;					
			if(rt==IronID){
				_MineID++;
			}else if(rt==CoalID){
				_MineID+=2;
			}else if(rt==StoneID){
				_MineID+=3;
			}else if(rt==FoodID){
				_MineID+=4;
			}else if(rt==TreeID){
				_MineID+=5;
			};
			GO=NATIONS->Mon[_MineID];
			NM=GO->newMons;				
			if(NM->ProdType & MS)return _MineID;
		}
	}
	return -1;
}
void MSpinf(OneSprite* OS){
	ObjCharacter* OC=OS->OC;
	if(OC->IntResType!=0xFF){
		int id=GetUnitModifiedBySprite(bMineID,OS);
		if(id!=-1){
			GeneralObject* GO=NATIONS->Mon[id];
			NewMonster* NM=GO->newMons;				
			if(NM->ProdType & (1<<OS->OC->IntResType)){
				int x=OS->x;
				int y=OS->y;
				int z=GetTotalHeight(x,y);
				IRS->SetTextureFactor(0x40808080);
				NewAnimation* DN=NM->GetAnimation(anm_StandLo);
				DN->DrawAt(0,MyNation,x,y,z,0,1.0,0x60FFFFFF,0,0,NULL);
				IRS->SetTextureFactor(0xFFFFFFFF);
			}
		}
	}
}
void ShowMines(int id){
	NewMonster* NM=NATIONS->Mon[id]->newMons;
	if(!NM->ProdType)return;	
	MineMask=NM->ProdType;
	bMineID=id;
	EnumerateScreenSprites(&MSpinf);
};
extern int tmtmt;
int CheckMinePosition(int id,int* xi,int* yi,int r,bool Manual=1);
void HandleMines(){
	if(!(tmtmt&255)){
		for(int i=0;i<MAXOBJECT;i++){
			OneObject* OB=Group[i];
			if(OB&&OB->newMons->ProdType){
				if(!OB->LocalOrder){
					int xx=OB->RealX;
					int yy=OB->RealY;
					int ID=CheckMinePosition(OB->NIndex,&xx,&yy,64);
					if(ID!=-1)OB->TakeResourceFromSprite(ID);
				};
			};
		};
	};
};
