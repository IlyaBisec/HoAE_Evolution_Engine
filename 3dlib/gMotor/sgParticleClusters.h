/*****************************************************************************/
/*	File:	sgParticleClusters.h
/*	Author:	Ruslan Shestopalyuk
/*	Date:	08-28-2003
/*****************************************************************************/
#ifndef __SGPARTICLECLUSTERS_H__
#define __SGPARTICLECLUSTERS_H__

namespace sg{
/*****************************************************************************/
/*	Class:	QuadCluster
/*	Desc:	Particle cluster of arbitrary oriented quads
/*****************************************************************************/
class QuadCluster : public ParticleCluster
{
public:
	QuadCluster		();
	virtual void		Render			();
	virtual void		Flush			();

	NODE(QuadCluster,ParticleCluster,QCLU);
protected:
}; // class QuadCluster

/*****************************************************************************/
/*	Class:	BillboardCluster
/*	Desc:	Particle cluster of camera-aligned quads
/*****************************************************************************/
class BillboardCluster : public ParticleCluster
{
public:
						BillboardCluster();
	virtual void		Render			();
	virtual void		Flush			();

	void				SetBillboardTM	( const Matrix3D& m );

	NODE(BillboardCluster,ParticleCluster,BCLU);

protected:
	void				RenderQuads();
	void				FillQuads();
	void				FixStripJoints();

	void				ResetQuadBuffer();
	bool				AddQuad( const Vector3D& pos, float sizeX, float sizeY,
								 float rot, DWORD color, const Rct& uv );
	bool				AddQuad( const Vector3D& pos, float size, DWORD color, const Rct& uv );
	bool				AddQuad( const Vector3D& pos1, float size1, DWORD color1, 
								 const Vector3D& pos2, float size2, DWORD color2,
								 const Rct& uv );

	void				SetMaxQuads( int nQuads );
	int					GetMaxQuads() const { return m_MaxQuads; }

private:
	BaseMesh			m_QuadBuffer;
	int					m_NQuads;
	int					m_MaxQuads;
	Matrix3D			m_BillboardTM;
	Vector3D			m_LT, m_RT, m_LB, m_RB;		//  cached rotated corners
}; // class BillboardCluster

/*****************************************************************************/
/*	Class:	PlaneCluster
/*	Desc:	Particle cluster of plane-aligned quads (local xOy plane)
/*****************************************************************************/
class PlaneCluster : public BillboardCluster
{
public:
							PlaneCluster		();
	virtual void			Flush				();
	virtual void			Render				();
	
	NODE(PlaneCluster,ParticleCluster,PLCL);
protected:
}; // class PlaneCluster

/*****************************************************************************/
/*	Class:	LineCluster
/*	Desc:	Particle cluster of line segments
/*****************************************************************************/
class LineCluster : public ParticleCluster
{
public:
	virtual void			Render				();

	NODE(LineCluster,ParticleCluster,LCLU);

protected:
	float					m_LineLen;
}; // class LineCluster

/*****************************************************************************/
/*	Class:	BeamCluster
/*	Desc:	Particle cluster of connected line segments
/*****************************************************************************/
class BeamCluster : public BillboardCluster
{
public:
					BeamCluster			();
	virtual void	Render				();
	virtual void	Expose				( PropertyMap& pm );

	virtual void	Flush				();
	void			Serialize			( OutStream& os ) const;
	void			Unserialize			( InStream& is  );

	NODE(BeamCluster,ParticleCluster,BECL);

protected:
	bool			m_bFixJoints;
}; // class BeamCluster

/*****************************************************************************/
/*	Class:	MeshPolyCluster
/*	Desc:	Consists of polygons from some mesh
/*****************************************************************************/
class MeshPolyCluster : public BillboardCluster
{
public:
					MeshPolyCluster		();
	virtual void	Render				();
	virtual void	Expose				( PropertyMap& pm );

	void			Serialize			( OutStream& os ) const;
	void			Unserialize			( InStream& is  );

	NODE(MeshPolyCluster,ParticleCluster,MPCL);

protected:

}; // class MeshPolyCluster

/*****************************************************************************/
/*	Class:	PolyObjectCluster
/*	Desc:	Particle cluster of mesh objects
/*****************************************************************************/
class PolyObjectCluster : public ParticleCluster
{
public:
	virtual void			Render				();

	NODE(PolyObjectCluster,ParticleCluster,MCLU);
protected:
}; // class PolyObjectCluster

}; // namespace sg

#endif __SGPARTICLECLUSTERS_H__
