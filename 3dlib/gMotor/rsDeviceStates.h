/*****************************************************************************/
/*	File:	rsDeviceStates.h
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	09-11-2003
/*****************************************************************************/
#ifndef __RSDEVICESTATES_H__
#define __RSDEVICESTATES_H__

/*****************************************************************************/
/*	Class:	DeviceState
/*****************************************************************************/
class DeviceState
{	
public:
	int							devID;
	DWORD						value;

								DeviceState(){}
								DeviceState( int _devID, DWORD _value );

	void						Apply();
	DWORD						GetValue() const { return value; }
	
	static int					CompareByDevID( const void* pDevState1, 
												const void* pDevState2 );
}; // class DeviceState

OutStream&	operator <<( OutStream& os, const DeviceState& ds );
InStream&	operator >>( InStream& is, DeviceState& ds );

/*****************************************************************************/
/*	Class:	DSBlock	
/*****************************************************************************/
class DSBlock
{
	std::vector<DeviceState>	m_RS;
	std::vector<DeviceState>	m_TSS[c_MaxTextureStages];
	std::string					m_Name;

	DWORD						m_DevHandle;
	bool						m_bSorted;
	bool						m_bDefaultBase;
	
public:
	static const DWORD			c_BadDevHandle = 0xFFFFFFFF;

								DSBlock();
								
	OutStream&					operator <<	( OutStream& os );
	InStream&					operator >>	( InStream& is );

	DSBlock&			operator	=( const DSBlock& orig );

	bool						AddRS		( const DeviceState& ds );
	bool						AddTSS		( int stage, const DeviceState& ds );
	bool						HaveRS		( const DeviceState& ds );
	bool						HaveTSS		( int stage, const DeviceState& ds );

	void						Clear		();

	bool						ParseScript	( const char* fileName );
	bool						ParseScript	( char* buf, int bufSize );
	
	bool						SaveToScript( const char* savePath ) const;

	DeviceState*				GetRS		( int handle ) { return &m_RS[handle]; }
	DeviceState*				GetTSS		( int stage, int handle ) { return &(m_TSS[stage])[handle]; }

	const char*					GetName		() const { if (m_Name.size() > 0) return m_Name.c_str(); else return ""; }
	void						SetName		( const char* _name ) { m_Name = _name; }

	void						SetIsDefaultBase( bool bDefBase ) { m_bDefaultBase = bDefBase; }
	bool						IsDefaultBase	() const { return m_bDefaultBase; }

	void						Sub			( DSBlock& b1, DSBlock& b2 );
	void						Append		( DSBlock& block );

	DWORD						GetDevHandle() const { return m_DevHandle; }
	void						SetDevHandle( DWORD handle ) { m_DevHandle = handle; }

	int							GetNRS() const { return m_RS.size(); }
	int							GetNTSS( int stage ) const { return m_TSS[stage].size(); }

protected:
	void						SortByState();

}; // class DSBlock

/*****************************************************************************/
/*	Enum:	FillMode 
/*	Desc:	Triangle filling mode
/*****************************************************************************/
enum FillMode
{
	fmPoint		= 1,
	fmWireframe	= 2,
	fmSolid		= 3
}; // enum FillMode

ENUM( FillMode, "FillMode", 
	 en_val( fmPoint,		"Point"		) <<
	 en_val( fmWireframe,	"Wireframe"	) <<
	 en_val( fmSolid,		"Solid"		) );

/*****************************************************************************/
/*	Enum:	ShadeMode 
/*	Desc:	Triangle color interpolation mode
/*****************************************************************************/
enum ShadeMode
{
	smFlat		= 1,
	smGouraud	= 2,
	smPhong		= 3
}; // enum ShadeMode

ENUM( ShadeMode, "ShadeMode", 
	 en_val( smFlat,		"Flat"		) <<
	 en_val( smGouraud,		"Gouraud"	) <<
	 en_val( smPhong,		"Phong"		) );

/*****************************************************************************/
/*	Enum:	CullMode 
/*	Desc:	Backface culling mode
/*****************************************************************************/
enum CullMode
{
	cmNone		= 1,
	cmCW		= 2,
	cmCCW		= 3
}; // enum CullMode

ENUM( CullMode, "CullMode", 
	 en_val( cmNone,	"None"		) <<
	 en_val( cmCW,		"CW"		) <<
	 en_val( cmCCW,		"CCW"		) );

/*****************************************************************************/
/*	Enum:	MaterialSource 
/*	Desc:	Source of the material applied to the vertex color
/*****************************************************************************/
enum MaterialSource
{
	msMaterial		= 0,
	msColor1		= 1,
	msColor2		= 2
}; // enum MaterialSource

ENUM( MaterialSource, "MaterialSource", 
	 en_val( msMaterial,	"Material"		) <<
	 en_val( msColor1,		"Color1"		) <<
	 en_val( msColor2,		"Color2"		) );

/*****************************************************************************/
/*	Enum:	CmpFunc 
/*	Desc:	Compare function, used in depth/alpha/stencil tests
/*****************************************************************************/
enum CmpFunc
{
	cfNever			= 1,
	cfLessEqual		= 4,	 
	cfLess			= 2,	
	cfEqual			= 3,	 
	cfGreaterEqual	= 7,
	cfGreater		= 5,
	cfNotEqual		= 6,
	cfAlways		= 8
}; // enum CmpFunc

ENUM( CmpFunc, "CmpFunc", 
	 en_val( cfNever,			"Never"			) <<
	 en_val( cfLessEqual,		"LessEqual"		) <<
	 en_val( cfLess,			"Less"			) <<
	 en_val( cfEqual,			"Equal"			) <<
	 en_val( cfGreaterEqual,	"GreaterEqual"	) <<
	 en_val( cfGreater,			"Greater"		) <<
	 en_val( cfNotEqual,		"NotEqual"		) <<
	 en_val( cfAlways,			"Always"		) );

/*****************************************************************************/
/*	Enum:	ColorWriteChannels 
/*	Desc:	Used for ColorWriteEnable render state
/*****************************************************************************/
enum ColorWriteChannels
{
	cwNone			= 0,
	cwB				= 1,
	cwG				= 2,
	cwGB			= 3, 
	cwR				= 4,
	cwRB			= 5, 
	cwRG			= 6,
	cwRGB			= 7, 
	cwA				= 8,
	cwAB			= 9, 
	cwAG			= 10,
	cwAGB			= 11, 
	cwAR			= 12,
	cwARB			= 13, 
	cwARG			= 14,
	cwARGB			= 15
}; // enum ColorWriteChannels

ENUM( ColorWriteChannels, "ColorWriteChannels", 
	 en_val( cwNone,		"None"					) <<
	 en_val( cwA,			"Alpha"					) <<
	 en_val( cwR,			"Red"					) <<
	 en_val( cwG,			"Green"					) <<
	 en_val( cwB,			"Blue"					) <<
	 en_val( cwAR,			"Alpha|Red"				) <<
	 en_val( cwAG,			"Alpha|Green"			) <<
	 en_val( cwAB,			"Alpha|Blue"			) <<
	 en_val( cwRG,			"Red|Green"				) <<
	 en_val( cwRB,			"Red|Blue"				) <<
	 en_val( cwRGB,			"Red|Green|Blue"		) <<
	 en_val( cwGB,			"Green|Blue"			) <<
	 en_val( cwAGB,			"Alpha|Green|Blue"		) <<
	 en_val( cwARG,			"Alpha|Red|Green"		) <<
	 en_val( cwARB,			"Alpha|Red|Blue"		) <<
	 en_val( cwARGB,		"Alpha|Red|Green|Blue"	) );

/*****************************************************************************/
/*	Enum:	Blend 
/*	Desc:	Blending argument
/*****************************************************************************/
enum Blend
{
	blZero				= 1,
	blOne				= 2,
	blSrcColor			= 3,
	blInvSrcColor		= 4,
	blSrcAlphaSat		= 11,
	blSrcAlpha			= 5,
	blInvSrcAlpha		= 6,
	blDestAlpha			= 7,
	blInvDestAlpha		= 8,
	blDestColor			= 9,
	blInvDestColor		= 10,
	blBothSrcAlpha		= 12,
	blBothInvSrcAlpha	= 13,
	blBlendFactor		= 14,
	blInvBlendFactor	= 15
}; // enum Blend

ENUM( Blend, "Blend", 
	 en_val( blZero,			"Zero"				) <<
	 en_val( blOne,				"One"				) <<
	 en_val( blSrcColor,		"SrcColor"			) <<
	 en_val( blInvSrcColor,		"InvSrcColor"		) <<
	 en_val( blSrcAlphaSat,		"SrcAlphaSat"		) <<
	 en_val( blSrcAlpha,		"SrcAlpha"			) <<
	 en_val( blInvSrcAlpha,		"InvSrcAlpha"		) <<
	 en_val( blDestAlpha,		"DestAlpha"			) <<
	 en_val( blInvDestAlpha,	"InvDestAlpha"		) <<
	 en_val( blDestColor,		"DestColor"			) <<
	 en_val( blInvDestColor,	"InvDestColor"		) <<
	 en_val( blBothSrcAlpha,	"BothSrcAlpha"		) <<
	 en_val( blBothInvSrcAlpha,	"BothInvSrcAlpha"	) <<
	 en_val( blBlendFactor,		"BlendFactor"		) <<
	 en_val( blInvBlendFactor,	"InvBlendFactor"	) );

/*****************************************************************************/
/*	Enum:	FogMode 
/*	Desc:	Fog mode
/*****************************************************************************/
enum FogMode
{
	fmNone	 =	0,
	fmExp2	 =	2,
	fmExp	 =	1,
	fmLinear =	3
}; // enum FogMode

ENUM( FogMode, "FogMode", 
	 en_val( fmNone,	"None"			) <<
	 en_val( fmExp2,	"Exp2"			) <<
	 en_val( fmExp,		"Exp"			) <<
	 en_val( fmLinear,	"Linear"		) );

/*****************************************************************************/
/*	Enum:	StencilOp 
/*	Desc:	Stencil operation
/*****************************************************************************/
enum StencilOp
{
	soKeep		=	0x00000001,
	soZero		=	0x00000002,
	soReplace	=	0x00000004,
	soIncrSat	=	0x00000008,
	soDecrSat	=	0x00000010,
	soInvert	=	0x00000020,
	soIncr		=	0x00000040,
	soDecr		=	0x00000080,
	soTwoSided	=	0x00000100
}; // enum StencilOp

ENUM( StencilOp, "StencilOp", 
	 en_val( soKeep,		"Keep"			) <<
	 en_val( soZero,		"Zero"			) <<
	 en_val( soReplace,		"Replace"		) <<
	 en_val( soIncrSat,		"IncrSat"		) <<
	 en_val( soDecrSat,		"DecrSat"		) <<
	 en_val( soInvert,		"Invert"		) <<
	 en_val( soIncr,		"Incr"			) <<
	 en_val( soDecr,		"Decr"			) <<
	 en_val( soTwoSided,	"TwoSided"		) );

/*****************************************************************************/
/*	Enum:	VertexBlendFlags 
/*	Desc:	Vertex blending flags
/*****************************************************************************/
enum VertexBlendFlags
{
	vbDisable	=	0,
	vb1Weights	=	1,
	vb2Weights	=	2,
	vb3Weights	=	3,
	vbTweening	=	255,
	vb0Weights	=	256,
}; // enum VertexBlendFlags

ENUM( VertexBlendFlags, "VertexBlendFlags", 
	 en_val( vbDisable,		"Disable"		) <<
	 en_val( vb1Weights,	"1Weights"		) <<
	 en_val( vb2Weights,	"2Weights"		) <<
	 en_val( vb3Weights,	"3Weights"		) <<
	 en_val( vbTweening,	"Tweening"		) <<
	 en_val( vb0Weights,	"0Weights"		) );

/*****************************************************************************/
/*	Enum:	BlendOp 
/*	Desc:	Blending operation 
/*****************************************************************************/
enum BlendOp
{
	boAdd			= 1,
	boSubtract		= 2,
	boRevSubtract	= 3,
	boMin			= 4,
	boMax			= 5 
}; // enum BlendOp

ENUM( BlendOp, "BlendOp", 
	 en_val( boAdd,			"Add"			) <<
	 en_val( boSubtract,	"Subtract"		) <<
	 en_val( boRevSubtract,	"RevSubtract"	) <<
	 en_val( boMin,			"Min"			) <<
	 en_val( boMax,			"Max"			) );

/*****************************************************************************/
/*	Enum:	TextureOp 
/*	Desc:	Texture operation 
/*****************************************************************************/
enum TextureOp
{
	toDisable					= 1,
	toSelectArg1				= 2,
	toSelectArg2				= 3,
	toModulate2x				= 5,
	toModulate4x				= 6,
	toModulate					= 4,
	toAddSigned2x				= 9,
	toAddSigned					= 8,
	toSubtract					= 10,
	toAddSmooth					= 11,
	toAdd						= 7,
	toBlendDiffuseAlpha			= 12,
	toBlendTextureAlpha			= 13,
	toBlendFactorAlpha			= 14,
	toBlendTextureAlphaPM		= 15,
	toBlendCurrentAlpha			= 16,
	toPreModulate				= 17,
	toModulateAlpha_AddColor  	= 18,
	toModulateColor_AddAlpha  	= 19,
	toModulateInvAlpha_AddColor	= 20,
	toModulateInvColor_AddAlpha	= 21,
	toBumpEnvMapLuminance		= 23,
	toBumpEnvMap				= 22,
	toDotProduct3				= 24,
	toMultiplyAdd				= 25,
	toLerp						= 26
}; // enum TextureOp

ENUM( TextureOp, "TextureOp", 
	 en_val( toDisable,						"Disable"						) <<
	 en_val( toSelectArg1,					"SelectArg1"					) <<
	 en_val( toSelectArg2,					"SelectArg2"					) <<
	 en_val( toModulate2x,					"Modulate2x"					) <<
	 en_val( toModulate4x,					"Modulate4x"					) <<
	 en_val( toModulate,					"Modulate"						) <<
	 en_val( toAddSigned2x,					"AddSigned2x"					) <<
	 en_val( toAddSigned,					"AddSigned"						) <<
	 en_val( toSubtract,					"Subtract"						) <<
	 en_val( toAddSmooth,					"AddSmooth"						) <<
	 en_val( toAdd,							"Add"							) <<
	 en_val( toBlendDiffuseAlpha,			"BlendDiffuseAlpha"				) <<
	 en_val( toBlendTextureAlpha,			"BlendTextureAlpha"				) <<
	 en_val( toBlendFactorAlpha,			"BlendFactorAlpha"				) <<
	 en_val( toBlendTextureAlphaPM,			"BlendTextureAlphaPM"			) <<
	 en_val( toBlendCurrentAlpha,			"BlendCurrentAlpha"				) <<
	 en_val( toPreModulate,					"PreModulate"					) <<
	 en_val( toModulateAlpha_AddColor,		"ModulateAlpha_AddColor"		) <<
	 en_val( toModulateColor_AddAlpha,		"ModulateColor_AddAlpha"		) <<
	 en_val( toModulateInvAlpha_AddColor,	"ModulateInvAlpha_AddColor"		) <<
	 en_val( toModulateInvColor_AddAlpha,	"ModulateInvColor_AddAlpha"		) <<
	 en_val( toBumpEnvMapLuminance,			"BumpEnvMapLuminance"			) <<
	 en_val( toBumpEnvMap,					"BumpEnvMap"					) <<
	 en_val( toDotProduct3,					"DotProduct3"					) <<
	 en_val( toMultiplyAdd,					"MultiplyAdd"					) <<
	 en_val( toLerp,						"Lerp"							) );

/*****************************************************************************/
/*	Enum:	TextureArg 
/*	Desc:	Texture operation argument
/*****************************************************************************/
enum TextureArg
{
	taSelectMask        = 0x0000000f,  
	taDiffuse           = 0x00000000,  
	taCurrent           = 0x00000001,  
	taTexture           = 0x00000002,  
	taTFactor           = 0x00000003,  
	taSpecular          = 0x00000004,  
	taTemp              = 0x00000005,  
	taComplement        = 0x00000010,  
	taAlphaReplicate    = 0x00000020,
}; // enum TextureArg

ENUM( TextureArg, "TextureArg", 
	 en_val( taSelectMask    ,			"SelectMask"			) <<
	 en_val( taDiffuse       ,			"Diffuse"				) <<
	 en_val( taCurrent       ,			"Current"				) <<
	 en_val( taTexture       ,			"Texture"				) <<
	 en_val( taTFactor       ,			"TFactor"				) <<
	 en_val( taSpecular      ,			"Specular"				) <<
	 en_val( taTemp          ,			"Temp"					) <<
	 en_val( taComplement    ,			"Complement"			) <<
	 en_val( taAlphaReplicate,			"AlphaReplicate"		) );

/*****************************************************************************/
/*	Enum:	TextureFilterType 
/*	Desc:	Texture filtering type
/*****************************************************************************/
enum TextureFilterType
{
	tfNone            = 0,    
	tfPoint           = 1,    
	tfLinear          = 2,    
	tfAnisotropic     = 3,    
	tfFlatCubic       = 4,    
	tfGaussianCubic   = 5   
}; // enum TextureFilterType

ENUM( TextureFilterType, "TextureFilterType", 
	 en_val( tfNone                ,			"None"			) <<
	 en_val( tfPoint               ,			"Point"			) <<
	 en_val( tfLinear              ,			"Linear"		) <<
	 en_val( tfAnisotropic         ,			"Anisotropic"	) <<
	 en_val( tfFlatCubic           ,			"FlatCubic"		) <<
	 en_val( tfGaussianCubic       ,			"GaussianCubic"	) );

/*****************************************************************************/
/*	Enum:	TextureAddress 
/*	Desc:	Texture coordinates transformation mode
/*****************************************************************************/
enum TextureAddress
{
	taWrap            = 1,
	taMirrorOnce      = 5,
	taMirror          = 2,
	taClamp           = 3,
	taBorder          = 4
}; // enum TextureAddress

ENUM( TextureAddress, "TextureAddress", 
	 en_val( taWrap                      ,			"Wrap"			) <<
	 en_val( taMirrorOnce                ,			"MirrorOnce"	) <<
	 en_val( taMirror                    ,			"Mirror"		) <<
	 en_val( taClamp                     ,			"Clamp"			) <<
	 en_val( taBorder                    ,			"Border"		) );

/*****************************************************************************/
/*	Enum:	TextureTransformFlags 
/*	Desc:	Texture coordinates transformation mode
/*****************************************************************************/
enum TextureTransformFlags
{
	ttDisable    		= 0,    
	ttCount1     		= 1,    
	ttCount2     		= 2,    
	ttCount3     		= 3,    
	ttCount4			= 4,    
	ttCount1Projected	= 257,    
	ttCount2Projected	= 258,    
	ttCount3Projected   = 259,    
	ttCount4Projected   = 260,   
	ttProjected			= 256
}; // enum TextureTransformFlags

ENUM( TextureTransformFlags, "TextureTransformFlags", 
	 en_val( ttDisable                          ,			"Disable"			) <<
	 en_val( ttCount1                           ,			"Count1"			) <<
	 en_val( ttCount2                           ,			"Count2"			) <<
	 en_val( ttCount3                           ,			"Count3"			) <<
	 en_val( ttCount4                           ,			"Count4"			) <<
	 en_val( ttCount1Projected                  ,			"Count1|Projected"	) <<
	 en_val( ttCount2Projected                  ,			"Count2|Projected"	) <<
	 en_val( ttCount3Projected                  ,			"Count3|Projected"	) <<
	 en_val( ttCount4Projected                  ,			"Count4|Projected"	) <<
	 en_val( ttProjected                        ,			"Projected"			) );

/*****************************************************************************/
/*	Enum:	TexCoordIndex
/*	Desc:	Index of the texture coordinates pair to use
/*****************************************************************************/
enum TexCoordIndex
{
	tcPassThru						= 0x00000000,
	tcPair1							= 0x00000001,
	tcPair2							= 0x00000002,
	tcPair3							= 0x00000003,
	tcPair4							= 0x00000004,
	tcCameraSpaceNormal				= 0x00010000,
	tcCameraSpacePosition			= 0x00020000,
	tcCameraSpaceReflectionVector	= 0x00030000
}; // enum TexCoordIndex

ENUM( TexCoordIndex, "TexCoordIndex", 
	 en_val( tcPassThru						 , "PassThru"						) <<
	 en_val( tcPair1						 , "Pair1"							) <<
	 en_val( tcPair2						 , "Pair2"							) <<
	 en_val( tcPair3						 , "Pair3"							) <<
	 en_val( tcPair4						 , "Pair4"							) <<
	 en_val( tcCameraSpaceNormal			 , "CameraSpaceNormal"				) <<
	 en_val( tcCameraSpacePosition			 , "CameraSpacePosition"			) <<
	 en_val( tcCameraSpaceReflectionVector	 , "CameraSpaceReflectionVector"	) );

/*****************************************************************************/
/*	Enum:	RenderState
/*	Desc:	Mimics Direct3D8 render states
/*****************************************************************************/
enum RenderState
{
	//  z-buffer
	rsZEnable					= 7,
	rsZWriteEnable				= 14,
	rsZFunc						= 23,
	rsZBias						= 47,
	rsZVisible					= 30,

	//  alpha
	rsAlphaTestEnable			= 15,
	rsAlphaBlendEnable			= 27,
	rsSrcBlend					= 19,
	rsDestBlend					= 20,
	rsAlphaRef					= 24,
	rsAlphaFunc					= 25,
	rsBlendop					= 171,

	//  shading
	rsFillMode					= 8,
	rsTextureFactor				= 60,
	rsShadeMode					= 9,
	rsDitherEnable				= 26,
	rsColorWriteEnable			= 168,
	rsEdgeAntialias				= 40,
	rsLastPixel 				= 10,
	rsLinePattern 				= 16,
	rsPointSize					= 154,
	rsPointSize_min				= 155,
	rsPointSpriteEnable 		= 156,
	rsPointScaleEnable			= 157,
	rsPointScale_a 				= 158,
	rsPointScale_b 				= 159,
	rsPointScale_c 				= 160,

	//  culling
	rsCullMode					= 22,
	rsClipPlaneEnable			= 152,

	//  vertex processing
	rsVertexBlend				= 151,
	rsSoftwareVertexProcessing	= 153,
	rsIndexedVertexBlendEnable	= 167,
	rsTweenFactor				= 170,

	//  lighting
	rsLighting					= 137,
	rsColorVertex				= 141,
	rsSpecularEnable			= 29,
	rsAmbient					= 139,
	rsNormalizeNormals			= 143,
	rsDiffuseMaterialSource 	= 145,
	rsSpecularMaterialSource	= 146,
	rsAmbientMaterialSource		= 147,
	rsEmissiveMaterialSource	= 148,
	rsLocalViewer				= 142,

	// fog
	rsFogEnable					= 28,
	rsFogColor					= 34,
	rsFogTableMode				= 35,
	rsFogStart					= 36,
	rsFogEnd					= 37,
	rsFogDensity				= 38,
	rsRangeFogEnable			= 48,
	rsFogVertexMode				= 140,

	// stencil
	rsStencilEnable				= 52,
	rsStencilFail				= 53,
	rsStencilZFail				= 54,
	rsStencilPass				= 55,
	rsStencilFunc				= 56,
	rsStencilRef				= 57,
	rsStencilMask				= 58,
	rsStencilWriteMask			= 59,

	// misc
	rsWrap0 					= 128,
	rsWrap1 					= 129,
	rsWrap2 					= 130,
	rsWrap3 					= 131,
	rsWrap4 					= 132,
	rsWrap5 					= 133,
	rsWrap6 					= 134,
	rsWrap7 					= 135,
	rsClipping					= 136
}; // enum RenderState

ENUM( RenderState, "RenderState", 
	 en_val( rsZEnable					,			"ZEnable"						) <<
	 en_val( rsZWriteEnable				,			"ZWriteEnable"					) <<
	 en_val( rsZFunc					,			"ZFunc"							) <<
	 en_val( rsZBias					,			"ZBias"							) <<
	 en_val( rsZVisible					,			"ZVisible"						) <<
	 en_val( rsAlphaTestEnable			,			"AlphaTestEnable"				) <<
	 en_val( rsAlphaBlendEnable			,			"AlphaBlendEnable"				) <<
	 en_val( rsSrcBlend					,			"SrcBlend"						) <<
	 en_val( rsDestBlend				,			"DestBlend"						) <<
	 en_val( rsAlphaRef					,			"AlphaRef"						) <<
	 en_val( rsAlphaFunc				,			"AlphaFunc"						) <<
	 en_val( rsBlendop					,			"Blendop"						) <<
	 en_val( rsFillMode					,			"FillMode"						) <<
	 en_val( rsTextureFactor			,			"TextureFactor"					) <<
	 en_val( rsShadeMode				,			"ShadeMode"						) <<
	 en_val( rsDitherEnable				,			"DitherEnable"					) <<
	 en_val( rsColorWriteEnable			,			"ColorWriteEnable"				) <<
	 en_val( rsEdgeAntialias			,			"EdgeAntialias"					) <<
	 en_val( rsLastPixel 				,			"LastPixel"						) <<
	 en_val( rsLinePattern 				,			"LinePattern"					) <<
	 en_val( rsPointSize				,			"PointSize"						) <<
	 en_val( rsPointSize_min			,			"PointSize_min"					) <<
	 en_val( rsPointSpriteEnable 		,			"PointSpriteEnable"				) <<
	 en_val( rsPointScaleEnable			,			"PointScaleEnable"				) <<
	 en_val( rsPointScale_a 			,			"PointScale_a "					) <<
	 en_val( rsPointScale_b 			,			"PointScale_b"					) <<
	 en_val( rsPointScale_c 			,			"PointScale_c"					) <<
	 en_val( rsCullMode					,			"CullMode"						) <<
	 en_val( rsClipPlaneEnable			,			"ClipPlaneEnable"				) <<
	 en_val( rsVertexBlend				,			"VertexBlend"					) <<
	 en_val( rsSoftwareVertexProcessing	,			"SoftwareVertexProcessing"		) <<
	 en_val( rsIndexedVertexBlendEnable	,			"IndexedVertexBlendEnable"		) <<
	 en_val( rsTweenFactor				,			"TweenFactor"					) <<
	 en_val( rsLighting					,			"Lighting"						) <<
	 en_val( rsColorVertex				,			"ColorVertex"					) <<
	 en_val( rsSpecularEnable			,			"SpecularEnable"				) <<
	 en_val( rsAmbient					,			"Ambient"						) <<
	 en_val( rsNormalizeNormals			,			"NormalizeNormals"				) <<
	 en_val( rsDiffuseMaterialSource 	,			"DiffuseMaterialSource"			) <<
	 en_val( rsSpecularMaterialSource	,			"SpecularMaterialSource"		) <<
	 en_val( rsAmbientMaterialSource	,			"AmbientMaterialSource"			) <<
	 en_val( rsEmissiveMaterialSource	,			"EmissiveMaterialSource"		) <<
	 en_val( rsFogEnable				,			"FogEnable"						) <<
	 en_val( rsFogColor					,			"FogColor"						) <<
	 en_val( rsFogTableMode				,			"FogTableMode"					) <<
	 en_val( rsFogStart					,			"FogStart"						) <<
	 en_val( rsFogEnd					,			"FogEnd"						) <<
	 en_val( rsFogDensity				,			"FogDensity	"					) <<
	 en_val( rsRangeFogEnable			,			"RangeFogEnable"				) <<
	 en_val( rsFogVertexMode			,			"FogVertexMode"					) <<
	 en_val( rsStencilEnable			,			"StencilEnable"					) <<
	 en_val( rsStencilFail				,			"StencilFail"					) <<
	 en_val( rsStencilZFail				,			"StencilZFail"					) <<
	 en_val( rsStencilPass				,			"StencilPass"					) <<
	 en_val( rsStencilFunc				,			"StencilFunc"					) <<
	 en_val( rsStencilRef				,			"StencilRef"					) <<
	 en_val( rsStencilMask				,			"StencilMask"					) <<
	 en_val( rsStencilWriteMask			,			"StencilWriteMask"				) <<
	 en_val( rsWrap0 					,			"Wrap0"							) <<
	 en_val( rsWrap1 					,			"Wrap1"							) <<
	 en_val( rsWrap2 					,			"Wrap2"							) <<
	 en_val( rsWrap3 					,			"Wrap3"							) <<
	 en_val( rsWrap4 					,			"Wrap4"							) <<
	 en_val( rsWrap5 					,			"Wrap5"							) <<
	 en_val( rsWrap6 					,			"Wrap6"							) <<
	 en_val( rsWrap7 					,			"Wrap7"							) <<
	 en_val( rsClipping					,			"Clipping"						) <<
	 en_val( rsLocalViewer				,			"LocalViewer"					) );

/*****************************************************************************/
/*	Enum:	TextureStageState
/*	Desc:	Mimics Direct3D8 texture stage states
/*****************************************************************************/
enum TextureStageState
{
	tssColorOp					=  1,
	tssColorArg1				=  2,
	tssColorArg2				=  3,
	tssAlphaOp					=  4,
	tssAlphaArg1 				=  5,
	tssAlphaArg2 				=  6,
	tssBumpEnvMat00 			=  7,
	tssBumpEnvMat01 			=  8,
	tssBumpEnvMat10 			=  9,
	tssBumpEnvMat11 			= 10,
	tssBumpEnvlScale			= 22,
	tssBumpEnvlOffset			= 23,
	tssTextureTransformFlags	= 24,
	tssTexCoordIndex			= 11,
	tssColorArg0 				= 26,
	tssAlphaArg0 				= 27,
	tssResultArg 				= 28,
	tssMipFilter				= 18,
	tssMinFilter				= 17,
	tssMagFilter				= 16,
	tssAddressU 				= 13,
	tssAddressV 				= 14,
	tssBorderColor				= 15,
	tssMipmapLodBias			= 19,
	tssMaxMipLevel				= 20,
	tssMaxAnisotropy			= 21,
	tssAddressW					= 25
}; // enum TextureStageState

ENUM( TextureStageState, "TextureStageState", 
	 en_val( tssColorOp					,			"tssColorOp"				) <<
	 en_val( tssColorArg1				,			"tssColorArg1"				) <<
	 en_val( tssColorArg2				,			"tssColorArg2"				) <<
	 en_val( tssAlphaOp					,			"tssAlphaOp"				) <<
	 en_val( tssAlphaArg1 				,			"tssAlphaArg1"				) <<
	 en_val( tssAlphaArg2 				,			"tssAlphaArg2"				) <<
	 en_val( tssBumpEnvMat00 			,			"tssBumpEnvMat00"			) <<
	 en_val( tssBumpEnvMat01 			,			"tssBumpEnvMat01"			) <<
	 en_val( tssBumpEnvMat10 			,			"tssBumpEnvMat10"			) <<
	 en_val( tssBumpEnvMat11 			,			"tssBumpEnvMat11"			) <<
	 en_val( tssBumpEnvlScale			,			"tssBumpEnvlScale"			) <<
	 en_val( tssBumpEnvlOffset			,			"tssBumpEnvlOffset"			) <<
	 en_val( tssTextureTransformFlags	,			"tssTextureTransformFlags"	) <<
	 en_val( tssTexCoordIndex			,			"tssTexCoordIndex"			) <<
	 en_val( tssColorArg0 				,			"tssColorArg0"				) <<
	 en_val( tssAlphaArg0 				,			"tssAlphaArg0"				) <<
	 en_val( tssResultArg 				,			"tssResultArg"				) <<
	 en_val( tssMipFilter				,			"tssMipFilter"				) <<
	 en_val( tssMinFilter				,			"tssMinFilter"				) <<
	 en_val( tssMagFilter				,			"tssMagFilter"				) <<
	 en_val( tssAddressU 				,			"tssAddressU"				) <<
	 en_val( tssAddressV 				,			"tssAddressV"				) <<
	 en_val( tssBorderColor				,			"tssBorderColor"			) <<
	 en_val( tssMipmapLodBias			,			"tssMipmapLodBias"			) <<
	 en_val( tssMaxMipLevel				,			"tssMaxMipLevel"			) <<
	 en_val( tssMaxAnisotropy			,			"tssMaxAnisotropy"			) <<
	 en_val( tssAddressW				,			"tssAddressW"				) <<
	 en_val( tssColorArg0				,			"tssColorArg0"				) <<
	 en_val( tssAlphaArg0				,			"tssAlphaArg0"				) <<
	 en_val( tssResultArg				,			"tssResultArg"				) );


#endif // __RSDEVICESTATES_H__

