/***********************************************************************
 *Direct Draw initialisation module                    
 *
 * This module creates the Direct Draw object with the primary surface
 * and a backbuffer and sets 800x600x8 display mode.
 *
 ***********************************************************************/
#ifndef __DDINI_H_
#define __DDINI_H_
//#include "afx.h"
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <stdarg.h>
#include "resource.h"
#include "lines.h"
#include "protest.h"
#include <ddraw.h>
//#include "..\3dlib\ddraw.h"
#define CEXPORT __declspec(dllexport)
#define CIMPORT __declspec(dllimport)
//#define STARFORCE
#define MAKE_PTC
#define DLLEXPORT extern "C" __declspec(dllexport)

#ifndef STARFORCE
 #define DLL0(x) x()
 #define DLL1(x,y) x(y)
 #define DLL2(x,y,z) x(y,z)
 #define DLL3(x,y,z,t) x(y,z,t)
 #define SFLB_DLLEXPORT void
#else
 #define SFLB_DLLEXPORT extern "C" void __declspec(dllexport) __cdecl
 #define DLL0(x) x()
 #define DLL1(x,y) x(y)
 #define DLL2(x,y,z) x(y,z)
 #define DLL3(x,y,z,t) x(y,z,t)
#endif


//#ifndef __ddini_cpp_
extern LPDIRECTDRAW            lpDD;			   // DirectDraw object
extern LPDIRECTDRAWSURFACE     lpDDSPrimary;   // DirectDraw primary surface
extern LPDIRECTDRAWSURFACE     lpDDSBack;      // DirectDraw back surface
extern BOOL                    bActive;        // is application active?
extern BOOL                    CurrentSurface; //=FALSE if backbuffer
											   // is active (Primary surface is visible)
											   //=TRUE if  primary surface is active
											   // (but backbuffer is visible)
extern BOOL                    DDError;        //=FALSE if Direct Draw works normally 
extern DDSURFACEDESC   ddsd;
extern HWND hwnd;
extern BOOL DDDebug;
/*  Create Direct Draw object
 * 
 * This procedure creadtes Direct Draw object with primary surface and 
 * one backbuffer and sets 800x600x8 display mode.
 * This procedure enables usage of all other procedure in thes module.
 */
bool CreateDDObjects(HWND hwnd);
/*     Closing all Direct Draw objects
 *
 * This procedure must be called before the program terminates,
 * otherwise Windows can occur some problems.
 */
void FreeDDObjects( void );
/*
 * Flipping Pages
 */
CEXPORT
void FlipPages(void);
/*
 * Getting Screen Pointer
 *
 * You will ge the pointer to the invisible area of the screen
 * i.e, if primary surface is visible, then you will obtain the
 * pointer to the backbuffer.
 * You must call UnlockSurface() to allow Windows draw on the screen
 */
void LockSurface(void);
/*
 *  Unlocking the surface 
 *
 *  You must unlock the Video memory for Windows to work properly
 */
void UnlockSurface(void);
/*
 * Getting then DC of the active (invisible) area of the screen
 */
HDC GetSDC(void);
//Sttting the palette
CEXPORT void LoadPalette(LPCSTR lpFileName);
void SetDebugMode();
void NoDebugMode();


extern int PREVT;
extern int NeedAddTime;
extern int AddTime;
int GetDADDT( int DT );
inline int GetRealTime()
{
    int T=GetTickCount();
    if(PREVT){
        if(T-PREVT>5){
            if(NeedAddTime<AddTime){
                AddTime-=GetDADDT(AddTime-NeedAddTime)*6;
                if(AddTime<NeedAddTime)AddTime=NeedAddTime;
            };
            if(NeedAddTime>AddTime){
                AddTime+=GetDADDT(NeedAddTime-AddTime)*6;
                if(AddTime>NeedAddTime)AddTime=NeedAddTime;
            };
            PREVT=T;
        };
    }else PREVT=T;
    return T+(AddTime>>7);
}; // GetRealTime

extern bool IgnoreProcessMessages;
extern bool IgnoreProcessMessages2;
extern bool PDIF_Inside;
inline bool ProcessMessages()
{	
    return false;


    if(IgnoreProcessMessages || IgnoreProcessMessages2)return false;
    if(PDIF_Inside)return false;	
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

//  wrapper for outside users, such as IntExplorer
CEXPORT bool ProcessWindowMessages();

#endif //__DDINI_H_


#include "newmemory.h"