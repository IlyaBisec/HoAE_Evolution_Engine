/*****************************************************************************/
/*    File:    kIO.cpp
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    20.02.2003
/*****************************************************************************/
#include "stdafx.h"
#include <malloc.h>
#include "kIO.h"
#include "kDirIterator.h"

#ifndef _INLINES
#include "kIO.inl"
#endif // !_INLINES
#include "IResourceManager.h"

BYTE* LoadFile( const char* path, DWORD& bufLen )
{
    HANDLE hFile = ::CreateFile( path, GENERIC_READ, FILE_SHARE_READ, NULL,OPEN_EXISTING, 
                                    FILE_FLAG_SEQUENTIAL_SCAN, NULL );
    if (hFile == INVALID_HANDLE_VALUE)
    {
        bufLen = 0;
        return NULL;
    }
    
    bufLen = ::GetFileSize( hFile, NULL );

    BYTE* pBuf = new BYTE[bufLen];

    DWORD readBytes = 0;
    BOOL res = ReadFile( hFile, pBuf, bufLen, &readBytes, NULL );

    if (res == 0 || readBytes == 0)
    {
        delete []pBuf;
        bufLen = 0;
        pBuf = NULL;
    }

    CloseHandle( hFile );
    return pBuf;
} // LoadFile

void EnsureLastSlash( char* path )
{
    int len = strlen( path );
    if (len == 0 || (path[len - 1] != '\\' && path[len - 1] != '/')) strcat( path, "\\" );
} // EnsureLastSlash

bool LocateFile( const char* fname, char* root )
{
    char path        [_MAX_PATH ];
    EnsureLastSlash( root );
    strcpy( path, root );
    strcat( path, fname );
    if (FileExists( path )) return true;

    char drive        [_MAX_DRIVE];
    char directory    [_MAX_DIR  ];
    char file        [_MAX_PATH ];
    char ext        [_MAX_EXT  ];

    _splitpath( fname, drive, directory, file, ext );

    DirIterator it( root );
    while (it)
    {
        if (it.IsDirectory())
        {
            strcpy( path, it.GetFullFilePath() );
            if (LocateFile( fname, path ))
            {
                strcpy( root, path );
                return true;
            }
        }
        ++it;
    }
    return false;
} // LocateFile

bool FileExists( const char* fname )
{
    if (!fname || fname[0] == 0) return false;
    FILE* fp = fopen( fname, "rb" );
    if (!fp) return false;
    fclose( fp );
    return true;
} // FileExists

bool DirectoryExists( const char* fname )
{
    if (!fname || fname[0] == 0) return false;
    char cwd[_MAX_PATH];
    getcwd( cwd, _MAX_PATH );
    if (_chdir( fname ) != 0)
    {
        return false;
    }
    _chdir( cwd );
    return true;
} // DirectoryExists

/*****************************************************************************/
/*    StackBuffer implementation
/*****************************************************************************/
StackBuffer::StackBuffer( int size ) : m_Size(size), m_Data(NULL) 
{ 
    m_Data = (BYTE*)_alloca( size ); 
}

/*****************************************************************************/
/*    InStream implementation
/*****************************************************************************/
InStream::InStream() 
{
    bytesRead        = 0;
} // InStream::InStream

InStream::~InStream()
{
}

/*****************************************************************************/
/*    FInStream implementation
/*****************************************************************************/
FInStream::FInStream()
{
    hFile        = INVALID_HANDLE_VALUE;
    fileSize    = 0;
} // FInStream::FInStream

FInStream::FInStream( const char* fname )
{
    hFile = INVALID_HANDLE_VALUE;
    OpenFile( fname );
} // FInStream::FInStream

FInStream::~FInStream()
{
    Close();
} // FInStream::~FInStream

DWORD FInStream::OnRead( void* buf, DWORD nBytes )
{
    if (hFile == INVALID_HANDLE_VALUE) return false;
    DWORD readBytes;
    BOOL res = ReadFile( hFile, buf, nBytes, &readBytes, NULL );
    if (res == FALSE)
    {
        DWORD err = GetLastError();
        return 0;
    }
    return readBytes;
} // FInStream::OnRead

void FInStream::OnSkip( DWORD nBytes ) 
{ 
    if (hFile == INVALID_HANDLE_VALUE) return;
    BOOL res = SetFilePointer( hFile, nBytes, 0, FILE_CURRENT );
    if (res == FALSE)
    {
        DWORD err = GetLastError();
        return;
    }
} // FInStream::OnSkip

bool FInStream::IsEndOfStream() const
{
    return (GetTotalBytesRead() >= fileSize);
} // FInStream::IsEndOfStream

bool FInStream::OpenFile( const char* fname )
{
    hFile = CreateFile( fname, 
        GENERIC_READ, 
        FILE_SHARE_READ, 
        NULL,
        OPEN_EXISTING, 
        FILE_FLAG_SEQUENTIAL_SCAN, 
        NULL );

    if (hFile == INVALID_HANDLE_VALUE)
    {
        fileSize = 0;
        return false;
    }
    else
    {
        fileSize = ::GetFileSize( hFile, NULL );
        return true;
    }
} // FInStream::OpenFile

bool FInStream::Close()
{
    if (hFile == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    BOOL res = CloseHandle( hFile );
    if (res == FALSE) 
    {
        return false;
    }
    hFile = INVALID_HANDLE_VALUE;
    return true;
} // FInStream::Close

bool FInStream::Reset()
{
    if (hFile == INVALID_HANDLE_VALUE) return false;
    SetFilePointer( hFile, 0, 0, FILE_BEGIN );
    return true;
} // FInStream::Reset

/*****************************************************************************/
/*    OutStream implementation
/*****************************************************************************/
OutStream::OutStream()
{
    bytesWritten = 0;
} // OutStream::OutStream

OutStream::~OutStream()
{
}

/*****************************************************************************/
/*    FOutStream implementation
/*****************************************************************************/
FOutStream::FOutStream()
{
    hFile = INVALID_HANDLE_VALUE;
}

FOutStream::FOutStream( const char* fname )
{
    hFile = INVALID_HANDLE_VALUE;
    OpenFile( fname );
} // FOutStream::FOutStream

FOutStream::~FOutStream()
{
    CloseFile();
}

bool FOutStream::OpenFile( const char* fname )
{
    IRM->CheckFileAccess(fname);
    hFile = CreateFile( fname, 
        FILE_WRITE_DATA, 
        FILE_SHARE_READ, 
        NULL,
        CREATE_ALWAYS, 
        FILE_ATTRIBUTE_NORMAL, 
        NULL );

    if (hFile == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    else
    {
        return true;
    }
} // FOutStream::OpenFile

bool FOutStream::CloseFile()
{
    if (hFile == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    BOOL res = CloseHandle( hFile );
    if (res == FALSE) 
    {
        return false;
    }
    hFile = INVALID_HANDLE_VALUE;
    return true;
} // FOutStream::CloseFile

DWORD FOutStream::OnWrite( const void* pBuf, DWORD nBytes )
{
    if (hFile == INVALID_HANDLE_VALUE) return false;
    DWORD wBytes;
    BOOL res = WriteFile( hFile, pBuf, nBytes, &wBytes, NULL );
    if (res == FALSE)
    {
        DWORD err = GetLastError();
        return 0;
    }
    return wBytes;
} // FInStream::OnWrite

/*****************************************************************************/
/*    ByteBuffer implementation
/*****************************************************************************/
ByteBuffer::ByteBuffer()
{
    data = NULL;
    size = 0;
}

ByteBuffer::~ByteBuffer()
{
    delete []data;
}

void ByteBuffer::Clear()
{
    delete []data;
    size = 0;
}

void ByteBuffer::Reserve( int _size )
{
    if (size == _size) 
    {
        assert( data );
        return;
    }
    delete []data;
    size = _size;
    data = new BYTE[size];
}

InStream& operator >>( InStream& is, ByteBuffer& buf )
{
    buf.Clear();
    int size = 0;
    is >> size;
    buf.Reserve( size );
    is.Read( buf.GetData(), size );
    return is;
} // operator >>

OutStream& operator <<( OutStream& os, const ByteBuffer& buf ) 
{
    os << buf.GetSize();
    os.Write( buf.GetData(), buf.GetSize() );
    return os;
} // operator <<

/*****************************************************************************/
/*    MemOutStream implementation
/*****************************************************************************/
DWORD MemOutStream::s_BufSize = 0;
DWORD MemOutStream::s_BufPos = 0;
BYTE* MemOutStream::s_pBuf = NULL;    

MemOutStream::MemOutStream( int maxSize )
{
    if (s_BufSize < maxSize || !s_pBuf)
    {
        delete []s_pBuf;
        s_pBuf = new BYTE[maxSize];
        s_BufSize = maxSize;
    }
    s_BufPos = 0;
}

DWORD MemOutStream::OnWrite( const void* pBuf, DWORD nBytes )
{
    if (nBytes > s_BufSize - s_BufPos) nBytes = s_BufSize - s_BufPos;
    memcpy( &s_pBuf[s_BufPos], pBuf, nBytes );
    s_BufPos += nBytes;
    return nBytes;
}

/*****************************************************************************/
/*    MemInStream implementation
/*****************************************************************************/
MemInStream::MemInStream() : m_BufPos(0)   
{
    m_pBuf      = MemOutStream::s_pBuf;
    m_BufSize   = MemOutStream::s_BufPos;
}

MemInStream::MemInStream( BYTE* pData, int size ) : m_BufPos(0)
{
    m_pBuf      = pData;
    m_BufSize   = size;
}

DWORD MemInStream::OnRead( void* buf, DWORD nBytes )
{
    if (nBytes > m_BufSize - m_BufPos) nBytes = m_BufSize - m_BufPos;
    memcpy( buf, &m_pBuf[m_BufPos], nBytes );
    m_BufPos += nBytes;
    return nBytes;
} // MemInStream::OnRead
