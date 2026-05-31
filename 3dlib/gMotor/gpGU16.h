/*****************************************************************************/
/*	File:	gpGU16.h
/*	Desc:	.g16 uniform format treatment
/*	Author:	Ruslan Shestopalyuk
/*	Date:	27.02.2003
/*****************************************************************************/
#ifndef __GPGU16_H__
#define __GPGU16_H__

#include "GPPackage.h"

namespace sg{

/*****************************************************************
/*	Class:	GU16PackSegmentHeader
/*	Desc:	Uniformly packed segment
/*****************************************************************/
class GU16PackSegmentHeader
{
	//unsigned int		offset		:28		//  offset of the segment
	//PackMethods		packMethod	:4;		//  packing method
	DWORD				data;
public:
	_inl unsigned int GetOffset()		const { return data >> 4;	}
	_inl unsigned int GetPackFlags()	const { return data & 0xF;	}
};  // class GU16PackSegmentHeader

/*****************************************************************
/*	Struct:	GU16SpriteHeader
/*	Desc:	Uniformly packed sprite header
/*****************************************************************/
class GU16SpriteHeader
{
	WORD			data;			
public:
	WORD&			GetNChunks() { return data; } 
}; // struct GU16SpriteHeader 

/*****************************************************************************/
/*	Class:	GU16Seq
/*****************************************************************************/
class GU16Seq : public GPSeq
{
public:
	_inl GU16Seq( MediaFileLoader& mFile );

protected:
	bool				CreateFromMFile( MediaFileLoader& mFile );

	virtual int			GetPSegSize		( int segIdx ) const;
	virtual int			GetPSegFlags	( int segIdx ) const;
	virtual int			GetPSegFileOffset( int segIdx )const;
	virtual int			GetPSegNFrames	( int segIdx ) const;

private:
	BYTE				framesPerSegment;
	WORD				frameWidth;		
	WORD				frameHeight;
						
	GU16SpriteHeader*		sprDescr;		//  sprite description data
	GU16PackSegmentHeader*	PSDescr;		//  pack segment description data
	WORD					nPackSegments;

	int						guBlockSize;
}; // class GU16Seq

}; // namespace sg

#ifdef _INLINES
#include "gpGU16.inl"
#endif // _INLINES

#endif // __GPGU16_H__