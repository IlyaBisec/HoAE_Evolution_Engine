/*****************************************************************************/
/*	File:	sgGN16.inl
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	03.03.2003
/*****************************************************************************/


/*****************************************************************************/
/*	GN16Package implementation
/*****************************************************************************/
_inl const GN16SpriteHdr* GN16Package::GetSpriteHeader( int frameID )
{
	const BYTE* pData = GetFileData();
	if (!pData) return NULL;
	return (GN16SpriteHdr*)(pData + sizeof(GN16Header) + 
				m_NSegments*sizeof(GN16SegHdr) + frameID*sizeof(GN16SpriteHdr));
}

_inl const GN16SegHdr* GN16Package::GetSegmentHeader( int segID )
{
	const BYTE* pData = GetFileData();
	if (!pData) return NULL;
	int t = sizeof(GN16Header);
	return (GN16SegHdr*)(pData + sizeof(GN16Header) + segID*sizeof(GN16SegHdr));
}


