/*****************************************************************************/
/*	File:	sgTexture.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	15.04.2003
/*****************************************************************************/


/*****************************************************************************/
/*	Texture implementation
/*****************************************************************************/
_inl Texture::Texture() : m_Stage(0), m_TexID(-1)
{
}

_inl Texture::Texture( const char* name, int stage ) 
		: m_Stage(stage), m_TexID(-1)
{
}

_inl int Texture::GetWidth() const
{
	return 0;
}

_inl int Texture::GetHeight() const
{
	return 0;
}

_inl TextureUsage Texture::GetUsage() const
{
	return tuUnknown;
}

_inl void Texture::SetUsage( TextureUsage  usage )
{
}


_inl void Texture::SetWidth	( int val )
{ 
}

_inl void Texture::SetHeight( int val )
{ 
}

_inl void Texture::SetColorFormat( ColorFormat format )
{
}

_inl ColorFormat Texture::GetColorFormat() const
{
	return cfUnknown;
} // Texture::GetColorFormat


_inl void Texture::SetDepthStencilFormat( DepthStencilFormat format )
{
}

_inl DepthStencilFormat Texture::GetDepthStencilFormat() const
{
	return dsfNone;
} // Texture::GetDepthStencilFormat

_inl bool Texture::IsProcedural() const 
{
	return false;
} // Texture::IsProcedural

_inl bool Texture::IsRT() const 
{
	return false;
} // Texture::IsRT

_inl void Texture::SetIsProcedural( bool val ) 
{
} // Texture::SetIsProcedural

_inl void Texture::SetIsRT( bool val )
{
}

_inl Texture* Texture::GetCurTexture( int m_Stage ) 
{ 
	return s_pCurTexture[m_Stage]; 
}

_inl TextureMemoryPool	Texture::GetMemoryPool() const
{
	return tmpUnknown;
}

_inl void Texture::SetMemoryPool( TextureMemoryPool pool )
{
}

_inl void Texture::SetNMips( int val )
{
}

_inl int Texture::GetNMips() const
{
    return 0;
}


