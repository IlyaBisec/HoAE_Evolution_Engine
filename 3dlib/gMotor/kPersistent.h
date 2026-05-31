/*****************************************************************************/
/*	File:	kPersistent.h
/*	Desc:	Persistence (saving/loading) of the data structures
/*	Author:	Ruslan Shestopalyuk
/*	Date:	23.05.2003
/*****************************************************************************/
#ifndef __KPERSISTENT_H__
#define __KPERSISTENT_H__

/*****************************************************************************/
/*	Class:	stcio
/*	Desc:	io wrapper class for standard types
/*****************************************************************************/
class stcio
{
public:
	_inl static int read( BYTE* buf, int& val );
	_inl static int read( FILE* fp,  int& val );

	_inl static int read( BYTE* buf, float& val );
	_inl static int read( FILE* fp,  float& val );
	
	_inl static int read( BYTE* buf, DWORD& val );
	_inl static int read( FILE* fp,  DWORD& val );

	_inl static int read( BYTE* buf, char* str, int numCh );
	_inl static int read( FILE* fp,  char* str, int numCh );

	
	_inl static int write( BYTE* buf, int& val );
	_inl static int write( FILE* fp,  int& val );
	
	_inl static int write( BYTE* buf, float& val );
	_inl static int write( FILE* fp,  float& val );
	
	_inl static int write( BYTE* buf, DWORD& val );
	_inl static int write( FILE* fp,  DWORD& val );

	_inl static int write( BYTE* buf, char* str );
	_inl static int write( FILE* fp,  char* str );

}; // class stcio

/*****************************************************************************/
/*	Class:	MagicWord
/*	Desc:	Stays for magic FOURCC id
/*****************************************************************************/
class MagicWord
{
	BYTE			mb[4];
public:
	_inl			MagicWord( const char* wstr );

	_inl int		write	( BYTE* buf );
	_inl int		write	( FILE* fp );
	_inl int		read	( BYTE* buf );
	_inl int		read	( FILE* fp );
	_inl int		size	() const { return sizeof(DWORD); }

}; // class MagicWord

#ifdef _INLINES
#include "kPersistent.inl"
#endif // _INLINES

#endif // __KPERSISTENT_H__