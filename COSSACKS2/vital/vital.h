// ActiveScenary.cpp
//#include "vital\vital.h"

struct ArmInfo{
	int x,y;
	word NMen;
	word MenID[3000];
	word MenSN[3000];
	word NIndex[2048];
	word aNIndex;			// amount of units type
	word NUnits[10][32];	// amount of warriors [NIndex][Kills level of unit]
};

DLLEXPORT
void GetEArmyInfo(byte NI, ArmInfo* Army, int GlRadius){	//return Force
	int xc,yc,R;
	xc = Army->x<<4;
	yc = Army->y<<4;
	R = GlRadius<<4;
	if(xc<0||yc<0||R<0) return;

	word* ATYPE = &Army->aNIndex;
	memset(Army->NIndex,0xFF,sizeof(Army->NIndex));

	word* ENLIST = Army->MenID;
	word* ENSN = Army->MenSN;
	word* NEN = &Army->NMen;
	byte MASK=NATIONS[NI].NMask;	
	
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
							if(OB&&(*NEN)<3000&&!(OB->NMask&MASK||OB->Sdoxlo)){
								int R0=Norma(OB->RealX-xc,OB->RealY-yc);
								if(R0<R){
									int TR=GetTopDistance(OB->RealX>>10,OB->RealY>>10,xc>>10,yc>>10);
									if(TR<TRMAX){
										ENLIST[*NEN]=OB->Index;
										ENSN[*NEN]=OB->Serial;

										*NEN++;
									};
								};
							};
						};
					};
				};
				/*
				OneObject* BID = SearchBuildingInCell(cell,~MASK);
				if(BID&&(BID->NInside||!BID->InFire)){
					ENLIST[NEN]=BID->Index;
					NEN++;
				}
				*/
			};
		};
	};

}

DLLEXPORT
void ClearGroup(GAMEOBJ* Grp){
	if(Grp->Type!='UNIT' || Grp->Index>=SCENINF.UGRP.GetAmount())return;
	UnitsGroup* GRP=&SCENINF.UGRP[Grp->Index];
	GRP->N=0;
}

DLLEXPORT int GetRND(int Max){
	addrand(Max);
	int re=(int(rando())*Max)>>15;
	return re;
	//return rando()%Max;
}
DLLEXPORT int GetRND_X(int Max,int line,const char* file){
	AddRandNew((char*)file,line,Max);
	int re=(int(rando())*Max)>>15;
	return re;
	//return rando()%Max;
}

extern int TIME1;

DLLEXPORT
void SetTime1(int val){
	TIME1=val;
};

DLLEXPORT
void RemoveAttackingUnits(GAMEOBJ* Grp){
	if(Grp->Type=='UNIT' && Grp->Index<SCENINF.UGRP.GetAmount()){
		UnitsGroup* GRP=&SCENINF.UGRP[Grp->Index];
		
		
		word* IDS=GRP->IDS;
		word* SNS=GRP->SNS;
		int N=GRP->N;
		for(int i=0;i<N;i++){
			word MID=IDS[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==SNS[i]&&!OB->Hidden&&OB->Attack){					
					SNS[i]=0xFFFF;
				};
			};
		};
		
		
	}
}

DLLEXPORT int GetUnitsTypeInGroup(GAMEOBJ* Grp, word NIndex){
	if(Grp->Type=='UNIT'&& Grp->Index<SCENINF.UGRP.GetAmount()){
		UnitsGroup* GRP=&SCENINF.UGRP[Grp->Index];
		
		
		word* IDS=GRP->IDS;
		word* SNS=GRP->SNS;
		int N=GRP->N;
		int n=0;
		for(int i=0;i<N;i++){
			word MID=IDS[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==SNS[i]&&(!OB->Sdoxlo||OB->Hidden)
					&&(!(OB->UnlimitedMotion||OB->LockedInBuilding!=0xFFFF))
					&&OB->NIndex==NIndex){					
					n++;
				};
			};
		};
		
		
		return n;
	}
	return -1;
}

DLLEXPORT
bool ArtAttackGroud(byte NI,GAMEOBJ* Grp, int x, int y){
	if(Grp->Type!='UNIT' || Grp->Index>=SCENINF.UGRP.GetAmount() || x<0 || y<0) return false;
	
	byte MASK=NATIONS[NI].NMask;

	UnitsGroup* GRP=&SCENINF.UGRP[Grp->Index];
	
	

	int N=GRP->N;
	word* IDS=GRP->IDS;
	word* SNS=GRP->SNS;
	
	bool att=false;
	for(int i=0;i<N;i++){
		word MID=IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]&&!OB->Sdoxlo&&!OB->Attack){
				addrand(1236);
				if(OB->NewAttackPoint(x,y,128+16,2,1)) att=true;
			}
		}
	}
	
	
	return att;
}

CEXPORT
int GetMaxObject(){
	return MAXOBJECT;
}

DLLEXPORT
bool ArtAttackObject(byte NI,GAMEOBJ* Grp,int TargetID){
	if(Grp->Type!='UNIT' || Grp->Index>=SCENINF.UGRP.GetAmount() || TargetID<0 || TargetID>MAXOBJECT) return false;
	
	byte MASK=NATIONS[NI].NMask;

	UnitsGroup* GRP=&SCENINF.UGRP[Grp->Index];
	
	

	int N=GRP->N;
	word* IDS=GRP->IDS;
	word* SNS=GRP->SNS;
	
	OneObject* TOB=Group[TargetID];
	if(TOB&&!TOB->Sdoxlo){
		for(int i=0;i<N;i++){
			word MID=IDS[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==SNS[i]&&!OB->Sdoxlo&&!(MASK&TOB->NMask)&&!OB->Attack){
					OB->AttackObj(TargetID,128+16,2,1);
				}
			}
		}
		
		
		return true;
	}
	
	
	return false;
}


DLLEXPORT
bool GrpAttackObject(byte NI,GAMEOBJ* Grp,int TargetID){
	if(Grp->Type!='UNIT' || Grp->Index>=SCENINF.UGRP.GetAmount() || TargetID<0 || TargetID>MAXOBJECT) return false;
	
	byte MASK=NATIONS[NI].NMask;

	UnitsGroup* GRP=&SCENINF.UGRP[Grp->Index];
	
	

	int N=GRP->N;
	word* IDS=GRP->IDS;
	word* SNS=GRP->SNS;
	
	OneObject* TOB=Group[TargetID];
	if(TOB&&!TOB->Sdoxlo){
		for(int i=0;i<N;i++){
			word MID=IDS[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];				
				if(OB&&OB->Serial==SNS[i]&&!OB->Sdoxlo){
					if(!OB->Attack){
						if(!(MASK&TOB->NMask)) OB->AttackObj(TargetID,128+15,0,0);
					}else{
						if(rando()<600&&!OB->UnlimitedMotion) OB->ClearOrders();
					}
					if(OB->BrigadeID!=0xFFFF){
						Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
						BR->AttEnm=true;
					}					
				}
			}
		}
		
		
		return true;
	}
	
	
	return false;
}

// strelki, pehota - storm build
DLLEXPORT
bool Attack5(GAMEOBJ* Grp,GAMEOBJ* Zone,byte NI, int& NArm){
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
	UnitsGroup* UG=&SCENINF.UGRP[Grp->Index];
	
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
					
					if(OB->newMons->SlowRecharge){
						if(OB->delay==0){
							//OB->NoSearchVictim
						}else{
							continue;
						}
					}

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
						if((kmask&EOB->MathMask)&&!(OB->newMons->AttackMask[0]&EOB->MathMask)){
							AdvCharacter* EAC=EOB->MoreCharacter;
							int Rv=Norma(OB->RealX-EOB->RealX,OB->RealY-EOB->RealY)>>4;
							int R=GetTopDistance(EOB->RealX>>10,EOB->RealY>>10,ox,oy);								
							if(R<Dist){
								EMID=EOB->Index;
								Dist=R;
								RNear=Rv;
								Type=0;
								R1=EAC->AttackRadius2[1];
							}
						};
					};
					if(!Arm){
						if(EMID!=0xFFFF){
							OneObject* EOB = Group[EMID];
							if(EOB->NewBuilding&&EOB->NInside==0){
								if(OB->GoToMine(EMID,255,0)){
									OB->PrioryLevel = 240;
									OB->LocalOrder->PrioryLevel = OB->PrioryLevel;
								};
							}else
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
								OB->DeleteLastOrder();
								OB->PrioryLevel = 0;
								Arm=false;								
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
int GetEnemyForce(GAMEOBJ* Zone,byte NI){
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

	byte MASK=NATIONS[NI].NMask;
	
	int Frc=0;

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
							if(OB&&!OB->Sdoxlo){
								int R0=Norma(OB->RealX-xc,OB->RealY-yc);
								if(R0<R){
									int TR=GetTopDistance(OB->RealX>>10,OB->RealY>>10,xc>>10,yc>>10);
									if(TR<TRMAX){
										int force=OB->newMons->Force;
										if(OB->NMask&MASK) Frc-=force;
											else Frc+=force;
									};
								};
							};
						};
					};
				};
				//OneObject* BID = SearchBuildingInCell(cell,~MASK);
				//if(BID){
				//	ENLIST[NEN]=BID->Index;
				//	NEN++;
				//}
			};
		};
	};
	return Frc;
}

DLLEXPORT
int GetAlliesForce(GAMEOBJ* Zone,byte NI){
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
	byte MASK=~NATIONS[NI].NMask;
	
	int Frc=0;

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
										Frc+=OB->newMons->Force;
									};
								};
							};
						};
					};
				};
				//OneObject* BID = SearchBuildingInCell(cell,~MASK);
				//if(BID){
				//	ENLIST[NEN]=BID->Index;
				//	NEN++;
				//}
			};
		};
	};
	return Frc;
}

DLLEXPORT
bool AddEnemyInZoneToGroup(GAMEOBJ* Zone,GAMEOBJ* GROUP,byte NI,int Usage, int* Force){
	int x=0;
	int y=0;
	int RR=0;
	if((Zone->Type&0xFF000000)==('@   '-0x202020)){
		x=int(Zone->Index);
		y=int(Zone->Serial);
		RR=Zone->Type&0xFFFFFF;
	}else
	if(Zone->Type=='ZONE'){
		ActiveZone* AZ=AZones[SCENINF.ZGRP[Zone->Index].ZoneID[0]];
		x=AZ->x;
		y=AZ->y;
		RR=AZ->R;
	}else{
		//IntErr("AddUnitsInZoneToGroup : Invalid <Zone>");
		//SCENINF.NErrors++;
		return false;
	};	
	int R=(RR>>7)+2;
	int cx=x>>7;
	int cy=y>>7;

	int zx=x<<4;
	int zy=y<<4;
	int Rz=RR<<4;
	if(GROUP->Type!='UNIT'){
		//IntErr("AddUnitsInZoneToGroup : Invalid <Group>");
		//SCENINF.NErrors++;
		return false;
	};
	word* UIDS=NULL;
	word* SNS=NULL;
	int NUIDS=0;
	int MaxUIDS=0;
	int mxx=msx>>2;
	int myy=msy>>2;
	*Force=0;
	for(int r=0;r<R;r++){
		char* xi=Rarr[r].xi;
		char* yi=Rarr[r].yi;
		int Np=Rarr[r].N;
		for(int p=0;p<Np;p++){
			int xp=cx+xi[p];
			int yp=cy+yi[p];
			if(xp>=0&&yp>=0&&xp<mxx&&yp<myy){
				int cell=xp+1+((yp+1)<<VAL_SHFCX);
				int NMon=MCount[cell];
				if(NMon){
					int ofs1=cell<<SHFCELL;
					word MID;
					for(int i=0;i<NMon;i++){
						MID=GetNMSL(ofs1+i);
						if(MID!=0xFFFF){
							OneObject* OB=Group[MID];
							if(OB&&OB->NNUM==NI&&(!OB->Sdoxlo)&&Norma(OB->RealX-zx,OB->RealY-zy)<Rz){
								if(Usage==0||(Usage!=0&&OB->newMons->Usage==Usage)){
									if(NUIDS>=MaxUIDS){
										MaxUIDS+=128;
										UIDS=(word*)realloc(UIDS,MaxUIDS<<1);
										SNS=(word*)realloc(SNS,MaxUIDS<<1);
									};
									UIDS[NUIDS]=MID;
									SNS[NUIDS]=OB->Serial;
									NUIDS++;
									*Force+=OB->newMons->Force;
								};
							};
						};
					};
				};
			};
		};
	};
	
	
	if(SCENINF.UGRP[GROUP->Index].IDS){
		free(SCENINF.UGRP[GROUP->Index].IDS);
		free(SCENINF.UGRP[GROUP->Index].SNS);
	};
	SCENINF.UGRP[GROUP->Index].IDS=UIDS;
	SCENINF.UGRP[GROUP->Index].SNS=SNS;
	SCENINF.UGRP[GROUP->Index].N=NUIDS;
	SCENINF.UGRP[GROUP->Index].Max=NUIDS;
	
	
	return true;
};
//////////////////////////
OneObject* SearchBuildingInCell(int cell,byte nmask);
DLLEXPORT
bool Attack7(GAMEOBJ* Grp,GAMEOBJ* Zone,byte NI, int& NArm, int Force){ // same as Attack3 but Grp.Force>Force
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
	
	int Frc=0;

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
										Frc+=OB->newMons->Force;
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
	if(!NEN || Force<Frc)return false;
	UnitsGroup* UG=&SCENINF.UGRP[Grp->Index];
	
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
						if(kmask&EOB->MathMask&&!(OB->newMons->AttackMask[0]&EOB->MathMask)){
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
									if(OB->AttackObj(EMID,128+16,0,0)){
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

DLLEXPORT int GetTopZone(int x,int y);

DLLEXPORT
void CreateEnemyBuildingsTopList(word* IDS,byte NI){
	byte NMask=NATIONS[NI].NMask;
	int NA=GetNAreas();
	memset(IDS,0xFF,NA*2);
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&OB->NewBuilding&&(!(OB->Sdoxlo||OB->NMask&NMask||OB->NNUM==7))&&(OB->Ready||OB->LocalOrder)){
			int top=GetTopZone(OB->RealX>>4,OB->RealY>>4);
			if(top<NA)IDS[top]=i;
		};
	};
};

// 1 - captured
// 2 - buildings
// 4 - near

DLLEXPORT
int SearchCapturedTopToDeffend(byte NI,word* DefTop){
	byte NMask=NATIONS[NI].NMask;
	int NA=GetNAreas();
	memset(DefTop,0,NA<<1);
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&OB->NNUM==NI&&!OB->Sdoxlo){
			int top=GetTopZone(OB->RealX>>4,OB->RealY>>4);			
			if(top<NA){
				if(OB->newMons->Capture) DefTop[top]|=1;
				if(OB->NewBuilding) DefTop[top]|=2;				
			}
		};
	};
	int ItrAmount=1;
	for(int p=0;p<ItrAmount;p++){
		
		for(int j=0;j<NA;j++)if(DefTop[j]&3){
			Area* AR=GetTopMap(j);
			int NL=AR->NLinks;
			//int dang=DangMap[j]/(ItrAmount<<1);
			for(int p=0;p<NL;p++){
				int id=AR->Link[p].NextAreaID;
				if(id<NA){
					DefTop[id]|=4;
					//int DD=int(DangMap[id])+dang;
					//DangMap[id]=DD>65535?65535:DD;
				};
			};
		};
	};
	return 0;
}

DLLEXPORT
void MakeEnemyAgrTopGrp(byte NI, GAMEOBJ* EATG, word* BaseTop){
	byte NMask=NATIONS[NI].NMask;
	int NA=GetNAreas();
	for(int i=0;i<NA;i++) ClearGroup(EATG+i);	
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&!(OB->Sdoxlo||(OB->NMask&NMask))&&OB->newMons->MeatTransformIndex==0xFF){
			int top=GetTopZone(OB->RealX>>4,OB->RealY>>4);			
			if(top<NA&&BaseTop[top]) InsertUnitToGroup(NULL,EATG+top,OB->Index);
		};
	};
}

DLLEXPORT
bool AttackGrpByGrp(GAMEOBJ* Att, GAMEOBJ* Enm){
	if(Att->Type!='UNIT'||Att->Index>=SCENINF.UGRP.GetAmount()||Enm->Type!='UNIT'||Enm->Index>=SCENINF.UGRP.GetAmount()) return false;
	UnitsGroup* ATT=&SCENINF.UGRP[Att->Index];
	UnitsGroup* ENM=&SCENINF.UGRP[Enm->Index];
	for(int i=0;i<ATT->N;i++){
		word MID=ATT->IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(!OB->Sdoxlo)&&OB->Serial==ATT->SNS[i]){
				if(!OB->Attack){
					AdvCharacter* ADC=OB->MoreCharacter;
					int R0=ADC->MinR_Attack;
					int R1=ADC->MaxR_Attack;
					//need to find appropriate unit;
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
					
					int NEN=ENM->N;
					for(int j=0;j<NEN;j++){
						word EID=ENM->IDS[j];
						OneObject* EOB=Group[EID];
						if(!EOB->Sdoxlo&&EOB->Serial==ENM->SNS[j]&&(kmask&EOB->MathMask)&&!(OB->newMons->AttackMask[0]&EOB->MathMask)&&!(OB->NMask&EOB->NMask)&&OB->LockType==EOB->LockType){
							int R=GetTopDistance(EOB->RealX>>10,EOB->RealY>>10,ox,oy);
							int Rv=Norma(OB->RealX-EOB->RealX,OB->RealY-EOB->RealY)>>4;
							if(R<Dist){
								EMID=EOB->Index;
								Dist=R;
								RNear=Rv;
								Type=0;
							};
						};
					};
					if(EMID!=0xFFFF){
						OB->AttackObj(EMID,128+15,0,0);
					}
				}else{
					if(rando()<900&&!OB->UnlimitedMotion){
						OB->ClearOrders();
					};
				}
			};
		};
	};
	return true;
};

DLLEXPORT
bool CheckDef(GAMEOBJ* Def, word DefZone, GAMEOBJ* Branch, word* BaseTop){
	if(Def->Type!='UNIT'||Def->Index>=SCENINF.UGRP.GetAmount()||Branch->Type!='UNIT'||Branch->Index>=SCENINF.UGRP.GetAmount()) return false;
	UnitsGroup* DEF=&SCENINF.UGRP[Def->Index];
	UnitsGroup* BRN=&SCENINF.UGRP[Branch->Index];	
	int N=DEF->N;
	int NA=GetNAreas();
	for(int i=0;i<N;i++){
		word MID=DEF->IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&!OB->Sdoxlo&&OB->Serial==DEF->SNS[i]){
				int top=GetTopZone(OB->RealX>>4,OB->RealY>>4);
				if(top<NA&&(OB->Attack&&!BaseTop[top] || !OB->LocalOrder&&top!=DefZone)){
					DEF->CopyUnits(BRN,i,1,1,1);
				}
			}
		}
	}
	return true;
}

DLLEXPORT word MakeOneShotToBuild(GAMEOBJ* Zone,GAMEOBJ* Grp,byte NI){
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
	//byte MASK=NATIONS[NI].NMask;
	byte MASK=~NATIONS[NI].NMask;

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
				OneObject* BOB = SearchBuildingInCell(cell,MASK);
				//OneObject* BOB = SearchBuildingInCell(cell,0xFF);
				//OneObject* FriendBOB = SearchBuildingInCell(cell,MASK);								
				if(BOB&&BOB->NNUM!=7&&/*BOB!=FriendBOB&&*/!BOB->Sdoxlo&&NEN<1600/*&&BOB->NInside*//*&&(InFire||!(InFire||BOB->InFire))*/){
					word NewID=BOB->Index;
					for(int i=0;i<NEN;i++){
						if(ENLIST[i]==NewID){
							NewID=0xFFFF;
						}
					}
					if(NewID!=0xFFFF){
						ENLIST[NEN]=NewID;
						NEN++;
					}
				}
			};
		};
	};

	if(!NEN)return 0xFFFF;
	UnitsGroup* UG=&SCENINF.UGRP[Grp->Index];

	word BID=0xFFFF;	
	for(int i=0;i<UG->N;i++){
		word MID=UG->IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(!OB->Sdoxlo)&&OB->Serial==UG->SNS[i]){
				AdvCharacter* ADC=OB->MoreCharacter;				
				int R0=ADC->AttackRadius1[1];
				int R1=ADC->MaxR_Attack;//+ADC->AttackRadius2[0];
				if(DetectShootingUnit(OB->Ref.General)) R1=R0+((R1-R0)>>3);
				bool Arm=false;
				if(!OB->Attack){
					//need to find appropriate unit;
					//1.Search for unit in the attack area					
					byte kmask=OB->newMons->KillMask;
					//for(int wp=0;wp<NAttTypes;wp++) 
					//	kmask |= OB->newMons->AttackMask[wp];					
					bool KBUI=OB->newMons->AttBuild;
					word EMID=0xFFFF;
					int Dist=10000; // Top distance
					int RNear=10000; // Real distance
					int Type=-1;
					int ox=OB->RealX>>10; // Object Soldier
					int oy=OB->RealY>>10;
					for(int j=0;j<NEN;j++){
						OneObject* EOB=Group[ENLIST[j]];
						if(kmask&EOB->MathMask&&!(OB->newMons->AttackMask[0]&EOB->MathMask)){
							int Rt=GetTopDistance(EOB->RealX>>10,EOB->RealY>>10,ox,oy);
							//int Rv=Norma(OB->RealX-EOB->RealX,OB->RealY-EOB->RealY)>>4;
							//if(Rv>R0&&Rv<R1+EOB->newMons->AddShotRadius&&R<Dist){
							if( Rt<(R>>10)+10 && Dist>Rt ){
								EMID=EOB->Index;
								Dist=Rt;
								//RNear=Rv;
								//Type=0;
							};
						};
					};
					if(EMID!=0xFFFF){
						//OB->Ready=1;
						//if(OB->BrigadeID!=0xFFFF){
						//	OB->Nat->CITY->Brigs[OB->BrigadeID].ClearBOrders();
						//};
						if(OB->AttackObj(EMID,128+16,0)){
							//OB->PrioryLevel = 240;
							//if(OB->LocalOrder) OB->LocalOrder->PrioryLevel = OB->PrioryLevel;
							BID=EMID;
						}
					};
				}else{					
					//if(rando()<600&&!OB->UnlimitedMotion){
					//	OB->ClearOrders();
					//};
				};
			};
		};
	};
	return BID;
};

DLLEXPORT
bool GetGroupCharge(GAMEOBJ* Grp){
	if(Grp->Type!='UNIT'||Grp->Index>SCENINF.UGRP.GetAmount()||Grp->Index<0) return false;
	UnitsGroup* UG=&SCENINF.UGRP[Grp->Index];
	int N=UG->N;
	word* IDS=UG->IDS;
	word* SNS=UG->SNS;
	for(int i=0;i<N;i++)if(IDS[i]!=0xFFFF){
		OneObject* OB=Group[IDS[i]];
		if(OB&&OB->Serial==SNS[i]&&!(OB->Sdoxlo)){
			if(OB->delay) return false;
		}
	}
	return true;
}

DLLEXPORT
bool AttackByTomahawks(GAMEOBJ* Grp,GAMEOBJ* Zone,byte NI, int& NArm){
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
				/*
				OneObject* BID = SearchBuildingInCell(cell,~MASK);
				if(BID){
					ENLIST[NEN]=BID->Index;
					NEN++;
				}
				*/
			};
		};
	};
	NArm=0;

	UnitsGroup* UG=&SCENINF.UGRP[Grp->Index];

	/*	
	for(i=0;i<UG->N;i++){
		word MID=UG->IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==UG->SNS[i]){
				OB->Ready=1;
			}
		}
	}
	*/

	//if(!NEN)return false;	
	
	/*
	const NVICTIM=128;
	int Victim[NVICTIM];
	int VicTarg[NVICTIM];
	memset(Victim,0xFFFF,sizeof(Victim));
	int NVictim=0;
	int NVicInArmy=0;
	*/

	for(int i=0;i<UG->N;i++){
		word MID=UG->IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(!OB->Sdoxlo)&&OB->Serial==UG->SNS[i]){
				AdvCharacter* ADC=OB->MoreCharacter;				
				int R0=ADC->MinR_Attack;
				int R1=ADC->MaxR_Attack;		
				int RShot0;
				int RShot1;
				if(DetectShootingUnit(OB->Ref.General)){
					RShot1=R1>>1;
					RShot0=RShot1-(R1>>2);					
				}else{
					RShot1=R1;
					RShot0=RShot1-(R1>>2);
					if(RShot0>400) RShot0=400;
				}

				bool Arm=false;
				if(!(OB->Attack||OB->delay)){
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
						if(kmask&EOB->MathMask&&!(OB->newMons->AttackMask[0]&EOB->MathMask)&&OB->LockType==EOB->LockType){
							AdvCharacter* EAC=EOB->MoreCharacter;
							int RArm=EAC->AttackRadius2[0]<<1;
							//if(!RArm) RArm=EAC->MaxR_Attack<<1;
							//int RArm=R1>>1+R1>>2;
							int R=GetTopDistance(EOB->RealX>>10,EOB->RealY>>10,ox,oy);
							int Rv=Norma(OB->RealX-EOB->RealX,OB->RealY-EOB->RealY)>>4;
							if(RArm&&Rv<RArm){
								Arm=true;
								break;
							}
							if(R<=Dist&&Rv<RNear){
								EMID=EOB->Index;
								Dist=R;
								RNear=Rv;
								//int RArm2=RArm<<1;
								//if(RArm2+120<RShot1) RShot0=RArm2;
								Type=0;
							};
						};
					};
					if(!Arm){
						if(EMID!=0xFFFF){
							/*
							if(!ADC->DetRadius2[1]){
								if(NVictim<NVICTIM){
									Victim[NVictim]=MID;
									VicTarg[NVictim]=EMID;
									NVictim++;
								}
							}else
							*/
							{
								//if(RNear>RShot||!OB->LocalOrder||OB->ArmAttack){
								OB->Ready=1;
								if((RShot0<RNear&&RNear<RShot1)||OB->ArmAttack){
									if(OB->BrigadeID!=0xFFFF){
										OB->Nat->CITY->Brigs[OB->BrigadeID].ClearBOrders();
									};
									if(OB->AttackObj(EMID,128+15,0,1)){
										OB->PrioryLevel = 240;
										if(OB->LocalOrder) OB->LocalOrder->PrioryLevel = OB->PrioryLevel;
									}
								}else if(RShot1<RNear){
									OneObject* EOB=Group[EMID];									
									OB->NewMonsterSmartSendTo(EOB->RealX>>4,EOB->RealY>>4,0,0,128+64,0);
									OB->Ready=0;
								}
							}
						}else{
							OB->Ready=1;
						}
					};
				}else{ // Soldier in battle
					//if(OB->LocalOrder->PrioryLevel==255){
					//	Smertniki--;
					//	break;
					//}
					
					if(OB->Attack&&rando()<500&&!OB->UnlimitedMotion){
						OB->ClearOrders();
					}else{
						for(int j=0;j<NEN;j++){
							OneObject* EOB=Group[ENLIST[j]];
							if(OB->newMons->KillMask&EOB->MathMask){
								/*
								// отменить атаку по убежавшему врагу
								if(OB->EnemyID!=0xFFFF){
									OneObject* COB = Group[OB->EnemyID];
									if(COB&&COB->Serial==OB->EnemySN){
										int Re=Norma(COB->RealX-xc,COB->RealY-yc);
										if(Re>R){
											OB->ClearOrders();
											//OB->PrioryLevel = 0;
											break;
										}
									}
								}
								*/
								int Rv=Norma(OB->RealX-EOB->RealX,OB->RealY-EOB->RealY)>>4;
								AdvCharacter* EAC=EOB->MoreCharacter;
								int EMaxAttR=EAC->MaxR_Attack;
								int RArm=EAC->AttackRadius2[0]<<1;
								if(!RArm) RArm=EMaxAttR+(EMaxAttR>>1);
								

								if(ADC->DetRadius2[1]){
									if(RArm&&(Rv<RArm||(Rv<EMaxAttR&&OB->delay))&&!OB->ArmAttack){
										Arm=true;								
										break;
									}
								}
								/*else{
								
									if((||Rv<EAC->AttackRadius2[0])&&!OB->ArmAttack){
										Arm=true;								
										break;
									}
									/*
									if(OB->PrioryLevel==240){
										NVicInArmy++;
										break;
									}
									*/
								//}
							};
						};
					}
					/*Order1* Ord=OB->LocalOrder;
					if(!Arm&&Ord){
						Ord=Ord->NextOrder;
						if(Ord&&Ord->DoLink==&RotUnitLink){
							OB->Clear
						}
					}*/

				};
				if(Arm){
					NArm++;
					//OB->PrioryLevel = 0;
					if(OB->Attack&&!(OB->ArmAttack||OB->UnlimitedMotion)){
						OB->ClearOrders();
					}
					OB->Ready=1;
					/*
					if(OB->delay<OB->MaxDelay){
						OB->PrioryLevel = 240;
						if(OB->LocalOrder) OB->LocalOrder->PrioryLevel = OB->PrioryLevel;
					}
					*/
				}else{
					if(OB->delay&&!OB->ArmAttack){
						//OB->StandGround=true;
						//OB->PrioryLevel = 240;
						OB->Ready=0;
					}
				}
			};
		};
	};
	/*
	int TotalVictim = UG->N>>5;
	for(i=0;i<NVictim;i++){
		OneObject* OB = Group[Victim[i]];		
		if(OB->AttackObj(VicTarg[i],128+16,0,1)){
			if(NVicInArmy<TotalVictim){
				OB->PrioryLevel = 240;
				if(OB->LocalOrder) OB->LocalOrder->PrioryLevel = OB->PrioryLevel;
			}
			NVicInArmy++;
		}
	}
	*/
	return (NEN>0);
};

DLLEXPORT
word GetMonUsage(word NIndex){
	if(NIndex>=NATIONS->NMon) return 0xFFFF;
	return NATIONS[0].Mon[NIndex]->newMons->Usage;
}

DLLEXPORT
short GetMinDamage(word NIndex){
	if(NIndex>=NATIONS->NMon) return -1;
	return NATIONS[0].Mon[NIndex]->newMons->MinDamage[0];
}

DLLEXPORT
short GetMaxDamage(word NIndex){
	if(NIndex>=NATIONS->NMon) return -1;
	return NATIONS[0].Mon[NIndex]->newMons->MaxDamage[0];
}

DLLEXPORT
int GetUnitStorm(word NIndex){
	if(NIndex>=NATIONS->NMon) return 0xFFFF;
	return NATIONS[0].Mon[NIndex]->newMons->StormForce;
}

DLLEXPORT
bool isUnitAbsorber(word Index){
	if(Index>=0&&Index<MAXOBJECT){
		OneObject* BO=Group[Index];
		if(BO&&!BO->Sdoxlo&&BO->newMons->UnitAbsorber) return true;
	}
	return false;
}

CEXPORT
void InitCurNation(byte Nat){
	if(Nat<7) SetMyNation(Nat);
}

CEXPORT
void SetCurNation(byte Nat){
	if(Nat<7){
		/*
		char buf[256];
		char* CurName=GSets.CGame.PL_INFO[GetMyNation()].name;		
		char* NewName=GSets.CGame.PL_INFO[Nat].name;
		strcpy(buf,CurName);
		strcpy(CurName,NewName);
		strcpy(NewName,buf);
		*/
		/*
		PlayerInfo* CP=GSets.CGame.PL_INFO+GetMyNation();
		PlayerInfo* NP=GSets.CGame.PL_INFO+Nat;
		PlayerInfo PI=*CP; *CP=*NP; *NP=PI;
		CP->ColorID=Nat;		
		*/
		
		byte mn=GetMyNation();
		if(mn==Nat) return;
		for(int i=0;i<8;i++){
			PlayerInfo* PI=GSets.CGame.PL_INFO+i;
			if(PI->ColorID==mn) PI->ColorID=Nat;
			else if(PI->ColorID==Nat) PI->ColorID=mn;
		}
		
		SetMyNation(Nat);
	}
}

CEXPORT
int GetFreePlacesInBrig(byte Owner, int BrigID, int MaxPlaces, int* posx, int* posy, int* pos, int& CenterX, int& CenterY){
	// return amount free places and coor in brigade
	if(Owner>=8||BrigID>=MaxBrig) return 0;
	Brigade* Br=CITY[Owner].Brigs+BrigID;
	int n=Br->NMemb;
	word* id=Br->Memb;//+3;
	word* sn=Br->MembSN;
	int* px=Br->posX;
	int* py=Br->posY;
	int nfree=0;
	for(int i=3;i<n&&nfree<MaxPlaces;i++){
		OneObject* OB=Group[id[i]];
		if(id[i]==0xFFFF||!(OB&&OB->Serial==sn[i]&&!OB->Sdoxlo)){
			posx[nfree]=px[i];
			posy[nfree]=py[i];
			pos[nfree]=i;
			nfree++;
		}
		//id++;
	}
	if(!Br->GetCenter(&CenterX,&CenterY)) CenterX=-1;
	return nfree;
};

CEXPORT
bool GetBrDir(byte Owner, int BrigID, byte& Dir){
	if(Owner>=8||BrigID>=MaxBrig) return false;
	Brigade* Br=CITY[Owner].Brigs+BrigID;
	Dir=Br->Direction;
	return true;
}

CEXPORT
bool GetBrAddDS(byte Owner, int BrigID, char& AddDamage, char& AddShield){
	if(Owner>=8||BrigID>=MaxBrig) return false;
	Brigade* Br=CITY[Owner].Brigs+BrigID;
	AddDamage=Br->AddDamage;
	AddShield=Br->AddShield;
	return true;
}

DLLEXPORT
void SetOBRifleAttack(OneObject* OB, word RifleAttack){
	if(OB)OB->RifleAttack=!!RifleAttack;
}

CEXPORT
bool GetBrGroup(byte Owner, int BrigID, int* N, word** ID, word** SN){
	if(Owner>=8||BrigID>=MaxBrig) return false;
	Brigade* Br=CITY[Owner].Brigs+BrigID;
	*N=Br->NMemb;
	*ID=Br->Memb;
	*SN=Br->MembSN;
	return true;
}
//
const int GlRadius=1500;
//
CEXPORT void DrawColoredCircle(int xc,int yc,int R0,int R1,DWORD LineColor1,DWORD LineColor2,DWORD FillColor1,DWORD FillColor2);
DynArray<int> GlBarracksX;
DynArray<int> GlBarracksY;
bool GlBarracksVisible=false;
bool ShowGlBarracksRadius(){
	if(GlBarracksVisible){
		GlBarracksVisible=false;
		int n=GlBarracksX.GetAmount();
		if(n){
			for(int i=0;i<n;i++){
				DrawColoredCircle(GlBarracksX[i],GlBarracksY[i],GlRadius,0,0xFF20FF20,0xFFFFFFFF,0x2F20FF20,0x2FFFFFFF);
			}
			GlBarracksX.Clear();
			GlBarracksY.Clear();
			return true;
		}		
	}
	return false;
}
//
void GotoFinePositionLink(OneObject* OB);
//
bool EnableGlobalCreateBrig=false;
//
int NIdlePeasant=0;
int NGlobalCenters=0;
// NIndex[officer(0..MaxOf)][oficer,drumer,flag,type0,type1,...(0..MaxType)] - type of units
// Amount[center(0..MaxCen)][officer(0..MaxOf)][oficer,drumer,flag,type0,type1,...(0..MaxType)] - amount of units near each
// UnlimitedOfficer[0..MaxCen][0..MaxOf] officers with unlimited motions near centers
CEXPORT int GetGlobalCreateBrigList(byte NI, byte MaxCen, word* CID, byte MaxOf, word* NIndex, word* Amount, byte MaxType, byte* UnlimitedOfficer, byte* UD, byte* UF){
	//if(PlayGameMode)return 0;	
	//return 0;
	memset(CID,0xFF,MaxCen<<1);
	memset(NIndex,0xFF,(MaxOf*MaxType)<<1);
	memset(Amount,0,(MaxCen*MaxOf*MaxType)<<1);
	memset(UnlimitedOfficer,0,MaxCen*MaxOf);
	memset(UD,0,MaxCen*MaxOf);
	memset(UF,0,MaxCen*MaxOf);
	
	int N=NtNUnits[NI];
	word* List=NatList[NI];
		
	// 1 - проход
	// коммандные центры
	byte RefNI=GSets.CGame.cgi_NatRefTBL[MyNation];
	if(NI==RefNI){
		GlBarracksX.Clear();
		GlBarracksY.Clear();
		NIdlePeasant=-1;
	}
	//
	byte nc=0;
	for(int i=0;i<N&&nc<MaxCen;i++){
		OneObject* OB=Group[List[i]];
		if(OB&&!(OB->Sdoxlo||OB->notSelectable())){
			if(OB->newMons->CommandCenter){
				CID[nc]=OB->Index;
				nc++;
				if(NI==RefNI){
					GlBarracksX.Add(OB->RealX>>4);
					GlBarracksY.Add(OB->RealY>>4);
				}
			}
			if(NI==RefNI){
				NewMonster* NM=OB->newMons;
				if(NM->Usage==PeasantID){
					if(NIdlePeasant==-1){
						NIdlePeasant=0;
					}
					if(!OB->LocalOrder||OB->LocalOrder->DoLink==&GotoFinePositionLink){
						NIdlePeasant++;
					}					
				}
			}
		}
	}
	if(NI==GSets.CGame.cgi_NatRefTBL[MyNation]&&!MOptions.Players.Player[NI].DontSelectPanicers){
		NGlobalCenters=nc;
	}
	//
	if(nc==0) return 0;	
	//  карта
	static byte* MapOf=(byte*)malloc(NATIONS->NMon+128);
	memset(MapOf,0xFF,NATIONS->NMon);
	static byte* MapTyp=(byte*)malloc(NATIONS->NMon+128);
	memset(MapTyp,0xFF,NATIONS->NMon);
	// поиск офицеров
	byte no=0;
	int NM=NATIONS[NI].NMon;
	GeneralObject** GGO=NATIONS[NI].Mon;
	for(int i=0;i<NM;i++){
		GeneralObject* GO=GGO[i];
		if(GO->OFCR){
			if(no<MaxOf){
				word* TList=NIndex+no*MaxType;						
				TList[0]=i;//OB->NIndex;						
				TList[1]=GO->OFCR->BarabanID;
				TList[2]=GO->OFCR->FlagID;
				StroiDescription* SD=GO->OFCR->SDES;
				int n=SD->NUnits;
				if(n+3>=MaxType){
					Log.Warning("GetGlobalCreateBrigList(): MaxType is small");
				}						
				for(int j=0;j<n;j++){
					//if(i!=SD->Units[j]){	// если юнит офицер
						TList[j+3]=SD->Units[j];
					//}
				}
				no++;
			}else{
				Log.Warning("GetGlobalCreateBrigList(): no>=MaxOf");
			}
		}	
	}
	//  карта 2
	//static byte* MapOf=(byte*)malloc(NATIONS->NMon+128);
	//memset(MapOf,0xFF,NATIONS->NMon);
	//static byte* MapTyp=(byte*)malloc(NATIONS->NMon+128);
	//memset(MapTyp,0xFF,NATIONS->NMon);	
	word* type=NIndex;
	for(int i=0;i<MaxOf;i++){
		//word oficer=type[0];
		for(int j=0;j<MaxType;j++){			
			if(*type!=0xFFFF){
				MapOf[*type]=i;
				//if(j==0&&type[0]==type[3]){
				//	MapTyp[*type]=3;
				//}else{
					MapTyp[*type]=j;
				//}
			}
			type++;
		}
	}	
	// 2 - проход
	for(int i=0;i<N;i++){
		OneObject* OB=Group[List[i]];
		if(OB&&!(OB->Sdoxlo||OB->InArmy||OB->NotSelectable)&&(EnableGlobalCreateBrig||OB->Mobilised)){
			byte of=MapOf[OB->NIndex];
			byte typ=MapTyp[OB->NIndex];
			if(of!=0xFF&&typ!=0xFF){
				// search center
				for(int c=0;c<nc;c++){
					OneObject* COB=Group[CID[c]];
					if(Norma(OB->RealX-COB->RealX,OB->RealY-COB->RealY)>>4<GlRadius){
						if(EnableGlobalCreateBrig) OB->Mobilised=true;
						int cc=0; // all units collect in first command center
						if(OB->UnlimitedMotion){
							//for(cc=0;cc<nc;cc++){
								switch(typ){
									case 0: UnlimitedOfficer[MaxOf*cc+of]++; break;
									case 1: UD[MaxOf*cc+of]++; break;
									case 2: UF[MaxOf*cc+of]++; break;
								};
							//}
						}else{
							Amount[MaxType*(MaxOf*cc+of)+typ]++;
						}
						break;
					}else{
						if(EnableGlobalCreateBrig) OB->Mobilised=false;
					}
				}
			}
		}
	}
	//free(MapOf);
	//free(MapTyp);
	return 0;
}

