/*****************************************************************************/
/*	File:	sgGPSpriteBlock.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	09-12-2003
/*****************************************************************************/
#include "stdafx.h"
#include "sgNode.h"
#include "sgAssetNode.h"
#include "sgController.h"
#include "sgGPSpriteBlock.h"

#include "kCache.h"

#include "kHash.hpp"
#include "kResource.h"
#include "gpText.h"
#include "gpPackage.h"

#include "gpGN16.h"
#include "gpGU16.h"

#include "gpBitmap.h"
#include "gpPackage.h"
#include "gpSystem.h"

BEGIN_NAMESPACE(sg)

/*****************************************************************************/
/*	Class:	GP 
/*	Desc:	Nifty GPSystem initializer hack class
/*****************************************************************************/
class GP 
{
public:
	static GPSystem* pGP()
	{
		static _gp_inited = false;
		static GPSystem gps;
		if (!_gp_inited && RC::iRS)
		{
			gps.Init( RC::iRS );
			gps.EnableClipping( false );
			gps.SetCurrentShader( RC::iRS->GetShaderID( "hud" ) );
			//gps.SetCurrentShader( 0 );
			_gp_inited = true;
		}
		return &gps;
	}

private:
	GP(){}
	virtual ~GP(){}

}; // class GPSWrapper

/*****************************************************************************/
/*	GPSpriteBlock implementation
/*****************************************************************************/
void GPSpriteBlock::Render()
{
	for (int i = 0; i < spr.size(); i++)
	{
		const GPSpriteInstance& inst = spr[i];
		GP::pGP()->DrawSprite(	inst.gpID, inst.frameID, 
								inst.pos.x, inst.pos.y, inst.pos.z, 
								inst.mirrored, inst.color );
	}

	GP::pGP()->FlushBatches();
} // GPSpriteBlock::Render

bool GPSpriteBlock::Serialize( OutStream& os ) const
{
	MovableNode::Serialize( os );
	return true;
}

bool GPSpriteBlock::Unserialize( InStream& is )
{
	MovableNode::Unserialize( is );
	return true;
}

bool GPSpriteBlock::AddSprite( const char* gpName, int frame, const Vector3D& pos, bool mirrored )
{
	GPSpriteInstance inst;

	inst.gpID = GP::pGP()->PreLoadGPImage( gpName );
	GP::pGP()->LoadGP( inst.gpID );
	inst.frameID	= frame;
	inst.pos		= pos;
	inst.mirrored	= mirrored;
	inst.color		= 0x00000000; 
	spr.push_back( inst );
	return true;
} // GPSpriteBlock::AddSprite

/*****************************************************************************/
/*	GPSpriteWBlock implementation
/*****************************************************************************/
GPSpriteWBlock::GPSpriteWBlock()
{
	Matrix4D tr;
	tr.scaling( 0.01f, 0.01f, 0.01f );
	SetTransform( tr );
}

void GPSpriteWBlock::Render()
{
	MovableNode::Render();

	for (int i = 0; i < spr.size(); i++)
	{
		const GPSpriteInstance& inst = spr[i];
		GP::pGP()->DrawWSprite( inst.gpID, inst.frameID, 
								inst.pos.x, inst.pos.y, inst.pos.z, 
								inst.mirrored, inst.color );
	}
	
	GP::pGP()->FlushBatches();
} // GPSpriteWBlock::Render

/*****************************************************************************/
/*	GPAnimation implementation
/*****************************************************************************/
void GPAnimation::Render()
{
}

bool GPAnimation::Serialize( OutStream& os ) const
{
	Animation::Serialize( os );
	os << anim << nDir;
	return true;
}

bool GPAnimation::Unserialize( InStream& is )
{
	Animation::Unserialize( is );
	is >> anim >> nDir;
	return true;
}

void GPAnimation::AddKeyFrame( int gpID, int sprID )
{
	GPKeyframe key;
	key.gpID	= gpID;
	key.frameID	= sprID;
	float cTime = anim.GetMaxTime() + 1.0f;
	if (anim.GetNKeys() == 0) cTime = 0.0f; 
	anim.AddKey( cTime, key );
} // GPAnimation::AddKeyFrame

void GPAnimation::AddFrameRange( int gpID, int startFrame, int lastFrame )
{
	GPKeyframe key1;
	key1.gpID		= gpID;
	key1.frameID	= startFrame;
	
	GPKeyframe key2;
	key2.gpID		= gpID;
	key2.frameID	= lastFrame;

	float cTime1 = anim.GetMaxTime();
	if (cTime1 > 0.0f) cTime1 += 1.0f;
	float cTime2 = cTime1 + fabs( static_cast<float>( lastFrame - startFrame ) );
	anim.AddKey( cTime1, key1 );
	anim.AddKey( cTime2, key2 );
} // GPAnimation::AddFrameRange

void GPAnimation::AddKeyFrame( float time, int gpID, int sprID )
{
	GPKeyframe key;
	key.gpID	= gpID;
	key.frameID	= sprID;
	anim.AddKey( time, key );
}

/*****************************************************************************/
/*	GPFile implementation
/*****************************************************************************/
bool GPFile::Serialize( OutStream& os ) const
{
	AssetNode::Serialize( os );
	os << pivotX << pivotY;
	return true;
}

bool GPFile::Unserialize( InStream& is	)
{
	AssetNode::Unserialize( is );
	is >> pivotX >> pivotY;
	return true;
}

END_NAMESPACE(sg)


