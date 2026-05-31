/*****************************************************************************/
/*	File:	sgMovable.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	22.04.2003
/*****************************************************************************/


/*****************************************************************************/
/*	TransformNode implementation
/*****************************************************************************/
_inl void TransformNode::SetScaleX( float val )
{
	Vector3D dir = GetDirX();
    if (dir.normalize() < c_SmallEpsilon) dir = Vector3D::oX;
	dir *= val;
	SetDirX( dir );
}

_inl void TransformNode::SetScaleY( float val )
{
	Vector3D dir = GetDirY();
    if (dir.normalize() < c_SmallEpsilon) dir = Vector3D::oY;
	dir *= val;
	SetDirY( dir );
}

_inl void TransformNode::SetScaleZ( float val )
{
	Vector3D dir = GetDirZ();
    if (dir.normalize() < c_SmallEpsilon) dir = Vector3D::oZ;
	dir *= val;
	SetDirZ( dir );
}

/*****************************************************************************/
/*	HudNode implementation
/*****************************************************************************/
_inl float	HudNode::GetPosX() const
{
	return pos.x;
}

_inl float	HudNode::GetPosY() const
{
	return pos.y;
}

_inl float	HudNode::GetPosZ() const
{
	return pos.z;
}

_inl void	HudNode::SetPosX( float val )
{
	pos.x = val;
}

_inl void	HudNode::SetPosY( float val )
{
	pos.y = val;
}

_inl void	HudNode::SetPosZ( float val )
{
	pos.z = val;
}

_inl float HudNode::GetWidth() const
{
	return width;
}

_inl float HudNode::GetHeight() const
{
	return height;
}

_inl void HudNode::SetWidth( float val )
{
	width = val;
}

_inl void HudNode::SetHeight( float val )
{
	height = val;
}

_inl float HudNode::GetScale() const
{
	return scale;
}

_inl void HudNode::SetScale( float val )
{
	scale = val;
}

