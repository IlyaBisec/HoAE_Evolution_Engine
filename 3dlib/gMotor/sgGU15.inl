/*****************************************************************************/
/*	File:	sgGU15.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	03.03.2003
/*****************************************************************************/


/*****************************************************************************/
/*	GU15Package implementation
/*****************************************************************************/
_inl const GU15SpriteHdr* GU15Package::GetSpriteHeader( int frameID ) 
{
	const BYTE* pData = GetFileData();
	if (!pData) return NULL;	
	return (GU15SpriteHdr*)( pData + sizeof(GU15Header) + m_InfoLen + frameID*sizeof(GU15SpriteHdr)); 
} // GU15Package::GetSpriteHeader


