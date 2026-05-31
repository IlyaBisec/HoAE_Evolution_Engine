/*****************************************************************************/
/*	File:	sgGPBlock.h
/*	Author:	Ruslan Shestopalyuk
/*	Date:	09-12-2003
/*****************************************************************************/
#ifndef __SGGPBLOCK_H__
#define __SGGPBLOCK_H__

namespace sg{

class GPUnitDescr;
class GPAnimation;

struct GPSpriteInstance
{
	GPUnitDescr*	pUnitDescr;
	GPAnimation*	pAnim;
	float			curAnimTime;

	Vector3D		pos;
	bool			mirrored;
	DWORD			color;
}; // struct GPSpriteInstance

/*****************************************************************************/
/*	Class:	GPBlock
/*	Desc:	Block of sprites, drawn by GPSystem in screen space
/*****************************************************************************/
class GPBlock : public TransformNode
{
public:
					GPBlock() : bScreenSpace( true ){}
	virtual void	Render();

	virtual void	Serialize( OutStream& os ) const;
	virtual void	Unserialize( InStream& is );

	bool			AddSprite( GPUnitDescr* pUnit, const Vector3D& pos, 
								bool mirrored = false, DWORD color = 0x00000000 );

	void			SetIsScreenSpace( bool bSSpace ) { bScreenSpace = bSSpace; }
	bool			GetIsScreenSpace() const { return bScreenSpace; }

	MAGIC(GPBlock,TransformNode,GPSB);

protected:
	std::vector<GPSpriteInstance>		spr;
	bool							bScreenSpace;

}; // class GPBlock

/*****************************************************************************/
/*	Class:	GPFile
/*	Desc:	GP sprite package file
/*****************************************************************************/
class GPFile : public AssetNode
{
public:
					GPFile(){}			
	virtual void	Serialize			( OutStream& os ) const;
	virtual void	Unserialize			( InStream& is	);

	MAGIC(GPFile,AssetNode,GPFI);
}; // class GPFile

static const int	c_BadHandle = -1;
/*****************************************************************************/
/*	Class:	GPFrameNode
/*	Desc:	Single sprite reference
/*****************************************************************************/
class GPFrameNode : public HudNode
{
	int					frameIdx;
	std::string			gpName;
	DWORD				color;

	int					handle;

public:
						GPFrameNode() : handle( c_BadHandle ), 
										color(0xFFFFFFFF), frameIdx(0) {}					

	_inl void			SetFrameIndex	( int val )				{ frameIdx = val;	}
	
	void				SetGPName		( const char* _gpName );
	
	_inl int			GetFrameIndex	() const				{ return frameIdx;	}
	_inl const char*	GetGPName		() const				{ return gpName.c_str();	}
	_inl int			GetGPHandle		() const				{ return handle;	}

	_inl DWORD			GetColor		() const				{ return color; };	
	int					GetGPWidth		();
	int					GetGPHeight		();

	_inl void			SetGPWidth		( int val ) {}
	_inl void			SetGPHeight		( int val ) {}


	_inl void			SetColor		( DWORD val ) { color &= 0xFF000000; color |= val; }
	_inl void			SetGPHandle		( int   val )			{ handle = val;}

	_inl void			GPToScreenSpace ( Vector3D& vec ); 
	_inl void			GPToScreenSpace ( Vector4D& vec );
	_inl void			ScreenToGPSpace ( Vector3D& vec ); 
	_inl void			ScreenToGPSpace ( Vector4D& vec );

	virtual Rct			GetBounds();

	_inl float			GetAlpha		() const 
	{ 
		return float( (color & 0xFF000000) >> 24 )/255.0f; 
	}			
	_inl void			SetAlpha		( float val ) 
	{ 
		color &= 0x00FFFFFF; 
		color |= DWORD( val * 255.0f ) << 24;
	}				


	virtual	void		Render();
	virtual void		Serialize	( OutStream& os ) const;
	virtual void		Unserialize	( InStream&	 is );
	virtual void		Expose		( PropertyMap& pmap );

	bool				Precache();

	MAGIC(GPFrameNode,HudNode,GPFR);
}; // class GPFrameNode

/*****************************************************************************/
/*	Class:	GPImpostor
/*****************************************************************************/
class GPImpostor : public TransformNode
{
	int					m_GPID;
	int					m_RealFrame;
	int					m_AnimFrame;
	int					m_NDirections;
	
	bool				m_bHalfCircle;
	bool				m_bBillboard;

	std::string  		m_GPName;

	float				m_PivotX, m_PivotY;
	float				m_Scale;
	int					m_Direction;
	DWORD				m_Color;

	//  bounding volume
	float				m_MaxH;
	float				m_MaxHalfW;
	
	_inl AABoundBox		GetAABB() const 
	{ 
		return AABoundBox( Vector3D( tm.e30 - m_MaxHalfW, tm.e31 - m_MaxHalfW, tm.e32 ),
					Vector3D( tm.e30 + m_MaxHalfW, tm.e31 + m_MaxHalfW, tm.e32 + m_MaxH ) );
	}

public:
						GPImpostor			();
	virtual void		Render				();

	virtual void		Serialize			( OutStream& os ) const;
	virtual void		Unserialize			( InStream&	 is );
	virtual void		Expose				( PropertyMap& pmap );
	virtual Node*		Clone				() const;
	void				SetColor			( DWORD color ) { m_Color = color; }

	_inl bool			Intersect( int scrX, int scrY, const Ray3D& ray, TestPrecision precision ) const;
	_inl bool			Intersect( const Frustum& fr ) const;

	bool				Precache			();
	int					GetNFrames			();

	void				SetGPName			( const char* _gpName );
	_inl const char*	GetGPName			() const { return m_GPName.c_str();	}
	_inl Matrix4D		GetImpostorMatrix	();

	_inl void			SetAnimFrame		( int frame );
	_inl int			GetAnimFrame		() const { return m_AnimFrame; }

	MAGIC(GPImpostor,TransformNode,GPIM);
protected:

}; // GPImpostor

struct GPKeyframe
{
	int		gpID;
	int		frameID;
}; // struct GPKeyframe

/*****************************************************************************/
/*	Class:	GPAnimationCurve
/*	Desc:	GP Frame sequence description
/*****************************************************************************/
class GPAnimationCurve : public AnimationCurve<GPKeyframe>
{
public:

}; // class GPAnimationCurve

/*****************************************************************************/
/*	Class:	GPAnimation
/*	Desc:	Node that controls animation of the sprite block
/*****************************************************************************/
class GPAnimation : public Animation
{
	GPAnimationCurve		m_Anim;
	int						m_NDir;

public:
					GPAnimation() : m_NDir(0) {}

	virtual void	Render();

	virtual void	Serialize( OutStream& os ) const;
	virtual void	Unserialize( InStream& is );

	void			SetNDirections( int ndir ) { m_NDir = ndir; } 
	void			AddKeyFrame( int gpID, int sprID );
	void			AddFrameRange( int gpID, int startFrame, int lastFrame );
	void			AddKeyFrame( float time, int gpID, int sprID );

	GPAnimationCurve*	GetFramesAnimation() { return &m_Anim; }
	void				SetFramesAnimation( GPAnimationCurve* pAnm ) {}

	int				GetNDirections() const { return m_NDir; }
	int				GetNFrames() const { return m_Anim.GetMaxTime() + 1; }
	void			SetNFrames( int nFr ) {}

	MAGIC(GPAnimation,Animation,GPAN);
}; // class GPAnimation

}; // namespace sg

#ifdef _INLINES
#include "sgGPBlock.inl"
#endif // _INLINES

#endif // __SGGPBLOCK_H__
