#ifndef FStreamsH
#define FStreamsH

#include <windows.h>
#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <io.h>

#define FOUTSTREAM_DELTA 4096

class f_FBaseStream {
public:
   unsigned char* Data;
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
      if(!Data)
      {
         Data = new unsigned char[len];
         AllocatedBytes = len;
      }
      else
      if(AllocatedBytes < len)
      {
         delete[] Data;
         Data = new unsigned char[len];
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

   bool attach(f_FBaseStream* s)
   {
      release();
      Data = s->Data;
      AllocatedBytes = s->AllocatedBytes;
      Size = s->Size;
      isAttached = true;
      return true;
   }

   // - выгрузить свой поток в другой
   bool remount(f_FBaseStream* s)
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

      unsigned char* tmp = new unsigned char[len];
      memcpy(tmp, Data, AllocatedBytes);
      delete[] Data;
      Data = tmp;
      AllocatedBytes = len;
   }

   f_FBaseStream()
   {
      drop();
   }

   ~f_FBaseStream()
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

class f_FOutStream : public f_FBaseStream {
public:
   // - Bit
   void putb(bool bit)
   {
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
      if(Pos >= AllocatedBytes) Expand();
      memmove(&Data[1], Data, Size);
      Data[0] = ch;
      Pos++;
      if(Pos > Size) Size = Pos;
   }

   void putc(unsigned ch)
   {
      if(Pos >= AllocatedBytes) Expand();
      Data[Pos++] = ch;
      if(Pos > Size) Size = Pos;
   }

   void putc(unsigned pos, unsigned ch)
   {
      assert(pos < Pos);
      Data[pos] = ch;
   }

   // - Short
   void pushshort(unsigned ch)
   {
      if(Pos + 2 > AllocatedBytes) Expand();
      memmove(&Data[2], Data, Size);
      *(unsigned short*)Data = ch;
      Pos += 2;
      if(Pos > Size) Size = Pos;
   }

   void putshort(unsigned ch)
   {
      if(Pos + 2 > AllocatedBytes) Expand();
      *(unsigned short*)&(Data[Pos]) = ch;
      Pos += 2;
      if(Pos > Size) Size = Pos;
   }

   void putshort(unsigned pos, unsigned ch)
   {
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

   // - 32-bit value in the SEM format
   void putvalue(unsigned ch)
   {
      int bnum32 = 32;
      unsigned mask32 = 1<<(bnum32-1);
      while(mask32 && !(ch & mask32)) mask32 >>= 1, bnum32--;

      // - запишем шесть бит величины bnum32
      {
         unsigned mask6 = 1<<(6-1);
         while(mask6) putb(!!(bnum32 & mask6)), mask6 >>= 1;
      }

      mask32 >>= 1;
      while(mask32)
      {
         putb(!!(mask32 & ch));
         mask32 >>= 1;
      }
   }

   // - 32-bit value in the SEM format
   void putvalue_f(unsigned ch)
   {
      putvalue(ch);
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
   void putstream(f_FBaseStream* Stream)
   {
      putvalue_f(Stream->Size);
      putblock(Stream->Data, Stream->Size);
   }

   f_FOutStream()
   {
   }

   ~f_FOutStream()
   {
   }
};

class f_FInStream : public f_FBaseStream {
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
   int getc(void)
   {
      if(Pos + 1 > AllocatedBytes) return -1;
      if(Pos >= Size) Size++;
      return Data[Pos++];
   }

   int getc(unsigned pos)
   {
      if(pos + 1 > AllocatedBytes) return -1;
      return Data[pos++];
   }

   // - Short
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

   // - 32-bit value in the SEM format
   unsigned getvalue(void)
   {
      int bnum32 = 0;
      // - прочитаем шесть бит величины bnum32
      {
         unsigned mask6 = 1<<(6-1);
         while(mask6)
         {
            if(getb()) bnum32 |= mask6;
            mask6 >>= 1;
         }
      }

      unsigned ch = 0;
      if(bnum32)
      {
         // - если длина ненулевая, то всегда есть старший установленный бит
         ch = 1;
         bnum32--;
         for(int i = 0; i < bnum32; i++)
         {
            ch <<= 1;
            ch |= getb();
         }
      }
      return ch;
   }

   // - 32-bit value in the SEM format whith flush
   unsigned getvalue_f(void)
   {
      unsigned ch = getvalue();
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
   void getstream(f_FBaseStream* Stream)
   {
      unsigned size = getvalue_f();
      Stream->reload(size);
      getblock(Stream->Data, size);
      Stream->Size = size;
      Stream->Pos = 0;
   }

   f_FInStream() : f_FBaseStream()
   {
   }

   ~f_FInStream()
   {
   }
};

#endif
