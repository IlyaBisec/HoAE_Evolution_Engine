/*****************************************************************************/
/*	File:	sgStateBlock.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	18.12.2003
/*****************************************************************************/
#include "stdafx.h"
#include "sgNodePool.h"
#include "sgNode.h"
#include "sgStateBlock.h"

#ifndef _INLINES
#include "sgStateBlock.inl"
#endif // !_INLINES

BEGIN_NAMESPACE( sg )
/*****************************************************************************/
/*	RenderStateBlock implementation
/*****************************************************************************/
RenderStateBlock::RenderStateBlock()
{
	m_bAlphaBlendEnable			= true;	
	m_SrcBlend					= blSrcAlpha;					
	m_DestBlend					= blInvSrcAlpha;	
	m_bAlphaTestEnable			= false;	
	m_AlphaRef					= 0x00;	
	m_AlphaFunc					= cfGreater;	
	m_Blendop					= boAdd;	

	m_bZEnable					= true;	
	m_bZWriteEnable 			= true;	
	m_ZFunc						= cfLessEqual;	

	m_FillMode					= fmSolid;	
	m_TextureFactor				= 0xFFFFFFFF;	
	m_ShadeMode					= smGouraud;	
	m_bDitherEnable				= false;	
	m_ColorWriteEnable			= cwARGB;	
	m_CullMode					= cmNone;	
	m_bClipPlaneEnable[0]		= false;	
	m_bClipPlaneEnable[1]		= false;	
	m_bClipPlaneEnable[2]		= false;	
	m_bClipPlaneEnable[3]		= false;	
	m_bClipPlaneEnable[4]		= false;	
	m_bClipPlaneEnable[5]		= false;	
	m_VertexBlend				= vbDisable;
	m_bSoftwareVertexProcessing = false;
	m_bIndexedVertexBlendEnable = false;
	m_TweenFactor				= 0.0f;
	m_bLighting					= false;
	m_bColorVertex				= false;
	m_bSpecularEnable			= false;
	m_bSpecularLocalViewer		= true;
	m_Ambient					= 0xFF000000;
	m_bNormalizeNormals			= false;

	m_DiffuseMaterialSource		= msColor1;
	m_SpecularMaterialSource	= msColor2;
	m_AmbientMaterialSource		= msMaterial;
	m_EmissiveMaterialSource	= msMaterial;

	m_bFogEnable				= false;
	
	m_bStencilEnable			= false;
	m_StencilFail				= soKeep;
	m_StencilZFail				= soKeep;
	m_StencilPass				= soKeep;
	m_StencilFunc				= cfAlways;
	m_StencilRef				= 0x0;
	m_StencilMask				= 0xFFFFFFFF;
	m_StencilWriteMask			= 0xFFFFFFFF;

	m_bClipping					= true;
} // RenderStateBlock::RenderStateBlock

RenderStateBlock::~RenderStateBlock()
{
}

void RenderStateBlock::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os.Write( &_BeginSaveBlock, ((BYTE*)&_EndSaveBlock) - ((BYTE*)&_BeginSaveBlock) );
}

void RenderStateBlock::Unserialize( InStream& is	)
{
	Parent::Unserialize( is );
	is.Read( &_BeginSaveBlock, ((BYTE*)&_EndSaveBlock) - ((BYTE*)&_BeginSaveBlock) );
} // RenderStateBlock::Unserialize

void RenderStateBlock::Render()
{
	IRS->SetRenderStateBlock( this );
} // RenderStateBlock::Render

void RenderStateBlock::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "RenderStateBlock", this );
	pm.f( "SrcBlend",					m_SrcBlend					);
	pm.f( "DestBlend",				m_DestBlend					);
	pm.f( "AlphaTestEnable",  		m_bAlphaTestEnable			);		
	pm.f( "AlphaBlendEnable", 		m_bAlphaBlendEnable			);
	pm.f( "AlphaRef",					m_AlphaRef					);				
	pm.f( "AlphaFunc",				m_AlphaFunc					);				
	pm.f( "Blendop",					m_Blendop					);

	pm.f( "ZEnable",					m_bZEnable					);				
	pm.f( "ZWriteEnable",				m_bZWriteEnable 			);			
	pm.f( "ZFunc",					m_ZFunc						);					

	pm.f( "FillMode",					m_FillMode					);	
	pm.f( "TextureFactor",			m_TextureFactor, "color"	);			
	pm.f( "ShadeMode",				m_ShadeMode					);				
	pm.f( "DitherEnable",				m_bDitherEnable				);	
	pm.f( "ColorWriteEnable", 		m_ColorWriteEnable			);		
	pm.f( "CullMode",					m_CullMode					);					
	pm.f( "ClipPlane0Enable", 		m_bClipPlaneEnable[0]		);	
	pm.f( "ClipPlane1Enable", 		m_bClipPlaneEnable[1]		);	
	pm.f( "ClipPlane2Enable", 		m_bClipPlaneEnable[2]		);	
	pm.f( "ClipPlane3Enable", 		m_bClipPlaneEnable[3]		);	
	pm.f( "ClipPlane4Enable", 		m_bClipPlaneEnable[4]		);	
	pm.f( "ClipPlane5Enable", 		m_bClipPlaneEnable[5]		);	
	pm.f( "VertexBlend",				m_VertexBlend				);				
	pm.f( "SoftwareVertexProcessing", m_bSoftwareVertexProcessing );	
	pm.f( "IndexedVertexBlendEnable", m_bIndexedVertexBlendEnable );	
	pm.f( "TweenFactor",				m_TweenFactor				);				
	pm.f( "Lighting",					m_bLighting					);					
	pm.f( "ColorVertex",				m_bColorVertex				);				
	pm.f( "SpecularEnable",			m_bSpecularEnable			);			
	pm.f( "LocalViewer",				m_bSpecularLocalViewer		);	
	pm.f( "Ambient",					m_Ambient					);					
	pm.f( "NormalizeNormals",			m_bNormalizeNormals			);			
				 
	pm.f( "DiffuseMaterialSource",	m_DiffuseMaterialSource		); 	
	pm.f( "SpecularMaterialSource",	m_SpecularMaterialSource	);	
	pm.f( "AmbientMaterialSource",	m_AmbientMaterialSource		);	
	pm.f( "EmissiveMaterialSource",	m_EmissiveMaterialSource	);
				 
	pm.f( "FogEnable",				m_bFogEnable				);				
	
	pm.f( "StencilEnable",			m_bStencilEnable			);		
	pm.f( "StencilFail",				m_StencilFail				);				
	pm.f( "StencilZFail",				m_StencilZFail				);				
	pm.f( "StencilPass",				m_StencilPass				);				
	pm.f( "StencilFunc",				m_StencilFunc				);				
	pm.f( "StencilRef",				m_StencilRef				);				
	pm.f( "StencilMask",				m_StencilMask				);				
	pm.f( "StencilWriteMask",			m_StencilWriteMask			);	
				 
	pm.f( "Clipping",					m_bClipping					);			
	pm.m( "ResetCache",				Invalidate					);

}; // RenderStateBlock::Expose

/*****************************************************************************/
/*	TextureStateBlock implementation
/*****************************************************************************/
TextureStateBlock::TextureStateBlock()
{
	m_Stage					= 0;

	m_ColorOp				= toModulate;				
	m_ColorArg1				= taTexture;			
	m_ColorArg2				= taDiffuse;			
	m_AlphaOp				= toModulate;				
	m_AlphaArg1				= taTexture; 			
	m_AlphaArg2				= taDiffuse; 			
	m_BumpEnvMat00			= 0.0f; 		
	m_BumpEnvMat01			= 0.0f; 		
	m_BumpEnvMat10			= 0.0f; 		
	m_BumpEnvMat11			= 0.0f; 		
	m_BumpEnvlScale			= 0.0f;		
	m_BumpEnvlOffset		= 0.0f;		
	m_TextureTransformFlags = ttDisable;
	m_TexCoordIndex			= tcPassThru;		
	m_ColorArg0				= taCurrent;			
	m_AlphaArg0				= taCurrent;			
	m_ResultArg				= taCurrent; 			
	m_MipFilter				= tfPoint;			
	m_MinFilter				= tfPoint;			
	m_MagFilter				= tfPoint;			
	m_AddressU				= taWrap; 			
	m_AddressV				= taWrap; 			
	m_BorderColor			= 0xFFFFFFFF;			
	m_MipmapLodBias			= 0.0f;		
	m_MaxMipLevel			= 0;			
	m_MaxAnisotropy			= 1;		
	m_AddressW				= taWrap;			
} // TextureStateBlock::TextureStateBlock

TextureStateBlock::~TextureStateBlock()
{
}

void TextureStateBlock::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os.Write( &_BeginSaveBlock, ((BYTE*)&_EndSaveBlock) - ((BYTE*)&_BeginSaveBlock) );
}

void TextureStateBlock::Unserialize( InStream& is	)
{
	Parent::Unserialize( is );
	is.Read( &_BeginSaveBlock, ((BYTE*)&_EndSaveBlock) - ((BYTE*)&_BeginSaveBlock) );
} // TextureStateBlock::Unserialize

void TextureStateBlock::Render()
{
	IRS->SetTextureStateBlock( this, m_Stage );
} // TextureStateBlock::Render

void TextureStateBlock::Disable()
{
	m_ColorOp = toDisable;
	m_AlphaOp = toDisable;
} // TextureStateBlock::Disable

void TextureStateBlock::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "TextureStateBlock", this );
	pm.f( "Stage",				m_Stage					);	
	pm.f( "ColorOp",				m_ColorOp				);				
	pm.f( "ColorArg1",			m_ColorArg1				);				
	pm.f( "ColorArg2",			m_ColorArg2				);				
	pm.f( "AlphaOp",				m_AlphaOp				);				
	pm.f( "AlphaArg1",			m_AlphaArg1				);				
	pm.f( "AlphaArg2",			m_AlphaArg2				);				
	pm.f( "TextureTransformFlags",m_TextureTransformFlags ); 
	pm.f( "TexCoordIndex",		m_TexCoordIndex			);			
	pm.f( "ColorArg0", 			m_ColorArg0 			);				
	pm.f( "AlphaArg0", 			m_AlphaArg0 			);				
	pm.f( "ResultArg", 			m_ResultArg 			);				
	pm.f( "MipFilter", 			m_MipFilter 			);				
	pm.f( "MinFilter", 			m_MinFilter 			);				
	pm.f( "MagFilter", 			m_MagFilter 			);				
	pm.f( "AddressU", 			m_AddressU 				);				
	pm.f( "AddressV", 			m_AddressV 				);
	pm.f( "BumpEnvMat00",			m_BumpEnvMat00			);			
	pm.f( "BumpEnvMat01",			m_BumpEnvMat01			);			
	pm.f( "BumpEnvMat10",			m_BumpEnvMat10			);			
	pm.f( "BumpEnvMat11",			m_BumpEnvMat11			);			
	pm.f( "BumpEnvlScale",		m_BumpEnvlScale			);			
	pm.f( "BumpEnvlOffset",		m_BumpEnvlOffset		);		
	pm.f( "BorderColor",			m_BorderColor, "color"	);			
	pm.f( "MipmapLodBias",		m_MipmapLodBias			);			
	pm.f( "MaxMipLevel",			m_MaxMipLevel			);			
	pm.f( "MaxAnisotropy",		m_MaxAnisotropy			);			
	pm.f( "AddressW",				m_AddressW				);

	pm.m( "ResetCache",			Invalidate				);
}; // TextureStateBlock::Expose

/*****************************************************************************/
/*	StateBlock implementation
/*****************************************************************************/
StateBlock::StateBlock()
{
	m_Handle = c_BadHandle;
}

void StateBlock::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "StateBlock", this );
	pm.p( "Script File", GetScriptFileName, SetScriptFileName, "file|Shaders\\DeviceStates" );
	pm.m( "LoadFromScript", CreateFromScript );
	pm.m( "SaveToScript", SaveToScript );
	pm.m( "CreateDefault", CreateDefault );
} // StateBlock::Expose

void StateBlock::CreateDefault()
{
	RemoveChildren();
	RenderStateBlock* pRS	 = AddChild<RenderStateBlock> ( "RS"   );
	TextureStateBlock* pTSS0 = AddChild<TextureStateBlock>( "TSS0" );
	TextureStateBlock* pTSS1 = AddChild<TextureStateBlock>( "TSS1" );
	pTSS1->Disable		();
	pTSS1->SetStage		( 1 );
} // StateBlock::CreateDefault

void StateBlock::Render()
{
	if (m_Handle == c_BadHandle)
	{
		m_Handle = IRS->CreateStateBlock( this );
	}
	if (m_Handle == c_BadHandle) return;
	IRS->ApplyStateBlock( m_Handle );
} // StateBlock::Render

void StateBlock::Invalidate()
{
	if (m_Handle != c_BadHandle) 
	{
		IRS->DeleteStateBlock( m_Handle );
		m_Handle = c_BadHandle;
	}
} // StateBlock::Invalidate

void StateBlock::CreateFromScript()
{
	FInStream is( m_ScriptName.c_str() );
	if (is.NoFile()) return;
	XMLNode xmlRoot( is );

	RemoveChildren();
	XMLNode::Iterator it( &xmlRoot );
	while (it)
	{
		XMLNode* pNode = it;	

		if (pNode->HasTag( "RenderState" ))
		{
			RenderStateBlock* pBlock = AddChild<RenderStateBlock>( "RenderState" );
			pBlock->FromXML( pNode );
		}
		else if (pNode->HasTag( "TextureStageState" ))
		{
			int stage = 0;
			if (pNode->GetAttr( "Stage", stage ))
			{
				TextureStateBlock* pBlock = AddChild<TextureStateBlock>( "TextureStageState" );
				pBlock->FromXML( pNode );
				pBlock->SetStage( stage );
			}
		}

		++it;
	}
} // StateBlock::CreateFromScript

void StateBlock::SaveToScript()
{
	XMLNode* pXMLRoot = ToXML();
	FOutStream os( m_ScriptName.c_str() );
	if (!pXMLRoot || os.NoFile()) return;
	pXMLRoot->Write( os );
}

/*****************************************************************************/
/*	StateBlockManager implementation
/*****************************************************************************/
StateBlockManager::StateBlockManager()
{
	SetName( "StateBlockManager" );
}

void StateBlockManager::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "StateBlockManager", this );
}

END_NAMESPACE( sg )