#pragma once
#include "FPath\FPathFinder.h"
class MotionBrush{
public:
	int* Data;
	int Lx,Ly;
	int Dx;
	int Dy;
	MotionBrush();
	~MotionBrush();
	void CreateRoundBrush(int d);
	void CreateSquareBrush(int L);
};
class MotionField : public FPathFinder{
public:	
	//byte* MapH;//(x,y)-bit matrix(1point=16x12 pix)
	byte* MapV;//(y,x)-T bit matrix
	//----Methods-----
	void Allocate();
	void FreeAlloc();
	void ClearMaps();
	void BSetPt(int x,int y);
	void BClrPt(int x,int y);
	void BXorPt(int x,int y);
	void BSetBar(int x,int y,int Lx);
	void BClrBar(int x,int y,int Lx);
	void BXorBar(int x,int y,int Lx);
	void BSetSQ(int x,int y,int Lx,int Ly);
	void BClrSQ(int x,int y,int Lx,int Ly);
	void BXorSQ(int x,int y,int Lx,int Ly);
	__forceinline int CheckPt(int x,int y){
		int MAPV=int(MapV);
		if(x>=0&&x<MAPSX&&y>=0&&y<MAPSY){
			switch(ADDSH){
			case 1:
				__asm{
					mov		eax,y
					mov		ecx,eax
					and		ecx,7
					shr		eax,3
					mov		ebx,x
					shl		ebx,7//MAPSHF
					add		ebx,eax
					mov		al,1
					shl		al,cl
					add		ebx,MAPV
					and		al,[ebx]
					and     eax,0xFF
				};
				break;
			case 2:
				__asm{
					mov		eax,y
					mov		ecx,eax
					and		ecx,7
					shr		eax,3
					mov		ebx,x
					shl		ebx,8//MAPSHF
					add		ebx,eax
					mov		al,1
					shl		al,cl
					add		ebx,MAPV
					and		al,[ebx]
					and     eax,0xFF
				};
				break;
			case 3:
				__asm{
					mov		eax,y
					mov		ecx,eax
					and		ecx,7
					shr		eax,3
					mov		ebx,x
					shl		ebx,9//MAPSHF
					add		ebx,eax
					mov		al,1
					shl		al,cl
					add		ebx,MAPV
					and		al,[ebx]
					and     eax,0xFF
				};
				break;
			};
			return;
		}else return 1;	
	};
	__forceinline int CheckHLine(int x,int y,int Lx){
		for(int i=0;i<Lx;i++)if(CheckPt(x+i,y))return 1;
		return 0;	
	};
	int  CheckVLine(int x,int y,int Lx);
	bool CheckBar(int x,int y,int Lx,int Ly);
	void FillNewCirc(int x,int y,int Lx,int Ly);
	bool CheckRound(int x,int y,int D);
	void SetRound(int x,int y,int D);
	void ClearRound(int x,int y,int D);
	void XorRound(int x,int y,int D);
	//----FPATH----
	int CurLx;
	virtual bool fCheckPt(int x, int y){
		if(CurLx==1)return !CheckPt(x,y);
		else return !CheckRound(x-2,y-2,CurLx+4);		
	}
	virtual bool fbCheckPt(int x, int y);
	virtual void fbBSetPt(int x, int y);
	virtual void fbBClrPt(int x, int y);
};

#define NMFIELDS 4	

extern MotionField MFIELDS[NMFIELDS];//0-Land 1-Water 2-low air 3-high air
