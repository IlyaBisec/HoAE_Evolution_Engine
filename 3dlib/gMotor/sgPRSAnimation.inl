/*****************************************************************************/
/*	File:	sgPRSAnimation.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	22.04.2003
/*****************************************************************************/

#include "mSplines.h"
/*****************************************************************************/
/*	PRSAnimation implementation
/*****************************************************************************/
_inl int PRSAnimation::GetPosXNKeys() const
{
	return posX.GetNKeys();
}

_inl int PRSAnimation::GetPosYNKeys() const
{
	return posY.GetNKeys();
}

_inl int PRSAnimation::GetPosZNKeys() const
{
	return posZ.GetNKeys();
}

_inl int PRSAnimation::GetRotNKeys() const
{
	return rot.GetNKeys();
}

_inl int PRSAnimation::GetScaleXNKeys() const
{
	return scX.GetNKeys();
}

_inl int PRSAnimation::GetScaleYNKeys() const
{
	return scY.GetNKeys();
}

_inl int PRSAnimation::GetScaleZNKeys() const
{
	return scZ.GetNKeys();
}

_inl void PRSAnimation::SetBaseAnimationName( const char* basename )
{
	m_BaseAnimationName = basename;
}

_inl const char* PRSAnimation::GetBaseAnimationName() const
{
	return m_BaseAnimationName.c_str();
}

_inl Matrix4D PRSAnimation::GetTransform( float time ) const
{
	Vector3D	sc( scX.GetValue( time ),  scY.GetValue( time ),  scZ.GetValue( time ) );
	Quaternion	quat = rot.GetValue( time );
	Vector3D	tr( posX.GetValue( time ), posY.GetValue( time ), posZ.GetValue( time ) );
	float L=tr.norm();
	return Matrix4D( sc, quat, tr );
	//return Matrix4D( Vector3D(1.1,1.1,1.1), quat, tr );
} // PRSAnimation::GetTransform


