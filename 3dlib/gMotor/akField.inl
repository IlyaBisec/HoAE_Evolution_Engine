/*****************************************************************************/
/*	File:	akField.inl
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	16.05.2003
/*****************************************************************************/

/*****************************************************************************/
/*	FieldModel implementation
/*****************************************************************************/
_inl void FieldModel::EnableWireframe( bool enable )
{
	if (enable)
	{
		static int shWire = IRS->GetShaderID( "wireSprite" );
		m_FieldMesh.setShader( shWire );
	}
	else
	{
		static int shField = IRS->GetShaderID( "field" );
		m_FieldMesh.setShader( shField );
	}
} // FieldModel::EnableWireframe

_inl void FieldModel::Setm_StrawHeight( float _m_StrawHeight )
{
	m_StrawHeight = _m_StrawHeight;
} // FieldModel::Setm_StrawHeight

_inl DWORD FieldModel::GetGrowColor( float growRatio ) const
{
	static const DWORD c_StartAlpha = 10;
	if (growRatio > 1.0f) growRatio = 1.0f;
	DWORD bclr = c_StartAlpha + growRatio * (255 - c_StartAlpha);
	DWORD growClr = 0x0000FF00 | (bclr << 24) | (bclr << 16) | (bclr);
	return growClr;
} // FieldModel::GetGrowColor

_inl DWORD FieldModel::GetAnimationTime() const
{
	return GetTickCount();
} // FieldModel::GetAnimationTime

_inl float FieldModel::GetStrawBend( float strawX, float strawY, float ftime,
									 float& bendDirX, float& bendDirY ) const
{
	float frnd = m_Rnd[int(fabs(strawX + strawY))%c_NumFieldRnd];
	float frnd1 = m_Rnd[int(fabs(strawX))%c_NumFieldRnd];
	bendDirX = 0.5f - frnd;
	bendDirY = 0.5f - frnd1;

	return 1.8f*sin( ftime * 0.0004f + frnd )*frnd1;
} // FieldModel::GetStrawBend

_inl float FieldModel::GetStrawPitch( float heightFromGround, float bendAmount )
{
	return heightFromGround*heightFromGround*bendAmount*0.01f;
} // FieldModel::GetStrawPitch

_inl float FieldModel::GetTexShift( float strawX, float strawY )
{
	return m_Rnd[int(fabs(strawY + strawX))%c_NumFieldRnd];
} // FieldModel::GetTexShift

_inl float FieldModel::GetStrawHeight( float strawX, float strawY, float growRatio )
{
	return m_StrawHeight*(1.0f - m_Rnd[int( fabs( strawY - strawX ) ) % c_NumFieldRnd]*0.1f)*growRatio;
} // FieldModel::GetStrawHeight
	
