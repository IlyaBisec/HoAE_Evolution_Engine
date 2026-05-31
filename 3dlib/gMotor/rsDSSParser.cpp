/*****************************************************************************/
/*	File:	rsDSSParser.h
/*	Desc:	Parser of the DSS files
/*	Author:	Ruslan Shestopalyuk
/*	Date:	03.04.2003
/*****************************************************************************/
#include "stdafx.h"
#include "rsDeviceStates.h"
#include "kHash.hpp"
#include "rsDSSParser.h"

BEGIN_NAMESPACE(dss)
/*****************************************************************************/
/*	DSSParser implementation
/*****************************************************************************/
DSSParser::DSSParser()
{
	Reset();
}

DSSParser::~DSSParser()
{
	for (int i = 0; i < m_StateDesc.numElem(); i++) delete m_StateDesc.elem( i );
	m_StateDesc.reset();
}

void DSSParser::Reset()
{
	m_pDSS			= 0;
	m_CurTexStage	= 0;
	m_State			= psIdle;
}

bool DSSParser::ParseFile( const char* fname, DSBlock& dss )
{
	m_pDSS = &dss;
	bool allOK = XMLParser::ParseFile( fname );
	m_pDSS = NULL;
	return allOK;
}

int	DSSParser::StateFromDevID( int devID )
{
	Init();
	for (int i = 0; i < m_StateDesc.numElem(); i++)
	{
		DSDefinition* pDSDef = m_StateDesc.elem( i );
		if (pDSDef && pDSDef->GetStateDevID() == devID) return i;
	}
	return -1;
} // DSSParser::StateFromDevID

bool DSSParser::GetStateDevID( int defIdx, int& devID, bool& isRS )
{
	if (defIdx < 0 || defIdx >= m_StateDesc.numElem()) return false;
	devID = m_StateDesc.elem( defIdx )->GetStateDevID();
	isRS  = m_StateDesc.elem( defIdx )->IsRS();
	return true;
} // DSSParser::GetStateDevID

bool DSSParser::ParseBuffer( char* buffer, DSBlock& dss )
{
	m_pDSS = &dss;
	m_State = psIdle;
	bool allOK = XMLParser::ParseBuffer( buffer );
	m_pDSS = NULL;
	return allOK;
}

void DSSParser::OnOpen( const char* tag )
{
	assert( m_pDSS );
	if (m_State == psInsideDSS)
	{
		if (!strcmp( tag, "RenderState" ))
		{
			m_State = psParsingRS;
		}
		else if (!strcmp( tag, "TextureStageState" ))
		{
			m_State = psParsingTSS;
		}
	}
	else if (m_State == psIdle)
	{
		m_State = psInsideDSS;
		m_pDSS->SetName( tag );
	}
	else
	{
		m_CurTag = tag;
		m_CurTag.toLower();
	}
} // DSSParser::OnOpen

const char*	DSSParser::GetStateName( const DeviceState& ds, bool isRS ) 
{
	for (int i = 0; i < m_StateDesc.numElem(); i++)
	{
		const DSDefinition* pDef = m_StateDesc.elem( i );
		if (pDef->GetStateDevID() == ds.devID &&
			pDef->IsRS() == isRS) return pDef->GetStateName();
	}
	return "Unknown";
} // DSSParser::GetStateName

const char* DSSParser::GetStateValue( const DeviceState& ds, bool isRS ) 
{
	for (int i = 0; i < m_StateDesc.numElem(); i++)
	{
		const DSDefinition* pDef = m_StateDesc.elem( i );
		if (pDef->GetStateDevID() == ds.devID &&
			pDef->IsRS() == isRS) 
		{
			return pDef->ValToStr( ds.value );
		}
	}
	return "Unknown";
} // DSSParser::GetStateValue

void DSSParser::OnClose( const char* tag )
{
	assert( m_pDSS );
	if (!strnicmp( tag, "RenderState", strlen( "RenderState" ) ))
	{
		if (m_State != psParsingRS)
		{
			Log.Error( "Unexpected closing tag: </RenderState> in file %s", m_pDSS->GetName() );
		}
		m_State = psInsideDSS;
	}
	else if (!strnicmp( tag, "TextureStageState", strlen( "TextureStageState" ) ))
	{
		if (m_State != psParsingTSS)
		{
			Log.Error( "Unexpected closing tag: </TextureStageState> in file %s", m_pDSS->GetName() );
		}
		m_State = psInsideDSS;
	}
	else if (!strnicmp( tag, m_CurTag.c_str(), strlen( m_CurTag.c_str() ) ))
	{
		AddDeviceState();
	}
	else if (m_pDSS->GetName() && !stricmp( tag, m_pDSS->GetName() ))
	{
		m_State = psIdle;
	}
	else
	{
		Log.Error( "Unexpected closing tag: </%s> in file %s", tag, m_pDSS->GetName() );
	}
} // DSSParser::OnClose

void DSSParser::AddDeviceState()
{
	int defHandle = m_StateDesc.find( m_CurTag.c_str() );
	if (defHandle == NO_ELEMENT) 
	{
		if (m_pDSS->GetName()) Log.Warning( "Unknown device state: %s in file %s", m_CurTag.c_str(), m_pDSS->GetName() );
	}
	else
	{
		DeviceState ds;
		DSDefinition* stDef = m_StateDesc.elem( defHandle );
		ds.devID	= stDef->GetStateDevID();
		bool res = stDef->ParseValue( m_CurValue, ds );
		
		if (!res)
		{
			Log.Error( "Error in device state block: '%s'. Unknown value '%s' in device state '%s'.", 
						m_pDSS->GetName(), m_CurValue.c_str(), m_CurTag.c_str() );
			return;
		}

		if (m_State == psParsingRS)
		{
			m_pDSS->AddRS( ds );
		}
		else if (m_State == psParsingTSS)
		{
			m_pDSS->AddTSS( m_CurTexStage,  ds );
		}
		else
		{
			assert( false );
		}
	}
} // DSSParser::AddDeviceState

void DSSParser::OnValue( const char* value )
{
	m_CurValue = value;
	m_CurValue.toLower();
}

void DSSParser::OnAttribute( const char* name, const char* value )
{
	assert( m_pDSS );
	if (!strnicmp( name, "stage", 5 ))
	{
		if (sscanf( value, "%d", &m_CurTexStage ) != 1)
		{
			Log.Error( "Wrong texture stage number: %s. Location %s", name, GetCurLocation() );
		}
		return;
	}

	if (!strnicmp( name, "defaultbase", 11 ))
	{
		bool bDefBase = true;
		
		if (!strnicmp( value, "true", 4 )) bDefBase = true;
		else if (!strnicmp( value, "false", 5 )) bDefBase = false;
		else Log.Error( "Wrong DefaultBase value: %s. Location %s", name, GetCurLocation() );
		m_pDSS->SetIsDefaultBase( bDefBase );
		return;
	}

	Log.Error( "Unknown attribute: %s. Location %s", name, GetCurLocation() );
} // DSSParser::OnAttribute

void DSSParser::SpewError( const char* errMsg )
{
	Log.Error( errMsg );
}

void DSSParser::AddStateDef( DSDefinition* pDS )
{
	assert( pDS );
	c2::string stName( pDS->GetStateName() );
	stName.toLower();
	m_StateDesc.add( stName, pDS );
}

void DSSParser::Init()
{
	if (m_bInited) return;
	m_bInited = true;

	using namespace dss;
	m_StateDesc.reset();

	//  z-buffer
	REG_DS(DSSZEnable);
	REG_DS(DSSZWriteEnable);
	REG_DS(DSSZFunc);
	REG_DS(DSSZBias);

	//  alpha
	REG_DS(DSSAlphaTestEnable);
	REG_DS(DSSAlphaBlendEnable);
	REG_DS(DSSSrcBlend);
	REG_DS(DSSDestBlend);
	REG_DS(DSSAlphaRef);
	REG_DS(DSSAlphaFunc);
	REG_DS(DSSSrcBlendAlpha);
	REG_DS(DSSDestBlendAlpha);
	REG_DS(DSSBlendOpAlpha);
	REG_DS(DSSBlendop);
	REG_DS(DSSBlendFactor);
	REG_DS(DSSSeparateAlphaBlendEnable);

	//  shading
	REG_DS(DSSFillMode);
	REG_DS(DSSTextureFactor);
	REG_DS(DSSShadeMode);
	REG_DS(DSSDitherEnable);
	REG_DS(DSSColorWriteEnable);
	
	//  lighting
	REG_DS(DSSLighting);
	REG_DS(DSSColorVertex);
	REG_DS(DSSSpecularEnable);
	REG_DS(DSSAmbient);
	REG_DS(DSSNormalizeNormals);
	REG_DS(DSSDiffuseMaterialSource);
	REG_DS(DSSSpecularMaterialSource);
	REG_DS(DSSAmbientMaterialSource);
	REG_DS(DSSEmissiveMaterialSource);

	// fog
	REG_DS(DSSFogEnable);
	REG_DS(DSSFogColor);
	REG_DS(DSSFogTableMode);
	REG_DS(DSSFogStart);
	REG_DS(DSSFogEnd);
	REG_DS(DSSFogDensity);
	REG_DS(DSSRangeFogEnable);
	REG_DS(DSSFogVertexMode);

	
	// stencil
	REG_DS(DSSStencilEnable);
	REG_DS(DSSStencilFail);
	REG_DS(DSSStencilZFail);
	REG_DS(DSSStencilPass);
	REG_DS(DSSStencilFunc);
	REG_DS(DSSStencilRef);
	REG_DS(DSSStencilMask);
	REG_DS(DSSStencilWriteMask);
	REG_DS(DSSCCW_StencilFail);
	REG_DS(DSSCCW_StencilZFail);
	REG_DS(DSSCCW_StencilPass);
	REG_DS(DSSCCW_StencilFunc);
	REG_DS(DSSTwoSidedStencilMode);

	// misc
	REG_DS(DSSLastPixel);
	REG_DS(DSSCullMode);
	REG_DS(DSSWrap0);
	REG_DS(DSSWrap1);
	REG_DS(DSSWrap2);
	REG_DS(DSSWrap3);
	REG_DS(DSSWrap4);
	REG_DS(DSSWrap5);
	REG_DS(DSSWrap6);
	REG_DS(DSSWrap7);
	REG_DS(DSSClipping);
	REG_DS(DSSLocalViewer);

	REG_DS(DSSVertexBlend);
	REG_DS(DSSClipPlaneEnable);
	REG_DS(DSSPointSize);
	REG_DS(DSSPointSize_min);
	REG_DS(DSSPointSpriteEnable);
	REG_DS(DSSPointScaleEnable);
	REG_DS(DSSPointScale_a);
	REG_DS(DSSPointScale_b);
	REG_DS(DSSPointScale_c);
	REG_DS(DSSMultisampleAntialias);
	REG_DS(DSSMultisampleMask);
	REG_DS(DSSPatchEdgeStyle);
	REG_DS(DSSDebugmonitorToken);
	REG_DS(DSSPointSize_max);
	REG_DS(DSSIndexedVertexBlendEnable);
	REG_DS(DSSTweenFactor);
	REG_DS(DSSPositionDegree);
	REG_DS(DSSNormalDegree);
	REG_DS(DSSScissorTestEnable);
	REG_DS(DSSSlopeScaleDepthBias);
	REG_DS(DSSAntialiasedLineEnable);
	REG_DS(DSSMinTessellationLevel);
	REG_DS(DSSMaxTessellationLevel);
	REG_DS(DSSAdaptiveTess_x);
	REG_DS(DSSAdaptiveTess_y);
	REG_DS(DSSAdaptiveTess_z);
	REG_DS(DSSAdaptiveTess_w);
	REG_DS(DSSEnableAdaptiveTesselation);

	REG_DS(DSSColorWriteEnable1);
	REG_DS(DSSColorWriteEnable2);
	REG_DS(DSSColorWriteEnable3);
	REG_DS(DSSSRGBWriteEnable);
	REG_DS(DSSDepthBias);
	REG_DS(DSSWrap8);
	REG_DS(DSSWrap9);
	REG_DS(DSSWrap10);
	REG_DS(DSSWrap11);
	REG_DS(DSSWrap12);
	REG_DS(DSSWrap13);
	REG_DS(DSSWrap14);
	REG_DS(DSSWrap15);

	//  tss
	REG_DS(DSSColorOp);
	REG_DS(DSSColorArg1);
	REG_DS(DSSColorArg2);
	REG_DS(DSSAlphaOp);
	REG_DS(DSSAlphaArg1);
	REG_DS(DSSAlphaArg2);
	REG_DS(DSSBumpEnvMat00);
	REG_DS(DSSBumpEnvMat01);
	REG_DS(DSSBumpEnvMat10);
	REG_DS(DSSBumpEnvMat11);
	REG_DS(DSSBumpEnvlScale);
	REG_DS(DSSBumpEnvlOffset);
	REG_DS(DSSTextureTransformFlags);
	REG_DS(DSSTexCoordIndex);
	REG_DS(DSSColorArg0);
	REG_DS(DSSAlphaArg0);
	REG_DS(DSSResultArg);
	REG_DS(DSSConstant);
	REG_DS(DSSMipFilter);
	REG_DS(DSSMinFilter);
	REG_DS(DSSMagFilter);
	REG_DS(DSSAddressU);
	REG_DS(DSSAddressV);
} // DSSParser::OnInit

/*****************************************************************************/
/*	RSDefinition implementation
/*****************************************************************************/
void RSDefinition::Apply( const DeviceState& ds )
{

}

/*****************************************************************************/
/*	TSSDefinition implementation
/*****************************************************************************/
void TSSDefinition::Apply( const DeviceState& ds )
{

}

END_NAMESPACE(dss)



