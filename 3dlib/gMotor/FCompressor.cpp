//---------------------------------------------------------------------------
#include <assert.h>
#include "FCompressor.h"
#include "FStream.hpp"
#include "bz2\bzlib.h"
#include "kLog.h"
#include "lzo1x.h"
#define CEXPORT __declspec(dllexport)
//---------------------------------------------------------------------------
int lzo1x_decompress_asm_fast(unsigned char* src, unsigned int src_len,
                              unsigned char* dst, unsigned int* dst_len,
                              unsigned char* wrkmem);
//---------------------------------------------------------------------------
bool FLZDecompressMemory(unsigned char* InData, unsigned char* OutData, int InLen, int OutLen);
//---------------------------------------------------------------------------
bool FCompressor::Initialize(void)
{
   if(isInitialized) return true;
   isInitialized = true;
   return true;
}
//---------------------------------------------------------------------------
bool FCompressor::Release(void)
{
   if(!isInitialized) return true;
   isInitialized = false;
   return true;
}
//---------------------------------------------------------------------------
bool FCompressor::CompressBlock(unsigned char** pOutData, unsigned* pOutLen,
                                unsigned char* InData, unsigned InLen)
{
   assert(pOutData);
   assert(pOutLen);

   FStream InStream;
   FStream OutStream;

   InStream.attach(InData, InLen);
   OutStream.reload(InLen*2 + 9);
   OutStream.Pos = OutStream.Size = 9;

   unsigned char* OutData = OutStream.Data + OutStream.Pos;
   unsigned OutLen = InLen;

   switch(type) {
      case FCOMPRESSOR_TYPE_BZ2:
      {
         bz_stream strm_C;
         //---------------------- BZ2 initialization ---------------------------
         {
            int blockSize100k = 9;
            int verbosity = 0;
            int workFactor = 30;
            strm_C.bzalloc = NULL;
            strm_C.bzfree = NULL;
            strm_C.opaque = NULL;
            int ret = BZ2_bzCompressInit(&strm_C, blockSize100k, verbosity, workFactor);
            if(ret != BZ_OK) return false;
         }
         //------------------------- Compression -------------------------------
         {
            strm_C.next_in = (char*)InData;
            strm_C.next_out = (char*)OutData;
            strm_C.avail_in = InLen;
            strm_C.avail_out = OutLen;
            int ret = BZ2_bzCompress(&strm_C, BZ_FINISH);
            assert(ret != BZ_FINISH_OK);
            assert(ret == BZ_STREAM_END);

            // normal termination
            OutLen -= strm_C.avail_out;
         }
         //---------------------- BZ2 deinitialization -------------------------
         {
            BZ2_bzCompressEnd(&strm_C);
         }
         //---------------------------------------------------------------------
         break;
      }
      case FCOMPRESSOR_TYPE_PPMd:
         assert(!"PPMd is not supported!");
		 break;
      case FCOMPRESSOR_TYPE_LZO:
		  {
			  static long wrkmem[LZO1X_1_15_MEM_COMPRESS];
			  unsigned int outlen = 0;
			  lzo_init();				  
			  int r = lzo1x_1_15_compress(InData, InLen, OutData, (lzo_uint*)&OutLen, wrkmem);
			  OutStream.putblock(OutData, OutLen);			  
		  }
         break;
      case FCOMPRESSOR_TYPE_FLZ:
         assert(!"FLZ is not supported!");
		 break;
   };
   OutStream.putc(0, type);
   OutStream.putint(1, OutLen);
   OutStream.putint(5, InLen);

   *pOutData = OutStream.Data;
   *pOutLen = OutLen + 9;
   OutStream.drop();

   return true;
}
//---------------------------------------------------------------------------
bool FCompressor::DecompressBlock(unsigned char** pOutData, unsigned* pOutLen, unsigned char* InData)
{
   assert(pOutData);
   assert(pOutLen);

   FStream InStream;
   FStream OutStream;
   InStream.attach(InData, 9);

   unsigned char Type = InStream.getc();
   unsigned InLen = InStream.getint();
   unsigned OutLen = InStream.getint();

   InStream.attach(InData+9, InLen);
   OutStream.reload(OutLen+3);

   switch(Type) {
      case FCOMPRESSOR_TYPE_BZ2:
      {
         bz_stream strm_D;
         //---------------------- BZ2 initialization ---------------------------
         {
            int verbosity = 0;

            strm_D.bzalloc = NULL;
            strm_D.bzfree = NULL;
            strm_D.opaque = NULL;
            int ret = BZ2_bzDecompressInit(&strm_D, verbosity, 0);
            if(ret != BZ_OK) return false;
         }
         //------------------------- Decompression -----------------------------
         {
            strm_D.next_in = (char*)InStream.Data;
            strm_D.next_out = (char*)OutStream.Data;
            strm_D.avail_in = InLen;
            strm_D.avail_out = OutLen;

            int ret = BZ2_bzDecompress(&strm_D);
            assert(ret != BZ_OK);
            assert(ret == BZ_STREAM_END);

            // normal termination
            OutLen -= strm_D.avail_out;
         }
         //---------------------- BZ2 deinitialization -------------------------
         {
            BZ2_bzDecompressEnd(&strm_D);
         }
         //---------------------------------------------------------------------
         break;
      }
      case FCOMPRESSOR_TYPE_PPMd:
         assert(!"PPMd is not supported!");
      case FCOMPRESSOR_TYPE_LZO:
      {
           if(lzo1x_decompress_asm_fast(InStream.Data, InLen,
            OutStream.Data, &OutStream.Pos, NULL) < 0) throw("LZO decompression failed!");
         break;
      }
      case FCOMPRESSOR_TYPE_FLZ:
      {
		  try{
			if(!FLZDecompressMemory(InStream.Data, OutStream.Data, InLen, OutLen))
            {
            }
		  }catch(...){
            Log.Warning("FLZ decompression failed!");
		  }
         break;
      }
   };
   *pOutData = OutStream.Data;
   *pOutLen = OutLen;
   OutStream.drop();

   return true;
};
//---------------------------------------------------------------------------

CEXPORT bool CompressData(unsigned char** pOutData, unsigned* pOutLen,
                                unsigned char* InData, unsigned InLen)
{
    FCompressor FCOMP;
    FCOMP.type=FCOMPRESSOR_TYPE_BZ2;
    return FCOMP.CompressBlock(pOutData, pOutLen, InData, InLen);
};
CEXPORT bool DecompressData(unsigned char** pOutData, unsigned* pOutLen, unsigned char* InData)
{
    FCompressor FCOMP;
    FCOMP.type=FCOMPRESSOR_TYPE_BZ2;
    return FCOMP.DecompressBlock(pOutData, pOutLen, InData);
};