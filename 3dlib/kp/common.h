#include "stdafx.h"

#define __STDAPPLICATION__

typedef BYTE byte;
typedef WORD word;

#include "..\..\ClassEngine\DynArray.h"
#include "..\..\cossacks2\mesh\unihash.h"
#include "..\..\ClassEngine\ClassEngine.h"
#include "..\..\ClassEngine\More_Types.h"

#include "kSystemDialogs.h"

#include "RasterOp.h"
#include "DynamicTexture.h"
#include "PolyMesh.h"
#include "ICamera.h"

#include "uiWidget.h"
#include "uiWindow.h"
#include "uiFrameWindow.h"
#include "uiModelEditor.h"
#include "TexturePattern.h"
#include "SimpleWidgets.h"
#include "PenShape.h"
#include "PenInterface.h"

#include "Utils.h"
#include "BasicTool.h"

#define FLT_MAX 3.402823466e+38F
#define FLT_MIN 1.175494351e-38F