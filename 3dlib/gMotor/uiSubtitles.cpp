/*****************************************************************************/
/*	File:	uiSubtitles.h
/*	Author:	Ruslan Shestopalyuk
/*	Date:	15.04.2003
/*****************************************************************************/
#include "stdafx.h"
#include "uiControl.h"
#include "sgFont.h"
#include "sgController.h"
#include "uiSubtitles.h"
#include "sgApplication.h"

BEGIN_NAMESPACE(sg)

/*****************************************************************************/
/*	Subtitles implementation
/*****************************************************************************/
Subtitles::Subtitles()
{
	m_pFont			= NULL;
	m_pLinesDSS		= NULL;
	m_pViewPort		= NULL;

	m_TextColor		= 0xFFFFFFFF;
	m_bDrawFrame	= false;
	m_Extents.Set( 0.0f, 0.864865f, 1.0f, 1.0f - 0.864865f );
}

Subtitles::~Subtitles()
{
}

Rct	Subtitles::GetBounds() const
{
	return Rct( GetX(), GetY(), GetW(), GetH() );
}

float Subtitles::GetX() const
{
	return m_Extents.x * float( AppWindow::instance()->GetWidth() );
}

float Subtitles::GetY() const
{
	return m_Extents.y * float( AppWindow::instance()->GetHeight() );
}

float Subtitles::GetW() const
{
	return m_Extents.w * float( AppWindow::instance()->GetWidth() );
}

float Subtitles::GetH() const
{
	return m_Extents.h * float( AppWindow::instance()->GetHeight() );
}

void Subtitles::SetX( float val ) 
{
	m_Extents.x = val / float( AppWindow::instance()->GetWidth() );
}

void Subtitles::SetY( float val ) 
{
	m_Extents.y = val / float( AppWindow::instance()->GetHeight() );
}

void Subtitles::SetW( float val ) 
{
	m_Extents.w = val / float( AppWindow::instance()->GetWidth() );
}

void Subtitles::SetH( float val ) 
{
	m_Extents.h = val / float( AppWindow::instance()->GetHeight() );
}

Font* Subtitles::GetFont()
{
	if (m_pFont)
	{
		return m_pFont;
	}
	m_pFont = AddChild<Font>( "SubtitlesFont" );
	m_pFont->SetHeight( 20 );
	m_pFont->SetFontName( "Comic Sans MS" );
	m_pFont->SetIsBold( true );
	m_pFont->Generate();

	return m_pFont;
} // Subtitles::GetFont

const char* Subtitles::GetString( float cTime )
{
	for (int i = 0; i < m_Phrases.size(); i++)
	{
		Phrase& phrase = m_Phrases[i];
		if (phrase.beg <= cTime && phrase.end >= cTime) 
		{
			return phrase.text.c_str();
		}
	}
	return NULL;
}

void Subtitles::AddPhrase( float beg, float end, const char* text )
{
	m_Phrases.push_back( Phrase( beg, end, text ) );
} // Subtitles::AddPhrase

void Subtitles::Render()
{
	if (!m_pViewPort)
	{
		m_pViewPort = AddChild<Canvas>( "SubtitlesVP" );
		m_pViewPort->SetClearColor	( false ); 
		m_pViewPort->SetClearDepth	( false ); 
		m_pViewPort->SetClearStencil( false );
		m_pViewPort->SetExtents( 0.0f, 0.0f, 1.0f, 1.0f );
	}
	
	Rct vp = IRS->GetViewPort();
	
	m_pViewPort->Render();
	m_CurrentTime = Animation::CurTime() - GetStartTime();

	Font* pFont = GetFont();
	
	if (pFont)
	{
		const char* str = GetString( m_CurrentTime );
		if (!str) return;
		Rct srct( pFont->GetStringWidth( str ), pFont->GetHeight() );
		srct.CenterInto( GetBounds() );
		pFont->AddString( Vector3D( srct.x, srct.y, 0.0f ), str, m_TextColor );
	}
	
	if (m_bDrawFrame)
	{
		if (!m_pLinesDSS) m_pLinesDSS = AddChild<DeviceStateSet>( "lines3d_blend" );
		m_pLinesDSS->Render();
		
		rsFrame( GetBounds(), 0.0f, m_TextColor );
		rsFlushLines2D();
	}
	Parent::Render();
} // Subtitles::Render

bool Subtitles::InitFromXML( const char* fName )
{
	return false;
} // Subtitles::InitFromXML

void Subtitles::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "Subtitles", this );
	pm.p( "ScriptFile",	GetScriptFile, SetScriptFile, "file" );
	pm.f( "TextColor",	m_TextColor, "color" );
	pm.f( "DrawFrame",	m_bDrawFrame );
	pm.f( "Left",			m_Extents.x );
	pm.f( "Top",			m_Extents.y );
	pm.f( "Width",		m_Extents.w );
	pm.f( "Height",		m_Extents.h );

} // Subtitles::Expose

void Subtitles::SetScriptFile( const char* fname )
{
	m_ScriptFile = fname;
} // Subtitles::SetScriptFile

END_NAMESPACE(sg)
