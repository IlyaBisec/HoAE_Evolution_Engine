#include "stdheader.h"
#include "CurrentMapOptions.h"
#include "unitability.h"
#include "IShadowManager.h"
#include "kContext.h"
#include "MassivePlayer.h"

DLLEXPORT int GetRND(int Max);

bool CheapMode;
void PerformAction(OneObject* OB,int x,int y);
extern const int drr[9]={7,6,5,0,0,4,1,2,3};
extern word LastObject;
void AssignHint1(char* s,int time);
//return 0-unable
//       n-busy(n=number of tasks)
//		 -1-ready
bool ApplyCost(byte NI,word NIndex,int x,int y);
void ProduceObjLink(OneObject* OBJ);
int OneObject::CheckAbility(word ID){
	if(!NewBuilding)return 0;
	Nation* NT=Nat;
	word NInd=NIndex;
	int p=NT->PACount[NInd];
	int i;
	for(i=0;i<p;i++){
		if(NT->PAble[NInd][i]==ID)break;
	};
	if(i>=p)return 0;
	int j=0;
	Order1* Ord1=LocalOrder;
	while(int(Ord1)){
		if(Ord1->DoLink==ProduceObjLink&&Ord1->info.Produce.ObjIndex==ID){
			if(Ord1->info.Produce.ID==0xFFFE)j+=1200;
			else j++;
		};
		Ord1=Ord1->NextOrder;
	};
	if(j==0)return -1;
	return j;	
};
extern MotionField UnitsField;
void MoveAwayShipInCell(int cell,int x,int y){
    cell+=VAL_MAXCX+1;
    int NMon=MCount[cell];
	if(!NMon)return;
	int ofs1=cell<<SHFCELL;
	word MID;
	for(int i=0;i<NMon;i++){
		MID=GetNMSL(ofs1+i);
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->LockType&&(!OB->Sdoxlo)&&!(OB->RealVx||OB->RealVy)){
				int NAtt=40;
				do{
					NAtt--;
					int dx=(rando()&1023)-512;
					int dy=(rando()&1023)-512;
					int N=Norma(dx,dy);
					if(N>300&&N<700){
						int xx=(x>>8)+(dx>>4)-8;
						int yy=(y>>8)+(dy>>4)-8;
						if(!(MFIELDS[1].CheckBar(xx,yy,16,16)||
							UnitsField.CheckBar(xx,yy,16,16))){
							if(!OB->LocalOrder)OB->NewMonsterSmartSendTo((OB->RealX>>4)+dx,(OB->RealY>>4)+dy,0,0,128,0);
							NAtt=-4;
						};
					};
				}while(NAtt&&NAtt!=-4);
				if(!NAtt){
					NAtt=40;
					do{
						NAtt--;
						int dx=(rando()&1023)-512;
						int dy=(rando()&1023)-512;
						int N=Norma(dx,dy);
						if(N>300&&N<700){
							int xx=(x>>8)+(dx>>4)-8;
							int yy=(y>>8)+(dy>>4)-8;
							if(!MFIELDS[1].CheckBar(xx,yy,16,16)){
								if(!OB->LocalOrder)OB->NewMonsterSmartSendTo((OB->RealX>>4)+dx,(OB->RealY>>4)+dy,0,0,128,0);
								NAtt=0;
							};
						};
					}while(NAtt);
				};
			};
		};
	};
};
void MoveAwayShip(int x,int y){
	int cell=((y>>11)<<VAL_SHFCX)+(x>>11);
	int CELL0=cell;
	int rx1=2;
	bool Capture=false;
    int rx2=rx1+rx1+1;
    int stcell=cell-rx1-(rx1<<VAL_SHFCX);
    for(int nx=0;nx<rx2;nx++){
        for(int ny=0;ny<rx2;ny++){
            if(stcell>=0&&stcell<VAL_MAXCIOFS){
				MoveAwayShipInCell(stcell,x,y);
            };
            stcell++;
        };
        stcell+=VAL_MAXCX-rx2;
    };
};
void ProduceObjLink(OneObject* OBJ);
extern City CITY[8];
void CheckArmies(City*);
bool OneObject::Produce(word ID){
	return Produce(ID,0xFFFF);
};
//extern int RESRC[8][8];
void GetUnitCost(byte NI,word NIndex,int* Cost,word Power);
void FindFirstPeasantInside(OneObject* OB){
	if(OB->NInside){
		int N=OB->NInside;
		for(int i=0;i<N;i++){
			word MID=OB->Inside[i];
			if(MID!=0xFFFF){
				OneObject* POB=Group[MID];
				if(POB&&POB->newMons->Peasant){
					if(!i)return;
					word M0=OB->Inside[0];
					OB->Inside[0]=MID;
					OB->Inside[i]=M0;
				};
			};
		};
	};
};
bool OneObject::Produce(word ID,word GroupID){
	addrand(ID);
	bool PREVIEW=0;
	if(CheckOrderAbility())return false;
	if(UnlimitedMotion)return false;
	NewMonster* NM=newMons;
	if(!(NM->Building&&Ready))return false;
	int FAST=ID>>13;
	ID&=8191;
    //checking for infinity
	Order1* OR2=LocalOrder;
	while(OR2){
		if(OR2->DoLink==&ProduceObjLink&&OR2->info.Produce.ObjIndex==ID
			&&OR2->info.Produce.ID==0xFFFE)return false;
		OR2=OR2->NextOrder;
	};
	if(GroupID==0xFFFF||GroupID==0xFFFE||GroupID==Nat->CITY->DestUnitsGroup){
		GeneralObject* GO=Nat->Mon[ID];
		if(GO->StageMask){
			word m=GO->StageMask;
			word s=StageState;
			for(int i=0;i<5;i++){
				if(m&1){
					byte s1=s&7;
					if(s1!=2)return false;
				};
				m>>=1;
				s>>=3;
			};
		};
	};
	if(GroupID==0xFFFE){
		OR2=LocalOrder;
		Order1* ORP=NULL;
		while(OR2){
			if(OR2->DoLink==&ProduceObjLink&&OR2->info.Produce.ObjIndex==ID
				&&OR2->info.Produce.ID!=0xFFFE){
				if(ORP){
					ORP->NextOrder=OR2->NextOrder;
				}else{
					LocalOrder=OR2->NextOrder;
				};
				Order1* OR3=OR2->NextOrder;
				if(OR2->info.Produce.Power!=0xFFFF){
					if(newMons->Port){
						if(OR2->info.Produce.Progress){
							MFIELDS[1].BClrBar(WallX-7,WallY-7,15);
						};
					};
					GeneralObject* GO=Nat->Mon[ID];
					int Cost[8];
					GetUnitCost(NNUM,ID,Cost,OR2->info.Produce.Power);
					for(int p=0;p<8;p++){
						addrand(XRESRC(NNUM,p));
						AddXRESRC(NNUM,p,Cost[p]);
						addrand(XRESRC(NNUM,p));
					};
				};
				FreeOrdBlock(OR2);
				OR2=OR3;
			}else{
				ORP=OR2;
				OR2=OR2->NextOrder;
			};
		};
	};
	GeneralObject* GG=Nat->Mon[ID];
	Nation* NT=Nat;
	int Power=CITY[NNUM].UnitAmount[ID];
	addrand(Power);
	for(int i=0;i<6;i++)addrand(XRESRC(NNUM,i));
	if(GroupID>=0xFFFE||GroupID==Nat->CITY->DestUnitsGroup){
		addrand(XRESRC(NNUM,FoodID));
		addrand(EngSettings.AutoProduceFoodLimit);
		addrand(GroupID);
		if((XRESRC(NNUM,FoodID)<EngSettings.AutoProduceFoodLimit&&GroupID==0xFFFE)||!ApplyCost(NNUM,ID,-1,-1)){
            addrand(ID);
			addrand(NNUM);
			//if(NNUM==MyNation)AssignHint1("Недостаточно ресурсов",50);
			if(NNUM==MyNation){
				void fes_neResourceForProduce(char* UnitMess);
				fes_neResourceForProduce(Nat->Mon[ID]->newMons->Message);
			}
			addrand(GroupID);
			if(GroupID==0xFFFE){
				PREVIEW=1;
			}else return false;
		};
	};
	if(newMons->ArtSet){
		int n=NT->NArtUnits[newMons->ArtSet-1];
		int m=NT->NArtdep[newMons->ArtSet-1]*newMons->NInArtDepot;
		if(n>=m){
			return false;
		}
	}
	addrand(1026);
    Order1* Or1=CreateOrder(2);
	Or1->PrioryLevel=96;
	Or1->OrderType=13;//Produce
	Or1->OrderTime=0;
	Or1->DoLink=&ProduceObjLink;
	Or1->info.Produce.ObjIndex=ID;
	Or1->info.Produce.PStep=1;
	Or1->info.Produce.Progress=0;
	Or1->info.Produce.NStages=Nat->Mon[ID]->MoreCharacter->ProduceStages;
#ifdef COSSACKS2
	if(FAST&&Or1->info.Produce.NStages>2*FAST*FAST)Or1->info.Produce.Progress=Or1->info.Produce.NStages-2*FAST*FAST;
#else //COSSACKS2
	if(FAST&&Or1->info.Produce.NStages>30*FAST)Or1->info.Produce.Progress=Or1->info.Produce.NStages-30*FAST;
#endif //COSSACKS2

	Or1	->info.Produce.ID=GroupID;
	if(PREVIEW)Or1->info.Produce.Power=0xFFFF;
	else Or1->info.Produce.Power=Power;
	addrand(Or1->info.Produce.Power);
	Or1->info.Produce.NStages<<=8;
	//OrderReport=NULL;
	//MessageKind=0;
	//Sender=0xFFFF;
	return true;
};	
bool AddSprRefl(int xg,int yg,int xc,int yc,int GP_File,int Sprite,int dy1,int dy2);
void ShowProducedShip(OneObject* Port,int CX,int CY){
	if(Port){
		int dxx=mapx<<(Shifter);
		int dyy=mapy<<(Shifter-1);
		Order1* OR1=Port->LocalOrder;
		if(OR1&&OR1->DoLink==&ProduceObjLink){
			NewMonster* NM=Port->newMons;
			if(!NM->Port)return;
			GeneralObject* GO=NATIONS[Port->NNUM].Mon[OR1->info.Produce.ObjIndex];
			NewMonster* SNM=GO->newMons;
			int prog=OR1->info.Produce.Progress;
			if(!prog)return;
			int stage=div(prog<<2,OR1->info.Produce.NStages).quot;
			if(stage>3)stage=3;
			NewAnimation* NA=SNM->GetAnimation(anm_BuildLo+stage);
			if(NA->Enabled){
				///NA->DrawAt(0,Port->NNUM,CX<<4,CY<<4,0,0,1.0,0xFF808080,0,0,0);				
				AddAnimation(CX<<4,CY<<4,0,NA,0,0,0xFF808080,NULL,1,0,0);
			};
		};
	};
};
extern MotionField UnitsField;
bool SubCost=false;
byte DetermineResource(int x,int y);
void GotoFinePosition(OneObject* OB);
int GetHeight(int,int);
void FindUnitPosition(int* x,int *y,int Type);
extern int DiffP[4];
void UnProduce(OneObject* OB,word Type);
extern word** BLDList;
void GetRect(OneObject* ZZ,int* x,int* y,int* Lx,int* Ly);
bool PInside(int x,int y,int x1,int y1,int xp,int yp);
word GetOneBld(int cell,int pos=0);
word CheckBuildingInCell(int cx,int cy,int dstx,int dsty){
	if(cx>=0&&cy>=0&&cx<VAL_MAXCX&&cy<VAL_MAXCX){
		word MID;
		int pos=0;
		do{
			MID=GetOneBld(cx+(cy<<VAL_SHFCX),pos);
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->NewBuilding&&OB->newMons->UnitAbsorber){
					int xs,ys,xs1,ys1,LY;
					GetRect(OB,&xs,&ys,&xs1,&LY);
					ys1=ys+LY-1;
					xs1+=xs-1;
					if(PInside(xs,ys,xs1,ys1,dstx,dsty))return MID;
				};
			};
			pos++;
		}while(MID!=0xFFFF);
	};
	return 0xFFFF;
};
word GetDir(int,int);
void ProduceObjLink(OneObject* OBJ){
	addrand(OBJ->LocalOrder->info.Produce.Power);
	addrand(OBJ->Index);
	if(OBJ->ActiveAbility){
		bool ProduceState=true;
		OBJ->ActiveAbility->ActiveAbilities.modifyProduceEnable(ProduceState,ProduceState);
		if(!ProduceState) return;
	}
	if(OBJ->StopBuildMode)return;
	OBJ->PrioryLevel=OBJ->LocalOrder->PrioryLevel;
	NewMonster* NM=OBJ->newMons;
	Nation* NT=OBJ->Nat;
	word OI=OBJ->LocalOrder->info.Produce.ObjIndex;
	word GID=OBJ->LocalOrder->info.Produce.ID;
	word AI_GID=0xFFFF;
	if(GID<0xFFFE&&GID==OBJ->Nat->CITY->DestUnitsGroup){
		AI_GID=GID;
		GID=0xFFFF;
	};
	GeneralObject* PGO=NT->Mon[OI];
	AdvCharacter* PADC=PGO->MoreCharacter;
	NewMonster* ONM=PGO->newMons;

	if(NM->MaxDamage[0]>0 && !OBJ->Attack){
        OBJ->SearchVictim();
	}

	addrand(99);
	if((!ONM->NoFarm)&&NT->NFarms<=NT->NGidot&&GID>=0xFFFE&&(ONM->Peasant||!NM->PeasantConvertor)){
		if(!MOptions.DontTellAboutLivingPlaces){
			byte NI=GSets.CGame.cgi_NatRefTBL[MyNation];			
			if(OBJ->NNUM==NI){				
				void fes_neLivingPlaces();
				fes_neLivingPlaces();
			}
			addrand(100);
		}
		//checking if there are units that dont require living places
		Order1* PRE=NULL;
		Order1* NLP=NULL;
		Order1* ORD=OBJ->LocalOrder;
		while(ORD){
			if(ORD->DoLink==&ProduceObjLink){
				int ID=ORD->info.Produce.ObjIndex;
				GeneralObject* PG=NT->Mon[ID];
				if(PG->newMons->NoFarm){
					NLP=ORD;
                    break;
				}
				PRE=ORD;
			}
			ORD=ORD->NextOrder;
		}
		if(NLP){
            if(PRE)PRE->NextOrder=NLP->NextOrder;
			NLP->NextOrder=OBJ->LocalOrder;
			OBJ->LocalOrder=NLP;
		}
		return;
	}else{
		if( !ONM->NoFarm && NT->NFarms>NT->NGidot && OBJ->NNUM==MyNation ){
			extern bool fes_bLivingPlaces;
			fes_bLivingPlaces=true;
		}
	}

	if(NM->PeasantConvertor&&GID>=0xFFFE&&!ONM->Peasant){
		if(OBJ->NInside){
			bool found=0;
			word MID=OBJ->Inside[0];
			OneObject* OB=NULL;
			if(MID!=0xFFFF){
				OB=Group[MID];
				if(OB&&OB->newMons->Peasant)found=1;
			};
			if(!found){
				//need to sort units
				//if(rando()<2048)
				FindFirstPeasantInside(OBJ);
				word MID=OBJ->Inside[0];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB){
						if(OB->newMons->Peasant)found=1;

					};
				};
				if((!found)&&OBJ->NNUM==MyNation&&(OBJ->ImSelected&GM(OBJ->NNUM))){
					AssignHint1(GetTextByID("NOT_ENOUGH_PEASANTS"),10,32);
				};
				addrand(101);
				return;
			};
		}else{
			if(OBJ->NNUM==MyNation&&(OBJ->ImSelected&GM(OBJ->NNUM))){
				AssignHint1(GetTextByID("NOT_ENOUGH_PEASANTS"),10,32);
			};
			addrand(102);
			return;
		};
	};
	addrand(OBJ->LocalOrder->info.Produce.Power);
	if(OBJ->LocalOrder->info.Produce.Power==0xFFFF){
		addrand(17);
		OBJ->DeleteLastOrder();
		OBJ->Produce(OI,GID);
		addrand(103);
		return;
	};
	//if(int(NT->NFarms)*15<NT->NGidot+1){
	//	if(NT->NNUM==MyNation){
	//		sprintf(Prompt,"Недостаточно ферм.");
	//		PromptTime=100;
	//	};
	//	return;
	//};
	
	if(ONM->ArtSet&&NT->NArtUnits[ONM->ArtSet-1]>=NT->NArtdep[ONM->ArtSet-1]*ONM->NInArtDepot){
		UnProduce(OBJ,OI);
		addrand(104);
		return;
	};
	if(PGO->LockID!=0xFFFF){
		int n=NT->CITY->UnitAmount[PGO->LockID];
		int m=PGO->NLockUnits;
		if(n>=m){
			UnProduce(OBJ,OI);
			addrand(105);
			return;
		}
	}

	byte NI=NT->NNUM;
	//int xcost=NT->Mon[OI]->cost;
	byte step=OBJ->LocalOrder->info.Produce.PStep<<SpeedSh;
	int xxx=OBJ->LocalOrder->info.Produce.Progress;
	int nxxx=OBJ->LocalOrder->info.Produce.NStages;
	int pst=int(PADC->ProduceStages)<<8;
	if(NT->AI_Enabled){
		pst=(pst*DiffP[NT->CITY->Difficulty])>>8;
	};
	if(nxxx!=pst){
		xxx=(pst*xxx)/nxxx;
		nxxx=pst;
		OBJ->LocalOrder->info.Produce.NStages=pst;
		OBJ->LocalOrder->info.Produce.Progress=xxx;
	};
	if(NM->Port){
		if(!xxx){
			if(MFIELDS[1].CheckBar(OBJ->WallX-7,OBJ->WallY-7,15,15)||
				UnitsField.CheckBar(OBJ->WallX-7,OBJ->WallY-7,15,15)){
				MoveAwayShip(int(OBJ->WallX)<<8,int(OBJ->WallY)<<8);
				addrand(105);
				return;
			};
			MFIELDS[1].BSetBar(OBJ->WallX-7,OBJ->WallY-7,15);
		};
	};
	if(GID>=0xFFFE&&!PGO->Enabled){
		if(NM->Port){
			MFIELDS[1].BClrBar(OBJ->WallX-7,OBJ->WallY-7,15);
		};
		addrand(18);
		UnProduce(OBJ,OI);
		OBJ->DeleteLastOrder();
		addrand(106);
		return;
	};
//	byte NI=OBJ->NNUM;
	xxx+=GameSpeed;//step;
	OBJ->LocalOrder->info.Produce.Progress=xxx;
	if(xxx<nxxx)return;
	int xx,yy;
	OBJ->GetPreciseCornerXY(&xx,&yy);
	//if(!NM->NBornPt){
	//	OBJ->DeleteLastOrder();
	//};
	int CheckIfObjectIsWall(int NIndex);
	if(CheckIfObjectIsWall(OI)){
		void UpgradeWallObject(OneObject* OB,int NewType);
		UpgradeWallObject(OBJ,OI);
		addrand(19);
		OBJ->DeleteLastOrder();
		addrand(107);
		return;
	}
	if(NM->Port){
		MFIELDS[1].BClrBar(OBJ->WallX-7,OBJ->WallY-7,15);
		SubCost=true;
        SetMyActiveSubset();
		int ID=NT->CreateNewMonsterAt(int(OBJ->WallX)<<8,int(OBJ->WallY)<<8,OI,true);
		SetDefaultActiveSubset();
		SubCost=false;
		if(ID!=-1){
			OneObject* OB=Group[ID];
			if(OB){
				//if(OBJ->ActiveAbility)OBJ->ActiveAbility->ActiveAbilities.modifyNewborn(OB);
				OB->NMask=OBJ->NMask;
				OB->RealDir=96;
				OB->GraphDir=OB->RealDir;
				if(OBJ->DstX>0)
					OB->NewMonsterSmartSendTo((OBJ->DstX+((rando()-16384)>>2))>>4,(OBJ->DstY+((rando()-16384)>>2))>>4,0,0,128,2+128);
			};
			if(GID<=0xFFFE&&GID<SCENINF.UGRP.GetAmount()){
				UnitsGroup* UG=SCENINF.UGRP+GID;
				
				UG->Allocate(UG->N+1);
				UG->IDS[UG->N]=OB->Index;
				UG->SNS[UG->N]=OB->Serial;
				UG->N++;
				
				if(GID==OB->Nat->CITY->DestUnitsGroup)OB->Borg=1;
				if(UG->NMASK)OB->NMask=UG->NMASK;
			};
			if(AI_GID<=0xFFFE&&AI_GID<SCENINF.UGRP.GetAmount()){
				UnitsGroup* UG=SCENINF.UGRP+AI_GID;
				
				UG->Allocate(UG->N+1);
				UG->IDS[UG->N]=OB->Index;
				UG->SNS[UG->N]=OB->Serial;
				UG->N++;
				
				OB->Borg=1;
				if(UG->NMASK)OB->NMask=UG->NMASK;
			};
			ext_OnUnitWasProduced(OBJ,OB);
		}else{
			addrand(20);
			OBJ->DeleteLastOrder();
			AssignHint1(UNILIMIT,50,32);
			addrand(108);
			return;
		};
		addrand(21);
		OBJ->DeleteLastOrder();
		if(GID==0xFFFE){
			OBJ->Produce(OI,0xFFFE);
		};
		if(OBJ->NNUM==MyNation&&!OBJ->LocalOrder){
			/*
			char ccc[200];
			sprintf(ccc,FINBLD,OBJ->Ref.General->Message);
			AssignHint1(ccc,100);
			LastActionX=OBJ->RealX>>4;
			LastActionY=OBJ->RealY>>4;
			*/
		};
		addrand(109);
		return;
	};
	short* PTX=NM->BornPtX.GetValues();
	short* PTY=NM->BornPtY.GetValues();
	short* PTZ=NM->BornPtZ.GetValues();
	if (!(PTX && PTY && PTZ)) {
		OBJ->DeleteLastOrder();
		return;
	}
	int nc=NM->BornPtX.GetAmount();
	if(NATIONS->Mon[OI]->newMons->Artilery&&OBJ->newMons->CraftPtX.GetAmount()){
		nc= NM->CraftPtX.GetAmount();
		PTX=NM->CraftPtX.GetValues();
		PTY=NM->CraftPtY.GetValues();
		PTZ=NM->CraftPtZ.GetValues();
	};
	SubCost=true;
	if(PTX==NULL){
		OBJ->DeleteLastOrder();
		return;
	}
	int XC=((xx)+PTX[0])<<4;
	int YC=((yy)+PTY[0])<<4;
	bool Immediate=NATIONS->Mon[OI]->newMons->BornBehindBuilding;
	if(Immediate){
		XC=OBJ->RealX;
		YC=OBJ->RealY;
		int Y0=YC;
		int i;
		for(i=0;i<50;i++){
			if(!CheckBar( (XC>>8)-3,(YC>>8)-3,6,6) ){
				break;
			}
			YC-=256;
		}
		if(i==50){
			Immediate=false;
			YC=Y0;
		}
	}

	SetMyActiveSubset();
	int ID=NT->CreateNewMonsterAt(XC,YC,OI,true,nc>1?GetDir(PTX[1]-PTX[0],PTY[1]-PTY[0]):-1);
	SetDefaultActiveSubset();
	
	SubCost=false;
	if(ID!=-1){
		if(NM->PeasantConvertor&&GID>=0xFFFE&&!ONM->Peasant){
			if(OBJ->NInside){
				word M0=OBJ->Inside[0];
				if(M0!=0xFFFF){
					OneObject* DOB=Group[M0];
					if(DOB){
						int NIN=DOB->NIndex;
						int NN=DOB->Nat->NKilled[NIN];
						int ACC=DOB->Nat->CITY->Account;
						DOB->Die();
						DOB->Nat->NKilled[NIN]=NN;
						DOB->Nat->CITY->Account=ACC;
					};
				};
			};
		};
		OneObject* OB=Group[ID];
		ext_OnUnitWasProduced(OBJ,OB);
		NewMonster* NM1=OB->newMons;
		{		
			//if(OBJ->ActiveAbility)OBJ->ActiveAbility->ActiveAbilities.modifyNewborn(OB);
			OB->SetOrderedUnlimitedMotion(0);
			OB->NMask=OBJ->NMask;
			if(NM1->BornSoundID!=-1)
				//AddEffect(OB->RealX>>4,(OB->RealY>>5)-GetHeight(OB->RealX>>4,OB->RealY>>4),NM1->BornSoundID);
				AddEffect(OB->RealX/16,OB->RealY/16,NM1->BornSoundID);
			//OB->PrioryLevel=16;
			word GetDirDbg(int dx,int dy);
			OB->RealDir=byte(GetDir(PTX[1]-PTX[0],PTY[1]-PTY[0]));
			OB->GraphDir=OB->RealDir;
			addrand(OB->RealDir);
			if(!Immediate){
				for(int j=1;j<nc;j++){
					if(j==nc-1){
						//OB->ClearOrderedUnlimitedMotion(2);
						OB->NewMonsterPreciseSendTo3D(((xx)+PTX[j]+32-GetRND(64))<<4,((yy)+PTY[j]+32-GetRND(64))<<4,PTZ[j-1],PTZ[j],16,2+128);
					}
					else OB->NewMonsterPreciseSendTo3D(((xx)+PTX[j])<<4,((yy)+PTY[j])<<4,PTZ[j-1],PTZ[j],16,2+128);
				};
			}
			if(GID<0xFFFE)OB->ClearOrderedUnlimitedMotion(2,GID);
			else if(AI_GID<0xFFFE)OB->ClearOrderedUnlimitedMotion(2,AI_GID);
			else OB->ClearOrderedUnlimitedMotion(2,0xFFFF);

			if(OBJ->DstX<0&&!Immediate)OB->NewMonsterSendTo((((xx)+PTX[nc-1])<<4)+(rando()%4096)-2048,(((yy)+PTY[nc-1])<<4)+(rando()%4096)-2048,16,2+128);

			addrand(OB->RealDir);
			if(OBJ->DstX>0){
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
						OB->HideMe();
						OB->ClearOrders();
						OBJ->Inside=(word*)realloc(OBJ->Inside,(OBJ->NInside+1)<<1);
						OBJ->Inside[OBJ->NInside]=OB->Index;
						OBJ->NInside++;
						addrand(22);
						OBJ->DeleteLastOrder();
						if(GID==0xFFFE){
							OBJ->Produce(OI,0xFFFE);
						};
						return;
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
						int dx=OBJ->DstX+(rando()%2048)-1024;
						int dy=OBJ->DstY+(rando()%2048)-1024;
						FindUnitPosition(&dx,&dy,OB->NIndex);
						OB->NewMonsterSmartSendTo(dx>>4,dy>>4,0,0,128,2+128);
					};
				};
			};
			GotoFinePosition(OB);;
			if(!OB->LockType)OB->UnlimitedMotion=true;
			OB->GroundState=0;
			addrand(OB->GroundState+1);
		};
	}else{
		addrand(23);
		OBJ->DeleteLastOrder();
		AssignHint1(UNILIMIT,50,32);
		addrand(110);
		return;
	};
	addrand(24);
	OBJ->DeleteLastOrder();
	if(GID==0xFFFE){
		OBJ->Produce(OI,0xFFFE);
	};
	if(OBJ->NNUM==MyNation&&!OBJ->LocalOrder){
		/*
		char ccc[200];
		sprintf(ccc,FINBLD,OBJ->Ref.General->Message);
		AssignHint1(ccc,100);
		LastActionX=OBJ->RealX>>4;
		LastActionY=OBJ->RealY>>4;
		*/
	};
	addrand(111);
};
int EnumCannonsInCell(int cell,int x,int y,int r,int MyID){
	int N=0;
	if(cell>0){
		cell+=VAL_MAXCX+1;
		if(cell>=VAL_MAXCX*VAL_MAXCX)return 0;
		int NMon=MCount[cell];
		if(!NMon)return NULL;
		int ofs1=cell<<SHFCELL;
		word MID;
		for(int i=0;i<NMon;i++){
			MID=GetNMSL(ofs1+i);
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&(!(OB->Guard!=0xFFFF||OB->Sdoxlo))&&OB->Index!=MyID&&OB->MotionStyle==3&&Norma(OB->RealX-x,OB->RealY-y)<r)N++;
			};
		};
	};
    return N;
};
bool SearchForCannonInRadius(int x,int y,int r,int MyID){
    int rx1=(r>>11)+1;
	int N=0;
	int x0=x>>11;
	int y0=y>>11;
	int maxx=msx>>2;
	for(int R=0;R<=rx1;R++){
		char* xi=Rarr[R].xi;
		char* yi=Rarr[R].yi;
		int NN=Rarr[R].N;
		for(int p=0;p<NN;p++){
			int xx=x0+xi[p];
			int yy=y0+yi[p];
			if(xx>=0&&yy>=0&&xx<maxx&&yy<maxx){
				int cell=xx+(yy<<VAL_SHFCX);
				if(EnumCannonsInCell(cell,x,y,r,MyID))return true;
			};
		};
	};
	return false;
};
int TryToMove(OneObject* OB,byte NewDir,bool Dirc,int MyID);
void GotoFinePositionLink(OneObject* OB){
	if(OB->Attack&&OB->newMons->HaveRotateAnimations){
		OB->DeleteLastOrder();
		return;
	}
	if(OB->CObjIndex!=0xFFFF){
		OB->DeleteLastOrder();
		return;
	}
	if(OB->MotionStyle==3){
		//seeking for another cannon
		if(SearchForCannonInRadius(OB->RealX,OB->RealY,80*16,OB->Index)){
			OB->DeleteLastOrder();
			OB->NewMonsterSendTo(OB->RealX+(int(TCos[OB->RealDir])*8),OB->RealY+(int(TSin[OB->RealDir])*8),16+128,1);
		}else{
			OB->DeleteLastOrder();
		}
		return;
	};
	int ms=OB->MotionStyle;
	if( (OB->LockType!=0 && OB->LockType!=3) ||ms==2||ms==5||ms==8){
		OB->DeleteLastOrder();
		return;
	};
	if(OB->BrigadeID!=0xFFFF){
		OB->DeleteLastOrder();
		return;
	};
	if(OB->FrameFinished){
		OB->Sdoxlo=true;
		int xx=OB->RealX;
		int yy=OB->RealY;
		int xx0=xx;
		int yy0=yy;
		FindUnitPosition(&xx,&yy,OB->NIndex);
		OB->Sdoxlo=0;
		if(xx==xx0&&yy==yy0){
			OB->DeleteLastOrder();
			return;
		}else{
			OB->NewMonsterSendTo(xx,yy,OB->PrioryLevel,1);
		};
	};
};
void GotoFinePosition(OneObject* OB){
	return;
	if(OB->Attack&&OB->newMons->HaveRotateAnimations)return;
	if(OB->CantRestrictMotion||OB->LockedInBuilding!=0xFFFF)return;
	if(OB->NewBuilding||OB->Wall)return;
	if(OB->UnlimitedMotion)return;
	if(OB->BrigadeID!=0xFFFF)return;
	addrand(1027);
	Order1* OR1=OB->CreateOrder(2);
	if(OR1){
		OR1->DoLink=&GotoFinePositionLink;
		OR1->PrioryLevel=OB->PrioryLevel;
	}
};
void GotoFinePosition1(OneObject* OB){
	return;
	if(OB->LockedInBuilding!=0xFFFF)return;
	if(OB->CantRestrictMotion)return;
	if(OB->NewBuilding||OB->Wall)return;
	if(OB->UnlimitedMotion)return;
	if(OB->BrigadeID!=0xFFFF)return;
	addrand(1027);
	Order1* OR1=OB->CreateOrder(1);
	if(OR1){
		OR1->DoLink=&GotoFinePositionLink;
		OR1->PrioryLevel=OB->PrioryLevel;
	}
}
struct ProCash{
	word ID;
	byte NI;
	int Amount;
	int SubMoney[6];
};
ProCash UNICASH[32];
int CSSIZE=0;
void GetUnitCost(byte NI,word NIndex,int* Cost,int x,int y);
//extern int RESRC[8][8];
void GetCorrectMoney(byte NI,int* MONEY);
void TakeUnitFromCash(byte NI,word ID){
	for(int i=0;i<CSSIZE;i++)if(UNICASH[i].ID==ID&&UNICASH[i].NI==NI){
		if(UNICASH[i].Amount>1){
			UNICASH[i].Amount--;
		}else{
			if(i<CSSIZE-1){
				memcpy(UNICASH+i,UNICASH+i+1,(CSSIZE-i-1)*sizeof ProCash);
			};
			CSSIZE--;
			i--;
		};
	};
};
bool AddUnitsToCash(byte NI,word ID){
	int cps=-1;
	int addc=0;
	for(int i=0;i<CSSIZE&&cps==-1;i++)if(UNICASH[i].ID==ID&&UNICASH[i].NI==NI)cps=i;
	if(cps==-1){
		if(CSSIZE>=32)return true;
		cps=CSSIZE;
		CSSIZE++;
		addc=1;
		memset(UNICASH+cps,0,sizeof ProCash);
	};
	int COST[8];
	int CMON[8];
	GetUnitCost(NI,ID&8191,COST,0,0);
	GetCorrectMoney(NI,CMON);
	for(int i=0;i<6;i++)if(CMON[i]<COST[i]){
		CSSIZE-=addc;
		return false;
	};
	for(int i=0;i<6;i++)UNICASH[cps].SubMoney[i]=COST[i];
	UNICASH[cps].ID=ID;
	UNICASH[cps].NI=NI;
	UNICASH[cps].Amount++;
	return true;
};
void ClearUniCash(){
	CSSIZE=0;
};
void GetCorrectMoney(byte NI,int* MONEY){
	for(int i=0;i<8;i++)MONEY[i]=XRESRC(NI,i);
	for(int i=0;i<CSSIZE;i++){
		if(UNICASH[i].NI==NI){
			for(int j=0;j<6;j++){
				MONEY[j]-=UNICASH[i].SubMoney[j];
				if(MONEY[j]<0)MONEY[j]=0;
			};
		};
	};
};
int GetAmount(word ID){
	//RunPF(29);
	word CID=0;
	word Nsel=ImNSL[MyNation];
	word* SMon=ImSelm[MyNation];
	for(int i=0;i<Nsel;i++){
		word MID=SMon[i];
		if(MID!=0xFFFF){
			OneObject* OBJ=Group[MID];
			if(int(OBJ)){
				Order1* OR1=OBJ->LocalOrder;
				while(int(OR1)){
					if(OR1->OrderType==13&&
						OR1->info.Produce.ObjIndex==ID){
						if(OR1->info.Produce.ID==0xFFFE)CID+=1200;
						else CID++;
					};
					OR1=OR1->NextOrder;
				};
			};
		};
	};
	for(int i=0;i<CSSIZE;i++)if(UNICASH[i].NI==MyNation&&UNICASH[i].ID==ID)CID+=UNICASH[i].Amount;
	//StopPF(29);
	return CID;
};
int GetProgress(word ID,int* MaxProgress){	
	word CID=0;
	word Nsel=ImNSL[MyNation];
	word* SMon=ImSelm[MyNation];
	for(int i=0;i<Nsel&&i<30;i++){
		word MID=SMon[i];
		if(MID!=0xFFFF){
			OneObject* OBJ=Group[MID];
			if(OBJ && OBJ->NewBuilding){
				Order1* OR1=OBJ->LocalOrder;
				while(OR1){
					if(OR1&&OR1->OrderType==13&&
						OR1->info.Produce.ObjIndex==ID&&
						OR1->info.Produce.Progress>CID){
							CID=OR1->info.Produce.Progress>>8;
							*MaxProgress=OR1->info.Produce.NStages>>8;
							break;
					};
					OR1=OR1->NextOrder;
				}
			};
		};
	};
	return CID;
};
void GetUnitCost(byte NI,word NIndex,int* Cost);
void EliminateBuilding(OneObject* OB);
void UnConstructBuilduingLink(OneObject* OBJ){
	if(OBJ->NInside&&OBJ->Inside){
		word ID=OBJ->Inside[0];
		if(ID!=0xFFFF){
			OneObject* OB=Group[ID];
			if(OB){
				OBJ->LeaveMine(OB->NIndex);
				return;
			};
		};
	};
	OBJ->Ready=0;
	OBJ->StageState=0;
	if(OBJ->Stage){
		OBJ->Life=100;
		OBJ->Stage--;
		int ACC=CITY[OBJ->NNUM].Account;
		OBJ->NextStage();
		CITY[OBJ->NNUM].Account=ACC;
		OBJ->Stage--;
	}else{
		int NI=OBJ->NNUM;
		int UT=OBJ->NIndex;
		int ID=OBJ->Index;
		OBJ->ClearOrders();
		OBJ->DeletePath();
		EliminateBuilding(OBJ);
		OBJ->Nat->CITY->UnRegisterNewUnit(OBJ);
		DelObject(OBJ);
		if(OBJ->NInside){
			free(OBJ->Inside);
		};
		Group[ID]=NULL;
		CITY[NI].EnumUnits();
		int COST[8];
		GetUnitCost(NI,UT,COST,-1,-1);
		EliminateBuilding(OBJ);
		for(int i=0;i<6;i++){
			addrand(XRESRC(NI,i));
			AddXRESRC(NI,i,COST[i]);
			addrand(XRESRC(NI,i));
		};
	};
};
void UnConstructBuilduing(OneObject* OB){
	if(!(OB->NewBuilding&&OB->Ready))return;
	addrand(1028);
	Order1* OR1=OB->CreateOrder(0);
	OR1->DoLink=&UnConstructBuilduingLink;
	OR1->PrioryLevel=127;
	OR1->OrderType=172;
};

//////////////////////////////////////////////////////////////////////////
//                BUILDSTAGES PROCESSING FOR 3D BUILDINGS               //
//////////////////////////////////////////////////////////////////////////
class ActiveShowBuildStage:public ActiveUnitAbility{
public:
    int BuildTime;
	int CreationTime;
	int CurBStage;
	ActiveShowBuildStage(){
		BuildTime=-1;
		CreationTime=-1;
		CurBStage=0;
	}

	SAVE(ActiveShowBuildStage);
		REG_PARENT(ActiveUnitAbility);
		REG_MEMBER(_int,BuildTime);
	ENDSAVE;
    virtual bool Process();
	virtual void modifyModelAnimation	(bool Basic,bool& Current);
};
class ShowBuildStage:public UnitAbility{
public:
	SubSection BuildStageParams;
	int InitialGrowModel;
	int InitialGrowAnimation;
	int BuildStageModel;
	int BuildStageAnimation;
	int BuildStageCycledAnimation;
	int BuildStageDestructionModel;
	int BuildStageDestructionAnimation;
    bool ConstantProportions;	
	bool  InverseOrder;
	float ModelScale;
	float ExtraZScale;
	float MinStage;

	__constructor(ShowBuildStage){
        ModelScale=1.0f;
		ExtraZScale=1.0f;
	}
	SAVE(ShowBuildStage);
		REG_PARENT(UnitAbility);
		REG_AUTO(BuildStageParams);
		REG_MEMBER(_ModelID,InitialGrowModel);
		REG_MEMBER(_ModelID,InitialGrowAnimation);
		REG_MEMBER(_ModelID,BuildStageModel);
		REG_MEMBER(_ModelID,BuildStageAnimation);
		REG_MEMBER(_ModelID,BuildStageCycledAnimation);
		REG_MEMBER(_ModelID,BuildStageDestructionModel);
		REG_MEMBER(_ModelID,BuildStageDestructionAnimation);
		REG_MEMBER(_bool,ConstantProportions);
		REG_MEMBER(_bool,InverseOrder);
		REG_MEMBER(_float,ModelScale);
		REG_MEMBER(_float,ExtraZScale);
		REG_MEMBER(_float01,MinStage);
	ENDSAVE;
	virtual ActiveUnitAbility* GetNewActiveAbility(){
		ActiveShowBuildStage* AA=new ActiveShowBuildStage;
		AA->SetA(this);
		return AA;
	}
	//virtual bool GetClassPresentationMask(_str& dest,void* DataPtr,void* ExtraPtr,const char* options,DWORD Flags){
	//	dest="ShowBuildStage(%$BuildStageModel$%,%$BuildStageAnimation$%,%$BuildStageDestructionAnimation$%)";
	//	return true;
	//}	
};

bool ActiveShowBuildStage::Process(){
	InfluenceMask.set_bit(ABL_ModelAnimation);
	OB->ActiveAbility->ActiveAbilities.InfluenceMask.set_bit(ABL_ModelAnimation);
	if(CreationTime<0)CreationTime=TrueTime;
	if(GetUnitIndex()!=0xFFFF){
		ShowBuildStage* SBS=(ShowBuildStage*)GetA();
		if(!SBS)return false;
		OneObject* OB=Group[GetUnitIndex()];
		if(OB){
			if(OB->Stage<OB->NStages){
				BuildTime=TrueTime;
				int d=(CurBStage>>16)-OB->Stage;
				CurBStage-=GameSpeed*d*(abs(d)+6)/2;
			}else{
				if(TrueTime-BuildTime>IMM->GetAnimTime(SBS->BuildStageDestructionAnimation)*2)return false;
			}
		}
		return true;
	}
	return false;
}
void ActiveShowBuildStage::modifyModelAnimation	(bool Basic,bool& Current){
	ShowBuildStage* SBS=(ShowBuildStage*)GetA();
	if(!SBS)return;
	if(GetUnitIndex()!=0xFFFF){
		OneObject* OB=Group[GetUnitIndex()];
		if(OB && OB->NewAnm && OB->NewAnm->AnimationType==1){
            PushEntityContext(OB->Index);
			int MI=OB->NewAnm->ModelID;
			float sc2=OB->NewAnm->Scale;
			if(MI==-1 && OB->LoLayer){
				MI=OB->LoLayer->ModelID;
				sc2=OB->LoLayer->Scale;
			}
			AABoundBox BB=IMM->GetBoundBox(MI);
			float scale=sc2;
			if(OB->ActiveAbility){
				word sc=256;
				OB->ActiveAbility->ActiveAbilities.modifyScale(sc,sc);
				scale*=float(sc)/256.0f;
			}
			AABoundBox BB2=IMM->GetBoundBox(SBS->BuildStageModel);

			float s_sizeXY=std::max(BB.maxv.x-BB.minv.x,BB.maxv.y-BB.minv.y);
			float s_sizeZ=BB.maxv.z-BB.minv.z;
			float d_sizeXY=std::max(BB2.maxv.x-BB2.minv.x,BB2.maxv.y-BB2.minv.y);
			float d_sizeZ=BB2.maxv.z-BB2.minv.z;
			float scaleZ=scale;			
			scale*=s_sizeXY*SBS->ModelScale/d_sizeXY;
			scaleZ*=s_sizeZ*SBS->ExtraZScale/d_sizeZ;

			if(SBS->ConstantProportions){
                scale=max(scale,scaleZ);
				scaleZ=scale;
			}

            Matrix4D M;
			M.st(Vector3D(scale,scale,scaleZ),Vector3D(OB->RealX/16,OB->RealY/16,OB->RZ));
			if(OB->Stage<OB->NStages){
				bool done=false;
				if(SBS->InitialGrowModel>0 && SBS->InitialGrowAnimation>0){
					int T=IMM->GetAnimTime(SBS->InitialGrowAnimation);
					if(TrueTime-CreationTime<T){
						IMM->StartModel(SBS->InitialGrowModel,M,OB->Index);
						IMM->AnimateModel(SBS->InitialGrowAnimation,TrueTime-CreationTime);						
                        //IMM->RenderShadow(SBS->InitialGrowModel,&M);
						IMM->DrawModel();
                        IShadowMgr->AddCaster(SBS->InitialGrowModel,M);
                        IMM->RenderShadow(SBS->InitialGrowModel,&M);
						done=true;
					}
				}
				if(!done){
					DWORD context = (DWORD)this;
					PushEntityContext( context );
					IMM->StartModel(SBS->BuildStageModel,M,CUR_CONTEXT);
					float stg=float(CurBStage)/OB->NStages/65536.0f;
					stg=SBS->MinStage+stg*(1-SBS->MinStage);
					IMM->AnimateModel(SBS->BuildStageAnimation,IMM->GetAnimTime(SBS->BuildStageAnimation)*stg,0);
					if(SBS->BuildStageCycledAnimation>0)
						IMM->AnimateModel(SBS->BuildStageCycledAnimation,int(TrueTime-CreationTime)%int(IMM->GetAnimTime(SBS->BuildStageCycledAnimation)+2),0);
					
                    //IMM->RenderShadow(SBS->InitialGrowModel,&M);
					IMM->DrawModel();
                    IShadowMgr->AddCaster(SBS->BuildStageModel,M);
                    IMM->RenderShadow(SBS->BuildStageModel,&M);
					PopEntityContext();
				}
			}else{
				IMM->StartModel(SBS->BuildStageDestructionModel,M,OB->Index);
				int T=IMM->GetAnimTime(SBS->BuildStageDestructionAnimation);
				int DT=TrueTime-BuildTime;
				if(SBS->InverseOrder)DT=T-DT;
				IMM->AnimateModel(SBS->BuildStageDestructionAnimation,DT);				                
				IMM->DrawModel();				
                IShadowMgr->AddCaster(SBS->BuildStageDestructionModel,M);
                IMM->RenderShadow(SBS->BuildStageDestructionModel,&M);
			}
            PopEntityContext();
		}
	}	
}
void RegBuilClass(){
	REG_CLASS(ShowBuildStage);
	REG_CLASS(ActiveShowBuildStage);
}