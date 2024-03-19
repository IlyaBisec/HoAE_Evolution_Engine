/*****************************************************************************/
/*    File:    kFileMapping.h
/*    Desc:    Wrapper for memory-mapping of the files
/*    Author:    Ruslan Shestopalyuk
/*    Date:    12-03-2004
/*****************************************************************************/
#ifndef __KFILEMAPPING_H__
#define __KFILEMAPPING_H__

const HANDLE hInvalid = INVALID_HANDLE_VALUE;

/*****************************************************************************/
/*    Class:    FileMapping
/*    Desc:    Wraps memory mapping of the file
/*****************************************************************************/
class FileMapping
{
    char                m_FileName[_MAX_PATH];        //  path on disk
    HANDLE                m_hFile;                    //  handle of the opened file
    HANDLE                m_hMap;                        //  handle to the mapping object
    BYTE*                m_pData;                    //  pointer to the address in the virtual memory
    DWORD                m_FileSize;

public:
                    FileMapping() : m_hFile(hInvalid), m_hMap(hInvalid), m_pData(NULL), m_FileSize(0) 
                    {
                        m_FileName[0] = 0;
                    } 
                    FileMapping( const char* fileName ) :    m_hFile(hInvalid), m_hMap(hInvalid), m_pData(NULL) 
                    {
                        strcpy( m_FileName, fileName );
                    }
                    
                    ~FileMapping();

    BYTE*            GetPointer    ();
    const char*        GetFileName    () const { return m_FileName; }
    DWORD            GetFileSize    () const { return m_FileSize; }
    void            Close        ();
    BYTE            TouchData    ();

protected:
    void            FixMapName( char* name );

}; // class FileMapping

#endif // __KFILEMAPPING_H__