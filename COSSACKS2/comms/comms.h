#pragma once

#include <windows.h>
#include <zmouse.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <fcntl.h>
#include <io.h>
#include <sys/stat.h>
#include <Shlwapi.h>
#include <Shlobj.h>
#include <memory.h>
#include <process.h>

class cVec2;
class cVec3;
class cVec4;
class cMat2;
class cMat3;
class cMat4;
class cAngles;
class cQuat;
class cRotation;
class cBounds;
class cSphere;
class cPlane;
class cColor;
class cStr;
class cBitMap;
class cDib24;
class cRawMesh;
class cIO;
class cHashIndex;
class cSegment;
class cMaterial;

class dVector;
class dMatrix;

#pragma pack(push)
#pragma pack(1)

#include "cTypeDefs.h"
#include "cAssert.h"
#include "cMath.h"
#include "cVec2.h"
#include "cVec3.h"
#include "cVec4.h"
#include "cVec.h"
#include "dVector.h"
#include "cHashIndex.h"
#include "cList.h"
#include "cMat2.h"
#include "cMat3.h"
#include "cMat4.h"
#include "cMatrix.h"
#include "dMatrix.h"
#include "cVectors.h"
#include "cQuat.h"
#include "cAngles.h"
#include "cRotation.h"
#include "cColor.h"
#include "cStr.h"
#include "cDib24.h"
#include "cBitMap.h"

#include "cData.h"

#include "cSolver.h"
#include "cSeg.h"
#include "cTimer.h"
#include "cSphere.h"
#include "cPlane.h"
#include "cBounds.h"
#include "cSegBezier.h"
#include "cSplineBezier.h"
#include "cViewPort2.h"
#include "cExtrapolate.h"
#include "cInterpolate.h"
#include "cTrack.h"
#include "cRect.h"
#include "cInput.h"

#include "cWidget.h"
#include "cMoveTool.h"
#include "cRotateTool.h"
#include "cScaleTool.h"
#include "cGrid.h"
#include "cViewer.h"
#include "cImage.h"
#include "cVertex.h"
#include "cRender.h"
#include "cMaterial.h"
#include "cMesh.h"
#include "cRawMesh.h"
#include "cIO.h"
#include "cImageBmp.h"
#include "cImageDds.h"
#include "cImageTga.h"
#include "cMeshObj.h"
#include "cIsoSurface.h"

#include "cWin32.h"
#include "cLog.h"


#pragma pack(pop)