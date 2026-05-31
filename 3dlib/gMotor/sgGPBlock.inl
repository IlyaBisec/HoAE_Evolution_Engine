/*****************************************************************************/
/*	File:	sgGPBlock.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	09-12-2003
/*****************************************************************************/

#include "sgGPSystem.h"
BEGIN_NAMESPACE(sg)

_inl void GPFrameNode::GPToScreenSpace( Vector3D& vec ) 
{ 
	vec.x = vec.x * GetScale() + pos.x; 
	vec.y = vec.y * GetScale() + pos.y; 
	vec.z = vec.z + pos.z; 
}

_inl void GPFrameNode::GPToScreenSpace( Vector4D& vec ) 
{ 
	vec.x = vec.x * GetScale() + pos.x; 
	vec.y = vec.y * GetScale() + pos.y; 
	vec.z = vec.z + pos.z; 
}

_inl void GPFrameNode::ScreenToGPSpace( Vector3D& vec ) 
{ 
	vec.x = (vec.x - pos.x) / GetScale(); 
	vec.y = (vec.y - pos.y) / GetScale(); 
	vec.z = vec.z - pos.z; 
}

_inl void GPFrameNode::ScreenToGPSpace( Vector4D& vec ) 
{ 
	vec.x = (vec.x - pos.x) / GetScale(); 
	vec.y = (vec.y - pos.y) / GetScale(); 
	vec.z = vec.z + pos.z; 
}

_inl Matrix4D GPImpostor::GetImpostorMatrix()
{
	BaseCamera* pCam = BaseCamera::GetActiveCamera();
	if (!pCam) return Matrix4D::identity;

	const Matrix4D& ctm = m_WorldTM;
	Vector4D dir( pCam->GetPos().x - ctm.e30, pCam->GetPos().y - ctm.e31, 0.0f, 0.0f );
	float dx = ctm.e00 * dir.x + ctm.e01 * dir.y;
	float dy = ctm.e10 * dir.x + ctm.e11 * dir.y;

	float dt = atan2( dx, dy );

	dt *= (float( m_NDirections )) / c_DoublePI;
	if (!m_bHalfCircle && m_NDirections > 0) 
	{ 
		dt += 0.5f; 
		while (dt < 0) dt += float( m_NDirections );
		m_Direction = (int( dt )) % m_NDirections;
	}
	else 
	{
		dt *= 2.0f; 
		m_Direction = dt;
	}

	Vector3D cx( pCam->GetRight() );
	Vector3D cy( -ctm.e20, -ctm.e21, -ctm.e22 );
	Vector3D ct( ctm.e30, ctm.e31, ctm.e32 ); 
	Vector3D cz( dir );
	cy.orthonormalize( cz, cx );

	cy /= c_CosPId6;
	if (m_Direction < 0) cx.reverse();

	Matrix4D m;
	m.e00 = cx.x; m.e01 = cx.y;	m.e02 = cx.z; m.e03 = 0.0f;
	m.e10 = cy.x; m.e11 = cy.y;	m.e12 = cy.z; m.e13 = 0.0f;
	m.e20 = cz.x; m.e21 = cz.y;	m.e22 = cz.z; m.e23 = 0.0f;	
	m.e30 = ct.x; m.e31 = ct.y; m.e32 = ct.z; m.e33 = 1.0f;

	Matrix4D pivTM;
	pivTM.translation( -m_PivotX, -m_PivotY, 0.0f );

	if (m_Scale != 1.0f)
	{
		Matrix4D sc;
		sc.scaling( m_Scale );
		m.mulLeft( sc );
	}

	m.mulLeft( pivTM );
	return m;
} // GPImpostor::GetImpostorMatrix

_inl void GPImpostor::SetAnimFrame( int frame )
{
	int nFrames = GetNFrames() / m_NDirections;
	if (nFrames == 0) return;
	m_AnimFrame = frame % nFrames;
} // GPImpostor::SetAnimFrame

_inl bool GPImpostor::Intersect( const Frustum& fr ) const
{
	//  test against cached bounding volume
	if (!fr.Overlap( GetAABB() )) return false;
	return true;
} // GPImpostor::Intersect

_inl bool GPImpostor::Intersect( int scrX, int scrY, const Ray3D& ray, TestPrecision precision ) const
{
	//  test against cached bounding volume
	if (!GetAABB().Overlap( ray )) return false;
	if (precision == tpLow) return true;
	bool bHighPrecision = (precision == tpHigh);
	Vector4D p1( GetPos() );
	Vector4D p2( p1 ); p2.z += m_MaxH;
	BaseCamera::GetActiveCamera()->WorldToScreenSpace( p1 );
	BaseCamera::GetActiveCamera()->WorldToScreenSpace( p2 );
	
	float scale = p2.distance( p1 ) / m_MaxH;
	
	int ptX = (scrX - (p1.x - m_PivotX*scale))/scale;
	int ptY = (scrY - (p1.y - m_PivotY*scale))/scale;
	WORD alpha = GPSystem::instance()->GetGPAlpha( m_GPID, m_RealFrame, 
													ptX, ptY, bHighPrecision );
	return (alpha > 0x1);
} // GPImpostor::Intersect

END_NAMESPACE(sg)
