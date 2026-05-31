/*****************************************************************
/*  File:   mMath3D.hpp
/*  Author: Silver, Copyright (C) GSC Game World                 
/*****************************************************************/

/*****************************************************************
/*	Matrix2D implementation
/*****************************************************************/
template <class T> void 
Matrix2D<T>::FromAngle( float ang )
{
	float fCos = cos( ang );
	float fSin = sin( ang );
	e00 = fCos;
	e01 = fSin;
	e10 = -fSin;
	e11 = fCos;
}

/*****************************************************************
/*	Vector2D implementation
/*****************************************************************/
template <class T>
Vector2D<T>::Vector2D() : x(0.0f), y(0.0f)
{
}

template <class T> _inl bool
Vector2D<T>::inAAQuad( T _x, T _y, T _side ) const
{
	return (x >= _x) && (x <= _x + _side) && (y >= _y) && (y <= _y + _side);
}

template <class T>
Vector2D<T>::Vector2D( const T _x, const T _y )
{
	x = _x;
	y = _y;
}

template <class T> _inl void 
Vector2D<T>::sub( const Vector2D<T>& v )
{
	x -= v.x;
	y -= v.y;
}

template <class T> _inl float 
Vector2D<T>::dot( const Vector2D<T>& v ) const
{
	return x * v.x + y * v.y;
}

template <class T> _inl T 
Vector2D<T>::triArea( const Vector2D<T>& v )
{
	return v.x * y - v.y * x;
}


template <class T> void 
Vector2D<T>::Dump()
{
	Log::Message( "x: %f. y: %f.", x, y );
}

template <class T> _inl void 
Vector2D<T>::set( T _x, T _y )
{
	x = _x;
	y = _y;
}

template <class T> _inl void 
Vector2D<T>::sub( const Vector2D& v1, const Vector2D& v2 )
{
	x = v1.x - v2.x;
	y = v1.y - v2.y;
}

template <class T> _inl void 
Vector2D<T>::clamp( T minX, T minY, T maxX, T maxY )
{
	if (x < minX) x = minX;
	if (y < minY) y = minY;

	if (x > maxX) x = maxX;
	if (y > maxY) y = maxY;
}

template <class T> _inl void 
Vector2D<T>::copy( const Vector2D& orig )
{
	x = orig.x;
	y = orig.y;
}

template <class T> _inl const Vector2D<T>& 
Vector2D<T>::operator +=( const Vector2D<T>& vec )
{
	x += vec.x;
	y += vec.y;
	return *this;
}

template <class T> _inl const Vector2D<T>& 
Vector2D<T>::operator -=( const Vector2D<T>& vec )
{
	x -= vec.x;
	y -= vec.y;
	return *this;
}

template <class T> _inl const Vector2D<T>& 
Vector2D<T>::operator *=( const T val )
{
	x *= val;
	y *= val;
	return *this;
}

template <class T> _inl const Vector2D<T>& 
Vector2D<T>::operator /=( const T val )
{
	x /= val;
	y /= val;
	return *this;
}

template <class T> _inl T 
Vector2D<T>::norm() const
{
	return static_cast<T>( sqrt( x * x + y * y ) );	
}

template <class T> _inl T
Vector2D<T>::norm2() const
{
	return x * x + y * y;
}

template <class T> _inl T
Vector2D<T>::dist2( Vector2D<T> r ) const
{
	return (x - r.x) * (x - r.x) + (y - r.y) * (y - r.y);
}

template <class T> _inl void 
Vector2D<T>::normalize()
{
	float len = static_cast<T>( sqrt( x * x + y * y ) );
	x /= len;
	y /= len;
}

template <class T> _inl const Vector2D<T>& 
Vector2D<T>::operator *=( const Matrix2D<T>& m )
{
	T tx = x * m.e00 + y * m.e10;
	T ty = x * m.e01 + y * m.e11;
	x = tx; y = ty;
	return *this;
}


