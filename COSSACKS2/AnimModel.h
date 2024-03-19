/*****************************************************************************/
/*	File:	AnimModel.h
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	20.11.2002
/*****************************************************************************/
#ifndef __ANIMMODEL_H__
#define __ANIMMODEL_H__

#include <stdio.h>
#include <assert.h>

/*****************************************************************************/
/*	Class:	VertexConfig
/*	Desc:	Vertex content configuration descriptor
/*****************************************************************************/
class VertexConfig
{
	DWORD			flagz;

public:
	VertexConfig() : flagz(0) {}

	void	Read		( FILE* fp ) { fread( &flagz, 1, sizeof(DWORD), fp ); }

	void	reset()		{ flagz = 0; }
	bool	hasXYZ		() const { return (flagz & 1) != 0; }
	bool	hasXYZW		() const { return (flagz & 2) != 0; }
	bool	hasUV		() const { return (flagz & 4) != 0; }
	bool	hasUV2		() const { return (flagz & 8) != 0; }
	bool	hasDiffuse	() const { return (flagz & 16) != 0;}
	bool	hasSpecular	() const { return (flagz & 32) != 0;}
	bool	hasNormal	() const { return (flagz & 64) != 0;}
	int		numBoneW	() const { return (flagz & 0xF0000000) >> 28;}

	void	setNumBoneW ( int nBW ) { flagz &= 0x0FFFFFFF; flagz |= (nBW << 28); }

	void	setXYZ		( bool has = true ) { if (has) flagz |= 1; else flagz &= ~1; }
	void	setXYZW		( bool has = true ) { if (has) flagz |= 2; else flagz &= ~2; }
	void	setUV		( bool has = true ) { if (has) flagz |= 4; else flagz &= ~4; }
	void	setUV2		( bool has = true ) { if (has) flagz |= 8; else flagz &= ~8; }
	void	setDiffuse	( bool has = true ) { if (has) flagz |= 16; else flagz &= ~16;}
	void	setSpecular	( bool has = true ) { if (has) flagz |= 32; else flagz &= ~32;}
	void	setNormal	( bool has = true ) { if (has) flagz |= 64; else flagz &= ~64;}

	int				getSizeBytes() const;
	VertexFormat	VF() const;
	void			ConvertToVF( void* dest, void* src, int nVert );

}; // class VertexConfig

typedef float Matr3x4[12];

/*****************************************************************************/
/*	Class:	anmChannel
/*	Desc:	
/*****************************************************************************/
class anmChannel
{
public:
	int			boneIdx;
	int			animTime;
	int			keyRate;
	int			nFrames;

	Matrix4D*	transf;

				anmChannel() : transf(0), nFrames(0), keyRate(0) {}
				~anmChannel() { delete []transf; }
	void		setNFrames( int nF )
				{
					nFrames = nF;
					delete []transf;
					transf = new Matrix4D[nF];
					nFrames = nF;
				}
	void		calcMatrByTime ( int time,  Matrix4D& matr );
	void		calcMatrByFrame( int frame, Matrix4D& matr );
	
	void		getRestPoseMatrix( Matrix4D& matr ) const 
				{  
					if (nFrames > 0) matr = transf[0];
					else matr.setIdentity();
				}

}; // class anmChannel

/*****************************************************************************/
/*	Class:	anmAnimation
/*	Desc:	
/*****************************************************************************/
class anmAnimation
{
public:
	DWORD 		curTime;
	DWORD 		startTime;
	anmChannel* channel;
	int			nChannels;

				anmAnimation() { channel = 0; nChannels = 0; }
				~anmAnimation() { delete []channel; }
	void		setNChannels( int nCh ) 
				{ 
					delete []channel; 
					channel = new anmChannel[nCh];
					nChannels = nCh;
				}
	anmChannel&	getChannel( int chIdx ) 
				{ 
					assert( chIdx < nChannels && chIdx >= 0 );
					return channel[chIdx];
				}
}; // class anmAnimation

/*****************************************************************************/
/*	Class:	anmMeshChunk
/*	Desc:	
/*****************************************************************************/
class anmMeshChunk
{
public:
	anmMeshChunk() : subIdx(0), firstPoly(0), numPoly(0) {}
	WORD	subIdx;
	int		firstPoly;
	int		numPoly;
	int		firstVert;
	int		numVert;
}; // class anmMeshChunk

/*****************************************************************************/
/*	Class:	anmSubMesh
/*	Desc:	Animated model submesh
/*****************************************************************************/
class anmSubMesh
{
public:
	DWORD			flags;
	anmChannel		mtlAnimation;
	anmMeshChunk*	chunks;
	int				nChunks;
	Matrix4D		transform;

	anmSubMesh() : flags(0), nChunks(0), chunks(0) {}
	~anmSubMesh() { delete []chunks; }

	void	setNChunks( int nCh ) 
	{ 
		delete []chunks; 
		chunks = new anmMeshChunk[nCh];
		nChunks = nCh;
	}

	bool		isAnimated() const { return flags & 4; }

	void		setTransform( const Matrix4D& matr )
	{
		transform = matr;
	}

}; // class anmSubMesh

const int c_MaxNameLen = 64;
/*****************************************************************************/
/*	Class:	anmBone
/*	Desc:	
/*****************************************************************************/
class anmBone
{
public:
	int				idx;
	int				papaIdx;
	char			name[c_MaxNameLen];
	int				nChunks;
	AABoundBox		aabb;


	Matrix4D		transform;
	anmMeshChunk*	chunk;


	anmBone() : nChunks(0), chunk(0) { name[0] = 0; papaIdx = -1; }
	void			setNChunks( int n )
	{
		delete []chunk;
		nChunks = n;
		chunk = new anmMeshChunk[n];
	}

	int				getNPolys() const
	{
		int nP = 0;
		for (int i = 0; i < nChunks; i++) nP += chunk[i].numPoly;
		return nP;
	}

	bool			hasName( const char* boneName ) const
	{
		return !strcmp( name, boneName );
	}

	void		setTransform( const Matrix4D& matr )
	{
		transform = matr;
	}
}; // class anmBone

/*****************************************************************************/
/*	Class:	anmSkeleton
/*	Desc:	
/*****************************************************************************/
class anmSkeleton
{
	anmBone*		bone;
	int				nBones;

	anmSubMesh*		sub;
	int				nSubs;
	bool			hierTransf;

public:
	anmSkeleton() : bone(0), nBones(0), sub(0), nSubs(0), hierTransf(false) {}
	~anmSkeleton() { delete []bone; }
	void		setNBones( int nb ) 
	{  
		delete []bone;
		nBones = nb;
		bone = new anmBone[nBones];
		for (int i = 0; i < nBones; i++) bone[i].idx = i;
	}

	void		setNSub( int ns ) 
	{  
		delete []sub;
		nSubs = ns;
		sub = new anmSubMesh[ns];
	}

	void setHierarchicalTransf( bool hier = true ) { hierTransf = hier; }

	anmBone* getBone( int idx ) 
	{
		if ( idx < 0 || idx >= nBones ) return 0;
		return &bone[idx];
	}

	anmBone* getBonesArray() const { return bone; }

	anmBone* getBone( const char* boneName )
	{
		for (int i = 0; i < nBones; i++)
		{
			if (bone[i].hasName( boneName )) return &bone[i];
		}
		return 0;
	}

	anmSubMesh* getSub( int idx ) 
	{
		if( idx < 0 || idx >= nSubs ) return 0;
		return &sub[idx];
	}


	int getNBones() const 
	{
		return nBones;
	}

	
	int getNSub() const 
	{
		return nSubs;
	}

	bool	calcAABB( AABoundBox& aabb );
}; // class anmSkeleton

/*****************************************************************************/
/*	Enum:	BlockType
/*	Desc:	Type of data block in the resource file
/*****************************************************************************/
enum BlockType
{
	btUnknown		= 0,
	btMesh			= 1,
	btSubmesh		= 2,
	btMaterial		= 3,
	btIBuf			= 4,
	btVBuf			= 5,
	btTexTable		= 6,
	btSkeleton		= 7,
	btAnimation		= 8,
	btAnimChannel	= 9,
	btC2Model		= 10
}; // enum BlockType

/*****************************************************************************/
/*	Class:	BlockReader
/*	Desc:
/*****************************************************************************/
class BlockReader
{
	char			type[4];
	unsigned int	blSize;
	long			filePos;
	FILE*			file;
public:
	BlockReader( FILE* fp )
	{
		fread( &type, 4, 1, fp );
		fread( &blSize, 4, 1, fp );
		filePos = ftell( fp );
		file	= fp;
	}

	void			end()
	{
		if (!feof( file )) 
		{
			fseek( file, filePos + blSize, SEEK_SET );
		}
	}

	int				blockSize()		const { return blSize;		}
	int				headerSize()	const { return 8;			}
	int				fullSize()		const { return blSize + 8;	}
	BlockType		blockType()		const;
}; // class BlockReader

/*****************************************************************************/
/*	Class:	ComplexMesh
/*	Desc:	
/*****************************************************************************/
class ComplexMesh
{
protected:
	BaseMesh*		bm;
	int				nSub;
	
	float			radius;

public:
				ComplexMesh() : bm(0), nSub(0) {};
	virtual		~ComplexMesh();
	
	void		setNMeshes		( int nM );
	bool		LoadMesh		( FILE* fp, int nBytes );

	float			getRadius() const { return radius; }
	
}; // ComplexMesh

const int c_StrBufSize = 2048;
/*****************************************************************************/
/*	Class:	AnimModel	
/*	Desc:	Animated 3D model
/*****************************************************************************/
class AnimModel : public ComplexMesh
{
	anmSkeleton		skeleton;
	anmAnimation	anim;

	static			BaseMesh swBM;
public:
				AnimModel();
	bool		LoadAnimation	( FILE* fp, int nBytes );
	bool		LoadSkeleton	( FILE* fp, int nBytes );
	bool		LoadTexTable	( IRenderSystem* irs, FILE* fp, int nBytes );
	bool		Load			( IRenderSystem* irs, const char* fname );
	void		ProgressAnim	( int frame );
	
	void		Draw( IRenderSystem* irs );
	void		DrawAABB( IRenderSystem* irs, DWORD color );
	void		DrawBoneAABBs( IRenderSystem* irs, DWORD color );
	void		DrawPlain( IRenderSystem* irs );
	void		DrawSWSkinning( IRenderSystem* irs );

	void		dump();

	anmSkeleton&	getSkeleton() { return skeleton; }

	static char strBuf[c_StrBufSize];

}; // class AnimModel
#endif // __ANIMMODEL_H__