/*****************************************************************************/
/*    File:    kFilePath.cpp
/*    Desc:    File path manipulations    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    7.11.2004
/*****************************************************************************/
#include "stdafx.h"
#include "kFilePath.h"

FilePath GetCurrentPath()
{
    char cwd[_MAX_PATH];
    getcwd( cwd, _MAX_PATH );
    return FilePath( cwd ); 
}

/*****************************************************************************/
/*  FilePath implementation
/*****************************************************************************/
void FilePath::SetCWD() const
{
    chdir( GetDrive() );
    chdir( GetDir() );
} // FilePath::SetCurrent

void FilePath::GetCWD()
{
    getcwd( m_SourcePath, _MAX_PATH );
    SetPath( m_SourcePath );
} // FilePath::GetCurrent

void FilePath::ToLowercase()
{
    _strlwr( m_SourcePath );
    SetPath( m_SourcePath );
} // FilePath::ToLowercase

void FilePath::SetExt( const char* ext )
{
    if (!ext) ext = "";
    if (*ext != 0) 
    {
        m_Ext[0] = '.';
        strcpy( m_Ext + 1, ext );
    }
    else m_Ext[0] = 0;
    UpdatePath();
} // FilePath::SetExt

void FilePath::SetDir( const char* dir )
{
    char directory  [_MAX_PATH];
    char drive      [_MAX_PATH];
    char fileName   [_MAX_PATH];
    char extension  [_MAX_PATH];
    _splitpath( dir, drive, directory, fileName, extension );
    strcpy( m_Dir, directory );
    UpdatePath();
    if (drive[0] != 0) SetDrive( drive );
} // FilePath::SetDir

void FilePath::SetDrive( const char* drv )
{
    char directory  [_MAX_PATH];
    char drive      [_MAX_PATH];
    char fileName   [_MAX_PATH];
    char extension  [_MAX_PATH];
    _splitpath( drv, drive, directory, fileName, extension );
    strcpy( m_Drive, drive );
    UpdatePath();
} // FilePath::SetDrive

void FilePath::SetFileName( const char* fname )
{
    char directory  [_MAX_PATH];
    char drive      [_MAX_PATH];
    char fileName   [_MAX_PATH];
    char extension  [_MAX_PATH];
    _splitpath( fname, drive, directory, fileName, extension );
    strcpy( m_FileName, fileName );
    UpdatePath();
} // FilePath::SetFileName

void FilePath::AppendDir( const char* dir )
{
    char directory  [_MAX_PATH];
    char drive      [_MAX_PATH];
    char fileName   [_MAX_PATH];
    char extension  [_MAX_PATH];
    _splitpath( dir, drive, directory, fileName, extension );

    char* pEnd = m_Dir + strlen( m_Dir ) - 1;
    const char* pBeg = directory;
    while (*pEnd && (*pEnd == '\\' || *pEnd == '/')) pEnd--;
    pEnd++;
    while (*pBeg && (*pBeg == '\\' || *pBeg == '/')) pBeg++;
    strcpy( pEnd, "\\" );
    strcat( pEnd, pBeg );
    UpdatePath();
} // FilePath::AppendDir

void FilePath::SetPath( const char* path )
{
    strcpy( m_SourcePath, path );
    _splitpath( m_SourcePath, m_Drive, m_Dir, m_FileName, m_Ext );
    int fExtSz = strlen( m_FileName ) + strlen( m_Ext );
    m_pFileExt = m_SourcePath + strlen( m_SourcePath ) - fExtSz;

    DWORD attr = GetFileAttributes( m_SourcePath );
    if (DirectoryExists( m_SourcePath ))
    {
        strcat( m_Dir, "\\" );
        strcat( m_Dir, m_FileName );
        m_FileName[0] = 0;
    }
} // FilePath::SetPath

void FilePath::UpdatePath()
{
    _makepath( m_SourcePath, m_Drive, m_Dir, m_FileName, m_Ext );
    int fExtSz = strlen( m_FileName ) + strlen( m_Ext );
    m_pFileExt = m_SourcePath + strlen( m_SourcePath ) - fExtSz;
} // FilePath::UpdatePath

bool FilePath::Exists() const
{
    return FileExists( GetFullPath() );
} // FilePath::Exists