#ifndef FStreamsH
#define FStreamsH

#include <windows.h>
#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>

namespace fal{

//#define FBASESTREAMSHOWINFO
#define FOUTSTREAM_DELTA 4096

// - таблица число/кол-во бит //    0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
static const int BitsPerVal[] = {0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 
                                        //    16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32
                                            5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,    6};

#define GetValuedBits(_ch, bits) {\
    __asm { xor        eax,eax  };\
    __asm { bsr        eax,_ch  };\
    __asm { setnz    cl            };\
    __asm { add        al,cl        };\
    __asm { mov        bits,eax };\
}

class FBaseStream {
public:
    union {
        char* cData;
        unsigned char* Data;
        int* iData;
        unsigned int* uiData;
        short* sData;
        unsigned short* usData;
    };
   unsigned S_Pos, S_Size, S_BitMask;
   unsigned Pos, BitMask, Size, AllocatedBytes;
   bool isAttached;

   void SaveState(void)
   {
      S_Pos = Pos;
      S_Size = Size;
      S_BitMask = BitMask;
   }

   void RestoreState(void)
   {
      Pos = S_Pos;
      Size = S_Size;
      BitMask = S_BitMask;
   }

   void rewind(void)
   {
      Pos = 0;
      BitMask = 0x80;
   }

   bool reload(unsigned len)
   {
        len += 16;
      if(Data == NULL)
      {
         Data = new unsigned char[len];
            #ifdef FBASESTREAMSHOWINFO
                printf("FStreams: %u bytes allocated\n", len);
            #endif
         AllocatedBytes = len;
      }
      else
      if(AllocatedBytes < len)
      {
         delete[] Data;
         Data = new unsigned char[len];
            #ifdef FBASESTREAMSHOWINFO
                printf("FStreams: %u bytes allocated\n", len);
            #endif
         AllocatedBytes = len;
      }
      Size = 0;

      rewind();
      isAttached = false;
      return true;
   }

   bool reload(char* FileName)
   {
      if(access(FileName, 0) != 0) return false;
       FILE* in = fopen(FileName, "rb");
       if(in == NULL) return false;

      // - читаем входные данные
      fseek(in, 0, SEEK_END);
      unsigned _Size = ftell(in);
      reload(_Size);
      ::rewind(in);
      fread(Data, _Size, 1, in);
      fclose(in);
      Size = _Size;

      rewind();
      isAttached = false;
      return true;
   }

   bool attach(unsigned char* InData, unsigned InLen)
   {
      release();
      Data = InData;
      AllocatedBytes = InLen;
      Size = InLen;
      isAttached = true;
      return true;
   }

   bool attach(FBaseStream* s)
   {
      release();
      Data = s->Data;
      AllocatedBytes = s->AllocatedBytes;
      Size = s->Size;
      isAttached = true;
      return true;
   }

   // - выгрузить свой поток в другой
   bool remount(FBaseStream* s)
   {
      s->release();
      memcpy(s, this, sizeof(*this));
      drop();
      s->Pos = 0;
      return true;
   }

   // - сброс без освобождения памяти
   void drop(void)
   {
      memset(this, 0, sizeof(*this));
      BitMask = 0x80;
   }

   bool save(char* FileName)
   {
       FILE* out = fopen(FileName, "w+b");
       if(out == NULL) return false;

      // - сохраняем данные
      fwrite(Data, Size, 1, out);
      fclose(out);

      return true;
   }

   void release(void)
   {
      if(!isAttached && Data) delete[] Data;
      drop();
   }

   void Expand(unsigned len = 0)
   {
      if(len == 0)
         len = AllocatedBytes + FOUTSTREAM_DELTA;
      else
         len += AllocatedBytes;

        len += (AllocatedBytes>>1);

      unsigned char* tmp = new unsigned char[len];
        #ifdef FBASESTREAMSHOWINFO
            printf("FStreams: %u bytes allocated\n", len);
        #endif
      memcpy(tmp, Data, AllocatedBytes);
      delete[] Data;
      Data = tmp;
      AllocatedBytes = len;
   }

   FBaseStream()
   {
      drop();
   }

   virtual ~FBaseStream()
   {
      release();
   }
};

#ifdef putc
#undef putc
#endif

#ifdef getc
#undef getc
#endif

class FOutStream : virtual public FBaseStream {
public:
   // - Bit
   void putb(bool bit)
   {
//        printf("%u", bit);
      if(Pos >= AllocatedBytes) Expand();

      if(bit) Data[Pos] |= BitMask;
      else    Data[Pos] &= ~BitMask;

      BitMask >>= 1;
      if(!BitMask)
      {
         Pos++;
         BitMask = 0x80;
      }
      if(Pos > Size) Size = Pos;
   }

   void flush(void)
   {
      while(BitMask != 0x80) putb(0);
   }

   // - Char
   void pushc(unsigned ch)
   {
        assert(ch <= 0xFF);
      if(Pos >= AllocatedBytes) Expand();
      memmove(&Data[1], Data, Size);
      Data[0] = ch;
      Pos++;
      if(Pos > Size) Size = Pos;
   }

    // - запись в конец потока без расширения и проверок!
   inline void _putc(unsigned ch)
   {
        assert(ch <= 0xFF);
      Data[Pos++] = ch;
      Size = Pos;
   }

   void putc(unsigned ch)
   {
        assert(ch <= 0xFF);
      if(Pos >= AllocatedBytes) Expand();
      Data[Pos++] = ch;
      if(Pos > Size) Size = Pos;
   }

   void putc(unsigned pos, unsigned ch)
   {
        assert(ch <= 0xFF);
      assert(pos < Pos);
      Data[pos] = ch;
   }

   // - Short
   void pushshort(unsigned ch)
   {
        assert(ch <= 0xFFFF);
      if(Pos + 2 > AllocatedBytes) Expand();
      memmove(&Data[2], Data, Size);
      *(unsigned short*)Data = ch;
      Pos += 2;
      if(Pos > Size) Size = Pos;
   }

    // - запись в конец потока без расширения и проверок!
   inline void _putshort(unsigned ch)
   {
      assert(Pos + 2 <= AllocatedBytes);
        assert(ch <= 0xFFFF);
      *(unsigned short*)&(Data[Pos]) = ch;
      Pos += 2;
      Size = Pos;
   }

   void putshort(unsigned ch)
   {
        assert(ch <= 0xFFFF);
      if(Pos + 2 > AllocatedBytes) Expand();
      *(unsigned short*)&(Data[Pos]) = ch;
      Pos += 2;
      if(Pos > Size) Size = Pos;
   }

   void putshort(unsigned pos, unsigned ch)
   {
        assert(ch <= 0xFFFF);
      assert(pos + 2 <= Pos);
      *(unsigned short*)&(Data[pos]) = ch;
   }

   // - Int
   void pushint(unsigned ch)
   {
      if(Pos + 4 > AllocatedBytes) Expand();
      memmove(&Data[4], Data, Size);
      *(unsigned*)Data = ch;
      Pos += 4;
      if(Pos > Size) Size = Pos;
   }

    // - запись в конец потока без расширения и проверок!
   inline void _putint(unsigned ch)
   {
      assert(Pos + 4 < AllocatedBytes);
      *(unsigned*)&(Data[Pos]) = ch;
      Pos += 4;
      Size = Pos;
   }

   void putint(unsigned ch)
   {
      if(Pos + 4 > AllocatedBytes) Expand();
      *(unsigned*)&(Data[Pos]) = ch;
      Pos += 4;
      if(Pos > Size) Size = Pos;
   }

   void putint(unsigned pos, unsigned ch)
   {
      assert(pos + 4 <= Pos);
      *(unsigned*)&(Data[pos]) = ch;
   }
    
    // - put a value in the Gamma format
   int putvalueGamma(unsigned _ch)
   {
        _ch++;
        int StoredBits = 0;

        // - подсчет числа значащих битов в величине _ch
        int bitsMant;
        GetValuedBits(_ch, bitsMant);

        unsigned mask = 0;
        mask = 1<<(bitsMant-1);
        mask >>= 1;

        // - запишем биты
      while(mask)
      {
         putb(0);
            StoredBits++;
         putb(!!(mask & _ch));
            StoredBits++;
         mask >>= 1;
      }
      putb(1);
        StoredBits++;

        return StoredBits;
    }

    // - put a value in the SEM format
   inline int putvalue(unsigned _ch, int bitsMant, int* _exp = NULL)
   {
        int StoredBits = 0;
        assert(_ch <= ((__int64)1<<bitsMant)-1);

        // - подсчет числа значащих битов экспоненты
        int bitsExp = BitsPerVal[bitsMant];

        // - подсчет числа значащих битов в величине _ch
        GetValuedBits(_ch, bitsMant);

      // - запишем биты экспоненты
      {
            if(_exp) *_exp = bitsMant;
            else
            {
                unsigned mask = 1<<(bitsExp-1);
                while(mask) putb(!!(bitsMant & mask)), mask >>= 1;
            }
            StoredBits += bitsExp;
      }

        // - пропустим самый старший бит, он всегда единица
      unsigned mask = 0;
        if(bitsMant >= 2)    mask = 1<<(bitsMant-2);

        // - запишем биты мантиссы
      while(mask)
      {
         putb(!!(mask & _ch));
         mask >>= 1;
            StoredBits++;
      }
        return StoredBits;
   }

   // - put a value in the SEM format whith the flush
   void putvalue_f(unsigned ch, int bits = 32)
   {
      putvalue(ch, bits);
      flush();
   }

   // - Block
   void pushblock(unsigned char* _Data, unsigned _Len)
   {
      if(Pos + _Len > AllocatedBytes) Expand(_Len);
      memmove(&Data[_Len], Data, Size);
      memcpy(Data, _Data, _Len);
      Pos += _Len;
      if(Pos > Size) Size = Pos;
   }

   void putblock(unsigned char* _Data, unsigned _Len)
   {
      if(Pos + _Len > AllocatedBytes) Expand(_Len);
      memcpy(&Data[Pos], _Data, _Len);
      Pos += _Len;
      if(Pos > Size) Size = Pos;
   }

   // - Stream
   void putstream(FBaseStream* Stream)
   {
      putvalue_f(Stream->Size);
      putblock(Stream->Data, Stream->Size);
   }

    FOutStream() : FBaseStream()
   {
   }

    virtual ~FOutStream()
   {
   }
};

class FInStream : virtual public FBaseStream {
public:
   // - Bit
   int getb(void)
   {
      if(Pos + 1 > AllocatedBytes) return -1;

      int bit = !!(Data[Pos] & BitMask);
      BitMask >>= 1;
      if(!BitMask)
      {
         Pos++;
         BitMask = 0x80;
      }
      return bit;
   }

   // - Char
   inline int _getc(void)
   {
      return Data[Pos++];
   }

    inline int getc(void)
   {
      if(Pos + 1 > AllocatedBytes) return -1;
      if(Pos >= Size) Size++;
      return Data[Pos++];
   }

   inline int getc(unsigned pos)
   {
      if(pos + 1 > AllocatedBytes) return -1;
      return Data[pos++];
   }

   // - Short
   inline unsigned _getshort(void)
   {
      assert(!(Pos + 2 > AllocatedBytes));
      unsigned ch = *(unsigned short*)&(Data[Pos]);
      Pos += 2;
      return ch;
   }

    unsigned getshort(void)
   {
      assert(!(Pos + 2 > AllocatedBytes));
      unsigned ch = *(unsigned short*)&(Data[Pos]);
      Pos += 2;
      if(Pos > Size) Size = Pos;
      return ch;
   }

   unsigned getshort(unsigned pos)
   {
      assert(!(pos + 2 > AllocatedBytes));
      return *(unsigned short*)&(Data[pos]);
   }

   // - Int
   inline unsigned _getint(void)
   {
      assert(!(Pos + 4 > AllocatedBytes));
      unsigned ch = *(unsigned*)&(Data[Pos]);
      Pos += 4;
      return ch;
   }

   unsigned getint(void)
   {
      assert(!(Pos + 4 > AllocatedBytes));
      unsigned ch = *(unsigned*)&(Data[Pos]);
      Pos += 4;
      if(Pos > Size) Size = Pos;
      return ch;
   }

   unsigned getint(unsigned pos)
   {
      assert(!(pos + 4 > AllocatedBytes));
      return *(unsigned*)&(Data[pos]);
   }

   // - get a value in the SEM format
   unsigned getvalue(int bitsMant = 32)
   {
        // - подсчет числа значащих битов экспоненты
      int bitsExp = 32;
        {
            unsigned mask = 1<<(32-1);
            while(mask && !(bitsMant & mask)) mask >>= 1, bitsExp--;
        }

      // - прочитаем экспоненту, равную числу битов мантиссы
        bitsMant = 0;
      {
         unsigned mask = 1<<(bitsExp-1);
         while(mask)
         {
            if(getb()) bitsMant |= mask;
            mask >>= 1;
         }
      }

      unsigned ch = 0;
      if(bitsMant)
      {
         // - если длина ненулевая, то всегда есть старший установленный бит
         ch = 1;
         bitsMant--;
         for(int i = 0; i < bitsMant; i++)
         {
            ch <<= 1;
            ch |= getb();
         }
      }
      return ch;
   }

   // - get a value in the SEM format whith flush
   unsigned getvalue_f(int bitsMant = 32)
   {
      unsigned ch = getvalue(bitsMant);
      while(BitMask != 0x80) getb();
      return ch;
   }

   // - Block
   void getblock(unsigned char* _Data, unsigned _Len)
   {
      assert(!(Pos + _Len > AllocatedBytes));
      memcpy(_Data, &Data[Pos], _Len);
      Pos += _Len;
      if(Pos > Size) Size = Pos;
   }

   void getblock(unsigned pos, unsigned char* _Data, unsigned _Len)
   {
      assert(!(pos + _Len > AllocatedBytes));
      memcpy(_Data, &Data[pos], _Len);
   }

   // - Stream
   void getstream(FBaseStream* Stream)
   {
      unsigned size = getvalue_f();
      Stream->reload(size);
      getblock(Stream->Data, size);
      Stream->Size = size;
      Stream->Pos = 0;
   }

   FInStream() : FBaseStream()
   {
   }

   virtual ~FInStream()
   {
   }
};

class FInOutStream : virtual public FInStream, virtual public FOutStream {
public:
    FInOutStream() : FInStream(), FOutStream()
   {
   }

   virtual ~FInOutStream()
   {
   }
};

}; // namespace fal

#endif
