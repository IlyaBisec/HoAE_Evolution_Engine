//---------------------------------------------------------------------------
#include "FLZCommon.h"

#pragma warning ( disable : 4731 )
//---------------------------------------------------------------------------
bool FLZDecompressMemory(unsigned char* _InData, unsigned char* _OutData, int _InLen, int _OutLen)
{
    static int ExpOffBits, ExpLenBits, RawByteBits, RawByteAdd, BaseOff;
    static int OutLen;
    static unsigned char *OutData, *InData;
    
    static unsigned char* Alphabet;
    static unsigned TabCnt[] = {(4<<8)|2, (5<<8)|3, (5<<8)|4};

    ExpOffBits = 4;
    ExpLenBits = 2;
    RawByteBits = 8;
    RawByteAdd = 0;
    BaseOff = 0;
    OutLen = _OutLen;
    OutData = _OutData;
    InData = _InData;

    unsigned char Flags = *InData++;

    if(Flags&1)
    {
        Alphabet = InData + *(unsigned*)(InData);
        InData += 4;
        int Alphas = (*Alphabet++) + 1;
    }
    else
        Alphabet = NULL;

    if(Flags&2) 
        TabCnt[1] = (5<<8)|3;
    else 
        TabCnt[1] = (4<<8)|3;

    if(Flags&4) 
    {
        TabCnt[0] = (4<<8)|3;
        ExpLenBits = 3;
    }
    else 
    {
        TabCnt[0] = (4<<8)|2;
        ExpLenBits = 2;
    }

    __asm {
        push    ebp
        mov    esi,InData
        mov    edi,OutData
        mov    ebx,[esi]
        add    InData,4
        mov    ebp,[esi+4]
        add    InData,4
        bswap    ebx
        bswap    ebp
        xor    edx,edx
        cld

MainLoop:

//        call    LoadData
        // - dl содержит число использованных битов
        // - dh содержит число занятых у источника битов
        mov    ecx,edx            
        shr    ebx,cl
        shld    ebx,ebp,cl
        shr    edx,8
        shl    ebp,cl
        add    dl,cl

        cmp    edx,32
        jb        _LoadData1

        mov    eax,InData
        mov    ebp,[eax]
        add    InData,4
        bswap    ebp

        sub    edx,32
        mov    ecx,edx
        shr    ebx,cl
        shld    ebx,ebp,cl
        shl    ebp,cl
_LoadData1:    
        shl    edx,8

        // - get the one bit
        inc    edx
        shl    ebx,1 
        jnc    RawByte

        // - Off exponenta
        mov    ecx,ExpOffBits
        add    edx,ecx
        mov    eax,ebx
        shl    ebx,cl
        sub    ecx,32
        neg    ecx
        shr    eax,cl
        mov    ecx,eax

        // - Off mantissa
        xor    esi,esi
        test    ecx,ecx
        jz        M_1
        dec    ecx
        inc    esi
        shld    esi,ebx,cl
        shl    ebx,cl
        add    edx,ecx
M_1:

//        call    LoadData
        // - dl содержит число использованных битов
        // - dh содержит число занятых у источника битов
        mov    ecx,edx            
        shr    ebx,cl
        shld    ebx,ebp,cl
        shr    edx,8
        shl    ebp,cl
        add    dl,cl

        cmp    edx,32
        jb        _LoadData2

        mov    eax,InData
        mov    ebp,[eax]
        add    InData,4
        bswap    ebp

        sub    edx,32
        mov    ecx,edx
        shr    ebx,cl
        shld    ebx,ebp,cl
        shl    ebp,cl
_LoadData2:    
        shl    edx,8

        // - Cnt switch
        cmp    esi,FCNTOFFFLAG1
        je        CntSwitch
        cmp    esi,FCNTOFFFLAG2
        je        CntSwitch
        cmp    esi,FCNTOFFFLAG3
        je        CntSwitch
        
        // - Raw switch
        cmp    esi,FRAWOFFFLAG
        jz        RawSwitch

        // - Len exponenta
        mov    ecx,ExpLenBits
        add    edx,ecx
        mov    eax,ebx
        shl    ebx,cl
        sub    ecx,32
        neg    ecx
        shr    eax,cl
        mov    ecx,eax

        // - Len mantissa
        xor    eax,eax
        test    ecx,ecx
        jz        M_2
        dec    ecx
        inc    eax
        shld    eax,ebx,cl
        shl    ebx,cl
        add    edx,ecx
M_2:

        // - Rle
        cmp    esi,FRLEOFFFLAG
        je        Rle

        // - Base
        cmp    esi,FBASEOFFFLAG
        jne    LZ

        // - OffIdx exponenta
        xor    ecx,ecx
        add    edx,3
        shld    ecx,ebx,3
        shl    ebx,3

        // - OffIdx mantissa
        xor    esi,esi
        test    ecx,ecx
        jz        M_3
        dec    ecx
        inc    esi
        shld    esi,ebx,cl
        shl    ebx,cl
        add    edx,ecx
M_3:
        mov    ecx,BaseOff
        shl    ecx,FOFFIDXBITS
        add    esi,ecx

LZ:            
        // - update the BaseOff
        mov    ecx,esi
        shr    ecx,FOFFIDXBITS
        mov    BaseOff,ecx
        // - correct Off and Len values
        add    eax,FMINSTRLEN
        sub    esi,FRESERVEDOFF
        add    esi,eax
        // - prepare to transfer match
        mov    ecx,eax
        sub    esi,edi
        neg    esi

        add    ecx,3
        push    eax
        shr    ecx,2
_movsd:
        mov    eax,[esi]
        mov    [edi],eax
        add    esi,4
        add    edi,4
        dec    ecx
        jnz    _movsd

        pop    eax
        mov    ecx,eax
        add    ecx,3
        add    edi,eax
        and    ecx,0xFFFFFFFC
        sub    edi,ecx

        sub    OutLen,eax
        jnz    MainLoop
        jmp    Exit

Rle:
        // - Rle
        add    eax,FMINRLELEN
        mov    ecx,eax

        push    eax
        inc    ecx
        mov    al,[edi-1]
        shr    ecx,1
        mov    ah,al
_stosw:
        mov    [edi],eax
        inc    edi
        inc    edi
        dec    ecx
        jnz    _stosw

        pop    eax
        mov    ecx,eax
        and    ecx,1
        sub    edi,ecx

        sub    OutLen,eax
        jnz    MainLoop
        jmp    Exit

RawByte:
        // - get the one byte
        xor    eax,eax
        mov    ecx,RawByteBits
        shld    eax,ebx,cl
        add    edx,ecx
        shl    ebx,cl
        add    eax,RawByteAdd
        mov    esi,Alphabet
        test    esi,esi
        jz        _putRaw
        add    esi,eax
        mov    al,[esi]
_putRaw:
        mov    [edi],al
        inc    edi
        
        dec    OutLen
        jnz    MainLoop
        jmp    Exit

RawSwitch:
        // - RawSwitch exponenta
        mov    ecx,4
        add    edx,ecx
        mov    eax,ebx
        shl    ebx,cl
        sub    ecx,32
        neg    ecx
        shr    eax,cl
        mov    ecx,eax

        // - RawSwitch mantissa
        xor    eax,eax
        test    ecx,ecx
        jz        M_4
        dec    ecx
        inc    eax
        shld    eax,ebx,cl
        shl    ebx,cl
        add    edx,ecx
M_4:
        // - raw switch
        push    edx
        xor    edx,edx
        mov    ecx,9
        div    ecx
        mov    RawByteAdd,eax
        mov    RawByteBits,edx
        pop    edx
        jmp    MainLoop

        // - context switch
CntSwitch:
        mov    eax,esi
        sub    eax,FCNTOFFFLAG1
        lea    esi,TabCnt
        mov    eax,[esi+eax*4]
        mov    byte ptr ExpOffBits,ah
        mov    byte ptr ExpLenBits,al
        jmp    MainLoop

Exit:
        pop    ebp
    };
    return true;
}
//---------------------------------------------------------------------------
