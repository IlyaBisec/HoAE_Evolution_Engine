/*****************************************************************
/*  File:   kUtilities.cpp                                       *
/*  Desc:   Helper routines implementation                       *
/*  Author: Silver, Copyright (C) GSC Game World                 *
/*  Date:   December 2001                                        *
/*****************************************************************/
#include "stdafx.h"
#include "kUtilities.h"

#ifndef _INLINES
#include "kUtilities.inl"
#endif // _INLINES

#include <string>

using namespace std;

BYTE  util::powSidesByte[8] = { 1, 2, 4, 8, 16, 32, 64, 128 };
float  util::powSidesFloat[8] = { 1.0f, 2.0f, 4.0f, 8.0f, 
                                    16.0f, 32.0f, 64.0f, 128.0f };
float util::byteAngleCos[256];
float util::byteAngleSin[256];
util  util::dummy;

float util::PI        = 3.1415926f;
float util::DoublePI= 3.1415926f * 2.0f;
float util::PIdiv16 = 0.1963495f;
float util::PIdiv32 = 0.0981747f;
float util::PImul2    = 6.2831853f;
float util::sin30    = 0.5f;
WORD  util::quad8x8Idx[384];
int      util::rnd[1024];

    
/*---------------------------------------------------------------*
/*  Func:    util::util
/*    Desc:    Fills arrays of const values, e.g, sin, cos values
/*---------------------------------------------------------------*/
util::util()
{
    //  sin and cos
    for (int i = 0; i < 256; i++)
    {
        byteAngleCos[i] = static_cast<float>( cos( i * PI / 128 ) );
        byteAngleSin[i] = static_cast<float>( sin( i * PI / 128 ) );
    }

    //  vertex indices for 8x8 triangle grid
    int idx = 0;
    int cVert = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            quad8x8Idx[idx++] = cVert;
            quad8x8Idx[idx++] = cVert + 1;
            quad8x8Idx[idx++] = cVert + 10;
            quad8x8Idx[idx++] = cVert;
            quad8x8Idx[idx++] = cVert + 10;
            quad8x8Idx[idx++] = cVert + 9;
            cVert++;
        }
        cVert++;
    }

    //  init random uniform values
    for (int i = 0; i < 1024; i++)
    {
        rnd[i] = rand();
    }
}//  util::util


bool HaveCPUID() 
{
    DWORD res = 0;
    __asm{
        pushfd                    // save EFLAGS to stack
        pop        eax                // store EFLAGS in EAX
        mov        ebx, eax        // save in EBX for testing later
        xor        eax, 0x00200000    // switch bit 21
        push    eax                // copy changed value to stack
        popfd                    // save changed EAX to EFLAGS
        pushfd                    // push EFLAGS to top of stack
        pop        eax                // store EFLAGS in EAX
        cmp        eax, ebx        // see if bit 21 has changed
        jz        no_cpuid        // if no change, no CPUID
        mov        res, 1
    no_cpuid:
    }
    return (res != 0);
} // HaveCPUID

bool HaveMMX()
{
    if (!HaveCPUID()) return false;
    DWORD res = 0;
    __asm{
        mov        eax, 1                // request for feature flags
        cpuid
        test    edx, 0x00800000        // MMX technology bit flag is 23th
        jz        no_mmx
        mov        res, 1
    no_mmx:
    }
    return (res != 0);
} // HaveMMX

bool Have3DNow()
{
    if (!HaveCPUID()) return false;
    DWORD res = 0;
    __asm{
        mov        eax, 0x80000001     // request for feature flags
        cpuid
        test    edx, 0x80000000        // 3DNow! technology bit flag is 30th
        jz        no_3dnow
        mov        res, 1
    no_3dnow:
    }
    return (res != 0);
} // Have3DNow

bool HaveSSE()
{
    if (!HaveCPUID()) return false;
    DWORD res = 0;
    __asm{
        mov        eax, 1                // request for feature flags
        cpuid
        test    edx, 0x02000000        // SSE technology bit flag is 25th
        jz        no_sse
        mov        res, 1
    no_sse:
    }
    return (res != 0);
} // HaveSSE

bool HaveSSE2() 
{
    if (!HaveCPUID()) return false;
    DWORD res = 0;
    __asm{
        mov        eax, 1                // request for feature flags
        cpuid
        test    edx, 0x04000000        // SSE2 technology bit flag is 26th
        jz        no_sse2
        mov        res, 1
    no_sse2:
    }
    return (res != 0);
} // HaveSSE2
