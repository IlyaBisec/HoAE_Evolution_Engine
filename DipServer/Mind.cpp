#include "CommonDip.h"
#include "UnitsGroup.h"
#include "Mind.h"
#include "Script.h"

int Norma(int x,int y);

Mind Player[8];

int Dang[4096];

void LoadMindData(byte* ptr,int size){
};

void SaveMindData(byte** ptr,int* size){
};

void ClearMindData(){
	for(int i=0;i<8;i++){
		Mind* Pl=Player+i;
		Pl->Free();
	}
}

CEXPORT
void CloseMind(){
	ClearMindData();
}

// Squad begin //
bool Squad::FindTargetZone(short* DangerMap, word* ObjIds, word& TOP, word& TZ, word& Dst){
	int maxdang;
	word DST=0xFFFF;
	
	TZ=(word)FindNextZoneOnTheSafeWayToObject(Top,DangerMap,ObjIds,&maxdang,5,&DST);

	if(DST!=0xFFFF){		

		word* Way;
		int NWayPoint=GetLastFullWay(&Way);
		
		TOP=Way[0];

		//if(NWayPoint>2) TZ=Way[NWayPoint-3];
		
		Dst=0;
		for(int i=1;i<NWayPoint;i++){
			Dst+=(word)GetZonesDist(Way[i-1],Way[i]);
		}

		return true;
	}

	TZ=0xFFFF;
	Dst=0;
	return false;
};

void Squad::MoveToTop(short* DangerMap){
	
	short NullDang[4096];
	if(!DangerMap){
		memset(NullDang,0,sizeof NullDang);
		DangerMap=NullDang;
	}
	
	int xc,yc;
	if(GetGrpCenter(&Group,&xc,&yc)){
		int top=GetTopZone(xc,yc);
		if(top>=0xFFFE){
			OneUnit UN;
			GetUnitInfo(&Group,0,&UN);
			xc=UN.x;
			yc=UN.y;
			top=GetTopZone(xc,yc);
		}
		if(top>=0&&top<GetNZones()){
			int maxdang;
			word DST=0xFFFF;
			word dz=FindNextZoneOnTheSafeWay(top,Top,DangerMap,&maxdang,5);
			if(top==Top) dz=top;	
			
			if(dz<0xFFFF){
				int dx,dy;
				if(GetTopZRealCoor(dz,&dx,&dy)){
					
					// вычисляем направление					
					int dir=512;
					{
						/*int NZ=GetNZones();						
						word IDS[4096];
						for(int i=0;i<NZ;i++){
							if(DangerMap[i]>10000){
								IDS[i]=0;
							}else{
								IDS[i]=0xFFFF;
							}
						}
						//memset(IDS,0xFF,sizeof IDS);

						short Dang[4096];
						memset(Dang,0,sizeof Dang);

						word DST=0xFFFF;

						int ez=FindNextZoneOnTheSafeWayToObject(dz,Dang,IDS,&maxdang,5,&DST);
						int ex,ey;
						if(ez<0xFFFF&&GetTopZRealCoor(ez,&ex,&ey)){
							dir=getDir(ex-dx,ey-dy);						
						}*/					

						
						word TopList[1024];
						TopList[0]=dz;
						int NTopList=1;
						bool Check[4096];
						memset(Check,0,sizeof Check);

						int dirx=0,diry=0;

						while(NTopList){
							NTopList--;
							int t=TopList[NTopList];							

							// add new links top zones
							OneLinkInfo* links=NULL;
							int NL=GetListOfNearZones(t,&links);
							for(int p=0;p<NL;p++){
								int id=links[p].NextAreaID;
								
								if(!Check[id]){
									int x=0,y=0;
									if(GetTopZRealCoor(id,&x,&y)){									
										x-=dx;
										y-=dy;
										int norm=Norma(x,y);

										if(norm<2000/*||!(dirx||diry)*/){
											Check[id]=1;
											TopList[NTopList]=id;
											NTopList++;
											int dang=DangerMap[id];
											if(dang){
												//if(norm>800) dang>>=1;
												//if(norm>1600) dang>>=1;
												dirx+=x*dang;
												diry+=y*dang;
											}
										};
									}
								}

							};
						}

						if(dirx||diry) dir=getDir(dirx,diry);

					}

					if(Brig!=0xFFFF) SelectUnits(&Group,0);
					SGP_MoveToPoint(NI,&Group,dx/*+16-GetRND(32)*/,dy/*+16-GetRND(32)*/,dir,0,0,1);

					SetUnitsState(&Group,1,0,0,0);
				}
			}

			Top=top;
		}
	}
};

// Squad end //

// Mind begin //
Mind::Mind(){
	Clear();
}

Mind::~Mind(){
	Free();
}

void Mind::Clear(){
	memset(this,0,sizeof(*this));
	NI=0xFF;
	CanGuardSqd=0xFFFF;
}

void Mind::Free(){
	if(Sqd) free(Sqd);
	
	Clear();
};

CEXPORT
void ActivateMind(byte NI, int type=0){
	if(NI<7) Player[NI].Init(NI,type);
}

CEXPORT
void DeactivateMind(byte NI){
	if(NI<7) Player[NI].~Mind();
}

word Mind::AddSquad(word BrgID, word NIndex){
	Sqd=(Squad*)realloc(Sqd,sizeof(Squad)*(NSqd+1));
	Squad* NewSqd=Sqd+NSqd;

	memset(NewSqd,0,sizeof(Squad));
	RegisterDynGroup(&NewSqd->Group);
	NewSqd->Index=NSqd;
	NewSqd->NI=NI;
	NewSqd->Brig=BrgID;
	NewSqd->NIndex=NIndex;
	NewSqd->Top=0xFFFF;
	NewSqd->LastTop=0xFFFF;
	
	memset(NewSqd->TarTop,0xFF,sizeof(NewSqd->TarTop));
	memset(NewSqd->TarZone,0xFF,sizeof(NewSqd->TarZone));

	//if(BrgID!=0xFFFF) BrigReformation(NI,BrgID,2);

	//NewSqd->MovingType=1;
		
	return NSqd++;
};

DLLIMPORT bool OBJ_GoToMine(word Index,word DestMine,byte prio,byte OrdType);
DLLIMPORT void SetBuildingsCollector(byte NI,GAMEOBJ* Grp);
DLLIMPORT void AddOneUnitToGroup(GAMEOBJ* Units,OneObject* OB);

void Mind::Init(byte ni, int type){
//	if(NI!=0xFF) ~Mind();
	if(ni>=7) return;
	
	switch(GetDiff(ni)){
	case 0:
		SetPlayerName(ni,GetTextByID("@RMID_EASY"));
		break;
	case 1:
		SetPlayerName(ni,GetTextByID("@RMID_NORMAL"));
		break;
	case 2:
		SetPlayerName(ni,GetTextByID("@RMID_HARD"));
		break;
	case 3:
		SetPlayerName(ni,GetTextByID("@RMID_VERYHARD"));
		break;
	case 4:
		SetPlayerName(ni,GetTextByID("@RMID_IMPOSIBLE"));
		break;
	}

	NI=ni;
	//ENM.
	Type=type;

	RegisterDynGroup(&New);
	RegisterDynGroup(&Panic);

	SetUnitsCollector(ni,&New);
	
	GAMEOBJ Bld;
	RegisterDynGroup(&Bld);
	SetBuildingsCollector(ni,&Bld);

	UnitsGroup* NEW=GetUnitsGroup(&New);

	RegisterDynGroup(&Repaires);

	if(NEW){
		int N=NEW->N;
		word* ID=NEW->IDS;
		word* SN=NEW->SNS;

		// init buildings
		word Prim[1024];
		int NPrim=0;

		//memset(PrimRepareGrp,0xFF,1024<<1);

		int NB=GetNUnits(&Bld);
		UnitsGroup* BG=GetUnitsGroup(&Bld);
		if(BG){
			word* BID=BG->IDS;

			CPlayerScript* PS=PScript+ni;		
			for(int i=0;i<NB;i++){
				OneObject* OBJ=GetOBJ(BID[i]);
				if(OBJ&&!OBJ->Sdoxlo){				
					for(int j=0;j<PS->NProtectGrp;j++) if(OBJ->NIndex==PS->ProtectGrp[j]&&NPrim<1024){
						Prim[NPrim]=OBJ->Index;
						NPrim++;
					}
				}
			}
		}

		// init warriors
		for(int i=0;i<N;i++){
			OneObject* OBJ=GetOBJ(ID[i]);
			if(OBJ&&OBJ->Serial==SN[i]&&!OBJ->Sdoxlo){

				if(DetCannon(OBJ->NIndex)&&NCannon<256){
					Cannon[NCannon]=OBJ->Index;
					CannSN[NCannon]=OBJ->Serial;
					NCannon++;
				}else
				if(OBJ->BrigadeID!=0xFFFF){
					// поиск Squad
					word sqd=0xFFFF;
					for(int i=0;i<128&&i<NSqd;i++){
						if(Sqd[i].Brig==OBJ->BrigadeID){
							sqd=i;
							break;
						}
					}
					if(sqd==0xFFFF){
						sqd=AddSquad(OBJ->BrigadeID, OBJ->NIndex);
						if(CanGuardSqd==0xFFFF) CanGuardSqd=sqd;
					}
					if(sqd!=0xFFFF) InsertUnitToGroup(NULL,&Sqd[sqd].Group,OBJ->Index);
				}else{

					// find house to spriatatsia
					word PrimID=0xFFFF;
					int PrimDist=1000000;
					for(int j=0;j<NPrim;j++){
						OneObject* PO=GetOBJ(Prim[j]);
						if(PO&&PO->NewBuilding){
							int dst=Norma(OBJ->RealX-PO->RealX,OBJ->RealY-PO->RealY);
							if(dst<PrimDist){
								PrimDist=dst;
								PrimID=PO->Index;
							}
						}
					}
					if(OBJ->LockType==1 && NSheep<1024){
						Sheep[NSheep]=OBJ->Index;
						SheSN[NSheep]=OBJ->Serial;
						NSheep++;
					}else
					if(PrimDist<16000){
						if(DetPeasant(OBJ->NIndex)){
							/*
							GAMEOBJ Grp;							
							if(PrimRepareGrp[PrimID]==0xFFFF){
								RegisterDynGroup(&Grp);
								PrimRepareGrp[PrimID]=Grp.Index;
								//SGP_RepairBuilding(&Grp,0,PrimID);
							}else{
								Grp.Index=PrimRepareGrp[PrimID];
								Grp.Type='UNIT';
							}
							*/
							AddOneUnitToGroup(&Repaires,OBJ);							
						}else
							OBJ_GoToMine(OBJ->Index,PrimID,128+16,0);
					}else{
						// add to squad
						word BrID=0xFFFF;//OBJ->BrigadeID;
						word ONIndex=OBJ->NIndex;
						// поиск Squad
						word sqd=0xFFFF;
						for(int i=0;i<128&&i<NSqd;i++){
							if(Sqd[i].Brig==BrID&&Sqd[i].NIndex==ONIndex&&GetNUnits(&Sqd[i].Group)<40){
								sqd=i;
								break;
							}
						}
						if(sqd==0xFFFF){
							sqd=AddSquad(BrID, ONIndex);
						}
						if(sqd!=0xFFFF) InsertUnitToGroup(NULL,&Sqd[sqd].Group,OBJ->Index);
					}
				}
			}
		}
	}
	
	for(int j=0;j<256;j++) Fear[j]=1;

	LastGlobalMove=0;

	// upgrade
	CPlayerScript* PS=PScript+ni;
	if(PS->Enable){
		bool* AIUpg=PS->AIUpg;
		for(int i=0;i<4096;i++) if(AIUpg[i]){
			GAMEOBJ upg;
			upg.Index=i;
			upg.Type='UPGR';
			InitialUpgradeNew(ni,&upg,0xFFFF);
		}
	}

}

void Mind::Process(){
	if(NI==0xFF) return;
	switch(Type){
	case 0:
		Process0();
		break;
	case 1:
		Process1();
		break;
	case 2:
		Process2();
		break;
	case 3:
		Process3();	// attack
		break;
	case 4:
		{
			int Time=GetGlobalTime();
			if(Time-LastGlobalMove>0){
				LastGlobalMove=Time+GetRND(40)+40;
				/*if(GetRND(100)>20) Process3();
					//else Process2();*/
				int PeasTime=0;
				ProcessCannons(PeasTime);
				if(Time<PeasTime) Process2();
			}
		}
		break;	
	case 5:
		{
			int Time=GetGlobalTime();
			if(Time-LastGlobalMove>0){
				LastGlobalMove=Time+GetRND(40)+40;
				/*if(GetRND(100)>20) Process3();
					//else Process2();*/
				int PeasTime=1400;
				ProcessCannons(PeasTime);
				if(Time<PeasTime) Process2();
			}
		}
		break;	
	case 6:
		{
			int Time=GetGlobalTime();
			if(Time-LastGlobalMove>0){
				LastGlobalMove=Time+GetRND(40)+40;
				/*if(GetRND(100)>20) Process3();
					//else Process2();*/
				int PeasTime=2200;
				ProcessCannons(PeasTime);
				if(Time<PeasTime) Process2();
			}
		}
		break;	
	}

	//ProcessCannons();
}

void Mind::Process0(){
	if(NI==0xFF) return;

	int nt[2048];
	int xt[2048];
	int yt[2048];

	GetEnemyTopInfo(NI, nt, xt, yt);
	SetDangerMap(nt);
	return;

	CleanGroup(&New);
	CleanGroup(&Panic);

	int Time=GetGlobalTime();
	
	// local target managment
	for(int i=0;i<NSqd;i++){
		Squad* SQD=Sqd+i;
		GAMEOBJ* Group=&SQD->Group;
		int NMen=CleanGroup(Group);		
		if(NMen && Time-SQD->LastMoveTime>60){

			SQD->LastMoveTime=Time+GetRND(100);

			int xc,yc;
			if(GetGrpCenter(Group,&xc,&yc)){
				int top=GetTopZone(xc,yc);
				if(top>=0xFFFE){
					OneUnit UN;
					GetUnitInfo(Group,0,&UN);
					xc=UN.x;
					yc=UN.y;
					top=GetTopZone(xc,yc);
				}
				if(top>=0&&top<GetNZones()){		
					
					SQD->Top=top;

					// сила страха для разных категорий юнитов
					int Fear[256];					
					for(int j=0;j<256;j++)Fear[j]=1;

					// список целей по топ зонам
					word IDS[2][4096];
					memset(IDS,0xFF,sizeof(IDS));

					// топологическая карта опасностей
					int Dang[2][4096];
					memset(Dang,0,sizeof(Dang));

					// Primary target
					bool CaptureCenter=false;
					if(!AddEnemyCaptBuildTopList(IDS[0],NI)){
						AddEnemyCenterTopList(IDS[0],NI);
						CaptureCenter=true;
					}

					// Secondary target (enemy armies)
					CreateTopListEnArmyBtl(IDS[1],NI,NMen>>2);

					// Direct danger map					

					// Round danger map
					CreateDangerMapBattle(NI,Dang[1],GetNZones(),Fear,2);


					for(int t=0;t<2;t++){
						for(int d=0;d<2;d++){
							short SDang[4096];
							int NZ=GetNZones();
							for(int s=0;s<NZ;s++) SDang[s]=short(Dang[d][s]);

							SQD->FindTargetZone(SDang,IDS[t],SQD->TarTop[t],SQD->TarZone[t][d],SQD->TarDist[t][d]);
						}
					}



					bool moving=true;		// перейти в новую топ-зону
					
					//GAMEOBJ Zone;
					//SetZone(&Zone,xc,yc,1600);
					/*
					if(AttackEnemyInZone2(Group,&Zone,NI)){
						moving=false;
					}
					*/

					if(moving){

						int zf=SQD->TarZone[SQD->Target][SQD->MovingType];
						if(zf!=0xFFFF&&zf!=top /*&&SQD->MovingType==1*/){

							int dx,dy;
							GetTopZRealCoor(zf,&dx,&dy);

							int tx,ty;
							int dir=512;
							word TT=SQD->TarTop[SQD->Target];
							if(TT!=zf&&GetTopZRealCoor(TT,&tx,&ty)){
								dir=getDir(tx-dx,ty-dy);
							};

							if(SQD->Brig!=0xFFFF) SelectUnits(Group,0);
							SGP_MoveToPoint(NI,Group,dx,dy,dir,0,0,1);

						}

					}
				}
			}					

		}
	}

	// Global target managment
	if(Time-LastGlobalMove>90){
		LastGlobalMove=Time+100+GetRND(200);

		NTrgList=0;
		//memset(NTrgList,0,sizeof(NTrgList));
		memset(NSqdList,0,sizeof(NSqdList));
		
		// заполнение матриц оценки
		for(int i=0;i<NSqd;i++){
			Squad* SQD=Sqd+i;
			GAMEOBJ* Group=&SQD->Group;
			int NMen=GetNUnits(Group);			
			if(NMen){

				word TT=SQD->TarTop[1];
				word DST=SQD->TarDist[1][0];

				if(DST>50){
					// цель далеко
					SQD->Target=0;
					SQD->MovingType=0;

				}else{

					word MinTT=0xFFFF;
					word MinDst=130;
					word TargID=0xFFFF;	//  индекс цели

					// поиск цели
					for(int t=0;t<NTrgList;t++){
						word dst=GetZonesDist(TT,TrgList[t]);
						if(dst<MinDst){
							MinTT=TrgList[t];
							MinDst=dst;
							TargID=t;
						}
					}				

					if(MinTT==0xFFFF){				
						// создать новую цель
						TrgList[NTrgList]=TT;
						TargID=NTrgList;
						NTrgList++;						
					}

					if(TargID!=0xFFFF){					

						word NS=NSqdList[TargID];

						SqdList[TargID][NS]=i;
						DstList[TargID][0][NS]=SQD->TarDist[1][0];
						DstList[TargID][1][NS]=SQD->TarDist[1][1];
						
						NSqdList[TargID]++;
							
					}

				}

			}
		}

		// раздача слонов
		for(i=0;i<NTrgList;i++){
			word MinRndDist=1000;
			word MaxDirDist=0;
			word SqdID=0xFFFF;

			word NSL=NSqdList[i];

			// найти передовой обходной отряд
			for(int s=0;s<NSL;s++){
				//if(DstList[i][1][s]<MinRndDist){
				if(DstList[i][0][s]>MaxDirDist){
					MaxDirDist=DstList[i][0][s];
					SqdID=s;
				}
			}

			if(SqdID!=0xFFFF){
				for(int s=0;s<NSL;s++){
					Squad* SQD=Sqd+SqdList[i][s];
					if(DstList[i][0][s]<MaxDirDist-3){
						SQD->Target=1;
						SQD->MovingType=1;
					}else{
						SQD->Target=1;
						SQD->MovingType=0;
					}
				}
			}
		}
	}
}

void Mind::Process1(){
	if(NI==0xFF) return;

	int nt[2048];
	int xt[2048];
	int yt[2048];

	GetEnemyTopInfo(NI, nt, xt, yt);
	//SetDangerMap(nt);

	CleanGroup(&New);
	CleanGroup(&Panic);

	int Time=GetGlobalTime();

	if(Time-LastGlobalMove>50){
		LastGlobalMove=Time+GetRND(30);

		ClearAZones();

		word IDSS[4096];
		memset(IDSS,0xFF,sizeof(IDSS));
		CreateTopListEnArmyBtl(IDSS,NI,1);

		word REAR[4096];
		memset(REAR,0xFF,sizeof(REAR));
		CreateFriendBuildingsTopList(REAR,NI);

		word ZREAR=0xFFFF;
		int NZ=GetNZones();
		for(int z=0;z<NZ;z++){
			if(REAR[z]!=0xFFFF){
				ZREAR=z;
				break;
			}
		}

		short dang[4096];
		memset(dang,0,sizeof(dang));

		int maxdang;
		word DST=0xFFFF;
		int TZ=0xFFFF;
		if(ZREAR!=0xFFFF) TZ=FindNextZoneOnTheSafeWayToObject(ZREAR,dang,IDSS,&maxdang,5,&DST);
		
		int MaxDist=1000;	// дистанция на которую можно отходить от базы
		if(TZ!=0xFFFF && ZREAR!=0xFFFF){
			word* WL=NULL;
			int NW=GetLastFullWay(&WL);
			if(NW>0){
				word dst=GetZonesDist(ZREAR,WL[0]);
				if(dst<MaxDist){
					MaxDist=dst;
				}
			}
		}
		if(MaxDist>23) MaxDist-=13;

		// сила страха для разных категорий юнитов
		int Fear[256];					
		for(int j=0;j<256;j++)Fear[j]=1;

		// топологическая карта опасностей
		int Dang[2][4096];
		memset(Dang,0,sizeof(Dang));

		// Direct danger map					

		// Round danger map
		CreateDangerMapBattle(NI,Dang[1],GetNZones(),Fear,2);

		// local target managment
		for(int i=0;i<NSqd;i++){
			Squad* SQD=Sqd+i;
			GAMEOBJ* Group=&SQD->Group;
			int NMen=CleanGroup(Group);
			if(NMen /*&& Time-SQD->LastMoveTime>50*/){

				//SQD->LastMoveTime=Time+GetRND(30);

				int xc,yc;
				if(GetGrpCenter(Group,&xc,&yc)){
					int top=GetTopZone(xc,yc);
					if(top>=0xFFFE){
						OneUnit UN;
						GetUnitInfo(Group,0,&UN);
						xc=UN.x;
						yc=UN.y;
						top=GetTopZone(xc,yc);
					}
					if(top>=0&&top<GetNZones()){		
						
						SQD->Top=top;

						SelectUnits(Group,0);
						char name[256];
						sprintf(name,"Squad %d",i);
						CreateAGroup(NI,name);

						// список целей по топ зонам
						word IDS[3][4096];
						memset(IDS,0xFF,sizeof(IDS));

						// Primary target
						bool CaptureCenter=false;
						if(!AddEnemyCaptBuildTopList(IDS[0],NI)){
							AddEnemyCenterTopList(IDS[0],NI);
							CaptureCenter=true;
						}

						// Secondary target (enemy armies)
						CreateTopListEnArmyBtl(IDS[1],NI,NMen>>2);

						for(int t=0;t<2;t++){
							for(int d=0;d<2;d++){
								if(t==1&&d==0){
									short SDang[4096];
									int NZ=GetNZones();
									for(int s=0;s<NZ;s++) SDang[s]=Dang[d][s];
									
									SQD->FindTargetZone(SDang,IDSS,SQD->TarTop[t],SQD->TarZone[t][d],SQD->TarDist[t][d]);
									word TT=SQD->TarTop[t];
																		
									/*
									if(TT!=0xFFFF){
										IDSS[TT]=0xFFFF;
										word* zl=NULL;
										int nz=GetListOfNearZones(TT,&zl);
										for(int i=0;i<nz;i++){
											if(zl[i+i]!=0xFFFF) IDSS[zl[i+i]]=0xFFFF;
										}
									}
									*/

								}
							}
						}

						bool moving=true;		// перейти в новую топ-зону
						
						//GAMEOBJ Zone;
						//SetZone(&Zone,xc,yc,1600);
						/*
						if(AttackEnemyInZone2(Group,&Zone,NI)){
							moving=false;
						}
						*/

						// find defend zone
						int zt=SQD->TarTop[1];
						if(zt==0xFFFF&&i){
							zt=Sqd[i-1].TarTop[1];
						}

						if(moving&&zt!=0xFFFF){

							word zf;

							word zb=ZREAR;
							//word dst;
							
							//CreateFriendBuildingsTopList(IDS[3],NI);
							//SQD->FindTargetZone(Dang[0],IDS[3],zb,zf,dst);

							zf=0xFFFF;
							int fdst=1000;
							int zbb=zb;						
							
							// вырезать цели в этом секторе
							int xx,yy;
							if(GetTopZRealCoor(zb,&xx,&yy)){
								int x,y;
								if(GetTopZRealCoor(zt,&x,&y)){
									int Dir=getDir(x-xx,y-yy);	// направление на найденную цель

									int NZ=GetNZones();
									for(int z=0;z<NZ;z++){											
										
										if(IDSS[z]!=0xFFFF && GetTopZRealCoor(z,&x,&y)){
											int dir=abs(getDir(x-xx,y-yy)-Dir);
											if(dir<15){
												IDSS[z]=0xFFFF;
											}
										}
									}
								}
							}

							int tdst=10000;

							// найти зону прикрытия
							zb=GetNextZone(zb,zt);
							while(zb!=0xFFFF && zb!=zt){
								int wdst=GetZonesDist(zb,zt);
								int bdst=GetZonesDist(zb,zbb);
								
								if(wdst<fdst /*&&wdst>15*/ && bdst<MaxDist){
									fdst=wdst;
									tdst=bdst;
									zf=zb;
								}
								zb=GetNextZone(zb,zt);
							}
							//if(zt!=0xFFFF) zf=zt;
							
							int btogdist=1000;
							if(zbb!=0xFFFF){
								btogdist=GetZonesDist(zbb,zt);
							}
													
							if(btogdist>19 && zf!=0xFFFF /*&&zf!=top*/ /*&&SQD->MovingType==1*/){

								int dx,dy;
								GetTopZRealCoor(zf,&dx,&dy);

								
								int dir=512;
								word TT=zt;
								
								int tx=0,ty=0,tn=0;
								
								int nz=GetNZones();
								for(int i=0;i<nz;i++){
									int nn=nt[i];
									if(nn){
										word ds=GetZonesDist(i,zt);
										if(ds<0xFFFE){
											//ds>>=1;																				
											if(ds>1) nn/=ds;
											if(nn){
												tx+=xt[i]*nn;
												ty+=yt[i]*nn;
												tn+=nn;
											}
										}
									}
								}
								if(tn){
									tx/=tn;
									ty/=tn;
									dir=getDir(tx-dx,ty-dy);
								}
								
								/*
								if(TT!=zf&&GetTopZRealCoor(TT,&tx,&ty)){
									dir=getDir(tx-dx,ty-dy);
								};
								*/

								if(SQD->Brig!=0xFFFF) SelectUnits(Group,0);
								SGP_MoveToPoint(NI,Group,dx+16-GetRND(32),dy+16-GetRND(32),dir,0,0,1);
								
								SetUnitsState(Group,0,0,0,0);

							}else{
								int d=GetZonesDist(top,zt);
								if(d<12){
									//if(SQD->Brig) BrigReformation(NI,SQD->Brig,2);
									SetUnitsState(Group,1,1,0,0);
								}else{
									SetUnitsState(Group,0,1,0,0);
								}
							}

						}
					}
				}					

			}
		}
	}
}

void GetArmyMap(int* ArmyMap, int* Dang, ActiveArmy* AA, int& NAA){
	int NZ=GetNZones();

	//memset(ArmyMap,0xFF,sizeof ArmyMap);
	for(int j=0;j<NZ;j++) ArmyMap[j]=0xFFFF;

	NAA=0;
	AA[0].Clear();

	for(int i=0;i<NZ;i++)if(ArmyMap[i]==0xFFFF){
					
		ActiveArmy* aa=AA+NAA;

		word TopList[1024];
		TopList[0]=i;
		int NTopList=1;
		
		bool add=false;

		while(NTopList){
			NTopList--;
			int t=TopList[NTopList];

			word armpos=0xFFFF;

			// add new links top zones
			OneLinkInfo* links=NULL;
			int NL=GetListOfNearZones(t,&links);
			for(int p=0;p<NL;p++){
				int id=links[p].NextAreaID;
				if(Dang[id]){
					if(ArmyMap[id]==0xFFFF){
						armpos=NAA;
						TopList[NTopList]=id;
						NTopList++;
					}else{
						armpos=ArmyMap[id];
					}
				};
			};

			if(Dang[t]){
				// add army
				ArmyMap[t]=NAA;
				int x,y;
				if(GetTopZRealCoor(t,&x,&y)){
					aa->xc+=x;
					aa->yc+=y;
					aa->n++;
				}
				add=true;
			}else{
				if(armpos<0xFFFF){
					// add position
					ArmyMap[t]=0x1000;
					AA[armpos].PID[AA[armpos].NP]=t;
					AA[armpos].NP++;
				}else{
					// free field
					ArmyMap[t]=0xFFFE;
				}
			}

			/*// add squad
			for(int s=0;s<NS;s++){
				if(t==ST[s]){
					//ArmyMap[t]=s;
					aa->SID[aa->NS]=s;
					aa->NS++;
				}
			}*/

		}

		if(add){
			NAA++;
			AA[NAA].Clear(NAA);
		}

	}

}

void ShowWay(char* Mess, word* Way, int NWay){
	if(!Way) return;
	char txt[255];
	
	for(int i=0;i<NWay;i++){
		sprintf(txt,"Way %s %d",Mess,i);
		int x,y;
		if(GetTopZRealCoor(Way[i],&x,&y)) CreateAZone(txt,x,y,64);
	}
}

int FindStepToObject(int Start,short* DangerMap,word* ObjIds,word* DestObj){
	int MaxDang;
	return FindNextZoneOnTheSafeWayToObject(Start,DangerMap,ObjIds,&MaxDang,1000,DestObj);
}

#include <vector>
#include <queue>

using namespace std;

void GetArmyMap(Enemy* ENM, word* DefTent, short* Dang){
	int NZ=GetNZones();
	int DM[4096];
	for(int i=0;i<NZ;i++) DM[i]=Dang[i];
	int* ArmyMap=ENM->ArmyMap;
	GetArmyMap(ArmyMap,DM,ENM->AA,ENM->NAA);

	short NullDang[4096];
	memset(NullDang,0,sizeof(NullDang));	

	int NA=ENM->NAA;

	ArmyTopInfo* ATI=&ENM->TopInf;
	int* AM=ENM->ArmyMap;
	for(i=0;i<NZ;i++){
		int arm=AM[i];
		if(arm<NA){
			ActiveArmy* a=ENM->AA+arm;
			a->Power+=ATI->Power[i];
		}
	}

	
	for(i=0;i<NA;i++){
		ActiveArmy* a=ENM->AA+i;
		if(a->n){
			a->xc/=a->n;
			a->yc/=a->n;
			a->TopCenter=GetTopZone(a->xc,a->yc);
			if(a->TopCenter<0xFFFE&&FindStepToObject(a->TopCenter,NullDang,DefTent,&a->TentID)<0xFFFE){
				word* W=NULL;
				int N=GetLastFullWay(&W);

				if(W){
					
					for(int j=N-1;j>=0;j--){
						int arm=ArmyMap[W[i]];
						if(arm<N&&arm!=i){
							a->Link.Front=arm;
							break;
						}
					}

					//while(ArmyMap[W[N-1]]==i) N--;					

					a->NTentWay=N;
					memcpy(a->TentWay,W,a->NTentWay<<1);

					a->TentWayDist=GetZonesDist(a->TentWay[0],a->TentWay[N-1]);
					
					char txt[200];
					sprintf(txt,"Arm %d",a->Power);//i
					ShowWay(txt,a->TentWay,a->NTentWay);
				}
			}
		}
	}

}

void Mind::MarkArmy(){
	// mark army on map by rectangle
	ActiveArmy* AA=ENM.AA;
	int NAA=ENM.NAA;
	
	for(int i=0;i<NAA;i++){
		ActiveArmy* a=AA+i;
		
		int N=NSqd;

		char name[255];

		/*
		ClearSelection(NI);
		for(int s=0;s<N;s++){
			GAMEOBJ* Grp=&Sqd[i].Group;
			if(GetNUnits(Grp)){
				SelectUnits(Grp,1);
			}
		}	
		sprintf(tname,"Army %d",i);
		CreateAGroup(NI,name);
		*/

		sprintf(name,"En %d, top %d",i,a->TopCenter);
		CreateAZone(name,a->xc,a->yc,256);

	}
}

void Mind::Process2(){
	if(NI==0xFF) return;

	int Time=GetGlobalTime();

	if(true){ //Time-LastGlobalMove>0
		//LastGlobalMove=Time+GetRND(50)+60;

		ClearAZones();
	
		word SList[255];	// squad list
		int SX[255];
		int SY[255];
		int ST[255];		// squad top
		int NS=0;

		for(int i=0;i<NSqd;i++){
			Squad* SQD=Sqd+i;
			GAMEOBJ* Group=&SQD->Group;
			int NMen=CleanGroup(Group);
			if(NMen /*&& Time-SQD->LastMoveTime>50*/){

				//SQD->LastMoveTime=Time+GetRND(30);

				int xc,yc;
				if(GetGrpCenter(Group,&xc,&yc)){
				
					int top=GetTopZone(xc,yc);
					if(top>=0xFFFE){
						OneUnit UN;
						GetUnitInfo(Group,0,&UN);
						xc=UN.x;
						yc=UN.y;
						top=GetTopZone(xc,yc);
					}
					if(top>=0&&top<GetNZones()){
						SX[NS]=xc;
						SY[NS]=yc;
						ST[NS]=top;
						SList[NS]=i;

						SQD->xc=xc;
						SQD->yc=yc;
					}
					
				}

				NS++;

			}
		}

		int NZ=GetNZones();

		// сила страха для разных категорий юнитов
		int Fear[256];
		for(int j=0;j<256;j++) Fear[j]=1;

		// топологическая карта опасностей
		int Dang[4096];
		memset(Dang,0,sizeof(Dang));

		CreateDangerMapBattle(NI,Dang,NZ,Fear,2);

		ActiveArmy AA[128];
		memset(AA,0,sizeof AA);
		int NAA=0;

		int ArmyMap[4096];

		GetArmyMap(ArmyMap,Dang,AA,NAA);

		// link squad to nearest enemy army
		short DD[4096];
		memset(DD,0,sizeof(DD));

		word IDS[4096];		
		for(i=0;i<NZ;i++){
			if(ArmyMap[i]<0x1000){
				IDS[i]=ArmyMap[i];
			}else{
				IDS[i]=0xFFFF;
			}
		}

		int MaxDan;

		for(i=0;i<NS;i++){		
			word DST=0xFFFF;
			FindNextZoneOnTheSafeWayToObject(ST[i],DD,IDS,&MaxDan,1000,&DST);
			
			if(DST<NAA){
				ActiveArmy* aa=AA+DST;
				aa->SID[aa->NS]=SList[i];
				aa->NS++;
			}

		}

		// mark army on map by rectangle
		for(i=0;i<NAA;i++){
			ActiveArmy* aa=AA+i;
			
			int N=aa->NS;
			word* SID=aa->SID;

			ClearSelection(NI);
			for(int s=0;s<N;s++){
				int ss=SID[s];
				if(ss<NSqd){
					SelectUnits(&Sqd[ss].Group,1);
				}
			}

			char name[255];
			sprintf(name,"Army %d",i);
			CreateAGroup(NI,name);

		}

		for(i=0;i<NZ;i++) DangerMap[i]=Dang[i];

		ActivateArmy(AA,NAA);

	}

}

CIMPORT bool DetectAbsorber(word NIndex);
CIMPORT bool DetShoot(word NIndex);

void Mind::ProcessCannons(int TimeToStorm){
	if(NI==0xFF) return;

	// collect game situation
	byte PrimNIndex[4096];
	memset(PrimNIndex,0,sizeof(PrimNIndex));
	
	for(int i=0;i<8;i++)if(i!=NI){
		CPlayerScript* PS=PScript+i;
		if(PS->Enable&&!PS->Defeat){
			for(int j=0;j<PS->NProtectGrp;j++){
				PrimNIndex[PS->ProtectGrp[j]]=1;					
			}
		}
	}

	// attacking target
	word Prim[1024];
	int NPrim=0;
	word Secd[1024];	// cannons
	int NSecd=0;

	int NTop=GetNZones();
	byte* SecdTop=(byte*)malloc(NTop);	// brigadnie enemy in top zone
	memset(SecdTop,0,NTop);

	int MO=GetMaxObject();
	for(i=0;i<MO;i++){
		OneObject* OB=GetOBJ(i);
		if(OB&&!OB->Sdoxlo){
			if(OB->NNUM==NI){ // alies
				CPlayerScript* PS=PScript+NI;
				for(int j=0;j<PS->NProtectGrp;j++){
					if(PS->ProtectGrp[j]==OB->NIndex){
						if(OB->Life<OB->MaxLife) SGP_RepairBuilding(&Repaires,2,OB->Index);
						break;
					}
						
				}
			}else{	// enemy
				if(PrimNIndex[OB->NIndex] && NPrim<1024){ // prim target: fortress
					Prim[NPrim]=OB->Index;
					NPrim++;
				}else
				if(NSecd<1024){ // secd target: for cannon
					if(OB->BrigadeID!=0xFFFF){
						int OT=GetTopZone(OB->RealX>>4,OB->RealY>>4);
						if(OT<NTop){
							SecdTop[OT]++;
							if(SecdTop[OT]==30){
								Secd[NSecd]=OB->Index;
								NSecd++;
							}
						}
					}else
					if(OB->LockType==1){
						Secd[NSecd]=OB->Index;
						NSecd++;
					}
				}
			}
		}
	}

	// cannons
	int ccx=0,ccy=0;
	if(NCannon){

		int n=0;
		for(i=0;i<NCannon;i++){
			OneObject* CO=GetOBJ(Cannon[i]);
			if(CO&&CO->Serial==CannSN[i]&&!CO->Sdoxlo){
				int cx=CO->RealX;
				int cy=CO->RealY;

				ccx+=cx;
				ccy+=cy;
				n++;
				
				int PrimDist=10000000;
				word PrimID=0xFFFF;
				byte PrimDir;
				int px,py;
				byte PrimLockType;
				for(int j=0;j<NPrim;j++){
					OneObject* PO=GetOBJ(Prim[j]);
					int dst=Norma(PO->RealX-cx,PO->RealY-cy);
					if(PrimDist>dst){
						PrimDist=dst;
						PrimID=PO->Index;
						PrimDir=getDir(PO->RealX-cx,PO->RealY-cy);
						px=PO->RealX>>4;
						py=PO->RealY>>4;
						PrimLockType=PO->LockType;
					}
				}
				int SecdDist=10000000;
				word SecdID=0xFFFF;
				//byte SecdDir;
				int sx,sy;
				byte SecdLockType;
				for(j=0;j<NSecd;j++){
					OneObject* SO=GetOBJ(Secd[j]);
					sx=SO->RealX;
					sy=SO->RealY;
					int dst=Norma(sx-cx,sy-cy);
					
					//byte dir;						
					byte tdir=0;

					if(PrimID!=0xFFFF){
						byte dir=getDir(sx-cx,sy-cy);
						tdir=PrimDir-dir;
						if(tdir>128) tdir=-tdir;
					}
					
					if(dst<SecdDist && tdir<24){
						SecdDist=dst;
						SecdID=SO->Index;
						//SecdDir=dir;
						SecdLockType=SO->LockType;
					}
				}

				if(PrimID!=0xFFFF && (SecdID==0xFFFF||PrimDist-SecdDist<32000)){
					if(PrimDist>60000){
						if(GetTopDist(cx>>4,cy>>4,px,py)<0xFFFE) OBJ_SendTo(CO->Index,px,py,128+16,0);
					}else{
						OBJ_AttackObj(CO->Index,PrimID,128+16,0);
					}
				}else
				if(SecdID!=0xFFFF){
					if(SecdDist>50000){
						if(GetTopDist(cx>>4,cy>>4,sx>>4,sy>>4)<0xFFFE) OBJ_SendTo(CO->Index,px,py,128+16,0);
					}else{
						OBJ_AttackObj(CO->Index,SecdID,128+16,0);
					}
				}
			}
		}

		if(n){
			ccx/=n;
			ccy/=n;
			ccx>>=4;
			ccy>>=4;
		}
		
	}

	// sheeps
	if(NSheep){

		for(i=0;i<NSheep;i++){
			OneObject* OB=GetOBJ(Sheep[i]);
			if(OB&&OB->Serial==SheSN[i]&&!OB->Sdoxlo){
				int cx=OB->RealX;
				int cy=OB->RealY;

				int px,py;

				int PrimDist=10000000;
				word PrimID=0xFFFF;
				byte PrimDir;
				for(int j=0;j<NPrim;j++){
					OneObject* PO=GetOBJ(Prim[j]);
					int dst=Norma(PO->RealX-cx,PO->RealY-cy);
					if(PrimDist>dst){
						PrimDist=dst;
						PrimID=PO->Index;
						PrimDir=getDir(PO->RealX-cx,PO->RealY-cy);
						px=PO->RealX>>4;
						py=PO->RealY>>4;
					}
				}
				int SecdDist=10000000;
				word SecdID=0xFFFF;
				//byte SecdDir;
				for(j=0;j<NSecd;j++){
					OneObject* SO=GetOBJ(Secd[j]);
					int dx=SO->RealX-cx;
					int dy=SO->RealY-cy;
					int dst=Norma(dx,dy);
					
					byte dir;						
					byte tdir=0;

					if(PrimID!=0xFFFF){
						dir=getDir(dx,dy);
						tdir=PrimDir-dir;
						if(tdir>128) tdir=-tdir;
					}
					
					if(dst<SecdDist && tdir<24){
						SecdDist=dst;
						SecdID=SO->Index;
						//SecdDir=dir;
					}
				}

				if(PrimID!=0xFFFF && (SecdID==0xFFFF||PrimDist-SecdDist<32000)){
					//if(PrimDist>70000){
					//	OBJ_SendTo(OB->Index,px,py,128+16,0);
					//}else{
						OBJ_AttackObj(OB->Index,PrimID,128+16,0);
					//}
				}else
				if(SecdID!=0xFFFF){
					if(SecdDist>40000){
						OBJ_SendTo(OB->Index,px,py,128+16,0);
					}else{
						OBJ_AttackObj(OB->Index,SecdID,128+16,0);
					}
				}
			}
		}
		
	}

	free(SecdTop);

	// time to storm
	if(GetGlobalTime()>TimeToStorm){
		RefreshSquadInfo();
		
		//int NZones=GetNZones();
		//Fear[0]=1;
		//CreateDangerMap(NI,DangerMap,NZones,Fear,1);

		for(int i=0;i<NSqd;i++){
			Squad* SQ=Sqd+i;
			
			GAMEOBJ* Grp=&SQ->Group;			
			if(SQ->Top<0xFFFE){
				if(i==CanGuardSqd&&ccx){
					if(SQ->Brig!=0xFFFF) SelectUnits(Grp,0);
					SGP_MoveToPoint(NI,Grp,ccx,ccy,256,0,0,1);
					SetUnitsState(Grp,1,0,0,0);
					continue;
				}

				if(DetArcher(SQ->NIndex)){
					if(SQ->Brig!=0xFFFF) EraseBrig(NI,SQ->Brig);
					AddTomahawks(Grp,NI,0,0,0);
					continue;
				}

				// find prim targ
				word PrimID=0xFFFF;
				int PrimDist=1000000;
				word PrimNIndex;
				bool UnitAbsorber;
				int px,py;
				for(int j=0;j<NPrim;j++){
					OneObject* PO=GetOBJ(Prim[j]);
					if(PO){
						int dst=Norma((PO->RealX>>4)-SQ->xc,(PO->RealY>>4)-SQ->yc);
						if(dst<PrimDist){
							PrimDist=dst;
							PrimID=PO->Index;
							UnitAbsorber=DetectAbsorber(PO->NIndex);
							px=PO->RealX>>4;
							py=PO->RealY>>4;
							PrimNIndex=PO->NIndex;
						}
					}
				}

				if(SQ->Brig==0xFFFF&&DetAbsorbeAbility(PrimNIndex,SQ->NIndex)&&GetNUnits(Grp)<20){
					AddStorm(Grp,NI,0);
					continue;
				};

				// find sec
				int SecdDist=10000000;
				word SecdID=0xFFFF;
				//byte SecdDir;
				for(j=0;j<NSecd;j++){
					OneObject* SO=GetOBJ(Secd[j]);
					int dx=(SO->RealX>>4)-SQ->xc;
					int dy=(SO->RealY>>4)-SQ->yc;
					int dst=Norma(dx,dy);
					
					if(dst<SecdDist){
						SecdDist=dst;
						SecdID=SO->Index;
						//SecdDir=dir;
					}
				}
				// go go go
				if(PrimID!=0xFFFF){
					if(UnitAbsorber&&PrimDist<1000){
						// Storm
						UnitsGroup* SG=GetUnitsGroup(Grp);
						if(SG){
							for(int u=0;u<SG->N;u++){
								OBJ_GoToMine(SG->IDS[u],PrimID,128+16,0);
							}
						}
					}else{
						// Attack
						bool move=true;
						
						if(DetShoot(SQ->NIndex)){
							if(PrimDist>400&&(SecdID==0xFFFF||SecdDist>800)){
								if(PrimDist<1000)
									SetUnitsState(Grp,1,0,0,0);
								else
									SetUnitsState(Grp,1,0,1,0);
							}else{
								if(SQ->Brig!=0xFFFF){
									SetUnitsState(Grp,1,1,0,0);
									move=false;
								}else{
									SetUnitsState(Grp,1,0,0,0);
								}
							}
						}else{
							SetUnitsState(Grp,1,0,0,0);
						};
						
						if(move){
							if(SQ->Brig!=0xFFFF) SelectUnits(Grp,0);
							SGP_MoveToPoint(NI,Grp,px,py,256,0,0,1);
						}
						
						// Storm
						UnitsGroup* SG=GetUnitsGroup(Grp);
						if(SG){
							for(int u=0;u<SG->N;u++){
								OneObject* OU=GetOBJ(SG->IDS[u]);
								if(OU&&OU->Serial==SG->SNS[u] && Norma(px-(OU->RealX>>4),py-(OU->RealY>>4))<800){
									OBJ_GoToMine(SG->IDS[u],PrimID,128+16,0);
								}
							}
						}
						//SQ->MoveToTop(NULL);
					}
				}

			}

		}
	}
};

void Mind::Process3(){
	if(NI==0xFF) return;

	int Time=GetGlobalTime();

	if(true){//Time-LastGlobalMove>0
		//LastGlobalMove=Time+GetRND(50)+60;

		SetGameGoals();
		//return;
		SetDangerMap(Dang);

		RefreshSquadInfo();

		ClearAZones();

		GetArmyMap(&ENM,DefTent,DangerMap);
		//SetDangerMap(ArmyMap);
		
		MarkArmy();

		SetLink0();

		GlobalAttack();

		switch(ShowMode){
		case 1:
			ShowVistrel(); break;
		};		
	
	}

}

void Mind::SetLink0(){
	
	// сортируем вражеские армии по расстоянию до наших палаток
	priority_queue <AAT,vector<AAT>,GreatAA> AQ;
	int NA=ENM.NAA;
	for(int i=0;i<NA;i++){
		ActiveArmy* a=ENM.AA+i;
		if(a->NTentWay&&a->Link.Front==0xFFFF) AQ.push(a);
	}

	/*
	FILE* f=fopen("c:\\queue.txt","w");
	while(!AQ.empty()&&f){
		fprintf(f,"%d\n",AQ.top().TentWayDist);
		AQ.pop();
	}
	fclose(f);
	*/
	
	// обрабатываем каждую армию, и раздаем приказы по ней
	while(!AQ.empty()){
		ActiveArmy* AA=AQ.top();		

		word dist=AA->TentWayDist;
		if(dist<0xFFFF){

			int pow=GetArmyActorPower(AA);
			
			// создаем список наших свободных отрядов
			priority_queue <SqdT,vector<SqdT>,GreatSQD> SqdQ;			
			for(int s=0;s<NSqd;s++){
				Squad* sqd=Sqd+s;
				if(sqd->Top!=0xFFFF&&sqd->ArmyID==0xFFFF){
					sqd->ArmyDist=GetZonesDist(sqd->Top,AA->TopCenter);
					SqdQ.push(sqd);
				}
			}

			//ClearAZones();
			CreateAZone("acti",AA->xc,AA->yc,64);			

			// линкуем отряд с армией противника
			s=0;
			while(!SqdQ.empty()&&pow<AA->Power){
				char txt[200];
				sprintf(txt,"prio=%d, dist=%d",s,SqdQ.top()->ArmyDist);
				SelectUnits(&SqdQ.top()->Group,0);
				CreateAGroup(NI,txt);
				
/*				Polk* plk=&AA->Actor;
				plk->SquadID[plk->NSquad]=SqdQ.top()->Index;
				plk->NSquad++;
				
				Squad* sqd=SqdQ.top();
				sqd->ArmyID=AA->Index;
				sqd->ArmyDist=GetZonesDist(AA->TopCenter,sqd->Top);

				pow+=GetGroupPower(&sqd->Group)>>10;
*/				
				SqdQ.pop();
				s++;
			}

		}

		AQ.pop();
	}
}

CEXPORT
void MindCheats(byte NI, char* com){
	if(NI>6) return;
	char scom[100];
	if(strstr(com,"acti")){
		int param;
		int n=sscanf(com,"%s%d",scom,&param);
		if(n<2) param=2;
		ActivateMind(NI,param);
	}else
	if(strstr(com,"deac")){
		DeactivateMind(NI);
	}else
	if(strstr(com,"run")){
		int param;
		sscanf(com,"%s%d",scom,&param);
		Player[NI].RunState=param;
	}else
	if(strstr(com,"show")){
		char param[100];
		sscanf(com,"%s%s",scom,param);
		if(strstr(param,"dang")){
			int Dang[4096];
			short* SDan=Player[NI].DangerMap;
			int NZ=GetNZones();
			for(int i=0;i<NZ;i++) Dang[i]=SDan[i];
			SetDangerMap(Dang);
		}else
		if(strstr(param,"vist")){
			Player[NI].ShowMode=1;		
		}else
		if(strstr(param,"kart")){
			Player[NI].ShowMode=2;			
		}//else		
	}else
	if(strstr(com,"type")){
		int type;
		if(sscanf(com,"%s%d",scom,&type)>1){
			Player[NI].Type=type;
		}
	}//else
}

int Mind::SetGameGoals(){
	int NZ=GetNZones();

	// топологическая карта опасностей
	memset(Dang,0,sizeof(Dang));
	//CreateDangerMapBattle(NI,Dang,NZ,Fear,2);

	ArmyTopInfo* ATI=&ENM.TopInf;
	CreateArmyInfo(NI,ATI->Life,ATI->Damage, NZ);
	for(int i=0;i<NZ;i++){
		if(ATI->Life[i])
			Dang[i]=ATI->Power[i]=(ATI->Life[i]*ATI->Damage[i])>>10;
	}
	
	for(i=0;i<NZ;i++) DangerMap[i]=Dang[i];
	//for(i=0;i<NZ;i++) DangerMap[i]=ATI->Power[i];
	
	// карты стрелковых юнитов
	memset(MapVistrel,0,sizeof MapVistrel);
	memset(MapKartech,0,sizeof MapKartech);
	SetMapOfShooters(NI,MapVistrel,MapKartech);
	ShowVistrel();

	// вражеские главные цели
	memset(DefTent,0xFF,sizeof(DefTent));
	CreateFriendBuildingsTopList(DefTent,NI);
	
	// наши главные цели
	memset(CapTent,0xFF,sizeof(CapTent));
	AddEnemyCaptBuildTopList(CapTent,NI);
	
	return 1;
};

void Mind::RefreshSquadInfo(){
	for(int i=0;i<NSqd;i++){
		Squad* SQD=Sqd+i;
		GAMEOBJ* Group=&SQD->Group;
		
		SQD->Top=0xFFFF;
		SQD->ArmyID=0xFFFF;

		int NMen=CleanGroup(Group);
		if(NMen /*&& Time-SQD->LastMoveTime>50*/){

			//SQD->LastMoveTime=Time+GetRND(30);

			int xc,yc;
			if(GetGrpCenter(Group,&xc,&yc)){
			
				int top=GetTopZone(xc,yc);
				if(top>=0xFFFE){
					OneUnit UN;
					GetUnitInfo(Group,0,&UN);
					xc=UN.x;
					yc=UN.y;
					top=GetTopZone(xc,yc);
				}
				if(top>=0&&top<GetNZones()){
					SQD->xc=xc;
					SQD->yc=yc;
					SQD->Top=top;
				}
			}

			SQD->Power=GetGroupPower(Group)>>10;			
		}
	}
};
int Mind::GetArmyActorPower(ActiveArmy* arm){
	if(arm){
	/*	Polk* plk=&arm->Actor;
		plk->Power=0;
		for(int i=0;i<plk->NSquad;i++){
			plk->Power+=GetGroupPower(&Sqd[plk->SquadID[i]].Group)>>10;
		}
		return plk->Power;
		*/
	}
	return -1;
};

void Mind::GlobalAttack(){
	for(int i=0;i<NSqd;i++){
		Squad* SQD=Sqd+i;
		GAMEOBJ* Group=&SQD->Group;
		
		if(SQD->Top<0xFFFE&&SQD->ArmyID!=0xFFFF){
			
			SQD->Top=ENM.AA[SQD->ArmyID].TopCenter;			
			SQD->MoveToTop(NULL);

		};

	}
};

void Mind::ActivateArmy(ActiveArmy* AA, int NA){
	// make quick round
	
	for(int a=0;a<NA;a++){
		ActiveArmy* arm=AA+a;

		word Dist[255];	// dist [squad] to nearest position (NewPos)
		memset(Dist,0xFF,sizeof Dist);
		word NewPos[255];		// position [squad]
		memset(NewPos,0xFF,sizeof NewPos);
		word NewSqd[255];		// squad [position]
		memset(NewSqd,0xFF,sizeof NewSqd);
				
		// center of army
		int xc=0,yc=0,n=0;
		for(int s=0;s<arm->NS;s++){
			word SID=arm->SID[s];
			Squad* sqd=Sqd+SID;
			int nn=GetNUnits(&sqd->Group);
			xc+=sqd->xc*nn;
			yc+=sqd->yc*nn;
			n+=nn;
		}
		if(n){
			xc/=n;
			yc/=n;
		}

		// ищем тыл
		int NZ=GetNZones();

		word REAR[4096];
		memset(REAR,0xFF,sizeof(REAR));
		CreateFriendBuildingsTopList(REAR,NI);

		short D[4096];
		memset(D,0,sizeof D);

		int maxdang;
		word DST=0xFFFF;
		
		FindNextZoneOnTheSafeWayToObject(GetTopZone(xc,yc),D,REAR,&maxdang,10000,&DST);

		if(DST!=0xFFFF){
			word* Way;
			int NWayPoint=GetLastFullWay(&Way);			
			if(NWayPoint){
				GetTopZRealCoor(Way[0],&xc,&yc);
			}
		}


		word ss;
		do{			
			int MinR=1000000;
			word pos=0xFFFF;
			int px,py;
			for(int p=0;p<arm->NP;p++)if(NewSqd[p]==0xFFFF){
				word TOP=arm->PID[p];
				int tx,ty;
				GetTopZRealCoor(TOP,&tx,&ty);

				int r = Norma(tx-xc,ty-yc);
				if(r<MinR){
					MinR=r;
					pos=p;
					px=tx;
					py=ty;
				}
			}			
			ss=0xFFFF;
			if(pos<0xFFFE){				
				MinR=1000000;				
				for(int s=0;s<arm->NS;s++)if(NewPos[s]==0xFFFF){
					word SID=arm->SID[s];
					Squad* sqd=Sqd+SID;

					int r = Norma(px-sqd->xc,py-sqd->yc);
					if(r<MinR){
						MinR=r;
						ss=s;
					}
					
				}
				if(ss<0xFFFF){
					NewPos[ss]=pos;
					NewSqd[pos]=ss;
					Dist[ss]=MinR;
				}
			}
		}while(ss<0xFFFF);
	

		// mark squad on map
		char name[255];
		int tx,ty;
				
		ClearAZones();
		sprintf(name,"REAR");
		CreateAZone(name,xc,yc,128);

		for(s=0;s<arm->NS;s++){
			word SID=arm->SID[s];
			Squad* sqd=Sqd+SID;

			sqd->Top=arm->PID[NewPos[s]];			
			sqd->MoveToTop(DangerMap);
			
			SelectUnits(&sqd->Group,0);			
			sprintf(name,"Squad %d",s);
			CreateAGroup(NI,name);
			
			GetTopZRealCoor(arm->PID[NewPos[s]],&tx,&ty);
			CreateAZone(name,tx,ty,128);
		}		

	}
}

// Mind end //

// ActiveArmy begin //
void ActiveArmy::Clear(int ID){
	if(ID==-1) ID=Index;
	memset(this,0,sizeof *this);
	Index=ID;
	TopCenter=0xFFFF;
	memset(&Link,0xFF,sizeof Link);
};
// ActiveArmy end //

// Branch begin //
Branch::Branch(word nindex){
}
// Branch end //

// Weapon begin //
MWeapon::MWeapon(word wk){
}
// Weapon end //
