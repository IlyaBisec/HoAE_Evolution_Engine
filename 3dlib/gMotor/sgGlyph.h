/*****************************************************************************/
/*	File:	sgGlyph.h
/*	Author:	Ruslan Shestopalyuk
/*	Date:	09-18-2003
/*****************************************************************************/
#ifndef __SGGLYPH_H__
#define __SGGLYPH_H__

namespace sg{

const int c_GlyphTextureWidth	= 256;
const int c_GlyphTextureHeight	= 128;
const int c_GlyphBucketSize		= 2048;
const int c_MaxGlyphNameLen		= 64;

const ColorFormat c_GlyphColorFormat = cfARGB4444;

/*****************************************************************************/
/*	Class:	GlyphPage
/*	Desc:	Subset of glyphs from the glyph set, allocated on the same 
/*				texture
/*****************************************************************************/
class GlyphPage : public Geometry
{
public:
	/*****************************************************************************/
	/*	Class:	Glyph
	/*	Desc:	Single glyph description
	/*****************************************************************************/
	class Glyph
	{
	public:
		DWORD			code;
		Rct				ext;
		Rct				uv;
		char			name[c_MaxGlyphNameLen];

		bool operator < ( DWORD val ) const { return code < val;  }
		bool operator ==( DWORD val ) const { return code == val; }
	}; // class Glyph

public:
						GlyphPage		();
	Rct*				AddGlyph		( int charIdx, const Vector3D& pos, DWORD clr, 
											const Matrix4D& tm = Matrix4D::identity );
	void				ClearGlyphs		();
	Glyph*				AllocateGlyph	( DWORD code, float width, float m_Height );

	_inl int			FindGlyph		( DWORD code );
	_inl float			GetGlyphWidth	( int charIdx ) const;
	_inl const Rct&		GetGlyphUV		( int charIdx ) const;
	_inl float			GetGlyphHeight	( int charIdx ) const;
	_inl int			GetTexWidth		() const { return m_TexWidth; }
	_inl int			GetTexHeight	() const { return m_TexHeight; }
	_inl bool			IsScreenSpace	() const { return m_bScreenSpace; }
	_inl void			SetIsScreenSpace( bool val = true ) { m_bScreenSpace = val; }
	_inl int			GetNGlyphTypes	() const { return m_Glyphs.size(); }
	_inl void			SetIsStatic		( bool val ) 
	{ 
		m_bStatic = val; 
		Create( c_GlyphBucketSize*4, 0, m_bStatic ? vf2Tex : vfTnL, ptTriangleList );
		GetPrimitive().setIsQuadList( true );
	}
	_inl bool			IsStatic		() const { return m_bStatic; }

	int					CreateGlyph		( const char* name );
	bool				SetGlyphRect	( int id, const Rct& rct );


	void				ClearMesh		();
	void				SetGlyphTexture ( const char* texName );

	Texture*			GetTexture		();
	bool				CreatePage		(	float width		= c_GlyphTextureWidth, 
											float m_Height	= c_GlyphTextureHeight,
											const char* texFileName = NULL );

	virtual void		Render			();
	virtual void		Serialize		( OutStream& os ) const;
	virtual void		Unserialize		( InStream& is );
	virtual void		Expose			( PropertyMap& pm );

	virtual bool		InitFromXML		( const char* fileName );

	NODE( GlyphPage, Geometry, GLYP );

protected:
	_inl void			Flush			();

private:
	std::vector<Glyph>		m_Glyphs;
	std::string				m_PageName;

	Rct						m_FreeSpace;	//  rectangle of the unfilled texture
	Rct						m_FreeLine;		//  unfilled rectangle to the end of the 

	bool					m_bScreenSpace;
	bool					m_bStatic;

	int						m_TexWidth, m_TexHeight;
	DWORD					m_Color;
}; // class GlyphPage

/*****************************************************************************/
/*	Class:	GlyphSet
/*	Desc:	Set of glyph pictures, allocated on the same texture
/*****************************************************************************/
class GlyphSet : public AssetNode
{
public:
	GlyphSet	();
	_inl void					SetTransform( const Matrix4D& tm ) { m_TM = tm; }
	void						AddGlyph	( const Vector3D& pos, DWORD code, DWORD color );
	bool						FindGlyph	( const char* name, int& page, int& id ) const;

	virtual bool				InitFromXML	( const char* fileName );

	_inl GlyphPage*				GetPage		( int pageIdx );
	_inl int					GetNPages	() const;
	int							GetPageID	( const char* pageName );
	GlyphPage*					AddPage		( const char* pageName );


	NODE(GlyphSet, AssetNode, GLYS);

protected:
	GlyphPage*					CreatePage	(	float width  = c_GlyphTextureWidth, 
												float height = c_GlyphTextureHeight, 
												const char* texFileName = NULL );
	void						CleanPageBuckets();
	
	std::vector<GlyphPage*>		m_Pages;
	Matrix4D					m_TM;
}; // class GlyphSet

/*****************************************************************************/
/*	Class:	IconSet
/*	Desc:	Set of the icons, which are glyphs with the same size
/*****************************************************************************/
class IconSet : public GlyphSet
{
public:
	virtual void	Serialize		( OutStream&	os	 ) const;
	virtual void	Unserialize		( InStream&		is	 );
	virtual void	Expose			( PropertyMap&	pmap );

	virtual bool	AddTexture		( const char* texName );

	_inl void		SetIconWidth	( int val )  { m_IconWidth  = val;	}
	_inl void		SetIconHeight	( int val )  { m_IconHeight = val;	}

	_inl int		GetIconWidth	() const	 { return m_IconWidth;	}
	_inl int		GetIconHeight	() const	 { return m_IconHeight; }

	NODE(IconSet, GlyphSet, ICOS);
protected:
	int				m_IconWidth;
	int				m_IconHeight;
}; // class IconSet


}; // namespace sg

#ifdef _INLINES
#include "sgGlyph.inl"
#endif // _INLINES

#endif // __SGGLYPH_H__
