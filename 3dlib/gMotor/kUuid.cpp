/*****************************************************************************/
/*    File:    kUuid.cpp
/*    Desc:    Unique universal identifier wrapper
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10-04-2004
/*****************************************************************************/
#include "stdafx.h"
#include <objbase.h>
#include "rpcdce.h"
#include "kUuid.h"

/*****************************************************************************/
/*    uuid implementation
/*****************************************************************************/
uuid::uuid()
{
    memset( _uuid, 0, sizeof( _uuid ) );
}

bool uuid::create()
{
    UUID id;
    HRESULT hr;
    hr = UuidCreate( &id );
    if (hr != RPC_S_OK) return false;
    memcpy( _uuid, &id, sizeof(_uuid) );
    return true;
} // uuid::Create

const char*    uuid::asString() const
{
    UUID* pID = (UUID*)&_uuid;
    static char str[128]; str[0] = 0;
    unsigned char* sTemp;
    HRESULT hr = UuidToString( pID, &sTemp );
    if (hr != RPC_S_OK) return "";
    strcpy( str, (char*)sTemp );
    RpcStringFree( &sTemp );
    return str;
} // uuid::AsString

bool uuid::fromString( const char* str )
{
    HRESULT hr = UuidFromString( (unsigned char*)str, (UUID*)(&_uuid[0]) );
    return (hr == RPC_S_OK);
} // uuid::FromString

DWORD uuid::hash()
{
    static const DWORD c_Prime0        = 127;
    static const DWORD c_Prime01    = 251;
    static const DWORD c_Prime02    = 379;
    static const DWORD c_Prime03    = 541;
    static const DWORD c_Prime1        = 15731;
    static const DWORD c_Prime2        = 789221;
    static const DWORD c_Prime3        = 1376312589;

    DWORD& b0 = *((DWORD*)&_uuid[0] );
    DWORD& b1 = *((DWORD*)&_uuid[4] );
    DWORD& b2 = *((DWORD*)&_uuid[8] );
    DWORD& b3 = *((DWORD*)&_uuid[12]);
    DWORD h = b0 + b1*c_Prime0 - b2*c_Prime01 + b3*c_Prime02 + c_Prime03;
    h = (h << 13) ^ h;
    h *= h * h * c_Prime1 + c_Prime2;
    h += c_Prime3;
    h ^= h >> 16;
    h ^= h >> 8;
    return h;
} // uuid::hash