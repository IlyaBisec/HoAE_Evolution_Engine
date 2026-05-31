/*****************************************************************************/
/*	File:	GPSeries.h
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	15.04.2003
/*****************************************************************************/
#ifndef __GPSERIESMGR_H__
#define __GPSERIESMGR_H__

#ifndef _NOSTAT
#include "kStatistics.h"

namespace sg{

const int		c_MaxSeriesNameLen	= 64;
const int		c_MaxSeries			= 16;
const int		c_SeriesLen			= 512;	

const float		c_LegendLineWidth	= 160.0f;
const float		c_LegendLineHeight	= 16.0f;
const float		c_DefSeriesWndX		= 100.0f;
const float		c_DefSeriesWndY		= 500.0f;
const float		c_DefSeriesWndW		= 600.0f;
const float		c_DefSeriesWndH		= 220.0f;
				
const DWORD		c_LegendFrameColor	= 0xFFEEEEEE;
const DWORD		c_SeriesFrameColor	= 0xFFFFFF00;


class GPFont;
class GPSystem;
/*****************************************************************************/
/*	Class:	GPSeries
/*	Desc:	Represents single value changing in time 
/*****************************************************************************/
class GPSeries
{
	float*		val;
	float		maxVal;
	
	int			maxLen;
	int			len;
	int			curPos;
	
	char		name[c_MaxSeriesNameLen];
	DWORD		color;

public:
					GPSeries();
					~GPSeries();

	void			Init( const char* _name, DWORD _color, float _maxVal, int _maxLen );
	const char*		GetName() const;
	void			PushValue( float value );
	void			DrawLegend( GPSystem* pGPS, int x, int y );
	void			DrawValues( GPSystem* pGPS, int x, int y, int w, int h );
	float			GetValue( int idx ) const;

}; // class GPSeries

/*****************************************************************************/
/*	Class:	GPSeriesMgr
/*	Desc:	Value plots used for diagnostic purposes
/*****************************************************************************/
class GPSeriesMgr
{
	static bool					active;
	static GPSystem*			pGPS;
	static GPSeries				series[c_MaxSeries];
	static int					nSeries;
	static float				wndX, wndY, wndW, wndH;

public:
					GPSeriesMgr();
					~GPSeriesMgr();

	static	void	OnFrame();
	static  void	Init( GPSystem* _pGPS );
	static	void	AddSeries( const char* name, float maxVal );

protected:
	
	static void		DrawFrames();

	friend class	GPSeries;

}; // class GPSeriesMgr

}; // namespace sg

#define REGISTER_SERIES( NAME )	{GPSeriesMgr::AddSeries( #NAME, 1.0f );}
#define INIT_SERIES(A)							{GPSeriesMgr::Init((A));}

#else
#define REGISTER_SERIES( NAME )			{}
#define INIT_SERIES(A)					{}
#endif // _NOSTAT

#endif // __GPSERIESMGR_H__