//---------------------------------------------------------------------------
#include <assert.h>
#include "FBaseStream.h"
#include "FCompressor.h"

using namespace fal;

//------------------------------------------------------------------------------
#pragma intrinsic(memset,memcpy)
//------------------------------------------------------------------------------
// Realtime unpacker
//------------------------------------------------------------------------------
static class FGA_G18Unpacker {
   //---------------------------------------------------------------------------
   FCompressor* Compressor;
   unsigned char *FrameAI;
   unsigned Size;
   //--------------------------- range coder -----------------------------------
   enum {TOP = (1<<24), BOT = (1<<16)};
    unsigned lowI, codeI, rangeI;
    unsigned lowA, codeA, rangeA;
   //---------------------------------------------------------------------------
   enum {
      CharsNumberA = 16,
      CharsNumberI = 64,
      ContextsNumberA = CharsNumberA*CharsNumberA*CharsNumberA*CharsNumberA,
      ContextsNumberI = CharsNumberI*CharsNumberI,
      ESC_A = 0x10>>4,
      ESC_I = CharsNumberI-1
   };
   //---------------------------------------------------------------------------
   // - символы текущего контекста
   //---------------------------------------------------------------------------
   unsigned cnt_ch1A, cnt_ch0A, cnt_chLA, cnt_chUA;
   unsigned cnt_ch1I, cnt_ch0I;
   //---------------------------------------------------------------------------
   // - все возможные частоты и суммы, для нулевой модели отведено последнее место
   //---------------------------------------------------------------------------
   static unsigned short FreqsA[ContextsNumberA+1][CharsNumberA];
   static unsigned short FreqsI[ContextsNumberI+1][CharsNumberI];
   static unsigned SummFreqsA[ContextsNumberA+1];
   static unsigned SummFreqsI[ContextsNumberI+1];
   //---------------------------------------------------------------------------
    // - текущие частота и сумма
   //---------------------------------------------------------------------------
   unsigned short *FreqA;
   unsigned short *FreqI;
   unsigned *SummFreqA;
   unsigned *SummFreqI;

   #define GETC(In) ((In)->_getc())
   //---------------------------------------------------------------------------
   void Realloc(unsigned _Size)
   {
      bool isRealloc = false;
      if(Size < _Size)
      {
         if(FrameAI) delete[] FrameAI;
         isRealloc = true;
      }
      if(!FrameAI) isRealloc = true;

      Size = _Size;
      if(isRealloc)
         FrameAI = new unsigned char[Size];

        memset(FrameAI, 0, Size);
   }
   //---------------------------------------------------------------------------
    __forceinline unsigned GetContextIndexA()
    {
      return (cnt_ch1A<<12)|(cnt_ch0A<<8)|(cnt_chLA<<4)|cnt_chUA;
    }
   //---------------------------------------------------------------------------
   __forceinline unsigned GetContextIndexI()
    {
      return (cnt_ch1I<<6)|cnt_ch0I;
    }
   //---------------------------------------------------------------------------
   __forceinline unsigned short* GetContextFreqA(unsigned idx)
    {
      return FreqsA[idx];
    }
   //---------------------------------------------------------------------------
   __forceinline unsigned short* GetContextFreqI(unsigned idx)
    {
      return FreqsI[idx];
    }
   //---------------------------------------------------------------------------
   __forceinline unsigned* GetContextSummFreqA(unsigned idx)
    {
      return &SummFreqsA[idx];
    }
   //---------------------------------------------------------------------------
   __forceinline unsigned* GetContextSummFreqI(unsigned idx)
    {
      return &SummFreqsI[idx];
    }
   //---------------------------------------------------------------------------
   __forceinline void SetContextA(int ch1, int ch0, int chL, int chU)
   {
      cnt_ch1A = (ch1)>>4;
      cnt_ch0A = (ch0)>>4;
      cnt_chLA = (chL)>>4;
      cnt_chUA = (chU)>>4;
      unsigned idx = GetContextIndexA();
      FreqA = GetContextFreqA(idx);
      SummFreqA = GetContextSummFreqA(idx);
   }
   //---------------------------------------------------------------------------
   __forceinline void SetContextI(int ch1, int ch0)
   {
      cnt_ch1I = ch1&0x3F;
      cnt_ch0I = ch0&0x3F;
      unsigned idx = GetContextIndexI();
      FreqI = GetContextFreqI(idx);
      SummFreqI = GetContextSummFreqI(idx);
   }
   //---------------------------------------------------------------------------
   __forceinline void RangeDecodeSymbolA(unsigned int& sym, FInStream* In)
   {
       unsigned HiCount;
      unsigned count = (codeA - lowA)/(rangeA /= (*SummFreqA));
        if(count >= (*SummFreqA)) throw("Input data corrupt");
       for(HiCount = sym = 0;; sym++)
       {
           HiCount += FreqA[sym];
           if(HiCount > count) break;
       }
      unsigned freq = FreqA[sym];
       unsigned cumFreq = HiCount - freq;
       assert(cumFreq + freq <= (*SummFreqA) && freq && (*SummFreqA) <= BOT);
       lowA += cumFreq*rangeA;
       rangeA *= freq;
       while((lowA^(lowA + rangeA)) < TOP || rangeA < BOT && ((rangeA = -(int)lowA & BOT-1), 1))
           codeA = codeA << 8 | GETC(In), rangeA <<= 8, lowA <<= 8;
   }
   //---------------------------------------------------------------------------
   __forceinline void UpdateModelA(unsigned int sym)
   {
      FreqA[sym] += 8;
        (*SummFreqA) += 8;
       while(*SummFreqA > BOT)
      {
           for(unsigned i = (*SummFreqA) = 0; i < CharsNumberA; i++)
               (*SummFreqA) += (FreqA[i] -= (FreqA[i] >> 1));
      }
   }
   //---------------------------------------------------------------------------
   __forceinline void DecodeSymbolA(unsigned int& sym, FInStream* In)
   {
      RangeDecodeSymbolA(sym, In);
      UpdateModelA(sym);
      if(sym == ESC_A)
      {
         unsigned short* _FreqA = FreqA;
         unsigned* _SummFreqA = SummFreqA;

            FreqA = GetContextFreqA(ContextsNumberA);
           SummFreqA = GetContextSummFreqA(ContextsNumberA);

         RangeDecodeSymbolA(sym, In);
         UpdateModelA(sym);

         FreqA = _FreqA;
         SummFreqA = _SummFreqA;

         UpdateModelA(sym);
      }
      sym <<= 4;
   }
   //---------------------------------------------------------------------------
   __forceinline void RangeDecodeSymbolI(unsigned int& sym, FInStream* In)
   {
       unsigned HiCount;
      unsigned count = (codeI - lowI)/(rangeI /= (*SummFreqI));
        if(count >= (*SummFreqI)) throw("Input data corrupt");
       for(HiCount = sym = 0;; sym++)
       {
           HiCount += FreqI[sym];
           if(HiCount > count) break;
       }
      unsigned freq = FreqI[sym];
       unsigned cumFreq = HiCount - freq;
       assert(cumFreq + freq <= (*SummFreqI) && freq && (*SummFreqI) <= BOT);
       lowI += cumFreq*rangeI;
       rangeI *= freq;
       while((lowI^(lowI + rangeI)) < TOP || rangeI < BOT && ((rangeI = -(int)lowI & BOT-1), 1))
           codeI = codeI << 8 | GETC(In), rangeI <<= 8, lowI <<= 8;
   }
   //---------------------------------------------------------------------------
   __forceinline void UpdateModelI(unsigned int sym)
   {
      FreqI[sym] += 8;
       (*SummFreqI) += 8;
       while(*SummFreqI > BOT)
      {
           for(unsigned i = (*SummFreqI) = 0; i < CharsNumberI; i++)
               (*SummFreqI) += (FreqI[i] -= (FreqI[i] >> 1));
      }
   }
   //---------------------------------------------------------------------------
   __forceinline void DecodeSymbolI(unsigned int& sym, FInStream* In)
   {
      RangeDecodeSymbolI(sym, In);
      UpdateModelI(sym);
      if(sym == ESC_I)
      {
         unsigned short* _FreqI = FreqI;
         unsigned* _SummFreqI = SummFreqI;

            FreqI = GetContextFreqI(ContextsNumberI);
           SummFreqI = GetContextSummFreqI(ContextsNumberI);

         RangeDecodeSymbolI(sym, In);
         UpdateModelI(sym);

         FreqI = _FreqI;
         SummFreqI = _SummFreqI;

         UpdateModelI(sym);
      }
   }
   //---------------------------------------------------------------------------
public:
   bool ProcessMemory(unsigned char** pOutData, unsigned* pOutLen, unsigned char* InData, unsigned InLen);
   unsigned ProcessFile(char* InFileName, char* OutFileName);
   //---------------------------------------------------------------------------
   FGA_G18Unpacker()
   {
      FrameAI = NULL;
      Size = 0;
      Compressor = new FCompressor;
      Compressor->Initialize();
   }
   //---------------------------------------------------------------------------
   ~FGA_G18Unpacker()
   {
      delete[] FrameAI;
      delete Compressor;
   }
   //---------------------------------------------------------------------------
} FGA_G18UnpackerInstance;
//------------------------------------------------------------------------------
unsigned short FGA_G18Unpacker::FreqsA[FGA_G18Unpacker::ContextsNumberA+1][FGA_G18Unpacker::CharsNumberA];
unsigned short FGA_G18Unpacker::FreqsI[FGA_G18Unpacker::ContextsNumberI+1][FGA_G18Unpacker::CharsNumberI];
unsigned FGA_G18Unpacker::SummFreqsA[FGA_G18Unpacker::ContextsNumberA+1];
unsigned FGA_G18Unpacker::SummFreqsI[FGA_G18Unpacker::ContextsNumberI+1];
//------------------------------------------------------------------------------
unsigned FGA_G18Unpacker::ProcessFile(char* InFileName, char* OutFileName)
{
   FInStream InStream;
   FOutStream OutStream;
   InStream.reload(InFileName);
    if(!ProcessMemory(&OutStream.Data, &OutStream.Size, InStream.Data, InStream.Size)) return 0;
    OutStream.AllocatedBytes = OutStream.Size;
    OutStream.save(OutFileName);
    return OutStream.Size;
}
//------------------------------------------------------------------------------
bool FGA_G18Unpacker::ProcessMemory(unsigned char** pOutData, unsigned* pOutLen, unsigned char* InData, unsigned InLen)
{
    FInStream InStream;
    InStream.attach(InData, InLen);
   if(InStream.getc() != 0xFF)
   {
      printf("Not a G18 format!");
      return false;
   }
   //---------------------------------------------------------------------------
    lowI = 0, codeI = 0, rangeI = (unsigned)-1;
    lowA = 0, codeA = 0, rangeA = (unsigned)-1;
   FreqA = FreqI = NULL;
    SummFreqA = SummFreqI = NULL;

    // - символы текущего контекста
   cnt_ch1A = 0, cnt_ch0A = 0, cnt_chLA = 0, cnt_chUA = 0;
   cnt_ch1I = 0, cnt_ch0I = 0;

   //------------------------ Initialize models --------------------------------
   {
        memset(FreqsA, 0, sizeof(FreqsA));
      for(unsigned k = 0; k < ContextsNumberA; k++)
      {
            FreqsA[k][ESC_A] = 1;
           SummFreqsA[k] = 1;
      }
        memset(FreqsI, 0, sizeof(FreqsI));
      for(unsigned k = 0; k < ContextsNumberI; k++)
      {
            FreqsI[k][ESC_I] = 1;
           SummFreqsI[k] = 1;
      }
   }
   //------------------------ InitializeModels 0 -------------------------------
   {
      for(unsigned i = 0; i < CharsNumberA; i++)
            FreqsA[ContextsNumberA][i] = 1;
      
        for(unsigned i = 0; i < CharsNumberI; i++)
            FreqsI[ContextsNumberI][i] = 1;

        SummFreqsA[ContextsNumberA] = CharsNumberA;
        SummFreqsI[ContextsNumberI] = CharsNumberI;
   }
   //---------------------------------------------------------------------------
   // - распаковка Data
   //---------------------------------------------------------------------------
    FInStream InStreamData;
   unsigned InDataLen = InStream.getint(InStream.Pos + 1)+9;
   if(!Compressor->Initialize())
   {
      delete Compressor;
      throw("Cannot initialize the decompressor");
   }
   Compressor->DecompressBlock(&InStreamData.Data, &InStreamData.Size, InStream.Data + InStream.Pos);
   InStreamData.AllocatedBytes = InStreamData.Size;
   InStream.Pos += InDataLen;

   unsigned FramesNumber = InStreamData._getshort();
   unsigned X1           = InStreamData._getshort();
   unsigned Y1           = InStreamData._getshort();
   unsigned BWidth       = InStreamData._getshort();
   unsigned BHeight      = InStreamData._getshort();
   unsigned Width        = InStreamData._getshort();
   unsigned Height       = InStreamData._getshort();
   unsigned InfoLen      = InStreamData._getint(), InfoPos = InStreamData.Pos;
   unsigned G15FileSize  = InStreamData._getint();
   unsigned Directions   = InStreamData._getc();
   InStreamData.Pos = InfoPos + InfoLen;
   unsigned ColorsNumber = InStreamData._getc()+1;

   static unsigned char palRGB[1024];
    InStreamData.getblock(palRGB, ColorsNumber*4);

   FInStream InStreamFramesInfos, InStreamSquares;
   InStreamFramesInfos.attach(InStreamData.Data + InStreamData.Pos, FramesNumber*2);
   InStreamSquares.attach(InStreamData.Data + InStreamData.Pos + FramesNumber*2,
                          InStreamData.Size - InStreamData.Pos - FramesNumber*2);

   FOutStream OutStream, OutStreamSquares;
   OutStream.reload(G15FileSize);

   // - расстояние в GU15 до начала квадратов
   unsigned ToSqrLen = sizeof(DWORD) + //    Magic
                       sizeof(DWORD) + //    FileSize
                         sizeof(WORD)  +    // FramesNumber
                          sizeof(DWORD) +    // InfoLen
                       sizeof(BYTE)  + // Info: Directions
                       sizeof(WORD)  + //    Width
                       sizeof(WORD)  + //    Height
                      (sizeof(WORD)  + // SquaresNumber
                         sizeof(WORD)  + // Reserved
                         sizeof(DWORD))* // Offset
                       FramesNumber;

   OutStreamSquares.attach(OutStream.Data + ToSqrLen, G15FileSize - ToSqrLen);

   OutStream._putint('51UG');
   OutStream._putint(G15FileSize);
   OutStream._putshort(FramesNumber);
   OutStream._putshort(BWidth);
   OutStream._putshort(BHeight);
   OutStream._putint(1);          // - InfoLen
   OutStream._putc(Directions);   // - Info

   FInStream InStreamI, InStreamA;
   unsigned OutLenI = InStream._getint();
   unsigned InLenI = InStream._getint();
   InStreamI.attach(InStream.Data + InStream.Pos, InLenI);
   InStream.Pos += InLenI;
   unsigned OutLenA = InStream._getint();
   unsigned InLenA = InStream._getint();
   InStreamA.attach(InStream.Data + InStream.Pos, InLenA);
   InStream.Pos += InLenA;

   //---------------------------------------------------------------------------
   for(int i = 0; i < 4; i++)
      codeA = (codeA<<8) | InStreamA._getc();
   for(int i = 0; i < 4; i++)
      codeI = (codeI<<8) | InStreamI._getc();
   //---------------------------------------------------------------------------
    int _width = ((Width+1)+15)&~15;
    int _height = ((Height+1)+15)&~15;
   Realloc(_width*_height*2*2);
   //---------------------------------------------------------------------------
    for(unsigned f = 0; f < FramesNumber; f++)
   {
      unsigned SquaresNumber = InStreamFramesInfos._getshort();

      OutStream._putint(SquaresNumber);
      OutStream._putint(OutStreamSquares.Pos);

      for(unsigned s = 0; s < SquaresNumber; s++)
      {
         unsigned RCXY = InStreamSquares._getint();
         OutStreamSquares._putint(RCXY);
         OutStreamSquares._putint(0);

         unsigned r = 1<<(RCXY>>28);
         unsigned x = ((RCXY>>12)&0xFFF) - X1;
         unsigned y = (RCXY&0xFFF) - Y1;

         int offset = ((y+1)*_width + (x+1))<<2;
            unsigned short* pRleCnt = NULL;
         for(unsigned j = 0; j < r; j++, offset -= ((r-_width)<<2))
         {
            for(unsigned i = 0; i < r; i++, offset += 4)
            {
               unsigned Idx = 0, A = 0;
                    union {
                        unsigned int* pF;
                        unsigned char* F;
                    };
                    F = FrameAI + offset - 2;
                    // A1I1A0I0
                    SetContextA(F[0], F[2], F[-2], F[2-(_width<<2)]);
               DecodeSymbolA(A, &InStreamA);

               if(A)
               {
                  SetContextI(F[1], F[3]);
                  DecodeSymbolI(Idx, &InStreamI);
               }

                    pF[0] = (pF[0]>>16)|(Idx<<24)|(A<<16);

               unsigned R = palRGB[(Idx<<2)+0]&0xF0;
               unsigned G = palRGB[(Idx<<2)+1]&0xF0;
               unsigned B = palRGB[(Idx<<2)+2]&0xF0;

                    if(A == 0)
                    {
                        if(pRleCnt && (pRleCnt[0]&0xFFF) < 0x7FF)
                            pRleCnt[0]++;
                        else
                        {
                          OutStreamSquares._putshort(0x1000);
                            pRleCnt = (unsigned short*)(OutStreamSquares.Data + OutStreamSquares.Pos-2);
                        }
                    }
                    else
                    {
                        unsigned ch = (A<<8)|(R<<4)|G|(B>>4);
                        OutStreamSquares._putshort(ch);
                        pRleCnt = NULL;
                    }
            }
         }
      }
   }
    OutStream.Size = (unsigned)((OutStreamSquares.Data + OutStreamSquares.Pos) - OutStream.Data);

    *pOutData = OutStream.Data;
    *pOutLen = OutStream.Size;
    OutStream.drop();

   return true;
}
//------------------------------------------------------------------------------
