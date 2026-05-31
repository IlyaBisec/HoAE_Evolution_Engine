/*****************************************************************************/
/*	File:	kUtilities.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	23.04.2003
/*****************************************************************************/
#include "mConstants.h"
#include "mUtil.h"
/*****************************************************************************/
/*	util implementation
/*****************************************************************************/
_inl void util::MemcpyRect(	BYTE* dst,			int dstPitch,
							const BYTE* src,	int srcPitch, 
							int nLines, 
							int bytesInLine )
{
	for (int i = 0; i < nLines; i++)
	{
		memcpy( dst, src, bytesInLine );
		dst += dstPitch;
		src += srcPitch;
	}
} // util::MemcpyRect


#define GET_A(W) ((W & 0xF000)>>12)
#define GET_R(W) ((W & 0x0F00)>>8)
#define GET_G(W) ((W & 0x00F0)>>4)
#define GET_B(W) ((W & 0x000F))
#define ARGB(A,R,G,B) (((A)<<12) | ((R)<<8) | ((G)<<4) | (B))
#define SHRINK_METHOD 1
_inl void util::QuadShrink2xW( BYTE* dst,		int dstPitch,
							   const BYTE* src,	int srcPitch, 
							   int srcQuadSide )
{
	int dstQuadSide = srcQuadSide >> 1;
	int srcPitch2 = srcPitch << 1;
	for (int  i = 0; i < dstQuadSide; i++)
	{
		WORD* pDst  = (WORD*)dst;
		WORD* pSrc1 = (WORD*)src;
		WORD* pSrc2 = (WORD*)(src + srcPitch);
		for (int j = 0; j < dstQuadSide; j++)
		{
#if SHRINK_METHOD == 1
			DWORD pix[4];
			pix[0] = pSrc1[j * 2		];
			pix[1] = pSrc1[j * 2 + 1	];
			pix[2] = pSrc2[j * 2		];
			pix[3] = pSrc2[j * 2 + 1	];			
			DWORD a = GET_A(pix[0]) + GET_A(pix[1]) + GET_A(pix[2]) + GET_A(pix[3]);
			if(a>0&&a<13*4){
				DWORD amax = tmax( GET_A(pix[0]), GET_A(pix[1]), GET_A(pix[2]), GET_A(pix[3]) );
				DWORD amin = tmin( GET_A(pix[0]), GET_A(pix[1]), GET_A(pix[2]), GET_A(pix[3]) );

				if (amax == amin)
				{
					DWORD r = GET_R(pix[0]) + GET_R(pix[1]) + GET_R(pix[2]) + GET_R(pix[3]);
					DWORD g = GET_G(pix[0]) + GET_G(pix[1]) + GET_G(pix[2]) + GET_G(pix[3]);
					DWORD b = GET_B(pix[0]) + GET_B(pix[1]) + GET_B(pix[2]) + GET_B(pix[3]);

					r >>= 2; g >>= 2; b >>= 2;

					r=(GET_R(pix[0])+r)>>1;
					g=(GET_G(pix[0])+g)>>1;
					b=(GET_B(pix[0])+b)>>1;
					pDst[j] = ARGB( amax, r, g, b );
				}
				else
				{
					int maxIdx = 0;
					if (GET_A(pix[1]) > GET_A(pix[0]))		maxIdx = 1;
					if (GET_A(pix[2]) > GET_A(pix[maxIdx])) maxIdx = 2;
					if (GET_A(pix[3]) > GET_A(pix[maxIdx])) maxIdx = 3;
					pDst[j] = pix[maxIdx];
				}
			}else{
				DWORD r = GET_R(pix[0]) + GET_R(pix[1]) + GET_R(pix[2]) + GET_R(pix[3]);
				DWORD g = GET_G(pix[0]) + GET_G(pix[1]) + GET_G(pix[2]) + GET_G(pix[3]);
				DWORD b = GET_B(pix[0]) + GET_B(pix[1]) + GET_B(pix[2]) + GET_B(pix[3]);				
				r >>= 2; g >>= 2; b >>= 2; a >>= 2;
				pDst[j] = ARGB( a , r, g, b );			
			}
#endif //SHRINK_METHOD == 1
#if SHRINK_METHOD == 2
			DWORD pix[4];
			pix[0] = pSrc1[j * 2		];
			pix[1] = pSrc1[j * 2 + 1	];
			pix[2] = pSrc2[j * 2		];
			pix[3] = pSrc2[j * 2 + 1	];
			
			DWORD amax = tmax( GET_A(pix[0]), GET_A(pix[1]), GET_A(pix[2]), GET_A(pix[3]) );
			DWORD amin = tmin( GET_A(pix[0]), GET_A(pix[1]), GET_A(pix[2]), GET_A(pix[3]) );
			
			if (amax == amin)
			{
				DWORD r = GET_R(pix[0]) + GET_R(pix[1]) + GET_R(pix[2]) + GET_R(pix[3]);
				DWORD g = GET_G(pix[0]) + GET_G(pix[1]) + GET_G(pix[2]) + GET_G(pix[3]);
				DWORD b = GET_B(pix[0]) + GET_B(pix[1]) + GET_B(pix[2]) + GET_B(pix[3]);
						 
				r >>= 2; g >>= 2; b >>= 2;

				r=(GET_R(pix[0])+r)>>1;
				g=(GET_G(pix[0])+g)>>1;
				b=(GET_B(pix[0])+b)>>1;
				pDst[j] = ARGB( amax, r, g, b );
			}
			else
			{
				int maxIdx = 0;
				if (GET_A(pix[1]) > GET_A(pix[0]))		maxIdx = 1;
				if (GET_A(pix[2]) > GET_A(pix[maxIdx])) maxIdx = 2;
				if (GET_A(pix[3]) > GET_A(pix[maxIdx])) maxIdx = 3;
				pDst[j] = pix[maxIdx];
			}
#endif //SHRINK_METHOD == 2
		}
		dst += dstPitch;
		src += srcPitch2;
	}
} // util::QuadShrink2xW

_inl void util::QuadShrink4xW( BYTE* dst,		int dstPitch,
						   const BYTE* src,	int srcPitch, 
							   int srcQuadSide )
{
	int dstQuadSide = srcQuadSide >> 2;
	int srcPitch4 = srcPitch << 2;
	for (int  i = 0; i < dstQuadSide; i++)
	{
		WORD* pDst  = (WORD*)dst;
		WORD* pSrc1 = (WORD*)src;
		WORD* pSrc2 = (WORD*)(src + srcPitch);
		for (int j = 0; j < dstQuadSide; j++)
		{
			DWORD pix[4];
			pix[0] = pSrc1[j * 4		];
			pix[1] = pSrc1[j * 4 + 1	];
			pix[2] = pSrc2[j * 4		];
			pix[3] = pSrc2[j * 4 + 1	];
#if SHRINK_METHOD == 1
			DWORD a = GET_A(pix[0]) + GET_A(pix[1]) + GET_A(pix[2]) + GET_A(pix[3]);
			if(a>0&&a<13*4){
				DWORD amax = tmax( GET_A(pix[0]), GET_A(pix[1]), GET_A(pix[2]), GET_A(pix[3]) );
				DWORD amin = tmin( GET_A(pix[0]), GET_A(pix[1]), GET_A(pix[2]), GET_A(pix[3]) );

				if (amax == amin)
				{
					DWORD r = GET_R(pix[0]) + GET_R(pix[1]) + GET_R(pix[2]) + GET_R(pix[3]);
					DWORD g = GET_G(pix[0]) + GET_G(pix[1]) + GET_G(pix[2]) + GET_G(pix[3]);
					DWORD b = GET_B(pix[0]) + GET_B(pix[1]) + GET_B(pix[2]) + GET_B(pix[3]);

					r >>= 2; g >>= 2; b >>= 2;

					r=(GET_R(pix[0])+r)>>1;
					g=(GET_G(pix[0])+g)>>1;
					b=(GET_B(pix[0])+b)>>1;
					pDst[j] = ARGB( amax, r, g, b );
				}
				else
				{
					int maxIdx = 0;
					if (GET_A(pix[1]) > GET_A(pix[0]))		maxIdx = 1;
					if (GET_A(pix[2]) > GET_A(pix[maxIdx])) maxIdx = 2;
					if (GET_A(pix[3]) > GET_A(pix[maxIdx])) maxIdx = 3;
					pDst[j] = pix[maxIdx];
				}
			}else{
				DWORD r=0;
				DWORD g=0;
				DWORD b=0;
				DWORD a=0;
				for(int iy=0;iy<4;iy++){
					int d=srcPitch*iy;
					for(int jx=0;jx<4;jx++){
                        WORD C=*((WORD*)(src+d+jx+jx+(j<<3)));
						r+=GET_R(C);
						g+=GET_G(C);
						b+=GET_B(C);
						a+=GET_A(C);
					}
				}			

				r >>= 4; g >>= 4; b >>= 4; a >>= 4;
				pDst[j] = ARGB( a , r, g, b );			
			}
#endif //#if SHRINK_METHOD == 1
#if SHRINK_METHOD == 2			
			
			DWORD amax = tmax( GET_A(pix[0]), GET_A(pix[1]), GET_A(pix[2]), GET_A(pix[3]) );
			DWORD amin = tmin( GET_A(pix[0]), GET_A(pix[1]), GET_A(pix[2]), GET_A(pix[3]) );
			
			if (amax == amin)
			{
				DWORD r = GET_R(pix[0]) + GET_R(pix[1]) + GET_R(pix[2]) + GET_R(pix[3]);
				DWORD g = GET_G(pix[0]) + GET_G(pix[1]) + GET_G(pix[2]) + GET_G(pix[3]);
				DWORD b = GET_B(pix[0]) + GET_B(pix[1]) + GET_B(pix[2]) + GET_B(pix[3]);
						 
				r >>= 2; g >>= 2; b >>= 2;

				r=(GET_R(pix[0])+r)>>1;
				g=(GET_G(pix[0])+g)>>1;
				b=(GET_B(pix[0])+b)>>1;
				pDst[j] = ARGB( amax, r, g, b );
			}
			else
			{
				int maxIdx = 0;
				if (GET_A(pix[1]) > GET_A(pix[0]))		maxIdx = 1;
				if (GET_A(pix[2]) > GET_A(pix[maxIdx])) maxIdx = 2;
				if (GET_A(pix[3]) > GET_A(pix[maxIdx])) maxIdx = 3;
				pDst[j] = pix[maxIdx];
			}
#endif //#if SHRINK_METHOD == 2
		}
		dst += dstPitch;
		src += srcPitch4;
	}
} // util::QuadShrink4xW

