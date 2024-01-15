/*****************************************************************************/
/*    File:    kSystemDialogs.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    25.02.2003
/*****************************************************************************/
#ifndef __KSYSTEMDIALOGS_H__
#define __KSYSTEMDIALOGS_H__

#include "commdlg.h"

const int c_MaxFilterLen = 512; 
/*****************************************************************************/
/*    Class:    FileDialog
/*    Desc:    General file path dialog
/*****************************************************************************/
class FileDialog
{
public:
                            FileDialog();
    virtual bool            Show() = 0;
    const char*                GetFilePath();
    void                    AddFilter( const char* descr, const char* filter );
    void                    SetDefaultExtension( const char* ext );

protected:
    OPENFILENAME            m_Ofs;
    char                    m_FileName    [_MAX_PATH];
    char                    m_DefExt    [_MAX_EXT ];

    char                    m_Filter    [c_MaxFilterLen];
    int                        m_nFilterChars;

}; // class FileDialog

/*****************************************************************************/
/*    Class:    OpenFileDialog 
/*    Desc:    System 'open file' dialog
/*****************************************************************************/
class OpenFileDialog : public FileDialog
{
public:
                            OpenFileDialog();
                            ~OpenFileDialog();

    virtual bool            Show();

}; // class OpenFileDialog

/*****************************************************************************/
/*    Class:    SaveFileDialog 
/*    Desc:    System 'save file' dialog
/*****************************************************************************/
class SaveFileDialog : public FileDialog
{
public:
                            SaveFileDialog();
                            ~SaveFileDialog();

    virtual    bool            Show();
}; // class SaveFileDialog

/*****************************************************************************/
/*    Class:    PickColorDialog 
/*    Desc:    System 'pick color' dialog
/*****************************************************************************/
class PickColorDialog
{
    CHOOSECOLOR        m_ChooseColor;

public:
    PickColorDialog( DWORD color = 0xFFFFFFFF );
    ~PickColorDialog();

    bool            Show();
    DWORD            GetColor() const { return FromColorRef( m_ChooseColor.rgbResult ); }
    void            SetColor( DWORD color ) { m_ChooseColor.rgbResult = ToColorRef( color ); }


    static DWORD    FromColorRef( COLORREF    clr );
    static COLORREF    ToColorRef    ( DWORD        clr );

    typedef UINT_PTR (CALLBACK *LPCCHOOKPROC) (HWND, UINT, WPARAM, LPARAM);

    void            SetHookProcedure( LPCCHOOKPROC proc );

protected:
    static COLORREF    s_CustomColors[16];

}; // class PickColorDialog

void RelaxDialog();

#endif // __KSYSTEMDIALOGS_H__
