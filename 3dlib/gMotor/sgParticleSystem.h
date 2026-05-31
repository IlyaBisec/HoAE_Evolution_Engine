/*****************************************************************************/
/*	File:	sgParticleSystem.h
/*	Author:	Ruslan Shestopalyuk
/*	Date:	08-28-2003
/*****************************************************************************/
#ifndef __SGPARTICLESYSTEM_H__
#define __SGPARTICLESYSTEM_H__

#include "IMediaManager.h"
#include <stack>

namespace sg{
/*****************************************************************************/
/*	Class:	Particle
/*	Desc:	Single particle
/*****************************************************************************/
struct Particle
{
	Vector3D 		m_Pos;			//  particle position
	Vector3D 		m_PrevPos;		//  previous particle position

	Vector3D 		m_Size;			//  size in 3 directions (z is billboard axis)
	Vector3D 		m_Velocity;		//  velocity vector

	Vector3D 		m_RotAxis;		//  rotation axis
	float			m_Rotation;		//  particle's rotation around rotation axis
	float 			m_AngVelocity;	//  angular velocity around rotation axis

	Vector4D 		m_Color;		//  particle color (r,g,b,a)
	
	float			m_TimeToLive;	//  particle's time to live, in ms
	float			m_Age;			//  particle's current age, in ms

	Rct				m_UV;			//  texture coordinates
	float			m_AnimFactor;	//  auxiliary particle animation factor

	DWORD			m_Index;
	DWORD			m_PrevParticle; //  previous particle in chain
	DWORD			m_NextParticle; //  next particle in chain

					Particle		() : m_TimeToLive(0.0f){}
	void			Init			();
	void			Init			( const Particle& mean, const Particle& deviation );

	void			Update			( float dt );
	
	bool			IsDead			() const { return m_Age >= m_TimeToLive; }
	void			Kill			()		 { m_Age = m_TimeToLive;		 }

	bool			IsFree			() const { return m_TimeToLive <= 0.0f;  }
	void			Free			()		 { m_TimeToLive = 0.0f;			 }

	float			GetAlpha		() const { return m_Color.w;			}
	float			GetVelocity		() const { return m_Velocity.norm();	}
	
	_inl Matrix4D	GetTransform	() const;

	Particle&		operator *=		( float val );
	Particle&		operator +=		( const Particle& p );

	void  SetColor( DWORD color ) 
	{ 
		ColorValue::FromARGB( color, m_Color.w, m_Color.x, m_Color.y, m_Color.z ); 
	}

	DWORD GetColor() const 
	{ 
		return ColorValue::ToARGB( m_Color.w, m_Color.x, m_Color.y, m_Color.z ); 
	}
}; // class Particle

OutStream&	operator<<( OutStream& os, const Particle& p );
InStream&	operator>>( InStream& is, Particle& p );

/*****************************************************************************/
/*	Class:	ParticleCluster
/*	Desc:	Group of particles
/*****************************************************************************/
class ParticleCluster : public TransformNode
{
public:
							ParticleCluster		();
	virtual					~ParticleCluster	();
	virtual void			Expose				( PropertyMap& pm );
	virtual void			Render				();
	virtual void			Flush				(){}

	void					Init				();

	void					SetMaxParticles		( int val );
	int						GetMaxParticles		() const { return m_MaxParticles; }

	void					Serialize			( OutStream& os ) const;
	void					Unserialize			( InStream& is  );

	void					UpdateParticles 	( float dt );

	Particle*				AddParticle			();
	Particle*				GetParticle			( int idx ) { return &m_Particles[idx]; }
	bool					IsWorldSpace		() const	{ return m_bWorldSpace; }

	NODE(ParticleCluster,TransformNode,PCLU);

	bool					m_bNeedDraw;	  //  hackery	

protected:
	int						m_MaxParticles;	  //  maximal number of particles allowed in cluster
	int						m_LastAdded;	  //  index of the least recently added particle

	bool					m_bOverwrite;	  //  whether overwrite yet alive particles
	bool					m_bWorldSpace;	  //  whether particle coordinates are world space
	bool					m_bDieOnFade;	  //  particle die when alpha == 0

	DWORD					m_LastUpdateFrame;
	Particle*				m_Particles;	  //  array of particles	
}; // class ParticleCluster

/*****************************************************************************/
/*	Class:	ParticleEmitter	
/*	Desc:	Emits particles
/*****************************************************************************/
class ParticleEmitter : public TransformNode
{
public:
							ParticleEmitter	();

	virtual void			Render			();
	virtual void			Expose			( PropertyMap& pm );
	virtual void			Serialize		( OutStream& os ) const;
	virtual void			Unserialize		( InStream& is  );

	virtual void			Emit			();
	int						NumToEmit		();

	DWORD					GetColor		() const;
	void					SetColor		( DWORD color );
	float					GetEmitFactor	() const { return m_EmitFactor; }
	void					SetEmitFactor	( float val ) { m_EmitFactor = val; }
	
	NODE(ParticleEmitter,TransformNode,PEMI);

protected:
	Particle				m_Initial;		//  initial particles state
	Particle				m_Deviation;	//  random value range

	float					m_Rate;			//  emit rate (particles per second)
	float					m_Velocity;		//  scalar multiplier of emitting velocity
	float					m_VelocityD;	//  velocity deviation

	bool					m_bPeriodic;	//  if emits periodically all at once
	float					m_Period;		//  emit period, if emitting periodically

	float					m_EmitFactor;  
	DWORD					m_LastUpdateFrame;
	int						m_NumToEmit;

	virtual void			OnEmit			( Particle& p ){}
	virtual void			OnBeginEmit		(){}

}; // class ParticleEmitter

/*****************************************************************************/
/*	Class:	ParticleAffector	
/*	Desc:	Influences particles in some way
/*****************************************************************************/
class ParticleAffector : public TransformNode
{
public:
							ParticleAffector();

	virtual void			Render();
	virtual void			Expose( PropertyMap& pm );

	NODE(ParticleAffector,TransformNode,PAFF);

protected:
	DWORD					m_LastUpdateFrame;

}; // class ParticleAffector

/*****************************************************************************/
/*	Class:	ParticleSystem	
/*	Desc:	Complex set of the particle effect parts
/*****************************************************************************/
class ParticleSystem : public Animation, public IParticleSystem
{
public:
					ParticleSystem			();

	virtual void	Render					();
	virtual void	Expose					( PropertyMap& pm );

	//  from IParticleSystem
	virtual void	SetEmitRateMultiplier	( float val ) { m_EmitRateMultiplier	= val; }
	virtual void	SetAlphaMultiplier		( float val ) { m_AlphaMultiplier		= val; }
	virtual void	SetBirthAlphaMultiplier	( float val ) { m_BirthAlphaMultiplier	= val; }
	virtual void	SetTTLMultiplier		( float val ) { m_TTLMultiplier			= val; }

	float			GetEmitRateMultiplier	() const { return m_EmitRateMultiplier;		}
	float			GetAlphaMultiplier		() const { return m_AlphaMultiplier;		}
	float			GetBirthAlphaMultiplier	() const { return m_BirthAlphaMultiplier;	}
	float			GetTTLMultiplier		() const { return m_TTLMultiplier;			}
	bool			NeedUpdate				() const { return m_bNeedUpdate;			}
	
	float			GetTimeDelta			() const { return m_UpdateRate;				}

	static ParticleSystem* GetCurPS			() { return s_CurrentPS.top(); }

	NODE(ParticleSystem,Animation,PSYS);

protected:
	float			m_EmitRateMultiplier;
	float			m_AlphaMultiplier;
	float			m_BirthAlphaMultiplier;
	float			m_TTLMultiplier;

	float			m_UpdateRate;
	float			m_TimeDelta;
	bool			m_bNeedUpdate;

	static void		PushPS					( ParticleSystem* pPS ) { s_CurrentPS.push( pPS ); }
	static void		PopPS					() { s_CurrentPS.pop(); }

	static std::stack<ParticleSystem*>		s_CurrentPS;
}; // class ParticleSystem


/*****************************************************************************/
/*	Class:	ParticleManager	
/*	Desc:	Manages all particle effects in the scene
/*****************************************************************************/
class ParticleManager : public Node, public PSingleton<ParticleManager>, 
						public IParticleManager
{
public:
	ParticleManager					();
	virtual void	Expose			( PropertyMap& pm );
	virtual void	Render			();
	virtual void	EnableBatching	( bool bEnable = true ) { m_bBatchClusters = bEnable; }

	bool			AddCluster		( ParticleCluster* pCluster, const Matrix4D& tm );
	void			ResetClusters	();

	virtual void	AddQuad			( const Vector3D& pos, const Vector3D& rot,
										float scale, DWORD color, 
										const Rct& uv, int texID );
	virtual void	AddBillboard	( const Vector3D& pos, float dir, float scale, 
										DWORD color, const Rct& uv, int texID );

	NODE(ParticleManager,Node,PAMG);

protected:
	
	struct DrawCluster
	{
		DrawCluster() : m_pCluster( NULL ) {}
		DrawCluster( ParticleCluster* pCluster, const Matrix4D& tm ) : 
							m_pCluster( pCluster ), m_Transform( tm ) {}
		ParticleCluster*	m_pCluster;
		Matrix4D			m_Transform;
	};

	std::vector<DrawCluster>	m_Clusters;
	bool						m_bBatchClusters;

}; // class ParticleManager

}; // namespace sg

#ifdef _INLINES
#include "sgParticleSystem.inl"
#endif // _INLINES

#endif __SGPARTICLESYSTEM_H__
