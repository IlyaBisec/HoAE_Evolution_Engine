/*****************************************************************************/
/*	File:	kstring.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	29.01.2003
/*****************************************************************************/

/*****************************************************************************/
/*	string implementation
/*****************************************************************************/
_inl string::string() : buf(NULL), len(0)
{
}

_inl string::string( const char* val ) 
{ 
	buf = NULL;
	len = 0;
	set( val ); 
}

_inl string::string( string& orig	)
{
	buf = NULL;
	len = 0;
	set( orig.buf );
}

_inl const string& string::operator=( const string& str	)
{
	set( str.buf );
	return *this;
}

_inl void string::copy( const string& str )
{
	set( str.buf );
}

_inl const string& string::operator=( const char* str )
{
	set( str );
	return *this;
}
	
_inl string::~string() 
{ 
	delete[] buf; 
	buf = NULL;
}

_inl void string::set( const char* val )
{
	if (val == NULL)
	{
		set( "", 0 );
		return;
	}

	set( val, strlen( val ) );
}

_inl void string::set( const char* val, int nsym )
{
	if (val == NULL)
	{
		set( "" );
		return;
	}

	int nlen = nsym + 1;
	if (nlen != len)
	{
		delete []buf;
		buf = new char[nlen];
	}
	memcpy( buf, val, nlen );
	buf[nlen - 1] = 0;
	len = nlen;
}

_inl bool string::operator ==( const string& str ) const
{
	return equal( str );
}

_inl bool string::equal( const string& str ) const
{
	if (buf == 0)
	{
		if (str.buf == 0) return true;
		return false;
	}
	return !strcmp( buf, str.buf );
}

_inl bool string::operator ==( const char* str ) const
{
	if (buf == NULL)
	{
		if (str == NULL) return true;
		return false;
	}
	return !strcmp( buf, str );
}

_inl void string::toLower()
{
	_strlwr( buf );
}

_inl char& string::operator[]( int idx )
{
	assert( idx >= 0 && idx < len );
	return buf[idx];
}

_inl void string::operator +=( const char* catStr )
{
	int catLen = strlen( catStr );
	int oldLen = len;
	resize( len + catLen );
	memcpy( buf + oldLen - 1, catStr, catLen + 1 );
} // string::operator +=

_inl void string::resize( int newSz )
{
	char* newBuf = new char[newSz];
	if (buf) memcpy( newBuf, buf, len );
	len = newSz;
	delete []buf;
	buf = newBuf;
} // string::resize

_inl int string::size() const 
{ 
	return len; 
}

_inl const char* string::c_str() const 
{
	return buf; 
} 

_inl unsigned int string::hash() const
{
	char* s = buf;
	unsigned int h = 0;
	while (*s)
	{
		h = (h * 729 + (*s) * 37) ^ (h >> 1);
		s++;
	}
	return h;
} // string::hash

_inl int string::write( BYTE* sbuf, int maxSize ) const
{
	DWORD nB = len;
	assert( nB + sizeof( nB ) < maxSize );
	*((DWORD*)sbuf) = nB;
	sbuf += sizeof( nB );
	memcpy( sbuf, buf, nB );
	sbuf += nB;
	return nB + sizeof(DWORD);
} // string::write

_inl int string::read( BYTE* sbuf, int maxSize )
{
	assert( maxSize >= 4 );
	DWORD nBytes = *((DWORD*)sbuf);
	sbuf += sizeof( nBytes );
	resize( nBytes );
	memcpy( buf, sbuf, nBytes );
	sbuf += nBytes;
	len = nBytes;
	sbuf++;
	return nBytes + sizeof(DWORD);
} // string::read

_inl int string::read( FILE* fp )
{
	int nB;
	stcio::read( fp, nB );
	resize( nB );
	fread( buf, nB, 1, fp );
	len = nB;
	return nB + sizeof( nB );
} // string::read

_inl int string::write( FILE* fp ) const
{
	int nB = len;
	stcio::write( fp, nB );
	fwrite( buf, nB, 1, fp );
	return nB + sizeof( nB );	
} // string::write



