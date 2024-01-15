//---------------------------------------------------------------------------
// Fal`s Graphics Array
// (c) Frolov Andrey, 2001-2002
// Real-time decompress routines
//---------------------------------------------------------------------------
#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include "FG16Common.h"
#include "FCompressor.h"

#pragma warning ( disable : 4005 )

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
                  DWORD Pow       :4;        //  side = 2^Pow
                   DWORD Config    :4;        //  configuration of the polygon
                   DWORD X            :12;       //  x-position of the chunk in sprite
                  DWORD Y          :12;       //  y-position of the chunk in sprite
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
                  DWORD Pow       :4;        //  side = 2^Pow
                   DWORD Config    :4;        //  configuration of the polygon
                   DWORD X            :12;       //  x-position of the chunk in sprite
                  DWORD Y          :12;       //  y-position of the chunk in sprite
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

//---------------------------------------------------------------------------
int G2DUnpackTexture(unsigned char* InData, unsigned short* OutData, int W, int H, int Flags)
{
   int CompressMode = Flags&0xFF;
   int PackMode = Flags>>8;

   // - декомпрессированная текстура (индекс или rgb)
   unsigned char* TextData = NULL;
   unsigned int TextLen = W*H*2;
   bool isAttached = false;

   // - декомпрессия текстуры
   if(CompressMode != FCOMPRESSOR_TYPE_STORE)
   {
      static FCompressor Compressor;
      Compressor.DecompressBlock(&TextData, &TextLen, InData);
   }
   else
   {
      TextData = InData;
      isAttached = true;
   }

   int ColorsNumber = -1;

   // - распаковка текстуры
   if(PackMode == 0)
   {
      // - ARGB mode
      memcpy(OutData, TextData, W*H*2);
   }
   else
   {
      // - indexed mode
      int TextPos = 0;

      // - грузим палитру
      static unsigned char PalRGB[1024];
      ColorsNumber = (int)TextData[TextPos++]+1;
      for(int i = 0; i < ColorsNumber; i++)
      {
         PalRGB[i*4+0] = TextData[TextPos++];
         PalRGB[i*4+1] = TextData[TextPos++];
         PalRGB[i*4+2] = TextData[TextPos++];
         PalRGB[i*4+3] = 0;
      }

      unsigned char* ColorData = TextData + TextPos;
      int PixelsPos = 0, OutPos = 0;

      if(PackMode == 1)
      {
         // - грузим пикселы, альфа отдельно
         unsigned char* AlphaData = ColorData + W*H;
         for(int PixelsPos = 0; PixelsPos < W*H/2; PixelsPos++)
         {
            int Idx1 = (int)ColorData[(PixelsPos<<1)+0]<<2;
            int Idx2 = (int)ColorData[(PixelsPos<<1)+1]<<2;

            int aa = AlphaData[PixelsPos];
            int a1 = aa&0xF0;
            int a2 = (aa<<4)&0xF0;

            int r1 = PalRGB[Idx1+0];
            int g1 = PalRGB[Idx1+1];
            int b1 = PalRGB[Idx1+2];

            int r2 = PalRGB[Idx2+0];
            int g2 = PalRGB[Idx2+1];
            int b2 = PalRGB[Idx2+2];

            OutData[OutPos++] = ((a1<<8)&0xF000)|((r1<<4)&0xF00)|(g1&0xF0)|((b1>>4)&0xF);
            OutData[OutPos++] = ((a2<<8)&0xF000)|((r2<<4)&0xF00)|(g2&0xF0)|((b2>>4)&0xF);
         }
      }
      else
      if(PackMode == 2)
      {
         // - грузим пикселы, альфа упакована вместе с индексами
         int PixelsPos = 0, OutPixels = 0, NLen = 0;
         while(OutPixels < W*H)
         {
            int Idx = (int)ColorData[PixelsPos++];
            int a = (Idx<<5)&0xFF;

            Idx >>= 3;
            if(a == 0 && Idx != 0)
            {
               NLen = Idx;
               continue;
            }
            Idx <<= 2;

            int r = PalRGB[Idx+0];
            int g = PalRGB[Idx+1];
            int b = PalRGB[Idx+2];

            if(NLen)
            {
               a |= 0x10;
               NLen--;
            }

            OutData[OutPos++] = ((a<<8)&0xF000)|((r<<4)&0xF00)|(g&0xF0)|((b>>4)&0xF);
            OutPixels++;
         }
      }
   }

   if(!isAttached)
      delete[] TextData;

   return ColorsNumber;
}
//---------------------------------------------------------------------------
