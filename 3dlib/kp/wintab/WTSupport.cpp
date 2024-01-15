#include <windows.h>
#include "wintab.h"
LOGCONTEXT lc;
POINT csr;
HCTX hCtx;

bool WT_IS_INIT=0;
int CurPressure=0;
bool PenBackSide=false;
bool PenPressed=false;

#define PACKETDATA	PK_X | PK_Y | PK_BUTTONS | PK_NORMAL_PRESSURE | PK_STATUS
#define PACKETMODE	0
#include "pktdef.h"
//extern HWND hwnd;
void InitWinTab(HWND h){	
	WTInfo( WTI_DEFCONTEXT, 0, &lc );
	lc.lcPktData = PACKETDATA;
	lc.lcPktMode = PACKETMODE;
	lc.lcOptions = CXO_SYSTEM;
	hCtx = WTOpen( h, &lc, TRUE );
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
			PenBackSide=pkt.pkStatus&TPS_INVERT;
			if(CurPressure)PenPressed=true;
		};
	}while(n);
};