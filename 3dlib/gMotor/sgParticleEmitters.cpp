/*****************************************************************************/
/*	File:	sgParticleEmitters.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	08-28-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include "kIOHelpers.h"
#include "sgParticleSystem.h"
#include "sgParticleEmitters.h"
#include "uiControl.h"
#include "sgTerrain.h"

BEGIN_NAMESPACE(sg)
/*****************************************************************************/
/*	ConeEmitter implementation
/*****************************************************************************/
ConeEmitter::ConeEmitter()
{
	m_Angle		= c_PId6;
	m_Height	= 50.0f;
	m_bPlanar	= false;
}

void ConeEmitter::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "ConeEmitter", this );
	pm.f( "ConeAngle", m_Angle );
	pm.f( "Planar", m_bPlanar );
}

void ConeEmitter::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Angle << m_bPlanar << m_Height;
}	

void ConeEmitter::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
	is >> m_Angle >> m_bPlanar >> m_Height;
}

void ConeEmitter::OnEmit( Particle& p )
{
	if (m_bPlanar)
	{
		float ang = rndValuef( -m_Angle, m_Angle );
		p.m_Velocity.set( sinf( ang ), 0.0f, cosf( ang ) );
	}
	else p.m_Velocity = GetCone().RandomDir();
} // ConeEmitter::Emit

Cone ConeEmitter::GetCone() const 
{ 
	Vector3D dir( GetDirZ() );
	dir *= m_Height;
	return Cone( GetPos(), dir, m_Angle ); 
} // ConeEmitter::GetCone

void ConeEmitter::Render()
{
	ParticleEmitter::Render();
	if (DoDrawGizmo())
	{
		if (m_bPlanar) 
		{
			Cone cone = GetCone();
			Vector3D a( cone.GetBaseRadius(), 0.0f, cone.GetHeight() ), b( a );
			b.x = -b.x;
			rsLine( cone.GetTop(), a, 0xFF4444FF, 0xFF4444FF );
			rsLine( cone.GetTop(), b, 0xFF4444FF, 0xFF4444FF );
			rsLine( a, b, 0xFF4444FF, 0xFF4444FF );
		}
		else
		{
			DrawCone( GetCone(), 0x00000000, 0xFF4444FF, 16 );
		}
	}
} // ConeEmitter::Render

/*****************************************************************************/
/*	LineEmitter implementation
/*****************************************************************************/
LineEmitter::LineEmitter()
{
	m_Length = 100.0f;
}

void LineEmitter::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "LineEmitter", this );
	pm.f( "LineLength", m_Length );
} // LineEmitter::Expose

void LineEmitter::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Length;
} // LineEmitter::Serialize

void LineEmitter::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Length;
} // LineEmitter::Unserialize

void LineEmitter::OnEmit( Particle& p )
{
	p.m_Velocity = Vector3D::GetRandomDir();
	p.m_Pos.set( 0.0f, 0.0f, rndValuef( 0.0f, m_Length ) );
} // LineEmitter::Emit

void LineEmitter::Render()
{
	ParticleEmitter::Render();
	if (DoDrawGizmo())
	{
		rsLine( GetBeg(), GetEnd(), 0xFF33FFFF, 0xFF33FFFF );
	}
} // LineEmitter::Render

Vector3D LineEmitter::GetEnd() const
{
	Vector3D dest( GetDirZ() );
	dest *= m_Length;
	dest += GetPos();
	return dest;
} // LineEmitter::GetEnd

/*****************************************************************************/
/*	TerrainEmitter implementation
/*****************************************************************************/
TerrainEmitter::TerrainEmitter()
{
	m_Radius = 1000.0f;
}

void TerrainEmitter::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "TerrainEmitter", this );
	pm.f( "Radius", m_Radius );
} // TerrainEmitter::Expose

void TerrainEmitter::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Radius;
} // TerrainEmitter::Serialize

void TerrainEmitter::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Radius;
} // TerrainEmitter::Unserialize

void TerrainEmitter::OnEmit( Particle& p )
{
	float tx = rndValuef( -m_Radius, m_Radius );
	float ty = rndValuef( -m_Radius, m_Radius );
	p.m_Pos.set( tx, ty, ITerra->GetH( tx, ty ) );
} // TerrainEmitter::Emit

void TerrainEmitter::OnBeginEmit()
{
}


void TerrainEmitter::Render()
{
	ParticleEmitter::Render();
} // TerrainEmitter::Render

/*****************************************************************************/
/*	PointEmitter implementation
/*****************************************************************************/
void PointEmitter::OnEmit( Particle& p )
{
	p.m_Velocity = Vector3D::GetRandomDir();
	if (m_bAffectRotation)
	{
		Vector3D vec( p.m_Velocity );
		m_ScreenPlane.ProjectVec( vec );
		p.m_Rotation = vec.Angle( m_PlaneUpVec, m_PlaneNVec );
	}
} // PointEmitter::Emit

void PointEmitter::OnBeginEmit()
{
	if (m_bAffectRotation)
	{
		BaseCamera* pCam = BaseCamera::GetActiveCamera();
		if (pCam)
		{
			m_ScreenPlane.fromPointNormal( pCam->GetPos(), pCam->GetDir() );
			m_PlaneUpVec = Vector3D::oZ;
			m_ScreenPlane.ProjectVec( m_PlaneUpVec );
			if (m_PlaneUpVec.normalize() < c_Epsilon)
			{
				m_PlaneUpVec = Vector3D::oX;
				m_ScreenPlane.ProjectVec( m_PlaneUpVec );
				if (m_PlaneUpVec.normalize() < c_Epsilon)
				{
					m_PlaneUpVec = Vector3D::oY;
					m_ScreenPlane.ProjectVec( m_PlaneUpVec );
					m_PlaneUpVec.normalize();
				}
			}
			m_PlaneNVec = pCam->GetDir();
		}
		else
		{
			m_ScreenPlane = Plane::xOy;
			m_PlaneUpVec  = Vector3D::oX;
			m_PlaneNVec	  = Vector3D::oZ;
		}
	}
} // PointEmitter::OnBeginEmit

void PointEmitter::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "PointEmitter", this );
	pm.f( "AffectRotation", m_bAffectRotation );
}

void PointEmitter::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	//os << m_bAffectRotation;
}

void PointEmitter::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
	//is >> m_bAffectRotation;
}

void PointEmitter::Render()
{
	ParticleEmitter::Render();
	if (DoDrawGizmo())
	{
		Sphere sphere( GetPos(), m_Velocity );
		DrawStar( sphere, 0xFF3333FF, 0xFF3333FF, 10 );
	}
} // PointEmitter::Render

/*****************************************************************************/
/*	SphereEmitter implementation
/*****************************************************************************/
SphereEmitter::SphereEmitter()
{
	m_Radius	= 100.0f;
	m_bSurface	= false;
	m_bPlanar	= false;
}

void SphereEmitter::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "SphereEmitter", this );
	pm.f( "Radius",	 m_Radius	);
	pm.f( "OnSurface", m_bSurface );
	pm.f( "Planar",	 m_bPlanar  );
}

void SphereEmitter::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Radius << m_bSurface << m_bPlanar;
}

void SphereEmitter::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Radius >> m_bSurface >> m_bPlanar;
}

void SphereEmitter::OnEmit( Particle& p )
{
	Vector3D pt;
	if (m_bPlanar)
	{
		float ang = rndValuef( 0.0f, c_DoublePI );
		p.m_Pos.set( m_Radius * cosf( ang ), m_Radius * sinf( ang ), 0.0f );
	}
	else p.m_Pos = GetSphere().RandomPoint();
} // SphereEmitter::Emit

Sphere SphereEmitter::GetSphere()
{
	return Sphere( GetPos(), m_Radius );
} // SphereEmitter::GetSphere

void SphereEmitter::Render()
{
	ParticleEmitter::Render();
	if (DoDrawGizmo())
	{
		if (m_bPlanar)
		{
			DrawCircle( GetPos(), GetDirZ(), m_Radius, 0, 0xFF3333FF, 16 );
		}
		else
		{
			DrawSphere( GetSphere(), 0, 0xFF3333FF, 16 );
		}
	}
} // SphereEmitter::Render

/*****************************************************************************/
/*	BoxEmitter implementation
/*****************************************************************************/
BoxEmitter::BoxEmitter()
{
	m_Dimensions.set( 50.0f, 50.0f, 50.0f );
	m_bSurface = false;
	m_bPlanar = false;
}

void BoxEmitter::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "BoxEmitter", this );
	pm.f( "BoxSideX",	m_Dimensions.x );
	pm.f( "BoxSideY",	m_Dimensions.y );
	pm.f( "BoxSideZ",	m_Dimensions.z );
	pm.f( "Surface",	m_bSurface	   );
	pm.f( "Planar",	m_bPlanar	   );
}

void BoxEmitter::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Dimensions << m_bSurface << m_bPlanar;
}

void BoxEmitter::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Dimensions >> m_bSurface >> m_bPlanar;
}

void BoxEmitter::OnEmit( Particle& p )
{
	p.m_Pos.x = rndValuef( 0.0f, m_Dimensions.x ) - m_Dimensions.y * 0.5f;
	p.m_Pos.y = rndValuef( 0.0f, m_Dimensions.x ) - m_Dimensions.y * 0.5f;
	if (m_bPlanar) p.m_Pos.z = 0.0f; else p.m_Pos.z = rndValuef( 0.0f, m_Dimensions.z ) - m_Dimensions.z * 0.5f;
} // BoxEmitter::Emit

AABoundBox BoxEmitter::GetBox()
{
	return AABoundBox( GetPos(), m_Dimensions.x * 0.5f, 
								 m_Dimensions.y * 0.5f, 
								 m_Dimensions.z * 0.5f ); 
} // BoxEmitter::GetBox

void BoxEmitter::Render()
{
	ParticleEmitter::Render();
	if (DoDrawGizmo())
	{
		AABoundBox aabb = GetBox();
		if (m_bPlanar)
		{
			aabb.minv.z = aabb.maxv.z = 0.0f;
		}
		DrawAABB( GetBox(), 0, 0xFF3333FF );
	}
} // BoxEmitter::Render

/*****************************************************************************/
/*	CylinderEmitter implementation
/*****************************************************************************/
CylinderEmitter::CylinderEmitter()
{
	m_Radius	= 100.0f;
	m_bSurface	= false;
}

void CylinderEmitter::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "CylinderEmitter", this );
	pm.f( "Radius",	 m_Radius	);
	pm.f( "Height",	 m_Height	);
	pm.f( "OnSurface", m_bSurface );
}

void CylinderEmitter::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Radius << m_Height << m_bSurface << m_bCapped;
}

void CylinderEmitter::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_Radius >> m_Height >> m_bSurface >> m_bCapped;
}

void CylinderEmitter::OnEmit( Particle& p )
{
	Vector3D pt;
	float ang = rndValuef( 0.0f, c_DoublePI );
	p.m_Pos.set( m_Radius * cosf( ang ), m_Radius * sinf( ang ), rndValuef( 0.0f, m_Height ) );
} // CylinderEmitter::Emit

Cylinder CylinderEmitter::GetCylinder()
{
	assert( false );
	return Cylinder();
} // CylinderEmitter::GetCylinder

void CylinderEmitter::Render()
{
	ParticleEmitter::Render();
	if (DoDrawGizmo())
	{
		DrawCylinder( GetCylinder(), 0, 0xFF3333FF, m_bCapped, 16 );
	}
} // CylinderEmitter::Render

/*****************************************************************************/
/*	MeshEmitter implementation
/*****************************************************************************/
MeshEmitter::MeshEmitter()
{
	m_EmitMode	= emUnknown;
	m_pMesh		= NULL;
}

void MeshEmitter::OnEmit( Particle& p )
{
	if (!m_pMesh) return;
	const BaseMesh& bm = m_pMesh->GetPrimitive();
	switch (m_EmitMode)
	{
	case emVertices:
		{
			int nV = bm.getNVert();
			if (nV <= 0) return;
			int eV = rand() % nV;
			p.m_Pos = *((Vector3D*)(((BYTE*)bm.getVertexData()) + bm.getVertexStride() * eV));
		}break;
	}
} // MeshEmitter::OnEmit

void MeshEmitter::OnBeginEmit()
{
	m_pMesh = (Geometry*)GetInput( 1 );
	if (m_pMesh && !m_pMesh->IsA<Geometry>()) m_pMesh = NULL;
}

void MeshEmitter::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	BYTE nEmitMode = (BYTE)m_EmitMode; 
	os << nEmitMode;
}

void MeshEmitter::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
	BYTE nEmitMode = 0;
	is >> nEmitMode;
	m_EmitMode = (EmitMode)nEmitMode; 
}

void MeshEmitter::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "MeshEmitter", this );
	pm.f( "EmitMode", m_EmitMode );
} // MeshEmitter::Expose

void MeshEmitter::Render()
{
	ParticleEmitter::Render();
	if (DoDrawGizmo())
	{
		DeviceStateSet::Freeze();
		static wDSS = IRS->GetShaderID( "wire" );
		IRS->SetShader( wDSS );
		Node::Render();
		DeviceStateSet::Unfreeze();
	}
} // MeshEmitter::Render

/*****************************************************************************/
/*	TargetEmitter implementation
/*****************************************************************************/
TargetEmitter::TargetEmitter()
{
	m_pTarget = NULL;
	m_bEndPoints = true;
}

void TargetEmitter::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "TargetEmitter", this );
	pm.f( "EndPoints", m_bEndPoints );
} // TargetEmitter::Expose

void TargetEmitter::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_bEndPoints;
} // TargetEmitter::Serialize

void TargetEmitter::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_bEndPoints;
} // TargetEmitter::Unserialize

void TargetEmitter::Render()
{
	ParticleEmitter::Render();
} // TargetEmitter::Render

void TargetEmitter::Emit()
{
	ParticleCluster* pCluster = (ParticleCluster*)GetInput( 0 );
	if (!pCluster || !pCluster->IsA<ParticleCluster>()) return;

	m_pTarget = (TransformNode*)GetInput( 1 );
	if (!m_pTarget) return;

	if (m_pTarget && !m_pTarget->IsA<TransformNode>()) { m_pTarget	= NULL; return; }
	const Vector3D& beg = GetWorldTM().getTranslation();
	const Vector3D& end = m_pTarget->GetWorldTM().getTranslation();

	float segLen = beg.distance( end );	
	int nParticles = NumToEmit();
	if (nParticles == 0) return;

	Matrix4D worldTM = TMStackTop();

	float step  = 0.0f;
	float delta = 0.0f; 

	if (m_bEndPoints)
	{
		step  = 1.0f / float( nParticles - 1 );
		delta = 0.0f; 
	}
	else
	{
		step  = 1.0f / float( nParticles + 1 );
		delta = step; 
	}

	Particle* prev = NULL;
	for (int i = 0; i < nParticles; i++)
	{
		Particle* p = pCluster->AddParticle();
		if (!p) return;
		
		p->Init( m_Initial, m_Deviation );
		OnEmit( *p );
		float vel = rndValuef( m_Velocity - m_VelocityD, m_Velocity + m_VelocityD );
		p->m_Velocity = Vector3D::GetRandomDir();
		p->m_Velocity	*= vel;

		p->m_TimeToLive *= ParticleSystem::GetCurPS()->GetTTLMultiplier();
		p->m_Color.w	*= ParticleSystem::GetCurPS()->GetAlphaMultiplier();
		
		if (prev) 
		{
			p->m_PrevParticle = prev->m_Index;
			p->m_NextParticle = p->m_Index;
		}
		prev = p;

		Vector3D pos = end;
		pos -= beg;
		pos *= delta;
		pos += beg;
		p->m_Pos = pos;
		delta += step;

		if (pCluster->IsWorldSpace())
		{
			worldTM.transformPt ( p->m_Pos		);
			worldTM.transformVec( p->m_Velocity );
		}
	}
} // TargetEmitter::Emit

END_NAMESPACE(sg)
