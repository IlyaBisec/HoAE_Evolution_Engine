/*****************************************************************************/
/*	File:	sgGPConstruction.h
/*	Desc:	Isometry-specific 2.5D sprite constructions
/*	Author:	Ruslan Shestopalyuk
/*	Date:	10-21-2003
/*****************************************************************************/
#ifndef __SGGPCONSTRUCTION_H__
#define __SGGPCONSTRUCTION_H__

namespace sg{

struct GPCarcassChunk
{
	GPCarcassChunk() : nTri(0), flags(0) {}
	GPCarcassChunk( int _nTri, int _nVert ) : nTri(_nTri), nVert(_nVert) {}
	WORD		nTri;
	WORD		nVert;
	DWORD		flags;

	friend OutStream&	operator <<( OutStream& os, const GPCarcassChunk& chunk );
	friend InStream&	operator >>( InStream& is, GPCarcassChunk& chunk );

}; // struct GPCarcassChunk

/*****************************************************************************/
/*	Class:	GPObj
/*	Desc:	GPSystem sprite, pulled onto mesh
/*****************************************************************************/
class GPObj : public Geometry
{
public:
	GPObj();

	virtual void				Serialize	( OutStream& os ) const;
	virtual void				Unserialize	( InStream& is );

	int							GetNChunks	() const { return chunks.size(); }
	int							GetChunkNPoly( int chunkIdx ) const { return chunks[chunkIdx].nTri; }
	void						AddChunk	( int nTri, int nV ) { chunks.push_back( GPCarcassChunk( nTri, nV ) ); }

	_inl const char*			GetGPName	() const				{ return gpName.c_str();	}
	_inl void					SetGPName	( const char* name )	{ gpName = name;	}

	_inl int					GetFrameIdx	() const				{ return frameIdx;	}	
	_inl void					SetFrameIdx ( int frame )			{ frameIdx = frame;	}

	void						Clear		();
	void						ClearChunks	();
	
	int							GetGPID		() const { return gpID;		}

	void						SetPivot	( const Vector3D& pivot ) { m_Pivot = pivot; }
	Vector3D					GetPivot	() const { return m_Pivot; }

	virtual void				Render		();
	virtual void				Expose		( PropertyMap& pmap );

	MAGIC(GPObj,Geometry,GPOB);

protected:
	int							gpID;
	std::string					gpName;
	int							frameIdx;
	std::vector<GPCarcassChunk>	chunks;	
	Vector3D					m_Pivot;
}; // class GPObj

/*****************************************************************************/
/*	Class:	GPCarcassObj
/*	Desc:	GPSystem sprite, which has depth
/*****************************************************************************/
class GPCarcassObj : public GPObj
{

public:
	GPCarcassObj();

	virtual void		Serialize	( OutStream& os ) const;
	virtual void		Unserialize	( InStream& is );


	MAGIC(GPCarcassObj,GPObj,GPCO);
}; // class GPCarcassObj

/*****************************************************************************/
/*	Class:	GPShadowObj
/*	Desc:	Shadow of the GPCarcassObj
/*****************************************************************************/
class GPShadowObj : public GPObj
{
public:
	virtual void	Render();

	MAGIC(GPShadowObj,GPObj,GPSO);
}; // class GPShadowObj

/*****************************************************************************/
/*	Class:	GPBuilding
/*	Desc:	Carcassed sprite with navigation and shadow
/*****************************************************************************/
class GPBuilding : public Node
{
	GPCarcassObj*			m_pCarcass;
	GPShadowObj*			m_pShadow;
	Geometry*				m_pNavimesh;

	bool					m_bShowWire;

public:	
	GPBuilding();

	virtual void		Render		();
	virtual void		Expose		( PropertyMap& pmap );

	void				RenderMain	();
	void				RenderShadow();

	GPShadowObj*		FindShadow	();
	GPCarcassObj*		FindCarcass	();

	int					GetMainGP	();
	int					GetMainFrame();
	Vector3D			GetPivot	();

	MAGIC(GPBuilding, Node, GPBU);
}; // class GPBuilding

}; // namespace sg

#endif // __SGGPCONSTRUCTION_H__
