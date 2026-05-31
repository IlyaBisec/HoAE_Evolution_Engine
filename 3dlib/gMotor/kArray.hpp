/*****************************************************************************/
/*	File:	kArray.hpp
/*	Desc:	Array-like templated containers
/*	Author:	Ruslan Shestopalyuk
/*	Date:	30.01.2003
/*	Rmrk:	Style is borrowed from STL to make it less confusing
/*****************************************************************************/
#ifndef __KARRAY_HPP__
#define __KARRAY_HPP__
#pragma once

BEGIN_NAMESPACE( c2 )

/*****************************************************************************/
/*	Class:	array
/*	Desc:	Linear array with self-adjusting
/*****************************************************************************/
template <class TElem>
class array
{
public:
	array() : buf(0), nElem(0), maxElem(0), ownBuf(true) {}
	array( int nEl ) : buf(0), nElem(0), maxElem(0), ownBuf(true)
	{
		reserve( nEl );
	}

	array( const array<TElem>& arr )
	{
		buf = NULL; nElem = 0; maxElem = 0;
		resize( arr.nElem );
		memcpy( buf, arr.buf, nElem * sizeof(TElem) );
	}

	void copy( const array<TElem>& arr )
	{
		resize( arr.nElem );
		memcpy( buf, arr.buf, nElem * sizeof(TElem) );
	}

	const array<TElem>& operator=( const array<TElem>& arr )
	{
		resize( arr.nElem );
		memcpy( buf, arr.buf, nElem * sizeof(TElem) );
		return (*this);	
	}

	~array()
	{
		freeBuf();
		buf		= 0;
		nElem	= 0;
		maxElem = 0;
	}
	
	void freeBuf()
	{
		if (ownBuf) delete []buf;
		buf = NULL;
	}

	void	reserve( int sz, TElem* _newBuf = NULL )
	{
		if (sz == maxElem) return;
		TElem*	newBuf = NULL;
		if (_newBuf) 
		{
			newBuf = _newBuf;
		}
		else
		{
			newBuf = new TElem[sz];
		}

		nElem	= sz < nElem ? sz : nElem; 
		if (nElem > 0) memcpy( newBuf, buf, nElem * sizeof(TElem) );
		maxElem = sz; 
		freeBuf();
		buf = newBuf;
		if (_newBuf) 
		{
			ownBuf = false;
		}
		else
		{
			ownBuf = true;
		}

	}

	void	clear()
	{
		nElem = 0;
	}

	void	resize( int newSize, TElem* newBuf = NULL )
	{
		if (newSize <= maxElem)
		{
			nElem = newSize;
			return;
		}
		freeBuf();
		nElem	= 0;
		maxElem = 0;
		buf		= 0;
		reserve( newSize, newBuf );
		nElem = newSize;
	}

	TElem&	operator[]( int idx )
	{
		assert( buf && idx >= 0 && idx < nElem );
		return buf[idx];
	}

	const TElem&	operator[]( int idx ) const
	{
		assert( buf && idx >= 0 && idx < nElem );
		return buf[idx];
	}

	const TElem& c_elem( int idx ) const
	{
		assert( buf && idx >= 0 && idx < nElem );
		return buf[idx];
	}

	int size_bytes() const
	{
		return maxElem * sizeof( TElem ) + sizeof( *this );
	}

	int	size() const { return nElem; }

	bool erase( int firstEl, int numEl = 1 )
	{
		if (firstEl < 0) firstEl = 0;
		if (firstEl + numEl > nElem) numEl = nElem - firstEl;
		if (numEl == 0) return false;
		for (int i = firstEl + numEl; i < nElem; i++)
		{
			buf[i - numEl] = buf[i];
		}
		nElem -= numEl;
		return true;
	}

	bool insert(  const TElem& el, int position )
	{
		if (position < 0) position = 0;
		if (position > nElem) position = nElem;

		if (nElem >= maxElem ) 
		{
			if (maxElem > 1) 
			{
				reserve( maxElem + maxElem / 2 );
			}
			else 
			{
				reserve( 2 );
			}
		}
		
		for (int i = nElem - 1; i >= position; i--)
		{
			buf[i + 1] = buf[i];
		}
		buf[position] = el;
		nElem++;
		return true;
	} // insert

	bool push_back( const TElem& el )
	{
		if (nElem >= maxElem ) 
		{
			if (maxElem > 1) 
			{
				reserve( maxElem + maxElem / 2 );
			}
			else 
			{
				reserve( 2 );
			}
		}
		buf[nElem++] = el;
		return true;
	}

	TElem& top()
	{
		assert( nElem > 0 );
		return buf[nElem - 1];
	}

	TElem pop_back()
	{
		assert( nElem > 0 );
		return buf[nElem--];
	}

	int capacity() const
	{
		return maxElem;
	}

private:
	TElem*		buf;
	int			nElem;
	int			maxElem;
	bool		ownBuf;

}; // class array

/*****************************************************************************/
/*	Class:	static_ptr_array
/*	Desc:	Static linear pointer array 
/*****************************************************************************/
template <class TElem, int MaxSz>
class static_ptr_array
{
public:
	static_ptr_array() : nElem(0) {}

	static_ptr_array( const static_ptr_array<TElem, MaxSz>& arr )
	{
		nElem = arr.nElem;
		memcpy( buf, arr.buf, nElem*sizeof(TElem*) );
	}

	~static_ptr_array()
	{
		nElem	= 0;
	}

	TElem*	operator[]( int idx )
	{
		massert( idx >= 0 && idx < nElem, "static_ptr_array index is out of bounds." );
		return buf[idx];
	}

	const TElem*	operator[]( int idx ) const
	{
		massert( idx >= 0 && idx < nElem, "static_ptr_array index is out of bounds." );
		return buf[idx];
	}

	const TElem* c_elem( int idx ) const
	{
		massert( idx >= 0 && idx < nElem, "static_ptr_array index is out of bounds." );
		return buf[idx];
	}

	int size_bytes() const
	{
		return MaxSz * sizeof( TElem* ) + sizeof( *this );
	}

	int	size() const { return nElem; }

	void push_back( TElem* el )
	{
		assert(nElem < MaxSz); 
		buf[nElem++] = el;
	}

	void set_size( int sz )
	{
		assert( sz > 0 && sz < MaxSz );
		nElem = sz;
	}

	void erase( int elIdx ) 
	{
		assert( elIdx >= 0 && elIdx < nElem );
		buf[elIdx] = NULL;
	}

	int capacity() const
	{
		return MaxSz;
	}

	void clear()
	{
		nElem = 0;
	}

	void dump( FILE* fp )
	{
		if (!fp) return;
		fprintf( fp, "\nDumping array(%X), nElem:%d, MaxSz:%d\n", this, nElem, MaxSz );
		for (int i = 0; i < nElem; i++)
		{
			fprintf( fp, "%d\n", buf[i] );
		}
	}

private:
	TElem*		buf[MaxSz];
	int			nElem;
}; // class static_ptr_array

/*****************************************************************************/
/*	Class:	circular_queue
/*****************************************************************************/
template <class TElem, int MaxSz>
class circular_queue
{
protected: 
	int	decr( int& idx ) { if (--idx < 0) idx = MaxSz - 1; return idx; }
	int	incr( int& idx ) { if (++idx >= MaxSz) idx = 0; return idx; }

public:
	circular_queue() : m_Top(-1), m_NumEl(0){}

	void push( const TElem& el )
	{ 
		incr( m_Top );
		m_Elem[m_Top] = el;
		m_NumEl = m_NumEl >= MaxSz ? MaxSz : m_NumEl + 1;
	}

	TElem&			pop()
	{
		assert( m_NumEl != 0 );
		int oldTop = m_Top;
		decr( m_Top );
		m_NumEl--;
		return m_Elem[oldTop];
	}

	const TElem&	top() const { return m_Elem[m_Top]; }
	TElem&			top() { return m_Elem[m_Top]; }

	const TElem&	bottom() const 
	{ 
		assert( m_NumEl != 0 );
		int idx = m_Top - m_NumEl + 1; 
		if (idx < 0) idx += MaxSz;
		return m_Elem[idx]; 
	}

	TElem&			bottom() 
	{
		assert( m_NumEl != 0 );
		int idx = m_Top - m_NumEl + 1; 
		if (idx < 0) idx += MaxSz;
		return m_Elem[idx]; 
	}

	int				numEl() const { return m_NumEl; }

	void clear()
	{
		m_NumEl = 0;
		m_Top = 0;
	}

	void dump( FILE* fp )
	{
		if (!fp) return;
		fprintf( fp, "\nDumping circular_queue(%X), Top:%d, MaxSz:%d, NumEl:%d\n", 
			this, m_Top, MaxSz, m_NumEl );
		for (int i = 0; i < MaxSz; i++)
		{
			fprintf( fp, "buf[%3d] = %d\n", i, m_Elem[i] );
		}
	}

private:
	TElem		m_Elem[MaxSz];

	int			m_Top;		
	int			m_Bottom;
	int			m_NumEl;
	int			m_MaxSize;

}; // class circular_queue

END_NAMESPACE( c2 )

#endif // __KARRAY_HPP__