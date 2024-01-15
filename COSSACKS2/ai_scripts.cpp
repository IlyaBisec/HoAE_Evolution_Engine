#include "stdheader.h"
#include "ai_scripts.h"
#include ".\cvi_HeroButtons.h"
#include "HashTop.h"
#include "BrigadeAI.h"
#include "unitability.h"
#include "PluginsInterface.h"

extern City CITY[8];
extern char** NatNames;
void ReplaceEditor(char* EditorName, BaseClass* pClass);
extern int TrueTime;
extern int tmtmt;
extern BrigadeAI WiselyFormations;
typedef bool tpUnitsCallback(OneObject* OB,void* param);
int PerformActionOverUnitsInRadius(int xc,int yc,int R,tpUnitsCallback* CB,void* Param);


//
int CurAI_Color=0;
ScriptsScope GScripts;
void UnLoadGScripts(){
	GScripts.reset_class(&GScripts);
}

ComplexClassEditor GScripts_CE;
bool GScripts_First=1;
DialogsSystem GScripts_DE;
//
bool AIScripsEditorProcess(ClassEditor* CE,BaseClass* BC,int Options){
	CurAI_Color=MyNation;
	if(Options==2){
		CurAI_Color=MyNation;
		static byte NI=0xFF;
		//
		//ReplaceEditor("Started AI script",GScripts.Scripts+MyNation);
		//
		Enumerator* EU=ENUM.Get("UNITS_SHORTCUTS");
		static int RefTime=GetTickCount();
		if(GetTickCount()>RefTime){
			RefTime=GetTickCount()+1200;
			EU->Clear();
			//for(int i=0;i<8;i++)if(GScripts_CE.ExpInf.find(&GScripts.Scripts[i])!=-1){
			int i=MyNation;
			for(int j=0;j<GScripts.Scripts[i].Units.GetAmount();j++)if(GScripts.Scripts[i].Units[j]->ShortName.pchar()){
				EU->Add(GScripts.Scripts[i].Units[j]->ShortName.pchar(),GScripts.Scripts[i].Units[j]->UnitID);
			}
			for(int j=0;j<GScripts.Scripts[i].Buildings.GetAmount();j++)if(GScripts.Scripts[i].Buildings[j]->ShortName.pchar()){
				EU->Add(GScripts.Scripts[i].Buildings[j]->ShortName.pchar(),GScripts.Scripts[i].Buildings[j]->UnitID);
			}
		}
		if(NI!=MyNation){
			CE->ExpInfChanged=true;			
			GScripts.Scripts[MyNation].Copy(&GScripts.DebugScript,false);
			NI=MyNation;
		}else{
			static int Time=0;
			if(GetTickCount()>Time){
				Time=GetTickCount()+1000;				
				GScripts.DebugScript.Copy(GScripts.Scripts+MyNation,false);
			}
		}
		return false;
	}
	return false;
}
void ProcessAIEditor(){	
	GScripts_CE.Top.ClearAll();
	GScripts_CE.Bottom.ClearAll();
	//GScripts_CE.CreateFromClass(&GScripts_DE,0,0,RealLx,RealLy,&GScripts,3);
	GScripts_CE.Create(0,0,RealLx,RealLy,70,&GScripts);
	GScripts_First=0;
	extern int LastKey;
	LastKey=0;
	extern int GameMode;
	GameMode=1;
	int T0=GetTickCount();
	do{
		ProcessMessages();	
		if(bActive){
			Enumerator* EU=ENUM.Get("UNITS_SHORTCUTS");
			EU->Clear();
			//for(int i=0;i<8;i++)if(GScripts_CE.ExpInf.find(&GScripts.Scripts[i])!=-1){
			int i=1;
			for(int j=0;j<GScripts.Scripts[i].Units.GetAmount();j++)if(GScripts.Scripts[i].Units[j]->ShortName.pchar()){
				EU->Add(GScripts.Scripts[i].Units[j]->ShortName.pchar(),GScripts.Scripts[i].Units[j]->UnitID);
			}
			for(int j=0;j<GScripts.Scripts[i].Buildings.GetAmount();j++)if(GScripts.Scripts[i].Buildings[j]->ShortName.pchar()){
				EU->Add(GScripts.Scripts[i].Buildings[j]->ShortName.pchar(),GScripts.Scripts[i].Buildings[j]->UnitID);
			}
				//break;
			//}
			GPS.DrawFillRect(0,0,RealLx,RealLy,0xFF404040);			
			//bool LP=Lpressed;
			GPS.FlushBatches();			
			//Lpressed=LP;
			GScripts_CE.Process();
			//GScripts_DE.ProcessDialogs();		
			GScripts_DE.RefreshView();
			if(LastKey==13||LastKey==27)break;
			LastKey=0;
			if(GetTickCount()-T0>15000){
				xmlQuote xml("ai");
				GScripts.Scripts[1].Save(xml,&GScripts.Scripts[1]);
                xml.WriteToFile("ai\\autosave.ai.xml");
				T0=GetTickCount();
			}
		}
	}while(true);
}
void ResetDiversionGroups(BaseClass* BC){
	GoDiversion* F=dynamic_cast<GoDiversion*>(BC);
	if(F){
		F->Group=0xFFFF;
		F->CurAmount=F->MinAmount;
	}
	int N=BC->GetAmountOfElements();
	for(int i=0;i<N;i++){
		void* ptr=BC->GetElementPtr(i,BC);
		BaseClass* B=BC->GetElementClass(i);
		if(B->CheckDirectCasting()){
			B=(BaseClass*)ptr;
			ResetDiversionGroups(B);
		}
	}
}
void LoadScriptedAI(int Nat,char* file){
	if(!strstr(file,".xml")){
		char ccc[256];
		sprintf(ccc,"Script for AI must be xml-file: %s",file);
		ErrM(ccc);
		return;
	}
	xmlQuote xml;	
	if(xml.ReadFromFile(file)){
		ErrorPager EP(0);
		GScripts.Scripts[Nat].reset_class(&GScripts.Scripts[Nat]);
        GScripts.Scripts[Nat].Load(xml,&GScripts.Scripts[Nat],&EP);
		ResetDiversionGroups(&GScripts.Scripts[Nat]);
	}else{
		char ccc[256];
		sprintf(ccc,"Script for AI not found: %s",file);
		ErrM(ccc);
	}
}
bool ProcessLoadSave(int x,int y,int Lx,int Ly,_str& str,const char* mask,const char* StartDir,bool Load);

////CEXPORT int ItemChoose;

bool AI_ed_proc(SimpleDialog* SD){
	AI_ed_params* LS=(AI_ed_params*)SD->AllocPtr;
	int SelectUnitTypeDialog();
	int r=SelectUnitTypeDialog();
	ItemChoose=-1;
	if(r!=-1){
		strcpy(LS->Name,NATIONS->Mon[r]->MonsterID);		
		*LS->UnitID=r;
	}
	return true;	
}
void AI_Script::ProcessAI_Script(byte Nation){
	CurAI_Color=Nation;	
	for(int i=0;i<Folders.GetAmount();i++){
		
		AI_Folder* AIF=Folders[i];
		if(AIF->FolderCondition.Check()){
			for(int j=0;j<AIF->Rules.GetAmount();j++){
				AI_Trigger* AIT=AIF->Rules[j];
				if(AIT->Condition.Check()){
					for(int q=0;q<AIT->Action.GetAmount();q++){
						AIT->Action[q]->Perform();
					}
				}
			}
		}
	}
}
void ProcessScriptForNation(int NI){
	GScripts.Scripts[NI].ProcessAI_Script(NI);
}
//---------------------FINDING PLACE FOR BUILDINGS---------------------//
int GetNRoads();
void GetPreciseTopCenter(int Zone,int LockType,int& x,int& y);
struct PointStaticInfo{
	int x,y;
	int DistanceToRoad;
	int DistanceToTrees;
	int DistanceToStone;	
	char DirToTrees;
	char DirToStones;
	bool Locked;
	void CreateForPoint(int x,int y);

	unsigned int	hash	() const{
		return x*13+y*19;
	}
	bool			equal	( const PointStaticInfo& el ){
		return x==el.x&&y==el.y;
	}
	void			copy	( const PointStaticInfo& el ){
        *this=el;		
	}
};

word GetDir(int,int);
void PointStaticInfo::CreateForPoint(int x,int y){	

	DistanceToRoad  = 10000;
	DistanceToTrees = 10000;
	DistanceToStone = 10000;	

	int xx=x>>4;
	int yy=y>>4;
	const int dPt=12;
	Locked=CheckPt(xx-dPt,yy)||CheckPt(xx+dPt,yy)||CheckPt(xx,yy-dPt)||CheckPt(xx,yy+dPt)||CheckPt(xx,yy);
	if(Locked)return;
	//roads
	int NR=GetNRoads();
	if(EngSettings.DontUseRoads)NR=0;
	int MinR=1000000;
	for(int i=0;i<NR;i++){
		int xr,yr;
		GetPreciseTopCenter(i,0,xr,yr);
		int R=Norma(xr-x,yr-y);
		if(R<MinR)MinR=R;            
	}
	DistanceToRoad=MinR;
	//trees&stones
	int RTree =100000;
	int RStone=100000;
	int x0=x>>7;
	int y0=y>>7;

	int tdx=0;
	int tdy=0;
	int nt=0;

	int sdx=0;
	int sdy=0;
	int ns=0;

	for(int R=0;R<8;R++){
		int N=Rarr[R].N;
		char* xi=Rarr[R].xi;
		char* yi=Rarr[R].yi;
		for(int j=0;j<N;j++){
			int cx=x0+xi[j];
			int cy=y0+yi[j];
			if(cx>=0&&cy>=0&&cx<VAL_SPRNX&&cy<VAL_SPRNX){
				int cell=cx+(cy<<SprShf);
				int* CEL=SpRefs[cell];
				int   NCEL=NSpri[cell];
				if(NCEL){ //if there is many trees or stones
					for(int q=0;q<NCEL;q++){
						int spr=CEL[q];
						OneSprite* OS=&Sprites[spr];
						if(OS->Enabled){
							ObjCharacter* OC=OS->OC;
							int D=Norma(x-OS->x,y-OS->y);
							if(OC->ResType==0){ //wood
								if(D<RTree)RTree=D;
								R=100;
								int W=1000/(20+D);
								tdx+=(OS->x-x)*W;
								tdy+=(OS->y-y)*W;
								nt+=W;
							}else if(OC->ResType==2){ //stone
								if(D<RStone)RStone=D;
								R=100;
								int W=1000/(20+D);
								sdx+=(OS->x-x)*W;
								sdy+=(OS->y-y)*W;
								ns+=W;
							}
						}
					}				
				}
			}
		}
	}
	if(nt){
		tdx/=nt;
		tdy/=nt;
	}
	if(ns){
		sdx/=ns;
		sdy/=ns;
	}

	if(sdx>0)sdx=100;
	if(sdx<0)sdx=-100;
	if(sdy>0)sdy=100;
	if(sdy<0)sdy=-100;

	if(tdx>0)tdx=100;
	if(tdx<0)tdx=-100;
	if(tdy>0)tdy=100;
	if(tdy<0)tdy=-100;

	DirToTrees=GetDir(-tdx,-tdy);
	DirToStones=GetDir(-sdx,-sdy);

	DistanceToTrees=nt>8?RTree:100000;
	if( RStone<150 ){
		ns=0;
		Locked=true;
	}else if( RStone>250 ){
		ns=0;
	}
	DistanceToStone=ns>0?RStone:100000;
	
}
class MapInfoHash{
public:
	Hash<PointStaticInfo> BuildInfo;
	PointStaticInfo* GetPointInfo(int x,int y);
	void ClearAll();
};
PointStaticInfo* MapInfoHash::GetPointInfo(int x,int y){
	PointStaticInfo PS;
	PS.x=x;
	PS.y=y;
	int p=BuildInfo.find(PS);
	if(p==-1){
		PS.CreateForPoint(x,y);
		p=BuildInfo.add(PS);
	}
    return &BuildInfo.elem(p);
}
void MapInfoHash::ClearAll(){
	BuildInfo.reset();
}

class CBuildPointInfo
{
public:
	CBuildPointInfo()
	{
		cx=-1;
		cy=-1;
		r=0;
		szx=0;
		szy=0;
		time=AnimTime;
	};
	CBuildPointInfo(int x, int y, int rad, int sx, int sy)
	{
		cx=x;
		cy=y;
		r=rad;
		szx=sx;
		szy=sy;
		time=AnimTime;
	}
	int cx;
	int cy;
	int r;
	int szx;
	int szy;
	int time;
};
class CCanBuildMap
{
public:
	DynArray<CBuildPointInfo*> BuilPointInfo;
	int GetMinR(int x, int y, int type)
	{
		int rez=0;
		NewMonster* NM=NATIONS->Mon[type]->newMons;
		if((!NM->MaxDamage)&&(!NM->Producer))
		{
			int dx=abs(NM->BuildX1-NM->BuildX0);
			int dy=abs(NM->BuildY1-NM->BuildY0);
			int n=BuilPointInfo.GetAmount();
			for(int i=0;i<n;i++)
			{
				CBuildPointInfo* inf = BuilPointInfo[i];
				if(inf)
				{
					if((inf->time+256*25*60*5)>AnimTime)
					{
						if(inf->szx>=dx&&inf->szy>=dy)
						{
							int rr=inf->r-Norma(inf->cx-x,inf->cy-y);
							if(rr>rez)
								rez=rr;
						}
					}
					else
					{
						delete inf;
						BuilPointInfo[i]=NULL;
					}
				}
			}
		}
		return rez;
	};

	void FoundBuildPlace(int x, int y, int rad, int type)
	{
		NewMonster* NM=NATIONS->Mon[type]->newMons;
		if((!NM->MaxDamage)&&(!NM->Producer))
		{
			int dx = abs(NM->BuildX1-NM->BuildX0);
			int dy = abs(NM->BuildY1-NM->BuildY0);
			CBuildPointInfo* nb= new CBuildPointInfo(x,y,rad,dx,dy);
			bool find=false;
			int n=BuilPointInfo.GetAmount();
			for(int i=0;i<n;i++)
			{
				if(BuilPointInfo[i]==NULL)
				{
					BuilPointInfo[i]=nb;
					find=true;
				}
			}
			if(!find)
				BuilPointInfo.Add(nb);
		}
	};
	
	void Clear()
	{
		int n = BuilPointInfo.GetAmount();
		for(int i=0;i<n;i++)
		{
			if(BuilPointInfo[i])
			{
				delete (BuilPointInfo[i]);
				BuilPointInfo[i]=NULL;
			}
		}
		BuilPointInfo.Clear();
	};
	~CCanBuildMap()
	{
		Clear();
	}
};

CCanBuildMap CanBuildMap;
MapInfoHash MHash;

void ShowPointStaticInfo(int x, int y, int mapX, int mapY){
	PointStaticInfo* PS=MHash.GetPointInfo(mapX,mapY);
	if(PS){
		DString str;
		//str.print("x: %d, y: %d",mapX,mapY);
		//ShowString(x,y,str.pchar(),&WhiteFont);
		y+=20;
		str.Clear();
		str.print("x: %d, y: %d",PS->x,PS->y);
		ShowString(x,y,str.pchar(),&WhiteFont);
		y+=20;
		str.Clear();
		str.print("DistanceToRoad: %d",PS->DistanceToRoad);
		ShowString(x,y,str.pchar(),&WhiteFont);
		y+=20;
		str.Clear();
		str.print("DistanceToTrees: %d",PS->DistanceToTrees);
		ShowString(x,y,str.pchar(),&WhiteFont);
		y+=20;
		str.Clear();
		str.print("DistanceToStone: %d",PS->DistanceToStone);
		ShowString(x,y,str.pchar(),&WhiteFont);
		y+=20;
		str.Clear();
		str.print("DirToTrees: %d",PS->DirToTrees);
		ShowString(x,y,str.pchar(),&WhiteFont);
		y+=20;
		str.Clear();
		str.print("DirToStones: %d",PS->DirToStones);
		if(PS->Locked)
			ShowString(x,y,"Locked",&WhiteFont);
		else
			ShowString(x,y,"Not Locked",&WhiteFont);
	}
}

void ClearMapInfoHash(){
    MHash.ClearAll();
	CanBuildMap.Clear();
}

int CheckCreationAbility(byte NI,int Type,int* x2i,int* y2i,word* BLD,int NBLD,bool);
int CreateBLDList(byte NI,word* BLD,int MaxBLD,int x0,int y0);
bool GetBuildingEntryPointShift(int* x,int* y,word TypeIndex);
DLLEXPORT int GetTopZone(int x,int y);

int TryToStandBuildingAnywhere(int x,int y,byte NI,int Type,BuildRequirements* Req){
	if(CITY[NI].NPeas<3){
		return -1;
	}
	int NC_Mines=0;
	int NC_NoMines=0;
	int n=CITY[NI].NConstructions;
	for(int i=0;i<n;i++){
		word ID=CITY[NI].Construction[i];
		OneObject* OB=Group[ID];
		if(OB&&OB->Serial==CITY[NI].ConstructionSN[i]){
            if(OB->Usage==MineID)NC_Mines++;
			else NC_NoMines++;
		}
	}
	if(NATIONS[NI].Mon[Type]->newMons->Usage==MineID){
		if(NC_Mines>int(Req->MultipleConstruction))return -1;        
	}else{
		if(NC_NoMines>int(Req->MultipleConstruction))return -1;        
	}
	//if(CITY[NI].NConstructions>int(Req->MultipleConstruction))return -1;
	int bdx=0;
	int bdy=0;
	GetBuildingEntryPointShift(&bdx,&bdy,Type);

	if(bdx>0)bdx=100;
	if(bdx<0)bdx=-100;
	if(bdy>0)bdy=100;
	if(bdy<0)bdy=-100;

	char EntryDir=GetDir(-bdx,-bdy);
	char EnemyDir=0;
	int cx0=(CITY[NI].CenterX<<7)+64;
	int cy0=(CITY[NI].CenterY<<7)+64;
	if(Req->NearToEnemy||Req->FarFromEnemy){
		int totdx=0;
		int totdy=0;
		int totw=0;
		int ecx=0;
		int ecy=0;
		byte NMask=NATIONS[NI].NMask;
		for(int i=0;i<MAXOBJECT;i++){
			OneObject* OB=Group[i];
			if(OB&&!(OB->Sdoxlo||(NMask&OB->NMask))&&(!OB->AI_Guard||OB->Attack)){
				int x=OB->RealX>>4;
				int y=OB->RealY>>4;
				ecx+=x;
				ecy+=y;
                totdx+=x-cx0;
				totdy+=y-cy0;
				totw++;
			}
		}
		if(totw){
			ecx/=totw;
			ecy/=totw;
			int DistToEnemy=Norma(ecx-cx0,ecy-cy0);
			if( DistToEnemy<2000 )
				return -1;
			totdx/=totw;
			totdy/=totw;
			EnemyDir=GetDir(totdx,totdy);
		}
	}
	int BLDX[64];
	int BLDY[64];
	int NBLD=0; 
	if(Req->FarFromSameType){
		int N=NtNUnits[NI];
		word* IDS=NatList[NI];
		for(int i=0;i<N;i++){
            word ID=IDS[i];
			if(ID!=0xFFFF){
				OneObject* OB=Group[ID];
				if(OB&&OB->NIndex==Type&&NBLD<64){
                    BLDX[NBLD]=OB->RealX>>4;                    
					BLDY[NBLD]=OB->RealY>>4;
					NBLD++;
				}
			}
		}		
	}	
	NewMonster* NM=NATIONS->Mon[Type]->newMons;
	if(!NM->Building)return -1;
	word TotBLD[512];
	int Nb=0;
	if(NM->Building)Nb=CreateBLDList(NI,TotBLD,512,x,y);
    int xbest=x;    
	int ybest=y;
	int Weight=0;
	int xt=(x+y)/2;
	int yt=(x-y)/2;	
	int dst=64;
	int dsh=6;
	if(NM->Port||NM->ProdType){
		if(NM->Port){
			dst=256;
			dsh=8;
		}else{
			dst=128;
			dsh=7;
		}
	}
	xt=(xt&(~(dst-1)))+(dst-1);
	yt=(yt&(~(dst-1)))+(dst-1);
	int R0=Req->MinDistFromCenter>>dsh;
	int RM=(min(4096,Req->MaxDistFromCenter)>>dsh)+R0;
	if(NM->ProdType)RM+=(4098)>>dsh;
	
	if(NM->Port){
		if(RM>20)RM=20;
	}else{
		if(RM>100)RM=100;
	}	
	int ISLAND=CITY[NI].MyIsland;

	int R00 = CanBuildMap.GetMinR(xt,yt,Type)>>dsh;
	int FoundR=0;
	if(R00>R0)
		R0=R00;
	for(int R=R0;R<RM;R++){
		int N=Rarr[R].N;
		char* xi=Rarr[R].xi;
		char* yi=Rarr[R].yi;
		for(int j=0;j<N;j++){
			int xt1=xt+(int(xi[j])<<dsh);
			int yt1=yt+(int(yi[j])<<dsh);
			int xr=(xt1+yt1)<<4;
			int yr=(xt1-yt1)<<4;
			int tx=xr>>10;
			int ty=yr>>10;
			if(tx>0&&ty>0&&tx<TopLx-1&&ty<TopLx){
				int tz=HashTable[0].GetTopAreaIndex(tx-4,ty-4);
				if(tz>=0xFFFE)tz=HashTable[0].GetTopAreaIndex(tx+4,ty+4);
				if(tz>=0xFFFE)tz=HashTable[0].GetTopAreaIndex(tx-4,ty+4);
				if(tz>=0xFFFE)tz=HashTable[0].GetTopAreaIndex(tx+4,ty-4);
				extern word* TopIslands;
				if(tz>=0&&tz<0xFFFE&&TopIslands[tz]==ISLAND){

					if( VAI() ){
						int d=VAI()->GetDistFromBase(NI,tz);
						if(d>Req->MaxDistFromCenter){
							continue;
						}
					}				

					int x_pixel=xr>>4;
					int y_pixel=yr>>4;

					PointStaticInfo* PS=MHash.GetPointInfo(x_pixel,y_pixel);
					if((!PS->Locked)&&PS->DistanceToRoad>250){
						int res=CheckCreationAbility(NI,Type,&xr,&yr,TotBLD,Nb,0);
						if( res!=-1 ){

							tz=GetTopZone(x_pixel,y_pixel);
							if( VAI() ){
								int d=VAI()->GetDistFromBase(NI,tz);
								if(d>Req->MaxDistFromCenter){
									continue;
								}
							}

							//building can be built
							int w=100000/(R+150);
							if(Req->NearRoad&&PS->DistanceToRoad<700)w+=1000;
							if(Req->NearTrees&&PS->DistanceToTrees<600){
								w+=500;
								if(EntryDir==PS->DirToTrees)w+=500;
							}
							if(Req->NearStones&&PS->DistanceToStone<500){
								w+=100;
								if(EntryDir==PS->DirToStones)w+=500;
							}
							if(Req->NearStones&&PS->DistanceToStone<700){
								w+=100;
								if(EntryDir==PS->DirToStones)w+=500;
							}
							if(Req->NearStones&&PS->DistanceToStone<900){
								w+=100;
								if(EntryDir==PS->DirToStones)w+=500;
							}
							if(Req->NearToEnemy!=Req->FarFromEnemy){
								char dd=GetDir(x_pixel-cx0,y_pixel-cy0)-EnemyDir;
								int ad=abs(int(dd))*10;
								if(Req->NearToEnemy)w-=ad;
								if(Req->FarFromEnemy)w+=ad;                        
							}
							if(Req->FarFromSameType){
								for(int i=0;i<NBLD;i++){
									int r=Norma(xr-BLDX[i],yr-BLDY[i]);
									w+=200000/(r+200);
								}
							}
							if(w>Weight){
								Weight=w;
								xbest=xr;
								ybest=yr;
							}
							if(FoundR==0)
							{
								FoundR=R;
							}
						}
					}
				}
			}
		}
	}
	if(Weight==0)
	{
		CanBuildMap.FoundBuildPlace(xt,yt,RM<<dsh,Type);
	}
	if(Weight>0){

		if( !VAI() || VAI()->CheckCleanArea(NI,iVector2D(xbest>>4,ybest>>4)) ){
			CanBuildMap.FoundBuildPlace(xt,yt,FoundR<<dsh,Type);
			cv_AI_ScriptExt::SetNewUnitBuildRequirements(Req);
			int idx=NATIONS[NI].CreateNewMonsterAt(xbest,ybest,Type,0,0);		
			if(idx!=-1){
				OneObject* OB=Group[idx];
				if(OB){
					/*
					int CreateTeamOfPeasants(byte NNUM,int x,int y,int MinP,int MaxP);
					void BuildWithSelected(byte NI,word ObjID,byte OrdType);
					int NP=CreateTeamOfPeasants(NI,OB->RealX,OB->RealY,Req->MinPeasants,Req->MaxPeasants);
					if(NP){
						BuildWithSelected(NI,idx,1);					
					}
					*/
					CITY[NI].AddConstruction(OB);
					NewMonster* NM=OB->newMons;
					//wood
					if(NM->ResConcentrator&1){
						UnitsGroup* UG=&SCENINF.UGRP[CITY[NI].WoodSkladGroup];
						UG->RefreshUnits();
						UG->AddNewUnit(OB);
					}
					//stone
					if(NM->ResConcentrator&4){
						UnitsGroup* UG=&SCENINF.UGRP[CITY[NI].StoneSkladGroup];
						UG->RefreshUnits();
						UG->AddNewUnit(OB);
					}
					//food
					if(NM->ResConcentrator&8){
						UnitsGroup* UG=&SCENINF.UGRP[CITY[NI].MelnGroup];
						UG->RefreshUnits();
						UG->AddNewUnit(OB);
					}
				}
			}
			return idx;
		}
	}
	return -1;
}
extern int CBB_GPFILE;
////CEXPORT int ItemChoose;
bool MMItemChoose(SimpleDialog* SD);
extern int NNations;
extern int TP_CurNation;
extern int LastKey;
extern bool KeyPressed;
int SelectUnitTypeDialog(){
	Lpressed=false;
	void UnPress();
	UnPress();
	DialogsSystem TOOL_PARAM;
	ColoredBar* CB=TOOL_PARAM.addColoredBar(0,0,RealLx,RealLy,0xFF);
	CB->Diffuse=CED_BackColor;
	ComboBox* TP_GPB=TOOL_PARAM.addGP_ComboBoxDLX(NULL,10,10,400,CBB_GPFILE,0,9,0,&WhiteFont,&YellowFont,NULL);
	for(int i=0;i<NNations;i++)TP_GPB->AddLine(NatNames[i]);
	TP_GPB->CurLine=TP_CurNation;
	DialogsDesk* DD=TOOL_PARAM.AddDialogsDesk(0,40,RealLx,RealLy-40-40,"EmptyBorder");
	int NBTN=3;
	int BTDY=30;
	const char* msgs[]={"Ok","Cancel",""};	
	TextButton* TBS[2];
	int Tl=0;
	for(int i=1;i<NBTN;i++){
		Tl+=GetRLCStrWidth((char*)msgs[i-1],&YellowFont);
	}
	int Lx=RealLx;
	int Ly=RealLy;
	Tl=Lx-Tl;
	Tl/=NBTN;
	int xx=Tl;
	for(int i=1;i<NBTN;i++){
		TextButton* TB=TOOL_PARAM.addTextButton(NULL,xx,Ly-BTDY,(char*)msgs[i-1],&WhiteFont,&YellowFont,&YellowFont,0);
		xx+=Tl+GetRLCStrWidth((char*)msgs[i-1],&YellowFont);
		TB->OnUserClick=&MMItemChoose;
		TB->UserParam=i;
	}
	ItemChoose=-1;
    TP_GPB->CurLine=TP_CurNation;
	TP_CurNation=-1;
	//Units list
	int yy=0;
	int IconFileLX=56;
	int IconFileLY=118;
	int IconDX=IconFileLX+20;
	int IconDY=IconFileLY+20;
	int ICNX=(RealLx-30)/IconDX;
	do{
		if(TP_GPB->CurLine!=TP_CurNation){
			yy=0;
			xx=0;
			DD->DSS.Clear();
			TP_CurNation=TP_GPB->CurLine;
			int NUNITS=NATIONS->NUnits[TP_CurNation];
			for(int j=0;j<NUNITS;j++){
				int UIDS=NATIONS->UnitsIDS[TP_CurNation][j];
				if(UIDS==0xFFFF){
					if(xx)yy+=IconDY;
					TextButton* TB=DD->addTextButton(NULL,10,yy,GetTextByID(NATIONS->UnitNames[TP_CurNation][j]),&YellowFont,&YellowFont,&YellowFont,0);					
					yy+=20;
					xx=0;
				}else {
					GeneralObject* GO=NATIONS->Mon[UIDS];
					if(xx>=ICNX){
						xx=0;
						yy+=IconDY;
					};
					SimpleDialog* SD=NULL;
					if(GO->newMons->IconID!=0xFFFF){
						if(GO->newMons->MinIconFile!=0xFFFF){
							GPPicture* GPP=DD->addGPPicture(NULL,6+xx*IconDX,yy,GO->newMons->MinIconFile,GO->newMons->MinIconIndex);
							GPP->Hint=GO->Message;
							SD=GPP;
						}else{
                            ColoredBar* CB=DD->addColoredBar(6+xx*IconDX,yy,IconFileLX,IconFileLY,0xFF);
							CB->Diffuse=0x30000000;
							CB->Hint=GO->Message;
							SD=CB;
						}
					};
					TextButton* TB=DD->addTextButton(NULL,6+xx*IconDX+IconFileLX/2,yy+IconFileLY,GO->MonsterID,&SmallYellowFont,&SmallYellowFont,&SmallYellowFont,1);
					xx++;
					if(SD){
						SD->UserParam=1024+UIDS;
						SD->OnUserClick=&MMItemChoose;
					}
				};
			};
		}
		ProcessMessages();
        TOOL_PARAM.ProcessDialogs();
		TOOL_PARAM.RefreshView();
		if(LastKey==27)ItemChoose=1;
	}while(ItemChoose==-1);	
	if(ItemChoose>=1024){
		return ItemChoose-1024;
	}else return -1;
	LastKey=0;
	KeyPressed=0;
}
bool SettlementsPriory::Perform(){
	CITY[CurAI_Color].SettlementPriory[ResType]=Prio;
	return false;
}
void SettlementsPriory::GetBriefing(_str& str){
	str.print("Prio(%s)=%d%%",RDS[ResType].Name,Prio);
};
void SettlementsPriory::GetShortBriefing(_str& str){
	str.print("Prio(%s)=%d%%",RDS[ResType].Name,Prio);
};
extern byte INVECO[6];
bool BuyResource::Perform(){
	void PerformTorg(byte Nation,byte SellRes,byte BuyRes,int SellAmount);
	PerformTorg(CurAI_Color,INVECO[ResTypeToSell],INVECO[ResTypeToBuy],Amount);
	return false;
}
void BuyResource::GetBriefing(_str& str){
	str.print("Sell(%d %s -> %s)=%d%%",Amount,RDS[ResTypeToSell].Name,RDS[ResTypeToBuy].Name);
}
void BuyResource::GetShortBriefing(_str& str){
	GetBriefing(str);
}
void PeonsDistribution::GetBriefing(_str& str){
	str.print("Peasants: Wood-%d,Stone-%d,Food-%d",OnWood,OnStone,OnFood);
};
void PeonsDistribution::GetShortBriefing(_str& str){
	str.print("W=%d\\S=%d\\F=%d",OnWood,OnStone,OnFood);
};
void AddResources::GetBriefing(_str& str){
	Enumerator* E=ENUM.Get("RESTYPE");
	if(E){
		str.print("%s+=%d",E->GetStr(ResType),Amount);
	}
}
void AddResources::GetShortBriefing(_str& str){
	GetBriefing(str);
}
void UpgradeSettlements::GetBriefing(_str& str){
	str.print("UpgSettlement(%s,%d%%,%d)",RDS[ResType].Name,CostPercent,MaxDistance);
};
void UpgradeSettlements::GetShortBriefing(_str& str){
	str.print("UpgSettlement(%s,%d%%)",RDS[ResType].Name,CostPercent);
};
bool EnemyNearBase::Check(){
	int x0=CITY[CurAI_Color].CenterX*128;
	int y0=CITY[CurAI_Color].CenterY*128;
	byte Mask=NATIONS[CurAI_Color].NMask;
	int N=0;
	for(int i=0;i<MAXOBJECT;i++){
        OneObject* OB=Group[i];
		if(OB&&!(OB->Sdoxlo||(OB->NMask&Mask)||OB->AI_Guard)){
            int R=Norma(x0-(OB->RealX>>4),y0-(OB->RealY>>4));
			if(R<Radius){
				if(OB->NewBuilding){
					if(OB->NNUM!=7){
						if(OB->MoreCharacter->MaxDam){
							if(Towers)N++;
						}else{
							if(Buildings)N++;
						}
					}
				}else{
					if(OB->newMons->Artilery){
                        if(Artillery)N++;
					}else{
						if(OB->newMons->MotionDist>48){
                            if(Cavalery)N++;
						}else{
							if(Infantry)N++;
						}
					}
				}
			}
		}
	}
	bool res=N>=MinAmount&&N<=MaxAmount;
	return res&&AI_Condition::Check();
}
//
CIMPORT void AddStorm(GAMEOBJ* Src, byte Owner, int Diff=2);
CEXPORT void AddStormSingleUnitMotion(GAMEOBJ* Src, byte Owner, int Diff=2);
CIMPORT void AddFirers(GAMEOBJ* Src,byte Owner);
CIMPORT void AddPsKillers(GAMEOBJ* Src,byte Owner,bool SeakMine=true);
CIMPORT void AddTomahawks(GAMEOBJ* Src,byte Owner,word Base=0,int DriftRadius=0, int CriticalMass=0);
DLLEXPORT int GetRND(int Max);
DLLEXPORT int GetTotalAmount0(GAMEOBJ* Units);
//DLLEXPORT int GetTotalAmount2(GAMEOBJ* Units,GAMEOBJ* UnitType,byte Nation);
DLLEXPORT int CleanGroup(GAMEOBJ* Grp);
//DLLEXPORT void CopyUnits(GAMEOBJ* Src,GAMEOBJ* Dest,int start,int n,bool add,bool remove);
DLLEXPORT int CollectUnits(GAMEOBJ* Dst,GAMEOBJ* Src,int N,int NIndex);
//
GoDiversion::GoDiversion(){
	UnitType=0;
	CurAmount=0;
	MinAmount=20;
	MaxAmount=100;
	StepAmount=30;
	Group=0xFFFF;
	StormPercent=50;
	FirePercent=50;
}
//
CEXPORT int CreateBrig(byte NI, UnitsGroup* Source, UnitsGroup* Dest, word UnitNIndex);
DLLEXPORT int GetUnitsTypeInGroup(GAMEOBJ* Grp, word NIndex);
CIMPORT void Cannon2Alex(GAMEOBJ* Src, byte Owner);
//
bool GoDiversion::Perform(){
	//int nOrd=Orders.GetAmount();
	//if(nOrd){
		if(Group==0xFFFF) Group=SCENINF.GetNewGroupIndex();
		GAMEOBJ g={Group,0,'UNIT'};		
		GAMEOBJ s={CITY[CurAI_Color].AllWariorsGroup,0,'UNIT'};	
		//
		Cannon2Alex(&s,CurAI_Color);
		//
		int n=GetUnitsTypeInGroup(&s,UnitType); //CleanGroup(&s);
		if(CurAmount==0){
			CurAmount=MinAmount;
		}
		bool gogogo = n>=CurAmount ||
				VAI() && VAI()->CheckProtectBase(CurAI_Color) && (n>CurAmount/3||n>11) ;
		if(gogogo){
			//CollectUnits(&g,&s,CurAmount,UnitType);
			
			//int ordsize[3]={36,64,100};
			//CurAmount=ordsize[GetRND(3)];
			CurAmount+=StepAmount;
			//if(CurAmount<MinAmount)CurAmount=MinAmount;
			if(CurAmount>MaxAmount)CurAmount=MaxAmount;

			UnitsGroup* Src=SCENINF.UGRP+s.Index;
			UnitsGroup* Dst=SCENINF.UGRP+Group;
			
			bool Fires=false;				
			if(!Melee&&canAttackBuilding&&GetRND(100)<FirePercent){
				Fires=true;
				//WithoutBrigade=true;
			};
			
			if(WithoutBrigade){
				for(int i=Src->N-1;i>=0;i--){
					word ID=Src->IDS[i];
					if(ID!=0xFFFF){
						OneObject* OB=::Group[ID];
						if(OB&&OB->Serial==Src->SNS[i]&&OB->NIndex==UnitType){
                            Src->CopyUnits(Dst,i,1,true,true);
						}
					}
				}
				//Src->CopyUnits(Dst,0,Src->N,true,true);
			}
			if(WithoutBrigade||CreateBrig(CurAI_Color,Src,Dst,UnitType)>=0){

				if(VAI()){
					VAI()->AddSquad(CurAI_Color,&g);
				}else{
					///AddStorm(&g,CurAI_Color,CITY[CurAI_Color].Difficulty);
					if(SingleUnitMotion){
						AddStormSingleUnitMotion(&g,CurAI_Color,CITY[CurAI_Color].Difficulty);
					}else
					if(Melee){
						if(GetRND(100)<StormPercent){
							AddStorm(&g,CurAI_Color,CITY[CurAI_Color].Difficulty);
						}else{
							AddPsKillers(&g,CurAI_Color,false);
						}				
					}else{
						if(Fires){
							AddFirers(&g,CurAI_Color);
						}else{
							AddTomahawks(&g,CurAI_Color,canAttackBuilding);
						}
					}
				}

			}

			CleanGroup(&g);
			CleanGroup(&s);

		}
	//}
	return false;
}
//
extern bool NOPAUSE;
//
void cv_AI_ScriptExt::ProcessingGame(){
	if(NOPAUSE&&TrueTime-pLastTime>200){
		pLastTime=TrueTime;
		for(int i=0;i<7;i++){
			//Nation* NT=NATIONS+i;
			if(NATIONS[i].AI_Enabled){
				City* CT=CITY+i;
				Scripts[i].Process(CT);
			}		
		}
	}
};
void cv_AI_ScriptExt::OnUnloading(){
	pLastTime=0;
	for(int i=0;i<7;i++){
		Scripts[i].reset_class(Scripts+i);
	}
};
void cv_AI_ScriptExt::OnUnitBirth(OneObject* NewUnit){
	byte NI=NewUnit->NNUM;
	if(NATIONS[NI].AI_Enabled){		
		Scripts[NI].addNewUnit(NewUnit,&NewUnitReq);
	}
};
//
BuildRequirements cv_AI_ScriptExt::NewUnitReq;
void cv_AI_ScriptExt::SetNewUnitBuildRequirements(BuildRequirements* Req){	
	Req->Copy(&NewUnitReq,false);
};
int cv_AI_Script::addNewUnit(OneObject* OB, BuildRequirements* Req){
	//if(OB->Usage==MineID){
	if(OB->NewBuilding){
		cv_AI_Building* BD=new cv_AI_Building;
		BD->obIndex=OB->Index;
		BD->obSerial=OB->Serial;
		BD->grpInvader=SCENINF.GetNewGroupIndex();
		BD->grpRepaire=SCENINF.GetNewGroupIndex();
		BD->repMin=Req->MinPeasants;
		BD->repMax=Req->MaxPeasants;
		Log.Info("Produce: %d %s",CITY[CurAI_Color].UnitAmount[OB->NIndex],OB->newMons->MD_File);
		return Buildings.Add(BD);
	}
	return -1;
};
void cv_AI_Script::Process(City* CT){
	for(int i=0;i<Buildings.GetAmount();i++){
		if(!Buildings[i]->Process(CT)){
			Buildings.DelElement(i);
			i--;
		};
	}
};
//
void ComeIntoBuilding(UnitsGroup* UGR,byte Type,int Index);
void RepairBuilding(UnitsGroup* UGR,byte Type,int Index);
extern "C" CEXPORT void SGP_ComeIntoBuilding(GAMEOBJ* Grp,byte Type,int Index);
DLLEXPORT bool RemoveNearestUnit(GAMEOBJ* G_Src,GAMEOBJ* G_Dst,int x,int y,int MaxR);
DLLEXPORT void DeleteHiddenUnits(GAMEOBJ* Grp);
DLLEXPORT void RemoveGroup(GAMEOBJ* Source,GAMEOBJ* Dest);
//
bool cv_AI_Building::Process(City* CT){	
	OneObject* OB=Group[obIndex];
	GAMEOBJ rep={grpRepaire,0,'UNIT'};
	if(OB&&OB->Serial==obSerial&&!OB->Sdoxlo){
		//
		if(OB->Ready){
			if(!Ready){
				Ready=true;
				Log.Info("Complete: %d %s",CITY[CurAI_Color].ReadyAmount[OB->NIndex],OB->newMons->MD_File);
			}
		}
		// construction & repaire		
		UnitsGroup* Rep=SCENINF.UGRP+grpRepaire;
		Rep->RefreshUnits();
		if(!OB->Ready||OB->Life<OB->MaxLife){
			int n=0;
			if(Rep->N==0){
				n=repMin;
				GAMEOBJ g={0,0,'UNIT'};
				g.Index=CT->LasyPeasantsGroup;
				while(n>0&&RemoveNearestUnit(&g,&rep,OB->RealX>>4,OB->RealY>>4,8000)){
					n--;
				}
				//UnitsGroup* ug;
				g.Index=CT->OnStoneGroup;
				//ug=SCENINF.UGRP+CT->OnStoneGroup;
				while(n>0&&RemoveNearestUnit(&g,&rep,OB->RealX>>4,OB->RealY>>4,4000)){
					n--;
				}
				g.Index=CT->OnWoodGroup;
				//ug=SCENINF.UGRP+CT->OnWoodGroup;
				while(n>0&&RemoveNearestUnit(&g,&rep,OB->RealX>>4,OB->RealY>>4,4000)){
					n--;
				}
				g.Index=CT->OnFoodGroup;
				//ug=SCENINF.UGRP+CT->OnFoodGroup;
				while(n>0&&RemoveNearestUnit(&g,&rep,OB->RealX>>4,OB->RealY>>4,4000)){
					n--;
				}
			}else{
				n=repMax*(OB->MaxLife-OB->Life)/OB->MaxLife-Rep->N;
				GAMEOBJ g={0,0,'UNIT'};
				g.Index=CT->LasyPeasantsGroup;
				while(n>0&&RemoveNearestUnit(&g,&rep,OB->RealX>>4,OB->RealY>>4,5000)){
					n--;
				}
				UnitsGroup* ug;
				g.Index=CT->OnStoneGroup;
				ug=SCENINF.UGRP+CT->OnStoneGroup;
				if(n>0&&(ug->N>2||XRESRC(CT->NI,StoneID)>1000)){
					if(RemoveNearestUnit(&g,&rep,OB->RealX>>4,OB->RealY>>4,1000)) n--;
				}
				g.Index=CT->OnWoodGroup;
				ug=SCENINF.UGRP+CT->OnWoodGroup;
				if(n>0&&(ug->N>2||XRESRC(CT->NI,TreeID)>1000)){
					if(RemoveNearestUnit(&g,&rep,OB->RealX>>4,OB->RealY>>4,1000)) n--;
				}
				g.Index=CT->OnFoodGroup;
				ug=SCENINF.UGRP+CT->OnFoodGroup;
				if(n>0&&(ug->N>5||XRESRC(CT->NI,FoodID)>2000)){
					if(RemoveNearestUnit(&g,&rep,OB->RealX>>4,OB->RealY>>4,1000)) n--;
				}
			}
			RepairBuilding(Rep,0,obIndex);
		}else{
			GAMEOBJ g={CT->LasyPeasantsGroup,0,'UNIT'};
			RemoveGroup(&rep,&g);
		}
		// come in
		if(OB->Usage==MineID){
			if(OB->Ready){			
				GAMEOBJ inv={grpInvader,0,'UNIT'};
				DeleteHiddenUnits(&inv);
				UnitsGroup* Inv=SCENINF.UGRP+grpInvader;
				Inv->RefreshUnits();
				int maxIn=OB->MoreCharacter->MaxInside+OB->AddInside;
				int n=maxIn-OB->NInside-Inv->N;
				if(n>0){
					GAMEOBJ g={0,0,'UNIT'};
					g.Index=CT->LasyPeasantsGroup;					
					while(n>0&&RemoveNearestUnit(&g,&inv,OB->RealX>>4,OB->RealY>>4,5000)){
						n--;
					}
					UnitsGroup* ug;
					g.Index=CT->OnStoneGroup;
					if(n>0) if(RemoveNearestUnit(&g,&inv,OB->RealX>>4,OB->RealY>>4,1000)) n--;
					g.Index=CT->OnWoodGroup;
					if(n>0) if(RemoveNearestUnit(&g,&inv,OB->RealX>>4,OB->RealY>>4,1000)) n--;
					g.Index=CT->OnFoodGroup;
					ug=SCENINF.UGRP+CT->OnFoodGroup;
					if(n>0&&(ug->N>5||XRESRC(CT->NI,FoodID)>2000)){
						if(RemoveNearestUnit(&g,&inv,OB->RealX>>4,OB->RealY>>4,1000)) n--;
					}
					ComeIntoBuilding(Inv,0,obIndex);
				}
			}
		}
		return true;
	}
	GAMEOBJ g={CT->LasyPeasantsGroup,0,'UNIT'};
	RemoveGroup(&rep,&g);
	return false;
};
const char*	AI_Folder::GetThisElementView(const char* LocalName){
	if(Description.pchar()){
		static char cc[256];
		sprintf(cc,"%s %s",LocalName,Description.pchar());
		return cc;
	}
	return LocalName;
}
bool aia_SplashBonus::Perform(){
	if(VAI())
		VAI()->SetSplashBonus(CurAI_Color,NIndex,Bonus);
	return false;
}
bool aia_UnitTypeLinks::Perform(){
	if(VAI()){
		static LinearArray<int,_int> L;
		L.Clear();
		int n=Links.GetAmount();
		for(int i=0;i<n;i++){
			L.Add(Links[i]->UnitType);
		}
		VAI()->SetUnitTypeLinks(CurAI_Color,NIndex,&L);
	}
	return false;
}
bool BridgeConstruction::Perform(){
	bool UseGroupLikeBridgeBuilders(UnitsGroup* ugrp,int BridgeType,byte NI);
	City* CT=CITY+CurAI_Color;
	CT->NBridgeBuilders=0;
	if(Enable){			
		UnitsGroup* UG=SCENINF.UGRP+CT->BridgeGroup;
		Enable=UseGroupLikeBridgeBuilders(UG,BridgeType,CurAI_Color);
		if(Enable){
			CT->NBridgeBuilders=NPeasant;
		}
	}		
	return true;
}
bool CheckEnemy(OneObject* OB,void* param)
{
	bool rez=false;
	if(OB&&!OB->Sdoxlo)
	{
		int *par=(int*)param;
		byte mm=(byte)par[3];
		if(!(OB->NMask&mm))
		{
			int ds=Norma(OB->RealX/16-par[4],OB->RealY/16-par[5]);
			if(ds<401)
			{
				par[0]+=((par[4]-OB->RealX/16)*100)/(401-ds);
				par[1]+=((par[5]-OB->RealY/16)*100)/(401-ds);
				par[2]++;
				rez=true;
			}
		}
	}
	return rez;
}
bool AutoUpRomeHero::Perform()
{
	if(tmtmt%5==0)
	{
		int ID=0;
		OneObject* Hero=vHeroButtons.GetObject(CurAI_Color,ID);
		while(Hero)
		{
			ActiveRomeHero *Hr=NULL;
			if(Hero->ActiveAbility)
			{
				int n = Hero->ActiveAbility->ActiveAbilities.GetAmount();
				int RandUp=0;
				int i;
				for(i=0;i<n&&!Hr;i++)
				{
					if(!Hr)
						Hr = dynamic_cast<ActiveRomeHero*> (Hero->ActiveAbility->ActiveAbilities[i]);
				}
				LeveledActiveUnitAbility* levelsA[3];
				levelsA[0]=NULL;levelsA[1]=NULL;levelsA[2]=NULL;
				if(Hr)
				{
					bool haveUp=false;
					int levels[3];
					for(i=0;i<n;i++)
					{
						LeveledActiveUnitAbility* Lv = dynamic_cast<LeveledActiveUnitAbility*> (Hero->ActiveAbility->ActiveAbilities[i]);
						if(Lv)
						{
							if(*Lv->Vi)
							{
								haveUp=true;
							}
							if(RandUp<3)
							{
								levels[RandUp]=*Lv->Le;
								levelsA[RandUp]=Lv;
							}
							RandUp++;
						}
					}
					RomeHero *H = (RomeHero*)Hr->GetA();
					if(H&&haveUp)
					{
						for(i=0;i<3;i++)
						{
							if(H->UpOrder[i]>0&&H->UpOrder[i]<4)
							{
								if(levels[H->UpOrder[i]-1]<5)
								{
									if(levelsA[H->UpOrder[i]-1])
									{
										levelsA[H->UpOrder[i]-1]->OnClick();
										break;
									}
								}
							}
						}
					}
					//HeroMove
					if(AutoMove)
					{
						int LifePr=Hero->Life*100/Hero->MaxLife;
						City* CT=CITY+CurAI_Color; 
						
						if(levelsA[2])
						{
							if(levelsA[2]->GetCoolDownProc()==0)
							{
								int nn=WiselyFormations.AllForm.GetAmount();
								bool fr=false;
								bool en=false;
								int rad=levelsA[2]->GetRadius();
								for(int i=0;i<nn;i++)
								{
									Atom *At=WiselyFormations.AllForm[i];
									if(At&&At->BSelf)
									{
										if(Norma(Hero->RealX/16-At->x,Hero->RealY/16-At->y)<rad)
										{
											if(At->GetNMask()&Hero->NMask)
												fr=true;
											else
												en=true;
										}
									}
								}
								if(fr&&en)
								{
									levelsA[2]->OnClick();
								}
							}
						}
						if(Hr)
						{
							RomeHero *H = (RomeHero*)Hr->GetA();
							if(H&&H->StayBack)
							{
								int par[6];
								par[0]=0;
								par[1]=0;
								par[2]=0;
								par[3]=(int)Hero->NMask;
								par[4]=Hero->RealX/16;
								par[5]=Hero->RealY/16;
								PerformActionOverUnitsInRadius(Hero->RealX/16,Hero->RealY/16,400,&CheckEnemy,(void*) par);
								if(par[2])
								{
									par[0]/=par[2];
									par[1]/=par[2];
									int ds=Norma(par[0],par[1]);
									if(ds)
									{
										par[0]=par[0]*400/ds;
										par[1]=par[1]*400/ds;
										if(!EngSettings.DontUseAgressiveState)Hero->ActivityState=1;
										Hero->NewMonsterSmartSendTo(Hero->RealX/16+par[0],Hero->RealY/16+par[1],0,0,128+16,0);
									}
								}
							}
						}

						if(Hero->Life<1000)
						{
							Hero->ActivityState=1;//Passiv
							word* id=NatList[CurAI_Color];
							int   n=NtNUnits[CurAI_Color];
							int pX=0;
							int pY=0;
							int pN=0;
							for(int i=0;i<n;i++)
							{
								if(id[i]!=0xFFFF)
								{
									OneObject *B=Group[id[i]];
									if(B&&B->newMons->Priest)
									{
										pX+=B->RealX/16;
										pY+=B->RealY/16;
										pN++;
									}
								}
							}
							int mtX=CT->CenterX*128;
							int mtY=CT->CenterY*128;
							int mDist=999999;
							if(pN)
							{
								pX/=pN;
								pY/=pY;
								for(int i=0;i<n;i++)
								{
									if(id[i]!=0xFFFF)
									{
										OneObject *B=Group[id[i]];
										if(B&&B->newMons->Priest)
										{
											int mDdd=Norma(B->RealX/16-pX,B->RealY/16-pY);
											if(mDdd<mDist)
											{
												mDdd=mDist;
												mtX=B->RealX/16;
												mtY=B->RealY/16;
											}
											B->AttackObj(Hero->Index,128+16,0,0);
										}
									}
								}
							}
							Hero->NewMonsterSmartSendTo(mtX,mtY,0,0,128+16,0);
						}
						else
						{
							//prist
							int nBrigs=0;
							int bX=0;
							int bY=0;
							int i;
							int mDist=9999999;
							int brX=0;
							int brY=0;
							//Brigade* GetNearesEnmBrigade();
							for(i=0;i<CT->NBrigs;i++)
							{
								if(CT->Brigs[i].Enabled)
								{
									int x,y;
									if(CT->Brigs[i].GetCenter(&x,&y))
									{
										Brigade* B=&CT->Brigs[i];
										Brigade* enmB=CT->Brigs[i].GetNearesEnmBrigade();
										int ex,ey;
										if(enmB&&enmB->GetCenter(&ex,&ey))
										{
											int d2e=Norma(x-ex,y-ey);
											if(d2e<mDist)
											{
												int nU=0;
												for(int k=0;k<B->NMemb;k++)
												{
													if(B->Memb[k]!=0xFFFF)
													{
														OneObject *bOB=Group[B->Memb[k]];
														if(bOB&&!bOB->Sdoxlo)
															nU++;

													}
												}
												if(nU>60)
												{
													mDist=d2e;
													brX=x;
													brY=y;	
												}
											}
											nBrigs++;
										}
									}
								}
							}
							if(nBrigs>1&&brX) 
							{
								/*
								bX/=nBrigs;
								bY/=nBrigs;
								for(i=0;i<CT->NBrigs;i++)
								{
									if(CT->Brigs[i].Enabled)
									{
										int x,y;
										if(CT->Brigs[i].GetCenter(&x,&y))
										{
											int dst=Norma(bX-x,bY-y);
											if(dst<mDist)
											{
												mDist=dst;
												brX=x;
												brY=y;
											}
										}
									}
								}
								*/
								if(!EngSettings.DontUseAgressiveState)Hero->ActivityState=0;//Normal
								//if(LifePr>55)
								//	Hero->ActivityState=2;//Agressiv
								Hero->NewMonsterSmartSendTo(brX-150+rando()%600,brY-150+rando()%600,0,0,128+16,0);
							}
							/*				
							Hero->ActivityState=0;//Normal
							if(LifePr>55)
								Hero->ActivityState=2;//Agressiv
							int cx=0;
							int cy=0;
							int Max=0;
							for(int i=1;i<VAL_MAXCX-1;i++)
							{
								for(int j=1;j<VAL_MAXCX-1;j++)
								{
									int NMon=0;
									if(NPresence[i+(j<<VAL_SHFCX)]&Hero->NMask)
										NMon+=MCount[i+(j<<VAL_SHFCX)];
									if(NPresence[i-1+((j-1)<<VAL_SHFCX)]&Hero->NMask)
										NMon+=MCount[i-1+((j-1)<<VAL_SHFCX)];
									if(NPresence[i-1+((j)<<VAL_SHFCX)]&Hero->NMask)
										NMon+=MCount[i-1+((j)<<VAL_SHFCX)];
									if(NPresence[i-1+((j+1)<<VAL_SHFCX)]&Hero->NMask)
										NMon+=MCount[i-1+((j+1)<<VAL_SHFCX)];
									if(NPresence[i+((j+1)<<VAL_SHFCX)]&Hero->NMask)
										NMon+=MCount[i+((j+1)<<VAL_SHFCX)];
									if(NPresence[i+((j-1)<<VAL_SHFCX)]&Hero->NMask)
										NMon+=MCount[i+((j-1)<<VAL_SHFCX)];
									if(NPresence[i+1+((j-1)<<VAL_SHFCX)]&Hero->NMask)
										NMon+=MCount[i+1+((j-1)<<VAL_SHFCX)];
									if(NPresence[i+1+((j)<<VAL_SHFCX)]&Hero->NMask)
										NMon+=MCount[i+1+((j)<<VAL_SHFCX)];
									if(NPresence[i+1+((j+1)<<VAL_SHFCX)]&Hero->NMask)
										NMon+=MCount[i+1+((j+1)<<VAL_SHFCX)];
									if(NMon>Max)
									{
										Max=NMon;
										cx=i;
										cy=j;
									}
								}
							}
							Hero->NewMonsterSmartSendTo(cx*128-300+rando()%600,cy*128-300+rando()%600,0,0,128+16,0);

							*/
						}
					}
				}
				//
			}
			Hero=vHeroButtons.GetObject(CurAI_Color,++ID);
		}
	}
	return true;
}
void SetAIEnable(byte NI, bool State){
	if(NI<8){
		NATIONS[NI].AI_Enabled=State;
	}
}


bool AI_Act_DeleteNotBusy::Perform(){
	itr_GetUnitsOfNation.Create(CurAI_Color);
	while(OneObject* OB=itr_GetUnitsOfNation.Next()){
		if( !(OB->Sdoxlo||OB->LocalOrder) && OB->NIndex==UnitType ){
			OB->Die();
		}
	}
	return true;
}

//
CEXPORT char* GetResourceName(byte ResID);
//

void ai_cResourcePeasants::GetBriefing(_str& str){
	str="ResourcePeasants(";
	char* rname=GetResourceName(ResourceID);
	str+=rname;
	str+=")";
	str+=getops(OpType);		
	str+=Value;		
	AI_Condition::GetBriefing(str);
};
bool ai_cResourcePeasants::Check(){
	int N=CITY[CurAI_Color].ResourcePeasants[ResourceID];
	bool res=PerformAI_Op(N,Value,OpType);
	return res&&AI_Condition::Check();
};

void ai_aEnableIndividualUpgrade::GetBriefing(_str& str){
	Nation* CNAT=NATIONS+CurAI_Color;
	str.print("EnableIndividualUpgrade(%s)",CNAT->UPGRADE[UpgradeID]->Message);
}
bool ai_aEnableIndividualUpgrade::Perform(){
	Nation* CNAT=NATIONS+CurAI_Color;
	NewUpgrade* NU=CNAT->UPGRADE[UpgradeID];
	if(NU->Individual||NU->StageUp){
		NU->Enabled=true;
		NU->PermanentEnabled=true;
		NU->IsDoing=false;
	}
	return true;
}
void ai_aDisableIndividualUpgrade::GetBriefing(_str& str){
	Nation* CNAT=NATIONS+CurAI_Color;
	str.print("DisableIndividualUpgrade(%s)",CNAT->UPGRADE[UpgradeID]->Message);
}
bool ai_aDisableIndividualUpgrade::Perform(){
	Nation* CNAT=NATIONS+CurAI_Color;
	NewUpgrade* NU=CNAT->UPGRADE[UpgradeID];
	if(NU->Individual||NU->StageUp){
		NU->Enabled=false;
		NU->PermanentEnabled=false;
		//NU->IsDoing=false;
	}
	return true;
}

//PerformUpgrade::PerformUpgrade(){
//}