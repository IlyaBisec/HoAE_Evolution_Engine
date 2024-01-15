/*****************************************************************************/
/*	File:	kIO.inl
/*	Desc:	
/*	Author:	Ruslan Shestopalyuk
/*	Date:	20.02.2003
/*****************************************************************************/

/*****************************************************************************/
/*	InStream implementation
/*****************************************************************************/
_inl InStream& InStream::operator >>( float& val )
{
	Read( &val, sizeof( float ) );
	return *this;
} // InStream::operator >>

_inl InStream& InStream::operator >>( int& val )
{
	Read( &val, sizeof( int ) );
	return *this;
} // InStream::operator >>

_inl InStream& InStream::operator >>( bool& val )
{
	unsigned char nVal = 0;
	Read( &nVal, sizeof( nVal ) );
	val = (nVal != 0);
	return *this;
} // InStream::operator >>

_inl InStream& InStream::operator >>( double& val )
{
	Read( &val, sizeof( double ) );
	return *this;
} // InStream::operator >>

_inl InStream& InStream::operator >>( DWORD& val )
{
	Read( &val, sizeof( DWORD ) );
	return *this;
} // InStream::operator >>

_inl InStream& InStream::operator >>( WORD& val )
{
	Read( &val, sizeof( WORD ) );
	return *this;
} // InStream::operator >>

_inl InStream& InStream:: operator >>( BYTE& val )
{
	Read( &val, sizeof( BYTE ) );
	return *this;
} // InStream::operator >>

_inl InStream::operator bool() const
{
	return !IsEndOfStream();
}

_inl int InStream::GetTotalBytesRead() const
{
	return bytesRead;
}

/*****************************************************************************/
/*	OutStream implementation
/*****************************************************************************/
_inl OutStream& OutStream::operator <<( float	val )
{
	Write( &val, sizeof( val ) );
	return *this;
} // OutStream::operator <<

_inl OutStream& OutStream::operator <<( int	val )
{
	Write( &val, sizeof( val ) );
	return *this;
} // OutStream::operator <<

_inl OutStream& OutStream::operator <<( bool val )
{
	unsigned char nVal = val ? 1 : 0;
	Write( &nVal, sizeof( nVal ) );
	return *this;
} // OutStream::operator <<

_inl OutStream& OutStream::operator <<( double val )
{
	Write( &val, sizeof( val ) );
	return *this;
} // OutStream::operator <<

_inl OutStream& OutStream::operator <<( DWORD val )
{
	Write( &val, sizeof( val ) );
	return *this;
} // OutStream::operator <<

_inl OutStream& OutStream::operator <<( WORD val )
{
	Write( &val, sizeof( val ) );
	return *this;
} // OutStream::operator <<

_inl OutStream& OutStream::operator <<( BYTE val )
{
	Write( &val, sizeof( val ) );
	return *this;
} // OutStream::operator <<

_inl OutStream& OutStream::operator <<( const char* val )
{
	DWORD nCh = val ? (DWORD)strlen( val ) : 0;
	operator<<( nCh );
	if (val && nCh > 0) Write( (void*)val, nCh );
	return *this;
} // OutStream::operator <<

/*****************************************************************************/
/*	FInStream implementation
/*****************************************************************************/
_inl bool FInStream::NoFile() const
{
	return (hFile == INVALID_HANDLE_VALUE);
}

/*****************************************************************************/
/*	FOutStream implementation
/*****************************************************************************/
_inl bool FOutStream::NoFile() const
{
	return (hFile == INVALID_HANDLE_VALUE);
}

/*****************************************************************************/
/*	MediaChunk implementation
/*****************************************************************************/
_inl MediaChunk::MediaChunk()
{
	memset( strMagic, 0, sizeof( strMagic ) );
} // MediaChunk::ReadHeader

_inl bool MediaChunk::ReadHeader( ResFile rf )
{
	assert( rf );
	//  magic word
	if (RBlockRead( rf, strMagic,	4 ) == 0) return false; 
	if (RBlockRead( rf, &size,		4 ) == 0) return false; 
	curPosInBlock	= 0;
	resFile			= rf;
	
	return true;
} // MediaChunk::ReadHeader
	
_inl DWORD MediaChunk::Skip()
{
	assert( resFile );
	int skipSize = GetBytesLeft();
	curPosInBlock = GetDataSize();
	RSeekFromCurrent( resFile, skipSize );
	return skipSize;
} // MediaChunk::Skip

_inl DWORD MediaChunk::Skip( int nBytes )
{
	assert( resFile && GetBytesLeft() >= nBytes);
	curPosInBlock += nBytes;
	RSeekFromCurrent( resFile, nBytes );
	return nBytes;
} // MediaChunk::Skip

_inl const char* MediaChunk::GetMagic() const
{
	return strMagic;
} // MediaChunk::GetMagic

_inl DWORD MediaChunk::GetMagicDW() const
{
	return *((DWORD*)strMagic);
} // MediaChunk::GetMagicDW

_inl int MediaChunk::GetFullSize() const
{
	return size;
} // MediaChunk::GetFullSize

_inl int MediaChunk::GetDataSize() const
{
	return size - 8;
} // MediaChunk::GetSize

_inl int MediaChunk::GetBytesLeft() const
{
	return size - 8 - curPosInBlock;
} // MediaChunk::GetSize

_inl DWORD MediaChunk::Read( void* buf, int sizeBytes )
{
	assert( buf && sizeBytes <= GetDataSize() - curPosInBlock );
	
	curPosInBlock += sizeBytes;
	return RBlockRead( resFile, buf, sizeBytes );
} // MediaChunk::Read

/*****************************************************************************/
/*	MediaFileLoader implementaiton
/*****************************************************************************/
_inl MediaFileLoader::MediaFileLoader( ResFile* rf )
{
	assert( rf );
	resFile			= rf;
	errorStatus		= mfleNoError;
	fileSize		= RFileSize( resFile );
	curFilePos		= 0;
} // MediaFileLoader::MediaFileLoader

_inl MediaFileLoader::MediaFileLoader( const char* fName )
{
	resFile		= RReset( fName );
	errorStatus	= mfleNoError;
	if (resFile == INVALID_HANDLE_VALUE)
	{
		errorStatus	= mfleFileOpenError;
		return; 
	}
	fileSize		= RFileSize( resFile );
	curFilePos		= 0;
} // MediaFileLoader::MediaFileLoader

_inl MediaFileLoader::~MediaFileLoader()
{
	if (resFile != INVALID_HANDLE_VALUE) RClose( resFile );
}

_inl MFLError MediaFileLoader::Error()
{
	return errorStatus;
} // MediaFileLoader::Error

_inl bool	MediaFileLoader::NextChunk()
{
	if (curFilePos >= fileSize) return false;
	assert( resFile );
	int nReadBytes = curChunk.ReadHeader( resFile );
	if (nReadBytes == 0)
	{
		errorStatus = mfleBadChunk;
		return false;
	}	
	curFilePos += nReadBytes;
	return true;
} // MediaFileLoader::NextChunk

_inl DWORD MediaFileLoader::Read( void* buf, int sizeBytes )
{
	curFilePos += sizeBytes;
	return curChunk.Read( buf, sizeBytes );
}

_inl DWORD MediaFileLoader::CurChunkSkip()
{
	curFilePos += curChunk.Skip();
	return curFilePos;
} // MediaFileLoader::CurChunkSkip

_inl DWORD MediaFileLoader::CurChunkSkip( DWORD nBytes )
{
	curFilePos += curChunk.Skip( nBytes );
	return curFilePos;
} // MediaFileLoader::CurChunkSkip


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


