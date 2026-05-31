/*****************************************************************************/
/*    File:    kFileMapping.cpp
/*    Desc:    Wrapper for memory-mapping of the files
/*    Author:    Ruslan Shestopalyuk
/*    Date:    12-03-2004
/*****************************************************************************/
#include "stdafx.h"
#include "kFileMapping.h"

/*****************************************************************************/
/*    FileMapping implementation
/*****************************************************************************/
FileMapping::~FileMapping()
{
}

void FileMapping::FixMapName( char* name )
{
    while (*name)
    {
        if (*name == '\\' || *name == '/') *name = '_';
        name++;
    }
} // FileMapping::FixMapName

const int c_TouchStep = 4096;
BYTE FileMapping::TouchData()
{
    m_pData = GetPointer();
    if (!m_pData) return 0;
    const BYTE* pData = m_pData;
    BYTE res = 0;
    int pos = 0;
    while (pos < m_FileSize)
    {
        res += m_pData[pos];
        pos += c_TouchStep;
    }

    return res;
} // FileMapping::TouchData

BYTE* FileMapping::GetPointer()
{
    if (m_pData) return m_pData;
    if (m_hFile == hInvalid)
    {
        m_hFile = CreateFile( m_FileName, GENERIC_READ, FILE_SHARE_READ, 0, 
                                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
        if (m_hFile == hInvalid) 
        {
            DWORD errCode = GetLastError();
            return NULL;
        }
        m_FileSize = ::GetFileSize( m_hFile, NULL );
    }
    
    if (m_hMap == hInvalid)
    {
        char mapName[_MAX_PATH];
        strcpy( mapName, m_FileName );
        FixMapName( mapName );
        m_hMap = CreateFileMapping( m_hFile, NULL, PAGE_READONLY, 0, 0, mapName );
        DWORD err = GetLastError();
        if (m_hMap == hInvalid) return NULL;
        m_pData = (BYTE*)MapViewOfFile( m_hMap, FILE_MAP_READ, 0, 0, 0 );
        CloseHandle( m_hFile );
        m_hFile = hInvalid;
    }
    return m_pData;
} // FileMapping::GetPointer

void FileMapping::Close()
{
    UnmapViewOfFile( m_pData );
    m_pData = NULL;
    CloseHandle( m_hMap );
    CloseHandle( m_hFile );
    m_hMap = hInvalid;
    m_hFile = hInvalid;
} // FileMapping::Close