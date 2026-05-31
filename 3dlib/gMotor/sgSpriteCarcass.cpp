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

#include "sgDummy.h"
#include "sgGeometry.h"
#include "sgCamera.h"

#include "sgSprite.h"
#include "sgSpriteCarcass.h"

BEGIN_NAMESPACE(sg)

/*****************************************************************************/
/*	CarcassObject implementation
/*****************************************************************************/
OutStream&	operator <<( OutStream& os, const CarcassChunk& chunk )
{
	os << chunk.nTri << chunk.nVert << chunk.flags;
	return os;
}

InStream&	operator >>( InStream& is, CarcassChunk& chunk )
{
	is >> chunk.nTri >> chunk.nVert >> chunk.flags;
	return is;
}

CarcassObject::CarcassObject()
{
}

void CarcassObject::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
} // CarcassObject::Serialize

void CarcassObject::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
} // CarcassObject::Unserialize

/*****************************************************************************/
/*	CarcassSprite implementation
/*****************************************************************************/
CarcassSprite::CarcassSprite()
{
	m_SeqID			= -1;
	m_FrameIdx		= 0;
} // CarcassSprite::CarcassSprite

void CarcassSprite::Render()
{	
	if (m_Chunks.size() == 0) return;

	if (m_SeqID == -1) 
	{
		const char* pGPName = m_SeqName.c_str();
		while (*pGPName == '\\' || *pGPName == '/') pGPName++;
		m_SeqID = ISM->GetPackageID( pGPName );
		ISM->LoadPackage( m_SeqID );
	}

	BaseMesh& bm = GetPrimitive();

	ISM->FlushBatches();
	IRS->SetWorldMatrix( TransformNode::TMStackTop() );

	int cPoly = 0;
	int cVert = 0;
	int nChunks = m_Chunks.size();
	for (int i = 0; i < nChunks; i++)
	{
		int nPoly = m_Chunks[i].nTri;
		int nVert = m_Chunks[i].nVert;

		ISM->DrawWChunk( m_SeqID, m_FrameIdx, i, Vector3D::null, bm, cPoly, nPoly, cVert, nVert );
		cPoly += nPoly;
		cVert += nVert;
	}
	ISM->FlushBatches();
} // CarcassSprite::Render

void CarcassSprite::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Chunks << m_SeqName << m_FrameIdx << m_Pivot;
} // CarcassSprite::Serialize

void CarcassSprite::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
	is >> m_Chunks >> m_SeqName >> m_FrameIdx >> m_Pivot;
	m_SeqID = -1;
} // CarcassSprite::Unserialize

void CarcassSprite::Clear()
{
	GetPrimitive().setNVert( 0 );
	GetPrimitive().setNInd ( 0 );
	GetPrimitive().setNPri ( 0 );

	ClearChunks();
} // CarcassSprite::Clear

void CarcassSprite::ClearChunks()
{
	m_Chunks.clear();
}

void CarcassSprite::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "CarcassSprite", this );
	pm.p( "GPFile", GetPackageName, SetPackageName, "file" );
	pm.f( "GPHandle", m_SeqID );
	pm.f( "FrameIdx", m_FrameIdx );
	pm.f( "CenterX",	m_Pivot.x );
	pm.f( "CenterY",	m_Pivot.y );
	pm.f( "CenterZ",	m_Pivot.z );

} // CarcassSprite::Expose

/*****************************************************************************/
/*	CarcassShadow implementation
/*****************************************************************************/
void CarcassShadow::Render()
{
	if (m_SeqID == -1) 
	{
		const char* pGPName = m_SeqName.c_str();
		while (*pGPName == '\\' || *pGPName == '/') pGPName++;
		m_SeqID = ISM->GetPackageID( pGPName );
		ISM->LoadPackage( m_SeqID );
	}

	if (m_FrameIdx >= ISM->GetNFrames( m_SeqID )) return;

	ISM->FlushBatches();
	IRS->SetWorldMatrix( TransformNode::TMStackTop() );

	BaseMesh& bm = GetPrimitive();	
	ISM->DrawWSprite( m_SeqID, m_FrameIdx, Vector3D::null, bm );
	ISM->FlushBatches( false );
} // CarcassSprite::Render

/*****************************************************************************/
/*	CarcassBuilding implementation
/*****************************************************************************/
CarcassBuilding::CarcassBuilding()
{
	m_pCarcass	= NULL;
	m_pNavimesh = NULL;
	m_pShadow	= NULL;
	m_bShowWire = false;
}

void CarcassBuilding::Render()
{
	Node::Render();
}

void CarcassBuilding::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "CarcassBuilding", this );
	pm.f( "Wire", m_bShowWire );
} // CarcassBuilding::Expose

void CarcassBuilding::RenderMain()
{
	if (!m_pCarcass)
	{
		m_pCarcass = FindCarcass();
		if (!m_pCarcass) return;
	}
	m_pCarcass->Render();
} // CarcassBuilding::RenderMain

void CarcassBuilding::RenderShadow()
{
	if (!m_pShadow)
	{
		m_pShadow = FindShadow();
		if (!m_pShadow) return;
	}
	m_pShadow->Render();
} // CarcassBuilding::RenderShadow

CarcassShadow* CarcassBuilding::FindShadow()
{
	for (int i = 0; i < GetNChildren(); i++)
	{
		if (GetChild( i )->IsA<CarcassShadow>()) return (CarcassShadow*)GetChild( i );
	}
	return NULL;
} // CarcassBuilding::FindShadow

CarcassObject* CarcassBuilding::FindCarcass()
{
	for (int i = 0; i < GetNChildren(); i++)
	{
		if (GetChild( i )->IsA<CarcassObject>()) return (CarcassObject*)GetChild( i );
	}
	return NULL;
} // CarcassBuilding::FindCarcass

INT	CarcassBuilding::GetMainGP() 
{
	if (!m_pCarcass) m_pCarcass = FindCarcass();
	if (!m_pCarcass) return -1;
	return m_pCarcass->GetGPID();
} // CarcassBuilding::GetMainGP

int	CarcassBuilding::GetMainFrame() 
{
	if (!m_pCarcass) m_pCarcass = FindCarcass();
	if (!m_pCarcass) return -1;
	return m_pCarcass->GetFrameIdx();
} // CarcassBuilding::GetMainFrame

Vector3D CarcassBuilding::GetPivot()
{
	if (!m_pCarcass) m_pCarcass = FindCarcass();
	if (!m_pCarcass) return Vector3D::null;
	return m_pCarcass->GetPivot();
} // CarcassBuilding::GetPivot

END_NAMESPACE(sg)
