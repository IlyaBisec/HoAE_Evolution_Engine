/*****************************************************************************/
/*	File:	mFilter.hpp
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	07-24-2003
/*****************************************************************************/

const WORD c_ABlurringKernel[] = {  1, 2, 1, 
									2, 4, 2, 
									1, 2, 1 };

const WORD c_BBlurringKernel[] = {  1, 1, 1, 1, 1,
									1, 1, 1, 1, 1,
									1, 1, 1, 1, 1,
									1, 1, 1, 1, 1,
									1, 1, 1, 1, 1 };

const WORD c_Gaussian1Kernel[] = {  1, 1, 2, 1, 1,
									1, 2, 4, 2, 1,
									2, 4, 8, 4, 2,
									1, 2, 4, 2, 1,
									1, 1, 2, 1, 1 };

const WORD c_Gaussian2Kernel[] = {  1, 4,  7,  4,  1,
									4, 16, 26, 16, 4,
									7, 26, 41, 26, 7,
									4, 16, 26, 16, 4,
									1, 4,  7,  4,  1 };
/*****************************************************************************/
/*	Filter implementation
/*****************************************************************************/
template <int order>
Filter<order>::Filter( const WORD* pKernel )
{
	int nEl = order * order;
	memcpy( kernel, pKernel, nEl * sizeof( WORD ) );
	scale = 0;
	for (int i = 0; i < nEl; i++)
	{
		scale += kernel[i];
	}
	
	int oMin = -order/2;
	int oMax = order/2;
	if (oMax - oMin + 1 > order) oMax--;

	int cK = 0;
	for (int i = oMin; i <= oMax; i++)
	{
		for (int j = oMin; j <= oMax; j++)
		{
			kShiftX[cK] = j;
			kShiftY[cK] = i;
			cK++;
		}
	}
} // Filter::Filter

template <int order>
_inl DWORD	Filter<order>::GetPixel( DWORD* arr, int x, int y, int w, int h ) const
{
	if (x < 0) x = 0; 
	if (y < 0) y = 0;
	if (x >= w) x = w - 1;
	if (y >= h) y = h - 1;
	return arr[x + y*w];
}

template <int order>
_inl BYTE	Filter<order>::GetPixel( BYTE* arr, int x, int y, int w, int h ) const
{
	if (x < 0) x = 0; 
	if (y < 0) y = 0;
	if (x >= w) x = w - 1;
	if (y >= h) y = h - 1;
	return arr[x + y*w];
}

template <int order>
bool Filter<order>::PerByteConvolve( DWORD* arr, BYTE* maskArr, 
									 int arrW, int arrH, 
									 DWORD resMask )
{
	int nElem = arrW * arrH;
	DWORD* tempArr = new DWORD[nElem];
	int cPixel = 0;
	for (int i = 0; i < arrH; i++)
	{
		for (int j = 0; j < arrW; j++)
		{
			//  do convolution for this pixel
			DWORD b0 = 0, b1 = 0, b2 = 0, b3 = 0;
			DWORD cScale = 0;
			for (int k = 0; k < kSize; k++)
			{		
				if (GetPixel(	maskArr, 
								j + kShiftX[k], i + kShiftY[k], 
								arrW, arrH ) == 0) continue;

				DWORD pixel = GetPixel( arr, j + kShiftX[k], i + kShiftY[k], 
					arrW, arrH );
				b0 += kernel[k] * ((pixel >> 24) & 0x000000FF);
				b1 += kernel[k] * ((pixel >> 16) & 0x000000FF);
				b2 += kernel[k] * ((pixel >>  8) & 0x000000FF);
				b3 += kernel[k] * ((pixel	   ) & 0x000000FF);
				cScale += kernel[k];
			}
			if (cScale == 0) 
			{
				tempArr[cPixel++] = 0;
				continue;
			}

			b0 /= cScale; b1 /= cScale; b2 /= cScale; b3 /= cScale;
			tempArr[cPixel++] = (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
		}
	}

	for (int i = 0; i < nElem; i++)
	{
		arr[i] &= ~resMask;
		arr[i] |= (tempArr[i] & resMask);
	}
	delete []tempArr;
	return true;
} //  Filter::PerByteConvolve

template <int order>
bool Filter<order>::PerByteConvolve( DWORD* arr, int arrW, int arrH, DWORD resMask )
{
	int nElem = arrW * arrH;
	DWORD* tempArr = new DWORD[nElem];
	int cPixel = 0;
	for (int i = 0; i < arrH; i++)
	{
		for (int j = 0; j < arrW; j++)
		{
			//  do convolution for this pixel
			DWORD b0 = 0, b1 = 0, b2 = 0, b3 = 0;
			for (int k = 0; k < kSize; k++)
			{			
				DWORD pixel = GetPixel( arr, j + kShiftX[k], i + kShiftY[k], 
										arrW, arrH );
				b0 += kernel[k] * ((pixel >> 24) & 0x000000FF);
				b1 += kernel[k] * ((pixel >> 16) & 0x000000FF);
				b2 += kernel[k] * ((pixel >>  8) & 0x000000FF);
				b3 += kernel[k] * ((pixel	   ) & 0x000000FF);
			}
			
			b0 /= scale; b1 /= scale; b2 /= scale; b3 /= scale;
			tempArr[cPixel++] = (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
		}
	}
	
	for (int i = 0; i < nElem; i++)
	{
		arr[i] &= ~resMask;
		arr[i] |= (tempArr[i] & resMask);
	}
	delete []tempArr;
	return true;
} // Filter::PerByteConvolve
