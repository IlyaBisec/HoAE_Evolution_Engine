/*****************************************************************************/
/*	File:	sgParticleAffectors.h
/*	Author:	Ruslan Shestopalyuk
/*	Date:	08-28-2003
/*****************************************************************************/
#ifndef __SGPARTICLEAFFECTORS_H__
#define __SGPARTICLEAFFECTORS_H__

namespace sg{
/*****************************************************************************/
/*	Class:	DeltaAffector	
/*	Desc:	Influences particles changing their attributes by delta values
/*****************************************************************************/
class DeltaAffector : public ParticleAffector
{
public:
							DeltaAffector	();

	virtual void			Render			();
	virtual void			Expose			( PropertyMap& pm );
	virtual void			Serialize		( OutStream& os ) const;
	virtual void			Unserialize		( InStream& is  );

	NODE(DeltaAffector,ParticleAffector,DAFF);

protected:

	Particle				m_Delta;	//  difference value			
}; // class DeltaAffector

/*****************************************************************************/
/*	Class:	ColorAnimateAffector	
/*	Desc:	Changes particle color using keyframed values
/*****************************************************************************/
class ColorAnimateAffector : public ParticleAffector
{
public:
							ColorAnimateAffector	();
	virtual void			Render					();
	virtual void			Expose					( PropertyMap& pm );
	virtual void			Serialize				( OutStream& os ) const;
	virtual void			Unserialize				( InStream& is  );

	const ColorAnimationCurve*	GetColorCurve			() const { return &m_Curve; }
	void						SetColorCurve			( const ColorAnimationCurve* pCurve ) { m_Curve = *pCurve; }

	NODE(ColorAnimateAffector,ParticleAffector,CANF);

protected:
	ColorAnimationCurve		m_Curve;
	bool					m_bRandomInitColor;
}; // class ColorAnimateAffector

/*****************************************************************************/
/*	Class:	SizeAnimateAffector	
/*	Desc:	Changes particle size using keyframed values
/*****************************************************************************/
class SizeAnimateAffector : public ParticleAffector
{
public:
							SizeAnimateAffector		();
	virtual void			Render					();
	virtual void			Expose					( PropertyMap& pm );
	virtual void			Serialize				( OutStream& os ) const;
	virtual void			Unserialize				( InStream& is  );

	const FloatAnimationCurve*	GetSizeCurve			() const { return &m_Curve; }
	void						SetSizeCurve			( const FloatAnimationCurve* pCurve ) { m_Curve = *pCurve; }


	NODE(SizeAnimateAffector,ParticleAffector,SZNF);

protected:
	bool					m_bAffectX;
	bool					m_bAffectY;
	bool					m_bAffectZ;

	bool					m_bRandomInitSize;

	FloatAnimationCurve		m_Curve;
}; // class SizeAnimateAffector

/*****************************************************************************/
/*	Class:	UVAnimateAffector	
/*	Desc:	Changes particle texture coordinates
/*	Rmrk:	Texture frames are packed onto single texture
/*****************************************************************************/
class UVAnimateAffector : public ParticleAffector
{
public:
							UVAnimateAffector		();
	virtual void			Render					();
	virtual void			Expose					( PropertyMap& pm );
	virtual void			Serialize				( OutStream& os ) const;
	virtual void			Unserialize				( InStream& is  );

	NODE(UVAnimateAffector,ParticleAffector,UVAA);

protected:
	int						m_NRows;		//  number of frame rows
	int						m_NCols;		//  number of frame columns
	float					m_Rate;			//  rate of frame changing, fps
	bool					m_bAsync;		//  do asynchronous animation
}; // class UVAnimateAffector

/*****************************************************************************/
/*	Class:	UVInitAffector	
/*	Desc:	Initializes particle texture coordinates
/*	Rmrk:	Texture frames are packed onto single texture
/*****************************************************************************/
class UVInitAffector : public ParticleAffector
{
public:
							UVInitAffector			();
	virtual void			Render					();
	virtual void			Expose					( PropertyMap& pm );
	virtual void			Serialize				( OutStream& os ) const;
	virtual void			Unserialize				( InStream& is  );

	NODE(UVInitAffector,ParticleAffector,UVIA);

protected:
	int						m_NRows;		//  number of frame rows
	int						m_NCols;		//  number of frame columns
	int						m_Row;			//  init row
	int						m_Col;			//  init column
	bool					m_bRandomize;	//  when true we pick cell randomly
}; // class UVInitAffector

/*****************************************************************************/
/*	Class:	HitAffector	
/*	Desc:	Influences particles deflecting them from something
/*****************************************************************************/
class HitAffector : public ParticleAffector
{
public:
	enum HitAction
	{
		haUnknown	= 0,
		haDie		= 1,
		haDeflect	= 2,
		haCustom	= 3
	};
							HitAffector	();

	virtual void			Expose			( PropertyMap& pm );
	virtual void			Serialize		( OutStream& os ) const;
	virtual void			Unserialize		( InStream& is  );

	NODE(HitAffector,ParticleAffector,DEFA);

protected:

	HitAction				m_HitAction;
	float					m_Damping;
}; // class HitAffector

}; // namespace sg
ENUM( sg::HitAffector::HitAction, "HitAction", 
	 en_val( sg::HitAffector::haUnknown,		"Unknown"		) <<
	 en_val( sg::HitAffector::haDie,			"Die"			) <<
	 en_val( sg::HitAffector::haDeflect,		"Deflect"		) <<
	 en_val( sg::HitAffector::haCustom,			"Custom"		) );

namespace sg{
/*****************************************************************************/
/*	Class:	PlaneHitAffector	
/*	Desc:	Influences particles deflecting them from plane
/*****************************************************************************/
class PlaneHitAffector : public HitAffector
{
public:
	PlaneHitAffector	();

	virtual void			Render			();
	virtual void			Expose			( PropertyMap& pm );
	virtual void			Serialize		( OutStream& os ) const;
	virtual void			Unserialize		( InStream& is  );

	NODE(PlaneHitAffector,HitAffector,PLDA);

protected:
}; // class PlaneHitAffector

/*****************************************************************************/
/*	Class:	TerrainHitAffector	
/*	Desc:	Influences particles deflecting them from terrain
/*****************************************************************************/
class TerrainHitAffector : public HitAffector
{
public:
	TerrainHitAffector	();

	virtual void			Render			();
	virtual void			Expose			( PropertyMap& pm );
	virtual void			Serialize		( OutStream& os ) const;
	virtual void			Unserialize		( InStream& is  );

	NODE(TerrainHitAffector,HitAffector,TRHA);

protected:
}; // class TerrainHitAffector

/*****************************************************************************/
/*	Class:	SphereProjectAffector	
/*	Desc:	Restricts particle movement to spherical shape
/*****************************************************************************/
class SphereProjectAffector : public ParticleAffector
{
public:
							SphereProjectAffector	();
	virtual void			Expose					( PropertyMap& pm );
	virtual void			Serialize				( OutStream& os ) const;
	virtual void			Unserialize				( InStream& is  );
	virtual void			Render					();

	NODE(SphereProjectAffector,ParticleAffector,SPRF);

protected:
	float					m_SphereRadius;
	bool					m_bPreserveVelocity;
}; // class SphereProjectAffector

/*****************************************************************************/
/*	Class:	ForceAffector	
/*	Desc:	Applies force to the particle
/*****************************************************************************/
class ForceAffector : public ParticleAffector
{
public:
	ForceAffector	();

	virtual void			Render			();
	virtual void			Expose			( PropertyMap& pm );
	virtual void			Serialize		( OutStream& os ) const;
	virtual void			Unserialize		( InStream& is  );

	NODE(ForceAffector,ParticleAffector,FAFF);

protected:
	
	Vector3D				m_Force;
}; // class ForceAffector

/*****************************************************************************/
/*	Class:	ExplodeAffector	
/*	Desc:	Accelerates particles from the explosion center
/*****************************************************************************/
class ExplodeAffector : public ParticleAffector
{
public:
							ExplodeAffector	();

	virtual void			Render			();
	virtual void			Expose			( PropertyMap& pm );
	virtual void			Serialize		( OutStream& os ) const;
	virtual void			Unserialize		( InStream& is  );

	NODE(ExplodeAffector,ParticleAffector,EXFF);

protected:

	float				m_Intensity;
}; // class ExplodeAffector

/*****************************************************************************/
/*	Class:	RandomTorqueAffector	
/*	Desc:	Applies turbulence to the particle
/*****************************************************************************/
class RandomTorqueAffector : public ParticleAffector
{
public:
	RandomTorqueAffector	();

	virtual void			Render			();
	virtual void			Expose			( PropertyMap& pm );
	virtual void			Serialize		( OutStream& os ) const;
	virtual void			Unserialize		( InStream& is  );

	NODE(RandomTorqueAffector,ParticleAffector,TAFF);

protected:
	float					m_Intensity;
}; // class RandomTorqueAffector

/*****************************************************************************/
/*	Class:	Turbulence	
/*	Desc:	Applies turbulence to the particle
/*****************************************************************************/
class Turbulence : public ParticleAffector
{
public:
							Turbulence		();

	virtual void			Render			();
	virtual void			Expose			( PropertyMap& pm );
	virtual void			Serialize		( OutStream& os ) const;
	virtual void			Unserialize		( InStream& is  );

	NODE(Turbulence,ParticleAffector,TRFF);

protected:
	float					m_Magnitude;
	float					m_Frequency;
	Vector3D				m_Phase;

}; // class Turbulence

/*****************************************************************************/
/*	Class:	VortexAffector	
/*	Desc:	
/*****************************************************************************/
class VortexAffector : public ParticleAffector
{
public:
							VortexAffector	();

	virtual void			Render			();
	virtual void			Expose			( PropertyMap& pm );
	virtual void			Serialize		( OutStream& os ) const;
	virtual void			Unserialize		( InStream& is  );

	NODE(VortexAffector,ParticleAffector,VOFF);

protected:
	float					m_Intensity;
	float					m_CenterAttraction;
}; // class VortexAffector

/*****************************************************************************/
/*	Class:	FollowAffector	
/*	Desc:	Makes particle to follow next particle in cluster
/*****************************************************************************/
class FollowAffector : public ParticleAffector
{
public:
							FollowAffector	();

	virtual void			Render			();
	virtual void			Expose			( PropertyMap& pm );
	virtual void			Serialize		( OutStream& os ) const;
	virtual void			Unserialize		( InStream& is  );

	NODE(FollowAffector,ParticleAffector,FOFF);

protected:
	float					m_Intensity;
	bool					m_bCircular;

}; // class FollowAffector

/*****************************************************************************/
/*	Class:	FluidFrictionAffector	
/*	Desc:	Applies fluid friction to the particle
/*****************************************************************************/
class FluidFrictionAffector : public ParticleAffector
{
public:
							FluidFrictionAffector();

	virtual void			Render			();
	virtual void			Expose			( PropertyMap& pm );
	virtual void			Serialize		( OutStream& os ) const;
	virtual void			Unserialize		( InStream& is  );

	void					SetToAir		();
	void					SetToWater		();
	
	void					SetDensity		( float val );
	void					SetViscosity	( float val );
	_inl float				GetDensity		() const { return m_Density;	}
	_inl float				GetViscosity	() const { return m_Viscosity;	}

	NODE(FluidFrictionAffector,ParticleAffector,FFAF);

protected:
	float					m_Density;			//  fluid density
	float					m_Viscosity;		//  fluid viscosity
	bool					m_bOverrideRadius;	//	whether override radius
	float					m_ParticleRadius;	//  overriden particle radius
	
private:
	//  cached values
	float					m_A;				
	float					m_B;
}; // class FluidFrictionAffector

/*****************************************************************************/
/*	Class:	RandomForceAffector	
/*	Desc:	Applies randomized force to the particle
/*****************************************************************************/
class RandomForceAffector : public ParticleAffector
{
public:
							RandomForceAffector	();

	virtual void			Render			();
	virtual void			Expose			( PropertyMap& pm );
	virtual void			Serialize		( OutStream& os ) const;
	virtual void			Unserialize		( InStream& is  );

	NODE(RandomForceAffector,ParticleAffector,RFFF);

protected:

	Vector3D				m_ForceMean;
	Vector3D				m_ForceDev;
}; // class RandomForceAffector

/*****************************************************************************/
/*	Class:	MatchVelocityAffector	
/*	Desc:	Accelerates towards average neighborhood velocity
/*****************************************************************************/
class MatchVelocityAffector : public ParticleAffector
{
public:
	MatchVelocityAffector	();

	virtual void			Render			();
	virtual void			Expose			( PropertyMap& pm );
	virtual void			Serialize		( OutStream& os ) const;
	virtual void			Unserialize		( InStream& is  );

	NODE(MatchVelocityAffector,ParticleAffector,MVAF);

protected:
	float				m_Intensity;		//  affect intensity
	float				m_Radius;			//  neighborhood radius
}; // class MatchVelocityAffector

/*****************************************************************************/
/*	Class:	LightningAffector	
/*****************************************************************************/
class LightningAffector : public ParticleAffector
{
public:
							LightningAffector	();
	virtual void			Render				();
	virtual void			Expose				( PropertyMap& pm );
	virtual void			Serialize			( OutStream& os ) const;
	virtual void			Unserialize			( InStream& is  );

	NODE(LightningAffector,ParticleAffector,LIFF);

protected:

	TransformNode*			m_pStartTM;
	TransformNode*			m_pEndTM;
	
	float					m_Phase;

	float					m_Spread;
	float					m_Frequency;
	bool					m_bAffectEndpoints;
}; // class LightningAffector

struct AnimationInstance
{
	Animation*		m_pAnimation;
	float			m_CurTime;
	float			m_EmitFactor;
	Matrix4D		m_Transform;
}; // struct AnimationInstance

/*****************************************************************************/
/*	Class:	ChainEffectAffector	
/*	Desc:	Starts new animations for cluster's particles, for which
/*				some event was triggered (death, for example). These new 
/*				animations can be also particle systems
/*****************************************************************************/
class ChainEffectAffector : public ParticleAffector
{
public:
							ChainEffectAffector	();
	virtual void			Render				();
	virtual void			Expose				( PropertyMap& pm );
	virtual void			Serialize			( OutStream& os		) const;
	virtual void			Unserialize			( InStream& is		);

	void					SetMaxInstances		( int val );
	int						GetMaxInstances		() const { return m_MaxInstances; }

	NODE(ChainEffectAffector,ParticleAffector,CHAF);

protected:
	int								m_MaxInstances;			//  maximal number of animations can be born
	float							m_TriggerProbability;	

	Animation*						m_pAnimation;
	std::vector<AnimationInstance>	m_Instances;

	bool AddAnimationInstance( Animation* pAnim, const Matrix4D& tm );

}; // class ChainEffectAffector

}; // namespace sg

#endif __SGPARTICLEAFFECTORS_H__
