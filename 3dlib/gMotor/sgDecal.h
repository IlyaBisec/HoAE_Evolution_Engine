/*****************************************************************************/
/*	File:	sgDecal.h
/*	Author:	Ruslan Shestopalyuk
/*	Date:	11-13-2003
/*****************************************************************************/
#ifndef __SGDECAL_H__
#define __SGDECAL_H__

namespace sg{

/*****************************************************************************/
/*	Class:	Decal
/*	Desc:	Single decal instance
/*****************************************************************************/
class Decal
{
public:
	int					m_ID;			//  decal ID
	int					m_TexID;		//  decal texture ID

	Vector3D			m_Pos;			//  position of the decal center
	Vector3D			m_Normal;		//  normal of the decal surface at the center
	
	int					m_FirstVert;	//  first decal vertex in the geometry buffer
	int					m_NumVert;		//  number of decal vertices in the geometry buffer
	int					m_FirstInd;		//  first decal index in the geometry buffer
	int					m_NumInd;		//  number of decal indices in the geometry buffer
	
	bool				m_bEnabled;		//  true when decal is drawn
}; // class Decal

const int c_MaxDecalMgrVert = 1024;
const int c_MaxDecalMgrInd  = c_MaxDecalMgrVert * 6;

class GlyphSet;
/*****************************************************************************/
/*	Class:	DecalManager
/*	Desc:	Responsible for creating, caching, mantaining and drawing decals
/*****************************************************************************/
class DecalManager : public Geometry
{
	int					m_MaxDecals;		//  maximal decal amount
	std::vector<Decal>	m_Decals;			//  decal array
	
	Node*				m_pDecalReceiver;	//  part of the scene which is decaled
	GlyphSet*			m_pGlyphSet;
	ZBias*				m_pZBias;

public:
						DecalManager	();
	virtual				~DecalManager	();


	void				SetDecalReceiver( Node* pReceiver );
	void				SetGlyphSet		( GlyphSet* pGlyphSet );
	void				Create			();

	bool				AddStaticDecal( int decalType, int decalID, const Ray3D& ray, 
										float rotation, float width, float height );

	virtual void		Render			();
	
	NODE(DecalManager,Geometry,DMGR);
}; // class DecalManager

}; // namespace sg

#endif // __SGDECAL_H__