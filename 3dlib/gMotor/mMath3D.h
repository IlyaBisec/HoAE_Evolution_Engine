/*****************************************************************
/*  File:   mMath3D.h                                             
/*    Desc:    Bundle math routines include header                                      
/*  Author: Silver, Copyright (C) GSC Game World                  
/*  Date:   January 2002                                          
/*****************************************************************/
#ifndef __MMATH3D_H__
#define __MMATH3D_H__

#ifndef DIALOGS_API
#ifndef DIALOGS_USER
#define DIALOGS_API __declspec(dllexport)
#else //DIALOGS_USER
#define DIALOGS_API __declspec(dllimport)
#endif //DIALOGS_USER
#endif //DIALOGS_API

class Matrix4D;
class Matrix3D;
class Vector4D;
class Vector3D;
class Plane;
class Quaternion;
class Line3D;
class AABoundBox;

#include "mConstants.h"
#include "mUtil.h"
#include "mRandom.h"
#include "mVector.h"
#include "mPlane.h"
#include "mTransform.h"
#include "mSegmentSet.h"

#endif // __MMATH3D_H__