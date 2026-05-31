/*****************************************************************************/
/*	File:	sgGPSpriteBlock.h
/*	Author:	Ruslan Shestopalyuk
/*	Date:	09-12-2003
/*****************************************************************************/
#ifndef __SGGPSPRITEBLOCK_H__
#define __SGGPSPRITEBLOCK_H__

namespace sg{

struct GPSpriteInstance
{
	int			gpID;
	int			frameID;
	Vector3D	pos;
	bool		mirrored;
	DWORD		color;

}; // struct GPSpriteInstance

/*****************************************************************************/
/*	Class:	GPSpriteBlock
/*	Desc:	Block of sprites, drawn by GPSystem in screen space
/*****************************************************************************/
class GPSpriteBlock : public MovableNode
{
public:
	virtual void	Render();

	virtual bool	Serialize( OutStream& os ) const;
	virtual bool	Unserialize( InStream& is );

	bool			AddSprite( const char* gpName, int frame, 
								const Vector3D& pos, bool mirrored = false );
	
	MAGIC(GPSpriteBlock,MovableNode,GPSB);

protected:
	c2::array<GPSpriteInstance>		spr;

}; // class GPSpriteBlock

/*****************************************************************************/
/*	Class:	GPSpriteWBlock
/*	Desc:	Block of sprites, drawn by GPSystem in world space
/*****************************************************************************/
class GPSpriteWBlock : public GPSpriteBlock
{
public:
	GPSpriteWBlock();
	
	virtual void	Render();
	
	MAGIC(GPSpriteWBlock,GPSpriteBlock,GPSW);
}; // class GPSpriteWBlock

/*****************************************************************************/
/*	Class:	GPFile
/*	Desc:	GP sprite package file
/*****************************************************************************/
class GPFile : public AssetNode
{
	float		pivotX, pivotY;
public:
	
	float		GetPivotX() const { return pivotX; }
	float		GetPivotY() const { return pivotY; }
	void		SetPivotX( float pivX ) { pivotX = pivX; }
	void		SetPivotY( float pivY ) { pivotY = pivY; }

	virtual bool	Serialize			( OutStream& os ) const;
	virtual bool	Unserialize			( InStream& is	);


	MAGIC(GPFile,AssetNode,GPFI);
}; // class GPFile

/*****************************************************************************/
/*	Class:	GPEntity
/*	Desc:	GP map entity description
/*****************************************************************************/
class GPEntity : public AssetNode
{
public:

	MAGIC(GPEntity,AssetNode,GPEN);
}; // class GPEntity

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
	GPAnimationCurve		anim;
	int						nDir;

public:
					GPAnimation() : nDir(0) {}

	virtual void	Render();

	virtual bool	Serialize( OutStream& os ) const;
	virtual bool	Unserialize( InStream& is );

	void			SetNDirections( int ndir ) { nDir = ndir; } 
	void			AddKeyFrame( int gpID, int sprID );
	void			AddFrameRange( int gpID, int startFrame, int lastFrame );
	void			AddKeyFrame( float time, int gpID, int sprID );

	GPAnimationCurve*	GetFramesAnimation() { return &anim; }
	void				SetFramesAnimation( GPAnimationCurve* pAnm ) {}

	int				GetNDirections() const { return nDir; }
	int				GetNFrames() const { return anim.GetMaxTime() + 1; }
	void			SetNFrames( int nFr ) {}

	MAGIC(GPAnimation,Animation,GPAN);
}; // class GPAnimation

}; // namespace sg

#endif // __SGGPSPRITEBLOCK_H__
