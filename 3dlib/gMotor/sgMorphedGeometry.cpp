/***********************************************************************************/
/*  File:   sgMorphedGeometry.cpp
/*  Date:   14.11.2005
/*  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "stdafx.h"
#include "sgMorphedGeometry.h"

IMPLEMENT_CLASS( MorphedGeometry    );
/*****************************************************************************/
/*    MorphedGeometry    implementation
/*****************************************************************************/
bool MorphedGeometry::s_bFrozen = false;
void MorphedGeometry::ProcessGeometry()
{
	if (s_bFrozen || m_bDisableMorphing) return;
	OnProcessGeometry();
} // MorphedGeometry::ProcessGeometry

void MorphedGeometry::RenderMorphedGeometry()
{
	ProcessGeometry();
	IRS->ResetWorldTM();
	DrawPrimBM( GetMesh() );
} // MorphedGeometry::RenderMorphedGeometry

void MorphedGeometry::Expose( PropertyMap& pm )
{
    pm.start<Parent>( "MorphedGeometry", this );
    pm.f( "DisableMorphing", m_bDisableMorphing );
}

void MorphedGeometry::Serialize( OutStream& os ) const
{
    Parent::Serialize( os );
} // MorphedGeometry::Serialize

void MorphedGeometry::Unserialize( InStream& is )
{
    Parent::Unserialize( is );
    ReplicateMesh();
} // MorphedGeometry::Unserialize

void MorphedGeometry::Render()
{
    SNode::Render();
    RenderMorphedGeometry();
} // MorphedGeometry::Render
