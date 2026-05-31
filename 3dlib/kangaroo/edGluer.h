/*****************************************************************************/
/*	File:	edGluer.h
/*	Author:	Ruslan Shestopalyuk
/*	Desc:	Glues sequence of pictures onto single picture
/*	Date:	15.04.2004
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include <direct.h>


/*****************************************************************************/
/*	Class:	Gluer
/*	Desc:	Glues sequence of pictures onto single picture
/*****************************************************************************/
class Gluer : public SNode
{
	DWORD				m_DTexID;
	std::string			m_SrcPath;
	std::string			m_DestPath;

	PowerOfTwo			m_DestW;
	PowerOfTwo			m_DestH;
	int					m_NRows;
	int					m_NCols;
	ColorFormat			m_DestCF;

public:
						Gluer		();
	void				Process		();
	virtual void		Expose		( PropertyMap& pm );

	DWORD				GetDestID	() const { return m_DTexID; }
	void				SetDestID	( DWORD val ) {}
	const char*			GetSrcPath	() const { return m_SrcPath.c_str(); }
	const char*			GetDestPath	() const { return m_DestPath.c_str(); }

	void				SetSrcPath	( const char* path );
	void				SetDestPath	( const char* path );

	DECLARE_SCLASS(Gluer, SNode, GLUE);
}; // class Gluer



