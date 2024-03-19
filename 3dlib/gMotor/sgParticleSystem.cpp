/*****************************************************************************/
/*	File:	sgParticleSystem.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	08-28-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include "kIOHelpers.h"
#include "sgParticleSystem.h"
#include "sgStateBlock.h"

#ifndef _INLINES
#include "sgParticleSystem.inl"
#endif // !_INLINES

BEGIN_NAMESPACE(sg)
/*****************************************************************************/
/*	Particle implementation
/*****************************************************************************/
OutStream&	operator<<( OutStream& os, const Particle& p )
{
	os << p.m_Pos << p.m_PrevPos << p.m_Size << p.m_Velocity << 
		p.m_RotAxis << p.m_Rotation << p.m_AngVelocity <<
		p.m_Color << p.m_TimeToLive << p.m_UV << p.m_AnimFactor;			
	return os;
}

InStream&	operator>>( InStream& is, Particle& p )
{
	is >> p.m_Pos >> p.m_PrevPos >> p.m_Size >> p.m_Velocity >> 
		p.m_RotAxis >> p.m_Rotation >> p.m_AngVelocity >>
		p.m_Color >> p.m_TimeToLive >> p.m_UV >> p.m_AnimFactor;	
	return is;
}

void Particle::Init()
{
	m_PrevPos		= Vector3D::null;
	m_Pos			= Vector3D::null;			
	m_Size			= Vector3D::null;			
	m_Velocity		= Vector3D::null;		

	m_RotAxis		= Vector3D::oZ;
	m_Rotation		= 0.0f;		
	m_AngVelocity	= 0.0f;	

	m_Color			= Vector4D( 1.0f, 1.0f, 1.0f, 1.0f );
	m_TimeToLive	= 0.0f;	
	m_Age			= 0.0f;
	m_UV			= Rct::unit;			
	m_AnimFactor			= 0.0f;			
} // Particle::Init

void Particle::Init( const Particle& mean, const Particle& deviation )
{
	m_PrevPos		= mean.m_PrevPos;
	m_Pos			= mean.m_Pos;			
	m_Velocity		= mean.m_Velocity;			
	m_Size.x		= rndValuef( mean.m_Size.x - deviation.m_Size.x, 
									 mean.m_Size.x + deviation.m_Size.x );		
	m_Size.y		= rndValuef( mean.m_Size.y - deviation.m_Size.y, 
									 mean.m_Size.y + deviation.m_Size.y );		
	m_Size.z		= rndValuef( mean.m_Size.z - deviation.m_Size.z, 
									 mean.m_Size.z + deviation.m_Size.z );		

	m_RotAxis		= mean.m_RotAxis;
	m_Rotation		= rndValuef( mean.m_Rotation - deviation.m_Rotation, 
								 mean.m_Rotation + deviation.m_Rotation );		
	m_AngVelocity	= rndValuef( mean.m_AngVelocity - deviation.m_AngVelocity, 
								 mean.m_AngVelocity + deviation.m_AngVelocity );		

	m_Color.x		= rndValuef( mean.m_Color.x - deviation.m_Color.x, 
								 mean.m_Color.x + deviation.m_Color.x );		
	m_Color.y		= rndValuef( mean.m_Color.y - deviation.m_Color.y, 
								 mean.m_Color.y + deviation.m_Color.y );		
	m_Color.z		= rndValuef( mean.m_Color.z - deviation.m_Color.z, 
								 mean.m_Color.z + deviation.m_Color.z );		
	m_Color.w		= rndValuef( mean.m_Color.w - deviation.m_Color.w, 
								 mean.m_Color.w + deviation.m_Color.w );		

	m_TimeToLive	= rndValuef( mean.m_TimeToLive - deviation.m_TimeToLive, 
								  mean.m_TimeToLive + deviation.m_TimeToLive );		

	m_UV			= mean.m_UV;			
	m_AnimFactor			= mean.m_AnimFactor;	
	m_Age			= 0.0f;

	m_PrevParticle	= m_NextParticle = 0xFFFFFFFF;
} // Particle::Init

void Particle::Update( float dt )
{
	m_PrevPos		= m_Pos;
	m_Pos.x		+= dt * m_Velocity.x;
	m_Pos.y		+= dt * m_Velocity.y;
	m_Pos.z		+= dt * m_Velocity.z;
	m_Rotation	+= dt * m_AngVelocity;
	m_Age		+= dt;
} // Particle::Update

Particle& Particle::operator *=( float val )
{
	m_PrevPos		= m_Pos;
	m_Pos			*= val;		
	m_Size			*= val;		
	m_Velocity		*= val;	

	m_Rotation		*= val;
	m_AngVelocity	*= val;

	m_Color			*= val;	

	m_TimeToLive	*= val;
	m_UV			*= val;
	m_AnimFactor	*= val;
	return *this;
} // Particle::operator *=

Particle& Particle::operator +=( const Particle& p )
{
	m_PrevPos		= m_Pos;
	m_Pos			+= p.m_Pos;	
	m_Size			+= p.m_Size;	
	m_Velocity		+= p.m_Velocity;	

	m_Rotation		+= p.m_Rotation;
	m_AngVelocity	+= p.m_AngVelocity;

	m_Color			+= p.m_Color;	

	m_TimeToLive	+= p.m_TimeToLive;
	m_UV			+= p.m_UV;
	m_AnimFactor	+= p.m_AnimFactor;	
	return *this;
} // Particle::operator +=

/*****************************************************************************/
/*	ParticleEmitter	implementation
/*****************************************************************************/
ParticleEmitter::ParticleEmitter()
{
	m_Initial.Init();
	m_Deviation.Init();

	m_Initial.m_Size		= Vector3D( 10.0f, 10.0f, 10.0f );
	m_Initial.m_TimeToLive	= 500.0f;

	m_Initial.m_Color		= Vector4D( 0.5f, 0.5f, 0.5f, 0.5f );
	m_Deviation.m_Color		= Vector4D( 0.5f, 0.5f, 0.5f, 0.5f );

	m_Rate					= 0.01f;		
	m_Velocity				= 0.5f;
	m_VelocityD				= 0.0f;
	m_EmitFactor				= 0.0f;
	m_LastUpdateFrame		= 0;
	m_NumToEmit				= 0;

	m_bPeriodic				= false;
	m_Period				= 1000.0f;

	m_Initial.SetColor( 0xFFFFFFFF );

} // ParticleEmitter::ParticleEmitter

void ParticleEmitter::Render()
{
	PushTM( tm );
	Emit();
	//Node::Render();
	PopTM();
}

void ParticleEmitter::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "ParticleEmitter", this );

	//pm.section( "Texture Coordinates" );
	pm.f( "uvLeft",   		m_Initial.m_UV.x			);
	pm.f( "uvTop",    		m_Initial.m_UV.y 			);
	pm.f( "uvWidth",  		m_Initial.m_UV.w 			);
	pm.f( "uvHeight", 		m_Initial.m_UV.h 			);
	
	//pm.section( "Rotation" );
	pm.f( "Rotation",		m_Initial.m_Rotation		);
	pm.f( " dRotation",		m_Deviation.m_Rotation		);

	pm.f( "AngVelocity",	m_Initial.m_AngVelocity		);
	pm.f( " dAngVelocity",	m_Deviation.m_AngVelocity	);

	//pm.section( "Size" );
	pm.f( "SizeX",			m_Initial.m_Size.x			);
	pm.f( "SizeY",			m_Initial.m_Size.y			);
	pm.f( "SizeZ",			m_Initial.m_Size.z			);
	pm.f( " dSizeX",		m_Deviation.m_Size.x		);
	pm.f( " dSizeY",		m_Deviation.m_Size.y		);
	pm.f( " dSizeZ",		m_Deviation.m_Size.z		);

	//pm.section( "Color" );
	pm.p( "Color",			GetColor, SetColor, "color" );
	pm.f( " dAlpha",		m_Deviation.m_Color.w		);
	pm.f( " dRed",			m_Deviation.m_Color.x		);
	pm.f( " dGreen",		m_Deviation.m_Color.y		);
	pm.f( " dBlue",			m_Deviation.m_Color.z		);

	//pm.section( "Emit" );
	pm.f( "TimeToLive",		m_Initial.m_TimeToLive		);
	pm.f( " dTimeToLive",	m_Deviation.m_TimeToLive	);

	pm.f( "Rate",   		m_Rate						);
	pm.f( "Velocity",   	m_Velocity					);
	pm.f( " dVelocity",   	m_VelocityD					);

	pm.f( "Periodic",		m_bPeriodic					);
	pm.f( "Period",			m_Period					);		
	pm.f( "Tag",			m_Initial.m_AnimFactor		);

} // ParticleEmitter::Expose

void ParticleEmitter::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	bool reserved = false;
	os << m_Initial << m_Deviation << m_Rate << m_bPeriodic << m_Period << m_Velocity << m_VelocityD << reserved;		
} // ParticleEmitter::Serialize

void ParticleEmitter::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
	bool reserved;
	is >> m_Initial >> m_Deviation >> m_Rate >> m_bPeriodic >> m_Period >> m_Velocity >> m_VelocityD >> reserved;	
} // ParticleEmitter::Unserialize

int	ParticleEmitter::NumToEmit()
{
	if (m_LastUpdateFrame == IRS->GetCurFrame()) return m_NumToEmit;
	m_LastUpdateFrame = IRS->GetCurFrame();
	
	if (m_bPeriodic) 
	{
		m_EmitFactor += Animation::CurTimeDelta();
		if (m_EmitFactor <= 0.0f) m_EmitFactor = 0.0f; 
		if (m_EmitFactor >= m_Period) 
		{ 
			m_EmitFactor = fmod( m_EmitFactor, m_Period ); 
			m_NumToEmit = m_Rate * m_Period;
		}
		else
		{
			m_NumToEmit = 0;
		}
		return m_NumToEmit;
	}

	float amt = m_Rate * Animation::CurTimeDelta() * ParticleSystem::GetCurPS()->GetEmitRateMultiplier();
	m_EmitFactor += amt;
	m_NumToEmit = int( m_EmitFactor );
	m_EmitFactor -= float( m_NumToEmit );
	return m_NumToEmit; 
} // ParticleEmitter::NumToEmit

DWORD ParticleEmitter::GetColor() const
{
	return m_Initial.GetColor();
}

void ParticleEmitter::SetColor( DWORD color )
{
	m_Initial.SetColor( color );
}

void ParticleEmitter::Emit()
{
	ParticleCluster* pCluster = (ParticleCluster*)GetInput( 0 );
	if (!pCluster || !pCluster->IsA<ParticleCluster>()) return;

	OnBeginEmit();
	int nParticles   = NumToEmit();
	Matrix4D worldTM = TMStackTop();

	for (int i = 0; i < nParticles; i++)
	{
		Particle* p = pCluster->AddParticle();
		if (!p) return;
		p->Init( m_Initial, m_Deviation );
		OnEmit( *p );
		float vel = rndValuef( m_Velocity - m_VelocityD, m_Velocity + m_VelocityD );
		p->m_Velocity *= vel;
		worldTM.transformPt ( p->m_Pos );
		p->m_TimeToLive *= ParticleSystem::GetCurPS()->GetTTLMultiplier();
		p->m_Color.w	*= ParticleSystem::GetCurPS()->GetBirthAlphaMultiplier();
	}

} // ParticleEmitter::Emit

/*****************************************************************************/
/*	ParticleAffector	implementation
/*****************************************************************************/
ParticleAffector::ParticleAffector()
{
	m_LastUpdateFrame = 0;
}

void ParticleAffector::Render()
{
}

void ParticleAffector::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "ParticleAffector", this );
}

/*****************************************************************************/
/*	ParticleCluster	implementation
/*****************************************************************************/
ParticleCluster::ParticleCluster()
{
	m_MaxParticles	= 0;
	m_LastAdded		= -1;
	m_Particles		= NULL;
	m_bOverwrite	= true;
	m_bWorldSpace	= true;
	m_bNeedDraw		= true;

	m_LastUpdateFrame = 0;

	SetMaxParticles( 10 );
}

ParticleCluster::~ParticleCluster()
{
	delete []m_Particles;
}

void ParticleCluster::SetMaxParticles( int val )
{
	if (val == m_MaxParticles || val < 0) return;
	delete []m_Particles;
	m_MaxParticles	= val;
	m_Particles		= new Particle[m_MaxParticles];
	m_LastAdded		= -1;
	m_MaxParticles	= val;
} // ParticleCluster::SetMaxParticles

void ParticleCluster::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "ParticleCluster", this );
	pm.p ( "MaxParticles", GetMaxParticles, SetMaxParticles );
	pm.f( "LastAdded",	m_LastAdded		);
	pm.f( "OverwriteOld", m_bOverwrite	);
	pm.f( "WorldSpace",	m_bWorldSpace	);
	pm.f( "DieOnFade",	m_bDieOnFade	);
	pm.m( "Init",		Init			);
} // ParticleCluster::Expose

void ParticleCluster::Render()
{
	if (m_LastUpdateFrame == IRS->GetCurFrame()) return;
	m_LastUpdateFrame = IRS->GetCurFrame();

	UpdateParticles( Animation::CurTimeDelta() );
	if (ParticleManager::instance()->AddCluster( this, TMStackTop() )) return;

	PushTM( tm );
	Flush();
	PopTM();
} // ParticleCluster::Render

Particle* ParticleCluster::AddParticle()
{
	if (m_MaxParticles == 0 || !m_Particles) return NULL;
	int cP = (m_LastAdded + 1) % m_MaxParticles;
	while (!m_Particles[cP].IsFree() && cP != m_LastAdded) cP = (cP + 1) % m_MaxParticles;
	if (!m_Particles[cP].IsFree())
	{
		if (!m_bOverwrite) return NULL;
		cP = (cP + 1) % m_MaxParticles;
	}
	m_LastAdded = cP;
	Particle* pParticle = &m_Particles[cP];
	pParticle->m_Index = cP;
	return pParticle;
} // ParticleCluster::AddParticle

void ParticleCluster::UpdateParticles( float dt )
{
	if (dt < 0.0f) Log.Error( "Negative dt: %f", dt );
	for (int i = 0; i < m_MaxParticles; i++)
	{
		Particle& p = m_Particles[i]; 
		if (p.IsDead()) p.Free();
		if (p.IsFree()) continue;
		p.Update( dt );
	}
	
	if (m_bDieOnFade)
	{
		for (int i = 0; i < m_MaxParticles; i++)
		{
			Particle& p = m_Particles[i]; 
			if (p.GetAlpha() <= 0.0f) p.Kill();
		}
	}
} // ParticleCluster::UpdateParticles

void ParticleCluster::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_MaxParticles << m_bOverwrite << m_bWorldSpace;
} // ParticleCluster::Serialize

void ParticleCluster::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
	int maxParticles;
	is >> maxParticles >> m_bOverwrite >> m_bWorldSpace;
	SetMaxParticles( maxParticles );
} // ParticleCluster::Unserialize

void ParticleCluster::Init()
{
	RemoveChildren();
	AddChild<Texture>( "particle.tga" );
	
	StateBlock* pStateBlock  = AddChild<StateBlock>( "Particle" );
	RenderStateBlock* pRS	 = pStateBlock->AddChild<RenderStateBlock>( "ParticleRS" );
	TextureStateBlock* pTSS0 = pStateBlock->AddChild<TextureStateBlock>( "ParticleTSS0" );
	TextureStateBlock* pTSS1 = pStateBlock->AddChild<TextureStateBlock>( "ParticleTSS1" );
	pTSS1->Disable();
	pTSS1->SetStage( 1 );
	pTSS0->SetAlphaOp( toModulate, taTexture, taDiffuse );
	pTSS0->SetColorOp( toModulate, taTexture, taDiffuse );
	pRS->AlphaBlending( blSrcAlpha, blOne );
	pRS->EnableDithering();
	pRS->SetZMode( true, false );

} // ParticleCluster::Init

/*****************************************************************************/
/*	ParticleSystem	implementation
/*****************************************************************************/
std::stack<ParticleSystem*>		ParticleSystem::s_CurrentPS;

ParticleSystem::ParticleSystem()
{
	m_EmitRateMultiplier	= 1.0f;
	m_AlphaMultiplier		= 1.0f;
	m_BirthAlphaMultiplier	= 1.0f;
	m_TTLMultiplier			= 1.0f;
	m_UpdateRate			= 50.0f;

	m_TimeDelta				= 0.0f;
	m_bNeedUpdate			= true;

	SetAnimationTime( 5000.0f );
} // ParticleSystem::ParticleSystem

void ParticleSystem::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "ParticleSystem", this );
	pm.f( "EmitRateMultiplier",	m_EmitRateMultiplier	);
	pm.f( "AlphaMultiplier",		m_AlphaMultiplier		);
	pm.f( "BirthAlphaMultiplier",	m_BirthAlphaMultiplier	);
	pm.f( "TTLMultiplier",		m_TTLMultiplier			);
	pm.f( "UpdateRate",			m_UpdateRate			);
} // ParticleSystem::Expose

void ParticleSystem::Render()
{
	PushPS( this );
	Parent::Render();
	
	PushTime( GetCurrentTime() );

	if (m_bPaused) PushTimeDelta( 0.0f );
	Node::Render();
	if (m_bPaused) PopTimeDelta();
	
	PopTime();
	PopPS();	
} // ParticleSystem::Render

/*****************************************************************************/
/*	ParticleManager	implementation
/*****************************************************************************/
ParticleManager::ParticleManager()
{
	SetName( "ParticleManager" );
	m_bBatchClusters = false;
}

void ParticleManager::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "ParticleManager", this );
	pm.f( "BatchClusters", m_bBatchClusters );
}

void ParticleManager::Render()
{
	if (!m_bBatchClusters) return;
	for (int i = 0; i < m_Clusters.size(); i++)
	{
		m_Clusters[i].m_pCluster->m_bNeedDraw = true;
	}
	for (int i = 0; i < m_Clusters.size(); i++)
	{
		if (!m_Clusters[i].m_pCluster->m_bNeedDraw) continue;
		TransformNode::ResetTMStack( &m_Clusters[i].m_Transform );
		m_Clusters[i].m_pCluster->Flush();
		m_Clusters[i].m_pCluster->m_bNeedDraw = false;
	}
	ResetClusters();
	TransformNode::ResetTMStack();
} // ParticleManager::Render

bool ParticleManager::AddCluster( ParticleCluster* pCluster, const Matrix4D& tm )
{
	if (!m_bBatchClusters) return false;
	m_Clusters.push_back( DrawCluster( pCluster, tm ) );
	return true;
}

void ParticleManager::ResetClusters()
{
	m_Clusters.clear();
}

void ParticleManager::AddQuad( const Vector3D& pos, const Vector3D& rot, float scale, DWORD color, const Rct& uv, int texID )
{

} // ParticleManager::AddQuad

void ParticleManager::AddBillboard( const Vector3D& pos, float dir, float scale, DWORD color, const Rct& uv, int texID )
{

} // ParticleManager::AddBillboard



END_NAMESPACE(sg)
