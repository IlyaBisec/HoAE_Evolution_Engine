#ifndef __COSSACKS2_STDAFX__
#define __COSSACKS2_STDAFX__

#ifndef DIALOGS_API
#ifndef DIALOGS_USER
#define DIALOGS_API __declspec(dllexport)
#else //DIALOGS_USER
#define DIALOGS_API __declspec(dllimport)
#endif //DIALOGS_USER
#endif //DIALOGS_API

#pragma        once
#define        WIN32_LEAN_AND_MEAN    

#define        WM_MOUSEWHEEL        0x020A
#define        WHEEL_DELTA            120

//  need this to enable some functions, for example ReadDirectoryChangesW
#define     _WIN32_WINNT        0x0400 

#include "windows.h"

#include <math.h> 
#include <direct.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "gmDefines.h"
#include "mUtil.h"

#include "kAssert.h"
#include "kResFile.h"
#include "kIO.h"
#include "kLog.h"
#include "kString.h"
#include "kCache.h"
#include "kArray.hpp"
#include "kTemplates.hpp"
#include "kPropertyMap.h"
#include "kObjectFactory.h"
#include "kUtilities.h"
#include "kMemorySpy.h"

#include "mVector.h"
#include "mTransform.h"
#include "IRenderSystem.h"
#include "rsRenderSystem.h"
#include "ISpriteManager.h"
#include "IPictureManager.h"

#include "kTypeTraits.h"
#include "kEnumTraits.h"

#include "mMath2D.h"
#include "mLine.h"
#include "mMath3D.h"
#include "mQuaternion.h"
#include "mGeom3D.h"
#include "mSphere.h"
#include "mAlgo.h"
#include "kColorValue.h"

#include "kTypeTraits.h"
#include "kMathTypeTraits.h"
#include "kXMLParser.h"

#include "vMesh.h"

#include "kIOHelpers.h"
#include "kStatistics.h"

//{{AFX_INSERT_LOCATION}}
#endif // __COSSACKS2_STDAFX__
