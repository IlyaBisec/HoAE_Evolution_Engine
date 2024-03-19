#include "stdheader.h"
#ifdef COSSACKS2
#include "BrigAITools.h"

bool CellSquad::Init(int NCells){
	if(NCells>0){
		int size=sizeof(CellType)*NCells;
		Cells=(CellType*)malloc(size);
		memset(Cells,0xFF,size);
		return true;
	}else{
		Cells=NULL;
		return false;
	}
};

void CellSystemS::Init(){
	/*
	MapLx=8192<<ADDSH;
	MapLy=8192<<ADDSH;
	MapSH=13+ADDSH;

	SquadNx=MapLx>>SquadSH;
	SquadNy=MapLy>>SquadSH;
	SquadNSH=13+ADDSH-SquadSH;
	SquadN=SquadNx*SquadNy;
	*/
	BitMap=13+ADDSH;
	BitSquare=10;
	BitCell=7;

	int SquadN=1<<(BitMap-BitSquare);	

	if(Squads) Free();
	Squads=(CellSquad**)malloc(sizeof(CellSquad*)*SquadN*SquadN);
}

void CellSystemS::Free(){
	if(Squads){
		int n=BitSquare-BitCell;
		n=(1<<n)<<n;
		for(int i=0;i<n;i++) if(Squads[i]){
			free(Squads[i]);
		}
		free(Squads);
	}
	Clear();
}	

/*
bool CellSystemS::AddCell(int x, int y, CellType* cell){
	if(!Squads) return NULL;
	if(x<0||y<0) return NULL;
	int MapL=1<<BitMap;
	if(x>=MapL||y>=MapL) return NULL;	

	int sn=BitMap-BitSquare;
	int sx=x>>BitSquare;
	int sy=y>>BitSquare;	

	int n=BitSquare-BitCell;

	CellSquad* SQD=Squads[sx+(sy<<sn)];
	if(!SQD){
		SQD=(CellSquad*)malloc(sizeof(CellSquad));
		if(!SQD->Init((1<<n)<<n)){
			free(SQD);
			SQD=NULL;
			return NULL;
		}
	}

	int m=(1<<n)-1;
	x&=m;
	y&=m;

	SQD->Cells[x+(y<<n)]=*cell;

};
*/

CellType* CellSystemS::GetCell(int x, int y, bool alloc){
	if(!Squads) return NULL;
	if(x<0||y<0) return NULL;
	int MapL=1<<BitMap;
	if(x>=MapL||y>=MapL) return NULL;	
	
	int sn=BitMap-BitSquare;
	int sx=x>>BitSquare;
	int sy=y>>BitSquare;	

	int n=BitSquare-BitCell;

	int sid=sx+(sy<<sn);
	CellSquad* SQD=Squads[sid];
	if(!SQD){
		if(alloc){
			SQD=(CellSquad*)malloc(sizeof(CellSquad));
			if(!SQD->Init(1<<(n+n))){
				free(SQD);
				SQD=NULL;
				return NULL;
			}
			Squads[sid]=SQD;
		}else{
			return NULL;
		}
	}

	int m=(1<<n)-1;
	x=(x>>BitCell)&m;
	y=(y>>BitCell)&m;
	
	return SQD->Cells+x+(y<<n);
};

CellSystemS TreeShotBar;

extern int MAXSPR;

void UnLoadAITools(){
	TreeShotBar.Free();
}

byte GetTreeShotPass(int x, int y){
	if(!TreeShotBar.Squads){
		TreeShotBar.Init();
		
		for(int i=0;i<MAXSPR;i++){
			OneSprite* OSP=Sprites+i;
			if(OSP->Enabled){
				ObjCharacter* OC=OSP->OC;
				SprGroup* SG=OSP->SG;
				if(SG==&TREES){
					//int id=OSP->SGIndex;
					//MF->BSetBar((OSP->x>>4)-4,(OSP->y>>4)-4,8);
					CellType* C=TreeShotBar.GetCell(OSP->x,OSP->y,1);
					if(C){
						if(*C>60) *C-=50;
							else *C=0;
					}
				};
			};
		};

	}
	CellType* C=TreeShotBar.GetCell(x,y,0);
	if(C) return *C;
		else return 255;
}

extern int RealLx;
extern int RealLy;
extern RLCFont YellowFont;

void ShowTreeShotPass(){
	return;

	int Nx=RealLx>>7;
	int Ny=RealLy>>6;
	
	for(int x=0;x<Nx;x++){
		for(int y=0;y<Ny;y++){
			char buf[256];
			sprintf(buf,"%d",GetTreeShotPass((mapx<<5)+(x<<7),(mapy<<5)+(y<<7)));
			//sprintf(buf,"%d,%d",(mapx<<5)+(x<<7),(mapy<<5)+(y<<7));
			ShowString(x<<7,y<<6,buf,&YellowFont);
		}
	}
}
#endif //COSSACKS2