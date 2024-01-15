//-----------------------------------------------------------------------------
// File: gRenderPch.h
//-----------------------------------------------------------------------------
#ifndef __GRENDERPCH_H__
#define __GRENDERPCH_H__

//#define ALLOW_TIMING

//#define ALLOW_COUNTS

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <stdio.h>
#include <tchar.h>
#include <commctrl.h>

#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif // _DEBUG

#include "kAssert.h"

#include <d3dx9.h>
#include <d3d9types.h>

#include "DXUtil.h"
#include "D3DEnumeration.h"
#include "D3DSettings.h"
#include "D3DApp.h"
#include "D3DUtil.h"
#include "D3DRes.h"


#include "kLog.h"
#include "gmDefines.h"
#include "mUtil.h"
#include "mConstants.h"
#include "mVector.h"
#include "mTransform.h"
#include "mMath2D.h"
#include "mQuaternion.h"
#include "IRenderSystem.h"
#include "rsRenderSystem.h"
#include "IInput.h"
#include "kUtilities.h"
#include "mPlane.h"
#include "mGeom3D.h"
#include "kFilePath.h"

#ifdef ALLOW_TIMING

__forceinline void __ptime(LARGE_INTEGER* LI){
    __asm{
        rdtsc;
        mov     ebx,DWORD PTR[LI]
        mov     DWORD PTR[ebx],eax;
        mov     DWORD PTR[ebx+4],edx;
    }
}
extern int NDX_calls;

#define __beginT() LARGE_INTEGER LI0;__ptime(&LI0);NDX_calls++;
#define __endT(tvar) LARGE_INTEGER LI1;__ptime(&LI1);LI1.QuadPart-=LI0.QuadPart;tvar+=LI1.LowPart;


#else //ALLOW_TIMING

#ifdef ALLOW_COUNTS
extern int NDX_calls;
#define __beginT() NDX_calls++;
#define __endT(tvar) tvar++;

#else //ALLOW_COUNTS

#define __beginT()
#define __endT(tvar)

#endif //ALLOW_COUNTS

#endif //ALLOW_TIMING

#define		WM_MOUSEWHEEL		0x020A
#define		WHEEL_DELTA			120

#pragma warning ( disable : 4018 )
#pragma warning ( disable : 4267 )

#endif // __GRENDERPCH_H__
