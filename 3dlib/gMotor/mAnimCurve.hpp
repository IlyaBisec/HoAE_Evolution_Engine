/*****************************************************************************/
/*	File:	mAnimCurve.hpp
/*	Author:	Ruslan Shestopalyuk
/*	Date:	20.11.2003
/*****************************************************************************/
#ifndef __MANIMCURVE_H__
#define __MANIMCURVE_H__

#include "kIOHelpers.h"
/*****************************************************************************/
/*	Class:	AnimationCurve
/*	Desc:	Keyframed value sequence
/*****************************************************************************/
template <class TVal> class AnimationCurve
{
public:
	AnimationCurve() {}

	void AddKey( float time, const TVal& value )
	{
		int pos = (m_Values.size() > 0) ? LocateTime( time ) : -1;
		if (pos >= m_Values.size() - 1 || pos == -1)
		{
			m_Values.push_back( value );
			m_Times.push_back( time );	
		}
		else
		{
			m_Values.insert( m_Values.begin() + pos + 1, value );
			m_Times.insert( m_Times.begin() + pos + 1, time );
		}
	}
	
	void DeleteKey( int idx )
	{
		assert( m_Times.size() == m_Values.size() );
		if (idx < 0 || idx >= m_Values.size()) return;
		m_Values.erase( m_Values.begin() + idx );
		m_Times.erase( m_Times.begin() + idx );
	}

	TVal* KeyValue( float time )
	{
		int pos = LocateTime( time );
		if (pos < m_Values.size() - 1)
		{
			const float dr = m_Times[pos + 1] - time;
			const float dl = time - m_Times[pos];
			if (dr < dl) pos++;
		}

		if (pos == -1) return NULL;
		return &m_Values[pos];
	}

	const int GetNKeys() const 
	{
		assert( m_Times.size() == m_Values.size() );
		return m_Times.size();
	}
	
	virtual TVal GetValue( float time ) const
	{
		int idx = BinarySearch( time, &m_Times[0], m_Times.size() );
		return m_Values[idx];
	}
	
	float GetMaxTime() const
	{
		if (m_Times.size() == 0) return 0.0f;
		return m_Times[m_Times.size() - 1];
	}
	
	float GetMinTime() const
	{
		if (m_Times.size() == 0) return 0.0f;
		return m_Times[0];
	}

	void SetDefaultValue( const TVal& val ) { m_DefaultValue = val; }
	TVal GetDefaultValue() const { return m_DefaultValue; }

	void SetKeyValue( int keyIdx, const TVal& value )
	{
		assert( keyIdx >= 0 && keyIdx < GetNKeys() );
		m_Values[keyIdx] = value;
	}

	void SetKeyTime( int keyIdx, float time )
	{
		assert( keyIdx >= 0 && keyIdx < GetNKeys() );
		m_Times[keyIdx] = time;
	}	

	const TVal&	GetKeyValue( int keyIdx ) const
	{
		assert( keyIdx >= 0 && keyIdx < GetNKeys() );
		return m_Values[keyIdx];
	}

	float GetKeyTime( int keyIdx ) const
	{
		assert( keyIdx >= 0 && keyIdx < GetNKeys() );
		return m_Times[keyIdx];
	}

	virtual bool IsConstant( float tolerance = 0.0f ) const { return false; }
	void Rescale( float timeScale )
	{
		for (int i = 0; i < GetNKeys(); i++) 
			SetKeyTime( i, GetKey( i ).GetTime() * timeScale );
	}

	void Serialize	( OutStream& os ) const
	{
		int nKeys = GetNKeys();
		os.Write( &m_DefaultValue, sizeof( m_DefaultValue ) );
		RawWriteArray( os, m_Times	);
		RawWriteArray( os, m_Values );
	}

	void Unserialize ( InStream& is )
	{
		is.Read( &m_DefaultValue, sizeof( m_DefaultValue ) );
		RawReadArray( is, m_Times );
		RawReadArray( is, m_Values );
	}

	void Normalize()
	{
		int nKeys = GetNKeys();
		for (int i = 0; i < nKeys; i++)
		{
			m_Values[i].normalize();
		}
		m_DefaultValue.normalize();
	}

    void Reset()
    {
        m_Times.clear();
        m_Values.clear();
    }

protected:
	int LocateTime	( float time ) const
	{
		return BinarySearch( time, &m_Times[0], m_Times.size() );
	}

	std::vector<float>			m_Times;		//  key times array
	std::vector<TVal>			m_Values;		//  key values array

	TVal						m_DefaultValue;	//  value when there is 0 keyframes
}; // class AnimationCurve


#endif  // __MANIMCURVE_H__
