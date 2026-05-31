/*****************************************************************************/
/*    File:    kFilePath.h
/*    Desc:    File path manipulations    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    7.11.2004
/*****************************************************************************/
#ifndef __KFILEPATH_H__
#define __KFILEPATH_H__

/*****************************************************************************/
/*    Class:    FilePath
/*    Desc:    File path class
/*****************************************************************************/
class FilePath
{    
    char            m_SourcePath    [_MAX_PATH];
    char            m_Drive         [_MAX_DRIVE];
    char            m_Dir           [_MAX_DIR];
    char            m_FileName      [_MAX_FNAME];
    char            m_Ext           [_MAX_EXT];
    
    char*           m_pFileExt;

public:
                FilePath        ()                      { SetPath( "" ); }
                FilePath        ( const char* path )    { SetPath( path ); }
    operator const char*        () const { return m_SourcePath; }
    const char* GetFileName     () const { return m_FileName; }
    const char* GetFullPath     () const { return m_SourcePath; }
    const char* GetFileWithExt  () const { return m_pFileExt; }
    const char* GetDir          () const { return m_Dir; }
    const char* GetExt          () const { return m_Ext[0] ? m_Ext + 1 : ""; }
    const char* GetDrive        () const { return m_Drive; }
    void        ToLowercase     ();     
    void        SetExt          ( const char* ext );
    void        SetDir          ( const char* dir );
    void        SetDrive        ( const char* drive );
    void        SetFileName     ( const char* fname );
    void        AppendDir       ( const char* dir );
    void        SetPath         ( const char* path );
    bool        HasExt          ( const char* ext ) const { return !stricmp( ext, GetExt() ); }
    bool        Exists          () const;
    void        SetCWD          () const;
    void        GetCWD          ();

private:
    void UpdatePath             ();
}; // class FilePath

FilePath GetCurrentPath();


#endif // __KFILEPATH_H__