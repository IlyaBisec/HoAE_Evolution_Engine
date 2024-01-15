/*****************************************************************/
/*  File:   kCache.cpp
/*  Desc:   
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#include "stdafx.h"
#include "kCache.h"

#ifndef _INLINES
#include "kCache.inl"
#endif // !_INLINES

/*****************************************************************/
/*  Cache implementation
/*****************************************************************/
Cache::Cache()
{
    memUsed = 0;
}

Cache::~Cache()
{}

bool Cache::ReserveSpace( int numBytes )
{
    return true;    
}

/*****************************************************************/
/*  Cached implementation
/*****************************************************************/
Cached::Cached() : data( 0 ), LRU( 0 )
{}

Cached::~Cached()
{}