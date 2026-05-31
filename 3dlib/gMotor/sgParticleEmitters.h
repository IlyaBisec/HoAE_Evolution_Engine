/*****************************************************************************/
/*	File:	sgParticleEmitters.h
/*	Author:	Ruslan Shestopalyuk
/*	Date:	08-28-2003
/*****************************************************************************/
#ifndef __SGPARTICLEEMITTERS_H__
#define __SGPARTICLEEMITTERS_H__

namespace sg{
/*****************************************************************************/
/*	Class:	ConeEmitter	
/*	Desc:	Emits particles inside cone
/*****************************************************************************/
class ConeEmitter : public ParticleEmitter
{
public:
							ConeEmitter ();
	
	virtual void			Expose		( PropertyMap& pm );
	virtual void			Serialize	( OutStream& os ) const;
	virtual void			Unserialize	( InStream& is  );
	virtual void			Render		();
	Cone					GetCone		() const;
	
	NODE(ConeEmitter,ParticleEmitter,COEM);

protected:
	virtual void			OnEmit		( Particle& p );

private:
	float					m_Angle;		//  cone angle
	float					m_Height;		//  cone height
	bool					m_bPlanar;		//  when true emit only in xOy plane

}; // class ConeEmitter

/*****************************************************************************/
/*	Class:	LineEmitter	
/*	Desc:	Emits particles from the line
/*****************************************************************************/
class LineEmitter : public ParticleEmitter
{
public:
							LineEmitter ();

	virtual void			Expose		( PropertyMap& pm );
	virtual void			Serialize	( OutStream& os ) const;
	virtual void			Unserialize	( InStream& is  );
	virtual void			Render		();

	Vector3D				GetBeg		() const { return GetPos(); }
	Vector3D				GetEnd		() const;

	NODE(LineEmitter,ParticleEmitter,LIEM);

protected:
	virtual void			OnEmit		( Particle& p );

private:
	float					m_Length;		//  line length

}; // class LineEmitter

/*****************************************************************************/
/*	Class:	PointEmitter	
/*	Desc:	Emits particles from the point
/*****************************************************************************/
class PointEmitter : public ParticleEmitter
{
public:
							PointEmitter() : m_bAffectRotation( true ){}
	virtual void			Render		();
	virtual void			Expose		( PropertyMap& pm );
	virtual void			Serialize	( OutStream& os ) const;
	virtual void			Unserialize	( InStream& is  );


	NODE(PointEmitter,ParticleEmitter,POEM);

protected:
	virtual void			OnEmit		( Particle& p );
	virtual void			OnBeginEmit	();

private:
	bool					m_bAffectRotation;
	Plane					m_ScreenPlane;
	Vector3D				m_PlaneUpVec;
	Vector3D				m_PlaneNVec;

}; // class PointEmitter

/*****************************************************************************/
/*	Class:	SphereEmitter	
/*	Desc:	Emits particles
/*****************************************************************************/
class SphereEmitter : public ParticleEmitter
{
public:
							SphereEmitter	();
	virtual void			Expose			( PropertyMap& pm );
	virtual void			Serialize		( OutStream& os ) const;
	virtual void			Unserialize		( InStream& is  );
	virtual void			Render			();

	Sphere					GetSphere		();

	NODE(SphereEmitter,ParticleEmitter,SPEM);

protected:
	virtual void			OnEmit			( Particle& p );

private:
	float					m_Radius;		//  sphere radius
	bool					m_bSurface;		//  when true emitting from surface
	bool					m_bPlanar;		//  emit from a plane (disk/circle)
}; // class SphereEmitter

class Terrain;
/*****************************************************************************/
/*	Class:	TerrainEmitter	
/*	Desc:	Emits particles from terrain
/*****************************************************************************/
class TerrainEmitter : public ParticleEmitter
{
public:
							TerrainEmitter	();
	virtual void			Expose			( PropertyMap& pm );
	virtual void			Serialize		( OutStream& os ) const;
	virtual void			Unserialize		( InStream& is  );
	virtual void			Render			();

	NODE(TerrainEmitter,ParticleEmitter,TEEM);

protected:
	virtual void			OnEmit			( Particle& p );
	virtual void			OnBeginEmit		();


private:
	float					m_Radius;		//  emit radius
}; // class TerrainEmitter

/*****************************************************************************/
/*	Class:	BoxEmitter	
/*	Desc:	Emits particles from box domain
/*****************************************************************************/
class BoxEmitter : public ParticleEmitter
{
public:
							BoxEmitter	();
	virtual void			Expose		( PropertyMap& pm );
	virtual void			Serialize	( OutStream& os ) const;
	virtual void			Unserialize	( InStream& is  );
	virtual void			Render		();

	AABoundBox				GetBox		();

	NODE(BoxEmitter,ParticleEmitter,BOEM);

protected:
	virtual void			OnEmit			( Particle& p );

private:
	Vector3D				m_Dimensions;		//  dox dimensions
	bool					m_bSurface;			//  when true emitting from surface
	bool					m_bPlanar;			//  emit from a plane (rectangle)
}; // class BoxEmitter

/*****************************************************************************/
/*	Class:	CylinderEmitter	
/*	Desc:	Emits particles from cylinder domain
/*****************************************************************************/
class CylinderEmitter : public ParticleEmitter
{
public:
	CylinderEmitter	();
	virtual void			Expose			( PropertyMap& pm );
	virtual void			Serialize		( OutStream& os ) const;
	virtual void			Unserialize		( InStream& is  );
	virtual void			Render			();
	Cylinder				GetCylinder		();

	NODE(CylinderEmitter,ParticleEmitter,CYEM);

protected:
	virtual void			OnEmit			( Particle& p );

private:
	float					m_Radius;		//  cylinder radius
	float					m_Height;		//  cylinder height
	bool					m_bCapped;

	bool					m_bSurface;		//  when true emitting from surface
}; // class CylinderEmitter

/*****************************************************************************/
/*	Class:	MeshEmitter	
/*	Desc:	Emits particles from the complex mesh
/*****************************************************************************/
class MeshEmitter : public ParticleEmitter
{
public:
	enum EmitMode
	{
		emUnknown		= 0,
		emVertices		= 1,
		emEdgeCenters	= 2,
		emEdges			= 3,
		emFaceCenters	= 4,
		emFaces			= 5
	}; // enum EmitMode
					
							MeshEmitter ();
	virtual void			Render		();
	virtual void			Expose		( PropertyMap& pm );
	virtual void			Serialize	( OutStream& os ) const;
	virtual void			Unserialize	( InStream& is  );

	NODE(MeshEmitter,ParticleEmitter,MEEM);

protected:
	virtual void			OnEmit		( Particle& p );
	virtual void			OnBeginEmit	();

private:
	Geometry*				m_pMesh;
	EmitMode				m_EmitMode;
}; // class MeshEmitter

/*****************************************************************************/
/*	Class:	TargetEmitter	
/*	Desc:	Emits particles consequtively on the segment joining two points
/*****************************************************************************/
class TargetEmitter : public ParticleEmitter
{
public:
							TargetEmitter	();

	virtual void			Expose			( PropertyMap& pm );
	virtual void			Serialize		( OutStream& os ) const;
	virtual void			Unserialize		( InStream& is  );
	virtual void			Render			();
	virtual void			Emit			();

	Vector3D				GetBeg			() const { return GetPos(); }
	Vector3D				GetEnd			() const;

	NODE(TargetEmitter,ParticleEmitter,TGEM);

protected:

private:
	
	TransformNode*			m_pTarget;
	bool					m_bEndPoints;

}; // class TargetEmitter

}; // namespace sg
ENUM( sg::MeshEmitter::EmitMode, "EmitMode", 
	 	en_val( sg::MeshEmitter::emUnknown,			"Unknown"		) <<
	 	en_val( sg::MeshEmitter::emVertices,		"Vertices"		) <<
	 	en_val( sg::MeshEmitter::emEdgeCenters,		"EdgeCenters"	) <<
	 	en_val( sg::MeshEmitter::emEdges,			"Edges"			) <<
	 	en_val( sg::MeshEmitter::emFaceCenters,		"FaceCenters"	) <<
	 	en_val( sg::MeshEmitter::emFaces,			"Faces"			) );
namespace sg{

}; // namespace sg

#endif __SGPARTICLEEMITTERS_H__
