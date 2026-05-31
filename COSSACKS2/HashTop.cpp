#include "stdheader.h"
#include "HashTop.h"
int GetLinkScale(int N1,int N2,int Type);
bool CheckIfRoadZone(int);
int GetNextWayPoints(int,int,int*);
void HashTopTable::AddLink(int N1,int N2,DWORD Mask){
	if(CheckIfRoadZone(N1)&&CheckIfRoadZone(N2)&&!GetNextWayPoints(N1,N2,NULL))return;
	Area* AR=TopMap+N1;
	int N=AR->NLinks;
	for(int i=0;i<N;i++){
		if(AR->Link[i].NextAreaID==N2){
			AR->Link[i].LinkNatMask|=Mask;
			return;
		}
	};
	if(AR->NLinks==AR->MaxLink){
		AR->MaxLink+=4;
		AR->Link=(OneLinkInfo*)realloc(AR->Link,AR->MaxLink*sizeof OneLinkInfo);
	};
	N=AR->NLinks;
	AR->Link[N].NextAreaID=N2;
	Area* A2=TopMap+N2;
	int SC=GetLinkScale(N1,N2,TopType);
	int D=(Norma(AR->x-A2->x,AR->y-A2->y)*SC)>>4;
	if(D<=0){
		D=1;
	}
	AR->Link[N].NextAreaDist=D;
	AR->Link[N].LinkNatMask=Mask;
	AR->NLinks++;
};

bool HashTopTable::AddArea(short x,short y,byte Sliv){
	int MinNorm;
	switch(ADDSH){
	case 1:
		MinNorm=1;
		break;
	case 2:
		MinNorm=1;
		break;
	case 3:
		MinNorm=1;
		break;
	};	

	if(Sliv!=2){		
		for(int i=0;i<NAreas;i++){
			Area* AR=TopMap+i;
			if(Norma(AR->x-x,AR->y-y)<=MinNorm){
				if(Sliv){
					AR->x=(AR->x+x)>>1;
					AR->y=(AR->y+y)>>1;
				};
				return false;
			};
		};
	};
	if(NAreas>=MaxArea){
		MaxArea+=120;
		TopMap=(Area*)realloc(TopMap,MaxArea* sizeof(Area));
	};
	Area* AR=TopMap+NAreas;
	memset(AR,0,sizeof(Area));
	AR->x=x;
	AR->y=y;
	AR->MaxLink=6;
	AR->Link=znew(OneLinkInfo,AR->MaxLink);
	NAreas++;
	return true;
}

bool GetTCStatus(int x,int y, byte TopType){
	int xxx=x<<2;
	int yyy=y<<2;
	MotionField* MF=MFIELDS+TopType;
	if(!MF->CheckBar(xxx,yyy,4,4))return true;
	//else{
	//	//accurate check
	//	if( ( (!MF->CheckPt(xxx+1,yyy))  || (!MF->CheckPt(xxx+2,yyy+1)) || (!MF->CheckPt(xxx+3,yyy+2)) )||
	//		( (!MF->CheckPt(xxx,yyy+1))  || (!MF->CheckPt(xxx+1,yyy+2)) || (!MF->CheckPt(xxx+2,yyy+3)) )||
	//		( (!MF->CheckPt(xxx+2,yyy))  || (!MF->CheckPt(xxx+1,yyy+1)) || (!MF->CheckPt(xxx,yyy+2  )) )||
	//		( (!MF->CheckPt(xxx+3,yyy+1))|| (!MF->CheckPt(xxx+2,yyy+2)) || (!MF->CheckPt(xxx+1,yyy+3)) )
	//	  )return true;
	//};
	return false;
};
void AddArea(short x,short y,byte Sliv,int Type){
	HashTable[Type].AddArea(x,y,Sliv);
};
void ResearchIslands();
void CreateNationDependentLinksForGates();

void MakeCellSingled(int tx,int ty,int LockType){
	byte tt[16];
	byte L [16];
	memset(tt,0xFF,sizeof tt);
	bool change=false;
	int x0=tx<<2;
	int y0=ty<<2;
	int N=0;
	for(int ix=0;ix<4;ix++){
		for(int iy=0;iy<4;iy++){
			if(L[ix+(iy<<2)]=MFIELDS[LockType].CheckPt(x0+ix,y0+iy))N++;
		}
	}
	if(N==0||N==16)return;
	int NTop=0;
	do{
		change=false;		
		for(int ix=0;ix<4;ix++){
			for(int iy=0;iy<4;iy++){
                int of=ix+(iy<<2);
				if(!L[of]){
					int tL=ix>0?tt[of-1]:0xFF;
					int tR=ix<3?tt[of+1]:0xFF;
					int tU=iy>0?tt[of-4]:0xFF;
					int tD=iy<3?tt[of+4]:0xFF;
					int tC=tt[of];
					int tm=min(tC,min(min(tL,tR),min(tU,tD)));
					if(tm==255){
                        tt[of]=NTop;
						NTop++;
						change=true;
					}else{
						if(tt[of]!=tm){
							tt[of]=tm;
							change=true;
						}
					}
				}
			}
		}
	}while(change);
	byte NZon[16];
	memset(NZon,0,NTop);
	for(int i=0;i<16;i++){
		int t=tt[i];
		if(t!=255){
			NZon[t]++;
		}
	}
	int maxidx=-1;
	int cm=0;
	for(int i=0;i<NTop;i++){
		int t=NZon[i];
		if(t>cm){
            cm=t;
			maxidx=i;
		}		
	}
	if(maxidx==-1)return;
	for(int i=0;i<16;i++){
		if(tt[i]!=maxidx&&tt[i]!=255){
            MFIELDS[LockType].BSetPt(x0+(i&3),y0+(i>>2));
		}
	}	
}
byte GetCellLinks(int tx,int ty,int LockType){
	bool link_L=0;
	bool link_R=0;
	bool link_U=0;
	bool link_D=0;

	int x0=tx<<2;
	int y0=ty<<2;

	for(int i=0;i<4;i++){
		link_L|=!(MFIELDS[LockType].CheckPt(x0-1,y0+i)||MFIELDS[LockType].CheckPt(x0,y0+i));
		link_R|=!(MFIELDS[LockType].CheckPt(x0+3,y0+i)||MFIELDS[LockType].CheckPt(x0+4,y0+i));
		link_U|=!(MFIELDS[LockType].CheckPt(x0+i,y0)||MFIELDS[LockType].CheckPt(x0+i,y0-1));
		link_D|=!(MFIELDS[LockType].CheckPt(x0+i,y0+3)||MFIELDS[LockType].CheckPt(x0+i,y0+4));
	}
	return int(link_L)+(int(link_R)<<1)+(int(link_U)<<2)+(int(link_D)<<3);
}
void HashTopTable::CreateAreas(){
	
	EraseAreas();
	MaxArea=200;
	TopMap=znew(Area,MaxArea);
	NAreas=0;

	//creating ref. map
	memset(TopRef,0xFFFF,TopLx*TopLy*2);
	int mmx=msx>>1;
	int mmy=msy>>1;
	for(int x=0;x<mmx;x++)
		for(int y=0;y<mmy;y++){
			if(GetTCStatus(x,y,TopType)) TopRef[x+y*TopLx]=0xFFFE;
		};

#ifdef _USE3D
	PreCreateTopLinks(TopType);
#endif
	for(int i=0;i<NAreas;i++){
		Area* AR=TopMap+i;
		TopRef[AR->x+AR->y*TopLx]=i;
	}
	//Try hexagon

	int HexSize=64*5;
	int max=(16384<<(ADDSH-1));
	int Nmax=max/HexSize;
	for(int i=0;i<Nmax;i++)
	{
		int j,x,y;
		if(i%2)
			for(j=0;j<(Nmax-1);j++)
			{
				int xx=HexSize+j*HexSize;
				int yy=HexSize/2+i*HexSize;
				x=xx>>6;
				y=yy>>6;
				if(x>1&&y>1&&x<TopLx-4&&y<TopLx-4){
					bool Empty=true;
					for(int dx=-1;dx<=1&&Empty;dx++)
						for(int dy=-1;dy<=1&&Empty;dy++)
							if(TopRef[x+dx+(y+dy)*TopLx]!=0xFFFE)Empty=false;
					if(Empty&&(!MFIELDS[TopType].CheckBar((x<<2)-4,(y<<2)-4,12,12)))AddArea(x,y,2);
				}
			}
		else
			for(j=0;j<Nmax;j++)
			{
				int xx=(HexSize/2+j*HexSize);
				int yy=(HexSize/2+i*HexSize);
				x=xx>>6;
				y=yy>>6;
				if(x>1&&y>1&&x<TopLx-4&&y<TopLx-4){
					bool Empty=true;
					for(int dx=-1;dx<=1&&Empty;dx++)
						for(int dy=-1;dy<=1&&Empty;dy++)
							if(TopRef[x+dx+(y+dy)*TopLx]!=0xFFFE)Empty=false;
					if(Empty&&(!MFIELDS[TopType].CheckBar((x<<2)-4,(y<<2)-4,12,12)))AddArea(x,y,2);
				}
			}
	}
	//

	ProcessMessages();

	//assert(_CrtCheckMemory());
	
	//creating linking between areas	
	ReCreateAreas(0,0,10000,10000);
}
void HashTopTable::ReCreateAreas(int x0,int y0,int x1,int y1){//xi,yi-64x coordinates

	if(x0<0)x0=0;
	if(y0<0)y0=0;
	if(x1>=TopLx)x1=TopLx-1;
	if(y1>=TopLy)y1=TopLy-1;	

	int LinksList[4096];
	int NFLinks=0;

	for(int ix=x0;ix<=x1;ix++){
		for(int iy=y0;iy<=y1;iy++){
			int ofs=ix+(iy<<TopSH);
            if(GetTCStatus(ix,iy,TopType)) TopRef[ofs]=0xFFFE;
			else TopRef[ofs]=0xFFFF;
		}
	}

	x0-=10;
	y0-=10;
	x1+=10;
	y1+=10;

	if(x0<0)x0=0;
	if(y0<0)y0=0;
	if(x1>=TopLx)x1=TopLx-1;
	if(y1>=TopLy)y1=TopLy-1;	

	for(int i=0;i<NAreas;i++){
		Area* AR=TopMap+i;
		if(AR->x>=x0&&AR->y>=y0&&AR->x<=x1&&AR->y<=y1){			
			AR->NLinks=0;            
			assert(NFLinks<4096);
			if(NFLinks<4096){
				LinksList[NFLinks]=i;
				NFLinks++;
			}
		}
	}
	int mmx=TopLx-1;
	int mmy=TopLx-1;
	int GetNRoads();	
	int NR=GetNRoads();
	if(EngSettings.DontUseRoads)NR=0;
	for(int i=0;i<NAreas;i++){
		Area* AR=TopMap+i;
		if(GetTCStatus(AR->x,AR->y,TopType)||i<NR)TopRef[AR->x+int(AR->y)*TopLx]=i;
	}
	bool Change=false;
	Change=true;		
	int GetKnotLink(int Index,int LinkIndex);
	int GetNRoadsLink(int index);
	if(TopType==0){
		int i;
		for(i=0;i<NR;i++){
			int NL=GetNRoadsLink(i);
			for(int j=0;j<NL;j++){
				int L=GetKnotLink(i,j);
				AddLink(i,L);
				AddLink(L,i);
			}
		}
	}
	//additional checking links	
	for(int ix=x0;ix<=x1;ix++){
		for(int iy=y0;iy<y1;iy++){
			MakeCellSingled(ix,iy,TopType);
		}
	}
	byte* temp=new byte[TopLx*TopLy];
	for(int ix=x0;ix<=x1;ix++){
		for(int iy=y0;iy<y1;iy++){
			int ofs=ix+(iy<<TopSH);
			int tp=TopRef[ofs];
			//if(tp>=0xFFFE){
				temp[ofs]=GetCellLinks(ix,iy,TopType);
			//}else{
			//	temp[ofs]=0;
			//}
		}
	}
	int i;
	for(i=1;i<14&&Change;i++){
		Change=false;
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		int N=Rarr[i].N;
		for(int k=0;k<N;k++){
			for(int q=0;q<NFLinks;q++){
				int j=LinksList[q];
				if(i<3||j>=NR||TopType!=0){
					Area* AR=TopMap+j;
					int xx=AR->x;
					int yy=AR->y;				
					int kN=k;//%N;
					int x=xx+xi[kN];
					int y=yy+yi[kN];
					if(x>0&&y>0&&x<mmx&&y<mmy){
						int ofs=x+(y<<TopSH);
						byte T=temp[ofs];
						int tC=TopRef[ofs];
						int tL=TopRef[ofs-1];
						int tR=TopRef[ofs+1];
						int tU=TopRef[ofs-TopLx];
						int tD=TopRef[ofs+TopLx];
						if(T&&tC>=0xFFFE){
							if(T&1&&tL==j){
								TopRef[ofs]=tL;
								tC=tL;
								Change=1;
							}else					
							if(T&2&&tR==j){
								TopRef[ofs]=tR;
								tC=tR;
								Change=1;
							}else					
							if(T&4&&tU==j){
								TopRef[ofs]=tU;
								tC=tU;
								Change=1;
							}else					
							if(T&8&&tD==j){
								TopRef[ofs]=tD;
								tC=tD;
								Change=1;
							}/*else
							if(T&1&&tL<0xFFFE){
								TopRef[ofs]=tL;
								tC=tL;
								Change=1;
							}else					
							if(T&2&&tR<0xFFFE){
								TopRef[ofs]=tR;
								tC=tR;
								Change=1;
							}else					
							if(T&4&&tU<0xFFFE){
								TopRef[ofs]=tU;
								tC=tU;
								Change=1;
							}else					
							if(T&8&&tD<0xFFFE){
								TopRef[ofs]=tD;
								tC=tD;
								Change=1;
							}*/
						}
						if(tC<0xFFFE){
							if(T&1&&tC!=tL&&tL<0xFFFE){
								AddLink(tC,tL);
								AddLink(tL,tC);
							}
							if(T&2&&tC!=tR&&tR<0xFFFE){
								AddLink(tC,tR);
								AddLink(tR,tC);
							}
							if(T&4&&tC!=tU&&tU<0xFFFE){
								AddLink(tC,tU);
								AddLink(tU,tC);
							}
							if(T&8&&tC!=tD&&tD<0xFFFE){
								AddLink(tC,tD);
								AddLink(tD,tC);
							}
						}
					}
				}
			}
		}
	}	
	Change=1;
	for(int i=0;i<8&&Change;i++){
		Change=0;
		for(int ix=x0;ix<=x1;ix++){
			for(int iy=y0;iy<=y1;iy++){
				if(ix>0&&iy>0&&ix<mmx&&iy<mmy){
					int ofs=ix+(iy<<TopSH);
					byte T=temp[ofs];

					int tC=TopRef[ofs];

					int tL=TopRef[ofs-1];
					int tR=TopRef[ofs+1];
					int tU=TopRef[ofs-TopLx];
					int tD=TopRef[ofs+TopLx];

					if(T&&tC>=0xFFFE){
						if(T&1&&tL<0xFFFE){
							TopRef[ofs]=tL;
							tC=tL;
							Change=1;
						}else					
						if(T&2&&tR<0xFFFE){
							TopRef[ofs]=tR;
							tC=tR;
							Change=1;
						}else					
						if(T&4&&tU<0xFFFE){
							TopRef[ofs]=tU;
							tC=tU;
							Change=1;
						}else					
						if(T&8&&tD<0xFFFE){
							TopRef[ofs]=tD;
							tC=tD;
							Change=1;
						}
						if(tC<0xFFFE){
							if(T&1&&tC!=tL&&tL<0xFFFE){
								AddLink(tC,tL);
								AddLink(tL,tC);
							}
							if(T&2&&tC!=tR&&tR<0xFFFE){
								AddLink(tC,tR);
								AddLink(tR,tC);
							}
							if(T&4&&tC!=tU&&tU<0xFFFE){
								AddLink(tC,tU);
								AddLink(tU,tC);
							}
							if(T&8&&tC!=tD&&tD<0xFFFE){
								AddLink(tC,tD);
								AddLink(tD,tC);
							}
						}
					}				
				}
			}
		}
	}
	ProcessMessages();
	CreateNationDependentLinksForGates();
	Clear();
	delete[](temp);
}

void HashTopTable::EraseAreas(){	
	if(NAreas){
		for(int i=0;i<NAreas;i++){
			Area* AR1=TopMap+i;
			if(AR1->MaxLink)free(AR1->Link);
			if(AR1->NMines)free(AR1->MinesIdx);
		};
		free(TopMap);
		TopMap=NULL;
		NAreas=0;
		MaxArea=0;
	};
};

extern word FULLWAY[128];
extern int NWPTS;
#define MXZ 16384

bool HashTopTable::CalculateWay(int Ofs, HashTop* HT,DWORD Mask){
	
	div_t res=div(Ofs,NAreas);
	word Start=res.quot;
	word Fin=res.rem;

	if(Start>=NAreas||Fin>=NAreas) return false;
	//TIME1=GetTickCount();
	NWPTS=0;
	//if(GetZonesDist(Start,Fin)>=0xFFFF)return 0xFFFF;
	word PrecisePointWeight    [MXZ];
	word PrecisePointPrevIndex [MXZ];
	word CandidatPointWeight   [MXZ];
	word CandidatPointPrevIndex[MXZ];
	word CandidatList          [MXZ];

	memset(PrecisePointWeight,0xFF,NAreas<<1);
	memset(CandidatPointWeight,0xFF,NAreas<<1);
	CandidatList[0]=Fin;
	CandidatPointWeight[Fin]=0;
	CandidatPointPrevIndex[Fin]=0xFFFF;
	int NCandidats=1;

	//addrand(Ofs);
	//addrand(Mask);

	do{
		//best candidat is a last candidat in sorted list
		NCandidats--;
		word TZ=CandidatList[NCandidats];
		PrecisePointWeight[TZ]=CandidatPointWeight[TZ];
		CandidatPointWeight[TZ]=0xFFFF;
		PrecisePointPrevIndex[TZ]=CandidatPointPrevIndex[TZ];
		if(TZ==Start){
			//it finally happen!!!
//			int MaxDang=DangerMap[TZ];
			int T0=TZ;
			FULLWAY[0]=TZ;
			NWPTS=1;
			for(int q=0;/*q<DangSteps&&*/T0!=0xFFFF;q++){
				T0=CandidatPointPrevIndex[T0];
				if(T0!=0xFFFF){
					if(NWPTS<128){
						FULLWAY[NWPTS]=T0;
						NWPTS++;
						//addrand(NWPTS);
					};
					//int w=DangerMap[T0];
					//if(w>MaxDang)MaxDang=w;
				};
			};
			//*MaxDanger=MaxDang;
			//TIME1=GetTickCount()-TIME1;
			HT->LD=PrecisePointWeight[TZ];
			HT->ML=PrecisePointPrevIndex[TZ];
			HT->Prio=0;
			return true;//PrecisePointPrevIndex[TZ];
		};
		Area* TAR=TopMap+TZ;
		int NL=TAR->NLinks;
		OneLinkInfo* LINK=TAR->Link;
		int w0=PrecisePointWeight[TZ];
		for(int i=0;i<NL;i++)if(LINK[i].LinkNatMask&Mask){
			int pi=LINK[i].NextAreaID;
			//adding point to the candidats list
			//1.checking if point is in precise list
			if(PrecisePointWeight[pi]==0xFFFF){
				int wi=LINK[i].NextAreaDist+w0;//+DangerMap[pi];
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
	return false;
	//return 0xFFFF;
}

CEXPORT void VitalError(char*);

HashTop* HashTopTable::GetHashTop(int Ofs,byte NI){
	word key0=Ofs&1023;
	word key1=Ofs>>10;

	HashTop* start=Key1+key0*NKey1;
	HashTop* final=start+NKey1;
	
	HashTop* cur=start;	

	while(start<final){
		if(start->NI==NI){
			if(start->K1==key1){
				if(start->Prio<7)start->Prio++;
				//addrand(Ofs);
				return start;
			}else
			if(start->K1==0xFFFF){
				cur=start;
				cur->Prio=0;
			}else
			if(start->K1<0xFFFF&&cur->Prio>start->Prio){
				cur=start;
			}
		}
		start++;
	}
	//addrand(Ofs);
	if(CalculateWay(Ofs,cur,1<<NI)){
		cur->K1=key1;
		cur->Prio=0;
		cur->NI=NI;
		return cur;
	}
	//VitalError("GetHashTop");
	return NULL;
}

#ifdef HASH_TOP
HashTopTable HashTable[NMFIELDS];
void LoadNewTop(ResFile F,int type){
    int NA;
	RBlockRead(F,&NA,4);
	HashTable[type].EraseAreas();
	HashTable[type].NAreas=NA;
	HashTable[type].MaxArea=NA;
	HashTable[type].TopMap=znew(Area,NA);
	memset(HashTable[type].TopMap,0,NA*sizeof Area);
	for(int i=0;i<NA;i++){
		Area* AR=HashTable[type].TopMap+i;
        RBlockRead(F,&AR->x,2);
		RBlockRead(F,&AR->y,2);
		RBlockRead(F,&AR->NLinks,2);
		AR->Link=znew(OneLinkInfo,AR->NLinks);
		RBlockRead(F,&AR->Link,AR->NLinks*sizeof OneLinkInfo);
	}
	RBlockRead(F,HashTable[type].TopRef,TopLx*TopLx*2);
}
void SaveNewTop(ResFile F,int type){
	return;
    int SIGN='LPT '-32+'0'+type;
	RBlockWrite(F,&SIGN,4);
	int sz=4+4;
	int NA=HashTable[type].NAreas;

	for(int i=0;i<NA;i++){
		Area* AR=HashTable[type].TopMap+i;
		sz+=6+AR->NLinks*4;
	}
	sz+=TopLx*TopLx*2;

	RBlockWrite(F,&sz,4);
	RBlockWrite(F,&NA,4);
	for(int i=0;i<NA;i++){
		Area* AR=HashTable[type].TopMap+i;
		RBlockWrite(F,&AR->x,2);
		RBlockWrite(F,&AR->y,2);
		RBlockWrite(F,&AR->NLinks,2);
		RBlockWrite(F,&AR->Link,AR->NLinks*sizeof OneLinkInfo);
	}
	RBlockWrite(F,HashTable[type].TopRef,TopLx*TopLx*2);
}
#endif //HASH_TOP

// --- MF ---

int GetZ(int z1,int z2,int z3,int z4);
void CreateWaterLocking(MotionField* MF,int x,int y,int x1,int y1){
	int ofst=x+(MaxWX)*y;
	int addofs=(MaxWX)+x-x1;
	for(int iy=x;iy<y1;iy++){
		for(int ix=x;ix<x1;ix++){
			int z1=WaterDeep[ofst];
			int z2=WaterDeep[ofst+1];
			int z3=WaterDeep[ofst+(MaxWX)];
			int z4=WaterDeep[ofst+(MaxWX)+1];
			int D1=GetZ(z1,z2,z3,z4);
			int D2=GetZ(z2,z4,z1,z3);
			int D3=GetZ(z3,z4,z1,z2);
			int D4=GetZ(z4,z3,z2,z1);
			/*
			int Z1=WaterBright[ofst];
			int Z2=WaterBright[ofst+1];
			int Z3=WaterBright[ofst+(MaxWX)];
			int Z4=WaterBright[ofst+(MaxWX)+1];
			int B1=GetZ(Z1,Z2,Z3,Z4);
			int B2=GetZ(Z2,Z4,Z1,Z3);
			int B3=GetZ(Z3,Z4,Z1,Z2);
			int B4=GetZ(Z4,Z3,Z2,Z1);
			*/
			int xx=ix+ix;
			int yy=iy+iy;
			//if(D1>130)MFIELDS[1].BClrPt(xx,yy);else MFIELDS[1].BSetPt(xx,yy);
			//if(D2>130)MFIELDS[1].BClrPt(xx+1,yy);else MFIELDS[1].BSetPt(xx+1,yy);
			//if(D3>130)MFIELDS[1].BClrPt(xx,yy+1);else MFIELDS[1].BSetPt(xx,yy+1);
			//if(D4>130)MFIELDS[1].BClrPt(xx+1,yy+1);else MFIELDS[1].BSetPt(xx+1,yy+1);
			//if(D1>128)MFIELDS->BSetPt(xx,yy);
			//if(D2>128)MFIELDS->BSetPt(xx+1,yy);
			//if(D3>128)MFIELDS->BSetPt(xx,yy+1);
			//if(D4>128)MFIELDS->BSetPt(xx+1,yy+1);
			if(D1>128)MF->BSetPt(xx,yy);
			if(D2>128)MF->BSetPt(xx+1,yy);
			if(D3>128)MF->BSetPt(xx,yy+1);
			if(D4>128)MF->BSetPt(xx+1,yy+1);
			ofst++;
		};
		ofst+=addofs;
	};
};

void SetLockMask(MotionField* MF,int x,int y,char* mask){
	int pos=0;
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++){
			if(mask[pos]=='*')MF->BSetPt(x+j,y+i);
			else MF->BClrPt(x+j,y+i);
			pos++;
		};
};

//void CreateMFieldLocking(byte LockType);
void CreateMFieldLocking(byte LockType){
	if(LockType<2||LockType>=NMFIELDS) return;
	MotionField* MF=MFIELDS+LockType;
	for(int i=0;i<MAXSPR;i++){
		OneSprite* OSP=Sprites+i;
		if(OSP->Enabled){
			ObjCharacter* OC=OSP->OC;
			SprGroup* SG=OSP->SG;
			if(OSP->Locking&(1<<LockType)){				
				int id=OSP->SGIndex;					
				MF->BSetBar((OSP->x>>4)-4,(OSP->y>>4)-4,8);
			}			
		};
	};
};
void SmoothFields(MotionField* MF,int idx){

	MF->ClearMaps();

	int mxx=msx>>1;
	int myy=msy>>1;
	int ix;	

	// mountings locking
	for(ix=0;ix<mxx;ix++){
		int ZLmin=1000000;
		int ZRmin=1000000;	
		for(int iy=myy-1;iy>=0;iy--){
			int x0=ix<<6;
			int y0=iy<<6;
			int ppx=ix<<2;
			int ppy=iy<<2;
			int ZZ=(iy<<5)-iy-iy;
			int z0=GetUnitHeight(x0,y0);
			int z1=GetUnitHeight(x0+64,y0);
			int z2=GetUnitHeight(x0,y0+64);
			int z3=GetUnitHeight(x0+64,y0+64);

			if(z0<-1)z0=-1;
			if(z1<-1)z1=-1;
			if(z2<-1)z2=-1;
			if(z3<-1)z3=-1;

			int ZL0=ZZ-z0;
			int ZL1=ZZ-z2+30;
			int ZR0=ZZ-z1;
			int ZR1=ZZ-z3+30;
			if(ZL1<ZL0){
				ZL0+=ZL1;
				ZL1=ZL0-ZL1;
				ZL0-=ZL1;
			};
			if(ZR1<ZR0){
				ZR0+=ZR1;
				ZR1=ZR0-ZR1;
				ZR0-=ZR1;
			};
			int za=(z0+z1+z2+z3)>>2;
			int zm=tmax(abs(z0-za),abs(z1-za),abs(z2-za),abs(z3-za));
			int ang=32;
			if(idx==2)ang=48;
			if(idx==3)ang=1000000;
			if(zm>=ang){
				MF->BSetBar(ppx,ppy,4);
			}else MF->BClrBar(ppx,ppy,4);			
			if(ZL0<ZLmin)ZLmin=ZL0;
			if(ZR0<ZRmin)ZRmin=ZR0;	
			
			if(za<-10){
				MFIELDS[0].BSetBar(ppx,ppy,4);
				//MFIELDS[3].BSetBar(ppx,ppy,4);
			}
			//if(za<0){
			//	MFIELDS[2].BSetBar(ppx,ppy,4);
			//}	
			
			if(za>0)MFIELDS[1].BSetBar(ppx,ppy,4);
			//if(idx==2){
            //    if(za<0)MF->BSetBar(ppx,ppy,4);
			//}
			//if(idx==3){
			//	if(za<-10)MF->BSetBar(ppx,ppy,4);
			//}
		};
	};

	// tree locking
	extern BlockBars HorceLockBars;
	if(idx==3){
		CreateMFieldLocking(3);
		int N=HorceLockBars.NBars;
		BlockCell* BC=HorceLockBars.BC;
		for(int i=0;i<N;i++){
			MF->BSetBar(int(BC[i].x)<<2,int(BC[i].y)<<2,4);
		};
	}

	//boloto
	extern BlockBars UnLockBars;
	int N=UnLockBars.NBars;
	BlockCell* BC=UnLockBars.BC;
	for(int i=0;i<N;i++){
		MF->BClrBar(int(BC[i].x)<<2,int(BC[i].y)<<2,4);
	};
    if(idx!=3){
	    extern BlockBars LockBars;
	    N=LockBars.NBars;
	    BC=LockBars.BC;
	    for(int i=0;i<N;i++){
		    MF->BSetBar(int(BC[i].x)<<2,int(BC[i].y)<<2,4);
	    };
    }
	// smoothing blocks
	for(ix=1;ix<mxx;ix++)
		for(int iy=1;iy<myy;iy++){
			int ppx=ix<<2;
			int ppy=iy<<2;
			int N=0;
			if(MF->CheckBar(ppx,ppy,4,4)){
				if(MF->CheckBar(ppx,ppy,2,2))N++;
				if(MF->CheckBar(ppx+2,ppy,2,2))N++;
				if(MF->CheckBar(ppx,ppy+2,2,2))N++;
				if(MF->CheckBar(ppx+2,ppy+2,2,2))N++;
			};
			if(N<3)MF->BClrBar(ppx,ppy,4);
			else MF->BSetBar(ppx,ppy,4);
		};

	MotionField* TMF=new MotionField;
	TMF->Allocate();
	//memcpy(TMF->MapH,MF->MapH,MAPSY*BMSX);
	memcpy(TMF->MapV,MF->MapV,MAPSY*BMSX);
	
	for(ix=1;ix<mxx;ix++)
		for(int iy=1;iy<myy;iy++){
			int ppx=ix<<2;
			int ppy=iy<<2;
			byte c=0;
			if(TMF->CheckBar(ppx+1,ppy+1-4,2,2))c|=1;
			if(TMF->CheckBar(ppx+1+4,ppy+1,2,2))c|=2;
			if(TMF->CheckBar(ppx+1,ppy+1+4,2,2))c|=4;
			if(TMF->CheckBar(ppx+1-4,ppy+1,2,2))c|=8;
			if(!TMF->CheckBar(ppx+1,ppy+1,2,2)){
				switch(c){
				case 3:
					SetLockMask(MF,ppx,ppy,"**** ***  **   *");
					break;
				case 6:
					SetLockMask(MF,ppx,ppy,"   *  ** *******");
					break;
				case 12:
					SetLockMask(MF,ppx,ppy,"*   **  *** ****");
					break;
				case 9:
					SetLockMask(MF,ppx,ppy,"******* **  *   ");
					break;
				case 7:
				case 14:
				case 13:
				case 11:
					MF->BSetBar(ppx,ppy,4);
					break;
				};
			};
		};
	//memcpy(TMF,MF,sizeof MotionField);
	//memcpy(TMF->MapH,MF->MapH,MAPSY*BMSX);
	memcpy(TMF->MapV,MF->MapV,MAPSY*BMSX);
	for(ix=1;ix<mxx;ix++)
		for(int iy=1;iy<myy;iy++){
			int ppx=ix<<2;
			int ppy=iy<<2;
			byte c=0;
			if(TMF->CheckBar(ppx+1,ppy+1-4,2,2))c|=1;
			if(TMF->CheckBar(ppx+1+4,ppy+1,2,2))c|=2;
			if(TMF->CheckBar(ppx+1,ppy+1+4,2,2))c|=4;
			if(TMF->CheckBar(ppx+1-4,ppy+1,2,2))c|=8;
			if(TMF->CheckBar(ppx+1,ppy+1,2,2)){
				switch(c){
				case 0:
					//BSetBar(ppx,ppy,0);
					break;
				case 3:
					SetLockMask(MF,ppx,ppy,"**** ***  **   *");
					break;
				case 6:
					SetLockMask(MF,ppx,ppy,"   *  ** *******");
					break;
				case 12:
					SetLockMask(MF,ppx,ppy,"*   **  *** ****");
					break;
				case 9:
					SetLockMask(MF,ppx,ppy,"******* **  *   ");
					break;
				};
			};
		};
	TMF->FreeAlloc();
	delete(TMF);

	///CreateWaterLocking(MF,0,0,(MaxWX)-1,(MaxWX)-1);
}
int GetNNearestLockPoints(int x,int y,byte* ms,MotionField* MF,int lx, int ly, int dx, int dy)
{
	int rez=0;
	if(x<=0||y<=0||x>=(dx-1)||y>=(dy-1))
	{
		if(MF->CheckPt(lx+x,ly+y))
		{
			rez=999999;
		}
	}
	else
	{
		if(ms[dx*x+y]==0)
		{
			ms[dx*x+y]=1;
			rez=1+GetNNearestLockPoints(x+1,y,ms,MF,lx,ly,dx,dy)+
				GetNNearestLockPoints(x-1,y,ms,MF,lx,ly,dx,dy)+
				GetNNearestLockPoints(x,y+1,ms,MF,lx,ly,dx,dy)+
				GetNNearestLockPoints(x,y-1,ms,MF,lx,ly,dx,dy);
		}
	}
	return rez;
}
bool ClearStandAloneLockPoints(int lx, int ly, int dx, int dy, int ClearSize)
{
	bool rez=false;
	byte* tm = (byte*)malloc(dx*dy);
	for(int k=0;k<(NMFIELDS-1);k++)
	{
		MotionField* MF=&MFIELDS[k];
		for(int i=1;i<(dx-1);i++)
		{
			for(int j=1;j<(dy-1);j++)
			{
				if(MF->CheckPt(lx+i,ly+j))
				{
					memset(tm,0,dx*dy);
					int nn=GetNNearestLockPoints(i,j,tm,MF,lx,ly,dx,dy);
					if(nn<ClearSize)
					{
						MF->BClrPt(lx+i,ly+j);
						rez=true;
					}
				}
			}
		}
	}
	free(tm);
	return rez;
}
int WNAreas(){
	return HashTable[1].NAreas;
}