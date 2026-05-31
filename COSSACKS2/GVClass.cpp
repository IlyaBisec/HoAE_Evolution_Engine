#include "stdheader.h"
void PreDrawGameProcess();
void PostDrawGameProcess();
SimpleDialog* ParentFrame::addViewPort(int x,int y,int Lx,int Ly){
	SimpleDialog* SD=new SimpleDialog;
	AddDialog(SD);
	SD->x=x+BaseX;
	SD->y=y+BaseY;
	SD->x1=SD->x+Lx;
	SD->y1=SD->y+Ly;
	SD->Enabled=1;
	SD->Parent=NULL;
	return SD;
};
extern int RealLx;
extern int RealLy;
bool MakeClipping(SimpleDialog* SD){
	if(!(SD->Visible))return false;
#ifdef _USE3D
	GPS.SetClipArea( 0, 0, RealLx, RealLy );
#else
	WindX=0;
	WindY=0;
	WindX1=RealLx-1;
	WindY1=RealLy-1;
	WindLx=RealLx;
	WindLy=RealLy;
#endif // _USE3D

	IntersectWindows(SD->x,SD->y,SD->x1,SD->y1);
	return false;
};
SimpleDialog* ParentFrame::addClipper(int x0,int y0,int x1,int y1){
	SimpleDialog* SD=new SimpleDialog;
	AddDialog(SD);
	SD->x=x0;
	SD->y=y0;
	SD->x1=x1;
	SD->y1=y1;
	SD->Enabled=1;
	SD->Visible=1;
	SD->Parent=NULL;
	SD->OnDraw=&MakeClipping;
	return SD;

};