/*****************************************************************************/
/*    File:    kDirIterator.cpp
/*    Desc:    Directory tree manipulation class
/*    Author:    Ruslan Shestopalyuk
/*    Date:    06-11-2003
/*****************************************************************************/
#include "stdafx.h"
#include "kDirIterator.h"
#include <direct.h>

/*****************************************************************************/
/*    DirIterator implementation
/*****************************************************************************/
DirIterator::DirIterator()
{
    searchEnded = true;
}

DirIterator::DirIterator( const char* startDir )
{
    Reset( startDir );
} // DirIterator::DirIterator

void DirIterator::Reset( const char* startDir )
{
    char oldWD[_MAX_PATH];
    char startWD[_MAX_PATH];

    _getcwd( oldWD, _MAX_PATH );
    _chdir( startDir );
    _getcwd( startWD, _MAX_PATH );
    _chdir( oldWD );

    strcpy( m_StartDir, startWD );
    int dnlen = strlen( m_StartDir );
    if (m_StartDir[dnlen - 1] != '\\') strcat( m_StartDir, "\\" );
    _splitpath( m_StartDir, fileDrive, fileDir, fileName, fileExt ); 

    strcpy( m_StartDir, fileDrive );
    strcat( m_StartDir, fileDir    );
    strcpy( rootDir, m_StartDir );
    strcat( m_StartDir, "*.*" );

    searchEnded        = false;
    m_pEndFilter    = m_Filters;
    m_NFilters        = 0;
    searchHandle    = NULL;

    m_Filters    [0]    = '\0';
    fileName    [0]    = '\0';
    fileExt        [0]    = '\0';    
    fileDir        [0]    = '\0';    
    fileDrive    [0]    = '\0';
    fileNameExt    [0]    = '\0';

    Reset();
} // DirIterator::Reset

bool DirIterator::Reset()
{
    searchHandle = FindFirstFile( m_StartDir, &findData );
    if (searchHandle == INVALID_HANDLE_VALUE)
    {
        searchEnded = true;
        return false;
    }

    OnFoundFile();

    if (IsSkipped()) OnNextFile();
    return true;
} // DirIterator::Reset

bool DirIterator::IsFile() const
{
    if (searchEnded) return false;
    DWORD flags = findData.dwFileAttributes;
    if (flags & FILE_ATTRIBUTE_SYSTEM) return false;
    if (flags & FILE_ATTRIBUTE_DIRECTORY) return false;
    if (GetFileName()[0] == 0) return false;

    return true;
} // DirIterator::IsFile

bool DirIterator::IsDirectory() const
{
    return ( (findData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) != 0); 
} // DirIterator::IsDirectory

bool DirIterator::AddFilter( const char* _extF )
{
    int len = strlen( _extF ) + 1;
    if ((m_pEndFilter - m_Filters) + len >= c_MaxExtNamesLen) return false;
    strcpy( m_pEndFilter, _extF );
    m_NFilters++;

    if (IsSkipped() && !IsDirectory()) OnNextFile();
    return true;
} // DirIterator::AddFilter

DirIterator& DirIterator::operator++()
{
    OnNextFile();
    return *this;
} // DirIterator::operator++ prefix

DirIterator DirIterator::operator++( int )
{
    OnNextFile();
    return *this;
} // DirIterator::operator++ postfix

bool DirIterator::IsSkipped()
{
    if (searchEnded) return true;
    
    //  check if it is some special-case crap
    if (!strcmp( GetFileNameExt(), "" ) ||
        !strcmp( GetFileNameExt(), "." ) ||
        !strcmp( GetFileNameExt(), ".." )) return true;

    if (m_NFilters == 0) return false;
    for (int i = 0; i < m_NFilters; i++)
    {
        const char* pFilter = m_Filters;
        const char* pFileExt = fileExt;

        //  skip possible dots in extensions
        if (fileExt[0] == '.') pFileExt++;
        if (pFilter[0] == '.') pFilter++;

        if (!stricmp( pFilter, pFileExt )) return false;
        pFilter += strlen( pFilter );
    }
    return true;
} // DirIterator::IsSkipped

const char*    DirIterator::GetFullFilePath() const
{
    _makepath( fileFullPath, "", rootDir, fileName, fileExt );
    return fileFullPath;
}

const char*    DirIterator::GetFullFileName() const
{
    if (searchEnded) return NULL;
    return findData.cFileName;
} // DirIterator::GetFullFileName

const char*    DirIterator::GetFileName() const
{
    if (searchEnded) return NULL;
    return fileName;
} // DirIterator::GetFileName

const char*    DirIterator::GetFileNameExt() const
{
    if (searchEnded) return NULL;
    return fileNameExt;
} // DirIterator::GetFileNameExt

void DirIterator::OnNextFile()
{
    assert( searchHandle );
    searchEnded = false;

    while( true )
    {
        BOOL res = FindNextFile( searchHandle, &findData );
        if (res == 0)
        {
            searchEnded = true;
            FindClose( searchHandle );
            searchHandle = NULL;
            return;
        }
        OnFoundFile();
        if (!IsSkipped()) break;
    } 

    searchEnded = false;
} // DirIterator::OnNextFile

void DirIterator::OnFoundFile()
{
    if (searchEnded) return;
    const char* fullPath = findData.cFileName;
    _splitpath( fullPath, fileDrive, fileDir, fileName, fileExt );

    strcpy( fileNameExt, fileName );
    strcat( fileNameExt, fileExt );

} // DirIterator::OnFoundFile

/*****************************************************************************/
/*    DirTreeIterator implementation
/*****************************************************************************/
DirTreeIterator::DirTreeIterator( const char* startDir )
{
    m_NFilters      = 0;
    m_pEndFilter    = m_Filters;
    Reset( startDir );   
} // DirTreeIterator::DirTreeIterator

DirTreeIterator::DirTreeIterator()
{
    m_StartDir[0]   = 0;
    m_Depth         = 0;
    m_pIterator     = NULL;
    m_NFilters      = 0;  
    m_pEndFilter    = m_Filters;
} // DirTreeIterator::DirTreeIterator

DirTreeIterator& DirTreeIterator::operator++()
{
    if (!m_pIterator) PushIterator( m_StartDir );

    do{
        if (m_pIterator->IsDirectory()) PushIterator( m_pIterator->GetFullFilePath() );
        ++(*m_pIterator);
        while (!(*m_pIterator)) 
        {
            PopIterator();
            if (m_pIterator) 
            {
                ++(*m_pIterator); 
            }else 
            {
                return *this;
            }
        }
    } while (m_pIterator && IsSkipped());
    return *this;
} // DirTreeIterator::operator++

void DirTreeIterator::Reset( const char* startDir )
{
    strcpy( m_StartDir, startDir );
    m_Depth      = 0;
    m_pIterator  = NULL;
    operator ++();
} // DirTreeIterator::Reset

bool DirTreeIterator::AddFilter( const char* m_Filters )
{
    int len = strlen( m_Filters ) + 1;
    if ((m_pEndFilter - m_Filters) + len >= c_MaxExtNamesLen) return false;
    strcpy( m_pEndFilter, m_Filters );
    m_NFilters++;
    
    return true;
} // DirTreeIterator::AddFilter

void DirTreeIterator::PushIterator( const char* path )
{
    m_pIterator = &m_DStack[m_Depth++];
    m_pIterator->Reset( path );
} // DirTreeIterator::PushIterator

void DirTreeIterator::PopIterator()
{
    if (m_Depth == 0) 
    {
        m_pIterator = NULL;
        return;
    }
    m_Depth--;
    m_pIterator = &m_DStack[m_Depth - 1];
} // DirTreeIterator::PopIterator

bool DirTreeIterator::IsSkipped()
{
    if (!m_pIterator) return true;
    const char* pFileExt = m_pIterator->GetFileExt();
    if (pFileExt[0] == '.') pFileExt++;

    if (m_NFilters == 0) return false;
    for (int i = 0; i < m_NFilters; i++)
    {
        const char* pFilter = m_Filters;
        //  skip possible dots in extensions
        if (pFilter[0] == '.') pFilter++;

        if (!stricmp( pFilter, pFileExt )) return false;
        pFilter += strlen( pFilter );
    }
    return true;
} // DirTreeIterator::IsSkipped
