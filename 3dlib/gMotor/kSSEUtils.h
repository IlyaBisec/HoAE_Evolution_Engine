/*****************************************************************************/
/*    File:    kSSEUtils.h
/*    Desc:    Unique universal identifier wrapper
/*    Author:    Manowar
/*    Date:    Oct 2004
/*****************************************************************************/
#ifndef __KSSEUTILS_H__
#define __KSSEUTILS_H__

__forceinline void* SSE_memcpy(void *dest,const void *src,size_t count)
{
    __asm{
    // Assuming we have SSE-capable processor and count is 4-byte aligned
        mov            esi,DWORD PTR [src]
        mov            edi,DWORD PTR [dest]
        mov            eax,DWORD PTR [count]
        ;
        prefetchnta    BYTE PTR [esi]
        ;
        mov            ebx,edi
        and            ebx,1111b
        jz            _al_start_
        mov            ecx,10000b
        sub            ecx,ebx
        sub            eax,ecx
        shr            ecx,2
_pr_loop_:
        mov            edx,DWORD PTR [esi]
        mov            DWORD PTR [edi],edx
        add            esi,4
        add            edi,4
        dec            ecx
        jnz            _pr_loop_
_al_start_:
        mov            edx,eax
        shr            edx,7
        jz            _vs_start_
        mov            ecx,edx
        test        esi,1111b
        jz            _bga_loop_
_bgu_loop_:
        movups        xmm0,XMMWORD PTR [esi+00]
        movups        xmm1,XMMWORD PTR [esi+16]
        movups        xmm2,XMMWORD PTR [esi+32]
        movups        xmm3,XMMWORD PTR [esi+48]
        movups        xmm4,XMMWORD PTR [esi+64]
        movups        xmm5,XMMWORD PTR [esi+80]
        movups        xmm6,XMMWORD PTR [esi+96]
        movups        xmm7,XMMWORD PTR [esi+112]
        ;
        movntps        XMMWORD PTR [edi+00],xmm0
        movntps        XMMWORD PTR [edi+16],xmm1
        movntps        XMMWORD PTR [edi+32],xmm2
        movntps        XMMWORD PTR [edi+48],xmm3
        movntps        XMMWORD PTR [edi+64],xmm4
        movntps        XMMWORD PTR [edi+80],xmm5
        movntps        XMMWORD PTR [edi+96],xmm6
        movntps        XMMWORD PTR [edi+112],xmm7
        ;
    add            esi,128
        add            edi,128
        dec            edx
        jnz            _bgu_loop_
        jmp short    _sm_start_
        ;
_bga_loop_:
        movaps        xmm0,XMMWORD PTR [esi+00]
        movaps        xmm1,XMMWORD PTR [esi+16]
        movaps        xmm2,XMMWORD PTR [esi+32]
        movaps        xmm3,XMMWORD PTR [esi+48]
        movaps        xmm4,XMMWORD PTR [esi+64]
        movaps        xmm5,XMMWORD PTR [esi+80]
        movaps        xmm6,XMMWORD PTR [esi+96]
        movaps        xmm7,XMMWORD PTR [esi+112]
        ;
        movntps        XMMWORD PTR [edi+00],xmm0
        movntps        XMMWORD PTR [edi+16],xmm1
        movntps        XMMWORD PTR [edi+32],xmm2
        movntps        XMMWORD PTR [edi+48],xmm3
        movntps        XMMWORD PTR [edi+64],xmm4
        movntps        XMMWORD PTR [edi+80],xmm5
        movntps        XMMWORD PTR [edi+96],xmm6
        movntps        XMMWORD PTR [edi+112],xmm7
        ;
    add            esi,128
        add            edi,128
        dec            edx
        jnz            _bga_loop_
_sm_start_:
        shl            ecx,7
        sub            eax,ecx
        jz            _loop_ok_
_vs_start_:
        shr            eax,2
_sm_loop_:
        mov            ecx,DWORD PTR [esi]
        mov            DWORD PTR [edi],ecx
        add            esi,4
        add            edi,4
        dec            eax
        jnz            _sm_loop_
_loop_ok_:
        sfence    
    };
    return dest;
}; // SSE_memcpy

#endif // __KSSEUTILS_H__