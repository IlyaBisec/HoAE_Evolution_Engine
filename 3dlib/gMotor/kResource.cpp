/*****************************************************************************/
/*    File:    kResource.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    27.02.2003
/*****************************************************************************/
#include "stdafx.h"
#include "kHash.hpp"
#include "kResource.h"

#ifndef _INLINES
#include "kResource.inl"
#endif // !_INLINES

DWORD BaseResource::curCacheFactor = 1;

void BaseResource::Dump()
{
}

