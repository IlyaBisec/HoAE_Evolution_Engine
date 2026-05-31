/*****************************************************************
/*  File:   gpColor.h                                            *
/*  Desc:   Color operations declaration
/*  Author: Silver, Copyright (C) GSC Game World                 *
/*  Date:   January 2002                                         *
/*****************************************************************/
#ifndef __GPCOLOR_H__
#define __GPCOLOR_H__
#include "gmDefines.h"

//  texture/surface/bitmap etc. color formats 
enum ColorFormat
{
	cfUnknown		= 0,
	cfARGB4444		= 1,
	cfXRGB1555		= 2,
	cfARGB8888		= 3,
	cfRGB565		= 4,
	cfA8			= 5,
	cfRGB888		= 6,
	cfXRGB8888		= 7,
	cfV8U8			= 8,

	cfBackBufferCompatible	= 9
}; // enum ColorFormat

ENUM(ColorFormat, "Color Format",
	 en_val( cfUnknown,		"Unknown"	) <<		
	 en_val( cfARGB4444,	"ARGB4444"	) <<	
	 en_val( cfXRGB1555,	"XRGB1555"	) <<	
	 en_val( cfARGB8888,	"ARGB8888"	) <<	
	 en_val( cfRGB565,		"RGB565"	) <<
	 en_val( cfA8,			"A8"		) <<	
	 en_val( cfRGB888,		"RGB888"	) <<	
	 en_val( cfXRGB8888,	"XRGB8888"	) <<
	 en_val( cfV8U8,		"V8U8"		) <<	
	 en_val( cfBackBufferCompatible, "BackBufferCompatible" ) );

/*****************************************************************
/*  Class:  GPColor                                              *
/*  Desc:   represents color value
/*****************************************************************/
class GPColor
{
	float					a;
	float					r;
	float					g;
	float					b;

public:
							GPColor();
	_inl void				SetARGB( BYTE a, BYTE r, BYTE g, BYTE b );
	
	static _inl DWORD		ToARGB( BYTE a, BYTE r, BYTE g, BYTE b );
	static _inl DWORD		ToARGB( float a, float r, float g, float b );
	static _inl void		FromARGB( DWORD col, BYTE& a, BYTE& r, BYTE& g, BYTE& b );
	static _inl void		FromARGB( DWORD col, float& a, float& r, float& g, float& b );

	static _inl int			GetBytesPerPixel( ColorFormat cf );

	static _inl DWORD		ARGB4444ToARGB8888( DWORD color );
	static _inl DWORD		Gradient( DWORD a, DWORD b, float grad );
	
	void					Dump();
}; // class GPColor

#ifdef _INLINES
#include "gpColor.inl"
#endif // _INLINES

#endif // __GPCOLOR_H__