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
#include "Curve.h"
#include <crtdbg.h>
#include "ClassEditor.h"
#include "GameSettings.h"
#include "IWater.h"
#include "ITerrain.h"
#include "CurrentMapOptions.h"
void CheckGP();
extern BlockBars LockBars;
extern BlockBars UnLockBars;
extern BlockBars HorceLockBars;
extern int TM_Angle0;
extern int TM_Angle1;
extern int TM_Height0;
extern int TM_Height1;
extern bool TexMapMod;
extern int TM_Tan0;
extern int TM_Tan1;
int Prop43(int y);
word GetTexture();
void SetTexture(int Vert,int nm);
//returns planar y from(screen coordinates (x,y)
extern int mul3(int);
void AddLockBar(word x,word y){
	if(GetKeyState(VK_CONTROL)&0x8000){
		HorceLockBars.Add(x,y);
		MFIELDS[3].BSetBar(x<<2,y<<2,4);
	}else{
		LockBars.Add(x,y);
		BSetBar(x<<2,y<<2,4);
	}
	UnLockBars.Delete(x,y);	
};
void AddLockFiller(word x,word y){
	void CloseHoleFromPoint(int xc,int yc,int MaximalSquare,bool);
	CloseHoleFromPoint(x,y,1000,GetKeyState(VK_CONTROL)&0x8000);
}
void AddLockRound(word x,word y,int R){
	if(R<1)R=1;
	for(int i=0;i<R&&i<40;i++){
		int N=Rarr[i].N;
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		for(int j=0;j<N;j++){
			AddLockBar(x+xi[j],y+yi[j]);
		}
	}	
};
void FastAddLockBar(word x,word y){
	LockBars.FastAdd(x,y);
	//UnLockBars.Delete(x,y);
	//BSetBar(x<<2,y<<2,4);
};
void FastAddUnLockBar(word x,word y){
	//LockBars.FastAdd(x,y);
	UnLockBars.FastAdd(x,y);
	//UnLockBars.Delete(x,y);
	//BSetBar(x<<2,y<<2,4);
};
void AddUnLockbar(word x,word y){
	LockBars.Delete(x,y);
	HorceLockBars.Delete(x,y);
	UnLockBars.Add(x,y);
	BClrBar(x<<2,y<<2,4);
};
void AddUnLockRound(word x,word y,int R){
	if(R<1)R=1;
	for(int i=0;i<R&&i<40;i++){
		int N=Rarr[i].N;
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		for(int j=0;j<N;j++){
			AddUnLockbar(x+xi[j],y+yi[j]);
		}
	}	
};
void AddDefaultBar(word x,word y){
	LockBars.Delete(x,y);
	HorceLockBars.Delete(x,y);
	UnLockBars.Delete(x,y);
};
void AddDefaultRound(word x,word y,int R){
	if(R<1)R=1;
	for(int i=0;i<R&&i<40;i++){
		int N=Rarr[i].N;
		char* xi=Rarr[i].xi;
		char* yi=Rarr[i].yi;
		for(int j=0;j<N;j++){
			AddDefaultBar(x+xi[j],y+yi[j]);
		}
	}	
};
int GetTerrAngle(int x,int y);
int GetTotalHeight(int,int);
int GetPSY(int i){
	int x=(i%VertInLine)<<5;
	int y=((i/VertInLine)<<5)-(x&1?16:0);
	int H=GetTotalHeight(x,y);//THMap[i];
	if(H<0)H=0;
	return (mul3(GetTriY(i))>>2)-H;
};
int GetPLY(int i){
	return (mul3(GetTriY(i))>>2);
};
int ConvScrY(int x,int y){
	int uu=x/(TriUnit+TriUnit);
	int vertex=uu+VertInLine*(MaxTH-1);
	int v1=((y/TriUnit)+50)*VertInLine+uu;
	if(v1<vertex)vertex=v1;
	int besty=-10000;
	int yy2=0;
	while(vertex>=0){
		if(vertex<MaxPointIndex-1){
			int xp1=GetTriX(vertex);
			int yp1=GetPSY(vertex);
			int xp2=GetTriX(vertex+1);
			int yp2=GetPSY(vertex+1);
			int yy1=yp1+div((yp2-yp1)*(x-xp1),xp2-xp1).quot;
			if(yy1<y){
				if(vertex+VertInLine<MaxPointIndex-1){
					yp1=GetPLY(vertex);
					yp2=GetPLY(vertex+1);
					besty=yp1+div((yp2-yp1)*(x-xp1),xp2-xp1).quot;
					if(yy1==yy2)return besty;
					else return besty+div((TriUnit)*(y-yy1),yy2-yy1).quot;
				}else return -10000;
			};
			vertex-=VertInLine;
			yy2=yy1;
		}else return -10000;
	};
	return -10000;
};
struct SelectionRequest{
	int x,y,r;
	byte TileType;
	byte BrushType;
};
int Norm(int dx,int dy){
	return int(sqrt(dx*dx+dy*dy));
};
void MarkTriangleToDraw(int i){
};
void MakeDirtyPiece(int x,int y);
void ReportVertexChange(int v);
void ReportVUpdate();
bool LockMarkPoint=0;
void MarkPointToDraw(int i){
	if(LockMarkPoint)return;
	if(i<0||i>(MaxTH+1)*MaxTH)return;
#ifdef _USE3D
	ReportVUpdate();
#endif
	ReportVertexChange(i);
	div_t uu=div(i,VertInLine);
	MakeDirtyPiece(GetTriX(i),GetTriY(i));
	int Sector=uu.quot*(MaxTH+MaxTH)+((uu.rem>>1)<<2);
	if(uu.rem&1){
		MarkTriangleToDraw(Sector-MaxTH-MaxTH);
		MarkTriangleToDraw(Sector-MaxTH-MaxTH+1);
		MarkTriangleToDraw(Sector-MaxTH-MaxTH+2);
		MarkTriangleToDraw(Sector-MaxTH-MaxTH+3);
		MarkTriangleToDraw(Sector+1);
		MarkTriangleToDraw(Sector+2);
	}else{
		//Sector-=4;
		MarkTriangleToDraw(Sector);
		MarkTriangleToDraw(Sector+1);
		MarkTriangleToDraw(Sector-1);
		MarkTriangleToDraw(Sector-2);
		MarkTriangleToDraw(Sector-MaxTH-MaxTH);
		MarkTriangleToDraw(Sector-MaxTH-MaxTH-1);
	};
};
void MarkLineToDraw(int i){
	div_t uu=div(i,6);
	int LineType=uu.rem;
	int StartTri=uu.quot<<2;
	switch(LineType){
	case 0:
		MarkTriangleToDraw(StartTri);
		MarkTriangleToDraw(StartTri-1);
		break;
	case 1:
		MarkTriangleToDraw(StartTri+1);
		MarkTriangleToDraw(StartTri+MaxTH+MaxTH);
		break;
	case 2:
		MarkTriangleToDraw(StartTri);
		MarkTriangleToDraw(StartTri+1);
		break;
	case 3:
		MarkTriangleToDraw(StartTri+1);
		MarkTriangleToDraw(StartTri+2);
		break;
	case 4:
		MarkTriangleToDraw(StartTri+2);
		MarkTriangleToDraw(StartTri+MaxTH+MaxTH+3);
		break;
	case 5:
		MarkTriangleToDraw(StartTri+2);
		MarkTriangleToDraw(StartTri+3);
		break;
	};
};
//returns division of line by cicle in CurDiv
void GetDivPoint(int x1,int y1,int x2,int y2,byte* CurDiv,SelectionRequest* SR){
	int rd=SR->r;
	int cdv=*CurDiv;
	int r0=Norm(x1-SR->x,y1-SR->y);
	int r1=Norm(x2-SR->x,y2-SR->y);
	if((r0>rd&&r1>rd)||(r0<rd&&r1<rd)){
		if(r0<rd&&r1<rd)*CurDiv=div(rand(),11000).quot;
		return;
	};
	int xp0=x1+((x2-x1)>>2);
	int yp0=y1+((y2-y1)>>2);
	int xp1=x1+((x2-x1)>>1);
	int yp1=y1+((y2-y1)>>1);
	int xp2=x2-((x2-x1)>>2);
	int yp2=y2-((y2-y1)>>2);
	int rp[3];
	rp[0]=Norm(xp0-SR->x,yp0-SR->y);	
	rp[1]=Norm(xp1-SR->x,yp1-SR->y);	
	rp[2]=Norm(xp2-SR->x,yp2-SR->y);	
	if(r0<rd){
		if(rp[cdv]>rd)return;
		if(rp[0]<rd)cdv=0;
		if(rp[1]<rd)cdv=1;
		if(rp[2]<rd)cdv=2;
		*CurDiv=cdv;
	};
	if(r1<rd){
		if(rp[cdv]>rd)return;
		if(rp[2]<rd)cdv=2;
		if(rp[1]<rd)cdv=1;
		if(rp[0]<rd)cdv=0;
		*CurDiv=cdv;
	};
};
void PutTexInPoint(int i,SelectionRequest* SR,int T=-1){
};
void MSetTexture(int tex,bool add);
int GetNearestTexture(int x,int y){
	int VX=x>>5;
	int VY=y>>5;
	int DR=2;
	int VX0=VX-DR;
	int VX1=VX+DR;
	int VY0=VY-DR;
	int VY1=VY+DR;
	if(VX0<0)VX0=0;
	if(VY0<0)VY0=0;
	if(VX1>=VertInLine)VX1=VertInLine-1;
	if(VY1>=MaxTH)VX1=MaxTH-1;
	int CTexID=-1;
	int NR=10000;
	for(int iy=VY0;iy<=VY1;iy++){
		for(int ix=VX0;ix<=VX1;ix++){
			int ofs=ix+iy*VertInLine;
			int xp=ix<<5;
			int yp=iy<<5;
			if(ix&1)yp-=16;
			int R=Norma(xp-x,yp-y);
			if(R<NR){
				NR=R;
				CTexID=TexMap[ofs];
			};
		};
	};
	return CTexID;
};
bool CheckTex(int Vertex,int id){
	if(id==-1)return true;
	int T=TexMap[Vertex];
	if(T!=id)return false;
	int vx=Vertex%VertInLine;
	int vy=Vertex/VertInLine;
	int tx=vx<<5;
	int ty=vy<<5;
	int TLU,TLD,TRU,TRD,TU,TD;
	
	if(vx&1){
		ty-=16;
		if(vy>0){
			TU=TexMap[Vertex-VertInLine];
			TLU=TexMap[Vertex-VertInLine-1];
			TRU=TexMap[Vertex-VertInLine+1];
		}else{
			TU=T;
			TLU=T;
			TRU=T;
		};
		TLD=TexMap[Vertex-1];
		TRD=TexMap[Vertex+1];
		TD=TexMap[Vertex+VertInLine];
	}else{
		if(vx>0){
			TLU=TexMap[Vertex-1];
			TLD=TexMap[Vertex+VertInLine-1];
		}else{
			TLU=T;
			TLD=T;
		};
		if(vy>0){
			TU=TexMap[Vertex-VertInLine];
		}else TU=T;
		TRU=TexMap[Vertex+1];
		TRD=TexMap[Vertex+VertInLine+1];
		TD=TexMap[Vertex+VertInLine];
	};
	if(T==TU&&T==TD&&T==TLU&&T==TRU&&T==TLD&&T==TRD)return true;
	else return false;
};
int GrassStart=0;
int GrassEnd=12;
#define TXCH(x) T=(x);if(T>=GrassStart&&T<=GrassEnd){T0+=T;NT++;};
int GetGrass(int ID){
	int v=ID%10;
	int T=ID/10;
	if(rand()%10<v)return T+1;
	else return T;
};
void SetTexturedRound(int x,int y,int rx,byte Tex){
	bool CheckIfNewTerrain(void);
	extern DWORD TXCOLOR[64];
	bool GETTEXMOD=GetKeyState(VK_MENU)&0x8000;
	if(GETTEXMOD)rx=64;
	bool SOFTING=0;
	bool ROMB=0;
#ifdef _USE3D
	SOFTING=GetKeyState(VK_CONTROL)&0x8000;
	ROMB=GetKeyState(VK_SPACE)&0x8000;
#endif //_USE3D
	int r=rx;
	int yy=y;
	//if(Mode3D)yy=ConvScrY(x,y);
	//yy=y;//Prop43(yy);
	int utx=div(x,TriUnit*2).quot;
	int uty=div(yy,TriUnit*2).quot;
	SelectionRequest SR;
	SR.BrushType=ROMB;
	SR.x=x;
	//if(Mode3D)yy=ConvScrY(x,y);
	if(yy<0)return;

	int TexID=-1;
	if(GetKeyState(VK_SCROLL))TexID=GetNearestTexture(x,yy);

	SR.y=yy;
	SR.r=r;
	SR.TileType=Tex;
	r=(r>>4)+4;
	int minr=100000;
	int besttex=0;
	for(int tx=-r;tx<r;tx++){
		for(int ty=-r;ty<r;ty++){
			int vx=utx+tx;
			int vy=uty+ty;
			if(vx>=0&&vx<VertInLine&&vy>0&&vy<MaxTH){
				int vert=vx+vy*VertInLine;
				if(SOFTING){
#ifdef _USE3D
					void PerformTexSmoothingInPoint(int vx,int vy,int x0,int y0,int r);
					PerformTexSmoothingInPoint(vx,vy,x,yy,SR.r);
#else
					int T0=0;
					int NT=0;
					int T;
					TXCH(TexMap[vert]);
					if(NT){
						int vx=vert%VertInLine;
						int vy=vert/VertInLine;
						if(vx>0&&vy>0&&vx<VertInLine-1&&vy<MaxTH-1){
							TXCH(TexMap[vert-VertInLine]);
							TXCH(TexMap[vert+VertInLine]);
							if(vx&1){
								TXCH(TexMap[vert-VertInLine-1]);
								TXCH(TexMap[vert-VertInLine+1]);
								TXCH(TexMap[vert-1]);
								TXCH(TexMap[vert+1]);
							}else{
								TXCH(TexMap[vert+VertInLine-1]);
								TXCH(TexMap[vert+VertInLine+1]);
								TXCH(TexMap[vert-1]);
								TXCH(TexMap[vert+1]);
							};
							T0=T0*10/NT;
							PutTexInPoint(vert,&SR,GetGrass(T0));
						};
					};
#endif //_USE3D
				}else
				if(GETTEXMOD){
					int tx=GetTriX(vert);
					int ty=(GetTriY(vert)>>1)-THMap[vert];
					int r=Norma(tx-x,ty-y);
					if(r<minr){
						minr=r;
						besttex=TexMap[vert];
					};
				}else{
					if(TexID==-1||TexID==TexMap[vert])PutTexInPoint(vert,&SR);
				};
			};
		};
	};
	if(GETTEXMOD&&minr<100000){
		MSetTexture(besttex,GetKeyState(VK_CONTROL)&0x8000);
	};
};
//extern byte darkfog[40960];
void SetTexture(int Vert,int nm){
	if(Vert<0||Vert>=MaxPointIndex)return;
	if(TexMapMod){
		int Hi=THMap[Vert];
		if(Hi<TM_Height0||Hi>TM_Height1)return;
		int xp,yp;
		div_t uu=div(Vert,VertInLine);
		if(uu.rem&1){
			yp=uu.quot*(TriUnit+TriUnit)-TriUnit;
		}else{
			yp=uu.quot*(TriUnit+TriUnit);
		};
		xp=uu.rem*(TriUnit+TriUnit);
		int ANG=GetTerrAngle(xp,yp);
		if(ANG<TM_Tan0||ANG>TM_Tan1)return;

	};	
	TexMap[Vert]=nm;
#ifdef _USE3D
	void AssignExTex(int V,byte tex,byte W);
	AssignExTex(Vert,nm,0);
#endif
	word tf=TexFlags[nm];
	void ReportVertexChange(int v);
	ReportVertexChange(Vert);
};
extern bool MiniMade;
extern int WMPSIZE;
extern int MaxWX;
extern int MaxWY;

extern byte* WaterDeep;//cell size=32x16
extern DWORD TXCOLOR[64];
WORD DWORD2WORD(DWORD C){
	word CW;
	__asm{
		mov eax,C
		xor edx,edx

		mov ebx,eax
		and ebx,0xF0
		shr ebx,4
		add edx,ebx

		mov ebx,eax
		and ebx,0xF000
		shr ebx,8
		add edx,ebx

		mov ebx,eax
		and ebx,0xF00000
		shr ebx,12
		add edx,ebx

		mov ebx,eax
		and ebx,0xF0000000
		shr ebx,16
		add edx,ebx

		mov CW,dx
	}
	return CW;
}
int GetMinimapTexture();
extern IRenderSystem* IRS;
word MixWORD(word C1,word C2,int W1,int W2){
	word R=0;
	if(W1<0)W1=0;
	if(W2<0)W2=0;
	if(W1>255)W1=255;
	if(W2>255)W2=255;
	for(int i=0;i<4;i++){
		//R<<=8;
		int CH1=C1&15;
		int CH2=C2&15;
		C1>>=4;
		C2>>=4;
		int CC=(CH1*W1+CH2*W2+128)>>8;
		if(CC>15)CC=15;
		R|=(CC<<(i<<2));
	}
	return R;
}
_inline void MapSpot(int x,int y,int R,word C,word* Data,int pitch,int x0,int y0,int x1,int y1){
	for(int r=0;r<R;r++){
		int N=Rarr[r].N;
		char* xi=Rarr[r].xi;
		char* yi=Rarr[r].yi;
		for(int j=0;j<N;j++){
			int xx=x+xi[j];
			int yy=y+yi[j];
			if(xx>=x0&&yy>=y0&&xx<x1&&yy<y1){
				int dx=xi[j]<<2;
				int dy=yi[j]<<2;
				int N=Norma(dx,dy);				
				int N0=R<<2;
                int W=255*(N0-N)/N0;
				if(W<0)W=0;
				if(W>255)W=255;
				W=(W*W)>>8;
				word* v=(Data+xx+yy*pitch);
				*v=MixWORD(C,*v,W,255-W)|0xF000;
			}
		}
	}
}
void MapLine(int x,int y,int xd,int yd,int R,word C,word* Data,int pitch,int x0,int y0,int x1,int y1){
	int N=Norma(x-xd,y-yd)*6/R;
	if(N<=0)N=1;
	for(int j=0;j<=N;j++){
		MapSpot(x+(xd-x)*j/N,y+(yd-y)*j/N,R,C,Data,pitch,x0,y0,x1,y1);
	}
}
int GetPointToLineDistEx(int x,int y,int x1,int y1,int x2,int y2,int& xn,int& yn,int& Weight);
void MapLine2(int x,int y,int xd,int yd,float fr,word C,word* Data,int pitch,int x0,int y0,int x1,int y1){
	int xL=tmin(x,xd)-fr-3;
	int yL=tmin(y,yd)-fr-3;
	int xR=tmax(x,xd)+fr+3;
	int yR=tmax(y,yd)+fr+3;
	int R=fr*16;
	x<<=4;y<<=4;xd<<=4;yd<<=4;
	for(int ix=xL;ix<=xR;ix++){
		for(int iy=yL;iy<=yR;iy++){
			if(ix>=x0&&iy>=y0&&ix<x1&&iy<y1){				
				int xn,yn,W;
				int r=GetPointToLineDistEx(ix<<4,iy<<4,x,y,xd,yd,xn,yn,W);
				if(r<R){
					W=255*(R-r)/R;
					word* v=(Data+ix+iy*pitch);
					*v=MixWORD(C,*v,W,255-W)|0xF000;
				}
			}
		}
	}
}
void MapLine2A(int x,int y,int xd,int yd,float fr,word C,word* Data,int pitch,int x0,int y0,int x1,int y1){
	int xL=tmin(x,xd)-fr-3;
	int yL=tmin(y,yd)-fr-3;
	int xR=tmax(x,xd)+fr+3;
	int yR=tmax(y,yd)+fr+3;
	int R=fr*16;
	x<<=4;y<<=4;xd<<=4;yd<<=4;
	for(int ix=xL;ix<=xR;ix++){
		for(int iy=yL;iy<=yR;iy++){
			if(ix>=x0&&iy>=y0&&ix<x1&&iy<y1){				
				int xn,yn,W;
				int r=GetPointToLineDistEx(ix<<4,iy<<4,x,y,xd,yd,xn,yn,W);
				if(r<R){
					W=255*(R-r)/R;
					word* v=(Data+ix+iy*pitch);
					*v=MixWORD(C,*v,W,255-W);
				}
			}
		}
	}
}
int GetMinimapRoadsTexture();
bool ShowHeightMode=false;
bool DontShowHeight=false;
void RenderSurfaceColorsToMiniMapPart(WORD* ptr,int Lx,int Ly,int wndX,int wndY,int wndLx,int wndLy);
DynArray<Rct> invRct;
void InvalidateQuadCallback( Rct& rct){
    for(int i=0;i<invRct.GetAmount();i++){
        if(rct.x==invRct[i].x && rct.y==invRct[i].y)return;
    }
    invRct.Add(rct);
}
void RenderSurfaceColorsToMiniMapPart(WORD* ptr,int Lx,int Ly,Rct& rect);
void UpdateMiniMap(){
    static bool HaveCB=false;
    if(!HaveCB){
        HaveCB=true;
        ITerra->SetInvalidateCallback(InvalidateQuadCallback);        
    }
    int tex=GetMinimapTexture();        
    int pitch;
    BYTE* tbits=IRS->LockTexBits(tex,pitch);        
    if(tbits){
        for(int i=0;i<invRct.GetAmount();i++){
            RenderSurfaceColorsToMiniMapPart((WORD*)tbits,pitch/2,pitch/2,invRct[i]);
        }
    }
    invRct.FastClear();
    IRS->UnlockTexBits(tex);
}
void CreateMiniMapPart(int x0,int y0,int x1,int y1,bool MiniMap){
	x0-=5;
	y0-=5;
	x1+=5;
	y1+=5;
	if(x0<0)x0=0;
	if(y0<0)y0=0;
	if(x1>255)x1=255;
	if(y1>255)y1=255;
	int tex=GetMinimapTexture();
	int rtex=GetMinimapRoadsTexture();
	int pitch;
	int rpitch;
	BYTE* tbits=IRS->LockTexBits(tex,pitch);
	BYTE* rbits=IRS->LockTexBits(rtex,rpitch);
	if(tbits&&rbits){
		RenderSurfaceColorsToMiniMapPart((WORD*)tbits,pitch/2,pitch/2,x0,y0,x1-x0,y1-y0);
		/*
		int msx2=msx>>ADDSH;
		int msy2=msy>>ADDSH;
		MiniMade=true;
		//int msx2=msx>>1;
		//int msy2=msy>>1;
		if(x0<0)x0=0;
		if(y0<0)y0=0;
		if(x1>=msx2)x1=msx2-1;
		if(y1>=msy2)y1=msy2-1;				
		int maxofs=y1*pitch;
		for(int ix=x0;ix<=x1;ix++){
			for(int iy=y0;iy<=y1;iy++){	
				if(ShowHeightMode){
					int Vert=VertInLine*(iy+iy)+ix+ix;
					int H=THMap[Vert];
					DWORD C;
					if(H<0){
                        if(H<-255)H=-255;
						H=-H;
						C=MixDWORD(0xFF0000DF,0xFF606090,H,255-H);
					}else{
						if(H<256){
							C=MixDWORD(0xFF00DF00,0xFFDF0000,255-H,H);                            
						}else{
							H-=256;
							if(H>255)H=255;
							C=MixDWORD(0xFFDF0000,0xFFDFDFDF,255-H,H);                            
						}
					}
					//H=8-abs(8-(H&15));
					//C+=H+(H<<8)+(H<<16);
					int yy=iy;
					int tof=(ix<<1)+yy*pitch;
					word* bf=(word*)(tbits+tof);				
					word* rbf=(word*)(rbits+tof);					
					word wc=DWORD2WORD(C);
					if(tof>=0&&tof<maxofs)*(bf)=wc;
				}else{
					int Vert=VertInLine*(iy+iy)+ix+ix;
					int GetLighting3D(int i);
					int Lit=GetLighting3D(Vert);				
					extern byte* TexMapEx;
					extern byte* WTexMapEx;
					int T1=TexMap[Vert];
					int T2=TexMapEx[Vert];
					int W=WTexMapEx[Vert];				
					DWORD C=MixDWORD(TXCOLOR[T1],TXCOLOR[T2],255-W,W);
					if(Lit<128){
						Lit=Lit*2;
						C=MixDWORD(C,0xFF000000,Lit,255-Lit);
					}else{
						Lit=(Lit-128)*2;
						if(Lit>255)Lit=255;
						C=MixDWORD(C,0xFFFFAF80,255-Lit,Lit);
					}
					int H=THMap[Vert];
					if(H<0){
						H=(-H)*5;
						if(H>255)H=255;
						DWORD WC=EngSettings.MiniWaterColor;
						C=MixDWORD(C,WC,255-H,H);
						H=0;
					}
    				int yy=iy-(H>>6);
					int tof=(ix<<1)+yy*pitch;
    				word* bf=(word*)(tbits+tof);				
					word* rbf=(word*)(rbits+tof);
					if(tof>=0&&tof<maxofs)rbf[0]=0;
					word wc=DWORD2WORD(C);
					for(int q=0;q<8;q++){					
						if(tof>=0&&tof<maxofs)*(bf)=wc;
						tof+=512;bf+=256;
					}
				}
			}							
		}
		*/
		int GetNRoads();
		void GetPreciseTopCenter(int Zone,int LockType,int& x,int& y);
		int N=GetNRoads();
		word RC=DWORD2WORD(EngSettings.MiniRoadColor);
		for(int j=0;j<N;j++){
			int xc,yc;

			GetPreciseTopCenter(j,0,xc,yc);
			int GetNRoadsLink(int index);
			int GetKnotLink(int Index,int LinkIndex);

			yc=(yc-GetHeight(xc,yc))>>6;
			xc>>=6;
			if(xc>x0&&yc>y0&&xc<x1&&yc<y1){
				int nrl=GetNRoadsLink(j);
				for(int q=0;q<nrl;q++){
					int k=GetKnotLink(j,q);
					if(k>j){
						int xl,yl;
						GetPreciseTopCenter(k,0,xl,yl);
						yl=(yl-GetHeight(xl,yl))>>6;
						xl>>=6;
						MapLine2A(xc,yc,xl,yl,1.1,RC,(word*)rbits,256,x0,y0,x1,y1);
					}
				}
			}
		}
		if(MiniMap){
			word C16[8];
			DWORD GetNatColor(int i);
			for(int q=0;q<8;q++)C16[q]=DWORD2WORD(GetNatColor(q));
			for(int g=0;g<MAXOBJECT;g++){
				OneObject* OO=Group[g];
				if(OO&&!(OO->Sdoxlo||OO->Invisible||(OO->InvisibleForEnemy))){
					int mxx=(OO->RealX>>(9+ADDSH));
					int myy=(OO->RealY>>(9+ADDSH))-(OO->RZ>>5);
					if(mxx>0&&myy>0&&mxx<240&&myy<240){
						word val=C16[OO->NNUM];						
						word& v=*(((word*)tbits)+mxx+(myy<<8));
						v=val;
						(&v)[-1]=val;
						(&v)[1]=val;
						(&v)[-256]=val;
						(&v)[256]=val;
					}
				}
			}
		}
		IRS->UnlockTexBits(rtex);
		IRS->UnlockTexBits(tex);
	}
};
int IMGMAX;
int ImgLx;
int ImgLy;
byte* ImgDat;

int* PrpX1;
int* PrpMul;

void PutMPPoint(int x,int y,byte c){
	if(x<0||x>=IMGMAX||y<0||y>=IMGMAX)return;
	ImgDat[PrpX1[x]+PrpMul[y]]=c;
};
int srando();
void CreateMapPreview(byte* Data,int Lx,int Ly){
	
};
extern byte* WaterBright;

int GetMinimapTexture(){
	static int MinimapTexture=0;
	if(MinimapTexture==0){
		MinimapTexture=IRS->CreateTexture("MinimapTexture",256,256,cfARGB4444,1,tmpManaged);
	}
	return MinimapTexture;
}
int GetMinimapRoadsTexture(){
	static int MinimapTexture=0;
	if(MinimapTexture==0){
		MinimapTexture=IRS->CreateTexture("MinimapRoadsTexture",256,256,cfARGB4444,1,tmpManaged);
	}
	return MinimapTexture;
}
void MakeDirtyGrassQuad(Rct rct);
void CreateMiniMap(){
	MiniMade=true;
	CreateMiniMapPart(0,0,msx>>1,msy>>1,false);		
	MakeDirtyGrassQuad(Rct(0,0,16384,16384));
};
void CreateMiniMapWithUnits(){
	MiniMade=true;
	CreateMiniMapPart(0,0,msx>>1,msy>>1,true);	
};
void DrawTexturedBar(float xL,float yL,float W,float H,
					 float txL,float tyL,float tW,float tH,
					 DWORD CLU,DWORD CRU,DWORD CLD,DWORD CRD,
                     int TextureID,int ShaderID){
	//IRS->SaveTexture(TextureID,"!!1.dds");
	BaseMesh BM;
 	BM.create(4,6,vfVertexTnL);
	word* idx=BM.getIndices();
	idx[0]=0;
	idx[1]=1;
	idx[2]=2;
	idx[3]=2;
	idx[4]=1;
	idx[5]=3;
	VertexTnL* V=(VertexTnL*)BM.getVertexData();

	V[0].x=xL;
	V[0].y=yL;
	V[0].w=1.0;
	V[0].u=txL;
	V[0].v=tyL;
	V[0].diffuse=CLU;

	V[1].x=xL+W;
	V[1].y=yL;
	V[1].w=1.0;
	V[1].u=txL+tW;
	V[1].v=tyL;
	V[1].diffuse=CRU;

	V[2].x=xL;
	V[2].y=yL+H;
	V[2].w=1.0;
	V[2].u=txL;
	V[2].v=tyL+tH;
	V[2].diffuse=CLD;

	V[3].x=xL+W;
	V[3].y=yL+H;
	V[3].w=1.0;
	V[3].u=txL+tW;
	V[3].v=tyL+tH;
	V[3].diffuse=CRD;

	BM.setNInd(6);
	BM.setNVert(4);
	BM.setNPri(2);
	BM.setShader(ShaderID);
	BM.setTexture(TextureID);
    DrawBM(BM);
}
void DrawTexturedBar(float xL,float yL,float W,float H,
			float txL,float tyL,float tW,float tH,
			DWORD CLU,DWORD CRU,DWORD CLD,DWORD CRD,
			int TextureID,int ShaderID,byte Rot){
	//IRS->SaveTexture(TextureID,"!!1.dds");
	BaseMesh BM;
	BM.create(4,6,vfVertexTnL);
	word* idx=BM.getIndices();
	idx[0]=0;
	idx[1]=1;
	idx[2]=2;
	idx[3]=2;
	idx[4]=1;
	idx[5]=3;
	VertexTnL* V=(VertexTnL*)BM.getVertexData();

	V[0].x=xL;
	V[0].y=yL;
	V[0].w=1.0;
	V[0].u=txL;
	V[0].v=tyL;
	V[0].diffuse=CLU;

	V[1].x=xL+W;
	V[1].y=yL;
	V[1].w=1.0;
	V[1].u=txL+tW;
	V[1].v=tyL;
	V[1].diffuse=CRU;

	V[2].x=xL;
	V[2].y=yL+H;
	V[2].w=1.0;
	V[2].u=txL;
	V[2].v=tyL+tH;
	V[2].diffuse=CLD;

	V[3].x=xL+W;
	V[3].y=yL+H;
	V[3].w=1.0;
	V[3].u=txL+tW;
	V[3].v=tyL+tH;
	V[3].diffuse=CRD;

	float xc=xL+W/2;
	float yc=yL+W/2;

	float dx=TCos[Rot]/256.0f;
	float dy=TSin[Rot]/256.0f;
	float ss=1.0f/(fabs(dx)+fabs(dy));
	dx*=ss;
	dy*=ss;

	for(int i=0;i<4;i++){
		float VDX=V[i].x-xc;
		float VDY=V[i].y-yc;
		V[i].x=xc+VDX*dx-VDY*dy;
		V[i].y=yc+VDX*dy+VDY*dx;
	}

	BM.setNInd(6);
	BM.setNVert(4);
	BM.setNPri(2);
	BM.setShader(ShaderID);
	BM.setTexture(TextureID);
	DrawBM(BM);
}
void CBar(int x,int y,int Lx,int Ly,byte c);
extern int tmtmt;
extern int RealLx;
extern int RealLy;
void ShowPen(int x,int y1,int R,DWORD ptc){
	int sh=5-Shifter;
	SetRLCWindow(0,0,RealLx,RealLy,ScrWidth);
	int y=y1;//Prop43(y1);
	int npt=R>>(1+sh);
	int tm=GetTickCount()&(0xFFFF);
	double angle0=double(tm)/20/R;
	int prx,pry,prz;
	for(int r=R;r>R-8;r-=3){
		for(int i=0;i<=npt;i++){
			double angl=(i*2*3.1415/npt)+angle0;
			int xp=x+int(r*sin(angl));
			int yp=y+int(r*cos(angl));			
			int h=0;
			h=Mode3D?GetTotalHeight(xp,yp):0;
			void DrawWorldLine(float x0,float y0,float z0,float x1,float y1,float z1,DWORD Diffuse);
			if(i){
				DrawWorldLine(prx,pry,prz,xp,yp,h,ptc);
			}		
			prx=xp;
			pry=yp;
			prz=h;
		}	
	}
	GPS.FlushBatches();
};
#ifdef _USE3D
#define cPI 3.1415
void ShowSqPen(int x,int y1,int R,DWORD ptc){
	int sh=5-Shifter;
	SetRLCWindow(0,0,RealLx,RealLy,ScrWidth);
	int y=y1;//Prop43(y1);
	int npt=R>>(1+sh);
	int tm=GetTickCount()&(0xFFFF);
	double angle0=double(tm)/20/R;
	int prx,pry,prz;
	for(int r=R;r>R-8;r-=3){
		for(int i=0;i<=npt;i++){
			double angl=(i*2*3.1415/npt)+angle0;
			double angl2=angl-(int(angl/cPI*2))*cPI/2;
			int RR=r/cos(cPI/4-angl2)/1.4142;
			int xp=x+int(RR*sin(angl));
			int yp=y+int(RR*cos(angl));
			int h=0;
			h=GetTotalHeight(xp,yp);			
			void DrawWorldLine(float x0,float y0,float z0,float x1,float y1,float z1,DWORD Diffuse);
			if(i){
				DrawWorldLine(prx,pry,prz,xp,yp,h,ptc);
			}		
			prx=xp;
			pry=yp;
			prz=h;
		}	
	}
	GPS.FlushBatches();	
};
#endif //_USE3D
void AddHi(int x,int y1,int r,int h){
	int y=Prop43(y1);

	int TexID=-1;
	if(GetKeyState(VK_SCROLL))TexID=GetNearestTexture(x,y);

	int utx=div(x,TriUnit*2).quot;
	int uty=div(y,TriUnit*2).quot;
	int r1=div(r,TriUnit*2).quot+2;
	for(int tx=-r1;tx<r1;tx++)
		for(int ty=-r1;ty<r1;ty++){
			int vx=utx+tx;
			int vy=uty+ty;
			if(vx>=0&&vx<VertInLine&&vy>0){
				int vert=vx+vy*VertInLine;
				int xx=GetTriX(vert);
				int yy=GetTriY(vert);
				int dst=int(sqrt((xx-x)*(xx-x)+(yy-y)*(yy-y)));
				if(dst<r){
					int dh=(h*(r-dst))/r;
					if(CheckTex(vert,TexID))SetHi(vert,GetHi(vert)+dh);
				};
				if(dst<r+TriUnit*3)MarkPointToDraw(vert);
			};
		};
};
void AddHiPlanar(int x,int y1,int r,int h){
	int y=Prop43(y1);

	int TexID=-1;
	if(GetKeyState(VK_SCROLL))TexID=GetNearestTexture(x,y);

	int utx=div(x,TriUnit*2).quot;
	int uty=div(y,TriUnit*2).quot;
	int r1=div(r,TriUnit*2).quot+2;
	for(int tx=-r1;tx<r1;tx++)
		for(int ty=-r1;ty<r1;ty++){
			int vx=utx+tx;
			int vy=uty+ty;
			if(vx>=0&&vx<VertInLine&&vy>0){
				int vert=vx+vy*VertInLine;
				int xx=GetTriX(vert);
				int yy=GetTriY(vert);
				int dst=int(sqrt((xx-x)*(xx-x)+(yy-y)*(yy-y)));
				if(dst<r){
					if(CheckTex(vert,TexID))SetHi(vert,GetHi(vert)+h);
				};
				if(dst<r+TriUnit*3)MarkPointToDraw(vert);
			};
		};
};
void CreatePlane(int x,int y1,int r){
	int y=Prop43(y1);

	int TexID=-1;
	if(GetKeyState(VK_SCROLL))TexID=GetNearestTexture(x,y);

	int utx=div(x,TriUnit*2).quot;
	int uty=div(y,TriUnit*2).quot;
	int r1=div(r,TriUnit*2).quot+2;
	int h=GetHeight(x,y);
	for(int tx=-r1;tx<r1;tx++)
		for(int ty=-r1;ty<r1;ty++){
			int vx=utx+tx;
			int vy=uty+ty;
			if(vx>=0&&vx<VertInLine&&vy>0){
				int vert=vx+vy*VertInLine;
				int xx=GetTriX(vert);
				int yy=GetTriY(vert);
				int dst=int(sqrt((xx-x)*(xx-x)+(yy-y)*(yy-y)));
				if(dst<r){
					if(CheckTex(vert,TexID))SetHi(vert,h);
				};
				if(dst<r+TriUnit*3)MarkPointToDraw(vert);
			};
		};
};
void PlanarHi(int x,int y1,int r){
	int y=Prop43(y1);

	int TexID=-1;
	if(GetKeyState(VK_SCROLL))TexID=GetNearestTexture(x,y);

	int utx=div(x,TriUnit*2).quot;
	int uty=div(y,TriUnit*2).quot;
	int r1=div(r,TriUnit*2).quot+2;
	int h0=GetHeight(x,y);
	for(int tx=-r1;tx<r1;tx++)
		for(int ty=-r1;ty<r1;ty++){
			int vx=utx+tx;
			int vy=uty+ty;
			if(vx>=0&&vx<VertInLine&&vy>0){
				int vert=vx+vy*VertInLine;
				int xx=GetTriX(vert);
				int yy=GetTriY(vert);
				int dst=int(sqrt((xx-x)*(xx-x)+(yy-y)*(yy-y)));
				if(dst<r){
					int h1=GetHeight(xx,yy);
					int dh=h1+div((h0-h1)*(r-dst),r).quot;
					if(CheckTex(vert,TexID))SetHi(vert,dh);
				};
				if(dst<r+TriUnit*3)MarkPointToDraw(vert);
			};
		};
};
void AverageHi(int x,int y1,int r){
	int y=Prop43(y1);

	int TexID=-1;
	if(GetKeyState(VK_SCROLL))TexID=GetNearestTexture(x,y);

	int utx=div(x,TriUnit*2).quot;
	int uty=div(y,TriUnit*2).quot;
	int r1=div(r,TriUnit*2).quot+2;
	for(int tx=-r1;tx<r1;tx++)
		for(int ty=-r1;ty<r1;ty++){
			int vx=utx+tx;
			int vy=uty+ty;
			if(vx>=0&&vx<VertInLine&&vy>0){
				int vert=vx+vy*VertInLine;
				int xx=GetTriX(vert);
				int yy=GetTriY(vert);
				int dst=int(sqrt((xx-x)*(xx-x)+(yy-y)*(yy-y)));
				if(dst<r){
					int dh=(GetHeight(xx-8,yy)+
							GetHeight(xx+8,yy)+
							GetHeight(xx,yy-8)+
							GetHeight(xx,yy+8))>>2;
					if(CheckTex(vert,TexID))SetHi(vert,dh);
				};
				if(dst<r+TriUnit*3)MarkPointToDraw(vert);
			};
		};
};
void SetHiInRound(int x,int y,int r1,int H1,int r2,int H2){
	if(r2<=r1)r2=r1+1;
	int utx=x/TriUnit/2;
	int uty=y/TriUnit/2;
	int rr=r2/TriUnit/2+2;
	for(int tx=-rr;tx<rr;tx++){
		for(int ty=-rr;ty<rr;ty++){
			int vx=utx+tx;
			int vy=uty+ty;
			if(vx>=0&&vx<VertInLine&&vy>0){
				int vert=vx+vy*VertInLine;
				int xx=GetTriX(vert);
				int yy=GetTriY(vert);
				int dst=int(sqrt((xx-x)*(xx-x)+(yy-y)*(yy-y)));
				int HF=H2;
				if(dst<r1){
					HF=H1;
				}else HF=H1+(H2-H1)*(dst-r1)/(r2-r1);
				if(HF>THMap[vert]){
					THMap[vert]=HF;
					MarkPointToDraw(vert);
				}
			}
		}
	}
};
void RandomHi(int x,int y1,int r,int ms,int fnd){
	int y=Prop43(y1);
	int utx=div(x,TriUnit*2).quot;
	int uty=div(y,TriUnit*2).quot;
	int r1=div(r,TriUnit*2).quot+2;
	for(int tx=-r1;tx<r1;tx++)
		for(int ty=-r1;ty<r1;ty++){
			int vx=utx+tx;
			int vy=uty+ty;
			if(vx>=0&&vx<VertInLine&&vy>0){
				int vert=vx+vy*VertInLine;
				int xx=GetTriX(vert);
				int yy=GetTriY(vert);
				int dst=int(sqrt((xx-x)*(xx-x)+(yy-y)*(yy-y)));
				if(dst<r){
					int dh=((rando()&ms)<<1)-fnd+GetHeight(xx,yy);
					SetHi(vert,dh);
				};
				if(dst<r+TriUnit*3)MarkPointToDraw(vert);
			};
		};
};
int iab(int i){
	if(i<0)return -i;
	else return i;
};
void BSetSQ(int x,int y,int Lx,int Ly);
void BClrSQ(int x,int y,int Lx,int Ly);
void ClearMaps();
struct VECTOR3D{
	int Dx;
	int Dy;
	int Dz;
};
void GetNormal(int x,int y,VECTOR3D* V3){
	int Dz=32<<4;
	int Dx=(GetHeight(x-16,y)-GetHeight(x+16,y))<<4;
	int Dy=(GetHeight(x,y-16)-GetHeight(x,y+16))<<4;
	int norm=int(sqrt(Dz*Dz+Dx*Dx+Dy*Dy));
	V3->Dx=div(Dx<<8,norm).quot;
	V3->Dy=div(Dy<<8,norm).quot;
	V3->Dz=div(Dz<<8,norm).quot;
};
int GetALP(VECTOR3D* V){
	int VXY=int(sqrt((V->Dx*V->Dx+V->Dy*V->Dy)));
	if(!V->Dz)return 10000;
	return div(VXY<<8,V->Dz).quot;
};
int GetTerrAngle(int x,int y){
	VECTOR3D V;
	GetNormal(x,y,&V);
	return GetALP(&V);
};
int GetCoef(int i,int j){
	return 64-((i+j+1)<<4)+(i+i+1)*(j+j+1);
};
void CreateFishMap();
void SetLockMask(int x,int y,char* mask){
	int pos=0;
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++){
			if(mask[pos]=='*')BSetPt(x+j,y+i);
			else BClrPt(x+j,y+i);
			pos++;
		};
};
extern int MAXSPR;
bool AddLockPts(int x0,int y0,ClonesArray<LockInfoPoint>& LockInfo,byte add);
void CreateLandLocking(int TAlp,bool ForVision){
	int mxx=msx>>1;
	int myy=msy>>1;
	for(int ix=0;ix<mxx;ix++){
		int ZLmin=1000000;
		int ZRmin=1000000;	
		for(int iy=myy-1;iy>=0;iy--){
			int x0=ix<<6;
			int y0=iy<<6;
			int ppx=ix<<2;
			int ppy=iy<<2;
			int ZZ=(iy<<5)-iy-iy;
			int z0=GetTotalHeight(x0,y0);
			int z1=GetTotalHeight(x0+64,y0);
			int z2=GetTotalHeight(x0,y0+64);
			int z3=GetTotalHeight(x0+64,y0+64);
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
			if(zm>=32){
                MFIELDS[0].BSetBar(ppx,ppy,4);
				if(zm>48)MFIELDS[2].BSetBar(ppx,ppy,4);
				else MFIELDS[2].BClrBar(ppx,ppy,4);
			}else BClrBar(ppx,ppy,4);
			
			if(ZL0<ZLmin)ZLmin=ZL0;
			if(ZR0<ZRmin)ZRmin=ZR0;
			if(za<-10){
				MFIELDS[0].BSetBar(ppx,ppy,4);
				//MFIELDS[3].BSetBar(ppx,ppy,4);
			}
			//if(za<0){
				//MFIELDS[2].BSetBar(ppx,ppy,4);
			//}				
			if(za>0)MFIELDS[1].BSetBar(ppx,ppy,4);
			else MFIELDS[1].BClrBar(ppx,ppy,4);
		};
	};
	//CreateWaterLocking(0,0,(MaxWX)-1,(MaxWX)-1);
	for(int i=0;i<MAXSPR;i++){
		OneSprite* OSP=Sprites+i;
		if(OSP->Enabled){
			ObjCharacter* OC=OSP->OC;
			SprGroup* SG=OSP->SG;
			if(OC->LockInfo.GetAmount()&&OC->IntResType>7){
				//int id=OSP->SGIndex;
				AddLockPts(OSP->x-OC->CenterX,(OSP->y>>1)-OC->CenterY,OC->LockInfo,1);
			};
		};
	};
	int N=UnLockBars.NBars;
	BlockCell* BC=UnLockBars.BC;
	for(i=0;i<N;i++){
		BClrBar(int(BC[i].x)<<2,int(BC[i].y)<<2,4);
	};
	N=LockBars.NBars;
	BC=LockBars.BC;
	for(i=0;i<N;i++){
		BSetBar(int(BC[i].x)<<2,int(BC[i].y)<<2,4);
	};
	N=HorceLockBars.NBars;
	BC=HorceLockBars.BC;
	for(i=0;i<N;i++){
		MFIELDS[3].BSetBar(int(BC[i].x)<<2,int(BC[i].y)<<2,4);
	};/*
	for(ix=1;ix<mxx;ix++)
		for(int iy=1;iy<myy;iy++){
			int ppx=ix<<2;
			int ppy=iy<<2;
			int N=0;
			if(CheckBar(ppx,ppy,4,4)){
				if(CheckBar(ppx,ppy,2,2))N++;
				if(CheckBar(ppx+2,ppy,2,2))N++;
				if(CheckBar(ppx,ppy+2,2,2))N++;
				if(CheckBar(ppx+2,ppy+2,2,2))N++;
			};
			if(N<3)BClrBar(ppx,ppy,4);
			else BSetBar(ppx,ppy,4);
		};
	*/
	MotionField* TMF=new MotionField;
	TMF->Allocate();
	//memcpy(TMF->MapH,MFIELDS->MapH,MAPSY*BMSX);
	memcpy(TMF->MapV,MFIELDS->MapV,MAPSY*BMSX);
	
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
					SetLockMask(ppx,ppy,"**** ***  **   *");
					break;
				case 6:
					SetLockMask(ppx,ppy,"   *  ** *******");
					break;
				case 12:
					SetLockMask(ppx,ppy,"*   **  *** ****");
					break;
				case 9:
					SetLockMask(ppx,ppy,"******* **  *   ");
					break;
				case 7:
				case 14:
				case 13:
				case 11:
					BSetBar(ppx,ppy,4);
					break;
				};
			};
		};
	//memcpy(TMF,MFIELDS,sizeof MotionField);
	//memcpy(TMF->MapH,MFIELDS->MapH,MAPSY*BMSX);
	memcpy(TMF->MapV,MFIELDS->MapV,MAPSY*BMSX);
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
					SetLockMask(ppx,ppy,"**** ***  **   *");
					break;
				case 6:
					SetLockMask(ppx,ppy,"   *  ** *******");
					break;
				case 12:
					SetLockMask(ppx,ppy,"*   **  *** ****");
					break;
				case 9:
					SetLockMask(ppx,ppy,"******* **  *   ");
					break;
				};
			};
		};
	TMF->FreeAlloc();
	delete(TMF);	
};
void CreateMinesLocking(){
	for(int i=0;i<MAXSPR;i++){
		OneSprite* OSP=Sprites+i;
		if(OSP->Enabled){
			ObjCharacter* OC=OSP->OC;
			SprGroup* SG=OSP->SG;
			if(OC->LockInfo.GetAmount()&&OC->IntResType<8){
				//int id=OSP->SGIndex;
				AddLockPts(OSP->x-OC->CenterX,(OSP->y>>1)-OC->CenterY,OC->LockInfo,1);
			};
		};
	};
};
void CheckBright();
void SmoothFields(MotionField* MF,int Index);
//extern int MinMapX;
//extern int MaxMapX;
//extern int MinMapY;
//extern int MaxMapY;
int LockCoef=256;//120*64/56;
void CreateHeightmap();
void CreateMapLocking(){
	CreateHeightmap();
	const RECT rc = MOptions.LimitCamArgs.GetRect();
//	int maxx=MaxMapX<<1;
//	int maxy=MaxMapY<<1;
	int maxx=rc.right<<1;
	int maxy=rc.bottom<<1;
	ClearMaps();
	CreateLandLocking(LockCoef,true);	
	for(int f=1;f<NMFIELDS;f++) SmoothFields(MFIELDS+f,f);//2
    MotionField* TMF=new MotionField;
    int mxx=msx<<1;
    int myy=msy<<1;
    TMF->Allocate();    
    memcpy(TMF->MapV,MFIELDS[1].MapV,MAPSY*BMSX);
    for(int ix=1;ix<mxx;ix++){
        for(int iy=1;iy<myy;iy++){
            if(!TMF->CheckPt(ix,iy)){
                MFIELDS[1].BClrBar(ix-2,iy-2,5);
            }
        }
    }
    memcpy(TMF->MapV,MFIELDS[1].MapV,MAPSY*BMSX);
    TMF->FreeAlloc();
    delete(TMF);

	CreateFishMap();
	//Border setting	
	int L1=32<<ADDSH;
	for(int i=0;i<NMFIELDS;i++){
		MFIELDS[i].BSetSQ(0,0,maxx,4);
		MFIELDS[i].BSetSQ(0,maxy-4,maxx,L1);
		MFIELDS[i].BSetSQ(0,4,4,maxy-4);
		MFIELDS[i].BSetSQ(maxx,0,L1,512<<ADDSH); 
	};
	
	/*
	for(int ix=0;ix<mxx;ix++){
		for(int iy=1;iy<25;iy++){
			if((iy<<5)-GetHeight((ix<<6)+32,(iy<<6))<16){
				for(int i=0;i<NMFIELDS;i++){
					MFIELDS[i].BSetBar(ix<<2,iy<<2,4);
				}				
			}
		}
	}
	*/
	void CreateAdditionalObjectsLocking();
	CreateAdditionalObjectsLocking();
};
void SetClearBuildigsLock(bool Set){
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB){
			if(OB->NewBuilding && !OB->Sdoxlo){
				int xx,yy;
				OB->GetCornerXY(&xx,&yy);
				NewMonster* NM=OB->newMons;
				int nn;
				char* LockX;
				char* LockY;
				if(OB->Stage<OB->MoreCharacter->ProduceStages&&NM->NBLockPt){
					nn=NM->NBLockPt;
					LockX=NM->BLockX;
					LockY=NM->BLockY;
				}else{
					nn=NM->NLockPt;
					LockX=NM->LockX;
					LockY=NM->LockY;
				}
				for(int i=0;i<nn;i++){
					if(Set)BSetPt(xx+LockX[i],yy+LockY[i]);
					else BClrPt(xx+LockX[i],yy+LockY[i]);
				}
			}			
		}
	}
}
void CreateUnitsLocking(){
	for(int i=0;i<MAXOBJECT;i++){
		OneObject* OB=Group[i];
		if(OB){
			if(OB->NewBuilding && !OB->Sdoxlo){
				int xx,yy;
				OB->GetCornerXY(&xx,&yy);
				NewMonster* NM=OB->newMons;
				int nn;
				char* LockX;
				char* LockY;
				if(OB->Stage<OB->MoreCharacter->ProduceStages&&NM->NBLockPt){
					nn=NM->NBLockPt;
					LockX=NM->BLockX;
					LockY=NM->BLockY;
				}else{
					nn=NM->NLockPt;
					LockX=NM->LockX;
					LockY=NM->LockY;
				};
				for(int i=0;i<nn;i++)BSetPt(xx+LockX[i],yy+LockY[i]);
			}else
			if(OB->CObjIndex!=0xFFFF){
				void ForceLockComplexObject(OneObject* OB);
				ForceLockComplexObject(OB);
			}
		};
	};	
};
void CheckBright();
void CreateTotalLocking(){	
	rando();
	CreateMapLocking();
	rando();
	if(!GetNAreas())CreateAreas();
	CreateMinesLocking();
	CreateUnitsLocking();
	CreateInfoMap();
};
void EraseAreas();
void CreateFastLocking(){
	EraseAreas();
	CreateMapLocking();
	CreateUnitsLocking();
	CreateInfoMap();
};

//---------------------Drawing curves-------------------
int CurveX[2048];
int CurveY[2048];
int NCurves=0;
extern int LastMx;
extern int LastMy;
bool CheckCurve(){
	return NCurves!=0;
};
int PlatoHi=128;
int SurfType=1;
void SetTextureInCurve();
void SetPlatoInCurve();
void SetHiInRegion(byte H);
bool EnterVal(int * val,char* Message);
int EnterHi(int * val,int Type);
void SoftRegion();
void InterpolateCurve();
void SetRoadInCurve(int);
void xLine(int x,int y,int x1,int y1,byte c);
bool CheckCurveLinked(int x,int y){
	if(NCurves>1){
		int L0=0;
		for(int i=0;i<NCurves-1;i++)L0+=Norma(CurveX[i+1]-CurveX[i],CurveY[i+1]-CurveY[i]);
		if(L0<30)return false;
		if(Norma(x-CurveX[0],y-CurveY[0])<10)return true;
		return false;
	}else return false;
};
int MINDST=80;
void AddPointToCurve(int x,int y,bool Final,byte Type){
	if(Final&&!NCurves)return;
	if(NCurves&&!Final){
		if(Norma(x-CurveX[NCurves-1],y-CurveY[NCurves-1])<MINDST)return;
	};
	if(NCurves<2048-2){
		CurveX[NCurves]=x;
		CurveY[NCurves]=y;
		NCurves++;
	}; 
	//if(Type&&NCurves>3&&!Final){
	//	InterpolateCurve();
	//};
	if(Final){
		CurveX[NCurves]=CurveX[0];
		CurveY[NCurves]=CurveY[0];
		NCurves++;
		if(Type==255){
			void PlaneRegion();
			PlaneRegion();
			NCurves=0;
			return;
		}else
		if(Type==1)SetTextureInCurve();
		if(Type==2){
			int p;
			if(p=EnterHi(&PlatoHi,SurfType)){
				SurfType=p;
				if(p==3)SoftRegion();
#ifdef _USE3D
				else if(p>=4){
					void CreateRoad(int,int);
					CreateRoad(p-4,0);
				}
#else
				else if(p>=4)SetRoadInCurve(p);
#endif
				else SetPlatoInCurve();
			};
		};
		if(Type>=127){
			SetRoadInCurve(Type-127);
		};
		NCurves=0;
	}
};
#ifndef _USE3D
void DrawCurves(){
	if(!NCurves)return;
	int xp,yp;
	int dx=mapx<<5;
	int dy=mapy<<5;
	int N=NCurves;
	if(LastMx>0&&LastMx<msx<<5&&LastMy>0&&LastMy<msy<<5){
		CurveX[NCurves]=LastMx;
		CurveY[NCurves]=LastMy;
		N++;
	};
	for(int i=0;i<N;i++){
		int x=CurveX[i]-dx;
		int y=((CurveY[i]-dy)>>1);
		if(Mode3D)y-=GetHeight(CurveX[i],CurveY[i]);
		if(i>0){
			if(!LMode){
				xLine(xp,yp,x,y,clrYello);
			}else{
				xLine(xp>>2,yp>>2,x>>2,y>>2,clrYello);
			};
		};
		xp=x;
		yp=y;
	};

};
#else
int SearchNearestRoadToPoint(int x,int y,bool Show);
extern int LastMx;
extern int LastMy;
void DeleteCurve(int idx);
extern int LastKey;
void AttractRoad(int idx,int x,int y);
void DrawCurves(){
	int CurveXt[2048];
	int CurveYt[2048];
	if(!NCurves){
		int idx=SearchNearestRoadToPoint(LastMx,LastMy,1);
		//if(LastKey==VK_DELETE){
		//	DeleteCurve(idx);
		//	LastKey=0;
		//};
		if(Rpressed){
			//AttractRoad(idx,LastMx,LastMy);
		};
		return;
	};
	int NC=NCurves;
	memcpy(CurveXt,CurveX,sizeof CurveX);
	memcpy(CurveYt,CurveY,sizeof CurveY);

	//if(NCurves>=3)InterpolateCurve();

	int xp,yp;
	int dx=mapx<<5;
	int dy=mapy<<5;
	int N=NCurves;
	if(LastMx>0&&LastMx<msx<<5&&LastMy>0&&LastMy<msy<<5){
		CurveX[NCurves]=LastMx;
		CurveY[NCurves]=LastMy;
		N++;
	};
	for(int i=0;i<N;i++){
		int x=CurveX[i]-dx;
		int y=((CurveY[i]-dy)>>1);
		if(Mode3D)y-=GetHeight(CurveX[i],CurveY[i]);
		if(i>0){
			if(LMode){
				xLine(xp>>2,yp>>2,x>>2,y>>2,clrYello);
			}else{
				xLine(xp,yp,x,y,clrYello);
				xLine(xp-2,yp-2,xp+2,yp+2,clrYello);
				xLine(xp+2,yp-2,xp-2,yp+2,clrYello);
			};
		};
		xp=x;
		yp=y;
	};

	NCurves=NC;
	memcpy(CurveX,CurveXt,sizeof CurveX);
	memcpy(CurveY,CurveYt,sizeof CurveY);

};
#endif
bool CheckPointInside(int x,int y){
	int NCross=0;
	for(int i=1;i<NCurves;i++){
		int x0=CurveX[i-1];
		int x1=CurveX[i  ];
		int y0=CurveY[i-1];
		int y1=CurveY[i  ];
		if(x1<x0){
			int t=x1;
			x1=x0;
			x0=t;
			t=y1;
			y1=y0;
			y0=t;
		};
		if(x0<=x&&x1>x&&(y0>y||y1>y)){
			if(x0!=x1){
				int yy=y0+((y1-y0)*(x-x0))/(x1-x0);
				if(yy>y)NCross++;
			};
		};
	};
	return NCross&1;
};
int GetCurveDistance(int x,int y){
	int NCross=0;
	int rmin=100000;
	for(int i=1;i<NCurves;i++){
		int x0=CurveX[i-1];
		int x1=CurveX[i  ];
		int y0=CurveY[i-1];
		int y1=CurveY[i  ];
		if(i==0){
			rmin=Norma(x-x0,y-y0);
		};
		int r=Norma(x-x1,y-y1);
		if(r<rmin){
			rmin=r;
		};
		if(x1<x0){
			int t=x1;
			x1=x0;
			x0=t;
			t=y1;
			y1=y0;
			y0=t;
		};
		if(x0<=x&&x1>x&&(y0>y||y1>y)){
			if(x0!=x1){
				int yy=y0+((y1-y0)*(x-x0))/(x1-x0);
				if(yy>y)NCross++;
			};
		};
	};
	if(NCross&1)return -rmin;
	else return rmin;
};
int GetCurveDistance1(int x,int y){
	int NCross=0;
	int rmin=100000;
	for(int i=1;i<NCurves;i++){
		int x0=CurveX[i-1];
		int x1=CurveX[i  ];
		int y0=CurveY[i-1];
		int y1=CurveY[i  ];
		if(x1<x0){
			int t=x1;
			x1=x0;
			x0=t;
			t=y1;
			y1=y0;
			y0=t;
		};
		int Dx=x1-x0;
		int Dy=y1-y0;
		int D=Dx*Dx+Dy*Dy;
		if(D){
			int N=(((x-x0)*Dx+(y-y0)*Dy)<<8)/D;
			if(N<=256&&N>=0){
				int r=abs(int(Dy*(x-x0)-Dx*(y-y0)))/int(sqrt(D));
				if(r<rmin)rmin=r;
			};
		};
		Dx=x-x0;
		Dy=y-y0;
		int r1=int(sqrt(Dx*Dx+Dy*Dy));
		if(r1<rmin)rmin=r1;
		if(x0<=x&&x1>x&&(y0>y||y1>y)){
			if(x0!=x1){
				int yy=y0+((y1-y0)*(x-x0))/(x1-x0);
				if(yy>y)NCross++;
			};
		};
	};
	if(NCross&1)return -rmin;
	else return rmin;
};
int GetCurveDistance0(int x,int y){
	int NCross=0;
	int rmin=100000;
	for(int i=1;i<NCurves-1;i++){
		int x0=CurveX[i-1];
		int x1=CurveX[i  ];
		int y0=CurveY[i-1];
		int y1=CurveY[i  ];
		if(x1<x0){
			int t=x1;
			x1=x0;
			x0=t;
			t=y1;
			y1=y0;
			y0=t;
		};
		int Dx=x1-x0;
		int Dy=y1-y0;
		int D=Dx*Dx+Dy*Dy;
		if(D){
			int N=(((x-x0)*Dx+(y-y0)*Dy)<<8)/D;
			if(N<=256&&N>=0){
				int r=abs(int(Dy*(x-x0)-Dx*(y-y0)))/int(sqrt(D));
				if(r<rmin)rmin=r;
			};
		};
		Dx=x-x0;
		Dy=y-y0;
		int r1=int(sqrt(Dx*Dx+Dy*Dy));
		if(r1<rmin)rmin=r1;
		if(x0<=x&&x1>x&&(y0>y||y1>y)){
			if(x0!=x1){
				int yy=y0+((y1-y0)*(x-x0))/(x1-x0);
				if(yy>y)NCross++;
			};
		};
	};
	if(NCross&1)return -rmin;
	else return rmin;
};
//return value: 0..4096;-1-no crossing
int GetCrossProportion(int x0,int y0,int x1,int y1){
	if(x0==x1||y0==y1)return -1;
	for(int i=1;i<NCurves;i++){
		int xc0=CurveX[i-1];
		int xc1=CurveX[i  ];
		int yc0=CurveY[i-1];
		int yc1=CurveY[i  ];
		if(xc1<xc0){
			int t=xc1;
			xc1=xc0;
			xc0=t;
			t=yc1;
			yc1=yc0;
			yc0=t;
		};
		if((x0<=xc0&&xc1>x0)||(xc0<=x1&&xc1>x1)){
			int D1=x1-x0;
			int D2=xc1-xc0;
			int B1=y1-y0;
			int B2=yc1-yc0;
			int A1=y0*x1-x0*y1;
			int A2=yc0*xc1-xc0*yc1;
			int DET=B2*D1-B1*D2;
			if(DET){
				if(abs(D1)>abs(B1)){
					int x=(A1*D2-A2*D1)/DET;
					int T=((x-x0)<<12)/(x1-x0);
					if(T>=0&&T<=4096)return T;
				}else{
					int y=(A1*B2-A2*B1)/DET;
					int T=((y-y0)<<12)/(y1-y0);
					if(T>=0&&T<=4096)return T;
				};
			};
		};
	};
	return -1;
};
void PutTexInPointWithCurve(int i){	
};
void SetSectionsInPoint(int i,int h,short* VertHi){	
};
void SetTextureInCurve(){
	int xmin=100000;
	int ymin=100000;
	int xmax=-1;
	int ymax=-1;
	for(int i=0;i<NCurves;i++){
		int x=CurveX[i];
		int y=CurveY[i];
		if(x<xmin)xmin=x;
		if(x>xmax)xmax=x;
		if(y<ymin)ymin=y;
		if(y>ymax)ymax=y;
	};

	int utx0=div(xmin,TriUnit*2).quot-1;
	int uty0=div(ymin,TriUnit*2).quot-1;
	int utx1=div(xmax,TriUnit*2).quot+1;
	int uty1=div(ymax,TriUnit*2).quot+1;

	if(utx0<0)utx0=0;
	if(uty0<0)uty0=0;

	if(utx1>=VertInLine)utx0=VertInLine-1;
	if(uty1>=MaxTH)uty0=MaxTH-1;

	for(int tx=utx0;tx<=utx1;tx++)
		for(int ty=uty0;ty<=uty1;ty++){
			int vert=tx+ty*VertInLine;
			PutTexInPointWithCurve(vert);
		};
};
int FUN1(int r){
	if(r>64){
		if(r<128){
			return (int(rand())*(128-r))>>16;
		};
		r=64;
	};
	if(r<-64)r=-64;
	//if(r<0)return 
	//else return ((64-r)*(64-r))>>4;
	return 512-12*r+((r*r*r)>>10);
};
int FUN2(int r){
	if(r>64){
		if(r<128){
			return (int(rand())*(128-r))>>16;
		};
		r=64;
	};
	if(r<-64)r=-64;
	return ((64-r)*(64-r))>>4;
};
typedef int FUNType(int);
void SetPlatoInCurve(){
	FUNType* FTP3;
	if(SurfType==1)FTP3=&FUN1;
	else if(SurfType==2)FTP3=&FUN2;
	else return;
	int xmin=100000;
	int ymin=100000;
	int xmax=-1;
	int ymax=-1;
	for(int i=0;i<NCurves;i++){
		int x=CurveX[i];
		int y=CurveY[i];
		if(x<xmin)xmin=x;
		if(x>xmax)xmax=x;
		if(y<ymin)ymin=y;
		if(y>ymax)ymax=y;
	};

	int utx0=div(xmin,TriUnit*2).quot-5;
	int uty0=div(ymin,TriUnit*2).quot-5;
	int utx1=div(xmax,TriUnit*2).quot+5;
	int uty1=div(ymax,TriUnit*2).quot+5;

	if(utx0<0)utx0=0;
	if(uty0<0)uty0=0;

	if(utx1>=VertInLine)utx0=VertInLine-1;
	if(uty1>=MaxTH)uty0=MaxTH-1;

	for(int tx=utx0;tx<=utx1;tx++)
		for(int ty=uty0;ty<=uty1;ty++){
			int vert=tx+ty*VertInLine;
			int x=GetTriX(vert);
			int y=GetTriY(vert);
			int r=GetCurveDistance1(x,y);
			int A=(*FTP3)(r);
			/*
			if(r>128)A=0;
			else if(r<-128)A=1024;
			else A=(128-r)<<2;
			*/
			int H=THMap[vert];
			int H1=(H*(1024-A)+PlatoHi*A)>>10;
			if(H1!=THMap[vert]){
				THMap[vert]=H1;
				MarkPointToDraw(vert);
			};
		};
};
void PlaneRegion(){
	int xmin=100000;
	int ymin=100000;
	int xmax=-1;
	int ymax=-1;
	for(int i=0;i<NCurves;i++){
		int x=CurveX[i];
		int y=CurveY[i];
		if(x<xmin)xmin=x;
		if(x>xmax)xmax=x;
		if(y<ymin)ymin=y;
		if(y>ymax)ymax=y;
	};

	int utx0=div(xmin,TriUnit*2).quot-5;
	int uty0=div(ymin,TriUnit*2).quot-5;
	int utx1=div(xmax,TriUnit*2).quot+5;
	int uty1=div(ymax,TriUnit*2).quot+5;

	if(utx0<0)utx0=0;
	if(uty0<0)uty0=0;

	if(utx1>=VertInLine)utx0=VertInLine-1;
	if(uty1>=MaxTH)uty0=MaxTH-1;

	int HS=0;
	int NH=0;

	for(int tx=utx0;tx<=utx1;tx++)
		for(int ty=uty0;ty<=uty1;ty++){
			int vert=tx+ty*VertInLine;
			int x=GetTriX(vert);
			int y=GetTriY(vert);
			int r=GetCurveDistance1(x,y);
			if(r<0){
				HS+=THMap[vert];
				NH++;
			}			
		};
	if(NH){
		HS/=NH;
		for(int tx=utx0;tx<=utx1;tx++)
			for(int ty=uty0;ty<=uty1;ty++){
				int vert=tx+ty*VertInLine;
				int x=GetTriX(vert);
				int y=GetTriY(vert);
				int r=GetCurveDistance1(x,y);
				if(r<0){
					if(THMap[vert]>50)THMap[vert]=HS;
					MarkPointToDraw(vert);
				}else if(r<64){
					int H=(int(THMap[vert])*r+HS*(64-r))/64;
					if(THMap[vert]>50)THMap[vert]=H;
					MarkPointToDraw(vert);
				}				
			};
	}
};
byte RTARR0[16]={81,81,81,11,4 ,5,6,0,0,0,0,0,0,0,0,0};
byte RTARR1[16]={81,11,4 ,5 ,6 ,0,0,0,0,0,0,0,0,0,0,0};
byte RTARR2[16]={11,4 ,5 ,6 ,0 ,0,0,0,0,0,0,0,0,0,0,0};
byte RoadPrio[256]={
	0x00,0x0C,0x0C,0x10,0x0D,0x0C,0x0B,0x10,0xFF,0xFF,//00
	0xFF,0x0E,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//10
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//20
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//30
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//40
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//50
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//60
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//70
	0xFF,0x10,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//80
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//90
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//100
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//110
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//120
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//130
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//140
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//150
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//160
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//170
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//180
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//190
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//200
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//210
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//220
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//230
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,//240
	0xFF,0xFF,0xFF,0xFF,0xFF};                        //250


void SetRoadInCurve(int tp){
	byte* RTARR;
	if(tp==4)RTARR=RTARR0;
	else if(tp==5)RTARR=RTARR1;
	else RTARR=RTARR2;
	int xmin=100000;
	int ymin=100000;
	int xmax=-1;
	int ymax=-1;
	for(int i=0;i<NCurves;i++){
		int x=CurveX[i];
		int y=CurveY[i];
		if(x<xmin)xmin=x;
		if(x>xmax)xmax=x;
		if(y<ymin)ymin=y;
		if(y>ymax)ymax=y;
	};

	int utx0=div(xmin,TriUnit*2).quot-2;
	int uty0=div(ymin,TriUnit*2).quot-2;
	int utx1=div(xmax,TriUnit*2).quot+2;
	int uty1=div(ymax,TriUnit*2).quot+2;

	if(utx0<0)utx0=0;
	if(uty0<0)uty0=0;

	if(utx1>=VertInLine)utx1=VertInLine-1;
	if(uty1>=MaxTH)uty1=MaxTH-1;

	for(int tx=utx0;tx<=utx1;tx++)
		for(int ty=uty0;ty<=uty1;ty++){
			int vert=tx+ty*VertInLine;
			int x=GetTriX(vert);
			int y=GetTriY(vert);
			int r=abs(GetCurveDistance0(x,y));
			if(r<128){
				int tidx=r*10/PlatoHi;
				if(tidx<0)tidx=0;
				if(tidx>14)tidx=14;
				int tx=RTARR[tidx];
				int t0=TexMap[vert];
				if(RoadPrio[tx]>RoadPrio[t0]){
					TexMap[vert]=tx;
					MarkPointToDraw(vert);
				};
			};
		};
};
void SoftRegion(){
	int NVert=0;
	int MaxVert=0;
	int* Vert=NULL;
	int* VertHi=NULL;
	int xmin=100000;
	int ymin=100000;
	int xmax=-1;
	int ymax=-1;
	for(int i=0;i<NCurves;i++){
		int x=CurveX[i];
		int y=CurveY[i];
		if(x<xmin)xmin=x;
		if(x>xmax)xmax=x;
		if(y<ymin)ymin=y;
		if(y>ymax)ymax=y;
	};

	int utx0=div(xmin,TriUnit*2).quot-1;
	int uty0=div(ymin,TriUnit*2).quot-1;
	int utx1=div(xmax,TriUnit*2).quot+1;
	int uty1=div(ymax,TriUnit*2).quot+1;

	if(utx0<0)utx0=0;
	if(uty0<0)uty0=0;

	if(utx1>=VertInLine)utx0=VertInLine-1;
	if(uty1>=MaxTH)uty0=MaxTH-1;

	for(int tx=utx0;tx<=utx1;tx++)
		for(int ty=uty0;ty<=uty1;ty++){
			int vert=tx+ty*VertInLine;
			int x=GetTriX(vert);
			int y=GetTriY(vert);
			if(CheckPointInside(x,y)){
				if(NVert>=MaxVert){
					MaxVert+=256;
					Vert=(int*)realloc(Vert,MaxVert*4);
					VertHi=(int*)realloc(VertHi,MaxVert*4);
				};
				Vert[NVert]=vert;
				NVert++;
			};
		};
	int NC=0;
	int maxdif=0;
	do{
		maxdif=0;
		for(int i=0;i<NVert;i++){
			int vert=Vert[i];
			int Lin=vert/VertInLine;
			int Lpos=vert%VertInLine;
			if(Lin>0&&Lpos>0&&Lpos<VertInLine-1&&Lin<MaxTH-1){
				if(Lpos&1){
					VertHi[i]=(THMap[vert+VertInLine]+THMap[vert-VertInLine]+THMap[vert-1]+
						THMap[vert+1]+THMap[vert-VertInLine+1]+THMap[vert-VertInLine-1])/6;
				}else{
					VertHi[i]=(THMap[vert+VertInLine]+THMap[vert-VertInLine]+THMap[vert-1]+
						THMap[vert+1]+THMap[vert+VertInLine+1]+THMap[vert+VertInLine-1])/6;
				};
			};
			//int x=GetTriX(vert);
			//int y=GetTriY(vert);
			//int hav=(GetHeight(x-32,y)+GetHeight(x+32,y)+GetHeight(x,y-32)+GetHeight(x,y+32))>>2;
			//assert(hav>=0);
			//VertHi[i]=hav;
		};
		for(i=0;i<NVert;i++){
			int DH=abs(VertHi[i]-THMap[Vert[i]]);
			if(DH>maxdif)maxdif=DH;
			THMap[Vert[i]]=VertHi[i];
		};
		NC++;
	}while(NC<80);
	for(i=0;i<NVert;i++){
		MarkPointToDraw(Vert[i]);
	};
	if(NVert){
		delete(Vert);
		delete(VertHi);
	};
};

void ClearCurve(){
	NCurves=0;
};
void GetBezierVect (int* xi,int* yi,int np,int i,int* dx,int* dy){
	if(i==0){
		*dx=2*(xi[i+1]-xi[i]);
		*dy=2*(yi[i+1]-yi[i]);
	}else if(i==np-1){
		*dx=2*(xi[i]-xi[i-1]);
		*dy=2*(yi[i]-yi[i-1]);
	}else{
		*dx=(xi[i+1]-xi[i-1]);
		*dy=(yi[i+1]-yi[i-1]);
	};

};
void Normalise(int* dx,int* dy,int N){
	int NR=Norma(*dx,*dy);
	if(NR){
		*dx=(N*(*dx))/NR;
		*dy=(N*(*dy))/NR;
	};
};
int SNorma(int dx,int dy){
	return sqrt(dx*dx+dy*dy);
};
void InterpolateCurve(){
	int* CurX=znew(int,NCurves);
	int* CurY=znew(int,NCurves);
	int NCur=NCurves;
	NCurves=0;
	for(int i=0;i<NCur;i++){
		CurX[i]=CurveX[i];
		CurY[i]=CurveY[i];
	};
	for(i=1;i<NCur;i++){
		double px0=CurX[i-1];
		double py0=CurY[i-1];
		double px3=CurX[i];
		double py3=CurY[i];
		double px1,py1,px2,py2;
		int Nr=2*SNorma(CurX[i]-CurX[i-1],CurY[i]-CurY[i-1])/5;
		int dx,dy;
		GetBezierVect(CurX,CurY,NCur,i-1,&dx,&dy);
		Normalise(&dx,&dy,Nr);
		px1=px0+dx;
		py1=py0+dy;
		GetBezierVect(CurX,CurY,NCur,i,&dx,&dy);
		Normalise(&dx,&dy,Nr);
		px2=px3-dx;
		py2=py3-dy;
		int Np=int(sqrt((px1-px0)*(px1-px0)+(py1-py0)*(py1-py0)))/5;
		if(Np>1){
			Np+=2;
			for(int j=0;j<Np;j++){
				double U=double(j)/Np;
				double B0=(1-U)*(1-U)*(1-U);
				double B1=3*U*(1-U)*(1-U);
				double B2=3*U*U*(1-U);
				double B3=U*U*U;
				int    x=int(B0*px0+B1*px1+B2*px2+B3*px3);
				int    y=int(B0*py0+B1*py1+B2*py2+B3*py3);
				MINDST=8;
				AddPointToCurve(x,y,false,0);
				MINDST=80;
			};
		}else{
			AddPointToCurve(CurX[i-1],CurY[i-1],false,0);
		};
		if(i==NCur-1){
			AddPointToCurve(CurX[i],CurY[i],false,0);
		};
	};
	delete(CurX);
	delete(CurY);
};
//------------------Special functions for random generation and texturing---------------
void SetPlatoInCurve(byte* VertHi,byte* VertType,byte Type){
	FUNType* FTP3;
	if(SurfType==1)FTP3=&FUN1;
	else if(SurfType==2)FTP3=&FUN2;
	else return;
	int xmin=100000;
	int ymin=100000;
	int xmax=-1;
	int ymax=-1;
	for(int i=0;i<NCurves;i++){
		int x=CurveX[i];
		int y=CurveY[i];
		if(x<xmin)xmin=x;
		if(x>xmax)xmax=x;
		if(y<ymin)ymin=y;
		if(y>ymax)ymax=y;
	};

	int utx0=div(xmin,TriUnit*2).quot-5;
	int uty0=div(ymin,TriUnit*2).quot-5;
	int utx1=div(xmax,TriUnit*2).quot+5;
	int uty1=div(ymax,TriUnit*2).quot+5;

	if(utx0<0)utx0=0;
	if(uty0<0)uty0=0;

	if(utx1>=VertInLine)utx0=VertInLine-1;
	if(uty1>=MaxTH)uty0=MaxTH-1;

	for(int tx=utx0;tx<=utx1;tx++)
		for(int ty=uty0;ty<=uty1;ty++){
			int vert=tx+ty*VertInLine;
			int x=GetTriX(vert);
			int y=GetTriY(vert);
			int r=GetCurveDistance1(x,y);
			int A=(*FTP3)(r);
			int B=A>>3;
			if(B>VertHi[vert])VertHi[vert]=B;
			if(B)VertType[vert]=Type;
			/*
			if(r>128)A=0;
			else if(r<-128)A=1024;
			else A=(128-r)<<2;
			*/
			int H=THMap[vert];
			int H1=(H*(1024-A)+PlatoHi*A)>>10;
			if(H1!=THMap[vert]){
				THMap[vert]=H1;
			};
		};
};
void SetHiInRegion(byte H){
	/*
	int xmin=100000;
	int ymin=100000;
	int xmax=-1;
	int ymax=-1;
	for(int i=0;i<NCurves;i++){
		int x=CurveX[i];
		int y=CurveY[i];
		if(x<xmin)xmin=x;
		if(x>xmax)xmax=x;
		if(y<ymin)ymin=y;
		if(y>ymax)ymax=y;
	};
	if(xmin<xmax){
		xmin>>=4;
		ymin>>=4;
		xmax>>=4;
		ymax>>=4;
		for(int dx=xmin;dx<=xmax;dx++)
			for(int dy=ymin;dy<=ymax;dy++)
				if(CheckPointInside(dx<<4,dy<<4))SetL3Point(dx,dy,H);
	};
	*/
};
void SoftRegion(short* MpVertHi){
	int NVert=0;
	int MaxVert=0;
	int* Vert=NULL;
	int* VertHi=NULL;
	int* TmVertHi=NULL;
	int xmin=100000;
	int ymin=100000;
	int xmax=-1;
	int ymax=-1;
	for(int i=0;i<NCurves;i++){
		int x=CurveX[i];
		int y=CurveY[i];
		if(x<xmin)xmin=x;
		if(x>xmax)xmax=x;
		if(y<ymin)ymin=y;
		if(y>ymax)ymax=y;
	};

	int utx0=div(xmin,TriUnit*2).quot-1;
	int uty0=div(ymin,TriUnit*2).quot-1;
	int utx1=div(xmax,TriUnit*2).quot+1;
	int uty1=div(ymax,TriUnit*2).quot+1;

	if(utx0<0)utx0=0;
	if(uty0<0)uty0=0;

	if(utx1>=VertInLine)utx0=VertInLine-1;
	if(uty1>=MaxTH)uty0=MaxTH-1;

	for(int tx=utx0;tx<=utx1;tx++)
		for(int ty=uty0;ty<=uty1;ty++){
			int vert=tx+ty*VertInLine;
			int x=GetTriX(vert);
			int y=GetTriY(vert);
			if(CheckPointInside(x,y)){
				if(NVert>=MaxVert){
					MaxVert+=256;
					Vert=(int*)realloc(Vert,MaxVert*4);
					VertHi=(int*)realloc(VertHi,MaxVert*4);
					TmVertHi=(int*)realloc(TmVertHi,MaxVert*4);
				};
				Vert[NVert]=vert;
				NVert++;
			};
		};
	int NC=0;
	int maxdif=0;
	do{
		maxdif=0;
		for(int i=0;i<NVert;i++){
			int vert=Vert[i];
			int Lin=vert/VertInLine;
			int Lpos=vert%VertInLine;
			if(Lin>0&&Lpos>0&&Lpos<VertInLine-1&&Lin<MaxTH-1){
				if(Lpos&1){
					VertHi[i]=(THMap[vert+VertInLine]+THMap[vert-VertInLine]+THMap[vert-1]+
						THMap[vert+1]+THMap[vert-VertInLine+1]+THMap[vert-VertInLine-1])/6;
					TmVertHi[i]=(MpVertHi[vert+VertInLine]+MpVertHi[vert-VertInLine]+MpVertHi[vert-1]+
						MpVertHi[vert+1]+MpVertHi[vert-VertInLine+1]+MpVertHi[vert-VertInLine-1])/6;
				}else{
					VertHi[i]=(THMap[vert+VertInLine]+THMap[vert-VertInLine]+THMap[vert-1]+
						THMap[vert+1]+THMap[vert+VertInLine+1]+THMap[vert+VertInLine-1])/6;
					TmVertHi[i]=(MpVertHi[vert+VertInLine]+MpVertHi[vert-VertInLine]+MpVertHi[vert-1]+
						MpVertHi[vert+1]+MpVertHi[vert+VertInLine+1]+MpVertHi[vert+VertInLine-1])/6;
				};
			};
			//int x=GetTriX(vert);
			//int y=GetTriY(vert);
			//int hav=(GetHeight(x-32,y)+GetHeight(x+32,y)+GetHeight(x,y-32)+GetHeight(x,y+32))>>2;
			//assert(hav>=0);
			//VertHi[i]=hav;
		};
		for(i=0;i<NVert;i++){
			int DH=abs(VertHi[i]-THMap[Vert[i]]);
			if(DH>maxdif)maxdif=DH;
			THMap[Vert[i]]=VertHi[i];
			MpVertHi[Vert[i]]=TmVertHi[i];
		};
		NC++;
	}while(NC<80);
	for(i=0;i<NVert;i++){
		MarkPointToDraw(Vert[i]);
	};
	if(NVert){
		delete(Vert);
		delete(VertHi);
		delete(TmVertHi);
	};
};
const int R14[10]={0,1,2,2,3,4,4,5,5,6};
int mrand();
void PaintAllMap(short* VertHi,byte* VertType,PaintHills* PHL,int NTypes){
	for(int i=0;i<MaxPointIndex;i++){
		word VType=VertType[i];
		if(VType>0&&VType<=NTypes){
			PaintHills* PH=PHL+VType-1;
			int vx=i%VertInLine;
			int vy=i/VertInLine;
			if(vx>0&&vx<VertInLine-1&&vy>1&&vx<MaxTH-1){
				int H=VertHi[i];
				int HU=VertHi[i-VertInLine];
				int HD=VertHi[i+VertInLine];
				int HRU,HRD,HLU,HLD;
				if(vx&1){
					HRU=VertHi[i-VertInLine+1];
					HLU=VertHi[i-VertInLine-1];
					HRD=VertHi[i+1];
					HLD=VertHi[i-1];
				}else{
					HRD=VertHi[i+VertInLine+1];
					HLD=VertHi[i+VertInLine-1];
					HRU=VertHi[i+1];
					HLU=VertHi[i-1];
				};
				int HL=(HLU+HLD)>>1;
				int HR=(HRU+HRD)>>1;
				int DH=Norma(HR-HL,HU-HD);
				bool UNFND=true;
				int L=0;
				for(int j=0;j<PH->NLayers&&UNFND;j++){
					if(H>=PH->TexStartHi[j]&&H<=PH->TexEndHi[j]&&DH>=PH->TexStartAng[j]&&DH<=PH->TexEndAng[j]){
						int ct=PH->CondType[j];
						L=j;
						if(ct){
							int r,rd;
							int xx=GetTriX(i)>>4;
							int yy=GetTriY(i)>>4;
							int r0=PH->Param1[j];
							int r1=PH->Param1[j];
							if(r0!=r1){
								r=r0+(((r1-r0+1)*int(mrand()))>>15);
							}else r=r0;
							if(r<10)rd=R14[r];else rd=0;
							switch(ct){
							case 1://L
								if(CheckPt(xx,yy)){
									UNFND=false;
									if(r&&CheckPt(xx-r,yy)    &&CheckPt(xx+r,yy)    &&CheckPt(xx,yy-r)    &&CheckPt(xx,yy+r)   &&
										  CheckPt(xx-rd,yy-rd)&&CheckPt(xx+rd,yy-rd)&&CheckPt(xx-rd,yy+rd)&&CheckPt(xx+rd,yy+rd))UNFND=true;
								};
								break;
							case 2://U
								if(!CheckPt(xx,yy)){
									UNFND=false;
									if(r&&!(CheckPt(xx-r,yy)  ||CheckPt(xx+r,yy)    ||CheckPt(xx,yy-r)    ||CheckPt(xx,yy+r)   ||
										  CheckPt(xx-rd,yy-rd)||CheckPt(xx+rd,yy-rd)||CheckPt(xx-rd,yy+rd)||CheckPt(xx+rd,yy+rd)))UNFND=true;
								};
								break;
	
							};
						};
					};
					if(!UNFND){
						int ti=(int(rand())*PH->TexAmount[L])>>15;
						TexMap[i]=byte(PH->Texs[L][ti]);
					};
				};
				for(int p=0;p<PH->NHiSections;p++)SetSectionsInPoint(i,PH->HiSect[p],VertHi);
			};
		};
	};
};
void GenerateRandomRoad(int idx){
	int cnt=0;
	SurfType=6;
	PlatoHi=100;
	do{
		cnt++;
		int x0=(mrand()*(480<<ADDSH))>>15;
		int y0=(mrand()*(480<<ADDSH))>>15;
		if(!CheckBar(x0-6,y0-6,12,12)){
			AddPointToCurve(x0<<4,y0<<4,0,idx);
			byte dir0=mrand()&255;
			int L=400;
			int L0=6;
			do{
				int maxd=32;
				int x1,y1;
				byte d1;
				do{
					int dd=((mrand()*maxd)>>14)-maxd;
					d1=dir0+dd;
					x1=x0+((10*int(TCos[d1]))>>8);
					y1=y0+((10*int(TSin[d1]))>>8);
					maxd+=10;
				}while(maxd<100&&CheckBar(x1-6,y1-6,12,12));
				if(maxd<100&&L>3){
					if(L0<1){
						AddPointToCurve(x1<<4,y1<<4,1,127+SurfType);
						L0=6;
					};
					L0--;
					AddPointToCurve(x1<<4,y1<<4,0,127+SurfType);
					x0=x1;
					y0=y1;
					dir0=d1;
				}else{
					L=1;
					AddPointToCurve(x1<<4,y1<<4,1,127+SurfType);
					return;
				};
				L--;
			}while(L);
			return;
		};
	}while(cnt<200);
};
extern int RealLx;
extern int RealLy;
void xLine(int x,int y,int x1,int y1,byte c);
void SHOWBARS(BlockBars* BB,DWORD c){
	int dx=mapx<<5;
	int dy=mapy<<4;
	static int idx=GPS.PreLoadGPImage("Interf3\\btnz");	
	for(int i=0;i<BB->NBars;i++){
		int xr=(BB->BC[i].x<<6);
		int yr=(BB->BC[i].y<<6);
        int zr=GetTotalHeight(xr,yr);
        Vector3D V(xr,yr,zr);
        WorldToScreenSpace(V);
        if(V.x>-32&&V.y>-32&&V.x<RealLx+32&&V.y<RealLy-32){
            Matrix4D M;
            M.translation(Vector3D(xr,yr,zr));
            GPS.DrawWSprite(idx,c,M,0);                
		};
	};
};
void ShowLockBars(){
	SHOWBARS(&UnLockBars,13);
	SHOWBARS(&LockBars,14);
	SHOWBARS(&HorceLockBars,15);
};
char* GSU_file;
void GSU_Error(char* Nation){
	char ccc[100];
	sprintf(ccc,"Invalid data for %s in %s",Nation,GSU_file);
	MessageBox(hwnd,ccc,"ERROR!",0);
};
extern HWND hwnd;
bool ReadWinString(GFILE* F,char* STR,int Max);
int ADDRES[8];
bool GenerateStartUnits(char* NationID,byte NI,int x,int y,int GenIndex){
	memset(ADDRES,0,8*4);
	if(!GenIndex)return false;
	char ccc[128];
	sprintf(ccc,"Data\\Start%d.dat",GenIndex);
	GSU_file=ccc;
	GFILE* F=Gopen(ccc,"r");
	bool READOK=1;
	if(F){
		char ccc[128];
		do{
			ccc[0]=0;
			READOK=ReadWinString(F,ccc,128);
			while(ccc[0]&&ccc[strlen(ccc)-1]==' ')ccc[strlen(ccc)-1]=0;
			if(ccc[0]=='#'&&!strcmp(ccc+1,NationID)){
				//found!
				word UnitsIDS[64];
				char UnitMark[64];
				int NUIDS=0;
				ccc[0]=0;
				ReadWinString(F,ccc,128);
				int z=sscanf(ccc,"%d%d%d%d%d%d",ADDRES+TreeID,ADDRES+FoodID,ADDRES+StoneID,ADDRES+GoldID,ADDRES+IronID,ADDRES+CoalID);
				if(z!=6){
					memset(ADDRES,0,8*4);
					GSU_Error(NationID);
					Gclose(F);
					return false;
				};
				ccc[0]=0;
				int N;
				ReadWinString(F,ccc,128);
				z=sscanf(ccc,"%d",&N);
				if(z==1){
					for(int j=0;j<N;j++){
						ReadWinString(F,ccc,128);
						char ccx[128];
						char ccy[128];
						int z=sscanf(ccc,"%s%s",ccx,ccy);
						if(z==2){
							//search for the monster
							GeneralObject** GOS=NATIONS[0].Mon;
							int NGO=NATIONS[0].NMon;
							for(int q=0;q<NGO;q++)if(!strcmp(GOS[q]->MonsterID,ccx)){
								UnitsIDS[NUIDS]=q;
								UnitMark[NUIDS]=ccy[0];
								NUIDS++;
								q=NGO+100;
							};
						}else{
							GSU_Error(NationID);
							Gclose(F);
							return false;
						};
					};
					int dx=0;
					int dy=0;
					ccc[0]=0;
					ReadWinString(F,ccc,128);
					if(!ccc[0]){
						GSU_Error(NationID);
						Gclose(F);
						return false;
					};
					int z=sscanf(ccc,"%d%d",&dx,&dy);
					if(z==2){
						//ready to read data
						int Y0=0;
						do{
							ccc[0]='#';
							ccc[1]=0;
							ReadWinString(F,ccc,128);
							if(ccc[0]!='#'){
								int L=strlen(ccc);
								for(int v=0;v<L;v++){
									char c=ccc[v];
									if(c!=' '){
										for(int u=0;u<NUIDS;u++)if(UnitMark[u]==c){
											NATIONS[NI].CreateNewMonsterAt((x<<4)+((v-dx)<<9),(y<<4)+((Y0-dy)<<9),UnitsIDS[u],1);
											u=NUIDS;
										};
									};
								};
							};
							Y0++;
						}while(ccc[0]!='#');
					}else{
						GSU_Error(NationID);
						Gclose(F);
						return false;
					};
				}else{
					GSU_Error(NationID);
					Gclose(F);
					return false;
				};
				Gclose(F);
				return true;
			};
		}while(READOK);
		Gclose(F);
		return false;
	}else return false;
};
void AddToLightHash(int Vertex,byte Light);
extern int LightDX;
extern int LightDY;
extern int LightDZ;
__forceinline int GetLighting3DV(int dx,int dy,int dz){
	int lig=(dx*LightDX+dy*LightDY+dz*LightDZ)/sqrt(dx*dx+dy*dy+dz*dz);
	if(lig<120)lig=120;
	if(lig>250)lig=250;
	return lig;

}
extern byte* FactureMap;
extern byte* FactureWeight;
bool CheckFactureWeightInPoint(int V){
	int vx=V%VertInLine;
	int vy=(V/VertInLine)<<4;
	if(vx&1)vy-=8;
	vx<<=5;
	int vy1=vy-THMap[V];
	int V1=V+VertInLine;
	int N=0;
	while(V1<MaxPointIndex){
		N++;
		vy+=16;
		int yy=vy-THMap[V1];
		if(yy<vy1){
			FactureWeight[V]=0;
			if(V+VertInLine<MaxPointIndex)FactureWeight[V+VertInLine]=0;
			if(V+VertInLine*2<MaxPointIndex)FactureWeight[V+VertInLine*2]=0;
			return true;
		}
		V1+=VertInLine;
	}
	return false;
}
int BeregTexU=24;
int BeregTexD=31;
int BeregFact=8;
int MinH=-80;
void ReadBeregParams(){
	GFILE* F=Gopen("bereg.txt","r");
	if(F){
		Gscanf(F,"%d%d%d",&BeregTexU,&BeregTexD,&BeregFact);
		Gclose(F);
	}
}
void DrawHeightRect(int xc,int yc,int r,int H){
	ReadBeregParams();
	yc=Prop43(yc);
	int xx=xc>>5;
	int yy=yc>>5;
	int n=(r>>5)+15;
	for(int dx=xx-n;dx<=xx+n;dx++){
		for(int dy=yy-n;dy<=yy+n;dy++){
			if(dx>=0&&dx<VertInLine){
				if(dy>=0&&dy<VertInLine){
                    int V=dx+dy*VertInLine;
					int vx=dx<<5;
					int vy=dy<<5;
					if(dx&1)vy-=16;
					vx-=xc;
					vy-=yc;
					int M1=256+128;
					int M2=256-64;

					int HH[4];
					int ldx,ldy;

					HH[0]=(vy+vx-r)*M2/256;
					HH[1]=(vy-vx-r)*M2/256;
					HH[2]=(vx-vy-r)*M1/256;
					HH[3]=(-vy-vx-r)*M1/256;
					int midx=0;
					int mv=-10000;
					for(int j=0;j<4;j++){
						if(HH[j]>mv){
                            mv=HH[j];
							midx=j;
						}
					}
					if(mv<MinH)mv=MinH;
					int DH;
					int D2=(midx==2||midx==3)?100:30;
					if((DH=mv-THMap[V])<D2){
						if(DH<0)THMap[V]=mv;
						int L0=120;
						int L1=GetLighting3DV(0,0,64);
						int DH0=DH;
						if(DH<0)DH=0;
						int TW=255*(D2-DH)/D2;
						FactureMap[V]=BeregFact;
						FactureWeight[V]=TW;
						void AssignExTex2(int V,byte tex1,byte tex2,int W);
						AssignExTex2(V,TexMap[V],BeregTexU,TW);
						int H=THMap[V]+(rand()&31)+70;
						int HH=100;
						if(H<HH&&H>MinH){
							if(H<0)H=0;
							H=(HH-H)*255/50;
							AssignExTex2(V,TexMap[V],BeregTexD,H);
						}
						switch(midx){
							case 2:							
								if(H>MinH)AddToLightHash(V,L1+(L0-L1)*(D2-DH)/D2);
							break;
							case 3:
								//AddToLightHash(V,GetLighting3DV(-64,64,64));
							break;
						}
                        MarkPointToDraw(V);
						void ResetWaterSys();						
						ResetWaterSys();
					}					
				}
			}
		}
	}
	for(dx=xx-n;dx<=xx+n;dx++){
		for(int dy=yy-n;dy<=yy+n;dy++){
			if(dx>=0&&dx<VertInLine){
				if(dy>=0&&dy<VertInLine){
                    int V=dx+dy*VertInLine;
					if(CheckFactureWeightInPoint(V)){
						//if(V>VertInLine)FactureWeight[V-VertInLine]=0;
						MarkPointToDraw(V);
					}
				}
			}
		}
	}
	Lpressed=0;
}
void DrawHeightRect1(int xc,int yc,int r,int H){
	ReadBeregParams();
	yc=Prop43(yc);
	int xx=xc>>5;
	int yy=yc>>5;
	int n=(r>>5)+15;
	for(int dx=xx-n;dx<=xx+n;dx++){
		for(int dy=yy-n;dy<=yy+n;dy++){
			if(dx>=0&&dx<VertInLine){
				if(dy>=0&&dy<VertInLine){
                    int V=dx+dy*VertInLine;
					int vx=dx<<5;
					int vy=dy<<5;
					if(dx&1)vy-=16;
					vx-=xc;
					vy-=yc;
					int M1=256-64;
					int M2=256+128;

					int HH[4];
					int ldx,ldy;

					HH[0]=-(vy+vx-r)*M2/256;
					HH[1]=-(vy-vx-r)*M2/256;
					HH[2]=-(vx-vy-r)*M1/256;
					HH[3]=-(-vy-vx-r)*M1/256;
					int midx=0;
					int mv=10000;
					for(int j=0;j<4;j++){
						if(HH[j]<mv){
                            mv=HH[j];
							midx=j;
						}
					}
					if(mv>180)mv=180;
					int DH;
					int D2=(midx==2||midx==3)?100:30;
					if((DH=mv-THMap[V])>-D2){
						if(DH>0)THMap[V]=mv;
						int L0=120;
						int L1=GetLighting3DV(0,0,64);
						int DH0=DH;
						if(DH<0)DH=0;
						int TW=255*(D2-DH)/D2;
						FactureMap[V]=BeregFact;
						FactureWeight[V]=TW;
						void AssignExTex2(int V,byte tex1,byte tex2,int W);
						AssignExTex2(V,TexMap[V],BeregTexU,TW);
						int H=THMap[V]+(rand()&31)+70;
						int HH=100;
						if(H<HH){
							if(H<0)H=0;
							H=(HH-H)*255/50;
							AssignExTex2(V,TexMap[V],BeregTexD,H);
						}
						switch(midx){
							case 2:							
								AddToLightHash(V,L1+(L0-L1)*(D2-DH)/D2);
							break;
							case 3:
								//AddToLightHash(V,GetLighting3DV(-64,64,64));
							break;
						}
                        MarkPointToDraw(V);
						void ResetWaterSys();						
						ResetWaterSys();
					}					
				}
			}
		}
	}
	for(dx=xx-n;dx<=xx+n;dx++){
		for(int dy=yy-n;dy<=yy+n;dy++){
			if(dx>=0&&dx<VertInLine){
				if(dy>=0&&dy<VertInLine){
                    int V=dx+dy*VertInLine;
					if(CheckFactureWeightInPoint(V)){
						//if(V>VertInLine)FactureWeight[V-VertInLine]=0;
						MarkPointToDraw(V);
					}
				}
			}
		}
	}
	Lpressed=0;
}