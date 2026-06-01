#include "stdheader.h"
OneBar* OBARS[8192];
int NBars;
word** Obj3Map = NULL;
word*  NObj3 = NULL;
void InitObjs3(){
	memset(OBARS,0,sizeof OBARS);
	if (Obj3Map) memset(Obj3Map,0,B3SZ*4);
	if (NObj3)	 memset(NObj3,0,B3SZ*2);
	NBars=0;
};
void ClearObjs3(){
	for(int i=0;i<NBars;i++){
		if(OBARS[i]){
			delete(OBARS[i]);
			OBARS[i]=NULL;
		};
	};
	for(int i=0;i<B3SZ;i++){
		if(Obj3Map[i]){
			free(Obj3Map[i]);
		};
	};
	InitObjs3();
};
void AddObj3(int CX,int CY,word BarID){
	if(CX<0||CY<0||CX>=B3SX||CY>=B3SY)return;
	int Cell=CX+(CY*B3SX);
	word* CEMAP=Obj3Map[Cell];
	if(CEMAP){
		int NCE=NObj3[Cell];
		for(int j=0;j<NCE;j++){
			if(CEMAP[j]==BarID)return;
		};
	};
	Obj3Map[Cell]=(word*)realloc(Obj3Map[Cell],(NObj3[Cell]+1)<<1);
	Obj3Map[Cell][NObj3[Cell]]=BarID;
	NObj3[Cell]++;
	OneBar* ONB=OBARS[BarID];
	if(ONB){
		if(CX<ONB->MinBX)ONB->MinBX=CX;
		if(CX>ONB->MaxBX)ONB->MaxBX=CX;
		if(CY<ONB->MinBY)ONB->MinBY=CY;
		if(CY>ONB->MaxBY)ONB->MaxBY=CY;
	};
};
void DelObj3(int CX,int CY,word BarID){
	if(CX<0||CY<0||CX>=B3SX||CY>=B3SY)return;
	int Cell=CX+(CY*B3SX);
	word* CEMAP=Obj3Map[Cell];
	if(CEMAP){
STARTA:;
		int NCE=NObj3[Cell];
		for(int j=0;j<NCE;j++){
			if(CEMAP[j]==BarID){
				if(j<NCE-1)memcpy(CEMAP+j,CEMAP+j+1,(NCE-j-1)<<1);
				NObj3[Cell]--;
				goto STARTA;
			};
		};
		if(Obj3Map[Cell]&&!NObj3[Cell]){
			delete(Obj3Map[Cell]);
			Obj3Map[Cell]=NULL;
		};
	};
};
word Add3DBar(int X0,int Y0,int X1,int Y1,short H,int ID,word Owner){
	word BarID=0xFFFF;
	for(int i=0;i<NBars&&BarID==0xFFFF;i++){
		if(!OBARS[i])BarID=i;
	};
	if(BarID==0xFFFF){
		if(NBars>=8192)return 0xFFFF;
		BarID=NBars;
		NBars++;
	};
	OneBar* ONB=new OneBar;
	OBARS[BarID]=ONB;
	ONB->MaxBX=-1000;
	ONB->MaxBY=-1000;
	ONB->MinBX=1000;
	ONB->MinBY=1000;
	ONB->H=H;
	ONB->x0=X0-Y0;
	ONB->y0=Y0+X0;
	ONB->x1=X1-Y1;
	ONB->y1=Y1+X1;
	ONB->Owner=Owner;
	ONB->DX=0;
	ONB->DY=0;
	ONB->Protection=0;
	OneObject* OB=Group[Owner];
	if(OB&&OB->newMons->Directional3DBarsEffect){
        ONB->Protection=OB->newMons->Directional3DBarsEffect;
		int xx=(X0+Y1)/2;
		int yy=(Y0+Y1)/2;
		ONB->DX=xx-(OB->RealX>>4);
		ONB->DY=yy-(OB->RealY>>4);
		int N=Norma(ONB->DX,ONB->DY);
		ONB->DX=(ONB->DX*16)/N;
		ONB->DY=(ONB->DY*16)/N;
	}
	int D2=(Y1-Y0+X1-X0)>>1;
	int D1=(X1-X0-Y1+Y0)>>1;
	int D12=D1>>1;
	int D22=D2>>1;
	AddObj3(X0>>B3SHIFT,Y0>>B3SHIFT,BarID);
	AddObj3(X1>>B3SHIFT,Y1>>B3SHIFT,BarID);
	AddObj3((X0+D1)>>B3SHIFT,(Y0-D1)>>B3SHIFT,BarID);
	AddObj3((X0+D2)>>B3SHIFT,(Y0+D2)>>B3SHIFT,BarID);
	AddObj3((X0+X1)>>(B3SHIFT+1),(Y0+Y1)>>(B3SHIFT+1),BarID);
	AddObj3((X0+D12)>>B3SHIFT,(Y0-D12)>>B3SHIFT,BarID);
	AddObj3((X1-D12)>>B3SHIFT,(Y1-D12)>>B3SHIFT,BarID);
	AddObj3((X0+D22)>>B3SHIFT,(Y0+D22)>>B3SHIFT,BarID);
	AddObj3((X0-D12)>>B3SHIFT,(Y1+D12)>>B3SHIFT,BarID);
	if(ID==0xFFFF)ID=BarID;
	ONB->ID=ID;
	return BarID;
};
void CheckBars3D();
void CureBars3D(){
	for(int x=0;x<B3SX;x++)
		for(int y=0;y<B3SY;y++){
			int Cell=x+(y*B3SX);
			word NBR=NObj3[Cell];
			if(NBR){
				int xx=x-y;
				int yy=y+x;
				word* CMAP=Obj3Map[Cell];
				for(int i=0;i<NBR;i++){
					if(!OBARS[CMAP[i]]){
						DelObj3(x,y,CMAP[i]);
					};
				};
			};
		};
};
void Delete3DBar(word ID){
	//CheckBars3D();
	//assert(ID!=11);
	for(int i=0;i<NBars;i++){
		OneBar* ONB=OBARS[i];
		if(ONB&&ONB->ID==ID){
			OBARS[i]=NULL;
			for(int ix=ONB->MinBX;ix<=ONB->MaxBX;ix++)
				for(int iy=ONB->MinBY;iy<=ONB->MaxBY;iy++){
					DelObj3(ix,iy,i);
				};
			delete(ONB);
		};
	};
	CureBars3D();
	CheckBars3D();
};
word OWNER;
int GetBar3DHeight(int x,int y){
	int CX=x>>B3SHIFT;
	int CY=y>>B3SHIFT;
	int MaxH=0;
	OWNER=0xFFFF;
	if(CX>=0&&CX<B3SX&&CY>=0&&CY<B3SY){
		int Cell=CX+(CY*B3SX);
		word NBR=NObj3[Cell];
		if(NBR){
			int xx=x-y;
			int yy=y+x;
			word* CMAP=Obj3Map[Cell];
			for(int i=0;i<NBR;i++){
				OneBar* ONB=OBARS[CMAP[i]];
				if(xx>=ONB->x0&&xx<=ONB->x1&&yy>=ONB->y0&&yy<=ONB->y1&&!ONB->Protection){
					if(MaxH<ONB->H){
						word OWN=ONB->Owner;
						if(OWN!=0xFFFF&&Group[OWN]){
							MaxH=ONB->H;
							OWNER=OWN;
						};
					};
				};
			};
		};
	};
	return MaxH;
};
int GetBar3DHeightEx(int x,int y,int dx,int dy,int rf){
	int CX=x>>B3SHIFT;
	int CY=y>>B3SHIFT;
	int MaxH=0;
	OWNER=0xFFFF;
	OneBar* BBR=NULL;
	if(CX>=0&&CX<B3SX&&CY>=0&&CY<B3SY){
		int Cell=CX+(CY*B3SX);
		word NBR=NObj3[Cell];
		if(NBR){
			int xx=x-y;
			int yy=y+x;
			word* CMAP=Obj3Map[Cell];
			for(int i=0;i<NBR;i++){
				OneBar* ONB=OBARS[CMAP[i]];
				if(xx>=ONB->x0&&xx<=ONB->x1&&yy>=ONB->y0&&yy<=ONB->y1){
					if(MaxH<ONB->H){
						word OWN=ONB->Owner;
						if(OWN!=0xFFFF&&Group[OWN]){
							MaxH=ONB->H;
							OWNER=OWN;
							BBR=ONB;
						};
					};
				};
			};
		};
	};
	if(BBR&&BBR->Protection){
		int N0=Norma(dx,dy);
		if(N0){
			dx=dx*16/N0;
			dy=dy*16/N0;
			int sc=dx*BBR->DX+dy*BBR->DY;
			if(sc<0){
				if(rf){
					if((rf%100)>BBR->Protection){
						MaxH=0;
					}
				}else{
					if((int(rando()*100)>>15)>BBR->Protection){
						MaxH=0;
					}
				}
			}else MaxH=0;
		}
	}
	return MaxH;
};

void CheckBars3D(){
	for(int x=0;x<B3SX;x++)
		for(int y=0;y<B3SY;y++){
			int Cell=x+(y*B3SX);
			word NBR=NObj3[Cell];
			if(NBR){
				int xx=x-y;
				int yy=y+x;
				word* CMAP=Obj3Map[Cell];
				for(int i=0;i<NBR;i++){
//					assert(OBARS[CMAP[i]]);
				};
			};
		};
};
int GetBar3DOwner(int x,int y){
	int CX=x>>B3SHIFT;
	int CY=y>>B3SHIFT;
	word Owner=0xFFFF;
	if(CX>=0&&CX<B3SX&&CY>=0&&CY<B3SY){
		int Cell=CX+(CY*B3SX);
		word NBR=NObj3[Cell];
		int MaxH=0;
		if(NBR){
			int xx=x-y;
			int yy=y+x;
			word* CMAP=Obj3Map[Cell];
			for(int i=0;i<NBR;i++){
				OneBar* ONB=OBARS[CMAP[i]];
				if(xx>=ONB->x0&&xx<=ONB->x1&&yy>=ONB->y0&&yy<=ONB->y1){
					if(MaxH<ONB->H){
						MaxH=ONB->H;
						Owner=ONB->Owner;
					};
				};
			};
		};
	};
	if(Owner!=0xFFFF){
		OneObject* OB=Group[Owner];
		if(!OB)return 0xFFFF;
	};
	return Owner;
};
void CBar(int x,int y,int Lx,int Ly,byte c);
CEXPORT
void LineRGB(int x,int y,int x1,int y1,DWORD Color){
#ifdef _USE3D
	GPS.DrawLine( x, y, x1, y1, Color);
#endif
};
CEXPORT
void xLine(int x,int y,int x1,int y1,byte c){
#ifdef _USE3D
	extern PALETTEENTRY GPal[256];
	GPS.DrawLine( x, y, x1, y1, 
					ColorValue::ToARGB( 0xFF, GPal[c].peRed, GPal[c].peGreen, GPal[c].peBlue )  );
	return;
#endif //_USE3D
	int DD=int(sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1)));
	if(!DD)return;
	int NP=DD+1;
	for(int i=0;i<=NP;i++){
		CBar(x+div((x1-x)*i,NP).quot,y+div((y1-y)*i,NP).quot,1,1,c);
	};
};
extern int RealLx;
extern int RealLy;
void DrawBar3D(OneBar* ONB){
	int x0=(ONB->x0+ONB->y0)>>1;
	int y0=(ONB->y0-ONB->x0)>>1;
	int x1=(ONB->x1+ONB->y1)>>1;
	int y1=(ONB->y1-ONB->x1)>>1;
	int L1=(ONB->x1-ONB->x0)>>1;
	int L2=(ONB->y1-ONB->y0)>>1;
	int xc=(x0+x1)>>1;
	int yc=(y0+y1)>>1;
	int zz=GetTotalHeight(xc,yc);
	int x2=x0+L2;
	int y2=y0+L2;
	int x3=x0+L1;
	int y3=y0-L1;
	int H=ONB->H;
	GPS.DrawLine(Vector3D(x0,y0,zz),Vector3D(x0,y0,zz+H),0xFFFFFFFF);
	GPS.DrawLine(Vector3D(x2,y2,zz),Vector3D(x2,y2,zz+H),0xFFFFFFFF);
	GPS.DrawLine(Vector3D(x1,y1,zz),Vector3D(x1,y1,zz+H),0xFFFFFFFF);	
	GPS.DrawLine(Vector3D(x0,y0,zz),Vector3D(x2,y2,zz),0xFFFFFFFF);	
	GPS.DrawLine(Vector3D(x1,y1,zz),Vector3D(x2,y2,zz),0xFFFFFFFF);	
	GPS.DrawLine(Vector3D(x0,y0,zz+H),Vector3D(x2,y2,zz+H),0xFFFFFFFF);	
	GPS.DrawLine(Vector3D(x1,y1,zz+H),Vector3D(x2,y2,zz+H),0xFFFFFFFF);	
	GPS.DrawLine(Vector3D(x0,y0,zz+H),Vector3D(x3,y3,zz+H),0xFFFFFFFF);	
	GPS.DrawLine(Vector3D(x1,y1,zz+H),Vector3D(x3,y3,zz+H),0xFFFFFFFF);	
};
void ShowAllBars(){
	for(int i=0;i<NBars;i++){
		OneBar* ONB=OBARS[i];
		if(ONB){
			DrawBar3D(ONB);
		};
	};
};