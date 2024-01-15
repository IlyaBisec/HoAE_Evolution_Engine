/*****************************************************************************/
/*	File:	sgEffect.h
/*	Author:	Ruslan Shestopalyuk
/*	Date:	08-28-2003
/*****************************************************************************/
#ifndef __SGEFFECT_H__
#define __SGEFFECT_H__

namespace sg{

/*****************************************************************************/
/*	Class:	PParticle	
/*	Desc:	Single particle instance
/*****************************************************************************/
class PParticle
{
	Vector3D		m_Position;
	Vector3D		m_Velocity;
	Vector3D		m_Size;
	
	float			m_Yaw;
	float			m_Pitch;
	float			m_Roll;
	
	DWORD			m_Color;
	float			m_Age;
	float			m_TimeToLive;

	DWORD			m_Index;
	PParticle*		m_pPrev;
	PParticle*		m_pNext;
public:
}; // class PParticle

class PEmitter;
/*****************************************************************************/
/*	Class:	PEmitterInstance	
/*	Desc:	Single particle emitter instance
/*****************************************************************************/
class PEmitterInstance
{
	PEmitter*			m_pTemplate;
	PParticle*			m_Particle;

}; // class PEmitterInstance

/*****************************************************************************/
/*	Class:	PEmitter	
/*	Desc:	Creates particles at given place with given rate
/*****************************************************************************/
class PEmitter : public TransformNode
{
	int						m_MaxParticles;

public:
							PEmitter		();
	virtual void			Render			();
	virtual void			Expose			( PropertyMap& pm );
	virtual void			Serialize		( OutStream& os ) const;
	virtual void			Unserialize		( InStream& is  );

	NODE(PEmitter,TransformNode,2EMI);
protected:
}; // class PEmitter

/*****************************************************************************/
/*	Class:	PEffect	
/*	Desc:	Creates particles at given place with given rate
/*****************************************************************************/
class PEffect : public Animation
{
public:
							PEffect			();
	virtual void			Render			();
	virtual void			Expose			( PropertyMap& pm );
	virtual void			Serialize		( OutStream& os ) const;
	virtual void			Unserialize		( InStream& is  );

	NODE(PEffect,TransformNode,2EFF);
protected:
}; // class PEffect

/*****************************************************************************/
/*	Class:	POperator	
/*	Desc:	Influences particles in some way
/*****************************************************************************/
class POperator : public Node
{
public:
							POperator		();
	virtual void			Render			();
	virtual void			Expose			( PropertyMap& pm );

	NODE(POperator,TransformNode,2POP);

protected:
}; // class POperator

/*****************************************************************************/
/*	Class:	EffectManager	
/*	Desc:	Manages all particle effects in the scene
/*****************************************************************************/
class EffectManager : public Node, public PSingleton<EffectManager>
{
public:
							EffectManager	();
	virtual void			Expose			( PropertyMap& pm );
	virtual void			Render			();

	NODE(EffectManager,Node,2EFM);

protected:
}; // class EffectManager

}; // namespace sg

#ifdef _INLINES
#include "sgEffect.inl"
#endif // _INLINES

#endif __SGEFFECT_H__
