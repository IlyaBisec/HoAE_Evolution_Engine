/*****************************************************************************/
/*	File:	sgConstraint.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	15.04.2003
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include "sgConstraint.h"

BEGIN_NAMESPACE( sg )
/*****************************************************************************/
/*	PosConstraint implementation
/*****************************************************************************/
void PosConstraint::Render()
{
	m_pHost			= (TransformNode*)GetInput( 0 );
	m_pConstrained	= (TransformNode*)GetInput( 1 );

	if (m_pConstrained && m_pHost)
	{
		const Matrix4D& hWorld = m_pHost->GetTopTM();
		Matrix4D cWorld = m_pConstrained->GetTopTM();
		Vector3D trans( m_Delta );
		trans += hWorld.getTranslation();
		cWorld.setTranslation( trans );
		m_pConstrained->SetWorldTM( cWorld );
	}
} // PosConstraint::Render

void PosConstraint::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "PosConstraint", this );
	pm.f( "DeltaX", m_Delta.x );
	pm.f( "DeltaY", m_Delta.y );
	pm.f( "DeltaZ", m_Delta.z );
}

END_NAMESPACE( sg )
