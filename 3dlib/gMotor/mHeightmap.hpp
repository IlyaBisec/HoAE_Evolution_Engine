/*****************************************************************************/
/*	File:	mHeightmap.hpp
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	06-18-2003
/*****************************************************************************/
#ifndef __MHEIGHTMAP_H__
#define __MHEIGHTMAP_H__

const float c_NoHeight = -999999999999.9f;
/*****************************************************************************/
/*	Class:	HeightMap
/*	Desc:
/*****************************************************************************/
template <int NWidth, int NHeight = NWidth>
class HeightMap
{
	static const int NNodes = NWidth*NHeight;

	static const int nMinX	= -NWidth	/ 2;
	static const int nMaxX	=  NWidth	/ 2;
	static const int nMinY	= -NHeight	/ 2;
	static const int nMaxY	=  NHeight	/ 2;

	float			h[NNodes];
public:
	void ResetHeight( float val )
	{
		for (int i = 0; i < NNodes; i++)
		{
			h[i] = val;
		}
	} // ResetHeight

	float GetHeight( int x, int y )
	{
		if (x < nMinX || x > nMaxX || 
			y < nMinY || y > nMaxY) 
		{
			return c_NoHeight;
		}

		return h[x - nMinX + (y - nMinY)*NWidth];
	} // GetHeight

	float GetHeight( float x, float y )
	{
		return GetHeight( (int)x, (int)y );
	} // GetHeight

	void SetHeight( int x, int y, float height )
	{
		if (x < nMinX || x > nMaxX || 
			y < nMinY || y > nMaxY) 
		{
			return;
		}
		h[x - nMinX + (y - nMinY)*NWidth] = height;
	} // SetHeight

	int GetMaxX() const
	{
		return nMaxX;
	}

	int GetMaxY() const
	{
		return nMaxY;
	}

	int GetMinX() const
	{
		return nMinX;
	}

	int GetMinY() const
	{
		return nMinY;
	}

}; // class HeightMap

#endif // __MHEIGHTMAP_H__