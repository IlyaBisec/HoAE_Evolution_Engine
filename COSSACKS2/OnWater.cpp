#include "stdheader.h"
//x,y-real coordinates
NewMonster* PrevCheckNM=NULL;
int PrevCheckX,PrevCheckY,PC_ResX,PC_ResY;
int PreTime;
extern MotionField UnitsField;
bool CheckPortPlace(NewMonster* NM,int BuiX,int BuiY){
	int L=NM->BuiDist;
	return !(MFIELDS[1].CheckVLine(BuiX-10,BuiY-10,21)||MFIELDS[1].CheckVLine(BuiX+10,BuiY-10,21)||
				MFIELDS[1].CheckVLine(BuiX,BuiY-10,21));
			/*!(MFIELDS[1].CheckBar(BuiX-L-6,BuiY,1,1)  ||
		     MFIELDS[1].CheckBar(BuiX+L+6,BuiY,1,1)  ||
			 MFIELDS[1].CheckBar(BuiX,BuiY+L+6,1,1)  ||
			 MFIELDS[1].CheckBar(BuiX,BuiY-L-6,1,1)  ||
			 MFIELDS[1].CheckBar(BuiX-L-1,BuiY-1,3,3)||
		     MFIELDS[1].CheckBar(BuiX+L-1,BuiY-1,3,3)||
			 MFIELDS[1].CheckBar(BuiX-1,BuiY+L-1,3,3)||
			 MFIELDS[1].CheckBar(BuiX-1,BuiY-L-1,3,3)||
			 UnitsField.CheckBar(BuiX-L-1,BuiY-1,3,3)||
			 UnitsField.CheckBar(BuiX+L-1,BuiY-1,3,3)||
			 UnitsField.CheckBar(BuiX-1,BuiY-L-1,3,3)||
			 UnitsField.CheckBar(BuiX-1,BuiY+L-1,3,3));
			 */
};
bool FindPortPlace(NewMonster* NM,int x,int y,int* BuiX,int* BuiY){
	int mx=x>>8;
	int my=y>>8;
	MotionField* MFI=MFIELDS+1;
	int minr=34;
	int maxr=35;
	int MinDist=1000000;
	int BestX=0;
	int BestY=0;
	for(int r=minr;r<maxr;r++){
		int L=(r<<1)+1;
		int xx=mx+r;
		int yy=my-r;
		for(int t=0;t<L;t++){
			if(CheckPortPlace(NM,xx,yy)){
				int dst=Norma(xx-mx,yy-my);
				if(dst<MinDist){
					MinDist=dst;
					BestX=xx;
					BestY=yy;
				};
			};
			yy++;
		};
		xx=mx-r;
		yy=my-r;
		for(t=0;t<L;t++){
			if(CheckPortPlace(NM,xx,yy)){
				int dst=Norma(xx-mx,yy-my);
				if(dst<MinDist){
					MinDist=dst;
					BestX=xx;
					BestY=yy;
				};
			};
			yy++;
		};
		xx=mx-r+1;
		yy=my-r;
		L-=2;
		for(t=0;t<L;t++){
			if(CheckPortPlace(NM,xx,yy)){
				int dst=Norma(xx-mx,yy-my);
				if(dst<MinDist){
					MinDist=dst;
					BestX=xx;
					BestY=yy;
				};
			};
			xx++;
		};
		xx=mx-r+1;
		yy=my+r;
		for(t=0;t<L;t++){
			if(CheckPortPlace(NM,xx,yy)){
				int dst=Norma(xx-mx,yy-my);
				if(dst<MinDist){
					MinDist=dst;
					BestX=xx;
					BestY=yy;
				};
			};
			xx++;
		};
		if(MinDist<r+3){
			*BuiX=BestX;
			*BuiY=BestY;
			return true;
		};
	};
	if(MinDist<1000000){
		*BuiX=BestX;
		*BuiY=BestY;
		return true;
	};
	return false;
};
