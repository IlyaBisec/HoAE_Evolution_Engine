#include "CommonDip.h"
#include "UnitsGroup.h"
//#include <xmlQuote.hpp>
#include "..\COSSACKS2\Plugins.h"
//
DLLIMPORT DWORD GetCurrentPurpose(GAMEOBJ* GRP);
//
DiplomacySystem::DiplomacySystem(){
	memset(this,0,sizeof(*this));

	memset(AIX,0,7*sizeof(int));
	memset(AIY,0,7*sizeof(int));
	for(int i=0;i<7;i++) AI_set[i]=false;
	FriendAI=false;

	//for(i=0;)
};

extern DiplomacySystem DIPS;

#ifdef AC_ADDON
CEXPORT
void AddDipFriends(word ID, GAMEOBJ* GFriends){	
	if(ID<DIPS.NDIPS && GFriends){
		DIP_SimpleBuilding* dip=(DIP_SimpleBuilding*)DIPS.DIPS[ID];
		CopyUnits(GFriends,&dip->Friends,0,10000,1,0);
	}
}

CEXPORT
void SetDipTalkRadius(word ID, int R){
	if(ID<DIPS.NDIPS && R>500){
		DIP_SimpleBuilding* dip=(DIP_SimpleBuilding*)DIPS.DIPS[ID];
		UnitsCenter(&dip->BigZone,&dip->CentralGroup,R);		
	}
}

CEXPORT
void SetDipEasyTalk(word ID, bool State){
	if(ID<DIPS.NDIPS){
		DIP_SimpleBuilding* dip=(DIP_SimpleBuilding*)DIPS.DIPS[ID];
		dip->EasyTalk=State;		
	}
}

CEXPORT
word AddDipCenter(char* FileID, char* Dip, char* GDance, char* ZDance){
	char FID[128];
	sprintf(FID,"%s00",FileID);	
	return DIPS.AddDipCenter(FID, Dip, GDance, ZDance);
}
#endif AC_ADDON

word DiplomacySystem::AddDipCenter(char* FileID, char* Dip, char* GDance, char* ZDance){
	DIPS=(BasicDiploRelation**)realloc(DIPS,4*NDIPS+4);
	DIPS[NDIPS]=new DIP_SimpleBuilding;
	DIPS[NDIPS]->InitThisTribe2(FileID, Dip, GDance, ZDance);
	DIPS[NDIPS]->Index=NDIPS;
	DIPS[NDIPS]->DIP=this;
	NDIPS++;
	return NDIPS-1;
};

int SI_DrawMiniMap(int x,int y,int Lx,int Ly,int mx0,int my0);
int SI_DrawOnMap();

class DIP_SimpleBuilding;
tpDrawOnMiniMapCallback* DIP_SimpleBuilding::fDrawOnMiniMap=NULL;
int DIP_SimpleBuilding::FlagFileID=-1;
tpDrawOnMapCallback* DIP_SimpleBuilding::fDrawOnMap=NULL;
int DIP_SimpleBuilding::ArrowFileID=-1;

void DiplomacySystem::SetCallBacks(){
	DIP_SimpleBuilding::fDrawOnMiniMap=SetDrawOnMiniMapCallback(SI_DrawMiniMap);	
	DIP_SimpleBuilding::FlagFileID=GetPictureIndex("Interf3/minimap");
	DIP_SimpleBuilding::fDrawOnMap=SetDrawOnMapCallback(SI_DrawOnMap);
	DIP_SimpleBuilding::ArrowFileID=GetPictureIndex("Interf3/arrow");
};



//
bool gInit=false;
int gFear[256];
int stDangTime=0;
short* stDang[8]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
short* stDangH[8]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
int frDangTime=0;
short* frDang[8]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
int psDangTime=0;
short* psDang[8]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
//
int KatapultPolkID[8];
//
void DiplomacySystem::ResearchMap(){
	gInit=false;
	stDangTime=0;
	frDangTime=0;
	psDangTime=0;
	for(int i=0;i<8;i++){
		//
		KatapultPolkID[i]=-1;
		//
		if(stDang[i]){
			free(stDang[i]);
			stDang[i]=NULL;
		}
		if(stDangH[i]){
			free(stDangH[i]);
			stDangH[i]=NULL;
		}
		if(frDang[i]){
			free(frDang[i]);
			frDang[i]=NULL;
		}
		if(psDang[i]){
			free(psDang[i]);
			psDang[i]=NULL;
		}
	}
	//
	SetCallBacks();

	memset(StartPopulation,0,sizeof StartPopulation);
	BusyPageIndex=-1;
	NWalk=0;
	WSN=NULL;
	WID=NULL;
	int N=GetAmountOfGroups();
	for(int i=0;i<N;i++){
		char* GName=GetGroupName(i);
		int len=strlen(GName);		
		SettlementInfo* info=(SettlementInfo*) GetExGroupInfo(GetGroupHandle(i),1,0);
		if(info){
			DIPS=(BasicDiploRelation**)realloc(DIPS,4*NDIPS+4);
			DIP_SimpleBuilding* dip=new DIP_SimpleBuilding;
			DIPS[NDIPS]=dip;
			DIPS[NDIPS]->Index=NDIPS;
			DIPS[NDIPS]->DIP=this;
			dip->Load(GName,info);
			//DIPS[NDIPS]->InitThisTribe(GName+pp,GName,filename);			
			NDIPS++;			
		}else
		if(GName[0]=='$'&&GName[1]=='D'&&GName[2]=='S'&&GName[3]=='E'&&GName[4]=='T'){
			// dip settlement
			// село добывающее ресурсы
			char filename[128],prefix[128];
			int flen=0,plen=0;
			int pp=0;	// prefix position
			byte state=0;
			for(int i=6;i<len;i++){
				if(state>1) break;
				byte chr=GName[i];
				if(chr=='-'){
					state++;
				}else{
					if(state==0){
						filename[flen]=chr;
						flen++;
					}else
					if(state==1){
						pp=i;
						break;
						//prefix[plen]=chr;
						//plen++;
					}
				}
			}
			filename[flen]=1;
			filename[flen+1]=1;
			filename[flen+2]=0;
			prefix[plen]=0;
			DIPS=(BasicDiploRelation**)realloc(DIPS,4*NDIPS+4);
			DIPS[NDIPS]=new DIP_SimpleBuilding;
			DIPS[NDIPS]->Index=NDIPS;
			DIPS[NDIPS]->DIP=this;
			DIPS[NDIPS]->InitThisTribe(GName+pp,GName,filename);			
			NDIPS++;			
		}else
		if(GName[0]=='$'&&GName[1]=='D'&&GName[2]=='I'&&GName[3]=='P'&&GName[4]=='-'){
			//group for diplomacy
			char sname[256];
			strcpy(sname,GName+5);
			sname[strlen(sname)-5]=0;
			if(sname[0]=='S'&&sname[1]=='B'&&sname[2]=='L'&&sname[3]=='D'){
				DIPS=(BasicDiploRelation**)realloc(DIPS,4*NDIPS+4);
				DIPS[NDIPS]=new DIP_SimpleBuilding;
				DIPS[NDIPS]->Index=NDIPS;
				DIPS[NDIPS]->DIP=this;
				DIPS[NDIPS]->InitThisTribe(GName+strlen(GName)-5,GName,sname);
				NDIPS++;
			};
		}else 
		if(GName[0]=='$'&&GName[1]=='U'&&GName[2]=='N'&&GName[3]=='I'&&GName[4]=='-'){
			char ccc[128];
			strcpy(ccc,GName);
			ccc[1]='Z';
			ccc[2]='O';
			ccc[3]='O';
			GAMEOBJ UNI;
			GAMEOBJ ZON;
			if(RegisterUnits(&UNI,GName)){
				RegisterZone(&ZON,ccc);
				ZooBirthZone ZBZ;
				memset(&ZBZ,0,sizeof ZBZ);
				ZBZ.Zone=ZON;
				int NU=GetNUnits(&UNI);
				OneUnit OU;
				int nur=0;
				if(NU){
					for(int p=0;p<NU;p++){
						if(GetUnitInfo(&UNI,p,&OU)){
							ZBZ.NIndex[nur]=OU.NIndex;
							nur++;
						};
					};
					if(nur){
						for(int i=nur;i<8;i++)ZBZ.NIndex[i]=ZBZ.NIndex[i%nur];
						ZBZ.Type=ZBZ.NIndex[0];
						AddZooBirthZone(&ZBZ);
						ZooGroup ZG;
						memset(&ZG,0,sizeof ZG);
						ZG.Type=ZBZ.NIndex[0];
						ZG.Group=UNI;
						ZG.MaxAmount=2+GetRND(4);
						ZG.MotionFrequency=700;
						if(GetGrpCenter(&UNI,&ZG.xc,&ZG.yc)){
							AddNewZooGroup(&ZG);
						};
					};
				};
			};
		};
	};
	GAMEOBJ UTP;
	UTP.Type='UTYP';
	UTP.Serial=0;
	memset(StartPopulation,0,sizeof StartPopulation);
	for(i=0;i<1024;i++){
		OneUnit OU;
		if(GetUnitGlobalInfo(i,&OU)){
			if(OU.NIndex<2048)StartPopulation[OU.NIndex]++;
		};
	};
	for(i=0;i<2048;i++)if(StartPopulation[i]){
		StartPopulation[i]>>=2;
		if(!StartPopulation[i])StartPopulation[i]=1;
	};
	DangerMap=znew(short,GetNZones());
	DMSize=GetNZones();
	memset(DangerMap,0,GetNZones()<<1);
};
void DiplomacySystem::PerformCommand(char* Data,int size){
	word* wData=(word*)Data;
	if(wData[0]<1024){
		//standart possibility[DIP_SimpleBuilding]
		DIPS[wData[1]]->PerformCommand(Data,size);
	};
};

#pragma warning(disable : 4035)
int Norma(int x,int y){
	__asm{
		mov		eax,x
		cmp		eax,0
		jge		ggg1
		neg		eax
ggg1:	mov		ebx,y
		cmp		ebx,0
		jge		ggg2
		neg		ebx
ggg2:	mov		ecx,eax
		cmp		eax,ebx
		ja		ggg3
		mov		ecx,ebx
ggg3:	add		ecx,eax
		add		ecx,ebx
		shr		ecx,1
		mov		eax,ecx
	};
};
#pragma warning(default : 4035)

void SetZone(GAMEOBJ* Zone, int X, int Y, int R){
	Zone->Index = X;
	Zone->Serial = Y;
	Zone->Type='@   '-0x202020+R;
}

extern DiplomacySystem DIPS;
CEXPORT
bool SetAIForDiplo(byte nat, int x, int y){
	return DIPS.SetAI(nat,x,y);
}

bool DiplomacySystem::SetAI(byte nat, int x, int y){
	if(nat>=7) return false;
	if(!AI_set[nat]){
		AIX[nat]=x;
		AIY[nat]=y;
		AI_set[nat]=true;
	}
	return true;
};
word CurDip=0;
CIMPORT bool DetArcher(word NIndex);
CIMPORT void AddXYPulse(int x,int y);
DLLIMPORT void LimitUnitsMotion(GAMEOBJ* Grp);
//
bool FreezeDipSysProcess;
CEXPORT bool GetFreezeDipSysProcess(){
	return FreezeDipSysProcess;
}
CEXPORT void SetFreezeDipSysProcess(bool State){
	FreezeDipSysProcess=State;
}
//
CIMPORT void maiSetGroupID(word ID, byte Type);
CIMPORT void maiSetWayArray();
CIMPORT void maiSetTargets(word* Targets);
CIMPORT void maiSetDangerMap(int* DangerMap);
CIMPORT int maiGetDangIteration();
CIMPORT int maiGetDangFear();
//
CIMPORT bool GetVit2Alex();
CIMPORT word GetDirEx(int dx,int dy);
CIMPORT bool isAttBuild(OneObject* OB);
//
CEXPORT void SetMinEnemy(int N);
//
CEXPORT bool CorrectPosition(byte NI, int& x, int& y);

//CIMPORT bool GiveHeroOrder( byte NI, bool& Retreat, int& MoveArea );
//
void DiplomacySystem::Process(){
	if(FreezeDipSysProcess) return;
	if(!FriendAI)
	{
		FriendAI=true;
		for(int i=0;i<NDIPS;i++){
			int x,y;
			if(DIPS[i]->Owner==7&&GetGrpCenter(&DIPS[i]->CentralGroup,&x,&y)){
				byte clr=0xFF;
				int MinR=10000;
				for(int c=0;c<7;c++)if(AI_set[c]){
					int r=Norma(AIX[c]-x,AIY[c]-y);
					if(r<MinR){
						MinR=r;
						clr=c;
					}
				}
				if(clr!=0xFF){
					word Data[8];
					Data[0]=1;// Передать племя AI
					Data[1]=i;//Index of tribe
					Data[2]=clr;//color of player
					Data[3]=0xFFFF;//Index of shaman
					SendDipCommand((char*)Data,8);
				}
			}
		}
	}

	RunPF(260,"ProcessTribe");
	/*
	if(NDIPS){
		CurDip=(CurDip+1)%NDIPS;
		DIPS[CurDip]->ProcessTribe();
	}
	*/
	
	for(int i=0;i<NDIPS;i++){
		RunPF(262+i,"Tribe");
		DIPS[i]->ProcessTribe();
		StopPF(262+i);
	}
	
	StopPF(260);

	RunPF(261,"merchants");
	//maiSetGroupID(0xFFFF,0);
	// processing walking units
	OneUnit OU;
	for(int i=0;i<NWalk;i++){
		bool erase=1;
		if(GetUnitGlobalInfo(WID[i],&OU)){
			if(WSN[i]==OU.Serial){
				erase=0;
				if(!OU.Busy){
					int xx=OU.x+GetRND(8192)-4096;
					int yy=OU.y+GetRND(8192)-4096;
					OBJ_SmartSendTo(WID[i],xx,yy,0,0,128+16,0);
				};
			};
		};
		if(erase){
			if(i<NWalk-1){
				memcpy(WID+i,WID+i+1,(NWalk-i-1)<<1);
				memcpy(WSN+i,WSN+i+1,(NWalk-i-1)<<1);
			};
			i--;
			NWalk--;
		};
	};
	// global group init
	if(!gInit){
		gInit=true;
		int fe=maiGetDangFear();		
		for(int j=0;j<256;j++)gFear[j]=fe;
	}
	// processing storms
	GetRND(stDangTime+1);
	GetRND(GetGlobalTime()+1);

	if(GetGlobalTime()>stDangTime){
		stDangTime=GetGlobalTime()+300;
		int NI=GetRND(18);
		int it=maiGetDangIteration();
		if(NI<8){
			if(stDangH[NI]){
				CreateDangerMapForStormHard(NI,stDangH[NI],GetNZones(),gFear,it);
				//CreateDangerMapForStorm(NI,stDang[NI],GetNZones(),gFear,it);
			}
		}else{
			NI&=7;
			if(NI<8){
				if(stDang[NI]){
					CreateDangerMapForStorm(NI,stDang[NI],GetNZones(),gFear,it);
				}
			}			
		}		
	}
	//
	bool CannonSupport[8];
	memset(CannonSupport,0,sizeof CannonSupport);
	//
	addrand(NStorms);
	for(i=0;i<NStorms;i++){
		
		addrand(8);

		StormGroup* Grp=STORMS+i;		
		GAMEOBJ* Group=&Grp->Group;
		GAMEOBJ* StrGrp=&Grp->StrGrp;
		int top=0xFFFF;
		DeleteHiddenUnits(StrGrp);
		//
		//Grp->Difficulty=0;
		LimitUnitsMotion(Group);
		//		
		Polk* DP=NULL;
		if(GetVit2Alex()&&Group->Type=='POLK'){
			DP=GetPolkByIndex(Group->Index);
		}
		int N=CleanGroup(Group)+CleanGroup(StrGrp);
		if(N||DP){
			int T0=GetGlobalTime();
			//SetUnitsState(Group,1,0,0,0);
			
			if(Grp->StormID!=0xFFFF){
				// штурм
				OneUnit u;
				GetUnitGlobalInfo(Grp->StormID,&u);

				int xc,yc;
				if(GetGrpCenter(&Grp->Group,&xc,&yc)){
					Grp->Top=GetTopZone(xc,yc);
				}

				if(u.Index!=0xFFFF && u.Serial==Grp->StormSN){
					// процесс штурма
					int x=u.x, y=u.y;
					GetBuildingEntryPoint(&x, &y, u.Index, 64);
					
					GAMEOBJ Z;
					Z.Type='@   '-'   '+1000;
					Z.Index=x;
					Z.Serial=y;

					int NIn=0;
					GetBuildingOposit(u.Index,NIn);
					if(NIn>8){						
						NIn>>=1;
						if(Grp->Difficulty<3) NIn>>=1;
					}
					NIn|=1;

					int NStr=GetNUnits(StrGrp);
					NIn-=NStr;
					
					if(NIn>0){
						//Grp->StormID=0xFFFF;												
						N=GetNUnits(Group);
						//OneUnit su;
						for(int id=0;id<N&&id<NIn;id++){
							//GetUnitInfo(Group,id,&su);
							//if(su.Index!=0xFFFF){
								//InviteUnitIntoBuilding(OU.Index,u.Index,0);												
								//InsertUnitToGroup(Group,StrGrp,id);
								//DeleteUnitFromGroup(Group,id);								
							RemoveNearestUnit(Group,StrGrp,x,y,1000);								
							//}
						}
					}

					AttackEnemyInZone2(Group,&Z,Grp->Owner);					

				}else{
					// штурм удался
					if(u.Index!=0xFFFF) DieUnit(u.Index);
					Grp->StormID=0xFFFF;
					RemoveGroup(StrGrp,Group);
					Grp->LastTop=0xFFFF;
				}
				
			}else{
				
				addrand(190);

				// подготовка к штурму
				DWORD prp=GetCurrentPurpose(Group);
				int t=T0-Grp->LastMoveTime;
				if(prp==0&&t>120||t>5156){

					addrand(212);
					Grp->LastMoveTime=T0;
					
					RemoveGroup(StrGrp,Group);
					if(DP){
						maiSetGroupID(Group->Index,3);
					}else{
						maiSetGroupID(Group->Index,1);
					}


					int xc=-1,yc;					
					if(DP){						
						DP->GetPolkFrontCenter(xc,yc);
					}else{
						GetGrpCenter(Group,&xc,&yc);
					}
					if(xc>=0){
						int top=GetTopZone(xc,yc);
						Grp->Top=top;
						if(top>=0xFFFE){
							OneUnit UN;
							GetUnitInfo(Group,0,&UN);
							xc=UN.x;
							yc=UN.y;
							top=GetTopZone(xc,yc);
						}
						if(top>=0&&top<GetNZones()){

							byte Owner=Grp->Owner;
							
							bool Retreat=false;
							int MoveArea=0xFFFF;
							bool atHero=false;//GiveHeroOrder(Owner,Retreat,MoveArea);

							if(false){

								int dx,dy;
								if(GetTopZRealCoor(MoveArea,&dx,&dy)){

									int r=GetZonesDist(top,MoveArea)*64;

									bool Attack=false;
									if( !Retreat && r<700 ){
										GAMEOBJ Z;
										Z.Index=xc;
										Z.Serial=yc;
										Z.Type='@   '-'   '+400;
										Attack=AttackEnemyInZone2(&Grp->Group,&Z,Owner);
										if(Attack) continue;
									}


									if( r>200 ){
										if(Retreat)
											SGP_MoveToPoint(Owner,Group,dx,dy,512,0,0,32); //32
										else
											SGP_MoveToPoint(Owner,Group,dx,dy,512,0,0,32); //32+1
									}
									
								}
								continue;

							}

							bool Attack=false;

							GAMEOBJ Z;
							Z.Index=xc;
							Z.Serial=yc;
							Z.Type='@   '-'   '+700;
							Attack=AttackEnemyInZone2(&Grp->Group,&Z,Owner);
							if(Attack) 
								continue;

							word BID=MakeOneShotToBuild(&Z,&Grp->Group,Owner);
							if(BID!=0xFFFF){
								continue;
							}

							DWORD prp=GetCurrentPurpose(&Grp->Group);
							if(prp!=0)
								continue;


							short* Dang;
							word IDS[4096];
							int N=GetNZones();
							if(Grp->Difficulty>1){
								if(!stDangH[Owner]){
									stDangH[Owner]=(short*)malloc(GetNZones()<<1);
								}
								Dang=stDangH[Owner];
							}else{
								if(!stDang[Owner]){
									stDang[Owner]=(short*)malloc(GetNZones()<<1);
								}
								Dang=stDang[Owner];
							}

							int DM[2048];
							memset(&DM,0,sizeof DM);
							for(int d=0;d<2048&&d<N;d++) DM[d]=Dang[d];
							SetDangerMap(DM);
							maiSetDangerMap(DM);

							bool find=false;
							CannonGroup* Can=NULL;	// cannon to defend
							int Force=2000;

							word DST=0xFFFF;
							int zf=0xFFFF;

							word StrmNIndex=0xFFFF;
							int n=GetNUnits(Group);
							addrand(n);
							for(int i=0;i<n;i++){
								OneUnit OU;
								if(GetUnitInfo(Group,i,&OU)){
									StrmNIndex=OU.NIndex;
									addrand(StrmNIndex);
									break;
								}
							}

							int maxdang;

							int NMin=n>>(5-Grp->Difficulty);
                            if(DP) NMin=Grp->Difficulty*3;
							SetMinEnemy(NMin);
							CreateTopListEnArmy(IDS,Owner,NMin);
							maiSetTargets(IDS);

							zf=FindNextZoneOnTheSafeWayToObject(top,Dang,IDS,&maxdang,100,&DST);
							addrand(zf);
							if(zf>=0xFFFE){
								addrand(Grp->Difficulty);
								if(Grp->Difficulty>0){
									Grp->Difficulty--;
								}else{
									if(!DP){
										UnitsGroup* UG=GetUnitsGroup(Group);
										if(UG){
											for(int i=0;i<UG->N;i++){
												OneObject* OB=GetOBJ(UG->IDS[i]);
												if(OB&&OB->Serial==UG->SNS[i]&&isAttBuild(OB)){
													//AddFirers(Group,Grp->Owner);
													addrand(123238);
													break;
												}
											}
										}											
									}
								}
								continue;
							}else{
								if(Grp->Difficulty<4){
									if(GetRND(8)==1){
										Grp->Difficulty++;
									}
								}
								addrand(Grp->Difficulty);
							}

							maiSetWayArray();

							if(zf<0xFFFE){
								int tx=-1;
								int ty=-1;
								
								OneUnit OU;
								OU.Index=0xFFFF;
								if(DST!=0xFFFF&&GetUnitGlobalInfo(DST,&OU)){								
									tx=OU.x;
									ty=OU.y;
								}

								addrand(DST);
								addrand(tx);
								addrand(ty);
								
								if(Grp->SingleUnitMotion){
									UnitsGroup* UG=GetUnitsGroup(Group);
									for(int i=0;i<UG->N;i++){
										OneObject* OB=GetOBJ(UG->IDS[i]);
										if(OB&&!OB->Sdoxlo&&OB->Serial==UG->SNS[i]){
											OBJ_SendTo(OB->Index,tx,ty,16+1,0);
										}
									}

									addrand(1256);
									continue;
								}

								int r=GetTopDist(xc,yc,tx,ty);
								
								addrand(r);
								if(r<25){
									int n=GetNUnits(Group);
									for(int i=0;i<n;i++){
										OneUnit OU;
										if(GetUnitInfo(Group,i,&OU)){
											int rr=GetTopDist(OU.x,OU.y,tx,ty);
											if(rr<r){
												r=rr;
												addrand(r);
											}											
										}
									}
								}
								
								if(!find){
									if(zf!=top&&r>3){
										int x,y;

										GetTopZRealCoor(zf,&x,&y);
										addrand(x);
										addrand(y);
									
										addrand(CannonSupport[Owner]);
										if(!CannonSupport[Owner]){
											CannonSupport[Owner]=true;
											Polk* KP=GetPolkByIndex(KatapultPolkID[Owner]);
											if(KP){
												KP->MovePolkTo(x,y);
											}
										}

										addrand(Attack);
										if(!Attack){
											DWORD prp=GetCurrentPurpose(Group);
											if(prp==0){
												if(DP){
													//GetTopZRealCoor(Way[NWayPoint-1],&x,&y);// Alexander insert
													DP->MovePolkTo(x,y,GetDirEx(x-xc,y-yc));
												}else{
													addrand(x);
													SGP_MoveToPoint(Owner,Group,x,y,GetDirEx(x-xc,y-yc),0,0,32+1);
												}
											}
										}
									}
								}

							}			
							addrand(top);
							Grp->LastTop=top;
						};
					};
				};
			};
		}
	}
	// processing cannons
	for(i=0;i<NCannons;i++){
		CannonGroup* Grp=CANNONS+i;
		GAMEOBJ* Group=&Grp->Group;
		int top=0xFFFF;
		int N=CleanGroup(Group);		
		if(N){
			SetUnitsState(Group,1,0,0,0);
			//
			int xc,yc;
			if(GetGrpCenter(Group,&xc,&yc)){
				
				GAMEOBJ Z;
				SetZone(&Z,xc,yc,1000);
				Grp->DefForce=GetAlliesForce(&Z,Grp->Owner);

				top=GetTopZone(xc,yc);
				if(top>=0&&top<GetNZones()){

					for(int j=0;j<NCannons;j++) if(j!=i){
						CannonGroup* Allies=CANNONS+j;
						if(GetNUnits(&Allies->Group)){
							int dist=GetZonesDist(top,Allies->Top);
							if(dist<10){
								RemoveGroup(&Allies->Group,Group);
							}
						}
					}
				
					if(CheckIfNotBusy(Group)){
						int ax=Grp->ArtDepoX;
						int ay=Grp->ArtDepoY;
						byte Owner=Grp->Owner;

						short Dang[4096];
						int Fear[256];
						for(int j=0;j<256;j++)Fear[j]=3;
						CreateDangerMapWithoutPeasants(Owner,Dang,GetNZones(),Fear,4);
						if(Grp->LastTop!=0xFFFF) Dang[Grp->LastTop]+=4000;

						/*
						int DM[2048];
						memset(DM,0,sizeof DM);
						int NZ=GetNZones();
						for(i=0;i<NZ;i++) DM[i]=Dang[i];
						SetDangerMap(DM);
						*/

						word IDS[2048];
						CreateNonFiredEnemyBuildingsTopList(IDS,Owner);
						int maxdang;
						word DST=0xFFFF;
						
						int zf=FindNextZoneOnTheSafeWayToObject(top,Dang,IDS,&maxdang,3,&DST);
						int dx,dy;
						if(DST!=0xFFFF&&GetTopZRealCoor(zf,&dx,&dy)){
							/*
							word* Way;
							int NWayPoint=GetLastFullWay(&Way);
							if(NWayPoint>2) GetTopZRealCoor(Way[NWayPoint-3],&x,&y);
								else GetTopZRealCoor(zf,&x,&y);
							*/
							word* WAY;
							int NStep=GetLastFullWay(&WAY);
							int DanTop=0xFFFF;
							for(int s=0;s<NStep;s++){
								int DT=WAY[s];
								if(Dang[DT]>1000){
									DanTop=DT;
								}
							}

							int destnorm=10000;
							int dnx=-1;
							int dny=-1;
							if(DanTop!=0xFFFF){								
								if(GetTopZRealCoor(DanTop,&dnx,&dny)){
									destnorm=Norma(xc-dnx,yc-dny);
								};
							}else{
								OneUnit u;
								GetUnitGlobalInfo(DST,&u);
								if(u.Index!=0xFFFF){
									destnorm=Norma(xc-u.x,yc-u.y);
								}
							}

							int artnorm=Norma(xc-ax,yc-ay);
							
							if(artnorm>1000 && destnorm<5600){
								//OBJ_AttackObj(ID,DST,128+16,0);
								if(DanTop!=0xFFFF && dnx!=-1){
									ArtAttackGroud(Owner,Group,dnx,dny);
								}else{
									ArtAttackObject(Owner,Group,DST);
								}
							}else{											
								DWORD prp=GetCurrentPurpose(Group);
								if(prp==0){
									SGP_MoveToPoint(Owner,Group,dx,dy,512,200,0,32);
								}

								Grp->LastTop=top;
							}
						}
					};					
				}
			}
		}
		Grp->Top=top;
	}

	int N=HEALERS.GetAmount();
	if(N){
	}else{
		memset(hNatEnable,0,sizeof hNatEnable);
	}

	// processing firers
	GetRND(GetGlobalTime()+1);
	GetRND(frDangTime+1);
	if(GetGlobalTime()>frDangTime){
		frDangTime=GetGlobalTime()+351;
		int NI=GetRND(10);
		int it=maiGetDangIteration();
		if(NI<8){
			if(frDang[NI]){				
				CreateDangerMapForFire(NI,frDang[NI],GetNZones(),gFear,it);
			}					
		}		
	}
	//
	for(i=0;i<NFirers;i++)if(CleanGroup(&FIRERS[i].Group)){
		FiringGroup* Grp=FIRERS+i;
		GAMEOBJ* Group=&Grp->Group;
		LimitUnitsMotion(Group);
		int NU=CleanGroup(Group);
		if(NU){
			//SetUnitsState(Group,1,0,0,0);
			//
			maiSetGroupID(Group->Index,0);
			//
			//if(Group->Index==4995) SelectUnits(Group,0);
			int T0=GetGlobalTime();
			//word BID=Grp->CurrentFiringBuilding;
			word BID=0xFFFF;
			word SNS=Grp->FBSN;
			OneUnit OU;
			GetUnitGlobalInfo(BID,&OU);
			//
			//SetUnitsState(Group,0,0,0,0);
			if(OU.Index!=0xFFFF&&OU.Serial==SNS&&T0-Grp->LastMoveTime>956){
				Grp->LastMoveTime=T0;
				if(OU.Index==0xFFFF || OU.NI==Grp->Owner || CheckIfBuildingIsFired(BID)){
					/*
					int xc,yc;
					if(GetGrpCenter(&Grp->Group,&xc,&yc)){
						GAMEOBJ Z;
						Z.Index=xc;
						Z.Serial=yc;
						Z.Type='@   '-'   '+1200;
						AttackEnemyInZone2(&Grp->Group,&Z,Grp->Owner);
					}
					*/
					Grp->CurrentFiringBuilding=0xFFFF;
				}else{
					GrpAttackObject(Grp->Owner,&Grp->Group,BID);
				}
			}else{
				if(/*CheckIfNotBusy(&Grp->Group)||*/T0-Grp->LastMoveTime>1280){
					Grp->LastMoveTime=T0;					
					SetToInternalResourcesZero(Group);
					int xc,yc;
					if(GetGrpCenter(&Grp->Group,&xc,&yc)){
						int top=GetTopZone(xc,yc);
						if(top>=0xFFFE){
							OneUnit UN;
							GetUnitInfo(Group,0,&UN);
							xc=UN.x;
							yc=UN.y;
							top=GetTopZone(xc,yc);
						}
						if(top>=0&&top<GetNZones()){
							byte Owner=FIRERS[i].Owner;
							
							int xxc=xc;
							int yyc=yc;
							if(CorrectPosition(Owner, xc, yc)){
								SGP_MoveToPoint(Owner,Group,xc,yc,GetDirEx(xc-xxc,yc-yyc),0,0,32+1);
								SetUnitsState(Group,1,0,0,0);
								continue;
							};

							if(!frDang[Owner]){
								frDang[Owner]=(short*)malloc((GetNZones()+1000)*2);
							}							
							short* Dang=frDang[Owner];
							word IDS[4096];
							int DM[2048];
							for(int d=0;d<2048;d++) DM[d]=Dang[d];
							SetDangerMap(DM);							
							maiSetDangerMap(DM);
							/*
							bool find=false;							
							memset(IDS,0xFF,sizeof(IDS));
							for(int f=0;f<NCannons;f++){
								CannonGroup* Grp=CANNONS+f;
								
								int top=Grp->Top;
								if(top<0xFFFE){
									IDS[top]=top;
									find=true;
								}
							}*/
							//if(!find){
								CreateNonFiredEnemyBuildingsTopList(IDS,Owner);
								maiSetTargets(IDS);
							//}
							int maxdang;
							word DST=0xFFFF;
							int zf=FindNextZoneOnTheSafeWayToObject(top,Dang,IDS,&maxdang,3,&DST);							
							maiSetWayArray();
							//
							if(DST!=0xFFFF&&zf<0xFFFE){
								int tx=-1;
								int ty=-1;							
								//if(!find){								
									if(GetUnitGlobalInfo(DST,&OU)){
										tx=OU.x;
										ty=OU.y;
									}
								//}else{
								//	GetTopZRealCoor(DST,&tx,&ty);
								//}
								
								if(tx!=-1){
									int r=GetTopDist(xc,yc,tx,ty);
									
									if(r<10){										
										GAMEOBJ Z;
										Z.Index=tx;
										Z.Serial=ty;
										Z.Type='@   '-'   '+400;
										OneObject* EOB=GetOBJ(DST);
										bool Peasant=false;
										if(EOB){
											Peasant=AttackEnemyPeasantsInZone(Group,&Z,EOB->NNUM);
										}
										if(!Peasant){
											GrpAttackObject(Owner,Group,DST);
											//Grp->CurrentFiringBuilding=OU.Index;
											//Grp->FBSN=OU.Serial;
										}										
										SetUnitsState(Group,0,0,0,0);
									}else{
										GAMEOBJ Z;
										Z.Index=xc;
										Z.Serial=yc;
										Z.Type='@   '-'   '+400;
										if(/*!AttackEnemyInZone2(&Grp->Group,&Z,Owner)&&*/zf!=top&&r>3){
											int x,y;
											
											word* Way;
											int NWayPoint=GetLastFullWay(&Way);
											if(NWayPoint>2) GetTopZRealCoor(Way[NWayPoint-3],&x,&y);
											else{
												//GetTopZRealCoor(zf,&x,&y);
												x=tx;
												y=ty;
											}
											
											DWORD prp=GetCurrentPurpose(Group);
											if(prp==0){												
												SGP_MoveToPoint(Owner,Group,x,y,GetDirEx(x-xc,y-yc),128-GetRND(256),128-GetRND(256),32);
											}

											Grp->LastTop=top;
										}
										SetUnitsState(Group,0,1,0,0);
									};
								};
							}else{
								//AddTomahawks(Group,Grp->Owner,0,0);
							};
						};
					};
				};
			}
		};
	};
	// processing killers
	if(GetGlobalTime()>psDangTime){
		psDangTime=GetGlobalTime()+344;
		int NI=GetRND(10);
		int it=maiGetDangIteration();
		if(NI<8){
			if(psDang[NI]){				
				CreateDangerMapWithoutPeasants(NI,psDang[NI],GetNZones(),gFear,it);
			}					
		}		
	}
	for(i=0;i<NKillers;i++){
		int T0=GetGlobalTime();
		OneUnit OU;		
		KillersGroup* Grp=KILLERS+i;
		GAMEOBJ* Group=&Grp->Group;
		LimitUnitsMotion(Group);
		int NUnits=CleanGroup(Group);		
		if(NUnits&&(/*CheckIfNotBusy(Group)||*/T0-KILLERS[i].LastMoveTime>1164)){

			//SetUnitsState(Group,1,0,0,0);
			Grp->LastMoveTime=T0;
			//
			maiSetGroupID(Group->Index,2);
			//
			int xc,yc;
			if(GetGrpCenter(&KILLERS[i].Group,&xc,&yc)){
				int top=GetTopZone(xc,yc);
				if(top>=0xFFFE){
					OneUnit UN;
					GetUnitInfo(Group,0,&UN);
					xc=UN.x;
					yc=UN.y;
					top=GetTopZone(xc,yc);
				}
				if(top>=0&&top<GetNZones()){
					byte Owner=KILLERS[i].Owner;
					
					//
					//int xxc=xc;
					//int yyc=yc;
					//if(CorrectPosition(Owner, xc, yc)){
					//	SGP_MoveToPoint(Owner,Group,xc,yc,GetDirEx(xc-xxc,yc-yyc),0,0,32+1);
					//	SetUnitsState(Group,1,0,0,0);
					//	continue;
					//};
					//

					word IDS[4096];
					if(!psDang[Owner]){
						psDang[Owner]=(short*)malloc((GetNZones()+1000)*2);
					}							
					short* Dang=psDang[Owner];
					//
					int DM[2048];
					for(int d=0;d<2048;d++) DM[d]=Dang[d];
					maiSetDangerMap(DM);
					//
					word DST=0xFFFF;
					int MinPS=NUnits/4;
					if(MinPS>Grp->LastTop) MinPS=Grp->LastTop;
					//
					SetMinEnemy(MinPS);
					//
					int maxdang;
					int zf=0xFFFF;
					bool FindPeasant=true;
					//
					if(CreatePeasantsTopList(IDS,Owner,MinPS,false)){ //Grp->SeakMine						
						zf=FindNextZoneOnTheSafeWayToObject(top,Dang,IDS,&maxdang,5,&DST);
					}
					if(zf!=0xFFFF){						
						maiSetTargets(IDS);
						maiSetWayArray();
						if(Grp->LastTop<10){
							if(GetRND(5)==1) Grp->LastTop++;
						}
					}else{
						if(Grp->LastTop>2){
							if(GetRND(5)==1) Grp->LastTop--;
							continue;
						}else{
							CreateTopListEnArmy(IDS,Owner,MinPS/2);
							zf=FindNextZoneOnTheSafeWayToObject(top,Dang,IDS,&maxdang,5,&DST);
							FindPeasant=false;
							maiSetTargets(IDS);
							maiSetWayArray();
						}
					}					
					
					if(DST!=0xFFFF){
						if(GetUnitGlobalInfo(DST,&OU)){							
							GAMEOBJ Z;
							Z.Index=xc;
							Z.Serial=yc;
							Z.Type='@   '-'   '+600; //1500
							int r=GetTopDist(xc,yc,OU.x,OU.y);
							if(r<20){
								//SGP_MoveToPoint(7,Group,OU.x,OU.y,512,0,0,1);
								//CancelAllPurposes(&KILLERS[i].Group);
								Z.Index=OU.x;
								Z.Serial=OU.y;
								if(OU.Building){
									//int x,y;
									//GetBuildingEntryPoint(&x,&y,OU.Index,32);
									Z.Type='@   '-'   '+800;
									if(!AttackEnemyInZone2(&KILLERS[i].Group,&Z,Owner)){
										DWORD prp=GetCurrentPurpose(Group);
										if(prp==0){
											SGP_MoveToPoint(7,Group,OU.x,OU.y,GetDirEx(OU.x-xc,OU.y-yc),0,0,32+1);
										}
									}
								}else{
									if(FindPeasant) AttackEnemyPeasantsInZone(&KILLERS[i].Group,&Z,OU.NI);
										else AttackEnemyInZone2(&KILLERS[i].Group,&Z,Owner);
								}
								//KILLERS[i].LastMoveTime=T0;
							}else{
								//int NArm;
								//AttackEnemyInZone2(&KILLERS[i].Group,&Z,Owner)
								//if(!Attack6(&KILLERS[i].Group,&Z,Owner,NArm)){
									//SetToInternalResourcesZero(Group);
									//if(!AttackEnemyInZone2(&KILLERS[i].Group,&Z,Owner)){
										int x,y;
										if(r<12){
											x=OU.x;
											y=OU.y;
										}else{										
											word* Way;
											int NWayPoint=GetLastFullWay(&Way);
											if(NWayPoint>2) GetTopZRealCoor(Way[NWayPoint-3],&x,&y);
											else{
												x=OU.x;
												y=OU.y;
												//GetTopZRealCoor(zf,&x,&y);
											}
										}									
										DWORD prp=GetCurrentPurpose(Group);
										if(prp==0){
											SGP_MoveToPoint(Owner,Group,x,y,GetDirEx(x-xc,y-yc),128-GetRND(256),128-GetRND(256),32);
										}
										//KILLERS[i].LastMoveTime=T0+15;
										//KILLERS[i].LastTop=top;
									//}									
								//}
								SetUnitsState(Group,0,1,0,0);
							};
						};
					}else{
						//AddStorm(Group,Grp->Owner,2);
					}
				};
			};
		};
	};
	// processing tomahawks
	for(i=0;i<NTomahawks;i++){
		int T0=GetGlobalTime();
		TomahawkGroup* Grp=TOMAHAWKS+i;
		int NMen = CleanGroup(&Grp->Group);
		GAMEOBJ* Group=&Grp->Group;
		LimitUnitsMotion(Group);
		if(NMen){
			// add friends
			/*
			for(int j=0;j<NTomahawks;j++)if(i!=j){
				TomahawkGroup* Friend=TOMAHAWKS+j;
				if(Friend->Owner==Grp->Owner&&GetNUnits(&Friend->Group)&&Norma(Grp->x-Friend->x,Grp->y-Friend->y)<1000){
					OneUnit FU;
					GetUnitInfo(&Friend->Group
					RemoveGroup(&Friend->Group, Group);
					CleanGroup(&Friend->Group);
				}
			}
			*/
			// thinking
			if(/*CheckIfNotBusy(&Grp->Group)||*/T0-Grp->LastMoveTime>72){

				//SetUnitsState(Group,1,0,0,0);
				Grp->LastMoveTime=T0;

				int xc,yc;
				if(GetGrpCenter(&Grp->Group,&xc,&yc)){
					Grp->x=xc;
					Grp->y=yc;
					int top=GetTopZone(xc,yc);
					if(top>=0xFFFE){
						OneUnit UN;
						GetUnitInfo(&Grp->Group,0,&UN);
						xc=UN.x;
						yc=UN.y;
						top=GetTopZone(xc,yc);
					}
					if(top>=0&&top<GetNZones()){
						byte Owner=Grp->Owner;
						short Dang[4096];
						int Fear[256];
						word IDS[4096];
						for(int j=0;j<256;j++)Fear[j]=2;
						Fear[1]=100;
						//CreateDangerMap(Owner,Dang,GetNZones(),Fear,3);
						CreateDangerMapForTom(Owner,Dang,GetNZones(),Fear,3);						

						
						int DD[4096];
						for(j=0;j<GetNZones();j++) DD[j]=Dang[j];
						SetDangerMap(DD);
						

						int maxdang;
						
						// looking to enemy force						
						GAMEOBJ Zone;
						SetZone(&Zone,xc,yc,Grp->MaxAttR+300); //400
						int Force=GetGroupForce(&Grp->Group);
						int AliForce=-GetEnemyForce(&Zone,Owner);
						
						bool attacking=true;	// нападаем, отступаем
						bool moving=true;		// перейти в новую топ-зону
						bool thinktwice=false;	// думать часче
						bool retreat=false;		// полное отступление

						// поиск щита
						if(Grp->ShieldID>=0xFFFE||!GetNUnits(&STORMS[Grp->ShieldID].Group)){
							memset(IDS,0xFF,sizeof(IDS));
							for(int i=0;i<NStorms;i++){
								StormGroup* GRP=STORMS+i;
								if(GRP->Owner==Grp->Owner){
									int ST=GRP->Top;
									if(GetNUnits(&GRP->Group)+GetNUnits(&GRP->StrGrp)>0 && ST>=0 && ST<0xFFFE){
										IDS[ST]=i;									
									}
								}
							}
							word DST=0xFFFF;
							int zf=FindNextZoneOnTheSafeWayToObject(top,Dang,IDS,&maxdang,5,&DST);
							if(DST!=0xFFFF) Grp->ShieldID=DST;
								else Grp->ShieldID=0xFFFF;
						}
						word DST=0xFFFF;
						if(Grp->ShieldID!=0xFFFF) DST=STORMS[Grp->ShieldID].Top;
						bool findstm=(DST!=0xFFFF);
						
						if(findstm){
							//Force += GetGroupForce(&Grp->Group);
							int sx,sy;
							if(GetTopZRealCoor(DST,&sx,&sy)){
								if(Norma(sx-xc,sy-yc)<800) DST=0xFFFF;
							}
						}

						//word Builds[128];
						//int NBuilds=0;
						//bool building=MakeListOfEnBuildingInZone(Owner,&Zone,Builds,&NBuilds);
						
						SetZone(&Zone,xc,yc,Grp->MaxAttR+200);
						//SetUnitsState(Group,1,0,0,0);
						word BID=0xFFFF;
						if(Grp->Base!=0&&T0-Grp->AttackTime>0&&(AliForce>=Force||GetRND(100)>30)){
							BID=MakeOneShotToBuild(&Zone,&Grp->Group,Owner);
						}
						bool building=false;
						if(BID!=0xFFFF){
							OneObject* BOB=GetOBJ(BID);
							if(BOB&&!(BOB->Sdoxlo||BOB->NNUM==7)&&isUnitAbsorber(BID)){								
								building=true;
								if(Grp->Strelok) moving=false;
								int NIn=0;								
								if(GetBuildingOposit(BID,NIn)){
									//if(!NIn) building=false;
									AliForce-=NIn*5;
									NIn>>=1;
									NIn++;
								}												
								if(NIn<NMen){										
									int NStr=GetRND(GetDiff(Grp->Owner))+1;
									int N=CleanGroup(Group);
									OneUnit u;
									for(int id=0;id<N&&NStr>0;id++){
										GetUnitInfo(Group,id,&u);
										bool ru=(GetRND(300)==0);
										if(u.Index!=0xFFFF&&(u.ChargeProgress||ru)){
											InviteUnitIntoBuilding(BID,u.Index,0);												
											//InsertUnitToGroup(&Grp->Group,&Grp->StrGrp,id);
											DeleteUnitFromGroup(Group,id);
											if(ru) ru=0;
											NStr--;
										}
									}
								}
							}
						};

						int NEnemy=0;
						if(moving){
							if(AliForce>Force-(Force>>3)){
								
								if(Grp->Strelok){
									//SetUnitsState(Group,1,0,1,0);
								}else{
									//SetUnitsState(Group,1,0,1,0);
								}
								
								SetZone(&Zone,xc,yc,Grp->MaxAttR+200);
								NEnemy=AttackEnemyInZone2(&Grp->Group,&Zone,Owner);
								if(NEnemy){
									//moving=false;
									Grp->LastThinkTime=T0;
									/*
									if(T0-Grp->AttackTime>0&&Grp->Strelok){
										Grp->AttackTime=T0+196;
									}
									*/
								}
							}else{
								// recession
								if(T0-Grp->LastThinkTime>100){
									thinktwice=true;
									attacking=false;
									if(building) retreat=true;
								}else{
									//SetUnitsState(Group,1,0,1,0);
									if(!Grp->Strelok) AttackEnemyInZone2(&Grp->Group,&Zone,Owner);
									moving=false;
								}
							}
						}
						
						if(Grp->LastTop!=0xFFFF&&!thinktwice) Dang[Grp->LastTop]+=4000;
						//if(attacking&&Grp->LastTop!=0xFFFF) Dang[Grp->LastTop]+=4000;

						// looking for nearest firest
						//bool find=false;	// поиск топ-зоны для прикрытия
						/*
						memset(IDS,0xFF,sizeof(IDS));
						for(int f=0;f<NFirers;f++){
							FiringGroup* SGrp=FIRERS+f;
							int xc,yc;
							if(SGrp->Owner==Owner&&GetNUnits(&SGrp->Group)){								
								int t=SGrp->LastTop;
								if(t!=0xFFFF){
									IDS[t]=f;
									find=true;
								}
							}
						}*/
						
						
						if(thinktwice&&T0-Grp->AttackTime>0){
							SetZone(&Zone,xc,yc,Grp->MaxAttR+500);
							//if(Grp->Strelok) //SetUnitsState(Group,1,0,0,0);
							//	else //SetUnitsState(Group,1,0,0,1);
							int NArm;
							bool at=AttackByTomahawks(&Grp->Group,&Zone,Owner,NArm);
							
							if(!at){
								Grp->LastMoveTime=T0;
							}
						}

						bool SearchEnemy=false;
						if(moving){
								
							//int r=GetTopDist(xc,yc,tx,ty);

							//if(!find){
								if(attacking && T0-Grp->AttackTime>0){
									CreateTopListEnArmy(IDS,Owner,Grp->LastNEnemy);
									Grp->LastNEnemy>>=1;
									SearchEnemy=true;
								}else{
									if(DST==0xFFFF){
										CreateFriendBuildingsTopList(IDS,Owner);
										IDS[top]=0xFFFF;
										if(Grp->LastTop!=0xFFFF) IDS[Grp->LastTop]=0xFFFF;
									}
								}
							//}
							
							int zf;
							if(DST==0xFFFF||attacking) zf=FindNextZoneOnTheSafeWayToObject(top,Dang,IDS,&maxdang,5,&DST);
								else zf=DST;

							if(SearchEnemy&&DST!=0xFFFF&&Grp->LastNEnemy<(NMen>>2)){
								Grp->LastNEnemy<<=1;
							}

							Grp->LastMoveTime=T0;

							if(DST!=0xFFFF&&zf!=top){
								int dx,dy;
								//bool AttEnm=false;
								
								/*
								if(SearchEnemy){
									OneUnit OU;
									GetUnitGlobalInfo(DST,&OU);
									if(OU.Index!=0xFFFF){
										int r=Norma(xc-OU.x,yc-OU.y);
										if(r<Grp->MaxAttR+300) thinktwice=true;
									}
								}
								*/
								if(GetTopZRealCoor(zf,&dx,&dy)){
									if(NEnemy==0&&(GetGroupCharge(Group)||!attacking)){
										if(!thinktwice){
											word* Way;
											int NWayPoint=GetLastFullWay(&Way);
											if(NWayPoint>2) GetTopZRealCoor(Way[NWayPoint-3],&dx,&dy);						
											//if(NWayPoint<5) thinktwice=true;
										}
										DWORD prp=GetCurrentPurpose(Group);
										if(prp==0){
											SGP_MoveToPoint(Owner,Group,dx,dy,512,128-GetRND(256),128-GetRND(256),32);
										}
										//AddXYPulse(dx,dy);
										//ClearAZones();
										//CreateAZone("Move",dx,dy,128);
									}
									if(moving&&attacking) Grp->LastTop=top;
										else Grp->LastTop=0xFFFF;									
									/*
										if(Grp->ShieldID!=0xFFFF){
										StormGroup* STR=STORMS+Grp->ShieldID;
									}
									*/
									
									/*
									if(findstm){
										int sx,sy;
										if(GetTopZRealCoor(DST,&sx,&sy)){
											if(Norma(sx-xc,sy-yc)>1000) retreat=false;
										}
									}
									*/
									if(thinktwice){ 
										Grp->LastMoveTime=T0-62;
									}else
									if(retreat && !findstm && T0-Grp->AttackTime>0){
										Grp->AttackTime=T0+256;
									}else 
										Grp->LastMoveTime=T0;
								}

							}else{
								if(Grp->Strelok){
									//SetUnitsState(Group,1,0,0,0);
								}else{
									//SetUnitsState(Group,1,0,1,0);
								}								
								AttackEnemyInZone2(&Grp->Group,&Zone,Owner);								
							}
						}else{
							Grp->LastMoveTime=T0;
						}

						/*
						if(thinktwice&&T0-Grp->AttackTime>0){
							SetZone(&Zone,xc,yc,Grp->MaxAttR+500);
							if(Grp->Strelok) SetUnitsState(Group,1,0,0,0);
								else SetUnitsState(Group,1,0,0,1);
							int NArm;
							bool at=AttackByTomahawks(&Grp->Group,&Zone,Owner,NArm);
							
							if(!at){
								Grp->LastMoveTime=T0;
							}
						}
						*/

						if(!moving) Grp->LastMoveTime=T0-60;					

					}
				}
			}
		}
	}
	for(i=0;i<6;i++)SetResource(7,i,1000000);
	ProcessZoology();
	StopPF(261);
};

bool DiplomacySystem::CreateContactsList(bool first){
	DString DST;
	DString One;
	if(first){
		DST.ReadFromFile("Dip\\ContactsList.cml");
		DST.Replace("$TRIBELIST$",GetTextByID("$TRIBELIST$"));
	};
	DST.Add("<FRM1>\n");
	int pp=0;
	for(int i=0;i<NDIPS;i++)if(DIPS[i]->Owner==GetMyNation()){
		DIP_SimpleBuilding* DSB=(DIP_SimpleBuilding*)DIPS[i];
		One.ReadFromFile("Dip\\ContactsListLine.cml");
		char ccc[128];
		strcpy(ccc,GetTextByID(DSB->NameOfTribe));
		if(DSB->Busy)strcat(ccc,"[B]");
		One.Replace("$TRIBELIST$",ccc);
		One.Replace("$IDX$",pp+1);
		One.Replace("$IDXX$",pp);
		One.Replace("$TRIBEINFO$",i);
		DST+=One;
		pp++;
	};
	DST.Add("\n<FRM1>");
	DST.WriteToFile("Dip\\tmp.cml");
	BusyPageIndex=-1;
	return pp!=0;
};
void DiplomacySystem::AddWalker(word ID){
	if(ID!=0xFFFF){
		OneUnit OU;
		if(GetUnitGlobalInfo(ID,&OU)){
			WID=(word*)realloc(WID,NWalk*2+2);
			WSN=(word*)realloc(WSN,NWalk*2+2);
			WID[NWalk]=ID;
			WSN[NWalk]=OU.Serial;
			NWalk++;
		};
	};
};
void DiplomacySystem::AddCannon(GAMEOBJ* Src,byte Owner){
	int N=CleanGroup(Src);
	if(N){
		OneUnit u;
		GetUnitInfo(Src,0,&u);
		if(u.Index!=0xFFFF){
			CANNONS=(CannonGroup*) realloc(CANNONS,sizeof(CannonGroup)*(NCannons+1));
			CannonGroup* Grp=CANNONS+NCannons;

			Grp->ArtDepoX=u.x;
			Grp->ArtDepoY=u.y;
			
			GAMEOBJ* Group=&Grp->Group;
			RegisterDynGroup(Group);
			RemoveGroup(Src,Group);
			Grp->Owner=Owner;

			int top=0xFFFF;
			int xc,yc;
			if(GetGrpCenter(Group,&xc,&yc)){
				top=GetTopZone(xc,yc);
			}
			Grp->Top=top;
			Grp->LastTop=0xFFFF;
			
			Grp->DefForce=0;

			NCannons++;
		}
	}
};
void DiplomacySystem::Cannon2Alex(GAMEOBJ* Src, byte Owner){
	if(!(Src&&Src->Type=='UNIT'/*&&CleanGroup(Src)*/)) return;
	
	// katapult	
	if(KatapultPolkID[Owner]==-1){
		KatapultPolkID[Owner]=CreatePolk();
	}
	Polk* KP=GetPolkByIndex(KatapultPolkID[Owner]);
	if(KP){
		UnitsGroup* UG=GetUnitsGroup(Src);
		if(UG){
			for(int i=0;i<UG->N;i++){
				OneObject* OB=GetOBJ(UG->IDS[i]);
				if(OB&&OB->Serial==UG->SNS[i]&&!OB->Sdoxlo){
					if(OB->Usage==PushkaID){ //KP->AddCannon(OB)){
						KP->AddCannon(OB);
						UG->IDS[i]=0xFFFF;						
					}
				}
			}
		}
	}	
}
StormGroup* DiplomacySystem::AddStorm(GAMEOBJ* Src, byte Owner, int Diff){
	
	if( VAI() && VAI()->AddSquad(Owner,Src) )
		return NULL;

	if(!(Src&&Src->Type=='UNIT'&&CleanGroup(Src))) return NULL;

	SetUnitsState(Src,1,0,0,0);
	SetAI_Guard(Src,false);

	/*
	// katapult	
	if(KatapultPolkID[Owner]==-1){
		KatapultPolkID[Owner]=CreatePolk();
	}
	Polk* KP=GetPolkByIndex(KatapultPolkID[Owner]);
	if(KP){
		UnitsGroup* UG=GetUnitsGroup(Src);
		if(UG){
			for(int i=0;i<UG->N;i++){
				OneObject* OB=GetOBJ(UG->IDS[i]);
				if(OB&&OB->Serial==UG->SNS[i]&&!OB->Sdoxlo){
					if(KP->AddCannon(OB)){
						UG->IDS[i]=0xFFFF;						
					}
				}
			}
		}
	}
	*/

	//
	if(GetVit2Alex()&&GetRND(3)!=0){		
		StormGroup* Grp=NULL;
		Polk* DP=NULL;
		for(int i=0;i<NStorms;i++){
			Grp=STORMS+i;
			if(Grp && Grp->Owner==Owner && Grp->Group.Type=='POLK'){
				DP=GetPolkByIndex(Grp->Group.Index);
				break;
			}
		}
		if(!DP){
			STORMS=(StormGroup*)realloc(STORMS,(NStorms+1)*sizeof(StormGroup));
			Grp=STORMS+NStorms;
			Grp->Owner=Owner;
			Grp->LastMoveTime=0;
			Grp->StormID=0xFFFF;
			Grp->Difficulty=Diff;
			NStorms++;
			Grp->Group.Type='POLK';
			Grp->Group.Index=CreatePolk();
			DP=GetPolkByIndex(Grp->Group.Index);
		}
		if(DP){			
			UnitsGroup* UG=GetUnitsGroup(Src);
			if(UG){
				for(int i=0;i<UG->N;i++){
					OneObject* OB=GetOBJ(UG->IDS[i]);
					if(OB&&OB->Serial==UG->SNS[i]&&!OB->Sdoxlo){
						if(OB->BrigadeID!=0xFFFF){
							DP->AddBrigadeToPolk(Owner,OB->BrigadeID);
							for(int j=i;j<UG->N;j++){
								OneObject* OBj=GetOBJ(UG->IDS[j]);
								if(OBj&&OBj->Serial==UG->SNS[j]&&OBj->BrigadeID==OB->BrigadeID){
									UG->IDS[j]=0xFFFF;
								}
							}
							//return;
						}else{
							if(DP->AddCannon(OB)){
								UG->IDS[i]=0xFFFF;
								//return;
							}
						}
					}
				}
			}
		}
		//Log.Error("AI Vit2Alex error...");
		//return;
	}

	int N=CleanGroup(Src);
	if(N==0) return NULL;

	STORMS=(StormGroup*)realloc(STORMS,(NStorms+1)*sizeof(StormGroup));
	StormGroup* Grp=STORMS+NStorms;

	Grp->Owner=Owner;
	SetGroupNMASK(&Grp->Group,GetNatNMASK(Owner));

	RegisterDynGroup(&Grp->Group);
	CopyUnits(Src,&Grp->Group,0,10000,0,1);
	RegisterDynGroup(&Grp->StrGrp);
	Grp->StormID=0xFFFF;
	
	Grp->Top=0xFFFF;
	Grp->LastTop=0xFFFF;

	Grp->LastMoveTime=0;

	Grp->CannonGuard=false;

	Grp->Difficulty=2;//Diff;

	NStorms++;

	GetRND(NStorms);

	return Grp;
};
void DiplomacySystem::AddFirers(GAMEOBJ* Src,byte Owner){
	if(!(Src->Type=='UNIT'&&CleanGroup(Src))) return;
	
	SetAI_Guard(Src,false);
	SetUnitsState(Src,1,0,0,0);

	FIRERS=(FiringGroup*)realloc(FIRERS,(NFirers+1)*sizeof FiringGroup);
	RegisterDynGroup(&FIRERS[NFirers].Group);
	CopyUnits(Src,&FIRERS[NFirers].Group,0,10000,0,1);
	FIRERS[NFirers].Owner=Owner;
	FIRERS[NFirers].CurrentFiringBuilding=0xFFFF;
	FIRERS[NFirers].LastMoveTime=GetGlobalTime();
	FIRERS[NFirers].LastThinkTime=GetGlobalTime();

	FIRERS[NFirers].LastTop=0xFFFF;
	NFirers++;
};
void DiplomacySystem::AddHealers(GAMEOBJ* Src,byte Owner){
	if(!(Src->Type=='UNIT'&&CleanGroup(Src))) return;
	SetUnitsState(Src,1,0,0,0);

	HealGroup HG;
	RegisterDynGroup(&HG.Group);
	CopyUnits(Src,&HG.Group,0,10000,0,1);
	HG.Owner=Owner;	
	HG.LastMoveTime=GetGlobalTime();
	HG.LastTop=0xFFFF;
	HEALERS.Add(HG);
};
void DiplomacySystem::AddKillers(GAMEOBJ* Src,byte Owner, bool SeakMine){
	int N=CleanGroup(Src);
	if(N==0) return;

	SetAI_Guard(Src,false);
	SetUnitsState(Src,1,0,0,0);

	KILLERS=(KillersGroup*)realloc(KILLERS,(NKillers+1)*sizeof KillersGroup);	
	RegisterDynGroup(&KILLERS[NKillers].Group);	
	CopyUnits(Src,&KILLERS[NKillers].Group,0,10000,0,1);
	KILLERS[NKillers].Owner=Owner;
	KILLERS[NKillers].LastMoveTime=GetGlobalTime();
	KILLERS[NKillers].LastThinkTime=GetGlobalTime();

	KILLERS[NKillers].LastTop=N/9;
	KILLERS[NKillers].SeakMine=SeakMine;
	NKillers++;
};
void DiplomacySystem::AddTomahawks(GAMEOBJ* Src,byte Owner,word Base,int DriftRadius, int CriticalMass){
	SetAI_Guard(Src,false);
	AddStorm(Src,Owner,2);
	return;

	if(!(Src->Type=='UNIT'&&CleanGroup(Src))) return;
	TOMAHAWKS=(TomahawkGroup*)realloc(TOMAHAWKS,(NTomahawks+1)* sizeof TomahawkGroup);
	TomahawkGroup* Grp=TOMAHAWKS+NTomahawks;
	RegisterDynGroup(&Grp->Group);

	FilterUnitsByCategory(Src,&Grp->Group,2);
//	if(GetNUnits(&Grp->Group)) Grp->Strelok=1;
//		else Grp->Strelok=0;
	Grp->Strelok=1;
	RemoveGroup(Src,&Grp->Group);
	
	GetGrpCenter(&Grp->Group,&Grp->x,&Grp->y);
	Grp->Owner=Owner;
	Grp->Base=Base;
	Grp->DRadius=DriftRadius;
	Grp->CMass=CriticalMass;
	Grp->LastMoveTime=GetGlobalTime();
	Grp->LastThinkTime=GetGlobalTime();
	
	SetUnitsState(&Grp->Group,1,0,0,1);
	Grp->AttackTime=GetGlobalTime();
	Grp->ShieldID=0xFFFF;
	Grp->LastNEnemy=GetTotalAmount0(&Grp->Group)>>1;

	OneUnit OU;
	GetUnitInfo(&Grp->Group,0,&OU);
	if(OU.Index!=0xFFFF){
		Grp->MaxAttR=GetMaxAttRadius(OU.Index);
	}else{
		Grp->MaxAttR=500;
	}

	Grp->LastTop=0xFFFF;
	NTomahawks++;
};
extern DiplomacySystem DIPS;
CEXPORT void Cannon2Alex(GAMEOBJ* Src, byte Owner){
	DIPS.Cannon2Alex(Src,Owner);
}
CEXPORT void AddStorm(GAMEOBJ* Src, byte Owner, int Diff=2){

	//addrand(Src->Index);
	//addrand(Owner);
	//addrand(Diff);

	DIPS.AddStorm(Src,Owner,Diff);
}
CEXPORT void AddStormSingleUnitMotion(GAMEOBJ* Src, byte Owner, int Diff=2){

	//addrand(Src->Index);
	//addrand(Owner);
	//addrand(Diff);

	StormGroup* Grp=DIPS.AddStorm(Src,Owner,Diff);
	if(Grp){
		Grp->SingleUnitMotion=true;
	}
	
}
CEXPORT void AddCannon(GAMEOBJ* Src, byte Owner){
	//DIPS.AddCannon(Src,Owner);
	DIPS.AddStorm(Src,Owner,0);
}
CEXPORT void AddTomahawks(GAMEOBJ* Src,byte Owner,word Base,int DriftRadius, int CriticalMass){
	//DIPS.AddTomahawks(Src,Owner,Base,DriftRadius,CriticalMass);
	DIPS.AddStorm(Src,Owner,0);
}
CEXPORT void AddFirers(GAMEOBJ* Src,byte Owner){
	//DIPS.AddFirers(Src,Owner);
	DIPS.AddStorm(Src,Owner,0);
};
CEXPORT void AddHealers(GAMEOBJ* Src,byte Owner){
	//DIPS.AddHealers(Src,Owner);
	DIPS.AddStorm(Src,Owner,0);
};
CEXPORT void AddPsKillers(GAMEOBJ* Src,byte Owner,bool SeakMine=true){
	//DIPS.AddKillers(Src,Owner,SeakMine);
	DIPS.AddStorm(Src,Owner,0);
};
void DiplomacySystem::AddNewZooGroup(ZooGroup* ZGrp){
	ZOO=(ZooGroup*)realloc(ZOO,(NZOO+1)*sizeof ZooGroup);
	ZOO[NZOO]=*ZGrp;
	NZOO++;
};
void DiplomacySystem::AddZooBirthZone(ZooBirthZone* BZone){
	BZOO=(ZooBirthZone*)realloc(BZOO,(NBZOO+1)*sizeof ZooBirthZone);
	BZOO[NBZOO]=*BZone;
	NBZOO++;
};
CEXPORT
bool AddAnimals(GAMEOBJ* Units,GAMEOBJ* Zone){
	GAMEOBJ UNI=*Units;
	GAMEOBJ ZON=*Zone;
	if(UNI.Type!='UNIT'||!(ZON.Type=='ZONE'||(ZON.Type&0xFF000000)==('@   '-0x202020))) return false;
	ZooBirthZone ZBZ;
	memset(&ZBZ,0,sizeof ZBZ);
	ZBZ.Zone=ZON;
	int NU=GetNUnits(&UNI);
	OneUnit OU;
	int nur=0;
	if(NU){
		for(int p=0;p<NU;p++){
			if(GetUnitInfo(&UNI,p,&OU)){
				ZBZ.NIndex[nur]=OU.NIndex;
				nur++;
			};
		};
		if(nur){
			for(int i=nur;i<8;i++)ZBZ.NIndex[i]=ZBZ.NIndex[i%nur];
			ZBZ.Type=ZBZ.NIndex[0];
			DIPS.AddZooBirthZone(&ZBZ);
			ZooGroup ZG;
			memset(&ZG,0,sizeof ZG);
			ZG.Type=ZBZ.NIndex[0];			
			DIPS.StartPopulation[ZG.Type]+=1+(NU>>2);
			ZG.Group=UNI;
			ZG.MaxAmount=GetRND(6)+2;
			ZG.MotionFrequency=GetRND(400)+500;
			if(GetGrpCenter(&UNI,&ZG.xc,&ZG.yc)){
				DIPS.AddNewZooGroup(&ZG);
			};
		};
	};
	return true;
}
void DiplomacySystem::ProcessZoology(){
	if(DMSize!=GetNZones()){
		free(DangerMap);
		DangerMap=znew(short,GetNZones());
		DMSize=GetNZones();
		memset(DangerMap,0,GetNZones()<<1);
	};
	if(GetRND(100)<10){
		//processing birth
		if(NBZOO){
			int RZoo=GetRND(NBZOO);
			bool born=0;
			ZooBirthZone* ZBZ=BZOO+RZoo;
			int zx=0;
			int zy=0;
			GetZoneCoor(&ZBZ->Zone,&zx,&zy);
			if(zx&&!CheckLandLocking(zx,zy,0,0)){
				GAMEOBJ UTP;
				UTP.Serial=0;
				UTP.Type='UTYP';
				for(int i=0;i<8&&!born;i++){
					UTP.Index=ZBZ->NIndex[i];
					if(UTP.Index<2048){
						int N=GetTotalAmount1(&UTP,7);
						if(N<StartPopulation[UTP.Index]){
							born=1;
							ZooGroup* ZG;
							//search for free zoo group
							for(int i=0;i<NZOO;i++){
								if(!GetTotalAmount0(&ZOO[i].Group))break;
							};
							bool newz=0;
							ZooGroup ZG0;
							if(i<NZOO){
								ZG=ZOO+i;
								memset(&ZG->xc,0,sizeof(ZooGroup)-sizeof(GAMEOBJ));
							}else{
								memset(&ZG0,0,sizeof ZG0);
								RegisterDynGroup(&ZG0.Group);
								newz=1;
								ZG=&ZG0;
							};
							ZG->Attract=1;
							ZG->MaxAmount=GetRND(6)+2;
							ZG->MotionFrequency=GetRND(400)+500;
							//ZG->MaxAmount=3;
							//ZG->MotionFrequency=500;
							ZG->LastMotionTime=GetGlobalTime();
							ZG->Type=UTP.Index;//ZBZ->Type;
							GAMEOBJ ODIN;
							RegisterFormation(&ODIN,"#ODIN");
							CreateObject0(&ZG->Group,&ODIN,&UTP,7,&ZBZ->Zone,GetRND(256));
							GetGrpCenter(&ZG->Group,&ZG->xc,&ZG->yc);
							if(newz)AddNewZooGroup(ZG);
						};
					};
				};
			};
		};
	};
	{
		//processing motion of animals
		ZooGroup* ZG=ZOO;
		int NTZ=GetNZones();
		if(GetRND(100)<10){
			//fear&danger map
			int Fear[256];
			for(int i=0;i<256;i++)Fear[i]=32;
			CreateDangerMap(7,DangerMap,NTZ,Fear,3);
		};
		word IDXS[4096];
		word TYPE[4096];
		memset(IDXS,0xFF,GetNZones()<<1);
		//list of purposes
		for(int i=0;i<NZOO;i++){//if(GetRND(100)<30){
			if(!ZG->Empty){
				int N=GetTotalAmount0(&ZG->Group);
				if(!N)ZG->Empty=1;
				if(!ZG->Empty){
					GetGrpCenter(&ZG->Group,&ZG->xc,&ZG->yc);
					if(N<ZG->MaxAmount){
						int tz=GetTopZone(ZG->xc,ZG->yc);
						if(tz<NTZ){
							IDXS[tz]=i;
							TYPE[tz]=ZG->Type;
						};
					};
				};
			};
			ZG++;
		};
		ZG=ZOO;
		int CTIME=GetGlobalTime();
		for(i=0;i<NZOO;i++){
			if(CheckIfNotBusy(&ZG->Group)){
				if(!ZG->Empty){
					if(ZG->Attract){
						//otstal ot stada
						int tz=GetTopZone(ZG->xc,ZG->yc);
						if(tz<NTZ){
							int IDX=IDXS[tz];
							IDXS[tz]=0xFFFF;
							int DNG;
							word DST;
							int tzn=FindNextZoneOnTheSafeWayToLimitedObject(tz,DangerMap,IDXS,TYPE,ZG->Type,&DNG,0,&DST);
							IDXS[tz]=IDX;
							if(tzn<NTZ){
								ZooGroup* ZG1=ZOO+DST;
								if(Norma(ZG->xc-ZG1->xc,ZG->yc-ZG1->yc)<1000){
									//uniting with other group
									CopyUnits(&ZG->Group,&ZG1->Group,0,1000,1,1);
									ZG->Empty=1;
									ZG1->Attract=0;
								}else{
									int x,y;
									GetTopZRealCoor(tzn,&x,&y);
									RemoveAttackingUnits(&ZG->Group);
									SGP_MakeOneStepTo(7,&ZG->Group,x,y,GetRND(256),0);
								};
							}else{
								int tz=GetTopZone(ZG->xc,ZG->yc);
								if(tz<NTZ){
									OneLinkInfo* List;
									int N=GetListOfNearZones(tz,&List);
									if(N){
										int tz1=List[GetRND(N)].NextAreaID;
										int x,y;
										GetTopZRealCoor(tz1,&x,&y);
										//SGP_MakeOneStepTo(7,&ZG->Group,x,y,GetRND(256),0);
										int N=GetNUnits(&ZG->Group);
										OneUnit OU;
										for(int p=0;p<N;p++){
											if(GetUnitInfo(&ZG->Group,p,&OU)){
												word ID=0xFFFF;
												word SN;
												GetTargetObject(OU.Index, &ID, &SN);
												if(ID!=0xFFFF) OBJ_SendTo(OU.Index,x+GetRND(400)-200,y+GetRND(400)-200,1+128,0);
												else{
													ZG->Attract=0;
													break;
												}
											};
										};
										ZG->LastMotionTime=CTIME;
									};
								};
							};
						};
					}else{						
						// free motion
						if(CTIME-ZG->LastMotionTime>ZG->MotionFrequency){
							// group							
							int tz=GetTopZone(ZG->xc,ZG->yc);
							if(tz<NTZ){
								OneLinkInfo* List;
								int N=GetListOfNearZones(tz,&List);
								if(N){
									//searching for 2 most dangerous zones
									int MAXD1=0;
									int MAXD2=0;
									int Z1=-1,Z2=-1;
									for(int p=0;p<N;p++){
										int tz=List[p].NextAreaID;
										int D=DangerMap[tz];
										if(D>MAXD1){
											MAXD2=MAXD1;
											Z2=Z1;
											MAXD1=D;
											Z1=tz;
										}else{
											if(D>MAXD2){
												MAXD2=D;
												Z2=tz;
											};
										};
									};
									int tz1=-1;
									int nac=0;
									do{
										tz1=List[GetRND(N)].NextAreaID;
										nac++;
									}while((tz1==Z1||tz1==Z2)&&nac<20);
									int x,y;
									GetTopZRealCoor(tz1,&x,&y);
									//SGP_MakeOneStepTo(7,&ZG->Group,x,y,GetRND(256),0);
									int N=GetNUnits(&ZG->Group);
									OneUnit OU;
									for(p=0;p<N;p++){
										if(GetUnitInfo(&ZG->Group,p,&OU)){
											OBJ_SendTo(OU.Index,x+GetRND(400)-200,y+GetRND(400)-200,1+128,0);
										};
									};
									ZG->LastMotionTime=CTIME;
									ZG->MotionFrequency=GetRND(300)+900;
								};
							};
						}else{
							// personal motion
							int N=GetNUnits(&ZG->Group);
							OneUnit OU;
							for(int p=0;p<N;p++){
								if(GetUnitInfo(&ZG->Group,p,&OU)&&!OU.Busy&&!GetRND(30)){									
									OBJ_SendTo(OU.Index,OU.x+GetRND(400)-200,OU.y+GetRND(400)-200,1+128,0);
								};
							};
						}
					};
				};
			};
			ZG++;
		};
	};
};

CEXPORT void GetDipSimpleBuildings(int &NDips, DIP_SimpleBuilding** &Dips){
	NDips=DIPS.NDIPS;
	Dips=(DIP_SimpleBuilding**)DIPS.DIPS;
}
