/*****************************************************************
/*  File:   mUtil.inl                                       
/*  Author: Silver, Copyright (C) GSC Game World                  
/*****************************************************************/
#include <math.h>
#include "mConstants.h"

template <typename T> void SmoothFilter( T* arr, int width, int height )
{
	int w1 = width	- 1;
	int h1 = height - 1;
	int cp = width + 1;
	int val;
	for (int i = 1; i < w1; i++)
	{
		for (int j = 1; j < h1; j++)
		{
			val =
				arr[cp - width - 1] + arr[cp - width] + arr[cp - width + 1]	+
				arr[cp		   - 1]	+ arr[cp		] + arr[cp		   + 1]	+ 
				arr[cp + width - 1]	+ arr[cp + width] + arr[cp + width + 1];
			val /= 9;
			arr[cp] = val;
			cp++;
		}
		cp += 2;
	}
}

