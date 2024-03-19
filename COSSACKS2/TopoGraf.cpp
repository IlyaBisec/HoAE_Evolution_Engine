#include "stdheader.h"
#include "HashTop.h"
#include "LoadSave.h"

//MediaTop GTOP[2];

Radio Rarr[RRad];
void CreateRadio(){
	for(int ix=-RRad;ix<=RRad;ix++){
		for(int iy=-RRad;iy<=RRad;iy++){
			int r=int(sqrt(ix*ix+iy*iy));
			if(r<RRad)Rarr[r].N++;
		};
	};
	for(int i=0;i<RRad;i++){
		if(Rarr[i].N){
			Rarr[i].xi=znew(char,Rarr[i].N);
			Rarr[i].yi=znew(char,Rarr[i].N);
			Rarr[i].N=0;
		};
	};
	for(ix=-RRad;ix<=RRad;ix++){
		for(int iy=-RRad;iy<=RRad;iy++){
			int r=int(sqrt(ix*ix+iy*iy));
			if(r<RRad){
				int N=Rarr[r].N;
				Rarr[r].xi[N]=ix;
				Rarr[r].yi[N]=iy;
				Rarr[r].N++;
			};
		};
	};
	Rarr[1].xi[0]=-1;
	Rarr[1].yi[0]=0;
	Rarr[1].xi[1]=1;
	Rarr[1].yi[1]=0;
	Rarr[1].xi[2]=0;
	Rarr[1].yi[2]=-1;
	Rarr[1].xi[3]=0;
	Rarr[1].yi[3]=1;

	Rarr[1].xi[4]=-1;
	Rarr[1].yi[4]= 1;
	Rarr[1].xi[5]=-1;
	Rarr[1].yi[5]=-1;
	Rarr[1].xi[6]= 1;
	Rarr[1].yi[6]=-1;
	Rarr[1].xi[7]= 1;
	Rarr[1].yi[7]= 1;

}
void ClearRadio(){
	for(int i=0;i<RRad;i++){
		free(Rarr[i].xi);
		free(Rarr[i].yi);
	};
};
void CreateLinkInfo();
int GetLinkScale(int N1,int N2,int t);

extern int MAXSPR;
void EraseAreas();
bool GetTCStatus(int x,int y){
	int xxx=x<<2;
	int yyy=y<<2;
	if(!MFIELDS->CheckBar(xxx,yyy,4,4))return true;
	return false;
};
void CheckBright();
void ResearchIslands();
void CreateCostPlaces();
void StopDynamicalTopology();
void PreCreateTopLinks(int);
void SetClearBuildigsLock(bool Set);
void CreateAreas(){
	SetClearBuildigsLock(0);
	for(int i=0;i<NMFIELDS;i++){
		HashTable[i].CreateAreas();
	}
	SetClearBuildigsLock(1);
	return;
};
void TestCreateAreas(){	
	for(int i=0;i<NMFIELDS;i++){
		HashTable[i].ReCreateAreas(0,0,10000,10000);
	}	
	return;
};

void ClearLinkInfo(){
	
};
int OneIteration(){
	return 0;
};
void CreateRoadsNet();
extern bool MiniMade;
void InitAreas(){
};
void EraseAreas(){
	for(int i=0;i<NMFIELDS;i++){
		HashTable[i].EraseAreas();
	}	
};

bool WasOnlyOpen;
//procesing variables
void ResearchIslands();
void ProcessDynamicalTopology(){	
};

void StopDynamicalTopology(){	
};

//------------------Roads tecnology------------------
// -------- Topology extern function --------------

#ifdef HASH_TOP //------------- HASH_TOP --------------------

word GetLinksDist(int Ofs, byte TopType,byte NI){
		
	//addrand(Ofs);

	HashTop* HT=HashTable[TopType].GetHashTop(Ofs,NI);
	if(HT){
		return HT->LD;
	}	
	return 0xFFFF;
}

word GetMotionLinks(int Ofs, byte TopType,byte NI){
	
	//addrand(Ofs);

	HashTop* HT=HashTable[TopType].GetHashTop(Ofs,NI);
	if(HT){
		int ML=HT->ML;
		if(ML==8191)ML=0xFFFF;
		return ML;
	}
	return 0xFFFF;
}

word GetTopRef(int Ofs, byte TopType){
	int mo=TopLx<<TopSH;
	if(TopType<NMFIELDS&&Ofs>=0&&Ofs<mo) return HashTable[TopType].TopRef[Ofs];
	else return 0xFFFF;
}
int GetTopZone(int x, int y, byte LockType){
	x>>=6;
	y>>=6;
	if(x>=0&&y>=0&&x<TopLx&&y<TopLy){
		return GetTopRef(x+(y<<TopSH),LockType);
		//for(int i=0;i<2;i++){
		//	Radio* R=Rarr+i;
		//	for(int j=0;j<R->N;j++){
		//		word Top=GetTopRef(x+R->xi[j]+((y+R->yi[j])<<TopSH),LockType);
		//		if(Top<0xFFFF) return Top;
		//	}
		//}
	}
	return 0xFFFF;
};

Area* GetTopMap(int Ofs, byte TopType){
	if( TopType<NMFIELDS && Ofs<HashTable[TopType].NAreas )
		return HashTable[TopType].TopMap+Ofs;
	return NULL;
}

int GetNAreas(byte TopType){
	if(TopType<NMFIELDS) return HashTable[TopType].NAreas;
	return 0;
}
#endif //------------- HASH_TOP --------------------


// --- Save-Load *.sav ---
extern bool NeedProcessTop;
extern bool WasOnlyOpen;


void LS_SaveTopology(SaveBuf* SB){
	/*for(int i=0;i<NMFIELDS;i++){
		int N=HashTable[i].NAreas;
		xBlockWrite(SB,&N,4);
		HashTopTable* HT=HashTable+i;
		for(int j=0;j<N;j++){
			xBlockWrite(SB,&HT->TopMap[j].x,4);
			xBlockWrite(SB,&HT->TopMap[j].y,4);
			int nl=HT->TopMap[j].NLinks;
			xBlockWrite(SB,&nl,4);			
			xBlockWrite(SB,&HT->Link,nl*sizeof OneLinkInfo);			
		}
	}*/
};
void CreateRoadsNet();
void ClearLinkInfo();
void LS_LoadTopology(SaveBuf* SB){
	/*ClearTopology();
	for(int i=0;i<NMFIELDS;i++){
		int N;
		xBlockRead(SB,&N,4);

		int N=HashTable[i].NAreas;
		xBlockWrite(SB,&N,4);
		HashTopTable* HT=HashTable+i;
		for(int j=0;j<N;j++){
			xBlockWrite(SB,&HT->TopMap[j].x,4);
			xBlockWrite(SB,&HT->TopMap[j].y,4);
			int nl=HT->TopMap[j].NLinks;
			xBlockWrite(SB,&nl,4);			
			xBlockWrite(SB,&HT->Link,nl*sizeof OneLinkInfo);			
		}
	}*/
};

// --- Save-Load *.m3d ---
void CreateTotalLocking();
void EraseAreas();
void SaveTopology(ResFile f1){
	return;
};
void CreateRoadsNet();
void SaveWTopology(ResFile f1){
	return;
};
void LoadTopology1(ResFile f1){
	return;
};
void ResearchIslands();
void LoadWTopology1(ResFile f1){
	return;
};

// --- Init ---
void ClearTopology(){
	
};
void SetupTopology(){
	for(int i=0;i<NMFIELDS;i++){
		HashTable[i].SetUp(i);
	}
};

void FreeTopology(){
	for(int i=0;i<NMFIELDS;i++){
		HashTable[i].Free();
	}
};
void ResetTopInRadius(int x,int y,int R){
	int x0=(x-R-200)/64;
	int y0=(y-R-200)/64;
	int x1=(x+R+200)/64;
	int y1=(y+R+200)/64;
	for(int i=0;i<NMFIELDS;i++){
		HashTable[i].ReCreateAreas(x0,y0,x1,y1);
	}
}
int GetMinH(int x,int y){
	int H1=GetTotalHeight(x-32,y);
	int H2=GetTotalHeight(x+32,y);
	int H3=GetTotalHeight(x,y-32);
	int H4=GetTotalHeight(x,y+32);
	H1=min(H1,H2);
	H3=min(H3,H4);
	return min(H1,H3);
}
void ClearLockPointsOnWater(int x,int y,int R){
	int xg=x>>4;
	int yg=y>>4;
	int rr=R>>4;
	if(rr>90)rr=90;
	for(int i=0;i<rr;i++){
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		int N=Rarr[i].N;
		for(int j=0;j<N;j++){
			int xx=x+int(xi[j])*16+8;
			int yy=y+int(yi[j])*16+8;
			if(GetMinH(xx,yy)<5){
				MFIELDS[1].BSetPt(xg+xi[j],yg+yi[j]);
				MFIELDS[0].BClrPt(xg+xi[j],yg+yi[j]);
			}
		}
	}
    ResetTopInRadius(x,y,R);
}
void RestoreLockPointsOnWater(int x,int y,int R){
	int xg=x>>4;
	int yg=y>>4;
	int rr=R>>4;
	if(rr>90)rr=90;
	for(int i=0;i<rr;i++){
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		int N=Rarr[i].N;
		for(int j=0;j<N;j++){
			int xx=x+int(xi[j])*16+8;
			int yy=y+int(yi[j])*16+8;
			if(GetMinH(xx,yy)<5){
				MFIELDS[0].BSetPt(xg+xi[j],yg+yi[j]);
				MFIELDS[1].BClrPt(xg+xi[j],yg+yi[j]);
			}
		}
	}
	ResetTopInRadius(x,y,R);
}
int GetHGradient(int x,int y){
	int dx=GetTotalHeight(x-16,y)-GetTotalHeight(x+16,y);
	int dy=GetTotalHeight(x,y-16)-GetTotalHeight(x,y+16);
	return Norma(dx,dy);
}
void SetLockPointsOnLand(int x,int y,int R){
	int xg=x>>4;
	int yg=y>>4;
	int rr=R>>4;
	if(rr>90)rr=90;
	for(int i=0;i<rr;i++){
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		int N=Rarr[i].N;
		for(int j=0;j<N;j++){
			int xx=x+int(xi[j])*16+8;
			int yy=y+int(yi[j])*16+8;
			if(GetHGradient(xx,yy)<16 && GetTotalHeight(xx,yy)>5){
				MFIELDS[0].BSetPt(xg+xi[j],yg+yi[j]);				
			}
		}
	}
	ResetTopInRadius(x,y,R);
}
void RestoreLockPointsOnLand(int x,int y,int R){
	int xg=x>>4;
	int yg=y>>4;
	int rr=R>>4;
	if(rr>90)rr=90;
	for(int i=0;i<rr;i++){
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		int N=Rarr[i].N;
		for(int j=0;j<N;j++){
			int xx=x+int(xi[j])*16+8;
			int yy=y+int(yi[j])*16+8;
			if(GetHGradient(xx,yy)<16 && GetTotalHeight(xx,yy)>5){
				MFIELDS[0].BClrPt(xg+xi[j],yg+yi[j]);				
			}
		}
	}
	ResetTopInRadius(x,y,R);
}