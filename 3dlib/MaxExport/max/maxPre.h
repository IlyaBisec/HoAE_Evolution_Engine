/*****************************************************************************/
/*    File:    maxPre.h
/*    Desc:    Precompiled headers file for max exporter
/*    Author:    Ruslan Shestopalyuk
/*    Date:    28.04.2003
/*****************************************************************************/
#ifndef __C2M_MAX_EXPORT_STDAFX_H__
#define __C2M_MAX_EXPORT_STDAFX_H__

#pragma        once
#define        WIN32_LEAN_AND_MEAN    

#include "windows.h"

//  Max FN_0 macro - too many actual parameters
#pragma warning ( disable : 4002 )

#include <direct.h>
#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <string>
#include <map>

#include "gmDefines.h"

#include "assert.h"
#include "kResFile.h"
#include "kIO.h"
#include "kArray.hpp"
#include "kTemplates.hpp"
#include "kString.h"
#include "kLog.h"
#include "kTypeTraits.h"
#include "kPropertyMap.h"

#include "mMath2D.h"
#include "mMath3D.h"
#include "mQuaternion.h"
#include "mGeom3D.h"
#include "mAlgo.h"

#include "IRenderSystem.h"
#include "rsVertex.h"

#include "gpMesh.h"
#include "kIOHelpers.h"

#include "mQuatCurve.h"
#include "mFloatCurve.h"

#include "sg.h"

#include "sgController.h"
#include "sgPRSAnimation.h"
#include "sgUVAnimation.h"
#include "sgAnimationBlock.h"
#include "vSkin.h"
#include "c2mExporter.h"
#include "MaxExport.h"



//{{AFX_INSERT_LOCATION}}
#endif // __C2M_MAX_EXPORT_STDAFX_H__