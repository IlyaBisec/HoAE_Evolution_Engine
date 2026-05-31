//---------------------------------------------------------------------------
// Fal`s Graphics Array
// (c) Frolov Andrey, 2001-2004
// Real-time decompress routines
//---------------------------------------------------------------------------
unsigned __int64 mask_000F = 0x000F000F000F000F;
unsigned __int64 mask_F000 = 0xF000F000F000F000;
unsigned __int64 mask_0001 = 0x0001000100010001;
unsigned __int64 mask_1000 = 0x1000100010001000;
unsigned __int64 mask_0000 = 0x0000000000000000;
//-------------------------------------------------------------------------
void G16PaintNationColor(int R, int G, int B, int Range, unsigned short* InData)
{
   unsigned Count = Range*Range>>2;

   R &= 0xF0;
   G &= 0xF0;
   B &= 0xF0;

   __asm {
      mov   ecx,Count
      mov   esi,InData
      movq  mm3,mask_000F
MainLoop:
      mov   eax,[esi]
      mov   ebx,[esi+4]

      // - взяли два пиксела
      movd  mm0,eax           // m0 = 0000 0000 1234 5678
      // - взяли еще два пиксела
      movd  mm1,ebx           // m1 = 0000 0000 9abc def0
      // - упаковали четыре пиксела в один регистр
      punpckldq mm0,mm1       // m0 = 9abc def0 1234 5678

      test  eax,0x10001000
      jnz   PresentN
      test  ebx,0x10001000
      jz    AbsentN

PresentN:
      // - сохраним пикселы на будущее
      movq  mm7,mm0           // m7 = 9abc def0 1234 5678
      // - выделим альфу, теперь mm0 содержит альфу четырех пикселей
      psrlw mm0,12            // m0 = 0009 000d 0001 0005
      // - сохраним исходную альфу
      movq  mm1,mm0           // m1 = 0009 000d 0001 0005
      movq  mm2,mm0           // m2 = 0009 000d 0001 0005
      // - определим, где нац. цвет
      pand  mm1,mask_0001
      // - получим маску нац. цвета
      pcmpeqw mm1,mask_0001
      // - избавимся от бита нац. цвета
      // - обнулим альфу там, где нет нац. цвета
      // - установим F там, где есть нац. цвет
      psrlw mm0,1
      por   mm2,mm1
      psllw mm0,1
      pand  mm2,mm3
      pand  mm0,mm1

      // - mm0 содержит нац. альфу с 0 в ненац. пикселах
      // - mm1 содержит исходную альфу с F в нац. пикселах

      // - получим умноженный нац. цвет
      movd  mm6,R             // m6 = 0000 0000 0000 00RR
      movd  mm5,G             // m5 = 0000 0000 0000 00GG
      movd  mm4,B             // m4 = 0000 0000 0000 00BB
      punpcklbw mm6,mm6       // m6 = 0000 0000 0000 RRRR
      punpcklbw mm5,mm5       // m5 = 0000 0000 0000 GGGG
      punpcklbw mm4,mm4       // m4 = 0000 0000 0000 BBBB
      punpcklwd mm6,mm6       // m6 = 0000 0000 RRRR RRRR
      punpcklwd mm5,mm5       // m5 = 0000 0000 GGGG GGGG
      punpcklwd mm4,mm4       // m4 = 0000 0000 BBBB BBBB
      punpckldq mm6,mm6       // m6 = RRRR RRRR RRRR RRRR
      punpckldq mm5,mm5       // m5 = GGGG GGGG GGGG GGGG
      punpckldq mm4,mm4       // m4 = BBBB BBBB BBBB BBBB
      psllw mm0,4             // m0 = 00a0 00a0 00a0 00a0
      psrlw mm6,8             // m6 = 00RR 00RR 00RR 00RR
      psrlw mm5,8             // m5 = 00GG 00GG 00GG 00GG
      psrlw mm4,8             // m4 = 00BB 00BB 00BB 00BB
      // - умножение нац. цвета на альфу
      pmullw mm6,mm0          // m6 = rrrr rrrr rrrr rrrr
      pmullw mm5,mm0          // m5 = gggg gggg gggg gggg
      pmullw mm4,mm0          // m4 = bbbb bbbb bbbb bbbb
      psrlw mm6,8             // m6 = 00rr 00rr 00rr 00rr
      psrlw mm5,8             // m5 = 00gg 00gg 00gg 00gg
      psrlw mm4,8             // m4 = 00bb 00bb 00bb 00bb

      // - распакуем цвета четырех пикселей
      movq  mm0,mm7           // m0 = 9abc def0 1234 5678
      movq  mm1,mm7           // m1 = 9abc def0 1234 5678
      psrlw mm0,8             // m0 = 009a 00de 0012 0056
      psrlw mm1,4             // m1 = 09ab 0def 0123 0567
      pand  mm0,mm3           // m0 = 000a 000e 0002 0006
      pand  mm1,mm3           // m1 = 000b 000f 0003 0007
      pand  mm7,mm3           // m7 = 000c 0000 0004 0008
      psllw mm0,4             // m0 = 00r0 00r0 00r0 00r0
      psllw mm1,4             // m1 = 00g0 00g0 00g0 00g0
      psllw mm7,4             // m7 = 00b0 00b0 00b0 00b0

      // - сложение с умноженным нац. цветом
      paddusb mm6,mm0         // m6 = 000r 000r 000r 000r
      paddusb mm5,mm1         // m5 = 000g 000g 000g 000g
      paddusb mm4,mm7         // m4 = 000b 000b 000b 000b
      psrlw mm6,4             // m6 = 000r 000r 000r 000r
      psrlw mm5,4             // m5 = 000g 000g 000g 000g
      psrlw mm4,4             // m4 = 000b 000b 000b 000b

      psllw mm6,8             // m6 = 0r00 0r00 0r00 0r00
      psllw mm5,4             // m5 = 00g0 00g0 00g0 00g0

      // - сборка четырех пикселей
      por   mm6,mm5           // m6 = 0rg0 0rg0 0rg0 0rg0
      por   mm6,mm4           // m6 = 0rgb 0rgb 0rgb 0rgb
      psllw mm2,12            // m2 = a000 a000 a000 a000
      por   mm6,mm2           // m6 = argb argb argb argb

      jmp   ContinueN

AbsentN:
      movq  mm6,mm0           // m6 = 9abc def0 1234 5678
      // - обнулим регистр    // m7 = 0000 0000 0000 0000
      pxor  mm7,mm7
      // - выделим альфу
      pand  mm0,mask_F000     // m0 = 9000 d000 1000 5000
      // - получим маску для ненулевой альфы
      pcmpeqw mm0,mm7
      // - получим младшие биты для установки в альфе
      pandn mm0,mask_1000
      // - установим младшие биты в ненулевой альфе
      por   mm6,mm0

ContinueN:
      movq  [esi],mm6
      add   esi,8

      dec   ecx
      jz    ExitLoop
      jmp   MainLoop

ExitLoop:
      emms
   }
}
//---------------------------------------------------------------------------
unsigned G15UnpackSquare(unsigned short* InData, unsigned short* OutData, int R)
{
   int RleLen = 0, InPos = 0;
   for(int i = 0; i < R*R; i++)
   {
         unsigned col = 0;
      if(RleLen == 0)
      {
         col = InData[InPos++];
         if((col & 0xF000) == 0x1000)
         {
            RleLen = col&0xFFF;
            col = 0;
         }
      }
      else
         RleLen--;
      OutData[i] = col;
   }
   return InPos;
}
//------------------------------------------------------------------------------

