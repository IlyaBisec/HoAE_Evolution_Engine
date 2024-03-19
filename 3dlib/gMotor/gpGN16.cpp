/*****************************************************************************/
/*	File:	gpGN16.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	03.03.2003
/*****************************************************************************/
#include "stdafx.h"

#include "kHash.hpp"
#include "kResource.h"
#include "gpGN16.h"

#ifndef _INLINES
#include "gpGN16.inl"
#endif // _INLINES

BEGIN_NAMESPACE(sg)

bool GN16Seq::CreateFromMFile( MediaFileLoader& mFile )
{
	SetType( gptNonUniform );	
	gnBlockSize = mFile.GetCurChunk().GetFullSize();
	mFile.Read( &nSprites,			sizeof( nSprites )			);
	mFile.Read( &maxWorkBufferSize,	sizeof( maxWorkBufferSize )	);
	mFile.Read( &nPackSegments,	sizeof( nPackSegments ) );
	
	PSDescr			= new GN16PackSegmentHeader	[nPackSegments]; 
	sprDescr		= new GN16SpriteHeader		[nSprites];
		
	mFile.Read( PSDescr,	nPackSegments * sizeof(GN16PackSegmentHeader) );
	mFile.Read( sprDescr,	nSprites * sizeof(GN16SpriteHeader) );
	
	frame.resize( nSprites );
	for (int i = 0; i < nSprites; i++)
	{
		frame[i].height			= sprDescr[i].GetFrameH();
		frame[i].width			= sprDescr[i].GetFrameW();
		frame[i].nChunks		= sprDescr[i].GetNumChunks();
		frame[i].packSegmentIdx = sprDescr[i].GetPackSegIdx();
		frame[i].lastFrameInst	= NULL;
	}
	
	mFile.CurChunkSkip();
	
	if (mFile.Error() != mfleNoError) return false;
	return true;
} // GN16Seq::CreateFromMFile

int GN16Seq::GetPSegSize( int segIdx ) const
{
	assert( segIdx >= 0 && segIdx < nPackSegments ); 
	if (segIdx == nPackSegments - 1) 
	{
		return gnBlockSize - PSDescr[segIdx].GetOffset();
	}
	return PSDescr[segIdx + 1].GetOffset() - PSDescr[segIdx].GetOffset();
}

int	GN16Seq::GetPSegFlags( int segIdx ) const
{
	assert( segIdx >= 0 && segIdx < nPackSegments ); 
	return PSDescr[segIdx].GetPackFlags();
}

int	GN16Seq::GetPSegFileOffset( int segIdx ) const
{
	assert( segIdx >= 0 && segIdx < nPackSegments ); 
	return PSDescr[segIdx].GetOffset();
}

int	GN16Seq::GetPSegNFrames( int segIdx ) const
{
	assert( segIdx >= 0 && segIdx < nPackSegments ); 
	return PSDescr[segIdx].GetNFrames();
}

END_NAMESPACE(sg)

	
