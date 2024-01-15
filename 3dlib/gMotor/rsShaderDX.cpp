/*****************************************************************/
/*  File:   rsShaderDX.cpp
/*  Desc:   
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#include "stdafx.h"
#include "direct.h"
#include "rsDX.h"

#ifndef _INLINES
#include "rsShaderDX.inl"
#endif // !_INLINES

const char* tBool[] = 
{
	"False", 
	"True",
	""
};

const char* tZEnable[] = 
{
	"False", 
	"True",
	"UseW",
	""
};

const char* tFillMode[] = 
{
	"Unknown",
	"Point", 
	"Wireframe",
	"Solid",
	""
};

const char* tShadeMode[] = 
{
	"Unknown",
	"Flat", 
	"Gouraud",
	"Phong",
	""
};

const char* tBlend[]=
{
	"Unknown",
	"Zero",
	"One",
	"SrcColor",
	"InvSrcColor",
	"SrcAlpha",
	"InvSrcAlpha",
	"DestAlpha",
	"InvDestAlpha",
	"DestColor",
	"InvDestColor"
	"SrcAlphaSat",
	"BothSracAlpha",
    "BothInvSrcAlpha",
	""
};

const char* tCull[]=
{
	"Unknown",
	"None",
	"CW",
	"CCW",
	""
};

const char* tCmpFunc[]=
{
	"Unknown",
	"Never",
	"Less",
	"Equal",
	"LessEqual",
	"Greater",
	"NotEqual",
	"GreaterEqual",
	"Always",
	""
};   

const char* tFogMode[]=
{
	"Unknown",
	"None",
	"Exp",
	"Exp2",
	"Linear",
	""
};

const char* tStencilOp[]=
{
	"Unknown",
	"Keep",
	"Zero",
	"Replace",
	"IncrSat",
	"DecrSat",
	"Invert",
	"Incr",
	"Decr",
	""
};

const char* tMaterialColorSource[]=
{
	"Material",
	"Color1",
	"Color2",
	""
};

const char* tVertexBlendFlags[]=
{
	"Disable",
	"1Weights",
	"2Weights",
	"3Weights",
	"","","","","","","","","","","","","","","","","","","","","","","","","",
	"","","","","","","","","","","","","","","","","","","","","","","","","",
	"","","","","","","","","","","","","","","","","","","","","","","","","",
	"","","","","","","","","","","","","","","","","","","","","","","","","",
	"","","","","","","","","","","","","","","","","","","","","","","","","",
	"","","","","","","","","","","","","","","","","","","","","","","","","",
	"","","","","","","","","","","","","","","","","","","","","","","","","",
	"","","","","","","","","","","","","","","","","","","","","","","","","",
	"","","","","","","","","","","","","","","","","","","","","","","","","",
	"","","","","","","","","","","","","","","","","","","","","","","","","",
	"",
	"Tweening",
	"0Weights",
	""
};

const char* tPatchEdgeStyle[]=
{
	"Discrete",
	"Continuous",
	""
};

const char* tDebugMonitorTokens[]=
{
	"Enable",
	"Disable",
	""
};

const char* tBlendOp[]=
{
	"Unknown",
	"Add",
	"Subtract",
	"RevSubtract",
	"Min",
	"Max",
	""
};

const char* tTextureOp[]=
{
	"Unknown",
	"Disable",
	"SelectArg1",
	"SelectArg2",
	"Modulate",
	"Modulate2x",
	"Modulate4x",
	"Add",
	"AddSigned",
	"AddSigned2x",
	"Subtract",
    "AddSmooth",
	"BlendDiffuseAlpha",
	"BlendTextureAlpha",
	"BlendFactorAlpha",
	"BlendTextureAlphaPM",
	"BlendCurrentAlpha",
	"PreModulate",
	"ModulateAlpha_AddColor",
	"ModulateColor_AddAlpha",
	"ModulateInvAlpha_AddColor",
	"ModulateInvColor_AddAlpha",
	"BumpEnvMap",
	"BumpEnvMapLuminance",
    "DotProduct3",
	"MultiplyAdd",
	"Lerp",
	""
};

const char* tTextureArg[]=
{
	"Diffuse", 
	"Current",
	"Texture",
	"Tfactor",
	"Specular",
	""
};

const char* tTextureAddress[]=
{
	"Unknown",
	"Wrap",
	"Mirror",
	"Clamp",
	"Border",
	"MirrorOnce",
	""
};

const char* tTextureFilterType[]=
{
	"None",
	"Point",
	"Linear",
	"Anisotropic",
	"FlatCubic",
	"GaussianCubic",
	""
};

const char* tTextureTransformFlags[]=
{
	"Disable",
	"Count1",
	"Count2",
	"Count3",
	"Count4",
	"","","","","","","","","","","","","","","","","","","","","","","","","",
	"","","","","","","","","","","","","","","","","","","","","","","","","",
	"","","","","","","","","","","","","","","","","","","","","","","","","",
	"","","","","","","","","","","","","","","","","","","","","","","","","",
	"","","","","","","","","","","","","","","","","","","","","","","","","",
	"","","","","","","","","","","","","","","","","","","","","","","","","",
	"","","","","","","","","","","","","","","","","","","","","","","","","",
	"","","","","","","","","","","","","","","","","","","","","","","","","",
	"","","","","","","","","","","","","","","","","","","","","","","","","",
	"","","","","","","","","","","","","","","","","","","","","","","","","",
	"",
	"Projected",
	""
};

const char* tLightType[]=
{
	"Unknown",
	"Point",
	"Spot",
	"Directional",
	""
};

/*****************************************************************************/
/*	Shader implementation
/*****************************************************************************/
ShaderStateDesc	Shader::stateDesc[c_MaxStates];
DWORD			Shader::nStateDesc = 0;
const char		Shader::errstr[] = "ERROR";
double			Shader::perfCounterRes;					

	

Shader::Shader() :	stateBlockID(NO_STATE_BLOCK), execTime(0.0), 
					nStates(0), baseShader(0), id(0), nameLen(0) 
{
	//  static table initialization
	if (nStateDesc == 0) initStatesTable();
	
	name[0]			= 0;
	correctOrder	= true;
	vshID			= 0;	
	iEffect			= NULL;
} // Shader constructor

Shader::Shader( const char* shName, int baseID )
{
	//  static table initialization
	if (nStateDesc == 0) initStatesTable();

	nameLen = strlen( shName );
	assert( nameLen < c_MaxShaderIDLen );
	strcpy( name, shName );
	baseShader		= baseID;
	stateBlockID	= NO_STATE_BLOCK;
	nStates			= 0;
	id				= 0;
	vshID			= 0;	
	iEffect			= NULL;
	execTime		= 0.0;
} // Shader::Shader

const char*	Shader::getStateName( const ShaderState& state )
{
	if (state.tableIdx < 0 || state.tableIdx >= nStateDesc) return errstr;
	return stateDesc[state.tableIdx].getName();
} // Shader::getStateName

int	Shader::getStateDescrTableIdx( const char* state )
{
	for (DWORD i = 0; i < nStateDesc; i++)
	{
		if (stateDesc[i].hasName( state )) return i;
	}
	return -1;
} // Shader::getStateDescrTableIdx

bool Shader::getValStr( const ShaderState& state, char* str, int maxlen )
{
	if (state.tableIdx < 0 || state.tableIdx >= nStateDesc) return false;
	return stateDesc[state.tableIdx].getValStr( state.value, str, maxlen );
}

bool Shader::getRenderState( DXDevice* dev, const char* stName )
{
	if (!dev) return false;
	DWORD val;
	ShaderStateDesc& desc = stateDesc[getStateDescrTableIdx( stName )];
	D3DRENDERSTATETYPE state = (D3DRENDERSTATETYPE) desc.stateDevID;
	HRESULT res = dev->GetRenderState( state, &val );
	if (res != S_OK) return false;
	ShaderState* shState = getCreateState( stName );
	assert( shState );
	shState->setValue( val );
	return true;
} // Shader::getRenderState

bool Shader::getTextureStageState(	DXDevice* dev, int stage, 
									const char* stName )
{
	if (!dev) return false;
	DWORD val;
	char fullName[128];
	sprintf( fullName, "%s[%d]", stName, stage );
	int idx = getStateDescrTableIdx( fullName );
	assert( idx != -1 );
	ShaderStateDesc& desc = stateDesc[idx];
	D3DTEXTURESTAGESTATETYPE state = (D3DTEXTURESTAGESTATETYPE)desc.stateDevID; 
	assert( stage >= 0 && stage < c_MaxShaderTextureStages );
	HRESULT res = dev->GetTextureStageState( stage, state, &val );
	if (res != S_OK) return false;
	ShaderState* shState = getCreateState( fullName );
	shState->setValue( val, stage );
	return true;
} // Shader::getTextureStageState

bool Shader::getLightField( const char* fieldName, int lightIdx, void* dataPtr )
{
	char fullName[128];
	sprintf( fullName, "Light%s[%d]", fieldName, lightIdx );
	int idx = getStateDescrTableIdx( fullName );
	assert( idx != -1 );
	ShaderState* shState = getCreateState( fullName );
	shState->setValueFromDataPtr( dataPtr );
	shState->stage = lightIdx;
	return true;
} // Shader::getLightField

bool Shader::getMaterialField( const char* fieldName, void* dataPtr )
{
	char fullName[128];
	sprintf( fullName, "Material%s", fieldName );
	int idx = getStateDescrTableIdx( fullName );
	assert( idx != -1 );
	ShaderState* shState = getCreateState( fullName );
	shState->setValueFromDataPtr( dataPtr );
	return true;
} // Shader::getMaterialField

void Shader::InvalidateDeviceObjects( DXDevice* pDevice )
{
	assert( pDevice );
	if (stateBlockID != NO_STATE_BLOCK) 
	{
		pDevice->DeleteStateBlock( stateBlockID );
		stateBlockID = NO_STATE_BLOCK;
	}
	SAFE_RELEASE( iEffect );
} // Shader::InvalidateDeviceObjects

bool Shader::CreateFromCurrentDeviceState( DXDevice* pDevice )
{	
	if (!pDevice) return false;
	//  vertex shader ID
	DX_CHK( pDevice->GetVertexShader( &vshID ) );

	getRenderState( pDevice, "ZEnable"					);
	getRenderState( pDevice, "ZWriteEnable"				);
	getRenderState( pDevice, "ZFunc" 					);
	getRenderState( pDevice, "ZBias" 					);
	getRenderState( pDevice, "FillMode"					);
	getRenderState( pDevice, "ShadeMode"				);
	getRenderState( pDevice, "LinePattern"				);
	getRenderState( pDevice, "SrcBlend" 				);
	getRenderState( pDevice, "DestBlend" 				);
	getRenderState( pDevice, "AlphaTestEnable"			);
	getRenderState( pDevice, "AlphaRef" 				);
	getRenderState( pDevice, "AlphaFunc" 				);
	getRenderState( pDevice, "AlphaBlendEnable"			);
	getRenderState( pDevice, "LastPixel" 				);
	getRenderState( pDevice, "CullMode" 				);
	getRenderState( pDevice, "DitherEnable" 			);
	getRenderState( pDevice, "SpecularEnable"			);
	getRenderState( pDevice, "ZVisible" 				);
	getRenderState( pDevice, "FogEnable" 				);
	getRenderState( pDevice, "FogColor" 				);
	getRenderState( pDevice, "FogTableMode" 			);
	getRenderState( pDevice, "FogStart" 				);
	getRenderState( pDevice, "FogEnd" 					);
	getRenderState( pDevice, "FogDensity" 				);
	getRenderState( pDevice, "EdgeAntialias" 			);
	getRenderState( pDevice, "RangeFogEnable"			);
	getRenderState( pDevice, "StencilEnable" 			);
	getRenderState( pDevice, "StencilFail" 				);
	getRenderState( pDevice, "StencilZFail"				);
	getRenderState( pDevice, "StencilPass" 				);
	getRenderState( pDevice, "StencilFunc" 				);
	getRenderState( pDevice, "StencilRef" 				);
	getRenderState( pDevice, "StencilMask" 				);
	getRenderState( pDevice, "StencilWriteMask"			);
	getRenderState( pDevice, "ColorWriteEnable" 		);
	getRenderState( pDevice, "TextureFactor" 			);
	getRenderState( pDevice, "Lighting" 				);
	getRenderState( pDevice, "NormalizeNormals"			);
	getRenderState( pDevice, "AmbientMaterialSource"	);
	getRenderState( pDevice, "DiffuseMaterialSource"	);
	getRenderState( pDevice, "SpecularMaterialSource"	);
	getRenderState( pDevice, "EmmisiveMaterialSource"	);
	getRenderState( pDevice, "Ambient"					);
	getRenderState( pDevice, "Wrap0"					);
	getRenderState( pDevice, "Wrap1"					);
	getRenderState( pDevice, "Wrap2"					);
	getRenderState( pDevice, "Wrap3"					);
	getRenderState( pDevice, "Wrap4"					);
	getRenderState( pDevice, "Wrap5"					);
	getRenderState( pDevice, "Wrap6"					);
	getRenderState( pDevice, "Wrap7"					);
	getRenderState( pDevice, "Clipping"					);
	getRenderState( pDevice, "FogVertexMode"			);
	getRenderState( pDevice, "ColorVertex"				);
	getRenderState( pDevice, "LocalViewer"				);
	getRenderState( pDevice, "VertexBlend"				);
	getRenderState( pDevice, "ClipPlaneEnable"			);
	getRenderState( pDevice, "SoftwareVertexProcessing"	);
	getRenderState( pDevice, "PointSize"				);
	getRenderState( pDevice, "PointSize_Min"			);
	getRenderState( pDevice, "PointSpriteEnable"		);
	getRenderState( pDevice, "PointScaleEnable"			);
	getRenderState( pDevice, "PointScale_A"				);
	getRenderState( pDevice, "PointScale_B"				);
	getRenderState( pDevice, "PointScale_C"				);
	getRenderState( pDevice, "MultiSamleAntialias"		);
	getRenderState( pDevice, "MultiSampleMask"			);
	getRenderState( pDevice, "PatchEdgeStyle"			);
	getRenderState( pDevice, "PatchSegments"			);
	getRenderState( pDevice, "PointSize_Max"			);
	getRenderState( pDevice, "IndexedVertexBlendEnable"	);
	getRenderState( pDevice, "TweenFactor"				);
	getRenderState( pDevice, "BlendOp"					);
	getRenderState( pDevice, "PositionOrder"			);
	getRenderState( pDevice, "NormalOrder"				);

	for (int i = 0; i < c_MaxShaderTextureStages; i++)
	{
		getTextureStageState( pDevice, i, "ColorOp"			);
		getTextureStageState( pDevice, i, "ColorArg1"		);
		getTextureStageState( pDevice, i, "ColorArg2"		);
		getTextureStageState( pDevice, i, "AlphaOp"			);
		getTextureStageState( pDevice, i, "AlphaArg1"		);
		getTextureStageState( pDevice, i, "AlphaArg2"		);
		getTextureStageState( pDevice, i, "BumpEnvMat00"	);
		getTextureStageState( pDevice, i, "BumpEnvMat01"	);
		getTextureStageState( pDevice, i, "BumpEnvMat10"	);
		getTextureStageState( pDevice, i, "BumpEnvMat11"	);
		getTextureStageState( pDevice, i, "TexCoordIndex"	);
		getTextureStageState( pDevice, i, "AddressU"		);
		getTextureStageState( pDevice, i, "AddressV"		);
        getTextureStageState( pDevice, i, "BorderColor"		);
        getTextureStageState( pDevice, i, "MagFilter"		);
        getTextureStageState( pDevice, i, "MinFilter"		);
        getTextureStageState( pDevice, i, "MipFilter"		);
        getTextureStageState( pDevice, i, "MipmapLodBias"	);
        getTextureStageState( pDevice, i, "MaxMipLevel"		);
        getTextureStageState( pDevice, i, "MaxAnisotropy"	);
        getTextureStageState( pDevice, i, "BumpEnvlScale"	);
        getTextureStageState( pDevice, i, "BumpEnvlOffset"	);
        getTextureStageState( pDevice, i, "TextureTransformFlags" );
        getTextureStageState( pDevice, i, "AddresW"				);
        getTextureStageState( pDevice, i, "ColorArg0"			);
        getTextureStageState( pDevice, i, "AlphaArg0"			);
        getTextureStageState( pDevice, i, "ResultArg"			);
	}

	BOOL Lighting = FALSE;
	if (pDevice->GetRenderState( D3DRS_LIGHTING, (DWORD*)&Lighting ) != S_OK)
	{
		return false;
	}
	if (!Lighting) return true;
	
	/*
	for (i = 0; i < c_MaxLights; i++)
	{
		BOOL Enable = FALSE;

		pDevice->GetLightEnable( i, &Enable );
		if (Enable != 0) 
		{
			Enable = 1;
		}
		getLightField( "Enable",	i, &Enable );
	}
	
	//  material properties
	DXMaterial	material;
	pDevice->GetMaterial( &material );
	
	getMaterialField( "Diffuse",	&(material.Diffuse)	 );
	getMaterialField( "Ambient",	&(material.Ambient)  );
	getMaterialField( "Specular",	&(material.Specular) );
	getMaterialField( "Emissive",	&(material.Emissive) );
	getMaterialField( "Power",		&(material.Power)	 );

	//  light states
	DXLight light;
	memset( &light, 0, sizeof( light ) );
	
	for (i = 0; i < c_MaxLights; i++)
	{
		BOOL Enable = FALSE;
		
		pDevice->GetLightEnable( i, &Enable );
		if (Enable != 0) 
		{
			Enable = 1;
		}
		getLightField( "Enable",	i, &Enable );
		
		if (!Enable) continue;
		
		if (pDevice->GetLight( i, &light ) != S_OK)
		{
			continue;
		}
		
		getLightField( "Type",			i, &light.Type			);
		getLightField( "Diffuse",		i, &light.Diffuse		);
		getLightField( "Specular",		i, &light.Specular		);
		getLightField( "Ambient",		i, &light.Ambient		);
		getLightField( "Position",		i, &light.Position		);	
		getLightField( "Direction",		i, &light.Direction		);
		getLightField( "Range",			i, &light.Ambient		);
		getLightField( "Falloff",		i, &light.Falloff		);
		getLightField( "Attenuation0",	i, &light.Attenuation0	);
		getLightField( "Attenuation1",	i, &light.Attenuation1	);
		getLightField( "Attenuation2",	i, &light.Attenuation2	);
		getLightField( "Theta",			i, &light.Theta			);
		getLightField( "Phi",			i, &light.Phi			);
		
	}
	
*/
	return true;
}

/*---------------------------------------------------------------------------*/
/*	Func:	Shader::Sub
/*	Desc:	Subtracts shader b from shader a, leaving only that states from 
/*				shader a, that have different value than same states in b have.
/*---------------------------------------------------------------------------*/
void Shader::Sub( Shader& a, Shader& b )
{
	int cB = 0;
	Reset();
	a.order();
	b.order();
	for (int i = 0; i < a.nStates; i++)
	{
		DWORD tidx = a.state[i].tableIdx;
		while (cB < b.nStates && b.state[cB].tableIdx < tidx) cB++;
		
		ShaderState& stA = a.state[i];
		if (cB == b.nStates) 
		{
			addState( stA );
			continue;
		}
		
		ShaderState& stB = b.state[cB];
		if (stA.tableIdx != stB.tableIdx)
		//  no such state in b
		{
			addState( stA );
		}
		else
		{
			if (!stA.equalValue( stB ))
			//  state is the same, but differs by value
			{
				addState( stA );
			}
		}
		cB++;
	}

	//  vertex shader handle
	if (a.vshID != b.vshID) vshID = a.vshID;
} // Shader::Sub

/*---------------------------------------------------------------------------*/
/*	Func:	Shader::Supp
/*	Desc:	Adds all states (with their values), 
/*				which are not already in this shader
/*---------------------------------------------------------------------------*/
void Shader::Supp( Shader& s )
{
	assert( false );
}

void Shader::order()
{
	if (correctOrder) return;
	
	//  sort pointers in stateOrder array
	for (int i = 0; i < nStates; i++)
	{
		stateOrder[i] = &state[i];
	}
	qsort( stateOrder, nStates, sizeof( ShaderState* ), ShaderState::cmpFunc );

	correctOrder = true;
}

void Shader::Reset()
{
	nStates = 0;
	correctOrder = true;
}

void Shader::Dump( FILE* fp, ShaderCache* cache )
{
	if (cache)
	{
		const char* bShName = cache->getShader( baseShader ).name;
		fprintf( fp, "Dumping shader: %s(%d); base: %s(%d); execTime=%f\n", 
						name, id, bShName, baseShader, execTime );
	}
	else
	{
		fprintf( fp, "\n\nDumping shader: %s(%d); base=%d; execTime=%f\n", 
						name, id, baseShader, execTime );
	}

	fprintf( fp, "vshID=%d\n", vshID );
	for (int i = 0; i < nStates; i++)
	{
		state[i].dump( fp );
	}
} // Shader::Dump

bool Shader::Load( const char* rootDirectory, DXDevice* pDevice )
{
	char fname[_MAX_PATH];
	sprintf( fname, "%s\\shaders\\%s.sha", rootDirectory, name );

	SAFE_RELEASE( iEffect );
	ID3DXBuffer* compileErr = NULL;
	HRESULT hres = D3DXCreateEffectFromFile( pDevice, fname, &iEffect, &compileErr );
	if (hres != S_OK) 
	{
		if (!compileErr)
		{
			Log.Error( "Could not load shader file %s", fname );
		}
		else
		{
			char* errbuf = (char*)compileErr->GetBufferPointer();
			Log.Error( "Could not compile shader <%s>. Error message: %s", 
							name, errbuf );
			SAFE_RELEASE( compileErr );
		}
		return false;
	}
	SAFE_RELEASE( compileErr );
	
	if (stateBlockID != NO_STATE_BLOCK)
	//  delete state block
	{
		pDevice->DeleteStateBlock( stateBlockID );
		stateBlockID = NO_STATE_BLOCK;
	}

	if (iEffect->Validate() != S_OK)
	{
		Log.Warning( "Shader <%s> is invalid for current device", name );
		//return false;
	}

	unsigned int numPasses;
	iEffect->SetTechnique( "T0" );
	iEffect->Begin( &numPasses, 0 );
	iEffect->Pass( 0 );
	CreateFromCurrentDeviceState( pDevice );
	iEffect->End();

	return true;
} // Shader::Load

void Shader::addState( const ShaderState& st )
{
	assert( nStates < c_MaxStates );
	ShaderState& nst = state[nStates++];
	nst.copy( st );
}

void Shader::addRenderStateDesc( const char* stname, D3DRENDERSTATETYPE state, void* valType )
{
	assert( nStateDesc < c_MaxStates );
	ShaderStateDesc& st = stateDesc[nStateDesc++];
	st.setName( stname  );
	st.setValType( valType );
	st.setStateType( sstRenderState );
	st.stateDevID = (DWORD) state;
} // Shader::addRenderStateDesc

void Shader::addTexStageStateDesc( int idx, const char* stname, 
									D3DTEXTURESTAGESTATETYPE state, void* valType )
{
	assert( nStateDesc < c_MaxStates );
	ShaderStateDesc& st = stateDesc[nStateDesc++];
	char buf[1024];
	assert( strlen( stname ) < 1024 );
	sprintf( buf, "%s[%d]", stname, idx );
	st.setName( buf );
	st.setValType( valType );
	st.setStateType( sstTexStageState );
	st.stateDevID = (DWORD) state;
} // Shader::addIndexedStateDesc

void Shader::addMaterialFieldDesc( const char* stname, int fieldStride, void* valType )
{
	assert( nStateDesc < c_MaxStates );
	ShaderStateDesc& st = stateDesc[nStateDesc++];
	char buf[1024];
	assert( strlen( stname ) < 1024 );
	sprintf( buf, "Material%s", stname );
	st.setName( buf );
	st.setValType( valType );
	st.setStateType( sstMaterialField );
	//  here lies field stride
	st.stateDevID = (DWORD) fieldStride;
} // Shader::addMaterialFieldDesc

void Shader::addLightFieldDesc(  const char* stname, int lightIdx, int fieldStride, void* valType )
{
	assert( nStateDesc < c_MaxStates );
	ShaderStateDesc& st = stateDesc[nStateDesc++];

	char buf[1024];
	assert( strlen( stname ) < 1024 );
	sprintf( buf, "Light%s[%d]", stname, lightIdx );
	st.setName( buf );
	st.setValType( valType );
	st.setStateType( sstLightField );

	if (!strcmp( stname, "Enable" ))  
	{
		st.setStateType( sstLightEnable );
	}

	//  here lies field stride
	st.stateDevID = (DWORD) fieldStride;
} // Shader::addLightFieldDesc

/*---------------------------------------------------------------------------*/
/*	Func:	Shader::getCreateState
/*	Desc:	returns reference to the state with given name, if there is 
/*				no such, it will be created
/*---------------------------------------------------------------------------*/
ShaderState* Shader::getCreateState( const char* stateName )
{
	for (int i = 0; i < nStates; i++)
	{
		ShaderStateDesc& descr = stateDesc[state[i].tableIdx];
		if (descr.hasName( stateName )) return &state[i];
	}
	//  no such state in shader => add it
	assert( nStates >= 0 && nStates < c_MaxStates );
	ShaderState& st = state[nStates++];
	st.tableIdx = getStateDescrTableIdx( stateName );
	correctOrder = false;
	return &st;
} // Shader::getCreateState
	
void Shader::initStatesTable()
{
	//  performance counter resolution
	__int64 perfFr;	
	QueryPerformanceFrequency( (LARGE_INTEGER*)&perfFr );
	perfCounterRes = (double)perfFr;

	addRenderStateDesc( "ZEnable",				D3DRS_ZENABLE,				tZEnable	);
	addRenderStateDesc( "ZWriteEnable",			D3DRS_ZWRITEENABLE,			tBool		);
	addRenderStateDesc( "ZFunc", 				D3DRS_ZFUNC,				tCmpFunc	);
	addRenderStateDesc( "ZBias", 				D3DRS_ZBIAS,				T_DWORD_HEX );
	addRenderStateDesc( "FillMode",				D3DRS_FILLMODE,				tFillMode	);
	addRenderStateDesc( "ShadeMode",			D3DRS_SHADEMODE,			tShadeMode	);
	addRenderStateDesc( "LinePattern",			D3DRS_LINEPATTERN,			T_DWORD_HEX	);
	addRenderStateDesc( "SrcBlend", 			D3DRS_SRCBLEND,				tBlend		);
	addRenderStateDesc( "DestBlend", 			D3DRS_DESTBLEND,			tBlend		);
	addRenderStateDesc( "AlphaTestEnable",		D3DRS_ALPHATESTENABLE,		tBool		);
	addRenderStateDesc( "AlphaRef", 			D3DRS_ALPHAREF,				T_DWORD_HEX	);
	addRenderStateDesc( "AlphaFunc", 			D3DRS_ALPHAFUNC,			tCmpFunc	);
	addRenderStateDesc( "AlphaBlendEnable",		D3DRS_ALPHABLENDENABLE,		tBool		);
	addRenderStateDesc( "LastPixel", 			D3DRS_LASTPIXEL,			tBool		);
	addRenderStateDesc( "CullMode", 			D3DRS_CULLMODE,				tCull		);
	addRenderStateDesc( "DitherEnable", 		D3DRS_DITHERENABLE,			tBool		);
	addRenderStateDesc( "SpecularEnable",		D3DRS_SPECULARENABLE,		tBool		);
	addRenderStateDesc( "ZVisible", 			D3DRS_ZVISIBLE,				T_DWORD_HEX );
	addRenderStateDesc( "FogEnable", 			D3DRS_FOGENABLE,			tBool		);
	addRenderStateDesc( "FogColor", 			D3DRS_FOGCOLOR,				T_DWORD_HEX );
	addRenderStateDesc( "FogTableMode", 		D3DRS_FOGTABLEMODE,			tFogMode	);
	addRenderStateDesc( "FogStart", 			D3DRS_FOGSTART,				T_FLOAT		);
	addRenderStateDesc( "FogEnd", 				D3DRS_FOGEND,				T_FLOAT		);
	addRenderStateDesc( "FogDensity", 			D3DRS_FOGDENSITY,			T_FLOAT		);
	addRenderStateDesc( "EdgeAntialias", 		D3DRS_EDGEANTIALIAS,		tBool		);
	addRenderStateDesc( "RangeFogEnable",		D3DRS_RANGEFOGENABLE,		tBool		);
	addRenderStateDesc( "StencilEnable", 		D3DRS_STENCILENABLE,		tBool		);
	addRenderStateDesc( "StencilFail", 			D3DRS_STENCILFAIL,			tStencilOp	);
	addRenderStateDesc( "StencilZFail", 		D3DRS_STENCILZFAIL,			tStencilOp	);
	addRenderStateDesc( "StencilPass", 			D3DRS_STENCILPASS,			tStencilOp  );
	addRenderStateDesc( "StencilFunc", 			D3DRS_STENCILFUNC,			tCmpFunc	);
	addRenderStateDesc( "StencilRef", 			D3DRS_STENCILREF,			T_DWORD_HEX	);
	addRenderStateDesc( "StencilMask", 			D3DRS_STENCILMASK,			T_DWORD_HEX );
	addRenderStateDesc( "StencilWriteMask",		D3DRS_STENCILWRITEMASK,		T_DWORD_HEX );
	addRenderStateDesc( "ColorWriteEnable", 	D3DRS_COLORWRITEENABLE,		T_DWORD_HEX	);
	addRenderStateDesc( "TextureFactor", 		D3DRS_TEXTUREFACTOR,		T_DWORD_HEX );
	addRenderStateDesc( "Lighting", 			D3DRS_LIGHTING,				tBool		);
	addRenderStateDesc( "NormalizeNormals",		D3DRS_NORMALIZENORMALS,		tBool		);
	addRenderStateDesc( "AmbientMaterialSource",D3DRS_AMBIENTMATERIALSOURCE,tMaterialColorSource );
	addRenderStateDesc( "DiffuseMaterialSource",D3DRS_DIFFUSEMATERIALSOURCE,tMaterialColorSource );
	addRenderStateDesc( "SpecularMaterialSource",D3DRS_SPECULARMATERIALSOURCE,tMaterialColorSource );
	addRenderStateDesc( "EmmisiveMaterialSource",D3DRS_EMISSIVEMATERIALSOURCE,tMaterialColorSource );
	addRenderStateDesc( "Ambient", 				D3DRS_AMBIENT,				T_DWORD_HEX			);
	addRenderStateDesc( "Wrap0", 				D3DRS_WRAP0,				T_DWORD_HEX			);
	addRenderStateDesc( "Wrap1", 				D3DRS_WRAP1,				T_DWORD_HEX			);
	addRenderStateDesc( "Wrap2", 				D3DRS_WRAP2,				T_DWORD_HEX			);
	addRenderStateDesc( "Wrap3", 				D3DRS_WRAP3,				T_DWORD_HEX			);
	addRenderStateDesc( "Wrap4", 				D3DRS_WRAP4,				T_DWORD_HEX			);
	addRenderStateDesc( "Wrap5", 				D3DRS_WRAP5,				T_DWORD_HEX			);
	addRenderStateDesc( "Wrap6", 				D3DRS_WRAP6,				T_DWORD_HEX			);
	addRenderStateDesc( "Wrap7", 				D3DRS_WRAP7,				T_DWORD_HEX			);
	
	addRenderStateDesc( "Clipping", 			D3DRS_CLIPPING,				tBool				);
	addRenderStateDesc( "FogVertexMode", 		D3DRS_FOGVERTEXMODE,		tFogMode			);
	addRenderStateDesc( "ColorVertex", 			D3DRS_COLORVERTEX,			tBool				);
	addRenderStateDesc( "LocalViewer", 			D3DRS_LOCALVIEWER,			tBool				);
	addRenderStateDesc( "VertexBlend", 			D3DRS_VERTEXBLEND,			tVertexBlendFlags	);
	addRenderStateDesc( "ClipPlaneEnable",		D3DRS_CLIPPLANEENABLE,		T_DWORD_HEX			);
	addRenderStateDesc( "SoftwareVertexProcessing", D3DRS_SOFTWAREVERTEXPROCESSING,	tBool		);
	addRenderStateDesc( "PointSize", 			D3DRS_POINTSIZE,			T_FLOAT				);
	addRenderStateDesc( "PointSize_Min", 		D3DRS_POINTSIZE_MIN,		T_FLOAT				);
	addRenderStateDesc( "PointSpriteEnable", 	D3DRS_POINTSPRITEENABLE,	tBool				);
	addRenderStateDesc( "PointScaleEnable", 	D3DRS_POINTSCALEENABLE,		tBool				);
	addRenderStateDesc( "PointScale_A", 		D3DRS_POINTSCALE_A,			T_FLOAT				);
	addRenderStateDesc( "PointScale_B", 		D3DRS_POINTSCALE_B,			T_FLOAT				);
	addRenderStateDesc( "PointScale_C", 		D3DRS_POINTSCALE_C,			T_FLOAT				);
	addRenderStateDesc( "MultiSamleAntialias",	D3DRS_MULTISAMPLEANTIALIAS,	tBool				);
	addRenderStateDesc( "MultiSampleMask", 		D3DRS_MULTISAMPLEMASK,		T_DWORD_HEX			);
	addRenderStateDesc( "PatchEdgeStyle", 		D3DRS_PATCHEDGESTYLE,		tPatchEdgeStyle		);
	addRenderStateDesc( "PatchSegments", 		D3DRS_PATCHSEGMENTS,		T_FLOAT				);
	addRenderStateDesc( "DebugMonitorToken", 	D3DRS_DEBUGMONITORTOKEN,	tDebugMonitorTokens );
	addRenderStateDesc( "PointSize_Max", 		D3DRS_POINTSIZE_MAX,		T_FLOAT				);
	addRenderStateDesc( "IndexedVertexBlendEnable", 
							D3DRS_INDEXEDVERTEXBLENDENABLE,					tBool				);
	addRenderStateDesc( "TweenFactor", 			D3DRS_TWEENFACTOR,			T_FLOAT				);
	addRenderStateDesc( "BlendOp", 				D3DRS_BLENDOP,				tBlendOp			);
	addRenderStateDesc( "PositionOrder", 		D3DRS_POSITIONORDER,		T_DWORD_HEX			);
	addRenderStateDesc( "NormalOrder", 			D3DRS_NORMALORDER,			T_DWORD_HEX			);

	for (int i = 0; i < c_MaxShaderTextureStages; i++)
	{
		addTexStageStateDesc( i, "ColorOp",			D3DTSS_COLOROP,			tTextureOp			);
		addTexStageStateDesc( i, "ColorArg1",		D3DTSS_COLORARG1,		tTextureArg			);
		addTexStageStateDesc( i, "ColorArg2",		D3DTSS_COLORARG2,		tTextureArg			);
		addTexStageStateDesc( i, "AlphaOp",			D3DTSS_ALPHAOP,			tTextureOp			);
		addTexStageStateDesc( i, "AlphaArg1",		D3DTSS_ALPHAARG1,		tTextureArg			);
		addTexStageStateDesc( i, "AlphaArg2",		D3DTSS_ALPHAARG2,		tTextureArg			);
		addTexStageStateDesc( i, "BumpEnvMat00",	D3DTSS_BUMPENVMAT00,	T_FLOAT				);
		addTexStageStateDesc( i, "BumpEnvMat01",	D3DTSS_BUMPENVMAT01,	T_FLOAT				);
		addTexStageStateDesc( i, "BumpEnvMat10",	D3DTSS_BUMPENVMAT10,	T_FLOAT				);
		addTexStageStateDesc( i, "BumpEnvMat11",	D3DTSS_BUMPENVMAT11,	T_FLOAT				);
		addTexStageStateDesc( i, "TexCoordIndex",	D3DTSS_TEXCOORDINDEX,	T_DWORD_HEX			);
		addTexStageStateDesc( i, "AddressU",		D3DTSS_ADDRESSU,		tTextureAddress		);
		addTexStageStateDesc( i, "AddressV",		D3DTSS_ADDRESSV,		tTextureAddress		);
        addTexStageStateDesc( i, "BorderColor",		D3DTSS_BORDERCOLOR,		T_DWORD_HEX			);
        addTexStageStateDesc( i, "MagFilter",		D3DTSS_MAGFILTER,		tTextureFilterType	);
        addTexStageStateDesc( i, "MinFilter",		D3DTSS_MINFILTER,		tTextureFilterType	);
        addTexStageStateDesc( i, "MipFilter",		D3DTSS_MIPFILTER,		tTextureFilterType	);
        addTexStageStateDesc( i, "MipmapLodBias",	D3DTSS_MIPMAPLODBIAS,	T_FLOAT				);
        addTexStageStateDesc( i, "MaxMipLevel",		D3DTSS_MAXMIPLEVEL,		T_DWORD_HEX			);
        addTexStageStateDesc( i, "MaxAnisotropy",	D3DTSS_MAXANISOTROPY,	T_DWORD_HEX			);
        addTexStageStateDesc( i, "BumpEnvlScale",	D3DTSS_BUMPENVLSCALE,	T_FLOAT				);
        addTexStageStateDesc( i, "BumpEnvlOffset",	D3DTSS_BUMPENVLOFFSET,	T_FLOAT				);
        addTexStageStateDesc( i, "TextureTransformFlags", D3DTSS_TEXTURETRANSFORMFLAGS,
													tTextureTransformFlags	);
        addTexStageStateDesc( i, "AddresW",			D3DTSS_ADDRESSW,		tTextureAddress		);
        addTexStageStateDesc( i, "ColorArg0",		D3DTSS_COLORARG0,		tTextureArg			);
        addTexStageStateDesc( i, "AlphaArg0",		D3DTSS_ALPHAARG0,		tTextureArg			);
        addTexStageStateDesc( i, "ResultArg",		D3DTSS_RESULTARG,		tTextureArg			);
	}

	/*
	//  material properties
	DXMaterial mat;
	addMaterialFieldDesc( "Diffuse",	STRIDE( mat, Diffuse ), T_VECTOR );
	addMaterialFieldDesc( "Ambient",	STRIDE( mat, Ambient ), T_VECTOR );
	addMaterialFieldDesc( "Specular",	STRIDE( mat, Specular), T_VECTOR );
	addMaterialFieldDesc( "Emissive",	STRIDE( mat, Emissive), T_VECTOR );
	addMaterialFieldDesc( "Power",		STRIDE( mat, Power	 ), T_FLOAT	 );
	
	

	//  light states
	//DXLight lt;
	for (i = 0; i < c_MaxLights; i++)
	{
		addLightFieldDesc( "Enable",		i, 0,							tBool		);
		
		addLightFieldDesc( "Type",			i, STRIDE( lt, Type			),	tLightType	);
		addLightFieldDesc( "Diffuse",		i, STRIDE( lt, Diffuse		),	T_VECTOR	);
		addLightFieldDesc( "Specular",		i, STRIDE( lt, Specular		),	T_VECTOR	);
		addLightFieldDesc( "Ambient",		i, STRIDE( lt, Ambient		),	T_VECTOR	);
		addLightFieldDesc( "Position",		i, STRIDE( lt, Position		),	T_VECTOR	);
		addLightFieldDesc( "Direction",		i, STRIDE( lt, Direction	),	T_VECTOR	);
		addLightFieldDesc( "Range",			i, STRIDE( lt, Range		),	T_FLOAT		);
		addLightFieldDesc( "Falloff",		i, STRIDE( lt, Falloff		),	T_FLOAT		);
		addLightFieldDesc( "Attenuation0",	i, STRIDE( lt, Attenuation0 ),	T_FLOAT		);
		addLightFieldDesc( "Attenuation1",	i, STRIDE( lt, Attenuation1 ),	T_FLOAT		);
		addLightFieldDesc( "Attenuation2",	i, STRIDE( lt, Attenuation2 ),	T_FLOAT		);
		addLightFieldDesc( "Theta",			i, STRIDE( lt, Theta		),	T_FLOAT		);
		addLightFieldDesc( "Phi",			i, STRIDE( lt, Phi			),	T_FLOAT		);	
		
		
	}
	*/
	
} // Shader::InitStatesTable

/*****************************************************************************/
/*	ShaderState implementation
/*****************************************************************************/
DXMaterial		ShaderState::curMtl;
DXLight			ShaderState::curLight[c_MaxLights];
bool			ShaderState::lchanged[c_MaxLights];
bool			ShaderState::mchanged;

ShaderState::~ShaderState()
{
	//  here we must delete dynamically allocated memory for value,
	//  if there was such
	ShaderStateDesc& stDesc = Shader::stateDesc[tableIdx];
	if (stDesc.valType == T_VECTOR) delete []((BYTE*)value);
	value			= 0;
	stDesc.valType	= 0;
} // ShaderState::~ShaderState


bool ShaderState::less( const ShaderState& state )
{
	return (strcmp(	Shader::getStateName( *this ), 
					Shader::getStateName( state ) ) < 0);
} // ShaderState::less

bool ShaderState::equal( const ShaderState& state )
{
	return !strcmp(	Shader::getStateName( *this ), 
					Shader::getStateName( state ) );
} // ShaderState::equal	

bool ShaderState::equalValue( const ShaderState& state )
{
	DWORD valType = (DWORD)Shader::stateDesc[tableIdx].valType;
	switch (valType)
	{
	case T_VECTOR:
		return (memcmp( (BYTE*)value, (BYTE*)state.value, sizeof( Vec4 ) ) == 0);
	default:
		return value == state.value;
	}
	return false;
} // ShaderState::equal	

void ShaderState::copy( const ShaderState& st )
{
	tableIdx	= st.tableIdx;
	value		= st.value;
	stage		= st.stage;
} // ShaderState::copy

void ShaderState::order()
{
	assert( false );
} // ShaderState::order

int ShaderState::cmpFunc( const void* pState1, const void* pState2 )
{
	assert( pState1 && pState2 );
	ShaderState& shs1 = *((ShaderState*) pState1);
	ShaderState& shs2 = *((ShaderState*) pState2);
	if (shs1.less( shs2 )) return -1;
	else if (shs1.equal( shs2 )) return 0;
	return 1;
} // ShaderState::cmpFunc

void ShaderState::dump( FILE* fp )
{
	const int c_StateLineBufSize = 512;
	char buf[c_StateLineBufSize];
	char valStr[c_StateLineBufSize];
	
	sprintf( buf, "    %s", Shader::getStateName( *this ) );
	const int c_StateAlignColumnWidth = 32;
	int nChar = strlen( buf );
	while (nChar < c_StateAlignColumnWidth) buf[nChar++] = ' ';
	Shader::getValStr( *this, valStr, c_StateLineBufSize );

	sprintf( buf + nChar, "= %s;"/*  devID = %d"*/, valStr
				/*, Shader::stateDesc[tableIdx].stateDevID*/ );
	fprintf( fp, "%s\n", buf );
} // ShaderState::dump

bool ShaderState::apply( DXDevice* pDevice )
{
	HRESULT hres;

	ShaderStateDesc& stDesc = Shader::stateDesc[tableIdx];
	switch (stDesc.stateType)
	{
	case sstRenderState:
		hres = pDevice->SetRenderState( (D3DRENDERSTATETYPE)stDesc.stateDevID, value );
		if (hres != S_OK) return false;
		return true;
	case sstTexStageState:
		hres = pDevice->SetTextureStageState( stage,
							(D3DTEXTURESTAGESTATETYPE)stDesc.stateDevID, value );
		if (hres != S_OK) return false;
		return true;

	/*case sstMaterialField:
		ShaderState::mchanged = true;
		setValueToDataPtr( &curMtl, stDesc.stateDevID );
		return true;

	case sstLightField:
		ShaderState::lchanged[stage] = true;
		setValueToDataPtr( &curLight[stage], stDesc.stateDevID );
		return true;

	case sstLightEnable:
		hres = pDevice->LightEnable( stage, value );
		if (hres != S_OK) return false;
		return true;*/
	default: return false;
	}
} // ShaderState::apply

void ShaderState::setValueFromDataPtr( void* data )
{
	ShaderStateDesc& stDesc = Shader::stateDesc[tableIdx];
	void* valType = stDesc.valType;
	if (valType == T_VECTOR) delete []((BYTE*)value);
	switch ((DWORD)valType)
	{
	case T_VECTOR: 
		value = (DWORD)(new BYTE[sizeof(Vec4)]); 
		memcpy( (BYTE*)value, data, sizeof(Vec4) );
		return;
	default: 
		value = *((DWORD*)data); 
		return;
	}
} // ShaderState::setValueFromDataPtr

void ShaderState::setValueToDataPtr( void* data, int stride )
{
	ShaderStateDesc& stDesc = Shader::stateDesc[tableIdx];
	void* valType = stDesc.valType;
	switch ((DWORD)valType)
	{
	case T_VECTOR: 
		memcpy( (BYTE*)data + stride, (BYTE*)value, sizeof(Vec4) );
		return;
	default: 
		*((DWORD*)((BYTE*)data + stride)) = value; 
		return;
	}	
} // ShaderState::setValueToDataPtr

/*****************************************************************************/
/*	ShaderStateDesc implementation
/*****************************************************************************/
ShaderStateDesc::ShaderStateDesc( const char* sname, void* type )
{
	assert( strlen( sname ) < c_MaxShaderIDLen );
	strcpy( name, sname );
	valType = type;
}

ShaderStateDesc::ShaderStateDesc()
{
	//name[0] = 0;
	//valType = 0;
}

void ShaderStateDesc::setName( const char* sname )
{
	assert( strlen( sname ) < c_MaxShaderIDLen );
	strcpy( name, sname );
}

void ShaderStateDesc::setValType( void* type )
{
	valType = type;
}

void ShaderStateDesc::setStateType( ShaderStateType type )
{
	stateType = type;
}	

bool ShaderStateDesc::getValStr( DWORD val, char* str, int maxlen )
{
	char** typeArr = (char**)valType;

	switch ((DWORD)valType)
	{
	case 0:
		{
			sprintf( str, "%d", val );
		}break;
	case T_DWORD_HEX:
		{
			sprintf( str, "0x%X", val );
		}break;
	case T_FLOAT:
		{
			sprintf( str, "%ff", (float)val );
		}break;
	case T_VECTOR:
		{
			float* fl = (float*)val;
			sprintf( str, "<%ff, %ff, %ff, %ff>", fl[0], fl[1], fl[2], fl[3] ); 
		}break;
	default:
		{
			sprintf( str, "%s", typeArr[val] );
		}
	}
	return true;
} // ShaderStateDesc::getValStr

/*****************************************************************************/
/*	ShaderCache implementation
/*****************************************************************************/
ShaderCache::ShaderCache()
{
	currentShader = 0;
	memset( table, 0, c_MaxZeroLevelShaders * c_MaxZeroLevelShaders * sizeof( HashedShader* ) );
}

ShaderCache::~ShaderCache()
{
}

bool ShaderCache::Init( DXDevice* dev )
{
	pDevice = dev;
	
	HashedShader defAttr( "default", 0 );
	hash.reset();
	hash.add( defAttr );

	SetInitialState();
	_getcwd( root, _MAX_PATH );
	return true;
} // ShaderCache::Init

void ShaderCache::Shut()
{
	int nSh = hash.numElem();
	for (int i = 0; i < nSh; i++)
	{
		hash.elem( i ).InvalidateDeviceObjects( pDevice );
	}
} // ShaderCache::Shut

int ShaderCache::GetShaderID( const char* name )
{
	return getShaderID( name, 0 );
} // ShaderCache::GetShaderID

HashedShader& ShaderCache::getShader( int idx )
{ 
	return hash.elem( idx ); 
}

int	ShaderCache::getShaderID( const char* name, int base )
{
	HashedShader sh( name, base );
	int id = hash.find( sh ); 
	if (id == NO_ELEMENT)
	{
		if (base != 0)
		//  create new shader by subtracting
		{
			int				idxA	= getShaderID( name, 0 );
			HashedShader&	shA		= hash.elem( idxA );
			HashedShader&	shB		= hash.elem( base );
			int				idxRes	= hash.add( sh );
			HashedShader&	shRes	= hash.elem( idxRes );
			shRes.Sub( shA, shB );
			shRes.SetID( idxRes );
			return idxRes;
		}
		else
		//  load shader
		{
			id = hash.add( sh );
			HashedShader& shNew = hash.elem( id );
			shNew.SetID( id );
			if (!shNew.Load( root, pDevice )) return 0;
		}
	}
	return id;
} // ShaderCache::getShaderID

void ShaderCache::Reset()
{
	hash.reset();
} // ShaderCache::Reset

bool ShaderCache::InvalidateDeviceObjects()
{
	assert( pDevice );
	currentShader = 0;
	int nSh = hash.numElem();
	for (int i = 0; i < nSh; i++)
	{
		HashedShader& sh = hash.elem( i );
		sh.InvalidateDeviceObjects( pDevice );
	}

	return true;
} // ShaderCache::InvalidateDeviceObjects

bool ShaderCache::RestoreDeviceObjects()
{
	SetInitialState();
	ReloadShaders();
	return true;
} // ShaderCache::RestoreDeviceObjects

void ShaderCache::SetInitialState()
{
	pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	HashedShader& defSh = hash.elem( 0 );
	defSh.SetID( 0 );
	defSh.CreateFromCurrentDeviceState( pDevice );
	defSh.SetVertexShaderID( 0 );
	defSh.Apply( pDevice );
} // ShaderCache::SetInitialState

void ShaderCache::ReloadShaders()
{
	assert( pDevice );

	currentShader	= 0;
	int		nSh		= hash.numElem();
	int		nOrigSh = 0;
	bool	res		= false;
	for (int i = 1; i < nSh; i++)
	{
		HashedShader& sh = hash.elem( i );
		if (sh.baseShader == 0)
		{
			res = sh.Load( root, pDevice );
			if (res)
			{
				Log.Info( "Reloading shader <%s> (base 0) - succeed.", sh.name );
			}
			else
			{
				Log.Info( "Reloading shader <%s> (base 0) - FAILED!!!", sh.name );
			}
		}
	}

	//  re-subtract all other shaders
	memset( table, 0, c_MaxZeroLevelShaders * c_MaxZeroLevelShaders * sizeof( HashedShader* ) );
	for (i = 1; i < nSh; i++)
	{
		HashedShader& sh = hash.elem( i );
		if (sh.baseShader != 0)
		{
			if (sh.stateBlockID != NO_STATE_BLOCK)
			//  delete state block
			{
				pDevice->DeleteStateBlock( sh.stateBlockID );
				sh.stateBlockID = NO_STATE_BLOCK;
			}

			HashedShader& shA = hash.elem( getShaderID( sh.name, 0 ) );
			HashedShader& shB = hash.elem( sh.baseShader );
			sh.Reset();
			sh.Sub( shA, shB );
		}
	}
} // ShaderCache::ReloadShaders

int ShaderCache::cmpShadersByExecTime( const void *s1, const void *s2 )
{
	HashedShader* sh1 = *((HashedShader**)s1);
	HashedShader* sh2 = *((HashedShader**)s2);
	
	if (sh1->execTime > sh2->execTime) return -1;
	else if (sh1->execTime < sh2->execTime) return 1;
	return 0; 
} // ShaderCache::cmpShadersByExecTime

void ShaderCache::Dump( FILE* fp )
{
	if (!fp) return;

	//  sort by execution time
	int nSh = hash.numElem();
	HashedShader** ppSh = new HashedShader*[nSh];
	for (int i = 0; i < nSh; i++) ppSh[i] = &(hash.elem( i ));
	qsort( ppSh, nSh, sizeof(HashedShader*), cmpShadersByExecTime ); 
	for (i = 0; i < nSh; i++)
	{
		ppSh[i]->Dump( fp, this );
	}

	delete []ppSh;
} // ShaderCache::Dump

void ShaderCache::DumpShader( int shaderID, FILE* fp )
{
	hash.elem( shaderID ).Dump( fp, this );
} // ShaderCache::DumpShader







