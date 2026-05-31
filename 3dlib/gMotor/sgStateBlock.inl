/*****************************************************************************/
/*	File:	sgStateBlock.inl
/*	Desc:	Render device state block
/*	Author:	Ruslan Shestopalyuk
/*	Date:	18.12.2003
/*****************************************************************************/
BEGIN_NAMESPACE(sg)
/*****************************************************************************/
/*	RenderStateBlock implementation
/*****************************************************************************/
_inl void RenderStateBlock::SetZMode( bool bEnableZ, bool bEnableZWrite ) 
{ 
	m_bZEnable		= bEnableZ; 
	m_bZWriteEnable = bEnableZWrite;
	Invalidate();
} // RenderStateBlock::EnableZ

_inl void RenderStateBlock::AlphaBlending( Blend src, Blend dest )
{
	m_SrcBlend = src;
	m_DestBlend = dest;
	m_bAlphaBlendEnable = true;
	Invalidate();
}

_inl void RenderStateBlock::Invalidate()
{
	StateBlock* pParent = (StateBlock*)GetParent();
	if (!pParent || !pParent->IsA<StateBlock>()) return;
	pParent->Invalidate();
} // RenderStateBlock::Invalidate

/*****************************************************************************/
/*	TextureStateBlock implementation
/*****************************************************************************/
_inl void TextureStateBlock::SetColorOp( TextureOp op, TextureArg arg1, TextureArg arg2 )
{
	m_ColorOp = op; 
	m_ColorArg1 = arg1; 
	m_ColorArg2 = arg2;
	Invalidate();
} // TextureStateBlock::SetColorOp

_inl void TextureStateBlock::Invalidate()
{
	StateBlock* pParent = (StateBlock*)GetParent();
	if (!pParent || !pParent->IsA<StateBlock>()) return;
	pParent->Invalidate();
} // TextureStateBlock::Invalidate

_inl void TextureStateBlock::SetAlphaOp( TextureOp op, TextureArg arg1, TextureArg arg2 )
{
	m_AlphaOp = op; 
	m_AlphaArg1 = arg1; 
	m_AlphaArg2 = arg2;
	Invalidate();
} // TextureStateBlock::SetAlphaOp

_inl void TextureStateBlock::SetSampling( TextureFilterType minFilter, 
										  TextureFilterType magFilter, 
										  TextureFilterType mipFilter )
{
	m_MipFilter = mipFilter;
	m_MinFilter = minFilter;
	m_MagFilter = magFilter;
	Invalidate();
}

_inl void TextureStateBlock::SetSampling( TextureFilterType filter )
{
	m_MipFilter = filter;
	m_MinFilter = filter;
	m_MagFilter = filter;
	Invalidate();
}

_inl void TextureStateBlock::SetMinFilter( TextureFilterType filter )
{
	m_MinFilter = filter;
} 

_inl void TextureStateBlock::SetMagFilter( TextureFilterType filter )
{
	m_MagFilter = filter;
}

_inl void TextureStateBlock::SetMipFilter( TextureFilterType filter )
{
	m_MipFilter = filter;
}

END_NAMESPACE(sg)