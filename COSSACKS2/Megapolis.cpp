// City organization module
#include "stdheader.h"
#include "unitability.h"
#include "BrigadeAbility.h"
//
void KeyTestMem();
extern int PeaceTimeLeft; 
extern City CITY[8];
bool CheckTowersPresence();
void CheckArmies(City* CT);

int CheckPositionForDanger(int x,int y,int z);
int GetTopology(int x,int y, byte LockType=0);
void A_BitvaLink(AI_Army* ARM);
void BuildWithSelected(byte NI,word ObjID,byte OrdType);
bool FindPortPlace(Nation* NT,int *xx,int *yy);
bool FindOilSpot(int *x,int *y,int r);
void AssignHint1(char*,int);
void SmartGamer(City* CT);
//extern byte NPresence[64][64];
int DHAPROB;
int DRECLPROB;
int DDIVPROB;
int HANSWPROB;
int HREQRADIUS;
int CROWDMIN;
int HCPROB;
int MaxAllowedComputerAI;
int CMC1;
int CMC2;
int CMC3;
int NumGid=0;
int NumFarm=0;


void SmartGamer(City* CT);
int BestForce;
int BestNat;
int NextBest;
byte NLocks[64][64];
Cell8x8::Cell8x8(){
	for(int i=0;i<5;i++)UnitsAmount[i]=0;
	Neighbor=0;
};
word FindTerrEnemy(int xCell,int yCell,int mx,int my,int dist,byte Mask);
//TotalCInfo  TCInf;

int Trx;
int Try;
int Trx1;
int Try1;
byte TerrMap[64][64];
int wer(byte x){
	if(x)return 1;
	else return 0;
};
void TerrSpot(int x,int y,byte c){
	if(x>0&&y>0)TerrMap[y][x]+=c;
};
void CreateTerrMap(byte NN){
};
//���� ������ ��� ����������
void Forces::SetDefaults(){
	MinX=55255;
	MaxX=0;
	MinY=55255;
	MaxX=0;
	Damage=0;
	Life=0;
	Shield=0;
	Population=0;
	NBuild=0;
	CenterX=0;
	CenterY=0;
	StrongFactor=0;
};
Forces NForces[8];
void GetForces(){
};

//����������� ��� ����������� ����� ��� ������
//1-����� ��� ������������ ������ (�����, �����,...)
int FUNC1(int* x,int* y,int Lx,int Ly,byte NI){
	return 0;
};
int FUNC3(int* x,int* y,int Lx,int Ly,byte NI){
	return 0;
};
//for Towers
int FUNC2(int* x,int* y,int Lx,int Ly,byte NI){
	return 0;
};
void OccupyCell(int x,int y,byte NI){

};
word FindPeasantInCell(int x,int y,byte NI){
	return 0xFFFF;
};
word FindPeasant(int x,int y,byte NI){
	return 0xFFFF;
};
word FindWPeasantInCell(int x,int y,byte NI){
	return 0xFFFF;
};
word FindWPeasant(int x,int y,byte NI){
	return 0xFFFF;
};
void FreeCell(int x,int y,byte NI){

};
CEXPORT
int GetSettlementPriory(byte NI,byte ResID){
	assert(NI<8&&ResID<6);
	return CITY[NI].SettlementPriory[ResID];
}

void KeyTestMem();
void TakeResources(UnitsGroup* UGR,byte Type,byte NI,byte ResType,GAMEOBJ* Zone);

void City::CreateCity(byte N){
	memset(this,0,sizeof City);
	NATIONS[N].CITY=this;
	Nat=&NATIONS[N];
	NI=N;
	CenterFound=false;
	PresentProject=false;
	FarmsUnderConstruction=0;
	NConstructions=0;
	NFields=0;
	AllowWood=false;
	AllowStone=false;
	AllowMine=false;
	INFORM=NULL;
	FieldReady=false;
	BestOnField=60;
	NDivr=0;
	MaxGuards=100;
	AbsMaxGuards=100;
	MaxInTransport=80;

	for(int i=0;i<MaxBrig;i++)Brigs[i].Init(this,i);
	for(int i=0;i<MaxArm;i++)ARMS[i].InitArmy(this);
	OnField.Enabled=true;
	OnStone.Enabled=true;
	OnWood.Enabled=true;
	Builders.Enabled=true;
	Free.Enabled=true;
	InMines.Enabled=true;
	FreeArmy.Enabled=true;
	Defenders.Enabled=true;
	Guards.Enabled=true;
	Agressors.Enabled=true;
	Settlers.Enabled=true;
	NAgressors=5;

	TransportID=0xFFFF;
	TransportSN=0xFFFF;

	Nat->SELO.Init();
	Nat->ARMY.Init();
	Nat->SCIENCE.Init();
	Nat->GENERAL.Init();
	AI_DEVELOPMENT=1;
	AI_WATER_BATTLE=1;
	AI_LAND_BATTLE=1;
	AI_MINES_CPTURE=1;
	AI_TOWN_DEFENCE=1;
	AI_MINES_UPGRADE=1;
	AI_FAST_DIVERSION=1;
	DestBldGroup=0xFFFF;
	DestEnemyGroup=0xFFFF;
	DestUnitsGroup=0xFFFF;
	NationalPeasantID=0xFFFF;
	DesantStartGroup=0xFFFF;
	DesantDestGroup=0xFFFF;
	AllPeasantsGroup=SCENINF.GetNewGroupIndex();
	LasyPeasantsGroup=SCENINF.GetNewGroupIndex();
	
	OnFoodGroup=SCENINF.GetNewGroupIndex();
	OnWoodGroup=SCENINF.GetNewGroupIndex();
	OnStoneGroup=SCENINF.GetNewGroupIndex();

	TakeResources(SCENINF.UGRP+OnFoodGroup,0,NI,FoodID,NULL);
	TakeResources(SCENINF.UGRP+OnWoodGroup,0,NI,TreeID,NULL);
	TakeResources(SCENINF.UGRP+OnStoneGroup,0,NI,StoneID,NULL);

	WoodSkladGroup=SCENINF.GetNewGroupIndex();
	StoneSkladGroup=SCENINF.GetNewGroupIndex();
	MelnGroup=SCENINF.GetNewGroupIndex();
	BridgeGroup=SCENINF.GetNewGroupIndex();
	
	NBridgeBuilders=0;

	AllWariorsGroup=0xFFFF;

	for(int j=0;j<6;j++)SettlementPriory[j]=100;

	NeedOnStone=1;
	NeedOnWood=1;
	NeedOnField=1;

	LFarmIndex=0;
	LFarmRadius=0;
	for(int i=0;i<8;i++)Nat->GENERAL.RESP[i]=100;
	MAX_WORKERS=Nat->DEFAULT_MAX_WORKERS;
	NDefn=0;
	MaxDefn=0;
	DefInf=NULL;
	DefArms=NULL;
	NDefArms=0;
	MaxDefArms=0;
	for(int i=0;i<128;i++)WaterBrigs[i]=0xFFFF;
	Nat->MU1G_PERCENT[0]=80;
	Nat->MU1G_PERCENT[1]=80;
	Nat->MU1G_PERCENT[2]=80;

	Nat->MU1I_PERCENT[0]=15;
	Nat->MU1I_PERCENT[1]=15;
	Nat->MU1I_PERCENT[2]=15;

	Nat->MU1C_PERCENT[0]=15;
	Nat->MU1C_PERCENT[1]=15;
	Nat->MU1C_PERCENT[2]=15;
//=============II==============//

	Nat->MU2G_PERCENT[0]=90;
	Nat->MU2G_PERCENT[1]=50;
	Nat->MU2G_PERCENT[2]=30;

	Nat->MU2I_PERCENT[0]=25;
	Nat->MU2I_PERCENT[1]=20;
	Nat->MU2I_PERCENT[2]=15;

	Nat->MU2C_PERCENT[0]=25;
	Nat->MU2C_PERCENT[1]=20;
	Nat->MU2C_PERCENT[2]=15;
//=============III==============//

	Nat->MU3G_PERCENT[0]=90;
	Nat->MU3G_PERCENT[1]=90;
	Nat->MU3G_PERCENT[2]=90;

	Nat->MU3I_PERCENT[0]=30;
	Nat->MU3I_PERCENT[1]=30;
	Nat->MU3I_PERCENT[2]=30;

	Nat->MU3C_PERCENT[0]=30;
	Nat->MU3C_PERCENT[1]=30;
	Nat->MU3C_PERCENT[2]=30;

	MyIsland=255;
};
void City::GetNextPlaceForFarm(int* x,int* y){
	int xx,yy;
	int szzz=msx<<5;
	if(LFarmRadius>70)LFarmRadius=0;
	//getting minimal topological distance on this radius
	int top0=GetTopology(CenterX<<7,CenterY<<7);
	if(top0>=0xFFFE)return;
	int MinTDS=100000;
	top0*=GetNAreas();
	char* xi=Rarr[LFarmRadius].xi;
	char* yi=Rarr[LFarmRadius].yi;
	int N=Rarr[LFarmRadius].N;
	for(int p=0;p<N;p++){
		int xi=Rarr[LFarmRadius].xi[LFarmIndex]*FarmLX;
		int yi=Rarr[LFarmRadius].yi[LFarmIndex]*FarmLY;
		xx=(CenterX<<7)+((xi+yi)>>1);
		yy=(CenterY<<7)+((yi-xi)>>1);
		if(xx>0&&xx<szzz&&yy>0&&yy<szzz){
			int top=SafeTopRef((xx>>6),(yy>>6));
			if(top<0xFFFE){
				int dst=GetLinksDist(top0+top);
				if(dst<MinTDS)MinTDS=dst;
			};
		};
	};
	if(MinTDS>=0xFFFE)MinTDS=0;
	do{
		this->LFarmIndex++;
		if(LFarmIndex>=Rarr[LFarmRadius].N){
			LFarmIndex=0;
			LFarmRadius++;
		};
		if(LFarmRadius>=60){
			LFarmIndex=0;
			LFarmRadius=60;
			*x=0;
			*y=0;
			return;
		};
		int xi=Rarr[LFarmRadius].xi[LFarmIndex]*FarmLX;
		int yi=Rarr[LFarmRadius].yi[LFarmIndex]*FarmLY;
		xx=(CenterX<<7)+((xi+yi)>>1);
		yy=(CenterY<<7)+((yi-xi)>>1);
		if(xx>0&&xx<szzz&&yy>0&&yy<szzz){
			if(!GNFO.EINF[NI]->GetSafeVal(xx>>7,yy>>7)){
				int top=SafeTopRef((xx>>6),(yy>>6));
				if(top<0xFFFE){
					int dst=GetLinksDist(top0+top);
					if(dst-MinTDS<30){
						*x=xx;
						*y=yy;
						return;
					};
				};
			};
		};
	}while(true);
};
int  City::GetFreeBrigade(){
	for(int i=0;i<MaxBrig-9;i++)if(!Brigs[i].Enabled){
		if(Brigs[i].Ability)
			delete Brigs[i].Ability;
		memset(Brigs+i,0,sizeof Brigade);
		Brigs[i].CT=this;
		Brigs[i].ID=i;
		Brigs[i].SN=rando();
		//Brigs[i].BOrder=NULL;
		Brigs[i].NewBOrder=NULL;
		Brigs[i].ArmyID=0xFFFF;
		Brigs[i].AutoFormID=0xFFFF;
		Brigs[i].LastTopology=0xFFFF;
		Brigs[i].LastEnemyID=0xFFFF;
		Brigs[i].LastEnemySN=0xFFFF;
		Brigs[i].AddMaxMorale=0;
		Brigs[i].PrevMorale=0;
		Brigs[i].OnlyThisBrigadeToKill=0xFFFF;
		Brigs[i].NIOnlyThisBrigadeToKill=0;
		Brigs[i].vLastEnemyID=0xFFFF;
		Brigs[i].vLastEnemyNI=NI;
		Brigs[i].ScaleFactor=100;
		Brigs[i].AddAttackRange=0;
		Brigs[i].UnitsSpeedBonus=100;
		Brigs[i].ExpGrowSpeed=100;
		Brigs[i].AttackRateBonus=100;
		if(i>=NBrigs)NBrigs=i+1;
		return i;
	};
	return -1;
};
void City::AddInform(Inform* Inf,I_Clear* ICL){
	Inf->Next=INFORM;
	Inf->Previous=NULL;
	Inf->CT=this;
	Inf->IClr=ICL;
	if(INFORM){
		INFORM->Previous=Inf;
	};
	INFORM=Inf;
};
Inform* City::SearchInform(word ID,word Essence,Inform* Inf){
	if(!Inf)Inf=INFORM;
	while(Inf&&(Inf->ID!=ID||Essence!=Inf->Essence))Inf=Inf->Next;
	return Inf;
};
Inform* City::SearchInform(word ID,Inform* Inf){
	if(!Inf)Inf=INFORM;
	else Inf=Inf->Next;
	while(Inf&&Inf->ID!=ID)Inf=Inf->Next;
	return Inf;
};
void City::DelInform(Inform* INF){
	if(INF->IClr)INF->IClr(INF);
	if(INF->Previous){
		INF->Previous->Next=INF->Next;
	}else{
		INFORM=INF->Next;
	};
	if(INF->Next)INF->Next->Previous=INF->Previous;
	free(INF);
};
void City::DelInform(){
	while(INFORM){
		if(INFORM->IClr)INFORM->IClr(INFORM);
		INFORM=INFORM->Next;
	};
};
Inform* SearchInform(word ID,word Essence,Inform* inf);

void TakeResLink(OneObject* OBJ);
void BuildObjLink(OneObject* OBJ);
void NewMonsterSendToLink(OneObject* OB);
void NewMonsterSmartSendToLink(OneObject* OBJ);
bool PUSE(OneObject* OB){
	return OB->UnlimitedMotion||CheckBar(OB->x,OB->y,OB->Lx,OB->Lx);
};
int CheckBuilders(OneObject* OB,City* CT){
	if(PUSE(OB))return -1;
	OB->DoNotCall=true;
	//OB->NoBuilder=true;
	if(OB->LocalOrder){
		Order1* OR1=OB->LocalOrder;
		if(OR1&&OR1->DoLink==NewMonsterSendToLink&&OR1->NextOrder)OR1=OR1->NextOrder;
		if(OR1&&OR1->DoLink==NewMonsterSmartSendToLink&&OR1->NextOrder)OR1=OR1->NextOrder;
		if(OR1->DoLink!=&BuildObjLink)return CT->Free.ID;
	}else return CT->Free.ID;
	return -1;
};
int CheckFree(OneObject* OB,City* CT){
	if(PUSE(OB))return -1;
	OB->DoNotCall=false;
	if(OB->LocalOrder){
		Order1* OR1=OB->LocalOrder;
		if(OR1&&OR1->DoLink==NewMonsterSendToLink&&OR1->NextOrder)OR1=OR1->NextOrder;
		if(OR1&&OR1->DoLink==NewMonsterSmartSendToLink&&OR1->NextOrder)OR1=OR1->NextOrder;
		if(OR1->DoLink==&BuildObjLink)return CT->Builders.ID;
		if(OR1->DoLink==&TakeResLink){
			if(OR1->info.TakeRes.ResID==FoodID)return -1;
		};
	};
	return -1;
};
int CheckFieldWorker(OneObject* OB,City* CT){
	if(OB->EnemyID!=0xFFFF&&rando()>1000)return -1;
	if(!CT->FieldReady)return -1;
	//if(CT->OnField.NMemb>35){
	//	if(rando()>25000)OB->DoNotCall=true;
	//	else OB->DoNotCall=false;
	//};
	if(PUSE(OB))return -1;
	OB->DoNotCall=false;
	if(OB->LocalOrder){
		Order1* OR1=OB->LocalOrder;
		if(OR1&&OR1->DoLink==NewMonsterSendToLink&&OR1->NextOrder)OR1=OR1->NextOrder;
		if(OR1&&OR1->DoLink==NewMonsterSmartSendToLink&&OR1->NextOrder)OR1=OR1->NextOrder;
		if(OR1->DoLink==&BuildObjLink)return CT->Builders.ID;
		if(OR1->DoLink==&TakeResLink){
			if(OR1->info.TakeRes.ResID==FoodID)return -1;
		};
	};
	for(int j=0;j<20;j++){
		int k=(int(rando())*CT->NFields)>>15;
		OneSprite* OS=&Sprites[CT->FieldsID[k]];
		if(OS->Enabled){
			OB->TakeResource(OS->x,OS->y,FoodID,128,0);
			return -1;
		};
	};
	return -1;
};
Area* GetAr(int rx,int ry){
	int tx=rx>>10;
	int ty=ry>>10;
	int ofst=tx+(ty<<TopSH);
	if(ofst>=0&&ofst<MaxTop){
		word ti=GetTopRef(ofst);
		Area* AR=NULL;
		bool OurZone=false;
		if(ti!=0xFFFF){
			return GetTopMap(ti);
		};
	};
	return NULL;
};
bool CheckPL(int rx,int ry){
	return !CheckBar((rx>>8)-2,(ry>>8)-2,5,5);
};
int Norma(int,int);
int CheckFreeArmy(OneObject* OB,City* CT){
	return -1;
	Order1* OR1=OB->LocalOrder;
	if(OR1&&OR1->DoLink==NewMonsterSendToLink&&OR1->NextOrder)OR1=OR1->NextOrder;
	if(OR1&&OR1->DoLink==NewMonsterSmartSendToLink&&OR1->NextOrder)OR1=OR1->NextOrder;
	if(OR1&&OR1->DoLink!=&NewMonsterSendToLink)return true;
	int tx=OB->RealX>>10;
	int ty=OB->RealY>>10;
	int ofst=tx+(ty<<TopSH);
	if(ofst>=0&&ofst<MaxTop){
		word ti=GetTopRef(ofst);
		Area* AR=NULL;
		bool OurZone=false;
		if(ti!=0xFFFF){
			AR=GetTopMap(ti);
		};
		if(AR){
			if(AR->SINF[OB->NNUM].BuildInfo)OurZone=true;
		};
		if(!OurZone){
			if(!OR1){
				//search nearest our zone
				int Cur=0xFFFF;
				int Dis=10000000;
				int NA=GetNAreas();
				for(int j=0;j<NA;j++){
					Area* AR2=GetTopMap(j);
					int dis=Norma(tx-AR2->x,ty-AR2->y);
					if(dis<Dis){
						Dis=dis;
						Cur=j;
					};
				};
				if(Cur!=0xFFFF){
					Area* AR=GetTopMap(Cur);
					OB->NewMonsterSendTo(int(AR->x)<<10,int(AR->y)<<10,0,0);
					return true;
				};
			};
		}else{
			//search for the next place
			if(OR1&&rando()>3000)return true;
			int rx=OB->RealX;
			int ry=OB->RealY;
			int dr=16*256;
			byte NI=OB->NNUM;
			for(int j=0;j<5;j++){
				byte dir1=OB->RealDir+(rando()&63)-32;
				int x1=rx+((int(TCos[dir1])*dr)>>8);
				int y1=ry+((int(TCos[dir1])*dr)>>8);
				Area* AR=GetAr(x1,y1);
				if(AR&&AR->SINF[NI].BuildInfo&&CheckPL(x1,y1)){
					OB->NewMonsterSendTo(x1,y1,0,0);
					return true;
				};
			};
			for(int j=0;j<5;j++){
				byte dir1=OB->RealDir+(rando()&127)-64;
				int x1=rx+((int(TCos[dir1])*dr)>>8);
				int y1=ry+((int(TCos[dir1])*dr)>>8);
				Area* AR=GetAr(x1,y1);
				if(AR&&AR->SINF[NI].BuildInfo&&CheckPL(x1,y1)){
					OB->NewMonsterSendTo(x1,y1,0,0);
					return true;
				};
			};
			for(int j=0;j<5;j++){
				byte dir1=rando()&255;
				int x1=rx+((int(TCos[dir1])*dr)>>8);
				int y1=ry+((int(TCos[dir1])*dr)>>8);
				Area* AR=GetAr(x1,y1);
				if(AR&&AR->SINF[NI].BuildInfo&&CheckPL(x1,y1)){
					OB->NewMonsterSendTo(x1,y1,0,0);
					return true;
				};
			};
		};
	};
	return true;
};
int Norma(int,int);

int CheckWoodWorker(OneObject* OB,City* CT){
	//if(!CT->FieldReady)return true;
	if(OB->EnemyID!=0xFFFF&&rando()>1000)return -1;
	if(PUSE(OB))return -1;
	OB->DoNotCall=false;
	OB->NoBuilder=false;
	if(OB->LocalOrder){
		Order1* OR1=OB->LocalOrder;
		if(OR1&&OR1->DoLink==NewMonsterSendToLink&&OR1->NextOrder)OR1=OR1->NextOrder;
		if(OR1&&OR1->DoLink==NewMonsterSmartSendToLink&&OR1->NextOrder)OR1=OR1->NextOrder;
		if(OR1->DoLink==&BuildObjLink)return CT->Builders.ID;
		if(OR1->DoLink==&TakeResLink){
			if(OR1->info.TakeRes.ResID==TreeID)return -1;
		};
	};
	{
		//1.Nearest
		OneObject* OBS=NULL;
		int MinR=1000000;
		int Best=-1;
		UnitsGroup* UG=&SCENINF.UGRP[CT->WoodSkladGroup];
		for(int i=0;i<UG->N;i++){
			word MID=UG->IDS[i];
			if(MID!=0xFFFF){
				OneObject* OBS=Group[MID];
				if(OBS){
					int RR=Norma(OB->RealX-OBS->RealX,OB->RealY-OBS->RealY);
					if(RR<MinR){
						MinR=RR;
						Best=i;
					}
				}
			}
		}
		if(Best!=-1){
			OBS=Group[UG->IDS[Best]];
			int xx=OBS->RealX>>4;
			int yy=OBS->RealY>>4;
			int xxx=xx+(rando()&511)-256;
			int yyy=yy+(rando()&511)-256;
			OB->TakeResource(xxx,yyy,TreeID,128,0);
			return -1;
		};
	};
	return -1;
};
int CheckStoneWorker(OneObject* OB,City* CT){
	if(OB->EnemyID!=0xFFFF&&rando()>1000)return -1;
	//if(!CT->FieldReady)return true;
	if(PUSE(OB))return -1;
	OB->DoNotCall=false;
	OB->NoBuilder=false;
	if(OB->LocalOrder){
		Order1* OR1=OB->LocalOrder;
		if(OR1&&OR1->DoLink==NewMonsterSendToLink&&OR1->NextOrder)OR1=OR1->NextOrder;
		if(OR1&&OR1->DoLink==NewMonsterSmartSendToLink&&OR1->NextOrder)OR1=OR1->NextOrder;
		if(OR1->DoLink==&BuildObjLink)return CT->Builders.ID;
		if(OR1->DoLink==&TakeResLink){
			if(OR1->info.TakeRes.ResID==StoneID)return -1;
		};
	};
	{
		//1.Nearest
		OneObject* OBS=NULL;
		int MinR=1000000;
		int Best=-1;
		UnitsGroup* UG=&SCENINF.UGRP[CT->StoneSkladGroup];
		for(int i=0;i<UG->N;i++){
			word MID=UG->IDS[i];
			if(MID!=0xFFFF){
				OneObject* OBS=Group[MID];
				if(OBS){
					int RR=Norma(OB->RealX-OBS->RealX,OB->RealY-OBS->RealY);
					if(RR<MinR){
						MinR=RR;
						Best=i;
					}
				}
			}
		}
		if(Best!=-1){
			OBS=Group[UG->IDS[Best]];
			int xx=OBS->RealX>>4;
			int yy=OBS->RealY>>4;
			int xxx=xx+(rando()&511)-256;
			int yyy=yy+(rando()&511)-256;
			OB->TakeResource(xxx,yyy,StoneID,128,0);
			return -1;
		};
	};
	return -1;
};

bool City::CheckTZone(int x,int y,int Lx,int Ly){
	return true;
};
void City::CloseCity(){
	for(int i=0;i<MaxBrig;i++)if(Brigs[i].Enabled)Brigs[i].DeleteAll();
	for(int i=0;i<MaxArm;i++)if(ARMS[i].Enabled)ARMS[i].ClearArmy();
	DelInform();
	DelIdeas();
	memset(this,0,sizeof City);
	if(NDefn){
		for(int i=0;i<NDefn;i++){
			if(DefInf[i].Def){
				free(DefInf[i].Def);
				free(DefInf[i].DefSN);
			};
		};
	};
	if(DefInf)free(DefInf);
	DefInf=NULL;
	NDefn=0;
	MaxDefn=0;
	NDivr=0;
	for(int i=0;i<NGroups;i++)free(GroupsSet[i]);
	if(GroupsSet){
		free(GroupsSet);
		free(NGroupsInSet);
	};
	GroupsSet=NULL;
	NGroupsInSet=NULL;
	NGroups=0;
	DestBldGroup=0xFFFF;
	TheyMovesInto.Clear();
	MoversIndices.Clear();
	TheyMovesInto_prev.Clear();
	MoversIndices_prev.Clear();
};
bool City::TryToFindPlace(int* x,int* y,int Lx,int Ly,byte Kind){
		return true;
};
void MakeShipBattle(Brigade* BR);
void MakeDiversion(Brigade* BR);
void SearchArmy(OneObject* OB);
void City::RegisterNewUnit(OneObject* OB){
	
	if(!Nat->AI_Enabled)return;
	if(Nat->CITY->DestUnitsGroup!=0xFFFF&&!OB->LockType)return;
	if(OB){
		//if(OB->NewBuilding&&rando()<28000)OB->AutoKill=true;

		if(OB->newMons->Peasant){
			//if(PUSE(OB))return;
			OB->Borg=1;
			//Free.AddObject(OB);			
            //SCENINF.UGRP[AllPeasantsGroup].AddNewUnit(OB);
			SCENINF.UGRP[LasyPeasantsGroup].AddNewUnit(OB);
		}
		else if(!OB->NewBuilding){
			OB->Borg=1;
			SCENINF.UGRP[AllWariorsGroup].AddNewUnit(OB);
			if(OB->LockType==1&&OB->newMons->MaxInside){
				OB->Borg=1;
				int ID=GetFreeBrigade();
				if(ID!=-1){
					Brigade* BR=Brigs+ID;
					BR->Enabled=true;
					BR->AddObject(OB);
					MakeDiversion(BR);
				};
				return;
			};

			/*
			if(OB->newMons->Officer||OB->newMons->Baraban){
				SearchArmy(OB);
				OB->Borg=1;
				OB->DoNotCall=1;
				OB->NoBuilder=1;
				return;
			};*/
			/*if(OB->newMons->Transport){	//FlameMaster - Adding transport to bigade statements
				OB->Borg=1;
				int ID=GetFreeBrigade();
				if(ID!=-1){
					Brigade* BR=Brigs+ID;
					BR->Enabled=true;
					BR->AddObject(OB);
					MakeDiversion(BR);
				};
				return;
			};*/
			/*if(!(OB->newMons->Building||OB->LockType)){
				if(PUSE(OB))return;
				OB->Borg=1;
				byte use=OB->newMons->Usage;
				bool AGRES=false;
				if(use==PushkaID||use==MortiraID||use==FastHorseID||use==SupMortID
					||use==GrenaderID||use==HardHorceID||use==StrelokID||use==ArcherID
					||use==HorseStrelokID)AGRES=true;
				if(!AI_TOWN_DEFENCE)AGRES=true;
				if(use==SupMortID){
					OB->Zombi=true;
				}else{
					if(AGRES){
						Agressors.AddObject(OB);
						OB->DoNotCall=true;
						//OB->AutoKill=true;
					}else{
						if(FreeArmy.NMemb>15&&Guards.NMemb>MaxGuards){
							if(rando()<2048&&Guards.NMemb<AbsMaxGuards)Defenders.AddObject(OB);
							else Agressors.AddObject(OB);
							OB->DoNotCall=true;
						}else{
							if(rando()<10000&&FreeArmy.NMemb<20)FreeArmy.AddObject(OB);
							else if(rando()<6000&&Guards.NMemb<100)Defenders.AddObject(OB);
							else{
								Agressors.AddObject(OB);
								OB->DoNotCall=true;
							};
						};
					};
				};
			};
			if(OB->LockType&&AI_WATER_BATTLE){
				byte use=OB->newMons->Usage;
				if(use!=FisherID){
					int id=GetFreeBrigade();
					if(id!=-1){
						Brigade* BR=Brigs+id;
						RegisterWaterBrigade(id);
						BR->Enabled=true;
						BR->AddObject(OB);
						BR->Direction=0;
						if(OB->newMons->Usage==GaleraID){
							BR->Direction=1;
						};
						MakeShipBattle(BR);
						OB->Borg=1;
					};
				};
			};			*/
		};
	};
};
void CheckArmies(City*);
void City::UnRegisterNewUnit(OneObject* OB){
	//if(!Nat->AI_Enabled)return;
	
	if(OB&&OB->BrigadeID!=0xFFFF&&!OB->NewBuilding){
		Brigade* BR=Brigs+OB->BrigadeID;
		if(!(BR->NMemb&&BR->Enabled)){
			OB->BrigadeID=0xFFFF;
			OB->InArmy=false;
			return;
		};
		word* Memb=BR->Memb;
		word* MembSN=BR->MembSN;
		int NMemb=BR->NMemb;
		int Index=OB->BrIndex;
		//assert(BR->NMemb<8000);
		if(Index<NMemb-1){
			if(OB->InArmy||BR->WarType){
				Memb[Index]=0xFFFF;
				MembSN[Index]=0xFFFF;
			}else{
				memcpy(Memb+Index,Memb+Index+1,(NMemb-Index-1)<<1);
				memcpy(MembSN+Index,MembSN+Index+1,(NMemb-Index-1)<<1);
				if(BR->PosCreated){
					memcpy(BR->posX+Index,BR->posX+Index+1,(NMemb-Index-1)<<2);
					memcpy(BR->posY+Index,BR->posY+Index+1,(NMemb-Index-1)<<2);
				};
				BR->NMemb--;
			};
		}else{
			if(OB->InArmy||BR->WarType){
				Memb[Index]=0xFFFF;
				MembSN[Index]=0xFFFF;
			}else{
				BR->NMemb--;
			};
		}; 
		//assert(BR->NMemb<8000);
		(&BR->BM.Peons)[GetBMIndex(OB)]--;
		OB->BrigadeID=0xFFFF;
		OB->InArmy=false;
		BR->SetIndex();
		if(OB->InArmy){
			int N=0;
			for(int j=0;j<NMemb;j++){
				if(Memb[j]!=0xFFFF)N++;
			};
			if(!N){
				if(BR->Memb){
					free(BR->Memb);
					free(BR->MembSN);
					BR->Memb=NULL;
					BR->MembSN=NULL;
				};
				BR->MaxMemb=0;
				BR->NMemb=0;
				BR->Enabled=false;
				memset(&BR->BM,0,sizeof BR->BM);
			};
		};
	};
};
void EraseBrigade(Brigade* BR);
void SearchArmy(OneObject* OB);
void TakeResourceFromSpriteLink(OneObject* OBJ);
bool InCEnum=0;
bool CanProduce=0;
bool REGI=false;
int __xc;
int __yc;
word Prodradius[2048];
bool BUNT;
int NBUILDERS;

void City::EnumOneUnit(OneObject* OB){
	extern bool GecOptimCityEnum;
	if(GecOptimCityEnum) return;
    if(OB&&OB->NNUM==NI&&OB->GetLiveState()==ls_LiveBody){
        addrand(OB->Index);
        addrand(OB->NIndex);
        if(REGI&&!(OB->Borg||OB->NewBuilding)){
            if(OB->BrigadeID==0xFFFF)RegisterNewUnit(OB);
            else OB->Borg=1;
        };
        UnitAmount[OB->BirthNIndex]++;
        AdvCharacter* ACR=OB->MoreCharacter;
        NewMonster* NM=(OB->ActiveAbility==NULL)?OB->newMons:OB->ActiveAbility->BornNM;
        if(REGI&&(NM->Officer||NM->Baraban)){
            if(OB->BrigadeID==0xFFFF&&!OB->LocalOrder){
                SearchArmy(OB);
            };
        };
        if(OB->Wall){
            if(OB->Ready)ReadyAmount[OB->NIndex]++;
        }else
            if((NM->Building&&OB->Stage>=ACR->ProduceStages)||(!NM->Building)){
                ReadyAmount[OB->NIndex]++;
                NewMonster* NM=OB->newMons;
                GeneralObject* GO=OB->Ref.General;
                if(NM->Farm)NumFarm+=OB->MoreCharacter->NInFarm+OB->AddFarms;
                NumFarm+=GO->NewNFarm;
                //if(GO->NFarmsPerSettlement)Nat->NFarms+=GO->NFarmsPerSettlement*NSettl;
				extern int NumGid;
                if(!(NM->Building||NM->NoFarm||(OB->GetLiveState()!=ls_LiveBody)))NumGid++;//Nat->NGidot++;
            };
        if(OB->Ready||OB->LocalOrder){
            if(NM->ArtDepo){
                for(int i=0;i<MaxArtDep;i++)Nat->NArtdep[i]+=NM->ArtCap[i];
            }
            if(NM->ArtSet)Nat->NArtUnits[NM->ArtSet-1]++;
        };
        if(OB->Ready) Nat->ResSpeed[NM->ResConsID]+=NM->ResConsumer;
        word NIND=OB->NIndex;
        if(OB->Ready){
            if((!OB->LocalOrder)||(OB->LocalOrder&&OB->LocalOrder->DoLink==&TakeResourceFromSpriteLink)){
                UnBusyAmount[NIND]++;
                int R=Norma((OB->RealX>>11)-__xc,(OB->RealY>>11)-__yc);
                addrand(Producer[NIND]);
                addrand(Prodradius[NIND]);
                if((rando()&32)||Producer[NIND]==0xFFFF||R<Prodradius[NIND]){
                    Producer[NIND]=OB->Index;
                    Prodradius[NIND]=R;
                };
            };
            if(OB->NewBuilding&&NBuild<64){
                BUILDLISTID[NBuild]=OB->Index;
                BUILDLISTSN[NBuild]=OB->Serial;
                NBuild++;
            };
        };
        if(NIND>MaxType)MaxType=NIND;
        if(NIND<MinType)MinType=NIND;
        OB->TurnOff=false;
        if(BUNT){
            switch(NM->Behavior){
                case 1://TURNOFF
                    OB->TurnOff=true;
                    break;
                case 2:
                    if(OB->NNUM==MyNation)AssignHint1(GOLDFIN,200,32);
                    NATIONS[OB->NNUM].CITY->Account-=EngSettings.LooseModulator*OB->newMons->Expa;
                    OB->NNUM=7;
                    OB->NMask=1<<7;
                    OB->Ref.General=NATIONS[7].Mon[OB->NIndex];
                    OB->Nat=NATIONS+7;
                    break;
            };
            //if(OB->InArmy&&OB->BrigadeID!=0xFFFF){
            //	Brigade* BR=OB->Nat->CITY->Brigs+OB->BrigadeID;
            //	EraseBrigade(BR);
            //};
        };
        if(NI==MyNation&&!CanProduce){
            CanProduce=OB->Nat->PACount[OB->NIndex]!=0;
        }
    }
}
void City::StartEnumUnits(){
	if(NI==MyNation)CanProduce=0;
	InCEnum=1;
	NBuild=0;
	OneObject* OB;
	memset(UnitAmount,0,sizeof UnitAmount);
	memset(ReadyAmount,0,sizeof ReadyAmount);
	memset(UnBusyAmount,0,sizeof UnitAmount);
	memset(PRPIndex,255,sizeof PRPIndex);
	memset(UPGIndex,255,sizeof UPGIndex);
	Amount=0;
	//Nat->NFarms=0;
	//Nat->NGidot=0;
	NumGid=0;
	NumFarm=0;
	BUNT=Nat->GoldBunt;
	NBUILDERS=0;
	REGI=Nat->AI_Enabled;

	for(int i=0;i<8;i++)Nat->ResSpeed[i]=0;
	for(int i=0;i<MaxArtDep;i++){
		Nat->NArtUnits[i]=0;
		Nat->NArtdep[i]=0;
	}
	__xc=CenterX;
	__yc=CenterY;	
	memset(Prodradius,0xFF,sizeof Prodradius);
	CEXPORT int vdf_GetAmountOfSettlements(byte Owner,	int ResType, bool CheckUpgrades, int Level);
	//int NSettl=0;
	//for(int r=0;r<6;r++)NSettl+=vdf_GetAmountOfSettlements(NI,r,0,0);
	addrand(MAXOBJECT);		
	InCEnum=0;
};
void City::PostEnumUnits(){
	Nat->NFarms=NumFarm+Nat->AddFarms;
    addrand(Nat->NFarms);
    extern word NPlayers;
    if( Nat->NFarms>GSets.CGame.UnitLimit && (NPlayers>1||NI==MyNation) )
        Nat->NFarms=GSets.CGame.UnitLimit;
	Nat->NGidot=NumGid;
	NumGid=0;
}
void City::EnumUnits(){
	int nf=NumFarm;
    StartEnumUnits();
    int N=NtNUnits[NI];
    for(int i=0;i<N;i++){
        word ID=NatList[NI][i];
        if(ID!=0xFFFF){
            OneObject* OB=Group[ID];
            EnumOneUnit(OB);
        }
    }
    PostEnumUnits();
	NumFarm=nf;
}
void City::EnumAllUnits(){
	int nf=NumFarm;
    for(int i=0;i<8;i++){
        CITY[i].StartEnumUnits();
		int N=NtNUnits[i];
		word* UList=NatList[i];
		for(int j=0;j<N;j++)if(UList[j]!=0xFFFF){
			OneObject* OB=Group[UList[j]];
			if(OB){
				CITY[i].EnumOneUnit(OB);
			}
		}
		CITY[i].PostEnumUnits();
    }    
	NumFarm=nf;
}
void City::AddProp(byte NI,word NIN,GeneralObject* GO,word prod,word per){
	if(NProp>=100)return;
	//if(GO->MaxAutoAmount<=UnitAmount[NIN])return;
	ProposedProject* PR=&Prop[NProp];
	NProp++;
	PR->PKind=0;
	PR->NIndex=NIN;
	PR->Percent=per;
	PRPIndex[NIN]=NProp-1;
	//PR->Cost=GO->cost;
	PR->ProducerIndex=prod;
};

void City::AddUpgr(byte NI,word UIN,word prod,word per){
	if(NProp>=100)return;
	for(int i=0;i<NProp;i++){
		ProposedProject* PRP=&Prop[i];
		if(PRP->NIndex==UIN&&PRP->PKind==1)return;
	};
	ProposedProject* PR=&Prop[NProp];
	NProp++;
	PR->PKind=1;
	PR->NIndex=UIN;
	PR->Percent=per;
	UPGIndex[UIN]=NProp-1;
	PR->ProducerIndex=prod;
};

void GetUnitCost(byte NI,word NIndex,int* Cost,int x,int y);
void City::EnumProp(){
	NProp=0;
	//enumerating monsters could be produced 
	int PC;
	//int* RESU=&RESRC[Nat->NNUM][0];
	GeneralObject** GOA=Nat->Mon;
	for(int i=MinType;i<=MaxType;i++){
		if(UnBusyAmount[i]) PC=Nat->PACount[i];
		else{
			if(GOA[i]->newMons->Usage==PeasantID)PC=Nat->PACount[i];
			else PC=0;
		};
		for(int j=0;j<PC;j++){
			word s=Nat->PAble[i][j];
			//��������� ����������� ������������
			GeneralObject* GO=Nat->Mon[s];
			bool Able=true;
			OneObject* POB=Group[Producer[i]];
			if(GO->newMons->Peasant){
				if(MAX_WORKERS<FreePS)Able=false;
			};
			if(GO->StageMask){
				byte m=GO->StageMask;
				byte s=POB->StageState;
				for(int i=0;i<5;i++){
					if(m&1){
						byte s1=s&7;
						if(s1!=2)Able=0;
					};
					m>>=1;
					s>>=3;
				};
			};
			if(GO->Enabled&&Able){
				int COST[8];
				GetUnitCost(Nat->NNUM,s,COST,-1,-1);
				for(int j=0;j<8&&COST[j]<=XRESRC(Nat->NNUM,j);j++);
				if(j==8){
					int maxper=Nat->GENERAL.GetMonsterCostPercent(NI,s);;
					AddProp(NI,s,GO,Producer[i],maxper);
				};
			};
		};
		
		GeneralObject* GO=GOA[i];
		if(UnBusyAmount[i]) PC=GO->NUpgrades;
		else PC=0;
		for(int j=0;j<PC;j++){
			word s=GO->Upg[j];
			//��������� ����������� ������������
			NewUpgrade* NU=Nat->UPGRADE[s];
			OneObject* POB=Group[Producer[i]];
			if(POB){
				bool OKK=false;
				if(NU->Individual||NU->StageUp){
#ifdef CONQUEST
					if(NU->StageUp){
						byte st=(POB->StageState>>((NU->CtgUpgrade-24)*3))&7;
						OKK=POB->Ready&&st!=2&&NU->Enabled&&((!POB->NewBuilding)||POB->Life>1999)&&!POB->InFire;
					};
					if(NU->StageMask){
						byte m=NU->StageMask;
						word st=POB->StageState;
						for(int i=0;i<5;i++){
							if(m&1){
								byte cs=st&7;
								if(cs!=2)OKK=false;
							};
							m >>=1;
							st>>=3;
						};
					};
#endif
					if(NU->Individual)OKK=POB->Ready&&POB->SingleUpgLevel==NU->Level&&NU->Enabled;
				}else{
					OKK=NU->Enabled&&!NU->IsDoing;
				};
				if(POB->InFire&&NU->CtgUpgrade>=24&&NU->CtgUpgrade<=28)OKK=false;
				if(OKK){
					word* COST=NU->Cost;
					for(int j=0;j<8&&COST[j]<=XRESRC(Nat->NNUM,j);j++);
					if(j==8){
						int maxper=Nat->GENERAL.GetUpgradeCostPercent(NI,s);
						/*
						switch(GO->Branch){
						case 0://SELO
						maxper=Nat->SELO.GetUpgradeCostPercent(NI,s);
						break;
						case 1://ARMY
						maxper=Nat->ARMY.GetUpgradeCostPercent(NI,s);
						break;
						case 2://SCIENCE
						maxper=Nat->SCIENCE.GetUpgradeCostPercent(NI,s);
						break;
						case 255:
						maxper=Nat->GENERAL.GetUpgradeCostPercent(NI,s);
						break;
						};
						*/
						AddUpgr(NI,s,Producer[i],maxper);
					};
				};
			}
		};
	};
};

#define FARMID 20
bool InAI=0;
word City::FindNeedProject(){
	//Groups calculating
	Nation* NT=Nat;
	NBestProjects=0;
	if(AllWariorsGroup!=0xFFFF){
		SCENINF.UGRP[AllWariorsGroup].RefreshUnits();
	}
	// process mine
	//
	void ProcessScriptForNation(int NI);
	ProcessScriptForNation(NI);
	return NBestProjects;
	//	
	if(NT->hLibAI&&NT->ProcessAIinDLL){
		NBestProjects=0;
		AiIsRunNow=true;
		CCIT=this;
		CNAT=Nat;
		CurAINation=Nat->NNUM;
		InAI=1;
		//assert(_CrtCheckMemory());
		NT->ProcessAIinDLL();
		//assert(_CrtCheckMemory());
		InAI=0;
		AiIsRunNow=false;
		return NBestProjects;
	};
	
	int MaxAI=-1;
	for(int i=0;i<NT->N_AI_Levels;i++){
		bool OK=true;
		int nrq=NT->N_AI_Req[i];
		for(int p=0;p<nrq;p++){
			sAI_Req* SRQ=&NT->AI_Req[i][p];
			switch(SRQ->Kind){
				case 0://unit
					if(UnitAmount[SRQ->ObjID]<SRQ->Amount)OK=false;
					break;
				case 1://upgrade
					if(!NT->UPGRADE[SRQ->ObjID]->Done)OK=false;
					break;
				case 16://unit cost
					break;
				case 17://upgrade cost
					break;

/*
				case 2://group
					if(NT->GAmount[SRQ->ObjID]<SRQ->Amount)OK=false;
					break;
*/
			};

		};
		if(OK)MaxAI=i;
	};
	NT->AI_Level=MaxAI;
	//Let us use current AI method
	//----Checking farms
	/*
	int NFNeed=div(NT->NGidot,15).quot+1;
	if(NFNeed>1)NFNeed++;
	if(NFNeed>5)NFNeed++;
	if(NFNeed>10)NFNeed++;
	int NF=NT->NFarms;
	if(NFNeed>UnitAmount[FARMID]){
		word FPR=PRPIndex[FARMID];
		if(FPR!=0xFFFF)return FPR;
	};
	*/
	if(MaxAI==-1)return 0;
	//------execute the command------
	sAI_Cmd* SCM=NT->AI_Cmd[MaxAI];
	int NCM=NT->N_AI_Cmd[MaxAI];
	MAX_WORKERS=NT->DEFAULT_MAX_WORKERS;
	for(int i=0;i<NCM;i++){
		switch(SCM->Kind){
		case 1://SELO
			memcpy(NT->SELO.RESP,SCM->Info,16);
			break;
		case 2://ARMY
			memcpy(NT->ARMY.RESP,SCM->Info,16);
			break;
		case 3://SCIENCE
			memcpy(NT->SCIENCE.RESP,SCM->Info,16);
			break;
		case 4://MAX_WORKERS
			MAX_WORKERS=SCM->Info[0];
			break;
		};
		SCM++;
	};
	//----Checking other units by script
	sAI_Devlp* SAD=NT->AI_Devlp[MaxAI];
	word ndev=NT->N_AI_Devlp[MaxAI];
	int npro=0;
	for(int i=0;i<ndev;i++){
		sAI_Devlp* sad=&SAD[i];
		if(!sad->Kind){
			//units
			GeneralObject* GO=NT->Mon[sad->ObjID];
			GO->Branch=sad->Source;
			if(sad->Amount>UnitAmount[sad->ObjID]){
				word prid=PRPIndex[sad->ObjID];
				if(prid!=0xFFFF){
					//random checking,money checking 
					ProposedProject* PRP=&Prop[prid];
					if(rando()<sad->AtnPercent&&sad->GoldPercent>=PRP->Percent){
						if(PresentProject){
							if(!GO->newMons->Building){
								BestProj[npro]=prid;
								npro++;
							};
						}else{
							BestProj[npro]=prid;
							npro++;
						};
						if(npro>=127)return npro;
					};
				};
			};
		}else{
			
			//upgrades
			word UpgID=sad->ObjID;
			NewUpgrade* NUP=Nat->UPGRADE[UpgID];
			NUP->Branch=sad->Source;
			//random condition checking
			word PropInd=UPGIndex[UpgID];
			ProposedProject* PRP=&Prop[PropInd];
			if(sad->AtnPercent>rando()&&sad->GoldPercent>=PRP->Percent){
				//checking the need conditions
				if(PropInd!=0xFFFF){
					//project present
					//ProposedProject* PRP=&Prop[PropInd];
					BestProj[npro]=PropInd;
					npro++;
					if(npro>=127)return npro;
				};
			};

		};
	};
	return npro;
};
int GetMonsterAccessControl(byte NI, word NIndex, char** acMessage){ //word* acNIndex
	int acN=0;
	City* CT=CITY+NI;
	Nation* Nat=CT->Nat;
	for(int i=0;i<Nat->NCOND;i++){
		word MID=Nat->CLRef[i][0];
		if(MID==NIndex){
			word NC=Nat->CLSize[i];
			//bool enab=true;
			for(int j=0;j<NC;j++){
				word uc=Nat->CLRef[i][j+1];
				if(uc<8192){
					GeneralObject* GO=Nat->Mon[uc];
					if(!CT->ReadyAmount[uc]){
						acMessage[acN]=GO->Message;
						acN++;						
					}
				}else{
					NewUpgrade* NU=Nat->UPGRADE[uc-8192];
					if(!NU->Done){
						acMessage[acN]=NU->Message;
						acN++;
					}
				};
			};
			break;
		}
	}
	return acN;
}
void City::RefreshAbility(){
	//Let's disable all checked units/upgrades 
	for(int i=0;i<Nat->NCOND;i++){
		word MID=Nat->CLRef[i][0];
		if(MID<8192){
			//Monster
			GeneralObject* GO=Nat->Mon[MID];
			GO->Enabled=false;
		}else{
			Nat->UPGRADE[MID&8191]->Enabled=false;
		};
	};
	//Let us check it now and enable all allowed monsters/upgrades
	for(int i=0;i<Nat->NCOND;i++){
		word MID=Nat->CLRef[i][0];
		word NC=Nat->CLSize[i];
		bool enab=true;
		for(int j=0;j<NC;j++){
			word uc=Nat->CLRef[i][j+1];
			if(uc<8192){
				if(!ReadyAmount[uc])enab=false;
			}else{
				if(!Nat->UPGRADE[uc-8192]->Done)enab=false;
			};
		};
		if(enab){
			if(MID<8192){
				//Monster
				GeneralObject* GO=Nat->Mon[MID];
				if(GO->CondEnabled||GO->newMons->Building)GO->Enabled=true;
			}else{
				NewUpgrade* sutp=Nat->UPGRADE[MID&8191];
				if(sutp->PermanentEnabled&&!sutp->Done)sutp->Enabled=true;
			};
		};
	};
	for(int i=0;i<Nat->NMon;i++){
		GeneralObject* GO=Nat->Mon[i];
		NewMonster* NM=GO->newMons;
		if(GO->ManualDisable){
			GO->Enabled=false;
		}else{
			// vital: perenes v interface: UnitsInterface.cpp::SetProduce()
			/*
			if(GO->LockID!=0xFFFF){
				if(UnitAmount[GO->LockID]>=Nat->Mon[i]->NLockUnits)	GO->Enabled=false;
				//else GO->Enabled=GO->CondEnabled;
			};
			if(NM->ArtSet){
				if(Nat->NArtUnits[NM->ArtSet-1]>=Nat->NArtdep*NM->NInArtDepot)	GO->Enabled=false;
				//else GO->Enabled=GO->CondEnabled;
			}
			*/
		};
		if(GO->ManualEnable){
			GO->Enabled=1;
		};
	};
};

extern byte NPORTS;
extern short PORTSX[32];
extern short PORTSY[32];

extern byte NSKLAD;
extern short SKLADSX[32];
extern short SKLADSY[32];

bool SearchTowerPlace(int* xx1,int* yy1,SearchFunction* SFN,int r,int xc,int yc,int xe,int ye);
bool CheckTower(int x,int y);
OneObject* DetermineMineBySprite(int Spr);
int GetTopDistance(int xa,int ya,int xb,int yb);
bool SearchMinePlace(int* x,int* y,GeneralObject* GO){
	NewMonster* NM=GO->newMons;
	byte RT=NM->ProdType;
	int RMIN=10000/64;
	int MX=-1;
	int MY=-1;
	int tx=(*x)<<1;
	int ty=(*y)<<1;
	for(int i=0;i<NMines;i++){
		int IDS=MineList[i];
		if(IDS>=0&&IDS<MaxSprt){
			OneSprite* OSP=Sprites+IDS;
			if(RT&(1<<OSP->OC->IntResType)){
				int R=GetTopDistance(tx,ty,OSP->x>>6,OSP->y>>6);
				int R0=Norma(tx-(OSP->x>>6),ty-(OSP->y>>6));
				if(R0<45)R=R0;
				if(R<RMIN){
					OneObject* OB=DetermineMineBySprite(IDS);
					if(!OB){
						RMIN=R;
						MX=OSP->x>>7;
						MY=OSP->y>>7;
					};
				};
			};
		};
	};
	if(MX!=-1){
		*x=MX;
		*y=MY;
		return true;
	}else return false;
};
bool City::FindApproximateBuildingPlace(GeneralObject* GO){
	NewMonster* NM=GO->newMons;
	bool Res;
	int xx=CenterX;
	int yy=CenterY;
	BPR.Options=0;
	switch(NM->Usage){
	case MineID:
		Res=SearchMinePlace(&xx,&yy,GO);
		break;
	case MelnicaID:
		Res=SearchPlace(&xx,&yy,&CheckMelnica,25);
		break;
	case CenterID:
		NSKLAD=0;
		Res=SearchPlace(&xx,&yy,&CheckStoneWoodSklad,20);
		if(!Res){
			if(NWoodSklads<NStoneSklads){
				Res=SearchPlace(&xx,&yy,&CheckWoodSklad,30);
				if(Res)BPR.Options=121+(2<<8);
			}else{
				Res=SearchPlace(&xx,&yy,&CheckStoneSklad,30);
				if(Res)BPR.Options=121+(4<<8);
			};
		}else{
			BPR.Options=121+((2+4)<<8);
		};
		if(!Res){
			Res=SearchPlace(&xx,&yy,&CheckBuilding,30);
		};
		break;
	case SkladID:
		NSKLAD=0;
		{
			for(int k=0;k<MAXOBJECT&&NSKLAD<32;k++){
				OneObject* OB=Group[k];
				if(OB&&(!OB->Sdoxlo)&&(OB->Ref.General==GO)){
					SKLADSX[NSKLAD]=OB->RealX>>(4+7);
					SKLADSY[NSKLAD]=OB->RealY>>(4+7);
					NSKLAD++;
				};
			};
		}

		Res=SearchPlace(&xx,&yy,&CheckWoodSklad,10);
		if(!Res){
			if(NWoodSklads<=NStoneSklads){
				Res=SearchPlace(&xx,&yy,&CheckWoodSklad,60);
				if(Res)BPR.Options=121+(2<<8);
			}else{
				//Res=SearchPlace(&xx,&yy,&CheckStoneSklad,30);
				Res=SearchPlace(&xx,&yy,&CheckWoodSklad,60);
				if(Res)BPR.Options=121+(4<<8);
			};
		}else{
			BPR.Options=121+((2+4)<<8);
		};
		break;
	case PortID:
		NPORTS=0;
		{
			for(int k=0;k<MAXOBJECT&&NPORTS<32;k++){
				OneObject* OB=Group[k];
				if(OB&&(!OB->Sdoxlo)&&(OB->Ref.General==GO)){
					PORTSX[NPORTS]=OB->RealX>>(4+7);
					PORTSY[NPORTS]=OB->RealY>>(4+7);
					NPORTS++;
				};
			};
		};
		Res=SearchPlace(&xx,&yy,&CheckPort,60);
		break;
	case TowerID:
		{
			int exc=0;
			int eyc=0;
			int ne=0;
			NPORTS=0;
			for(int k=0;k<MAXOBJECT&&NPORTS<32;k++){
				OneObject* OB=Group[k];
				if(OB&&(!OB->Sdoxlo)){
					if(OB->Ref.General==GO){
						PORTSX[NPORTS]=OB->RealX>>(4+7);
						PORTSY[NPORTS]=OB->RealY>>(4+7);
						NPORTS++;
					}else if(OB->NNUM==0){
						ne++;
						exc+=OB->RealX>>11;
						eyc+=OB->RealY>>11;
					};
				};
			};
			if(ne){
				exc/=ne;
				eyc/=ne;
			};
			Res=SearchTowerPlace(&xx,&yy,&CheckTower,40,CenterX,CenterY,exc,eyc);
		};
		break;
	case FarmID:
	default:
		Res=SearchPlace(&xx,&yy,&CheckBuilding,40);
		break;
	};
	BPR.AttemptsToFindApprPlace++;
	BPR.AttemptsToStand=0;
	if(Res){
		BPR.NearX=xx;
		BPR.NearY=yy;
		//assert(BPR.NearY!=17648);
		return true;
	}else return false;
};

void City::MarkUnusablePlace(){
	switch(BPR.Usage){
	case MelnicaID:
		SetUnusable(BPR.NearX,BPR.NearY,CB_Melnica);
		break;
	case CenterID:
		SetUnusable(BPR.NearX,BPR.NearY,CB_Sklad);
		SetUnusable(BPR.NearX,BPR.NearY,CB_Building);
		break;
	case SkladID:
		SetUnusable(BPR.NearX,BPR.NearY,CB_Sklad);
		break;
	case PortID:
		SetUnusable(BPR.NearX,BPR.NearY,CB_Port);
		break;
	default:
		SetUnusable(BPR.NearX,BPR.NearY,CB_Building);
	};
};
int CheckCreationAbility(byte NI,int Type,int* x2i,int* y2i,word* BLD,int NBLD,bool Manual);
bool City::FindPreciseBuildingPlace(int Type){
	BPR.AttemptsToStand++;
	int xx=(int(BPR.NearX)<<11)+(rando()&4096)-2048;
	int yy=(int(BPR.NearY)<<11)+(rando()&4096)-2048;
	if(CheckCreationAbility(NI,Type,&xx,&yy,NULL,0,0)!=-1){
		BPR.x=xx;
		BPR.y=yy;
		return true;
	};
	return false;
};

void GoToMineLink(OneObject* OB);
int SearchPeasants(int x,int y,bool Workers,byte NI,word* AddTo,int maxpos){
	if(x<0||y<0||x>=VAL_MAXCX-1||y>=VAL_MAXCX-1)return 0;
	int cell=(x+1)+((y+1)<<VAL_SHFCX);
	int pos=0;
    int NMon=MCount[cell];
	if(!NMon)return NULL;
	int ofs1=cell<<SHFCELL;
	word MID;
	int BRID=0x5432;
	if(!Workers)BRID=NATIONS[NI].CITY->Builders.ID;
	for(int i=0;i<NMon;i++){
		MID=GetNMSL(ofs1+i);
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->NNUM==NI&&OB->BrigadeID!=BRID&&(!OB->Sdoxlo)&&OB->newMons->Peasant&&!OB->DoNotCall){
				bool BLD=(OB->LocalOrder&&OB->LocalOrder->DoLink==&BuildObjLink);
				if(BLD){
					word ID=OB->LocalOrder->info.BuildObj.ObjIndex;
					OneObject* BOB=Group[ID];
					if(BOB&&BOB->Ready)BLD=0;
				}
				if(!BLD){
					if(maxpos){
						AddTo[pos]=OB->Index;
						maxpos--;
						pos++;
						if(!maxpos)return pos;
					};
				};
			};
		};
	};
    return pos;
};
int SearchPeasantsInSquare(int x,int y,int r,byte NI,bool Workers,word* AddOn,int max){
	int pos=0;
	if(r>90)r=0;
	for(int i=0;i<r;i++){
		int N=Rarr[i].N;
        char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		for(int j=0;j<N;j++){
			int dd=SearchPeasants(x+xi[j],y+yi[j],Workers,NI,AddOn+pos,max);
			if(dd){
				max-=dd;
				pos+=dd;
				if(!max)return pos;
			};
		}
	}
	return pos;
};
int CreateTeamOfPeasants(byte NNUM,int x,int y,int MinP,int MaxP){
	word pids[128];
	int npids=0;
	if(MaxP>120)MaxP=120;
	int xc=x>>11;
	int yc=y>>11;
	int pos=SearchPeasantsInSquare(xc,yc,45,NNUM,false,pids,MaxP);
	if(pos<MinP)pos+=SearchPeasantsInSquare(xc,yc,55,NNUM,true,pids+pos,MinP-pos);
	if(!pos){
		pos=SearchPeasantsInSquare(xc,yc,200,NNUM,false,pids,MaxP);
		if(pos<MinP)pos+=SearchPeasantsInSquare(xc,yc,200,NNUM,true,pids+pos,MinP-pos);
	};
	if(pos){
		if(Selm[NNUM]){
			int ns=NSL[NNUM];
			word* smn=Selm[NNUM];
			word* ssn=SerN[NNUM];
			for(int j=0;j<ns;j++){
				word MID=smn[j];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB->Serial==ssn[j])OB->Selected&=~GM(OB->NNUM);
				};
			};
			free(Selm[NNUM]);
		};
		if(SerN[NNUM])free(SerN[NNUM]);
		Selm[NNUM]=znew(word,pos);
		SerN[NNUM]=znew(word,pos);
		int ps1=0;
		for(int i=0;i<pos;i++){
			OneObject* OB=Group[pids[i]];
			if(OB){
				Selm[NNUM][ps1]=OB->Index;
				SerN[NNUM][ps1]=OB->Serial;
				OB->Selected|=GM(NNUM);
				ps1++;
			};
		};
		NSL[NNUM]=ps1;
		addrand(NSL[NNUM]+1);
	};
	return pos;
};
void City::AddConstruction(OneObject* OB){
	if(NConstructions<64){
		Construction[NConstructions]=OB->Index;
		ConstructionSN[NConstructions]=OB->Serial;
		ConstructionTime[NConstructions]=95;
		NConstructions++;
	};
};
void City::HandleConstructions(){	
	if(DestBldGroup!=0xFFFF)return;
	for(int i=0;i<NConstructions;i++){
		word Idx=Construction[i];
		word tim=ConstructionTime[i];
		OneObject* OB=Group[Idx];
		bool DeleteThis=false;
		if(OB){
			if(OB->Serial==ConstructionSN[i]){
				if(OB->Stage<OB->MoreCharacter->ProduceStages){
					NewMonster* NM=OB->newMons;
					if(NM->Farm&&NM->NInFarm)FarmsUnderConstruction+=NM->NInFarm;
				}else DeleteThis=true;
			}else DeleteThis=true;
		}else DeleteThis=true;
		if(DeleteThis){
			if(i<NConstructions-1){
				memcpy(Construction+i,Construction+i+1,(NConstructions-i-1)<<1);
				memcpy(ConstructionSN+i,ConstructionSN+i+1,(NConstructions-i-1)<<1);
				memcpy(ConstructionTime+i,ConstructionTime+i+1,(NConstructions-i-1)<<1);
			}
			NConstructions--;
			i--;
		}else{
			/*
			if(ConstructionTime[i]++>100){
				word MID=Construction[i];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&OB->Serial==ConstructionSN[i]){
						int NP=0;
						if(OB->Life<10){
							//checking if someone wants to build this building
							int nw=0;
							for(int i=0;i<MAXOBJECT;i++){
								OneObject* POB=Group[i];
								if(POB&&POB->NNUM==NI&&POB->newMons->Peasant){
									Order1* OR1=POB->LocalOrder;
									while(OR1){
										if(OR1->DoLink==&BuildObjLink){
											if(OR1->info.BuildObj.ObjIndex==MID)nw++;
											OR1=NULL;
										}else OR1=OR1->NextOrder;
									};
								};
							};
							if(nw<4){
								NP=CreateTeamOfPeasants(Nat->NNUM,OB->RealX,OB->RealY,10,10);
							}else{
								NP=0;
							};
						}else NP=CreateTeamOfPeasants(Nat->NNUM,OB->RealX,OB->RealY,3,3);
						if(NP){
							BuildWithSelected(Nat->NNUM,OB->Index,1);
							ConstructionTime[i]=0;
						}else ConstructionTime[i]=30;
					};
				};
			};
			*/
		};
	};
};
int Norma(int,int);
extern bool SpriteSuccess;
extern int LastSpriteIndex;
#define FieldR1 3
void CHKS();
void CreateFields(byte NI,int x,int y,int n);
void City::MakeZasev(){
	int FieldR=FieldR1;
	if(Nat->NGidot>200){
		//FieldR+=2;
		BestOnField=120;
	};
	//search for the field GO
	int N=Nat->NMon;
	int i;
	for(i=0;i<N&&Nat->Mon[i]->newMons->Usage!=FieldID;i++);
	if(i<N){
		word FGID=i;
		bool FDone=false;
		for(i=0;i<NMeln&&!FDone;i++){
			OneObject* OB=Group[MelnID[i]];
			if(OB&&OB->Ready&&OB->Serial==MelnSN[i]){
#ifdef CONQUEST
				if(OB->Nat->PACount[OB->NIndex])CreateFields(OB->NNUM,OB->RealX,OB->RealY,OB->Nat->PAble[OB->NIndex][0]);
#else
				int fcx=OB->RealX+40*16;
				int fcy=OB->RealY-80*16;
				for(int dx=-FieldR;dx<=FieldR;dx++){
					for(int dy=-FieldR;dy<=FieldR;dy++){
						int dxx=((dx+dy)<<10)+32*16;
						int dyy=((dx-dy)<<10)-32*16;
						SpriteSuccess=false;
						Nat->CreateNewMonsterAt(fcx+dxx,fcy+dyy,FGID,false);
						if(SpriteSuccess){
							if(NFields<512){
								FieldsID[NFields]=LastSpriteIndex;
								OneSprite* OS=&Sprites[LastSpriteIndex];
								if(OS->Enabled){
									FieldsSN[NFields]=(OS->x>>5)+((OS->y>>5)<<8);
									NFields++;
								};
							};
						};
					};
				};
#endif
			};
		};
	};
};
word FIELDID=0xFFFF;
void SlowCreateFields(byte NI,int x,int y,int n);
void City::MakeSlowZasev(){
	int FieldR=FieldR1;
	//search for the field GO
	int N=Nat->NMon;
	if(FIELDID==0xFFFF){
		int i;
		for(i=0;i<N&&Nat->Mon[i]->newMons->Usage!=FieldID;i++);
		if(i>=N)return;
		FIELDID=i;
	};
	word FGID=FIELDID;
	bool FDone=false;
	for(int i=0;i<NMeln&&!FDone;i++){
		OneObject* OB=Group[MelnID[i]];
		if(OB&&OB->Ready&&OB->Serial==MelnSN[i]){
#ifdef CONQUEST
			SlowCreateFields(OB->NNUM,OB->RealX,OB->RealY,OB->Nat->PAble[OB->NIndex][0]);
#else
			int fcx=OB->RealX+40*16;
			int fcy=OB->RealY-80*16;
			for(int dx=-FieldR;dx<=FieldR;dx++){
			for(int dy=-FieldR;dy<=FieldR;dy++){
					if(rando()<1024){
						int dxx=((dx+dy)<<10)+32*16;
						int dyy=((dx-dy)<<10)-32*16;
						SpriteSuccess=false;
						Nat->CreateNewMonsterAt(fcx+dxx,fcy+dyy,FGID,false);
						if(SpriteSuccess){
							if(NFields<512){
								FieldsID[NFields]=LastSpriteIndex;
								OneSprite* OS=&Sprites[LastSpriteIndex];
								if(OS->Enabled){
									FieldsSN[NFields]=(OS->x>>5)+((OS->y>>5)<<8);
									NFields++;
								};
							};
						};
					};
				};
			};
#endif
		};
	};
};
void City::HandleFields(){
	if(NMeln){
#ifdef CONQUEST
		if(rando()<4096)MakeSlowZasev();
#else
		if((!NFields)||(NFields<MaxFields-5)){
		//need zasev
			MakeZasev();
			MaxFields=NFields;
		}else{
			if(rando()<4096)MakeSlowZasev();
		};
#endif
	};
	FieldReady=false;
	if(NFields){
		for(int i=0;i<NFields;i++){
			bool RemoveField=false;
			OneSprite* OS=&Sprites[FieldsID[i]];
			if(OS->Enabled&&FieldsID[i]>=0){
				//int SN=(OS->x>>5)+((OS->y>>5)<<8);
				//if(SN==FieldsSN[i]){
					SprGroup* SG=OS->SG;
					ObjCharacter* OC=OS->OC;
					if(OC->ResType==FoodID)FieldReady=true;
				//}else RemoveField=true;
			}else RemoveField=true;
			if(RemoveField){
				if(i<NFields-1){
					memcpy(FieldsID+i,FieldsID+i+1,(NFields-i-1)<<2);
					memcpy(FieldsSN+i,FieldsSN+i+1,(NFields-i-1)<<1);
				};
				NFields--;
				i--;
			};
		};
	};
};
//
DLLEXPORT bool RemoveNearestUnit(GAMEOBJ* G_Src,GAMEOBJ* G_Dst,int x,int y,int MaxR);
DLLEXPORT void RemoveGroup(GAMEOBJ* Source,GAMEOBJ* Dest);
//
void City::HandleBrigades(int NP){
	//UnitsGroup* UG=&SCENINF.UGRP[AllPeasantsGroup];
	UnitsGroup* LG=&SCENINF.UGRP[LasyPeasantsGroup];
	UnitsGroup* WG=&SCENINF.UGRP[OnWoodGroup];
	UnitsGroup* SG=&SCENINF.UGRP[OnStoneGroup];
	UnitsGroup* FG=&SCENINF.UGRP[OnFoodGroup];
	//UG->RefreshUnits();
	LG->RefreshUnits();
	WG->RefreshUnits();
	SG->RefreshUnits();
	FG->RefreshUnits();

	// ���
	//if(!FieldReady) NeedOnField=0;
	//
	
	NP=LG->N+WG->N+SG->N+FG->N;
    int NT=NeedOnWood+NeedOnStone+NeedOnField;
	if(!NT)return;
	int NF=NP*NeedOnField/NT;
	int NS=NP*NeedOnStone/NT;
	int NW=NP-NF-NS;
	if(FG->N>NF)FG->CopyUnits(LG,0,1,1,1);
	if(WG->N>NW)WG->CopyUnits(LG,0,1,1,1);
	if(SG->N>NS)SG->CopyUnits(LG,0,1,1,1);
	if(LG->N){
		if(NF>FG->N)LG->CopyUnits(FG,0,1,1,1);
		if(LG->N){
			if(NS>SG->N)LG->CopyUnits(SG,0,1,1,1);
			if(LG->N){
				if(NW>WG->N)LG->CopyUnits(WG,0,1,1,1);
			}
		}
	}
	for(int i=0;i<FG->N;i++)CheckFieldWorker(Group[FG->IDS[i]],this);
	for(int i=0;i<WG->N;i++)CheckWoodWorker(Group[WG->IDS[i]],this);
	for(int i=0;i<SG->N;i++)CheckStoneWorker(Group[SG->IDS[i]],this);
	
	// bridge buildings
	UnitsGroup* Rep=SCENINF.UGRP+BridgeGroup;
	Rep->RefreshUnits();
	GAMEOBJ rep={BridgeGroup,0,'UNIT'};
	if(Rep->GOrder){
		int x=0;
		int y=0;
		int r=100000;
		int n=NBridgeBuilders;
		GAMEOBJ g={0,0,'UNIT'};
		g.Index=LasyPeasantsGroup;
		while(n>0&&RemoveNearestUnit(&g,&rep,x,y,r)){
			n--;
		}
		//UnitsGroup* ug;
		g.Index=OnStoneGroup;
		//ug=SCENINF.UGRP+CT->OnStoneGroup;
		while(n>0&&RemoveNearestUnit(&g,&rep,x,y,r)){
			n--;
		}
		g.Index=OnWoodGroup;
		//ug=SCENINF.UGRP+CT->OnWoodGroup;
		while(n>0&&RemoveNearestUnit(&g,&rep,x,y,r)){
			n--;
		}
		g.Index=OnFoodGroup;
		//ug=SCENINF.UGRP+CT->OnFoodGroup;
		while(n>0&&RemoveNearestUnit(&g,&rep,x,y,r)){
			n--;
		}
	}else{
		GAMEOBJ g={LasyPeasantsGroup,0,'UNIT'};
		RemoveGroup(&rep,&g);
	}

	//CalculateBalance();
	//int NN=NeedPF+NeedPW+NeedPS;
	//if(!NN)return;
	////if(!FieldReady){
	//	//NeedOnWood=0;
	//	//NeedOnStone=1;
	//	//NeedOnField=0;
	//	//FreePS=NP-1;
	////}else{
	//	NeedOnWood=(NeedPW*NP)/NN;
	//	NeedOnStone=(NeedPS*NP)/NN;
	//	NeedOnField=NP-NeedOnWood-NeedOnStone;
	//	FreePS=NP-NN;
	//	if(XRESRC(Nat->NNUM,FoodID)>50000){
	//		NeedOnWood+=NeedOnField>>2;
	//		NeedOnStone+=NeedOnField>>2;
	//		NeedOnField-=(NeedOnField>>2)<<1;
	//	}else
	//	if(XRESRC(Nat->NNUM,FoodID)>75000){
	//		NeedOnWood+=NeedOnField>>1;
	//		NeedOnStone+=NeedOnField>>1;
	//		NeedOnField-=(NeedOnField>>1)<<1;
	//	};
	//	if(FreePS<0)FreePS=0;
	////};
	//if(OnField.NMemb>NeedOnField){
	//	OnField.RemoveObjects(OnField.NMemb-NeedOnField,&Free);
	//};
	//if(OnStone.NMemb>NeedOnStone){
	//	OnStone.RemoveObjects(OnStone.NMemb-NeedOnStone,&Free);
	//};
	//if(OnWood.NMemb>NeedOnWood){
	//	OnWood.RemoveObjects(OnWood.NMemb-NeedOnWood,&Free);
	//};
	//if(Free.NMemb){
	//	if(NeedOnField>OnField.NMemb){
	//		int NO=NeedOnField-OnField.NMemb;
	//		if(NO>Free.NMemb)NO=Free.NMemb;
	//		Free.RemoveObjects(NO,&OnField);
	//	};
	//	if(NeedOnWood>OnWood.NMemb){
	//		//int NO=NeedOnWood-OnWood.NMemb;
	//		if(NeedOnWood>Free.NMemb)NeedOnWood=Free.NMemb;
	//		Free.RemoveObjects(NeedOnWood,&OnWood);
	//	};
	//	if(NeedOnStone>OnStone.NMemb){
	//		//int NO=NeedOnWood-OnStone.NMemb;
	//		if(NeedOnStone>Free.NMemb)NeedOnStone=Free.NMemb;
	//		Free.RemoveObjects(NeedOnStone,&OnStone);
	//	};
	//};
	//OnField.CFN=&CheckFieldWorker;
	//OnField.CheckMembers(this);

	//OnWood.CFN=&CheckWoodWorker;
	//OnWood.CheckMembers(this);

	//OnStone.CFN=&CheckStoneWorker;
	//OnStone.CheckMembers(this);

	//FreeArmy.CFN=&CheckFreeArmy;
	//FreeArmy.CheckMembers(this);

	//Free.CFN=&CheckFree;
	//Free.CheckMembers(this);

	//Builders.CFN=&CheckBuilders;
	//Builders.CheckMembers(this);
};
void ProduceByAI(byte nat,word Producer,word ID);
void CmdAddBuildObj(byte NI,word ObjID);
int SmartCreationUnit(byte NI,int NIndex,int x,int y);
extern int tmtmt;
int CURRENTAINATION;
byte DefPreBDef[6]={TowerID,5,6};
                    
byte* PreBDef=DefPreBDef;
int NPreBDef=1;
void AddOneUnitToGroup(GAMEOBJ* Units,OneObject* OB);
CIMPORT int vdf_GetAmountOfPeasant(byte Owner, int ResType);
int ConvertNationToSkirmish(int NatID);
int NationID;

void City::ProcessCreation(){
	Nat=NATIONS+NI;
	CURRENTAINATION=NI;
	RefreshAbility();
	addrand(NI+1);
	ExecuteBrigades();
	addrand(NI+1);
	if(REALTIME-LastTime>50){ // 1000
		/*
		int Dt=REALTIME-LastTime;
		WoodSpeed=WoodAmount*25/Dt;
		FoodSpeed=FoodAmount*25/Dt;
		StoneSpeed=StoneAmount*25/Dt;		
		WoodAmount=0;
		FoodAmount=0;
		StoneAmount=0;
		*/
		LastTime=REALTIME;
		// academy 
		memset(ResourcePeasants,0,sizeof ResourcePeasants);
		for(int i=0;i<6;i++){
			ResourcePeasants[i]+=vdf_GetAmountOfPeasant(NI,i);
		}
		//int& NID=GSets.CGame.InterfaceNatID=-1;
		//OneObject* NationalOB=NULL;
		byte MyNI=GSets.CGame.cgi_NatRefTBL[MyNation];
		NPeas=0;

		word* id=NatList[NI];
		int   n=NtNUnits[NI];
		for(int i=0;i<n;i++,id++){
			OneObject* OB=Group[*id];
			if( OB && OB->GetLiveState()==ls_LiveBody ){
				if( GSets.CGame.InterfaceNatID==-1 && MyNI==NI /*&& !NationalOB*/ ){
					GSets.CGame.InterfaceNatID=ConvertNationToSkirmish(OB->Ref.General->NatID);
					//if(GSets.CGame.InterfaceNatID!=-1)
					//	NationalOB=OB;
					NationID = MyNI;
				}
				NewMonster* NM=OB->newMons;
				if(NM->Producer/*&&NM->ResSubst*/){
					for(int i=0;i<6;i++){
						if(NM->ProdType&(1<<i)){
							ResourcePeasants[i]+=OB->NInside;
						}
					}
				}else{
					if( NM->Peasant && !OB->Hidden ){
						NPeas++;
						Order1* Ord=OB->LocalOrder;
						while(Ord){
							if(Ord->DoLink==&TakeResLink){
								byte ResType=Ord->info.TakeRes.ResID;
								if(ResType<6){
									ResourcePeasants[ResType]++;
								}else{
									if(ResType==8) // meat
										ResourcePeasants[FoodID]++;
								}
							}
							Ord=Ord->NextOrder;
						}
					}
				}
			}
		}

	};
	addrand(NI+1);
	rando();
	if(!Nat->AI_Enabled)return;
	addrand(NI+1);
	//if(Nat->NNUM==1){
	{
		if(CenterFound){
			//assert(FreeArmy.CT);
			//assert(NI==1);//????
			SmartGamer(this);
			//assert(NI==1);//????
			FarmsUnderConstruction=0; 

			//assert(NI==1);//????
			HandleConstructions();
			HandleFields();
			HandleBrigades(Free.NMemb+OnField.NMemb+OnWood.NMemb+OnStone.NMemb);
			if(AI_MINES_CPTURE)TakeNewMine();
			///HandleIdeas();
			//if(AI_MINES_UPGRADE)UpgradeMines();
			SendAgressors();
			HandleDefending();
			EnumProp();
			//Search for the project
			//check farms
			word proj=0xFFFF;
			int NGID=Nat->NGidot;
			int NF=Nat->NFarms+FarmsUnderConstruction;
			bool NeedFarm=false;
			/*
			if(DestBldGroup==0xFFFF){
				if(!(NF==0&&NGID<10)){
					if(NGID+1>=NF){
						NeedFarm=true;
					}else{
						if(NGID>10&&NF-NGID<7)NeedFarm=true;
						if(NGID>20&&NF-NGID<10)NeedFarm=true;
						if(NGID>30&&NF-NGID<20)NeedFarm=true;
						if(NGID>50&&NF-NGID<30)NeedFarm=true;
						if(NGID>60&&NF-NGID<40)NeedFarm=true;
						if(NGID>80&&NF-NGID<60)NeedFarm=true;
					};
					if(NeedFarm&&!PresentProject){
						for(int i=0;i<NProp;i++)if(Prop[i].PKind==0&&Nat->Mon[Prop[i].NIndex]->newMons->Usage==FarmID){
							proj=i;
							i=NProp;
						};
					};
				};
			};
			*/
			if(!AI_DEVELOPMENT)proj=0xFFFF;
			int NPR=0;
			if(proj==0xFFFF){
				/*if(AI_DEVELOPMENT)*/NPR=FindNeedProject();
			}else{
				NPR=1;
				BestProj[0]=proj;
			};
			for(int k=0;k<NPR;k++){
				proj=BestProj[k];
				ProposedProject* PRP=&Prop[proj];
				if(PRP->PKind==1){
					//upgrade
					OneObject* POB=Group[PRP->ProducerIndex];
					if(POB){
						POB->PerformUpgrade(PRP->NIndex,PRP->ProducerIndex);
					};
				}else{
					GeneralObject* IGO=Nat->Mon[PRP->NIndex];
					NewMonster* INM=IGO->newMons;
					if(!INM->Building){
						OneObject* POB=Group[PRP->ProducerIndex];
						if(POB){
							ProduceByAI(NI,POB->Index,PRP->NIndex);
							//POB->Produce(PRP->NIndex);
						};
					}else{
						if(!PresentProject){
							if(PRP->NIndex==Nat->UID_HOUSE){
								BPR.Used=true;
								PresentProject=true;
								BPR.NIndex=PRP->NIndex;
								BPR.PlaceFound=false;
								BPR.PeasantsCalled=false;
								BPR.AttemptsToFindApprPlace=0;
								BPR.AttemptsToStand=0;
								BPR.Usage=IGO->newMons->Usage;
							}else
							if(FindApproximateBuildingPlace(IGO)){
								BPR.Used=true;
								PresentProject=true;
								BPR.NIndex=PRP->NIndex;
								BPR.PlaceFound=false;
								BPR.PeasantsCalled=false;
								BPR.AttemptsToFindApprPlace=0;
								BPR.AttemptsToStand=0;
								BPR.Usage=IGO->newMons->Usage;
							};
						};
					};
				};
			};
			//if(PresentProject){
			//	GeneralObject* IGO=Nat->Mon[BPR.NIndex];
			//	if(BPR.NIndex==Nat->UID_HOUSE){
			//		int fx,fy;
			//		int IDX;
			//		int NN=0;
			//		do{
			//			GetNextPlaceForFarm(&fx,&fy);
			//			IDX=SmartCreationUnit(Nat->NNUM,BPR.NIndex,fx<<4,fy<<4);
			//			//IDX=Nat->CreateNewMonsterAt(fx<<4,fy<<4,BPR.NIndex,false);
			//			NN++;
			//		}while(NN<5&&IDX==-1);
			//		if(IDX!=-1){
			//			//need to send peasants
			//			if(DestBldGroup==0xFFFF){
			//				OneObject* OB=Group[IDX];
			//				NewMonster* INM=IGO->newMons;
			//				BPR.Index=IDX;
			//				AddConstruction(OB);
			//				int NP=CreateTeamOfPeasants(Nat->NNUM,OB->RealX,OB->RealY,2,3);
			//				if(NP)BuildWithSelected(Nat->NNUM,BPR.Index,1);
			//				BPR.Used=false;
			//				PresentProject=false;
			//			}else{
			//				//GAMEOBJ GO;
			//				//GO.Type='UNIT';
			//				//GO.Index=DestBldGroup;
			//				//AddOneUnitToGroup(&GO,Group[IDX]);
			//				BPR.Used=false;
			//				PresentProject=false;
			//			};
			//		};
			//	}else
			//	if(!BPR.PlaceFound){
			//		for(int F=0;F<1&&PresentProject&&!BPR.PlaceFound;F++){
			//			//if(FindPreciseBuildingPlace(IGO)){
			//			int xx=(int(BPR.NearX)<<11)+(rando()&4096)-2048;
			//			int yy=(int(BPR.NearY)<<11)+(rando()&4096)-2048;
			//			int IDX=SmartCreationUnit(Nat->NNUM,BPR.NIndex,xx,yy);
			//			if(IDX>=0){
			//				if(DestBldGroup!=0xFFFF){
			//					//GAMEOBJ GO;
			//					//GO.Type='UNIT';
			//					//GO.Index=DestBldGroup;
			//					//AddOneUnitToGroup(&GO,Group[IDX]);
			//					BPR.Used=false;
			//					PresentProject=false;
			//				};
			//				//int IDX=Nat->CreateNewMonsterAt(BPR.x,BPR.y,BPR.NIndex,false);
			//				//if(IDX>=0){
			//					/*
			//					if(BPR.Usage==PortID){
			//						OneObject* OB=Group[IDX];
			//						int x0=OB->WallX;
			//						int y0=OB->WallY;
			//						do{
			//							int xx=x0+(rando()&
			//						};
			//					};
			//					*/
			//					if(BPR.Usage==MelnicaID){
			//						if(MaxFields)MaxFields+=20;
			//						int xx=BPR.NearX;
			//						int yy=BPR.NearY;
			//						for(int dx=-6;dx<=6;dx++){
			//							for(int dy=-6;dy<=6;dy++){
			//								SetUnusable(xx+((dx+dy)>>1),yy+((dx-dy)>>1),CB_Melnica|CB_Sklad|CB_Building);
			//							};
			//						};
			//					};
			//					BPR.PlaceFound=true;
			//					OneObject* OB=Group[IDX];
			//					NewMonster* INM=IGO->newMons;
			//					BPR.Index=IDX;
			//					AddConstruction(OB);
			//					if(INM->Usage==SkladID||INM->Usage==CenterID){
			//						if((BPR.Options&255)==121){
			//							if(BPR.Options&512){
			//								//wood
			//								if(NWoodSklads<8){
			//									WoodSkladID[NWoodSklads]=IDX;
			//									WoodSkladSN[NWoodSklads]=OB->Serial;
			//									NWoodSklads++;
			//								};
			//							}else{
			//								bool UnFND=true;
			//								for(int i=0;i<8&&UnFND;i++){
			//									word ID=WoodSkladID[i];
			//									if(ID!=0xFFFF){
			//										OneObject* OBJ=Group[ID];
			//										if(!(OBJ&&OBJ->Serial==WoodSkladSN[i])){
			//											WoodSkladID[i]=IDX;
			//											WoodSkladSN[i]=OB->Serial;
			//											UnFND=false;
			//										};
			//									};
			//								};
			//							};
			//							if(BPR.Options&1024){
			//								//stone
			//								if(NStoneSklads<8){
			//									StoneSkladID[NStoneSklads]=IDX;
			//									StoneSkladSN[NStoneSklads]=OB->Serial;
			//									NStoneSklads++;
			//								}else{
			//									bool UnFND=true;
			//									for(int i=0;i<8&&UnFND;i++){
			//										word ID=StoneSkladID[i];
			//										if(ID!=0xFFFF){
			//											OneObject* OBJ=Group[ID];
			//											if(!(OBJ&&OBJ->Serial==StoneSkladSN[i])){
			//												StoneSkladID[i]=IDX;
			//												StoneSkladSN[i]=OB->Serial;
			//												UnFND=false;
			//											};
			//										};
			//									};
			//								};
			//							};
			//						};
			//					};
			//					if(INM->Usage==MelnicaID){
			//						if(NMeln<4){
			//							MelnID[NMeln]=IDX;
			//							MelnSN[NMeln]=OB->Serial;
			//							if(!NMeln){
			//								//CenterX=OB->RealX>>11;
			//								//CenterY=OB->RealY>>11;
			//							};
			//							NMeln++;
			//						};
			//					};
			//					//how mutch peasats do we need?
			//					AdvCharacter* IDC=IGO->MoreCharacter;
			//					if(IDC->ProduceStages>800||INM->Usage==MelnicaID||INM->Usage==CenterID){
			//						BPR.MaxPeasants=byte(INM->BuildPtX.GetAmount());
			//						if(INM->BuildPtX.GetAmount()>7)BPR.MinPeasants=byte(INM->BuildPtX.GetAmount()-5);
			//						else BPR.MinPeasants=BPR.MaxPeasants-1;
			//					}else{
			//						byte USAGE=IGO->newMons->Usage;
			//						bool used=1;
			//						for(int p=0;p<NPreBDef;p++)if(USAGE==PreBDef[p+p+p]){
			//							used=0;
			//							BPR.MinPeasants=PreBDef[p+p+p+1];
			//							BPR.MaxPeasants=PreBDef[p+p+p+2];
			//						};
			//						if(used){

			//							if(IDC->ProduceStages<100){
			//								BPR.MinPeasants=1;
			//								BPR.MaxPeasants=2;
			//							}else
			//							if(IDC->ProduceStages<150){
			//								BPR.MinPeasants=2;
			//								BPR.MaxPeasants=5;
			//							}else
			//							if(IDC->ProduceStages<200){
			//								BPR.MinPeasants=3;
			//								BPR.MaxPeasants=6;
			//							}else
			//							if(IDC->ProduceStages<250){
			//								BPR.MinPeasants=4;
			//								BPR.MaxPeasants=5;
			//							}else
			//							if(IDC->ProduceStages<300){
			//								BPR.MinPeasants=4;
			//								BPR.MaxPeasants=6;
			//							}else
			//							if(IDC->ProduceStages<500){
			//								BPR.MinPeasants=6;
			//								BPR.MaxPeasants=8;
			//							}else{
			//								BPR.MinPeasants=10;
			//								BPR.MaxPeasants=14;
			//							};
			//						};
			//					};
			//					//}else{

			//				//	BPR.Used=false;
			//				//	PresentProject=false;
			//				//};
			//			}else{
			//				if(BPR.AttemptsToFindApprPlace>4){
			//					PresentProject=false;
			//					BPR.Used=false;
			//				}else{
			//					//if(BPR.AttemptsToStand>5){
			//						MarkUnusablePlace();
			//						if(!FindApproximateBuildingPlace(IGO)){
			//							PresentProject=false;
			//							BPR.Used=false;
			//						};
			//					//};
			//				};
			//			};
			//		};
			//	}else{
			//		//place found,now we need to call peasants!
			//		OneObject* OB=Group[BPR.Index];
			//		if(OB){
			//			if(DestBldGroup==0xFFFF){
			//				int NP=CreateTeamOfPeasants(Nat->NNUM,OB->RealX,OB->RealY,BPR.MinPeasants,BPR.MaxPeasants);
			//				if(NP){
			//					BuildWithSelected(Nat->NNUM,BPR.Index,1);
			//					BPR.Used=false;
			//					PresentProject=false;
			//				};
			//			};
			//		}else{
			//			BPR.Used=false;
			//			PresentProject=false;
			//		};
			//	};
			//};
		}else{
			int xx,yy;
			CenterFound=FindCenter(&xx,&yy,Nat->NNUM);
			if(CenterFound){
				CenterX=xx;
				CenterY=yy;
			};
		};
	};
};

bool CINFMOD;
void OutCInf(byte x,byte y){
	return;
	if(!CINFMOD)return;
	Xbar(smapx+((x&252)-mapx)*32,smapy+((y&252)-mapy)*16,32*4,16*4,255);
};
void City::HelpMe(word ID){
	for(int i=0;i<32;i++){
		if(ID==EnemyList[i])return;
	};
	for(int i=0;i<32;i++){
		if(EnemyList[i]==0xFFFF){
			EnemyList[i]=ID;
			return;
		};
	};
	EnemyList[rando()&31]=ID;
};
word FindObjToAtt(){
	return 0xFFFF;
};
void City::HandleFly(){
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(rando()<4096&&OB&&OB->NNUM==NI&&OB->Kind==5&&!OB->LocalOrder){
			word ww=FindObjToAtt();
			if(ww!=0xFFFF)
			OB->AttackObj(ww,0);
		};
	};
};
void Branch::AddTo(byte ResID,int Amount){
	RESRM[ResID]+=Amount*RESP[ResID];
	div_t r=div(RESRM[ResID],100);
	RESRM[ResID]=r.rem;
	if((RESAM[ResID]+=r.quot)<0){
		RESAM[ResID]=0;
		RESRM[ResID]=0;
	};
};
void Branch::AddPerc(byte ResID,int Amount,int perc){
	RESRM[ResID]+=Amount*perc;
	div_t r=div(RESRM[ResID],100);
	RESRM[ResID]=r.rem;
	if((RESAM[ResID]+=r.quot)<0){
		RESAM[ResID]=0;
		RESRM[ResID]=0;
	};
};
void Branch::AddEntire(byte ResID,int Amount){
	if((RESAM[ResID]+=Amount)<0)RESAM[ResID]=0;

};
void Branch::Check(byte NI){
	//int* RS=&RESRC[NI][0];
	for(int i=0;i<8;i++)if(RESAM[i]>XRESRC(NI,i))RESAM[i]=XRESRC(NI,i);
};
void Branch::Init(){
	memset(this,0,sizeof Branch);
};
int Branch::GetMonsterCostPercent(byte NI,word NIndex){
	int COST[8];
	GetUnitCost(NI,NIndex,COST,-1,-1);
	int maxp=0;
	//int* RSS=RESRC[NI];
	for(int i=0;i<8;i++){
		int RP=XRESRC(NI,i);//RESAM[i];
		if(RP){
			int per=div(COST[i]*100,RP).quot;
			if(!per)per=1;
			if(per>maxp)maxp=per;
		};
	};
	if(!maxp)return 101;
	return maxp;
};
int Branch::GetUpgradeCostPercent(byte NI,word NIndex){
	byte* LSTATE=CITY[NI].LockUpgrade;
	word *COST=NATIONS[NI].UPGRADE[NIndex]->Cost;
	int maxp=0;
	//int* RSS=RESRC[NI];
	for(int i=0;i<8;i++){
		int RP=XRESRC(NI,i);//RESAM[i];
		if(RP){
			int per=div(int(COST[i])*100,RP).quot;
			if(LSTATE[i]&&COST[i]>600)per=1000;
			if(!per)per=1;
			if(per>maxp)maxp=per;
		};
	};
	if(!maxp)return 101;
	return maxp;
};
void Nation::AddResource(byte Rid,int Amount){
	SELO.AddTo(Rid,Amount);
	ARMY.AddTo(Rid,Amount);
	SCIENCE.AddTo(Rid,Amount);
	GENERAL.AddTo(Rid,Amount);
	
};
void Nation::ControlProduce(byte Branch,byte ResID,int Amount){
	switch(Branch){
	case 0://SELO
		SELO.AddEntire(ResID,Amount);
		GENERAL.AddEntire(ResID,Amount);
		break;
	case 1:
		ARMY.AddEntire(ResID,Amount);
		GENERAL.AddEntire(ResID,Amount);
		break;
	case 2:
		SCIENCE.AddEntire(ResID,Amount);
		GENERAL.AddEntire(ResID,Amount);
		break;
	case 255:
		if(GENERAL.RESAM[ResID]){
			int pp=GENERAL.RESAM[ResID];;
			SELO.AddPerc(ResID,Amount,div(SELO.RESAM[ResID]*100,pp).quot);
			ARMY.AddPerc(ResID,Amount,div(ARMY.RESAM[ResID]*100,pp).quot);
			SCIENCE.AddPerc(ResID,Amount,div(SCIENCE.RESAM[ResID]*100,pp).quot);
		};
		GENERAL.AddTo(ResID,Amount);
		break;
	};
	
};
void CheckIdeas(City* CT){
	return;
	Idea* ID=CT->IDEA;
	while(ID){
		assert(int(ID->NextIdea)!=0xdddddddd&&ID!=ID->NextIdea);
		if(ID&&int(ID->NextIdea)!=0xdddddddd&&ID->NextIdea){
			assert(ID==ID->NextIdea->PrevIdea);
		};
		ID=ID->NextIdea;
	};
};
Idea* City::AddIdea(IdeaBrain* IBR,bool Duplicate){
	CheckIdeas(this);
	Idea* ID;
	if(IDEA&&!Duplicate){
		ID=IDEA;
		while(ID){
			if(ID->Brain==IBR){
				CheckIdeas(this);
				return NULL;
			};
			ID=ID->NextIdea;
		};
	};
	if(!IDEA){
		ID=new Idea;
		IDEA=ID;
		ID->PrevIdea=NULL;
	}else{
		ID=IDEA;
		while(ID->NextIdea)ID=ID->NextIdea;
		ID->NextIdea=new Idea;
		ID->NextIdea->PrevIdea=ID;
		ID=ID->NextIdea;
	};
	ID->NextIdea=NULL;
	ID->Brain=IBR;
	ID->IdeaData=NULL;
	ID->DataSize=0;
	ID->CT=this;
	ID->FI=NULL;
	CheckIdeas(this);
	return ID;
};
void Idea::ClearIdea(){
	CheckIdeas(CT);
	if(PrevIdea){
		PrevIdea->NextIdea=NextIdea;
		if(NextIdea)NextIdea->PrevIdea=PrevIdea;
	}else{
		CT->IDEA=NextIdea;
		if(NextIdea)NextIdea->PrevIdea=NULL;
	};
	if(FI)FI(this);
	CheckIdeas(CT);
	delete(this);
};
void City::DelIdeas(){
	CheckIdeas(this);
	while(IDEA){
		Idea* NID=IDEA->NextIdea;
		IDEA->ClearIdea();
		IDEA=NID;
	};
	CheckIdeas(this);
};
void City::HandleIdeas(){
	CheckIdeas(this);
	if(IDEA){
		Idea* ID=IDEA;
		do{
			Idea* ID1=ID->NextIdea;
			if(ID->Brain)ID->Brain(ID);
			ID=ID1;
		}while(ID);
	};
	CheckIdeas(this);
};

const byte BMIND[40]={6,6,6,6,6,6,6,6,1,3,
                      4,5,6,6,6,6,6,6,6,6,
					  1,2,1,0,2,6,6,6,6,6,
					  6,6,5,6,6,6,6,6,6,6};
int GetBMIndex(OneObject* OB){
	byte Use=OB->newMons->Usage;
	if(Use<40)return BMIND[Use];
	else return 6;
};
void EraseBrigade(Brigade* BR);
void MakeStandGround(Brigade* BR);
void CalculateFreeUnits(AI_Army* AIR);
void AttackObjLink(OneObject* OBJ);
extern bool NOPAUSE;
void City::ExecuteBrigades(){
	// academi info
	NATIONS[NI].vsInfoInit=true;
	NATIONS[NI].NBrInfantry.Active=0;
	NATIONS[NI].NBrCavalery.Active=0;
	//assert(Nat);
	for(int i=0;i<NBrigs;i++){
		Brigade* BRIG=Brigs+i;
		if(BRIG->Enabled){
			//if(BRIG->BrigDelay&&!BRIG->BOrder)
			if(BRIG->BrigDelay&&BRIG->WarType&&!BRIG->NewBOrder)
			{
				int NAct=0;
				int NTot=0;
				int N=BRIG->NMemb;
				for(int j=0;j<N;j++){
					word MID=BRIG->Memb[j];
					if(MID!=0xFFFF){
						OneObject* OB=Group[MID];
						if(OB&&OB->Serial==BRIG->MembSN[j]){
							if((OB->LocalOrder&&(!OB->Attack))||Norma(OB->RealX/16-BRIG->posX[j],OB->RealY/16-BRIG->posY[j])>30)NAct++;
							NTot++;
						};
					};
				};
				if(NAct<3){
					int DB=(100*GameSpeed)>>8;
					if(BRIG->BrigDelay>=DB)BRIG->BrigDelay-=DB;
					else BRIG->BrigDelay=0;
				};
				if(!BRIG->BrigDelay)MakeStandGround(BRIG);
			}
			if(BRIG->WarType){
				// academi info
				if(NATIONS->Mon[BRIG->MembID]->newMons->PortBranch==0){
					NATIONS[NI].NBrInfantry.Active++;
				}else
				if(NATIONS->Mon[BRIG->MembID]->newMons->PortBranch==1){
					NATIONS[NI].NBrCavalery.Active++;
				}
				//
				bool CheckDupInBrigade(Brigade* BR);
				CheckDupInBrigade(BRIG);
				int MG=NATIONS[NI].Mon[BRIG->MembID]->newMons->MaxGrenadesInFormation;
				MG=(int(BRIG->NMemb-NBPERSONAL)*MG)/100;
				if(BRIG->NGrenades<MG && NOPAUSE){
					if(!BRIG->GrenDelay){
						BRIG->GrenDelay=int(NATIONS[NI].Mon[BRIG->MembID]->newMons->GrenadeRechargeTime)*256*8;
					}else{
						BRIG->GrenDelay-=GameSpeed;
						if(BRIG->GrenDelay<=0){
							BRIG->NGrenades++;
							BRIG->GrenDelay=int(NATIONS[NI].Mon[BRIG->MembID]->newMons->GrenadeRechargeTime)*256*8;
						};
					};
				};
				for(int j=0;j<BRIG->NABL;j++){
					if(BRIG->LeftTime[j]<0xFFFF){
						int T=int(BRIG->LeftTime[j])-(FrmDec<<3);
						if(T<=0){
							CEXPORT void DelAbility(byte NI,word BrIndex,int AblIndex);
							DelAbility(BRIG->CT->NI,BRIG->ID,BRIG->Abilki[j]);
							break;
						}else BRIG->LeftTime[j]=T;
					};
				};
				OrderDescription* ODS=ElementaryOrders+BRIG->WarType+1;
				int USAGE=ODS->OrdUsage;
				if(!BRIG->NewBOrder){
					int NU=BRIG->NMemb;
					word* IDS=BRIG->Memb;
					word* SNS=BRIG->MembSN;
					for(int p=0;p<NU;p++){
						word MID=IDS[p];
						if(MID!=0xFFFF){
							OneObject* OB=Group[MID];
							if(OB&&OB->Serial==SNS[p]&&!OB->LocalOrder){
								int R=Norma(BRIG->posX[p]-(OB->RealX>>4),BRIG->posY[p]-(OB->RealY>>4));
								if(R>32){
									if(!MFIELDS[OB->LockType].CheckBar(BRIG->posX[p]/16-8,BRIG->posY[p]/16-8,17,17)){
										OB->UnitSpeed=64;
										OB->NewMonsterPreciseSendTo(BRIG->posX[p]*16,BRIG->posY[p]*16,128+BRIG->AttEnm?0:16,0);
									}
								}else{
									char dd=OB->RealDir-BRIG->Direction;
									if(abs(int(dd))>16 && USAGE!=2){
										void RotUnit(OneObject* OB,char Dir,byte OrdType);
										RotUnit(OB,BRIG->Direction,0);
									}
								}
							}
						}
					}
				}
				//abilities implementation
				int VH=BRIG->GetAbilityValue(1);//healing during the time (+VH)
				int VHP=BRIG->GetAbilityValue(2);//healing during the time (+VH%)
				if(VH){
					GeneralObject* GO=NATIONS[BRIG->CT->NI].Mon[BRIG->MembID];
					int ML=GO->MoreCharacter->Life;
					int ADP=VH+((ML*VHP)/100);
					word* IDS=BRIG->Memb;
					word* SNS=BRIG->MembSN;
					int NU=BRIG->NMemb;
					int DL=0;
					int NDL=0;
					for(int p=0;p<NU;p++){
						word MID=IDS[p];
						if(MID!=0xFFFF){
							OneObject* OB=Group[MID];
							if(OB&&OB->Serial==SNS[p]){
								int dl=OB->MoreCharacter->Life-OB->Life;
								if(dl){
									DL+=dl;
									NDL++;
								}
							}
						}
					}
					if(NDL){
						int MXDL=(NDL*ADP*80);
						int CURDL=0;
						int ONEDL=MXDL/NDL;
						int v=0;
						for(int p=0;p<NU;p++){
							word MID=IDS[p];
							if(MID!=0xFFFF){
								OneObject* OB=Group[MID];
								if(OB&&OB->Serial==SNS[p]){
									int dl=OB->MoreCharacter->Life-OB->Life;
									if(dl){
										CURDL+=ONEDL;
										if(CURDL>10000){
											int dd=CURDL/10000;
											if(dd>dl)dd=dl;
											CURDL-=dd*10000;
											OB->Life+=dd;
										}
									}
								}
							}
						}
					}
				}
			}
			rando();
			//if(BRIG->BOrder&&(!BRIG->BOrder->Fast)&&BRIG->BOrder->BLink)BRIG->BOrder->BLink(BRIG);
			if(BRIG->NewBOrder&&(!BRIG->NewBOrder->IsFast()))BRIG->NewBOrder->Process(BRIG);
			if(BRIG->OnlyThisBrigadeToKill!=0xFFFF){
				bool erase=true;
                int NI=BRIG->NIOnlyThisBrigadeToKill;
				if(NI>=0&&NI<=8){
					Brigade* BR=CITY[NI].Brigs+BRIG->OnlyThisBrigadeToKill;
					if(BR->Enabled&&BR->WarType){
						if(BR->NMemb<=44){
							itr_GetNotCommandUnitsOfBrigade.Create(BR);
							OneObject* OB=itr_GetNotCommandUnitsOfBrigade.Next();
							if(OB){
								if(!(OB->RifleAttack&&OB->newMons->ArmAttack))erase=false;
							}
						}else erase=false;
					}
				}
				if(erase){
					BRIG->OnlyThisBrigadeToKill=0xFFFF;
					BRIG->NIOnlyThisBrigadeToKill=0;
					itr_GetUnitsOfBrigade.Create(BRIG);
					while(OneObject* OB=itr_GetUnitsOfBrigade.Next()){
						OB->SearchOnlyThisBrigadeToKill=0xFFFF;
					}

				}
			}
			rando();
			//if(i<MaxBrig-11&&BRIG->ArmyID==0xFFFF&&BRIG->NMemb==0&&!BRIG->BOrder)
			if(i<MaxBrig-11&&BRIG->ArmyID==0xFFFF&&BRIG->NMemb==0&&!BRIG->NewBOrder)
			{
				BRIG->DeleteAll();
				BRIG->Enabled=0;
			};
			//if(BRIG->WarType&&EngSettings.AutoChangeFormationType&&BRIG->GetActivity()==0){
			//	for(int i=0;i<BRIG->NMemb;i++){
			//		word MID=BRIG->Memb[i];
			//		if(MID!=0xFFFF){
			//			OneObject* OB=Group[MID];
			//			if(OB&&OB->Serial==BRIG->MembSN[i]){
			//				OB->ActivityState=1;//passive by default
			//			}
			//		}
			//	}
			//	OrderDescription* ODE=ElementaryOrders+BRIG->WarType-1;
			//	if(ODE->GroupID!=0xFF){
			//		SingleGroup* FGD=FormGrp.Grp+ODE->GroupID;
			//		if(FGD->NCommon){
			//			void MakeReformation(byte NI,word BrigadeID,byte FormType);
			//			if(FGD->IDX[0]<0xFF)MakeReformation(BRIG->CT->NI,BRIG->ID,FGD->IDX[0]);
			//		}			
			//	}
			//}
			if(BRIG->Ability)
				BRIG->Ability->Process(BRIG);
		};
	};
	//if(!Nat->AI_Enabled)
	{
		for(int i=0;i<MaxBrig;i++){
			Brigade* BRIG=Brigs+i;
			if(BRIG->Enabled){
				word* BMM=(word*)&BRIG->BM;
				if(BRIG->WarType){
					OrderDescription* ODE=ElementaryOrders+BRIG->WarType-1;
					int NN=0;
					for(int j=NBPERSONAL;j<BRIG->NMemb;j++){
						if(BRIG->Memb[j]!=0xFFFF)NN++;
					}
					if(NN<ODE->NUnits*EngSettings.DisbandFormLimit/100){
						if(EngSettings.MoraleType==0){//Cossacks2
							for(int i=0;i<BRIG->NMemb;i++){
								word ID=BRIG->Memb[i];
								if(ID!=0xFFFF){
									OneObject* OB=Group[ID];
									if(OB){
                                        void PanicUnit(OneObject* OBJ);
										PanicUnit(OB);
									}									
								}
							}
						}
						EraseBrigade(BRIG);
					}else{
						if(NN+NN<ODE->NUnits){
							//if(!BRIG->ErasureTime)BRIG->ErasureTime=300;
						}else BRIG->ErasureTime=0;
						if(BRIG->ErasureTime){
							if(BRIG->ErasureTime==1){
								EraseBrigade(BRIG);
							}else{
								BRIG->ErasureTime--;
							};
						};
					};
				};
			};
		};
	};
	rando();
	for(int i=0;i<MaxArm;i++){
		AI_Army* AR=ARMS+i;
		if(AR->Enabled){
			if(AR->AOrder&&AR->AOrder->ALink){
				rando();
				AR->AOrder->ALink(AR);
				rando();
			}else{
				rando();
				AR->MakeBattle();
				rando();
			};
			rando();
			CalculateFreeUnits(AR);
			rando();
		};
	};
	//assert(Nat);
	rando();
};
//-------------------------------IDEAS-----------------------------

void TakeResourceFromSpriteLink(OneObject* OBJ);

OneObject* DetermineMineBySprite(int Spr){
	OneSprite* OS=&Sprites[Spr];
	if(OS->Enabled){
		int ofs=(OS->x>>7)+((OS->y>>7)<<VAL_SHFCX)+1+VAL_MAXCX;
		for(int dx=-1;dx<=1;dx++){
			for(int dy=-1;dy<=1;dy++){
				int OFS1=ofs+dx+(dy<<VAL_SHFCX);
				if(OFS1>=0&&OFS1<VAL_MAXCIOFS){
					word MID;
					word GetOneBld(int cell,int pos);
					for(int pos=0;(MID=GetOneBld(OFS1,pos))!=0xFFFF;pos++){									
						OneObject* OB=Group[MID];
						if(OB&&OB->newMons->ProdType){
							Order1* OR1=OB->LocalOrder;
							if(OR1&&OR1->DoLink==&TakeResourceFromSpriteLink){
								if(OR1->info.BuildObj.ObjIndex==Spr)return OB;
							}else{
								if(OR1){
									OR1=OR1->NextOrder;
									if(OR1&&OR1->DoLink==&TakeResourceFromSpriteLink){
										if(OR1->info.BuildObj.ObjIndex==Spr)return OB;
									};
								};
							};
						};
					};
				};
			};
		};
		/*
		for(int i=0;i<MAXOBJECT;i++){
			OneObject* OB=Group[i];
			if(OB&&OB->newMons->ProdType){
				Order1* OR1=OB->LocalOrder;
				if(OR1&&OR1->DoLink==&TakeResourceFromSpriteLink){
					if(OR1->info.BuildObj.ObjIndex==Spr)return OB;
				};
			};
		};
		*/
		return NULL;
	}else return NULL;
};
struct TNM_Data{
	int NAttempts;
	int Mines[1];
	
};
int DefaultResTBL[12]={
	0       ,4,
	0x010201,4,//iron
	0x000100,1,
	0x010101,1,//gold
	0x000101,5,
	0x010202,5};//coal
int* ResTBL=DefaultResTBL;
int  NInResTBL=6;
extern int tmtmt;
bool CheckBuildPossibility(byte NI,int x,int y);
void TakeNewMineBrain(Idea* ID){
	City* CT=ID->CT;
	if(CT->DestBldGroup!=0xFFFF)return;
	bool GoodIdea=1;
	if(CT->FreePS<CT->Nat->MIN_PBRIG){
		ID->ClearIdea();
		GoodIdea=0;
		return;
	};
	Nation* NT=CT->Nat;
	TNM_Data* TD=(TNM_Data*)ID->IdeaData;
	int cx=CT->CenterX;
	int cy=CT->CenterY;
	int IDI=-1;
	int StartI=0;
	int TopC=GetTopology(CT->CenterX<<7,CT->CenterY<<7);
	bool Special=false;
	int ResID=IronID;
	int NMinFE=CT->UnitAmount[CT->Nat->UID_MINE+1];
	int NMinGL=CT->UnitAmount[CT->Nat->UID_MINE];
	int NMinCO=CT->UnitAmount[CT->Nat->UID_MINE+2];
	//if(!NMinFE)Special=true;
	int RSM=NMinGL+(NMinFE<<8)+(NMinCO<<16);
	for(int u=0;u<NInResTBL;u++)if(ResTBL[u+u]==RSM){
		Special=true;
		ResID=ResTBL[u+u+1];
	};
	
BeginMine:
	IDI=-1;
	int MinDist=1000000;
	int i;
	for(i=0;i<NMines;i++){
		int SID=TD->Mines[i];
		if(SID<MaxSprt){
			OneSprite* OS=&Sprites[SID];
			if((!Special)||(Special&&OS->OC->IntResType==ResID)){
				if(CheckBuildPossibility(NT->NNUM,OS->x,OS->y)&&!GNFO.EINF[CT->NI]->GetSafeVal(OS->x>>7,OS->y>>7)){
					int dst=Norma(cx-(OS->x>>7),cy-(OS->y>>7));
					if(dst<MinDist){
						MinDist=dst;
						IDI=i;
					};
				};
			};
		};
	};
	StartI=i;
	if(IDI!=-1){
		if(MinDist>(NT->MINE_CAPTURE_RADIUS>>2)){
			TD->Mines[IDI]=INITBEST;
			return;
		};
		int SID=TD->Mines[IDI];
		OneSprite* OS=&Sprites[SID];
		int Top1=GetTopology(OS->x,OS->y);
		if(TopC!=Top1&&(Top1>=0xFFFE||GetMotionLinks(Top1+TopC*GetNAreas())>=0xFFFE)){
			TD->Mines[IDI]=INITBEST;
			return;
		};
		OneObject* OB=DetermineMineBySprite(SID);
		/*
		if(!(OB&&OB->NNUM==ID->CT->NI)){
			if(!GoodIdea)ID->ClearIdea();
			return;
		};
		*/
		word Npeons=CT->Nat->MIN_PBRIG;
		word Ninf=2;
		word Nstrel=0;
		word MinWar=0;
		SendPInform* INF=(SendPInform*)CT->SearchInform(0x1256,SID,NULL);
		if(INF&&tmtmt-INF->time>SendPTime){
			CT->DelInform(INF);
			INF=NULL;
		};
		if(INF){
			//CT->DelInform(INF);
			TD->Mines[IDI]=INITBEST;
			return;
		};
		if(OB){
			if(OB->NNUM==NT->NNUM||OB->NNUM==7){
				int maxi=OB->MoreCharacter->MaxInside+OB->AddInside;
				if(OB->NInside<maxi){
					Npeons=maxi-OB->NInside;
					if(Npeons<3){
						TD->Mines[IDI]=INITBEST;
						goto BeginMine;
					};
				}else{
					TD->Mines[IDI]=INITBEST;
					goto BeginMine;
				};
			}else{
				//
				TD->Mines[IDI]=INITBEST;
				goto BeginMine;
				Ninf=25;
				Nstrel=15;
				Npeons=CT->Nat->MIN_PBRIG;
				MinWar=18;
			};
		};
		int N=CT->GetFreeBrigade();
		if(N!=-1){
			Brigade* BR=CT->Brigs+N;
			BR->Enabled=true;
			BrigMemb BMEM;
			memset(&BMEM,0,sizeof BrigMemb);
			BMEM.Peons=Npeons;
			BMEM.Infantry=Ninf;
			BMEM.Strelkov=Nstrel;
			BR->AddInRadius(OS->x,OS->y,20000,&BMEM);
			BrigMemb* BM1=&BR->BM;
			int N1=BM1->Grenaderov+BM1->Infantry+BM1->Mortir+BM1->Pushek+BM1->Strelkov;
			if(N1<MinWar||BM1->Peons!=Npeons){
				BR->Rospusk();
				BR->DeleteAll();
				BR->Enabled=false;
				TD->Mines[IDI]=INITBEST;
				return;
			};
			BR->CaptureMine(SID,128,0);
			//int NN=N-OB->NInside;
			//int N=CreateTeamOfPeasants(OB->NNUM,OB->RealX,OB->RealY,NN,NN);
			SendPInform* IN1=new SendPInform;
			IN1->ID=0x1256;
			IN1->time=tmtmt;
			IN1->Size=sizeof SendPInform;
			IN1->Essence=SID;
			CT->AddInform(IN1,NULL);
			return;
		};
	};
	ID->ClearIdea();
};
void City::TakeNewMine(){
	if(DestBldGroup!=0xFFFF)return;
	if(FreePS<Nat->MIN_PBRIG){//||FreeArmy.NMemb<1){
		return;
	};
	if(XRESRC(NI,TreeID)<100||XRESRC(NI,StoneID)<100)return;
	//if(rando()<10000&&RESRC[NI][GoldID]>600){
		//ProtectMine();
	//	return;
	//};
	Idea* ID=AddIdea(&TakeNewMineBrain,false);
	if(!ID)return;
	ID->DataSize=NMines*4-4+sizeof(TNM_Data);
	ID->IdeaData=malloc(ID->DataSize);
	TNM_Data* TDA=(TNM_Data*)ID->IdeaData;
	memcpy(&TDA->Mines[0],MineList,4*NMines);
	TDA->NAttempts=0;
};
void City::UpgradeSomeMine(){
};
void City::CreateWallNearMine(){
};
void City::BuildTower(){
};
//------------------------------Upgrade mines------------------------//
class UM_Data:public Inform{
public:
	word N;
	bool Gold;
	byte Ulevel;
	word Sids[10];
};
void PerformUpgradeLink(OneObject* OBJ);
void UpgradeMineBrain(Idea* ID){
	UM_Data* UM=(UM_Data*)ID->IdeaData;
	int N=UM->N;
	int best=-1;
	bool Gold=UM->Gold;
	MineBase* BBase=NULL;
	int Mindis=100000;
	City* CT=ID->CT;
	//if(CT->UnitAmount[CT->Nat->UID_PEASANT]<320)return;
	int cx=CT->CenterX<<7;
	int cy=CT->CenterY<<7;
	byte Lev=UM->Ulevel;
	int udst=CT->Nat->MINE_UPGRADE1_RADIUS<<5;
	for(int i=0;i<N;i++){
		word ess=UM->Sids[i];
		if(ess!=0xFFFF){
			MineBase* MB=(MineBase*)CT->SearchInform(0x4519,ess,NULL);
			if(MB){
				OneObject* OB=Group[MB->M_ID];
				if(OB&&OB->Serial==MB->M_SN){
					if(OB->SingleUpgLevel<=Lev+2){
						if((!Gold)||(Gold&&MB->ResKind==GoldID)){
							int dst=Norma(cx-(OB->RealX>>4),cy-(OB->RealY>>4));
							if(dst<Mindis&&dst<udst){
								Mindis=dst;
								best=i;
								BBase=MB;
							};
						};
					};
				};
			};
		};
	};
	if(best==-1){
		if(Gold)UM->Gold=false;
		else ID->ClearIdea();
		return;
	};
	UM->Sids[best]=-1;
	OneObject* OB=Group[BBase->M_ID];
	if(OB->LocalOrder&&OB->LocalOrder->DoLink==&PerformUpgradeLink)return;
	CT->N_mineUp[OB->SingleUpgLevel-2]++;
	OB->PerformUpgrade(CT->Nat->UGRP_MINEUP.UIDS[OB->SingleUpgLevel-2],OB->Index);

	ID->ClearIdea();
};
int MUtbl[10]={100,100,80,60,50,40,30,30,20,10};
int GetMineMult(int n){
	if(n<10)return MUtbl[n];
	else return MUtbl[9];
}
int GetMaxCostPercent(byte Nat,word* Cost){
	int maxp=0;
	//int * res=RESRC[Nat];
	for(int i=0;i<8;i++){
		if(Cost[i]){
			int p=XRESRC(Nat,i)?(int(Cost[i])*100)/XRESRC(Nat,i):1000;
			if(p>maxp)maxp=p;
		};
	};
	return maxp;
};
void City::UpgradeMines(){
	//Idea* ID1=AddIdea(&UpgradeMineBrain,false);
	//if(!ID1)return;
	//if(UnitAmount[Nat->UID_PEASANT]<280)return;
	MineBase* MBS=(MineBase*)SearchInform(0x4519,NULL);
	int MaxLev=100;
	int N=0;
	int cx=CenterX<<11;
	int cy=CenterY<<11;
	int udst=Nat->MINE_UPGRADE1_RADIUS<<9;
	word TotMines[32];
	word MIDX[32];
	int DST[32];
	byte Level[32];
	byte Type[32];
	int NMin=0;
//----------------------------//
	while(MBS){
		OneObject* OB=Group[MBS->M_ID];
		if(NMin<32&&OB&&OB->Serial==MBS->M_SN&&OB->Ready){
			int D=Norma(OB->RealX-cx,OB->RealY-cy);
			if(D<udst){
				TotMines[NMin]=OB->Index;
				MIDX[NMin]=NMin;
				Level[NMin]=OB->SingleUpgLevel-2;
				Type[NMin]=MBS->ResKind;
				NMin++;
			};
		};
		MBS=(MineBase*)SearchInform(0x4519,MBS);
	};
	if(!NMin)return;
	UNISORT.CheckSize(NMin);
	memcpy(UNISORT.Uids,MIDX,NMin<<1);
	memcpy(UNISORT.Parms,DST,NMin<<2);
	UNISORT.Sort();
	//upgrading to 3-rd Level
	int NU1=0;
	int TotM=0;

	word* L1PERCENT=Nat->MU1G_PERCENT;
	word* L2PERCENT=Nat->MU2G_PERCENT;
	word* L3PERCENT=Nat->MU3G_PERCENT;
	byte CurRes=GoldID;
	int MaxULevel=0;
	U_Grp* MUP=&Nat->UGRP_MINEUP;
	for(int q=0;q<MUP->N;q++)if(Nat->UPGRADE[MUP->UIDS[q]]->Enabled)MaxULevel=q;
	do{
		NU1=0;
		TotM=0;
		for(int i=0;i<NMin;i++){
			if(Type[i]==CurRes){
				TotM++;
				if(Level[i])NU1++;
			};
		};
		if(NU1!=TotM&&TotM){
			int p=GetMaxCostPercent(Nat->NNUM,Nat->UPGRADE[Nat->UGRP_MINEUP.UIDS[0]]->Cost);
			if(NU1>=2)NU1=2;
			if(L1PERCENT[NU1]>=p){
				//need to upgrade
				for(int i=0;i<NMin;i++){
					if(Level[MIDX[i]]==0){
						OneObject* OB=Group[TotMines[MIDX[i]]];
						OB->PerformUpgrade(Nat->UGRP_MINEUP.UIDS[0],OB->Index);
						return;
					};
				};
			};
		}else{
			//high level upgrade
			int NU=0;
			for(int j=0;j<NMin;j++){
				if(Type[MIDX[j]]==CurRes){
					if(Level[MIDX[j]]<=MaxULevel){
						int Lev=Level[MIDX[j]];
						int p;
						for(p=0;p<j;p++)if(Level[MIDX[p]]>=Lev)NU++;
						if(NU>2)NU=2;
						p=GetMaxCostPercent(Nat->NNUM,Nat->UPGRADE[Nat->UGRP_MINEUP.UIDS[Lev]]->Cost);
						if(Lev==1){
							if(L2PERCENT[NU]>=p){
								OneObject* OB=Group[TotMines[MIDX[j]]];
								OB->PerformUpgrade(Nat->UGRP_MINEUP.UIDS[Lev],OB->Index);
								return;
							};
						}else
						if(Lev>=2){
							if(L3PERCENT[NU]>=p){
								OneObject* OB=Group[TotMines[MIDX[j]]];
								OB->PerformUpgrade(Nat->UGRP_MINEUP.UIDS[Lev],OB->Index);
								return;
							};

						};
					};
				};
			};
		};
		if(CurRes==GoldID){
			L1PERCENT=Nat->MU1I_PERCENT;
			L2PERCENT=Nat->MU2I_PERCENT;
			L3PERCENT=Nat->MU3I_PERCENT;
			CurRes=IronID;
		}else
		if(CurRes==IronID){
			L1PERCENT=Nat->MU1C_PERCENT;
			L2PERCENT=Nat->MU2C_PERCENT;
			L3PERCENT=Nat->MU3C_PERCENT;
			CurRes=CoalID;
		}else CurRes=0xFF;
	}while(CurRes!=0xFF);

};



bool FindNearestWall(int* x,int* y,byte NI){
	int xx=(*x)>>6;
	int yy=(*y)>>6;
	int MyTop=GetTopology(*x,*y);
	if(MyTop>GetNAreas())return false;
	int MTPS=MyTop*GetNAreas();
	int MinDis=100000;
	int BestTop=0xFFFF;
	EnemyInfo* EIN=GNFO.EINF[NI];
	if(!EIN)return false;
	for(int i=0;i<EIN->NEnWalls;i++){
		int xw=EIN->WallsX[i];
		int yw=EIN->WallsY[i];
		int Ofs0=xw+(yw<<TopSH);
		word Tp_U=0xFFFF;
		word Tp_D=0xFFFF;
		word Tp_L=0xFFFF;
		word Tp_R=0xFFFF;
		if(xw>0&&yw>0&&xw<TopLx-1&&yw<TopLy-1){
			Tp_U=GetTopRef(Ofs0-TopLx);
			Tp_D=GetTopRef(Ofs0+TopLx);
			Tp_L=GetTopRef(Ofs0-1);
			Tp_R=GetTopRef(Ofs0+1);
		};
		int cds=0xFFFF;
		int LocBestTop=0xFFFF;
		if(Tp_U<0xFFFE){
			if(Tp_U==MyTop){
				cds=0;
				LocBestTop=Tp_U;
			}else{
				int dst=GetLinksDist(MTPS+Tp_U);
				if(dst<cds){
					cds=dst;
					LocBestTop=Tp_U;
				};
			};
		};
		if(Tp_D<0xFFFE){
			if(Tp_D==MyTop){
				cds=0;
				LocBestTop=Tp_D;
			}else{
				int dst=GetLinksDist(MTPS+Tp_D);
				if(dst<cds){
					cds=dst;
					LocBestTop=Tp_D;
				};
			};
		};
		if(Tp_L<0xFFFE){
			if(Tp_L==MyTop){
				cds=0;
				LocBestTop=Tp_L;
			}else{
				int dst=GetLinksDist(MTPS+Tp_L);
				if(dst<cds){
					cds=dst;
					LocBestTop=Tp_L;
				};
			};
		};
		if(Tp_R<0xFFFE){
			if(Tp_R==MyTop){
				cds=0;
				LocBestTop=Tp_R;
			}else{
				int dst=GetLinksDist(MTPS+Tp_R);
				if(dst<cds){
					cds=dst;
					LocBestTop=Tp_R;
				};
			};
		};
		if(cds<MinDis){
			MinDis=cds;
			BestTop=LocBestTop;
		};
		if(!MinDis){
			Area* AR=GetTopMap(BestTop);
			*x=int(AR->x)<<6;
			*y=int(AR->y)<<6;
			return true;
		};
	};
	if(BestTop!=0xFFFF){
		Area* AR=GetTopMap(BestTop);
		*x=int(AR->x)<<6;
		*y=int(AR->y)<<6;
		return true;
	};
	return false;
};

void City::SendAgressors(){
	int N=Agressors.NMemb;
	
	if(N>NAgressors){
		if(N<10){
			int N1=GetFreeBrigade();
			if(N1!=-1){
				Brigade* BR=&Brigs[N1];
				BR->Enabled=true;
				Agressors.RemoveObjects(NAgressors,BR);
				BR->MakeBattle();
				NAgressors+=5000;
				if(NAgressors>15000)NAgressors=15000;
			};
		}else NAgressors=15000;
	};
	/*
	if(N>100){
		int ID=GetFreeArmy();
		if(ID!=-1){
			AI_Army* ARM=ARMS+ID;
			ARM->AddBrigade(&Agressors);
			ARM->MakeBattle();
		};
	};
	*/
	
};
void City::ProtectMine(){
	PRM_Info* PR1=(PRM_Info*)SearchInform(0xFF4B,0,NULL);
	if(PR1){
		if(PR1->NBrigs>2)return;
	}else{
		PR1=new PRM_Info;
		PR1->NBrigs=0;
		PR1->ID=0xFF4B;
		PR1->Essence=0;
		PR1->Size=sizeof(PRM_Info);
		AddInform((Inform*)PR1,NULL);
	};
	int N1=GetFreeBrigade();
	if(N1!=-1){
		Brigade* BR=Brigs+N1;
		BR->Enabled=true;
		BrigMemb BMEM;
		memset(&BMEM,0,sizeof BrigMemb);
		BMEM.Peons=9;
		BMEM.Infantry=6;
		BMEM.Strelkov=2;
		BR->AddInRadius(CenterX<<7,CenterY<<7,20000,&BMEM);
		BrigMemb* BM1=&BR->BM;
		if(BM1->Peons!=9||BM1->Infantry+BM1->Strelkov<5){
			BR->Rospusk();
			BR->DeleteAll();
			BR->Enabled=false;
			return;
		};
		BR->ProtectFarMines();
		PR1->BrigsID[PR1->NBrigs]=BR->ID;
		PR1->BrigsSN[PR1->NBrigs]=BR->SN;
		PR1->NBrigs++;
	};
};
void City::CalculateBalance(){
	//NPeas=UnitAmount[Nat->UID_PEASANT];
	if(!NPeas){
		NeedPF=0;
		NeedPW=0;
		NeedPS=0;
		return;
	};
	int POnWood=Nat->POnWood;
	int POnFood=Nat->POnFood;
	int POnStone=Nat->POnStone;
	int tot=POnWood+POnStone+POnFood;

	if(Nat->hLibAI&&Nat->PBL&&tot){
		word nn=Nat->NPBal;
		word* pp=Nat->PBL;
		for(int i=0;i<nn,pp[0]<=NPeas;pp+=2,i++);
		int NeedOnRes=pp[-1]+(int(NPeas-pp[-2])*int(pp[1]-pp[-1]))/(pp[0]-pp[-2]);
		NeedPW=(POnWood*NeedOnRes)/tot;
		NeedPS=(POnStone*NeedOnRes)/tot;
		NeedPF=NeedOnRes-NeedPW-NeedPS;
	};/*else{
		word nn=Nat->NPBal;
		word* pp=Nat->PBalance;
		for(int i=0;i<nn,pp[0]<=NPeas;pp+=4,i++);
		int nF=pp[1];
		int nW=pp[2];
		int nS=pp[3];
		NeedPF=pp[-3]+((int(NPeas-pp[-4])*int(pp[1]-pp[-3]))/(pp[0]-pp[-4]));
		NeedPW=pp[-2]+((int(NPeas-pp[-4])*int(pp[2]-pp[-2]))/(pp[0]-pp[-4]));
		NeedPS=pp[-1]+((int(NPeas-pp[-4])*int(pp[3]-pp[-1]))/(pp[0]-pp[-4]));
	};*/
};
//----------------------Build walls near mines --------------------

class BWM_Idea{
public:
	word SID;
	MineBase MBAS;
	int Over;
	bool Ready;
};
int wrs=5;
bool CheckGateUpXY(int x,int y){
	return false;
};
void CreateGates(OneObject* OB);

void LeaveMineLink(OneObject* OBJ);
void BuildWallBrain(Idea* ID){
	
};
void CreateWallBar(int tx,int ty,int r,City* CT){

};
void City::BuildWallsNearMines(){

};
void FogSpot(int x,int y);
extern int minix;
extern int	miniy;
extern int MiniLx;
extern int MiniLy;
extern int MiniX;
extern int MiniY;
void HandleGeology(){
	// improved by Xeno
	if(GetKeyState(VK_MENU)&0x8000) return;
	else
	{	
		extern byte BaloonState;
		extern int FogMode;
		bool usefog=FogMode&&BaloonState!=1&&(!NATIONS[GSets.CGame.cgi_NatRefTBL[MyNation]].Vision);
		for(int i=0;i<NMines;i++){
			int MID=MineList[i];
			OneSprite* OSP=&Sprites[MID];
			if(OSP&&OSP->Enabled){
				bool vis=true;
				if(usefog&&!NATIONS[GSets.CGame.cgi_NatRefTBL[MyNation]].Geology)
				{
					int GetInterpFOW(int x,int y);
					vis=GetInterpFOW(OSP->x,OSP->y)>600;
				}
				if(vis)
				{
					// Edit by Xeno: draw mine image on map
					static int f=GPS.PreLoadGPImage("Interf3\\Mod\\Map_Res");
					int spr = 0;
					switch(OSP->OC->IntResType){
						case 1://gold
							spr = 2;
							break;
						case 4://iron
							spr = 1;
							break;
						case 5://crystal
							spr = 0;
							break;
					}
					int x=GetXOnMiniMap(OSP->x,OSP->y)-GPS.GetGPWidth(f,spr)/2;
					int y=GetYOnMiniMap(OSP->x,OSP->y)-GPS.GetGPHeight(f,spr)/2;

					GPS.ShowGP(x,y,f,spr,MyNation);

					/*
					int x=GetXOnMiniMap(OSP->x,OSP->y);
					int y=GetYOnMiniMap(OSP->x,OSP->y);
					DWORD CL=0;
					switch(OSP->OC->IntResType){
						case 1://gold
							CL=0xA0FFFF00;
							break;
						case 4://iron
							CL=0xA0FFFFFF;
							break;
						case 5:
							CL=0xA0000000;
							break;
					}
					GPS.DrawFillRect(x-1,y-1,3,3,CL);
					*/
				}
			}
		}
		GPS.FlushBatches();
	}
}
void City::HandleDefending(){
	if(rando()<512){
		//1.Check units 
		for(int i=0;i<NDefn;i++){
			DefendInfo* DI=DefInf+i;
			if(DI->NDefenders){
				int Minx=int(DI->x)<<12;
				int Miny=int(DI->y)<<12;
				int Maxx=Minx+4096;
				int Maxy=Miny+4096;
				word* IDS=DI->Def;
				word* USN=DI->DefSN;
				int N=DI->NDefenders;
				for(int j=0;j<N;j++){
					OneObject* OB=Group[IDS[j]];
					if(OB&&OB->Serial==USN[j]){
						//check coordinates
						if(OB->RealX<Minx||OB->RealX>Maxx||OB->RealY<Miny||OB->RealY>Maxy){
							OB->NewMonsterSendTo(Minx+(rando()&4095),Miny+(rando()&4095),0,0);
						};
					}else{
						if(j<N-1){
							memcpy(IDS+j,IDS+j+1,(N-j-1)<<1);
							memcpy(USN+j,USN+j+1,(N-j-1)<<1);
						};
						N--;
						DI->NDefenders--;
					};
				};
			};
		};
	};
	if(Defenders.NMemb){
		int N=Defenders.NMemb;
		word* IDS=Defenders.Memb;
		word* MSN=Defenders.MembSN;
		for(int i=0;i<N;i++){
			OneObject* OB=Group[IDS[i]];
			if(OB&&OB->Serial==MSN[i])AddUnitDefender(OB);
		};
		Defenders.RemoveObjects(N,&Guards);
		N=Guards.NMemb;
		IDS=Guards.Memb;
		MSN=Guards.MembSN;
		for(int i=0;i<N;i++){
			word MID=IDS[i];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==MSN[i])OB->DoNotCall=true;
			};
		};

	};
	//scanning the territory
	int N=NtNUnits[NI];
	word* UIDS=NatList[NI];
	if(rando()<300){
		for(int i=0;i<N;i++){
			OneObject* OB=Group[UIDS[i]];
			if(OB&&OB->NewBuilding&&OB->NNUM==NI){
				int cx=OB->RealX>>12;
				int cy=OB->RealY>>12;
				if(CheckDefending(cx,cy)==-1){
					AddDefending(cx,cy,1);
				};
			};
		};
	};
};
int City::CheckDefending(byte x,byte y){
	for(int i=0;i<NDefn;i++){
		if(x==DefInf[i].x&&y==DefInf[i].y)return i;
	};
	return -1;
};
void City::AddDefending(byte x,byte y,byte Imp){
	if(NDefn>=MaxDefn){
		MaxDefn+=32;
		DefInf=(DefendInfo*)realloc(DefInf,MaxDefn*sizeof DefendInfo);
	};
	DefendInfo* DI=DefInf+NDefn;
	DI->x=x;
	DI->y=y;
	DI->Importance=Imp;
	DI->NDefenders=0;
	DI->MaxDefs=0;
	DI->Def=NULL;
	DI->DefSN=NULL;
	NDefn++;
};
void City::AddUnitDefender(OneObject* OB){
	if(!NDefn)return;
	int didx=-1;
	int dmin=100;
	for(int i=0;i<NDefn;i++){
		if(DefInf[i].NDefenders<dmin){
			didx=i;
			dmin=DefInf[i].NDefenders;
		};
	};
	DefInf[didx].AddUnit(OB);
};
void DefendInfo::AddUnit(OneObject* OB){
	if(NDefenders>=MaxDefs){
		MaxDefs+=8;
		Def=(word*)realloc(Def,MaxDefs*2);
		DefSN=(word*)realloc(DefSN,MaxDefs*2);
	};
	OB->NoBuilder=true;
	OB->DoNotCall=true;
	Def[NDefenders]=OB->Index;
	DefSN[NDefenders]=OB->Serial;
	NDefenders++;
};
//------------------------------------ARMY--------------------------------//

byte ArmType[34]={0xFF,
                  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,   2,   5,   5,
                     6,0xFF,0xFF,0xFF,   8,0xFF,0xFF,   9,0xFF,   0,
					 1,   3,   7,   4,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
					 1,   1,   1};
byte ArmSpec[34]={0,
                  0,0,0,0,0,2,0,0,0,0,
				  0,0,0,0,0,0,0,0,0,0,
				  0,0,0,0,0,0,0,0,0,0,
				  0,0,0};

#define MaxBT 10
//0-Light infantry(short range)
//1-infantry(strelok)
//2-fasthorce
//3-hardhorce
//4-horse-strelok
//5-mortira&pushka
//6-grenader
//7-peasant
//8-weak units
//9-supermortira
//word BriMax[MaxBT]={36,16,16,16,16,36,16,16,25,36};
word BriMax[MaxBT]=   {256,256,256,256,256,36,256,16,256,256};
word BriMin[MaxBT]=   {36 ,16 , 16, 16, 16, 1, 16,16, 25,  4};
word DefBriMin[MaxBT]={4 ,16 , 16, 16, 16, 1, 16,16, 25,  4};
void CheckArmies(City* CT){
	return;
	for(int r=0;r<4;r++){
		CT=CITY+r;
		for(int w=0;w<MaxBrig;w++){
			Brigade* BR=CT->Brigs+w;
			if(BR->Enabled){
				int N=BR->NMemb;
				word* Mem=BR->Memb;
				word* MSN=BR->MembSN;
				for(int q=0;q<N;q++){
					word MID=Mem[q];
					if(MID!=0xFFFF){
						OneObject* OB=Group[MID];
						//assert(OB);
						if(OB){
							//assert((!OB->Sdoxlo)||(OB->Sdoxlo&&OB->Hidden));
							//assert(OB->Serial==MSN[q]);
							//assert(OB->BrigadeID==w);
							//assert(OB->BrIndex==q);
						};
					};
				};
			};

		};
	};
	return;
	word* Amid=CT->DefArms;
	int N=CT->NDefArms;
	for(int i=0;i<N;i++){
		int NA=0;
		//assert(Amid[i]<400);
		AI_Army* AIR=CT->ARMS+Amid[i];
		//assert(AIR->Enabled);
		int N1=AIR->NExBrigs;
		int ID=AIR->ArmyID;
		for(int j=0;j<N1;j++){
			Brigade* BR=AIR->ExBrigs[j].Brig;
			assert(BR);
			if(BR){
				assert(BR->ArmyID==ID);
				NA+=BR->NMemb;
			};
			for(int j=0;j<BR->NMemb;j++){
				OneObject* OB=Group[BR->Memb[j]];
				if(OB&&OB->Serial==BR->MembSN[j]){
					assert(OB->BrigadeID==BR->ID);
				};
			};
		};
		if(!NA){
			AIR->ClearArmy();
			AIR->Enabled=false;
		};
	};
	
	N=CT->NDefn;
	for(int i=0;i<N;i++){
		DefendInfo* DI=CT->DefInf+i;
		if(DI->NDefenders){
			//assert(DI->NDefenders<=DI->MaxDefs);
		};
	};

};
void AI_Army::InitArmy(City* C){
	memset(this,0,sizeof AI_Army);
	CT=C;
	NT=C->Nat;
	NI=C->NI;
	LastBitvaTime=-10000;
};
void AI_Army::ClearArmy(){
	for(int i=0;i<NExBrigs;i++){
		ExBrigs[i].Brig->DeleteAll();
		ExBrigs[i].Brig->Enabled=false;
	};
	if(ExBrigs)free(ExBrigs);
	InitArmy(CT);
	LastBitvaTime=-10000;
	SpecialOrder=false;
};
int CheckMinArmyCreationAbility(Brigade* BR){
	int N=BR->NMemb;
	word* Mem=BR->Memb;
	word NBMEM[MaxBT+4];
	memset(NBMEM,0,sizeof NBMEM);

	for(int i=0;i<N;i++){
		word MID=Mem[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB){
				int idx=ArmType[OB->Usage];
				if(idx!=0xFF)NBMEM[idx]++;
			};
		};
	};
	if(BR->CT->DefenceStage){
		for(int i=0;i<MaxBT;i++){
			if(NBMEM[i]>=DefBriMin[i]){
				return i;
			};
		};
	}else{
		for(int i=0;i<MaxBT;i++){
			if(NBMEM[i]>=BriMin[i]){
				return i;
			};
		};
	};
	return -1;
};
int CheckSuperMinArmyCreationAbility(Brigade* BR){
	int N=BR->NMemb;
	word* Mem=BR->Memb;
	word NBMEM[MaxBT+4];
	memset(NBMEM,0,sizeof NBMEM);
	for(int i=0;i<N;i++){
		word MID=Mem[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB){
				int idx=ArmType[OB->Usage];
				if(idx!=0xFF)NBMEM[idx]++;
			};
		};
	};
	for(int i=0;i<MaxBT;i++){
		if(NBMEM[i]>=1){
			return i;
			//if(i!=5)return i;
			//else if(NBMEM[0]>=8)return i;
		};
	};
	return -1;
};
void AI_Army::CreateMinimalArmyFromBrigade(Brigade* BR,int Type){
	int N=BR->NMemb;
	word IDS[2048];
	memcpy(IDS,BR->Memb,N<<1);
	word BriInd[MaxBT];
	memset(BriInd,0xFF,MaxBT<<1);
	int Nu=0;
	int NuMax=BriMax[Type];
	//if(Type==5)NuMax+=8;
	int N0=0;
	for(int i=0;i<N;i++){
		word MID=IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB){
				int AT=ArmType[OB->Usage];
				//if(Type==5&&N0<8&&AT==0){
				//	AT=Type;
				//	N0++;
				//};
				if(AT==Type){
					Spec=ArmSpec[AT];
					if(BriInd[AT]!=0xFFFF&&ExBrigs[BriInd[AT]].Brig->NMemb>=BriMax[AT])BriInd[AT]=0xFFFF;
					if(BriInd[AT]==0xFFFF){
						if(NExBrigs){
							bool unfound=true;
							for(int i=0;i<NExBrigs&&!(ExBrigs[i].BrigadeType==AT&&ExBrigs[i].Brig->NMemb<BriMax[AT]);i++);
							if(i<NExBrigs)BriInd[AT]=i;
						};
					};
					if(BriInd[AT]==0xFFFF){
						int BIN=CT->GetFreeBrigade();
						if(BIN==-1)return;
						BriInd[AT]=NExBrigs;
						if(NExBrigs>=MaxExBrigs){
							MaxExBrigs+=32;
							ExBrigs=(ExtendedBrigade*)realloc(ExBrigs,MaxExBrigs*sizeof ExtendedBrigade);
						};
						ExtendedBrigade* EXB=ExBrigs+NExBrigs;
						EXB->Brig=CT->Brigs+BIN;
						EXB->Brig->Enabled=1;
						NExBrigs++;
						EXB->BrigadeType=AT;
						EXB->Brig->ArmyID=ArmyID;
						EXB->Force=0;
						EXB->NeedMembers=0;
						EXB->NextBrigade=0;

					};
					ExtendedBrigade* EXB=ExBrigs+BriInd[AT];
					Brigade* NBR=EXB->Brig;
					BR->RemoveOne(OB->BrIndex,NBR);
					OB->Zombi=1;
					OB->DoNotCall=true;
					OB->NoBuilder=true;
					Nu++;
					if(Nu>=NuMax){
						return;
					};
				};
			};
		};
	};
};
void AI_Army::AddBrigade(Brigade* BR){
	if(BR->WarType){
		if(NExBrigs>=MaxExBrigs){
			MaxExBrigs+=32;
			ExBrigs=(ExtendedBrigade*)realloc(ExBrigs,MaxExBrigs*sizeof ExtendedBrigade);
		};
		ExtendedBrigade* EXB=ExBrigs+NExBrigs;
		EXB->Brig=BR;
		BR->ArmyID=ArmyID;
		EXB->BrigadeType=0;
		EXB->Force=0;
		EXB->NeedMembers=0;
		EXB->NextBrigade=0;
		NExBrigs++;
		return;
	};
	int N=BR->NMemb;
	word IDS[1024];
	memcpy(IDS,BR->Memb,N<<1);
	word BriInd[MaxBT];
	memset(BriInd,0xFF,MaxBT<<1);
	for(int i=0;i<N;i++){
		word MID=IDS[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB){
				int AT=ArmType[OB->Usage];
				if(AT!=0xFF){
					Spec=ArmSpec[AT];
					if(BriInd[AT]!=0xFFFF&&ExBrigs[BriInd[AT]].Brig->NMemb>=BriMax[AT])BriInd[AT]=0xFFFF;
					if(BriInd[AT]==0xFFFF){
						if(NExBrigs){
							bool unfound=true;	
							for(int i=0;i<NExBrigs&&!(ExBrigs[i].BrigadeType==AT&&ExBrigs[i].Brig->NMemb<BriMax[AT]&&ExBrigs[i].Brig->WarType==0);i++);
							if(i<NExBrigs)BriInd[AT]=i;
						};
					};
					if(BriInd[AT]==0xFFFF){
						int BIN=CT->GetFreeBrigade();
						if(BIN==-1)return;
						BriInd[AT]=NExBrigs;
						if(NExBrigs>=MaxExBrigs){
							MaxExBrigs+=32;
							ExBrigs=(ExtendedBrigade*)realloc(ExBrigs,MaxExBrigs*sizeof ExtendedBrigade);
						};
						ExtendedBrigade* EXB=ExBrigs+NExBrigs;
						EXB->Brig=CT->Brigs+BIN;
						EXB->Brig->Enabled=1;
						EXB->Brig->ArmyID=ArmyID;
						NExBrigs++;
						EXB->BrigadeType=AT;
						EXB->Force=0;
						EXB->NeedMembers=0;
						EXB->NextBrigade=0;

					};
					ExtendedBrigade* EXB=ExBrigs+BriInd[AT];
					Brigade* NBR=EXB->Brig;
					BR->RemoveOne(OB->BrIndex,NBR);
					OB->Zombi=1;
					OB->DoNotCall=true;
					OB->NoBuilder=true;
					OB->InArmy=0;
				};
			};
		};
	};
};	
ArmyOrder* AI_Army::CreateOrder(byte OrdType,int Size){
	ArmyOrder* OR1=(ArmyOrder*)malloc(Size);
	ArmyOrder* OR2;
	switch(OrdType){
	case 1:
		OR1->Next=AOrder;
		AOrder=OR1;
		break;
	case 2:
		OR1->Next=NULL;
		if(AOrder){
			OR2=AOrder;
			while(OR2->Next)OR2=OR2->Next;
			OR2->Next=OR1;
		}else AOrder=OR1;
		break;
	default:
		ClearAOrders();
		AOrder=OR1;
		OR1->Next=NULL;
	};
	return OR1;
};
void AI_Army::DeleteAOrder(){
	if(AOrder){
		ArmyOrder* OR1=AOrder->Next;
		free(AOrder);
		AOrder=OR1;
	};
};
void AI_Army::ClearAOrders(){
	while(AOrder)DeleteAOrder();
};
int City::GetFreeArmy(){
	for(int i=0;i<MaxArm;i++){
		if(!ARMS[i].Enabled){
			AI_Army* AR=ARMS+i;
			AR->InitArmy(this);
			AR->Enabled=true;
			AR->TopPos=-1;
			AR->ArmyID=i;
			return i;
		};
	};
	return -1;
};
//++++++++++++++f+++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++ LOCAL SEND TO +++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

class ALST_Order:public ArmyOrder{
public:
	int N;
	word pos[15];//[xi][yi][BrigID][SN][done]
};
__forceinline bool ValidTXY(int x,int y){
	int TX=TopLx-1;
	return x>0&&y>0&&x<TX&&y<TX;
};
void CheckTopPos(int xc,int yc,int* xd,int* yd){
	int x=*xd;
	int y=*yd;
	int mx=msx>>1;
	int my=msy>>1;

	if(xc<=0)xc=1;
	if(yc<=0)yc=1;
	if(xc>=mx)xc=mx-1;
	if(yc>=my)yc=my-1;

	if(x<=0)x=1;
	if(y<=0)y=1;
	if(x>=mx)x=mx-1;
	if(y>=my)y=my-1;

	int sx=xc>x?-1:1;
	int sy=yc>y?-1:1;
	int ax=abs(xc-x);
	int ay=abs(yc-y);
	int xp=xc;
	int yp=yc;
	if(ax>ay){
		int cum=ax>>1;
		while(xc!=x){
			xc+=sx;
			cum+=ay;
			if(cum>=ax){
				cum-=ax;
				yc+=sy;
			};
			int of1=xc+(yc<<TopSH);
			if(ValidTXY(xc,yc)&&GetTopRef(of1)>=0xFFFE||GetTopRef(of1+1)>=0xFFFE||GetTopRef(of1+TopLx)>=0xFFFE||GetTopRef(of1+TopLx+1)>=0xFFFE){
				*xd=xp;
				*yd=yp;
				return;
			};
		};
	}else{
		int cum=ay>>1;
		while(yc!=y){
			yc+=sy;
			cum+=ax;
			if(cum>=ay){
				cum-=ay;
				xc+=sx;
			};
			int of1=xc+(yc<<TopSH);
			if(ValidTXY(xc,yc)&&GetTopRef(of1)>=0xFFFE||GetTopRef(of1+1)>=0xFFFE||GetTopRef(of1+TopLx)>=0xFFFE||GetTopRef(of1+TopLx+1)>=0xFFFE){
				*xd=xp;
				*yd=yp;
				return;
			};
		};
	};
	*xd=xc;
	*yd=yc;
	return;
};
char* AO_LST="[AI_Army::LocalSendTo]";
bool CheckArmyDanger(AI_Army* ARM);
void ArmyLocalSendToLink(AI_Army* ARM){
	if(rando()<16384){
		if(CheckArmyDanger(ARM)){
			ARM->DeleteAOrder();
			ARM->Bitva();
			return;
		};
	};

	rando();
	ALST_Order* AOR=(ALST_Order*)ARM->AOrder;
	word* pos=AOR->pos;
	int N=AOR->N;
	int ps=0;
	City* CT=ARM->CT;
	bool NoReady=false;
	int NR=0;
	int RD=0;
	for(int i=0;i<N;i++){
		if(!pos[ps+4]){
			Brigade* BR=CT->Brigs+pos[ps+2];
			if(BR->Enabled){
				if(BR->SN==pos[ps+3]){
					if(BR->BOrder)NR++;
					else RD++;
				};
			};
		};
		ps+=5;
	};
	if(!NR){
		ARM->DeleteAOrder();
		ARM->Parad();
	};
};
void AI_Army::LocalSendTo(int x,int y,byte Prio,byte OrdType){
	if(!NExBrigs)return;
	if(CheckArtillery())return;
	x>>=6;
	y>>=6;
	int sz=sizeof(ALST_Order)+(NExBrigs-3)*10;
	ALST_Order* AOR=(ALST_Order*)CreateOrder(OrdType,sz);
	AOR->N=NExBrigs;
	int odis=4;
	int Nx=int(sqrt(NExBrigs));
	int Ny=Nx;
	if(Nx*Ny<NExBrigs)Nx++;
	if(Nx*Ny<NExBrigs)Ny++;
	int x0=x-(((Nx-1)*odis)>>1);
	int y0=y-(((Ny-1)*odis)>>1);
	int ps=0;
	int nn=0;
/*	
	int xs=0;
	int ys=0;
	int nu=0;
	int nb=NExBrigs;
	for(int j=0;j<nb;j++){
		Brigade* BR=ExBrigs[j].Brig;
		int xb=0;
		int yb=0;
		if(BR->GetCenter(&xb,&yb)){
			xs+=xb;
			ys+=yb;
			nu++;
		};
	};
	int dx=0;
	int dy=0;
	if(nu){
		xs/=nu;
		ys/=nu;
		dx=(x<<6)-xs;
		dy=(y<<6)-ys;
		int N=Norma(dx,dy);
		if(N){
			dx=(dx*200)/N;
			dy=(dy*200)/N;
		};
	};
	*/
	for(int ix=0;ix<Nx;ix++){
		for(int iy=0;iy<Ny;iy++){
			if(nn<NExBrigs){
				int x1=x0+ix*odis;
				int y1=y0+iy*odis;
				CheckTopPos(x,y,&x1,&y1);
				AOR->pos[ps]=x1;
				AOR->pos[ps+1]=y1;
				Brigade* BR=ExBrigs[nn].Brig;
				AOR->pos[ps+2]=BR->ID;
				AOR->pos[ps+3]=BR->SN;
				AOR->pos[ps+4]=0;
				if(ExBrigs[nn].BrigadeType!=5)BR->LocalSendTo((x1<<6)+32,(y1<<6)+32,Prio,0);				
				else BR->LinearLocalSendTo((x1<<6)+32,(y1<<6)+32,Prio,0);
				ps+=5;
				nn++;
			};
		};
	};
	AOR->Message=AO_LST;
	AOR->Prio=0;
	AOR->Size=sz;
	AOR->ALink=&ArmyLocalSendToLink;
};

void AI_Army::WideLocalSendTo(int x,int y,byte Prio,byte OrdType){
	if(!NExBrigs)return;
	x>>=6;
	y>>=6;
	int sz=sizeof(ALST_Order)+(NExBrigs-3)*10;
	ALST_Order* AOR=(ALST_Order*)CreateOrder(OrdType,sz);
	AOR->N=NExBrigs;
	int odis=4;
	int Nx=int(sqrt(NExBrigs));
	int Ny=Nx;
	if(Nx*Ny<NExBrigs)Nx++;
	if(Nx*Ny<NExBrigs)Ny++;
	int x0=x-(((Nx-1)*odis)>>1);
	int y0=y-(((Ny-1)*odis)>>1);
	int ps=0;
	int nn=0;
	for(int ix=0;ix<Nx;ix++){
		for(int iy=0;iy<Ny;iy++){
			if(nn<NExBrigs){
				int x1=x0+ix*odis;
				int y1=y0+iy*odis;
				CheckTopPos(x,y,&x1,&y1);
				AOR->pos[ps]=x1;
				AOR->pos[ps+1]=y1;
				Brigade* BR=ExBrigs[nn].Brig;
				AOR->pos[ps+2]=BR->ID;
				AOR->pos[ps+3]=BR->SN;
				AOR->pos[ps+4]=0;
				BR->WideLocalSendTo((x1<<6)+32,(y1<<6)+32,Prio,0);
				ps+=5;
				nn++;
			};
		};
	};
	AOR->Message=AO_LST;
	AOR->Prio=0;
	AOR->Size=sz;
	AOR->ALink=&ArmyLocalSendToLink;
};
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//++++++++++++++++++++++++++ CONNECT TO ARMY ++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
char* ACTA_Message="[AI_Army::ConnectToArmy]";
class ACTA_Order:public ArmyOrder{
public:
	int ID;
};
void ArmyConnectToArmyLink(AI_Army* ARM){
	rando();
	ACTA_Order* ACO=(ACTA_Order*)ARM->AOrder;
	if(ARM->TopPos==-1){
		ARM->Parad();
		return;
	};
	int ID=ACO->ID;
	AI_Army* MAR=ARM->CT->ARMS+ID;
	if(!MAR->Enabled){
		ARM->DeleteAOrder();
		return;
	};
	if(MAR->TopPos==-1){
		MAR->Parad();
		return;
	};
	int MyTop=ARM->TopPos;
	int HisTop=MAR->TopPos;
	int NextTop=GetMotionLinks(MyTop*GetNAreas()+HisTop);
	if(HisTop==MyTop||NextTop==HisTop){
		//Can connect now
		ARM->ClearAOrders();
		int N=ARM->NExBrigs;
		for(int i=0;i<N;i++){
			Brigade* BR=ARM->ExBrigs[i].Brig;
			MAR->AddBrigade(BR);
			if(BR->NMemb){
				BR->RemoveObjects(BR->NMemb,&MAR->CT->Defenders);
			};
			BR->DeleteAll();
			BR->Enabled=false;
		};
		ARM->ClearArmy();
		ARM->Enabled=false;
		return;
	}else{
		if(NextTop!=0xFFFF){
			//Can move nearer
			Area* NXAR=GetTopMap(NextTop);
			ARM->LocalSendTo(int(NXAR->x)<<6,int(NXAR->y)<<6,0,1);
		};
	};
};
void AI_Army::ConnectToArmy(int ID,byte Prio,byte OrdType){
	ACTA_Order* AOR=(ACTA_Order*)CreateOrder(OrdType,sizeof ACTA_Order);
	AOR->Message=ACTA_Message;
	AOR->Prio=Prio;
	AOR->ID=ID;
	AOR->ALink=&ArmyConnectToArmyLink;
	if(TopPos==-1)Parad();
	if(TopPos==-1){
		DeleteAOrder();
	};
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//++++++++++++++++++++++++++++++++ PARAD ++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

void AI_Army::Parad(){
	if(!NExBrigs)return;
	int N=0;
	int xx=0;
	int yy=0;
	for(int i=0;i<NExBrigs;i++){
		Brigade* BR=ExBrigs[i].Brig;
		if(BR){
				int ax,ay;
			if(BR->GetCenter(&ax,&ay)){
				xx+=ax;
				yy+=ay;
				N++;
				};
		};
	};
	if(N){
		xx/=N;
		yy/=N;
		x=xx;
		y=yy;
		int tp=GetTopology(xx,yy);
			if(tp==0xFFFF)tp=-1;
		else{
			if(TopPos==-1){
				LocalSendTo(xx,yy,0,1);
			};
		};
		TopPos=tp;
	};
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//++++++++++++++++++++  PROCESSING SIMPLE BATTLE ++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
class Army_BTL:public ArmyOrder{
public:
	int xdest;
	int ydest;
	int time;
};
int GetTopDanger(int Top,byte NI){
	if(GNFO.EINF[NI])return 0;
	Area* AR=GetTopMap(Top);
	int MyX=AR->x>>2;
	int MyY=AR->y>>2;
	ArmyInfo* AINF=GNFO.EINF[NI]->GAINF.AINF;
	int Na=GNFO.EINF[NI]->GAINF.NArmy;
	int EnForces=0;
	for(int i=0;i<Na;i++){
		if(Norma(int(AINF->MaxX)-MyX,int(AINF->MaxY)-MyY)<10||
		   Norma(int(AINF->MaxX)-MyX,int(AINF->MinY)-MyY)<10||
		   Norma(int(AINF->MinX)-MyX,int(AINF->MaxY)-MyY)<10||
		   Norma(int(AINF->MinX)-MyX,int(AINF->MinY)-MyY)<10){
			int curforce=AINF->N;//>NSword+AINF->NCaval+(AINF->NStrel<<1);
			EnForces+=curforce;
		};
		AINF++;
	};
	return EnForces;
};
int GetTopDanger(int Top,int r,int NI){
	if(GNFO.EINF[NI])return 0;
	if(!GetNAreas())return 0;
	Area* AR=GetTopMap(Top);
	int MyX=AR->x>>2;
	int MyY=AR->y>>2;
	ArmyInfo* AINF=GNFO.EINF[NI]->GAINF.AINF;
	int Na=GNFO.EINF[NI]->GAINF.NArmy;
	int EnForces=0;
	for(int i=0;i<Na;i++){
		if(Norma(int(AINF->MaxX)-MyX,int(AINF->MaxY)-MyY)<r||
		   Norma(int(AINF->MaxX)-MyX,int(AINF->MinY)-MyY)<r||
		   Norma(int(AINF->MinX)-MyX,int(AINF->MaxY)-MyY)<r||
		   Norma(int(AINF->MinX)-MyX,int(AINF->MinY)-MyY)<r){
			int curforce=AINF->N;//>NSword+AINF->NCaval+(AINF->NStrel<<1);
			EnForces+=curforce;
		};
		AINF++;
	};
	return EnForces;
};
/*
#define AS_ARTILLERY 0x10000
#define AS_SHIPS     0x20000
#define AS_TOWERS    0x40000
#define AS_BATTLE    0x80000
byte GetDangValue(int x,int y);
int FastCheckPosDanger(int x,int y);
DWORD AnalyseArmyPosition(City* CT,AI_Army* ARM,int xp,int yp){
	int MaxDang=0;
	int x=xp>>6;
	int y=yp>>6;
	int odis=4;
	int NExBrigs=ARM->NExBrigs;
	int Nx=sqrt(NExBrigs);
	int Ny=Nx;
	if(Nx*Ny<NExBrigs)Nx++;
	if(Nx*Ny<NExBrigs)Ny++;
	int x0=x-(((Nx-1)*odis)>>1);
	int y0=y-(((Ny-1)*odis)>>1);
	int ps=0;
	int nn=0;
	int Dang=0;
	DWORD State=0;
	for(int ix=0;ix<Nx;ix++){
		for(int iy=0;iy<Ny;iy++){
			if(nn<NExBrigs){
				int x1=x0+ix*odis;
				int y1=y0+iy*odis;
				CheckTopPos(x,y,&x1,&y1);
				byte D=GetDangValue(x1>>1,y1>>1);
				if(D>128)State|=AS_SHIPS;
				else 
				if(D)State|=AS_ARTILLERY;
				//int xx=(x1<<6)+32;
				//int yy=(y1<<6)+32;
				int Dang=FastCheckPosDanger(x1-2,y1-2);
				if(Dang>MaxDang)MaxDang=Dang;
				Dang=FastCheckPosDanger(x1-2,y1+1);
				if(Dang>MaxDang)MaxDang=Dang;
				Dang=FastCheckPosDanger(x1+2,y1-2);
				if(Dang>MaxDang)MaxDang=Dang;
				Dang=FastCheckPosDanger(x1+2,y1+2);
				if(Dang>MaxDang)MaxDang=Dang;
				if(Dang)State|=AS_TOWERS;
				ps+=5;
				nn++;
			};
		};
	};
	int MyX=x>>2;
	int MyY=y>>2;
	int EnForces=0;
	int MaxForce=0;
	int MaxForceX;
	int MaxForceY;
	//int DoBitva=false;
	int rmin=4;
	int Endst=8;
	if(ARM->Spec==2){
		Endst=4;
		rmin=2;
	};
	DWORD DNG0=GetEnemyForce(MyX,MyY,0,rmin);
	DWORD DNG=DNG0+GetEnemyForce(MyX,MyY,rmin+1,Endst);
	if(DNG0)State|=AS_BATTLE;
	int cx=CT->CenterX>>1;
	int cy=CT->CenterY>>1;
	int R=Norma(MyX-cx,MyY-cy);
	if(R<10)DNG>>=3;
	else if(R<25)DNG>>=1;
	if(DNG>65000)DNG=65000;
	if(MaxDang>500)MaxDang=200;
	return State|DNG|(MaxDang<<24);
};
*/
word GetNextGreCell(byte NI,int F,int Start);
word GetNextDivCell(byte NI,int F,int Start);
bool HandleArchers(AI_Army* ARM,int R);
bool CheckArmyDanger(AI_Army* ARM){
	bool DoBitva=0;
	ArmyInfo* AINF=GNFO.EINF[ARM->CT->NI]->GAINF.AINF;
	int Na=GNFO.EINF[ARM->CT->NI]->GAINF.NArmy;
	int MyX=(ARM->x)>>8;
	int MyY=(ARM->y)>>8;
	int EnForces=0;
	int MaxForce=0;
	int MaxForceX=10000000;
	int MaxForceY=10000000;
	int rmin=5;
	int Endst=8;
	if(ARM->Spec==2)Endst=4;
	if(ARM->Spec==2)rmin=3;
	for(int i=0;i<Na;i++){
		int rr=Norma(int(AINF->MaxX)-MyX,int(AINF->MaxY)-MyY);
		int rr1=Norma(int(AINF->MaxX)-MyX,int(AINF->MinY)-MyY);
		if(rr1<rr)rr=rr1;
		rr1=Norma(int(AINF->MinX)-MyX,int(AINF->MaxY)-MyY);
		if(rr1<rr)rr=rr1;
		rr1=Norma(int(AINF->MinX)-MyX,int(AINF->MinY)-MyY);
		if(rr1<rr)rr=rr1;

		if(rr<Endst){
			int curforce=AINF->N;//NSword+AINF->NCaval+(AINF->NStrel>>2)+AINF->NStrel;
			if(curforce>MaxForce){
				MaxForce=curforce;
				MaxForceX=(int(AINF->MaxX)+int(AINF->MinX))>>1;
				MaxForceY=(int(AINF->MaxY)+int(AINF->MinY))>>1;
			};
			EnForces+=curforce;
		};
		AINF++;
		if(rr<rmin)DoBitva=true;
	};
	return DoBitva;
};
void ArmyMakeBattleLink(AI_Army* ARM){
	if(PeaceTimeLeft)return;
	if(HandleArchers(ARM,18)){
		ARM->Bitva();
		return;
	};
	rando();
	ARM->SendGrenaders();
	Army_BTL* OR1=(Army_BTL*)ARM->AOrder;
	if((tmtmt-OR1->time)<64)return;
	OR1->time=tmtmt;
	if(!ARM->NExBrigs){
		ARM->ClearArmy();
		ARM->Enabled=false;
		return;
	};
	if(ARM->TopPos==-1){
		ARM->Parad();
		return;
	}else ARM->Parad();
	
	int Dsx=int(ARM->x);
	int Dsy=int(ARM->y);
	//check near army to connect
	City* CT=ARM->CT;
	int Na=CT->NDefArms;
	word *Amid=CT->DefArms;
	int MyTop=ARM->TopPos;
	int NA=GetNAreas();
	int MTPM=MyTop*NA;
	int NearestGreatArmyTop=-1;
	int MinArmDist=1600;
	int MySize=ARM->NExBrigs;
	//assert(MyTop!=0xFFFF);
	if(MyTop!=-1&&MyTop!=0xFFFF){
		for(int i=0;i<Na;i++){
			AI_Army* AIAR=CT->ARMS+Amid[i];
			int Atop=AIAR->TopPos;
			if((ARM->Spec==AIAR->Spec)&&(!AIAR->SpecialOrder)&&Atop>=0&&Atop<NA&&AIAR!=ARM){
				if(Atop==MyTop||GetMotionLinks(MTPM+Atop)==Atop){
					//Can connect now
					AIAR->ClearAOrders();
					int N=AIAR->NExBrigs;
					for(int j=0;j<N;j++){
						Brigade* BR=AIAR->ExBrigs[j].Brig;
						ARM->AddBrigade(BR);
						if(!BR->WarType){
							if(BR->NMemb){
								BR->RemoveObjects(BR->NMemb,&ARM->CT->Defenders);
							};
							BR->DeleteAll();
							BR->Enabled=0;
						};
					};
					AIAR->NExBrigs=0;
					AIAR->ClearArmy();
					AIAR->Enabled=false;
					if(i<Na-1){
						memcpy(Amid+i,Amid+i+1,(Na-i-1)<<1);
					};
					CT->NDefArms--;
					return;
				}else{
					if(AIAR->NExBrigs>=MySize){
						int d=GetLinksDist(MTPM+Atop);
						if(d<MinArmDist){
							MinArmDist=d;
							NearestGreatArmyTop=Atop;
						};
					};
				};
			};
		};
	};
	//check safety
	int MyForce=0;
	int NMEM=0;
	for(int i=0;i<ARM->NExBrigs;i++){
		Brigade* BR=ARM->ExBrigs->Brig;
		if(BR&&BR->NMemb){
			int N=BR->NMemb;
			word* Mem=BR->Memb;
			word* MemSN=BR->MembSN;
			for(int i=0;i<N;i++){
				word MID=Mem[i];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&OB->Serial==MemSN[i]&&!OB->Sdoxlo){
						MyForce+=OB->newMons->Force;
						NMEM++;
					};
				};
			};
		};
	};
	if(NMEM>200||MyForce>3800)MyForce=100000;
	ArmyInfo* AINF=GNFO.EINF[ARM->CT->NI]->GAINF.AINF;
	Na=GNFO.EINF[ARM->CT->NI]->GAINF.NArmy;
	int MyX=(ARM->x)>>8;
	int MyY=(ARM->y)>>8;
	int EnForces=0;
	int MaxForce=0;
	int MaxForceX=10000000;
	int MaxForceY=10000000;
	int DoBitva=false;
	int rmin=4;
	int Endst=7;
	if(ARM->Spec==2)Endst=3;
	if(ARM->Spec==2)rmin=2;
	for(int i=0;i<Na;i++){
		int rr=Norma(int(AINF->MaxX)-MyX,int(AINF->MaxY)-MyY);
		int rr1=Norma(int(AINF->MaxX)-MyX,int(AINF->MinY)-MyY);
		if(rr1<rr)rr=rr1;
		rr1=Norma(int(AINF->MinX)-MyX,int(AINF->MaxY)-MyY);
		if(rr1<rr)rr=rr1;
		rr1=Norma(int(AINF->MinX)-MyX,int(AINF->MinY)-MyY);
		if(rr1<rr)rr=rr1;

		if(rr<Endst){
			int curforce=AINF->N;//NSword+AINF->NCaval+(AINF->NStrel>>2)+AINF->NStrel;
			if(curforce>MaxForce){
				MaxForce=curforce;
				MaxForceX=(int(AINF->MaxX)+int(AINF->MinX))>>1;
				MaxForceY=(int(AINF->MaxY)+int(AINF->MinY))>>1;
			};
			EnForces+=curforce;
		};
		AINF++;
		if(rr<rmin)DoBitva=true;
	};
	int cx=CT->CenterX>>1;
	int cy=CT->CenterY>>1;
	int RR1=Norma(MaxForceX-cx,MaxForceY-cy);
	if(RR1<10)MyForce<<=3;
	else if(RR1<25)MyForce<<=1;
	if(EnForces&&EnForces>MyForce){
		//karraulll!!! Nado smativatca
		int CurR=Norma(MaxForceX-MyX,MaxForceY-MyY);
		Na=CT->NDefArms;
		Amid=CT->DefArms;
		int Mindst=1000000;
		int FinTop=0xFFFF;
		for(int i=0;i<Na;i++){
			AI_Army* AIAR=CT->ARMS+Amid[i];
			int AIX=AIAR->x>>8;
			int AIY=AIAR->y>>8;
			if(AIAR->TopPos>=0&&AIAR->TopPos<NA&&(AIX!=MyX||AIY!=MyY)){
				int dst=Norma(AIX-MaxForceX,AIY-MaxForceY);
				int topdst=GetLinksDist(int(AIAR->TopPos)*NA+MyTop);
				if(topdst<Mindst&&dst>CurR){
					Mindst=topdst;
					FinTop=AIAR->TopPos;
				};
			};
		};
		if(FinTop==0xFFFF){
			FinTop=GetTopology(int(CT->CenterX<<7),int(CT->CenterY<<7));
		};
		if(FinTop!=0xFFFF){
			word NextTop=GetMotionLinks(MyTop*NA+FinTop);
			if(NextTop!=0xFFFF){
				Area* AR=GetTopMap(NextTop);
				ARM->LocalSendTo((AR->x<<6)+32,(AR->y<<6)+32,128+64,1);		
				return;
			};
		};
	}else if(DoBitva){
		ARM->Bitva();
		if(ARM->AOrder&&ARM->AOrder->ALink==&A_BitvaLink)return;
	};
	bool FastExit=false;
	if(ARM->Spec==2){
		//if(!FindNearestWall(&Dsx,&Dsy)){
		//	if(!FindNearestEnemy(0,&Dsx,&Dsy,true))FastExit=true;
		//};
		word FTop=GetNextGreCell(ARM->NI,ARM->NExBrigs*16,MyTop);
		if(FTop!=0xFFFF){
			Area* AR=GetTopMap(FTop);
			Dsx=AR->x<<6;
			Dsy=AR->y<<6;
		}else{
			if(!FindNearestWall(&Dsx,&Dsy,CT->NI)){
				if(!GNFO.FindNearestEnemy(ARM->CT->NI,&Dsx,&Dsy,true,30,ARM->CT->DefenceStage))FastExit=true;
			};
		};
	}else{
		if(ARM->Spec==1){
			word FTop=GetNextDivCell(ARM->NI,ARM->NExBrigs*16,MyTop);
			if(FTop!=0xFFFF){
				Area* AR=GetTopMap(FTop);
				Dsx=AR->x<<6;
				Dsy=AR->y<<6;
			}else{
				if(!GNFO.FindNearestEnemy(ARM->CT->NI,&Dsx,&Dsy,true,30,ARM->CT->DefenceStage)){
					if(!FindNearestWall(&Dsx,&Dsy,CT->NI)) FastExit=true;
				};
			};
		}else{
			if(!GNFO.FindNearestEnemy(ARM->CT->NI,&Dsx,&Dsy,true,30,ARM->CT->DefenceStage)){
				if(!FindNearestWall(&Dsx,&Dsy,CT->NI)) FastExit=true;
			};
		};
	};
	if(FastExit){
		Area* AR=GetTopMap(MyTop);
		int tpx=(int(AR->x)<<6)+32;
		int tpy=(int(AR->y)<<6)+32;
		if(ARM->GetArmyDanger(tpx,tpy)>2){
			ARM->SendToMostSafePosition();
		};
		return;
	};
	word Top=GetTopology(Dsx,Dsy);
	if(Top==0xFFFF){
		//BR->DeleteBOrder();
		return;
	};
	word Top1=ARM->TopPos;
	if(Top1==0xFFFF){
		ARM->DeleteAOrder();
		return;
	};
	word NextTop=GetMotionLinks(Top1*NA+Top);
	if(NextTop==0xFFFF){
		Area* AR=GetTopMap(MyTop);
		int tpx=(int(AR->x)<<6)+32;
		int tpy=(int(AR->y)<<6)+32;
		if(ARM->GetArmyDanger(tpx,tpy)>2){
			ARM->SendToMostSafePosition();
		};
		return;
	};
	if(GetTopDanger(NextTop,Endst,ARM->CT->NI)>MyForce){
		if(NearestGreatArmyTop!=-1){
			if(NextTop!=NearestGreatArmyTop){
				NextTop=GetMotionLinks(MTPM+NearestGreatArmyTop);
			};
		}else return;
	};
	Area* AR=GetTopMap(NextTop);
	Area* MAR=GetTopMap(MyTop);
	int tpx=(int(MAR->x)<<6)+32;
	int tpy=(int(MAR->y)<<6)+32;
	if(ARM->GetArmyDanger(tpx,tpy)>1){
		ARM->SendToMostSafePosition();
		return;
	};
	tpx=(int(AR->x)<<6)+32;
	tpy=(int(AR->y)<<6)+32;
	if(ARM->GetArmyDanger(tpx,tpy)>2){
		if(rando()<4096)ARM->Parad();
		for(int n=0;n<ARM->NExBrigs;n++){
			if(ARM->ExBrigs[n].BrigadeType==5)return;
		};
		//ARM->SendToMostSafePosition();
		return;
	};
	if(NextTop==Top){
		//OR1->Final=true;
		ARM->LocalSendTo(Dsx,Dsy,128,1);
	}else{
		ARM->LocalSendTo((AR->x<<6)+32,(AR->y<<6)+32,128+64,1);
	};
};
void ArmyMakeDiversiaLink(AI_Army* ARM){
	rando();
	ARM->SendGrenaders();
	Army_BTL* OR1=(Army_BTL*)ARM->AOrder;
	if((tmtmt-OR1->time)<64)return;
	OR1->time=tmtmt;
	if(!ARM->NExBrigs){
		ARM->ClearArmy();
		ARM->Enabled=false;
		return;
	};
	if(ARM->TopPos==-1){
		ARM->Parad();
		return;
	}else ARM->Parad();
	
	int Dsx=int(ARM->x);
	int Dsy=int(ARM->y);
	//check near army to connect
	City* CT=ARM->CT;
	int Na=CT->NDefArms;
	word *Amid=CT->DefArms;
	int MyTop=ARM->TopPos;
	int HisTop=0xFFFF;
	int HisMinDis=0xFFFF;
	int NA=GetNAreas();
	if(MyTop!=-1){		
		for(int i=0;i<Na;i++){
			AI_Army* AIAR=CT->ARMS+Amid[i];
			int Atop=AIAR->TopPos;
			if((ARM->Spec==AIAR->Spec)&&(!AIAR->SpecialOrder)&&Atop>=0&&Atop<NA&&AIAR!=ARM){
				if(Atop==MyTop||GetMotionLinks(MyTop*NA+Atop)==Atop){
					//Can connect now
					AIAR->ClearAOrders();
					int N=AIAR->NExBrigs;
					for(int j=0;j<N;j++){
						Brigade* BR=AIAR->ExBrigs[j].Brig;
						ARM->AddBrigade(BR);
						if(BR->NMemb){
							BR->RemoveObjects(BR->NMemb,&ARM->CT->Defenders);
						};
						BR->DeleteAll();
						BR->Enabled=true;
					};
					AIAR->ClearArmy();
					AIAR->Enabled=false;
					if(i<Na-1){
						memcpy(Amid+i,Amid+i+1,(Na-i-1)<<1);
					};
					CT->NDefArms--;
					return;
				}else{
					int R=GetLinksDist(MyTop*NA+Atop);
					if(R<HisMinDis){
						HisMinDis=R;
						HisTop=Atop;
					};
				};
			};
		};
	};
	ArmyInfo* AINF=GNFO.EINF[ARM->CT->NI]->GAINF.AINF;
	Na=GNFO.EINF[ARM->CT->NI]->GAINF.NArmy;
	int MyX=(ARM->x)>>8;
	int MyY=(ARM->y)>>8;
	int DoBitva=false;
	int rmin=4;
	int Endst=7;
	if(ARM->Spec==2)Endst=3;
	if(ARM->Spec==2)rmin=2;
	for(int i=0;i<Na;i++){
		int rr=Norma(int(AINF->MaxX)-MyX,int(AINF->MaxY)-MyY);
		int rr1=Norma(int(AINF->MaxX)-MyX,int(AINF->MinY)-MyY);
		if(rr1<rr)rr=rr1;
		rr1=Norma(int(AINF->MinX)-MyX,int(AINF->MaxY)-MyY);
		if(rr1<rr)rr=rr1;
		rr1=Norma(int(AINF->MinX)-MyX,int(AINF->MinY)-MyY);
		if(rr1<rr)rr=rr1;
		AINF++;
		if(rr<rmin)DoBitva=true;
	};
	if(DoBitva){
		ARM->Bitva();
		if(ARM->AOrder&&ARM->AOrder->ALink==&A_BitvaLink)return;
	};
	bool FastExit=false;
	if(HisTop==0xFFFF){
		if(ARM->Spec==2){
			if(!FindNearestWall(&Dsx,&Dsy,CT->NI)){
				if(!GNFO.FindNearestEnemy(ARM->CT->NI,&Dsx,&Dsy,true,0,0))FastExit=true;
			};
		}else{
			if(!GNFO.FindNearestEnemy(ARM->CT->NI,&Dsx,&Dsy,true,0,0)){
				if(!FindNearestWall(&Dsx,&Dsy,CT->NI)) FastExit=true;
			};
		};
	};
	if(FastExit){
		Area* AR=GetTopMap(MyTop);
		int tpx=(int(AR->x)<<6)+32;
		int tpy=(int(AR->y)<<6)+32;
		if(ARM->GetArmyDanger(tpx,tpy)>2){
			ARM->SendToMostSafePosition();
		};
		return;
	};
	word Top;
	if(HisTop==0xFFFF)Top=GetTopology(Dsx,Dsy);
	else Top=HisTop;

	if(Top==0xFFFF){
		//BR->DeleteBOrder();
		return;
	};
	word Top1=ARM->TopPos;
	if(Top1==0xFFFF){
		ARM->DeleteAOrder();
		return;
	};
	word NextTop=GetMotionLinks(Top1*NA+Top);
	if(NextTop==0xFFFF||NextTop==MyTop){
		Area* AR=GetTopMap(MyTop);
		int tpx=(int(AR->x)<<6)+32;
		int tpy=(int(AR->y)<<6)+32;
		if(ARM->GetArmyDanger(tpx,tpy)>2){
			ARM->SendToMostSafePosition();
		};
		//-CheckArmies(CT);
		return;
	};
	//if(GetTopDanger(NextTop,Endst)>MyForce)return;
	Area* AR=GetTopMap(NextTop);
	Area* MAR=GetTopMap(MyTop);
	int tpx=(int(MAR->x)<<6)+32;
	int tpy=(int(MAR->y)<<6)+32;
	//if(ARM->GetArmyDanger(tpx,tpy)>1){
	//	ARM->SendToMostSafePosition();
		//-CheckArmies(CT);
	//	return;
	//};
	tpx=(int(AR->x)<<6)+32;
	tpy=(int(AR->y)<<6)+32;
	//if(ARM->GetArmyDanger(tpx,tpy)>2){
	//	if(rando()<4096)ARM->Parad();
	//	for(int n=0;n<ARM->NExBrigs;n++){
	//		if(ARM->ExBrigs[n].BrigadeType==5)return;
	//	};
		//ARM->SendToMostSafePosition();
//		return;
//	};
	if(NextTop==Top){
		//OR1->Final=true;
		ARM->LocalSendTo(Dsx,Dsy,128,1);
	}else{
		ARM->LocalSendTo((AR->x<<6)+32,(AR->y<<6)+32,128+64,1);
	};
};
char* ABTL_Mess="[AI_Army::Make Battle]";
void AI_Army::MakeBattle(){
	Army_BTL* OR1=(Army_BTL*)CreateOrder(0,sizeof(Army_BTL));
	OR1->Message=ABTL_Mess;
	OR1->time=-10000;
	OR1->xdest=-1;
	OR1->ydest=-1;
	OR1->Size=sizeof(Army_BTL);
	OR1->ALink=&ArmyMakeBattleLink;
	return;	
};
int  AI_Army::GetAmountOfBrigs(byte Type){
	int N=0;
	for(int i=0;i<NExBrigs;i++){
		if(ExBrigs[i].BrigadeType==Type)N++;
	};
	addrand(N);
	return N;
};
void SHIP_DIVERSIA(AI_Army* ARM){
	rando();
	Army_BTL* OR1=(Army_BTL*)ARM->CreateOrder(0,sizeof(Army_BTL));
	OR1->Message=ABTL_Mess;
	OR1->time=-10000;
	OR1->xdest=-1;
	OR1->ydest=-1;
	OR1->Size=sizeof(Army_BTL);
	OR1->ALink=&ArmyMakeDiversiaLink;
	return;	
};
bool FastSearchGreAttObject(int* x,int* y,int r,byte NI);
void AI_Army::SendGrenaders(){
	addrand(x);
	addrand(y);
	int x0=x>>6;
	int y0=y>>6;
	if(GetAmountOfBrigs(6)&&FastSearchGreAttObject(&x0,&y0,35,CT->NI)){
		addrand(NExBrigs);
		for(int i=0;i<NExBrigs;i++){
			if(ExBrigs[i].BrigadeType==6){
				addrand(77);
				Brigade* BR=ExBrigs[i].Brig;
				int N=BR->NMemb;
				word* Mem=BR->Memb;
				word* MSN=BR->MembSN;
				for(int j=0;j<N;j++){
					word MID=Mem[j];
					if(MID!=0xFFFF){
						OneObject* OB=Group[MID];
						if(OB&&OB->Serial==MSN[j]){
							GrenaderSuperman(OB);
						};
					};
				};
			};
		};
	};
};
//-------------------------------------Bitva-------------------------------------//
char* ABIT_Message="[Army::Bitva]";
//#define maxen
class Army_Bitva:public ArmyOrder{
public:
	byte BitMask[1024];
	word Enm[512];
	word EnSN[512];
	byte NDang[512];
	int  NEn;
	int MinX;
	int MinY;
	int MaxX;
	int MaxY;
	void AddEnemXY(int x,int y,int MyTop,byte msk);
};
word GetTopFast(int x,int y){
	if(x>=0&&y>=0&&x<TopLx&&y<TopLy){
		return GetTopRef(x+(y<<TopSH));
	}else return 0xFFFF;
};
void Army_Bitva::AddEnemXY(int x,int y,int MyTop,byte msk){
	if(NEn>=512||MyTop<0||MyTop>=GetNAreas())return;
	int x0=x<<1;
	int y0=y<<1;
	
	int HimTop1=GetTopFast(x0,y0);
	int HimTop2=GetTopFast(x0+1,y0);
	int HimTop3=GetTopFast(x0,y0+1);
	int HimTop4=GetTopFast(x0+1,y0+1);
	if(HimTop1>=0xFFFE&&HimTop2>=0xFFFE&&HimTop3>=0xFFFE&&HimTop4>=0xFFFE)return;
	int NTP=MyTop*GetNAreas();
	if(HimTop1<0xFFFE){
		if(HimTop1!=MyTop&&GetLinksDist(HimTop1+NTP)>50)return;
	};
	if(HimTop2<0xFFFE){
		if(HimTop2!=MyTop&&GetLinksDist(HimTop2+NTP)>50)return;
	};
	if(HimTop3<0xFFFE){
		if(HimTop3!=MyTop&&GetLinksDist(HimTop3+NTP)>50)return;
	};
	if(HimTop4<0xFFFE){
		if(HimTop4!=MyTop&&GetLinksDist(HimTop4+NTP)>50)return;
	};
	int cell=x+1+((y+1)<<VAL_SHFCX);
	if(cell>0&&cell<MAXCIOFS){
		int NMon=MCount[cell];
		if(!NMon)return;
		int ofs1=cell<<SHFCELL;
		word MID;
		for(int i=0;i<NMon;i++){
			MID=GetNMSL(ofs1+i);
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&!(OB->NMask&msk||OB->Sdoxlo||OB->LockType)){
					int id=OB->Index;
					int ms=1<<(id&7);
					int of=id>>3;
					if(!(BitMask[of]&ms)){
						BitMask[of]|=ms;
						Enm[NEn]=id;
						EnSN[NEn]=OB->Serial;
						int xx=OB->RealX>>10;
						int yy=OB->RealY>>10;
						if(xx>0&&yy>0){
							int off=xx+(yy<<TopSH);
							NDang[NEn]=0;//byte(((InflMap[off-1]&255)+(InflMap[off+1]&255)+(InflMap[off+TopLx]&255)+(InflMap[off-TopLx]&255))>>2);
						}else NDang[NEn]=0;
						NEn++;
						if(NEn>=512)return;
					};
				};
			};
		};
	};
};
word GetDir(int dx,int dy);
void AttackObjLink(OneObject* OBJ);
void A_BitvaLink(AI_Army* ARM){
	rando();
	Army_Bitva* OR1=(Army_Bitva*)ARM->AOrder;
	bool ARCHOBJ=HandleArchers(ARM,10000000);
	//1.Check range of battle
	if(rando()<8192){
		int MinX=10000000;
		int MinY=10000000;
		int MaxX=0;
		int MaxY=0;	

		for(int i=0;i<ARM->NExBrigs;i++){
			Brigade* BR=ARM->ExBrigs[i].Brig;
			int N=BR->NMemb;
			word* Mem=BR->Memb;
			word* MSN=BR->MembSN;
			for(int j=0;j<N;j++){
				word MID=Mem[j];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&OB->Serial==MSN[j]){
						int xx=OB->RealX>>11;
						int yy=OB->RealY>>11;
						if(xx<MinX)MinX=xx;
						if(yy<MinY)MinY=yy;
						if(xx>MaxX)MaxX=xx;
						if(yy>MaxY)MaxY=yy;
					};
				};
			};
		};
		if(MaxX>=MinX){
			MinX-=6;
			MaxX+=6;
			MinY-=6;
			MaxY+=6;
			if(MinX<0)MinX=0;
			if(MinY<0)MinY=0;
			OR1->MinX=MinX;
			OR1->MaxX=MaxX;
			OR1->MinY=MinY;
			OR1->MaxY=MaxY;
		}else{
			ARM->DeleteAOrder();
			return;
		};
	};
	//2.renew enemy list
	if(rando()<2048){
		int N=OR1->NEn;
		word* Mem=OR1->Enm;
		word* MSN=OR1->EnSN;
		byte* ATT=OR1->NDang;
		for(int i=0;i<N;i++){
			OneObject* OB=Group[Mem[i]];
			if((!OB)||(OB&&(OB->Sdoxlo||OB->Serial!=MSN[i]))){
				//delete enemy from list
				if(i<N-1){
					memcpy(Mem+i,Mem+i+1,(N-i-1)<<1);
					memcpy(MSN+i,MSN+i+1,(N-i-1)<<1);
					memcpy(ATT+i,ATT+i+1,N-i-1);
				};
				i--;
				N--;
			};
		};
		OR1->NEn=N;
	};
	int MaxAttServ=64;
	bool InBattle=false;
	//3.Adding new units to battle
	int MinX=OR1->MinX;
	int MinY=OR1->MinY;
	int Dx=OR1->MaxX-OR1->MinX+1;
	int Dy=OR1->MaxY-OR1->MinY+1;
	byte msk=1<<ARM->CT->NI;
	for(int p=0;p<25;p++){
		int xx=((int(rando())*Dx)>>15)+MinX;
		int yy=((int(rando())*Dy)>>15)+MinY;
		OR1->AddEnemXY(xx,yy,ARM->TopPos,msk);
	};
	//4.Attack service
	bool MorPresent=false;
	for(int i=0;i<ARM->NExBrigs;i++){
		Brigade* BR=ARM->ExBrigs[i].Brig;
		int N=BR->NMemb;
		word* Mem=BR->Memb;
		word* MSN=BR->MembSN;
		for(int j=0;j<N;j++){
			word MID=Mem[j];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==MSN[j]&&!(OB->newMons->Artilery||OB->newMons->Archer)){
					if((!(OB->LocalOrder))||(OB->EnemyID==0xFFFF)){
						//need to find enemy
						NewMonster* NM=OB->newMons;
						AdvCharacter* ADC=OB->MoreCharacter;
						byte mms=NM->KillMask;
						int MinR=ADC->MinR_Attack;
						int MaxR=ADC->MaxR_Attack;
						int myx=OB->RealX;
						int myy=OB->RealY;
						if(MaxR){
							//search best enemy
							int NearDist=1000000;
							int ReadyDist=1000000;
							word NearMID=0xFFFF;
							word ReadyMID=0xFFFF;
							int NEn=OR1->NEn;
							word* Mem=OR1->Enm;
							word* MSN=OR1->EnSN;
							byte* DANG=OR1->NDang;
							char mdr=OB->RealDir;
							for(int t=0;t<NEn;t++){
								word MID=Mem[t];
								OneObject* EOB=Group[MID];
								int dan=DANG[t];
								NewMonster* ENM=NULL;
								if(EOB)ENM=EOB->newMons;
								if(EOB&&(ENM->MathMask&mms)&&(!EOB->Sdoxlo)&&EOB->Serial==MSN[t]){
									int Eusage=ENM->Usage;
									if(Eusage==MortiraID)MorPresent=true;
									//nash paren!
									int R=Norma(myx-EOB->RealX,myy-EOB->RealY)>>4;
									if(R>MinR){
										if(R<NearDist/*&&(dan<7||R<MaxR||MorPresent)*/){
											//checking for direction of motion
											if(EOB->InMotion){
												int dr1=GetDir(EOB->RealX-myx,EOB->RealY-myy);
												char dr=EOB->RealDir-dr1;
												if(abs(dr)>64){
													NearMID=MID;
													NearDist=R;
												};
											}else{
												NearMID=MID;
												NearDist=R;
											};
										};
										if(R<MaxR&&R<ReadyDist){
											ReadyMID=MID;
											ReadyDist=R;
										};
									};
								};
							};
							if(ReadyMID==0xFFFF)ReadyMID=NearMID;
							if(ReadyMID!=0xFFFF){
								OB->AttackObj(ReadyMID,128+64);
								if(OB->LocalOrder&&
									OB->LocalOrder->DoLink==&AttackObjLink){
										OB->LocalOrder->DoLink(OB);
										if(OB->LocalOrder&&
											OB->LocalOrder->DoLink==&AttackObjLink)
											InBattle=true;
								};
							}else{
								/*
								int xxx=OB->RealX>>10;
								int yyy=OB->RealY>>10;
								if(xxx>0&&yyy>0){
									int MyDang=InflMap[xxx+(yyy<<TopSH)]&255;
									if(MyDang){
										
									};
								};
								*/
							};
						};
					}else 
						InBattle=true;
				};
			};
		};
	};
	if(!(InBattle||ARCHOBJ)){
		ARM->DeleteAOrder();
	};
};
void UpdateAttackR(AdvCharacter* ADC){
	int MinR=10000;
	int MaxR=0;
	int MaxD=0;
	for(int p=0;p<NAttTypes;p++){
		int Dam=ADC->MaxDamage[p];
		if(Dam>MaxD)MaxD=Dam;
		int R2=ADC->AttackRadius2[p];
		if(R2){
			int R1=ADC->AttackRadius1[p];
			if(R1<MinR)MinR=R1;
			if(R2>MaxR)MaxR=R2;
		};
	};
	if(MaxR<MinR){
		MaxR=0;
		MinR=0;
	};
	ADC->MinR_Attack=MinR;
	ADC->MaxR_Attack=MaxR;
	ADC->MaxDam=MaxD;
};
void AI_Army::Bitva(){
	if(SpecialOrder)return;
	if(tmtmt-LastBitvaTime<64||(AOrder&&AOrder->ALink==&A_BitvaLink))return;
	LastBitvaTime=tmtmt;
	Army_Bitva* OR1=(Army_Bitva*)CreateOrder(1,sizeof Army_Bitva);
	OR1->Message=ABIT_Message;
	OR1->Size=sizeof Army_Bitva;
	memset(OR1->BitMask,0,sizeof OR1->BitMask);
	OR1->NEn=0;

	int MinX=10000000;
	int MinY=10000000;
	int MaxX=0;
	int MaxY=0;

	for(int i=0;i<NExBrigs;i++){
		Brigade* BR=ExBrigs[i].Brig;
		int N=BR->NMemb;
		word* Mem=BR->Memb;
		word* MSN=BR->MembSN;
		for(int j=0;j<N;j++){
			word MID=Mem[j];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==MSN[j]){
					int xx=OB->RealX>>11;
					int yy=OB->RealY>>11;
					if(xx<MinX)MinX=xx;
					if(yy<MinY)MinY=yy;
					if(xx>MaxX)MaxX=xx;
					if(yy>MaxY)MaxY=yy;
				};
			};
		};
	};
	byte msk=1<<CT->NI;
	if(MaxX>=MinX){
		MinX-=6;
		MaxX+=6;
		MinY-=6;
		MaxY+=6;
		if(MinX<0)MinX=0;
		if(MinY<0)MinY=0;
		for(int ix=MinX;ix<=MaxX;ix++){
			for(int iy=MinY;iy<=MaxY;iy++){
				OR1->AddEnemXY(ix,iy,TopPos,msk);
			};
		};
		OR1->MinX=MinX;
		OR1->MaxX=MaxX;
		OR1->MinY=MinY;
		OR1->MaxY=MaxY;
		OR1->ALink=&A_BitvaLink;
	}else{
		DeleteAOrder();
		return;
	};
	A_BitvaLink(this);
};
//----------------------------------DIVERSION GROUP------------------------------//
//Problems of diversants:
//1.Erase mines of enemy
//2.Erase small groups of units
//3.Erase any capturable objects
int FindStrongestUnitInCell(int cell,int* MinStrength,byte NAT){
	cell+=VAL_MAXCX+1;
	int strength=*MinStrength;
    int NMon=MCount[cell];
	if(!NMon)return 0xFFFF;
	int ofs1=cell<<SHFCELL;
	word MID;
	word BestMID=0xFFFF;
	int N=0;
	for(int i=0;i<NMon;i++){
		MID=GetNMSL(ofs1+i);
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&(!OB->Sdoxlo)&&OB->NNUM==NAT){
				int Dam=OB->MoreCharacter->MaxDam;
				if(strength<=Dam){
					strength=Dam;
					BestMID=MID;
				};
			};
		};
	};
	*MinStrength=strength;
    return BestMID;
};
int FindStrongestUnit(int x,int y,byte NAT){
	int cell=(y<<(VAL_SHFCX+1))+x+x;
	word MID=0xFFFF;
	if(cell>=0){
		int Stren=0;
		int MID1=FindStrongestUnitInCell(cell,&Stren,NAT);
		if(MID1!=0xFFFF)MID=MID1;
		MID1=FindStrongestUnitInCell(cell+1,&Stren,NAT);
		if(MID1!=0xFFFF)MID=MID1;
		MID1=FindStrongestUnitInCell(cell+VAL_MAXCX,&Stren,NAT);
		if(MID1!=0xFFFF)MID=MID1;
		MID1=FindStrongestUnitInCell(cell+VAL_MAXCX+1,&Stren,NAT);
		if(MID1!=0xFFFF)MID=MID1;
	};
	return MID;
};
char* ADIV_Message="[AI_Army::Diversion]";
class DiversionOrder:public ArmyOrder{
public:
	word EnMID;
	byte MotionMethod;
	word NextTop;
	word Prev1Top;
	word Prev2Top;
	word Prev3Top;
	int  LastEnemyTime;
};
int GetAmountOfProtectors(OneObject* OBJ);
word GetNearestDefender(OneObject* OB);
word GetNextSafeCell(byte NI,int F,int start,int Fin);
int LastRND=0;
void A_DiversiaLink(AI_Army* ARM){

};
void AI_Army::Diversia(){
	DiversionOrder* OR1=(DiversionOrder*)CreateOrder(0,sizeof DiversionOrder);
	OR1->ALink=&A_DiversiaLink;
	OR1->EnMID=0xFFFF;
	OR1->LastEnemyTime=tmtmt;
	OR1->Message=ADIV_Message;
	OR1->Size=sizeof DiversionOrder;
	OR1->MotionMethod=0;
	SpecialOrder=true;
};

byte MaxDiver[18]={5,8,12,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15};
void CheckDiversants(City* CT){
	if(!CT->Difficulty)return;
	Brigade* AGRESS=&CT->Agressors;
	if(CT->MyIsland!=CITY[0].MyIsland)AGRESS=&CT->Settlers;
	int NDivers=0;
	int N=CT->Agressors.NMemb;
	word* Mem=CT->Agressors.Memb;
	word* MSN=CT->Agressors.MembSN;
	for(int i=0;i<N;i++){
		OneObject* OB=Group[Mem[i]];
		if(OB&&OB->Serial==MSN[i]){
			if(OB->newMons->Usage==FastHorseID)NDivers++;
		};
	};
	int Maxdvr=MaxDiver[CT->NDivr];
	if(NDivers>=Maxdvr&&CT->NDivr<16){
		int NDivArms=0;
		for(int i=0;i<MaxArm;i++){
			AI_Army* AIAR=CT->ARMS+i;
			if(AIAR->Enabled){
				if(AIAR->AOrder&&AIAR->AOrder->ALink==&A_DiversiaLink)NDivArms++;
			};
		};
		if(NDivArms<2&&CT->NDivr<6){
			//need to make diversion
			//if(NDivers>Maxd6)NDivers=6;
			//2
			int old=BriMax[2];
			BriMax[2]=NDivers;
			int aid=CT->GetFreeArmy();
			if(aid!=-1){
				AI_Army* AIAR=CT->ARMS+aid;
				AIAR->Enabled=true;
				AIAR->CreateMinimalArmyFromBrigade(&CT->Agressors,2);
				AIAR->Diversia();
				AIAR->Spec=5;
				CT->NDivr++;
			};
			BriMax[2]=old;
		};
	};
};
//------------------------------MAIN STRATEGICAL BRAIN---------------------------//
void SHIP_DIVERSIA(AI_Army* ARM);
void SmartGamer(City* CT){
#ifndef CONQUEST
	//minimal armies creation from Agressors
	//assert(CT->NI==1);
	if(CT->AI_LAND_BATTLE){
		int AType=CheckMinArmyCreationAbility(&CT->Agressors);
		if(AType!=-1){
			int Aind=CT->GetFreeArmy();
			if(Aind!=-1){
				if(CT->NDefArms>=CT->MaxDefArms){
					CT->MaxDefArms+=64;
					CT->DefArms=(word*)realloc(CT->DefArms,CT->MaxDefArms<<1);
				};
				CT->DefArms[CT->NDefArms]=Aind;	
				CT->NDefArms++;
				AI_Army* ARM=CT->ARMS+Aind;
				ARM->Enabled=true;
				ARM->CreateMinimalArmyFromBrigade(&CT->Agressors,AType);
				ARM->Parad();
				//goto some mine
				//int N=NtNUnits[i];
				//word* units=
				ARM->MakeBattle();
				//ARM->LocalSendTo((int(ARM->x))+(rando()&1024)-512,(int(ARM->y))+(rando()&1023)-512,0,0);
			};
		};
		//--------------Settlers---------------
		AType=CheckSuperMinArmyCreationAbility(&CT->Settlers);
		if(AType!=-1){
			int Aind=CT->GetFreeArmy();
			if(Aind!=-1){
				if(CT->NDefArms>=CT->MaxDefArms){
					CT->MaxDefArms+=64;
					CT->DefArms=(word*)realloc(CT->DefArms,CT->MaxDefArms<<1);
				};
				CT->DefArms[CT->NDefArms]=Aind;
				CT->NDefArms++;
				AI_Army* ARM=CT->ARMS+Aind;
				ARM->Enabled=true;
				ARM->CreateMinimalArmyFromBrigade(&CT->Settlers,AType);
				ARM->Parad();
				//goto some mine
				//int N=NtNUnits[i];
				//word* units=
				SHIP_DIVERSIA(ARM);
				//ARM->LocalSendTo((int(ARM->x))+(rando()&1024)-512,(int(ARM->y))+(rando()&1023)-512,0,0);
			};
		};
	};
	//-------------------------------------
	if(CT->AI_FAST_DIVERSION)CheckDiversants(CT);
	if(CT->AI_TOWN_DEFENCE){
		//check base safety
		Nation* Nat=CT->Nat;
		int NNUM=Nat->NNUM;
		int N=NtNUnits[NNUM];
		int NArm=GNFO.EINF[NNUM]->GAINF.NArmy;
		ArmyInfo* AIN=GNFO.EINF[NNUM]->GAINF.AINF;
		word* units=NatList[NNUM];
		int NDanger=0;
		byte UseFlags[512];
		memset(UseFlags,0,sizeof UseFlags);
		word Danger[128];
		if(N){
		for(int i=0;i<N;i++){
				word MID=units[i];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&OB->NewBuilding){
						int xx=OB->RealX>>12;
						int yy=OB->RealY>>12;
						for(int k=0;k<NArm;k++){
							if(!UseFlags[k]){
								ArmyInfo* ARM=AIN+k;
								if(Norma(ARM->MinX-xx,ARM->MinY-yy)<10||
								   Norma(ARM->MinX-xx,ARM->MaxY-yy)<10||
								   Norma(ARM->MaxX-xx,ARM->MinY-yy)<10||
								   Norma(ARM->MaxX-xx,ARM->MaxY-yy)<10){
									if(ARM->NSword+ARM->NCaval+ARM->NMort>50){
										Danger[NDanger]=k;
										NDanger++;
										UseFlags[k]=1;
									};
								};
							};
						};
					};
				};
			};
			if(NDanger){
				//need to protect base;
				CT->FreeArmy.RemoveObjects(CT->FreeArmy.NMemb,&CT->Defenders);
			};
		};
	};
	if(rando()<1024){
		//determination of walls to erase

	};
#endif
};
void AI_Army::ClearZombi(){
	for(int i=0;i<NExBrigs;i++){
		Brigade* BR=ExBrigs[i].Brig;
		word* Mem=BR->Memb;
		word* MSN=BR->MembSN;
		int N=BR->NMemb;
		for(int j=0;j<N;j++){
			word MID=Mem[j];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB)OB->Zombi=false;
			};
		};
	};
};
void AI_Army::SetZombi(){
	for(int i=0;i<NExBrigs;i++){
		Brigade* BR=ExBrigs[i].Brig;
		word* Mem=BR->Memb;
		word* MSN=BR->MembSN;
		int N=BR->NMemb;
		for(int j=0;j<N;j++){
			word MID=Mem[j];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB)OB->Zombi=true;
			};
		};
	};
};


//-------------------Processing the danger map------------------//

//--------------------------------------------------------------//



int CheckPositionForDanger(int x,int y,int z){
	EnemyInfo* EIN=GNFO.EINF[CURRENTAINATION];
	DangerInfo* DIF=EIN->DINF;
	int MAXR=DIF->MaxR+128;
	for(int i=0;i<EIN->NDINF;i++){
		int dst=Norma(DIF->x-x,DIF->y-y)+((z-DIF->z)<<1);
		if(dst<=MAXR)return DIF->UpgradeLevel;
		DIF++;
	};
	return 0;
};
byte GetDangValue(int x,int y);
int FastCheckPosDanger(int x,int y){
	if(x>0&&y>0&&x<TopLx&&y<TopLx){
		int v=0;//GetDangValue(x>>1,y>>1);
		if(v>128)v=(v&127)+5;else v=0;
		if(!GNFO.EINF[CURRENTAINATION])return 0;
		return (GNFO.EINF[CURRENTAINATION]->InflMap[x+(y<<TopSH)]+v)&255;
	}else return 0;
};
int AI_Army::GetArmyDanger(int xp,int yp){
	int MaxDang=0;
	int x=xp>>6;
	int y=yp>>6;
	int odis=4;
	int Nx=int(sqrt(NExBrigs));
	int Ny=Nx;
	if(Nx*Ny<NExBrigs)Nx++;
	if(Nx*Ny<NExBrigs)Ny++;
	int x0=x-(((Nx-1)*odis)>>1);
	int y0=y-(((Ny-1)*odis)>>1);
	int ps=0;
	int nn=0;
	int Dang=0;
	for(int ix=0;ix<Nx;ix++){
		for(int iy=0;iy<Ny;iy++){
			if(nn<NExBrigs){
				int x1=x0+ix*odis;
				int y1=y0+iy*odis;
				CheckTopPos(x,y,&x1,&y1);
				//int xx=(x1<<6)+32;
				//int yy=(y1<<6)+32;
				int Dang=FastCheckPosDanger(x1-2,y1-2);
				if(Dang>MaxDang)MaxDang=Dang;
				Dang=FastCheckPosDanger(x1-2,y1+1);
				if(Dang>MaxDang)MaxDang=Dang;
				Dang=FastCheckPosDanger(x1+2,y1-2);
				if(Dang>MaxDang)MaxDang=Dang;
				Dang=FastCheckPosDanger(x1+2,y1+2);
				if(Dang>MaxDang)MaxDang=Dang;
				ps+=5;
				nn++;
			};
		};
	};
	return MaxDang;
};
void AI_Army::SendToMostSafePosition(){
	if(TopPos==-1||TopPos>=0xFFFE)return;
	Area* AR=GetTopMap(TopPos);
	int MinDang=100000;
	int xx,yy;
	int N=AR->NLinks;
	for(int j=0;j<N;j++){
		int i=(int(rando())*N)>>15;
		Area* AR1=GetTopMap(AR->Link[i].NextAreaID);
		int x=((AR1->x)<<6)+32;
		int y=((AR1->y)<<6)+32;
		int dan=GetArmyDanger(x,y);
		if(dan<=MinDang){
			xx=x;
			yy=y;
			MinDang=dan;
			if(!dan)goto Smps;
		};
	};
Smps:
	if(MinDang<100000){
		LocalSendTo(xx,yy,128+64,1);
	};
};
//-----------------------Search object for the grenader's attack----------------------//
int tdxy[]={1,0 ,0,1 ,-1,0 ,0,-1, 1,1 ,1,-1 ,-1,1 ,-1,-1};
int GetTopDistance(int xa,int ya,int xb,int yb){
	int Top1=SafeTopRef(xa,ya);
	if(Top1==0xFFFF){
		for(int i=0;i<16&&Top1==0xFFFF;i+=2)Top1=SafeTopRef(xa+tdxy[i],ya+tdxy[i+1]);
	};
	int Top2=SafeTopRef(xb,yb);
	if(Top2==0xFFFF){
		for(int i=0;i<16&&Top2==0xFFFF;i+=2)Top2=SafeTopRef(xa+tdxy[i],ya+tdxy[i+1]);
	};
	if(Top1>=0xFFFE||Top2>=0xFFFE)return 0xFFFF;
	if(Top1==Top2)return Norma(xa-xb,ya-yb);
	int NA=GetNAreas();
	int Next1=GetMotionLinks(Top1*NA+Top2);
	if(Next1==0xFFFF)return 0xFFFF;
	if(Next1==Top2)return Norma(xa-xb,ya-yb);
	int Next2=GetMotionLinks(Top2*NA+Top1);
	if(Next2==0xFFFF)return 0xFFFF;
	if(Next2==Top1)return Norma(xa-xb,ya-yb);
	Area* AR1=GetTopMap(Next1);
	Area* AR2=GetTopMap(Next2);
	if(Next1==Next2)return Norma(xa-AR1->x,ya-AR1->y)+Norma(xb-AR2->x,yb-AR2->y);
	return Norma(xa-AR1->x,ya-AR1->y)+Norma(xb-AR2->x,yb-AR2->y)+GetLinksDist(Next1*NA+Next2);
};
__forceinline word SafeTopRef(int x,int y,int LT){
	if(x>=0&&y>=0&&x<TopLx&&y<TopLx)return GetTopRef(x+(y<<TopSH),LT);
	else return 0xFFFF;
};
int GetTopDistance(int xa,int ya,int xb,int yb,int LT){
	int Top1=SafeTopRef(xa,ya,LT);
	if(Top1==0xFFFF){
		for(int i=0;i<16&&Top1==0xFFFF;i+=2)Top1=SafeTopRef(xa+tdxy[i],ya+tdxy[i+1],LT);
	};
	int Top2=SafeTopRef(xb,yb,LT);
	if(Top2==0xFFFF){
		for(int i=0;i<16&&Top2==0xFFFF;i+=2)Top2=SafeTopRef(xa+tdxy[i],ya+tdxy[i+1],LT);
	};
	if(Top1>=0xFFFE||Top2>=0xFFFE)return 0xFFFF;
	if(Top1==Top2)return Norma(xa-xb,ya-yb);
	int NA=GetNAreas(LT);
	int Next1=GetMotionLinks(Top1*NA+Top2,LT);
	if(Next1==0xFFFF)return 0xFFFF;
	if(Next1==Top2)return Norma(xa-xb,ya-yb);
	int Next2=GetMotionLinks(Top2*NA+Top1,LT);
	if(Next2==0xFFFF)return 0xFFFF;
	if(Next2==Top1)return Norma(xa-xb,ya-yb);
	Area* AR1=GetTopMap(Next1,LT);
	Area* AR2=GetTopMap(Next2,LT);
	if(Next1==Next2)return Norma(xa-AR1->x,ya-AR1->y)+Norma(xb-AR2->x,yb-AR2->y);
	return Norma(xa-AR1->x,ya-AR1->y)+Norma(xb-AR2->x,yb-AR2->y)+GetLinksDist(Next1*NA+Next2,LT);
};
int GetTopDistance(int xa,int ya,int xb,int yb,int LT,byte NI){
	int Top1=SafeTopRef(xa,ya,LT);
	if(Top1==0xFFFF){
		for(int i=0;i<16&&Top1==0xFFFF;i+=2)Top1=SafeTopRef(xa+tdxy[i],ya+tdxy[i+1],LT);
	};
	int Top2=SafeTopRef(xb,yb,LT);
	if(Top2==0xFFFF){
		for(int i=0;i<16&&Top2==0xFFFF;i+=2)Top2=SafeTopRef(xa+tdxy[i],ya+tdxy[i+1],LT);
	};
	if(Top1>=0xFFFE||Top2>=0xFFFE)return 0xFFFF;
	if(Top1==Top2)return Norma(xa-xb,ya-yb);
	int NA=GetNAreas(LT);
	int Next1=GetMotionLinks(Top1*NA+Top2,LT,NI);
	if(Next1==0xFFFF)return 0xFFFF;
	if(Next1==Top2)return Norma(xa-xb,ya-yb);
	int Next2=GetMotionLinks(Top2*NA+Top1,LT,NI);
	if(Next2==0xFFFF)return 0xFFFF;
	if(Next2==Top1)return Norma(xa-xb,ya-yb);
	Area* AR1=GetTopMap(Next1,LT);
	Area* AR2=GetTopMap(Next2,LT);
	if(Next1==Next2)return Norma(xa-AR1->x,ya-AR1->y)+Norma(xb-AR2->x,yb-AR2->y);
	return Norma(xa-AR1->x,ya-AR1->y)+Norma(xb-AR2->x,yb-AR2->y)+GetLinksDist(Next1*NA+Next2,LT,NI);
};

bool SearchGreAttObject(int* x,int* y,int r,byte NI){
	int MinTopR=10000000;
	int xb,yb;
	int x0=*x;
	int y0=*y;
	addrand(x0);
	addrand(y0);
	EnemyInfo* EIN=GNFO.EINF[NI];
	addrand(65);
	addrand(NI);
	if(!EIN)return false;
	addrand(66);
	for(int rr=0;rr<r;rr++){
		char* xi=Rarr[rr].xi;
		char* yi=Rarr[rr].yi;
		int N=Rarr[rr].N;
		for(int p=0;p<N;p++){
			int x1=x0+xi[p];
			int y1=y0+yi[p];
			SafeCellInfo* SCI=EIN->GetCellInfo(x1,y1);
			addrand(p);
			if(SCI&&SCI->BusyIndex==0xFFFF){
				addrand(SCI->BusyIndex);
				int r=GetTopDistance(x0,y0,x1,y1);
				addrand(r);
				if(r<MinTopR){
					MinTopR=r;
					xb=x1;
					yb=y1;
					addrand(x1);
					addrand(y1);
				};
			};
		};
	};
	if(MinTopR<(r<<1)){
		*x=xb;
		*y=yb;
		addrand(xb);
		addrand(yb);
		return true;
	}else return false;
};
bool FastSearchGreAttObject(int* x,int* y,int r,byte NI){
	int rr=tmtmt%r;
	int MinTopR=10000000;
	int xb,yb;
	int x0=*x;
	int y0=*y;
	char* xi=Rarr[rr].xi;
	char* yi=Rarr[rr].yi;
	int N=Rarr[rr].N;
	addrand(rr);
	EnemyInfo* EIN=GNFO.EINF[NI];
	addrand(N);
	if(!EIN)return false;
	for(int p=0;p<N;p++){
		int x1=x0+xi[p];
		int y1=y0+yi[p];
		addrand(x1);
		addrand(y1);
		SafeCellInfo* SCI=EIN->GetCellInfo(x1,y1);
		if(SCI){
			addrand(SCI->BusyIndex);
		};
		if(SCI&&SCI->BusyIndex==0xFFFF){
			int r=GetTopDistance(x0,y0,x1,y1);
			addrand(r);
			if(r<MinTopR){
				MinTopR=r;
				xb=x1;
				yb=y1;
				addrand(xb);
				addrand(yb);
			};
		};
	};
	addrand(MinTopR);
	if(MinTopR<(r<<1)){
		*x=xb;
		*y=yb;
		addrand(xb);
		addrand(yb);
		return true;
	}else return false;
};
AI_Army* GetNearestArmy(int x,int y,City* CT){
	AI_Army* AAR=CT->ARMS;
	int Abest=-1;
	int minR=100000;
	for(int i=0;i<MaxArm;i++){
		if(AAR->Enabled&&AAR->TopPos!=0xFFFF){
			int dst=Norma(x-AAR->x,y-AAR->y);
			if(dst<minR){
				minR=dst;
				Abest=i;
			};
		};
		AAR++;
	};
	if(Abest==-1)return NULL;
	else return CT->ARMS+Abest;
};
word SearchVictim(OneObject* OBJ,int R0,int R1);
void GrenaderSupermanLink(OneObject* OBJ){
	word vic=SearchVictim(OBJ,0,250);
	EnemyInfo* EIN=GNFO.EINF[OBJ->NNUM];
	if(!EIN)vic=0xFFFF;
	addrand(6);
	if(vic!=0xFFFF){
		OBJ->AttackObj(vic,254,1);
		return;
	};
	int Attx=OBJ->LocalOrder->info.AttackXY.x;
	int Atty=OBJ->LocalOrder->info.AttackXY.y;
	addrand(Attx);
	addrand(Atty);
	//int w=OBJ->LocalOrder->info.AttackXY.wep;
	int x0=OBJ->RealX>>10;
	int y0=OBJ->RealY>>10;
	/*
	if(w){
		if(OBJ->delay){
			int Top1=GetTopology(x0,y0);
			int Top2=GetTopology(Attx,Atty);
			if(Top1!=0xFFFF&&Top2!=0xFFFF){
				if(Top1==Top2||GetMotionLinks[Top1*GetNAreas()+Top2]==Top2){
					//connecting to the nearest army
					AI_Army* AAR=GetNearestArmy(
				};
			};
		};
	};
	*/
	SafeCellInfo* SCI=EIN->GetCellInfo(Attx,Atty);
	if(SCI){
		if(x0==Attx&&y0==Atty){
			//OneObject* EOB=Group[SCR->Index];
			//if(EOB&&EOB->NewBuilding&&!EOB->Sdoxlo)
				//OBJ->AttackPoint(EOB->RealX>>4,EOB->RealY>>4,EOB->RZ+32,
			OBJ->AttackObj(SCI->Index,255,1,1);
		}else{
			int xd=Attx<<2;
			int yd=Atty<<2;
			if(x0<Attx)xd+=3;
			if(y0<Atty)yd+=3;
			addrand(Attx);
			addrand(Atty);
			addrand(xd);
			addrand(yd);
			OBJ->CreatePath(xd,yd);
		};
	}else{
		addrand(76);
		if(SearchGreAttObject(&x0,&y0,40,OBJ->NNUM)){
			OBJ->LocalOrder->info.AttackXY.x=x0;
			OBJ->LocalOrder->info.AttackXY.y=y0;
			addrand(x0);
			addrand(y0);
			EIN->RegisterSafePoint(OBJ,x0,y0);
		}else{
			AI_Army* AAR=GetNearestArmy(OBJ->RealX>>4,OBJ->RealY>>4,OBJ->Nat->CITY);
			if(AAR){
				int HimTop=AAR->TopPos;
				if(HimTop!=0xFFFF){
					int MyTop=GetTopology(OBJ->RealX>>4,OBJ->RealY>>4);
					if(MyTop!=0xFFFF){
						int NextTop=HimTop;
						if(MyTop!=HimTop)NextTop=GetMotionLinks(MyTop*GetNAreas()+HimTop);
						if(NextTop!=0xFFFF){
							if(NextTop==HimTop){
								//connect
								OBJ->ClearOrders();
								City* CT=OBJ->Nat->CITY;
								int id=CT->GetFreeBrigade();
								if(id!=-1){
									Brigade* BRR=CT->Brigs+id;
									BRR->Enabled=true;
									BRR->AddObject(OBJ);
									AAR->AddBrigade(BRR);
									BRR->DeleteAll();
									BRR->Enabled=false;
								};
								return;
							}else{
								//moving to...
								Area* AR1=GetTopMap(NextTop);
								OBJ->NewMonsterSendTo(int(AR1->x)<<10,int(AR1->y)<<10,255,1);
								return;
								
							};
						};
					};
				};
			};
		};
	};
};
void GrenaderSuperman(OneObject* OB){
	addrand(87);
	if(OB->UnlimitedMotion)return;
	byte use=OB->newMons->Usage;
	if(use!=GrenaderID||use!=ArcherID)return;
	int x0=OB->RealX>>10;
	int y0=OB->RealY>>10;
	if(OB->EnemyID!=0xFFFF)return;
	EnemyInfo* EIN=GNFO.EINF[OB->NNUM];
	addrand(44);
	if(SearchGreAttObject(&x0,&y0,30,OB->NNUM)){
		if(OB->BrigadeID!=0xFFFF){
			Brigade* BR=OB->Nat->CITY->Brigs+OB->BrigadeID;
			BR->FreeMember(OB->BrIndex);
		};
		Order1* OR1=OB->CreateOrder(0);
		if(OR1){
			OR1->info.AttackXY.x=x0;
			OR1->info.AttackXY.y=y0;
			OR1->info.AttackXY.wep=0;
			OR1->info.AttackXY.ox=0xFFFF;
			OR1->info.AttackXY.oy=0xFFFF;
			OR1->PrioryLevel=255;
			addrand(x0);
			addrand(y0);
			OR1->DoLink=&GrenaderSupermanLink;
			OR1->OrderType=99;
			OR1->OrderTime=0;
			OB->PrioryLevel=254;
			EIN->RegisterSafePoint(OB,x0,y0);
		};
	};
};
//------------------------------------------------------------------------------------//
bool AI_Army::CheckArtillery(){//int Dx,int Dy,bool Battle){
	//creating normal order of brigades in army
	//search for nearest tower
	//if(rando()>4096)return false;
	return false;
	int EnmX,EnmY;
	int EnmDst=10000;
	EnemyInfo* EIN=GNFO.EINF[NI];
	for(int i=0;i<EIN->NDINF;i++){
		DangerInfo* DIN=EIN->DINF+i;
		int tx=DIN->x;
		int ty=DIN->y;
		int R=Norma(tx-int(x),ty-int(y));
		if(R<EnmDst){
			EnmDst=R;
			EnmX=tx;
			EnmY=ty;
		};
	};
	if(EnmDst<10000){
		//find nearest to the towers brigade(No artillery)
		int odis=4;
		int MinNAR=10000;
		int NABIndex=0xFFFF;
		int Nx=int(sqrt(NExBrigs));
		int Ny=Nx;
		if(Nx*Ny<NExBrigs)Nx++;
		if(Nx*Ny<NExBrigs)Ny++;
		int xt=x>>6;
		int yt=y>>6;
		int x0=xt-(((Nx-1)*odis)>>1);
		int y0=yt-(((Ny-1)*odis)>>1);
		int ps=0;
		int nn=0;
		int Dang=0;
		int MaxArtR=0;
		int ArtIndex=0xFFFF;
		for(int ix=0;ix<Nx;ix++){
			for(int iy=0;iy<Ny;iy++){
				if(nn<NExBrigs){
					int x1=((x0+ix*odis)<<6)+32;
					int y1=((y0+iy*odis)<<6)+32;
					int typ=ExBrigs[nn].BrigadeType;
					int R1=Norma(x1-EnmX,y1-EnmY);
					if(typ!=9&&typ!=5&&typ!=6){
						if(R1<MinNAR){
							MinNAR=R1;
							NABIndex=nn;
						};
					}else{
						if(R1>MaxArtR){
							MaxArtR=R1;
							ArtIndex=nn;
						};
					};
					nn++;
				}
			};
		};
		if(ArtIndex!=0xFFFF&&NABIndex!=0xFFFF&&MaxArtR>MinNAR&&ArtIndex!=NABIndex){
			Brigade* BR=ExBrigs[ArtIndex].Brig;
			ExBrigs[ArtIndex].Brig=ExBrigs[NABIndex].Brig;
			ExBrigs[NABIndex].Brig=BR;

			int xx=ExBrigs[ArtIndex].BrigadeType;
			ExBrigs[ArtIndex].BrigadeType=ExBrigs[NABIndex].BrigadeType;
			ExBrigs[NABIndex].BrigadeType=xx;
			
			LocalSendTo(x,y,128+96,1);
			return true;
		};
	};
	return false;
};
void DeleteFromGroups(byte NI,word ID){
	City* CT=CITY+NI;
	int N=CT->NGroups;
	for(int i=0;i<N;i++){
		word* GRP=CT->GroupsSet[i];
		int NG=CT->NGroupsInSet[i];
		for(int j=0;j<NG;j++){
			if(GRP[j]==ID){
				if(j<NG-1)memcpy(GRP+j,GRP+j+1,(NG-j-1)<<1);
				NG--;
				j=NG;
				CT->NGroupsInSet[i]=NG;
				if(!NG){
					free(CT->GroupsSet[i]);
					if(i<N-1)memcpy(CT->GroupsSet+i,CT->GroupsSet+i+1,(N-1-i)<<2);
					N--;
					i=N;
					CT->NGroups=N;
				};
			};
		};
	};
};
void UnGroupSelectedUnits(byte NI){
	word FormsIDS[256];
	int NForms=0;
	int N=NSL[NI];
	word* SID=Selm[NI];
	word* SNS=SerN[NI];
	City* CT=CITY+GSets.CGame.cgi_NatRefTBL[NI];
	for(int i=0;i<N;i++){
		word MID=SID[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]&&OB->BrigadeID!=0xFFFF&&!OB->Sdoxlo){
				word BID=OB->BrigadeID;
				for(int p=0;p<NForms;p++)if(FormsIDS[p]==BID)BID=0xFFFF;
				if(BID!=0xFFFF&&NForms<256){
					FormsIDS[NForms]=BID;
					NForms++;
				};
			};
		};
	};
	for(int i=0;i<NForms;i++)DeleteFromGroups(NI,FormsIDS[i]);
};
void GroupSelectedFormations(byte NI){
	word FormsIDS[256];
	int NForms=0;
	int N=NSL[NI];
	word* SID=Selm[NI];
	word* SNS=SerN[NI];
	City* CT=CITY+GSets.CGame.cgi_NatRefTBL[NI];
	for(int i=0;i<N;i++){
		word MID=SID[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]&&OB->BrigadeID!=0xFFFF&&!OB->Sdoxlo){
				word BID=OB->BrigadeID;
				for(int p=0;p<NForms;p++)if(FormsIDS[p]==BID)BID=0xFFFF;
				if(BID!=0xFFFF&&NForms<256){
					FormsIDS[NForms]=BID;
					NForms++;
				};
			};
		};
	};
	for(int i=0;i<NForms;i++)DeleteFromGroups(NI,FormsIDS[i]);
	if(NForms){
		CT->GroupsSet=(word**)realloc(CT->GroupsSet,CT->NGroups*4+4);
		CT->NGroupsInSet=(word*)realloc(CT->NGroupsInSet,CT->NGroups*2+2);
		CT->GroupsSet[CT->NGroups]=znew(word,NForms);
		memcpy(CT->GroupsSet[CT->NGroups],FormsIDS,NForms*2);
		CT->NGroupsInSet[CT->NGroups]=NForms;
		CT->NGroups++;
	};
};
void ImSelBrigade(byte NI,byte Type,byte BNAT,int ID);
void CorrectImSelectionInGroups(byte NI){
	word FormsIDS[256];
	byte FNats   [256];
	int NForms=0;
	int N=ImNSL[NI];
	word* SID=ImSelm[NI];
	word* SNS=ImSerN[NI];
	City* CT=CITY+GSets.CGame.cgi_NatRefTBL[NI];
	for(int i=0;i<N;i++){
		word MID=SID[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]&&OB->BrigadeID!=0xFFFF&&!OB->Sdoxlo){
				word BID=OB->BrigadeID;
				for(int p=0;p<NForms;p++)if(FormsIDS[p]==BID)BID=0xFFFF;
				if(BID!=0xFFFF&&NForms<256){
					FormsIDS[NForms]=BID;
					FNats[NForms]=OB->NNUM;
					NForms++;
				};
			};
		};
	};
	int N3=NForms;
	for(int i=0;i<N3;i++){
		word ID=FormsIDS[i];
		for(int j=0;j<CT->NGroups;j++){
			int N1=CT->NGroupsInSet[j];
			word* IDS=CT->GroupsSet[j];
			for(int p=0;p<N1;p++)if(IDS[p]==ID){
				for(int v=0;v<N1;v++){
					word ID1=IDS[v];
					for(int z=0;z<NForms;z++)if(FormsIDS[z]==ID1)ID1=0xFFFF;
					if(ID1!=0xFFFF&&NForms<256){
						FormsIDS[NForms]=ID1;
						NForms++;
					};
				};
			};
		};
	};
	for(int i=0;i<NForms;i++)ImSelBrigade(NI,1,FNats[i],FormsIDS[i]);
};
bool CheckGroupPossibility(byte NI){
	word* SID=ImSelm[NI];
	word* SNS=ImSerN[NI];
	int N=ImNSL[NI];
	for(int j=0;j<N;j++){
		word MID=SID[j];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[j]&&!OB->Sdoxlo){
				if(OB->BrigadeID==0xFFFF)return false;
			};
		};
	};
	return true;
};
bool CheckUnGroupPossibility(byte NI){
	word FormsIDS[256];
	int NForms=0;
	int N=ImNSL[NI];
	word* SID=ImSelm[NI];
	word* SNS=ImSerN[NI];
	City* CT=CITY+GSets.CGame.cgi_NatRefTBL[NI];
	for(int i=0;i<N;i++){
		word MID=SID[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]&&OB->BrigadeID!=0xFFFF&&!OB->Sdoxlo){
				word BID=OB->BrigadeID;
				for(int p=0;p<NForms;p++)if(FormsIDS[p]==BID)BID=0xFFFF;
				if(BID!=0xFFFF&&NForms<256){
					FormsIDS[NForms]=BID;
					NForms++;
				};
			};
		};
	};
	for(int i=0;i<CT->NGroups;i++)if(NForms==CT->NGroupsInSet[i]){
		bool FOUND=1;
		for(int j=0;j<NForms;j++){
			word ID=FormsIDS[j];
			word* CIDS=CT->GroupsSet[i];
			int N2=CT->NGroupsInSet[i];
			bool OK=false;
			for(int p=0;p<N2;p++)if(CIDS[p]==ID)OK=true;
			if(!OK)FOUND=0;
		};
		if(FOUND)return true;
	};
	return false;
};
/*
int NWallDel;
int MaxWallDel;
short
void CreateListOfWallSegmentsToDelete(){
	if(tmtmt&63!=2)return;
	int N1=WSys.NClusters;
	WallCluser** WCLS=WSys.WCL;
	for(int i=0;i<N1;i++){
		WallCluster* WCL=WCLS[i];
		if(WCL->NI==0){
			int Nc=WCL->NCells;
			WallCell* WC=WCL->Cells;
			for(int j=0;j<Nc;j++){
				if(WC->Visible){
					int x=WC->x;
					int y=WC->y;

				};
				WC++;
			};
		};
	};
};
*/
bool CheckWallForKilling(OneObject* OB){
	return false;
};
int GetBestVictimForArchers(byte NI,int x,int y,int R,int MyTop){
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
bool HandleArchers(AI_Army* ARM,int R){
	word ARCHID[512];
	int NARCH=0;
	int N=ARM->NExBrigs;
	int xc=0;
	int yc=0;
	int XCT=0;
	int YCT=0;
	int NCT=0;
	for(int i=0;i<N;i++){
		Brigade* BR=ARM->ExBrigs[i].Brig;
		int NM=BR->NMemb;
		word* MEM=BR->Memb;
		word* MSN=BR->MembSN;
		for(int v=0;v<NM;v++){
			word MID=MEM[v];
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&(!OB->Sdoxlo)){
					if(OB->newMons->Archer){
						if(NARCH<512){
							ARCHID[NARCH]=OB->Index;
							NARCH++;
							xc+=OB->RealX>>4;
							yc+=OB->RealY>>4;
							XCT+=OB->RealX>>4;
							YCT+=OB->RealY>>4;
							NCT++;
						};
					}else{
						XCT+=OB->RealX;
						YCT+=OB->RealY;
						NCT+=16;
					};
				};
			};
		};
	};
	if(NARCH){
		XCT/=NCT;
		YCT/=NCT;
		word ID=GetBestVictimForArchers(ARM->NI,XCT,YCT,R,ARM->TopPos);
		if(ID!=0xFFFF){
			for(int i=0;i<NARCH;i++){
				Group[ARCHID[i]]->AttackObj(ID,16+128,0,0);
			};
			return true;
		};
	};
	return false;
};
#ifdef COSSACKS2
extern Abilky ABILS;
Abilky::Abilky(){
	memset(this,0,sizeof *this);
};
Abilky::~Abilky(){
	if(Abils)free(Abils);
	memset(this,0,sizeof *this);
};
int Abilky::GetAbility(char* ID){
	for(int i=0;i<NAbils;i++)if(!strcmp(Abils[i].ID,ID))return i;
	return -1;
};
const char* AblList[]={
		"HEAL","HEALTIME","HEALPERCENTTIME","???","???","???","???","???",//0-7
		"???","???","???","???","???","???","???","???",//8-15
		"TIRING","TIRINGSPEED","???","???","???","???","???","???",//16-23
		"???","???","???","???","???","???","???","???",//24-31
		"ARMDAMAGE","RIFLEDAMAGE","???","???","???","???","???","???",//32-39
		"???","???","???","???","???","???","???","???",//40-47
		"SHIELD","???","???","???","???","???","???","???",//48-55
		"???","???","???","???","???","???","???","???",//56-63
		"MORALEINCSPEED","ADDMORALE","???","???","???","???","???","???",//64-71
		"???","???","???","???","???","???","???","???",//72-79
		"SPEED","???","???","???","???","???","???","???",//80-87
		""
};
bool ReadS(char** ins,char* outs){
	while((*ins)[0]<=32)(*ins)++;
	outs[0]=0;
	int z=sscanf(*ins,"%s",outs);
	if(z==0)return false;
	while((*ins)[0]>32)(*ins)++;
	while((*ins)[0]<=32)(*ins)++;
	return true;
};
bool ReadI(char** ins,int* v){
	char s[64];
	if(ReadS(ins,s)){
		*v=0;
		int z=sscanf(s,"%d",v);
		return z!=0;
	};
	return 0;	
};
void AblError(char* File,int Line){
	char s[64];
	sprintf(s,"Syntax error while reading abilities from <%s>,line %d",File,Line);
	MessageBox(hwnd,s,"Ability reading error.",0);
}
#define ABLERR AblError(File,Line);
bool ReadWinString(GFILE* F,char* STR,int Max);
int GetIconByName(char* Name);
int GetResID(char* Name);
void Abilky::Load(char* File){
	GFILE* F=Gopen(File,"r");
	int Line=0;
	bool RF=0;
	if(F){
		do{
			char S[512];
			RF=ReadWinString(F,S,511);
			Line++;
			if(S[0]!='/'&&S[0]){
				char* curs=S;
				if(NAbils>=MaxAbils){
					MaxAbils+=64;
					Abils=(OneBrigAbility*)realloc(Abils,MaxAbils*sizeof OneBrigAbility);
				};
				OneBrigAbility* ABL=Abils+NAbils;
				memset(ABL,0,sizeof OneBrigAbility);
				NAbils++;
				char sp[64];
				bool done=0;
				int v;
				if(ReadS(&curs,sp)){
					ABL->ID=znew(char,strlen(sp)+1);
					strcpy(ABL->ID,sp);
					int v1,v2,v3;
					if(ReadI(&curs,&v3)&&ReadI(&curs,&v1)&&ReadI(&curs,&v2)){
						ABL->Radius=v1;
						ABL->ExistTime=v2;
						ABL->UsageKind=v3;
						if(ReadS(&curs,sp)){
							ABL->IconID=GetIconByName(sp);
							ABL->FileID=0;
							if(ReadS(&curs,sp)){
								char* id=GetTextByID(sp);
								ABL->Message=znew(char,strlen(id));
								strcpy(ABL->Message,id);
								int nr=0;
								if(ReadI(&curs,&nr)){
									bool good=1;
									for(int p=0;p<nr;p++){
										if(ReadS(&curs,sp)){
											int R=GetResID(sp);
											int v=0;
											if(R>0&&ReadI(&curs,&v)){
												ABL->Cost[R]=v;
											}else good=0;
										}
									}
									if(good&&ReadS(&curs,sp)){
										if(!strcmp(sp,"COLLECT")){
											ABL->AbilityKind=255;
											int nc=0;
											if(ReadI(&curs,&nc)){
												bool good=1;
												for(int p=0;p<nc;p++){
													if(ReadS(&curs,sp)){
														ABL->Collection[p]=GetAbility(sp);
														if(ABL->Collection[p]==-1)good=0;
													}
												}
												ABL->Value=nc;
												done=good;
											}
										}else{
											int p=0;
											while(AblList[p][0]&&strcmp(sp,AblList[p]))p++;
											if(AblList[p][0]){
												ABL->AbilityKind=p;
												if(ReadI(&curs,&ABL->Value)){
													done=1;
												}
											}
										}
									}
								}
							}
						}
					}
				}
				if(!done)ABLERR;
			};
		}while(RF);
		Gclose(F);
	}
}
void LoadAbils(){
	ABILS.Load("Ability.dat");
};
//export functions for ability control
CEXPORT
int GetAbilityByName(char* Name){
	return ABILS.GetAbility(Name);
}
CEXPORT
void GetAbilityCost(int Index,int* Cost){
	memcpy(Cost,ABILS.Abils[Index].Cost,8*8);
}
CEXPORT
char* GetAbilityMessage(int Index){
	return ABILS.Abils[Index].Message;
}
CEXPORT
void GetAbilitySprite(int Index,int& File,int& Sprite){
	File  =ABILS.Abils[Index].FileID;
	Sprite=ABILS.Abils[Index].IconID;
};
bool Brigade::DisbandOne(){
	for(int i=NBPERSONAL;i<NMemb;i++){
		OneObject* OB=Group[Memb[i]];
		if(i!=0xFFFF&&OB&&OB->Serial==MembSN[i]){
			Memb[i]=0xFFFF;
			OB->BrigadeID=0xFFFF;
			OB->InArmy=false;
			return true;
		}
	}
	return false;
};
CEXPORT bool BrigDisbandOne(byte NI, int BrIndex){
	if(NI<8&&BrIndex<CITY[NI].NBrigs){
		Brigade* BR=CITY[NI].Brigs+BrIndex;
		if(BR){
			return BR->DisbandOne();
		}
	}
	return false;
};
#endif //COSSACKS2