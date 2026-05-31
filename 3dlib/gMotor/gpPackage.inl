/*****************************************************************************/
/*	File:	gpPackage.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	27.02.2003
/*****************************************************************************/

BEGIN_NAMESPACE(sg)
/*****************************************************************
/*	GPSeq implementation
/*****************************************************************/
_inl int GPSeq::GetNDirections() const
{
	return numDirections;
}

_inl BYTE* GPSeq::GetWorkBuf() const
{
	assert( s_WorkBuffer );
	return s_WorkBuffer;
}	

_inl int GPSeq::GetNFrames() const
{
	return nSprites;
} // GPSeq::getNSprites

_inl GPFrame* GPSeq::GetFrame( int fidx )
{
	if (fidx >= nSprites) return NULL;
	return &(frame[fidx]);
}

_inl GPType	GPSeq::GetType() const
{
	return type;
}

_inl void GPSeq::SetType( GPType _type ) 
{
	type = _type;
}

_inl bool GPSeq::AdjustWorkBuffer()
{
	if (s_WorkBufferSize < maxWorkBufferSize)
	{
		delete []s_WorkBuffer;
		s_WorkBuffer = new BYTE[maxWorkBufferSize];
		s_WorkBufferSize = maxWorkBufferSize;
		return true;
	}
	return false;
} // GPSeq::AdjustWorkBuffer

_inl void GPSeq::AllocatePalette( int palIdx, int size )
{
	assert( palIdx < c_MaxGPPalettes );
	if (paletteSize[palIdx] == size) return;
	delete [](palette[palIdx]);
	palette[palIdx] = new BYTE[size];
	paletteSize[palIdx] = size;
} // GPSeq::AllocatePalette

_inl BYTE* GPSeq::GetPalette( int palIdx ) const
{
	assert( palIdx < c_MaxGPPalettes );
	return palette[palIdx];
} // GPSeq::getPalette

_inl const char* GPSeq::GetName() const
{
	return name.c_str();
}

_inl void GPSeq::SetName( const char* _name )
{
	name = static_cast<c2::string>( _name );
}

_inl const char* GPSeq::GetPath() const
{
	return path.c_str();
}

_inl void GPSeq::SetPath( const char* _path )
{
	path = static_cast<c2::string>( _path );
}

_inl int GPSeq::GetID()	const
{
	return id;
}

_inl void GPSeq::SetID( int gpID )
{
	id = gpID;
}

/*****************************************************************
/*	GPChunkHeader inplementation
/*****************************************************************/
_inl int GPChunkHeader::GetSidePow() const
{
	return (*((DWORD*)this)) >> 28;
}

_inl int GPChunkHeader::GetSide() const
{
	return 1 << GetSidePow();
}

_inl int GPChunkHeader::GetX() const
{
	DWORD res = ((*((DWORD*)this)) >> 12) & 0xFFF;
	if (res & 0x800) res |= 0xFFFFF000;
	return res;
}
 
_inl int GPChunkHeader::GetY() const
{
	DWORD res = (*((DWORD*)this)) & 0xFFF;
	if (res & 0x800) res |= 0xFFFFF000;
	return res;
}

#define FGA_POLY_CONF_ALL    0        
#define FGA_POLY_CONF_LU     1      
#define FGA_POLY_CONF_RB     2       
#define FGA_POLY_CONF_RU     3        
#define FGA_POLY_CONF_LB     4      

_inl bool GPChunkHeader::IsFull() const
{
	return (((*((DWORD*)this)) >> 24) & 0xF) == FGA_POLY_CONF_ALL;
}

_inl bool GPChunkHeader::IsLU() const
{
	return (((*((DWORD*)this)) >> 24) & 0xF) == FGA_POLY_CONF_LU;
}

_inl bool GPChunkHeader::IsLB() const
{
	return (((*((DWORD*)this)) >> 24) & 0xF) == FGA_POLY_CONF_LB;
}

_inl bool GPChunkHeader::IsRU() const
{
	return (((*((DWORD*)this)) >> 24) & 0xF) == FGA_POLY_CONF_RU;
}

_inl bool GPChunkHeader::IsRB() const
{
	return (((*((DWORD*)this)) >> 24) & 0xF) == FGA_POLY_CONF_RB;
}

_inl BYTE* GPChunkHeader::GetPixelData() const
{
	return (BYTE*)(this + 1);
}

_inl int GPChunkHeader::GetSizeBytes() const
{
	return sizeof(*this) + ((1 << (GetSidePow() << 1)) << 1);
}

/*****************************************************************************/
/*	GPFrameInst implementation
/*****************************************************************************/
_inl unsigned int GPFrameInst::Key::hash() const
{
	static const DWORD c_Prime0		= 127;
	static const DWORD c_Prime01	= 251;
	static const DWORD c_Prime02	= 379;
	static const DWORD c_Prime03	= 541;
	static const DWORD c_Prime1		= 15731;
	static const DWORD c_Prime2		= 789221;
	static const DWORD c_Prime3		= 1376312589;
	
	DWORD h = gpID +	sprID	* c_Prime0  - 
						color	* c_Prime01 + 
						lod		* c_Prime02 + 
								  c_Prime03;
	h = (h << 13) ^ h;
	h *= h * h * c_Prime1 + c_Prime2;
	h += c_Prime3;
	h ^= h >> 16;
	h ^= h >> 8;
	return h;
} // GPFrameInst::hash

_inl int GPFrameInst::GetSeqID()	const 
{ 
	return pSeq->GetID();
}

_inl int GPFrameInst::GetFrameID()	const 
{ 
	return sprID;		
}

_inl GPChunk& GPFrameInst::GetChunk( int idx )		
{ 
	return chunk[idx];	
} // GPFrameInst::GetChunk

_inl const GPChunk& GPFrameInst::GetChunk( int idx ) const
{
	return chunk[idx];
}

_inl DWORD GPFrameInst::GetColor() const
{
	return color;
} // GPFrameInst::GetColor

_inl BYTE* GPFrameInst::GetPixelData() const
{
	if (!pixData) const_cast<GPFrameInst*>( this )->InitPrefix();
	return pixData->GetData() + pFrame->offsInPixelSegment;
} // GPFrameInst::GetPixelData

_inl int GPFrameInst::GetNChunks() const
{
	return chunk.size();
}

_inl void GPFrameInst::SetNChunks( int nChunks, void* buf )
{ 
	chunk.resize( nChunks, (GPChunk*)buf ); 
}

_inl void GPFrameInst::OnDrop()
{
	GPSeq::s_PixelCache.Drop( this );
}

/*****************************************************************************/
/*	GPPixelData implementation
/*****************************************************************************/
_inl GPPixelData::GPPixelData( GPSeq* seq, int segmentIdx, DWORD playerColor ) 
					: BaseResource()
{
	pSeq	= seq;
	color	= playerColor;
	segIdx	= segmentIdx;

	pData		= 0;
	dataSize	= 0;
}

_inl BYTE* GPPixelData::GetData()
{
	if (IsDismissed()) Restore();
	Hit( BaseResource::curCacheFactor );
	assert( pData );
	return pData;
}

_inl unsigned int GPPixelData::Key::hash() const
{
	static const DWORD c_Prime0		= 127;
	static const DWORD c_Prime01	= 251;
	static const DWORD c_Prime02	= 379;
	static const DWORD c_Prime1		= 15731;
	static const DWORD c_Prime2		= 789221;
	static const DWORD c_Prime3		= 1376312589;
	
	DWORD h = seqID + segIdx * c_Prime0 - 
				color * c_Prime01 + c_Prime02;
	h = (h << 13) ^ h;
	h *= h * h * c_Prime1 + c_Prime2;
	h += c_Prime3;
	h ^= h >> 16;
	h ^= h >> 8;
	return h;
} // GPPixelData::Key::hash

/*****************************************************************************/
/*	GPPackSegment implementation
/*****************************************************************************/
_inl GPPackSegment::GPPackSegment( GPSeq* seq, int segmentIdx ) : BaseResource()
{
	pSeq		= seq;
	segIdx		= segmentIdx;
	pData		= NULL;
	dataSize	= 0;
}

_inl BYTE* GPPackSegment::GetData() 
{
		if (IsDismissed()) Restore();
		Hit( BaseResource::curCacheFactor );
		assert( pData );
		return pData;
} // GPPackSegment::GetData

_inl int GPPackSegment::GetDataSize() const
{
	return dataSize;
}

_inl int GPPackSegment::GetUnpackedDataSize() const
{
	return unpackedDataSize;
}

_inl DWORD GPPackSegment::GetPackFlags() const
{
	return packFlags;
}

_inl int GPPackSegment::GetNFrames() const
{
	return nFrames;
}

_inl unsigned int GPPackSegment::Key::hash() const
{
	static const DWORD c_Prime0		= 127;
	static const DWORD c_Prime1		= 15731;
	static const DWORD c_Prime2		= 789221;
	static const DWORD c_Prime3		= 1376312589;
	
	DWORD h = seqID + segIdx * c_Prime0;
	h = (h << 13) ^ h;
	h *= h * h * c_Prime1 + c_Prime2;
	h += c_Prime3;
	h ^= h >> 16;
	h ^= h >> 8;
	return h;
} // GPPackSegment::hash

/*****************************************************************************/
/*	GPChunk implementation
/*****************************************************************************/
_inl int GPChunk::EstimateSizeBytes() const
{
	return side * side * 2;
}

_inl int GPChunk::GetSurfID() const
{
	return item_n_pos & 0x0000FFFF;
}
	
_inl int GPChunk::GetPosInSurfClientList() const 
{
	return (item_n_pos & 0xFFFF0000) >> 16;
}

_inl void GPChunk::SetSurfID( int id ) 
{
	item_n_pos &= 0xFFFF0000;
	item_n_pos |= id;
}
	
_inl void GPChunk::SetPosInSurfClientList( int id )
{
	item_n_pos &= 0x0000FFFF;
	item_n_pos |= (id << 16);
}

/*****************************************************************************/
/*	GPPixelCache implementation
/*****************************************************************************/
_inl int GPPixelCache::GetItemTextureSurfID( int itemID	)
{
	return item[itemID].surfTexID;
} // GPPixelCache::GetItemTextureSurfID
	
_inl void GPPixelCache::DeleteClient( int surfID, int clientIdx )
{
	item[surfID].DeleteClient( clientIdx );
} // GPPixelCache::DeleteClient
	
/*****************************************************************************/
/*	GPPixelCacheItem implementation
/*****************************************************************************/
_inl GPPixelCacheItem::GPPixelCacheItem()
{
	surfTexID	= 0;
}

_inl int GPPixelCacheItem::AddClient( BaseResource* _client )
{
	client.push_back( _client );
	return client.size() - 1;
} // GPPixelCacheItem::AddClient

_inl void GPPixelCacheItem::DeleteClient( int idx )
{
	client.erase( idx );
} // GPPixelCacheItem::DeleteClient
	
_inl void GPPixelCacheItem::ResetClients()
{
	for (int i = 0; i < client.size(); i++)
	{
		if (client[i])
		{
			assert( !client[i]->IsLocked() );
			client[i]->Dismiss();
		}
	}
	client.clear();
} // GPPixelCacheItem::ResetClients

_inl bool GPPixelCacheItem::HasLockedClients() const
{
	for (int i = 0; i < client.size(); i++)
	{
		if (client[i] && client[i]->IsLocked()) return true;
	}
	return false;
} // GPPixelCacheItem::HasLockedClients

/*****************************************************************
/*  TexItemLayout implementation                                 *
/*****************************************************************/
/*---------------------------------------------------------------*
/*  Func:	TexItemLayout::AllocChunk
/*	Desc:	Finds the place on the surface for the given chunk
/*---------------------------------------------------------------*/
_inl bool TexItemLayout::AllocChunk( int sidePow, WORD& allocx, WORD& allocy )
{
	if (sidePow > curAvailLevel) return false;

	assert( sidePow <= maxLevel && sidePow > 0 ); 

	QuadLayoutLevel* curLevel = &level[sidePow];
	//  no free place - split parent greater free chunks
	if (level[sidePow].numQuads == 0) 
	{
		BYTE idx = sidePow;
		while (level[idx].numQuads == 0) 
		{
			idx++;
			if (idx >= c_MaxQuadLevels) return false;
		}

		//  free last chunk
		QuadLayoutLevel* parentLevel = &level[idx];
		parentLevel->numQuads--;
		WORD x = parentLevel->quad[parentLevel->numQuads].x;
		WORD y = parentLevel->quad[parentLevel->numQuads].y;
		//  now do descending splitting of the last free chunk on levels
		for (int i = idx - 1; i >= sidePow; i--)
		{
			curLevel = &level[i];
			curLevel->numQuads = 3;
			
			curLevel->quad[0].x = x;
			curLevel->quad[1].x = x;
			x += c_PowSidesByte[i];

			curLevel->quad[2].x = x;
			
			curLevel->quad[0].y = y;
			curLevel->quad[2].y = y;
			y += c_PowSidesByte[i];

			curLevel->quad[1].y = y;
		}

		//  now do allocate forth chunk
		allocx = x; 
		allocy = y;
	}
	else
	//  there IS free chunk on the level - so allocate it
	{
		curLevel->numQuads--;
		allocx = curLevel->quad[curLevel->numQuads].x; 
		allocy = curLevel->quad[curLevel->numQuads].y;
	}

	pixelCapacity -= (int)c_PowSidesWORD[sidePow] * (int)c_PowSidesWORD[sidePow];
	return true;
} // TexItemLayout::AllocChunk
	
_inl void TexItemLayout::Dump()
{
	for (int i = 0; i < c_MaxQuadLevels; i++)
	{
		Log.Message( "Level %d, side %d", i, c_PowSidesByte[i] );
		level[i].Dump();
	}
} // TexItemLayout::Dump

_inl void TexItemLayout::Free()
{
	//  make all quads "free"
	memset( &level, 0, sizeof( level ) );
	curAvailLevel = maxLevel;
	level[maxLevel].numQuads = 1;
	pixelCapacity = maxPixelCapacity;
} // TexItemLayout::FreeAllChunks

END_NAMESPACE(sg)


