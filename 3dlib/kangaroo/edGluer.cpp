/*****************************************************************************/
/*	File:	edGluer.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Desc:	Glues sequence of pictures onto single picture
/*	Date:	15.04.2004
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include <direct.h>
#include "edGluer.h"
#include "kDirIterator.h"



/*****************************************************************************/
/*	Gluer implementation
/*****************************************************************************/
Gluer::Gluer()
{
	m_DTexID	= -1;
	m_DestW		= pow8;
	m_DestH		= pow8;
	m_NRows		= 4;
	m_NCols		= 4;
	m_DestCF	= cfARGB8888;
}

void Gluer::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "Gluer", this );
	pm.p( "Source",			GetSrcPath,	 SetSrcPath,	"file"		);
	pm.p( "Destination",	GetDestPath, SetDestPath,	"file"		);
	pm.p( "Result",			GetDestID,	 SetDestID,		"texture"	);
	pm.f( "Width",			m_DestW		);
	pm.f( "Height",			m_DestH		);
	pm.f( "Rows",			m_NRows		);
	pm.f( "Columns",		m_NCols 	);
	pm.f( "ColorFormat",	m_DestCF	);
	pm.m( "Glue!",			Process		);
}

void Gluer::SetSrcPath( const char* path )
{
	m_SrcPath = path;
}

void Gluer::SetDestPath( const char* path )
{
	m_DestPath = path;
}

void Gluer::Process()
{
	IRS->DeleteTexture( m_DTexID );
	
	/*TextureDescr td;
	td.setValues( m_DestW, m_DestH, m_DestCF, mpManaged, 1, tuProcedural );
	m_DTexID = IRS->CreateTexture( "GluerDest", td );

	Rct ext;
	
	ext.w = m_DestW / m_NCols;
	ext.h = m_DestW / m_NRows;

	DirIterator dit( m_SrcPath.c_str() );
	int cFrame = 0;
	while (dit)
	{
		int texID = IRS->LoadTexture( dit.GetFullFilePath() );
		if (texID > 0)
		{
			ext.x = (cFrame % m_NCols)*ext.w;
			ext.y = (cFrame / m_NCols)*ext.h;
			if (!sd) continue;
			Rct srcExt( 0, 0, IRS->GetTextureWidth(), IRS->GetTextureHeight() );
			IRS->CopyTexture( m_DTexID, ext, texID, srcExt );
			cFrame++;
			IRS->DeleteTexture( texID );
		}
		++dit;
	}*/
	IRS->SaveTexture( m_DTexID, m_DestPath.c_str() );
} // Gluer::Process




