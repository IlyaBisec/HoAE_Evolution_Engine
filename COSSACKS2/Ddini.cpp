/***********************************************************************
 *Direct Draw initialisation module                    
 *
 * This module creates the Direct Draw object with the primary surface
 * and a backbuffer and sets 800x600x8 display mode.
 *
 ***********************************************************************/
#include "stdheader.h"
#define __ddini_cpp_
#define _WINSOCKAPI_
#include <malloc.h>
#include "ddini.h"
#include "ResFile.h"
#include "FastDraw.h" 
#include "mode.h" 
#include "MapDiscr.h" 
#include "fog.h"
#include "GSound.h"
#include "fonts.h" 
#include "GameSettings.h"
#include "ITerrain.h"
#include "IShadowManager.h"

inline bool ProcessMessages2()
{    
    MSG         msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
        if (msg.message == WM_QUIT)
        {			
            return true;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }	
    return false;
}; // ProcessMessages

CEXPORT bool ProcessWindowMessages()
{
	static int p=0;
	if(!p){
		void InternalProcess();
		p=1;
		InternalProcess();
		p=0;
	}
    return false;//ProcessMessages2();
}

void DDLog (LPSTR sz,...)
{
	
        char ach[256];
        va_list va;

        va_start( va, sz );
        vsprintf ( ach, sz, va );   
        va_end( va );
		FILE* f=fopen("DDraw.log","a");
		fprintf(f,ach);
		fclose(f);
	
};
 
#include "GP_Draw.h"
int				VBUF;
 
extern GP_System GPS;

void Rept (LPSTR sz,...);
CEXPORT int ModeLX[32];
CEXPORT int ModeLY[32];
CEXPORT int NModes=0;
void SERROR();
void SERROR1();
void SERROR2();
void PropCopy();
void InitRLCWindows();
//#define COPYSCR
const int InitLx=1024;
const int InitLy=768;
CEXPORT int RealLx;
CEXPORT int RealLy;
CEXPORT int SCRSizeX;
CEXPORT int SCRSizeY;
CEXPORT int RSCRSizeX;
CEXPORT int RSCRSizeY;
CEXPORT int COPYSizeX;
CEXPORT int Pitch;
LPDIRECTDRAW            lpDD=NULL;      // DirectDraw object
LPDIRECTDRAWSURFACE     lpDDSPrimary;   // DirectDraw primary surface
LPDIRECTDRAWSURFACE     lpDDSBack;      // DirectDraw back surface
BOOL                    bActive;        // is application active?
BOOL                    CurrentSurface; //=FALSE if backbuffer
                                        // is active (Primary surface is visible)
										//=TRUE if  primary surface is active
										// (but backbuffer is visible)
BOOL                    DDError;        //=FALSE if Direct Draw works normally 
DDSURFACEDESC           ddsd;
PALETTEENTRY            GPal[256];
LPDIRECTDRAWPALETTE     lpDDPal;
BOOL					DDDebug;
extern bool PalDone;
extern word PlayerMenuMode;
typedef struct zzz{			
	BITMAPINFO bmp;
	PALETTEENTRY XPal[255];
};
CEXPORT
byte GetPaletteColor(int r,int g,int b){
	int dmax=10000;
	int bestc=0;
	for(int i=0;i<256;i++){
		int d=abs(r-GPal[i].peRed)+abs(g-GPal[i].peGreen)+abs(b-GPal[i].peBlue);
		if(d<dmax){
			dmax=d;
			bestc=i;
		};
	};
	return bestc;
};
zzz xxt;
//typedef byte barr[ScreenSizeX*ScreenSizeY];
void* offScreenPtr;
/*
 * Flipping Pages
 */
//extern int BestBPP;
extern int SCRSZY;
void ClearRGB(){
	if(!bActive)return;
	memset(RealScreenPtr,0,RSCRSizeX*SCRSZY);
};
//#define TEST16
#ifndef TEST16
bool test16=1;
#endif
//#ifdef TEST16
//bool test16=1;
word PAL16[256];
int P16Idx=-1;
extern int CurPalette;
void CheckPal16(){
	//if(CurPalette!=P16Idx&&CurPalette==0){
		P16Idx=CurPalette;
		for(int i=0;i<256;i++)PAL16[i]=(GPal[i].peBlue>>3)+((GPal[i].peGreen>>2)<<5)+((GPal[i].peRed>>3)<<11);
	//};
};
int NATT=10;
void CheckPal16x(){
	for(int i=0;i<256;i++)PAL16[i]=(GPal[i].peBlue>>3)+((GPal[i].peGreen>>2)<<5)+((GPal[i].peRed>>3)<<11);
};
void ChangeColorFF(){
	if(!RealScreenPtr)return;
	try{
		int DD=10000;
		int c=0xFF;
		for(int i=0;i<255;i++){
			int D=255+255+255-GPal[i].peBlue-GPal[i].peGreen-GPal[i].peRed;
			if(D<DD){
				c=i;
				DD=D;
			};
		};
		int sz=RSCRSizeX*RealLy;
		for(i=0;i<sz;i++)((byte*)RealScreenPtr)[i]=0;//if(((byte*)RealScreenPtr)[i]==0xFF)((byte*)RealScreenPtr)[i]=c;
	}catch(...){};
};
void Copy16(byte* Src,int SrcPitch,byte* Dst,int DstPitch,int Lx,int Ly){
	CheckPal16x();
	int DXR=DstPitch-(Lx<<1);
	int XDX=(SrcPitch-Lx);
	int NY=Ly;
	int NX=Lx>>1;
	__asm{
		push esi
		push edi
		mov  esi,Src
		mov  edi,Dst
LPP0:
		mov  ecx,NX
		xor  eax,eax
LPP1:
		mov  bx,[esi]
		mov  al,bl
		mov  dx,[PAL16+eax*2];
		rol  edx,16
		mov  al,bh
		mov  dx,[PAL16+eax*2];
		rol  edx,16
		mov  [edi],edx
		add  edi,4
		add  esi,2
		dec  ecx
		jnz  LPP1
		add  edi,DXR
		add  esi,XDX
		dec  NY
		jnz  LPP0
		pop  edi
		pop  esi
	}
};
//#endif
void TestMem(){
	int NSF,NEF,NEXC;
	int CheckMemoryIntegrityEx(int&,int&,int&);
	int NE=CheckMemoryIntegrityEx(NSF,NEF,NEXC);
	if(NE){
		FlipPages();		
	}
}
extern bool LockMouse;
extern bool KeysAreFocusedOnInputBox;
extern bool WasAtLeastOneTimeInInputBox;
int LastFlipTime=0;
void _dbgPrintState(const char* var,char* mask,...);
extern word NPlayers;
extern bool GameInProgress;
extern int TimeInDrawPrim;
int LimitedDipsCount=1000000;
void DrawPgb(int x,int y,float T){
    DWORD C[4]={0xF0000090,0xF0009000,0xF0909000,0xF0900000};
    float D=100;
    for(int i=0;i<4;i++){
        float p=T*D;
        D/=10;
        if(p>1)p=1;
        if(p<0.1)p=0;
        if(p>0)GPS.DrawFillRect(x+i*22,y,22*p,14,C[i]);
        if(p<1)GPS.DrawFillRect(x+i*22+22*p,y,22-22*p,14,0xA0000000);
    }    
}
//#define __showT(ID,Var)extern int Var;DrawPgb(RealLx-48-40, 15*Nlines,float(Var*1000.0f/Freq));sprintf( strfps, #ID": %.03f", float(Var*1000.0f/Freq) );ShowString( RealLx-40-40, 15*(Nlines++), strfps, &SmallWhiteFont );AllTime+=Var;Var=0;
#define __showT(ID,Var)extern int Var;DrawPgb(RealLx-48-40, 15*Nlines,float(Var*1000.0f/Freq));sprintf( strfps, #ID": %d", Var);ShowString( RealLx-40-40, 15*(Nlines++), strfps, &SmallWhiteFont );AllTime+=Var;Var=0;
int FlipTime=0;
int AllTime=0;
extern int MaxTerrTris;
CEXPORT void FlipPages(void){

	_dbgPrintState("NPlayers","%d",NPlayers);
	_dbgPrintState("CurrentMap","%s",GSets.CGame.cgi_CurrentMap);
	_dbgPrintState("GameInProgress","%d",int(GameInProgress));	

	if(GSets.SVOpt.MusicVolume<8)GSets.SVOpt.MusicVolume=8;

	void ProfilerOnFrame();
	ProfilerOnFrame();

	void DrawGlobalHint();
	DrawGlobalHint();
	LastFlipTime=GetTickCount();

	if(rand()<100)POOL.pool_08.KillDirtyGarbage();
	if(rand()<100)POOL.pool_16.KillDirtyGarbage();
	if(rand()<100)POOL.pool_32.KillDirtyGarbage();
	if(rand()<100)POOL.pool_64.KillDirtyGarbage();
	if(rand()<100)POOL.pool_128.KillDirtyGarbage();

	KeysAreFocusedOnInputBox=WasAtLeastOneTimeInInputBox;
	WasAtLeastOneTimeInInputBox=0;
	extern bool IgnoreProcessMessages;
	IgnoreProcessMessages=0;
    ProcessMessages();
	LockMouse=false;
	GPS.SetScreenSpaceTM(NULL);

	static DWORD lightID = IMM->GetNodeID( "GameLight" );
	ILight* IL=IMM->GetLight(lightID);
    if (IL)
    {
	    IL->SetDiffuse(EngSettings.LightDiffuse);
	    IL->SetAmbient(EngSettings.LightAmbient);
        IL->Render();
    }
/*
	if(!bActive){
		Sleep(20);
        ProcessMessages2();
		return;
	}
	*/ //Sleep Alt+Tab, вернуть на место
	void ProcessTimers();
	ProcessTimers();
	extern bool IgnoreProcessMessages;
	IgnoreProcessMessages=0;
	extern bool NoText;
	void ProcessMouseCursor();
	ProcessMouseCursor();	
	extern int Flips;
	extern int Diplimit;
	char strfps[128];		
	sprintf( strfps, "fps: %d", Flips );
	if(GetKeyState(VK_SCROLL/*'0'*/)&&GSets.ShowFPS){
		GPS.DrawFillRect(RealLx-45-10, 0,45+10,14,0xA0000000);
		ShowString( RealLx-40-10, 0, strfps, &SmallWhiteFont );
        if(GetKeyState(VK_CONTROL)&0x8000)
            PGR1.Draw(RealLx-85-10,0,60);
		PGR1.New();
        //TimeInDrawPrim;        
        static LARGE_INTEGER FREQ;
        static int WinTime=0;
        LARGE_INTEGER FREQ1;        
        LARGE_INTEGER FREQ2;        
        __ptime(&FREQ1);
        int WinTime1=GetTickCount();        
        FREQ2=FREQ1;
        FREQ2.QuadPart-=FREQ.QuadPart;
        int  WinTime2=WinTime1-WinTime;
        WinTime=WinTime1;
        if(!WinTime2)WinTime2=1;
        FREQ=FREQ1;
        double Freq=double(FREQ2.LowPart)+double(FREQ2.HighPart)*double(0x100000000);
        Freq*=1000.0f/WinTime2;
        Diplimit=1000000;
        extern int NumDips;
        GPS.DrawFillRect(RealLx-45-40, 15,45+40,14,0xA0000000);
        extern int NDX_calls;
        sprintf( strfps, "DIPS: %d(%d)", NumDips,NDX_calls );
        NDX_calls=0;
        ShowString( RealLx-40-40, 15, strfps, &SmallWhiteFont );
        int Nlines=2;            
        __showT(DIPT,TimeInDrawPrim);
        __showT(VLKT,VLockTime);
        __showT(ILKT,ILockTime);
        __showT(SWTM,TimeInSWTM);
        __showT(EFFT,BEffTime);
        __showT(SVPT,TimeInSVP);
        __showT(SSST,SetStreamSourceTime);
        __showT(SRTT,SetRenderTargetTime);
        __showT(STFT,SetTextureFactorTime);
        __showT(TEX,SetTextureTime);
        __showT(OTHER,OtherTime);
        __showT(FLIP,FlipTime);
        __showT(TOTAL,AllTime);

        extern int Diplimit;
        Diplimit=LimitedDipsCount;
        if( GetKeyState(VK_CONTROL)<0 && GetKeyState(VK_SHIFT)<0 && GetKeyState(VK_MENU)<0 ){
            static int lastt=0;
            if(GetTickCount()-lastt>100){
				if(GetKeyState(VK_ADD)<0){
					LimitedDipsCount++;
					if(MaxTerrTris<10000){
						MaxTerrTris+=3;
					}
				}
				if(GetKeyState(VK_SUBTRACT)<0){
					LimitedDipsCount--;
					if(MaxTerrTris<10000 && MaxTerrTris>0){
						MaxTerrTris-=3;
					}
				}
                lastt=GetTickCount();
            }                
        }
        NumDips=0;
        TimeInDrawPrim=0;
		int FMM_GetFreeMem();
		int FMM_GetAllocatedBlocks();
		int N=FMM_GetAllocatedBlocks()/1000;
		int N1=FMM_GetFreeMem()/1000/1000;
		sprintf( strfps, "P: %dk M: %dm",N,N1);
		GPS.DrawFillRect(0,0,64,18,0x80000000);
		if(N<500 && N1<320){
			ShowString( 0, 0, strfps, &SmallWhiteFont );
		}else{
			if((GetTickCount()/300)&1)ShowString( 0, 0, strfps, &RedFont);
		}
	}
    extern int NumDips;    
    Diplimit=LimitedDipsCount;
    NumDips=0;
	if(!NoText){
		static int T=GetTickCount();
		static int NE=0;
		static int NSF=0;
		static int NEF=0;
		extern bool InfoMode;
		if(!InfoMode)ShowPF();
	}
    __beginT();
	GPS.OnFrame();
	IRS->EndFrame();
    IRS->StartFrame();    
	IRS->ClearDevice(0,false,true);
    __endT(FlipTime);
    IgnoreProcessMessages=false;
    IgnoreProcessMessages2=false;
    bool ProcessWindowMessages();
    ProcessMessages2();
    /*
	if(GetKeyState(VK_CAPITAL)&&GetKeyState(VK_SCROLL)){
		char tt[128];
		extern int tmtmt;
		sprintf(tt,"screens\\scr%6d.bmp",tmtmt);
		IRS->SaveTexture(0,tt); 
	};
	extern DWORD g_NationalColors[];
	for(int i=0;i<8;i++){
		g_NationalColors[i]=EngSettings.NatColor.CLR[i];
	}
	void SetTerrainZBias( float bias );
	SetTerrainZBias(EngSettings.TerrainZBias);
    */
}
/*
 * Getting Screen Pointer
 *
 * You will ge the pointer to the invisible area of the screen
 * i.e, if primary surface is visible, then you will obtain the
 * pointer to the backbuffer.
 * You must call UnlockSurface() to allow Windows draw on the screen
 */
int SCRSZY=0;
void LockSurface(void)
{
	FILE* LR=fopen("lock.report","w");
	if(LR)fprintf(LR,"DDError:%d\n",DDError);
	long dderr=0;
	if (DDDebug)
	{
		ScreenPtr=(void*)(int(offScreenPtr)+MaxSizeX*32);
		ddsd.lpSurface=ScreenPtr;
		RealScreenPtr=ScreenPtr;
		return;
	}
	if (DDError){
		if(LR)fclose(LR);
		return;
	};
#ifdef COPYSCR
	if ((dderr=lpDDSPrimary->Lock(NULL,&ddsd,
							    DDLOCK_SURFACEMEMORYPTR|
								DDLOCK_WAIT,NULL))!=DD_OK) DDError=0;//true ;
	DDLog("lpDDSPrimary->Lock:%d\n",dderr);
	DDLog("Lock result:%d\n",dderr);
	DDLog("ptr:%d pitch=%d ly=%d\n",ddsd.lpSurface,ddsd.lPitch,ddsd.dwHeight);
	RSCRSizeX=ddsd.lPitch;
#else
	if ((dderr=lpDDSBack->Lock(NULL,&ddsd,
							    DDLOCK_SURFACEMEMORYPTR|
								DDLOCK_WAIT,NULL))!=DD_OK) DDError=0;//true ;
	RSCRSizeX=ddsd.lPitch;
#endif
#ifdef COPYSCR
	ScreenPtr=(void*)(int(offScreenPtr)+MaxSizeX*32);
	//ddsd.lpSurface=ScreenPtr;
	RealScreenPtr=ScreenPtr;
	RealScreenPtr=ddsd.lpSurface;
	SCRSZY=ddsd.dwHeight;
	//Rept("RealPtr:%d\n",int(ddsd.lpSurface));
	ClearScreen();
#else
	ScreenPtr=ddsd.lpSurface;
	offScreenPtr=ScreenPtr;
	if (lpDDSPrimary->Lock(NULL,&ddsd,
							    DDLOCK_SURFACEMEMORYPTR|
								DDLOCK_WAIT,NULL)!=DD_OK) DDError=0;//true ;
	RealScreenPtr=ScreenPtr;
#endif
	if(LR)fclose(LR);
}
/*
 *  Unlocking the surface 
 *
 *  You must unlock the Video memory for Windows to work properly
 */
void UnlockSurface(void)
{
	if(DDDebug) return;
	if (DDError)  return;
	//Back Buffer is active
#ifdef COPYSCR
	if (lpDDSPrimary->Unlock(NULL)!=DD_OK) DDError=true ;
#else
	if (lpDDSBack->Unlock(NULL)!=DD_OK) DDError=true ;
	if (lpDDSPrimary->Unlock(NULL)!=DD_OK) DDError=true ;
#endif
}
/*
 * Getting then DC of the active (invisible) area of the screen
 */
HDC GetSDC(void)
{
	if(DDDebug) return 0;
	HDC hdc;
	if (DDError) return 0;
	if (CurrentSurface)
	{
		//Back Buffer is active
		if (lpDDSPrimary->GetDC(&hdc)!=DD_OK) DDError=true ;
	}else
	{
		//Primary Surface is active
		if (lpDDSBack->GetDC(&hdc)!=DD_OK) DDError=true;
	}
	return hdc;
}
/*
 * Timer Callback 
 */
bool m640_16=0;
bool m640_24=0;
bool m640_32=0;
bool m1024_768=0;
int BestVX=640;
int BestVY=480;
int BestBPP=32;

bool EnumDisplayModes()
{
	if (!IRS) return false;
	int nModes = IRS->GetNDisplayModes();
	for (int i = 0; i < nModes; i++)
	{
		bool bAlreadyPresent = false;
		int width  = 0;
		int height = 0;
		IRS->GetDisplayMode( i, width, height );
		for (int j = 0; j < NModes; j++)
		{
			if (ModeLX[j]==width && ModeLY[j]==height) 
			{
				bAlreadyPresent = true;
				break;
			}
		}

		if (width < 1024) bAlreadyPresent = true;

		if (!bAlreadyPresent)
		{
			ModeLX[NModes] = width;
			ModeLY[NModes] = height;
			NModes++;
		}
	}
	BestBPP = 16;
	return true;
} // EnumDisplayModes

void DelLog(){
	DeleteFile("DDraw.log");
};
void KeyTestMem();
DWORD GetObjectsShadowQuality();

bool CreateDDObjects(HWND hwnd) 
{
	assert( IRS );
 	IRS->Init( GetModuleHandle( NULL ), hwnd );

    //  do this here, because shadow map texture should be created first
    IShadowMgr->SetShadowQuality( (ShadowQuality)GetObjectsShadowQuality() ); 

    int ts=IRS->GetTexMemorySize();
	if(ts<142*1000*1000){
		//char* t=GetTextByID("$AGP_APERTURE");
        char* s=GetGlobalBuffer();
        sprintf(s,"Warning! Video memory is low! (%d M) It is possible that  video memory is not configured properly. Please restart computer and set AGP Aperture size = 256 MB or more in BIOS configuration program. Usually you can enter BIOS configuration utility by pressing 'DEL' while computer restarts.",ts/1000/1000);
		//if(t[0]=='$')t="Engine.exe has detected, that video memory is not configured properly. Please restart computer and set AGP Aperture size = 256 MB or more in BIOS configuration program. Usually you can enter BIOS configuration utility by pressing 'DEL' while computer restarts.";
        MessageBox(hwnd,s,"warning!",MB_ICONWARNING);
	}
	if (!DDDebug)
	{
		ScreenProp sp = IRS->GetScreenProp();
		sp.m_bFullScreen = true;
		IRS->SetScreenProp( sp );
		RealLx=1024;//GetSystemMetrics(SM_CXSCREEN);
		RealLy=768;//GetSystemMetrics(SM_CYSCREEN);		
	}	
	IMM->Init();
    ITerra->Init();
	GPS.Init();
	InitPictureManager();

    void InitGroundZbuffer();
    InitGroundZbuffer();
    ITerra->Init();
	InitPictureManager();
	
	GPS.Init();

	DDError=false;
	SCRSizeX=MaxSizeX;
	SCRSizeY=MaxSizeY;
	RSCRSizeX=RealLx;
	//Pitch=ddsd.lPitch;
	COPYSizeX=RealLx;
	RSCRSizeY=RealLy;
	ScrHeight=SCRSizeY;
	ScrWidth=SCRSizeX;

	GPS.SetClipArea( 0, 0, RealLx, RealLy );
	BytesPerPixel=2;
	return true;
}

BOOL CreateRGB640DDObjects(HWND hwnd)
{
	HRESULT ddrval;
	DDSCAPS ddscaps;
	char    buf[256];
	DDError=false;
	CurrentSurface=true;
	if (DDDebug)
	{
		
		DDError=false;
		SCRSizeX=MaxSizeX;
		SCRSizeY=MaxSizeY;
		COPYSizeX=RealLx;
		RSCRSizeX=RealLx;
		RSCRSizeY=RealLy;
		ScrHeight=SCRSizeY;
		ScrWidth=SCRSizeX;
		InitRLCWindows();

		GPS.SetClipArea( 0, 0, SCRSizeX, SCRSizeY );
		BytesPerPixel=1;
		offScreenPtr=(malloc(SCRSizeX*(SCRSizeY+32*4)));
		return true;
	}
#ifdef COPYSCR
	offScreenPtr=offScreenPtr=(malloc(MaxSizeX*(MaxSizeY+32*4)));
#endif
	if(lpDD){
		lpDDSPrimary->Release();
		goto SDMOD;
	};
	ddrval = DirectDrawCreate( NULL, &lpDD, NULL );
	DDLog("RGB640: DirectDrawCreate:%d\n",ddrval);
    if( ddrval == DD_OK )
    {
SDMOD:;
        // Get exclusive mode
        ddrval = lpDD->SetCooperativeLevel( hwnd,
                                DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN );
		DDLog("RGB640: SetCooperativeLevel:%d\n",ddrval);
        if(ddrval == DD_OK )
        {
            ddrval = lpDD->SetDisplayMode(640,480,BestBPP); //COPYSizeX,RSCRSizeY, 8 );
			DDLog("RGB640: SetDisplayMode:%d\n",ddrval);
            if( ddrval == DD_OK )
            {
                // Create the primary surface with 1 back buffer
                ddsd.dwSize = sizeof( ddsd );
                ddsd.dwFlags = DDSD_CAPS;
                ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
                ddrval = lpDD->CreateSurface( &ddsd, &lpDDSPrimary, NULL );
				DDLog("RGB640: CreateSurface:%d\n",ddrval);
                if( ddrval == DD_OK )
                {
					DDError=false;
					SCRSizeX=MaxSizeX; 
					SCRSizeY=MaxSizeY;
					RSCRSizeX=RealLx;//ddsd.lPitch;
					Pitch=ddsd.lPitch;
					COPYSizeX=RealLx;
					RSCRSizeY=RealLy;
					ScrHeight=SCRSizeY;
					ScrWidth=SCRSizeX; 

					GPS.SetClipArea( 0, 0, SCRSizeX, SCRSizeY );
					BytesPerPixel=1;
					return true;
                }
            }
        }
    }
    wsprintf(buf, "Direct Draw Init Failed (%08lx)\n", ddrval );
    MessageBox( hwnd, buf, "ERROR", MB_OK );
	return false;
}

/*   Direct Draw palette loading*/
int clrRed;
int clrGreen;
int clrBlue;
int clrYello;
CEXPORT
void LoadPalette(char const* lpFileName)
{
	AFile((char*)lpFileName);
	//if (DDDebug) return;
	if (DDError) return;
	ResFile pf=RReset(lpFileName);
	memset(&GPal,0,1024);
	if (pf!=INVALID_HANDLE_VALUE)
	{
		for(int i=0;i<256;i++){
			RBlockRead(pf,&GPal[i],3);
			//RBlockRead(pf,&xx.bmp.bmiColors[i],3);
		};
		RClose(pf);
		if(!strcmp(lpFileName,"agew_1.pal")){
			int DCL=6;
			int C0=65;//128-DCL*4;
			for(int i=0;i<12;i++){
				int gray=0;
				if(i>2)gray=(i-2)*2;
				if(i>7)gray+=(i-7)*8;
				if(i>9)gray+=(i-10)*10;
				if(i>10)gray+=50;
				gray=gray*6/3;
				//gray=(i+5)*6;
				int rr=0*C0/150+gray*8/2;
				int gg=80*C0/150+gray*6/2;//80
				int bb=132*C0/150+gray*4/2;
				if(rr>255)rr=255;
				if(gg>255)gg=255;
				if(bb>255)bb=255;
				if(i<5){
					rr=rr-((rr*(5-i))/6);
					gg=gg-((rr*(5-i))/6);
					bb=bb-((rr*(5-i))/6);
				};
				if(i<3){
					rr=rr-((rr*(3-i))/4);
					gg=gg-((rr*(3-i))/4);
					bb=bb-((rr*(3-i))/4);
				};
				if(i<2){
					rr=rr-((rr*(2-i))/3);
					gg=gg-((rr*(2-i))/3);
					bb=bb-((rr*(2-i))/3);
				};
				//if(!i){
				//	rr=rr*10/11;
				//	gg=gg*10/11;
				//	bb=bb*10/11;
				//};
				GPal[0xB0+i].peBlue=bb;
				GPal[0xB0+i].peRed=rr;
				GPal[0xB0+i].peGreen=gg;
				C0+=5; 
			};
			ResFile pf=RRewrite(lpFileName);
			for(i=0;i<256;i++)RBlockWrite(pf,&GPal[i],3);
			RClose(pf);
		};
		if(!DDDebug){
			CheckPal16();
		};
	}
	clrRed=0xD0;
	clrGreen=GetPaletteColor(0,255,0);
	clrBlue=GetPaletteColor(0,0,255);
	clrYello=GetPaletteColor(255,255,0);
}
void CBar(int x,int y,int Lx,int Ly,byte c);
void Susp(char* str){
	return;
	if(!DDDebug){
		void* oldsof=ScreenPtr;
		ScreenPtr=RealScreenPtr;
		int ScLx=ScrWidth;
		ScrWidth=RealLx;
		CBar(700,1,100,5,0x93);
		ShowString(700,0,str,&fn8);
		ScrWidth=ScLx;
		ScreenPtr=oldsof;
	}else{
		CBar(700,1,100,5,0x93);
		ShowString(700,0,str,&fn8);

		HDC WH=GetWindowDC(hwnd);

		xxt.bmp.bmiHeader.biSize=sizeof BITMAPINFOHEADER;
		xxt.bmp.bmiHeader.biWidth=SCRSizeX;
		xxt.bmp.bmiHeader.biHeight=SCRSizeY;
		xxt.bmp.bmiHeader.biPlanes=1;
		xxt.bmp.bmiHeader.biBitCount=8;
		xxt.bmp.bmiHeader.biCompression=BI_RGB;
		xxt.bmp.bmiHeader.biSizeImage=0;
		int z=StretchDIBits(WH,700,1,100,5,
			700,1,100,5,ScreenPtr,&xxt.bmp,
			DIB_RGB_COLORS,SRCCOPY);
			
	};
};
void SetDarkPalette(){};

CEXPORT void SlowLoadPalette(LPCSTR lpFileName)
{
	LoadPalette(lpFileName);
	return;
}

CEXPORT
void SlowUnLoadPalette(LPCSTR lpFileName)
{
	PALETTEENTRY            NPal[256];
	if (DDError) return;
	ChangeColorFF();
	//ResFile pf=RReset(lpFileName);
	//memset(&GPal,0,1024);
	//if (pf!=INVALID_HANDLE_VALUE)
	//{
		//for(int i=0;i<256;i++){
		//	RBlockRead(pf,&GPal[i],3);
		//	//RBlockRead(pf,&xx.bmp.bmiColors[i],3);
		//};
		//RClose(pf);
		if(!DDDebug){
			byte* pal=(byte*)NPal;
			byte* pal0=(byte*) GPal;
			int mul=0;
			int t0=GetTickCount();
			int mul0=0;
			do{
				mul=(GetTickCount()-t0)*2;
				if(mul>255)mul=255;
				if(mul!=mul0){
					for(int j=0;j<1024;j++){
						pal[j]=byte((int(pal0[j])*(255-mul))>>8);
					};
					pal[1023]=0;
					pal[1022]=0;
					pal[1021]=0;
					pal[1020]=0;
					lpDDPal->SetEntries(0,0,255,NPal);
				};
				mul0=mul;
			}while(mul!=255);
		};
	//}
}	
/*     Closing all Direct Draw objects
 *
 * This procedure must be called before the program terminates,
 * otherwise Windows can occur some problems.
 */
void FreeDDObjects( void )
{
	free(offScreenPtr);
	offScreenPtr=NULL;
	if (DDDebug)
	{
		//free(offScreenPtr);
		return;
	}
    if( lpDD != NULL )
    {
		/*if( lpDDSBack != NULL )
        {
            lpDDSBack->Release();
            lpDDSBack = NULL;
        };*/
		//ClearScreen();
        if( lpDDSPrimary != NULL )
        {
            lpDDSPrimary->Release();
            lpDDSPrimary = NULL;
        };
		lpDD->Release();
        lpDD = NULL;
    }
}
void SetDebugMode()
{
	DDDebug=true;
}
void NoDebugMode()
{
	DDDebug=false;
}
CEXPORT
void GetPalColor(byte idx,byte* r,byte* g,byte* b){
	*r=GPal[idx].peRed;
	*g=GPal[idx].peGreen;
	*b=GPal[idx].peBlue;
};
#ifdef _USE3D
extern BaseMesh BMS;
bool init=0;
void TnLset(VertexTnL* V,float x,float y,float z,float u,float v,DWORD D,DWORD S){
	V->x=x;
	V->y=y;
	V->z=z;
	V->w=1.0;
	V->u=u;
	V->v=v;
	V->diffuse=D;
	V->specular=S;
};
void TnLset2(VertexTnL2* V,float x,float y,float z,float u,float v,float u2,float v2,DWORD D){
	V->x=x+100;
	V->y=y+100;
	V->z=z;
	V->w=1.0;
	V->u=u;
	V->v=v;
	V->u2=u2;
	V->v2=v2;
	V->diffuse=D;
};
void TestPiro();
float fdx=0;
float fdy=0;
extern int mouseX;
extern int mouseY;
extern int tmtmt;
void Test3D(){
	//Test4Piro();
	//int Id=GPS.PreLoadGPImage("list");
	//GPS.ShowGP(200,200,Id,tmtmt%88,0);

	return;
	//if(!init){
		//if(GetKeyState('A')&0x8000)fdx-=0.001;
		//if(GetKeyState('D')&0x8000)fdx+=0.001;
		//if(GetKeyState('W')&0x8000)fdy-=0.001;
		//if(GetKeyState('X')&0x8000)fdy+=0.001;
		fdx=float(mouseX-100-128)/1500.0;
		fdy=float(mouseY-100-32)/1500.0;

		BMS.create(256,256,vfVertexTnL2,ptTriangleList,false);
		word ID=IRS->GetShaderID("road");
		word TID=IRS->GetTextureID("road.tga");
		BMS.setShader(ID);
		BMS.setTexture(TID);
		BMS.setTexture(TID,1);
		VertexTnL2* VTX=(VertexTnL2*)BMS.getVertexData();
		word* IDX=BMS.getIndices();

		TnLset2(VTX+0,  0  ,  0,   0,   0,   0,      0,   0,0xFFFFFFFF);
		TnLset2(VTX+1,  256,  0,   0,   1,   0,1.0+fdx,   0,0xFFFFFFFF);
		TnLset2(VTX+2,  0  ,256,   0,   0,   1,   0, 1.0+fdy,0xFFFFFFFF);
		TnLset2(VTX+3,  256,256,   0,   1,   1,1+fdx, 1.0+fdy,0xFFFFFFFF);

		IDX[0]=0;
		IDX[1]=1;
		IDX[2]=2;
		IDX[3]=1;
		IDX[4]=3;
		IDX[5]=2;
		BMS.setNInd(6);
		BMS.setNPri(2);
		BMS.setNVert(4);
		init=1;
	//};
	DrawBM(BMS);
};
#endif