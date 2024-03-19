/*****************************************************************
/*  File:   gpColor.cpp                                            *
/*  Desc:                                                        *
/*  Author: Silver, Copyright (C) GSC Game World                 *
/*  Date:   January 2002                                         *
/*****************************************************************/
#include "stdafx.h"

#ifndef _INLINES
#include "gpColor.inl"
#endif // !_INLINES

/*****************************************************************
/*	gpColor implementation
/*****************************************************************/
GPColor::GPColor() : a(0.0f), r(0.0f), g(0.0f), b(0.0f)
{
}

void GPColor::Dump()
{
	Log.Message( "a:%f r:%f g:%f b:%f", a, r, g, b );
}


