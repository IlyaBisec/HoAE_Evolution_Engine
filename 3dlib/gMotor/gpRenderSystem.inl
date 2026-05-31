/*****************************************************************
/*  File:   gpRenderSystem.inl                                   *
/*  Author: Silver, Copyright (C) GSC Game World                 *
/*  Date:   Mar 2002                                             *
/*****************************************************************/

/*****************************************************************************/
/*	ViewPort implementation
/*****************************************************************************/
_inl bool ViewPort::ClipHLine( int& px1, int& px2, int py ) const
{
	if (py < y || py >= y + height) return false; 
	px1 = px1 < x ? x : px1;
	px2 = px2 < x ? x : px2;
	
	int vpRight  = x + width - 1;

	px1 = px1 > vpRight ? vpRight : px1;
	px2 = px2 > vpRight ? vpRight : px2;
	return (px1 != px2);
} // ViewPort::ClipHLine

_inl bool ViewPort::ClipVLine( int px, int& py1, int& py2 ) const
{
	if (px < x || px >= x + width) return false;
	py1 = py1 < y ? y : py1;
	py2 = py2 < y ? y : py2;
	
	int vpBottom  = y + height - 1;

	py1 = py1 > vpBottom ? vpBottom : py1;
	py2 = py2 > vpBottom ? vpBottom : py2;
	return (py1 != py2);
} // ViewPort::ClipVLine

_inl bool ViewPort::ClipRect( int& px, int& py, int& pw, int& ph ) const
{
	int px1 = px + pw;
	int py1 = py + ph;

	if (px < int( x ))
	{
		px = x;
	}

	if (py < int( y ))
	{
		py = y;
	}

	int vpRight  = x + width;
	int vpBottom = y + height;
	if (px1 > vpRight)  
	{
		px1 = vpRight;
	}

	if (py1 > vpBottom) 
	{
		py1 = vpBottom;
	}

	pw = px1 - px;
	ph = py1 - py;

	if (pw < 0) pw = 0;
	if (ph < 0) ph = 0;

	return ((pw > 0) && (ph > 0));
} // ViewPort::ClipRect

_inl bool ViewPort::IsPtInside( int px, int py ) const
{
	return (px >= x && px < x + width && py >= y && py < y + height);
} // ViewPort::IsPtInside

_inl bool ViewPort::IsRectInside( int px, int py, int pw, int ph ) const
{
	return (px >= x && px + pw < x + width && 
			py >= y && py + ph < y + height);
} // ViewPort::IsRectInside

_inl bool ViewPort::ClipY( int py ) const
{
	return (py < y) || ( py >= y + height ); 
} // ViewPort::ClipY

_inl DWORD ViewPort::GetRight() const
{
	return x + width - 1;
} // ViewPort::GetRight

_inl DWORD ViewPort::GetBottom() const
{
	return y + height - 1;
} // ViewPort::GetBottom




