/*****************************************************************************/
/*	File:	sgFont.h
/*	Author:	Ruslan Shestopalyuk
/*	Date:	09-18-2003
/*****************************************************************************/
#ifndef __SGFONT_H__
#define __SGFONT_H__

namespace sg{
/*****************************************************************************/
/*	Class:	Font
/*	Desc:	Font resource node
/*****************************************************************************/
class Font : public GlyphSet
{
public:
								Font		();
								~Font		();
	
	virtual void				Serialize	( OutStream& os ) const;
	virtual void				Unserialize	( InStream& is	);

	void						AddString	( const Vector3D& pos, const char* str, DWORD color,
												Geometry* pBucket = NULL );
	float						GetStringWidth( const char* str, int nChar = -1 );

	bool						Generate	();
	
	_inl bool					GetIsItalic	()	const { return m_bItalic;	}
	_inl bool					GetIsBold	()	const { return m_bBold;		}
	_inl const char*			GetFontName	()  const { return m_FontName.c_str();	}
	_inl int					GetHeight	()	const { return m_Height;	}

	_inl void					SetIsItalic	( bool val )		{ m_bItalic		= val; }
	_inl void					SetIsBold	( bool val )		{ m_bBold		= val; }
	_inl void					SetFontName	( const char* val ) { m_FontName	= val; }
	_inl void					SetHeight	( int val )			{ m_Height	= val; }

	virtual void				Expose( PropertyMap& pm );

	NODE( Font, GlyphSet, FONT );
private:
	std::string					m_FontName;
	bool						m_bBold;
	bool						m_bItalic;
	int							m_Height;
	int							m_HorzSpacing;

	DWORD						m_Charset;
}; // class Font

}; // namespace sg

#ifdef _INLINES
#include "sgFont.inl"
#endif // _INLINES

#endif // __SGFONT_H__
