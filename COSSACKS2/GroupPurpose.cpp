#include "stdheader.h"
#include "ua_Item.h"
#include "GroupPurpose.h"

GrpOrder* UnitsGroup::CreateOrder(byte Type){
	switch(Type){
	case 1:
		{
			GrpOrder* GO=new GrpOrder;
			memset(GO,0,sizeof GrpOrder);
			GO->Next=GOrder;
			GOrder=GO;
			return GO;
		};
	case 2:
		{
			GrpOrder* GO=GOrder;
			while(GO&&GO->Next)GO=GO->Next;
			if(GO){
				GO->Next=new GrpOrder;
				memset(GO->Next,0,sizeof GrpOrder);
				return GO;
			}else{
				GOrder=new GrpOrder;
				memset(GOrder,0,sizeof GrpOrder);
				return GOrder;
			};
		};
	default:
		ClearOrders();
		GOrder=new GrpOrder;
		memset(GOrder,0,sizeof GrpOrder);
		return GOrder;
	};
};
void UnitsGroup::ClearOrders(){
	while(GOrder)DeleteLastOrder();
};
void UnitsGroup::DeleteLastOrder(){
	if(GOrder){
		GrpOrder* GRO=GOrder->Next;
		if(GOrder->Data)delete(GOrder->Data);
		delete(GOrder);
		GOrder=GRO;
	};
};
void LeaveMineLink(OneObject* OBJ);
void UGR_LeaveMineLink(UnitsGroup* UGR){
	word* UID=UGR->IDS;
	word* SNS=UGR->SNS;
	int N=UGR->N;
	for(int i=0;i<N;i++){
		word MID=UID[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]&&OB->Hidden&&(!OB->LocalOrder||OB->LocalOrder->DoLink!=&LeaveMineLink)){
				OB->LeaveMine(0);
			}
		}
	}
}
void LeaveMine(UnitsGroup* UGR){
	GrpOrder* GOR=UGR->CreateOrder(0);
	GOR->Link=&UGR_LeaveMineLink;
	GOR->DataSize=0;
	GOR->ID='LEBL';
};
void BuildObjLink(OneObject* OBJ);
void UGR_RepairBuildingLink(UnitsGroup* UGR){
	int* Data=(int*)UGR->GOrder->Data;
	int Idx=Data[0];
	int SN=Data[1];
	OneObject* OB=Group[Idx];
	bool done=0;
	if(OB&&OB->Serial==SN&&(!OB->Sdoxlo)&&OB->Life<OB->MaxLife){
		word* UID=UGR->IDS;
		word* SNS=UGR->SNS;
		int N=UGR->N;
		for(int i=0;i<N;i++){
			word MID=UID[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==SNS[i]&&!(OB->UnlimitedMotion||OB->LockedInBuilding!=0xFFFF)){
					if(!(OB->LocalOrder&&OB->LocalOrder->DoLink==&BuildObjLink)){
						Order1* OR1=OB->LocalOrder;
						bool BLD=0;
						while(OR1&&!BLD){
							BLD=OR1->DoLink==&BuildObjLink;
							OR1=OR1->NextOrder;
						};
						OB->BuildObj(Idx,128+16,1,0);
						done=1;
					};
				};
			};
		};
		if(done){
			OB->ClearBuildPt();
		};
	}else{
		UGR->DeleteLastOrder();
	};
	
};
void RepairBuilding(UnitsGroup* UGR,byte Type,int Index){
	if(Index<0||Index>=MAXOBJECT)return;
	OneObject* OB=Group[Index];
	if(OB&&(!OB->Sdoxlo)&&OB->NewBuilding&&OB->Life<OB->MaxLife){
		GrpOrder* GOR=UGR->CreateOrder(Type);
		GOR->Data=new int[2];
		((int*)GOR->Data)[0]=Index;
		((int*)GOR->Data)[1]=OB->Serial;
		GOR->Link=&UGR_RepairBuildingLink;
		GOR->DataSize=8;
		GOR->ID='REPB';
	};
};
struct TakeRes_Data{
	int NSklads;
	int StoreX[16];
	int StoreY[16];
	int ResType;
	int LastRefTime;
	byte NI;
};
struct TakeRes_Data_Zone: public TakeRes_Data{
	int xc,yc,R;	// zone for working
};
extern int tmtmt;
void TakeResLink(OneObject* OBJ);
DLLEXPORT
bool IsResourceEnabled(byte NI, word ResID){
	word* IDX=NatList[NI];
	int N=NtNUnits[NI];
	word ms=1<<ResID;
	if(ms){
		for(int i=0;i<N;i++){
			word MID=IDX[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->NewBuilding&&OB->Ready){
					NewMonster* NM=OB->newMons;
					if(NM->ResConcentrator&ms){
						int sx=OB->RealX>>4;
						int sy=OB->RealY>>4;
						for(int k=0;k<16;k++){
							int xx=sx+(rando()&1023)-512;
							int yy=sy+(rando()&1023)-512;
							byte rt=DetermineResource(xx,yy);
							if(rt==ResID){
								return true;
							};
						};
					};
				};
			};
		};
	};
	return false;
}
CEXPORT bool isMine(OneObject* OB){
	return OB->newMons->Producer;
}
CEXPORT bool isStorage(OneObject* OB){
	return OB->newMons->ResConcentrator||OB->AbRes;
}
void UGR_TakeResourcesLink(UnitsGroup* UGR){
	int xc,yc,R;
	if(UGR->GOrder->ID=='TREZ'){
		TakeRes_Data_Zone* TRD=(TakeRes_Data_Zone*)UGR->GOrder->Data;
		xc=TRD->xc;
		yc=TRD->yc;
		R=TRD->R;
	}else{
		R=-1;
	}
	TakeRes_Data* TRD=(TakeRes_Data*)UGR->GOrder->Data;
	if((!TRD->LastRefTime)||tmtmt-TRD->LastRefTime>64||rando()<4096){
		TRD->LastRefTime=tmtmt;
		TRD->NSklads=0;
		word* IDX=NatList[TRD->NI];
		int N=NtNUnits[TRD->NI];
		word ms=1<<TRD->ResType;
		if(ms){
			for(int i=0;i<N;i++){
				word MID=IDX[i];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&OB->NewBuilding&&OB->Ready&&(R<0||Norma(OB->RealX-xc,OB->RealY-yc)<R)){
						NewMonster* NM=OB->newMons;						
						if(TRD->NSklads<16){
							if(NM->Producer&&NM->ProdType&ms){
								TRD->StoreX[TRD->NSklads]=-1234;
								TRD->StoreY[TRD->NSklads]=OB->Index;
								TRD->NSklads++;
							}else
							if((NM->ResConcentrator&ms)||(OB->AbRes&ms)){
								TRD->StoreX[TRD->NSklads]=OB->RealX;
								TRD->StoreY[TRD->NSklads]=OB->RealY;
								TRD->NSklads++;
							}
						};
					};
				};
			};
		};
	};
	if(TRD->NSklads){
		word* UID=UGR->IDS;
		word* SNS=UGR->SNS;
		int N=UGR->N;
		for(int i=0;i<N;i++){
			word MID=UID[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==SNS[i]&&!(OB->UnlimitedMotion||OB->LockedInBuilding!=0xFFFF)){
					int rtp=-1;
					if(OB->LocalOrder&&OB->LocalOrder->DoLink==&TakeResLink){
						rtp=OB->LocalOrder->info.TakeRes.ResID;
					}else{
						Order1* OR1=OB->LocalOrder;
						bool BLD=0;
						while(OR1&&!BLD){
							void GoToMineLink(OneObject* OBJ);
							if(OR1->DoLink==&GoToMineLink)rtp=TRD->ResType;
							if(BLD=(OR1->DoLink==&TakeResLink)){
								rtp=OR1->info.TakeRes.ResID;
							};
							OR1=OR1->NextOrder;
						};
					};
					if(rtp!=TRD->ResType){
						//lazy pig... Work now!!!
						int MINDST=1000000;
						int sx=-1;
						int sy=-1;
						/*
						for(int j=0;j<TRD->NSklads;j++){
							int N=Norma(TRD->StoreX[j]-OB->RealX,TRD->StoreY[j]-OB->RealY);
							if(N<MINDST){
								MINDST=N;
								sx=TRD->StoreX[j];
								sy=TRD->StoreY[j];
							};
						};
						*/
						int sid=(rando()*TRD->NSklads)>>15;
						sx=TRD->StoreX[sid];
						sy=TRD->StoreY[sid];

						if(sx==-1234){
							OB->GoToMine(sy,128+16,0);
						}else
						if(sx!=-1){
							//search for a resource
							int MinR=100000;
							int rx=-1;
							int ry=-1;
							sx>>=4;
							sy>>=4;
							for(int k=0;k<32;k++){
								int xx=sx+(rando()&2047)-1024;
								int yy=sy+(rando()&2047)-1024;
								byte rt=DetermineResource(xx,yy);
								if(rt==TRD->ResType){
									int RR=Norma(sx-xx,sy-yy);
									if(RR<MinR){
										MinR=RR;
										rx=xx;
										ry=yy;
									};
								};
							};
							if(MinR<100000){
								OB->TakeResource(rx,ry,TRD->ResType,128+1,0);
							}else{
								if(!OB->LocalOrder&&rando()<400){
									int dx=(rando()&511)-256;
									int dy=(rando()&511)-256;
									int norm=Norma(dx,dy);
									dx+=dx*120/norm;
									dy+=dy*120/norm;
									OB->NewMonsterSmartSendTo(sx+dx,sy+dy,0,0,128+1,0);
								}
							};
						};
					};
				};
			};
		};
	};
};
void TakeResources(UnitsGroup* UGR,byte Type,byte NI,byte ResType,GAMEOBJ* Zone){
	GrpOrder* GOR=UGR->CreateOrder(Type);

	int xc,yc,R;
	if(!Zone){
		R=-1;
	}else
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		xc=Zone->Index<<4;
		yc=Zone->Serial<<4;
		R=(Zone->Type&0xFFFFFF)<<4;
	}else
	if(Zone->Type!='ZONE'){
		R=-1;
	}else{
		ActiveZone* AZ=AZones[SCENINF.ZGRP[Zone->Index].ZoneID[0]];
		xc=AZ->x<<4;
		yc=AZ->y<<4;
		R=AZ->R<<4;
	};

	if(R>0){
		TakeRes_Data_Zone* TRD=new TakeRes_Data_Zone;
		
		GOR->Data=TRD;
		GOR->Link=&UGR_TakeResourcesLink;
		GOR->DataSize=sizeof TakeRes_Data_Zone;
		GOR->ID='TREZ';
		memset(GOR->Data,0,GOR->DataSize);
	
		TRD->ResType=ResType;	
		TRD->NI=NI;
		
		TRD->xc=xc;
		TRD->yc=yc;
		TRD->R=R;
	}else{
		TakeRes_Data* TRD=new TakeRes_Data;
		
		GOR->Data=TRD;
		GOR->Link=&UGR_TakeResourcesLink;
		GOR->DataSize=sizeof TakeRes_Data;
		GOR->ID='TRES';
		memset(GOR->Data,0,GOR->DataSize);
	
		TRD->ResType=ResType;	
		TRD->NI=NI;		
	}
	
};
void GoToMineLink(OneObject* OB);
void UGR_ComeIntoBuildingLink(UnitsGroup* UGR){
	int* Data=(int*)UGR->GOrder->Data;
	int Idx=Data[0];
	int SN=Data[1];
	OneObject* OB=Group[Idx];
	if(OB&&OB->Serial==SN){
		word* UID=UGR->IDS;
		word* SNS=UGR->SNS;
		int N=UGR->N;
		for(int i=0;i<N;i++){
			word MID=UID[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==SNS[i]&&!(OB->UnlimitedMotion||OB->LockedInBuilding!=0xFFFF)){
					Order1* OR1=OB->LocalOrder;
					bool GO=0;
					while(OR1&&!GO){
						if(OR1->DoLink==&GoToMineLink&&OR1->info.BuildObj.ObjIndex==Idx)GO=1;
						OR1=OR1->NextOrder;
					};
					if(!GO){
						OB->GoToMine(Idx,128+16,0);
					};
				};
			};
		};
	};
};
void ComeIntoBuilding(UnitsGroup* UGR,byte Type,int Index){
	if(Index<0||Index>0xFFFE)return;
	OneObject* OB=Group[Index];
	if(OB&&(!OB->Sdoxlo)&&OB->NewBuilding){
		GrpOrder* GOR=UGR->CreateOrder(Type);
		GOR->Data=new int[2];
		((int*)GOR->Data)[0]=Index;
		((int*)GOR->Data)[1]=OB->Serial;
		GOR->Link=&UGR_ComeIntoBuildingLink;
		GOR->DataSize=8;
		GOR->ID='IBLD';
	};
};

DLLEXPORT
void SGP_LeaveBuilding(GAMEOBJ* Grp){
	if(Grp->Type=='UNIT'&&Grp->Index<SCENINF.UGRP.GetAmount()){
		UGR_LeaveMineLink(SCENINF.UGRP+Grp->Index);
	};
};
extern "C" CEXPORT
void SGP_RepairBuilding(GAMEOBJ* Grp,byte Type,int Index){
	if(Grp->Type=='UNIT'&&Grp->Index<SCENINF.UGRP.GetAmount()){
		RepairBuilding(SCENINF.UGRP+Grp->Index,Type,Index);
	};
};
DLLEXPORT void SGP_TakeResources(GAMEOBJ* Grp,byte Type,byte NI,int ResType){
	if(Grp->Type=='UNIT'&&Grp->Index<SCENINF.UGRP.GetAmount()){
		TakeResources(SCENINF.UGRP+Grp->Index,Type,NI,ResType,NULL);
	};
};
DLLEXPORT void SGP_TakeResourcesZone(GAMEOBJ* Grp,byte Type,byte NI,int ResType,GAMEOBJ* Zone){
	if(Grp->Type=='UNIT'&&Grp->Index<SCENINF.UGRP.GetAmount()){
		TakeResources(SCENINF.UGRP+Grp->Index,Type,NI,ResType,Zone);
	};
};
extern "C" CEXPORT
void SGP_ComeIntoBuilding(GAMEOBJ* Grp,byte Type,int Index){
	if(Grp->Type=='UNIT'&&Grp->Index<SCENINF.UGRP.GetAmount()){
		ComeIntoBuilding(SCENINF.UGRP+Grp->Index,Type,Index);
	};
};
extern City CITY[8];
extern "C" CEXPORT
void SetBuildingsCollector(byte NI,GAMEOBJ* Grp){
	if(NI<8&&Grp->Type=='UNIT'&&Grp->Index<SCENINF.UGRP.GetAmount()){
		CITY[NI].DestBldGroup=Grp->Index;
		for(int i=0;i<MAXOBJECT;i++){
			OneObject* OB=Group[i];
			if(OB&&OB->NewBuilding&&!OB->Sdoxlo){
				if(OB->Nat->CITY->DestBldGroup!=0xFFFF){
					UnitsGroup* UG=SCENINF.UGRP+OB->Nat->CITY->DestBldGroup;
					UG->AddNewUnit(OB);
				};
			};
		};
	};
};
extern "C" CEXPORT
void SetUnitsCollector(byte NI,GAMEOBJ* Grp){
	if(NI<8&&Grp->Type=='UNIT'&&Grp->Index<SCENINF.UGRP.GetAmount()){
		CITY[NI].DestUnitsGroup=Grp->Index;
		for(int i=0;i<MAXOBJECT;i++){
			OneObject* OB=Group[i];
			if(OB&&!OB->NewBuilding&&!OB->Sdoxlo){
				if(OB->Nat->CITY->DestUnitsGroup!=0xFFFF){
					UnitsGroup* UG=SCENINF.UGRP+OB->Nat->CITY->DestUnitsGroup;
					UG->AddNewUnit(OB);
				};
			};
		};
	};
};
/*
DLLEXPORT
bool RemoveUnitsToCollector(GAMEOBJ* Grp){
	if(NI<8&&Grp->Type=='UNIT'&&Grp->Index<SCENINF.NUGRP){
		CITY[NI].DestEnemyGroup=Grp->Index;
		UnitsGroup* UG=SCENINF.UGRP+Grp->Index;
		CheckDynamicalPtr(UG->IDS);
		
		for(int i=0;i<MAXOBJECT;i++){
			OneObject* OB=Group[i];
			if(OB->Nat->CITY->DestUnitsGroup!=0xFFFF){
				UnitsGroup* UG=SCENINF.UGRP+OB->Nat->CITY->DestUnitsGroup;
				UG->AddNewUnit(OB);
			};
		}
	}
}
*/
DLLEXPORT
void SetEnemyBuildingsCollector(byte NI,GAMEOBJ* Grp){
	if(NI<8&&Grp->Type=='UNIT'&&Grp->Index<SCENINF.UGRP.GetAmount()){
		CITY[NI].DestEnemyGroup=Grp->Index;
		UnitsGroup* UG=SCENINF.UGRP+Grp->Index;
		CheckDynamicalPtr(UG->IDS);
		for(int i=0;i<MAXOBJECT;i++){
			OneObject* OB=Group[i];
			if(OB&&OB->NewBuilding&&!OB->Borg){
				if(!(OB->NMask&NATIONS[NI].NMask)){
					UG->Allocate(UG->N+1);
					UG->IDS[UG->N]=OB->Index;
					UG->SNS[UG->N]=OB->Serial;
					UG->N++;
					OB->Borg=1;
				};
			};
		};
		CheckDynamicalPtr(UG->IDS);
	};
};
void CreateFields(byte NI,int x,int y,int n);
extern "C" CEXPORT
void CreateFields(byte NI){
	int MIDXS[64];
	int NMLN=0;
	word* Units=NatList[NI];
	int N=NtNUnits[NI];
	for(int i=0;i<N;i++){
		word MID=Units[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(!OB->Sdoxlo)&&OB->Ready&&OB->newMons->Usage==MelnicaID&&OB->NNUM==NI&&!OB->AutoZasev){
				if(NMLN<64){
					MIDXS[NMLN]=OB->Index;
					NMLN++;
				};
			};
		};
	};
	if(NMLN){
		int RID=(tmtmt/71)%NMLN;
		OneObject* OB=Group[MIDXS[RID]];
		if(OB->Nat->PACount[OB->NIndex])CreateFields(NI,OB->RealX,OB->RealY,OB->Nat->PAble[OB->NIndex][0]);
	};
};
extern "C" CEXPORT
int GetMaxUnits(byte NI){
	if(NI<8)return NATIONS[NI].NFarms;
	else return 0;
};
extern "C" CEXPORT
int GetCurrentUnits(byte NI){
	if(NI<8)return NATIONS[NI].NGidot;
	else return 0;
};
extern "C" CEXPORT
int GetInsideAmount(int Index){
	if(Index>=0&&Index<0xFFFF){
		OneObject* OB=Group[Index];
		if(OB&&!OB->Sdoxlo)return OB->NInside;
	};
	return 0;
};
extern "C" CEXPORT
int GetMaxInsideAmount(int Index){
	if(Index>=0&&Index<0xFFFF){
		OneObject* OB=Group[Index];
		if(OB&&!OB->Sdoxlo)return OB->MoreCharacter->MaxInside+OB->AddInside;
	};
	return 0;
};
//Fields
#define AFL_COOR     1
#define AFL_NUNITS   2
#define AFL_FORCE    4
#define AFL_MINIMAX  8
extern short LastDirection;
DLLEXPORT
int EnumEnemyArmies(byte NI,DWORD Fields,DWORD* Data,int BufSize){
	int pos=0;
	EnemyInfo* EINF=GNFO.EINF[NI];
	if(EINF){
		ArmyInfo* AINF=EINF->GAINF.AINF;
		int NAINF=EINF->GAINF.NArmy;
		for(int i=0;i<NAINF;i++){
			if(pos<BufSize&&Fields&AFL_COOR){
				Data[pos]=AINF->xc;
				Data[pos+1]=AINF->yc;
				pos+=2;
			};
			if(pos<BufSize&&Fields&AFL_NUNITS){
				Data[pos]=AINF->NUnits;
				pos++;
			};
			if(pos<BufSize&&Fields&AFL_FORCE){
				Data[pos]=AINF->N;
				pos++;
			};
			if(pos<BufSize&&Fields&AFL_MINIMAX){
				Data[pos  ]=AINF->MinX;
				Data[pos+1]=AINF->MinY;
				Data[pos+2]=AINF->MaxX;
				Data[pos+3]=AINF->MaxY;
				pos+=4;
			};
			AINF++;
		};
		int sz=0;
		if(Fields&AFL_COOR)sz+=2;
		if(Fields&AFL_NUNITS)sz++;
		if(Fields&AFL_FORCE)sz++;
		if(Fields&AFL_MINIMAX)sz+=4;
		if(sz)return pos/sz;
		else return 0;
	};
	return 0;
};
#define MSO_ALLOW_ENEMY_SEARCH 1
//#define MSO_ALLOW_ANSWER_ATTACK 2
//#define MSO_ALLOW_ONLY_ARMATTACK 4
//#define MSO_ALLOW_ATTACK_WITHOUT_MOVEMENT 8
#define MSO_CANCEL_WHEN_ATTACKED 16
#define MSO_CANCEL_WHEN_FIRST 32

struct GMovementData{
	int DestX;
	int DestY;
	int DestTop;
	int MovementStartTime;
	int FirstUnitReachedTime;
	int UInfo[32];
};
struct GCoorMovement{
	int StartTime;
	int FirstTime;
	DWORD Flags;
};

void ExGroupSendSelectedTo(byte NI,word* SMon,word* MSN,int Nsel,int x,int y,int DF,int DR,byte Prio,byte OrdType);
void GroupMoveToPointLink(UnitsGroup* UGR){
	GCoorMovement* GM=(GCoorMovement*)UGR->GOrder->Data;
	int dt0;
	if((dt0=tmtmt-GM->StartTime)>12){
		if(dt0>768){
			UGR->DeleteLastOrder();
			return;
		};
		int NORD=0;
		int NTOT=0;
		bool Cancel=0;
		for(int i=0;i<UGR->N;i++){
			word MID=UGR->IDS[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==UGR->SNS[i]&&!OB->Sdoxlo){
					NTOT++;
					if(OB->LocalOrder){
						/*if(OB->Attack){
							if(GM->Flags&MSO_CANCEL_WHEN_ATTACKED){
								UGR->DeleteLastOrder();
								return;
							};
						}else*/ NORD++;
					};
				};
			};
		};
		if(!NORD){
			UGR->DeleteLastOrder();
			return;
		};
		if(GM->FirstTime!=-1){
			if(NORD+NORD>NTOT&&tmtmt-GM->FirstTime>128){
				UGR->DeleteLastOrder();
				return;
			};
		}else{ 
			if(NTOT>NORD){
				GM->FirstTime=tmtmt;
				if(GM->Flags&MSO_CANCEL_WHEN_FIRST){
					UGR->DeleteLastOrder();
					return;
				};
			}
		};
	};
};
void GroupMoveToPoint(byte NI,UnitsGroup* UGR,int x,int y,int DF,int DR,int Direction,DWORD Flags){
	LastDirection=Direction==512?512:Direction&255;
	byte Prio=128+16;
	if(Flags&MSO_ALLOW_ENEMY_SEARCH){
		Prio=128+1;
		int N=UGR->N;
		for(int i=0;i<N;i++){
			word ID=UGR->IDS[i];
			word SN=UGR->SNS[i];
			OneObject* OB=Group[ID];
			if(OB&&OB->Serial==SN&&!OB->Sdoxlo&&OB->BrigadeID!=0xFFFF){
				CITY[OB->NNUM].Brigs[OB->BrigadeID].AttEnm=1;
				Prio=128;
			}
		}
	}
	ExGroupSendSelectedTo(NI,UGR->IDS,UGR->SNS,UGR->N,x<<4,y<<4,DF,DR,Prio,0);
	GCoorMovement* GM=(GCoorMovement*)malloc(sizeof GCoorMovement);
	GrpOrder* GO=UGR->CreateOrder(0);
	//GrpOrder* GO=UGR->CreateOrder(2);
	GO->Data=GM;
	GO->ID='MOVE';
	GO->Link=&GroupMoveToPointLink;
	GO->DataSize=sizeof GCoorMovement;
	GM->StartTime=tmtmt;
	GM->FirstTime=-1;
	GM->Flags=Flags;
	//LastDirection=512;
};
//
struct GAttackInZone{
	byte NI;
	int x,y,r;	
};
DLLEXPORT bool AttackEnemyInZone3(GAMEOBJ* Grp,GAMEOBJ* Zone,byte NI);
bool GroupAttackInZoneLink(UnitsGroup* UGR){
	GAttackInZone* GA=(GAttackInZone*)UGR->GOrder->Data;	
	GAMEOBJ Grp={UGR->Index,0,'UNIT'};
	GAMEOBJ Zone={GA->x,GA->y,'@   '-0x202020+GA->r};
	if( !AttackEnemyInZone3(&Grp,&Zone,GA->NI) ){
		UGR->DeleteLastOrder();
		return false;
	}
	return true;
}
void GroupAttackInZoneLink2(UnitsGroup* UGR){
	if( !GroupAttackInZoneLink(UGR) ){
		UGR->DeleteLastOrder();
	}
}
bool GroupAttackInZone(UnitsGroup* UGR, byte NI, int x, int y, int r){
	GAttackInZone* GA=NULL;
	GrpOrder* GO=UGR->GOrder;
	while(GO){
		if(GO->ID=='MOVE'){
			//return false;
			//
			//UGR->ClearOrders();
			//break;
		}else
		if(GO->ID=='ATCK'){
			GA=(GAttackInZone*)GO->Data;
			break;
		}
		GO=GO->Next;
	}
	if(!GA){		
		GA=new GAttackInZone;
		GrpOrder* GO=UGR->CreateOrder(1);
		GO->ID='ATCK';	
		GO->Data=GA;
		GO->DataSize=sizeof GAttackInZone;
		GO->Link=&GroupAttackInZoneLink2;
	}
	GA->NI=NI;
	GA->x=x;
	GA->y=y;
	GA->r=r;
	bool rez=GroupAttackInZoneLink(UGR);
	if(rez){
		if(UGR->GOrder->Next){
			GrpOrder* GOr=UGR->GOrder;
			GrpOrder* GOr2=GOr->Next;
			GrpOrder* GOr3=GOr2->Next;
			UGR->GOrder=GOr2;
			UGR->GOrder->Next=GOr;
			GOr->Next=GOr3;
			UGR->DeleteLastOrder();
		}
	}
	return rez;
}
//
DLLEXPORT bool DetectPanic(int Index);
bool GetGroupCenter(UnitsGroup* UGRP,int* x,int* y){
	int xx=0;
	int yy=0;
	int NU=0;
	for(int i=0;i<UGRP->N;i++){
		word MID=UGRP->IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==UGRP->SNS[i]&&!(OB->Sdoxlo/*||DetectPanic(OB->Index)*/)){
				xx+=OB->RealX>>4;
				yy+=OB->RealY>>4;
				NU++;
			};
		};
	};
	if(NU){
		*x=xx/NU;
		*y=yy/NU;
		return true;
	}else return false;
};
int GetTopDistance(int xa,int ya,int xb,int yb);
int GetTopology(int x,int y, byte LockType=0);
int GroupMakeOneStepTo(byte NI,UnitsGroup* GRP,int x,int y,int DF,int DR,int Direction,DWORD Flags){
	int gx,gy;
	if(GetGroupCenter(GRP,&gx,&gy)){
		int tr=GetTopDistance(gx>>6,gy>>6,x>>6,y>>6);
		if(tr<3000){
			int Top=GetTopology(gx,gy);
			if(Top<0xFFFE){
				int dtop=GetTopology(x,y);
				if(dtop<0xFFFE){
					int xd,yd;
					int ntop=GetMotionLinks(Top*GetNAreas()+dtop);
					if(dtop==ntop||dtop==Top){
						xd=x;
						yd=y;
					}else if(ntop<0xFFFE){
						Area* AR=GetTopMap(ntop);
						xd=AR->x<<6;
						yd=AR->y<<6;
					}else return false;
					GroupMoveToPoint(NI,GRP,xd,yd,DF,DR,Direction,Flags);
					return true;
				};
			};
		};
	};
	return false;
};

DLLEXPORT bool GetGrpCenter(GAMEOBJ* Grp,int* x,int* y){
	if(Grp->Type!='UNIT'||Grp->Index>=SCENINF.UGRP.GetAmount())return false;
	return GetGroupCenter(SCENINF.UGRP+Grp->Index,x,y);
};
DLLEXPORT void SGP_MoveToPoint(byte NI,GAMEOBJ* Grp,int x,int y,int Direction,int DF,int DR,DWORD Flags){
	if(Grp->Type!='UNIT'||Grp->Index>=SCENINF.UGRP.GetAmount())return;
	GroupMoveToPoint(NI,SCENINF.UGRP+Grp->Index,x,y,DF,DR,Direction,Flags);
};
DLLEXPORT
int SGP_MakeOneStepTo(byte NI,GAMEOBJ* Grp,int x,int y,int Direction,DWORD Flags){
	if(Grp->Type!='UNIT'||Grp->Index>=SCENINF.UGRP.GetAmount())return false;
	return GroupMakeOneStepTo(NI,SCENINF.UGRP+Grp->Index,x,y,0,0,Direction,Flags);
};
DLLEXPORT
int SGP_ExMakeOneStepTo(byte NI,GAMEOBJ* Grp,int x,int y,int DF,int DR,int Direction,DWORD Flags){
	if(Grp->Type!='UNIT'||Grp->Index>=SCENINF.UGRP.GetAmount())return false;
	return GroupMakeOneStepTo(NI,SCENINF.UGRP+Grp->Index,x,y,DF,DR,Direction,Flags);
};
DLLEXPORT DWORD GetCurrentPurpose(GAMEOBJ* GRP){
	if(GRP->Type!='UNIT'||GRP->Index>=SCENINF.UGRP.GetAmount())return 0;
	UnitsGroup* UG=SCENINF.UGRP+GRP->Index;
	if(UG->GOrder)return UG->GOrder->ID;
	else return 0;
};
DLLEXPORT
void CancelCurrentPurpose(GAMEOBJ* GRP){
	if(GRP->Type!='UNIT'||GRP->Index>=SCENINF.UGRP.GetAmount())return;
	UnitsGroup* UG=SCENINF.UGRP+GRP->Index;
	if(UG->GOrder)UG->DeleteLastOrder();
};
DLLEXPORT
void CancelAllPurposes(GAMEOBJ* GRP){
	if(GRP->Type!='UNIT'||GRP->Index>=SCENINF.UGRP.GetAmount())return;
	UnitsGroup* UG=SCENINF.UGRP+GRP->Index;
	UG->ClearOrders();
};
void MakeStandGroundTemp(Brigade* BR);
void CancelStandGround(Brigade* BR);

DLLEXPORT void SetUnitsState(GAMEOBJ* GRP,bool SearchEnemy,bool StandGround,bool ArmAttack,bool FriendlyFire){
	
	//
	return;
	//

	if(GRP->Type!='UNIT'||GRP->Index>=SCENINF.UGRP.GetAmount())	return;	
	// in Alexander "Rome"	
	//ArmAttack=false;
	//FriendlyFire=true;
	SearchEnemy=true;
	StandGround=false;
	//
	word BrigadeID=0xFFFF;
	byte NI=0;
	int ActivityState=0;
	UnitsGroup* UG=SCENINF.UGRP+GRP->Index;
	for(int i=0;i<UG->N;i++){
		word MID=UG->IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==UG->SNS[i]&&!OB->Sdoxlo){
				//NewMonster* NM=OB->newMons;
				//if(NM->ArmAttack)OB->ArmAttack=ArmAttack;
				//if(NM->FriendlyFire)OB->FriendlyFire=FriendlyFire;
				//OB->NoSearchVictim=!SearchEnemy;
				OB->ActivityState=SearchEnemy?2:0;
				if(StandGround) OB->ActivityState=1;
				//
				OB->GroundState=OB->ActivityState==2;
				if(OB->NewState!=OB->GroundState)OB->NewState=OB->GroundState;
				//
				if(OB->BrigadeID!=0xFFFF){
					BrigadeID=OB->BrigadeID;
					NI=OB->NNUM;
					ActivityState=OB->ActivityState;
				}
				//
				//StandGround=false;
				/*
				if(StandGround){
					if(OB->BrigadeID!=0xFFFF){
						Brigade* BR=OB->Nat->CITY->Brigs+OB->BrigadeID;
						MakeStandGroundTemp(BR);
					}else{
						OB->StandGround=true;
					};
				}else{
					if(OB->BrigadeID!=0xFFFF){
						//Brigade* BR=OB->Nat->CITY->Brigs+OB->BrigadeID;
						//CancelStandGround(BR);
					}else{
						OB->StandGround=false;
					};
				};
				*/
			};
		};
	};
	if(BrigadeID!=0xFFFF){
		Brigade* BR=CITY[NI].Brigs+BrigadeID;
		if(BR) BR->SetActivityState(ActivityState);
	}
}

DLLEXPORT
int GetEnemyBuildList(byte NI,word** IDS){
	word* ids=GNFO.EINF[NI]->EnmBuildList;
	word* sns=GNFO.EINF[NI]->EnmBuildSN;
	int N=GNFO.EINF[NI]->NEnmBuild;
	int pos=0;
	for(int i=0;i<N;i++){
		word MID=ids[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==sns[i]&&!OB->Sdoxlo){
				ids[pos]=ids[i];
				sns[pos]=sns[i];
				pos++;
			};
		};
	};
	N=pos;
	GNFO.EINF[NI]->NEnmBuild=N;
	*IDS=ids;
	return N;
};
struct EnemyInfo2{
	word Index;
	int NInside;
	int MaxInside;
	int x,y,NI;
	byte Usage;
};
DLLEXPORT
bool GetUnitInfo2(word Index,EnemyInfo2* BINF){
	if(Index!=0xFFFF){
		OneObject* OB=Group[Index];
		if(OB){
			BINF->Index=Index;
			BINF->MaxInside=OB->MoreCharacter->MaxInside+OB->AddInside;
			BINF->NInside=OB->NInside;
			BINF->NI=OB->NNUM;
			BINF->Usage=OB->newMons->Usage;
			BINF->x=OB->RealX>>4;
			BINF->y=OB->RealY>>4;
			return true;
		};
	};
	return false;
};
/*
#define ABA_FIRE_BUILDINGS							1
#define ABA_ELIMINATE_BUILDINGS						2
#define ABA_SHOT_CONCENTRATIONS_OF_ENEMY			4
#define ABA_SHOT_ONLY_BIG_CONCENTRATIONS_OF_ENEMY	8
#define ABA_HI_MINE_KILLING_PRIORY					16
*/
int GetBestVictimForArchers(byte NI,int x,int y,int R,int MyTop);
struct GAttackByArchers{
	byte NI;
	int xc,yc;
	word TopPos;
};
void AttackByArchersLink(UnitsGroup* UGR){
	GAttackByArchers* GAA=(GAttackByArchers*)UGR->GOrder->Data;
	int ID=GetBestVictimForArchers(GAA->NI,GAA->xc,GAA->yc,40,GAA->TopPos);
	if(ID!=0xFFFF){
		int N=UGR->N;
		for(int i=0;i<N;i++){
			word MID=UGR->IDS[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==UGR->SNS[i]){
					OB->AttackObj(ID,128+16,0,0);
				};
			};
		};
		UGR->GOrder->ID='ARCA';
	}else UGR->GOrder->ID='ARCP';
};
DLLEXPORT
bool SGP_AttackByArchers(byte NI,GAMEOBJ* GRP,int R,DWORD Flags){
	if(GRP->Type!='UNIT'||GRP->Index>=SCENINF.UGRP.GetAmount())return false;
	UnitsGroup* UG=SCENINF.UGRP+GRP->Index;
	int x,y;
	if(GetGroupCenter(UG,&x,&y)){
		GrpOrder* GO=UG->CreateOrder(0);
		GAttackByArchers* GAA=(GAttackByArchers*)malloc(sizeof(GAttackByArchers));
		GAA->TopPos=GetTopology(x,y);
		GAA->xc=x;
		GAA->yc=y;
		GAA->NI=NI;
		GO->ID='ARCA';
		GO->Link=&AttackByArchersLink;
		GO->Data=GAA;
		GO->DataSize=sizeof GAttackByArchers;
		return true;
	}else return false;
};
DLLEXPORT int GetTopZone(int x,int y){
	x>>=6;
	y>>=6;
	if(x>=0&&y>=0&&x<TopLx&&y<TopLy){
		for(int i=0;i<2;i++){
			Radio* R=Rarr+i;
			for(int j=0;j<R->N;j++){
				word Top=GetTopRef(x+R->xi[j]+((y+R->yi[j])<<TopSH));
				if(Top<0xFFFF) return Top;
			}
		}
	}
	return 0xFFFF;
};
DLLEXPORT
bool GetTopZoneCoor(int Zone,int* x,int* y){
	if(Zone>=0&&Zone<GetNAreas()){
		Area* AR=GetTopMap(Zone);
		*x=AR->x;
		*y=AR->y;
		return true;
	}else{
		*x=0;
		*y=0;
		return false;
	};
};
DLLEXPORT
int GetListOfNearZones(int Zone,OneLinkInfo** ZonesAndDist){
	if(Zone>=0&&Zone<GetNAreas()){
		Area* AR=GetTopMap(Zone);
		*ZonesAndDist=AR->Link;
		return AR->NLinks;
	}else return 0;
};
DLLEXPORT
int GetNextZone(int Start,int Fin){
	int NA=GetNAreas();
	if(Start>=0&&Start<NA&&Fin>=0&&Fin<NA){
		if(Start==Fin)return Fin;
		return GetMotionLinks(Start*NA+Fin);
	}else return 0xFFFF;
};
DLLEXPORT int GetZonesDist2(int Start,int Fin,byte TopType){
	int NA=GetNAreas(TopType);
	if(Start>=0&&Start<NA&&Fin>=0&&Fin<NA){
		if(Start==Fin)return 0;
		return GetLinksDist(Start*NA+Fin,TopType);
	}else return 0xFFFF;
};
DLLEXPORT int GetZonesDist(int Start,int Fin)
{
	return GetZonesDist2(Start,Fin,0);
}
DLLEXPORT int GetTopDist(int xa,int ya,int xb,int yb){
	return GetTopDistance(xa>>6,ya>>6,xb>>6,yb>>6);
};
DLLEXPORT int GetNZones(){
	return GetNAreas();
};
DLLEXPORT
bool CheckIfPointsNear(int xa,int ya,int xb,int yb){
	int T1=GetTopZone(xa,ya);
	int T2=GetTopZone(xb,yb);
	if(T1==T2)return true;
	int NA=GetNAreas();
	if(T1>=NA||T2>=NA)return false;
	if(GetNextZone(T1,T2)==T2)return true;
	return false;
};

word FULLWAY[128];
int NWPTS=0;
int TOPTYPE=0;
#define MXZ 8192
extern int TIME1;

DLLEXPORT int FindNextZoneOnTheSafeWay(int Start,int Fin,short* DangerMap,int* MaxDanger,int DangSteps){
	TIME1=GetTickCount();
	if(GetZonesDist(Start,Fin)>=0xFFFF)return 0xFFFF;
	word PrecisePointWeight    [MXZ];
	word PrecisePointPrevIndex [MXZ];
	word CandidatPointWeight   [MXZ];
	word CandidatPointPrevIndex[MXZ];
	word CandidatList          [MXZ];
	int NAR=GetNAreas();
	if(NAR>8192)NAR=8192;
	memset(PrecisePointWeight,0xFF,NAR<<1);
	memset(CandidatPointWeight,0xFF,NAR<<1);
	CandidatList[0]=Fin;
	CandidatPointWeight[Fin]=0;
	CandidatPointPrevIndex[Fin]=0xFFFF;
	int NCandidats=1;

	addrand(Start);
	addrand(Fin);

	do{
		//best candidat is a last candidat in sorted list
		NCandidats--;
		word TZ=CandidatList[NCandidats];
		PrecisePointWeight[TZ]=CandidatPointWeight[TZ];
		CandidatPointWeight[TZ]=0xFFFF;
		PrecisePointPrevIndex[TZ]=CandidatPointPrevIndex[TZ];
		if(TZ==Start){
			//it finally happen!!!
			int MaxDang=DangerMap[TZ];
			int T0=TZ;
			FULLWAY[0]=TZ;
			NWPTS=1;
			for(int q=0;q<DangSteps&&T0!=0xFFFF;q++){
				T0=CandidatPointPrevIndex[T0];
				if(T0!=0xFFFF){
					if(NWPTS<128){
						FULLWAY[NWPTS]=T0;
						NWPTS++;
						addrand(NWPTS);
					};
					int w=DangerMap[T0];
					if(w>MaxDang)MaxDang=w;
				};
			};
			//*MaxDanger=MaxDang;
			*MaxDanger=PrecisePointWeight[TZ];
			//TIME1=GetTickCount()-TIME1;
			return PrecisePointPrevIndex[TZ];
		};
		Area* TAR=GetTopMap(TZ);
		int NL=TAR->NLinks;
		OneLinkInfo* LINK=TAR->Link;
		int w0=PrecisePointWeight[TZ];
		for(int i=0;i<NL;i++){
			int pi=LINK[i].NextAreaID;
			//adding point to the candidats list
			//1.checking if point is in precise list
			if(PrecisePointWeight[pi]==0xFFFF){
				int wi=LINK[i].NextAreaDist+w0+DangerMap[pi];
				//2.checking if point is already in candidats list
				int wc;
				bool add=1;
				if((wc=CandidatPointWeight[pi])!=0xFFFF){
					if(wc>wi){
						//new point is better. Need to delete old point
						for(int j=0;j<NCandidats;j++)if(CandidatList[j]==pi){
							if(j<NCandidats-1)memcpy(CandidatList+j,CandidatList+j+1,(NCandidats-j-1)<<1);
							j=NCandidats;
							NCandidats--;
						};
						CandidatPointWeight[pi]=wi;
					}else{
						add=0;
					};
				};
				if(add){
					//need to add point to candidats list and sort array
					if(NCandidats==0){
						CandidatList[0]=pi;
						CandidatPointWeight[pi]=wi;
						CandidatPointPrevIndex[pi]=TZ;
						NCandidats++;
					}else{
						int IDXmax=0;
						int IDXmin=NCandidats-1;
						int wcmax=CandidatPointWeight[CandidatList[IDXmax]];
						int wcmin=CandidatPointWeight[CandidatList[IDXmin]];
						if(wi<=wcmin){
							CandidatList[NCandidats]=pi;
							CandidatPointWeight[pi]=wi;
							CandidatPointPrevIndex[pi]=TZ;
							NCandidats++;
						}else
						if(wi>wcmax){
							memmove(CandidatList+1,CandidatList,NCandidats<<1);
							CandidatList[0]=pi;
							CandidatPointWeight[pi]=wi;
							CandidatPointPrevIndex[pi]=TZ;
							NCandidats++;
						}else{
							while(IDXmax!=IDXmin-1){
								int IDXmid=(IDXmin+IDXmax)>>1;
								int wm=CandidatPointWeight[CandidatList[IDXmid]];
								if(wm>wi){
									wcmax=wm;
									IDXmax=IDXmid;
								}else{
									wcmin=wm;
									IDXmin=IDXmid;
								};
							};
							memmove(CandidatList+IDXmin+1,CandidatList+IDXmin,(NCandidats-IDXmin+1)<<1);
							CandidatList[IDXmin]=pi;
							CandidatPointWeight[pi]=wi;
							CandidatPointPrevIndex[pi]=TZ;
							NCandidats++;
						};
					};
				};
			};
		};
	}while(NCandidats);
	return 0xFFFF;
};
DLLEXPORT
int FindNextZoneOnTheSafeWayEx2(int Start,int Fin,int* DangerMap,int* MaxDanger,int DangSteps, byte TopType=0){
	TIME1=GetTickCount();
	if(GetZonesDist2(Start,Fin,TopType)>=0xFFFF)return 0xFFFF;
	static word PrecisePointWeight    [MXZ];
	static word PrecisePointPrevIndex [MXZ];
	static word CandidatPointWeight   [MXZ];
	static word CandidatPointPrevIndex[MXZ];
	static word CandidatList          [MXZ];
	int NAR=GetNAreas(TopType);
	if(NAR>8192)NAR=8192;
	memset(PrecisePointWeight,0xFF,NAR<<1);
	memset(PrecisePointPrevIndex,0x00,NAR<<1);
	memset(CandidatPointWeight,0xFF,NAR<<1);
	memset(CandidatPointPrevIndex,0x00,NAR<<1);
	memset(CandidatList,0x00,NAR<<1);
	CandidatList[0]=Fin;
	CandidatPointWeight[Fin]=0;
	CandidatPointPrevIndex[Fin]=0xFFFF;
	int NCandidats=1;

	addrand(Start);
	addrand(Fin);

	do{
		//best candidat is a last candidat in sorted list
		NCandidats--;
		word TZ=CandidatList[NCandidats];
		PrecisePointWeight[TZ]=CandidatPointWeight[TZ];
		CandidatPointWeight[TZ]=0xFFFF;
		PrecisePointPrevIndex[TZ]=CandidatPointPrevIndex[TZ];
		if(TZ==Start){
			//it finally happen!!!
			int MaxDang=DangerMap[TZ];
			int T0=TZ;
			FULLWAY[0]=TZ;
			NWPTS=1;
			for(int q=0;q<DangSteps&&T0!=0xFFFF;q++){
				T0=CandidatPointPrevIndex[T0];
				if(T0!=0xFFFF){
					if(NWPTS<128){
						FULLWAY[NWPTS]=T0;
						NWPTS++;
						addrand(NWPTS);
					};
					int w=DangerMap[T0];
					if(w>MaxDang)MaxDang=w;
				};
			};
			//*MaxDanger=MaxDang;
			*MaxDanger=PrecisePointWeight[TZ];
			//TIME1=GetTickCount()-TIME1;
			return PrecisePointPrevIndex[TZ];
		};
		Area* TAR=GetTopMap(TZ,TopType);
		int NL=TAR->NLinks;
		OneLinkInfo* LINK=TAR->Link;
		int w0=PrecisePointWeight[TZ];
		for(int i=0;i<NL;i++){
			int pi=LINK[i].NextAreaID;
			//adding point to the candidats list
			//1.checking if point is in precise list
			if(PrecisePointWeight[pi]==0xFFFF){
				int wi=LINK[i].NextAreaDist+w0+DangerMap[pi];
				//2.checking if point is already in candidats list
				int wc;
				bool add=1;
				if((wc=CandidatPointWeight[pi])!=0xFFFF){
					if(wc>wi){
						//new point is better. Need to delete old point
						for(int j=0;j<NCandidats;j++)if(CandidatList[j]==pi){
							if(j<NCandidats-1)memcpy(CandidatList+j,CandidatList+j+1,(NCandidats-j-1)<<1);
							j=NCandidats;
							NCandidats--;
						};
						CandidatPointWeight[pi]=wi;
					}else{
						add=0;
					};
				};
				if(add){
					//need to add point to candidats list and sort array
					if(NCandidats==0){
						CandidatList[0]=pi;
						CandidatPointWeight[pi]=wi;
						CandidatPointPrevIndex[pi]=TZ;
						NCandidats++;
					}else{
						int IDXmax=0;
						int IDXmin=NCandidats-1;
						int wcmax=CandidatPointWeight[CandidatList[IDXmax]];
						int wcmin=CandidatPointWeight[CandidatList[IDXmin]];
						if(wi<=wcmin){
							CandidatList[NCandidats]=pi;
							CandidatPointWeight[pi]=wi;
							CandidatPointPrevIndex[pi]=TZ;
							NCandidats++;
						}else
						if(wi>wcmax){
							memmove(CandidatList+1,CandidatList,NCandidats<<1);
							CandidatList[0]=pi;
							CandidatPointWeight[pi]=wi;
							CandidatPointPrevIndex[pi]=TZ;
							NCandidats++;
						}else{
							while(IDXmax!=IDXmin-1){
								int IDXmid=(IDXmin+IDXmax)>>1;
								int wm=CandidatPointWeight[CandidatList[IDXmid]];
								if(wm>wi){
									wcmax=wm;
									IDXmax=IDXmid;
								}else{
									wcmin=wm;
									IDXmin=IDXmid;
								};
							};
							memmove(CandidatList+IDXmin+1,CandidatList+IDXmin,(NCandidats-IDXmin+1)<<1);
							CandidatList[IDXmin]=pi;
							CandidatPointWeight[pi]=wi;
							CandidatPointPrevIndex[pi]=TZ;
							NCandidats++;
						};
					};
				};
			};
		};
	}while(NCandidats);
	return 0xFFFF;
};
DLLEXPORT
int FindNextZoneOnTheSafeWayEx(int Start,int Fin,int* DangerMap,int* MaxDanger,int DangSteps)
{
	return FindNextZoneOnTheSafeWayEx2(Start,Fin,DangerMap,MaxDanger,DangSteps,0);
}
DLLEXPORT int FindNextZoneOnTheSafeWayToObject(int Start,short* DangerMap,word* ObjIds,int* MaxDanger,int DangSteps,word* DestObj){
	TIME1=GetTickCount();
	//if(GetZonesDist(Start,Fin)>=0xFFFF)return 0xFFFF;
	int NAR=GetNAreas();
	if(Start<0||Start>=NAR)return 0xFFFF;
	word PrecisePointWeight    [MXZ];
	word PrecisePointPrevIndex [MXZ];
	word CandidatPointWeight   [MXZ];
	word CandidatPointPrevIndex[MXZ];
	word CandidatList          [MXZ];	
	if(NAR>8192)NAR=8192;
	memset(PrecisePointWeight,0xFF,NAR<<1);
	memset(CandidatPointWeight,0xFF,NAR<<1);
	CandidatList[0]=Start;
	CandidatPointWeight[Start]=0;
	CandidatPointPrevIndex[Start]=0xFFFF;
	int NCandidats=1;

	addrand(Start);

	do{
		//best candidat is a last candidat in sorted list
		NCandidats--;
		word TZ=CandidatList[NCandidats];
		if(TZ!=0xFFFF){
			PrecisePointWeight[TZ]=CandidatPointWeight[TZ];
			CandidatPointWeight[TZ]=0xFFFF;
			PrecisePointPrevIndex[TZ]=CandidatPointPrevIndex[TZ];
			if(ObjIds[TZ]!=0xFFFF){
				//it finally happen!!!
				int MaxDang=DangerMap[TZ];
				int T0=TZ;
				FULLWAY[0]=TZ;
				NWPTS=1;
				for(int q=0;T0!=0xFFFF && NWPTS<128 ;q++){
					T0=CandidatPointPrevIndex[T0];
					if(T0!=0xFFFF){
						if(NWPTS<128){
							FULLWAY[NWPTS]=T0;
							NWPTS++;
							addrand(NWPTS);
						};
						int w=DangerMap[T0];
						if(w>MaxDang)MaxDang=w;
					};
				};
				*MaxDanger=MaxDang;
				*DestObj=ObjIds[TZ];
				//TIME1=GetTickCount()-TIME1;
				if(NWPTS>1)return FULLWAY[NWPTS-2];
				else return FULLWAY[NWPTS-1];
			};
			Area* TAR=GetTopMap(TZ);
			int NL=TAR->NLinks;
			OneLinkInfo* LINK=TAR->Link;
			int w0=PrecisePointWeight[TZ];
			for(int i=0;i<NL;i++){
				int pi=LINK[i].NextAreaID;
				//adding point to the candidats list
				//1.checking if point is in precise list
				if(PrecisePointWeight[pi]==0xFFFF){
					int wi=LINK[i].NextAreaDist+w0+DangerMap[pi];
					//2.checking if point is already in candidats list
					int wc;
					bool add=1;
					if((wc=CandidatPointWeight[pi])!=0xFFFF){
						if(wc>wi){
							//new point is better. Need to delete old point
							for(int j=0;j<NCandidats;j++)if(CandidatList[j]==pi){
								if(j<NCandidats-1)memcpy(CandidatList+j,CandidatList+j+1,(NCandidats-j-1)<<1);
								j=NCandidats;
								NCandidats--;
							};
							CandidatPointWeight[pi]=wi;
						}else{
							add=0;
						};
					};
					if(add){
						//need to add point to candidats list and sort array
						if(NCandidats==0){
							CandidatList[0]=pi;
							CandidatPointWeight[pi]=wi;
							CandidatPointPrevIndex[pi]=TZ;
							NCandidats++;
						}else{
							int IDXmax=0;
							int IDXmin=NCandidats-1;
							int wcmax=CandidatPointWeight[CandidatList[IDXmax]];
							int wcmin=CandidatPointWeight[CandidatList[IDXmin]];
							if(wi<=wcmin){
								CandidatList[NCandidats]=pi;
								CandidatPointWeight[pi]=wi;
								CandidatPointPrevIndex[pi]=TZ;
								NCandidats++;
							}else
							if(wi>wcmax){
								memmove(CandidatList+1,CandidatList,NCandidats<<1);
								CandidatList[0]=pi;
								CandidatPointWeight[pi]=wi;
								CandidatPointPrevIndex[pi]=TZ;
								NCandidats++;
							}else{
								while(IDXmax!=IDXmin-1){
									int IDXmid=(IDXmin+IDXmax)>>1;
									int wm=CandidatPointWeight[CandidatList[IDXmid]];
									if(wm>wi){
										wcmax=wm;
										IDXmax=IDXmid;
									}else{
										wcmin=wm;
										IDXmin=IDXmid;
									};
								};
								memmove(CandidatList+IDXmin+1,CandidatList+IDXmin,(NCandidats-IDXmin+1)<<1);
								CandidatList[IDXmin]=pi;
								CandidatPointWeight[pi]=wi;
								CandidatPointPrevIndex[pi]=TZ;
								NCandidats++;
							};
						};
					};
				};
			};
		}else return 0xFFFF;
	}while(NCandidats);
	return 0xFFFF;
};

DLLEXPORT int FindNextZoneOnTheSafeWayToObjectEx(int Start,int* DangerMap,word* ObjIds,int* MaxDanger,int DangSteps,word* DestObj){
	TIME1=GetTickCount();
	//if(GetZonesDist(Start,Fin)>=0xFFFF)return 0xFFFF;
	int NAR=GetNAreas();
	if(Start<0||Start>=NAR)return 0xFFFF;
	word PrecisePointWeight    [MXZ];
	word PrecisePointPrevIndex [MXZ];
	word CandidatPointWeight   [MXZ];
	word CandidatPointPrevIndex[MXZ];
	word CandidatList          [MXZ];	
	if(NAR>8192)NAR=8192;
	memset(PrecisePointWeight,0xFF,NAR<<1);
	memset(CandidatPointWeight,0xFF,NAR<<1);
	CandidatList[0]=Start;
	CandidatPointWeight[Start]=0;
	CandidatPointPrevIndex[Start]=0xFFFF;
	int NCandidats=1;

	addrand(Start);

	do{
		//best candidat is a last candidat in sorted list
		NCandidats--;
		word TZ=CandidatList[NCandidats];
		if(TZ!=0xFFFF){
			PrecisePointWeight[TZ]=CandidatPointWeight[TZ];
			CandidatPointWeight[TZ]=0xFFFF;
			PrecisePointPrevIndex[TZ]=CandidatPointPrevIndex[TZ];
			if(ObjIds[TZ]!=0xFFFF){
				//it finally happen!!!
				int MaxDang=DangerMap[TZ];
				int T0=TZ;
				FULLWAY[0]=TZ;
				NWPTS=1;
				for(int q=0;T0!=0xFFFF && NWPTS<128 ;q++){
					T0=CandidatPointPrevIndex[T0];
					if(T0!=0xFFFF){
						if(NWPTS<128){
							FULLWAY[NWPTS]=T0;
							NWPTS++;
							addrand(NWPTS);
						};
						int w=DangerMap[T0];
						if(w>MaxDang)MaxDang=w;
					};
				};
				*MaxDanger=MaxDang;
				*DestObj=ObjIds[TZ];
				//TIME1=GetTickCount()-TIME1;
				if(NWPTS>1)return FULLWAY[NWPTS-2];
				else return FULLWAY[NWPTS-1];
			};
			Area* TAR=GetTopMap(TZ);
			int NL=TAR->NLinks;
			OneLinkInfo* LINK=TAR->Link;
			int w0=PrecisePointWeight[TZ];
			for(int i=0;i<NL;i++){
				int pi=LINK[i].NextAreaID;
				//adding point to the candidats list
				//1.checking if point is in precise list
				if(PrecisePointWeight[pi]==0xFFFF){
					int wi=LINK[i].NextAreaDist+w0+DangerMap[pi];
					//2.checking if point is already in candidats list
					int wc;
					bool add=1;
					if((wc=CandidatPointWeight[pi])!=0xFFFF){
						if(wc>wi){
							//new point is better. Need to delete old point
							for(int j=0;j<NCandidats;j++)if(CandidatList[j]==pi){
								if(j<NCandidats-1)memcpy(CandidatList+j,CandidatList+j+1,(NCandidats-j-1)<<1);
								j=NCandidats;
								NCandidats--;
							};
							CandidatPointWeight[pi]=wi;
						}else{
							add=0;
						};
					};
					if(add){
						//need to add point to candidats list and sort array
						if(NCandidats==0){
							CandidatList[0]=pi;
							CandidatPointWeight[pi]=wi;
							CandidatPointPrevIndex[pi]=TZ;
							NCandidats++;
						}else{
							int IDXmax=0;
							int IDXmin=NCandidats-1;
							int wcmax=CandidatPointWeight[CandidatList[IDXmax]];
							int wcmin=CandidatPointWeight[CandidatList[IDXmin]];
							if(wi<=wcmin){
								CandidatList[NCandidats]=pi;
								CandidatPointWeight[pi]=wi;
								CandidatPointPrevIndex[pi]=TZ;
								NCandidats++;
							}else
								if(wi>wcmax){
									memmove(CandidatList+1,CandidatList,NCandidats<<1);
									CandidatList[0]=pi;
									CandidatPointWeight[pi]=wi;
									CandidatPointPrevIndex[pi]=TZ;
									NCandidats++;
								}else{
									while(IDXmax!=IDXmin-1){
										int IDXmid=(IDXmin+IDXmax)>>1;
										int wm=CandidatPointWeight[CandidatList[IDXmid]];
										if(wm>wi){
											wcmax=wm;
											IDXmax=IDXmid;
										}else{
											wcmin=wm;
											IDXmin=IDXmid;
										};
									};
									memmove(CandidatList+IDXmin+1,CandidatList+IDXmin,(NCandidats-IDXmin+1)<<1);
									CandidatList[IDXmin]=pi;
									CandidatPointWeight[pi]=wi;
									CandidatPointPrevIndex[pi]=TZ;
									NCandidats++;
								};
						};
					};
				};
			};
		}else return 0xFFFF;
	}while(NCandidats);
	return 0xFFFF;
};

int FindNextZoneOnTheSafeWayToObjectWithCallback(int Start,short* DangerMap,tpGetObjID* fn,int* MaxDanger,int DangSteps,word* DestObj){
	TIME1=GetTickCount();
	//if(GetZonesDist(Start,Fin)>=0xFFFF)return 0xFFFF;
	int NAR=GetNAreas();
	if(Start<0||Start>=NAR)return 0xFFFF;
	word PrecisePointWeight    [MXZ];
	word PrecisePointPrevIndex [MXZ];
	word CandidatPointWeight   [MXZ];
	word CandidatPointPrevIndex[MXZ];
	word CandidatList          [MXZ];	
	if(NAR>8192)NAR=8192;
	memset(PrecisePointWeight,0xFF,NAR<<1);
	memset(CandidatPointWeight,0xFF,NAR<<1);
	CandidatList[0]=Start;
	CandidatPointWeight[Start]=0;
	CandidatPointPrevIndex[Start]=0xFFFF;
	int NCandidats=1;

	addrand(Start);

	do{
		//best candidat is a last candidat in sorted list
		NCandidats--;
		word TZ=CandidatList[NCandidats];
		if(TZ!=0xFFFF){
			PrecisePointWeight[TZ]=CandidatPointWeight[TZ];
			CandidatPointWeight[TZ]=0xFFFF;
			PrecisePointPrevIndex[TZ]=CandidatPointPrevIndex[TZ];
			if(fn(TZ)!=0xFFFF){
				//it finally happen!!!
				int MaxDang=DangerMap[TZ];
				int T0=TZ;
				FULLWAY[0]=TZ;
				NWPTS=1;
				for(int q=0;T0!=0xFFFF && NWPTS<128 ;q++){
					T0=CandidatPointPrevIndex[T0];
					if(T0!=0xFFFF){
						if(NWPTS<128){
							FULLWAY[NWPTS]=T0;
							NWPTS++;
							addrand(NWPTS);
						};
						int w=DangerMap[T0];
						if(w>MaxDang)MaxDang=w;
					};
				};
				*MaxDanger=MaxDang;
				*DestObj=fn(TZ);
				//TIME1=GetTickCount()-TIME1;
				if(NWPTS>1)return FULLWAY[NWPTS-2];
				else return FULLWAY[NWPTS-1];
			};
			Area* TAR=GetTopMap(TZ);
			int NL=TAR->NLinks;
			OneLinkInfo* LINK=TAR->Link;
			int w0=PrecisePointWeight[TZ];
			for(int i=0;i<NL;i++){
				int pi=LINK[i].NextAreaID;
				//adding point to the candidats list
				//1.checking if point is in precise list
				if(PrecisePointWeight[pi]==0xFFFF){
					int wi=LINK[i].NextAreaDist+w0+DangerMap[pi];
					//2.checking if point is already in candidats list
					int wc;
					bool add=1;
					if((wc=CandidatPointWeight[pi])!=0xFFFF){
						if(wc>wi){
							//new point is better. Need to delete old point
							for(int j=0;j<NCandidats;j++)if(CandidatList[j]==pi){
								if(j<NCandidats-1)memcpy(CandidatList+j,CandidatList+j+1,(NCandidats-j-1)<<1);
								j=NCandidats;
								NCandidats--;
							};
							CandidatPointWeight[pi]=wi;
						}else{
							add=0;
						};
					};
					if(add){
						//need to add point to candidats list and sort array
						if(NCandidats==0){
							CandidatList[0]=pi;
							CandidatPointWeight[pi]=wi;
							CandidatPointPrevIndex[pi]=TZ;
							NCandidats++;
						}else{
							int IDXmax=0;
							int IDXmin=NCandidats-1;
							int wcmax=CandidatPointWeight[CandidatList[IDXmax]];
							int wcmin=CandidatPointWeight[CandidatList[IDXmin]];
							if(wi<=wcmin){
								CandidatList[NCandidats]=pi;
								CandidatPointWeight[pi]=wi;
								CandidatPointPrevIndex[pi]=TZ;
								NCandidats++;
							}else
								if(wi>wcmax){
									memmove(CandidatList+1,CandidatList,NCandidats<<1);
									CandidatList[0]=pi;
									CandidatPointWeight[pi]=wi;
									CandidatPointPrevIndex[pi]=TZ;
									NCandidats++;
								}else{
									while(IDXmax!=IDXmin-1){
										int IDXmid=(IDXmin+IDXmax)>>1;
										int wm=CandidatPointWeight[CandidatList[IDXmid]];
										if(wm>wi){
											wcmax=wm;
											IDXmax=IDXmid;
										}else{
											wcmin=wm;
											IDXmin=IDXmid;
										};
									};
									memmove(CandidatList+IDXmin+1,CandidatList+IDXmin,(NCandidats-IDXmin+1)<<1);
									CandidatList[IDXmin]=pi;
									CandidatPointWeight[pi]=wi;
									CandidatPointPrevIndex[pi]=TZ;
									NCandidats++;
								};
						};
					};
				};
			};
		}else return 0xFFFF;
	}while(NCandidats);
	return 0xFFFF;
};
DLLEXPORT
int FindNextZoneOnTheSafeWayToLimitedObject(int Start,short* DangerMap,word* ObjIds,word* Types,word ReqType,int* MaxDanger,int DangSteps,word* DestObj){
	TIME1=GetTickCount();
	//if(GetZonesDist(Start,Fin)>=0xFFFF)return 0xFFFF;
	int NAR=GetNAreas();
	if(Start<0||Start>=NAR)return 0xFFFF;
	word PrecisePointWeight    [MXZ];
	word PrecisePointPrevIndex [MXZ];
	word CandidatPointWeight   [MXZ];
	word CandidatPointPrevIndex[MXZ];
	word CandidatList          [MXZ];	
	if(NAR>8192)NAR=8192;
	memset(PrecisePointWeight,0xFF,NAR<<1);
	memset(CandidatPointWeight,0xFF,NAR<<1);
	CandidatList[0]=Start;
	CandidatPointWeight[Start]=0;
	CandidatPointPrevIndex[Start]=0xFFFF;
	int NCandidats=1;

	addrand(Start);

	do{
		//best candidat is a last candidat in sorted list
		NCandidats--;
		word TZ=CandidatList[NCandidats];
		PrecisePointWeight[TZ]=CandidatPointWeight[TZ];
		CandidatPointWeight[TZ]=0xFFFF;
		PrecisePointPrevIndex[TZ]=CandidatPointPrevIndex[TZ];
		if(ObjIds[TZ]!=0xFFFF&&Types[TZ]==ReqType){
			//it finally happen!!!
			int MaxDang=DangerMap[TZ];
			int T0=TZ;
			FULLWAY[0]=TZ;
			NWPTS=1;
			for(int q=0;T0!=0xFFFF && NWPTS<128;q++){
				T0=CandidatPointPrevIndex[T0];
				if(T0!=0xFFFF){
					if(NWPTS<128){
						FULLWAY[NWPTS]=T0;
						NWPTS++;
						addrand(NWPTS);
					};
					int w=DangerMap[T0];
					if(w>MaxDang)MaxDang=w;
				};
			};
			*MaxDanger=MaxDang;
			*DestObj=ObjIds[TZ];
			//TIME1=GetTickCount()-TIME1;
			if(NWPTS>1)return FULLWAY[NWPTS-2];
			else return FULLWAY[NWPTS-1];
		};
		Area* TAR=GetTopMap(TZ);
		int NL=TAR->NLinks;
		OneLinkInfo* LINK=TAR->Link;
		int w0=PrecisePointWeight[TZ];
		for(int i=0;i<NL;i++){
			int pi=LINK[i].NextAreaID;
			//adding point to the candidats list
			//1.checking if point is in precise list
			if(PrecisePointWeight[pi]==0xFFFF){
				int wi=LINK[i].NextAreaDist+w0+DangerMap[pi];
				//2.checking if point is already in candidats list
				int wc;
				bool add=1;
				if((wc=CandidatPointWeight[pi])!=0xFFFF){
					if(wc>wi){
						//new point is better. Need to delete old point
						for(int j=0;j<NCandidats;j++)if(CandidatList[j]==pi){
							if(j<NCandidats-1)memcpy(CandidatList+j,CandidatList+j+1,(NCandidats-j-1)<<1);
							j=NCandidats;
							NCandidats--;
						};
						CandidatPointWeight[pi]=wi;
					}else{
						add=0;
					};
				};
				if(add){
					//need to add point to candidats list and sort array
					if(NCandidats==0){
						CandidatList[0]=pi;
						CandidatPointWeight[pi]=wi;
						CandidatPointPrevIndex[pi]=TZ;
						NCandidats++;
					}else{
						int IDXmax=0;
						int IDXmin=NCandidats-1;
						int wcmax=CandidatPointWeight[CandidatList[IDXmax]];
						int wcmin=CandidatPointWeight[CandidatList[IDXmin]];
						if(wi<=wcmin){
							CandidatList[NCandidats]=pi;
							CandidatPointWeight[pi]=wi;
							CandidatPointPrevIndex[pi]=TZ;
							NCandidats++;
						}else
						if(wi>wcmax){
							memmove(CandidatList+1,CandidatList,NCandidats<<1);
							CandidatList[0]=pi;
							CandidatPointWeight[pi]=wi;
							CandidatPointPrevIndex[pi]=TZ;
							NCandidats++;
						}else{
							while(IDXmax!=IDXmin-1){
								int IDXmid=(IDXmin+IDXmax)>>1;
								int wm=CandidatPointWeight[CandidatList[IDXmid]];
								if(wm>wi){
									wcmax=wm;
									IDXmax=IDXmid;
								}else{
									wcmin=wm;
									IDXmin=IDXmid;
								};
							};
							memmove(CandidatList+IDXmin+1,CandidatList+IDXmin,(NCandidats-IDXmin+1)<<1);
							CandidatList[IDXmin]=pi;
							CandidatPointWeight[pi]=wi;
							CandidatPointPrevIndex[pi]=TZ;
							NCandidats++;
						};
					};
				};
			};
		};
	}while(NCandidats);
	return 0xFFFF;
};
DLLEXPORT
void CreateNonFiredEnemyBuildingsTopList(word* IDS,byte NI){
	byte NMask=NATIONS[NI].NMask;
	int NA=GetNAreas();
	memset(IDS,0xFF,NA*2);
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&OB->NewBuilding&&(!(OB->newMons->Immortal||OB->Sdoxlo||OB->InFire||OB->NMask&NMask||OB->NNUM==7))){
			int top=GetTopZone(OB->RealX>>4,OB->RealY>>4);
			if(top<NA)IDS[top]=i;
		};
	};
};

DLLEXPORT
void CreateTopListForStorm(word* IDS,byte NI,word StormerNIndex=0xFFFF){
	bool high=false;
	if(StormerNIndex<NATIONS->NMon) high=NATIONS->Mon[StormerNIndex]->newMons->HighUnit;
	byte NMask=NATIONS[NI].NMask;
	int NA=GetNAreas();
	memset(IDS,0xFF,NA*2);
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&OB->NewBuilding&&(!(OB->Sdoxlo||OB->NMask&NMask||OB->NNUM==7))&&OB->newMons->UnitAbsorber&&OB->Stage==OB->MoreCharacter->ProduceStages&&!(high&&OB->newMons->HighUnitCantEnter)){
			int top=GetTopZone(OB->RealX>>4,OB->RealY>>4);
			if(top<NA)IDS[top]=i;
		};
	};
};
DLLEXPORT bool GetBuildingOposit(word Index, int& N);
DLLEXPORT
void CreateTopListForStormHard(word* IDS,byte NI,int NMax,word StormerNIndex=0xFFFF){
	if(StormerNIndex==0xFFFF)return;
	bool high=false;
	if(StormerNIndex<NATIONS->NMon) high=NATIONS->Mon[StormerNIndex]->newMons->HighUnit;
	byte NMask=NATIONS[NI].NMask;
	word NPS[4096];
	int NA=GetNAreas();
	memset(NPS,0,NA<<1);
	memset(IDS,0xFF,NA<<1);
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&OB->NewBuilding&&(!(OB->Sdoxlo||OB->NMask&NMask||OB->NNUM==7))&&OB->newMons->UnitAbsorber&&OB->Stage==OB->MoreCharacter->ProduceStages&&!(high&&OB->newMons->HighUnitCantEnter)){
			int top=GetTopZone(OB->RealX>>4,OB->RealY>>4);
			int NMen=0;			
			GetBuildingOposit(OB->Index,NMen);
			if(top<NA&&NMen<NMax&&(IDS[top]==0xFFFF||NMen<NPS[top])){
				IDS[top]=i;				
				NPS[top]=NMen;
			}
		};
	};
};
DLLEXPORT
void CreateFriendBuildingsTopList(word* IDS,byte NI){
	byte NMask=NATIONS[NI].NMask;
	int NA=GetNAreas();
	memset(IDS,0xFF,NA*2);
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&OB->NewBuilding&&(!(OB->Sdoxlo||OB->NNUM==7)||NI==7)&&OB->NMask&NMask){
			int top=GetTopZone(OB->RealX>>4,OB->RealY>>4);
			if(top<NA)IDS[top]=i;
		};
	};
};
CEXPORT
void CreateSkladTopList(word* IDS,byte NI,byte ResID){
	byte NMask=NATIONS[NI].NMask;

	int NA=GetNAreas();
	memset(IDS,0xFF,NA*2);

	word* IDX=NatList[NI];
	int N=NtNUnits[NI];
	word ms=1<<ResID;
	if(ms){
		for(int i=0;i<N;i++){
			word MID=IDX[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->NewBuilding&&OB->Ready){
					NewMonster* NM=OB->newMons;
					if((NM->ResConcentrator&ms)||(OB->AbRes&ms)){
						int top=GetTopZone(OB->RealX>>4,OB->RealY>>4);
						if(top<NA)IDS[top]=MID;
					};
				};
			};
		};
	};
	/*
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&OB->NewBuilding&&(!(OB->Sdoxlo||OB->NNUM==7)||NI==7)&&OB->NMask&NMask){
			int top=GetTopZone(OB->RealX>>4,OB->RealY>>4);
			if(top<NA)IDS[top]=i;
		};
	};
	*/
};
CEXPORT
word FindNearestSklad(byte NI, byte ResID, int x, int y){
	byte NMask=NATIONS[NI].NMask;

	word SID=0xFFFF;
	int MinR=10000000;

	word ms=1<<ResID;
	if(ms){
		word* IDX=NatList[NI];
		int N=NtNUnits[NI];
		for(int i=0;i<N;i++){
			word MID=IDX[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->NewBuilding&&OB->Ready){
					NewMonster* NM=OB->newMons;
					if((NM->ResConcentrator&ms)||(OB->AbRes&ms)){
						int R=GetTopDist(x,y,OB->RealX>>4,OB->RealY>>4);
						if(SID==0xFFFF||R<MinR){
							SID=OB->Index;
							MinR=R;
						}
					};
				};
			};
		};
	};
	return SID;
};
CEXPORT
word FindNearestSklad(byte NI, byte ResID){
	byte NMask=NATIONS[NI].NMask;
	
	word ms=1<<ResID;
	if(ms){
		word* IDX=NatList[NI];
		int N=NtNUnits[NI];
		for(int i=0;i<N;i++){
			word MID=IDX[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->NewBuilding&&OB->Ready){
					NewMonster* NM=OB->newMons;
					if((NM->ResConcentrator&ms)||(OB->AbRes&ms)) return OB->Index;
				};
			};
		};
	};
	return 0xFFFF;
};

DLLEXPORT
bool GetBuildingOposit(word Index, int& N){
	if(Index>=0&&Index<MAXOBJECT){
		OneObject* OB=Group[Index];
		if(OB&&!OB->Sdoxlo&&OB->NewBuilding){
			int min=OB->newMons->MinOposit;
			int max=OB->newMons->MaxOposit;
			int ins=OB->NInside;
			int mins=OB->MoreCharacter->MaxInside;
			if(!mins){
				N=0;
				return false;
			};
			int Op=min+(max-min)*ins/mins;
			if(OB->newMons->MinDamage<OB->newMons->MaxDamage) Op<<=1;
			N=ins*Op;
			return true;
		}
	}
	N=0;
	return false;
}

uaa_Chest* DetectChest(OneObject* OB);
bool isNotInvisible(OneObject* OB, byte SeekerNMask);
bool GetHero(OneObject* OB);

DLLEXPORT void CreateTopListEnArmy(word* IDS,byte NI,int MinPS){
	word NPS[4096];
	int NA=GetNAreas();
	memset(NPS,0,NA<<1);
	memset(IDS,0xFF,NA<<1);
	byte Mask=NATIONS[NI].NMask;
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if( OB&&!((OB->NMask&Mask)||OB->Sdoxlo||OB->NewBuilding||OB->NNUM==7) ){	//OB->newMons->MeatTransformIndex==0xFF

			if( !DetectChest(OB) && isNotInvisible(OB,Mask) && !OB->newMons->Immortal ){
				int top=GetTopZone(OB->RealX>>4,OB->RealY>>4);
				if(top<NA){
					int NIn;
					if(GetHero(OB)){
						NPS[top]+=MinPS;
					}else{
						NPS[top]++;
					}
					IDS[top]=i;
				};
			}

		};
	};
	for(int i=0;i<NA;i++)if(NPS[i]<MinPS){
		IDS[i]=0xFFFF;
	}
};
DLLEXPORT
bool CreatePeasantsTopList(word* IDS,byte NI,int MinPS, bool SeakMine=true){
	bool find=false;
	word NPS[4096];
	int NA=GetNAreas();
	memset(NPS,0,NA<<1);
	memset(IDS,0xFF,NA<<1);
	byte Mask=NATIONS[NI].NMask;
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&(!((OB->NMask&Mask)||OB->Sdoxlo))){
			int n=0;
			if(OB->newMons->Peasant) n=1;
			if(SeakMine&&NI!=7&&OB->NewBuilding && OB->newMons->Usage==MineID) n=OB->NInside<<1;
			if(n){
				int top=GetTopZone(OB->RealX>>4,OB->RealY>>4);
				if(top<NA){
					NPS[top]+=n;
					IDS[top]=i;
					find=true;
				};
			}
		};
	};
	for(int i=0;i<NA;i++)if(NPS[i]<MinPS)IDS[i]=0xFFFF;
	return find;
};
DLLEXPORT
void CreateMeatList(word* IDS, byte NI, int MaxOB){
	word NOB[4096];
	int NA=GetNAreas();
	memset(NOB,0,NA<<1);
	memset(IDS,0xFF,NA<<1);
	byte Mask=NATIONS[NI].NMask;
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&(!((OB->NMask&Mask)||OB->Sdoxlo))&&OB->newMons->MeatTransformIndex!=0xFF){
			int top=GetTopZone(OB->RealX>>4,OB->RealY>>4);
			if(top<NA){
				NOB[top]++;
				IDS[top]=i;
			};
		};
	};
	for(int i=0;i<NA;i++)if(NOB[i]>MaxOB) IDS[i]=0xFFFF;
}
DLLEXPORT
void CreateFarmList(word* IDS, byte NI){
	//word NOB[4096];
	int NA=GetNAreas();
	memset(IDS,0xFF,NA<<1);
	byte Mask=NATIONS[NI].NMask;
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB && OB->NNUM==NI && !OB->Sdoxlo && OB->NewBuilding && (OB->newMons->ResConcentrator&(1<<8))){
			int top=GetTopZone(OB->RealX>>4,OB->RealY>>4);
			if(top<NA){
				IDS[top]=i;
			};
		};
	};
}
DLLEXPORT
bool CheckIfBuildingIsFired(word Index){
	OneObject* OB=Group[Index];
	if(OB&&!OB->InFire)return false;
	return true;
};
DLLEXPORT int GetLastFullWay(word** buf){
	*buf=FULLWAY;
	return NWPTS;
};
//
DLLEXPORT void SetDangerMap(int* Map);
void SmoothDangerMap(short* DangMap, int ItrAmount);
//
DLLEXPORT void CreateDangerMap(byte NI,short* DangMap,int MaxSize,int* FearArray,int ItrAmount){
	byte MASK=NATIONS[NI].NMask;
	memset(DangMap,0,MaxSize<<1);
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&(!OB->Sdoxlo)){							
			int z=GetTopZone(OB->RealX>>4,OB->RealY>>4);
			if(z<MaxSize){
				int DD=int(DangMap[z]);
				if(!(OB->NMask&MASK)){
					// enemy
					if(OB->NNUM==7&&OB->NewBuilding){
						DD+=FearArray[OB->newMons->Category]<<7;
					}else
					if(OB->newMons->ResConcentrator){
						DD-=FearArray[OB->newMons->Category]<<8;
					}else
					if(OB->NInside){
						if(OB->newMons->Usage==MineID){
							DD-=FearArray[OB->newMons->Category]*OB->NInside<<4;
						}else						
							DD+=FearArray[OB->newMons->Category]*(OB->NInside+OB->Kills);
					}else						
					if(OB->newMons->Peasant){						
						DD-=FearArray[OB->newMons->Category]<<1;
					}else
						DD+=FearArray[OB->newMons->Category];
				}else{
					// alies
					DD-=FearArray[OB->newMons->Category];
				}
				DangMap[z]=DD;
			};			
		};
	};
	//
	SmoothDangerMap(DangMap, ItrAmount);
	//
	/*
	for(int p=0;p<ItrAmount;p++){		
		for(int j=0;j<MaxSize;j++){
			Area* AR=GetTopMap(j);
			int NL=AR->NLinks;
			int dang=DangMap[j]/(ItrAmount<<1);
			for(int p=0;p<NL;p++){
				int id=AR->Link[p].NextAreaID;
				if(id<MaxSize){
					int DD=int(DangMap[id])+dang;
					DangMap[id]=DD;
				};
			};
		};
	};
	*/
	for(int j=0;j<MaxSize;j++){
		int DD=DangMap[j];
		if(DD>30000) DD=30000;
			else if(DD<0) DD=0;
		DangMap[j]=DD;
	}
	//int Dang[8192];
	//for(i=0;i<MaxSize;i++) Dang[i]=DangMap[i];
	//SetDangerMap(Dang);
};
DLLEXPORT
void CreateDangerMapForDef(byte NI,short* DangMap,int MaxSize,int* FearArray,int ItrAmount){
	byte MASK=NATIONS[NI].NMask;
	memset(DangMap,0,MaxSize<<1);
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&!(OB->Sdoxlo||OB->NewBuilding)){							
			int z=GetTopZone(OB->RealX>>4,OB->RealY>>4);
			if(z<MaxSize){
				int DD=int(DangMap[z]);
				if(!(OB->NMask&MASK)){
					DD+=(OB->newMons->StormForce>>5)|1;
				}
				DangMap[z]=DD;
			};			
		};
	};
	//
	SmoothDangerMap(DangMap, ItrAmount);
	//
	/*	
	for(int p=0;p<ItrAmount;p++){		
		for(int j=0;j<MaxSize;j++){
			Area* AR=GetTopMap(j);
			int NL=AR->NLinks;
			int dang=DangMap[j]/(ItrAmount<<1);
			for(int p=0;p<NL;p++){
				int id=AR->Link[p].NextAreaID;
				if(id<MaxSize){
					int DD=int(DangMap[id])+dang;
					DangMap[id]=DD;
				};
			};
		};
	};
	*/
	//for(int j=0;j<MaxSize;j++){
	//	DangMap[j]>>=3;
	//}
	//int Dang[8192];
	//for(i=0;i<MaxSize;i++) Dang[i]=DangMap[i];
	//SetDangerMap(Dang);
};
DLLEXPORT
void CreateDangerMapForTom(byte NI,short* DangMap,int MaxSize,int* FearArray,int ItrAmount){
	byte MASK=NATIONS[NI].NMask;
	memset(DangMap,0,MaxSize<<1);
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&(!OB->Sdoxlo)){							
			int z=GetTopZone(OB->RealX>>4,OB->RealY>>4);
			if(z<MaxSize){
				int DD=int(DangMap[z]);
				if(!(OB->NMask&MASK)){
					// enemy
					if(OB->NNUM==7&&OB->NewBuilding){
						DD+=FearArray[OB->newMons->Category]<<5;
					}else
					if(OB->newMons->ResConcentrator){
						DD-=FearArray[OB->newMons->Category]<<3;
					}else
					if(OB->NInside){
						if(OB->newMons->Usage==MineID){
							DD-=FearArray[OB->newMons->Category]*OB->NInside<<2;
						}else{
							DD+=FearArray[OB->newMons->Category]*(OB->NInside+OB->Kills)<<3;
						}
					}else						
					if(OB->newMons->Peasant){						
						DD-=FearArray[OB->newMons->Category];
					}else
						DD+=FearArray[OB->newMons->Category];
				}else{
					// alies
					DD-=FearArray[OB->newMons->Category];
				}
				DangMap[z]=DD;
			};			
		};
	};
	//
	SmoothDangerMap(DangMap, ItrAmount);
	//
	/*
	for(int p=0;p<ItrAmount;p++){		
		for(int j=0;j<MaxSize;j++){
			Area* AR=GetTopMap(j);
			int NL=AR->NLinks;
			int dang=DangMap[j]/(ItrAmount<<1);
			for(int p=0;p<NL;p++){
				int id=AR->Link[p].NextAreaID;
				if(id<MaxSize){
					int DD=int(DangMap[id])+dang;
					DangMap[id]=DD;
				};
			};
		};
	};
	*/
	for(int j=0;j<MaxSize;j++){
		int DD=DangMap[j];
		if(DD>30000) DD=30000;
			else if(DD<0) DD=0;
		DangMap[j]=DD;
	}
	
};
void SmoothDangerMap(short* DangMap, int ItrAmount){
	static short DM[4096];
	int MaxSize=GetNAreas();
	for(int p=0;p<ItrAmount;p++){
		memset(DM,0,MaxSize<<1);		
		for(int j=0;j<MaxSize;j++){
			Area* AR=GetTopMap(j);
			int NL=AR->NLinks;
			int dang=DangMap[j]/(ItrAmount<<1);
			for(int p=0;p<NL;p++){
				int id=AR->Link[p].NextAreaID;
				if(id<MaxSize){					
					DM[id]+=dang;
				};
			};
		};
		for(int j=0;j<MaxSize;j++){
			DangMap[j]+=DM[j];
		}
	};
}

bool CheckIfPossibleToAttackUnit(OneObject* Attacker,OneObject* Victim);

DLLEXPORT void CreateDangerMapForStorm(byte NI,short* DangMap,int MaxSize,int* FearArray,int ItrAmount){
	byte MASK=NATIONS[NI].NMask;
	memset(DangMap,0,MaxSize<<1);
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&(!OB->Sdoxlo)){							
			int z=GetTopZone(OB->RealX>>4,OB->RealY>>4);
			if(z<MaxSize){
				int DD=int(DangMap[z]);
				if(!(OB->NMask&MASK)){
					// enemy
					if(OB->NewBuilding){
						AdvCharacter* ADC=OB->MoreCharacter;
						if(ADC->MaxDam){
							int Fear=FearArray[OB->newMons->Category]*36;
							DD+=Fear;
							Area* AR=GetTopMap(z);
							int NL=AR->NLinks;							
							for(int p=0;p<NL;p++){
								int zl=AR->Link[p].NextAreaID;
								if(zl<MaxSize){
									DangMap[zl]+=Fear;
								}
							}
						}else{
							//DD-=FearArray[OB->newMons->Category]*4;
						}
					}else
					if(OB->newMons->ResConcentrator){
						DD-=FearArray[OB->newMons->Category]*16;
					}else
					if(OB->newMons->Peasant){						
						//DD=FearArray[OB->newMons->Category];
					}else
						DD+=FearArray[OB->newMons->Category];
				}else{
					// alies
					//DD-=FearArray[OB->newMons->Category];
				}
				DangMap[z]=DD;
			};			
		};
	};
	SmoothDangerMap(DangMap, ItrAmount);
	for(int j=0;j<MaxSize;j++){
		int DD=DangMap[j];
		if(DD>30000) DD=30000;
			else if(DD<0) DD=0;
		DangMap[j]=DD;
	}
};
// не притягиваться к рабочим
DLLEXPORT void CreateDangerMapForStormHard(byte NI,short* DangMap,int MaxSize,int* FearArray,int ItrAmount){
	byte MASK=NATIONS[NI].NMask;
	memset(DangMap,0,MaxSize<<1);
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&(!OB->Sdoxlo)){							
			int z=GetTopZone(OB->RealX>>4,OB->RealY>>4);
			if(z<MaxSize){
				int DD=int(DangMap[z]);
				if(!(OB->NMask&MASK)){
					// enemy
					if(OB->NewBuilding){
						AdvCharacter* ADC=OB->MoreCharacter;
						if(ADC->MaxDam){
							int Fear=FearArray[OB->newMons->Category]*36;
							DD+=Fear;
							Area* AR=GetTopMap(z);
							int NL=AR->NLinks;							
							for(int p=0;p<NL;p++){
								int zl=AR->Link[p].NextAreaID;
								if(zl<MaxSize){
									DangMap[zl]+=Fear;
								}
							}
						}else{
							//DD-=FearArray[OB->newMons->Category]*4;
						}
					}else
					if(OB->newMons->ResConcentrator){
						DD-=FearArray[OB->newMons->Category]*16;
					}else
					if(OB->newMons->Peasant){						
						//DD=FearArray[OB->newMons->Category];
					}else
						DD+=FearArray[OB->newMons->Category];
				}else{
					// alies
					//DD-=FearArray[OB->newMons->Category];
				}
				DangMap[z]=DD;
			};			
		};
	};
	SmoothDangerMap(DangMap, ItrAmount);
	for(int j=0;j<MaxSize;j++){
		int DD=DangMap[j];
		if(DD>30000) DD=30000;
			else if(DD<0) DD=0;
		DangMap[j]=DD;
	}
};
//
CEXPORT void CreateDangerMapForHealer(byte NI,short* DangMap,int MaxSize,int ItrAmount){
	byte MASK=NATIONS[NI].NMask;
	memset(DangMap,0,MaxSize<<1);
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&(!OB->Sdoxlo)){							
			int z=GetTopZone(OB->RealX>>4,OB->RealY>>4);
			if(z<MaxSize){
				int DD=int(DangMap[z]);
				AdvCharacter* Adv=OB->MoreCharacter;
				if(!(OB->NMask&MASK)){
					// enemy
					DD+=Adv->MaxDam;
					/*
					if(OB->NNUM==7&&OB->NewBuilding){
						DD+=FearArray[OB->newMons->Category]<<8;
					}else
						if(OB->newMons->ResConcentrator){
							DD-=FearArray[OB->newMons->Category]<<8;
						}else
						if(OB->NInside){
							if(OB->newMons->Usage==MineID){
								DD-=FearArray[OB->newMons->Category]*OB->NInside<<2;
							}else						
								DD+=FearArray[OB->newMons->Category]*(OB->NInside+OB->Kills-(OB->MaxLife>>10));
						}else						
						if(OB->newMons->Peasant){						
							DD-=FearArray[OB->newMons->Category]<<1;
						}else
							DD+=FearArray[OB->newMons->Category];
					*/
				}else{
					// alies
					//DD-=FearArray[OB->newMons->Category];
				}
				DangMap[z]=DD;
			};			
		};
	};
	//
	SmoothDangerMap(DangMap, ItrAmount);
	//
	/*
	for(int p=0;p<ItrAmount;p++){		
		for(int j=0;j<MaxSize;j++){
			Area* AR=GetTopMap(j);
			int NL=AR->NLinks;
			int dang=DangMap[j]/(ItrAmount<<1);
			for(int p=0;p<NL;p++){
				int id=AR->Link[p].NextAreaID;
				if(id<MaxSize){
					int DD=int(DangMap[id])+dang;
					DangMap[id]=DD;
				};
			};
		};
	};
	*/
}
//
DLLEXPORT
void CreateDangerMapForFire(byte NI,short* DangMap,int MaxSize,int* FearArray,int ItrAmount){
	byte MASK=NATIONS[NI].NMask;
	memset(DangMap,0,MaxSize<<1);
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&(!OB->Sdoxlo)){							
			int z=GetTopZone(OB->RealX>>4,OB->RealY>>4);
			if(z<MaxSize){
				int DD=int(DangMap[z]);
				if(!(OB->NMask&MASK)){
					// enemy
					if(OB->NewBuilding){
						AdvCharacter* ADC=OB->MoreCharacter;
						if(ADC->MaxDam){
							int Fear=FearArray[OB->newMons->Category]*8;
							DD+=Fear;
							Area* AR=GetTopMap(z);
							int NL=AR->NLinks;							
							for(int p=0;p<NL;p++){
								int zl=AR->Link[p].NextAreaID;
								if(zl<MaxSize){
									DangMap[zl]+=Fear;
								}
							}
						}else{
							DD-=FearArray[OB->newMons->Category]*4;
						}
					}else
					if(OB->newMons->ResConcentrator){
						DD-=FearArray[OB->newMons->Category]*32;
					}else
					if(OB->newMons->Peasant){						
						//DD-=FearArray[OB->newMons->Category];
					}else
						DD+=FearArray[OB->newMons->Category];
				}else{
					// alies
					//DD-=FearArray[OB->newMons->Category];
				}
				DangMap[z]=DD;
			};			
		};
	};
	//
	SmoothDangerMap(DangMap, ItrAmount);
	//
	/*
	for(int p=0;p<ItrAmount;p++){		
		for(int j=0;j<MaxSize;j++){
			Area* AR=GetTopMap(j);
			int NL=AR->NLinks;
			int dang=DangMap[j]/(ItrAmount<<1);
			for(int p=0;p<NL;p++){
				int id=AR->Link[p].NextAreaID;
				if(id<MaxSize){
					int DD=int(DangMap[id])+dang;
					DangMap[id]=DD;
				};
			};
		};
	};
	*/
	for(int j=0;j<MaxSize;j++){
		int DD=DangMap[j];
		if(DD>30000) DD=30000;
			else if(DD<0) DD=0;
		DangMap[j]=DD;
	}
	//int Dang[8192];
	//for(i=0;i<MaxSize;i++) Dang[i]=DangMap[i];
	//SetDangerMap(Dang);
};
DLLEXPORT
void CreateDangerMapWithoutPeasants(byte NI,short* DangMap,int MaxSize,int* FearArray,int ItrAmount){
	byte MASK=NATIONS[NI].NMask;
	memset(DangMap,0,MaxSize<<1);
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&(!OB->Sdoxlo)){							
			int z=GetTopZone(OB->RealX>>4,OB->RealY>>4);
			if(z<MaxSize){
				int DD=int(DangMap[z]);
				if(!(OB->NMask&MASK)){
					// enemy
					if(OB->NewBuilding){
						AdvCharacter* ADC=OB->MoreCharacter;
						if(ADC->MaxDam){
							int Fear=FearArray[OB->newMons->Category]*16;
							DD+=Fear;
							Area* AR=GetTopMap(z);
							int NL=AR->NLinks;							
							for(int p=0;p<NL;p++){
								int zl=AR->Link[p].NextAreaID;
								if(zl<MaxSize){
									DangMap[zl]+=Fear;
								}
							}
						}else{
							//DD-=FearArray[OB->newMons->Category]*4;
						}
					}else
					if(OB->newMons->ResConcentrator){
						DD-=FearArray[OB->newMons->Category]*32;
					}else
					if(OB->newMons->Peasant){						
						//DD=FearArray[OB->newMons->Category];
					}else
						DD+=FearArray[OB->newMons->Category];
				}else{
					// alies
					//DD-=FearArray[OB->newMons->Category];
				}
				DangMap[z]=DD;
			};			
		};
	};
	//
	SmoothDangerMap(DangMap, ItrAmount);
	for(int j=0;j<MaxSize;j++){
		int DD=DangMap[j];
		if(DD>30000) DD=30000;
			else if(DD<0) DD=0;
		DangMap[j]=DD;
	}
};
//
void xLine(int x,int y,int x1,int y1,byte c);
extern int TOPTYPE;
CIMPORT int GetXOnMiniMapEx(int x,int y);
CIMPORT int GetYOnMiniMapEx(int x,int y);
//
void DrawLastWayOnMinimap(){
	for(int i=1;i<NWPTS;i++){
		int top0=FULLWAY[i-1];
		Area* AR0=GetTopMap(top0,TOPTYPE);
		int top=FULLWAY[i];
		Area* AR=GetTopMap(top,TOPTYPE);
		//xLine(minix+AR0->x,miniy+AR0->y,minix+AR->x,miniy+AR->y,0xFF);
		int x0=GetXOnMiniMapEx(AR0->x,AR0->y);
		int y0=GetYOnMiniMapEx(AR0->x,AR0->y);
		int x1=GetXOnMiniMapEx(AR->x,AR->y);
		int y1=GetYOnMiniMapEx(AR->x,AR->y);
		xLine(x0,y0,x1,y1,0xFF);
	};
};

// рисует дороги на миникарте
int GetNRoads();
void DrawRoadsOnMinimap(){
	//return;
	int n=GetNRoads();
	int na=GetNAreas();
	if(!na) return;
	for(int i=0;i<n;i++){
		Area* AR=GetTopMap(i);
		int nl=AR->NLinks;
		for(int l=0;l<nl;l++){
			word ltop=AR->Link[l].NextAreaID;
			if(ltop<n&&i<ltop){
				Area* AR0=GetTopMap(ltop);
				GPS.DrawLine(minix+AR0->x,miniy+AR0->y,minix+AR->x,miniy+AR->y,0xFFA0A000);
			}
		}
	}
}
// рисует путь для выделенных юнитов
int NSelWay=0;
word SelWay[8192];
byte SelLockType=0;
void DrawSelWayOnMinimap(){
	if(SelLockType>3||!GSets.ShowWayOnRoad)return;
    int NA=GetNAreas(SelLockType);
	for(int i=1;i<NSelWay;i++){
		int top0=SelWay[i-1];
		if(top0>=NA)break;
		Area* AR0=GetTopMap(top0,SelLockType);
		int top=SelWay[i];
		if(top>=NA)break;
		Area* AR=GetTopMap(top,SelLockType);
		if(AR&&AR0){
			//xLine(minix+AR0->x,miniy+AR0->y,minix+AR->x,miniy+AR->y,0xFF);
			int x0=GetXOnMiniMapEx(AR0->x<<6,AR0->y<<6);
			int y0=GetYOnMiniMapEx(AR0->x<<6,AR0->y<<6); 
			int x1=GetXOnMiniMapEx(AR->x<<6,AR->y<<6);
			int y1=GetYOnMiniMapEx(AR->x<<6,AR->y<<6);
			GPS.DrawLine(x0,y0,x1,y1,0xFFFFFF00);
		}		
	};
}
void CreateSelWay(word t0, word t1, byte LockType=0,byte NI=0){
	SelLockType=LockType;
	NSelWay=0;
	if(t0!=0xFFFF&&t1!=0xFFFF&&t1!=t0){
		int NA=GetNAreas(LockType);
		do{
			SelWay[NSelWay]=t0;
			t0=GetMotionLinks(t1+t0*NA,LockType,NI);
			NSelWay++;
		}while(t1!=t0&&NSelWay<600);
	}
}

int TestCapture(OneObject* OBJ);
DLLEXPORT
bool CheckMineCaptureAbility(int Index){
	if(Index>=0&&Index<0xFFFE){
		OneObject* OB=Group[Index];
		if(OB&&!OB->Sdoxlo)return TestCapture(OB)!=0;
	};
	return false;
};
DLLEXPORT
void DeleteUnit(int Index){
	if(Index>=0&&Index<0xFFFE){
		OneObject* OB=Group[Index];
		if(OB)OB->Die();
	};
};
bool DetectShootingUnit(GeneralObject* GO);

CEXPORT bool GroupIsUnderAttack(GAMEOBJ* Grp){
	if(Grp->Type=='UNIT' && Grp->Index<SCENINF.UGRP.GetAmount()){
		UnitsGroup* UG=SCENINF.UGRP+Grp->Index;
		for(int i=0;i<UG->N;i++){
			word MID=UG->IDS[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&(!OB->Sdoxlo)&&OB->Serial==UG->SNS[i]){
					if(OB->Attack){
						return true;
					}
				}
			}
		}
	}
	return false;
}

DLLEXPORT void SetUnitsState(GAMEOBJ* GRP,bool SearchEnemy,bool StandGround,bool ArmAttack,bool FriendlyFire);

int vMinEnemy=0;
CEXPORT void SetMinEnemy(int N){
	vMinEnemy=N;
}

OneObject* SearchBuildingInCell(int cell,byte nmask);
bool CheckIfPossibleToAttackUnit(OneObject* Attacker,OneObject* Victim);
void SetUnitsState(GAMEOBJ* GRP,bool SearchEnemy,bool StandGround,bool ArmAttack,bool FriendlyFire);

DLLEXPORT bool AttackEnemyInZone3(GAMEOBJ* Grp,GAMEOBJ* Zone,byte NI){
	int xc,yc,R;
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		xc=Zone->Index<<4;
		yc=Zone->Serial<<4;
		R=(Zone->Type&0xFFFFFF)<<4;
	}else
	if(Zone->Type!='ZONE'){
		return false;
	}else{
		ActiveZone* AZ=AZones[SCENINF.ZGRP[Zone->Index].ZoneID[0]];
		xc=AZ->x<<4;
		yc=AZ->y<<4;
		R=AZ->R<<4;
	};
	word ENLIST[1600];
	int NEN=0;
	int NEB=0;
	byte MASK=NATIONS[NI].NMask;
	
	bool Hero=false;

	int MinR=100000000;
	word EMID=0xFFFF;

	int X0=xc>>11;
	int Y0=yc>>11;
	int RR=(R>>11)+2;
	int TRMAX=(R>>9)+3;
	for(int i=0;i<RR;i++){
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		int N=Rarr[i].N;
		for(int j=0;j<N;j++){
			int XX=X0+xi[j];
			int YY=Y0+yi[j];
			if(XX>=0&&YY>=0&&XX<VAL_MAXCX-1&&YY<VAL_MAXCX-1){
				int cell=XX+1+((YY+1)<<VAL_SHFCX);
				int NMon=MCount[cell];
				if(NMon){
					int ofs1=cell<<SHFCELL;
					word MID;
					for(int i=0;i<NMon;i++){
						MID=GetNMSL(ofs1+i);
						if(MID!=0xFFFF){
							OneObject* OB=Group[MID];
							if( OB && NEN<1600 && !((OB->NMask&MASK)||OB->Sdoxlo||OB->Hidden||OB->newMons->Immortal||OB->NNUM==7&&OB->AI_Guard) ){
								int R0=Norma(OB->RealX-xc,OB->RealY-yc);
								if(R0<R){
									//int TR=GetTopDistance(OB->RealX>>10,OB->RealY>>10,xc>>10,yc>>10);
									//if(TR<TRMAX){
										ENLIST[NEN]=OB->Index;
										if(!Hero)
											Hero=GetHero(OB);
										NEN++;
									//};
								};
							};
						};
					};
				};
				/*
				OneObject* BO = SearchBuildingInCell(cell,~MASK);
				if(BO&&NEN<1600&&!((BO->NMask&MASK)||BO->Sdoxlo||BO->Hidden||BO->newMons->Immortal||BO->NNUM==7)){
					word BID=BO->Index;
					bool exist=false;
					for(int b=0;b<NEN;b++) if(ENLIST[b]==BID) exist=true;
					if(!exist){
						ENLIST[NEN]=BID;
						NEN++;
						NEB++;
					}
				}
				*/
			};
		};
	};
	
	bool SomethingDone=0;
	UnitsGroup* UG=SCENINF.UGRP+Grp->Index;

	if( NEN+NEB>0 && (Hero||NEN>=vMinEnemy) ){
		SetUnitsState(Grp,1,0,0,0);
	}else{
		SetUnitsState(Grp,0,1,0,0);
		return false;
	}
	
	for(int i=0;i<UG->N;i++){
		word MID=UG->IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(!OB->Sdoxlo)&&OB->Serial==UG->SNS[i]){
				if( !OB->Attack ){
					//need to find appropriate unit;
					//1.Search for unit in the attack area					
					byte kmask=OB->newMons->KillMask;
					bool KBUI=OB->newMons->AttBuild;
					word EMID=0xFFFF;
					int Dist=10000;
					int Type=-1;
					int R=1000000;
					int tx0=OB->RealX>>10;
					int ty0=OB->RealY>>10;
					AdvCharacter* ADC=OB->MoreCharacter;
					for(int j=0;j<NEN;j++){
						OneObject* EOB=Group[ENLIST[j]];
						if((kmask&EOB->MathMask)/*||(EOB->NewBuilding&&OB->newMons->AttBuild)*/){
							//int R=GetTopDistance(EOB->RealX>>10,EOB->RealY>>10,tx0,ty0);
							int Rv=Norma(OB->RealX-EOB->RealX,OB->RealY-EOB->RealY)>>4;							
							if(R>Rv&&(EOB->LockType!=1||ADC->MaxR_Attack>Rv)){ //&&R<Dist
								EMID=EOB->Index;
								//Dist=R;
								//Type=0;
								R=Rv;
							};							
						};
					};
					if(EMID!=0xFFFF){
						/*
						if(OB->BrigadeID!=0xFFFF){
							OB->Nat->CITY->Brigs[OB->BrigadeID].ClearBOrders();
						};
						*/
						if(OB->AttackObj(EMID,128+15,0,0)){
							SomethingDone=true;
						}
					};
				}else{
					//if(OB->EnemyID!=)
					if( rando()<900 && !OB->UnlimitedMotion && (!OB->ActiveAbility||OB->ActiveAbility->AttackCasterID==0xFFFF) ){
						OB->ClearOrders();
					};
					SomethingDone=true;
				};
			};
		};
	};
	return SomethingDone;
};

DLLEXPORT bool AttackEnemyInZone2(GAMEOBJ* Grp,GAMEOBJ* Zone,byte NI){
	if(Grp->Type!='UNIT') return false;
	//return GroupIsUnderAttack(Grp);
	//
	int xc,yc,R;
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		xc=Zone->Index;
		yc=Zone->Serial;
		R=(Zone->Type&0xFFFFFF);
	}else
		if(Zone->Type!='ZONE'){
			return false;
		}else{
			ActiveZone* AZ=AZones[SCENINF.ZGRP[Zone->Index].ZoneID[0]];
			xc=AZ->x;
			yc=AZ->y;
			R=AZ->R;
		};
		UnitsGroup* UG=SCENINF.UGRP+Grp->Index;
		//GroupAttackInZone(UG,NI,xc,yc,R);
		//return AttackEnemyInZone3(Grp,Zone,NI);
		return GroupAttackInZone(UG,NI,xc,yc,R);
}
OneObject* SearchBuildingInCell(int cell,byte nmask);

int GetBuildListInZone(int X, int Y, int R, byte Mask, word* IDS){
	int xc=X;
	int yc=Y;

	int NEN=0;
	byte MASK=Mask;

	int X0=xc>>11;
	int Y0=yc>>11;
	int RR=(R>>11)+2;
	int TRMAX=(R>>9)+3;
	for(int i=0;i<RR;i++){
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		int N=Rarr[i].N;
		for(int j=0;j<N;j++){
			int XX=X0+xi[j];
			int YY=Y0+yi[j];
			if(XX>=0&&YY>=0&&XX<VAL_MAXCX-1&&YY<VAL_MAXCX-1){
				int cell=XX+1+((YY+1)<<VAL_SHFCX);
				OneObject* BO = SearchBuildingInCell(cell,~MASK);
				if(BO){
					word BID=BO->Index;
					bool exist=false;
					for(int b=0;b<NEN;b++) if(IDS[b]==BID) exist=true;
					if(!exist){
						IDS[NEN]=BID;
						NEN++;
					}
				}
			};
		};
	};
	return NEN;
}
int GetFriendUnitsAmount(byte NI,int xc,int yc,int R){	// pixel coordinate
	int NEN=0;
	byte MASK=NATIONS[NI].NMask;
	
	xc<<=4;
	yc<<=4;
	R<<=4;
	int X0=xc>>11;
	int Y0=yc>>11;
	int RR=(R>>11)+2;
	int TRMAX=(R>>9)+3;
	for(int i=0;i<RR;i++){
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		int N=Rarr[i].N;
		for(int j=0;j<N;j++){
			int XX=X0+xi[j];
			int YY=Y0+yi[j];
			if(XX>=0&&YY>=0&&XX<VAL_MAXCX-1&&YY<VAL_MAXCX-1){
				int cell=XX+1+((YY+1)<<VAL_SHFCX);
				int NMon=MCount[cell];
				if(NMon){
					int ofs1=cell<<SHFCELL;
					word MID;
					for(int i=0;i<NMon;i++){
						MID=GetNMSL(ofs1+i);
						if(MID!=0xFFFF){
							OneObject* OB=Group[MID];
							if(OB&&(OB->NMask&MASK)&&!(OB->Sdoxlo)){
								int R0=Norma(OB->RealX-xc,OB->RealY-yc);
								if(R0<R){
									int TR=GetTopDistance(OB->RealX>>10,OB->RealY>>10,xc>>10,yc>>10);
									if(TR<TRMAX){
										NEN++;
									};
								};
							};
						};
					};
				};
			};
		};
	};
	return NEN;
}
int GetEnemyUnitsAmount(byte NI,int xc,int yc,int R){	// pixel coordinate
	int NEN=0;
	byte MASK=NATIONS[NI].NMask;
	
	xc<<=4;
	yc<<=4;
	R<<=4;
	int X0=xc>>11;
	int Y0=yc>>11;
	int RR=(R>>11)+2;
	int TRMAX=(R>>9)+3;
	for(int i=0;i<RR;i++){
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		int N=Rarr[i].N;
		for(int j=0;j<N;j++){
			int XX=X0+xi[j];
			int YY=Y0+yi[j];
			if(XX>=0&&YY>=0&&XX<VAL_MAXCX-1&&YY<VAL_MAXCX-1){
				int cell=XX+1+((YY+1)<<VAL_SHFCX);
				int NMon=MCount[cell];
				if(NMon){
					int ofs1=cell<<SHFCELL;
					word MID;
					for(int i=0;i<NMon;i++){
						MID=GetNMSL(ofs1+i);
						if(MID!=0xFFFF){
							OneObject* OB=Group[MID];
							if(OB&&!(OB->NMask&MASK||OB->Sdoxlo)){
								int R0=Norma(OB->RealX-xc,OB->RealY-yc);
								if(R0<R){
									int TR=GetTopDistance(OB->RealX>>10,OB->RealY>>10,xc>>10,yc>>10);
									if(TR<TRMAX){
										NEN++;
									};
								};
							};
						};
					};
				};
			};
		};
	};
	return NEN;
}
int GetEnemyMaskList(byte MASK,int xc,int yc,int R,word* ENLIST,int MaxEnm){	
	int NEN=0;	
	int MinR=100000000;
	word EMID=0xFFFF;

	int X0=xc>>11;
	int Y0=yc>>11;
	int RR=(R>>11)+2;
	int TRMAX=(R>>9)+3;
	for(int i=0;i<RR;i++){
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		int N=Rarr[i].N;
		for(int j=0;j<N;j++){
			int XX=X0+xi[j];
			int YY=Y0+yi[j];
			if(XX>=0&&YY>=0&&XX<VAL_MAXCX-1&&YY<VAL_MAXCX-1){
				int cell=XX+1+((YY+1)<<VAL_SHFCX);
				int NMon=MCount[cell];
				if(NMon){
					int ofs1=cell<<SHFCELL;
					word MID;
					for(int i=0;i<NMon;i++){
						MID=GetNMSL(ofs1+i);
						if(MID!=0xFFFF){
							OneObject* OB=Group[MID];
							if(OB&&NEN<MaxEnm&&!(OB->NMask&MASK||OB->Sdoxlo)){
								int R0=Norma(OB->RealX-xc,OB->RealY-yc);
								if(R0<R){
									int TR=GetTopDistance(OB->RealX>>10,OB->RealY>>10,xc>>10,yc>>10);
									if(TR<TRMAX){
										ENLIST[NEN]=OB->Index;
										NEN++;
									};
								};
							};
						};
					};
				};
				OneObject* BID = SearchBuildingInCell(cell,0xFF);
				OneObject* FriendBID = SearchBuildingInCell(cell,MASK);
				if(BID&&BID!=FriendBID){
					ENLIST[NEN]=BID->Index;
					NEN++;
				}
			};
		};
	};
	return NEN;
}
CEXPORT
int GetEnemyList(byte NI,int xc,int yc,int R,word* ENLIST,int MaxEnm){
	byte MASK=NATIONS[NI].NMask;
	return GetEnemyMaskList(MASK,xc,yc,R,ENLIST,MaxEnm);
}
DLLEXPORT int GetRND(int Max);
CEXPORT void ProcessDeffenders(GAMEOBJ* Grp, byte Mask, int xc, int yc, int R,int BigR){
	if(Grp->Type!='UNIT'||R<0||BigR<0) return;
	xc<<=4;
	yc<<=4;
	R<<=4;
	BigR<<=4;

	word ENLIST[1600];
	int NEN=GetEnemyMaskList(Mask,xc,yc,BigR,ENLIST,1600);
	//if(!NEN) return;

	UnitsGroup* UG=SCENINF.UGRP+Grp->Index;

	int ND=UG->N;
	word* ID=UG->IDS;
	word* SN=UG->SNS;
	addrand(xc);
	addrand(yc);
	addrand(R);
	addrand(BigR);
	for(int i=0;i<ND;i++){
		if(*ID!=0xFFFF){
			OneObject* OB=Group[*ID];
			if(OB&&!(OB->Sdoxlo||OB->InArmy)&&OB->Serial==*SN){		
				addrand(i);
				OB->NoSearchVictim=1;
				//AdvCharacter* ADC=OB->MoreCharacter;
				int norm=Norma(OB->RealX-xc,OB->RealY-yc);				
				addrand(norm);
				// вернуть домой, патруль
				addrand(int(OB->LocalOrder!=NULL)+1);
				if(!OB->LocalOrder&&GetRND(15)==3||norm>BigR&&GetRND(5)==3){ // (rando()&0xC)==0&&norm>BigR){
					int dx=(rando()&511)-256;
					int dy=(rando()&511)-256;
					int dn=Norma(dx,dy);
					dx+=dx*200/dn;
					dy+=dy*200/dn;
					OB->NewMonsterSmartSendTo((xc>>4)+dx,(yc>>4)+dy,0,0,128+1,0);
				}else
				//  атака агрессора
				if(NEN&&!(OB->LocalOrder||OB->Attack)&&norm<R){
					addrand(177);
					AdvCharacter* ADC=OB->MoreCharacter;				
					int R0=ADC->MinR_Attack;
					int R1=ADC->MaxR_Attack;//+ADC->AttackRadius2[0];

					byte kmask=OB->newMons->KillMask;
					for(int wp=0;wp<NAttTypes;wp++) 
						kmask |= OB->newMons->AttackMask[wp];					
					//bool KBUI=OB->newMons->AttBuild;
					word EMID=0xFFFF;
					//int Dist=10000; // Top distance
					int RNear=10000; // Real distance
					int Type=-1;
					int ox=OB->RealX>>10; // Object Soldier
					int oy=OB->RealY>>10;
					addrand(NEN);
					for(int j=0;j<NEN;j++){
						OneObject* EOB=Group[ENLIST[j]];
						if(kmask&EOB->MathMask){
							//int R=GetTopDistance(EOB->RealX>>10,EOB->RealY>>10,ox,oy);
							int Rv=Norma(OB->RealX-EOB->RealX,OB->RealY-EOB->RealY)>>4;
							if(R0<Rv/*&&Rv<R1&&R<Dist*/){
								EMID=EOB->Index;
								addrand(EMID);
								//Dist=R;
								RNear=Rv;
								Type=0;
							};
						};
					};
					if(EMID!=0xFFFF){
						//if(!OB->LocalOrder){
							OB->RifleAttack=word(ND>9);
							addrand(EMID);
							if(OB->AttackObj(EMID,128+16,0,1)){
								//OB->PrioryLevel = 240;
								//OB->LocalOrder->PrioryLevel = OB->PrioryLevel;
							}
						//}
					}
				}
			}
		}
		ID++;
		SN++;
	}
};

DLLEXPORT
bool Attack3(GAMEOBJ* Grp,GAMEOBJ* Zone,byte NI, int& NArm){
	int xc,yc,R;
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		xc=Zone->Index<<4;
		yc=Zone->Serial<<4;
		R=(Zone->Type&0xFFFFFF)<<4;
	}else
	if(Zone->Type!='ZONE'){
		return false;
	}else{
		ActiveZone* AZ=AZones[SCENINF.ZGRP[Zone->Index].ZoneID[0]];
		xc=AZ->x<<4;
		yc=AZ->y<<4;
		R=AZ->R<<4;
	};
	word ENLIST[1600];
	int NEN=0;
	byte MASK=NATIONS[NI].NMask;
	
	int MinR=100000000;
	word EMID=0xFFFF;

	int X0=xc>>11;
	int Y0=yc>>11;
	int RR=(R>>11)+2;
	int TRMAX=(R>>9)+3;
	for(int i=0;i<RR;i++){
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		int N=Rarr[i].N;
		for(int j=0;j<N;j++){
			int XX=X0+xi[j];
			int YY=Y0+yi[j];
			if(XX>=0&&YY>=0&&XX<VAL_MAXCX-1&&YY<VAL_MAXCX-1){
				int cell=XX+1+((YY+1)<<VAL_SHFCX);
				int NMon=MCount[cell];
				if(NMon){
					int ofs1=cell<<SHFCELL;
					word MID;
					for(int i=0;i<NMon;i++){
						MID=GetNMSL(ofs1+i);
						if(MID!=0xFFFF){
							OneObject* OB=Group[MID];
							if(OB&&NEN<1600&&!(OB->NMask&MASK||OB->Sdoxlo)){
								int R0=Norma(OB->RealX-xc,OB->RealY-yc);
								if(R0<R){
									int TR=GetTopDistance(OB->RealX>>10,OB->RealY>>10,xc>>10,yc>>10);
									if(TR<TRMAX){
										ENLIST[NEN]=OB->Index;
										NEN++;
									};
								};
							};
						};
					};
				};
				OneObject* BID = SearchBuildingInCell(cell,0xFF);
				OneObject* FriendBID = SearchBuildingInCell(cell,MASK);
				if(BID&&BID!=FriendBID){
					ENLIST[NEN]=BID->Index;
					NEN++;
				}
			};
		};
	};
	NArm=0;
	if(!NEN)return false;
	UnitsGroup* UG=SCENINF.UGRP+Grp->Index;
	
	const int NVICTIM=128;
	int Victim[NVICTIM];
	int VicTarg[NVICTIM];
	memset(Victim,0xFFFF,sizeof(Victim));
	int NVictim=0;
	int NVicInArmy=0;

	for(int i=0;i<UG->N;i++){
		word MID=UG->IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(!OB->Sdoxlo)&&OB->Serial==UG->SNS[i]){
				AdvCharacter* ADC=OB->MoreCharacter;				
				int R0=ADC->MinR_Attack;
				int R1=ADC->MaxR_Attack;//+ADC->AttackRadius2[0];
				//ADC->
				//int RArm=ADC->AttackRadius2[0];
				int RArm=R1>>1;
				bool Arm=false;
				if(!OB->Attack){
					//need to find appropriate unit;
					//1.Search for unit in the attack area					
					byte kmask=OB->newMons->KillMask;
					for(int wp=0;wp<NAttTypes;wp++) 
						kmask |= OB->newMons->AttackMask[wp];					
					bool KBUI=OB->newMons->AttBuild;
					word EMID=0xFFFF;
					int Dist=10000; // Top distance
					int RNear=10000; // Real distance
					int Type=-1;
					int ox=OB->RealX>>10; // Object Soldier
					int oy=OB->RealY>>10;
					for(int j=0;j<NEN;j++){
						OneObject* EOB=Group[ENLIST[j]];
						if(kmask&EOB->MathMask){
							int R=GetTopDistance(EOB->RealX>>10,EOB->RealY>>10,ox,oy);
							int Rv=Norma(OB->RealX-EOB->RealX,OB->RealY-EOB->RealY)>>4;
							if(R<Dist){
								EMID=EOB->Index;
								Dist=R;
								RNear=Rv;
								Type=0;
							};
							if(Rv<RArm){
								Arm=true;
								break;
							}
						};
					};
					if(!Arm){
						if(EMID!=0xFFFF){
							if(!ADC->DetRadius2[1]){
								if(NVictim<NVICTIM){
									Victim[NVictim]=MID;
									VicTarg[NVictim]=EMID;
									NVictim++;
								}
							}else{
								if(RNear>R1||!OB->LocalOrder){
									if(OB->AttackObj(EMID,128+16,0,1)){
										OB->PrioryLevel = 240;
										OB->LocalOrder->PrioryLevel = OB->PrioryLevel;
									}
								}
							}
						}
					};
				}else{ // Soldier in battle
					//if(OB->LocalOrder->PrioryLevel==255){
					//	Smertniki--;
					//	break;
					//}
					
					//if(OB->PrioryLevel<128)					
					for(int j=0;j<NEN;j++){
						OneObject* EOB=Group[ENLIST[j]];
						if(OB->newMons->KillMask&EOB->MathMask){							
							if(OB->EnemyID!=0xFFFF){
								OneObject* COB = Group[OB->EnemyID];
								if(COB&&COB->Serial==OB->EnemySN){
									int Re=Norma(COB->RealX-xc,COB->RealY-yc);
									if(Re>R){
										OB->DeleteLastOrder();
										OB->PrioryLevel = 0;
										break;
									}
								}
							}		
							int Rv=Norma(OB->RealX-EOB->RealX,OB->RealY-EOB->RealY)>>4;
							AdvCharacter* EAC=EOB->MoreCharacter;
							if(ADC->DetRadius2[1]){
								if(Rv<RArm){
									Arm=true;								
									break;
								}
							}else{
								if(Rv<EAC->AttackRadius2[0]){
									Arm=true;								
									break;
								}
								if(OB->PrioryLevel==240){
									NVicInArmy++;
									break;
								}
							}
						};
					};
				};
				if(Arm){
					NArm++;
					if(OB->Attack&&!OB->UnlimitedMotion){
						OB->DeleteLastOrder();
						OB->PrioryLevel = 0;
					}
					//OB->Ready=true;
				}
			};
		};
	};
	int TotalVictim = UG->N>>5;
	for(int i=0;i<NVictim;i++){
		OneObject* OB = Group[Victim[i]];		
		if(OB->AttackObj(VicTarg[i],128+16,0,1)){
			if(NVicInArmy<TotalVictim){
				OB->PrioryLevel = 240;
				OB->LocalOrder->PrioryLevel = OB->PrioryLevel;
			}
			NVicInArmy++;
		}
	}
	return true;
};
bool CheckWallForKilling(OneObject* OB);
int GetBestVictimForArchers2(byte NI,int x,int y,int R,int MyTop){
	int NA=GetNAreas();
	if(MyTop<0||MyTop>=NA)return 0xFFFF;
	char ARRAY[64*64];
	word BESTID[64*64];
	memset(ARRAY,0,sizeof ARRAY);
	memset(BESTID,0xFF,sizeof BESTID);
	int minx=(x-1024+64)<<4;
	int miny=(y-1024+64)<<4;
	int maxx=(x+1024-64)<<4;
	int maxy=(y+1024-64)<<4;
	int x0=(x-1024)<<4;
	int y0=(y-1024)<<4;
	byte Mask=1<<NI;
	for(int i=0;i<MAXOBJECT;i++){
		OneObject*OB=Group[i];
		if(OB&&OB->RealX>minx&&OB->RealX<maxx&&OB->RealY>miny&&OB->RealY<maxy&&!OB->Sdoxlo){
			int ofs=((OB->RealX-x0)>>9)+(((OB->RealY-y0)>>9)<<6);
			if(ofs<4096){
				if(!(OB->newMons->Artilery||OB->Sdoxlo)){
					if(OB->NewBuilding){
						if(!(OB->NMask&Mask)){
							byte use=OB->newMons->Usage;
							if(use==TowerID||use==MineID||use==CenterID){
								ARRAY[ofs]+=50;
							}else{
								if(OB->Life<4000){
									ARRAY[ofs]+=30;
								}else
								if(OB->Life<8000){
									ARRAY[ofs]+=20;
								}else
								if(OB->Life<20000){
									ARRAY[ofs]+=8;
								}else ARRAY[ofs]+=3;
							};
							BESTID[ofs]=OB->Index;
						};
					}else{
						if(OB->NMask&Mask){
							ARRAY[ofs   ]--;
							ARRAY[ofs+1 ]--;
							ARRAY[ofs-1 ]--;
							ARRAY[ofs-64]--;
							ARRAY[ofs+64]--;
							ARRAY[ofs-63]--;
							ARRAY[ofs+63]--;
							ARRAY[ofs-65]--;
							ARRAY[ofs+65]--;
						}else{
							ARRAY[ofs   ]++;
							ARRAY[ofs+1 ]++;
							ARRAY[ofs-1 ]++;
							ARRAY[ofs-64]++;
							ARRAY[ofs+64]++;
							ARRAY[ofs-63]++;
							ARRAY[ofs+63]++;
							ARRAY[ofs-65]++;
							ARRAY[ofs+65]++;
							BESTID[ofs]=OB->Index;
						};
					};
				};
			};
		};
	};
	//well. Time to kill!
	int MAXVAL=0;
	int BESTENID=0xFFFF;
	int tx0=x>>6;
	int ty0=y>>6;
	for(int i=0;i<4096;i++){
		int V;
		if(BESTID[i]!=0xFFFF&&(V=ARRAY[i])>0){
			int ix=(i&63)-32;
			int iy=(i>>6)-32;
			int N=Norma(ix,iy);
			if(N<8)N=8;
			if(N>R)N=100000;
			V=(V<<8)/N;
			if(V>MAXVAL){
				int tx=tx0+(ix>>1);
				int ty=ty0+(iy>>1);
				OneObject* OB=Group[BESTID[i]];
				if(OB->Wall){
					MAXVAL=V;
					BESTENID=BESTID[i];
				}else{
					if(tx>0&&tx<TopLx&&ty>0&&ty<TopLy){	
						int FTOP=GetTopRef(tx+(ty<<TopSH));
						if(FTOP<NA){
							if(FTOP==MyTop||GetLinksDist(FTOP*NA+MyTop)<40){
								MAXVAL=V;
								BESTENID=BESTID[i];
							};
						};
					};
				};
			};
		};
	};
	return BESTENID;
};
DLLEXPORT
bool Attack4(GAMEOBJ* Grp,GAMEOBJ* Zone,byte NI, int& NArm){
	int xc,yc,R;
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		xc=Zone->Index<<4;
		yc=Zone->Serial<<4;
		R=(Zone->Type&0xFFFFFF)<<4;
	}else
	if(Zone->Type!='ZONE'){
		return false;
	}else{
		ActiveZone* AZ=AZones[SCENINF.ZGRP[Zone->Index].ZoneID[0]];
		xc=AZ->x<<4;
		yc=AZ->y<<4;
		R=AZ->R<<4;
	};
	word ENLIST[1600];
	int NEN=0;
	byte MASK=NATIONS[NI].NMask;
	
	int MinR=100000000;
	word EMID=0xFFFF;

	int X0=xc>>11;
	int Y0=yc>>11;
	int RR=(R>>11)+2;
	int TRMAX=(R>>9)+3;
	for(int i=0;i<RR;i++){
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		int N=Rarr[i].N;
		for(int j=0;j<N;j++){
			int XX=X0+xi[j];
			int YY=Y0+yi[j];
			if(XX>=0&&YY>=0&&XX<VAL_MAXCX-1&&YY<VAL_MAXCX-1){
				int cell=XX+1+((YY+1)<<VAL_SHFCX);
				int NMon=MCount[cell];
				if(NMon){
					int ofs1=cell<<SHFCELL;
					word MID;
					for(int i=0;i<NMon;i++){
						MID=GetNMSL(ofs1+i);
						if(MID!=0xFFFF){
							OneObject* OB=Group[MID];
							if(OB&&NEN<1600&&!(OB->NMask&MASK||OB->Sdoxlo)){
								int R0=Norma(OB->RealX-xc,OB->RealY-yc);
								if(R0<R){
									int TR=GetTopDistance(OB->RealX>>10,OB->RealY>>10,xc>>10,yc>>10);
									if(TR<TRMAX){
										ENLIST[NEN]=OB->Index;
										NEN++;
									};
								};
							};
						};
					};
				};
				OneObject* BID = SearchBuildingInCell(cell,~MASK);
				if(BID){
					ENLIST[NEN]=BID->Index;
					NEN++;
				}
			};
		};
	};
	NArm=0;
	if(!NEN)return false;
	UnitsGroup* UG=SCENINF.UGRP+Grp->Index;
	
	/*const NVICTIM=128;
	int Victim[NVICTIM];
	for(i=0;i<NVICTIM;i++) Victim[i]=0xFFFF;
	int NVictim=UG->N>>5;
	if(NVictim>NVICTIM) NVictim=NVICTIM;*/

	for(int i=0;i<UG->N;i++){
		word MID=UG->IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(!OB->Sdoxlo)&&OB->Serial==UG->SNS[i]){
				AdvCharacter* ADC=OB->MoreCharacter;				
				if(ADC->DetRadius2[1]) continue;
				int R0=ADC->MinR_Attack;
				int R1=ADC->MaxR_Attack;//+ADC->AttackRadius2[0];
				//ADC->
				//int RArm=ADC->AttackRadius2[0];
				int RArm=R1>>1;
				if(RArm<R0) RArm=R0;
				bool Arm=false;
				if(!OB->Attack){
					//need to find appropriate unit;
					//1.Search for unit in the attack area					
					byte kmask=OB->newMons->KillMask;
					for(int wp=0;wp<NAttTypes;wp++) 
						kmask |= OB->newMons->AttackMask[wp];					
					bool KBUI=OB->newMons->AttBuild;
					word EMID=0xFFFF;
					int Dist=10000; // Top distance
					int RNear=10000; // Real distance
					int Type=-1;
					int ox=OB->RealX>>10; // Object Soldier
					int oy=OB->RealY>>10;
					for(int j=0;j<NEN;j++){
						OneObject* EOB=Group[ENLIST[j]];
						if((kmask&EOB->MathMask)){
							AdvCharacter* EAC=EOB->MoreCharacter;
							int Rv=Norma(OB->RealX-EOB->RealX,OB->RealY-EOB->RealY)>>4;
							if(EAC->DetRadius2[1]){
								int R=GetTopDistance(EOB->RealX>>10,EOB->RealY>>10,ox,oy);								
								if(R<Dist){
									EMID=EOB->Index;
									Dist=R;
									RNear=Rv;
									Type=0;
									R1=EAC->AttackRadius2[1];
								};							
							}else{
								if(Rv<EAC->MaxR_Attack){
									Arm=true;
									break;
								}
							}
						};
					};
					if(!Arm){
						if(EMID!=0xFFFF){
							if(RNear>R1||!OB->LocalOrder){
								if(OB->AttackObj(EMID,128+16,0,0)){
									OB->PrioryLevel = 240;
									OB->LocalOrder->PrioryLevel = OB->PrioryLevel;
								}
							}
						}
					};
				}else{ // Soldier in battle
					//if(OB->LocalOrder->PrioryLevel==255){
					//	Smertniki--;
					//	break;
					//}
					
					//if(OB->PrioryLevel<128)					
					for(int j=0;j<NEN;j++){
						OneObject* EOB=Group[ENLIST[j]];
						if(OB->newMons->KillMask&EOB->MathMask){
							if(OB->EnemyID!=0xFFFF){
								OneObject* COB = Group[OB->EnemyID];
								if(COB&&COB->Serial==OB->EnemySN){
									int Re=Norma(COB->RealX-xc,COB->RealY-yc);
									if(Re>R){
										OB->DeleteLastOrder();
										OB->PrioryLevel = 0;
										break;
									}
								}
							}		
							int Rv=Norma(OB->RealX-EOB->RealX,OB->RealY-EOB->RealY)>>4;
							AdvCharacter* EAC=EOB->MoreCharacter;
							if(!EAC->DetRadius2[1] && Rv<EAC->MaxR_Attack){
								if(OB->Attack&&!OB->UnlimitedMotion){
									OB->DeleteLastOrder();
									OB->PrioryLevel = 0;
								}
								Arm=true;								
								break;
							}
						};
					};
				};
				if(Arm){
					NArm++;
					//OB->Ready=true;
				}
			};
		};
	};
	return true;
};
DLLEXPORT
bool Attack6(GAMEOBJ* Grp,GAMEOBJ* Zone,byte NI, int& NArm){
	int xc,yc,R;
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		xc=Zone->Index<<4;
		yc=Zone->Serial<<4;
		R=(Zone->Type&0xFFFFFF)<<4;
	}else
	if(Zone->Type!='ZONE'){
		return false;
	}else{
		ActiveZone* AZ=AZones[SCENINF.ZGRP[Zone->Index].ZoneID[0]];
		xc=AZ->x<<4;
		yc=AZ->y<<4;
		R=AZ->R<<4;
	};
	word ENLIST[1600];
	int NEN=0;
	byte MASK=NATIONS[NI].NMask;
	
	int MinR=100000000;
	word EMID=0xFFFF;

	int X0=xc>>11;
	int Y0=yc>>11;
	int RR=(R>>11)+2;
	int TRMAX=(R>>9)+3;
	for(int i=0;i<RR;i++){
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		int N=Rarr[i].N;
		for(int j=0;j<N;j++){
			int XX=X0+xi[j];
			int YY=Y0+yi[j];
			if(XX>=0&&YY>=0&&XX<VAL_MAXCX-1&&YY<VAL_MAXCX-1){
				int cell=XX+1+((YY+1)<<VAL_SHFCX);
				int NMon=MCount[cell];
				if(NMon){
					int ofs1=cell<<SHFCELL;
					word MID;
					for(int i=0;i<NMon;i++){
						MID=GetNMSL(ofs1+i);
						if(MID!=0xFFFF){
							OneObject* OB=Group[MID];
							if(OB&&NEN<1600&&!(OB->NMask&MASK||OB->Sdoxlo)){
								int R0=Norma(OB->RealX-xc,OB->RealY-yc);
								if(R0<R){
									int TR=GetTopDistance(OB->RealX>>10,OB->RealY>>10,xc>>10,yc>>10);
									if(TR<TRMAX){
										ENLIST[NEN]=OB->Index;
										NEN++;
									};
								};
							};
						};
					};
				};
				OneObject* BID = SearchBuildingInCell(cell,~MASK);
				if(BID){
					ENLIST[NEN]=BID->Index;
					NEN++;
				}
			};
		};
	};
	NArm=0;
	if(!NEN)return false;
	UnitsGroup* UG=SCENINF.UGRP+Grp->Index;
	
	const int NVICTIM=128;
	int Victim[NVICTIM];
	int VicTarg[NVICTIM];
	memset(Victim,0xFFFF,sizeof(Victim));
	int NVictim=0;
	int NVicInArmy=0;

	for(int i=0;i<UG->N;i++){
		word MID=UG->IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(!OB->Sdoxlo)&&OB->Serial==UG->SNS[i]){
				AdvCharacter* ADC=OB->MoreCharacter;				
				int R0=ADC->MinR_Attack;
				int R1=ADC->MaxR_Attack;//+ADC->AttackRadius2[0];
				//ADC->
				//int RArm=ADC->AttackRadius2[0];
				int RArm=R1>>1;
				bool Arm=false;
				if(!OB->Attack){
					//need to find appropriate unit;
					//1.Search for unit in the attack area					
					byte kmask=OB->newMons->KillMask;
					for(int wp=0;wp<NAttTypes;wp++) 
						kmask |= OB->newMons->AttackMask[wp];					
					bool KBUI=OB->newMons->AttBuild;
					word EMID=0xFFFF;
					int Dist=10000; // Top distance
					int RNear=10000; // Real distance
					int Type=-1;
					int ox=OB->RealX>>10; // Object Soldier
					int oy=OB->RealY>>10;
					for(int j=0;j<NEN;j++){
						OneObject* EOB=Group[ENLIST[j]];
						//if(kmask&EOB->MathMask)
						{
							int R=GetTopDistance(EOB->RealX>>10,EOB->RealY>>10,ox,oy);
							int Rv=Norma(OB->RealX-EOB->RealX,OB->RealY-EOB->RealY)>>4;
							if(R<Dist){
								EMID=EOB->Index;
								Dist=R;
								RNear=Rv;
								Type=0;
							};
							if(Rv<RArm){
								Arm=false;
								break;
							}
						};
					};
			
					
					if(!Arm){
						if(EMID!=0xFFFF){
							if(!ADC->DetRadius2[1]){
								if(NVictim<NVICTIM){
									Victim[NVictim]=MID;
									VicTarg[NVictim]=EMID;
									NVictim++;
								}
							}else{
								OneObject* EOB = Group[EMID];
								if(EOB->NewBuilding&&EOB->NInside==0){
									if(OB->GoToMine(EMID,255,0)){
										OB->PrioryLevel = 240;
										OB->LocalOrder->PrioryLevel = OB->PrioryLevel;
									}
								}else{
									if(RNear>R1||!OB->LocalOrder){
										if(OB->AttackObj(EMID,128+16,0,0)){
											OB->PrioryLevel = 240;
											OB->LocalOrder->PrioryLevel = OB->PrioryLevel;
										}
									}
								}
							}
						}
					};
				}else{ // Soldier in battle
					//if(OB->LocalOrder->PrioryLevel==255){
					//	Smertniki--;
					//	break;
					//}
					
					//if(OB->PrioryLevel<128)					
					for(int j=0;j<NEN;j++){
						OneObject* EOB=Group[ENLIST[j]];
						if(OB->newMons->KillMask&EOB->MathMask){							
							if(OB->EnemyID!=0xFFFF){
								OneObject* COB = Group[OB->EnemyID];
								if(COB&&COB->Serial==OB->EnemySN){
									int Re=Norma(COB->RealX-xc,COB->RealY-yc);
									if(Re>R){
										OB->DeleteLastOrder();
										OB->PrioryLevel = 0;
										break;
									}
								}
							}		
							int Rv=Norma(OB->RealX-EOB->RealX,OB->RealY-EOB->RealY)>>4;
							AdvCharacter* EAC=EOB->MoreCharacter;
							if(ADC->DetRadius2[1]){
								if(Rv<RArm){
									Arm=false;								
									break;
								}
							}else{
								if(Rv<EAC->AttackRadius2[0]){
									Arm=false;								
									break;
								}
								if(OB->PrioryLevel==240){
									NVicInArmy++;
									break;
								}
							}
						};
					};
				};
				if(Arm){
					NArm++;
					OB->DeleteLastOrder();
					OB->PrioryLevel = 0;
					//OB->Ready=true;
				}
			};
		};
	};
	int TotalVictim = UG->N>>5;
	for(int i=0;i<NVictim;i++){
		OneObject* OB = Group[Victim[i]];		
		if(OB->AttackObj(VicTarg[i],128+16,0,0)){
			if(NVicInArmy<TotalVictim){
				OB->PrioryLevel = 240;
				OB->LocalOrder->PrioryLevel = OB->PrioryLevel;
			}
			NVicInArmy++;
		}
	}
	return true;
};
DLLEXPORT bool GetTopZRealCoor(int Zone,int* x,int* y){
	if(Zone>=0&&Zone<GetNAreas()){
		Area* AR=GetTopMap(Zone);
		*x=AR->x<<6;
		*y=AR->y<<6;
		return true;
	}else{
		*x=0;
		*y=0;
		return false;
	};
};
DLLEXPORT
int GetEnemy3(int Zone, int Radius, byte NI){
	if(Zone<0||Zone>=GetNAreas()||Radius<0) return 0;
	int xc,yc,R;
	GetTopZoneCoor(Zone,&xc,&yc);
	xc<<=10;
	yc<<=10;
	R=Radius<<4;
	/*
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		xc=Zone->Index<<4;
		yc=Zone->Serial<<4;
		R=(Zone->Type&0xFFFFFF)<<4;
	}else
	if(Zone->Type!='ZONE'){
		return false;
	}else{
		ActiveZone* AZ=AZones+SCENINF.ZGRP[Zone->Index].ZoneID[0];
		xc=AZ->x<<4;
		yc=AZ->y<<4;
		R=AZ->R<<4;
	};
	*/
	word ENLIST[1600];
	int NEN=0;
	byte MASK=NATIONS[NI].NMask;
	
	int MinR=100000000;
	word EMID=0xFFFF;

	int X0=xc>>11;
	int Y0=yc>>11;
	int RR=(R>>11)+2;
	int TRMAX=(R>>9)+3;
	for(int i=0;i<RR;i++){
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		int N=Rarr[i].N;
		for(int j=0;j<N;j++){
			int XX=X0+xi[j];
			int YY=Y0+yi[j];
			if(XX>=0&&YY>=0&&XX<VAL_MAXCX-1&&YY<VAL_MAXCX-1){
				int cell=XX+1+((YY+1)<<VAL_SHFCX);
				int NMon=MCount[cell];
				if(NMon){
					int ofs1=cell<<SHFCELL;
					word MID;
					for(int i=0;i<NMon;i++){
						MID=GetNMSL(ofs1+i);
						if(MID!=0xFFFF){
							OneObject* OB=Group[MID];
							if(OB&&NEN<1600&&!(OB->NMask&MASK||OB->Sdoxlo)){
								int R0=Norma(OB->RealX-xc,OB->RealY-yc);
								if(R0<R){
									int TR=GetTopDistance(OB->RealX>>10,OB->RealY>>10,xc>>10,yc>>10);
									if(TR<TRMAX){
										ENLIST[NEN]=OB->Index;
										NEN++;
									};
								};
							};
						};
					};
				};
			};
		};
	};
	return NEN;
}
DIALOGS_API void VitalError(char* Mess){
#ifndef _INLINES
	MessageBox(NULL,Mess,"Vital's error",MB_TOPMOST);
	assert(false);
#endif
}