/*****************************************************************
/*  File:   mConstants.h                                             
/*    Desc:    Different mathematical constabts                                      
/*  Author: Silver, Copyright (C) GSC Game World                  
/*  Date:   January 2002                                          
/*****************************************************************/
#ifndef __MCONSTANTS_H__
#define __MCONSTANTS_H__

const float c_Epsilon           = 0.000001f;
const float c_SpaceEpsilon      = 0.00001f;
const float c_SmallEpsilon      = 0.00000000001f;
const float c_PI                = 3.1415926535897932f;
const float c_DoublePI          = 6.2831853071795864f;
const float c_HalfPI            = 1.5707963267948966f;
const float c_QuarterPI         = 0.7853981633974483f;
const float c_CosPId6           = 0.8660254037844386f;
const float c_PId6              = 0.5235987755982988f;

__forceinline float __fabs(float x){\
	int i = *reinterpret_cast<int *>(&x);
	i &= 0x7fffffff;
	return *reinterpret_cast<float *>(&i);
}
#define fabs(x) __fabs(x)
/*****************************************************************/
/*    Enum:    XStatus
/*    Desc:    Status of the intersection test
/*****************************************************************/
enum XStatus
{
    xsUnknown           = 0,
    xsInside            = 1,
    xsOutside           = 2,
    xsIntersects        = 3
}; // enum XStatus

/*****************************************************************/
/*    Enum:    TestPrecision
/*    Desc:    Precision of some tests (intersection, whatever)
/*****************************************************************/
enum TestPrecision
{
    tpUnknown           = 0,
    tpLow               = 1,
    tpMedium            = 2,
    tpHigh              = 3
}; // enum TestPrecision

#endif // __MCONSTANTS_H__