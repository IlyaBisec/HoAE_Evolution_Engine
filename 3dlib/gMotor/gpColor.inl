/*****************************************************************
/*  File:   GPColor.inl                                           
/*  Desc:   Color operations inlines
/*  Author: Silver, Copyright (C) GSC Game World                 
/*  Date:   January 2002                                         
/*****************************************************************/
#include "mConstants.h"
#include "mUtil.h"
/*****************************************************************
/*	GPColor implementation
/*****************************************************************/
_inl DWORD GPColor::ToARGB( BYTE a, BYTE r, BYTE g, BYTE b )
{
		return  (
				((a & 0xFF) << 24) | 
				((r & 0xFF) << 16) |
				((g & 0xFF) << 8)  |
				(b & 0xFF)
				);
}

_inl DWORD GPColor::ToARGB( float a, float r, float g, float b )
{
	clamp( a, 0.0f, 1.0f );
	clamp( r, 0.0f, 1.0f );
	clamp( g, 0.0f, 1.0f );
	clamp( b, 0.0f, 1.0f );

	BYTE ba, br, bg, bb;
	ba = (BYTE)(a * 255.0f);
	br = (BYTE)(r * 255.0f);
	bg = (BYTE)(g * 255.0f);
	bb = (BYTE)(b * 255.0f);
	return ToARGB( ba, br, bg, bb );
} // GPColor::ToARGB

_inl void GPColor::FromARGB( DWORD col, BYTE& a, BYTE& r, BYTE& g, BYTE& b )
{
	a = (col & 0xFF000000) >> 24;
	r = (col & 0x00FF0000) >> 16;
	g = (col & 0x0000FF00) >> 8;
	b = (col & 0x000000FF);
}

_inl void GPColor::FromARGB( DWORD col, float& a, float& r, float& g, float& b )
{
	BYTE ba, br, bg, bb;
	FromARGB( col, ba, br, bg, bb );
	a = float( ba ) / 255.0f;
	r = float( br ) / 255.0f;
	g = float( bg ) / 255.0f;
	b = float( bb ) / 255.0f;
} // GPColor::FromARGB

_inl DWORD GPColor::Gradient( DWORD a, DWORD b, float grad )
{
	float aa, ar, ag, ab;
	float ba, br, bg, bb;

	FromARGB( a, aa, ar, ag, ab );
	FromARGB( b, ba, br, bg, bb );
	
	float ra = (ba - aa)* grad + aa;
	float rb = (bb - ab)* grad + ab;
	float rg = (bg - ag)* grad + ag;
	float rr = (br - ar)* grad + ar;
	
	return ToARGB( ra, rr, rg, rb );
} // GPColor::Gradient

_inl DWORD GPColor::ARGB4444ToARGB8888( DWORD color )
{
	return  ((color & 0xF000) << 16) | ((color & 0x0F00) << 12) |
			((color & 0x00F0) << 8)  | ((color & 0x000F) << 4);
}

_inl int GPColor::GetBytesPerPixel( ColorFormat cf )
{
	switch (cf)
	{
	case cfUnknown:		return 0;
	case cfARGB4444:	return 2;
	case cfXRGB1555:	return 2;
	case cfARGB8888:	return 4;
	case cfRGB565:		return 2;
	case cfA8:			return 1;
	case cfRGB888:		return 3;
	case cfXRGB8888:	return 4;
	case cfV8U8:		return 2;
	}
	return 0;
}
