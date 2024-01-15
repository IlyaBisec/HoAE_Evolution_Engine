////////////////
int CreateNewZone(char* Name,int x,int y,int R);
CEXPORT void CreateAZone(char* Name,int x,int y,int R){
	CreateNewZone(Name,x,y,R);
}
CEXPORT void CreateAGroup(byte NI,char* gName){
	ActiveGroup* AG=new ActiveGroup;
	AGroups.Add(AG);	
	int NU=NSL[NI];
	word* ids=Selm[NI];
	AG->Name=gName;
	for(int i=0;i<NU;i++){
		word MID=ids[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB){
				OneUS U;
				U.ID=MID;
				U.SN=OB->Serial;
				AG->Units.Add(U);
			};
		};
	};
	AG->Visible=false;
	AG->Draw(0);
}
CEXPORT void ClearAZones(){
	ClearAllZones();
}
////////////////
CEXPORT
word getDir(int dx,int dy){
	int PhDir;
	if(dx!=0||dy!=0){
		int adx=abs(dx);
		int ady=abs(dy);
		if(adx>ady)PhDir=byte(TAtg[div(ady<<8,adx).quot]);else
				   PhDir=64-byte(TAtg[div(adx<<8,ady).quot]);
		if(dx<0)PhDir=128-PhDir;
		if(dy<0)PhDir=256-PhDir;
	}else PhDir=64+128;
	return (PhDir+1024)&255;
};
////////////////
void MakeReformation(byte NI,word BrigadeID,byte FormType);
CEXPORT
void BrigReformation(byte NI,word BrigadeID,byte FormType){
	MakeReformation(NI,BrigadeID,FormType);
};
////////////////
CEXPORT UnitsGroup* GetUnitsGroup(GAMEOBJ* Group){
	if(!Group||Group->Type!='UNIT'||Group->Index>=SCENINF.UGRP.GetAmount()) return NULL;
		else return &SCENINF.UGRP[Group->Index];
}

/*
CEXPORT
int GetBrigadeTop(byte NI, word BID){
	if(NI>7||BID>511) return 0xFFFF;	
	CITY[NI].Brigs->
}
*/

DLLEXPORT int GetTopZone(int x,int y);

CEXPORT
bool AddEnemyCaptBuildTopList(word* IDS,byte NI){
	byte NMask=NATIONS[NI].NMask;	
	bool Result=false;
	int NA=GetNAreas();
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&OB->NewBuilding&&OB->newMons->Capture&&!(OB->Sdoxlo||(OB->NMask&NMask))&&(OB->NNUM!=7||NI==7)){
			int top=GetTopZone(OB->RealX>>4,OB->RealY>>4);
			if(top<NA){
				IDS[top]=i;
				Result=true;
			}
		};
	};
	return Result;
};

CEXPORT
bool AddEnemyCenterTopList(word* IDS,byte NI){
	byte NMask=NATIONS[NI].NMask;	
	bool Result=false;
	int NA=GetNAreas();
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&OB->NewBuilding&&OB->newMons->CommandCenter&&!(OB->Sdoxlo||(OB->NMask&NMask))&&(OB->NNUM!=7||NI==7)){
			int top=GetTopZone(OB->RealX>>4,OB->RealY>>4);
			if(top<NA){
				IDS[top]=i;
				Result=true;
			}
		};
	};
	return Result;
};

bool DetectShootingUnit(GeneralObject* GO);
bool DetectCannonUnit(GeneralObject* GO){
	NewMonster* NM=GO->newMons;
	return NM->WeaponKind[5]==5||NM->WeaponKind[4]==5;
};

CEXPORT bool DetCannon(word NIndex){
	if(NIndex<NATIONS->NMon){
		return DetectCannonUnit(NATIONS->Mon[NIndex]);
	}
	return false;
};

CEXPORT bool DetSheep(word NIndex){
	if(NIndex<NATIONS->NMon){
		return NATIONS->Mon[NIndex]->newMons->LockType==1;
	}
	return false;
};

DLLEXPORT bool GetTopZRealCoor(int Zone,int* x,int* y);

CEXPORT
long int GetGroupPower(GAMEOBJ* Grp){
	if(Grp->Type!='UNIT'||Grp->Index<0||Grp->Index>=SCENINF.UGRP.GetAmount()) return -1;
	UnitsGroup* UGR=&SCENINF.UGRP[Grp->Index];
	word* IDS=UGR->IDS;
	word* SNS=UGR->SNS;
	int N=UGR->N;
	int life=0,dam=0;
	for(int i=0;i<N;i++){	
		OneObject* OB=Group[IDS[i]];
		if(OB&&OB->Serial==SNS[i]&&!OB->Sdoxlo){
			life+=OB->Life;
			dam+=OB->MoreCharacter->MaxDamage[0];
		}
	}
	return life*dam;
}

CEXPORT
void CreateArmyInfo(byte NI, int* Life, int* Damage, int MaxSize){
	byte MASK=NATIONS[NI].NMask;
	memset(Life,0,MaxSize * sizeof(int));
	memset(Life,0,MaxSize * sizeof(int));
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&!(OB->Sdoxlo||OB->NewBuilding)){							
			int z=GetTopZone(OB->RealX>>4,OB->RealY>>4);
			if(z<MaxSize){
				//int DD=int(DangMap[z]);
				if(!(OB->NMask&MASK)){
					int x=OB->RealX>>4;
					int y=OB->RealY>>4;
					int top=GetTopZone(x,y);
					if(top<0xFFFE){
						Life[top]+=OB->Life;
						AdvCharacter* Adv=OB->MoreCharacter;
						Damage[top]+=Adv->MaxDamage[0];						
					}
				}
			}
		}
	}
}

CEXPORT
void CreateDangerMapBattle(byte NI,int* DangMap,int MaxSize,int* FearArray,int ItrAmount){
	byte MASK=NATIONS[NI].NMask;
	memset(DangMap,0,MaxSize * sizeof(int));
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&!(OB->Sdoxlo||OB->NewBuilding)){							
			int z=GetTopZone(OB->RealX>>4,OB->RealY>>4);
			if(z<MaxSize){
				//int DD=int(DangMap[z]);
				if(!(OB->NMask&MASK)){
					int x=OB->RealX>>4;
					int y=OB->RealY>>4;
					int top=GetTopZone(x,y);
					if(top<0xFFFE){
						// враг
						if(OB->BrigadeID!=0xFFFF){
							Brigade* BRG=CITY[OB->NNUM].Brigs+OB->BrigadeID;						
							if(BRG){
								short Dan=0; 
								if(DetectShootingUnit(OB->Ref.General)){
									Dan+=FearArray[OB->newMons->Category]<<1;
								}else{
									Dan+=FearArray[OB->newMons->Category];
								}

									
								// arm attack
								DangMap[top]+=Dan;	

								/*
								x+=TCos[BRG->Direction]<<1;
								y+=TSin[BRG->Direction]<<1;
								top=GetTopZone(x,y);
								if(top<0xFFFE){
									DangMap[top]+=Dan;
									//if(BRG->)
								}
								*/

							}
						}
						//DD+=(OB->newMons->StormForce>>5)|1;

						// strelok
						if(OB->MoreCharacter->MaxDamage[1]){
							//word MinR=OB->newMons->AdvChar->AttackRadius1[1];
							word MaxR=OB->MoreCharacter->AttackRadius2[1];
							word Dam=OB->MoreCharacter->MaxDamage[1];
							word DamDecr=OB->newMons->DamageDecr[1];
 							
							bool Check[4096];
							memset(Check,0,sizeof(Check));

							word TopList[1024];
							TopList[0]=top;
							int NTopList=1;									

							while(NTopList){
								NTopList--;
								int t=TopList[NTopList];
								Check[t]=1;

								int tx,ty;
								if(GetTopZRealCoor(t,&tx,&ty)){
									short R=Norma(x-tx,y-ty);
									if(R<MaxR){
										DangMap[t]+=short(float(Dam)*exp(-0.693147*float(R)/float(DamDecr)));

										// add links
										Area* AR=GetTopMap(t);
										int NL=AR->NLinks;
										for(int p=0;p<NL;p++){
											int id=AR->Link[p].NextAreaID;
											if(!Check[id]){
												TopList[NTopList]=id;
												NTopList++;													
											};
										};
									}
								}
							}

						}

					}
				}
				//DangMap[z]=DD;
			};			
		};
	};
	
	
	/*
	for(int j=0;j<MaxSize;j++){
		DangMap[j]>>=1;
	}
	
	
	for(int p=0;p<ItrAmount;p++){
		Area* AR=GetTopMap;
		for(int j=0;j<MaxSize;j++){
			int NL=AR[j].NLinks;
			int dang=DangMap[j]/(ItrAmount<<2);
			for(int p=0;p<NL;p++){
				int id=AR[j].Link[p+p];
				if(id<MaxSize){
					int DD=int(DangMap[id])+dang;
					DangMap[id]=DD;
				};
			};
		};
	};
	*/
		
	int Dang[2048];
	for(int i=0;i<MaxSize;i++) Dang[i]=DangMap[i];
	SetDangerMap(Dang);
};

int GetPtDamageInSector(WeaponInSector* WS,byte Angle,int dx,int dy);
bool CheckPtInSector(WeaponInSector* WS,byte Angle,int dx,int dy);
CEXPORT word GetDir(int,int);

CEXPORT
void SetMapOfShooters(byte NI,int* Vistrel, int* Kartech){
	byte MASK=NATIONS[NI].NMask;
	
	int NA=GetNAreas();
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&!(OB->Sdoxlo||OB->NewBuilding)){
			int z=GetTopZone(OB->RealX>>4,OB->RealY>>4);
			if(z<NA){
				
				if(!(OB->NMask&MASK)){
					int x=OB->RealX>>4;
					int y=OB->RealY>>4;
					int top=GetTopZone(x,y);
					if(top<0xFFFE){
												
						// strelok
						if(DetectShootingUnit(OB->Ref.General)){
							AdvCharacter* ADV=OB->MoreCharacter;
							//word MinR=OB->newMons->AdvChar->AttackRadius1[1];
							word MaxR=ADV->AttackRadius2[1];
							word Dam=ADV->MaxDamage[1];
							word DamDecr=OB->newMons->DamageDecr[1];							
							
							bool Check[4096];
							memset(Check,0,sizeof(Check));						

							word TopList[1024];
							TopList[0]=top;
							int NTopList=1;
							Check[top]=1;

							while(NTopList){
								NTopList--;
								int t=TopList[NTopList];								

								int tx,ty;
								if(GetTopZRealCoor(t,&tx,&ty)){
									short R=Norma(x-tx,y-ty);
									if(R<MaxR){
										Vistrel[t]+=short(float(Dam)*exp(-0.693147*float(R)/float(DamDecr)));

										// add links
										Area* AR=GetTopMap(t);
										int NL=AR->NLinks;
										for(int p=0;p<NL;p++){
											int id=AR->Link[p].NextAreaID;
											if(!Check[id]){
												Check[id]=1;
												TopList[NTopList]=id;
												NTopList++;													
											};
										};
									}
								}
							}

						}else
						if(DetectCannonUnit(OB->Ref.General)){
							//Cannon[top]++;

							NewMonster* NMN=OB->newMons;
							
							bool Check[4096];
							memset(Check,0,sizeof(Check));

							word TopList[1024];
							TopList[0]=top;
							int NTopList=1;
							Check[top]=1;

							while(NTopList){
								NTopList--;
								int t=TopList[NTopList];								

								int tx,ty;
								if(GetTopZRealCoor(t,&tx,&ty)){
									int dx=tx-x;
									int dy=ty-y;
									bool fire=CheckPtInSector(NMN->WSECT,OB->RealDir,dx,dy);
									//dx=dx*9/10;
									//dy=dy*9/10;
									if(fire){
										Kartech[t]+=GetPtDamageInSector(NMN->WSECT,OB->RealDir,dx,dy);
									}
									if(fire||t==top){
										// add links
										Area* AR=GetTopMap(t);
										int NL=AR->NLinks;
										for(int p=0;p<NL;p++){
											int id=AR->Link[p].NextAreaID;
											if(!Check[id]){
												Check[id]=1;
												TopList[NTopList]=id;
												NTopList++;													
											};
										};
									}
								}
							}							
						}
					}
				}				
			};			
		};
	};

}

DLLEXPORT
void CreateTopListEnArmyBtl(word* IDS,byte NI,int MinPS){
	word NPS[4096];
	int NA=GetNAreas();
	memset(NPS,0,NA<<1);
	memset(IDS,0xFF,NA<<1);
	byte Mask=NATIONS[NI].NMask;
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&!((OB->NMask&Mask)||OB->Sdoxlo||OB->NewBuilding||OB->NNUM==7)){	//OB->newMons->MeatTransformIndex==0xFF
			int top=GetTopZone(OB->RealX>>4,OB->RealY>>4);
			if(top<NA){				
				//int NIn;
				NPS[top]++;
				IDS[top]=i;
			};
		};
	};
	for(int i=0;i<NA;i++)if(NPS[i]<MinPS){
		IDS[i]=0xFFFF;
	}
};

CEXPORT
bool BrigInStandGround(Brigade* BR){
	if(BR&&BR->WarType){
		return (BR->AddShield>ElementaryOrders[BR->WarType-1].AddShield1);
	}
	return false;
}

int GetTopology(int,int, byte LockType=0);

CEXPORT
void GetEnemyTopInfo(byte NI, int* N, int *xc, int *yc){
	int NZ=GetNAreas();

	memset(N,0,sizeof(int)*NZ);
	memset(xc,0,sizeof(int)*NZ);
	memset(yc,0,sizeof(int)*NZ);	

	byte Mask=NATIONS[NI].NMask;

	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&!(OB->Sdoxlo||OB->NMask==Mask)){
			int x=OB->RealX>>4;
			int y=OB->RealY>>4;
			int top=GetTopology(x,y);
			if(top<0xFFFE){
				N[top]++;
				xc[top]+=x;
				yc[top]+=y;
			}
		}
	}

	for(int i=0;i<NZ;i++){
		int ni=N[i];
		if(ni){
			xc[i]/=ni;
			yc[i]/=ni;
		}
	}
}

CEXPORT
bool DetectAbsorber(word NIndex){
	if(NIndex<NATIONS->NMon){		
		return NATIONS->Mon[NIndex]->newMons->UnitAbsorber;
	}
	return false;
}

CEXPORT
bool DetAbsorbeAbility(word BuildNIndex, word UnitNIndex){
	int NMon=NATIONS->NMon;
	GeneralObject** Mon=NATIONS->Mon;
	if(BuildNIndex<NMon && UnitNIndex<NMon){
		NewMonster* Build=Mon[BuildNIndex]->newMons;
		NewMonster* Unit=Mon[UnitNIndex]->newMons;
		return (Build->UnitAbsorber||Build->PeasantAbsorber&&Unit->Peasant)&&Build->ConcPtX.GetAmount()&&!(Unit->HighUnit&&Build->HighUnitCantEnter);
	}
	return false;
};

void EraseBrigade(Brigade* BR);
CEXPORT
void EraseBrig(byte NI,word BrigadeID){
	if(BrigadeID<MaxBrig-11){
		Brigade* BR=CITY[GSets.CGame.cgi_NatRefTBL[NI]].Brigs+BrigadeID;
		EraseBrigade(BR);	
	}
}