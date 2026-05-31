/*****************************************************************/
/*  File:   rsShaderCache.h
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#ifndef __RSSHADERCACHE_H__
#define __RSSHADERCACHE_H__

#include "kHash.hpp"

const int c_BadShaderID		= -1;
const int c_DefaultShaderID = 0;
/*****************************************************************************/
/*	Class:	ShaderCache
/*	Desc:	Performs caching of shader render states
/*****************************************************************************/
class ShaderCache
{
	static const int c_HTableSz			= 17389;
	static const int c_MaxShadersNum	= 1024;
	typedef PointerHash<DSBlock, c2::string, c_HTableSz, c_MaxShadersNum> ShadersHash;

public:
						ShaderCache();
						~ShaderCache();

	bool				ApplyShader				( int ID );
	int					GetShaderID				( const char* name, BYTE* shBuf	= NULL, int	size = 0 );
	const char*			GetShaderName			( int shID ) const;

	void				Clear					();
	void				Init					();
	void				ReloadShaders			();
	void				DumpShaders				( const char* dir );


	void				GetCurrentDeviceState	( DSBlock& dsb );
	bool				InvalidateDeviceObjects	();
	void				OnFrame					();	

protected:
	DSBlock*			CreateDefaultShader		();
	DeviceState			CreateState				( int devID, DWORD value );

private:
	DSBlock				defaultDeviceState;
	ShadersHash			reg;
}; // class ShaderCache

const DWORD c_BadVS = 0xFFFFFFFF;
/*****************************************************************************/
/*	Class:	DXVertexShader
/*	Desc:	D3D8 Vertex shader
/*****************************************************************************/
class DXVertexShader
{
	c2::string		m_Name;

	DWORD			m_Handle;

	DWORD			m_Declaration[MAX_FVF_DECL_SIZE];
	bool			m_bSoftwareProcessed;
	bool			m_bDebugCompiled;
	bool			m_bValidation;

	DWORD			m_FVF;

public:
							DXVertexShader	();
							~DXVertexShader	();

	virtual bool			Create			( const char* name );
	virtual bool			Apply			();
	virtual void			Reset			();
	virtual const char*		GetName			() const { return m_Name.c_str(); }
	virtual bool			SetVertexFormat	( VertexFormat vf );
	virtual bool			IsLoaded		() { return m_Handle != c_BadVS; }
	virtual void			SetSoftwareMode ( bool val = true ) { m_bSoftwareProcessed = val; }

protected:
	void			SetName		( const char* name ) { m_Name = name; }
	bool			SetFVF		( DWORD fvf );
}; // class DXVertexShader 

#ifdef _INLINES
#include "rsShaderCache.inl"
#endif // _INLINES

#endif // __RSSHADERCACHE_H__