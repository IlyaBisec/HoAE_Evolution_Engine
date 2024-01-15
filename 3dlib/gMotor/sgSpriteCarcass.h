/*****************************************************************************/
/*	File:	sgSpriteCarcass.h
/*	Desc:	Isometry-specific 2.5D sprite constructions
/*	Author:	Ruslan Shestopalyuk
/*	Date:	10-21-2003
/*****************************************************************************/
#ifndef __SGSPRITECARCASS_H__
#define __SGSPRITECARCASS_H__

namespace sg{

struct CarcassChunk
{
	CarcassChunk() : nTri(0), flags(0) {}
	CarcassChunk( int _nTri, int _nVert ) : nTri(_nTri), nVert(_nVert) {}
	WORD		nTri;
	WORD		nVert;
	DWORD		flags;

	friend OutStream&	operator <<( OutStream& os, const CarcassChunk& chunk );
	friend InStream&	operator >>( InStream& is, CarcassChunk& chunk );

}; // struct CarcassChunk

/*****************************************************************************/
/*	Class:	CarcassSprite
/*	Desc:	SpriteManager sprite, pulled onto mesh
/*****************************************************************************/
class CarcassSprite : public Geometry
{
public:
								CarcassSprite	();

	virtual void				Serialize		( OutStream& os ) const;
	virtual void				Unserialize		( InStream& is );

	int							GetNChunks		() const					{ return m_Chunks.size();	}
	int							GetChunkNPoly	( int chunkIdx ) const		{ return m_Chunks[chunkIdx].nTri; }
	void						AddChunk		( int nTri, int nV )		{ m_Chunks.push_back( CarcassChunk( nTri, nV ) ); }

	_inl const char*			GetPackageName	() const					{ return m_SeqName.c_str();	}
	_inl void					SetPackageName	( const char* name )		{ m_SeqName = name;			}
	_inl int					GetFrameIdx		() const					{ return m_FrameIdx;		}	
	_inl void					SetFrameIdx		( int frame )				{ m_FrameIdx = frame;		}

	void						Clear			();
	void						ClearChunks		();
	int							GetGPID			() const					{ return m_SeqID;			}
	void						SetPivot		( const Vector3D& pivot )	{ m_Pivot = pivot;			}
	Vector3D					GetPivot		() const					{ return m_Pivot;			}

	virtual void				Render			();
	virtual void				Expose			( PropertyMap& pm );

	NODE(CarcassSprite,Geometry,GPOB);

protected:
	int							m_SeqID;
	std::string					m_SeqName;
	int							m_FrameIdx;
	std::vector<CarcassChunk>	m_Chunks;	
	Vector3D					m_Pivot;
}; // class CarcassSprite

/*****************************************************************************/
/*	Class:	CarcassObject
/*	Desc:	SpriteManager sprite, which has depth
/*****************************************************************************/
class CarcassObject : public CarcassSprite
{

public:
	CarcassObject();

	virtual void		Serialize	( OutStream& os ) const;
	virtual void		Unserialize	( InStream& is );


	NODE(CarcassObject,CarcassSprite,GPCO);
}; // class CarcassObject

/*****************************************************************************/
/*	Class:	CarcassShadow
/*	Desc:	Shadow of the CarcassObject
/*****************************************************************************/
class CarcassShadow : public CarcassSprite
{
public:
	virtual void	Render();

	NODE(CarcassShadow,CarcassSprite,GPSO);
}; // class CarcassShadow

/*****************************************************************************/
/*	Class:	CarcassBuilding
/*	Desc:	Carcassed sprite with navigation and shadow
/*****************************************************************************/
class CarcassBuilding : public Node
{
	CarcassObject*			m_pCarcass;
	CarcassShadow*			m_pShadow;
	Geometry*				m_pNavimesh;

	bool					m_bShowWire;

public:	
	CarcassBuilding();

	virtual void		Render		();
	virtual void		Expose		( PropertyMap& pm );

	void				RenderMain	();
	void				RenderShadow();

	CarcassShadow*		FindShadow	();
	CarcassObject*		FindCarcass	();

	int					GetMainGP	();
	int					GetMainFrame();
	Vector3D			GetPivot	();

	NODE(CarcassBuilding, Node, GPBU);
}; // class CarcassBuilding

}; // namespace sg

#endif // __SGSPRITECARCASS_H__
