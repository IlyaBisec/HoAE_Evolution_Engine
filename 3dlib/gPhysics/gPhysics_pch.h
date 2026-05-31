// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0400	// Change this to the appropriate value to target IE 5.0 or later.
#endif

#pragma		once
#define		WIN32_LEAN_AND_MEAN	

#define		WM_MOUSEWHEEL		0x020A
#define		WHEEL_DELTA			120

#include "windows.h"

#include <direct.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <string>
#include <map>
#include <algorithm>

#include "gmDefines.h"
#include "kAssert.h"
#include "kResFile.h"
#include "kIO.h"
#include "kLog.h"
#include "kString.h"
#include "kCache.h"
#include "kArray.hpp"
#include "kTemplates.hpp"
#include "kPropertyMap.h"
#include "kUtilities.h"
#include "kMemorySpy.h"
#include "kColorValue.h"

#include "mMath2D.h"
#include "mMath3D.h"
#include "mQuaternion.h"
#include "mGeom3D.h"
#include "mSphere.h"
#include "mAlgo.h"

#include "IRenderSystem.h"
#include "rsRenderSystem.h"

#include "kTypeTraits.h"
#include "kEnumTraits.h"

#include "kIOHelpers.h"
#include "kStatistics.h"
#include "IInput.h"
#include "vMesh.h"

#include "sg.h"

