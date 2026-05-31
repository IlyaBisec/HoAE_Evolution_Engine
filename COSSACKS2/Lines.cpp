/* Horisontal and vertical lines,rectangles*/
#include "stdheader.h"
void Hline(int x,int y,int xend,DWORD c)
{
	extern PALETTEENTRY GPal[256];
	float fx = x, fy = y, fxend = xend;
	if (!GPS.GetClipArea().ClipHLine( fx, fxend, fy )) return;
	GPS.DrawLine( fx, fy, fxend, fy, c);
};

extern PALETTEENTRY GPal[256];

void Vline(int x,int y,int yend,DWORD c)
{
	float fx = x, fy = y, fyend = yend;
	if (!GPS.GetClipArea().ClipVLine( fx, fy, fyend )) return;
	GPS.DrawLine( fx, fy, fx, fyend,  c );
};

void Xbar(int x,int y,int lx,int ly,DWORD c)
{
	GPS.DrawRect( x, y, lx - 1, ly - 1, c );
};

void DrawLine(int x,int y,int x1,int y1,DWORD c)
{
	GPS.DrawLine( x, y, x1, y1, c );
	return;
};
