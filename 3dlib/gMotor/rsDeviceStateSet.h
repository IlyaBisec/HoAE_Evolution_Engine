/*****************************************************************/
/*  File:   rsDeviceStateSet.h
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#ifndef __RSDEVICESTATESET_H__
#define __RSDEVICESTATESET_H__

/*****************************************************************************/
/*	Class:	DSSCache
/*	Desc:	Performs caching of device state blocks
/*****************************************************************************/
class DSSCache
{
	DXDevice*		pDevice;
public:
	DSSCache();
	~DSSCache();

	bool			Init( DXDevice* dev );
	void			Shut();

	_inl bool		SetShader			( int ID );
	
	int				GetShaderID			( const char* name );
	void			Reset				();
	void			ReloadShaders		();

	bool			InvalidateDeviceObjects();
	bool			RestoreDeviceObjects();
	void			SetDevice( DXDevice* pDev ) { pDevice = pDev; }

	void			Dump( FILE* fp );
	void			DumpShader( int shaderID, FILE* fp );

protected:
	int				getShaderID( const char* name, int base );
	HashedShader&	getShader( int idx );

	void			SetInitialState();

private:

	char				root[_MAX_PATH];
	HashedShader*		table[c_MaxZeroLevelShaders][c_MaxZeroLevelShaders];

}; // class DSSCache

#ifdef _INLINES
#include "rsDeviceStateSet.inl"
#endif // _INLINES

#endif // __RSDEVICESTATESET_H__