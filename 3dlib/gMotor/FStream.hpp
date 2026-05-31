#ifndef __FSTREAM__
#define __FSTREAM__

#include <windows.h>
#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>

//#define FSTREAMSHOWINFO
#define FOUTSTREAM_DELTA 4096

#ifdef putc
#undef putc
#endif

#ifdef getc
#undef getc
#endif

class FStream {
public:
	union {
		char* cData;
		unsigned char* Data;
		int* iData;
		unsigned int* uiData;
		short* sData;
		unsigned short* usData;
	};
   unsigned Pos, BitMask, Size, AllocatedBytes;
   bool isAttached;

   void rewind(void)
   {
      Pos = 0;
      BitMask = 0x80;
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

	bool save(char* FileName, bool isAppend = false)
   {
   	FILE* out;
		if(isAppend)
	   	out = fopen(FileName, "ab");
		else
	   	out = fopen(FileName, "wb");
	   if(out == NULL) return false;
      // - сохраняем данные
      fwrite(Data, Size, 1, out);
      fclose(out);

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

   bool attach(FStream* s)
   {
      release();
      Data = s->Data;
      AllocatedBytes = s->AllocatedBytes;
      Size = s->Size;
      isAttached = true;
      return true;
   }

   // - выгрузить свой поток в другой
   bool remount(FStream* s)
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

   void release(void)
   {
      if(!isAttached && Data) delete[] Data;
      drop();
   }

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

   void putc(unsigned ch)
   {
		assert(ch <= 0xFF);
      if(Pos >= AllocatedBytes) Expand();
      Data[Pos++] = ch;
      if(Pos > Size) Size = Pos;
   }

	// - запись в конец потока без расширения и проверок!
   inline void _putc(unsigned ch)
   {
		assert(ch <= 0xFF);
      Data[Pos++] = ch;
      Size = Pos;
   }

	unsigned getc(void)
   {
      if(Pos + 1 > AllocatedBytes) return -1;
      if(Pos >= Size) Size++;
      return Data[Pos++];
   }

	inline int _getc(void)
   {
      return Data[Pos++];
   }

   void putc(unsigned pos, unsigned ch)
   {
		assert(ch <= 0xFF);
      assert(pos < Pos);
      Data[pos] = ch;
   }

	unsigned getc(unsigned pos)
   {
      if(pos + 1 > AllocatedBytes) return -1;
      return Data[pos++];
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
   inline void _putshort(short ch)
   {
      assert(Pos + 2 <= AllocatedBytes);
      *(short*)&(Data[Pos]) = ch;
      Pos += 2;
      Size = Pos;
   }

   void putshort(short ch)
   {
      if(Pos + 2 > AllocatedBytes) Expand();
      *(short*)&(Data[Pos]) = ch;
      Pos += 2;
      if(Pos > Size) Size = Pos;
   }

	short getshort(void)
   {
      if(Pos + 2 > AllocatedBytes)
         assert(!(Pos + 2 > AllocatedBytes));
      short ch = *(short*)&(Data[Pos]);
      Pos += 2;
      if(Pos > Size) Size = Pos;
      return ch;
   }

	inline short _getshort(void)
   {
      assert(!(Pos + 2 > AllocatedBytes));
      short ch = *(short*)&(Data[Pos]);
      Pos += 2;
      return ch;
   }

   void putshort(unsigned pos, short ch)
   {
      assert(pos + 2 <= Pos);
      *(short*)&(Data[pos]) = ch;
   }

   short getshort(unsigned pos)
   {
      assert(!(pos + 2 > AllocatedBytes));
      return *(short*)&(Data[pos]);
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

	// - запись в конец потока без расширения и проверок!
   inline void _putint(unsigned ch)
   {
      assert(Pos + 4 < AllocatedBytes);
      *(unsigned*)&(Data[Pos]) = ch;
      Pos += 4;
      Size = Pos;
   }

	unsigned getint(void)
   {
      if(Pos + 4 > AllocatedBytes)
         assert(!(Pos + 4 > AllocatedBytes));
      unsigned ch = *(unsigned*)&(Data[Pos]);
      Pos += 4;
      if(Pos > Size) Size = Pos;
      return ch;
   }

	inline unsigned _getint(void)
   {
      assert(!(Pos + 4 > AllocatedBytes));
      unsigned ch = *(unsigned*)&(Data[Pos]);
      Pos += 4;
      return ch;
   }

   void putint(unsigned pos, unsigned ch)
   {
      assert(pos + 4 <= Pos);
      *(unsigned*)&(Data[pos]) = ch;
   }

	unsigned getint(unsigned pos)
   {
      assert(!(pos + 4 > AllocatedBytes));
      return *(unsigned*)&(Data[pos]);
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
		if(_Len)
		{
			if(Pos + _Len > AllocatedBytes) Expand(_Len);
			memcpy(&Data[Pos], _Data, _Len);
			Pos += _Len;
			if(Pos > Size) Size = Pos;
		}
   }

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

	FStream()
   {
      drop();
   }


   ~FStream()
   {
		release();
   }
};

#endif
