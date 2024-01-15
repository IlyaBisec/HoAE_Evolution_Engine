#include "stdheader.h"
#ifndef _USE3D
// -- Inserted by Silver ---06.02.2003
// -- reason:
//		Clip area should be set by GPS.SetClipArea( x, y, w, h ); call
// -- end of change -- 06.02.2003
extern int WindX; 
extern int WindY;
extern int WindLx;
extern int WindLy;
extern int WindX1;
extern int WindY1; 
#endif // !_USE3D

extern int ScrWidth;

void PushWindow(TempWindow* W){
	W->ScrWidth=ScrWidth;
	
	Rct vp = GPS.GetClipArea();
	W->WindLx	= vp.w;
	W->WindLy	= vp.h;
	W->WindX	= vp.x;
	W->WindX1	= vp.x + vp.w - 1;
	W->WindY	= vp.y;
	W->WindY1	= vp.y + vp.h - 1;
};
void PopWindow(TempWindow* W){
	ScrWidth=W->ScrWidth;
#ifndef _USE3D
	WindLx	= W->WindLx;
	WindLy	= W->WindLy;
	WindX	= W->WindX;
	WindX1	= W->WindX1;
	WindY	= W->WindY;
	WindY1	= W->WindY1;
#else
	GPS.SetClipArea( W->WindX, W->WindY, W->WindLx, W->WindLy );
#endif // !_USE3D
	
};
void IntersectWindows(int x0,int y0,int x1,int y1)
{
	const Matrix4D* M=GPS.GetScreenSpaceTM();
	if(M){
        Vector3D v1(x0,y0,0);
		Vector3D v2(x1,y1,0);
		M->transformPt(v1);
		M->transformPt(v2);
		x0=v1.x;
		y0=v1.y;
		x1=v2.x;
		y1=v2.y;
		if(x1<x0)swap(x1,x0);
		if(y1<y0)swap(y1,y0);
	}
	Rct vp = GPS.GetClipArea();
	int WindX = vp.x;
	int WindY = vp.y;
	int WindLx = vp.w;
	int WindLy = vp.h;
	int WindX1 = WindX + WindLx - 1;
	int WindY1 = WindY + WindLy - 1;

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
	GPS.SetClipArea( WindX, WindY, WindLx, WindLy );
};
void DrawRect(int x0,int y0,int x1,int y1,Corners* CR,int GP_File){
	TempWindow TEMP;
	PushWindow(&TEMP);
	IntersectWindows(x0,y0-32,x1,y1+32);
	int N=(x1-x0+1)>>6;
	for(int i=0;i<=N;i++){
		if(CR->LU!=-1)GPS.ShowGP(x0+(i<<6),y0-32,GP_File,CR->LU,MyNation);
		if(CR->LD!=-1)GPS.ShowGP(x0+(i<<6),y1-32,GP_File,CR->LD,MyNation);
	};
	PopWindow(&TEMP);
	IntersectWindows(x0-32,y0,x1+32,y1);
	N=(y1-y0+1)>>6;
	for(int i=0;i<=N;i++){
		if(CR->LL!=-1)GPS.ShowGP(x0-32,y0+(i<<6),GP_File,CR->LL,MyNation);
		if(CR->LR!=-1)GPS.ShowGP(x1-32,y0+(i<<6),GP_File,CR->LR,MyNation);
	};
	PopWindow(&TEMP);
	if(CR->CLU!=-1)GPS.ShowGP(x0-32,y0-32,GP_File,CR->CLU,MyNation);
	if(CR->CRU!=-1)GPS.ShowGP(x1-32,y0-32,GP_File,CR->CRU,MyNation);
	if(CR->CLD!=-1)GPS.ShowGP(x0-32,y1-32,GP_File,CR->CLD,MyNation);
	if(CR->CRD!=-1)GPS.ShowGP(x1-32,y1-32,GP_File,CR->CRD,MyNation);
	PopWindow(&TEMP);
};
void DrawRect1(int x0,int y0,int x1,int y1,Corners* CR,int GP_File){
	TempWindow TEMP;
	PushWindow(&TEMP);
	IntersectWindows(x0+32,y0-32,x1-33,y1+32);
	int N=(x1-x0+1-64)>>6;
	for(int i=0;i<=N;i++){
		if(CR->LU!=-1)GPS.ShowGP(x0+(i<<6)+32,y0-32,GP_File,CR->LU,MyNation);
		if(CR->LD!=-1)GPS.ShowGP(x0+(i<<6)+32,y1-32,GP_File,CR->LD,MyNation);
	};
	PopWindow(&TEMP);
	IntersectWindows(x0-32,y0+32,x1+32,y1-33);
	N=(y1-y0+1-64)>>6;
	for(int i=0;i<=N;i++){
		if(CR->LL!=-1)GPS.ShowGP(x0-32,y0+(i<<6)+32,GP_File,CR->LL,MyNation);
		if(CR->LR!=-1)GPS.ShowGP(x1-32,y0+(i<<6)+32,GP_File,CR->LR,MyNation);
	};
	PopWindow(&TEMP);
	IntersectWindows(x0-32,y0-32,x1+32,(y1+y0)/2-1);
	if(CR->CLU!=-1)GPS.ShowGP(x0-32,y0-32,GP_File,CR->CLU,MyNation);
	if(CR->CRU!=-1)GPS.ShowGP(x1-32,y0-32,GP_File,CR->CRU,MyNation);
	PopWindow(&TEMP);
	IntersectWindows(x0-32,(y1+y0)/2,x1+32,y1+32);
	if(CR->CLD!=-1)GPS.ShowGP(x0-32,y1-32,GP_File,CR->CLD,MyNation);
	if(CR->CRD!=-1)GPS.ShowGP(x1-32,y1-32,GP_File,CR->CRD,MyNation);
	PopWindow(&TEMP);
};
void DrawRect3(int x0,int y0,int x1,int y1,Corners* CR,int GP_File){
	TempWindow TEMP;
	PushWindow(&TEMP);
	IntersectWindows(x0+32,y0-32,x1-33,y1+32);
	int N=(x1-x0+1-64)>>6;
	for(int i=0;i<=N;i++){
		if(CR->LU!=-1)GPS.ShowGP(x0+(i<<6)+32,y0-32,GP_File,CR->LU,MyNation);
		if(CR->LD!=-1)GPS.ShowGP(x0+(i<<6)+32,y1-32,GP_File,CR->LD,MyNation);
	};
	PopWindow(&TEMP);
	IntersectWindows(x0-32,y0+32,x1+32,y1-33);
	N=(y1-y0+1-64)>>6;
	for(int i=0;i<=N;i++){
		if(CR->LL!=-1)GPS.ShowGP(x0-32,y0+(i<<6)+32,GP_File,CR->LL,MyNation);
		if(CR->LR!=-1)GPS.ShowGP(x1-32,y0+(i<<6)+32,GP_File,CR->LR,MyNation);
	};
	PopWindow(&TEMP);
	IntersectWindows(x0-32,y0-32,x1+32,(y1+y0)/2-1);
	if(CR->CLU!=-1)GPS.ShowGP(x0-32,y0-32,GP_File,CR->CLU,MyNation);
	if(CR->CRU!=-1)GPS.ShowGP(x1-32,y0-32,GP_File,CR->CRU,MyNation);
	PopWindow(&TEMP);
	IntersectWindows(x0-32,(y1+y0)/2,x1+32,y1+32);
	if(CR->CLD!=-1)GPS.ShowGP(x0-32,y1-32,GP_File,CR->CLD,MyNation);
	if(CR->CRD!=-1)GPS.ShowGP(x1-32,y1-32,GP_File,CR->CRD,MyNation);
	PopWindow(&TEMP);
};
void DrawRect4(int x0,int y0,int x1,int y1,Corners* CR,int GP_File){
	TempWindow TEMP;
	PushWindow(&TEMP);

	int ULLX=GPS.GetGPWidth(GP_File,CR->CLU);
	if(ULLX==0)ULLX=32;
	int LX2=ULLX>>1;
	int DLLX;
	if(CR->CLD==-1)DLLX=GPS.GetGPWidth(GP_File,CR->CLU);
	else DLLX=GPS.GetGPWidth(GP_File,CR->CLD);

	if(DLLX==0)DLLX=32;
	int LX3=DLLX>>1;

	int UPLX=GPS.GetGPWidth(GP_File,CR->LU);
	if(UPLX==0)UPLX=32;
	int DNLX=GPS.GetGPWidth(GP_File,CR->LD);
	if(DNLX==0)DNLX=32;

	int LSLY=GPS.GetGPHeight(GP_File,CR->CLU);
	int LLY2=LSLY>>1;

	int LLY3=GPS.GetGPHeight(GP_File,CR->CLD)>>1;

	int LDLY=GPS.GetGPWidth(GP_File,CR->CLD);
	if(LDLY>1000)LDLY=0;
	int LDY2=LDLY>>1;

	int LEFTLY=GPS.GetGPHeight(GP_File,CR->LL);
	if(LEFTLY==0)LEFTLY=32;
	int RIGHTLY=GPS.GetGPHeight(GP_File,CR->LR);
	if(RIGHTLY==0)RIGHTLY=32;

	//horisontal lines
	IntersectWindows(x0+LX2,y0-LLY2,x1-LX2-1,(y1+y0)/2);//LLY2);
	int N=(x1-x0+1-ULLX)/UPLX;
	for(int i=0;i<=N+2;i++){
		if(CR->LU!=-1)GPS.ShowGP(x0+i*UPLX+LX2,y0-LLY2,GP_File,CR->LU,MyNation);
	};
	PopWindow(&TEMP);
	IntersectWindows(x0+LX3,(y1+y0)/2+1,x1-LX3-1,y1+LLY3);//LLY2);
	N=(x1-x0+1-DLLX)/DNLX;
	for(int i=0;i<=N+2;i++){
		if(CR->LD!=-1)GPS.ShowGP(x0+i*DNLX+LX3,y1-LDY2,GP_File,CR->LD,MyNation);
	};
	//vertical lines
	PopWindow(&TEMP);
	IntersectWindows(x0-LX3,y0+LLY2,x1+LX3,y1-LDY2);
	N=(y1-y0+1-LLY2-LDY2)/LEFTLY;
	for(int i=0;i<=N+1;i++){
		if(CR->LL!=-1)GPS.ShowGP(x0-LX3,y0+i*LEFTLY+LLY2,GP_File,CR->LL,MyNation);
	};
	N=(y1-y0+1-LLY2-LDY2)/RIGHTLY;
	for(int i=0;i<=N+1;i++){
		if(CR->LR!=-1)GPS.ShowGP(x1-LX3,y0+i*RIGHTLY+LLY2,GP_File,CR->LR,MyNation);
	};
	//corners
	PopWindow(&TEMP);
	IntersectWindows(x0-LX2,y0-LLY2,(x1+x0)/2-1,(y1-LDY2+y0+LLY2)/2-1);
	if(CR->CLU!=-1)GPS.ShowGP(x0-LX2,y0-LLY2,GP_File,CR->CLU,MyNation);

	PopWindow(&TEMP);
	IntersectWindows((x0+x1)/2,y0-LLY2,x1+LX2,(y1-LDY2+y0+LLY2)/2-1);
	if(CR->CRU!=-1)GPS.ShowGP(x1-LX2,y0-LLY2,GP_File,CR->CRU,MyNation);

	PopWindow(&TEMP);
	IntersectWindows(x0-LX3,(y1-LDY2+y0+LLY2)/2,(x0+x1)/2-1,y1+LLY3);
	if(CR->CLD!=-1)GPS.ShowGP(x0-LX3,y1-LDY2,GP_File,CR->CLD,MyNation);

	PopWindow(&TEMP);
	IntersectWindows((x0+x1)/2,(y1-LDY2+y0+LLY2)/2,x1+LX3,y1+LLY3);
	if(CR->CRD!=-1)GPS.ShowGP(x1-LX3,y1-LDY2,GP_File,CR->CRD,MyNation);
	PopWindow(&TEMP);
};
void DrawFilledRect(int x0,int y0,int x1,int y1,Corners* CR,int GP_File,int StartFill,int NFill){
	int Lx=GPS.GetGPWidth(GP_File,StartFill);
	int Ly=GPS.GetGPHeight(GP_File,StartFill);
	if(Lx&&Ly){
		TempWindow TEMP;
		PushWindow(&TEMP);
		IntersectWindows(x0,y0,x1,y1);
		int nx=(x1-x0)/Lx;
		int ny=(y1-y0)/Ly;
		for(int ix=0;ix<=nx;ix++)
			for(int iy=0;iy<=ny;iy++)
				GPS.ShowGP(x0+(ix*Lx),y0+(iy*Ly),GP_File,StartFill+((ix*ix+iy*iy*iy)%NFill),MyNation);
		PopWindow(&TEMP);
		DrawRect4(x0,y0,x1,y1,CR,GP_File);
	}
}
void DrawFilledRect1(int x0,int y0,int x1,int y1,Corners* CR,int GP_File,int StartFill,int NFill){
	int Lx=GPS.GetGPWidth(GP_File,StartFill);
	int Ly=GPS.GetGPHeight(GP_File,StartFill);
	if(Lx&&Ly){
		TempWindow TEMP;
		PushWindow(&TEMP);
		IntersectWindows(x0,y0,x1,y1);
		int nx=(x1-x0)/Lx;
		int ny=(y1-y0)/Ly;
		for(int ix=0;ix<=nx;ix++)
			for(int iy=0;iy<=ny;iy++)
				GPS.ShowGP(x0+(ix*Lx),y0+(iy*Ly),GP_File,StartFill+((ix*ix+iy*iy*iy)%NFill),MyNation);
		PopWindow(&TEMP);
		DrawRect4(x0,y0,x1,y1,CR,GP_File);
	}
};
CEXPORT
void DrawFilledRect3(int x0,int y0,int x1,int y1,Corners* CR,int GP_File,int StartFill,int NFill){
	int Lx=GPS.GetGPWidth(GP_File,StartFill);
	int Ly=GPS.GetGPHeight(GP_File,StartFill);
	if(Lx&&Ly){
		TempWindow TEMP;
		PushWindow(&TEMP);
		IntersectWindows(x0,y0,x1,y1);
		int nx=(x1-x0)/Lx;
		int ny=(y1-y0)/Ly;
		for(int ix=0;ix<=nx;ix++)
			for(int iy=0;iy<=ny;iy++)
				GPS.ShowGP(x0+(ix*Lx),y0+(iy*Ly),GP_File,StartFill+((ix*ix+iy*iy*iy)%NFill),MyNation);
		PopWindow(&TEMP);
		DrawRect4(x0,y0,x1,y1,CR,GP_File);
	}
};
void DrawHeader(int x0,int y0,int Lx,int GP_File,int Frame){
	TempWindow TEMP;
	PushWindow(&TEMP);
	if(Lx<64)Lx=64;
	if(Lx>64){
		Lx>>=1;
		IntersectWindows(x0-Lx+32,y0-32,x0+Lx-32,y0+32);
		int N=(Lx>>5)+1;
		for(int i=0;i<N;i++)GPS.ShowGP(x0-Lx+(i<<6),y0-32,GP_File,Frame+1,MyNation);
	}else Lx>>=1;
	PopWindow(&TEMP);
	GPS.ShowGP(x0-32-Lx,y0-32,GP_File,Frame,MyNation);
	GPS.ShowGP(x0-32+Lx,y0-32,GP_File,Frame+2,MyNation);
};
void DrawHeaderEx2(int x0,int y0,int Lx,int GP_File,int FrameL,int FrameR,int FrameC1,int FrameC2,int FrameC3, byte Nation){
	TempWindow TEMP;
	PushWindow(&TEMP);
	if(Lx<24)Lx=24;
	//if(Lx>64){
	//	Lx>>=1;
	//  Виталик, я это-таки испортил, своими грязными руками (Руслан)
	int frWidthL = GPS.GetGPWidth(GP_File,FrameL);
	int frWidthR = GPS.GetGPWidth(GP_File,FrameR);
	if (frWidthR > 2048) frWidthR = 0;
	if (frWidthL > 2048) frWidthL = 0;
	IntersectWindows(x0+frWidthL,y0-32,x0+Lx-frWidthR,y0+128/*32*/);
	//  --- конец надругательства

		int N=0;
		for(int i=0;i<Lx&&N<300;){
			N++;
			int spr;
			switch(i%3){
				case 0:
					spr=FrameC1;
					break;
				case 1:
					spr=FrameC2;
					break;
				case 2:
					spr=FrameC3;
					break;
			}
			GPS.ShowGP(x0+i,y0,GP_File,spr,Nation);
			i+=GPS.GetGPWidth(GP_File,spr);
		}
	//}else Lx>>=1;
	PopWindow(&TEMP);
	GPS.ShowGP(x0,y0,GP_File,FrameL,Nation);
	GPS.ShowGP(x0+Lx-frWidthR,y0,GP_File,FrameR,Nation);
};
void DrawHeaderEx(int x0,int y0,int Lx,int GP_File,int FrameL,int FrameR,int FrameC1,int FrameC2,int FrameC3){
	DrawHeaderEx2(x0, y0, Lx, GP_File, FrameL, FrameR, FrameC1, FrameC2, FrameC3, MyNation);
}
void DrawVScroller(int x,int y,int Ly,int GP_File,int Up,int Down,int Center,int Ncenter){
	TempWindow TEMP;
	PushWindow(&TEMP);
	int ULY=GPS.GetGPHeight(GP_File,Up);
	IntersectWindows(x-64,y+ULY,x+64,y+Ly-1-ULY);
	int CLY=GPS.GetGPHeight(GP_File,Center);
	
	if(CLY){
		int N=Ly/CLY;
		for(int i=0;i<=N;i++)GPS.ShowGP(x,y+i*CLY+ULY,GP_File,Center+(i%Ncenter),0);
	};
	PopWindow(&TEMP);
	GPS.ShowGP(x,y,GP_File,Up,0);
	GPS.ShowGP(x,y+Ly-GPS.GetGPHeight(GP_File,Down),GP_File,Down,0);
};
//---------------------Using----------------------//
Corners StdRect=  {0,2,6,8,12,14,9,11};
Corners StdRectV=  {0,1,2,3,4,5,6,7};
Corners LeftRect= {0,-1,6,-1,12,-1,9,11};
Corners RightRect={1,2,7,8,13,14,9,11};
Corners UpRect=   {0,2,-1,-1,12,14,9,-1};
Corners DnRect=   {3,5,6,8,12,14,10,11};
Corners Table=    {17,18,19,20,21,22,23,24};
Corners UpTable=  {17,18,-1,-1,12,14,23,-1};
Corners UpTable1= {17,87,-1,-1,12,36,23,-1};
Corners DnTable=  {3,5,6,8,12,14,10,11};
Corners DnTable1= {3,22,6,24,12,36,10,11};
Corners DnTable2= {3,86,6,8,12,14,10,11};
Corners Table1=   {17,18,6,24,12,36,23,11};
Corners Paper=    {43,44,45,46,49,50,47,48};
Corners HintPan=  {58,59,60,61,62,63,64,65};
Corners UpTable3= {0,2,-1,-1,12,14,9,-1};
Corners DnTable3= {3,5,6,8,12,14,10,11};
Corners HdrTbl=   {17,18,6,8,12,14,23,11};
//Corners for the statistics panel
Corners ST_Top=   {17,18,3,5,12,14,23,10};
Corners ST_Middle={-1,-1,68,69,12,14,-1,70};
Corners ST_Bottom={-1,-1,6,8,12,14,-1,11};
Corners ST_Left  ={17,-1,6,-1,12,-1,23,11};
Corners ST_Right ={23/*wrong!*/,18,7,8,13,14,23,11};
//-----PALETTE 2---------//
Corners P2_NORMAL={0,1,2,3,4,5,6,7};
int BordGP2=-1;
void DrawHdrTable(int x0,int y0,int x1,int y1){
	DrawFilledRect1(x0,y0,x1,y1,&HdrTbl,BordGP,28,3);
};
void DrawStdRect2(int x0,int y0,int x1,int y1,int GP){
	DrawRect4(x0,y0,x1,y1,&P2_NORMAL,GP);
};
CEXPORT
void DrawStdBar2(int x0,int y0,int x1,int y1,int GP){
	DrawFilledRect1(x0,y0,x1,y1,&P2_NORMAL,GP,11,3);
};
void DrawStdBar12(int x0,int y0,int x1,int y1,int GP){
	DrawFilledRect1(x0,y0,x1,y1,&P2_NORMAL,GP,11,3);
};


int BordGP=-1;
int BordGPv=-1;
int BordGPh=-1;
StdBorder* Brd_Std=NULL;
StdBorder* Brd_Hint=NULL;

void DrawStdHeader(int xc,int yc,int Lx){
	DrawHeader(xc,yc,Lx,BordGP,25);
};
void DrawStdRect(int x0,int y0,int x1,int y1){
	Brd_Std=BScope.Get("StandartBorder");
	if(Brd_Std) DrawRect4(x0,y0,x1,y1,&Brd_Std->Edges,Brd_Std->GP_File);
	else DrawRect4(x0,y0,x1,y1,&StdRectV,BordGPv);	
};
void DrawStdBar(int x0,int y0,int x1,int y1){
	//Brd_Std=BScope.Get("StandartBorder");
	Brd_Std=BScope.Get("newRama");	
	if(Brd_Std) DrawFilledRect1(x0,y0,x1,y1,&Brd_Std->Edges,Brd_Std->GP_File,Brd_Std->StartFiller,Brd_Std->NFillers);
	else DrawFilledRect1(x0,y0,x1,y1,&StdRectV,BordGPv,8,3);
};
void DrawStdBar1(int x0,int y0,int x1,int y1){
	Brd_Std=BScope.Get("newRama");	
	if(Brd_Std) DrawFilledRect1(x0,y0,x1,y1,&Brd_Std->Edges,Brd_Std->GP_File,Brd_Std->StartFiller,Brd_Std->NFillers);
	else DrawFilledRect1(x0,y0,x1,y1,&StdRectV,BordGPv,8,3);
};
void DrawStdBar2(int x0,int y0,int x1,int y1){
	//Brd_Std=BScope.Get("frameInventar");
	Brd_Std=BScope.Get("Hint");
	if(Brd_Std) DrawFilledRect1(x0,y0,x1,y1,&Brd_Std->Edges,Brd_Std->GP_File,Brd_Std->StartFiller,Brd_Std->NFillers);
	else DrawFilledRect1(x0,y0,x1,y1,&StdRectV,BordGPv,8,3);
};
void DrawLeftRect(int x0,int y0,int x1,int y1){
	DrawRect4(x0,y0,x1,y1,&LeftRect,BordGP);
};
void DrawRightRect(int x0,int y0,int x1,int y1){
	DrawRect4(x0,y0,x1,y1,&RightRect,BordGP);
};
void DrawUpRect(int x0,int y0,int x1,int y1){
	DrawRect4(x0,y0,x1,y1,&UpRect,BordGP);
};
void DrawDnRect(int x0,int y0,int x1,int y1){
	DrawRect4(x0,y0,x1,y1,&DnRect,BordGP);
};
void DrawTable(int x0,int y0,int x1,int y1){
	DrawFilledRect1(x0,y0,x1,y1,&Table,BordGP,28,3);
	DrawHeader((x0+x1)>>1,y0,150,BordGP,25);
};
void DrawDoubleTable(int x0,int y0,int x1,int y1,int ym){
	DrawFilledRect1(x0,y0,x1,ym,&UpTable,BordGP,28,3);
	DrawFilledRect1(x0,ym,x1,y1,&DnTable,BordGP,28,3);
	//DrawHeader((x0+x1)>>1,y0,150,BordGP,25);
};
void DrawDoubleTable2(int x0,int y0,int x1,int y1,int ym){
	DrawFilledRect1(x0,y0,x1,ym,&UpTable3,BordGP,28,3);
	DrawFilledRect1(x0,ym,x1,y1,&DnTable3,BordGP,28,3);
	//DrawHeader((x0+x1)>>1,y0,150,BordGP,25);
};
void DrawScrollDoubleTable(int x0,int y0,int x1,int y1,int ym){
	DrawRect4(x0,y0,x1,ym,&UpTable1,BordGP);
	DrawFilledRect1(x0,ym,x1,y1,&DnTable1,BordGP,28,3);
	//DrawHeader((x0+x1)>>1,y0,150,BordGP,25);
};
void DrawScrollDoubleTableF(int x0,int y0,int x1,int y1,int ym){
	DrawFilledRect1(x0,y0,x1,ym,&UpTable,BordGP,28,3);
	DrawFilledRect1(x0,ym,x1,y1,&DnTable1,BordGP,28,3);
	//DrawHeader((x0+x1)>>1,y0,150,BordGP,25);
};
void DrawScrollDoubleTable1(int x0,int y0,int x1,int y1,int ym){
	DrawFilledRect1(x0,y0,x1,ym,&UpTable1,BordGP,28,3);
	DrawRect4(x0,ym,x1,y1,&DnTable2,BordGP);
	//DrawHeader((x0+x1)>>1,y0,150,BordGP,25);
};
void DrawScrollTable1(int x0,int y0,int x1,int y1,int ym){
	DrawFilledRect1(x0,y0,x1,ym,&Table1,BordGP,28,3);
	//DrawRect4(x0,ym,x1,y1,&DnTable2,BordGP);
	//DrawHeader((x0+x1)>>1,y0,150,BordGP,25);
};
void DrawPaperPanel(int x0,int y0,int x1,int y1){
	DrawFilledRect1(x0,y0,x1,y1,&Paper,BordGP,51,3);
};
void DrawPaperPanelShadow(int x0,int y0,int x1,int y1){
	DrawRect4(x0,y0,x1,y1,&Paper,BordGP);
};
void CBar(int x,int y,int Lx,int Ly,byte c);
void DrawHintPattern(int x0,int y0,int x1,int y1){
	Brd_Hint=BScope.Get("Hint");
	if(Brd_Hint) DrawFilledRect(x0,y0,x1,y1,&Brd_Hint->Edges,Brd_Hint->GP_File,Brd_Hint->StartFiller,Brd_Hint->NFillers);
		else DrawFilledRect(x0,y0,x1,y1,&StdRectV,BordGPh,8,3);
};
void DrawStatTable(int x0,int y0,int Lx,int GraphLy,int LineLy,int NLines){
	int y1=y0+GraphLy+LineLy*NLines;
	if(NLines){
		DrawFilledRect1(x0,y1-LineLy,x0+Lx-1,y1,&ST_Bottom,BordGP,28,3);
		y1-=LineLy;
		for(int i=1;i<NLines;i++){
			DrawFilledRect1(x0,y1-LineLy,x0+Lx-1,y1,&ST_Middle,BordGP,28,3);
			y1-=LineLy;
		};
		CBar(x0,y0+18,Lx,y1-y0+1-18,0x38);
		//DrawFilledRect1(x0,y0,x0+Lx-1,y1,&ST_Top,BordGP,51,1);
		DrawRect4(x0,y0,x0+Lx-1,y1,&ST_Top,
			BordGP);
	};
};
void DrawStatTable1(int x0,int y0,int x1,int y1,int xm){
	DrawFilledRect1(x0,y0,xm,y1,&ST_Left,BordGP,28,3);
	DrawFilledRect1(xm,y0,x1,y1,&ST_Right,BordGP,28,3);
};
void DrawStHeader(int x,int y,int Lx){
	DrawHeader(x,y,Lx,BordGP,71);
};
extern BordersScope BScope;
void LoadBorders(){
	BordGP=GPS.PreLoadGPImage("Border");
	BordGP2=GPS.PreLoadGPImage("Interface\\Bor2");
	BordGPv=GPS.PreLoadGPImage("interf3\\b01");
	BordGPh=GPS.PreLoadGPImage("interf3\\tips01");
	Enumerator* E=ENUM.Get("BORDERS");
	if(E){
		Brd_Std=BScope.Get(E->Get("StandartBorder"));
		Brd_Hint=BScope.Get(E->Get("Hint"));
	}
};
