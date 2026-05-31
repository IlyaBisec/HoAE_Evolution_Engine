/*****************************************************************************/
/*    File:    kDirIterator.h
/*    Desc:    Directory tree manipulation class
/*    Author:    Ruslan Shestopalyuk
/*    Date:    06-11-2003
/*****************************************************************************/
#ifndef __KDIREITERATOR_H__
#define __KDIREITERATOR_H__

const int c_MaxExtNamesLen = _MAX_EXT*16;
/*****************************************************************************/
/*    Class:    DirIterator
/*    Desc:    Allows searching in the directories
/*    Example: 
/*            
/*            DirIterator it( "c:\\images" );
/*            it.AddFilter( "jpg" );
/*            it.AddFilter( "bmp" );
/*            it.AddFilter( "tga" );
/*
/*            while (it)
/*            {
/*                const char* fname = it.GetFullFileName();
/*                printf( "%s\n", fname );
/*                it++;
/*            }
/*
/*****************************************************************************/
class DirIterator
{
    char                m_StartDir[_MAX_PATH];
    char                m_Filters[c_MaxExtNamesLen];
    char*                m_pEndFilter;
    int                    m_NFilters;

    bool                searchEnded;
    WIN32_FIND_DATA        findData;
    HANDLE                searchHandle;

    char                fileName    [_MAX_FNAME];
    char                fileExt        [_MAX_EXT];
    char                fileDir        [_MAX_DIR];
    char                fileDrive    [_MAX_DRIVE];

    char                fileNameExt    [_MAX_PATH];
    char                rootDir        [_MAX_PATH];
    mutable char        fileFullPath[_MAX_PATH];

public:
                        DirIterator        ();
                        DirIterator        ( const char* m_StartDir );
    void                Reset           ( const char* m_StartDir );
    bool                AddFilter        ( const char* m_Filters );
    const char*            GetFullFileName    () const;
    const char*            GetFullFilePath    () const;
    const char*            GetFileName        () const;
    const char*            GetFileExt        () const { return fileExt + 1; }
    DWORD                GetFileSize        () const { return findData.nFileSizeLow; }
    const char*            GetFileNameExt    () const;
    bool                Reset            ();
    bool                IsFile            () const;
    bool                IsDirectory        () const;
    DirIterator&        operator++        ();            // Prefix increment operator.
    DirIterator            operator++        ( int );    // Postfix increment operator.
    operator            bool            () const { return !searchEnded; }

protected:
    void                OnFoundFile        ();
    void                OnNextFile        ();
    bool                IsSkipped        ();

}; // class DirIterator

const int c_MaxDirNesting = 64;
/*****************************************************************************/
/*    Class:    DirTreeIterator
/*    Desc:    Works like DirIterator, but iterates into sub-directories
/*****************************************************************************/
class DirTreeIterator
{
    char                m_StartDir[_MAX_PATH];
    DirIterator         m_DStack[c_MaxDirNesting];
    int                 m_Depth;
    DirIterator*        m_pIterator;

    char                m_Filters[c_MaxExtNamesLen];
    char*                m_pEndFilter;
    int                    m_NFilters;

public:
                        DirTreeIterator    ( const char* startDir );
                        DirTreeIterator    ();

    bool                AddFilter        ( const char* m_Filters );
    DirTreeIterator&    operator++        ();            // Prefix increment operator.
    void                Reset           ( const char* startDir );

    const char*            GetFullFileName    () const { return m_pIterator ? m_pIterator->GetFullFileName() : NULL; }
    const char*            GetFullFilePath    () const { return m_pIterator ? m_pIterator->GetFullFilePath() : NULL; }
    const char*            GetFileName        () const { return m_pIterator ? m_pIterator->GetFileName() : NULL; }
    const char*            GetFileExt        () const { return m_pIterator ? m_pIterator->GetFileExt() : NULL; }
    DWORD                GetFileSize        () const { return m_pIterator ? m_pIterator->GetFileSize() : NULL; }
    const char*            GetFileNameExt    () const { return m_pIterator ? m_pIterator->GetFileNameExt() : NULL; }
    operator            bool            () const { return m_pIterator ? (bool)(*m_pIterator) : false; }
    
private:
    void                PushIterator    ( const char* path );
    void                PopIterator     ();
    bool                IsSkipped        ();

}; // class DirTreeIterator

#endif // __KDIREITERATOR_H__
