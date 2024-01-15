/*****************************************************************************/
/*	File:	gpGN16.h
/*	Desc:	.g16 non-uniform format treatment
/*	Author:	Ruslan Shestopalyuk
/*	Date:	27.02.2003
/*****************************************************************************/
#ifndef __GPGN16_H__
#define __GPGN16_H__

#include "GPPackage.h"

namespace sg{

/*****************************************************************
/*	Class:	GN16PackSegmentHeader
/*	Desc:	Non-uniformly packed segment header
/*****************************************************************/
class GN16PackSegmentHeader
{
	//unsigned int		offset		:28;	//  offset of the segment
	//PackMethods		packMethod	:4;		//  packing method
	//WORD				firstSprite;
	BYTE				data[6];
public:
	unsigned int	GetOffset()			const	{ return (*((DWORD*)data)) >> 4;	}
	unsigned int	GetPackFlags()		const	{ return (*((DWORD*)data)) & 0xF;	}
	WORD&			GetNFrames()				{ return *((WORD*)(&data[4]));		}
}; // class GN16PackSegmentHeader

/*****************************************************************
/*	Class:	GN16SpriteHeader
/*	Desc:	Non-uniformly sprite header
/*****************************************************************/
class GN16SpriteHeader
{
	BYTE			data[8];

public:
	WORD&			GetNumChunks()	{ return *((WORD*)(&data[0])); }
	WORD&			GetFrameW()		{ return *((WORD*)(&data[2])); }
	WORD&			GetFrameH()		{ return *((WORD*)(&data[4])); }				
	WORD&			GetPackSegIdx()	{ return *((WORD*)(&data[6])); }				
}; // class GN16SpriteHeader 

/*****************************************************************************/
/*	Class:	GN16Seq
/*****************************************************************************/
class GN16Seq : public GPSeq
{
public:
	_inl GN16Seq( MediaFileLoader& mFile );

	virtual int			GetPSegSize		( int segIdx ) const;
	virtual int			GetPSegFlags	( int segIdx ) const;
	virtual int			GetPSegFileOffset( int segIdx )const;
	virtual int			GetPSegNFrames	( int segIdx ) const;
	

protected:
	bool				CreateFromMFile( MediaFileLoader& mFile );

private:
	
	GN16SpriteHeader*		sprDescr;		//  sprite description data
	GN16PackSegmentHeader*	PSDescr;		//  pack segment description data
	WORD					nPackSegments;

	int						gnBlockSize;
}; // class GN16Seq

}; // namespace sg

#ifdef _INLINES
#include "gpGN16.inl"
#endif // _INLINES

#endif // __GPGN16_H__