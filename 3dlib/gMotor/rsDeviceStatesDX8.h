/*****************************************************************************/
/*	File:	rsDSDefinitionsDX8.h
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	09-11-2003
/*****************************************************************************/
#ifndef __RSDSDefinitionS_H__
#define __RSDSDefinitionS_H__

namespace dss{
/*****************************************************************************/
/*	DX8 Render States
/*****************************************************************************/
REGENUM(FillMode)
	ENVAL(Point,	1)
	ENVAL(Wireframe,2)
	ENVAL(Solid,	3)
ENDENUM(FillMode)

REGENUM(ShadeMode)
	ENVAL(Flat,		1)
	ENVAL(Gouraud,	2)
	ENVAL(Phong,	3)
ENDENUM(ShadeMode)

REGENUM(CullMode)
	ENVAL(None,		1)
	ENVAL(CW,		2)
	ENVAL(CCW,		3)
ENDENUM(CullMode)

REGENUM(MaterialSource)
	ENVAL(Material,	0)
	ENVAL(Color1,	1)
	ENVAL(Color2,	2)
ENDENUM(MaterialSource)

REGENUM(CmpFunc)
	ENVAL(Never,		1)
	ENVAL(LessEqual,	4)
	ENVAL(Less,			2)
	ENVAL(Equal,		3)
	ENVAL(GreaterEqual,	7)
	ENVAL(Greater,		5)
	ENVAL(NotEqual,		6)
	ENVAL(Always,		8)
ENDENUM(CmpFunc)

REGENUM(Blend)
	ENVAL(Zero,				1)
	ENVAL(One,				2)
	ENVAL(SrcColor,			3)
	ENVAL(InvSrcColor,		4)
	ENVAL(SrcAlphaSat,		11)
	ENVAL(SrcAlpha,			5)
	ENVAL(InvSrcAlpha,		6)
	ENVAL(DestAlpha,		7)
	ENVAL(InvDestAlpha,		8)
	ENVAL(DestColor,		9)
	ENVAL(InvDestColor,		10)
	ENVAL(BothSrcAlpha,		12)
	ENVAL(BothInvSrcAlpha,	13)
	ENVAL(BlendFactor,		14)
	ENVAL(InvBlendFactor,	15)
ENDENUM(Blend)

REGENUM(FogMode)
	ENVAL(None,		0)
	ENVAL(Exp2,		2)
	ENVAL(Exp,		1)
	ENVAL(Linear,	3)
ENDENUM(FogMode)

REGENUM(StencilOp)
	ENVAL(Keep,		0x00000001)
	ENVAL(Zero,		0x00000002)
	ENVAL(Replace,	0x00000004)
	ENVAL(IncrSat,	0x00000008)
	ENVAL(DecrSat,	0x00000010)
	ENVAL(Invert,	0x00000020)
	ENVAL(Incr,		0x00000040)
	ENVAL(Decr,		0x00000080)
	ENVAL(TwoSided,	0x00000100)
ENDENUM(StencilOp)

REGENUM(VertexBlendFlags)
	ENVAL(Disable,	0	)
	ENVAL(1Weights,	1	)
	ENVAL(2Weights,	2	)
	ENVAL(3Weights,	3	)
	ENVAL(Tweening,	255	)
	ENVAL(0Weights,	256	)
ENDENUM(VertexBlendFlags)

REGENUM(BlendOp)
	ENVAL(Add,			1 )
	ENVAL(Subtract,		2 )
	ENVAL(RevSubtract,	3 )
	ENVAL(Min,			4 )
	ENVAL(Max,			5 )
ENDENUM(BlendOp)

REGENUM(TextureOp)
	ENVAL(Disable,					1)
	ENVAL(SelectArg1,				2)
	ENVAL(SelectArg2,				3)
	ENVAL(Modulate2x,				5)
	ENVAL(Modulate4x,				6)
	ENVAL(Modulate,					4)
	ENVAL(AddSigned2x,				9)
	ENVAL(AddSigned,				8)
	ENVAL(Subtract,					10)
	ENVAL(AddSmooth,				11)
	ENVAL(Add,						7)
	ENVAL(BlendDiffuseAlpha,		12)
	ENVAL(BlendTextureAlpha,		13)
	ENVAL(BlendFactorAlpha,			14)
	ENVAL(BlendTextureAlphaPM,		15)
	ENVAL(BlendCurrentAlpha,		16)
	ENVAL(PreModulate,				17)
	ENVAL(ModulateAlpha_AddColor,	18)
	ENVAL(ModulateColor_AddAlpha,	19)
	ENVAL(ModulateInvAlpha_AddColor,20)
	ENVAL(ModulateInvColor_AddAlpha,21)
	ENVAL(BumpEnvMapLuminance,		23)
	ENVAL(BumpEnvMap,				22)
	ENVAL(DotProduct3,				24)
	ENVAL(MultiplyAdd,				25)
	ENVAL(Lerp,						26)
ENDENUM(TextureOp)

REGENUM(TextureArg)
	ENVAL(SelectMask        ,0x0000000f)  
	ENVAL(Diffuse           ,0x00000000)  
	ENVAL(Current           ,0x00000001)  
	ENVAL(Texture           ,0x00000002)  
	ENVAL(TFactor           ,0x00000003)  
	ENVAL(Specular          ,0x00000004)  
	ENVAL(Temp              ,0x00000005)  
	ENVAL(Complement        ,0x00000010)  
	ENVAL(AlphaReplicate    ,0x00000020)  
ENDENUM(TextureArg)

REGENUM(TextureFilterType)
	ENVAL(None            , 0)    
	ENVAL(Point           , 1)    
	ENVAL(Linear          , 2)    
	ENVAL(Anisotropic     , 3)    
	ENVAL(FlatCubic       , 4)    
	ENVAL(GaussianCubic   , 5)    
ENDENUM(TextureFilterType)

REGENUM(TextureAddress)
	ENVAL(Wrap            , 1)
	ENVAL(MirrorOnce      , 5)
	ENVAL(Mirror          , 2)
	ENVAL(Clamp           , 3)
	ENVAL(Border          , 4)
ENDENUM(TextureAddress)

REGENUM(TextureTransformFlags)
	ENVAL(Disable    , 0)    
	ENVAL(Count1Projected	  , 257)    
	ENVAL(Count2Projected     , 258)    
	ENVAL(Count3Projected     , 259)    
	ENVAL(Count4Projected     , 260) 
	ENVAL(Count1     , 1)    
	ENVAL(Count2     , 2)    
	ENVAL(Count3     , 3)    
	ENVAL(Count4     , 4)    
	ENVAL(Projected  , 256)    
ENDENUM(TextureTransformFlags)

REGENUM(TexCoordIndex)
	ENVAL(PassThru                   , 0x00000000)
	ENVAL(CameraSpaceNormal          , 0x00010000)
	ENVAL(CameraSpacePosition        , 0x00020000)
	ENVAL(CameraSpaceReflectionVector, 0x00030000)
ENDENUM(TexCoordIndex)

/*****************************************************************************/
class DSSZEnable : public RSDefinition 
{ public:
	DEVSTATE(ZEnable, 7, BoolValue);
};

class DSSFillMode : public RSDefinition 
{ public:
	DEVSTATE(FillMode, 8, FillModeValue);
};

class DSSShadeMode : public RSDefinition 
{ public:
	DEVSTATE(ShadeMode, 9, ShadeModeValue);
};

class DSSZWriteEnable : public RSDefinition 
{ public:
	DEVSTATE(ZWriteEnable, 14, BoolValue);
};

class DSSAlphaTestEnable : public RSDefinition 
{ public:
	DEVSTATE(AlphaTestEnable, 15, BoolValue);
};

class DSSAlphaBlendEnable : public RSDefinition 
{ public:
	DEVSTATE(AlphaBlendEnable, 27, BoolValue);
};

class DSSSeparateAlphaBlendEnable : public RSDefinition 
{ public:
	DEVSTATE(SeparateAlphaBlendEnable, 206, BoolValue);
};

class DSSLastPixel : public RSDefinition 
{ public:
	DEVSTATE(LastPixel, 16, BoolValue);
};

class DSSSrcBlend : public RSDefinition 
{ public:
	DEVSTATE(SrcBlend, 19, BlendValue);
};

class DSSDestBlend : public RSDefinition 
{ public:
	DEVSTATE(DestBlend, 20, BlendValue);
};

class DSSCullMode : public RSDefinition 
{ public:
	DEVSTATE(CullMode, 22, CullModeValue);
};

class DSSZFunc : public RSDefinition 
{ public:
	DEVSTATE(ZFunc, 23, CmpFuncValue);
};

class DSSAlphaRef : public RSDefinition 
{ public:
	DEVSTATE(AlphaRef, 24, HexValue);
};

class DSSAlphaFunc : public RSDefinition 
{ public:
	DEVSTATE(AlphaFunc, 25, CmpFuncValue);
};

class DSSDitherEnable : public RSDefinition 
{ public:
	DEVSTATE(DitherEnable, 26, BoolValue);
};

class DSSFogEnable : public RSDefinition 
{ public:
	DEVSTATE(FogEnable, 28, BoolValue);
};

class DSSSpecularEnable : public RSDefinition 
{ public:
	DEVSTATE(SpecularEnable, 29, BoolValue);
};

class DSSFogColor : public RSDefinition 
{ public:
	DEVSTATE(FogColor, 34, HexValue);
};

class DSSFogTableMode : public RSDefinition 
{ public:
	DEVSTATE(FogTableMode, 35, FogModeValue);
};

class DSSFogStart : public RSDefinition 
{ public:
	DEVSTATE(FogStart, 36, FloatValue);
};

class DSSFogEnd : public RSDefinition 
{ public:
	DEVSTATE(FogEnd, 37, FloatValue);
};

class DSSFogDensity : public RSDefinition 
{ public:
	DEVSTATE(FogDensity, 38, FloatValue);
};

class DSSRangeFogEnable : public RSDefinition 
{ public:
	DEVSTATE(RangeFogEnable, 48, BoolValue);
};

class DSSStencilEnable : public RSDefinition 
{ public:
	DEVSTATE(StencilEnable, 52, BoolValue);
};

class DSSStencilFail : public RSDefinition 
{ public:
	DEVSTATE(StencilFail, 53, StencilOpValue);
};

class DSSStencilZFail : public RSDefinition 
{ public:
	DEVSTATE(StencilZFail, 54, StencilOpValue);
};

class DSSStencilPass : public RSDefinition 
{ public:
	DEVSTATE(StencilPass, 55, StencilOpValue);
};

class DSSStencilFunc : public RSDefinition 
{ public:
	DEVSTATE(StencilFunc, 56, CmpFuncValue);
};

class DSSStencilRef : public RSDefinition 
{ public:
	DEVSTATE(StencilRef, 57, IntValue);
};

class DSSStencilMask : public RSDefinition 
{ public:
	DEVSTATE(StencilMask, 58, HexValue);
};

class DSSStencilWriteMask : public RSDefinition 
{ public:
	DEVSTATE(StencilWriteMask, 59, HexValue);
};

class DSSTextureFactor : public RSDefinition 
{ public:
	DEVSTATE(TextureFactor, 60, HexValue);
};

class DSSWrap0 : public RSDefinition 
{ public:
	DEVSTATE(Wrap0, 128, HexValue);
};

class DSSWrap1 : public RSDefinition 
{ public:
	DEVSTATE(Wrap1, 129, HexValue);
};

class DSSWrap2 : public RSDefinition 
{ public:
	DEVSTATE(Wrap2, 130, HexValue);
};

class DSSWrap3 : public RSDefinition 
{ public:
	DEVSTATE(Wrap3, 131, HexValue);
};

class DSSWrap4 : public RSDefinition 
{ public:
	DEVSTATE(Wrap4, 132, HexValue);
};

class DSSWrap5 : public RSDefinition 
{ public:
	DEVSTATE(Wrap5, 133, HexValue);
};

class DSSWrap6 : public RSDefinition 
{ public:
	DEVSTATE(Wrap6, 134, HexValue);
};

class DSSWrap7 : public RSDefinition 
{ public:
	DEVSTATE(Wrap7, 135, HexValue);
};

class DSSClipping : public RSDefinition 
{ public:
	DEVSTATE(Clipping, 136, BoolValue);
};

class DSSLighting : public RSDefinition 
{ public:
	DEVSTATE(Lighting, 137, BoolValue);
};

class DSSAmbient : public RSDefinition 
{ public:
	DEVSTATE(Ambient, 139, HexValue);
};

class DSSFogVertexMode : public RSDefinition 
{ public:
	DEVSTATE(FogVertexMode, 140, FogModeValue);
};

class DSSColorVertex : public RSDefinition 
{ public:
	DEVSTATE(ColorVertex, 141, BoolValue);
};

class DSSLocalViewer : public RSDefinition 
{ public:
	DEVSTATE(LocalViewer, 142, BoolValue);
};

class DSSNormalizeNormals : public RSDefinition 
{ public:
	DEVSTATE(NormalizeNormals, 143, BoolValue);
};

class DSSDiffuseMaterialSource : public RSDefinition 
{ public:
	DEVSTATE(DiffuseMaterialSource, 145, MaterialSourceValue);
};

class DSSSpecularMaterialSource : public RSDefinition 
{ public:
	DEVSTATE(SpecularMaterialSource, 146, MaterialSourceValue);
};

class DSSAmbientMaterialSource : public RSDefinition 
{ public:
	DEVSTATE(AmbientMaterialSource, 147, MaterialSourceValue);
};

class DSSEmissiveMaterialSource : public RSDefinition 
{ public:
	DEVSTATE(EmissiveMaterialSource, 148, MaterialSourceValue);
};

class DSSVertexBlend : public RSDefinition 
{ public:
	DEVSTATE(VertexBlend, 151, VertexBlendFlagsValue);
};

class DSSClipPlaneEnable : public RSDefinition 
{ public:
	DEVSTATE(ClipPlaneEnable, 152, HexValue);
};

class DSSPointSize : public RSDefinition 
{ public:
	DEVSTATE(PointSize, 154, FloatValue);
};

class DSSPointSize_min : public RSDefinition 
{ public:
	DEVSTATE(PointSize_min, 155, FloatValue);
};

class DSSPointSpriteEnable : public RSDefinition 
{ public:
	DEVSTATE(PointSpriteEnable, 156, BoolValue);
};

class DSSPointScaleEnable : public RSDefinition 
{ public:
	DEVSTATE(PointScaleEnable, 157, BoolValue);
};

class DSSPointScale_a : public RSDefinition 
{ public:
	DEVSTATE(PointScale_a, 158, FloatValue);
};

class DSSPointScale_b : public RSDefinition 
{ public:
	DEVSTATE(PointScale_b, 159, FloatValue);
};

class DSSPointScale_c : public RSDefinition 
{ public:
	DEVSTATE(PointScale_c, 160, FloatValue);
};

class DSSMultisampleAntialias : public RSDefinition 
{ public:
	DEVSTATE(MultisampleAntialias, 161, BoolValue);
};

class DSSMultisampleMask : public RSDefinition 
{ public:
	DEVSTATE(MultisampleMask, 162, HexValue);
};

class DSSPatchEdgeStyle : public RSDefinition 
{ public:
	DEVSTATE(PatchEdgeStyle, 163, IntValue);
};

class DSSDebugmonitorToken : public RSDefinition 
{ public:
	DEVSTATE(DebugmonitorToken, 165, IntValue);
};

class DSSPointSize_max : public RSDefinition 
{ public:
	DEVSTATE(PointSize_max, 166, FloatValue);
};

class DSSIndexedVertexBlendEnable : public RSDefinition 
{ public:
	DEVSTATE(IndexedVertexBlendEnable, 167, BoolValue);
};

//  TODO - remove this shame...
class ColorWriteEnableValue
{
public:
	static bool ParseValue( const char* strVal, DeviceState& ds ) 
	{ 
		DWORD res = 0;
		if (sscanf( strVal, "%d", &res ) == 1) 
		{
			ds.value = res;
			return true;
		}
		c2::string str( strVal );
		str.toLower();
		if (strstr( str.c_str(), "alpha" )) res |= (1L<<3);
		if (strstr( str.c_str(), "red" ))	res |= (1L<<0);
		if (strstr( str.c_str(), "green" )) res |= (1L<<1);
		if (strstr( str.c_str(), "blue" ))	res |= (1L<<2);
		ds.value = res;
		return true; 
	}

	static const char* ValToStr( DWORD val )
	{
		static char buf[256];
		if (val == 0) return "0";
		sprintf( buf, "%d", val );
		return buf;
	}
}; // class ColorWriteEnableValue 
class DSSColorWriteEnable : public RSDefinition 
{ public:
	DEVSTATE(ColorWriteEnable, 168, ColorWriteEnableValue);
};

class DSSTweenFactor : public RSDefinition 
{ public:
	DEVSTATE(TweenFactor, 170, FloatValue);
};

class DSSBlendop : public RSDefinition 
{ public:
	DEVSTATE(Blendop, 171, BlendOpValue);
};

class DSSPositionDegree : public RSDefinition 
{ public:
	DEVSTATE(PositionDegree, 172, IntValue);
};

class DSSNormalDegree : public RSDefinition 
{ public:
	DEVSTATE(NormalDegree, 173, IntValue);
};

class DSSScissorTestEnable : public RSDefinition 
{ public:
	DEVSTATE(ScissorTestEnable, 174, BoolValue);
};

class DSSSlopeScaleDepthBias : public RSDefinition 
{ public:
	DEVSTATE(SlopeScaleDepthBias, 175, FloatValue);
};

class DSSAntialiasedLineEnable : public RSDefinition 
{ public:
	DEVSTATE(AntialiasedLineEnable, 176, BoolValue);
};

class DSSMinTessellationLevel : public RSDefinition 
{ public:
	DEVSTATE(MinTessellationLevel, 178, FloatValue);
};

class DSSMaxTessellationLevel : public RSDefinition 
{ public:
	DEVSTATE(MaxTessellationLevel, 179, FloatValue);
};

class DSSAdaptiveTess_x : public RSDefinition 
{ public:
	DEVSTATE(AdaptiveTess_x, 180, FloatValue);
};

class DSSAdaptiveTess_y : public RSDefinition 
{ public:
	DEVSTATE(AdaptiveTess_y, 181, FloatValue);
};

class DSSAdaptiveTess_z : public RSDefinition 
{ public:
	DEVSTATE(AdaptiveTess_z, 182, FloatValue);
};

class DSSAdaptiveTess_w : public RSDefinition 
{ public:
	DEVSTATE(AdaptiveTess_w, 183, FloatValue);
};

class DSSEnableAdaptiveTesselation : public RSDefinition 
{ public:
	DEVSTATE(EnableAdaptiveTesselation, 184, BoolValue);
};

class DSSTwoSidedStencilMode : public RSDefinition 
{ public:
	DEVSTATE(TwoSidedStencilMode, 185, BoolValue);
};

class DSSCCW_StencilFail : public RSDefinition 
{ public:
	DEVSTATE(CCW_StencilFail, 186, StencilOpValue);
};

class DSSCCW_StencilZFail : public RSDefinition 
{ public:
	DEVSTATE(CCW_StencilZFail, 187, StencilOpValue);
};

class DSSCCW_StencilPass : public RSDefinition 
{ public:
	DEVSTATE(CCW_StencilPass, 188, StencilOpValue);
};

class DSSCCW_StencilFunc : public RSDefinition 
{ public:
	DEVSTATE(CCW_StencilFunc, 189, CmpFuncValue);
};

class DSSColorWriteEnable1 : public RSDefinition 
{ public:
	DEVSTATE(ColorWriteEnable1, 190, BoolValue);
};

class DSSColorWriteEnable2 : public RSDefinition 
{ public:
	DEVSTATE(ColorWriteEnable2, 191, BoolValue);
};

class DSSColorWriteEnable3 : public RSDefinition 
{ public:
	DEVSTATE(ColorWriteEnable3, 192, BoolValue);
};

class DSSBlendFactor : public RSDefinition 
{ public:
	DEVSTATE(BlendFactor, 193, HexValue);
};

class DSSSRGBWriteEnable : public RSDefinition 
{ public:
	DEVSTATE(SRGBWriteEnable, 194, HexValue);
};

class DSSDepthBias : public RSDefinition 
{ public:
	DEVSTATE(DepthBias, 195, FloatValue);
};

class DSSWrap8 : public RSDefinition 
{ public:
	DEVSTATE(Wrap8, 198, HexValue);
};

class DSSWrap9 : public RSDefinition 
{ public:
	DEVSTATE(Wrap9, 199, HexValue);
};

class DSSWrap10 : public RSDefinition 
{ public:
	DEVSTATE(Wrap10, 200, HexValue);
};

class DSSWrap11 : public RSDefinition 
{ public:
	DEVSTATE(Wrap11, 201, HexValue);
};

class DSSWrap12 : public RSDefinition 
{ public:
	DEVSTATE(Wrap12, 202, HexValue);
};

class DSSWrap13 : public RSDefinition 
{ public:
	DEVSTATE(Wrap13, 203, HexValue);
};

class DSSWrap14 : public RSDefinition 
{ public:
	DEVSTATE(Wrap14, 204, HexValue);
};

class DSSWrap15 : public RSDefinition 
{ public:
	DEVSTATE(Wrap15, 205, HexValue);
};


class DSSSrcBlendAlpha : public RSDefinition 
{ public:
	DEVSTATE(SrcBlendAlpha, 207, BlendValue);
};

class DSSDestBlendAlpha : public RSDefinition 
{ public:
	DEVSTATE(DestBlendAlpha, 208, BlendValue);
};

class DSSBlendOpAlpha : public RSDefinition 
{ public:
	DEVSTATE(BlendOpAlpha, 209, BlendOpValue);
};

class DSSZBias : public RSDefinition 
{ public:
	DEVSTATE(ZBias, 47, IntValue);
};

/*****************************************************************************/
/*	DX8 Texture Stage States
/*****************************************************************************/
class DSSColorOp : public TSSDefinition 
{ public:
	DEVSTATE(ColorOp, 1, TextureOpValue);
};

class DSSColorArg1 : public TSSDefinition 
{ public:
	DEVSTATE(ColorArg1, 2, TextureArgValue);
};

class DSSColorArg2 : public TSSDefinition 
{ public:
	DEVSTATE(ColorArg2, 3, TextureArgValue);
};

class DSSAlphaOp : public TSSDefinition 
{ public:
	DEVSTATE(AlphaOp, 4, TextureOpValue);
};

class DSSAlphaArg1 : public TSSDefinition 
{ public:
	DEVSTATE(AlphaArg1, 5, TextureArgValue);
};

class DSSAlphaArg2 : public TSSDefinition 
{ public:
	DEVSTATE(AlphaArg2, 6, TextureArgValue);
};

class DSSBumpEnvMat00 : public TSSDefinition 
{ public:
	DEVSTATE(BumpEnvMat00, 7, FloatValue);
};

class DSSBumpEnvMat01 : public TSSDefinition 
{ public:
	DEVSTATE(BumpEnvMat01, 8, FloatValue);
};

class DSSBumpEnvMat10 : public TSSDefinition 
{ public:
	DEVSTATE(BumpEnvMat10, 9, FloatValue);
};

class DSSBumpEnvMat11 : public TSSDefinition 
{ public:
	DEVSTATE(BumpEnvMat11, 10, FloatValue);
};

class DSSTexCoordIndex : public TSSDefinition 
{ public:
	DEVSTATE(TexCoordIndex, 11, TexCoordIndexValue);
};

class DSSBumpEnvlScale : public TSSDefinition 
{ public:
	DEVSTATE(BumpEnvlScale, 22, FloatValue);
};

class DSSBumpEnvlOffset : public TSSDefinition 
{ public:
	DEVSTATE(BumpEnvlOffset, 23, FloatValue);
};

class DSSTextureTransformFlags : public TSSDefinition 
{ public:
	DEVSTATE(TextureTransformFlags, 24, TextureTransformFlagsValue);
};

class DSSColorArg0 : public TSSDefinition 
{ public:
	DEVSTATE(ColorArg0, 26, TextureArgValue);
};

class DSSAlphaArg0 : public TSSDefinition 
{ public:
	DEVSTATE(AlphaArg0, 27, TextureArgValue);
};

class DSSResultArg : public TSSDefinition 
{ public:
	DEVSTATE(ResultArg, 28, TextureArgValue);
};

class DSSConstant : public TSSDefinition 
{ public:
	DEVSTATE(Constant, 32, HexValue);
};

class DSSAddressU : public TSSDefinition 
{ public:
	DEVSTATE(AddressU, 13, TextureAddressValue);
};

class DSSAddressV : public TSSDefinition 
{ public:
	DEVSTATE(AddressV, 14, TextureAddressValue);
};

class DSSBorderColor : public TSSDefinition 
{ public:
	DEVSTATE(BorderColor, 15, HexValue);
};

class DSSMagFilter : public TSSDefinition 
{ public:
DEVSTATE(MagFilter, 16, TextureFilterTypeValue);
};

class DSSMinFilter : public TSSDefinition 
{ public:
	DEVSTATE(MinFilter, 17, TextureFilterTypeValue);
};

class DSSMipFilter : public TSSDefinition 
{ public:
	DEVSTATE(MipFilter, 18, TextureFilterTypeValue);
};

class DSSMipmapLoadBias : public TSSDefinition 
{ public:
	DEVSTATE(MipmapLoadBias, 19, FloatValue);
};

class DSSMaxMipLevel : public TSSDefinition 
{ public:
	DEVSTATE(MaxMipLevel, 20, IntValue);
};

}; // namespace dss

#endif // __RSDSDefinitionSDX8_H__

