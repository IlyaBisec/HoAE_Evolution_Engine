/*****************************************************************************/
/*	File:	kPersistent.inl
/*	Author:	Ruslan Shestopalyuk
/*	Date:	23.05.2003
/*****************************************************************************/

/*****************************************************************************/
/*	stcio implementation
/*****************************************************************************/
_inl int stcio::read( BYTE* buf, int& val )
{
	val = *((int*)buf);
	return sizeof( int );
}

_inl int stcio::read( FILE* fp,  int& val )
{
	int res = fread( &val, sizeof( int ), 1, fp );
	return res == 1 ? sizeof( int ) : 0;
}

_inl int stcio::read( BYTE* buf, float& val )
{
	val = *((float*)buf);
	return sizeof( float );
}

_inl int stcio::read( FILE* fp,  float& val )
{
	int res = fread( &val, sizeof( float ), 1, fp );
	return res == 1 ? sizeof( float ) : 0;
}

_inl int stcio::read( BYTE* buf, DWORD& val )
{
	val = *((DWORD*)buf);
	return sizeof( DWORD );
}

_inl int stcio::read( FILE* fp,  DWORD& val )
{
	
	int res = fread( &val, sizeof( DWORD ), 1, fp );
	return res == 1 ? sizeof( DWORD ) : 0;
}

_inl int  stcio::read( BYTE* buf, char* str, int numCh )
{
	int nCh = *((int*)buf);
	buf += sizeof( nCh );
	assert( nCh < numCh );
	memcpy( str, buf, numCh + 1 );
	return numCh + 1 + sizeof( nCh );
}

_inl int  stcio::read( FILE* fp,  char* str, int numCh )
{
	int nCh;
	fread( &nCh, sizeof( nCh ), 1, fp ); 
	nCh++;
	fread( str, nCh + 1, 1, fp );
	assert( nCh < numCh );
	return nCh + 1 + sizeof( nCh );
}

_inl int  stcio::write( BYTE* buf, int& val )
{
	*((int*)buf) = val;
	return sizeof( int );
}

_inl int  stcio::write( FILE* fp,  int& val )
{
	fwrite( &val, sizeof( int ), 1, fp );
	return sizeof( int );
}

_inl int  stcio::write( BYTE* buf, float& val )
{
	*((float*)buf) = val;
	return sizeof( float );
}

_inl int  stcio::write( FILE* fp,  float& val )
{
	fwrite( &val, sizeof( float ), 1, fp );
	return sizeof( float );
}

_inl int  stcio::write( BYTE* buf, DWORD& val )
{
	*((DWORD*)buf) = val;
	return sizeof( DWORD );
}

_inl int  stcio::write( FILE* fp,  DWORD& val )
{
	fwrite( &val, sizeof( DWORD ), 1, fp );
	return sizeof( DWORD );
}

_inl int  stcio::write( BYTE* buf, char* str )
{
	int nCh = 0;
	while (*str) 
	{
		*(buf++) = *(str++);
		nCh++;
	}
	*buf = 0;
	return nCh;
}

_inl int  stcio::write( FILE* fp,  char* str )
{
	int nCh = strlen( str );
	fwrite( &nCh, 1, sizeof( int ), fp );
	return sizeof( int );
}

/*****************************************************************************/
/*	MagicWord implementation
/*****************************************************************************/
_inl MagicWord::MagicWord( const char* wstr )
{
	mb[0] = wstr[0];
	mb[1] = wstr[1];
	mb[2] = wstr[2];
	mb[3] = wstr[3];
}

_inl int MagicWord::write( BYTE* buf )
{
	buf[0] = mb[0];
	buf[1] = mb[1];
	buf[2] = mb[2];
	buf[3] = mb[3];

	return sizeof( mb );
}

_inl int MagicWord::write( FILE* fp )
{
	fwrite( &mb, 1, sizeof( mb ), fp );
	return sizeof( mb );
}

_inl int MagicWord::read( BYTE* buf )
{
	DWORD res = *((DWORD*)buf);
	if (res == *((DWORD*)&mb)) return sizeof( mb );
	return 0;
}

_inl int MagicWord::read( FILE* fp )
{
	DWORD res;
	int nIt = fread( &res, sizeof( res ), 1, fp );
	if (nIt == 0) return 0;
	if (res != *((DWORD*)&mb)) 
	{
		fseek( fp, -4, SEEK_CUR );
		return 0;
	}
	return sizeof( mb );
}
