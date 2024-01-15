#include <crtdbg.h>
#include <assert.h>
#include "..\COSSACKS2\common.h"
#include "IntExplorer.h"
#include "ParseRQ.h"
#include "bmptool.h"
#include "GameonMap.h"
OneMap::OneMap(){
	memset(this,0,sizeof *this);
};

void PushWindow(TempWindow* W){
	W->ScrWidth=ScrWidth;
#ifndef _COSSACKS2
	W->WindLx=WindLx;
	W->WindLy=WindLy;
	W->WindX=WindX;
	W->WindX1=WindX1;
	W->WindY=WindY;
	W->WindY1=WindY1;
#else
	g_GetWindParam( W->WindX, W->WindY, W->WindX1, W->WindY1 );
	W->WindLx	= W->WindX1 - W->WindX + 1;
	W->WindLy	= W->WindY1 - W->WindY + 1;
#endif
};
void PopWindow(TempWindow* W){
	ScrWidth=W->ScrWidth;
#ifndef _COSSACKS2
	WindLx=W->WindLx;
	WindLy=W->WindLy;
	WindX=W->WindX;
	WindX1=W->WindX1;
	WindY=W->WindY;
	WindY1=W->WindY1;
#else
	g_SetWindParam( W->WindX, W->WindY, W->WindLx, W->WindLy );
#endif
	
};
void IntersectWindows(int x0,int y0,int x1,int y1){
#ifndef _COSSACKS2
	if(x0>WindX)WindX=x0;
	if(y0>WindY)WindY=y0;
	if(x1<WindX1)WindX1=x1;
	if(y1<WindY1)WindY1=y1;
	if(WindX1<WindX){
		WindX1=0;
		WindX=0;
	};
	if(WindY1<WindY){
		WindY1=0;
		WindY=0;
	};
	WindLx=WindX1-WindX+1;
	WindLy=WindY1-WindY+1;
#else
	int WindX, WindY, WindX1, WindY1;
	g_GetWindParam( WindX, WindY, WindX1, WindY1 );

	int WindLx = WindX1 - WindX + 1;
	int WindLy = WindY1 - WindY + 1;
	
	if(x0>WindX)WindX=x0;
	if(y0>WindY)WindY=y0;
	if(x1<WindX1)WindX1=x1;
	if(y1<WindY1)WindY1=y1;
	if(WindX1<WindX){
		WindX1=0;
		WindX=0;
	};
	if(WindY1<WindY){
		WindY1=0;
		WindY=0;
	};
	WindLx=WindX1-WindX+1;
	WindLy=WindY1-WindY+1;
	g_SetWindParam( WindX, WindY, WindX1, WindY1 );
#endif // _COSSACKS2
};

CIMPORT byte GetPaletteColor(int r,int g,int b);
CIMPORT void GetPalColor(byte idx,byte* r,byte* g,byte* b);
void SendSmartRequest(sicExplorer* SXP,char* Str);
bool ReadWinStringF(FILE* F,char* STR,int Max){
	STR[0]=0;
	int cc=0;
	//int z1=0;
	int nn=0;
	while(!(cc==0x0A||cc==EOF)){
		cc=fgetc(F);
		if(Max>2&&!(cc==0x0A||cc==EOF)){
			STR[nn]=cc;
			nn++;
			Max--;
		};
	};
	STR[nn]=0;
	return (cc!=EOF)||STR[0];
	//DosToWin(STR);
};
void OneMap::LoadMapData(sicExplorer* SXP){
	//loading colors
	char ccc[64];
	sprintf(ccc,"Internet\\Cash\\%s.colors",Title);
	FILE* F=fopen(ccc,"r");
	if(F){
		int z;
		int idx=0;
		do{
			int r0,g0,b0;
			z=fscanf(F,"%d%d%d",&r0,&g0,&b0);
			if(z==3){
				sprintf(ccc,"Internet\\Cash\\colors_%d_%d_%d.dat",r0,g0,b0);
				ResFile F=RReset(ccc);
				COLORS[idx].r=r0;
				COLORS[idx].g=g0;
				COLORS[idx].b=b0;				
			};
			idx++;
		}while(z==3&&idx<255);
		COLORS[255].r=255;
		COLORS[255].g=255;
		COLORS[255].b=255;
		fclose(F);		
	};
	//loading set of maps
	int i;
	for(i=0;i<=100;i++){
		sprintf(ccc,"Internet\\Cash\\%s_%d.bmp",Title,i);
		BMFH BM;
		byte* Data;
		byte PAL[768];
		if(ReadBMP8PAL(ccc,&BM,&Data,PAL)){
			SCALE=(OneScale*)realloc(SCALE,(NScales+1)*sizeof OneScale);
			SCALE[NScales].Data=Data;
			SCALE[NScales].Loaded=1;
			SCALE[NScales].Lx=BM.biWidth;
			SCALE[NScales].Ly=BM.biHeight;
			SCALE[NScales].Scale=i;
			SCALE[NScales].Colored=0;
			SCALE[NScales].ColoredData=NULL;
			SCALE[NScales].FONT[0]=0;
			memcpy(SCALE[NScales].PAL,PAL,768);
			NScales++;
		};
	};
	sprintf(ccc,"Internet\\Cash\\%s.fonts",Title);
	F=fopen(ccc,"r");
	if(F){
		for(i=0;i<NScales;i++){
			int z=fscanf(F,"%s",SCALE[i].FONT);
			if(z){
				SCALE[i].Font=NULL;
				SCALE[i].Font=SXP->GetFontByName(SCALE[i].FONT);
			};
		};
	};
	//loading base countries distribution
	sprintf(ccc,"Internet\\Cash\\%s_nations.bmp",Title,i);
	BMFH BM;
	byte* Data;
	memset(XSumm,0,sizeof XSumm);
	memset(YSumm,0,sizeof YSumm);
	memset(NPoints,0,sizeof NPoints);

	if(ReadBMP8(ccc,&BM,&Data)){
		CountryMap=Data;
		CountryLx=BM.biWidth;
		CountryLy=BM.biHeight;
		int ofs=0;
		int MAXLX[256];
		int CY[256];
		int CX[256];
		memset(MAXLX,0,sizeof MAXLX);
		byte prevc=255;
		int cx=0;
		int cx1=0;
		int CLX=0;
		for(int iy=0;iy<CountryLy-7;iy++){
			for(int ix=0;ix<CountryLx;ix++){
				byte c=Data[ofs];
				byte c1=Data[ofs+CountryLx];
				byte c2=Data[ofs+CountryLx*2];
				byte c3=Data[ofs+CountryLx*3];
				byte c4=Data[ofs+CountryLx*4];
				byte c5=Data[ofs+CountryLx*5];
				byte c6=Data[ofs+CountryLx*6];
				byte c7=Data[ofs+CountryLx*7];
				if(!(c==c1&&c==c2&&c==c3&&c==c4&&c==c5&&c==c6&&c==c7))c=255;
				if(c==prevc&&c!=255){
					CLX++;
					cx1=ix;
				}else{
					if(prevc!=255){
						if(MAXLX[prevc]<CLX){
							MAXLX[prevc]=CLX;
							CX[prevc]=(cx+cx1)>>1;
							CY[prevc]=iy+4;
						};
					};
					prevc=c;
					cx=ix;
					cx1=ix;
					CLX=1;
				};
				ofs++;
			};
		};
		for(i=0;i<256;i++)if(MAXLX[i]){
			XSumm[i]=CX[i];
			YSumm[i]=CY[i];
			NPoints[i]=1;
		};
	};
	UpdateData=0;
	sprintf(ccc,"Internet\\Cash\\%s.date",Title,i);
	F=fopen(ccc,"r");
	if(F){
		fscanf(F,"%u",&UpdateData);
		fclose(F);
		sprintf(ccc,"GW|mupdate&%s\\00&%d\\00",Title,UpdateData);
		SendSmartRequest(SXP,ccc);
	};
	sprintf(ccc,"Internet\\Cash\\%s.nations",Title,i);
	F=fopen(ccc,"r");
	memset(Names,0,sizeof Names);
	if(F){
		char ccc[256];
		int p=0;
		while(ReadWinStringF(F,ccc,255)){
			Names[p]=(char*)malloc(strlen(ccc)+1);
			strcpy(Names[p],ccc);
			p++;
		};
	};
};
#define VTYPE4
void OneMap::ApplyColorsToScaleLayer(int Idx){
	if(!CountryMap)return;
	if(Idx>=0&&Idx<NScales){
		OneScale* OSC=SCALE+Idx;
		if(!OSC->ColoredData){
			OSC->ColoredData=(byte*)malloc(OSC->Lx*OSC->Ly*4);
			int SCDX=(CountryLx*65536)/OSC->Lx;
			int SCDY=(CountryLy*65536)/OSC->Ly;
			int SCDX2=SCDX*2;
			int SCDY2=SCDY*2;
			int SCDX3=SCDX*3;
			int SCDY3=SCDY*3;
			int SCDX4=SCDX*4;
			int SCDY4=SCDY*4;
			int NX=OSC->Lx;
			int NY=OSC->Ly;
			byte* CMAP=CountryMap;
			byte* CDT=OSC->ColoredData;
			byte* SData=OSC->Data;
			byte* PAL=OSC->PAL;

			int CLX=CountryLx;
			int CLY=CountryLy;
			int RY=0;
			int ofs=0;
			int ofs2=0;
			for(int iy=0;iy<NY;iy++){
				int RX=0;
				int RRY=RY>>16;
				for(int ix=0;ix<NX;ix++){
					int mc=SData[ofs];
					int mc3=mc*3;
					int RS=PAL[mc3  ];
					int GS=PAL[mc3+1];
					int BS=PAL[mc3+2];
					int RRX=RX>>16;
					int MOFF=RRX+RRY*CLX;
					byte CC=CMAP[MOFF];					
					int DR,DG,DB;
					if(CC==255){
                        DR=RS;                        
						DG=GS;
						DB=BS;
					}else{
						int RD=COLORS[CC].r;
						int GD=COLORS[CC].g;
						int BD=COLORS[CC].b;
						DR=(RS+RD)>>1;                        
						DG=(GS+GD)>>1;                        
						DB=(BS+BD)>>1;                        
					}
					*((DWORD*)(CDT+ofs2))=(DR)+((DG)<<8)+((DB)<<16)+0xFF000000;
					ofs++;
					ofs2+=4;
					RX+=SCDX;
				};
				RY+=SCDY;
			};
		};
	};
};
CIMPORT void ShowRGBBuff(byte* Buf,int x,int y,int Lx,int Ly,int SizeX,int xs,int ys);
void OneMap::ShowMapPart(){
	if(CurScale<0||CurScale>=NScales)return;
	OneScale* OSC=SCALE+CurScale;
	OneScale* MOSC=SCALE+NScales-1;
	if(!OSC->ColoredData)ApplyColorsToScaleLayer(CurScale);
	if(!OSC->ColoredData)return;
	NormalizeCoordinates();
	int SX=WX;
	int SY=WY;
	int SX1=WX+CurLx-1;
	int SY1=WY+CurLy-1;
	int X0=CurX;
	int Y0=CurY;
	int X1=CurX+CurLx-1;
	int Y1=CurY+CurLy-1;

#ifdef _COSSACKS2
	int WindX, WindY, WindX1, WindY1;
	g_GetWindParam( WindX, WindY, WindX1, WindY1 );
#endif // _COSSACKS2

	if(SX<WindX){
		X0+=WindX-SX;
		SX=WindX;
	};
	if(SY<WindY){
		Y0+=WindY-SY;
		SY=WindY;
	};
	if(SX1>WindX1){
		X1-=SX1-WindX1;
		SX1=WindX1;
	};
	if(SY1>WindY1){
		Y1-=SY1-WindY1;
		SY1=WindY1;
	};

	if(SX>SX1||SY>SY1)return;
	ShowRGBBuff(OSC->ColoredData,CurX,CurY,CurLx,CurLy,OSC->Lx,SX,SY);
	
	int SCAL=OSC->Lx*100/MOSC->Lx;
	int LX=SCALE[CurScale].Lx;
	int LY=SCALE[CurScale].Ly;
	TempWindow TW;
	PushWindow(&TW);
	IntersectWindows(WX,WY,WX+WLX-1,WY+WLY-1);
	for(int i=0;i<NMapPix;i++){
		OneMapPicture* OMP=MapPix+i;
		int x=WX+(OMP->x*LX/CountryLx)-CurX;
		int y=WY+(OMP->y*LY/CountryLy)-CurY;
		int BID=-1;
		int DS=100;
		for(int j=0;j<OMP->NScales;j++){
			int D=abs(OMP->SCALE[j]-SCAL);
			if(D<DS){
				DS=D;
				BID=OMP->GPID[j];
			};
		};
		OMP->Drawn=0;
		if(BID!=-1){
			GPS.ShowGP(x-OMP->dx,y-OMP->dy,BID,OMP->Sprite,0);
			OMP->Drawn=1;
			OMP->LastX=x-OMP->dx;
			OMP->LastY=y-OMP->dy;
			OMP->LastGPID=BID;
		};
	};
	//show text
	//RLCFont* FNT=SCALE[CurScale].Font; 
	RLCFont* FNT=&BlackFont; //new 8.12
	if(FNT){
		int H=GetRLCHeight(FNT->RLC,'W');
		for(int i=0;i<255;i++)if(NPoints[i]&&Names[i]){
			int L=GetRLCStrWidth(Names[i],FNT);
			int x=WX+(XSumm[i]*LX/CountryLx)-CurX;
			int y=WY+(YSumm[i]*LY/CountryLy)-CurY;
			ShowString(x-(L/2),y-(H/2),Names[i],FNT);
		};
	};
	PopWindow(&TW);
};
void OneMap::SetCenterCoor(int x,int y){
	VCenterX=x;
	VCenterY=y;
	NormalizeCoordinates();
};
void OneMap::RelativeMove(int dx,int dy){
	if(!NScales)return;
	OneScale* CUR=SCALE+CurScale;
	OneScale* MAX=SCALE+NScales-1;
	VCenterX+=(MAX->Lx*dx)/CUR->Lx;
	VCenterY+=(MAX->Ly*dy)/CUR->Lx;
	NormalizeCoordinates();
};
void OneMap::ChangeScale(int Direction){
	if(!NScales)return;
	CurScale+=Direction;
	NormalizeCoordinates();
};
void OneMap::NormalizeCoordinates(){
	if(!NScales)return;
	if(CurScale>=NScales)CurScale=NScales-1;
	if(CurScale<0)CurScale=0;
	OneScale* MAX=SCALE+NScales-1;
	OneScale* OSC=SCALE+CurScale;
	CurX=(VCenterX*OSC->Lx/MAX->Lx)-WLX/2;
	CurY=(VCenterY*OSC->Ly/MAX->Ly)-WLY/2;
	if(CurX<0){
		CurX=0;
		VCenterX=WLX*MAX->Lx/OSC->Lx/2;
	};
	if(CurY<0){
		CurY=0;
		VCenterY=WLY*MAX->Ly/OSC->Ly/2;
	};
	CurLx=WLX;
	CurLy=WLY;
	if(CurX+CurLx>=OSC->Lx){
		CurX=OSC->Lx-CurLx;
		if(CurX<0){
			CurX=0;
			CurLx=OSC->Lx;
		};
		VCenterX=(CurX+WLX/2)*MAX->Lx/OSC->Lx;
	};
	if(CurY+CurLy>=OSC->Ly){
		CurY=OSC->Ly-CurLy;
		if(CurY<0){
			CurY=0;
			CurLy=OSC->Ly;
		};
		VCenterY=(CurY+WLY/2)*MAX->Ly/OSC->Ly;
	};
};
void OneMap::ClearAll(){
	for(int i=0;i<NScales;i++){
		OneScale* OSC=SCALE+i;
		if(OSC->Data)free(OSC->Data);
		if(OSC->ColoredData)free(OSC->ColoredData);
	};
	if(SCALE)free(SCALE);
	if(MapPix)free(MapPix);
	if(CountryMap)free(CountryMap);
	for(int i=0;i<255;i++)if(Names[i])free(Names[i]);
	memset(this,0,sizeof *this);
};
OneMap::~OneMap(){
	ClearAll();
};
//LW_mapdata&bindata
//bindata:
//DWORD StartUpdateData
//DWORD CurrentUpdateData
//DWORD NPoints
//for every point:
//WORD  x
//WORD  y
//BYTE  color 
int NPT=0;
int NUP=0;
void OneMap::UpdateMapData(byte* Date){
	NUP++;
	DWORD StartDate=*((DWORD*)Date);
	Date+=4;
	DWORD CurrentDate=*((DWORD*)Date);
	Date+=4;
	//FILE* F1=fopen("Internet\\Cash\\map.log","a");
	//fprintf(F1,"UPDATE: %d->%d (TOTAL: pts: %d  upd: %d)\n",StartDate,CurrentDate,NPT,NUP);
	if(UpdateData==StartDate){
		char ccc[256];
		sprintf(ccc,"Internet\\Cash\\%s.date",Title);
		FILE* F=fopen(ccc,"w");
		if(F){
			fprintf(F,"%u",CurrentDate);
			fclose(F);
			UpdateData=CurrentDate;
			int NPt=*((DWORD*)Date);
			Date+=4;
			int i;
			for(i=0;i<NPt;i++){
				int x=*((word*)Date);
				Date+=2;
				int y=*((word*)Date);
				Date+=2;
				byte c=*Date;
				Date++;
				if(x<CountryLx&&y<CountryLy){
					CountryMap[x+y*CountryLx]=c;
				};
				NPT++;
				//fprintf(F1,"x=%4d y=%4d c=%4d\n",x,y,c);
			};
			sprintf(ccc,"Internet\\Cash\\%s_nations.bmp",Title,i);
			SaveToBMP8(ccc,CountryLx,CountryLy,CountryMap,COLORS);
		};
		//fclose(F1);
		for(int i=0;i<NScales;i++){
			if(SCALE[i].ColoredData)free(SCALE[i].ColoredData);
			SCALE[i].ColoredData=NULL;
			SCALE[i].Colored=0;
		};
	};
};
OneMap BIGMAP;
void OneMap::AddMapPicture(char* Name,int x,int y,int dx,int dy,int Sprite){
	if(NMapPix>=MaxMapPix){
		MaxMapPix+=16;
		MapPix=(OneMapPicture*)realloc(MapPix,MaxMapPix*sizeof OneMapPicture);
	};
	OneMapPicture* OMP=MapPix+NMapPix;
	memset(OMP,0,sizeof OneMapPicture);
	OMP->x=x;
	OMP->y=y;
	OMP->dx=dx;
	OMP->dy=dy;
	OMP->Sprite=Sprite;
	int NF=0;
	char cc[128];
	sprintf(cc,"Internet\\Cash\\%s_*.gp",Name);
	int L=strlen(Name);
	WIN32_FIND_DATA FD;
	HANDLE H=FindFirstFile(cc,&FD);
	if(H!=INVALID_HANDLE_VALUE){
		do{
			sprintf(cc,"Internet\\Cash\\%s",FD.cFileName);
			cc[strlen(cc)-3]=0;
			int scale=atoi(FD.cFileName+L+1);
			int GPI=GPS.PreLoadGPImage(cc);
			if(GPI!=-1){
				GPS.LoadGP(GPI);
				if(Sprite<GPS.GPNFrames(GPI)){
					OMP->SCALE[NF]=scale;
					OMP->GPID[NF]=GPI;
					NF++;
					OMP->NScales=NF;
				};
			};
		}while(FindNextFile(H,&FD)&&NF<15);
	};
	if(NF)NMapPix++;
};
void OneMap::ClearMapPictures(){
	NMapPix=0;
};