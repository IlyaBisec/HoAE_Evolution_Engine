// c2Pre.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
#ifndef __COSSACKS2_STDAFX__
#define __COSSACKS2_STDAFX__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//  exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN	

#define		WM_MOUSEWHEEL		0x020A
#define		WHEEL_DELTA			120

#ifdef _USE3D

	#include "assert.h"
	#include "gmDefines.h"
	
	#include "kLog.h"
	#include "kString.h"
	#include "kArray.hpp"
	#include "IRenderSystem.h"
	
	#include "mMath3D.h"
	#include "mCamera.h"
	#include "kDebug.h"
	#include "kStatistics.h"
	#include "gpColor.h"
	#include "gpMesh.h"

#endif // _USE3D

#include "ddini.h"
#include "ResFile.h"
#include "GP_Draw.h"

//{{AFX_INSERT_LOCATION}}
#endif // __COSSACKS2_STDAFX__
