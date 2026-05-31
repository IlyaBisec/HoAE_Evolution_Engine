/*****************************************************************************/
/*	File:	sgLight.inl
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	22.04.2003
/*****************************************************************************/


/*****************************************************************************/
/*	LightSource implementation
/*****************************************************************************/
_inl LightSource::LightSource()
{
	m_Ambient = 0xFF000000;	
	m_Diffuse = 0xFFFFFFFF;
	m_Specular= 0xFFFFFFFF;

	m_Index = -1;
} // LightSource::LightSource

_inl LightSource::LightSource( const Vector3D& dir )
{
	SetDir( dir ); 
	
	m_Ambient = 0xFF000000;	
	m_Diffuse = 0xFFFFFFFF;
	m_Specular= 0xFFFFFFFF;

	m_Index = -1;
} // LightSource::LightSource

_inl DWORD	LightSource::GetAmbient()	const
{
	return m_Ambient;
}

_inl DWORD LightSource::GetDiffuse() const
{
	return m_Diffuse;
}

_inl DWORD LightSource::GetSpecular()	const
{
	return m_Specular;
}

_inl Vector3D LightSource::GetPos() const
{
	return  Vector3D( m_LocalTM.e30, m_LocalTM.e31, m_LocalTM.e32 );
}

_inl Vector3D LightSource::GetDir() const
{
	return  Vector3D( m_LocalTM.e20, m_LocalTM.e21, m_LocalTM.e22 );
} // LightSource::GetDir

_inl void LightSource::SetPos( const Vector3D& pos )
{
	m_LocalTM.e30 = pos.x;
	m_LocalTM.e31 = pos.y;
	m_LocalTM.e32 = pos.z;
}

_inl void LightSource::SetDir( const Vector3D& dir )
{
	Vector3D dx = GetDirX();
	Vector3D dy = GetDirY();
	Vector3D dz( dir );
	
	Vector3D::orthonormalize( dz, dx, dy );
	
	SetDirX( dx );
	SetDirY( dy );
	SetDirZ( dz );
} // LightSource::SetDir

_inl void LightSource::SetDiffuse( DWORD diffuse )
{
	m_Diffuse = diffuse;
}

_inl void LightSource::SetAmbient( DWORD ambient )
{
	m_Ambient = ambient;
}

_inl void LightSource::SetSpecular( DWORD specular )
{
	m_Specular = specular;
}

/*****************************************************************************/
/*	PointLight implementation
/*****************************************************************************/
_inl Sphere	PointLight::GetLightSphere() const
{
	return Sphere( GetPos(), GetRange() ); 
} // PointLight::GetLightSphere

/*****************************************************************************/
/*	DirectionalLight implementation
/*****************************************************************************/
_inl Ray3D DirectionalLight::GetLightRay() const
{
	return Ray3D( GetPos(), GetDir() );
} // DirectionalLight::GetLightRay


