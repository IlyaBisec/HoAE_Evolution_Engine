/*****************************************************************************/
/*	File:	gpPackage.h
/*	Desc:	Generic .g16 sprite package	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	27.02.2003
/*****************************************************************************/
#ifndef __GPPACKAGE_H__
#define __GPPACKAGE_H__

const int c_MaxQuadLevels			= 16;	//  maximal powers of quad side in texture layout

const int c_GPTexSide				= 256;	//  side of texture in gp cache
const int c_GPTexSidePow			= 8;	//  2 power of c_GPTexSide

const int c_MaxGPSurfaceClients		= 4096;
const int c_PixelCapacityBias		= 32*32;
const int c_MaxFramesInPackedSegment= 256;
const int c_MaxGPChunkBlocks		= 16;
const int c_MaxGPPalettes			= 2;

const ColorFormat c_SpriteColorFormat = cfARGB4444;

const BYTE	c_PowSidesByte[]		= { 1, 2, 4, 8, 16, 32, 64, 128 };
const WORD	c_PowSidesWORD[]		= { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 
										1024, 2048, 4096, 8192, 16384, 32768 };
const float c_PowSidesFloat[]		= { 1.0f, 2.0f, 4.0f, 8.0f, 16.0f, 32.0f, 64.0f, 128.0f,
										256.0f, 512.0f, 1024.0f, 2048.0f, 4096.0f, 8192.0f, 
										16384.0f, 32768.0f };

namespace sg{

/*****************************************************************************/
/*	Enum:	GPType
/*	Desc:	Types of sprite packages
/*****************************************************************************/
enum GPType
{
	gptUnknown		= 0,
	gptUniform		= 1,
	gptNonUniform	= 2
};  // enum GPType

/*****************************************************************
/*	Class:	GPChunkHeader
/*****************************************************************/
class GPChunkHeader
{
public:
	_inl int	GetSidePow()				const;
	_inl int	GetSide()					const;
	_inl int	GetX()						const; 
	_inl int	GetY()						const;
	_inl int	GetCacheItem()				const;
	_inl void	GetUV( WORD& u, WORD& v )	const;
	_inl BYTE*	GetPixelData()				const;
	_inl int	GetSizeBytes()				const;

	_inl bool	IsFull()					const;
	_inl bool	IsLU()						const;
	_inl bool	IsLB()						const;
	_inl bool	IsRU()						const;
	_inl bool	IsRB()						const;

	_inl void	SetCacheItem( int cItem );
	_inl void	SetUV( WORD u, WORD v );

protected:
private:
	BYTE			data[8]; //  dummy data. Don't touch it with dirty hands
}; // class GPChunkHeader

/*****************************************************************************/
/*	Class:	GPChunk
/*	Desc:	
/*****************************************************************************/
class GPChunk
{
public:
public:
	float			x;
	float			y;

	float			u;
	float			v;
	float			u2;
	float			v2;
	
	float			side;

	_inl int		GetSurfID				()			const; 
	_inl int		GetPosInSurfClientList	()			const; 
	
	_inl void		SetSurfID				( int id );
	_inl void		SetPosInSurfClientList	( int id ); 
	

	_inl int		EstimateSizeBytes()					const;

protected:
	DWORD			item_n_pos;

}; // class GPChunk

class GPFrameInst;
/*****************************************************************************/
/*	Class:	GPFrame
/*	Desc:	GP sequence single frame description	
/*****************************************************************************/
struct GPFrame
{
	int				nChunks;
	int				width;
	int				height;
	

	DWORD			offsInPixelSegment;
	int				packSegmentIdx;		
	GPFrameInst*	lastFrameInst;
	
	static DWORD	curCacheFactor;
}; // struct GPFrame

class GPSeq;

/*****************************************************************************/
/*	Class:	GPPackSegment
/*	Desc:	Packed sprite pixel data segment resource
/*****************************************************************************/
class GPPackSegment : public BaseResource
{
	BYTE*			pData;
	int				dataSize;
	int				unpackedDataSize;

	int				offsetFromFileBegin;
	int				nFrames;
	
	GPSeq*			pSeq;
	int				segIdx;
	DWORD			packFlags;

public:
	struct Key
	{
		int			seqID;
		int			segIdx;

		Key( int _segIdx, int _seqID ) : segIdx(_segIdx), seqID(_seqID) {}
		Key(){}

		bool operator==( const Key& key ) const 
		{ 
			return (seqID == key.seqID) && (segIdx == key.segIdx);
		}
		_inl unsigned int	hash() const;
	};

	_inl GPPackSegment( GPSeq* seq, int segmentIdx );
	~GPPackSegment();

	virtual bool	InitPrefix();
	virtual bool	Restore();
	virtual void	Dismiss();
	virtual int		GetDismissableSizeBytes() const;
	virtual int		GetHeaderSizeBytes() const;

	
	_inl BYTE*			GetData(); 
	_inl int			GetDataSize() const; 
	_inl int			GetUnpackedDataSize() const; 

	_inl DWORD			GetPackFlags() const;
	_inl int			GetNFrames() const;
	

private:
}; // class GPPackSegment

/*****************************************************************************/
/*	Class:	GPPixelData
/*	Desc:	Represents single unpacked segment of pixel data resource
/*****************************************************************************/
class GPPixelData : public BaseResource
{
	BYTE*			pData;
	int				dataSize;
	
	GPSeq*			pSeq;
	int				segIdx;
	DWORD			color;

public:
	struct Key
	{
		int			seqID;
		int			segIdx;
		DWORD		color;

		Key( int _segIdx, int _seqID, DWORD _color ) 
			: segIdx(_segIdx), seqID(_seqID), color(_color) {}

		bool operator==( const Key& key ) const 
		{ 
			return	(seqID	== key.seqID)	&& 
					(segIdx == key.segIdx)	&&
					(color	== key.color);
		}

		Key(){}
		_inl unsigned int	hash() const;
	};

	_inl GPPixelData( GPSeq* seq, int segmentIdx, DWORD playerColor );
	~GPPixelData();

	
	virtual bool	InitPrefix();
	virtual bool	Restore();
	virtual void	Dismiss();
	virtual int		GetDismissableSizeBytes() const;
	virtual int		GetHeaderSizeBytes() const;

	_inl BYTE*		GetData();

private:
	//  unpacked pixel data resource manager
	static DWORD s_FrameOffsetArray[c_MaxFramesInPackedSegment];
}; // class GPPixelData


/*****************************************************************************/
/*	Class:	GPFrameInst
/*****************************************************************************/
class GPFrameInst : public BaseResource
{
public:
	struct Key
	{
		WORD				gpID;
		DWORD				sprID;
		DWORD				color;
		WORD				lod;

		Key( WORD _gpID, DWORD _sprID, DWORD _color, WORD _lod = 0 ) 
				: gpID(_gpID), sprID(_sprID), color(_color), lod(_lod) {}
		Key(){}

		bool operator==( const Key& key ) const 
		{ 
			return	(gpID	== key.gpID	)	&& 
					(sprID	== key.sprID)	&& 
					(color	== key.color)	&&
					(lod	== key.lod	);
		}
		_inl unsigned int	hash() const;
	};

	_inl unsigned int hash() const;	
	_inl void copy( const GPFrameInst& fr )
	{
		*this = fr;
	}

	_inl bool equal( const GPFrameInst& fr )
	{
		return	(gpID	== fr.gpID	) &&
				(sprID	== fr.sprID	) && 
				(color	== fr.color	) &&
				(lod	== fr.lod	);
	}

	_inl int			GetNChunks() const;
	_inl void			SetNChunks( int nChunks, void* buf = NULL );
	_inl int			GetSeqID()		const; 
	_inl int			GetFrameID()	const; 
	_inl bool			IsDrawnAtOnce() const; 

	_inl BYTE*			GetPixelData()	const;
	_inl GPChunk&		GetChunk( int idx );
	_inl const GPChunk&	GetChunk( int idx ) const;

	_inl DWORD			GetColor()		const;
	void				OnDrop();

	bool				GetLeftTop( int& left, int& top ) const;

	virtual int			GetDismissableSizeBytes() const;
	virtual int			GetHeaderSizeBytes() const;
	virtual bool		InitPrefix();
	virtual void		Dismiss();
	virtual bool		Restore();
	virtual void		Dump();

						GPFrameInst(){}

protected:

	c2::array<GPChunk>	chunk;
	GPFrame*			pFrame;				
	GPSeq*				pSeq;

	WORD				gpID;		//  package index
	DWORD				sprID;		//  frame index in package
	DWORD				color;		//  national color
	WORD				lod;		//  level of detail - 0==actual size, 1==half a side, etc.

	DWORD				factor;

private:
	GPPixelData*		pixData;
	
	friend class		GPPixelCache;
	friend class		GPSystem;
}; // class GPFrameInst


template <class T> 
class Point2
{
public:
	T	x;
	T	y;
};

typedef Point2<BYTE> BytePoint2;
typedef Point2<WORD> WordPoint2;

/*****************************************************************************/
/*	Struct:	QuadLayoutLevel
/*	Desc:	Helper structure for packing sprite chunks on the texture surface
/*****************************************************************************/
struct QuadLayoutLevel
{
	WordPoint2	quad[3];
	BYTE		numQuads;

	void Dump();
};  // struct QuadLayoutLevel

/****************************************************************************/
/*  Class:  TexItemLayout                                        
/*  Desc:	Describes layout of sprite chunks on the texture surface
/****************************************************************************/
class TexItemLayout
{
public:
	TexItemLayout();

	_inl void	Free();
	_inl bool	AllocChunk( int sidePow, WORD& allocx, WORD& allocy );
	_inl int	GetPixelCapacity() const { return pixelCapacity; }

	void Init( int texSide ); 					
	void Dump();

	int						texID;

protected:

	QuadLayoutLevel			level[c_MaxQuadLevels + 1];
	int						curAvailLevel;
	int						maxLevel;
	int						pixelCapacity;
	int						maxPixelCapacity;

}; //  class TexItemLayout

/*****************************************************************************/
/*	Class:	GPPixelCacheItem
/*	Desc: 
/*****************************************************************************/
class GPPixelCacheItem : public TextureClient
{
public:
	_inl			GPPixelCacheItem();

	virtual void	Invalidate( TextureDescr* texture );
	virtual void	Restore( TextureDescr* texture );

	void			Dump();

	_inl int		AddClient		( BaseResource* _client );
	_inl void		DeleteClient	( int idx );
	_inl void		ResetClients	();
	_inl bool		HasLockedClients() const;

protected:
	int					surfTexID;		//  texture memory surface ID
	TexItemLayout		layout;			//  layouter
	int					ID;				//  item ID

private:
	typedef c2::static_ptr_array<BaseResource, c_MaxGPSurfaceClients> ClientList;
	
	ClientList			client;

	friend class GPPixelCache;
}; // class GPPixelCacheItem

class GPSystem;
/*****************************************************************************/
/*	Class:	GPPixelCache
/*	Desc: 
/*****************************************************************************/
class GPPixelCache
{
public:
	GPPixelCache();

	void				Init				( IRenderSystem*	iRS, GPSystem* _pGPS );
	bool				MovePixelsToCache	( GPFrameInst*		frameInst	);
	void				OnFrame				();
	void				Flush				();
	void				Dump				();
	int					AllocCacheItem		( BaseResource*		client		);
	void				FreeItem			( int				itemID		);
	

	_inl int			GetItemTextureSurfID( int itemID );
	_inl void			DeleteClient		( int surfID, int clientIdx );
	void				Drop				(  GPFrameInst*		frameInst	);


protected:
	int					GetFreeCacheItem	();
	int					CalcNDupes			();

private:
	GPSystem*						pGPS;
	int								m_DismissCandidate;
	std::vector<GPPixelCacheItem>		item;
	GPPixelCacheItem*				curItem;	
	
	float				uvTableL	[c_GPTexSide + 1];	//  texture coordinates - eps
	float				uvTableU	[c_GPTexSide + 1];	//  texture coordinates + eps
}; // class GPPixelCache


/*****************************************************************
/*	Class:	GPSeq
/*	Desc:	Sprite sequence with uniform sprites
/*****************************************************************/
class GPSeq
{
public:
	GPSeq();
	~GPSeq();

	_inl int			GetNFrames		()						const;
	_inl BYTE*			GetWorkBuf		()						const;
	_inl int			GetNDirections	()						const;
	_inl int			GetID			()						const;

	virtual int			GetPSegSize		( int segIdx )			const = 0;
	virtual int			GetPSegFlags	( int segIdx )			const = 0;
	virtual int			GetPSegFileOffset( int segIdx )			const = 0;
	virtual int			GetPSegNFrames	( int segIdx )			const = 0;
	

	_inl void			SetID			( int gpID );
	_inl GPFrame*		GetFrame		( int fidx );
	
	_inl const char*	GetName			()						const;
	_inl void			SetName			( const char* _name );

	_inl const char*	GetPath			()						const;
	_inl void			SetPath			( const char* _path );

	_inl void			SetType			( GPType type );
	_inl GPType			GetType			()						const;
	_inl bool			IsValid			()						const;

	_inl void			AllocatePalette	( int palIdx, int size );
	_inl BYTE*			GetPalette		( int palIdx )			const;	

	void				SetFrameOffsets	( DWORD* offsets, int segIdx, int nFrames );

	static GPSeq*		CreateHeaderFromFile( const char* fname );
	void				Cleanup			();
	_inl bool			AdjustWorkBuffer();


	typedef DynamicResMgr<GPPixelData, GPPixelData::Key> GPPixelDataMgr;
	typedef DynamicResMgr<GPPackSegment, GPPackSegment::Key> GPPackedSegmentMgr;
	

	static GPPixelDataMgr					s_PixDataMgr;
	static GPPackedSegmentMgr				s_PackSegMgr;
	static GPPixelCache						s_PixelCache;

	static GPPackSegment*	GetPackSegment	( GPSeq* seq, int segIdx );
	static GPPixelData*		GetPixelData	( GPSeq* seq, int segIdx, DWORD color );


protected:
	static GPSeq*		CreateInstanceFromFile( const char* fname );
	static BYTE*		s_WorkBuffer;
	static DWORD		s_WorkBufferSize;
	
	DWORD				maxWorkBufferSize;
	WORD				nSprites;
	WORD				numDirections;

	GPType				type;			//  sequence type
	std::string			path;			//  file system path, from the root
	std::string			name;			//  string ID 

	std::vector<GPFrame>	frame;	
	
	BYTE*				palette		[c_MaxGPPalettes];
	int					paletteSize	[c_MaxGPPalettes];

private:		
	int					id;		
	

	friend class		GPSystem;
}; // class GPSeq

}; // namespace sg

#ifdef _INLINES
#include "gpPackage.inl"
#endif // _INLINES

#endif // __GPPACKAGE_H__