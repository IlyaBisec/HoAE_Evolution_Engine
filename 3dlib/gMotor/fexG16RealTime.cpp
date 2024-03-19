//---------------------------------------------------------------------------
// Fal`s Graphics Array
// (c) Frolov Andrey, 2001-2002
// Real-time decompress routines
//---------------------------------------------------------------------------
#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include "fexG16Common.h"

int lzo1x_decompress_asm_fast(unsigned char* src, unsigned int src_len,
                              unsigned char* dst, unsigned int* dst_len,
                              unsigned char* wrkmem);

bool ucl_decompress(const unsigned char* src, unsigned int src_len,
                    unsigned char *dst, unsigned int *dst_len);
//---------------------------------------------------------------------------
void G16PaintNationColor(int R, int G, int B, int Range, unsigned short* InData);
//---------------------------------------------------------------------------
unsigned NationR = 0, NationG = 255, NationB = 0;
void G16SetNationalColor(unsigned int r, unsigned int g, unsigned int b)
{
	NationR = r;
	NationG = g;
	NationB = b;
}
//---------------------------------------------------------------------------
unsigned short G16PalRGB[256];
void G16SetPalette(BYTE* pPal235, BYTE* pPalRGB)
{
	unsigned* pal = (unsigned*)pPalRGB;
	for(int i = 0; i < 256; i++)
	{
		unsigned color = pal[i];
      unsigned b = (color>>16)&0xFF;
      unsigned g = (color>>8)&0xFF;
      unsigned r = color&0xFF;
      unsigned ch = ((r&0xF0)<<4)|(g&0xF0)|(b>>4);
      G16PalRGB[i] = ch;
	}
}
//-------------------------------------------------------------------------
/*
   Функция распаковывает сегмент с кадрами, сегмент в виде:
      unsigned int outlen  // - размер разархивированного блока
      <compressed data>    // - заархивированные данные
   После разархивации получается набор фреймов, содержащих данные о
   квадратах, зависимо от метода упаковки.
*/
bool G16UnpackSegment(unsigned char* InData,       // - упакованные данные
                      unsigned int   InLen,        // - размер входного буфера
                      unsigned char* OutData,      // - распакованный сегмент
                      unsigned char* WorkData,     // - рабочий буфер
                      unsigned int*  FOffsData,    // - массив смещений кадров
                      unsigned int   FramesNumber, // - количество кадров в сегменте
                      unsigned int   Flags)        // - флаги (метод упаковки)
{
   unsigned int OutLen = *(unsigned int*)InData;
   InData += 4;

   //////////////////////////////////////////////////////////////////////////
   // Разархивируем raw-данные из inbuf в workbuf
   //////////////////////////////////////////////////////////////////////////
   if((Flags & G16_COMPRESS_METHOD_MASK) == G16_COMPRESSED_BY_UCL)
   {
   	if(!ucl_decompress(InData, InLen, WorkData, &OutLen))
         return false;
   }
   else
   if((Flags & G16_COMPRESS_METHOD_MASK) == G16_COMPRESSED_BY_LZO)
   {
   	if(lzo1x_decompress_asm_fast(InData, InLen, WorkData, &OutLen, WorkData) < 0)
         return false;
   }
   else
   if((Flags & G16_COMPRESS_METHOD_MASK) == G16_NONCOMPRESSED)
   {
      WorkData = InData;
      OutLen = InLen;
   }
   else
      return false;

   unsigned allsize = 0;
 	int workbuf_pos = 0;
   #define GETC() WorkData[workbuf_pos++]
   #define GETSH(a) {a = *(unsigned short*)&(WorkData[workbuf_pos]); workbuf_pos += 2;}

   if((Flags & G16_PACK_METHOD_MASK) == G16_IDXSTORE)
   {
      /*
         Структура сегмента для IdxStore:
         SEGMENT {
            DWORD ColorOffset;            // - относительное смещение в байтах до данных цвета
            DWORD AlphaOffset;            // - относительное смещение в байтах до альфы

            FRAME {
               WORD SquaresNumber;        // - количество квадратов в кадре
               SQUARE {
                  // - header 4 bytes
                  DWORD Pow	   :4; 	   //  side = 2^Pow
      	         DWORD Config	:4; 	   //  configuration of the polygon
	               DWORD X			:12;	   //  x-position of the chunk in sprite
                  DWORD Y  		:12;	   //  y-position of the chunk in sprite
               } Squares[SquaresNumber];
            } Frames[FramesNumber];

            ColorData[];                  // - массив цветовой информации для всего сегмента
            AlphaData[];                  // - массив информации о альфе для всего сегмента
         }
      */
      unsigned ch = 0, c_pos = 0, a_pos = 0, src_pos = 0, dst_pos = 0;
      #define GETINT(ch) ch = *((unsigned*)&(WorkData[src_pos])); src_pos += 4;
      #define GETSH(ch)  ch = *((unsigned short*)&(WorkData[src_pos])); src_pos += 2;
      #define PUTINT(ch) *((unsigned*)&(OutData[dst_pos])) = ch; dst_pos += 4;
      #define PUTSH(ch)  *((unsigned short*)&(OutData[dst_pos])) = ch; dst_pos += 2;
      #define PUTC(ch)   *((unsigned char*)&(OutData[dst_pos])) = ch; dst_pos += 1;

      GETINT(ch);
      unsigned char* ColorOffset = WorkData + src_pos + ch;
      GETINT(ch);
      unsigned char* AlphaOffset = WorkData + src_pos + ch;

      for(unsigned f = 0; f < FramesNumber; f++)
      {
         WORD SquaresNumber;
         GETSH(SquaresNumber);

         FOffsData[f] = dst_pos;   // - указатель на текущий кадр
         for(unsigned s = 0; s < SquaresNumber; s++)
         {
            GETINT(ch);
            PUTINT(ch);       // - заголовок квадрата
            PUTINT(0);        // - reserved
            int side = 1<<(ch>>28);

				unsigned short* CurrSqrStart = (unsigned short*)&(OutData[dst_pos]);
            for(int j = 0; j < side; j++)
            {
               for(int i = 0; i < side; i+=2)
               {
                  int aa = *AlphaOffset++;
                  int a1 = aa&0xF0;
                  int a2 = aa&0x0F;
                  if(a1) 
					ch = (a1<<8)|G16PalRGB[*ColorOffset++];
				  else
					ch = 0;
				  PUTSH(ch);

                  if(a2) 
					ch = (a2<<12)|G16PalRGB[*ColorOffset++];
				  else
					ch = 0;
				  PUTSH(ch);
			   }
            }
				G16PaintNationColor(NationR, NationG, NationB, side, CurrSqrStart);
         }
      }
   }
   else
   if((Flags & G16_PACK_METHOD_MASK) == G16_444STORE)
   {
      /*
         Структура сегмента для 444Store:
         SEGMENT {
            DWORD ColorOffset;            // - относительное смещение в байтах до данных цвета
            DWORD AlphaOffset;            // - относительное смещение в байтах до альфы

            FRAME {
               WORD SquaresNumber;        // - количество квадратов в кадре
               SQUARE {
                  // - header 4 bytes
                  DWORD Pow	   :4; 	   //  side = 2^Pow
      	         DWORD Config	:4; 	   //  configuration of the polygon
	               DWORD X			:12;	   //  x-position of the chunk in sprite
                  DWORD Y  		:12;	   //  y-position of the chunk in sprite
               } Squares[SquaresNumber];
            } Frames[FramesNumber];

            ColorData[];                  // - массив цветовой информации для всего сегмента
            AlphaData[];                  // - массив информации о альфе для всего сегмента
         }
      */
      unsigned ch = 0, c_pos = 0, a_pos = 0, src_pos = 0, dst_pos = 0;
      #define GETINT(ch) ch = *((unsigned*)&(WorkData[src_pos])); src_pos += 4;
      #define GETSH(ch)  ch = *((unsigned short*)&(WorkData[src_pos])); src_pos += 2;
      #define PUTINT(ch) *((unsigned*)&(OutData[dst_pos])) = ch; dst_pos += 4;
      #define PUTSH(ch)  *((unsigned short*)&(OutData[dst_pos])) = ch; dst_pos += 2;
      #define PUTC(ch)   *((unsigned char*)&(OutData[dst_pos])) = ch; dst_pos += 1;

      GETINT(ch);
      unsigned char* ColorOffset = WorkData + src_pos + ch;
      GETINT(ch);
      unsigned char* AlphaOffset = WorkData + src_pos + ch;

      for(unsigned f = 0; f < FramesNumber; f++)
      {
         WORD SquaresNumber;
         GETSH(SquaresNumber);

         FOffsData[f] = dst_pos;   // - указатель на текущий кадр
         for(unsigned s = 0; s < SquaresNumber; s++)
         {
            GETINT(ch);
            PUTINT(ch);       // - заголовок квадрата
            PUTINT(0);        // - reserved
            int side = 1<<(ch>>28);

            #undef MAKECOLOR
            #define MAKECOLOR(_a) {\
               unsigned color = *((unsigned short*)ColorOffset);\
               ColorOffset += 2;\
               ch = color;\
               ch |= ((_a&0xF0)<<8);\
            }

				unsigned short* CurrSqrStart = (unsigned short*)&(OutData[dst_pos]);
            for(int j = 0; j < side; j++)
            {
               for(int i = 0; i < side; i+=2)
               {
                  int aa = *AlphaOffset++;
                  int a1 = aa&0xF0;
                  int a2 = (aa<<4)&0xF0;
                  if(a1)
                  {
                     MAKECOLOR(a1);
                  }
                  else ch = 0;
                  PUTSH(ch);

                  if(a2)
                  {
                     MAKECOLOR(a2);
                  }
                  else ch = 0;
                  PUTSH(ch);
               }
            }
				G16PaintNationColor(NationR, NationG, NationB, side, CurrSqrStart);
         }
      }
   }
   else
      return false;
   // -----------------------------------------------------------------------
   return true;
}

//
//#include "stdafx.h"
//#include <windows.h>
//#include <stdio.h>
//#include "fexG16Common.h"
//
//int lzo1x_decompress_asm_fast(unsigned char* src, unsigned int src_len,
//                              unsigned char* dst, unsigned int* dst_len,
//                              unsigned char* wrkmem);
//
//bool ucl_decompress(const unsigned char* src, unsigned int src_len,
//                    unsigned char *dst, unsigned int *dst_len);
////---------------------------------------------------------------------------
//unsigned __int64 _DecompressTicks = 0;
////---------------------------------------------------------------------------
//unsigned __int64 mask_F0       = 0xF0F0F0F0F0F0F0F0;
//unsigned __int64 mask_0F       = 0x0F0F0F0F0F0F0F0F;
//unsigned __int64 mask_FF00     = 0xFF00FF00FF00FF00;
//unsigned __int64 mask_F000     = 0xF000F000F000F000;
//unsigned __int64 mask_0F00     = 0x0F000F000F000F00;
//unsigned __int64 mask_00F0     = 0x00F000F000F000F0;
//unsigned __int64 mask_000F     = 0x000F000F000F000F;
//unsigned __int64 mask_F0000000 = 0xF0000000F0000000;
//unsigned __int64 mask_0000F000 = 0x0000F0000000F000;
//unsigned __int64 mask_0001     = 0x0001000100010001;
//unsigned __int64 mask_10       = 0x1010101010101010;
//unsigned __int64 mask_7F       = 0x7F7F7F7F7F7F7F7F;
//unsigned __int64 mask_tmp      = 0;
//
//unsigned __int64 nr            = 0x00f000f000f000f0;
//unsigned __int64 ng            = 0x0000000000000000;
//unsigned __int64 nb            = 0x0000000000000000;
//
//// -- Inserted by Silver ---20.02.2003
//signed char*	G16Pal235 = 0;    // - указатель на палитру 2,3,5 каналов
//unsigned char*	G16PalRGB = 0;  // - указатель на палитру RGB канала
//
//void G16SetPalette(BYTE* pPal235, BYTE* pPalRGB)
//{
//	G16Pal235 = reinterpret_cast<signed char*>	( pPal235 );
//	G16PalRGB = reinterpret_cast<unsigned char*>( pPalRGB );
//}
//// -- end of change -- 20.02.2003
//
////-------------------------------------------------------------------------
////signed char G16Pal235[1024];    // - указатель на палитру 2,3,5 каналов
////unsigned char G16PalRGB[1024];  // - указатель на палитру RGB канала
//unsigned isNCPresent = 1;
////-------------------------------------------------------------------------
//
//void G16SetNationalColor(unsigned int R, unsigned int G, unsigned int B)
//{
//   __asm{
//      xor   eax,eax
//      mov   al,byte ptr R
//      and   al,0xF0
//      mov   ah,al
//      mov   ebx,eax
//      shl   eax,16
//      or    eax,ebx
//      movd  mm0,eax
//      punpcklwd mm0,mm0
//      movq  nr,mm0
//
//      xor   eax,eax
//      mov   al,byte ptr G
//      and   al,0xF0
//      mov   ah,al
//      mov   ebx,eax
//      shl   eax,16
//      or    eax,ebx
//      movd  mm0,eax
//      punpcklwd mm0,mm0
//      movq  ng,mm0
//
//      xor   eax,eax
//      mov   al,byte ptr B
//      and   al,0xF0
//      mov   ah,al
//      mov   ebx,eax
//      shl   eax,16
//      or    eax,ebx
//      movd  mm0,eax
//      punpcklwd mm0,mm0
//      movq  nb,mm0
//
//      emms
//   }
//}
////-------------------------------------------------------------------------
//void Decode3DSquare(unsigned char* sqr1, unsigned short* sqr2, unsigned short* sqr3,
//                    unsigned char* sqr_a, unsigned int r)
//{
//	// -- Inserted by Silver ---21.02.2003
//	// -- reason: Silver's paranoia
//	assert( G16Pal235 && G16PalRGB );
//	// -- end of change -- 21.02.2003
//
//   unsigned half_side = (1<<(r-1));
//   unsigned pos_rgb = 0;
//   unsigned pos_idx = half_side*half_side;
//
//   for(unsigned int j = 0; j < half_side; j++)
//   {
//      for(unsigned int i = 0; i < half_side; i++)
//      {
//         __asm{
//            mov   esi,sqr1
//
//            // - готовим предварительные суммы wavelet-коэффициентов
//            mov   ebx,pos_idx
//            xor   eax,eax
//            mov   al,[esi+ebx]   // idx
//            shl   eax,2          // - умножим на 4 для доступа к палитре
//            mov   ebx,dword ptr G16Pal235
//            add   ebx,eax
//
//            // mm6,mm7
//            mov   al,[ebx+2]     // b5
//            cbw
//            cwde
//            movd  mm6,eax        // m6 = xxxx xxxx ssss ssp5
//            punpcklwd mm6,mm6    // m6 = xxxx xxxx ssp5 ssp5
//            punpckldq mm6,mm6    // m6 = ssp5 ssp5 ssp5 ssp5
//            neg   eax
//            movd  mm7,eax        // m7 = xxxx xxxx ssss ssn5
//            punpcklwd mm7,mm7    // m7 = xxxx xxxx ssn5 ssn5
//            punpckldq mm7,mm7    // m7 = ssn5 ssn5 ssn5 ssn5
//
//            // mm0,mm1
//            mov   al,[ebx+0]     // b2
//            cbw
//            cwde
//            movd  mm0,eax        // m0 = xxxx xxxx 0000 ssp2
//            punpcklwd mm0,mm0    // m0 = xxxx xxxx ssp2 ssp2
//            neg   eax
//            movd  mm1,eax        // m1 = xxxx xxxx 0000 ssn2
//            punpcklwd mm1,mm1    // m1 = xxxx xxxx ssn2 ssn2
//            punpckldq mm0,mm1    // m0 = ssn2 ssn2 ssp2 ssp2
//            movq  mm1,mm0        // m1 = ssn2 ssn2 ssp2 ssp2
//
//            // mm4,mm5
//            mov   al,[ebx+1]     // b3
//            cbw
//            cwde
//            movd  mm4,eax        // m4 = xxxx xxxx 0000 ssp3
//            neg   eax
//            movd  mm5,eax        // m5 = xxxx xxxx 0000 ssn3
//            punpcklwd mm4,mm5    // m4 = xxxx xxxx ssn3 ssp3
//            punpckldq mm4,mm4    // m4 = ssn3 ssp3 ssn3 ssp3
//            movq  mm5,mm4        // m5 = ssn3 ssp3 ssn3 ssp3
//
//            // mm0 и mm1 содержат просчитанные суммы
//            paddsw mm0,mm4
//            paddsw mm1,mm7
//            paddsw mm0,mm6
//            paddsw mm1,mm5
//
//            // - складываем просчитанные суммы с RGB-каналом
//
//            xor   eax,eax
//            mov   ebx,pos_rgb
//            mov   al,[esi+ebx]
//            shl   eax,2          // - умножим на 4 для доступа к палитре
//            mov   ebx,dword ptr G16PalRGB
//            add   ebx,eax
//
//            xor   eax,eax
//            mov   al,[ebx+2]     // b1b
//            movd  mm7,eax        // m7 = xxxx xxxx 0000 00bb
//            punpcklwd mm7,mm7    // m7 = xxxx xxxx 00bb 00bb
//            punpckldq mm7,mm7    // m7 = 00bb 00bb 00bb 00bb
//            movq  mm4,mm7        // m4 = 00bb 00bb 00bb 00bb
//            paddsw mm7,mm0
//            paddsw mm4,mm1
//            packuswb mm7,mm4
//
//            mov   al,[ebx+1]     // b1g
//            movd  mm6,eax        // m6 = xxxx xxxx 0000 00bg
//            punpcklwd mm6,mm6    // m6 = xxxx xxxx 00bg 00bg
//            punpckldq mm6,mm6    // m6 = 00bg 00bg 00bg 00bg
//            movq  mm4,mm6        // m4 = 00bg 00bg 00bg 00bg
//            paddsw mm6,mm0
//            paddsw mm4,mm1
//            packuswb mm6,mm4
//
//            mov   al,[ebx+0]     // b1r
//            movd  mm5,eax        // m5 = xxxx xxxx 0000 00br
//            punpcklwd mm5,mm5    // m5 = xxxx xxxx 00br 00br
//            punpckldq mm5,mm5    // m5 = 00br 00br 00br 00br
//            movq  mm4,mm5        // m4 = 00br 00br 00br 00br
//            paddsw mm5,mm0
//            paddsw mm4,mm1
//            packuswb mm5,mm4     // m5 = 8877 6655 4433 2211
//
//            // mm5,mm6,mm7 содержат RGB для восьми пикселей
//
//            // preload alpha
//            mov   esi,sqr_a
//            mov   eax,[esi]
//
//            // alpha
//            movd  mm2,eax           // m2 = xxxx xxxx 8765 4321
//            punpcklbw mm2,mm2       // m2 = 87xx 65xx 43xx 21xx
//            punpckhwd mm2,mm2       // m2 = 87xx xxxx 65xx xxxx
//            movq  mm3,mm2           // m3 = 87xx xxxx 65xx xxxx
//            psrld mm2,28            // m2 = 0000 0008 0000 0006
//            psrld mm3,24            // m3 = 0000 0087 0000 0065
//            pslld mm2,28            // m2 = 8000 0000 6000 0000
//            psllw mm3,12            // m3 = 0000 7000 0000 5000
//            por   mm2,mm3           // m2 = 8000 7000 6000 5000
//
//            movd  mm3,eax           // m3 = xxxx xxxx 8765 4321
//            punpcklbw mm3,mm3       // m3 = 87xx 65xx 43xx 21xx
//            punpcklwd mm3,mm3       // m3 = 43xx xxxx 21xx xxxx
//            movq  mm4,mm3           // m4 = 43xx xxxx 21xx xxxx
//            psrld mm3,28            // m3 = 0000 0004 0000 0002
//            psrld mm4,24            // m4 = 0000 0043 0000 0021
//            pslld mm3,28            // m3 = 4000 0000 2000 0000
//            psllw mm4,12            // m4 = 0000 3000 0000 1000
//            por   mm3,mm4           // m3 = 4000 3000 2000 1000
//
//            mov   ebx,isNCPresent
//            or    ebx,ebx
//            jz    absent
//
//            // mm2,mm3 содержат альфу:
//            // m2 = 8000 7000 6000 5000
//            // m3 = 4000 3000 2000 1000
//            test  eax,0x11111111
//            jnz   a10
//absent:
//////////////// National color is absent //////////////////////////////////
//            // red
//            punpcklbw mm0,mm5    // m0 = 44xx 33xx 22xx 11xx
//            punpckhbw mm1,mm5    // m1 = 88xx 77xx 66xx 55xx
//            psrlw mm0,12         // m0 = 0004 0003 0002 0001
//            psrlw mm1,12         // m1 = 0008 0007 0006 0005
//            psllw mm0,8          // m0 = 0400 0300 0200 0100
//            psllw mm1,8          // m1 = 0800 0700 0600 0500
//            por mm0,mm2          // m0 = a400 a300 a200 a100
//            por mm1,mm3          // m1 = a800 a700 a600 a500
//
//            // green
//            punpcklbw mm2,mm6    // m2 = 44xx 33xx 22xx 11xx
//            punpckhbw mm3,mm6    // m3 = 88xx 77xx 66xx 55xx
//            psrlw mm2,12         // m2 = 0004 0003 0002 0001
//            psrlw mm3,12         // m3 = 0008 0007 0006 0005
//            psllw mm2,4          // m2 = 0040 0030 0020 0010
//            psllw mm3,4          // m3 = 0080 0070 0060 0050
//            por mm0,mm2          // m0 = a440 a330 a220 a110
//            por mm1,mm3          // m1 = a880 a770 a660 a550
//
//            // blue
//            punpcklbw mm2,mm7    // m2 = 44xx 33xx 22xx 11xx
//            punpckhbw mm3,mm7    // m3 = 88xx 77xx 66xx 55xx
//            psrlw mm2,12         // m2 = 0004 0003 0002 0001
//            psrlw mm3,12         // m3 = 0008 0007 0006 0005
//            por mm0,mm2          // m0 = a444 a333 a222 a111
//            por mm1,mm3          // m1 = a888 a777 a666 a555
//
//            // mm0 и mm1 содержат восемь двухбайтных пикселей
//
//            jmp   a20
//////////////// National color is present /////////////////////////////////
//a10:
//            // mm2,mm3 содержат альфу:
//            // m2 = 8000 7000 6000 5000
//            // m3 = 4000 3000 2000 1000
//            // mm5,mm6,mm7 содержат RGB для восьми пикселей
//
//            // - просчитываем альфу, там где н-ый цвет, а == 0xF
//
//            psrlw mm2,8             // m2 = 0080 0070 0060 0050
//            psrlw mm3,8             // m3 = 0040 0030 0020 0010
//
//            movq mm0,mm2            // m0 = 0080 0070 0060 0050
//            movq mm1,mm3            // m1 = 0040 0030 0020 0010
//            packuswb mm0,mm1        // m0 = 8070 6050 4030 2010
//            movq mm1,mm0            // m1 = 8070 6050 4030 2010
//            pand mm1,mask_10        // m1 = ?0?0 ?0?0 ?0?0 ?0?0
//            pcmpeqb mm1,mask_10     // m1 = ???? ???? ???? ????
//            pand mm1,mask_F0        // m1 = ?0?0 ?0?0 ?0?0 ?0?0
//            por mm0,mm1
//
//            // mm0 содержит готовую альфу
//
//            // - просчет масок для умножения на н-ый цвет
//
//            movq  mm4,mm2           // m4 = 0080 0070 0060 0050
//            psrlw mm4,4             // m4 = 0008 0007 0006 0005
//            pand  mm4,mask_0001     // m4 = 000? 000? 000? 000?
//            pcmpeqw mm4,mask_0001
//            pand mm2,mm4            // m2 = ???? ???? ???? ????
//
//            movq  mm4,mm3           // m4 = 0040 0030 0020 0010
//            psrlw mm4,4             // m4 = 0004 0003 0002 0001
//            pand  mm4,mask_0001     // m4 = 000? 000? 000? 000?
//            pcmpeqw mm4,mask_0001
//            pand mm3,mm4            // m3 = ???? ???? ???? ????
//
//            // mm2,mm3 содержат альфу:
//            // m2 = 0008 0007 0006 0005
//            // m3 = 0004 0003 0002 0001
//
//            // - умножаем на н-ый цвет
//
//            // Red
//            movq  mm4,nr            // m4 = 00nr 00nr 00nr 00nr
//            movq  mm1,mm4           // m1 = 00nr 00nr 00nr 00nr
//            pmullw mm4,mm2          // m4 = r800 r700 r600 r500
//            pmullw mm1,mm3          // m1 = r400 r300 r200 r100
//            psrlw mm4,8             // m4 = 00r8 00r7 00r6 00r5
//            psrlw mm1,8             // m1 = 00r4 00r3 00r2 00r1
//
//            // - получим восемь значений для R-компоненты н-го цвета
//            packuswb mm4,mm1        // m4 = rara rara rara rara
//            paddusb mm5,mm4         // m5 - сумма с национальным цветом
//
//            // Green
//            movq  mm4,ng            // m4 = 00ng 00ng 00ng 00ng
//            movq  mm1,mm4           // m1 = 00ng 00ng 00ng 00ng
//            pmullw mm4,mm2          // m4 = g800 g700 g600 g500
//            pmullw mm1,mm3          // m1 = g400 g300 g200 g100
//            psrlw mm4,8             // m4 = 00g8 00g7 00g6 00g5
//            psrlw mm1,8             // m1 = 00g4 00g3 00g2 00g1
//
//            // - получим восемь значений для G-компоненты н-го цвета
//            packuswb mm4,mm1        // m4 = gaga gaga gaga gaga
//            paddusb mm6,mm4         // m6 - сумма с национальным цветом
//
//            // Blue
//            movq  mm4,nb            // m4 = 00nb 00nb 00nb 00nb
//            movq  mm1,mm4           // m1 = 00nb 00nb 00nb 00nb
//            pmullw mm4,mm2          // m4 = b800 b700 b600 b500
//            pmullw mm1,mm3          // m1 = b400 b300 b200 b100
//            psrlw mm4,8             // m4 = 00b8 00b7 00b6 00b5
//            psrlw mm1,8             // m1 = 00b4 00b7 00b6 00b5
//
//            // - получим восемь значений для B-компоненты н-го цвета
//            packuswb mm4,mm1        // m4 = baba baba baba baba
//            paddusb mm7,mm4         // m7 - сумма с национальным цветом
//
//            // - упаковываем пиксели по два байта
//
//            movq  mm4,mm0
//
//            // red
//            punpcklbw mm0,mm5    // m0 = 44xx 33xx 22xx 11xx
//            punpckhbw mm1,mm5    // m1 = 88xx 77xx 66xx 55xx
//            psrlw mm0,12         // m0 = 0004 0003 0002 0001
//            psrlw mm1,12         // m1 = 0008 0007 0006 0005
//            psllw mm0,8          // m0 = 0400 0300 0200 0100
//            psllw mm1,8          // m1 = 0800 0700 0600 0500
//            // green
//            punpcklbw mm2,mm6    // m2 = 44xx 33xx 22xx 11xx
//            punpckhbw mm3,mm6    // m3 = 88xx 77xx 66xx 55xx
//            psrlw mm2,12         // m2 = 0004 0003 0002 0001
//            psrlw mm3,12         // m3 = 0008 0007 0006 0005
//            psllw mm2,4          // m2 = 0040 0030 0020 0010
//            psllw mm3,4          // m3 = 0080 0070 0060 0050
//            por   mm0,mm2
//            por   mm1,mm3
//            // blue
//            punpcklbw mm2,mm7    // m2 = 44xx 33xx 22xx 11xx
//            punpckhbw mm3,mm7    // m3 = 88xx 77xx 66xx 55xx
//            psrlw mm2,12         // m2 = 0004 0003 0002 0001
//            psrlw mm3,12         // m3 = 0008 0007 0006 0005
//            por   mm0,mm2
//            por   mm1,mm3
//
//            // mm0 и mm1 содержат восемь двухбайтных пикселей
//
//            movq  mm7,mm4
//            punpcklbw mm2,mm7    // m2 = 44xx 33xx 22xx 11xx
//            punpckhbw mm3,mm7    // m3 = 88xx 77xx 66xx 55xx
//            pand  mm2,mask_F000
//            pand  mm3,mask_F000
//
//            por   mm0,mm2
//            por   mm1,mm3
//a20:
//            // - выводим восемь пикселей в смежные квадраты
//            mov   esi,sqr2
//            mov   edi,sqr3
//            mov   ebx,j
//            mov   ecx,r
//            shl   ebx,cl
//            mov   edx,2
//            add   ebx,i
//            shl   edx,cl
//            shl   ebx,2
//
//            movd  eax,mm0
//            add   esi,ebx
//            movd  ecx,mm1
//            add   edi,ebx
//            mov   [esi],eax
//            mov   [edi],ecx
//
//            psrlq mm0,32
//            add   esi,edx
//            psrlq mm1,32
//            add   edi,edx
//            movd  eax,mm0
//            movd  ecx,mm1
//            mov   [esi],eax
//            mov   [edi],ecx
//         }
//         pos_rgb += 1;
//         pos_idx += 1;
//         sqr_a += 4;
//      }
//   }
//   __asm emms;
//}
////---------------------------------------------------------------------------
//void Decode2DSquare(unsigned char* sqr1, unsigned short* sqr2,
//                    unsigned char* sqr_a, unsigned int r)
//{
//   unsigned int range = 1<<r;
//   unsigned int pos_rgb = 0;
//   unsigned int pos_2 = range*range/2;
//   unsigned int pos_3 = range*range/2 + range*range/4;
//
//   unsigned int pos_a = 0;
//
//   for(int j = 0; j < range/2; j++)
//   {
//      for(int i = 0; i < range/2; i++)
//      {
//         unsigned int b1 = *(unsigned short*)&(sqr1[pos_rgb]);
//         pos_rgb += 2;
//         int br1 = (b1>>7)&0xF8;
//         int bg1 = (b1>>2)&0xF8;
//         int bb1 = (b1<<3)&0xF8;
//
//         int b2 = (signed char)sqr1[pos_2++];
//         int b3 = (signed char)sqr1[pos_3++];
//
//         int a1 = sqr_a[pos_a++];
//         int a2 = sqr_a[pos_a++];
//
//      	int cr1 = br1 + b2 + b3;
//      	int cr2 = br1 + b2 - b3;
//      	int cr3 = br1 - b2 + b3;
//      	int cr4 = br1 - b2 - b3;
//
//      	int cg1 = bg1 + b2 + b3;
//      	int cg2 = bg1 + b2 - b3;
//      	int cg3 = bg1 - b2 + b3;
//      	int cg4 = bg1 - b2 - b3;
//
//      	int cb1 = bb1 + b2 + b3;
//      	int cb2 = bb1 + b2 - b3;
//      	int cb3 = bb1 - b2 + b3;
//      	int cb4 = bb1 - b2 - b3;
//
//      	if(cr1 >= 256) cr1 = 255;
//      	if(cr1 < 0)    cr1 = 0;
//      	if(cr2 >= 256) cr2 = 255;
//      	if(cr2 < 0)    cr2 = 0;
//      	if(cr3 >= 256) cr3 = 255;
//      	if(cr3 < 0)    cr3 = 0;
//      	if(cr4 >= 256) cr4 = 255;
//      	if(cr4 < 0)    cr4 = 0;
//
//      	if(cg1 >= 256) cg1 = 255;
//      	if(cg1 < 0)    cg1 = 0;
//      	if(cg2 >= 256) cg2 = 255;
//      	if(cg2 < 0)    cg2 = 0;
//      	if(cg3 >= 256) cg3 = 255;
//      	if(cg3 < 0)    cg3 = 0;
//      	if(cg4 >= 256) cg4 = 255;
//      	if(cg4 < 0)    cg4 = 0;
//
//      	if(cb1 >= 256) cb1 = 255;
//      	if(cb1 < 0)    cb1 = 0;
//      	if(cb2 >= 256) cb2 = 255;
//      	if(cb2 < 0)    cb2 = 0;
//      	if(cb3 >= 256) cb3 = 255;
//      	if(cb3 < 0)    cb3 = 0;
//      	if(cb4 >= 256) cb4 = 255;
//      	if(cb4 < 0)    cb4 = 0;
//
//         int nr = 0;
//         int ng = 0xF0;
//         int nb = 0;
//
//         if(isNCPresent)
//         {
//            if(a1 & 0x10)
//            {
//               int a = a1&0xF0;
//               cr1 += cr1+(a*nr>>8);
//               cg1 += cg1+(a*ng>>8);
//               cb1 += cb1+(a*nb>>8);
//               if(cr1 > 255) cr1 = 255;
//               if(cg1 > 255) cg1 = 255;
//               if(cb1 > 255) cb1 = 255;
//
//               a1 |= 0xF0;
//            }
//            if(a1 & 0x1)
//            {
//               int a = (a1<<4)&0xF0;
//               cr2 += cr2+(a*nr>>8);
//               cg2 += cg2+(a*ng>>8);
//               cb2 += cb2+(a*nb>>8);
//               if(cr2 > 255) cr2 = 255;
//               if(cg2 > 255) cg2 = 255;
//               if(cb2 > 255) cb2 = 255;
//
//               a1 |= 0xF;
//            }
//            if(a2 & 0x10)
//            {
//               int a = a2&0xF0;
//               cr3 += cr3+(a*nr>>8);
//               cg3 += cg3+(a*ng>>8);
//               cb3 += cb3+(a*nb>>8);
//               if(cr3 > 255) cr3 = 255;
//               if(cg3 > 255) cg3 = 255;
//               if(cb3 > 255) cb3 = 255;
//
//               a2 |= 0xF0;
//            }
//            if(a2 & 0x1)
//            {
//               int a = (a2<<4)&0xF0;
//               cr4 += cr4+(a*nr>>8);
//               cg4 += cg4+(a*ng>>8);
//               cb4 += cb4+(a*nb>>8);
//               if(cr4 > 255) cr4 = 255;
//               if(cg4 > 255) cg4 = 255;
//               if(cb4 > 255) cb4 = 255;
//
//               a2 |= 0xF;
//            }
//         }
//
//         sqr2[(j*2)*range + i*2] = (unsigned short)((a1&0xF0)<<8)|((unsigned short)(cr1&0xF0)<<4)|((unsigned short)cg1&0xF0)|((unsigned short)(cb1&0xF0)>>4);
//         sqr2[(j*2)*range + i*2+1] = (unsigned short)((a1&0xF)<<12)|((unsigned short)(cr2&0xF0)<<4)|((unsigned short)cg2&0xF0)|((unsigned short)(cb2&0xF0)>>4);
//         sqr2[(j*2+1)*range + i*2] = (unsigned short)((a2&0xF0)<<8)|((unsigned short)(cr3&0xF0)<<4)|((unsigned short)cg3&0xF0)|((unsigned short)(cb3&0xF0)>>4);
//         sqr2[(j*2+1)*range + i*2+1] = (unsigned short)((a2&0xF)<<12)|((unsigned short)(cr4&0xF0)<<4)|((unsigned short)cg4&0xF0)|((unsigned short)(cb4&0xF0)>>4);
//      }
//   }
//}
////---------------------------------------------------------------------------
///*
//   Функция распаковывает сегмент с кадрами, сегмент в виде:
//      unsigned int outlen  // - размер разархивированного блока
//      <compressed data>    // - заархивированные данные
//   После разархивации получается набор фреймов, содержащих данные о
//   квадратах, зависимо от метода упаковки.
//*/
//bool G16UnpackSegment(unsigned char* inbuf,     // - упакованные данные
//                      unsigned int inlen,       // - размер входного буфера
//                      unsigned char* outbuf,    // - распакованный сегмент
//                      unsigned char* workbuf,   // - рабочий буфер
//                      unsigned int* fbuf,       // - массив смещений кадров
//                      unsigned int Frames,      // - количество кадров в сегменте
//                      unsigned int Flags)       // - флаги (метод упаковки)
//{
//   unsigned int outlen = *(unsigned int*)inbuf;
//   inbuf += 4;
//
//   //////////////////////////////////////////////////////////////////////////
//   // Разархивируем raw-данные из inbuf в workbuf
//   //////////////////////////////////////////////////////////////////////////
//   // unsigned __int64 counter1, counter2;
//   // QueryPerformanceCounter((LARGE_INTEGER*)&counter1);
//   //////////////////////////////////////////////////////////////////////////
//   if((Flags & G16_COMPRESS_METHOD_MASK) == G16_COMPRESSED_BY_UCL)
//   {
//   	if(!ucl_decompress(inbuf, inlen, workbuf, &outlen))
//         return false;
//   }
//   else
//   if((Flags & G16_COMPRESS_METHOD_MASK) == G16_COMPRESSED_BY_LZO)
//   {
//   	if(lzo1x_decompress_asm_fast(inbuf, inlen, workbuf, &outlen, workbuf) < 0)
//         return false;
//   }
//   else
//   if((Flags & G16_COMPRESS_METHOD_MASK) == G16_NONCOMPRESSED)
//   {
//      memcpy(workbuf, inbuf, inlen);
//      outlen = inlen;
//   }
//   else
//      return false;
//   //////////////////////////////////////////////////////////////////////////
//   // QueryPerformanceCounter((LARGE_INTEGER*)&counter2);
//   // _DecompressTicks += (unsigned __int64)(counter2-counter1);
//   //////////////////////////////////////////////////////////////////////////
//
//   unsigned allsize = 0;
// 	int workbuf_pos = 0;
//   #define GETC() workbuf[workbuf_pos++]
//   #define GETSH(a) {a = *(unsigned short*)&(workbuf[workbuf_pos]); workbuf_pos += 2;}
//
//   //////////////////////////////////////////////////////////////////////////
//   // Распакуем разархивированный сегмент в outbuf
//   //////////////////////////////////////////////////////////////////////////
//   /*
//      Структура сегмента для 3D/2D-wavelet:
//      SEGMENT
//      {
//         DWORD AlphaOffset; // - смещение к данным о альфе
//         FRAME
//         {
//            WORD Squares;   // - число квадратов в кадре
//            SQUARE
//            {
//               WORD X;      // - x-координата квадрата в кадре
//               WORD Y;      // - y-координата квадрата в кадре
//               BYTE RC;     // - размер и конфигурация квадрата
//               BYTE Data[]; // - упакованное изображение
//            } Squares[];
//         } Frames[];
//         BYTE AlphaData[];
//      }
//   */
//   if((Flags & G16_PACK_METHOD_MASK) == G16_PACKED_BY_3DWAVELET)
//   {
//      // - alpha`s data offset
//      unsigned int alpha_pos = *(unsigned int*)&(workbuf[workbuf_pos]);
//      workbuf_pos += 4;
//
//      //-----------------------------------------------------------------------
//      unsigned char* FrmData = outbuf;
//      unsigned char* AlphaData = workbuf + alpha_pos;
//      for(int f = 0; f < Frames; f += 2)
//      {
//         int squares;
//         GETSH(squares);
//         unsigned int FrmSize = *(unsigned int*)&(workbuf[workbuf_pos]);
//         workbuf_pos += 4;
//
//         unsigned int SqrDataOffset = 0;  // - смещение до начала текущего квадрата
//
//         for(int s = 0; s < squares; s++)
//         {
//            unsigned int x, y, r = 0;
//
//            GETSH(x);
//            GETSH(y);
//            r = GETC(); // - старшие 4 бита содержат степень стороны
//
//            int range = 1<<(r>>4);  // - численное значение длины стороны квадрата
//
//            unsigned int SqrSize = ((1<<((r>>4)<<1))<<1)+8;
//            unsigned char* SqrData1 = FrmData + SqrDataOffset;
//            unsigned char* SqrData2 = FrmData + FrmSize + SqrDataOffset;
//
//            // - запишем заголовки квадратов (8 bytes)
//            *(unsigned int*)SqrData1 = (r<<24)|((x&0xFFF)<<12)|(y&0xFFF);
//            *(unsigned int*)SqrData2 = *(unsigned int*)SqrData1;
//
//            Decode3DSquare(workbuf + workbuf_pos,
//               (unsigned short*)(SqrData1+8),
//               (unsigned short*)(SqrData2+8),
//               AlphaData,
//               r>>4);
//
////            workbuf_pos += (5*range*range)/4;
////            workbuf_pos += (3*range*range)/4;
//            workbuf_pos += (range*range)/2;
//            SqrDataOffset += SqrSize;
//            AlphaData += range*range;
//         }
//         fbuf[f] = FrmData-outbuf;
//         fbuf[f+1] = FrmData+FrmSize-outbuf;
//         FrmData += FrmSize*2;
//      }
//   }
//   else
//   if((Flags & G16_PACK_METHOD_MASK) == G16_PACKED_BY_2DWAVELET)
//   {
//      // - alpha`s data offset
//      unsigned int alpha_pos = *(unsigned int*)&(workbuf[workbuf_pos]);
//      workbuf_pos += 4;
//
//      //-----------------------------------------------------------------------
//      unsigned char* FrmData = outbuf; //- выходной рабочий буфер
//      unsigned char* AlphaData = workbuf + alpha_pos;
//      for(int f = 0; f < Frames; f++)
//      {
//         int squares;
//         GETSH(squares);
//
//         unsigned int SqrDataOffset = 0;  // - смещение до начала текущего квадрата
//
//         fbuf[f] = FrmData-outbuf;
//         for(int s = 0; s < squares; s++)
// 	      {
//            unsigned int x, y, r = 0;
//
//            GETSH(x);
//            GETSH(y);
//            r = GETC();
//
//            int range = 1<<(r>>4);
//
//            unsigned int SqrSize = ((1<<((r>>4)<<1))<<1)+8;
//            unsigned char* SqrData = FrmData + SqrDataOffset;
//            *(unsigned int*)SqrData = (r<<24)|((x&0xFFF)<<12)|(y&0xFFF);
//
//            Decode2DSquare(workbuf + workbuf_pos,
//               (unsigned short*)(SqrData+8),
//               AlphaData,
//               r>>4);
//
//            workbuf_pos += range*range;
//            SqrDataOffset += SqrSize;
//            AlphaData += range*range/2;
//         }
//         FrmData += SqrDataOffset;
//      }
//   }
//   /*
//      Структура сегмента для Store:
//      SEGMENT
//      {
//         FRAME
//         {
//            DWORD FrameSize; // - размер фрейма в байтах
//            SQUARE
//            {
//               // - header 8 bytes
//               DWORD Pow	   :4; 	   //  side = 2^Pow
//   	         DWORD Config	:4; 	   //  configuration of the polygon
//	            DWORD X			:12;	   //  x-position of the chunk in sprite
//            	DWORD Y  		:12;	   //  y-position of the chunk in sprite
//            	DWORD	reserved;		   //  reserved for additional bind pointer
//               struct {
//                  WORD a :4;
//                  WORD r :4;
//                  WORD g :4;
//                  WORD b :4;
//               } Data[side*side*2];    // - ARGB изображение
//            } Squares[];
//         } Frames[];
//      }
//   */
//   else
//   if((Flags & G16_PACK_METHOD_MASK) == G16_IDXSTORE)
//   {
//      /*
//         Структура сегмента для IdxStore:
//         SEGMENT {
//            DWORD ColorOffset;            // - относительное смещение в байтах до данных цвета
//            DWORD AlphaOffset;            // - относительное смещение в байтах до альфы
//
//            FRAME {
//               WORD SquaresNumber;        // - количество квадратов в кадре
//               SQUARE {
//                  // - header 4 bytes
//                  DWORD Pow	   :4; 	   //  side = 2^Pow
//      	         DWORD Config	:4; 	   //  configuration of the polygon
//	               DWORD X			:12;	   //  x-position of the chunk in sprite
//                  DWORD Y  		:12;	   //  y-position of the chunk in sprite
//               } Squares[SquaresNumber];
//            } Frames[FramesNumber];
//
//            ColorData[];                  // - массив цветовой информации для всего сегмента
//            AlphaData[];                  // - массив информации о альфе для всего сегмента
//         }
//      */
//      unsigned ch = 0, c_pos = 0, a_pos = 0, src_pos = 0, dst_pos = 0;
//      #define GETINT(ch) ch = *((unsigned*)&(workbuf[src_pos])); src_pos += 4;
//		#undef  GETSH
//      #define GETSH(ch)  ch = *((unsigned short*)&(workbuf[src_pos])); src_pos += 2;
//      #define PUTINT(ch) *((unsigned*)&(outbuf[dst_pos])) = ch; dst_pos += 4;
//      #define PUTSH(ch)  *((unsigned short*)&(outbuf[dst_pos])) = ch; dst_pos += 2;
//      #define PUTC(ch)   *((unsigned char*)&(outbuf[dst_pos])) = ch; dst_pos += 1;
//
//      GETINT(ch);
//      unsigned char* ColorOffset = workbuf + src_pos + ch;
//      GETINT(ch);
//      unsigned char* AlphaOffset = workbuf + src_pos + ch;
//
//      int _nr = (unsigned char)nr;
//      int _ng = (unsigned char)ng;
//      int _nb = (unsigned char)nb;
//
//      for(unsigned f = 0; f < Frames; f++)
//      {
//         WORD SquaresNumber;
//         GETSH(SquaresNumber);
//
//         fbuf[f] = dst_pos;   // - указатель на текущий кадр
//         for(unsigned s = 0; s < SquaresNumber; s++)
//         {
//            GETINT(ch);
//            PUTINT(ch);       // - заголовок квадрата
//            PUTINT(0);        // - reserved
//            int side = 1<<(ch>>28);
//
//            #define MAKECOLOR(_a) {\
//               unsigned color = *(unsigned*)&G16PalRGB[(*ColorOffset++)<<2];\
//               b = (color>>16)&0xFF;\
//               g = (color>>8)&0xFF;\
//               r = color&0xFF;\
//               if(_a & 0x10)\
//               {\
//                  int a = _a&0xE0;\
//                  r += (a*_nr)/0xE0;\
//                  g += (a*_ng)/0xE0;\
//                  b += (a*_nb)/0xE0;\
//                  if(r > 255) r = 255;\
//                  if(g > 255) g = 255;\
//                  if(b > 255) b = 255;\
//                  _a = 0xF0;\
//               }\
//               else\
//               if(_a) _a |= 0x10;\
//               ch = ((_a&0xF0)<<8)|((r&0xF0)<<4)|(g&0xF0)|(b>>4);\
//            }
//
//            for(int j = 0; j < side; j++)
//            {
//               for(int i = 0; i < side; i+=2)
//               {
//                  int aa = *AlphaOffset++;
//                  int a1 = aa&0xF0;
//                  int a2 = (aa<<4)&0xF0;
//                  int r, g, b;
//                  if(a1)
//                  {
//                     MAKECOLOR(a1);
//                  }
//                  else ch = 0;
//                  PUTSH(ch);
//
//                  if(a2)
//                  {
//                     MAKECOLOR(a2);
//                  }
//                  else ch = 0;
//                  PUTSH(ch);
//               }
//            }
//         }
//      }
//   }
//   else
//   if((Flags & G16_PACK_METHOD_MASK) == G16_444STORE)
//   {
//      /*
//         Структура сегмента для 444Store:
//         SEGMENT {
//            DWORD ColorOffset;            // - относительное смещение в байтах до данных цвета
//            DWORD AlphaOffset;            // - относительное смещение в байтах до альфы
//
//            FRAME {
//               WORD SquaresNumber;        // - количество квадратов в кадре
//               SQUARE {
//                  // - header 4 bytes
//                  DWORD Pow	   :4; 	   //  side = 2^Pow
//      	         DWORD Config	:4; 	   //  configuration of the polygon
//	               DWORD X			:12;	   //  x-position of the chunk in sprite
//                  DWORD Y  		:12;	   //  y-position of the chunk in sprite
//               } Squares[SquaresNumber];
//            } Frames[FramesNumber];
//
//            ColorData[];                  // - массив цветовой информации для всего сегмента
//            AlphaData[];                  // - массив информации о альфе для всего сегмента
//         }
//      */
//      unsigned ch = 0, c_pos = 0, a_pos = 0, src_pos = 0, dst_pos = 0;
//      #define GETINT(ch) ch = *((unsigned*)&(workbuf[src_pos])); src_pos += 4;
//      #define GETSH(ch)  ch = *((unsigned short*)&(workbuf[src_pos])); src_pos += 2;
//      #define PUTINT(ch) *((unsigned*)&(outbuf[dst_pos])) = ch; dst_pos += 4;
//      #define PUTSH(ch)  *((unsigned short*)&(outbuf[dst_pos])) = ch; dst_pos += 2;
//      #define PUTC(ch)   *((unsigned char*)&(outbuf[dst_pos])) = ch; dst_pos += 1;
//
//      GETINT(ch);
//      unsigned char* ColorOffset = workbuf + src_pos + ch;
//      GETINT(ch);
//      unsigned char* AlphaOffset = workbuf + src_pos + ch;
//
//      int _nr = (unsigned char)nr;
//      int _ng = (unsigned char)ng;
//      int _nb = (unsigned char)nb;
//
//      for(unsigned f = 0; f < Frames; f++)
//      {
//         WORD SquaresNumber;
//         GETSH(SquaresNumber);
//
//         fbuf[f] = dst_pos;   // - указатель на текущий кадр
//         for(unsigned s = 0; s < SquaresNumber; s++)
//         {
//            GETINT(ch);
//            PUTINT(ch);       // - заголовок квадрата
//            PUTINT(0);        // - reserved
//            int side = 1<<(ch>>28);
//
//            #undef MAKECOLOR
//            #define MAKECOLOR(_a) {\
//               unsigned color = *((unsigned short*)ColorOffset);\
//               ColorOffset += 2;\
//               if(_a & 0x10)\
//               {\
//                  r = (color<<4)&0xF0;\
//                  g = (color)&0xF0;\
//                  b = (color>>4)&0xF0;\
//                  int a = _a&0xE0;\
//                  r += (a*_nr)/0xE0;\
//                  g += (a*_ng)/0xE0;\
//                  b += (a*_nb)/0xE0;\
//                  if(r > 0xF0) r = 0xF0;\
//                  if(g > 0xF0) g = 0xF0;\
//                  if(b > 0xF0) b = 0xF0;\
//                  _a = 0xF0;\
//                  ch = ((r&0xF0)<<4)|(g&0xF0)|(b>>4);\
//               }\
//               else\
//               {\
//                  if(_a) _a |= 0x10;\
//                  ch = color;\
//               }\
//               ch |= ((_a&0xF0)<<8);\
//            }
//
//            for(int j = 0; j < side; j++)
//            {
//               for(int i = 0; i < side; i+=2)
//               {
//                  int aa = *AlphaOffset++;
//                  int a1 = aa&0xF0;
//                  int a2 = (aa<<4)&0xF0;
//                  int r, g, b;
//                  if(a1)
//                  {
//                     MAKECOLOR(a1);
//                  }
//                  else ch = 0;
//                  PUTSH(ch);
//
//                  if(a2)
//                  {
//                     MAKECOLOR(a2);
//                  }
//                  else ch = 0;
//                  PUTSH(ch);
//               }
//            }
//         }
//      }
//   }
//   else
//      return false;
//   // -----------------------------------------------------------------------
//   return true;
//}
