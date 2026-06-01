/*****************************************************************
/*  File:   Defines.h                                            *
/*  Desc:   Constant values and definitions                      *
/*  Author: Silver, Copyright (C) GSC Game World                 *
/*  Date:   January 2002                                         *
/*****************************************************************/
#ifndef __DEFINES_H__
#define __DEFINES_H__
#pragma     once

typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned int        UINT;
typedef unsigned long       DWORD;
typedef long                LONG;

#ifdef _INLINES
#define _inl inline
#define _finl __forceinline
#else
#define _inl 
#define _finl 
#endif // _INLINES

//  disable some pain in the ass...

// warning 4018: 'expression' : signed/unsigned mismatch
#pragma warning ( disable : 4018 )
// warning C4244: 'initializing' : 
//  conversion from 'double' to 'float', possible loss of data
#pragma warning ( disable : 4244 )
//  256 symbols name truncation
#pragma warning ( disable : 4786 )
//  EH disabled
#pragma warning ( disable : 4530 )
//  downcasting
#pragma warning ( disable : 4312 )
//  deprecated symbols
#pragma warning ( disable : 4996 )
//  macro redefinition
#pragma warning ( disable : 4005 )
//  truncation from double to float
#pragma warning ( disable : 4305 )
#pragma warning ( disable : 4005 )
#pragma warning ( disable : 4267 )
//  'no return type' - doesn't like macros with typename arguments in class declaration
#pragma warning ( disable : 4183 )
//  unreferenced formal parameter
#pragma warning ( disable : 4100 )
//  conditional expression is constant
#pragma warning ( disable : 4127 )
//  pointer truncation
#pragma warning ( disable : 4311 )
//  class/struct mangling
#pragma warning ( disable : 4099 )

//  wide-used constants
const int    c_NoID  = 0xFFFFFFFF;

#define BEGIN_NAMESPACE(A)  namespace A{
#define END_NAMESPACE(A)    }

#define BEGIN_ONCE         { static bool ___first__ = true; if (___first__) {___first__ = false;

#define END_ONCE           }}
#endif //__DEFINES_H__
