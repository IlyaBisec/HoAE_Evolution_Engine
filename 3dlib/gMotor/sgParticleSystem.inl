/*****************************************************************************/
/*	File:	sgParticleSystem.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	22.04.2003
/*****************************************************************************/

BEGIN_NAMESPACE(sg)
/*****************************************************************************/
/*	Particle implementation
/*****************************************************************************/
_inl Matrix4D Particle::GetTransform() const
{
	Matrix3D mrot; 
	Vector3D d( m_Velocity ), s, t;
	d.CreateBasis( s, t );
	mrot.SetRows( s, t, d );
	return Matrix4D( Vector3D::one, mrot, m_Pos );
}

END_NAMESPACE(sg)

