/*****************************************************************
/*  File:   mUtil.cpp                                      
/*  Author: Silver, Copyright (C) GSC Game World                
/*****************************************************************/
#include "stdafx.h"
#include "mUtil.h"

#ifndef _INLINES
#include "mUtil.inl"
#endif // !_INLINES


//-------------------------------------------------------------------------------
//  Func:  _ftol
//  Desc:  Replace for standard float-to-int conversion function
//-------------------------------------------------------------------------------
extern "C" __declspec(naked) long _ftol( float arg )
{
    __asm{
        // store as a quadword int and reload
        fld     st(0)                  // X X
        fistp   QWORD PTR [esp-8]      // X
        fild    QWORD PTR [esp-8]      // X [X]
        mov     edx,DWORD PTR [esp-4]
        mov     eax,DWORD PTR [esp-8]
        test    eax,eax
        je      maybe_zero

        // number isn't zero, so get X - [X]
not_zero:
        fsubp   st(1),st               // X - [X]
        test    edx,edx
        jns     positive

        // number < 0 - inc eax if X - [X] is >0
        fstp    DWORD PTR [esp-8]
        mov     ecx,DWORD PTR [esp-8]  // get IEEE rep
        xor     ecx,80000000h          // now <0 if diff >0
        add     ecx,7FFFFFFFh          // carry if it was 00000001 to 7FFFFFFF
        adc     eax,0                  // add carry in
        ret

positive:
        // number > 0 - dec eax if X - [X] is <0
        fstp    DWORD PTR [esp-8]
        mov     ecx,DWORD PTR [esp-8]  // get IEEE rep
        add     ecx,7FFFFFFFh          // carry if it was 80000001 to FFFFFFFF
        sbb     eax,0                  // sub carry
        ret

maybe_zero:
        test    edx,7FFFFFFFh
        jnz     not_zero

        // number is zero - clear the stack
        fstp    st(0)
        fstp    st(0)
        ret
    }
} // _ftol







