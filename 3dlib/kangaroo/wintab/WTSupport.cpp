#include <windows.h>
#include "wintab.h"
LOGCONTEXT lc;
POINT csr;
HCTX hCtx=0;

bool WT_IS_INIT=0;
int CurPressure=0;
bool PenBackSide=false;
bool PenPressed=false;

#define PACKETDATA	PK_X | PK_Y | PK_BUTTONS | PK_NORMAL_PRESSURE | PK_STATUS
#define PACKETMODE	0
#include "pktdef.h"
HMODULE H=NULL;
//extern HWND hwnd;

typedef UINT API _tpWTInfoA(UINT, UINT, LPVOID);
_tpWTInfoA* _WTInfo=NULL;

typedef HCTX API _tpWTOpenA(HWND, LPLOGCONTEXTA, BOOL);
_tpWTOpenA* _WTOpen=NULL;

typedef int API _tpWTPacketsGet(HCTX, int, LPVOID);
_tpWTPacketsGet* _WTPacketsGet=NULL;


void InitWinTab(HWND h){
	H=LoadLibrary("Wintab32.dll");
	if(H){		
		_WTInfo = (_tpWTInfoA*)GetProcAddress(H,"WTInfoA");		
		_WTOpen = (_tpWTOpenA*)GetProcAddress(H,"WTOpenA");		
		_WTPacketsGet = (_tpWTPacketsGet*)GetProcAddress(H,"WTPacketsGet");
		if(_WTInfo && _WTOpen && _WTPacketsGet){
			_WTInfo( WTI_DEFCONTEXT, 0, &lc );
			lc.lcPktData = PACKETDATA;
			lc.lcPktMode = PACKETMODE;
			lc.lcOptions = CXO_SYSTEM;
			hCtx = _WTOpen( h, &lc, TRUE );
		}
	}
};
void ProcessTablet(){
	if(!hCtx)return;
	PACKET pkt;
	int n;
	do{
		n=_WTPacketsGet(hCtx,1,&pkt);
		if(n){
			WT_IS_INIT=1;
			CurPressure=pkt.pkNormalPressure;
			PenBackSide=pkt.pkStatus&TPS_INVERT;
			if(CurPressure)PenPressed=true;
		};
	}while(n);
};