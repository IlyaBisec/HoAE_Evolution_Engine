#include "stdheader.h"
#include "CurrentMapOptions.h"
#include "vui_Actions.h"
#include <sys/stat.h>
#include <sys/types.h>
#include "IResourceManager.h"
#include "MassivePlayer.h"
#include "QuadsRender.h"
#include "UndoMaster.h"

bool Loadingmap=0;
void CreateTotalLocking();
void CreateMapLocking();
extern char MapScenaryDLL[200];
void InitLayersSystem();
void CleanPool();
void SaveBinaryData(ResFile F);
void LoadBinaryData(ResFile F);

//------------Saving&Loading 3D map & textures--------
void SaveHeader(ResFile f1){
	int i='PMD3'+ADDSH-1;
	RBlockWrite(f1,&i,4);
	i=VertInLine;
	RBlockWrite(f1,&i,4);
	i=MaxTH;
	RBlockWrite(f1,&i,4);
};
//extern int MinMapX;
//extern int MaxMapX;
//extern int MinMapY;
//extern int MaxMapY;
void SaveMapSize(ResFile f1){
	int MinMapX = 0, MinMapY = 0, MaxMapX = 0, MaxMapY = 0; // for stub
	int i='MPSZ';
	RBlockWrite(f1,&i,4);
	i=4+4*4;
	RBlockWrite(f1,&i,4);
	RBlockWrite(f1,&MinMapX,4);
	RBlockWrite(f1,&MinMapY,4);
	RBlockWrite(f1,&MaxMapX,4);
	RBlockWrite(f1,&MaxMapY,4);
};
void LoadMapSize(ResFile f1){
	int MinMapX, MinMapY, MaxMapX, MaxMapY; // for stub
	RBlockRead(f1,&MinMapX,4);
	RBlockRead(f1,&MinMapY,4);
	RBlockRead(f1,&MaxMapX,4);
	RBlockRead(f1,&MaxMapY,4);
};
void FreeArrays();
void SetupArrays();
void NewMap(int szX,int szY);
void CleanArrays();
extern byte* RivDir;

bool LoadHeader(ResFile f1){
	int i;
	RBlockRead(f1,&i,4);
	int ADDX=i-'PMD3'+1;
	if(ADDX<1||ADDX>3)return false;
	RBlockRead(f1,&i,4);
	RBlockRead(f1,&i,4);
	if((!RivDir)||ADDSH!=ADDX){
		ADDSH=ADDX;
		//FreeArrays();
		SetupArrays();
	};
	msx=240<<ADDSH;
	msy=240<<ADDSH;
	void SetInitialScrollLimitations();
	SetInitialScrollLimitations();
	return true;
};
bool xLoadHeader(ResFile f1){
	int i;
	RBlockRead(f1,&i,4);
	int ADDX=i-'PMD3'+1;
	if(ADDX<1||ADDX>3)return false;
	RBlockRead(f1,&i,4);
	RBlockRead(f1,&i,4);
	return true;
};
#ifdef STARFORCE
extern HGLOBAL PTR_LAND;
#endif
void SaveSurface(ResFile f1){
	int i='FRUS';
	RBlockWrite(f1,&i,4);
	i=4+(MaxTH+1)*MaxTH*2;
	RBlockWrite(f1,&i,4);
	RBlockWrite(f1,THMap,i-4);
};
extern int RivNX;
extern byte* RivVol;
void SelectSurfaceType(bool New);
extern short* DeepMap;
void LoadSurface(ResFile f1){
	SelectSurfaceType(false);
	RBlockRead(f1,THMap,(MaxTH+1)*MaxTH*2);
	memcpy(DeepMap,THMap,(MaxTH+1)*MaxTH*2);
	memset(RivDir,0,RivNX*RivNX);
	memset(RivVol,0,RivNX*RivNX);
	#ifdef STARFORCE
	if(!(PTR_LAND&&((DWORD*)PTR_LAND)[0]==0x4344EA34)){
		int N=(MaxTH+1)*MaxTH;
		for(int i=0;i<N;i++)THMap[i]=rand()&63;
	};
	#endif
};
void SaveLock(ResFile f1){
	
};
extern BlockBars LockBars;
extern BlockBars UnLockBars;
extern BlockBars HorceLockBars;
void LoadLock(ResFile f1){
	
};
void SaveLockNew(ResFile f1){
	int i='2COL';
	RBlockWrite(f1,&i,4);
	i=4+4+4+4+((LockBars.NBars+UnLockBars.NBars+HorceLockBars.NBars)<<2);
	RBlockWrite(f1,&i,4);
	RBlockWrite(f1,&LockBars.NBars,4);
	RBlockWrite(f1,&UnLockBars.NBars,4);
	RBlockWrite(f1,&HorceLockBars.NBars,4);
	if(LockBars.NBars)RBlockWrite(f1,LockBars.BC,LockBars.NBars<<2);
	if(UnLockBars.NBars)RBlockWrite(f1,UnLockBars.BC,UnLockBars.NBars<<2);
	if(HorceLockBars.NBars)RBlockWrite(f1,HorceLockBars.BC,HorceLockBars.NBars<<2);
};
void LoadLockNew(ResFile f1){
	LockBars.Clear();
	UnLockBars.Clear();
	RBlockRead(f1,&LockBars.NBars,4);
	RBlockRead(f1,&UnLockBars.NBars,4);
	LockBars.BC=new BlockCell[LockBars.NBars];
	UnLockBars.BC=new BlockCell[UnLockBars.NBars];
	RBlockRead(f1,LockBars.BC,LockBars.NBars<<2);
	RBlockRead(f1,UnLockBars.BC,UnLockBars.NBars<<2);
	LockBars.MaxBars=LockBars.NBars;
	UnLockBars.MaxBars=UnLockBars.NBars;
};
void LoadLockNew2(ResFile f1){
	LockBars.Clear();
	UnLockBars.Clear();
	HorceLockBars.Clear();
	RBlockRead(f1,&LockBars.NBars,4);
	RBlockRead(f1,&UnLockBars.NBars,4);
	RBlockRead(f1,&HorceLockBars.NBars,4);
	LockBars.BC=new BlockCell[LockBars.NBars];
	UnLockBars.BC=new BlockCell[UnLockBars.NBars];
	HorceLockBars.BC=new BlockCell[HorceLockBars.NBars];
	RBlockRead(f1,LockBars.BC,LockBars.NBars<<2);
	RBlockRead(f1,UnLockBars.BC,UnLockBars.NBars<<2);
	RBlockRead(f1,HorceLockBars.BC,HorceLockBars.NBars<<2);
	LockBars.MaxBars=LockBars.NBars;
	UnLockBars.MaxBars=UnLockBars.NBars;
	HorceLockBars.MaxBars=HorceLockBars.NBars;
};
void SaveTiles(ResFile f1){
	int i='ELIT';
	RBlockWrite(f1,&i,4);
	i=4+(MaxTH+1)*MaxTH;
	RBlockWrite(f1,&i,4);
	RBlockWrite(f1,TexMap,i-4);
};
void LoadTiles(ResFile f1){
	int N=(MaxTH+1)*MaxTH;
	RBlockRead(f1,TexMap,N);
	for(int i=0;i<N;i++){
		word tf=TexFlags[TexMap[i]];
		//if(tf&TEX_PLAIN)AddTHMap[i]=0;
		//else if(tf&TEX_HARD)AddTHMap[i]=rand()&15;
		//else AddTHMap[i]=rand()&7;
	};
};
#ifdef _USE3D
extern byte* TexMapEx;
extern byte* WTexMapEx;
void SaveTilesEx(ResFile f1){
	
};
void LoadTilesEx(ResFile f1){
	
};
#endif //_USE3D
void SaveSect(ResFile f1){
};

void LoadSect(ResFile f1){	
};
#ifdef _USE3D
extern byte* FactureMap;
extern byte* FactureWeight;
void SaveFactures(ResFile f1){
	if(FactureMap){
		int i='PAMF';
		RBlockWrite(f1,&i,4);
		int sz=VertInLine*MaxTH;
		i=4+2*sz;
		RBlockWrite(f1,&i,4);
		RBlockWrite(f1,FactureMap,sz);
		RBlockWrite(f1,FactureWeight,sz);
	};
};
void LoadFactures(ResFile f1){
	if(FactureMap){
		int sz=VertInLine*MaxTH;
		RBlockRead(f1,FactureMap,sz);
		RBlockRead(f1,FactureWeight,sz);
	};
};
#endif //_USE3D
extern SprGroup SPECIAL;
extern SprGroup WALLS;
void SaveSprites(ResFile f1){
	//FILE* F1=fopen("SPR.DAT","w");
	int ns=0;
	for(int i=0;i<MaxSprt;i++){
		if(Sprites[i].Enabled)ns++;
	};
	i='1ERT';
	RBlockWrite(f1,&i,4);
	i=(ns*14)+8;
	word j='GA';
	RBlockWrite(f1,&i,4);
	RBlockWrite(f1,&ns,4);
	for(i=0;i<MaxSprt;i++){
		OneSprite* OS=&Sprites[i];
		if(OS->Enabled){
			j=0;
			if(OS->SG==&TREES)j='GA';else
			if(OS->SG==&STONES)j='TS';else
			if(OS->SG==&HOLES)j='OH';else
			if(OS->SG==&COMPLEX)j='OC';
			if(OS->SG==&SPECIAL)j='SP';
			if(OS->SG==&ANMSPR)j='AS';
			if(OS->SG==&WALLS)j='WL';
			RBlockWrite(f1,&j,2);
			RBlockWrite(f1,&OS->x,4);
			RBlockWrite(f1,&OS->y,4);
			RBlockWrite(f1,&OS->SGIndex,2);
			word N=OS->NIndex;
			if(OS->Locking){
				N|=(word(OS->Locking)<<12);
			}
			RBlockWrite(f1,&N,2);
			//fprintf(F1,"%d %d %d %d %d\n",i,j,OS->x,OS->y,OS->SGIndex);
		};
	};
	//fclose(F1);
};
void SaveSprites2(ResFile f1){
	//FILE* F1=fopen("SPR.DAT","w");
	int ns=0;
	int nm=0;
	for(int i=0;i<MaxSprt;i++){
		if(Sprites[i].Enabled){
			ns++;
			if(Sprites[i].M4)nm++;
		}
	};
	i='2ERT';
	RBlockWrite(f1,&i,4);
	i=(ns*15)+nm*sizeof(Matrix4D)+8;
	word j='GA';
	RBlockWrite(f1,&i,4);
	RBlockWrite(f1,&ns,4);
	for(i=0;i<MaxSprt;i++){
		OneSprite* OS=&Sprites[i];
		if(OS->Enabled){
			j=0;
			if(OS->SG==&TREES)j='GA';else
			if(OS->SG==&STONES)j='TS';else
			if(OS->SG==&HOLES)j='OH';else
			if(OS->SG==&COMPLEX)j='OC';
			if(OS->SG==&SPECIAL)j='SP';
			if(OS->SG==&ANMSPR)j='AS';
			if(OS->SG==&WALLS)j='WL';
			RBlockWrite(f1,&j,2);
			RBlockWrite(f1,&OS->x,4);
			RBlockWrite(f1,&OS->y,4);
			RBlockWrite(f1,&OS->SGIndex,2);
			word N=OS->NIndex;
			if(OS->Locking){
				N|=(word(OS->Locking)<<12);
			}
			byte V=OS->M4!=0;			
			RBlockWrite(f1,&N,2);
			RBlockWrite(f1,&V,1);
			if(V){
				RBlockWrite(f1,OS->M4,sizeof(Matrix4D));
			}			
			//fprintf(F1,"%d %d %d %d %d\n",i,j,OS->x,OS->y,OS->SGIndex);
		};
	};
	//fclose(F1);
};
void addSpriteAnyway(int x,int y,SprGroup* SG,word id);
void DeleteAllSprites();
extern bool SpriteSuccess;
extern int LastAddSpr;
void CheckMemoryIntegrity();
void LoadSprites2(ResFile f1){
	void ClearExtraHeightObjs();
	ClearExtraHeightObjs();
	DeleteAllSprites();
	int ns;
	RBlockRead(f1,&ns,4);
	int x,y;
	word GSIND,sign,NIND;
	for(int i=0;i<MaxSprt;i++)Sprites[i].Enabled=false;
	for(i=0;i<ns;i++){
		RBlockRead(f1,&sign,2);
		RBlockRead(f1,&x,4);
		RBlockRead(f1,&y,4);
		RBlockRead(f1,&GSIND,2);
		RBlockRead(f1,&NIND,2);
		byte V;
		RBlockRead(f1,&V,1);
		Matrix4D* M4=NULL;
		if(V){
			M4=new Matrix4D;
			RBlockRead(f1,M4,sizeof Matrix4D);
		}
		if(sign=='GA')
			addSpriteAnyway(x,y,&TREES,GSIND);
		else
		if(sign=='TS')
			addSpriteAnyway(x,y,&STONES,GSIND);
		else
		if(sign=='OH')
			addSpriteAnyway(x,y,&HOLES,GSIND);
		else
		if(sign=='OC'){
			addSpriteAnyway(x,y,&COMPLEX,GSIND);
		}else
		if(sign=='SP')
			addSpriteAnyway(x,y,&SPECIAL,GSIND);
		else
		if(sign=='WL')
			addSpriteAnyway(x,y,&WALLS,GSIND);
		else
		if(sign=='AS')
			addSpriteAnyway(x,y,&ANMSPR,GSIND);
		if(SpriteSuccess){
			OneSprite* OS=Sprites+LastAddSpr;
			OS->NIndex=NIND&4095;
			OS->Locking|=NIND>>12;
			if(M4){
				if(OS->OC->HaveAligning){
                    delete(M4);
					M4=NULL;
				}else{
                    delete(OS->M4);
					OS->M4=M4;
					M4=NULL;
				}
			}
			//if(OS->M4){
			//	if(OS->OC->ViewType==1){
			//		void AddExtraHeightObject(int x,int y,int id,Matrix4D* m4);
			//		AddExtraHeightObject(OS->x,OS->y,OS->OC->ModelManagerID,OS->M4);
			//	}
			//}
			if(M4)delete(M4);
			if(OS->CurrentModIndex!=-1){
				int GetRXY(int V1,int V2,int x,int y);
                OS->Color=MulDWORD(OS->OC->TreeMods[OS->CurrentModIndex]->Color,GetRXY(256-16,256+16,OS->x,OS->y));
            }
		}
	}
}
void LoadSprites1(ResFile f1){
	DeleteAllSprites();
	int ns;
	RBlockRead(f1,&ns,4);
	int x,y;
	word GSIND,sign,NIND;
	for(int i=0;i<MaxSprt;i++)Sprites[i].Enabled=false;
	for(i=0;i<ns;i++){
		RBlockRead(f1,&sign,2);
		RBlockRead(f1,&x,4);
		RBlockRead(f1,&y,4);
		RBlockRead(f1,&GSIND,2);
		RBlockRead(f1,&NIND,2);
		if(sign=='GA')
			addSpriteAnyway(x,y,&TREES,GSIND);
		else
		if(sign=='TS')
			addSpriteAnyway(x,y,&STONES,GSIND);
		else
		if(sign=='OH')
			addSpriteAnyway(x,y,&HOLES,GSIND);
		else
		if(sign=='OC'){
			addSpriteAnyway(x,y,&COMPLEX,GSIND);
		}else
		if(sign=='SP')
			addSpriteAnyway(x,y,&SPECIAL,GSIND);
		else
		if(sign=='WL')
			addSpriteAnyway(x,y,&WALLS,GSIND);
		else
		if(sign=='AS')
			addSpriteAnyway(x,y,&ANMSPR,GSIND);
		if(SpriteSuccess){
			Sprites[LastAddSpr].NIndex=NIND&4095;
			Sprites[LastAddSpr].Locking|=NIND>>12;            
		}
	};
};
void LoadSprites(ResFile f1){
	DeleteAllSprites();
	int ns;
	RBlockRead(f1,&ns,4);
	int x,y;
	word GSIND,sign;
	for(int i=0;i<MaxSprt;i++)Sprites[i].Enabled=false;
	for(i=0;i<ns;i++){
		RBlockRead(f1,&sign,2);
		RBlockRead(f1,&x,4);
		RBlockRead(f1,&y,4);
		RBlockRead(f1,&GSIND,2);
		//RBlockRead(f1,&NIND,2);
		if(sign=='GA')
			addSpriteAnyway(x,y,&TREES,GSIND);
		else
		if(sign=='TS')
			addSpriteAnyway(x,y,&STONES,GSIND);
		else
		if(sign=='OH')
			addSpriteAnyway(x,y,&HOLES,GSIND);
		else
		if(sign=='OC')
			addSpriteAnyway(x,y,&COMPLEX,GSIND);
		else
		if(sign=='SP')
			addSpriteAnyway(x,y,&SPECIAL,GSIND);
		else
		if(sign=='AS')
			addSpriteAnyway(x,y,&ANMSPR,GSIND);
		//if(SpriteSuccess)Sprites[LastAddSpr].NIndex=NIND;
	};
};
void DeleteAllUnits(){
	extern bool InCloseObjects;
	InCloseObjects=1;
	for(int i=0;i<ULIMIT;i++){
		OneObject* OB=Group[i];
		if(OB){
			OB->ClearOrders();
			OB->DeletePath();
			OB->Nat->CITY->UnRegisterNewUnit(OB);
			DelObject(OB);
			if(OB->NInside){
				free(OB->Inside);
			};
			delete(Group[i]);
			Group[i]=NULL;
		};
	};
	void ClearAllComplexObjects();
	ClearAllComplexObjects();
	InCloseObjects=0;
};
void SaveUnits(ResFile f1){
	int i='TINU';
	RBlockWrite(f1,&i,4);
	int NU=0;
	for(i=0;i<ULIMIT;i++){
		OneObject* OB=Group[i];
		if(OB&&!OB->Wall)NU++;
	};
	i=8+NU*48;
	RBlockWrite(f1,&i,4);
	RBlockWrite(f1,&NU,4);
	for(i=0;i<ULIMIT;i++){
		OneObject* OB=Group[i];
		if(OB&&!OB->Wall){
			OneObject* OB=Group[i];
			GeneralObject* GO=OB->Ref.General;
			NewMonster* NM=OB->newMons;
			char Name[33];
			memset(Name,0,33);
			strcpy(Name,GO->MonsterID);
			RBlockWrite(f1,&OB->NNUM,1);
			RBlockWrite(f1,&OB->NIndex,2);
			RBlockWrite(f1,&OB->RealX,4);
			RBlockWrite(f1,&OB->RealY,4);
			RBlockWrite(f1,&OB->Life,2);
			RBlockWrite(f1,&OB->Stage,2);
			RBlockWrite(f1,Name,33);
		};
	};
};
void SaveUnits2(ResFile f1){
	int i='2INU';
	RBlockWrite(f1,&i,4);
	int NU=0;
	for(i=0;i<ULIMIT;i++){
		OneObject* OB=Group[i];
		if(OB&&!OB->Wall)NU++;
	};
	i=8+NU*52;
	RBlockWrite(f1,&i,4);
	RBlockWrite(f1,&NU,4);
	for(i=0;i<ULIMIT;i++){
		OneObject* OB=Group[i];
		if(OB&&!OB->Wall){
			OneObject* OB=Group[i];
			GeneralObject* GO=OB->Ref.General;
			NewMonster* NM=OB->newMons;
			char Name[33];
			memset(Name,0,33);
			strcpy(Name,GO->MonsterID);
			RBlockWrite(f1,&OB->NNUM,1);
			RBlockWrite(f1,&OB->NIndex,2);
			RBlockWrite(f1,&OB->RealX,4);
			RBlockWrite(f1,&OB->RealY,4);
			RBlockWrite(f1,&OB->Life,2);
			RBlockWrite(f1,&OB->Stage,2);
			RBlockWrite(f1,&OB->WallX,2);
			RBlockWrite(f1,&OB->WallY,2);
			RBlockWrite(f1,Name,33);
		};
	};
};
void SaveUnits3(ResFile f1){
	int i='3INU';
	RBlockWrite(f1,&i,4);
	int NU=0;
	for(i=0;i<ULIMIT;i++){
		OneObject* OB=Group[i];
		if(OB&&!(OB->Wall||OB->Sdoxlo||OB->NotSavable))NU++;
	};
	i=8+NU*54;
	RBlockWrite(f1,&i,4);
	RBlockWrite(f1,&NU,4);
	for(i=0;i<ULIMIT;i++){
		OneObject* OB=Group[i];
		if(OB&&!(OB->Wall||OB->Sdoxlo||OB->NotSavable)){
			OneObject* OB=Group[i];
			GeneralObject* GO=OB->Ref.General;
			NewMonster* NM=OB->newMons;
			char Name[33];
			memset(Name,0,33);
			strcpy(Name,GO->MonsterID);
			RBlockWrite(f1,&OB->NNUM,1);
			RBlockWrite(f1,&OB->NIndex,2);
			Vector3D GetSelectionBarPos(OneObject* OB);
			Vector3D V1=GetSelectionBarPos(OB);
			int rx=V1.x*16;
			int ry=V1.y*16;
			RBlockWrite(f1,&rx,4);
			RBlockWrite(f1,&ry,4);
			RBlockWrite(f1,&OB->Life,2);
			word ST=OB->MoreCharacter->ProduceStages==OB->Stage?OB->Stage:0xFFFF-OB->Stage;
			RBlockWrite(f1,&ST,2);
			RBlockWrite(f1,&OB->WallX,2);
			RBlockWrite(f1,&OB->WallY,2);
			RBlockWrite(f1,&OB->RealDir,1);
			byte Opt=0;
			if(OB->StandGround)Opt|=1;
			if(OB->NoSearchVictim)Opt|=2;
#ifdef SIMPLEMANAGE
			if(OB->ArmAttack)Opt|=4;
			if(OB->RifleAttack)Opt|=8;
#endif //SIMPLEMANAGE
			//if(OB->Illusion)Opt|=16;
			if(OB->AbRes==0xFFFF)Opt|=32;
			Opt|=(OB->ActivityState)<<6;
			RBlockWrite(f1,&Opt,1);
			RBlockWrite(f1,Name,33);
		};
	};
};
void SaveUnits4(ResFile f1){
	int i='4INU';
	RBlockWrite(f1,&i,4);
	int NU=0;
	for(i=0;i<ULIMIT;i++){
		OneObject* OB=Group[i];
		if(OB&&!(OB->Wall||( OB->Sdoxlo && !OB->Hidden )||OB->NotSavable))NU++;
	};
	i=8+NU*58;
	RBlockWrite(f1,&i,4);
	RBlockWrite(f1,&NU,4);
	for(i=0;i<ULIMIT;i++){
		OneObject* OB=Group[i];
		if(OB&&!(OB->Wall||( OB->Sdoxlo && !OB->Hidden )||OB->NotSavable)){
			OneObject* OB=Group[i];
			GeneralObject* GO=OB->Ref.General;
			NewMonster* NM=OB->newMons;
			char Name[33];
			memset(Name,0,33);
			strcpy(Name,GO->MonsterID);
			RBlockWrite(f1,&OB->NNUM,1);
			RBlockWrite(f1,&OB->NIndex,2);
			Vector3D GetSelectionBarPos(OneObject* OB);
			Vector3D V1=GetSelectionBarPos(OB);
			int rx=V1.x*16;
			int ry=V1.y*16;
			RBlockWrite(f1,&rx,4);
			RBlockWrite(f1,&ry,4);
			RBlockWrite(f1,&OB->Life,2);
			RBlockWrite(f1,&OB->UniqID,4);
			word ST=OB->MoreCharacter->ProduceStages==OB->Stage?OB->Stage:0xFFFF-OB->Stage;
			RBlockWrite(f1,&ST,2);
			RBlockWrite(f1,&OB->WallX,2);
			RBlockWrite(f1,&OB->WallY,2);
			RBlockWrite(f1,&OB->RealDir,1);
			byte Opt=0;
			if(OB->StandGround)Opt|=1;
			if(OB->NoSearchVictim)Opt|=2;
#ifdef SIMPLEMANAGE
			if(OB->ArmAttack)Opt|=4;
			if(OB->RifleAttack)Opt|=8;
#endif //SIMPLEMANAGE
			//if(OB->Illusion)Opt|=16;
			if(OB->AbRes==0xFFFF)Opt|=32;
            if(OB->Sdoxlo)Opt|=64;
			Opt|=(OB->ActivityState)<<6;
			RBlockWrite(f1,&Opt,1);
			RBlockWrite(f1,Name,33);
		};
	};
};
void SaveInsideUnits(ResFile f1){
	int s='INUN';
	RBlockWrite(f1,&s,4);
	int N=0;
	int sz=8;
	for(int i=0;i<MAXOBJECT;i++){
        OneObject* OB=Group[i];
		if(OB && OB->NInside && OB->Inside){
            sz+=4+2+OB->NInside*4;
			N++;
		}
	}
	RBlockWrite(f1,&sz,4);
    RBlockWrite(f1,&N,4);
	for(int i=0;i<MAXOBJECT;i++){
        OneObject* OB=Group[i];
		if(OB && OB->NInside && OB->Inside){
            RBlockWrite(f1,&OB->UniqID,4);
			RBlockWrite(f1,&OB->NInside,2);
			for(int j=0;j<OB->NInside;j++){
				DWORD UID=0xFFFFFFFF;
				if(OB->Inside[j]!=0xFFFF){
					OneObject* OBJ=Group[OB->Inside[j]];
					if(OBJ){
                        UID=OBJ->UniqID;
					}
				}
				RBlockWrite(f1,&UID,4);
			}			
		}
	}
}
OneObject* _getByUniqUD(DWORD ID){
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB && ID==OB->UniqID)return OB;
	}
	return NULL;
}
void LoadInsideUnits(ResFile f1){
	int N=0;
	RBlockRead(f1,&N,4);
	for(int i=0;i<N;i++){
		DWORD ID=0xFFFFFFFF;
		RBlockRead(f1,&ID,4);
		word n;
		RBlockRead(f1,&n,2);
		OneObject* OB=_getByUniqUD(ID);
		if(OB){
			OB->NInside=n;
			OB->Inside=znew(word,n);			
		}
		for(int j=0;j<n;j++){
            DWORD UID=0xFFFFFFFF;
			RBlockRead(f1,&UID,4);
			OneObject* IOB=_getByUniqUD(UID);
			if(OB && IOB){
				if(OB->newMons->ShowInsideUnits){
                    IOB->LockedInBuilding=OB->Index;
				}else{
					IOB->Sdoxlo = true;
					IOB->Hidden = true;
				}
				OB->Inside[j]=IOB->Index;
			}
		}
	}
}
void ClearHints();
int LASTADDID;
extern bool NoGround;
void CreateNewUnitAt(byte NI,int x,int y,word Type,word Life,word Stage){
	LASTADDID=0xFFFF;
	Nation* NT=&NATIONS[NI];
	NoGround=1;
	int ID=NT->CreateNewMonsterAt(x,y,Type,true);
	NoGround=0;
	if(ID!=-1){
		LASTADDID=ID;
		OneObject* OB=Group[ID];
		if(OB->NewBuilding){
			OB->Stage=0;
			for(int i=0;i<Stage;i++)OB->NextStage();
			OB->Ready=true;
		}else{
			OB->Life=Life;
			OB->Ready=true;
		};
	}
	ClearHints();
};
extern bool GroundBox;
OneObject* CreateNewUnitAt3(byte NI,int x,int y,word Type,word Life,word Stage,byte Dir){
	Nation* NT=&NATIONS[NI];
	GroundBox=0;
	NoGround=1;
	int ID=NT->CreateNewMonsterAt(x,y,Type,true,Dir);
	GroundBox=1;
	NoGround=0;
	if(ID!=-1){
		OneObject* OB=Group[ID];
		if(OB->NewBuilding){
			if(Stage<0x8000){
				OB->Stage=0;
				int NS=OB->MoreCharacter->ProduceStages;
				for(int i=0;i<NS;i++)OB->NextStage();
				OB->Ready=true;
			}else{
				int NS=0xFFFF-Stage;
				for(int i=0;i<NS;i++)OB->NextStage();
			}
			ClearHints();
		}else{
			OB->Life=OB->MoreCharacter->Life;
			OB->Ready=true;
#ifdef COSSACKS2
			OB->GroupSpeed=OB->newMons->MotionDist;
#endif //COSSACKS2
		};

		return OB;
	};
	return NULL;
};
void LoadUnits(ResFile f1){
	DeleteAllUnits();
	int NU;
	RBlockRead(f1,&NU,4);
	for(int i=0;i<NU;i++){
		byte NI;
		word Life,NIndex,Stage;
		int x,y;
		char Name[33];
		RBlockRead(f1,&NI,1);
		RBlockRead(f1,&NIndex,2);
		RBlockRead(f1,&x,4);
		RBlockRead(f1,&y,4);
		RBlockRead(f1,&Life,2);
		RBlockRead(f1,&Stage,2);
		RBlockRead(f1,Name,33);
		//search for NIndex
		Nation* NT=&NATIONS[NI];		
		bool found=false;
		GeneralObject* GO;
		do{
			i++;
			GO=NT->Mon[i];
			if(!strcmp(GO->MonsterID,Name))found=true;
		}while((!found)&&i<NT->NMon-1);

		if(found&&!GO->newMons->Wall){
			CreateNewUnitAt(NI,x,y,i,Life,Stage);
		};
	};
};
extern int NNations;
void TestUnits(ResFile f1){
	int NU;
	RBlockRead(f1,&NU,4);
	for(int i=0;i<NU;i++){
		byte NI;
		word Life,NIndex,Stage;
		int x,y;
		char Name[33];
		RBlockRead(f1,&NI,1);
		RBlockRead(f1,&NIndex,2);
		RBlockRead(f1,&x,4);
		RBlockRead(f1,&y,4);
		RBlockRead(f1,&Life,2);
		RBlockRead(f1,&Stage,2);
		RBlockRead(f1,Name,33);
		for(int j=0;j<NNations;j++){
			if(strstr(Name,NatCharLo[j])||strstr(Name,NatCharHi[j]))LOADNATMASK|=1<<j;
		};
	};
};
extern int PortBuiX;
extern int PortBuiY;
void LoadUnits2(ResFile f1){
	DeleteAllUnits();
	int NU;
	RBlockRead(f1,&NU,4);
	for(int i=0;i<NU;i++){
		byte NI;
		word Life,NIndex,Stage;
		int x,y;
		short wx,wy;
		char Name[33];
		RBlockRead(f1,&NI,1);
		RBlockRead(f1,&NIndex,2);
		RBlockRead(f1,&x,4);
		RBlockRead(f1,&y,4);
		RBlockRead(f1,&Life,2);
		RBlockRead(f1,&Stage,2);
		RBlockRead(f1,&wx,2);
		RBlockRead(f1,&wy,2);
		RBlockRead(f1,Name,33);
		//search for NIndex
		Nation* NT=&NATIONS[NI];
		PortBuiX=wx;
		PortBuiY=wy;		
		bool found=false;
		GeneralObject* GO;
		do{
			i++;
			GO=NT->Mon[i];
			if(!strcmp(GO->MonsterID,Name))found=true;
		}while((!found)&&i<NT->NMon-1);

		if(found&&!GO->newMons->Wall){
			CreateNewUnitAt(NI,x,y,i,Life,Stage);
		};
	};
};
void TestUnits2(ResFile f1){
	int NU;
	RBlockRead(f1,&NU,4);
	for(int i=0;i<NU;i++){
		byte NI;
		word Life,NIndex,Stage;
		int x,y;
		short wx,wy;
		char Name[33];
		RBlockRead(f1,&NI,1);
		RBlockRead(f1,&NIndex,2);
		RBlockRead(f1,&x,4);
		RBlockRead(f1,&y,4);
		RBlockRead(f1,&Life,2);
		RBlockRead(f1,&Stage,2);
		RBlockRead(f1,&wx,2);
		RBlockRead(f1,&wy,2);
		RBlockRead(f1,Name,33);
		for(int j=0;j<NNations;j++){
			if(strstr(Name,NatCharLo[j])||strstr(Name,NatCharHi[j]))LOADNATMASK|=1<<j;
		};
	};
};
OneObject* CreateNewUnitAt3(byte NI,int x,int y,word Type,word Life,word Stage);
void TestCGARB();
void LoadUnits3(ResFile f1){
	DeleteAllUnits();
	int NU;
	RBlockRead(f1,&NU,4);
	NewMonster* NMS[1024];
	int NNms=0;
	byte NUSAGE[1024];
	memset(NUSAGE,0,sizeof NUSAGE);
	for(int q=0;q<NU;q++){
		byte NI;
		word Life,NIndex,Stage;
		int x,y;
		short wx,wy;
		char Name[33];
		RBlockRead(f1,&NI,1);
		RBlockRead(f1,&NIndex,2);
		RBlockRead(f1,&x,4);
		RBlockRead(f1,&y,4);
		RBlockRead(f1,&Life,2);
		RBlockRead(f1,&Stage,2);
		RBlockRead(f1,&wx,2);
		RBlockRead(f1,&wy,2);
		byte DIR,Opt;
		RBlockRead(f1,&DIR,1);
		RBlockRead(f1,&Opt,1);
		RBlockRead(f1,Name,33);
		//search for NIndex
		Nation* NT=&NATIONS[NI];
		PortBuiX=wx;
		PortBuiY=wy;
		int i=-1;
		bool found=false;
		GeneralObject* GO;		
		if(NI<8){
			do{
				i++;
				if(i<NT->NMon){
					GO=NT->Mon[i];
					if(!strcmp(GO->MonsterID,Name))found=true;
				};
			}while((!found)&&i<NT->NMon-1);

			if(found&&!GO->newMons->Wall){
				OneObject* OB=CreateNewUnitAt3(NI,x,y,i,Life,Stage,DIR);
				if(OB){
					//OB->RealDir=DIR;
					//OB->GraphDir=DIR;
					OB->StandGround=bool(Opt&1);
					OB->NoSearchVictim=Opt&2;
#ifdef SIMPLEMANAGE
					OB->ArmAttack=bool(Opt&4);
					OB->RifleAttack=bool(Opt&8);
					//if(!(OB->newMons->Peasant||OB->NewBuilding))OB->Illusion=1;//bool(Opt&16);
					if(Opt&32)OB->AbRes=0xFFFF;
					OB->ActivityState=(Opt>>6)&3;
					//if(OB->newMons->ArmAttack)
					OB->NoSearchVictim=0;
#endif //SIMPLEMANAGE
					int idx=OB->NIndex;
					NUSAGE[idx>>3]|=1<<(idx&7);					
				};
			};
		};
	};
	void SetupCamera();
	SetupCamera();	
	for(int i=0;i<1024;i++){
		for(int j=0;j<8;j++){
			if(NUSAGE[i]&(1<<j)){
				NewMonster* NM=NATIONS->Mon[i*8+j]->newMons;
				for(int q=0;q<NM->Animations->ANM.GetAmount();q++){
					NewAnimation* NA=NM->Animations->ANM[q];
					if(NA->Enabled){
						extern float fMapX;
						extern float fMapY;
						if(NA->ModelID>0 && NA->AnimationType==1){
                            NA->DrawAt(0,0,fMapX+RealLx/2,fMapY+RealLx/2,0,0,1,0,0,0,NULL);
						}
						if(NA->ModelID<=0&&NA->NFrames&&NA->Frames.GetAmount()){						
							if(NA->Frames[0]->FileID!=0xFFFF)GPS.LoadGP(NA->Frames[0]->FileID);
							//if(NA->DoubleAnm)GPS.LoadGP(NA->Frames[NA->NFrames]->FileID);
						}
					}
				}
			}
		}
	}
	IRS->ClearDevice();
	
	void SetMonstersInCells();
	SetMonstersInCells();
	void RefreshBiuldings();
	RefreshBiuldings();
};
void LoadUnits4(ResFile f1){
	DeleteAllUnits();
	int NU;
	RBlockRead(f1,&NU,4);
	NewMonster* NMS[1024];
	int NNms=0;
	byte NUSAGE[1024];
	memset(NUSAGE,0,sizeof NUSAGE);
	for(int q=0;q<NU;q++){
		byte NI;
		word Life,NIndex,Stage;
		int x,y;
		short wx,wy;
		char Name[33];
		DWORD UID;
		RBlockRead(f1,&NI,1);
		RBlockRead(f1,&NIndex,2);
		RBlockRead(f1,&x,4);
		RBlockRead(f1,&y,4);
		RBlockRead(f1,&Life,2);
		RBlockRead(f1,&UID,4);
		RBlockRead(f1,&Stage,2);
		RBlockRead(f1,&wx,2);
		RBlockRead(f1,&wy,2);
		byte DIR,Opt;
		RBlockRead(f1,&DIR,1);
		RBlockRead(f1,&Opt,1);
		RBlockRead(f1,Name,33);
		//search for NIndex
		Nation* NT=&NATIONS[NI];
		PortBuiX=wx;
		PortBuiY=wy;
		int i=-1;
		bool found=false;
		GeneralObject* GO;		
		if(NI<8){
			do{
				i++;
				if(i<NT->NMon){
					GO=NT->Mon[i];
					if(GO->MonsterID && !strcmp(GO->MonsterID,Name))found=true;
                };
			}while((!found)&&i<NT->NMon-1);

            if(!found)Log.Warning("Loading map warning: unit type %s not found",Name);

			if(found&&!GO->newMons->Wall){
				OneObject* OB=CreateNewUnitAt3(NI,x,y,i,Life,Stage,DIR);
				if(OB){
					//OB->RealDir=DIR;
					//OB->GraphDir=DIR;
					OB->StandGround=bool(Opt&1);
					OB->NoSearchVictim=Opt&2;
					OB->UniqID=UID;
#ifdef SIMPLEMANAGE
					OB->ArmAttack=bool(Opt&4);
					OB->RifleAttack=bool(Opt&8);
					//if(!(OB->newMons->Peasant||OB->NewBuilding))OB->Illusion=1;//bool(Opt&16);
					if(Opt&32)OB->AbRes=0xFFFF;
					OB->ActivityState=(Opt>>6)&3;
					//if(OB->newMons->ArmAttack)
					OB->NoSearchVictim=0;
#endif //SIMPLEMANAGE
                    if(Opt&64){
                        OB->Sdoxlo=true;
                        OB->Hidden=true;
                    }
					int idx=OB->NIndex;
					NUSAGE[idx>>3]|=1<<(idx&7);					
				};
			};
		};
	};
	for(int i=0;i<1024;i++){
		for(int j=0;j<8;j++){
			if(NUSAGE[i]&(1<<j)){
				NewMonster* NM=NATIONS->Mon[i*8+j]->newMons;
				for(int q=0;q<NM->Animations->ANM.GetAmount();q++){
					NewAnimation* NA=NM->Animations->ANM[q];
					if(NA->Enabled&&NA->ModelID<=0&&NA->NFrames&&NA->Frames.GetAmount()){						
						if(NA->Frames[0]->FileID!=0xFFFF)GPS.LoadGP(NA->Frames[0]->FileID);
						//if(NA->DoubleAnm)GPS.LoadGP(NA->Frames[NA->NFrames]->FileID);
					}
				}
			}
		}
	}
	void SetMonstersInCells();
	SetMonstersInCells();
	void RefreshBiuldings();
	RefreshBiuldings();
	void SetupCamera();
	SetupCamera();	
    extern bool EditMapMode;
    if(EditMapMode==false || (GetKeyState(VK_CONTROL)&0x8000)){
	    for(int i=0;i<1024;i++){
		    for(int j=0;j<8;j++){
			    if(NUSAGE[i]&(1<<j)){
				    NewMonster* NM=NATIONS->Mon[i*8+j]->newMons;
					if(NM->Building){
						NewAnimation* NA=NM->Animations->Get(anm_StandLo);
						if(NA->Enabled){
							extern float fMapX;
							extern float fMapY;
							if(NA->ModelID>0 && NA->AnimationType==1){
								NA->DrawAt(0,0,fMapX+RealLx/2,fMapY+RealLx/2,0,0,1,0,0,0,NULL);
							}
						}
					}else{
						for(int q=0;q<NM->Animations->ANM.GetAmount();q++){
							NewAnimation* NA=NM->Animations->ANM[q];
							if(NA->Enabled){
								extern float fMapX;
								extern float fMapY;
								if(NA->ModelID>0 && NA->AnimationType==1){
									NA->DrawAt(0,0,fMapX+RealLx/2,fMapY+RealLx/2,0,0,1,0,0,0,NULL);
								}
								if(NA->ModelID<=0&&NA->NFrames&&NA->Frames.GetAmount()){						
									if(NA->Frames[0]->FileID!=0xFFFF)GPS.LoadGP(NA->Frames[0]->FileID);
									//if(NA->DoubleAnm)GPS.LoadGP(NA->Frames[NA->NFrames]->FileID);
								}
							}
						}
					}
			    }
		    }
	    }
    }
	IRS->ClearDevice();
};
void TestUnits3(ResFile f1){
	int NU;
	RBlockRead(f1,&NU,4);
	for(int i=0;i<NU;i++){
		byte NI;
		word Life,NIndex,Stage;
		int x,y;
		short wx,wy;
		char Name[33];
		RBlockRead(f1,&NI,1);
		RBlockRead(f1,&NIndex,2);
		RBlockRead(f1,&x,4);
		RBlockRead(f1,&y,4);
		RBlockRead(f1,&Life,2);
		RBlockRead(f1,&Stage,2);
		RBlockRead(f1,&wx,2);
		RBlockRead(f1,&wy,2);
		byte DIR,Opt;
		RBlockRead(f1,&DIR,1);
		RBlockRead(f1,&Opt,1);
		RBlockRead(f1,Name,33);
		for(int j=0;j<NNations;j++){
			if(strstr(Name,NatCharLo[j])||strstr(Name,NatCharHi[j]))LOADNATMASK|=1<<j;
		};
	};
};
void SaveNewWalls(ResFile f1){	
};
void LoadNewWalls(ResFile f1){	
};

void SaveNewWallsV1(ResFile f1){
	
};
void LoadNewWallsV1(ResFile f1){
	
};
void SaveNewWallsV2(ResFile f1){
	
};
void LoadNewWallsV2(ResFile f1){
	
};
void SaveGates(ResFile f1){
	
};
void LoadGates(ResFile f1){
	
};
extern byte* WaterBright;
void SaveWaterCost(ResFile f1){
	return;
	int i='2AES';
	RBlockWrite(f1,&i,4);
	int Lx=msx+2;
	int Ly=msy+2;
	i=12+Lx*Ly*2;
	RBlockWrite(f1,&i,4);
	RBlockWrite(f1,&Lx,4);
	RBlockWrite(f1,&Ly,4);
	int dsx=(MaxWX);
	int pos=0;
	for(i=0;i<Ly;i++){
		RBlockWrite(f1,WaterDeep+pos,Lx);
		RBlockWrite(f1,WaterBright+pos,Lx);
		pos+=dsx;
	};
};
void LoadWaterCost(ResFile f1){
	int i,Lx,Ly;
	RBlockRead(f1,&Lx,4);
	RBlockRead(f1,&Ly,4);
	int dsx=(MaxWX);
	int pos=0;
	for(i=0;i<Ly;i++){
		RBlockRead(f1,WaterDeep+pos,Lx);
		RBlockRead(f1,WaterBright+pos,Lx);
		pos+=dsx;
	};
};
extern int RES[8][8];
void SaveRES(ResFile f1){
	int i='USER';
	RBlockWrite(f1,&i,4);
	i=sizeof(RES)+4;
	RBlockWrite(f1,&i,4);
	RBlockWrite(f1,&(RES[0][0]),i-4);
};

void LoadRES(ResFile f1){
	int i=sizeof(RES);
	RBlockRead(f1,&(RES[0][0]),i);
	for(int p=0;p<8;p++){
		for(int q=0;q<8;q++){
			SetXRESRC(p,q,RES[p][q]);
		};
	};
	for(i=0;i<8;i++){
		Nation* NT=NATIONS+i;
		NT->SELO.Init();
		NT->ARMY.Init();
		NT->SCIENCE.Init();
		for(int j=0;j<8;j++){
			NT->GENERAL.RESAM[j]=XRESRC(i,j);
			NT->GENERAL.RESRM[j]=0;
		};
	};		
};

void SaveZonesAndGroups(ResFile f1){
	int i='2NOZ';
	RBlockWrite(f1,&i,4);
	xmlQuote xml1;
	AGroups.Save(xml1,&AGroups);
	xmlQuote xml2;
	AZones.Save(xml2,&AZones);
	DString D1;
	xml1.GetXMLSource(&D1);
	DString D2;
	xml2.GetXMLSource(&D2);
	int sz1=strlen(D1.pchar())+1;
	int sz2=strlen(D2.pchar())+1;
	int sz=sz1+sz2+8+4;
	RBlockWrite(f1,&sz,4);
	RBlockWrite(f1,&sz1,4);
	RBlockWrite(f1,D1.pchar(),sz1);
	RBlockWrite(f1,&sz2,4);
	RBlockWrite(f1,D2.pchar(),sz2);	
};
void SaveMapOptions(ResFile f1){
	int i='TPOM';
	RBlockWrite(f1,&i,4);
	xmlQuote xml1;
	MOptions.Save(xml1,&MOptions);	
	DString D1;
	xml1.GetXMLSource(&D1);	
	int sz1=strlen(D1.pchar())+1;	
	int sz=sz1+4+4;
	RBlockWrite(f1,&sz,4);
	RBlockWrite(f1,&sz1,4);
	RBlockWrite(f1,D1.pchar(),sz1);
};
void LoadMapOptions(ResFile f1){	
	int sz1;
	RBlockRead(f1,&sz1,4);
	char* s=new char[sz1];
	RBlockRead(f1,s,sz1);	
	xmlQuote xml1;
	xml1.Load(s);
	delete[](s);
	ErrorPager err(0);
	MOptions.reset_class(&MOptions);	
	MOptions.Load(xml1,&MOptions,&err);
//	MOptions.LimitUnitsInSettlementRemoveDups(); // Not needed!
};
struct GNUC_struct{
	int Type;
	int xc,yc;
	int MinR;
	int Index;
};
bool GetNearUnitCallback(OneObject* OB,void* Param){
	GNUC_struct* GS=(GNUC_struct*)Param;
	int r=Norma(GS->xc-(OB->RealX>>4),GS->yc-(OB->RealY>>4));
	if(r<GS->MinR){
		GS->MinR=r;
		GS->Index=OB->Index;
	}
	return true;
}
bool GetNearUnitCallbackEx(OneObject* OB,void* Param){	
	GNUC_struct* GS=(GNUC_struct*)Param;
    //int fb=NATIONS->Mon[GS->Type]->FlagBearerID;
	if(OB->BrigadeID!=0xFFFF)return false;
	int r=Norma(GS->xc-(OB->RealX>>4),GS->yc-(OB->RealY>>4));	
	if(r>64)return false;//&&(GS->Type!=0xFFFF&&!(OB->NIndex==GS->Type/*||fb==OB->NIndex*/)))return false;	
	if(r<GS->MinR){
		GS->MinR=r;
		GS->Index=OB->Index;
	}
	return true;
}
typedef bool tpUnitsCallback(OneObject* OB,void* param);
int PerformActionOverUnitsInRadius(int xc,int yc,int R,tpUnitsCallback* CB,void* Param);
int PerformActionOverBuildingsInRadius(int xc,int yc,int R,tpUnitsCallback* CB,void* Param);
int FindUnitByCoor(int x,int y){
	GNUC_struct temp;
	temp.Index=-1;
	temp.MinR=100000;
	temp.xc=x>>4;
	temp.yc=y>>4;
	PerformActionOverUnitsInRadius(x>>4,y>>4,300,&GetNearUnitCallback,(void*)&temp);
	PerformActionOverBuildingsInRadius(x>>4,y>>4,300,&GetNearUnitCallback,(void*)&temp);
	return temp.Index;
};
int FindUnitByCoorEx(int x,int y){
	GNUC_struct temp;
	temp.Index=-1;
	temp.MinR=1000;
	temp.xc=x>>4;
	temp.yc=y>>4;
	temp.Type=0xFFFF;//Type;
	PerformActionOverUnitsInRadius(x>>4,y>>4,300,&GetNearUnitCallbackEx,(void*)&temp);
	PerformActionOverBuildingsInRadius(x>>4,y>>4,300,&GetNearUnitCallbackEx,(void*)&temp);
	return temp.Index;
};
void LoadZonesAndGroups(ResFile f1){
	int NAZones,NAGroups;
	RBlockRead(f1,&NAZones,4);
	RBlockRead(f1,&NAGroups,4);
	for(int i=0;i<NAZones;i++){
		ActiveZone* AZ=new ActiveZone;
		AZones.Add(AZ);
		int x,y,r,t;
		RBlockRead(f1,&x,4);
		RBlockRead(f1,&y,4);
		RBlockRead(f1,&r,4);
		RBlockRead(f1,&t,4);
		RBlockRead(f1,&t,1);
		AZ->x=x;
		AZ->y=y;
		AZ->R=r;
		AZ->Dir=0;
		byte L;
		RBlockRead(f1,&L,1);
		char* s=znew(char,L);
		RBlockRead(f1,s,L);
		AZ->Name=s;
		free(s);
	};	
	for(int q=0;q<NAGroups;q++){
		int tmp[13];
		ActiveGroup* AG=new ActiveGroup;
		AGroups.Add(AG);
		RBlockRead(f1,tmp,49);
		byte L;
		RBlockRead(f1,&L,1);
		char* s=znew(char,L);
		RBlockRead(f1,s,L);
		AG->Name=s;
		free(s);
		for(int i=0;i<tmp[2];i++){
			int x,y;
			RBlockRead(f1,&x,4);
			RBlockRead(f1,&y,4);
			int id=FindUnitByCoor(x,y);
			if(id!=-1){
				OneObject* OB=Group[id];
				OneUS U;
				U.ID=id;
				U.SN=OB->Serial;
				AG->Units.Add(U);
			}else{
				OneUS U;
				U.ID=0xFFFF;
				U.SN=0xFFFF;
				AG->Units.Add(U);				
			};
		};
	};
};
void LoadZonesAndGroupsNew(ResFile f1){	
	int sz1,sz2;
	RBlockRead(f1,&sz1,4);
	char* s=new char[sz1+1];
	RBlockRead(f1,s,sz1);
	s[sz1]=0;
	xmlQuote xml1;
	xml1.Load(s);
	delete[](s);
	ErrorPager err;
	AGroups.Load(xml1,&AGroups,&err);
	for(int i=0;i<AGroups.GetAmount();i++){
		ActiveGroup* AG=AGroups[i];
		AG->Visible=true;
	}

	RBlockRead(f1,&sz2,4);
	s=new char[sz2+1];
	RBlockRead(f1,s,sz2);
	s[sz2]=0;
	xmlQuote xml2;
	xml2.Load(s);
	AZones.Load(xml2,&AZones,&err);
	delete[](s);
};
extern City CITY[8];
void SaveFormations(ResFile f1){
	int i='MROF';
	RBlockWrite(f1,&i,4);
	int sz=4+4;
	int NB=0;
	for(i=0;i<8;i++){
		Brigade* BR=CITY[i].Brigs;
		for(int j=0;j<MaxBrig;j++){
			if(BR->Enabled&&BR->WarType){
				NB++;
				sz+=8+sizeof(BR->AddDamage)+sizeof(BR->AddShield)+sizeof(BR->BM)
					+sizeof(BR->Direction)+sizeof(BR->NMemb)+sizeof(BR->WarType)
					+sizeof(BR->SN)+sizeof(BR->MembID);
				sz+=4*BR->NMemb;
				if(BR->NMemb){
					sz+=BR->NMemb*8;
				};
			};
			BR++;
		};
	};
	RBlockWrite(f1,&sz,4);
	RBlockWrite(f1,&NB,4);
	for(i=0;i<8;i++){
		Brigade* BR=CITY[i].Brigs;
		for(int j=0;j<MaxBrig;j++){
			if(BR->Enabled&&BR->WarType){
				RBlockWrite(f1,&i,4);
				RBlockWrite(f1,&j,4);
				RBlockWrite(f1,&BR->AddDamage,sizeof BR->AddDamage);
				RBlockWrite(f1,&BR->AddShield,sizeof BR->AddShield);
				RBlockWrite(f1,&BR->BM,sizeof BR->BM);
				RBlockWrite(f1,&BR->Direction,sizeof BR->Direction);
				RBlockWrite(f1,&BR->NMemb,sizeof BR->NMemb);
				RBlockWrite(f1,&BR->WarType,sizeof BR->WarType);
				RBlockWrite(f1,&BR->SN,sizeof BR->SN);
				RBlockWrite(f1,&BR->MembID,sizeof BR->MembID);
				if(BR->NMemb){
					for(int k=0;k<BR->NMemb;k++)RBlockWrite(f1,&BR->posX[k],2);
					for(k=0;k<BR->NMemb;k++)RBlockWrite(f1,&BR->posY[k],2);
					for(int j=0;j<BR->NMemb;j++){
						word MID=BR->Memb[j];
						if(MID!=0xFFFF){
							OneObject* OB=Group[MID];
							if(OB&&OB->Serial==BR->MembSN[j]){
								RBlockWrite(f1,&OB->RealX,4);
								RBlockWrite(f1,&OB->RealY,4);
							}else{
								int p=-1;
								RBlockWrite(f1,&p,4);
								RBlockWrite(f1,&p,4);
							};
						}else{
							int p=-1;
							RBlockWrite(f1,&p,4);
							RBlockWrite(f1,&p,4);
						};
					};
				};
			};
			BR++;
		};
	};
};
void LoadFormations(ResFile f1){
	int NB;
	RBlockRead(f1,&NB,4);
	bool oldvers=0;
	word* tmp;
	byte* tmp1;
	if(NB){
		tmp=znew(word,NB);
		tmp1=znew(byte,NB);
	};
	for(int i=0;i<NB;i++){
		int bid,nat;
		RBlockRead(f1,&nat,4);
		RBlockRead(f1,&bid,4);
		tmp[i]=bid;
		tmp1[i]=nat;
		Brigade* BR=CITY[nat].Brigs+bid;
		memset(BR,0,sizeof Brigade);
		BR->Enabled=true;
		BR->ArmyID=0xFFFF;
		RBlockRead(f1,&BR->AddDamage,sizeof BR->AddDamage);
		RBlockRead(f1,&BR->AddShield,sizeof BR->AddShield);
		RBlockRead(f1,&BR->BM,sizeof BR->BM);
		RBlockRead(f1,&BR->Direction,sizeof BR->Direction);
		RBlockRead(f1,&BR->NMemb,sizeof BR->NMemb);
		RBlockRead(f1,&BR->WarType,sizeof BR->WarType);
		RBlockRead(f1,&BR->SN,sizeof BR->SN);
		RBlockRead(f1,&BR->MembID,sizeof BR->MembID);
		if(BR->NMemb-NBPERSONAL!=ElementaryOrders[BR->WarType-1].NUnits)oldvers=1;
		BR->MaxMemb=BR->NMemb;
		BR->CT=CITY+nat;
		BR->ID=bid;
		memset(&BR->BM,0,sizeof BR->BM);
		if(BR->NMemb){
			BR->PosCreated=1;
			BR->posX=znew(int,BR->NMemb);
			BR->posY=znew(int,BR->NMemb);
			BR->Memb=znew(word,BR->NMemb);
			BR->MembSN=znew(word,BR->NMemb);
			for(int k=0;k<BR->NMemb;k++){
				BR->posX[k]=0;
				RBlockRead(f1,&BR->posX[k],2);
			};
			for(k=0;k<BR->NMemb;k++){
				BR->posY[k]=0;
				RBlockRead(f1,&BR->posY[k],2);
			};
			for(int q=0;q<BR->NMemb;q++){
				int x,y;
				RBlockRead(f1,&x,4);
				RBlockRead(f1,&y,4);
				if(x==-1){
					BR->Memb[q]=0xFFFF;
					BR->MembSN[q]=0xFFFF;
				}else{
					int ID=FindUnitByCoorEx(x,y);
					if(ID==-1){
						BR->Memb[q]=0xFFFF;
						BR->MembSN[q]=0xFFFF;
					}else{
						OneObject* OB=Group[ID];
						BR->Memb[q]=OB->Index;
						BR->MembSN[q]=OB->Serial;
						OB->InArmy=true;
						OB->BrigadeID=bid;
						OB->BrIndex=q;
						if(EngSettings.MoraleType!=1){
							OB->AddDamage=BR->AddDamage;
							OB->AddShield=BR->AddShield;
						}
						if(q>1){
							BR->MembID=OB->NIndex;
						};
						(&BR->BM.Peons)[GetBMIndex(OB)]++;
					};
				};
			};
#ifdef NEWMORALE
			int GetMaxMorale(Brigade* BR);
			BR->MaxMorale=GetMaxMorale(BR);
			BR->Morale=BR->MaxMorale;
			BR->PrevMorale=BR->Morale;
#endif
		};
		CITY[nat].NBrigs=bid+1;
	};
	if(oldvers){
		for(i=0;i<NB;i++){
			CITY[tmp1[i]].Brigs[tmp[i]].WarType--;
		};
	};
	if(NB){
		free(tmp);
		free(tmp1);
	};
};
void SaveFormationsNew(ResFile f1){
	int i='1ROF';
	RBlockWrite(f1,&i,4);
	int sz=4+4;
	int NB=0;
	for(i=0;i<8;i++){
		Brigade* BR=CITY[i].Brigs;
		for(int j=0;j<MaxBrig;j++){
			if(BR->Enabled&&BR->WarType){
				NB++;
				sz+=8+sizeof(BR->AddDamage)+sizeof(BR->AddShield)+sizeof(BR->BM)
					 +sizeof(BR->Direction)+sizeof(BR->NMemb)+32
					 +sizeof(BR->SN)+sizeof(BR->MembID);
				sz+=4*BR->NMemb;
				if(BR->NMemb){
					sz+=BR->NMemb*8;
				};
			};
			BR++;
		};
	};
	RBlockWrite(f1,&sz,4);
	RBlockWrite(f1,&NB,4);
	char FRMNM[32];
	for(i=0;i<8;i++){
		Brigade* BR=CITY[i].Brigs;
		for(int j=0;j<MaxBrig;j++){
			if(BR->Enabled&&BR->WarType){
				RBlockWrite(f1,&i,4);
				RBlockWrite(f1,&j,4);
				RBlockWrite(f1,&BR->AddDamage,sizeof BR->AddDamage);
				RBlockWrite(f1,&BR->AddShield,sizeof BR->AddShield);
				RBlockWrite(f1,&BR->BM,sizeof BR->BM);
				RBlockWrite(f1,&BR->Direction,sizeof BR->Direction);
				RBlockWrite(f1,&BR->NMemb,sizeof BR->NMemb);
				memset(FRMNM,0,32);
				OrderDescription* ODE=ElementaryOrders+BR->WarType-1;
				strcpy(FRMNM,ODE->ID);
				//RBlockWrite(f1,&BR->WarType,sizeof BR->WarType);
				RBlockWrite(f1,FRMNM,32);
				RBlockWrite(f1,&BR->SN,sizeof BR->SN);
				RBlockWrite(f1,&BR->MembID,sizeof BR->MembID);
				if(BR->NMemb){
					for(int k=0;k<BR->NMemb;k++)RBlockWrite(f1,&BR->posX[k],2);
					for( k=0;k<BR->NMemb;k++)RBlockWrite(f1,&BR->posY[k],2);
					for(int j=0;j<BR->NMemb;j++){
						word MID=BR->Memb[j];
						if(MID!=0xFFFF){
							OneObject* OB=Group[MID];
							if(OB&&OB->Serial==BR->MembSN[j]){
								RBlockWrite(f1,&OB->RealX,4);
								RBlockWrite(f1,&OB->RealY,4);
							}else{
								int p=-1;
								RBlockWrite(f1,&p,4);
								RBlockWrite(f1,&p,4);
							};
						}else{
							int p=-1;
							RBlockWrite(f1,&p,4);
							RBlockWrite(f1,&p,4);
						};
					};
				};
			};
			BR++;
		};
	};
};
void SaveFormationsNewC2(ResFile f1){
#ifdef COSSACKS2
	int i='2ROF';
	RBlockWrite(f1,&i,4);
	int sz=4+4;
	int NB=0;
	for(i=0;i<8;i++){
		Brigade* BR=CITY[i].Brigs;
		for(int j=0;j<MaxBrig;j++){
			if(BR->Enabled&&BR->WarType){
				NB++;
				sz+=8+sizeof(BR->AddDamage)+sizeof(BR->AddShield)+sizeof(BR->BM)
					 +sizeof(BR->Direction)+sizeof(BR->NMemb)+32
					 +sizeof(BR->SN)+sizeof(BR->MembID)+sizeof(BR->OfficerID)+
					 +sizeof(BR->BarID)+sizeof(BR->FlagID);;
				sz+=4*BR->NMemb;
				if(BR->NMemb){
					sz+=BR->NMemb*8;
				};
			};
			BR++;
		};
	};
	RBlockWrite(f1,&sz,4);
	RBlockWrite(f1,&NB,4);
	char FRMNM[32];
	for(i=0;i<8;i++){
		Brigade* BR=CITY[i].Brigs;
		for(int j=0;j<MaxBrig;j++){
			if(BR->Enabled&&BR->WarType){
				RBlockWrite(f1,&i,4);
				RBlockWrite(f1,&j,4);
				RBlockWrite(f1,&BR->AddDamage,sizeof BR->AddDamage);
				RBlockWrite(f1,&BR->AddShield,sizeof BR->AddShield);
				RBlockWrite(f1,&BR->BM,sizeof BR->BM);
				RBlockWrite(f1,&BR->Direction,sizeof BR->Direction);
				RBlockWrite(f1,&BR->NMemb,sizeof BR->NMemb);
				memset(FRMNM,0,32);
				OrderDescription* ODE=ElementaryOrders+BR->WarType-1;
				strcpy(FRMNM,ODE->ID);
				//RBlockWrite(f1,&BR->WarType,sizeof BR->WarType);
				RBlockWrite(f1,FRMNM,32);
				RBlockWrite(f1,&BR->SN,sizeof BR->SN);
				RBlockWrite(f1,&BR->MembID,sizeof BR->MembID);
				RBlockWrite(f1,&BR->OfficerID,sizeof BR->OfficerID);
				RBlockWrite(f1,&BR->BarID,sizeof BR->BarID);
				RBlockWrite(f1,&BR->FlagID,sizeof BR->FlagID);
				if(BR->NMemb){
					for(int k=0;k<BR->NMemb;k++)RBlockWrite(f1,&BR->posX[k],2);
					for( k=0;k<BR->NMemb;k++)RBlockWrite(f1,&BR->posY[k],2);
					for(int j=0;j<BR->NMemb;j++){
						word MID=BR->Memb[j];
						if(MID!=0xFFFF){
							OneObject* OB=Group[MID];
							if(OB&&OB->Serial==BR->MembSN[j]){
								RBlockWrite(f1,&OB->RealX,4);
								RBlockWrite(f1,&OB->RealY,4);
							}else{
								int p=-1;
								RBlockWrite(f1,&p,4);
								RBlockWrite(f1,&p,4);
							};
						}else{
							int p=-1;
							RBlockWrite(f1,&p,4);
							RBlockWrite(f1,&p,4);
						};
					};
				};
			};
			BR++;
		};
	};
#endif //COSSACKS2
};
void LoadFormationsNew(ResFile f1){
	int NB;
	RBlockRead(f1,&NB,4);
	char FRMNM[32];
	for(int i=0;i<NB;i++){
		int bid,nat;
		RBlockRead(f1,&nat,4);
		RBlockRead(f1,&bid,4);
		Brigade* BR=CITY[nat].Brigs+bid;
		memset(BR,0,sizeof Brigade);
		BR->Enabled=true;
		BR->ArmyID=0xFFFF;
		RBlockRead(f1,&BR->AddDamage,sizeof BR->AddDamage);
		RBlockRead(f1,&BR->AddShield,sizeof BR->AddShield);
		RBlockRead(f1,&BR->BM,sizeof BR->BM);
		RBlockRead(f1,&BR->Direction,sizeof BR->Direction);
		RBlockRead(f1,&BR->NMemb,sizeof BR->NMemb);
		RBlockRead(f1,FRMNM,32);
		int wt=-1;
		for(int j=0;j<NEOrders&&wt==-1;j++){
			if(!strcmp(FRMNM,ElementaryOrders[j].ID))wt=j;
		};
		//assert(wt!=-1);
		BR->WarType=wt+1;
		//RBlockRead(f1,&BR->WarType,sizeof BR->WarType);
		RBlockRead(f1,&BR->SN,sizeof BR->SN);
		RBlockRead(f1,&BR->MembID,sizeof BR->MembID);
		BR->MaxMemb=BR->NMemb;
		BR->CT=CITY+nat;
		BR->ID=bid;
		if(BR->NMemb){
			BR->PosCreated=1;
			BR->posX=znew(int,BR->NMemb);
			BR->posY=znew(int,BR->NMemb);
			BR->Memb=znew(word,BR->NMemb);
			BR->MembSN=znew(word,BR->NMemb);
			for(int k=0;k<BR->NMemb;k++){
				BR->posX[k]=0;
				RBlockRead(f1,&BR->posX[k],2);
			};
			for(k=0;k<BR->NMemb;k++){
				BR->posY[k]=0;
				RBlockRead(f1,&BR->posY[k],2);
			};
			for(int q=0;q<BR->NMemb;q++){
				int x,y;
				RBlockRead(f1,&x,4);
				RBlockRead(f1,&y,4);
				if(x==-1){
					BR->Memb[q]=0xFFFF;
					BR->MembSN[q]=0xFFFF;
				}else{
					int ID=FindUnitByCoorEx(x,y);
					if(ID==-1){
						BR->Memb[q]=0xFFFF;
						BR->MembSN[q]=0xFFFF;
					}else{
						OneObject* OB=Group[ID];
						BR->Memb[q]=OB->Index;
						BR->MembSN[q]=OB->Serial;
						OB->InArmy=true;
						OB->BrigadeID=bid;
						OB->BrIndex=q;
						if(EngSettings.MoraleType!=1){
							OB->AddDamage=BR->AddDamage;
							OB->AddShield=BR->AddShield;
						}
						if(q>1){
							byte use=OB->newMons->Usage;
#ifndef COSSACKS2
							BR->Strelki=(use==StrelokID||OB->newMons->Artilery);
#endif
						};
						if(q>1){
							BR->MembID=OB->NIndex;
						};
					};
				};
			};
#ifdef NEWMORALE
			int GetMaxMorale(Brigade* BR);
			BR->MaxMorale=GetMaxMorale(BR);
			BR->Morale=BR->MaxMorale;
			BR->PrevMorale=BR->Morale;
#endif
		};
	};
};
void LoadFormationsNewC2(ResFile f1){
#ifdef COSSACKS2
	int NB;
	void SetMonstersInCells();
	SetMonstersInCells();
	RBlockRead(f1,&NB,4);
	char FRMNM[32];
	for(int i=0;i<NB;i++){
		int bid,nat;
		RBlockRead(f1,&nat,4);
		RBlockRead(f1,&bid,4);
		Brigade* BR=CITY[nat].Brigs+bid;
		memset(BR,0,sizeof Brigade);
		BR->Enabled=false;
		BR->ArmyID=0xFFFF;
		RBlockRead(f1,&BR->AddDamage,sizeof BR->AddDamage);
		RBlockRead(f1,&BR->AddShield,sizeof BR->AddShield);
		RBlockRead(f1,&BR->BM,sizeof BR->BM);
		RBlockRead(f1,&BR->Direction,sizeof BR->Direction);
		RBlockRead(f1,&BR->NMemb,sizeof BR->NMemb);
		RBlockRead(f1,FRMNM,32);
		int wt=-1;
		for(int j=0;j<NEOrders&&wt==-1;j++){
			if(!strcmp(FRMNM,ElementaryOrders[j].ID))wt=j;
		};
		//assert(wt!=-1);
		BR->WarType=wt+1;
		//RBlockRead(f1,&BR->WarType,sizeof BR->WarType);
		RBlockRead(f1,&BR->SN,sizeof BR->SN);
		RBlockRead(f1,&BR->MembID,sizeof BR->MembID);
		RBlockRead(f1,&BR->OfficerID,sizeof BR->OfficerID);
		RBlockRead(f1,&BR->BarID,sizeof BR->BarID);
		RBlockRead(f1,&BR->FlagID,sizeof BR->FlagID);
		BR->MaxMemb=BR->NMemb;
		BR->CT=CITY+nat;
		BR->ID=bid;
        BR->AttackRateBonus=100;
		BR->AddAttackRange=0;
		BR->ExpGrowSpeed=100;
        BR->UnitsSpeedBonus=100;				
		CITY[nat].NBrigs=bid+1;
		if(BR->NMemb){
			BR->PosCreated=1;
			BR->posX=znew(int,BR->NMemb);
			BR->posY=znew(int,BR->NMemb);
			BR->Memb=znew(word,BR->NMemb);
			BR->MembSN=znew(word,BR->NMemb);
			for(int k=0;k<BR->NMemb;k++){
				BR->posX[k]=0;
				RBlockRead(f1,&BR->posX[k],2);
			};
			for(k=0;k<BR->NMemb;k++){
				BR->posY[k]=0;
				RBlockRead(f1,&BR->posY[k],2);
			};
			for(int q=0;q<BR->NMemb;q++){
				int x,y;
				RBlockRead(f1,&x,4);
				RBlockRead(f1,&y,4);
				if(x==-1){
					BR->Memb[q]=0xFFFF;
					BR->MembSN[q]=0xFFFF;
				}else{	
					int ID=-1;
					if(q<NBPERSONAL){
						if(q==0)ID=FindUnitByCoorEx(x,y);//,BR->OfficerID);
						if(q==1)ID=FindUnitByCoorEx(x,y);//,BR->BarID);
						if(q==2)ID=FindUnitByCoorEx(x,y);//,BR->FlagID);
					}else{
						ID=FindUnitByCoorEx(x,y);//,BR->MembID);
					}
					if(ID==-1){
						BR->Memb[q]=0xFFFF;
						BR->MembSN[q]=0xFFFF;
					}else{
						BR->Enabled=true;
						OneObject* OB=Group[ID];
						BR->Memb[q]=OB->Index;
						BR->MembSN[q]=OB->Serial;
						OB->InArmy=true;
						OB->BrigadeID=bid;
						OB->BrIndex=q;
						if(OB->StandGround)BR->InStandGround=1;
						if(EngSettings.MoraleType!=1){
							OB->AddDamage=BR->AddDamage;
							OB->AddShield=BR->AddShield;
						}
						if(q>1){
							byte use=OB->newMons->Usage;
#ifndef COSSACKS2
							BR->Strelki=(use==StrelokID||OB->newMons->Artilery);
#endif //COSSACKS2
						};
						//if(q>1){
							//BR->MembID=OB->NIndex;
						//};
					};
				};
			};
			if(BR->Memb[0]!=0xFFFF){
				OneObject* OB=Group[BR->Memb[0]];
				if(OB)BR->OfficerID=OB->NIndex;
			}
			if(BR->Memb[1]!=0xFFFF){
				OneObject* OB=Group[BR->Memb[0]];
				if(OB)BR->BarID=OB->NIndex;
			}
			if(BR->Memb[2]!=0xFFFF){
				OneObject* OB=Group[BR->Memb[0]];
				if(OB)BR->FlagID=OB->NIndex;
			}
			for(int i=NBPERSONAL;i<BR->NMemb;i++){
				word ID=BR->Memb[i];
				if(ID!=0xFFFF){
					OneObject* OB=Group[ID];
					if(OB&&OB->Ref.General->UsualFormID==0xFFFF){
						BR->MembID=OB->NIndex;
						break;
					}
				}
			}
#ifdef NEWMORALE
			int GetMaxMorale(Brigade* BR);
			BR->MaxMorale=GetMaxMorale(BR);
			BR->Morale=BR->MaxMorale;
			BR->PrevMorale=BR->Morale;
#endif
			//for(){
			//}
		};
	};
#endif //COSSACKS2
};
void SaveDLL(ResFile f1){
	int i='LLD.';
	RBlockWrite(f1,&i,4);
	i=4+200;
	RBlockWrite(f1,&i,4);
	RBlockWrite(f1,MapScenaryDLL,200);
};
void LoadDLL(ResFile f1){
	RBlockRead(f1,MapScenaryDLL,200);
};
extern int RM_LandType;
extern int RM_Resstart;
extern int RM_Restot;
void SaveAIParam(ResFile f1){
	int i='AIIA';
	RBlockWrite(f1,&i,4);
	i=4+12;
	RBlockWrite(f1,&i,4);
	RBlockWrite(f1,&RM_LandType,4);
	RBlockWrite(f1,&RM_Resstart,4);
	RBlockWrite(f1,&RM_Restot,4);
};
void LoadAIParam(ResFile f1){
	RBlockRead(f1,&RM_LandType,4);
	RBlockRead(f1,&RM_Resstart,4);
	RBlockRead(f1,&RM_Restot,4);
};
void CloseMap(ResFile f1){
	int i='MDNE';
	RBlockWrite(f1,&i,4);
	RClose(f1);
};
void CreateMapPreview(byte* Data,int Lx,int Ly);
char* MPNAME;
bool GetPreviewName(char* Name,char* ccc){
	if(Name[0]=='R'&&Name[1]=='N'&&Name[3]==' '){
		char cc1[64];
		char cc4[64];
		int x1,x2,x3;
		int z=sscanf(Name,"%s%x%x%s%d",cc1,&x1,&x2,cc4,&x3);
		if(z==5){
			strcpy(ccc,"Preview\\P");
			ccc[9]='0'+(x2&15);//Land type
			ccc[10]='0'+((x2>>20)&15);//climatic zone
			int N=0;
			for(int v=0;v<8;v++)if(cc4[v]!='0')N++;
			ccc[11]='0'+N;
			ccc[12]=0;
			return true;
		};
	};
	return false;
};
#define PREVIEWLX 400
#define PREVIEWLY 224
void SavePreview(ResFile f1){
	int i='2EIV';
	void CreateMiniMapWithUnits();
	void CreateMiniMap();
	CreateMiniMapWithUnits();	
	int GetMinimapTexture();
	int t=GetMinimapTexture();
	int pitch;
	byte* bits=IRS->LockTexBits(t,pitch);
	if(bits){
		RBlockWrite(f1,&i,4);
		i=4+240*pitch;
		RBlockWrite(f1,&i,4);
		RBlockWrite(f1,bits,240*pitch);
        IRS->UnlockTexBits(t);
	}
	CreateMiniMap();
};
int GetMinimapTexture();
void SavePreviewDirectly(ResFile f1){
	int i='2EIV';
	int t=GetMinimapTexture();
	int pitch;
	byte* bits=IRS->LockTexBits(t,pitch);
	if(bits){
		RBlockWrite(f1,&i,4);
		i=4+240*pitch;
		RBlockWrite(f1,&i,4);
		RBlockWrite(f1,bits,240*pitch);
		IRS->UnlockTexBits(t);
	}	
};
ResFile ForceOpenFileEx(char* Name){
	char tmp[512];
	int pos=0;
	char c;
	do{
		while((c=Name[pos])&&c!='\\'&&c!='/')pos++;
		if(c){
			memcpy(tmp,Name,pos);
			tmp[pos]=0;			
			CreateDirectory(tmp,0);
			pos++;
		};
	}while(c);	
	return RRewrite(Name);
};
void SaveSomething(ResFile f1,char* FileName,char* TempName){
	ResFile F=RReset(FileName);
	if(F!=INVALID_HANDLE_VALUE){
		int sz=RFileSize(F);
		byte* buf=new byte[sz];
		RBlockRead(F,buf,sz);
		RClose(F);
		int i='ELIF';
		RBlockWrite(f1,&i,4);
		int L=strlen(FileName)+1;
		int L1=strlen(TempName)+1;
		i=4+4+4+L+4+L1+sz;
		RBlockWrite(f1,&i,4);
		RBlockWrite(f1,&sz,4);
		RBlockWrite(f1,&L,4);
		RBlockWrite(f1,FileName,L);
		RBlockWrite(f1,&L1,4);
		RBlockWrite(f1,TempName,L1);
		RBlockWrite(f1,buf,sz);        
		delete[](buf);
	}
}
void LoadSomething(ResFile f1){
	int L,L1,sz;
	char fn[256];
	char fn1[256];
	RBlockRead(f1,&sz,4);
	RBlockRead(f1,&L,4);	
	RBlockRead(f1,fn,L);
	RBlockRead(f1,&L1,4);	
	RBlockRead(f1,fn1,L1);

	byte* Buf=new byte[sz];
	RBlockRead(f1,Buf,sz);
	ResFile F=ForceOpenFileEx(fn);
	if(F!=INVALID_HANDLE_VALUE){
		RBlockWrite(F,Buf,sz);
		RClose(F);
	}
	if(fn1[0]){
		F=ForceOpenFileEx(fn1);
		if(F!=INVALID_HANDLE_VALUE){
			RBlockWrite(F,Buf,sz);
			RClose(F);
		}
	}
	delete[](Buf);
}
#define PREVIEWLX 400
#define PREVIEWLY 224
void LoadPreview(ResFile f1,byte* Data){
	int GetMinimapTexture();
	int t=GetMinimapTexture();
	int pitch;
	byte* bits=IRS->LockTexBits(t,pitch);
	if(bits){
		RBlockRead(f1,bits,240*pitch);
		IRS->UnlockTexBits(t);
	}
};
void ClearSMS();
void LoadSMSInMap(ResFile F);
void SaveSMSInMap(ResFile F);
extern byte* NatDeals;
#define NATLX (TopLx>>1)
extern int PeaceTimeLeft;
extern byte CordonIDX[8];

extern byte BaloonState;
extern byte CannonState;
extern byte XVIIIState;

extern int MaxPeaceTime;
extern int PeaceTimeStage;
int GetPixDataToSave(byte** ptr);
void SetPixDataFromSave(byte* ptr);
void SavePixData(ResFile f1){
	/*
	int i='PIXD';
	RBlockWrite(f1,&i,4);
	byte* ptr;
	int sz=GetPixDataToSave(&ptr);
	i=8+sz;
	RBlockWrite(f1,&i,4);
	RBlockWrite(f1,&sz,4);
	RBlockWrite(f1,ptr,sz);
	free(ptr);
	*/
};
void LoadPixData(ResFile f1){
	int sz;
	byte* ptr;
	RBlockRead(f1,&sz,4);
	ptr=znew(byte,sz);
	RBlockRead(f1,ptr,sz);
	SetPixDataFromSave(ptr);
	free(ptr);
};
int SavePointTexInfo(ResFile F);
void LoadPointTexInfo(ResFile F);
void SaveGroundData(ResFile F){
	return;
	int i='GROU';
	RBlockWrite(F,&i,4);
	SavePointTexInfo(F);
};
void LoadGroundData(ResFile F){
	LoadPointTexInfo(F);
};
void SavePeaceTimeInfo(ResFile f1){
	if(NatDeals){
		int i='PEAC';
		RBlockWrite(f1,&i,4);
		int sz=4+NATLX*NATLX+4+8+3;
		RBlockWrite(f1,&sz,4);
		RBlockWrite(f1,NatDeals,NATLX*NATLX);
		RBlockWrite(f1,&PeaceTimeLeft,4);
		MaxPeaceTime=PeaceTimeLeft;
		PeaceTimeStage=PeaceTimeLeft/60;

		RBlockWrite(f1,CordonIDX,8);

		RBlockWrite(f1,&BaloonState,1);
		RBlockWrite(f1,&CannonState,1);
		RBlockWrite(f1,&XVIIIState,1);
	};
};
extern int RivNX;
extern byte* RivVol;
void SaveRivers(ResFile f1){
	return;
	int i='1VIR';
	RBlockWrite(f1,&i,4);
	i=RivNX*RivNX*2+4;
	RBlockWrite(f1,&i,4);
	RBlockWrite(f1,RivDir,RivNX*RivNX);
	RBlockWrite(f1,RivVol,RivNX*RivNX);
};
void LoadRivers(ResFile f1){
	RBlockRead(f1,RivDir,RivNX*RivNX);
	RBlockRead(f1,RivVol,RivNX*RivNX);
};
void LoadPeaceTimeInfo(ResFile f1){
	if(NatDeals)free(NatDeals);
	NatDeals=znew(byte,NATLX*NATLX);
	RBlockRead(f1,NatDeals,NATLX*NATLX);
	RBlockRead(f1,&PeaceTimeLeft,4);
	RBlockRead(f1,CordonIDX,8);
	RBlockWrite(f1,&BaloonState,1);
	RBlockWrite(f1,&CannonState,1);
	RBlockWrite(f1,&XVIIIState,1);
};

void SaveTopology(ResFile f1);
void SaveWTopology(ResFile f1);
void LoadTopology1(ResFile f1);
void LoadWTopology1(ResFile f1);
void SaveLightHash(ResFile F);

#include "FCompressor.h"
extern FCompressor FCOMP;
void CompressSB(SaveBuf* SB,char* dst){
	if(SB->Buf && SB->Size){
		IRM->CheckFileAccess(dst);
		unsigned char* dd=NULL;
		unsigned dLen=0;
		FCOMP.type=FCOMPRESSOR_TYPE_LZO;
		FCOMP.CompressBlock(&dd,&dLen,(unsigned char*)SB->Buf,SB->Size);		
		if(dd){
			ResFile F=RRewrite(dst);
			if(F!=INVALID_HANDLE_VALUE){
				RBlockWrite(F,dd,dLen);
				RClose(F);
			}
			free(dd);
		}
	}
}
void CompressFile(char* src,char* dst){
	
	IRM->CheckFileAccess(dst);
	CopyFile(src,dst,false);
	
	//return;

    ResFile F=RReset(src);
	if(F!=INVALID_HANDLE_VALUE){
		int sz=RFileSize(F);
		char* sd=new char[sz];
		RBlockRead(F,sd,sz);
        unsigned char* dd=NULL;
		unsigned dLen=0;
        //FCOMP.type=FCOMPRESSOR_TYPE_BZ2;
		FCOMP.type=FCOMPRESSOR_TYPE_LZO;
		FCOMP.CompressBlock(&dd,&dLen,(unsigned char*)sd,sz);
		RClose(F);
		if(dd){
			ResFile F=RRewrite(dst);
			if(F!=INVALID_HANDLE_VALUE){
				RBlockWrite(F,dd,dLen);
				RClose(F);
			}                        
            delete[](dd);
		}
	}
}
void DeCompressFile(char* src,char* dst){
	ResFile F=RReset(src);
	if(F!=INVALID_HANDLE_VALUE){
		int sz=RFileSize(F);
		char* sd=new char[sz];
		RBlockRead(F,sd,sz);
		unsigned char* dd=NULL;
		unsigned dLen=0;
		FCOMP.DecompressBlock(&dd,&dLen,(unsigned char*)sd);
		RClose(F);
		if(dd){
			ResFile F=RRewrite(dst);
			if(F!=INVALID_HANDLE_VALUE){
				RBlockWrite(F,dd,dLen);
				RClose(F);
			}                        
			delete[](dd);
		}
		delete[](sd);
	}
}
extern bool SaveMod;
void Save3DMap(char* Map){
	SaveMod=false;
	_chdir(IRM->GetHomeDirectory());
	void SaveMLayersToFileInCompactForm(char* filename);
	SaveMLayersToFileInCompactForm("Surface\\SurfaceShape.dat");
	MPNAME=Map;
	//EraseAreas();
	//ResFile f1=RRewrite(Map);
	ResFile f1=RRewrite("Temp\\$temp$.$$$");
	//if(IOresult())return;
	SaveHeader(f1);
	cmo_Preview* Prv=&MOptions.Preview;
	if(!Prv->UserPicture.isClear()){
		SaveSomething(f1,Prv->UserPicture.pchar(),Prv->Temp.pchar());
	}	
	SavePreview(f1);
	void SaveNewSurface(ResFile F);
	bool CheckIfNewTerrain();
	if(CheckIfNewTerrain()){
		SaveNewSurface(f1);
	}else{
		SaveSurface(f1);
		SaveTiles(f1);
		SaveSect(f1);	
	}
	SaveMapSize(f1);
	SaveUnits4(f1);
	SaveInsideUnits(f1);
	SaveSprites2(f1);
	SaveNewWallsV2(f1);
	SaveLockNew(f1);
	SaveWaterCost(f1);
	SaveRES(f1);
	SaveTopology(f1);
	SaveWTopology(f1);
	SaveGates(f1);
	SaveZonesAndGroups(f1);
	SaveMapOptions(f1);
#ifdef COSSACKS2
	SaveFormationsNewC2(f1);
#else
	SaveFormationsNew(f1);
#endif
	SaveDLL(f1);
	SaveAIParam(f1);
	SaveSMSInMap(f1);
	SavePeaceTimeInfo(f1);
	SaveRivers(f1);
	//SavePixData(f1);
	//SaveGroundData(f1);
#ifdef _USE3D
	SaveFactures(f1);
	GrassRenderer::SaveGrass(f1);
	void SaveRoadData(ResFile F);
	SaveRoadData(f1);
	//void SaveGSprites(ResFile F);
	//SaveGSprites(f1);
	//void SaveTilesEx(ResFile f1);
	//SaveTilesEx(f1);
	void SaveRoadsNet(ResFile SB);
	SaveRoadsNet(f1);
	//SaveLightHash(f1);
	//void SaveWaterMaps(ResFile F);
	//SaveWaterMaps(f1);
	SaveSomething(f1,"Surface\\SurfaceShape.dat","");
	void SaveNewTop(ResFile F,int type);
	void SaveAllXMLSToMap(ResFile f1);
	SaveAllXMLSToMap(f1);
	for(int i=0;i<NMFIELDS;i++)SaveNewTop(f1,i);	
#endif //_USE3D
	SaveBinaryData(f1);
	CloseMap(f1);
	CompressFile("Temp\\$temp$.$$$",Map);
	CleanPool();
	if(!SaveMod)MessageBox(NULL,"No one mod unit was saved!","Warning",0);
};
void RenameAnyway(char* src,char* dst){
	IRM->CheckFileAccess(src);
	IRM->CheckFileAccess(dst);
	DeleteFile(dst);
	rename(src,dst);
};
CEXPORT void RenameAnywayEx(char* src,char* dst){
	RenameAnyway(src,dst);
}
void FastSave3DMap(char* Map);
extern bool EditMapMode;
int ME_prevtime=0;
void ProcessMapAutosave(){
	if(!(EditMapMode||IsMassiveGame()))return;
	if(ME_prevtime==0){
		ME_prevtime=GetTickCount();
	};
	int T=GetTickCount();
	if(T-ME_prevtime>300000){
		ShowString(RealLx/2-GetRLCStrWidth("Map autosaving",&BigWhiteFont)/2,RealLy,"Map autosaving",&BigWhiteFont);
		GPS.FlushBatches();
		FlipPages();
		RenameAnyway("Temp\\MapAutosave.40min.m3d","Temp\\MapAutosave.45min.m3d");
		RenameAnyway("Temp\\MapAutosave.35min.m3d","Temp\\MapAutosave.40min.m3d");
		RenameAnyway("Temp\\MapAutosave.30min.m3d","Temp\\MapAutosave.35min.m3d");
		RenameAnyway("Temp\\MapAutosave.25min.m3d","Temp\\MapAutosave.30min.m3d");
		RenameAnyway("Temp\\MapAutosave.20min.m3d","Temp\\MapAutosave.25min.m3d");
		RenameAnyway("Temp\\MapAutosave.15min.m3d","Temp\\MapAutosave.20min.m3d");
		RenameAnyway("Temp\\MapAutosave.10min.m3d","Temp\\MapAutosave.15min.m3d");
		RenameAnyway("Temp\\MapAutosave.5min.m3d" ,"Temp\\MapAutosave.10min.m3d");
		RenameAnyway("Temp\\MapAutosave.m3d","Temp\\MapAutosave.5min.m3d");
		FastSave3DMap("Temp\\MapAutosave.m3d");
		ME_prevtime=GetTickCount();
	};
};
void FastSave3DMap(char* Map){
	_chdir(IRM->GetHomeDirectory());
	void SaveMLayersToFileInCompactForm(char *);
	SaveMLayersToFileInCompactForm("Surface\\SurfaceShape.dat");
	MPNAME=Map;	
	ResFile f1=RRewrite(Map);
	SaveHeader(f1);
	void SaveNewSurface(ResFile F);
	bool CheckIfNewTerrain();
	if(CheckIfNewTerrain()){
		SaveNewSurface(f1);
		SaveSomething(f1, "Surface\\SurfaceShape.dat", "");
	}else{
		SaveSurface(f1);
		SaveTiles(f1);
		SaveSect(f1);	
	}
	SaveMapSize(f1);
	SaveUnits4(f1);
	SaveInsideUnits(f1);
	SaveSprites2(f1);
	SaveNewWallsV2(f1);
	SaveLockNew(f1);
	SaveWaterCost(f1);
	SaveRES(f1);
	SaveTopology(f1);
	SaveWTopology(f1);
	SaveGates(f1);
	SaveZonesAndGroups(f1);
	SaveMapOptions(f1);
	SaveFormationsNewC2(f1);
	SaveDLL(f1);
	SaveAIParam(f1);
	SaveSMSInMap(f1);
	SavePeaceTimeInfo(f1);
	SaveRivers(f1);
	SavePixData(f1);
	//SaveGroundData(f1);
	void SaveRoadData(ResFile F);
	SaveRoadData(f1);
	void SaveRoadsNet(ResFile SB);
	SaveRoadsNet(f1);
	void SaveNewTop(ResFile F,int type);
	void SaveAllXMLSToMap(ResFile f1);
	SaveAllXMLSToMap(f1);
	for(int i=0;i<NMFIELDS;i++)SaveNewTop(f1,i);	
	CloseMap(f1);
	CleanPool();
};
void ClearRender();
void CreateMiniMap();
void ClearMaps();
void CheckMapName(char* Name);
void CreateWTopMap();
void ResearchCurrentIsland(byte Nat);
void CreateCostPlaces();
void CheckMapNameForStart(char* Name);
void ClearPixUndoStack();
void ClearSFPICT();
void ClearAllLModeData();
void ResetGroundCache();
void BrigadeAIUnLoading();
void CheckMemoryIntegrity();
void LoadLightHash(ResFile F);
void TestMem();
char* GetTrueMapName(char* Name){
	/*
    ResFile F=RReset(Name);
	if(F!=INVALID_HANDLE_VALUE){
		DWORD ID[3];
		RBlockRead(F,&ID,1);
		RBlockRead(F,&ID,12);
		RClose(F);
		if(ID[2]=='9hZB'){
			DeCompressFile(Name,"$temp$.$$$");
			return "$temp$.$$$";
		}		
	}
	return Name;
	*/
	ResFile F=RReset(Name);
	if(F!=INVALID_HANDLE_VALUE){
		DWORD ID[3];
		RBlockRead(F,&ID,4);		
		RClose(F);
		if(ID[0]!='PMD3' && ID[0]!='FSAK'){
			DeCompressFile(Name,"Temp\\$temp$.$$$");
			return "Temp\\$temp$.$$$";
		}		
	}
	return Name;
}
char* GetTrueMapName1(char* Name){
	/*
	ResFile F=RReset(Name);
	if(F!=INVALID_HANDLE_VALUE){
		DWORD ID[3];
		RBlockRead(F,&ID,1);
		RBlockRead(F,&ID,12);
		RClose(F);
		if(ID[2]=='9hZB'){
			DeCompressFile(Name,"$temp1$.$$$");
			return "$temp1$.$$$";
		}		
	}
	return Name;
	*/
	ResFile F=RReset(Name);
	if(F!=INVALID_HANDLE_VALUE){
		DWORD ID[3];
		RBlockRead(F,&ID,4);
		RClose(F);
		if(ID[0]!='PMD3'){
			DeCompressFile(Name,"Temp\\$temp1$.$$$");
			return "Temp\\$temp1$.$$$";
		}		
	}
	return Name;
}
void SaveBinaryData(ResFile F){
	MemoryBinStream M;
	ext_OnSaveBinaryDataFromMap(&M);
	if(M.Size()){
		DWORD C='MBDB';
		RBlockWrite(F,&C,4);
		C=8+M.Size();
		RBlockWrite(F,&C,4);
		C=M.Size();
		RBlockWrite(F,&C,4);
		RBlockWrite(F,M.GetData(),M.Size());
	}
}
void LoadBinaryData(ResFile F){
	MemoryBinStream M;
	DWORD C;
	RBlockRead(F,&C,4);
	BYTE* Data=new BYTE[C];
	M.Write(Data,C);
	delete[]Data;
	ext_OnLoadBinaryDataFromMap(&M);
}
void TestCGARB();
bool GenerateRandomMap(char* MapName);
void ShowLoadProgress(char* Mark, int v, int vMax);
CEXPORT void PrecacheOneUnit(int Nindex,byte NI){
	NewMonster* NM=NATIONS->Mon[Nindex]->newMons;
	for(int i=0;i<NM->Animations->ANM.GetAmount();i++){
		NewAnimation* NA=NM->Animations->ANM[i];
		for(int j=0;j<NA->Frames.GetAmount();j++){
			NewFrame* NF=NA->Frames[j];
			if(NF->FileID>0){
				GPS.ShowGP(-1000,-1000,0,NF->FileID,NF->SpriteID,NI);
			}
		}
	}
}
void PrecacheMapUnits(){
	byte UTypes[4096];
	memset(UTypes,0xFF,sizeof UTypes);
	itr_GetAllUnits.Create();
	while(OneObject* OB=itr_GetAllUnits.Next()){
		if(OB->NIndex<4096)UTypes[OB->NIndex]=OB->NNUM;
	}
	for(int i=0;i<4096;i++)if(UTypes[i]!=0xFF){
		PrecacheOneUnit(i,UTypes[i]);		
	}
}
bool ReducedLoadMode=false;
extern bool IgnoreTreesProcessing;
void Load3DMap(char* Map){
	_chdir(IRM->GetHomeDirectory());
	UndoMaster::RemoveAllFromDisk();
	IgnoreTreesProcessing=false;
	int L0=500;
	int L1=850;
	int Lw=(L1-L0)/10;
	//
	void MapUnloading();
	ShowLoadProgress("MapUnLoading",0,1);
	MapUnloading();
	void CheckMapNameForStart(char* Name);
	//CheckMapNameForStart(Map);

	for(int i=0;i<8;i++){
		NATIONS[i].CloseNation();		
	};	
	void LoadAllNations(byte msk,byte NIndex);
	LoadAllNations(1,255);	
	for(int i=0;i<8;i++){
		memset(NATIONS[i].NProduced,0,sizeof NATIONS[i].NProduced);
	};
	if(GenerateRandomMap(Map))return;	
	char* tName=GetTrueMapName1(Map);
	ResFile f1=RReset(tName);
	ClearMaps();
	if(f1==INVALID_HANDLE_VALUE)return;
	if(!LoadHeader(f1)){
		RClose(f1);
		return;
	};
	int fsz=RFileSize(f1);
	int posit=12;
	int sign,size;
	//InitLayersSystem();
    strcpy(GSets.CGame.cgi_CurrentMap,Map);
    extern char CurrentBackgroundMap[128];
    strcpy(CurrentBackgroundMap,Map);
	GrassRenderer::ClearGrass();
	Loadingmap=1;
	_clearfp();
	bool WasError=false;
	DWORD PrevB=0;
	do{
		ShowLoadProgress("LoadMap",posit,fsz);
		sign='MDNE';
		RBlockRead(f1,&sign,4);
		RBlockRead(f1,&size,4);
		posit+=4+size;
		switch(sign){
		case 'MPSZ':
            LoadMapSize(f1);
			break;
		case 'WBGR':
			void LoadWaterMaps(ResFile F);
			LoadWaterMaps(f1);
			break;
		case 'FRUS':
			LoadSurface(f1);
			break;
		case 'HSEM':
			void LoadNewSurface(ResFile F);
			LoadNewSurface(f1);
			break;
		case '2HSM':
			void LoadNewSurface2(ResFile F);
			LoadNewSurface2(f1);
			void CreateHeightmap();
			CreateHeightmap();
			break;
        case '3HSM':
			void LoadNewSurface3(ResFile F);
			LoadNewSurface3(f1);
			void CreateHeightmap();
			CreateHeightmap();
			break;
        case '4HSM':
            void LoadNewSurface4(ResFile F);
            LoadNewSurface4(f1);
            void CreateHeightmap();
            CreateHeightmap();
            break;
        case '5HSM':
            void LoadNewSurface5(ResFile F);
            LoadNewSurface5(f1);
            void CreateHeightmap();
			CreateHeightmap();
            break;
		case '6HSM':
			void LoadNewSurface6(ResFile F,bool);
			LoadNewSurface6(f1,true);
			void CreateHeightmap();
			CreateHeightmap();
			break;
		case '7HSM':
			void LoadNewSurface6(ResFile F,bool);
			LoadNewSurface6(f1,true);
			void CreateHeightmap();
			CreateHeightmap();
			break;
		case 'ELIT':
			LoadTiles(f1);
			break;
#ifdef _USE3D
		//case '2LIT':
		//	void LoadTilesEx(ResFile);
		//	LoadTilesEx(f1);
		//	break;
		case 'TENR':
			void LoadRoadsNetOld(ResFile SB);
			LoadRoadsNetOld(f1);
			break;
		case '2ENR':
			void LoadRoadsNet(ResFile SB);
			LoadRoadsNet(f1);
			break;
#endif //_USE3D
		case 'TCES':
			LoadSect(f1);
			break;
		case 'EERT':
			LoadSprites(f1);
			break;
		case '1ERT':
			LoadSprites1(f1);
			break;
		case '2ERT':
			LoadSprites2(f1);
			break;
		case 'SAHL':
			LoadLightHash(f1);
			break;
		case 'TINU':
			LoadUnits(f1);
			break;
		//case 'LLAW':
		//	LoadNewWalls(f1);
		//	break;
		//case '1LAW':
		//	LoadNewWallsV1(f1);
		//	break;
		//case '2LAW':
		//	LoadNewWallsV2(f1);
		//	break;
		case '1COL':
			LoadLockNew(f1);
			break;
		case '2COL':
			LoadLockNew2(f1);
			break;
		case '2AES':
			LoadWaterCost(f1);
			break;
		case 'USER':
			LoadRES(f1);
			break;
		case '2INU':
			LoadUnits2(f1);
			break;
		case '3INU':
			LoadUnits3(f1);
			break;
		case '4INU':
			LoadUnits4(f1);
			break;
		case 'INUN':
			LoadInsideUnits(f1);
			break;
		case '1TAG':
			LoadGates(f1);
			break;
		case '1NOZ':
			LoadZonesAndGroups(f1);
			break;
		case 'TPOM':
			LoadMapOptions(f1);
			break;
		case '2NOZ':
			LoadZonesAndGroupsNew(f1);
			break;
		case 'GROU':
			LoadGroundData(f1);
			break;
		//case 'OPOT':
		//	LoadTopology(f1);
		//	break;
#ifndef HASH_TOP
		case '1POT':
			LoadTopology1(f1);
			break;
		case 'WPOT':
			LoadWTopology1(f1);
			break;
#endif
		case 'MROF':
			LoadFormations(f1);
			break;
		case '1ROF':
			LoadFormationsNew(f1);
			break;
#ifdef COSSACKS2
		case '2ROF':
			LoadFormationsNewC2(f1);
			break;
#endif //COSSACKS2
		case 'LLD.':
			LoadDLL(f1);
			break;
		case 'AIIA':
			LoadAIParam(f1);
			break;
		case 'SMSP':
			LoadSMSInMap(f1);
			break;
		case 'PEAC':
			LoadPeaceTimeInfo(f1);
			break;
		case '1VIR':
			LoadRivers(f1);
			break;
		//case 'PIXD':
		//	LoadPixData(f1);
		//	break;
		//case 'PAMF':
		//	LoadFactures(f1);
		//	break;
		case 'DAOR':
			void LoadRoadData(ResFile SB);
			LoadRoadData(f1);
			break;
		//case 'RPSG':
		//	void LoadGSprites(ResFile SB);
		//	LoadGSprites(f1);
		//	break;
		case 'LMXS':
			void LoadXML_FromMap(ResFile f1);
			LoadXML_FromMap(f1);
			break;
		case 'ELIF':
			LoadSomething(f1);
			break;
		case 'GRS1':
			GrassRenderer::LoadGrass(f1);
			break;
		case 'MBDB':
            LoadBinaryData(f1);
			break;
		default:
			if(sign!='2EIV' && sign!='MDNE' && sign!='GRSS'){
				if(!WasError){
					WasError=true;
					char c[5];
					c[4]=0;
					memcpy(&c[0],&PrevB,4);
					char  s[256];
					sprintf(s,"Warning! Unknown block found! Previous block ID: %s",c);
					MessageBox(NULL,s,"LoadMap warning",0);
				}
			}
			RSeek(f1,posit);
		};
		PrevB=sign;
	}while(sign!='MDNE');
	void InitLayersSystem();
	InitLayersSystem();
	//void CreateHeightmap();
	//CreateHeightmap();
	RClose(f1);	
	//PrecacheMapUnits();
	rando();
	ShowLoadProgress("CreateTotalLocking",0,1);
	CreateTotalLocking();
	void RemoveRoadsFromLockPoints();
	RemoveRoadsFromLockPoints();
	ClearRender();
	ShowLoadProgress("CreateMiniMap",0,1);
	CreateMiniMap();
	void FreeTopology();
	void SetupTopology();
	void CreateAreas();
	ShowLoadProgress("CreateAreas",0,1);
	FreeTopology();
	SetupTopology();	
	CreateAreas();
	void ResearchIslands();
	ResearchIslands();
	for(int q=0;q<8;q++)ResearchCurrentIsland(q);
	CreateCostPlaces();
	Loadingmap=0;
	ClearPixUndoStack();
	void CreateLightMap();
	void MakeAllDirtyGBUF();
	ShowLoadProgress("CreateLightMap",0,1);
	CreateLightMap();
	MakeAllDirtyGBUF();
	ShowLoadProgress("ResetGroundCache",0,1);
	ResetGroundCache();	
	ext_OnInitAfterMapLoading();
	CheckMemoryIntegrity();
	for(int i=0;i<=MaxNatColors;i++){
		SetXRESRC(i,0,MOptions.Players.Player[i].StartRes.Wood );
		SetXRESRC(i,1,MOptions.Players.Player[i].StartRes.Gold );
		SetXRESRC(i,2,MOptions.Players.Player[i].StartRes.Stone);
		SetXRESRC(i,3,MOptions.Players.Player[i].StartRes.Food );
		SetXRESRC(i,4,MOptions.Players.Player[i].StartRes.Iron );
		SetXRESRC(i,5,MOptions.Players.Player[i].StartRes.Coal );
	}
	ShowLoadProgress("ResetGroundCache",1,1);
	CleanPool();
	void SetCorrectTreesTypees();
	SetCorrectTreesTypees();
	extern char CurrentBackgroundMap[128];
	CurrentBackgroundMap[0]=0;
};
void MapUnloading();
void Load3DMapForIntro(char* Map){
	_chdir(IRM->GetHomeDirectory());
	MapUnloading();
	char* tName=GetTrueMapName1(Map);
	ResFile f1=RReset(tName);
	ClearMaps();
	if(f1==INVALID_HANDLE_VALUE)return;
	if(!LoadHeader(f1)){
		RClose(f1);
		return;
	};
	IgnoreTreesProcessing=true;
	int fsz=RFileSize(f1);
	int posit=12;
	int sign,size;
	//InitLayersSystem();
    strcpy(GSets.CGame.cgi_CurrentMap,Map);
    extern char CurrentBackgroundMap[128];
    strcpy(CurrentBackgroundMap,Map);
	Loadingmap=1;	
	do{
		ShowLoadProgress("LoadMap",posit,fsz);
		sign='MDNE';
		RBlockRead(f1,&sign,4);
		RBlockRead(f1,&size,4);
		posit+=4+size;
		switch(sign){
		case 'MPSZ':
            LoadMapSize(f1);
			break;
		case '5HSM':
            void LoadNewSurface5(ResFile F);
            LoadNewSurface5(f1);
            void CreateHeightmap();
            CreateHeightmap();
            break;
		case '2ENR':
			void LoadRoadsNet(ResFile SB);
			LoadRoadsNet(f1);
			break;
		case '2ERT':
			LoadSprites2(f1);
			break;
		case '4INU':
			LoadUnits4(f1);
			break;
		case '1TAG':
			LoadGates(f1);
			break;
		case 'TPOM':
			LoadMapOptions(f1);
			break;
		case '2NOZ':
			LoadZonesAndGroupsNew(f1);
			break;
		case '2ROF':
			LoadFormationsNewC2(f1);
			break;		
		default:
			RSeek(f1,posit);
		};
	}while(sign!='MDNE');
	RClose(f1);
	//PrecacheMapUnits();
	ClearRender();
	void MakeAllDirtyGBUF();	
	MakeAllDirtyGBUF();
	ext_OnInitAfterMapLoading();	
	for(int i=0;i<=MaxNatColors;i++){
		SetXRESRC(i,0,MOptions.Players.Player[i].StartRes.Wood );
		SetXRESRC(i,1,MOptions.Players.Player[i].StartRes.Gold );
		SetXRESRC(i,2,MOptions.Players.Player[i].StartRes.Stone);
		SetXRESRC(i,3,MOptions.Players.Player[i].StartRes.Food );
		SetXRESRC(i,4,MOptions.Players.Player[i].StartRes.Iron );
		SetXRESRC(i,5,MOptions.Players.Player[i].StartRes.Coal );
	}
	ShowLoadProgress("ResetGroundCache",1,1);
	CleanPool();	
}
extern bool FastLoad;
void Load3DMapLandOnly(char* Map){
	if(GenerateRandomMap(Map))return;
	_chdir(IRM->GetHomeDirectory());
	char* tName=GetTrueMapName1(Map);
#ifdef _USE3D
	ResetGroundCache();
#endif
	IgnoreTreesProcessing=false;
	ClearAllLModeData();
	ClearSFPICT();
	LockBars.Clear();
	UnLockBars.Clear();
	HorceLockBars.Clear();
	ClearSMS();
	MapScenaryDLL[0]=0;
	CheckMapName(Map);
	ResFile f1=RReset(tName);
	FreeTopology();
	SetupTopology();
	ClearMaps();
	if(f1==INVALID_HANDLE_VALUE)return;
	if(!LoadHeader(f1)){
		RClose(f1);
		return;
	};
	int posit=12;
	int sign,size;
	Loadingmap=1;
	InitLayersSystem();
	RunPF(512+15,"LoadMapCycle");
	do{
		sign='MDNE';
		RBlockRead(f1,&sign,4);
		RBlockRead(f1,&size,4);
		posit+=4+size;
		switch(sign){
		case 'MPSZ':
			LoadMapSize(f1);
			break;
		case 'WBGR':
			void LoadWaterMaps(ResFile F);
			LoadWaterMaps(f1);
			break;
		case 'FRUS':
			if(FastLoad)RSeek(f1,posit);
			else LoadSurface(f1);
			break;
		//case 'ELIT':
		//	if(FastLoad)RSeek(f1,posit);
		//	else LoadTiles(f1);
		//	break;
        case '2HSM':
            void LoadNewSurface2(ResFile F);
            LoadNewSurface2(f1);
            void CreateHeightmap();
            CreateHeightmap();
            break;
        case '3HSM':
            void LoadNewSurface3(ResFile F);
            LoadNewSurface3(f1);
            void CreateHeightmap();
            CreateHeightmap();
            break;
		case '4HSM':
            void LoadNewSurface4(ResFile F);
            LoadNewSurface4(f1);
            void CreateHeightmap();
            CreateHeightmap();
            break;
        case '5HSM':
            void LoadNewSurface5(ResFile F);
            LoadNewSurface5(f1);
            void CreateHeightmap();
            CreateHeightmap();
            break;
		case 'TCES':
			if(FastLoad)RSeek(f1,posit);
			else LoadSect(f1);
			break;
		case '1VIR':
			if(FastLoad)RSeek(f1,posit);
			else LoadRivers(f1);
			break;
		case 'SAHL':
			LoadLightHash(f1);
			break;
		case 'TPOM':
			LoadMapOptions(f1);
			break;
		//case 'EERT':
		//	LoadSprites(f1);
		//	break;
		//case 'TINU':
		//	LoadUnits(f1);
		//	break;
		//case 'LLAW':
		//	LoadNewWalls(f1);
		//	break;
		//case '1LAW':
		//	LoadNewWallsV1(f1);
		//	break;
		//case 'KCOL':
		//	LoadLock(f1);
		//	break;
		case '2AES':
			if(FastLoad)RSeek(f1,posit);
			else LoadWaterCost(f1);
			break;
#ifndef HASH_TOP
		case '1POT':
			LoadTopology1(f1);
			break;
		case 'WPOT':
			LoadWTopology1(f1);
			break;
#endif
		case 'LLD.':
			LoadDLL(f1);
			break;
		case 'AIIA':
			LoadAIParam(f1);
			break;
		case 'PEAC':
			LoadPeaceTimeInfo(f1);
			break;
		case 'PIXD':
			LoadPixData(f1);
			break;
		case 'GROU':
			if(FastLoad)RSeek(f1,posit);
			else LoadGroundData(f1);
			break;
#ifdef _USE3D
		//case 'PAMF':
		//	if(FastLoad)RSeek(f1,posit);
		//	else LoadFactures(f1);
		//	break;
		case 'DAOR':
			void LoadRoadData(ResFile SB);
			LoadRoadData(f1);
			break;
		case 'RPSG':
			void LoadGSprites(ResFile SB);
			LoadGSprites(f1);
			break;
		//case '2LIT':
		//	void LoadTilesEx(ResFile);
		//	if(FastLoad)RSeek(f1,posit);
		//	else LoadTilesEx(f1);
		//	break;
		case 'TENR':
			void LoadRoadsNetOld(ResFile SB);
			LoadRoadsNetOld(f1);
			break;
		case '2ENR':
			void LoadRoadsNet(ResFile SB);
			LoadRoadsNet(f1);
			break;
		/*case 'LPT0':
			void LoadNewTop(ResFile F,int type);
			LoadNewTop(f1,0);
			break;
		case 'LPT1':
			void LoadNewTop(ResFile F,int type);
			LoadNewTop(f1,1); 
			break;
		case 'LPT2':
			void LoadNewTop(ResFile F,int type);
			LoadNewTop(f1,2);
			break;
		case 'LPT3':
			void LoadNewTop(ResFile F,int type);
			LoadNewTop(f1,3);
			break;*/
		case 'ELIF':
			LoadSomething(f1);
			break;
		case 'MBDB':
			LoadBinaryData(f1);
			break;
#endif //_USE3D
		//case 'USER':
		//	LoadRES(f1);
		//	break;
		default:
			RSeek(f1,posit);
		};
	}while(sign!='MDNE');
	StopPF(512+15);
	void FreeTopology();
	//FreeTopology();
	void SetupTopology();
	void CreateAreas();
	//if(!GetNAreas(0)){
	//CreateAreas();
	//}
	RClose(f1);
	ClearRender();	
	//CreateCostPlaces();
	Loadingmap=0;
	ResetGroundCache();
	ClearPixUndoStack();
	CleanPool();
};
void LoadZonesAndGroupsNewZO(ResFile f1){	
	int sz1,sz2;
	RBlockRead(f1,&sz1,4);
	char* s=new char[sz1+1];
	RBlockRead(f1,s,sz1);
	s[sz1]=0;
	xmlQuote xml1;
	xml1.Load(s);
	delete[](s);
	ErrorPager err;
	//AGroups.Load(xml1,&AGroups,&err);

	RBlockRead(f1,&sz2,4);
	s=new char[sz2+1];
	RBlockRead(f1,s,sz2);
	s[sz2]=0;
	xmlQuote xml2;
	xml2.Load(s);
	AZones.reset_class(&AZones);
	AZones.Load(xml2,&AZones,&err);
	delete[](s);
};
void LoadZonesOnly(char* Name){	
	char* tName=GetTrueMapName1(Name);
	ResFile f1=RReset(tName);
	if(f1==INVALID_HANDLE_VALUE)return;
	if(!LoadHeader(f1)){
		RClose(f1);
		return;
	};
	int posit=12;
	int sign,size;
	Loadingmap=1;	
	do{
		sign='MDNE';
		RBlockRead(f1,&sign,4);
		RBlockRead(f1,&size,4);
		posit+=4+size;
		switch(sign){
		case '2NOZ':
			LoadZonesAndGroupsNewZO(f1);
			break;            
		default:
			RSeek(f1,posit);
		};
	}while(sign!='MDNE');
	RClose(f1);
	CleanPool();
}
bool ProcessLoadSave(int x,int y,int Lx,int Ly,_str& str,const char* mask,const char* StartDir,bool Load,ParentFrame* Back);
void LoadZonesOnlyDialog(){
	static _str name;
	LastKey=0;
	if(ProcessLoadSave(100,100,RealLx-200,RealLy-200,name,".m3d","",true,NULL)){
		if(name.pchar())LoadZonesOnly(name.pchar());
	}
}
void CreateNationalMaskForRandomMap(char* Name);
char pname[256]="";

void CreateNationalMaskForMap(char* Name){	
	if(Name[0]=='R'&&Name[1]=='N'&&Name[3]==' '){
		MOptions.reset_class(&MOptions);
		CreateNationalMaskForRandomMap(Name);
		strcpy(pname,Name);
		return;
	}	
	if(!strcmp(pname,Name))return;
	MOptions.reset_class(&MOptions);
    strcpy(pname,Name);
	//char* tName=GetTrueMapName(Name);

	//checking for temporary map
	CreateDirectory("Preview",0);
	int L=strlen(Name);
	DWORD v=0;
	for(int i=0;i<L;i++){
		v+=Name[i]*(i+10);
	}
	char tname[128];
	sprintf(tname,"Preview\\%X",v);
	struct _stat info1;
	struct _stat info2;
	bool recr=true;
	if(_stat(Name,&info1)==0 && _stat(tname,&info2)==0)
	{
		if(info1.st_mtime<=info2.st_mtime)recr=false;
	}
	char* tName=tname;
	if(recr)tName=GetTrueMapName(Name);

	LOADNATMASK=0xFFFFFFFF;
	ResFile f1=RReset(tName);
	if(f1==INVALID_HANDLE_VALUE)return;
	if(!LoadHeader(f1)){
		RClose(f1);
		return;
	};
	int posit=12;
	int sign,size;
	do{
		sign='MDNE';
		RBlockRead(f1,&sign,4);
		RBlockRead(f1,&size,4);
		posit+=4+size;
		switch(sign){		
		case 'TPOM':
			LoadMapOptions(f1);
			break;
		case 'ELIF':
			LoadSomething(f1);
			break;
		case '2EIV':
			LoadPreview(f1,NULL);			
			break;
		default:
			RSeek(f1,posit);
		};
	}while(sign!='MDNE');
GGG:;
	LOADNATMASK=0xFFFFFFFF;
	RClose(f1);
	if(recr){
		ResFile R=RRewrite(tname);
		if(R!=INVALID_HANDLE_VALUE){
			SaveHeader(R);
			cmo_Preview* Prv=&MOptions.Preview;
			if(!Prv->UserPicture.isClear()){
				SaveSomething(f1,Prv->UserPicture.pchar(),Prv->Temp.pchar());
			}	
			SavePreviewDirectly(R);
			SaveMapOptions(R);
			RClose(R);
		}else tName==GetTrueMapName(Name);
	}
	CleanPool();
};
#define PREVIEWLX 400
#define PREVIEWLY 224
bool GetPreview(char* Name,byte* Data){
	char* tName=GetTrueMapName(Name);
	ResFile f1=RReset(tName);
	if(f1==INVALID_HANDLE_VALUE)return false;
	if(!xLoadHeader(f1)){
		RClose(f1);
		return false;
	};
	int posit=12;
	int sign,size;
	do{
		sign='MDNE';
		RBlockRead(f1,&sign,4);
		RBlockRead(f1,&size,4);
		posit+=4+size;
		if(sign=='FRUS'){
			RClose(f1);
			return false;
		};
		if(sign=='2EIV'){
			LoadPreview(f1,Data);
			RClose(f1);
			return true;
		}else RSeek(f1,posit);
	}while(sign!='MDNE');
	RClose(f1);
	CleanPool();
	return false;
};
void ShowNoCD();
extern bool NoPress;
void PerformCheckCD(){
//#ifdef _DEMO_
	return;
//#endif
	static bool inside=false;
	if(inside)return;	
	bool OK=false;	
	do{
		DWORD drives=_getdrives();
		for(int i=2;i<26;i++)if(drives&(1<<i)){
			char cc[128];
			sprintf(cc,"%c:\\",'A'+i);			
			if(GetDriveType(cc)==DRIVE_CDROM){
				//sprintf(cc,"%c:\\engine.exe",'A'+i);
				sprintf(cc,"%c:\\engine.exe",'A'+i);
				LPGSCFindInfo FI=GSFILES.gFindFirst(cc);
				if(FI){
					OK=true;
					delete(FI);
					break;
				}				
			}
		}
		if(!OK){
			int VM=GSets.CGame.ViewMask;
			GSets.CGame.ViewMask=255;
			NoPress=false;
			void SetCross(int Cross);
			int GetCross();
			int c=GetCross();
			SetCross(0);
			inside=true;
			ShowNoCD();            
			inside=false;
			GSets.CGame.ViewMask=VM;
			SetCross(c);
		}
	}while(!OK);	
}
void CleanPool(){
	POOL.pool_08.KillDirtyGarbage();
	POOL.pool_16.KillDirtyGarbage();
	POOL.pool_32.KillDirtyGarbage();
	POOL.pool_64.KillDirtyGarbage();
	POOL.pool_128.KillDirtyGarbage();
}