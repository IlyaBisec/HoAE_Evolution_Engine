//  Modified by Silver, 22.03.2002:
//		- lzo1x_decompress_asm_fast body implemented here, as inline assembly
#include "stdafx.h"

__declspec ( naked ) 
int lzo1x_decompress_asm_fast(unsigned char* src, unsigned int src_len,
                              unsigned char* dst, unsigned int* dst_len,
                              unsigned char* wrkmem)
{
	__asm{
                push    ebp
                push    edi
                push    esi
                push    ebx
                push    ecx
                push    edx
                sub     esp,0000000cH
                cld
                mov     esi,[esp + 28H]
                mov     edi,[esp + 30H]
                mov     ebp,00000003H
                xor     eax,eax
                xor     ebx,ebx
                lodsb
                cmp     al,11H
                jbe     L6
                sub     al,0eH
                jmp     L7
L3:             add     eax,000000ffH
L4:             mov     bl,[esi]
                inc     esi
                or      bl,bl
                je      L3
                lea     eax,[eax + ebx + 15H]
                jmp     L7
                mov     esi,esi
L5:             mov     al,[esi]
                inc     esi
L6:             cmp     al,10H
                jae     L9
                or      al,al
                je      L4
                add     eax,00000006H
L7:             mov     ecx,eax
                xor     eax,ebp
                shr     ecx,02H
                and     eax,ebp
L8:             mov     edx,[esi]
                add     esi,00000004H
                mov     [edi],edx
                add     edi,00000004H
                dec     ecx
                jne     L8
                sub     esi,eax
                sub     edi,eax
                mov     al,[esi]
                inc     esi
                cmp     al,10H
                jae     L9
                shr     eax,02H
                mov     bl,[esi]
                lea     edx,[edi - 801H]
                lea     eax,[eax + ebx*4]
                inc     esi
                sub     edx,eax
                mov     ecx,[edx]
                mov     [edi],ecx
                add     edi,ebp
                jmp     L16
L9:             cmp     al,40H
                jb      L12
                mov     ecx,eax
                shr     eax,02H
                lea     edx,[edi - 1H]
                and     eax,00000007H
                mov     bl,[esi]
                shr     ecx,05H
                lea     eax,[eax + ebx*8]
                inc     esi
                sub     edx,eax
                add     ecx,00000004H
                cmp     eax,ebp
                jae     L14
                jmp     L17
L10:            add     eax,000000ffH
L11:            mov     bl,[esi]
                inc     esi
                or      bl,bl
                je      L10
                lea     ecx,[eax + ebx + 24H]
                xor     eax,eax
                jmp     L13
                nop
L12:            cmp     al,20H
                jb      L20
                and     eax,0000001fH
                je      L11
                lea     ecx,[eax + 5H]
L13:            mov     ax,[esi]
                lea     edx,[edi - 1H]
                shr     eax,02H
                add     esi,00000002H
                sub     edx,eax
                cmp     eax,ebp
                jb      L17
L14:            lea     eax,[edi + ecx - 3H]
                shr     ecx,02H
L15:            mov     ebx,[edx]
                add     edx,00000004H
                mov     [edi],ebx
                add     edi,00000004H
                dec     ecx
                jne     L15
                mov     edi,eax
                xor     ebx,ebx
L16:            mov     al,[esi - 2H]
                and     eax,ebp
                je      L5
                mov     edx,[esi]
                add     esi,eax
                mov     [edi],edx
                add     edi,eax
                mov     al,[esi]
                inc     esi
                jmp     L9
                lea     esi,[esi + 0H]
L17:            xchg    edx,esi
                sub     ecx,ebp
                repe    movsb
                mov     esi,edx
                jmp     L16
L18:            add     ecx,000000ffH
L19:            mov     bl,[esi]
                inc     esi
                or      bl,bl
                je      L18
                lea     ecx,[ebx + ecx + 0cH]
                jmp     L21
                lea     esi,[esi + 0H]
L20:            cmp     al,10H
                jb      L22
                mov     ecx,eax
                and     eax,00000008H
                shl     eax,0dH
                and     ecx,00000007H
                je      L19
                add     ecx,00000005H
L21:            mov     ax,[esi]
                add     esi,00000002H
                lea     edx,[edi - 4000H]
                shr     eax,02H
                je      L23
                sub     edx,eax
                jmp     L14
                lea     esi,[esi + 0H]
L22:            shr     eax,02H
                mov     bl,[esi]
                lea     edx,[edi - 1H]
                lea     eax,[eax + ebx*4]
                inc     esi
                sub     edx,eax
                mov     al,[edx]
                mov     [edi],al
                mov     bl,[edx + 1H]
                mov     [edi +1H ],bl
                add     edi,00000002H
                jmp     L16
L23:            cmp     ecx,00000006H
                setne   al
                mov     edx,[esp + 28H]
                add     edx,[esp + 2cH]
                cmp     esi,edx
                ja      L26
                jb      L25
L24:            sub     edi,[esp + 30H]
                mov     edx,[esp + 34H]
                mov     [edx],edi
                neg     eax
                add     esp,0000000cH
                pop     edx
                pop     ecx
                pop     ebx
                pop     esi
                pop     edi
                pop     ebp
                ret
                mov     eax,00000001H
                jmp     L24
L25:            mov     eax,00000008H
                jmp     L24
L26:            mov     eax,00000004H
                jmp     L24
                nop
	}
}