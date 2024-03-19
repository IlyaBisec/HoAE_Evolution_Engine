/*****************************************************************************/
/*	File:	sgController.hpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	22.04.2003
/*****************************************************************************/
#include "mSplines.h"

BEGIN_NAMESPACE( sg )
/*****************************************************************************/
/*	AnimationCurve implementation
/*****************************************************************************/
template <class TVal>
OutStream& operator <<( OutStream& os, const AnimationCurve<TVal>& curve )
{
	curve.Serialize( os );
	return os;
}

template <class TVal>
InStream& operator >>( InStream& is, AnimationCurve<TVal>& curve )
{
	curve.Unserialize( is );
	return is;
}


template <class TVal>
void AnimationCurve<TVal>::Serialize( OutStream& os ) const
{
	os.Write( (void*)&type, sizeof( type ) );
	os.Write( (void*)&defVal, sizeof( defVal ) );
	os << nodes.size();
	for (int i = 0; i < nodes.size(); i++)
	{
		os << nodes[i];
	}
}

template <class TVal>
void AnimationCurve<TVal>::Unserialize( InStream& is ) 
{
	int size;
	is.Read( &type, sizeof( type ) );
	is.Read( &defVal, sizeof( defVal ) );
	is >> size;
	nodes.resize( size );
	for (int i = 0; i < size; i++)
	{
		is >> nodes[i];
	}
}

template <class TVal>
_inl void AnimationCurve<TVal>::AddKey( float time, const TVal& value )
{
	nodes.push_back( Keyframe<TVal>( time, value ) );
}

template <class TVal>
_inl void AnimationCurve<TVal>::DeleteKey( int idx )
{
	if (idx < 0 || idx >= nodes.size()) return;
	nodes.erase( idx );
}

template <class TVal>
_inl const int AnimationCurve<TVal>::GetNKeys() const
{
	return nodes.size();
}

template <class TVal>
_inl void AnimationCurve<TVal>::SetKeyValue( int keyIdx, const TVal& value )
{
	nodes[keyIdx].SetValue( value );
}

template <class TVal>
_inl void AnimationCurve<TVal>::SetKeyTime( int keyIdx, float _time )
{
	nodes[keyIdx].SetTime( _time );
}

template <class TVal>
_inl float AnimationCurve<TVal>::GetMaxTime() const
{
	if (nodes.size() == 0) return 0.0f;
	return nodes[nodes.size() - 1].GetTime();
}

template <class TVal>
_inl float AnimationCurve<TVal>::GetMinTime() const
{
	if (nodes.size() == 0) return 0.0f;
	return nodes[0].GetTime();
}

template <class TVal>
_inl const Keyframe<TVal>&	AnimationCurve<TVal>::GetKey( int idx ) const
{
	assert( idx >= 0 && idx < nodes.size() );
	return nodes[idx];
}

template <class TVal>
_inl TVal AnimationCurve<TVal>::GetValue( float time, int& kfIndex ) const
{
	return GetValue( time );
}

template <class TVal>
_inl TVal AnimationCurve<TVal>::GetValue( float time ) const
{
	if (nodes.size() == 0) return defVal;
	
	int kfIdx1 = LocateTime( time );
	if (kfIdx1 < 0) 
	{
		return nodes[0].GetValue();
	}

	float t1 = nodes[kfIdx1].GetTime();
	const TVal&  v1 = nodes[kfIdx1].GetValue();
	
	int kfIdx2 = kfIdx1 + 1;
	if (kfIdx2 >= nodes.size()) return v1;
	
	float t2 = nodes[kfIdx2].GetTime();
	const TVal&  v2 = nodes[kfIdx2].GetValue();
	
	if (time == t1) return v1;
	if (time == t2) return v2;

	return LinearInterpolate( time, t1, v1, t2, v2 );
} // AnimationCurve::GetValue

//  do binary search to locate keyframes
template <class TVal>
_inl int AnimationCurve<TVal>::LocateTime( float time ) const
{
	return BinarySearch( time, &nodes[0], nodes.size() );
}

template <class TVal>
void AnimationCurve<TVal>::Rescale( float timeScale )
{
	for (int i = 0; i < GetNKeys(); i++)
	{
		SetKeyTime( i, GetKey( i ).GetTime() * timeScale );
	}
}

/*****************************************************************************/
/*	KeyFrame implementation
/*****************************************************************************/
template <class TVal>
OutStream& operator <<( OutStream& os, const Keyframe<TVal>& kf )
{
	kf.Serialize( os );
	return os;
}

template <class TVal> 
InStream& operator >>( InStream& is, Keyframe<TVal>& kf )
{
	kf.Unserialize( is );
	return is;
}

template <class TVal> 
_inl void Keyframe<TVal>::Serialize	( OutStream& os ) const
{
	os << time;
	os.Write( (void*)&val, sizeof( val ) );
}

template <class TVal> 
_inl void Keyframe<TVal>::Unserialize ( InStream& is )
{
	is >> time;
	is.Read( &val, sizeof( val ) );
}

END_NAMESPACE( sg )
