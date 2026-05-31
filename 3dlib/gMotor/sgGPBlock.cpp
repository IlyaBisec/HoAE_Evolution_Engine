/*****************************************************************************/
/*	File:	sgGPBlock.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	09-12-2003
/*****************************************************************************/
#include "stdafx.h"

#include "kCache.h"
#include "kHash.hpp"
#include "kResource.h"
#include "kIOHelpers.h"

#include "mMath2D.h"
#include "mAlgo.h"

#include "sgNodePool.h"
#include "sgNode.h"
#include "sgMovable.h"
#include "sgAssetNode.h"
#include "sgController.h"

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
#include "sgApplication.h"

#ifndef _INLINES
#include "sgGPBlock.inl"
#endif // _INLINES

BEGIN_NAMESPACE(sg)
/*****************************************************************************/
/*	GPBlock implementation
/*****************************************************************************/
void GPBlock::Render()
{
	static int gpID = GPSystem::instance()->PreLoadGPImage( "Models\\gnest" );
	GPSystem::instance()->LoadGP( gpID );
		
	if (bScreenSpace)
	{
		for (int i = 0; i < spr.size(); i++)
		{
			const GPSpriteInstance& inst = spr[i];
			if (!inst.pAnim) continue;
			inst.pAnim->GetNFrames();

			int frameID = 0;
			GPSystem::instance()->DrawSprite(	gpID, frameID, 
				inst.pos.x + tm.e30, inst.pos.y + tm.e31, inst.pos.z + tm.e32, 
				inst.mirrored, inst.color );
		}
		GPSystem::instance()->FlushBatches();
		GPSystem::instance()->Dump();
	}
	else
	{
		s_TMStack.Push( tm );
		m_WorldTM = s_TMStack.Top();

		RC::iRS->SetWorldMatrix( m_WorldTM );
		
		for (int i = 0; i < spr.size(); i++)
		{
			const GPSpriteInstance& inst = spr[i];
			if (!inst.pAnim) continue;

			inst.pAnim->GetNFrames();

			GPSystem::instance()->LoadGP( gpID );

			int frameID = 0;
			GPSystem::instance()->DrawWSprite(	gpID, frameID, 
									inst.pos.x, inst.pos.y, inst.pos.z, 
									inst.mirrored, inst.color );
		}
		GPSystem::instance()->FlushBatches();
		s_TMStack.Pop();
	}

} // GPBlock::Render

void GPBlock::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
}

void GPBlock::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
}

bool GPBlock::AddSprite( GPUnitDescr* pUnit, const Vector3D& pos, 
							bool mirrored, DWORD color )
{
	GPSpriteInstance inst;

	inst.pUnitDescr	= pUnit;
	inst.pAnim		= NULL;
	inst.pos		= pos;
	inst.mirrored	= mirrored;
	inst.color		= color; 
	spr.push_back( inst );
	return true;
} // GPBlock::AddSprite

/*****************************************************************************/
/*	GPAnimation implementation
/*****************************************************************************/
void GPAnimation::Render()
{
}

void GPAnimation::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_Anim << m_NDir;
}

void GPAnimation::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
	is >> m_Anim >> m_NDir;
}

void GPAnimation::AddKeyFrame( int gpID, int sprID )
{
	GPKeyframe key;
	key.gpID	= gpID;
	key.frameID	= sprID;
	float cTime = m_Anim.GetMaxTime() + 1.0f;
	if (m_Anim.GetNKeys() == 0) cTime = 0.0f; 
	m_Anim.AddKey( cTime, key );
} // GPAnimation::AddKeyFrame

void GPAnimation::AddFrameRange( int gpID, int startFrame, int lastFrame )
{
	GPKeyframe key1;
	key1.gpID		= gpID;
	key1.frameID	= startFrame;
	
	GPKeyframe key2;
	key2.gpID		= gpID;
	key2.frameID	= lastFrame;

	float cTime1 = m_Anim.GetMaxTime();
	if (cTime1 > 0.0f) cTime1 += 1.0f;
	float cTime2 = cTime1 + fabs( static_cast<float>( lastFrame - startFrame ) );
	m_Anim.AddKey( cTime1, key1 );
	m_Anim.AddKey( cTime2, key2 );
} // GPAnimation::AddFrameRange

void GPAnimation::AddKeyFrame( float time, int gpID, int sprID )
{
	GPKeyframe key;
	key.gpID	= gpID;
	key.frameID	= sprID;
	m_Anim.AddKey( time, key );
}

/*****************************************************************************/
/*	GPFile implementation
/*****************************************************************************/
void GPFile::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
}

void GPFile::Unserialize( InStream& is	)
{
	Parent::Unserialize( is );
}

/*****************************************************************************/
/*	GPFrameNode implementation
/*****************************************************************************/
void GPFrameNode::Render()
{
	if (handle == c_BadHandle)
	{
		Precache();
	}
	
	if (handle != c_BadHandle)
	{
		GPSystem::instance()->SetScale( GetScale() );
		GPSystem::instance()->SetCurrentDiffuse( color );
		GPSystem::instance()->DrawSprite( handle, frameIdx, pos.x, pos.y, pos.z, false );
		GPSystem::instance()->FlushBatches();
	}
} // GPFrameNode::Render

bool GPFrameNode::Precache()
{
	if(!GetGPName()) return false;
	handle = GPSystem::instance()->PreLoadGPImage( GetGPName() );
	bool allOk =  GPSystem::instance()->LoadGP( handle );
	return allOk;
} // GPFrameNode::Precache

void GPFrameNode::Serialize( OutStream& os ) const
{
	Parent::Serialize( os );
	os << frameIdx << gpName << color;
} // GPFrameNode::Serialize

void GPFrameNode::Unserialize( InStream& is )
{
	Parent::Unserialize( is );
	is >> frameIdx >> gpName >> color;
	handle = c_BadHandle;
} // GPFrameNode::Unserialize

void GPFrameNode::Expose( PropertyMap& pmap )
{
	pmap.start<Parent>( "GPFrameNode", this				);
	pmap.prop ( "GPName", GetGPName, SetGPName, "file"	);
	pmap.field( "FrameIndex",	frameIdx				);
	pmap.field( "Color",		color,		"color"		);
	pmap.prop ( "Handle",		GetGPHandle				);

} // GPFrameNode::Expose

Rct	GPFrameNode::GetBounds()
{
	if (handle == c_BadHandle)
	{
		Precache();
	}
	Rct rct;
	GPSystem::instance()->GetGPBoundFrame( handle, frameIdx, rct );
	return rct;
} // GPFrameNode::GetBounds

int	GPFrameNode::GetGPWidth()
{
	if (handle == c_BadHandle)
	{
		Precache();
	}
	return GPSystem::instance()->GetFrameWidth( handle, frameIdx );
} // GPFrameNode::GetGPWidth

int	GPFrameNode::GetGPHeight()
{
	if (handle == c_BadHandle)
	{
		Precache();
	}
	return GPSystem::instance()->GetFrameHeight( handle, frameIdx );
} // GPFrameNode::GetGPHeight

void GPFrameNode::SetGPName( const char* _gpName )
{ 
	if (!_gpName) return;
	char buf[_MAX_PATH];
	strcpy( buf, _gpName );
	Application::instance()->ToRelativePath( buf, _MAX_PATH );

	char* cExt = strrchr( buf, '.' );
	if (cExt) *cExt = 0;
	gpName = buf;
	handle = c_BadHandle;
} // GPFrameNode::SetGPName


/*****************************************************************************/
/*	GPImpostor implementation
/*****************************************************************************/
GPImpostor::GPImpostor()
{
	m_GPID				= -1;
	m_RealFrame			= 0;
	m_AnimFrame			= 0;
	m_NDirections		= 16;
	m_bHalfCircle		= false;
	m_bBillboard		= false;
	m_PivotX			= 0.0f;
	m_PivotY			= 0.0f;
	m_Scale				= 1.0f;
	m_Direction			= 0;
	m_Color				= 0xFFFFFFFF;

	m_MaxH				= 0.0f;
	m_MaxHalfW			= 0.0f;

	SetTransform		( Matrix4D::identity );
	SetInitialTM	( Matrix4D::identity );
} // GPImpostor::GPImpostor

void GPImpostor::Render()
{
	s_TMStack.Push( tm );
	m_WorldTM = s_TMStack.Top();
	
	if (m_GPID == c_BadHandle)
	{
		Precache();
	}

	if (m_GPID != c_BadHandle)
	{
		Matrix4D tmatr = GetImpostorMatrix();
		if (!m_bBillboard)
		{
			m_RealFrame = m_AnimFrame * m_NDirections + abs( m_Direction );
		}
		else
		{
			m_RealFrame = m_AnimFrame;
		}

		GPSystem::instance()->SetCurrentDiffuse( m_Color );
		GPSystem::instance()->DrawWSpriteTransformed( m_GPID, m_RealFrame, tmatr );
		RC::iRS->SetWorldMatrix( Matrix4D::identity );

		//  +HACKHACK
		if (!m_bBillboard) SetAnimFrame( GetTickCount() / 50 );
		//  -HACKHACK
	}

	s_TMStack.Pop();
} // GPImpostor::Render

void GPImpostor::Serialize	( OutStream& os ) const
{
	Parent::Serialize( os );
	os << m_RealFrame << m_AnimFrame << m_NDirections << m_bHalfCircle << m_bBillboard 
		<< m_GPName << m_Scale << m_PivotX << m_PivotY;
}

void GPImpostor::Unserialize	( InStream&	 is )
{
	Parent::Unserialize( is );
	is >> m_RealFrame >> m_AnimFrame >> m_NDirections >> m_bHalfCircle >> m_bBillboard >>
		m_GPName >> m_Scale >> m_PivotX >> m_PivotY;
}

void GPImpostor::Expose( PropertyMap& pmap )
{
	pmap.start<Parent>( "GPImpostor", this );
	pmap.field( "GPHandle",		m_GPID			);
	pmap.field( "RealFrame",	m_RealFrame		);
	pmap.prop ( "AnimFrame",	GetAnimFrame, SetAnimFrame );
	pmap.field( "NDir",			m_NDirections	);
	pmap.field( "HalfCircle",	m_bHalfCircle	);
	pmap.field( "Billboard",	m_bBillboard	);
	pmap.prop ( "GPName",		GetGPName, SetGPName, "file" );
	pmap.field( "PivotX",		m_PivotX );
	pmap.field( "PivotY",		m_PivotY );
	pmap.field( "Scale",		m_Scale  );
	pmap.field( "Dir",			m_Direction );
	pmap.field( "Color",		m_Color, "color" );
	pmap.field( "MaxH",			m_MaxH,		NULL, true );
	pmap.field( "MaxHalfW",		m_MaxHalfW, NULL, true );
} // GPImpostor::Expose

Node* GPImpostor::Clone() const
{
	GPImpostor* clone = (GPImpostor*)Parent::Clone();
	clone->m_AnimFrame = m_AnimFrame;
	clone->m_PivotX = m_PivotX;
	clone->m_PivotY = m_PivotY;
	clone->m_Scale = m_Scale;
	clone->m_GPName = m_GPName;
	clone->m_bHalfCircle = m_bHalfCircle;
	clone->m_NDirections = m_NDirections;
	return clone;
} // GPImpostor::Clone

bool GPImpostor::Precache()
{
	if(!GetGPName()) return false;
	m_GPID = GPSystem::instance()->PreLoadGPImage( GetGPName() );
	bool allOk =  GPSystem::instance()->LoadGP( m_GPID );
	Rct rct;
	GPSystem::instance()->GetGPBoundFrame( m_GPID, 0, rct );
	m_MaxHalfW = rct.w * 0.5;
	m_MaxH = rct.h;
	return allOk;
} // GPImpostor::Precache

int	GPImpostor::GetNFrames()
{
	if (m_GPID == c_BadHandle)
	{
		Precache();
	}
	return GPSystem::instance()->GPNFrames( m_GPID );
} // GPImpostor::GetNFrames

void GPImpostor::SetGPName( const char* _gpName )
{ 
	if (!_gpName) return;
	char buf[_MAX_PATH];
	strcpy( buf, _gpName );
	Application::instance()->ToRelativePath( buf, _MAX_PATH );

	char* cExt = strrchr( buf, '.' );
	if (cExt) *cExt = 0;
	m_GPID = c_BadHandle;
	m_GPName = buf;
} // GPFrameNode::SetGPName
END_NAMESPACE(sg)


