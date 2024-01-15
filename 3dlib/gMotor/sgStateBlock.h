/*****************************************************************************/
/*	File:	sgStateBlock.h
/*	Desc:	Render device state block
/*	Author:	Ruslan Shestopalyuk
/*	Date:	18.12.2003
/*****************************************************************************/
#ifndef __SGSTATEBLOCK_H__
#define __SGSTATEBLOCK_H__

#include "rsDeviceStates.h"

namespace sg{
/*****************************************************************************/
/*	Class:	RenderStateBlock
/*	Desc:	Set of the device render states
/*****************************************************************************/
class RenderStateBlock : public Node
{
public:
	DWORD						_BeginSaveBlock;

	bool						m_bZEnable;				
	bool						m_bZWriteEnable;			
	CmpFunc						m_ZFunc;					
	bool						m_bAlphaTestEnable;		
	bool						m_bAlphaBlendEnable;
	Blend						m_SrcBlend;
	Blend						m_DestBlend;
	DWORD						m_AlphaRef;				
	CmpFunc						m_AlphaFunc;				
	BlendOp						m_Blendop;
	FillMode					m_FillMode;	
	DWORD						m_TextureFactor;			
	ShadeMode					m_ShadeMode;				
	bool						m_bDitherEnable;	
	ColorWriteChannels			m_ColorWriteEnable;		
	CullMode					m_CullMode;					
	bool						m_bClipPlaneEnable[6];			
	VertexBlendFlags			m_VertexBlend;				
	bool						m_bSoftwareVertexProcessing;	
	bool						m_bIndexedVertexBlendEnable;	
	float						m_TweenFactor;				
	bool						m_bLighting;					
	bool						m_bColorVertex;				
	bool						m_bSpecularEnable;			
	DWORD						m_Ambient;					
	bool						m_bNormalizeNormals;			
	
	MaterialSource				m_DiffuseMaterialSource; 	
	MaterialSource				m_SpecularMaterialSource;	
	MaterialSource				m_AmbientMaterialSource;	
	MaterialSource				m_EmissiveMaterialSource;
	
	bool						m_bFogEnable;				
		
	bool						m_bStencilEnable;		
	StencilOp					m_StencilFail;				
	StencilOp					m_StencilZFail;				
	StencilOp					m_StencilPass;				
	CmpFunc						m_StencilFunc;				
	DWORD						m_StencilRef;				
	DWORD						m_StencilMask;				
	DWORD						m_StencilWriteMask;	
	
	bool						m_bClipping;					
	bool						m_bSpecularLocalViewer;	

	DWORD						_EndSaveBlock;

	//  hack
	friend class				D3DRenderSystem;

								RenderStateBlock	();
	virtual						~RenderStateBlock	();

	virtual void				Render				();
	virtual void				Expose				( PropertyMap& pm );
	virtual void				Serialize			( OutStream& os ) const;
	virtual void				Unserialize			( InStream& is	);

	_inl void					Invalidate			();
	
	void						EnableLighting		( bool bEnable = true ) { m_bLighting		= bEnable; Invalidate(); }
	void						EnableSpecular		( bool bEnable = true ) { m_bSpecularEnable = bEnable; Invalidate(); }
	void						EnableColorVertex	( bool bEnable = true ) { m_bColorVertex	= bEnable; Invalidate(); }
	void						EnableFog			( bool bEnable = true ) { m_bFogEnable		= bEnable; Invalidate(); }
	void						EnableStencil		( bool bEnable = true ) { m_bStencilEnable	= bEnable; Invalidate(); }
	void						EnableDithering		( bool bEnable = true ) { m_bDitherEnable	= bEnable; Invalidate(); }
	void						EnableZ				( bool bEnable = true ) { m_bZEnable		= bEnable; Invalidate(); }
	void						EnableZWrite		( bool bEnable = true ) { m_bZWriteEnable	= bEnable; Invalidate(); }
	void						EnableAlphaBlend	( bool bEnable = true ) { m_bAlphaBlendEnable = bEnable; Invalidate(); }
	void						EnableAlphaTest		( bool bEnable = true ) { m_bAlphaTestEnable  = bEnable; Invalidate(); }

	void						SetTextureFactor	( DWORD factor		  )	{ m_TextureFactor	= factor;   Invalidate(); }
	void						SetCullMode			( CullMode cullMode   )	{ m_CullMode		= cullMode; Invalidate(); }
	void						SetFillMode			( FillMode fillMode   ) { m_FillMode		= fillMode; Invalidate(); }
	void						SetBlendOp			( BlendOp blendOp     ) { m_Blendop			= blendOp;  Invalidate(); }

	//  shortcuts
	_inl void					SetZMode			( bool bEnableZ = true, bool bEnableZWrite = true ); 
	_inl void					AlphaBlending		( Blend src, Blend dest );

	NODE(RenderStateBlock,Node,RSBL);
}; // RenderStateBlock

/*****************************************************************************/
/*	Class:	TextureStateBlock
/*	Desc:	Set of the texture stage states for given stage
/*****************************************************************************/
class TextureStateBlock : public Node
{
public:
	DWORD						_BeginSaveBlock;
	int							m_Stage;

	TextureOp					m_ColorOp;					
	TextureArg					m_ColorArg1;				
	TextureArg					m_ColorArg2;				
	TextureOp					m_AlphaOp;					
	TextureArg					m_AlphaArg1; 				
	TextureArg					m_AlphaArg2; 				
	float 						m_BumpEnvMat00; 			
	float 						m_BumpEnvMat01; 			
	float 						m_BumpEnvMat10; 			
	float 						m_BumpEnvMat11; 			
	float 						m_BumpEnvlScale;			
	float 						m_BumpEnvlOffset;			
	TextureTransformFlags		m_TextureTransformFlags;	
	TexCoordIndex				m_TexCoordIndex;			
	TextureArg					m_ColorArg0;				
	TextureArg					m_AlphaArg0;				
	TextureArg					m_ResultArg; 				
	TextureFilterType			m_MipFilter;				
	TextureFilterType			m_MinFilter;				
	TextureFilterType			m_MagFilter;				
	TextureAddress				m_AddressU; 				
	TextureAddress				m_AddressV; 				
	DWORD						m_BorderColor;				
	float						m_MipmapLodBias;			
	int							m_MaxMipLevel;				
	int							m_MaxAnisotropy;			
	TextureAddress				m_AddressW;					

	DWORD						_EndSaveBlock;
	
								TextureStateBlock	();
	virtual						~TextureStateBlock	();

	virtual void				Render				();
	virtual void				Expose				( PropertyMap& pm );
	virtual void				Serialize			( OutStream& os ) const;
	virtual void				Unserialize			( InStream& is	);
	void						Disable				();

	_inl void					Invalidate			();

	void						SetStage			( int stage ) { m_Stage = stage; }
	_inl void					SetColorOp			( TextureOp op, TextureArg arg1, TextureArg arg2 );
	_inl void					SetAlphaOp			( TextureOp op, TextureArg arg1, TextureArg arg2 );
	_inl void					SetSampling			(	TextureFilterType minFilter, 
														TextureFilterType magFilter, 
														TextureFilterType mipFilter );
	_inl void					SetSampling			( TextureFilterType filter );
	
	_inl void					SetMinFilter		( TextureFilterType filter );
	_inl void					SetMagFilter		( TextureFilterType filter );
	_inl void					SetMipFilter		( TextureFilterType filter );
	_inl void					SetUWrap			( TextureAddress addr ) { m_AddressU = addr; }
	_inl void					SetVWrap			( TextureAddress addr ) { m_AddressV = addr; }
	_inl void					SetBorderColor		( DWORD color ) { m_BorderColor = color; }
	_inl DWORD					GetBorderColor		() const { return m_BorderColor; }

	NODE(TextureStateBlock,Node,TSBL);
}; // TextureStateBlock

/*****************************************************************************/
/*	Class:	StateBlock
/*	Desc:	Generalized block of device states
/*****************************************************************************/
class StateBlock : public Node
{
	static const DWORD			c_BadHandle = 0xFFFFFFFF;

	DWORD						m_Handle;		//  internal renderer handle
	std::string					m_ScriptName;   //  state block xml file name
public:

								StateBlock		();
	virtual void				Expose			( PropertyMap& pm );
	
	void						CreateFromScript();
	void						SaveToScript	();
	void						Render			();
	void						Invalidate		();
	void						CreateDefault	();
	
	const char*					GetScriptFileName() const { return m_ScriptName.c_str(); }
	void						SetScriptFileName( const char* pName ) { m_ScriptName = pName; }

	NODE(StateBlock,Node,DSBL);
}; // StateBlock

/*****************************************************************************/
/*	Class:	StateBlockManager
/*	Desc:	Manages stateblocks: caches them, does deltas, loads/reloads etc.
/*****************************************************************************/
class StateBlockManager : public Node, public PSingleton<StateBlockManager>
{
public:
								StateBlockManager	();
	virtual void				Expose				( PropertyMap& pm );

	NODE(StateBlockManager,Node,SMGR);
}; // StateBlockManager

}; // namespace sg

#ifdef _INLINES
#include "sgStateBlock.inl"
#endif // _INLINES

#endif // __SGSTATEBLOCK_H__