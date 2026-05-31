#include "stdafx.h"
#include "sgNodePool.h"
#include "sgNode.h"
#include "sgMovable.h"
#include "sgAssetNode.h"
#include "sgController.h"
#include "mMath2D.h"

#include "kCache.h"
#include "kHash.hpp"
#include "kResource.h"
#include "kIOHelpers.h"

#include "gpText.h"
#include "gpPackage.h"

#include "gpGN16.h"
#include "gpGU16.h"

#include "gpBitmap.h"
#include "gpPackage.h"
#include "sgGPSystem.h"

#include "sgDummy.h"
#include "sgGeometry.h"
#include "sgCamera.h"

#include "sgGPBlock.h"
#include "sgGPConstruction.h"

BEGIN_NAMESPACE(sg)

/*****************************************************************************/
/*	GPCarcassObj implementation
/*****************************************************************************/
OutStream&	operator <<( OutStream& os, const GPCarcassChunk& chunk )
{
	os << chunk.nTri << chunk.nVert << chunk.flags;
	return os;
}

InStream&	operator >>( InStream& is, GPCarcassChunk& chunk )
{
	is >> chunk.nTri >> chunk.nVert >> chunk.flags;
	return is;
}

GPCarcassObj::GPCarcassObj()
{
}

void GPCarcassObj::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
} // GPCarcassObj::Serialize

void GPCarcassObj::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
} // GPCarcassObj::Unserialize

/*****************************************************************************/
/*	GPObj implementation
/*****************************************************************************/
void GPObj::Render()
{	
	if (chunks.size() == 0) return;

	if (gpID == -1) 
	{
		const char* pGPName = gpName.c_str();
		while (*pGPName == '\\' || *pGPName == '/') pGPName++;
		gpID = GPSystem::instance()->PreLoadGPImage( pGPName );
		GPSystem::instance()->LoadGP( gpID );
	}

	BaseMesh& bm = GetPrimitive();

	RC::iRS->SetWorldMatrix( TransformNode::TMStackTop() );

	int cPoly = 0;
	int cVert = 0;
	for (int i = 0; i < chunks.size(); i++)
	{
		int nPoly = chunks[i].nTri;
		int nVert = chunks[i].nVert;

		GPSystem::instance()->DrawWChunk( gpID, frameIdx, i, Vector3D::null, bm, cPoly, nPoly, cVert, nVert );
		cPoly += nPoly;
		cVert += nVert;
	}

	GPSystem::instance()->FlushBatches();
} // GPObj::Render

GPObj::GPObj()
{
	gpID		= -1;
	frameIdx	= 0;
}

void GPObj::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << chunks << gpName << frameIdx << m_Pivot;
} // GPObj::Serialize

void GPObj::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
	is >> chunks >> gpName >> frameIdx >> m_Pivot;
	gpID = -1;
} // GPObj::Unserialize

void GPObj::Clear()
{
	GetPrimitive().setNVert( 0 );
	GetPrimitive().setNInd ( 0 );
	GetPrimitive().setNPri ( 0 );

	ClearChunks();
} // GPObj::Clear

void GPObj::ClearChunks()
{
	chunks.clear();
}

void GPObj::Expose( PropertyMap& pmap )
{
	pmap.start<Parent>( "GPObj", this );
	pmap.prop( "GPFile", GetGPName, SetGPName, "file" );
	pmap.field( "GPHandle", gpID );
	pmap.field( "FrameIdx", frameIdx );
	pmap.field( "CenterX", m_Pivot.x );
	pmap.field( "CenterY", m_Pivot.y );
	pmap.field( "CenterZ", m_Pivot.z );

} // GPObj::Expose

/*****************************************************************************/
/*	GPShadowObj implementation
/*****************************************************************************/
void GPShadowObj::Render()
{
	if (gpID == -1) 
	{
		const char* pGPName = gpName.c_str();
		while (*pGPName == '\\' || *pGPName == '/') pGPName++;
		gpID = GPSystem::instance()->PreLoadGPImage( pGPName );
		GPSystem::instance()->LoadGP( gpID );
	}

	if (frameIdx >= GPSystem::instance()->GPNFrames( gpID )) return;

	RC::iRS->SetWorldMatrix( TransformNode::TMStackTop() );

	BaseMesh& bm = GetPrimitive();
	GPSystem::instance()->DrawWSprite( gpID, frameIdx, Vector3D::null, bm );

	GPSystem::instance()->FlushBatches();
} // GPObj::Render

/*****************************************************************************/
/*	GPBuilding implementation
/*****************************************************************************/
GPBuilding::GPBuilding()
{
	m_pCarcass	= NULL;
	m_pNavimesh = NULL;
	m_pShadow	= NULL;
	m_bShowWire = false;
}

void GPBuilding::Render()
{
	Node::Render();
}

void GPBuilding::Expose( PropertyMap& pmap )
{
	pmap.start<Parent>( "GPBuilding", this );
	pmap.field( "Wire", m_bShowWire );
} // GPBuilding::Expose

void GPBuilding::RenderMain()
{
	if (!m_pCarcass)
	{
		m_pCarcass = FindCarcass();
		if (!m_pCarcass) return;
	}
	m_pCarcass->Render();
} // GPBuilding::RenderMain

void GPBuilding::RenderShadow()
{
	if (!m_pShadow)
	{
		m_pShadow = FindShadow();
		if (!m_pShadow) return;
	}
	m_pShadow->Render();
} // GPBuilding::RenderShadow

GPShadowObj* GPBuilding::FindShadow()
{
	for (int i = 0; i < GetNChildren(); i++)
	{
		if (GetChild( i )->IsA<GPShadowObj>()) return (GPShadowObj*)GetChild( i );
	}
	return NULL;
} // GPBuilding::FindShadow

GPCarcassObj* GPBuilding::FindCarcass()
{
	for (int i = 0; i < GetNChildren(); i++)
	{
		if (GetChild( i )->IsA<GPCarcassObj>()) return (GPCarcassObj*)GetChild( i );
	}
	return NULL;
} // GPBuilding::FindCarcass

INT	GPBuilding::GetMainGP() 
{
	if (!m_pCarcass) m_pCarcass = FindCarcass();
	if (!m_pCarcass) return -1;
	return m_pCarcass->GetGPID();
} // GPBuilding::GetMainGP

int	GPBuilding::GetMainFrame() 
{
	if (!m_pCarcass) m_pCarcass = FindCarcass();
	if (!m_pCarcass) return -1;
	return m_pCarcass->GetFrameIdx();
} // GPBuilding::GetMainFrame

Vector3D GPBuilding::GetPivot()
{
	if (!m_pCarcass) m_pCarcass = FindCarcass();
	if (!m_pCarcass) return Vector3D::null;
	return m_pCarcass->GetPivot();
} // GPBuilding::GetPivot

END_NAMESPACE(sg)
