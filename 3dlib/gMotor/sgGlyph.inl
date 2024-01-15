/*****************************************************************************/
/*	File:	sgGlyph.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	09-18-2003
/*****************************************************************************/
BEGIN_NAMESPACE(sg)

#include "mAlgo.h"

/*****************************************************************************/
/*  GlyphPage implementation
/*****************************************************************************/
_inl int GlyphPage::FindGlyph( DWORD code )
{
	if (m_Glyphs.size() == 0) return -1;
	int pos = BinarySearch( code, &(m_Glyphs[0]), m_Glyphs.size() );
	if (pos < 0) return -1;
	if (m_Glyphs[pos].code == code) return pos;
	return -1;
} // GlyphPage::FindGlyph

_inl void GlyphPage::Flush()
{
	Render();
	GetPrimitive().setNVert( 0 );
	GetPrimitive().setNInd ( 0 );
	GetPrimitive().setNPri ( 0 );

} // GlyphPage::Flush

_inl float GlyphPage::GetGlyphWidth( int charIdx ) const
{
	return m_Glyphs[charIdx].ext.w;
} // GlyphPage::GetGlyphWidth

_inl float GlyphPage::GetGlyphHeight( int charIdx ) const
{
	return m_Glyphs[charIdx].ext.h;
} // GlyphPage::GetGlyphHeight

_inl const Rct& GlyphPage::GetGlyphUV( int charIdx ) const
{
	return m_Glyphs[charIdx].uv;
} // GlyphPage::GetGlyphUV


/*****************************************************************************/
/*  GlyphSet implementation
/*****************************************************************************/
GlyphPage* GlyphSet::GetPage( int pageIdx )
{
	if (pageIdx < 0 || pageIdx >= GetNPages()) return NULL;
	return m_Pages[pageIdx];
}

int	GlyphSet::GetNPages() const
{
	return m_Pages.size(); 
}

END_NAMESPACE(sg)
