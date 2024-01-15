/*****************************************************************************/
/*	File:	sgDecal.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	11-13-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sgDecal.h"

#include "mNoise.h"
#include "mHeightmap.h"
#include "uiControl.h"
#include "sgTerrain.h"

BEGIN_NAMESPACE(sg)
/*****************************************************************************/
/*	DecalManager implementation
/*****************************************************************************/
DecalManager::DecalManager()
{
	m_pGlyphSet			= NULL;
	m_pDecalReceiver	= NULL;
	m_pZBias			= NULL;
}

DecalManager::~DecalManager()
{
}

void DecalManager::Create()
{
	AddInput( m_pDecalReceiver );

	m_pZBias = AddChild<ZBias>( "DecalZBias" );
	m_pZBias->SetBias( 0.00005f );

	if (!m_pGlyphSet) return;
	m_pZBias->AddInput( m_pGlyphSet );

	/*int leafPage = m_pGlyphSet->GetPageID( "Earth" );
	GlyphPage* pLeafPage = m_pGlyphSet->GetPage( leafPage );
	if (!pLeafPage) return;
	pLeafPage->SetIsStatic( true );
	int nLeafTypes = pLeafPage->GetNGlyphTypes();
	Ray3D ray( Vector3D::null, Vector3D::oZ );

	if (m_pGlyphSet)
	{
		for (int i = 0; i < 100; i++)
		{
			Vector3D spawn;
			float patchSide = 3000;
			spawn.random( -patchSide, patchSide, -patchSide, patchSide, 0.0f, 0.0f );
			int		glyphType	= rand() % nLeafTypes;
			float	angle		= rndValuef( 0.0f, c_DoublePI );
			float	width		= pLeafPage->GetGlyphWidth( glyphType );
			float	height		= pLeafPage->GetGlyphHeight( glyphType );
			ray.setOrig( spawn.x, spawn.y, spawn.z );
			AddStaticDecal( leafPage, glyphType, ray, angle, width * 1.5f, height * 1.5f );
		}
	}*/

} // DecalManager::Create

bool DecalManager::AddStaticDecal( int decalType, int decalID, const Ray3D& ray, 
									float rotation, float width, float height )
{
	GlyphPage* pPage = m_pGlyphSet->GetPage( decalType );
	if (!pPage) return false;
	const Rct& uv = pPage->GetGlyphUV( decalID );
	
	assert( false );
	return false;
} // DecalManager::AddStaticDecal

void DecalManager::SetDecalReceiver( Node* pReceiver )
{
	m_pDecalReceiver = pReceiver;
} // DecalManager::SetDecalReceiver

void DecalManager::SetGlyphSet( GlyphSet* pGlyphSet )
{
	m_pGlyphSet = pGlyphSet;
} // DecalManager::SetDecalReceiver

void DecalManager::Render()
{
	if (m_pZBias) m_pZBias->Render();
}

END_NAMESPACE(sg)

