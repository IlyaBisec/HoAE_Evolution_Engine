/*****************************************************************************/
/*	File:	sgGU16.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	03.03.2003
/*****************************************************************************/


/*****************************************************************************/
/*	GU16Package implementation
/*****************************************************************************/
_inl const GU16SpriteHdr* GU16Package::GetSpriteHeader( int frameID ) 
{
	const BYTE* pData = GetFileData();
	if (!pData) return NULL;	
	return (GU16SpriteHdr*)( pData + sizeof(GU16Header) + m_NSegments*sizeof(GU16SegHdr) + 
		frameID*sizeof(GU16SpriteHdr)); 
} // GU16Package::GetSpriteHeader


