/*****************************************************************************/
/*	File:	gpPackage.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	27.02.2003
/*****************************************************************************/
#include "stdafx.h"

#include "kHash.hpp"
#include "kResource.h"
#include "gpGN16.h"
#include "gpGU16.h"

#include "fexPack.h"
#include "kUtilities.h"

#include "sg.h"
#include "sgGPSystem.h"
#include "gpPackage.h"

#ifndef _INLINES
#include "gpPackage.inl"
#endif // !_INLINES

BEGIN_NAMESPACE(sg)

DWORD		GPFrame::curCacheFactor = 0;

/*****************************************************************
/*	GPSeq implementation
/*****************************************************************/
BYTE*						GPSeq::s_WorkBuffer			= NULL;
DWORD						GPSeq::s_WorkBufferSize		= 0;

GPSeq::GPPixelDataMgr		GPSeq::s_PixDataMgr;
GPSeq::GPPackedSegmentMgr	GPSeq::s_PackSegMgr;
GPPixelCache				GPSeq::s_PixelCache;

GPSeq::GPSeq()
{
	memset( this, 0, sizeof( *this ) );
}

GPSeq::~GPSeq()
{
	Cleanup();
}

void GPSeq::Cleanup()
{
	//todo( "GPSeq::Cleanup" );
	for (int i = 0; i < c_MaxGPPalettes; i++)
	{
		delete []palette[i];
	}
}

void GPSeq::SetFrameOffsets( DWORD* offsets, int segIdx, int nFrames )
{
	assert( nFrames == GetPSegNFrames( segIdx ) );
	int firstSprite = 0;
	while (segIdx > 0) 
	{ 
		firstSprite += GetPSegNFrames( segIdx - 1 ); 
		segIdx--; 
	}
	for (int i = 0; i < nFrames; i++)
	{
		frame[i + firstSprite].offsInPixelSegment = offsets[i];
	}
} // GPSeq::SetFrameOffsets

/*---------------------------------------------------------------------------*/
/*	Func:	GPSeq::CreateHeaderFromFile	
/*---------------------------------------------------------------------------*/
GPSeq* GPSeq::CreateHeaderFromFile( const char* fname )
{
	MediaFileLoader mFile( fname );
	if (mFile.Error() != mfleNoError) 
	{
		Log.Warning( "Could not load sprite package <%s>", fname );
		return NULL;
	}

	GPSeq* result = 0;

	while (mFile.NextChunk())
	{
		switch (MAGIC_FOURCC( mFile.GetCurChunk().GetMagicDW() ))
		{
		case 'GU16': 
			{
				result = new GU16Seq( mFile );
			}break;
		case 'GN16':
			{
				result = new GN16Seq( mFile );
			}break;
		case 'GPAL':
			{
				if (!result) 
				{
					mFile.CurChunkSkip();
					break;
				}
				//  some hacky code here...
				result->AllocatePalette( 0, 1024 );
				result->AllocatePalette( 1, 1024 );
				
				//  skip all useful info in the palette block header
				mFile.CurChunkSkip( 10 );
				//  skip all useful info in the 1st palette header
				mFile.CurChunkSkip( 8 );
				mFile.Read( result->GetPalette( 0 ), 1024 );
				//  skip all useful info in the 2nd palette header
				mFile.CurChunkSkip( 8 );
				mFile.Read( result->GetPalette( 1 ), 1024 );
			}
			break;
		case 'GINF':
			{
				if (!result) 
				{
					mFile.CurChunkSkip();
					break;
				}
				//  skip all megauseful stuff ;)
				//		DWORD	UnpackedSize;		// size for unpack
				//		WORD	Flags;				// flags (pack method)
				//		WORD	Reserved;			// reserved (zero)
				//
				//		DWORD	SizeOf;				// total size of the item in bytes
				//		WORD	Flags;				// flags
				mFile.CurChunkSkip( 16 );
				//  read numDirections
				mFile.Read( &result->numDirections, sizeof( result->numDirections ) ); 
				mFile.CurChunkSkip();
			}
			break;
		default:
			Log.Error( "Unsupported GP file data chunk (%s) in file %s", 
						mFile.GetCurChunk().GetMagic(), fname );
			mFile.CurChunkSkip();
			return false;
		}
	}
	
	if (!result)
	{
		Log.Error( "Unknown .g16 format in file <%s>", fname );
	}
	
	if (result) result->SetPath( fname );
	return result;
};  //  GPSeq::CreateHeaderFromFile

GPPackSegment* GPSeq::GetPackSegment( GPSeq* seq, int segIdx )
{
	int psID = s_PackSegMgr.FindResource( GPPackSegment::Key( seq->GetID(), segIdx ) );
	GPPackSegment* pSeg = 0;
	if (psID == NO_ELEMENT)
	{
		pSeg = new GPPackSegment( seq, segIdx );
		pSeg->InitPrefix();
		psID = s_PackSegMgr.InsertResource( pSeg, GPPackSegment::Key( seq->GetID(), segIdx ) );
	}
	assert( psID != NO_ELEMENT );
	pSeg = s_PackSegMgr.GetResource( psID ); 
	return pSeg;
} // GPSeq::GetPackSegmentInstance

GPPixelData*	GPSeq::GetPixelData( GPSeq* seq, int segIdx, DWORD color )
{
	GPPixelData::Key key( seq->GetID(), segIdx, color );
	int pdID = s_PixDataMgr.FindResource( key );
	GPPixelData* pPixData = 0;
	if (pdID == NO_ELEMENT)
	{
		pPixData = new GPPixelData( seq, segIdx, color );
		pPixData->InitPrefix();
		pdID = s_PixDataMgr.InsertResource( pPixData, key );
	}
	assert( pdID != NO_ELEMENT );
	pPixData = s_PixDataMgr.GetResource( pdID ); 
	return pPixData;
} // GPSeq::GetPixelData

/*****************************************************************************/
/*	GPPixelData implementation
/*****************************************************************************/
DWORD GPPixelData::s_FrameOffsetArray[c_MaxFramesInPackedSegment];
GPPixelData::~GPPixelData()
{
	delete []pData;
}

bool GPPixelData::InitPrefix()
{
	assert( pSeq );
	return true;
} // GPPixelData::InitPrefix
	
bool GPPixelData::Restore()
{
	assert( pSeq && !pData );
	//  get pointer to the packed data
	GPPackSegment* packSeg = GPSeq::GetPackSegment( pSeq, segIdx );
	BYTE* packed = packSeg->GetData();
	assert( packed );
	
	G16SetPalette( pSeq->GetPalette( 0 ), pSeq->GetPalette( 1 ) );
	G16SetNationalColor( (color & 0x00FF0000) >> 16, 
						 (color & 0x0000FF00) >> 8, 
						 color & 0x000000FF );
	pSeq->AdjustWorkBuffer();

	assert( !pData );
	dataSize = packSeg->GetUnpackedDataSize();
	pData = new BYTE[dataSize];

	//  unpack data
	int nFrames = packSeg->GetNFrames();
	bool res = G16UnpackSegment(	packed, packSeg->GetDataSize() - 4, 
									pData, 
									pSeq->GetWorkBuf(), 
									(unsigned int*)s_FrameOffsetArray, 
									nFrames, 
									packSeg->GetPackFlags() );
	if (!res)
	{
		Log.Error( "Could not unpack pixel data in file %s", pSeq->GetPath() );
		return false;
	}
	pSeq->SetFrameOffsets( s_FrameOffsetArray, segIdx, nFrames );
	dismissed = false;
	return true;
} // GPPixelData::Restore

void GPPixelData::Dismiss()
{
	delete []pData;
	pData		= 0;
	dismissed	= true;
} // GPPixelData::Dismiss

int GPPixelData::GetDismissableSizeBytes() const
{
	return dataSize;
}

int	GPPixelData::GetHeaderSizeBytes() const
{
	return sizeof( *this );
}

/*****************************************************************************/
/*	GPPackSegment implementation
/*****************************************************************************/
GPPackSegment::~GPPackSegment()
{
	delete []pData;
}

bool GPPackSegment::Restore()
{
	assert( !pData );
	pData = new BYTE[dataSize];
	ResFile rf = RReset( pSeq->GetPath() );
	if (rf == INVALID_HANDLE_VALUE) return false;
	RSeek( rf, offsetFromFileBegin );
	RBlockRead( rf, pData, dataSize );
	RClose( rf );

	unpackedDataSize	= *((DWORD*)pData);
	dismissed			= false;
	return true;
} // GPPackSegment::Restore

bool GPPackSegment::InitPrefix()
{
	assert( pSeq );
	dataSize			= pSeq->GetPSegSize			( segIdx );
	packFlags			= pSeq->GetPSegFlags		( segIdx );
	offsetFromFileBegin = pSeq->GetPSegFileOffset	( segIdx );
	nFrames				= pSeq->GetPSegNFrames		( segIdx );
	return true;
} // GPPackSegment::InitPrefix

void GPPackSegment::Dismiss()
{
	delete []pData;
	pData = 0;
	dismissed = true;
}

int	GPPackSegment::GetDismissableSizeBytes() const
{
	return dataSize;
}

int	GPPackSegment::GetHeaderSizeBytes() const
{
	return sizeof( *this );
}

/*****************************************************************************/
/*	GPFrameInst implementation
/*****************************************************************************/
int	GPFrameInst::GetDismissableSizeBytes() const
{
	int nBytes = 0;
	for (int i = 0; i < chunk.size(); i++)
	{
		const GPChunk* curChunk = &(chunk[i]);
		nBytes += curChunk->EstimateSizeBytes();
	}
	return nBytes;
} // GPFrameInst::GetDismissableSizeBytes

int	GPFrameInst::GetHeaderSizeBytes() const
{
	return sizeof( *this ) + chunk.size_bytes();
}
	
bool GPFrameInst::InitPrefix()
{
	Hit( curCacheFactor );
	pixData	= GPSeq::GetPixelData( pSeq, pFrame->packSegmentIdx, color );
	BYTE* pChunkH = GetPixelData();
	assert( pChunkH );
	int nChunks = pFrame->nChunks;
	SetNChunks( nChunks );

	for (int i = 0; i < nChunks; i++)
	{
		GPChunk& cChunk = GetChunk( i );
		GPChunkHeader* chdr = reinterpret_cast<GPChunkHeader*>( pChunkH );
		cChunk.side = (float) chdr->GetSide();
		cChunk.x	= (float) chdr->GetX();
		cChunk.y	= (float) chdr->GetY();
		pChunkH += chdr->GetSizeBytes();
	}
	return true;
} // GPFrameInst::InitPrefix

void GPFrameInst::Dismiss()
{
	for (int i = 0; i < chunk.size(); i++)
	{
		GPSeq::s_PixelCache.DeleteClient( 
			chunk[i].GetSurfID(), 
			chunk[i].GetPosInSurfClientList() 
			);
	}
	dismissed = true;
}

void GPFrameInst::Dump()
{
	Log.Info( "GPFrameInst, gpID:%d(%s) sprID:%d color:%X", 
					gpID, pSeq->GetPath(), sprID, color );
}

bool GPFrameInst::GetLeftTop( int& left, int& top ) const
{
	int nChunks = chunk.size();
	if (nChunks == 0) return false;
	const GPChunk& fChunk = GetChunk( 0 );
	left = fChunk.x;
	top  = fChunk.y;

	for (int i = 1; i < nChunks; i++)
	{
		const GPChunk& cChunk = GetChunk( i );
		if (cChunk.x < left)	left = cChunk.x;
		if (cChunk.y < top)		top  = cChunk.y;
	}	
	return true;
} // GPFrameInst::GetLeftTop

bool GPFrameInst::Restore()
{
	bool res = GPSeq::s_PixelCache.MovePixelsToCache( this );
	//  set in the generic frame object reference to my instance, as most recently restored.
	//  this can be used in cases when only alpha value is needed by GPSystem despite the color
	if (!res) return false;
	pFrame->lastFrameInst = this;
	assert( res );
	dismissed = false;
	return true;
} // GPFrameInst::Restore

/*****************************************************************************/
/*	GPPixelCache implementation
/*****************************************************************************/
GPPixelCache::GPPixelCache() : pGPS(0)
{
	m_DismissCandidate = 0;
}

void GPPixelCache::Drop( GPFrameInst*	frameInst )
{
	if (pGPS) pGPS->Drop( frameInst );
}

void GPPixelCache::Init( IRenderSystem* iRS, GPSystem* _pGPS )
{
	float side = static_cast<float>( c_GPTexSide ); 
	for (int i = 0; i <= c_GPTexSide; i++)
	{
		uvTableL[i] = (static_cast<float>( i ) + 0.0f)/(side);
		uvTableU[i] = (static_cast<float>( i ) - 0.0f)/(side);
	}

	//  init sprite surfaces
	TextureDescr spriteTD;
	spriteTD.setValues( c_GPTexSide, 
						c_GPTexSide,
                        c_SpriteColorFormat, 
						mpManaged, 
						1,
						tuProcedural );
	
	TextureDescr workTexTD;
	workTexTD.copy( spriteTD );
	workTexTD.setMemPool( mpSysMem );

	//  texture cache surfaces
	char texName[64];
	int gpTexNum = _pGPS->GetNumSurfaces();
	item.reserve( gpTexNum );
	for (int i = 0; i < gpTexNum; i++)
	{
		GPPixelCacheItem cItem;
		sprintf( texName, "gpSurf%d", i );
		cItem.layout.Init( c_GPTexSide );
		cItem.surfTexID	= iRS->CreateTexture( texName, spriteTD, &(item[i]) );
		cItem.ID		= i;
		item.push_back( cItem );
	}
	
	curItem = 0;
	pGPS = _pGPS;
} // GPPixelCache::Init

int	GPPixelCache::AllocCacheItem( BaseResource*	client )
{
	int res = GetFreeCacheItem();
	if (res == -1) 
	{
		Flush();
		res = GetFreeCacheItem();
		assert( res != -1 );
	}
	WORD sx, sy;
	item[res].layout.AllocChunk( c_GPTexSidePow, sx, sy );
	item[res].AddClient( client );
	return res;
} // GPPixelCache::AllocCacheItem

void GPPixelCache::FreeItem( int itemID )
{
	item[itemID].ResetClients();
}	

int	GPPixelCache::CalcNDupes()
{
	int nItems = item.size();
	for (int i = 0; i < nItems; i++)
	{
		GPPixelCacheItem& it = item[i];
	}
	assert( false );
	return 0;
} // GPPixelCache::CalcNDupes

int	GPPixelCache::GetFreeCacheItem()
{
	int freeItem;
	do{
		freeItem = m_DismissCandidate;
		GPPixelCacheItem& ditem = item[m_DismissCandidate];
		m_DismissCandidate = (m_DismissCandidate + 1) % item.size();
		if (ditem.HasLockedClients()) continue;

		pGPS->FlushSpriteBucket( ditem.ID );
		ditem.ResetClients();
		ditem.layout.Free();
		return freeItem;
	} while( true );
	return -1;
} // GPPixelCache::GetSurface

void  GPPixelCache::OnFrame()
{
}

void GPPixelCache::Dump()
{
	char name[64];
	for (int i = 0; i < item.size(); i++)
	{
		sprintf( name, "c:\\dumps\\it%03d.dds", item[i].surfTexID );
		RC::iRS->SaveTexture( item[i].surfTexID, name );
		Log.Info( "Item %d", i );
		item[i].Dump();
	}
}	

void GPPixelCache::Flush()
{
	assert( pGPS );
	pGPS->FlushBatches();
} // GPPixelCache::Flush

bool GPPixelCache::MovePixelsToCache( GPFrameInst* frameInst )
{
	BYTE* pChunk = frameInst->GetPixelData();
	int nChunks = frameInst->chunk.size();
	
	if (curItem == 0)
	{
		int newItemID = GetFreeCacheItem();
		curItem = &(item[newItemID]);
	}
	
	bool thrashed = false;


	frameInst->Lock();

	//  iterate on chunks
	for (int i = 0; i < nChunks; i++)
	{
		GPChunkHeader*	pChunkHdr = (GPChunkHeader*) pChunk;
		GPChunk&		chunk	  = frameInst->chunk[i];
		int				sidePow	  = pChunkHdr->GetSidePow() - frameInst->lod;
		
		//  allocate chunk on the surface	
		WORD ax, ay;
		bool res = curItem->layout.AllocChunk( sidePow, ax, ay ); 
		const int maxAllocTries = 16; 
		if (!res) 
		//  no more place on the current texture
		{
			//  pick surfaces till we find one with some place
			int nTries = 0;
			const int maxAllocTries = 4;//item.size() >> 1;
			while (!res)
			{
				curItem = &item[(curItem->ID + 1)%item.size()];
				nTries++;
				//  tries limit reached
				if (nTries >= maxAllocTries) 
				{
					int newItemID = GetFreeCacheItem();
					curItem = &(item[newItemID]);
					nTries = 0;
				}
				thrashed = true;
				res = curItem->layout.AllocChunk( sidePow, ax, ay );
			}
			assert( res );
		}
		
		int srcPitch = pChunkHdr->GetSide() << 1;
		int side	 = pChunkHdr->GetSide() >> frameInst->lod;
		
		//  set chunk properties
		chunk.u			= uvTableL[ax];
		chunk.v			= uvTableL[ay];
		chunk.u2		= uvTableU[ax + side];
		chunk.v2		= uvTableU[ay + side];

		chunk.SetSurfID( curItem->ID );
		chunk.SetPosInSurfClientList( curItem->AddClient( frameInst ) );

		//  copy chunk pixel data to the working surface
		int		pitch		= 0;
		int		sideBytes	= side << 1;
		BYTE*	pCh			= pChunkHdr->GetPixelData();
		BYTE*	pB			= RC::iRS->LockTexBits( curItem->surfTexID, pitch );
		pB += ay * pitch + (ax << 1);
		if (frameInst->lod == 0)
		{
			util::MemcpyRect(	pB, pitch,
								pChunkHdr->GetPixelData(), srcPitch, 
								side, sideBytes );
		}
		else if (frameInst->lod == 1)
		{
			util::QuadShrink2xW( pB, pitch,
								 pChunkHdr->GetPixelData(),	srcPitch, 
								 pChunkHdr->GetSide() );
		}
		else if (frameInst->lod == 2)
		{
			util::QuadShrink4xW( pB, pitch, 
								 pChunkHdr->GetPixelData(), srcPitch,
								 pChunkHdr->GetSide() );
		}
			
		RC::iRS->UnlockTexBits( curItem->surfTexID );

		pChunk += pChunkHdr->GetSizeBytes();
	}

	frameInst->Unlock();
	return true;
} // GPPixelCache::MoveChunkToCache

/*****************************************************************************/
/*	GPPixelCacheItem implementation
/*****************************************************************************/
void GPPixelCacheItem::Invalidate( TextureDescr* texture )
{
	ResetClients();
} // GPPixelCacheItem::Invalidate

void GPPixelCacheItem::Restore( TextureDescr* texture )
{
}

void GPPixelCacheItem::Dump()
{
	Log.Info( "Num Clients: %d", client.size() );
	for (int i = 0; i < client.size(); i++)
	{
		if (client[i]) client[i]->Dump();
	}
} // GPPixelCacheItem::Dump

END_NAMESPACE(sg)





