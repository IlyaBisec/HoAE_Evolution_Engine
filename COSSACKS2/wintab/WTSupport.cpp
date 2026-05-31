#include <windows.h>
#include "wintab.h"

LOGCONTEXT lc;
POINT csr;
HCTX hCtx;
bool WT_IS_INIT=0;
int CurPressure=0;
#define PACKETDATA	PK_X | PK_Y | PK_BUTTONS | PK_NORMAL_PRESSURE | PK_STATUS
#define PACKETMODE	0
#include "pktdef.h"
extern HWND hwnd;
void InitWinTab(){
	WTInfo( WTI_DEFCONTEXT, 0, &lc );
	lc.lcPktData = PACKETDATA;
	lc.lcPktMode = PACKETMODE;
	lc.lcOptions = CXO_SYSTEM;//CXO_MESSAGES;
	hCtx = WTOpen( hwnd, &lc, TRUE );
};
extern bool Lpressed;
extern bool realLpressed;
extern int mouseX;
extern int mouseY;
extern int RealLx;
extern int RealLy;
bool PenBackSide=0;
void OnWTPacket(WPARAM wSerial, LPARAM hCtx){
	/*
	WT_IS_INIT=1;
	PACKET pkt;
	WTPacket( (HCTX)hCtx, wSerial, &pkt );
	CurPressure=pkt.pkNormalPressure;
	mouseX=(RealLx*pkt.pkX)/lc.lcInExtX;
	mouseY=RealLy-(RealLy*pkt.pkY)/lc.lcInExtY;
	SetCursorPos(mouseX,mouseY);
	PenBackSide=pkt.pkStatus&TPS_INVERT;
	if(CurPressure){
		Lpressed=1;
		realLpressed=1;
	};
	*/
};
void ProcessTablet(){
	if(!hCtx)return;
	PACKET pkt;
	int n;
	do{
		n=WTPacketsGet(hCtx,1,&pkt);
		if(n){
			WT_IS_INIT=1;
			CurPressure=pkt.pkNormalPressure;
			//mouseX=(RealLx*pkt.pkX)/lc.lcInExtX;
			//mouseY=RealLy-(RealLy*pkt.pkY)/lc.lcInExtY;
			//SetCursorPos(mouseX,mouseY);
			PenBackSide=pkt.pkStatus&TPS_INVERT;
			//if(CurPressure){
			//	Lpressed=1;
			//	realLpressed=1;
			//};
		};
	}while(n);
};