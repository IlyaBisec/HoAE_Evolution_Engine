/*****************************************************************************/
/*	File:	sgParticleAffectors.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	08-28-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include "kIOHelpers.h"
#include "sgParticleSystem.h"
#include "sgParticleAffectors.h"
#include "uiControl.h"
#include "sgTerrain.h"

BEGIN_NAMESPACE(sg)
/*****************************************************************************/
/*	DeltaAffector implementation
/*****************************************************************************/
DeltaAffector::DeltaAffector()
{
	m_Delta.Init();
	m_Delta.m_UV = Rct::null;			
} // DeltaAffector::DeltaAffector

void DeltaAffector::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Delta;			
} // DeltaAffector::Serialize

void DeltaAffector::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Delta;
} // DeltaAffector::Unserialize

void DeltaAffector::Render()
{
	if (m_LastUpdateFrame == IRS->GetCurFrame()) return;
	m_LastUpdateFrame = IRS->GetCurFrame();

	ParticleCluster* pCluster = (ParticleCluster*)GetInput( 0 );
	if (!pCluster || !pCluster->IsA<ParticleCluster>()) return;
	
	Particle delta = m_Delta;
	delta *= Animation::CurTimeDelta() / 1000.0f;

	int nParticles = pCluster->GetMaxParticles();
	for (int i = 0; i < nParticles; i++)
	{
		Particle* p = pCluster->GetParticle( i );
		if (p->IsFree()) continue;
		*p += delta;
	}
} // DeltaAffector::Render

void DeltaAffector::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "DeltaAffector", this );
	
	pm.f( "Rotation",	m_Delta.m_Rotation		);
	pm.f( "AngVelocity",m_Delta.m_AngVelocity	);
	
	pm.f( "SizeX",		m_Delta.m_Size.x		);
	pm.f( "SizeY",		m_Delta.m_Size.y		);
	pm.f( "SizeZ",		m_Delta.m_Size.z		);

	pm.f( "Alpha",		m_Delta.m_Color.w		);
	pm.f( "Red",		m_Delta.m_Color.x		);
	pm.f( "Green",		m_Delta.m_Color.y		);
	pm.f( "Blue",		m_Delta.m_Color.z		);
} // DeltaAffector::Expose

/*****************************************************************************/
/*	ForceAffector implementation
/*****************************************************************************/
ForceAffector::ForceAffector()
{
	m_Force = Vector3D::null;
} // ForceAffector::ForceAffector

void ForceAffector::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Force;
} // ForceAffector::Serialize

void ForceAffector::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Force;
} // ForceAffector::Unserialize

void ForceAffector::Render()
{
	if (m_LastUpdateFrame == IRS->GetCurFrame()) return;
	m_LastUpdateFrame = IRS->GetCurFrame();

	ParticleCluster* pCluster = (ParticleCluster*)GetInput( 0 );
	if (!pCluster || !pCluster->IsA<ParticleCluster>()) return;
	
	Vector3D m_dForce( m_Force );
	m_dForce *= Animation::CurTimeDelta();

	int nParticles = pCluster->GetMaxParticles();
	for (int i = 0; i < nParticles; i++)
	{
		Particle* p = pCluster->GetParticle( i );
		if (p->IsFree()) continue;
		p->m_Velocity += m_dForce;
	}
} // ForceAffector::Render

void ForceAffector::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "ForceAffector", this );
	pm.f( "ForceX",	m_Force.x );
	pm.f( "ForceY",	m_Force.y );
	pm.f( "ForceZ",	m_Force.z );
} // ForceAffector::Expose

/*****************************************************************************/
/*	RandomForceAffector implementation
/*****************************************************************************/
RandomForceAffector::RandomForceAffector()
{
	m_ForceMean = Vector3D::null;
	m_ForceDev  = Vector3D::null;
} // RandomForceAffector::RandomForceAffector

void RandomForceAffector::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_ForceMean << m_ForceDev;
} // RandomForceAffector::Serialize

void RandomForceAffector::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_ForceMean >> m_ForceDev;
} // RandomForceAffector::Unserialize

void RandomForceAffector::Render()
{
	if (m_LastUpdateFrame == IRS->GetCurFrame()) return;
	m_LastUpdateFrame = IRS->GetCurFrame();

	ParticleCluster* pCluster = (ParticleCluster*)GetInput( 0 );
	if (!pCluster || !pCluster->IsA<ParticleCluster>()) return;

	float dt = Animation::CurTimeDelta();

	int nParticles = pCluster->GetMaxParticles();
	for (int i = 0; i < nParticles; i++)
	{
		Particle* p = pCluster->GetParticle( i );
		if (p->IsFree()) continue;
		p->m_Velocity.x += dt * rndValuef( m_ForceMean.x - m_ForceDev.x, m_ForceMean.x + m_ForceDev.x );
		p->m_Velocity.y += dt * rndValuef( m_ForceMean.y - m_ForceDev.y, m_ForceMean.y + m_ForceDev.y );
		p->m_Velocity.z += dt * rndValuef( m_ForceMean.z - m_ForceDev.z, m_ForceMean.z + m_ForceDev.z );
	}
} // RandomForceAffector::Render

void RandomForceAffector::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "RandomForceAffector", this );
	pm.f( "ForceX",	m_ForceMean.x );
	pm.f( "ForceY",	m_ForceMean.y );
	pm.f( "ForceZ",	m_ForceMean.z );
	
	pm.f( "ForceDX",	m_ForceDev.x );
	pm.f( "ForceDY",	m_ForceDev.y );
	pm.f( "ForceDZ",	m_ForceDev.z );

} // RandomForceAffector::Expose

/*****************************************************************************/
/*	UVAnimateAffector implementation
/*****************************************************************************/
UVInitAffector::UVInitAffector()
{
	m_NRows		= 4;		
	m_NCols		= 4;		
	m_Row		= 0;			
	m_Col		= 0;			
	m_bRandomize = false;	
}

void UVInitAffector::Render()
{

}

void UVInitAffector::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "UVInitAffector", this );
	pm.f( "NRows",		m_NRows			);
	pm.f( "NCols",		m_NCols			);
	pm.f( "Row",		m_Row			);
	pm.f( "Col",		m_Col			);
	pm.f( "Randomize",	m_bRandomize	);
} // UVInitAffector::Expose

void UVInitAffector::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
}

void UVInitAffector::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
}

/*****************************************************************************/
/*	UVAnimateAffector implementation
/*****************************************************************************/
UVAnimateAffector::UVAnimateAffector()
{
	m_NRows		= 4;
	m_NCols		= 4;
	m_Rate		= 10;
	m_bAsync	= true;
}

void UVAnimateAffector::Render()
{
	if (m_LastUpdateFrame == IRS->GetCurFrame()) return;
	m_LastUpdateFrame = IRS->GetCurFrame();

	ParticleCluster* pCluster = (ParticleCluster*)GetInput( 0 );
	if (!pCluster || !pCluster->IsA<ParticleCluster>()) return;

	float du = 1.0f / m_NCols;
	float dv = 1.0f / m_NRows;
	
	int nParticles = pCluster->GetMaxParticles();
	for (int i = 0; i < nParticles; i++)
	{
		Particle* p = pCluster->GetParticle( i );
		if (p->IsFree()) continue;
		Rct& uv = p->m_UV;

		uv.w = du;
		uv.h = dv;
		
		float age = p->m_Age;
		int anmf = int( age/m_Rate );
		if (m_bAsync) anmf += (anmf/nParticles)*i;

		anmf = anmf%(m_NCols*m_NRows);
		int row = anmf / m_NCols;
		int col = anmf % m_NCols;
		
		uv.x = float( col )*uv.w;
		uv.y = float( row )*uv.h;
	}
} //  UVAnimateAffector::Render

void UVAnimateAffector::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "UVAnimateAffector", this );
	pm.f( "NumRows",		m_NRows  );
	pm.f( "NumColumns",		m_NCols	 );
	pm.f( "FrameRate",		m_Rate	 );	
	pm.f( "Asynchronous",	m_bAsync );
}

void UVAnimateAffector::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_NRows << m_NCols << m_Rate << m_bAsync;
}

void UVAnimateAffector::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
	is >> m_NRows >> m_NCols >> m_Rate >> m_bAsync;
}

/*****************************************************************************/
/*	VortexAffector implementation
/*****************************************************************************/
VortexAffector::VortexAffector()
{
	m_Intensity			= 0.001f;
	m_CenterAttraction	= 0.0f;
} // VortexAffector::VortexAffector

void VortexAffector::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Intensity << m_CenterAttraction;
} // VortexAffector::Serializew

void VortexAffector::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Intensity >> m_CenterAttraction;
} // VortexAffector::Unserialize

void VortexAffector::Render()
{
	if (m_LastUpdateFrame == IRS->GetCurFrame()) return;
	m_LastUpdateFrame = IRS->GetCurFrame();

	ParticleCluster* pCluster = (ParticleCluster*)GetInput( 0 );
	if (!pCluster || !pCluster->IsA<ParticleCluster>()) return;

	Vector3D dir    = GetDirZ();
	Vector3D center = GetPos(); 

	float intensity  = m_Intensity * Animation::CurTimeDelta();
	float centerAttr = m_CenterAttraction * Animation::CurTimeDelta();

	int nParticles = pCluster->GetMaxParticles();
	for (int i = 0; i < nParticles; i++)
	{
		Particle* p = pCluster->GetParticle( i );
		if (p->IsFree()) continue;
		Vector3D diff( p->m_Pos );
		diff -= center;
		float pr = diff.dot( dir );
		diff = center; diff.addWeighted( dir, pr );
		Vector3D vr( p->m_Pos );
		vr -= diff;
		Vector3D accDir; accDir.cross( vr, dir );
		accDir *= intensity;
		accDir.addWeighted( vr, centerAttr );
		p->m_Velocity += accDir;
	}
} // VortexAffector::Render

void VortexAffector::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "VortexAffector", this );
	pm.f( "Intensity", m_Intensity );
	pm.f( "CenterAttraction", m_CenterAttraction );
} // VortexAffector::Expose

/*****************************************************************************/
/*	ExplodeAffector implementation
/*****************************************************************************/
ExplodeAffector::ExplodeAffector()
{
	m_Intensity			= 0.001f;
} // ExplodeAffector::ExplodeAffector

void ExplodeAffector::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Intensity;
} // ExplodeAffector::Serializew

void ExplodeAffector::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Intensity;
} // ExplodeAffector::Unserialize

void ExplodeAffector::Render()
{
	if (m_LastUpdateFrame == IRS->GetCurFrame()) return;
	m_LastUpdateFrame = IRS->GetCurFrame();

	ParticleCluster* pCluster = (ParticleCluster*)GetInput( 0 );
	if (!pCluster || !pCluster->IsA<ParticleCluster>()) return;

	Vector3D center = GetPos(); 

	int nParticles = pCluster->GetMaxParticles();
	for (int i = 0; i < nParticles; i++)
	{
		Particle* p = pCluster->GetParticle( i );
		if (p->IsFree()) continue;
		Vector3D acc( p->m_Pos );
		acc -= center;
		float mag = acc.normalize();
		if (mag < c_Epsilon) mag = c_Epsilon;
		acc *= m_Intensity / mag;
		p->m_Velocity += acc;
	}
} // ExplodeAffector::Render

void ExplodeAffector::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "ExplodeAffector", this );
	pm.f( "Intensity", m_Intensity );
} // ExplodeAffector::Expose

/*****************************************************************************/
/*	FollowAffector implementation
/*****************************************************************************/
FollowAffector::FollowAffector()
{
	m_Intensity = 1.0f;
	m_bCircular = false;
} // FollowAffector::FollowAffector

void FollowAffector::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Intensity << m_bCircular;
} // FollowAffector::Serialize

void FollowAffector::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Intensity >> m_bCircular;
} // FollowAffector::Unserialize

void FollowAffector::Render()
{
	if (m_LastUpdateFrame == IRS->GetCurFrame()) return;
	m_LastUpdateFrame = IRS->GetCurFrame();

	ParticleCluster* pCluster = (ParticleCluster*)GetInput( 0 );
	if (!pCluster || !pCluster->IsA<ParticleCluster>()) return;

	int nParticles = pCluster->GetMaxParticles();
	Particle* pPrev = NULL;
	for (int i = 0; i < nParticles; i++)
	{
		Particle* p = pCluster->GetParticle( i );
		if (p->IsFree()) continue;
		if (pPrev)
		{
			Vector3D v( p->m_Pos );
			v -= pPrev->m_Pos;
			v.normalize();
			v *= m_Intensity;
			pPrev->m_Velocity += v;
		}
		pPrev = p;
	}
} // FollowAffector::Render

void FollowAffector::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "FollowAffector", this );
	pm.f( "Intensity", m_Intensity );
	pm.f( "Circular", m_bCircular );
} // FollowAffector::Expose

/*****************************************************************************/
/*	RandomTorqueAffector implementation
/*****************************************************************************/
RandomTorqueAffector::RandomTorqueAffector()
{
	m_Intensity = 0.0001f;
} // RandomTorqueAffector::RandomTorqueAffector

void RandomTorqueAffector::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Intensity;
} // RandomTorqueAffector::Serialize

void RandomTorqueAffector::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Intensity;
} // RandomTorqueAffector::Unserialize

void RandomTorqueAffector::Render()
{
	if (m_LastUpdateFrame == IRS->GetCurFrame()) return;
	m_LastUpdateFrame = IRS->GetCurFrame();

	ParticleCluster* pCluster = (ParticleCluster*)GetInput( 0 );
	if (!pCluster || !pCluster->IsA<ParticleCluster>()) return;

	float dI = m_Intensity * Animation::CurTimeDelta() * 0.001f;
	int nParticles = pCluster->GetMaxParticles();
	for (int i = 0; i < nParticles; i++)
	{
		Particle* p = pCluster->GetParticle( i );
		if (p->IsFree()) continue;
		p->m_AngVelocity += rndValuef( -dI, dI );
	}
} // RandomTorqueAffector::Render

void RandomTorqueAffector::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "RandomTorqueAffector", this );
	pm.f( "Intensity", m_Intensity );
} // RandomTorqueAffector::Expose


/*****************************************************************************/
/*	Turbulence implementation
/*****************************************************************************/
Turbulence::Turbulence()
{
	m_Magnitude = 5.0f;
	m_Frequency = 1.0f;
	m_Phase		= Vector3D::oZ;
} // Turbulence::Turbulence

void Turbulence::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Magnitude << m_Frequency << m_Phase;
} // Turbulence::Serialize

void Turbulence::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Magnitude >> m_Frequency >> m_Phase;
} // Turbulence::Unserialize

void Turbulence::Render()
{
	if (m_LastUpdateFrame == IRS->GetCurFrame()) return;
	m_LastUpdateFrame = IRS->GetCurFrame();

	ParticleCluster* pCluster = (ParticleCluster*)GetInput( 0 );
	if (!pCluster || !pCluster->IsA<ParticleCluster>()) return;

	int nParticles = pCluster->GetMaxParticles();
	for (int i = 0; i < nParticles; i++)
	{
		Particle* p = pCluster->GetParticle( i );
		if (p->IsFree()) continue;
		


	}
} // Turbulence::Render

void Turbulence::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "Turbulence", this );
	pm.f( "Magmitude",	m_Magnitude );
	pm.f( "Frequency",	m_Frequency );
	pm.f( "PhaseX",		m_Phase.x	);
	pm.f( "PhaseY",		m_Phase.y	);
	pm.f( "PhaseZ",		m_Phase.z	);
} // Turbulence::Expose

/*****************************************************************************/
/*	FluidFrictionAffector implementation
/*****************************************************************************/
FluidFrictionAffector::FluidFrictionAffector()
{
	m_Density			= 1.0f;			
	m_Viscosity			= 1.8e-5f;		
	m_bOverrideRadius	= 1;	
	m_ParticleRadius	= 1.0f;	
	m_A					= 6.0f * c_PI * m_Viscosity;
	m_B					= 0.2f * c_PI * m_Density;
} // FluidFrictionAffector::FluidFrictionAffector

void FluidFrictionAffector::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Density << m_Viscosity << m_bOverrideRadius << m_ParticleRadius;
} // FluidFrictionAffector::Serialize

void FluidFrictionAffector::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Density >> m_Viscosity >> m_bOverrideRadius >> m_ParticleRadius;
	SetDensity( m_Density );
	SetViscosity( m_Viscosity );
} // FluidFrictionAffector::Unserialize

void FluidFrictionAffector::SetDensity( float val )
{
	m_Density = val;
	m_B = 0.2f * c_PI * m_Density;
} // FluidFrictionAffector::SetDensity

void FluidFrictionAffector::SetViscosity( float val )
{
	m_Viscosity = val;
	m_A = 6.0f * c_PI * m_Viscosity;
} // FluidFrictionAffector::SetViscosity

void FluidFrictionAffector::Render()
{
	if (m_LastUpdateFrame == IRS->GetCurFrame()) return;
	m_LastUpdateFrame = IRS->GetCurFrame();

	ParticleCluster* pCluster = (ParticleCluster*)GetInput( 0 );
	if (!pCluster || !pCluster->IsA<ParticleCluster>()) return;

	float dt = Animation::CurTimeDelta();

	int nParticles = pCluster->GetMaxParticles();
	for (int i = 0; i < nParticles; i++)
	{
		Particle* p = pCluster->GetParticle( i );
		if (p->IsFree()) continue;

		Vector3D v = p->m_Velocity;
		float nv = v.normalize();
		float radius = m_bOverrideRadius ? m_ParticleRadius : p->m_Size.norm();
		
		float R = m_A * radius * nv + m_B * radius * radius * nv * nv;
		float dv = R * dt * 0.001f;
		if (dv > nv) dv = nv;
		v *= -dv;
		p->m_Velocity += v;
	}
} // FluidFrictionAffector::Render

void FluidFrictionAffector::SetToAir()
{
	SetViscosity( 1.8e-5f	);
	SetDensity	( 1.2929f	);
} // FluidFrictionAffector::SetToAir

void FluidFrictionAffector::SetToWater()
{
	SetViscosity( 1.002e-3f	);
	SetDensity	( 1.0f		);
} // FluidFrictionAffector::SetToWater

void FluidFrictionAffector::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "FluidFrictionAffector", this );
	pm.p( "FluidDensity",		GetDensity,		SetDensity	 );
	pm.p( "FluidViscosity",	GetViscosity,	SetViscosity );
	pm.f( "OverrideRadius",	m_bOverrideRadius	);
	pm.f( "ParticleRadius",	m_ParticleRadius	);
	pm.m( "SetToAir",		SetToAir			);
	pm.m( "SetToWater",		SetToWater			);
} // FluidFrictionAffector::Expose

/*****************************************************************************/
/*	HitAffector implementation
/*****************************************************************************/
HitAffector::HitAffector()
{
	m_HitAction = haDeflect;
	m_Damping = 1.0f;
} // HitAffector::HitAffector

void HitAffector::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Damping;
} // HitAffector::Serialize

void HitAffector::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Damping;
} // HitAffector::Unserialize

void HitAffector::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "HitAffector", this );
	pm.f( "HitAction", m_HitAction );
	pm.f( "Damping", m_Damping );
} // HitAffector::Expose

/*****************************************************************************/
/*	SphereProjectAffector implementation
/*****************************************************************************/
SphereProjectAffector::SphereProjectAffector()
{
	m_SphereRadius		= 100.0f;
	m_bPreserveVelocity	= true;
} // SphereProjectAffector::SphereProjectAffector

void SphereProjectAffector::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_SphereRadius;
} // SphereProjectAffector::Serialize

void SphereProjectAffector::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_SphereRadius;
} // SphereProjectAffector::Unserialize

void SphereProjectAffector::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "SphereProjectAffector", this );
	pm.f( "SphereRadius", m_SphereRadius );
	pm.f( "PreserveVelocity", m_bPreserveVelocity );
} // SphereProjectAffector::Expose

void SphereProjectAffector::Render()
{
	if (m_LastUpdateFrame == IRS->GetCurFrame()) return;
	m_LastUpdateFrame = IRS->GetCurFrame();

	ParticleCluster* pCluster = (ParticleCluster*)GetInput( 0 );
	if (!pCluster || !pCluster->IsA<ParticleCluster>()) return;

	Sphere sp( GetPos(), m_SphereRadius );

	int nParticles = pCluster->GetMaxParticles();

	if (m_bPreserveVelocity)
	{
		for (int i = 0; i < nParticles; i++)
		{
			Particle* p = pCluster->GetParticle( i );
			if (p->IsFree()) continue;
			Vector3D nNorm, nVel;
			float velMag = p->m_Velocity.norm();
			sp.ProjectPt( p->m_Pos );
			sp.Decompose( p->m_Pos, p->m_Velocity, nNorm, nVel );
			nVel.normalize();
			nVel *= velMag;
			p->m_Velocity	= nVel;
		}		
	}
	else
	{
		for (int i = 0; i < nParticles; i++)
		{
			Particle* p = pCluster->GetParticle( i );
			if (p->IsFree()) continue;
			Vector3D nNorm, nVel;
			sp.Decompose( p->m_Pos, p->m_Velocity, nNorm, nVel );
			sp.ProjectPt( p->m_Pos );
			p->m_Velocity	= nVel;
		}		
	}
}// SphereProjectAffector::Render

/*****************************************************************************/
/*	PlaneHitAffector implementation
/*****************************************************************************/
PlaneHitAffector::PlaneHitAffector()
{
} // PlaneHitAffector::PlaneHitAffector

void PlaneHitAffector::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
} // PlaneHitAffector::Serialize

void PlaneHitAffector::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
} // PlaneHitAffector::Unserialize

void PlaneHitAffector::Render()
{
	if (m_LastUpdateFrame == IRS->GetCurFrame()) return;
	m_LastUpdateFrame = IRS->GetCurFrame();

	ParticleCluster* pCluster = (ParticleCluster*)GetInput( 0 );
	if (!pCluster || !pCluster->IsA<ParticleCluster>()) return;

	Plane pl( GetPos(), GetDirZ() );

	int nParticles = pCluster->GetMaxParticles();
	if (m_HitAction == haDie)
	{
		for (int i = 0; i < nParticles; i++)
		{
			Particle* p = pCluster->GetParticle( i );
			if (p->IsFree()) continue;
			if (!pl.OnPositiveSide( p->m_Pos ))
			{
				p->m_Age = p->m_TimeToLive;
			}
		}
	}
	else if (m_HitAction == haDeflect)
	{
		for (int i = 0; i < nParticles; i++)
		{
			Particle* p = pCluster->GetParticle( i );
			if (p->IsFree()) continue;
			if (!pl.OnPositiveSide( p->m_Pos ))
			{
				Vector3D normV, planeV;
				pl.Decompose( p->m_Velocity, normV, planeV );
				normV.reverse();
				p->m_Velocity.add( normV, planeV );
				Vector3D dampV( p->m_Velocity );
				if (dampV.normalize() > m_Damping)
				{
					dampV *= m_Damping;
					p->m_Velocity -= dampV;
				}
				else p->m_Velocity = Vector3D::null;
			}
		}
	}

}// PlaneHitAffector::Render

void PlaneHitAffector::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "PlaneHitAffector", this );
} // PlaneHitAffector::Expose


/*****************************************************************************/
/*	TerrainHitAffector implementation
/*****************************************************************************/
TerrainHitAffector::TerrainHitAffector()
{
	m_Damping = 1.0f;
} // TerrainHitAffector::TerrainHitAffector

void TerrainHitAffector::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
} // TerrainHitAffector::Serialize

void TerrainHitAffector::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
} // TerrainHitAffector::Unserialize

void TerrainHitAffector::Render()
{
	if (m_LastUpdateFrame == IRS->GetCurFrame()) return;
	m_LastUpdateFrame = IRS->GetCurFrame();

	ParticleCluster* pCluster = (ParticleCluster*)GetInput( 0 );
	if (!pCluster || !pCluster->IsA<ParticleCluster>()) return;

	int nParticles = pCluster->GetMaxParticles();
	if (m_HitAction == haDie)
	{
		for (int i = 0; i < nParticles; i++)
		{
			Particle* p = pCluster->GetParticle( i );
			if (p->IsFree()) continue;
			float terrZ = ITerra->GetH( p->m_Pos.x, p->m_Pos.y );
			if (terrZ > p->m_Pos.z)
			{
				p->m_Age = p->m_TimeToLive;
				p->m_Pos.z = terrZ;
			}
		}
	}
	else if (m_HitAction == haDeflect)
	{
		for (int i = 0; i < nParticles; i++)
		{
			Particle* p = pCluster->GetParticle( i );
			if (p->IsFree()) continue;
			float terrZ = ITerra->GetH( p->m_Pos.x, p->m_Pos.y );
			if (terrZ > p->m_Pos.z)
			{
				Vector3D normV, planeV;
				Plane pl;
				pl.fromPointNormal( p->m_Pos, ITerra->GetNormal(  p->m_Pos.x, p->m_Pos.y ) );
				pl.Decompose( p->m_Velocity, normV, planeV );
				normV.reverse();
				p->m_Velocity.add( normV, planeV );
				p->m_Pos.z = terrZ; 
				Vector3D dampV( p->m_Velocity );
				if (dampV.normalize() > m_Damping)
				{
					dampV *= m_Damping;
					p->m_Velocity -= dampV;
				}
				else p->m_Velocity = Vector3D::null;
			}
		}
	}

}// TerrainHitAffector::Render

void TerrainHitAffector::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "TerrainHitAffector", this );
} // TerrainHitAffector::Expose


/*****************************************************************************/
/*	MatchVelocityAffector implementation
/*****************************************************************************/
MatchVelocityAffector::MatchVelocityAffector()
{
	m_Intensity = 0.01f;
	m_Radius	= 100.0f;
} // MatchVelocityAffector::MatchVelocityAffector

void MatchVelocityAffector::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Intensity << m_Radius;
} // MatchVelocityAffector::Serialize

void MatchVelocityAffector::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Intensity >> m_Radius;
} // MatchVelocityAffector::Unserialize

void MatchVelocityAffector::Render()
{
	if (m_LastUpdateFrame == IRS->GetCurFrame()) return;
	m_LastUpdateFrame = IRS->GetCurFrame();

	ParticleCluster* pCluster = (ParticleCluster*)GetInput( 0 );
	if (!pCluster || !pCluster->IsA<ParticleCluster>()) return;

	int nParticles = pCluster->GetMaxParticles();

	float mag = m_Intensity * Animation::CurTimeDelta();
	float radsq = m_Radius * m_Radius;	

	for (int i = 0; i < nParticles; i++)
	{
		Particle* p = pCluster->GetParticle( i );
		if (p->IsFree()) continue;
		for (int j = i + 1; j < nParticles; j++)
		{
			Particle* q = pCluster->GetParticle( j );
			if (q->IsFree()) continue;
			float d2 = p->m_Pos.distance2( q->m_Pos );
			if (d2 < radsq)
			{
				Vector3D acc( q->m_Velocity );
				acc *= mag/(d2 + c_Epsilon);
				p->m_Velocity += acc;
				q->m_Velocity -= acc;
			}
		}
	}
} // MatchVelocityAffector::Render

void MatchVelocityAffector::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "MatchVelocityAffector", this );
	pm.f( "Intensity",	m_Intensity );
	pm.f( "Neighborhood", m_Radius   );
} // MatchVelocityAffector::Expose

/*****************************************************************************/
/*	ChainEffectAffector implementation
/*****************************************************************************/
ChainEffectAffector::ChainEffectAffector()
{
	SetMaxInstances( 16 );
	m_TriggerProbability	= 1.0f;
} // ChainEffectAffector::ChainEffectAffector

void ChainEffectAffector::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_MaxInstances;
} // ChainEffectAffector::Serialize

void ChainEffectAffector::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_MaxInstances;
} // ChainEffectAffector::Unserialize

void ChainEffectAffector::SetMaxInstances( int val )
{
	m_MaxInstances = val;
} // ChainEffectAffector::SetMaxInstances

bool ChainEffectAffector::AddAnimationInstance( Animation* pAnim, const Matrix4D& tm )
{
	if (m_Instances.size() == m_MaxInstances) return false;
	AnimationInstance inst;
	inst.m_CurTime		= 0.0f;
	inst.m_Transform	= tm;
	inst.m_pAnimation	= pAnim;
	inst.m_EmitFactor	= 0.0f;

	m_Instances.push_back( inst );
	return true;
} // ChainEffectAffector::AddAnimationInstance

void ChainEffectAffector::Render()
{
	if (m_LastUpdateFrame == IRS->GetCurFrame()) return;
	m_LastUpdateFrame = IRS->GetCurFrame();

	ParticleCluster* pCluster	= (ParticleCluster*)GetInput( 0 );
	Animation*		 pAnimation = (Animation*)GetChild( 1 );
	
	if (!pCluster   || !pCluster->IsA<ParticleCluster>()) return;
	if (!pAnimation || !pAnimation->IsA<Animation>()	) return;

	Iterator it( pAnimation, ParticleEmitter::FnFilter );
	ParticleEmitter* pEmitter = it ? (ParticleEmitter*)(Node*)it : NULL;

	int nParticles = pCluster->GetMaxParticles();

	for (int i = 0; i < nParticles; i++)
	{
		Particle* p = pCluster->GetParticle( i );
		if (p->IsFree()) continue;
		if (p->IsDead())
		{
			AddAnimationInstance( pAnimation, p->GetTransform() );
		}
	}

	for (int i = 0; i < m_Instances.size(); i++)
	{
		AnimationInstance& inst = m_Instances[i];
		PushTM( inst.m_Transform );
		inst.m_CurTime += Animation::CurTimeDelta();
		Animation::PushTime( inst.m_CurTime );
		if (pEmitter) pEmitter->SetEmitFactor( inst.m_EmitFactor );
		inst.m_pAnimation->Render();
		if (pEmitter) inst.m_EmitFactor = pEmitter->GetEmitFactor();
		if (inst.m_CurTime >= inst.m_pAnimation->GetAnimationTime())
		{
			m_Instances.erase( m_Instances.begin() + i );
		}
		PopTM();
		Animation::PopTime();
	}

} // ChainEffectAffector::Render

void ChainEffectAffector::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "ChainEffectAffector", this );
	pm.p( "MaxInstances",	GetMaxInstances, SetMaxInstances );
	pm.f( "TriggerProbability", m_TriggerProbability );
} // ChainEffectAffector::Expose

/*****************************************************************************/
/*	LightningAffector implementation
/*****************************************************************************/
LightningAffector::LightningAffector()
{
	m_pStartTM = m_pEndTM = NULL;
	m_Spread			= 100.0f;
	m_bAffectEndpoints	= false;
	m_Frequency			= 200.0f;

	m_Phase				= 0.0f;
} // LightningAffector::LightningAffector

void LightningAffector::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Spread << m_bAffectEndpoints << m_Frequency;
} // LightningAffector::Serialize

void LightningAffector::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Spread >> m_bAffectEndpoints >> m_Frequency;
} // LightningAffector::Unserialize

void LightningAffector::Render()
{
	if (m_LastUpdateFrame == IRS->GetCurFrame()) return;
	m_LastUpdateFrame = IRS->GetCurFrame();

	m_Phase += Animation::CurTimeDelta();
	if (m_Phase <= m_Frequency) return;
	m_Phase = fmod( m_Phase, m_Frequency );
	
	// --- temporary
	ParticleCluster* pCluster = (ParticleCluster*)GetInput( 0 );
	if (!pCluster || !pCluster->IsA<ParticleCluster>()) return;

	m_pStartTM = (TransformNode*)GetInput( 1 );
	m_pEndTM   = (TransformNode*)GetInput( 2 );
	if (!m_pStartTM || !m_pEndTM) return;

	if (m_pStartTM && !m_pStartTM->IsA<TransformNode>()) { m_pStartTM	= NULL; return; }
	if (m_pEndTM && !m_pEndTM->IsA<TransformNode>())	 { m_pEndTM		= NULL; return; }
	// ---

	const Vector3D& beg = m_pStartTM->GetPos();
	const Vector3D& end = m_pEndTM->GetPos();

	float segLen = beg.distance( end );	
	int nParticles = pCluster->GetMaxParticles();
	if (nParticles == 0) return;

	Matrix4D worldTM = TMStackTop();

	Vector3D dir( end );
	dir -= beg;
	Vector3D dx, dy;
	dir.CreateBasis( dx, dy );

	for (int i = 0; i < nParticles; i++)
	{
		Particle* p = pCluster->GetParticle( i );
		if (p->IsFree()) continue;
		
		Vector3D pos( p->m_Pos );

		if (!m_bAffectEndpoints)
		{
			if (pos.distance2( beg ) < c_Epsilon) continue;
			if (pos.distance2( end ) < c_Epsilon) continue;
		}

		pos -= beg;
		float dist = pos.dot( dir );
		pos = dir;
		pos *= dist;
		pos.addWeighted( dx, rndValuef( -m_Spread, m_Spread ) );
		pos.addWeighted( dy, rndValuef( -m_Spread, m_Spread ) );
		pos += beg;
		p->m_Pos = pos;
	}

} // LightningAffector::Render

void LightningAffector::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "LightningAffector", this );
	pm.f( "Spread",			m_Spread );
	pm.f( "Frequency",		m_Frequency );
	pm.f( "AffectEndpoints",	m_bAffectEndpoints );
} // LightningAffector::Expose

/*****************************************************************************/
/*	ColorAnimateAffector implementation
/*****************************************************************************/
ColorAnimateAffector::ColorAnimateAffector()
{
	m_bRandomInitColor = false;
	m_Curve.AddKey( 0.0f, 0xFFFFFFFF );
	m_Curve.AddKey( 1.0f, 0xFFFFFFFF );
}

void ColorAnimateAffector::Render()
{
	if (m_LastUpdateFrame == IRS->GetCurFrame()) return;
	m_LastUpdateFrame = IRS->GetCurFrame();

	ParticleCluster* pCluster = (ParticleCluster*)GetInput( 0 );
	if (!pCluster || !pCluster->IsA<ParticleCluster>()) return;

	int nParticles = pCluster->GetMaxParticles();

	if (m_bRandomInitColor)
	{
		for (int i = 0; i < nParticles; i++)
		{
			Particle* p = pCluster->GetParticle( i );
			if (p->IsFree()) continue;
			if (p->m_Age != 0.0f) continue;
			ColorValue cval = m_Curve.GetValue( rndValuef() );
			p->m_Color = Vector4D( cval.r, cval.g, cval.b, cval.a );
		}
		return;
	}

	for (int i = 0; i < nParticles; i++)
	{
		Particle* p = pCluster->GetParticle( i );
		if (p->IsFree()) continue;
		float ratio = p->m_Age / p->m_TimeToLive;
		ColorValue cval = m_Curve.GetValue( ratio );
		p->m_Color = Vector4D( cval.r, cval.g, cval.b, cval.a );
	}
} // ColorAnimateAffector::Render

void ColorAnimateAffector::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "ColorAnimateAffector", this );
	pm.f( "RandomInitColor", m_bRandomInitColor );
	pm.p( "Color", GetColorCurve, SetColorCurve, "colorAnimCurve" );
} // ColorAnimateAffector::Expose

void ColorAnimateAffector::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	m_Curve.Serialize( os );
	os << m_bRandomInitColor;
} // ColorAnimateAffector::Serialize

void ColorAnimateAffector::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	m_Curve.Unserialize( is );
	is >> m_bRandomInitColor;
} // ColorAnimateAffector::Unserialize

/*****************************************************************************/
/*	SizeAnimateAffector implementation
/*****************************************************************************/
SizeAnimateAffector::SizeAnimateAffector()
{
	m_bRandomInitSize = false;	
	m_bAffectX = m_bAffectY = m_bAffectZ = true;
	m_Curve.AddKey( 0.0f, 0.0f	 );
	m_Curve.AddKey( 1.0f, 100.0f );
}

void SizeAnimateAffector::Render()
{
	if (m_LastUpdateFrame == IRS->GetCurFrame()) return;
	m_LastUpdateFrame = IRS->GetCurFrame();

	ParticleCluster* pCluster = (ParticleCluster*)GetInput( 0 );
	if (!pCluster || !pCluster->IsA<ParticleCluster>()) return;

	int nParticles = pCluster->GetMaxParticles();

	if (m_bRandomInitSize)
	{
		for (int i = 0; i < nParticles; i++)
		{
			Particle* p = pCluster->GetParticle( i );
			if (p->IsFree()) continue;
			if (p->m_Age != 0.0f) continue;
			float cval = m_Curve.GetValue( rndValuef() );
			if (m_bAffectX) p->m_Size.x = cval;
			if (m_bAffectY) p->m_Size.y = cval;
			if (m_bAffectZ) p->m_Size.z = cval;
		}
		return;
	}

	for (int i = 0; i < nParticles; i++)
	{
		Particle* p = pCluster->GetParticle( i );
		if (p->IsFree()) continue;
		float ratio = p->m_Age / p->m_TimeToLive;
		float cval = m_Curve.GetValue( ratio );
		if (m_bAffectX) p->m_Size.x = cval;
		if (m_bAffectY) p->m_Size.y = cval;
		if (m_bAffectZ) p->m_Size.z = cval;
	}
} // SizeAnimateAffector::Render

void SizeAnimateAffector::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "SizeAnimateAffector", this );
	pm.f( "AffectX", m_bAffectX );
	pm.f( "AffectY", m_bAffectY );
	pm.f( "AffectZ", m_bAffectZ );
	pm.f( "RandomInitSize", m_bRandomInitSize );
	pm.p( "Size", GetSizeCurve, SetSizeCurve, "floatAnimCurve" );
} // SizeAnimateAffector::Expose

void SizeAnimateAffector::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	m_Curve.Serialize( os ); 
	os << m_bAffectX << m_bAffectY << m_bAffectZ << m_bRandomInitSize;
}

void SizeAnimateAffector::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	m_Curve.Unserialize( is );
	is >> m_bAffectX >> m_bAffectY >> m_bAffectZ >> m_bRandomInitSize;
}

END_NAMESPACE(sg)
