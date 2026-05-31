/*****************************************************************************/
/*	File:	uiSubtitles.h
/*	Author:	Ruslan Shestopalyuk
/*	Date:	15.04.2003
/*****************************************************************************/
#ifndef __UISUBTITLES_H__
#define __UISUBTITLES_H__

namespace sg{
/*****************************************************************************/
/*	Class:	Subtitles
/*	Desc:	Animated overlay text
/*****************************************************************************/
class Subtitles : public Animation
{
	std::string			m_ScriptFile;
	DWORD				m_TextColor;
	Font*				m_pFont;
	DeviceStateSet*		m_pLinesDSS;
	Canvas*			m_pViewPort;
	bool				m_bDrawFrame;
	Rct					m_Extents;

	struct Phrase
	{
		Phrase() : beg(0.0f), end(-1.0f){}
		Phrase( float _beg, float _end, const char* _text ) : beg(_beg), end(_end), text(_text) {}
		float			beg;
		float			end;
		std::string		text;
	};

	std::vector<Phrase>	m_Phrases;

public:
						Subtitles();
						~Subtitles();
	
	virtual void		Render			();
	virtual void		Expose			( PropertyMap& pm );

	const char*			GetScriptFile	() const { return m_ScriptFile.c_str(); }
	void				SetScriptFile	( const char* fname );

	const char*			GetString		( float cTime );

	Rct					GetBounds		() const;
	
	float				GetX			() const;
	float				GetY			() const;
	float				GetW			() const;
	float				GetH			() const;

	void				SetX			( float val );
	void				SetY			( float val );
	void				SetW			( float val );
	void				SetH			( float val );

	bool				InitFromXML		( const char* fName );

	void				AddPhrase		( float beg, float end, const char* text );

	Font*				GetFont();

	NODE(Subtitles, Animation, SUBT);
}; // class Subtitles

}; // namespace sg

#endif // __UISUBTITLES_H__