#include "stdheader.h"
#include <malloc.h>
#include "ddini.h"
#include "ResFile.h"
#include "FastDraw.h"
#include "mgraph.h"
#include "mouse.h"
#include "menu.h"
#include "MapDiscr.h"
#include "multipl.h"
#include "fog.h"
#include "Nature.h"
#include <time.h>
#include "Nucl.h"
#include "Megapolis.h"
#include "dialogs.h"
#include "smart_assert.h"
#include <math.h>
#include "Masks.h"
#include "fonts.h"
#include "3DGraph.h"
#include "MapSprites.h"
#include "NewMon.h"
#include "Path.h"
#include "RealWater.h"
#include "NewAI.h"
#include "TopoGraf.h"
#include "3DMapEd.h"
#include "mode.h"
#include "Sort.h"
#include "Curve.h"
#include "Process.h"
#include "3DRandmap.h"
#include "bmptool.h"
#include "GP_Draw.h"
#include "DrawForm.h"
#include "Fonts.h"
#include <crtdbg.h>
#include "activezone.h"
#include "Globals.h"
#include "Surface\SurfPiece.h"

int MinLinkDist=9;
int StartHillHeight=90;
int MaxHillHeight=110;
int HillWidth=250;
void Save3DMap(char* Map);
void InitHillAmp1();
extern short randoma[8192];
extern int ntex;


int mrpos=0;
word XXP=0;
int mrand(){
	mrpos++;
	if(mrpos>8191)XXP+=0x3571;
	mrpos&=8191;
	return (randoma[mrpos]^XXP)&32767;
};
void CheckFirstLine(){
	int N=VertInLine+VertInLine;
	for(int i=0;i<N;i++)if(THMap[i]<0)THMap[i]=0;
};
struct GenArea{
	short x;
	short y;
	byte State;
	byte Nation;
	byte Zone;
	byte Usage;
	byte Type;
	word  NLinks;
	word* Link;
	word MaxLink;
};
PaintHills PHILL[1];
class GenMap{
public:
	word*  GTopRef;//[TopLx*TopLy];
	GenArea* GTopMap;
	int   NGAreas;
	int   MaxGArea;
	short*  VertHi;//[MaxPointIndex];
	byte*  VertType;//[MaxPointIndex];
	int SIZE;
	void CreateNet();
	bool AddGArea(int x,int y);
	void AddGLink(int N1,int N2);
	void SetStateAround(int N,byte State);
	GenMap();
	~GenMap();
	void Clear();
	void Show(int x,int y);
	void MakeLinearHill(int N1,int N2);
	void MakeSoftArea(int N1,int N2);
	void MakeHillSpot(int x,int y,int r0,byte* Height);
	bool CheckWebPoint(int i);
	bool GenerateHillLink(int N);
	bool GenerateHillLink(int N,int Thick);
	void Allocate();
	void Free();
};
void GenMap::Allocate(){
	GTopRef=znew(word,TopLx*TopLy);
	VertHi=znew(short,MaxPointIndex);
	VertType=znew(byte,MaxPointIndex);
	SIZE=MaxPointIndex;
};
void GenMap::Free(){
	delete(GTopRef);
	delete(VertHi);
	delete(VertType);
	VertHi=NULL;
	VertType=NULL;
	GTopRef=NULL;
};
void GenMap::SetStateAround(int N,byte State){
	GenArea* GA=GTopMap+N;
	for(int p=0;p<GA->NLinks;p++)GTopMap[GA->Link[p+p]].State=State;
};
bool GenMap::AddGArea(int x,int y){
	for(int i=0;i<NGAreas;i++){
		GenArea* GAR=GTopMap+i;
		if(Norma(GAR->x-x,GAR->y-y)<MinLinkDist)return false;
	};
	if(NGAreas>=MaxGArea){
		MaxGArea+=256;
		GTopMap=(GenArea*)realloc(GTopMap,MaxGArea*sizeof GenArea);
	};
	GenArea* GAR=GTopMap+NGAreas;
	GAR->x=x;
	GAR->y=y;
	GAR->Link=NULL;
	GAR->MaxLink=0;
	GAR->NLinks=0;
	GAR->State=0;
	GAR->Nation=0xFF;
	GAR->Zone=0xFF;
	GAR->Type=0xFF;
	GAR->Usage=0;
	NGAreas++;
	return true;
};
void GenMap::AddGLink(int N1,int N2){
	GenArea* AR=&GTopMap[N1];
	int N=AR->NLinks;
	for(int i=0;i<N;i++){
		if(AR->Link[i+i]==N2)return;
	};
	if(AR->NLinks==AR->MaxLink){
		AR->MaxLink+=4;
		AR->Link=(word*)realloc(AR->Link,AR->MaxLink<<2);
	};
	N=AR->NLinks;
	AR->Link[N+N]=N2;
	int dst=Norma(AR->x-GTopMap[N2].x,AR->y-GTopMap[N2].y);
	//assert(dst<40);
	AR->Link[N+N+1]=dst;
	AR->NLinks++;
};
void CheckNet(GenMap* GM){
	for(int i=0;i<GM->NGAreas;i++){
		word* LINK=GM->GTopMap[i].Link;
		int NL=GM->GTopMap[i].NLinks;
		int x=GM->GTopMap[i].x;
		int y=GM->GTopMap[i].y;
		for(int j=0;j<NL;j++){
			int id=LINK[j+j];
			int ds=LINK[j+j+1];
			GenArea* GA1=GM->GTopMap+id;
			int dst=Norma(GA1->x-x,GA1->y-y);
			//assert(dst==ds&&dst<40);
		};
	};
};
word GetDir(int,int);
bool CheckDeep(int x,int y){
	int x1=x<<1;
	int y1=y<<1;
	if(x>0&&x<msx&&y>0&&y<msx)return WaterDeep[x1+(y<<MapShift)]>128;
	return false;
};
void GenMap::CreateNet(){
	memset(GTopRef,0xFF,TopLx*TopLy*2);
	int LX=240<<(ADDSH-1);
	int LL=(LX-1)<<1;
	int LY=240<<(ADDSH-1);
	int mmx=LX;
	int mmy=LY;
	for(int y=1;y<LY;y++){
		memset(GTopRef+1+y*TopLx,0xEE,LL);
	};
	GTopMap=NULL;
	MaxGArea=0;
	NGAreas=0;
	int N3=0;
	int L3=LX-10;
	do{
		int x=5+((int(mrand())*L3)>>15);
		int y=5+((int(mrand())*L3)>>15);
		if(AddGArea(x,y))N3=0;
		else N3++;
	}while(N3<50);
	//creating links
	for(int i=0;i<NGAreas;i++)GTopRef[GTopMap[i].x+int(GTopMap[i].y)*TopLx]=i;
	bool Change=false;
	//word TempTop[TopLx*TopLy];
	Change=true;
	for(i=1;i<RRad&&Change;i++){
		//memcpy(TempTop,GetTopRef,sizeof(GetTopRef));
		Change=false;
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		int N=Rarr[i].N;
		for(int j=0;j<NGAreas;j++){
			int xx=GTopMap[j].x;
			int yy=GTopMap[j].y;
			for(int k=0;k<N;k++){
				int x=xx+xi[k];
				int y=yy+yi[k];
				if(x>0&&y>0&&x<mmx&&y<mmy){
					int ofst=x+y*TopLx;
					word ZZ=GTopRef[ofst	  ];
					word ZU=GTopRef[ofst-TopLx];
					word ZD=GTopRef[ofst+TopLx];
					word ZL=GTopRef[ofst-1    ];
					word ZR=GTopRef[ofst+1    ];
					if(ZZ==0xEEEE){
						if(ZU==j||ZD==j||ZL==j||ZR==j||
							GTopRef[ofst-TopLx-1]==j||
							GTopRef[ofst-TopLx+1]==j||
							GTopRef[ofst+TopLx-1]==j||
							GTopRef[ofst+TopLx+1]==j){
							GTopRef[ofst]=j;
							Change=true;
							ZZ=j;
						};
					};
					if(ZZ<0xEEEE){
						if(ZU!=ZZ&&ZU<0xEEEE){
							AddGLink(ZZ,ZU);
							AddGLink(ZU,ZZ);
						};
						if(ZD!=ZZ&&ZD<0xEEEE){
							AddGLink(ZZ,ZD);
							AddGLink(ZD,ZZ);
						};
						if(ZL!=ZZ&&ZL<0xEEEE){
							AddGLink(ZZ,ZL);
							AddGLink(ZL,ZZ);
						};
						if(ZR!=ZZ&&ZR<0xEEEE){
							AddGLink(ZZ,ZR);
							AddGLink(ZR,ZZ);
						};
					};
				};
			};
		};
	};
	CheckNet(this);
	//sorting links
	for(i=0;i<NGAreas;i++){
		GenArea* GA=GTopMap+i;
		int x0=GA->x;
		int y0=GA->y;
		UNISORT.CheckSize(GA->NLinks);
		int Nga=GA->NLinks;
		for(int p=0;p<Nga;p++){
			word LI=GA->Link[p+p];
			GenArea* GA1=GTopMap+LI;
			byte Dir=byte(GetDir(GA1->x-x0,GA1->y-y0));
			UNISORT.Uids[p]=LI;
			UNISORT.Parms[p]=Dir;
			/*
			if(p>0){
				char dd;
				if(p<Nga-1){
					dd=abs(char(UNISORT.Parms[p])-char(UNISORT.Parms[p-1]));
				}else{
					dd=abs(char(UNISORT.Parms[p])-char(UNISORT.Parms[0]));
				};
				if(dd>74)GA->State=1;
			};
			*/
		};
		UNISORT.NUids=Nga;
		UNISORT.Sort();
		byte CDr=0;
		for(p=0;p<Nga;p++){
			word LI=UNISORT.Uids[p];
			GenArea* GA2=GTopMap+LI;
			GA->Link[p+p]=LI;
			GA->Link[p+p+1]=Norma(GA->x-GA2->x,GA->y-GA2->y);
			int dx=GA2->x-x0;
			int dy=GA2->y-y0;
			if(dx>0)CDr|=1;
			if(dx<0)CDr|=2;
			if(dy>0)CDr|=4;
			if(dy<0)CDr|=8;

			if(dx>0&&dy>0)CDr|=16;
			if(dx<0&&dy>0)CDr|=32;
			if(dx>0&&dy<0)CDr|=64;
			if(dx<0&&dy<0)CDr|=128;
		};
		int x=GA->x;
		int y=GA->y;
		if(CDr!=255){
			if(x<20||y<20||x>LX-20||y>LX-20)GA->State=1;
		};
		if(CheckDeep(x,y))GA->State=2;
		else if(CheckDeep(x-2,y)||CheckDeep(x-4,y)||CheckDeep(x+2,y)||CheckDeep(x+4,y)||
				CheckDeep(x,y-2)||CheckDeep(x,y-4)||CheckDeep(x,y+2)||CheckDeep(x,y+4)||
				CheckDeep(x-3,y-3)||CheckDeep(x-3,y+3)||CheckDeep(x+3,y-3)||CheckDeep(x+3,y+3))
				GA->State=2;



	};
	CheckNet(this);
	/*
	for(i=0;i<3;i++){
		int idx=(int(rand())*NGAreas)>>15;
		GenArea* GA=GTopMap+idx;
		if(!GA->State){
			int LI=(int(rand())*GA->NLinks)>>15;
			MakeLinearHill(idx,GA->Link[LI+LI]);
		};
	};
	*/
};
void ErrM(char* s);
void InvHill(char* Name){
	char ccc[128];
	sprintf(ccc,"Invalid file: %s",Name);
	ErrM(ccc);
};
bool LoadTerrain(char* Name){
	GFILE* F=Gopen(Name,"r");
	if(F){
		int x,y,z,N;
		char str[128];
		z=Gscanf(F,"%d",&N);
		PHILL[0].NHiSections=N;
		for(int p=0;p<N;p++){
			z=Gscanf(F,"%d",&PHILL[0].HiSect[p]);
		};
		z=Gscanf(F,"%s%d",str,&N);
		if(z!=2||strcmp(str,"NLayers")){
			InvHill(Name);
		};
		PHILL[0].NLayers=N;
		Gscanf(F,"%s",str);
		for(int i=0;i<N;i++){
			z=Gscanf(F,"%s%d",str,&x);
			if(z!=2||strcmp(str,"TexAmount"))InvHill(Name);
			PHILL[0].TexAmount[i]=x;
			for(int j=0;j<x;j++){
				z=Gscanf(F,"%d",&y);
				if(z!=1)InvHill(Name);
				if(y<0)PHILL[0].Texs[i][j]=128-y;
				else PHILL[0].Texs[i][j]=y;
			};
			z=Gscanf(F,"%s%d%d",str,&x,&y);
			if(z!=3)InvHill(Name);
			PHILL[0].TexStartHi[i]=x;
			PHILL[0].TexEndHi[i]=y;
			z=Gscanf(F,"%s%d%d",str,&x,&y);
			if(z!=3)InvHill(Name);
			PHILL[0].TexStartAng[i]=x;
			PHILL[0].TexEndAng[i]=y;
			z=Gscanf(F,"%s",str);
			if(str[0]=='#'){
				switch(str[1]){
				case 'L':
					PHILL[0].CondType[i]=1;
					PHILL[0].Param1[i]=str[2]-'0';
					PHILL[0].Param2[i]=str[3]-'0';
					break;
				case 'U':
					PHILL[0].CondType[i]=2;
					PHILL[0].Param1[i]=str[2]-'0';
					PHILL[0].Param2[i]=str[3]-'0';
					break;
				};
				Gscanf(F,"%s",str);
			};
		};
		Gclose(F);
		return true;
	};
	return false;
};
void LoadRandomHills(){
	int NAttm=100;
	do{
		char ccc[64];
		sprintf(ccc,"Terrain\\Hills%d.dat",mrand()&7);
		if(LoadTerrain(ccc))return;
		NAttm--;
	}while(NAttm);
};
GenMap::GenMap(){
	NGAreas=0;
	MaxGArea=0;
	GTopMap=NULL;
	//memset(VertHi,0,sizeof VertHi);
	//memset(VertType,0,sizeof VertType);
	//CreateNet();
	memset(PHILL,0,sizeof PHILL[0]);
	PHILL[0].NLayers=11;
	//-----------------------------
	PHILL[0].TexAmount  [0]   =4;

	PHILL[0].Texs       [0][0]=56+128;
	PHILL[0].Texs       [0][1]=57+128;
	PHILL[0].Texs       [0][2]=58+128;
	PHILL[0].Texs       [0][3]=59+128;

	PHILL[0].TexStartHi [0]=0;
	PHILL[0].TexEndHi   [0]=10;
	PHILL[0].TexStartAng[0]=0;
	PHILL[0].TexEndAng  [0]=1024;
	//-----------------------------
	PHILL[0].TexAmount  [1]   =4;

	PHILL[0].Texs       [1][0]=60+128;
	PHILL[0].Texs       [1][1]=61+128;
	PHILL[0].Texs       [1][2]=62+128;
	PHILL[0].Texs       [1][3]=63+128;

	PHILL[0].TexStartHi [1]=10;
	PHILL[0].TexEndHi   [1]=100;
	PHILL[0].TexStartAng[1]=40;
	PHILL[0].TexEndAng  [1]=1024;
	//-----------------------------
	PHILL[0].TexAmount  [2]   =1;

	PHILL[0].Texs       [2][0]=64+128;

	PHILL[0].TexStartHi [2]=100;
	PHILL[0].TexEndHi   [2]=126;
	PHILL[0].TexStartAng[2]=25;
	PHILL[0].TexEndAng  [2]=300;
	//-----------------------------
	PHILL[0].TexAmount  [3]   =3;

	PHILL[0].Texs       [3][0]=85+128;
	PHILL[0].Texs       [3][1]=86+128;
	PHILL[0].Texs       [3][2]=87+128;

	PHILL[0].TexStartHi [3]=127;
	PHILL[0].TexEndHi   [3]=129;
	PHILL[0].TexStartAng[3]=39;
	PHILL[0].TexEndAng  [3]=1024;
	//-----------------------------
	PHILL[0].TexAmount  [4]   =2;

	PHILL[0].Texs       [4][0]=4;
	PHILL[0].Texs       [4][1]=11;

	PHILL[0].TexStartHi [4]=127;
	PHILL[0].TexEndHi   [4]=129;
	PHILL[0].TexStartAng[4]=0;
	PHILL[0].TexEndAng  [4]=99;
	//-----------------------------
	PHILL[0].TexAmount  [5]   =2;

	PHILL[0].Texs       [5][0]=11;
	PHILL[0].Texs       [5][1]=4;

	PHILL[0].TexStartHi [5]=110;
	PHILL[0].TexEndHi   [5]=127;
	PHILL[0].TexStartAng[5]=0;
	PHILL[0].TexEndAng  [5]=99;
	//-----------------------------
	PHILL[0].TexAmount  [6]   =1;

	PHILL[0].Texs       [6][0]=4;

	PHILL[0].TexStartHi [6]=90;
	PHILL[0].TexEndHi   [6]=110;
	PHILL[0].TexStartAng[6]=0;
	PHILL[0].TexEndAng  [6]=99;
	//-----------------------------
	PHILL[0].TexAmount  [7]   =2;

	PHILL[0].Texs       [7][0]=4;
	PHILL[0].Texs       [7][1]=5;

	PHILL[0].TexStartHi [7]=70;
	PHILL[0].TexEndHi   [7]=90;
	PHILL[0].TexStartAng[7]=0;
	PHILL[0].TexEndAng  [7]=99;
	//-----------------------------
	PHILL[0].TexAmount  [8]   =1;

	PHILL[0].Texs       [8][0]=5;

	PHILL[0].TexStartHi [8]=55;
	PHILL[0].TexEndHi   [8]=70;
	PHILL[0].TexStartAng[8]=0;
	PHILL[0].TexEndAng  [8]=99;
	//-----------------------------
	PHILL[0].TexAmount  [9]   =2;

	PHILL[0].Texs       [9][0]=5;
	PHILL[0].Texs       [9][1]=6;

	PHILL[0].TexStartHi [9]=45;
	PHILL[0].TexEndHi   [9]=55;
	PHILL[0].TexStartAng[9]=0;
	PHILL[0].TexEndAng  [9]=99;
	//-----------------------------
	PHILL[0].TexAmount  [10]   =2;

	PHILL[0].Texs       [10][0]=6;
	PHILL[0].Texs       [10][1]=0;

	PHILL[0].TexStartHi [10]=30;
	PHILL[0].TexEndHi   [10]=45;
	PHILL[0].TexStartAng[10]=0;
	PHILL[0].TexEndAng  [10]=99;

};
//-------------------------------Drawing Linear segment------------------------------//
struct BiSegment{
	double x0;
	double y0;
	double x1;
	double y1;
	double Dx;
	double Dy;
	double R0;
	double Ang0;
	double MaxPhase;
};
#define PI 3.14159265358979
void InitBiSegment(BiSegment* BS,int x0,int y0,int x1,int y1){
	BS->x0=x0;
	BS->y0=y0;
	BS->x1=x1;
	BS->y1=y1;
	BS->Dx=x1-x0;
	BS->Dy=y1-y0;
	BS->R0=sqrt(BS->Dx*BS->Dx+BS->Dy*BS->Dy);
	BS->Ang0=atan2(BS->Dy,BS->Dx);
};

double GetBiFunction(double x,double y,double* Phase,BiSegment* BS){

	double Pdx=x-BS->x0;
	double Pdy=y-BS->y0;
	double R=BS->R0;
	double Dx=BS->Dx;
	double Dy=BS->Dy;
	double R2=2*R;
	double R1=sqrt(Pdx*Pdx+Pdy*Pdy);
	double scl=(Pdx*Dx+Pdy*Dy);
	if(R1)scl/=R;
	else scl=0;
	double Rd,phase;
	if(scl<=0){
		double Ang=atan2(y-BS->y0,x-BS->x0)-BS->Ang0;
		if(Ang<0)Ang+=PI*2;
		if(Ang>PI*2)Ang-=PI*2;
		Rd=R1;
		phase=(R2+Rd*(PI/2+Ang));
	}else
	if(scl>R){
		double Ang=atan2(y-BS->y1,x-BS->x1)-BS->Ang0+PI;
		if(Ang<0)Ang+=PI*2;
		if(Ang>PI*2)Ang-=PI*2;
		Pdx=x-BS->x1;
		Pdy=y-BS->y1;
		Rd=sqrt(Pdx*Pdx+Pdy*Pdy);
		phase=(R+(Ang-PI/2)*Rd);
	}else{
		Rd=(Dy*Pdx-Dx*Pdy)/R;
		if(Rd>0){
			phase=scl;
		}else{
			Rd=-Rd;
			phase=(R2+Rd*PI-scl);
		};
	};
	double L=2*(PI*Rd+R);
	phase=(phase+Rd*PI/2)/L;
	if(phase>1)phase-=1;
	*Phase=phase;
	return Rd;
};

//-----------------------------------------------------------------------------------//
void GenMap::Clear(){
	for(int p=0;p<NGAreas;p++){
		GenArea* GA=GTopMap+p;
		if(GA->MaxLink)delete(GA->Link);
	};
	if(GTopMap){
		delete(GTopMap);
		GTopMap=NULL;
	};
	MaxGArea=0;
	NGAreas=0;
	if(VertHi)memset(VertHi,0,SIZE*2);
	if(VertType)memset(VertType,0,SIZE);
};
GenMap::~GenMap(){
	Clear();
};
void GenMap::Show(int x,int y){
	for(int i=0;i<NGAreas;i++){
		GenArea* GA=GTopMap+i;
		int x0=GA->x;
		int y0=GA->y;
		for(int j=0;j<GA->NLinks;j++){
			GenArea* GA1=GTopMap+GA->Link[j+j];
			DrawLine(x0+x,y0+y,GA1->x+x,GA1->y+y,clrRed);
		};
		if(GA->State==1){
			DrawLine(x0-2,y0,x0+2,y0,clrYello);		
			DrawLine(x0,y0-2,x0,y0+2,clrYello);		
		}else
		if(GA->State==2){
			DrawLine(x0-2,y0,x0+2,y0,0xF9);		
			DrawLine(x0,y0-2,x0,y0+2,0xF9);		
		}else{
			DrawLine(x0-2,y0,x0+2,y0,255);		
			DrawLine(x0,y0-2,x0,y0+2,255);		
		};
	};
	
};
void InterpolateCurve();
void UnLockLine(int x0,int y0,int x1,int y1){
	int N=Norma(x1-x0,y1-y0)+1;
	x0<<=16;
	y0<<=16;
	x1<<=16;
	y1<<=16;
	int r=Norma(x1-x0,y1-y0);
	int dx=(x1-x0)/N;
	int dy=(y1-y0)/N;
	int x=x0;
	int y=y0;
	for(int i=0;i<N;i++){
		int xs=x>>16;
		int ys=y>>16;
		AddUnLockbar(xs,ys);

		AddUnLockbar(xs+1,ys);
		AddUnLockbar(xs-1,ys);
		AddUnLockbar(xs,ys+1);
		AddUnLockbar(xs,ys-1);

		AddUnLockbar(xs+1,ys+1);
		AddUnLockbar(xs-1,ys+1);
		AddUnLockbar(xs+1,ys-1);
		AddUnLockbar(xs-1,ys-1);
		x+=dx;
		y+=dy;
	};
};
void GenMap::MakeSoftArea(int N1,int N2){
	//return;
	GenArea* GA1=GTopMap+N1;
	GenArea* GA2=GTopMap+N2;
	//search for common link
	int CL1=-1;
	int CL2=-1;
	for(int i=0;i<GA1->NLinks;i++){
		if(GA1->Link[i+i]==N2)CL1=i;
	};
	for(i=0;i<GA2->NLinks;i++){
		if(GA2->Link[i+i]==N1)CL2=i;
	};
	if(CL1==-1||CL2==-1)return;
	//enumerating key points
	int NKeys=0;
	int KeyX[32];
	int KeyY[32];
	int x0=GA1->x;
	int y0=GA1->y;
	GA1->State=2;
	GA2->State=2;
	for(i=0;i<GA1->NLinks;i++){
		if(i!=CL1){
			word LI=GA1->Link[i+i];
			GenArea* GA3=GTopMap+LI;
			int xa1=(x0+GA3->x*2)/3;
			int ya1=(y0+GA3->y*2)/3;
			KeyX[NKeys]=xa1;
			KeyY[NKeys]=ya1;
			NKeys++;
		};
	};
	x0=GA2->x;
	y0=GA2->y;
	for(i=0;i<GA2->NLinks;i++){
		if(i!=CL2){
			word LI=GA2->Link[i+i];
			GenArea* GA3=GTopMap+LI;
			int xa1=(x0+GA3->x*2)/3;
			int ya1=(y0+GA3->y*2)/3;
			KeyX[NKeys]=xa1;
			KeyY[NKeys]=ya1;
			NKeys++;
		};
	};
	//sorting key points by angle
	x0=(GA1->x+GA2->x)>>1;
	y0=(GA1->y+GA2->y)>>1;
	UNISORT.CheckSize(NKeys);
	for(i=0;i<NKeys;i++){
		UNISORT.Uids[i]=i;
		UNISORT.Parms[i]=GetDir(KeyX[i]-x0,KeyY[i]-y0);
	};
	UNISORT.NUids=NKeys;
	UNISORT.Sort();
	ClearCurve();
	x0=GA1->x;
	y0=GA1->y;
	NKeys=0;
	for(i=0;i<GA1->NLinks;i++){
		if(i!=CL1){
			word LI=GA1->Link[i+i];
			GenArea* GA3=GTopMap+LI;
			int xa1=(x0*2+GA3->x)/3;
			int ya1=(y0*2+GA3->y)/3;
			KeyX[NKeys]=xa1;
			KeyY[NKeys]=ya1;
			NKeys++;
		};
	};
	x0=GA2->x;
	y0=GA2->y;
	for(i=0;i<GA2->NLinks;i++){
		if(i!=CL2){
			word LI=GA2->Link[i+i];
			GenArea* GA3=GTopMap+LI;
			int xa1=(x0+GA3->x*3)/4;
			int ya1=(y0+GA3->y*3)/4;
			KeyX[NKeys]=xa1;
			KeyY[NKeys]=ya1;
			NKeys++;
		};
	};
	for(i=0;i<NKeys;i++){
		int idx=UNISORT.Uids[i];
		AddPointToCurve(KeyX[idx]<<6,KeyY[idx]<<6,false,0);
	};
	int idx=UNISORT.Uids[0];
	AddPointToCurve(KeyX[idx]<<6,KeyY[idx]<<6,false,0);
	//InterpolateCurve();
	SurfType=1;
	//if(rand()<16384)SurfType=2;
	//PlatoHi=128;
	SoftRegion(VertHi);
	UnLockLine(GA1->x,GA1->y,GA2->x,GA2->y);
	ClearCurve();
};
void InitHillAmp();
double GetHillHi(double r,double phase,double r0,double Width);
int SqInt(byte* REND,int x,int y);
void GenMap::MakeHillSpot(int x,int y,int r0,byte* Height){
	r0<<=ADDSH-1;
	int rrx=(r0+320)>>5;
	int x0=(x>>5)-rrx;
	int y0=(y>>5)-rrx;
	int x1=(x>>5)+rrx;
	int y1=(y>>5)+rrx;

	if(x0<0)x0=0;
	if(y0<0)y0=0;
	if(x1>=VertInLine)x1=VertInLine-1;
	if(y1>=MaxTH)y1=MaxTH-1;

	InitHillAmp1();
	HillWidth=300;
	double RR=r0;
	for(int vx=x0;vx<x1;vx++){
		for(int vy=y0;vy<y1;vy++){
			int v=vx+VertInLine*vy;
			int xv=GetTriX(v);
			int yv=GetTriY(v);
			int H=SqInt(Height,xv,yv);
			double phase=atan2(xv-x,yv-y)/3.14159265;
			if(phase<0)phase+=1;
			if(phase>1)phase-=1;

			double r=sqrt((x-xv)*(x-xv)+(y-yv)*(y-yv));
			int Wi=(HillWidth*H)>>12;
			if(Wi<120)Wi=120;
			double f=GetHillHi(r,phase,RR,Wi);
			int h=int(128.0*f);
			int Hi=int(double(H)*f*180.0/4096.0);
			if(vx==x0||vy==y0||vx==x1-1||vy==y1-1){
				//assert(Hi==0);
			};
			if(h){
				VertType[v]=1;
				if(VertHi[v]<h)VertHi[v]=h;
				if(THMap[v]<Hi)THMap[v]=Hi;
			}else VertType[v]=1;
		};
	};
};
int GetFractalVal(int x,int y);
bool UseFractal=0;
void GenMap::MakeLinearHill(int N1,int N2){
	GenArea* GA1=GTopMap+N1;
	GenArea* GA2=GTopMap+N2;
	//search for common link
	int CL1=-1;
	int CL2=-1;
	for(int i=0;i<GA1->NLinks;i++){
		if(GA1->Link[i+i]==N2)CL1=i;
	};
	for(i=0;i<GA2->NLinks;i++){
		if(GA2->Link[i+i]==N1)CL2=i;
	};
	if(CL1==-1||CL2==-1)return;
	int MinR=0;
	int NS=0;
	for(int p=0;p<GA1->NLinks;p++){
		int r=GA1->Link[p+p+1];
		MinR+=r;
		NS++;
	};
	for(p=0;p<GA2->NLinks;p++){
		int r=GA2->Link[p+p+1];
		MinR+=r;
		NS++;
	};
	MinR/=NS;
	BiSegment BS;

	InitBiSegment(&BS,int(GA1->x)<<6,int(GA1->y)<<6,int(GA2->x)<<6,int(GA2->y)<<6);

	int x0=(int(GA1->x)<<1);
	int y0=(int(GA1->y)<<1);
	int x1=(int(GA2->x)<<1);
	int y1=(int(GA2->y)<<1);
	if(x0>x1){
		x0+=x1;
		x1=x0-x1;
		x0=x0-x1;
	};
	if(y0>y1){
		y0+=y1;
		y1=y0-y1;
		y0=y0-y1;
	};
	x0-=MinR+20;
	y0-=MinR+20;
	x1+=MinR+20;
	y1+=MinR+20;

	if(x0<0)x0=0;
	if(y0<0)y0=0;
	if(x1>=VertInLine)x1=VertInLine-1;
	if(y1>=MaxTH)y1=MaxTH-1;

	int r0=MinR<<5;
	r0=(r0*14)>>4;
	InitHillAmp();
	for(int vx=x0;vx<x1;vx++){
		for(int vy=y0;vy<y1;vy++){
			int v=vx+VertInLine*vy;
			int x=GetTriX(v);
			int y=GetTriY(v);
			double phase;
			double r=GetBiFunction(x,y,&phase,&BS);
			double f=GetHillHi(r,phase,r0,HillWidth);
			if(f<0)f=0;
			int h;//=int(128.0*f);
			if(UseFractal){
				int v=GetFractalVal(x,y)+256;
				f=1;
				f=(f*v)/512;
				double re=r/650;
				double f0=0;
				if(re>15)f=0;
				else{
					f0=exp(-re*re)-0.05;
					if(f0<0)f0=0;
					f*=f0;
					//f0=f;
				};
				//h=int(120.0*f0);
				h=int(double(PlatoHi)*f)>>1;
			}else h=int(128.0*f);
			int Hi=int(double(PlatoHi)*f);
			if(vx==x0||vy==y0||vx==x1-1||vy==y1-1){
				//assert(Hi==0);
			};
			if(h){
				VertType[v]=1;
				if(VertHi[v]<h)VertHi[v]=h;
				if(THMap[v]<Hi)THMap[v]=Hi;
			}else VertType[v]=1;
		};
	};
	GA1->State=2;
	GA2->State=2;
	/*
	//enumerating key points
	int NKey=0;
	int KeyX[32];
	int KeyY[32];
	int x0=GA1->x;
	int y0=GA1->y;
	GA1->State=2;
	GA2->State=2;
	for(i=0;i<GA1->NLinks;i++){
		if(i!=CL1){
			word LI=GA1->Link[i+i];
			GenArea* GA3=GTopMap+LI;
			int xa1=(x0+GA3->x*2)/3;
			int ya1=(y0+GA3->y*2)/3;
			KeyX[NKey]=xa1;
			KeyY[NKey]=ya1;
			NKey++;
		};
	};
	x0=GA2->x;
	y0=GA2->y;
	for(i=0;i<GA2->NLinks;i++){
		if(i!=CL2){
			word LI=GA2->Link[i+i];
			GenArea* GA3=GTopMap+LI;
			int xa1=(x0+GA3->x*2)/3;
			int ya1=(y0+GA3->y*2)/3;
			KeyX[NKey]=xa1;
			KeyY[NKey]=ya1;
			NKey++;
		};
	};
	//sorting key points by angle
	x0=(GA1->x+GA2->x)>>1;
	y0=(GA1->y+GA2->y)>>1;
	UNISORT.CheckSize(NKey);
	for(i=0;i<NKey;i++){
		UNISORT.Uids[i]=i;
		UNISORT.Parms[i]=GetDir(KeyX[i]-x0,KeyY[i]-y0);
	};
	UNISORT.NUids=NKey;
	UNISORT.Sort();
	ClearCurve();
	x0=GA1->x;
	y0=GA1->y;
	NKey=0;
	for(i=0;i<GA1->NLinks;i++){
		if(i!=CL1){
			word LI=GA1->Link[i+i];
			GenArea* GA3=GTopMap+LI;
			int xa1=(x0+GA3->x)>>1;
			int ya1=(y0+GA3->y)>>1;
			KeyX[NKey]=xa1;
			KeyY[NKey]=ya1;
			NKey++;
		};
	};
	x0=GA2->x;
	y0=GA2->y;
	for(i=0;i<GA2->NLinks;i++){
		if(i!=CL2){
			word LI=GA2->Link[i+i];
			GenArea* GA3=GTopMap+LI;
			int xa1=(x0+GA3->x)>>1;
			int ya1=(y0+GA3->y)>>1;
			KeyX[NKey]=xa1;
			KeyY[NKey]=ya1;
			NKey++;
		};
	};
	for(i=0;i<NKey;i++){
		int idx=UNISORT.Uids[i];
		AddPointToCurve(KeyX[idx]<<6,KeyY[idx]<<6,false,0);
	};
	int idx=UNISORT.Uids[0];
	AddPointToCurve(KeyX[idx]<<6,KeyY[idx]<<6,false,0);
	InterpolateCurve();
	SurfType=1;
	if(rand()<16384)SurfType=2;
	//PlatoHi=128;
	SetPlatoInCurve(VertHi,VertType,1);
	ClearCurve();
	*/
};
bool GenMap::CheckWebPoint(int i){
	GenArea* GA=GTopMap+i;
	if(GA->State)return false;
	for(int j=0;j<GA->NLinks;j++){
		GenArea* GA1=GTopMap+GA->Link[j+j];
		if(GA1->State>1)return false;
	};
	return true;
};
bool GenMap::GenerateHillLink(int N){
	return GenerateHillLink(N,0);
};
bool GenMap::GenerateHillLink(int N,int Thick){
	int Hills[96];
	int NHills=0;
	do{
		int idx=(int(mrand())*NGAreas)>>15;
		if(CheckWebPoint(idx)){
			Hills[0]=idx;
			NHills=1;
			
			for(int k=0;k<N;k++){
				int NAttm=0;
				int NextLink=-1;
				for(int p=0;p<NHills-1;p++){
					GTopMap[Hills[p]].State+=16;
				};
				GenArea* GA0=GTopMap+idx;
				int nLink=GA0->NLinks;
				do{
					int L=(int(mrand())*nLink)>>15;
					int idx1=GA0->Link[L+L];
					if(CheckWebPoint(idx1)){
						NextLink=idx1;
					};
					NAttm++;
				}while(NAttm<15&&NextLink==-1);
				for(p=0;p<NHills-1;p++){
					GTopMap[Hills[p]].State-=16;
				};
				if(NextLink!=-1){
					Hills[NHills]=NextLink;
					NHills++;
					idx=NextLink;
				}else return false;
			};
			for(k=1;k<NHills;k++){
				PlatoHi=StartHillHeight+((int(mrand())*(MaxHillHeight-StartHillHeight))>>15);
				MakeLinearHill(Hills[k],Hills[k-1]);
				ShowProcessInfo();
			};
			int np=4;
			if(Thick)np=7;
			//expanding Hills
			for(k=0;k<NHills;k++){
				GTopMap[Hills[k]].State=1;
			};
			for(int p=0;p<Thick;p++){
				int NH=NHills;
				for(int m=0;m<NH;m++){
					GenArea* GA=GTopMap+Hills[m];
					for(int q=0;q<GA->NLinks;q++){
						int idx=GA->Link[q+q];
						if(CheckWebPoint(idx)){
							Hills[NHills]=idx;
							MakeLinearHill(Hills[m],idx);
							GTopMap[idx].State=1;
							GA->State=1;
							ShowProcessInfo();
							NHills++;
						};
					};
				};
			};
			//for(k=1;k<NHills;k++){
				//PlatoHi=128+((int(rand())*100)>>15)-20;
				//MakeSoftArea(Hills[k],Hills[k-1]);
				//ShowProcessInfo();
			//};
			for(k=0;k<NHills;k++){
				GTopMap[Hills[k]].State=2;
			};
			if(!UseFractal){
				for(k=0;k<np;k++){
					int hil=(int(mrand())*NHills)>>15;
					int nattm=0;
					int aidx=Hills[hil];
					GenArea* GA4=GTopMap+aidx;
					int nLink=GA4->NLinks;
					int GX4=GA4->x;
					int GY4=GA4->y;
					do{
						int LI=(int(mrand())*nLink)>>15;
						int mm=GA4->Link[LI+LI];
						GenArea* GA5=GTopMap+mm;
						if(GA5->State<2){
							int GX5=GA5->x;
							int GY5=GA5->y;
							//GX5+=(GX5-GX4)>>2;
							//GY5+=(GY5-GY4)>>2;
							if(GetHeight((GX5<<6)+32,(GY5<<6)+32)<3){
								MakeSoftArea(aidx,mm);
								ShowProcessInfo();
								GA5->State=2;
								nattm=10000;
							};
						};
						nattm++;
					}while(nattm<1000);
				};
			};
			for(k=0;k<NHills;k++){
				SetStateAround(Hills[k],2);
			};
			
			
			return true;
		};
		N++;
	}while(N<10);
	return false;
};
#define NATLX (TopLx>>1)
#define NATSH (TopSH-1)
byte* NatDeals;//[NATLX*NATLX];
double HillR[256];
double HillW[256];
#define NFUR 10
void InitHillAmp(){
	double HR[NFUR];
	double PH[NFUR];
	double HRW[NFUR];
	double PHW[NFUR];
	for(int i=0;i<NFUR;i++){
		HR[i]=double(mrand())/32768.0/(i+4);
		PH[i]=double(mrand())/10768.0;
		HRW[i]=double(mrand())/32768.0/(i+4);
		PHW[i]=double(mrand())/10768.0;
	};
	double smax=0;
	double smin=0;
	double smaxw=0;
	double sminw=0;
	for(i=0;i<256;i++){
		double Ang=(PI*i)/128;
		double s=0;
		for(int j=0;j<NFUR;j++)s+=sin((j+4)*Ang+PH[j])*HR[j];
		if(s>smax)smax=s;
		if(s<smin)smin=s;
		HillR[i]=s;
		s=0;
		for(j=0;j<NFUR;j++)s+=sin((j+4)*Ang+PH[j])*HR[j];
		if(s>smaxw)smaxw=s;
		if(s<sminw)sminw=s;
		HillW[i]=s;
	};
	if(smax!=smin){
		double amp=smax-smin;
		double ampw=smaxw-sminw;
		for(i=0;i<256;i++){
			HillR[i]=(HillR[i]-smin)/amp;
			HillW[i]=0.8+((HillW[i]-sminw)*0.4/ampw);
		};
	};
};
void InitHillAmp1(){
	double HR[NFUR];
	double PH[NFUR];
	double HRW[NFUR];
	double PHW[NFUR];
	int NFR=5;
	for(int i=0;i<NFR;i++){
		HR[i]=double(mrand())/32768.0/(i+2);
		PH[i]=double(mrand())/10768.0;
		HRW[i]=double(mrand())/32768.0/(i+2);
		PHW[i]=double(mrand())/10768.0;
	};
	double smax=0;
	double smin=0;
	double smaxw=0;
	double sminw=0;
	for(i=0;i<256;i++){
		double Ang=(PI*i)/128;
		double s=0;
		for(int j=0;j<NFR;j++)s+=sin((j+2)*Ang+PH[j])*HR[j];
		if(s>smax)smax=s;
		if(s<smin)smin=s;
		HillR[i]=s;
		s=0;
		for(j=0;j<NFR;j++)s+=sin((j+2)*Ang+PH[j])*HR[j];
		if(s>smaxw)smaxw=s;
		if(s<sminw)sminw=s;
		HillW[i]=s;
	};
	if(smax!=smin){
		double amp=smax-smin;
		double ampw=smaxw-sminw;
		for(i=0;i<256;i++){
			HillR[i]=(1+(HillR[i]-smin)/amp)/2;
			HillW[i]=0.6+((HillW[i]-sminw)*0.4/ampw);
		};
	};
};
//double HILLHI[32]={0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,
//                  0.00,0.01,0.02,0.11,0.21,0.27,0.35,0.41,
//				  0.50,0.64,0.82,0.93,0.98,1.00,1.00,1.00,
//				  1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00};
double HILLHI[32]={0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,
                  0.00,0.01,0.01,0.03,0.08,0.13,0.23,0.36,
				  0.50,0.64,0.78,0.88,0.94,0.97,1.00,1.00,
				  1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00};

double GetHillHi(double r,double phase,double r0,double Width){
	int pidx=int(phase*255);
	if(pidx<0)pidx=0;
	if(pidx>255)pidx=255;
	double ang=PI*2*phase;
	double mul=0.7+HillR[pidx]*0.6;
	Width*=HillW[pidx];
	//r*=mul;
	r0*=mul;
	double dr=r-r0;
	if(dr>Width)return 0;
	else if(dr<-Width)return 1;
	int idx=int(16*(1-dr/Width));
	if(idx<0)idx=0;
	if(idx>30)idx=30;
	//if(idx>16)
	return HILLHI[idx];//+double(rand()&15)*double(24-idx)/5400;
	//else return HILLHI[idx]+double(rand()&15)*double(idx)/3600;
};
void DoGen(){
	
};

void CreateMiniMap();
void ClearRender();
void GenerateByStyle(char* Style,bool);
void GenerateHill(int Thick){
	
};
void LimitZones();
void CreateLandLocking(int TAlp,bool ForVision);
void CreateFractal();
byte* REND;
void RandomMapDialog(char* Result);
extern int RES[8][8];
extern int NNations;
void CreateNationalMaskForRandomMap(char* Name){
	LOADNATMASK=0xFFFFFFFF;
	return;
	char cc1[128];
	char cc2[32]="00000000";
	word NatUnit[8]={0,0,0,0,0,0,0,0};
	int x1,x2;
	sscanf(Name,"%s%x%x%s",cc1,&x1,&x2,cc2);
	LOADNATMASK=0;
	for(int i=0;i<8;i++){
		char c=cc2[7-i];
		int id=0;
		if(c>='0'&&c<='9')id=c-'0';
		else if(c>='A'&&c<='Z')id=c-'A'+10;
		if(id){
			id--;
			char* pname=GlobalAI.Ai[id].Peasant_ID;
			int ntt=-1;
			for(int j=0;j<NNations;j++)if(strstr(pname,NatCharLo[j])||strstr(pname,NatCharHi[j]))ntt=j;
			if(ntt!=-1){
				LOADNATMASK|=1<<ntt;
			};
		};
	};
};
extern int RM_LandType;
extern int RM_Resstart;
extern int RM_Restot;
char CurTerrName[48];
extern int ADDRES[8];
void SetupArrays();
void MakeDesert();
extern int TEXARR[8];
void LoadFacturesSet(int set);
void CreateMapByName(char* Name){
	ADDSH=Name[2]-'0'+1;
	SetupArrays();
	char cc1[128];
	char cc2[32]="00000000";
	word NatUnit[8]={0,0,0,0,0,0,0,0};
	int x1,x2;
	int ADD_PARAM=0;
	sscanf(Name,"%s%x%x%s%d",cc1,&x1,&x2,cc2,&ADD_PARAM);
	for(int i=0;i<8;i++){
		char c=cc2[7-i];
		int id=0;
		if(c>='0'&&c<='9')id=c-'0';
		else if(c>='A'&&c<='Z')id=c-'A'+10;
		if(id){
			id--;
			char* pname=GlobalAI.Ai[id].Peasant_ID;
			int nu=NATIONS[i].NMon;
			GeneralObject** GOS=NATIONS[i].Mon; 
			for(int k=0;k<nu&&strcmp(GOS[k]->MonsterID,pname);k++);
			if(k<nu)NatUnit[i]=k;
			else{
				NatUnit[k]=0;
				sprintf(cc1,"Creating map: Unknown unit: %s",pname);
				ErrM(cc1);
			};
		}else{
			NatUnit[i]=0xFFFF;
		};
	};
	
	RandomMapDesc RD;
	RD.Load("Terrain\\res.dat");
	int Style=x2&15;
	int Play=(x2>>4)&15;
	int Mount=(x2>>8)&15;
	int Resst=(x2>>12)&15;
	int Resmn=(x2>>16)&15;
	int ClmZone=(x2>>20)&15;
	for(i=0;i<8;i++){
		for(int p=0;p<8;p++)RES[i][p]=RD.RES[Resst].RES[p];
	};
	RM_LandType=Style;
	RM_Resstart=Resst;
	RM_Restot=Resmn;
	mrpos=x1&8191;
	XXP=0;
	char FRAC[128];
	sprintf(FRAC,"Terrain\\fract_set%d.txt",ClmZone);
	GFILE* FF=Gopen(FRAC,"r");
	if(FF){
		for(i=0;i<8;i++){
			int n;
			Gscanf(FF,"%d",&n);
			TEXARR[i]=n;
		};
		Gclose(FF);
	};
	LoadFacturesSet(ClmZone);
	sprintf(cc1,"Terrain\\%s",RD.STY[Style].Players[Play].name);
	CurTerrName[0]=0;
	GFILE* F=Gopen("Terrain\\terr2.dat","r");
	if(F){
		if(F){
			int cz,n1,n2,z;
			char ccc[48];
			do{
				z=Gscanf(F,"%d%d%d%s",&cz,&n1,&n2,ccc);
				if(z==4){
					if(cz==ClmZone&&n1==ADDSH&&n2==Mount)strcpy(CurTerrName,ccc);
				};
			}while(z==4);
			Gclose(F);
		};
	}else{
		F=Gopen("Terrain\\terrlist.dat","r");
		if(F){
			int n1,n2,z;
			char ccc[48];
			do{
				z=Gscanf(F,"%d%d%s",&n1,&n2,ccc);
				if(z==3){
					if(n1==ADDSH&&n2==Mount)strcpy(CurTerrName,ccc);
				};
			}while(z==3);
			Gclose(F);
		};
	};
	//CreateRandomTerrain(cc1,RD.STY[Style].Players[Play].NPlayers,Mount,RD.MINES[Resmn].Ng,RD.MINES[Resmn].Ni,RD.MINES[Resmn].Nc,NatUnit,ADD_PARAM,ClmZone);
	//if(Mount==RD.NRelief-1)MakeDesert();
	for(i=0;i<8;i++){
		for(int p=0;p<8;p++)RES[i][p]+=ADDRES[p];
	};
};
void AddHill(){
	char ccc[128];
	RandomMapDialog(ccc);
	if(ccc[0]){
		CreateMapByName(ccc);
	};
};
void CreateRandomMap(char* Name){
	GPROG.Setup();
	int S0=1<<(ADDSH+ADDSH);
	GPROG.AddPosition('WATG',30,30);
	GPROG.AddPosition('WBRI',8*S0,1);
	GPROG.AddPosition('COST',3*S0,1);
	GPROG.AddPosition('NTDL',15*S0,1);
	GPROG.AddPosition('STRS',50,1);
	GPROG.AddPosition('TERR',20*S0,1);
	GPROG.AddPosition('FTEX',5*S0,1);
	GPROG.AddPosition('LINK',10*S0,1);
	GPROG.AddPosition('TRES',60,1);
	GPROG.AddPosition('CORD',10*sqrt(S0),1);
	GPROG.AddPosition('SETH',20*S0,1);
	GPROG.AddPosition('NATR',10*S0,1);
	GPROG.AddPosition('SAVM',30*S0,1);
	GPROG.AddPosition('LOAD',6*S0,1);
	CreateMapByName(Name);
	ProcessMessages();
	GPROG.SetCurrentStage('SAVM');
	ProcessMessages();
	Save3DMap(Name);
	GPROG.SetCurrentStage('LOAD');
	ProcessMessages();
	GPROG.Setup();
};
void CreateNationalMaskForRandomMap(char*);
void UnLoading();
void CheckMapName(char* Name){
	if(Name[0]=='R'&&Name[1]=='N'&&Name[3]==' '){
		ResFile F=RReset(Name);
		if(F==INVALID_HANDLE_VALUE){
			UnLoading();
			CreateNationalMaskForRandomMap(Name);
			for(int i=0;i<8;i++){
				LoadAllNations(1<<i,i);
			};
			CreateRandomMap(Name);
			UnLoading();
		}else RClose(F);
	};
};
void CheckMapNameForStart(char* Name){
	if(Name[0]=='R'&&Name[1]=='N'&&Name[3]==' '){
		ResFile F=RReset(Name);
		if(F==INVALID_HANDLE_VALUE){
			UnLoading();
			CreateNationalMaskForRandomMap(Name);
			for(int i=0;i<8;i++){
				LoadAllNations(1<<i,i);
			};
			CreateRandomMap(Name);
		}else RClose(F);
	};
};
//----------------------------Undo---------------------------
class UndoSegment{
public:
	byte* Data;
	int MaxData;
	int Size;
	int pos;
	UndoSegment();
	~UndoSegment();
	void Close();
	void Add(byte val);
	void Add(short val);
	void Add(int val);
	bool Read(byte* val);
	bool Read(short* val);
	bool Read(int* val);
};
UndoSegment::UndoSegment(){
	Data=NULL;
	MaxData=0;
	Size=0;
	pos=0;
};
void UndoSegment::Close(){
	if(Data)delete(Data);
	Data=NULL;
	MaxData=0;
	Size=0;
	pos=0;
};
UndoSegment::~UndoSegment(){
	Close();
};
void UndoSegment::Add(byte val){
	if(Size+6>=MaxData){
		MaxData+=16384;
		Data=(byte*)realloc(Data,MaxData);
	};
	Data[Size]=val;
	Size++;
};
void UndoSegment::Add(short val){
	if(Size+6>=MaxData){
		MaxData+=16384;
		Data=(byte*)realloc(Data,MaxData);
	};
	((short*)(Data+Size))[0]=val;
	Size+=2;
};
void UndoSegment::Add(int val){
	if(Size+6>=MaxData){
		MaxData+=16384;
		Data=(byte*)realloc(Data,MaxData);
	};
	((int*)(Data+Size))[0]=val;
	Size+=4;
};
bool UndoSegment::Read(byte* val){
	if(pos>=Size)return false;
	*val=Data[pos];
	pos++;
	return true;
};
bool UndoSegment::Read(short* val){
	if(pos>=Size)return false;
	*val=((short*)(Data+pos))[0];
	pos+=2;
	return true;
};
bool UndoSegment::Read(int* val){
	if(pos>=Size)return false;
	*val=((int*)(Data+pos))[0];
	pos+=4;
	return true;
};
UndoSegment UNDO1;
void SetTexture(int Vert,int nm);
void MarkPointToDraw(int i);
void EraseSprite(int Index);
extern byte* FactureMap;
extern byte* FactureWeight;
void MakeUndo(){
	bool DoUndo=true;
	while(DoUndo){
		byte Type;
		if(DoUndo=UNDO1.Read(&Type)){
			int vint;
			byte vbyte;
			byte fc,fw;
			byte et,wt;
			short vshort;
			switch(Type){
			case 0://sect
				break;
			case 1://height&texure
#ifdef _USE3D
				if(UNDO1.Read(&vint)&&UNDO1.Read(&vshort)&&UNDO1.Read(&vbyte)&&UNDO1.Read(&fc)&&UNDO1.Read(&fw)&&UNDO1.Read(&et)&&UNDO1.Read(&wt)){
					MarkPointToDraw(vint);
#else
				if(UNDO1.Read(&vint)&&UNDO1.Read(&vshort)&&UNDO1.Read(&vbyte)){
#endif
					
#ifdef _USE3D
					
#endif
					MarkPointToDraw(vint);
				};
				break;
			case 2://object;
				{
					short ID;
					UNDO1.Read(&ID);
					OneObject* OB=Group[ID];
					if(OB){
						OB->delay=6000;
						if(OB->LockType==1)OB->RealDir=32;
						OB->Die();
						OB=Group[ID];
						if(OB){
							OB->Sdoxlo=65000;
							if(OB->NewBuilding){
								void EliminateBuilding(OneObject* OB);
								EliminateBuilding(OB);
								DelObject(OB);
								Group[OB->Index]=NULL;
							}
						}
					};
				};
				break;
			case 3://sprite
				{
					int Index;
					UNDO1.Read(&Index);
					EraseSprite(Index);
				};
				break;
			};
		};
	};
	UNDO1.Close();
};

void ChangeSect(int Sect,byte val){
	
};
void ChangeVert(int Vert,short Height,byte Tex){
	if(Vert<0||Vert>=MaxPointIndex)return;
	UNDO1.Add(byte(1));
	UNDO1.Add(Vert);
	UNDO1.Add(THMap[Vert]);
	UNDO1.Add(TexMap[Vert]);
};
void ChangeObject(short Index){
	UNDO1.Add(byte(2));
	UNDO1.Add(Index);
};
void ChangeSprite(int Index){
	UNDO1.Add(byte(3));
	UNDO1.Add(Index);
};
//--------------------Including samples----------------
//SMP-file format
//SMPF
//int Lx,Ly (pix)
//Blocks:
//int BlockID
//int size
//.....
//int -1

//vertex format:
//"VERT"
//int Size
//int NVert
//-------------------
//short dx
//short dy
//byte texture
//byte s1,s2,s3
//short Height
bool ImmVis=false;
bool AllowUndo=false;

void RM_SaveVertices(ResFile F,int x0,int y0,int Lx,int Ly,int* KeyX,int* KeyY){
	
};
void SetTexture(int Vert,int nm);
void MarkPointToDraw(int i);
bool CheckHI=0;
void RM_LoadVertices(ResFile F,int Vsx,int Vsy,bool Vnew=0){
	
};
void RM_LoadVerticesEmpty(ResFile F,int Vsx,int Vsy,bool Vnew=0){
	
};
void RM_LoadVerticesPos(ResFile F,short** VX,short** VY,int* lpN,bool Vnew=0){
	
};
void RM_CalcVerticesPos(ResFile F,short* VX,short* VY,bool Vnew=0){
	
};
//Save sprites
//short dx,dy
//byte SprGroup
//word Index

//-----------------------------------------------------------
void addSpriteAnyway(int x,int y,SprGroup* SG,word id);
extern SprGroup WALLS;
void RM_SaveSprites(ResFile F,int x0,int y0,int Lx,int Ly,int xc,int yc){
	int sign='TRPS';
	RBlockWrite(F,&sign,4);
	int ns=0;
	for(int i=0;i<MaxSprt;i++){
		OneSprite* OS=Sprites+i;
		if(OS->Enabled){
			int x=OS->x;
			int y=OS->y;
			if(x>x0&&y>y0&&x<x0+Lx&&y<y0+Ly)ns++;
		};
	};
	i=(ns*12)+12;
	RBlockWrite(F,&i,4);
	RBlockWrite(F,&ns,4);

	word j='GA';
	for(i=0;i<MaxSprt;i++){
		OneSprite* OS=&Sprites[i];
		if(OS->Enabled){
			int x=OS->x;
			int y=OS->y;
			if(x>x0&&y>y0&&x<x0+Lx&&y<y0+Ly){
				j=0;	
				if(OS->SG==&TREES)j='GA';else
				if(OS->SG==&STONES)j='TS';else
				if(OS->SG==&HOLES)j='OH';else
				if(OS->SG==&WALLS)j='WL';else
				if(OS->SG==&COMPLEX)j='OC';
				RBlockWrite(F,&j,2);
				int xx=OS->x-xc;
				RBlockWrite(F,&xx,4);
				xx=OS->y-yc;
				RBlockWrite(F,&xx,4);
				RBlockWrite(F,&OS->SGIndex,2);
			};
		};
	};
};
extern bool SpriteSuccess;
extern int LastAddSpr;
void RM_LoadGSprites(ResFile F,int xc,int yc);
void RM_LoadGSpritesEmpty(ResFile F,int xc,int yc);
void RM_LoadGSprites(ResFile F,int xc,int yc);
void RM_LoadSprites(ResFile F,int xc,int yc){
	int ns;
	RBlockRead(F,&ns,4);
	int maxx=(msx<<5)-32;
	int maxy=(msy<<5)-32;
	for(int i=0;i<ns;i++){
		int x,y;
		word sign,id;
		RBlockRead(F,&sign,2);
		RBlockRead(F,&x,4);
		RBlockRead(F,&y,4);
		RBlockRead(F,&id,2);
		x+=xc;
		y+=yc;
		if(x>0&&y>0&&x<maxx&&y<maxy){
			SpriteSuccess=0;
			if(sign=='GA'){
				addSpriteAnyway(x,y,&TREES,id);
			}else if(sign=='TS'){
				addSpriteAnyway(x,y,&STONES,id);
			}else if(sign=='OH'){
				addSpriteAnyway(x,y,&HOLES,id);
			}else if(sign=='WL'){
				addSpriteAnyway(x,y,&WALLS,id);
			}else if(sign=='OC'){
				addSpriteAnyway(x,y,&COMPLEX,id);
			};
			if(AllowUndo&&SpriteSuccess)ChangeSprite(LastAddSpr);
		};
	};
};
void RM_LoadSpritesEmpty(ResFile F,int xc,int yc){
	int ns;
	RBlockRead(F,&ns,4);
	int maxx=(msx<<5)-32;
	int maxy=(msy<<5)-32;
	for(int i=0;i<ns;i++){
		int x,y;
		word sign,id;
		RBlockRead(F,&sign,2);
		RBlockRead(F,&x,4);
		RBlockRead(F,&y,4);
		RBlockRead(F,&id,2);
	};
};

void RM_SaveObj(ResFile F,int x0,int y0,int Lx,int Ly,int xc,int yc){
	int sign='SJBO';
	RBlockWrite(F,&sign,4);
	int ns=0;
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB&&!OB->Sdoxlo){
			int x=OB->RealX>>4;
			int y=OB->RealY>>4;
			if(x>x0&&y>y0&&x<x0+Lx&&y<y0+Ly)ns++;
		};
	};
	char ONAME[31];
	i=ns*(32+4+4)+12;
	RBlockWrite(F,&i,4);
	RBlockWrite(F,&ns,4);
	for(i=0;i<MAXOBJECT;i++){
		OneSprite* OS=&Sprites[i];
		OneObject* OB=Group[i];
		if(OB&&!OB->Sdoxlo){
			int x=OB->RealX>>4;
			int y=OB->RealY>>4;
			if(x>x0&&y>y0&&x<x0+Lx&&y<y0+Ly){
				memset(ONAME,0,31);
				strcpy(ONAME,OB->Ref.General->MonsterID);
				int xx=(OB->RealX>>4)-xc;
				RBlockWrite(F,&xx,4);
				xx=(OB->RealY>>4)-yc;
				RBlockWrite(F,&xx,4);
				RBlockWrite(F,&OB->NNUM,1);
				RBlockWrite(F,ONAME,31);

			};
		};
	};
};

void CreateNewUnitAt(byte NI,int x,int y,word Type,word Life,word Stage);
extern int LASTADDID;
void RM_LoadObj(ResFile F,int xc,int yc){
	int ns;
	RBlockRead(F,&ns,4);
	char ONAME[32]="";
	for(int i=0;i<ns;i++){
		int xm,ym;
		RBlockRead(F,&xm,4);
		RBlockRead(F,&ym,4);
		byte NT;
		RBlockRead(F,&NT,1);
		NT=MyNation;
		RBlockRead(F,ONAME,31);
		GeneralObject** GO=NATIONS[NT].Mon;
		int Nmon=NATIONS[NT].NMon;
		for(int p=0;p<Nmon;p++){
			if(!strcmp(GO[p]->MonsterID,ONAME)){
				if(!GO[p]->newMons->Wall){
					CreateNewUnitAt(NT,(xc+xm)<<4,(yc+ym)<<4,p,GO[p]->MoreCharacter->Life,GO[p]->MoreCharacter->ProduceStages);
					if(LASTADDID!=0xFFFF&&AllowUndo){
						ChangeObject(LASTADDID);
					};
					goto ttt;
				};
			};
		};
ttt:;
	};
};
void RM_GetObjXY(ResFile F,int* xc,int* yc){
	int ns;
	RBlockRead(F,&ns,4);
	char ONAME[32];
	*xc=0;
	*yc=0;
	for(int i=0;i<ns;i++){
		int xm,ym;
		RBlockRead(F,&xm,4);
		RBlockRead(F,&ym,4);
		*xc=xm;
		*yc=ym;
		byte NT;
		RBlockRead(F,&NT,1);
		NT=MyNation;
		RBlockRead(F,ONAME,31);
	};
};
void RM_LoadObjEmpty(ResFile F,int xc,int yc){
	int ns;
	RBlockRead(F,&ns,4);
	char ONAME[32];
	for(int i=0;i<ns;i++){
		int xm,ym;
		RBlockRead(F,&xm,4);
		RBlockRead(F,&ym,4);
		byte NT;
		RBlockRead(F,&NT,1);
		NT=MyNation;
		RBlockRead(F,ONAME,31);
ttt:;
	};
};
void RM_SaveZonesAndGroups(ResFile F,int x0,int y0,int Lx,int Ly,int xc,int yc){
	/*
	int NAZ=0;
	for(int i=0;i<AZones.GetAmount();i++){
		int x=AZones[i].x;
		int y=AZones[i].y;
		y=(y>>1)-GetHeight(x,y);
		if(x>=x0&&y>=y0&&x<x0+Lx&&y<y0+Ly)NAZ++;
	};
	int NAG=0;
	for(i=0;i<AGroups.GetAmount();i++){
		ActiveGroup* AG=AGroups[i];
		int N=AG->Units.GetAmount();
		for(int j=0;j<N;j++){
			word MID=AG->Units[j].ID;
			if(MID!=0xFFFF){
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==AG->Units[j].SN&&!OB->Sdoxlo){
					int x=OB->RealX>>4;
					int y=(OB->RealY>>5)-OB->RZ;
					if(x>=x0&&y>=y0&&x<x0+Lx&&y<y0+Ly){
						NAG++;
						j=N;
					};
				};
			};
		};
	};
	if(NAG||NAZ){
		i='ENOZ';
		RBlockWrite(F,&i,4);
		i=0;
		RBlockWrite(F,&i,4);
		RBlockWrite(F,&NAZ,4);
		RBlockWrite(F,&NAG,4);
		NAZ=0;
		for(i=0;i<NAZones;i++){
			int x=AZones[i].x;
			int y=AZones[i].y;
			y=(y>>1)-GetHeight(x,y);
			if(x>=x0&&y>=y0&&x<x0+Lx&&y<y0+Ly){
				int dx=AZones[i].x-xc;
				RBlockWrite(F,&dx,4);

				dx=AZones[i].y-yc;
				RBlockWrite(F,&dx,4);

				RBlockWrite(F,&AZones[i].R,4);

				RBlockWrite(F,&AZones[i].Dir,1);
				char zn[128];
				strcpy(zn,AZones[i].Name);
				char* c=strchr(zn,'~');
				if(c)c[0]=0;
				int L=strlen(zn)+1;
				RBlockWrite(F,&L,2);
				RBlockWrite(F,zn,L);
			};
		};
		for(i=0;i<NAGroups;i++){
			int N=AGroups[i].N;
			word* IDS=AGroups[i].Units;
			word* SNS=AGroups[i].Serials;
			for(int j=0;j<N;j++){
				word MID=IDS[j];
				if(MID!=0xFFFF){
					OneObject* OB=Group[MID];
					if(OB&&OB->Serial==SNS[j]&&!OB->Sdoxlo){
						int x=OB->RealX>>4;
						int y=(OB->RealY>>5)-OB->RZ;
						if(x>=x0&&y>=y0&&x<x0+Lx&&y<y0+Ly){
							int NUN=0;
							for(int p=0;p<N;p++){
								word MID=IDS[p];
								if(MID!=0xFFFF){
									OneObject* OB=Group[MID];
									if(OB&&OB->Serial==SNS[p]&&!OB->Sdoxlo)NUN++;
								};
							};
							char zn[128];
							strcpy(zn,AGroups[i].Name);
							char* c=strchr(zn,'~');
							if(c)c[0]=0;
							int L=strlen(zn)+1;
							RBlockWrite(F,&L,2);
							RBlockWrite(F,zn,L);
							RBlockWrite(F,&NUN,2);
							for(p=0;p<N;p++){
								word MID=IDS[p];
								if(MID!=0xFFFF){
									OneObject* OB=Group[MID];
									if(OB&&OB->Serial==SNS[p]&&!OB->Sdoxlo){
										int dx=(OB->RealX>>4)-xc;
										int dy=(OB->RealY>>4)-yc;
										RBlockWrite(F,&dx,4);
										RBlockWrite(F,&dy,4);
									};
								};
							};
							j=N;
						};
					};
				};
			};
		};
	};
	*/
};
/*class SaveBuf{
public:
	byte* Buf;
	int Pos;
	int Size;
	int RealSize;
	SaveBuf();
	void Init();
	~SaveBuf();
	void Clear();
	void SaveToFile(ResFile f1);
	void LoadFromFile(ResFile f1);
};*/
int CUR_ZG_ID=0;
int CreateFreeActiveGroup(char* gName);
void xBlockRead(SaveBuf* SB,void* Data,int Size);
int CreateNewZone(char* Name,int x,int y,int R);
void FAST_RM_LoadGSprites(SaveBuf*SB,int xc,int yc);
void FAST_RM_LoadZonesAndGroups(SaveBuf* SB,int xc,int yc){
	/*
	int NAZ,NAG;
	CUR_ZG_ID++;
	xBlockRead(SB,&NAZ,4);
	xBlockRead(SB,&NAG,4);
	for(int i=0;i<NAZ;i++){
		int x,y,R;
		byte Dir;
		word L;
		char STR[1024];
		xBlockRead(SB,&x,4);
		xBlockRead(SB,&y,4);
		xBlockRead(SB,&R,4);
		xBlockRead(SB,&Dir,1);
		xBlockRead(SB,&L,2);
		xBlockRead(SB,STR,L);
		if(CUR_ZG_ID<0x10)sprintf(STR+strlen(STR),"~000%X",CUR_ZG_ID);
		else if(CUR_ZG_ID<0x100)sprintf(STR+strlen(STR),"~00%X",CUR_ZG_ID);
		else if(CUR_ZG_ID<0x1000)sprintf(STR+strlen(STR),"~0%X",CUR_ZG_ID);
		else sprintf(STR+strlen(STR),"~%X",CUR_ZG_ID);
		CreateNewZone(STR,xc+x,yc+y,R);
	};
	for(i=0;i<NAG;i++){
		word L;
		char STR[1024];
		int NUN=0;
		xBlockRead(SB,&L,2);
		xBlockRead(SB,STR,L);
		xBlockRead(SB,&NUN,2);

		if(CUR_ZG_ID<0x10)sprintf(STR+strlen(STR),"~000%X",CUR_ZG_ID);
		else if(CUR_ZG_ID<0x100)sprintf(STR+strlen(STR),"~00%X",CUR_ZG_ID);
		else if(CUR_ZG_ID<0x1000)sprintf(STR+strlen(STR),"~0%X",CUR_ZG_ID);
		else sprintf(STR+strlen(STR),"~%X",CUR_ZG_ID);

		int idx=CreateFreeActiveGroup(STR);
		AGroups[idx].Units=(word*)malloc(NUN<<1);
		AGroups[idx].Serials=(word*)malloc(NUN<<1);
		int NU=0;
		for(int j=0;j<NUN;j++){
			int x,y;
			xBlockRead(SB,&x,4);
			xBlockRead(SB,&y,4);
			x+=xc;
			y+=yc;
			int MINR=10;
			word BID=0xFFFF;
			for(int p=0;p<MAXOBJECT;p++){
				OneObject* OB=Group[p];
				if(OB&&!OB->Sdoxlo){
					int xu=OB->RealX>>4;
					int yu=OB->RealY>>4;
					int N=Norma(xu-x,yu-y);
					if(N<3&&N<MINR){
						BID=p;
						MINR=N;
					};
				};
			};
			if(BID!=0xFFFF){
				AGroups[idx].Units[NU]=BID;
				AGroups[idx].Serials[NU]=Group[BID]->Serial;
				NU++;
			};
		};
		AGroups[idx].N=NU;
	};
	*/
};
void RM_LoadZonesAndGroups(ResFile SB,int xc,int yc){
	/*
	int NAZ,NAG;
	CUR_ZG_ID++;
	char cn[64];
	{
STSCAN:
		if(CUR_ZG_ID<0x10)sprintf(cn,"~000%X",CUR_ZG_ID);
		else if(CUR_ZG_ID<0x100)sprintf(cn,"~00%X",CUR_ZG_ID);
		else if(CUR_ZG_ID<0x1000)sprintf(cn,"~0%X",CUR_ZG_ID);
		else sprintf(cn,"~%X",CUR_ZG_ID);
		for(int j=0;j<NAZones||j<NAGroups;j++){
			if(j<NAZones&&strstr(AZones[j].Name,cn)){
				CUR_ZG_ID++;
				goto STSCAN;
			};
			if(j<NAGroups&&strstr(AGroups[j].Name,cn)){
				CUR_ZG_ID++;
				goto STSCAN;
			};
		};
	};
	RBlockRead(SB,&NAZ,4);
	RBlockRead(SB,&NAG,4);
	for(int i=0;i<NAZ;i++){
		int x,y,R;
		byte Dir;
		word L;
		char STR[1024];
		RBlockRead(SB,&x,4);
		RBlockRead(SB,&y,4);
		RBlockRead(SB,&R,4);
		RBlockRead(SB,&Dir,1);
		RBlockRead(SB,&L,2);
		RBlockRead(SB,STR,L);
		if(CUR_ZG_ID<0x10)sprintf(STR+strlen(STR),"~000%X",CUR_ZG_ID);
		else if(CUR_ZG_ID<0x100)sprintf(STR+strlen(STR),"~00%X",CUR_ZG_ID);
		else if(CUR_ZG_ID<0x1000)sprintf(STR+strlen(STR),"~0%X",CUR_ZG_ID);
		else sprintf(STR+strlen(STR),"~%X",CUR_ZG_ID);
		CreateNewZone(STR,xc+x,yc+y,R);
	};
	for(i=0;i<NAG;i++){
		word L;
		char STR[1024];
		int NUN=0;
		RBlockRead(SB,&L,2);
		RBlockRead(SB,STR,L);
		RBlockRead(SB,&NUN,2);

		if(CUR_ZG_ID<0x10)sprintf(STR+strlen(STR),"~000%X",CUR_ZG_ID);
		else if(CUR_ZG_ID<0x100)sprintf(STR+strlen(STR),"~00%X",CUR_ZG_ID);
		else if(CUR_ZG_ID<0x1000)sprintf(STR+strlen(STR),"~0%X",CUR_ZG_ID);
		else sprintf(STR+strlen(STR),"~%X",CUR_ZG_ID);

		int idx=CreateFreeActiveGroup(STR);
		AGroups[idx].Units=(word*)malloc(NUN<<1);
		AGroups[idx].Serials=(word*)malloc(NUN<<1);
		int NU=0;
		for(int j=0;j<NUN;j++){
			int x,y;
			RBlockRead(SB,&x,4);
			RBlockRead(SB,&y,4);
			x+=xc;
			y+=yc;
			int MINR=10;
			word BID=0xFFFF;
			for(int p=0;p<MAXOBJECT;p++){
				OneObject* OB=Group[p];
				if(OB&&!OB->Sdoxlo){
					int xu=OB->RealX>>4;
					int yu=OB->RealY>>4;
					int N=Norma(xu-x,yu-y);
					if(N<3&&N<MINR){
						BID=p;
						MINR=N;
					};
				};
			};
			if(BID!=0xFFFF){
				AGroups[idx].Units[NU]=BID;
				AGroups[idx].Serials[NU]=Group[BID]->Serial;
				NU++;
			};
		};
		AGroups[idx].N=NU;
	};
	*/
};
extern BlockBars LockBars;
extern BlockBars UnLockBars;
void RM_SaveLock(ResFile F,int x0,int y0,int Lx,int Ly,int xc,int yc){
	return;
	int NL=0;
	int NU=0;
	for(int i=0;i<LockBars.NBars;i++){
		int x=(int(LockBars.BC[i].x)<<6)+32;
		int y=(int(LockBars.BC[i].y)<<5)+16;
		if(x>x0&&y>y0&&x<x0+Lx&&y<y0+Ly){
			NL++;
		};
	};
	for(i=0;i<UnLockBars.NBars;i++){
		int x=(int(UnLockBars.BC[i].x)<<6)+32;
		int y=(int(UnLockBars.BC[i].y)<<5)+16;
		if(x>x0&&y>y0&&x<x0+Lx&&y<y0+Ly){
			NU++;
		};
	};
	i='KCOL';
	RBlockWrite(F,&i,4);
	i=16+2*(NU+NL);
	RBlockWrite(F,&i,4);
	RBlockWrite(F,&NL,4);
	RBlockWrite(F,&NU,4);
	for(i=0;i<LockBars.NBars;i++){
		int xb=LockBars.BC[i].x;
		int yb=LockBars.BC[i].y;
		int x=(int(xb)<<6)+32;
		int y=(int(yb)<<6)+32;
		if(x>x0&&y>y0&&x<x0+Lx&&y<y0+Ly){
			xb-=xc;
			yb-=yc;
			RBlockWrite(F,&xb,2);
			RBlockWrite(F,&yb,2);
		};
	};
	for(i=0;i<UnLockBars.NBars;i++){
		int xb=UnLockBars.BC[i].x;
		int yb=UnLockBars.BC[i].y;
		int x=(int(xb)<<6)+32;
		int y=(int(yb)<<6)+32;
		if(x>x0&&y>y0&&x<x0+Lx&&y<y0+Ly){
			xb-=xc;
			yb-=yc;
			RBlockWrite(F,&xb,2);
			RBlockWrite(F,&yb,2);
		};
	};
};
void RM_LoadLock(ResFile F,int xc,int yc){
	int NL,NU;
	RBlockRead(F,&NL,4);
	RBlockRead(F,&NU,4);
	for(int i=0;i<NL;i++){
		short x,y;
		RBlockRead(F,&x,2);
		RBlockRead(F,&y,2);
		x+=xc;
		y+=yc;
		if(!AllowUndo)AddLockBar(x,y);
	};
	for(i=0;i<NU;i++){
		short x,y;
		RBlockRead(F,&x,2);
		RBlockRead(F,&y,2);
		x+=xc;
		y+=yc;
		if(!AllowUndo)AddUnLockbar(x,y);
	};
};
void RM_LoadLockEmpty(ResFile F,int xc,int yc){
	int NL,NU;
	RBlockRead(F,&NL,4);
	RBlockRead(F,&NU,4);
	for(int i=0;i<NL;i++){
		short x,y;
		RBlockRead(F,&x,2);
		RBlockRead(F,&y,2);
		x+=xc;
		y+=yc;
	};
	for(i=0;i<NU;i++){
		short x,y;
		RBlockRead(F,&x,2);
		RBlockRead(F,&y,2);
		x+=xc;
		y+=yc;
	};
};
int GetPixelsIntoBuf(int x,int y,int x1,int y1,int xr,int yr,byte** ptr);
void RM_SavePix(ResFile F,int x0,int y0,int Lx,int Ly,int xc,int yc){
	int sign='1XIP';
	RBlockWrite(F,&sign,4);
	byte* ptr=NULL;
	int sz=GetPixelsIntoBuf(x0>>5,y0>>5,(x0+Lx)>>5,(y0+Ly)>>5,xc>>5,yc>>5,&ptr);
	sign=sz+12;
	RBlockWrite(F,&sign,4);
	RBlockWrite(F,&sz,4);
	if(sz){
		RBlockWrite(F,ptr,sz);
		delete(ptr);
	};
};
void SetPixelsFromBuf(int xr,int yr,byte* Buf,int size);
void RM_LoadPix(ResFile F,int xc,int yc){
	int sz;
	RBlockRead(F,&sz,4);
	byte* ptr=znew(byte,sz);
	RBlockRead(F,ptr,sz);
	///SetPixelsFromBuf(xc>>5,yc>>5,ptr,sz);
	free(ptr);
};
void RM_LoadPixEmpty(ResFile F,int xc,int yc){
	int sz;
	RBlockRead(F,&sz,4);
	byte* ptr=znew(byte,sz);
	RBlockRead(F,ptr,sz);	
	free(ptr);
};
int GetGroundIntoBuf(int x,int y,int x1,int y1,int xr,int yr,byte** ptr);
void SetGroundFromBuf(int xr,int yr,byte* Buf,int size,int DZ);
void RM_SaveGround(ResFile F,int x0,int y0,int Lx,int Ly,int xc,int yc){
	
};
void RM_LoadGround(ResFile F,int xc,int yc){
	
};
void RM_LoadGroundEmpty(ResFile F,int xc,int yc){
	
};
void SaveSMSInPieces(ResFile F,int keyX,int keyY,int x0,int y0,int x1,int y1);

void RM_Save(char* Name,int x0,int y0,int x1,int y1) {
	Surface::SurfPiece::Save(Name, x0, y0, x1, y1);
/*	ResFile F=RRewrite(Name);
	if(F!=INVALID_HANDLE_VALUE){
		SaveBuf sb;
		void SavePiece(SaveBuf* sb,int x,int y,int x0,int y0,int x1,int y1);
		SavePiece(&sb,(x1+x0)/2,(y1+y0)/2,x0,y0,x1,y1);
		sb.SaveToFile(F);
		RClose(F);
	}*/
};
void RM_Load(char* Name,int x,int y) {
	Surface::SurfPiece::Load(Name, x, y);
/*	ResFile F=RReset(Name);
	if(F!=INVALID_HANDLE_VALUE){
		SaveBuf sb;
		sb.LoadFromFile(F);
		void LoadPiece(SaveBuf* sb,int x,int y);
		LoadPiece(&sb,x,y);
		RClose(F);
	};*/
};
void RM_Preview(char* Name,int x,int y) {
	Surface::SurfPiece::Preview(Name, x, y);
/*	ResFile F=RReset(Name);
	if(F!=INVALID_HANDLE_VALUE){
		SaveBuf sb;
		sb.LoadFromFile(F);
		void PreviewPiece(SaveBuf* sb,int x,int y);
		PreviewPiece(&sb,x,y);
		RClose(F);
	}*/
}
void RM_LoadNotObj(char* Name,int x,int y){
	
};
void RM_GetObjVector(char* Name,int* x,int* y){
	
};
bool GetCenter(char* Name,short* dx,short* dy){
	ResFile F=RReset(Name);
	*dx=0;
	*dy=0;
	if(F!=INVALID_HANDLE_VALUE){
		int SIGN,SIZE;
		RBlockRead(F,&SIGN,4);
		if(SIGN=='PMAS'){
			while(SIGN!=-1){
				SIGN=-1;
				RBlockRead(F,&SIGN,4);
				RBlockRead(F,&SIZE,4);
				switch(SIGN){
				case 'TREV':
					RM_CalcVerticesPos(F,dx,dy);
					break;
#ifdef _USE3D
				case '3REV':
					RM_CalcVerticesPos(F,dx,dy,1);
					break;
#endif
				case 'TRPS':
					SIGN=-1;
					break;
				case '1XIP':
				case 'KCOL':
					SIGN=-1;
					break;
				};
			};
		};
		RClose(F);
		return true;
	}else return false;
};
void RM_LoadVerticesOnly(char* Name,short** vx,short** vy,int * N){
	ResFile F=RReset(Name);
	if(F!=INVALID_HANDLE_VALUE){
		int SIGN,SIZE;
		RBlockRead(F,&SIGN,4);
		if(SIGN=='PMAS'){
			while(SIGN!=-1){
				SIGN=-1;
				RBlockRead(F,&SIGN,4);
				RBlockRead(F,&SIZE,4);
				switch(SIGN){
				case 'TREV':
					RM_LoadVerticesPos(F,vx,vy,N);
					break;
#ifdef _USE3D
				case '3REV':
					RM_LoadVerticesPos(F,vx,vy,N,1);
					break;
#endif
				case 'TRPS':
					SIGN=-1;
					break;
				case '1XIP':
				case 'KCOL':
					SIGN=-1;
					break;
				};
			};
		};
		RClose(F);
	};

};
int xRMp=-10000;
int yRMp=-10000;
void RM_PreLoadOpen(){
	xRMp=-10000;
	yRMp=-10000;
};
void RM_PreLoadClose(){
	MakeUndo();
	UNDO1.Close();
};
void RM_PreLoad(char* Name,int x,int y){
	//if((x>>6)==xRMp&&(y>>6)==yRMp)return;
	MakeUndo();
	

	AllowUndo=true;
	ImmVis=true;
	ResFile F=RReset(Name);
	if(F!=INVALID_HANDLE_VALUE){
		int SIGN,SIZE;
		RBlockRead(F,&SIGN,4);
		if(SIGN=='PMAS'){
			while(SIGN!=-1){
				SIGN=-1;
				RBlockRead(F,&SIGN,4);
				RBlockRead(F,&SIZE,4);
				switch(SIGN){
				case 'TREV':
					RM_LoadVertices(F,(x>>6)<<1,(y>>6)<<1);
					break;
#ifdef _USE3D
				case '3REV':
					RM_LoadVertices(F,(x>>6)<<1,(y>>6)<<1,1);
					break;
				case 'RPSG':
					RM_LoadGSprites(F,(x>>6)<<6,(y>>6)<<6);
					break;
#endif
				case 'KCOL':
					RM_LoadLock(F,x>>6,y>>6);
					break;
				case 'TRPS':
					RM_LoadSprites(F,(x>>6)<<6,(y>>6)<<6);
					break;
				case 'SJBO':
					RM_LoadObj(F,(x>>6)<<6,(y>>6)<<6);
					break;
				case '1XIP':
					RClose(F);
					AllowUndo=false;
					ImmVis=false;
					return;
				};
				/*
				switch(SIGN){
				case 'TREV':
					RM_LoadVertices(F,(x>>6)<<1,(y>>6)<<1);
					break;
				case 'TRPS':
					RClose(F);
					AllowUndo=false;
					ImmVis=false;
					return;
					//RM_LoadSprites(F,(x>>6)<<6,(y>>6)<<6);
				case 'KCOL':
					RClose(F);
					AllowUndo=false;
					ImmVis=false;
					return;
					//RM_LoadLock(F,x>>6,y>>6);
				};
				*/

			};
		};
		RClose(F);
	};
	AllowUndo=false;
	ImmVis=false;
};
char RN_FName[64]="sample";
void DelExt(){
	char* cc=strchr(RN_FName,'.');
	if(cc)cc[0]=0;
};
bool EnterStr(char * str,char* Message);
bool TexPieceMode=false;
void ProcessRM_Load(int x,int y){
	ImmVis=true;
	if(EnterStr(RN_FName,"Load sample")){
		DelExt();
		char cc[128];
		sprintf(cc,"UserPieces\\%s.smp",RN_FName);
		RM_PreLoadOpen();
		TexPieceMode=true;
		//RM_Load(cc,x,y);
	};
	ImmVis=false;
};
void ProcessRM_LoadEx(int x,int y,char* Name){
	strcpy(RN_FName,Name);
	DelExt();
	TexPieceMode=true;	
};
void ProcessRM_Save(int x0,int y0,int x1,int y1){
	if(EnterStr(RN_FName,"Save sample")){
		DelExt();
		char cc[128];
		CreateDirectory("UserPieces", NULL); // for the first time
		sprintf(cc,"UserPieces\\%s.smp",RN_FName);
		RM_Save(cc,x0,y0,x1,y1);
	};

};
extern int LastKey;
extern bool KeyPressed;
void ProcessTexPiece(int x,int y,bool press){
	if(KeyPressed&&LastKey==27){		
		TexPieceMode=false;
	};
	if(TexPieceMode){
		DelExt();
		char cc[128];
		sprintf(cc,"UserPieces\\%s.smp",RN_FName);
		if(press){
			RM_Load(cc,x,y);
			TexPieceMode=false;
		}else{
			RM_Preview(cc,x,y);
		}
	}
}
void ClearAllRounds();
void SetRandomPiece(char* Name){
	short* vx=NULL;
	short* vy=NULL;
	int Nv=0;
	int t=GetTickCount();
	RM_LoadVerticesOnly(Name,&vx,&vy,&Nv);
	t=GetTickCount()-t;
	if(Nv){
		//search for the place
		int NAttm=0;
		int xmax=msx>>1;
		int vmax=msx-3;
		//xmax-=20;
		//int xmax=msx>>2;
		//int vmax=msx>>1;
		while(NAttm<100){
			int x=(int(mrand())*xmax)>>15;
			int y=(int(mrand())*xmax)>>15;
			//checking
			x<<=1;
			y<<=1;
			bool Good=true;
			for(int j=0;j<Nv&&Good;j++){
				int vrx=x+vx[j];
				int vry=y+vy[j];
				if(vrx>2&&vrx<vmax&&vry>2&&vry<vmax){
					int v=vrx+vry*VertInLine;
					if(TexMap[v]>=128)Good=false;
				}else Good=false;
				j+=2;
			};
			if(Good){
				RM_Load(Name,x<<5,y<<5);
				NAttm=10000;
			};
			NAttm++;
		};
		delete(vx);
		delete(vy);
	};
};
void SetPieceInPoints(char* Name,int* xi,int* yi,int N,byte* Used,int NATT){
	short* vx=NULL;
	short* vy=NULL;
	int Nv=0;
	int t=GetTickCount();
	RM_LoadVerticesOnly(Name,&vx,&vy,&Nv);
	t=GetTickCount()-t;
	if(Nv){
		//search for the place
		int NAttm=0;
		int xmax=msx>>1;
		int vmax=msx-3;
		//xmax-=20;
		//int xmax=msx>>2;
		//int vmax=msx>>1;
		while(NAttm<NATT){
			int NP=(int(rand())*N)>>15;
			if(!Used[NP]){
				int x=xi[NP]>>6;
				int y=yi[NP]>>6;
				//checking
				x<<=1;
				y<<=1;
				bool Good=true;
				for(int j=0;j<Nv&&Good;j++){
					int vrx=x+vx[j];
					int vry=y+vy[j];
					if(vrx>2&&vrx<vmax&&vry>2&&vry<vmax){
						int v=vrx+vry*VertInLine;
						if(TexMap[v]>=128)Good=false;
					}else Good=false;
					j+=2;
				};
				if(Good){
					RM_Load(Name,x<<5,y<<5);
					NAttm=10000;
					Used[NP]=1;
					ShowProcessInfo();
				};
			};
			NAttm++;
		};
		delete(vx);
		delete(vy);
	};
};
extern int LastAddSpr;
void GenerateByStyle(char* Style,bool LIMIT){
	GFILE* f=Gopen(Style,"r");
	if(f){
		char cc[128];
		int N1,N2;
		int z;
		do{
			z=Gscanf(f,"%s%d%d",cc,&N1,&N2);
			if(z==3){
				int N=N1+((int(mrand())*(N2-N1))>>15);
				N<<=ADDSH+ADDSH-2;
				int NNames=0;
				int MaxNames=0;
				char** Names=NULL;
				GFILE* F1=Gopen(cc,"r");
				if(F1){
					int q;
					char cc1[128];
					do{
						q=Gscanf(F1,"%s",cc1);
						if(q==1){
							if(NNames>=MaxNames){
								MaxNames+=512;
								Names=(char**)realloc(Names,MaxNames<<2);
							};
							Names[NNames]=znew(char,strlen(cc1)+1);
							strcpy(Names[NNames],cc1);
							NNames++;
						};
					}while(q==1);
					Gclose(F1);
					if(NNames){
						for(int j=0;j<N;j++){
							SetRandomPiece(Names[(int(mrand())*NNames)>>15]);
							ShowProcessInfo();
							ProcessMessages();
							//if(LIMIT&&LastAddSpr>50000)j=N;
						};
						for(j=0;j<NNames;j++)delete(Names[j]);
						delete(Names);
					};
				};
			};
			if(z==1){
				if(!strcmp(cc,"TEXTURESONLY")){
					ClearAllRounds();
					ProcessMessages();
					z=3;
				};
			};
		}while(z==3);
		Gclose(f);
	};
};
int NNamesXX=0;
void GenerateInPoints(char* Piece,int* ObjX,int* ObjY,int NObj,int NATT){
	int NNamesXX=0;
	int MaxNames=0;
	char** Names=NULL;
	GFILE* F1=Gopen(Piece,"r");
	if(F1){
		int q;
		char cc1[128];
		do{
			q=Gscanf(F1,"%s",cc1);
			if(q==1){
				if(NNamesXX>=MaxNames){
					MaxNames+=512;
					Names=(char**)realloc(Names,MaxNames<<2);
				};
				Names[NNamesXX]=znew(char,strlen(cc1)+1);
				strcpy(Names[NNamesXX],cc1);
				NNamesXX++;
			};
		}while(q==1);
		Gclose(F1);
		if(NNamesXX){
			byte Used[4096];
			memset(Used,0,sizeof Used);
			for(int j=0;j<NATT;j++)
				SetPieceInPoints(Names[(int(rand())*NNamesXX)>>15],ObjX,ObjY,NObj,Used,200);
			for(j=0;j<NNamesXX;j++)delete(Names[j]);
			delete(Names);
		};
	};
};
//---------------------------------------------------------------
void UnregisterSprite(int N);
void DeleteInArea(int x0,int y0,int x1,int y1){
	//erasing vertices
	int Vx0=(x0-128)/(TriUnit+TriUnit);
	int Vx1=(x1+128)/(TriUnit+TriUnit);
	int Vy0=(y0-256)/(TriUnit);
	int Vy1=(y1+256)/(TriUnit);
	int Nv=0;
	int vxs=0;
	int vys=0;
	int NVER=0;
	int HI0=4096;
	int Nvc=0;
	for(int ix=Vx0;ix<Vx1;ix++){
		for(int iy=Vy0;iy<Vy1;iy++){
			if(ix>0&&ix<VertInLine-1&&iy>0&&iy<MaxTH-1){
				int v=ix+iy*VertInLine;
				int x=GetTriX(v);
				int y=(GetTriY(v)>>1)-GetHi(v);
				if(x>x0&&y>y0&&x<x1&&y<y1){
					vxs+=ix;
					vys+=iy;
					Nvc++;
					if(HI0==4096){
						HI0=THMap[v];
					};
					THMap[v]=HI0;
					SetTexture(v,0);
					MarkPointToDraw(v);
				};
			};
		};
	};	
	//erasing objects
	for(int i=0;i<MaxSprt;i++){
		OneSprite* OS=Sprites+i;
		if(OS->Enabled){
			int x=OS->x;
			int y=(OS->y>>1)-OS->z;
			if(x>x0&&y>y0&&x<x1&&y<y1){
				OS->Enabled=false;
                UnregisterSprite(OS->Index);
                ObjTimer.Del(OS->Index,0);
			};
		};
	};
	//erasing locking
	for(i=0;i<LockBars.NBars;i++){
		int xb=LockBars.BC[i].x;
		int yb=LockBars.BC[i].y;
		int x=(int(xb)<<6)+32;
		int y=(int(yb)<<5)+16;
		if(x>x0&&y>y0&&x<x1&&y<y1){
			BClrBar(xb<<2,yb<<2,4);
			AddDefaultBar(xb,yb);
		};
	};
	for(i=0;i<UnLockBars.NBars;i++){
		int xb=UnLockBars.BC[i].x;
		int yb=UnLockBars.BC[i].y;
		int x=(int(xb)<<6)+32;
		int y=(int(yb)<<5)+16;
		if(x>x0&&y>y0&&x<x1&&y<y1){
			BClrBar(xb<<2,yb<<2,4);
			AddDefaultBar(xb,yb);
		};
	};
	
};
bool CheckInArea(int x0,int y0,int x1,int y1){
	int Vx0=(x0-128)/(TriUnit+TriUnit);
	int Vx1=(x1+128)/(TriUnit+TriUnit);
	int Vy0=(y0-256)/(TriUnit);
	int Vy1=(y1+256)/(TriUnit);
	int HI0=4096;
	for(int ix=Vx0;ix<Vx1;ix++){
		for(int iy=Vy0;iy<Vy1;iy++){
			if(ix>0&&ix<VertInLine-1&&iy>0&&iy<MaxTH-1){
				int v=ix+iy*VertInLine;
				int x=GetTriX(v);
				int y=(GetTriY(v)>>1)-GetHi(v);
				if(x>x0&&y>y0&&x<x1&&y<y1){
					if(TexMap[v])return true;
				};
			};
		};
	};	
	return false;
};
void SaveInSquares(int x0,int y0,int Lx,int Ly,int Nx,int Ny,char* Name){
	char cc[120];
	sprintf(cc,"Pieces\\%s.lst",Name);
	FILE* f=fopen(cc,"w");
	for(int iy=0;iy<Ny;iy++){
		for(int ix=0;ix<Nx;ix++){
			int xx0=x0+ix*Lx;
			int yy0=y0+iy*Ly;
			if(CheckInArea(xx0+4,yy0+4,xx0+Lx-4,yy0+Ly-4)){
				char tri[5];
				int N=ix+iy*Nx;
				if(N<10)sprintf(tri,"00%d",N);
				else if(N<100)sprintf(tri,"0%d",N);
				else sprintf(tri,"%d",N);
				sprintf(cc,"Pieces\\%s%s.smp",Name,tri);
				fprintf(f,"%s\n",cc);
				RM_Save(cc,xx0+4,yy0+4,xx0+Lx-4,yy0+Ly-4);
			};
		};
	};
	fclose(f);
};
char GSaveName[64]="";
extern int GridX;
extern int GridY;
extern int GridLx;
extern int GridLy;
extern int GridNx;
extern int GridNy;
extern int GridMode;
void ProcessSaveInSquares(){
	if(GridMode&&EnterStr(GSaveName,"Save all pieces.")){
		SaveInSquares(GridX*64,GridY*32,GridLx*64,GridLy*32,GridNx,GridNy,GSaveName);
	};
};
//--------------------------TERRAINS--------------------------
struct TerrainData{
	short Lx,Ly;
	byte  NPlaces;
	byte  NIDS;
	word  PLACES[8];
	byte  PlayerID[8];
	int   CompSize;
	byte  Data[32];
};
//SqInt params
int SqMaxX=(480<<(ADDSH-1))*32;
int SqMaxY=(480<<(ADDSH-1))*32;
int SqDX=(480<<(ADDSH-1))*32/256;
int SqDY=(480<<(ADDSH-1))*32/256;
//------------
int SqInt(byte* REND,int x,int y){
	if(x<0)x=0;
	if(y<0)y=0;
	if(x>=SqMaxX)x=SqMaxX-1;
	if(y>=SqMaxY)y=SqMaxY-1;
	int cx=(x<<8)/SqMaxX;
	int cy=(y<<8)/SqMaxY;
	if(cx>=255)cx=254;
	if(cy>=255)cy=254;
	int x0=(cx*SqMaxX)>>8;
	int y0=(cy*SqMaxY)>>8;
	int dx=x-x0;
	int dy=y-y0;
	int pos=cx+(cy<<8);
	int v0=REND[pos  ]<<4;
	int v1=REND[pos+1]<<4;
	int v2=REND[pos+256]<<4;
	int v3=REND[pos+257]<<4;
	return v0+((v1-v0)*dx)/SqDX+((v2-v0)*dy+((v3+v0-v1-v2)*dx*dy)/SqDX)/SqDY;
};
extern int MaxWX;
extern int MaxWY;
extern byte* WaterDeep;
extern byte* WaterBright;
void ErrM(char* s);
void DeleteAllSprites();
void ClearGoodDeepSpot(int x,int y,int r,int dr,int dh);
void CheckWaterOnMountains(){
	return;
	for(int ix=1;ix<msx-2;ix++){
		for(int iy=1;iy<msx-2;iy++){
			int x=ix<<5;
			int y=iy<<5;
			if(WaterDeep[ix+(iy<<(MapShift-1))]>128&&GetHeight(x,y)>=2){
				ClearGoodDeepSpot(x,y>>1,64,24,170);
			};
		};
	};
};
void CreateInfoMap();
void CreateNatDealing(int N,byte* Nats,short* Nx,short* Ny);
void CheckBright(){
	//for(int i=0;i<sizeof WaterBright;i++){
//		assert(WaterBright[i]<=120);
	//};
};
extern int RM_LandType;
extern int RM_Resstart;
extern int RM_Restot;
void GenerateRandomRoad(int idx);
extern int LastAddSpr;
void GenerateWithStyle(char* terr);
extern int PeaceTimeLeft;
extern int MaxPeaceTime;
extern int PeaceTimeStage;
byte BaloonState=0;
byte CannonState=0;
byte XVIIIState=0;
byte CaptState=0;
byte SaveState=6;
void AutoSetFactures();
void FreeTZBuf();

//--------------------------------REPORT ABOUT GENERATING------------------
extern int CurPalette;
int LastPalette=-1;
extern LocalGP ReportPanel;

char InfMessage[128];
char CurMessage[128];
int LastCallTime;
int InitInfTime;
int LastStageTime;
//SQPicture GPANEL;
void SetupInfMessage(char* Header){
	//if(!GPANEL.PicPtr)GPANEL.LoadPicture("y288x128.bpx");
	strcpy(InfMessage,Header);
	InitInfTime=GetTickCount();
};

void SetNextInfStage(char* Message){
	strcpy(CurMessage,Message);
	LastStageTime=GetTickCount();
};
extern int RealLx;
extern int RealLy;
bool NotInGame=false;
void ShowProcessInfo(){
	return;
	if(LastPalette!=CurPalette){
		ReportPanel.UnLoad();
		if(CurPalette){
			ReportPanel.Load("Interface\\Loading1");
		}else{
			ReportPanel.Load("Interface\\Palettes\\Loading0");
		};
		LastPalette=CurPalette;
	};
	int tm=GetTickCount();
	if(tm-LastCallTime<200)return;
	LastCallTime=tm;
	ProcessMessages();
	double t1=double(tm-InitInfTime)/1000;
	double t2=double(tm-LastStageTime)/1000;
	int x0=RealLx>>1;
	if(CurPalette)x0-=82;
	int y0=RealLy>>1;

	void* oldsof=ScreenPtr;
	ScreenPtr=RealScreenPtr;
	int ScLx=ScrWidth;
	ScrWidth=RSCRSizeX;
	int LX=GPS.GetGPWidth(ReportPanel.GPID,0);
	int LY=GPS.GetGPHeight(ReportPanel.GPID,0);
	GPS.ShowGP(x0-(LX>>1),y0-(LY>>1),ReportPanel.GPID,0,0);
	int L1=GetRLCStrWidth(InfMessage,&BigRedFont)>>1;
	ShowString(x0-L1,y0-64+20,InfMessage,&BigRedFont);
	L1=GetRLCStrWidth(CurMessage,&BlackFont)>>1;
	ShowString(x0-L1,y0-64+56,CurMessage,&BlackFont);
	char ccc[128];
	sprintf(ccc,"Stage:%.1fs Total:%.1fs",t2,t1);
	L1=GetRLCStrWidth(ccc,&BlackFont)>>1;
	ShowString(x0-L1,y0-64+48+35,ccc,&BlackFont);

	ScrWidth=ScLx;
	ScreenPtr=oldsof; 
};
void ShowProgressBar(char* Message,int N,int NMax){
	if(LastPalette!=CurPalette){
		ReportPanel.UnLoad();
		if(CurPalette){
			//ReportPanel.Load("Interface\\Loading1");
		}else{
			ReportPanel.Load("Interface\\Palettes\\Loading0");
		};
		LastPalette=CurPalette;
	};
	int x0=RealLx>>1;
	int y0=RealLy>>1;
	int LX=GPS.GetGPWidth(ReportPanel.GPID,0);
	int LY=GPS.GetGPHeight(ReportPanel.GPID,0);
	if(!CurPalette){
		LX=347;
		LY=165;
		DrawStdBar(x0-(LX>>1)+16,y0-(LY>>1)+16,x0+(LX>>1)-16,y0+(LY>>1)-16);
	}else GPS.ShowGP(x0-(LX>>1),y0-(LY>>1),ReportPanel.GPID,0,0);
	int L1=GetRLCStrWidth(Message,&BigRedFont)>>1;
	ShowString(x0-L1,y0-64+20,Message,&BigRedFont);
	char cc[32];
	sprintf(cc,"%d%%",N*100/NMax);
	L1=GetRLCStrWidth(cc,&BigWhiteFont)>>1;
	ShowString(x0-L1,y0-64+48+10,cc,&BigYellowFont);
};
void ShowTable(char* Message,char* Hint){
	if(LastPalette!=CurPalette){
		ReportPanel.UnLoad();
		if(CurPalette){
			ReportPanel.Load("Interface\\Loading1");
		}else{
			ReportPanel.Load("Interface\\Palettes\\Loading0");
		};
		LastPalette=CurPalette;
	};
	int x0=RealLx>>1;
	int y0=RealLy>>1;
	int LX=GPS.GetGPWidth(ReportPanel.GPID,0);
	int LY=GPS.GetGPHeight(ReportPanel.GPID,0);
	if(!CurPalette){
		LX=347;
		LY=165;
		DrawStdBar(x0-(LX>>1)+16,y0-(LY>>1)+16,x0+(LX>>1)-16,y0+(LY>>1)-16);
	}else GPS.ShowGP(x0-(LX>>1),y0-(LY>>1),ReportPanel.GPID,0,0);
	int L1=GetRLCStrWidth(Message,&BigRedFont)>>1;
	ShowString(x0-L1,y0-64+25,Message,&BigRedFont);
	L1=GetRLCStrWidth(Hint,&BlackFont)>>1;
	ShowString(x0-L1,y0-64+48+30,Hint,&BlackFont);
};
//------------------------------NATIONAL DIVISION OF THE TERRITORY------------------//

int NatX[8];
int NatY[8];
byte NatPres[8];
int NdMaxX=120;
void InitNatDeal(){
	NdMaxX=msx>>2;
	NatDeals=znew(byte,NATLX*NATLX);
	memset(NatDeals,0xFF,NATLX*NATLX);
	for(int i=1;i<NdMaxX;i++)memset(NatDeals+(i<<NATSH)+1,0xEE,NdMaxX-1);
	memset(NatPres,0,sizeof NatPres);
	
	for(int x=1;x<NdMaxX;x++)
		for(int y=1;y<NdMaxX;y++){
			int ofs=(x<<2)+1+(((y<<2)+1)*MaxWX);
			if(WaterDeep[ofs]>128)NatDeals[x+(y<<NATSH)]=0xFF;
		};
	
};
void SetNatDealPoint(int x,int y,byte Nat){
	if(x>0&&y>0&&x<NdMaxX&&y<NdMaxX){
		NatX[Nat]=x;
		NatY[Nat]=y;
		NatPres[Nat]=1;
		NatDeals[x+(y<<NATSH)]=Nat<<4;
	};
	
};
int NFreeIsl=0;
short FreeIsX[768];
short FreeIsY[768];
void ExtendNatDealing(){

	int MAX=(60<<ADDSH);
	bool change;
	int SH0=6+ADDSH;
	byte* TMP=znew(byte,NATLX*NATLX);
	memcpy(TMP,NatDeals,NATLX*NATLX);
	int CenterX[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int CenterY[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	byte c;
	for(int iy=2;iy<MAX;iy++){
		for(int ix=2;ix<MAX;ix++){
			int ofs=ix+(iy<<SH0);
			c=TMP[ofs];
			if(c<128){
				c>>=4;
				CenterX[c]=ix;
				CenterY[c]=iy;
			};
		};
	};

	do{
		memcpy(TMP,NatDeals,NATLX*NATLX);
		change=0;
		for(int iy=0;iy<MAX;iy++){
			for(int ix=0;ix<MAX;ix++){
				int ofs=ix+(iy<<SH0);
				byte cxx=TMP[ofs];
				byte c;
				if(cxx==0xEE){
					if((c=TMP[ofs+1])<0xEE){
						c>>=4;
						int q=Norma(ix-CenterX[c],iy-CenterY[c])>>2;
						if(q>15)q=15;
						NatDeals[ofs]=q+(c<<4);
						change=1;
					}else
					if(ix>0&&(c=TMP[ofs-1])<0xEE){
						c>>=4;
						int q=Norma(ix-CenterX[c],iy-CenterY[c])>>2;
						if(q>15)q=15;
						NatDeals[ofs]=q+(c<<4);
						change=1;
					}else
					if((c=TMP[ofs+NATLX])<0xEE){
						c>>=4;
						int q=Norma(ix-CenterX[c],iy-CenterY[c])>>2;
						if(q>15)q=15;
						NatDeals[ofs]=q+(c<<4);
						change=1;
					}else
					if(iy>0&&(c=TMP[ofs-NATLX])<0xEE){
						c>>=4;
						int q=Norma(ix-CenterX[c],iy-CenterY[c])>>2;
						if(q>15)q=15;
						NatDeals[ofs]=q+(c<<4);
						change=1;
					};
				};
			};
		};
		ProcessMessages();
	}while(change);
	free(TMP);
/*


	for(int i=1;i<RRad;i++){
		byte q;
		q=i>>2;
		if(q>15)q=15;
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		int N=Rarr[i].N;
		for(int n=0;n<8;n++){
			byte p=n<<4;
			byte v=p+q;
			int x0=NatX[n];
			int y0=NatY[n];
			for(int j=0;j<N;j++){
				int xc=x0+xi[j];
				int yc=y0+yi[j];
				if(xc>0&&yc>0&&xc<NdMaxX&&yc<NdMaxX){
					int ofs=xc+(yc<<NATSH);
					if(NatDeals[ofs]==0xEE){
						byte vu=NatDeals[ofs-NATLX]&240;
						byte vd=NatDeals[ofs+NATLX]&240;
						byte vl=NatDeals[ofs-1]&240;
						byte vr=NatDeals[ofs+1]&240;

						byte vlu=NatDeals[ofs-NATLX-1]&240;
						byte vru=NatDeals[ofs-NATLX+1]&240;
						byte vld=NatDeals[ofs+NATLX-1]&240;
						byte vrd=NatDeals[ofs+NATLX+1]&240;

						if(vl==p||vd==p||vr==p||vu==p||
						   vlu==p||vru==p||vld==p||vrd==p)NatDeals[ofs]=v;
					};
				};
			};
		};
	};
	bool Change=false;
	do{
		Change=false;
		for(int x=1;x<NATLX-1;x++){
			for(int y=1;y<NATLX-1;y++){
				int ofs=x+(y<<NATSH);
				if(NatDeals[ofs]==0xEE){
					byte vu=NatDeals[ofs-NATLX];
					byte vd=NatDeals[ofs+NATLX];
					byte vl=NatDeals[ofs-1];
					byte vr=NatDeals[ofs+1];
					if(vu<0xEE){
						NatDeals[ofs]=vu;
						Change=1;
					};
					if(vd<0xEE){
						NatDeals[ofs]=vd;
						Change=1;
					};
					if(vl<0xEE){
						NatDeals[ofs]=vl;
						Change=1;
					};
					if(vr<0xEE){
						NatDeals[ofs]=vr;
						Change=1;
					};
				};
			};
		};
	}while(Change);
	*/
	NFreeIsl=0;
	for(int x=1;x<NATLX-1;x+=2){
		for(int y=1;y<NATLX-1;y+=2){
			int ofs=x+(y<<NATSH);
			if(NatDeals[ofs]==0xEE&&NFreeIsl<768){
				FreeIsX[NFreeIsl]=x;
				FreeIsY[NFreeIsl]=y;
				NFreeIsl++;
			};
		};
	};
};


class PieceList{
public:
	char** Names;
	int NNames;
	int MaxNames;
	PieceList();
	void Load(char* Name);
	PieceList(char* Name);
	~PieceList();
	void Clear();
};
PieceList::PieceList(){
	Names=NULL;
	NNames=0;
	MaxNames=0;
};
PieceList::~PieceList(){
	Clear();
};
void PieceList::Clear(){
	for(int i=0;i<NNames;i++)free(Names[i]);
	if(Names)free(Names);
	Names=NULL;
	NNames=0;
	MaxNames=0;
};
void PieceList::Load(char* Name){
	Clear();
	GFILE* F1=Gopen(Name,"r");
	if(F1){
	int q;
		char cc1[128];
		do{
			q=Gscanf(F1,"%s",cc1);
			if(q==1){
				if(NNames>=MaxNames){
					MaxNames+=512;
					Names=(char**)realloc(Names,MaxNames<<2);
				};
				Names[NNames]=znew(char,strlen(cc1)+1);
				strcpy(Names[NNames],cc1);
				NNames++;
			};
		}while(q==1);
		Gclose(F1);
	};
};
PieceList::PieceList(char* Name){
	Names=NULL;
	NNames=0;
	MaxNames=0;
	Load(Name);
};
void CreateNatDealing(int N,byte* Nats,short* Nx,short* Ny){
	InitNatDeal();
	for(int i=0;i<N;i++){
		SetNatDealPoint(Nx[i],Ny[i],Nats[i]);
	};
	ExtendNatDealing();
};

void CreateCenterRound(int x,int y,int R){
	int DR=(R>>5)+3;
	int vx0=(x>>5)-DR;
	int vy0=(y>>5)-DR;
	int vx1=(x>>5)+DR;
	int vy1=(y>>5)+DR;
	if(vx0<0)vx0=0;
	if(vx0>=VertInLine)vx0=VertInLine-1;
	if(vx1<0)vx1=0;
	if(vx1>=VertInLine)vx1=VertInLine-1;
	if(vy0<0)vy0=0;
	if(vy0>MaxTH-1)vy0=MaxTH-1;
	if(vy1<0)vy1=0;
	if(vy1>MaxTH-1)vy1=MaxTH-1;
	for(int ix=vx0;ix<=vx1;ix++)
		for(int iy=vy0;iy<=vy1;iy++){
			int v=ix+iy*VertInLine;
			if(!TexMap[v]){
				int xc=GetTriX(v);
				int yc=GetTriY(v);
				if(Norma(x-xc,y-yc)<R)TexMap[v]=255;
			};
		};
};
void ClearCenterRound(int x,int y,int R){
	int DR=(R>>5)+3;
	int vx0=(x>>5)-DR;
	int vy0=(y>>5)-DR;
	int vx1=(x>>5)+DR;
	int vy1=(y>>5)+DR;
	if(vx0<0)vx0=0;
	if(vx0>=VertInLine)vx0=VertInLine-1;
	if(vx1<0)vx1=0;
	if(vx1>=VertInLine)vx1=VertInLine-1;
	if(vy0<0)vy0=0;
	if(vy0>MaxTH-1)vy0=MaxTH-1;
	if(vy1<0)vy1=0;
	if(vy1>MaxTH-1)vy1=MaxTH-1;
	for(int ix=vx0;ix<=vx1;ix++)
		for(int iy=vy0;iy<=vy1;iy++){
			int v=ix+iy*VertInLine;
			if(TexMap[v]==255)TexMap[v]=0;
		};
};
int NCRound;
int CRoundX[8];
int CRoundY[8];
bool GenerateStartUnits(char* NationID,byte NI,int x,int y,int GenIndex);
void PictureCordons();
byte CordonIDX[8];
int PeaceTimeLeft=300;
int MaxPeaceTime=0;
int PeaceTimeStage=0;
int GetUnitActivity(OneObject* OB){
	if(!(PeaceTimeLeft&&NatDeals))return 0;
	if(OB){
		int xm=OB->RealX>>11;
		int ym=OB->RealY>>11;
		if(xm<=0||ym<=0||xm>=NATLX||ym>=NATLX)return 0;
		byte Deal=NatDeals[xm+(ym<<(TopSH-1))];
		if(Deal>=128)return 0;
		Deal>>=4;
		if(CordonIDX[Deal]==255)return 0;
		if(OB->NMask&NATIONS[CordonIDX[Deal]].NMask)return 1;
	};
	return -1;
};
bool CheckBuildPossibility(byte NI,int x,int y){
	if(!(PeaceTimeLeft&&NatDeals))return 1;
	int xm=x>>7;
	int ym=y>>7;
	if(xm<=0||ym<=0||xm>=NATLX||ym>=NATLX)return 0;
	byte Deal=NatDeals[xm+(ym<<(TopSH-1))];
	if(Deal>=128)return 1;
	Deal>>=4;
	if(CordonIDX[Deal]==255)return 1;
	return NATIONS[CordonIDX[Deal]].NMask&NATIONS[NI].NMask;
};
#define NMINETYPES 4
void ClearAllRounds(){
	for(int i=0;i<NCRound;i++){
		ClearCenterRound(CRoundX[i],CRoundY[i],750);
	};
	NCRound=0;
};
#define C_NATLX 128

void SetBrightSpot(int x,int y,int Brightness,bool dir);
void DrawCordonPoint(int x,int y){
	if(WaterDeep[(x>>5)+((y>>5)<<(TopSH+1))]>127){
		SetBrightSpot(x,y>>1,6,1);
	}else{
		addSpriteAnyway(x+(mrand()%7)-3,y+(mrand()%7)-3,&HOLES,mrand()%6);
	};
};
void PictureCordons(){
	if(!PeaceTimeLeft)return;
	//1. extending on water
	int MAX=(60<<ADDSH)-2;
	bool change;
	int SH0=6+ADDSH;
	byte* TMP=znew(byte,NATLX*NATLX);
	memcpy(TMP,NatDeals,NATLX*NATLX);
	do{
		memcpy(TMP,NatDeals,NATLX*NATLX);
		change=0;
		for(int iy=2;iy<MAX;iy++){
			for(int ix=2;ix<MAX;ix++){
				int ofs=ix+(iy<<SH0);
				byte c=TMP[ofs];
				if(c==0xFF){
					if(TMP[ofs+1]!=0xFF){
						NatDeals[ofs]=TMP[ofs+1];
						change=1;
					}else
					if(TMP[ofs-1]!=0xFF){
						NatDeals[ofs]=TMP[ofs-1];
						change=1;
					}else
					if(TMP[ofs+NATLX]!=0xFF){
						NatDeals[ofs]=TMP[ofs+NATLX];
						change=1;
					}else
					if(TMP[ofs-NATLX]!=0xFF){
						NatDeals[ofs]=TMP[ofs-NATLX];
						change=1;
					};
				};
			};
		};
		ProcessMessages();
	}while(change);
	free(TMP);
	//
	int ofs=0;
	int MaxX=NATLX-2;
	int Stolb=47;
	for(int y=0;y<NATLX;y++){
		for(int x=0;x<NATLX;x++){
			if(x>0&&x<MaxX&&y>0&&y<MaxX){
				int x0=(x<<7)+64;
				int y0=(y<<7)+64;
				byte c=NatDeals[ofs];
				if(c!=0xFF){	
					c>>=4;
					int cr=NatDeals[ofs+1];
					if(cr!=0xFF){
						cr>>=4;
						if(cr!=c){
							for(int k=-1;k<3;k++)
								DrawCordonPoint(x0+64,y0-16+32*k);
						};
					};
					/*
					cr=NatDeals[ofs-1];
					if(cr!=0xFF){
						cr>>=4;
						if(cr!=c){
							for(int k=-1;k<3;k++)
								addSpriteAnyway(x0-64,y0-16+32*k,&TREES,Stolb);
						};
					};
					cr=NatDeals[ofs-NATLX];
					if(cr!=0xFF){
						cr>>=4;
						if(cr!=c){
							for(int k=-1;k<3;k++)
								addSpriteAnyway(x0-16+32*k,y0-64,&TREES,Stolb);
						};
					};
					*/
					cr=NatDeals[ofs+NATLX];
					if(cr!=0xFF){
						cr>>=4;
						if(cr!=c){
							for(int k=-1;k<3;k++)
								DrawCordonPoint(x0-16+32*k,y0+64);
						};
					};
					
				};
			};
			ofs++;
		};
	};
};
//--------------------------------DIALOG----------------------------

RandomMapDesc::RandomMapDesc(){
	NMINES=0;
	MINES=NULL;
	NRES=0;
	RES=NULL;
	NRelief=0;
	Relief=NULL;
	NSTY=0;
	STY=NULL;
};
void RandomMapDesc::Close(){
	if(NMINES)delete(MINES);
	MINES=NULL;
	NMINES=0;
	if(NRES)delete(RES);
	RES=NULL;
	NRES=0;
	if(NRelief){
		for(int p=0;p<NRelief;p++){
			free(Relief[p]);
		};
		free(Relief);
	};
	Relief=NULL;
	NRelief=0;
	if(NSTY){
		for(int p=0;p<NSTY;p++){
			int N2=STY[p].NPl;
			for(int q=0;q<N2;q++){
				free(STY[p].Players[q].name);
			};
			delete(STY[p].Players);
		};
		delete(STY);
	};
	STY=NULL;
	NSTY=0;
};
RandomMapDesc::~RandomMapDesc(){
	Close();
};
void ERRMP(char* name,char* Sect){
	char cc[128];
	sprintf(cc,"File %s is corrupted in section %s",name,Sect);
	ErrM(cc);
};
void normstr(char* str);
char* GetTextByID(char* ID);
void RandomMapDesc::Load(char* name){
	Close();
	GFILE* F=Gopen(name,"r");
	if(F){
		char cc[128];
		int z=Gscanf(F,"%d%d",&NMINES,&MINES_DEF);
		if(z!=2)ERRMP(name,"MINES");
		MINES=new MineRec[NMINES];
		for(int p=0;p<NMINES;p++){	
			int n1,n2,n3;
			z=Gscanf(F,"%s%d%d%d",cc,&n1,&n2,&n3);
			if(z!=4)ERRMP(name,"MINES");
			char* ust=GetTextByID(cc);
			MINES[p].Name=znew(char,strlen(ust)+1);
			strcpy(MINES[p].Name,ust);
			MINES[p].Ng=n1;
			MINES[p].Ni=n2;
			MINES[p].Nc=n3;
		};
		z=Gscanf(F,"%d%d",&NRES,&RES_DEF);
		if(z!=2)ERRMP(name,"STARTRES");
		RES=new ResRec[NRES];
		for(p=0;p<NRES;p++){
			int Nr;
			z=Gscanf(F,"%s%d",cc,&Nr);
			char* ust=GetTextByID(cc);
			if(z!=2)ERRMP(name,"STARTRES");
			memset(RES[p].RES,0,sizeof RES[p].RES);
			RES[p].Name=znew(char,strlen(ust)+1);
			strcpy(RES[p].Name,ust);
			for(int q=0;q<Nr;q++){
				int n1;
				z=Gscanf(F,"%d",&n1);
				if(z!=1)ERRMP(name,"STARTRES");
				RES[p].RES[q]=n1;
			};
		};
		z=Gscanf(F,"%d%d",&NRelief,&Relief_DEF);
		if(z!=2)ERRMP(name,"RELIEF");
		Relief=(char**)malloc(NRelief*4);
		for(p=0;p<NRelief;p++){
			z=Gscanf(F,"%s",cc);
			char* ust=GetTextByID(cc);
			//normstr(cc);
			if(z!=1)ERRMP(name,"RELIEF");
			Relief[p]=znew(char,strlen(ust)+1);
			strcpy(Relief[p],ust);
		};
		z=Gscanf(F,"%d%d",&NSTY,&STY_DEF);
		if(z!=2)ERRMP(name,"STYLES");
		STY=new StyleRec[NSTY];
		for(p=0;p<NSTY;p++){
			StyleRec* SR=STY+p;
			Gscanf(F,"%s",cc);
			//normstr(cc);
			int aitp=p;
			if(cc[0]=='#'){
				aitp=cc[1]-'0';
				Gscanf(F,"%s",cc);
			};
			char* ust=GetTextByID(cc);
			SR->Name=znew(char,strlen(ust)+1);
			strcpy(SR->Name,ust);
			Gscanf(F,"%s",cc);
			SR->Style=znew(char,strlen(cc)+1);
			strcpy(SR->Style,cc);
			z=Gscanf(F,"%d",&SR->NPl);
			SR->Players=new PlRec[SR->NPl];
			SR->AI_Style=aitp;
			for(int q=0;q<SR->NPl;q++){
				int n1;
				Gscanf(F,"%d%s",&n1,cc);
				SR->Players[q].name=znew(char,strlen(cc)+1);
				strcpy(SR->Players[q].name,cc);
				SR->Players[q].NPlayers=n1;
			};
		};
		Gclose(F);
	};
};
//-------------------------Generating map by the bitmap for missions-------------------//

void DelWhitePt(byte* Data,int x,int y){
	if(x>=0&&y>=0&&x<256&&y<256){
		int pos=(x+(y<<8))*3;
		int R=Data[pos];
		int G=Data[pos+1];
		int B=Data[pos+2];
		if(R==G&&R==B&&R>10){
			Data[pos]=0;
			Data[pos+1]=0;
			Data[pos+2]=0;
		};
	};
};
void ReadHiMap(char* Name,byte** Data,byte Default){
	BMPformat BM;
	byte* Data24;
	*Data=znew(byte,65536);
	memset(*Data,0,65536);
	if(ReadBMP24(Name,&BM,&Data24)){
		int pos=0;
		for(int i=0;i<65536;i++){
			(*Data)[i]=Data24[pos+2];
			pos+=3;
		};
		delete(Data24);
	};
};
void DelWhiteSpot(byte* Data,int x,int y){
	DelWhitePt(Data,x,y);

	DelWhitePt(Data,x-1,y);
	DelWhitePt(Data,x+1,y);
	DelWhitePt(Data,x,y-1);
	DelWhitePt(Data,x,y+1);

	DelWhitePt(Data,x-1,y-1);
	DelWhitePt(Data,x-1,y+1);
	DelWhitePt(Data,x+1,y-1);
	DelWhitePt(Data,x+1,y+1);

	DelWhitePt(Data,x-2,y);
	DelWhitePt(Data,x+2,y);
	DelWhitePt(Data,x,y-2);
	DelWhitePt(Data,x,y+2);

	DelWhitePt(Data,x-2,y-1);
	DelWhitePt(Data,x-2,y+1);
	DelWhitePt(Data,x+2,y-1);
	DelWhitePt(Data,x+2,y+1);
	DelWhitePt(Data,x-1,y-2);
	DelWhitePt(Data,x+1,y-2);
	DelWhitePt(Data,x-1,y+2);
	DelWhitePt(Data,x+1,y+2);
};
void DelBorder(byte* Data){
	int pos=0;
	byte* Data1=znew(byte,256*256*3);
	memcpy(Data1,Data,256*256*3);
	for(int i=0;i<65536;i++){
		byte B=Data1[pos];
		byte G=Data1[pos+1];
		byte R=Data1[pos+2];
		if(!(R||G||B))DelWhiteSpot(Data,i&255,i>>8);
		pos+=3;
	};
	delete(Data1);
};
void SoftZones(char* Name,short* VHI){
	byte* Data;
	ReadHiMap(Name,&Data,0);
	int* Vertices=NULL;
	int NVert=0;
	int MaxVert=0;
	for(int v=0;v<MaxPointIndex;v++){
		int x=GetTriX(v);
		int y=GetTriY(v);
		if(x>64&&y>64){
			int H=SqInt(Data,x,y);
			if(H>2000){
				if(NVert>=MaxVert){
					MaxVert+=4096;
					Vertices=(int*)realloc(Vertices,MaxVert*4);
				};
				Vertices[NVert]=v;
				NVert++;
			};
		};
	};
	delete(Data);
	//softing
	if(NVert){
		short* VertHi=znew(short,NVert);
		byte*  VHIT=znew(byte,NVert);
		for(int j=0;j<50;j++){
			for(int i=0;i<NVert;i++){
				int vert=Vertices[i];
				int Lin=vert/VertInLine;
				int Lpos=vert%VertInLine;
				if(Lin>0&&Lpos>0&&Lpos<VertInLine-1&&Lin<MaxTH-1){
					if(Lpos&1){
						VertHi[i]=(THMap[vert+VertInLine]+THMap[vert-VertInLine]+THMap[vert-1]+
							THMap[vert+1]+THMap[vert-VertInLine+1]+THMap[vert-VertInLine-1])/6;
						VHIT[i]=(VHI[vert+VertInLine]+VHI[vert-VertInLine]+VHI[vert-1]+
							VHI[vert+1]+VHI[vert-VertInLine+1]+VHI[vert-VertInLine-1])/6;
					}else{
						VertHi[i]=(THMap[vert+VertInLine]+THMap[vert-VertInLine]+THMap[vert-1]+
							THMap[vert+1]+THMap[vert+VertInLine+1]+THMap[vert+VertInLine-1])/6;
						VHIT[i]=(VHI[vert+VertInLine]+VHI[vert-VertInLine]+VHI[vert-1]+
							VHI[vert+1]+VHI[vert+VertInLine+1]+VHI[vert+VertInLine-1])/6;
					};
				};
			};
			for(i=0;i<NVert;i++){
				int vi=Vertices[i];
				THMap[vi]=VertHi[i];
				VHI[vi]=VHIT[i];
			};
			ShowProcessInfo();
		};
		if(Vertices)delete(Vertices);
	};
};
void SoftAllMap(){
	for(int vert=0;vert<MaxPointIndex;vert++){
		int Lin=vert/VertInLine;
		int Lpos=vert%VertInLine;
		if(Lin>0&&Lpos>0&&Lpos<VertInLine-1&&Lin<MaxTH-1){
			int VHU=THMap[vert-VertInLine];
			int VH=THMap[vert];
			if(VHU<VH){
				if(Lpos&1){
					THMap[vert]=(THMap[vert+VertInLine]+THMap[vert-VertInLine]+THMap[vert-1]+
						THMap[vert+1]+THMap[vert-VertInLine+1]+THMap[vert-VertInLine-1])/6;
				}else{
					THMap[vert]=(THMap[vert+VertInLine]+THMap[vert-VertInLine]+THMap[vert-1]+
						THMap[vert+1]+THMap[vert+VertInLine+1]+THMap[vert+VertInLine-1])/6;
				};
			};
		};
	};
};
void SetFractalTexture();
void ClearAllZones();
int HARRAY[65]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
33,35,37,39,41,43,45,47,49,51,53,55,56,58,59,60,61,62,63,64,64,64,64,64,64,64,64,64,64,64,64,64,64};
void ClearAllLModeData();
void CreateTotalLocking();
void ResearchIslands();
void FreeTZBuf();
//----------------------------SAMPLES ENGINE-------------------//

SamplesSet::SamplesSet(){
	NSmp=0;
	SSET=NULL;
	Passive=0;
	CurrentSet[0]=0;
};
void SamplesSet::ClearAll(){
	for(int i=0;i<NSmp;i++){
		if(SSET[i].xi)free(SSET[i].xi);
		if(SSET[i].yi)free(SSET[i].yi);
		if(SSET[i].ROOT)free(SSET[i].ROOT);
	};
	NSmp=0;
	SSET=NULL;
};
void SamplesSet::AddSquare(int x,int y){
	if(CurrentSet[0]){
		for(int i=0;i<NSmp;i++)if(!_stricmp(SSET[i].Name,CurrentSet)){
			int n=SSET[i].Npt;
			for(int j=0;j<n;j++)if(SSET[i].xi[j]==x&&SSET[i].yi[j]==y)return;
			SSET[i].Npt++;
			SSET[i].xi=(short*)realloc(SSET[i].xi,SSET[i].Npt<<1);
			SSET[i].yi=(short*)realloc(SSET[i].yi,SSET[i].Npt<<1);
			SSET[i].xi[SSET[i].Npt-1]=x;
			SSET[i].yi[SSET[i].Npt-1]=y;
			return;
		};
	};
};
void SamplesSet::DelSquare(int x,int y,int mode){
	if(CurrentSet[0]){
		for(int i=0;i<NSmp;i++)if(!_stricmp(SSET[i].Name,CurrentSet)){
			int n;
			if(mode&1){
				n=SSET[i].Npt;
				for(int j=0;j<n;j++)if(SSET[i].xi[j]==x&&SSET[i].yi[j]==y){
					if(j<n-1){
						memcpy(SSET[i].xi+j,SSET[i].xi+j+1,(n-j-1)<<1);
						memcpy(SSET[i].yi+j,SSET[i].yi+j+1,(n-j-1)<<1);
					};
					SSET[i].Npt--;
				};
			};
			if(mode&2){
				n=SSET[i].NRoots;
				for(int j=0;j<n;j++)if(SSET[i].ROOT[j].x==x&&SSET[i].ROOT[j].y==y){
					if(j<n-1){
						memcpy(SSET[i].ROOT+j,SSET[i].ROOT+j+1,(n-j-1)*sizeof SampleRoot);
					};
					SSET[i].NRoots--;
				};
			};
			return;
		};
	};
};
#define WNDLX 450
#define WNDLY 300
//CEXPORT int ItemChoose;
void StdKeys();
int prevRoot=0;
void SamplesSet::EditRootParam(int x,int y){
	DialogsSystem DSS((RealLx-WNDLX)>>1,(RealLy-WNDLY)>>1);
	int x0=150;
	int y0=150;
	int DD=40;
	
	TextButton* U=DSS.addTextButton(NULL,x0,y0-DD,"U",&WhiteFont,&YellowFont,&YellowFont,1);
	TextButton* D=DSS.addTextButton(NULL,x0,y0+DD,"D",&WhiteFont,&YellowFont,&YellowFont,1);
	TextButton* L=DSS.addTextButton(NULL,x0-DD,y0,"L",&WhiteFont,&YellowFont,&YellowFont,1);
	TextButton* R=DSS.addTextButton(NULL,x0+DD,y0,"R",&WhiteFont,&YellowFont,&YellowFont,1);
	TextButton* LU=DSS.addTextButton(NULL,x0-DD,y0-DD,"LU",&WhiteFont,&YellowFont,&YellowFont,1);
	TextButton* RU=DSS.addTextButton(NULL,x0+DD,y0-DD,"RU",&WhiteFont,&YellowFont,&YellowFont,1);
	TextButton* LD=DSS.addTextButton(NULL,x0-DD,y0+DD,"LD",&WhiteFont,&YellowFont,&YellowFont,1);
	TextButton* RD=DSS.addTextButton(NULL,x0+DD,y0+DD,"RD",&WhiteFont,&YellowFont,&YellowFont,1);
	TextButton* DIRSET[8]={U,RU,R,RD,D,LD,L,LU};

	x0=300;
	TextButton* mU=DSS.addTextButton(NULL,x0,y0-DD,"U",&WhiteFont,&YellowFont,&YellowFont,1);
	TextButton* mD=DSS.addTextButton(NULL,x0,y0+DD,"D",&WhiteFont,&YellowFont,&YellowFont,1);
	TextButton* mL=DSS.addTextButton(NULL,x0-DD,y0,"L",&WhiteFont,&YellowFont,&YellowFont,1);
	TextButton* mR=DSS.addTextButton(NULL,x0+DD,y0,"R",&WhiteFont,&YellowFont,&YellowFont,1);
	TextButton* mLU=DSS.addTextButton(NULL,x0-DD,y0-DD,"LU",&WhiteFont,&YellowFont,&YellowFont,1);
	TextButton* mRU=DSS.addTextButton(NULL,x0+DD,y0-DD,"RU",&WhiteFont,&YellowFont,&YellowFont,1);
	TextButton* mLD=DSS.addTextButton(NULL,x0-DD,y0+DD,"LD",&WhiteFont,&YellowFont,&YellowFont,1);
	TextButton* mRD=DSS.addTextButton(NULL,x0+DD,y0+DD,"RD",&WhiteFont,&YellowFont,&YellowFont,1);
	TextButton* mSET[8]={mU,mRU,mR,mRD,mD,mLD,mL,mLU};

	TextButton* HDR=DSS.addTextButton(NULL,WNDLX/2,20,CurrentSet,&YellowFont,&YellowFont,&YellowFont,1);
	TextButton* Type=DSS.addTextButton(NULL,WNDLX/2,WNDLY-35,"Type:",&YellowFont,&YellowFont,&YellowFont,2);
	char CRT[32]="0";
	InputBox* IBT=DSS.addInputBox(NULL,WNDLX/2,WNDLY-35,CRT,8,80,20,&YellowFont,&WhiteFont);
	ItemChoose=-1;
	DWORD MASK=0;
	byte CurDir=0;
	byte CurType=prevRoot;
	//search for existing root
	for(int i=0;i<NSmp;i++)if(!_stricmp(SSET[i].Name,CurrentSet)){
		int n=SSET[i].NRoots;
		for(int j=0;j<n;j++)if(SSET[i].ROOT[j].x==x&&SSET[i].ROOT[j].y==y){
			MASK=SSET[i].ROOT[j].AttachMask;
			CurDir=SSET[i].ROOT[j].Direction;
			CurType=SSET[i].ROOT[j].RootType;
		};
	};
	sprintf(CRT,"%d",CurType);
	do{
		DrawStdBar((RealLx-WNDLX)>>1,(RealLy-WNDLY)>>1,(RealLx+WNDLX)>>1,(RealLy+WNDLY)>>1);
		for(int i=0;i<8;i++){
			DIRSET[i]->PassiveFont=CurDir==i?&RedFont:&YellowFont;
			mSET[i]->PassiveFont=MASK&(1<<i)?&RedFont:&YellowFont;
			if(mSET[i]->MouseOver&&Lpressed){
				if(MASK&(1<<i)){
					MASK&=~(1<<i);
				}else MASK|=1<<i;
				Lpressed=0;
			};
			if(DIRSET[i]->MouseOver&&Lpressed){
				CurDir=i;
				Lpressed=0;
			};
		};
		ProcessMessages();
		StdKeys();
		DSS.ProcessDialogs();
		DSS.RefreshView();
		CurType=atoi(CRT);
	}while(ItemChoose==-1);
	if(ItemChoose==mcmOk){
		for(int i=0;i<NSmp;i++)if(!_stricmp(SSET[i].Name,CurrentSet)){
			int n=SSET[i].NRoots;
			for(int j=0;j<n;j++)if(SSET[i].ROOT[j].x==x&&SSET[i].ROOT[j].y==y){
				SSET[i].ROOT[j].AttachMask=MASK;
				SSET[i].ROOT[j].Direction=CurDir;
				SSET[i].ROOT[j].RootType=CurType;
				return;
			};
			//root not found!
			SSET[i].ROOT=(SampleRoot*)realloc(SSET[i].ROOT,(SSET[i].NRoots+1)*sizeof SampleRoot);
			int cur=SSET[i].NRoots;
			SSET[i].NRoots++;
			SSET[i].ROOT[cur].AttachMask=MASK;
			SSET[i].ROOT[cur].Direction=CurDir;
			SSET[i].ROOT[cur].Reserved=0;
			SSET[i].ROOT[cur].RootType=CurType;
			prevRoot=CurType;
			SSET[i].ROOT[cur].x=x;
			SSET[i].ROOT[cur].y=y;
			return;
		};
	};
};
int DDDXX[8]={0,1,1,1,0,-1,-1,-1};
int DDDYY[8]={-1,-1,0,1,1,1,0,-1};
void CBar(int x,int y,int Lx,int Ly,byte c);
void xLine(int x,int y,int x1,int y1,byte c);
void SamplesSet::Draw(){
	int dx=mapx<<5;
	int dy=mapy<<4;
	int shf=LMode?2:0;
	for(int i=0;i<NSmp;i++){
		SampleSrc* SS=SSET+i;
		bool CURR=!_stricmp(CurrentSet,SS->Name);
		for(int j=0;j<SS->Npt;j++){
			int x0=(SS->xi[j]<<8)-dx;
			int y0=(SS->yi[j]<<7)-dy;
			Xbar(x0>>shf,y0>>shf,256>>shf,128>>shf,0xFF);
			if(CURR)Xbar((x0>>shf)+1,(y0>>shf)+1,(256>>shf)-2,(128>>shf)-2,0xFF);
			ShowString((x0>>shf)+4,(y0>>shf)+4,SS->Name,&SmallWhiteFont);
		};
		for(j=0;j<SS->NRoots;j++){
			int x0=(SS->ROOT[j].x<<8)-dx;
			int y0=(SS->ROOT[j].y<<7)-dy;
			DWORD MSK=SS->ROOT[j].AttachMask;
			for(int q=0;q<8;q++)if(MSK&(1<<q)){
				int dx=DDDXX[q];
				int dy=DDDYY[q];
				xLine((x0+128)>>shf,(y0+64)>>shf,(x0+128+dx*32)>>shf,(y0+64+dy*16)>>shf,0xFF);
			};
			CBar(((x0+128)>>shf)-4,((y0+64)>>shf)-4,8,8,0x9B);
			ShowString((x0>>shf)+4,(y0>>shf)+16,SS->Name,&SmallRedFont);
			if(SS->ROOT[j].Direction<8){
				byte dd=SS->ROOT[j].Direction;
				int dx=DDDXX[dd];
				int dy=DDDYY[dd];
				xLine((x0+128)>>shf,(y0+64)>>shf,(x0+128+dx*32)>>shf,(y0+64+dy*16)>>shf,0xFF);
			};
		};
	};
};
void SamplesSet::NewSet(char* name){
	for(int i=0;i<NSmp;i++)if(!_stricmp(SSET[i].Name,name))return;
	SSET=(SampleSrc*)realloc(SSET,(NSmp+1)*sizeof SampleSrc);
	SampleSrc* SS=SSET+NSmp;
	strcpy(SS->Name,name);
	strcpy(CurrentSet,name);
	SS->NRoots=0;
	SS->ROOT=NULL;
	SS->Npt=0;
	SS->xi=NULL;
	SS->yi=NULL;
	NSmp++;
};
void SamplesSet::SetActiveSet(char* name){
	//for(int i=0;i<NSmp;i++)if(!_stricmp(SSET[i].Name,name))return;
	strcpy(CurrentSet,name);
};
extern SamplesSet SAMSET;
bool SamSetMode=0;
bool EditRoot=0;
void HandleSMSChat(char* Mess){
	if(!_stricmp(Mess,"::dosmp")){
		SamSetMode=!SamSetMode;
		SAMSET.Passive=0;
	}else
	if(!_stricmp(Mess,"::root")){
		EditRoot=!EditRoot;
		SAMSET.Passive=0;
	}else
	if(!_stricmp(Mess,"::pass")){
		SAMSET.Passive=!SAMSET.Passive;
	}else{
		char cmd[256];
		char par[256];
		int z=sscanf(Mess,"%s%s",cmd,par);
		if(z==2){
			if(!_stricmp(cmd,"::goto")){
				SAMSET.SetActiveSet(par);
			}else
			if(!_stricmp(cmd,"::new")){
				SAMSET.NewSet(par);
			};
		};
	};
};
void HandlwSMSMouse(){
	if(SAMSET.Passive)return;
	if(SamSetMode){
		int xx=((mapx<<5)+(mouseX<<(5-Shifter)))>>8;
		int yy=((mapy<<4)+(mouseY<<(5-Shifter)))>>7;
		if(SAMSET.CurrentSet[0]){
			if(Lpressed){
				if(EditRoot){
					SAMSET.EditRootParam(xx,yy);
					//EditRoot=0;
				}else SAMSET.AddSquare(xx,yy);
				Lpressed=0;
			};
			if(Rpressed){
				SAMSET.DelSquare(xx,yy,1+EditRoot);
				Rpressed=0;
			};
		};
	};
};
void AutoSMSSet(){
	if(SAMSET.Passive)return;
	if(SamSetMode){
		int N=(MaxTH+1)*MaxTH;
		for(int i=0;i<N;i++){
			if(TexMap[i]){
				int x=GetTriX(i)>>8;
				int y=GetTriY(i)>>8;
				SAMSET.AddSquare(x,y);
			};
		};
	};
};
void DrawSMS(){
	if(SamSetMode){
		SAMSET.Draw();
	};
};
void SaveSMSInPieces(ResFile F,int keyX,int keyY,int x0,int y0,int x1,int y1){
	int Nsq=0;
	int Nrt=0;
	for(int i=0;i<SAMSET.NSmp;i++){
		SampleSrc* SS=SAMSET.SSET+i;
		for(int j=0;j<SS->Npt;j++){
			int xx=(SS->xi[j]<<8)+128;
			int yy=(SS->yi[j]<<7)+64;
			if(xx>x0&&yy>y0&&xx<x1&&yy<y1){
				Nsq++;
			};
		};
		for(j=0;j<SS->NRoots;j++){
			int xx=(SS->ROOT[j].x<<8)+128;
			int yy=(SS->ROOT[j].y<<7)+64;
			if(xx>x0&&yy>y0&&xx<x1&&yy<y1){
				Nrt++;
			};
		};
	};
	i='SMSP';
	RBlockWrite(F,&i,4);
	int sz=4+4+4+Nsq*2+Nrt*sizeof(SampleRoot);
	RBlockWrite(F,&sz,4);
	RBlockWrite(F,&Nsq,2);
	RBlockWrite(F,&Nrt,2);
	int dx=keyX&255;
	RBlockWrite(F,&dx,2);
	int dy=keyY&255;
	RBlockWrite(F,&dy,2);
	keyX=(keyX-dx)>>8;
	keyY=(keyY-dy)>>8;
	for(i=0;i<SAMSET.NSmp;i++){
		SampleSrc* SS=SAMSET.SSET+i;
		for(int j=0;j<SS->Npt;j++){
			int xx=(SS->xi[j]<<8)+128;
			int yy=(SS->yi[j]<<7)+64;
			if(xx>x0&&yy>y0&&xx<x1&&yy<y1){
				dx=SS->xi[j]-keyX;
				RBlockWrite(F,&dx,1);
				dx=SS->yi[j]-keyY;
				RBlockWrite(F,&dx,1);
			};
		};
	};
	for(i=0;i<SAMSET.NSmp;i++){
		SampleSrc* SS=SAMSET.SSET+i;
		for(int j=0;j<SS->NRoots;j++){
			int xx=(SS->ROOT[j].x<<8)+128;
			int yy=(SS->ROOT[j].y<<7)+64;
			if(xx>x0&&yy>y0&&xx<x1&&yy<y1){
				SampleRoot SR=SS->ROOT[j];
				SR.x-=keyX;
				SR.y-=keyY;
				RBlockWrite(F,&SR,sizeof SR);
			};
		};
	};
};
void SaveSMSInMap(ResFile F){
	int sz=4+4;
	for(int i=0;i<SAMSET.NSmp;i++){
		SampleSrc* SS=SAMSET.SSET+i;
		sz+=32+4+4+4*SS->Npt+SS->NRoots*sizeof(SampleRoot);
	};
	i='SMSP';
	RBlockWrite(F,&i,4);
	RBlockWrite(F,&sz,4);
	RBlockWrite(F,&SAMSET.NSmp,4);
	for(i=0;i<SAMSET.NSmp;i++){
		RBlockWrite(F,SAMSET.SSET[i].Name,32);
		RBlockWrite(F,&SAMSET.SSET[i].Npt,4);
		RBlockWrite(F,&SAMSET.SSET[i].NRoots,4);
		for(int j=0;j<SAMSET.SSET[i].Npt;j++){
			RBlockWrite(F,&SAMSET.SSET[i].xi[j],2);
			RBlockWrite(F,&SAMSET.SSET[i].yi[j],2);
		};
		for(j=0;j<SAMSET.SSET[i].NRoots;j++){
			RBlockWrite(F,SAMSET.SSET[i].ROOT+j,sizeof SampleRoot);
		};
	};
};
void ClearSMS(){
	SAMSET.ClearAll();
	memset(&SAMSET,0,sizeof SAMSET);
};
void LoadSMSInMap(ResFile F){
	ClearSMS();
	RBlockRead(F,&SAMSET.NSmp,4);
	if(SAMSET.NSmp){
		SAMSET.SSET=new SampleSrc[SAMSET.NSmp];
		memset(SAMSET.SSET,0,SAMSET.NSmp*sizeof SampleSrc);
	};
	for(int i=0;i<SAMSET.NSmp;i++){
		RBlockRead(F,SAMSET.SSET[i].Name,32);
		RBlockRead(F,&SAMSET.SSET[i].Npt,4);
		RBlockRead(F,&SAMSET.SSET[i].NRoots,4);
		if(SAMSET.SSET[i].Npt){
			SAMSET.SSET[i].xi=znew(short,SAMSET.SSET[i].Npt);
			SAMSET.SSET[i].yi=znew(short,SAMSET.SSET[i].Npt);
		};
		if(SAMSET.SSET[i].NRoots){
			SAMSET.SSET[i].ROOT=znew(SampleRoot,SAMSET.SSET[i].NRoots);
		};
		for(int j=0;j<SAMSET.SSET[i].Npt;j++){
			RBlockRead(F,&SAMSET.SSET[i].xi[j],2);
			RBlockRead(F,&SAMSET.SSET[i].yi[j],2);
		};
		for(j=0;j<SAMSET.SSET[i].NRoots;j++){
			RBlockRead(F,SAMSET.SSET[i].ROOT+j,sizeof SampleRoot);
		};
	};
};
#define MaxType 128
struct SampleStamp{
	char Name[64];
	int NS;
	int maxS;

	int * Coor;
};
struct OneLinkStart{
	int x;
	int y;
	DWORD Mask;
	byte Type;
};

class LinkSys{
public:
	SamplesSet LINKS;
	word* Map;
	word* Map2;
	int Nx;
	SampleStamp* STM;
	int NStm;
	OneLinkStart* LIS;
	int NLis;
	int MaxLis;


	word NSMP[MaxType][8];
	word* SMP[MaxType][8];
	byte* SMIDX[MaxType][8];
	void LoadOneLink(char* Name);
	void Init(int Nx,int Ny);
	void Close();
	void LoadSSet(char* list);
	bool GenerateLink(int x,int y,DWORD Mask,byte Type,int BreakProb);
	bool GenerateRandomLink(int BreakProb);
	void SetLockInMap(int x0,int y0,int SmID,int Root);
	bool CheckLockInMap(int x0,int y0,int SmID,int Root);
	void SetLockPt(int x,int y,word Val);
	word GetLockPt(int x,int y);
	void AddStamp(char* Name,int x,int y);
	void ExecuteStamps();
	void ClearStamps();
	void AddLinkStart(int x,int y,DWORD Mask,byte Type);
	~LinkSys(){
		for(int i=0;i<NStm;i++){
			if(STM[i].Coor)free(STM[i].Coor);
		};
		if(STM)free(STM);
		if(LIS)free(LIS);
		ClearStamps();
	};
};
void LinkSys::AddLinkStart(int x,int y,DWORD Mask,byte Type){
	if(NLis>=MaxLis){
		MaxLis+=32;
		LIS=(OneLinkStart*)realloc(LIS,MaxLis*sizeof OneLinkStart);
	};
	LIS[NLis].x=x;
	LIS[NLis].y=y;
	LIS[NLis].Mask=Mask;
	LIS[NLis].Type=Type;
	NLis++;
};
void LinkSys::Init(int x,int y){
	memset(&NSMP,0,sizeof NSMP);
	memset(&SMP,0,sizeof SMP);
	memset(&SMIDX,0,sizeof SMIDX);
	Nx=x;
	Map=znew(word,Nx*Nx);
	memset(Map,0,Nx*Nx*2);
	Map2=znew(word,Nx*Nx);
	memset(Map2,0,Nx*Nx*2);
	STM=NULL;
	NStm=0;
	LIS=NULL;
	NLis=0;
	MaxLis=0;
};
void  LinkSys::LoadOneLink(char* Name){
	char ccx[64];
	sprintf(ccx,"%s.lnk",Name);
	ResFile F=RReset(ccx);
	if(F!=INVALID_HANDLE_VALUE){
		int i;

		RBlockRead(F,&i,4);
		RBlockRead(F,&i,4);
		word Nsq,Nrt;
		RBlockRead(F,&Nsq,2);
		RBlockRead(F,&Nrt,2);
		LINKS.SSET=(SampleSrc*)realloc(LINKS.SSET,(LINKS.NSmp+1)*sizeof(SampleSrc));
		LINKS.NSmp++;
		int cur=LINKS.NSmp-1;
		short dx,dy;

		RBlockRead(F,&dx,2);
		RBlockRead(F,&dy,2);
		SampleSrc* SS=LINKS.SSET+cur;
		SS->dx=dx;
		SS->dy=dy;
		SS->Npt=Nsq;
		SS->NRoots=Nrt;
		strcpy(SS->Name,Name);
		SS->ROOT=znew(SampleRoot,SS->NRoots);
		SS->xi=znew(short,Nsq);
		SS->yi=znew(short,Nsq);
		for(int j=0;j<SS->Npt;j++){
			char xx;
			RBlockRead(F,&xx,1);
			SS->xi[j]=xx;
			RBlockRead(F,&xx,1);
			SS->yi[j]=xx;
		};
		for(j=0;j<Nrt;j++){
			RBlockRead(F,SS->ROOT+j,sizeof SampleRoot);
			int rt=SS->ROOT[j].RootType;
			int k=SS->ROOT[j].Direction;
			//for(int k=0;k<8;k++)if(SS->ROOT[j].AttachMask&(1<<k)){
			int nn=NSMP[rt][k];
			SMP[rt][k]=(word*)realloc(SMP[rt][k],(nn+1)*2);
			SMIDX[rt][k]=(byte*)realloc(SMIDX[rt][k],nn+1);
			SMP[rt][k][nn]=cur;
			SMIDX[rt][k][nn]=j;
			NSMP[rt][k]++;
			//};
		};
		RClose(F);
	};
};
int TypeIDX[64];
int NTypeIDX=0;
void LinkSys::LoadSSet(char* list){
	GFILE* F=Gopen(list,"r");
	if(F){
		char cc1[128];
		int z;
		do{
			int nn=0;
			z=Gscanf(F,"%s",cc1);
			if(z==1){
				LoadOneLink(cc1);
			};
		}while(z==1);
		Gclose(F);
	};
};
#define NRLPAR 4
struct RLPAR{
	bool Busy;
	int x0,y0,tt,idx,type;
	int Prev_x0,Prev_y0,Prev_tt,Prev_idx;
	DWORD NEXTMASK;
};

bool LinkSys::GenerateRandomLink(int rr){
	//int mrpos=1665;
	int x0=mrand()%Nx;
	int y0=mrand()%Nx;
	int nn=mrand()%NTypeIDX;
	return GenerateLink(x0,y0,0xFF,TypeIDX[nn+nn],TypeIDX[nn+nn+1]);
};
/*
bool LinkSys::GenerateLink(int x0,int y0,DWORD Mask,byte Type,int rr){
	RLPAR THR[NRLPAR];
	memset(THR,0xFF,sizeof THR);
	for(int k=0;k<NRLPAR;k++){
		THR[k].Busy=0;
		THR[k].NEXTMASK=0;
		THR[k].type=Type;
	};
	THR[0].Busy=1;
	THR[0].NEXTMASK=0xFF;
	THR[0].x0=x0;
	THR[0].y0=y0;
	
	bool First=1;
	int PrevIDX=-1;
	do{
		for(int p=0;p<NRLPAR;p++)if(THR[p].Busy){
			int NATT=0;
			bool NoDone=1;
			do{
				int dir=mrand()&7;
				DWORD NEXTMASK=THR[p].NEXTMASK;
				if(NEXTMASK&(1<<dir)){
					int nn=NSMP[THR[p].type][dir];
					if(nn){
						int vv=(mrand()*nn)>>15;
						int tt=SMP[THR[p].type][dir][vv];
						int idx=SMIDX[THR[p].type][dir][vv];
						if(idx!=PrevIDX&&CheckLockInMap(THR[p].x0,THR[p].y0,tt,idx)){
							First=0;
							CheckHI=1;
							int x0=THR[p].x0;
							int y0=THR[p].y0;
							
							AddStamp(LINKS.SSET[tt].Name,((x0-LINKS.SSET[tt].ROOT[idx].x)<<8)+LINKS.SSET[tt].dx,
								((y0-LINKS.SSET[tt].ROOT[idx].y)<<8)+LINKS.SSET[tt].dy);
							CheckHI=0;
							PrevIDX=idx;
							//search for final index;
							int ifin;
							int nr=LINKS.SSET[tt].NRoots;
							if(nr==1){
								if(tt!=-1){
									SetLockInMap(x0,y0,tt,idx);
								};
								ifin=-1;
							}else
							if(nr==2){
								ifin=1-idx;
							}else{
								//search for ifin!=idx<16
								ifin=-1;
								for(int t=0;t<nr&&ifin==-1;t++){
									if(t!=idx&&LINKS.SSET[tt].ROOT[t].RootType<15){
										ifin=t;
									};
								};
								int IFIN[3]={-2,-3,-4};;
								for(t=0;t<nr&&IFIN[0]==-1;t++){
									if(t!=idx&&t!=ifin&&LINKS.SSET[tt].ROOT[t].RootType<15){
										IFIN[0]=t;
									};
								};
								for(t=0;t<nr&&IFIN[1]==-1;t++){
									if(t!=idx&&t!=ifin&&t!=IFIN[0]&&LINKS.SSET[tt].ROOT[t].RootType<15){
										IFIN[1]=t;
									};
								};
								for(t=0;t<nr&&IFIN[2]==-1;t++){
									if(t!=idx&&t!=ifin&&t!=IFIN[0]&&t!=IFIN[1]&&LINKS.SSET[tt].ROOT[t].RootType<15){
										IFIN[2]=t;
									};
								};
								for(int vx=0;vx<3;vx++)if(IFIN[vx]>=0){
									for(int s=0;s<NRLPAR;s++)if(!THR[s].Busy){
										THR[s].Busy=1;
										THR[s].NEXTMASK=LINKS.SSET[tt].ROOT[IFIN[vx]].AttachMask;
										THR[s].x0=x0+LINKS.SSET[tt].ROOT[IFIN[vx]].x-LINKS.SSET[tt].ROOT[idx].x;
										THR[s].y0=y0+LINKS.SSET[tt].ROOT[IFIN[vx]].y-LINKS.SSET[tt].ROOT[idx].y;
										THR[s].Prev_x0=-1;
										THR[s].Prev_y0=-1;
										THR[s].Prev_tt=-1;
										THR[s].type=LINKS.SSET[tt].ROOT[IFIN[vx]].RootType;
										s=NRLPAR;
									};
								};
							};
							for(int tv=0;tv<nr;tv++)if(LINKS.SSET[tt].ROOT[tv].RootType>15){
								AddLinkStart(x0+LINKS.SSET[tt].ROOT[tv].x-LINKS.SSET[tt].ROOT[idx].x,
									y0+LINKS.SSET[tt].ROOT[tv].y-LINKS.SSET[tt].ROOT[idx].y,
									LINKS.SSET[tt].ROOT[tv].AttachMask,LINKS.SSET[tt].ROOT[tv].RootType);
							};
							if(ifin!=-1){
								int tx=x0;
								int ty=y0;
								THR[p].x0=x0+LINKS.SSET[tt].ROOT[ifin].x-LINKS.SSET[tt].ROOT[idx].x;
								THR[p].y0=y0+LINKS.SSET[tt].ROOT[ifin].y-LINKS.SSET[tt].ROOT[idx].y;

								NoDone=0;
								THR[p].NEXTMASK=LINKS.SSET[tt].ROOT[ifin].AttachMask;
								if(THR[p].Prev_tt!=-1){
									SetLockInMap(THR[p].Prev_x0,THR[p].Prev_y0,THR[p].Prev_tt,THR[p].Prev_idx);
								};
							
								THR[p].Prev_tt=tt;
								THR[p].Prev_idx=idx;
								THR[p].Prev_x0=x0;
								THR[p].Prev_y0=y0;
							}else return true;
								
						}else{
							if(First)return false;
						};
					};
				}
				NATT++;
			}while(NATT<50&&NoDone);
		};
		rr--;
	}while(rr>0);
	for(int p=0;p<NRLPAR;p++)if(THR[p].Busy&&THR[p].Prev_tt!=-1){
		SetLockInMap(THR[p].Prev_x0,THR[p].Prev_y0,THR[p].Prev_tt,THR[p].Prev_idx);
	};
	return true;
};
*/
bool LinkSys::GenerateLink(int x0,int y0,DWORD Mask,byte Type,int rr){
	RLPAR THR[NRLPAR];
	memset(THR,0xFF,sizeof THR);
	for(int k=0;k<NRLPAR;k++){
		THR[k].Busy=0;
		THR[k].NEXTMASK=0;
		THR[k].type=Type;
	};
	THR[0].Busy=1;
	THR[0].NEXTMASK=0xFF;
	THR[0].x0=x0;
	THR[0].y0=y0;

	int SMP_X0[128];
	int SMP_Y0[128];
	int SMP_VX[128];
	int SMP_VY[128];
	int SMP_TT[128];
	int SMP_IDX[128];
	char* SMP_Name[128];
	int N_SMP=0;

	int ALS_X0[128];
	int ALS_Y0[128];
	int ALS_MASK[128];
	int ALS_RT[128];
	int N_ALS=0;

	int MinSmp=1;
	if(rr>=4)MinSmp=4;
	if(rr>=10)MinSmp=6;
	BOOL AbsFinal=0;
	
	bool First=1;
	int PrevIDX6=-1;
	int PrevIDX5=-1;
	int PrevIDX4=-1;
	int PrevIDX3=-1;
	int PrevIDX2=-1;
	int PrevIDX1=-1;
	int PrevIDX=-1;
	do{
		for(int p=0;p<NRLPAR;p++)if(THR[p].Busy){
			int NATT=0;
			bool NoDone=1;
			do{
				int dir=mrand()&7;
				DWORD NEXTMASK=THR[p].NEXTMASK;
				if(NEXTMASK&(1<<dir)){
					int nn=NSMP[THR[p].type][dir];
					if(nn){
						int vv=(mrand()*nn)>>15;
						int tt=SMP[THR[p].type][dir][vv];
						int idx=SMIDX[THR[p].type][dir][vv];
						if(tt!=PrevIDX&&tt!=PrevIDX1&&tt!=PrevIDX2&&tt!=PrevIDX3&&tt!=PrevIDX4&&tt!=PrevIDX5&&tt!=PrevIDX6&&CheckLockInMap(THR[p].x0,THR[p].y0,tt,idx)){
							First=0;
							CheckHI=1;
							int x0=THR[p].x0;
							int y0=THR[p].y0;
							
							//AddStamp(LINKS.SSET[tt].Name,((x0-LINKS.SSET[tt].ROOT[idx].x)<<8)+LINKS.SSET[tt].dx,
							//	((y0-LINKS.SSET[tt].ROOT[idx].y)<<8)+LINKS.SSET[tt].dy);
							if(N_SMP<128){
								SMP_VX[N_SMP]=((x0-LINKS.SSET[tt].ROOT[idx].x)<<8)+LINKS.SSET[tt].dx;
								SMP_VY[N_SMP]=((y0-LINKS.SSET[tt].ROOT[idx].y)<<8)+LINKS.SSET[tt].dy;
								SMP_X0[N_SMP]=x0;
								SMP_Y0[N_SMP]=y0;
								SMP_Name[N_SMP]=LINKS.SSET[tt].Name;
								SMP_IDX[N_SMP]=idx;
								SMP_TT[N_SMP]=tt;
								N_SMP++;
							};
							CheckHI=0;
							PrevIDX6=PrevIDX5;
							PrevIDX5=PrevIDX4;
							PrevIDX4=PrevIDX3;
							PrevIDX3=PrevIDX2;
							PrevIDX2=PrevIDX1;
							PrevIDX1=PrevIDX;
							PrevIDX=tt;
							//search for final index;
							int ifin;
							int nr=LINKS.SSET[tt].NRoots;
							if(nr==1){
								if(tt!=-1){
									//SetLockInMap(x0,y0,tt,idx);
								};
								ifin=-1;
								if(!First){
									AbsFinal=1;
									NoDone=0;
								};
							}else
							if(nr==2){
								ifin=1-idx;
							}else{
								//search for ifin!=idx<16
								ifin=-1;
								for(int t=0;t<nr&&ifin==-1;t++){
									if(t!=idx&&LINKS.SSET[tt].ROOT[t].RootType<15){
										ifin=t;
									};
								};
								int IFIN[3]={-2,-3,-4};;
								for(t=0;t<nr&&IFIN[0]==-1;t++){
									if(t!=idx&&t!=ifin&&LINKS.SSET[tt].ROOT[t].RootType<15){
										IFIN[0]=t;
									};
								};
								for(t=0;t<nr&&IFIN[1]==-1;t++){
									if(t!=idx&&t!=ifin&&t!=IFIN[0]&&LINKS.SSET[tt].ROOT[t].RootType<15){
										IFIN[1]=t;
									};
								};
								for(t=0;t<nr&&IFIN[2]==-1;t++){
									if(t!=idx&&t!=ifin&&t!=IFIN[0]&&t!=IFIN[1]&&LINKS.SSET[tt].ROOT[t].RootType<15){
										IFIN[2]=t;
									};
								};
								for(int vx=0;vx<3;vx++)if(IFIN[vx]>=0){
									for(int s=0;s<NRLPAR;s++)if(!THR[s].Busy){
										THR[s].Busy=1;
										THR[s].NEXTMASK=LINKS.SSET[tt].ROOT[IFIN[vx]].AttachMask;
										THR[s].x0=x0+LINKS.SSET[tt].ROOT[IFIN[vx]].x-LINKS.SSET[tt].ROOT[idx].x;
										THR[s].y0=y0+LINKS.SSET[tt].ROOT[IFIN[vx]].y-LINKS.SSET[tt].ROOT[idx].y;
										THR[s].Prev_x0=-1;
										THR[s].Prev_y0=-1;
										THR[s].Prev_tt=-1;
										THR[s].type=LINKS.SSET[tt].ROOT[IFIN[vx]].RootType;
										s=NRLPAR;
									};
								};
							};
#ifdef CONQUEST
							for(int tv=0;tv<nr;tv++)if(LINKS.SSET[tt].ROOT[tv].RootType>63){
#else
							for(int tv=0;tv<nr;tv++)if(LINKS.SSET[tt].ROOT[tv].RootType>15){
#endif
								//AddLinkStart(x0+LINKS.SSET[tt].ROOT[tv].x-LINKS.SSET[tt].ROOT[idx].x,
								//	y0+LINKS.SSET[tt].ROOT[tv].y-LINKS.SSET[tt].ROOT[idx].y,
								//	LINKS.SSET[tt].ROOT[tv].AttachMask,LINKS.SSET[tt].ROOT[tv].RootType);
								if(N_ALS<128){
									ALS_X0  [N_ALS]=x0+LINKS.SSET[tt].ROOT[tv].x-LINKS.SSET[tt].ROOT[idx].x;
									ALS_Y0  [N_ALS]=y0+LINKS.SSET[tt].ROOT[tv].y-LINKS.SSET[tt].ROOT[idx].y;
									ALS_MASK[N_ALS]=LINKS.SSET[tt].ROOT[tv].AttachMask;
									ALS_RT  [N_ALS]=LINKS.SSET[tt].ROOT[tv].RootType;
									N_ALS++;
								};
							};
							if(ifin!=-1){
								int tx=x0;
								int ty=y0;
								THR[p].x0=x0+LINKS.SSET[tt].ROOT[ifin].x-LINKS.SSET[tt].ROOT[idx].x;
								THR[p].y0=y0+LINKS.SSET[tt].ROOT[ifin].y-LINKS.SSET[tt].ROOT[idx].y;

								NoDone=0;
								THR[p].NEXTMASK=LINKS.SSET[tt].ROOT[ifin].AttachMask;
								if(THR[p].Prev_tt!=-1){
									//SetLockInMap(THR[p].Prev_x0,THR[p].Prev_y0,THR[p].Prev_tt,THR[p].Prev_idx);
								};
							
								THR[p].Prev_tt=tt;
								THR[p].Prev_idx=idx;
								THR[p].Prev_x0=x0;
								THR[p].Prev_y0=y0;
							};//else return true;
								
						}else{
							if(First)return false;
						};
					};
				}
				NATT++;
			}while(NATT<50&&NoDone);
		};
		rr--;
	}while(rr>0);
	if(N_SMP>=MinSmp||(AbsFinal&&N_SMP>4)){
		for(int i=0;i<N_SMP;i++){
			AddStamp(SMP_Name[i],SMP_VX[i],SMP_VY[i]);
			SetLockInMap(SMP_X0[i],SMP_Y0[i],SMP_TT[i],SMP_IDX[i]);
		};
		for(i=0;i<N_ALS;i++){
			AddLinkStart(ALS_X0[i],ALS_Y0[i],ALS_MASK[i],ALS_RT[i]);
		};
	};
	//for(int p=0;p<NRLPAR;p++)if(THR[p].Busy&&THR[p].Prev_tt!=-1){
	//	SetLockInMap(THR[p].Prev_x0,THR[p].Prev_y0,THR[p].Prev_tt,THR[p].Prev_idx);
	//};
	return true;
};
//LinkSys LSS;
void TEST_GM(){
	
};
word LIDX=1;
void LinkSys::SetLockInMap(int x0,int y0,int SmID,int Root){
	SampleRoot* SR=LINKS.SSET[SmID].ROOT+Root;
	x0-=SR->x;
	y0-=SR->y;
	int N=LINKS.SSET[SmID].Npt;
	short* xi=LINKS.SSET[SmID].xi;
	short* yi=LINKS.SSET[SmID].yi;
	for(int i=0;i<N;i++)SetLockPt(x0+xi[i],y0+yi[i],SmID);
	LIDX++;
};
bool LinkSys::CheckLockInMap(int x0,int y0,int SmID,int Root){
	SampleRoot* SR=LINKS.SSET[SmID].ROOT+Root;
	x0-=SR->x;
	y0-=SR->y;
	int N=LINKS.SSET[SmID].Npt;
	short* xi=LINKS.SSET[SmID].xi;
	short* yi=LINKS.SSET[SmID].yi;
	for(int i=0;i<N;i++)if(GetLockPt(x0+xi[i],y0+yi[i]))return false;
	return true;
};
void LinkSys::SetLockPt(int x,int y,word Val){
	if(x<0||x>=Nx||y<0||y>=Nx)return;
	if(Map[x+y*Nx]){
		Map2[x+y*Nx]=LIDX;
		return;
	}else{;
		Map[x+y*Nx]=LIDX;//=Val;
		Map2[x+y*Nx]=LIDX;
	};
};
word LinkSys::GetLockPt(int x,int y){
	if(x<0||x>=Nx||y<0||y>=Nx)return 1;
	return Map[x+y*Nx];
};
//LinkSys LSS;
void Draw_GRASS();
void DrawAreas(int xs,int ys,int x0,int y0,int Lx,int Ly);
void DrawWaysToSel();
void DRAW_MAP_TMP(){
	DrawWaysToSel();
	//DrawAreas(100,30,0,0,TopLx,600);
	//Draw_GRASS();
	/*
	int Lx=LSS.Nx;
	word* M;
	if(GetKeyState(VK_CONTROL)&0x8000){
		M=LSS.Map2;
	}else M=LSS.Map;
	for(int i=0;i<Lx;i++)for(int j=0;j<Lx;j++)
		if(M[i+j*Lx]){
			CBar(i*2,40+j*2,2,2,0xD4);//+(M[i+j*Lx]<<2));
			//char cc[32];
			//sprintf(cc,"%d",M[i+j*Lx]);
			//ShowString(i*15,40+j*10-2,cc,&fn8);
		};
	*/
};
void EraseBlockPt();
void GenerateWithStyle(char* terr){
	GPROG.SetCurrentStage('FTEX');
	SetFractalTexture();
	LinkSys LSS;
	int NY=30<<ADDSH;
	LSS.Init(NY,NY);
	int VIDX[512];
	for(int i=0;i<NY;i++)VIDX[i]=i*NY;
	//1.
	int vrt=0;
	for(int j=0;j<MaxTH;j++){
		for(i=0;i<VertInLine;i++){
			byte tex=TexMap[vrt];
			if(tex>127){
				LSS.SetLockPt(i>>3,j>>3,1);
			};
			vrt++;
		};
	};
	GPROG.SetCurrentStage('LINK');
	GFILE* F=Gopen(terr,"r");
	if(F){
		char cc[128];
		int N;
		int z;
		do{
			z=Gscanf(F,"%s%d%d",cc,&N,&NTypeIDX);
			
			N<<=ADDSH-1;
			if(z==3){
				for(int j=0;j<NTypeIDX;j++)Gscanf(F,"%d%d",&TypeIDX[j+j],&TypeIDX[j+j+1]);
				LSS.LoadSSet(cc);
				for(int i=0;i<N;i++){
					LSS.GenerateRandomLink(10);
				};
				for(i=0;i<LSS.NLis;i++){
					LSS.GenerateLink(LSS.LIS[i].x,LSS.LIS[i].y,LSS.LIS[i].Mask,LSS.LIS[i].Type,50);
				};
				//LSS.LINKS.ClearAll();
				//LSS.ClearStamps();
			};
		}while(z==3);
		Gclose(F);
		LSS.ExecuteStamps();
	};
	EraseBlockPt();
};

void LinkSys::AddStamp(char* Name,int x,int y){
	for(int i=0;i<NStm;i++)if(!_stricmp(Name,STM[i].Name)){
		if(STM[i].NS>=STM[i].maxS){
			STM[i].maxS+=32;
			STM[i].Coor=(int*)realloc(STM[i].Coor,STM[i].maxS<<3);
		};
		int p=STM[i].NS;
		STM[i].NS++;
		STM[i].Coor[p+p]=x;
		STM[i].Coor[p+p+1]=y;
		return;
	};
	STM=(SampleStamp*)realloc(STM,(NStm+1)*sizeof SampleStamp);
	strcpy(STM[NStm].Name,Name);
	STM[NStm].Coor=znew(int,8*32);
	STM[NStm].NS=1;
	STM[NStm].maxS=32;
	STM[NStm].Coor[0]=x;
	STM[NStm].Coor[1]=y;
	NStm++;
};
void LinkSys::ClearStamps(){
	for(int i=0;i<NStm;i++){
		free(STM[i].Coor);
	};
	if(STM)free(STM);
	NStm=0;
	STM=NULL;
	if(LIS)free(LIS);
	NLis=0;
	MaxLis=0;
	for(i=0;i<MaxType;i++){
		for(int j=0;j<8;j++)if(SMP[i][j])free(SMP[i][j]);
		for(j=0;j<8;j++)if(SMIDX[i][j])free(SMIDX[i][j]);
	};
	memset(&NSMP,0,sizeof NSMP);
	memset(&SMP,0,sizeof SMP);
	memset(&SMIDX,0,sizeof SMIDX);
};

void FAST_RM_Load(SaveBuf* SB,int x,int y);
byte IsMount;
void LinkSys::ExecuteStamps(){
	byte NAT=MyNation;
	SetMyNation(7);
	for(int i=0;i<NStm;i++){
		IsMount=1;
		SaveBuf SB;
		SB.Init();
		_strupr(STM[i].Name);
		if(strstr(STM[i].Name,"LOW_"))IsMount=0;
		ResFile F=RReset(STM[i].Name);
		if(F!=INVALID_HANDLE_VALUE){
			SB.LoadFromFile(F);
			int n=STM[i].NS;
			for(int j=0;j<n;j++){
				CheckHI=1;
				FAST_RM_Load(&SB,STM[i].Coor[j+j],STM[i].Coor[j+j+1]);
				CheckHI=0;
			};
			SB.Clear();
			RClose(F);
		};
		//FastLoadSample
	};
	ClearStamps();
	//checking sprite objects
	for(i=0;i<MaxSprt;i++){
		OneSprite* OS=Sprites+i;
		if(OS->Enabled){
			if(OS->z<GetHeight(OS->x,OS->y))EraseSprite(i);
		};
	};
	SetMyNation(NAT);
};
//-----------------OPTIMIZING----------------//

void xBlockRead(SaveBuf* SB,void* Data,int Size);
inline int GET_INT(SaveBuf* SB){
	SB->Pos+=4;
	return *((int*)(SB->Buf+SB->Pos-4));
};
inline int GET_DWORD(SaveBuf* SB){
	SB->Pos+=4;
	return *((DWORD*)(SB->Buf+SB->Pos-4));
};
inline int GET_BYTE(SaveBuf* SB){
	SB->Pos++;
	return *((byte*)(SB->Buf+SB->Pos-1));
};
inline int GET_CHAR(SaveBuf* SB){
	SB->Pos++;
	return *((char*)(SB->Buf+SB->Pos-1));
};
inline int GET_SHORT(SaveBuf* SB){
	SB->Pos+=2;
	return *((short*)(SB->Buf+SB->Pos-2));
};
inline int GET_WORD(SaveBuf* SB){
	SB->Pos+=2;
	return *((word*)(SB->Buf+SB->Pos-2));
};
inline void GET_BUF(SaveBuf* SB,int size,byte* Buf){
	memcpy(Buf,SB->Buf+SB->Pos,size);
	SB->Pos+=size;
};
int FAST_RM_LoadVertices(SaveBuf* SB,int Vsx,int Vsy,int* VIDX,int MaxVIDX,bool Vnew=0){
	int Nv;
	bool NPOS=0;
	bool NNEG=0;
	Nv=GET_INT(SB);
	int CIdx=0;
	for(int i=0;i<Nv;i++){
		short vx;
		short vy;
		byte tex;
		byte s1,s2,s3;
		short Height;
		vx=GET_SHORT(SB);
		vy=GET_SHORT(SB);
		vx+=Vsx;
		vy+=Vsy;
		tex=GET_BYTE(SB);
#ifdef _USE3D
		byte fc,fw;
		if(Vnew){
			fc=GET_BYTE(SB);
			fw=GET_BYTE(SB);
		};
#endif //_USE3D
		s1=GET_BYTE(SB);
		s2=GET_BYTE(SB);
		s3=GET_BYTE(SB);
		Height=GET_SHORT(SB);

		if(Height>0)NPOS++;
		if(Height<0)NNEG++;

		if(vx>0&&vy>0&&vx<VertInLine-1&&vy<MaxTH-1){
			int StartSect;
				
			int v=vx+vy*VertInLine;
			if(v>=0&&v<MaxPointIndex){
				if(CheckHI){
					if(!IsMount){
						if(THMap[v]>=Height){
							if(v<VertInLine){
								if(Height<0)THMap[v]=0;
								THMap[v]=Height;
							}else{
								THMap[v]=Height;
							};
							if(tex!=0&&tex<ntex){
								TexMap[v]=tex+128;
							};
#ifdef _USE3D
							if(Vnew){
								FactureMap[v]=fc;
								FactureWeight[v]=fw;
							};
#endif //_USE3D
							if(CIdx<MaxVIDX){
								VIDX[CIdx]=v;
								CIdx++;
							};
						};
					}else{
						if(THMap[v]<=Height){
							if(v<VertInLine){
								if(Height<0)THMap[v]=0;
								THMap[v]=Height;
							}else{
								THMap[v]=Height;
							};
							if(tex!=0&&tex<ntex){
								TexMap[v]=tex+128;
							};
#ifdef _USE3D
							if(Vnew){
								FactureMap[v]=fc;
								FactureWeight[v]=fw;
							};
#endif //_USE3D
							if(CIdx<MaxVIDX){
								VIDX[CIdx]=v;
								CIdx++;
							};
						};
					};
				}else{
					THMap[v]+=Height;
					if(tex!=0&&tex<ntex){
						TexMap[v]=tex+128;
					};
#ifdef _USE3D
					if(Vnew){
						FactureMap[v]=fc;
						FactureWeight[v]=fw;
					};
#endif //_USE3D
				};
			};
		};
	};
	return CIdx;
};
void FAST_RM_LoadSprites(SaveBuf*SB,int xc,int yc){
	int ns;
	ns=GET_INT(SB);
	int maxx=(msx<<5)-32;
	int maxy=(msy<<5)-32;
	for(int i=0;i<ns;i++){
		int x,y;
		word sign,id;
		sign=GET_WORD(SB);
		x=GET_INT(SB);
		y=GET_INT(SB);
		id=GET_WORD(SB);
		
		x+=xc;
		y+=yc;
		//if(LastAddSpr>50000)return;
		if(x>0&&y>0&&x<maxx&&y<maxy){
			int z=GetHeight(x,y);
			if(z>1024){
				if(sign=='GA'){
					addSpriteAnyway(x,y,&TREES,id);
				}else if(sign=='TS'){
					addSpriteAnyway(x,y,&STONES,id);
				}else if(sign=='OH'){
					addSpriteAnyway(x,y,&HOLES,id);
				}else if(sign=='WL'){
					addSpriteAnyway(x,y,&WALLS,id);
				}else if(sign=='OC'){
					addSpriteAnyway(x,y,&COMPLEX,id);
				};
			};
		};
	};
};
void FastAddLockBar(word x,word y);
void FastAddUnLockBar(word x,word y);
void FAST_RM_LoadLock(SaveBuf* SB,int xc,int yc){
	int NL,NU;
	NL=GET_INT(SB);
	NU=GET_INT(SB);
	for(int i=0;i<NL;i++){
		short x,y;
		x=GET_SHORT(SB);
		y=GET_SHORT(SB);
		x+=xc;
		y+=yc;
		FastAddLockBar(x,y);
	};
	for(i=0;i<NU;i++){
		short x,y;
		x=GET_SHORT(SB);
		y=GET_SHORT(SB);
		x+=xc;
		y+=yc;
		FastAddUnLockBar(x,y);
	};
};
void FAST_RM_LoadObj(SaveBuf* SB,int xc,int yc){
	int ns=GET_INT(SB);
	char ONAME[32];
	for(int i=0;i<ns;i++){
		int xm,ym;
		xm=GET_INT(SB);
		ym=GET_INT(SB);
		byte NT=GET_BYTE(SB);
		NT=MyNation;
		xBlockRead(SB,ONAME,31);
		GeneralObject** GO=NATIONS[NT].Mon;
		int Nmon=NATIONS[NT].NMon;
		for(int p=0;p<Nmon;p++){
			if(!strcmp(GO[p]->MonsterID,ONAME)){
				if(!GO[p]->newMons->Wall){
					CreateNewUnitAt(NT,(xc+xm)<<4,(yc+ym)<<4,p,GO[p]->MoreCharacter->Life,GO[p]->MoreCharacter->ProduceStages);
					goto ttt;
				};
			};
		};
ttt:;
	};
};

void FAST_RM_LoadPix(SaveBuf* SB,int xc,int yc){
	int sz=GET_INT(SB);
	byte* ptr=znew(byte,sz);
	GET_BUF(SB,sz,ptr);
	SetPixelsFromBuf(xc>>5,yc>>5,ptr,sz);
	free(ptr);
};
void FAST_RM_LoadGround(SaveBuf* SB,int xc,int yc){
	int sz=GET_INT(SB);
	byte* ptr=znew(byte,sz);
	GET_BUF(SB,sz,ptr);
	SetGroundFromBuf(xc>>5,yc>>5,ptr,sz,2);
	free(ptr);
};
void FAST_RM_Load(SaveBuf* SB,int x,int y){
	int TMP_VERTEX[8192];
	int NVert=0;
	int tt0=GetTickCount();
	int tt1,tt2,tt3,tt4;
	SB->Pos=0;
	int SIGN,SIZE;
	SIGN=GET_INT(SB);
	if(SIGN=='PMAS'){
		while(SIGN!=-1){
			SIGN=-1;
			SIGN=GET_INT(SB);
			SIZE=GET_INT(SB);
			switch(SIGN){
			case 'TREV':
				tt1=GetTickCount();
				NVert=FAST_RM_LoadVertices(SB,(x>>6)<<1,(y>>6)<<1,TMP_VERTEX,8192);
				tt1=GetTickCount()-tt1;
				break;
#ifdef _USE3D
			case '3REV':
				NVert=FAST_RM_LoadVertices(SB,(x>>6)<<1,(y>>6)<<1,TMP_VERTEX,8192,1);
				break;
			case 'RPSG':
					FAST_RM_LoadGSprites(SB,(x>>6)<<6,(y>>6)<<6);
					break;
#endif
			case 'TRPS':
				tt2=GetTickCount();
				{
					for(int i=0;i<NVert;i++)THMap[TMP_VERTEX[i]]+=2048;
					FAST_RM_LoadSprites(SB,(x>>6)<<6,(y>>6)<<6);
					for(i=0;i<NVert;i++)THMap[TMP_VERTEX[i]]-=2048;
				};
				tt2=GetTickCount()-tt2;
				break;
			case 'KCOL':
				tt3=GetTickCount();
				FAST_RM_LoadLock(SB,x>>6,y>>6);
				tt3=GetTickCount()-tt3;
				break;
			case 'SJBO':
				tt4=GetTickCount();
				FAST_RM_LoadObj(SB,(x>>6)<<6,(y>>6)<<6);
				tt4=GetTickCount()-tt4;
				break;
			case '1XIP':
				FAST_RM_LoadPix(SB,(x>>6)<<6,(y>>6)<<5);
				break;
			case '1GRN':
				FAST_RM_LoadGround(SB,(x>>6)<<6,(y>>6)<<5);
				break;
			case 'ENOZ':
				FAST_RM_LoadZonesAndGroups(SB,(x>>6)<<6,(y>>6)<<6);
				break;
			};
		};
	};
	tt0=GetTickCount()-tt0;
	tt0=0;
};

byte carr[39]={47,241,85,85,45,45,47,87,45,87,47,85,241,85,85,45,47,241,85,85,45,47,85,45,45,47,85,91,45,47,85};
void Draw1(int x,int y){
	
};
void Draw_GRASS(){
	
};
byte GrassMask[256]={
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,1,0,1,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,

	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	0,0,0,1, 1,1,1,1, 1,1,1,1, 0,0,0,0,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,

	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,

	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	0,1,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
	
void ProlongGrass(int x,int y){	
};
int dx_p[11]={-5,-4,-3, 2,-1, 0, 1, 2, 3, 4, 5};
int dy_p[11]={ 1, 3, 2, 0, 2, 4, 2, 3, 1, 2, 3};
int sprs;
int getr(){
	sprs=(sprs+1)&8191;
	return randoma[sprs];
};
void DrawGrassNearUnit(int x,int y){
	/*
	sprs=(x*3+y*7)&127;
	for(int i=-12;i<13;i++)
		for(int j=-4;j<6;j++){
			ProlongGrass(x+i,y+j+(getr()&3));
		};
		*/
};
extern int LastMx;
extern int LastMy;
void DrawWaysToSel(){	
};

void GlobalProgress::Setup(){
	memset(this,0,sizeof GlobalProgress);
};
void GlobalProgress::AddPosition(int ID,int Weight,int Max){
	if(NWeights<64){
		StageID[NWeights]=ID;
		StageWeight[NWeights]=Weight;
		StagePositions[NWeights]=Max;
		MaxPosition+=Weight;
		NWeights++;
	};
};
void GlobalProgress::SetCurrentStage(int ID){
	CurStage=0;
	for(int i=0;i<NWeights;i++)if(StageID[i]==ID)CurStage=i;
	CurPosition=0;
};
void GlobalProgress::SetCurrentPosition(int Pos){
	CurPosition=Pos;
	if(CurPosition>StagePositions[CurStage])CurPosition=StagePositions[CurStage]-1;
	if(CurPosition<0)CurPosition=0;
};
int GlobalProgress::GetCurProgress(){
	int S=0;
	for(int i=0;i<CurStage;i++)S+=StageWeight[i];
	S+=(CurPosition*StageWeight[i])/StagePositions[i];
	if(MaxPosition)return (S*100)/MaxPosition;
	else return 100;
};
GlobalProgress::GlobalProgress(){
	Setup();
};
extern GlobalProgress GPROG;
byte ExchangeTex(byte tex){
	switch(tex){
	case 0:return 17;
	case 3:
	case 7:return mrand()&1?56:57;
	case 8:
	case 9:
	case 10:return mrand()&1?58:59;
	case 6:
	case 1:return 16;
	case 5:
	case 2:return 12;
	case 4:return 13;
	case 45:
	case 11:return mrand()&1?23:13;
	};
	return tex;
};
void MakeDesert(){
	int N=(MaxTH+1)*MaxTH;
	for(int i=0;i<N;i++){
		TexMap[i]=ExchangeTex(TexMap[i]);
	};
	int TRDES[8]={79,80,81,82,84,79,80,82};
	for(i=0;i<MaxSprt;i++){
		OneSprite* OS=Sprites+i;
		if(OS->Enabled&&OS->SG==&TREES){
			OS->SGIndex=TRDES[mrand()&7];
			OS->OC=OS->SG->Objects[OS->SGIndex];
		};
	};
};
//------------------Facturing the surface--------------//
byte FacturesSet[256][5];
void LoadFacturesSet(){
	GFILE* F=Gopen("Pieces\\factures_set.dat","r");
	memset(FacturesSet,0xFF,sizeof FacturesSet);
	if(F){
		int z=0;
		do{
			int ti,t1,t2,t3,t4,tt;
			z=Gscanf(F,"%d%d%d%d%d%d",&ti,&t1,&t2,&t3,&t4,&tt);
			if(z==6&&ti<=255){
				FacturesSet[ti][0]=t1;
				FacturesSet[ti][1]=t2;
				FacturesSet[ti][2]=t3;
				FacturesSet[ti][3]=t4;
				FacturesSet[ti][4]=tt;
			};
		}while(z==6);
		Gclose(F);
	};
};
void LoadFacturesSet(int set){
	char fset[128];
	sprintf(fset,"Terrain\\factures_set%d.dat",set);
	GFILE* F=Gopen(fset,"r");
	memset(FacturesSet,0xFF,sizeof FacturesSet);
	if(F){
		int z=0;
		do{
			int ti,t1,t2,t3,t4,tt;
			z=Gscanf(F,"%d%d%d%d%d%d",&ti,&t1,&t2,&t3,&t4,&tt);
			if(z==6&&ti<=255){
				FacturesSet[ti][0]=t1;
				FacturesSet[ti][1]=t2;
				FacturesSet[ti][2]=t3;
				FacturesSet[ti][3]=t4;
				FacturesSet[ti][4]=tt;
			};
		}while(z==6);
		Gclose(F);
	};
};
extern char* PointTexWeight;
extern word* PointTexIndex;
void AutoSetFactures(){
#ifdef _USE3D
	extern byte* FactureMap;
	extern byte* FactureWeight;
	int N=MaxPointIndex;
	for(int i=0;i<N;i++){
		int t=TexMap[i];
		if(FacturesSet[t][0]!=0xFF){
			FactureMap[i]=FacturesSet[t][mrand()&3];
			FactureWeight[i]=FacturesSet[t][4];
		};
	};
#else //_USE3D
	byte* FactMap;
	byte* FactWeight;
	int Nx=256<<ADDSH;
	int Ny=128<<ADDSH;
	int SHF=8+ADDSH;
	int sz=Nx*Ny;
	FactMap=(byte*)malloc(sz);
	FactWeight=(byte*)malloc(sz);
	memset(FactMap,0xFF,sz);
	memset(FactWeight,0,sz);
	int ofs=0;
	for(int iy=4;iy<MaxTH-4;iy++){
		ofs=iy*VertInLine+1;
		for(int ix=1;ix<VertInLine-4;ix++){
			int H=THMap[ofs];
			if(H<=70){
				int t=TexMap[ofs];
				if(FacturesSet[t][0]!=0xFF){
					int off1=ix+(((iy>>1)-(H>>5))<<SHF);
					FactMap[off1]=FacturesSet[t][mrand()&3];
					FactWeight[off1]=FacturesSet[t][4];
				};
			};
			ofs++;
		};
	};
	int N=(256*120)<<(ADDSH+ADDSH);
	for(int p=256<<ADDSH;p<N;p++){
		if(FactMap[p]!=0xFF){
			int ix=p&127;
			int iy=p/Nx;
			int dx,dy;
			int ox;
			int oy=iy/3;
			if(oy&1){
				ox=ix/3;
			}else{
				ox=(ix+1)/3;
			}
			if(oy&1){
				dx=randoma[(ox*19+oy*17)&8191]&7;
				dy=randoma[(ox*13+oy*23)&8191]&7;
			};
			PointTexIndex[p]=(int(FactMap[p])<<6)+dx+(dy<<3);
			PointTexWeight[p]=(int(FactWeight[p-1])+int(FactWeight[p+1])+int(FactWeight[p-Nx])+int(FactWeight[p+Nx]))>>2;
		};
	};
	free(FactMap);
	free(FactWeight);
#endif //_USE3D
};
void EraseBlockPt(){
	int N=LockBars.NBars;
	int np=0;
	for(int i=0;i<N;i++){
		int x=LockBars.BC[i].x;
		int y=LockBars.BC[i].y;
		int x0=x<<6;
		int y0=y<<6;
		int z0=GetHeight(x0+32,y0+32);
		int z1=GetHeight(x0,y0);
		int z2=GetHeight(x0+64,y0);
		int z3=GetHeight(x0,y0+64);
		int z4=GetHeight(x0+64,y0+64);
		int zx=(z2+z4-z1-z3)/2;
		int zy=(z1+z2-z3-z4)/2;
		int zz=64;
		int rr=sqrt(zx*zx+zy*zy);
		if(rr>35){
			LockBars.BC[np].x=x;
			LockBars.BC[np].y=y;
			np++;
		};
	};
	LockBars.NBars=np;
};
