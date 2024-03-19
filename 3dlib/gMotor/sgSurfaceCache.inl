/*****************************************************************************/
/*	File:	sgSurfaceCache.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	15.12.2003
/*****************************************************************************/

/*****************************************************************************/
/*	QuadPack implementation
/*****************************************************************************/
_inl bool QuadPack::AllocateQuad( PowerOfTwo side, int& allocx, int& allocy )
{
	return true;
} // QuadPack::AllocateQuad

_inl void QuadPack::Free()
{
	//  make all quads "free"
	memset( &m_Level, 0, sizeof( m_Level ) );
	m_Level[m_TopLevel].numQuads = 1;
} // QuadPack::FreeAllChunks


/*****************************************************************************/
/*	SurfaceCache implementation
/*****************************************************************************/

