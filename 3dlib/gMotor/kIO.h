/*****************************************************************************/
/*    File:    kIO.h
/*    Desc:    Input-output routines    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    20.02.2003
/*****************************************************************************/
#ifndef __KIO_H__
#define __KIO_H__

/*****************************************************************************/
/*    Class:    InStream
/*    Desc:    Generic write to storage input interface
/*****************************************************************************/
#ifndef DIALOGS_API
#define DIALOGS_API
#endif
class DIALOGS_API InStream
{    
public:
    InStream();
    virtual            ~InStream();

    DWORD Read( void* buf, DWORD nBytes )
    {
        DWORD realBytes = OnRead( buf, nBytes );
        bytesRead += realBytes;
        return realBytes;
    }

    void Skip( DWORD nBytes )
    {
        OnSkip( nBytes );
        bytesRead += nBytes;
    }

    virtual bool Close() = 0;
    virtual bool Reset() { return false; }

    _inl InStream&    operator >>( float&        val );
    _inl InStream&    operator >>( int&        val );
    _inl InStream&    operator >>( bool&        val );
    _inl InStream&    operator >>( double&    val );
    _inl InStream&    operator >>( DWORD&        val );
    _inl InStream&    operator >>( WORD&        val );
    _inl InStream&    operator >>( BYTE&        val );

    _inl            operator bool() const;

    virtual int GetTotalSize() = 0;


protected:

    _inl int        GetTotalBytesRead() const;

    virtual bool    IsEndOfStream()    const                    = 0;
    virtual DWORD    OnRead( void* buf, DWORD nBytes )        = 0;
    virtual void    OnSkip( DWORD nBytes ){}

private:
    DWORD            bytesRead;
}; // class InStream
/*****************************************************************************/
/*    Class:    FInStream
/*    Desc:    File input stream
/*****************************************************************************/
class DIALOGS_API FInStream : public InStream
{
public:
                        FInStream();
                        FInStream( const char* fname );
    virtual             ~FInStream();
    
    virtual bool        IsEndOfStream() const;
    virtual DWORD       OnRead      ( void* buf, DWORD nBytes );
    virtual void        OnSkip      ( DWORD nBytes );


    bool                OpenFile    ( const char* fname );
    virtual bool        Close       ();
    int                 GetFileSize () const { return fileSize; }
    virtual int         GetTotalSize() { return fileSize; }
    virtual bool        Reset       ();

    _inl bool           NoFile      () const;

private:
    HANDLE              hFile;
    DWORD               fileSize;
}; // class FInStream

/*****************************************************************************/
/*    Class:    OutStream
/*    Desc:    Generic write to storage output interface
/*****************************************************************************/
class DIALOGS_API OutStream
{    
public:
    OutStream();
    virtual            ~OutStream();

    _inl DWORD        Write( const void* data, DWORD nBytes )
    {
        int realBytes = OnWrite( data, nBytes );
        bytesWritten += realBytes;
        return realBytes;
    }

    _inl OutStream& operator <<( float    val );
    _inl OutStream& operator <<( int    val );
    _inl OutStream& operator <<( bool    val );
    _inl OutStream& operator <<( double val );
    _inl OutStream& operator <<( DWORD  val );
    _inl OutStream& operator <<( WORD   val );
    _inl OutStream& operator <<( BYTE    val );
    _inl OutStream& operator <<( const char*  val );

protected:

    virtual    DWORD    OnWrite( const void* pBuf, DWORD nBytes ) = 0;

    DWORD            bytesWritten;
}; // class OutStream

/*****************************************************************************/
/*    Class:    FOutStream
/*    Desc:    File output stream
/*****************************************************************************/
class DIALOGS_API FOutStream : public OutStream
{
public:
                    FOutStream();
                    FOutStream( const char* fname );
    virtual            ~FOutStream();
    virtual DWORD    OnWrite( const void* pBuf, DWORD nBytes );
    bool            OpenFile( const char* fname );
    bool            CloseFile();

    _inl bool        NoFile() const;

private:
    HANDLE            hFile;
}; // class FOutStream

/*****************************************************************************/
/*    Class:    CountStream
/*    Desc:    Stream, which only counts number of bytes, written by the object
/*****************************************************************************/
class DIALOGS_API CountStream : public OutStream
{
public:
    CountStream() : totalBytes( 0 ) {}
    virtual DWORD    OnWrite( const void* pBuf, DWORD nBytes ) { totalBytes += nBytes; return nBytes; }
    
    _inl DWORD        GetNBytes() const { return totalBytes; }
    _inl bool        Reset() { totalBytes = 0; return true; }

private:
    DWORD            totalBytes;
}; // class CountStream

/*****************************************************************************/
/*    Class:    MemOutStream
/*    Desc:    out stream for temporary holding memory buffer
/*****************************************************************************/
class DIALOGS_API MemOutStream : public OutStream
{
public:
                    MemOutStream    ( int maxSize );
    virtual DWORD    OnWrite         ( const void* pBuf, DWORD nBytes );

protected:
    static DWORD    s_BufSize;
    static DWORD    s_BufPos;
    static BYTE*    s_pBuf;    

    friend class MemInStream;
}; // class MemOutStream

/*****************************************************************************/
/*    Class:    MemInStream
/*    Desc:    in stream for temporary holding memory buffer
/*****************************************************************************/
class DIALOGS_API MemInStream : public InStream
{
public:
                    MemInStream     ();
                    MemInStream     ( BYTE* pData, int size );
    virtual bool    IsEndOfStream   () const 
    { 
        return (m_BufPos >= m_BufSize); 
    }
    virtual DWORD    OnRead          ( void* buf, DWORD nBytes );
    virtual bool    Close           () { return false; }
    virtual void    OnSkip          ( DWORD nBytes ) { m_BufPos += nBytes; }
    virtual int     GetTotalSize    () { return MemOutStream::s_BufPos; }
    virtual bool    Reset           () { m_BufPos = 0; return true; }

protected:
    DWORD            m_BufPos;
    DWORD           m_BufSize;
    BYTE*           m_pBuf;
}; // class MemInStream

/*****************************************************************************/
/*    Class:    MediaChunk
/*    Desc:    chunk loading wrapper
/*****************************************************************************/
class MediaChunk
{
    DWORD        size;
    DWORD        curPosInBlock;
    char        strMagic[5];
    ResFile        resFile;
public:
    _inl                MediaChunk();    
    _inl bool            ReadHeader( ResFile rf );
    _inl DWORD            Skip();
    _inl DWORD            Skip( int nBytes );
    _inl DWORD            Read( void* buf, int sizeBytes );

    _inl const char*    GetMagic()        const;
    _inl DWORD            GetMagicDW()    const;
    _inl int            GetFullSize()    const;
    _inl int            GetBytesLeft()    const;
    _inl int            GetDataSize()    const;
    
}; // class MediaChunk

/*****************************************************************************/
/*    Enum:    MFLError
/*    Desc:    media file loading error codes    
/*****************************************************************************/
enum MFLError
{
    mfleNoError            = 0,
    mfleBadChunk        = 1,
    mfleEndFile            = 2,
    mfleFileOpenError    = 3
}; // enum MFLError

/*****************************************************************************/
/*    Class:    MediaFileLoader
/*    Desc:    Loads chunked media file block-by-block
/*****************************************************************************/
class MediaFileLoader
{
    ResFile                resFile;
    int                    fileSize;
    int                    curFilePos;

    MFLError            errorStatus;
    MediaChunk            curChunk;

public:
    _inl                MediaFileLoader( ResFile* rf );
    _inl                MediaFileLoader( const char* fName );
    _inl                ~MediaFileLoader();
    _inl DWORD            Read( void* buf, int sizeBytes );
    
    _inl MFLError        Error();
    _inl bool            NextChunk();
    _inl DWORD            CurChunkSkip();
    _inl DWORD            CurChunkSkip( DWORD nBytes );

    _inl const MediaChunk& GetCurChunk() const { return curChunk; }

protected:
    _inl ResFile        GetFileHandle() const { return resFile; }
}; // class MediaFileLoader

/*****************************************************************************/
/*    Class:    MagicWord
/*    Desc:    Stays for magic FOURCC id
/*****************************************************************************/
class MagicWord
{
    BYTE            mb[4];
public:
    _inl            MagicWord( const char* wstr );

    _inl int        write    ( BYTE* buf );
    _inl int        write    ( FILE* fp );
    _inl int        read    ( BYTE* buf );
    _inl int        read    ( FILE* fp );
    _inl int        size    () const { return sizeof(DWORD); }

}; // class MagicWord

/*****************************************************************************/
/*    Class:    stcio
/*    Desc:    io wrapper class for standard types
/*****************************************************************************/
class stcio
{
public:
    _inl static int read( BYTE* buf, int& val );
    _inl static int read( FILE* fp,  int& val );

    _inl static int read( BYTE* buf, float& val );
    _inl static int read( FILE* fp,  float& val );

    _inl static int read( BYTE* buf, DWORD& val );
    _inl static int read( FILE* fp,  DWORD& val );

    _inl static int read( BYTE* buf, char* str, int numCh );
    _inl static int read( FILE* fp,  char* str, int numCh );


    _inl static int write( BYTE* buf, int& val );
    _inl static int write( FILE* fp,  int& val );

    _inl static int write( BYTE* buf, float& val );
    _inl static int write( FILE* fp,  float& val );

    _inl static int write( BYTE* buf, DWORD& val );
    _inl static int write( FILE* fp,  DWORD& val );

    _inl static int write( BYTE* buf, char* str );
    _inl static int write( FILE* fp,  char* str );

}; // class stcio

/*****************************************************************************/
/*    Class:    ByteBuffer
/*    Desc:    Simple binary buffer
/*****************************************************************************/
class ByteBuffer
{
    BYTE*        data;
    int            size;

public:
                ByteBuffer();
                ~ByteBuffer();

    bool        IsEmpty() const { return (data == NULL); }
    BYTE*        GetData() const { return data; }
    int            GetSize() const { return size; }
    void        Clear();
    void        Reserve( int _size );
}; // class ByteBuffer

/*****************************************************************************/
/*    Class:    StackBuffer
/*    Desc:    Byte buffer allocated in the stack
/*****************************************************************************/
class StackBuffer
{
    BYTE*        m_Data;
    int            m_Size;

public:
    StackBuffer( int size );
    ~StackBuffer() {}

    BYTE*        GetData() const { return m_Data; }
    int            GetSize() const { return m_Size; }
}; // class StackBuffer

InStream&    operator >>( InStream& val, ByteBuffer& buf );
OutStream&    operator <<( OutStream& val, const ByteBuffer& buf );

void    EnsureLastSlash ( char* path );
BYTE*    LoadFile        ( const char* path, DWORD& bufLen );
bool    LocateFile      ( const char* fname, char* root );
bool    FileExists      ( const char* fname );
bool    DirectoryExists ( const char* fname );

#define MAGIC_FOURCC(A) ((A << 24) | ((A & 0x0000FF00) << 8) | ((A & 0x00FF0000) >> 8) | ((A & 0xFF000000) >> 24)) 

#ifdef _INLINES
#include "kIO.inl"
#endif // _INLINES

#endif // __KIO_H__