/*****************************************************************************/
/*	File:	sgBillboardGroup.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	08-21-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sgBillboardGroup.h"

BEGIN_NAMESPACE(sg)

/*****************************************************************************/
/*	BillboardGroup implementation
/*****************************************************************************/
BillboardGroup::BillboardGroup()
{
	SetMaxQuads( c_DefaultMaxBillboardQuads );
	SetBillboardTM( Matrix3D::identity );
}

BillboardGroup::~BillboardGroup()
{
}

void BillboardGroup::Render()
{
	m_QuadBuffer.setNVert( m_NQuads * 4 );
	m_QuadBuffer.setNPri( m_NQuads * 2 );
	IRS->DrawPrim( m_QuadBuffer );
}

void BillboardGroup::ResetQuadBuffer()
{
	m_NQuads = 0;
	m_QuadBuffer.setNVert( 0 );
}

void BillboardGroup::SetBillboardTM( const Matrix3D& m )
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
} // BillboardGroup::SetBillboardTM

bool BillboardGroup::AddQuad( const Vector3D& pos, float sizeX, float sizeY,
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
} // BillboardGroup::AddQuad

bool BillboardGroup::AddQuad( const Vector3D& pos, float size, DWORD color, const Rct& uv )
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
} // BillboardGroup::AddQuad

void BillboardGroup::SetMaxQuads( int nQuads )
{
	m_MaxQuads = nQuads;
	m_NQuads = 0;
	m_QuadBuffer.create( m_MaxQuads * 4, 0, vf2Tex, ptTriangleList );
	m_QuadBuffer.setIsQuadList( true );
} //  BillboardGroup::SetMaxQuads


END_NAMESPACE(sg)
