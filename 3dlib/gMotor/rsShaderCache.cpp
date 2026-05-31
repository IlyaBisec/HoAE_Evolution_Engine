/*****************************************************************/
/*  File:   rsShaderCache.cpp
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#include "stdafx.h"
#include "direct.h"

#include "rsDeviceStates.h"
#include "rsDX.h"
#include "rsShaderCache.h"
#include "rsDSSParser.h"

#ifndef _INLINES
#include "rsShaderCache.inl"
#endif // !_INLINES

/*****************************************************************************/
/*	ShaderCache implementation
/*****************************************************************************/
ShaderCache::ShaderCache()
{
	dss::DSSParser::instance().Init();
	DSBlock* pDefaultShader = CreateDefaultShader();
	reg.add( "default", pDefaultShader );	
}

ShaderCache::~ShaderCache()
{
	Clear();
}

bool ShaderCache::InvalidateDeviceObjects()
{
	for (int i = 0; i < reg.numElem(); i++)
	{
		DWORD stBlockID = reg.elem( i )->GetDevHandle();
		D3DRenderSystem::instance().DeleteStateBlock( stBlockID );
		reg.elem( i )->SetDevHandle( DSBlock::c_BadDevHandle );
	}
	return true;
} // ShaderCache::InvalidateDeviceObjects

void ShaderCache::Init()
{
} // ShaderCache::Init

DSBlock* ShaderCache::CreateDefaultShader()
{
	DSBlock* pBlock = new DSBlock();
	
	pBlock->AddRS( DeviceState( D3DRS_ALPHABLENDENABLE		, TRUE					) );
	pBlock->AddRS( DeviceState( D3DRS_SRCBLEND				, D3DBLEND_SRCALPHA		) );
	pBlock->AddRS( DeviceState( D3DRS_DESTBLEND				, D3DBLEND_INVSRCALPHA	) );
	pBlock->AddRS( DeviceState( D3DRS_ALPHAFUNC				, D3DCMP_GREATER		) );
	pBlock->AddRS( DeviceState( D3DRS_ALPHAREF				, 0x00					) );

	pBlock->AddRS( DeviceState( D3DRS_ALPHATESTENABLE		, FALSE					) );
	pBlock->AddRS( DeviceState( D3DRS_COLORVERTEX			, TRUE					) );
	pBlock->AddRS( DeviceState( D3DRS_SPECULARENABLE		, FALSE					) );
	pBlock->AddRS( DeviceState( D3DRS_NORMALIZENORMALS		, TRUE					) );
	pBlock->AddRS( DeviceState( D3DRS_CULLMODE				, D3DCULL_NONE			) );
	pBlock->AddRS( DeviceState( D3DRS_LIGHTING				, FALSE					) );
	pBlock->AddRS( DeviceState( D3DRS_FILLMODE				, D3DFILL_SOLID			) );
	pBlock->AddRS( DeviceState( D3DRS_SHADEMODE				, D3DSHADE_GOURAUD		) );
	pBlock->AddRS( DeviceState( D3DRS_DITHERENABLE			, FALSE					) );
	pBlock->AddRS( DeviceState( D3DRS_DIFFUSEMATERIALSOURCE	, D3DMCS_COLOR1			) );
	pBlock->AddRS( DeviceState( D3DRS_AMBIENTMATERIALSOURCE	, D3DMCS_MATERIAL		) );
	pBlock->AddRS( DeviceState( D3DRS_SPECULARMATERIALSOURCE, D3DMCS_COLOR1			) );

	pBlock->AddRS( DeviceState( D3DRS_AMBIENT               , 0xFF000000			) );
 
	pBlock->AddRS( DeviceState( D3DRS_STENCILENABLE			, FALSE					) );
	pBlock->AddRS( DeviceState( D3DRS_ZENABLE				, TRUE					) );
	pBlock->AddRS( DeviceState( D3DRS_ZWRITEENABLE			, TRUE					) );
	pBlock->AddRS( DeviceState( D3DRS_ZFUNC					, D3DCMP_LESSEQUAL		) );
	pBlock->AddRS( DeviceState( D3DRS_CLIPPLANEENABLE		, 0						) );
	pBlock->AddRS( DeviceState( D3DRS_COLORWRITEENABLE		, D3DCOLORWRITEENABLE_ALPHA | 
															  D3DCOLORWRITEENABLE_BLUE  | 
															  D3DCOLORWRITEENABLE_GREEN | 
															  D3DCOLORWRITEENABLE_RED ) );

	pBlock->AddTSS( 0, DeviceState( D3DTSS_COLOROP  		, D3DTOP_MODULATE		) );
	pBlock->AddTSS( 0, DeviceState( D3DTSS_COLORARG1		, D3DTA_TEXTURE			) );
	pBlock->AddTSS( 0, DeviceState( D3DTSS_COLORARG2		, D3DTA_DIFFUSE			) );

	pBlock->AddTSS( 0, DeviceState( D3DTSS_ALPHAOP  		, D3DTOP_MODULATE		) );
	pBlock->AddTSS( 0, DeviceState( D3DTSS_ALPHAARG1		, D3DTA_TEXTURE			) );
	pBlock->AddTSS( 0, DeviceState( D3DTSS_ALPHAARG2		, D3DTA_DIFFUSE			) );
	pBlock->AddTSS( 0, DeviceState( D3DTSS_TEXCOORDINDEX	, D3DTSS_TCI_PASSTHRU	) );
	pBlock->AddTSS( 0, DeviceState( D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE	) );

	for (int i = 1; i < c_MaxTextureStages; i++)
	{
		pBlock->AddTSS( i, DeviceState( D3DTSS_COLOROP  	, D3DTOP_DISABLE		) );
		pBlock->AddTSS( i, DeviceState( D3DTSS_ALPHAOP  	, D3DTOP_DISABLE		) );
	}

	pBlock->SetName( "default" );

	return pBlock;
} // ShaderCache::CreateDefaultShader

void ShaderCache::GetCurrentDeviceState( DSBlock& dsb )
{
	dsb.Clear();
	dss::DSSParser& parser = dss::DSSParser::instance();
	
	int		defIdx	 = 0;
	bool	isRS;
	int		devID;
	
	while (parser.GetStateDevID( defIdx++, devID, isRS )) 
	{
		if (isRS) dsb.AddRS( DeviceState( devID, 0 ) );
	}

	for (int i = 0; i < c_MaxTextureStages; i++)
	{
		defIdx = 0;
		while (parser.GetStateDevID( defIdx++, devID, isRS ))
		{
			if (!isRS) dsb.AddTSS( i, DeviceState( devID, 0 ) );
		}
	}
	
	D3DRenderSystem::instance().GetCurrentStateBlock( dsb );
} // ShaderCache::GetCurrentDeviceState

bool ShaderCache::ApplyShader( int shID )
{
	if (shID < 0 || shID >= reg.numElem()) return false;
	DSBlock* pBlock = reg.elem( shID );
	if (!pBlock) return false;

	DWORD devHandle = pBlock->GetDevHandle();
	bool res = false;
	
	if (devHandle != DSBlock::c_BadDevHandle)
	{
		res = D3DRenderSystem::instance().ApplyStateBlock( devHandle );
	}
	else
	{
		pBlock->SetDevHandle( D3DRenderSystem::instance().CreateStateBlock( pBlock ) );
		res = D3DRenderSystem::instance().ApplyStateBlock( pBlock->GetDevHandle() );
	}	

	INC_COUNTER(ShaderChanges,1);
	
	D3DRenderSystem::instance().SetVSMode( false );
	return res;
} // ShaderCache::ApplyShader

const char* ShaderCache::GetShaderName( int shID ) const
{
	if (shID < 0) return "";
	DSBlock* pBlock = reg.elem( shID );
	if (!pBlock) return "";
	return pBlock->GetName();
} // ShaderCache::GetShaderName

int ShaderCache::GetShaderID( const char* name, BYTE* shBuf, int size )
{
	c2::string key( name );
	int id = reg.find( key );
	if (id == NO_ELEMENT)
	{
		DSBlock* pBlock = new DSBlock();
		char fullPath[_MAX_PATH];
		bool loadedOK = false;
		if (!shBuf)
		{
			sprintf( fullPath, "%s\\Shaders\\DeviceStates\\%s.xml", 
				D3DRenderSystem::instance().GetRootDir(), name );
			loadedOK = pBlock->ParseScript( fullPath );
		}
		else
		{
			loadedOK = pBlock->ParseScript( (char*)shBuf, size );
		}

		if (loadedOK)
		{
			pBlock->SetName( name );
			if (reg.numElem() > 0 && pBlock->IsDefaultBase()) pBlock->Append( *reg.elem( 0 ) );
			id = reg.add( key, pBlock );
			return id;
		}
		else
		{
			delete pBlock;
			if (reg.numElem() > 0) Log.Warning( "Could not load shader <%s>", name );
			return c_BadShaderID;
		}
	}
	return id;
} // ShaderCache::GetShaderID

void ShaderCache::Clear()
{
	for (int i = 1; i < reg.numElem(); i++)
	{
		delete reg.elem( i );
	}
} // ShaderCache::Clear

void ShaderCache::OnFrame()
{
}

void ShaderCache::ReloadShaders()
{
	for (int i = 0; i < reg.numElem(); i++)
	{
		DSBlock* pBlock = reg.elem( i );
		if (!pBlock) continue;
		char fullPath[_MAX_PATH];
		sprintf( fullPath, "%s\\Shaders\\DeviceStates\\%s.xml", 
							D3DRenderSystem::instance().GetRootDir(),
							pBlock->GetName() );
		pBlock->Clear();

		D3DRenderSystem::instance().DeleteStateBlock( pBlock->GetDevHandle() );
		pBlock->SetDevHandle( DSBlock::c_BadDevHandle );
		
		if (!pBlock->ParseScript( fullPath ))
		{
			if (i == 0)
			{
				delete pBlock;
				reg.entry( 0 ).el = CreateDefaultShader();
			}
			else
			{
				Log.Error( "Could not load shader <%s>", pBlock->GetName() );
			}
		}
		else
		{
			if (i > 0 && pBlock->IsDefaultBase()) pBlock->Append( *reg.elem( 0 ) );
		}
	}
} // ShaderCache::ReloadShaders

void ShaderCache::DumpShaders( const char* dir )
{
	_mkdir( dir );
	chdir( dir );
	for (int i = 0; i < reg.numElem(); i++)
	{
		DSBlock* pBlock = reg.elem( i );
		if (!pBlock) continue;
		pBlock->SaveToScript( dir );
	}
} // ShaderCache::DumpShaders

/*****************************************************************************/
/*	DXVertexShader implementation
/*****************************************************************************/
DXVertexShader::DXVertexShader() :	
					m_Handle			( c_BadVS	), 
					m_bSoftwareProcessed( false		),  
					m_bDebugCompiled	( true		),
					m_bValidation		( false		)
{
	memset( &m_Declaration, sizeof( m_Declaration ), 0 );
	m_FVF = 0;
}

DXVertexShader::~DXVertexShader()
{
	Reset();
}

bool DXVertexShader::SetVertexFormat( VertexFormat vf )
{
	return SetFVF( FvfDX( vf ) );
} // DXVertexShader::SetVertexFormat

bool DXVertexShader::SetFVF( DWORD fvf )
{
	HRESULT hres = D3DXDeclaratorFromFVF( fvf, m_Declaration );
	m_FVF = fvf;
	if (hres != S_OK) return false;
	return true;
} // DXVertexShader::SetFVF

bool DXVertexShader::Create( const char* name ) 
{
	m_Name = name;
	//  path on the disk
	char path[_MAX_PATH];
	sprintf( path, "%s\\Shaders\\VShaders\\%s",
					D3DRenderSystem::instance().GetRootDir(), GetName() );

	DWORD vsBufLen = 0;
	//BYTE* pVSBuf = LoadFile( path, vsBufLen );
	BYTE* pVSBuf = LoadFile( name, vsBufLen );

	if (!pVSBuf)
	{
		Log.Error( "Could not open vertex shader file: %s", path );
		return false;
	}

	DWORD flags = 0;
	if (m_bDebugCompiled)	flags |= D3DXASM_DEBUG;
	if (!m_bValidation)		flags |= D3DXASM_SKIPVALIDATION;

	//  assemble from file
	DXBuffer* pFunctionBuffer	= NULL;
	DXBuffer* pErrorBuffer		= NULL;
	HRESULT hres = S_FALSE;
    //HRESULT hres = D3DXAssembleShader( pVSBuf, vsBufLen, flags, NULL, &pFunctionBuffer, &pErrorBuffer );

	delete []pVSBuf;
	//  check if assembled OK
	if (hres != D3D_OK)
	{
		if (pErrorBuffer)
		{
			Log.Error( "Could not compile vertex shader: %s. Error: %s", 
				m_Name.c_str(), pErrorBuffer->GetBufferPointer() );
		}
		else
		{
			Log.Error( "Could not compile vertex shader: %s.", m_Name.c_str() );
		}
		SAFE_RELEASE( pFunctionBuffer	);
		SAFE_RELEASE( pErrorBuffer		);
		return false;
	}
	
	//  create vertex shader itself
	DWORD usage = 0;
	if (m_bSoftwareProcessed) usage = D3DUSAGE_SOFTWAREPROCESSING;
	DWORD* pFunction = (DWORD*)pFunctionBuffer->GetBufferPointer();
	hres = D3DRenderSystem::instance().GetDevice()->CreateVertexShader( 
								m_Declaration, pFunction, &m_Handle, usage );
	
	if (hres != D3D_OK)
	{
		if (hres == D3DERR_OUTOFVIDEOMEMORY) 
			Log.Error( "Could not create vertex shader <%s>"
						"due to the lack of video memory.", m_Name.c_str() );
		else if (hres == E_OUTOFMEMORY) 
			Log.Error( "Could not create vertex shader <%s>"
						"due to the lack of system memory.", m_Name.c_str() );
		else Log.Error( "Could not create vertex shader <%s>"
						"- possibly invalid vertex declaration.", m_Name.c_str() );
	}

	SAFE_RELEASE( pFunctionBuffer	);
	SAFE_RELEASE( pErrorBuffer		);
	return (hres == D3D_OK);
} // DXVertexShader::Create

bool DXVertexShader::Apply()
{
	if (m_Handle == c_BadVS) return false;
	DX_CHK( D3DRenderSystem::instance().GetDevice()->SetVertexShader( m_Handle ) );
	D3DRenderSystem::instance().SetVSMode();
	return true;
} // DXVertexShader::Apply

void DXVertexShader::Reset()
{
	if (m_Handle != c_BadVS)
	{
		DX_CHK( D3DRenderSystem::instance().GetDevice()->DeleteVertexShader( m_Handle ) );
		m_Handle = c_BadVS;
	}

} // DXVertexShader::Reset






