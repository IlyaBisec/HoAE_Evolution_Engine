/*****************************************************************************/
/*	File:	GPSeries.cpp
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	15.04.2003
/*****************************************************************************/
#include "stdafx.h"
#include "kHash.hpp"
#include "kResource.h"
#include "gpPackage.h"

#include "gpGN16.h"
#include "gpGU16.h"

#include "mMath2D.h"
#include "gpBitmap.h"

#include "sg.h"
#include "sgGPSystem.h"

#include "gpText.h"
#include "GPSeries.h"

#ifndef _NOSTAT

BEGIN_NAMESPACE(sg)

const DWORD c_SeriesColorsPresets[] = 
{
	0xFF00FF00, 
	0xFFFF3333, 
	0xFF00FFFF,
	0xFFFFF000,
	0xFFFFFFFF,
	0xFFFF8000,
	0xFFFF0080, 
	0xFFFF0080,
	0xFF00E615,
	0xFF00FFE4,
	0xFF646464,
	0xFF3860A2,
	0xFF492E00
}; // c_SeriesColorsPresets

/*****************************************************************************/
/*	GPSeries implementation
/*****************************************************************************/
GPSeries::GPSeries()
{
	val = NULL;
	name[0] = 0;
}

GPSeries::~GPSeries()
{
	delete []val;
}

const char* GPSeries::GetName() const
{
	return name;
}

void GPSeries::Init( const char* _name, DWORD _color, float _maxVal, int _maxLen )
{
	strcpy( name, _name );
	color		= _color;
	maxVal		= _maxVal;
	curPos		= 0;
	len			= 0;
	maxLen		= _maxLen;
	val = new float[maxLen];
} // GPSeries::Init

void GPSeries::PushValue( float value )
{
	val[curPos] = value;
	curPos = (curPos + 1)%maxLen;
	if (len < maxLen - 1) len++;
	if (value > maxVal) maxVal = value;
} // GPseries::PushValue

void GPSeries::DrawLegend( GPSystem* pGPS, int x, int y )
{
	char buf[c_MaxSeriesNameLen + 16];
	sprintf( buf, "%s (%.0f/%.0f)", name, GetValue( curPos - 1 ), maxVal );
	assert( pGPS );
	//pGPS->DrawString( buf, x, y, 0.0f, color );
} // GPSeries::DrawLegend

void GPSeries::DrawValues( GPSystem* pGPS, int x, int y, int w, int h )
{
	assert( pGPS );
	if (len == 0) return;
	float cX	= x;
	float nVal	= GetValue( 0 );
	float ratio = 1.0f - nVal/float( maxVal ); 
	float cY	= y + h*ratio;
	float cY1	= cY;
	float step  = w / float( maxLen ); 
	for (int i = 0; i < len; i++)
	{
		nVal = GetValue( i );
		ratio = 1.0f - nVal/float( maxVal ); 
		cY1	= y + h*ratio;
		pGPS->DrawLine( cX, cY, cX + step, cY1, color );
		cY = cY1;
		cX += step;
	}
} // GPSeries::DrawValues

float	GPSeries::GetValue( int idx ) const
{
	int ridx = curPos - len + idx;
	if (ridx < 0) ridx += maxLen;
	return val[ridx%maxLen]; 
} // GPSeries::GetValue

/*****************************************************************************/
/*	GPSeriesMgr implementation
/*****************************************************************************/
bool			GPSeriesMgr::active	= false;
GPSystem*		GPSeriesMgr::pGPS		= NULL;
int				GPSeriesMgr::nSeries	= 0;
float			GPSeriesMgr::wndX		= c_DefSeriesWndX;
float			GPSeriesMgr::wndY		= c_DefSeriesWndY;
float			GPSeriesMgr::wndW		= c_DefSeriesWndW;
float			GPSeriesMgr::wndH		= c_DefSeriesWndH;
GPSeries		GPSeriesMgr::series[c_MaxSeries];

GPSeriesMgr::GPSeriesMgr()
{
}

GPSeriesMgr::~GPSeriesMgr()
{
}

void GPSeriesMgr::OnFrame()
{
	/*
	if (GetKeyState( VK_RSHIFT	) < 0 && 
		GetKeyState( VK_LSHIFT	) < 0 &&
		GetKeyState( VK_RCONTROL) < 0 && 
		GetKeyState( VK_LCONTROL) < 0)
	*/
	if (GetKeyState( 'S' ) < 0 && 
		GetKeyState( 'T' ) < 0 &&
		GetKeyState( 'A' ) < 0)
	{
		static DWORD lastTime = 0;
		if (GetTickCount() - lastTime > 500)
		{
			active = !active;
			lastTime = GetTickCount();
		}
	}

	for (int i = 0; i < nSeries; i++)
	{
		series[i].PushValue( GET_COUNTER_STR(series[i].GetName()) );
	}

	if (!active || !pGPS) return;

	float cY = wndY + 10;
	float cX = wndX + 10;
	DrawFrames();
	for (int i = 0; i < nSeries; i++)
	{
		series[i].DrawLegend( pGPS, cX, cY );
		series[i].DrawValues( pGPS, cX + c_LegendLineWidth - 8, wndY + 6, 
									wndW - c_LegendLineWidth - 6, wndH - 12 );
		cY += c_LegendLineHeight;
	}

	pGPS->FlushBatches();

} // GPSeriesMgr::OnFrame

void GPSeriesMgr::AddSeries( const char* name, float maxVal )
{
	assert( nSeries < c_MaxSeries );
	DWORD color = c_SeriesColorsPresets[nSeries];
	series[nSeries++].Init( name, color, maxVal, c_SeriesLen );
} // GPSeriesMgr::AddSeries

void GPSeriesMgr::DrawFrames()
{
	assert( pGPS );
	pGPS->DrawFillRect( wndX, wndY, wndW, wndH, 0x332222FF );
	pGPS->DrawRect( wndX, wndY, wndW, wndH, c_SeriesFrameColor );
	pGPS->DrawRect( wndX + 3, wndY + 3, c_LegendLineWidth - 8, wndH - 8, c_LegendFrameColor );
	pGPS->DrawRect( wndX + c_LegendLineWidth, wndY + 3, 
					wndW - c_LegendLineWidth - 5, wndH - 8, c_LegendFrameColor );

} // GPSeriesMgr::DrawFrames

void GPSeriesMgr::Init( GPSystem* _pGPS )
{
	pGPS = _pGPS;
}

END_NAMESPACE(sg)


#endif  // _NOSTAT
