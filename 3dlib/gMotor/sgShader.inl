/*****************************************************************************/
/*	File:	sgShader.inl
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	22.04.2003
/*****************************************************************************/

/*****************************************************************************/
/*	SurfaceMaterial implementation
/*****************************************************************************/
_inl SurfaceMaterial::SurfaceMaterial() : 
	m_Diffuse		( 0xFFFFFFFF ),
	m_Ambient		( 0xFFFFFFFF ),
	m_Specular		( 0xFFFFFFFF ),
	m_Transparency	( 255		 ),
	m_Shininess		( 0.0f		 )
{
}

_inl void SurfaceMaterial::Render()
{
	IRS->SetMaterial( m_Ambient, m_Diffuse, m_Specular, 0, m_Shininess );
}

_inl void	SurfaceMaterial::SetDiffuse( DWORD _diffuse )
{
	m_Diffuse = _diffuse;
}

_inl void	SurfaceMaterial::SetSpecular( DWORD _specular )
{
	m_Specular = _specular;
}

_inl void	SurfaceMaterial::SetAmbient( DWORD _ambient )
{
	m_Ambient = _ambient;
}

_inl void	SurfaceMaterial::SetShininess( float _shininess )
{
	m_Shininess = _shininess;
}

_inl void	SurfaceMaterial::SetTransparency( BYTE _transparency )
{
	_transparency = _transparency;
}

_inl DWORD	SurfaceMaterial::GetDiffuse() const
{
	return m_Diffuse;
}

_inl DWORD	SurfaceMaterial::GetSpecular() const
{
	return m_Specular;
}

_inl DWORD	SurfaceMaterial::GetAmbient() const
{
	return m_Ambient;
}

_inl float	SurfaceMaterial::GetShininess() const
{
	return m_Shininess;
}

_inl BYTE	SurfaceMaterial::GetTransparency() const
{
	return m_Transparency;
}



