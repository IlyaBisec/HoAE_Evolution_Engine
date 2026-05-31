/*****************************************************************************/
/*	File:	sgParticleClusters.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	08-28-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include "kIOHelpers.h"
#include "sgParticleSystem.h"
#include "sgParticleClusters.h"

BEGIN_NAMESPACE(sg)
/*****************************************************************************/
/*	QuadCluster implementation
/*****************************************************************************/
QuadCluster::QuadCluster()
{
}

void QuadCluster::Render()
{

}

void QuadCluster::Flush()
{

}

/*****************************************************************************/
/*	BillboardCluster implementation
/*****************************************************************************/
BillboardCluster::BillboardCluster()
{
	SetMaxQuads( m_MaxParticles );
	SetBillboardTM( Matrix3D::identity );
}

void BillboardCluster::ResetQuadBuffer()
{
	m_NQuads = 0;
	m_QuadBuffer.setNVert( 0 );
}

void BillboardCluster::SetBillboardTM( const Matrix3D& m )
{
	m_BillboardTM = m;

	m_LT.x = -0.5f*(m.e00 + m.e10);
	m_LT.y = -0.5f*(m.e01 + m.e11);
	m_LT.z = -0.5f*(m.e02 + m.e12);

	m_RT.x = 0.5f*(m.e00 - m.e10);
	m_RT.y = 0.5f*(m.e01 - m.e11);
	m_RT.z = 0.5f*(m.e02 - m.e12);

	m_LB.x = -m_RT.x;
	m_LB.y = -m_RT.y;
	m_LB.z = -m_RT.z;

	m_RB.x = -m_LT.x;
	m_RB.y = -m_LT.y;
	m_RB.z = -m_LT.z;
} // BillboardCluster::SetBillboardTM

bool BillboardCluster::AddQuad( const Vector3D& pos, float sizeX, float sizeY,
							 float rot, DWORD color, const Rct& uv )
{
	if (m_NQuads == m_MaxQuads) return false;
	Vertex2t* v = ((Vertex2t*)m_QuadBuffer.getVertexData() + m_NQuads * 4);

	float cr = cosf( rot );
	float sr = sinf( rot );
	float xc = 0.5f * sizeX * cr;
	float xs = 0.5f * sizeX * sr;
	float yc = 0.5f * sizeY * cr;
	float ys = 0.5f * sizeY * sr;

	Vector3D lt( -xc + ys, -xs - yc, 0.0f );
	Vector3D rt(  xc + ys,  xs - yc, 0.0f );
	Vector3D lb( -xc - ys, -xs + yc, 0.0f );
	Vector3D rb(  xc - ys,  xs + yc, 0.0f );

	lt *= m_BillboardTM;
	rt *= m_BillboardTM;
	lb *= m_BillboardTM;
	rb *= m_BillboardTM;

	v[0].x = lt.x + pos.x;
	v[0].y = lt.y + pos.y;
	v[0].z = lt.z + pos.z;

	v[1].x = rt.x + pos.x;
	v[1].y = rt.y + pos.y;
	v[1].z = rt.z + pos.z;

	v[2].x = lb.x + pos.x;
	v[2].y = lb.y + pos.y;
	v[2].z = lb.z + pos.z;

	v[3].x = rb.x + pos.x;
	v[3].y = rb.y + pos.y;
	v[3].z = rb.z + pos.z;

	//  color and texture coordinates
	v[0].diffuse = color;
	v[0].u		 = uv.x;
	v[0].v		 = uv.y + uv.h;

	v[1].diffuse = color;
	v[1].u		 = uv.x + uv.w;
	v[1].v		 = uv.y + uv.h;

	v[2].diffuse = color;
	v[2].u		 = uv.x;
	v[2].v		 = uv.y ;

	v[3].diffuse = color;
	v[3].u		 = uv.x + uv.w;
	v[3].v		 = uv.y;

	m_NQuads++;
	return true;
} // BillboardCluster::AddQuad

bool BillboardCluster::AddQuad( const Vector3D& pos, float size, DWORD color, const Rct& uv )
{
	if (m_NQuads == m_MaxQuads) return false;
	Vertex2t* v = ((Vertex2t*)m_QuadBuffer.getVertexData() + m_NQuads * 4);

	v[0].x = pos.x + m_LT.x * size;
	v[0].y = pos.y + m_LT.y * size;
	v[0].z = pos.z + m_LT.z * size;

	v[1].x = pos.x + m_RT.x * size;
	v[1].y = pos.y + m_RT.y * size;
	v[1].z = pos.z + m_RT.z * size;

	v[2].x = pos.x + m_LB.x * size;
	v[2].y = pos.y + m_LB.y * size;
	v[2].z = pos.z + m_LB.z * size;

	v[3].x = pos.x + m_RB.x * size;
	v[3].y = pos.y + m_RB.y * size;
	v[3].z = pos.z + m_RB.z * size;

	//  color and texture coordinates
	v[0].diffuse = color;
	v[0].u		 = uv.x;
	v[0].v		 = uv.y;

	v[1].diffuse = color;
	v[1].u		 = uv.x + uv.w;
	v[1].v		 = uv.y;

	v[2].diffuse = color;
	v[2].u		 = uv.x;
	v[2].v		 = uv.y + uv.h;

	v[3].diffuse = color;
	v[3].u		 = uv.x + uv.w;
	v[3].v		 = uv.y + uv.h;

	m_NQuads++;
	return true;
} // BillboardCluster::AddQuad

bool BillboardCluster::AddQuad( const Vector3D& pos1, float size1, DWORD color1, 
								const Vector3D& pos2, float size2, DWORD color2,
								const Rct& uv )
{
	if (m_NQuads == m_MaxQuads) return false;
	Vertex2t* v = ((Vertex2t*)m_QuadBuffer.getVertexData() + m_NQuads * 4);

	v[0].x = pos1.x;
	v[0].y = pos1.y;
	v[0].z = pos1.z;

	v[1].x = pos2.x;
	v[1].y = pos2.y;
	v[1].z = pos2.z;

	v[2].x = pos1.x + size1;
	v[2].y = pos1.y;
	v[2].z = pos1.z;

	v[3].x = pos2.x + size2;
	v[3].y = pos2.y;
	v[3].z = pos2.z;

	//  color and texture coordinates
	v[0].diffuse = color1;
	v[0].u		 = uv.x;
	v[0].v		 = uv.y;

	v[1].diffuse = color2;
	v[1].u		 = uv.x + uv.w;
	v[1].v		 = uv.y;

	v[2].diffuse = color1;
	v[2].u		 = uv.x;
	v[2].v		 = uv.y + uv.h;

	v[3].diffuse = color2;
	v[3].u		 = uv.x + uv.w;
	v[3].v		 = uv.y + uv.h;

	m_NQuads++;
	return true;
} // BillboardCluster::AddQuad

void BillboardCluster::FixStripJoints()
{
	
} // BillboardCluster::FixStripJoints

void BillboardCluster::SetMaxQuads( int nQuads )
{
	m_MaxQuads = nQuads;
	m_NQuads = 0;
	m_QuadBuffer.create( m_MaxQuads * 4, 0, vfVertex2t, ptTriangleList );
	m_QuadBuffer.setIsQuadList( true );
} //  BillboardCluster::SetMaxQuads

void BillboardCluster::FillQuads()
{
	if (GetMaxParticles() != m_MaxQuads) SetMaxQuads( GetMaxParticles() );
	ResetQuadBuffer();
	for (int i = 0; i < m_MaxParticles; i++)
	{
		const Particle& p = m_Particles[i]; 
		if (p.IsFree()) continue;
		AddQuad( p.m_Pos, p.m_Size.x, p.m_Size.y, p.m_Rotation, 
			ColorValue::ToARGB( p.m_Color.w * ParticleSystem::GetCurPS()->GetAlphaMultiplier(), 
			p.m_Color.x, p.m_Color.y, p.m_Color.z ), p.m_UV );		
	}
} // BillboardCluster::FillQuads

void BillboardCluster::RenderQuads()
{
	IRS->ResetWorldTM();
	m_QuadBuffer.setNVert	( m_NQuads * 4 );
	m_QuadBuffer.setNPri	( m_NQuads * 2 );
	DrawPrimBM( m_QuadBuffer );
} // BillboardCluster::RenderQuads

void BillboardCluster::Render()
{
	if (m_LastUpdateFrame == IRS->GetCurFrame()) return;
	BaseCamera* pCam = BaseCamera::GetActiveCamera();
	if (!pCam) return;
	Matrix4D clusterTM( pCam->GetWorldTM() );
	SetBillboardTM( clusterTM );
	FillQuads();
	ParticleCluster::Render();
} // BillboardCluster::Render

void BillboardCluster::Flush()
{
	Node::Render();
	RenderQuads();
} // PlaneCluster::Render

/*****************************************************************************/
/*	PlaneCluster implementation
/*****************************************************************************/
PlaneCluster::PlaneCluster()
{
	SetBillboardTM( Matrix3D::identity );
}

void PlaneCluster::Flush()
{
	Node::Render();
	RenderQuads();
} // PlaneCluster::Render

void PlaneCluster::Render()
{ 
	if (m_LastUpdateFrame == IRS->GetCurFrame()) return;
	FillQuads();
	ParticleCluster::Render(); 
}

/*****************************************************************************/
/*	LineCluster	implementation
/*****************************************************************************/
void LineCluster::Render()
{
	UpdateParticles( Animation::CurTimeDelta() );

	PushTM( tm );
	
	for (int i = 0; i < m_MaxParticles; i++)
	{
		const Particle& p = m_Particles[i]; 
		if (p.IsFree()) continue;
		rsLine( p.m_PrevPos, p.m_Pos, p.GetColor(), p.GetColor() );	
	}

	PopTM();
} // LineCluster::Render

/*****************************************************************************/
/*	BeamCluster	implementation
/*****************************************************************************/
BeamCluster::BeamCluster()
{
	m_bFixJoints = false;
}

void BeamCluster::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_bFixJoints;
} // BeamCluster::Serialize

void BeamCluster::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
	is >> m_bFixJoints;
} // BeamCluster::Unserialize

void BeamCluster::Flush()
{
	Node::Render();
	if (m_bFixJoints) FixStripJoints();
	RenderQuads();
} // PlaneCluster::Render

void BeamCluster::Render()
{
	UpdateParticles( Animation::CurTimeDelta() );

	PushTM( tm );
	ResetQuadBuffer();
	if (GetMaxParticles() != GetMaxQuads()) SetMaxQuads( GetMaxParticles() );
	for (int i = 0; i < m_MaxParticles; i++)
	{
		const Particle* p = &m_Particles[i]; 
		const Particle* prev = (p->m_PrevParticle == 0xFFFFFFFF) ? 
								p : &m_Particles[p->m_PrevParticle];	
		if (p->IsFree()) continue;

		AddQuad( prev->m_Pos, prev->m_Size.x, prev->GetColor(), 
				 p->m_Pos, p->m_Size.x, p->GetColor(), p->m_UV );
	}
	
	ParticleCluster::Render();

	PopTM();
} // BeamCluster::Render

void BeamCluster::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "BeamCluster", this );
	pm.f( "FixJoints", m_bFixJoints );
} // BeamCluster::Expose

/*****************************************************************************/
/*	PolyObjectCluster	implementation
/*****************************************************************************/
void PolyObjectCluster::Render()
{
	if (m_LastUpdateFrame == IRS->GetCurFrame()) return;

	UpdateParticles( Animation::CurTimeDelta() );

	PushTM( tm );
	
	for (int i = 0; i < m_MaxParticles; i++)
	{
		const Particle& p = m_Particles[i]; 
		if (p.IsFree()) continue;
		Matrix3D rot;
		rot.rotation( p.m_RotAxis, p.m_Rotation );
		IRS->SetTextureFactor( p.GetColor() );
		PushTM( Matrix4D( p.m_Size, rot, p.m_Pos ) );
		Node::Render();
		PopTM();
	}

	PopTM();
} // PolyObjectCluster::Render

/*****************************************************************************/
/*	MeshPolyCluster	implementation
/*****************************************************************************/
MeshPolyCluster::MeshPolyCluster()
{

} // MeshPolyCluster::MeshPolyCluster

void MeshPolyCluster::Render()
{
	UpdateParticles( Animation::CurTimeDelta() );

	PushTM( tm );

	for (int i = 0; i < m_MaxParticles; i++)
	{
		const Particle& p = m_Particles[i]; 
		if (p.IsFree()) continue;
	}

	PopTM();
} // MeshPolyCluster::Render

void MeshPolyCluster::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "MeshPolyCluster", this );
}

void MeshPolyCluster::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
}

void MeshPolyCluster::Unserialize( InStream& is  )
{
	Parent::Unserialize( is );
}

END_NAMESPACE(sg)
