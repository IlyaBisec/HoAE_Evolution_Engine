//---------------------------------------------------------------------------
#include "FCompressor.h"
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
bool FCompressor::CompressBlock(char** pOutData, unsigned* pOutLen, char* InData, unsigned InLen)
{
	unsigned char Type;
	unsigned PackedLen;
	unsigned RawLen;

	int HdrSize = sizeof(Type) + sizeof(PackedLen) + sizeof(RawLen);
	char* _pDestBuf = new char[InLen + HdrSize];
	char* pDestBuf = _pDestBuf + HdrSize;
	unsigned OutLen = InLen;

	//---------------------- BZ2 compresssion --------------------------------
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
			 strm_C.next_in = InData;
			 strm_C.next_out = pDestBuf;
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
	}
	//------------------------------------------------------------------------

	*((unsigned char*)&(_pDestBuf[0])) = FCOMPRESSOR_TYPE_BZ2;
	*((unsigned*)&(_pDestBuf[sizeof(Type)])) = OutLen;
	*((unsigned*)&(_pDestBuf[sizeof(Type) + sizeof(PackedLen)])) = InLen;

	*pOutData = _pDestBuf;
	*pOutLen = OutLen + HdrSize;
	
	return true;	
}

//---------------------------------------------------------------------------
bool FCompressor::DecompressBlock(char** ppDestBuf, unsigned* pDestLen, char* pSrcBuf)
{
   assert(ppDestBuf);
   assert(pDestLen);

   unsigned char Type;
   unsigned PackedLen;
   unsigned RawLen;

   Type = *((unsigned char*)&(pSrcBuf[0]));
   PackedLen = *((unsigned*)&(pSrcBuf[sizeof(Type)]));
   RawLen = *((unsigned*)&(pSrcBuf[sizeof(Type) + sizeof(PackedLen)]));

   int HdrSize = sizeof(Type) + sizeof(PackedLen) + sizeof(RawLen);

   pSrcBuf += HdrSize;

   char* pDestBuf = new char[RawLen];

	//---------------------- BZ2 decompresssion ------------------------------
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

		strm_D.next_in = pSrcBuf;
		strm_D.next_out = pDestBuf;
		strm_D.avail_in = PackedLen;
		strm_D.avail_out = RawLen;

		int ret = BZ2_bzDecompress(&strm_D);
      //assert(ret != BZ_OK);
      //assert(ret == BZ_STREAM_END);
 
		// normal termination
		RawLen -= strm_D.avail_out;

        //---------------------- BZ2 deinitialization -------------------------
		{
			BZ2_bzDecompressEnd(&strm_D);
		}
		//---------------------------------------------------------------------
   }
   //------------------------------------------------------------------------

   *ppDestBuf = pDestBuf;
   *pDestLen = RawLen;

   return true;
}
//---------------------------------------------------------------------------

