/*****************************************************************************/
/*	File:	gpGU16.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	03.03.2003
/*****************************************************************************/
#include "stdafx.h"

#include "kHash.hpp"
#include "kResource.h"
#include "gpGU16.h"

#ifndef _INLINES
#include "gpGU16.inl"
#endif // _INLINES

BEGIN_NAMESPACE(sg)

bool GU16Seq::CreateFromMFile( MediaFileLoader& mFile )
{
	guBlockSize = mFile.GetCurChunk().GetFullSize();
	mFile.Read( &framesPerSegment,	sizeof( framesPerSegment )	);
	mFile.Read( &nSprites,			sizeof( nSprites )			);
	mFile.Read( &frameWidth,		sizeof( frameWidth )		);
	mFile.Read( &frameHeight,		sizeof( frameHeight )		);
	mFile.Read( &maxWorkBufferSize,	sizeof( maxWorkBufferSize )	);
	mFile.Read( &nPackSegments,		sizeof( nPackSegments ) );
		
	PSDescr			= new GU16PackSegmentHeader	[nPackSegments]; 
	sprDescr		= new GU16SpriteHeader		[nSprites];
		
	mFile.Read( PSDescr,	nPackSegments * sizeof(GU16PackSegmentHeader)	);
	mFile.Read( sprDescr,	nSprites * sizeof(GU16SpriteHeader)				);
	
	mFile.CurChunkSkip();

	frame.resize( nSprites );
	for (int i = 0; i < nSprites; i++)
	{
		frame[i].height			= frameHeight;
		frame[i].width			= frameWidth;
		frame[i].nChunks		= sprDescr[i].GetNChunks();
		frame[i].packSegmentIdx = i / framesPerSegment;
		frame[i].lastFrameInst	= NULL;
	}

	if (mFile.Error() != mfleNoError) return false;
	return true;
} // GU16Seq::CreateFromMFile

int	GU16Seq::GetPSegSize( int segIdx ) const
{
	assert( segIdx >= 0 && segIdx < nPackSegments ); 
	if (segIdx == nPackSegments - 1) 
	{
		return guBlockSize - PSDescr[segIdx].GetOffset();
	}
	return PSDescr[segIdx + 1].GetOffset() - PSDescr[segIdx].GetOffset();
}

int	GU16Seq::GetPSegFlags( int segIdx ) const
{
	assert( segIdx >= 0 && segIdx < nPackSegments ); 
	return PSDescr[segIdx].GetPackFlags();
}

int	GU16Seq::GetPSegFileOffset( int segIdx ) const
{
	assert( segIdx >= 0 && segIdx < nPackSegments ); 
	return PSDescr[segIdx].GetOffset();
}

int	GU16Seq::GetPSegNFrames( int segIdx ) const
{
	assert( segIdx >= 0 && segIdx < nPackSegments ); 
	if (segIdx == nPackSegments - 1) return GetNFrames() - segIdx * framesPerSegment;
	return framesPerSegment;
}

END_NAMESPACE(sg)
